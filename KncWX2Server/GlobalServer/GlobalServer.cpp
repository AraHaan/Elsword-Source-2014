#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>
#include "GlobalServer.h"
#include "ActorManager.h"
#include "GlobalSimLayer.h"
#include "GlobalNetLayer.h"
#include "socket/NetCommon.h"
#include "DBLayer.h"

#include "GlobalLogDBThread.h"
//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "GlobalSMSDBThread.h"
#endif SERV_MORNITORING
//}}
//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "GlobalGameDBThread.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
#include "GlobalAccountDBThread.h"
#include "defaultFSM.h"
#include "GlobalUser.h"

//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	#include "GlobalScriptRefreshManager.h"
#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

#include "NetError.h"

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
#include "WorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

// ���� ����
#include "GlobalServerCCUManager.h"

//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "MatchMakingManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2012. 02. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "AutoPartyMakingManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	#include "X2Data/XSLQuestManager.h"
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	#include "Temp.h"
#endif SERV_EVENT_JACKPOT
//}}

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
#include "GameEvent/GameEventManager.h"
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#include "GameSysVal/GameSysVal.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2013. 05. 14	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	#include "BlockListManager.h"
#endif SERV_BLOCK_LIST
//}}

#ifdef SERV_ADD_EVENT_DB
	#include "GlobalEventDBThread.h"
#endif //SERV_ADD_EVENT_DB

//{{ 2012. 09. 06	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 06	��ȫ��	�۷ι� �̼� �Ŵ���

//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-01	// �ڼ���
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLFieldBossData.h"
	#include "FieldBossManager.h"
#endif // SERV_BATTLE_FIELD_BOSS

//#include "vld.h"

NiImplementRTTI( KGlobalServer, KBaseServer );
ImplPfID( KGlobalServer, PI_GLOBAL_SERVER );

#define CLASS_TYPE KGlobalServer

ImplToStringW( KGlobalServer )
{
	return START_TOSTRING_PARENTW( KBaseServer );
}

KGlobalServer::KGlobalServer(void)
#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
: m_iTimeEnchantEventUID( 0 )
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-29	// �ڼ���
, m_iExchangeLimitUID( 0 )
#endif // SERV_ITEM_EXCHANGE_LIMIT
{
	m_kTimer.restart();
	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	m_kTimerRandomQuest.restart();
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	m_kTimerShutDown.restart();
#endif SERV_SHUTDOWN_SYSTEM
	//}}
}

KGlobalServer::~KGlobalServer(void)
{
}

#include <lua_tinker.h>

void KGlobalServer::RegToLua()
{
	KBaseServer::RegToLua();

	lua_tinker::class_add<KGlobalServer>( g_pLua, "KGlobalServer" );
	lua_tinker::class_inh<KGlobalServer, KBaseServer>( g_pLua );

	lua_tinker::class_def<KGlobalServer>( g_pLua, "dump",				&KGlobalServer::Dump );
	//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
	lua_tinker::class_def<KGlobalServer>( g_pLua, "SMSTest",				&KGlobalServer::SMSTest_LUA );
#endif SERV_SMS_TEST
	//}}

	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	lua_tinker::class_def<KGlobalServer>( g_pLua, "AddMSGBeforShutDown",				&KGlobalServer::AddMSGBeforShutDown_LUA );
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	lua_tinker::decl( g_pLua, "Server", this );
}

const wchar_t*  g_szConfigFile = L"config_gb";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KGlobalServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
	switch( iDBConnectionInfo )
	{
	case KDBLayer::DC_ACCOUNT:
		return new KGlobalAccountDBThread( szDSN, bDBConnStr );
		//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case KDBLayer::DC_GAME:
		return new KGlobalGameDBThread( szDSN, bDBConnStr );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	case KDBLayer::DC_LOG:
		return new KGlobalLogDBThread( szDSN, bDBConnStr );
		//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	case KDBLayer::DC_SMS:
		return new KGlobalSMSDBThread( szDSN, bDBConnStr );
#endif SERV_MORNITORING
		//}}
#ifdef SERV_ADD_EVENT_DB
	case KDBLayer::DC_EVENT:
		return new KGlobalEventDBThread( szDSN, bDBConnStr );
#endif //SERV_ADD_EVENT_DB
	default:
		START_LOG( cerr, L"�����Ϸ��� DB ������ �̻���." )
			<< BUILD_LOG( iDBConnectionInfo )
			<< END_LOG;
		return new KGlobalLogDBThread( szDSN, bDBConnStr );
	}
}

bool KGlobalServer::Init()
{
	KSimLayer::KeepObject<KGlobalSimLayer>();
	KNetLayer::KeepObject<KGlobalNetLayer>();
	KActorManager::KeepObject<KActorManager>();

	KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KGlobalUser, KDefaultFSM, 512*1024> ) );
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_SERVER );
#endif SERV_KEVENT_FROM
	//}}

	_JIF( KBaseServer::Init(), return false );

	//WriteServerInfoToDB();

	//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	SiKMatchMakingManager()->Init();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	SiKAutoPartyMakingManager()->Init();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SetServerReady( true );

	//{{ 2009. 8. 27  ������	server ready
	OnServerReadyComplete();
	//}}

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	SiCXSLQuestManager()->SelectTodayRandomQuest( true );
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	m_kTimerEarthQuake.restart();
	m_iNextEarthQuake = SiKWorldMissionManager()->GetRandomEarthQuakeTime();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	// DB�� ���� �̺�Ʈ ������ ��û�Ѵ�.
	SendToLogDB( DBE_JACKPOT_EVENT_INIT_REQ );
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	return true;
}

void KGlobalServer::OnServerReadyComplete()
{
#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
	SendToLogDB( DBE_TIME_ENCHANT_EVENT_INFO_REQ );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-29	// �ڼ���
	SendToLogDB( DBE_EXCHANGE_LIMIT_INFO_REQ );
#endif // SERV_ITEM_EXCHANGE_LIMIT

	KBaseServer::OnServerReadyComplete();

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	SiKGlobalScriptRefreshManager()->Init();	
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
	// �̺�Ʈ ���� ���
	SiKGameEventManager()->Init();
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\GlobalServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\GlobalServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS
	//{{ 2013. 05. 14	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	SiKBlockListManager()->Init();
#endif SERV_BLOCK_LIST
	//}}

	// ������ �̸� ǥ��
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );
}

void KGlobalServer::ShutDown()
{
	//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	SiKMatchMakingManager()->ShutDown();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	SiKAutoPartyMakingManager()->ShutDown();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT	
	SiKGlobalScriptRefreshManager()->EndThread();
	KGlobalScriptRefreshManager::ReleaseInstance();
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	// ���� �ƹ� ����ó���� ����Ǳ� ��.
	KBaseServer::ShutDown();        // �̰����� ��� ���ᰡ ������. ������ Layer�鵵 ��� �Ҹ��.
}

bool KGlobalServer::DestroyAllSession()
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

void KGlobalServer::Tick()
{
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-01	// �ڼ���
	// ���� �ð� ���
	const CTime tCurrentTime = CTime::GetCurrentTime();
#endif // SERV_BATTLE_FIELD_BOSS

	KBaseServer::Tick();

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	// ���� �̼��� �������̸�
	// 1�� ���� ������ ���� DB�� ����Ѵ�
	if( m_kTimer.elapsed() >= 60.0 )
	{
		SiKWorldMissionManager()->WriteWorldMissionCrystalCount();
		m_kTimer.restart();
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	if( m_kTimerRandomQuest.elapsed() >= 30.0 )
	{
		if( SiCXSLQuestManager()->SelectTodayRandomQuest( false ) == true )
		{
			KEGB_TODAY_RANDOM_QUEST_ACK kAck;

			SiCXSLQuestManager()->GetRandomQuestGroupList( kAck.m_mapTodayRandomQuest );

			BroadCastAllGS( EGB_TODAY_RANDOM_QUEST_ACK, kAck );
		}
		m_kTimerRandomQuest.restart();
	}
#endif SERV_RANDOM_DAY_QUEST
	//}}

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
	// ���� �̺�Ʈ
	SiKGameEventManager()->Tick();
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER
	
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	CheckSendMSGBeforeShutDown();
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	CheckEarthQuakeEffect();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	CheckJackPotEvent();
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
	CheckHeroPvpUserList();
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\GlobalServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\GlobalServerWrite"));
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

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
	CheckTimeEnchantEvent();
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	CheckExchangeLimit();
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_GLOBAL_MISSION_MANAGER
	// �۷ι� �̼� �������� 1�� ���� DB�� ���
	if( m_kTimerGlobalMission.elapsed() >= 60.0 )
	{
		SiKGlobalMissionManager()->WriteGlobalMissionInfo();
		m_kTimerGlobalMission.restart();
	}
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-01	// �ڼ���
	CheckFieldBossSystem( tCurrentTime );
#endif // SERV_BATTLE_FIELD_BOSS
}

//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
void KGlobalServer::SMSTest_LUA()
{
    // ���� ����!
    KDBE_SEND_PHONE_MSG_NOT kPacketNot;
#ifdef SERV_SMS_TOTAL_MANAGER
    SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_TEST, kPacketNot.m_vecPhoneNum );
#else
    kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );
#endif SERV_SMS_TOTAL_MANAGER

    kPacketNot.m_wstrSMSMessage = L"SMS �׽�Ʈ ���� ����!";
	SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

	START_LOG( cout, L"SMS �׽�Ʈ ���� ����!" );
}
#endif SERV_SMS_TEST
//}}

void KGlobalServer::ProcessEvent( const KEventPtr& spEvent_ )
{
	KSerializer ks;
	ks.BeginReading( &spEvent_->m_kbuff );

	DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

	switch( spEvent_->m_usEventID )
	{
	   _CASE( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK );
		CASE_NOPARAM( DBE_UPDATE_SERVER_INFO_ACK );

		//{{ 2010. 07. 19  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	   _CASE( ESR_ORDER_TO_GLOBAL_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
		//}}

		//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	   _CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
		//}}

		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		CASE( DBE_GET_DEFENCE_CRYSTAL_ACK );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

		//{{ 2012. 04. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		_CASE( EGB_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, int );
		CASE_NOPARAM( EGB_RESET_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
		CASE( DBE_JACKPOT_EVENT_INIT_ACK );
		CASE( DBE_JACKPOT_EVENT_UPDATE_ACK );
#endif SERV_EVENT_JACKPOT
		//}}
		
#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		CASE( DBE_CHECK_EVENT_UPDATE_ACK );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK 
		CASE( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

		CASE( DBE_EVENT_UPDATE_ACK );
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER	

		//{{ 2013. 05. 14	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
		CASE( DBE_CHECK_BLOCK_LIST_ACK );
#endif SERV_BLOCK_LIST
		//}}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
		CASE( DBE_TIME_ENCHANT_EVENT_INFO_ACK );
		CASE( DBE_TIME_ENCHANT_EVENT_INFO_NOT );
#endif // SERV_TIME_ENCHANT_EVENT
		
#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-29	// �ڼ���
		CASE( DBE_EXCHANGE_LIMIT_INFO_ACK );
		CASE( DBE_EXCHANGE_LIMIT_INFO_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_GLOBAL_MISSION_MANAGER
		CASE( DBE_GET_GLOBAL_MISSION_INFO_ACK );
#endif SERV_GLOBAL_MISSION_MANAGER

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

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK )
{
	//��ŸƮ�� ���������� ���α׷��� ����
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		::exit( 0 );
		return;
	}

	SetServerInfo( kPacket_.m_kServerInfo );

	SetEvent( m_spEventAck.get() );
}

IMPL_ON_FUNC_NOPARAM( DBE_UPDATE_SERVER_INFO_ACK )
{
	// ��� ����.
}

//{{ 2010. 07. 19  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

_IMPL_ON_FUNC( ESR_ORDER_TO_GLOBAL_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// �۷ι������� ���� ������� Ȯ��!
	if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SENTINEL < kPacket_.m_iOrderType  && 
		KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_SENTINEL > kPacket_.m_iOrderType )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GLOBAL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		SiKGlobalScriptRefreshManager()->QueueingEvent( spEvent );
	}
	else
	{
		START_LOG( cerr, L"�������� ���� �ȵǴ� ���� ��û Ÿ���Դϴ�!" )
			<< BUILD_LOG( kPacket_.m_iOrderType )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// ������ Thread���� Refresh�� �ؾ߸� �ϴ°��� ���⼭ ó������!
	switch( kPacket_.m_iOrderType )
	{
		//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	case KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_PVP_MATCH_MANAGER:
		{
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GLOBAL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
			SiKMatchMakingManager()->QueueingEventToMatch( spEvent );
		}
		return;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_AUTO_PARTY_SCRIPT_MANAGER:
		{
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GLOBAL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
			SiKAutoPartyMakingManager()->QueueingEvent( spEvent );
		}
		return;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}

	switch( kPacket_.m_iOrderType )
	{
#ifdef SERV_MORNITORING	// ���� ������ �ؿ��� �߰�
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_MORNITORING_MANAGER, KMornitoringManager );
#endif // SERV_MORNITORING
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_WORLD_MISSION_MANAGER, KWorldMissionManager );
		//{{ 2011. 11. 04	��μ� ���� ���� ����Ʈ �ǽð� ��ġ 
#ifdef SERV_RANDOM_DAY_QUEST
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_QUEST_MANAGER, CXSLQuestManager );
#endif SERV_RANDOM_DAY_QUEST
		//}}
		//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
		//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager );
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GlobalServer ��ũ��Ʈ �ǽð� ��ġ �Ϸ�"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT )
{
	SiKMornitoringManager()->ReserveSMSMessage( kPacket_ );
}
#endif SERV_MORNITORING
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
IMPL_ON_FUNC( DBE_GET_DEFENCE_CRYSTAL_ACK )
{
	SiKWorldMissionManager()->SetInitMissionInfo( kPacket_ );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
void KGlobalServer::AddMSGBeforShutDown_LUA( int iHour, int iMin, int iSec )
{
	if( iHour < 0 || iHour > 23 || iMin < 0 || iMin > 59  || iSec < 0 || iSec > 60 )
	{
		START_LOG( cerr, L"�˴ٿ����� �̸� �˷��ִ� �ð����� �̻��մϴ�." )
			<< BUILD_LOG( iHour )
			<< BUILD_LOG( iMin )
			<< BUILD_LOG( iSec )
			<< END_LOG;

		return;
	}

	CTimeSpan tTime( 0, iHour, iMin, iSec );

	m_vecShutDownMSGTime.push_back( tTime );
}

void KGlobalServer::CheckSendMSGBeforeShutDown()
{
	if( m_kTimerShutDown.elapsed() < 60.0 )
		return;
	
	if( CheckMSGTimeBeforeShutDown() == true )
	{
		KEGS_NOTIFY_MSG_NOT	kPacketNot;
		kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY;
		kPacketNot.m_wstrMSG = L"�˴ٿ��� ���� �����̹Ƿ� 00�ú��� 06�ñ��� ���� ������ ���ѵ˴ϴ�. �����ϰ� �����Ͻñ� �ٶ��ϴ�.";

		SendToGameServer( EGB_SHUT_DOWN_NOTIFY_NOT, kPacketNot );
	}
	m_kTimerShutDown.restart();
}

bool KGlobalServer::CheckMSGTimeBeforeShutDown()
{
	CTime tCurr = CTime::GetCurrentTime();
	tCurr = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), tCurr.GetHour(), tCurr.GetMinute(), 0 );
	CTime tTemp;

	std::vector<CTimeSpan>::iterator vit = m_vecShutDownMSGTime.begin();
	for(  ; vit != m_vecShutDownMSGTime.end() ; ++vit )
	{
		tTemp = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), (*vit).GetHours(), (*vit).GetMinutes(), 0 );
		if( tTemp == tCurr )
		{
			return true;
		}
	}
	return false;
}
#endif SERV_SHUTDOWN_SYSTEM
//}}

//{{ 2012. 04. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
_IMPL_ON_FUNC( EGB_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, int )
{
	START_LOG( cout, L"WorldMissionManager ��ȣ�� ������ ���� ���� ó�� ����.");
	SiKWorldMissionManager()->IncreaseProtectedCrystalCount( kPacket_ );
	START_LOG( cout, L"WorldMissionManager ��ȣ�� ������ ���� ���� ó�� �Ϸ�.");
}

IMPL_ON_FUNC_NOPARAM( EGB_RESET_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT )
{
	START_LOG( cout, L"WorldMissionManager ��ȣ�� ������ ���� �ʱ�ȭ ó�� ����.");
	SiKWorldMissionManager()->ResetProtectedCrystalCount();
	START_LOG( cout, L"WorldMissionManager ��ȣ�� ������ ���� �ʱ�ȭ ó�� �Ϸ�.");
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
void KGlobalServer::CheckEarthQuakeEffect()
{
	if( m_kTimerEarthQuake.elapsed() < m_iNextEarthQuake )
		return;

	m_kTimerEarthQuake.restart();
	m_iNextEarthQuake = SiKWorldMissionManager()->GetRandomEarthQuakeTime();

	START_LOG( cout, L"���� ���� Ÿ��" )
		<< BUILD_LOG( m_iNextEarthQuake )
		<< END_LOG;

	BroadCastAllGS( EGB_EARTH_QUAKE_NOT, char() );
}
#endif SERV_EARTH_QUAKE_EFFECT
//}}

//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
IMPL_ON_FUNC( DBE_JACKPOT_EVENT_INIT_ACK )
{	
	// ���� ���� �ʱ�ȭ�� �����ߴ��� ���� ���� �ð��� Ȯ���� �� �ֵ���
	// ���� ���� �б⿡ �����ߴٸ�, ���α׷��� ����.
	if( SiKJackPotEventManager()->Init( kPacket_.m_vecJackpotEventInitInfo ) == false )
	{
		START_LOG( cerr, L"���� �̺�Ʈ �ʱ�ȭ�� ���� �Ͽ����ϴ�." )
			<< END_LOG;

		//::exit( 0 ); // ���α׷� ������~ ���� SMS���ڸ� ����~
		return;
	}

	START_LOG( cout, L"���� �̺�Ʈ ���� �Ϸ�")
		<< BUILD_LOG( kPacket_.m_vecJackpotEventInitInfo.size() )
		<< END_LOG;
}

IMPL_ON_FUNC( DBE_JACKPOT_EVENT_UPDATE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ���� ����!
		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		kPacketNot.m_wstrSMSMessage = L"[���� ����] ";
		kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L"%d, " ) % kPacket_.m_iUnitUID );

		//{{ 2012. 11. 16	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_EVENT_JACK_POT, kPacketNot.m_vecPhoneNum );
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
		return;
	}

	// ����� ���Ӽ����� �ش�Ǵ� �������� ���� �ѱ����� ����
	std::set<UidType> setServerUIDList;
	UidType iGameServerUID = -1;

	GetServerUIDList( static_cast<int>( KncUid::ExtractServerGroupID( kPacket_.m_iUnitUID ) ), setServerUIDList );

	for( std::set<UidType>::const_iterator it=setServerUIDList.begin(); it != setServerUIDList.end(); ++it )
	{
		KActorPtr spActor( KActorManager::GetKObj()->Get( *it ) );
		if( spActor != NULL )
		{
			iGameServerUID = *it;
			break;
		}
	}

	if( iGameServerUID < 0 )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ��÷�ڰ� ���� �������� ���� UID�� ������ ���߽��ϴ�." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( iGameServerUID )
			<< END_LOG;
		return;
	}

	KDBE_INSERT_REWARD_TO_POST_REQ kPacket;
	kPacket.m_iFromUnitUID		= kPacket_.m_iUnitUID;
	kPacket.m_iToUnitUID		= kPacket_.m_iUnitUID;
	kPacket.m_iRewardType		= KPostItemInfo::LT_JACKPOT_EVENT_REWARD;
	kPacket.m_iRewardID			= 0;
	kPacket.m_wstrMessage		= L" õ�� ������ �ູ\n���� �̺�Ʈ�� ��÷�Ǽ̽��ϴ�!\n";
	kPacket.m_wstrMessage		+= kPacket_.m_wstrCuponNumber;
	kPacket.m_wstrMessage		+= L" ��\nĳ�ü��� ���� ����Ͻø�\n�ؽ� ĳ�ø� �����ص帳�ϴ�.\n������ ��ȿ�Ⱓ�� 2013�� 2�� 28�ϱ����Դϴ�.";
	kPacket.m_bGameServerEvent	= true;
	UidType anTrace[2] = { iGameServerUID, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, EGB_JACKPOT_EVENT_POST_NOT, kPacket );
}

void KGlobalServer::CheckJackPotEvent( void )
{
	std::vector<KJackPotEventWinInfo>	vecJackPotEventWinInfo;
	std::vector<KJackPotEndProcessInfo>	vecJackPotEndProcessInfo;

	SiKJackPotEventManager()->Tick( vecJackPotEventWinInfo, vecJackPotEndProcessInfo );

	// ��÷�� ó��
	BOOST_TEST_FOREACH( const KJackPotEventWinInfo&, kJackPotEventWinInfo, vecJackPotEventWinInfo )
	{
		KGlobalUserList::KGSUserInfo kUserInfo;

		// UnitUID������ ���� ���� ���������� ã�ƺ���
		if( GetKGlobalSimLayer()->GetGSUserInfoByUnitUID( kJackPotEventWinInfo.m_iUnitUID, kUserInfo ) == false )
		{
			// ���� ó�� �ʿ���
			continue;
		}

		KEGS_NOTIFY_MSG_NOT	kPacket;
		kPacket.m_cNotifyType	= KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS;
		kPacket.m_Count			= 1;

		UidType iServerGroupID = KncUid::ExtractServerGroupID( kJackPotEventWinInfo.m_iUnitUID );
		if( iServerGroupID == SEnum::SGI_SOLES )
		{
			kPacket.m_wstrMSG = L"�ַ��� ������ ";
		}
		else if( iServerGroupID == SEnum::SGI_GAIA )
		{
			kPacket.m_wstrMSG = L"���̾� ������ ";
		}
		else
		{
			START_LOG( cerr, L"���� �̺�Ʈ - �߸��� Server Group ID �Դϴ�.")
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( kJackPotEventWinInfo.m_iUnitUID )
				<< END_LOG;
		}

		kPacket.m_wstrMSG += kUserInfo.m_wstrUnitNickName;
		kPacket.m_wstrMSG += L"���� õ�� ������ �ູ! ���� �̺�Ʈ�� ��÷�Ǽż� �ؽ� ĳ�� 1������ �����ص帳�ϴ�^^";

		SendToGameServer( EGB_JACKPOT_EVENT_NOTIFY_NOT, kPacket );
		SendToLogDB( DBE_JACKPOT_EVENT_UPDATE_REQ, kJackPotEventWinInfo );
	}

	// �̺�Ʈ ���� ó��
	BOOST_TEST_FOREACH( const KJackPotEndProcessInfo&, kJackPotEndProcessInfo, vecJackPotEndProcessInfo )
	{
		// ���� ����!
		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		kPacketNot.m_wstrSMSMessage = L"[���� ����] ";
		kPacketNot.m_wstrSMSMessage += kJackPotEndProcessInfo.m_wstrBeginDate;
		kPacketNot.m_wstrSMSMessage += L"~";
		kPacketNot.m_wstrSMSMessage += kJackPotEndProcessInfo.m_wstrEndDate;
		kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L"(%d)" ) % kJackPotEndProcessInfo.m_iValidCuponCount );

		//{{ 2012. 11. 16	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_EVENT_JACK_POT, kPacketNot.m_vecPhoneNum );
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
	}
}

void KGlobalServer::ReserveServerUID( IN const int iServerGroupID, IN const UidType iServerUID )
{
	std::map< int, std::set<UidType> >::iterator it = m_mapServerUIDList.find( iServerGroupID );
	if( it == m_mapServerUIDList.end() )
	{
		std::set<UidType> setServerUIDList;
		std::pair<std::map< int, std::set<UidType> >::iterator, bool> pairResult = m_mapServerUIDList.insert( std::map< int, std::set<UidType> >::value_type( iServerGroupID, setServerUIDList ) );
		if( pairResult.second == false )
		{
			START_LOG( cerr, L"�۷ι� ������ ���� ���� ���̵� ���� ����")
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( iServerUID )
				<< END_LOG;
			return;
		}
		it = pairResult.first;
	}

	it->second.insert( iServerUID );
}

void KGlobalServer::RemoveServerUID( IN const UidType iServerUID )
{
	for( std::map< int, std::set<UidType> >::iterator it=m_mapServerUIDList.begin(); it != m_mapServerUIDList.end(); ++it )
	{
		it->second.erase( iServerUID );
	}
}

void KGlobalServer::GetServerUIDList( IN const int iServerGroupID, OUT std::set<UidType>& setServerUIDList ) const
{
	std::map< int, std::set<UidType> >::const_iterator it = m_mapServerUIDList.find( iServerGroupID );
	if( it == m_mapServerUIDList.end() )
	{
		return;
	}

	setServerUIDList = it->second;
}
#endif SERV_EVENT_JACKPOT
//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
void KGlobalServer::CheckHeroPvpUserList( void )
{
	KEGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT kPacket;

	// GetHeroPvpUserChangeInfo �� �����͸� ���� �ڿ� ���� �Լ���.
	GetKGlobalSimLayer()->GetHeroPvpUserChangeInfo( kPacket.m_vecUserList );
	if( kPacket.m_vecUserList.empty() == true )
	{
		return;
	}

	std::vector< std::pair< UidType, UidType > > vecHeroPvpObserverList;
	GetKGlobalSimLayer()->GetHeroPvpObserverList( vecHeroPvpObserverList );
	if( vecHeroPvpObserverList.empty() == true )
	{
		return;
	}

	std::vector< std::pair< UidType, UidType > >::iterator it;
	for( it = vecHeroPvpObserverList.begin(); it != vecHeroPvpObserverList.end(); ++it )
	{
		SendToGSCharacter( it->first, it->second, EGB_ADMIN_HERO_PVP_USER_CONNECTION_NOT, kPacket );
	}
}
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER

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

IMPL_ON_FUNC( DBE_EVENT_UPDATE_ACK )
{
	if( kPacket_.m_vecEventList.size() > 0 )
	{
		SiKGameEventManager()->SetEvent( kPacket_.m_vecEventList );
	}
}
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

//{{ 2013. 05. 14	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
IMPL_ON_FUNC( DBE_CHECK_BLOCK_LIST_ACK )
{
	std::vector< KBlockInfo > vecNewBlock;
	std::vector< KBlockInfo > vecDelBlock;

	// DB���� ������ �� ����Ʈ�� ������Ʈ����.	
	SiKBlockListManager()->UpdateblockList( kPacket_.m_iReleaseTick, kPacket_.m_mapBlockInfo, vecNewBlock, vecDelBlock );

	// ���� ����Ǵ� ���� �ش� ������ ã�Ƽ� ����Ű��!
	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecNewBlock )
	{
		KGlobalUserList::KGSUserInfo kGSUserInfo;
		if( GetKGlobalSimLayer()->GetGSUserInfoByUserUID( kInfo.m_iBlockTarget, kGSUserInfo ) == true )
		{
			KEGB_UPDATE_BLOCK_RESULT_NOT kPacketNot;
			kPacketNot.m_mapBlockResult.insert( std::make_pair( kInfo.m_cBlockType, true ) );
            SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, EGB_UPDATE_BLOCK_RESULT_NOT, kPacketNot );
		}
	}

	// �����Ǵ� ���� �ش� ������ ã�Ƽ� ���� ��Ű��!
	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecDelBlock )
	{
		if( kInfo.m_cBlockType != KBlockInfo::BT_TRADE_BLOCK )
			continue;

		KGlobalUserList::KGSUserInfo kGSUserInfo;
		if( GetKGlobalSimLayer()->GetGSUserInfoByUserUID( kInfo.m_iBlockTarget, kGSUserInfo ) == true )
		{
			KEGB_UPDATE_BLOCK_RESULT_NOT kPacketNot;
			kPacketNot.m_mapBlockResult.insert( std::make_pair( KBlockInfo::BT_TRADE_BLOCK, false ) );
			SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, EGB_UPDATE_BLOCK_RESULT_NOT, kPacketNot );
		}
	}
}
#endif SERV_BLOCK_LIST
//}}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
bool KGlobalServer::RetrieveTimeEnchantEventData( OUT KTimeEnchantEventInfo& kInfo, OUT int& iTimeEnchantEventUID )
{
	if( m_mapBeginTimeEnchantEventInfo.empty() == true )
	{
		return false;
	}

	std::map<int, KTimeEnchantEventInfo>::iterator it = m_mapBeginTimeEnchantEventInfo.begin();

	kInfo					= it->second;
	iTimeEnchantEventUID	= m_iTimeEnchantEventUID;

	// �ٷ� �����Ѵ�.
	m_mapBeginTimeEnchantEventInfo.erase( it );
	m_setEndTimeEnchantEventInfo.insert( it->first );
	return true;
}

void KGlobalServer::CheckTimeEnchantEvent( void )
{
	if( m_kTimeEnchantEventTimer.elapsed() < 10.0f )
	{
		return;
	}
	m_kTimeEnchantEventTimer.restart();

	__time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

	bool bChanged = ( m_setEndTimeEnchantEventInfo.empty() == false );
	std::map<int, KTimeEnchantEventInfo>::iterator it;
	
	// ����� �̺�Ʈ ó��
	BOOST_TEST_FOREACH( const int, iEventUID, m_setEndTimeEnchantEventInfo )
	{
		m_mapBeginTimeEnchantEventInfo.erase( iEventUID );
	}
	m_setEndTimeEnchantEventInfo.clear();

	for( it = m_mapBeginTimeEnchantEventInfo.begin(); it != m_mapBeginTimeEnchantEventInfo.end(); )
	{
		if( it->second.m_tEndDate <= tCurrentTime )
		{
			it = m_mapBeginTimeEnchantEventInfo.erase( it );
			bChanged = true;
			continue;
		}
		
		++it;
	}

	// ����/����� �̺�Ʈ ó��
	for( it = m_mapTimeEnchantEventInfo.begin(); it != m_mapTimeEnchantEventInfo.end(); )
	{
		if( it->second.m_tEndDate <= tCurrentTime )
		{
			it = m_mapTimeEnchantEventInfo.erase( it );
			continue;
		}
		else if( it->second.m_tBeginDate <= tCurrentTime )
		{
			m_mapBeginTimeEnchantEventInfo.insert( std::map<int, KTimeEnchantEventInfo>::value_type( it->first, it->second ) );
			it = m_mapTimeEnchantEventInfo.erase( it );
			bChanged = true;
			continue;
		}

		++it;
	}

	if( bChanged == true )
	{
		KEGB_TIME_ENCHANT_EVENT_INFO_NOT kPacket;
		GetTimeEnchantEventUID( kPacket.m_iTimeEnchantEventUID );
		GetNumOfBeginTimeEnchantEventInfo( kPacket.m_iNumOfTimeEnchantEvent );
		BroadCastAllGS( EGB_TIME_ENCHANT_EVENT_INFO_NOT, kPacket );
	}
}

IMPL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_ACK )
{
	if( ++m_iTimeEnchantEventUID == 0 )
	{
		++m_iTimeEnchantEventUID;
	}

	m_mapTimeEnchantEventInfo.clear();
	m_mapBeginTimeEnchantEventInfo.clear();
	m_setEndTimeEnchantEventInfo.clear();

	BOOST_TEST_FOREACH( const KTimeEnchantEventInfo&, kInfo, kPacket_.m_vecTimeEnchantEventInfo )
	{
		m_mapTimeEnchantEventInfo.insert( std::map<int, KTimeEnchantEventInfo>::value_type( kInfo.m_iEventUID, kInfo ) );
	}
	
	START_LOG( cout, L"Ư�� �ð� ��ȭ �ο� �̺�Ʈ ����" )
		<< BUILD_LOG( m_iTimeEnchantEventUID )
		<< BUILD_LOG( m_mapTimeEnchantEventInfo.size() )
		<< END_LOG;
}

IMPL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_NOT )
{
	if( CheckTimeEnchantEventUID( kPacket_.m_iTimeEnchantEventUID ) == false )
	{
		return;
	}

	m_mapTimeEnchantEventInfo.insert( std::map<int, KTimeEnchantEventInfo>::value_type( kPacket_.m_kTimeEnchantEventInfo.m_iEventUID, kPacket_.m_kTimeEnchantEventInfo ) );
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-29	// �ڼ���
void KGlobalServer::GetBeginExchangeLimitInfo( OUT std::map< int, int >& mapBeginExchangeLimitInfo ) const
{
	for( EXCHANGE_LIMIT_CONTAINER::const_iterator it = m_listBeginExchangeLimitInfo.begin(); it != m_listBeginExchangeLimitInfo.end(); ++it )
	{
		BOOST_TEST_FOREACH( const int, iDestItemID, it->m_setDestItemID )
		{
			std::pair< std::map< int, int >::iterator, bool > pairResult = mapBeginExchangeLimitInfo.insert( std::map< int, int >::value_type( iDestItemID, it->m_iLimitQuantity ) );
			if( pairResult.second == false )
			{
				pairResult.first->second += it->m_iLimitQuantity;
			}
		}
	}
}

int KGlobalServer::RetrieveExchangeLimitData( IN const int iDestItemID, OUT int& iLimitUID )
{
	iLimitUID = 0;

	if( m_iExchangeLimitUID == 0 )
	{
		return KGlobalServer::ELR_NOT_INITED;
	}

	// ���⼭�� ���� ���� ���� �߿��� ������� ���� ���� ��Ȯ�ϰ� ã�� �ʿ䰡 �ִ�.
	__time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();
	int iResult = KGlobalServer::ELR_PASS;

	for( EXCHANGE_LIMIT_CONTAINER::iterator it = m_listBeginExchangeLimitInfo.begin(); it != m_listBeginExchangeLimitInfo.end(); ++it )
	{
		KExchangeLimitInfo& kInfo = *it;
		
		if( kInfo.m_setDestItemID.find( iDestItemID ) == kInfo.m_setDestItemID.end() )
		{
			// ã�� �ִ� ��ȯ ����� �ƴ϶�� �Ѿ��
			continue;
		}
		
		if( kInfo.m_iLimitQuantity <= 0 )
		{
			// �̹� ��� ������ ������ �����̶�� �Ѿ��
			iResult = KGlobalServer::ELR_EXHAUSTED;
			continue;
		}

		--kInfo.m_iLimitQuantity;
		m_bExchangeLimitInfoChanged	= true;

		iLimitUID	= kInfo.m_iLimitUID;
		iResult		= KGlobalServer::ELR_SUCCEED;
		break;
	}

	return iResult;
}

int KGlobalServer::ReInsertExchangeLimitData( IN const int iLimitUID )
{
	if( m_iExchangeLimitUID == 0 )
	{
		return KGlobalServer::ELR_NOT_INITED;
	}

	for( EXCHANGE_LIMIT_CONTAINER::iterator it = m_listBeginExchangeLimitInfo.begin(); it != m_listBeginExchangeLimitInfo.end(); ++it )
	{
		KExchangeLimitInfo& kInfo = *it;

		if( kInfo.m_iLimitUID != iLimitUID )
		{
			// ã�� �ִ� ����� �ƴ϶�� �Ѿ��
			continue;
		}

		++kInfo.m_iLimitQuantity;
		m_bExchangeLimitInfoChanged = true;
		break;
	}

	return KGlobalServer::ELR_SUCCEED;
}

void KGlobalServer::CheckExchangeLimit( IN const bool bForceNotify /*= false */)
{
	bool bExchangeLimitChanged = false;
	if( KGlobalServer::ELI_CHECK_TERM <= m_kExchangeLimitCheckTimer.elapsed() )
	{
		m_kExchangeLimitCheckTimer.restart();

		__time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

		// ����� �̺�Ʈ ó��
		for( EXCHANGE_LIMIT_CONTAINER::iterator it = m_listBeginExchangeLimitInfo.begin(); it != m_listBeginExchangeLimitInfo.end(); )
		{
			if( it->m_tEndDate <= tCurrentTime )
			{
				it = m_listBeginExchangeLimitInfo.erase( it );
				bExchangeLimitChanged = true;
			}
			else
			{
				++it;
			}
		}

		// ���۵� �̺�Ʈ ó��
		for( EXCHANGE_LIMIT_CONTAINER::iterator it = m_listExchangeLimitInfo.begin(); it != m_listExchangeLimitInfo.end(); )
		{
			// �̹� ����� �̺�Ʈ�� �����Ѵ�.
			if( it->m_tEndDate <= tCurrentTime )
			{
				it = m_listExchangeLimitInfo.erase( it );
			}
			else if( it->m_tBeginDate <= tCurrentTime )
			{
				// ���۵� �̺�Ʈ ó��
				m_listBeginExchangeLimitInfo.push_back( *it );
				it = m_listExchangeLimitInfo.erase( it );
				bExchangeLimitChanged = true;
			}
			else
			{
				++it;
			}
		}
	}

	if( ( bExchangeLimitChanged == true ) ||	// ���� ���۵� ��ȯ ���� ����� �ְų�,
		( ( m_bExchangeLimitInfoChanged == true ) &&	// ������ ��ȭ�� ���� ���̶� ����Ǵ� ���¿���
		  ( KGlobalServer::ELI_QUANTITY_TERM <= m_kExchangeLimitQuantityTimer.elapsed() ) )	||// ����ȭ ���� �ð��� �����ϰų�
		  ( bForceNotify == true )	// ����ȭ ��Ŷ�� ���� ���� ������ ��
		  )
	{
		m_bExchangeLimitInfoChanged = false;
		m_kExchangeLimitQuantityTimer.restart();

		KEGB_EXCHANGE_LIMIT_INFO_NOT kPacket;
		GetExchangeLimitUID( kPacket.m_iExchangeLimitUID );
		GetBeginExchangeLimitInfo( kPacket.m_mapExchangeLimitInfo );
		BroadCastAllGS( EGB_EXCHANGE_LIMIT_INFO_NOT, kPacket );
	}
}

IMPL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_ACK )
{
	if( ++m_iExchangeLimitUID == 0 )
	{
		++m_iExchangeLimitUID;
	}
	
	m_listExchangeLimitInfo	= kPacket_.m_listExchangeLimitInfo;
	m_listBeginExchangeLimitInfo.clear();
	m_bExchangeLimitInfoChanged = false;

	CheckExchangeLimit( true );
	
	START_LOG( cout, L"��ȯ ���� ���� ����" )
		<< BUILD_LOG( m_iExchangeLimitUID )
		<< BUILD_LOG( m_listExchangeLimitInfo.size() )
		<< END_LOG;
}

IMPL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_NOT )
{
	if( CheckExchangeLimitUID( kPacket_.m_iExchangeLimitUID ) == false )
	{
		return;
	}

	ReInsertExchangeLimitData( kPacket_.m_iLimitUID );
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
IMPL_ON_FUNC( DBE_GET_GLOBAL_MISSION_INFO_ACK )
{
	SiKGlobalMissionManager()->SetInitMissionInfo( kPacket_ );
}
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-04	// �ڼ���
void KGlobalServer::CheckFieldBossSystem( IN const CTime tCurrentTime )
{
	if( SiKFieldBossManager()->CheckTerm( tCurrentTime.GetTime() ) == false )
		return;

	const bool bPortalOpen	= SiKFieldBossManager()->IsPortalOpen();
	const bool bCoolTime	= SiKFieldBossManager()->IsCoolTime();

	const CTime tPortalAppearanceTime( SiKFieldBossManager()->GetPortalAppearanceTime() );

	// ��Ż�� ���� �ִ��� Ȯ��
	if( bPortalOpen == true )
	{
		const CTime tPortalHoldingTime = tPortalAppearanceTime + CTimeSpan( 0, 0, SiCXSLFieldBossData()->GetFieldBossPortalHoldingTime_M(), 0 );
		if( tPortalHoldingTime <= tCurrentTime )
		{
			// ��Ż ���� ó��
			SiKFieldBossManager()->ClosePortal();
		}
	}

	// �� Ÿ�� üũ
	if( bCoolTime == true )
	{
		const CTime tFieldBossCoolTime = tPortalAppearanceTime + CTimeSpan( 0, 0, SiCXSLFieldBossData()->GetFieldBossCoolTime_M(), 0 );
		if( tFieldBossCoolTime <= tCurrentTime )
		{
			// �� Ÿ�� ���� ó��
			SiKFieldBossManager()->SetCoolTimeState( false );
			SiKFieldBossManager()->ClearTotalDangerousValue();	// TotalDangerousValue�� ó������ ����Ѵ�.
		}
	}

	// ��Ż�� �����ų� �� Ÿ���� ����Ǿ��ٸ� �˸� ��Ŷ�� ������
	if( ( bPortalOpen != SiKFieldBossManager()->IsPortalOpen() ) || ( bCoolTime != SiKFieldBossManager()->IsCoolTime() ) )
	{
		KEGB_BATTLE_FIELD_BOSS_INFO_NOT kPacket;
		kPacket.m_iPortalAppearanceMap	= SiKFieldBossManager()->GetPortalAppearanceMap();
		kPacket.m_iPortalDestination	= SiKFieldBossManager()->GetPortalDestination();
		kPacket.m_bPortalOpen			= SiKFieldBossManager()->IsPortalOpen();
		kPacket.m_bCoolTime				= SiKFieldBossManager()->IsCoolTime();
		BroadCastAllGS( EGB_BATTLE_FIELD_BOSS_INFO_NOT, kPacket );
	}
}
#endif // SERV_BATTLE_FIELD_BOSS