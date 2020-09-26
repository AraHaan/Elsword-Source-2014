#include "LoginProxy.h"
#include "NetError.h"

#include "GSUser.h"
#include "ActorManager.h"
#include "ProxyManager.h"
#include "KncSend.h"
#include "GameServer.h"
#include "DBLayer.h"
#include "RoomListManager.h"

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

//{{ 2011. 12. 14	������	��Ŷ ó�� ���� �ð� üũ
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
	#include "PacketMornitoring/PacketMornitoring.h"
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

#ifdef DEF_TRADE_BOARD
	#include "PersonalShopItemList.h"
#endif DEF_TRADE_BOARD

NiImplementRTTI( KLoginProxy, KActorProxy );
//ImplementProxy( KLoginProxy );
ImplPfID( KLoginProxy, PI_LOGIN_PROXY );

#define CLASS_TYPE KLoginProxy

KLoginProxy::KLoginProxy(void)
{
}

KLoginProxy::~KLoginProxy(void)
{
}

ImplToStringW( KLoginProxy )
{
	return START_TOSTRING_PARENTW( KActorProxy )
		<< TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
		<< TOSTRINGW( GetPort() )
		<< TOSTRINGWb( IsConnected() );
}

// Call to ProcessEventQueue (Client)
void KLoginProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
	if( RoutePacket( spEvent_ ) )
	{
		return;
	}

	switch( spEvent_->m_usEventID )
	{
		//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
		//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
	   _CASE( ELG_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
		//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
		//}}
        CASE_NOPARAM( E_RESERVE_DESTROY );
	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ��� - " << spEvent_->GetIDStr() );
	}
}

void KLoginProxy::OnDestroy()
{
	KActorProxy::OnDestroy();

	SiKProxyManager()->DestroyProxy( KProxyManager::PT_LOGIN, GetProxyID() );

	//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	if( GetUID() != 0 )
	{
#ifdef DEF_TRADE_BOARD
		SiKPersonalShopItemList()->DelItemInfo( SEnum::PSHOP_AGENCY_PROXY_ID );
#endif DEF_TRADE_BOARD
	}
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2009. 1. 30  ������	��ü �Ҹ� �α�
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"���� �˴ٿ����� ���� LoginProxy��ü �Ҹ�" )
			<< BUILD_LOG( GetProxyID() );
	}
	else
	{
		START_LOG( cout2, L"���������� �������� ���� LoginProxy��ü �Ҹ�" )
			<< BUILD_LOG( GetProxyID() )
			<< BUILD_LOG( GetDisconnectReason() )			
			<< dbg::tab << L"���� ���� : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl
			<< BUILD_LOG( GetLastIocpFailedError() );

		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetProxyID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}

		//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
		{
			KE_DISCONNECT_SERVER_REPORT_NOT kNot;
			kNot.m_cType = KMornitoringManager::ET_DISCONNECT_GAME_LOGIN_PROXY;
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
#endif SERV_MORNITORING
		//}}
	}
	//}}
}

//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
_IMPL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK )
{
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nOK );

	m_nAckOK = kPacket_.m_nOK;
	if( m_nAckOK == NetError::NET_OK )
	{
		KBaseServer::GetKObj()->SetUID( kPacket_.m_nUID );

		SetUID( kPacket_.m_iServerUID );

		//{{ 2010. 04. 16  ������	�α� �߰�		
		START_LOG( cout2, L"�α��μ����� ���������� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iServerUID );
		//}}
	}

	SetEvent( m_spEventAck.get() );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}

bool KLoginProxy::RoutePacket( const KEventPtr& spEvent_ )
{
	//{{ 2011. 12. 14	������	��Ŷ ó�� ���� �ð� üũ
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
	SiKPacketMornitoring()->CheckLoginProxyPacketProcessingCount( spEvent_->m_usEventID );
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
	//}}

	//START_LOG( clog, L"�����" )
	//    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
	//    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_iUID )
	//    << BUILD_LOG( pkEvent_->GetLastSenderUID() )
	//    << BUILD_LOG( pkEvent_->m_usEventID )
	//    << BUILD_LOG( pkEvent_->GetIDStr() );

	if( spEvent_->m_kDestPerformer.m_dwPerformerID != PI_LOGIN_PROXY )
	{
		int iEval = CompareServerLevel( spEvent_->m_kDestPerformer.m_dwPerformerID, PI_LOGIN_PROXY );
		if( iEval < 0 )
		{
			_JIF( !spEvent_->IsEmptyTrace(), return false );
			KActorManager::GetKObj()->QueueingTo( spEvent_->GetLastSenderUID(), spEvent_ );
			return true;
		}
		else if( iEval > 0 )
		{
			START_LOG( cerr, L"���� ������ ���� �̺�Ʈ�� ���� �ȵȴ�." )
				<< BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
				<< BUILD_LOG( spEvent_->m_usEventID )
				<< BUILD_LOG( spEvent_->GetIDStr() )
				<< END_LOG;
			return true;
		}
		else
		{
			switch( GetPerformerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) )
			{
			case PC_USER:
				{
					//{{ 2009. 7. 15  ������	���� ������ ������ ó��
					std::vector< UidType > vecUID;
					int iNotExistUserCnt = KActorManager::GetKObj()->MultiQueueing( spEvent_, vecUID );
					if( iNotExistUserCnt > 0 )
					{
						ProcbyNotExistUser( spEvent_, vecUID );

						START_LOG( cwarn, L"��Ŷ���� ������ ����." )
							<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
							<< BUILD_LOG( iNotExistUserCnt )
							<< END_LOG;
					}
					//}}
				}
				return true;
			case PC_CHARACTER:
				{
					//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
					//080711.hoons.
					//�켱 1�������� ���ΰŷ� ��ó���� ���Ǳ� ������ ĳ���ͺκи��� ���..
					std::vector<UidType> vecUID;
					int iNotExistUserCnt = KActorManager::GetKObj()->MultiQueueingByCID( spEvent_, vecUID );
					if( iNotExistUserCnt > 0 )
					{
						ProcbyNotExistUser( spEvent_, vecUID );

						START_LOG( cwarn, L"��Ŷ���� ������ ����." )
							<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
							<< BUILD_LOG( iNotExistUserCnt )
							<< END_LOG;
					}
#else
					KActorManager::GetKObj()->MultiQueueingByCID( spEvent_ );
#endif SERV_PSHOP_AGENCY
					//}}
				}
				return true;
			case PC_SERVER:
				KBaseServer::GetKObj()->QueueingEvent( spEvent_ );
				return true;
			case PC_GAME_DB:
				SiKDBLayer()->QueueingEvent( spEvent_ );
				return true;
			default:
				START_LOG( cerr, L"�������� �̻��� ��Ŷ." )
					<< BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
					<< END_LOG;
				return true;
			}
		}
	}

	return false;
}

void KLoginProxy::ProcbyNotExistUser( const KEventPtr& spEvent_, std::vector<UidType>& vecUID )
{
	switch( spEvent_->m_usEventID )
	{
	case ELG_GET_UNIT_INFO_REQ:
		{
			for( int i = 0; i < (int)vecUID.size(); ++i )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KELG_GET_UNIT_INFO_REQ kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialize failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
				else
				{
					KELG_GET_UNIT_INFO_ACK kPacket;
					kPacket.m_iOK						= NetError::ERR_SEARCH_UNIT_03;
					kPacket.m_usEventID					= kData_.m_usEventID;
					kPacket.m_iDemandGSUID				= kData_.m_iDemandGSUID;
					kPacket.m_iDemandUserUID			= kData_.m_iDemandUserUID;
					kPacket.m_kCUnitInfo.m_wstrCharName = kData_.m_wstrNickName;

					UidType anTrace[2] = { vecUID[i], -1 };
					KncSend( PI_GS_USER, vecUID[i], PI_LOGIN_USER, 0, anTrace, ELG_GET_UNIT_INFO_ACK, kPacket );

					spEvent_->m_kbuff.Reset();

					// �ϴ� �����α� ����!
					START_LOG( cerr, L"���� ���� �������� �ߴµ� �ð����� ������ ���ܹ��ȱ�!" )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
			}
		}
		break;

		//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	case ERM_BUY_PSHOP_AGENCY_ITEM_COMPLETE_NOT:
		{
			for( int i = 0; i < (int)vecUID.size(); ++i )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialze failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
				else
				{
					KDBE_INSERT_TRADE_ITEM_BY_SERVER_NOT kPacketToDB;
					kPacketToDB.m_iUnitUID = vecUID[i];
					kPacketToDB.m_kItemQuantityUpdate = kData_.m_kItemQuantityUpdateTemp;
					kPacketToDB.m_vecItemInfo = kData_.m_vecItemInfoTemp;
					kPacketToDB.m_iED = kData_.m_iEDTemp;

					UidType anTrace[2] = { vecUID[i], -1 };
					KncSend( PI_GS_USER, vecUID[i], PI_GS_GAME_DB, 0, anTrace, DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT, kPacketToDB );

					spEvent_->m_kbuff.Reset();
				}
			}
		}
		break;
#endif SERV_PSHOP_AGENCY
		//}}

		//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	case ERM_CHECK_ZOMBIE_USER_REQ:
		{
			for( int i = 0; i < (int)vecUID.size(); ++i )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KERM_CHECK_ZOMBIE_USER_REQ kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialize failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
				else
				{
					switch( CXSLRoom::GetRoomType( kData_.m_iRoomUID ) )
					{
					case CXSLRoom::RT_PSHOP_AGENCY:
						{
							KERM_CHECK_ZOMBIE_USER_ACK kPacket;
							kPacket.m_cZombieCheckType = KERM_CHECK_ZOMBIE_USER_ACK::ZCT_DISCONNECT_ZOMBIE;

							UidType anTrace[2] = { vecUID[i], -1 };
							KncSend( PI_GS_USER, vecUID[i], PI_LOGIN_ROOM, kData_.m_iRoomUID, anTrace, ERM_CHECK_ZOMBIE_USER_ACK, kPacket );
						}
						break;

					default:
						{
							START_LOG( cerr, L"�̷� �� Ÿ���� ���� üũ ��Ŷ�� �������� �����ٵ�?" )
								<< BUILD_LOG( kData_.m_iRoomUID )
								<< END_LOG;
						}
						break;
					}

					spEvent_->m_kbuff.Reset();
				}
			}
		}
		break;
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
		//}}
	}
}


