#pragma once

#include "GameEvent.h"
//#include "TimerManager.h"

//{{ 2009. 10. 26  ������	PC���̺�Ʈ
//struct KUserCumulativeTimeEventInfo
//{
//	int				m_iEventUID;
//	int				m_iEventID;
//	bool			m_bInit;
//	std::wstring	m_wstrEventName;
//	float			m_fEventTime;
//	KTimerManager	m_kEventTimer;
//	bool			m_bDirectReward;
//
//	KUserCumulativeTimeEventInfo()
//	{
//		m_iEventUID		= 0;
//		m_iEventID		= 0;
//		m_bInit			= false;
//		m_fEventTime	= 0.0f;
//		m_bDirectReward = false;
//	}
//};
//}}

class KGameEventCumulativeTime : public KGameEvent
{
public:
	KGameEventCumulativeTime(void);
	virtual ~KGameEventCumulativeTime(void);
	
	void	SetEventTime( float fEventTime )	{ m_fEventTime = fEventTime; } // �̺�Ʈ Ÿ�� ������ �д����Դϴ�.
	float	GetEventTime()						{ return m_fEventTime; }

	void	SetEventID( int iEventID )			{ m_iEventID = iEventID; }
	int		GetEventID()						{ return m_iEventID; }
	
	void	SetRewardID( int iRewardID )		{ m_iRewardID = iRewardID; }
	int		GetRewardID()						{ return m_iRewardID; }

	//{{ 2009. 10. 26  ������	PC���̺�Ʈ
	void	SetPcBangEvent( bool bPcBangEvent )	{ m_bPcBangEvent = bPcBangEvent; }
	bool	IsPcBangEvent()						{ return m_bPcBangEvent; }
	//}}

	//{{ 2009. 12. 8  ������	���ӽð��̺�Ʈ
	void	SetDirectReward( bool bVal )		{ m_bDirectReward = bVal; }
	bool	IsDirectReward()					{ return m_bDirectReward; }
	//}}

	//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
	void	SetAccountEvent( bool bAccountEvent )	{ m_bAccountEvent = bAccountEvent; }
	bool	IsAccountEvent()						{ return m_bAccountEvent; }
	//}}
	//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	void	SetChannelCode( int iChannelCode )		{ m_iChannelCode = iChannelCode; }
	int		GetChannelCode()						{ return m_iChannelCode; }
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//}}

#ifdef SERV_CUSTOM_CONNECT_EVENT
	void	SetCustomEventID( IN const int iCustomEventID ){	m_iCustomEventID = iCustomEventID;	}
	int		GetCustomEventID( void ) const	{ return m_iCustomEventID;	}
#endif SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	void	SetNewUnitEvent( bool bNewUnitEvent )		{ m_bNewUnitEvent = bNewUnitEvent; }	
	bool	IsNewUnitEvent()							{ return m_bNewUnitEvent; }

	void	SetNewUnitEvent2( bool bNewUnitEvent2 )		{ m_bNewUnitEvent2 = bNewUnitEvent2; }	
	bool	IsNewUnitEvent2()							{ return m_bNewUnitEvent2; }

	void	SetCurrentUnitEvent( bool bCurrentUnitEvent )		{ m_bCurrentUnitEvent = bCurrentUnitEvent; }	
	bool	IsCurrentUnitEvent()							{ return m_bCurrentUnitEvent; }
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

private:
	float	m_fEventTime;
	int		m_iEventID;
	int		m_iRewardID;
	//{{ 2009. 10. 26  ������	PC���̺�Ʈ
	bool	m_bPcBangEvent;
	//}}
	//{{ 2009. 12. 8  ������	���ӽð��̺�Ʈ
	bool	m_bDirectReward;
	//}}
	//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
	bool	m_bAccountEvent;
	//}}
	//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	int		m_iChannelCode;
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
	int		m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		bool	m_bNewUnitEvent;
		bool	m_bNewUnitEvent2;
		bool	m_bCurrentUnitEvent;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
};
