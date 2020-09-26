#include "DungeonRoom.h"
#include "DefenceDungeonManager.h"
#include "X2Data/XSLDefenceDungeonManager.h"


//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON


KDefenceDungeonManager::KDefenceDungeonManager() :
m_bEnterDefenceDungeon( false ),
m_bOpenDefenceDungeon( false ),
m_iDefenceDungeonPhase( CXSLDefenceDungeonManager::DDP_INIT )
{
}

KDefenceDungeonManager::~KDefenceDungeonManager()
{
}

void KDefenceDungeonManager::Clear()
{
	// ���� �ʱ�ȭ
	m_bOpenDefenceDungeon = false;
	m_bEnterDefenceDungeon = false;	
	m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_INIT;
}

void KDefenceDungeonManager::StartGame()
{
	m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_INIT;
}

void KDefenceDungeonManager::EndGame()
{
	m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_INIT;
}

void KDefenceDungeonManager::OnCloseRoom()
{
	Clear();
}

//{{ 2013. 04. 29	������	����� �� ���� ���� ����
#ifdef SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
bool KDefenceDungeonManager::DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const int iNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate, IN bool b100PerEventOn/*=false*/ )
#else //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
bool KDefenceDungeonManager::DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const int iNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate )
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
#else
bool KDefenceDungeonManager::DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate )
#endif SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
//}}
{
	static int ENTER_DEFENCE = 1;

	// ����� �������� �ٽ� üũ���� �ʴ´�.
	if( CXSLDungeon::IsDefenceDungeon( iDungeonID ) == true )
		return false;

	// ����� ���� �߻����� �ʴ� �������� üũ
	if( SiCXSLDefenceDungeonManager()->IsNotBeginDefenceDungeonID( iDungeonID ) == true )
		return false;

	//{{ 2013. 04. 29	������	����� �� ���� ���� ����
#ifdef SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
	int iDungeonNpcLevel = 0;

	// ���� ���� ���
	if( CXSLDungeon::IsSecretDungeon( iDungeonID ) )
	{
		iDungeonNpcLevel = iNpcLevel;
	}
	else
	{
		iDungeonNpcLevel = SiCXSLDungeonManager()->GetDungeonNpcLevel( iDungeonID );
		if( iDungeonNpcLevel == 0 )
		{
			START_LOG( cerr, L"���� npc ���� ������ �̻��մϴ�!" )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
			return false;
		}
	}
#else
	const int iDungeonNpcLevel = SiCXSLDungeonManager()->GetDungeonNpcLevel( iDungeonID );
	if( iDungeonNpcLevel == 0 )
	{
		START_LOG( cerr, L"���� npc ���� ������ �̻��մϴ�!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
		return false;
	}
#endif SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
	//}}	

	// ����� �� ���� Ȯ�� ���
	float fEnterDefenceRate = SiCXSLDefenceDungeonManager()->GetEnterDefenceDungeonRate( iDungeonNpcLevel, mapUnitLevelList );
	
	// ����� ���� ������ �ʾҴٸ� Ȯ���� ������!
	if( m_bOpenDefenceDungeon == false )
	{
		fEnterDefenceRate *= SiCXSLDefenceDungeonManager()->GetNotOpenGateMultiplyValue();
	}
	
	if( fMultipleRate > 1.f )
	{
		fEnterDefenceRate *= fMultipleRate;
	}

	if( fEnterDefenceRate > 100.f )
		fEnterDefenceRate = 100.f;


#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	if(b100PerEventOn)
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_0_PERCENT_EVENT
		fEnterDefenceRate = 0.f;
#else //SERV_NEW_DEFENCE_DUNGEON_ENTER_0_PERCENT_EVENT
		fEnterDefenceRate = 100.f;
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_0_PERCENT_EVENT
		START_LOG( clog, L"�輮��_����� �� 100% �̺�Ʈ ����!" )
			<< BUILD_LOG( fEnterDefenceRate )
			<< END_LOG;
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT

	// Ȯ�� �Է�
	KLottery kLotEnterDefence;
	kLotEnterDefence.AddCase( ENTER_DEFENCE, fEnterDefenceRate );

	// �����ϱ�!
	if( kLotEnterDefence.Decision() == ENTER_DEFENCE ) // �׽�Ʈ�� ������ �����ϵ��� ó��
	{
		m_bEnterDefenceDungeon = true;
	}
	else
	{
		m_bEnterDefenceDungeon = false;
	}

	return IsEnterDefenceDungeon();
}

bool KDefenceDungeonManager::IncreaseDefenceDungeonPhase( IN const int iNpcID )
{
	if( SiCXSLDefenceDungeonManager()->IsDefenceGuardianMonster( iNpcID ) == false )
		return false;

	// ������ ����!
	++m_iDefenceDungeonPhase;

	// 3�̻��� ����
	if( m_iDefenceDungeonPhase > CXSLDefenceDungeonManager::DDP_PHASE_3 )
	{
		m_iDefenceDungeonPhase = CXSLDefenceDungeonManager::DDP_PHASE_3;
		return false;
	}

	return true;
}


#endif SERV_NEW_DEFENCE_DUNGEON
//}}


