#include "GlobalScriptRefreshThread.h"
#include "GlobalScriptRefreshManager.h"

#include "BaseServer.h"

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
#include "WorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	#include "X2Data/XSLQuestManager.h"
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS


#define CASE_REFRESH_SCRIPT( flag, classname )	_CASE_REFRESH_SCRIPT( flag, classname, OpenScriptFile )

#define _CASE_REFRESH_SCRIPT( flag, classname, functionname ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	START_LOG( cout, L#classname L" ��ũ��Ʈ ������ �����մϴ�!" ); \
	{ \
		KLocker lock( classname::GetTempCS() ); \
		if( SiTemp##classname()->functionname( SiKGlobalScriptRefreshManager()->GetRefreshLuaState() ) == false ) \
		{ \
			START_LOG( cerr, L"��ũ��Ʈ ���� ����.!" ) \
			<< END_LOG; \
			return; \
		} \
	} \
	break; \


//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT


#define CLASS_TYPE  KGlobalScriptRefreshThread

KGlobalScriptRefreshThread::KGlobalScriptRefreshThread( void )
{
}

KGlobalScriptRefreshThread::~KGlobalScriptRefreshThread( void )
{
	End( 15000 );
}

void KGlobalScriptRefreshThread::ProcessEvent( const KEventPtr& spEvent_ )
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
void KGlobalScriptRefreshThread::SendToServer( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_SERVER, NULL, usEventID, data );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	CASE_REFRESH_SCRIPT( OT_GB_MORNITORING_MANAGER, KMornitoringManager );
#endif SERV_MORNITORING
	//}}
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	CASE_REFRESH_SCRIPT( OT_GB_WORLD_MISSION_MANAGER, KWorldMissionManager );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_REFRESH_SCRIPT( OT_GB_PVP_MATCH_MANAGER, KPvpMatchManager );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	CASE_REFRESH_SCRIPT( OT_GB_QUEST_MANAGER, CXSLQuestManager );
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_REFRESH_SCRIPT( OT_GB_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE_REFRESH_SCRIPT( OT_GB_AUTO_PARTY_SCRIPT_MANAGER, KAutoPartyScriptManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_REFRESH_SCRIPT( OT_GB_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-11	// �ڼ���
	_CASE_REFRESH_SCRIPT( OT_GB_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager, OpenScriptFile_AllBattleFieldScriptLoad );
	CASE_REFRESH_SCRIPT( OT_GB_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		return;
	}

	// ���� �����忡�� ��ü�ϵ��� ó��!
	SendToServer( ESR_ORDER_TO_REFRESH_MANAGER_ACK, kPacket_ );
}


#endif SERV_REALTIME_SCRIPT
//}}


