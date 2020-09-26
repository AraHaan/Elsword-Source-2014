#pragma once
#include "RoomMonsterManager.h"


//{{ 2011. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KDungeonMonsterManager : public KRoomMonsterManager
{
public:
	struct SNpcDieCount
	{
		int		m_iNormalNpcDieCount;
		int		m_iLowEliteNpcDieCount;
		int		m_iHighEliteNpcDieCount;
		int		m_iMiddleBossDieCount;
		int		m_iBossDieCount;

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
		}
	};

	typedef std::pair< int, int >	KSubStageKey;
	struct SUB_STAGE_NPC_INFO
	{
		std::set< int > m_setNormalNpcUID;
		std::set< int > m_setMiddleBossNpcUID;
		std::set< int > m_setBossNpcUID;
	};

public:
    KDungeonMonsterManager();
    virtual ~KDungeonMonsterManager();
	
	void StartGame();
	void EndGame();
	void OnCloseRoom();

	// ���� ���������� NPC����
	bool InsertSubStageNpcList( IN const int iStageID, IN const int iSubStageID, IN const std::vector< KNPCUnitReq >& vecSubStageNpcList );
	bool IsAllSubStageNpcDie( IN const int iStageID, IN const int iSubStageID );
	
	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	void GetSubStageNpcList( IN int iStageID, IN int iSubStageID, OUT std::vector< int >& vecSubStageNpcList );
#endif SERV_REFORM_QUEST
	//}}

	// ���� ���� ����
	void IncreaseMonsterDieCount( IN const char cMonsterTypeFator );

	// for lua
	int GetNormalNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iNormalNpcDieCount; }
	int	GetLowEliteNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iLowEliteNpcDieCount; }
	int	GetHighEliteNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iHighEliteNpcDieCount; }
	int	GetMiddleBossDieCount_LUA() const			{ return m_kNpcDieCount.m_iMiddleBossDieCount; }
	int	GetBossDieCount_LUA() const					{ return m_kNpcDieCount.m_iBossDieCount; }
		
protected:
	SNpcDieCount									m_kNpcDieCount;
	std::map< KSubStageKey, SUB_STAGE_NPC_INFO >	m_mapSubStageNpcInfo; // ���꽺���������� �����ϴ� npc uid����
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


