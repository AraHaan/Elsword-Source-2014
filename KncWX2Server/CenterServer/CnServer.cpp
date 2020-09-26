#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>
#include "CnServer.h"
#include "ActorManager.h"
#include "CnSimLayer.h"
#include "CnNetLayer.h"
#include "socket/NetCommon.h"
#include "CnRoomManager.h"
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewCnPartyManager.h"
#else
	#include "CnPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
#include "DBLayer.h"
#include "CnAccountDBThread.h"
//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
	#include "CnGameDBThread.h"
#endif SERV_TRADE_DDOS_DEFENCE
//}}
#include "CnLogDBThread.h"
#include "CnSMSDBThread.h"
#include "defaultFSM.h"
#include "CnUser.h"

#include "NetError.h"

#include "GameEvent/GameEventManager.h"
#include "X2Data/XSLItemManager.h"
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2009. 5. 23  ������	�ð����
#include "KDropTable.h"
//}}
//{{ 2009. 12. 21  ������	������ Ȯ��
#include "Enum/Enum.h"
//}}
//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	#include "CnScriptRefreshManager.h"
#endif SERV_REALTIME_SCRIPT
//}}
//{{ 2010. 11. 15	������	���� ��� ó�� ����
#ifdef SERV_EVENT_SCRIPT_REFRESH
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_SCRIPT_REFRESH
//}}
//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	#include "DungeonEventManager.h"
#endif SERV_TIME_DROP_MONSTER_EVENT
//}}

#ifdef SERV_ADD_EVENT_DB
#include "CnEventDBThread.h"
#endif //SERV_ADD_EVENT_DB

//{{ 2011. 03. 29	������	TBB �޸� ������
//#ifdef SERV_TBB_MALLOC_PROXY_TEST
//	#include <tbb/tbbmalloc_proxy.h>
//#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/ReleaseTickManager.h"
	#include "EventDataRefresh/EventNotifyMsgServerInfo.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	#include "InteriorUdpSession.h"
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#include "GameSysVal/GameSysVal.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS


#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" ��ũ��Ʈ ������ �Ϸ��Ͽ����ϴ�!" ); \
	Si##className()->DumpToLogFile(); \
	break; \

//#include "vld.h"

NiImplementRTTI( KCnServer, KBaseServer );
ImplPfID( KCnServer, PI_CN_SERVER );

#define CLASS_TYPE KCnServer

ImplToStringW( KCnServer )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

KCnServer::KCnServer(void)
{
}

KCnServer::~KCnServer(void)
{
	//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	KInteriorUdpSession::GetKObj()->ReleaseKObj();
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}
}

#include <lua_tinker.h>

void KCnServer::RegToLua()
{
    KBaseServer::RegToLua();

	lua_tinker::class_add<KCnServer>( g_pLua, "KCnServer" );
	lua_tinker::class_inh<KCnServer, KBaseServer>( g_pLua );
	lua_tinker::class_def<KCnServer>( g_pLua, "dump", &KCnServer::Dump );

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	lua_tinker::class_def<KCnServer>( g_pLua, "SetActiveLagCheck", &KCnServer::SetActiveLagCheck_LUA );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

	lua_tinker::decl( g_pLua, "Server", this );
}

const wchar_t*  g_szConfigFile = L"config_cn";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KCnServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KCnAccountDBThread( szDSN, bDBConnStr );
		//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
	case KDBLayer::DC_GAME:
		return new KCnGameDBThread( szDSN, bDBConnStr );
#endif SERV_TRADE_DDOS_DEFENCE
		//}}
    case KDBLayer::DC_LOG:
        return new KCnLogDBThread( szDSN, bDBConnStr );
	case KDBLayer::DC_SMS:
		return new KCnSMSDBThread( szDSN, bDBConnStr );
#ifdef SERV_ADD_EVENT_DB
	case KDBLayer::DC_EVENT:
		return new KCnEventDBThread( szDSN, bDBConnStr );
#endif //SERV_ADD_EVENT_DB

    default:
        START_LOG( cerr, L"�����Ϸ��� DB ������ �̻���." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KCnAccountDBThread( szDSN, bDBConnStr );
    }
}

bool KCnServer::Init()
{
    KSimLayer::KeepObject<KCnSimLayer>();
    KNetLayer::KeepObject<KCnNetLayer>();
    KActorManager::KeepObject<KActorManager>();
	//081020.hoons.
	KRoomManager::KeepObject<KCnRoomManager>();

    KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KCnUser, KDefaultFSM, 512*1024> ) );
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_SERVER );
#endif SERV_KEVENT_FROM
	//}}

    _JIF( KBaseServer::Init(), return false );

	//{{ 2010. 02. 20  ������	ä�� ��Ƽ ���� - RoomManager Thread�� �����ϱ� ���� �ʱ�ȭ ���Ѿ���.
#ifdef SERV_CHANNEL_PARTY
	SiKCnPartyManager()->Init();
#endif SERV_CHANNEL_PARTY
	//}}

	//081020.hoons. BaseServer::Init() �� ��� ����Ľ̷�Ʈ�� ����ֱ⶧���� �Ľ��� init�� �ؾ���.
	_JIF( GetKCnRoomManager()->Init(), return false );	

	SetServerReady( true );

	//{{ 2009. 8. 27  ������	server ready
	OnServerReadyComplete();
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    return true;
}

void KCnServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

	//{{ 2009. 8. 27  ������	����� �α�
	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKAbuserLogManager()->UpdateAllAbuserList();
#else
	SiKAbuserLogManager()->SendAbuserListReq();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	//}}

	//{{ 2009. 5. 23  ������	�ð� ���
	SendToLogDB( DBE_GET_TIME_DROP_INFO_REQ );
	//}}

	// �̺�Ʈ ���� ���
	SiKGameEventManager()->Init();

	//{{ 2010. 11. 15	������	���� ��� ó�� ����
#ifdef SERV_EVENT_SCRIPT_REFRESH
		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
	#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		SiKEventNotifyMsgServerInfo()->Init();
	#else
		SiKGameEventScriptManager()->Init();
	#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}

	//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	SiKDungeonEventManager()->Init();
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	SiKCnScriptRefreshManager()->Init();
#endif SERV_REALTIME_SCRIPT
	//}}	

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\CenterServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\CenterServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	// ������ �̸� ǥ��
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );
}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
void KCnServer::SetActiveLagCheck_LUA( bool bActiveLagCheck ) 
{ 
	KECN_SET_ACTIVE_LAGCHECK_NOT kPacket;
	kPacket.m_bActiveLagCheck = bActiveLagCheck;

	BroadCastAllGS( ECN_SET_ACTIVE_LAGCHECK_NOT, kPacket );

	START_LOG( cout, L"����üũ : " )
		<< BUILD_LOG( bActiveLagCheck )
		<< END_LOG;
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2


void KCnServer::ShutDown()
{
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    // ���� �ƹ� ����ó���� ����Ǳ� ��.
    KBaseServer::ShutDown();        // �̰����� ��� ���ᰡ ������. ������ Layer�鵵 ��� �Ҹ��.

	//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	KInteriorUdpSession::GetKObj()->ShutDown();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

    GetKCnRoomManager()->ShutDown();

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT	
	SiKCnScriptRefreshManager()->EndThread();
	KCnScriptRefreshManager::ReleaseInstance();
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2010. 03. 10  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	KCnPartyManager::ReleaseInstance();
#endif SERV_CHANNEL_PARTY
	//}}
}

bool KCnServer::DestroyAllSession()
{
    // NetLayer�� Accepter�� ���� ����. ���� ��� Layer���� ����ִ�.
 
    KActorManager* pkActorManager = KActorManager::GetKObj();
   
    START_LOG( cout, L"���� ���� : " << pkActorManager->GetCount() << L" ��" );

	//{{ 2009. 1. 30  ������	���� �˴ٿ� ���� ���� ����
	int iIndex = KStatistics::eSIColDR_Server_Shutdown;

	pkActorManager->ForEach( boost::bind( &KActor::SetDisconnectReason, _1, iIndex ) );
	//}}

    pkActorManager->ForEach( boost::bind( &KActor::ReserveDestroy, _1 ) );

    DWORD dwBeginTick = ::GetTickCount();

    // �������� ���� ���� :
    // 1. ��� ���� ��ü�� �Ҹ�Ǿ�� �ϰ� 
    // 2. ��� db event�� ó�� fetch �Ǿ�� �Ѵ�. (ó�����ϼ� ����)
    bool bSuccess = pkActorManager->GetCount() == 0 &&
        SiKDBLayer()->GetTotalEventNum() == 0;

    while( !bSuccess && ::GetTickCount() - dwBeginTick < m_dwShutDownWait )   // ������ ���� �ð��� ������ �׳� ���.
    {
        ::Sleep( 500 );

        bSuccess = pkActorManager->GetCount() == 0 &&
            SiKDBLayer()->GetTotalEventNum() == 0;
    }

    LOG_CONDITION( bSuccess, cout, cerr )
        << BUILD_LOG( m_dwShutDownWait )
        << BUILD_LOG( pkActorManager->GetCount() )
        << BUILD_LOG( SiKDBLayer()->GetTotalEventNum() )
        << BUILD_LOG( ::GetTickCount() - dwBeginTick );

    return bSuccess;
}

void KCnServer::Tick()
{
    KBaseServer::Tick();

    //if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwServerInfoUpdateGap )
    //{
    //    if( GetServerReady() )
    //    {
    //        WriteServerInfoToDB();
    //    }
    //}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\CenterServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\CenterServerWrite"));
			SiKProcessCommunicationManager()->Begin();
			m_bServerRunningProcessCommunicationOnOff = false;
		}
		else if (SiKGameSysVal()->GetProcessCommunication() == false && m_bServerRunningProcessCommunicationOnOff == false)
		{
			SiKProcessCommunicationManager()->ShutDown();

			m_bServerRunningProcessCommunicationOnOff = true;
		}
		m_tTimeProcessCommunicationONOFF.restart();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KCnServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_ACK, KDBE_CENTER_SERVER_ON_ACK );
       _CASE( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
	   _CASE( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT );
		//{{ 2009. 4. 22  ������	������������Ʈ
		CASE( ECN_NEW_PVP_USER_INFO_NOT );
		CASE( ECN_UPDATE_PVP_USER_INFO_NOT );
	   _CASE( ECN_CHECK_INVALID_PVP_USER_NOT, UidType );
		//}}
		//{{ 2009. 5. 23  ������	�ð����		
		CASE( DBE_CHECK_TIME_DROP_RESULT_ACK );
		//}}
		//{{ 2009. 6. 1  ������		ä���̵�
		CASE( ECN_CHECK_CHANNEL_CHANGE_REQ );
		CASE( ECN_CHECK_CHANNEL_CHANGE_ACK );
		//}}
		//{{ 2009. 6. 3  ������		Ȯ����
	   _CASE( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT );
		//}}
		//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	   _CASE( ESR_ORDER_TO_CENTER_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );	   
#endif SERV_REALTIME_SCRIPT
		//}}
	   //{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_LOG_SYSTEM_NEW
	   //}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		CASE( DBE_CHECK_EVENT_UPDATE_ACK );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
		CASE( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	   
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	   //{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	   _CASE( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	   //}}
#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	   _CASE( ECN_ADD_COMMON_FLAG_NOT, KECN_ADD_COMMON_FLAG_NOT );
	   _CASE( ECN_DEL_COMMON_FLAG_NOT, KECN_DEL_COMMON_FLAG_NOT );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
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

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_CENTER_SERVER_ON_ACK )
{
	//��ŸƮ�� ���������� ���α׷��� ����
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		::exit( 0 );
		return;
	}

	SetServerInfo( kPacket_.m_kServerInfo );
	//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	KInteriorUdpSession::KeepObject<KInteriorUdpSession>();
	KInteriorUdpSession::GetKObj()->Init( KNetLayer::GetKObj()->GetNCUDPPort() );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}
	SetProxyID( kPacket_.m_iProxyID );
	GetKCnRoomManager()->PrepareSquare( kPacket_.m_vecSquareList );

    SetEvent( m_spEventAck.get() );
}

_IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList )
{
    SetServerList( kPacket_ );
}

_IMPL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT )
{
	//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS )
	{
		// ��� ���Ӽ����� ��ε�ĳ����
		BroadCastAllGS( ECN_NOTIFY_MSG_NOT, kPacket_ );
	}
	//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
	else if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM_ALL_NOTIFY )
	{
		// ��� ���Ӽ����� ��ε�ĳ����
		kPacket_.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM;
		BroadCastAllGS( ECN_NOTIFY_MSG_NOT, kPacket_ );
	}
#endif SERV_NAVER_PROMOTIONS
	//}} 
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	else if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
	{
		// ��� ���Ӽ����� ��ε�ĳ����
		BroadCastAllGS( ECN_NOTIFY_MSG_NOT, kPacket_ );
	}
#endif SERV_SHUTDOWN_SYSTEM
	//}}
	else
#endif SERV_INTEGRATION
	//}}
	{
		// ���� �������� ���Ӽ����� ��ε�ĳ����
		BroadCastSameGroupGS( ECN_NOTIFY_MSG_NOT, kPacket_ );
	}
}

//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

_IMPL_ON_FUNC( ESR_ORDER_TO_CENTER_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// ���ͼ����� ���� ������� Ȯ��!
	if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_NONE < kPacket_.m_iOrderType  && 
		KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SENTINEL > kPacket_.m_iOrderType )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CN_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		SiKCnScriptRefreshManager()->QueueingEvent( spEvent );
	}
	// �α��μ��� �Ǵ� ���Ӽ����� ���� ������� Ȯ��!
	else if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SENTINEL < kPacket_.m_iOrderType  && 
			 KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SENTINEL > kPacket_.m_iOrderType )
	{
		BroadCastAllGS( ESR_ORDER_TO_GAME_SERVER_NOT, kPacket_ );
	}
	else
	{
		START_LOG( cerr, L"���ǵ��� ���� ��ũ��Ʈ ���� ��û Ÿ���Դϴ�!" )
			<< BUILD_LOG( kPacket_.m_iOrderType )
			<< END_LOG;
	}
}

#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_LOG_MANAGER, KLogManager );		

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}

#endif SERV_LOG_SYSTEM_NEW
//}}

//{{ 2009. 4. 22  ������	������������Ʈ
IMPL_ON_FUNC( ECN_NEW_PVP_USER_INFO_NOT )
{
	// ��� ���Ӽ����� ��ε�ĳ����
	BroadCastAllGS( ECN_NEW_PVP_USER_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( ECN_UPDATE_PVP_USER_INFO_NOT )
{
	// ��� ���Ӽ����� ��ε�ĳ����
	BroadCastAllGS( ECN_UPDATE_PVP_USER_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ECN_CHECK_INVALID_PVP_USER_NOT, UidType )
{
	// ��� ���Ӽ����� ��ε�ĳ����
	BroadCastAllGS( ECN_CHECK_INVALID_PVP_USER_NOT, kPacket_ );
}
//}}

//{{ 2009. 5. 23  ������	�ð����
IMPL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_ACK )
{
	// �ð� ��� �������� üũ �����ߴٸ�?
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Ư�� �ð� ��� �̺�Ʈ ���� �ߴµ� ����� �Գ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_iGSUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_iTimeDropID )
			<< BUILD_LOG( kPacket_.m_iRewardItemID )
			<< BUILD_LOG( kPacket_.m_wstrRewardItemName )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< END_LOG;
		return;
	}

	// �������� ����ó���Ѵ�!
	KEGS_GET_ITEM_NOT kDropNot;
	kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_DROP_EVENT_ITEM;
	kDropNot.m_GetUnitUID	= kPacket_.m_iUnitUID;
	kDropNot.m_iItemID		= kPacket_.m_iRewardItemID;
	kDropNot.m_iDropItemUID = kPacket_.m_iDropItemUID;
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );

	// ��ü ���� ������!
	KEGS_NOTIFY_MSG_NOT kNotMsg;
	kNotMsg.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM;
	kNotMsg.m_Count = 1;
	kNotMsg.m_wstrMSG = kPacket_.m_wstrNickName;
	kNotMsg.m_wstrMSG += L";";
	kNotMsg.m_wstrMSG += kPacket_.m_wstrRewardItemName;
	//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	BroadCastSameGroupGS( ECN_NOTIFY_MSG_NOT, kNotMsg );
#else
	BroadCastAllGS( ECN_NOTIFY_MSG_NOT, kNotMsg );
#endif SERV_INTEGRATION
	//}}	
}
//}}

//{{ 2009. 6. 1  ������		ä���̵�
IMPL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_REQ )
{
	// �ش� ���Ӽ����� ����
	SendToGSServer( kPacket_.m_iDestGSUID, ECN_CHECK_CHANNEL_CHANGE_REQ, kPacket_ );
}

IMPL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_ACK )
{
	// ������ �������� ���Ӽ����� ����
	SendToGSCharacter( kPacket_.m_iSourGSUID, kPacket_.m_iCharUID, ECN_CHECK_CHANNEL_CHANGE_ACK, kPacket_ );
}
//}}

//{{ 2009. 6. 3  ������		Ȯ����
_IMPL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT )
{
	// ��ü ���Ӽ����� ��ε�ĳ����
	//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	// ���� ���������Ը� ����!
	BroadCastSameGroupGS( ECN_MEGAPHONE_MESSAGE_NOT, kPacket_ );
#else
	BroadCastAllGS( ECN_MEGAPHONE_MESSAGE_NOT, kPacket_ );
#endif SERV_INTEGRATION
	//}}	
}
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_RELEASE_TICK_UPDATE_ACK )
{
	std::map< int, int >::iterator it;
	
	for( it=kPacket_.m_mapReleaseTick.begin(); it != kPacket_.m_mapReleaseTick.end(); )
	{
		// ��Ŷ���� �Ѱܹ��� m_mapReleaseTick �߿��� Abuser Type�� ������ �Ѿ�� 
		if( ( RTT_ABUSER_TYPE_PVP < it->first ) || ( RTT_ABUSER_TYPE_TRAFFIC < it->first ) )
		{
			kPacket_.m_mapReleaseTick.erase( it++ );
			continue;
		}
		++it;
	}

	if( !kPacket_.m_mapReleaseTick.empty() )
	{
		SiKReleaseTickManager()->SetReleaseTickData( kPacket_.m_mapReleaseTick );
	}
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT )
{

	BroadCastAllGS( ECN_UDP_KICK_GAMEEDIT_NOT, kPacket_ );
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH // ���� �������� ��� ���� ������ ��ε� ĳ����
_IMPL_ON_FUNC( ECN_ADD_COMMON_FLAG_NOT, KECN_ADD_COMMON_FLAG_NOT )
{
	KEGS_ADD_COMMON_FLAG_NOT kPacket;
	kPacket.dwFlag = kPacket_.dwFlag;
	BroadCastAllGS( EGS_ADD_COMMON_FLAG_NOT, kPacket );
}
_IMPL_ON_FUNC( ECN_DEL_COMMON_FLAG_NOT, KECN_DEL_COMMON_FLAG_NOT )
{
	KEGS_DEL_COMMON_FLAG_NOT kPacket;
	kPacket.dwFlag = kPacket_.dwFlag;
	BroadCastAllGS( EGS_DEL_COMMON_FLAG_NOT, kPacket );
}
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM

IMPL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK )
{

	std::map<int , int>::const_iterator cmit;

	cmit = kPacket_.m_mapReleaseTick.find( KGameEventManager::ERTT_EVENT_DB_SCRIPT_CHECK );

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKGameEventManager()->GetEventDBScriptReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L" �̺�Ʈ DB ��ũ��Ʈ ������ ���� �� ���� ���� �޾� �ɴϴ�." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

			SendToEventDB( DBE_EVENT_DB_SCRIPT_REQ );

			SiKGameEventManager()->SetEventDBScriptReleaseTick( iNewReleaseTick );
		}
	}


#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	cmit = kPacket_.m_mapReleaseTick.find( KGameEventManager::ERTT_EVENT_CHECK );

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKGameEventManager()->GetEventReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L"�̺�Ʈ �ٲ� ���� Ȯ���Ͽ����Ƿ� ���� �޾ƿɴϴ�." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

			SendToEventDB( DBE_EVENT_UPDATE_REQ );

			SiKGameEventManager()->SetEventReleaseTick( iNewReleaseTick );
		}
	}
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
}
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
IMPL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapReleaseTick.find( KGameEventManager::ERTT_EVENT_CHECK );

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKGameEventManager()->GetEventReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L"�̺�Ʈ �ٲ� ���� Ȯ���Ͽ����Ƿ� ���� �޾ƿɴϴ�." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

#ifdef SERV_ADD_EVENT_DB
			SendToEventDB( DBE_EVENT_UPDATE_REQ );
#else //SERV_ADD_EVENT_DB
			SendToLogDB( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB


			SiKGameEventManager()->SetEventReleaseTick( iNewReleaseTick );
		}
	}
}
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
