#include "Lottery.h"
#include "dbg/dbg.hpp"

//#ifdef SERV_DOUBLE_LOTTERY
KLottery::KLottery() : m_dTotalProb( 0.0f )
{
}

KLottery::~KLottery() 
{
}

KLottery& KLottery::operator=( const KLottery& t )
{
    m_mapCase       = t.m_mapCase;
//#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
	m_multimapCase  = t.m_multimapCase;
//#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
    m_dTotalProb    = t.m_dTotalProb;
    return *this;
}

void KLottery::Clear()
{
	m_mapCase.clear();
//#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
	m_multimapCase.clear();
//#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
	m_dTotalProb = 0.f;
}

//{{ 2013. 04. 15	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
int	KLottery::GetFirstCase() const
{
	std::map< int, KCaseUnit >::const_iterator mit;
	mit = m_mapCase.begin();
	if( mit == m_mapCase.end() )
	{
		return CASE_BLANK;
	}

	return mit->first;
}
//#endif SERV_NEW_DEFENCE_DUNGEON
//}}

bool KLottery::AddCase( int nCaseID, double dProbability, int nParam1 /*= PARAM_BLANK*/, int nParam2 /*= PARAM_BLANK*/ )
{
    if( m_dTotalProb + dProbability > 100.000001 )
    {
        START_LOG( cerr, L"100�ۼ�Ʈ�� �Ѵ� Ȯ�� ������ �õ�." )
            << BUILD_LOG( nCaseID )
            << BUILD_LOG( dProbability )
            << BUILD_LOG( m_dTotalProb )
            << BUILD_LOG( m_mapCase.size() )
			<< END_LOG;

        return false;
    }

    std::map< int, KCaseUnit >::iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit != m_mapCase.end() )    // ������ ������ case�� Ȯ���� �����ϰ� �ִ� ���
    {
        mit->second.m_dProb     += dProbability;
        mit->second.m_nParam1    = nParam1;
		mit->second.m_nParam2    = nParam2;

        m_dTotalProb += dProbability;
    }
    else
    {
        KCaseUnit kUnit = { dProbability, nParam1, nParam2 };
        m_mapCase.insert( std::make_pair( nCaseID, kUnit ) );
        m_dTotalProb += dProbability;
    }

    return true;
}

bool KLottery::AddCaseIntegerCast( int nCaseID, double dProbability, int nParam1 /*= PARAM_BLANK*/, int nParam2 /*= PARAM_BLANK*/ )
{
	if( static_cast<int>(m_dTotalProb + dProbability) > 100 )
	{
		START_LOG( cerr, L"100.1�ۼ�Ʈ�� �Ѵ� Ȯ�� ������ �õ�." )
			<< BUILD_LOG( nCaseID )
			<< BUILD_LOG( dProbability )
			<< BUILD_LOG( m_dTotalProb )
			<< BUILD_LOG( m_mapCase.size() )
			<< END_LOG;

		return false;
	}

	std::map< int, KCaseUnit >::iterator mit;
	mit = m_mapCase.find( nCaseID );

	if( mit != m_mapCase.end() )    // ������ ������ case�� Ȯ���� �����ϰ� �ִ� ���
	{
		mit->second.m_dProb     += dProbability;
		mit->second.m_nParam1    = nParam1;
		mit->second.m_nParam2    = nParam2;

		m_dTotalProb += dProbability;
	}
	else
	{
		KCaseUnit kUnit = { dProbability, nParam1, nParam2 };
		m_mapCase.insert( std::make_pair( nCaseID, kUnit ) );
		m_dTotalProb += dProbability;
	}

	return true;
}

#include <boost/random.hpp>
static boost::minstd_rand generator(42u);

int KLottery::Decision( double& dCheckRoulette ) const
{
	static boost::uniform_real<> uni_dist(0,100);
	static boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni(generator, uni_dist);

    double dRoulette = (double)uni();

	//{{ 2008. 5. 23  ������  ����� üũ
	dCheckRoulette = dRoulette;
	//}}

    double dAccumulate = 0.0f;
	std::map<int,KCaseUnit>::const_iterator mit;

	for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
	{
        dAccumulate += mit->second.m_dProb;
        if( dRoulette <= dAccumulate ) return mit->first;
	}

	return CASE_BLANK;
}

//{{ 2008. 5. 23  ������  ����� üũ - ���� �������̽�
int KLottery::Decision() const
{
    double dCheckRoulette;
	return Decision( dCheckRoulette );
}
//}}

void KLottery::Seed( const int iSeed )
{
	generator.seed( iSeed );
}

#include <boost/format.hpp>

ImplToStringW( KLottery )
{
    double dAccumulate = 0;
    std::map<int,KCaseUnit>::const_iterator mit;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); mit++ )
    {
        stm_ << boost::wformat( L"%2.2f ~ %2.2f (%2.2f%%) : %d (param1:%d, param2:%d) \n" )
            % dAccumulate
            % (double)(dAccumulate + mit->second.m_dProb)
            % (double)(mit->second.m_dProb)
            % mit->first
            % mit->second.m_nParam1
			% mit->second.m_nParam2;

        dAccumulate += mit->second.m_dProb;
    }

    stm_ << boost::wformat( L"%2.2f ~ 100 (%2.2f%%) : -1 \n" )
        % dAccumulate
        % (100.F - dAccumulate);

    return stm_ << std::endl;
}

void KLottery::GetCaseList( std::vector< int >& vecCaseList ) const
{
	vecCaseList.clear();

	std::map< int, KCaseUnit >::const_iterator mit;
	for( mit = m_mapCase.begin(); mit != m_mapCase.end(); ++mit )
	{
        vecCaseList.push_back( mit->first );
	}
}

int KLottery::GetParam1( int nCaseID ) const
{
    std::map< int, KCaseUnit >::const_iterator mit;
    mit = m_mapCase.find( nCaseID );

    if( mit == m_mapCase.end() )
        return PARAM_BLANK;

    return mit->second.m_nParam1;
}

int KLottery::GetParam2( int nCaseID ) const
{
	std::map< int, KCaseUnit >::const_iterator mit;
	mit = m_mapCase.find( nCaseID );

	if( mit == m_mapCase.end() )
		return PARAM_BLANK;

	return mit->second.m_nParam2;
}

bool KLottery::AddProbability( double dProbAdd )
{
    _JIF( dProbAdd > 0.0f, return false );

    if( dProbAdd * m_mapCase.size() + m_dTotalProb > 100.0f )
    {
        START_LOG( cerr, L"Probability overflow." )
            << BUILD_LOG( dProbAdd )
            << BUILD_LOG( m_mapCase.size() )
            << BUILD_LOG( m_dTotalProb )
            << BUILD_LOG( dProbAdd * m_mapCase.size() + m_dTotalProb )
			<< END_LOG;

        return false;
    }

    m_dTotalProb = 0.0f;    // ��ü Ȯ���յ� ������ ���� �������.
    std::map<int,KCaseUnit>::iterator mit;
    for( mit = m_mapCase.begin(); mit != m_mapCase.end(); ++mit )
    {
        mit->second.m_dProb += dProbAdd;

        m_dTotalProb += mit->second.m_dProb;
    }

    LIF( m_dTotalProb <= 100.0f );

    return true;
}

//{{ 2013. 04. 15	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
bool KLottery::DelProbability( int iCase )
{
	std::map< int, KCaseUnit >::iterator mit;
	mit = m_mapCase.find( iCase );
	if( mit == m_mapCase.end() )
	{
		return false;
	}

	m_dTotalProb -= mit->second.m_dProb;
	m_mapCase.erase( mit );
	return true;
}
//#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KLottery::MakeHundredProbabillty()
{
	_JIF( m_dTotalProb < 100.0f, return false );
	_JIF( GetCaseNum() > 0, return false );

	const double dRemainProb = 100.0f - m_dTotalProb;
	const double dRemainProbByCase = dRemainProb / (double)GetCaseNum();

	return AddProbability( dRemainProb );
}
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ �ʵ� ��� ���� - ��μ�
//#ifdef SERV_REFORM_ITEM_DROP
bool KLottery::AddMultiProbRate( double dRate )
{
	_JIF( dRate > 0.0f, return false );

	double dTempTotalProb = m_dTotalProb;

	// ������ ��ü Ȯ���� 100�� ������ fRate�� ���� ����
	if( m_dTotalProb * dRate > 100.0f )
	{
		dRate = 100.f / m_dTotalProb;
	}
	
	// ���� ��������
	m_dTotalProb = 0.0f;
	std::map<int,KCaseUnit>::iterator mit;
	for( mit = m_mapCase.begin(); mit != m_mapCase.end(); ++mit )
	{
		mit->second.m_dProb *= dRate;

		m_dTotalProb += mit->second.m_dProb;
	}

// 	START_LOG( cerr, L"[�׽�Ʈ]��ȭ�� Ȯ�� Ȯ��" )
// 		<< BUILD_LOG( m_fTotalProb )
// 		<< BUILD_LOG( fTempTotalProb )
// 		<< BUILD_LOG( fRate )
// 		<< END_LOG;

	//LIF( m_dTotalProb <= 100.0f );

	return true;
}
//#endif SERV_REFORM_ITEM_DROP
//}}

//#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
bool KLottery::AddDuplicateCaseIntegerCast( int nCaseID, double dProbability, int nParam1 /*= PARAM_BLANK*/, int nParam2 /*= PARAM_BLANK*/ )
{
	if( static_cast<int>(m_dTotalProb + dProbability) > 100 )
	{
		START_LOG( cerr, L"100.1�ۼ�Ʈ�� �Ѵ� Ȯ�� ������ �õ�." )
			<< BUILD_LOG( nCaseID )
			<< BUILD_LOG( dProbability )
			<< BUILD_LOG( m_dTotalProb )
			<< BUILD_LOG( m_multimapCase.size() )
			<< END_LOG;

		return false;
	}

	KCaseUnit kUnit = { dProbability, nParam1, nParam2 };
	m_multimapCase.insert( std::make_pair( nCaseID, kUnit ) );
	m_dTotalProb += dProbability;

	return true;
}

int KLottery::GetDuplicateParam1( int nCaseID, int iPeriod, int iQuantity  ) const
{
	std::multimap< int, KCaseUnit >::const_iterator mmit;

	for( mmit = m_multimapCase.begin(); mmit != m_multimapCase.end(); ++mmit )
	{		
		if( mmit->first == nCaseID && mmit->second.m_nParam1 == iPeriod && mmit->second.m_nParam2 == iQuantity )
		{
			return mmit->second.m_nParam1;
		}
	}

	return PARAM_BLANK;
}

int KLottery::GetDuplicateParam2( int nCaseID, int iPeriod, int iQuantity  ) const
{
	std::multimap< int, KCaseUnit >::const_iterator mmit;

	for( mmit = m_multimapCase.begin(); mmit != m_multimapCase.end(); ++mmit )
	{		
		if( mmit->first == nCaseID && mmit->second.m_nParam1 == iPeriod && mmit->second.m_nParam2 == iQuantity )
		{
			return mmit->second.m_nParam2;
		}
	}

	return PARAM_BLANK;
}

KLottery::KDuplicateCaseResult KLottery::DuplicateDecision( double& dCheckRoulette ) const
{
	static boost::uniform_real<> uni_dist(0,100);
	static boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni(generator, uni_dist);

	double dRoulette = (double)uni();

	//{{ 2008. 5. 23  ������  ����� üũ
	dCheckRoulette = dRoulette;
	//}}

	double dAccumulate = 0.0f;
	std::multimap<int,KCaseUnit>::const_iterator mit;
	KDuplicateCaseResult kDuplicateCaseResult;
	kDuplicateCaseResult.m_iItemID = -1;
	kDuplicateCaseResult.m_nParam1 = -1;
	kDuplicateCaseResult.m_nParam2 = -1;

	for( mit = m_multimapCase.begin(); mit != m_multimapCase.end(); mit++ )
	{
		dAccumulate += mit->second.m_dProb;
		if( dRoulette <= dAccumulate )
		{
			kDuplicateCaseResult.m_iItemID = mit->first;
			kDuplicateCaseResult.m_nParam1 = mit->second.m_nParam1;
			kDuplicateCaseResult.m_nParam2 = mit->second.m_nParam2;
			return kDuplicateCaseResult;
		}
	}
	return kDuplicateCaseResult;
}

//{{ 2008. 5. 23  ������  ����� üũ - ���� �������̽�
KLottery::KDuplicateCaseResult KLottery::DuplicateDecision() const
{
	double dCheckRoulette;
	return DuplicateDecision( dCheckRoulette );
}
//}}
//#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP

//#endif SERV_DOUBLE_LOTTERY