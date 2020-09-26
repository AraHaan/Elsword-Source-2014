#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;


#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" ��ũ��Ʈ ������ �Ϸ��Ͽ����ϴ�!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
//#ifdef SERV_REALTIME_SCRIPT


class KGSScriptRefreshManager : public KThreadManager
{
	DeclareSingleton( KGSScriptRefreshManager );
	DeclPfID;

public:
	KGSScriptRefreshManager();
	virtual ~KGSScriptRefreshManager();

	void RegToLua( lua_State* pLuaState );

	// ��ũ��Ʈ ���� ��û lua�Լ�
    void ItemManager_LUA();
	void RandomItemManager_LUA();
	void ManufactureItemManager_LUA();
	void RewardTable_LUA();
	void QuestManager_LUA();
	void TitleManager_LUA();
	void DungeonManager_LUA();
	void DropTable_LUA();
	void GameEventScriptManager_LUA();
	void MapData_LUA();
	void GuildManager_LUA();
	void GameSysVal_LUA();
	void ResolveItemManager_LUA();
	void EnchantItemManager_LUA();
	void SocketItem_LUA();
	void AttribEnchantItem_LUA();
	void RecommendUserTable_LUA();
	void ExpTable_LUA();	

	//{{ 2010. 9. 11	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	void PetManager_LUA();
#endif SERV_PET_SYSTEM
	//}}
	void AttribNpcTable_LUA();
	//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	void CnHackingCheckManager_LUA();
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	void MornitoringManager_LUA();
#endif SERV_MORNITORING
	//}}
	void AbuserLogManager_LUA();
	//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	void ResultProcess_LUA();
#endif SERV_DUNGEON_RANK_NEW
	//}}

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	void WorldMissionManager_LUA();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	void PvpMatchManager_LUA();
	void PvpMatchResultTable_LUA();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	������	�ð� ����
#ifdef SERV_NEW_HENIR_TEST
	void HenirResultTable_LUA();
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	void ContentManager_LUA();
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2011.10.14     ��μ�    ��� ����� Ư�� IP ������ ��� ����
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	void CompanyIPTable_LUA();
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void ClassChangeTable_LUA();
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	void StringFilter_LUA();
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 04. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void WorldMissionManager_IncreaseCrystalCount_LUA( int iIncreaseCrystalNum );
	void WorldMissionManager_ResetIncreaseCrystalCount_LUA( void );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 06. 06	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	void DailyGiftBoxManager_Lua( void );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void BattleFieldManager_LUA();
	void AutoPartyScriptManager_LUA();
	void PartyManager_LUA();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	void SmsManager_Lua( void );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	void	BingoEventManager_Lua( void );
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	void BadAttitudeTable_LUA();
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

#ifdef SERV_SYNTHESIS_AVATAR
	void SynthesisTable_LUA();
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	void LogManager_LUA();
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	void DefenceDungeonManager_LUA();
	void BuffManager_LUA();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	void RidingPetManager_LUA( void );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
	void SkillTree_LUA( void );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	void	FieldBossData_LUA( void );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    void EDInventoryExpand_LUA();
#endif 
	// derived from KThreadManager
	virtual void Init();
	virtual KThread* CreateThread();

	lua_State* GetRefreshLuaState() { return m_pRefreshLuaState; }

protected:
	void RefreshOrderToCnServer( IN int iOrderType );
	void RefreshOrderToCnServerBroadCast( IN int iOrderType );
	void RefreshOrderToCnServerEachServerGroup( IN int iOrderType );
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	void RefreshOrderToGBServer( IN int iOrderType );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�


protected:
	lua_State* m_pRefreshLuaState;
};

DefSingletonInline( KGSScriptRefreshManager );


//#endif SERV_REALTIME_SCRIPT
//}}