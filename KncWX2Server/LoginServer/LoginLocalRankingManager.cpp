#include "LoginLocalRankingManager.h"

//{{ 2013. 3. 15	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
#include "GameSysVal/GameSysVal.h"
#include "NetError.h"

#define CLASS_TYPE  KLoginLocalRankingManager
ImplPfID( KLoginLocalRankingManager, PI_NULL );
ImplementSingleton( KLoginLocalRankingManager );

KLoginLocalRankingManager::KLoginLocalRankingManager()
: m_bInit( false )
, m_iUID( 0 )
{
}

void KLoginLocalRankingManager::clear( void )
{
	m_bInit	= false;
	m_iUID	= 0;
	m_kResetTimer.restart();

	m_mapRankerTab.clear();

	m_mapRankerUserInfo.clear();
	m_mapRankerUnitInfo.clear();

	m_mapUserUnitList.clear();

	m_mapBackUpUnitInfo.clear();
	m_mapBackUpUIDInfo.clear();
	m_mapBackUpUserInfo.clear();
}

bool KLoginLocalRankingManager::Begin( void )
{
	SendToGameDB( DBE_GAME_LOCAL_RANKING_INIT_INFO_REQ );
	m_kResetTimer.restart();
	return KThread::Begin();
}

void KLoginLocalRankingManager::ShutDown( void )
{
	KThread::End();
}

void KLoginLocalRankingManager::Init( IN const int iUID
									, IN const std::wstring& wstrResetTime
									, IN const std::map<UidType, KLocalRankingUnitInfo>& mapRankerUnitInfo
									, IN const std::map<UidType, UidType>& mapRankerUIDInfo
									, IN const std::map<UidType, KLocalRankingUserInfo>& mapRankerUserInfo
									)
{
	clear();

	// �ý��� ���� ����
	CTime tTemp;
	if( KncUtil::ConvertStringToCTime( wstrResetTime, tTemp ) == false )
	{
		START_LOG( cerr, L"���� ��ŷ �ý��� ���� Ÿ�� �ʱ�ȭ ����" )
			<< BUILD_LOG( wstrResetTime )
			<< END_LOG;
		return;
	}

	m_bInit = true;
	m_iUID	= iUID;

	// ������ �ʱ�ȭ �ð����κ��� 1���� �� ����� ���� 6�ø� ����
	int iDayOfWeek			= tTemp.GetDayOfWeek();
	int iNextResetDayTerm	= 5 - iDayOfWeek;
	if( iNextResetDayTerm <= 0 )
	{
		iNextResetDayTerm += 7;
	}

	CTimeSpan tNextResetDayTerm( iNextResetDayTerm, 0, 0, 0 );

	m_tResetTime = CTime( tTemp.GetYear(), tTemp.GetMonth(), tTemp.GetDay(), 6, 0, 0 ) + tNextResetDayTerm;

	// ��ŷ ���� ����
	std::map<UidType, KLocalRankingUnitInfo>::const_iterator it;
	for( it = mapRankerUnitInfo.begin(); it != mapRankerUnitInfo.end(); ++it )
	{
		std::map<UidType, UidType>::const_iterator itUIDInfo = mapRankerUIDInfo.find( it->first );
		if( itUIDInfo == mapRankerUIDInfo.end() )
		{
			continue;
		}

		std::map<UidType, KLocalRankingUserInfo>::const_iterator itUserInfo = mapRankerUserInfo.find( itUIDInfo->second );
		if( itUserInfo == mapRankerUserInfo.end() )
		{
			continue;
		}

		std::map<UidType, bool>	mapChangeInfo;
		std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT> vecRankUpNot;
		SomethingChanged( m_iUID, itUserInfo->second, it->second, mapChangeInfo, vecRankUpNot );
	}

	m_mapBackUpUnitInfo	= mapRankerUnitInfo;
	m_mapBackUpUIDInfo	= mapRankerUIDInfo;
	m_mapBackUpUserInfo	= mapRankerUserInfo;

	std::wstring wstrNextResetTime = m_tResetTime.Format( _T("%Y-%m-%d %H:%M:%S") );
	START_LOG( cout, L"���� ��ŷ �ý��� Init �Ϸ�" )
		<< BUILD_LOG( m_iUID )
		<< BUILD_LOG( wstrResetTime )
		<< BUILD_LOG( wstrNextResetTime );
}

void KLoginLocalRankingManager::Tick( void )
{
	KPerformer::Tick();

	// Tick���鼭 Init üũ�� �ǽ��Ѵ�.
	if( 60.0 < m_kResetTimer.elapsed() )
	{
		if( CheckInit() == true )
		{
			// ���� �� �ʱ�ȭ üũ
			CTime tCurrentTime = CTime::GetCurrentTime();

			// ���� �ð� üũ
			if( m_tResetTime < tCurrentTime )
			{
				// ���� ��ȣ ����
				// ������ �Ϸ�� ������ ��� ��û���� �������� �ʴ´�.
				// UID�� 0�� �ƴ� ������ ����� �ٸ� ���� �ǵ��� �Ѵ�.
				while( ++m_iUID == 0 );
				SendToGameDB( DBE_LOCAL_RANKING_RESET_REQ, m_iUID );
				clear();
			}
			else	// �ǽð� ���� �ʱ�ȭ üũ
			{
				SendToGameDB( DBE_LOCAL_RANKING_RESET_CHECK_REQ );
			}

			// ��ŷ ������ üũ ( ��� �۾� )
			SizeProcess();
		}
		else
		{
			SendToGameDB( DBE_GAME_LOCAL_RANKING_INIT_INFO_REQ );
		}

		m_kResetTimer.restart();
	}
}

void KLoginLocalRankingManager::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

	switch( spEvent_->m_usEventID )
	{
		CASE( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT );
		CASE( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK );
		CASE( DBE_LOCAL_RANKING_RESET_ACK );
		_CASE( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, int );
		_CASE( DBE_LOCAL_RANKING_RESET_CHECK_ACK, int );
		CASE_NOPARAM( ELG_LOCAL_RANKING_RESET_NOT );
		CASE( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT );

	default:
		dbg::cerr << L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr();
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

void KLoginLocalRankingManager::Run( void )
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KLoginLocalRankingManager::SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID ) const
{
	SendToGSUser( nToGSUID, nToGSUser, usEventID, char() );
}

void KLoginLocalRankingManager::SendToGameServer( UidType iGameServerUID, unsigned short usEventID ) const
{
	SendToGameServer( iGameServerUID, usEventID, char() );
}

void KLoginLocalRankingManager::SendToAccountDB( unsigned short usEventID ) const
{
	SendToAccountDB( usEventID, char() );
}

void KLoginLocalRankingManager::SendToGameDB( unsigned short usEventID ) const
{
	SendToGameDB( usEventID, char() );
}

void KLoginLocalRankingManager::BroadCastAllGS( unsigned short usEventID ) const
{
	BroadCastAllGS( usEventID, char() );
}

int KLoginLocalRankingManager::CheckChangedInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const
{
	bool bDataChanged = false;

	std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		// ī�װ��� �ٲ�ų� ����/������� ��ȯ�� �� ī�װ� ���� ó���� �Ѵ�.
		if( ( SEnum::GetCategoryIndex( itUser->second.m_kInfo.m_iCategory ) != SEnum::GetCategoryIndex( kUserInfo.m_iCategory ) )
		 || ( SEnum::CheckFlag( itUser->second.m_kInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) != SEnum::CheckFlag( kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) )
		 )
		{
			// ī�װ� ������ �����Ǿ���.
			return LRM_CATEGORY_CHANGED;
		}
		else
		{
			bDataChanged |= !( itUser->second.m_kInfo == kUserInfo );
		}
	}

	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		if( itUnit->second.m_kInfo.m_iPoint[SEnum::LRMTC_SPIRIT] != kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] )
		{
			return LRM_SPIRIT_CHANGED;
		}
		else if( itUnit->second.m_kInfo.m_iPoint[SEnum::LRMTC_AP] != kUnitInfo.m_iPoint[SEnum::LRMTC_AP] )
		{
			return LRM_AP_CHANGED;
		}
		else
		{
			bDataChanged |= !( itUnit->second.m_kInfo == kUnitInfo );
		}
	}
	else
	{
		if( ( 0 < kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] ) || ( 0 < kUnitInfo.m_iPoint[SEnum::LRMTC_AP] ) )
			return LRM_INSERT_DATA;
		else
			return LRM_NONE;
	}

	if( bDataChanged == true )
	{
		return LRM_DATA_CHANGED;
	}

	return LRM_NONE;
}

bool KLoginLocalRankingManager::SomethingChanged( IN const int iUID
												, IN const KLocalRankingUserInfo& kUserInfo
												, IN const KLocalRankingUnitInfo& kUnitInfo
												, OUT std::map<UidType, bool>& mapChangeInfo
												, OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
												)
{
	if( CheckInit() == false )
	{
		// �ʱ�ȭ���� ���� ����
		return false;
	}

	if( CheckUID( iUID ) == false )
	{
		// UID ����ġ
		return false;
	}

	// ������ �ٲ���°�?
	const int iChangeFlag = CheckChangedInfo( kUserInfo, kUnitInfo );
	switch( iChangeFlag )
	{
	case LRM_CATEGORY_CHANGED:
		{
			// 1. ���� ī�װ� ������ �ٲ����.
			// ���� ���� ����Ʈ �纻�� ��������,
			std::set< UidType > setUnitList;
			std::map< UidType, std::set< UidType > >::const_iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
			if( itUnitList != m_mapUserUnitList.end() )
			{
				setUnitList = itUnitList->second;
			}
			
			// ���� ���� UnitUID�� ������ ��,
			setUnitList.erase( kUnitInfo.m_iUnitUID );

			KLocalRankingUserInfo kPrevUserInfo;
			KLocalRankingUnitInfo kPrevUnitInfo;

			if( GetUserInfo( kUserInfo.m_iUserUID, kPrevUserInfo ) == false )
			{
				return false;
			}

			// �纻�� ���� ������ ����, ��� ���ֿ� ���� RankingProcess�� �����Ѵ�.
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setUnitList )
			{
				// UnitInfo�� �纻�� ����ؾ� �Ѵ�. ( RankingProcess ���ο��� ���� ������ �߻����״�... )
				if( GetUnitInfo( iUnitUID, kPrevUnitInfo ) == false )
				{
					continue;
				}

				RankingProcess( iChangeFlag, kPrevUserInfo, kPrevUnitInfo, kUserInfo, kPrevUnitInfo, mapChangeInfo, vecRankUpNot );
			}

			if( GetUnitInfo( kUnitInfo.m_iUnitUID, kPrevUnitInfo ) == true )
			{
				RankingProcess( iChangeFlag, kPrevUserInfo, kPrevUnitInfo, kUserInfo, kUnitInfo, mapChangeInfo, vecRankUpNot );
			}
		}
		break;

	case LRM_SPIRIT_CHANGED:
	case LRM_AP_CHANGED:
		{
			// 2. ���� ELP ������ �ٲ����.
			// RankingProcess�� �����Ѵ�.
			KLocalRankingUserInfo kPrevUserInfo;
			KLocalRankingUnitInfo kPrevUnitInfo;

			if( GetUserInfo( kUserInfo.m_iUserUID, kPrevUserInfo ) == false )
			{
				return false;
			}

			if( GetUnitInfo( kUnitInfo.m_iUnitUID, kPrevUnitInfo ) == false )
			{
				return false;
			}

			RankingProcess( iChangeFlag, kPrevUserInfo, kPrevUnitInfo, kUserInfo, kUnitInfo, mapChangeInfo, vecRankUpNot );
		}
		break;

	case LRM_INSERT_DATA:
		RankingProcess( iChangeFlag, kUserInfo, kUnitInfo, kUserInfo, kUnitInfo, mapChangeInfo, vecRankUpNot );
		break;

	case LRM_DATA_CHANGED:
		// 3. ��Ÿ ������ �ٲ����.
		// ������ ����.
		DataUpdate( kUserInfo, kUnitInfo );
		break;

	default:
		// �ƹ��͵� ���� �ʴ´�.
		return false;
	}

	return true;
}

void KLoginLocalRankingManager::RankingProcess( IN const int iChangeFlag
											  , IN const KLocalRankingUserInfo& kPrevUserInfo
											  , IN const KLocalRankingUnitInfo& kPrevUnitInfo
											  , IN const KLocalRankingUserInfo& kUserInfo
											  , IN const KLocalRankingUnitInfo& kUnitInfo
											  , OUT std::map<UidType, bool>& mapChangeInfo
											  , OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
											  )
{
	bool bRankerBefore = false;
	const int iPrevCategoryIndex = SEnum::GetCategoryIndex( kPrevUserInfo.m_iCategory );
	const int iNextCategoryIndex = ( SEnum::CheckFlag( kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == true ) ? SEnum::LRLC_NONE : SEnum::GetCategoryIndex( kUserInfo.m_iCategory );

	// �̹� ��Ŀ�� �����ΰ�? ���� ��Ŀ��� �Ҽӵ� ī�װ� �ε����� ���Ѵ�.
	if( m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID ) != m_mapRankerUnitInfo.end() )
	{
		bRankerBefore = true;
	}

	int iMin;
	int iMax;

	if( ( iChangeFlag == LRM_CATEGORY_CHANGED ) || ( iChangeFlag == LRM_INSERT_DATA ) )
	{
		iMin = SEnum::LRMTC_NONE + 1;
		iMax = SEnum::LRMTC_MAX;
	}
	else if( iChangeFlag == LRM_SPIRIT_CHANGED )
	{
		iMin = SEnum::LRMTC_SPIRIT;
		iMax = iMin + 1;
	}
	else if( iChangeFlag == LRM_AP_CHANGED )
	{
		iMin = SEnum::LRMTC_AP;
		iMax = iMin + 1;
	}

	for( int i = iMin; i < iMax; ++i )
	{
		// ���� ��ũ ���
		UidType iNextUnitUID	= 0;
		int		iPrevRank		= GetRankAndNextUnitUID( i, iPrevCategoryIndex, kUnitInfo.m_iUnitUID, iNextUnitUID );
		int		iNextRank		= 0;

		// ��ũ ó��
		Remove( i, iPrevCategoryIndex, kPrevUserInfo, kPrevUnitInfo );
		if( Add( i, iNextCategoryIndex, kUnitInfo.m_iPoint[i], kUserInfo, kUnitInfo ) == true )
		{
			// ���� ��ũ ��� ( iterator ���� )
			iNextRank			= GetRankAndNextUnitUID( i, iNextCategoryIndex, kUnitInfo.m_iUnitUID, iNextUnitUID );
		}

		// ��ŷ�� ����Ͽ��ٸ�, ��Ŷ�� �ۼ��Ͽ� ��ȯ�� �����̳ʿ� ��´�.
		// ����( ��ŷ 0 )�� ���� ����
		if( ( 0 < iNextRank ) && ( ( iNextRank < iPrevRank ) || ( iPrevRank == 0 ) ) )
		{
			KELG_LOCAL_RANKING_RANK_UP_NOT kPacket;
			kPacket.m_iMyRank		= iNextRank;
			kPacket.m_cMainTabIndex	= i;
			kPacket.m_cSubTabIndex	= SEnum::LRSTC_LOCATION;
			GetUnitInfo( kUnitInfo.m_iUnitUID, kPacket.m_kMyUnitInfo );
			GetUnitInfo( iNextUnitUID, kPacket.m_kNextUnitInfo );
			vecRankUpNot.push_back( kPacket );
		}
	}

	bool bRankerAfter = ( m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID ) != m_mapRankerUnitInfo.end() );

	if( bRankerBefore != bRankerAfter )
	{
		// ������ ��Ŀ�� �����߰ų� ������� ����̴�.
		mapChangeInfo.insert( std::map<UidType, bool>::value_type( kUnitInfo.m_iUnitUID, bRankerAfter ) );
	}
}

void KLoginLocalRankingManager::DataInsert( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	// ���� ���� ó��
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser == m_mapRankerUserInfo.end() )
	{
		KLocalRankingUserInfoCounter kUserInfoCount;
		itUser = m_mapRankerUserInfo.insert( std::map<UidType, KLocalRankingUserInfoCounter>::value_type( kUserInfo.m_iUserUID, kUserInfoCount ) ).first;
	}

	itUser->second.m_kInfo = kUserInfo;
	++itUser->second.m_iCounter;

	// ���� ���� ó��
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit == m_mapRankerUnitInfo.end() )
	{
		KLocalRankingUnitInfoCounter kUnitInfoCount;
		itUnit = m_mapRankerUnitInfo.insert( std::map<UidType, KLocalRankingUnitInfoCounter>::value_type( kUnitInfo.m_iUnitUID, kUnitInfoCount ) ).first;
	}

	itUnit->second.m_iUserUID	= kUserInfo.m_iUserUID;
	itUnit->second.m_kInfo		= kUnitInfo;
	++itUnit->second.m_iCounter;

	// ������ ���� ���� �̾��ֱ�
	std::map< UidType, std::set< UidType > >::iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
	if( itUnitList == m_mapUserUnitList.end() )
	{
		std::set<UidType> setUnitList;
		itUnitList = m_mapUserUnitList.insert( std::map< UidType, std::set< UidType > >::value_type( kUserInfo.m_iUserUID, setUnitList ) ).first;
	}

	itUnitList->second.insert( kUnitInfo.m_iUnitUID );
}

void KLoginLocalRankingManager::DataRemove( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	int iRemainUserInfoCount = 0;

	// ���� ���� ó��
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		if( 1 < itUser->second.m_iCounter )
		{
			--itUser->second.m_iCounter;
			iRemainUserInfoCount = itUser->second.m_iCounter;
		}
		else
		{
			m_mapRankerUserInfo.erase( itUser );
		}
	}

	// ���� ���� ó��
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		if( 1 < itUnit->second.m_iCounter )
		{
			--itUnit->second.m_iCounter;
		}
		else
		{
			m_mapRankerUnitInfo.erase( itUnit );
		}
	}
	
	// ������ ���� ���� �����ϱ�
	if( 0 < iRemainUserInfoCount )
	{
		std::map< UidType, std::set< UidType > >::iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
		if( itUnitList != m_mapUserUnitList.end() )
		{
			itUnitList->second.erase( kUnitInfo.m_iUnitUID );
		}
	}
	else
	{
		m_mapUserUnitList.erase( kUserInfo.m_iUserUID );
	}
}

void KLoginLocalRankingManager::DataUpdate( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	// ���� ���� ����
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		itUser->second.m_kInfo = kUserInfo;
	}

	// ���� ���� ����
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		itUnit->second.m_kInfo = kUnitInfo;
	}
}

bool KLoginLocalRankingManager::Remove( IN const int iMainTabIndex
									  , IN int iCategory
									  , IN const KLocalRankingUserInfo& kUserInfo
									  , IN const KLocalRankingUnitInfo& kUnitInfo
									  )
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return false;
	}

	return Remove( itRanker->second, kUserInfo, kUnitInfo );
}

bool KLoginLocalRankingManager::Add( IN const int iMainTabIndex
									, IN int iCategory
									, IN const int iAccumulatedPoint
									, IN const KLocalRankingUserInfo& kUserInfo
									, IN const KLocalRankingUnitInfo& kUnitInfo
									)
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		std::pair< std::map< std::pair< int, int >, RankMMAP >::iterator, bool> pairResult = m_mapRankerTab.insert( std::map< std::pair< int, int >, RankMMAP >::value_type( std::make_pair( iMainTabIndex, iCategory ), RankMMAP() ) );
		if( pairResult.second == false )
		{
			return false;
		}

		itRanker = pairResult.first;
	}

	return Add( itRanker->second, iAccumulatedPoint, kUserInfo, kUnitInfo );
}

bool KLoginLocalRankingManager::Remove( IN RankMMAP& mmapRanker
									  , IN const KLocalRankingUserInfo& kUserInfo
									  , IN const KLocalRankingUnitInfo& kUnitInfo
									  )
{
	RankMMAP::iterator it;
	for( it = mmapRanker.begin(); it != mmapRanker.end(); ++it )
	{
		if( it->second == kUnitInfo.m_iUnitUID )
		{
			break;
		}
	}

	// �ش��ϴ� ������ ���ٸ� ��ƾ ����
	if( it == mmapRanker.end() )
	{
		return false;
	}

	// ��ũ���� ����
	mmapRanker.erase( it );

	DataRemove( kUserInfo, kUnitInfo );

	return true;
}

bool KLoginLocalRankingManager::Add( IN RankMMAP& mmapRanker
								   , IN const int iAccumulatedPoint
								   , IN const KLocalRankingUserInfo& kUserInfo
								   , IN const KLocalRankingUnitInfo& kUnitInfo
								   )
{
	if( SEnum::GetCategoryIndex( kUserInfo.m_iCategory ) == SEnum::LRLC_NONE )
	{
		return false;
	}

	if( iAccumulatedPoint <= 0 )
	{
		return false;
	}

	if( static_cast<size_t>( SiKGameSysVal()->GetLocalRankingNumOfRanker() ) <= mmapRanker.size() )
	{
		// �������� �ѾƳ��� ��Ŀ�� �� �� �ִ� �����̴�.
		RankMMAP::reverse_iterator rit = mmapRanker.rbegin();
		if( rit != mmapRanker.rend() )
		{
			if( iAccumulatedPoint < rit->first )
			{
				// ��ŷ�� �� �� ���� ELP�̴�.
				return false;
			}
		}
	}

	// ������ �������, ���߿� ���Ե� ����� ���� ��ũ�� ��ġ ��Ų��.
	//RankMMAP::iterator it = mmapRanker.find( iAccumulatedPoint );
	//mmapRanker.insert( it, RankMMAP::value_type( iAccumulatedPoint, kUnitInfo.m_iUnitUID ) );
	mmapRanker.insert( RankMMAP::value_type( iAccumulatedPoint, kUnitInfo.m_iUnitUID ) );

	// ���� ���� ó��
	DataInsert( kUserInfo, kUnitInfo );

	return true;
}


void KLoginLocalRankingManager::SizeProcess( void )
{
	KDBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT kPacket;

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker;
	for( itRanker = m_mapRankerTab.begin(); itRanker != m_mapRankerTab.end(); ++itRanker )
	{
		_SizeProcess( itRanker->second, kPacket.m_mapChangeInfo );
	}

	if( kPacket.m_mapChangeInfo.empty() == false )
	{
		SendToGameDB( DBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT, kPacket );
	}
}

void KLoginLocalRankingManager::_SizeProcess( IN OUT RankMMAP& mmapRanker, IN OUT std::map<UidType, bool>& mapChangeInfo )
{
	// ���ڸ��� ���ƴ��� Ȯ���Ѵ�.
	int iGab = mmapRanker.size() - SiKGameSysVal()->GetLocalRankingNumOfRanker();
	if( iGab <= 0 )
	{
		return;
	}

	// ���������� ��ģ ����ŭ �����Ѵ�.
	for( int i = 0; i < iGab; ++i )
	{
		RankMMAP::reverse_iterator rit = mmapRanker.rbegin();
		if( rit == mmapRanker.rend() )
		{
			break;
		}

		const UidType iUnitUID = rit->second;
		
		// UnitInfo ���ϱ�
		std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( iUnitUID );
		if( itUnit == m_mapRankerUnitInfo.end() )
		{
			continue;
		}
		const KLocalRankingUnitInfo kUnitInfo = itUnit->second.m_kInfo;

		// UserInfo ���ϱ�
		std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
		if( itUser == m_mapRankerUserInfo.end() )
		{
			continue;
		}
		const KLocalRankingUserInfo kUserInfo = itUser->second.m_kInfo;

		// ���� ó�� �� ��ŷ �������� ������ ������ٸ�, �����̳ʿ� ��´�.
		Remove( mmapRanker, kUserInfo, kUnitInfo );
		if( m_mapRankerUnitInfo.find( iUnitUID ) == m_mapRankerUnitInfo.end() )
		{
			mapChangeInfo.insert( std::map<UidType, bool>::value_type( iUnitUID, false ) );
		}
	}
}

int KLoginLocalRankingManager::GetRankAndNextUnitUID( IN const int iMainTabIndex, IN const int iCategory, IN const UidType iMyUnitUID, OUT UidType& iNextUnitUID ) const
{
	int iRank		= 0;
	iNextUnitUID	= 0;

	std::map< std::pair< int, int >, RankMMAP >::const_iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return iRank;
	}

	const RankMMAP& mmapRanker = itRanker->second;

	RankMMAP::const_iterator it;
	for( it = mmapRanker.begin(); it != mmapRanker.end(); ++it )
	{
		++iRank;
		if( it->second == iMyUnitUID )
		{
			break;
		}
	}

	if( it == mmapRanker.end() )
	{
		return 0;
	}

	if( ++it == mmapRanker.end() )
	{
		iNextUnitUID = 0;
		return iRank;
	}

	iNextUnitUID = it->second;
	return iRank;
}

bool KLoginLocalRankingManager::GetUserInfo( IN const UidType iUserUID, OUT KLocalRankingUserInfo& kInfo ) const
{
	std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator it = m_mapRankerUserInfo.find( iUserUID );
	if( it != m_mapRankerUserInfo.end() )
	{
		kInfo = it->second.m_kInfo;
		return true;
	}

	return false;
}

bool KLoginLocalRankingManager::GetUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUnitInfo& kInfo ) const
{
	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator it = m_mapRankerUnitInfo.find( iUnitUID );
	if( it != m_mapRankerUnitInfo.end() )
	{
		kInfo = it->second.m_kInfo;
		return true;
	}

	return false;
}

int KLoginLocalRankingManager::GetInitInfo( IN const int iNumOnce
										  , IN OUT std::map<UidType, KLocalRankingUnitInfo>::const_iterator& itUnitEnd
										  , IN const std::map<UidType, KLocalRankingUnitInfo>& inMapUnitInfo
										  , OUT std::map<UidType, KLocalRankingUnitInfo>& outMapUnitInfo
										  , IN OUT std::map<UidType, UidType>::const_iterator& itUIDEnd
										  , IN const std::map<UidType, UidType>& inMapUIDInfo
										  , OUT std::map<UidType, UidType>& outMapUIDInfo
										  , IN OUT std::map<UidType, KLocalRankingUserInfo>::const_iterator& itUserEnd
										  , IN const std::map<UidType, KLocalRankingUserInfo>& inMapUserInfo
										  , OUT std::map<UidType, KLocalRankingUserInfo>& outMapUserInfo
										  )
{
	if( itUnitEnd != inMapUnitInfo.end() )
	{
		// begin, end ����
		std::map<UidType, KLocalRankingUnitInfo>::const_iterator itUnitBegin = itUnitEnd;
		for( int i = 0; i < iNumOnce; ++i )
		{
			if( ++itUnitEnd == inMapUnitInfo.end() )
			{
				break;
			}
		}

		// ����
		outMapUnitInfo.insert( itUnitBegin, itUnitEnd );
	}

	if( itUIDEnd != inMapUIDInfo.end() )
	{
		// begin, end ����
		std::map<UidType, UidType>::const_iterator itUIDBegin = itUIDEnd;
		for( int i = 0; i < iNumOnce; ++i )
		{
			if( ++itUIDEnd == inMapUIDInfo.end() )
			{
				break;
			}
		}

		// ����
		outMapUIDInfo.insert( itUIDBegin, itUIDEnd );
	}

	if( itUserEnd != inMapUserInfo.end() )
	{
		// begin, end ����
		std::map<UidType, KLocalRankingUserInfo>::const_iterator itUserBegin = itUserEnd;
		for( int i = 0; i < iNumOnce; ++i )
		{
			if( ++itUserEnd == inMapUserInfo.end() )
			{
				break;
			}
		}

		// ����
		outMapUserInfo.insert( itUserBegin, itUserEnd );
	}

	if( ( itUnitEnd != inMapUnitInfo.end() ) ||
		( itUIDEnd != inMapUIDInfo.end() ) ||
		( itUserEnd != inMapUserInfo.end() )
		)
	{
		return NetError::ERR_UNKNOWN;
	}

	return NetError::NET_OK;
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT )
{
	KDBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT	kPacketToDB;
	std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>	vecRankUpNot;

	if( SomethingChanged( kPacket_.m_iUID, kPacket_.m_kUserInfo, kPacket_.m_kUnitInfo, kPacketToDB.m_mapChangeInfo, vecRankUpNot ) == false )
	{
		return;
	}

	if( vecRankUpNot.empty() == false )
	{
		BOOST_TEST_FOREACH( const KELG_LOCAL_RANKING_RANK_UP_NOT&, kPacket, vecRankUpNot )
		{
			SendToGSUser( FIRST_SENDER_UID, kPacket_.m_kUserInfo.m_iUserUID, ELG_LOCAL_RANKING_RANK_UP_NOT, kPacket );
		}
	}

	if( kPacketToDB.m_mapChangeInfo.empty() == false )
	{
		SendToGameDB( DBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT, kPacketToDB );
	}

	// ��� ���� ������ ��ε� ĳ����
	BroadCastAllGS( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ��ŷ �ý��� �ʱ�ȭ ����" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
		return;
	}

	Init( kPacket_.m_iUID
		, kPacket_.m_wstrResetTime
		, kPacket_.m_mapRankerUnitInfo
		, kPacket_.m_mapRankerUIDInfo
		, kPacket_.m_mapRankerUserInfo
		);

	// ���Ӽ����� ��ε� ĳ����
	std::map<UidType, KLocalRankingUnitInfo>::const_iterator itUnitEnd = kPacket_.m_mapRankerUnitInfo.begin();
	std::map<UidType, UidType>::const_iterator itUIDEnd = kPacket_.m_mapRankerUIDInfo.begin();
	std::map<UidType, KLocalRankingUserInfo>::const_iterator itUserEnd = kPacket_.m_mapRankerUserInfo.begin();

	while( 1 )
	{
		KELG_LOCAL_RANKING_RESET_NOT kPacket;
		kPacket.m_iUID = kPacket_.m_iUID;

		kPacket.m_iOK = GetInitInfo( 100
			, itUnitEnd
			, kPacket_.m_mapRankerUnitInfo
			, kPacket.m_mapRankerUnitInfo
			, itUIDEnd
			, kPacket_.m_mapRankerUIDInfo
			, kPacket.m_mapRankerUIDInfo
			, itUserEnd
			, kPacket_.m_mapRankerUserInfo
			, kPacket.m_mapRankerUserInfo
			);

		BroadCastAllGS( ELG_LOCAL_RANKING_RESET_NOT, kPacket );

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			break;
		}
	}
}

IMPL_ON_FUNC( DBE_LOCAL_RANKING_RESET_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ��ŷ �ý��� ���� ���� ���� �� �ʱ�ȭ ����" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
		return;
	}

	std::map<UidType, KLocalRankingUnitInfo>	mapRankerUnitInfo;
	std::map<UidType, UidType>					mapRankerUIDInfo;
	std::map<UidType, KLocalRankingUserInfo>	mapRankerUserInfo;
	Init( kPacket_.m_iUID
		, kPacket_.m_wstrUpdated
		, mapRankerUnitInfo
		, mapRankerUIDInfo
		, mapRankerUserInfo
		);

	// ���Ӽ����� ��ε� ĳ����
	KELG_LOCAL_RANKING_RESET_NOT kPacket;
	kPacket.m_iUID = kPacket_.m_iUID;
	BroadCastAllGS( ELG_LOCAL_RANKING_RESET_NOT, kPacket );
}

_IMPL_ON_FUNC( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, int )
{
	if( CheckInit() == false )
	{
		// �ʱ�ȭ���� ���� ����
		return;
	}

	if( CheckUID( kPacket_ ) == true )
	{
		// UID�� ��ġ�Ѵ�. ������ �ʿ䰡 ����.
		return;
	}

	// ���Ӽ����� ����
	std::map<UidType, KLocalRankingUnitInfo>::const_iterator itUnitEnd = m_mapBackUpUnitInfo.begin();
	std::map<UidType, UidType>::const_iterator itUIDEnd = m_mapBackUpUIDInfo.begin();
	std::map<UidType, KLocalRankingUserInfo>::const_iterator itUserEnd = m_mapBackUpUserInfo.begin();

	while( 1 )
	{
		KELG_LOCAL_RANKING_RESET_NOT kPacket;
		kPacket.m_iUID = m_iUID;

		kPacket.m_iOK = GetInitInfo( 100
			, itUnitEnd
			, m_mapBackUpUnitInfo
			, kPacket.m_mapRankerUnitInfo
			, itUIDEnd
			, m_mapBackUpUIDInfo
			, kPacket.m_mapRankerUIDInfo
			, itUserEnd
			, m_mapBackUpUserInfo
			, kPacket.m_mapRankerUserInfo
			);

		SendToGameServer( FIRST_SENDER_UID, ELG_LOCAL_RANKING_RESET_NOT, kPacket );

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			break;
		}
	}
}

_IMPL_ON_FUNC( DBE_LOCAL_RANKING_RESET_CHECK_ACK, int )
{
	if( CheckInit() == false )
	{
		// �ʱ�ȭ���� ���� ����
		return;
	}

	if( CheckUID( kPacket_ ) == true )
	{
		// UID�� ��ġ�Ѵ�. ������ �ʿ䰡 ����.
		return;
	}

	// ���� ��ȣ ����
	// ������ �Ϸ�� ������ ��� ��û���� �������� �ʴ´�.
	// UID�� 0�� �ƴ� ������ ����� �ٸ� ���� �ǵ��� �Ѵ�.
	while( ++m_iUID == 0 );
	SendToGameDB( DBE_LOCAL_RANKING_RESET_REQ, m_iUID );
	clear();
}

IMPL_ON_FUNC_NOPARAM( ELG_LOCAL_RANKING_RESET_NOT )
{
	// ���� ��ȣ ����
	// ������ �Ϸ�� ������ ��� ��û���� �������� �ʴ´�.
	// UID�� 0�� �ƴ� ������ ����� �ٸ� ���� �ǵ��� �Ѵ�.
	while( ++m_iUID == 0 );
	SendToGameDB( DBE_LOCAL_RANKING_RESET_REQ, m_iUID );
	clear();
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT )
{
	const int iMainTabIndex	= kPacket_.m_iMainTabIndex;
	const int iSubTabIndex	= kPacket_.m_iSubTabIndex;

	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		START_LOG( cerr, L"��ȿ���� ���� iMainTabIndex �Դϴ�." )
			<< BUILD_LOG( iMainTabIndex )
			<< END_LOG;
		return;
	}

	if( ( iSubTabIndex <= SEnum::LRLC_NONE ) || ( SEnum::LRLC_MAX <= iSubTabIndex ) )
	{
		START_LOG( cerr, L"��ȿ���� ���� iSubTabIndex �Դϴ�." )
			<< BUILD_LOG( iSubTabIndex )
			<< END_LOG;
		return;
	}

	START_LOG( cout, L"���� ��ŷ �ý��� ���� ����" );

	std::map< std::pair< int, int >, RankMMAP >::const_iterator itTab = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iSubTabIndex ) );
	if( itTab != m_mapRankerTab.end() )
	{
		START_LOG( cout, L"��" )
			<< BUILD_LOG( iMainTabIndex )
			<< BUILD_LOG( iSubTabIndex );

		const RankMMAP& mmapRanker = itTab->second;
		RankMMAP::const_iterator itRank;
		int	iRank = 0;
		const int iMainTabIndex = itTab->first.first;
		for( itRank = mmapRanker.begin(); itRank != mmapRanker.end(); ++itRank )
		{
			++iRank;

			// ���� ������ ���������� �����ϴ°�?
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( itRank->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				START_LOG( cout2, L"���� ��ŷ ���� ���� �̻� ����" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second );
				continue;
			}

			// ���� ������ ���������� �����ϴ°�?
			if( m_mapRankerUserInfo.find( itUnit->second.m_iUserUID ) == m_mapRankerUserInfo.end() )
			{
				START_LOG( cout2, L"���� ��ŷ ���� ���� �̻� ����" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second );
				continue;
			}

			// �� ���� ��Ŀ�� �����ϴ°�?
			RankMMAP::const_iterator itRankNext = itRank;
			if( ++itRankNext == mmapRanker.end() )
			{
				continue;
			}

			// ��ũ ���̺����� ��ŷ ����Ʈ�� ������ �°� �� �ִ°�?
			if( itRank->first < itRankNext->first )
			{
				START_LOG( cout2, L"���� ��ŷ ����Ʈ ���� �̻� ����" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second );

				START_LOG( cout2, L"���� ����" )
					<< BUILD_LOG( iRank + 1 )
					<< BUILD_LOG( itRankNext->first )
					<< BUILD_LOG( itRankNext->second );
				continue;
			}

			// �� ���� ��Ŀ�� ���� ������ �����ϴ°�?
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnitNext = m_mapRankerUnitInfo.find( itRankNext->second );
			if( itUnitNext == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			// ���� ī�װ� �ε����� ��ȿ�Ѱ�?
			if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) ||
				( SEnum::LRMTC_MAX <= iMainTabIndex )
				)
			{
				continue;
			}

			// ���� ���������� ��ŷ ����Ʈ�� ������ �°� �� �ִ°�?
			if( itUnit->second.m_kInfo.m_iPoint[iMainTabIndex] < itUnitNext->second.m_kInfo.m_iPoint[iMainTabIndex] )
			{
				START_LOG( cout2, L"���� ��ŷ ����Ʈ ���� �̻� ����" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second )
					<< BUILD_LOG( itUnit->second.m_kInfo.m_iPoint[iMainTabIndex] );

				START_LOG( cout2, L"���� ����" )
					<< BUILD_LOG( iRank + 1 )
					<< BUILD_LOG( itRankNext->first )
					<< BUILD_LOG( itRankNext->second )
					<< BUILD_LOG( itUnitNext->second.m_kInfo.m_iPoint[iMainTabIndex] );
				continue;
			}
		}
	}

	START_LOG( cout, L"���� ��ŷ �ý��� ���� ����" );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
