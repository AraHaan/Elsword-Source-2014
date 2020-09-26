#include ".\gameeventconnecttime.h"
#include "Enum\Enum.h"

KGameEventConnectTime::KGameEventConnectTime(void) : 
m_fEventTime( 0.0f ),
m_iEventID( 0 ),
m_iRewardID( 0 ),
m_bGiveOnce( true ),
m_bDelEndEvent( false ),
//{{ 2009. 10. 26  ������	PC���̺�Ʈ
m_bPcBangEvent( false ),
//}}
//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
m_bAccountEvent( false ),
//}}
//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
m_iChannelCode( -1 ),
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
//}}
//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
m_iBF_Team( 0 ),
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
//}}
//{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
m_cUnitType( 0 ),
m_cUnitClass( 0 ),
#endif SERV_CHAR_CONNECT_EVENT
//}}
//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
m_bRepeatEvent( false ),
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
//}}
//{{ 2009. 12. 16  ������	�̺�Ʈ����
m_iUnitLevel( 0 )
//}}
//{{ 2012. 12. 25	�ڼ���	Ư�� ���� ���� ���� �̺�Ʈ
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
, m_iEventGroupID( -1 )
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
, m_bRepeatRewardEvent(false)
, m_iBeforeEventID(-1)
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
, m_iEventItemID(0)
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
,m_iCustomEventID( 0 )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT// ���볯¥: 2013-04-11
, m_byteCriterionUserType( 0 )
, m_byteCriterionPresentUserType( 0 )
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
, m_bOnlySteamUserEvent( false )
, m_bOnlyNotSteamUserEvent( false )
#endif SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
, m_bNewUnitEvent( false )
, m_bNewUnitEvent2( false )
, m_bCurrentUnitEvent( false )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
, m_iUnitClassLevel( -1 )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
{
}

KGameEventConnectTime::~KGameEventConnectTime(void)
{
}

//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
int	KGameEventConnectTime::GetMinMaxRewardID( const int iLevel ) const
{
	int iResultRewardID = 0;

	std::map< std::pair< int, int >, int >::const_iterator mit;
	for( mit = m_mapMinMaxReward.begin(); mit != m_mapMinMaxReward.end(); ++mit )
	{
		const int iMinLevel = mit->first.first;
		const int iMaxLevel = mit->first.second;

		if( iMinLevel != 0  &&  iMinLevel > iLevel )
			continue;

		if( iMaxLevel != 0  &&  iMaxLevel < iLevel )
			continue;

		// ã�Ҵ�!
		iResultRewardID = mit->second;
		break;
	}

	return iResultRewardID;
}
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
//}}
