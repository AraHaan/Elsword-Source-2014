#pragma once

#include "..\Enum\DungeonEnum.h"
#include "XSLDungeonStage.h"

#define DUNGEON_DEFAULT_LIMIT_PLAY_TIME 19999.0f

#define CASE_DEFENCE_DUNGEON \
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_NORMAL:	\
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_HARD:	\
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_EXPERT:	\

class CXSLDungeon
{
public:
	enum DIFFICULTY_LEVEL
	{
		DL_NORMAL  = 0,
		DL_HARD,
		DL_EXPERT,

		DL_NUM,
	};

	//{{ 2010. 03. 18  ������	��д��� ����	
	enum DUNGEON_TYPE
	{
		DT_INVALID = 0,
		DT_NORMAL, 
		DT_HENIR,
		DT_SECRET,
		//{{ 2013. 04. 15  ��μ�	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
		DT_DEFENCE_DUNGEON,
//#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

		//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		DT_NUM,
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}
	};
	//}}

	//{{ 2010. 03. 18  ������	��д��� ����	
	enum DUNGEON_MODE
	{
		DM_INVALID = 0,
		DM_HENIR_PRACTICE,
		DM_HENIR_CHALLENGE,
		DM_SECRET_NORMAL,
		DM_SECRET_HELL,
	};
	//}}

	//{{ 2010. 07. 01  ������	������ ����
	enum DUNGEON_ENDURANCE
	{
		DE_LEAVE_ROOM_ENDURANCE_DAMAGE = 2000,
	};
	//}}

	//{{ 2010. 11. 8	������	�ڵ� ����	
	enum RANK_TYPE
	{
		RT_NONE		= 0,
		RT_F,
		RT_E,
		RT_D,
		RT_C,
		RT_B,
		RT_A,
		RT_S,
		//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
		RT_SS,
#endif SERV_DUNGEON_RANK_NEW
		//}}
	};
	//}}

	//{{ 2012. 05. 14	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
	enum DUNGEON_ENUM
	{
		DE_CALC_AVERAGE_INCLUDE_LEVEL_GAP = 5,
	};
#endif SERV_DUNGEON_NPC_LEVEL
	//}}

	struct DungeonData
	{
		SEnum::DUNGEON_ID		m_DungeonID;
		std::wstring			m_DungeonName;
		std::wstring			m_DataFileName;

		SEnum::DUNGEON_ID		m_RequireDungeonID;
		int						m_RequireItemID;
		int						m_RequireItemCount;
#ifdef SERV_ADDITIONAL_TICKET
		std::vector< std::pair< int, int > > m_vecAdditionalTicket;
#endif SERV_ADDITIONAL_TICKET

		int				m_RequireItemID2;
		int				m_RequireItemCount2;

		int				m_RequireSpirit;

		//{{ 2012. 05. 22	������	���� ���� ������ ����
#ifdef SERV_DUNGEON_REQUIRE_ITEM_LEVEL
		int				m_RequireItemLevel;
#endif SERV_DUNGEON_REQUIRE_ITEM_LEVEL
		//}}

		int				m_MinLevel;
		int				m_MaxLevel;

		//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_DUNGEON_NPC_LEVEL
		int				m_NpcLevel;
#endif SERV_DUNGEON_NPC_LEVEL
		//}}

		DIFFICULTY_LEVEL m_eDifficulty;

		bool			m_bNormalOnly;	// �븻 ���̵��� �� �� ����
		float			m_fTimeLimit;
		//{{ 2010. 04. 26  ������	��д��� ����
#ifdef SERV_SECRET_HELL
		float			m_fPoisonTimeLimit;
		float			m_fExtraStagePoisonTimeLimit;
#endif SERV_SECRET_HELL
		//}}
#ifdef SERV_DUNGEON_OPTION_IN_LUA
		bool			m_bLevelEqualized;
		bool			m_bDamageEqualized;
		bool			m_bEventDungeon;
		unsigned short	m_usFixedMembers;
#endif SERV_DUNGEON_OPTION_IN_LUA

		DUNGEON_TYPE	m_eDungeonType;
		bool			m_bRelativeMonsterLevel;

		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		int				m_iDefenseDungeonClearEXP;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

#ifdef SERV_NEW_EVENT_TYPES
		bool			m_bSwitchingWithEventInfo;
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		int				m_iLimitedPlayTimes;
		int				m_iLimitedClearTimes;
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
	};

public:
	CXSLDungeon( const DungeonData& dungeonData );
	~CXSLDungeon();

	bool	OpenScriptFile();

	const CXSLDungeon::DungeonData& GetDungeonData() const;

	//////////////////////////////////////////////////////////////////////////
	//BEGIN : SERVER ������ ���Ǵ� �Լ�
	void	Clear();

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	int		GetNPCDropTime( IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID ) const;
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// ���볯¥: 2013-08-13
	float	GetNPCExpRate( IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID ) const;
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

	//{{ 2012. 05. 10	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
	bool	GetNPCData( IN const int iStageIndex, IN const int iRelativeMonsterLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage ) const;
#else
	bool	GetNPCData( IN const int iStageIndex, IN const int iHighestUserLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage ) const;
#endif SERV_DUNGEON_NPC_LEVEL
	//}}	

	int		GetStartPosByRandom( IN int iStageIndex, IN int iSubStageIndex ) const;
	//{{ 2010. 04. 26  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	void	GetExtraNpcInfo( IN int iStageIndex, IN int iSubStageIndex, OUT int& iStartPos, OUT bool& bIsRight ) const;
#endif SERV_SECRET_HELL
	//}}
	int		GetSubStageIndexByRandom( IN int iStageIndex );

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
	int		GetSecretStageEnteringEvent( IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex ) const;
	bool	GetNextStage( OUT CXSLDungeonSubStage::NextStageData& kNextStageData, IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex, IN int iSecretPadIndex ) const;
#endif // SERV_STAGE_CLEAR_IN_SERVER

	//{{ 2010. 03. 24  ������	��д��� ����
	CXSLDungeonStage::STAGE_TYPE GetStageType( IN u_int uiStageIndex ) const;
	//}}

	//{{ 2010. 11. 8	������	�ڵ� ����	
	static const wchar_t* GetRankString( IN RANK_TYPE eRankType );
	//}}

	static bool	IsTutorialDungeon( IN int iDungeonID );
	static bool	IsTCDungeon( IN int iDungeonID );
	static bool IsArcadeDungeon( IN int iDungeonID );
	static bool IsArcadeSeasonDungeon( IN int iDungeonID );
	static bool IsRubenDungeon( IN int iDungeonID );
	static bool IsElderDungeon( IN int iDungeonID );
	static bool IsBesmaDungeon( IN int iDungeonID );
	static bool IsAlteraDungeon( IN int iDungeonID );
	static bool IsPeitaDungeon( IN int iDungeonID );
	static bool IsVelderDungeon( IN int iDungeonID );
	static bool IsHamelDungeon( IN int iDungeonID );
	//{{ 2012. 10. 29	�ڼ���	���� ���� �߰�
#ifdef SERV_VILLAGE_SANDER
	static bool IsSanderDungeon( IN int iDungeonID );
#endif SERV_VILLAGE_SANDER
	//}}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	static bool IsFixedMembers( IN int iDungeonID );
	static short GetFixedMembers( IN int iDungeonID );
#endif SERV_DUNGEON_OPTION_IN_LUA

	//{{ 2009. 4. 24  ������	�̺�Ʈ ����
	static bool IsEventDungeon( IN int iDungeonID );
	//}}
	//{{ 2009. 7. 26  ������	��ϸ� �ð�
	static bool IsHenirDungeon( IN int iDungeonID );
	//}}
	static bool IsSinglePlayDungeon( IN int iDungeonID );
    //{{ 2009. 3. 3  ������		������� ����
	static bool IsItemDropDungeon( IN int iItemID, IN int iDungeonID );
	//}}
	//{{ 2010. 03. 18  ������	��д��� ����
	static bool IsValidDungeonMode( DUNGEON_MODE eDungeonMode );
	//}}	
	//{{ 2009. 12. 18  ������	������ Ȯ��
	static bool IsScriptCheckDungeon( IN int iDungeonID );
	//}}
	//{{ 2010. 03. 25  ������	��д��� ����
	static bool IsSecretDungeon( IN int iDungeonID );
	//}}
	//{{ 2010. 12. 28	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
	static bool IsShipDungeon( IN int iDungeonID );
#endif SERV_SHIP_DUNGEON
	//}}
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	static bool IsDefenceDungeon( IN int iDungeonID );
	int	GetDefeseDungeonClearEXP()						{ return m_sDungeonData.m_iDefenseDungeonClearEXP; }
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	static bool IsValentineDungeon( IN int iDungeonID );
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	static bool IsValentineDungeonInt( IN int iDungeonID );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 //2013.10.14
	static bool IsHalloweenDungeon( IN int iDungeonID );
#endif //SERV_HALLOWEEN_EVENT_2013

	//{{ 2010. 04. 30  ������	��д��� ����
#ifdef SERV_SECRET_HELL 
	static float CheckDungeonBalRateByNpcLv( IN const int iUnitLevel, IN const int iNpcLv );
#endif SERV_SECRET_HELL
	//}}
	//{{ 2012. 06. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	static float GetMonsterLevelBalanceRate( IN const int iUnitLevel, IN const int iNpcLv );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
	static bool IsHenirChallengeMode( IN int iDungeonModeID );
	static bool IsHenirPracticeMode( IN int iDungeonModeID );
#endif SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
	//END :
	//////////////////////////////////////////////////////////////////////////



private:
	DungeonData										m_sDungeonData;
	std::vector< CXSLDungeonStage::StageData* >		m_vecStageData;
};
