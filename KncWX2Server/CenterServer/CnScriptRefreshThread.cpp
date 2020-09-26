#include "CnScriptRefreshThread.h"
#include "CnScriptRefreshManager.h"

#include "CnRoomManager.h"

// include singleton class header file
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLQuestManager.h"
#include "X2Data/XSLDungeonManager.h"
#include "KDropTable.h"
#include "GameEvent/GameEventScriptManager.h"
#include "GameSysVal/GameSysVal.h"
#include "KAttribNpcTable.h"
//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_CN_SERVER_HACKING_CHECK
//}}
#include "AbuserLog/KAbuserLogManager.h"
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
//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}
//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
	#include "X2Data/XSLBuffManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#define CASE_REFRESH_SCRIPT( flag, classname )	_CASE_REFRESH_SCRIPT( flag, classname, OpenScriptFile )

#define _CASE_REFRESH_SCRIPT( flag, classname, functionname ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	START_LOG( cout, L#classname L" ��ũ��Ʈ ������ �����մϴ�!" ); \
	{ \
		KLocker lock( classname::GetTempCS() ); \
		if( SiTemp##classname()->functionname( SiKCnScriptRefreshManager()->GetRefreshLuaState() ) == false ) \
		{ \
			START_LOG( cerr, L"��ũ��Ʈ ���� ����.!" ) \
			<< END_LOG; \
			return; \
		} \
	} \
	break; \


//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT


#define CLASS_TYPE  KCnScriptRefreshThread

KCnScriptRefreshThread::KCnScriptRefreshThread( void )
{
}

KCnScriptRefreshThread::~KCnScriptRefreshThread( void )
{
	End( 15000 );
}

void KCnScriptRefreshThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );

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

	return;
}

template < class T >
void KCnScriptRefreshThread::SendToServer( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, usEventID, data );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

template < class T >
void KCnScriptRefreshThread::SendToCnRoomManager( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, usEventID, data );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
	CASE_REFRESH_SCRIPT( OT_CN_ITEM_MANAGER, CXSLItemManager );
	CASE_REFRESH_SCRIPT( OT_CN_QUEST_MANAGER, CXSLQuestManager );
   _CASE_REFRESH_SCRIPT( OT_CN_DUNGEON_MANAGER, CXSLDungeonManager, OpenScriptFile_AllDungeonScriptLoad );
	CASE_REFRESH_SCRIPT( OT_CN_DROP_TABLE, KDropTable );
	CASE_REFRESH_SCRIPT( OT_CN_GAME_EVENT_SCRIPT_MANAGER, KGameEventScriptManager );
	CASE_REFRESH_SCRIPT( OT_CN_GAME_SYS_VAL, KGameSysVal );
	CASE_REFRESH_SCRIPT( OT_CN_ATTRIB_NPC_TABLE, KAttribNpcTable );
	//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	CASE_REFRESH_SCRIPT( OT_CN_HACKING_CHECK_MANAGER, KCnHackingCheckManager );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	CASE_REFRESH_SCRIPT( OT_CN_ABUSER_LOG_MANAGER, KAbuserLogManager );
	//{{ 2011. 02. 23	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	CASE_REFRESH_SCRIPT( OT_CN_RESULT_PROCESS, KResultProcess );
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_REFRESH_SCRIPT( OT_CN_PVP_MATCH_MANAGER, KPvpMatchManager );
	CASE_REFRESH_SCRIPT( OT_CN_PVP_MATCH_RESULT_TABLE, KPvpMatchResultTable );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	������	�ð� ����
#ifdef SERV_NEW_HENIR_TEST
	CASE_REFRESH_SCRIPT( OT_CN_HENIR_RESULT_TABLE, KHenirResultTable );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _CASE_REFRESH_SCRIPT( OT_CN_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager, OpenScriptFile_AllBattleFieldScriptLoad );
	CASE_REFRESH_SCRIPT( OT_CN_PARTY_MANAGER, CXSLPartyManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_REFRESH_SCRIPT( OT_CN_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	CASE_REFRESH_SCRIPT( OT_CN_AUTO_PARTY_SCRIPT_MANAGER, KAutoPartyScriptManager );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	CASE_REFRESH_SCRIPT( OT_CN_BAD_ATTITUDE_TABLE, KBadAttitudeTable );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_REFRESH_SCRIPT( OT_CN_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE_REFRESH_SCRIPT( OT_CN_DEFENCE_DUNGEON_MANAGER, CXSLDefenceDungeonManager );
	CASE_REFRESH_SCRIPT( OT_CN_BUFF_MANAGER, CXSLBuffManager );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	CASE_REFRESH_SCRIPT( OT_CN_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		return;
	}

	switch( kPacket_.m_iOrderType )
	{
		//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	case KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_LOG_MANAGER:
		{
			// ��� CnServer�� ������!
            SendToServer( ESR_ORDER_TO_REFRESH_MANAGER_ACK, kPacket_ );
		}
		return;
#endif SERV_LOG_SYSTEM_NEW
		//}}

	default:
		break;
	}
	

	// ���ͼ����� ���� �������� CnRoomManager���� ��ũ��Ʈ�� ��ü�ϵ��� ó��!
	SendToCnRoomManager( ESR_ORDER_TO_REFRESH_MANAGER_ACK, kPacket_ );
}


#endif SERV_REALTIME_SCRIPT
//}}


