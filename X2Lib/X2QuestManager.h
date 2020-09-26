#pragma once


#include <set>

class CX2QuestManager
{
	public:
		enum QUEST_TYPE
		{
			QT_NORMAL = 0,
			QT_EVENT,
			QT_CHANGE_JOB,
			//{{ kimhc // 2009-08-03 // ��ų �߰�
			QT_SKILL,
			// kimhc // 2009-08-03 // ��ų �߰�

			//{{ kimhc // 2010-02-01 // PC�� �߰�
			QT_PCBANG,
			//}} kimhc // 2010-02-01 // PC�� �߰�
//#ifdef SERV_EPIC_QUEST
			QT_EPIC,
			QT_CONTEST,
//#endif SERV_EPIC_QUEST
			QT_CHARACTER,	// ���� // 2009-12-23 �߰�

			QT_END,
		};


	//{{ kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_DAILY_QUEST
		enum QUEST_REPEAT_TYPE
		{
			QRT_NORMAL,				// �Ϸ��ϸ� �ݺ� ���� �ʴ� ����Ʈ
			QRT_DAY,				// ���� ������ �ð��� ����Ʈ ���� �� �ִ� ����
			QRT_REPEAT,				// ����Ʈ�� �Ϸ��ϸ� �ٷ� �ٽ� �� �� �ִ� ����
		};
#endif SERV_DAILY_QUEST
	//}} kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)

		// fix!! ������ ���� ����Ʈ�� ���� ��Ÿ������ ���� �� �ִ� �� üũ�ϴ� ����� Ư�� ����Ʈ�� ������ �ְų� �Ϸ� ������ �ش� ����Ʈ�� ���� �� ���� �ϴ� ������� �ٲ���
		enum CHANGE_JOB_TYPE
		{
			CJT_JOB_NONE,

			CJT_JOB_FIRST_OLD,		// �� ���� ����Ʈ (�ҵ峪��Ʈ, ���̸�����, �������η�����, �ҵ�����Ŀ, �ڵ� ����ƽ, ǻ�� �����)
			CJT_JOB_SECOND_OLD,		// �� ���� ����Ʈ (��������Ʈ, ��ũ������, �Ĺ����, ��������Ŀ, �ڵ� ��Ű��ó, ���� �����)

			CJT_JOB_FIRST,			// �� ���� ����Ʈ (�ҵ峪��Ʈ, ���̸�����, �������η�����, �ҵ�����Ŀ, �ڵ� ����ƽ, ǻ�� �����)
			CJT_JOB_SECOND,			// �� ���� ����Ʈ (��������Ʈ, ��ũ������, �Ĺ����, ��������Ŀ, �ڵ� ��Ű��ó, ���� �����)
			CJT_JOB_THIRD,			// �� ���� ����Ʈ (�ý�����Ʈ, ... )

			CJT_JOB_END,
		};

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

		//����Ʈ�� �������ִ� ���� ���̵� Ÿ��
		enum QUEST_MAPID_TYPE
		{
			QMT_NONE	= 0,
			QMT_VILLAGE,
			QMT_DUNGEON,
		};



	public:
		struct QuestCondition
		{
			int						m_iLevel;
			CX2Unit::UNIT_TYPE		m_eUnitType;
			std::set< CX2Unit::UNIT_CLASS > m_setUnitClass;
			int						m_iConditionItemID;
			int						m_iConditionItemNum;
			int						m_iNotBeforeQuestID;	// �� ����Ʈ�� clear ������ ����Ʈ�� ���� �� ����
			std::vector<int>		m_vecBeforeQuestID;

#ifdef SERV_EPIC_QUEST
	#ifdef REFORM_QUEST			
			std::set<int>			m_setEnableVillage;		//# �����̵��� ��������Ʈ �ڵ������� ����			
			std::set<int>			m_setEnableDungeon;		//# ���� ����� ��������Ʈ �ڵ������� ����			
			std::set<int>			m_setEnableBattleField; //# ��Ʋ�ʵ� ����� ��������Ʈ �ڵ������� ����
			int						m_iLimitLevel;			//���� ���� ���� ����
	#else
			//# �����̵��� ��������Ʈ �ڵ������� ����
			int						m_iEnableVillage;
	#endif //REFORM_QUEST
#endif SERV_EPIC_QUEST


			QuestCondition()
			{
				m_iLevel				= -1;
				m_eUnitType				= CX2Unit::UT_NONE;
				m_iConditionItemID		= -1;
				m_iConditionItemNum		= -1;
				m_iNotBeforeQuestID		= -1;
#ifdef SERV_EPIC_QUEST
	#ifdef REFORM_QUEST
				m_iLimitLevel			= -1;		//���� ���� ���� ����
	#else
				m_iEnableVillage		= 0;
	#endif //REFORM_QUEST
#endif SERV_EPIC_QUEST
			}
		};

		struct ClearCondition
		{
			//{{ 2009. 8. 3  ������		����Ʈ����
			std::set< CX2UnitManager::NPC_UNIT_ID > m_setKillNPCID;
			//}}
			int								m_iKillNum;

			int								m_iCollectionItemID;
			int								m_iCollectionItemNum;

#ifdef REFORM_QUEST
			std::set<SEnum::DUNGEON_ID>	m_setDungeonID;
#else
			SEnum::DUNGEON_ID			m_eDungeonID;
			char							m_cDifficulty;
#endif REFORM_QUEST
			//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
			CX2Dungeon::DUNGEON_MODE		m_eDungeonMode;
#endif SERV_SECRET_HELL
			//}}
			//{{ 2009. 3. 31  ������	���̵� �̻�
			bool							m_bUpperDifficulty;
			//}}
			CX2DungeonRoom::RANK_TYPE		m_eDungeonRank;
			int								m_iDungeonClearTime;

			CX2UnitManager::NPC_UNIT_ID		m_eTalkNPCID;
			bool							m_bTalkNPC;

			CX2PVPRoom::PVP_GAME_TYPE		m_ePVPType;
			int								m_iPVPPlay;
			int								m_iPVPWin;
			int								m_iPVPKill;
			float							m_fQuestItemDropRate;
			int								m_iDungeonDamage;
			int								m_iDungeonClearCount;
			//{{ 2010. 10. 25	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
			int								m_iEnchantLevel;			
			int								m_iSocketCount;
			KItemAttributeEnchantInfo		m_kAttribEnchantInfo;
			CX2EnchantItem::ATTRIB_COUNT_TYPE	m_eAttribCountType;
			int								m_iResolveCount;
			int								m_iSkillID;
			int								m_iSpUseCount;
#endif SERV_QUEST_CLEAR_EXPAND
			//}}
#ifdef SERV_NEW_PVP_QUEST
			KPVPChannelInfo::PVP_CHANNEL_CLASS m_ePvpChannelClass;
#endif SERV_NEW_PVP_QUEST
			//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	#ifdef REFORM_QUEST
			std::set<int>					m_setVillageMapID;
			std::set<int>					m_setBattleFieldID;
			int								m_iFindNPCID;
	#else
			int								m_iVillageMapID;
			int								m_iBattleFieldID;
	#endif //REFORM_QUEST
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
			int								m_iCharLevel;
#endif //SERV_ACCOUNT_MISSION_SYSTEM

#ifdef SERV_EVENT_TITLE_SUBQUEST
		int								m_iUseTitleID;
#endif SERV_EVENT_TITLE_SUBQUEST

#ifdef SERV_SUB_QUEST_USE_ITEM
			std::vector< int >				m_vecUseItemID;
			int								m_iUseItemNum;
#endif SERV_SUB_QUEST_USE_ITEM

#ifdef SERV_POINT_COUNT_SYSTEM
		int								m_iPointCount;
#endif SERV_POINT_COUNT_SYSTEM

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

				m_iCollectionItemID		= -1;
				m_iCollectionItemNum	= 0;
#ifndef REFORM_QUEST
				m_eDungeonID			= SEnum::DI_NONE;
				m_cDifficulty			= -1;
#endif //REFORM_QUEST
				//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
				m_eDungeonMode			= CX2Dungeon::DM_INVALID;
#endif SERV_SECRET_HELL
				//}}
				//{{ 2009. 3. 31  ������	���̵� �̻�
				m_bUpperDifficulty		= false;
				//}}
				m_eDungeonRank			= CX2DungeonRoom::RT_NONE;
				m_iDungeonClearTime		= 0;

				m_eTalkNPCID			= CX2UnitManager::NUI_NONE;
				m_bTalkNPC				= false;

				m_ePVPType				= CX2PVPRoom::PGT_TEAM;
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
				m_eAttribCountType		= CX2EnchantItem::ACT_NONE;
				m_iResolveCount			= 0;
				m_iSkillID				= 0;
				m_iSpUseCount			= 0;
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
#ifdef SERV_NEW_PVP_QUEST
				m_ePvpChannelClass		= KPVPChannelInfo::PCC_NONE;
#endif SERV_NEW_PVP_QUEST
				//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	#ifdef REFORM_QUEST
				m_iFindNPCID			= 0;
	#else
				m_iVillageMapID			= 0;
				m_iBattleFieldID		= 0;
	#endif //REFORM_QUEST
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
				m_iCharLevel			= 0;
#endif //SERV_ACCOUNT_MISSION_SYSTEM
#ifdef SERV_EVENT_TITLE_SUBQUEST
				m_iUseTitleID           = -1;
#endif SERV_EVENT_TITLE_SUBQUEST
#ifdef SERV_SUB_QUEST_USE_ITEM
				m_iUseItemNum			= 0;
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_POINT_COUNT_SYSTEM
				m_iPointCount			= 0;
#endif SERV_POINT_COUNT_SYSTEM
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
			int		m_iSocketOption1;

			ITEM_DATA()
			{
				m_iItemID			= 0;
				m_iQuantity			= 0;
				m_iPeriod			= 0;
				m_iSocketOption1	= 0;
			}
		};

		//����
		struct Reward
		{
			bool					m_bEnable;
			int						m_iEXP;
			int						m_iED;
			std::wstring			m_wstrName;	//���� Īȣ
			int						m_iBuff;	//���� ����
			int						m_iSP;
			CX2Unit::UNIT_CLASS 	m_eChangeUnitClass;
			std::vector<ITEM_DATA>	m_vecItem;

			Reward()
			{
				m_bEnable	= true;
				m_iEXP		= 0;
				m_iED		= 0;
				m_iBuff		= 0;
				m_iSP		= 0;
				m_eChangeUnitClass = CX2Unit::UC_NONE;
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
				m_iSelectionCount = 0;
				m_vecSelectItem.clear();
			}
		};

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
				m_eClearType	= CX2QuestManager::SQT_NONE;
				m_bAutomaticDescription = true;
			}
		};

		struct QuestTemplet
		{
#ifdef ADD_SERVER_GROUP
			int								m_iServerGroupID;
#endif
			int								m_iID;

			QUEST_TYPE						m_eQuestType;

			CX2UnitManager::NPC_UNIT_ID		m_eStartNPCID;
			CX2UnitManager::NPC_UNIT_ID		m_eEndNPCID;
			std::wstring					m_wstrTitle;
			int								m_iFairLevel;

	//{{ kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef	SERV_DAILY_QUEST
			QUEST_REPEAT_TYPE				m_eRepeatType;
#else	SERV_DAILY_QUEST
			bool							m_bRepeat;
#endif	SERV_DAILY_QUEST
	//}} kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)

			//{{ 2010. 01. 29  ������	��������Ʈ
			bool							m_bIsPcBang;
			//}}
			//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
			bool							m_bIsTimeEvent;
			//}}

			std::wstring					m_wstrDissConditionText;	//���� ������ ���� ���� ���� NPC ���
			std::wstring					m_wstrGreetingText;			//NPC ù�λ縻
			std::wstring					m_wstrMainText;				//NPC ù�λ縻�� �����ϸ� ������ �����
			std::wstring					m_wstrThanksText;			//Ŭ���� ������ �������� ���� ���
			std::wstring					m_wstrDissClearText;		//Ŭ���� ������ ������Ű�� ������ ���� ���
			QuestCondition					m_Condition;
			std::vector<int>				m_vecSubQuest;
#ifdef REFORM_QUEST
			std::map< int, std::vector<int>>m_mapSubQuestGroup;			// ��������Ʈ�� �׷�ȭ <�ε���, ��������Ʈ ID>
#endif REFORM_QUEST

			Reward							m_Reward;					/// Ŭ���� �⺻ ���� (���� ���ص� ����)
			SelectReward					m_SelectReward;				/// �����ؼ� �޴� ���� (���� ���� ���� �ؾ� ��)

#ifdef SERV_EPIC_QUEST
			int								m_iEpisodeGroupID;
			std::wstring					m_wstrEpisodeGroupTitle;
			int								m_iPlayLevel;
			std::wstring					m_wstrStartScene;
			std::wstring					m_wstrEndScene;
			int								m_iAfterQuestID;
#endif SERV_EPIC_QUEST

			std::vector< int >										m_vecShowItemID;			// �ش� �������� �κ��丮(����,ĳ�ú����� ����)�� ���� �ϰ� ���� ���, �ش� ����Ʈ�� ���̰� �ϰ�, �������� ������ �������� ����Ʈ�� �ڵ����� ���� ��Ų��.
			std::vector< int >										m_vecHideItemID;			// �ش� �������� �κ��丮(����,ĳ�ú����� ����)�� ���� �ϰ� ���� ���, �ش� ����Ʈ�� ������ �ʰ� �ϰ�, �������� ����Ʈ�� �ڵ����� ���� ��Ų��.
			std::vector< int >										m_vecHideSkillID;			// �ش� ��ų�� ������ ���� ���
			std::vector<CX2Inventory::MEMBERSHIP_PRIVATE_BANK>		m_vecHideBankGrade;

#ifdef SHOW_DAILY_REPEAT_MARK_ON_EVENT_QUEST_FOR_DEVELOPER_FIX
			bool								m_bStartTimeEvent;
#endif //SHOW_DAILY_REPEAT_MARK_ON_EVENT_QUEST_FOR_DEVELOPER_FIX

#ifdef SERV_RANDOM_DAY_QUEST
			int								m_iRandomGroupID;
#endif SERV_RANDOM_DAY_QUEST
			int								m_iNextVillageID;		// ���� �̵� ���̵带 ���� ���� ���� ���̵�( ������ ��������Ʈ���� ����)


			QuestTemplet()
			{
#ifdef ADD_SERVER_GROUP
				m_iServerGroupID  = -1;
#endif
				m_iID			= -1;
				m_eQuestType	= QT_NORMAL;
				m_eStartNPCID	= CX2UnitManager::NUI_NONE;
				m_eEndNPCID		= CX2UnitManager::NUI_NONE;
				m_iFairLevel	= 0;
				//{{ kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef	SERV_DAILY_QUEST
				m_eRepeatType	= QRT_NORMAL;
#else	SERV_DAILY_QUEST
				m_bRepeat		= false;
#endif	SERV_DAILY_QUEST
				//}} kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)

				m_bIsPcBang		= false;
				m_bIsTimeEvent	= false;

				m_wstrDissConditionText = L"";
				m_wstrGreetingText		= L"";

#ifdef SERV_EPIC_QUEST
				m_iEpisodeGroupID	= 0;
				m_iPlayLevel		= 0;
				m_iAfterQuestID		= 0;
#endif SERV_EPIC_QUEST

#ifdef SHOW_DAILY_REPEAT_MARK_ON_EVENT_QUEST_FOR_DEVELOPER_FIX
				m_bStartTimeEvent = false;
#endif //SHOW_DAILY_REPEAT_MARK_ON_EVENT_QUEST_FOR_DEVELOPER_FIX

#ifdef SERV_RANDOM_DAY_QUEST
				m_iRandomGroupID	= 0;
#endif SERV_RANDOM_DAY_QUEST

				m_iNextVillageID	= 0;		// ���� �̵� ���̵带 ���� ���� ���� ���̵�( ������ ��������Ʈ���� ����)
			}
			~QuestTemplet()
			{
				m_vecSubQuest.clear();
#ifdef REFORM_QUEST
				m_mapSubQuestGroup.clear();
#endif //REFORM_QUEST
			}
		};

		struct SubQuestInst
		{
			int				m_iID;

			UCHAR			m_ucClearData;
			bool			m_bIsSuccess;

			SubQuestInst()
			{
				m_iID				= -1;
				m_ucClearData		= 0;
				m_bIsSuccess		= false;
			}
			void SetKSubQuestInstance( const KSubQuestInstance* pKSubQuestInstance )
			{
				m_iID				= pKSubQuestInstance->m_iID;
				m_ucClearData		= pKSubQuestInstance->m_ucClearData;
				m_bIsSuccess		= pKSubQuestInstance->m_bIsSuccess;
			}

			bool IsComplete() const;
		};

		struct QuestInst
		{
			int				m_iID;
			UidType			m_OwnorUnitUID;
			vector<SubQuestInst>	m_vecSubQuestInst;
			//bool			m_bIsEverySubQuestComplete;		/// ��� ��������Ʈ�� �Ϸ� �ߴ°�?
			
			QuestInst()
			{
				m_iID				= -1;
				m_OwnorUnitUID		= -1;
				m_vecSubQuestInst.clear();
				//m_bIsEverySubQuestComplete = false;
			}

			~QuestInst()
			{
				m_vecSubQuestInst.clear();
			}

// 			bool GetIsEverySubQuestComplete() const { return m_bIsEverySubQuestComplete; }
// 			void SetIsEverySubQuestComplete(bool val) { m_bIsEverySubQuestComplete = val; }

			bool IsComplete() const;
#ifdef SERV_EPIC_QUEST_HIDE_QUESTMARK_WHEN_SUBQUEST_COMPLETED
			bool HasNotCompletedSubQuest( int iSubQuestID ) const;
#endif SERV_EPIC_QUEST_HIDE_QUESTMARK_WHEN_SUBQUEST_COMPLETED

#ifdef REFORM_QUEST
			bool IsCompleteVisitQuest( int iSubQuestID_ ) const;
#endif //REFORM_QUEST

			void SetKQuestInstance( const KQuestInstance* pKQuestInstance )
			{
				m_iID				= pKQuestInstance->m_iID;
				m_OwnorUnitUID		= pKQuestInstance->m_OwnorUnitUID;

				m_vecSubQuestInst.resize(0);

				for( int i= 0; i < (int)pKQuestInstance->m_vecSubQuestInstance.size(); i++ )
				{
					SubQuestInst	subQuestInst;
					subQuestInst.SetKSubQuestInstance( &(pKQuestInstance->m_vecSubQuestInstance[i]) );
					m_vecSubQuestInst.push_back( subQuestInst );
				}
			}

			const CX2QuestManager::SubQuestInst* GetSubQuest( const int iSubQuestID ) const 
			{
				for( UINT i = 0; i<m_vecSubQuestInst.size(); i++ )
				{
					if( iSubQuestID == m_vecSubQuestInst[i].m_iID )
					{
						return &m_vecSubQuestInst[i];
					}
				}

				return NULL;
			}
		};

		struct ChangeJobQuest
		{
			std::set<int> m_setQuestID[CJT_JOB_END];
		};

	public:
		CX2QuestManager(void);
		virtual ~CX2QuestManager(void);

		bool	OpenScriptFile( const WCHAR* pFileName );

		bool	AddQuestTemplet_LUA();
		bool	AddSubQuestTemplet_LUA();
#ifdef LUA_TRANS_DEVIDE
		bool	AddQuestTempletTrans_LUA();
		bool	AddSubQuestTempletTrans_LUA();
#endif LUA_TRANS_DEVIDE
#ifdef DAY_OF_THE_WEEK_QUEST
		bool	AddAccountQuestDate_LUA();
#endif //DAY_OF_THE_WEEK_QUEST

		void	ClearUnitQuest();

//{{ kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef	SERV_DAILY_QUEST
		void	SetUnitQuest( const std::vector<KQuestInstance>& vecQuestInst, const std::vector<KCompleteQuestInfo>& vecCompleteQuest );
		__int64	GetTimeToRefreshQuest() const;	// ���� ����Ʈ�� ���� �Ǿ��ϴ� �ð��� ���� (���� AM 6�ÿ� ����)
		bool	RefreshDailyQuest();	// �Ϸ�����Ʈ ��Ͽ��� ��������Ʈ�� ������(��������Ʈ ����)
#else	SERV_DAILY_QUEST
		void	SetUnitQuest( const std::vector<KQuestInstance>& vecQuestInst, const std::vector<int>& vecCompleteQuest );
#endif	SERV_SECRET_HELL
//}} kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)
		

		bool	CreateUnitQuest( int iQuestID, const KQuestInstance& questInst );
		bool	UpdateUnitQuest( int iQuestID, const KQuestInstance& questInst );
		bool	RemoveUnitQuest( int questID, bool bComplete );

		const QuestTemplet*		GetQuestTemplet( int questID );
		const SubQuestTemplet*	GetSubQuestTemplet( int subQuestID );
		QuestInst*				GetUnitQuest( int iQuestID );			//�������� ����Ʈ
		bool					GetUnitCompleteQuest( int iQuestID );	//�Ϸ��� ����Ʈ Ȯ��

		bool					GetStartQuestList( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID ); // �ش� NPC�� �ִ� ����Ʈ ���
		bool					GetEndQuestList( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID ); // �ش� NPC�� �Ϸ����� �� �ִ� ����Ʈ ���
		bool					GetAvailableQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID ); // �ش� NPC�� �ִ� ����Ʈ �߿��� ���� ���� �� �ִ� ����Ʈ ���
		bool					GetCompletableQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID ); // ���� ������ �ִ� ����Ʈ ����߿��� �ش� NPC�� �Ϸ����� �� �ִ� ����Ʈ
		bool					GetOnGoingQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, std::vector<int>& vecQuestID );
#ifdef SERV_EPIC_QUEST
		bool					HasCompletableQuest();
		bool					GetBoardStartQuestList(std::vector<int>& vecQuestID );
		wstring					GetEpicGroupTitle( int iEpicGroupID);
		bool					CheckUpdateSubQuestInst( const int iQuestID,  const KSubQuestInstance& NewSubQuestInst);
		bool					GetCompletableQuestEpic( std::vector<int>& vecQuestID ); // ���� ������ �ִ� ����Ʈ ����߿��� �Ϸ��� �� �ִ� ����Ʈ
#endif SERV_EPIC_QUEST
		const ChangeJobQuest&	GetChangeJobQuest( CX2Unit::UNIT_CLASS eUnitClass )	{ return m_mapChangeJobQuest[ eUnitClass ]; }

		const map<int,QuestInst*>& GetUnitQuestMap() { return m_mapUnitQuest; }
		const map<int,int>& GetMapCompleteQuest() const { return m_mapCompleteQuest; }
		// 09.03.04 ���¿� : ����Ʈ ��ü�� ���� �� �� ���
		const map<int,QuestTemplet*>& GetMapQuestTemplet() { return m_mapQuestTemplet; }

		D3DXCOLOR GetQuestColor( int questID, int checkLevel );
		// 09.03.21 ���¿� : #CFFFFFF ������ �������� ��ȯ
		wstring GetQuestColorString( int questID, int checkLevel );
		wstring MakeSubQuestString( bool bAuto, const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet, const CX2QuestManager::QuestInst* pQuestInst, bool bShowCount = true );
		wstring LineBreakSubQuestString( int& nLineCount, wstring& wstrText, CKTDGFontManager::CUKFont* pFont, int iWidth );

		int		GetNewQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nRepeat, int &nNormal, int &nEvent, int checkLevel = 0);	// �ݺ� ����Ʈ
		void	GetDoQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nQuest );				// �������� ����Ʈ
		void	GetCompleteQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nRepeat, int &nNormal );			// �Ϲ� ����Ʈ �Ϸ�


		void UpdateQuest(bool val = true) { m_bUpdateQuest = val; }
		bool GetUpdateQuest() { return m_bUpdateQuest; }

		//{{ kimhc // �ǽð����� ����Ʈ�� �ʿ��� ������ ȹ�� �κ� üũ
		bool CheckItemCollection( int iItemID );		// �ǽð����� ����Ʈ�� �ʿ��� ������ ȹ�� �κ� üũ
		//}} kimhc // �ǽð����� ����Ʈ�� �ʿ��� ������ ȹ�� �κ� üũ
		
		//{{ kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#ifdef	SERV_DAY_QUEST
		void SetTimedEventQuest( int iQuestID, bool bValue );
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����


		bool IsForbiddenQuest( int iQuestID );
		void GiveUpForbiddenQuest();	
#ifdef SERV_RANDOM_DAY_QUEST
		void					SetRandomQuestInfo(std::map<int,int>& mapRandomQuestInfo);
		bool					CheckGiveUpRandomQuest( int iRandomGroupId, int iQuestID );
#endif SERV_RANDOM_DAY_QUEST	
		
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		void					CompleteQuestToVisitVillage();
		void					InsertCompletedVisitQuestID(QuestInst* pQuestInst);
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef QUEST_GUIDE
		bool GetNeedToTalkNPCList( OUT map<CX2UnitManager::NPC_UNIT_ID, bool>& mapGuideTarget_ );// ����Ʈ �ϷḦ ���� �湮�� �ʿ��� Village ID
		bool GetNeedToVisitVillageList( OUT map<int, bool>& mapGuideTarget_ );					// ����Ʈ �ϷḦ ���� �湮�� �ʿ��� Village ID
		bool GetNeedToVisitFieldList( OUT map<int, bool>& mapGuideTarget_ );					// ����Ʈ �ϷḦ ���� �湮�� �ʿ��� FIELD ID
		bool GetNeedToClearDungeonList( OUT map<SEnum::DUNGEON_ID, bool>& mapGuideTarget_ );	// ����Ʈ �ϷḦ ���� Ŭ���� �ؾ� �ϴ� Dungeon ID
		void GetCompleteTalkQuest( const CX2UnitManager::NPC_UNIT_ID eQuestNPCID, int &nNormal );// ��ȭ ����Ʈ �Ϸ�
		void SetHasDungeonQuest();
		bool GetHasDungeonQuest(){return m_bHasDungeonQuest;}
#ifdef DAY_OF_THE_WEEK_QUEST
		void ClearDayOfTheWeekQuest(){ m_mapDayOfTheWeekQuestList.clear(); }; // ���Ϻ� ����Ʈ ���� ����
		UINT GetQuestDayByQuestID( int iQuestID_ );
#endif //DAY_OF_THE_WEEK_QUEST
#endif //QUEST_GUIDE
#ifdef SERV_POINT_COUNT_SYSTEM
		void SetUpdataQuestInstance(std::map< int, KQuestInstance > mapQuestInstance);
#endif //SERV_POINT_COUNT_SYSTEM
		//{{ �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
#ifdef PRINT_INGAMEINFO_TO_EXCEL
		void PrintQuestInfo_ToExcel();
#endif PRINT_INGAMEINFO_TO_EXCEL
		//}} �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		static bool IsNewUserOnlyQuest( int iQuestID );
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

#ifdef SERV_SKILL_USE_SUBQUEST
		bool HasSkillUseSubQuest( const int& iSkillID );
#endif SERV_SKILL_USE_SUBQUEST
	private:
		bool	LoadQuestCondition( KLuaManager& luaManager, QuestCondition& questCondition );
		bool	LoadReward( KLuaManager& luaManager, const char* pTableName, Reward& reward );
		bool	LoadClearCondition( KLuaManager& luaManager, SubQuestTemplet* pSubQuestTemplet );


	private:
		map<int,QuestTemplet*>				m_mapQuestTemplet;
		map<int,SubQuestTemplet*>			m_mapSubQuestTemplet;

		map<int,QuestInst*>					m_mapUnitQuest;
		map<int,int>						m_mapCompleteQuest;

		map< CX2Unit::UNIT_CLASS, ChangeJobQuest > m_mapChangeJobQuest;

		bool								m_bUpdateQuest;
		
		//int									m_nString_Index;	//09. 05. 14 ������ ������ ���ؼ� �ε��� �д� �κ� ����

#ifdef SERV_EPIC_QUEST
		map<int, wstring>					m_mapEpicGroupTitle;
		
#endif SERV_EPIC_QUEST
#ifdef SERV_RANDOM_DAY_QUEST
		map< int, std::vector< int > >		m_mapRandomQuest;			//[RandomGroupID][vecQuestID]
		map<int, int>						m_mapTodayRandomQuest;
#endif SERV_RANDOM_DAY_QUEST

#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		vector<int>							m_vecCompletedQuestID;
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef QUEST_GUIDE
		bool								m_bHasDungeonQuest;
#endif //QUEST_GUIDE
#ifdef DAY_OF_THE_WEEK_QUEST
		// <����Ʈ ID, ���� >  ���� : 1~ 7 ( ��~ �� )
		map< int, UINT >					m_mapDayOfTheWeekQuestList; // ���Ϻ� ����Ʈ ���� ����
#endif //DAY_OF_THE_WEEK_QUEST
};
