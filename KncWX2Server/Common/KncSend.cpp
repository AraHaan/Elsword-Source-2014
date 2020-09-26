#include <WinSock2.h>

#include "KncSend.h"

#include "DBLayer.h"
#include "BaseServer.h"
#include "ProxyManager.h"
#include "ActorManager.h"
#include "Room/RoomManager.h"

int CompareServerLevel( DWORD dwDestPI, DWORD dwCurrentPI )
{
    // 0���� ū ��      : �������� �� ���� ����
    // 0���� ���� ��    : �������� �� ���� ����
    // 0                : ���� ����

    return (dwDestPI & SC_MASK_BIT) - (dwCurrentPI & SC_MASK_BIT);
}

DWORD GetServerClass( DWORD dwPI )
{
    return dwPI & SC_MASK_BIT;
}

DWORD GetPerformerClass( DWORD dwPI )
{
    return dwPI & PC_MASK_BIT;
}

//{{ 2010. 05. 10  ������	�������� ������ ����
DWORD GetSendType( DWORD dwSt )
{
	return dwSt & ST_MASK_BIT;
}
//}}

void KncSend( DWORD dwPIFrom_, UidType nFrom_, KEventPtr spEvent_ )
{
    int iEval = CompareServerLevel( spEvent_->m_kDestPerformer.m_dwPerformerID, dwPIFrom_ );

    // ���� ������ ������ ������ ��� 
    if( iEval == 0 || GetServerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) == 0 )
    {
        const std::set<UidType>& setUID = spEvent_->m_kDestPerformer.GetUidListReference();
        std::set<UidType>::const_iterator sit;

        switch( GetPerformerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) )
        {
        case PC_SERVER:
            
            if( spEvent_->m_kDestPerformer.FindUID( KBaseServer::GetKObj()->GetUID() ) )
            {
                KBaseServer::GetKObj()->QueueingEvent( spEvent_ );
            }
            else
            {
                SiKProxyManager()->SendPacket( nFrom_, *spEvent_ );
            }
            break;
        case PC_ACCOUNT_DB:
        case PC_GAME_DB:
        case PC_GAME_DB_2ND:
        case PC_LOG_DB:
        case PC_LOG_DB_2ND:
		case PC_SMS_DB:	//sms�� ���Ϸ��������� ����ϰ� ��..
        case PC_NX_WEB_DB:
		case PC_CHAT_LOG_DB:				// SERV_RECORD_CHAT
		case PC_KOG_BILLING_DB:				// SERV_GLOBAL_BILLING
		case PC_PUBLISHER_BILLING_DB:		// SERV_GLOBAL_BILLING
		case PC_ID_PCBANG_AUTH_DB:				// SERV_ID_NETMARBLE_PCBANG
			//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
			//#ifdef SERV_RELAY_DB_CONNECTION
		case PC_JP_RELAY_DB:				// �Ϻ� �̺�Ʈ �߰�DB
			//#endif SERV_RELAY_DB_CONNECTION
			//}}
		case PC_EVENT_DB:					// SERV_ADD_EVENT_DB
		case PC_PUBLISHER_AUTH_DB:			// SERV_GLOBAL_AUTH , SERV_COUNTRY_IN
		case PC_SCRIPT_DB:					// SERV_ADD_SCRIPT_DB
            SiKDBLayer()->QueueingEvent( spEvent_ );
            break;
        case PC_USER:
            for( sit = setUID.begin(); sit != setUID.end(); sit++ )
            {
                KActorPtr spActor = KActorManager::GetKObj()->Get( *sit );
                if( spActor ) // ���� �������� �̺�Ʈ ���� ����� ã��.
                {
                    spActor->QueueingEvent( spEvent_->Clone() );
                }
                else if( !spEvent_->IsEmptyTrace() )    // ���ÿ� ����� ������ trace�� ���� - �ٸ� ������ routing.
                {
                    SiKProxyManager()->SendPacket( nFrom_, *spEvent_ );
                }
            }
            break;
        case PC_CHARACTER:
            for( sit = setUID.begin(); sit != setUID.end(); sit++ )
            {
                if( KActorManager::GetKObj()->FindCID( *sit ) )     // ���� �������� �̺�Ʈ ���� ����� ã��
                {
                    KActorManager::GetKObj()->QueueingByCID( *sit, spEvent_->Clone() );
                }
                else if( !spEvent_->IsEmptyTrace() )    // ���ÿ� ����� ������ trace�� ���� - �ٸ� ������ routing.
                {
                    SiKProxyManager()->SendPacket( nFrom_, *spEvent_ );
                }
            }
            break;
		case PC_ROOM:
			if( GetServerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) == SC_GAME )
			{
				if( KRoomManager::GetKObj()  != NULL )
					KRoomManager::GetKObj()->QueueingEventToRoom( spEvent_ );
			}
			break;
        default:
            START_LOG( cerr, L"�۽� �������� ������ �ų�." )
                << BUILD_LOG( dwPIFrom_ )
                << BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( spEvent_->m_usEventID )
                << BUILD_LOG( spEvent_->GetIDStr() )
                << END_LOG;
            break;
        }
    }
    else if( iEval < 0 )
    {
        // ���� ������ ������ �̺�Ʈ�̴�.
        if( GetPerformerClass( dwPIFrom_ ) == PC_USER )
        {
            START_LOG( cerr, L"SendPacket�� ���." )
                << BUILD_LOG( dwPIFrom_ )
                << BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( spEvent_->m_usEventID )
                << BUILD_LOG( spEvent_->GetIDStr() )
                << END_LOG;
            return;
        }

        if( spEvent_->IsEmptyTrace() )
        {
            START_LOG( cerr, L"���� ������ �ϳ�.." )
                << BUILD_LOG( dwPIFrom_ )
                << BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( spEvent_->m_usEventID )
                << BUILD_LOG( spEvent_->GetIDStr() )
                << END_LOG;
            return;
        }

        //START_LOG( clog, L"Trace ������" )
        //    << BUILD_LOG( spEvent_->m_vecTrace.size() );
        UidType nTo = spEvent_->GetLastSenderUID();
        if( nTo == 0 )
        {
            //START_LOG( clog, L"��ε�ĳ����" );
            KActorManager::GetKObj()->QueueingToAll( spEvent_ );
        }
        else
        {
            //START_LOG( clog, L"�ϳ�����" )
            //    << BUILD_LOG( nTo );
            KActorManager::GetKObj()->QueueingTo( nTo, spEvent_ );
        }
    }
    else // ( iEval > 0 ) ���� ������ ������ �̺�Ʈ�̴�.
    {
        SiKProxyManager()->SendPacket( nFrom_, *spEvent_ );
    }

    return;
}
