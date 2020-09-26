#pragma once

#include <map>
#include <ToString.h>
#include <KncUtil.h>    // IN OUT

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
        float   m_fProb;
        int     m_nParam;
    };

    KLottery();
    ~KLottery();
    KLottery( const KLottery& t ) { *this = t; }    // copy constructor
    KLottery& operator=( const KLottery& t );       // assign operator

    float GetTotalProb() const  { return m_fTotalProb; }
    size_t GetCaseNum() const   { return m_mapCase.size(); }

    bool AddCase( int nCaseID, float fProbability, int nParam = PARAM_BLANK );
    int Decision() const;
    int GetParam( int nCaseID ) const;

    // various implement... (for Elsword project. 07.06.12.)
    bool AddProbability( float fProbAdd );  // ������ ��� case�� Ȯ���� �Ȱ��� ����. over-flow ���� return false.
	//{{ 2007. 11. 8  ������  ������ ��� case�� Ȯ���� �Ȱ��� ����.
	// over-flow���� ( 100% - TotalProb ) / mapCaseCount �� ���� �� case�� Prob�� ���� [TotalProb�� 100%�� ���� �ʰ� Ȯ���������� �����ϰ� �й�]
	void AddProbabilityHundredMaxProb( float fProbAdd );
	//}}

public:
//protected:
    std::map<int,KCaseUnit> m_mapCase;          // key : case id, element : KCaseUnit struct.
    float                   m_fTotalProb;       // ������ �ۼ�Ʈ(%)�� �Ѵ�. 100�̸� �׻� �Ͼ.
};