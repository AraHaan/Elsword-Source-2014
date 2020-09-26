#include ".\userpvpmanager.h"
//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch\PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


KUserPvpManager::KUserPvpManager(void)
{
	Clear();
}

KUserPvpManager::~KUserPvpManager(void)
{
}

void KUserPvpManager::Clear()
{
	m_iMatchUID = 0;
	m_iOfficialMatchCnt.SetValue( 0 );
	m_iRating.SetValue( 0 );
	m_iMaxRating.SetValue( 0 );
	m_iRPoint.SetValue( 0 );
	m_iAPoint.SetValue( 0 );

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	m_fKFactor.SetValue( 0.0 );
	m_bIsRedistributionUser	= false;
	m_iPastSeasonWin		= 0;
#endif SERV_2012_PVP_SEASON2
	//}}

	m_bIsWinBeforeMatch = false;
	m_iWin.SetValue( 0 );
	m_iLose.SetValue( 0 );
	m_ePvpRank = CXSLUnit::PVPRANK_NONE;
	m_bRequestMatchMaking = false;
}

void KUserPvpManager::Init( IN const int iOfficialMatchCnt, 
							IN const int iRating, 
							IN const int iMaxRating, 
							IN const int iRPoint, 
							IN const int iAPoint,
							IN const bool bIsWinBeforeMatch,
							IN const int iWin, 
							IN const int iLose
							//{{ 2012. 06. 20	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
							, IN const char cRank
							//{{ 2012. 06. 25	�ڼ���	2012 ���� ����2
							, IN const float fKFactor
							, IN const bool bIsRedistributionUser
							, IN const int iPastSeasonWin
							, IN const UidType iUnitUID
#endif SERV_2012_PVP_SEASON2
							//}}
							)
{
	m_iOfficialMatchCnt.SetValue( iOfficialMatchCnt );
	m_iRating.SetValue( iRating );
	m_iMaxRating.SetValue( iMaxRating );
	m_iRPoint.SetValue( iRPoint );
	m_iAPoint.SetValue( iAPoint );
	m_bIsWinBeforeMatch = bIsWinBeforeMatch;
	m_iWin.SetValue( iWin );
	m_iLose.SetValue( iLose );

	// ��ũ ���ϱ�
	//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	m_ePvpRank				= static_cast<CXSLUnit::PVP_RANK>( cRank );
	m_fKFactor.SetValue( fKFactor );
	m_bIsRedistributionUser	= bIsRedistributionUser;
	m_iPastSeasonWin		= iPastSeasonWin;

	AdjustInitValue( iUnitUID );
#else
	const CXSLUnit::PVP_EMBLEM eEmblem = CXSLUnit::GetPVPEmblem( GetRating() );
	m_ePvpRank = CXSLUnit::PvpEmblemToPvpRank( eEmblem );
#endif SERV_2012_PVP_SEASON2
	//}}
}

void KUserPvpManager::GetDBUpdateInfo( OUT int& iOfficialMatchCnt,
										OUT int& iRating, 
										OUT int& iMaxRating, 
										OUT int& iRPoint, 
										OUT int& iAPoint, 
										OUT bool& bIsWinBeforeMatch, 
										OUT int& iWin, 
										OUT int& iLose,
										OUT char& cEmblemEnum

										//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
										, OUT float& fKFactor
#endif SERV_2012_PVP_SEASON2
										//}}
										)
{
	iOfficialMatchCnt = m_iOfficialMatchCnt.GetChangeValue();
	m_iOfficialMatchCnt.SetValue( m_iOfficialMatchCnt );

	iRating		= m_iRating.GetChangeValue();
	m_iRating.SetValue( m_iRating );

	iMaxRating	= m_iMaxRating.GetChangeValue();
	m_iMaxRating.SetValue( m_iMaxRating );

	iRPoint		= m_iRPoint.GetChangeValue();
	m_iRPoint.SetValue( m_iRPoint );

	iAPoint		= m_iAPoint.GetChangeValue();
	m_iAPoint.SetValue( m_iAPoint );

    bIsWinBeforeMatch = m_bIsWinBeforeMatch;

	iWin		= m_iWin.GetChangeValue();
	m_iWin.SetValue( m_iWin );

	iLose		= m_iLose.GetChangeValue();
	m_iLose.SetValue( m_iLose );

	cEmblemEnum = static_cast<CXSLUnit::PVP_RANK>(GetPvpRank());

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	fKFactor	= m_fKFactor.GetChangeValue();
	m_fKFactor.SetValue( m_fKFactor );
#endif SERV_2012_PVP_SEASON2
	//}}
}

void KUserPvpManager::CheckRollbackInfo( IN const int iOfficialMatchCnt,
										IN const int iRating,
										IN const int iMaxRating,
										IN const int iRPoint,
										IN const int iAPoint,
										IN const int iWin, 
										IN const int iLose

										//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
										, IN const float& fKFactor
#endif SERV_2012_PVP_SEASON2
										//}}
										)
{
	m_iOfficialMatchCnt.AdjustInitValue( m_iOfficialMatchCnt.GetInitValue() - iOfficialMatchCnt );
	m_iRating.AdjustInitValue( m_iRating.GetInitValue() - iRating );
	m_iMaxRating.AdjustInitValue( m_iMaxRating.GetInitValue() - iMaxRating );
	m_iRPoint.AdjustInitValue( m_iRPoint.GetInitValue() - iRPoint );
	m_iAPoint.AdjustInitValue( m_iAPoint.GetInitValue() - iAPoint );
	m_iWin.AdjustInitValue( m_iWin.GetInitValue() - iWin );
	m_iLose.AdjustInitValue( m_iLose.GetInitValue() - iLose );

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	m_fKFactor.AdjustInitValue( m_fKFactor.GetInitValue() - fKFactor );
#endif SERV_2012_PVP_SEASON2
	//}}
}

void KUserPvpManager::AdjustInitValue( IN const UidType iUnitUID )
{
	if( ( m_ePvpRank < CXSLUnit::PVPRANK_RANK_E ) || ( CXSLUnit::PVPRANK_RANK_SSS < m_ePvpRank ) )
	{
		START_LOG( cerr, L"���� �� ���� ��ũ ������ ���Խ��ϴ�!! DB ������ ���� �� �ڵ� ���� üũ�� �ʿ��մϴ�!!")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_ePvpRank )
			<< END_LOG;
	}

	if( m_iAPoint < KPMV_AP_MIN )
	{
		START_LOG( cerr, L"���� �� ���� AP ������ ���Խ��ϴ�!! DB ������ ���� �� �ڵ� ���� üũ�� �ʿ��մϴ�!!")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_iAPoint )
			<< END_LOG;
	}

	if( m_fKFactor < KPMV_KFACTOR_MIN )
	{
		START_LOG( cerr, L"KFactor�� �ּ� ��ġ���� �۽��ϴ�. �ּ� ��ġ�� �����մϴ�.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_fKFactor )
			<< BUILD_LOG( KPMV_KFACTOR_MIN )
			<< END_LOG;

		m_fKFactor.SetValue( KPMV_KFACTOR_MIN );
	}
	else if( KPMV_KFACTOR_MAX < m_fKFactor )
	{
		START_LOG( cerr, L"KFactor�� �ִ� ��ġ���� Ů�ϴ�. �ִ� ��ġ�� �����մϴ�.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_fKFactor )
			<< BUILD_LOG( KPMV_KFACTOR_MAX )
			<< END_LOG;

		m_fKFactor.SetValue( KPMV_KFACTOR_MAX );
	}

	if( m_iRating < KPMV_RATING_MIN )
	{
		START_LOG( cerr, L"Rating�� �ּ� ��ġ���� �۽��ϴ�. �ּ� ��ġ�� �����մϴ�.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_iRating )
			<< BUILD_LOG( KPMV_RATING_MIN )
			<< END_LOG;

		m_iRating.SetValue( KPMV_RATING_MIN );
	}
	else if( KPMV_RATING_MAX < m_iRating )
	{
		START_LOG( cerr, L"Rating�� �ִ� ��ġ���� Ů�ϴ�. �ִ� ��ġ�� �����մϴ�.")
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( m_iRating )
			<< BUILD_LOG( KPMV_RATING_MAX )
			<< END_LOG;

		m_iRating.SetValue( KPMV_RATING_MAX );
	}
}

//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2	// Rating ����Ʈ ���Ѽ� 3300
bool KUserPvpManager::AddRatingAndRankUpDownProcess( IN const int iRating )
#else
void KUserPvpManager::AddRating( IN const int iRating )
#endif SERV_2012_PVP_SEASON2
//}}
{
	int iAddRating = iRating;

	// �ִ� Rating���� ���� �ʴ��� �˻�
	if( ( GetRating() + iAddRating ) > CXSLUnit::PE_END )
	{
        iAddRating = ( CXSLUnit::PE_END - GetRating() );
	}

	// �ּ� Rating���� ���� �ʴ��� �˻�
	if( ( GetRating() + iAddRating ) < 0 )
	{
		iAddRating = -( GetRating() );
	}

	// Rating�� ����!
	m_iRating += iAddRating;

	// �ִ� Rating�� üũ!
	if( GetMaxRating() < GetRating() )
	{
		m_iMaxRating += ( GetRating() - GetMaxRating() );
	}

	// ��ũ ����
	if( iAddRating != 0 )
	{
		//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2	// ��ũ ��/�ٿ� �� ��ŷ ����Ʈ ����
		const CXSLUnit::PVP_RANK eNowPvpRank = CXSLUnit::ComputeNewPVPRank( GetPvpRank(), GetRating() );
		if( eNowPvpRank == CXSLUnit::PE_NONE )
		{
			return false;
		}

		if( m_ePvpRank < eNowPvpRank )
		{
			// ��ũ ��
			//m_iRPoint = 0;
			m_iRPoint -= m_iRPoint;
		}
		else if( eNowPvpRank < m_ePvpRank )
		{
			// ��ũ �ٿ�
			switch( eNowPvpRank )
			{
			case CXSLUnit::PVPRANK_RANK_E:	m_iRPoint -= m_iRPoint;	m_iRPoint += 67;	break;
			case CXSLUnit::PVPRANK_RANK_D:	m_iRPoint -= m_iRPoint;	m_iRPoint += 111;	break;
			case CXSLUnit::PVPRANK_RANK_C:	m_iRPoint -= m_iRPoint;	m_iRPoint += 156;	break;
			case CXSLUnit::PVPRANK_RANK_B:	m_iRPoint -= m_iRPoint;	m_iRPoint += 156;	break;
			case CXSLUnit::PVPRANK_RANK_A:	m_iRPoint -= m_iRPoint;	m_iRPoint += 200;	break;
			case CXSLUnit::PVPRANK_RANK_S:	m_iRPoint -= m_iRPoint;	m_iRPoint += 200;	break;
			case CXSLUnit::PVPRANK_RANK_SS:	m_iRPoint -= m_iRPoint;	m_iRPoint += 289;	break;
			}
		}

		if( m_ePvpRank != eNowPvpRank )
		{
			m_ePvpRank = eNowPvpRank;
			return true;
		}
	}
	return false;
#else
		const CXSLUnit::PVP_EMBLEM eEmblem = CXSLUnit::GetPVPEmblem( GetRating() );
		m_ePvpRank = CXSLUnit::PvpEmblemToPvpRank( eEmblem );
	}
#endif SERV_2012_PVP_SEASON2
		//}}
}

void KUserPvpManager::AddRPoint( IN const int iRPoint )
{
	int iAddRPoint = iRPoint;

	// �ִ� RankingPoint���� ���� �ʴ��� �˻�
	if( ( GetRPoint() + iAddRPoint ) > SiKPvpMatchManager()->GetMaxRankingPoint() )
	{
		iAddRPoint = ( SiKPvpMatchManager()->GetMaxRankingPoint() - GetRPoint() );
	}

	// �ּ� RankingPoint���� ���� �ʴ��� �˻�
	if( ( GetRPoint() + iAddRPoint ) < 0 )
	{
		iAddRPoint = -( GetRPoint() );
	}

	// Ranking Point�� ����!
	m_iRPoint += iAddRPoint;
}

void KUserPvpManager::AddAPoint( IN const int iAPoint )
{
	int iAddAPoint = iAPoint;

	// �ִ� ArenaPoint���� ���� �ʴ��� �˻�
	if( ( GetAPoint() + iAddAPoint ) > SiKPvpMatchManager()->GetMaxArenaPoint() )
	{
		iAddAPoint = ( SiKPvpMatchManager()->GetMaxArenaPoint() - GetAPoint() );
	}

	// �ּ� ArenaPoint���� ���� �ʴ��� �˻�
	if( ( GetAPoint() + iAddAPoint ) < 0 )
	{
		iAddAPoint = -( GetAPoint() );
	}

	// Rating�� ����!
	m_iAPoint += iAddAPoint;
}

bool KUserPvpManager::UpdateWinLose( IN const int iWin, IN const int iLose, IN const bool bIsOfficialMatch )
{
	if( iWin > 1  &&  iLose > 1 )
	{
		START_LOG( cerr, L"�¸���� ���� ���ÿ� 2���� ���õǾ� �Դ�.!" )
			<< BUILD_LOG( iWin )
			<< BUILD_LOG( iLose )
			<< END_LOG;
		return false;
	}
	if( iWin > 1 )
	{
		START_LOG( cerr, L"WIN ����Ʈ �̻�.!" )
			<< BUILD_LOG( iWin )
			<< END_LOG;
		//iWin = 1;
		return false;
	}
	if( iLose > 1 )
	{
		START_LOG( cerr, L"LOSE ����Ʈ �̻�.!" )
			<< BUILD_LOG( iLose )
			<< END_LOG;

		//iLose = 1;
		return false;
	}

	m_iWin	+= iWin;
	m_iLose	+= iLose;

	// ���� ������ ���
	if( bIsOfficialMatch )
	{
        // �¸��ߴٸ�!
		if( iWin == 1 )
		{
			m_bIsWinBeforeMatch = true;
		}
		else if( iLose == 1 )
		{
			m_bIsWinBeforeMatch = false;
		}
	}
	return true;
}

//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
void KUserPvpManager::AddKFactor( IN const float& fKFactor )
{
	float fAddKFactor = fKFactor;

	// �ּ� K-Factor���� ���� �ʴ��� �˻�
	if( ( GetKFactor() + fAddKFactor ) < 0.f )
	{
		fAddKFactor = -( GetKFactor() );

		START_LOG( cerr, L"���� KFactor�� ���а��� �������� 0���� �۴�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetKFactor() )
			<< BUILD_LOG( fAddKFactor )
			<< END_LOG;
	}

	if( ( GetKFactor() + fAddKFactor ) > 500.f )
	{
		fAddKFactor = ( 500.f - GetKFactor() );

		START_LOG( cerr, L"���� KFactor�� ���а��� �������� 500���� ũ��! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetKFactor() )
			<< BUILD_LOG( fAddKFactor )
			<< END_LOG;
	}

	// K-Factor�� ����!
	m_fKFactor += fAddKFactor;
}

void KUserPvpManager::UpdatePvpResult( IN const KERM_UPDATE_PVP_UNIT_INFO_NOT kData )
{
	/* Rating������Ʈ
		- AddRating�� ��� ��ũ ��/�ٿ��� �߻��� ���,
		������ RankPoint���� �������ش�. ( AddRatingAndRankUpDownProcess ���ο���. )
		�׷��Ƿ� ���� AddRPoint�� ����Ǿ�� �ȵȴ�.
	*/
	if( AddRatingAndRankUpDownProcess( kData.m_iRating ) == false )
	{
		// RPoint������Ʈ
		AddRPoint( kData.m_iRPoint );
	}

	// APoint������Ʈ
	AddAPoint( kData.m_iAPoint );

	// K-Factor ������Ʈ
	AddKFactor( kData.m_fKFactorIncrement );

	// ���� ������Ʈ
	if( UpdateWinLose( kData.m_iWin, kData.m_iLose, kData.m_bIsOfficialMatch ) == false )
	{
		START_LOG( cerr, L"���� ���� ������Ʈ ����! ���� ���� �̻��մϴ�!" )
			<< BUILD_LOG( kData.m_iWin )
			<< BUILD_LOG( kData.m_iLose )
			<< END_LOG;
	}

	// ���� ���� �÷��� ī��Ʈ
	if( kData.m_bIsOfficialMatch == true  &&  kData.m_bOutRoom == false )
	{
		// ���� ���� �÷��� ī��Ʈ ����!
		IncreaseOfficialMatchCount();
	}

	//////////////////////////////////////////////////////////////////////////
	if( kData.m_iWin > 0  &&  kData.m_iAPoint < 0 )
	{
        START_LOG( cout, L"[�������Ȯ��] �̰�µ� Arena Point���� ���̳ʽ��� ���´�! ������ �ִ�!" )
			<< BUILD_LOG( kData.m_iWin )
			<< BUILD_LOG( kData.m_iAPoint );
	}
	//////////////////////////////////////////////////////////////////////////	
}
#else
void KUserPvpManager::UpdatePvpResult( IN const int iRating, IN const int iRPoint, IN const int iAPoint, IN const int iWin, IN const int iLose, IN const bool bIsOfficialMatch, IN const bool bOutRoom )
{
	// Rating������Ʈ
	AddRating( iRating );

	// RPoint������Ʈ
	AddRPoint( iRPoint );

	// APoint������Ʈ
	AddAPoint( iAPoint );

	// ���� ������Ʈ
	if( UpdateWinLose( iWin, iLose, bIsOfficialMatch ) == false )
	{
		START_LOG( cerr, L"���� ���� ������Ʈ ����! ���� ���� �̻��մϴ�!" )
			<< BUILD_LOG( iWin )
			<< BUILD_LOG( iLose )
			<< END_LOG;
	}

	// ���� ���� �÷��� ī��Ʈ
	if( bIsOfficialMatch == true  &&  bOutRoom == false )
	{
		// ���� ���� �÷��� ī��Ʈ ����!
		IncreaseOfficialMatchCount();
	}

	//////////////////////////////////////////////////////////////////////////
	if( iWin > 0  &&  iAPoint < 0 )
	{
        START_LOG( cout, L"[�������Ȯ��] �̰�µ� Arena Point���� ���̳ʽ��� ���´�! ������ �ִ�!" )
			<< BUILD_LOG( iWin )
			<< BUILD_LOG( iAPoint );
	}
	//////////////////////////////////////////////////////////////////////////	
}
#endif SERV_2012_PVP_SEASON2
//}}

#endif SERV_PVP_NEW_SYSTEM
//}}


