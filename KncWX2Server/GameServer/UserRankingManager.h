#pragma once
#include "CommonPacket.h"


SmartPointer(KGSUser);

class KUserRankingManager
{
public:
	KUserRankingManager(void);
	~KUserRankingManager(void);

	void clear();
	void Init( IN const std::vector< KHenirRankingInfo >& vecHenirRanking
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
		, IN const std::vector< KHenirRankingInfo >& vecHenirHeroRanking
#endif // SERV_HENIR_RENEWAL_2013
		);

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	void GetUpdateInfo( OUT std::vector< KHenirRankingInfo >& vecHenirRanking, OUT std::vector< KHenirRankingInfo >& vecHenirHeroRanking );
#else // SERV_HENIR_RENEWAL_2013
	void GetUpdateInfo( OUT std::vector< KHenirRankingInfo >& vecHenirRanking );
#endif // SERV_HENIR_RENEWAL_2013

	void GetRankingInfo( IN KGSUserPtr spUser, IN const KEGS_GET_RANKING_INFO_REQ& kReq, OUT KEGS_GET_RANKING_INFO_ACK& kAck );
	void SetRankingInfo( IN const KHenirRankingInfo& kInfo );

private:
	KHenirRankingInfo		m_kHenirRanking;
	bool					m_bHenirRankingInited;
	bool					m_bHenirRankingChanged;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	KHenirRankingInfo		m_kHenirHeroRanking;
	bool					m_bHenirHeroRankingInited;
	bool					m_bHenirHeroRankingChanged;
#endif // SERV_HENIR_RENEWAL_2013
};
