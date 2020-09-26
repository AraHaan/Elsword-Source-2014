#include "Enum/Enum.h"
#include "GSUser.h"
#include ".\userstatistics.h"
//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	#include "GameSysVal/GameSysVal.h"
#endif SERV_CHAR_LOG
//}}


KUserStatistics::KUserStatistics(void)
:
m_bIsNormalUser( true ) // �⺻������ �������ǵ��� �ʱ�ȭ. -> ��ڴ� false��
{
	//{{ 2007. 11. 19  ������  ���� �÷��� Ÿ�� ���
	m_iStatisticsPlayTime[PT_TOTAL_PLAY_TIME]	= 0;
	m_iStatisticsPlayTime[PT_DUNGEON_PLAY_TIME]	= 0;
	m_iStatisticsPlayTime[PT_PVP_PLAY_TIME]		= 0;
	//{{ 2012. 11. 14	�ڼ���	Field PT �α� �߰�
#ifdef SERV_FIELD_PLAY_TIME_LOG
	m_iStatisticsPlayTime[PT_SPIRIT_PLAY_TIME]	= 0;
	m_iStatisticsPlayTime[PT_FIELD_PLAY_TIME]	= 0;
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}

	//}}
	//{{ 2011. 02. 23	������	���� ���� ī��Ʈ
#ifdef SERV_ACCOUNT_COUNT
	for( int iIndex = 0; iIndex < AGCT_MAX; ++iIndex )
	{
		m_iAccountGameCount[iIndex].SetValue( 0 );
	}
	m_bIsAccountCount = false;
#endif SERV_ACCOUNT_COUNT
	//}}

	Clear();
}

KUserStatistics::~KUserStatistics(void)
{
}

void KUserStatistics::SetIsNormalUser( char cAuthLevel )
{
	m_bIsNormalUser = ( cAuthLevel == SEnum::UAL_NORMAL || cAuthLevel == SEnum::UAL_BLOCKED_1 );
}

void KUserStatistics::GetCharacterStatistics( std::map< std::pair< int, int >, std::vector< int > >& mapPvpStatistics,
											  std::map< std::pair< int, int >, std::vector< int > >& mapDungeonStatistics,
											  std::map< std::pair< int, int >, std::vector< int > >& mapTCenterStatistics,
											  std::map< std::pair< int, int >, std::vector< int > >& mapEtcStatistics 
#ifdef SERV_USER_STATISTICS_RENEWAL
											  , std::map< std::pair< int, int >, std::vector< int > >& mapEDStatistics 
#endif //SERV_USER_STATISTICS_RENEWAL
											  )
{
	mapPvpStatistics	 = m_mapPvpStatistics;
	mapDungeonStatistics = m_mapDungeonStatistics;
	mapTCenterStatistics = m_mapTCenterStatistics;
	mapEtcStatistics	 = m_mapEtcStatistics;
#ifdef SERV_USER_STATISTICS_RENEWAL
	mapEDStatistics		= m_mapEDStatistics;
#endif //SERV_USER_STATISTICS_RENEWAL
}

void KUserStatistics::IncreaseCount( int iTableID, int iSubTableID , int iColumn, int iCount )
{
	// ��ڱ� �̻��̶�� ��� ���� �ȵ�
	if( m_bIsNormalUser == false )
		return;

	UserStatisticsKey key( iTableID, iSubTableID );

	// ������Ʈ�� �����͸� vector�� ��ȯ
	std::vector< int > vecCount;
	for( int i = 0; i < iColumn; i++ )
	{
		vecCount.push_back( 0 );
	}
	vecCount.push_back( iCount );

	switch( iTableID )
	{
	case USTable_PvpModeCount:
	case USTable_PvpPlayTime:
		IncreaseCount( m_mapPvpStatistics ,key, vecCount );
		break;

	case USTable_Dungeon:
		IncreaseCount( m_mapDungeonStatistics, key, vecCount );
		break;

	case USTable_TrainingCenter:
		IncreaseCount( m_mapTCenterStatistics, key, vecCount );
		break;

	case USTable_EtcData:
		IncreaseCount( m_mapEtcStatistics, key, vecCount );
		break;
#ifdef SERV_USER_STATISTICS_RENEWAL
	case USTable_EDData:
		IncreaseCount( m_mapEDStatistics, key, vecCount );
		break;
#endif //SERV_USER_STATISTICS_RENEWAL
	default:
		START_LOG( cerr, L"���� ��� : ���������ʴ� ���̺�ID�Դϴ�." )
			<< BUILD_LOG( iTableID )
			<< END_LOG;

		break;
	}
}

void KUserStatistics::IncreaseCount( std::map< std::pair< int, int >, std::vector< int > >& mapStatistics, UserStatisticsKey& key, std::vector< int >& vecCount )
{
	// ��� ������ ������Ʈ
	UserStatisticsIterator mit = mapStatistics.find( key );
	if( mit == mapStatistics.end() )
	{
		mapStatistics.insert( std::make_pair( key, vecCount ) );
	}
	else
	{
		std::vector< int >::const_iterator vitSrc = vecCount.begin();
		std::vector< int >::iterator vitDest = mit->second.begin();

		while( vitSrc != vecCount.end() )
		{
			if( vitDest == mit->second.end() )
			{
				while( vitSrc != vecCount.end() )
				{
					mit->second.push_back( *vitSrc );
					vitSrc++;
				}
			}
			else
			{
				( *vitDest ) += ( *vitSrc );
				vitSrc++;
				vitDest++;
			}
		}
	}
}

void KUserStatistics::Clear()
{
    m_mapPvpStatistics.clear();
	m_mapDungeonStatistics.clear();
	m_mapTCenterStatistics.clear();
	m_mapEtcStatistics.clear();
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_mapEDStatistics.clear();
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2010. 06. 30  ������	�ټ��� ���
#ifdef SERV_SPIRIT_STATISTICS
	m_iUsedSpirit = 0;
#endif SERV_SPIRIT_STATISTICS
	//}}
	//{{ 2011. 01. 16	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	for( int iIndex = 0; iIndex < CGCT_MAX; ++iIndex )
	{
		m_iCharGameCount[iIndex].SetValue( 0 );
	}
	m_wstrUnitCreateDate.clear();
	m_bIsCharCount = false;
#endif SERV_CHAR_LOG
	//}}
}

void KUserStatistics::AddPlayTime( PLAY_TIME ePT, int iTotalPlayTime )
{
	if( iTotalPlayTime < 0  ||  iTotalPlayTime > 86400 ) // 24�ð�
	{
		START_LOG( cwarn, L"�� �������� ���� �÷��� Ÿ��!" )
			<< BUILD_LOG( ePT )
			<< BUILD_LOG( iTotalPlayTime )
			<< END_LOG;

		// ���� ���� �����������̱� ������ �����Ⱚ�� ���� �ȵȴ�.
		return; 
	}

	m_iStatisticsPlayTime[ePT] += iTotalPlayTime;
}

//{{ 2011. 02. 23	������	���� ���� ī��Ʈ
#ifdef SERV_ACCOUNT_COUNT
void KUserStatistics::InitAccountCountInfo( IN const std::wstring& wstrAccountRegDate, IN const std::map< int, int >& mapAccCountInfo )
{
	for( int iIndex = 0; iIndex < AGCT_MAX; ++iIndex )
	{
		std::map< int, int >::const_iterator mit;
		mit = mapAccCountInfo.find( iIndex );
		if( mit == mapAccCountInfo.end() )
			continue;

		m_iAccountGameCount[iIndex].SetValue( mit->second );
	}

	// ���� ���� ��¥ ����
	m_wstrAccountRegDate = wstrAccountRegDate;

	CTime tAccountRegDate;
	if( KncUtil::ConvertStringToCTime( wstrAccountRegDate, tAccountRegDate ) == false )
	{
		START_LOG( cerr, L"���� ���� ��¥ ������ �̻��մϴ�. ��ȯ ����!" )
			<< BUILD_LOG( wstrAccountRegDate )
			<< END_LOG;

		std::wstring wstrFailedDate = L"2000-01-01 00:00:00";
		LIF( KncUtil::ConvertStringToCTime( wstrFailedDate, tAccountRegDate ) );
	}

	m_bIsAccountCount = ( tAccountRegDate > SiKGameSysVal()->GetAccountStatisticsStartDate() );
}

void KUserStatistics::IncreaseAccountCount( IN const ACCOUNT_GAME_COUNT_TYPE eCountType, IN const int iCount /*= 1*/ )
{
	if( IsAccountCount() == false )
		return;

	if( eCountType < 0  ||  eCountType > AGCT_MAX )
	{
		START_LOG( cerr, L"���� ���� ���� ���Դϴ�!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return;
	}

	m_iAccountGameCount[eCountType] += iCount;
}

int KUserStatistics:: GetAccountCount( IN const ACCOUNT_GAME_COUNT_TYPE eCountType ) const
{
	if( eCountType < 0  ||  eCountType > AGCT_MAX )
	{
		START_LOG( cerr, L"���� ���� ���� ���Դϴ�!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return 0;
	}

	return m_iAccountGameCount[eCountType];
}

void KUserStatistics::GetAccountDBUpdateInfo( OUT std::map< int, int >& mapAccCountInfo )
{
	mapAccCountInfo.clear();

	if( IsAccountCount() == false )
		return;

	for( int iIndex = 0; iIndex < AGCT_MAX; ++iIndex )
	{
		if( m_iAccountGameCount[iIndex].IsChanged() == false )
			continue;

		const int iChangedValue = m_iAccountGameCount[iIndex].GetChangeValue();
		m_iAccountGameCount[iIndex].SetValue( iChangedValue );

		mapAccCountInfo.insert( std::make_pair( iIndex, iChangedValue ) );
	}
}
#endif SERV_ACCOUNT_COUNT
//}}

//{{ 2011. 01. 16	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
void KUserStatistics::InitCharacterCountInfo( IN const std::map< int, int >& mapCharCountInfo, IN const std::wstring& wstrUnitCreateDate )
{
	for( int iIndex = 0; iIndex < CGCT_MAX; ++iIndex )
	{
		std::map< int, int >::const_iterator mit;
		mit = mapCharCountInfo.find( iIndex );
		if( mit == mapCharCountInfo.end() )
			continue;

		m_iCharGameCount[iIndex].SetValue( mit->second );
	}

	// ĳ���� ���� ��¥ ����
	m_wstrUnitCreateDate = wstrUnitCreateDate;

	CTime tUnitCreateDate;
	if( KncUtil::ConvertStringToCTime( wstrUnitCreateDate, tUnitCreateDate ) == false )
	{
        START_LOG( cerr, L"ĳ���� ���� ��¥ ������ �̻��մϴ�. ��ȯ ����!" )
			<< BUILD_LOG( wstrUnitCreateDate )
			<< END_LOG;

		std::wstring wstrFailedDate = L"2000-01-01 00:00:00";
		LIF( KncUtil::ConvertStringToCTime( wstrFailedDate, tUnitCreateDate ) );
	}

	m_bIsCharCount = ( tUnitCreateDate > SiKGameSysVal()->GetCharStatisticsStartDate() );
}

void KUserStatistics::IncreaseCharacterCount( IN const CHAR_GAME_COUNT_TYPE eCountType, IN const int iCount /*= 1*/ )
{
	if( IsCharCount() == false )
		return;

    if( eCountType < 0  ||  eCountType > CGCT_MAX )
	{
		START_LOG( cerr, L"���� ���� ���� ���Դϴ�!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return;
	}

	m_iCharGameCount[eCountType] += iCount;
}

int	KUserStatistics::GetCharacterCount( IN const CHAR_GAME_COUNT_TYPE eCountType ) const
{
	if( eCountType < 0  ||  eCountType > CGCT_MAX )
	{
		START_LOG( cerr, L"���� ���� ���� ���Դϴ�!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return 0;
	}

    return m_iCharGameCount[eCountType];
}

void KUserStatistics::GetDBUpdateInfo( OUT std::map< int, int >& mapCharCountInfo )
{
	mapCharCountInfo.clear();

	if( IsCharCount() == false )
		return;

	for( int iIndex = 0; iIndex < CGCT_MAX; ++iIndex )
	{
		if( m_iCharGameCount[iIndex].IsChanged() == false )
			continue;

		const int iChangedValue = m_iCharGameCount[iIndex].GetChangeValue();
		m_iCharGameCount[iIndex].SetValue( iChangedValue );

		mapCharCountInfo.insert( std::make_pair( iIndex, iChangedValue ) );
	}    
}

void KUserStatistics::SetLastClearDungeonID( IN const int iDungeonID )
{
    m_iCharGameCount[CGCT_LAST_PLAY_DUNGEON_ID] += ( iDungeonID - m_iCharGameCount[CGCT_LAST_PLAY_DUNGEON_ID] );
}

#endif SERV_CHAR_LOG
//}}

//{{ 2011. 03. 22	������	ĳ���� ù ���� �α�
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
bool KUserStatistics::IsFirstCharSelectToday( IN const std::wstring& wstrUnitLastLoginDate )
{
	std::wstring wstrModifiedLastLoginDate;
	const bool bIsFirstSelect = ( GetCharacterCount( KUserStatistics::CGCT_SELECT_CHAR ) == 0 );
	if( bIsFirstSelect )
	{
		wstrModifiedLastLoginDate = L"2000-01-01 00:00:00";
	}
	else
	{
		wstrModifiedLastLoginDate = wstrUnitLastLoginDate;
	}

	CTime tCurTime = CTime::GetCurrentTime();
	CTime tLastLoginTime;
	if( KncUtil::ConvertStringToCTime( wstrModifiedLastLoginDate, tLastLoginTime ) == false )
	{
		START_LOG( cerr, L"������ ĳ���� ���ýð� ��ȯ ����!" )
			<< BUILD_LOG( wstrModifiedLastLoginDate )
			<< BUILD_LOG( wstrUnitLastLoginDate )
			<< END_LOG;
		return false;
	}
	
	CTime tCurToday = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 0, 0, 0 );
	CTime tLastLoginDay = CTime( tLastLoginTime.GetYear(), tLastLoginTime.GetMonth(), tLastLoginTime.GetDay(), 0, 0, 0 );

	// ���� ��¥�� ������ �α��� ��¥�� ���ߴµ� ���ú��� �� ������ �α��� ������ true�� ��ȯ!
	return ( tCurToday > tLastLoginDay );
}
#endif SERV_DAILY_CHAR_FIRST_SELECT
//}}

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
bool KUserStatistics::IsFirstUserLoginToday( IN const UidType& UserUID, IN const std::wstring& wstrUserLastLoginDate )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTime tLastLoginTime;
	if( KncUtil::ConvertStringToCTime( wstrUserLastLoginDate, tLastLoginTime ) == false )
	{
		START_LOG( clog, L"���� �α��� �ð� ��ȯ ����_���� ���� �ִ�!" )
			<< BUILD_LOG( UserUID )
			<< BUILD_LOG( wstrUserLastLoginDate )
			<< END_LOG;
		return false;
	}

	CTime tCurToday = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), 0, 0, 0 );
	CTime tLastLoginDay = CTime( tLastLoginTime.GetYear(), tLastLoginTime.GetMonth(), tLastLoginTime.GetDay(), 0, 0, 0 );

	// ���� ��¥�� ������ �α��� ��¥�� ���ߴµ� ���ú��� �� ������ �α��� ������ true�� ��ȯ!
	return ( tCurToday > tLastLoginDay );

}
#endif SERV_SECOND_SECURITY
//}}
