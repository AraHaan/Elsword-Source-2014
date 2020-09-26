#include "GlobalUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "DBLayer.h"
#include "NetError.h"
#include "GlobalSimLayer.h"
#include "GlobalServer.h"
#include <boost/format.hpp>

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

#include "WorldMissionManager.h"
#include "GlobalServerCCUManager.h"

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "MatchMakingManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "AutoPartyMakingManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	#include "X2Data/XSLQuestManager.h"
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
#include "ServerSNManager.h"
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}

//{{ 2012. 11. 21	�ڼ���	���� ���� ������ ������ ����Ʈ
#ifdef SERV_HERO_PVP_MANAGE_LIST
	#include "odbc/Odbc.h"
#endif SERV_HERO_PVP_MANAGE_LIST
//}}

//{{ 2013. 05. 29	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	#include "BlockListManager.h"
#endif SERV_BLOCK_LIST
//}}
#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER

//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	#include "..\Common\Temp.h"
#endif SERV_EVENT_JACKPOT
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
	#include "FieldBossManager.h"
	#include "X2Data/XSLFieldBossData.h"
	#include "X2Data/XSLBattleFieldManager.h"
#endif // SERV_BATTLE_FIELD_BOSS

NiImplementRTTI( KGlobalUser, KActor );
ImplPfID( KGlobalUser, PI_GLOBAL_USER );

#define CLASS_TYPE  KGlobalUser

KGlobalUser::KGlobalUser(void) 
{
	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	m_bConnectGameServer = false;
#endif SERV_RANDOM_DAY_QUEST
	//}}
}

KGlobalUser::~KGlobalUser(void)
{
}

ImplToStringW( KGlobalUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KGlobalUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    if( RoutePacket( spEvent_.get() ) )
        return;

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
   _CASE( EGB_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
   _CASE( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}

	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	_CASE( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT );
#endif SERV_MORNITORING
	//}}
	
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	CASE( EGB_GET_MISSION_INFO_REQ );
	_CASE( EGB_CLEAR_MISSION_DUNGEON_NOT, KEGB_CLEAR_MISSION_DUNGEON_NOT );
	_CASE( EGB_UPDATE_CCU_INFO_NOT, KELG_UPDATE_CCU_INFO_NOT );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	_CASE( EGB_TODAY_RANDOM_QUEST_REQ, UidType );
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	CASE( EGB_REGISTER_SERVERSN_REQ );
	CASE( EGB_UNREGISTER_SERVERSN_NOT );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	//}}
	//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( EGB_REGISTER_UNIT_NOT );
	CASE( EGB_UNIT_DISCONNECT_NOT );

   _CASE( EGB_INVITE_PARTY_FIND_RECEIVER_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );
   _CASE( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
   _CASE( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   //{{ 2012. 11. 21	�ڼ���	���� ���� ������ ������ ����Ʈ
#ifdef SERV_HERO_PVP_MANAGE_LIST
   CASE_NOPARAM( EGB_ADMIN_HERO_PVP_USER_LIST_REQ );
   _CASE( EGB_ADMIN_HERO_PVP_USER_ADD_REQ, KEGS_ADMIN_HERO_PVP_USER_ADD_REQ );
   _CASE( EGB_ADMIN_HERO_PVP_USER_REMOVE_REQ, KEGS_ADMIN_HERO_PVP_USER_REMOVE_REQ );
   _CASE( EGB_ADMIN_NOTIFY_HERO_PVP_USER_REQ, KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ );
   CASE( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT );
#endif SERV_HERO_PVP_MANAGE_LIST
   //}}

	//{{ 2013. 05. 29	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	CASE( EGB_CHECK_BLOCK_USER_NOT );
   _CASE( EGB_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT );
#endif SERV_BLOCK_LIST
	//}}
   //{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
   CASE( EGB_GET_GLOBAL_MISSION_INFO_REQ );
   _CASE( EGB_GLOBAL_MISSION_UPDATE_NOT, KEGB_GLOBAL_MISSION_UPDATE_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER
   //}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
   	
#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
   CASE_NOPARAM( EGB_TIME_ENCHANT_EVENT_INFO_REQ );
   CASE( EGB_TIME_ENCHANT_EVENT_NOT );
   CASE( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
   CASE_NOPARAM( EGB_EXCHANGE_LIMIT_INFO_REQ );
   CASE( EGB_EXCHANGE_LIMIT_REQ );
   CASE( DBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK );
   CASE( EGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
   CASE( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT );
   CASE_NOPARAM( EGB_BATTLE_FIELD_BOSS_INFO_NOT );

   CASE( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ );
   CASE_NOPARAM( EGB_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );
   CASE( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ );
   CASE( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ );
#endif // SERV_BATTLE_FIELD_BOSS

    CASE_NOPARAM( E_RESERVE_DESTROY );

    default:
        dbg::cerr << L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr();
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

void KGlobalUser::Tick()
{
    KActor::Tick();

    switch( GetStateID() ) 
	{
    case KDefaultFSM::S_EXIT:
        KActorManager::GetKObj()->ReserveDelete( m_nUID );    // �ڱ� �ڽ��� ������ ������ �� ����.
        break;
    }

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	if( m_bConnectGameServer == false )
	{
		KEGB_TODAY_RANDOM_QUEST_ACK kAck;

		SiCXSLQuestManager()->GetRandomQuestGroupList( kAck.m_mapTodayRandomQuest );

		UidType anTrace[2] = { m_nUID, -1 };
		KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, EGB_TODAY_RANDOM_QUEST_ACK, kAck );

		m_bConnectGameServer = true;
	}
#endif SERV_RANDOM_DAY_QUEST
	//}}
}

void KGlobalUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() ) {
    case KDefaultFSM::S_INIT:
    case KDefaultFSM::S_LOGINED:
        StateTransition( KDefaultFSM::I_EXIT );    // OnDestroy()�� �Ҹ� ���� KSession::Tick ó�� ���̸�,
                                                   // KLoginUser::Tick ó�� �ܰ迡�� ���� ����ܿ� ��ϵȴ�.
        break;
    }

    GetKGlobalSimLayer()->UnRegAllUserOfThisGS( GetUID() );

	//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, EGB_CHECK_INVALID_MATCH_NOT, char() );
	SiKMatchMakingManager()->QueueingEventToMatch( spEvent );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	KGlobalServer* pGlobalServer = static_cast<KGlobalServer*>( KBaseServer::GetKObj() );
	if( pGlobalServer != NULL )
	{
		pGlobalServer->RemoveServerUID( GetUID() );
	}
#endif SERV_EVENT_JACKPOT
	//}}

    // ���Ӽ����� ���� ���, �ش� ���Ӽ����� �������̴� �������� �濡�� ��� ������.
    //KEventPtr spEvent( new KEvent );
    //spEvent->SetData( PI_GS_SERVER, NULL, ERM_CHECK_INVALID_USER_NOT, m_nUID );
    //SiKRoomManager()->QueueingToAll( spEvent );

	//{{ 2009. 1. 30  ������	��ü �Ҹ� �α�
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"���� �˴ٿ����� ���� GlobalUser��ü �Ҹ�" );
	}
	else
	{
		START_LOG( cout2, L"���������� �������� ���� GlobalUser��ü �Ҹ�" )
			<< BUILD_LOG( GetDisconnectReason() )
			<< BUILD_LOG( GetIPStr() )
			<< dbg::tab << L"���� ���� : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetUID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}

		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
		SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_GLOBAL_PROXY );
#endif SERV_MORNITORING
		//}}
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	}
	//}}
}

bool KGlobalUser::RoutePacket( const KEvent* pkEvent_ )
{
    if( pkEvent_->m_kDestPerformer.m_dwPerformerID != PI_GLOBAL_USER )
    {
        int iEval = CompareServerLevel( pkEvent_->m_kDestPerformer.m_dwPerformerID, PI_GLOBAL_USER );
        if( iEval < 0 )
        {
            if( pkEvent_->IsEmptyTrace() )
            {
                START_LOG_WITH_NAME( cerr ) 
                    << BUILD_LOG( pkEvent_->GetIDStr() );

                return true;
            }

            KEvent kEvent( *pkEvent_ );

            kEvent.PopTrace();

            LIF( KSession::SendPacket( kEvent ) );
            return true;
        }
        else if( iEval > 0 )
        {
            START_LOG( cerr, L"����� ���� ������ �ֻ��� �����̴�." )
                << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( pkEvent_->m_usEventID )
                << BUILD_LOG( pkEvent_->GetIDStr() )
                << END_LOG;

            return true;
        }
        else
        {
            KEventPtr spEvent( pkEvent_->Clone() );
            spEvent->PushTrace( GetUID() );

            switch( GetPerformerClass( pkEvent_->m_kDestPerformer.m_dwPerformerID ) )
            {
            case PC_SERVER:
                KBaseServer::GetKObj()->QueueingEvent( spEvent );
                return true;
            case PC_ACCOUNT_DB:
            case PC_LOG_DB:
                SiKDBLayer()->QueueingEvent( spEvent );
                return true;
				//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			case PC_MATCH_MAKING:
				SiKMatchMakingManager()->QueueingEventToMatch( spEvent );
				return true;
#endif SERV_PVP_NEW_SYSTEM
				//}}
				//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case PC_AUTO_PARTY:
				SiKAutoPartyMakingManager()->QueueingEvent( spEvent );
				return true;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
            default:
                START_LOG( cerr, L"�������� �̻��� ��Ŷ." )
                    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                    << END_LOG;
                return true;
            }
        }
    }

    return false;
}

//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
_IMPL_ON_FUNC( EGB_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_usPort );

	if( kPacket_.m_kNetAddress.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"��ȿ���� ���� IP" )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
			<< BUILD_LOG( GetIPStr() );

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( EGB_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::S_INIT ));

	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_iDBRegServerGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );

    SendPacket( EGB_VERIFY_SERVER_CONNECT_ACK, kPacket_ );

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        ReserveDestroy();
        return;
    }
	else
	{
		//{{ 2010. 04. 16  ������	�α� �߰�
		START_LOG( cout2, L"���Ӽ����� ���������� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_nUID )
			<< BUILD_LOG( kPacket_.m_strName );
		//}}

		//////////////////////////////////////////////////////////////////////////
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GLOBAL_SERVER, NULL, EGB_GET_AUTO_PARTY_BONUS_INFO_REQ, char() );
		spEvent->PushTrace( GetUID() );
		SiKAutoPartyMakingManager()->QueueingEvent( spEvent );
		//////////////////////////////////////////////////////////////////////////		
	}

    KActorPtr spActor( KActorManager::GetKObj()->Get( kPacket_.m_nUID ) );

    if( spActor != NULL )   // ���� ���� ����
    {
        SET_ERROR( ERR_VERIFY_03 ); // ���� UID �浹
        ReserveDestroy();
        return;
    }

    SetName( kPacket_.m_strName );
    LIF( KActorManager::GetKObj()->UpdateUID( kPacket_.m_nUID, *this ) );

	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	KGlobalServer* pGlobalServer = static_cast<KGlobalServer*>( KBaseServer::GetKObj() );
	if( pGlobalServer != NULL )
	{
		pGlobalServer->ReserveServerUID( kPacket_.m_iLocalServerGroupID, kPacket_.m_nUID );
	}
#endif SERV_EVENT_JACKPOT
	//}}

    StateTransition( KDefaultFSM::I_AUTHENTICATED );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
_IMPL_ON_FUNC( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY );
		}
		break;

	case KMornitoringManager::ET_DISCONNECT_GAME_BILLING:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_BILLING );
		}
		break;

	default:
		START_LOG( cerr, L"�߸��� Ÿ���Դϴ�!" )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< END_LOG;
		return;
	}
}
#endif SERV_MORNITORING
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
IMPL_ON_FUNC( EGB_GET_MISSION_INFO_REQ )
{
	if( SiKWorldMissionManager()->GetIsFirstTick() == true )	// ���� ������ ���� �ʾҽ�
	{
		return;
	}
	// �̼������� ���Ӽ����� ������
	KEGB_GET_MISSION_INFO_ACK kPacket;

	//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	SiKWorldMissionManager()->GetWorldMissionInfo( KEGB_GET_MISSION_INFO_ACK::FT_INIT, kPacket );
#else
	SiKWorldMissionManager()->GetWorldMissionInfo( kPacket );
	kPacket.m_iFlag = KEGB_GET_MISSION_INFO_ACK::FT_INIT;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	UidType anTrace[2] = { kPacket_.m_iServerUID, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, EGB_GET_MISSION_INFO_ACK, kPacket );
}

_IMPL_ON_FUNC( EGB_CLEAR_MISSION_DUNGEON_NOT, KEGB_CLEAR_MISSION_DUNGEON_NOT )
{
	SiKWorldMissionManager()->IncreaseProtectedCrystalCount( kPacket_ );

	START_LOG( clog, L"���潺 ���� Ŭ���� (��ȣ�� ������ ��)" )
		<< BUILD_LOG( kPacket_ )
		<< END_LOG;
}

_IMPL_ON_FUNC( EGB_UPDATE_CCU_INFO_NOT, KELG_UPDATE_CCU_INFO_NOT )
{
	// ���� ���� ����
	SiKGlobalServerCCUManager()->UpdateCCUInfo( kPacket_.m_kCCUInfo );

	START_LOG( clog, L"CCU �����͸� �޾ҽ��ϴ�." )
		<< BUILD_LOG( kPacket_.m_kCCUInfo.m_iConcurrentUser )
		<< END_LOG;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�


//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
_IMPL_ON_FUNC( EGB_TODAY_RANDOM_QUEST_REQ, UidType )
{
	KEGB_TODAY_RANDOM_QUEST_ACK kAck;

	SiCXSLQuestManager()->GetRandomQuestGroupList( kAck.m_mapTodayRandomQuest );

	UidType anTrace[2] = { kPacket_, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, EGB_TODAY_RANDOM_QUEST_ACK, kAck );
}
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
IMPL_ON_FUNC( EGB_REGISTER_SERVERSN_REQ )
{
	KEGB_REGISTER_SERVERSN_ACK kAck;
	kAck.m_iUserUid = kPacket_.m_iUserUid;

	std::map<UidType, UidType> mapOverlapUser;
	UidType iGSUID = GetUID();
	SiKServerSNManager()->RegServerSN( kPacket_.m_iServerSN, kAck.m_iUserUid, iGSUID,  kAck.m_iOK, mapOverlapUser );

	SendToGSUser( FIRST_SENDER_UID, EGB_REGISTER_SERVERSN_ACK, kAck );

	if( kAck.m_iOK != NetError::NET_OK )
	{
		std::map<UidType, UidType>::iterator mit = mapOverlapUser.begin();
		for( ; mit != mapOverlapUser.end() ; ++mit )
		{
			if( mit->first == kAck.m_iUserUid )
			{
				continue;
			}

			KEGB_REGISTER_SERVERSN_NOT kNot;
			kNot.m_iOK = kAck.m_iOK;
			kNot.m_iUserUid = mit->first;

			SendToGSUser( mit->second, mit->first, EGB_REGISTER_SERVERSN_NOT, kNot );
		}
	}
}

IMPL_ON_FUNC( EGB_UNREGISTER_SERVERSN_NOT )
{
	int iOK = NetError::NET_OK;

	SiKServerSNManager()->UnRegServerSN( kPacket_.m_iServerSN, kPacket_.m_iUserUid, iOK );

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ServerSN Manager���� �ش� SN ���� ����!" );
	}
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}
//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EGB_REGISTER_UNIT_NOT )
{
	int iOK = GetKGlobalSimLayer()->RegUnit( kPacket_.m_kUnitInfo );
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ��� ����" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( NetError::GetErrStr( iOK ) )
			<< END_LOG;
		return;
	}

	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT_TEST
	if( kPacket_.m_kUnitInfo.m_iChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
#else
	if( kPacket_.m_kUnitInfo.m_iChannelCode == KNexonAccountInfo::CE_NEXON_ACCOUNT )
#endif SERV_EVENT_JACKPOT_TEST
		//}}
	{
		SiKJackPotEventManager()->AddUser( kPacket_.m_kUnitInfo.m_iUserUID, kPacket_.m_kUnitInfo.m_iUnitUID );
	}
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
	// ���� ���� ���� �����ڶ��, ��ȸ ä�� ���� ���θ� �˸����� �����Ѵ�.
	if( GetKGlobalSimLayer()->FindHeroPvpUnit( KncUid::ExtractServerGroupID( kPacket_.m_kUnitInfo.m_iUnitUID ), kPacket_.m_kUnitInfo.m_wstrUnitNickName ) == NetError::NET_OK )
	{
		SendToGSCharacter( FIRST_SENDER_UID, kPacket_.m_kUnitInfo.m_iUnitUID, EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_CHECK_NOT, true );
	}
#endif	// SERV_HERO_PVP_MANAGE_LIST
}

IMPL_ON_FUNC( EGB_UNIT_DISCONNECT_NOT )
{
#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
	// ���� ���� ���� �����ڶ��, ��ȸ ä�� ���� ���θ� �˸����� �����Ѵ�.
	KGlobalUserList::KGSUserInfo kGSUserInfo;
	if( GetKGlobalSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kGSUserInfo ) == true )
	{
		if( GetKGlobalSimLayer()->FindHeroPvpUnit( KncUid::ExtractServerGroupID( kGSUserInfo.m_nUnitUID ), kGSUserInfo.m_wstrUnitNickName ) == NetError::NET_OK )
		{
			SendToGSCharacter( FIRST_SENDER_UID, kPacket_.m_iUnitUID, EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_CHECK_NOT, false );
		}
	}
#endif	// SERV_HERO_PVP_MANAGE_LIST

	GetKGlobalSimLayer()->UnRegUnit( kPacket_.m_iUnitUID );
	
	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	SiKJackPotEventManager()->RemoveUser( kPacket_.m_iUserUID, kPacket_.m_iUnitUID );
#endif SERV_EVENT_JACKPOT
	//}}
}

_IMPL_ON_FUNC( EGB_INVITE_PARTY_FIND_RECEIVER_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	//VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	bool bFindSuccess = false;
	KGlobalUserList::KGSUserInfo kUserInfo;

	// UnitUID������ ���� ���� ���������� ã�ƺ���
	if( kPacket_.m_iReceiverUnitUID != 0 )
	{
		bFindSuccess = GetKGlobalSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kUserInfo );
	}

	// UnitUID�ε� ã�� ���ߴٸ� �г������� ã�ƺ���!
	if( kPacket_.m_wstrReceiverNickName.empty() == false  &&  bFindSuccess == false )
	{
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // ���볯¥: 2013-04-04
		bFindSuccess = GetKGlobalSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_iServerGroupID, kPacket_.m_wstrReceiverNickName, kUserInfo );
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
		bFindSuccess = GetKGlobalSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrReceiverNickName, kUserInfo );
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	}

	if( bFindSuccess == false )
	{
		//////////////////////////////////////////////////////////////////////////
		// ������ �ִ�! �ش� �۷ι� ������ �ϳ��� ������DB���� ������ �ֱ� ������ ���� �������� �г��� üũ�� �Ұ����ϴ�.
		// �� ������ �ذ��ϱ� ���� ����� ã�ƾ��Ѵ�!
		//if( kPacket_.m_wstrReceiverNickName.empty() == false )
		//{			
		//	SendToGameDB( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, kPacket_ );
		//}
		
		KELG_INVITE_PARTY_FIND_RECEIVER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PARTY_07;
#ifdef SERV_FIX_INVITE_PARTY_FIND_RECEIVER_ACK// �۾���¥: 2013-07-15	// �ڼ���
		SendToGSUser( FIRST_SENDER_UID, EGB_INVITE_PARTY_FIND_RECEIVER_ACK, kPacketAck );
#else
		SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_PARTY_FIND_RECEIVER_ACK, kPacket_ );
#endif // SERV_FIX_INVITE_PARTY_FIND_RECEIVER_ACK
		return;
	}

	// ã�� ĳ���� ������ �ش� GS�� ������û
	SendToGSCharacter( kUserInfo.m_nGSUID, kUserInfo.m_nUnitUID, EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	//VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KGlobalUserList::KGSUserInfo kUserInfo;
	if( GetKGlobalSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUnitUID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"��Ƽ �ʴ����� ������ ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iHostUnitUID )
			<< END_LOG;
		return;
	}

	SendToGSCharacter( kUserInfo.m_nGSUID, kUserInfo.m_nUnitUID, EGB_INVITE_PARTY_FIND_RECEIVER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	//VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KGlobalUserList::KGSUserInfo kUserInfo;
	if( GetKGlobalSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUnitUID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"��Ƽ �ʴ����� ������ ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iHostUnitUID )
			<< END_LOG;
		return;
	}

	SendToGSCharacter( kUserInfo.m_nGSUID, kUserInfo.m_nUnitUID, EGB_INVITE_PARTY_FIND_RECEIVER_ACK, kPacket_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 05. 29	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
IMPL_ON_FUNC( EGB_CHECK_BLOCK_USER_NOT )
{
	KEGB_UPDATE_BLOCK_RESULT_NOT kPacket;

	BOOST_TEST_FOREACH( const int, iBlockType, kPacket_.m_vecBlockType )
	{
		switch( iBlockType )
		{
		case KBlockInfo::BT_TRADE_BLOCK:
			{
				if( SiKBlockListManager()->IsTradeBlockUser( kPacket_.m_iUserUID ) == true )
				{
					kPacket.m_mapBlockResult.insert( std::make_pair( iBlockType, true ) );
				}
			}
			break;

		case KBlockInfo::BT_ACCOUNT_BLOCK:
			{
				if( SiKBlockListManager()->IsAccountBlockUser( kPacket_.m_iUserUID ) == true )
				{
					kPacket.m_mapBlockResult.insert( std::make_pair( iBlockType, true ) );
				}
			}
			break;

		case KBlockInfo::BT_IP_BLOCK:
			{
				if( SiKBlockListManager()->IsIpBlockUser( kPacket_.m_wstrIP ) == true )
				{
					kPacket.m_mapBlockResult.insert( std::make_pair( iBlockType, true ) );
				}
			}
			break;

		case KBlockInfo::BT_DLL_BLOCK:
			{

			}
			break;

		default:
			{
				START_LOG( cerr, L"���� ���� ���� �� Ÿ�� �Դϴ�." )
					<< BUILD_LOG( kPacket_.m_vecBlockType.size() )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_wstrIP )
					<< END_LOG;
			}
			break;
		}
	}	

	// ����� �Ǵ� ��쿡�� ��Ŷ�� ������!
	if( kPacket.m_mapBlockResult.empty() == false )
	{
		SendToGSUser( FIRST_SENDER_UID, EGB_UPDATE_BLOCK_RESULT_NOT, kPacket );
	}
}

_IMPL_ON_FUNC( EGB_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT )
{
	//VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//{{ 2012. 01. 27  ��μ�	��ŷ ���μ��� �˻� ���
#ifdef SERV_HACKING_DLL_CHECK
	KELG_CHECK_MODULE_INFO_UPDATE_NOT kNot;
	kNot.m_iOK = NetError::NET_OK;

	SiKBlockListManager()->UpdateModuleList( kPacket_.m_vecModuleName );

	if( SiKBlockListManager()->CheckHackingModuleList( kPacket_.m_vecModuleName, kNot.m_mapModuleName ) == false )
	{
		// �����Ͱ� ��� ������ ������ �ʴ´�.
		if( kNot.m_mapModuleName.empty() == true )
		{
			return;
		}

		// �� ����Ʈ�� ��ϵ� DLL �� ����ϴ� �����̹Ƿ� ���� ��Ű��!
		kNot.m_iOK = NetError::ERR_CHECK_DLL_01;
		SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_MODULE_INFO_UPDATE_NOT, kNot );
	}
#else
	SiKBlockListManager()->UpdateModuleList( kPacket_.m_vecModuleName );
#endif SERV_HACKING_DLL_CHECK
	//}}
}
#endif SERV_BLOCK_LIST
//}}
//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
IMPL_ON_FUNC( EGB_GET_GLOBAL_MISSION_INFO_REQ )
{
	if( SiKGlobalMissionManager()->GetIsFirstTick() == true )	// ù ��° ƽ�̸� �۷ι� �̼� ������ ������ ����.
	{
		return;
	}

	KEGB_GET_GLOBAL_MISSION_INFO_ACK kPacket;
	SiKGlobalMissionManager()->GetGlobalMissionInfo( KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_INIT, kPacket );

	UidType anTrace[2] = { kPacket_.m_iServerUID, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, EGB_GET_GLOBAL_MISSION_INFO_ACK, kPacket );
}

_IMPL_ON_FUNC( EGB_GLOBAL_MISSION_UPDATE_NOT, KEGB_GLOBAL_MISSION_UPDATE_NOT )
{
	SiKGlobalMissionManager()->IncreaseGlobalMissionClearCount(kPacket_.m_wstrGlobalMissionDay, kPacket_.m_iNowCount);
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���

//{{ 2012. 11. 21	�ڼ���	���� ���� ������ ������ ����Ʈ
#ifdef SERV_HERO_PVP_MANAGE_LIST
IMPL_ON_FUNC_NOPARAM( EGB_ADMIN_HERO_PVP_USER_LIST_REQ )
{
	KEGS_ADMIN_HERO_PVP_USER_LIST_ACK kPacket;
	GetKGlobalSimLayer()->GetHeroPvpUserList( kPacket.m_vecUserList );
	SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_HERO_PVP_USER_LIST_ACK, kPacket );
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_ADD_REQ, KEGS_ADMIN_HERO_PVP_USER_ADD_REQ )
{
	KEGS_ADMIN_HERO_PVP_USER_ADD_ACK kPacket;
	GetKGlobalSimLayer()->AddHeroPvpUserList( kPacket_.m_vecUserList, kPacket.m_vecUserList );
	
	std::vector< std::pair< UidType, std::wstring > >::const_iterator it;
	for( it = kPacket.m_vecUserList.begin(); it != kPacket.m_vecUserList.end(); ++it )
	{
		KGlobalUserList::KGSUserInfo kGSUserInfo;
		GetKGlobalSimLayer()->GetGSUserInfoByUnitNickName( it->first, it->second, kGSUserInfo );
		SendToGSCharacter( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUnitUID, EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_CHECK_NOT, true );
	}

	SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_HERO_PVP_USER_ADD_ACK, kPacket );
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_REMOVE_REQ, KEGS_ADMIN_HERO_PVP_USER_REMOVE_REQ )
{
	KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK kPacket;
	GetKGlobalSimLayer()->RemoveHeroPvpUserList( kPacket_.m_vecUserList, kPacket.m_vecUserList );

	std::vector< std::pair< UidType, std::wstring > >::const_iterator it;
	for( it = kPacket.m_vecUserList.begin(); it != kPacket.m_vecUserList.end(); ++it )
	{
		KGlobalUserList::KGSUserInfo kGSUserInfo;
		GetKGlobalSimLayer()->GetGSUserInfoByUnitNickName( it->first, it->second, kGSUserInfo );
		SendToGSCharacter( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUnitUID, EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_CHECK_NOT, false );
	}

	SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_HERO_PVP_USER_REMOVE_ACK, kPacket );
}

_IMPL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_REQ, KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ )
{
	KPacketOK kPacket;

	// ĳ���� �г��� ���� �˻�.
	if( kPacket_.m_wstrNickName.empty() == true )
	{
		// ��� ����
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );

		KGlobalUserList::HERO_PVP_USER_TYPE vecUserList;
		GetKGlobalSimLayer()->GetHeroPvpUserList( vecUserList );

		KGlobalUserList::HERO_PVP_USER_TYPE::const_iterator it;
		for( it = vecUserList.begin(); it != vecUserList.end(); ++it )
		{
			if( it->m_bOn == false )
			{
				continue;
			}

			KGlobalUserList::KGSUserInfo kUserInfo;
			if( GetKGlobalSimLayer()->GetGSUserInfoByUnitNickName( it->m_iServerGroupID, it->m_wstrNickName, kUserInfo ) == false )
			{
				continue;
			}

			SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGB_ADMIN_NOTIFY_HERO_PVP_USER_NOT, kPacket_ );
		}
	}
	else
	{
		// �ش� ���� �˻�
		kPacket.m_iOK = GetKGlobalSimLayer()->FindHeroPvpUnit( kPacket_.m_iServerGroupID, kPacket_.m_wstrNickName );
		if( kPacket.m_iOK != NetError::NET_OK )
		{
			SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
			return;
		}

		// ���� ���̶�� ��Ŷ ����
		KGlobalUserList::KGSUserInfo kUserInfo;
		if( GetKGlobalSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_iServerGroupID, kPacket_.m_wstrNickName, kUserInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_03;
			SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
			return;
		}

		SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGB_ADMIN_NOTIFY_HERO_PVP_USER_NOT, kPacket_ );

		// ��� ����
		SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
	}
}

IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT )
{
	GetKGlobalSimLayer()->SetHeroPvpUserTournamentConnection( kPacket_.m_iServerGroupID, kPacket_.m_wstrNickName, kPacket_.m_bConnect );
}
#endif SERV_HERO_PVP_MANAGE_LIST
//}}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
IMPL_ON_FUNC_NOPARAM( EGB_TIME_ENCHANT_EVENT_INFO_REQ )
{
	KEGB_TIME_ENCHANT_EVENT_INFO_ACK kPacket;
	GetKGlobalServer()->GetTimeEnchantEventUID( kPacket.m_iTimeEnchantEventUID );
	GetKGlobalServer()->GetNumOfBeginTimeEnchantEventInfo( kPacket.m_iNumOfTimeEnchantEvent );
	SendToGameServer( EGB_TIME_ENCHANT_EVENT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_NOT )
{
	KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ kPacketToDB;
	if( GetKGlobalServer()->RetrieveTimeEnchantEventData( kPacketToDB.m_kTimeEnchantEventInfo, kPacketToDB.m_iTimeEnchantEventUID ) == true )
	{
		kPacketToDB.m_iGSUID				= kPacket_.m_iGSUID;
		kPacketToDB.m_iUnitUID				= kPacket_.m_iUnitUID;
		kPacketToDB.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;
		kPacketToDB.m_usEventID				= kPacket_.m_usEventID;
		SendToLogDB( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ, kPacketToDB );
	}
	else
	{
		KTimeEnchantEventResult kPacket;
		kPacket.m_iAfterWorkStorageKey = kPacket_.m_iAfterWorkStorageKey;
		SendToGSUser( FIRST_SENDER_UID, kPacket_.m_usEventID, kPacket );
	}
}

IMPL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK )
{
	KTimeEnchantEventResult kPacket;
	kPacket.m_iAfterWorkStorageKey		= kPacket_.m_iAfterWorkStorageKey;
	kPacket.m_cTimeEnchantEventLevel	= kPacket_.m_cEnchantLevel;
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, kPacket_.m_usEventID, kPacket );
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
IMPL_ON_FUNC_NOPARAM( EGB_EXCHANGE_LIMIT_INFO_REQ )
{
	KEGB_EXCHANGE_LIMIT_INFO_ACK kPacket;
	GetKGlobalServer()->GetExchangeLimitUID( kPacket.m_iExchangeLimitUID );
	if( kPacket.m_iExchangeLimitUID != 0 )
	{
		GetKGlobalServer()->GetBeginExchangeLimitInfo( kPacket.m_mapExchangeLimitInfo );
	}

	SendToGameServer( EGB_EXCHANGE_LIMIT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_REQ )
{	
	int iLimitUID = 0;
	const int iResult = GetKGlobalServer()->RetrieveExchangeLimitData( kPacket_.m_iDestItemID, iLimitUID );

	switch( iResult )
	{
	case KGlobalServer::ELR_SUCCEED:
		{
			KDBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ kPacketToDB;
			GetKGlobalServer()->GetExchangeLimitUID( kPacketToDB.m_iExchangeLimitUID );
			kPacketToDB.m_iLimitUID				= iLimitUID;
			kPacketToDB.m_iDestItemID			= kPacket_.m_iDestItemID;
			kPacketToDB.m_wstrRegDate			= CTime::GetCurrentTime().Format( _T( "%Y-%m-%d %H:%M:%S" ) );

			kPacketToDB.m_iGSUID				= kPacket_.m_iGSUID;
			kPacketToDB.m_iUnitUID				= kPacket_.m_iUnitUID;
			kPacketToDB.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;

			SendToLogDB( DBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ, kPacketToDB );
		}
		break;

	case KGlobalServer::ELR_NOT_INITED:
		{
			KEGB_EXCHANGE_LIMIT_ACK kPacket;
			kPacket.m_iOK					= NetError::ERR_EXCHANGE_LIMIT_01;	// ���� ��� ������ �ʱ�ȭ ���� �ʾ� ������ ��ȯ�� �����Ͽ����ϴ�.
			kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;
			SendToGSUser( FIRST_SENDER_UID, EGB_EXCHANGE_LIMIT_ACK, kPacket );
		}
		break;

	case KGlobalServer::ELR_EXHAUSTED:
		{
			KEGB_EXCHANGE_LIMIT_ACK kPacket;
#ifdef SERV_ITEM_EXCHANGE_LIMIT_POPUP// �۾���¥: 2013-07-02	// �ڼ���
			switch( kPacket_.m_iDestItemID )
			{
			case 135422:
			case 135423:
			case 135424:
				kPacket.m_iOK				= NetError::ERR_EXCHANGE_LIMIT_03;	// ��ȯ ������ ���� ������ ��� �����Ǿ����ϴ�.
				break;

			default:
				kPacket.m_iOK				= NetError::ERR_EXCHANGE_LIMIT_02;	// ��ȯ ������ ���� ������ ��� �����Ǿ����ϴ�.
				break;
			}
#else // SERV_ITEM_EXCHANGE_LIMIT_POPUP
			kPacket.m_iOK					= NetError::ERR_EXCHANGE_LIMIT_02;	// ��ȯ ������ ���� ������ ��� �����Ǿ����ϴ�.
#endif // SERV_ITEM_EXCHANGE_LIMIT_POPUP
			kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;
			SendToGSUser( FIRST_SENDER_UID, EGB_EXCHANGE_LIMIT_ACK, kPacket );
		}
		break;

	case KGlobalServer::ELR_PASS:
		{
			KEGB_EXCHANGE_LIMIT_ACK kPacket;
			kPacket.m_iOK					= NetError::NET_OK;					// ��ȯ ���� ����� �ƴմϴ�.
			kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;
			SendToGSUser( FIRST_SENDER_UID, EGB_EXCHANGE_LIMIT_ACK, kPacket );
		}
		break;

	default:
		START_LOG( cerr, L"RetrieveExchangeLimitData: ���ǵ��� ���� �����Դϴ�." )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( kPacket_.m_iGSUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iDestItemID )
			<< END_LOG;
		break;
	}
}

IMPL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK )
{
	KEGB_EXCHANGE_LIMIT_ACK kPacket;
	
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		kPacket.m_iExchangeLimitUID	= kPacket_.m_iExchangeLimitUID;
		kPacket.m_iLimitUID			= kPacket_.m_iLimitUID;
		kPacket.m_iRollBackUID		= kPacket_.m_iRollBackUID;
	}

	kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;

	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, EGB_EXCHANGE_LIMIT_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT )
{
	if( GetKGlobalServer()->CheckExchangeLimitUID( kPacket_.m_iExchangeLimitUID ) == true )
	{
		GetKGlobalServer()->ReInsertExchangeLimitData( kPacket_.m_iLimitUID );
	}

	KDBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT kPacket;
	kPacket.m_iRollBackUID	= kPacket_.m_iRollBackUID;
	SendToLogDB( DBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT, kPacket );
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
int KGlobalUser::BossFieldOpenProcess( IN const int iVillageMapID, IN const __time64_t tCurrentTime ) const
{
	int iPortalDestination;
	if( SiCXSLBattleFieldManager()->GetRandomBossFieldID( iPortalDestination ) == false )
	{
		START_LOG( cerr, L"�ʵ� ���� �ý��ۿ��� ����� ���� �ʵ������� �������� �ʽ��ϴ�." )
			<<END_LOG;
		
		return NetError::ERR_FIELD_BOSS_06;	// ���� �ʵ������� �������� �ʽ��ϴ�.
	}

	SiKFieldBossManager()->OpenPortal( iVillageMapID, iPortalDestination, tCurrentTime );	// ���� ���

	// ��� ���� ������ �˸���
	const CTime tPortalAppearanceTime( SiKFieldBossManager()->GetPortalAppearanceTime() );
	const CTime tPortalHoldingTime = tPortalAppearanceTime + CTimeSpan( 0, 0, SiCXSLFieldBossData()->GetFieldBossPortalHoldingTime_M(), 0 );
	const CTime tFieldBossCoolTime = tPortalAppearanceTime + CTimeSpan( 0, 0, SiCXSLFieldBossData()->GetFieldBossCoolTime_M(), 0 );

	KEGB_BATTLE_FIELD_BOSS_INFO_NOT kPacket;
	kPacket.m_tRemainPortalTime			= tPortalHoldingTime.GetTime() - tCurrentTime;
	kPacket.m_tRemainCoolTime			= tFieldBossCoolTime.GetTime() - tCurrentTime;
	kPacket.m_iPortalAppearanceMap		= SiKFieldBossManager()->GetPortalAppearanceMap();
	kPacket.m_iPortalDestination		= SiKFieldBossManager()->GetPortalDestination();
	kPacket.m_bPortalOpen				= SiKFieldBossManager()->IsPortalOpen();
	kPacket.m_bCoolTime					= SiKFieldBossManager()->IsCoolTime();

	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, EGB_BATTLE_FIELD_BOSS_INFO_NOT, kPacket );
	
	return NetError::NET_OK;
}

int KGlobalUser::TotalDangerousValueUpdateProcess( IN const int iBattleFieldID, IN const byte byteValue, IN const __time64_t tCurrentTime ) const
{
	// �ʵ� ���� �ý��� CoolTime ���ΰ�?
	if( SiKFieldBossManager()->IsCoolTime() == true )
	{
		return NetError::ERR_FIELD_BOSS_05;	// �� Ÿ�� �����Դϴ�.
	}

	// ���� �� TotalDangerousValue�� ��������
	const byte byteOldTotalDangerousValue = SiKFieldBossManager()->GetTotalDangerousValue();

	// ������ ����
	SiKFieldBossManager()->UpdateTotalDangerousValue( iBattleFieldID, byteValue );

	const byte byteTotalDangerousVale = SiKFieldBossManager()->GetTotalDangerousValue();

	// ���� ���� TotalDangerousValue ���� �����Ͽ���, ������ ��ġ�� �Ѿ�ٸ� �ʵ� ������ ������Ű��
	if( ( byteOldTotalDangerousValue < byteTotalDangerousVale )
		&& ( SiCXSLFieldBossData()->GetTotalDangerousValue() <= byteTotalDangerousVale )
		)
	{
		int iPortalLocationID;
		if( SiCXSLFieldBossData()->GetRandomPortalLocationID( iPortalLocationID ) == true )
		{
			BossFieldOpenProcess( iPortalLocationID, tCurrentTime );
		}
	}

	return NetError::NET_OK;
}

IMPL_ON_FUNC( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT )
{
	// ���� �ð� ���
	const CTime tCurrentTime = CTime::GetCurrentTime();
	const byte byteValue = SiKFieldBossManager()->GetTotalDangerousValue( kPacket_.m_iBattleFieldID ) + kPacket_.m_byteIncreasedValue;
	TotalDangerousValueUpdateProcess( kPacket_.m_iBattleFieldID, byteValue, tCurrentTime.GetTime() );
}

IMPL_ON_FUNC_NOPARAM( EGB_BATTLE_FIELD_BOSS_INFO_NOT )
{
	// ���� �ð� ���
	const CTime tCurrentTime = CTime::GetCurrentTime();

	const bool bPortalOpen	= SiKFieldBossManager()->IsPortalOpen();
	const bool bCoolTime	= SiKFieldBossManager()->IsCoolTime();

	const CTime tPortalAppearanceTime( SiKFieldBossManager()->GetPortalAppearanceTime() );
	
	KEGB_BATTLE_FIELD_BOSS_INFO_NOT kPacket;

	// ��Ż ����
	if( bPortalOpen == true )
	{
		const CTime tPortalHoldingTime = tPortalAppearanceTime + CTimeSpan( 0, 0, SiCXSLFieldBossData()->GetFieldBossPortalHoldingTime_M(), 0 );
		kPacket.m_tRemainPortalTime = tPortalHoldingTime.GetTime() - tCurrentTime.GetTime();
	}
	else
	{
		kPacket.m_tRemainPortalTime = 0;
	}

	// �� Ÿ�� ����
	if( bCoolTime == true )
	{
		const CTime tFieldBossCoolTime = tPortalAppearanceTime + CTimeSpan( 0, 0, SiCXSLFieldBossData()->GetFieldBossCoolTime_M(), 0 );
		kPacket.m_tRemainCoolTime = tFieldBossCoolTime.GetTime() - tCurrentTime.GetTime();
	}
	else
	{
		kPacket.m_tRemainCoolTime = 0;
	}

	kPacket.m_iPortalAppearanceMap	= SiKFieldBossManager()->GetPortalAppearanceMap();
	kPacket.m_iPortalDestination	= SiKFieldBossManager()->GetPortalDestination();
	kPacket.m_bPortalOpen			= bPortalOpen;
	kPacket.m_bCoolTime				= bCoolTime;

	SendToGameServer( EGB_BATTLE_FIELD_BOSS_INFO_NOT, kPacket );
}

IMPL_ON_FUNC( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ )
{
	KEGB_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kPacket;

	// �ʵ� ���� �ý��� CoolTime ���ΰ�?
	if( SiKFieldBossManager()->IsCoolTime() == true )
	{
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_02;	// ���� �ʵ� ��Ÿ�� �����Դϴ�. ���� �Ŀ� �ٽ� �õ����ּ���.
		SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = BossFieldOpenProcess( kPacket_.m_iBattleFieldID, CTime::GetCurrentTime().GetTime() );
	SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGB_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT )
{
	const bool bPortalOpen	= SiKFieldBossManager()->IsPortalOpen();
	const bool bCoolTime	= SiKFieldBossManager()->IsCoolTime();

	// ��Ż�� ���� �ִ��� Ȯ��
	if( bPortalOpen == true )
	{
		// ��Ż ���� ó��
		SiKFieldBossManager()->ClosePortal();
	}

	// �� Ÿ�� üũ
	if( bCoolTime == true )
	{
		// �� Ÿ�� ���� ó��
		SiKFieldBossManager()->SetCoolTimeState( false );
		SiKFieldBossManager()->ClearTotalDangerousValue();	// TotalDangerousValue�� ó������ ����Ѵ�.
	}

	// ��Ż�� �����ų� �� Ÿ���� ����Ǿ��ٸ� �˸� ��Ŷ�� ������
	if( ( bPortalOpen != SiKFieldBossManager()->IsPortalOpen() ) || ( bCoolTime != SiKFieldBossManager()->IsCoolTime() ) )
	{
		KEGB_BATTLE_FIELD_BOSS_INFO_NOT kPacket;
		kPacket.m_iPortalAppearanceMap	= SiKFieldBossManager()->GetPortalAppearanceMap();
		kPacket.m_iPortalDestination	= SiKFieldBossManager()->GetPortalDestination();
		kPacket.m_bPortalOpen			= SiKFieldBossManager()->IsPortalOpen();
		kPacket.m_bCoolTime				= SiKFieldBossManager()->IsCoolTime();

		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, EGB_BATTLE_FIELD_BOSS_INFO_NOT, kPacket );
	}
}

IMPL_ON_FUNC( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ )
{
	KEGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
	kPacket.m_iOK						= NetError::NET_OK;
	kPacket.m_iBattleFieldID			= kPacket_.m_iBattleFieldID;
	kPacket.m_byteTotalDangerousValue	= SiKFieldBossManager()->GetTotalDangerousValue( kPacket_.m_iBattleFieldID );
	
	SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ )
{
	// ���� �ð� ���
	const CTime tCurrentTime = CTime::GetCurrentTime();
	byte byteValue = kPacket_.m_iTotalDangerousValue;

	if( kPacket_.m_bSet == false )
	{
		byteValue += SiKFieldBossManager()->GetTotalDangerousValue( kPacket_.m_iBattleFieldID );
	}

	KEGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kPacket;

	kPacket.m_iOK = TotalDangerousValueUpdateProcess( kPacket_.m_iBattleFieldID, byteValue, tCurrentTime.GetTime() );

	SendToGSUser( FIRST_SENDER_UID, EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
}
#endif // SERV_BATTLE_FIELD_BOSS
