#include "CnRMEventProcess.h"
#include "CnRoomManager.h"

#include "NetError.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLQuestManager.h"
#include "KDropTable.h"
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include "EventDataRefresh/EventNotifyMsgServerInfo.h"
#else
#include "GameEvent/GameEventScriptManager.h"
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#include "GameEvent/GameEventDBManager.h"
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
#include "GameSysVal/GameSysVal.h"
#include "KAttribNpcTable.h"
//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_CN_SERVER_HACKING_CHECK
//}}
#include "AbuserLog/KAbuserLogManager.h"
#include "Socket/NetCommon.h"
//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	#include "GameEvent/GameEventManager.h"
#endif SERV_EVENT_SCRIPT_REFRESH
//}}
//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	#include "DungeonEventManager.h"
#endif SERV_TIME_DROP_MONSTER_EVENT
//}}
//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	#include "ResultProcess.h"
#endif SERV_DUNGEON_RANK_NEW
//}}
//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
	#include "PvpMatchResultTable.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 08. 24	������	�ð� ����
#ifdef SERV_NEW_HENIR_TEST
	#include "HenirResultTable.h"
#endif SERV_NEW_HENIR_TEST
//}}
//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	#include "BadAttitudeTable.h"
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
	#include "X2Data/XSLBuffManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" ��ũ��Ʈ ������ �Ϸ��Ͽ����ϴ�!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


ImplPfID( KCnRMEventProcess, PI_CN_ROOM );

#define CLASS_TYPE  KCnRMEventProcess

KCnRMEventProcess::KCnRMEventProcess()
{
}

KCnRMEventProcess::~KCnRMEventProcess()
{
}

//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP

ImplToStringW( KCnRMEventProcess )
{
	return START_TOSTRING_PARENTW( KPerformer );
}

#endif SERV_MAX_QUEUE_SIZE_DUMP
//}}

void KCnRMEventProcess::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );

	CASE( DBE_GET_TIME_DROP_INFO_ACK );
	CASE( DBE_CHECK_TIME_DROP_RESULT_ACK );
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	CASE( DBE_ABUSER_LIST_ACK );
	CASE( DBE_EVENT_UPDATE_ACK );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	CASE( DBE_GET_DUNGEON_EVENT_INFO_ACK );
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	CASE( DBE_EVENT_DB_SCRIPT_ACK );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	default:
		START_LOG( cerr, L"�ڵ鷯�� �������� ���� �̺�Ʈ." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_ITEM_MANAGER, CXSLItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_QUEST_MANAGER, CXSLQuestManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_DUNGEON_MANAGER, CXSLDungeonManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_DROP_TABLE, KDropTable );
	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#else
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_GAME_EVENT_SCRIPT_MANAGER, KGameEventScriptManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_GAME_SYS_VAL, KGameSysVal );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_ATTRIB_NPC_TABLE, KAttribNpcTable );
	//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_HACKING_CHECK_MANAGER, KCnHackingCheckManager );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_ABUSER_LOG_MANAGER, KAbuserLogManager );
	//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_RESULT_PROCESS, KResultProcess );
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_PVP_MATCH_MANAGER, KPvpMatchManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_PVP_MATCH_RESULT_TABLE, KPvpMatchResultTable );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	������	�ð� ����
#ifdef SERV_NEW_HENIR_TEST
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_HENIR_RESULT_TABLE, KHenirResultTable );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_PARTY_MANAGER, CXSLPartyManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_AUTO_PARTY_SCRIPT_MANAGER, KAutoPartyScriptManager );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_BAD_ATTITUDE_TABLE, KBadAttitudeTable );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_DEFENCE_DUNGEON_MANAGER, CXSLDefenceDungeonManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_BUFF_MANAGER, CXSLBuffManager );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CN_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}

	// ��� ���̺��� ���� �Ǵ� ���
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DROP_TABLE )
	{
		// DB�κ��� Ư�� �ð� ��� ���� �޾ƿ���!
		KncSend( PI_NULL, 0, PI_LOG_DB, 0, NULL, DBE_GET_TIME_DROP_INFO_REQ, char() );
	}
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_GAME_EVENT_SCRIPT_MANAGER )
	{

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const std::map< int, EVENT_DATA >	mapEventScriptData = SiKGameEventScriptManager()->GetMapEventScriptData();
		const std::map< int, EVENT_DATA >	mapEventDBData = SiKGameEventDBManager()->GetMapEventDBData();

		SiKGameEventManager()->SetTotalEventData( mapEventScriptData, mapEventDBData );

#endif //SERV_EVENT_DB_CONTROL_SYSTEM

		SiKGameEventManager()->RefreshEventScript();
		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		SiKEventNotifyMsgServerInfo()->Init();
#else
		SiKGameEventScriptManager()->Init();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
	}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"CenterServer ��ũ��Ʈ �ǽð� ��ġ �Ϸ�"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

IMPL_ON_FUNC( DBE_GET_TIME_DROP_INFO_ACK )
{
	// �ð� ��� ���� ������Ʈ
	SiKDropTable()->SetTimeDropInfo( kPacket_.m_vecTimeDropInfo, NetCommon::GetLocalIPW() );
}

IMPL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		START_LOG( cerr, L"Ư�� �ð� ��� �̺�Ʈ �����ߴµ� ����� �Գ�? �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_iGSUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_iTimeDropID )
			<< BUILD_LOG( kPacket_.m_iRewardItemID )
			<< BUILD_LOG( kPacket_.m_wstrRewardItemName )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< END_LOG;
		return;
	}

	// �ѹ� ����!
	SiKDropTable()->RollbackTimeDrop( kPacket_.m_iTimeDropID );
}

//{{ 2008. 10. 31  ������	����� ����Ʈ
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_ABUSER_LIST_ACK )
{
	if( kPacket_.m_vecAbuserList.empty() )
		return;

	START_LOG( cout, L"����� ����Ʈ ����!" );

	// ����� ����Ʈ ������Ʈ
	SiKAbuserLogManager()->SetAbuserList( kPacket_.m_iRtt, kPacket_.m_vecAbuserList );
}
#else
IMPL_ON_FUNC( DBE_ABUSER_LIST_ACK )
{
	if( kPacket_.m_mapAbuserList.empty() )
		return;

	START_LOG( cout, L"����� ����Ʈ ����!" );

	// ����� ����Ʈ ������Ʈ
	SiKAbuserLogManager()->UpdateAbuserList( kPacket_.m_mapAbuserList );
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
IMPL_ON_FUNC( DBE_EVENT_UPDATE_ACK )
{
	if( kPacket_.m_vecEventList.size() > 0 )
	{
		SiKGameEventManager()->SetEvent( kPacket_.m_vecEventList );
	}
}

#endif SERV_EVENT_SCRIPT_REFRESH
//}}

//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT

IMPL_ON_FUNC( DBE_GET_DUNGEON_EVENT_INFO_ACK )
{
    SiKDungeonEventManager()->UpdateTimeDropEventMonsterInfo( kPacket_.m_vecEventInfo );

	START_LOG( cout, L"Ư�� �ð� ���� ���� �̺�Ʈ ���� ����!" )
		<< BUILD_LOG( kPacket_.m_vecEventInfo.size() );
}

#endif SERV_TIME_DROP_MONSTER_EVENT
//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM

IMPL_ON_FUNC( DBE_EVENT_DB_SCRIPT_ACK )
{

	if( kPacket_.m_mapEventDBData.size() > 0 )
	{
		SiKGameEventDBManager()->SetEventDBData(kPacket_.m_mapEventDBData);

		const std::map< int, EVENT_DATA >	mapEventScriptData = SiKGameEventScriptManager()->GetMapEventScriptData();
		const std::map< int, EVENT_DATA >	mapEventDBData = SiKGameEventDBManager()->GetMapEventDBData();

		SiKGameEventManager()->SetTotalEventData( mapEventScriptData, mapEventDBData );

		SiKGameEventManager()->RefreshEventScript();

	}
}

#endif //SERV_EVENT_DB_CONTROL_SYSTEM

