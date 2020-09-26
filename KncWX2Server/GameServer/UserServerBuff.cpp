#include "BaseServer.h"
#include ".\UserServerBuff.h"
#include "X2Data/XSLBuffManager.h"
#include "GSWorldMissionManager.h"


//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM


KUserServerBuff::KUserServerBuff( IN const KBuffInfo& kBuffInfo ) : 
m_iBuffID( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID ),
m_iFactorID( kBuffInfo.m_kFactorInfo.m_iFactorID ),
m_fBuffPeriodTime( 0.0 )
{
}

KUserServerBuff::~KUserServerBuff(void)
{
}

void KUserServerBuff::InitPeriod( IN const CTime& tStartTime )
{
	const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( GetBuffID() );
	if( pBuffTemplet == NULL )
	{
		START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( GetBuffID() )
			<< END_LOG;
		return;
	}

	// �Ⱓ�� �������� �˻�
	if( pBuffTemplet->m_fBuffPeriodSecond == 0.0f )
		return;

	const CTime tCurrentTime = CTime::GetCurrentTime();

	// �� ������ �Ⱓ �����ΰ��� �����ϱ� ���� �Ⱓ������ �����Ѵ�.
	m_fBuffPeriodTime = pBuffTemplet->m_fBuffPeriodSecond;

	// ������ ������ �ð��� �����Ѵ�!
	if( tStartTime != 0 )
	{
		// ������ �ð� ������ ���� ������ �ٷ� ����!
		m_tStartTime = tStartTime;
	}
	else
	{
		// ������ ���� �ð��� ������ �ð����� ����!
		m_tStartTime = tCurrentTime;
	}
	
	// �ܺο��� �Է��� ���� �ð��� �ִٸ� �װ��� ����ϰ�, �׷��� �ʴٸ� ���� ���ش�.
	switch( GetBuffID() )
	{
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_1:
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_2:
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_3:
	case CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_4:
		{
			CTime tDefenseDungeonBuffStartTime;

			// ���� ��� ���� �ð��� ���� ������, �װ� useTime�� ���غ���.
			if( SiKGSWorldMissionManager()->GetDefenseDungeonEndTime( tDefenseDungeonBuffStartTime, m_tEndTime ) == false )
			{
				START_LOG( cerr, L"���潺 ������ �˸��� ���� ������ �����ϴ�.")
					<< END_LOG;
				return;
			}

			if( m_tStartTime < tDefenseDungeonBuffStartTime )
			{
				return;
			}
			else if( ( m_tEndTime + CTimeSpan( static_cast<int>( pBuffTemplet->m_fBuffPeriodSecond ) ) ) <= m_tStartTime )
			{
				return;
			}

			m_tEndTime += CTimeSpan( static_cast<int>( pBuffTemplet->m_fBuffPeriodSecond ) );
		}
		break;

	default:
		{
			// �⺻�����δ� ���� �ð����κ��� ���ӽð��� ���� ���� ���� �ð��� �ȴ�.
			m_tEndTime = m_tStartTime + CTimeSpan( ( __int64 )pBuffTemplet->m_fBuffPeriodSecond );
		}
		break;
	}

	// �ð� ������ ���ڿ��ε� ����!
	m_wstrStartTime = m_tStartTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	m_wstrEndTime = m_tEndTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
}

void KUserServerBuff::GetBuffInfo( OUT KBuffInfo& kBuffInfo ) const
{
	kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID	= GetBuffID();
	kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = m_iFactorID;
	kBuffInfo.m_iBuffStartTime	= m_tStartTime.GetTime();
	kBuffInfo.m_iBuffEndTime	= m_tEndTime.GetTime();
}

bool KUserServerBuff::IsExpired() const
{
	CTime tCurTime = CTime::GetCurrentTime();
	return( tCurTime > m_tEndTime );
}


#endif SERV_SERVER_BUFF_SYSTEM
//}}


