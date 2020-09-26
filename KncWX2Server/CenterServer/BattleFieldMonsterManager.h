#pragma once
#include "RoomMonsterManager.h"
#include "X2Data/XSLBattleFieldManager.h"


//{{ 2011. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldMonsterManager : public KRoomMonsterManager
{
public:
	// ���� ���� ī��Ʈ
	struct SNpcDieCount
	{
		int		m_iNormalNpcDieCount;
		int		m_iLowEliteNpcDieCount;
		int		m_iHighEliteNpcDieCount;
		int		m_iMiddleBossDieCount;
		int		m_iBossDieCount;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		int		m_iEventBossDieCount;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

		SNpcDieCount()
		{
			Clear();
		}

		void Clear()
		{
			m_iNormalNpcDieCount	= 0;
			m_iLowEliteNpcDieCount	= 0;
			m_iHighEliteNpcDieCount	= 0;
			m_iMiddleBossDieCount		= 0;
			m_iBossDieCount				= 0;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			m_iEventBossDieCount		= 0;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
		}
	};

	// ���� ���� Ÿ�� ����
	typedef std::pair< int, int >	KSubStageKey;
	struct SUB_STAGE_NPC_INFO
	{
		std::set< int > m_setNormalNpcUID;
		std::set< int > m_setMiddleBossNpcUID;
		std::set< int > m_setBossNpcUID;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		std::set< int > m_setEventBossNpcUID;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	};

	// ���� ������ ���� ����
	struct SMonsterRespawnInfo
	{
		int				m_iMonsterGroupID;
		double			m_fRespawnTime;
		boost::timer	m_tTimer;

		SMonsterRespawnInfo()
		{
			m_iMonsterGroupID = 0;
			m_fRespawnTime = 0.0;
		}

		bool IsRespawnTimeOver() const
		{
            return ( m_tTimer.elapsed() > m_fRespawnTime );
		}
	};

	// �����ڸ� ���� ���� ������ ���� ����Ʈ
	struct SNpcInfoForClient
	{
		KNPCUnitReq								m_kNpcInfo;
		std::vector< KAttribEnchantNpcInfo >	m_vecAttribNpcInfo;
	};

public:
    KBattleFieldMonsterManager();
    virtual ~KBattleFieldMonsterManager();

	// ���� ����, ���� ó��
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-14	// �ڼ���
	void	StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue, IN const bool bBossField );
	bool	FirstCreateBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID );
	bool	IsAliveBossMonster( IN const int iNpcUID ) const;
#else // SERV_BATTLE_FIELD_BOSS
	void	StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue );
#endif // SERV_BATTLE_FIELD_BOSS
	void	EndGame();
	void	OnCloseRoom();

	// ���� �ű� ���� ó��
	bool	FirstCreateMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount );

	// ������ ó��
	bool	CheckRespawnMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
								 IN const int iPlayerCount, 
								 IN OUT KDangerousEventInfo& kDangerousEvent,
								 OUT KEGS_NPC_UNIT_CREATE_NOT& kResultNot );

	// ���� ���� ó��
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	bool	CheckMiddleBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
									IN const int iDangerousValue,
									IN OUT KDangerousEventInfo& kDangerousEvent,
									OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot );
	void	CreateMiddleBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetMiddleBossMonsterDie( IN const int iNpcUID );
	bool	IsMiddleBossMonster( IN const int iNpcUID ) const;
	bool	IsMiddleBossMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveMiddleBossList.find( iNpcUID ) != m_mapAliveMiddleBossList.end() ); }
	bool	IsRemainMiddleBoss()									{ return (m_mapAliveMiddleBossList.begin() != m_mapAliveMiddleBossList.end()); }
	bool	GetMiddleBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
	void	GetMiddleBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	bool	CheckEventBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
									IN const int iDangerousValue,
									IN OUT KDangerousEventInfo& kDangerousEvent,
									OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot );
	void	CreateEventBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetEventBossMonsterDie( IN const int iNpcUID );
	bool	IsEventBossMonster( IN const int iNpcUID ) const;
	bool	IsEventBossMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveEventBossList.find( iNpcUID ) != m_mapAliveEventBossList.end() ); }
	bool	IsRemainEventBoss()									{ return (m_mapAliveEventBossList.begin() != m_mapAliveEventBossList.end()); }
	bool	GetEventBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
	void	GetEventBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT


	void	GetAliveMonsterList( OUT std::vector< KNPCUnitReq >& vecNpcList, OUT std::map< int, KAttribEnchantNpcInfo >& mapAttirbNpcInfo ) const;
	int		GetAtStartedMonsterCount() const			{ return m_iAtStartedMonsterCount; }

	// ���� ó��
	bool	CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID );
	bool	SetAttribMonster( IN const int iNpcUID, IN const KAttribEnchantNpcInfo& kInfo );

	// ���� ���� ����
	void	IncreaseMonsterDieCount( IN const char cMonsterTypeFator );

	bool	IsAttribNpc( IN const int iNpcUID );

	// for lua
	int		GetNormalNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iNormalNpcDieCount; }
	int		GetLowEliteNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iLowEliteNpcDieCount; }
	int		GetHighEliteNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iHighEliteNpcDieCount; }
	int		GetMiddleBossDieCount_LUA() const			{ return m_kNpcDieCount.m_iMiddleBossDieCount; }
	int		GetBossDieCount_LUA() const					{ return m_kNpcDieCount.m_iBossDieCount; }

	//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	void	SetNpcOwner( IN const int iNPCUID, IN const UidType iOwnerUnit );
	void	EraseNpcOwner( IN const int iNPCUID );
	void	GetNpcOwnerListByUnitUID( IN const UidType iUnitUID, OUT std::vector< int >& vecNpcUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	//}
	//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	void	ClassifyMonsterEvent( OUT std::vector<int>& vecStartEventID
								, OUT std::vector<int>& vecEndEventID
								);

	void	StartedMonsterEventProcess( IN const std::vector<int>& vecStartEventID
									  , IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
									  , IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
									  );

	void	EndedMonsterEventProcess( IN const std::vector<int>& vecEndEventID
									, OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
									);

	bool	CheckEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
							 , OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
							 , OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
							 );

	void	CheckRespawnEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
									, IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
									);

	void	CreateEventMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetEventMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID );
	bool	GetEventMonsterEventID( IN const int iNpcUID, OUT int& iEventID ) const;

	bool	IsEventMonster( IN const int iNpcUID ) const;
	bool	IsEventMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveEventNpcList.find( iNpcUID ) != m_mapAliveEventNpcList.end() ); }
	bool	GetEventNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
#endif SERV_FIELD_EVENT_MONSTER
	//}}

	//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
	int		GetAttribNpcAttribCount( IN const int iNpcUID );
#endif SERV_REFORM_ITEM_DROP
	//}}

protected:
	int										m_iAtStartedMonsterCount;		// ��Ʋ�ʵ� �������� ��ȯ��ų ���� �� ( �ִ� ���� �� )
	SNpcDieCount							m_kNpcDieCount;
	std::map< int, SMonsterRespawnInfo >	m_mapReserveMonsterRespawn;		// key : GroupID, value : RespawnInfo
	std::map< int, SNpcInfoForClient >		m_mapNpcInfoForClient;			// �����ڸ� ���� �����صδ� ���� ����
	
	//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	std::map< int, UidType >				m_mapNpcOwner;					// ������ ��ȯ�� ���� ����  key : npcuid, value : unituid
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	//}
	//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	std::map<int, SMonsterRespawnInfo>		m_mapReserveEventMonsterRespawn;	// key : GroupID, value : RespawnInfo
	std::map< int, std::set<int> >			m_mapEventMonster;					// ���� ���� ���� ���� ���� �̺�Ʈ�� UID �� �ش� �̺�Ʈ�� ������ �̺�Ʈ ���� UID
	std::map< int, SNpcInfoForClient >		m_mapEventNpcInfoForClient;			// �����ڸ� ���� �����صδ� ���� ����
	std::map<int, NPC_DATA>	m_mapAliveEventNpcList;		// ���� ������ �̺�Ʈ ����	[key:NpcUID,	 value:NPC_DATA]
	std::map<int, NPC_DATA>	m_mapDieEventNpcList;		// ���� �̺�Ʈ ���� ����Ʈ	[key:NpcUID,	 value:NPC_DATA]
#endif SERV_FIELD_EVENT_MONSTER
	//}}
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::map<int, NPC_DATA>							m_mapAliveMiddleBossList;		// ���� ������ �ߺ� ����	[key:NpcUID,	 value:NPC_DATA]
	std::vector< std::vector<KNPCUnitReq> >			m_vecMiddleBossListForClient;	// �ߺ��� ������ ������� �׷캰(spawn Group id - npc)
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	std::map<int, NPC_DATA>							m_mapAliveEventBossList;		// ���� ������ �̺�Ʈ ���� ����	[key:NpcUID,	 value:NPC_DATA]
	std::vector< std::vector<KNPCUnitReq> >			m_vecEventBossListForClient;	// �̺��� ������ ������� �׷캰(spawn Group id - npc)
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


