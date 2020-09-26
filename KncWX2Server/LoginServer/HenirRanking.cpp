#include ".\henirranking.h"
#include <algorithm>
#include "Enum/Enum.h"
//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	#include "LoginServer.h"
#endif SERV_INTEGRATION
//}}

KHenirRanking::KHenirRanking(void) :
m_uiLastRank( 0 ),
m_bIsRankingChange( false ),
m_iRankingType( -1 ),
m_iRefreshWeekOfMonth( 0 ),
m_iRefreshDayOfWeek( 0 ),
m_iRefreshHour( 0 )
{
}

KHenirRanking::~KHenirRanking(void)
{
}

void KHenirRanking::SetRefreshTime( const int iRankingType, const int iWeekOfMonth, const int iDayOfWeek, const int iHour )
{
	m_iRankingType = iRankingType;
	m_iRefreshWeekOfMonth = iWeekOfMonth;
	m_iRefreshDayOfWeek = iDayOfWeek;
	m_iRefreshHour = iHour;

	switch( m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
		{
			// �Ϻ� ��ŷ �ʱ�ȭ �ð� ����
			CTime tCurTime	 = CTime::GetCurrentTime();
			m_tRefreshTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0 );
			if( m_tRefreshTime < tCurTime )
			{
				// �̹� ������ �ð��̶�� �������� ����
				m_tRefreshTime += CTimeSpan( 1, 0, 0, 0 );
			}

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"�ϰ� ��ŷ ���� ���� ����!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_WEEK_RANKING:
		{				
			CTime tCurTime = CTime::GetCurrentTime();

			// ���� �������� ���
			CTime tResetTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0 );

			int iIncreaseDayCount = iDayOfWeek - tResetTime.GetDayOfWeek(); // �ش� ���ϱ��� ���� �������� ���			
			tResetTime += CTimeSpan( iIncreaseDayCount, 0, 0, 0 );			// ������ ���ϸ�ŭ ������!

			if( tResetTime < tCurTime )
			{
				// ResetTime�� ���ú��� ���Ŷ�� �������� ���Ѵ�
				tResetTime += CTimeSpan( 7, 0, 0, 0 );
			}

			// ���� �ð� ����
			m_tRefreshTime = tResetTime;

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"�ְ� ��ŷ ���� ���� ����!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_MONTH_RANKING:
		{			
			CTime tCurTime = CTime::GetCurrentTime();

			// �״��� ���°�� �������� ���
			CTime tResetTime = GetDayOfWeekInMonth( tCurTime.GetYear(), tCurTime.GetMonth(), iWeekOfMonth, iDayOfWeek, iHour );

			// ���� ���� �����ΰ�?
			if( tResetTime < tCurTime )
			{
				// �Ѵ� �������� ������..
				int iYear = tResetTime.GetYear();
				int iNextMonth = tResetTime.GetMonth() + 1;
				if( iNextMonth > 12 )
				{
					++iYear; // ������
					iNextMonth = 1; // 1��
				}

				tResetTime = GetDayOfWeekInMonth( iYear, iNextMonth, iWeekOfMonth, iDayOfWeek, iHour );
			}

			// ���� �ð� ����
			m_tRefreshTime = tResetTime;

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"���� ��ŷ ���� ���� ����!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	case SEnum::RT_HERO_RANKING:
		{			
			CTime tCurTime = CTime::GetCurrentTime();

			// �״��� ���°�� �������� ���
			CTime tResetTime = GetDayOfWeekInMonth( tCurTime.GetYear(), tCurTime.GetMonth(), iWeekOfMonth, iDayOfWeek, iHour );

			// ���� ���� �����ΰ�?
			if( tResetTime < tCurTime )
			{
				// �Ѵ� �������� ������..
				int iYear = tResetTime.GetYear();
				int iNextMonth = tResetTime.GetMonth() + 1;
				if( iNextMonth > 12 )
				{
					++iYear; // ������
					iNextMonth = 1; // 1��
				}

				tResetTime = GetDayOfWeekInMonth( iYear, iNextMonth, iWeekOfMonth, iDayOfWeek, iHour );
			}

			// ���� �ð� ����
			m_tRefreshTime = tResetTime;

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"���� ��ŷ ���� ���� ����!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;
#endif // SERV_HENIR_RENEWAL_2013

	default:
		{
            START_LOG( cerr, L"��ŷ Ÿ���� �̻��մϴ�. ���ų�¥ ���� ����!" )
				<< BUILD_LOG( m_iRankingType )
				<< END_LOG;
		}
		break;
	}
}

void KHenirRanking::NextRefreshTimeSet()
{
	// ���� ���� �ð��� ����!
	switch( m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
		{
			m_tRefreshTime += CTimeSpan( 1, 0, 0, 0 );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"�ϰ� ��ŷ ���� ���ڸ� ���� ���� ��¥�� ���� �Ϸ�!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_WEEK_RANKING:
		{
			m_tRefreshTime += CTimeSpan( 7, 0, 0, 0 );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"�ְ� ��ŷ ���� ���ڸ� ���� ���� ��¥�� ���� �Ϸ�!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_MONTH_RANKING:
		{
			int iYear = m_tRefreshTime.GetYear();
			int iNextMonth = m_tRefreshTime.GetMonth() + 1;
			if( iNextMonth > 12 )
			{
				++iYear; // ������
				iNextMonth = 1; // 1��
			}

			m_tRefreshTime = GetDayOfWeekInMonth( iYear, iNextMonth, m_iRefreshWeekOfMonth, m_iRefreshDayOfWeek, m_iRefreshHour );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"���� ��ŷ ���� ���ڸ� ���� ���� ��¥�� ���� �Ϸ�!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	case SEnum::RT_HERO_RANKING:
		{
			int iYear = m_tRefreshTime.GetYear();
			int iNextMonth = m_tRefreshTime.GetMonth() + 1;
			if( iNextMonth > 12 )
			{
				++iYear; // ������
				iNextMonth = 1; // 1��
			}

			m_tRefreshTime = GetDayOfWeekInMonth( iYear, iNextMonth, m_iRefreshWeekOfMonth, m_iRefreshDayOfWeek, m_iRefreshHour );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"���� ��ŷ ���� ���ڸ� ���� ���� ��¥�� ���� �Ϸ�!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;
#endif // SERV_HENIR_RENEWAL_2013

	default:
		{
			START_LOG( cerr, L"��ŷ Ÿ���� �̻��մϴ�. ���ų�¥ ���� ����!" )
				<< BUILD_LOG( m_iRankingType )
				<< END_LOG;
		}
		break;
	}
}

void KHenirRanking::SetRewardBeginTime( const std::wstring& wstrBeginTime )
{
	std::wstring wstrTemp = wstrBeginTime;

	if( KncUtil::ConvertStringToCTime( wstrTemp, m_tRewardBeginTime ) == false )
	{
        START_LOG( cerr, L"��ŷ ���۽ð� ��� ����!" )
			<< BUILD_LOG( m_iRankingType )
			<< BUILD_LOG( wstrBeginTime )
			<< END_LOG;
	}
	else
	{
		START_LOG( cout, L"��ŷ ���۽ð� ��� ����!" )
			<< BUILD_LOG( m_iRankingType )
			<< BUILD_LOG( wstrBeginTime );
	}
}

#ifdef SERV_HENIR_RANKING_GROUP_FIX
bool KHenirRanking::RankingSort( const UidType& iUnitUID )
{
	bool bRankOK = true;

	// �ϴ� ���� ��� ������ ����
	std::sort( m_vecRankingInfo.begin(), m_vecRankingInfo.end(), &KHenirRankingInfo::RankCompare );

	// ���ĵ� ��ŷ������ ��ũ �ű���! ( ������ ����̸� ���� ��ũ )
	int iOverLastRankCount = 0;
	{	
		KHenirRankingInfo kBeforeRank;

		for( unsigned int ui = 0; ui < m_vecRankingInfo.size(); ++ui )
		{
			KHenirRankingInfo& kRankInfo = m_vecRankingInfo[ui];

			// ���� ��ũ �˻�
			if( KHenirRankingInfo::IsSameRank( kRankInfo, kBeforeRank ) )
			{
				kRankInfo.m_iRank = kBeforeRank.m_iRank;
			}
			else
			{
				kRankInfo.m_iRank = static_cast<int>(ui + 1);
			}

			kBeforeRank = kRankInfo;

			// ������ ��ũ�� �Ѿ��ٸ� ī��Ʈ
			if( kRankInfo.m_iRank > static_cast<int>(m_uiLastRank) )
			{
				++iOverLastRankCount;
			}
		}
	}

	// ������ ��ũ���� ���� ��ũ���� �ڿ������� �ڸ���
	for( int iIndex = 0; iIndex < iOverLastRankCount; ++iIndex )
	{
		const KHenirRankingInfo& kOverLastRank = m_vecRankingInfo.back();
		if( iUnitUID == kOverLastRank.m_iUnitUID )
			bRankOK = false;
		m_vecRankingInfo.pop_back();
	}

	return bRankOK;
}
#endif SERV_HENIR_RANKING_GROUP_FIX

void KHenirRanking::RankingSort()
{
	// �ϴ� ���� ��� ������ ����
	std::sort( m_vecRankingInfo.begin(), m_vecRankingInfo.end(), &KHenirRankingInfo::RankCompare );

	// ���ĵ� ��ŷ������ ��ũ �ű���! ( ������ ����̸� ���� ��ũ )
	int iOverLastRankCount = 0;
	{	
		KHenirRankingInfo kBeforeRank;

		for( unsigned int ui = 0; ui < m_vecRankingInfo.size(); ++ui )
		{
			KHenirRankingInfo& kRankInfo = m_vecRankingInfo[ui];

			// ���� ��ũ �˻�
			if( KHenirRankingInfo::IsSameRank( kRankInfo, kBeforeRank ) )
			{
				kRankInfo.m_iRank = kBeforeRank.m_iRank;
			}
			else
			{
				kRankInfo.m_iRank = static_cast<int>(ui + 1);
			}

			kBeforeRank = kRankInfo;

			// ������ ��ũ�� �Ѿ��ٸ� ī��Ʈ
			if( kRankInfo.m_iRank > static_cast<int>(m_uiLastRank) )
			{
				++iOverLastRankCount;
			}
		}
	}

	// ������ ��ũ���� ���� ��ũ���� �ڿ������� �ڸ���
	for( int iIndex = 0; iIndex < iOverLastRankCount; ++iIndex )
	{
		m_vecRankingInfo.pop_back();
	}
}

CTime KHenirRanking::GetDayOfWeekInMonth( int iYear, int iMonth, int iWeekOfMonth, int iDayOfWeek, int iHour )
{
	// �״��� ù��°�� ���ϱ�
	CTime tFirstDayOfMonth = CTime( iYear, iMonth, 1, iHour, 0, 0 );

	// �״��� ù��° �ش� ���� ���ϱ�
	int iIncreaseDayCount = iDayOfWeek - tFirstDayOfMonth.GetDayOfWeek();
	if( iIncreaseDayCount < 0 )
	{
		iIncreaseDayCount = 7 + iIncreaseDayCount;
	}
	tFirstDayOfMonth += CTimeSpan( iIncreaseDayCount, 0, 0, 0 );

	// �״��� ���°�� ���������� CTime ���ϱ�
	CTime tDayOfMonth = tFirstDayOfMonth + CTimeSpan( ( 7 * ( iWeekOfMonth - 1 ) ), 0, 0, 0 );

	return tDayOfMonth;
}

bool KHenirRanking::CheckNewRecord( IN const KHenirRankingInfo& kNewRankingInfo )
{
	std::vector< UidType >::const_iterator vitFIND;
	std::vector< KHenirRankingInfo >::reverse_iterator vitTR;

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	// 0. �ش� ������ �������� Ȯ���Ѵ�!
	if( KncUid::ExtractServerGroupID( kNewRankingInfo.m_iUnitUID ) != GetKLoginServer()->GetServerGroupID() )
	{
		START_LOG( cerr, L"�ش� ������ ������ �ƴ϶� ��ŷ ��� ����!" )
			<< BUILD_LOG( kNewRankingInfo.m_iUnitUID )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kNewRankingInfo.m_iUnitUID ) )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
			<< END_LOG;

		return false;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// 1. ������ ��ŷ����Ʈ�� �ڽ��� ����� �ִ��� ã�ƺ���!
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = m_vecRankingInfo.begin(); vitFind != m_vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_iUnitUID != kNewRankingInfo.m_iUnitUID )
			continue;

		if( KHenirRankingInfo::RankCompare( kNewRankingInfo, *vitFind ) == false )
		{
			// ������ �ڽ��� ��ϰ� ���غ��� �� ���� ����̸� �����Ѵ�
			return false;
		}
		else
		{
			// �� ���� ����̸� ����� �������!
			*vitFind = kNewRankingInfo;

			// ��ŷ ��Ʈ��..
			goto end_proc;
		}
	}

	// 2. ���� ��ŷ��������Ʈ�� 100������ �۴ٸ�..?
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	if( m_vecRankingInfo.size() < ( m_uiLastRank + 3 ) )	// ���� 100���� ������ �� �����Ƿ� �ϴ� ��ŷ ���� ������ 103������ �����صд�.
#else
	if( m_vecRankingInfo.size() < m_uiLastRank )
#endif SERV_HENIR_RANKING_GROUP_FIX
	{
        m_vecRankingInfo.push_back( kNewRankingInfo );

		// ��ŷ ��Ʈ��..
		goto end_proc;
	}

	// 3. ������ ��ũ�� ���Ͽ� 100���ȿ� ����� ã�ƺ���	
	vitTR = m_vecRankingInfo.rbegin();
	if( vitTR == m_vecRankingInfo.rend() )
	{
		START_LOG( cerr, L"��ŷ ����Ʈ�� �̻���" )
			<< BUILD_LOG( kNewRankingInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	const KHenirRankingInfo& kOldRankingInfo = *vitTR;
	if( KHenirRankingInfo::RankCompare( kNewRankingInfo, kOldRankingInfo ) == false )
	{
		// 100���ȿ� ���� ���ߴ�!
		return false;
	}

	// 4. 100���ȿ� ����� Ȯ�εǾ����� ��ŷ����Ʈ�� �ְ� ��Ʈ����
	m_vecRankingInfo.push_back( kNewRankingInfo );

end_proc:
	// 5. ��ŷ ��Ʈ
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	if( RankingSort(kNewRankingInfo.m_iUnitUID) == false )
		return false;
#else
	RankingSort();
#endif SERV_HENIR_RANKING_GROUP_FIX

	// 6. ��ŷ ���� refresh ����
	SetRankingChanged( true );
	return true;
}

bool KHenirRanking::DeleteRecord( IN const std::wstring& wstrNickName )
{
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = m_vecRankingInfo.begin(); vitFind != m_vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_wstrNickName == wstrNickName )
			break;
	}

	if( vitFind == m_vecRankingInfo.end() )
	{
		START_LOG( cout, L"��ŷ���� ���� ������ ã�� ���ߴ�!" )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( m_iRankingType )
			<< BUILD_LOG( m_vecRankingInfo.size() )
			<< END_LOG;

		return false;
	}

	START_LOG( cout2, L"----- ��ŷ ���� ���� -----" )
		<< BUILD_LOG( vitFind->m_iRank )
		<< BUILD_LOG( vitFind->m_iStageCount )
		<< BUILD_LOG( vitFind->m_ulPlayTime )
		<< BUILD_LOG( vitFind->m_tRegDate )
		<< BUILD_LOG( vitFind->m_iUnitUID )
		<< BUILD_LOG( vitFind->m_wstrNickName )
		<< BUILD_LOGc( vitFind->m_cUnitClass )
		<< BUILD_LOGc( vitFind->m_ucLevel );

	// ã�Ҵ�! ������!
	m_vecRankingInfo.erase( vitFind );

	// ��ŷ ��Ʈ
	RankingSort();

	// ��ŷ ���� refresh ����
	SetRankingChanged( true );
	return true;
}

bool KHenirRanking::DeleteRecord( IN const UidType iUnitUID )
{
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = m_vecRankingInfo.begin(); vitFind != m_vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_iUnitUID == iUnitUID )
			break;
	}

	if( vitFind == m_vecRankingInfo.end() )
		return false;

	// ã�Ҵ�! ������!
	m_vecRankingInfo.erase( vitFind );

	// ��ŷ ��Ʈ
	RankingSort();

	// ��ŷ ���� refresh ����
	SetRankingChanged( true );
	return true;
}

void KHenirRanking::SetRankingInfo( const std::vector< KHenirRankingInfo >& vecRankingInfo )
{
	m_vecRankingInfo.clear();
    m_vecRankingInfo = vecRankingInfo;

	// ��ŷ ��Ʈ
	RankingSort();
}

void KHenirRanking::ClearRanking()
{
	m_vecRankingInfo.clear();
	SetRankingChanged( true ); // ���Ӽ����� ��ε� ĳ���� �ǵ��� flag�� ����
}


