#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <KncSend.h>
#include <map>
#include <boost/timer.hpp>
#include "BaseServer.h"
#include "HenirRanking.h"

class KRankingManager
{
	DeclareSingleton( KRankingManager );
	DeclDump;
	DeclToStringW;

	typedef std::pair< int, int >	KTitleRewardKey;

public:
	KRankingManager(void);
	~KRankingManager(void);

	bool OpenScriptFile( const char* pFileName );
	bool AddHenirRankingInfo_LUA( int iRankingType, int iLastRank, int iHour, int iDayOfWeek = 0, int iWeekOfMonth = 0 );	
	bool AddRankingRewardInfo_LUA( int iRankingType, int iRank, int iTitleID, short sPeriod );
	//{{ 2009. 8. 3  ������		��ŷ�������
	bool SetRankingRewardBeginDate_LUA( int iRankingType, const char* pBeginDate );
	//}}

	void NewRecordForTest_LUA( int iCount );
	//{{ 2009. 7. 31  ������	��ŷ �߰�/����
	void NewRecord_LUA( int iRankingType, int iStageCount, UINT ulPlayTime, __int64 tRegDate );
	void DeleteRecord_LUA( int iRankingType, const char* pNickName );
	//}}

	virtual	void Tick();

	void Init();
	void ShutDown();

	void DumpRankingInfo();

	//////////////////////////////////////////////////////////////////////////
	// ��ϸ� ��ŷ
	void TickHenirRanking();

	void ClearRanking_LUA( IN bool bSendToFTP = true );
	void CheckRankingClearAndReward( IN bool bForce = false );
	
	void InitHenirRanking( IN const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking );
	void GetHenirRankingInfo( OUT std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking, IN bool bChangedOnly );
	void GetHenirRankingInfo( IN int iRankingType, OUT std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking );
	void GetHenirLastRank( OUT std::map< int, u_int >& mapLastRank );

	bool NewRecordHenirRanking( IN const KHenirRankingInfo& kNewRecord );
	bool NewRecordHenirRanking( IN const KHenirRankingInfo& kNewRecord, IN int iRankingType );	

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 7. 31  ������	��ŷ ����
	void NewRecordForTest( const std::vector< KHenirRankingInfo >& vecNewRecordList );
	bool DeleteRecord( int iRankingType, const std::wstring& wstrNickName );
	bool DeleteRecord( int iRankingType, UidType iUnitUID );
	//}}

	//{{ 2011.03.04  �ӱԼ� ��ϸ� ��ŷ ���� ġƮ ( ���,������ ���� )
#ifdef SERV_DELETE_HENIR_RANKING
	bool DeleteSearchRank( int iRankingType, int iRank);
#endif SERV_DELETE_HENIR_RANKING
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ����&���� ��ŷ
	void TickDungeonAndPvpRanking();
	void UpdateDungeonRanking( const std::vector< KDungeonRankingInfo >& vecDungeonRanking );
	void UpdatePvpRanking( const std::vector< KPvpRankingInfo >& vecPvpRanking );
	void GetDungeonRankingVector( IN u_int uiLastIndex, OUT KELG_WEB_RANKING_REFRESH_NOT& kNot );
	void GetDungeonRankingMap( IN u_int uiLastIndex, OUT KELG_WEB_RANKING_REFRESH_NOT& kNot );
	void GetPvpRankingVector( IN u_int uiLastIndex, OUT KELG_WEB_RANKING_REFRESH_NOT& kNot );
	void GetPvpRankingMap( IN u_int uiLastIndex, OUT KELG_WEB_RANKING_REFRESH_NOT& kNot );

protected:
	bool BackupRankingLogFile( IN int iRankingType, IN bool bSendToFTP = true );
	void RefreshTimeFileBackup();
	bool CheckRefreshTimeFileBackup();

	bool RankingRewardProcess( IN int iRankingType );
	bool GetRankingTitleRewardInfo( IN int iRankingType, IN int iRank, OUT std::vector< KTitleReward >& vecReward ); // ��ŷ Īȣ ����

protected:
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	//////////////////////////////////////////////////////////////////////////
	// ��ϸ� �ð� ��ŷ
	std::map< int, KHenirRanking >								m_mapHenirRanking;			// ��ϸ� �ð� ��ŷ
	std::map< KTitleRewardKey, std::vector< KTitleReward > >	m_mapRankingTitleReward;	// ��ŷ Ÿ��Ʋ ����

	boost::timer								m_tBroadCastTimer;							// ��ε� ĳ������ ���� Ÿ�̸�
	boost::timer								m_tClearTimer;								// �ʱ�ȭ üũ�� ���� Ÿ�̸�
	bool										m_bInit;									// DB�κ��� ��ŷ ����Ʈ �޾Ҵ��� ����
	bool										m_bReservedRankingReward;					// ��ŷ ����ó�� ����
	
	//////////////////////////////////////////////////////////////////////////
	// ����&���� ��ŷ
	std::vector< KDungeonRankingInfo >			m_vecDungeonRanking;						// ���� ��ŷ
	std::map< UidType, KDungeonRankingInfo >	m_mapDungeonRanking;
	std::vector< KPvpRankingInfo >				m_vecPvpRanking;							// ���� ��ŷ
	std::map< UidType, KPvpRankingInfo >		m_mapPvpRanking;
	
	CTime										m_tResetRankingTime;
	boost::timer								m_kRefreshTimer;	
};

DefSingletonInline( KRankingManager );

template < class T >
void KRankingManager::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}

template < class T >
void KRankingManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}


