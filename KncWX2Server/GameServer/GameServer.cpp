#include <WinSock2.h>
#include <boost/bind.hpp>
#include <sstream>
#include <iomanip>
#include "ActorManager.h"
#include "ProxyManager.h"
#include "GameServer.h"
#include "GSSimLayer.h"
#include "GSNetLayer.h"
#include "GSAccountDBThread.h"
#include "GSGameDBThread.h"
#include "GSGameDBThread2nd.h"
#include "GSLogDBThread.h"
#include "GSLogDBThread2nd.h"
#include "GSSMSDBThread.h"
#ifdef SERV_ADD_EVENT_DB
#include "GSEventDBThread.h"
#endif //SERV_ADD_EVENT_DB
#ifdef SERV_ADD_SCRIPT_DB
#include "GSScriptDBThread.h"
#endif //SERV_ADD_SCRIPT_DB

#ifdef SERV_COUNTRY_TH
#include "../Common/OnlyGlobal/AuthAndBilling/TH/GSASBillingDBThread.h"	//	AsiaSoft Billing
#endif // SERV_COUNTRY_TH

#ifdef SERV_COUNTRY_TWHK
#include "../Common/OnlyGlobal/AuthAndBilling/TW/GSGASHBillingThread.h"	//	AsiaSoft Billing
#endif //SERV_COUNTRY_TWHK

#ifdef SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/HanBillingForSvr.h"
//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
#include "GSRelayDBThread.h"
#endif //SERV_RELAY_DB_CONNECTION
//}}
#endif //SERV_COUNTRY_TWHK

#ifdef SERV_GLOBAL_BILLING
#include "GSBillingDBThread.h"
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_COUNTRY_ID
#ifdef SERV_ID_NETMARBLE_PCBANG
#include "../Common/OnlyGlobal/NetCafe/ID/GSIDPcbangDBThread.h"	//	2013.07.01 lygan_������ // �ε��׽þ� �ۺ��� PC �� ���� DB
#endif //SERV_ID_NETMARBLE_PCBANG
#endif //SERV_COUNTRY_ID
//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	#include "GSNXWebDBThread.h"
#endif SERV_WEB_POINT_EVENT
//}}
#include "DBLayer.h"
#include "socket/NetCommon.h"
#include "KncSend.h"
#include "UserManager.h"
#include "GSUser.h"
#include "GSFSM.h"
#include "NProtectFunc.h"
#include "HackShield/HSAuth.h"

#include "NetError.h"
#include "RoomListManager.h"
//{{ 2009. 7. 16  ������	ä�θŴ���
#include "ChannelManager.h"
//}}

#include "X2Data/XSLRandomItemManager.h"
#include "X2Data/XSLManufactureItemManager.h"
#include "X2Data/XSLCashItemManager.h"
#include "X2Data/XSLTitleManager.h"
#include "X2Data/XSLQuestManager.h"
#include "AbuserLog/KAbuserLogManager.h"
#include "RewardTable.h"
#include "X2Data/XSLGuildManager.h"
#include "GameSysVal/GameSysVal.h"
#include "X2Data/XSLResolveItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLAttribEnchantItem.h"
#include "RecommendUserTable.h"
#include "ExpTable.h"
//{{ 2010. 9. 11	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	#include "X2Data/XSLPetManager.h"
#endif SERV_PET_SYSTEM
//}}
//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "BattleFieldListManager.h"
	#include "GSAutoPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 10. 19	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "GameEvent/GameEventManager.h"

//081021.hoons.
#include "FieldManager.h"
//081106.hoons.
#include "TRServer.h"

//{{ 2008. 11. 24  ������	��ŷ �Ŵ���
#include "GSRankingManager.h"
//}}

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 4. 22  ������	������������Ʈ
#include "PVPLobbyManager.h"
//}}

//{{ 2009. 11. 21  ������	���������ӱ�������
#include "NexonBillingTCPManager.h"
//}}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	#include "HackingProtectionManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}
//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	#include "GSScriptRefreshManager.h"
#endif SERV_REALTIME_SCRIPT
//}}

#ifdef DEF_TRADE_BOARD
	#include "PersonalShopItemList.h"
#endif DEF_TRADE_BOARD

#ifdef SERV_KOG_STATISTICS
	#include "KOGAgent.h"
#endif SERV_KOG_STATISTICS

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_SCRIPT_REFRESH
//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	#include "GameEvent/GameEventDBManager.h"
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

//{{ 2011. 03. 29	������	TBB �޸� ������
#ifdef SERV_TBB_MALLOC_PROXY_TEST
	#include <tbb/tbbmalloc_proxy.h>
#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011.10.14     ��μ�    ��� ����� Ư�� IP ������ ��� ����
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	#include "CompanyIPTable.h"
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}
//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#include "ClassChangeTable.h"
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	#include "X2Data\XSLStringFilter.h"
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}

//{{ 2012. 05. 09	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	#include "UdpRelayChecker/UdpRelayChecker.h"
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/ReleaseTickManager.h"
	#include "EventDataRefresh/EventDataRefreshManager.h"
	#include "EventDataRefresh/EventMonsterRefreshManager.h"
	#include "EventDataRefresh/EventNotifyMsgServerInfo.h"
	#include "EventDataRefresh/EventRewardDataRefreshManager.h"
	#include "EventDataRefresh/EventRewardLevelUpRefreshManager.h"
	#include "EventDataRefresh/EventRewardRetainUserItemRefreshManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2012. 06. 06	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	#include "..\Common\GameEvent\DailyGiftBoxManager.h"
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2012. 10. 16	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	#include "CorruptionMonitor.h"
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}
//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}
//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_CHANNELING_USER_MANAGER
#include "ChannelingUserManager.h"
#endif SERV_CHANNELING_USER_MANAGER

//{{ 2010. 11. 12  ��ȿ��	GameForge ä�� �α� ���� ó�� 
#ifdef SERV_RECORD_CHAT
#include "GSChatLogDBThread.h"
#endif SERV_RECORD_CHAT
//}}
#ifdef SERV_SYNTHESIS_AVATAR
#include "X2Data/XSLSynthesisManager.h"
#endif SERV_SYNTHESIS_AVATAR

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2013. 3. 11	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#include "GSLocalRankingManager.h"
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
	#include "X2Data/XSLBuffManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	#include "X2Data/XSLRidingPetManager.h"
#endif	// SERV_RIDING_PET_SYSTM

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallListManager.h"
	#include "GSWeddingHall.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GSGlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 13	��ȫ��	�۷ι� �̼� �Ŵ���

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"
	#include "GSFieldBossManager.h"
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    #include "X2Data/XSLEDInventoryExpand.h"
#endif //SERV_GOOD_ELSWORD

#ifdef SERV_STRING_FILTER_USING_DB
	#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//#include "vld.h"

#ifdef SERV_NPROTECT_CS_AUTH_30
void __stdcall CS3LogCallback( int nLogType, char* szLog )
{
	// �αװ� �߻� �� callback �Լ��� �α׸� �����մϴ�.
	// callback �Լ��� ��ϵǾ� ���� �ʴٸ� ȣ����� �ʽ��ϴ�.

	switch(nLogType)
	{
	case LOG_UPDATE:
		// ���Ӱ��� �������� ����, ���ι����Ǿ�����Ʈ���߻�������ȣ��˴ϴ�. (ȣ��� ����)
		START_LOG( cout, L"nProtect Update Log" )
			<< BUILD_LOG( szLog )
			<< END_LOG;
		break;
	case LOG_NORMAL:
		// �ʱ�ȭ��Ȳ, �Ϲ����ηα�, �����߻����Ƿαװ��߻�������ȣ��˴ϴ�. (ȣ��� ����)
		START_LOG( cout, L"nProtect Normal Log" )
			<< BUILD_LOG( szLog )
			<< END_LOG;
		break;
	case LOG_DEBUG:
		// ��� ������ ��Ŷ ������ ���� �˴ϴ�. (ȣ���,�� �ſ� ����)
		START_LOG( clog, L"nProtect Debug Log" )
			<< BUILD_LOG( szLog )
			<< END_LOG;
		break;
	default:
		START_LOG( cerr, L"nProtect Unknown Log" )
			<< BUILD_LOG( nLogType )
			<< BUILD_LOG( szLog )
			<< END_LOG;
	}
}

void __stdcall CS3UpdateInfoCallback( int nUpdateType, int nBefore, int nAfter )
{
	switch(nUpdateType)
	{
	case UPDATE_PROTOCOL:
		START_LOG( cout, L"nProtect Protocol Update Notification" )
			<< BUILD_LOG( nBefore )
			<< BUILD_LOG( nAfter )
			<< END_LOG;
		break;
	case UPDATE_GGVERSION:
		START_LOG( cout, L"nProtect GGVersion Update Notification" )
			<< BUILD_LOG( nBefore )
			<< BUILD_LOG( nAfter )
			<< END_LOG;
		break;
	default:
		START_LOG( cerr, L"nProtect Unknown Update Info" )
			<< BUILD_LOG( nUpdateType )
			<< BUILD_LOG( nBefore )
			<< BUILD_LOG( nAfter )
			<< END_LOG;
	}
}
#endif SERV_NPROTECT_CS_AUTH_30

NiImplementRTTI( KGameServer, KBaseServer );
ImplPfID( KGameServer, PI_GS_SERVER );

ImplToStringW( KGameServer )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

#define CLASS_TYPE      KGameServer

KGameServer::KGameServer(void)
: KBaseServer()
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	, m_bActiveLagCheck( false )
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-06-29	// �ڼ���
	, m_iTimeEnchantEventUID( 0 )
	, m_iNumOfTimeEnchantEvent( 0 )
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	, m_iExchangeLimitUID( 0 )
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
	, m_bFirstCashshopOnOffCheck( true )
#endif SERV_CONTENT_MANAGER_INT
{
}

KGameServer::~KGameServer(void)
{
}

#include <lua_tinker.h>
//using namespace lua_tinker;

void KGameServer::RegToLua()
{
    KBaseServer::RegToLua();

    //class_<KGameServer>( "KGameServer" )
    //    .inh<KBaseServer>()
    //    .def( "dump", KGameServer::Dump );

    //decl( "Server", this );
	lua_tinker::class_add<KGameServer>( g_pLua, "KGameServer" );
	lua_tinker::class_inh<KGameServer, KBaseServer>( g_pLua );
	lua_tinker::class_def<KGameServer>( g_pLua, "dump",						&KGameServer::Dump );
	lua_tinker::class_def<KGameServer>( g_pLua, "ver",						&KGameServer::ServerVersion );
	lua_tinker::class_def<KGameServer>( g_pLua, "SetHackShieldHSBFilePath",	&KGameServer::SetHackShieldHSBFilePath );
	//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
	lua_tinker::class_def<KGameServer>( g_pLua, "SMSTest",					&KGameServer::SMSTest_LUA );
#endif SERV_SMS_TEST
	//}}

	lua_tinker::decl( g_pLua, "Server", this );

	// TRServer reg lua
	SiKTRServer()->RegToLua();
}

const wchar_t*  g_szConfigFile = L"config_gs";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KGameServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KGSAccountDBThread( szDSN, bDBConnStr );
    case KDBLayer::DC_GAME:
        return new KGSGameDBThread( szDSN, bDBConnStr );
    case KDBLayer::DC_LOG:
        return new KGSLogDBThread( szDSN, bDBConnStr );
	case KDBLayer::DC_SMS:
		return new KGSSMSDBThread( szDSN, bDBConnStr );
		//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	case KDBLayer::DC_NX_WEB:
		return new KGSNXWebDBThread( szDSN, bDBConnStr );
#endif SERV_WEB_POINT_EVENT
		//}}
    case KDBLayer::DC_GAME_2ND:
        return new KGSGameDBThread2nd( szDSN, bDBConnStr );
    case KDBLayer::DC_LOG_2ND:
        return new KGSLogDBThread2nd( szDSN, bDBConnStr );
		//{{ 2010. 11. 12  ��ȿ��	GameForge ä�� �α� ���� ó��
#ifdef SERV_RECORD_CHAT
	case KDBLayer::DC_CHAT_LOG:
		return new KGSChatLogDBThread( szDSN, bDBConnStr );
#endif SERV_RECORD_CHAT
		//}}
#ifdef SERV_GLOBAL_BILLING
	case KDBLayer::DC_KOG_BILLING:
		return new KGSBillingDBThread( szDSN, bDBConnStr );

#if defined (SERV_COUNTRY_TWHK)
	case KDBLayer::DC_PUBLISHER_BILLING:	//	Gamania Elsword Point Billing
		return new KGSGASHBillingThread( szDSN, bDBConnStr );
#elif defined (SERV_COUNTRY_TH)
	case KDBLayer::DC_PUBLISHER_BILLING:	//	AsiaSoft Billing
		return new KGSASBillingDBThread( szDSN, bDBConnStr );
#endif // SERV_COUNTRY_XX

#endif // SERV_GLOBAL_BILLING

#ifdef SERV_ID_NETMARBLE_PCBANG
	case KDBLayer::DC_ID_PCBANG_AUTH:
		return new KGSIDPcbangDBThread( szDSN, bDBConnStr );
#endif //SERV_ID_NETMARBLE_PCBANG

		//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
	case KDBLayer::DC_JP_RELAY_DB:
		return new KGSRelayDBThread( szDSN, bDBConnStr );
#endif SERV_RELAY_DB_CONNECTION
		//}}
#ifdef SERV_ADD_EVENT_DB
	case KDBLayer::DC_EVENT:
		return new KGSEventDBThread( szDSN, bDBConnStr );
#endif //SERV_ADD_EVENT_DB

#ifdef SERV_ADD_SCRIPT_DB
	case KDBLayer::DC_SCRIPT:
		return new KGSScriptDBThread( szDSN, bDBConnStr );
#endif //SERV_ADD_SCRIPT_DB

    default:
        START_LOG( cerr, L"�����Ϸ��� DB ������ �̻���." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KGSGameDBThread( szDSN, bDBConnStr );
    }
}

bool KGameServer::Init()
{
    KSimLayer::KeepObject<KGSSimLayer>();
    KNetLayer::KeepObject<KGSNetLayer>();
    KActorManager::KeepObject<KUserManager>();
	//081021.hoons.
	KRoomManager::KeepObject<KFieldManager>();
	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::KeepObject<KUdpRelayChecker>();
#endif SERV_UDP_RELAY_CHECKER
	//}}

    KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KGSUser, KGSFSM, 32*1024> ) );
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_CLIENT );
#endif SERV_KEVENT_FROM
	//}}	

    _JIF( KBaseServer::Init(), return false );

	// ���� ����
	START_LOG( cout, GetMainVersion() );

	//081021.hoons. BaseServer::Init() �� ��� ����Ľ̷�Ʈ�� ����ֱ⶧���� �Ľ��� init�� �ؾ���.
	_JIF( GetKFieldManager()->Init(), return false );

	//////////////////////////////////////////////////////////////////////////
	// TRServer �����ϱ� ���� Profiler �̱��� ��ü �����ϱ�
    //SiKProfiler()->GetInstance();
	//////////////////////////////////////////////////////////////////////////

	//081106.hoons. TCP RELAY START.
	_JIF( SiKTRServer()->Init(), return false );

	dbg::cout << dbg::lbreak;

	//{{ 2009. 11. 21  ������	���������ӱ�������
    SiKProxyManager()->ConnectAll();
	//}}
    WriteServerInfoToDB();

#ifdef SERV_NPROTECT_CS_AUTH_30
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_NPGG ) == true )
	{
		UINT32 uReturn = InitCSAuth3("./nProtect/");
		if( uReturn != ERROR_SUCCESS )
		{
			START_LOG( cerr, L"nProtect CS ����3 �ʱ�ȭ ����" )
				<< BUILD_LOG( uReturn )
				<< END_LOG;
			return false;
		}

		SetCallbackFunction( CALLBACK_LOG, (PVOID)CS3LogCallback );
		SetCallbackFunction( CALLBACK_UPDATE, (PVOID)CS3UpdateInfoCallback );
	}
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef SERV_COUNTRY_JP
	// 2010. 10  ������ �Ϻ� ����
	int ret = -1;
	if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_ALPHA )
	{
		ret = HanBillingInit("J_ES_PRE");

		if(ret == 0)
		{
			START_LOG( clog, L"���Ĺ��� ���� �ʱ�ȭ �Ϸ�" )				
				<< END_LOG;
		}

	}
	else if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_REAL )
	{
		ret = HanBillingInit("J_ES");

		if(ret == 0)
		{
			START_LOG( clog, L"������� ���� �ʱ�ȭ �Ϸ�" )				
				<< END_LOG;
		}
	}

	if(ret != 0)
	{
		START_LOG( cerr, L"�����ʱ�ȭ ����" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}
#endif //SERV_COUNTRY_JP

	// nProtect �ѱ����� ��� ��뼳��
	//if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_NPGG ) == true )
	//{
	//	KNP_Enable( true );

	//	if( KNP_Init() == false )
	//	{
	//		START_LOG( cerr, L"nProtect �ʱ�ȭ ����.!" )
	//			<< END_LOG;

	//		return false;
	//	}
	//}
	//{{ 2009. 8. 18  ������	hack shield
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_HSHIELD ) == true )
	{
		KHSAuth::SetAhnHSServerHandle( _AhnHS_CreateServerObject( KHSAuth::GetHSBFilePath().c_str() ) );

		if( KHSAuth::GetAhnHSServerHandle() == ANTICPX_INVALID_HANDLE_VALUE )
		{
			START_LOG( cerr, L"Hack Shield �ʱ�ȭ ����! HSB������ ���� ���Ͽ����ϴ�." )
				<< BUILD_LOG( KHSAuth::GetHSBFilePath() )
				<< END_LOG;

			return false;
		}
	}
	//}}

#ifdef SERV_USE_XTRAP
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_XTRAP ) == true )
	{
		GetKGSSimLayer()->ReloadXTRAP_MAP();
	}
#endif SERV_USE_XTRAP

	SetServerReady( true );

	//{{ 2009. 8. 27  ������	���� �ʱ�ȭ �Ϸ�
	OnServerReadyComplete();
	//}}

#ifdef DEF_TRADE_BOARD
	SiKPersonalShopItemList();	// �ʱ������ ����.
#endif DEF_TRADE_BOARD

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	RequestRandomQuestInfo();
#endif SERV_RANDOM_DAY_QUEST
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS
	
#ifdef SERV_USE_GM_TOOL_INFO
	if( GetServerRollType() == KServerInfo::SRT_UPDATE_CASH_PRODUCT_LIST )
	{
		KDBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT kPacketNOT;
		kPacketNOT.m_CurrentItemTempletNameMap = SiCXSLItemManager()->GetItemNameContainer();
		SendToGameDB( DBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT, kPacketNOT );
	}
#endif //SERV_USE_GM_TOOL_INFO

    return true;
}

void KGameServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

	//{{ 2009. 8. 27  ������	����� ����Ʈ
	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKAbuserLogManager()->UpdateAllAbuserList();
#else
	SiKAbuserLogManager()->SendAbuserListReq();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	//}}

	//{{ 2009. 7. 6  ������		��ŷ ����
	SiKGSRankingManager()->Init(); // ��ŷ ���� �ʱ�ȭ
	//}}

	//{{ 2009. 11. 21  ������	���������ӱ�������
	switch( KSimLayer::GetKObj()->GetBillingFlag() )
	{
	case KSimLayer::BF_NEXON_KOREA_TEST:
	case KSimLayer::BF_NEXON_KOREA:
		{
			if( SiKNexonBillingTCPManager()->IsFirstConnectSucc() )
			{
				START_LOG( cout, L"�ؽ� ���� ���� ���� ó�� ���� ����!" );
			}
			else
			{
				START_LOG( cerr, L"�ؽ� ���� ���� ���� ���� ����." )
					<< END_LOG;

				// First Connect Successüũ �ؾ߸� ���Ӳ������� �����α� �����.
				SiKNexonBillingTCPManager()->FirstConnectSucc();
			}
		}
		break;
	}
	//}}

	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKReleaseTickManager()->Init();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	// 2013.07.18 lygan_������ // SiKGameEventManager ���� ���� �ʱ�ȭ �Ǿ���
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\GameServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\GameServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	// �̺�Ʈ ���� ���
	SiKGameEventManager()->Init();

#ifdef SERV_STRING_FILTER_USING_DB
	SiKStringFilterManager()->Init();
#endif //SERV_STRING_FILTER_USING_DB

	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKEventNotifyMsgServerInfo()->Init();
#else
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	SiKGameEventScriptManager()->Init();
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager()->Init();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
	SiKGSScriptRefreshManager()->Init();
	SiKGSScriptRefreshManager()->RegToLua( g_pLua );
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	if( SEnum::CheckFlag( GetServerRollType(), KServerInfo::SRT_UPDATE_CASH_PRODUCT_LIST ) == true )
	{
		KELOG_CASH_PRODUCT_LIST_UPDATE_NOT kPacketToLogDB;
		SiKNexonBillingTCPManager()->GetCashProductList( kPacketToLogDB.m_vecCashProductList );
		SendToLogDB( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT, kPacketToLogDB );

		START_LOG( cout, L"--- ��踦 ���� ĳ�� ������ ����Ʈ ������Ʈ ---" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacketToLogDB.m_vecCashProductList.size() );
	}
#endif SERV_CASH_ITEM_LIST
	//}}

	//{{ 2012. 10. 16	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	SiKCorruptionMonitor()->Init();
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	SiKGSWorldMissionManager()->Init();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2013. 3. 17	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKGSLocalRankingManager()->Begin();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_KOG_STATISTICS
	SiKOGAgent();
#endif SERV_KOG_STATISTICS
	
	// ������ �̸� ǥ��
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );

#ifdef SERV_TBB_MALLOC_PROXY_TEST
	START_LOG( cout, L"----------------------------------------------" );
	START_LOG( cout, L"----------- TBB �޸� ������ ��� -----------" );
	START_LOG( cout, L"----------------------------------------------" );
#endif SERV_TBB_MALLOC_PROXY_TEST

	//{{  2011.11.24     ��μ�    ĳ���� ���� �α�
#ifdef SERV_CHAR_CONNECT_LOG
	KDBE_CHANNEL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iServerGroupID = SiKChannelManager()->GetServerGroupID();
	SendToLogDB( DBE_CHANNEL_LIST_REQ, kPacketToDB );
#endif SERV_CHAR_CONNECT_LOG
	//}}
#ifdef SERV_ENTRY_POINT
    // �������� 2�� ���������� ����
    if ( GetKGSSimLayer()->GetServerGroupNum() == KGSSimLayer::DOUBLE_SERVER_GROUP )
    {
        KDBE_CHANNEL_LIST_REQ kPacketToDB2nd;
        kPacketToDB2nd.m_iServerGroupID = ( GetServerGroupID() == SEnum::SGI_SOLES ? SEnum::SGI_GAIA : SEnum::SGI_SOLES );
        SendToLogDB2nd( DBE_CHANNEL_LIST_REQ, kPacketToDB2nd );
    }
#endif SERV_ENTRY_POINT

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	InitTimeControlItemReleaseTick();
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
	InitTimeControlCubeInItemMappingReleaseTick();
#endif //SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->Init();
#endif SERV_UDP_RELAY_CHECKER
	//}}
#ifdef SERV_GLOBAL_MISSION_MANAGER
	SiKGSGlobalMissionManager()->Init();
#endif SERV_GLOBAL_MISSION_MANAGER
}

void KGameServer::ShutDown()
{
	//{{ 2013. 3. 17	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKGSLocalRankingManager()->ShutDown();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	//{{ 2009. 8. 18  ������	hack shield
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_HSHIELD ) == true )
	{
		_AhnHS_CloseServerHandle( KHSAuth::GetAhnHSServerHandle() );
	}
	//}}
#ifdef SERV_NPROTECT_CS_AUTH_30
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_NPGG ) == true )
	{
		CloseCSAuth3();
	}
#endif SERV_NPROTECT_CS_AUTH_30

	//{{ 2009. 9. 2  ������		�������������
#ifdef ABNORMAL_REASON
	GetKGSSimLayer()->SendToDBAbnormalReason( true );
#endif ABNORMAL_REASON
	//}}

    KBaseServer::ShutDown();      // �̰����� ��� ���ᰡ ������. ������ Layer�鵵 ��� �Ҹ��.

	//korea nProtect ��� ��������
	//KNP_CleanupGameguardAuth();

	GetKFieldManager()->ShutDown();

	//081106.hoons. tcp relay shutdown
	SiKTRServer()->ShutDown();
	KTRServer::ReleaseInstance();

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT	
	SiKGSScriptRefreshManager()->EndThread();
	KGSScriptRefreshManager::ReleaseInstance();
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->ShutDown();
	KUdpRelayChecker::ReleaseKObj();
#endif SERV_UDP_RELAY_CHECKER
	//}}
}

bool KGameServer::DestroyAllSession()
{
    // NetLayer�� Accepter�� ���� ����. ���� ��� Layer���� ����ִ�.

    KActorManager* pkActorManager = KActorManager::GetKObj();

    START_LOG( cout, L"���� ���� : " << pkActorManager->GetCount() << L" ��" );

	int iIndex = KStatistics::eSIColDR_Server_Shutdown;

	pkActorManager->ForEach( boost::bind( &KActor::SetDisconnectReason, _1, iIndex ) );

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

    // proxy manager�� ���� ó���� �ϴ� ���� ��ü�� �Ҹ�ó���� ���� ������ ���� �����Ѵ�.
    bool bProxyShutdown = SiKProxyManager()->Shutdown( 60.0f );

    LOG_CONDITION( bSuccess && bProxyShutdown, cout, cerr )
        << BUILD_LOG( m_dwShutDownWait )
        << BUILD_LOG( bSuccess )
        << BUILD_LOG( bProxyShutdown )
        << BUILD_LOG( pkActorManager->GetCount() )
        << BUILD_LOG( SiKDBLayer()->GetTotalEventNum() )
        << BUILD_LOG( ::GetTickCount() - dwBeginTick );

    return bSuccess && bProxyShutdown;
}

void KGameServer::Tick()
{
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	CTime tCurrentTime = CTime::GetCurrentTime();
#endif // SERV_BATTLE_FIELD_BOSS

	KBaseServer::Tick();

	//{{ 2010. 05. 31  ������	���� ���� ����
#ifdef SERV_CCU_NEW
	if( ::GetTickCount() - m_dwServerInfoUpdateTick > 20 * 1000 )
	{
		if( GetServerReady() )
		{
			WriteServerInfoToDB();
		}
	}
#else
	if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwServerInfoUpdateGap )
	{
		if( GetServerReady() )
		{
			WriteServerInfoToDB();
		}
	}
#endif SERV_CCU_NEW
	//}}    

	//nProtect ��ȣȭ ��� ������Ʈ
	//KNP_Tick();

	// TRServer tick
	SiKTRServer()->Tick();

//#ifdef SERV_KOG_STATISTICS
//	SiKOGAgent()->ConcurrentUser(GetServerGroupID(), SiKChannelManager()->GetChannelID(), KActorManager::GetKObj()->GetCount());
//
//	if( true == SiKOGAgent()->CheckDailyTime() &&	// �����ð��̰�
//		false == SiKOGAgent()->CheckDaily(CTime::GetCurrentTime()) && // ���� �Ⱥ��� ���̸�
//		SiKChannelManager()->GetChannelID() == 1 // 1�� ä���� ���
//		)
//	{
//		// ������¥ ����
//		SiKOGAgent()->SetDailyTime(CTime::GetCurrentTime());
//
//		//�Ϸ���踦 DB�� ��û����
//		SendToLogDB(DBE_CCU_AGENT_REQ, char());
//	}
//#endif SERV_KOG_STATISTICS
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	TickCheckTimeControlItem();
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	CheckGSFieldBossSystemRequest( tCurrentTime );
	CheckGSFieldBossSystem( tCurrentTime );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING  
	TickCheckTimeControlCubeInItemMapping();
#endif //SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\GameServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\GameServerWrite"));
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

#ifdef SERV_CONTENT_MANAGER_INT
	if( m_tCashshopOnOffCheckTimer.elapsed() > 120.0f )
	{
		KDBE_GET_CASHSHOP_ON_OFF_INFO_REQ kPacket;
		kPacket.m_bFirstCashshopOnOffCheck = m_bFirstCashshopOnOffCheck;
		kPacket.m_iReleaseTick = SiKGSContentManager()->GetReleaseTick();		
		kPacket.m_bEnableCashshop = SiKGSContentManager()->IsEnableCashShop();
		SendToKOGBillingDB( DBE_GET_CASHSHOP_ON_OFF_INFO_REQ, kPacket );

		m_tCashshopOnOffCheckTimer.restart();
		m_bFirstCashshopOnOffCheck = false;
	}
#endif SERV_CONTENT_MANAGER_INT
}

void KGameServer::SetHackShieldHSBFilePath( const char* pFilePath )
{
	KHSAuth::SetHSBFilePath( std::string( pFilePath ) );

	START_LOG( cout, L"Hack Shield HSB File Path" )
		<< BUILD_LOG( pFilePath );
}

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
void KGameServer::SetPCBangIPList( const std::map< std::wstring, int >& mapPCBangIPList )
{
	m_mapPCBangIPList = mapPCBangIPList;

	START_LOG( cout, L"PC Bang IP List size" )
		<< BUILD_LOG( mapPCBangIPList.size() );
	return;
}

int KGameServer::IsPCBangIP( const std::wstring& wstrIP )
{
	START_LOG( clog2, L"PC Bang IP Check" )
		<< BUILD_LOG( wstrIP );

	std::map<std::wstring, int>::const_iterator sit = m_mapPCBangIPList.find(wstrIP);
	if(sit != m_mapPCBangIPList.end())
	{
		return sit->second;
	}
	else
	{
		return 0;
	}

}
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

void KGameServer::ServerVersion()
{
	std::wcout << L"Server Ver : " << GetMainVersion() << std::endl;
}

//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
void KGameServer::SMSTest_LUA()
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

//{{ 2010. 05. 31  ������	���� ���� ����
#ifdef SERV_CCU_NEW
//////////////////////////////////////////////////////////////////////////

void KGameServer::WriteServerInfoToDB()
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	// �α��μ������� ���� ���� �����ϵ��� ������.
	KELG_UPDATE_CCU_INFO_NOT kNot;
	kNot.m_kCCUInfo.m_iServerUID		  = GetUID();
	kNot.m_kCCUInfo.m_iConcurrentUser	  = KActorManager::GetKObj()->GetCount();
	kNot.m_kCCUInfo.m_iCurPartyCount	  = SiKPartyListManager()->GetCurrentPartyCount();
	kNot.m_kCCUInfo.m_iPlayGamePartyCount = SiKPartyListManager()->GetPlayGamePartyCount();

#ifdef SERV_CHANNELING_USER_MANAGER
	SiKChannelingUserManager()->GetChannelingUserList( kNot.m_kCCUInfo.m_mapChannelingUserList );
#endif SERV_CHANNELING_USER_MANAGER

	SendToLoginServer( ELG_UPDATE_CCU_INFO_NOT, kNot );

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	SendToGlobalServer( EGB_UPDATE_CCU_INFO_NOT, kNot );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	m_dwServerInfoUpdateTick = ::GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

void KGameServer::WriteServerInfoToDB()
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	KDBE_UPDATE_SERVER_INFO_REQ kPacket;
	kPacket.m_wstrRegDate = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kPacket.m_iServerUID = GetUID();
	kPacket.m_iConcurrentUser = KActorManager::GetKObj()->GetCount();
	//{{ 2010. 01. 28  ������	������Ƽ
#ifdef SERV_CHANNEL_PARTY
	kPacket.m_iCurPartyCount = SiKPartyListManager()->GetCurrentPartyCount();
	kPacket.m_iPlayGamePartyCount = SiKPartyListManager()->GetPlayGamePartyCount();
#else
	kPacket.m_iCurPartyCount = SiKPartyManager()->GetCurrentPartyCount();
	kPacket.m_iPlayGamePartyCount = SiKPartyManager()->GetPlayGamePartyCount();
#endif SERV_CHANNEL_PARTY
	//}}
	SendToLogDB( DBE_UPDATE_SERVER_INFO_REQ, kPacket );

	m_dwServerInfoUpdateTick = ::GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_CCU_NEW
//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
void KGameServer::InitTimeControlItemReleaseTick()
{
	m_mapTimeControlItem_StaticDBReleaseTick.clear();
	std::map<int, int>::iterator mit = m_mapTimeControlItem_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_ITEM_CHECK);

	if ( mit != m_mapTimeControlItem_StaticDBReleaseTick.end() )
	{
		mit->second = 0;
	}
	else
	{
		m_mapTimeControlItem_StaticDBReleaseTick.insert(std::make_pair(TCIRTT_TIME_CONTROL_ITEM_CHECK, 0));
	}
}

void KGameServer::SetTimeControlItemReleaseTick(TimeControl_Item_Release_Tick_Type _enum ,std::map<int, int> &mapReleaseTick)
{

	std::map<int, int>::iterator sitstr = mapReleaseTick.find(_enum);
	std::map<int, int>::iterator sitdst = m_mapTimeControlItem_StaticDBReleaseTick.find(_enum);

	if(sitdst != m_mapTimeControlItem_StaticDBReleaseTick.end())
	{
		m_mapTimeControlItem_StaticDBReleaseTick.find(_enum)->second = sitstr->second;
	}
	else
	{
		m_mapTimeControlItem_StaticDBReleaseTick.insert(std::make_pair(sitstr->first, sitstr->second));
	}
}

int KGameServer::GetTimeControlItemReleaseTick(TimeControl_Item_Release_Tick_Type _enum)
{
	std::map<int, int>::const_iterator sit = m_mapTimeControlItem_StaticDBReleaseTick.find(_enum);

	if(sit != m_mapTimeControlItem_StaticDBReleaseTick.end())
	{
		return sit->second;
	}
	else
	{
		return -1;
	}
}

bool KGameServer::TickCheckTimeControlItem()
{
	if( m_tTimeControlItemCheckTimer.elapsed() > 60.0f )
	{
		int iReleaseTickCount = 0;

		iReleaseTickCount = GetTimeControlItemReleaseTick( TCIRTT_TIME_CONTROL_ITEM_CHECK );
		if ( iReleaseTickCount == -1 )
		{
			m_tTimeControlItemCheckTimer.restart();
			return false;
		}

		KDBE_GET_ITEM_ONOFF_NPCSHOP_REQ kPacket;
		kPacket.m_mapTimeControlItem_StaticDBReleaseTick.insert(std::make_pair( TCIRTT_TIME_CONTROL_ITEM_CHECK, iReleaseTickCount ));
		SendToLogDB( DBE_GET_ITEM_ONOFF_NPCSHOP_REQ, kPacket );

		m_tTimeControlItemCheckTimer.restart();
	}
	return true;
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
void KGameServer::InitTimeControlCubeInItemMappingReleaseTick()
{
	m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.clear();
	std::map<int, int>::iterator mit = m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING);

	if ( mit != m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end() )
	{
		mit->second = 0;
	}
	else
	{
		m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.insert(std::make_pair(TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING, 0));
	}
}

void KGameServer::SetTimeControlCubeInItemMappingReleaseTick(TimeControl_CubeInItemMapping_Release_Tick_Type _enum ,std::map<int, int> &mapReleaseTick)
{
	std::map<int, int>::iterator sitstr = mapReleaseTick.find(_enum);
	std::map<int, int>::iterator sitdst = m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(_enum);

	if(sitdst != m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end())
	{
		m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(_enum)->second = sitstr->second;
	}
	else
	{
		m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.insert(std::make_pair(sitstr->first, sitstr->second));
	}
}

int KGameServer::GetTimeControlCubeInItemMappingReleaseTick(TimeControl_CubeInItemMapping_Release_Tick_Type _enum)
{
	std::map<int, int>::const_iterator sit = m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(_enum);

	if(sit != m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end())
	{
		return sit->second;
	}
	else
	{
		return -1;
	}
}

bool KGameServer::TickCheckTimeControlCubeInItemMapping()
{
	if( m_tTimeControlCubeInItemMappingCheckTimer.elapsed() > 60.0f )
	{
		int iReleaseTickCount = 0;

		iReleaseTickCount = GetTimeControlCubeInItemMappingReleaseTick( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING );
		if ( iReleaseTickCount == -1 )
		{
			m_tTimeControlCubeInItemMappingCheckTimer.restart();
			return false;
		}

		KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ kPacket;
		kPacket.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.insert(std::make_pair( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING, iReleaseTickCount ));
		SendToLogDB( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ, kPacket );

		m_tTimeControlCubeInItemMappingCheckTimer.restart();
	}
	return true;
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 

void KGameServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
   _CASE( DBE_SERVER_ON_ACK, KDBE_GAME_SERVER_ON_ACK );
   _CASE( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
	//{{ 2009. 2. 13  ������	ä�θ���Ʈ
	CASE( DBE_CHANNEL_LIST_ACK );
	//}}
    CASE( ERM_ROOM_LIST_INFO_NOT );
	CASE( ERM_SQUARE_LIST_INFO_NOT );

   _CASE( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT );
	CASE( ENX_UPDATE_PRODUCT_LIST_NOT );
	CASE( DBE_EVENT_UPDATE_ACK );

	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	CASE( DBE_RELEASE_TICK_UPDATE_ACK );
	CASE( DBE_EVENT_DATA_UPDATE_ACK );
	CASE( DBE_EVENT_MONSTER_UPDATE_ACK );
	CASE( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK );
	CASE( DBE_EVENT_REWARD_DATA_UPDATE_ACK );
	//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	CASE( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}
	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	CASE( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
#else
	//CASE( ELG_RANKING_TITLE_REWARD_NOT );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}	
	CASE( DBE_INSERT_TITLE_ACK );
	CASE( DBE_INSERT_REWARD_TO_POST_ACK );
	//{{ 2008. 10. 31  ������	����� ����Ʈ
	CASE( DBE_ABUSER_LIST_ACK );
	//}}
	//{{ 2008. 11. 5  ������	���� ����	
	CASE( ERM_UPDATE_FIELD_INFO_NOT );
	//{{ 2009. 4. 22  ������	������������Ʈ
	CASE( ECN_NEW_PVP_USER_INFO_NOT );
	CASE( ECN_UPDATE_PVP_USER_INFO_NOT );
   _CASE( ECN_CHECK_INVALID_PVP_USER_NOT, UidType );
	//}}
	//{{ 2009. 6. 1  ������		ä���̵�
	CASE( ECN_CHECK_CHANNEL_CHANGE_REQ );
	//}}

	//{{ 2009. 6. 3  ������		Ȯ����
   _CASE( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT );
	//}}

	//{{ 2009. 7. 6  ������		��ŷ ����
	CASE( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK );
	CASE( ELG_HENIR_RANKING_REFRESH_NOT );
	CASE( ELG_WEB_RANKING_REFRESH_NOT );
	//}}

	//{{ 2009. 7. 10  ������	��� �ý���
	CASE( ERM_INCREASE_PARTY_FEVER_NOT );
	//}}
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2009. 10. 30  ������	��巹��
#ifdef GUILD_TEST
	CASE( DBE_JOIN_GUILD_ACK );

	CASE( ELG_KICK_RESULT_LETTER_NOT );

	CASE( DBE_EXPAND_GUILD_MAX_MEMBER_ACK );

	CASE( DBE_UPDATE_GUILD_EXP_ACK );

   _CASE( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
#endif GUILD_TEST
	//}}

   //{{ 2010. 02. 01  ������	��� �Խ���
#ifdef SERV_GUILD_AD
   _CASE( ELG_SEND_LETTER_JOIN_GUILD_NOT, KDBE_ACCEPT_JOIN_GUILD_ACK );
#endif SERV_GUILD_AD
   //}}

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	CASE( DBE_CHECK_HACKING_TOOL_LIST_ACK );
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2010. 02. 17  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY
	CASE( EPM_OPEN_PARTY_LIST_NOT );
	CASE( EPM_CLOSE_PARTY_LIST_NOT );
	CASE( EPM_ADD_PARTY_LIST_NOT );
	CASE( EPM_DEL_PARTY_LIST_NOT );
	CASE( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT );
	CASE( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT );
	CASE( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT );
	CASE( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT );
#endif SERV_CHANNEL_PARTY
	//}}
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	CASE( DBE_INCREASE_WEB_POINT_ACK );
   _CASE( ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
	//}}

#ifdef DEF_TRADE_BOARD
    CASE( ERM_PERSONAL_SHOP_INFO_NOT );
#endif DEF_TRADE_BOARD

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
   _CASE( ESR_ORDER_TO_GAME_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );
   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_KOG_STATISTICS
   CASE( DBE_CCU_AGENT_ACK );
#endif SERV_KOG_STATISTICS

	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	CASE( E_DISCONNECT_SERVER_REPORT_NOT );
   _CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
	//}}

   //{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
   _CASE( EGB_UPDATE_MISSION_INFO_NOT, KEGB_GET_MISSION_INFO_ACK );
	CASE( EGB_GET_MISSION_INFO_ACK );
	CASE( EGB_UPDATE_WORLD_BUFF_NOT );
	CASE( EGB_UPDATE_MISSION_START_TIME_NOT );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	CASE( EGB_TODAY_RANDOM_QUEST_ACK );
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	_CASE( EGB_SHUT_DOWN_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
#endif SERV_SHUTDOWN_SYSTEM
	//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	CASE( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK );
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
	CASE( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	CASE( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ERM_BATTLE_FIELD_ROOM_INFO_NOT );
	CASE( EGB_GET_AUTO_PARTY_BONUS_INFO_ACK );
	CASE( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT );
	CASE( ERM_END_GAME_REGROUP_PARTY_NOT );
	CASE( ERM_COMPLETE_RETURN_TO_FIELD_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	_CASE( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	CASE_NOPARAM( EGB_EARTH_QUAKE_NOT );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
	//{{ 2012. 10. 16	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	CASE( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	_CASE( EGB_JACKPOT_EVENT_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
	_CASE( EGB_JACKPOT_EVENT_POST_NOT, KDBE_INSERT_REWARD_TO_POST_REQ );
#endif SERV_EVENT_JACKPOT
	//}}
	//{{ 2013. 3. 11	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, SiKGSLocalRankingManager() );
	CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_RESET_NOT, SiKGSLocalRankingManager() );
	CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, SiKGSLocalRankingManager() );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	CASE( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	CASE( ECN_SET_ACTIVE_LAGCHECK_NOT );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	_CASE( ELG_WEDDING_HALL_INFO_UPDATE_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK );
	CASE( ERM_WEDDING_HALL_INFO_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
	CASE( EGB_TIME_ENCHANT_EVENT_INFO_NOT );
	CASE( EGB_TIME_ENCHANT_EVENT_INFO_ACK );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	CASE( EGB_EXCHANGE_LIMIT_INFO_NOT );
	CASE( EGB_EXCHANGE_LIMIT_INFO_ACK );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
	CASE( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK );
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	CASE( DBE_EVENT_DB_SCRIPT_ACK );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
	_CASE( EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, KEGB_GET_GLOBAL_MISSION_INFO_ACK );
	CASE( EGB_GET_GLOBAL_MISSION_INFO_ACK );
	CASE( EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER
	//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-29	// �ڼ���
	CASE( ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT );
	CASE_NOPARAM( EGB_VERIFY_SERVER_CONNECT_NOT );
	CASE_NOPARAM( EGB_VERIFY_SERVER_DISCONNECT_NOT );
	CASE( EGB_BATTLE_FIELD_BOSS_INFO_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_ENTRY_POINT
    _CASE( DBE_CHANNEL_LIST_2ND_ACK, KDBE_CHANNEL_LIST_ACK );
#endif SERV_ENTRY_POINT

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	_CASE( EGS_ADD_COMMON_FLAG_NOT, KEGS_ADD_COMMON_FLAG_NOT );
	_CASE( EGS_DEL_COMMON_FLAG_NOT, KEGS_DEL_COMMON_FLAG_NOT );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

#ifdef SERV_STRING_FILTER_USING_DB
	CASE( DBE_CHECK_STRING_FILTER_UPDATE_ACK );
	CASE( DBE_STRING_FILTER_UPDATE_ACK );
#endif //SERV_STRING_FILTER_USING_DB

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ��� - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
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

void KGameServer::SendToCnServer( unsigned short usEventID )
{
    SendToCnServer( usEventID, char() );
}

void KGameServer::SendToLoginServer( unsigned short usEventID )
{
	SendToLoginServer( usEventID, char() );
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
void KGameServer::SendToGlobalServer( unsigned short usEventID )
{
	SendToGlobalServer( usEventID, char() );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_GAME_SERVER_ON_ACK )
{
	//��ŸƮ�� ���������� ���α׷��� ����
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
        ::exit( 0 );
        return;
	}

    SetServerInfo( kPacket_.m_kServerInfo );

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	SetPCBangIPList( kPacket_.m_mapPCBangIPList );
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

	std::map< int, KNetAddress >::iterator mit;
	for( mit = kPacket_.m_mapCenterList.begin(); mit != kPacket_.m_mapCenterList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_CENTER, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}

	for( mit = kPacket_.m_mapLoginList.begin(); mit != kPacket_.m_mapLoginList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_LOGIN, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	for( mit = kPacket_.m_mapGlobalList.begin(); mit != kPacket_.m_mapGlobalList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_GLOBAL, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2009. 5. 5  ������		ä�� ����
	SiKChannelManager()->Init( GetServerGroupID(), kPacket_.m_iChannelID, kPacket_.m_kBonusInfo );
	//}}

    SetEvent( m_spEventAck.get() );
}

_IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList )
{
    SetServerList( kPacket_ );
}

//{{ 2009. 2. 13  ������	ä�θ���Ʈ
IMPL_ON_FUNC( DBE_CHANNEL_LIST_ACK )
{
	SiKChannelManager()->UpdateChannelList( kPacket_.m_mapChannelList );

	//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	SiKChannelManager()->UpdateChannelBonusList(kPacket_.m_mapChannelBonusList);
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
}

#ifdef SERV_ENTRY_POINT
_IMPL_ON_FUNC( DBE_CHANNEL_LIST_2ND_ACK, KDBE_CHANNEL_LIST_ACK )
{
    SiKChannelManager()->UpdateChannelList2nd( kPacket_.m_mapChannelList );

    SiKChannelManager()->UpdateChannelBonusList2nd( kPacket_.m_mapChannelBonusList );
}
#endif SERV_ENTRY_POINT

IMPL_ON_FUNC( ERM_ROOM_LIST_INFO_NOT )
{
    switch( kPacket_.m_iCode )
    {
    case NetError::ERR_ROOM_14:
        // �� ���� �߰�.
		if( SiKRoomListManager()->AddRoomInfo( kPacket_.m_kRoomInfo ) )
        {
			int iProxyID = ( int )KncUid::ExtractServerID( kPacket_.m_kRoomInfo.m_RoomUID );
            SiKProxyManager()->SetRoomServerLoad( iProxyID, true );
        }
        break;
    case NetError::ERR_ROOM_15:
        // �� ���� ����.
		SiKRoomListManager()->ChangeRoomInfo( kPacket_.m_kRoomInfo );
        break;
    case NetError::ERR_ROOM_16:
        // �� ���� ����.
		if( SiKRoomListManager()->DelRoomInfo( kPacket_.m_kRoomInfo ) )
        {
			int iProxyID = ( int )KncUid::ExtractServerID( kPacket_.m_kRoomInfo.m_RoomUID );
            SiKProxyManager()->SetRoomServerLoad( iProxyID, false );
        }
        break;
    default:
        START_LOG( cerr, L"�� ���� ���� �ڵ尡 �̻���." )
            << BUILD_LOG( kPacket_.m_iCode )
            << END_LOG;

        break;
    }
}

IMPL_ON_FUNC( ERM_SQUARE_LIST_INFO_NOT )
{
#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 22  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( KncUid::ExtractServerGroupID( kPacket_.m_kSquareInfo.m_iSquareUID ) != GetServerGroupID() )
	{
		START_LOG( cerr, L"�ٸ� �������� ���� �����Դϴ�. �������� ���� �ȵǴµ�.." )
			<< BUILD_LOG( kPacket_.m_kSquareInfo.m_iSquareUID )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_kSquareInfo.m_iSquareUID ) )
			<< BUILD_LOG( GetServerGroupID() )
			<< END_LOG;		
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	switch( kPacket_.m_iCode )
	{
	case NetError::ERR_ROOM_14:
		// ���� ���� �߰�.
		if( SiKRoomListManager()->AddSquareInfo( kPacket_.m_kSquareInfo ) )
		{
			//SiKProxyManager()->SetLoad( kPacket_.m_kRoomInfo.m_RoomUID, true );
		}
		break;
	case NetError::ERR_ROOM_15:
		// ���� ���� ����.
		SiKRoomListManager()->ChangeSquareInfo( kPacket_.m_kSquareInfo );
		break;
	case NetError::ERR_ROOM_16:
		// ���� ���� ����.
		if( SiKRoomListManager()->DelSquareInfo( kPacket_.m_kSquareInfo ) )
		{
			//SiKProxyManager()->SetLoad( kPacket_.m_kRoomInfo.m_RoomUID, false );
		}
		break;
	default:
		START_LOG( cerr, L"���� ���� ���� �ڵ尡 �̻���." )
			<< BUILD_LOG( kPacket_.m_iCode )
			<< END_LOG;

		break;
	}
}

_IMPL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT )
{
    UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket_ );

	// GM������ ��쿡�� cout���� �����
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS )
	{
		START_LOG( cout, L"[ALL SERVERS NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
	else if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_SERVER_GROUP )
	{
		START_LOG( cout, L"[SERVER GROUP NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	else if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
	{
		START_LOG( cout, L"[ALL SERVERS {SHUT_DOWN} NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
#endif SERV_SHUTDOWN_SYSTEM
	//}}
	else 
	{
		START_LOG( clog, L"[NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
}

IMPL_ON_FUNC( ENX_UPDATE_PRODUCT_LIST_NOT )
{
	SiCXSLCashItemManager()->UpdateProductList( kPacket_.m_vecProductItemID, kPacket_.m_vecCashItemRessolveInfo, kPacket_.m_ulProductNoResStone );

	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	if( SEnum::CheckFlag( GetServerRollType(), KServerInfo::SRT_UPDATE_CASH_PRODUCT_LIST ) == true )
	{
		KELOG_CASH_PRODUCT_LIST_UPDATE_NOT kPacketToLogDB;
		kPacketToLogDB.m_vecCashProductList = kPacket_.m_vecCashProductList;
		SendToLogDB( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT, kPacketToLogDB );

		START_LOG( cout, L"--- ��踦 ���� ĳ�� ������ ����Ʈ ������Ʈ ---" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacketToLogDB.m_vecCashProductList.size() );
	}
#endif SERV_CASH_ITEM_LIST
	//}}
}

IMPL_ON_FUNC( DBE_EVENT_UPDATE_ACK )
{
	if( kPacket_.m_vecEventList.size() > 0 )
	{

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		
		if ( SiKGameEventDBManager()->GetMapEventDBData().empty() == true )
		{
			SendToEventDB( DBE_EVENT_DB_SCRIPT_REQ );
		}
		else
		{
			SiKGameEventManager()->SetEvent( kPacket_.m_vecEventList );
		}
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		SiKGameEventManager()->SetEvent( kPacket_.m_vecEventList );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	}
}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_RELEASE_TICK_UPDATE_ACK )
{
	SiKReleaseTickManager()->SetReleaseTickData( kPacket_.m_mapReleaseTick );
}
IMPL_ON_FUNC( DBE_EVENT_DATA_UPDATE_ACK )
{
	SiKEventDataRefreshManager()->SetEventData( kPacket_.m_mapEventData );
}

IMPL_ON_FUNC( DBE_EVENT_MONSTER_UPDATE_ACK )
{
	SiKEventMonsterRefreshManager()->SetEventMonsterData( kPacket_.m_mapEventMonster );
}

IMPL_ON_FUNC( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK )
{
	SiKEventNotifyMsgServerInfo()->SetEventNotifyMsgServerInfoData( kPacket_.m_vecEventNotifyMsgServerInfo );
}

IMPL_ON_FUNC( DBE_EVENT_REWARD_DATA_UPDATE_ACK )
{
	SiKEventRewardDataRefreshManager()->SetRewardData( kPacket_.m_mapRewardData );
}

//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
IMPL_ON_FUNC( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK )
{
	SiKEventRewardLevelUpRefreshManager()->SetRewardLevelUp( kPacket_.m_mapLevelUpRewardItem );
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK )
{
	SiKEventRewardRetainUserItemRefreshManager()->SetRewardRetainUserItem( kPacket_.m_mapRewardItem );
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//{{ 2008. 10. 9  ������	Ÿ��Ʋ
//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
#else
//IMPL_ON_FUNC( ELG_RANKING_TITLE_REWARD_NOT )
//{
//	// Ư�� ������ ������Ʈ �ϵ��� �Ұ�!
//	START_LOG( cout, L"�α��� �����κ��� ��ŷ Īȣ ���� ������ �����߽��ϴ�! Īȣ ���� ������ �����մϴ�!" )
//		<< BUILD_LOG( kPacket_.m_mapTitleReward.size() );
//	
//	// GameDB�� Ÿ��Ʋ�� insert���ش�.
//	std::map< UidType, KRankingTitleReward >::const_iterator mit;
//	for( mit = kPacket_.m_mapTitleReward.begin(); mit != kPacket_.m_mapTitleReward.end(); ++mit )
//	{
//		const KRankingTitleReward& kTitleReward = mit->second;
//		std::wstring wstrMessage;
//
//		std::vector< KTitleReward >::const_iterator vit;
//		for( vit = kTitleReward.m_vecReward.begin(); vit != kTitleReward.m_vecReward.end(); ++vit )
//		{
//			// Ÿ��Ʋ ���� �ֱ�
//			KDBE_INSERT_TITLE_REQ kPacket;
//			kPacket.m_iUnitUID		   = kTitleReward.m_iUnitUID;
//			kPacket.m_iTitleID		   = vit->m_iTitleID;
//			kPacket.m_sPeriod		   = vit->m_sPeriod;
//			kPacket.m_bGameServerEvent = true;
//			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacket );
//
//			const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( vit->m_iTitleID );
//			if( pTitleInfo == NULL )
//			{
//				START_LOG( cerr, L"�������� �ʴ� Īȣ�Դϴ�!" )
//					<< BUILD_LOG( vit->m_iTitleID )
//					<< END_LOG;
//
//				continue;
//			}
//
//			wstrMessage += pTitleInfo->m_wstrTitleName;
//			wstrMessage += L",\n\n";
//		}
//
//		// Ÿ��Ʋ ���� ���� ������ �˸���
//		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
//		kPacketToDB.m_iFromUnitUID = kTitleReward.m_iUnitUID;
//		kPacketToDB.m_iToUnitUID   = kTitleReward.m_iUnitUID;
//		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_RANKING_TITLE;
//		kPacketToDB.m_iRewardID	   = 0; // �������
//		kPacketToDB.m_wstrMessage  = wstrMessage;
//		kPacketToDB.m_bGameServerEvent = true;
//		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
//	}
//}
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

IMPL_ON_FUNC( DBE_INSERT_TITLE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( ELG_INSERT_TITLE_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_ACK )
{
	// ���� ������
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KELG_FIND_USER_SEND_LETTER_NOT kPacketNot;
		kPacketNot.m_kLetter = kPacket_.m_iRewardLetter;
		SendToLoginServer( ELG_FIND_USER_SEND_LETTER_NOT, kPacketNot );
	}
}
//}}

//{{ 2008. 10. 31  ������	����� ����Ʈ
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_ABUSER_LIST_ACK )
{
	if( kPacket_.m_vecAbuserList.empty() )
		return;

	START_LOG( cout, L"����� ����Ʈ ����!" );

	// ����� ����Ʈ ������Ʈ
	SiKAbuserLogManager()->SetAbuserList( kPacket_.m_iRtt, kPacket_.m_vecAbuserList );
}
#else
IMPL_ON_FUNC( DBE_ABUSER_LIST_ACK )
{
	if( kPacket_.m_mapAbuserList.empty() )
		return;

	START_LOG( cout, L"����� ����Ʈ ����!" );

	// ����� ����Ʈ ������Ʈ
	SiKAbuserLogManager()->UpdateAbuserList( kPacket_.m_mapAbuserList );
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

//{{ 2008. 11. 5  ������	���� ����
IMPL_ON_FUNC( ERM_UPDATE_FIELD_INFO_NOT )
{
	// �ʵ� ���� ������Ʈ
	SiCXSLMapData()->UpdateFieldInfo( kPacket_.m_iMapID, kPacket_.m_iFieldUID );

	START_LOG( cout, L"�ʵ� ���� ������Ʈ!" )
		<< BUILD_LOG( kPacket_.m_iMapID )
		<< BUILD_LOG( kPacket_.m_iFieldUID );
}
//}}

//{{ 2009. 4. 22  ������	������������Ʈ
IMPL_ON_FUNC( ECN_NEW_PVP_USER_INFO_NOT )
{
	if( !SiKPVPLobbyManager()->AddPVPUser( kPacket_.m_kUserInfo, kPacket_.m_nGSUID ) )
	{
        START_LOG( cerr, L"pvp user �߰� ����!" )
			<< BUILD_LOG( kPacket_.m_nGSUID )
			<< BUILD_LOG( kPacket_.m_kUserInfo.m_iUnitUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ECN_UPDATE_PVP_USER_INFO_NOT )
{
	std::vector< KPVPUserSimpleInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecUserInfo.begin(); vit != kPacket_.m_vecUserInfo.end(); ++vit )
	{
		if( vit->m_cState == KCommunityUserInfo::US_LEAVE_LOBBY )
		{
			if( !SiKPVPLobbyManager()->RemovePVPUser( vit->m_iUnitUID ) )
			{
				START_LOG( cerr, L"pvp user ���� ����!" )
					<< BUILD_LOG( vit->m_iUnitUID )
					<< END_LOG;
			}
		}
		else
		{
			if( !SiKPVPLobbyManager()->UpdatePVPUserInfo( vit->m_iUnitUID, vit->m_cState, vit->m_iStateCode, vit->m_iRoomUID ) )
			{
				START_LOG( cwarn, L"pvp user ���� ����!" )
					<< BUILD_LOG( vit->m_iUnitUID )
					<< BUILD_LOGc( vit->m_cState )
					<< BUILD_LOG( vit->m_iStateCode )
					<< BUILD_LOG( vit->m_iRoomUID )
					<< END_LOG;
			}
		}
	}	
}

_IMPL_ON_FUNC( ECN_CHECK_INVALID_PVP_USER_NOT, UidType )
{
	// ������ ������ ���Ӽ����� �������� ����Ʈ���� �����ϱ�
	SiKPVPLobbyManager()->RemovePVPUserDisconnectGS( kPacket_ );
}
//}}

//{{ 2009. 6. 1  ������		ä���̵�
IMPL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_REQ )
{
	KECN_CHECK_CHANNEL_CHANGE_ACK kPacket;
	kPacket.m_iSourGSUID = kPacket_.m_iSourGSUID;
	kPacket.m_iCharUID = kPacket_.m_iCharUID;

	// ���� ����( ä���̵� �������� )
	int iMaxUser = GetMaxUser();
	if( iMaxUser <= 0 )
	{
		// 0������ ����
		iMaxUser = 1;
	}

	int iNowUser = KActorManager::GetKObj()->GetCountNoLock() + SiKChannelManager()->GetChannelChangeWaitingUserCount();

	// ���� �ִ� ������ 5%�� �Ǿ�� ä���̵� ����
	if( ( static_cast<float>( iMaxUser - iNowUser ) / static_cast<float>( iMaxUser ) ) < 0.05f )
	{
		START_LOG( clog, L"ä���� ���� ���������� �����Ͽ� ä���̵��Ҽ� �����ϴ�" )
			<< BUILD_LOG( SiKChannelManager()->GetChannelID() )
			<< BUILD_LOG( iMaxUser )
			<< BUILD_LOG( iNowUser )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_08;
		goto end_proc;
	}

	// ä�� ���� ����
	if( SiKChannelManager()->NewChannelChangeUser( kPacket_.m_iCharUID, kPacket_.m_kChangeUserInfo ) == false )
	{
		START_LOG( cerr, L"ä�� ���� ���� ����" )
			<< BUILD_LOG( kPacket_.m_iCharUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_05;
		goto end_proc;
	}

	// ��ϼ���!
	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToCnServer( ECN_CHECK_CHANNEL_CHANGE_ACK, kPacket );
}
//}}

//{{ 2009. 6. 3  ������		Ȯ����
_IMPL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT )
{
	// ��� �����鿡�� ��ε�ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ECN_MEGAPHONE_MESSAGE_NOT, kPacket_ );
}
//}}

//{{ 2009. 7. 6  ������		��ŷ ����
IMPL_ON_FUNC( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK )
{
	// ��ϸ��� �ð� ��ŷ ���� �α��� �����κ��� �޾ƿ°� ������Ʈ
	SiKGSRankingManager()->UpdateHenirRanking( kPacket_.m_mapHenirRanking, kPacket_.m_mapLastRank );
}

IMPL_ON_FUNC( ELG_HENIR_RANKING_REFRESH_NOT )
{	
	START_LOG( clog2, L"�α��μ����κ��� ��ϸ� ��ŷ������ �����ߴ�!" )
		<< BUILD_LOG( kPacket_.m_mapHenirRanking.size() );

	// ���ŵ� ��ϸ��� �ð� ��ŷ ���� ������Ʈ
	SiKGSRankingManager()->UpdateHenirRanking( kPacket_.m_mapHenirRanking );
}

IMPL_ON_FUNC( ELG_WEB_RANKING_REFRESH_NOT )
{
	// ���ŵ� ����&���� ��ŷ ���� ������Ʈ
	SiKGSRankingManager()->UpdateDungeonAndPvpRanking( kPacket_ );
}
//}}

//{{ 2009. 7. 10  ������	��� �ý���
IMPL_ON_FUNC( ERM_INCREASE_PARTY_FEVER_NOT )
{
	// ��Ƽ ����ġ ����!
	//{{ 2010. 01. 28  ������	������Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_IncreasePartyFever( kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_IncreasePartyFever( kPacket_.m_iPartyUID ) == false )
#endif SERV_CHANNEL_PARTY
	//}}
	{
        START_LOG( cerr, L"��Ƽ ��� ��ġ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}
}
//}}

//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( ERM_MAX_PARTY_FEVER_NOT )
{
	// ��Ƽ ����ġ �ִ� ����
	if( SiKPartyListManager()->E_MaxPartyFever( kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ ��� ��ġ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 10. 30  ������	��巹��
#ifdef GUILD_TEST

IMPL_ON_FUNC( DBE_JOIN_GUILD_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��� ���� ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kJoinGuildMember.m_iUnitUID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// �����ϴ��� �α��� ���� �ѹ� ���ٿ���.
	}

	// ���� ���� ������Ʈ�Ϸ� ����!
	KELG_JOIN_GUILD_REQ kPacketToLg;
	kPacketToLg.m_iOK = kPacket_.m_iOK;
	kPacketToLg.m_iUnitUID = kPacket_.m_kJoinGuildMember.m_iUnitUID;
	kPacketToLg.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacketToLg.m_kLoginGuildMember = kPacket_.m_kJoinGuildMember;
	kPacketToLg.m_kLoginGuildMember.m_ucMemberShipGrade = SEnum::GUG_NEW_USER; // ��� ó�� ���������� ��� ���� ȸ������ �����
	SendToLoginServer( ELG_JOIN_GUILD_REQ, kPacketToLg );
}

IMPL_ON_FUNC( ELG_KICK_RESULT_LETTER_NOT )
{
	// ��� �������� ���� ����� �˸���.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iToUnitUID   = kPacket_.m_iUnitUID;
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_GUILD_KICK; // ��� ���� ����
	kPacketToDB.m_bGameServerEvent = true;
	kPacketToDB.m_wstrMessage = kPacket_.m_wstrGuildName;
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_EXPAND_GUILD_MAX_MEMBER_ACK )
{
	// ����ػ��� �����ϸ� ���Ŵ����� ������Ʈ �Ϸ� ����!
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( DLG_UPDATE_EXPAND_GUILD_MAX_MEMBER_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( DBE_UPDATE_GUILD_EXP_ACK )
{
	// ��� EXP DB������Ʈ �����ϸ� ���Ŵ����� ������Ʈ �Ϸ� ����!
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( ELG_UPDATE_GUILD_EXP_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT )
{
	// ��� ������ ��ü ������ ���ͼ����� ������!
	SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket_ );
}

#endif GUILD_TEST
//}}

//{{ 2010. 02. 01  ������	��� �Խ���
#ifdef SERV_GUILD_AD

_IMPL_ON_FUNC( ELG_SEND_LETTER_JOIN_GUILD_NOT, KDBE_ACCEPT_JOIN_GUILD_ACK )
{
	// ��� �������� ���� ����� �˸���.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID;
	kPacketToDB.m_iToUnitUID   = kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID;
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_GUILD_JOIN; // ��� ���� ����
	kPacketToDB.m_wstrMessage  = kPacket_.m_wstrGuildName;
	kPacketToDB.m_bGameServerEvent = true;	
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
}

#endif SERV_GUILD_AD
//}}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST

IMPL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_ACK )
{
	SiKHackingProtectionManager()->UpdateHackingToolList( kPacket_ );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2010. 02. 17  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY

IMPL_ON_FUNC( EPM_OPEN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_OpenPartyList( kPacket_.m_kPartyInfo, kPacket_.m_vecUserInfo );
}

IMPL_ON_FUNC( EPM_CLOSE_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_ClosePartyList( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( EPM_ADD_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_AddPartyList( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( EPM_DEL_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_DelPartyList( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_UpdatePartyInfo( kPacket_.m_kPartyInfo );
}

//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_UpdatePartyFever( kPacket_.m_iPartyUID, kPacket_.m_sPartyFever );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_AddPartyMemberInPartyList( kPacket_.m_iPartyUID, kPacket_.m_kPartyUserInfo );
}

IMPL_ON_FUNC( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_DelPartyMemberInPartyList( kPacket_.m_iPartyUID, kPacket_.m_iUnitUID );
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	// ���� ��Ż ó��
	std::vector< UidType >::const_iterator vit;
	for( vit = kPacket_.m_vecDeletePartyUser.begin(); vit != kPacket_.m_vecDeletePartyUser.end(); ++vit )
	{
		// ���� ó��
		SiKPartyListManager()->E_DelPartyMemberInPartyList( kPacket_.m_iPartyUID, *vit );
	}

	// ������Ʈ
    SiKPartyListManager()->E_UpdatePartyMemberInfo( kPacket_.m_iPartyUID, kPacket_.m_vecUpdatePartyUser );
}

#endif SERV_CHANNEL_PARTY
//}}

//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT

IMPL_ON_FUNC( DBE_INCREASE_WEB_POINT_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// �� ����Ʈ ���� ����!
		return;
	}

	// �� ����Ʈ ���� ���������� �츮DB���� �α׸� ������!
	SendToGameDB( DBE_INCREASE_WEB_POINT_LOG_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, KDBE_GET_WEB_POINT_REWARD_ACK )
{
	std::vector< KWebPointRewardInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecWebPointReward.begin(); vit != kPacket_.m_vecWebPointReward.end(); ++vit )
	{
		if( vit->m_iUnitUID <= 0 )
		{
			START_LOG( cerr, L"Web Point ������ �Ϸ��µ� UnitUID���� 0�̴�." )
				<< BUILD_LOG( vit->m_iApplySN )
				<< BUILD_LOG( vit->m_wstrUserID )
				<< BUILD_LOG( vit->m_wstrNickName )
				<< END_LOG;
			continue;
		}

		// �������� ���� ����		
		CStringW cwstrItemID;
		cwstrItemID.Format( L"%d", vit->m_iRewardItemID );

		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = vit->m_iUnitUID;
		kPacketToDB.m_iToUnitUID   = vit->m_iUnitUID;
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_WEB_POINT_EVENT;
		kPacketToDB.m_iRewardID	   = vit->m_iRewardItemID;
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_wstrMessage  = cwstrItemID.GetBuffer();
		kPacketToDB.m_bGameServerEvent = true;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}    
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
IMPL_ON_FUNC( ERM_PERSONAL_SHOP_INFO_NOT )
{
	switch( kPacket_.m_iCode )
	{
		// ������ �߰�
	case NetError::ERR_PERSONAL_SHOP_26:
		{
			SiKPersonalShopItemList()->AddItemInfo(kPacket_);
		}
		break;
		// ������ ����
	case NetError::ERR_PERSONAL_SHOP_27:
		{
			SiKPersonalShopItemList()->ChangeItemInfo(kPacket_);
		}
		break;
		// ������ ����
	case NetError::ERR_PERSONAL_SHOP_28:
		{
			SiKPersonalShopItemList()->DelItemInfo(kPacket_);
		}
		break;

	default:
		START_LOG( cerr, L"���λ��� ������ ����Ʈ �����ڵ� ����." )
			<< BUILD_LOG( kPacket_.m_iCode )
			<< END_LOG;
		break;
	}
}
#endif DEF_TRADE_BOARD

//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

_IMPL_ON_FUNC( ESR_ORDER_TO_GAME_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// �α��μ����� ���� ������� Ȯ��!
	if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SENTINEL < kPacket_.m_iOrderType
		&& KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SENTINEL > kPacket_.m_iOrderType
		)
	{
		if( SEnum::CheckFlag( GetServerRollType(), KServerInfo::SRT_LOGIN_SERVER_SCRIPT_REFRESH ) == true )
		{
			UidType anTrace[2] = { GetUID(), -1 };
			KncSend( PI_GS_SERVER, GetUID(), PI_LOGIN_SERVER, 0, anTrace, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		}
	}
	// ���Ӽ����� ���� ������� Ȯ��!
	else if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SENTINEL < kPacket_.m_iOrderType  && 
			 KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SENTINEL > kPacket_.m_iOrderType )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		SiKGSScriptRefreshManager()->QueueingEvent( spEvent );
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
	std::map< int, UidType > mapFieldInfo;

	// MapData�� ���ŵǴ� ����� ���̺��� ���� �Ǵ� ���
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MAP_DATA )
	{
        mapFieldInfo = SiCXSLMapData()->GetFieldInfo();
	}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	std::map< int, std::vector< KRewardData > > mapRewardScriptData;
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_REWARD_TABLE )
	{
		mapRewardScriptData = SiKRewardTable()->GetMapRewardScriptData();
	}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	switch( kPacket_.m_iOrderType )
	{
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ITEM_MANAGER, CXSLItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RANDOM_ITEM_MANAGER, CXSLRandomItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_MANUFACTURE_ITEM_MANAGER, CXSLManufactureItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_REWARD_TABLE, KRewardTable );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_QUEST_MANAGER, CXSLQuestManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_TITLE_MANAGER, CXSLTitleManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_DUNGEON_MANAGER, CXSLDungeonManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_MAP_DATA, CXSLMapData );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_GUILD_MANAGER, CXSLGuildManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_GAME_SYS_VAL, KGameSysVal );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RESOLVE_ITEM_MANAGER, CXSLResolveItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ENCHANT_ITEM_MANAGER, CXSLEnchantItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SOCKET_ITEM, CXSLSocketItem );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ATTRIB_ENCHANT_ITEM, CXSLAttribEnchantItem );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RECOMMEND_USER_TABLE, KRecommendUserTable );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_EXP_TABLE, KExpTable );
	//{{ 2010. 9. 11	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_PET_MANAGER, CXSLPetManager );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_MORNITORING_MANAGER, KMornitoringManager );
#endif SERV_MORNITORING
	//}}
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ABUSER_LOG_MANAGER, KAbuserLogManager );
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_GAME_EVENT_SCRIPT_MANAGER, KGameEventScriptManager );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_PVP_MATCH_MANAGER, KPvpMatchManager );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_CONTENT_MANAGER, KGSContentManager );
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2011.10.14     ��μ�    ��� ����� Ư�� IP ������ ��� ����
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_COMPANY_IP_TABLE, KCompanyIPTable );
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_CLASS_CHANGE_TABLE, KClassChangeTable );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_STRING_FILTER, CXSLStringFilter );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 06. 06	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_DAILY_GIFT_BOX_MANAGER, KDailyGiftBoxManager );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 10. 19	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_PARTY_MANAGER, CXSLPartyManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_BINGO_EVENT_MANAGER, KGSBingoEventInfo );
#endif SERV_EVENT_BINGO
	//}}

#ifdef SERV_SYNTHESIS_AVATAR
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SYNTHESIS_TABLE, CXSLSynthesisManager );
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_DEFENCE_DUNGEON_MANAGER, CXSLDefenceDungeonManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_BUFF_MANAGER, CXSLBuffManager );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RIDING_PET_MANAGER, CXSLRidingPetManager );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SKILL_TREE, CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ED_INVENTORY_EXPAND, CXSLEDInventoryExpand );
#endif
	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}

	// MapData�� ���ŵǴ� ����� ���̺��� ���� �Ǵ� ���
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MAP_DATA )
	{
		BOOST_MAP_CONST_FOREACH( int, UidType, kFieldInfo, mapFieldInfo )
		{
			SiCXSLMapData()->UpdateFieldInfo( kFieldInfo.first, kFieldInfo.second );
		}

		SiCXSLMapData()->DumpToLogFile();
	}
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GAME_EVENT_SCRIPT_MANAGER )
	{
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const std::map< int, EVENT_DATA >	mapEventScriptData = SiKGameEventScriptManager()->GetMapEventScriptData();
		const std::map< int, EVENT_DATA >	mapEventDBData = SiKGameEventDBManager()->GetMapEventDBData();
		
		SiKGameEventManager()->SetTotalEventData( mapEventScriptData, mapEventDBData );

#endif //SERV_EVENT_DB_CONTROL_SYSTEM
		SiKGameEventManager()->RefreshEventScript();
	}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_CONTENT_MANAGER )
	{
		// ĳ���� ������ ���� ������ Ŭ���̾�Ʈ�� �˸���.
		KEGS_CASH_SHOP_OPEN_NOT kNot;
		kNot.m_bCashShopOpen = SiKGSContentManager()->IsEnableCashShop();

		{
			KEvent kEvent;
			kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_SHOP_OPEN_NOT, kNot );
			KActorManager::GetKObj()->SendToAll( kEvent );
		}
		
		{
			// Ŭ���̾�Ʈ�� ��ǰ ������ �ٽ� �� ������� �˸���.
			KEvent kEvent;
			kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
			KActorManager::GetKObj()->SendToAll( kEvent );
		}

		START_LOG( cout, L"[������ ������] ĳ���� ������ �¿��� ������ Ŭ���̾�Ʈ�� ��ε�ĳ���� �Ͽ����ϴ�!" )
			<< BUILD_LOG( kNot.m_bCashShopOpen );
	}
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2011. 11. 04	��μ� ���� ���� ����Ʈ �ǽð� ��ġ 
#ifdef SERV_RANDOM_DAY_QUEST
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_QUEST_MANAGER )
	{
		// Random Quest �� �޾� ����
		SendToGlobalServer( EGB_TODAY_RANDOM_QUEST_REQ, KBaseServer::GetKObj()->GetUID() );
	}
#endif SERV_RANDOM_DAY_QUEST
		//}}
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_REWARD_TABLE )
	{
		const std::map< int, std::vector< KRewardData > >	mapRewardDBData = SiKRewardTable()->GetMapRewardDBData();

		SiKRewardTable()->SetMapTotalRewardData(mapRewardScriptData, mapRewardDBData);

		KESG_REWARD_DB_DATA_NOT kPacket;

		kPacket.m_mapTotalRewardData = SiKRewardTable()->GetMapTotalRewardData();
		// ��� �����鿡�� ��ε�ĳ����
		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ESG_REWARD_DB_DATA_NOT, kPacket );
	}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GameServer ��ũ��Ʈ �ǽð� ��ġ �Ϸ�"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

#endif SERV_REALTIME_SCRIPT
//}}

#ifdef SERV_KOG_STATISTICS
IMPL_ON_FUNC( DBE_CCU_AGENT_ACK )
{
	if( NetError::NET_OK == kPacket_.m_iOK )
	{
		SiKOGAgent()->DailyInfobyUnitandUser(GetServerGroupID(), SiKChannelManager()->GetChannelID(), kPacket_.m_iDailyUniqueUser, kPacket_.m_iDailyResisterredUser );
	}
	else
	{
		START_LOG(cerr, L"KOG STATISTICS - DAILY INFO SEND FAIL.!")
			<< BUILD_LOG(NetError::GetErrStr(kPacket_.m_iOK))
			<< END_LOG;
	}
}
#endif SERV_KOG_STATISTICS

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
IMPL_ON_FUNC( E_DISCONNECT_SERVER_REPORT_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KMornitoringManager::ET_DISCONNECT_GAME_LOGIN_PROXY:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_LOGIN_PROXY );

			// �α��� �������� ������ ������ �����̹Ƿ� �α��μ����� ���� ���� �ʴ´�!
		}
		break;

	case KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY );

			// �α��� ������ �����ϱ�
			SendToLoginServer( ELG_DISCONNECT_SERVER_REPORT_NOT, kPacket_ );
		}
		break;

	case KMornitoringManager::ET_DISCONNECT_GAME_BILLING:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_BILLING );

			// �α��� ������ �����ϱ�
			SendToLoginServer( ELG_DISCONNECT_SERVER_REPORT_NOT, kPacket_ );
		}
		break;

		//{{ 2012. 01. 27  ��μ�	�α��� �������� ���� ���� �ڵ� sms ����
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
	case KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH );

			// �α��� ������ �����ϱ�
			SendToLoginServer( ELG_DISCONNECT_SERVER_REPORT_NOT, kPacket_ );
		}
		break;

#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
		//}}

	default:
		START_LOG( cerr, L"�߸��� Ÿ���Դϴ�!" )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< END_LOG;
		return;
	}
}

_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT )
{
	SiKMornitoringManager()->ReserveSMSMessage( kPacket_ );
}

#endif SERV_MORNITORING
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
_IMPL_ON_FUNC( EGB_UPDATE_MISSION_INFO_NOT, KEGB_GET_MISSION_INFO_ACK )
{
	SiKGSWorldMissionManager()->UpdateMissionInfo( kPacket_ );
}
IMPL_ON_FUNC( EGB_GET_MISSION_INFO_ACK )
{	
	SiKGSWorldMissionManager()->UpdateMissionInfo( kPacket_ );
}

IMPL_ON_FUNC( EGB_UPDATE_WORLD_BUFF_NOT )
{	
	if( SiKGSWorldMissionManager()->UpdateWorldBuffInfo( kPacket_ ) == true )
	{
		KEGS_UPDATE_WORLD_BUFF_NOT kNot;
		
		SiKGSWorldMissionManager()->GetWorldBuff( kNot );

		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, EGB_UPDATE_WORLD_BUFF_NOT, kNot );
	}
}

IMPL_ON_FUNC( EGB_UPDATE_MISSION_START_TIME_NOT )
{
	SiKGSWorldMissionManager()->UpdateMissionStartTime( kPacket_.m_wsrtStartTime );

	if( kPacket_.m_wsrtStartTime.empty() == true)
	{
		START_LOG( cerr, L"���� ���� �ð��� ����ٰ� �Ѵ�." )
			<< BUILD_LOG( kPacket_.m_wsrtStartTime )
			<< END_LOG;
	}	
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
void KGameServer::RequestRandomQuestInfo()
{
	SendToGlobalServer( EGB_TODAY_RANDOM_QUEST_REQ, KBaseServer::GetKObj()->GetUID() );
}

IMPL_ON_FUNC( EGB_TODAY_RANDOM_QUEST_ACK )
{
	SiCXSLQuestManager()->SetRandomQuestGroupList( kPacket_.m_mapTodayRandomQuest );

	// ������ �ִ� ��� �������� �˷��ش�
	KEGS_CHANGE_RANDOM_QUEST_NOT kNot;
	kNot.m_mapRandomQuestList = kPacket_.m_mapTodayRandomQuest;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_SERVER, NULL, EGS_CHANGE_RANDOM_QUEST_NOT, kNot );
	KActorManager::GetKObj()->SendToAll( *spEvent );
}

#endif SERV_RANDOM_DAY_QUEST
//}}


//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
int KGameServer::GetMaxUser() const
{
	// ä�θŴ����� ����� �ִ��ο��� ���� ã�´�.
	int iChannelMaxUser = SiKChannelManager()->GetMaxUser();
	if( iChannelMaxUser == 0 )
	{
        iChannelMaxUser = GetDefaultMaxUser();
	}

	return iChannelMaxUser;
}
#endif SERV_REALTIME_MAX_USER
//}}

//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
_IMPL_ON_FUNC( EGB_SHUT_DOWN_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT )
{
	SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket_ );
	
	START_LOG( cout, L"[ALL SERVERS {SHUT_DOWN} NOTIFY MESSAGE]" )
		<< L" ----------------------------- "		<< dbg::endl
		<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
		<< L" ----------------------------- "		<< dbg::endl;

}
#endif SERV_SHUTDOWN_SYSTEM
//}}

//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
IMPL_ON_FUNC( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT )
{
	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	SiKGSWorldMissionManager()->UpdateIndividualBuffInfo( kPacket_ );
#else
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, 0, PI_GS_USER, 0 , anTrace, EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT, kPacket_ );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT )
{
	GetKGSSimLayer()->SetUDPKickOff(kPacket_.m_bUDPSetKick);

	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ECN_UDP_KICK_GAMEEDIT_NOT, kPacket_ );
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
IMPL_ON_FUNC_NOPARAM( EGB_EARTH_QUAKE_NOT )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_SERVER, NULL, EGS_EARTH_QUAKE_NOT );
	KActorManager::GetKObj()->SendToAll( *spEvent );
}
#endif SERV_EARTH_QUAKE_EFFECT
//}}

//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( ERM_BATTLE_FIELD_ROOM_INFO_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE:
		{
			// ����� ��Ʋ�ʵ� ������ �ִ´�!
			SiKBattleFieldListManager()->UpdateBattleFieldRoomInfo( kPacket_.m_kBFRoomInfo );
		}
		break;

	case KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_DELETE:
		{
			// ����� ��Ʋ�ʵ� ������ �ִ´�!
			SiKBattleFieldListManager()->DeleteBattleFieldRoomInfo( kPacket_.m_kBFRoomInfo );
		}
		break;

	default:
		{
			START_LOG( cerr, L"��Ʋ�ʵ� �� ���� ������Ʈ" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC( EGB_GET_AUTO_PARTY_BONUS_INFO_ACK )
{
	BOOST_TEST_FOREACH( const KAutoPartyBonusInfo&, kInfo, kPacket_.m_vecAutoPartyBonusInfo )
	{
        SiKGSAutoPartyManager()->UpdateAutoPartyBonusInfo( kInfo.m_cDungeonMode, kInfo.m_iDungeonIDWithDif, kInfo.m_bActivatedBonus );
	}
}

IMPL_ON_FUNC( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT )
{
	SiKGSAutoPartyManager()->UpdateAutoPartyBonusInfo( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonIDWithDif, kPacket_.m_bActivatedBonus );
}

IMPL_ON_FUNC( ERM_END_GAME_REGROUP_PARTY_NOT )
{    
	// ��Ƽ ����� ��û
	SiKPartyListManager()->E_EndGamePartyRegroup( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( ERM_COMPLETE_RETURN_TO_FIELD_NOT )
{
    KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_ROOM, kPacket_.m_kReturnToFieldInfo.m_iRoomUID, NULL, ERM_NEXT_RETURN_TO_FIELD_NOT, kPacket_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 10. 16	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
IMPL_ON_FUNC( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK )
{
    SiKCorruptionMonitor()->Update( kPacket_ );
}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}

//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
_IMPL_ON_FUNC( EGB_JACKPOT_EVENT_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT )
{
	KncSend( PI_GS_SERVER, GetUID(), PI_CN_SERVER, 0, NULL, ECN_NOTIFY_MSG_NOT, kPacket_ );

	START_LOG( clog, L"[NOTIFY MESSAGE] => SEND CENTER SERVER " )
		<< L" ----------------------------- "	<< dbg::endl
		<< L"MSG : " << kPacket_.m_wstrMSG	    << dbg::endl
		<< L" ----------------------------- "	<< dbg::endl;
}

_IMPL_ON_FUNC( EGB_JACKPOT_EVENT_POST_NOT, KDBE_INSERT_REWARD_TO_POST_REQ )
{
	// �̺�Ʈ ������ ����!
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacket_ );
}
#endif SERV_EVENT_JACKPOT
//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
IMPL_ON_FUNC( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapTimeControlItem_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_ITEM_CHECK);

	if ( cmit !=  kPacket_.m_mapTimeControlItem_StaticDBReleaseTick.end() )
	{
		if ( GetTimeControlItemReleaseTick( TCIRTT_TIME_CONTROL_ITEM_CHECK )  != cmit->second )
		{

			if ( kPacket_.m_mapGetItemOnOff.empty() == false )
			{
				GetKGSSimLayer()->SetTimeControlItem_Info(kPacket_.m_mapGetItemOnOff);

				KEGS_GET_TIME_CONTROL_ITME_LIST_NOT kPacket;
				kPacket.m_iOK = NetError::NET_OK;

				UidType anTrace[2] = { 0, -1 };
				KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, DBE_GET_TIME_CONTROL_ITME_LIST_NOT, kPacket );
			}
			
			SetTimeControlItemReleaseTick( TCIRTT_TIME_CONTROL_ITEM_CHECK, kPacket_.m_mapTimeControlItem_StaticDBReleaseTick );
		}
	}
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
IMPL_ON_FUNC( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING);

	if ( cmit !=  kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end() )
	{
		if ( GetTimeControlCubeInItemMappingReleaseTick( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING )  != cmit->second )
		{

			//if ( kPacket_.m_mapGetCubeInItemMappingOnOff.empty() == false )
			//{
				GetKGSSimLayer()->SetTimeControlCubeInItemMapping_Info(kPacket_.m_mapGetCubeInItemMappingOnOff);

				//KEGS_GET_TIME_CONTROL_ITME_LIST_NOT kPacket;
				//kPacket.m_iOK = NetError::NET_OK;

				//UidType anTrace[2] = { 0, -1 };
				//KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, DBE_GET_TIME_CONTROL_ITME_LIST_NOT, kPacket );
			//}
			
			SetTimeControlCubeInItemMappingReleaseTick( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING, kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick );
		}
	}
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

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

IMPL_ON_FUNC( DBE_EVENT_DB_SCRIPT_ACK )
{
	if ( kPacket_.m_mapDBRewardData.size() > 0 )
	{
		const std::map< int, std::vector< KRewardData > >	mapRewardScriptData = SiKRewardTable()->GetMapRewardScriptData();
		SiKRewardTable()->SetMapRewardDBData(kPacket_.m_mapDBRewardData);
		const std::map< int, std::vector< KRewardData > >	mapRewardDBData = SiKRewardTable()->GetMapRewardDBData();

		SiKRewardTable()->SetMapTotalRewardData(mapRewardScriptData, mapRewardDBData);

		KESG_REWARD_DB_DATA_NOT kPacket;

		kPacket.m_mapTotalRewardData = SiKRewardTable()->GetMapTotalRewardData();
		// ��� �����鿡�� ��ε�ĳ����
		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ESG_REWARD_DB_DATA_NOT, kPacket );
	}

	if( kPacket_.m_mapEventDBData.size() > 0 )
	{
		SiKGameEventDBManager()->SetEventDBData(kPacket_.m_mapEventDBData);

		const std::map< int, EVENT_DATA >	mapEventScriptData = SiKGameEventScriptManager()->GetMapEventScriptData();
		const std::map< int, EVENT_DATA >	mapEventDBData = SiKGameEventDBManager()->GetMapEventDBData();

		SiKGameEventManager()->SetTotalEventData( mapEventScriptData, mapEventDBData );

		if ( SiKGameEventManager()->CheckMapIngEventDataEmpty() == true)
		{
#ifdef SERV_ADD_EVENT_DB
			SendToEventDB( DBE_EVENT_UPDATE_REQ );
#else //SERV_ADD_EVENT_DB
			SendToLogDB( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
		}
		else
		{
			SiKGameEventManager()->RefreshEventScript();
		}
	}
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

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
IMPL_ON_FUNC( ECN_SET_ACTIVE_LAGCHECK_NOT )
{
	m_bActiveLagCheck = kPacket_.m_bActiveLagCheck;
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
_IMPL_ON_FUNC( ELG_WEDDING_HALL_INFO_UPDATE_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	std::map<int, KWeddingHallInfo>::iterator mit = kPacket_.m_mapWeddingInfo.begin();
	for(  ; mit != kPacket_.m_mapWeddingInfo.end() ; ++mit )
	{
		SiKWeddingHallListManager()->AddWeddingHallInfo( mit->second );
	}
}

IMPL_ON_FUNC( ERM_WEDDING_HALL_INFO_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KERM_WEDDING_HALL_INFO_NOT::IT_INSERT:
		{
			// ���� ������ ��ȥ���� ����
			SiKWeddingHallListManager()->AddWeddingHallInfo( kPacket_.m_kHallInfo );
		}
		break;
	case KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE:
		{
			// ���� ������ ��ȥ���� ����
			SiKWeddingHallListManager()->UpdateWeddingHallInfo( kPacket_.m_kHallInfo, kPacket_.m_vecJoinUnitList );
		}
		break;
	case KERM_WEDDING_HALL_INFO_NOT::IT_DELETE:
		{
			// �Ҹ�� ��ȥ���� ����
			SiKWeddingHallListManager()->DeleteWeddingHallInfo( kPacket_.m_kHallInfo.m_iWeddingUID );
		}
		break;

	default:
		{
			START_LOG( cerr, L"��ȥ���� ���� ������Ʈ" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
bool KGameServer::PopFirstTimeEnchantEvent( void )
{
	if( m_iNumOfTimeEnchantEvent <= 0 )
	{
		return false;
	}

	--m_iNumOfTimeEnchantEvent;
	return true;
}

IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_INFO_NOT )
{
	m_iTimeEnchantEventUID		= kPacket_.m_iTimeEnchantEventUID;
	m_iNumOfTimeEnchantEvent	= kPacket_.m_iNumOfTimeEnchantEvent;
}

IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_INFO_ACK )
{
	m_iTimeEnchantEventUID		= kPacket_.m_iTimeEnchantEventUID;
	m_iNumOfTimeEnchantEvent	= kPacket_.m_iNumOfTimeEnchantEvent;
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
int	KGameServer::DecreaseExchangeLimit( IN const int iDestItemID )
{
	if( m_iExchangeLimitUID == 0 )
	{
		return ELR_NOT_INITED;
	}

	std::map< int, int >::iterator it = m_mapExchangeLimitInfo.find( iDestItemID );
	if( it == m_mapExchangeLimitInfo.end() )
	{
		// ��� ��ü�� ���ٸ�, ���� ����� �ƴϴ�.
		return ELR_PASS;
	}

	if( it->second <= 0 )
	{
		// ��Ͽ� ������, ������ �����Ǿ���.
		return ELR_EXHAUSTED;
	}

	// ������ ���� ��Ű��, üũ�Ϸ� ����.
	--it->second;

	return ELR_CHECK;
}

IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_NOT )
{
	m_iExchangeLimitUID		= kPacket_.m_iExchangeLimitUID;
	m_mapExchangeLimitInfo	= kPacket_.m_mapExchangeLimitInfo;
}

IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_ACK )
{
	m_iExchangeLimitUID		= kPacket_.m_iExchangeLimitUID;
	m_mapExchangeLimitInfo	= kPacket_.m_mapExchangeLimitInfo;
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
IMPL_ON_FUNC( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if (SiKGSContentManager()->GetReleaseTick() != kPacket_.m_iReleaseTick )
		{
			SiKGSContentManager()->SetReleaseTick( kPacket_.m_iReleaseTick );
			SiKGSContentManager()->SetEnableCashShop( kPacket_.m_bEnableCashshop );

			// ĳ���� ������ ���� ������ Ŭ���̾�Ʈ�� �˸���.
			KEGS_CASH_SHOP_OPEN_NOT kNot;
			kNot.m_bCashShopOpen = kPacket_.m_bEnableCashshop;

			{
				KEvent kEvent;
				kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_SHOP_OPEN_NOT, kNot );
				KActorManager::GetKObj()->SendToAll( kEvent );
			}

			if ( kNot.m_bCashShopOpen == true)
			{
				// Ŭ���̾�Ʈ�� ��ǰ ������ �ٽ� �� ������� �˸���.
				KEvent kEvent;
				kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
				KActorManager::GetKObj()->SendToAll( kEvent );
			}

			START_LOG( cout, L"[������ ������] ĳ���� ������ �¿��� ������ Ŭ���̾�Ʈ�� ��ε�ĳ���� �Ͽ����ϴ�!" )
				<< BUILD_LOG( kNot.m_bCashShopOpen );
		}
	}	
}
#endif SERV_CONTENT_MANAGER_INT

//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
_IMPL_ON_FUNC( EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, KEGB_GET_GLOBAL_MISSION_INFO_ACK )	// ���� ������ GlobalMissionInfo ������ ��ε� ĳ����
{
	SiKGSGlobalMissionManager()->UpdateGlobalMissionInfo( kPacket_ );
}

IMPL_ON_FUNC( EGB_GET_GLOBAL_MISSION_INFO_ACK )
{	
	SiKGSGlobalMissionManager()->UpdateGlobalMissionInfo( kPacket_ );
}

IMPL_ON_FUNC( EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT )
{
	//SiKGSGlobalMissionManager()->UpdateGlobalMissionInfo( kPacket_.m_wstrStartTime );
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-29	// �ڼ���
void KGameServer::CheckGSFieldBossSystemRequest( IN const CTime tCurrentTime )
{
	const __time64_t tReservedTimeForInitRequest = SiKGSFieldBossManager()->GetReservedTimeForInitRequest();
	
	if( tReservedTimeForInitRequest == 0 )
		return;

	if( tCurrentTime.GetTime() < tReservedTimeForInitRequest )
		return;

	// ���и� ����Ͽ� 1�� �ڿ� �� �õ��ϵ��� ��������
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( tCurrentTime.GetTime() + 60 );
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GLOBAL_USER, 0, NULL, EGB_BATTLE_FIELD_BOSS_INFO_NOT, char() );
}

void KGameServer::CheckGSFieldBossSystem( IN const CTime tCurrentTime )
{
	if( SiKGSFieldBossManager()->CheckTerm( tCurrentTime.GetTime() ) == false )
		return;

	// �۷ι� ������ ����Ǿ� �ִٸ�, ��ü������ ó���� �ʿ� ����.
	if( SiKGSFieldBossManager()->IsConnected() == true )
		return;

	const bool bPortalOpen	= SiKGSFieldBossManager()->IsPortalOpen();
	const bool bCoolTime	= SiKGSFieldBossManager()->IsCoolTime();

	const __time64_t tPortalAppearanceTime = SiKGSFieldBossManager()->GetPortalAppearanceTime();

	// ��Ż�� ���� �ִ��� Ȯ��
	if( bPortalOpen == true )
	{
		const __time64_t tPortalHoldingTime = tPortalAppearanceTime + SiKGSFieldBossManager()->GetRemainPortalTime();
		if( tPortalHoldingTime <= tCurrentTime.GetTime() )
		{
			// ��Ż ���� ó��
			SiKGSFieldBossManager()->ClosePortal();

			// ��� �����鿡�� ��ε�ĳ����
			KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT kPacket;
			kPacket.m_iBattleFieldID	= SiKGSFieldBossManager()->GetPortalAppearanceMap();

			UidType anTrace[2] = { 0, -1 };
			KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT, kPacket );
		}
	}

	// �� Ÿ�� üũ
	if( bCoolTime == true )
	{
		const __time64_t tFieldBossCoolTime = tPortalAppearanceTime + SiKGSFieldBossManager()->GetRemainCoolTime();
		if( tFieldBossCoolTime <= tCurrentTime.GetTime() )
		{
			SiKGSFieldBossManager()->SetCoolTimeState( false );
		}
	}
}

IMPL_ON_FUNC( ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT )
{
	SendToGlobalServer( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGB_VERIFY_SERVER_CONNECT_NOT )
{
	// �ٷ� ��û�� �������� ����
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( 1 );
	SiKGSFieldBossManager()->SetConnected( true );
}

IMPL_ON_FUNC_NOPARAM( EGB_VERIFY_SERVER_DISCONNECT_NOT )
{
	// ������ ������������ ���� ��û ��Ŷ�� ������ ����
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( 0 );
	SiKGSFieldBossManager()->SetConnected( false );
}

IMPL_ON_FUNC( EGB_BATTLE_FIELD_BOSS_INFO_NOT )
{
	// �ʱ�ȭ ������ ������� ����ؼ� ���� ��û ��Ŷ�� ���� �ʿ䰡 ����.
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( 0 );

	const bool bPortalOpen	= SiKGSFieldBossManager()->IsPortalOpen();

	SiKGSFieldBossManager()->SetInfo( CTime::GetCurrentTime().GetTime()
									, kPacket_.m_tRemainPortalTime
									, kPacket_.m_tRemainCoolTime
									, kPacket_.m_iPortalAppearanceMap
									, kPacket_.m_iPortalDestination
									, kPacket_.m_bPortalOpen
									, kPacket_.m_bCoolTime
									);

	// ��Ż Open/Close�� ��ȭ�� ���ٸ� ��ƾ ����
	if( bPortalOpen == kPacket_.m_bPortalOpen )
		return;

	// BroadCasting
	KEvent kEvent;

	if( kPacket_.m_bPortalOpen == true )
	{
		KEGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT kPacket;
		kPacket.m_iBattleFieldID	= kPacket_.m_iPortalAppearanceMap;
		kPacket.m_iBossFieldID		= kPacket_.m_iPortalDestination;
		
		// ��� �����鿡�� ��ε�ĳ����
		kEvent.SetData( PI_GS_PROXY, NULL, EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT, kPacket );
	}
	else
	{
		KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT kPacket;
		kPacket.m_iBattleFieldID	= kPacket_.m_iPortalAppearanceMap;

		// ��� �����鿡�� ��ε�ĳ����
		kEvent.SetData( PI_GS_PROXY, NULL, EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT, kPacket );
	}

	KActorManager::GetKObj()->SendToAll( kEvent );
}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
_IMPL_ON_FUNC( EGS_ADD_COMMON_FLAG_NOT, KEGS_ADD_COMMON_FLAG_NOT )
{
	START_LOG( cout, L"EGS_ADD_COMMON_FLAG_NOT Recieve UID: " << GetUID() );
	KSimLayer::GetKObj()->AddCommonFlag( kPacket_.dwFlag );	
}
_IMPL_ON_FUNC( EGS_DEL_COMMON_FLAG_NOT, KEGS_DEL_COMMON_FLAG_NOT )
{
	START_LOG( cout, L"EGS_DEL_COMMON_FLAG_NOT Recieve UID: " << GetUID() );
	KSimLayer::GetKObj()->DeleteCommonFlag( kPacket_.dwFlag );	
}
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH


#ifdef SERV_STRING_FILTER_USING_DB
IMPL_ON_FUNC( DBE_CHECK_STRING_FILTER_UPDATE_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapReleaseTick.find( KStringFilterManager::iReleaseTickType );

	START_LOG( clog, L"��Ʈ�� ���� �޾ƿԳ�" )
		<< BUILD_LOG( SiKStringFilterManager()->GetReleaseTick() )
		<< BUILD_LOG( kPacket_.m_mapReleaseTick.size() )
		<< END_LOG;

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKStringFilterManager()->GetReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L"��Ʈ�� ���� �ٲ� ���� Ȯ���Ͽ����Ƿ� ���� �޾ƿɴϴ�." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

			SendToScriptDB( DBE_STRING_FILTER_UPDATE_REQ );

			SiKStringFilterManager()->SetReleaseTick( iNewReleaseTick );
		}
	}
}
IMPL_ON_FUNC( DBE_STRING_FILTER_UPDATE_ACK )
{
	START_LOG( clog, L"��Ʈ�� ���� ��� �޾ƿԴ�" )
		<< BUILD_LOG( kPacket_.m_vecStringFilterList.size() )
		<< END_LOG;

	if( kPacket_.m_vecStringFilterList.size() > 0 )
	{
		SiKStringFilterManager()->SetStringFilter( kPacket_.m_vecStringFilterList );
	}
}
#endif //SERV_STRING_FILTER_USING_DB
