#include "GameEventManager.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"
#include "Socket/NetCommon.h"
#include "X2Data/XSLDungeon.h"

//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventDataRefreshManager.h"
	#include "EventDataRefresh/EventNotifyMsgServerInfo.h"
#else
	
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	#include "GameEventDBManager.h"
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

//{{ 2011. 7.29 ��ȿ�� �ݺ� ����Ʈ �� ���� ����Ʈ�� ����Ʈ �̺�Ʈ�� ��ϰ����ϵ��� ����
#ifdef SERV_QUEST_EVENT_EXTEND
#include "../X2Data/XSLQuestManager.h"
#endif SERV_QUEST_EVENT_EXTEND

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	#include "GameEventScriptManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

#endif SERV_EVENT_SCRIPT_REFRESH
//}}

//{{ 2012. 06. 05	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	#include ".\GameEvent\DailyGiftBoxManager.h"
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

ImplementSingleton( KGameEventManager );

KGameEventManager::KGameEventManager()
{
	m_eServerType		 = ST_NONE;
}

KGameEventManager::~KGameEventManager()
{
	for( int idx = 0; idx < KGameEvent::GET_MAX; ++idx )
	{
		std::map< int, KGameEvent* >::iterator mit;
		for( mit = m_mapIngEventData[idx].begin(); mit != m_mapIngEventData[idx].end(); ++mit )
		{
			SAFE_DELETE( mit->second );
		}
		m_mapIngEventData[idx].clear();
	}
}

ImplToStringW( KGameEventManager )
{
	return stm_;
}

void KGameEventManager::Tick()
{
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	TickCheckEvent();
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	TickCheckEvent();
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifndef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	//////////////////////////////////////////////////////////////////////////	
	// �켱 2�и��� �޾ƿ���..
	if( m_TimerRefreshEvent.elapsed() > 120.0 )
	{
#ifdef SERV_ADD_EVENT_DB
		SendToEventDB( DBE_EVENT_UPDATE_REQ );
#else //SERV_ADD_EVENT_DB
		SendToLogDB( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB

		m_TimerRefreshEvent.restart();
	}
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

	//////////////////////////////////////////////////////////////////////////
	// 10�� ���� Enableüũ
	if( m_TimerEnableCheck.elapsed() < 10.0 )
		return;

	m_TimerEnableCheck.restart();

	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

	//�̺�Ʈ�� ���۰� ���Ḧ �����Ŵ..

	CTime tCurrentTime = CTime::GetCurrentTime();

	for( int iGET = 0; iGET < KGameEvent::GET_MAX; ++iGET )
	{
		std::map< int, KGameEvent* >::iterator mit;
		for( mit = m_mapIngEventData[iGET].begin(); mit != m_mapIngEventData[iGET].end(); ++mit )
		{
			if( mit->second == NULL )
				continue;

			//���� ����Ǵ� �̺�Ʈ��..
			if( mit->second->IsEnable() == true )
			{
				//����ð��� ����ð����� ������ ����.
				if( mit->second->GetEndDate() < tCurrentTime
#ifdef SERV_ALLOW_EVENT_ERASE
					|| mit->second->IsOn() == false
#endif SERV_ALLOW_EVENT_ERASE
					)
				{
					mit->second->SetEnable( false );

#ifdef SERV_NEW_EVENT_TYPES
					// ���� �̺�Ʈ�� ���°� ���� �� �� ���� �۾��� �����ϱ� ���� �ڵ�
					++m_iAppliedEventTick;
#endif SERV_NEW_EVENT_TYPES
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
					std::wstring wstrEventBegin = (CStringW)(mit->second->GetBeginDate().Format(_T("%Y-%m-%d %H:%M:%S")));
					std::wstring wstrEventEnd = (CStringW)(mit->second->GetEndDate().Format(_T("%Y-%m-%d %H:%M:%S")));
					SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
						% 2 %mit->second->GetUID() %mit->second->GetEventName() %mit->second->GetType() %mit->second->GetScriptID() %wstrEventBegin %wstrEventEnd %L"�̺�Ʈ ����" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

					START_LOG( cout2, L"�̺�Ʈ ����.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetEventName() )
						<< BUILD_LOG( mit->second->GetType() )
//#ifdef SERV_NEW_EVENT_TYPES
						<< BUILD_LOG( m_iAppliedEventTick )
//#endif SERV_NEW_EVENT_TYPES
						;

					//���Ŀ� ����� �̺�Ʈ ����Ʈ�� ��������.�Ѥ�;
					//vecEraseEvent.push_back( mit );
				}
			}
			//���� ���۵��� ���� �̺�Ʈ��..
			else
			{
				if( mit->second->GetType() == KGameEvent::GET_NOTIFY_MSG  &&  GetServerType() == ST_CENTER )
				{
					KGameEventNotifyMSG* pEvent = static_cast<KGameEventNotifyMSG*>(mit->second);

					if( pEvent == NULL )
						continue;

					if( pEvent->GetType() != KGameEvent::GET_NOTIFY_MSG )
						continue;

					if( pEvent->IsSend() == true )
						continue;

					if( pEvent->GetBeginDate() <= tCurrentTime 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
						&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE
						)
					{
						pEvent->SetIsSend( true );

						//{{ 2008. 7. 11  ������  GM �̺�Ʈ ����
						//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
						if( SiKEventNotifyMsgServerInfo()->IsNotifyMSGEvent() == true )
#else
						if( SiKGameEventScriptManager()->IsNotifyMSGEvent() == true )
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
						//}}
						{
							KEGS_NOTIFY_MSG_NOT kPacket;
							kPacket.m_Count = 1;
							kPacket.m_wstrMSG = pEvent->GetNotifyMSG();

							//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
							if( pEvent->IsAllServers() )
							{
								kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_ALL_SERVERS;

								UidType anTrace[2] = { 0, -1 };
								KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket );
							}
							else
							{
								kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_SERVER_GROUP;

								UidType anTrace[2] = { 0, -1 };
								KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket );
							}
//#else
//							UidType anTrace[2] = { 0, -1 };
//							KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket );
#endif SERV_INTEGRATION
							//}}
						}
						//}}
					}
				}
				else
				{
					//������ ���۵Ǿ����� 
					if( mit->second->GetBeginDate() <= tCurrentTime && mit->second->GetEndDate() >= tCurrentTime
#ifdef SERV_ALLOW_EVENT_ERASE
						&& mit->second->IsOn() == true
#endif SERV_ALLOW_EVENT_ERASE
						)
					{
						mit->second->SetEnable( true );

#ifdef SERV_NEW_EVENT_TYPES
						// ���� �̺�Ʈ�� ���°� ���� �� �� ���� �۾��� �����ϱ� ���� �ڵ�
						++m_iAppliedEventTick;
#endif SERV_NEW_EVENT_TYPES
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
						std::wstring wstrEventBegin = (CStringW)(mit->second->GetBeginDate().Format(_T("%Y-%m-%d %H:%M:%S")));
						std::wstring wstrEventEnd = (CStringW)(mit->second->GetEndDate().Format(_T("%Y-%m-%d %H:%M:%S")));
						SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
							% 2 %mit->second->GetUID() %mit->second->GetEventName() %mit->second->GetType() %mit->second->GetScriptID() %wstrEventBegin %wstrEventEnd %L"�̺�Ʈ ����" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
						START_LOG( cout2, L"�̺�Ʈ ����.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetEventName() )
							<< BUILD_LOG( mit->second->GetType() )
//#ifdef SERV_NEW_EVENT_TYPES
							<< BUILD_LOG( m_iAppliedEventTick )
//#endif SERV_NEW_EVENT_TYPES
							;
					}
				}
			}
		}
	}

	//for( int i = 0; i < (int)vecEraseEvent.size(); ++i )
	//{
	//	if( vecEraseEvent[i]->second != NULL )
	//		delete vecEraseEvent[i]->second;

	//	m_mapIngEventData.erase( vecEraseEvent[i] );
	//}
	//vecEraseEvent.clear();

	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

void KGameEventManager::Init()
{
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	InitEventReleaseTick();
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	InitEventDBScriptReleaseTick();
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	// �̺�Ʈ ���� ���
#ifdef SERV_ADD_EVENT_DB
	// �̺�Ʈ ���� ���� ��� ����
	SendToEventDB( DBE_EVENT_DB_SCRIPT_REQ );
	SendToEventDB( DBE_EVENT_UPDATE_REQ );
#else //SERV_ADD_EVENT_DB
	SendToLogDB( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
}

void KGameEventManager::SetEvent( IN const std::vector< KEventInfo >& vecEventList )
{
#ifdef SERV_ALLOW_EVENT_ERASE
	std::vector< KEventInfo >::const_iterator pNewEvent;
	for( pNewEvent = vecEventList.begin(); pNewEvent != vecEventList.end(); ++pNewEvent )
	{
		for( int iEventType = KGameEvent::GET_QUEST; iEventType < KGameEvent::GET_MAX; ++iEventType )
		{
			std::map< int, KGameEvent* >::iterator mitIng = m_mapIngEventData[iEventType].find( pNewEvent->m_iEventUID );

			// ���ư��� �ִ� �̺�Ʈ �߿��� �̹��� ���� �̺�Ʈ�� ã�Ҵ�.
			if( mitIng != m_mapIngEventData[iEventType].end() )
			{
				KGameEvent* pIngEvent = mitIng->second;

				// ���� �ִ��� off�� �Ǹ鼭 ���� ���
				if( pIngEvent->IsEnable() == true && pNewEvent->m_bOn == false )
				{
					// ���� �ڵ�. ���� �̺�Ʈ�� ���ݴϴ�.
					if ( iEventType == KGameEvent::GET_CONNECT_TIME )
					{
						KGameEventConnectTime* pEvent = static_cast<KGameEventConnectTime*>( pIngEvent );
						if ( pEvent != NULL )
							pEvent->SetDelEndEvent(true);
					
					}

					pIngEvent->SetOn(false); // 2013.01.22 lygan_������ // DB���� �̺�Ʈ�� ���� ���� ���ư��� �ִ� �̺�Ʈ�� ������ ���ش�
				}
			}
		}
	}
#endif SERV_ALLOW_EVENT_ERASE

	BOOST_TEST_FOREACH( const KEventInfo&, kEventInfo, vecEventList )
	{
		switch( kEventInfo.m_iEventType )
		{
		case KGameEvent::GET_QUEST:
			AddQuestEvent( kEventInfo );
			break;

			//{{ 2009. 5. 22  ������	��ӷ��̺�Ʈ
		case KGameEvent::GET_ITEM_DROP:
			AddItemDropEvent( kEventInfo );
			break;
			//}}

		case KGameEvent::GET_REWARD:
			AddRewardEvent( kEventInfo );
			break;

			//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER
		case KGameEvent::GET_MONSTER:
			AddMonsterEvent( kEventInfo );
			break;
#endif SERV_EVENT_MONSTER
			//}}

		case KGameEvent::GET_TOURNAMENT:
			AddTournamentEvent( kEventInfo );
			break;

		case KGameEvent::GET_SPIRIT:
			AddSpiritEvent( kEventInfo );
			break;
			
		case KGameEvent::GET_CONNECT_TIME:
			AddConnectTimeEvent( kEventInfo );
			break;

		case KGameEvent::GET_SECRET_DUNGEON:
			AddSecretDungeonEvent( kEventInfo );
			break;

		case KGameEvent::GET_NOTIFY_MSG:
			AddNotifyMSGEvent( kEventInfo );
			break;

			//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
		case KGameEvent::GET_CUMULATIVE_TIME:
			AddCumulativeTimeEvent( kEventInfo );
			break;
#endif CUMULATIVE_TIME_EVENT
			//}}
			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		case KGameEvent::GET_HENIR_REWARD:
			AddHenirRewardCountEvent( kEventInfo );
			break;
#endif SERV_NEW_HENIR_TEST
			//}}
			
#ifdef SERV_ADVERTISEMENT_EVENT
		case KGameEvent::GET_ADVERTISEMENT_EVENT:
			AddAdvertisementEvent( kEventInfo );
			break;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_NEW_EVENT_TYPES
		case KGameEvent::GET_DUNGEON:
			AddDungeonEvent( kEventInfo );
			break;
		case KGameEvent::GET_MAX_LEVEL:
			AddMaxLevelEvent( kEventInfo );
			break;
		case KGameEvent::GET_CONTENTS:
			AddContents( kEventInfo );
			break;
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
		case KGameEvent::GET_CODE:
			AddCodeEvent( kEventInfo );
			break;
#endif SERV_CODE_EVENT			
			
			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
		case KGameEvent::GET_QUEST_ITEM_DROP:
			AddQuestItemDropEvent( kEventInfo );
			break;
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
		case KGameEvent::GET_DEFENSE_DUNGEON:
			AddDefenseDungeonOpenRateEvent( kEventInfo );
			break;
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef SERV_LEVEL_UP_EVENT// �۾���¥: 2013-06-24	// �ڼ���
		case KGameEvent::GET_LEVEL_UP:
			AddLevelUpEvent( kEventInfo );
			break;
#endif // SERV_LEVEL_UP_EVENT

#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-09	// �ڼ���
		case KGameEvent::GET_JUMPING_CHARACTER:
			AddJumpingCharacterEvent( kEventInfo );
			break;
#endif // SERV_JUMPING_CHARACTER

		default:
			{
				START_LOG( cerr, L"���� �̺�Ʈ Ÿ���̰ų� �̱��� �̺�Ʈ �����.!" )
					<< BUILD_LOG( kEventInfo.m_iEventUID )
					<< BUILD_LOG( kEventInfo.m_iEventType )
					<< BUILD_LOG( kEventInfo.m_wstrEventName )
					<< BUILD_LOG( kEventInfo.m_wstrBeginDate )
					<< BUILD_LOG( kEventInfo.m_wstrEndDate )
					<< BUILD_LOG( kEventInfo.m_iScriptID )
					<< BUILD_LOG( (int)kEventInfo.m_bEnable )
					<< END_LOG;
			}
		}
	}
}

//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

//{{ 2012. 12. 17	������	�̺�Ʈ ���ʽ� �ڵ� �����丵
void KGameEventManager::GetRewardEvent( IN OUT KRoomUserInfo& kRoomUserInfo
									  , IN const int iDungeonID
									  , IN const int iUnitLevel
									  , IN const char cUnitClass
									  , IN const int iPartyNum
#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
									  , IN const bool isPcBang
#endif // SERV_PCBANG_USER_REWARD_EVENT

#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP	// �۾���¥: 2014-01-08 // ����ö // Ư�� ĳ���Ϳ� �÷��̽� ����ġ ���� �̺�Ʈ�� ���� �� �� �ٸ� ����ġ ������ ������� �ʴ� ���� ����
									  , IN const bool bHasPlayWithSpecificCharacterBuff_
#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

									  )
{
	SGameEventBonusRate kBonusRateInfo;

	GetRewardEvent( kBonusRateInfo
		, iDungeonID
		, iUnitLevel
		, cUnitClass
		, iPartyNum
#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
		, isPcBang
#endif // SERV_PCBANG_USER_REWARD_EVENT
		);

	if( kBonusRateInfo.m_fEXPRate > 0.0f )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_EXP, kBonusRateInfo.m_fEXPRate );
	}
	if( kBonusRateInfo.m_fVPRate > 0.0f )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_VP, kBonusRateInfo.m_fVPRate );
	}
	if( kBonusRateInfo.m_fEDRate > 0.0f )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_ED, kBonusRateInfo.m_fEDRate );
	}
	if( kBonusRateInfo.m_fPartyEXPRate > 0.0f )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_PARTY_EXP, kBonusRateInfo.m_fPartyEXPRate );
	}
	if( kBonusRateInfo.m_fPartyVPRate > 0.0f )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_PARTY_VP, kBonusRateInfo.m_fPartyVPRate );
	}
	if( kBonusRateInfo.m_fPartyEDRate > 0.0f )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_PARTY_ED, kBonusRateInfo.m_fPartyEDRate );
	}
	//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
	// �۾���¥: 2014-01-08 // ����ö // Ư�� ĳ���Ϳ� �÷��̽� ����ġ ���� �̺�Ʈ�� ���� �� �� �ٸ� ����ġ ������ ������� �ʴ� ���� ����
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
 	if( kBonusRateInfo.m_fWithCharEXPRate > 0.0f && bHasPlayWithSpecificCharacterBuff_ )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_WITH_CHAR_EXP, kBonusRateInfo.m_fWithCharEXPRate );
	}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
	if( 0.0f < kBonusRateInfo.m_fRankingPointRate )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_RANKING_POINT, kBonusRateInfo.m_fRankingPointRate );
	}

	if( 0.0f < kBonusRateInfo.m_fPartyRankingPointRate )
	{
		kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_PARTY_RANKING_POINT, kBonusRateInfo.m_fPartyRankingPointRate );
	}
#endif	// SERV_RANKING_POINT_EVENT
}
//}}

#endif SERV_INTEGRATION
//}}

//{{ 2012. 12. 17	������	�̺�Ʈ ���ʽ� �ڵ� �����丵
void KGameEventManager::GetRewardEvent( OUT SGameEventBonusRate& kBonusRateInfo
									  , IN const int iDungeonID
									  , IN const int iUnitLevel
									  , IN const char cUnitClass
									  , IN const int iPartyNum
#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
									  , IN const bool isPcBang
#endif // SERV_PCBANG_USER_REWARD_EVENT
									  )
{
	kBonusRateInfo.Clear();
	const char cUnitType = static_cast<char>( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(cUnitClass) ) );

	//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
	float fNonAccumulateEXPRate_Solo = 0.f;
	float fNonAccumulateEXPRate_party = 0.f;
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
	//}}

	//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( GetServerType() != ST_GAME )
		return;
#else
	if( GetServerType() != ST_CENTER )
		return;
#endif SERV_INTEGRATION
	//}}

	//�̺�Ʈ ����ð� üũ�� ���� ���� �ð��� �޾ƿ´�.
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, KGameEvent* >::iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_REWARD].begin(); mit != m_mapIngEventData[KGameEvent::GET_REWARD].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		KGameEventReward* pEvent = static_cast<KGameEventReward*>(mit->second);

		//#1. ���� �̺�Ʈ���� üũ..
		if( pEvent->GetType() != KGameEvent::GET_REWARD )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
		if ( pEvent->IsOn() == false )
			continue;
#endif //SERV_ALLOW_EVENT_ERASE	

		//#2. ����� �̺�Ʈ���� üũ
		if( pEvent->IsEnable() == false )
			continue;

		//#3. ���ַ��� �˻�(�̰� �ִ����� �⺻���� 0�̻��̱⶧���� ������ �˻��ص� �������.)
		if( pEvent->GetUnitLevel() != 0  &&  pEvent->GetUnitLevel() < iUnitLevel )
			continue;

		//#4. �������̵�˻�(0�� �ƴ� ������ ���� �������� üũ)
		if( pEvent->GetDungeonID() != 0  &&  pEvent->GetDungeonID() != iDungeonID )
			continue;

		//#5. ����Ÿ�� �˻�
		if( pEvent->GetUnitType() != CXSLUnit::UT_NONE  &&  pEvent->GetUnitType() != cUnitType )
			continue;

		//#6. ����Ŭ���� �˻�
		if( pEvent->GetUnitClass() != CXSLUnit::UC_NONE  &&  pEvent->GetUnitClass() != cUnitClass )
			continue;

		//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		//#7. ������ �˻�

#ifdef EXTEND_SERVER_GROUP_MASK
		if( pEvent->GetServerGroupID() != -1 && pEvent->GetServerGroupID() != KBaseServer::GetKObj()->GetServerGroupID() )
#else EXTEND_SERVER_GROUP_MASK
		if( pEvent->GetServerGroupID() != SEnum::SGI_INVALID  &&  pEvent->GetServerGroupID() != KBaseServer::GetKObj()->GetServerGroupID() )
#endif EXTEND_SERVER_GROUP_MASK
			continue;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}

		//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		if( pEvent->IsPlayWithCharEXP() == true )
		{
			kBonusRateInfo.m_fWithCharEXPRate += pEvent->GetEXPRate();
			continue;
		}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}

#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
		if( ( pEvent->IsPcBangEvent() == true ) && ( isPcBang == false ) )
		{
			continue;
		}
#endif // SERV_PCBANG_USER_REWARD_EVENT
		
		//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
		// ���� �������̾�� ��

		//#Last. ��ø ����		
		if( pEvent->IsAccumulateEXP() == false )
		{
			//#Last-1. ��ø �Ұ��ε� ���� �߰� ����ġ
			if( pEvent->GetPartyNum() == 0 )
			{
				if( fNonAccumulateEXPRate_Solo < pEvent->GetEXPRate() )
				{
					fNonAccumulateEXPRate_Solo = pEvent->GetEXPRate();
					continue;
				}
			}
			//#Last-2. ��ø �Ұ��ε� ��Ƽ �߰� ����ġ
			else if( pEvent->GetPartyNum() <= iPartyNum )
			{
				if( fNonAccumulateEXPRate_party < pEvent->GetEXPRate() )
				{
					fNonAccumulateEXPRate_party = pEvent->GetEXPRate();
					continue;
				}
			}
		}
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
		//}}		

		if( pEvent->GetPartyNum() == 0 )
		{
			//������ Ȯ�������͸� �ִ� ��ŭ �����ش�.
			kBonusRateInfo.m_fEXPRate			+= pEvent->GetEXPRate();
			kBonusRateInfo.m_fVPRate			+= pEvent->GetVPRate();
			kBonusRateInfo.m_fEDRate			+= pEvent->GetEDRate();
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
			kBonusRateInfo.m_fRankingPointRate	+= pEvent->GetRankingPointRate();
#endif	// SERV_RANKING_POINT_EVENT
		}
		else if( pEvent->GetPartyNum() <= iPartyNum )
		{
			//������ Ȯ�������͸� �ִ� ��ŭ �����ش�.
			kBonusRateInfo.m_fPartyEXPRate			+= pEvent->GetEXPRate();
			kBonusRateInfo.m_fPartyVPRate			+= pEvent->GetVPRate();
			kBonusRateInfo.m_fPartyEDRate			+= pEvent->GetEDRate();
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
			kBonusRateInfo.m_fPartyRankingPointRate	+= pEvent->GetRankingPointRate();
#endif	// SERV_RANKING_POINT_EVENT
		}
	}

	//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
	if( kBonusRateInfo.m_fEXPRate < fNonAccumulateEXPRate_Solo )
	{
		kBonusRateInfo.m_fEXPRate = fNonAccumulateEXPRate_Solo;
	}
	if( kBonusRateInfo.m_fPartyEXPRate < fNonAccumulateEXPRate_party )
	{
		kBonusRateInfo.m_fPartyEXPRate = fNonAccumulateEXPRate_party;
	}
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
	//}}
}
//}}

//{{ 2009. 5. 22  ������	��ӷ��̺�Ʈ
#ifdef SERV_PC_BANG_DROP_EVENT
//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
bool KGameEventManager::CheckItemDropProbEvent(IN int iDungeonID, IN std::vector<char> vecUintClass, IN std::vector<char> vecUintType,IN std::vector<int> vecUintLevel, OUT int& iDropCount, OUT bool& bWithPlayPcBang )
#else //DROPEVENT_RENEWAL

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
bool KGameEventManager::CheckItemDropProbEvent( OUT float& fDropRate, OUT bool& bWithPlayPcBang )
#else // SERV_DROP_EVENT_RENEWAL
bool KGameEventManager::CheckItemDropProbEvent( OUT int& iDropCount, OUT bool& bWithPlayPcBang )
#endif // SERV_DROP_EVENT_RENEWAL

#endif //DROPEVENT_RENEWAL
{
	if( GetServerType() != ST_CENTER )
		return false;

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_ITEM_DROP].begin(); mit != m_mapIngEventData[KGameEvent::GET_ITEM_DROP].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_ITEM_DROP )
			continue;

		//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
		KGameEventItemDrop* pEvent = static_cast<KGameEventItemDrop*>(mit->second);

		if( pEvent->GetDungeonID() != 0  &&  pEvent->GetDungeonID() != iDungeonID )
			continue;


		//{{ //2012.03.20 lygan_������ // ������ ���� Ÿ�� üũ
		bool bCheckUintType = false;

		if( pEvent->GetUnitType() != CXSLUnit::UT_NONE)
		{
			for( size_t i = 0; i < vecUintType.size(); i++ )
			{
				if(pEvent->GetUnitType() == vecUintType[i] )
					bCheckUintType = true;
			}

			if(bCheckUintType == false)
				continue;
		}

		//}}

		//{{ //2012.03.20 lygan_������ // ������ ���� Ŭ���� üũ
		bool bCheckUintClass = false;
		if( pEvent->GetUnitClass() != CXSLUnit::UC_NONE )
		{
			for( size_t i = 0; i < vecUintClass.size(); i++ )
			{
				if(pEvent->GetUnitClass() == vecUintClass[i] )
					bCheckUintClass = true;
			}

			if(bCheckUintClass == false)
				continue;
		}
		//}}

		//{{ //2012.03.21 lygan_������ // ������ ���� ���� üũ
		bool bCheckUintLevel = false;
		if(pEvent->GetStartLevel() != 0 && pEvent->GetEndLevel() != 0)
		{
			for( size_t i = 0; i < vecUintLevel.size(); i++ )
			{
				if( vecUintLevel[i] >= pEvent->GetStartLevel() &&  vecUintLevel[i] <= pEvent->GetEndLevel() )
					bCheckUintLevel = true;
			}

			if(bCheckUintLevel == false)
				continue;
		}
		//}}

#else //DROPEVENT_RENEWAL
		KGameEventItemDrop* pEvent = static_cast<KGameEventItemDrop*>(mit->second);
#endif //DROPEVENT_RENEWAL

		// �������� ��ӷ� �̺�Ʈ
		if( mit->second->IsEnable() == true 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.03 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
			//2012.03.21 lygan_������ // ��ӷ� �����Ҷ� ��ӷ��� �ٸ� �̺�Ʈ ������ �Ǹ� ��ӷ��� ���� ���� ��ġ�� ����ǵ��� ����
			if(iDropCount <= pEvent->GetDropCount())
				iDropCount = pEvent->GetDropCount();
#else //DROPEVENT_RENEWAL

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
			fDropRate = pEvent->GetDropRate();
#else // SERV_DROP_EVENT_RENEWAL
			iDropCount = pEvent->GetDropCount();
#endif // SERV_DROP_EVENT_RENEWAL

#endif //DROPEVENT_RENEWAL
			bWithPlayPcBang = pEvent->IsWithPlayPcBang();
			return true;
		}
	}

	return false;
}

#endif SERV_PC_BANG_DROP_EVENT
//}}

//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
bool KGameEventManager::CheckItemDropProbEvent( OUT float& fDropRate )
#else // SERV_DROP_EVENT_RENEWAL
bool KGameEventManager::CheckItemDropProbEvent( OUT int& iDropCount )
#endif // SERV_DROP_EVENT_RENEWAL
{
	if( GetServerType() != ST_CENTER )
		return false;

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_ITEM_DROP].begin(); mit != m_mapIngEventData[KGameEvent::GET_ITEM_DROP].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_ITEM_DROP )
			continue;

		KGameEventItemDrop* pEvent = static_cast<KGameEventItemDrop*>(mit->second);

		// �������� ��ӷ� �̺�Ʈ
		if( mit->second->IsEnable() == true 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE	
			)
		{
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
			fDropRate = pEvent->GetDropRate();
#else // SERV_DROP_EVENT_RENEWAL
			iDropCount = pEvent->GetDropCount();
#endif // SERV_DROP_EVENT_RENEWAL

			return true;
		}
	}

	return false;
}
#endif SERV_ITEM_DROP_EVENT
//}}

bool KGameEventManager::CheckTournamentEvent()
{
	if( GetServerType() != ST_GAME  && 
		GetServerType() != ST_CENTER )
	{
		return false;
	}

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_TOURNAMENT].begin(); mit != m_mapIngEventData[KGameEvent::GET_TOURNAMENT].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_TOURNAMENT )
			continue;

		//. �������� ��ȸ�� ������..
		if( mit->second->IsEnable() == true
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			return true;
		}
	}

	return false;
}

bool KGameEventManager::CheckSpiritEvent()
{
	if( GetServerType() != ST_GAME )
		return false;

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_SPIRIT].begin(); mit != m_mapIngEventData[KGameEvent::GET_SPIRIT].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_SPIRIT )
			continue;

		//. �������� ��ȸ�� ������..
		if( mit->second->IsEnable() == true 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			return true;
		}
	}

	return false;
}

bool KGameEventManager::CheckSecretDungeonEvent( int iDungeonID, int iDungeonMode )
{
	if( GetServerType() != ST_GAME )
		return false;

	if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == false  &&
		CXSLDungeon::IsEventDungeon( iDungeonID ) == false  &&
		CXSLDungeon::IsHenirDungeon( iDungeonID ) == false )
	{
		// ��д��� �Ǵ� ��ϸ� �ð� �Ǵ� �̺�Ʈ������ �ƴϸ� ����!
		return false;
	}

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_SECRET_DUNGEON].begin(); mit != m_mapIngEventData[KGameEvent::GET_SECRET_DUNGEON].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_SECRET_DUNGEON )
			continue;		

		//{{ 2010. 10. 19	������	��д��� �̺�Ʈ ������Ʈ
#ifdef SERV_SECRET_DUNGEON_EVENT
		KGameEventSecretDungeon* pEvent = static_cast<KGameEventSecretDungeon*>(mit->second);

		if( pEvent->GetDungeonID() != 0 )
		{
			if( pEvent->GetDungeonID() != iDungeonID )
				continue;
		}

		//{{ 2011. 09. 28  �ӱԼ�		�ҷ��� ����
#ifdef SERV_DUNGEON_STAGE_RANDOM_NPC_POS
		if( CXSLDungeon::IsEventDungeon( iDungeonID ) == false )  // �߰�
		{
			if( iDungeonMode != CXSLDungeon::DM_INVALID )
			{
				if( pEvent->GetDungeonMode() != iDungeonMode )
					continue;
			}
		}
#else
		if( iDungeonMode != CXSLDungeon::DM_INVALID )
		{
			if( pEvent->GetDungeonMode() != iDungeonMode )
				continue;
		}
#endif SERV_DUNGEON_STAGE_RANDOM_NPC_POS
		//}}
		
#endif SERV_SECRET_DUNGEON_EVENT
		//}}

		//. �������� ��д��� ���� �̺�Ʈ�� ������..
		if( mit->second->IsEnable() == true
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			return true;
		}
	}

	return false;
}

//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER

void KGameEventManager::CheckEventMonster( OUT std::vector< int >& vecEventID )
{
	vecEventID.clear();

	if( GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_MONSTER].begin(); mit != m_mapIngEventData[KGameEvent::GET_MONSTER].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_MONSTER )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.04.23 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		if (mit->second->IsOn() == false)
			continue;
#endif //SERV_ALLOW_EVENT_ERASE

		const int iEventUID = mit->first;
		KGameEventMonster* pEvent = static_cast<KGameEventMonster*>(mit->second);

		// �������� ���� ���� �̺�Ʈ�� ������..
		if( mit->second->IsEnable() )
		{
			vecEventID.push_back( pEvent->GetEventID() );
		}
	}
}

#endif SERV_EVENT_MONSTER
//}}

//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
void KGameEventManager::CheckEventMonster( OUT std::set<int>& setEventID )
{
	setEventID.clear();

	if( GetServerType() != ST_CENTER )
		return;

	for( std::map<int, KGameEvent*>::const_iterator it=m_mapIngEventData[KGameEvent::GET_MONSTER].begin(); it != m_mapIngEventData[KGameEvent::GET_MONSTER].end(); ++it )
	{
		if( it->second == NULL )
			continue;

		if( it->second->GetType() != KGameEvent::GET_MONSTER )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.04.23 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		if (it->second->IsOn() == false)
			continue;
#endif //SERV_ALLOW_EVENT_ERASE

		const int iEventUID = it->first;
		KGameEventMonster* pEvent = static_cast<KGameEventMonster*>( it->second );

		// �������� ���� ���� �̺�Ʈ�� ������..
		if( it->second->IsEnable() )
		{
			setEventID.insert( pEvent->GetEventID() );
		}
	}
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
void KGameEventManager::CheckEnableEventQuest( IN OUT std::map< int, KEventQuestInfo >& mapEventQuest,
											  IN const std::map< int, KCompleteQuestInfo >& mapCompeletedQuest,
											  OUT std::vector< int >& vecBeginQuest, 
											  OUT std::vector< int >& vecEndQuest )
#else
void KGameEventManager::CheckEnableEventQuest( IN OUT std::map< int, KEventQuestInfo >& mapEventQuest,
											  IN const std::map< int, int >& mapCompeletedQuest,
											  OUT std::vector< int >& vecBeginQuest, 
											  OUT std::vector< int >& vecEndQuest )
#endif SERV_DAILY_QUEST
//}}
{
	if( GetServerType() != ST_GAME )
		return;

	vecBeginQuest.clear();
	vecEndQuest.clear();

	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, KGameEvent* >::const_iterator mit;
	std::map< int, KEventQuestInfo >::iterator mitET;

#ifdef SERV_ALLOW_EVENT_ERASE
	std::map< int, std::set< int > > mapQuestTurnOn;
	std::set< int > setQuestTurnOff;
#endif SERV_ALLOW_EVENT_ERASE

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	for( mit = m_mapIngEventData[KGameEvent::GET_QUEST].begin(); mit != m_mapIngEventData[KGameEvent::GET_QUEST].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_QUEST )
			continue;

		const int iEventUID = mit->first;
		KGameEventQuest* pEvent = static_cast<KGameEventQuest*>(mit->second);

#ifdef SERV_USE_TIME_QUEST_FLAG_IN_LUA
		{
			const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pEvent->GetQuestID() );
			if( pQuestTemplet != NULL && pQuestTemplet->m_bIsTimeEvent == false )
			{
				continue;
			}
		}
#endif SERV_USE_TIME_QUEST_FLAG_IN_LUA

		// 3. �̺�Ʈ�� ����������..
		if( mit->second->IsEnable() 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������ // ����Ʈ�� ��� Enable �� ������ ����Ʈ���� ����� �Ѵ�.
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE			
			)
		{
#ifdef SERV_ALLOW_EVENT_ERASE
			mapQuestTurnOn[ pEvent->GetQuestID() ].insert( iEventUID );
#else SERV_ALLOW_EVENT_ERASE
			//{{ 2011. 7.29 ��ȿ�� �ݺ� ����Ʈ �� ���� ����Ʈ�� ����Ʈ �̺�Ʈ�� ��ϰ����ϵ��� ����
#ifdef SERV_QUEST_EVENT_EXTEND
			const int iQuestID = pEvent->GetQuestID();
			std::map< int, KCompleteQuestInfo >::const_iterator const_it = mapCompeletedQuest.find( iQuestID );

			// �Ϸ��� ������ �ִ� �̺�Ʈ��� �ݺ� ���� ���θ� üũ�Ѵ�.
			if( const_it != mapCompeletedQuest.end() )
			{
				const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );

				if( pQuestTemplet == NULL )
					continue;

				// �ݺ� ���� ���� üũ //
				// �Ϲ� ����Ʈ�� �� �ѹ� ����
				if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
				{
					continue;
				}
				else if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
				{
					const KCompleteQuestInfo tmpCompletQuestInfo = const_it->second;

					if( tmpCompletQuestInfo.m_tCompleteDate == 0 )
					{
						START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
							<< BUILD_LOG( iQuestID )
							<< END_LOG;

						continue;
					}

					// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
					CTime tCompleteDate( tmpCompletQuestInfo.m_tCompleteDate );
					CTime tCurTime = CTime::GetCurrentTime();

					CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
					if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
					{
						// 6�ú��� �Ѵ� �ð��̶�� �������� ����
						tCheckTime += CTimeSpan( 1, 0, 0, 0 );
					}

					// ���� ���� �����Ѱ���?
					if( tCheckTime > tCurTime )
					{
						continue;
					}
				}
			}
#else
			// �̹� �Ϸ��� �̺�Ʈ��� �Ѿ��!
			if( mapCompeletedQuest.find( pEvent->GetQuestID() ) != mapCompeletedQuest.end() )
				continue;
#endif SERV_QUEST_EVENT_EXTEND
			//}}

			// 4-1. �������� �̺�Ʈ��� ������ �̺�Ʈ ����Ʈ�� Ȯ�� �� ���� �̺�Ʈ��� ä���ش�.
			mitET = mapEventQuest.find( iEventUID );
			if( mitET == mapEventQuest.end() )
			{
				//////////////////////////////////////////////////////////////////////////
				// �̺�Ʈ ���� ����!
				KEventQuestInfo kInfo;
				kInfo.m_bInit = true;
				kInfo.m_iEventUID = iEventUID;
				kInfo.m_iQuestID = pEvent->GetQuestID();
				mapEventQuest.insert( std::make_pair( iEventUID, kInfo ) );
				
				// Ŭ���̾�Ʈ���� ���۵Ǵ� �̺�Ʈ�� �˸��� ���ؼ� QuestID ����
				vecBeginQuest.push_back( pEvent->GetQuestID() );
			}
			else
			{
				// ĳ���͸� ó�� ������ ��Ȳ�̶� �̺�Ʈ �����Ͱ� �ʱ�ȭ �Ǿ� ���� �ʴٸ�..?
				if( mitET->second.m_bInit == false )
				{
					mitET->second.m_bInit = true;
					mitET->second.m_iEventUID = iEventUID;
					mitET->second.m_iQuestID = pEvent->GetQuestID();
				}
			}
#endif SERV_ALLOW_EVENT_ERASE
		}
		else
		{
			// 4-2. ����� �̺�Ʈ�� ������ �̺�Ʈ ����Ʈ���� ã�Ƽ� �����Ѵ�.
			mitET = mapEventQuest.find( iEventUID );
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // ����Ʈ�� ��� Enable �� ������ ����Ʈ���� ����� �Ѵ�.
			if( mitET != mapEventQuest.end() || mit->second->IsOn() == false )
			{
				setQuestTurnOff.insert( pEvent->GetQuestID() );
			}
#endif //SERV_ALLOW_EVENT_ERASE
			if( mitET != mapEventQuest.end() )
			{
				// �������� �̺�Ʈ ����Ʈ���� ����!
				mapEventQuest.erase( iEventUID );

#ifdef SERV_ALLOW_EVENT_ERASE
#else SERV_ALLOW_EVENT_ERASE
				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				vecEndQuest.push_back( pEvent->GetQuestID() );
				//}}
#endif SERV_ALLOW_EVENT_ERASE
			}
		}
	}

#ifdef SERV_ALLOW_EVENT_ERASE
	std::map< int, std::set< int > >::iterator mitQuestTurnOn = mapQuestTurnOn.begin();
	for( ; mitQuestTurnOn != mapQuestTurnOn.end(); ++mitQuestTurnOn )
	{
		std::set< int >& setQuestUIDTurnOn = mitQuestTurnOn->second;

		// ���� ��Ȳ���� �߻����� ���� ��������, ���� set�� ��� �ִ� ��찡 ������� ó���� �Ӵϴ�. by ������
		if( setQuestUIDTurnOn.empty() == true )
			continue;

		int iQuestID = mitQuestTurnOn->first;

		//{{ 2011. 7.29 ��ȿ�� �ݺ� ����Ʈ �� ���� ����Ʈ�� ����Ʈ �̺�Ʈ�� ��ϰ����ϵ��� ����
#ifdef SERV_QUEST_EVENT_EXTEND
		std::map< int, KCompleteQuestInfo >::const_iterator const_it = mapCompeletedQuest.find( iQuestID );

		// �Ϸ��� ������ �ִ� �̺�Ʈ��� �ݺ� ���� ���θ� üũ�Ѵ�.
		if( const_it != mapCompeletedQuest.end() )
		{
			const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );

			if( pQuestTemplet == NULL )
				continue;

			// �ݺ� ���� ���� üũ //
			// �Ϲ� ����Ʈ�� �� �ѹ� ����
			if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
			{
				continue;
			}
			else if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
			{
				const KCompleteQuestInfo tmpCompletQuestInfo = const_it->second;

				if( tmpCompletQuestInfo.m_tCompleteDate == 0 )
				{
					START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
						<< BUILD_LOG( iQuestID )
						<< END_LOG;

					continue;
				}

				// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
				CTime tCompleteDate( tmpCompletQuestInfo.m_tCompleteDate );
				CTime tCurTime = CTime::GetCurrentTime();

				CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
				if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
				{
					// 6�ú��� �Ѵ� �ð��̶�� �������� ����
					tCheckTime += CTimeSpan( 1, 0, 0, 0 );
				}

				// ���� ���� �����Ѱ���?
				if( tCheckTime > tCurTime )
				{
					continue;
				}
			}
		}
#else SERV_QUEST_EVENT_EXTEND
		// �̹� �Ϸ��� �̺�Ʈ��� �Ѿ��!
		if( mapCompeletedQuest.find( iQuestID ) != mapCompeletedQuest.end() )
			continue;
#endif SERV_QUEST_EVENT_EXTEND
		//}}

		std::set< int >::iterator sitQuestUIDTurnOn = setQuestUIDTurnOn.begin();
		for( ; sitQuestUIDTurnOn != setQuestUIDTurnOn.end(); ++sitQuestUIDTurnOn )
		{
			int iEventUID = *sitQuestUIDTurnOn;

			// 4-1. �������� �̺�Ʈ��� ������ �̺�Ʈ ����Ʈ�� Ȯ�� �� ���� �̺�Ʈ��� ä���ش�.
			mitET = mapEventQuest.find( iEventUID );
			if( mitET == mapEventQuest.end() )
			{
				//////////////////////////////////////////////////////////////////////////
				// �̺�Ʈ ���� ����!
				KEventQuestInfo kInfo;
				kInfo.m_bInit = true;
				kInfo.m_iEventUID = iEventUID;
				kInfo.m_iQuestID = iQuestID;
				mapEventQuest.insert( std::make_pair( iEventUID, kInfo ) );

				// Ŭ���̾�Ʈ���� ���۵Ǵ� �̺�Ʈ�� �˸��� ���ؼ� QuestID ����
				vecBeginQuest.push_back( iQuestID );
			}
			else
			{
				// ĳ���͸� ó�� ������ ��Ȳ�̶� �̺�Ʈ �����Ͱ� �ʱ�ȭ �Ǿ� ���� �ʴٸ�..?
				if( mitET->second.m_bInit == false )
				{
					mitET->second.m_bInit = true;
					mitET->second.m_iEventUID = iEventUID;
					mitET->second.m_iQuestID = iQuestID;
				}
			}
		}
	}

	std::set< int >::iterator sitQuestTurnOff = setQuestTurnOff.begin();
	for( ; sitQuestTurnOff != setQuestTurnOff.end(); ++sitQuestTurnOff )
	{
		int iQuestID = *sitQuestTurnOff;
		if( mapQuestTurnOn.find( iQuestID ) != mapQuestTurnOn.end() )
			continue;

		//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
		vecEndQuest.push_back( iQuestID );		// 2013.06.10 lygan_������ // ����Ʈ�� ��� Enable �� ������ ����Ʈ���� ����� �Ѵ�.
		//}}
	}
#endif SERV_ALLOW_EVENT_ERASE
}

#endif SERV_DAY_QUEST
//}}

void KGameEventManager::InitTimeEventForCharInit( IN const std::vector< KConnectTimeEventInfo >& vecEventTime, 
												  IN const std::vector< KCumulativeTimeEventInfo >& vecCumulativeEventTime,
												  OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime,
												  OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime,
												  OUT std::set< int >& setCompletedEvent 
												  )
{
	CTime tEventTime;
	CTime tCurTime = CTime::GetCurrentTime();

	// ���� �ð� �̺�Ʈ ����
	for( u_int ui = 0; ui < vecCumulativeEventTime.size(); ++ui )
	{
		std::pair< KUserCumulativeTimeEventInfo, KTimerManager > kUserCumulativeEvent;
		kUserCumulativeEvent.first.m_iEventUID = vecCumulativeEventTime[ui].m_iEventUID;
		kUserCumulativeEvent.second.restart();
		kUserCumulativeEvent.second.SetAddTime( static_cast<double>( vecCumulativeEventTime[ui].m_iCumulativeTime * 60 ) );
		mapCumulativeEventTime.insert( std::make_pair( vecCumulativeEventTime[ui].m_iEventUID, kUserCumulativeEvent ) );
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
			setCompletedEvent.insert( vecEventTime[ui].m_iEventUID );
			continue;
		}

		// �ð� �̺�Ʈ ����Ʈ���� ���� �ð� �̺�Ʈ�� �ƴѰ͸� ã�Ƽ� �����Ѵ�.
		if( mapCumulativeEventTime.find( vecEventTime[ui].m_iEventUID ) == mapCumulativeEventTime.end() )
		{
			std::pair< KUserConnectTimeEventInfo, KTimerManager > kUserConnectEvent;
			kUserConnectEvent.first.m_fEventTime = END_EVENT; // �⺻������ ����� �̺�Ʈ���� �ִ´�
			kUserConnectEvent.first.m_kCTime		= tEventTime.GetTime();
			kUserConnectEvent.first.m_bGiveOnce	= true;
			kUserConnectEvent.second.restart(); // Ÿ�̸� �ʱ�ȭ
			mapConnectEventTime.insert( std::make_pair( vecEventTime[ui].m_iEventUID, kUserConnectEvent ) );
		}
	}
}

void KGameEventManager::GetEventTimerInfo( IN const std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime, OUT std::map< int, double >& mapEventTimer )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = mapConnectEventTime.begin(); mit != mapConnectEventTime.end(); ++mit )
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

void KGameEventManager::SetEventTimerInfo( OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime, IN const std::map< int, double >& mapEventTimer )
{
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitFind;
	std::map< int, double >::const_iterator mit;
	for( mit = mapEventTimer.begin(); mit != mapEventTimer.end(); ++mit )
	{
		mitFind = mapConnectEventTime.find( mit->first );
		if( mitFind == mapConnectEventTime.end() )
			continue;

		KTimerManager& kEventTimer = mitFind->second.second;

		// ä���̵����� ���� �̺�Ʈ Ÿ�̸� ����
		kEventTimer.SetAddTime( mit->second );
	}
}

//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
bool KGameEventManager::GetConnectTimeEventReward( IN const u_char ucLevel, IN const int iEventUID, OUT int& iRewardID )
#else
bool KGameEventManager::GetConnectTimeEventReward( IN int iEventUID, OUT int& iRewardID )
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
//}}
{
	std::map< int, KGameEvent* >::const_iterator mit = m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].find( iEventUID );
	if( mit == m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].end() )
		return false;

	if( mit->second == NULL )
		return false;

	if( mit->second->GetType() != KGameEvent::GET_CONNECT_TIME )
		return false;

	KGameEventConnectTime* pEvent = static_cast<KGameEventConnectTime*>(mit->second);

#ifdef SERV_CONNECT_EVENT_LEVEL_UP_CHECK
	iRewardID = pEvent->GetRewardID();
#else //SERV_CONNECT_EVENT_LEVEL_UP_CHECK
	//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
	if( pEvent->IsMinMaxReward() == true )
	{
        iRewardID = pEvent->GetMinMaxRewardID( ucLevel );
	}
	else
	{
		iRewardID = pEvent->GetRewardID();
	}
#else
	iRewardID = pEvent->GetRewardID();
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
	//}}	
#endif //SERV_CONNECT_EVENT_LEVEL_UP_CHECK

	return true;
}

//{{ 2012. 10. 13	�ڼ���	���� �̺�Ʈ ���� ����
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
void KGameEventManager::CheckEnableConnectTimeEventForCharInit( IN OUT ConnectEventFactorSet& kFactor )
{
	if( ( kFactor.m_pMapEventTime == NULL ) ||
		( kFactor.m_pSetCompletedEvent == NULL ) )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ���� �� �̻�!! ( �־ �ȵǴ� �����Դϴ�. ��� Ȯ�����ּ��� )" )
			<< BUILD_LOG( kFactor.m_pMapEventTime )
			<< BUILD_LOG( kFactor.m_pSetCompletedEvent )
			<< END_LOG;
		return;
	}

	std::vector< KTimeEventSimpleInfo > vecDummy1;
	std::vector< KTimeEventSimpleInfo > vecDummy2;

	kFactor.m_pVecBeginEvent	= &vecDummy1;
	kFactor.m_pVecEndEvent		= &vecDummy2;

	CheckEnableConnectTimeEvent( kFactor );

	// ����� �̺�Ʈ �Ǵ� ������� �̺�Ʈ��� ���� ó�� ����!
	std::vector< int > vecEndEvent;
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = kFactor.m_pMapEventTime->begin(); mit != kFactor.m_pMapEventTime->end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kEventInfo = mit->second.first;

		// �̺�Ʈ �ð������� ������ Ȱ��ȭ���� ���� �̺�Ʈ���..
		if( kEventInfo.m_fEventTime <= END_EVENT )
		{
			vecEndEvent.push_back( mit->first );
		}
	}

	// ����� �̺�Ʈ���� ����
	std::vector< int >::const_iterator vit;
	for( vit = vecEndEvent.begin(); vit != vecEndEvent.end(); ++vit )
	{
		kFactor.m_pMapEventTime->erase( *vit );
	}
}

void KGameEventManager::CheckEnableConnectTimeEvent( IN OUT ConnectEventFactorSet& kFactor )
{
	if( GetServerType() != ST_GAME )
		return;

	if( ( kFactor.m_pVecBeginEvent == NULL  ) ||
		( kFactor.m_pVecEndEvent == NULL  ) ||
		( kFactor.m_pMapEventTime == NULL  ) ||
		( kFactor.m_pSetCompletedEvent == NULL  ) )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ���� �� �̻�!! ( �־ �ȵǴ� �����Դϴ�. ��� Ȯ�����ּ��� )" )
			<< BUILD_LOG( kFactor.m_iUnitUID )
			<< BUILD_LOG( kFactor.m_pVecBeginEvent )
			<< BUILD_LOG( kFactor.m_pVecEndEvent )
			<< BUILD_LOG( kFactor.m_pMapEventTime )
			<< BUILD_LOG( kFactor.m_pSetCompletedEvent )
			<< END_LOG;
		return;
	}

	kFactor.m_pVecBeginEvent->clear();
	kFactor.m_pVecEndEvent->clear();

	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, KGameEvent* >::const_iterator mit;
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitET;

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	for( mit = m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].begin(); mit != m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_CONNECT_TIME )
			continue;

		const int iEventUID = mit->first;
		KGameEventConnectTime* pEvent = static_cast<KGameEventConnectTime*>(mit->second);

		// 3. �̺�Ʈ�� ����������..
		if( mit->second->IsEnable() )
		{
			//{{ 2009. 10. 26  ������	PC���̺�Ʈ
			if( pEvent->IsPcBangEvent() == true  &&  kFactor.m_bIsPcBang == false )
				continue;
			//}}

			//{{ 2009. 12. 16  ������	�̺�Ʈ����
			if( pEvent->GetUnitLevel() != 0  &&  pEvent->GetUnitLevel() > static_cast<int>( kFactor.m_ucLevel ) )
				continue;
			//}}

			//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			if( pEvent->GetChannelCode() != KNexonAccountInfo::CE_NONE )
			{
				if( pEvent->GetChannelCode() != kFactor.m_iChannelCode )
					continue;
			}
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			//}}

			//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			if( pEvent->GetBF_Team() != 0 && pEvent->GetBF_Team() != kFactor.m_iBF_Team )
				continue;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			//}}

			//{{ 2011. 10. 12	��μ�	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
			// ���� ���� ������ �����밡 �ƴ϶�� �Ѿ��!
			if( pEvent->IsMinMaxReward() == true )
			{
				if( pEvent->GetMinMaxRewardID( static_cast<int>( kFactor.m_ucLevel ) ) == 0 )
				{
					START_LOG( clog, L"���� ���� ������ �ƴ϶� �̺�Ʈ ��󿡼� ����!" )
						<< END_LOG;
					continue;
				}
			}
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
			//}}

			//{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
			if( pEvent->GetUnitType() != 0  &&  pEvent->GetUnitType() != kFactor.m_cUnitType )
				continue;

			if( pEvent->GetUnitClass() != 0  &&  pEvent->GetUnitClass() != kFactor.m_cUnitClass )
				continue;
#endif SERV_CHAR_CONNECT_EVENT
			//}}
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
			if( pEvent->GetUnitClassLevel() == 0 && CXSLUnit::IsInitNormalJob(static_cast<CXSLUnit::UNIT_CLASS>(kFactor.m_cUnitClass)) == false )
				continue;

			if( pEvent->GetUnitClassLevel() == 1 && CXSLUnit::IsFirstChangeJob(static_cast<CXSLUnit::UNIT_CLASS>(kFactor.m_cUnitClass)) == false )
				continue;

			if( pEvent->GetUnitClassLevel() == 2 && CXSLUnit::IsSecondChangeJob(static_cast<CXSLUnit::UNIT_CLASS>(kFactor.m_cUnitClass)) == false )
				continue;			
#endif SERV_UNIT_CLASS_LEVEL_EVENT

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
			if( pEvent->GetEventItemID() != 0 )
			{
				std::set< int >::iterator sitItemIDInInventory = kFactor.m_setItemID.find( pEvent->GetEventItemID() );
				if( sitItemIDInInventory == kFactor.m_setItemID.end() )
					continue;
			}
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT


#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
			if( pEvent->IsNewUnitEvent() == true && kFactor.m_bIsNewUnit == false  )
			{
				continue;
			}

			if( pEvent->IsNewUnitEvent2() == true && kFactor.m_bIsNewUnit2 == false )
			{
				continue;
			}

			if( pEvent->IsCurrentUnitEvent() == true && kFactor.m_bIsCurrentUnit == false)// bIsCurrentUnit = false
			{
				continue;
			}
			
			//START_LOG( cerr, L"�輮��_������ �����׽�Ʈ!" )
			//	<< BUILD_LOG( pEvent->GetScriptID() )
			//	<< BUILD_LOG( pEvent->IsNewUnitEvent() )
			//	<< BUILD_LOG( pEvent->IsNewUnitEvent2() )
			//	<< BUILD_LOG( pEvent->IsCurrentUnitEvent() )
			//	<< BUILD_LOG( kFactor.m_bIsNewUnit )
			//	<< BUILD_LOG( kFactor.m_bIsNewUnit2 )
			//	<< BUILD_LOG( kFactor.m_bIsCurrentUnit )
			//	<< END_LOG;
			
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

			// �̹� �Ϸ��� �̺�Ʈ��� �Ѿ��!
			if( kFactor.m_pSetCompletedEvent->find( iEventUID ) != kFactor.m_pSetCompletedEvent->end() )
				continue;

			//{{ 2012. 06. 05	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			// ���ϸ��� ���� ���� �̺�Ʈ�ΰ�?
			{
				int iItemID;

				if( SiKDailyGiftBoxManager()->IsDailyGiftBoxScriptID( pEvent->GetScriptID(), iItemID ) == true )
				{
					// �ش� ��Ű���� ������ �������� Ȯ���Ѵ�.
					std::pair<std::multimap<int, KDailyGiftBoxInfo>::const_iterator, std::multimap<int, KDailyGiftBoxInfo>::const_iterator> pairFindResult = mmapDailyGiftBoxList.equal_range( iItemID );
					for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
					{
						if( pairFindResult.first->second.m_iUnitUID != iUnitUID )
						{
							continue;
						}

						if( pairFindResult.first->second.m_iState != KDailyGiftBoxManager::PURCHASED_CONFIRM )
						{
							continue;
						}

						// �ð� �˻� ( �����ϰ��� 7�� �̳��� �͸� �־�� �Ѵ�. )
						CTime tPurchasedDate;
						if( KncUtil::ConvertStringToCTime( pairFindResult.first->second.m_wstrRegDate, tPurchasedDate ) == false )
						{
							continue;
						}

						CTime tAfterWeekFromPurchasedDate = CTime( tPurchasedDate.GetYear(), tPurchasedDate.GetMonth(), tPurchasedDate.GetDay(), 0, 0, 0 ) + CTimeSpan( 6, 23, 59, 59 );
						if( tAfterWeekFromPurchasedDate < CTime::GetCurrentTime() )
						{
							continue;
						}
						break;
					}

					if( pairFindResult.first == pairFindResult.second )
					{
						continue;
					}
				}
			}
#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}

			//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
			if( CheckWinterVacationEvent( iEventUID, pEvent->GetScriptID(), kFactor ) == false )
			{
				continue;
			}
#endif SERV_2012_WINTER_VACATION_EVENT
			//}}
			//{{ 2012. 12. 25	�ڼ���	Ư�� ���� ���� ���� �̺�Ʈ
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
			if( SiKGameEventScriptManager()->CheckEventGroup( pEvent->GetEventGroupID(), kFactor.m_iUserUID ) == false )
			{
				continue;
			}
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
			//}}

#ifdef SERV_CUSTOM_CONNECT_EVENT
			if( pEvent->GetCustomEventID() != 0  &&  pEvent->GetCustomEventID() != kFactor.m_iCustomEventID )
				continue;
#endif SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
			if( pEvent->IsOnlySteamUserEvent() == true && kFactor.m_bIsSteamUser == false )
			{
				START_LOG( clog, L"[TEST] �������� �����ε� ���������� �ƴ϶� �н�" )
					<< END_LOG;
				continue;
			}

			if( pEvent->IsOnlyNotSteamUserEvent() == true && kFactor.m_bIsSteamUser == true )
			{
				START_LOG( clog, L"[TEST] NOT�������� �����ε� ���������� �н�" )
					<< END_LOG;
				continue;
			}
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD // 2013.01.23 lygan_������ // �ڵ� ���� ���� �̺�Ʈ ������ 
			if ( pEvent->IsRepeatRewardEvent() == true && ( pEvent->GetBeforeEventID())!= 0 ) // 2013.01.23 lygan_������ // �ڵ� ���� ���� �̺�Ʈ�ε� ���� �̺�Ʈ �� 0 �� �ƴϸ� ���� ���� �̺�Ʈ�� �Ϸ� �ߴ��� üũ �ؾ� �Ѵ�.
			{

				std::set< int >::const_iterator  mitSetComplet = kFactor.m_pSetCompletedEvent->begin();
				bool bCheckNoStartRewardEvent = true;

				for ( ; mitSetComplet != kFactor.m_pSetCompletedEvent->end() ; ++mitSetComplet )
				{
					std::map< int, KGameEvent* >::const_iterator mit2 = m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].find( *mitSetComplet );	

					if ( mit2 != m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].end() )
					{
						KGameEventConnectTime* pEventOld = static_cast<KGameEventConnectTime*>(mit2->second);

						if ( pEventOld->IsRepeatRewardEvent() == true && pEventOld->GetScriptID() == pEvent->GetBeforeEventID() && pEventOld->GetBeginDate() == pEvent->GetBeginDate() )
						{
							bCheckNoStartRewardEvent = false;

						}

					}

				}

				if ( bCheckNoStartRewardEvent == true ) 
					continue;

			}
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef	SERV_CRITERION_DATE_EVENT// ���볯¥: 2013-04-11
			if( CheckCriterionUserEvent( kFactor.m_byteCriterionDateEventInfo, kFactor.m_wstrRegDate, pEvent->GetScriptID() ) == false )
			{
				continue;
			}
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_NEW_USER_QUEST_HARD_CODING
			if( 2044 == pEvent->GetScriptID() )
			{
				CTime tRegDate;
				if( KncUtil::ConvertStringToCTime( kFactor.m_wstrRegDate, tRegDate ) == false )
				{
					START_LOG( cwarn, L"���� ���� ���� ��ȯ ����( ä�� �������� ���� ���� �Ͻ����� ���ɼ��� ���� )" )
						<< BUILD_LOG( kFactor.m_wstrRegDate )
						<< END_LOG;
					continue;
				}

				if( false == (( CTime(2014, 1, 8, 6, 0, 0) <= tRegDate ) &&
					( tRegDate < CTime(2014, 1, 22, 6, 0, 0) )))
				{
					continue;
				}
			}
#endif //SERV_NEW_USER_QUEST_HARD_CODING

			// 4-1. �������� �̺�Ʈ��� ������ �̺�Ʈ ����Ʈ�� Ȯ�� �� ���� �̺�Ʈ��� ä���ش�.
			mitET = kFactor.m_pMapEventTime->find( iEventUID );
			if( mitET == kFactor.m_pMapEventTime->end() )
			{
				//////////////////////////////////////////////////////////////////////////
				// �̺�Ʈ ���� ����!
				CTime tSetTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0 );

				std::pair< KUserConnectTimeEventInfo, KTimerManager > kEventTime;
				kEventTime.first.m_bInit			= true;				
				kEventTime.first.m_iEventID			= pEvent->GetEventID();
				kEventTime.first.m_wstrEventName	= pEvent->GetEventName();
				kEventTime.first.m_fEventTime		= pEvent->GetEventTime();
				kEventTime.first.m_kCTime			= tSetTime.GetTime();
				kEventTime.first.m_bGiveOnce		= pEvent->IsGiveOnce();
				//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
				kEventTime.first.m_bAccountEvent	= pEvent->IsAccountEvent();
#endif SERV_ACC_TIME_EVENT
				//}}
				//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
				kEventTime.first.m_bRepeatEvent	= pEvent->GetRepeatEvent();
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
				//}}

				//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
				kEventTime.first.m_iScriptID	= pEvent->GetScriptID();
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
				//}}
				
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
				kEventTime.first.m_bRepeatRewardEvent = pEvent->IsRepeatRewardEvent();
				kEventTime.first.m_iBeforeEventID = pEvent->GetBeforeEventID();
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD	

#ifdef SERV_CUSTOM_CONNECT_EVENT
				kEventTime.first.m_iCustomEventID = pEvent->GetCustomEventID();
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
				kEventTime.first.m_bOnlySteamUser = pEvent->IsOnlySteamUserEvent();
				kEventTime.first.m_bOnlyNotSteamUser = pEvent->IsOnlyNotSteamUserEvent();
#endif //SERV_STEAM_USER_CONNECT_EVENT

				kEventTime.second.restart(); // Ÿ�̸� �ʱ�ȭ
				kFactor.m_pMapEventTime->insert( std::make_pair( iEventUID, kEventTime ) );

				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				KTimeEventSimpleInfo kBeginEventInfo;
				kBeginEventInfo.m_iEventUID		= iEventUID;
				kBeginEventInfo.m_iEventID		= pEvent->GetEventID();
				kBeginEventInfo.m_wstrEventName = pEvent->GetEventName();
				kBeginEventInfo.m_fEventTime	= pEvent->GetEventTime();
				kFactor.m_pVecBeginEvent->push_back( kBeginEventInfo );
				//}}
			}
			else
			{
				// ĳ���͸� ó�� ������ ��Ȳ�̶� �̺�Ʈ �����Ͱ� �ʱ�ȭ �Ǿ� ���� �ʴٸ�..?
				if( mitET->second.first.m_bInit == false )
				{
					mitET->second.first.m_bInit			= true;
					mitET->second.first.m_wstrEventName	= pEvent->GetEventName();
					mitET->second.first.m_iEventID		= pEvent->GetEventID();
					mitET->second.first.m_fEventTime	= pEvent->GetEventTime();
					mitET->second.first.m_bGiveOnce		= pEvent->IsGiveOnce();
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					mitET->second.first.m_bAccountEvent	= pEvent->IsAccountEvent();
#endif SERV_ACC_TIME_EVENT
					//}}
					//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					mitET->second.first.m_bRepeatEvent	= pEvent->GetRepeatEvent();
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					//}}
					
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
					mitET->second.first.m_bRepeatRewardEvent = pEvent->IsRepeatRewardEvent();
					mitET->second.first.m_iBeforeEventID = pEvent->GetBeforeEventID();
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_CUSTOM_CONNECT_EVENT
					mitET->second.first.m_iCustomEventID = pEvent->GetCustomEventID();
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
					mitET->second.first.m_bOnlySteamUser = pEvent->IsOnlySteamUserEvent();
					mitET->second.first.m_bOnlyNotSteamUser = pEvent->IsOnlyNotSteamUserEvent();
#endif //SERV_STEAM_USER_CONNECT_EVENT
					
				}
			}
		}
		else
		{
			// 4-2. �̺�Ʈ �Ⱓ�� ����Ǿ����� ������ �̺�Ʈ�� ��� �����ų���� üũ�Ѵ�.
			if( !kFactor.m_bDelFinishEvent  &&  !pEvent->IsDelEndEvent() )
				continue;

			// 4-3. ����� �̺�Ʈ�� ������ �̺�Ʈ ����Ʈ���� ã�Ƽ� �����Ѵ�.
			mitET = kFactor.m_pMapEventTime->find( iEventUID );
			if( mitET != kFactor.m_pMapEventTime->end() )
			{
				// �������� �̺�Ʈ ����Ʈ���� ����!
				kFactor.m_pMapEventTime->erase( iEventUID );
#ifdef SERV_ALLOW_EVENT_ERASE		// 2013.06.11 lygan_������ // ���� �̺�Ʈ�� ���ư��ٰ� ���� ���ܼ� �ٷ� �� ��� DB���� off ó�� �Ȱ� �Ϸ��ʿ� ���� �ʴ´�. ���� ������ �ش� �̺�Ʈ�� �ٽ� ����� �ϱ� ����. ���� �� �ű� ����ϸ� �ѹ� �� ������ �ִ�.
				if ( pEvent->IsOn() == true  )
				{
					kFactor.m_pSetCompletedEvent->insert( iEventUID );
				}
#else //SERV_ALLOW_EVENT_ERASE				
				// �Ϸ� �̺�Ʈ�� �߰�
				kFactor.m_pSetCompletedEvent->insert( iEventUID );
#endif //SERV_ALLOW_EVENT_ERASE		

				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				kFactor.m_pVecEndEvent->push_back( KTimeEventSimpleInfo( iEventUID, pEvent->GetEventID() ) );
				//}}
			}
		}
	}
}

void KGameEventManager::CheckConnectTimeEvent( IN OUT ConnectEventFactorSet& kFactor )
{
	if( GetServerType() != ST_GAME )
		return;

	if( ( kFactor.m_pVecUpdatedTime == NULL  ) ||
		( kFactor.m_pVecBeginEvent == NULL  ) ||
		( kFactor.m_pVecEndEvent == NULL  ) ||
		( kFactor.m_pMapEventTime == NULL  ) ||
		( kFactor.m_pSetCompletedEvent == NULL  ) )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ���� �� �̻�!! ( �־ �ȵǴ� �����Դϴ�. ��� Ȯ�����ּ��� )" )
			<< BUILD_LOG( kFactor.m_iUnitUID )
			<< BUILD_LOG( kFactor.m_pVecUpdatedTime )
			<< BUILD_LOG( kFactor.m_pVecBeginEvent )
			<< BUILD_LOG( kFactor.m_pVecEndEvent )
			<< BUILD_LOG( kFactor.m_pMapEventTime )
			<< BUILD_LOG( kFactor.m_pSetCompletedEvent )
			<< END_LOG;
		return;
	}

	kFactor.m_pVecUpdatedTime->clear();
	kFactor.m_pVecBeginEvent->clear();
	kFactor.m_pVecEndEvent->clear();

	CheckEnableConnectTimeEvent( kFactor );

	CTime tCurrentTime = CTime::GetCurrentTime();

	std::vector< int > vecCompletedEvent;
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitET;

	for( mitET = kFactor.m_pMapEventTime->begin(); mitET != kFactor.m_pMapEventTime->end(); ++mitET )
	{
		const int iEventUID = mitET->first;
		KUserConnectTimeEventInfo& kEventInfo = mitET->second.first;
		KTimerManager& kEventTimer = mitET->second.second;

		// 1. �̹� ������ �޾Ҵ��� �˻�
		if( tCurrentTime.GetTime() <= kEventInfo.m_kCTime )
		{
			vecCompletedEvent.push_back( iEventUID );
			continue;
		}

		// 2. END_EVENT���� �̹� '�������� �̺�Ʈ'��.
		if( kEventInfo.m_fEventTime <= END_EVENT )
		{
			vecCompletedEvent.push_back( iEventUID );
			continue;
		}

		// 3. ��� �ð��� ��������..
		if( kEventTimer.elapsed() < ( kEventInfo.m_fEventTime * 60.0 ) )
			continue;

		kEventTimer.restart();

		// 4. ���� �ð� ����
		CTime tUpdateTime;
		if( kEventInfo.m_bGiveOnce )
		{
			tUpdateTime = tCurrentTime;
			tUpdateTime += CTimeSpan( 18250, 0, 0, 0 ); // �ѹ��� ��� �ϹǷ� 50���� ���Ѵ�.
		}
		else
		{
#ifdef SERV_INT_ONLY
			tUpdateTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 23, 59, 59 ); // ������ �Ǳ������� ���� ����.
#else //SERV_INT_ONLY
			tUpdateTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 05, 59, 59 ) + CTimeSpan( 1, 0, 0, 0 ); // ������ �Ǳ������� ���� ����.
#endif //SERV_INT_ONLY
		}

		//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		if( kEventInfo.m_bRepeatEvent )
		{
			tUpdateTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), tCurrentTime.GetHour(), tCurrentTime.GetMinute(), 0 ); // �ٷ� ���� �� �ִ�
		}
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}

		kEventInfo.m_kCTime = tUpdateTime.GetTime();

		// 5. DB�� ������Ʈ
		KConnectTimeEventInfo kConnectTimeEvent;
		kConnectTimeEvent.m_iEventUID		= iEventUID;
		kConnectTimeEvent.m_wstrEventTime	= ( CStringW )tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		kConnectTimeEvent.m_bAccountEvent	= kEventInfo.m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
		//}}

		//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		kConnectTimeEvent.m_iScriptID		= kEventInfo.m_iScriptID;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}

		kFactor.m_pVecUpdatedTime->push_back( kConnectTimeEvent );

		// 6. Ŭ���̾�Ʈ�� ����Ǵ� �̺�Ʈ ������
		kFactor.m_pVecEndEvent->push_back( KTimeEventSimpleInfo( iEventUID, kEventInfo.m_iEventID ) );

		// 7. �Ϸ� �̺�Ʈ ����Ʈ�� �ֱ�
		//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		if( kEventInfo.m_bRepeatEvent == false )
		{
			vecCompletedEvent.push_back( iEventUID );
		}
		else
		{
			KTimeEventSimpleInfo kRepeatEvent;
			kRepeatEvent.m_iEventUID = iEventUID;
			kRepeatEvent.m_iEventID = kEventInfo.m_iEventID;
			kRepeatEvent.m_wstrEventName = kEventInfo.m_wstrEventName;
			kRepeatEvent.m_fEventTime = kEventInfo.m_fEventTime;
			kFactor.m_pVecBeginEvent->push_back( kRepeatEvent );

			mitET->second.second.restart();
		}
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}
	}

	std::vector< int >::const_iterator vit;
	for( vit = vecCompletedEvent.begin(); vit != vecCompletedEvent.end(); ++vit )
	{
		// �������� �̺�Ʈ ����Ʈ���� ����
		kFactor.m_pMapEventTime->erase( *vit );

		// �Ϸ� �̺�Ʈ ����Ʈ�� �߰�
		kFactor.m_pSetCompletedEvent->insert( *vit );
	}
}
#else
void KGameEventManager::CheckEnableConnectTimeEventForCharInit( IN const u_char ucLevel,
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

																IN OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapEventTime,
																IN OUT std::set< int >& setCompletedEvent

																//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
																, IN const UidType& iUnitUID
																, IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
																//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
															   , IN const int iEventItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
															   , IN bool bIsNewUnit
															   , IN bool bIsNewUnit2
															   , IN bool bIsCurrentUnit
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
																)
{
	std::vector< KTimeEventSimpleInfo > vecDummy1;
	std::vector< KTimeEventSimpleInfo > vecDummy2;

	CheckEnableConnectTimeEvent(
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

		, mapEventTime,
		setCompletedEvent,
		vecDummy1,
		vecDummy2,
		true

		//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		, iUnitUID
		, mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		,iEventItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		, bIsNewUnit
		, bIsNewUnit2
		, bIsCurrentUnit
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		);

	// ����� �̺�Ʈ �Ǵ� ������� �̺�Ʈ��� ���� ó�� ����!
	std::vector< int > vecEndEvent;
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = mapEventTime.begin(); mit != mapEventTime.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kEventInfo = mit->second.first;

		// �̺�Ʈ �ð������� ������ Ȱ��ȭ���� ���� �̺�Ʈ���..
		if( kEventInfo.m_fEventTime <= END_EVENT )
		{
			vecEndEvent.push_back( mit->first );
		}
	}

	// ����� �̺�Ʈ���� ����
	std::vector< int >::const_iterator vit;
	for( vit = vecEndEvent.begin(); vit != vecEndEvent.end(); ++vit )
	{
		mapEventTime.erase( *vit );
	}
}

void KGameEventManager::CheckEnableConnectTimeEvent( IN const u_char ucLevel,
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
                                                     IN OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapEventTime,
                                                     IN OUT std::set< int >& setCompletedEvent,
                                                     OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
                                                     OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent,
                                                     IN const bool bDelFinishEvent
													 //{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
													 , IN const UidType& iUnitUID
													 , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
													 //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
													 , IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
													 ,IN bool bIsNewUnit
													 ,IN bool bIsNewUnit2
													 ,IN bool bIsCurrentUnit
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
													 )
{
	if( GetServerType() != ST_GAME )
		return;

	vecBeginEvent.clear();
	vecEndEvent.clear();

	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, KGameEvent* >::const_iterator mit;
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitET;

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	for( mit = m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].begin(); mit != m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_CONNECT_TIME )
			continue;

		const int iEventUID = mit->first;
		KGameEventConnectTime* pEvent = static_cast<KGameEventConnectTime*>(mit->second);

		// 3. �̺�Ʈ�� ����������..
		if( mit->second->IsEnable() 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			//{{ 2009. 10. 26  ������	PC���̺�Ʈ
			if( pEvent->IsPcBangEvent() == true  &&  bIsPcBang == false )
				continue;
			//}}

			//{{ 2009. 12. 16  ������	�̺�Ʈ����
			if( pEvent->GetUnitLevel() != 0  &&  pEvent->GetUnitLevel() > static_cast<int>(ucLevel) )
				continue;
			//}}

			//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			if( pEvent->GetChannelCode() != KNexonAccountInfo::CE_NONE )
			{
				if( pEvent->GetChannelCode() != iChannelCode )
					continue;
			}
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			//}}

			//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			if( pEvent->GetBF_Team() != 0 && pEvent->GetBF_Team() != iBF_Team )
				continue;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			//}}

			//{{ 2011. 10. 12	��μ�	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
			// ���� ���� ������ �����밡 �ƴ϶�� �Ѿ��!
			if( pEvent->IsMinMaxReward() == true )
			{
				if( pEvent->GetMinMaxRewardID( static_cast<int>(ucLevel) ) == 0 )
				{
					START_LOG( clog, L"���� ���� ������ �ƴ϶� �̺�Ʈ ��󿡼� ����!" )
						<< END_LOG;
					continue;
				}
			}
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
			//}}

			//{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
			if( pEvent->GetUnitType() != 0  &&  pEvent->GetUnitType() != cUnitType )
				continue;
#endif SERV_CHAR_CONNECT_EVENT
			//}}

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
			if( pEvent->GetEventItemID() != 0 )
			{
				std::set< int >::iterator sitItemIDInInventory = setItemID.find( pEvent->GetEventItemID() );
				if( sitItemIDInInventory == setItemID.end() )
					continue;
			}
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
			if( pEvent->IsNewUnitEvent() == true && bIsNewUnit == false  )
			{
				continue;
			}

			if( pEvent->IsNewUnitEvent2() == true && bIsNewUnit2 == false )
			{
				continue;
			}

			if( pEvent->IsCurrentUnitEvent() == true && bIsCurrentUnit == false)// bIsCurrentUnit = false
			{
				continue;
			}

			//START_LOG( cerr, L"�輮��_������ �����׽�Ʈ!" )
			//	<< BUILD_LOG( pEvent->GetScriptID() )
			//	<< BUILD_LOG( pEvent->IsNewUnitEvent() )
			//	<< BUILD_LOG( pEvent->IsNewUnitEvent2() )
			//	<< BUILD_LOG( pEvent->IsCurrentUnitEvent() )
			//	<< BUILD_LOG( bIsNewUnit )
			//	<< BUILD_LOG( bIsNewUnit2 )
			//	<< BUILD_LOG( bIsCurrentUnit )
			//	<< END_LOG;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

			// �̹� �Ϸ��� �̺�Ʈ��� �Ѿ��!
			if( setCompletedEvent.find( iEventUID ) != setCompletedEvent.end() )
				continue;

			//{{ 2012. 06. 05	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			// ���ϸ��� ���� ���� �̺�Ʈ�ΰ�?
			{
				int iItemID;

				if( SiKDailyGiftBoxManager()->IsDailyGiftBoxScriptID( pEvent->GetScriptID(), iItemID ) == true )
				{
					// �ش� ��Ű���� ������ �������� Ȯ���Ѵ�.
					std::pair<std::multimap<int, KDailyGiftBoxInfo>::const_iterator, std::multimap<int, KDailyGiftBoxInfo>::const_iterator> pairFindResult = mmapDailyGiftBoxList.equal_range( iItemID );
					for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
					{
						if( pairFindResult.first->second.m_iUnitUID != iUnitUID )
						{
							continue;
						}

						if( pairFindResult.first->second.m_iState != KDailyGiftBoxManager::PURCHASED_CONFIRM )
						{
							continue;
						}

						// �ð� �˻� ( �����ϰ��� 7�� �̳��� �͸� �־�� �Ѵ�. )
						CTime tPurchasedDate;
						if( KncUtil::ConvertStringToCTime( pairFindResult.first->second.m_wstrRegDate, tPurchasedDate ) == false )
						{
							continue;
						}

						CTime tAfterWeekFromPurchasedDate = CTime( tPurchasedDate.GetYear(), tPurchasedDate.GetMonth(), tPurchasedDate.GetDay(), 0, 0, 0 ) + CTimeSpan( 6, 23, 59, 59 );
						if( tAfterWeekFromPurchasedDate < CTime::GetCurrentTime() )
						{
							continue;
						}
						break;
					}

					if( pairFindResult.first == pairFindResult.second )
					{
						continue;
					}
				}
			}
#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}

			// 4-1. �������� �̺�Ʈ��� ������ �̺�Ʈ ����Ʈ�� Ȯ�� �� ���� �̺�Ʈ��� ä���ش�.
			mitET = mapEventTime.find( iEventUID );
			if( mitET == mapEventTime.end() )
			{
				//////////////////////////////////////////////////////////////////////////
				// �̺�Ʈ ���� ����!
				CTime tSetTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0 );

				std::pair< KUserConnectTimeEventInfo, KTimerManager > kEventTime;
				kEventTime.first.m_bInit			= true;				
				kEventTime.first.m_iEventID			= pEvent->GetEventID();
				kEventTime.first.m_wstrEventName	= pEvent->GetEventName();
				kEventTime.first.m_fEventTime		= pEvent->GetEventTime();
				kEventTime.first.m_kCTime			= tSetTime.GetTime();
				kEventTime.first.m_bGiveOnce		= pEvent->IsGiveOnce();
				//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
				kEventTime.first.m_bAccountEvent	= pEvent->IsAccountEvent();
#endif SERV_ACC_TIME_EVENT
				//}}
				//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
				kEventTime.first.m_bRepeatEvent	= pEvent->GetRepeatEvent();
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
				//}}

				//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
				kEventTime.first.m_iScriptID	= pEvent->GetScriptID();
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
				//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
				kEventTime.first.m_bRepeatRewardEvent = pEvent->IsRepeatRewardEvent();
				kEventTime.first.m_iBeforeEventID = pEvent->GetBeforeEventID();
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
				kEventTime.first.m_iEventItemID = pEvent->GetEventItemID();
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
				kEventTime.first.m_iCustomEventID = pEvent->GetCustomEventID();
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
				kEventTime.first.m_bOnlySteamUser = pEvent->IsOnlySteamUserEvent();
				kEventTime.first.m_bOnlyNotSteamUser = pEvent->IsOnlyNotSteamUserEvent();
#endif //SERV_STEAM_USER_CONNECT_EVENT

				kEventTime.second.restart(); // Ÿ�̸� �ʱ�ȭ
				mapEventTime.insert( std::make_pair( iEventUID, kEventTime ) );
				
				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				KTimeEventSimpleInfo kBeginEventInfo;
				kBeginEventInfo.m_iEventUID		= iEventUID;
				kBeginEventInfo.m_iEventID		= pEvent->GetEventID();
				kBeginEventInfo.m_wstrEventName = pEvent->GetEventName();
				kBeginEventInfo.m_fEventTime	= pEvent->GetEventTime();
				vecBeginEvent.push_back( kBeginEventInfo );
				//}}
			}
			else
			{
				// ĳ���͸� ó�� ������ ��Ȳ�̶� �̺�Ʈ �����Ͱ� �ʱ�ȭ �Ǿ� ���� �ʴٸ�..?
				if( mitET->second.first.m_bInit == false )
				{
					mitET->second.first.m_bInit			= true;
					mitET->second.first.m_wstrEventName	= pEvent->GetEventName();
					mitET->second.first.m_iEventID		= pEvent->GetEventID();
					mitET->second.first.m_fEventTime	= pEvent->GetEventTime();
					mitET->second.first.m_bGiveOnce		= pEvent->IsGiveOnce();
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					mitET->second.first.m_bAccountEvent	= pEvent->IsAccountEvent();
#endif SERV_ACC_TIME_EVENT
					//}}
					//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					mitET->second.first.m_bRepeatEvent	= pEvent->GetRepeatEvent();
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
					mitET->second.first.m_bRepeatRewardEvent = pEvent->IsRepeatRewardEvent();
					mitET->second.first.m_iBeforeEventID = pEvent->GetBeforeEventID();
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
					mitET->second.first.m_iEventItemID = pEvent->GetEventItemID();
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
					mitET->second.first.m_iCustomEventID = pEvent->GetCustomEventID();
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
					mitET->second.first.m_bOnlySteamUser = pEvent->IsOnlySteamUserEvent();
					mitET->second.first.m_bOnlyNotSteamUser = pEvent->IsOnlyNotSteamUserEvent();
#endif //SERV_STEAM_USER_CONNECT_EVENT
				}
			}
		}
		else
		{
			// 4-2. �̺�Ʈ �Ⱓ�� ����Ǿ����� ������ �̺�Ʈ�� ��� �����ų���� üũ�Ѵ�.
			if( !bDelFinishEvent  &&  !pEvent->IsDelEndEvent() )
				continue;

			// 4-3. ����� �̺�Ʈ�� ������ �̺�Ʈ ����Ʈ���� ã�Ƽ� �����Ѵ�.
			mitET = mapEventTime.find( iEventUID );
			if( mitET != mapEventTime.end() )
			{
				// �������� �̺�Ʈ ����Ʈ���� ����!
				mapEventTime.erase( iEventUID );

#ifdef SERV_ALLOW_EVENT_ERASE				
				if ( pEvent->IsOn() == true ) // 2013.03.15 lygan_������ // ���� �̺�Ʈ�� ���ư��ٰ� ���� ���ܼ� �ٷ� �� ��� DB���� off ó�� �Ȱ� �Ϸ��ʿ� ���� �ʴ´�. ���� ������ �ش� �̺�Ʈ�� �ٽ� ����� �ϱ� ����. ���� �� �ű� ����ϸ� �ѹ� �� ������ �ִ�.
				{
					// �Ϸ� �̺�Ʈ�� �߰�
					setCompletedEvent.insert( iEventUID );
				}
				
#else //SERV_ALLOW_EVENT_ERASE				
				// �Ϸ� �̺�Ʈ�� �߰�
				setCompletedEvent.insert( iEventUID );
#endif //SERV_ALLOW_EVENT_ERASE				
				
				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				vecEndEvent.push_back( KTimeEventSimpleInfo( iEventUID, pEvent->GetEventID() ) );
				//}}
			}
		}
	}
}

void KGameEventManager::CheckConnectTimeEvent( IN const u_char ucLevel,
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
											   IN OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapEventTime, 
											   IN OUT std::set< int >& setCompletedEvent,
											   OUT std::vector< KConnectTimeEventInfo >& vecUpdatedTime, 
											   OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
											   OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent
											   //{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
											   , IN const UidType& iUnitUID
											   , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
											   //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
											   , IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
											   , IN bool	bIsNewUnit
											   , IN bool	bIsNewUnit2
											   , IN bool	bIsCurrentUnit
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
											   )
{
	if( GetServerType() != ST_GAME )
		return;

	vecUpdatedTime.clear();
	vecBeginEvent.clear();
	vecEndEvent.clear();

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	CheckEnableConnectTimeEvent(
		ucLevel,
		bIsPcBang,
		iChannelCode,
		iBF_Team,
		cUnitType,
		mapEventTime,
		setCompletedEvent,
		vecBeginEvent,
		vecEndEvent,
		false
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

	CTime tCurrentTime = CTime::GetCurrentTime();
	
	std::vector< int > vecCompletedEvent;
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitET;

	for( mitET = mapEventTime.begin(); mitET != mapEventTime.end(); ++mitET )
	{
		const int iEventUID = mitET->first;
		KUserConnectTimeEventInfo& kEventInfo = mitET->second.first;
		KTimerManager& kEventTimer = mitET->second.second;

		// 1. �̹� ������ �޾Ҵ��� �˻�
		if( tCurrentTime.GetTime() <= kEventInfo.m_kCTime )
		{
			vecCompletedEvent.push_back( iEventUID );
			continue;
		}
		
		// 2. END_EVENT���� �̹� '�������� �̺�Ʈ'��.
		if( kEventInfo.m_fEventTime <= END_EVENT )
		{
			vecCompletedEvent.push_back( iEventUID );
			continue;
		}
		
		// 3. ��� �ð��� ��������..
		if( kEventTimer.elapsed() < ( kEventInfo.m_fEventTime * 60.0 ) )
			continue;

		kEventTimer.restart();

		// 4. ���� �ð� ����
		CTime tUpdateTime;
		if( kEventInfo.m_bGiveOnce )
		{
			tUpdateTime = tCurrentTime;
			tUpdateTime += CTimeSpan( 18250, 0, 0, 0 ); // �ѹ��� ��� �ϹǷ� 50���� ���Ѵ�.
		}
		else
		{
			tUpdateTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 23, 59, 59 ); // ������ �Ǳ������� ���� ����.
		}

		//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		if( kEventInfo.m_bRepeatEvent )
		{
			tUpdateTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), tCurrentTime.GetHour(), tCurrentTime.GetMinute(), 0 ); // �ٷ� ���� �� �ִ�
		}
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}

		kEventInfo.m_kCTime = tUpdateTime.GetTime();

		// 5. DB�� ������Ʈ
		KConnectTimeEventInfo kConnectTimeEvent;
		kConnectTimeEvent.m_iEventUID		= iEventUID;
		kConnectTimeEvent.m_wstrEventTime	= ( CStringW )tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		kConnectTimeEvent.m_bAccountEvent	= kEventInfo.m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
		//}}

		//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		kConnectTimeEvent.m_iScriptID		= kEventInfo.m_iScriptID;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}

		vecUpdatedTime.push_back( kConnectTimeEvent );

		// 6. Ŭ���̾�Ʈ�� ����Ǵ� �̺�Ʈ ������
		vecEndEvent.push_back( KTimeEventSimpleInfo( iEventUID, kEventInfo.m_iEventID ) );

		// 7. �Ϸ� �̺�Ʈ ����Ʈ�� �ֱ�
		//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		if( kEventInfo.m_bRepeatEvent == false )
		{
			vecCompletedEvent.push_back( iEventUID );
		}
		else
		{
			KTimeEventSimpleInfo kRepeatEvent;
			kRepeatEvent.m_iEventUID = iEventUID;
			kRepeatEvent.m_iEventID = kEventInfo.m_iEventID;
			kRepeatEvent.m_wstrEventName = kEventInfo.m_wstrEventName;
			kRepeatEvent.m_fEventTime = kEventInfo.m_fEventTime;
			vecBeginEvent.push_back( kRepeatEvent );

			mitET->second.second.restart();
		}
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}
	}

	std::vector< int >::const_iterator vit;
	for( vit = vecCompletedEvent.begin(); vit != vecCompletedEvent.end(); ++vit )
	{
		// �������� �̺�Ʈ ����Ʈ���� ����
		mapEventTime.erase( *vit );

		// �Ϸ� �̺�Ʈ ����Ʈ�� �߰�
		setCompletedEvent.insert( *vit );
	}
}
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}
//}}

//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT

bool KGameEventManager::GetCumulativeTimeEventReward( IN const int iEventUID, OUT int& iRewardID )
{
	std::map< int, KGameEvent* >::const_iterator mit = m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].find( iEventUID );
	if( mit == m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].end() )
		return false;

	if( mit->second == NULL )
		return false;

	if( mit->second->GetType() != KGameEvent::GET_CUMULATIVE_TIME )
		return false;

	KGameEventCumulativeTime* pEvent = static_cast<KGameEventCumulativeTime*>(mit->second);

	iRewardID = pEvent->GetRewardID();
	return true;
}

void KGameEventManager::CheckEnableCumulativeTimeEventForCharInit( IN const u_char ucLevel, 
																   IN const bool bIsPcBang, 
																   //{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
																   IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
																   //}}
																   //{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
																   IN ConnectEventFactorSet& kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
																   //}}
																   IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapEventTime, 
																   IN OUT std::set< int >& setCompleteEvent )
{
	std::vector< KTimeEventSimpleInfo > vecDummy1;
	std::vector< KTimeEventSimpleInfo > vecDummy2;
	std::vector< KUserCumulativeTimeEventInfo > vecCompleteEventInfo;

	//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	CheckEnableAndCompleteCumulativeTimeEvent( ucLevel, bIsPcBang, iChannelCode, kFactor, mapEventTime, setCompleteEvent, vecCompleteEventInfo, vecDummy1, vecDummy2 );
#else
	CheckEnableAndCompleteCumulativeTimeEvent( ucLevel, bIsPcBang, iChannelCode, mapEventTime, setCompleteEvent, vecCompleteEventInfo, vecDummy1, vecDummy2 );
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}
#else
	CheckEnableAndCompleteCumulativeTimeEvent( ucLevel, bIsPcBang, mapEventTime, setCompleteEvent, vecCompleteEventInfo, vecDummy1, vecDummy2 );
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//}}	

	// �ʱ�ȭ ���� ���� �̺�Ʈ�� �̹� ����� �̺�Ʈ�� �Ǵ��Ͽ� ����ó���Ѵ�.
	std::vector< int > vecEndEvent;

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = mapEventTime.begin(); mit != mapEventTime.end(); ++mit )
	{
		if( mit->second.first.m_bInit == false )
		{
			// ������ ����Ʈ�� ����
			vecEndEvent.push_back( mit->first );
		}
	}

	std::vector< int >::const_iterator vit;
	for( vit = vecEndEvent.begin(); vit != vecEndEvent.end(); ++vit )
	{
		mapEventTime.erase( *vit );
	}
}

void KGameEventManager::CheckEnableAndCompleteCumulativeTimeEvent( IN const u_char ucLevel,
																   IN const bool bIsPcBang, 
																   //{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
																   IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
																   //}}
																   //{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
																   IN ConnectEventFactorSet& kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
																   //}}
																   IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapEventTime,
																   IN OUT std::set< int >& setCompleteEvent,
																   OUT std::vector< KUserCumulativeTimeEventInfo >& vecCompleteEventInfo,
									   							   OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
									   							   OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent
									   							   )
{
	if( GetServerType() != ST_GAME )
		return;

	vecCompleteEventInfo.clear();
	vecBeginEvent.clear();
	vecEndEvent.clear();

	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, KGameEvent* >::const_iterator mit;
	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::iterator mitET;

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	for( mit = m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].begin(); mit != m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_CUMULATIVE_TIME )
			continue;

		const int iEventUID = mit->first;
		KGameEventCumulativeTime* pEvent = static_cast<KGameEventCumulativeTime*>(mit->second);

		// 3. �̺�Ʈ�� ����������..
		if( mit->second->IsEnable() 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			//{{ 2009. 10. 26  ������	PC���̺�Ʈ
			if( pEvent->IsPcBangEvent() == true  &&  bIsPcBang == false )
				continue;
			//}}

			//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			if( pEvent->GetChannelCode() != KNexonAccountInfo::CE_NONE )
			{
				if( pEvent->GetChannelCode() != iChannelCode )
					continue;
			}
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			//}}

			//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
			if( CheckWinterVacationEvent( iEventUID, pEvent->GetScriptID(), kFactor ) == false )
			{
				continue;
			}
#endif SERV_2012_WINTER_VACATION_EVENT
			//}}

			// �̹� �Ϸ��� �̺�Ʈ ���� �˻�!
			if( setCompleteEvent.find( iEventUID ) != setCompleteEvent.end() )
				continue;

			//{{ 2009. 12. 9  ������	���ӽð��̺�Ʈ
			//if( ucLevel < 10 ) // 10�������� ������ �̺�Ʈ ����ȵ�
			//	continue;
			//}}			

			// 4-1. �������� �̺�Ʈ��� ������ �̺�Ʈ ����Ʈ�� Ȯ�� �� ���� �̺�Ʈ��� ä���ش�.
			mitET = mapEventTime.find( iEventUID );
			if( mitET == mapEventTime.end() )
			{
				std::pair< KUserCumulativeTimeEventInfo, KTimerManager > kEventTime;
				kEventTime.first.m_bInit			= true; // ���� �ִ°��̱� ������ �ʱ�ȭ�Ѱ����� �Ѵ�.
				kEventTime.first.m_iEventUID		= iEventUID;
				kEventTime.first.m_iEventID			= pEvent->GetEventID();
				kEventTime.first.m_wstrEventName	= pEvent->GetEventName();
				kEventTime.first.m_fEventTime		= pEvent->GetEventTime();
				kEventTime.first.m_bDirectReward	= pEvent->IsDirectReward();
				//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
				kEventTime.first.m_bAccountEvent	= pEvent->IsAccountEvent();
#endif SERV_ACC_TIME_EVENT
				//}}
				kEventTime.second.restart(); // Ÿ�̸� �ʱ�ȭ
				mapEventTime.insert( std::make_pair( iEventUID, kEventTime ) );

				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				KTimeEventSimpleInfo kBeginEventInfo;
				kBeginEventInfo.m_iEventUID		= iEventUID;
				kBeginEventInfo.m_iEventID		= pEvent->GetEventID();
				kBeginEventInfo.m_wstrEventName = pEvent->GetEventName();
				kBeginEventInfo.m_fEventTime	= pEvent->GetEventTime();
				vecBeginEvent.push_back( kBeginEventInfo );
				//}}
			}
			else
			{
				// ĳ���͸� ó�� ������ ��Ȳ�̶� �̺�Ʈ �����Ͱ� �ʱ�ȭ �Ǿ� ���� �ʴٸ�..?
				if( mitET->second.first.m_bInit == false )
				{
					mitET->second.first.m_bInit			= true;
					mitET->second.first.m_iEventUID		= iEventUID;
					mitET->second.first.m_iEventID		= pEvent->GetEventID();
					mitET->second.first.m_wstrEventName	= pEvent->GetEventName();
					mitET->second.first.m_fEventTime	= pEvent->GetEventTime();
					mitET->second.first.m_bDirectReward	= pEvent->IsDirectReward();
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					mitET->second.first.m_bAccountEvent	= pEvent->IsAccountEvent();
#endif SERV_ACC_TIME_EVENT
					//}}
				}
				else
				{
					// �̺�Ʈ�� �Ϸ� �Ǿ��� üũ!
					if( static_cast<double>(mitET->second.first.m_fEventTime * 60.0 ) < mitET->second.second.elapsed() )
					{
						vecCompleteEventInfo.push_back( mitET->second.first );
					}
				}
			}
		}
		else
		{
			// 4-3. ����� �̺�Ʈ�� ������ �̺�Ʈ ����Ʈ���� ã�Ƽ� �����Ѵ�.
			mitET = mapEventTime.find( iEventUID );
			if( mitET != mapEventTime.end() )
			{
				mapEventTime.erase( iEventUID );
			
#ifdef SERV_ALLOW_EVENT_ERASE				
				if ( pEvent->IsOn() == true ) // 2013.06.11 lygan_������ // ���� �̺�Ʈ�� ���ư��ٰ� ���� ���ܼ� �ٷ� �� ��� DB���� off ó�� �Ȱ� �Ϸ��ʿ� ���� �ʴ´�. ���� ������ �ش� �̺�Ʈ�� �ٽ� ����� �ϱ� ����. ���� �� �ű� ����ϸ� �ѹ� �� ������ �ִ�.
				{
					// �Ϸ�� �̺�Ʈ�� �߰�
					setCompleteEvent.insert( iEventUID );
				}

#else //SERV_ALLOW_EVENT_ERASE				
				// �Ϸ�� �̺�Ʈ�� �߰�
				setCompleteEvent.insert( iEventUID );
#endif //SERV_ALLOW_EVENT_ERASE		

				//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
				vecEndEvent.push_back( KTimeEventSimpleInfo( iEventUID, pEvent->GetEventID() ) );
				//}}
			}
		}
	}
}

void KGameEventManager::CheckCumulativeTimeEvent( IN const u_char ucLevel,
												  IN const bool bIsPcBang, 
												  //{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
												  IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
												  //}}
												  //{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
												  IN ConnectEventFactorSet& kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
												  //}}
												  IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapEventTime,
												  IN OUT std::set< int >& setCompleteEvent,
												  IN OUT std::set< int >& setDirectRewardEvent,
								 				  OUT std::vector< KCumulativeTimeEventInfo >& vecUpdatedTime,
								 				  OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
								 				  OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent
								 				  )
{
	if( GetServerType() != ST_GAME )
		return;

	vecUpdatedTime.clear();
	vecBeginEvent.clear();
	vecEndEvent.clear();

	std::vector< KUserCumulativeTimeEventInfo > vecCompleteEventInfo;

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	CheckEnableAndCompleteCumulativeTimeEvent(
		ucLevel
		, bIsPcBang
		, iChannelCode
		//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		, kFactor
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		, mapEventTime
		, setCompleteEvent
		, vecCompleteEventInfo
		, vecBeginEvent
		, vecEndEvent
		);
#else
	CheckEnableAndCompleteCumulativeTimeEvent( ucLevel, bIsPcBang, mapEventTime, setCompleteEvent, vecCompleteEventInfo, vecBeginEvent, vecEndEvent );
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//}}	

	std::vector< KUserCumulativeTimeEventInfo >::const_iterator vit;
	for( vit = vecCompleteEventInfo.begin(); vit != vecCompleteEventInfo.end(); ++vit )
	{
		// ���� ����޴� �̺�Ʈ���� Ȯ���Ѵ�
		if( vit->m_bDirectReward )
		{
			// �̹� ������ ���
			if( setDirectRewardEvent.find( vit->m_iEventUID ) != setDirectRewardEvent.end() )
				continue;

			setDirectRewardEvent.insert( vit->m_iEventUID );

			// Ŭ���̾�Ʈ�� �̺�Ʈ ������ �˸��� ���� ��´�.
			vecEndEvent.push_back( KTimeEventSimpleInfo( vit->m_iEventUID, vit->m_iEventID ) );
			continue;
		}

		// ���� ���� ����
		KCumulativeTimeEventInfo kCumulativeTimeEvent;
		kCumulativeTimeEvent.m_iEventUID		= vit->m_iEventUID;
		kCumulativeTimeEvent.m_iCumulativeTime	= static_cast<int>(vit->m_fEventTime); // ����޴� Ÿ���� DB�� ������Ʈ
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		kCumulativeTimeEvent.m_bAccountEvent	= vit->m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
		//}}
		vecUpdatedTime.push_back( kCumulativeTimeEvent );

		// �̺�Ʈ ����!
		mapEventTime.erase( vit->m_iEventUID );

		// �Ϸ�� �̺�Ʈ�� �߰�
		setCompleteEvent.insert( vit->m_iEventUID );

		//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
		vecEndEvent.push_back( KTimeEventSimpleInfo( vit->m_iEventUID, vit->m_iEventID ) );
		//}}
	}
}

void KGameEventManager::GetDBUpdateInfo( IN const std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime, 
										 IN const std::set< int >& setCompletedEvent, 
										 OUT std::vector< KCumulativeTimeEventInfo >& vecEventTime )
{
	vecEventTime.clear();

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = mapCumulativeEventTime.begin(); mit != mapCumulativeEventTime.end(); ++mit )
	{
		// �ʱ�ȭ�� ���� ���� �̺�Ʈ�� �̹� ����� �̺�Ʈ�̹Ƿ� DB�� ������Ʈ �� �ʿ䰡 ����.
		if( mit->second.first.m_bInit == false )
			continue;

		// �Ϸ��� �̺�Ʈ�� DB�� ������Ʈ�� �ʿ䰡 ����.
		if( setCompletedEvent.find( mit->first ) != setCompletedEvent.end() )
			continue;

		const KTimerManager& kEventTimer = mit->second.second;

		KCumulativeTimeEventInfo kInfo;
		kInfo.m_iEventUID = mit->first;
		kInfo.m_iCumulativeTime = static_cast<int>( kEventTimer.elapsed() / 60.0 );
		vecEventTime.push_back( kInfo );
	}
}

void KGameEventManager::GetNowProcessingTimeEvent( IN const std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime,
												   IN const std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime,
												   IN const std::set< int >& setCompletedEvent,
												   OUT std::vector< KTimeEventSimpleInfo >& vecTimeEvent,
												   IN bool bAccountTimeEvent )
{
	vecTimeEvent.clear();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = mapConnectEventTime.begin(); mit != mapConnectEventTime.end(); ++mit )
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
	for( mitCT = mapCumulativeEventTime.begin(); mitCT != mapCumulativeEventTime.end(); ++mitCT )
	{
		// �Ϸ��� �̺�Ʈ�� Ŭ���̾�Ʈ�� ���ʿ� ����.
		if( setCompletedEvent.find( mitCT->first ) != setCompletedEvent.end() )
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

void KGameEventManager::DeleteDirectRewardAndGetDBUpdateInfo( IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime,
										 					  IN OUT std::set< int >& setCompletedEvent,
															  IN OUT std::set< int >& setDirectRewardEvent,
															  OUT std::vector< KCumulativeTimeEventInfo >& vecCompleteEvent )
{
	vecCompleteEvent.clear();

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::iterator mit;
	std::set< int >::const_iterator sit;
	for( sit = setDirectRewardEvent.begin(); sit != setDirectRewardEvent.end(); ++sit )
	{
		mit = mapCumulativeEventTime.find( *sit );
		if( mit == mapCumulativeEventTime.end() )
			continue;

		KUserCumulativeTimeEventInfo& kCumulativeEventInfo = mit->second.first;

		// DB�� ������Ʈ�� ���� ����
		KCumulativeTimeEventInfo kInfo;
		kInfo.m_iEventUID = kCumulativeEventInfo.m_iEventUID;
		kInfo.m_iCumulativeTime = static_cast<int>(kCumulativeEventInfo.m_fEventTime);
		vecCompleteEvent.push_back( kInfo );

		// �������� �̺�Ʈ���� ����
		mapCumulativeEventTime.erase( mit );

		// �Ϸ� �̺�Ʈ�� �߰�
		setCompletedEvent.insert( *sit );
	}

	// DirectReward �ʱ�ȭ
	setDirectRewardEvent.clear();
}

#endif CUMULATIVE_TIME_EVENT
//}}

//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
void KGameEventManager::RefreshEventScript()
{
	START_LOG( cout, L"�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ ����" );

	for( int iGET = 0; iGET < KGameEvent::GET_MAX; ++iGET )
	{
		switch( iGET )
		{
		case KGameEvent::GET_ITEM_DROP:
		case KGameEvent::GET_REWARD:
		case KGameEvent::GET_CONNECT_TIME:
		case KGameEvent::GET_CUMULATIVE_TIME:
			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		case KGameEvent::GET_HENIR_REWARD:
#endif SERV_NEW_HENIR_TEST
			//}}
			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
		case KGameEvent::GET_QUEST_ITEM_DROP:
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
		case KGameEvent::GET_DEFENSE_DUNGEON:
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef SERV_LEVEL_UP_EVENT// �۾���¥: 2013-06-24	// �ڼ���
		case KGameEvent::GET_LEVEL_UP:
#endif // SERV_LEVEL_UP_EVENT

#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-09	// �ڼ���
		case KGameEvent::GET_JUMPING_CHARACTER:
#endif // SERV_JUMPING_CHARACTER
			break;

		default:
			continue;
		}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_NEW_EVENT_TYPES
		// ���� �̺�Ʈ�� ���°� ���� �� �� ���� �۾��� �����ϱ� ���� �ڵ�
		++m_iAppliedEventTick;
#endif SERV_NEW_EVENT_TYPES
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

		std::map< int, KGameEvent* >::iterator mit;
		for( mit = m_mapIngEventData[iGET].begin(); mit != m_mapIngEventData[iGET].end(); ++mit )
		{
			if( mit->second == NULL )
				continue;

			switch( mit->second->GetType() )
			{
				//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_PC_BANG_DROP_EVENT
			case KGameEvent::GET_ITEM_DROP:
				{
					KGameEventItemDrop* pEvent = static_cast<KGameEventItemDrop*>(mit->second);

					//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
					if( pEvent->GetDropRate() != pEventData->m_fDropRate  ||
						pEvent->IsWithPlayPcBang() != pEventData->m_bWithPlayPcBang )
					{
						pEvent->SetDropRate( pEventData->m_fDropRate );
						pEvent->SetWithPlayPcBang( pEventData->m_bWithPlayPcBang );

						START_LOG( cout, L"��� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetDropRate() )
							<< BUILD_LOG( pEvent->IsWithPlayPcBang() );
					}
#else // SERV_DROP_EVENT_RENEWAL
					if( pEvent->GetDropCount() != pEventData->m_iDropCount  ||
						pEvent->IsWithPlayPcBang() != pEventData->m_bWithPlayPcBang )
					{
						pEvent->SetDropCount( pEventData->m_iDropCount );
						pEvent->SetWithPlayPcBang( pEventData->m_bWithPlayPcBang );

						START_LOG( cout, L"��� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetDropCount() )
							<< BUILD_LOG( pEvent->IsWithPlayPcBang() );
					}
#endif // SERV_DROP_EVENT_RENEWAL
				}
				break;
#endif SERV_PC_BANG_DROP_EVENT
				//}}

			case KGameEvent::GET_REWARD:
				{
					KGameEventReward* pEvent = static_cast<KGameEventReward*>(mit->second);

					//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

					if( pEvent->GetEXPRate()			!= pEventData->m_fEXPRate			||
						pEvent->GetVPRate()				!= pEventData->m_fVPRate			||
						pEvent->GetEDRate()				!= pEventData->m_fEDRate			||
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
						pEvent->GetRankingPointRate()	!= pEventData->m_fRankingPointRate  ||
#endif	// SERV_RANKING_POINT_EVENT
						pEvent->GetDungeonID()			!= pEventData->m_iDungeonID			||
						pEvent->GetUnitLevel()			!= pEventData->m_iUnitLevel			||
						pEvent->GetUnitType()			!= pEventData->m_cUnitType			||
						pEvent->GetUnitClass()			!= pEventData->m_cUnitClass			||
						//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
						pEvent->GetServerGroupID()		!= pEventData->m_iServerGroup		||
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
						//}}
						pEvent->GetPartyNum()			!= pEventData->m_iPartyNum

#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
						|| pEvent->IsPcBangEvent()		!= pEventData->m_bPcBangEvent
#endif // SERV_PCBANG_USER_REWARD_EVENT
						)
					{
						//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
						pEvent->SetEXPRate( pEventData->m_fEXPRate );
						pEvent->SetVPRate( pEventData->m_fVPRate );
						pEvent->SetEDRate( pEventData->m_fEDRate );
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
						pEvent->SetRankingPointRate( pEventData->m_fRankingPointRate );
#endif	// SERV_RANKING_POINT_EVENT
						pEvent->SetDungeonID( pEventData->m_iDungeonID );
						pEvent->SetUnitLevel( pEventData->m_iUnitLevel );
						pEvent->SetUnitType( pEventData->m_cUnitType );
						pEvent->SetUnitClass( pEventData->m_cUnitClass );
						pEvent->SetPartyNum( pEventData->m_iPartyNum );
						pEvent->SetServerGroupID( pEventData->m_iServerGroup );
						//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
						pEvent->SetAccumulateEXP( pEventData->m_bAccumulateEXP );
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
						//}}

#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
						pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
#endif // SERV_PCBANG_USER_REWARD_EVENT

						pEvent->Dump();

						START_LOG( cout, L"���� ���� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetEXPRate() )
							<< BUILD_LOG( pEvent->GetVPRate() )
							<< BUILD_LOG( pEvent->GetEDRate() )
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
							<< BUILD_LOG( pEvent->GetRankingPointRate() )
#endif	// SERV_RANKING_POINT_EVENT
							<< BUILD_LOG( pEvent->GetDungeonID() )
							<< BUILD_LOG( pEvent->GetUnitLevel() )
							<< BUILD_LOGc( pEvent->GetUnitType() )
							<< BUILD_LOGc( pEvent->GetUnitClass() )
							<< BUILD_LOG( pEvent->GetPartyNum() )
							<< BUILD_LOG( pEvent->GetServerGroupID() )
							//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
							<< BUILD_LOG( pEvent->IsAccumulateEXP() )
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
							//}}
#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
							<< BUILD_LOG( pEvent->IsPcBangEvent() )
#endif // SERV_PCBANG_USER_REWARD_EVENT
							;
#else
						//�̺�Ʈ ��������..
						pEvent->SetData( pEventData->m_fEXPRate, 
							pEventData->m_fVPRate, 
							pEventData->m_fEDRate, 
							pEventData->m_iDungeonID, 
							pEventData->m_iUnitLevel, 
							pEventData->m_cUnitType, 
							pEventData->m_cUnitClass, 
							pEventData->m_iPartyNum
							);

						START_LOG( cout, L"���� ���� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetEXPRate() )
							<< BUILD_LOG( pEvent->GetVPRate() )
							<< BUILD_LOG( pEvent->GetEDRate() )
							<< BUILD_LOG( pEvent->GetDungeonID() )
							<< BUILD_LOG( pEvent->GetUnitLevel() )
							<< BUILD_LOGc( pEvent->GetUnitType() )
							<< BUILD_LOGc( pEvent->GetUnitClass() )
							<< BUILD_LOG( pEvent->GetPartyNum() );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
						//}}
					}
				}
				break;

			case KGameEvent::GET_CONNECT_TIME:
				{
					KGameEventConnectTime* pEvent = static_cast<KGameEventConnectTime*>(mit->second);

					//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

					if( pEvent->GetEventTime() != pEventData->m_fEventTime  ||
						pEvent->GetRewardID() != pEventData->m_iEventReward  ||
						pEvent->IsGiveOnce() != pEventData->m_bGiveOnce  ||
						pEvent->IsDelEndEvent() != pEventData->m_bDelEndEvent  ||
						pEvent->IsPcBangEvent() != pEventData->m_bPcBangEvent
						//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
						|| pEvent->IsAccountEvent() != pEventData->m_bAccountEvent
#endif SERV_ACC_TIME_EVENT
						//}}
						//{{ 2009. 12. 16  ������	ĳ���� ������ ���� �߰�
						|| pEvent->GetUnitLevel() != pEventData->m_iUnitLevel
						//}}
						//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
						|| pEvent->GetRepeatEvent() != pEventData->m_bRepeatEvent
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
						//}}
						//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK						
						|| pEvent->GetMinMaxRewardSize() != pEventData->m_mapMinMaxReward.size()
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
						//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
						|| pEvent->IsRepeatRewardEvent() != pEventData->m_bRepeatRewardEvent
						|| pEvent->GetBeforeEventID() != ( pEventData->m_iBeforeEventID )
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
						|| pEvent->GetEventItemID() != pEventData->m_iEventItemID
#endif
#ifdef SERV_CUSTOM_CONNECT_EVENT
						|| pEvent->GetCustomEventID() != ( pEventData->m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
						|| pEvent->IsOnlySteamUserEvent() != ( pEventData->m_bOnlySteamUser )
						|| pEvent->IsOnlyNotSteamUserEvent() != ( pEventData->m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_CRITERION_DATE_EVENT// �۾���¥: 2013-06-26	// �ڼ���
						|| ( pEvent->GetCriterionUserType() != pEventData->m_byteCriterionUserType )
						|| ( pEvent->GetCriterionPresentUserType() != pEventData->m_byteCriterionPresentUserType )
						|| ( pEvent->GetCriterionDateBegin() != pEventData->m_tCriterionDateBegin )
						|| ( pEvent->GetCriterionDateEnd() != pEventData->m_tCriterionDateEnd )
#endif // SERV_CRITERION_DATE_EVENT
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
						|| ( pEvent->GetUnitClassLevel() != pEventData->m_iUnitClassLevel )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
						)
					{
						//�̺�Ʈ ��������..
						pEvent->SetEventTime( pEventData->m_fEventTime );
						pEvent->SetRewardID( pEventData->m_iEventReward );
						pEvent->SetGiveOnce( pEventData->m_bGiveOnce );
						pEvent->SetDelEndEvent( pEventData->m_bDelEndEvent );
						//{{ 2009. 10. 26  ������	PC���̺�Ʈ
						pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
						//}}
						//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
						pEvent->SetAccountEvent( pEventData->m_bAccountEvent );
#endif SERV_ACC_TIME_EVENT
						//}}
						//{{ 2009. 12. 16  ������	ĳ���� ������ ���� �߰�
						pEvent->SetUnitLevel( pEventData->m_iUnitLevel );
						//}}
						//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
						pEvent->SetMinMaxReward( pEventData->m_mapMinMaxReward );
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
						//}}
						//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
						pEvent->SetRepeatEvent( pEventData->m_bRepeatEvent );
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
						//}}
						//{{ 2012. 12. 25	�ڼ���	Ư�� ���� ���� ���� �̺�Ʈ
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
						pEvent->SetEventGroupID( pEventData->m_iEventGroupID );
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
						//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
						pEvent->SetRepeatRewardEvent(pEventData->m_bRepeatRewardEvent);
						pEvent->SetBeforeEventID(pEventData->m_iBeforeEventID );
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
						pEvent->SetEventItemID( pEventData->m_iEventItemID );
#endif
#ifdef SERV_CUSTOM_CONNECT_EVENT
						pEvent->SetCustomEventID(pEventData->m_iCustomEventID);
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
						pEvent->SetOnlySteamUserEvent( pEventData->m_bOnlySteamUser );
						pEvent->SetOnlyNotSteamUserEvent( pEventData->m_bOnlyNotSteamUser );
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_CRITERION_DATE_EVENT// �۾���¥: 2013-06-26	// �ڼ���
						pEvent->SetCriterionUserType( pEventData->m_byteCriterionUserType );
						pEvent->SetCriterionPresentUserType( pEventData->m_byteCriterionPresentUserType );
						pEvent->SetCriterionDateBegin( pEventData->m_tCriterionDateBegin );
						pEvent->SetCriterionDateEnd( pEventData->m_tCriterionDateEnd );
#endif // SERV_CRITERION_DATE_EVENT
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
						pEvent->SetUnitClassLevel( pEventData->m_iUnitClassLevel );
#endif SERV_UNIT_CLASS_LEVEL_EVENT

						START_LOG( cout, L"���� �ð� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetEventTime() )
							<< BUILD_LOG( pEvent->GetRewardID() )
							<< BUILD_LOG( pEvent->IsGiveOnce() )
							<< BUILD_LOG( pEvent->IsDelEndEvent() )
							<< BUILD_LOG( pEvent->IsPcBangEvent() )
							<< BUILD_LOG( pEvent->IsAccountEvent() )
							<< BUILD_LOG( pEvent->GetUnitLevel() )
							//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
							<< BUILD_LOG( pEvent->GetMinMaxRewardSize() )
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
							//}}
							//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
							<< BUILD_LOG( pEvent->GetRepeatEvent() )
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
							//}}
							//{{ 2012. 12. 25	�ڼ���	Ư�� ���� ���� ���� �̺�Ʈ
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
							<< BUILD_LOG( pEvent->GetEventGroupID() )
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
							//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
							<< BUILD_LOG( pEvent->IsRepeatRewardEvent() )
							<< BUILD_LOG( pEvent->GetBeforeEventID() )
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
							<< BUILD_LOG( pEvent->GetEventItemID() )
#endif
#ifdef SERV_CUSTOM_CONNECT_EVENT
							<< BUILD_LOG( pEvent->GetCustomEventID() )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
							<< BUILD_LOG( pEvent->IsOnlySteamUserEvent() )
							<< BUILD_LOG( pEvent->IsOnlyNotSteamUserEvent() )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_CRITERION_DATE_EVENT// �۾���¥: 2013-06-26	// �ڼ���
							<< BUILD_LOG( pEvent->GetCriterionUserType() )
							<< BUILD_LOG( pEvent->GetCriterionPresentUserType() )
							<< BUILD_LOG( pEvent->GetCriterionDateBegin().GetTime() )
							<< BUILD_LOG( pEvent->GetCriterionDateEnd().GetTime() )
#endif // SERV_CRITERION_DATE_EVENT
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
							<< BUILD_LOG( pEvent->GetUnitClassLevel() )
#endif SERV_UNIT_CLASS_LEVEL_EVENT

							<< END_LOG;
							;
					}
				}
				break;

				//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
			case KGameEvent::GET_CUMULATIVE_TIME:
				{
					KGameEventCumulativeTime* pEvent = static_cast<KGameEventCumulativeTime*>(mit->second);

					//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

					if( pEvent->GetEventTime() != pEventData->m_fEventTime  ||
						pEvent->GetRewardID() != pEventData->m_iEventReward  ||
						pEvent->IsPcBangEvent() != pEventData->m_bPcBangEvent  ||
						pEvent->IsDirectReward() != pEventData->m_bDirectReward
						//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
						|| pEvent->IsAccountEvent() != pEventData->m_bAccountEvent
#endif SERV_ACC_TIME_EVENT
						//}}
						)
					{
						//�̺�Ʈ ��������..
						pEvent->SetEventTime( pEventData->m_fEventTime );
						pEvent->SetRewardID( pEventData->m_iEventReward );
						//{{ 2009. 10. 26  ������	PC���̺�Ʈ
						pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
						//}}
						//{{ 2009. 12. 8  ������	���ӽð��̺�Ʈ
						pEvent->SetDirectReward( pEventData->m_bDirectReward );
						//}}
						//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
						pEvent->SetAccountEvent( pEventData->m_bAccountEvent );
#endif SERV_ACC_TIME_EVENT
						//}}

						START_LOG( cout, L"���� �ð� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetEventTime() )
							<< BUILD_LOG( pEvent->GetRewardID() )
							<< BUILD_LOG( pEvent->IsDirectReward() )
							<< BUILD_LOG( pEvent->IsPcBangEvent() )
							<< BUILD_LOG( pEvent->IsAccountEvent() )
							<< END_LOG;
					}
				}
				break;
#endif CUMULATIVE_TIME_EVENT
				//}}

				//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
			case KGameEvent::GET_HENIR_REWARD:
				{
					KGameEventHenirReward* pEvent = static_cast<KGameEventHenirReward*>(mit->second);
					//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

					if( pEvent->GetHenirRewardEventCount() != pEventData->m_iHenirRewardEventCount || 
						//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
						pEvent->GetServerGroupID() != pEventData->m_iServerGroup  ||
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
						//}}
						pEvent->GetHenirRewardUnLimited() != pEventData->m_bHenirRewardUnLimited
#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
						|| pEvent->GetPcBangEvent() != pEventData->m_bPcBangEvent
						|| pEvent->GetAccumulate() != pEventData->m_bAccumulate
#endif // SERV_HENIR_REWARD_EVENT
						)
					{
						//�̺�Ʈ ��������..
						pEvent->SetHenirRewardEventCount( pEventData->m_iHenirRewardEventCount );
						pEvent->SetHenirRewardUnLimited( pEventData->m_bHenirRewardUnLimited );
						//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
						pEvent->SetServerGroupID( pEventData->m_iServerGroup );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
						//}}

#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
						pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
						pEvent->SetAccumulate( pEventData->m_bAccumulate );
#endif // SERV_HENIR_REWARD_EVENT
					
						START_LOG( cout, L"���� �ð� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetHenirRewardEventCount() )
							<< BUILD_LOG( pEvent->GetHenirRewardUnLimited() )
							//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
							<< BUILD_LOG( pEvent->GetServerGroupID() )
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
							//}}
#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
							<< BUILD_LOG( pEvent->GetPcBangEvent() )
							<< BUILD_LOG( pEvent->GetAccumulate() )
#endif // SERV_HENIR_REWARD_EVENT
							;
					}
				}
				break;
#endif SERV_NEW_HENIR_TEST
				//}}

				//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			case KGameEvent::GET_QUEST_ITEM_DROP:
				{
					KGameEventQuestItemDrop* pEvent = static_cast<KGameEventQuestItemDrop*>(mit->second);

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM


					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

					if( pEvent->GetQuestItemDropRate() != pEventData->m_fQuestItemDropRate )
					{
						pEvent->SetQuestItemDropRate( pEventData->m_fQuestItemDropRate );

						START_LOG( cout, L"��� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetQuestItemDropRate() );
					}
				}
				break;
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
			case KGameEvent::GET_DEFENSE_DUNGEON:
				{
					KGameEventDefenseDungeon* pEvent = static_cast<KGameEventDefenseDungeon*>( mit->second );
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
					const EVENT_DATA* pEventData = GetTotalEventData( pEvent->GetScriptID() );
#else //SERV_EVENT_DB_CONTROL_SYSTEM					
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM	
					

					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}

					if( pEvent->GetDefenseDungeonOpenRate() != pEventData->m_fDefenseDungeonOpenRate )
					{
						pEvent->SetDefenseDungeonOpenRate( pEventData->m_fDefenseDungeonOpenRate );

						START_LOG( cout, L"����� �� ���� Ȯ�� �̺�Ʈ ��ũ��Ʈ ����" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< BUILD_LOG( pEvent->GetDefenseDungeonOpenRate() );
					}
				}
				break;
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef SERV_LEVEL_UP_EVENT// �۾���¥: 2013-06-24	// �ڼ���
			case KGameEvent::GET_LEVEL_UP:
#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-09	// �ڼ���
			case KGameEvent::GET_JUMPING_CHARACTER:
#endif // SERV_JUMPING_CHARACTER
				{
					const KGameEvent* pEvent = mit->second;
					const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );

					if( pEventData == NULL )
					{
						START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
							<< BUILD_LOG( mit->second->GetUID() )
							<< BUILD_LOG( mit->second->GetType() )
							<< BUILD_LOG( pEvent->GetEventName() )
							<< BUILD_LOG( pEvent->GetScriptID() )
							<< END_LOG;
						break;
					}
				}
				break;
#endif // SERV_LEVEL_UP_EVENT
			}
		}
	}
}
#endif SERV_EVENT_SCRIPT_REFRESH
//}}
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
void KGameEventManager::RefreshEventData()
{
	for( int iGET = 0; iGET < KGameEvent::GET_MAX; ++iGET )
	{
		switch( iGET )
		{
		case KGameEvent::GET_ITEM_DROP:
		case KGameEvent::GET_REWARD:
		case KGameEvent::GET_CONNECT_TIME:
		case KGameEvent::GET_CUMULATIVE_TIME:
			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		case KGameEvent::GET_HENIR_REWARD:
#endif SERV_NEW_HENIR_TEST
			//}}
			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
		case KGameEvent::GET_QUEST_ITEM_DROP:
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
			break;

		default:
			continue;
		}

		std::map< int, KGameEvent* >::iterator mit;
		for( mit = m_mapIngEventData[iGET].begin(); mit != m_mapIngEventData[iGET].end(); ++mit )
		{
			if( mit->second == NULL )
				continue;

			switch( mit->second->GetType() )
			{
				//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_PC_BANG_DROP_EVENT
		case KGameEvent::GET_ITEM_DROP:
			{
				KGameEventItemDrop* pEvent = static_cast<KGameEventItemDrop*>(mit->second);

				//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
				const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				//}}
				if( pEventData == NULL )
				{
					START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< END_LOG;
					break;
				}

				if( pEvent->GetDropCount() != pEventData->m_iDropCount  ||
					pEvent->IsWithPlayPcBang() != pEventData->m_bWithPlayPcBang )
				{
					pEvent->SetDropCount( pEventData->m_iDropCount );
					pEvent->SetWithPlayPcBang( pEventData->m_bWithPlayPcBang );

					START_LOG( cout, L"��� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetDropCount() )
						<< BUILD_LOG( pEvent->IsWithPlayPcBang() );
				}
			}
			break;
#endif SERV_PC_BANG_DROP_EVENT
			//}}

		case KGameEvent::GET_REWARD:
			{
				KGameEventReward* pEvent = static_cast<KGameEventReward*>(mit->second);

				//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
				const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				//}}
				if( pEventData == NULL )
				{
					START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< END_LOG;
					break;
				}

				if( pEvent->GetEXPRate() != pEventData->m_fEXPRate  ||
					pEvent->GetVPRate() != pEventData->m_fVPRate  ||
					pEvent->GetEDRate() != pEventData->m_fEDRate  ||
					pEvent->GetDungeonID() != pEventData->m_iDungeonID  ||
					pEvent->GetUnitLevel() != pEventData->m_iUnitLevel  ||
					pEvent->GetUnitType() != pEventData->m_cUnitType  ||
					pEvent->GetUnitClass() != pEventData->m_cUnitClass  ||
					//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
					pEvent->GetServerGroupID() != pEventData->m_iServerGroup  ||
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
					//}}
					pEvent->GetPartyNum() != pEventData->m_iPartyNum )
				{
					//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
					pEvent->SetEXPRate( pEventData->m_fEXPRate );
					pEvent->SetVPRate( pEventData->m_fVPRate );
					pEvent->SetEDRate( pEventData->m_fEDRate );
					pEvent->SetDungeonID( pEventData->m_iDungeonID );
					pEvent->SetUnitLevel( pEventData->m_iUnitLevel );
					pEvent->SetUnitType( pEventData->m_cUnitType );
					pEvent->SetUnitClass( pEventData->m_cUnitClass );
					pEvent->SetPartyNum( pEventData->m_iPartyNum );
					pEvent->SetServerGroupID( pEventData->m_iServerGroup );
					//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
					pEvent->SetAccumulateEXP( pEventData->m_bAccumulateEXP );
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
					//}}
					pEvent->Dump();

					START_LOG( cout, L"���� ���� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetEXPRate() )
						<< BUILD_LOG( pEvent->GetVPRate() )
						<< BUILD_LOG( pEvent->GetEDRate() )
						<< BUILD_LOG( pEvent->GetDungeonID() )
						<< BUILD_LOG( pEvent->GetUnitLevel() )
						<< BUILD_LOGc( pEvent->GetUnitType() )
						<< BUILD_LOGc( pEvent->GetUnitClass() )
						<< BUILD_LOG( pEvent->GetPartyNum() )
						<< BUILD_LOG( pEvent->GetServerGroupID() )
						//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
						<< BUILD_LOG( pEvent->GetAccumulateEXP() )
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
						//}}
						;
#else
					//�̺�Ʈ ��������..
					pEvent->SetData( pEventData->m_fEXPRate, 
						pEventData->m_fVPRate, 
						pEventData->m_fEDRate, 
						pEventData->m_iDungeonID, 
						pEventData->m_iUnitLevel, 
						pEventData->m_cUnitType, 
						pEventData->m_cUnitClass, 
						pEventData->m_iPartyNum
						);

					START_LOG( cout, L"���� ���� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetEXPRate() )
						<< BUILD_LOG( pEvent->GetVPRate() )
						<< BUILD_LOG( pEvent->GetEDRate() )
						<< BUILD_LOG( pEvent->GetDungeonID() )
						<< BUILD_LOG( pEvent->GetUnitLevel() )
						<< BUILD_LOGc( pEvent->GetUnitType() )
						<< BUILD_LOGc( pEvent->GetUnitClass() )
						<< BUILD_LOG( pEvent->GetPartyNum() );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
					//}}
				}
			}
			break;

		case KGameEvent::GET_CONNECT_TIME:
			{
				KGameEventConnectTime* pEvent = static_cast<KGameEventConnectTime*>(mit->second);

				//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
				const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				//}}
				if( pEventData == NULL )
				{
					START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< END_LOG;
					break;
				}

				if( pEvent->GetEventTime() != pEventData->m_fEventTime  ||
					pEvent->GetRewardID() != pEventData->m_iEventReward  ||
					pEvent->IsGiveOnce() != pEventData->m_bGiveOnce  ||
					pEvent->IsDelEndEvent() != pEventData->m_bDelEndEvent  ||
					pEvent->IsPcBangEvent() != pEventData->m_bPcBangEvent  ||
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					pEvent->IsAccountEvent() != pEventData->m_bAccountEvent  ||
#endif SERV_ACC_TIME_EVENT
					//}}
					//{{ 2009. 12. 16  ������	ĳ���� ������ ���� �߰�
					pEvent->GetUnitLevel() != pEventData->m_iUnitLevel  ||
					//}}
					//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					pEvent->GetRepeatEvent() != pEventData->m_bRepeatEvent  ||
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					//}}
					//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK						
					pEvent->GetMinMaxRewardSize() != pEventData->m_mapMinMaxReward.size() 
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
					//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
					|| pEvent->IsRepeatRewardEvent() != pEventData->m_bRepeatRewardEvent
					|| pEvent->GetBeforeEventID() != ( pEventData->m_iBeforeEventID )
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_CUSTOM_CONNECT_EVENT
					|| pEvent->GetCustomEventID() != ( pEventData->m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
					|| pEvent->IsOnlySteamUserEvent() != ( pEventData->m_bOnlySteamUser )
					|| pEvent->IsOnlyNotSteamUserEvent() != ( pEventData->m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
					|| pEvent->GetUnitClassLevel() != ( pEventData->m_iUnitClassLevel )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
					)
				{
					//�̺�Ʈ ��������..
					pEvent->SetEventTime( pEventData->m_fEventTime );
					pEvent->SetRewardID( pEventData->m_iEventReward );
					pEvent->SetGiveOnce( pEventData->m_bGiveOnce );
					pEvent->SetDelEndEvent( pEventData->m_bDelEndEvent );
					//{{ 2009. 10. 26  ������	PC���̺�Ʈ
					pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
					//}}
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					pEvent->SetAccountEvent( pEventData->m_bAccountEvent );
#endif SERV_ACC_TIME_EVENT
					//}}
					//{{ 2009. 12. 16  ������	ĳ���� ������ ���� �߰�
					pEvent->SetUnitLevel( pEventData->m_iUnitLevel );
					//}}
					//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
					pEvent->SetMinMaxReward( pEventData->m_mapMinMaxReward );
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
					//}}
					//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					pEvent->SetRepeatEvent( pEventData->m_bRepeatEvent );
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
					//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
					pEvent->SetRepeatRewardEvent(pEventData->m_bRepeatRewardEvent);
					pEvent->SetBeforeEventID(pEventData->m_iBeforeEventID );
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_CUSTOM_CONNECT_EVENT
					pEvent->SetCustomEventID(pEventData->m_iCustomEventID);
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
					pEvent->SetOnlySteamUserEvent(pEventData->m_bOnlySteamUser )
					pEvent->SetOnlyNotSteamUserEvent( pEventData->m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
					pEvent->SetUnitClassLevel( pEventData->m_iUnitClassLevel )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
					START_LOG( cout, L"���� �ð� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetEventTime() )
						<< BUILD_LOG( pEvent->GetRewardID() )
						<< BUILD_LOG( pEvent->IsGiveOnce() )
						<< BUILD_LOG( pEvent->IsDelEndEvent() )
						<< BUILD_LOG( pEvent->IsPcBangEvent() )
						<< BUILD_LOG( pEvent->IsAccountEvent() )
						<< BUILD_LOG( pEvent->GetUnitLevel() )
						//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
						<< BUILD_LOG( pEvent->GetMinMaxRewardSize() )
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
						//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
						<< BUILD_LOG( pEvent->IsRepeatRewardEvent() )
						<< BUILD_LOG( pEvent->GetBeforeEventID() )
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
						<< BUILD_LOG( pEvent->GetUnitClassLevel() )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
						<< END_LOG;
						;
				}
			}
			break;

			//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
		case KGameEvent::GET_CUMULATIVE_TIME:
			{
				KGameEventCumulativeTime* pEvent = static_cast<KGameEventCumulativeTime*>(mit->second);

				//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
				const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				//}}
				if( pEventData == NULL )
				{
					START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< END_LOG;
					break;
				}

				if( pEvent->GetEventTime() != pEventData->m_fEventTime  ||
					pEvent->GetRewardID() != pEventData->m_iEventReward  ||
					pEvent->IsPcBangEvent() != pEventData->m_bPcBangEvent  ||
					pEvent->IsDirectReward() != pEventData->m_bDirectReward  ||
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					pEvent->IsAccountEvent() != pEventData->m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
					//}}
				{
					//�̺�Ʈ ��������..
					pEvent->SetEventTime( pEventData->m_fEventTime );
					pEvent->SetRewardID( pEventData->m_iEventReward );
					//{{ 2009. 10. 26  ������	PC���̺�Ʈ
					pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
					//}}
					//{{ 2009. 12. 8  ������	���ӽð��̺�Ʈ
					pEvent->SetDirectReward( pEventData->m_bDirectReward );
					//}}
					//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
					pEvent->SetAccountEvent( pEventData->m_bAccountEvent );
#endif SERV_ACC_TIME_EVENT
					//}}

					START_LOG( cout, L"���� �ð� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetEventTime() )
						<< BUILD_LOG( pEvent->GetRewardID() )
						<< BUILD_LOG( pEvent->IsDirectReward() )
						<< BUILD_LOG( pEvent->IsPcBangEvent() )
						<< BUILD_LOG( pEvent->IsAccountEvent() );
				}
			}
			break;
#endif CUMULATIVE_TIME_EVENT
			//}}

			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		case KGameEvent::GET_HENIR_REWARD:
			{
				KGameEventHenirReward* pEvent = static_cast<KGameEventHenirReward*>(mit->second);
				//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( pEvent->GetScriptID() );
#else
				const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
				//}}
				if( pEventData == NULL )
				{
					START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< END_LOG;
					break;
				}

				if( pEvent->GetHenirRewardEventCount() != pEventData->m_iHenirRewardEventCount || 
					//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
					pEvent->GetServerGroupID() != pEventData->m_iServerGroup  ||
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
					//}}
					pEvent->GetHenirRewardUnLimited() != pEventData->m_bHenirRewardUnLimited)
				{
					//�̺�Ʈ ��������..
					pEvent->SetHenirRewardEventCount( pEventData->m_iHenirRewardEventCount );
					pEvent->SetHenirRewardUnLimited( pEventData->m_bHenirRewardUnLimited );
					//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
					pEvent->SetServerGroupID( pEventData->m_iServerGroup );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
					//}}

					START_LOG( cout, L"���� �ð� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetHenirRewardEventCount() )
						<< BUILD_LOG( pEvent->GetHenirRewardUnLimited() )
						//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
						<< BUILD_LOG( pEvent->GetServerGroupID() )
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
						//}}
						;
				}
			}
			break;
#endif SERV_NEW_HENIR_TEST
			//}}

			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
		case KGameEvent::GET_QUEST_ITEM_DROP:
			{
				KGameEventQuestItemDrop* pEvent = static_cast<KGameEventQuestItemDrop*>(mit->second);

				const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );

				if( pEventData == NULL )
				{
					START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( mit->second->GetType() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< END_LOG;
					break;
				}

				if( pEvent->GetQuestItemDropRate() != pEventData->m_fQuestItemDropRate )
				{
					pEvent->SetQuestItemDropRate( pEventData->m_fQuestItemDropRate );

					START_LOG( cout, L"��� �̺�Ʈ ��ũ��Ʈ ����" )
						<< BUILD_LOG( mit->second->GetUID() )
						<< BUILD_LOG( pEvent->GetEventName() )
						<< BUILD_LOG( pEvent->GetScriptID() )
						<< BUILD_LOG( pEvent->GetQuestItemDropRate() );
				}
			}
			break;
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}

			}
		}
	}
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
bool KGameEventManager::RefreshEventTime( IN const KEventInfo& kInfo, OUT KGameEvent* pkGameEvent )
{
	if( pkGameEvent == NULL )
	{
		START_LOG( cerr, L"�̺�Ʈ��ü ������ �̻�.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< END_LOG;

		return false;
	}

#ifdef SERV_ALLOW_EVENT_TITLE_REFRESH
	pkGameEvent->SetEventName( kInfo.m_wstrEventName );
#endif SERV_ALLOW_EVENT_TITLE_REFRESH

	CTime ctBegin, ctEnd;

	if( KncUtil::ConvertStringToCTime( kInfo.m_wstrBeginDate, ctBegin ) == false )
	{
		START_LOG( cerr, L"���ڿ� �ð����� ����.!" )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< END_LOG;

		return false;
	}

	if( KncUtil::ConvertStringToCTime( kInfo.m_wstrEndDate, ctEnd ) == false )
	{
		START_LOG( cerr, L"���ڿ� �ð����� ����.!" )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< END_LOG;

		return false;
	}

	std::wstring wstrTime;

	//�̺�Ʈ ���۽ð� ����
	wstrTime = ( CStringW )( pkGameEvent->GetBeginDate().Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	if( pkGameEvent->GetBeginDate() != ctBegin )
	{
		START_LOG( cout2, L"�̺�Ʈ ���۽ð� ����.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( wstrTime )
			;

		pkGameEvent->SetBeginDate( ctBegin );
	}

	//�̺�Ʈ ����ð� ����.
	wstrTime = ( CStringW )( pkGameEvent->GetEndDate().Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	if( pkGameEvent->GetEndDate() != ctEnd )
	{
		START_LOG( cout2, L"�̺�Ʈ ����ð� ����.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( wstrTime )
			;

		pkGameEvent->SetEndDate( ctEnd );
	}

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // �̹� ��� �Ǿ� �ִ� �̺�Ʈ�� ��� ���� �κп��� DB���� �� on/off �κ��� ���� ������� �Ѵ�.
	pkGameEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

	return true;
}

bool KGameEventManager::SetPeriodEventTime( IN const KEventInfo& kInfo, OUT CTime& ctBegin, OUT CTime& ctEnd )
{
	if( KncUtil::ConvertStringToCTime( kInfo.m_wstrBeginDate, ctBegin ) == false )
	{
		START_LOG( cerr, L"���ڿ� �ð����� ����.!" )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< END_LOG;

		return false;
	}

	if( KncUtil::ConvertStringToCTime( kInfo.m_wstrEndDate, ctEnd ) == false )
	{
		START_LOG( cerr, L"���ڿ� �ð����� ����.!" )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< END_LOG;

		return false;
	}

	return true;
}

void KGameEventManager::AddQuestEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_QUEST].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_QUEST].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEventQuest* pEvent = new KGameEventQuest();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 02. 05  ������	���� �̺�Ʈ ����Ʈ
		pEvent->SetQuestID( kInfo.m_iScriptID );
		//}}
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_QUEST].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		pEvent->SetScriptID(kInfo.m_iScriptID);

		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID  %wstrEventBegin %wstrEventEnd %L"�ű� Quest �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

//{{ 2009. 5. 22  ������	��ӷ��̺�Ʈ
void KGameEventManager::AddItemDropEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_ITEM_DROP].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_ITEM_DROP].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}


		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
		if( pEventData == NULL )
#else
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

		if( pEventData == NULL )
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventItemDrop* pEvent = new KGameEventItemDrop();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		pEvent->SetScriptID( kInfo.m_iScriptID );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_PC_BANG_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
		pEvent->SetDropRate( pEventData->m_fDropRate );
#else // SERV_DROP_EVENT_RENEWAL
		pEvent->SetDropCount( pEventData->m_iDropCount );
#endif // SERV_DROP_EVENT_RENEWAL

		pEvent->SetWithPlayPcBang( pEventData->m_bWithPlayPcBang );
#endif SERV_PC_BANG_DROP_EVENT
		//}}
		
		//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
		pEvent->SetDungeonID( pEventData->m_iDungeonID );
		pEvent->SetUnitType( pEventData->m_cUnitType );
		pEvent->SetUnitClass( pEventData->m_cUnitClass  );
		pEvent->SetStartLevel( pEventData->m_iStartLevel );
		pEvent->SetEndLevel( pEventData->m_iEndLevel );
#endif //DROPEVENT_RENEWAL

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_ITEM_DROP].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID  %wstrEventBegin %wstrEventEnd %L"�ű� ��� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}
//}}

void KGameEventManager::AddRewardEvent( IN const KEventInfo& kInfo )
{
	//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( GetServerType() != ST_GAME )
		return;
#else
	if( GetServerType() != ST_CENTER )
		return;
#endif SERV_INTEGRATION
	//}}	

	std::map<int,KGameEvent*>::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_REWARD].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_REWARD].end() )
	{
        if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ���..
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventReward* pEvent = new KGameEventReward();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		pEvent->SetScriptID( kInfo.m_iScriptID );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		pEvent->SetEXPRate( pEventData->m_fEXPRate );
		pEvent->SetVPRate( pEventData->m_fVPRate );
		pEvent->SetEDRate( pEventData->m_fEDRate );
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
		pEvent->SetRankingPointRate( pEventData->m_fRankingPointRate );
#endif	// SERV_RANKING_POINT_EVENT
		pEvent->SetDungeonID( pEventData->m_iDungeonID );
		pEvent->SetUnitLevel( pEventData->m_iUnitLevel );
		pEvent->SetUnitType( pEventData->m_cUnitType );
		pEvent->SetUnitClass( pEventData->m_cUnitClass );
		pEvent->SetPartyNum( pEventData->m_iPartyNum );
		pEvent->SetServerGroupID( pEventData->m_iServerGroup );
		//{{ 2012. 06. 20	��μ�       �̺�Ʈ ����ġ ��ø �ȵ� ���� �߰�
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
		pEvent->SetAccumulateEXP( pEventData->m_bAccumulateEXP );
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
		//}}
		//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		pEvent->SetPlayWithCharEXP( pEventData->m_bPlayWithCharEXP );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}

#ifdef SERV_PCBANG_USER_REWARD_EVENT// �۾���¥: 2013-07-02	// �ڼ���
		pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
#endif // SERV_PCBANG_USER_REWARD_EVENT

		pEvent->Dump();
#else
		pEvent->SetData( pEventData->m_fEXPRate, 
						 pEventData->m_fVPRate, 
						 pEventData->m_fEDRate, 
						 pEventData->m_iDungeonID, 
						 pEventData->m_iUnitLevel, 
						 pEventData->m_cUnitType, 
						 pEventData->m_cUnitClass, 
						 pEventData->m_iPartyNum
						 );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}		

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_REWARD].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID  %wstrEventBegin %wstrEventEnd %L"�ű� ����ġ&ED&VP �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
            << BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER

void KGameEventManager::AddMonsterEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_MONSTER].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_MONSTER].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEventMonster* pEvent = new KGameEventMonster();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		pEvent->SetEventID( kInfo.m_iScriptID );

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_MONSTER].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		pEvent->SetScriptID( kInfo.m_iScriptID );
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID  %wstrEventBegin %wstrEventEnd %L"�ű� ���� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}    
}

#endif SERV_EVENT_MONSTER
//}}

void KGameEventManager::AddTournamentEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME && GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_TOURNAMENT].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_TOURNAMENT].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEventTournament* pEvent = new KGameEventTournament();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_TOURNAMENT].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

void KGameEventManager::AddSpiritEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_SPIRIT].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_SPIRIT].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEventTournament* pEvent = new KGameEventTournament();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_SPIRIT].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID  %wstrEventBegin %wstrEventEnd %L"�ű� �ټ��� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

//{{ 2008. 6. 9  ������  ������ �̺�Ʈ
void KGameEventManager::AddConnectTimeEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ���..
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventConnectTime* pEvent = new KGameEventConnectTime();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		pEvent->SetScriptID( kInfo.m_iScriptID );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		pEvent->SetEventID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		pEvent->SetEventTime( pEventData->m_fEventTime );
		pEvent->SetRewardID( pEventData->m_iEventReward );
		pEvent->SetGiveOnce( pEventData->m_bGiveOnce );
		pEvent->SetDelEndEvent( pEventData->m_bDelEndEvent );
		//{{ 2009. 10. 26  ������	PC���̺�Ʈ
		pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
		//}}
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		pEvent->SetAccountEvent( pEventData->m_bAccountEvent );
#endif SERV_ACC_TIME_EVENT
		//}}
		//{{ 2009. 12. 16  ������	ĳ���� ������ ���� �߰�
        pEvent->SetUnitLevel( pEventData->m_iUnitLevel );
		//}}
		// ������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
		pEvent->SetMinMaxReward( pEventData->m_mapMinMaxReward );
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
		//}}
		//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		pEvent->SetChannelCode( pEventData->m_iChannelCode );
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}
		//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		pEvent->SetBF_Team( pEventData->m_iBF_Team );
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
		//{{  2011.11.30     ��μ�    ĳ���ͺ� ���� �̺�Ʈ
#ifdef SERV_CHAR_CONNECT_EVENT
		pEvent->SetUnitType( pEventData->m_cUnitType );
		pEvent->SetUnitClass( pEventData->m_cUnitClass );
#endif SERV_CHAR_CONNECT_EVENT
		//}}
		//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		pEvent->SetRepeatEvent( pEventData->m_bRepeatEvent );
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}
		//{{ 2012. 12. 25	�ڼ���	Ư�� ���� ���� ���� �̺�Ʈ
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
		pEvent->SetEventGroupID( pEventData->m_iEventGroupID );
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
		//}}
		
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE	

#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
		pEvent->SetRepeatRewardEvent(pEventData->m_bRepeatRewardEvent);
		pEvent->SetBeforeEventID(pEventData->m_iBeforeEventID );
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		pEvent->SetEventItemID( pEventData->m_iEventItemID );
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

#ifdef SERV_CUSTOM_CONNECT_EVENT
		pEvent->SetCustomEventID(pEventData->m_iCustomEventID);
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
		pEvent->SetOnlySteamUserEvent( pEventData->m_bOnlySteamUser );
		pEvent->SetOnlyNotSteamUserEvent( pEventData->m_bOnlyNotSteamUser );
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// ���볯¥: 2013-04-11
		pEvent->SetCriterionUserType( pEventData->m_byteCriterionUserType );
		pEvent->SetCriterionPresentUserType( pEventData->m_byteCriterionPresentUserType );
		pEvent->SetCriterionDateBegin( pEventData->m_tCriterionDateBegin );
		pEvent->SetCriterionDateEnd( pEventData->m_tCriterionDateEnd );
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		pEvent->SetUnitClassLevel( pEventData->m_iUnitClassLevel );
#endif SERV_UNIT_CLASS_LEVEL_EVENT

		m_mapIngEventData[KGameEvent::GET_CONNECT_TIME].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ���ӽð� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		pEvent->SetNewUnitEvent( pEventData->m_bNewUnitEvent );
		pEvent->SetNewUnitEvent2( pEventData->m_bNewUnitEvent2 );
		pEvent->SetCurrentUnitEvent( pEventData->m_bCurrentUnitEvent );
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( pEventData->m_iEventReward )
			<< BUILD_LOG( pEventData->m_fEventTime )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}
//}}

void KGameEventManager::AddSecretDungeonEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_SECRET_DUNGEON].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_SECRET_DUNGEON].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventSecretDungeon* pEvent = new KGameEventSecretDungeon();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 10. 19	������	��д��� �̺�Ʈ ������Ʈ
#ifdef SERV_SECRET_DUNGEON_EVENT
		pEvent->SetDungeonID( pEventData->m_iDungeonID );
		pEvent->SetDungeonMode( pEventData->m_iDungeonMode );
#endif SERV_SECRET_DUNGEON_EVENT
		//}}

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_SECRET_DUNGEON].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		pEvent->SetScriptID(kInfo.m_iScriptID);
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ��� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

void KGameEventManager::AddNotifyMSGEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_NOTIFY_MSG].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_NOTIFY_MSG].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEventNotifyMSG* pEvent = new KGameEventNotifyMSG();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );

		//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
		pEvent->SetIsAllServers( ( kInfo.m_iScriptID == 1 ) );
#endif SERV_INTEGRATION
		//}}

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_NOTIFY_MSG].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ���� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT

void KGameEventManager::AddCumulativeTimeEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ���..
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventCumulativeTime* pEvent = new KGameEventCumulativeTime();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		pEvent->SetScriptID( kInfo.m_iScriptID );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		pEvent->SetEventID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		pEvent->SetEventTime( pEventData->m_fEventTime );
		pEvent->SetRewardID( pEventData->m_iEventReward );
		//{{ 2009. 10. 26  ������	PC���̺�Ʈ
		pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
		//}}
		//{{ 2009. 12. 8  ������	���ӽð��̺�Ʈ
		pEvent->SetDirectReward( pEventData->m_bDirectReward );
		//}}
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		pEvent->SetAccountEvent( pEventData->m_bAccountEvent );
#endif SERV_ACC_TIME_EVENT
		//}}
		//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		pEvent->SetChannelCode( pEventData->m_iChannelCode );
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

#ifdef SERV_CUSTOM_CONNECT_EVENT
		pEvent->SetCustomEventID(pEventData->m_iCustomEventID);
#endif //SERV_CUSTOM_CONNECT_EVENT

		m_mapIngEventData[KGameEvent::GET_CUMULATIVE_TIME].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� �����ð� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

#endif CUMULATIVE_TIME_EVENT
//}}
//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
void KGameEventManager::AddHenirRewardCountEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ���..
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventHenirReward* pEvent = new KGameEventHenirReward();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		pEvent->SetScriptID( kInfo.m_iScriptID );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		pEvent->SetEventID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		
		pEvent->SetHenirRewardUnLimited( pEventData->m_bHenirRewardUnLimited );
		pEvent->SetHenirRewardEventCount( pEventData->m_iHenirRewardEventCount );
		//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		pEvent->SetServerGroupID( pEventData->m_iServerGroup );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}
#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
		pEvent->SetPcBangEvent( pEventData->m_bPcBangEvent );
		pEvent->SetAccumulate( pEventData->m_bAccumulate );
#endif // SERV_HENIR_REWARD_EVENT

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ��ϸ� ���� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			<< END_LOG;
	}
}

#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
void KGameEventManager::GetHenirRewardCountEvent( IN const bool bPcBangUser
	, OUT bool& bUnLimited
	, OUT int& iEventMax
	)
{
	bUnLimited = false;
	iEventMax = 0;

	if( GetServerType() != ST_GAME )
		return;

	// �ڵ� �ľ��� ���� �ϱ� ���ؼ��� ��հŸ��� iEventMax ������ ����ϱ� ���ٴ� ���� �ϳ� ����� ���� ���� �� �ϴ�.
	int iAccumulateMax = 0;
	int iNonAccumulateMax = 0;
	for( std::map< int, KGameEvent* >::const_iterator mit = m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].begin(); mit != m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_HENIR_REWARD )
			continue;

		KGameEventHenirReward* pEvent = static_cast<KGameEventHenirReward*>(mit->second);

		// �������� ��ϸ� �ð� ���� Ƚ�� �̺�Ʈ
		if( mit->second->IsEnable() == true )
		{
			if( pEvent->GetServerGroupID() != SEnum::SGI_INVALID  &&  pEvent->GetServerGroupID() != KBaseServer::GetKObj()->GetServerGroupID() )
				continue;

			// PC�� ���� �̺�Ʈ�ΰ�?
			if( ( pEvent->GetPcBangEvent() == true ) && ( bPcBangUser == false ) )
				continue;

			if( pEvent->GetHenirRewardUnLimited() == true )
			{
				// �������� �ɷ��ִٸ�, Ƚ���� �߿����� �ʴ�.
				bUnLimited = true;
				return;
			}

			// ������ �Ǵ� �̺�Ʈ�ΰ�?
			if( pEvent->GetAccumulate() == true )
			{
				iAccumulateMax += pEvent->GetHenirRewardEventCount();
			}
			else if( iNonAccumulateMax < pEvent->GetHenirRewardEventCount() )
			{
				iNonAccumulateMax = pEvent->GetHenirRewardEventCount();
			}
		}
	}

	iEventMax = max( iAccumulateMax, iNonAccumulateMax );
}
#else // SERV_HENIR_REWARD_EVENT
bool KGameEventManager::GetHenirRewardCountEvent( OUT bool& bUnLimited, OUT int& iEventMax  )
{
	bUnLimited = false;
	iEventMax = 0;

	if( GetServerType() != ST_GAME )
		return false;

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].begin(); mit != m_mapIngEventData[KGameEvent::GET_HENIR_REWARD].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_HENIR_REWARD )
			continue;

		KGameEventHenirReward* pEvent = static_cast<KGameEventHenirReward*>(mit->second);

		// �������� ��ӷ� �̺�Ʈ
		if( mit->second->IsEnable() == true 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE			
			)
		{
			//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
#ifdef EXTEND_SERVER_GROUP_MASK
			if( pEvent->GetServerGroupID() != -1 && pEvent->GetServerGroupID() != KBaseServer::GetKObj()->GetServerGroupID() )
#else EXTEND_SERVER_GROUP_MASK
			if( pEvent->GetServerGroupID() != SEnum::SGI_INVALID  &&  pEvent->GetServerGroupID() != KBaseServer::GetKObj()->GetServerGroupID() )
#endif EXTEND_SERVER_GROUP_MASK
				continue;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
			//}}

			//{{ 2013. 04. 14	������	��ϸ� �ð� ���� Ƚ�� �̺�Ʈ ���� ����
#ifdef SERV_BUG_FIX_HENIR_REWARD_COUNT
			if( bUnLimited == false )
				bUnLimited = pEvent->GetHenirRewardUnLimited();

			if( iEventMax == 0 )
				iEventMax = pEvent->GetHenirRewardEventCount();
#else
			bUnLimited = pEvent->GetHenirRewardUnLimited();
			iEventMax = pEvent->GetHenirRewardEventCount();
			return true;
#endif SERV_BUG_FIX_HENIR_REWARD_COUNT
			//}}
		}
	}

	//{{ 2013. 04. 14	������	��ϸ� �ð� ���� Ƚ�� �̺�Ʈ ���� ����
#ifdef SERV_BUG_FIX_HENIR_REWARD_COUNT
	return true;
#else
	return false;
#endif SERV_BUG_FIX_HENIR_REWARD_COUNT
	//}}
}
#endif // SERV_HENIR_REWARD_EVENT
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
bool KGameEventManager::CheckWinterVacationEvent( IN const int iEventUID, IN const int iScriptID, IN ConnectEventFactorSet& kFactor )
{
	// õ�� ������ ���� �̺�Ʈ�ΰ�?
	if( ( iScriptID != 616 ) &&
		( iScriptID != 617 ) &&
		( iScriptID != 618 ) &&
		( iScriptID != 619 ) &&
		( iScriptID != 620 ) &&
		( iScriptID != 621 )
		)
	{
		// õ�� ������ ���� �̺�Ʈ�� �ƴ� ���� ��� �����ش�.
		return true;
	}
	else if( kFactor.m_iWinterVacationEventCount < 0 )
	{
		// ���� ���� ����ڰ� �ƴ϶�� ���� ó��
		return false;
	}

	// �ں� ����Ʈ ��Ű���ΰ�?
	if( iScriptID == 616 )
	{
		return true;
	}

	if( kFactor.m_pSetCompletedEvent->find( iEventUID ) != kFactor.m_pSetCompletedEvent->end() )
	{
		// �̹� �Ϸ��� �̺�Ʈ��� ���⿡ �������� �ʰ�����....
		return false;
	}

	// �Ϸ���� ���� �̺�Ʈ�� ù ��° �̺�Ʈ���, �ٷ� ���
	if( iScriptID == 617 )
	{
		std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mit;
		mit = kFactor.m_pMapEventTime->find( iEventUID );
		if( mit != kFactor.m_pMapEventTime->end() )
		{
			CTime		tCurrentTime = CTime::GetCurrentTime();
			CTimeSpan	tTime( static_cast<__int64>( mit->second.second.elapsed() ) );
			CTime		tFixedTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 6, 0, 0 );

			if( ( tCurrentTime - tTime ) < tFixedTime )
			{
				if( tFixedTime <= tCurrentTime )
				{
					kFactor.m_pVecEndEvent->push_back( KTimeEventSimpleInfo( iEventUID, iScriptID ) );
					kFactor.m_pMapEventTime->erase( mit );
					return false;
				}
			}
		}
		return true;
	}

	// �׷��� �ʴٸ�, ���� �̺�Ʈ�� �Ϸ�Ǿ����� Ȯ���Ѵ�.
	if( kFactor.m_pSetCompletedEvent->find( iEventUID - 1 ) != kFactor.m_pSetCompletedEvent->end() )
	{
		// �Ϸ� �ð��� ���ؼ� ���� �̺�Ʈ�� ó���� �ð��� �Ǿ����� Ȯ���Ѵ�.
		CTime tCurrentTime = CTime::GetCurrentTime();
		CTime tFixedTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 6, 0, 0 );
		CTime tClearTime( 2012, 12, 13, 0, 0, 0 );
		tClearTime += CTimeSpan( kFactor.m_iWinterVacationEventCount );

		if( tFixedTime <= tClearTime )
		{
			// ���� ��ħ 6�� ���Ŀ� Ŭ������ �̺�Ʈ�� �ִ�.
			// ������ ��ħ 6�� ������ �̺�Ʈ�� ������ �� ����.
			return false;
		}
	}
	else
	{
		// ���� �̺�Ʈ�� �Ϸ���� �ʾҴٸ�, ����� �� ����.
		return false;
	}

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mit;
	mit = kFactor.m_pMapEventTime->find( iEventUID );
	if( mit != kFactor.m_pMapEventTime->end() )
	{
		CTime		tCurrentTime = CTime::GetCurrentTime();
		CTimeSpan	tTime( static_cast<__int64>( mit->second.second.elapsed() ) );
		CTime		tFixedTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 6, 0, 0 );

		if( ( tCurrentTime - tTime ) < tFixedTime )
		{
			if( tFixedTime <= tCurrentTime )
			{
				kFactor.m_pVecEndEvent->push_back( KTimeEventSimpleInfo( iEventUID, iScriptID ) );
				kFactor.m_pMapEventTime->erase( mit );
				return false;
			}
		}
	}
	return true;
}
#endif SERV_2012_WINTER_VACATION_EVENT
//}}

//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
void KGameEventManager::AddQuestItemDropEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_QUEST_ITEM_DROP].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_QUEST_ITEM_DROP].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}


		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventQuestItemDrop* pEvent = new KGameEventQuestItemDrop();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetScriptID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		pEvent->SetQuestItemDropRate( pEventData->m_fQuestItemDropRate );

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_QUEST_ITEM_DROP].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ����Ʈ ������ ��ӷ� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

bool KGameEventManager::CheckQuestItemDropProbEvent( OUT float& fRate )
{
	if( GetServerType() != ST_CENTER )
		return false;

	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_QUEST_ITEM_DROP].begin(); mit != m_mapIngEventData[KGameEvent::GET_QUEST_ITEM_DROP].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetType() != KGameEvent::GET_QUEST_ITEM_DROP )
			continue;

		KGameEventQuestItemDrop* pEvent = static_cast<KGameEventQuestItemDrop*>(mit->second);

		// �������� ��ӷ� �̺�Ʈ
		if( mit->second->IsEnable() == true 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE	
			)
		{
			fRate = pEvent->GetQuestItemDropRate();

			return true;
		}
	}

	return false;
}
#endif SERV_QUEST_ITEM_DROP_EVENT
//}

#ifdef SERV_ADVERTISEMENT_EVENT
void KGameEventManager::AddAdvertisementEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ���..
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetAdvertisementData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"���� ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventAdvertisement* pEvent = new KGameEventAdvertisement();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
		pEvent->SetScriptID( kInfo.m_iScriptID );
#endif SERV_EVENT_SCRIPT_REFRESH
		//}}
		pEvent->SetEventID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		pEvent->SetAdvertisementURL( pEventData->m_wstrAdvertisementURL );
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.04.18 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� URL���� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

void KGameEventManager::CheckEnableAdvertisementEvent( IN OUT std::set< int >& setAdvertisementEvent, OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent, OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent )
{
	if( GetServerType() != ST_GAME )
		return;

	vecBeginEvent.clear();
	vecEndEvent.clear();

	std::map< int, KGameEvent* >::const_iterator mit;

	// ���� ���۵Ǵ� �̺�Ʈ�� ������ �߰��ϰ� ����Ǵ� �̺�Ʈ�� ������ ����
	for( mit = m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].begin(); mit != m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_ADVERTISEMENT_EVENT )
			continue;

		const int iEventUID = mit->first;
		KGameEventAdvertisement* pEvent = static_cast<KGameEventAdvertisement*>(mit->second);

		// 3. �̺�Ʈ�� ����������..
		if( mit->second->IsEnable() 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			// �̹� �Ϸ��� �̺�Ʈ��� �Ѿ��!
			if( setAdvertisementEvent.find( iEventUID ) != setAdvertisementEvent.end() )
				continue;

			// 4-1. �������� �̺�Ʈ��� ������ �̺�Ʈ ����Ʈ�� Ȯ�� �� ���� �̺�Ʈ��� ä���ش�.
			//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
			KTimeEventSimpleInfo kBeginEventInfo;
			kBeginEventInfo.m_iEventUID		= iEventUID;
			kBeginEventInfo.m_iEventID		= pEvent->GetEventID();
			kBeginEventInfo.m_wstrEventName = pEvent->GetEventName();
			kBeginEventInfo.m_wstrAdvertisementURL = pEvent->GetAdvertisementURL();
			vecBeginEvent.push_back( kBeginEventInfo );
			//}}
		}
		else
		{
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.11 lygan_������ // ���� �̺�Ʈ�� ���ư��ٰ� ���� ���ܼ� �ٷ� �� ��� DB���� off ó�� �Ȱ� �Ϸ��ʿ� ���� �ʴ´�. ���� ������ �ش� �̺�Ʈ�� �ٽ� ����� �ϱ� ����. ���� �� �ű� ����ϸ� �ѹ� �� ������ �ִ�.
			if (  mit->second->IsOn() == true )  
			{
				// �Ϸ� �̺�Ʈ�� �߰�
				setAdvertisementEvent.insert( iEventUID );
			}
#else //SERV_ALLOW_EVENT_ERASE	
			// �Ϸ� �̺�Ʈ�� �߰�
			setAdvertisementEvent.insert( iEventUID );
#endif //SERV_ALLOW_EVENT_ERASE	
			
			vecEndEvent.push_back( KTimeEventSimpleInfo( iEventUID, pEvent->GetEventID() ));
		}
	}
}

void KGameEventManager::GetAdvertisementURL( OUT std::vector< std::wstring >& vecAdvertisementURL )
{
	std::map< int, KGameEvent* >::const_iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].begin(); mit != m_mapIngEventData[KGameEvent::GET_ADVERTISEMENT_EVENT].end(); ++mit )
	{
		if( mit->second->IsEnable() 
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.05.22 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
			&& mit->second->IsOn() == true
#endif //SERV_ALLOW_EVENT_ERASE		
			)
		{
			KGameEventAdvertisement* pEvent = static_cast<KGameEventAdvertisement*>(mit->second);
			vecAdvertisementURL.push_back( pEvent->GetAdvertisementURL() );
		}
	}
}
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_NEW_EVENT_TYPES
int KGameEventManager::GetMaxLevel()
{
	int iMaxLevel = -1;
	// maxlevel �̺�Ʈ�� ���� ������ �̿��ϵ��� �մϴ�.
	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_MAX_LEVEL].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_MAX_LEVEL].end(); ++mitIngEventData )
	{
		// ���� �ִ� �̺�Ʈ�� �ǹ� ����.
		if( mitIngEventData->second->IsEnable() == false )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.03 lygan_������  // DB���� �� on/off �κ� �߰� �۾�
		if( mitIngEventData->second->IsOn() == false )
			continue;
#endif //SERV_ALLOW_EVENT_ERASE		

		iMaxLevel = max( mitIngEventData->second->GetScriptID(), iMaxLevel );
	}

	return iMaxLevel;
}

bool KGameEventManager::IsEnableDungeon( int nDunegonID, bool bEventDungeon )
{
	// �̺�Ʈ ������ �ƴϸ� ������ ����
	if( bEventDungeon == false )
		return true;

	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_DUNGEON].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_DUNGEON].end(); ++mitIngEventData )
	{
		// ���� �ִ� �̺�Ʈ�� �ǹ� ����.
		if( mitIngEventData->second->IsEnable() == false )
			continue;

		// 2013.03.28 lygan_������ // DB���� ��� on / off ���� �ٷ� ���� �ǰ�
		if ( mitIngEventData->second->IsOn() == false )
			continue;

		if( mitIngEventData->second->GetScriptID() == nDunegonID )
		{
			START_LOG( clog, L"���� ����" )
				<< BUILD_LOG( nDunegonID )
				<< BUILD_LOG( bEventDungeon )
				;

			return true;
		}
	}

	START_LOG( clog, L"���� ����" )
		<< BUILD_LOG( nDunegonID )
		<< BUILD_LOG( bEventDungeon )
		;

	return false;
}

void KGameEventManager::CheckEnableEventDungeon( IN OUT std::map< int, bool >& mapCheckDungeon )
{
	// on/off �� ������ on/off ���� ������ �־�� �ϴµ�...

	std::map< int, bool >::iterator mitCheckDungeon;
	for( mitCheckDungeon = mapCheckDungeon.begin(); mitCheckDungeon != mapCheckDungeon.end(); ++mitCheckDungeon )
	{
		// �켱 ������
		mitCheckDungeon->second = false;
	}

	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_DUNGEON].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_DUNGEON].end(); ++mitIngEventData )
	{
		// ���� �ִ� �̺�Ʈ�� �ǹ� ����.
		if( mitIngEventData->second->IsEnable() == false )
			continue;

		// 2013.03.28 lygan_������ // DB���� ��� on / off ���� �ٷ� ���� �ǰ�
		if ( mitIngEventData->second->IsOn() == false )
			continue;

		// �̺�Ʈ �������� ����ID
		int iIngDungeonID = mitIngEventData->second->GetScriptID();

		// ���� ���� ������ ��� ���ش�.
		mitCheckDungeon = mapCheckDungeon.find( iIngDungeonID );

		if( mitCheckDungeon != mapCheckDungeon.end() )
			mitCheckDungeon->second = true;

		START_LOG( clog, L"���� ����" )
			<< BUILD_LOG( iIngDungeonID )
			;
	}

}


void KGameEventManager::AddDungeonEvent( IN const KEventInfo& kInfo )
{
	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_DUNGEON].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_DUNGEON].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEvent* pEvent = new KGameEvent();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 02. 05  ������	���� �̺�Ʈ ����Ʈ
		pEvent->SetScriptID( kInfo.m_iScriptID );
		//}}
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_DUNGEON].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ���� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}
void KGameEventManager::AddMaxLevelEvent( IN const KEventInfo& kInfo )
{
	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_MAX_LEVEL].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_MAX_LEVEL].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEvent* pEvent = new KGameEvent();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 02. 05  ������	���� �̺�Ʈ ����Ʈ
		pEvent->SetScriptID( kInfo.m_iScriptID );
		//}}
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_MAX_LEVEL].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );


#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ���� ���� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}
void KGameEventManager::AddContents( IN const KEventInfo& kInfo )
{
	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_CONTENTS].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_CONTENTS].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEvent* pEvent = new KGameEvent();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 02. 05  ������	���� �̺�Ʈ ����Ʈ
		pEvent->SetScriptID( kInfo.m_iScriptID );
		//}}
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_CONTENTS].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� ������ �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}
#endif SERV_NEW_EVENT_TYPES


#ifdef SERV_ADD_EVENT_DB

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
void KGameEventManager::InitEventReleaseTick()
{
	m_iEventReleaseTick = 0;
#ifdef SERV_NEW_EVENT_TYPES
	m_iAppliedEventTick = 0;
#endif SERV_NEW_EVENT_TYPES
}
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK

void KGameEventManager::InitEventDBScriptReleaseTick()
{
	m_iEventDBScriptReleaseTick = 0;

}

bool KGameEventManager::TickCheckEvent()
{


	if( m_tEventReleaseTickTimer.elapsed() > 60.0f )
	{
		int iReleaseTickCount = 0;

		iReleaseTickCount = GetEventDBScriptReleaseTick();
		if ( iReleaseTickCount == -1 )
		{
			m_tEventReleaseTickTimer.restart();
			return false;
		}

		SendToEventDB( DBE_CHECK_EVENT_UPDATE_REQ );

		m_tEventReleaseTickTimer.restart();

	}

	return true;
}

#else //SERV_ADD_EVENT_DB
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
void KGameEventManager::InitEventReleaseTick()
{
	m_iEventReleaseTick = 0;
#ifdef SERV_NEW_EVENT_TYPES
	m_iAppliedEventTick = 0;
#endif SERV_NEW_EVENT_TYPES
}

bool KGameEventManager::TickCheckEvent()
{

	if( m_tEventCheckTimer.elapsed() > 60.0f )
	{
		int iReleaseTickCount = 0;

		iReleaseTickCount = GetEventReleaseTick();
		if ( iReleaseTickCount == -1 )
		{

			m_tEventCheckTimer.restart();

			return false;
		}

		SendToLogDB( DBE_CHECK_EVENT_UPDATE_REQ );

		m_tEventCheckTimer.restart();

	}

	return true;
}
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_ADD_EVENT_DB


#ifdef SERV_CODE_EVENT
void KGameEventManager::AddCodeEvent( IN const KEventInfo& kInfo )
{
	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_CODE].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_CODE].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		KGameEvent* pEvent = new KGameEvent();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		//{{ 2010. 02. 05  ������	���� �̺�Ʈ ����Ʈ
		pEvent->SetScriptID( kInfo.m_iScriptID );
		//}}
#ifdef SERV_ALLOW_EVENT_ERASE // 2013.01.22 lygan_������ // DB���� �� on/off �κ� �߰� �۾�
		pEvent->SetOn(kInfo.m_bOn);
#endif //SERV_ALLOW_EVENT_ERASE

		m_mapIngEventData[KGameEvent::GET_CODE].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
		std::wstring wstrEventBegin = (CStringW)(ctBegin.Format(_T("%Y-%m-%d %H:%M:%S")));
		std::wstring wstrEventEnd = (CStringW)(ctEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
		SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%_%3%_%4%_%5%_%6%_%7%_%8%")
			% 1 %pEvent->GetUID() %pEvent->GetEventName() %pEvent->GetType() %kInfo.m_iScriptID %wstrEventBegin %wstrEventEnd %L"�ű� �ڵ� �̺�Ʈ ���" ));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

bool KGameEventManager::IsEnableCode( IN int iEventID )
{
	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_CODE].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_CODE].end(); ++mitIngEventData )
	{
		// ���� �ִ� �̺�Ʈ�� �ǹ� ����.
		if( mitIngEventData->second->IsEnable() == false )
			continue;

		// 2013.03.28 lygan_������ // DB���� ��� on / off ���� �ٷ� ���� �ǰ�
		if ( mitIngEventData->second->IsOn() == false)
			continue;


		if( mitIngEventData->second->GetScriptID() == iEventID )
			return true;
	}

	return false;
}

void KGameEventManager::CheckEnableCode( OUT std::map< int, bool >& mapCheckCode )
{
	// �켱 ������
	mapCheckCode.clear();
	for( int iCodeEventID = 1; iCodeEventID < CEI_END; ++iCodeEventID )
	{
		mapCheckCode[iCodeEventID] = false;
	}

	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_CODE].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_CODE].end(); ++mitIngEventData )
	{
		// ���� �ִ� �̺�Ʈ�� �ǹ� ����.
		if( mitIngEventData->second->IsEnable() == false )
			continue;

		// 2013.03.28 lygan_������ // DB���� ��� on / off ���� �ٷ� ���� �ǰ�
		if ( mitIngEventData->second->IsOn() == false)
			continue;

		// �̺�Ʈ �������� ����ID
		int iIngCodeID = mitIngEventData->second->GetScriptID();

		// ���� ���� ������ ��� ���ش�.
		std::map< int, bool >::iterator mitCheckCode = mapCheckCode.find( iIngCodeID );

		if( mitCheckCode != mapCheckCode.end() )
			mitCheckCode->second = true;

		START_LOG( clog, L"���� �ڵ� enum" )
			<< BUILD_LOG( iIngCodeID )
			;
	}
}

#ifdef SERV_GLOBAL_EVENT_TABLE
void KGameEventManager::CheckEnableCodeAndSetGlobalEventdata( OUT std::map< int, KGlobalEventTableData >& mapCheckCodeAndData )
{
	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_CODE].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_CODE].end(); ++mitIngEventData )
	{
		// ���� �ִ� �̺�Ʈ�� �ǹ� ����.
		if( mitIngEventData->second->IsEnable() == false )
			continue;

		// 2013.03.28 lygan_������ // DB���� ��� on / off ���� �ٷ� ���� �ǰ�
		if ( mitIngEventData->second->IsOn() == false)
			continue;


		std::map< int, KGlobalEventTableData >::iterator mit = mapCheckCodeAndData.find(mitIngEventData->second->GetUID());

		if ( mit == mapCheckCodeAndData.end())
		{
			KGlobalEventTableData kGlobalEventTableDataTemp;
			kGlobalEventTableDataTemp.m_iEventID = mitIngEventData->second->GetUID();
			kGlobalEventTableDataTemp.m_iEventScriptID = mitIngEventData->second->GetScriptID();
			kGlobalEventTableDataTemp.m_iEventType = mitIngEventData->second->GetType();

			switch( kGlobalEventTableDataTemp.m_iEventScriptID )
			{
				case CEI_CHRISTMAS_DISCOUNT_EVENT_2013:
					{
						kGlobalEventTableDataTemp.m_cEventAccountType = KGlobalEventTableData::EAE_ACCOUNT_NORMAL;
					}
					break;
#ifdef SERV_4TH_ANNIVERSARY_EVENT
				case CEI_4TH_ANNIVERSARY_EVENT:
					{
						kGlobalEventTableDataTemp.m_cEventAccountType = KGlobalEventTableData::EAE_ACCOUNT_SUM;
					}
					break;
#endif // SERV_4TH_ANNIVERSARY_EVENT
				default:
					{
						kGlobalEventTableDataTemp.m_cEventAccountType = KGlobalEventTableData::EAE_NONE;
					}
					break;
			}

			mapCheckCodeAndData.insert(std::make_pair(mitIngEventData->second->GetUID(), kGlobalEventTableDataTemp));
		}

	}

}

void KGameEventManager::CheckInGameDisableCodeEvent( IN std::map< int, KGlobalEventTableData >& mapCheckEnableCodeAndData, OUT std::map< int, KGlobalEventTableData >& mapCheckDisableCodeAndData ) // 2013.12.19 darkstarbt_������ // ������ �����߿� ����� �ڵ� �̺�Ʈ�� ���ؼ� üũ �ϴ� ���
{

	mapCheckDisableCodeAndData.clear();

	std::map< int, KGameEvent* >::iterator mitIngEventData = m_mapIngEventData[KGameEvent::GET_CODE].begin();
	for( ; mitIngEventData != m_mapIngEventData[KGameEvent::GET_CODE].end(); ++mitIngEventData )
	{
		if( mitIngEventData->second->IsEnable() == false )
		{
			std::map< int, KGlobalEventTableData >::iterator mit = mapCheckEnableCodeAndData.find(mitIngEventData->second->GetUID());
			
			if ( mit != mapCheckEnableCodeAndData.end() )
			{
				mapCheckDisableCodeAndData.insert(std::make_pair(mitIngEventData->second->GetUID(), mit->second));

				mapCheckEnableCodeAndData.erase(mitIngEventData->second->GetUID());

				continue;
			}
		}

		if ( mitIngEventData->second->IsOn() == false)
		{
			std::map< int, KGlobalEventTableData >::iterator mit = mapCheckEnableCodeAndData.find(mitIngEventData->second->GetUID());

			if ( mit != mapCheckEnableCodeAndData.end() )
			{
				mapCheckDisableCodeAndData.insert(std::make_pair(mitIngEventData->second->GetUID(), mit->second));

				mapCheckEnableCodeAndData.erase(mitIngEventData->second->GetUID());

				continue;
			}
		}
		
	}
}

#endif //SERV_GLOBAL_EVENT_TABLE

#endif SERV_CODE_EVENT

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
bool KGameEventManager::IsEnableEventQuest( IN int iQuestID )
{
	if( GetServerType() != ST_GAME )
		return false;


	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );

	if( pQuestTemplet == NULL )
		return false;

	// Ÿ������Ʈ�� �ƴѰ��
	if ( pQuestTemplet->m_bIsTimeEvent == false )
		return true;

	// Ÿ������Ʈ�� ���
	std::map< int, KGameEvent* >::const_iterator mit;
	for( mit = m_mapIngEventData[KGameEvent::GET_QUEST].begin(); mit != m_mapIngEventData[KGameEvent::GET_QUEST].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_QUEST )
			continue;

		const int iEventUID = mit->first;
		KGameEventQuest* pEvent = static_cast<KGameEventQuest*>(mit->second);

		const int iEnableQuestID = pEvent->GetQuestID();

		if ( iEnableQuestID == iQuestID )
		{
			// 3. �̺�Ʈ�� ����������..
			if( mit->second->IsEnable() && mit->second->IsOn() == true )
			{
				return true;
			}
		}
	}

	// Ÿ������Ʈ�ε� ���� �ش� �̺�Ʈ�� ���ư��� �ʰ� �ִ�
	return false;
}
#endif //SERV_ALLOW_EVENT_ERASE


#ifdef SERV_CHECK_TIME_QUEST
void KGameEventManager::GetWarningEventQuest( OUT KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT& kPacket_ )
{
	if( GetServerType() != ST_GAME )
		return;

	kPacket_.m_vecWarningQuestID.clear();

	std::wstringstream wstrMsg; // ���� �α� ��
	std::map< int, KGameEvent* >::iterator mit;

	for( mit = m_mapIngEventData[KGameEvent::GET_QUEST].begin(); mit != m_mapIngEventData[KGameEvent::GET_QUEST].end(); ++mit )
	{
		// 1. NULL üũ
		if( mit->second == NULL )
			continue;

		// 2. �̺�Ʈ Ÿ�� üũ
		if( mit->second->GetType() != KGameEvent::GET_QUEST )
			continue;

		// 3. On Off üũ
		if( false == mit->second->IsOn() )
			continue;

		// 4. Enable üũ
		if( false == mit->second->IsEnable() )
			continue;

		const int iEventUID = mit->first;
		KGameEventQuest* pEvent = static_cast<KGameEventQuest*>(mit->second);

		const int iQuestID = pEvent->GetQuestID();
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );

		if( pQuestTemplet == NULL )
		{
			START_LOG( cout, L"[�׽�Ʈ] ���� ����Ʈ ID�� �̺�Ʈ�� ����� �Ǿ����ϴ�." )
				<< BUILD_LOG( iQuestID );
		}
		else
		{
			if( false == pQuestTemplet->m_bIsTimeEvent )
			{
				kPacket_.m_vecWarningQuestID.push_back( iQuestID );
				wstrMsg << iQuestID << L", ";
			}
		}
	}

	if( kPacket_.m_vecWarningQuestID.empty() == false )
	{		
		START_LOG( cout, L"[�׽�Ʈ] TimeEvent ������ �ȵ� ����Ʈ�� DB�� ��ϵǾ� �ֽ��ϴ�! Ȯ�� �ʿ�!!" )
			<< BUILD_LOG( wstrMsg.str().c_str() );
	}
}
#endif SERV_CHECK_TIME_QUEST
//}}

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
void KGameEventManager::AddDefenseDungeonOpenRateEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_CENTER )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_DEFENSE_DUNGEON].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_DEFENSE_DUNGEON].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const EVENT_DATA* pEventData = GetTotalEventData( kInfo.m_iScriptID );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEventDefenseDungeon* pEvent = new KGameEventDefenseDungeon();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetScriptID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );
		pEvent->SetDefenseDungeonOpenRate( pEventData->m_fDefenseDungeonOpenRate );

		m_mapIngEventData[KGameEvent::GET_DEFENSE_DUNGEON].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

bool KGameEventManager::GetDefenseDungeonOpenRate( OUT float& fDefenseDungeonOpenRate ) const
{
	fDefenseDungeonOpenRate = 0.0f;

	if( GetServerType() != ST_CENTER )
		return false;

	std::map<int, KGameEvent*>::const_iterator it;
	for( it = m_mapIngEventData[KGameEvent::GET_DEFENSE_DUNGEON].begin(); it != m_mapIngEventData[KGameEvent::GET_DEFENSE_DUNGEON].end(); ++it )
	{
		if( it->second == NULL )
			continue;

		if( it->second->GetType() != KGameEvent::GET_DEFENSE_DUNGEON )
			continue;

		KGameEventDefenseDungeon* pEvent = static_cast<KGameEventDefenseDungeon*>( it->second );

		// �������� ��ӷ� �̺�Ʈ
		if( it->second->IsEnable() == true )
		{
			float fTemp = pEvent->GetDefenseDungeonOpenRate();
			if( fDefenseDungeonOpenRate < fTemp )
			{
				fDefenseDungeonOpenRate = fTemp;
			}
		}
	}

	return ( 0.0f < fDefenseDungeonOpenRate );
}
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef SERV_LEVEL_UP_EVENT// �۾���¥: 2013-06-24	// �ڼ���
void KGameEventManager::AddLevelUpEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_LEVEL_UP].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_LEVEL_UP].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;

			return;
		}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEvent* pEvent = new KGameEvent();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetScriptID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );

		m_mapIngEventData[KGameEvent::GET_LEVEL_UP].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

void KGameEventManager::GetLevelUpEvent( IN const unsigned char ucBeforeLevel
#ifdef SERV_LEVEL_UP_EVENT_CRITERION_DATE// �۾���¥: 2013-06-26	// �ڼ���
									   , IN const byte byteCriterionDateEventInfo
									   , IN const std::wstring& wstrRegDate
#endif // SERV_LEVEL_UP_EVENT_CRITERION_DATE
									   , OUT std::vector<int>& vecRewardID
									   ) const
{
	vecRewardID.clear();

	if( GetServerType() != ST_GAME )
		return;

	std::map<int, KGameEvent*>::const_iterator it;
	for( it = m_mapIngEventData[KGameEvent::GET_LEVEL_UP].begin(); it != m_mapIngEventData[KGameEvent::GET_LEVEL_UP].end(); ++it )
	{
		if( it->second == NULL )
			continue;

		if( it->second->GetType() != KGameEvent::GET_LEVEL_UP )
			continue;

		KGameEvent* pEvent = static_cast<KGameEvent*>( it->second );

		// �������� �̺�Ʈ
		if( it->second->IsEnable() == true )
		{
			const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
			if( pEventData == NULL )
			{
				START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
					<< BUILD_LOG( pEvent->GetUID() )
					<< BUILD_LOG( pEvent->GetEventName() )
					<< BUILD_LOG( pEvent->GetScriptID() )
					<< END_LOG;
				continue;
			}

#ifdef SERV_LEVEL_UP_EVENT_CRITERION_DATE// �۾���¥: 2013-06-26	// �ڼ���
			if( CheckCriterionUserEvent( byteCriterionDateEventInfo, wstrRegDate, pEvent->GetScriptID() ) == false )
			{
				continue;
			}
#endif // SERV_LEVEL_UP_EVENT_CRITERION_DATE

			std::map< std::pair< int, int >, int >::const_iterator itMinMaxReward;
			for( itMinMaxReward = pEventData->m_mapMinMaxReward.begin(); itMinMaxReward != pEventData->m_mapMinMaxReward.end(); ++itMinMaxReward )
			{
				int iMinLevel = itMinMaxReward->first.first;
				int iMaxLevel = itMinMaxReward->first.second;

				if( ( iMinLevel == 0 ) || ( iMaxLevel == 0 ) )
					continue;

				// ��ũ��Ʈ���� �����ϴ� ������ ������ ���ǻ� �޼��ؾ� �� ������ �����ϵ�
				// �ѹ��� 2���� �̻� ���� ���� ��츦 ����Ͽ�, ������ �ϴ� ������ ������ �������� �����Ѵ�.
				--iMinLevel;
				--iMaxLevel;

				if( ( ucBeforeLevel < iMinLevel ) || ( iMaxLevel < ucBeforeLevel ) )
				{
					continue;
				}

				vecRewardID.push_back( itMinMaxReward->second );
			}
		}
	}
}
#endif // SERV_LEVEL_UP_EVENT

#ifdef SERV_CRITERION_DATE_EVENT// �۾���¥: 2013-06-26	// �ڼ���
bool KGameEventManager::CheckCriterionUserEvent( IN const byte byteCriterionDateEventInfo
											   , IN const std::wstring& wstrRegDate
											   , IN const int iScriptID
											   ) const
{
	const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( iScriptID );
	if( pEventData == NULL )
	{
		START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
			<< BUILD_LOG( iScriptID )
			<< END_LOG;
		return false;
	}

	return CheckCriterionUserEvent( byteCriterionDateEventInfo, wstrRegDate, pEventData );
}

bool KGameEventManager::CheckCriterionUserEvent( IN const byte byteCriterionDateEventInfo
												, IN const std::wstring& wstrRegDate
												, IN const KGameEventScriptManager::EVENT_DATA* pEventData
												) const
{
	if( pEventData == NULL )
	{
		START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
			<< END_LOG;
		return false;
	}

	if( pEventData->m_byteCriterionUserType & SEnum::CUT_ALL )
	{
#ifdef SERV_CRITERION_DATE_EVENT_JUMPING_CHARACTER// �۾���¥: 2013-06-26	// �ڼ���
		if( pEventData->m_byteCriterionUserType & SEnum::CUT_JUMPING )
		{
			if( byteCriterionDateEventInfo & SEnum::CUT_JUMPING )
			{
				return ( pEventData->m_byteCriterionPresentUserType & SEnum::CUT_JUMPING ) ? true : false;
			}
		}
#endif // SERV_CRITERION_DATE_EVENT_JUMPING_CHARACTER_

		if( pEventData->m_byteCriterionUserType & SEnum::CUT_NEW )
		{
			CTime tRegDate;
			if( KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate ) == false )
			{
				START_LOG( cwarn, L"���� ���� ���� ��ȯ ����( ä�� �������� ���� ���� �Ͻ����� ���ɼ��� ���� )" )
					<< BUILD_LOG( wstrRegDate )
					<< END_LOG;
				return false;
			}

			if( ( pEventData->m_tCriterionDateBegin <= tRegDate ) &&
				( tRegDate < pEventData->m_tCriterionDateEnd )
				)
			{
				return ( pEventData->m_byteCriterionPresentUserType & SEnum::CUT_NEW ) ? true : false;
			}
		}

		if( pEventData->m_byteCriterionUserType & SEnum::CUT_COMEBACK )
		{
			if( byteCriterionDateEventInfo & SEnum::CUT_COMEBACK )
			{
				return ( pEventData->m_byteCriterionPresentUserType & SEnum::CUT_COMEBACK ) ? true : false;
			}
		}

		if( pEventData->m_byteCriterionUserType & SEnum::CUT_EXIST )
		{
			return ( pEventData->m_byteCriterionPresentUserType & SEnum::CUT_EXIST ) ? true : false;
		}

		return false;
	}

	return true;
}
#endif // SERV_CRITERION_DATE_EVENT

#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-09	// �ڼ���
void KGameEventManager::AddJumpingCharacterEvent( IN const KEventInfo& kInfo )
{
	if( GetServerType() != ST_GAME )
		return;

	std::map< int, KGameEvent* >::iterator mit;
	mit = m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].find( kInfo.m_iEventUID );

	//�̹� �ִ� �̺�Ʈ���..
	if( mit != m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].end() )
	{
		if( RefreshEventTime( kInfo, mit->second ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ ����ð� ���Ž���.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< END_LOG;
		}
	}
	//���� �̺�Ʈ��� �ű��߰�.
	else
	{
		CTime ctBegin, ctEnd;

		if( SetPeriodEventTime( kInfo, ctBegin, ctEnd ) == false )
		{
			START_LOG( cerr, L"�̺�Ʈ �Ⱓ���� ����.!" )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< END_LOG;
			return;
		}

		if( ctEnd <= ctBegin )
		{
			START_LOG( cerr, L"���� �ð����� ���� �ð��� �� �۽��ϴ�." )
				<< BUILD_LOG( kInfo.m_iEventType )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_wstrBeginDate )
				<< BUILD_LOG( kInfo.m_wstrEndDate )
				<< END_LOG;
			return;
		}

		//// ���� ĳ���� �̺�Ʈ�� �ߺ����� �� �� ����.
		//// �̹� ��ϵ� ���� ĳ���� �̺�Ʈ�� �ִٸ�, �ð� üũ�� ���� �ɷ������� �Ѵ�.
		//for( std::map<int, KGameEvent*>::const_iterator it = m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].begin(); it != m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].end(); ++it )
		//{
		//	if( it->second == NULL )
		//		continue;

		//	if( it->second->GetType() != KGameEvent::GET_JUMPING_CHARACTER )
		//		continue;

		//	KGameEvent* pEvent = static_cast<KGameEvent*>( it->second );

		//	// �ߺ� �ð� �˻�
		//	if( ( ctEnd < pEvent->GetBeginDate() ) || ( pEvent->GetEndDate() < ctBegin ) )
		//	{
		//		continue;
		//	}
		//	
		//	START_LOG( cerr, L"���� ĳ���� �̺�Ʈ�� �ߺ����� �� �� �����ϴ�." )
		//		<< BUILD_LOG( kInfo.m_iEventType )
		//		<< BUILD_LOG( kInfo.m_iEventUID )
		//		<< BUILD_LOG( pEvent->GetUID() )
		//		<< END_LOG;
		//	return;
		//}

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		const EVENT_DATA* pEventData = SiKEventDataRefreshManager()->GetEventData( kInfo.m_iScriptID );
#else
		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( kInfo.m_iScriptID );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�����̺�Ʈ ��ũ��Ʈ�� �����ϴ�.!" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( kInfo.m_wstrEventName )
				<< BUILD_LOG( kInfo.m_iScriptID )
				<< END_LOG;
			return;
		}

		KGameEvent* pEvent = new KGameEvent();

		//�̺�Ʈ ��������..
		pEvent->SetUID( kInfo.m_iEventUID );
		pEvent->SetScriptID( kInfo.m_iScriptID );
		pEvent->SetType( (KGameEvent::GAME_EVENT_TYPE)kInfo.m_iEventType );
		pEvent->SetEventName( kInfo.m_wstrEventName );
		pEvent->SetBeginDate( ctBegin );
		pEvent->SetEndDate( ctEnd );

		m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].insert( std::make_pair( kInfo.m_iEventUID, pEvent ) );

		START_LOG( cout2, L"�ű� �̺�Ʈ ���.!" )
			<< BUILD_LOG( kInfo.m_iEventUID )
			<< BUILD_LOG( kInfo.m_iEventType )
			<< BUILD_LOG( kInfo.m_wstrEventName )
			<< BUILD_LOG( kInfo.m_wstrBeginDate )
			<< BUILD_LOG( kInfo.m_wstrEndDate )
			<< BUILD_LOG( kInfo.m_iScriptID )
			<< BUILD_LOG( (int)kInfo.m_bEnable )
			;
	}
}

bool KGameEventManager::GetJumpingCharacterEvent( IN const char cUnitType
												, IN const int iLevel
												, OUT CTime& tBeginDate
												, OUT CTime& tEndDate
												, OUT int& iTargetLevel
												, OUT bool& bAccountEvent
												, OUT CTime& tCurrentDate
												) const
{
	if( GetServerType() != ST_GAME )
	{
		START_LOG( cerr, L"���� ���������� ����� �� �ִ� ����Դϴ�." )
			<< END_LOG;
		
		return false;
	}

	tCurrentDate = CTime::GetCurrentTime();

	for( std::map<int, KGameEvent*>::const_iterator it = m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].begin(); it != m_mapIngEventData[KGameEvent::GET_JUMPING_CHARACTER].end(); ++it )
	{
		if( it->second == NULL )
			continue;

		if( it->second->GetType() != KGameEvent::GET_JUMPING_CHARACTER )
			continue;

		const KGameEvent* pEvent = static_cast<KGameEvent*>( it->second );

		// �������� �̺�Ʈ
		if( tCurrentDate < it->second->GetBeginDate() )
		{
			continue;
		}

		if( it->second->GetEndDate() < tCurrentDate )
		{
			continue;
		}

		const KGameEventScriptManager::EVENT_DATA* pEventData = SiKGameEventScriptManager()->GetEventData( pEvent->GetScriptID() );
		if( pEventData == NULL )
		{
			START_LOG( cerr, L"�̺�Ʈ ��ũ��Ʈ ������ �����ϴ�.!" )
				<< BUILD_LOG( pEvent->GetUID() )
				<< BUILD_LOG( pEvent->GetType() )
				<< BUILD_LOG( pEvent->GetEventName() )
				<< BUILD_LOG( pEvent->GetScriptID() )
				<< END_LOG;
			continue;
		}

		// ��� ���� Ÿ�� üũ!!
		if( ( cUnitType != CXSLUnit::UT_NONE )
			&& ( pEventData->m_cUnitType != CXSLUnit::UT_NONE )
			&& ( pEventData->m_cUnitType != cUnitType )
			)
		{
			continue;
		}

		if( 0 < iLevel )
		{
			// ��� ���� üũ!!
			bool bCheck = false;
			for( std::map< std::pair< int, int >, int >::const_iterator itMinMaxReward = pEventData->m_mapMinMaxReward.begin(); itMinMaxReward != pEventData->m_mapMinMaxReward.end(); ++itMinMaxReward )
			{
				int iMinLevel = itMinMaxReward->first.first;
				int iMaxLevel = itMinMaxReward->first.second;

				if( ( iLevel < iMinLevel ) || ( iMaxLevel < iLevel ) )
				{
					continue;
				}

				bCheck = true;
				break;
			}

			if( bCheck == false )
			{
				continue;
			}
		}

		tBeginDate		= pEvent->GetBeginDate();
		tEndDate		= pEvent->GetEndDate();
		iTargetLevel	= pEventData->m_iUnitLevel;
		bAccountEvent	= pEventData->m_bAccountEvent;
		return true;
	}

	return false;
}
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
void KGameEventManager::SetTotalEventData( IN const std::map< int,  EVENT_DATA > mapEventScriptData, IN const std::map< int,  EVENT_DATA > mapEventDBData )
{	
	m_mapTotalEventData.clear();

	std::map< int,  EVENT_DATA >::const_iterator cmitEventScript = mapEventScriptData.begin();

	for (; cmitEventScript != mapEventScriptData.end(); ++cmitEventScript )
	{
		m_mapTotalEventData.insert(std::make_pair(cmitEventScript->first, cmitEventScript->second));
	}

	std::map< int,  EVENT_DATA >::const_iterator cmitEventDB = mapEventDBData.begin();

	for (; cmitEventDB != mapEventDBData.end(); ++cmitEventDB )
	{
		std::map< int,  EVENT_DATA >::const_iterator mitTotalEvent = m_mapTotalEventData.find(cmitEventDB->first);

		if (mitTotalEvent == m_mapTotalEventData.end())
		{
			m_mapTotalEventData.insert(std::make_pair(cmitEventDB->first, cmitEventDB->second));
		}
		else
		{
			START_LOG( cerr, L"EventData.lua���� ���� ScriptID �� DB �̺�Ʈ ���� ���� �ϴ� IndexID �� �ߺ��Ǿ����ϴ�. �ش� DB ������ ���� ���� �ʾҽ��ϴ�." )
				<< BUILD_LOG( cmitEventDB->first )
				;
		}
	}
}

const EVENT_DATA* KGameEventManager::GetTotalEventData( IN int iEventID )  const
{
	std::map< int, EVENT_DATA >::const_iterator mit = m_mapTotalEventData.find( iEventID );
	if( mit == m_mapTotalEventData.end() )
	{
		START_LOG( cerr, L"���� ���� �ʴ� �̺�Ʈ�Դϴ�." )
			<< BUILD_LOG( iEventID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

bool KGameEventManager::CheckMapIngEventDataEmpty()
{
	for( int iGET = 0; iGET < KGameEvent::GET_MAX; ++iGET )
	{
		std::map< int, KGameEvent* >::iterator mit;
		for( mit = m_mapIngEventData[iGET].begin(); mit != m_mapIngEventData[iGET].end(); ++mit )
		{
			return false;
		}
	}

	return true;
}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM