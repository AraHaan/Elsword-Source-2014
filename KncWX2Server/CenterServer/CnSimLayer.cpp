#include "CnSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "ActorManager.h"
#include "DBLayer.h"
#include "CnServer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "CnUser.h"

#include "DBLayer.h"
#include "NetError.h"
#include "CnRoomManager.h"
#include "KAutoPath.h"
#include "KDropTable.h"
#include "KPVPResultTable.h"
#include "ResultProcess.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLUnitManager.h"
#include "X2Data/XSLQuestManager.h"
#include "GameEvent/GameEventManager.h"
#include "GameSysVal/GameSysVal.h"
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2009. 7. 9  ������		��ϸ� �ð�
#include "HenirResultTable.h"
//}}
//{{ 2008. 10. 20  ������	�Ӽ�NPC
#include "KAttribNpcTable.h"
//}}
//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_MONSTER
//}}
//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_CN_SERVER_HACKING_CHECK
//}}
//{{ 2011. 01. 20	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	#include "RoomMonsterManager.h"
#endif SERV_DUNGEON_RANK_NEW
//}}
//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_ROOM_TICK_DUMP
//}}
//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatchResultTable.h"
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 11. 4	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 07. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	#include "BadAttitudeTable.h"
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}
//{{ 2013. 04. 12	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#include <lua_tinker.h>
using namespace lua_tinker;

#include <boost/bind.hpp>

NiImplementRTTI( KCnSimLayer, KSimLayer );

KCnSimLayer::KCnSimLayer(void) 
{
}

KCnSimLayer::~KCnSimLayer(void)
{
}

ImplToStringW( KCnSimLayer )
{
    return START_TOSTRING_PARENTW( KSimLayer );
}

//{{ 2009. 12. 15  ������	��������
void KCnSimLayer::DumpToLogFile()
{
	KSimLayer::DumpToLogFile();
}
//}}

ImplOstmOperatorW2A( KCnSimLayer );

void KCnSimLayer::RegToLua()
{
    KSimLayer::RegToLua();

    // 061127. jseop. ��Ŵ���.
    //SiKRoomManager()->RegToLua();
	// 081017. hoons. ��Ŵ���.
	GetKCnRoomManager()->RegToLua();

	lua_tinker::class_add<KCnSimLayer>( g_pLua, "KCnSimLayer" );
	lua_tinker::class_inh<KCnSimLayer, KSimLayer>( g_pLua );
	lua_tinker::class_def<KCnSimLayer>( g_pLua, "dump", &KCnSimLayer::Dump );
	//{{ 2009. 3. 24  ������	CnUser�����Ȳ ���� �׽�Ʈ �ڵ�
	//lua_tinker::class_def<KCnSimLayer>( g_pLua, "DieCnUser",		&KCnSimLayer::DisconnectAllCnUser_LUA );
	//}}

	lua_tinker::decl( g_pLua, "SimLayer", this );	
}

//////////////////////////////////////////////////////////////////////////
void KCnSimLayer::DisconnectAllCnUser_LUA()
{
	START_LOG( cout, L"��� CnUser ���� ����!" );

	KActorManager* pkActorManager = KActorManager::GetKObj();

	pkActorManager->ForEach( boost::bind( &KActor::ReserveDestroy, _1 ) );
}
//////////////////////////////////////////////////////////////////////////

void KCnSimLayer::Init()
{
    KSimLayer::Init();

	KAutoPath		kAutoPath;
	std::string		strFile;

	//���� ���Ϸε� ���н� �����ٿ��� ���Ѿ��Ѵ�.
    lua_State* L = g_pLua;

	{
		LoadingTimer lt(L"Enum.lua" );

		strFile = "Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( L, strFile.c_str() ) )
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
		LoadingTimer lt(L"DungeonEnum.lua" );

		strFile = "DungeonEnum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( L, strFile.c_str() ) )
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

	//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER
	{
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		SiKGameEventManager()->SetServerType( KGameEventManager::ST_CENTER );
		KGameEventScriptManager::RegScriptName( "EventData.lua" );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		KGameEventScriptManager::RegScriptName( "EventMonsterData.lua" );
#ifdef  SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		KGameEventScriptManager::RegScriptName( "ValenTineData.lua" );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		OPEN_SCRIPT_FILE( KGameEventScriptManager );
	}
#endif SERV_EVENT_MONSTER
	//}}

	{
		LoadingTimer lt(L"DLG_Map_Enum.lua" );

		strFile = "DLG_Map_Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( L, strFile.c_str() ) )
		{
			START_LOG( cerr, L"DLG_Map_Enum ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"DLG_Map_Enum.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"DLG_Map_Enum ���� �ε� ����.!" );
		}
	}

	{
#ifdef SERV_UNITED_SERVER_EU
		std::string strItemLuaName = GetStrItemLuaName();
		CXSLItemManager::RegScriptName( strItemLuaName.c_str() );
#else // SERV_UNITED_SERVER_EU
		CXSLItemManager::RegScriptName( "Item.lua" );
#endif // SERV_UNITED_SERVER_EU

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
#ifdef SERV_UNITED_SERVER_EU
		std::string strItemTransLuaName = GetStrItemTransLuaName();
		CXSLItemManager::RegScriptName( strItemTransLuaName.c_str() );
#else
		CXSLItemManager::RegScriptName( "ItemTrans.lua" );
#endif
#endif SERV_ITEM_LUA_TRANS_DEVIDE

		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		CXSLItemManager::RegScriptName( "WeddingItemData.lua" );
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		OPEN_SCRIPT_FILE( CXSLItemManager );
	}

	{
		KDropTable::RegScriptName( "DropTable.lua" );
#ifdef DUNGEON_ITEM
		KDropTable::RegScriptName( "PvPDropTable.lua" );
#endif DUNGEON_ITEM
		KDropTable::RegScriptName( "StaticDropTable.lua" );
		KDropTable::RegScriptName( "DropData.lua" );
		//{{ 2009. 12. 18  ������	��������ġ����
#ifdef DUNGEON_EXP_NEW
		KDropTable::RegScriptName( "NpcExpTable.lua" );
#endif DUNGEON_EXP_NEW
		//}}
		//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		KDropTable::RegScriptName( "PaymentItemTable.lua" );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
		//{{ 2011. 08. 23	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
		KDropTable::RegScriptName( "HenirSpecialNpcDrop.lua" );
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2011. 11. 23	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
		//KDropTable::RegScriptName( "BattleFieldNpcDropTable.lua" );
		//KDropTable::RegScriptName( "BattleFieldStaticDropTable.lua" );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		OPEN_SCRIPT_FILE( KDropTable );
	}	

	{
		CXSLDungeonManager::RegScriptName( "DungeonData.lua" );
		//{{ 2010. 03. 24  ������	��д��� ����
#ifdef SERV_SECRET_HELL
		CXSLDungeonManager::RegScriptName( "SecretDungeonData.lua" );
#endif SERV_SECRET_HELL
		//}}
		
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

	//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	{
		KPvpMatchResultTable::RegScriptName( "PvpMatchResultTable.lua" );
		OPEN_SCRIPT_FILE( KPvpMatchResultTable );
	}

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
#else
	{
		LoadingTimer lt(L"PVPResultTable.lua" );

		strFile = "PVPResultTable.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiKPVPResultTable()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"PVPResultTable ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"PVPResultTable.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"PVPResultTable ���� �ε� ����.!" );
		}
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	{
		//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
		KResultProcess::RegScriptName( "ResultData_new.lua" );
#else
		KResultProcess::RegScriptName( "ResultData.lua" );
#endif SERV_DUNGEON_RANK_NEW
		//}}
		OPEN_SCRIPT_FILE( KResultProcess );
	}

	{
		LoadingTimer lt( L"NPCTemplet.lua" );

		strFile = "NPCTemplet.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLUnitManager()->OpenNPCScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"NPC ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"NPCTemplet.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"NPC ���� �ε� ����.!" );
		}
	}

	{
		LoadingTimer lt( L"NPCStat.lua" );

		strFile = "NPCStat.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLUnitManager()->OpenNPCStatScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"NPC Stat ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"NPCStat.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"NPC Stat ���� �ε� ����.!" );
		}
	}

	{
		KGameSysVal::RegScriptName( "GameSysValTable.lua" );
		OPEN_SCRIPT_FILE( KGameSysVal );
	}
	
	//08.03.10. hoons : �������� ����Ʈ ����������� ��ӵǰ� �Ǿ� �߰���.
	{
		CXSLQuestManager::RegScriptName( "FieldQuest.lua" );
		CXSLQuestManager::RegScriptName( "FieldSubQuest.lua" );
		OPEN_SCRIPT_FILE( CXSLQuestManager );
	}

	{
		KHenirResultTable::RegScriptName( "HenirData.lua" );
		KHenirResultTable::RegScriptName( "HenirResultTable.lua" );
		//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
		KHenirResultTable::RegScriptName( "HenirChallangeReward.lua" );
#endif SERV_NEW_HENIR_TEST
		//}}
		OPEN_SCRIPT_FILE( KHenirResultTable );
	}
		
	//{{ 2008. 7. 16  ������	�Ӽ�NPC
	{
		KAttribNpcTable::RegScriptName( "AttribNpcTable.lua" );
		OPEN_SCRIPT_FILE( KAttribNpcTable );
	}
	//}}	

	//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	{
		KCnHackingCheckManager::RegScriptName( "CnHackingCheckData.lua" );
		OPEN_SCRIPT_FILE( KCnHackingCheckManager );
	}
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}

	{
		KAbuserLogManager::RegScriptName( "AbuserList.lua" );
		OPEN_SCRIPT_FILE( KAbuserLogManager );
	}

	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	{
		KMornitoringManager::RegScriptName( "ServerMornitoringConfig.lua" );
		OPEN_SCRIPT_FILE( KMornitoringManager );
	}
#endif SERV_ROOM_TICK_DUMP
	//}}

	//{{ 2011. 11. 4	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	{
		CXSLBattleFieldManager::RegScriptName( "BattleFieldServerData.lua" );
		CXSLBattleFieldManager::RegScriptName( "BattleFieldData.lua" );
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		CXSLBattleFieldManager::RegScriptName( "FieldBonusDrop.lua" );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

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
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 07. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	{
		CXSLPartyManager::RegScriptName( "PartyServerData.lua" );
		OPEN_SCRIPT_FILE( CXSLPartyManager );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	{
		KSMSPhoneNumberManager::RegScriptName( "SMSPhoneNumberList.lua" );
		OPEN_SCRIPT_FILE( KSMSPhoneNumberManager );
	}
#endif SERV_SMS_TOTAL_MANAGER
	//}}

	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	{
		KAutoPartyScriptManager::RegScriptName( "AutoPartyData.lua" );
		OPEN_SCRIPT_FILE( KAutoPartyScriptManager );
	}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	{
		KBadAttitudeTable::RegScriptName( "BadAttitudeTable.lua" );
		OPEN_SCRIPT_FILE( KBadAttitudeTable );
	}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 04. 12	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	{
		CXSLDefenceDungeonManager::RegScriptName( "DefenceDungeonData.lua" );
		OPEN_SCRIPT_FILE( CXSLDefenceDungeonManager );
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	{
		CXSLFieldBossData::RegScriptName( "FieldBossData.lua" );
		OPEN_SCRIPT_FILE( CXSLFieldBossData );
	}
#endif // SERV_BATTLE_FIELD_BOSS
}


void KCnSimLayer::Tick()
{
    KSimLayer::Tick();

	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	if( GetKCnServer()->IsConnectionMornitoringEnable() == true )
	{
		SiKMornitoringManager()->Tick_CnSimLayer();
	}
#endif SERV_ROOM_TICK_DUMP
	//}}
}

void KCnSimLayer::ShutDown()
{
	CXSLItemManager::ReleaseInstance();
	CXSLDungeonManager::ReleaseInstance();
	SiKResultProcess()->ReleaseInstance();
	KDropTable::ReleaseInstance();
	//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	KPvpMatchResultTable::ReleaseInstance();
	KPvpMatchManager::ReleaseInstance();
#else
	KPVPResultTable::ReleaseInstance();
#endif SERV_PVP_NEW_SYSTEM
	//}}	
	CXSLUnitManager::ReleaseInstance();
	KGameEventManager::ReleaseInstance();
	KGameSysVal::ReleaseInstance();
	KAbuserLogManager::ReleaseInstance();
	CXSLQuestManager::ReleaseInstance();
	//{{ 2009. 7. 9  ������		��ϸ� �ð�
	KHenirResultTable::ReleaseInstance();
	//}}
	//{{ 2008. 10. 20  ������	�Ӽ�NPC
	KAttribNpcTable::ReleaseInstance();
	//}}
	//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER	
	KGameEventScriptManager::ReleaseInstance();
#endif SERV_EVENT_MONSTER
	//}}
	//{{ 2012. 07. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CXSLPartyManager::ReleaseInstance();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 22	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	KSMSPhoneNumberManager::ReleaseInstance();
#endif SERV_SMS_TOTAL_MANAGER
	//}}

	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	KAutoPartyScriptManager::ReleaseInstance();
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	KBadAttitudeTable::ReleaseInstance();
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 04. 12	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CXSLDefenceDungeonManager::ReleaseInstance();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	CXSLFieldBossData::ReleaseInstance();
#endif // SERV_BATTLE_FIELD_BOSS

    KSimLayer::ShutDown();
}



//{{ ��ȫ�� [2012.05.16] ���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
std::string KCnSimLayer::GetStrItemLuaName()
{
	std::string strFile = "Item.lua";

	switch(static_cast<int>(GetDwNationFlag()))
	{
	case NF_DE:		strFile = "Item_DE.lua";	break;
	case NF_FR:		strFile = "Item_FR.lua";	break;
	case NF_IT:		strFile = "Item_IT.lua";	break;
	case NF_PL:		strFile = "Item_PL.lua";	break;
	case NF_ES:		strFile = "Item_ES.lua";	break;
	case NF_TR:		strFile = "Item_TR.lua";	break;
	case NF_EN:		strFile = "Item_EN.lua";	break;
	default:
		strFile = "Item.lua";
		break;
	}		

	return strFile;
}

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
std::string KCnSimLayer::GetStrItemTransLuaName()
{
	std::string strFile = "Item.lua";

	switch(static_cast<int>(GetDwNationFlag()))
	{
	case NF_DE:		strFile = "ItemTrans_DE.lua";	break;
	case NF_FR:		strFile = "ItemTrans_FR.lua";	break;
	case NF_IT:		strFile = "ItemTrans_IT.lua";	break;
	case NF_PL:		strFile = "ItemTrans_PL.lua";	break;
	case NF_ES:		strFile = "ItemTrans_ES.lua";	break;
	case NF_TR:		strFile = "ItemTrans_TR.lua";	break;
	case NF_EN:		strFile = "ItemTrans_EN.lua";	break;
	default:
		strFile = "ItemTrans.lua";
		break;
	}		

	return strFile;
}

#endif // SERV_ITEM_LUA_TRANS_DEVIDE

std::string KCnSimLayer::GetStrPvpNpcDataLua()
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