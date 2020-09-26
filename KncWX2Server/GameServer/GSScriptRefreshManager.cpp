#include "ProxyManager.h"
#include "GSScriptRefreshThread.h"
#include "GSScriptRefreshManager.h"
#include "KncSend.h"
#include "KAutoPath.h"


#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
//#ifdef SERV_REALTIME_SCRIPT

ImplPfID( KGSScriptRefreshManager, PI_NULL );

ImplementSingleton( KGSScriptRefreshManager );

KGSScriptRefreshManager::KGSScriptRefreshManager() :
m_pRefreshLuaState( NULL )
{
}

KGSScriptRefreshManager::~KGSScriptRefreshManager()
{
	if( m_pRefreshLuaState != NULL )
	{
		lua_close( m_pRefreshLuaState );
		//{{ 2010. 8. 30	������	���� ���� ���� ó��		
		m_pRefreshLuaState = NULL;
		//}}
	}
}

void KGSScriptRefreshManager::RegToLua( lua_State* pLuaState )
{
	lua_tinker::class_add<KGSScriptRefreshManager>( pLuaState, "KGSScriptRefreshManager" );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ItemManager",				&KGSScriptRefreshManager::ItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RandomItemManager",			&KGSScriptRefreshManager::RandomItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ManufactureItemManager",	&KGSScriptRefreshManager::ManufactureItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RewardTable",				&KGSScriptRefreshManager::RewardTable_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "QuestManager",				&KGSScriptRefreshManager::QuestManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "TitleManager",				&KGSScriptRefreshManager::TitleManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DungeonManager",			&KGSScriptRefreshManager::DungeonManager_LUA );	
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DropTable",					&KGSScriptRefreshManager::DropTable_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "GameEventScriptManager",	&KGSScriptRefreshManager::GameEventScriptManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "MapData",					&KGSScriptRefreshManager::MapData_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "GuildManager",				&KGSScriptRefreshManager::GuildManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "GameSysVal",				&KGSScriptRefreshManager::GameSysVal_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ResolveItemManager",		&KGSScriptRefreshManager::ResolveItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "EnchantItemManager",		&KGSScriptRefreshManager::EnchantItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SocketItem",				&KGSScriptRefreshManager::SocketItem_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AttribEnchantItem",			&KGSScriptRefreshManager::AttribEnchantItem_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RecommendUserTable",		&KGSScriptRefreshManager::RecommendUserTable_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ExpTable",					&KGSScriptRefreshManager::ExpTable_LUA );
	//{{ 2010. 9. 11	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PetManager",				&KGSScriptRefreshManager::PetManager_LUA );
#endif SERV_PET_SYSTEM
	//}}
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AttribNpcTable",			&KGSScriptRefreshManager::AttribNpcTable_LUA );
	//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "CnHackingCheckManager",		&KGSScriptRefreshManager::CnHackingCheckManager_LUA );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "MornitoringManager",		&KGSScriptRefreshManager::MornitoringManager_LUA );
#endif SERV_MORNITORING
	//}}
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AbuserLogManager",			&KGSScriptRefreshManager::AbuserLogManager_LUA );
	//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ResultProcess",				&KGSScriptRefreshManager::ResultProcess_LUA );
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "WorldMissionManager",		&KGSScriptRefreshManager::WorldMissionManager_LUA );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PvpMatchManager",			&KGSScriptRefreshManager::PvpMatchManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PvpMatchResultTable",		&KGSScriptRefreshManager::PvpMatchResultTable_LUA );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	������	�ð� ����
#ifdef SERV_NEW_HENIR_TEST
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "HenirResultTable",			&KGSScriptRefreshManager::HenirResultTable_LUA );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ContentManager",			&KGSScriptRefreshManager::ContentManager_LUA );
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2011.10.14     ��μ�    ��� ����� Ư�� IP ������ ��� ����
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "CompanyIPTable",			&KGSScriptRefreshManager::CompanyIPTable_LUA );
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ClassChangeTable",			&KGSScriptRefreshManager::ClassChangeTable_LUA );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "StringFilter",				&KGSScriptRefreshManager::StringFilter_LUA );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 04. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "IncreaseTheProtectedCrystalCount",	&KGSScriptRefreshManager::WorldMissionManager_IncreaseCrystalCount_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ResetTheProtectedCrystalCount",	&KGSScriptRefreshManager::WorldMissionManager_ResetIncreaseCrystalCount_LUA );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 06. 06	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DailyGiftBoxManager",	&KGSScriptRefreshManager::DailyGiftBoxManager_Lua );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BattleFieldManager",		&KGSScriptRefreshManager::BattleFieldManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AutoPartyScriptManager",	&KGSScriptRefreshManager::AutoPartyScriptManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PartyManager",				&KGSScriptRefreshManager::PartyManager_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SmsManager",	&KGSScriptRefreshManager::SmsManager_Lua );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BingoEventManager",	&KGSScriptRefreshManager::BingoEventManager_Lua );
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BadAttitudeTable",				&KGSScriptRefreshManager::BadAttitudeTable_LUA );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

#ifdef SERV_SYNTHESIS_AVATAR
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SynthesisTable",			&KGSScriptRefreshManager::SynthesisTable_LUA );
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "LogManager",				&KGSScriptRefreshManager::LogManager_LUA );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DefenceDungeonManager",		&KGSScriptRefreshManager::DefenceDungeonManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BuffManager",				&KGSScriptRefreshManager::BuffManager_LUA );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RidingPetManager",			&KGSScriptRefreshManager::RidingPetManager_LUA );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SkillTree",			&KGSScriptRefreshManager::SkillTree_LUA );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "FieldBossData",		&KGSScriptRefreshManager::FieldBossData_LUA );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "EDInventory", &KGSScriptRefreshManager::EDInventoryExpand_LUA );
#endif // SERV_GOOD_ELSWORD

	lua_tinker::decl( pLuaState, "ScriptRefresh", this );
}

void KGSScriptRefreshManager::ItemManager_LUA()
{
	START_LOG( cout, L"ItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� ItemManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ITEM_MANAGER );

	// ���ͼ����� ItemManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::RandomItemManager_LUA()
{
	START_LOG( cout, L"RandomItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� RandomItemManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RANDOM_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"RandomItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::ManufactureItemManager_LUA()
{
	START_LOG( cout, L"ManufactureItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� ManufactureItemManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MANUFACTURE_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ManufactureItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::RewardTable_LUA()
{
	START_LOG( cout, L"RewardTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� RewardTable ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_REWARD_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"RewardTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::QuestManager_LUA()
{
	START_LOG( cout, L"QuestManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� QuestManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_QUEST_MANAGER );

	// ���ͼ����� QuestManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_QUEST_MANAGER );

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	// �۷ι������� QuestManager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_QUEST_MANAGER );
#endif SERV_RANDOM_DAY_QUEST
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"QuestManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::TitleManager_LUA()
{
	START_LOG( cout, L"TitleManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� TitleManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_TITLE_MANAGER );

	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_TITLE_MANAGER );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"TitleManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::DungeonManager_LUA()
{
	START_LOG( cout, L"DungeonManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� DungeonManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_DUNGEON_MANAGER );

	// ���ͼ����� DungeonManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DUNGEON_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"DungeonManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::DropTable_LUA()
{
	START_LOG( cout, L"DropTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� DropTable ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DROP_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"DropTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::GameEventScriptManager_LUA()
{
	START_LOG( cout, L"GameEventScriptManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� GameEventScriptManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GAME_EVENT_SCRIPT_MANAGER );

	// ���ͼ����� GameEventScriptManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_GAME_EVENT_SCRIPT_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GameEventScriptManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::MapData_LUA()
{
	START_LOG( cout, L"MapData Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� MapData ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MAP_DATA );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"MapData Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::GuildManager_LUA()
{
	START_LOG( cout, L"GuildManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� GuildManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GUILD_MANAGER );

	// �α��μ����� GuildManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_GUILD_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GuildManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::GameSysVal_LUA()
{
	START_LOG( cout, L"GameSysVal Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Game Sys Val ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GAME_SYS_VAL );

	// ���ͼ����� Game Sys Val ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_GAME_SYS_VAL );

	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_GAME_SYS_VAL );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GameSysVal Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::ResolveItemManager_LUA()
{
	START_LOG( cout, L"ResolveItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Resolve Item Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RESOLVE_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ResolveItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::EnchantItemManager_LUA()
{
	START_LOG( cout, L"EnchantItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Enchant Item Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ENCHANT_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"EnchantItemManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::SocketItem_LUA()
{
	START_LOG( cout, L"SocketItem Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Socket Item ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SOCKET_ITEM );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"SocketItem Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::AttribEnchantItem_LUA()
{
	START_LOG( cout, L"AttribEnchantItem Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Attrib Enchant Item ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ATTRIB_ENCHANT_ITEM );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AttribEnchantItem Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::RecommendUserTable_LUA()
{
	START_LOG( cout, L"RecommendUserTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Recommend User Table ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RECOMMEND_USER_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"RecommendUserTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::ExpTable_LUA()
{
	START_LOG( cout, L"ExpTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Exp Table ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_EXP_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ExpTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

//{{ 2010. 9. 11	������	�� �ý���
#ifdef SERV_PET_SYSTEM
void KGSScriptRefreshManager::PetManager_LUA()
{
	START_LOG( cout, L"PetManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Pet Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_PET_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PetManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS


}
#endif SERV_PET_SYSTEM
//}}

void KGSScriptRefreshManager::AttribNpcTable_LUA()
{
	START_LOG( cout, L"AttribNpcTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� Attrib Npc Table ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_ATTRIB_NPC_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AttribNpcTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
void KGSScriptRefreshManager::CnHackingCheckManager_LUA()
{
	START_LOG( cout, L"CnHackingCheckManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� Cn Hacking Check Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_HACKING_CHECK_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"CnHackingCheckManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_CN_SERVER_HACKING_CHECK
//}}

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
void KGSScriptRefreshManager::MornitoringManager_LUA()
{
	START_LOG( cout, L"MornitoringManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� MornitoringManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MORNITORING_MANAGER );

	// �α��μ����� MornitoringManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_MORNITORING_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"MornitoringManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_MORNITORING
//}}

void KGSScriptRefreshManager::AbuserLogManager_LUA()
{
	START_LOG( cout, L"AbuserLogManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Abuser Log Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ABUSER_LOG_MANAGER );

	// ���ͼ����� Abuser Log Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_ABUSER_LOG_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AbuserLogManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
void KGSScriptRefreshManager::ResultProcess_LUA()
{
	START_LOG( cout, L"ResultProcess Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� ResultProcess ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_RESULT_PROCESS );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ResultProcess Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_DUNGEON_RANK_NEW
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
void KGSScriptRefreshManager::WorldMissionManager_LUA()
{
	START_LOG( cout, L"WorldMissionManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// �۷ι������� WorldMissionManager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_WORLD_MISSION_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"WorldMissionManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
void KGSScriptRefreshManager::PvpMatchManager_LUA()
{
	START_LOG( cout, L"PvpMatchManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Pvp Match Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_PVP_MATCH_MANAGER );

	// ���ͼ����� Pvp Match Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_PVP_MATCH_MANAGER );

	// �۷ι������� Pvp Match Manager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_PVP_MATCH_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PvpMatchManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::PvpMatchResultTable_LUA()
{
	START_LOG( cout, L"PvpMatchResultTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� Pvp Match Result Table ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_PVP_MATCH_RESULT_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PvpMatchResultTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 08. 24	������	�ð� ����
#ifdef SERV_NEW_HENIR_TEST
void KGSScriptRefreshManager::HenirResultTable_LUA()
{
	START_LOG( cout, L"HenirResultTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );
    
	// ���ͼ����� Henir Result Table ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_HENIR_RESULT_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"HenirResultTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_NEW_HENIR_TEST
//}}


//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
void KGSScriptRefreshManager::ContentManager_LUA()
{
	START_LOG( cout, L"ContentManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� ContentManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_CONTENT_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ContentManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KGSScriptRefreshManager::BattleFieldManager_LUA()
{
	START_LOG( cout, L"BattleFieldManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	// �۷ι������� Battle Field Manager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_BATTLE_FIELD_MANAGER );
#endif // SERV_BATTLE_FIELD_BOSS

	// ���ͼ����� Battle Field Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_BATTLE_FIELD_MANAGER );

	// ���Ӽ����� Battle Field Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_BATTLE_FIELD_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"BattleFieldManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::AutoPartyScriptManager_LUA()
{
	START_LOG( cout, L"AutoPartyScriptManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");

	// �۷ι������� AutoPartyScriptManager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_AUTO_PARTY_SCRIPT_MANAGER );

	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	// ���ͼ����� AutoPartyScriptManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_AUTO_PARTY_SCRIPT_MANAGER );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AutoPartyScriptManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::PartyManager_LUA()
{
	START_LOG( cout, L"PartyManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );
    
	// ���ͼ����� Party Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_PARTY_MANAGER );

	// ���Ӽ����� Party Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_PARTY_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PartyManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
void KGSScriptRefreshManager::BadAttitudeTable_LUA()
{
	START_LOG( cout, L"BadAttitudeTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� BadAttitudeTable ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_BAD_ATTITUDE_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"BadAttitudeTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
void KGSScriptRefreshManager::DefenceDungeonManager_LUA()
{
	START_LOG( cout, L"Defence Dungeon Manager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� Defence Dungeon Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DEFENCE_DUNGEON_MANAGER );

	// ���Ӽ����� Defence Dungeon Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_DEFENCE_DUNGEON_MANAGER );
}

void KGSScriptRefreshManager::BuffManager_LUA()
{
	START_LOG( cout, L"Buff Manager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���ͼ����� Buff Manager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_BUFF_MANAGER );

	// ���Ӽ����� Buff Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_BUFF_MANAGER );
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

void KGSScriptRefreshManager::Init()
{
	// ��ũ��Ʈ ���� ���� lua_state ����!
	m_pRefreshLuaState = lua_open();

	_JIF( m_pRefreshLuaState != NULL, return );

	luaL_openlibs( m_pRefreshLuaState );
	lua_settop( m_pRefreshLuaState, 0 );

	{
		// enum.lua �Ľ�
		std::string strFile = "Enum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"GSScriptRefreshManager : Enum ���� �ε� ����.!" );
			return;
		}
	}

	{
		// enum.lua �Ľ�
		std::string strFile = "DungeonEnum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"GSScriptRefreshManager : DungeonEnum ���� �ε� ����.!" );
			return;
		}
	}

	// ������� ������ 1���� ����!
	KThreadManager::Init( 1 );

	// ������ ����!
	KThreadManager::BeginThread();

	START_LOG( cout, L"��ũ��Ʈ �������� ������ ����!" );
}

KThread* KGSScriptRefreshManager::CreateThread()
{
	return new KGSScriptRefreshThread;
}

void KGSScriptRefreshManager::RefreshOrderToCnServer( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;
	KncSend( PI_GS_SERVER, GetUID(), PI_CN_SERVER, 0, NULL, ESR_ORDER_TO_CENTER_SERVER_NOT, kNot );
}

void KGSScriptRefreshManager::RefreshOrderToCnServerBroadCast( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, ESR_ORDER_TO_CENTER_SERVER_NOT, kNot );

	SiKProxyManager()->BroadCastToCnServer( *spEvent );
}

void KGSScriptRefreshManager::RefreshOrderToCnServerEachServerGroup( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, ESR_ORDER_TO_CENTER_SERVER_NOT, kNot );

	SiKProxyManager()->BroadCastToCnServerEachServerGroup( *spEvent );
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
void KGSScriptRefreshManager::RefreshOrderToGBServer( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;
	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_SERVER, 0, NULL, ESR_ORDER_TO_GLOBAL_SERVER_NOT, kNot );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011.10.14     ��μ�    ��� ����� Ư�� IP ������ ��� ����
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
void KGSScriptRefreshManager::CompanyIPTable_LUA()
{
	START_LOG( cout, L"CompanyIPTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� TitleManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_COMPANY_IP_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"CompanyIPTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
void KGSScriptRefreshManager::ClassChangeTable_LUA()
{
	START_LOG( cout, L"ClassChangeTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� TitleManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_CLASS_CHANGE_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ClassChangeTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
void KGSScriptRefreshManager::StringFilter_LUA()
{
	START_LOG( cout, L"StringFitler Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� String Fitler ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_STRING_FILTER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"StringFitler Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}

//{{ 2012. 04. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
void KGSScriptRefreshManager::WorldMissionManager_IncreaseCrystalCount_LUA( int iIncreaseCrystalNum )
{
	START_LOG( cout, L"WorldMissionManager ��ȣ�� ������ ���� ������ ��û�մϴ�.");

	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_SERVER, 0, NULL, EGB_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, iIncreaseCrystalNum );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"WorldMissionManager ��ȣ�� ������ ���� ������ ��û�մϴ�."));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::WorldMissionManager_ResetIncreaseCrystalCount_LUA( void )
{
	START_LOG( cout, L"WorldMissionManager ��ȣ�� ������ ���� �ʱ�ȭ�� ��û�մϴ�.");

	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_SERVER, 0, NULL, EGB_RESET_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, char() );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"WorldMissionManager ��ȣ�� ������ ���� �ʱ�ȭ�� ��û�մϴ�."));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 06. 06	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
void KGSScriptRefreshManager::DailyGiftBoxManager_Lua( void )
{
	START_LOG( cout, L"DailyGiftBoxManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");
	// ���Ӽ����� DailyGiftBoxPackage ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_DAILY_GIFT_BOX_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"DailyGiftBoxManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
void KGSScriptRefreshManager::SmsManager_Lua( void )
{
	START_LOG( cout, L"KSMSPhoneNumberManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");
	
	// ���Ӽ����� SMSPhoneNumberManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SMS_MANAGER );

	// ���ͼ����� SMSPhoneNumberManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SMS_MANAGER );

	// �α��μ����� SMSPhoneNumberManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SMS_MANAGER );

	// �۷ι������� SMSPhoneNumberManager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_SMS_MANAGER );
}
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
void KGSScriptRefreshManager::BingoEventManager_Lua( void )
{
	START_LOG( cout, L"KGSBingoEventInfo Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");

	// ���Ӽ����� SMSPhoneNumberManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_BINGO_EVENT_MANAGER );
}
#endif SERV_EVENT_BINGO
//}}

//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
void KGSScriptRefreshManager::LogManager_LUA()
{
	START_LOG( cout, L"KLogManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");

	// ���Ӽ����� KLogManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_LOG_MANAGER );

	// ���ͼ����� KLogManager ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_LOG_MANAGER );

	// �α��μ����� KLogManager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_LOG_MANAGER );

	// �۷ι������� KLogManager ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_LOG_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"KLogManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_SYNTHESIS_AVATAR
void KGSScriptRefreshManager::SynthesisTable_LUA()
{
	START_LOG( cout, L"SynthesisTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SYNTHESIS_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"SynthesisTable Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}
#endif SERV_SYNTHESIS_AVATAR

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
void KGSScriptRefreshManager::RidingPetManager_LUA()
{
	START_LOG( cout, L"RidingPetManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� Riding Pet Manager ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RIDING_PET_MANAGER );
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
void KGSScriptRefreshManager::SkillTree_LUA( void )
{
	START_LOG( cout, L"SkillTree Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ����� SkillTree ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SKILL_TREE );

	// �α��μ����� SkillTree ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SKILL_TREE );
}
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
void KGSScriptRefreshManager::FieldBossData_LUA( void )
{
	START_LOG( cout, L"CXSLFieldBossData Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );

	// ���Ӽ��� ���� ��û!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_FIELD_BOSS_DATA );

	// ���ͼ��� ���� ��û!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_FIELD_BOSS_DATA );

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	// �۷ι����� ���� ��û!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_FIELD_BOSS_DATA );
#endif SERV_RANDOM_DAY_QUEST
	//}}
}
#endif // SERV_BATTLE_FIELD_BOSS

//#endif SERV_REALTIME_SCRIPT
//}}

#ifdef SERV_GOOD_ELSWORD
void KGSScriptRefreshManager::EDInventoryExpand_LUA()
{
    START_LOG( cout, L"InventoryExpand Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!" );
    // ���Ӽ����� InventoryExpand ���� ��û!
    RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ED_INVENTORY_EXPAND );
}
#endif // SERV_GOOD_ELSWORD