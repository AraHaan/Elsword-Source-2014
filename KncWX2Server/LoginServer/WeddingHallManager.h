#pragma once

#include "RefreshSingleton.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include "BaseServer.h"
#include <map>


//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM

class KWeddingHallManager
{
private:
	DeclareRefreshSingleton( KWeddingHallManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KWeddingHallManager(void);
	~KWeddingHallManager(void);

	

	void Init();
	void ShutDown();
	void SetWeddingInfoDB( IN std::map< int, KWeddingHallInfo >& mapWeddingInfo );
	void UpdateWeddingInfo_DB();



	bool AddWeddingInfo( IN KWeddingHallInfo& kWeddingInfo );		// �������� �Ϸ�
	bool UpdateWeddingInfo( IN KWeddingHallInfo& kWeddingInfo );	// ��UID ���� ����
	bool DeleteWeddingInfo( IN int iWeddingUID );					// ��ȥ�� �Ϸ�

	bool GetWeddingInfo( IN int iWeddingUID, OUT KWeddingHallInfo& kWeddingInfo );
	bool GetWeddingItemInfo( IN OUT std::map< UidType, KWeddingItemInfo >& mapWeddingItemInfo );

	void GetAllWeddingInfo( OUT std::map< int, KWeddingHallInfo >& mapWeddingInfo ) { mapWeddingInfo = m_mapWeddingHallDBData; }
protected:
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	void SendToGameDB( unsigned short usEventID );
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:

	std::map< int, KWeddingHallInfo >			m_mapWeddingHallDBData;  // [��ȥ��uid,��ȥ������] ����� ��ȥ�� ���� - db ���� ��� ����Ÿ

};

DefRefreshSingletonInline( KWeddingHallManager );

template < class T >
void KWeddingHallManager::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, usEventID, data );
}

template < class T >
void KWeddingHallManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#endif SERV_RELATIONSHIP_SYSTEM
//}