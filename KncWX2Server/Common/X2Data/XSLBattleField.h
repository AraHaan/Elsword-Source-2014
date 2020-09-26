#pragma once
#include <string>
#include <vector>
#include "Lua/KLuaManager.h"
#include "dbg/dbg.hpp"
#include "Event.h"
#include "CommonPacket.h"

#include "X2Data/XSLUnitManager.h"
#include "Lottery.h"
#include <d3dx9math.h>
#include "Enum/Enum.h"


class CXSLBattleField
{
public:
	// �ʵ� ���� �ִ� ��Ż�� �̵� Ÿ��
	enum PORTAL_MOVE_TYPE
	{
		PMT_INVALID 			= 0,
		PMT_MOVE_TO_VILLAGE		= 1, // �ʵ� -> ������ �̵��ϴ� ��Ż
		PMT_MOVE_TO_FIELD		= 2, // �ʵ� -> �ʵ�� �̵��ϴ� ��Ż
		PMT_END					= 3,
	};

	//////////////////////////////////////////////////////////////////////////	
	// ��Ż ����
	struct SPotalMovingInfo
	{
		PORTAL_MOVE_TYPE	m_ePortalMoveType;
		int					m_iLineNumber;
		int					m_iPlaceIDToMove;
		int					m_iPositionIndexToMove;
        
		SPotalMovingInfo()
		{
			m_ePortalMoveType		= PMT_INVALID;
			m_iLineNumber			= 0;
			m_iPlaceIDToMove		= 0;
			m_iPositionIndexToMove	= 0;
		}
	};
	
	//////////////////////////////////////////////////////////////////////////	
	// ���赵 ����
	struct SRiskInfo
	{
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		int					m_iSpawnID;				// ���� ID
		int					m_iSpawnRate;			// Ȯ��
		std::vector<int>	m_vecSpawnGroupID;		// ���� ���Ͱ� ���� �׷�
#else
		int					m_iSpawnGroupID;		// ���� ���Ͱ� ���� �׷�
		int					m_iSpawnMonsterID;		// ���� ����ID
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
		int					m_iRiskValue;
		bool				m_bShowBossName;
		bool				m_bBossGaugeHpLines;

		SRiskInfo()
		{
			//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			m_iSpawnID			= 0;
			m_iSpawnRate		= 0;
#else
			m_iSpawnGroupID		= 0;
			m_iSpawnMonsterID	= 0;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
			m_iRiskValue		= 0;
			m_bShowBossName		= false;
			m_bBossGaugeHpLines = false;
		}
	};


	//////////////////////////////////////////////////////////////////////////	
	// ���� ����
	struct SSpawnNpcInfo
	{
        CXSLUnitManager::NPC_UNIT_ID	m_eNpcID;
		float							m_fRate;
		int								m_iMaxNumberOfThisMonster;
		int								m_iAddLevel;
		bool							m_bDrop;
		bool							m_bAggressive;
        float							m_fAddPosY;	
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		bool				m_bFocusCamera;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
		

		KLotterySameProb< int >			m_StartPositionLot;
		std::vector< int >				m_vecPetrolLineIndex;
		std::vector< int >				m_vecPlayLineIndex;

		SSpawnNpcInfo()
		{
			m_eNpcID		= CXSLUnitManager::NUI_NONE;
			m_fRate			= 0.f;
			m_iMaxNumberOfThisMonster = 0;
			m_iAddLevel		= 0;
			m_bDrop			= false;
			m_fAddPosY		= 0.f;
			m_bAggressive	= true;
			//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			m_bFocusCamera	= false;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
		}
	};

	//////////////////////////////////////////////////////////////////////////	
	// ���� �׷� ����
	struct SSpawnNpcGroup
	{
		bool							m_bIsBossMonsterGroup;
		int								m_iGroupID;		
		int								m_iMinNumberOfMonsterInThisGroup;
		int								m_iMaxNumberOfMonsterInThisGroup;
		std::set< int >					m_setSiegeMonster;
		std::map< int, SSpawnNpcInfo >	m_mapSpawnNpcList;

		SSpawnNpcGroup()
		{
			m_bIsBossMonsterGroup = false;
			m_iGroupID = 0;
			m_iMinNumberOfMonsterInThisGroup = 0;
			m_iMaxNumberOfMonsterInThisGroup = 0;
		}

		bool IsSiegeMonster( IN const int iNpcID ) const
		{
			return ( m_setSiegeMonster.find( iNpcID ) != m_setSiegeMonster.end() );
		}
	};

	//////////////////////////////////////////////////////////////////////////	
	// �׷� ���� ���� ī��Ʈ ����
	class KGroupMonsterCountInfo
	{
	private:
		int						m_iTotalMonsterCount;
		std::map< int, int >	m_mapMonsterCount;		// [key : NpcID, value : AliveCount] 

	public:
		KGroupMonsterCountInfo() : 
		m_iTotalMonsterCount( 0 )
		{
		}

		void AddMonsterCount( IN const int iNpcID )
		{
			std::map< int, int >::iterator mit;
			mit = m_mapMonsterCount.find( iNpcID );
			if( mit == m_mapMonsterCount.end() )
			{
				m_mapMonsterCount.insert( std::make_pair( iNpcID, 1 ) );
			}
			else
			{
                ++mit->second;
			}

			++m_iTotalMonsterCount;
		}

		bool GetMonsterCount( IN const int iNpcID, OUT int& iCount ) const
		{
			iCount = 0;

			std::map< int, int >::const_iterator mit;
			mit = m_mapMonsterCount.find( iNpcID );
			if( mit == m_mapMonsterCount.end() )
			{
				return false;
			}

			iCount = mit->second;
			return true;
		}

		int GetTotalMonsterCount() const	{ return m_iTotalMonsterCount; }
	};

	//////////////////////////////////////////////////////////////////////////	
	// �ʵ� ���� ���� ī��Ʈ ����
	class KBattleFieldMonsterCountInfo
	{
	private:
		std::map< int, KGroupMonsterCountInfo >	m_mapGroupMonsterCount;		// [key : GroupID, value : GroupInfo] 

	public:
		KBattleFieldMonsterCountInfo()
		{
		}

		void AddGroupMonsterCount( IN const int iGroupID, IN const int iNpcID )
		{
			std::map< int, KGroupMonsterCountInfo >::iterator mitMC;
			mitMC = m_mapGroupMonsterCount.find( iGroupID );
			if( mitMC == m_mapGroupMonsterCount.end() )
			{
				KGroupMonsterCountInfo sCountInfo;
				sCountInfo.AddMonsterCount( iNpcID );
				m_mapGroupMonsterCount.insert( std::make_pair( iGroupID, sCountInfo ) );
			}
			else
			{
				mitMC->second.AddMonsterCount( iNpcID );
			}
		}

		bool GetGroupMonsterCount( IN const int iGroupID, OUT int& iCount ) const
		{
			iCount = 0;

			std::map< int, KGroupMonsterCountInfo >::const_iterator mit;
			mit = m_mapGroupMonsterCount.find( iGroupID );
			if( mit == m_mapGroupMonsterCount.end() )
			{
				return false;
			}

			iCount = mit->second.GetTotalMonsterCount();
			return true;
		}

		bool IsExistGroupID( IN const int iGroupID ) const
		{
			return ( m_mapGroupMonsterCount.find( iGroupID ) != m_mapGroupMonsterCount.end() );
		}

		bool GetMonsterCountByGroupIDAndNpcID( IN const int iGroupID, IN const int iNpcID, OUT int& iNpcCount ) const
		{
			iNpcCount = 0;

			std::map< int, KGroupMonsterCountInfo >::const_iterator mitMC;
			mitMC = m_mapGroupMonsterCount.find( iGroupID );
			if( mitMC == m_mapGroupMonsterCount.end() )
			{
				return false;
			}

			return mitMC->second.GetMonsterCount( iNpcID, iNpcCount );
		}

		int GetTotalMonsterCount() const
		{
			int iTotalCount = 0;

			std::map< int, KGroupMonsterCountInfo >::const_iterator mit;
			for( mit = m_mapGroupMonsterCount.begin(); mit != m_mapGroupMonsterCount.end(); ++mit )
			{
				iTotalCount += mit->second.GetTotalMonsterCount();
			}

			return iTotalCount;
		}
	};

	//////////////////////////////////////////////////////////////////////////	
	// ���θ� NPC ��ŸƮ ������ ����
	struct SMonsterStartPos
	{
		VECTOR3			m_vPos;
		bool			m_bRight;

		SMonsterStartPos()
		{
			m_bRight = false;
		}
	};
	

public:
	CXSLBattleField();
	~CXSLBattleField();

	// �ʱ�ȭ
	bool	LoadBattleFieldData( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN OUT KLuaManager& kLuaManager );
	bool	AddSpawnMonsterGroup_LUA();

	bool	OpenScriptFile();
	void	SetLandHeight_LUA( int iLandHeight );	
	void	SetTeamStartPos_LUA();
	bool	LoadLineMapData_LUA( int iStartPos, D3DXVECTOR3 vPos, bool bRight, int iLineMapIndex );
	void	AddLine_LUA();
	void	AddCameraData_LUA();
	void	AddNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup );

	// ���� ����Ʈ �ʱ�ȭ
	bool	GetCreateMonsterList( IN const int iPlayerCount,
							   OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const;

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	bool	GetCreateBossMonsterList( OUT std::vector<KNPCUnitReq>& vecCreateNPCList ) const;
#endif // SERV_BATTLE_FIELD_BOSS

	// ������ ����
	bool	GetRespawnMonsterList( IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterCountInfo,
								IN const std::map< int, int >& mapRespawnReadyNpcCount,
								OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const;

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	bool	GetBattieFieldEventBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo );
	bool	MakeEventBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	bool	GetBattieFieldMiddleBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo );
	bool	MakeMiddleBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	bool	GetBattieFieldBossMonsterInfo( IN const int iDangerousValue, OUT KNPCUnitReq& kNpcInfo );

	int		GetMaxMonsterCountInThisBattleField( IN const int iPlayerCount ) const;

	int		GetMaxNumberMonster() const		{ return m_iMaxNumberMonster; }
	int		GetMaxJoinUserCount() const		{ return m_iMaxJoinUserCount; }
	int		GetRequireLevel() const			{ return m_iRequireLevel; }
	int		GetMonsterRespawnFactorByUserCount() const { return m_iMonsterRespawnFactorByUserCount; }
	void	SetMonsterRespawnFactorByUserCount( IN const int MonsterRespawnFactorByUserCount ) { m_iMonsterRespawnFactorByUserCount = MonsterRespawnFactorByUserCount; }
	int		GetMonsterStartPosByRandom( IN const std::vector< int >& vecLineMapIndexList ) const;
	bool	GetMonsterStartPosInfo( IN const int iStartPos, OUT VECTOR3& vPos, OUT bool& bRight ) const;
	const SSpawnNpcGroup* GetSpawnNpcGroup( IN const int iGroupID ) const;

	// ���� ���� üũ
	bool	IsCanEnterBattleField( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iRequireLevel, OUT int& iRequireDungeonID ) const;
	//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int		GetEnterVillageMapID() const	{ return m_iEnterVillageMapID; }
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}
	//{{ 2013. 1. 1	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	bool	GetEventMonsterCreateInfo( OUT KNPCUnitReq& kInfo );
#endif SERV_FIELD_EVENT_MONSTER
	//}}

	int		GetStandardMonsterLevel() const { return m_iStandardMonsterLevel; }

private:
	// ���� ����
	bool	CreateMonsterFromSpawnNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kMonsterCountInfo, OUT KNPCUnitReq& kInfo ) const;

	// ���� ���� ����
	bool	MakeMonsterInfo( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const int iNpcID, OUT KNPCUnitReq& kInfo ) const;

private:
	lua_State*							m_luaState;

private:
	SEnum::BATTLE_FIELD_ID				m_eBattleFieldID;					// ��Ʋ�ʵ�ID
	short								m_sWorldID;							// ��������
	std::wstring						m_wstrLineMapFileName;				// ���θ� ���� �̸�
	std::wstring						m_wstrNpcTableFileName;				// NPC���� ���� ���� �̸�
	int									m_iStandardMonsterLevel;			// �ʵ忡 ������ ���͵��� ���� ����
	std::wstring						m_wstrBattleFieldName;				// ��Ʋ�ʵ� �̸�	
	int									m_iMaxNumberMonster;				// �� �ʵ峻�� �ִ� ���� ��
	int									m_iMaxJoinUserCount;				// �ִ� ���� ������ ���� ��
	int									m_iMonsterRespawnFactorByUserCount; // ��Ʋ�ʵ忡 ������ ���� ���� ���� ���� ���� �� ��� ���

	int									m_iRequireLevel;					// ���� ���� ����
	std::set< int >						m_setRequireDungeon;				// ���� ���� ����
	
	std::vector< SPotalMovingInfo > 	m_vecPotalMovingInfo;				// ��Ż�� �̵��ϴ°��� ����
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::vector< SRiskInfo >			m_vecMiddleBossInfo;				// �߰����� �׷�����
#else
	std::vector< SRiskInfo >			m_vecRiskInfo;						// ���赵 ����
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	std::vector< SRiskInfo >			m_vecEventBossInfo;				// �̺�Ʈ���� �׷�����
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	std::vector<SSpawnNpcGroup>			m_vecBossNpcGroupList;				// ���� Boss NPC �׷�
#endif // SERV_BATTLE_FIELD_BOSS
	std::vector< SSpawnNpcGroup >		m_vecRespawnNpcGroupList;			// ���� NPC �׷�
	std::map< int, SSpawnNpcGroup >		m_mapNpcGroupList;					// ���� NPC �׷�

	std::map< int, std::vector< int > >	m_mapLineIndexRandomStartPos;		// ���θ� �ε����� ���� StartPos�׷�
	std::map< int, SMonsterStartPos >	m_mapMonsterStartPos;				// ���� Start Position

	//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int									m_iEnterVillageMapID;				// �ش� ��Ʋ�ʵ尡 ���� ���� MapID
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}
};



