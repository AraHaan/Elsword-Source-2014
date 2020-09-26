#include "GlobalSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "ActorManager.h"
#include "DBLayer.h"
#include "GlobalServer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "GlobalUser.h"

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "WorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
	#include "MatchMakingManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2012. 02. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLDungeonManager.h"
//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#else
	#include "AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	#include "X2Data/XSLQuestManager.h"
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2013. 05. 14	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	#include "BlockListManager.h"
#endif SERV_BLOCK_LIST
//}}
#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
	#include "GameSysVal/GameSysVal.h"
#endif // SERV_TIME_ENCHANT_EVENT

//{{ 2012. 09. 03	��ȫ�� �۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	��ȫ�� �۷ι� �̼� �Ŵ���

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"
	#include "X2Data/XSLBattleFieldManager.h"
#endif // SERV_BATTLE_FIELD_BOSS

#include "DBLayer.h"
#include "NetError.h"

#include <lua_tinker.h>
using namespace lua_tinker;

#include <boost/bind.hpp>

NiImplementRTTI( KGlobalSimLayer, KSimLayer );

KGlobalSimLayer::KGlobalSimLayer(void) 
{
}

KGlobalSimLayer::~KGlobalSimLayer(void)
{
}

ImplToStringW( KGlobalSimLayer )
{
    return START_TOSTRING_PARENTW( KSimLayer );
}

ImplOstmOperatorW2A( KGlobalSimLayer );

void KGlobalSimLayer::RegToLua()
{
    KSimLayer::RegToLua();

    //class_<KLoginSimLayer>( "KLoginSimLayer" )
    //    .inh<KSimLayer>()
    //    .def( "dump", KLoginSimLayer::Dump );

    //decl( "SimLayer", this );
	lua_tinker::class_add<KGlobalSimLayer>( g_pLua, "KGlobalSimLayer" );
	lua_tinker::class_inh<KGlobalSimLayer, KSimLayer>( g_pLua );
	lua_tinker::class_def<KGlobalSimLayer>( g_pLua, "dump", &KGlobalSimLayer::Dump );

	lua_tinker::decl( g_pLua, "SimLayer", this );
}

//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KGlobalSimLayer::GetGSUserInfoByUnitUID( IN const UidType iUnitUID, OUT KGlobalUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByUnitUID( iUnitUID, kInfo );
}

#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // ���볯¥: 2013-04-04
bool KGlobalSimLayer::GetGSUserInfoByUnitNickName( IN const UidType iServerGroupID, IN const std::wstring& wstrUnitNickName, OUT KGlobalUserList::KGSUserInfo& kInfo )
{
	return m_kUserList.GetGSUserInfoByUnitNickName( iServerGroupID, wstrUnitNickName, kInfo );
}
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
bool KGlobalSimLayer::GetGSUserInfoByUnitNickName( IN const std::wstring& wstrUnitNickName, OUT KGlobalUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByUnitNickName( wstrUnitNickName, kInfo );
}
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE

//{{ 2013. 05. 29	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
bool KGlobalSimLayer::GetGSUserInfoByUserUID( IN const UidType iUserUID, OUT KGlobalUserList::KGSUserInfo& kInfo )
{
	return m_kUserList.GetGSUserInfoByUserUID( iUserUID, kInfo );
}
#endif SERV_BLOCK_LIST
//}}

void KGlobalSimLayer::UnRegAllUserOfThisGS( IN const UidType iGSUID )
{
    m_kUserList.UnRegAllUserOfThisGS( iGSUID );
}

int KGlobalSimLayer::RegUnit( IN const KRegUnitInfo& kUnitInfo )
{
    return m_kUserList.RegUnit( kUnitInfo );
}

void KGlobalSimLayer::UnRegUnit( IN const UidType iUnitUID )
{
    m_kUserList.UnRegUnit( iUnitUID );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KGlobalSimLayer::Init()
{
    KSimLayer::Init();

	KAutoPath kAutoPath;
	std::string		strFile;

	{
		LoadingTimer lt( L"Enum.lua" );

		//���� �鿡�� �̷�� ���� ������ �ε��� �����Ұ�� ������ ���� ���Ѿ��Ѵ�.
		strFile = "Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != luaL_dofile( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"Enum ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );

			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"Enum.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Enum ���� �ε� ����.!" );
		}
	}

	{
		LoadingTimer lt( L"DungeonEnum.lua" );

		//���� �鿡�� �̷�� ���� ������ �ε��� �����Ұ�� ������ ���� ���Ѿ��Ѵ�.
		strFile = "DungeonEnum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != luaL_dofile( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"DungeonEnum ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );

			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
			KBaseServer::GetKObj()->AddFailScriptFileName( L"DungeonEnum.lua" );
			//}}
		}
		else
		{
			START_LOG( cout, L"DungeonEnum ���� �ε� ����.!" );
		}
	}

	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	{
		KMornitoringManager::RegScriptName( "ServerMornitoringConfig.lua" );
		OPEN_SCRIPT_FILE( KMornitoringManager );
	}
#endif SERV_MORNITORING
	//}}

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	{
		KWorldMissionManager::RegScriptName( "WorldMissionConfig.lua" );
		OPEN_SCRIPT_FILE( KWorldMissionManager );
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 07. 18	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	{
		KPvpMatchManager::RegScriptName( "PvpMatchData.lua" );
#ifdef SERV_UNITED_SERVER_EU
		std::string strPvpNpcData = GetStrPvpNpcDataLua();
		KPvpMatchManager::RegScriptName( strPvpNpcData.c_str() );
#else SERV_UNITED_SERVER_EU
		KPvpMatchManager::RegScriptName( "PvpNpcData.lua" );
#endif SERV_UNITED_SERVER_EU
		OPEN_SCRIPT_FILE( KPvpMatchManager );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	{
		CXSLDungeonManager::RegScriptName( "DungeonData.lua" );

		if( SiCXSLDungeonManager()->OpenScriptFile_AllDungeonScriptLoad( g_pLua ) == false )
			START_LOG( cerr, L"Dungeon Manager ���� �ε� ����.!" );
		else
			START_LOG( cout, L"Dungeon Manager ���� �ε� ����.!" );

		//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
		if( SiCXSLDungeonManager()->IsLuaPasingSuccess() == false )
		{
			KBaseServer::GetKObj()->AddFailScriptFileName( SiCXSLDungeonManager()->GetParsingLuaFileName().c_str() );
		}
#endif SERV_SCRIPT_PARSING_ERR_REPORT
		//}}
	}
	{
		KAutoPartyScriptManager::RegScriptName( "AutoPartyData.lua" );
		OPEN_SCRIPT_FILE( KAutoPartyScriptManager );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	{
		CXSLQuestManager::RegScriptName( "FieldQuest.lua" );
		OPEN_SCRIPT_FILE( CXSLQuestManager );
	}
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	{
		KSMSPhoneNumberManager::RegScriptName( "SMSPhoneNumberList.lua" );
		OPEN_SCRIPT_FILE( KSMSPhoneNumberManager );
	}
#endif SERV_SMS_TOTAL_MANAGER
	//}}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
	{
		KGameSysVal::RegScriptName( "GameSysValTable.lua" );
		OPEN_SCRIPT_FILE( KGameSysVal );
	}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	{
		CXSLFieldBossData::RegScriptName( "FieldBossData.lua" );
		OPEN_SCRIPT_FILE( CXSLFieldBossData );

		CXSLBattleFieldManager::RegScriptName( "BattleFieldData.lua" );
		if( SiCXSLBattleFieldManager()->OpenScriptFile_AllBattleFieldScriptLoad( g_pLua ) == false )
			START_LOG( cerr, L"Battle Field Manager ���� �ε� ����.!" );
		else
			START_LOG( cout, L"Battle Field Manager ���� �ε� ����.!" );

		//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
		if( SiCXSLBattleFieldManager()->IsLuaPasingSuccess() == false )
		{
			KBaseServer::GetKObj()->AddFailScriptFileName( SiCXSLBattleFieldManager()->GetParsingLuaFileName().c_str() );
		}
#endif SERV_SCRIPT_PARSING_ERR_REPORT
		//}}
	}
#endif // SERV_BATTLE_FIELD_BOSS
}

void KGlobalSimLayer::Tick()
{
    KSimLayer::Tick();

	//{{ 2010. 10. 12	������	���� ����͸�
//#ifdef SERV_MORNITORING
//	if( GetKGlobalServer()->IsConnectionMornitoringEnable() == true )
//	{
//		SiKMornitoringManager()->Tick_LoginSImLayer();
//	}
//#endif SERV_MORNITORING
	//}}

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	if( SiKWorldMissionManager()->GetWorldMissionState() == true )
	{
		SiKWorldMissionManager()->Tick();
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2013. 05. 14	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	SiKBlockListManager()->Tick();
#endif SERV_BLOCK_LIST
	//}}
	//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
	SiKGlobalMissionManager()->Tick();
#endif SERV_GLOBAL_MISSION_MANAGER
	//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
}

void KGlobalSimLayer::ShutDown()
{
    KSimLayer::ShutDown();
	
#ifdef SERV_MORNITORING	// ���� ������ �ؿ��� �߰�
	KMornitoringManager::ReleaseInstance();
#endif // SERV_MORNITORING
	KWorldMissionManager::ReleaseInstance();
	//{{ 2011. 07. 18	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	KPvpMatchManager::ReleaseInstance();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KAutoPartyScriptManager::ReleaseInstance();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 22	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	KSMSPhoneNumberManager::ReleaseInstance();
#endif SERV_SMS_TOTAL_MANAGER
	//}}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
	KGameSysVal::ReleaseInstance();
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_GLOBAL_MISSION_MANAGER
	KGlobalMissionManager::ReleaseInstance();
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	CXSLFieldBossData::ReleaseInstance();
	CXSLBattleFieldManager::ReleaseInstance();
#endif // SERV_BATTLE_FIELD_BOSS
}


//{{ ��ȫ�� [2012.05.16] ���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
std::string KGlobalSimLayer::GetStrPvpNpcDataLua()
{
	std::string strFile = "PvpNpcData.lua";
	switch(static_cast<int>(m_dwNationFlag))
	{
	case NF_DE:		strFile = "PvpNpcData_DE.lua";	break;
	case NF_FR:		strFile = "PvpNpcData_FR.lua";	break;
	case NF_IT:		strFile = "PvpNpcData_IT.lua";	break;
	case NF_PL:		strFile = "PvpNpcData_PL.lua";	break;
	case NF_ES:		strFile = "PvpNpcData_ES.lua";	break;
	case NF_TR:		strFile = "PvpNpcData_TR.lua";	break;
	case NF_EN:		strFile = "PvpNpcData_EN.lua";	break;
	default:
		strFile = "PvpNpcData.lua";
		break;
	}	

	return strFile;
}
#endif SERV_UNITED_SERVER_EU
//}}
