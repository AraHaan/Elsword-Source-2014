#pragma once

#include <map>
#include <ToString.h>
#include <KncUtil.h>    // IN OUT

//#ifdef SERV_DOUBLE_LOTTERY
class KLottery
{
    DeclToStringW;
public:
    enum {
        CASE_BLANK = -1,
        PARAM_BLANK = -2,
    };

    struct KCaseUnit    // Decision 1ȸ�� ������ ���. Ȯ���� ������ ��������� ���� param�� ����.
    {
        double	m_dProb;
        int     m_nParam1;
		int     m_nParam2;
    };

//#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
	struct KDuplicateCaseResult    // Decision 1ȸ�� ������ ���. Ȯ���� ������ ��������� ���� param�� ����.
	{
		int		m_iItemID;
		int     m_nParam1;
		int     m_nParam2;
	};
//#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP

    KLottery();
    ~KLottery();
    KLottery( const KLottery& t ) { *this = t; }    // copy constructor
    KLottery& operator=( const KLottery& t );       // assign operator

	void	Clear();
    double	GetTotalProb() const			{ return m_dTotalProb; }
    size_t	GetCaseNum() const			{ return m_mapCase.size(); }
	void	GetCaseList( std::vector< int >& vecCaseList ) const;
	//{{ 2013. 04. 15	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
	int		GetFirstCase() const;
//#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	bool	IsExistCase( int iCase ) const { return ( m_mapCase.find( iCase ) != m_mapCase.end() ); }

    bool	AddCase( int nCaseID, double dProbability, int nParam1 = PARAM_BLANK, int nParam2 = PARAM_BLANK );
	bool	AddCaseIntegerCast( int nCaseID, double dProbability, int nParam1 = PARAM_BLANK, int nParam2 = PARAM_BLANK );
//#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
	bool	AddDuplicateCaseIntegerCast( int nCaseID, double dProbability, int nParam1 /*= PARAM_BLANK*/, int nParam2 /*= PARAM_BLANK*/ );
	
	KDuplicateCaseResult DuplicateDecision( double& dCheckRoulette ) const;
	KDuplicateCaseResult DuplicateDecision() const;
	
	int		GetDuplicateParam1( int nCaseID, int iPeriod, int iQuantity  ) const;
	int		GetDuplicateParam2( int nCaseID, int iPeriod, int iQuantity  ) const;
//#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP   

    int		Decision( double& dCheckRoulette ) const;
	int		Decision() const;

	int		GetParam1( int nCaseID ) const;
	int		GetParam2( int nCaseID ) const;

    bool AddProbability( int iCase, double dProb );
    bool AddProbability( double dProbAdd );  // ������ ��� case�� Ȯ���� �Ȱ��� ����. over-flow ���� return false.	
	//{{ 2013. 04. 15	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool	DelProbability( int iCase );
//#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM	
	bool	MakeHundredProbabillty();					 // case���� �ڵ����� Ȯ���� ���ؼ� Ȯ�� �ջ��� 100%�� ����
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ �ʵ� ��� ���� - ��μ�
//#ifdef SERV_REFORM_ITEM_DROP
	bool	AddMultiProbRate( double dRate );
//#endif SERV_REFORM_ITEM_DROP
	//}}

	// �õ尪
	static void Seed( const int iSeed );

public:
//protected:
    std::map< int, KCaseUnit >	m_mapCase;          // key : case id, element : KCaseUnit struct.
//#ifdef SERV_DUPLICATE_RANDOM_ITEM_GROUP
	std::multimap< int, KCaseUnit >	m_multimapCase;          // key : case id, element : KCaseUnit struct.
//#endif //SERV_DUPLICATE_RANDOM_ITEM_GROUP
    double						m_dTotalProb;       // ������ �ۼ�Ʈ(%)�� �Ѵ�. 100�̸� �׻� �Ͼ.
};

//#endif SERV_DOUBLE_LOTTERY

// [����] ������ Ȯ���� ����(�ּ� 100�� �̻�) ����� ���� ���� �������� ���ϴ� Ŭ����
template< class T >
class KLotterySameProb
{
public:
	enum {
		CASE_BLANK = -1,	
	};

	KLotterySameProb()	{}
	~KLotterySameProb()	{}
	KLotterySameProb( const KLotterySameProb& t ) { *this = t; }    // copy constructor
	KLotterySameProb& operator=( const KLotterySameProb& t )
	{	
		m_vecCase		= t.m_vecCase;
		return *this;
	};

	void	Clear()			{ m_vecCase.clear(); }
	u_int	Size()			{ return m_vecCase.size(); }

	bool IsExist( const T CaseID ) const
	{
		std::vector< T >::const_iterator vit;
		for( vit = m_vecCase.begin(); vit != m_vecCase.end(); ++vit )
		{
			if( (*vit) == CaseID )
				return true;
		}

		return false;
	}	

	void AddCaseSameProb( T CaseID )
	{
		m_vecCase.push_back( CaseID );
		return;
	}

	T	DecisionSameProb() const
	{
		int iLastIndex = m_vecCase.size();

		// 0������ ���� ����!
		if( iLastIndex <= 0 )
			iLastIndex = 1;

		// ���� �� ���
		int iDecisionIndex = rand();

		iDecisionIndex = iDecisionIndex % iLastIndex; // 0������ ���� ����!

		_JIF( 0 <= iDecisionIndex && static_cast<int>(m_vecCase.size()) > iDecisionIndex, return KLotterySameProb::CASE_BLANK; );
		return m_vecCase[iDecisionIndex];
	}

	bool Empty() const
	{
		return ( m_vecCase.size() == 0 ); 
	}

	bool Erase( const T CaseID )
	{
		std::vector< T >::iterator vit;
		for( vit = m_vecCase.begin(); vit != m_vecCase.end(); ++vit )
		{
			if( (*vit) == CaseID )
				break;
		}

		if( vit != m_vecCase.end() )
		{
			m_vecCase.erase( vit );
			return true;
		}

		return false;
	}

	//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
//#ifdef SERV_2012_PVP_SEASON2
	void	EraseAll( const T CaseID )
	{
		std::vector< T >::iterator it = m_vecCase.begin();
		while( it != m_vecCase.end() )
		{
			if( *it == CaseID )
			{
				it = m_vecCase.erase( it );
			}
			else
			{
				++it;
			}
		}
	}
//#endif SERV_2012_PVP_SEASON2
	//}}
	
public:
	std::vector< T > m_vecCase;
};



