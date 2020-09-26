#include "GSUser.h"
#include ".\UserEventManager.h"
//{{ 2012. 10. 13	�ڼ���	���� �̺�Ʈ ���� ����
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
#else
	#include "GameEvent\GameEventManager.h"
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}



KUserEventManager::KUserEventManager(void)
{
	Clear();
}

KUserEventManager::~KUserEventManager(void)
{
}

void KUserEventManager::Clear()
{
	//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	m_bAttendanceCheck = false;
#endif SERV_WEB_POINT_EVENT
	//}}

	m_mapConnectTimeEvent.clear();
	//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
	m_mapCumulativeTimeEvent.clear();
	m_setCompletedEvent.clear();
	m_setDirectRewardEvent.clear();
#endif CUMULATIVE_TIME_EVENT
	//}}
}

//{{ 2012. 10. 13	�ڼ���	���� �̺�Ʈ ���� ����
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
void KUserEventManager::InitTimeEvent( IN KGameEventManager::ConnectEventFactorSet& kFactor )
{
	if( ( kFactor.m_pVecConnectTimeEvent == NULL ) ||
		( kFactor.m_pVecCumulativeTimeEvent == NULL ) )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ���� �� �̻�!! ( �־ �ȵǴ� �����Դϴ�. ��� Ȯ�����ּ��� )" )
			<< BUILD_LOG( kFactor.m_pVecConnectTimeEvent )
			<< BUILD_LOG( kFactor.m_pVecCumulativeTimeEvent )
			<< END_LOG;
		return;
	}

	//////////////////////////////////////////////////////////////////////////	
	// 1. �̺�Ʈ ���� �ʱ�ȭ
	Clear();

	//////////////////////////////////////////////////////////////////////////	
	// 2. DB�κ��� �޾ƿ� ���ֺ� ���� �ð� �̺�Ʈ ���� ����

	CTime tEventTime;
	CTime tCurTime = CTime::GetCurrentTime();

	// ���� �ð� �̺�Ʈ ����
	for( u_int ui = 0; ui < kFactor.m_pVecCumulativeTimeEvent->size(); ++ui )
	{
		std::pair< KUserCumulativeTimeEventInfo, KTimerManager > kUserCumulativeEvent;
		kUserCumulativeEvent.first.m_iEventUID = ( *kFactor.m_pVecCumulativeTimeEvent )[ui].m_iEventUID;
		kUserCumulativeEvent.second.restart();
		kUserCumulativeEvent.second.SetAddTime( static_cast<double>( ( *kFactor.m_pVecCumulativeTimeEvent )[ui].m_iCumulativeTime * 60 ) );
		m_mapCumulativeTimeEvent.insert( std::make_pair( ( *kFactor.m_pVecCumulativeTimeEvent )[ui].m_iEventUID, kUserCumulativeEvent ) );
	}

	// ���� �ð� �̺�Ʈ ����
	for( u_int ui = 0; ui < kFactor.m_pVecConnectTimeEvent->size(); ++ui )
	{
		if( !KncUtil::ConvertStringToCTime( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_wstrEventTime, tEventTime ) )
		{
			START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
				<< BUILD_LOG( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_wstrEventTime )
				<< END_LOG;

			// ���ڿ� ��ȯ ���н� ���� ���� �ʴ°����� ó��
			tEventTime = CTime( 2050, tCurTime.GetMonth(), tCurTime.GetDay(), 23, 59, 59 );
		}

		// �̹� �̺�Ʈ ������ �޾Ҵ°�?
		if( tCurTime <= tEventTime )
		{
			// �Ϸ� ����Ʈ�� �ִ´�.
			m_setCompletedEvent.insert( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_iEventUID );
			continue;
		}

		// �ð� �̺�Ʈ ����Ʈ���� ���� �ð� �̺�Ʈ�� �ƴѰ͸� ã�Ƽ� �����Ѵ�.
		if( m_mapCumulativeTimeEvent.find( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_iEventUID ) == m_mapCumulativeTimeEvent.end() )
		{
			std::pair< KUserConnectTimeEventInfo, KTimerManager > kUserConnectEvent;
			kUserConnectEvent.first.m_fEventTime = END_EVENT; // �⺻������ ����� �̺�Ʈ���� �ִ´�
			kUserConnectEvent.first.m_kCTime		= tEventTime.GetTime();
			kUserConnectEvent.first.m_bGiveOnce	= true;
			kUserConnectEvent.second.restart(); // Ÿ�̸� �ʱ�ȭ
			m_mapConnectTimeEvent.insert( std::make_pair( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_iEventUID, kUserConnectEvent ) );
		}
	}

	kFactor.m_pMapEventTime			= &m_mapConnectTimeEvent;
	kFactor.m_pSetCompletedEvent	= &m_setCompletedEvent;

	// 3. ���� �������� �̺�Ʈ�� üũ�Ͽ� EventTime�� ��´�.
	SiKGameEventManager()->CheckEnableConnectTimeEventForCharInit( kFactor );

	// 4. ���� �������� �̺�Ʈ�� üũ�Ͽ� EventTime�� ��´�.
	SiKGameEventManager()->CheckEnableCumulativeTimeEventForCharInit(
		kFactor.m_ucLevel,
		kFactor.m_bIsPcBang

		//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		, kFactor.m_iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}
		//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		, kFactor
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}

		, m_mapCumulativeTimeEvent
		, m_setCompletedEvent
		);

#ifdef SERV_ADVERTISEMENT_EVENT
	for( u_int ui = 0; ui < kFactor.m_pVecCompletedAdvertisementEvent->size(); ++ui )
	{
		if( m_setCompletedEvent.find( ( *kFactor.m_pVecCompletedAdvertisementEvent )[ui] ) == m_setCompletedEvent.end() )
		{
			m_setCompletedEvent.insert( ( *kFactor.m_pVecCompletedAdvertisementEvent )[ui] );
		}
	}
#endif SERV_ADVERTISEMENT_EVENT
}

void KUserEventManager::CheckConnectTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor )
{
	if( spUser == NULL )
		return;

	KDBE_UPDATE_EVENT_TIME_REQ kPacketReq;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotEnd;

	kFactor.m_pMapEventTime			= &m_mapConnectTimeEvent;
	kFactor.m_pSetCompletedEvent	= &m_setCompletedEvent;
	kFactor.m_pVecUpdatedTime		= &kPacketReq.m_vecConnectTimeEvent;
	kFactor.m_pVecBeginEvent		= &kPacketNotBegin.m_vecTimeEvent;
	kFactor.m_pVecEndEvent			= &kPacketNotEnd.m_vecTimeEvent;

	// �̺�Ʈ ���� ���� �ð��� �� �Ǿ����� üũ, ����Ǵ� �̺�Ʈ üũ, ���� �����ϴ� �̺�Ʈ üũ
	SiKGameEventManager()->CheckConnectTimeEvent( kFactor );

	// ���ӽð��� �������Ѽ� ������� �̺�Ʈ�� �ִ���?
	if( !kPacketReq.m_vecConnectTimeEvent.empty() )
	{
		// ���� �ð� �̺�Ʈ ���� ������ ����!
		kPacketReq.m_iUnitUID = spUser->GetCharUID();
#ifdef SERV_ADD_EVENT_DB
		spUser->SendToEventDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#else //SERV_ADD_EVENT_DB
		spUser->SendToGameDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#endif //SERV_ADD_EVENT_DB
	}

	// ����� �̺�Ʈ�� �ִ���?
	if( !kPacketNotEnd.m_vecTimeEvent.empty() )
	{
		kPacketNotEnd.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_END;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotEnd );
	}

	// ���� ���۵Ǵ� �̺�Ʈ�� �ִ���?
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotBegin );
	}
}
#else
void KUserEventManager::InitTimeEvent( IN const u_char ucLevel,
									   IN const bool bIsPcBang,
									   //{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
									   IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
									   //}}
									   //{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
									   IN const int iBF_Team,
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
									   //}}
									   //{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
									   IN const char cUnitType,
#endif SERV_CHAR_CONNECT_EVENT
									   //}}
									   IN const std::vector< KConnectTimeEventInfo >& vecEventTime, 
									   IN const std::vector< KCumulativeTimeEventInfo >& vecCumulativeEventTime
									   //{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
									   , IN const UidType& iUnitUID
									   , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
									   //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
									   , IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
									   )
{
	//////////////////////////////////////////////////////////////////////////	
	// 1. �̺�Ʈ ���� �ʱ�ȭ
	Clear();

	//////////////////////////////////////////////////////////////////////////	
	// 2. DB�κ��� �޾ƿ� ���ֺ� ���� �ð� �̺�Ʈ ���� ����

	CTime tEventTime;
	CTime tCurTime = CTime::GetCurrentTime();

	// ���� �ð� �̺�Ʈ ����
	for( u_int ui = 0; ui < vecCumulativeEventTime.size(); ++ui )
	{
		std::pair< KUserCumulativeTimeEventInfo, KTimerManager > kUserCumulativeEvent;
		kUserCumulativeEvent.first.m_iEventUID = vecCumulativeEventTime[ui].m_iEventUID;
		kUserCumulativeEvent.second.restart();
		kUserCumulativeEvent.second.SetAddTime( static_cast<double>( vecCumulativeEventTime[ui].m_iCumulativeTime * 60 ) );
		m_mapCumulativeTimeEvent.insert( std::make_pair( vecCumulativeEventTime[ui].m_iEventUID, kUserCumulativeEvent ) );
	}

	// ���� �ð� �̺�Ʈ ����
	for( u_int ui = 0; ui < vecEventTime.size(); ++ui )
	{
		if( !KncUtil::ConvertStringToCTime( vecEventTime[ui].m_wstrEventTime, tEventTime ) )
		{
			START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
				<< BUILD_LOG( vecEventTime[ui].m_wstrEventTime )
				<< END_LOG;

			// ���ڿ� ��ȯ ���н� ���� ���� �ʴ°����� ó��
			tEventTime = CTime( 2050, tCurTime.GetMonth(), tCurTime.GetDay(), 23, 59, 59 );
		}

		// �̹� �̺�Ʈ ������ �޾Ҵ°�?
		if( tCurTime <= tEventTime )
		{
			// �Ϸ� ����Ʈ�� �ִ´�.
			m_setCompletedEvent.insert( vecEventTime[ui].m_iEventUID );
			continue;
		}

		// �ð� �̺�Ʈ ����Ʈ���� ���� �ð� �̺�Ʈ�� �ƴѰ͸� ã�Ƽ� �����Ѵ�.
		if( m_mapCumulativeTimeEvent.find( vecEventTime[ui].m_iEventUID ) == m_mapCumulativeTimeEvent.end() )
		{
			std::pair< KUserConnectTimeEventInfo, KTimerManager > kUserConnectEvent;
			kUserConnectEvent.first.m_fEventTime = END_EVENT; // �⺻������ ����� �̺�Ʈ���� �ִ´�
			kUserConnectEvent.first.m_kCTime		= tEventTime.GetTime();
			kUserConnectEvent.first.m_bGiveOnce	= true;
			kUserConnectEvent.second.restart(); // Ÿ�̸� �ʱ�ȭ
			m_mapConnectTimeEvent.insert( std::make_pair( vecEventTime[ui].m_iEventUID, kUserConnectEvent ) );
		}
	}

	// 3. ���� �������� �̺�Ʈ�� üũ�Ͽ� EventTime�� ��´�.
	SiKGameEventManager()->CheckEnableConnectTimeEventForCharInit(
		ucLevel,
		bIsPcBang

		//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		, iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}

		//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		, iBF_Team
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}

		//{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
		, cUnitType
#endif SERV_CHAR_CONNECT_EVENT
		//}}

		, m_mapConnectTimeEvent
		, m_setCompletedEvent
		//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		, iUnitUID
		, mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		, setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		);

	// 4. ���� �������� �̺�Ʈ�� üũ�Ͽ� EventTime�� ��´�.
	SiKGameEventManager()->CheckEnableCumulativeTimeEventForCharInit(
		ucLevel,
		bIsPcBang
		
		//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		, iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}

		, m_mapCumulativeTimeEvent
		, m_setCompletedEvent
		);
}

void KUserEventManager::CheckConnectTimeEvent( IN KGSUserPtr spUser
											  //{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
											  , IN const UidType& iUnitUID
											  , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
											  //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
											  , IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
											  )
{
	if( spUser == NULL )
		return;
	
	KDBE_UPDATE_EVENT_TIME_REQ kPacketReq;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotEnd;

	// �̺�Ʈ ���� ���� �ð��� �� �Ǿ����� üũ, ����Ǵ� �̺�Ʈ üũ, ���� �����ϴ� �̺�Ʈ üũ
	SiKGameEventManager()->CheckConnectTimeEvent( spUser->GetLevel(),
												  spUser->IsPcBang(),
												  //{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
												  spUser->GetChannelCode(),
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
												  //}}
												  //{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
												  spUser->GetBF_Team(),
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
												  //}}
												  //{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
												  spUser->GetUnitType(),
#endif SERV_CHAR_CONNECT_EVENT
												  //}}
												  m_mapConnectTimeEvent, 
												  m_setCompletedEvent, 
												  kPacketReq.m_vecConnectTimeEvent, 
												  kPacketNotBegin.m_vecTimeEvent, 
												  kPacketNotEnd.m_vecTimeEvent
												  //{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
												  , iUnitUID
												  , mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
												 //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
												  , setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
												  );

	// ���ӽð��� �������Ѽ� ������� �̺�Ʈ�� �ִ���?
	if( !kPacketReq.m_vecConnectTimeEvent.empty() )
	{
		// ���� �ð� �̺�Ʈ ���� ������ ����!
		kPacketReq.m_iUnitUID = spUser->GetCharUID();
#ifdef SERV_ADD_EVENT_DB
		spUser->SendToEventDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#else //SERV_ADD_EVENT_DB
		spUser->SendToGameDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#endif //SERV_ADD_EVENT_DB
	}

	// ����� �̺�Ʈ�� �ִ���?
	if( !kPacketNotEnd.m_vecTimeEvent.empty() )
	{
		kPacketNotEnd.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_END;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotEnd );
	}

	// ���� ���۵Ǵ� �̺�Ʈ�� �ִ���?
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotBegin );
	}
}
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}

//{{ 2010. 7. 27  ������	���� �ð� �̺�Ʈ ���� ����
#ifdef SERV_TIME_EVENT_BUG_FIX

void KUserEventManager::GetEventTimerInfo( OUT std::map< int, KUserConnectTimeEventInfo >& mapEventTimer )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapConnectTimeEvent.begin(); mit != m_mapConnectTimeEvent.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kConnectEventInfo = mit->second.first;
		const KTimerManager& kEventTimer = mit->second.second;

		// 1. �̹� ������ �޾Ҵ��� �˻�
		if( tCurrentTime.GetTime() <= kConnectEventInfo.m_kCTime )
			continue;

		// 2. END_EVENT���� �̹� '�������� �̺�Ʈ'��.
		if( kConnectEventInfo.m_fEventTime <= END_EVENT )
			continue;

		// 3. ���� ���� ������ �̺�Ʈ Ÿ�̸� ���
		KUserConnectTimeEventInfo kInfo;
		kInfo = kConnectEventInfo;
		kInfo.m_fBackUpTime = kEventTimer.elapsed();
		mapEventTimer.insert( std::make_pair( mit->first, kInfo ) );
	}
}

void KUserEventManager::SetEventTimerInfo( IN const std::map< int, KUserConnectTimeEventInfo >& mapEventTimer )
{
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitFind;
	std::map< int, KUserConnectTimeEventInfo >::const_iterator mit;
	for( mit = mapEventTimer.begin(); mit != mapEventTimer.end(); ++mit )
	{
		mitFind = m_mapConnectTimeEvent.find( mit->first );
		if( mitFind == m_mapConnectTimeEvent.end() )
		{
			// �̺�Ʈ�� �������� �ƴϸ� ���� ä�ο��� ���� �޾ƿ°� ����!
			std::pair< KUserConnectTimeEventInfo, KTimerManager > pairTimeEvent;
			pairTimeEvent.first = mit->second;
			pairTimeEvent.second.SetAddTime( mit->second.m_fBackUpTime );

			m_mapConnectTimeEvent.insert( std::make_pair( mit->first, pairTimeEvent ) );
		}
		else
		{
			// �������� �̺�Ʈ ������ �ִٸ� �ð��� ���� ����!
			KTimerManager& kEventTimer = mitFind->second.second;

			kEventTimer.SetAddTime( mit->second.m_fBackUpTime );
		}
	}
}

#else
#pragma ELSE_MESSAGE

void KUserEventManager::GetEventTimerInfo( OUT std::map< int, double >& mapEventTimer )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapConnectTimeEvent.begin(); mit != m_mapConnectTimeEvent.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kConnectEventInfo = mit->second.first;
		const KTimerManager& kEventTimer = mit->second.second;

		// 1. �̹� ������ �޾Ҵ��� �˻�
		if( tCurrentTime.GetTime() <= kConnectEventInfo.m_kCTime )
			continue;

		// 2. END_EVENT���� �̹� '�������� �̺�Ʈ'��.
		if( kConnectEventInfo.m_fEventTime <= END_EVENT )
			continue;

		// 3. ���� ���� ������ �̺�Ʈ Ÿ�̸� ���
		mapEventTimer.insert( std::make_pair( mit->first, kEventTimer.elapsed() ) );
	}
}

void KUserEventManager::SetEventTimerInfo( IN const std::map< int, double >& mapEventTimer )
{
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitFind;
	std::map< int, double >::const_iterator mit;
	for( mit = mapEventTimer.begin(); mit != mapEventTimer.end(); ++mit )
	{
		mitFind = m_mapConnectTimeEvent.find( mit->first );
		if( mitFind == m_mapConnectTimeEvent.end() )
			continue;

		KTimerManager& kEventTimer = mitFind->second.second;

		// ä���̵����� ���� �̺�Ʈ Ÿ�̸� ����
		kEventTimer.SetAddTime( mit->second );
	}
}

#endif SERV_TIME_EVENT_BUG_FIX
//}}

//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
void KUserEventManager::CheckCumulativeTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor )
#else
void KUserEventManager::CheckCumulativeTimeEvent( IN KGSUserPtr spUser )
#endif SERV_2012_WINTER_VACATION_EVENT
//}}
{
	if( spUser == NULL )
		return;

	KDBE_UPDATE_EVENT_TIME_REQ kPacketReq;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotEnd;

	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	kFactor.m_iWinterVacationEventCount = spUser->GetWinterVacationEventCount();
	kFactor.m_pSetCompletedEvent = &m_setCompletedEvent;
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

	// �̺�Ʈ ���� ���� �ð��� �� �Ǿ����� üũ, ����Ǵ� �̺�Ʈ üũ, ���� �����ϴ� �̺�Ʈ üũ
	SiKGameEventManager()->CheckCumulativeTimeEvent( spUser->GetLevel(),
													 spUser->IsPcBang(), 
													 //{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													 spUser->GetChannelCode(),
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													 //}}
													 //{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
													 kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
													 //}}
													 m_mapCumulativeTimeEvent, 
													 m_setCompletedEvent, 
													 m_setDirectRewardEvent,
													 kPacketReq.m_vecCumulativeTimeEvent, 
													 kPacketNotBegin.m_vecTimeEvent, 
													 kPacketNotEnd.m_vecTimeEvent );

	// ���ӽð��� �������Ѽ� ������� �̺�Ʈ�� �ִ���?
	if( !kPacketReq.m_vecCumulativeTimeEvent.empty() )
	{
		// ���� �ð� �̺�Ʈ ���� ������ ����!
		kPacketReq.m_iUnitUID = spUser->GetCharUID();
#ifdef SERV_ADD_EVENT_DB
		spUser->SendToEventDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#else //SERV_ADD_EVENT_DB
		spUser->SendToGameDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#endif //SERV_ADD_EVENT_DB
	}

	// ���� ���۵Ǵ� �̺�Ʈ�� �ִ���?
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotBegin );
	}

	// ����� �̺�Ʈ�� �ִ���?
	if( !kPacketNotEnd.m_vecTimeEvent.empty() )
	{
		kPacketNotEnd.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_END;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotEnd );
	}
}

void KUserEventManager::GetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecEventTime )
{
	vecEventTime.clear();

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapCumulativeTimeEvent.begin(); mit != m_mapCumulativeTimeEvent.end(); ++mit )
	{
		// �ʱ�ȭ�� ���� ���� �̺�Ʈ�� �̹� ����� �̺�Ʈ�̹Ƿ� DB�� ������Ʈ �� �ʿ䰡 ����.
		if( mit->second.first.m_bInit == false )
			continue;

		// �Ϸ��� �̺�Ʈ�� DB�� ������Ʈ�� �ʿ䰡 ����.
		if( m_setCompletedEvent.find( mit->first ) != m_setCompletedEvent.end() )
			continue;

		const KTimerManager& kEventTimer = mit->second.second;

		KCumulativeTimeEventInfo kInfo;
		kInfo.m_iEventUID = mit->first;
		kInfo.m_iCumulativeTime = static_cast<int>( kEventTimer.elapsed() / 60.0 );
		//{{ 2010. 06. 15  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		kInfo.m_bAccountEvent = mit->second.first.m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
		//}}
		vecEventTime.push_back( kInfo );
	}
}

void KUserEventManager::DeleteDirectRewardAndGetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecCompleteEvent )
{
	vecCompleteEvent.clear();

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::iterator mit;
	std::set< int >::const_iterator sit;
	for( sit = m_setDirectRewardEvent.begin(); sit != m_setDirectRewardEvent.end(); ++sit )
	{
		mit = m_mapCumulativeTimeEvent.find( *sit );
		if( mit == m_mapCumulativeTimeEvent.end() )
			continue;

		KUserCumulativeTimeEventInfo& kCumulativeEventInfo = mit->second.first;

		// DB�� ������Ʈ�� ���� ����
		KCumulativeTimeEventInfo kInfo;
		kInfo.m_iEventUID = kCumulativeEventInfo.m_iEventUID;
		kInfo.m_iCumulativeTime = static_cast<int>(kCumulativeEventInfo.m_fEventTime);
		vecCompleteEvent.push_back( kInfo );

		// �������� �̺�Ʈ���� ����
		m_mapCumulativeTimeEvent.erase( mit );

		// �Ϸ� �̺�Ʈ�� �߰�
		m_setCompletedEvent.insert( *sit );
	}

	// DirectReward �ʱ�ȭ
	m_setDirectRewardEvent.clear();
}

void KUserEventManager::RestartDirectRewardEvent( IN KGSUserPtr spUser, IN const std::vector< int >& vecRestartEvent )
{
	if( spUser == NULL )
		return;

	std::vector< int >::const_iterator vit;
	for( vit = vecRestartEvent.begin(); vit != vecRestartEvent.end(); ++vit )
	{
		// �Ϸ� �̺�Ʈ���� ���� �Ѵ�.
		m_setCompletedEvent.erase( *vit );
	}

	// �̺�Ʈ�� ������ϱ����� �̺�Ʈ üũ �Լ� ȣ��
	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	KGameEventManager::ConnectEventFactorSet kFactor;
	CheckCumulativeTimeEvent( spUser, kFactor );
#else
	CheckCumulativeTimeEvent( spUser );
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}
}

#endif CUMULATIVE_TIME_EVENT
//}}

//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
void KUserEventManager::GetNowProcessingTimeEvent( OUT std::vector< KTimeEventSimpleInfo >& vecTimeEvent )
{
	vecTimeEvent.clear();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapConnectTimeEvent.begin(); mit != m_mapConnectTimeEvent.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kConnectEventInfo = mit->second.first;

		KTimeEventSimpleInfo kInfo;
		kInfo.m_iEventUID	  = mit->first;
		kInfo.m_iEventID	  = kConnectEventInfo.m_iEventID;
		kInfo.m_wstrEventName = kConnectEventInfo.m_wstrEventName;
		kInfo.m_fEventTime	  = kConnectEventInfo.m_fEventTime;
		vecTimeEvent.push_back( kInfo );
	}

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::const_iterator mitCT;
	for( mitCT = m_mapCumulativeTimeEvent.begin(); mitCT != m_mapCumulativeTimeEvent.end(); ++mitCT )
	{
		// �Ϸ��� �̺�Ʈ�� Ŭ���̾�Ʈ�� ���ʿ� ����.
		if( m_setCompletedEvent.find( mitCT->first ) != m_setCompletedEvent.end() )
			continue;

		const KUserCumulativeTimeEventInfo& kCumulativeEventInfo = mitCT->second.first;
		const KTimerManager& kEventTimer = mitCT->second.second;

		// �ʱ�ȭ ���� ���� �̺�Ʈ�� Ŭ���̾�Ʈ�� ���ʿ� ����.
		if( !kCumulativeEventInfo.m_bInit )
			continue;

		KTimeEventSimpleInfo kInfo;
		kInfo.m_iEventUID	  = mitCT->first;
		kInfo.m_iEventID	  = kCumulativeEventInfo.m_iEventID;
		kInfo.m_wstrEventName = kCumulativeEventInfo.m_wstrEventName;
		kInfo.m_fEventTime	  = kCumulativeEventInfo.m_fEventTime - static_cast<float>( kEventTimer.elapsed() / 60.0 );
		if( kInfo.m_fEventTime < 0.0f )
			kInfo.m_fEventTime = 0.0f;
		vecTimeEvent.push_back( kInfo );
	}
}
//}}

//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT

void KUserEventManager::CheckWebPointEventQuest( IN int iQuestID, IN KGSUserPtr spUser )
{
	switch( iQuestID )
	{
	case 8650: // ���ϸ��� ����Ʈ ����!(2�� 25��)
	case 8660: // ���ϸ��� ����Ʈ ����!(2�� 26��)
	case 8670: // ���ϸ��� ����Ʈ ����!(2�� 27��)
	case 8680: // ���ϸ��� ����Ʈ ����!(2�� 28��)
	case 8690: // ���ϸ��� ����Ʈ ����!(3�� 1��)
	case 8700: // ���ϸ��� ����Ʈ ����!(3�� 2��)
	case 8710: // ���ϸ��� ����Ʈ ����!(3�� 3��)
	case 8720: // ���ϸ��� ����Ʈ ����!(3�� 4��)
	case 8730: // ���ϸ��� ����Ʈ ����!(3�� 5��)
	case 8740: // ���ϸ��� ����Ʈ ����!(3�� 6��)
	case 8750: // ���ϸ��� ����Ʈ ����!(3�� 7��)
	case 8760: // ���ϸ��� ����Ʈ ����!(3�� 8��)
	case 8770: // ���ϸ��� ����Ʈ ����!(3�� 9��)
	case 8780: // ���ϸ��� ����Ʈ ����!(3�� 10��)
	case 8790: // ���ϸ��� ����Ʈ ����!(3�� 11��)
	case 8800: // ���ϸ��� ����Ʈ ����!(3�� 12��)
	case 8810: // ���ϸ��� ����Ʈ ����!(3�� 13��)
	case 8820: // ���ϸ��� ����Ʈ ����!(3�� 14��)
	case 8830: // ���ϸ��� ����Ʈ ����!(3�� 15��)
	case 8840: // ���ϸ��� ����Ʈ ����!(3�� 16��)
	case 8850: // ���ϸ��� ����Ʈ ����!(3�� 17��)
		break;

	default:
		return;
	}
    
	// �� ����Ʈ�� �ø��� ����!
	KDBE_INCREASE_WEB_POINT_REQ kPacket;
	kPacket.m_iUnitUID = spUser->GetCharUID();
	kPacket.m_wstrUserID = spUser->GetName();
	kPacket.m_cPointType = 2; // 2 : ����Ʈ
	kPacket.m_iIncreasePoint = 20;
	spUser->SendToNXWebDB( DBE_INCREASE_WEB_POINT_REQ, kPacket );

	// Ŭ���̾�Ʈ�� �˾� ����
	spUser->SendID( EGS_GET_WEB_POINT_NOT );
}

bool KUserEventManager::CheckAttendance( IN KGSUserPtr spUser )
{
	// �⼮üũ�� ������ �ִ°�?
	if( m_bAttendanceCheck == true )
	{
		CTime tCurTime = CTime::GetCurrentTime();

		// ������ �⼮ üũ�ߴ� ��¥�� ���� ��¥�� ���ٸ� DB�� �⼮üũ�Ϸ� �Ȱ���.
		if( tCurTime.GetDay() == m_tLastDBCheckAttendanceTime.GetDay() )		
			return false;
	}
    
	// DB�� �⼮üũ �Ϸ�����!
	KDBE_ATTENDANCE_CHECK_REQ kPacket;
	kPacket.m_iUnitUID = spUser->GetCharUID();
	spUser->SendToGameDB( DBE_ATTENDANCE_CHECK_REQ, kPacket );
	return true;
}

void KUserEventManager::CheckAttendanceSuccess( IN KGSUserPtr spUser, IN bool bWebPointReward )
{
	// �⼮üũ ���� ��
	m_bAttendanceCheck = true;

	// �⼮üũ�� ������ ���� �ð��� ����
	m_tLastDBCheckAttendanceTime = CTime::GetCurrentTime();

	if( bWebPointReward )
	{
		// �� ����Ʈ�� �ø��� ����!
		KDBE_INCREASE_WEB_POINT_REQ kPacket;
		kPacket.m_iUnitUID = spUser->GetCharUID();
		kPacket.m_wstrUserID = spUser->GetName();
		kPacket.m_cPointType = 1; // 1 : �⼮üũ
		kPacket.m_iIncreasePoint = 10;
		spUser->SendToNXWebDB( DBE_INCREASE_WEB_POINT_REQ, kPacket );
	}
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
void KUserEventManager::CheckAdvertisementEvent( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	KEGS_ADVERTISEMENT_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_ADVERTISEMENT_EVENT_INFO_NOT kPacketNotEnd;
	KDBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT kPacketDB;

	SiKGameEventManager()->CheckEnableAdvertisementEvent( m_setCompletedEvent, kPacketNotBegin.m_vecTimeEvent, kPacketNotEnd.m_vecTimeEvent );

	// ���� ���۵Ǵ� �̺�Ʈ�� �ִٸ� Ŭ���̾�Ʈ�� �˸���
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_ADVERTISEMENT_EVENT_INFO_NOT, kPacketNotBegin );
		
		for( u_int ui = 0; ui < kPacketNotBegin.m_vecTimeEvent.size(); ++ui )
		{
			m_setCompletedEvent.insert( kPacketNotBegin.m_vecTimeEvent[ui].m_iEventUID );
			kPacketDB.m_vecCompletedEvent.push_back( kPacketNotBegin.m_vecTimeEvent[ui].m_iEventUID );
		}

		kPacketDB.m_iUserUID = spUser->GetUID();
#ifdef SERV_ADD_EVENT_DB
				spUser->SendToEventDB( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT, kPacketDB );
#else //SERV_ADD_EVENT_DB
				spUser->SendToGameDB( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT, kPacketDB );
#endif //SERV_ADD_EVENT_DB
	}
}

void KUserEventManager::GetAdvertisementURL( OUT std::vector< std::wstring >& vecAdvertisementURL )
{
	SiKGameEventManager()->GetAdvertisementURL( vecAdvertisementURL );
}
#endif SERV_ADVERTISEMENT_EVENT