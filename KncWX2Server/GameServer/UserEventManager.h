#pragma once
#include "Event.h"
#include "CommonPacket.h"
//#include <boost/timer.hpp>
#include "TimerManager.h"
//#include "GameEvent\GameEventConnectTime.h"
//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
//#ifdef CUMULATIVE_TIME_EVENT
//	#include "GameEvent\GameEventCumulativeTime.h"
//#endif CUMULATIVE_TIME_EVENT
//}}

//{{ 2012. 10. 13	�ڼ���	���� �̺�Ʈ ���� ����
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
	#include "GameEvent\GameEventManager.h"
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}


SmartPointer(KGSUser);

class KUserEventManager
{
public:

	enum USER_EVENT_ENUM
	{
		EVENT_TIME_CONVERT = 60,
	};

	KUserEventManager(void);
	~KUserEventManager(void);

	void		Clear();

	//////////////////////////////////////////////////////////////////////////	
	// ���� �ð� �̺�Ʈ

	//{{ 2012. 10. 13	�ڼ���	���� �̺�Ʈ ���� ����
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
	void		InitTimeEvent( IN KGameEventManager::ConnectEventFactorSet& kFactor );
	void		CheckConnectTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor );
#else
	void		InitTimeEvent( IN const u_char ucLevel,
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
							   );
	
	//////////////////////////////////////////////////////////////////////////	
	// ���� �ð� �̺�Ʈ
		
	void		CheckConnectTimeEvent( IN KGSUserPtr spUser
		//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		, IN const UidType& iUnitUID
		, IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		, IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		);
#endif SERV_CONNECT_EVENT_FACTOR_SET
	//}}
	
	//{{ 2010. 7. 27  ������	���� �ð� �̺�Ʈ ���� ����
#ifdef SERV_TIME_EVENT_BUG_FIX
	void		GetEventTimerInfo( OUT std::map< int, KUserConnectTimeEventInfo >& mapEventTimer );			// �̺�Ʈ Ÿ�̸� ���� ���
	void		SetEventTimerInfo( IN const std::map< int, KUserConnectTimeEventInfo >& mapEventTimer );	// �̺�Ʈ Ÿ�̸� ���� ����
#else
#pragma ELSE_MESSAGE
	void		GetEventTimerInfo( OUT std::map< int, double >& mapEventTimer );		// �̺�Ʈ Ÿ�̸� ���� ���
	void		SetEventTimerInfo( IN const std::map< int, double >& mapEventTimer );	// �̺�Ʈ Ÿ�̸� ���� ����
#endif SERV_TIME_EVENT_BUG_FIX
	//}}	

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	void		CheckCumulativeTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor );
#else
	void		CheckCumulativeTimeEvent( IN KGSUserPtr spUser );
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}

	void		GetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecEventTime );

	//{{ 2009. 12. 8  ������	ũ���������̺�Ʈ
	bool		IsCompleteDirectRewardEvent() { return !m_setDirectRewardEvent.empty(); }
	void		DeleteDirectRewardAndGetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecCompleteEvent );
	void		RestartDirectRewardEvent( IN KGSUserPtr spUser, IN const std::vector< int >& vecRestartEvent );
	//}}

#endif CUMULATIVE_TIME_EVENT
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ���� �Լ�
	//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
	void		GetNowProcessingTimeEvent( OUT std::vector< KTimeEventSimpleInfo >& vecTimeEvent );
	//}}	

	//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	void		CheckWebPointEventQuest( IN int iQuestID, IN KGSUserPtr spUser );
	bool		CheckAttendance( IN KGSUserPtr spUser );
	void		CheckAttendanceSuccess( IN KGSUserPtr spUser, IN bool bWebPointReward );
#endif SERV_WEB_POINT_EVENT
	//}}

#ifdef SERV_ADVERTISEMENT_EVENT
	void		CheckAdvertisementEvent( IN KGSUserPtr spUser );
	void		GetAdvertisementURL( OUT std::vector< std::wstring >& vecAdvertisementURL );
#endif SERV_ADVERTISEMENT_EVENT
private:
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >		m_mapConnectTimeEvent;		// ���� �������� ���ӽð� �̺�Ʈ
	//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >	m_mapCumulativeTimeEvent;	// ���� �������� �����ð� �̺�Ʈ	
	std::set< int >									m_setCompletedEvent;		// �Ϸ��� �̺�Ʈ
	std::set< int >									m_setDirectRewardEvent;		// ���� ���� �޾ƾ��ϴ� �̺�Ʈ
#endif CUMULATIVE_TIME_EVENT
	//}}	

	//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	bool											m_bAttendanceCheck;
	CTime											m_tLastDBCheckAttendanceTime;
#endif SERV_WEB_POINT_EVENT
	//}}	
};

