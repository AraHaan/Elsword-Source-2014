#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>
#include "ChannelServer.h"
#include "ActorManager.h"
#include "ChannelSimLayer.h"
#include "ChannelNetLayer.h"
#include "socket/NetCommon.h"
#include "DBLayer.h"
#include "ChannelAccountDBThread.h"
#include "ChannelLogDBThread.h"
//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "CHFSM.h"
#else
	#include "defaultFSM.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
#include "ChannelUser.h"

#include "NetError.h"
//{{ 2009. 12. 21  ������	������ Ȯ��
#include "Enum/Enum.h"
//}}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	#include "HackingProtectionManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#include "ProxyManager.h"
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/HanAuthForSvr.h"
#endif //SERV_COUNTRY_JP

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
#include "..\Common\OnlyGlobal\AuthAndBilling\IN\ChannelFunizenAuthDBThread.h"
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2011. 03. 29	������	TBB �޸� ������
//#ifdef SERV_TBB_MALLOC_PROXY_TEST
//	#include <tbb/tbbmalloc_proxy.h>
//#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "MachineBlockManager.h"
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	#include "ChannelSMSDBThread.h"
#endif SERV_CHECK_DROP_CCU
//}}
//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	#include "UdpRelayChecker/UdpRelayChecker.h"
#endif SERV_UDP_RELAY_CHECKER
//}}
//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	#include "ChannelScriptRefreshManager.h"
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}
//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

//#include "vld.h"

NiImplementRTTI( KChannelServer, KBaseServer );
ImplPfID( KChannelServer, PI_CHANNEL_SERVER );

#define CLASS_TYPE KChannelServer

ImplToStringW( KChannelServer )
{
	START_TOSTRING_PARENTW( KBaseServer );

	stm_ << L"Channel List Size : " << m_mapChannelList.size() << std::endl;

    return stm_;
}

KChannelServer::KChannelServer(void) :
m_bIsFirstUpdate( false )
#ifdef SERVER_GROUP_UI_ADVANCED
,m_bIsFirstUpdateServerGroup( false )
#endif SERVER_GROUP_UI_ADVANCED
{
}

KChannelServer::~KChannelServer(void)
{
}

#include <lua_tinker.h>

void KChannelServer::RegToLua()
{
    KBaseServer::RegToLua();

	lua_tinker::class_add<KChannelServer>( g_pLua, "KChannelServer" );
	lua_tinker::class_inh<KChannelServer, KBaseServer>( g_pLua );
	lua_tinker::class_def<KChannelServer>( g_pLua, "dump", &KChannelServer::Dump );

	lua_tinker::decl( g_pLua, "Server", this );
}

const wchar_t*  g_szConfigFile = L"config_ch";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KChannelServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KChannelAccountDBThread( szDSN, bDBConnStr );
    case KDBLayer::DC_LOG:
        return new KChannelLogDBThread( szDSN, bDBConnStr );
		//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	case KDBLayer::DC_SMS:
		return new KChannelSMSDBThread( szDSN, bDBConnStr );
#endif SERV_CHECK_DROP_CCU
		//}}
#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
	case KDBLayer::DC_PUBLISHER_AUTH:
		return new KChannelFunizenAuthDBThread( szDSN, bDBConnStr );
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH
    default:
        START_LOG( cerr, L"�����Ϸ��� DB ������ �̻���." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KChannelLogDBThread( szDSN, bDBConnStr );
    }
}

bool KChannelServer::Init()
{
    KSimLayer::KeepObject<KChannelSimLayer>();
    KNetLayer::KeepObject<KChannelNetLayer>();
    KActorManager::KeepObject<KActorManager>();
	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::KeepObject<KUdpRelayChecker>();
#endif SERV_UDP_RELAY_CHECKER
	//}}

	//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KChannelUser, KCHFSM, 32*1024> ) );
#else
	KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KChannelUser, KDefaultFSM, 32*1024> ) );
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}    
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_CLIENT );
#endif SERV_KEVENT_FROM
	//}}

#ifdef SERV_LOGIN_RESULT_INFO
	InitLoginResultInfo();
#endif SERV_LOGIN_RESULT_INFO

    _JIF( KBaseServer::Init(), return false );

    //WriteServerInfoToDB();

#ifdef SERV_COUNTRY_JP
	int ret = -1;
	if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_ALPHA )
	{
		ret = HanAuthInit("J_ES_PRE");

		if(ret == 0)
		{
			START_LOG( clog, L"���Ĺ��� ���� �ʱ�ȭ �Ϸ�" )				
				<< END_LOG;
		}

	}
	else if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_REAL )
	{
		ret = HanAuthInit("J_ES");

		if(ret == 0)
		{
			START_LOG( clog, L"������� ���� �ʱ�ȭ �Ϸ�" )				
				<< END_LOG;
		}
	}

	//{{ 2009. 11. 27  ������ �Ϻ�����
	if(ret != 0)
	{
		START_LOG( cerr, L"�����ʱ�ȭ ����" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}
#endif //SERV_COUNTRY_JP

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	SiKProxyManager()->ConnectAll();
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	SetServerReady( true );

	//{{ 2009. 8. 27  ������	server ready
	OnServerReadyComplete();
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    return true;
}

void KChannelServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

#ifdef SERVER_GROUP_UI_ADVANCED
	RefreshServerGroupList();
#endif SERVER_GROUP_UI_ADVANCED

	//{{ 2009. 8. 13  ������	ä�θ���Ʈ
	RefreshChannelList();
	//}}

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager()->Init();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	SiKChannelScriptRefreshManager()->Init();
	SiKChannelScriptRefreshManager()->RegToLua( g_pLua );
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	//}}

	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	SiKMachineBlockManager()->Init();
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}

	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->Init();
#endif SERV_UDP_RELAY_CHECKER
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\ChannelServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\ChannelServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	// ������ �̸� ǥ��
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );
}

void KChannelServer::ShutDown()
{
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    // ���� �ƹ� ����ó���� ����Ǳ� ��.
    KBaseServer::ShutDown();        // �̰����� ��� ���ᰡ ������. ������ Layer�鵵 ��� �Ҹ��.

	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->ShutDown();
	KUdpRelayChecker::ReleaseKObj();
#endif SERV_UDP_RELAY_CHECKER
	//}}
}

bool KChannelServer::DestroyAllSession()
{
    // NetLayer�� Accepter�� ���� ����. ���� ��� Layer���� ����ִ�.
 
    KActorManager* pkActorManager = KActorManager::GetKObj();
   
    START_LOG( cout, L"���� ���� : " << pkActorManager->GetCount() << L" ��" );

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

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	// proxy manager�� ���� ó���� �ϴ� ���� ��ü�� �Ҹ�ó���� ���� ������ ���� �����Ѵ�.
	bool bProxyShutdown = SiKProxyManager()->Shutdown( 60.0f );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

    LOG_CONDITION( bSuccess, cout, cerr )
        << BUILD_LOG( m_dwShutDownWait )
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		<< BUILD_LOG( bProxyShutdown )
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
        << BUILD_LOG( pkActorManager->GetCount() )
        << BUILD_LOG( SiKDBLayer()->GetTotalEventNum() )
        << BUILD_LOG( ::GetTickCount() - dwBeginTick );

    return bSuccess;
}

void KChannelServer::Tick()
{
    KBaseServer::Tick();

#ifdef SERVER_GROUP_UI_ADVANCED
	if( m_kServerGroupListRefreshTimer.elapsed() > 60.f * 60 )
	{
		RefreshServerGroupList();

		m_kServerGroupListRefreshTimer.restart();
	}
#endif SERVER_GROUP_UI_ADVANCED

	//{{ 2009. 1. 16  ������	ä�� ����Ʈ ������Ʈ
	if( m_kChannelListRefreshTimer.elapsed() > 60.f )
	{
		RefreshChannelList();

		m_kChannelListRefreshTimer.restart();
	}
	//}}

	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	SiKMachineBlockManager()->Tick();
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}

#ifdef SERV_LOGIN_RESULT_INFO
	if( m_kLoginResultInfoTimer.elapsed() > 60.f )
	{
		WriteLoginResultInfoToDB();
		m_kLoginResultInfoTimer.restart();

		InitLoginResultInfo();
	}
#endif SERV_LOGIN_RESULT_INFO

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\ChannelServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\ChannelServerWrite"));
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

#ifdef SERVER_GROUP_UI_ADVANCED
void KChannelServer::RefreshServerGroupList()
{
	SendToLogDB( DBE_SERVERGROUP_LIST_REQ );
}

void KChannelServer::UpdateServerGroupList( const std::map< int, KServerGroupInfo >& mapServerGroupList )
{
	m_mapServerGroupList.clear();
	m_mapServerGroupList = mapServerGroupList;

	// ó�� ������Ʈ �ѰŶ�� ����Ʈ�� �α׷� ����Ѵ�!
	if( !m_bIsFirstUpdateServerGroup )
	{
		std::map< int, KServerGroupInfo >::iterator mit;
		for( mit = m_mapServerGroupList.begin(); mit != m_mapServerGroupList.end(); ++mit )
		{
			START_LOG( cout, L"������ :" )
				<< BUILD_LOG( mit->second.m_iOrder )
				<< BUILD_LOG( mit->second.m_iServerGroupUID )
				<< BUILD_LOG( mit->second.m_wstrServerGroupName )
				<< BUILD_LOG( mit->second.m_kServerIP )
				<< BUILD_LOG( mit->second.m_usMasterPort )
				<< BUILD_LOG( mit->second.m_usNCUDPPort )
				<< BUILD_LOG( mit->second.m_bEnable );
		}

		m_bIsFirstUpdateServerGroup = true;
	}
}
#endif SERVER_GROUP_UI_ADVANCED

void KChannelServer::RefreshChannelList()
{
	KDBE_CHANNEL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iServerGroupID = GetServerGroupID();
	SendToLogDB( DBE_CHANNEL_LIST_REQ, kPacketToDB );
}

void KChannelServer::UpdateChannelList( const std::map< int, KChannelInfo >& mapChannelList )
{
	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	if( SiKGameSysVal()->GetCheckDropCCU() == true )
	{
		std::map< int, KChannelInfo >			mapBeforChannelList = m_mapChannelList;
		
		std::map< int, KChannelInfo >::const_iterator mitNew = mapChannelList.begin();
		std::map< int, KChannelInfo >::iterator mitOld;

        for(  ; mitNew != mapChannelList.end() ; ++mitNew )
		{
			mitOld = mapBeforChannelList.find( mitNew->first );
			if( mitOld != mapBeforChannelList.end() )
			{
				if( mitOld->second.m_iCurrentUser < 500 )
				{
					continue;
				}

				int iGap = mitOld->second.m_iCurrentUser - mitNew->second.m_iCurrentUser;
				if( iGap > 0 )  // ������ �϶��Ͽ���
				{
					// ������ ���� �϶� ��ġ ���� ���ٸ�
					float fPercent = SiKGameSysVal()->GetDropCCUPercent();
					if( iGap > static_cast<int>(mitOld->second.m_iCurrentUser * fPercent) )
					{
						// sms �߼�
						KDBE_SEND_PHONE_MSG_NOT kPacketNot;
						//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
						SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_CHECK_DROP_CCU, kPacketNot.m_vecPhoneNum );
#else
						//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
						SiKGameSysVal()->GetRecipientSMS( KGameSysVal::SMS_SURVEILLANCE_THE_CCU_DROP, kPacketNot.m_vecPhoneNum );
#else
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4118-3867" ) );		// ��âȣ �����
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-7466-7958" ) );		// �����
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );		// ������
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-5598-4163" ) );		// ��μ�
						//kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-9490-8761" ) );		// Ȳ����
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8982-3382" ) );		// ��â��
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-2906-5792" ) );		// ������
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8771-2480" ) );		// ������

						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4072-1031" ) ); // ���
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-3898-7886" ) ); // �谭�� 
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-5671-0502" ) ); // �̿���
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
						//}}
#endif SERV_SMS_TOTAL_MANAGER
						//}}
						
						
						kPacketNot.m_wstrSMSMessage = L"!���� �޶� �߰�! IP : ";
						kPacketNot.m_wstrSMSMessage += mitNew->second.m_wstrIP;
						kPacketNot.m_wstrSMSMessage += L" ";
						kPacketNot.m_wstrSMSMessage += mitNew->second.m_wstrChannelName;
						kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L" %d " ) % mitOld->second.m_iCurrentUser );
						kPacketNot.m_wstrSMSMessage += L"->";
						kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L" %d" ) % mitNew->second.m_iCurrentUser );
						kPacketNot.m_wstrSMSMessage += L"�� �϶�!";
						SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
					}
				}
			}
		}
	}
#endif SERV_CHECK_DROP_CCU
	//}}

	m_mapChannelList.clear();
	m_mapChannelList = mapChannelList;

	std::map< int, KChannelInfo >::iterator mit;
	for( mit = m_mapChannelList.begin(); mit != m_mapChannelList.end(); ++mit )
	{
		mit->second.m_iCurPartyCount = 0;
		mit->second.m_iPlayGamePartyCount = 0;
	}

	// ó�� ������Ʈ �ѰŶ�� ����Ʈ�� �α׷� ����Ѵ�!
	if( !m_bIsFirstUpdate )
	{
		std::map< int, KChannelInfo >::iterator mit;
		for( mit = m_mapChannelList.begin(); mit != m_mapChannelList.end(); ++mit )
		{
			START_LOG( cout, L"ä�� �̸� : " << mit->second.m_wstrChannelName << L",  IP : " << mit->second.m_wstrIP << L",  Port : " << mit->second.m_usMasterPort );
		}

		m_bIsFirstUpdate = true;
	}
}

//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
void KChannelServer::UpdateChannelBonusList( const std::map< int, KChannelBonusInfo >& mapChannelBonusList )
{
	m_mapChannelBonusList.clear();

	m_mapChannelBonusList = mapChannelBonusList;

}
#endif SERV_CHANNEL_LIST_RENEWAL
//}}

void KChannelServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_ACK, KDBE_CHANNEL_SERVER_ON_ACK );
       _CASE( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
		CASE( DBE_CHANNEL_LIST_ACK );

		//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
		CASE( DBE_CHECK_HACKING_TOOL_LIST_ACK );
#endif SERV_HACKING_TOOL_LIST
		//}}
		//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
		CASE( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK );
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
		//}}
		//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
		CASE( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK );
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

		//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
		_CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
		//}}
#ifdef SERVER_GROUP_UI_ADVANCED
		CASE( DBE_SERVERGROUP_LIST_ACK );
#endif SERVER_GROUP_UI_ADVANCED
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

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_CHANNEL_SERVER_ON_ACK )
{
	//��ŸƮ�� ���������� ���α׷��� ����
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		::exit( 0 );
		return;
	}

	SetServerInfo( kPacket_.m_kServerInfo );

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	std::map< int, KNetAddress >::iterator mit;
	for( mit = kPacket_.m_mapLoginList.begin(); mit != kPacket_.m_mapLoginList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_LOGIN, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

    SetEvent( m_spEventAck.get() );
}

_IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList )
{
    SetServerList( kPacket_ );
}

IMPL_ON_FUNC( DBE_CHANNEL_LIST_ACK )
{
    UpdateChannelList( kPacket_.m_mapChannelList );

	//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	UpdateChannelBonusList(kPacket_.m_mapChannelBonusList);
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST

IMPL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_ACK )
{
	SiKHackingProtectionManager()->UpdateHackingToolList( kPacket_ );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
IMPL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK )
{
	std::vector< std::string > vecBlockMachineIDList;
	SiKMachineBlockManager()->UpdateMachineBlockList( kPacket_, vecBlockMachineIDList );

	START_LOG( cout2, L"MachinBlock ID ��� ���� ����!" );
}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
IMPL_ON_FUNC( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK )
{
	SiKWaitQueueManager()->SetTotalCCU( kPacket_.m_iTotalCCU );
}
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CH_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CH_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
		//}}

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}

#ifdef SERVER_GROUP_UI_ADVANCED
IMPL_ON_FUNC( DBE_SERVERGROUP_LIST_ACK )
{
	UpdateServerGroupList( kPacket_.m_mapServerGroupList );
}
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_LOGIN_RESULT_INFO
void KChannelServer::InitLoginResultInfo()
{
	m_kLoginResultInfo.m_vecResultCount.clear();

	for(int i = 0; i < KLoginResultInfo::LRTE_SENTINEL; i++)
	{
		m_kLoginResultInfo.m_vecResultCount.push_back(0);
	}
}

void KChannelServer::WriteLoginResultInfoToDB()
{
	// InitLoginResultInfo()�� ȣ��� ���� ServerUID ���� �������� ���̶� ���⼭ ó����.
	m_kLoginResultInfo.m_iServerGroupID = GetServerGroupID();
	m_kLoginResultInfo.m_uiServerUID = GetUID();

	KDBE_INSERT_LOGIN_RESULT_INFO_REQ kPacket;
	kPacket.m_iServerGroupID = m_kLoginResultInfo.m_iServerGroupID;
	kPacket.m_uiServerUID = m_kLoginResultInfo.m_uiServerUID;
	kPacket.m_vecResultCount = m_kLoginResultInfo.m_vecResultCount;

	SendToLogDB( DBE_INSERT_LOGIN_RESULT_INFO_REQ, kPacket );
}

void KChannelServer::PlusLoginResultCount(KLoginResultInfo::LOGIN_RESULT_TYPE_ENUM eResultType)
{
	if( eResultType < KLoginResultInfo::LRTE_SUCCESS || KLoginResultInfo::LRTE_SENTINEL < eResultType )
	{
		START_LOG( cerr, L"��ȿ�������� �α��� ��� �����Դϴ�!" )
			<< BUILD_LOG( eResultType )
			<< END_LOG;
		return ;
	}

	m_kLoginResultInfo.m_vecResultCount[eResultType]++;

	START_LOG( clog, L"�α��� ��� ���� ī��Ʈ ����!!" )
		<< BUILD_LOG( eResultType )
		<< BUILD_LOG( m_kLoginResultInfo.m_vecResultCount[eResultType] )
		<< END_LOG;
}
#endif SERV_LOGIN_RESULT_INFO