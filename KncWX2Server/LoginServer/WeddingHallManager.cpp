#include "WeddingHallManager.h"
#include "NetError.h"
#include "kncutil.h"
#include "Enum/Enum.h"
#include "BaseServer.h"

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM

ImplementRefreshSingleton( KWeddingHallManager );

ImplToStringW( KWeddingHallManager )
{
	return stm_;
}

KWeddingHallManager::KWeddingHallManager()
{
}

KWeddingHallManager::~KWeddingHallManager()
{
}

ImplementLuaScriptParser( KWeddingHallManager )
{

}

void KWeddingHallManager::Init()
{
	// DB �� �ִ� ��ȥ�� ������ ��û�Ѵ�.
	SendToGameDB( DBE_LOAD_WEDDING_HALL_INFO_REQ );
}

void KWeddingHallManager::ShutDown()
{
	// Manager �� �ִ� ��ȥ�� ������ DB �� ������Ʈ �Ѵ�.
	UpdateWeddingInfo_DB();
}

void KWeddingHallManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

void KWeddingHallManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

void KWeddingHallManager::SetWeddingInfoDB( IN std::map< int, KWeddingHallInfo >& mapWeddingInfo )
{
	// ���� DB ������ �������� �����ϴ� �Լ�

	std::map< int, KWeddingHallInfo >::iterator mitNew = mapWeddingInfo.begin();
	for( ; mitNew != mapWeddingInfo.end() ; ++mitNew )
	{
		CTime tWeddingDate;
		KncUtil::ConvertStringToCTime( mitNew->second.m_wstrWeddingDate, tWeddingDate );
		mitNew->second.m_tWeddingDate = tWeddingDate.GetTime();

		std::map< int, KWeddingHallInfo >::iterator mitOld = m_mapWeddingHallDBData.find( mitNew->first );
		if( mitOld == m_mapWeddingHallDBData.end() )
		{
			m_mapWeddingHallDBData.insert( std::make_pair( mitNew->first, mitNew->second ));
		}
		else
		{
			// �����Ѵٸ� ��� �� ���ΰ�? // DB ���� �� ������ �̹Ƿ� ���� ����
			mitOld->second = mitNew->second;
		}
	}
}

void KWeddingHallManager::UpdateWeddingInfo_DB()
{
	// �α��� ���� ���� �� �� ȣ�� ��

	// ��ȥ �ð��� �ѱ� ������
	// ��ȥ�� �Ϸ�� ������
	// ��UID 0���� �ʱ�ȭ? ��� �� �غ���

	CTime tCurr = CTime::GetCurrentTime();

	KDBE_WEDDING_HALL_INFO_UPDATE_NOT kPacketToDB;

	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.begin();
	for( ; mit != m_mapWeddingHallDBData.end() ; ++mit )
	{
		KWeddingHallInfo kInfo = mit->second;

		if( mit->second.m_bSuccess == true )
		{
			kInfo.m_bDelete = true;
		}

		if( mit->second.m_tWeddingDate < tCurr.GetTime() )
		{
			kInfo.m_bDelete = true;
		}
		
		kPacketToDB.m_mapWeddingInfo.insert( std::make_pair( kInfo.m_iWeddingUID, kInfo ) );

		if( kPacketToDB.m_mapWeddingInfo.size() >= 100 )
		{
			SendToGameDB( DBE_WEDDING_HALL_INFO_UPDATE_NOT, kPacketToDB );
			kPacketToDB.m_mapWeddingInfo.clear();
		}
	}

	SendToGameDB( DBE_WEDDING_HALL_INFO_UPDATE_NOT, kPacketToDB );
}

bool KWeddingHallManager::AddWeddingInfo( IN KWeddingHallInfo& kWeddingInfo )
{
	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( kWeddingInfo.m_iWeddingUID );
	if( mit != m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[�׽�Ʈ]�����ϴ� ��ȥ�� ���� �Դϴ�. �ߺ� ���� �Է� �Ϸ� �մϴ�." )
			<< BUILD_LOG( kWeddingInfo.m_iWeddingUID )
			<< BUILD_LOG( kWeddingInfo.m_iGroom )
			<< BUILD_LOG( kWeddingInfo.m_iBride )
			<< BUILD_LOGc( kWeddingInfo.m_cWeddingHallType )
			<< BUILD_LOGc( kWeddingInfo.m_cOfficiantNPC )
			<< BUILD_LOG( kWeddingInfo.m_wstrWeddingDate )
			<< BUILD_LOG( kWeddingInfo.m_bSuccess )
			<< BUILD_LOG( kWeddingInfo.m_bDelete )
			<< END_LOG;

		return false;
	}

	m_mapWeddingHallDBData.insert( std::make_pair( kWeddingInfo.m_iWeddingUID, kWeddingInfo ));
	return true;
}

bool KWeddingHallManager::UpdateWeddingInfo( IN KWeddingHallInfo& kWeddingInfo )
{
	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( kWeddingInfo.m_iWeddingUID );
	if( mit == m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[�׽�Ʈ]�������� �ʴ� ��ȥ�� UID �� �ƴմϴ�." )
			<< BUILD_LOG( kWeddingInfo.m_iWeddingUID )
			<< END_LOG;

		return false;
	}

	mit->second = kWeddingInfo;
	return true;
}

bool KWeddingHallManager::DeleteWeddingInfo( IN int iWeddingUID )
{
	if( iWeddingUID <= 0 )
	{
		START_LOG( cerr, L"�������� ��ȥ�� UID �� �ƴմϴ�." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( iWeddingUID );
	if( mit == m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[�׽�Ʈ]�������� �ʴ� ��ȥ�� UID �� �ƴմϴ�." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	m_mapWeddingHallDBData.erase( mit );

	return true;
}

bool KWeddingHallManager::GetWeddingInfo( IN int iWeddingUID, OUT KWeddingHallInfo& kWeddingInfo )
{
	if( iWeddingUID <= 0 )
	{
		START_LOG( cerr, L"�������� ��ȥ�� UID �� �ƴմϴ�." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	std::map< int, KWeddingHallInfo >::iterator mit = m_mapWeddingHallDBData.find( iWeddingUID );
	if( mit == m_mapWeddingHallDBData.end() )
	{
		START_LOG( cerr, L"[�׽�Ʈ]�������� �ʴ� ��ȥ�� UID �� �ƴմϴ�." )
			<< BUILD_LOG( iWeddingUID )
			<< END_LOG;

		return false;
	}

	if( mit->second.m_bSuccess == true )
	{
		return false;
	}

	kWeddingInfo = mit->second;

	return true;
}

bool KWeddingHallManager::GetWeddingItemInfo( IN OUT std::map< UidType, KWeddingItemInfo >& mapWeddingItemInfo )
{
	std::map< UidType, KWeddingItemInfo >::iterator mit = mapWeddingItemInfo.begin();
	for( ; mit != mapWeddingItemInfo.end() ; ++mit )
	{
		KWeddingHallInfo kWeddingInfo;
		if( GetWeddingInfo( mit->second.m_iWeddingUID, kWeddingInfo ) == false )
		{
			continue;
		}

		mit->second.m_cWeddingHallType = kWeddingInfo.m_cWeddingHallType;
		mit->second.m_cOfficiantNPC = kWeddingInfo.m_cOfficiantNPC;
		mit->second.m_iGroom = kWeddingInfo.m_iGroom;
		mit->second.m_iBride = kWeddingInfo.m_iBride;
		mit->second.m_wstrWeddingDate = kWeddingInfo.m_wstrWeddingDate;
		mit->second.m_wstrWeddingMsg = kWeddingInfo.m_wstrWeddingMsg;
	}
	
	return true;
}

#endif SERV_RELATIONSHIP_SYSTEM
//}


