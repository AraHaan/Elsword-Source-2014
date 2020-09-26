#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>
#include "LoginServer.h"
#include "ActorManager.h"
#include "LoginSimLayer.h"
#include "LoginNetLayer.h"
#include "socket/NetCommon.h"
//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	#include "LoginRoomManager.h"
#endif SERV_PSHOP_AGENCY
//}}
#include "DBLayer.h"
#include "LoginAccountDBThread.h"
//{{ 2009. 11. 9  ������	�α��μ���GameDB
#include "LoginGameDBThread.h"
//}}
#include "LoginLogDBThread.h"
//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "LoginSMSDBThread.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	#include "LoginNXWebDBThread.h"
#endif SERV_WEB_POINT_EVENT
//}}
#ifdef SERV_GLOBAL_BILLING
#include "LoginBillingDBThread.h"
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_COUNTRY_CN
#include <boost/foreach.hpp>
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantRoleRegManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantInfoManager.h"
#endif SERV_COUNTRY_CN

#include "defaultFSM.h"
#include "LoginUser.h"
//{{ 2009. 7. 6  ������		��ŷ����
#include "RankingManager.h"
//}}
//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_TEST
	#include "GuildManager.h"
#endif GUILD_TEST
//}}
//{{ 2010. 01. 15  ������	���Խ���
#ifdef SERV_GUILD_AD
	#include "GuildBoardManager.h"
#endif SERV_GUILD_AD
//}}
//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
	#include "ServerCCUManager.h"
#endif SERV_CCU_NEW
//}}
//{{ 2010. 06. 22  ������	OTP����
#ifdef SERV_OTP_AUTH
	#include "Nexon/AuthCheck.h"
#endif SERV_OTP_AUTH
//}}

//{{ 2010. 06. 22  ������	���������
#ifdef SERV_ACCOUNT_BLOCK
	#include "AccountBlockManager.h"
#endif SERV_ACCOUNT_BLOCK
//}}
//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	#include "LoginScriptRefreshManager.h"
#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	#include "DLLManager.h"
#endif SERV_DLL_MANAGER
//}}
//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	#include "RandomItemEventManager.h"
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}
//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

#include "X2Data/XSLGuildManager.h"
//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	#include "X2Data/XSLTitleManager.h"
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}
//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#include "LoginLocalRankingManager.h"
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallManager.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
#include "X2Data/XSLSkillTree.h"
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#include "NetError.h"

//{{ ������ : [2012/10/9] //	�±� OTP ���� ��� ���
#ifdef SERV_AUTH_TCP_THREAD_MANAGER
#include "../Common/OnlyGlobal/AuthAndBilling/TH/LoginOtpAuthTcpThread.h"
#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}
#ifdef SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/HanNetCafeForSvr.h"
#endif //SERV_COUNTRY_JP

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2011. 03. 29	������	TBB �޸� ������
//#ifdef SERV_TBB_MALLOC_PROXY_TEST
//	#include <tbb/tbbmalloc_proxy.h>
//#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//#include "vld.h"

#ifdef SERV_COUNTRY_PH
#include "../Common/OnlyGlobal/AuthAndBilling/PH/GarenaBillingServer.h"
#endif //SERV_COUNTRY_PH

NiImplementRTTI( KLoginServer, KBaseServer );
ImplPfID( KLoginServer, PI_LOGIN_SERVER );

#define CLASS_TYPE KLoginServer

ImplToStringW( KLoginServer )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

KLoginServer::KLoginServer(void)
{
}

KLoginServer::~KLoginServer(void)
{
}

#include <lua_tinker.h>

void KLoginServer::RegToLua()
{
    KBaseServer::RegToLua();

    //lua_tinker::class_<KLoginServer>( "KLoginServer" )
    //    .inh<KBaseServer>()
    //    .def( "dump", KLoginServer::Dump );

    //lua_tinker::decl( "Server", this );
	lua_tinker::class_add<KLoginServer>( g_pLua, "KLoginServer" );
	lua_tinker::class_inh<KLoginServer, KBaseServer>( g_pLua );
	//{{ 2010. 06. 22  ������	OTP����
#ifdef SERV_OTP_AUTH
	lua_tinker::class_def<KLoginServer>( g_pLua, "SetPassportLevel",	&KLoginServer::SetPassportLevel_LUA );
#endif SERV_OTP_AUTH
	//}}
	lua_tinker::class_def<KLoginServer>( g_pLua, "dump",				&KLoginServer::Dump );
	//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
	lua_tinker::class_def<KLoginServer>( g_pLua, "SMSTest",				&KLoginServer::SMSTest_LUA );
#endif SERV_SMS_TEST
	//}}

#ifdef SERV_COUNTRY_PH
	SiKGarenaBillingServer()->RegToLua();
#endif //SERV_COUNTRY_PH

	lua_tinker::decl( g_pLua, "Server", this );
}

const wchar_t*  g_szConfigFile = L"config_lg";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KLoginServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KLoginAccountDBThread( szDSN, bDBConnStr );
		//{{ 2009. 11. 9  ������	�α��μ���GameDB
	case KDBLayer::DC_GAME:
		return new KLoginGameDBThread( szDSN, bDBConnStr );
		//}}
    case KDBLayer::DC_LOG:
        return new KLoginLogDBThread( szDSN, bDBConnStr );
		//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	case KDBLayer::DC_SMS:
		return new KLoginSMSDBThread( szDSN, bDBConnStr );
#endif SERV_MORNITORING
		//}}
		//{{ 2010. 02. 25  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	case KDBLayer::DC_NX_WEB:
		return new KLoginNXWebDBThread( szDSN, bDBConnStr );
#endif SERV_WEB_POINT_EVENT
		//}}
#ifdef SERV_GLOBAL_BILLING
	case KDBLayer::DC_KOG_BILLING:
		return new KLoginBillingDBThread( szDSN, bDBConnStr );
#endif // SERV_GLOBAL_BILLING
    default:
        START_LOG( cerr, L"�����Ϸ��� DB ������ �̻���." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KLoginAccountDBThread( szDSN, bDBConnStr );
    }
}

//{{ ������ : [2012/10/9] //	�±� OTP ���� ��� ���
#ifdef SERV_AUTH_TCP_THREAD_MANAGER
KThread*        CreateTCPThread( const std::wstring& wstrIP, const short sPort )
{
	return new KLoginOtpAuthTcpThread( wstrIP, sPort );
}
#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}

bool KLoginServer::Init()
{
    KSimLayer::KeepObject<KLoginSimLayer>();
    KNetLayer::KeepObject<KLoginNetLayer>();
    KActorManager::KeepObject<KActorManager>();
	//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	KRoomManager::KeepObject<KLoginRoomManager>();
#endif SERV_PSHOP_AGENCY
	//}}

    KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KLoginUser, KDefaultFSM, 512*1024> ) );
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_SERVER );
#endif SERV_KEVENT_FROM
	//}}

    _JIF( KBaseServer::Init(), return false );

    //WriteServerInfoToDB();

#ifdef SERV_COUNTRY_JP
	// 2010. 05. 27  ������ ��ī�� ��� �ʱ�ȭ �߰�
	int ret = -1;
	ret = HanNetCafe_Init();
	if(ret == HAN_NETCAFE_OK)
	{
		START_LOG( clog, L"��ī�� ��� �ʱ�ȭ �Ϸ�" )				
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"��ī�� ��� �ʱ�ȭ ����" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}
#endif //SERV_COUNTRY_JP

	//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	_JIF( GetKLoginRoomManager()->Init(), return false );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ ������ : [2012/10/9] //	�±� OTP ���� ��� ���
#ifdef SERV_AUTH_TCP_THREAD_MANAGER
	//m_kTHOTPThread.SetTCPConnectionInfo( L"5465165165165", 4654 );	//	���߿� ��ũ��Ʈ�� ����!
	m_kTHOTPThread.Init( 50 ); // ���߿� ��ũ��Ʈ�� ����!
	m_kTHOTPThread.BeginThread();
#endif SERV_AUTH_TCP_THREAD_MANAGER
	//}}

#ifdef SERV_COUNTRY_PH
	_JIF( SiKGarenaBillingServer()->Init(), return false );
#endif //SERV_COUNTRY_PH

	SetServerReady( true );

	//{{ 2009. 8. 27  ������	server ready
	OnServerReadyComplete();
	//}}
	
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    return true;
}

void KLoginServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

	//{{ 2010. 06. 22  ������	OTP����
#ifdef SERV_OTP_AUTH
	// Passport Level
	AuthCheck_SetPassportLevel( m_uiPassportLevel );

	START_LOG( cout2, L"Nexon AuthCheck SetPassportLevel : " << m_uiPassportLevel );
	
	// Locale
	AuthCheck_SetLocale( kLocaleID_KR );

	START_LOG( cout2, L"Nexon AuthCheck SetLocale : kLocaleID_KR" );
#endif SERV_OTP_AUTH
	//}}

	//{{ 2009. 7. 6  ������		��ŷ ����
	SiKRankingManager()->Init();
	//}}

	//{{ 2010. 01. 15  ������	���Խ���
#ifdef SERV_GUILD_AD
	SiKGuildBoardManager()->Init();
#endif SERV_GUILD_AD
	//}}

	//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	SiKRandomItemEventManager()->Init();
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}

	//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
	// ���� ���ڸ��� ���ð��� ���� ���� ������Ʈ
	SiKServerCCUManager()->DBUpdateCCUInfo();
#endif SERV_CCU_NEW
	//}}

	//{{ 2010. 06. 22  ������	���������
#ifdef SERV_ACCOUNT_BLOCK
	SiKAccountBlockManager()->Init();
#endif SERV_ACCOUNT_BLOCK
	//}}

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	SiKLoginScriptRefreshManager()->Init();	
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	SiKDLLManager()->OnServerReadyComplete();
#endif SERV_DLL_MANAGER
	//}}

	//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	SendToGameDB( DBE_LOAD_PSHOP_AGENCY_REQ, char() );
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKLoginLocalRankingManager()->Begin();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}
	
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\LoginServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\LoginServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS
	
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	SiKWeddingHallManager()->Init();
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	// ������ �̸� ǥ��
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );
}

void KLoginServer::ShutDown()
{
#ifdef SERV_COUNTRY_JP
	// 2010. 05. 27  ������ ��ī�� ��� �ʱ�ȭ �߰�
	int ret = -1;
	ret = HanNetCafe_Cleanup();
	if(ret == HAN_NETCAFE_OK)
	{
		START_LOG( clog, L"��ī�� ��� ������ �Ϸ�" )				
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"��ī�� ��� ������ ����" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}	
#endif //SERV_COUNTRY_JP

	//{{ 2013. 3. 15	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKLoginLocalRankingManager()->ShutDown();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	//{{ 2009. 7. 6  ������		��ŷ ���� -	DBLayer�� �ױ����� ��ŷ������ ������Ʈ �ؾ��Ѵ�.
    SiKRankingManager()->ShutDown();
	//}}

	//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	SiKDLLManager()->Shutdown();
#endif SERV_DLL_MANAGER
	//}}

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT	
	SiKLoginScriptRefreshManager()->EndThread();
	KLoginScriptRefreshManager::ReleaseInstance();
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_COUNTRY_PH
	SiKGarenaBillingServer()->ShutDown();
	KGarenaBillingServer::ReleaseInstance();
#endif SERV_COUNTRY_PH

    // ���� �ƹ� ����ó���� ����Ǳ� ��.
    KBaseServer::ShutDown();        // �̰����� ��� ���ᰡ ������. ������ Layer�鵵 ��� �Ҹ��.

	//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	GetKLoginRoomManager()->ShutDown();
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	SiKWeddingHallManager()->ShutDown();
#endif SERV_RELATIONSHIP_SYSTEM
	//}
}

bool KLoginServer::DestroyAllSession()
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

void KLoginServer::Tick()
{
    KBaseServer::Tick();

	//{{ 2008. 5. 27  ������  ü��ID ����
	//{{ 2010. 06. 01  ������	���� ó�� ����
//#ifdef SERV_CCU_NEW
//	if( ::GetTickCount() - m_dwServerInfoUpdateTick > 20 * 1000 )
//#else
//	if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwServerInfoUpdateGap )
//#endif SERV_CCU_NEW
//	//}}    
//    {
//        if( GetServerReady() )
//        {
//            WriteServerInfoToDB();
//        }
//    }
	//}}

	//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
	if( GetServerReady() )
	{
		SiKServerCCUManager()->Tick();
	}
#endif SERV_CCU_NEW
	//}}

	//{{ 2010. 06. 22  ������	���������
#ifdef SERV_ACCOUNT_BLOCK
	SiKAccountBlockManager()->Tick();
#endif SERV_ACCOUNT_BLOCK
	//}}

#ifdef SERV_COUNTRY_PH
	SiKGarenaBillingServer()->Tick();
#endif SERV_COUNTRY_PH

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\LoginServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\LoginServerWrite"));
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

//{{ 2008. 5. 27  ������  ü��ID ����
void KLoginServer::WriteServerInfoToDB()
{
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	{
		//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
		KUpdateCCUInfo kInfo;
		kInfo.m_iServerUID			= GetUID();
		kInfo.m_iConcurrentUser		= GetKLoginSimLayer()->GetGuestConcurrentUserCount(); // ü��ID ����
		kInfo.m_iCurPartyCount		= 0;
		kInfo.m_iPlayGamePartyCount = 0;
		SiKServerCCUManager()->UpdateCCUInfo( kInfo );
#else
		CTime tCurrentTime = CTime::GetCurrentTime();
		KDBE_UPDATE_SERVER_INFO_REQ kPacket;
		kPacket.m_wstrRegDate		  = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		kPacket.m_iServerUID		  = GetUID();
		kPacket.m_iConcurrentUser	  = GetKLoginSimLayer()->GetGuestConcurrentUserCount(); // ü��ID ����
		kPacket.m_iCurPartyCount	  = 0;
		kPacket.m_iPlayGamePartyCount = 0;
		SendToLogDB( DBE_UPDATE_SERVER_INFO_REQ, kPacket );
#endif SERV_CCU_NEW
		//}}		
	}

	m_dwServerInfoUpdateTick = ::GetTickCount();
}
//}}

//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
void KLoginServer::SMSTest_LUA()
{
	// ���� ����!
	KDBE_SEND_PHONE_MSG_NOT kPacketNot;
	//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_TEST, kPacketNot.m_vecPhoneNum );
#else
	kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	kPacketNot.m_wstrSMSMessage = L"SMS �׽�Ʈ ���� ����!";
	SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

	START_LOG( cout, L"SMS �׽�Ʈ ���� ����!" );
}
#endif SERV_SMS_TEST
//}}

void KLoginServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK );
		CASE_NOPARAM( DBE_UPDATE_SERVER_INFO_ACK );
#ifdef SERV_GLOBAL_AUTH 
		CASE( EPUBLISHER_REG_USER_ACK );
		CASE( EPUBLISHER_UNREG_USER_ACK );
#ifdef SERV_COUNTRY_CN
		CASE( EGIANT_INFO_GET_CCU_REQ );
#endif SERV_COUNTRY_CN
#endif // SERV_GLOBAL_AUTH

// ���� ������� �ʴ� �ڵ�� ����
// #ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
// 		CASE( DBE_GET_GIANT_DELETED_UNIT_LIST_ACK );
// #endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
		CASE( ENX_AUTH_LOGIN_ACK );
		CASE( ENX_AUTH_TERMINATE_NOT );
		CASE( ENX_AUTH_MESSAGE_NOT );
		CASE( ENX_AUTH_SYNCHRONIZE_REQ );
#else
		CASE( ENX_COMMON_ACK_NOT );
#endif SERV_PCBANG_AUTH_NEW
		//}}
		//{{ 2009. 7. 6  ������		��ŷ ����
		CASE( DBE_GET_HENIR_RANKING_INFO_ACK );
	   _CASE( ELG_CLEAR_RANKING_NOT, bool );
		CASE_NOPARAM( DMP_DUMP_RANKING_MANAGER_NOT );
		CASE( ELG_ADMIN_NEW_RECORD_TEST_NOT );
		CASE( ELG_ADMIN_NEW_RECORD_NOT );
		CASE( ELG_ADMIN_DELETE_RECORD_NOT );
		//}}
		//{{ 2009. 11. 9  ������	�α��μ���GameDB
		CASE( DBE_GET_WEB_RANKING_INFO_ACK );
		//}}

		//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_TEST
		CASE( DBE_GUILD_LEVEL_UP_ACK );
#endif GUILD_TEST
		//}}

		//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
		CASE( DBE_INSERT_GUILD_CASH_SKILL_POINT_ACK );
		CASE( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK );

	   _CASE( DBE_ADMIN_GET_GUILD_SKILL_POINT_ACK, KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK );
#endif GUILD_SKILL_TEST	
		//}}

		//{{ 2010. 01. 15  ������	���Խ���
#ifdef SERV_GUILD_AD
		CASE( DBE_GET_GUILD_BOARD_INFO_ACK );
		CASE( DBE_ACCEPT_JOIN_GUILD_ACK );
#endif SERV_GUILD_AD
		//}}

		//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT	
		CASE( DBE_GET_WEB_POINT_REWARD_ACK );
	   _CASE( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_ACK, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
		//}}

		//{{ 2010. 06. 22  ������	���������
#ifdef SERV_ACCOUNT_BLOCK
		CASE( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK );
#endif SERV_ACCOUNT_BLOCK
		//}}

		//{{ 2010. 07. 19  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
		//}}
		//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
		CASE( DBE_GET_MODULE_INFO_LIST_ACK );
	   _CASE( DBE_UPDATE_MODULE_INFO_LIST_ACK, KDBE_GET_MODULE_INFO_LIST_ACK );
#endif SERV_DLL_MANAGER
		//}}
	   //{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	   _CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
	   //}}
	   //{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	   CASE( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	   //}}
	   //{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	   CASE( DBE_INSERT_REWARD_TO_POST_ACK );
	   CASE( DBE_LOAD_PSHOP_AGENCY_ACK );
	  _CASE( DBE_BREAK_PSHOP_AGENCY_ACK, KERM_BREAK_PSHOP_AGENCY_NOT );
	  _CASE( ERM_PSHOP_AGENCY_MESSAGE_NOT, KEGS_PSHOP_AGENCY_MESSAGE_NOT );
#endif SERV_PSHOP_AGENCY
	   //}}
		//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
		CASE( ELG_RANKING_TITLE_REWARD_NOT );
		CASE( DBE_INSERT_TITLE_ACK );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
		//}}
		//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
		CASE( DBE_GET_HACKING_MODULE_LIST_ACK );
#endif SERV_HACKING_DLL_CHECK
		//}}
		//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK, SiKLoginLocalRankingManager() );
		CASE( DBE_LOCAL_RANKING_RESET_ACK );
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( DBE_LOCAL_RANKING_RESET_CHECK_ACK, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_RESET_NOT, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, SiKLoginLocalRankingManager() );
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}

		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		CASE( DBE_LOAD_WEDDING_HALL_INFO_ACK );
		_CASE( ELG_ADD_WEDDING_HALL_INFO_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS// �۾���¥: 2013-10-28	// �ڼ���
		CASE( E_DISCONNECT_SERVER_REPORT_NOT );
#endif // SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS

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

//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW

IMPL_ON_FUNC( ENX_AUTH_LOGIN_ACK )
{
	START_LOG( cerr, L"�ؽ� PC�� ���� ���� : �ؿܴ� ���� �α��� Ÿ�� �ȵ�!!!!!!" )
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_iSessionNo )
		<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
		<< BUILD_LOG( kPacket_.m_mapProperty.size() );

	// ���� �����κ��� ���� SessionNo�� ���������� ������Ʈ	
// 	if( GetKLoginSimLayer()->UpdateSessionNoByUserID( kPacket_.m_wstrUserID, kPacket_.m_iSessionNo ) == false )
// 	{
// 		START_LOG( cerr, L"SessionNo ������Ʈ ����." )
// 			<< BUILD_LOG( kPacket_.m_wstrUserID )
// 			<< BUILD_LOG( kPacket_.m_bytePacketType )
// 			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
// 			<< BUILD_LOG( kPacket_.m_iSessionNo )
// 			<< END_LOG;
// 		return;
// 	}
// 
// 	KUserList::KGSUserInfo kInfo;
// 	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) == false )
// 	{
// 		START_LOG( cerr, L"���� ���� ���� �˻� ����." )
// 			<< BUILD_LOG( kPacket_.m_wstrUserID )
// 			<< BUILD_LOG( kPacket_.m_bytePacketType )
// 			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
// 			<< END_LOG;
// 		return;
// 	}
// 
// 	//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
// #ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
// 	GetKLoginSimLayer()->UnRegAuthWaitUser( kInfo.m_nUserUID );
// #endif SERV_DUPLICATE_CONNECT_BUG_FIX
// 	//}}
// 	
// 	switch( kPacket_.m_byteAuthorizeResult )
// 	{
// 	case 0: // Forbidden - ������ �������� ���� ������ �����Ѵ�.
// 	case 1: // Allowed - �����̾��� �����ϰ� ������ �����Ѵ�.
// 	case 2: // Trial - �����̾� ���� ������ �����Ѵ�.
// 	case 3: // Terminate - ���� ������ �����ϰ� ���������� �α׾ƿ� ��Ŷ�� �����Ѵ�.
// 		break;
// 
// 	default:
// 		START_LOG( cerr, L"�ؽ� ���� ���� �α��� ������� �̻��ϴ�." )
// 			<< BUILD_LOG( kPacket_.m_wstrUserID )
// 			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
// 			<< END_LOG;
// 		return;
// 	}
// 
// 	//{{ 2010. 07. 01  ������	PC�� ���� ���� ����
// 	if( kInfo.m_bChannelChanged )
// 	{
// 		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
// 		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_CHANNEL_CHANGE_USER_ACK, kPacket_ );
// 	}
// 	else
// 	{
// 		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
// 		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_USER_ACK, kPacket_ );
// 	}
// 	//}}

	// 2008.12.23.hoons.
	// �Ǿ��� ������ ���� �ӽ� �α�
	//if( kPacket_.m_wstrUserID.compare(L"nateawooda") == 0 || kPacket_.m_wstrUserID.compare(L"demiahun") == 0 )
	//{
	//	SOCKADDR_IN sAddr;
	//	sAddr.sin_addr.S_un.S_addr = kInfo.m_uiIP;

	//	START_LOG( cout2, L"�Ǿ��� �ӽ÷α�.!(Ȯ���ʿ�)" )
	//		<< BUILD_LOG( kPacket_.m_wstrUserID )
	//		<< BUILD_LOG( static_cast<int>(kPacket_.m_byteIsCharged) )
	//		<< BUILD_LOG( ::inet_ntoa(sAddr.sin_addr) )
	//		<< END_LOG;
	//}
}

IMPL_ON_FUNC( ENX_AUTH_TERMINATE_NOT )
{
	// ������ ����Ǿ �ش� ������ ���� �����Ű��!
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� �˻� ����." )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_bytePacketType )
			<< BUILD_LOG( kPacket_.m_byteTerminateType )
			<< END_LOG;
		return;
	}

	UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ENX_AUTH_TERMINATE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ENX_AUTH_MESSAGE_NOT )
{
    // �ش� �������� �޽��� ����!
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� �˻� ����." )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_bytePacketType )
			<< BUILD_LOG( kPacket_.m_byteMessageType )
			<< END_LOG;
		return;
	}

	UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ENX_AUTH_MESSAGE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ENX_AUTH_SYNCHRONIZE_REQ )
{
	START_LOG( cout, L"�ؽ� PC�� ���� ���� : ����ȭ üũ ��Ŷ ����!" )
		<< BUILD_LOG( kPacket_.m_byteIsMonitoring )
		<< BUILD_LOG( kPacket_.m_mapSynchronizeReq.size() );

	GetKLoginSimLayer()->CheckAuthSynchronize( kPacket_ );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( ENX_COMMON_ACK_NOT )
{
	KUserList::KGSUserInfo kInfo;

	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) )
	{
		unsigned short usEventID;
		switch( kPacket_.m_byteResult )
		{
		case 0:
		case 1:
		case 2:
			usEventID = ELG_REGISTER_NEXON_USER_ACK;
			break;
		case 3:
			usEventID = ENX_USER_TERMINATE_NOT;
			break;
		case 4:
			usEventID = ENX_MESSAGE_NOT;
			break;
		default:
			START_LOG( cerr, L"�ؽ� ���� ���� ���� ��Ŷ �̻�." )
				<< BUILD_LOG( kPacket_.m_wstrUserID )
				<< BUILD_LOG( kPacket_.m_bytePacketType )
				<< BUILD_LOG( kPacket_.m_byteResult )
				<< END_LOG;
			return;
		}

		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, usEventID, kPacket_ );

		// 2008.12.23.hoons.
		// �Ǿ��� ������ ���� �ӽ� �α�
		if( kPacket_.m_wstrUserID.compare(L"nateawooda") == 0 || kPacket_.m_wstrUserID.compare(L"demiahun") == 0 )
		{
			SOCKADDR_IN sAddr;
			sAddr.sin_addr.S_un.S_addr = kInfo.m_uiIP;

			START_LOG( cout2, L"�Ǿ��� �ӽ÷α�.!(Ȯ���ʿ�)" )
				<< BUILD_LOG( kPacket_.m_wstrUserID )
				<< BUILD_LOG( static_cast<int>(kPacket_.m_byteIsGameBangBillType) )
				<< BUILD_LOG( ::inet_ntoa(sAddr.sin_addr) )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"���� ���� ���� �˻� ����." )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_bytePacketType )
			<< BUILD_LOG( kPacket_.m_byteResult )
			<< END_LOG;
	}
}

#endif SERV_PCBANG_AUTH_NEW
//}}

//{{ 2009. 7. 6  ������		��ŷ ����
IMPL_ON_FUNC( DBE_GET_HENIR_RANKING_INFO_ACK )
{
	// ��ϸ��� �ð� ��ŷ ������Ʈ
	SiKRankingManager()->InitHenirRanking( kPacket_.m_mapHenirRanking );
}

_IMPL_ON_FUNC( ELG_CLEAR_RANKING_NOT, bool )
{
	// ��ŷ �ʱ�ȭ
	SiKRankingManager()->CheckRankingClearAndReward( true );

	START_LOG( cout, L"��ŷ ���� �ʱ�ȭ �� ����ó�� �Ϸ�!" );
}

IMPL_ON_FUNC_NOPARAM( DMP_DUMP_RANKING_MANAGER_NOT )
{
    SiKRankingManager()->DumpRankingInfo();
}

IMPL_ON_FUNC( ELG_ADMIN_NEW_RECORD_TEST_NOT )
{
	SiKRankingManager()->NewRecordForTest( kPacket_.m_vecNewRecordTest );
}

IMPL_ON_FUNC( ELG_ADMIN_NEW_RECORD_NOT )
{
	if( SiKRankingManager()->NewRecordHenirRanking( kPacket_.m_kNewRecordRanking, kPacket_.m_iRankingType ) )
	{
		START_LOG( cout, L"��ŷ ��� �߰� ����!" );
	}
	else
	{
		START_LOG( cout, L"��ŷ ��� �߰� ����!" );
	}
};

IMPL_ON_FUNC( ELG_ADMIN_DELETE_RECORD_NOT )
{	
	if( SiKRankingManager()->DeleteRecord( kPacket_.m_iRankingType, kPacket_.m_wstrNickName ) )
	{
        START_LOG( cout, L"��ŷ ��� ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_wstrNickName );
	}
	else
	{
		START_LOG( cout, L"��ŷ ��� ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_wstrNickName );
	}
}
//}}

IMPL_ON_FUNC( DBE_GET_WEB_RANKING_INFO_ACK )
{
	// ����, ���� ��ŷ ������Ʈ
	SiKRankingManager()->UpdateDungeonRanking( kPacket_.m_vecDungeonRanking );
	SiKRankingManager()->UpdatePvpRanking( kPacket_.m_vecPvpRanking );

	// ���Ӽ����� ��ε� ĳ����
	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR;
		SiKRankingManager()->GetDungeonRankingVector( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );        
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP;
		SiKRankingManager()->GetDungeonRankingMap( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR;
		SiKRankingManager()->GetPvpRankingVector( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP;
		SiKRankingManager()->GetPvpRankingMap( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}
}

//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_TEST

IMPL_ON_FUNC( DBE_GUILD_LEVEL_UP_ACK )
{
	// ��� ������ ������Ʈ ��� ó��
	if( SiKGuildManager()->UpdateGuildLevelResult( kPacket_.m_iGuildUID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"��� ������ ������Ʈ ��� ó�� ����" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOGc( kPacket_.m_ucGuildLevel )
			<< END_LOG;

		return;
	}

	//{{ 2010. 01. 25  ������	��� �Խ���
#ifdef SERV_GUILD_AD

	// ��� ���� �ִ��� ã�ƺ��� �ִٸ� ��������� ��������!
	SiKGuildBoardManager()->UpdateGuildLevelResult( kPacket_.m_iGuildUID, kPacket_.m_ucGuildLevel );

#endif SERV_GUILD_AD
	//}}

	// ��� ���� 5�϶�, 10�϶�, 15�϶� ��ü ���� ������
	switch( kPacket_.m_ucGuildLevel )
	{
	case 5:
	case 10:
	case 15:
		{
			KEGS_NOTIFY_MSG_NOT	kPacketNot;
			kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_GUILD_LEVEL_UP;
			kPacketNot.m_Count = 1;

			CStringW cwstrMessage;
			cwstrMessage.Format( L"%s;%d", kPacket_.m_wstrGuildName.c_str(), static_cast<int>(kPacket_.m_ucGuildLevel) );
			kPacketNot.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

			SendToGameServer( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, kPacketNot );
		}
		break;
	}
}

#endif GUILD_TEST
//}}

//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( DBE_INSERT_GUILD_CASH_SKILL_POINT_ACK )
{
	// ��� �Ŵ����� ��� ������Ʈ
	if( SiKGuildManager()->InsertGuildCashSkillPointResult( kPacket_ ) == false )
	{
		START_LOG( cerr, L"��� ĳ�� ��ų ����Ʈ ������Ʈ ��� ó�� ����" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK )
{
	// ��� �Ŵ����� ��� ������Ʈ
	if( SiKGuildManager()->ExpireGuildCashSkillPointResult( kPacket_ ) == false )
	{
		START_LOG( cerr, L"��� ĳ�� ��ų ����Ʈ �Ⱓ���� ��� ó�� ����" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_GUILD_SKILL_POINT_ACK, KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK )
{
	// ġƮ ��� ������Ʈ
    if( SiKGuildManager()->AdminInsertGuildSkillPointResult( kPacket_ ) == false )
	{
		START_LOG( cerr, L"��� ��ų ����Ʈ ġƮ ��� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

#endif GUILD_SKILL_TEST	
//}}

//{{ 2010. 01. 15  ������	���Խ���
#ifdef SERV_GUILD_AD

IMPL_ON_FUNC( DBE_GET_GUILD_BOARD_INFO_ACK )
{
	// DB�κ��� ���� ��� �Խ��� ���� ������Ʈ
	SiKGuildBoardManager()->UpdateGuildBoardInfo( kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCEPT_JOIN_GUILD_ACK )
{
	// ����� ������ �����
	if( SiKGuildManager()->RemoveReserve( kPacket_.m_iGuildUID, kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"��� ���� ���� ���� ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID )
			<< END_LOG;
	}

	// ��� ���� ������ �ƴϸ� ����ó��
	if( kPacket_.m_iOK != NetError::NET_OK )
		return;

	// ��� ���� ó��!
	if( SiKGuildManager()->JoinGuildMember( kPacket_.m_iGuildUID, kPacket_.m_kJoinGuildMemberInfo, false ) == false )
	{
		START_LOG( cerr, L"��� ��� �α��� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID )
			<< END_LOG;
	}

	// ��� ���� ���
	KEGS_GUILD_INFO_NOT kNot;
	SiKGuildManager()->GetGuildInfo( kPacket_.m_iGuildUID, kNot );

	// ���� ����� ��� ���� �ִ��� Ȯ��
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		// ���� �ο� ���� ������Ʈ
		SiKGuildBoardManager()->UpdateGuildCurMemberCount( kPacket_.m_iGuildUID, static_cast<u_short>(kNot.m_vecMemberUnitUID.size()) );
	}

	// �α׿����� ������ ��� �����ϴ°��̹Ƿ� ������ ���� ����� �˷�����!	
	kPacket_.m_wstrGuildName = kNot.m_kGuildInfo.m_wstrGuildName;
	SendToGameServer( ELG_SEND_LETTER_JOIN_GUILD_NOT, kPacket_ );
}

#endif SERV_GUILD_AD
//}}

//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT	

IMPL_ON_FUNC( DBE_GET_WEB_POINT_REWARD_ACK )
{
	if( kPacket_.m_vecWebPointReward.empty() )
		return;

	SendToGameDB( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_ACK, KDBE_GET_WEB_POINT_REWARD_ACK )
{
	if( kPacket_.m_vecWebPointReward.empty() )
		return;

	// ���Ӽ����� ����ó�� �Ϸ�����
	UidType iGameServerUID = KBaseServer::GetKObj()->GetFirstActorKey();
	if( iGameServerUID <= 0 )
	{
		START_LOG( clog, L"�� ����Ʈ ������ ���Ӽ����� �����Ϸ��µ� ActorManager�� ����� ���Ӽ����� �ϳ��� ����.." )
			<< BUILD_LOG( iGameServerUID )
			<< END_LOG;

		return;
	}

	// ���Ӽ����� ��ŷ ���� ������
	UidType anTrace[2] = { iGameServerUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, kPacket_ );	

	// �ؽ� ��DB�� ��� ������Ʈ
	SendToNXWebDB( DBE_RESULT_WEB_POINT_REWARD_NOT, kPacket_ );
}

#endif SERV_WEB_POINT_EVENT
//}}

//{{ 2010. 06. 22  ������	���������
#ifdef SERV_ACCOUNT_BLOCK

IMPL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK )
{
	std::map< int, std::vector< UidType > > mapBlockUserList;
	std::map< int, std::vector< UidType > > mapUnblockUserList;
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	std::vector< std::string > vecBlockMachineIDList;
	SiKAccountBlockManager()->UpdateAccountBlockList( kPacket_, mapBlockUserList, mapUnblockUserList, vecBlockMachineIDList );
#else
	SiKAccountBlockManager()->UpdateAccountBlockList( kPacket_, mapBlockUserList, mapUnblockUserList );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}    

	// ���� �߰��� �������� ������ ���� ���θ� Ȯ���Ͽ� ���������Ų��!
	{
		std::map< int, std::vector< UidType > >::const_iterator mit;
		for( mit = mapBlockUserList.begin(); mit != mapBlockUserList.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const std::vector< UidType >& vecBlockList = mit->second;

			if( iBlockType < 0  ||  iBlockType >= KAccountBlockManager::BT_MAX )
				continue;

			switch( iBlockType )
			{
			case KAccountBlockManager::BT_CONNECT_BLOCK:
				{
					BOOST_TEST_FOREACH( const UidType, iUserUID, vecBlockList )
					{
						KUserList::KGSUserInfo kInfo;
						if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
							continue;

						int iOK = NetError::ERR_ADMIN_COMMAND_07;
						SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, E_KICK_USER_NOT, iOK );

						START_LOG( cout, L"���� ���� ������� �ǽð����� KICK�մϴ�!" )
							<< BUILD_LOG( kInfo.m_nGSUID )
							<< BUILD_LOG( kInfo.m_nUserUID )
							<< BUILD_LOG( iUserUID );
					}
				}
				break;

			case KAccountBlockManager::BT_TRADE_BLOCK:
				{
					BOOST_TEST_FOREACH( const UidType, iUserUID, vecBlockList )
					{
						KUserList::KGSUserInfo kInfo;
						if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
							continue;

						KELG_TRADE_BLOCK_NOT kNot;
						kNot.m_bIsTradeBlock = true;
						SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_TRADE_BLOCK_NOT, kNot );

						START_LOG( cout, L"�ŷ� ���� ������� �ǽð����� �����մϴ�!" )
							<< BUILD_LOG( kInfo.m_nGSUID )
							<< BUILD_LOG( kInfo.m_nUserUID )
							<< BUILD_LOG( iUserUID );
					}
				}
				break;

			default:
				break;
			}
		}
	}	

	// ������ �������� ������ ���� ���θ� Ȯ���Ͽ� �ǽð� ���� ��Ų��!
	{
		std::map< int, std::vector< UidType > >::const_iterator mit;
		for( mit = mapUnblockUserList.begin(); mit != mapUnblockUserList.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const std::vector< UidType >& vecBlockList = mit->second;

			if( iBlockType < 0  ||  iBlockType >= KAccountBlockManager::BT_MAX )
				continue;

			switch( iBlockType )
			{
			case KAccountBlockManager::BT_CONNECT_BLOCK:
				break;

			case KAccountBlockManager::BT_TRADE_BLOCK:
				{
					BOOST_TEST_FOREACH( const UidType, iUserUID, vecBlockList )
					{
						KUserList::KGSUserInfo kInfo;
						if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
							continue;

						KELG_TRADE_BLOCK_NOT kNot;
						kNot.m_bIsTradeBlock = false;
						SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_TRADE_BLOCK_NOT, kNot );

						START_LOG( cout, L"�ŷ� ���� ������� �ǽð����� �����մϴ�!" )
							<< BUILD_LOG( kInfo.m_nGSUID )
							<< BUILD_LOG( kInfo.m_nUserUID )
							<< BUILD_LOG( iUserUID );
					}
				}
				break;

			default:
				break;
			}
		}
	}	

	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	{
		// �������� ���� ���� ���� ��Ű��!
		BOOST_TEST_FOREACH( const std::string&, strMachineID, vecBlockMachineIDList )
		{
			KUserList::KGSUserInfo kInfo;
			if( GetKLoginSimLayer()->GetGSUserInfoByMachineID( strMachineID, kInfo ) == false )
				continue;

			int iOK = NetError::ERR_ADMIN_COMMAND_07;
			SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, E_KICK_USER_NOT, iOK );

			START_LOG( cout, L"�ӽ�ID ���� ������� �ǽð����� �����մϴ�!" )
				<< BUILD_LOG( kInfo.m_nGSUID )
				<< BUILD_LOG( kInfo.m_nUserUID )
				<< BUILD_LOG( strMachineID );
		}
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
}

#endif SERV_ACCOUNT_BLOCK
//}}

//{{ 2010. 07. 19  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// �α��μ����� ���� ������� Ȯ��!
	if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SENTINEL < kPacket_.m_iOrderType  && 
		KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SENTINEL > kPacket_.m_iOrderType )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		SiKLoginScriptRefreshManager()->QueueingEvent( spEvent );
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
	switch( kPacket_.m_iOrderType )
	{
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_GUILD_MANAGER, CXSLGuildManager );
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_MORNITORING_MANAGER, KMornitoringManager );
#endif SERV_MORNITORING
	//}}
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_GAME_SYS_VAL, KGameSysVal );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_TITLE_MANAGER, CXSLTitleManager );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}

	//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_SKILL_TREE, CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"LoginServer ��ũ��Ʈ �ǽð� ��ġ �Ϸ�"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
IMPL_ON_FUNC( DBE_GET_MODULE_INFO_LIST_ACK )
{
	//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
	SiKDLLManager()->Init( kPacket_ );
#else
	SiKDLLManager()->Init( kPacket_.m_vecModuleList );
#endif SERV_HACKING_DLL_CHECK
	//}}

	START_LOG( cout, L"DLL ��� ����Ʈ ������Ʈ �Ϸ�!" )
		<< BUILD_LOG( kPacket_.m_vecModuleList.size() );
}

_IMPL_ON_FUNC( DBE_UPDATE_MODULE_INFO_LIST_ACK, KDBE_GET_MODULE_INFO_LIST_ACK )
{
	SiKDLLManager()->UpdateModuleList( kPacket_.m_vecModuleList );
}
#endif SERV_DLL_MANAGER
//}}

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT )
{
	SiKMornitoringManager()->ReserveSMSMessage( kPacket_ );
}
#endif SERV_MORNITORING
//}}

//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
IMPL_ON_FUNC( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK )
{
    SiKRandomItemEventManager()->UpdateEventInfo( kPacket_.m_vecTimeOpenRandomItemInfo, kPacket_.m_setGetRewardUserUIDList );
}
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}

//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_ACK )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRewardLetter.m_iToUnitUID, kUserInfo ) == false )
		return;

	KELG_FIND_USER_SEND_LETTER_NOT kPacket;
	kPacket.m_kLetter = kPacket_.m_iRewardLetter;
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_FIND_USER_SEND_LETTER_NOT, kPacket );
}

IMPL_ON_FUNC( DBE_LOAD_PSHOP_AGENCY_ACK )
{
	if( kPacket_.m_vecOpenPShopAgency.empty() )
	{
		START_LOG( cout, L"������ �븮 ������ �����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_vecOpenPShopAgency.size() );
		return;
	}

	int iOpenPShopAgencyCount = 0;

	BOOST_TEST_FOREACH( const KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT&, kPacket, kPacket_.m_vecOpenPShopAgency )
	{
		if( kPacket.m_vecSellItemInfo.empty() )
		{
			START_LOG( clog, L"�Ǹ� �������� ����ֽ��ϴ�! �ڵ����� ���������� ���� �ʽ��ϴ�!" )
				<< BUILD_LOG( kPacket.m_iUnitUID );
			continue;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_SERVER, NULL, ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, kPacket );
		//LIF( spEvent->m_kDestPerformer.AddUID( mit->first ) ); -- RoomUID�� ���� �ʿ� ����!
		GetKLoginRoomManager()->QueueingEventToRoom( spEvent );

		++iOpenPShopAgencyCount;
	}

	START_LOG( cout, L"DB�� ����� �븮���� �ڵ� ���� �Ϸ�!" )
		<< BUILD_LOG( iOpenPShopAgencyCount );
}

_IMPL_ON_FUNC( DBE_BREAK_PSHOP_AGENCY_ACK, KERM_BREAK_PSHOP_AGENCY_NOT )
{
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUID, kInfo ) == false )
		return;
	
	SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ERM_BREAK_PSHOP_AGENCY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_PSHOP_AGENCY_MESSAGE_NOT, KEGS_PSHOP_AGENCY_MESSAGE_NOT )
{
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUnitUID, kInfo ) == false )	
		return;

	SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_PSHOP_AGENCY_MESSAGE_NOT, kPacket_ );
}
#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
IMPL_ON_FUNC( ELG_RANKING_TITLE_REWARD_NOT )
{
	// Ư�� ������ ������Ʈ �ϵ��� �Ұ�!
	START_LOG( cout, L"��ŷ �Ŵ����� ��û���� ��ŷ Īȣ ���� ������ �����մϴ�!" )
		<< BUILD_LOG( kPacket_.m_mapTitleReward.size() );
	
	// GameDB�� Ÿ��Ʋ�� insert���ش�.
	std::map< UidType, KRankingTitleReward >::const_iterator mit;
	for( mit = kPacket_.m_mapTitleReward.begin(); mit != kPacket_.m_mapTitleReward.end(); ++mit )
	{
		const KRankingTitleReward& kTitleReward = mit->second;
		std::wstring wstrMessage;
	
		BOOST_TEST_FOREACH( const KTitleReward&, kRewardInfo, kTitleReward.m_vecReward )
		{
			// Ÿ��Ʋ ���� �ֱ�
			KDBE_INSERT_TITLE_REQ kPacket;
			kPacket.m_iUnitUID		   = kTitleReward.m_iUnitUID;
			kPacket.m_iTitleID		   = kRewardInfo.m_iTitleID;
			kPacket.m_sPeriod		   = kRewardInfo.m_sPeriod;
			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacket );
	
			const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( kRewardInfo.m_iTitleID );
			if( pTitleInfo == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� Īȣ�Դϴ�!" )
					<< BUILD_LOG( kRewardInfo.m_iTitleID )
					<< END_LOG;
	
				continue;
			}
			//{{ 2011.11.20 ��ȫ��	���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
			wstrMessage += boost::str( boost::wformat( L"%d" ) % pTitleInfo->m_iTitleID );
			//wstrMessage += L",\n\n";					// EU Mail ���� ���� ����
#else SERV_UNITED_SERVER_EU
			wstrMessage += pTitleInfo->m_wstrTitleName;
			wstrMessage += L",\n\n";
#endif SERV_UNITED_SERVER_EU
			//}}
		}
	
		// Ÿ��Ʋ ���� ���� ������ �˸���
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = kTitleReward.m_iUnitUID;
		kPacketToDB.m_iToUnitUID   = kTitleReward.m_iUnitUID;
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_RANKING_TITLE;
		kPacketToDB.m_iRewardID	   = 0; // �������
		kPacketToDB.m_wstrMessage  = wstrMessage;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
}

IMPL_ON_FUNC( DBE_INSERT_TITLE_ACK )
{
	KUserList::KGSUserInfo kUserInfo;
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kUserInfo ) )
		return;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INSERT_TITLE_NOT, kPacket_ );
}
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
IMPL_ON_FUNC( DBE_GET_HACKING_MODULE_LIST_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK && kPacket_.m_bRequest == true )
	{
		SiKDLLManager()->InitHackingModuleList( kPacket_.m_vecModuleList, kPacket_.m_iReleaseTick );

		START_LOG( cout, L"Module List�� �ٽ� �޾ҽ��ϴ�." )
			<< END_LOG;
	}
}
#endif SERV_HACKING_DLL_CHECK
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-24
IMPL_ON_FUNC( DBE_LOCAL_RANKING_RESET_ACK )
{
	KEventPtr	spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_SERVER, NULL, DBE_LOCAL_RANKING_RESET_ACK, kPacket_ );
	SiKLoginLocalRankingManager()->QueueingEvent( spEvent );
	SiKGuildManager()->ResetLocalRankingInfo();
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( DBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB�� �ִ� ��ȥ�� ������ �޾ƿ��� ����!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
		return;
	}

	SiKWeddingHallManager()->SetWeddingInfoDB( kPacket_.m_mapWeddingInfo );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		BroadCastAllGS( ELG_WEDDING_HALL_INFO_UPDATE_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ELG_ADD_WEDDING_HALL_INFO_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	SiKWeddingHallManager()->SetWeddingInfoDB( kPacket_.m_mapWeddingInfo );
	BroadCastAllGS( ELG_WEDDING_HALL_INFO_UPDATE_NOT, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS// �۾���¥: 2013-10-28	// �ڼ���
IMPL_ON_FUNC( E_DISCONNECT_SERVER_REPORT_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH );
		}
		break;

	default:
		START_LOG( cerr, L"�߸��� Ÿ���Դϴ�!" )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< END_LOG;
		break;
	}
}
#endif // SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS

#ifdef SERV_GLOBAL_AUTH 
IMPL_ON_FUNC( EPUBLISHER_REG_USER_ACK )
{
	// �������� ���� RegUserAck ó�� �ϴ°� ��� �Լ��� ���� �������� ���� //
	KUserList::KGSUserInfo kInfo;
	
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrServiceAccountID, kInfo ) )
	{
		//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
		GetKLoginSimLayer()->UnRegAuthWaitUser( kInfo.m_nUserUID );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
		//}}
		KELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrServiceAccountID = kPacket_.m_wstrServiceAccountID;
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		kPacketAck.m_bIsPcBang = kPacket_.m_bIsPcBang;
		kPacketAck.m_iPCBangType = kPacket_.m_iPCBangType;
		kPacketAck.m_iCheckState = kPacket_.m_iCheckState;

		START_LOG( clog, L"PC�� ���� üũ." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOGc( kPacket_.m_bIsPcBang )
			<< BUILD_LOG( kPacket_.m_iPCBangType )
			<< BUILD_LOG( kPacket_.m_iCheckState )
			<< END_LOG;
#else //SERV_CHECK_PCBANG_BY_PUBLISHER
		START_LOG( clog, L"PC�� ���� üũ." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< END_LOG;
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_COUNTRY_TH
		kPacketAck.m_wstrSockID = kPacket_.m_wstrSockID;
#endif //SERV_COUNTRY_TH

		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK, kPacketAck );
	}
	else
	{
		START_LOG( cerr, L"���� ���� ���� �˻� ����." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EPUBLISHER_UNREG_USER_ACK )
{
	// �������� ���� UnRegUserAck ó�� �ϴ°� ��� �Լ��� ���� �������� ���� //
}

#ifdef SERV_COUNTRY_CN
IMPL_ON_FUNC( EGIANT_INFO_GET_CCU_REQ )
{
	typedef std::map< UidType, KUpdateCCUInfo > CCUInfoMap;

	KEGIANT_INFO_GET_TOTAL_CCU_ACK kTotalCCUAck;
	kTotalCCUAck.m_TimeStamp = kPacket_.m_TimeStamp;
	kTotalCCUAck.m_usServerID = static_cast<unsigned short>(GetUID());
	kTotalCCUAck.m_usServerType = GetServerClass();
	kTotalCCUAck.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kTotalCCUAck.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	kTotalCCUAck.m_strZoneName = SiKGiantInfoManager()->GetServerInfo().m_strZoneName;
	kTotalCCUAck.m_uiCCU = 0;
	BOOST_FOREACH( const CCUInfoMap::value_type& value, SiKServerCCUManager()->GetLastCCUInfo() )
	{
		kTotalCCUAck.m_uiCCU += value.second.m_iConcurrentUser;
	}

	KEventPtr spTotalEvent( new KEvent );
	spTotalEvent->SetData( PI_GS_SERVER, NULL, EGIANT_INFO_GET_TOTAL_CCU_ACK, kTotalCCUAck );
	SiKGiantInfoManager()->QueueingEvent( spTotalEvent );


	KEGIANT_INFO_GET_CHANNEL_CCU_ACK kChannelCCUAck;
	kChannelCCUAck.m_TimeStamp = kPacket_.m_TimeStamp;
	kChannelCCUAck.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kChannelCCUAck.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	BOOST_FOREACH( const CCUInfoMap::value_type& value, SiKServerCCUManager()->GetLastCCUInfo() )
	{
		kChannelCCUAck.m_mapChannelCCU.insert(std::make_pair(value.second.m_iServerUID, value.second.m_iConcurrentUser));
	}

	KEventPtr spChannelEvent( new KEvent );
	spChannelEvent->SetData( PI_GS_SERVER, NULL, EGIANT_INFO_GET_CHANNEL_CCU_ACK, kChannelCCUAck );
	SiKGiantInfoManager()->QueueingEvent( spChannelEvent );
}
#endif SERV_COUNTRY_CN

#endif // SERV_GLOBAL_AUTH