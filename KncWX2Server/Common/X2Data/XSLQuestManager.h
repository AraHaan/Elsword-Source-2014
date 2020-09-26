#pragma once

#include "RefreshSingleton.h"
#include "X2Data/XSLMain.h"
#include "X2Data/XSLDungeon.h"
#include "Enum/Enum.h"
#include <RTTI.h>
#include "x2data/XSLRoom.h"
//{{ 2010. 10. 25	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	#include "X2Data/XSLAttribEnchantItem.h"
#endif SERV_QUEST_CLEAR_EXPAND
//}}

//Quest Manager �� ������ ����Ŭ�����θ� ����Ѵ�.
//quest/sub quest �����͸� ���������Ͽ� quest���� sub index�� ������ �����͸� ���� �ϴ� ���.



class CXSLQuestManager
{
	DeclareRefreshSingleton( CXSLQuestManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;	

	enum QUEST_NUM_INFO
	{
		MAX_QUEST_NUM = 3,
		MAX_SUB_QUEST_NUM = 5,
	};

	enum QUEST_ENUM
	{
#ifdef SERV_COUNTRY_US
		QE_DAILY_REPEAT_HOUR = 0,
#else // SERV_COUNTRY_US
		QE_DAILY_REPEAT_HOUR = 6,
#endif // SERV_COUNTRY_US

		//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		QE_ACCOUNT_DAILY_REPEAT_HOUR = 0,
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

	};

	enum QUEST_TYPE
	{
		QT_NORMAL = 0,
		QT_EVENT,
		QT_CHANGE_JOB,
		QT_SKILL,
		//{{ kimhc // 2010-02-01 // PC�� �߰�
		QT_PCBANG,
		//}} kimhc // 2010-02-01 // PC�� �߰�
#ifdef SERV_EPIC_QUEST
		QT_EPIC,
		QT_CONTEST,
#endif SERV_EPIC_QUEST
		QT_CHARACTER,	// ���� // 2009-12-23 �߰�

		QT_END,
	};

 	enum QUEST_REPEAT_TYPE
 	{
 		QRT_NORMAL,				// �Ϸ��ϸ� �ݺ� ���� �ʴ� ����Ʈ
 		QRT_DAY,				// ���� ������ �ð��� ����Ʈ ���� �� �ִ� ����
 		QRT_REPEAT,				// ����Ʈ�� �Ϸ��ϸ� �ٷ� �ٽ� �� �� �ִ� ����
 	};

	//Sub quest data �� ����Ʈ �������Ǻ� ������(Ex. npc�� ���ϱ�/�������/�����ۼ��� ��..)�� ���� ������
	//�Ʒ� Ÿ���� ������ �Ǿ Ÿ�Կ� �´� �����͸� ����ϰ� ��.
	//�� sub quest type�� ��� �����ʹ� LoadClearCondition()�Լ��� Ȯ��.
	enum SUB_QUEST_TYPE
	{
		SQT_NONE							= 0,
		SQT_NPC_TALK						= 1,
		SQT_NPC_HUNT						= 2,
		SQT_ITEM_COLLECTION					= 3,
		SQT_QUEST_ITEM_COLLECTION			= 4,
		SQT_DUNGEON_TIME					= 5,
		SQT_DUNGEON_RANK					= 6,
		SQT_DUNGEON_DAMAGE					= 7,
		SQT_DUNGEON_CLEAR_COUNT				= 8,	//������ Ŭ�����Ͽ� �Ǽ��� ä��� ����Ʈ
		SQT_PVP_PLAY						= 9,	//������ ������ ��ŭ �÷����ϱ�(���â���� ���� �÷��̷� ����)
		SQT_PVP_WIN							= 10,	//������ �÷����Ͽ� �¶��ϱ�
		SQT_PVP_KILL						= 11,	//������ �÷����Ͽ� ų���ø���(KILL / MAX DAMAGE KILL) 
		//{{ 2010. 05. 17  ������	���� ���� ������ ����
		SQT_WITH_DIF_SERV_USER				= 12,
		//}}
		//{{ 2010. 10. 25	������	����Ʈ ���� �߰�
//#ifdef SERV_QUEST_CLEAR_EXPAND
		SQT_ITEM_ENCHANT					= 13,
		SQT_ITEM_SOCKET						= 14,
		SQT_ITEM_ATTRIB						= 15,
		SQT_ITEM_RESOLVE					= 16,
		SQT_ITEM_EQUIP_DUNGEON_CLEAR		= 17,
		SQT_USE_SKILL_POINT					= 18,
		SQT_FEED_PET						= 19,
		SQT_USER_DIE						= 20,
//#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
//#ifdef SERV_NEW_PVP_QUEST
		SQT_PVP_NPC_HUNT					= 21,	// �������� NPC���� �¸�
//#endif SERV_NEW_PVP_QUEST
		//}}
		SQT_ITEM_USE						= 22,
		SQT_SKILL_USE						= 23,
		//{{ 2012. 04. 17	������	DB���� ���� üũ
//#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		SQT_VISIT_VILLAGE					= 24,
		SQT_VISIT_FIELD						= 25,
//#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		//{{ QUEST ���� - ��μ�
//#ifdef SERV_REFORM_QUEST
		SQT_VISIT_DUNGEON					= 26,
		SQT_FIND_NPC						= 27,
//#endif SERV_REFORM_QUEST

//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
//#ifdef SERV_2012_PVP_SEASON2
		SQT_PVP_PLAY_ARRANGE				= 28,	// ��������� ���� ���� �÷��� Ƚ���� �������� �Ѵ�.
//#endif SERV_2012_PVP_SEASON2
//}}
		SQT_PVP_TAG_COUNT					= 29,
		SQT_HYPER_MODE_USE					= 30,
//#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		SQT_CHAR_LEVEL_UP					= 31,
//#endif SERV_ACCOUNT_MISSION_SYSTEM
//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
//#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
		SQT_SUITABLE_LEVEL_DUNGEON_CLEAR	= 32,
//#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
//}}
//#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND // 2013.02.08 lygan_������ // ���� ����Ʈ ���� �߰� ( �̰� �������� �ȵ��� ���� �� �ֱ� ������ ������ ����д� )
		SQT_PVP_HERO_NPC_KILL				= 33,
		SQT_PVP_HERO_NPC_PLAY				= 34,
//#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND
//#ifdef SERV_POINT_COUNT_SYSTEM
		SQT_POINT_COUNT						= 35,
//#endif SERV_POINT_COUNT_SYSTEM
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
		SQT_LEARN_NEW_SKILL					= 36,
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
		SQT_END,
	};

	//����Ʈ�� ������ �ִ� ���� ���̵� Ÿ��
	enum QUEST_MAPID_TYPE
	{
		QMT_NONE	= 0,
		QMT_VILLAGE,
		QMT_DUNGEON,
	};

public:
	//Quest �� �ޱ����� �������
	struct QuestCondition
	{
		int						m_iLevel;
		CXSLUnit::UNIT_TYPE		m_eUnitType;
		std::set< CXSLUnit::UNIT_CLASS > m_setUnitClass;
		//CXSLUnit::UNIT_CLASS	m_eUnitClass;
		int						m_iConditionItemID;
		int						m_iConditionItemNum;
		int						m_iNotBeforeQuestID;
		std::vector<int>		m_vecBeforeQuestID;

		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		int						m_iLimitLevel;			// �Էµ� ������ �Ѿ�� �Ǹ�, Ŭ���̾�Ʈ���� Ȯ�� �� �� ����.(���� �Ұ�)

		//# �����̵��� ��������Ʈ �ڵ������� ����
		std::set<int>			m_setEnableVillage;
		//# ���� ����� ��������Ʈ �ڵ������� ����
		std::set<int>			m_setEnableDungeon;
		//# ��Ʋ�ʵ� ����� ��������Ʈ �ڵ������� ����
		std::set<int>			m_setEnableBattleField;
#else
	#ifdef SERV_EPIC_QUEST
		//# �����̵��� ��������Ʈ �ڵ������� ����
		int						m_iEnableVillage;
	#endif SERV_EPIC_QUEST
#endif SERV_REFORM_QUEST
		//}} 

		QuestCondition()
		{
			m_iLevel				= -1;
			m_eUnitType				= CXSLUnit::UT_NONE;
			//m_eUnitClass			= CXSLUnit::UC_NONE;
			m_iConditionItemID		= -1;
			m_iConditionItemNum		= -1;
			m_iNotBeforeQuestID		= -1;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			m_iLimitLevel			= -1;
#else
	#ifdef SERV_EPIC_QUEST
			m_iEnableVillage		= 0;
	#endif SERV_EPIC_QUEST
#endif SERV_REFORM_QUEST
			//}} 
		}
	};

	//Sub quest type ���Ǻ� Ȯ���� ������
	struct ClearCondition
	{
		//::���� ������
		//::1. Dungeon ID
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		std::set<SEnum::DUNGEON_ID>	m_setDungeonID;
#else
		SEnum::DUNGEON_ID			m_eDungeonID;
#endif SERV_REFORM_QUEST
		//}} 

		//::2. Dungeon Difficulty
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
#else
		char							m_cDifficulty;
#endif SERV_REFORM_QUEST
		//}} 
		//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
		CXSLDungeon::DUNGEON_MODE		m_eDungeonMode;
#endif SERV_SECRET_HELL
		//}}
		//::3. Upper Difficulty
		bool							m_bUpperDifficulty;

		//::4. NPC ID & Monster ID 2���� ���.
		std::set< CXSLUnitManager::NPC_UNIT_ID > m_setKillNPCID;

		//::5. PVP TYPE
		CXSLRoom::PVP_GAME_TYPE			m_ePVPType;
		//::6. Item ID
		int								m_iItemID;

		//::���Ǻ� ������
		//::1. Monster kill count..
		int								m_iKillNum;
		//::2. Item collection count
		int								m_iCollectionItemNum;
		//::3. clear dungeon rank
		CXSLDungeon::RANK_TYPE			m_eDungeonRank;
		//::4. clear dungeon time
		int								m_iDungeonClearTime;
		//::5. talk npc
		CXSLUnitManager::NPC_UNIT_ID	m_eTalkNPCID;
		bool							m_bTalkNPC;	
		//::6. pvp play count
		int								m_iPVPPlay;
		//::7. pvp win count
		int								m_iPVPWin;
		//::8. pvp kill count
		int								m_iPVPKill;
		//::9. quest item drop rate
		float							m_fQuestItemDropRate;
		//::10. damage
		int								m_iDungeonDamage;
		//::11. Dungeon clear count
		int								m_iDungeonClearCount;
		//{{ 2010. 10. 25	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		//::12. Item Enchant
		int								m_iEnchantLevel;
		//::13. Item Socket
		int								m_iSocketCount;
		//::14. Item Attribute Enchant
		KItemAttributeEnchantInfo		m_kAttribEnchantInfo;
		CXSLAttribEnchantItem::ATTRIB_COUNT_TYPE	m_eAttribCountType;
		//::15. Item Resolve
		int								m_iResolveCount;
		//::16. Use Skill Point
		int								m_iSkillID;
		int								m_iSpUseCount;
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
		KPVPChannelInfo::PVP_CHANNEL_CLASS		m_ePvpChannelClass;
#endif SERV_NEW_PVP_QUEST
		//}} 
		//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		std::set<int>					m_setVillageMapID;
		std::set<int>					m_setBattleFieldID;
		int								m_iFindNPCID;
#else
		int								m_iVillageMapID;
		int								m_iBattleFieldID;
#endif SERV_REFORM_QUEST
		//}} 
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		int								m_iCharLevel;
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
#ifdef SERV_EVENT_TITLE_SUBQUEST
		int								m_iUseTitleID;
#endif SERV_EVENT_TITLE_SUBQUEST
#ifdef SERV_POINT_COUNT_SYSTEM
		int								m_iPointCount;
#endif SERV_POINT_COUNT_SYSTEM
#ifdef SERV_SUB_QUEST_USE_ITEM
		std::vector<int>				m_vecUseItemID;
		int								m_iUseItemNum;
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_SKILL_USE_SUBQUEST
		int								m_iUseSkillCount;
		std::set< int >					m_setSkillID;
#endif SERV_SKILL_USE_SUBQUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
		int								m_iLearnNewSkillCount;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

		ClearCondition()
		{
			m_iKillNum				= 0;
			m_iItemID				= -1;
			m_iCollectionItemNum	= 0;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
#else
			m_eDungeonID			= SEnum::DI_NONE;
			m_cDifficulty			= -1;
#endif SERV_REFORM_QUEST
			//}} 

			//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
			m_eDungeonMode			= CXSLDungeon::DM_INVALID;
#endif SERV_SECRET_HELL
			//}}
			m_bUpperDifficulty		= false;
			m_eDungeonRank			= CXSLDungeon::RT_NONE;
			m_iDungeonClearTime		= 0;
			m_eTalkNPCID			= CXSLUnitManager::NUI_NONE;
			m_bTalkNPC				= false;
			m_ePVPType				= CXSLRoom::PGT_TEAM;
			m_iPVPPlay				= 0;
			m_iPVPWin				= 0;
			m_iPVPKill				= 0;
			m_fQuestItemDropRate	= 0.0f;
			m_iDungeonDamage		= 0;
			m_iDungeonClearCount	= 0;
			//{{ 2010. 10. 25	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
			m_iEnchantLevel			= 0;
            m_iSocketCount			= 0;			
			m_eAttribCountType		= CXSLAttribEnchantItem::ACT_NONE;
			m_iResolveCount			= 0;
			m_iSkillID				= 0;
			m_iSpUseCount			= 0;
#endif SERV_QUEST_CLEAR_EXPAND
			//}}
			//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
			m_ePvpChannelClass		= KPVPChannelInfo::PCC_NONE;
#endif SERV_NEW_PVP_QUEST
			//}} 
			//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
#else
			m_iVillageMapID			= 0;
			m_iBattleFieldID		= 0;
#endif SERV_REFORM_QUEST
			//}} 
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}

			//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
			m_iCharLevel			= 99999; // 0 ���� ���õǸ� �ٷ� �Ϸ� �Ǿ� ������ ������...
#endif SERV_ACCOUNT_MISSION_SYSTEM
			//}}
#ifdef SERV_EVENT_TITLE_SUBQUEST
		    m_iUseTitleID           = -1;
#endif SERV_EVENT_TITLE_SUBQUEST
#ifdef SERV_SUB_QUEST_USE_ITEM
			m_iUseItemNum			= 0;
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_POINT_COUNT_SYSTEM
			m_iPointCount			= 0;
#endif  SERV_POINT_COUNT_SYSTEM
#ifdef SERV_SKILL_USE_SUBQUEST
			m_iUseSkillCount		= 0;
#endif SERV_SKILL_USE_SUBQUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
			m_iLearnNewSkillCount	= 0;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
		}
	};

	struct ITEM_DATA
	{
		int		m_iItemID;
		int		m_iQuantity;
		int		m_iPeriod;
		//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		int		m_iSocketOption1;
#else
		short	m_sSocketOption1;
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}}

		//{{ 2012. 11. 04	�ڼ���	����Ʈ ���� ������ ��ȭ ���� ����
#ifdef SERV_QUEST_REWARD_ITEM_ENCHANT
		char	m_cEnchantLevel;
#endif SERV_QUEST_REWARD_ITEM_ENCHANT
		//}}

		ITEM_DATA()
			: m_iItemID( 0 )
			//{{ 2012. 11. 04	�ڼ���	����Ʈ ���� ������ ��ȭ ���� ����
#ifdef SERV_QUEST_REWARD_ITEM_ENCHANT
			, m_cEnchantLevel( 0 )
#endif SERV_QUEST_REWARD_ITEM_ENCHANT
			//}}
		{
		}
	};

	//����
	struct Reward
	{
		bool				m_bEnable;
		int					m_iEXP;
		int					m_iED;
		std::wstring		m_wstrName;	//���� Īȣ
		int					m_iBuff;	//���� ����
		int					m_iSP;
		CXSLUnit::UNIT_CLASS 	m_eChangeUnitClass;
		std::vector<ITEM_DATA>	m_vecItem;

		Reward()
		{
			m_bEnable	= true;
			m_iEXP		= 0;
			m_iED		= 0;
			m_iBuff		= 0;
			m_iSP		= 0;
			m_eChangeUnitClass = CXSLUnit::UC_NONE;			
		}
		~Reward()
		{
			m_vecItem.clear();
		}
	};

	//���� ����
	struct SelectReward
	{
		int						m_iSelectionCount;
		std::vector<ITEM_DATA>	m_vecSelectItem;

		~SelectReward()
		{
			m_vecSelectItem.clear();
			m_iSelectionCount = 0;
		}
	};

	//Sub Quest ID�� DB/Client�� ��� ����Ʈ ������ ������ �Ѵ�.
	struct SubQuestTemplet
	{
		int					m_iID;

		std::wstring		m_wstrDescription;
		SUB_QUEST_TYPE		m_eClearType;
		ClearCondition		m_ClearCondition;
		bool				m_bAutomaticDescription;

		SubQuestTemplet()
		{
			m_iID			= -1;
			m_eClearType	= SQT_NONE;
			m_bAutomaticDescription = true;
		}
	};

	struct QuestTemplet
	{
		int								m_iID;

		QUEST_TYPE						m_eQuestType;


		CXSLUnitManager::NPC_UNIT_ID	m_eStartNPCID;
		CXSLUnitManager::NPC_UNIT_ID	m_eEndNPCID;
		std::wstring					m_wstrTitle;
		int								m_iFairLevel;
		//{{ 2010. 04. 01  ������	��д��� ����
#ifdef SERV_DAILY_QUEST
		QUEST_REPEAT_TYPE				m_eRepeatType;
#else
		bool							m_bRepeat;
#endif SERV_DAILY_QUEST
		//}}

		//{{ 2010. 01. 29  ������	��������Ʈ
		bool							m_bIsPcBang;
		//}}
		//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
		bool							m_bIsTimeEvent;
		//}}
		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		std::map< int, int >			m_mapAcceptRewardItem;		// ����Ʈ ������ ����Ǵ� ������
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

		//std::wstring					m_wstrDissConditionText;	//���� ������ ���� ���� ���� NPC ���
		//std::wstring					m_wstrGreetingText;			//NPC ù�λ縻
		std::wstring					m_wstrMainText;				//NPC ù�λ縻�� �����ϸ� ������ �����
		std::wstring					m_wstrThanksText;			//Ŭ���� ������ �������� ���� ���
		std::wstring					m_wstrDissClearText;		//Ŭ���� ������ ������Ű�� ������ ���� ���
		//std::wstring					m_wstrQuestImage;			//QUEST �̹��� ����

		QuestCondition					m_Condition;
		
		std::vector<int>				m_vecSubQuest;
		
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		std::map< int, std::vector<int> >		m_mapSubQuestGroup;
#endif SERV_REFORM_QUEST
		//}}

		Reward							m_Reward;
		SelectReward					m_SelectReward;

#ifdef SERV_EPIC_QUEST
		int								m_iEpisodeGroupID;
		std::wstring					m_wstrEpisodeGroupTitle;
		int								m_iPlayLevel;
		std::wstring					m_wstrStartScene;
		std::wstring					m_wstrEndScene;
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		std::vector<int>				m_vecAfterQuestID;
#else
		int								m_iAfterQuestID;
#endif SERV_REFORM_QUEST
		//}} 
#endif SERV_EPIC_QUEST

		std::set< int >					m_setHideItemID;

		//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
		int								m_iRandomGroupID;
		float							m_fRandomRate;
#endif SERV_RANDOM_DAY_QUEST
		//}}

		//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		bool							m_bIsAccountQuest;	// ���� ����Ʈ �ΰ�?
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
#ifdef SERV_NAVER_CHANNELING
        KNexonAccountInfo::CHANNELING_ENUM    m_ePublisher; // ä�θ� ���� ����Ʈ
#endif SERV_NAVER_CHANNELING

		QuestTemplet()
		{
			m_iID			= -1;
			m_eStartNPCID	= CXSLUnitManager::NUI_NONE;
			m_eEndNPCID		= CXSLUnitManager::NUI_NONE;
			m_iFairLevel	= 0;
			//{{ 2010. 04. 01  ������	��д��� ����
#ifdef SERV_DAILY_QUEST
			m_eRepeatType	= QRT_NORMAL;
#else
			m_bRepeat		= false;
#endif SERV_DAILY_QUEST
			//}}

#ifdef SERV_EPIC_QUEST
			m_iEpisodeGroupID	= 0;
			m_iPlayLevel		= 0;
			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
#else
			m_iAfterQuestID		= 0;
#endif SERV_REFORM_QUEST
			//}} 
#endif SERV_EPIC_QUEST
			//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
			m_iRandomGroupID		= 0;
			m_fRandomRate			= 0.f;
#endif SERV_RANDOM_DAY_QUEST
			//}}
			//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
			m_bIsAccountQuest		= false;	// ���� ����Ʈ �ΰ�?
#endif SERV_ACCOUNT_MISSION_SYSTEM
			//}}
#ifdef SERV_NAVER_CHANNELING
            m_ePublisher = KNexonAccountInfo::CE_NEXON_ACCOUNT;
#endif SERV_NAVER_CHANNELING

		}
		
		~QuestTemplet()
		{
			m_vecSubQuest.clear();
		}
	};

	//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	struct AccountQuestDateInfo
	{
		CTime	m_tBegin;
		CTime	m_tEnd;
		int		m_iDayOfWeek;			// ���� ����

		AccountQuestDateInfo()
		{
			m_tBegin		= CTime( 0 );
			m_tEnd			= CTime( 0 );
			m_iDayOfWeek	= 0;

		}
		~AccountQuestDateInfo()
		{
		}
	};
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}
	

public:
	CXSLQuestManager(void);
	virtual ~CXSLQuestManager(void);

	// for lua	
	bool	AddQuestTemplet_LUA();
	bool	AddSubQuestTemplet_LUA();

	const QuestTemplet*		GetQuestTemplet( int iQuestID );
	const SubQuestTemplet*	GetSubQuestTemplet( int iSubQuestID );

	bool	GetQuestConditionItemList( int iQuestID, std::map< int, int >& mapItem );
	bool	IsSubItemCondition( int iQuestID );		//�ش� ����Ʈ�� �����׸��� ������ ������ �ִ°�..?

#ifdef SERV_EPIC_QUEST
	bool	IsEpicQuest( IN int iQuestID );
	void	GetEpicQuestTempletList( OUT std::vector<QuestTemplet*> &vecEpicQuestTemplet );
#endif SERV_EPIC_QUEST
	//{{ 2011. 07. 09	������	���� ����Ʈ üũ
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	void	GetExistBeforeEpicQuestTempletList( OUT std::vector<QuestTemplet*> &vecEpicQuestTemplet );
#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}
	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	bool	SelectTodayRandomQuest( IN bool bIsFirst );
	int		GetTodayRandomQuest( IN int iRandomGroupID );
	void	SetRandomQuestGroupList( IN std::map< int, int >& RandomQuest )			{ m_mapTodayRandomQuest = RandomQuest; }
	void	GetRandomQuestGroupList( OUT std::map< int, int >& RandomQuest )		{ RandomQuest = m_mapTodayRandomQuest; }
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	void	GetQuestListByType( IN QUEST_TYPE eType, OUT std::map< int, QuestTemplet >& mapQuestTemplet );
	bool	CheckValidDungeonID( IN int iSubQuestID );
	bool	CheckValidVillageMapID( IN int iSubQuestID );
	bool	CheckValidBattleFiledID( IN int iSubQuestID );
	
	bool	IsExistDungeonInSubQuest( IN int iSubQuestID, IN const int& iDungeonID, IN bool bUpperDifficulty );
	bool	IsExistVillageInSubQuest( IN int iSubQuestID, IN const int& iMapID );
	bool	IsExistBattleFiledIInSubQuest( IN int iSubQuestID, IN const int& iMapID );

	bool	IsEpicAndNormalQuest( IN int iQuestID );
#endif SERV_REFORM_QUEST
	//}} 

	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	const QuestTemplet*		GetAccountQuestTemplet( int iQuestID );
	void	GetAccountQuestTempletList( OUT std::vector<QuestTemplet*> &vecAccountQuestTemplet );
	bool	AddAccountQuestTemplet_LUA();
	bool	IsAccountQuest( IN const int iQuestID ) { return ( m_mapAccountQuestTemplet.find( iQuestID ) != m_mapAccountQuestTemplet.end() ); }
	//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	bool	AddAccountQuestDate_LUA();
	bool	IsValidAccountQuestDate( IN int iAccountQuestID, IN CTime& tNowTime );
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}
	

private:
	bool	LoadQuestCondition( KLuaManager& luaManager, QuestCondition& questCondition );
	bool	LoadReward( KLuaManager& luaManager, const WCHAR* pTableName, Reward& reward );
	bool	LoadClearCondition( KLuaManager& luaManager, SubQuestTemplet& pSubQuestTemplet );

private:
	std::map< int, QuestTemplet >		m_mapQuestTemplet;
	std::map< int, SubQuestTemplet >	m_mapSubQuestTemplet;

#ifdef SERV_EPIC_QUEST
	std::vector< QuestTemplet* >		m_vecEpicQuestTemplet;
#endif SERV_EPIC_QUEST
	//{{ 2011. 07. 09	������	���� ����Ʈ üũ
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	std::vector< QuestTemplet* >		m_vecExistBeforeEpicQuestTemplet;
#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}
	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	CTime											m_tLastSelectTime;
	std::map< int, std::vector< int > >				m_mapRandomQuest;			//[RandomGroupID][vecQuestID]
	std::map< int, int >							m_mapTodayRandomQuest;		//[RandomGroupID][QuestID]
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	std::map< int, QuestTemplet >					m_mapQuestTempletByType[QT_END];		// ����Ʈ Ÿ�Ժ� ����Ʈ ���ø� ���
#endif SERV_REFORM_QUEST
	//}}
	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	std::map< int, QuestTemplet >					m_mapAccountQuestTemplet;		// ���� ����Ʈ���ø� ���
	std::vector< QuestTemplet* >					m_vecAccountQuestTemplet;
	//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	std::map< int, AccountQuestDateInfo >			m_mapAccountQuestDayOfWeekInfo;				// ���� quest �� ����, �ð� ����  [account quest id, date info]
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

};

DefRefreshSingletonInline( CXSLQuestManager );

