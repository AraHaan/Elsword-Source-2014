#pragma once

#include "RefreshSingleton.h"
#include "XSLBattleField.h"


class CXSLBattleFieldManager
{
	DeclareRefreshSingleton( CXSLBattleFieldManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum DEFAULT_ENUM
	{
        DE_MONSTER_RESPAWN_FACTOR_BY_USER_COUNT = 3,
		DE_MONSTER_RESPAWN_TIME_MIN = 3,
		DE_MONSTER_RESPAWN_TIME_MAX = 9,
	};

	struct SEliteMonsterDropInfo
	{
		char							m_cDifficultyLevel;
		int								m_iMaxMonsterCount;

		SEliteMonsterDropInfo()
		{
			m_cDifficultyLevel = 0;
            m_iMaxMonsterCount = 0;
		}
	};

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	typedef std::pair< int, int >	KFieldMiddleBossDropKey;

	struct KFieldMiddleBossDropInfo
	{
		int m_iMiddleBossCommonBonusItemID;
		int m_iMiddleBossCommonBonusItemCount;
		int m_iMiddleBossBonusItemID;
		int m_iMiddleBossBonusItemIDCount_High;
		int m_iMiddleBossBonusItemIDCount_Low;

		KFieldMiddleBossDropInfo()
		{
			m_iMiddleBossCommonBonusItemID		= 0;
			m_iMiddleBossCommonBonusItemCount	= 0;
			m_iMiddleBossBonusItemID			= 0;
			m_iMiddleBossBonusItemIDCount_High	= 0;
			m_iMiddleBossBonusItemIDCount_Low	= 0;
		}
	};
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	typedef std::pair< int, int >	KFieldEventBossDropKey;

	struct KFieldEventBossDropInfo
	{
		int m_iEventBossCommonBonusItemID;
		int m_iEventBossCommonBonusItemCount;
		int m_iEventBossBonusItemID;
		int m_iEventBossBonusItemIDCount_High;
		int m_iEventBossBonusItemIDCount_Low;

		KFieldEventBossDropInfo()
		{
			m_iEventBossCommonBonusItemID		= 0;
			m_iEventBossCommonBonusItemCount	= 0;
			m_iEventBossBonusItemID			= 0;
			m_iEventBossBonusItemIDCount_High	= 0;
			m_iEventBossBonusItemIDCount_Low	= 0;
		}
	};
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

public:
	~CXSLBattleFieldManager();
	CXSLBattleFieldManager();

	// for lua
	bool	OpenScriptFile_AllBattleFieldScriptLoad( IN lua_State* pLuaState );

	void	SetMonsterRespawnFactorByUserCount_LUA( int iFactor );
	void	SetMonsterRespawnTimeMinMax_LUA( int iMin, int iMax );
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	void	SetBattleFieldEDFactor_LUA( float fFactor );
	void	SetBattleFieldEXPFactor_LUA( float fFactor );
#else
	void	SetBattleFieldFactor_LUA( float fFactor );
#endif SERV_FIELD_ED_EXP_FACTOR
	//}
	void	AddEliteMonsterDropInfo_LUA( int iBeginUserCount, int iEndUserCount, char cDifficultyLevel, int iMaxMonsterCount );
	bool	AddBattleFieldData_LUA();

	void	SetDangerousValueMax_LUA( int iValue );
	void	SetDangerousValueWarning_LUA( int iValue );
	void	SetBossCheckUserCount_LUA( int iValue );
	void	SetEliteMonsterDropValue_LUA( int iValue );

	//{{ 2013. 03. 07	��Ʋ �ʵ� ���赵 �̺�Ʈ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	void	SetDangerousValueEventRate_LUA( int iValue );
	int		GetDangerousValueEventRate() { return m_iDangerousValueEventRate; }
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	//}

	// function
	bool	IsExistBattleFieldID( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID ) const		{ return ( m_mapBattleField.find( eBattleFieldID ) != m_mapBattleField.end() ); }

	// create
	bool	GetCreateMonsterList( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
								  IN const int iPlayerCount,
								  OUT KNPCList& kBattleFieldNpcList );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	bool	GetCreateBossMonsterList( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, OUT KNPCList& kBattleFieldNpcList );
#endif // SERV_BATTLE_FIELD_BOSS

	// respawn
	bool	GetRespawnMonsterList( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
								   IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterCountInfo,
								   IN const std::map< int, int >& mapRespawnReadyNpcCount,
								   OUT KNPCList& kBattleFieldNpcList );

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	float	GetMiddleBossMonsterDropRate( IN const int iCurDangerousValue, IN const int iOldDangerousValue );
	bool	GetBattieFieldMiddleBossMonsterInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
												 IN const int iDangerousValue,
												 OUT std::vector<KNPCUnitReq>& vecNpcInfo );

	void	SetBattleFieldMiddleBossDropInfo_LUA();
	bool	GetBattleFieldMiddleBossDropInfo( IN int iFieldID, IN int iNpcID, OUT KFieldMiddleBossDropInfo& kDropInfo );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	float	GetEventBossMonsterDropRate( IN const int iCurEventDangerousValue, IN const int iOldEventDangerousValue );
	bool	GetBattieFieldEventBossMonsterInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
												IN const int iEventDangerousValue,
												OUT std::vector<KNPCUnitReq>& vecNpcInfo );

	void	SetBattleFieldEventBossDropInfo_LUA();
	bool	GetBattleFieldEventBossDropInfo( IN int iFieldID, IN int iNpcID, OUT KFieldEventBossDropInfo& kDropInfo );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	bool	GetBattieFieldBossMonsterInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
										   IN const int iDangerousValue,
										   OUT KNPCUnitReq& kNpcInfo );

	bool	CheckPossibleEliteMonsterDrop( IN const int iPlayerCount, 
										   IN const int iCurrentEliteMonsterCount,
										   OUT char& cDifficultyLevelForEliteMonsterDrop );

	// get
	int		GetMaxMonsterCountInBattleField( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount ) const;
	int		GetMaxJoinUserCountInBattleField( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID ) const;
	int		GetMonsterRespawnFactorByUserCount() const		{ return m_iMonsterRespawnFactorByUserCount; }
	double	GetMonsterRespawnTimeByRandom() const;
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	float	GetBattleFieldEDFactor() const					{ return m_fBattleFieldEDFactor; }
	float	GetBattleFieldEXPFactor() const					{ return m_fBattleFieldEXPFactor; }
#else
	float	GetBattleFieldFactor() const					{ return m_fBattleFieldFactor; }
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	int		GetDangerousValueMax() const					{ return m_iDangerousValueMax; }
	int		GetDangerousValueWarning() const				{ return m_iDangerousValueWarning; }
	int		GetBossCheckUserCount() const					{ return m_iBossCheckUserCount; }
	int		GetEliteMonsterDropValue() const				{ return m_iEliteMonsterDropValue; }

	float	GetBossMonsterDropRate( IN const int iCurDangerousValue );

	int		GetStandardMonsterLevel( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID );

	// ���� ���� üũ
	bool	IsCanEnterBattleField( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
								   IN const u_char ucLevel, 
								   IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, 
								   OUT int& iRequireLevel, 
								   OUT int& iRequireDungeonID ) const;

	//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int		GetEnterVillageMapID( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID ) const;
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}

	//{{ 2013. 1. 1	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	bool	GetEventMonsterCreateInfo( IN const SEnum::BATTLE_FIELD_ID	eBattleFieldID, OUT KNPCUnitReq& kNpcInfo );
#endif SERV_FIELD_EVENT_MONSTER
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
	bool	IsBossFieldID( IN const int iVillageMapID ) const	{	return ( m_setBossFieldID.find( iVillageMapID ) != m_setBossFieldID.end() ); }
	bool	GetRandomBossFieldID( OUT int& iVillageMapID ) const;
#endif // SERV_BATTLE_FIELD_BOSS
 
private:
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >		m_mapBattleField;					// ��Ʋ�ʵ� ����
	int														m_iMonsterRespawnFactorByUserCount;	// ��Ʋ�ʵ� ���� �ο��� ���� ������ �����
	int														m_iMonsterRespawnTimeMin;			// ���� ������ Ÿ�� �ּҰ�
	int														m_iMonsterRespawnTimeMax;			// ���� ������ Ÿ�� �ִ밪
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	float													m_fBattleFieldEDFactor;				// ��Ʋ�ʵ� ED �����
	float													m_fBattleFieldEXPFactor;				// ��Ʋ�ʵ� EXP �����
#else
	float													m_fBattleFieldFactor;				// ��Ʋ�ʵ� �����
#endif SERV_FIELD_ED_EXP_FACTOR
	//}
	std::map< int, SEliteMonsterDropInfo >					m_mapEliteMonsterDropInfo;			// ����Ʈ ���� ���� ����

	// ���赵 �ý���
	int														m_iDangerousValueMax;				// �ִ� ���赵
	int														m_iDangerousValueWarning;			// ��� ���赵
	int														m_iBossCheckUserCount;				// ������ ���� ������ �ּ� ���� ī��Ʈ
	int														m_iEliteMonsterDropValue;			// ����Ʈ ���� ���� �����
	//{{ 2013. 03. 07	��Ʋ �ʵ� ���赵 �̺�Ʈ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	int														m_iDangerousValueEventRate;			// ���赵 ����
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	//}

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::map< KFieldMiddleBossDropKey, KFieldMiddleBossDropInfo >				m_mapBattleFieldMiddleBossDropInfo;	// �ʵ� ���� ��� ����
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
	std::set<int>	m_setBossFieldID;
#endif // SERV_BATTLE_FIELD_BOSS
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	std::map< KFieldEventBossDropKey, KFieldEventBossDropInfo >				m_mapBattleFieldEventBossDropInfo;	// �ʵ� ���� ��� ����
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
};

DefRefreshSingletonInline( CXSLBattleFieldManager );



