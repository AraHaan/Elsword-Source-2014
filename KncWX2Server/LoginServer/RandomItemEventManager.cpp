#include "LoginSimLayer.h"
#include "BaseServer.h"

#include ".\randomitemeventmanager.h"



//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT


ImplementSingleton( KRandomItemEventManager );

KRandomItemEventManager::KRandomItemEventManager(void)
{
}

KRandomItemEventManager::~KRandomItemEventManager(void)
{
}

ImplToStringW( KRandomItemEventManager )
{
	return stm_;
}

void KRandomItemEventManager::Init()
{
	START_LOG( cout, L"Ư�� �ð� ȹ�� ���� ť�� �̺�Ʈ ���� ��û!" );

	SendToLogDB( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_REQ );
}

void KRandomItemEventManager::UpdateEventInfo( IN const std::vector< KTimeOpenRandomItemEventInfo >& vecEventInfo, IN const std::set< UidType >& setGetRewardUserUIDList )
{
	CTime tCurTime = CTime::GetCurrentTime();

	BOOST_TEST_FOREACH( const KTimeOpenRandomItemEventInfo&, kInfo, vecEventInfo )
	{
		SRandomItemEvent kEventData;
		kEventData.m_iID			= kInfo.m_iID;
		kEventData.m_iItemID		= kInfo.m_iItemID;
		if( KncUtil::ConvertStringToCTime( kInfo.m_wstrDropTime, kEventData.m_tDropTime ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ���� �ð� ���� ��ȯ�� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kInfo.m_iID )
				<< BUILD_LOG( kInfo.m_wstrDropTime )
				<< END_LOG;
			continue;
		}
		//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
		if( KncUtil::ConvertStringToCTime( kInfo.m_wstrDropEndTime, kEventData.m_tDropEndTime ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ���� �ð� ���� ��ȯ�� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kInfo.m_iID )
				<< BUILD_LOG( kInfo.m_wstrDropEndTime )
				<< END_LOG;
			continue;
		}
		// �̹� ���� �ð��̶�� ��������!
		if( tCurTime > kEventData.m_tDropEndTime )
			continue;
#else
		// �̹� ���� �ð��̶�� ��������!
		if( tCurTime > kEventData.m_tDropTime )
			continue;
#endif SERV_NAVER_PROMOTIONS
		//}} 

		//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		kEventData.m_iOpenItemID		= kInfo.m_iOpenItemID;
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}

		//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
		std::map< __int64, std::vector< SRandomItemEvent > >::iterator mit = m_mapEventInfo.find( kEventData.m_tDropTime.GetTime() );
		if( mit != m_mapEventInfo.end() )	// ������ �����Ͱ� �ִ�.
		{
			std::vector< SRandomItemEvent >& vecEventList = mit->second;

			if( vecEventList.size() > 0 ) // �������� vector ���� Ȯ��
			{
				vecEventList.push_back( kEventData );
			}
		}
		else // ������ �����Ͱ� ���� �� vector ���� �� ����
		{
			std::vector< SRandomItemEvent > vecEventList;
			vecEventList.push_back( kEventData );

			m_mapEventInfo.insert( std::make_pair( kEventData.m_tDropTime.GetTime(), vecEventList ) );
		}
#else
		m_mapEventInfo.insert( std::make_pair( kEventData.m_tDropTime.GetTime(), kEventData ) );
#endif SERV_NAVER_PROMOTIONS
		//}} 
	}

	m_setGetRewardUserUIDList = setGetRewardUserUIDList;

	START_LOG( cout, L"Ư�� �ð� ȹ�� ���� ť�� �̺�Ʈ ���� ������Ʈ!" )
		<< BUILD_LOG( m_mapEventInfo.size() )
		<< BUILD_LOG( m_setGetRewardUserUIDList.size() );
}

//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
void KRandomItemEventManager::CheckTimeOpenRandomItemEvent( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iRandomItemID, OUT int& iEventItemID )
#else
void KRandomItemEventManager::CheckTimeOpenRandomItemEvent( IN const UidType iUserUID, IN const UidType iUnitUID, OUT int& iEventItemID )
#endif SERV_RANDOM_CUBE_GOLD_REWARD
//}}
{
	iEventItemID = 0;

	// �̺�Ʈ�� ����.
	if( m_mapEventInfo.empty() )
		return;

	// ���� �����̴�.
	if( m_setGetRewardUserUIDList.find( iUserUID ) != m_setGetRewardUserUIDList.end() )	
		return;

	CTime tCurTime = CTime::GetCurrentTime();

	SRandomItemEvent sEventData;
	//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
	std::map< __int64, std::vector< SRandomItemEvent > >::iterator mit;
#else
	std::map< __int64, SRandomItemEvent >::iterator mit;
#endif SERV_NAVER_PROMOTIONS
	//}} 

	//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
	std::vector< __int64 > vecDeleteList;

	for( mit = m_mapEventInfo.begin(); mit != m_mapEventInfo.end(); ++mit )
	{
		CTime tDropTime = CTime( mit->first );

		// �̺�Ʈ �ð��� �Ǿ����� üũ!
		if( tCurTime < tDropTime )
			continue;

		std::vector< SRandomItemEvent >& vecEventDataList = mit->second;
		if( vecEventDataList.size() <= 0 )
		{
			vecDeleteList.push_back( mit->first );
			continue;
		}
		
		std::vector< SRandomItemEvent >::iterator vit = vecEventDataList.begin();
		for( ; vit != vecEventDataList.end() ; ++vit ) // vector �� �����
		{
			sEventData = (*vit);

			if( sEventData.m_iOpenItemID != iRandomItemID )
				continue;

			// �̺�Ʈ ���� ���� ������ ���!
			m_setGetRewardUserUIDList.insert( iUserUID );

			// �̺�Ʈ ���� ����
			iEventItemID = sEventData.m_iItemID;
			LIF( iEventItemID != 0 );

			KDBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT kNot;
			kNot.m_kDBUpdateInfo.m_iID = sEventData.m_iID;
			kNot.m_kDBUpdateInfo.m_iGetUserUID = iUserUID;
			kNot.m_kDBUpdateInfo.m_iGetUnitUID = iUnitUID;
			kNot.m_kDBUpdateInfo.m_tUpdateTime = tCurTime.GetTime();
			SendToLogDB( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT, kNot );
			break;
		}

		if( vit != vecEventDataList.end() )
		{
			vecEventDataList.erase( vit );
			break;
		}
	}

	BOOST_TEST_FOREACH( const __int64, iTime, vecDeleteList )
	{
		m_mapEventInfo.erase( iTime );
	}
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� �ϴ� �̺�Ʈ - �ϳ��� �ð� �ۿ� ��� ���ϴ� ����?!
	//////////////////////////////////////////////////////////////////////////
	/*
	for( mit = m_mapEventInfo.begin(); mit != m_mapEventInfo.end(); ++mit )
	{
		CTime tDropTime = CTime( mit->first );

		// �̺�Ʈ �ð��� �Ǿ����� üũ!
		if( tCurTime < tDropTime )
			continue;

		//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
		
#else
		// �̺�Ʈ ����
		sEventData = mit->second;
#endif SERV_NAVER_PROMOTIONS
		//}} 
		break;
	}

	if( mit != m_mapEventInfo.end() )
	{
		//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
		std::vector< SRandomItemEvent >& vecEventDataList = mit->second;
		if( vecEventDataList.size() <= 0  ) // vector �� �����
		{
			m_mapEventInfo.erase( mit );
			return;
		}

		sEventData = vecEventDataList.back();
		vecEventDataList.pop_back();

		if( vecEventDataList.size() <= 0 )
		{
			m_mapEventInfo.erase( mit );
		}

#else
		m_mapEventInfo.erase( mit );
#endif SERV_NAVER_PROMOTIONS
		//}} 

		// �̺�Ʈ ���� ���� ������ ���!
		m_setGetRewardUserUIDList.insert( iUserUID );

		// �̺�Ʈ ���� ����
		iEventItemID = sEventData.m_iItemID;
		LIF( iEventItemID != 0 );

		KDBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT kNot;
		kNot.m_kDBUpdateInfo.m_iID = sEventData.m_iID;
		kNot.m_kDBUpdateInfo.m_iGetUserUID = iUserUID;
		kNot.m_kDBUpdateInfo.m_iGetUnitUID = iUnitUID;
		kNot.m_kDBUpdateInfo.m_tUpdateTime = tCurTime.GetTime();
        SendToLogDB( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT, kNot );
	}
	*/
	//////////////////////////////////////////////////////////////////////////
	
}

void KRandomItemEventManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}

