#include "DungeonMonsterManager.h"
#include "X2Data/XSLUnitManager.h"


//{{ 2011. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KDungeonMonsterManager::KDungeonMonsterManager()
{
}

KDungeonMonsterManager::~KDungeonMonsterManager()
{
}

void KDungeonMonsterManager::StartGame()
{
	KRoomMonsterManager::StartGame();

	m_kNpcDieCount.Clear();			// ���� ���� ī��Ʈ �ʱ�ȭ
	m_mapSubStageNpcInfo.clear();	// ���� �������� ���� �ʱ�ȭ
}

void KDungeonMonsterManager::EndGame()
{
	KRoomMonsterManager::EndGame();

	m_kNpcDieCount.Clear();			// ���� ���� ī��Ʈ �ʱ�ȭ
	m_mapSubStageNpcInfo.clear();	// ���� �������� ���� �ʱ�ȭ
}

void KDungeonMonsterManager::OnCloseRoom()
{
	KRoomMonsterManager::OnCloseRoom();

	m_kNpcDieCount.Clear();
	m_mapSubStageNpcInfo.clear();
}

void KDungeonMonsterManager::IncreaseMonsterDieCount( IN const char cMonsterTypeFator )
{
	switch( cMonsterTypeFator )
	{
	case CXSLUnitManager::MTF_NORMAL_NPC:		++m_kNpcDieCount.m_iNormalNpcDieCount;		break;
	case CXSLUnitManager::MTF_LOW_ELITE_NPC:	++m_kNpcDieCount.m_iLowEliteNpcDieCount;	break;
	case CXSLUnitManager::MTF_HIGH_ELITE_NPC:	++m_kNpcDieCount.m_iHighEliteNpcDieCount;	break;
	case CXSLUnitManager::MTF_MIDDLE_BOSS_NPC:	++m_kNpcDieCount.m_iMiddleBossDieCount;		break;
	case CXSLUnitManager::MTF_BOSS_NPC:			++m_kNpcDieCount.m_iBossDieCount;			break;
	default:
		START_LOG( cerr, L"�̻��� ���� Ÿ�� �Դϴ�!" )
			<< BUILD_LOGc( cMonsterTypeFator )
			<< END_LOG;
		return;
	}
}

bool KDungeonMonsterManager::InsertSubStageNpcList( IN const int iStageID, IN const int iSubStageID, IN const std::vector< KNPCUnitReq >& vecSubStageNpcList )
{
	SUB_STAGE_NPC_INFO sSubStageNpcInfo;

	// ���� ��޺��� �����ؼ� ����Ʈ�� ������!
	BOOST_TEST_FOREACH( const KNPCUnitReq&, kNpcInfo, vecSubStageNpcList )
	{
		if( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC )
		{
			sSubStageNpcInfo.m_setBossNpcUID.insert( kNpcInfo.m_UID );
		}
		else if( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_MIDDLE_BOSS_NPC )
		{
			sSubStageNpcInfo.m_setMiddleBossNpcUID.insert( kNpcInfo.m_UID );
		}
		else if( kNpcInfo.m_bActive )
		{
			sSubStageNpcInfo.m_setNormalNpcUID.insert( kNpcInfo.m_UID );
		}
	}

	// ���� ����Ʈ�� ��������!
	KSubStageKey kSubStageKey( iStageID, iSubStageID );
	if( m_mapSubStageNpcInfo.find( kSubStageKey ) != m_mapSubStageNpcInfo.end() )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ���� �������� ���� NPC������ �ִ�?" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( iSubStageID )
			<< BUILD_LOG( m_mapSubStageNpcInfo.size() )
			<< END_LOG;

		// �̹� �ִ°��� ������!
		m_mapSubStageNpcInfo.erase( kSubStageKey );
	}

	m_mapSubStageNpcInfo.insert( std::make_pair( kSubStageKey, sSubStageNpcInfo ) );
	return true;
}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
void KDungeonMonsterManager::GetSubStageNpcList( IN int iStageID, IN int iSubStageID, OUT std::vector< int >& vecSubStageNpcList )
{
	vecSubStageNpcList.clear();

	KSubStageKey kSubStageKey( iStageID, iSubStageID );
	std::map< KSubStageKey, SUB_STAGE_NPC_INFO >::const_iterator mit;
	mit = m_mapSubStageNpcInfo.find( kSubStageKey );
	if( mit == m_mapSubStageNpcInfo.end() )
	{
		START_LOG( clog, L"Npc�� ���� ���������Դϴ�. ������ �ƴ� - ���߿� clog �� �ٲ�����" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( iSubStageID )
			<< END_LOG;
		return;
	}

	// �Ϲ� npc ���� ���
	{
		std::set< int >::const_iterator sit = mit->second.m_setNormalNpcUID.begin();
		for( ; sit != mit->second.m_setNormalNpcUID.end() ; ++sit )
		{
			NPC_DATA kData;
			if( GetNpcData( *sit, kData ) == true )
			{
				vecSubStageNpcList.push_back( kData.m_iNPCID );
			}
		}
	}

	// �ߺ� npc ���� ���
	{
		std::set< int >::const_iterator sit = mit->second.m_setMiddleBossNpcUID.begin();
		for( ; sit != mit->second.m_setMiddleBossNpcUID.end() ; ++sit )
		{
			NPC_DATA kData;
			if( GetNpcData( *sit, kData ) == true )
			{
				vecSubStageNpcList.push_back( kData.m_iNPCID );
			}
		}
	}

	// ���� npc ���� ���
	{
		std::set< int >::const_iterator sit = mit->second.m_setBossNpcUID.begin();
		for( ; sit != mit->second.m_setBossNpcUID.end() ; ++sit )
		{
			NPC_DATA kData;
			if( GetNpcData( *sit, kData ) == true )
			{
				vecSubStageNpcList.push_back( kData.m_iNPCID );
			}
		}
	}
}
#endif SERV_REFORM_QUEST
//}}

bool KDungeonMonsterManager::IsAllSubStageNpcDie( IN const int iStageID, IN const int iSubStageID )
{
	// ��� NPC�� �׿��޶�� ��û�� ���� ���������� ������ �ִ� ���� ������������ Ȯ������!
	KSubStageKey kSubStageKey( iStageID, iSubStageID );

	std::map< KSubStageKey, SUB_STAGE_NPC_INFO >::const_iterator mit;
	mit = m_mapSubStageNpcInfo.find( kSubStageKey );
	if( mit == m_mapSubStageNpcInfo.end() )
	{
		START_LOG( cerr, L"Npc�� ���� ���������ε� ���NPC �׿��޶�� ��û��Ŷ�� �Գ�? ��ŷ�ΰ�?" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( iSubStageID )
			<< END_LOG;
		return false;
	}

	const SUB_STAGE_NPC_INFO& sSubStageNpcInfo = mit->second;

	// ���� �������� ���� �Ϲ� �������� ���� Ȯ��
	if( sSubStageNpcInfo.m_setBossNpcUID.empty() )
	{
		// �ش� ���� ���������� ��� �Ϲ� ���Ͱ� �׾����� Ȯ��!
		BOOST_TEST_FOREACH( const int, iNpcUID, sSubStageNpcInfo.m_setNormalNpcUID )
		{
			bool bIsDie = false;
			if( IsExistMonster( iNpcUID, bIsDie ) == false )
			{
				START_LOG( cerr, L"�Ϲ� npc ����Ʈ�� �ִ� NpcUID�� �˻��ߴ��� NPC_DATA�� ����?" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOG( iSubStageID )
					<< BUILD_LOG( iNpcUID )
					<< END_LOG;
				continue;
			}

			// npc�� ���� �ʾҴٸ�?
			if( bIsDie == false )
			{
				START_LOG( cerr, L"��� npc�� ������ �ʾҴµ� ��� ���� �׿��޶�� ��Ŷ�� �Գ�.. ��ŷ�ΰ�?" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOG( iSubStageID )
					<< BUILD_LOG( iNpcUID )
					<< END_LOG;
				return false;
			}
		}
	}
	else
	{
		// �ش� ���� ���������� ��� ���� ���Ͱ� �׾����� Ȯ��!
		BOOST_TEST_FOREACH( const int, iNpcUID, sSubStageNpcInfo.m_setBossNpcUID )
		{
			bool bIsDie = false;
			if( IsExistMonster( iNpcUID, bIsDie ) == false )
			{
				START_LOG( cerr, L"���� ����Ʈ�� �ִ� NpcUID�� �˻��ߴ��� NPC_DATA�� ����?" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOG( iSubStageID )
					<< BUILD_LOG( iNpcUID )
					<< END_LOG;
				continue;
			}

			// ������ ���� �ʾҴٸ�?
			if( bIsDie == false )
			{
				START_LOG( cerr, L"������ ������ �ʾҴµ� ��� ���� �׿��޶�� ��Ŷ�� �Գ�.. ��ŷ�ΰ�?" )
					<< BUILD_LOG( iStageID )
					<< BUILD_LOG( iSubStageID )
					<< BUILD_LOG( iNpcUID )
					<< END_LOG;
				return false;
			}
		}
	}

	return true;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


