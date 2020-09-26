#pragma once

#include "GSSimLayer.h"

class KGSHenirRanking
{
public:
	enum RANKING_ENUM
	{
        PAGE_PER_COUNT = 5,	// ���������� ǥ�õǴ� ��ŷ ��
	};

public:
	KGSHenirRanking(void);
	~KGSHenirRanking(void);
	
	//{{ 2009. 10. 27  ������	ĳ���ͻ���
	bool IsInRankingUser( UidType iUnitUID ) const;
	//}}
	void SetLastRank( u_int uiLastRank ) { m_uiLastRank = m_uiLastRank; }

	void ClearRankingInfo() { m_vecRankingInfo.clear(); }
	bool CheckNewRecord( IN const KHenirRankingInfo& kNewRank ) const;
	void UpdateRankingInfo( IN const std::vector< KHenirRankingInfo >& vecRankingInfo );
	void GetRankingInfo( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KHenirRankingInfo >& vecRankingInfo ) const;

private:
	std::vector< KHenirRankingInfo >	m_vecRankingInfo;
	u_int								m_uiLastRank;
};
