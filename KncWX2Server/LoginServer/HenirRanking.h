#pragma once

#include "Event.h"
#include "CommonPacket.h"


class KHenirRanking
{
public:
	KHenirRanking(void);
	virtual ~KHenirRanking(void);

	u_int	GetLastRank()												{ return m_uiLastRank; }
	void	SetLastRank( u_int uiLastRank )								{ m_uiLastRank = uiLastRank; }
	void	SetRefreshTime( const int iRankingType, const int iWeekOfMonth, const int iDayOfWeek, const int iHour );
	CTime	GetRefreshTime() const										{ return m_tRefreshTime; }
	void	NextRefreshTimeSet();

	void	SetRewardBeginTime( const std::wstring& wstrBeginTime );
	CTime	GetRewardBeginTime()										{ return m_tRewardBeginTime; }
	
	bool	IsRankingChanged() const									{ return m_bIsRankingChange; }		// ��ŷ ���� Ȯ��
	void	SetRankingChanged( bool bVal )								{ m_bIsRankingChange = bVal; }
	
	bool	CheckNewRecord( IN const KHenirRankingInfo& kNewRankingInfo );									// ��ŷ ��� �õ�
	bool	DeleteRecord( IN const std::wstring& wstrNickName );
	bool	DeleteRecord( IN const UidType iUnitUID );

	const std::vector< KHenirRankingInfo >& GetRankingInfo() const		{ return m_vecRankingInfo; }
	void	SetRankingInfo( const std::vector< KHenirRankingInfo >& vecRankingInfo );
	void	ClearRanking();

private:	
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	bool	RankingSort( const UidType& iUnitUID );
#endif SERV_HENIR_RANKING_GROUP_FIX
	void	RankingSort();
	static CTime GetDayOfWeekInMonth( int iYear, int iMonth, int iWeekOfMonth, int iDayOfWeek, int iHour );

private:
	std::vector< KHenirRankingInfo >	m_vecRankingInfo;		// ��ŷ ����Ʈ
	u_int								m_uiLastRank;			// ��ŷ ����Ʈ�� ������ ��ũ
	bool								m_bIsRankingChange;		// ��ŷ ���� ����
	
	int									m_iRankingType;			// ��ŷ Ÿ��
	CTime								m_tRefreshTime;			// ��ŷ ���� �ʱ�ȭ �ð�
	int									m_iRefreshWeekOfMonth;
	int									m_iRefreshDayOfWeek;
	int									m_iRefreshHour;

	//{{ 2009. 8. 3  ������		��ŷ ���� ���۽ð�
	CTime								m_tRewardBeginTime;		// ���� ���� �ð�
	//}}
};