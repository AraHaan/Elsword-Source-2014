#pragma once

#include <map>
#include "GSHenirRanking.h"
#include <KNCSingleton.h>

class KGSRankingManager
{
	DeclareSingleton( KGSRankingManager );

public:
	KGSRankingManager(void);
	~KGSRankingManager(void);
	
	void Init();
	
	//////////////////////////////////////////////////////////////////////////
	// ��ŷ ���� ���
	bool GetRankingInfo( IN const KEGS_GET_RANKING_INFO_REQ& kReq, OUT KEGS_GET_RANKING_INFO_ACK& kAck );
	bool CheckNewRecord( const KHenirRankingInfo& kRankingInfo );	
	bool GetDungeonRankingByUnitUID( IN UidType iUnitUID, OUT KDungeonRankingInfo& kInfo );
	bool GetPvpRankingByUnitUID( IN UidType iUnitUID, OUT KPvpRankingInfo& kInfo );

	//{{ 2009. 10. 27  ������	ĳ���ͻ���
	void IsInRankingUser( IN UidType iUnitUID, OUT std::vector< int >& vecRankingType );
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ������Ʈ
	void UpdateHenirRanking( const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking );
	void UpdateHenirRanking( const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking, const std::map< int, u_int >& mapLastRank );
	void UpdateDungeonAndPvpRanking( IN const KELG_WEB_RANKING_REFRESH_NOT& kInfo );

protected:
	void GetDungeonRankingPage( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KDungeonRankingInfo >& vecRankingInfo ) const;
	void GetPvpRankingPage( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KPvpRankingInfo >& vecRankingInfo ) const;

protected:
	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data );

private:
	// ��ϸ��� �ð� ��ŷ
	std::map< int, KGSHenirRanking >			m_mapHenirRanking;

	// ���� ��ŷ
	std::vector< KDungeonRankingInfo >			m_vecDungeonRanking;
	std::map< UidType, KDungeonRankingInfo >	m_mapDungeonRanking;

	// ���� ��ŷ
	std::vector< KPvpRankingInfo >				m_vecPvpRanking;
	std::map< UidType, KPvpRankingInfo >		m_mapPvpRanking;
};

DefSingletonInline( KGSRankingManager );

template < class T >
void KGSRankingManager::SendToLoginServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_USER, 0, NULL, usEventID, data );
}


