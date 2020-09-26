#include "BattleFieldMonsterManager.h"
#include "X2Data/XSLUnitManager.h"
#include "KAttribNpcTable.h"
//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	#include "..\Common\GameEvent\GameEventManager.h"
	#include "..\Common\GameEvent\GameEventScriptManager.h"
#endif SERV_FIELD_EVENT_MONSTER
//}}



//{{ 2011. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldMonsterManager::KBattleFieldMonsterManager() : 
m_iAtStartedMonsterCount( 0 )
{
}

KBattleFieldMonsterManager::~KBattleFieldMonsterManager()
{
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
void KBattleFieldMonsterManager::StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue, IN const bool bBossField )
{
	KRoomMonsterManager::StartGame();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR// �۾���¥: 2013-11-20	// �ڼ���
	m_mapNpcOwner.clear();
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR

#ifdef SERV_FIELD_EVENT_MONSTER// �۾���¥: 2013-05-21	// �ڼ���
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	// ��Ʋ�ʵ尡 ������ ���Ŀ� ���� ���� ó��!
	if( bBossField == true )
	{
		if( FirstCreateBossMonster( eBattleFieldID ) == false )
		{
			START_LOG( cerr, L"���� ��ȯ ����! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iPlayerCount )
				<< END_LOG;
		}
	}
	else
	{
		if( FirstCreateMonster( eBattleFieldID, iPlayerCount ) == false )
		{
			START_LOG( cerr, L"���� ��ȯ ����! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iPlayerCount )
				<< END_LOG;
		}
	}

	// ��Ʋ�ʵ� �� ó�� ���������� ���� ��
	m_iAtStartedMonsterCount = GetAliveMonsterCount();
}

bool KBattleFieldMonsterManager::FirstCreateBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID )
{
	// 1. ������ų ���� ����Ʈ�� �޾ƿ���!
	KNPCList kBattleFieldNpcList;
	if( SiCXSLBattleFieldManager()->GetCreateBossMonsterList( eBattleFieldID, kBattleFieldNpcList ) == false )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� NPC����Ʈ �޾ƿ��� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	// 2. ���� ���� ó��!
	BOOST_TEST_FOREACH( KNPCUnitReq&, kNpcInfo, kBattleFieldNpcList.m_NPCList )
	{
		// ���� ����!
		if( CreateMonster( kNpcInfo, kNpcInfo.m_UID ) == false )
		{
			START_LOG( cerr, L"���� ������ �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< END_LOG;
			continue;
		}
	}

	return true;
}

bool KBattleFieldMonsterManager::IsAliveBossMonster( IN const int iNpcUID ) const
{
	std::map< int, NPC_DATA >::const_iterator it = m_mapAliveNpcList.find( iNpcUID );	
	if( it == m_mapAliveNpcList.end() )
	{
		return false;
	}

	return it->second.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC;
}
#else // SERV_BATTLE_FIELD_BOSS
void KBattleFieldMonsterManager::StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue )
{
	KRoomMonsterManager::StartGame();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_FIELD_EVENT_MONSTER// �۾���¥: 2013-05-21	// �ڼ���
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	if( FirstCreateMonster( eBattleFieldID, iPlayerCount ) == false )
	{
        START_LOG( cerr, L"���� ��ȯ ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iPlayerCount )
			<< END_LOG;
	}

	// ��Ʋ�ʵ� �� ó�� ���������� ���� ��
	m_iAtStartedMonsterCount = GetAliveMonsterCount();
}
#endif // SERV_BATTLE_FIELD_BOSS

void KBattleFieldMonsterManager::EndGame()
{
	KRoomMonsterManager::EndGame();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_FIELD_EVENT_MONSTER// �۾���¥: 2013-05-21	// �ڼ���
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
}

void KBattleFieldMonsterManager::OnCloseRoom()
{
	KRoomMonsterManager::OnCloseRoom();

	m_kNpcDieCount.Clear();
	m_mapReserveMonsterRespawn.clear();
	m_mapNpcInfoForClient.clear();

#ifdef SERV_FIELD_EVENT_MONSTER// �۾���¥: 2013-05-21	// �ڼ���
	m_mapReserveEventMonsterRespawn.clear();
	m_mapEventMonster.clear();
	m_mapEventNpcInfoForClient.clear();
	m_mapAliveEventNpcList.clear();
	m_mapDieEventNpcList.clear();
#endif // SERV_FIELD_EVENT_MONSTER

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_mapAliveMiddleBossList.clear();
	m_vecMiddleBossListForClient.clear();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_mapAliveEventBossList.clear();
	m_vecEventBossListForClient.clear();
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
}

bool KBattleFieldMonsterManager::FirstCreateMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount )
{
	// 1. ������ų ���� ����Ʈ�� �޾ƿ���!
	KNPCList kBattleFieldNpcList;
	if( SiCXSLBattleFieldManager()->GetCreateMonsterList( eBattleFieldID,
														  iPlayerCount,
														  kBattleFieldNpcList ) == false )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� NPC����Ʈ �޾ƿ��� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iPlayerCount )
			<< END_LOG;
		return false;
	}

	// 2. ���� ���� ó��!
	BOOST_TEST_FOREACH( KNPCUnitReq&, kNpcInfo, kBattleFieldNpcList.m_NPCList )
	{
		// ���� ����!
		if( CreateMonster( kNpcInfo, kNpcInfo.m_UID ) == false )
		{
			START_LOG( cerr, L"���� ������ �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< END_LOG;
			continue;
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// ���� ������ �ý���
//////////////////////////////////////////////////////////////////////////

// �ʵ忡�� ���� N = (�����ʵ� ��������)�� ���� �ִ�� ���� ������ ���� ���� ������ �ִ�.

// ��, �ʵ忡�� �׻� Max���� ���� ��ŭ�� ���Ͱ� �����Ѵ�.

// Max���� �� = (�Ϲݸ��ͼ� * 1) + (�ϱ޿���Ʈ���ͼ�*2) + (��޿���Ʈ���ͼ�*3) + (�߰��������ͼ�*4) + (�������ͼ�*12)

// ó�� �ʵ� �����ÿ���, (���������ο���*3) = N ���� ����Ͽ� N*3��ŭ�� NPC�� ��� ��ġ�Ѵ�.

// NPC�� ������ ��Ģ�� ���ͼ��� MAX�̸��� ��� (or�̸��� �� ���), 3~9��(����)���Ŀ� ���͸� �������Ѵ�(MIN~MAX�ð� ���� �����ϵ���)

// ���� ���� MAX�̸��� ���(or�̸��� �� ���), 3~9��(����) ���Ŀ� ���͸� �������Ѵ�(MIN~MAX�ð� ���� �����ϵ���)

// ���� ���� MAX�̻��� ���, �������� �����Ѵ�.

// ������ ���͸� �����ϴ� ��Ģ�� �Ʒ��� ������, (�ʵ庰�� ������ �׷��� ������ �� �ִ�)

// �ش� ����(�ʵ�)���� �����ϴ� ���� �׷��� ���̺�(��ũ��Ʈ)�� �����Ѵ�.

//////////////////////////////////////////////////////////////////////////

bool KBattleFieldMonsterManager::CheckRespawnMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
													  IN const int iPlayerCount, 
													  IN OUT KDangerousEventInfo& kDangerousEvent,
													  OUT KEGS_NPC_UNIT_CREATE_NOT& kResultNot )
{
	// 1. ���ݱ��� ����ִ� ���� ���� ��� ���� üũ�Ѵ�.
	const int ALIVE_MONSTER_COUNT = GetAliveMonsterCount();

	// 2. ���� �ʵ忡�� �ִ� ��ȯ ������ ���� �� ���Ѵ�.
	const int MAX_MONSTER_COUNT_IN_BATTLE_FIELD = SiCXSLBattleFieldManager()->GetMaxMonsterCountInBattleField( eBattleFieldID, iPlayerCount );
	if( MAX_MONSTER_COUNT_IN_BATTLE_FIELD <= ALIVE_MONSTER_COUNT )
		return false;

	// 3. ������ ����� ���Ͱ� �ִ��� Ȯ��!
	if( m_mapReserveMonsterRespawn.empty() == true )
		return false;

	// 4. ������ ���� ����Ʈ�� Ȯ���Ͽ� ������ Ÿ���� ���� �׷��� ���ʹ� ������ ��ų �غ� ����!
	std::map< int, int > mapRespawnReadyNpcCount;
	std::vector< int > vecRespawnTimeOverList;

	std::map< int, SMonsterRespawnInfo >::const_iterator mit;
	for( mit = m_mapReserveMonsterRespawn.begin(); mit != m_mapReserveMonsterRespawn.end(); ++mit )
	{
		const int iNpcUID = mit->first;

		// ������ �ð��� �ٵ� ���� ���� ī��Ʈ�Ѵ�.
		if( mit->second.IsRespawnTimeOver() == true )
		{
			// ������ ������ �����ϱ� ���� ���� ��´�.
			vecRespawnTimeOverList.push_back( iNpcUID );

			std::map< int, int >::iterator mitRRN;
			mitRRN = mapRespawnReadyNpcCount.find( mit->second.m_iMonsterGroupID );
			if( mitRRN == mapRespawnReadyNpcCount.end() )
			{
				mapRespawnReadyNpcCount.insert( std::make_pair( mit->second.m_iMonsterGroupID, 1 ) );
			}
			else
			{
				++mitRRN->second;
			}
		}
	}

	// 5. ������ Ÿ���� ������ ���Ͱ� �ϳ��� ���ٸ� ���̻� ���͸� �߰� ���� ���� �ʴ´�.
	if( mapRespawnReadyNpcCount.empty() == true )
		return false;

	// 6. ���� ����ִ� ���� ������ ����!
	CXSLBattleField::KBattleFieldMonsterCountInfo kAliveMonsterCountInfo;
	GetAliveMonsterCountInfo( kAliveMonsterCountInfo );

	// 7. ������ų ���� ����Ʈ�� �޾ƿ���!
	KNPCList kBattleFieldNpcList;
	if( SiCXSLBattleFieldManager()->GetRespawnMonsterList( eBattleFieldID,
														   kAliveMonsterCountInfo, 
														   mapRespawnReadyNpcCount,
														   kBattleFieldNpcList ) == false )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� NPC����Ʈ �޾ƿ��� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( kAliveMonsterCountInfo.GetTotalMonsterCount() )
			<< BUILD_LOG( mapRespawnReadyNpcCount.size() )
			<< END_LOG;
		return false;
	}

	// 8. ���� ���� ó��!
	BOOST_TEST_FOREACH( KNPCUnitReq&, kNpcInfo, kBattleFieldNpcList.m_NPCList )
	{
		// ���� ����!
		if( CreateMonster( kNpcInfo, kNpcInfo.m_UID ) == false )
		{
			START_LOG( cerr, L"���� ������ �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< END_LOG;
			continue;
		}

		// ���� �ٷ� Ŭ���̾�Ʈ�� �����ϱ� ���� �͵� ��������!
		KNPCUnitNot kNot;
		kNot.m_kNPCUnitReq = kNpcInfo;
		kResultNot.m_vecNPCUnitAck.push_back( kNot );

		//////////////////////////////////////////////////////////////////////////
		// �Ӽ� ���ʹ� ���⼭ �����ϸ� �ǰڱ�!
		//////////////////////////////////////////////////////////////////////////
		if( SiKAttribNpcTable()->IsNoDropNpc( kNpcInfo.m_NPCID ) == true )
			continue;

		if( kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_ELITE_MONSTER_DROP ) == false )
			continue;

		kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_ELITE_MONSTER_DROP );

		// ����Ʈ ���͸� �ҷ� ����!
		char cDifficultyLevel = 0;
		if( SiCXSLBattleFieldManager()->CheckPossibleEliteMonsterDrop( iPlayerCount, 0, cDifficultyLevel ) == false )
			continue;
		
		// �Ӽ� ���� ���̺��� �ش� ���� ������ ���ɴϴ�!
		KAttribEnchantNpcInfo kAttribNpcInfo;
		if( SiKAttribNpcTable()->GetAttribNpcInfo( CXSLDungeon::DT_NORMAL, cDifficultyLevel, kNpcInfo.m_Level, kNpcInfo.m_NPCID, kAttribNpcInfo ) == false )
		{
			START_LOG( cerr, L"�Ӽ� ���� ���� ��� ����!" )
				<< BUILD_LOGc( cDifficultyLevel )
				<< BUILD_LOG( kNpcInfo.m_NPCID )
				<< BUILD_LOGc( kNpcInfo.m_Level )
				<< END_LOG;
			continue;
		}
		
		// �Ӽ� ���� ������ �ӽ�����ҿ� ��������!
		if( SetAttribMonster( kNpcInfo.m_UID, kAttribNpcInfo ) == false )
		{
			START_LOG( cerr, L"�Ӽ� ������ ����ϴµ� ���� �Ͽ���!" )
				<< BUILD_LOG( kNpcInfo.m_UID )
				<< END_LOG;
			continue;
		}

		// �Ӽ� ���� ���� ����!
		kResultNot.m_mapAttribNpcInfo.insert( std::make_pair( kNpcInfo.m_UID, kAttribNpcInfo ) );
	}

	// 9. ������ ���� ����Ʈ���� ���� ����
	LIF( vecRespawnTimeOverList.size() == kResultNot.m_vecNPCUnitAck.size() );

	BOOST_TEST_FOREACH( const int, iNpcUID, vecRespawnTimeOverList )
	{
		m_mapReserveMonsterRespawn.erase( iNpcUID );
	}

	return true;
}

void KBattleFieldMonsterManager::GetAliveMonsterList( OUT std::vector< KNPCUnitReq >& vecNpcList, OUT std::map< int, KAttribEnchantNpcInfo >& mapAttirbNpcInfo ) const
{
	vecNpcList.clear();
	mapAttirbNpcInfo.clear();
    
	std::map< int, SNpcInfoForClient >::const_iterator mit;
	for( mit = m_mapNpcInfoForClient.begin(); mit != m_mapNpcInfoForClient.end(); ++mit )
	{
		// �ش� ���Ͱ� ����ִ��� Ȯ��!
		if( IsMonsterAlive( mit->first ) == false )
		{
			START_LOG( cerr, L"���� ���͸� ����Ʈ�� �������� �Ѵ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// Npc����Ʈ�� ����!
		vecNpcList.push_back( mit->second.m_kNpcInfo );

		// �ش� ���Ͱ� �Ӽ� ���Ͷ�� �Ӽ������� ����!
		BOOST_TEST_FOREACH( const KAttribEnchantNpcInfo&, kAttribNpcInfo, mit->second.m_vecAttribNpcInfo )
		{
			mapAttirbNpcInfo.insert( std::make_pair( mit->first, kAttribNpcInfo ) );
			break;
		}
	}

	//{{ 2013. 1. 4	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	for( mit = m_mapEventNpcInfoForClient.begin(); mit != m_mapEventNpcInfoForClient.end(); ++mit )
	{
		const SNpcInfoForClient& sNpcInfoForClient = mit->second;

		// �ش� ���Ͱ� ����ִ��� Ȯ��!
		if( IsEventMonsterAlive( mit->first ) == false )
		{
			START_LOG( cerr, L"���� ���͸� ����Ʈ�� �������� �Ѵ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// Npc����Ʈ�� ����!
		vecNpcList.push_back( mit->second.m_kNpcInfo );
	}
#endif SERV_FIELD_EVENT_MONSTER
	//}}
}

bool KBattleFieldMonsterManager::CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// ���纻
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	const bool bRet = KRoomMonsterManager::CreateMonster( kCreateNpcInfo.m_kNpcInfo, kCreateNpcInfo.m_kNpcInfo.m_UID );
	if( bRet )
	{
		// ������ NpcUID���� �ѱ���!
		iCreatedNpcUID = kCreateNpcInfo.m_kNpcInfo.m_UID;

		// Ŭ���̾�Ʈ�� �����ϱ� ���� ������ ��������!
		m_mapNpcInfoForClient.insert( std::make_pair( kCreateNpcInfo.m_kNpcInfo.m_UID, kCreateNpcInfo ) );
	}

    return bRet;
}

bool KBattleFieldMonsterManager::SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID )
{
	int iGroupID = 0;

	// ���� ���� ó�� �ϱ� ���� �׷�ID�� ��´�.
	if( GetMonsterGroupID( iNpcUID, iGroupID ) == false )
	{
		START_LOG( cerr, L"����ִ� ���Ͱ� �ƴմϴ�!" )
			<< BUILD_LOG( iNpcUID  )
			<< END_LOG;
		return false;
	}

	bool bRet = false;
	
	if( iGroupID == 0 ) 
	{
		// ���� �׷�ID�� 0�̸� ������ ������ ���Ͱ� �ƴϴ�.
		bRet = KRoomMonsterManager::SetMonsterDie( iNpcUID, iAttUnitUID, false );
		if( bRet )
		{
			// ���� ������ req������ �����ص� ����!
			m_mapNpcInfoForClient.erase( iNpcUID );
			
			//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			EraseNpcOwner( iNpcUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			//}
		}
	}
	else
	{
		// ������ ������ ���� �̸� ���͸� ���� ���ͷ� ó���ϰ� ������ ������ ����!
		bRet = KRoomMonsterManager::SetMonsterDie( iNpcUID, iAttUnitUID );
		if( bRet )
		{
			// ���� ������ req������ �����ص� ����!
			m_mapNpcInfoForClient.erase( iNpcUID );
			//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			EraseNpcOwner( iNpcUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			//}

			// ���� ���� ó���� �����ߴٸ� ���������� ���� ������ �ð��� ��������!
			SMonsterRespawnInfo sRespawnInfo;
			sRespawnInfo.m_iMonsterGroupID = iGroupID;
			sRespawnInfo.m_fRespawnTime = SiCXSLBattleFieldManager()->GetMonsterRespawnTimeByRandom();
			sRespawnInfo.m_tTimer.restart();
			m_mapReserveMonsterRespawn.insert( std::make_pair( iNpcUID, sRespawnInfo ) );
		}
	}

	return bRet;
}

bool KBattleFieldMonsterManager::SetAttribMonster( IN const int iNpcUID, IN const KAttribEnchantNpcInfo& kInfo )
{
	std::map< int, SNpcInfoForClient >::iterator mit;
	mit = m_mapNpcInfoForClient.find( iNpcUID );
	if( mit == m_mapNpcInfoForClient.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ���� �Դϴ�!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return false;
	}

	if( mit->second.m_vecAttribNpcInfo.empty() == false )
	{
		START_LOG( cerr, L"�̹� �Ӽ� ������ �ִ� ���� �Դϴ�. �Ͼ�� ���� ����!" )
			<< BUILD_LOG( iNpcUID )
			<< BUILD_LOG( mit->second.m_vecAttribNpcInfo.size() )
			<< END_LOG;
		return false;
	}

	mit->second.m_vecAttribNpcInfo.push_back( kInfo );
	return true;
}

bool KBattleFieldMonsterManager::IsAttribNpc( IN const int iNpcUID )
{
	std::map< int, SNpcInfoForClient >::iterator mit;
	mit = m_mapNpcInfoForClient.find( iNpcUID );
	if( mit == m_mapNpcInfoForClient.end() )
	{
		START_LOG( clog, L"�������� �ʴ� ���� �Դϴ�!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return false;
	}

	return (mit->second.m_vecAttribNpcInfo.empty() == false);
}

void KBattleFieldMonsterManager::IncreaseMonsterDieCount( IN const char cMonsterTypeFator )
{
	switch( cMonsterTypeFator )
	{
	case CXSLUnitManager::MTF_NORMAL_NPC:		++m_kNpcDieCount.m_iNormalNpcDieCount;		break;
	case CXSLUnitManager::MTF_LOW_ELITE_NPC:	++m_kNpcDieCount.m_iLowEliteNpcDieCount;	break;
	case CXSLUnitManager::MTF_HIGH_ELITE_NPC:	++m_kNpcDieCount.m_iHighEliteNpcDieCount;	break;
	case CXSLUnitManager::MTF_MIDDLE_BOSS_NPC:	++m_kNpcDieCount.m_iMiddleBossDieCount;		break;
	case CXSLUnitManager::MTF_BOSS_NPC:			++m_kNpcDieCount.m_iBossDieCount;			break;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	case CXSLUnitManager::MTF_EVENT_BOSS_NPC:	++m_kNpcDieCount.m_iEventBossDieCount;			break;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	default:
		START_LOG( cerr, L"�̻��� ���� Ÿ�� �Դϴ�!" )
			<< BUILD_LOGc( cMonsterTypeFator )
			<< END_LOG;
		return;
	}
}

//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
void KBattleFieldMonsterManager::SetNpcOwner( IN const int iNPCUID, IN const UidType iOwnerUnit )
{
	std::map< int, UidType >::iterator mit = m_mapNpcOwner.find( iNPCUID );
	if( mit == m_mapNpcOwner.end() )
	{
		m_mapNpcOwner.insert( std::make_pair( iNPCUID, iOwnerUnit ) );
	}
	else
	{
		START_LOG( cerr, L"�ߺ� �Ǵ� npc uid �� ����Ϸ��� �Ѵ�." )
			<< BUILD_LOGc( iNPCUID )
			<< BUILD_LOGc( iOwnerUnit )
			<< END_LOG;
	}
}

void KBattleFieldMonsterManager::EraseNpcOwner( IN const int iNPCUID )
{
	std::map< int, UidType >::iterator mit = m_mapNpcOwner.find( iNPCUID );
	if( mit == m_mapNpcOwner.end() )
	{
		START_LOG( clog, L"���� npc uid �� ����� �Ѵ�." )
			<< BUILD_LOGc( iNPCUID )
			<< END_LOG;
	}
	else
	{
		m_mapNpcOwner.erase( mit );
	}
}

void KBattleFieldMonsterManager::GetNpcOwnerListByUnitUID( IN const UidType iUnitUID, OUT std::vector< int >& vecNpcUID )
{
	vecNpcUID.clear();

	std::map< int, UidType >::iterator mit = m_mapNpcOwner.begin();
	for( ; mit != m_mapNpcOwner.end() ; ++mit )
	{
		if( mit->second == iUnitUID )
		{
			vecNpcUID.push_back( mit->first );
		}
	}
}
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
//}

//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
void KBattleFieldMonsterManager::ClassifyMonsterEvent( OUT std::vector<int>& vecStartEventID
													 , OUT std::vector<int>& vecEndEventID
													 )
{
	std::set<int> setEventID;
	SiKGameEventManager()->CheckEventMonster( setEventID );

	std::set<int>::const_iterator					itRunEvent = setEventID.begin();
	std::map< int, std::set<int> >::const_iterator	itMyEvent = m_mapEventMonster.begin();

	// ����/���� �̺�Ʈ �з� �۾�
	{
		while( ( itRunEvent != setEventID.end() ) && ( itMyEvent != m_mapEventMonster.end() ) )
		{
			if( *itRunEvent == itMyEvent->first )
			{
				// �̹� ���� ���� ���� ���� �̺�Ʈ�̴�.
				++itRunEvent;
				++itMyEvent;
			}
			else if( itMyEvent->first < *itRunEvent )
			{
				// �̰��� ���� ó�� �Ǿ���� �̺�Ʈ�̴�.
				vecEndEventID.push_back( itMyEvent->first );
				++itMyEvent;
			}
			else
			{
				// �̰��� ���� ó�� �Ǿ���� �̺�Ʈ�̴�.
				vecStartEventID.push_back( *itRunEvent );
				++itRunEvent;
			}
		}

		// ���� �߰��Ǵ� �̺�Ʈ �з�
		while( itRunEvent != setEventID.end() )
		{
			vecStartEventID.push_back( *itRunEvent );
			++itRunEvent;
		}

		// ����Ǵ� �̺�Ʈ �з�
		while( itMyEvent != m_mapEventMonster.end() )
		{
			vecEndEventID.push_back( itMyEvent->first );
			++itMyEvent;
		}
	}
}

void KBattleFieldMonsterManager::StartedMonsterEventProcess( IN const std::vector<int>& vecStartEventID
														   , IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
														   , IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
														   )
{
	// ���� ���۵� ���� ���� �̺�Ʈ ó��
	BOOST_TEST_FOREACH( int, iEventID, vecStartEventID )
	{
		std::vector<int> vecEventID;
		std::set<int> setEventMonsterUID;
		std::vector< KGameEventScriptManager::KAdventNpcEventInfo > vecAdventNpcEventInfo;

		vecEventID.push_back( iEventID );
		SiKGameEventScriptManager()->GetAdventNpcEventInfo( vecEventID, vecAdventNpcEventInfo );

		BOOST_TEST_FOREACH( KGameEventScriptManager::KAdventNpcEventInfo&, kInfo, vecAdventNpcEventInfo )
		{
			// ������ �� �ִ� �ʵ����� üũ!
			if( kInfo.m_setAdventField.empty() == false )
			{
				if( kInfo.m_setAdventField.find( eBattleFieldID ) == kInfo.m_setAdventField.end() )
				{
					continue;
				}
			}

			// ���� �ϸ� �ȵǴ� �ʵ����� üũ!
			if( kInfo.m_setNoAdventField.empty() == false )
			{
				if( kInfo.m_setNoAdventField.find( eBattleFieldID ) != kInfo.m_setNoAdventField.end() )
				{
					continue;
				}
			}

			// 2. ���� ���� ó��!
			for( int i=0; i < kInfo.m_iMonsterDropCount; ++i )
			{
				KNPCUnitReq kNpcInfo;

				// ������ ���͸� ��������!
				kNpcInfo.m_NPCID = kInfo.m_kAdventNpcLot.Decision();
				if( kNpcInfo.m_NPCID == KLottery::CASE_BLANK )
				{
					continue;
				}

				// ��Ʋ �ʵ忡�� �ʿ��� ������ ������.
				bool bResult = SiCXSLBattleFieldManager()->GetEventMonsterCreateInfo( eBattleFieldID, kNpcInfo );
				if( bResult == false )
				{
					START_LOG( cerr, L"��Ʋ�ʵ� NPC����Ʈ �޾ƿ��� ����!" )
						<< BUILD_LOG( eBattleFieldID )
						<< END_LOG;
					return;
				}

				// ������ ���� ����
				kNpcInfo.m_Level			+= kInfo.m_iAddLevel;
				kNpcInfo.m_bAggressive		= kInfo.m_bAggressive;
				kNpcInfo.m_AddPos.y			= kInfo.m_fAddPosY;
				kNpcInfo.m_bNoDrop			= !kInfo.m_bDrop;

				// ���� ����!
				CreateEventMonster( kNpcInfo, kNpcInfo.m_UID );

				// ���� ��Ŷ�� ���� �غ�
				KNPCUnitNot kNpcUnitNot;
				kNpcUnitNot.m_kNPCUnitReq = kNpcInfo;
				kNpcCreateNot.m_vecNPCUnitAck.push_back( kNpcUnitNot );

				// ���� ó���� ���� ���� ����
				setEventMonsterUID.insert( kNpcInfo.m_UID );
			}
		}
		// ���� ó���� ���� ���� ����
		if( setEventMonsterUID.empty() == false )
		{
			m_mapEventMonster.insert( std::map< int, std::set<int> >::value_type( iEventID, setEventMonsterUID ) );
		}
	}
}

void KBattleFieldMonsterManager::EndedMonsterEventProcess( IN const std::vector<int>& vecEndEventID
														 , OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
														 )
{
	// �̺�Ʈ���� ���� ó�� �Ѵ�.
	BOOST_TEST_FOREACH( int, iEndEventID, vecEndEventID )
	{
		// ���� �ؾ� �� �̺�Ʈ ������ �ִ��� �˻�!
		std::map< int, std::set<int> >::iterator it = m_mapEventMonster.find( iEndEventID );
		if( it == m_mapEventMonster.end() )
		{
			continue;
		}

		// �ش� �̺�Ʈ�� ���� ��ϵ� ������ ó��
		BOOST_TEST_FOREACH( int, iEventMonsterUID, it->second )
		{
			std::map< int, NPC_DATA >::iterator itAliveNpc = m_mapAliveEventNpcList.find( iEventMonsterUID );
			if( itAliveNpc == m_mapAliveEventNpcList.end() )
			{
				// ���� npc���, �ش� ��Ͽ����� �����Ѵ�.
				m_mapDieEventNpcList.erase( iEventMonsterUID );
			}
			else
			{
				// ��� �ִ� npc���, �ش� ��Ͽ��� �����ϰ�,
				m_mapAliveEventNpcList.erase( itAliveNpc );

				// �����ڸ� ���� npc ���������� �����Ѵ�.
				m_mapEventNpcInfoForClient.erase( iEventMonsterUID );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				DeleteReservedNpcUID( iEventMonsterUID );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

				// ������ ���� ���������� �����Ѵ�.
				m_mapReserveEventMonsterRespawn.erase( iEventMonsterUID );

				// �׸��� �׾��ٴ� ������ �����ֱ� ���� ��Ŷ ������ �غ��Ѵ�.
				KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
				kNpcDietNot.m_nDieNPCUID		 = iEventMonsterUID;
				kNpcDietNot.m_iNPCID			 = itAliveNpc->second.m_iNPCID;
				veckPacketNpcDie.push_back( kNpcDietNot );
			}
		}

		// �ش� �̺�Ʈ ������ �����Ѵ�.
		m_mapEventMonster.erase( it );
	}
}

void KBattleFieldMonsterManager::CreateEventMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// ���纻
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>( kNpcInfo.m_cMonsterGrade ) ) == false )
	{
		START_LOG( cerr, L"���� ��� ������ �̻��մϴ�!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
	}	

	// 1. NPC UID �߱�
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 3. NPC(Monster) ������ ���� data ����
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// ���� ������� ������������ üũ.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//������������ üũ.
	//{{ 2009. 12. 18  ������	��������ġ����
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //�Ϲݸ����� �߰��������� �������� üũ.
	//}}
	//{{ 2011. 11. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_mapAliveEventNpcList.insert( std::make_pair( iCreatedNpcUID, npcData ) );

	// ������ NpcUID���� �ѱ���!
	kCreateNpcInfo.m_kNpcInfo.m_UID = iCreatedNpcUID;

	// Ŭ���̾�Ʈ�� �����ϱ� ���� ������ ��������!
	m_mapEventNpcInfoForClient.insert( std::make_pair( kCreateNpcInfo.m_kNpcInfo.m_UID, kCreateNpcInfo ) );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	InsertReservedNpcUID( kCreateNpcInfo.m_kNpcInfo.m_UID );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

bool KBattleFieldMonsterManager::SetEventMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID )
{
	bool bRet = false;

	// ������ ������ ���� �̸� ���͸� ���� ���ͷ� ó���ϰ� ������ ������ ����!
	{
		std::map<int, NPC_DATA>::iterator mit;
		mit = m_mapAliveEventNpcList.find( iNpcUID );
		if( mit == m_mapAliveEventNpcList.end() )
		{
			// ����ִ� ���� ����Ʈ�� ������ ����!
			bRet = false;
		}

		// ���� ���� ����Ʈ�� �ִ´�!
		m_mapDieEventNpcList.insert( std::make_pair( iNpcUID, mit->second ) );

		// ����ִ� ���� ����Ʈ������ ����!
		m_mapAliveEventNpcList.erase( mit );
		bRet = true;
	}

	if( bRet )
	{
		// ���� ������ req������ �����ص� ����!
		m_mapEventNpcInfoForClient.erase( iNpcUID );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		DeleteReservedNpcUID( iNpcUID );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		// ���� ���� ó���� �����ߴٸ� ���������� ���� ������ �ð��� ��������!
		SMonsterRespawnInfo sRespawnInfo;
		//sRespawnInfo.m_iMonsterGroupID = iGroupID;
		sRespawnInfo.m_fRespawnTime = SiCXSLBattleFieldManager()->GetMonsterRespawnTimeByRandom();
		sRespawnInfo.m_tTimer.restart();
		m_mapReserveEventMonsterRespawn.insert( std::make_pair( iNpcUID, sRespawnInfo ) );
	}

	return bRet;
}

bool KBattleFieldMonsterManager::GetEventMonsterEventID( IN const int iNpcUID, OUT int& iEventID ) const
{
	for( std::map< int, std::set<int> >::const_iterator it = m_mapEventMonster.begin(); it != m_mapEventMonster.end(); ++it )
	{
		BOOST_TEST_FOREACH( const int, iEventMonsterUID, it->second )
		{
			if( iNpcUID == iEventMonsterUID )
			{
				iEventID = it->first;
				return true;
			}
		}
	}
	return false;
}

void KBattleFieldMonsterManager::CheckRespawnEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
														 , IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
														 )
{
	// ������ ����� ���Ͱ� �ִ��� Ȯ��!
	if( m_mapReserveEventMonsterRespawn.empty() == true )
		return;

	// ������ ���� ����Ʈ�� Ȯ���Ͽ� ������ Ÿ���� ���� �׷��� ���ʹ� ������ ��ų �غ� ����!
	std::set<int>	setRespawnTimeOverList;
	for( std::map<int, SMonsterRespawnInfo>::const_iterator mit = m_mapReserveEventMonsterRespawn.begin(); mit != m_mapReserveEventMonsterRespawn.end(); ++mit )
	{
		const int iNpcUID = mit->first;

		// ������ �ð��� �ٵ� ���� ���� ī��Ʈ�Ѵ�.
		if( mit->second.IsRespawnTimeOver() == true )
		{
			// ������ ������ �����ϱ� ���� ���� ��´�.
			setRespawnTimeOverList.insert( iNpcUID );
		}
	}

	// ������ Ÿ���� ������ ���Ͱ� �ϳ��� ���ٸ� ���̻� ���͸� �߰� ���� ���� �ʴ´�.
	if( setRespawnTimeOverList.empty() == true )
		return;

	// ���� ���� ó��!
	std::vector<int>	vecTimeRestartRespawnList;
	BOOST_TEST_FOREACH( const int, iNpcUID, setRespawnTimeOverList )
	{
		if( m_mapDieEventNpcList.find( iNpcUID ) == m_mapDieEventNpcList.end() )
		{
			START_LOG( cwarn, L"m_mapDieEventNpcList���� �̺�Ʈ ������ ������ �����͸� ã�� �� �����ϴ�." )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iNpcUID )
				<< END_LOG;
			continue;
		}

		// �ش� EventMonster�� �Ҽӵ� �̺�Ʈ�� ã�Ƴ����� ����!
		std::map< int, std::set<int> >::iterator itMapEventMonster;
		std::set<int>::iterator itSetEventMonsterUID;
		for( itMapEventMonster=m_mapEventMonster.begin(); itMapEventMonster != m_mapEventMonster.end(); ++itMapEventMonster )
		{
			itSetEventMonsterUID = itMapEventMonster->second.find( iNpcUID );
			if( itSetEventMonsterUID != itMapEventMonster->second.end() )
			{
				break;
			}
		}

		// �������� �ʴ� �̺�Ʈ���, �������� �� �� ����. ( �̺�Ʈ�� ����� �� �翬 ������ ������ �����Ǿ�� �Ѵ�! )
		if( itSetEventMonsterUID == itMapEventMonster->second.end() )
		{
			START_LOG( cwarn, L"�������� �ʴ� �̺�Ʈ�� �Ҽӵ� �̺�Ʈ ���͸� ������ ��Ű�� �Ͽ����ϴ�." )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iNpcUID )
				<< END_LOG;
			continue;
		}
		
		std::vector<int> vecEventID;
		std::vector< KGameEventScriptManager::KAdventNpcEventInfo > vecAdventNpcEventInfo;

		// �ش� �̺�Ʈ�� ���� ������ �о� ������ ����!
		vecEventID.push_back( itMapEventMonster->first );
		SiKGameEventScriptManager()->GetAdventNpcEventInfo( vecEventID, vecAdventNpcEventInfo );

		BOOST_TEST_FOREACH( KGameEventScriptManager::KAdventNpcEventInfo&, kInfo, vecAdventNpcEventInfo )
		{
			// ������ �� �ִ� �ʵ����� üũ!
			if( kInfo.m_setAdventField.empty() == false )
			{
				if( kInfo.m_setAdventField.find( eBattleFieldID ) == kInfo.m_setAdventField.end() )
				{
					continue;
				}
			}

			// ���� �ϸ� �ȵǴ� �ʵ����� üũ!
			if( kInfo.m_setNoAdventField.empty() == false )
			{
				if( kInfo.m_setNoAdventField.find( eBattleFieldID ) != kInfo.m_setNoAdventField.end() )
				{
					continue;
				}
			}

			// 2. ���� ���� ó��!
			{
				KNPCUnitReq kNpcInfo;

				// ������ ���͸� ��������!
				kNpcInfo.m_NPCID = kInfo.m_kAdventNpcLot.Decision();
				if( kNpcInfo.m_NPCID == KLottery::CASE_BLANK )
				{
					vecTimeRestartRespawnList.push_back( iNpcUID );
					continue;
				}

				// ��Ʋ �ʵ忡�� �ʿ��� ������ ������.
				bool bResult = SiCXSLBattleFieldManager()->GetEventMonsterCreateInfo( eBattleFieldID, kNpcInfo );
				if( bResult == false )
				{
					START_LOG( cerr, L"��Ʋ�ʵ� NPC����Ʈ �޾ƿ��� ����!" )
						<< BUILD_LOG( eBattleFieldID )
						<< END_LOG;
					return;
				}

				// ������ ���� ����
				kNpcInfo.m_Level			+= kInfo.m_iAddLevel;
				kNpcInfo.m_bAggressive		= kInfo.m_bAggressive;
				kNpcInfo.m_AddPos.y			= kInfo.m_fAddPosY;
				kNpcInfo.m_bNoDrop			= !kInfo.m_bDrop;

				// ���� ����!
				CreateEventMonster( kNpcInfo, kNpcInfo.m_UID );

				// ���� ��Ŷ�� ���� �غ�
				KNPCUnitNot kNpcUnitNot;
				kNpcUnitNot.m_kNPCUnitReq = kNpcInfo;
				kNpcCreateNot.m_vecNPCUnitAck.push_back( kNpcUnitNot );

				// ���� ó���� ���� ���� ����
				// setEventMonsterUID.insert( kNpcInfo.m_UID );
				itMapEventMonster->second.erase( itSetEventMonsterUID );
				itMapEventMonster->second.insert( kNpcInfo.m_UID );
			}
		}
	}

	// �̹� ������ Ÿ�ӿ��� ���ܵ� ����� ���� ����
	BOOST_TEST_FOREACH( const int, iNpcUID, vecTimeRestartRespawnList )
	{
		std::map<int, SMonsterRespawnInfo>::iterator it = m_mapReserveEventMonsterRespawn.find( iNpcUID );
		if( it != m_mapReserveEventMonsterRespawn.end() )
		{
			it->second.m_fRespawnTime = SiCXSLBattleFieldManager()->GetMonsterRespawnTimeByRandom();
			it->second.m_tTimer.restart();

			setRespawnTimeOverList.erase( iNpcUID );
		}
	}

	// �������� ����� ����Ʈ���� �����Ѵ�.
	BOOST_TEST_FOREACH( const int, iNpcUID, setRespawnTimeOverList )
	{
		m_mapReserveEventMonsterRespawn.erase( iNpcUID );
	}
}

bool KBattleFieldMonsterManager::CheckEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
												   , OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
												   , OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
												   )
{
	std::vector<int>	vecStartEventID;
	std::vector<int>	vecEndEventID;

	// ���� ���� ���� ���� ���� �̺�Ʈ�κ��� ����/���� �̺�Ʈ �з�
	ClassifyMonsterEvent( vecStartEventID, vecEndEventID );

	// ���� ���۵� ���� ���� �̺�Ʈ ó��
	StartedMonsterEventProcess( vecStartEventID, eBattleFieldID, kNpcCreateNot );

	// ����� ���� ���� �̺�Ʈ ó��
	EndedMonsterEventProcess( vecEndEventID, veckPacketNpcDie );

	// ���� ���� �̺�Ʈ ������ ó��
	CheckRespawnEventMonster( eBattleFieldID, kNpcCreateNot );

	return true;
}

bool KBattleFieldMonsterManager::IsEventMonster( IN const int iNpcUID ) const
{
	if( m_mapAliveEventNpcList.find( iNpcUID ) != m_mapAliveEventNpcList.end() )
		return true;

	if( m_mapDieEventNpcList.find( iNpcUID ) != m_mapDieEventNpcList.end() )
		return true;

	return false;
}

bool KBattleFieldMonsterManager::GetEventNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map<int, NPC_DATA>::const_iterator mit;
	mit = m_mapAliveEventNpcList.find( iNpcUID );
	if( mit == m_mapAliveEventNpcList.end() )
	{
		// ����ִ� �����߿� ������ ���� ���͸���Ʈ���� ã�´�.
		mit = m_mapDieEventNpcList.find( iNpcUID );
		if( mit == m_mapDieEventNpcList.end() )
		{
			return false;
		}

		kNpcData = mit->second;
		return true;
	}

	kNpcData = mit->second;
	return true;
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
int KBattleFieldMonsterManager::GetAttribNpcAttribCount( IN const int iNpcUID )
{
	std::map< int, SNpcInfoForClient >::iterator mit;
	mit = m_mapNpcInfoForClient.find( iNpcUID );
	if( mit == m_mapNpcInfoForClient.end() )
	{
		START_LOG( clog, L"�������� �ʴ� ���� �Դϴ�!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return 0;
	}

	return static_cast<int>(mit->second.m_vecAttribNpcInfo.size());
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
bool KBattleFieldMonsterManager::CheckMiddleBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
													IN const int iDangerousValue,
													IN OUT KDangerousEventInfo& kDangerousEvent,
													OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot )
{
	// ���� �ʵ忡 �ߺ��� ���� �ִٸ� ������!
	if( IsRemainMiddleBoss() == true )
		return false;

	// ���� ���� �������� �˻�
	if( kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP ) == false )
		return false;

	// ���� ���� ���� ������ ������!
	kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP );

	// �ش� �ʵ忡 �����ϴ� ���� ���ͷ� ������ �ٲߴϴ�.
	
	std::vector<KNPCUnitReq> vecMiddleBossNpcInfo;
	if( SiCXSLBattleFieldManager()->GetBattieFieldMiddleBossMonsterInfo( eBattleFieldID, iDangerousValue, vecMiddleBossNpcInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( KNPCUnitReq&, kMiddleBossNpcInfo, vecMiddleBossNpcInfo )
	{
		// ���� ����!
		CreateMiddleBossMonster( kMiddleBossNpcInfo, kMiddleBossNpcInfo.m_UID );		

		// ���� �ٷ� Ŭ���̾�Ʈ�� �����ϱ� ���� �͵� ��������!
		KNPCUnitNot kNot;
		kNot.m_kNPCUnitReq = kMiddleBossNpcInfo;
		kResultNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kNot );
	}

	m_vecMiddleBossListForClient.push_back( vecMiddleBossNpcInfo );

	return true;
}

void KBattleFieldMonsterManager::CreateMiddleBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// ���纻
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>( kNpcInfo.m_cMonsterGrade ) ) == false )
	{
		START_LOG( cerr, L"���� ��� ������ �̻��մϴ�!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
	}	

	// 1. NPC UID �߱�
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 3. NPC(Monster) ������ ���� data ����
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// ���� ������� ������������ üũ.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//������������ üũ.
	//{{ 2009. 12. 18  ������	��������ġ����
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //�Ϲݸ����� �߰��������� �������� üũ.
	//}}
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
	npcData.m_iBossGroupID	= kNpcInfo.m_iBossGroupID;

	m_mapAliveMiddleBossList.insert( std::make_pair( iCreatedNpcUID, npcData ) );

	// ������ NpcUID���� �ѱ���!
	kCreateNpcInfo.m_kNpcInfo.m_UID = iCreatedNpcUID;
}

bool KBattleFieldMonsterManager::SetMiddleBossMonsterDie( IN const int iNpcUID )
{
	bool bRet = false;

	// ������ ������ ���� �̸� ���͸� ���� ���ͷ� ó������!
	{
		std::map<int, NPC_DATA>::iterator mit;
		mit = m_mapAliveMiddleBossList.find( iNpcUID );
		if( mit == m_mapAliveMiddleBossList.end() )
		{
			// ����ִ� ���� ����Ʈ�� ������ ����!
			bRet = false;
		}

		// ����ִ� ���� ����Ʈ������ ����!
		m_mapAliveMiddleBossList.erase( mit );
		bRet = true;
	}

	if( bRet )
	{
		// �ߺ� ���� ����Ʈ ������ �����Ѵ�(�ش� �ߺ� ����)
		std::vector< std::vector<KNPCUnitReq> >	vecTempMiddleBossList = m_vecMiddleBossListForClient;
		m_vecMiddleBossListForClient.clear();

		BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempMiddleBossList )
		{
			std::vector<KNPCUnitReq> vecNew;

			BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
			{
				if( kNpc.m_UID != iNpcUID )
				{
					vecNew.push_back( kNpc );
				}
			}

			m_vecMiddleBossListForClient.push_back( vecNew );
		}
	}

	return bRet;
}

bool KBattleFieldMonsterManager::IsMiddleBossMonster( IN const int iNpcUID ) const
{
	if( m_mapAliveMiddleBossList.find( iNpcUID ) != m_mapAliveMiddleBossList.end() )
		return true;

	bool bRet = false;
	
	for( int index = 0 ; index < (int)m_vecMiddleBossListForClient.size() ; ++index )
	{
		std::vector<KNPCUnitReq> vecList = m_vecMiddleBossListForClient[index];

		BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
		{
			if( kNpc.m_UID == iNpcUID )
			{
				bRet = true;
				break;
			}
		}

		if( bRet == true )
			break;
	}

	return bRet;
}

bool KBattleFieldMonsterManager::GetMiddleBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map<int, NPC_DATA>::const_iterator mit;
	mit = m_mapAliveMiddleBossList.find( iNpcUID );
	if( mit == m_mapAliveMiddleBossList.end() )
	{
		return false;
	}

	kNpcData = mit->second;
	return true;
}

void KBattleFieldMonsterManager::GetMiddleBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList )
{
	vecNpcDataList = m_vecMiddleBossListForClient;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
bool KBattleFieldMonsterManager::CheckEventBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
														IN const int iDangerousValue,
														IN OUT KDangerousEventInfo& kDangerousEvent,
														OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot )
{
	//�ڵ� �̺�Ʈ �ɸ��� �ʾ����� ������
	if( SiKGameEventManager()->IsEnableCode( CEI_EVENT_BATTLEFIELD_EVENT_BOSS ) == false )
		return false;	

	// ���� �ʵ忡 �ߺ��� ���� �ִٸ� ������!
	if( IsRemainEventBoss() == true )
	{
		kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP );
		return false;
	}
		

	// ���� ���� �������� �˻�
	if( kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP ) == false )
		return false;

	// ���� ���� ���� ������ ������!
	kDangerousEvent.DeleteEvent( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP );

	// �ش� �ʵ忡 �����ϴ� ���� ���ͷ� ������ �ٲߴϴ�.

	std::vector<KNPCUnitReq> vecEventBossNpcInfo;
	if( SiCXSLBattleFieldManager()->GetBattieFieldEventBossMonsterInfo( eBattleFieldID, iDangerousValue, vecEventBossNpcInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( KNPCUnitReq&, kEventBossNpcInfo, vecEventBossNpcInfo )
	{
		// ���� ����!
		CreateEventBossMonster( kEventBossNpcInfo, kEventBossNpcInfo.m_UID );		

		// ���� �ٷ� Ŭ���̾�Ʈ�� �����ϱ� ���� �͵� ��������!
		KNPCUnitNot kNot;
		kNot.m_kNPCUnitReq = kEventBossNpcInfo;
		kResultNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kNot );
	}

	m_vecEventBossListForClient.push_back( vecEventBossNpcInfo );

	return true;
}

void KBattleFieldMonsterManager::CreateEventBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID )
{
	// ���纻
	SNpcInfoForClient kCreateNpcInfo;
	kCreateNpcInfo.m_kNpcInfo = kNpcInfo;

	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>( kNpcInfo.m_cMonsterGrade ) ) == false )
	{
		START_LOG( cerr, L"���� ��� ������ �̻��մϴ�!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
	}	

	// 1. NPC UID �߱�
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	//iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 3. NPC(Monster) ������ ���� data ����
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_EVENT_BOSS_NPC );	// ���� ������� ������������ üũ.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//������������ üũ.
	//{{ 2009. 12. 18  ������	��������ġ����
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //�Ϲݸ����� �߰��������� �������� üũ.
	//}}
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
	npcData.m_iBossGroupID	= kNpcInfo.m_iBossGroupID;

	m_mapAliveEventBossList.insert( std::make_pair( iCreatedNpcUID, npcData ) );

	// ������ NpcUID���� �ѱ���!
	kCreateNpcInfo.m_kNpcInfo.m_UID = iCreatedNpcUID;
}

bool KBattleFieldMonsterManager::SetEventBossMonsterDie( IN const int iNpcUID )
{
	bool bRet = false;

	// ������ ������ ���� �̸� ���͸� ���� ���ͷ� ó������!
	{
		std::map<int, NPC_DATA>::iterator mit;
		mit = m_mapAliveEventBossList.find( iNpcUID );
		if( mit == m_mapAliveEventBossList.end() )
		{
			// ����ִ� ���� ����Ʈ�� ������ ����!
			bRet = false;
		}

		// ����ִ� ���� ����Ʈ������ ����!
		m_mapAliveEventBossList.erase( mit );
		bRet = true;
	}

	if( bRet )
	{
		// �ߺ� ���� ����Ʈ ������ �����Ѵ�(�ش� �ߺ� ����)
		std::vector< std::vector<KNPCUnitReq> >	vecTempEventBossList = m_vecEventBossListForClient;
		m_vecEventBossListForClient.clear();

		BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempEventBossList )
		{
			std::vector<KNPCUnitReq> vecNew;

			BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
			{
				if( kNpc.m_UID != iNpcUID )
				{
					vecNew.push_back( kNpc );
				}
			}

			m_vecEventBossListForClient.push_back( vecNew );
		}
	}

	return bRet;
}

bool KBattleFieldMonsterManager::IsEventBossMonster( IN const int iNpcUID ) const
{
	if( m_mapAliveEventBossList.find( iNpcUID ) != m_mapAliveEventBossList.end() )
		return true;

	bool bRet = false;

	for( int index = 0 ; index < (int)m_vecEventBossListForClient.size() ; ++index )
	{
		std::vector<KNPCUnitReq> vecList = m_vecEventBossListForClient[index];

		BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
		{
			if( kNpc.m_UID == iNpcUID )
			{
				bRet = true;
				break;
			}
		}

		if( bRet == true )
			break;
	}

	return bRet;
}

bool KBattleFieldMonsterManager::GetEventBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map<int, NPC_DATA>::const_iterator mit;
	mit = m_mapAliveEventBossList.find( iNpcUID );
	if( mit == m_mapAliveEventBossList.end() )
	{
		return false;
	}

	kNpcData = mit->second;
	return true;
}

void KBattleFieldMonsterManager::GetEventBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList )
{
	vecNpcDataList = m_vecEventBossListForClient;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


