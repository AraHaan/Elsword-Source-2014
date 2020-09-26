#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "x2data/XSLQuestManager.h"

//:::DB CONNECT TIME:::
//(�ǽð�)-UNIT SELECT�� �Ͽ� ���ӽ��۽� ������/�Ϸ� QUEST �޾ƿ���
//(�ǽð�)-QUEST �Ϸ�� ������ ���� => �Ϸ� UPDATE
//(�ǽð�)-���������(UNIT SELECT ����) ������ QUEST UPDATE
//-unit info update �ø��� ����Ʈ�� ������ update �ϱ�
//(�ǽð�)-����Ʈ ����� ������� db����

SmartPointer(KGSUser);

class KUserQuestManager
{
public:
	enum 
	{
		MAX_ING_QUEST_NUM = 30,
	};

public:
	KUserQuestManager(void);
	~KUserQuestManager(void);

	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	void			SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser );
#else
	void			SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< int >& vecCompleteQuest, IN KGSUserPtr spUser );
#endif SERV_DAILY_QUEST
	//}}	

	bool			AddQuest( IN KQuestInstance& kInfo );
	bool			RemoveQuest( IN int iQuestID );	
	
	//{{begin} 2010-06-07.hoons.Ŭ���̾�Ʈ�� �������̽��� ���߱�����.
	//{{ 2011. 01. 03	������	���� ����Ʈ
//#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	bool			Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool IsAfterQuest = true, IN const bool bSendAck = true );
#else
	bool			Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool bSendAck = true );
#endif SERV_REFORM_QUEST
	//}}
//#else
//	bool			Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser );
//#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}	

	void			Handler_EGS_GIVE_UP_QUEST_REQ( IN KEGS_GIVE_UP_QUEST_REQ& kReq, IN KGSUserPtr spUser );
	//{{ 2011. 08. 31  ��μ�	��д��� ����
//#ifdef SERV_RANDOM_DAY_QUEST
	void			Handler_ERM_NPC_UNIT_DIE_NOT( IN const int iDungeonID,
												IN const char cDifficulty,
												IN const int iMonsterID,
												IN KGSUserPtr spUser,
												IN const char cDungeonMode
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
												, IN const bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST
												);
//#else
//	void			Handler_ERM_NPC_UNIT_DIE_NOT( IN int iDungeonID, IN char cDifficulty, IN int iMonsterID, IN KGSUserPtr spUser );
//#endif SERV_RANDOM_DAY_QUEST
	//}}
	void			Handler_EGS_TALK_WITH_NPC_REQ( IN int iNPCID, IN KGSUserPtr spUser );
	
	void			Handler_OnPVPPlay( IN int iGameType
									 , IN KGSUserPtr spUser
									 //{{ 2012. 09. 18   ��μ�   ���� �÷��� ����Ʈ ���� ����
#ifdef SERV_PVP_PLAY_QUEST
									 , IN bool bOfficialMatch
#endif SERV_PVP_PLAY_QUEST
									 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
									 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
									 , IN bool bCouplePvp
									 , IN bool bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
									 , IN const bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST
									 );

	//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	void			Handler_OnPVPPlay_AccumulatedOfficialMatchCount( IN KGSUserPtr spUser, IN const bool bIncreaseCount );
#endif SERV_2012_PVP_SEASON2
	//}}

	void			Handler_OnPVPKill( IN int iGameType
									 , IN KGSUserPtr spUser
									 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
									 , IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
									 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
									 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
									 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
									 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
									 );

	void			Handler_OnPVPWin( IN int iGameType
									, IN KGSUserPtr spUser
									//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
									//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
									, IN bool bMatchNPC
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
									//}}
									, IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
									//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
									, IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
									);

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	void			Handler_OnEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iEnchantLevel );
	void			Handler_OnSocketItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iSocketUseCount );
	void			Handler_OnAttribEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const KItemAttributeEnchantInfo& kItemAttribEnchantInfo );
	void			Handler_OnResolveItem( IN KGSUserPtr spUser, IN const int iItemID );
	void			Handler_OnUseSkillPoint( IN KGSUserPtr spUser, IN const int iSkillID );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
	void			Handler_OnResolveItem( IN KGSUserPtr spUser );
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	void			Handler_OnEnterTheVillage( IN KGSUserPtr spUser );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	//{{ 2012. 07. 16	������		��Ʋ�ʵ� ���� ����Ʈ
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	void			Handler_OnEnterTheBattleField( IN KGSUserPtr spUser );
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	//}}
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	void			Handler_OnLearnNewSkill( IN KGSUserPtr spUser, IN std::vector<int> vecNowLearnSkill );
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

	void			Handler_EGS_QUEST_COMPLETE_REQ( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin = false );
	void			Handler_DBE_QUEST_COMPLETE_ACK( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser );
	//{{end} 2010-06-07.hoons.Ŭ���̾�Ʈ�� �������̽��� ���߱�����.

	bool			IsQuest( const IN int iQuestID );
	bool			IsChangeJobQuest();	//���� ���� ����Ʈ�� ���� ����Ʈ�� �ִ���..
	//{{ 2009. 8. 8  ������		����
	bool			IsCompleteQuest( const IN int iQuestID ) { return ( m_mapCompleteQuest.find( iQuestID ) != m_mapCompleteQuest.end() ); }
	//}}
	//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST
	bool			IsExistEventQuest( IN int iQuestID );
#endif SERV_DAY_QUEST
	//}}

	KQuestInstance*	GetQuestInstance( const IN int iQuestID );
	int				GetQuestCompleteCount( IN const int iQuestID );
#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
	bool			CheckCompletedQuestToday( IN const int iQuestID );
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX

	void			GetUpdatedQuestInfo( OUT std::vector< KQuestUpdate >& vecQuestData );

	//{{ 2007. 11. 1  ������  
	int				GetQuestRewardItemCount();
	//}}

	//���� ����Ǵ� ����Ʈ�� ����Ʈ �������� ������ ����Ʈ�� �Ѱ��ش�.
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	void			GetOngoingQuestForRoom( IN KGSUserPtr spUser, OUT std::map< int, KSubQuestInfo >& mapOngoingQuest );
#else
	void			GetDropQuestitembyIngQuest( OUT std::map< int, KSubQuestInfo >& mapDropQuestitembyIngQuest, IN KGSUserPtr spUser );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}	

	//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST
	void			InitEventQuest( IN KGSUserPtr spUser );
	void			CheckQuestEvent( IN KGSUserPtr spUser );
	void			GetReservedGiveUpQuest( OUT std::vector< int >& vecGiveUpQuest );
#endif SERV_DAY_QUEST
	//}}

	//{{ 2010. 04. 05  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	KCompleteQuestInfo* GetCompleteQuestInfo( IN int iQuestID );
#endif SERV_DAILY_QUEST
	//}}

#ifdef SERV_EPIC_QUEST
	void			CheckEpicQuest_SuccessStateChange( IN int iMapID, IN KGSUserPtr spUser );

	int				GetQuestingSize(){ return m_mapQuesting.size(); }
	int				GetCompleteQuestSize(){ return m_mapCompleteQuest.size(); }
#endif SERV_EPIC_QUEST

	//{{ 2011. 01. 03	������	���� ����Ʈ
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	void			CheckEpicQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser, IN const bool bSendAck );
	void			CheckEpicQuest_NewQuestByAfterQuest( IN const int iQuestID, IN KGSUserPtr spUser, IN const bool bSendAck, IN const bool bForce );
#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}

	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	void			Handler_OnDungeonClear(	IN const KGSUserPtr spUser, 
											IN const int iDungeonID, 
											IN const char cDifficulty, 
											IN const char cDungeonMode, 
											IN const char cRank, 
											IN const int iPlayTime, 
											IN const int iDamage, 
											IN const std::set< UidType >& setPartyMembers, 
											IN const std::map< int, KSubQuestInfo >& mapOngoingQuest,
											IN const std::map< UidType, bool >& mapHaveExpInDungeon, 
											IN const std::map< UidType, bool >&	mapSuitableLevelInfo
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT											
											,IN const bool bChar1
											,IN const bool bChar2
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
											,IN const std::vector<UidType> vecRelation
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
											, IN const bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST
#ifdef SERV_THREE_COLOR_EVENT
											,IN const std::set< int >& setEquippedTitle
#endif SERV_THREE_COLOR_EVENT
											);		// ���� ���� ��� ���� - ���� ���� �̾��°�?

	bool			IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo );
#else
	void			Handler_OnDungeonClear(	IN const KGSUserPtr spUser, 
											IN const int iDungeonID, 
											IN const char cDifficulty, 
											IN const char cDungeonMode, 
											IN const char cRank, 
											IN const int iPlayTime, 
											IN const int iDamage, 
											IN const std::set< UidType >& setPartyMembers, 
											IN const std::map< int, KSubQuestInfo >& mapOngoingQuest,
											IN const std::map< UidType, bool >& mapHaveExpInDungeon );
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}
	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
//#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	bool			ExistHaveExpInDungeon( IN UidType CharUID, IN const std::map< UidType, bool >& mapHaveExpInDungeon );
//#endif
	//}}

	//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	void GetUserAllQuest( OUT std::set< int >& mapPaymentQuest );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	void GetUserGoingQuest( KGSUserPtr spUser, OUT std::set< int >& mapPaymentQuest );
	bool CheckIsGoingComplete( IN const int iQuestID, IN KGSUserPtr spUser );
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_SUB_QUEST_USE_ITEM
	void CountUseItem( IN KGSUserPtr spUser, IN const int iDungeonID, IN const char cDifficulty, IN const int iItemID );
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_EVENT_LEVEL_UP_QUEST_CLEAR
	bool Handler_OnCheckLevelUpQuest(IN const KGSUserPtr spUser);
#endif SERV_EVENT_LEVEL_UP_QUEST_CLEAR
	//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void GetCompleteChangeClassQuest( OUT std::vector< KCompleteQuestInfo >& vecQuest );
	void GetInProgressClassChangeQuest( OUT std::vector< KQuestInstance >& vecQuest );
	void GetCompleteChangeClassQuest( OUT std::vector< int >& vecQuest );
	void GetInProgressClassChangeQuest( OUT std::vector< int >& vecQuest );
	void SetClassChangeQuest( IN UidType iUnitUID, IN std::map< int, int >& mapComplete, IN std::map< int, int >& mapInProgress );
	void SetClassChangeDeleteQuest( IN UidType iUnitUID, IN std::vector< int >& vecDeleteCompleteQuest, IN std::vector< int >& vecDeleteInProgressQuest );
	void GetClassChangeDeleteQuest( IN char cDownUnitClass, OUT std::vector< int >& vecCompleteDelete, OUT std::vector< int >& vecInProgressDelete );
	void GetInProgressQuest( OUT std::vector< int >& vecQuest );
	void GetCompleteQuest( OUT std::vector< int >& vecQuest );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	void ClearAutoCompleteQuest();
	void SetAutoCompleteQuest( IN OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ& kPacketReq );
	bool GetAndDeleteAutoCompleteQuest( OUT KEGS_QUEST_COMPLETE_REQ& kReq );
	void AddAutoCompleteQuestResult( IN const KEGS_QUEST_COMPLETE_ACK& kAck );
	void GetAutoCompleteQuestResult( OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK& kPacketAck );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	void CheckEventQuest( IN KGSUserPtr spUser );
	void CheckResetDayEventQuest_AutoAccept( IN KGSUserPtr spUser );
	void CheckLimitLevelQuest( IN KGSUserPtr spUser );
	void Handler_OnEnterTheDungeon( IN KGSUserPtr spUser );
	void Handler_OnFindNPC( IN KGSUserPtr spUser, std::vector< int >& vecNPCData, bool bDungeon );
	bool CheckCompleteSubQuest_BeforGroup( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser );
	void CheckEpicQuest_EnterDungeon( IN int iDungeonID, IN KGSUserPtr spUser );
	void CheckEpicQuest_EnterBattleField( IN int iMapID, IN KGSUserPtr spUser );
#endif SERV_REFORM_QUEST
	//}}

#ifdef SERV_POINT_COUNT_SYSTEM
	void			SetUpdateQuestInstance( IN KGSUserPtr spUser);
#endif //SERV_POINT_COUNT_SYSTEM

#ifdef SERV_SKILL_USE_SUBQUEST
	void			Handler_EGS_SKILL_USE_REQ( IN int iSkillID, IN KGSUserPtr spUser );
#endif SERV_SKILL_USE_SUBQUEST

protected:
	void			ClearUnitQuest();
	void			OnPVP( IN CXSLQuestManager::SUB_QUEST_TYPE eSubQuestType
						 , IN int iGameType
						 , IN KGSUserPtr spUser
						 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
						 , IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
						 //}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT
						 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
						 , IN int iPvpChannelClass = 0
						 , IN bool bCouplePvp = false
						 , IN bool bIsDrawn = false
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
						 , IN bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
						 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
						 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
						 );

#ifdef SERV_EPIC_QUEST
	int				GetNormalQuestSize();
#endif SERV_EPIC_QUEST

protected:
	std::map< int, KQuestInstance >		m_mapQuesting;				// �������� quest
	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	std::map< int, KCompleteQuestInfo > m_mapCompleteQuest;			// �Ϸ���   quest
#else
	std::map< int, int >				m_mapCompleteQuest;			// �Ϸ���   quest
#endif SERV_DAILY_QUEST
	//}}

	//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST
	std::map< int, KEventQuestInfo >	m_mapEventQuest;			// ���� ���� ������ �̺�Ʈ ����Ʈ
	std::vector< int >					m_vecReservedGiveUpQuest;	// ����� ����Ʈ ���� ����Ʈ
#endif SERV_DAY_QUEST
	//}}

	//{{ 2007. 11. 1  ������  ����Ʈ ���� ������ ȹ�� ī��Ʈ
	int									m_iRewardItemCount;
	//}}

	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	std::map< int, KEGS_QUEST_COMPLETE_REQ >	m_mapAutoQuestCompleteReq;
	std::vector< KEGS_QUEST_COMPLETE_ACK >		m_vecAutoQuestCompleteAck;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
};
