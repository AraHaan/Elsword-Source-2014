#include ".\gsrankingmanager.h"
#include "GameServer.h"
#include "Enum/Enum.h"
#include "socket/NetCommon.h"


ImplementSingleton( KGSRankingManager );

KGSRankingManager::KGSRankingManager(void)
{	
}

KGSRankingManager::~KGSRankingManager(void)
{
}

void KGSRankingManager::Init()
{
	// �α��� ������ ���� ��ŷ ���� ���
	KELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ kPacketNot;
	kPacketNot.m_iServerUID = KBaseServer::GetKObj()->GetUID();
	SendToLoginServer( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ, kPacketNot );
}

bool KGSRankingManager::GetRankingInfo( IN const KEGS_GET_RANKING_INFO_REQ& kReq, OUT KEGS_GET_RANKING_INFO_ACK& kAck )
{
	kAck.m_iRankingType = kReq.m_iRankingType;
	kAck.m_uiViewPage = kReq.m_uiViewPage;
	kAck.m_uiTotalPage = 1;

	switch( kAck.m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
	case SEnum::RT_WEEK_RANKING:
	case SEnum::RT_MONTH_RANKING:
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	case SEnum::RT_HERO_RANKING:
#endif // SERV_HENIR_RENEWAL_2013
		{
			std::map< int, KGSHenirRanking >::const_iterator mit;
			mit = m_mapHenirRanking.find( kReq.m_iRankingType );
			if( mit == m_mapHenirRanking.end() )
			    return true;

            mit->second.GetRankingInfo( kAck.m_uiViewPage, kAck.m_uiTotalPage, kAck.m_vecHenirRankingPage );
		}
		break;

	case SEnum::RT_DUNGEON_RANKING:
		{
			GetDungeonRankingPage( kAck.m_uiViewPage, kAck.m_uiTotalPage, kAck.m_vecDungeonRankingPage );
		}
		break;

	case SEnum::RT_PVP_RANKING:
		{
			GetPvpRankingPage( kAck.m_uiViewPage, kAck.m_uiTotalPage, kAck.m_vecPvpRankingPage );
		}
		break;

	default:
		{
			START_LOG( cerr, L"�������� �ʴ� Ranking type�Դϴ�" )
				<< BUILD_LOG( kAck.m_iRankingType )
				<< END_LOG;

			return false;
		}
	}
	
	return true;
}

bool KGSRankingManager::CheckNewRecord( const KHenirRankingInfo& kRankingInfo )
{
	bool bNewRecord = false;

	std::map< int, KGSHenirRanking >::const_iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
		if( ( mit->first == SEnum::RT_HERO_RANKING ) && ( kRankingInfo.QualificationForHeroRank() == false ) )
			continue;
#endif // SERV_HENIR_RENEWAL_2013

        if( mit->second.CheckNewRecord( kRankingInfo ) == true )
		{
			bNewRecord = true;
		}
	}	

	return bNewRecord;
}

bool KGSRankingManager::GetDungeonRankingByUnitUID( IN UidType iUnitUID, OUT KDungeonRankingInfo& kInfo )
{
	std::map< UidType, KDungeonRankingInfo >::const_iterator mit;
	mit = m_mapDungeonRanking.find( iUnitUID );
	if( mit == m_mapDungeonRanking.end() )	
		return false;

	kInfo = mit->second;
	return true;
}

bool KGSRankingManager::GetPvpRankingByUnitUID( IN UidType iUnitUID, OUT KPvpRankingInfo& kInfo )
{
	std::map< UidType, KPvpRankingInfo >::const_iterator mit;
	mit = m_mapPvpRanking.find( iUnitUID );
	if( mit == m_mapPvpRanking.end() )
		return false;

	kInfo = mit->second;
	return true;
}

//{{ 2009. 10. 27  ������	ĳ���ͻ���
void KGSRankingManager::IsInRankingUser( IN UidType iUnitUID, OUT std::vector< int >& vecRankingType )
{
	vecRankingType.clear();

	std::map< int, KGSHenirRanking >::const_iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		if( mit->second.IsInRankingUser( iUnitUID ) == true )
		{
			vecRankingType.push_back( mit->first );
		}
	}
}
//}}

void KGSRankingManager::UpdateHenirRanking( const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking )
{
	std::map< int, std::vector< KHenirRankingInfo > >::const_iterator mit;
	for( mit = mapHenirRanking.begin(); mit != mapHenirRanking.end(); ++mit )
	{
		std::map< int, KGSHenirRanking >::iterator mitGSR;
		mitGSR = m_mapHenirRanking.find( mit->first );
		if( mitGSR == m_mapHenirRanking.end() )
		{
			// ��ŷ ������ ���ٸ� ��ü����!
			m_mapHenirRanking.insert( std::make_pair( mit->first, KGSHenirRanking() ) );

			mitGSR = m_mapHenirRanking.find( mit->first );
			if( mitGSR == m_mapHenirRanking.end() )
			{
				START_LOG( cerr, L"KGSRanking��ü ������ �ȵǾ���? ������ ���� ����!" )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}
		}
		
		if( mit->second.empty() )
		{
			// ��ŷ ����Ʈ�� ����ִٸ� �ʱ�ȭ
			mitGSR->second.ClearRankingInfo();
		}
		else
		{
			// ������� �ʴٸ� ������Ʈ
			mitGSR->second.UpdateRankingInfo( mit->second );
		}
	}
}

void KGSRankingManager::UpdateHenirRanking( const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking, const std::map< int, u_int >& mapLastRank )
{
	std::map< int, u_int >::const_iterator mit;
	for( mit = mapLastRank.begin(); mit != mapLastRank.end(); ++mit )
	{
		std::map< int, KGSHenirRanking >::iterator mitGSR;
		mitGSR = m_mapHenirRanking.find( mit->first );
		if( mitGSR == m_mapHenirRanking.end() )
		{
			// ��ŷ ������ ���ٸ� ��ü����!
			m_mapHenirRanking.insert( std::make_pair( mit->first, KGSHenirRanking() ) );

			mitGSR = m_mapHenirRanking.find( mit->first );
			if( mitGSR == m_mapHenirRanking.end() )
			{
				START_LOG( cerr, L"KGSRanking��ü ������ �ȵǾ���? ������ ���� ����!" )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}
		}

		// Last Rank Update
		mitGSR->second.SetLastRank( mit->second );
	}

	// ��ŷ ���� ������Ʈ
	UpdateHenirRanking( mapHenirRanking );
}

void KGSRankingManager::UpdateDungeonAndPvpRanking( IN const KELG_WEB_RANKING_REFRESH_NOT& kInfo )
{
	switch( kInfo.m_cRankingRefreshType )
	{
	case KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR:
		{
			// �ʱ�ȭ�� ���
			if( kInfo.m_bInit )
			{
				m_vecDungeonRanking.clear();
				START_LOG( cout, L"vector ���� ��ŷ �ʱ�ȭ!" );
			}
			
			std::vector< KDungeonRankingInfo >::const_iterator vit;
			for( vit = kInfo.m_vecDungeonRanking.begin(); vit != kInfo.m_vecDungeonRanking.end(); ++vit )
			{
				m_vecDungeonRanking.push_back( *vit );
			}

			// ��ü ũ�⸦ ���Ͽ� �� ������ ������ ����
			if( kInfo.m_uiTotalSize <= m_vecDungeonRanking.size() )
			{
				START_LOG( cout, L"vector ���� ��ŷ ���� �Ϸ�!" )
					<< BUILD_LOG( m_vecDungeonRanking.size() );
			}
			else
			{
				// ��ŷ ����Ʈ �� ������ ����!
				KELG_GET_WEB_RANKING_REFRESH_NOT kNot;
				kNot.m_iServerUID = KBaseServer::GetKObj()->GetUID();
				kNot.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR;
				kNot.m_uiLastIndex = m_vecDungeonRanking.size();
				SendToLoginServer( ELG_GET_WEB_RANKING_REFRESH_NOT, kNot );
			}
		}
		break;

	case KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP:
		{
			// �ʱ�ȭ�� ���
			if( kInfo.m_bInit )
			{
				m_mapDungeonRanking.clear();
				START_LOG( cout, L"map ���� ��ŷ �ʱ�ȭ!" );
			}

			std::map< UidType, KDungeonRankingInfo >::const_iterator mit;
			for( mit = kInfo.m_mapDungeonRanking.begin(); mit != kInfo.m_mapDungeonRanking.end(); ++mit )
			{
				m_mapDungeonRanking.insert( std::make_pair( mit->first, mit->second ) );
			}

			// ��ü ũ�⸦ ���Ͽ� �� ������ ������ ����
			if( kInfo.m_uiTotalSize <= m_mapDungeonRanking.size() )
			{
				START_LOG( cout, L"map ���� ��ŷ ���� �Ϸ�!" )
					<< BUILD_LOG( m_mapDungeonRanking.size() );
			}
			else
			{
				// ��ŷ ����Ʈ �� ������ ����!
				KELG_GET_WEB_RANKING_REFRESH_NOT kNot;
				kNot.m_iServerUID = KBaseServer::GetKObj()->GetUID();
				kNot.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP;
				kNot.m_uiLastIndex = m_mapDungeonRanking.size();
				SendToLoginServer( ELG_GET_WEB_RANKING_REFRESH_NOT, kNot );
			}
		}
		break;

	case KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR:
		{
			// �ʱ�ȭ�� ���
			if( kInfo.m_bInit )
			{
				m_vecPvpRanking.clear();
				START_LOG( cout, L"vector ���� ��ŷ �ʱ�ȭ!" );
			}

			std::vector< KPvpRankingInfo >::const_iterator vit;
			for( vit = kInfo.m_vecPvpRanking.begin(); vit != kInfo.m_vecPvpRanking.end(); ++vit )
			{
				m_vecPvpRanking.push_back( *vit );
			}

			// ��ü ũ�⸦ ���Ͽ� �� ������ ������ ����
			if( kInfo.m_uiTotalSize <= m_vecPvpRanking.size() )
			{
				START_LOG( cout, L"vector ���� ��ŷ ���� �Ϸ�!" )
					<< BUILD_LOG( m_vecPvpRanking.size() );
			}
			else
			{
				// ��ŷ ����Ʈ �� ������ ����!
				KELG_GET_WEB_RANKING_REFRESH_NOT kNot;
				kNot.m_iServerUID = KBaseServer::GetKObj()->GetUID();
				kNot.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR;
				kNot.m_uiLastIndex = m_vecPvpRanking.size();
				SendToLoginServer( ELG_GET_WEB_RANKING_REFRESH_NOT, kNot );
			}

			
		}
		break;

	case KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP:
		{
			// �ʱ�ȭ�� ���
			if( kInfo.m_bInit )
			{
				m_mapPvpRanking.clear();
				START_LOG( cout, L"map ���� ��ŷ �ʱ�ȭ!" );
			}

			std::map< UidType, KPvpRankingInfo >::const_iterator mit;
			for( mit = kInfo.m_mapPvpRanking.begin(); mit != kInfo.m_mapPvpRanking.end(); ++mit )
			{
				m_mapPvpRanking.insert( std::make_pair( mit->first, mit->second ) );
			}

			// ��ü ũ�⸦ ���Ͽ� �� ������ ������ ����
			if( kInfo.m_uiTotalSize <= m_mapPvpRanking.size() )
			{
				START_LOG( cout, L"map ���� ��ŷ ���� �Ϸ�!" )
					<< BUILD_LOG( m_mapPvpRanking.size() );
			}
			else
			{
				// ��ŷ ����Ʈ �� ������ ����!
				KELG_GET_WEB_RANKING_REFRESH_NOT kNot;
				kNot.m_iServerUID = KBaseServer::GetKObj()->GetUID();
				kNot.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP;
				kNot.m_uiLastIndex = m_mapPvpRanking.size();
				SendToLoginServer( ELG_GET_WEB_RANKING_REFRESH_NOT, kNot );
			}

			
		}
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� ����&���� ��ŷ Ÿ���Դϴ�." )
				<< BUILD_LOGc( kInfo.m_cRankingRefreshType )
				<< END_LOG;
		}
		break;
	}
}

void KGSRankingManager::GetDungeonRankingPage( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KDungeonRankingInfo >& vecRankingInfo ) const
{
	vecRankingInfo.clear();

	_JIF( uiViewPage > 0, return; );

	if( m_vecDungeonRanking.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return;
	}

	uiTotalPage = m_vecDungeonRanking.size() / KGSHenirRanking::PAGE_PER_COUNT;
	if( ( m_vecDungeonRanking.size() % KGSHenirRanking::PAGE_PER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * KGSHenirRanking::PAGE_PER_COUNT ) - KGSHenirRanking::PAGE_PER_COUNT;
	if( m_vecDungeonRanking.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * KGSHenirRanking::PAGE_PER_COUNT ) - KGSHenirRanking::PAGE_PER_COUNT;

		if( m_vecDungeonRanking.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecDungeonRanking.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KDungeonRankingInfo >::const_iterator vit = m_vecDungeonRanking.begin();
	vit += uiBeginIndex;

	while( vit != m_vecDungeonRanking.end() )
	{
		vecRankingInfo.push_back( *vit );

		if( vecRankingInfo.size() >= KGSHenirRanking::PAGE_PER_COUNT )
			break;

		++vit;
	}
}

void KGSRankingManager::GetPvpRankingPage( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KPvpRankingInfo >& vecRankingInfo ) const
{
	vecRankingInfo.clear();

	_JIF( uiViewPage > 0, return; );

	if( m_vecPvpRanking.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return;
	}

	uiTotalPage = m_vecPvpRanking.size() / KGSHenirRanking::PAGE_PER_COUNT;
	if( ( m_vecPvpRanking.size() % KGSHenirRanking::PAGE_PER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * KGSHenirRanking::PAGE_PER_COUNT ) - KGSHenirRanking::PAGE_PER_COUNT;
	if( m_vecPvpRanking.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * KGSHenirRanking::PAGE_PER_COUNT ) - KGSHenirRanking::PAGE_PER_COUNT;

		if( m_vecPvpRanking.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecPvpRanking.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KPvpRankingInfo >::const_iterator vit = m_vecPvpRanking.begin();
	vit += uiBeginIndex;

	while( vit != m_vecPvpRanking.end() )
	{
		vecRankingInfo.push_back( *vit );

		if( vecRankingInfo.size() >= KGSHenirRanking::PAGE_PER_COUNT )
			break;

		++vit;
	}
}


