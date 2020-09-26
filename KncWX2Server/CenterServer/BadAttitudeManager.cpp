#include "BadAttitudeManager.h"
#include "BaseServer.h"
#include "BadAttitudeTable.h"
#include "X2Data/XSLDungeonManager.h"
#include "ResultProcess.h"

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

KBadAttitudeManager::KBadAttitudeManager(void)
{
}

KBadAttitudeManager::~KBadAttitudeManager(void)
{
}

void KBadAttitudeManager::init( IN KRoomUserManagerPtr m_spRoomUserManager, IN int iDungeonIDAndDiff )
{
	m_mapUnitInfoList.clear();
	m_mapSubStageMonsterScore.clear();
	m_eDungeonType = CXSLDungeon::DT_INVALID;
	m_iWaveID = 1;

	for( int iUnitIndex = 0 ; iUnitIndex < m_spRoomUserManager->GetNumMember() ; ++iUnitIndex )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iUnitIndex );
		if( IS_NULL( spRoomUser ) )
			continue;

		KBadAttitudeInfo kUnitInfo( spRoomUser->GetCID() );

		m_mapUnitInfoList.insert( std::make_pair( kUnitInfo.m_iUnitUID, kUnitInfo ) );
	}

	m_eDungeonType = SiCXSLDungeonManager()->GetDungeonType( iDungeonIDAndDiff );
	
	// ���潺 �����̶�� ����̶� ������ �����̴�
	if( CXSLDungeon::IsDefenceDungeon( iDungeonIDAndDiff ) == true )
	{
		m_eDungeonType = CXSLDungeon::DT_DEFENCE_DUNGEON;
	}
}

bool KBadAttitudeManager::RemoveUnit( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData )
{
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		int iBeforeUnitCount = static_cast<int>(m_mapUnitInfoList.size());

		kUnitData = mit->second;

		m_mapUnitInfoList.erase( mit );

		START_LOG( clog, L"���� ���� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iBeforeUnitCount )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"�������� �ʴ� ������ ������� �մϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;

		return false;
	}

	// ������ ������ ��ǥ�� �Ѱ��� �ִٸ� ã�Ƽ� ���� ����
	std::map<UidType, KBadAttitudeInfo>::iterator mitAfter = m_mapUnitInfoList.begin();
	for( ; mitAfter != m_mapUnitInfoList.end() ; ++mitAfter )
	{
		std::set<UidType>::iterator sit = mitAfter->second.m_setVotedUnitUID.find( iUnitUID );
		if( sit != mitAfter->second.m_setVotedUnitUID.end() )
		{
			mitAfter->second.m_setVotedUnitUID.erase( sit );
			--mitAfter->second.m_iVotePoint;
		}
	}

	return true;
}

void KBadAttitudeManager::IncreaseBadAttitudeOnePoint( IN UidType iUnitUID )
{
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		mit->second.m_iBadAttitudePoint += 1;
	}
	else
	{
		START_LOG( cerr, L"�������� �ʴ� �������� �ҷ� ������ �߰��Ϸ��� �մϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;
	}
}

void KBadAttitudeManager::IncreaseVoteOnePoint( IN UidType iBadAttitudeUnitUID, IN UidType iVoteUnitUID )
{ 
	std::map<UidType, KBadAttitudeInfo>::iterator mitVoteUnit = m_mapUnitInfoList.find( iVoteUnitUID );
	if( mitVoteUnit == m_mapUnitInfoList.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ������ ��ǥ�Ϸ� �մϴ�." )
			<< BUILD_LOG( iBadAttitudeUnitUID )
			<< BUILD_LOG( iVoteUnitUID )
			<< END_LOG;

		return;
	}

	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iBadAttitudeUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		std::set<UidType>::iterator sit = mit->second.m_setVotedUnitUID.find( iVoteUnitUID );
		if( sit == mit->second.m_setVotedUnitUID.end() )
		{
			mit->second.m_iVotePoint += 1;
			mit->second.m_setVotedUnitUID.insert( iVoteUnitUID );
		}
		else
		{
			START_LOG( clog, L"�ߺ� ��ǥ�� �Ϸ��� �մϴ�." )
				<< BUILD_LOG( iBadAttitudeUnitUID )
				<< BUILD_LOG( iVoteUnitUID )
				<< BUILD_LOG( static_cast<int>(mit->second.m_setVotedUnitUID.size()) )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"�������� �ʴ� �������� ���� ��ǥ ������ �߰��Ϸ��� �մϴ�." )
			<< BUILD_LOG( iBadAttitudeUnitUID )
			<< BUILD_LOG( iVoteUnitUID )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;
	}
}
 
void KBadAttitudeManager::CheckBadAttitudeUnit( OUT std::vector<UidType>& vecNewBadAttitudeUnit, OUT std::vector<UidType>& vecNewForceExitUnit )
{
	vecNewBadAttitudeUnit.clear();
	vecNewForceExitUnit.clear();

	// ���� Ÿ�Ժ� �ҷ� ����, ���� �߰� ���� ��������
	int iBadAttitudeCutLinePoint = SiKBadAttitudeTable()->GetBadAttitudeCutLinePoint( m_eDungeonType );
	int iForceExitPoint = SiKBadAttitudeTable()->GetForceExitPoint( m_eDungeonType );

	if( iBadAttitudeCutLinePoint <= 0 || iForceExitPoint <= 0 )
	{
		START_LOG( cerr, L"���� Ÿ�Ժ� �ҷ� ���� ���� �߰� ���� ���� �̻��մϴ�." )
			<< BUILD_LOG( m_eDungeonType )
			<< BUILD_LOG( iBadAttitudeCutLinePoint )
			<< BUILD_LOG( iForceExitPoint )
			<< END_LOG;
		return;
	}

	// ���� �߰ߵ� �ҷ� ������ �߷� ����
	{
		// �ҷ����� �ӽ� ����
		std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.begin();
		for(  ; mit != m_mapUnitInfoList.end() ; ++mit )
		{
			// �ҷ� ���� �߰�!
			if( mit->second.m_iBadAttitudePoint >= iBadAttitudeCutLinePoint )
			{
				// ������ Ȯ�ε� �ҷ� ������ ����
				if( mit->second.m_bIsBadAttitudeUnit == true )
					continue;
				
				mit->second.m_bIsBadAttitudeUnit = true;
				vecNewBadAttitudeUnit.push_back( mit->first );
			}
		}
	}

	BOOST_TEST_FOREACH( UidType, iUnitUID, vecNewBadAttitudeUnit )
	{
		BadAttitudeForceVote( iUnitUID );
	}
	
	// ���� ���� ������ �߷� ����
	{
		int iVoteUnitCountHalf = 100;

		// ��ü ��ǥ �ο� �� - ���ݼ� ���ذ� ���ϱ�
		switch( m_mapUnitInfoList.size() )
		{
		case 2:		// ��ǥ ���� �ο� 1  -> ���ݼ� 1
		case 3:		// ��ǥ ���� �ο� 2  -> ���ݼ� 1
			{
				iVoteUnitCountHalf = 1;
			}break;
		case 4:		// ��ǥ ���� �ο� 3  -> ���ݼ� 2
			{
				iVoteUnitCountHalf = 2;
			}break;
		}
			
		std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.begin();
		for(  ; mit != m_mapUnitInfoList.end()  ; ++mit )
		{
			// ���� Ȯ��
			if( mit->second.m_bIsBadAttitudeUnit == true )
			{
				// ���ݼ� �̻� ��ǥ �ߴ°�?
				if( mit->second.m_iVotePoint >= iVoteUnitCountHalf )
				{
					// ��ü ������ Ȯ������
					if( mit->second.m_iBadAttitudePoint >= iBadAttitudeCutLinePoint + iForceExitPoint )
					{
						mit->second.m_bForceExit = true;

						// ���� �߰� �ߴٸ� �ӽ� ������ ����
						vecNewForceExitUnit.push_back( mit->second.m_iUnitUID );
					}
				}
			}
		}
	}
}

void KBadAttitudeManager::BadAttitudeForceVote( IN UidType iBadAttitudeUnitUID )
{
	// �ҷ� ������ �ƴ϶�� �н�!
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iBadAttitudeUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		std::map<UidType, KBadAttitudeInfo>::iterator mitAther = m_mapUnitInfoList.begin();
		for( ; mitAther != m_mapUnitInfoList.end() ; ++mitAther )
		{
			// �ڽſ��Դ� ��ǥ���� ����
			if( mitAther->first == mit->first )
				continue;
			
			// �ҷ� ������� ���� ��ǥ
			if( mitAther->second.m_bIsBadAttitudeUnit == true )
			{
				IncreaseVoteOnePoint( mitAther->first, mit->first );
			}
		}
	}
}

bool KBadAttitudeManager::IsAllUnitGetScore()
{
	// ��� ������ rankinfo ���� stage, substage ���� ���ٸ� ��� ���� �޾Ҵٰ� �� �� �ִ�.

	int iStage = -1;
	int iSubStage = -1;
	bool bFirst = true;

	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.begin();
	for( ; mit != m_mapUnitInfoList.end() ; ++mit )
	{
		if( bFirst == true )
		{
			iStage = mit->second.m_kRankInfo.m_iStage;
			iSubStage = mit->second.m_kRankInfo.m_iSubStage;
			bFirst = false;
			continue;
		}
		
		if( (iStage != mit->second.m_kRankInfo.m_iStage) || (iSubStage != mit->second.m_kRankInfo.m_iSubStage) )
		{
			return false;
		}
	}

	return true;
}

void KBadAttitudeManager::SetUnitSubStageInfo( IN UidType iUnitUID, IN int iStage, IN int iSubStage, IN int iDungeonIDDiff, IN int iEndNumMember, IN int iCombo, IN int iTech )
{
	if( iStage < 0 || iSubStage < 0 )
	{
		START_LOG( cerr, L"�������� ���� ���� �̻��մϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< BUILD_LOG( iCombo )
			<< BUILD_LOG( iTech )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;
		return;
	}

	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit == m_mapUnitInfoList.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ������ ���� ���� �Ϸ� �մϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< BUILD_LOG( iCombo )
			<< BUILD_LOG( iTech )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;
		return;
	}

	if( iStage < mit->second.m_kRankInfo.m_iStage && iSubStage < mit->second.m_kRankInfo.m_iSubStage )
	{
		START_LOG( cerr, L"���ο� ���� �������� ������ �ƴմϴ�" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< BUILD_LOG( iCombo )
			<< BUILD_LOG( iTech )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;
		return;
	}

	// ���� score ���� ������
	int iMonsterPoint = 0;
	std::map< std::pair< int, int >, int >::iterator mitMonster = m_mapSubStageMonsterScore.find( std::make_pair(iStage,iSubStage) );
	if( mitMonster != m_mapSubStageMonsterScore.end() )
	{
		iMonsterPoint = mitMonster->second;
	}
	else
	{
		START_LOG( clog, L"�������� �ʴ� ��������, ���� �������������� ���� ������ ������ �մϴ�." )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< END_LOG;
	}

	// ���� ���������� ��ũ ���� ���غ���
	char cComboRank = SiKResultProcess()->GetSubStageComboResult( iDungeonIDDiff, iEndNumMember, iCombo, iMonsterPoint );
	char cTechRank  = SiKResultProcess()->GetSubStageTechResult( iDungeonIDDiff, iEndNumMember, iTech, iMonsterPoint );

// 	START_LOG( cerr, L"���� ���� ���������� ��ũ ���� Ȯ�� ����" )
// 		<< BUILD_LOG( iUnitUID )
// 		<< BUILD_LOGc( cComboRank )
// 		<< BUILD_LOGc( cTechRank )
// 		<< BUILD_LOG( iCombo )
// 		<< BUILD_LOG( iTech )
// 		<< BUILD_LOG( iMonsterPoint )
// 		<< BUILD_LOG( iStage )
// 		<< BUILD_LOG( iSubStage )
// 		<< END_LOG;

	mit->second.m_kRankInfo.m_iStage = iStage;
	mit->second.m_kRankInfo.m_iSubStage = iSubStage;
	mit->second.m_kRankInfo.m_cRank = ( cComboRank > cTechRank ) ? cComboRank : cTechRank;		// ���� ��ũ�� ������ �д�

	if( mit->second.m_kRankInfo.m_cRank <= CXSLDungeon::RT_F )
	{
		if( iMonsterPoint > 0 )	// ���� ������ �̻��� ���� ����ó��
		{
			IncreaseBadAttitudeOnePoint( iUnitUID );

//			START_LOG( cerr, L"���� ���� ���������� ��ũ ���� Ȯ�� ����" )
//				<< BUILD_LOG( iUnitUID )
//				<< BUILD_LOGc( mit->second.m_kRankInfo.m_cRank )
//				<< END_LOG;
		}
	}
}

void KBadAttitudeManager::IncreaseSubStageMonsterDieCount( IN int iStageID, IN int iSubStageID, IN const char cMonsterTypeFactor, IN bool bIsDefenceDungeon/* = false*/ )
{
	int iMonsterScore = static_cast<int>(cMonsterTypeFactor);
	
	std::map< std::pair< int, int >, int >::iterator mit;

	// �Ϲ� ���� �϶��� ���Ͱ� ���� ������ �������� ���� ����ϸ� �ȴ�.
	if( bIsDefenceDungeon == false )
	{
		// �Ķ������ stage , substage ���� ����ϸ� �ȴ�.
		mit = m_mapSubStageMonsterScore.find( std::make_pair(iStageID,iSubStageID) );
		if( mit == m_mapSubStageMonsterScore.end() )
		{
			m_mapSubStageMonsterScore.insert( std::make_pair(std::make_pair(iStageID,iSubStageID), iMonsterScore) );
		}
		else
		{
			mit->second += iMonsterScore;
		}
	}
	else		// ���潺 ���� �϶��� 
	{
		// ���� �����ص� stage , wave ����ϸ� �ȴ�.
		mit = m_mapSubStageMonsterScore.find( std::make_pair(iStageID,m_iWaveID) );
		if( mit == m_mapSubStageMonsterScore.end() )
		{
			m_mapSubStageMonsterScore.insert( std::make_pair(std::make_pair(iStageID,m_iWaveID), iMonsterScore) );
		}
		else
		{
			mit->second += iMonsterScore;
		}
	}	
}

void KBadAttitudeManager::SetDefenceDungeonWaveID( IN int iWaveID )
{
	if( m_iWaveID > iWaveID )
	{
		START_LOG( cerr, L"���� wave id �� Ů�ϴ�?!" )
			<< BUILD_LOG( m_iWaveID )
			<< BUILD_LOG( iWaveID )
			<< END_LOG;
	}

	m_iWaveID = iWaveID;
}

bool KBadAttitudeManager::GetUnitData( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData )
{
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		kUnitData = mit->second;
	}
	else
	{
		START_LOG( cerr, L"�������� �ʴ� ������ ������ ������ �մϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	return true;
}

#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}