#include "GameEventHenirReward.h"

//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST

KGameEventHenirReward::KGameEventHenirReward(void)
	: m_iHenirRewardEventCount(0)
//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
, m_iServerGroupID(-1)
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
//}}
, m_bHenirRewardUnLimited( false )
#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
, m_bPcBangEvent( false )
, m_bAccumulate( false )
#endif // SERV_HENIR_REWARD_EVENT
{
}

KGameEventHenirReward::~KGameEventHenirReward(void)
{
}

#endif SERV_NEW_HENIR_TEST
//}}
