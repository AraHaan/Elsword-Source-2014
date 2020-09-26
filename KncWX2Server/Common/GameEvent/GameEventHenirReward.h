#pragma once

#include "GameEvent.h"

//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST

class KGameEventHenirReward : public KGameEvent
{
public:
	KGameEventHenirReward(void);
	virtual ~KGameEventHenirReward(void);

	void	SetEventID( int iEventID )						{ m_iEventID = iEventID; }
	int		GetEventID()									{ return m_iEventID; }
	
	void	SetHenirRewardEventCount( int iEventCount )		{ m_iHenirRewardEventCount = iEventCount; }
	int		GetHenirRewardEventCount()						{ return m_iHenirRewardEventCount; }

	void	SetHenirRewardUnLimited( bool bUnLimited )		{ m_bHenirRewardUnLimited = bUnLimited; }
	bool	GetHenirRewardUnLimited()						{ return m_bHenirRewardUnLimited; }

	//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	void	SetServerGroupID( int iServerGroupID )			{ m_iServerGroupID = iServerGroupID; }
	int		GetServerGroupID() const						{ return m_iServerGroupID; }
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}

#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
	void	SetPcBangEvent( IN const bool bPcBangEvent ) 	{	m_bPcBangEvent = bPcBangEvent;	}
	bool	GetPcBangEvent( void ) const					{	return m_bPcBangEvent;	}

	void	SetAccumulate( IN const bool bAccumulate )		{	m_bAccumulate = bAccumulate;	}
	bool	GetAccumulate( void ) const						{	return m_bAccumulate;	}
#endif // SERV_HENIR_REWARD_EVENT

private:

	int						m_iEventID;

	int						m_iHenirRewardEventCount;		//  event �߰� ���� Ƚ��
	bool					m_bHenirRewardUnLimited;		//  ������
	//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	int						m_iServerGroupID;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}

#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
	bool	m_bPcBangEvent;
	bool	m_bAccumulate;
#endif // SERV_HENIR_REWARD_EVENT
};

#endif SERV_NEW_HENIR_TEST
//}}
