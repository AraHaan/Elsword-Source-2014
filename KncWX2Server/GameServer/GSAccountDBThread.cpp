#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSAccountDBThread.h"
#include "GameServer.h"
//{{ 2012. 05. 10	������	���� ����
//#ifdef SERV_VERSION_FLAG
#include "SimLayer.h"
//#endif SERV_VERSION_FLAG
//}}
#include "NetError.h"

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
#include "GameSysVal/GameSysVal.h"
#endif SERV_COME_BACK_USER_REWARD
//}} 

#ifdef SERV_KEY_MAPPING_INT
#define GAMEPAD_ID_MODIFIER 20000
#endif //SERV_KEY_MAPPING_INT

//ImplementDBThread( KGSAccountDBThread );
ImplPfID( KGSAccountDBThread, PI_GS_ACCOUNT_DB );

#define CLASS_TYPE KGSAccountDBThread

IMPL_PROFILER_DUMP( KGSAccountDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.mup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#endif //SERV_ALL_RENEWAL_SP
		continue;

end_proc:
		START_LOG( cerr, vecDump[ui].m_wstrQuery )
			<< BUILD_LOG( vecDump[ui].m_iMinTime )
			<< BUILD_LOG( vecDump[ui].m_iMaxTime )
			<< BUILD_LOG( vecDump[ui].m_iTotalTime )
			<< BUILD_LOG( vecDump[ui].m_iQueryCount )
			<< BUILD_LOG( iAvg )
			<< BUILD_LOG( vecDump[ui].m_iOver1Sec )
			<< BUILD_LOG( vecDump[ui].m_iQueryFail )
			<< END_LOG;
	}
}

KGSAccountDBThread::~KGSAccountDBThread(void)
{
}

void KGSAccountDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
		//{{ 2009. 12. 22  ������	������Ȯ��
		CASE( ELOG_STAT_PLAY_TIME );
		//}}

        CASE_NOPARAM( DBE_UPDATE_LOGIN_TIME_NOT );
        CASE( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ );

	   _CASE( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, KEGS_ADMIN_CHANGE_AUTH_LEVEL_REQ );
	   _CASE( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ );

	   _CASE( DBE_ACCOUNT_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_ACK );
	   //{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	   _CASE( DBE_ACCOUNT_SELECT_UNIT_REQ, KDBE_SELECT_UNIT_ACK );
#else
	   _CASE( DBE_ACCOUNT_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_ACK );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	   //}}
		//{{ 2009. 6. 4  ������	ĳ���� ����
	   _CASE( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_REQ, KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK );
		//}}
#ifdef SERV_UNIT_WAIT_DELETE
	   _CASE( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_ACK );
#else //SERV_UNIT_WAIT_DELETE
	   _CASE( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_ACK );
#endif //SERV_UNIT_WAIT_DELETE

		//{{ 2008. 3. 28  ������  ��õ��
		CASE_NOPARAM( DBE_RECOMMEND_USER_NOT );
		//}}

	   _CASE( DBE_AGREE_HACK_USER_REQ, UidType );
	   _CASE( DBE_REPORT_HACK_USER_NOT, UidType );

	   _CASE( DBE_GET_MY_MESSENGER_SN_REQ, KEGS_GET_MY_MESSENGER_SN_REQ );

		//{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
		CASE( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT );
#endif SERV_IP_ACCOUNT_LOG
		//}}

		//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
		CASE( DBE_GET_SECOND_SECURITY_INFO_REQ );
		CASE( DBE_SEUCCESS_SECOND_SECURITY_REQ );
		_CASE( DBE_FAILED_SECOND_SECURITY_REQ, UidType );
		CASE( DBE_CREATE_SECOND_SECURITY_REQ );
		CASE( DBE_DELETE_SECOND_SECURITY_REQ );
		CASE( DBE_CHANGE_SECOND_SECURITY_PW_REQ );
		CASE( DBE_UPDATE_SECURITY_TYPE_NOT );
#endif SERV_SECOND_SECURITY
		//}}

		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		CASE( DBE_WRITE_COME_BACK_REWARD_NOT );
		CASE( DBE_WRITE_COME_BACK_END_NOT );
		CASE( DBE_WRITE_COME_BACK_LOGOUT_NOT );
#endif SERV_COME_BACK_USER_REWARD
		//}}
		//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
		CASE( DBE_CHECK_RETAINING_USER_REQ );
		CASE( DBE_INSERT_RETAINING_USER_REQ );
#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
		//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		_CASE( DBE_UPDATE_CHANNEL_RANDOMKEY_NOT, KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		CASE( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}

		//{{ 2012. 05. 17	�ڼ���	Ű���� ���� ������ DB���� �а� ����
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
		CASE( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ );
		_CASE( DBE_KEYBOARD_MAPPING_INFO_READ_REQ, UidType );
#endif SERV_KEYBOARD_MAPPING_INFO_RW
		//}}

		//{{ 2012. 05. 29	�ڼ���	ä�� �ɼ� ������ DB���� �а� ����
#ifdef SERV_CHAT_OPTION_INFO_RW
		CASE( DBE_CHAT_OPTION_INFO_WRITE_REQ );
		_CASE( DBE_CHAT_OPTION_INFO_READ_REQ, UidType );
#endif SERV_CHAT_OPTION_INFO_RW
		//}}
		//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		CASE( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT );
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
		CASE( DBE_UPDATE_EVENT_MONEY_REQ );
#endif // SERV_EVENT_MONEY	

		//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
		//CASE_NOPARAM( DBE_BINGO_EVENT_INFO_READ_REQ );
		CASE( DBE_BINGO_EVENT_INFO_WRITE_REQ );
#endif SERV_EVENT_BINGO
		//}}
		//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		CASE_NOPARAM( DBE_2012_WINTER_VACATION_EVENT_NOT );
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		_CASE( DBE_NEW_ACCOUNT_QUEST_REQ, KDBE_NEW_QUEST_REQ );
		_CASE( DBE_ACCOUNT_QUEST_COMPLETE_REQ, KDBE_QUEST_COMPLETE_REQ );
		CASE( DBE_ACCOUNT_QUEST_UPDATE_NOT );
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		//{{ 2012. 12. 19  ��ȯ(���� ����) - ��μ�
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
		CASE( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ );
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
		//}}
		//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		CASE( DBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT );
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}

#ifdef SERV_NEW_PUNISHMENT
		_CASE( DBE_GET_PUNISHMENT_REQ, UidType );
#endif SERV_NEW_PUNISHMENT

		//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		_CASE( DBE_GET_ANTI_ADDICTION_INFO_REQ, UidType );
		CASE( DBE_UPDATE_ANTI_ADDICTION_INFO );
#endif SERV_ANTI_ADDICTION_SYSTEM
		//}}
		//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CASE( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ );
		CASE( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ );
		CASE( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ );
		CASE( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-22	// �ڼ���
		CASE( DBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT );
		CASE_NOPARAM( DBE_BLOCK_COUNT_CHECK_INFO_READ_REQ );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-02	// �ڼ���
		CASE( DBE_HACKING_USER_CHECK_COUNT_INFO_REQ );
		CASE( DBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT );
		CASE( DBE_ACCOUNT_BLOCK_NOT );
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_ENTRY_POINT
        CASE( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ );
#endif SERV_ENTRY_POINT
    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KGSAccountDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

int KGSAccountDBThread::GetMessengerSN( IN UidType iUnitUID, OUT u_int& uiKNMSerialNum )
{
	int iOK = NetError::ERR_ODBC_01;

	const int iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();

	// �ؽ� �޽����� CharacterSN.
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
	DO_QUERY( L"exec dbo.P_MessengerSN_SEL", L"%d, %d", % iServerGroupID % iUnitUID );
#else //SERV_GLOBAL_COMMON
	DO_QUERY( L"exec dbo.P_MessengerSN_SEL_KR", L"%d, %d", % iServerGroupID % iUnitUID );
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_MessengerSN", L"%d, %d", % iServerGroupID % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		int iTemp = 0;
		FETCH_DATA( iTemp );

		uiKNMSerialNum = static_cast<u_int>(iTemp);

		m_kODBC.EndFetch();
	}
	else
	{
		iOK = NetError::ERR_SELECT_UNIT_04;
		goto end_proc;
	}

	iOK = NetError::NET_OK;

end_proc:
	return iOK;
}

//{{ 2009. 12. 22  ������	������Ȯ��
IMPL_ON_FUNC( ELOG_STAT_PLAY_TIME )
{
	int iOK = NetError::ERR_ODBC_01;

	//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
	//{{ 2012. 11. 14	�ڼ���	Field PT �α� �߰�
#ifdef SERV_FIELD_PLAY_TIME_LOG

#ifdef SERV_RENEWAL_SP
	bool bPCBangFlag = kPacket_.m_bIsPcbang;	//m_bIsPcbang �ؿܵ� �ʿ��ؼ� ������ define ������ ��
	int iChannelCode = 0;
	
#ifdef SERV_CONNECT_LOG_CHANNELING
	iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);
#endif //SERV_CONNECT_LOG_CHANNELING

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_AConnectLog_INS", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		% bPCBangFlag	
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		% bPCBangFlag	
		);
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_RENEWAL_SP
#ifdef SERV_CONNECT_LOG_CHANNELING
	int iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);

	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		);
#else SERV_CONNECT_LOG_CHANNELING
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		);

#endif SERV_CONNECT_LOG_CHANNELING
#endif //SERV_RENEWAL_SP
#else

#ifdef SERV_CONNECT_LOG_CHANNELING
	int iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);

	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		);
#else SERV_CONNECT_LOG_CHANNELING
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		);

#endif SERV_CONNECT_LOG_CHANNELING

#endif SERV_FIELD_PLAY_TIME_LOG
	//}}
#else
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		);
#endif SERV_MACHINE_ID_BLOCK
	//}}	

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}
//}}

IMPL_ON_FUNC_NOPARAM( DBE_UPDATE_LOGIN_TIME_NOT )
{
    int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_LoginTime", L"%d", % FIRST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.mup_update_login_time", L"%d", % FIRST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );

        m_kODBC.EndFetch();
    }

end_proc:
    if( iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"�ֱ� �α��� �ð� ������Ʈ ����." )
            << BUILD_LOG( iOK )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
    }

    return;
}

IMPL_ON_FUNC( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ )
{
    KPacketOK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_PlayGuide", L"%d, %d", % LAST_SENDER_UID % kPacket_.m_bOn );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.mup_set_play_guide", L"%d, %d", % LAST_SENDER_UID % kPacket_.m_bOn );
#endif //SERV_ALL_RENEWAL_SP

    int iOK;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

		switch( iOK )
		{
		case 0:
			kPacket.m_iOK = NetError::NET_OK;
			break;

		case -1:
			kPacket.m_iOK = NetError::ERR_CLIENT_STATE;
			break;

		case -11:
			kPacket.m_iOK = NetError::ERR_NEW_ACCOUNT_02;
			break;			
		}
    }

end_proc:
    SendToUser( LAST_SENDER_UID, EGS_CHANGE_OPTION_PLAY_GUIDE_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, KEGS_ADMIN_CHANGE_AUTH_LEVEL_REQ )
{
	KEGS_ADMIN_CHANGE_AUTH_LEVEL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserAuthority_MER", L"N\'%s\', %d", % kPacket_.m_wstrUserID % (int)kPacket_.m_cAuthLevel );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_set_user_authority", L"N\'%s\', %d", % kPacket_.m_wstrUserID % (int)kPacket_.m_cAuthLevel );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_03;
			break;

		case -2:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
			break;

		case -11:
		case -12:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_05;
			break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ )
{
	KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Muser_SEL_Authority", L"%d", % (int)kPacket_.m_cAuthLevel );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_authority_list", L"%d", % (int)kPacket_.m_cAuthLevel );
#endif //SERV_ALL_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		std::wstring wstrUserID;
		std::wstring wstrUserName;
		FETCH_DATA( wstrUserID >> wstrUserName );

		wstrUserID += L" : ";
		wstrUserID += wstrUserName;

		kPacket.m_vecUserIDList.push_back( wstrUserID );
	}

	if( kPacket.m_vecUserIDList.empty() == true )
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_06;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_GET_AUTH_LEVEL_LIST_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_ACK )
{
	//{{ 2012. 05. 10	������	���� ����
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"�系 �׽�Ʈ ���� ���Ӽ������� ���ο� ĳ���Ͱ� �����Ǿ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_kUnitInfo.m_wstrNickName );

		kPacket_.m_iOK = NetError::NET_OK;
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
		DO_QUERY( L"exec dbo.P_MessengerSN_INS", L"%d, %d, %d",
			% LAST_SENDER_UID
			% GetKGameServer()->GetServerGroupID()
			% kPacket_.m_kUnitInfo.m_nUnitUID
			);
#else //SERV_GLOBAL_COMMON
		DO_QUERY( L"exec dbo.P_MessengerSN_INS_KR", L"%d, %d, %d",
			% LAST_SENDER_UID
			% GetKGameServer()->GetServerGroupID()
			% kPacket_.m_kUnitInfo.m_nUnitUID
			);
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_insert_MessengerSN", L"%d, %d, %d",
			% LAST_SENDER_UID
			% GetKGameServer()->GetServerGroupID()
			% kPacket_.m_kUnitInfo.m_nUnitUID
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			kPacket_.m_iOK = NetError::ERR_CREATE_UNIT_07;
			goto end_proc;
		}
	}
	//#endif SERV_VERSION_FLAG
	//}}

end_proc:
	LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iOK) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_CREATE_UNIT_ACK, kPacket_ );
}

//{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
_IMPL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_REQ, KDBE_SELECT_UNIT_ACK )
#else
_IMPL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_ACK )
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}}
{
	//{{ 2012. 05. 10	������	���� ����
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"�系 �׽�Ʈ ���� ���Ӽ������� ĳ���Ͱ� ���õǾ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_kUnitInfo.m_wstrNickName );

		kPacket_.m_iOK = NetError::NET_OK;
		kPacket_.m_kUnitInfo.m_uiKNMSerialNum = static_cast<u_int>(rand()); // dummy���� ����
	}
	else
	{
		kPacket_.m_iOK = GetMessengerSN( kPacket_.m_kUnitInfo.m_nUnitUID, kPacket_.m_kUnitInfo.m_uiKNMSerialNum );

		if( kPacket_.m_kUnitInfo.m_uiKNMSerialNum == 0 )
		{
			START_LOG( cerr, L"�ؽ� �޽���SN���� �̻��մϴ�. ���ο� �޽���SN���� �߱��մϴ�." )
				<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
				<< BUILD_LOG( kPacket_.m_kUnitInfo.m_uiKNMSerialNum )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// �ؽ� �޽���SN ���� �߱�
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kUnitInfo.m_nUnitUID
				);
#else //SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS_KR", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_insert_MessengerSN", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_ALL_RENEWAL_SP		

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"�ؽ��޽���SN ��߱� ����!?" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( GetKGameServer()->GetServerGroupID() )
					<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
					<< END_LOG;

				// ������ �����Ҽ� �ֵ��� ok�� ������
				kPacket_.m_iOK = NetError::NET_OK;
			}
			else
			{
				// �߱޵Ǿ����� SN�� ����!
				kPacket_.m_iOK = GetMessengerSN( kPacket_.m_kUnitInfo.m_nUnitUID, kPacket_.m_kUnitInfo.m_uiKNMSerialNum );
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		kPacket_.m_iEventMoney = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_SEL ", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_GET ", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iEventMoney );

			m_kODBC.EndFetch();
		}
	}
#endif // SERV_EVENT_MONEY

	//{{ 2012. 10. 10	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		ReadBingoEvent( anTrace_, kPacket_.m_kBingoEvent );
		kPacket_.m_iOK = kPacket_.m_kBingoEvent.m_iOK;

		if( kPacket_.m_kBingoEvent.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"DB���� ���� �̺�Ʈ ���� �б� ����!!" )
				<< BUILD_LOG( kPacket_.m_kBingoEvent.m_iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< END_LOG;
		}
	}
#endif SERV_EVENT_BINGO
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
	if( ( kPacket_.m_iOK == NetError::NET_OK ) && ( kPacket_.m_kLocalRankingUserInfo.m_iUserUID == 0 ) )
	{
		// ��Ŀ ���� ���� �б�
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			kPacket_.m_iOK = NetError::ERR_ODBC_01;

			FETCH_DATA( kPacket_.m_iOK
				>> kPacket_.m_kLocalRankingUserInfo.m_byteFilter
				>> kPacket_.m_kLocalRankingUserInfo.m_iCategory
				>> kPacket_.m_kLocalRankingUserInfo.m_byteGender
				>> kPacket_.m_kLocalRankingUserInfo.m_iBirth
				>> kPacket_.m_kLocalRankingUserInfo.m_wstrProfile
				>> kPacket_.m_wstrLocalRankingUserInfoUpdated
				);
			m_kODBC.EndFetch();
		}
	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	// ���� ����Ʈ ������ �о� �;� �Ѵ�.
	{
		//////////////////////////////////////////////////////////////////////////
		// �Ϸ��� ���� ����Ʈ�� �޾� �´�.
		CTime tCompleteDate;
		std::wstring wstrCompleteDate;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// �Ϸ� ��¥ ��ȯ
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_vecAccountCompleteQuest.push_back( kInfo );
		}
		//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// �Ϸ� ��¥ ��ȯ
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_vecAccountCompleteQuest.push_back( kInfo );
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

		//////////////////////////////////////////////////////////////////////////
		// ���� ����(������) ���� ����Ʈ�� �޾� �´�.
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KQuestInstance		kQuest;
			KSubQuestInstance	kSub[5];
			kQuest.m_OwnorUnitUID = kPacket_.m_kUnitInfo.m_nUnitUID;

			FETCH_DATA( kQuest.m_iID
				>> kSub[0].m_ucClearData
				>> kSub[1].m_ucClearData
				>> kSub[2].m_ucClearData
				>> kSub[3].m_ucClearData
				>> kSub[4].m_ucClearData
				);
			for( int i = 0; i < 5; i++ )
				kQuest.m_vecSubQuestInstance.push_back( kSub[i] );

			kPacket_.m_vecAccountQuesting.push_back( kQuest );
		}
	}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
		<< END_LOG;
	//#endif SERV_VERSION_FLAG
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_SELECT_UNIT_ACK, kPacket_ );
}

//{{ 2009. 6. 4  ������	ĳ���� ����
_IMPL_ON_FUNC( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_REQ, KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK )
{
	//{{ 2012. 05. 10	������	���� ����
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"�系 �׽�Ʈ ���� ���Ӽ������� ä�� �̵����� ���� ĳ���Ͱ� �ڵ����� �缱�õǾ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_wstrNickName );

		kPacket_.m_kSelectUnitAck.m_iOK = NetError::NET_OK;

		kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum = static_cast<u_int>(rand()); // dummy���� ����

	}
	else
	{
		kPacket_.m_kSelectUnitAck.m_iOK = GetMessengerSN( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID, kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum );

		if( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum == 0 )
		{
			START_LOG( cerr, L"�ؽ� �޽���SN���� �̻��մϴ�. ���ο� �޽���SN���� �߱��մϴ�." )
				<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
				<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// �ؽ� �޽���SN ���� �߱�
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID
				);
#else //SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS_KR", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP

			DO_QUERY( L"exec dbo.mup_insert_MessengerSN", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_ALL_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"�ؽ��޽���SN ��߱� ����!?" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( GetKGameServer()->GetServerGroupID() )
					<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
					<< END_LOG;

				// ������ �����Ҽ� �ֵ��� ok�� ������
				kPacket_.m_kSelectUnitAck.m_iOK = NetError::NET_OK;
			}
			else
			{
				// �߱޵Ǿ����� SN�� ����!
				kPacket_.m_kSelectUnitAck.m_iOK = GetMessengerSN( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID, kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum );
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
//#endif SERV_VERSION_FLAG
//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	if( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
	{
		kPacket_.m_kSelectUnitAck.m_iEventMoney = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_SEL ", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_GET ", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_kSelectUnitAck.m_iEventMoney );

			m_kODBC.EndFetch();
		}
	}
#endif // SERV_EVENT_MONEY

	//{{ 2012. 10. 10	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	if( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
	{
		ReadBingoEvent( anTrace_, kPacket_.m_kSelectUnitAck.m_kBingoEvent );
		kPacket_.m_kSelectUnitAck.m_iOK = kPacket_.m_kSelectUnitAck.m_kBingoEvent.m_iOK;
	}
#endif SERV_EVENT_BINGO
	//}}

	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	// ���� ����Ʈ ������ �о� �;� �Ѵ�.
	{
		//////////////////////////////////////////////////////////////////////////
		// �Ϸ��� ���� ����Ʈ�� �޾� �´�.
		CTime tCompleteDate;
		std::wstring wstrCompleteDate;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// �Ϸ� ��¥ ��ȯ
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_kSelectUnitAck.m_vecAccountCompleteQuest.push_back( kInfo );
		}

		//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// �Ϸ� ��¥ ��ȯ
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_kSelectUnitAck.m_vecAccountCompleteQuest.push_back( kInfo );
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

		//////////////////////////////////////////////////////////////////////////
		// ���� ����(������) ���� ����Ʈ�� �޾� �´�.
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KQuestInstance		kQuest;
			KSubQuestInstance	kSub[5];
			kQuest.m_OwnorUnitUID = kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID;

			FETCH_DATA( kQuest.m_iID
				>> kSub[0].m_ucClearData
				>> kSub[1].m_ucClearData
				>> kSub[2].m_ucClearData
				>> kSub[3].m_ucClearData
				>> kSub[4].m_ucClearData
				);
			for( int i = 0; i < 5; i++ )
				kQuest.m_vecSubQuestInstance.push_back( kSub[i] );

			kPacket_.m_kSelectUnitAck.m_vecAccountQuesting.push_back( kQuest );
		}
	}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	LOG_SUCCESS( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_kSelectUnitAck.m_iOK ) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
		<< END_LOG;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_ACK, kPacket_ );
}
//}}

#ifdef SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_ACK )
#else //SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_ACK )
#endif //SERV_UNIT_WAIT_DELETE
{
	//{{ 2012. 05. 10	������	���� ����
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"�系 �׽�Ʈ ���� ���Ӽ������� ĳ���Ͱ� ���� �Ǿ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_wstrNickName );

		kPacket_.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket_.m_iOK = GetMessengerSN( kPacket_.m_iUnitUID, kPacket_.m_uiKNMSerialNum );
	}
	//#endif SERV_VERSION_FLAG
	//}}

	LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iOK) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_DELETE_UNIT_ACK, kPacket_ );
}

//{{ 2008. 3. 28  ������  ��õ��
IMPL_ON_FUNC_NOPARAM( DBE_RECOMMEND_USER_NOT )
{
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_IsRecommend", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_isrecommend", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
	int iOK = NetError::ERR_ODBC_01;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"MUser ���̺��� ��õ�� flag ���� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< END_LOG;
	}
}
//}}

_IMPL_ON_FUNC( DBE_AGREE_HACK_USER_REQ, UidType )
{
	KEGS_AGREE_HACK_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserHackerList_UPD", L"%d, %d", % kPacket_ % 1 );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_hackerlist", L"%d, %d", % kPacket_ % 1 );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = NetError::ERR_HACK_USER_01;

		START_LOG( cerr, L"��ŷ ���� ������ ������Ʈ ����.!" )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_AGREE_HACK_USER_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_REPORT_HACK_USER_NOT, UidType )
{
	int iOK = NetError::ERR_ODBC_01;

	// �̸��� ���� ����� ���
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserHackerList_INS", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_hackerlist", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��ŷ���� �Ű��� ����.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( DBE_GET_MY_MESSENGER_SN_REQ, KEGS_GET_MY_MESSENGER_SN_REQ )
{
	KEGS_GET_MY_MESSENGER_SN_ACK kPacket;
	
	//{{ 2012. 05. 10	������	���� ����
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"�系 �׽�Ʈ ���� ���Ӽ������� �޽���SN�� ��û�Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_ );

		kPacket.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket.m_iOK = GetMessengerSN( kPacket_, kPacket.m_uiKNMSerialNum );
	}
	//#endif SERV_VERSION_FLAG
	//}}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�ؽ� �޽��� SN ��û ����.!" )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_GET_MY_MESSENGER_SN_ACK, kPacket );
}

//{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
IMPL_ON_FUNC( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_HackingUser_INS", L"%d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_wstrLoginIP );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_hacking_user", L"%d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_wstrLoginIP );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Ư�� IP�� ���� �����Ǵ� ���� �α� ����� ����!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wstrLoginIP )
			<< END_LOG;
	}
}
#endif SERV_IP_ACCOUNT_LOG
//}}

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY

IMPL_ON_FUNC( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ )
{
    KDBE_GET_SECOND_SECURITY_INFO_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iFailedCount = 0;
    kPacket.m_bUseSecondPW = false;

    //{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
    CTime tCurr = CTime::GetCurrentTime();
    CTime tEndDate;
#endif SERV_COME_BACK_USER_REWARD
    //}} 

    // 2�� ��� ��� : kPacket_ --> UserUID
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_second_pw_Info", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_bUseSecondPW
            >> kPacket.m_wstrSecondPW
            >> kPacket.m_iFailedCount
            >> kPacket.m_wstrLastAuthDate );

        m_kODBC.EndFetch();
    }

    if( kPacket.m_bUseSecondPW == true && kPacket.m_iFailedCount >= 10 )	// 2�� ��й�ȣ �Է� ���� 10ȸ �̻�
    {
        kPacket.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT;

        START_LOG( clog, L"2�� ��й�ȣ �Է� ���� 10ȸ �̻�" )
            << BUILD_LOG( kPacket.m_bUseSecondPW )
            << BUILD_LOG( kPacket.m_iFailedCount );
        goto end_proc;
    }

    kPacket.m_iOK = NetError::NET_OK;

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_GET_PURE_SECOND_SECURITY_INFO_ACK, kPacket );

}

IMPL_ON_FUNC( DBE_GET_SECOND_SECURITY_INFO_REQ )
{
	KDBE_GET_SECOND_SECURITY_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iFailedCount = 0;
	kPacket.m_bUseSecondPW = false;

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	CTime tCurr = CTime::GetCurrentTime();
	CTime tEndDate;
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	// 2�� ��� ��� : kPacket_ --> UserUID
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_second_pw_Info", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_bUseSecondPW
			     >> kPacket.m_wstrSecondPW
				 >> kPacket.m_iFailedCount
				 >> kPacket.m_wstrLastAuthDate );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_bUseSecondPW == true && kPacket.m_iFailedCount >= 10 )	// 2�� ��й�ȣ �Է� ���� 10ȸ �̻�
	{
		kPacket.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT;

		START_LOG( clog, L"2�� ��й�ȣ �Է� ���� 10ȸ �̻�" )
			<< BUILD_LOG( kPacket.m_bUseSecondPW )
			<< BUILD_LOG( kPacket.m_iFailedCount );
		goto end_proc;
	}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	kPacket.m_iRewardStep = 0;
	kPacket.m_bIsFirst = false;
	kPacket.m_bIsComeBackUser = false;

	// DB�� ���� ���� ������ �ִ��� Ȯ���Ѵ�.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_Comebackuser_info", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_bIsComeBackUser
			>> kPacket.m_wstrComeBackBuffEndDate );

		m_kODBC.EndFetch();
	}

	START_LOG( clog, L"������ �޸� ���� ������ DB���� ������ ����!!" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
		<< BUILD_LOG( kPacket.m_bIsComeBackUser )
		<< BUILD_LOG( kPacket.m_iRewardStep  )
		<< END_LOG;

	// DB�� �޸� ���� ������ �ִ�.
	if( kPacket.m_bIsComeBackUser == true )
	{
		if( KncUtil::ConvertStringToCTime( kPacket.m_wstrComeBackBuffEndDate , tEndDate ) == false )
		{
			START_LOG( cerr, L"��¥ ��ȯ ����!!!" )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
				<< END_LOG;
			
			// ���� �ð����� ��ȭ ��Ŵ
			tEndDate = CTime( 2000, 1, 1, 0, 0, 0 );
		}

		// ���� ���� ������ �ð��� ���纸�� ����(���� ����)
		if( tEndDate < tCurr )
		{
			kPacket.m_bIsComeBackUser = false;	// ���� ������ �Ѵ�.

			// LastConnection ������ �̿뿩 ���� �������� �Ǵ�
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUserComeback_Logout_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_get_comeback_logout", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_wstrLastConnectDate );

				m_kODBC.EndFetch();
			}

			// �ð� ������ ��� ���� �ʴٸ�
			if( kPacket.m_wstrLastConnectDate.empty() == false )
			{
				// �޸� �������� �Ǵ��Ͽ� �´ٸ� ���� ��� Ȯ��
				if( CheckIsComeBackUser( kPacket.m_wstrLastConnectDate, kPacket.m_iRewardStep, kPacket.m_wstrComeBackBuffEndDate, kPacket_.m_mapComeBackRewardCondition ) == true )
				{
					kPacket.m_bIsFirst = true;
					kPacket.m_bIsComeBackUser = true;	// ���� ������ �´�!

					START_LOG( clog, L"������ ���� �ð����� �޸� ���� �Ǵ� �� ���� ��� �Ǵ�" )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
						<< BUILD_LOG( kPacket.m_bIsComeBackUser )
						<< BUILD_LOG( kPacket.m_bIsFirst )
						<< BUILD_LOG( kPacket.m_iRewardStep )
						<< END_LOG;
				}
				else
				{
					// ���� ������ �ƴϴ� ����Ÿ �ʱ�ȭ
					kPacket.m_iRewardStep = 0;
					kPacket.m_bIsFirst = false;
					kPacket.m_bIsComeBackUser = false;
					kPacket.m_wstrComeBackBuffEndDate = L"";

					// DB�� ������� - ������ �����ٰ�
					int _iOK = NetError::ERR_ODBC_00;
#ifdef SERV_ALL_RENEWAL_SP
					DO_QUERY( L"exec dbo.P_MUserComeback_UPD", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
					DO_QUERY( L"exec dbo.mup_update_Comebackuser_finish", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
					if( m_kODBC.BeginFetch() )
					{
						FETCH_DATA( _iOK );

						m_kODBC.EndFetch();
					}

					if( _iOK != NetError::NET_OK )
					{
						START_LOG( cerr, L"�߸��� ���� ���� ���� �Է½���!!" )
							<< BUILD_LOG( kPacket_.m_iUserUID )
							<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
							<< BUILD_LOG( kPacket.m_bIsComeBackUser )
							<< BUILD_LOG( _iOK )
							<< END_LOG;
					}

					START_LOG( clog, L"������ ���� �ð��� ���� �޸� ���� ������ �ƴ϶�� �Ǵ�" )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
						<< BUILD_LOG( kPacket.m_bIsComeBackUser )
						<< BUILD_LOG( kPacket.m_bIsFirst )
						<< BUILD_LOG( kPacket.m_iRewardStep )
						<< END_LOG;
				}
			}
		}
		// ���� ���� ������ �ð��� ���纸�� ����(���� ����)
		else
		{
			// ���� ������ �ƴϴ�
			kPacket.m_bIsFirst = false;
			kPacket.m_bIsComeBackUser = true;

			START_LOG( clog, L"�̹� �޸� ���� ����!!" )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
				<< BUILD_LOG( kPacket.m_bIsComeBackUser )
				<< BUILD_LOG( kPacket.m_bIsFirst )
				<< BUILD_LOG( kPacket.m_iRewardStep )
				<< END_LOG;
			//{{ 2012. 05. 16	�ڼ���	ù ���� �� ���̵� ���� ����ֱ�
#ifdef SERV_EVENT_GUIDELINE_POPUP
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUserComeback_Logout_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_get_comeback_logout", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_wstrLastConnectDate );
				m_kODBC.EndFetch();
			}
#endif SERV_EVENT_GUIDELINE_POPUP
			//}}
		}
	}
	// DB�� �޸� ���� ������ ����.
	else
	{
		// LastConnection ������ �̿��Ͽ� ���� �������� �Ǵ�
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserComeback_Logout_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_get_comeback_logout", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_wstrLastConnectDate );

			m_kODBC.EndFetch();
		}

		// �ð� ������ ��� ���� �ʴٸ�
		if( kPacket.m_wstrLastConnectDate.empty() == false )
		{
			// �޸� �������� �Ǵ��Ͽ� �´ٸ� ���� ��� Ȯ��
			if( CheckIsComeBackUser( kPacket.m_wstrLastConnectDate, kPacket.m_iRewardStep, kPacket.m_wstrComeBackBuffEndDate, kPacket_.m_mapComeBackRewardCondition ) == true )
			{
				// ���� �����̴�.
				kPacket.m_bIsFirst = true;
				kPacket.m_bIsComeBackUser = true;

				START_LOG( clog, L"������ ���� �ð����� �޸� ���� �Ǵ� �� ���� ��� �Ǵ�" )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
					<< BUILD_LOG( kPacket.m_bIsComeBackUser )
					<< BUILD_LOG( kPacket.m_bIsFirst )
					<< BUILD_LOG( kPacket.m_iRewardStep )
					<< END_LOG;
			}
			else
			{
				// ���� ������ �ƴϴ� ����Ÿ �ʱ�ȭ
				kPacket.m_iRewardStep = 0;
				kPacket.m_bIsFirst = false;
				kPacket.m_bIsComeBackUser = false;
				kPacket.m_wstrComeBackBuffEndDate = L"";

				START_LOG( clog, L"������ ���� �ð��� ����. �� ������ �����Ͽ���." )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
					<< BUILD_LOG( kPacket.m_bIsComeBackUser )
					<< BUILD_LOG( kPacket.m_bIsFirst )
					<< BUILD_LOG( kPacket.m_iRewardStep )
					<< END_LOG;
			}
		}
	}

#ifdef SERV_COME_BACK_USER_NOT_USE
	kPacket.m_iRewardStep = 0;
	kPacket.m_bIsFirst = false;
	kPacket.m_bIsComeBackUser = false;
	kPacket.m_wstrComeBackBuffEndDate = L"";
#endif // SERV_COME_BACK_USER_NOT_USE

#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_SEL", L"%d", %kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_GET", L"%d", %kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			// �ش� ������ ���� �� -1 ��ȯ
			FETCH_DATA( kPacket.m_wstrWinterVacationEventRegDate
				>>	kPacket.m_iWinterVacationEventCount
				);
			m_kODBC.EndFetch();
		}
		else
		{
			START_LOG( cerr, L"���� ȣ�� ����")
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_ODBC_01;
			goto end_proc;
		}

		CTime tCurrentTime = CTime::GetCurrentTime();

		// kPacket.m_iWinterVacationEventCount < 0 �̶�� ���� ���� DB�� ��ϵǾ� ���� ���� ������� ���̴�.
		// '2012-12-13 00:00:00' ~ '2013-02-14 07:00:00' �Ⱓ ���� ���� ������ ���Դٸ� �̺�Ʈ ���� �ڰ��� ������
		if(	( kPacket.m_iWinterVacationEventCount < 0 )			&&
			( kPacket.m_bIsComeBackUser == true )				&&
			( CTime( 2012, 12, 13, 0, 0, 0 ) <= tCurrentTime )	&&
			( tCurrentTime < CTime( 2013, 2, 14, 7, 0, 0 ) )
			)
		{
			kPacket.m_wstrWinterVacationEventRegDate = tCurrentTime.Format( _T("%Y-%m-%d %H:%M:%S") );

			DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_INT", L"%d, N\'%s\'", % kPacket_.m_iUserUID % kPacket.m_wstrWinterVacationEventRegDate );
			if( m_kODBC.BeginFetch() )
			{
				// �Է� ���� �� 0, ���� �� -1 ��ȯ
				FETCH_DATA( kPacket.m_iWinterVacationEventCount );
				m_kODBC.EndFetch();

				if( kPacket.m_iWinterVacationEventCount < 0 )
				{
					START_LOG( cerr, L"2012 �ܿ� ���� ���� �̺�Ʈ insert ���� ����" )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< END_LOG;
					kPacket.m_iOK = NetError::ERR_ODBC_01;
					goto end_proc;
				}
			}
			else
			{
				START_LOG( cerr, L"���� ȣ�� ����")
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< END_LOG;
				kPacket.m_iOK = NetError::ERR_ODBC_01;
				goto end_proc;
			}
		}
	}
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

		//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//////////////////////////////////////////////////////////////////////////
	//���� �̺�Ʈ �� Ư���� ���� ������
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Elsword_Bubble_Event_SEL", L"N\'%s\'", % kPacket_.m_wstrID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_elsword_bubble_team", L"N\'%s\'", % kPacket_.m_wstrID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iBF_Team );

		m_kODBC.EndFetch();
	}
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}

	//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_GET", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int					iResult = NetError::ERR_ODBC_01;
		int					iItemID;
		KDailyGiftBoxInfo	kDailyGiftBoxInfo;

		FETCH_DATA( iResult
			>> kDailyGiftBoxInfo.m_iUnitUID
			>> iItemID
			>> kDailyGiftBoxInfo.m_iState
			>> kDailyGiftBoxInfo.m_wstrRegDate
			);

		if( iResult == NetError::NET_OK )
		{
			CTime tRegDate;
			if( KncUtil::ConvertStringToCTime( kDailyGiftBoxInfo.m_wstrRegDate, tRegDate ) == true )
			{
				kPacket.m_mmapDailyGiftBoxList.insert( std::multimap<int, KDailyGiftBoxInfo>::value_type( iItemID, kDailyGiftBoxInfo ) );
			}
		}
	}
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	
	//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	{
		short sServerGroupID = static_cast<short>(KBaseServer::GetKObj()->GetServerGroupID());

		DO_QUERY( L"exec dbo.P_MEventReward_GET", L"%d, %d", %LAST_SENDER_UID %sServerGroupID );

		while( m_kODBC.Fetch() )
		{
			KFirstSelectUnitReward kRewardInfo;

			FETCH_DATA( kRewardInfo.m_iDBIndexID
					 >> kRewardInfo.m_iItemID
					 >> kRewardInfo.m_iQuantity
					 >> kRewardInfo.m_cGiveType
					 >> kRewardInfo.m_cRewardType)

			kPacket.m_vecFirstSelectUnitReward.push_back( kRewardInfo );
		}
	}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_SECOND_SECURITY_INFO_ACK, kPacket );

}

IMPL_ON_FUNC( DBE_SEUCCESS_SECOND_SECURITY_REQ )
{

	KDBE_SEUCCESS_SECOND_SECURITY_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_AuthSuccess", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrCurrTime );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_second_pw_authsuccess", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrCurrTime );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK 
				 >> kAck.m_wstrCurrTime );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2�� ���� ���� ���� ��� ����!!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kAck.m_wstrCurrTime )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_SEUCCESS_SECOND_SECURITY_ACK, kAck );
}

_IMPL_ON_FUNC( DBE_FAILED_SECOND_SECURITY_REQ, UidType )
{
	int iOK = NetError::ERR_ODBC_01;
	int iFailedCount = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_AuthFailed", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_second_pw_authfailed", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iFailedCount );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2�� ���� ���� ���� ��� ����!!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( iFailedCount )
			<< END_LOG;
	}
	else
	{
		if( iFailedCount >= 10 )
		{
			START_LOG( clog, L"���� ī��Ʈ�� 10�̻��� ��� 10���� ����!" )
				<< BUILD_LOG( kPacket_ )
				<< BUILD_LOG( iFailedCount )
				<< END_LOG;

			iFailedCount = 10;
		}
	}

end_proc:	
	SendToUser( LAST_SENDER_UID, DBE_FAILED_SECOND_SECURITY_ACK, iFailedCount );
}

IMPL_ON_FUNC( DBE_CREATE_SECOND_SECURITY_REQ )
{
	KDBE_CREATE_SECOND_SECURITY_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_MER_Create", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_create_second_pw", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	kPacket.m_wstrSecondPW = kPacket_.m_wstrSecondPW;

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2�� ���� ���� ����!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserID )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

	}
	SendToUser( LAST_SENDER_UID, DBE_CREATE_SECOND_SECURITY_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DELETE_SECOND_SECURITY_REQ )
{
	KDBE_DELETE_SECOND_SECURITY_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_Delete", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_delete_second_pw", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#endif //SERV_ALL_RENEWAL_SP
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2�� ���� ���� ����!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserID )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

	}
	SendToUser( LAST_SENDER_UID, DBE_DELETE_SECOND_SECURITY_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CHANGE_SECOND_SECURITY_PW_REQ )
{
	KDBE_CHANGE_SECOND_SECURITY_PW_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD", L"%d, N\'%s\', N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrOldSecondPW % kPacket_.m_wstrNewSecondPW );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_second_pw", L"%d, N\'%s\', N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrOldSecondPW % kPacket_.m_wstrNewSecondPW );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	kPacket.m_wstrSecondPW = kPacket_.m_wstrNewSecondPW;

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2�� ���� ��й�ȣ ���� ����!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserID )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW  )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW  )
			<< END_LOG;

	}
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_SECOND_SECURITY_PW_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UPDATE_SECURITY_TYPE_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_Refresh_WEB", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iSecurityType );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_securitytype", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iSecurityType );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK ); // DB���� ��Ģ�� ������ �����ϸ� ������ 0���� ��ȯ
		m_kODBC.EndFetch();
	}
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ���� ��� ���� DB ��� ����!!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iSecurityType )
			<< END_LOG;
	}
}
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
bool KGSAccountDBThread::CheckIsComeBackUser( IN std::wstring& wstrLastDate, OUT int& iRewardStep, OUT std::wstring& wstrEndBuffDate, IN std::map< int, int >& mapComeBackRewardCondition )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTime tLastConnect;

	if( KncUtil::ConvertStringToCTime( wstrLastDate, tLastConnect ) == false )
	{
		START_LOG( cerr, L"�ð� ���� ����" )
			<< BUILD_LOG( wstrLastDate )
			<< END_LOG;
		return false;
	}

	CTimeSpan tSpanLastConnect = tCurr - tLastConnect;

	if( tSpanLastConnect.GetDays() < 0 )
	{
		CStringW cStrCurr, cStrLast;
		cStrCurr.Format(L"%d/%d/%d %d:%d:%d", tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), tCurr.GetHour(), tCurr.GetMinute(), tCurr.GetSecond());
		cStrLast.Format(L"%d/%d/%d %d:%d:%d", tLastConnect.GetYear(), tLastConnect.GetMonth(), tLastConnect.GetDay(), tLastConnect.GetHour(),
			tLastConnect.GetMinute(), tLastConnect.GetSecond());

		START_LOG( clog, L"���� ��¥ ���� �̻��մϴ�" )
			<< BUILD_LOG( tSpanLastConnect.GetDays() )
			<< BUILD_LOG( cStrCurr.GetBuffer())
			<< BUILD_LOG( cStrLast.GetBuffer())
			<< END_LOG;
		return false;
	}

	{
		// �ݺ����� ���鼭 ���� �� �� ��� ���� �ܰ踦 ���Ѵ�.
		std::map< int, int >::iterator	mitMain = mapComeBackRewardCondition.begin();
		for( ; mitMain != mapComeBackRewardCondition.end() ; ++mitMain )
		{
			if( mitMain->second <= tSpanLastConnect.GetDays() )
			{
				if( iRewardStep < mitMain->first )
				{
					iRewardStep = mitMain->first;
				}
			}
		}
	}

	CTimeSpan tSpan;

	switch( iRewardStep )
	{
	case KGameSysVal::CBS_ONE:
		{
			tSpan = CTimeSpan( KGameSysVal::CBSD_ONE , 0, 0, 0);
			tCurr += tSpan;
			wstrEndBuffDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		}break;
	case KGameSysVal::CBS_TWO:
		{
			tSpan = CTimeSpan( KGameSysVal::CBSD_TWO , 0, 0, 0);
			tCurr += tSpan;
			wstrEndBuffDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		}break;
	case KGameSysVal::CBS_THREE:
		{
			tSpan = CTimeSpan( KGameSysVal::CBSD_THREE , 0, 0, 0);
			tCurr += tSpan;
			wstrEndBuffDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		}break;
	default:
		{
			wstrEndBuffDate = L"2000-01-01 00:00:00";
			START_LOG( clog, L"�������� ���� �ܰ谡 �߸� �Ǿ���" )
				<< BUILD_LOG( iRewardStep )
				<< BUILD_LOG( tSpanLastConnect.GetDays() )
				<< END_LOG;
			return false;
		}break;
	}

	return true;
}

bool KGSAccountDBThread::CheckComeBackFirst( IN std::wstring& wstrEndDate )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTime tEndDate;

	if( KncUtil::ConvertStringToCTime( wstrEndDate , tEndDate ) == false )
	{
		START_LOG( cerr, L"��¥ ��ȯ ����!!!" )
			<< BUILD_LOG( wstrEndDate )
			<< END_LOG;
		return false;
	}

	if( tEndDate < tCurr )	// ���� ���� �Ⱓ�� ������.
	{
		// �׷��� ���� ������.
		// ���� �����ؾ� �Ѵ�.
		return true;
	}

	return false;
}

IMPL_ON_FUNC( DBE_WRITE_COME_BACK_REWARD_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	CTime tCurr = CTime::GetCurrentTime();
	std::wstring wstrComeBackRegDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S"));
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_MER", L"%d, %d, N\'%s\', N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_iRewardStep %kPacket_.m_wstrComeBackBuffEndDate %wstrComeBackRegDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_create_Comebackuser_info", L"%d, %d, N\'%s\', N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_iRewardStep %kPacket_.m_wstrComeBackBuffEndDate %wstrComeBackRegDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

	// iOK = -1 : MUser ���̺� �ش� UserUID�� ������
	// iOK = -2 : �޸պ��� ���� ���̺� ���ʷ� ������ insert ��, ���� 
	// iOK = -3 : �޸պ��� ���� ���̺� �̹� ������ �ִ� ���¿��� update ��, ����

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�޸� ���� ������ �Էµ��� �ʾҽ��ϴ�." )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iRewardStep )
			<< BUILD_LOG( kPacket_.m_wstrComeBackBuffEndDate )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_WRITE_COME_BACK_END_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_UPD", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_Comebackuser_finish", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

	// iOK = -1 : MUserComeback ���̺� �ش� UserUID�� ������
	// iOK = -2 : �޸պ��� ���� ���̺� IsComeBackUser = 0 ������Ʈ ��, ����

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�޸� ���� ������ �Էµ��� �ʾҽ��ϴ�." )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( iOK )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_WRITE_COME_BACK_LOGOUT_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_Logout_MER", L"%d, N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_wsrtLogOutDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_comeback_logout", L"%d, N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_wsrtLogOutDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

	// dbo.muser �� �ش� useruid ������ @iOK = -1
	// dbo.MUserComeback_Logout �μ�Ʈ �� ���� @iOK = -2
	// dbo.MUserComeback_Logout ������Ʈ �� ���� @iOK = -3

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�޸� ���� ������ �α׾ƿ� ��� ����!!" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wsrtLogOutDate )
			<< BUILD_LOG( iOK )
			<< END_LOG;
	}
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( DBE_CHECK_RETAINING_USER_REQ )
{
	KDBE_CHECK_RETAINING_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_RetainingUser_SEL", L"%d", 
		% kPacket_.m_iUserUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_Retaining_User", L"%d", 
		% kPacket_.m_iUserUID
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_wstrDate  );
		m_kODBC.EndFetch();
	}

end_proc:

	if( kPacket.m_wstrDate.empty() == true )  // ����� -> �������� ���� 
	{
		kPacket.m_wstrDate = L"2000-01-01 00:00:00";   // �ʱ�ȭ ���
	}
	
	SendToUser( LAST_SENDER_UID, DBE_CHECK_RETAINING_USER_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_RETAINING_USER_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_RetainingUser_INS", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iItemID
		% kPacket_.m_wstrRegDate
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_Retaining_User", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iItemID
		% kPacket_.m_wstrRegDate
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��Ż ���� ������ ���� ���� ��� ����" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;

		kPacket_.m_iUserUID = 0;   // �α׸� ������ �ʱ� ���� ��ġ
	}
    
	SendToUser( LAST_SENDER_UID, DBE_INSERT_RETAINING_USER_ACK, kPacket_ );
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
_IMPL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_NOT, KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_00;

	// ID Ư�� ���ڰ� ���� �Ǿ� �ִٸ� 
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUserID ) )
	{
		START_LOG( cout, L"Ư�� ���� ����� ID, PW �Է����� ���� ����!")
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_iRandomKey )
			<< END_LOG;

		goto end_proc;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserAuthentication_MER", L"N\'%s\', %d", % kPacket_.m_wstrUserID % kPacket_.m_iRandomKey );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_user_authkey", L"N\'%s\', %d", % kPacket_.m_wstrUserID % kPacket_.m_iRandomKey );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"User ���� RandomKey ��� ����.!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOGc( kPacket_.m_iRandomKey )
			<< END_LOG;
	}
}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
IMPL_ON_FUNC( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ )
{
	kPacket_.m_iOK = NetError::NET_OK;
	std::string	strMachineID;
	__int64	iServerSN = 0;
	int	iResult = -1;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Temp_MID_SN_SEL_IsAbnormalSN", L"%d", % kPacket_.m_iServerSN );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_serversn_check", L"%d", % kPacket_.m_iServerSN );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iResult );
		m_kODBC.EndFetch();
	}

	if( iResult == 1 ) // ���� ���� SN �����Ѵ�
	{
		kPacket_.m_iOK = NetError::ERR_SERVERSN_07;
		goto end_proc;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Temp_MID_SN_SEL", L"%d, N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_mid_sn", L"%d, N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( strMachineID
				 >> iServerSN
				 >> iResult     );

		m_kODBC.EndFetch();
	}

	bool bCompareSN = false;
	bool bCompareMid = false;
	bool bExistSN	= false;
	bool bExistMid	= false;

	if( iServerSN > 0 ) // DB�� �ӽ�ID�� �����Ѵ�.
	{
		bExistMid = true;
		// MachineID�� ��Ī�� �Ǵ� SN�� Ŭ���̾�Ʈ���� ���� SN�� ������
		if( iServerSN == kPacket_.m_iServerSN )
		{
			bCompareSN = true;
		}
	}

	if( strMachineID.empty() == false ) // DB�� sn�� �����Ѵ�.
	{
		bExistSN = true;
		if( strMachineID.compare( kPacket_.m_strMachineID ) == 0 )
		{
			bCompareMid = true;
		}
	}


//////////////////////////////////////////////////////////////////////////
	if( bExistSN == true )
	{
		if( bExistMid == true )
		{
			if( bCompareSN == true )
			{
				if( bCompareMid == true )
				{
					// ����
				}
				else
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// �� ���� �ϴµ� mid �� ���� �ʴ�?
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_1" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
			else
			{
				if( bCompareMid == true )
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// �� ���� �ϴµ� sn �� ���� �ʴ�?
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_2" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// a/2, b/1 
					// �����ϱ�� ������ ��� ���� �ʴ� �������̴�.  ��κ��� �̰��̴�.
					// ��߱�, ����, ����
					kPacket_.m_iOK = NetError::ERR_SERVERSN_01;

					START_LOG( clog, L"Server SN - ��߱�, ����, ����" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
		else
		{
			if( bCompareSN == true )
			{
				kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

				// Ŭ�� �� mid �� ã�� sn�� ���µ� bCompareSN �� true �� �� �ֳ�?
				START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_3" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else
			{
				if( bCompareMid == true )
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// Ŭ�� �� mid �� ã�� sn�� ���µ� bCompareMid �� true �� �� �ֳ�?
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_4" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// c/1 
					// Ŭ�� �� mid �� ã�� sn�� Ŭ�� �� sn���� ã�� mid�� �ִ�.
					// ������Ʈ, ����, ����					
					kPacket_.m_iOK = NetError::ERR_SERVERSN_03;

					START_LOG( clog, L"MachineID ���� update" )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
	}
	else
	{
		if( bExistMid == true )
		{
			if( bCompareSN == true )
			{
				kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

				// Ŭ�� �� sn �� �������� �ʴµ� bCompareMid �� bCompareSN �� True �� �� �� ����.
				START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_4" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else
			{
				if( bCompareMid == true )
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// Ŭ�� �� sn �� �������� �ʴµ� bCompareMid �� bCompareSN �� True �� �� �� ����.
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_5" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// a/3
					// Ŭ�� �� sn �� ���������� mid�� ����. 
					// ��߱�, ����, ����
					kPacket_.m_iOK = NetError::ERR_SERVERSN_05;

					START_LOG( clog, L"Server SN - ��߱�, ����, ����" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
		else
		{
			if( bCompareSN == true || bCompareMid == true )
			{
				kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

				// Ŭ�� �� sn �� mid �� �������� �ʴµ� bCompareMid �� bCompareSN �� True �� �� �� ����.
				START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_6" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else //( bCompareSN == false && bCompareMid == false )
			{
				// c/3
				// Ŭ�� �� sn, mid �� ����. ���� ����� ����
				// �߱�, ����, ����
				kPacket_.m_iOK = NetError::ERR_SERVERSN_02;

				START_LOG( clog, L"Server SN - �߱�, ����, ����" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////

	/*
	// DB�� ��� ���� ���� MID, SN �̴�. - ������ ���� - channel server ���� �߱� �Ǿ���
	if( bExistSN == false || bExistMid == false )
	{
		START_LOG( cerr, L"GameServe Server SN Ȯ�� - �������� �ʴ´�." )
			<< BUILD_LOG( kPacket_.m_iServerSN )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< BUILD_LOG( iServerSN )
			<< BUILD_LOG( strMachineID )
			<< BUILD_LOG( bExistSN )
			<< BUILD_LOG( bExistMid )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_SERVERSN_01;
		goto end_proc;
	}
	// ��Ī ���� �ʴ� MID �� SN �� ���� �ʴ�.
	if( bCompareSN == false || bCompareMid == false )
	{
		START_LOG( cerr, L"GameServe Server SN Ȯ�� - ���������� ���� �ʴ�." )
			<< BUILD_LOG( kPacket_.m_iServerSN )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< BUILD_LOG( iServerSN )
			<< BUILD_LOG( strMachineID )
			<< BUILD_LOG( bCompareSN )
			<< BUILD_LOG( bCompareMid )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_SERVERSN_02;
		goto end_proc;
	}
	*/

	//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_SNMonitoring_SEL", L"%d", % kPacket_.m_iServerSN );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iMonitoringFlag );
			m_kODBC.EndFetch();
		}

		START_LOG( clog, L"SN����͸� ���� �� �޾� �Դ°�?" )
			<< BUILD_LOG( kPacket_.m_iServerSN )
			<< BUILD_LOG( kPacket_.m_iMonitoringFlag )
			<< END_LOG;
	}
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHECK_SERVER_SN_IN_GAMESERVER_ACK, kPacket_ );
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

//{{ 2012. 05. 17	�ڼ���	Ű���� ���� ������ DB���� �а� ����
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
IMPL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ )
{
	KEGS_KEYBOARD_MAPPING_INFO_WRITE_ACK kPacket;
	std::map<short, short>::const_iterator cit;

	kPacket.m_iOK = NetError::NET_OK;

#ifdef SERV_KEY_MAPPING_INT
	for( cit = kPacket_.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.begin(); cit != kPacket_.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.end(); ++cit )
	{
		short sAction = cit->first + GAMEPAD_ID_MODIFIER;
		short sKey = cit->second;
		kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.insert( std::make_pair( sAction, sKey ) );
	}
#endif //SERV_KEY_MAPPING_INT

	for( cit = kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.begin(); cit != kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.end(); ++cit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserKeyBoard_MER", L"%d, %d, %d", % kPacket_.m_iUserUID % cit->first % cit->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserKeyBoard_INT", L"%d, %d, %d", % kPacket_.m_iUserUID % cit->first % cit->second );
#endif //SERV_ALL_RENEWAL_SP
		int iResult = -1;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iResult );
			m_kODBC.EndFetch();
		}

		if( iResult != NetError::NET_OK )
		{
			kPacket.m_iOK = -1;
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( cit == kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.begin() )
		{
			// �Ѱ��� ��� ���ߴ�.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_01;
		}
		else
		{
			// �Ϻθ� ����ߴ�.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_02;
		}
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_KEYBOARD_MAPPING_INFO_WRITE_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_READ_REQ, UidType )
{
	KEGS_KEYBOARD_MAPPING_INFO_NOT kPacket;
	
	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserKeyBoard_SEL", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserKeyBoard_GET", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int iResult;
		short iKey;
		short iValue;
		FETCH_DATA( iResult
			>> iKey
			>> iValue
			);

		if( iResult != NetError::NET_OK )
		{
			if( iResult != -1 )
			{
				kPacket.m_iOK = iResult;
			}
		}
		else
		{
#ifdef SERV_KEY_MAPPING_INT
			if( iKey >= GAMEPAD_ID_MODIFIER )
				kPacket.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.insert( std::map<int, int>::value_type( iKey - GAMEPAD_ID_MODIFIER, iValue ) );
			else
#endif //SERV_KEY_MAPPING_INT
			kPacket.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.insert( std::map<int, int>::value_type( iKey, iValue ) );
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( kPacket.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.empty() == true )
		{
			// �Ѱ��� �������� ���ߴ�.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_03;
		}
		else
		{
			// �Ϻθ� �����Դ�.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_04;
		}
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_KEYBOARD_MAPPING_INFO_READ_ACK, kPacket );
}
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 05. 29	�ڼ���	ä�� �ɼ� ������ DB���� �а� ����
#ifdef SERV_CHAT_OPTION_INFO_RW
IMPL_ON_FUNC( DBE_CHAT_OPTION_INFO_WRITE_REQ )
{
	KEGS_CHAT_OPTION_INFO_WRITE_ACK kPacket;

	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_INT", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_cIndex % kPacket_.m_bValue );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_INT", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_cIndex % kPacket_.m_bValue );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = NetError::ERR_CHAT_OPTION_01;
		START_LOG( cerr, L"ä�� �ɼ� ���� ���� ����!!" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_cIndex )
			<< BUILD_LOG( kPacket_.m_bValue )
			<< END_LOG;
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHAT_OPTION_INFO_WRITE_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_CHAT_OPTION_INFO_READ_REQ, UidType )
{
	KEGS_CHAT_OPTION_INFO_NOT kPacket;

	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_SEL", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_GET", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		byte cIndex;
		bool bValue;
		FETCH_DATA( kPacket.m_iOK
			>> cIndex
			>> bValue
			);

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			kPacket.m_mapChatOptionSetInfo.insert( std::map<byte, bool>::value_type( cIndex, bValue ) );
		}
		else if( kPacket.m_iOK == -1 )
		{
			kPacket.m_iOK = NetError::NET_OK;
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( kPacket.m_mapChatOptionSetInfo.empty() == true )
		{
			// �Ѱ��� �������� ���ߴ�.
			kPacket.m_iOK = NetError::ERR_CHAT_OPTION_02;
		}
		else
		{
			// �Ϻθ� �����Դ�.
			kPacket.m_iOK = NetError::ERR_CHAT_OPTION_03;
		}
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHAT_OPTION_INFO_READ_ACK, kPacket );
}
#endif SERV_CHAT_OPTION_INFO_RW
//}}


//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
IMPL_ON_FUNC( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT )
{
	int iResult = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_MER", L"%d, %d, %d, %d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iItemID % kPacket_.m_iState % kPacket_.m_wstrRegDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_INT", L"%d, %d, %d, %d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iItemID % kPacket_.m_iState % kPacket_.m_wstrRegDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iResult );
		m_kODBC.EndFetch();
	}

	if( iResult != NetError::NET_OK )
	{
		if( iResult == -2 )
		{
			START_LOG( cerr, L"�̹� �����ϴ� �����Դϴ�.")
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"������ DailyGiftBox ������ ��Ͽ� �����Ͽ����ϴ�.")
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< BUILD_LOG( kPacket_.m_iState )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< END_LOG;
		}
	}
end_proc:
	return;
}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
IMPL_ON_FUNC( DBE_UPDATE_EVENT_MONEY_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MEventPoint_MER", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iOldQuantity 
		% kPacket_.m_iNewQuantity 
		% kPacket_.m_iRewardItemID 
		% kPacket_.m_wstrRegDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MEventPoint_UPD", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iOldQuantity 
		% kPacket_.m_iNewQuantity 
		% kPacket_.m_iRewardItemID 
		% kPacket_.m_wstrRegDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"õ���� ���� ������Ʈ ����" )
			<< BUILD_LOG( kPacket_.m_iUserUID  )
			<< BUILD_LOG( kPacket_.m_iUnitUID  )
			<< BUILD_LOG( kPacket_.m_iOldQuantity  )
			<< BUILD_LOG( kPacket_.m_iNewQuantity  )
			<< BUILD_LOG( kPacket_.m_iRewardItemID )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UPDATE_EVENT_MONEY_ACK, kPacket_.m_iOldQuantity + kPacket_.m_iNewQuantity );
}
#endif // SERV_EVENT_MONEY

//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
void KGSAccountDBThread::ReadBingoEvent( IN const UidType anTrace_[], OUT KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket )
//IMPL_ON_FUNC_NOPARAM( DBE_BINGO_EVENT_INFO_READ_REQ )
{
	//KDBE_BINGO_EVENT_INFO_READ_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//----------------------------------------------------//
	// ��ȸ ��� �б�
	DO_QUERY( L"exec dbo.P_MEvent_Bingo_UserInfo_GET", L"%d", 
		% LAST_SENDER_UID
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iBalance
			>> kPacket.m_iLotteryChance
			>> kPacket.m_iLotteryRecord
			>> kPacket.m_iMixChance
			>> kPacket.m_iResetChance
			);
		m_kODBC.EndFetch();
	}

	//----------------------------------------------------//

	byte	i;
	byte	iPos;
	int		iValue;
	bool	bOpen;

	// ������ ���� �б�
	DO_QUERY( L"exec dbo.P_MEvent_Bingo_GET", L"%d", 
		% LAST_SENDER_UID
		);

	// ���� �� ���� ���� ���� ���� üũ�� ���� �Ϻη� ��ĭ �� �ޱ⸦ �õ��Ѵ�.
	kPacket.m_vecBingoBoard.resize( BSV_BINGOBOARD_SLOT_NUM_CHECK );

	for( i=0; i < BSV_BINGOBOARD_SLOT_NUM_CHECK; ++i )
	{
		if( m_kODBC.Fetch() )
		{
			FETCH_DATA( iPos
				>> iValue
				>> bOpen
				);
			kPacket.m_vecBingoBoard[iPos] = ( bOpen ) ? iValue + 100 : iValue;
		}
		else
		{
			kPacket.m_vecBingoBoard.resize( i );
			break;
		}
	}

	if( i != BSV_BINGOBOARD_SLOT_NUM )
	{
		kPacket.m_vecBingoBoard.clear();
	}

	//----------------------------------------------------//
	// ���� ���� �б�
	DO_QUERY( L"exec dbo.P_MEvent_Bingo_Present_GET", L"%d", 
		% LAST_SENDER_UID
		);

	// ���� �� ���� ���� ���� ���� üũ�� ���� �Ϻη� ��ĭ �� �ޱ⸦ �õ��Ѵ�.
	kPacket.m_vecPresentInfo.resize( BSV_PRESENT_NUM_CHECK );

	for( i=0; i < BSV_PRESENT_NUM_CHECK; ++i )
	{
		if( m_kODBC.Fetch() )
		{
			FETCH_DATA( iPos
				>> iValue
				>> bOpen
				);
			kPacket.m_vecPresentInfo[iPos] = std::make_pair( iValue, bOpen );
		}
		else
		{
			kPacket.m_vecPresentInfo.resize( i );
			break;
		}
	}

	if( i != BSV_PRESENT_NUM )
	{
		kPacket.m_vecBingoBoard.clear();
	}

	//----------------------------------------------------//

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	return;
//	SendToUser( LAST_SENDER_UID, DBE_BINGO_EVENT_INFO_READ_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_INFO_WRITE_REQ )
{
	KDBE_BINGO_EVENT_INFO_WRITE_ACK kPacket;
	int iOK = NetError::ERR_ODBC_01;

	kPacket.m_usAckEventID = kPacket_.m_usAckEventID;
	//----------------------------------------------------//

	if( kPacket_.m_bChanceChanged == true )
	{
		iOK = NetError::ERR_ODBC_01;
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_UserInfo_INT", L"%d, %d, %d, %d, %d, %d", 
			% LAST_SENDER_UID
			% kPacket_.m_iBalance
			% kPacket_.m_iLotteryChance
			% kPacket_.m_iLotteryRecord
			% kPacket_.m_iMixChance
			% kPacket_.m_iResetChance
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ��ȸ ���� ������Ʈ ����")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iBalance )
				<< BUILD_LOG( kPacket_.m_iLotteryChance )
				<< BUILD_LOG( kPacket_.m_iLotteryRecord )
				<< BUILD_LOG( kPacket_.m_iMixChance )
				<< BUILD_LOG( kPacket_.m_iResetChance )
				<< END_LOG;

			kPacket.m_iBalance			= kPacket_.m_iBalance;
			kPacket.m_iLotteryChance	= kPacket_.m_iLotteryChance;
			kPacket.m_iLotteryRecord	= kPacket_.m_iLotteryRecord;
			kPacket.m_iMixChance		= kPacket_.m_iMixChance;
			kPacket.m_iResetChance		= kPacket_.m_iResetChance;
			kPacket.m_bChanceChanged	= false;
		}
	}

	//----------------------------------------------------//
	// ������ ���� ����
	for( std::map<byte, byte>::const_iterator it=kPacket_.m_mapBingoBoard.begin(); it != kPacket_.m_mapBingoBoard.end(); ++it )	// ���� ������
	{
		iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_MER", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% ( ( it->second < BSV_NUM_MAX ) ? it->second : it->second - BSV_NUM_MAX )
			% ( BSV_NUM_MAX <= it->second )
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_INT", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% ( ( it->second < BSV_NUM_MAX ) ? it->second : it->second - BSV_NUM_MAX )
			% ( BSV_NUM_MAX <= it->second )
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"������ ���� ������Ʈ ����")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( it->first )
				<< BUILD_LOG( it->second )
				<< END_LOG;
			kPacket.m_mapBingoBoard.insert( std::map<byte, byte>::value_type( it->first, it->second ) );
		}
	}

	//----------------------------------------------------//
	// ���� ���� ����
	for( std::map<byte, std::pair<int, bool> >::const_iterator it=kPacket_.m_mapPresentInfo.begin(); it != kPacket_.m_mapPresentInfo.end(); ++it )	// ���� ����
	{
		iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Present_MER", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% it->second.first
			% it->second.second
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Present_INT", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% it->second.first
			% it->second.second
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ���� ���� ������Ʈ ����")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( it->first )
				<< BUILD_LOG( it->second.first )
				<< BUILD_LOG( it->second.second )
				<< END_LOG;
			kPacket.m_mapPresentInfo.insert( std::map<byte, std::pair<int, bool> >::value_type( it->first, std::make_pair( it->second.first, it->second.second ) ) );
		}
	}

	//----------------------------------------------------//
	// �α� ���� ����
	BOOST_TEST_FOREACH( KBingoEventLog, kBingoEventLog, kPacket_.m_vecLog )
	{
		iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Log_INS", L"%d, %d, %d, %d, %d, %d, N\'%s\'", 
			% LAST_SENDER_UID
			% kBingoEventLog.iUnitUID
			% kBingoEventLog.iActionType
			% kBingoEventLog.iPos
			% kBingoEventLog.iOpenNum
			% kBingoEventLog.iItemID
			% kBingoEventLog.wstrRegDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Log_INT", L"%d, %d, %d, %d, %d, %d, N\'%s\'", 
			% LAST_SENDER_UID
			% kBingoEventLog.iUnitUID
			% kBingoEventLog.iActionType
			% kBingoEventLog.iPos
			% kBingoEventLog.iOpenNum
			% kBingoEventLog.iItemID
			% kBingoEventLog.wstrRegDate
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� �̺�Ʈ �α� ���� ������Ʈ ����")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kBingoEventLog.iUnitUID )
				<< BUILD_LOG( kBingoEventLog.iActionType )
				<< BUILD_LOG( kBingoEventLog.iPos )
				<< BUILD_LOG( kBingoEventLog.iOpenNum )
				<< BUILD_LOG( kBingoEventLog.iItemID )
				<< BUILD_LOG( kBingoEventLog.wstrRegDate )
				<< END_LOG;
			kPacket.m_vecLog.push_back( kBingoEventLog );
		}
	}

	//----------------------------------------------------//

end_proc:
	SendToUser( LAST_SENDER_UID, kPacket_.m_usAckEventID, kPacket );
}
#endif SERV_EVENT_BINGO
//}}

//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
IMPL_ON_FUNC_NOPARAM( DBE_2012_WINTER_VACATION_EVENT_NOT )
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan tGab = tCurrentTime - CTime( 2012, 12, 13, 0, 0, 0 );

	DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_UPT", L"%d, %d", % FIRST_SENDER_UID % tGab.GetTotalSeconds() );
	if( m_kODBC.BeginFetch() )
	{
		int iTemp = 0;
		FETCH_DATA( iTemp );
		m_kODBC.EndFetch();

		if( iTemp != 0 )
		{
			START_LOG( cerr, L"������ �̺�Ʈ �Ϸ� �ð� ��� ����!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( tGab.GetTotalSeconds() )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"������ �̺�Ʈ �Ϸ� �ð� ��� ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( tGab.GetTotalSeconds() )
			<< END_LOG;
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_2012_WINTER_VACATION_EVENT_NOT, static_cast<int>( tGab.GetTotalSeconds() ) );
	return;
}
#endif SERV_2012_WINTER_VACATION_EVENT
//}}

//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
_IMPL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_REQ, KDBE_NEW_QUEST_REQ )
{
	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	KDBE_NEW_ACCOUNT_QUEST_ACK kPacket;
	kPacket.m_iOK		= NetError::ERR_ODBC_01;
	kPacket.m_kQuestReq	= kPacket_;

	//////////////////////////////////////////////////////////////////////////
	// ���� ����Ʈ ����
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Mquests_MER", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_UserUID 
		% kPacket_.m_iQuestID 
		% 0 
		% 0 
		% 0 
		% 0
		% 0
		% wstrNow  );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MQuests_INS_UPT", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_UserUID 
		% kPacket_.m_iQuestID 
		% 0 
		% 0 
		% 0 
		% 0
		% 0
		% wstrNow  );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ����Ʈ DB ���� ����." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_NEW_ACCOUNT_QUEST_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_REQ, KDBE_QUEST_COMPLETE_REQ )
{
	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	KDBE_ACCOUNT_QUEST_COMPLETE_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_kQuestReq		= kPacket_;

	std::wstring wstrCompleteDate;

	//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	if( kPacket_.m_bIsRepeat == true )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_INS", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_INT", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#endif //SERV_ALL_RENEWAL_SP
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_INS", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_INT", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#endif //SERV_ALL_RENEWAL_SP
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ����Ʈ �Ϸ� DB ������Ʈ ����." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}
	else
	{
		CTime tCompleteDate;
		LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );

		// ����Ʈ �Ϸ� ������Ʈ ����!
		kPacket.m_tCompleteTime = tNow.GetTime();
	}
#else
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MQuests_Complete_INS", L"%d, %d, %d, N\'%s\'", 
		% FIRST_SENDER_UID 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iQuestID
		% wstrNow   );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MQuests_Complete_INT", L"%d, %d, %d, N\'%s\'", 
		% FIRST_SENDER_UID 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iQuestID
		% wstrNow   );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ����Ʈ �Ϸ� DB ������Ʈ ����." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}
	else
	{
		CTime tCompleteDate;
		LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );

		// ����Ʈ �Ϸ� ������Ʈ ����!
		kPacket.m_tCompleteTime = tNow.GetTime();
	}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_QUEST_COMPLETE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_UPDATE_NOT )
{
	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	BOOST_TEST_FOREACH( KQuestUpdate&, kInfo, kPacket_.m_vecAccountQuesting )
	{
		if( kInfo.m_vecClearData.size() != 5 )
		{
			START_LOG( cerr, L"����Ʈ Ŭ���� ������ 5���� �ƴϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kInfo.m_iQuestID )
				<< BUILD_LOG( kInfo.m_vecClearData.size() )
				<< END_LOG;
			continue;
		}
		
		int iOK = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_SEL", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kPacket_.m_iUserUID 
			% kInfo.m_iQuestID 
			% (int)kInfo.m_vecClearData[0]
			% (int)kInfo.m_vecClearData[1]
			% (int)kInfo.m_vecClearData[2]
			% (int)kInfo.m_vecClearData[3]
			% (int)kInfo.m_vecClearData[4]
			% wstrNow  );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_INS_UPT", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kPacket_.m_iUserUID 
			% kInfo.m_iQuestID 
			% (int)kInfo.m_vecClearData[0]
			% (int)kInfo.m_vecClearData[1]
			% (int)kInfo.m_vecClearData[2]
			% (int)kInfo.m_vecClearData[3]
			% (int)kInfo.m_vecClearData[4]
			% wstrNow  );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� ����Ʈ DB ���� ����." )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kInfo.m_iQuestID )
				<< BUILD_LOG( iOK )
				<< END_LOG;
		}
	}

	return;
end_proc:
	START_LOG( cerr, L"�ƿ�!!!!���� ����Ʈ DB ���� ����." )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< END_LOG;
}
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2012. 12. 19  ��ȯ(���� ����) - ��μ�
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
IMPL_ON_FUNC( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ )
{
	KDBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_00;
	kAck.m_kDB = kPacket_.m_kToDB;

	// ������� �ϳ� ���̶�� �˻縦 �տ��� �߱� ������...
	std::map<int, int>::iterator mit = kPacket_.m_kToDB.m_mapResultItem.begin();
	if( mit != kPacket_.m_kToDB.m_mapResultItem.end() )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_ExchangeQuantity_UPD", L"%d, %d, %d", 
			% kPacket_.m_kReq.m_iSourceItemID
			% mit->first
			% mit->second	  );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_ExchangeQuantity_UPT", L"%d, %d, %d", 
			% kPacket_.m_kReq.m_iSourceItemID
			% mit->first
			% mit->second	  );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kAck.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� ����Ʈ DB ���� ����." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_kReq.m_iSourceItemID )
				<< BUILD_LOG( kPacket_.m_kReq.m_iDestItemID )
				<< BUILD_LOG( kAck.m_iOK )
				<< END_LOG;
		}
	}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK, kAck );
}
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
//}}

//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
IMPL_ON_FUNC( DBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT )
{
	DO_QUERY( L"exec dbo.P_MEventReward_DEL", L"%d, %d, %d", %LAST_SENDER_UID %kPacket_.m_iUnitUID %kPacket_.m_iDBIndexID );

	int iOK = NetError::ERR_ODBC_00;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ���ý� ���� ������ ���� ���� ���� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iDBIndexID )
			<< END_LOG;
	}
end_proc: ;
}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

#ifdef SERV_NEW_PUNISHMENT
_IMPL_ON_FUNC( DBE_GET_PUNISHMENT_REQ, UidType )
{
	KDBE_GET_PUNISHMENT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	DO_QUERY( L"exec dbo.mup_get_user_punishment", L"%d", % kPacket_ );

	while( m_kODBC.Fetch() )
	{
		KPunishmentInfo info;

		FETCH_DATA( info.m_iType
			>> info.m_iStartTime
			>> info.m_iDuration	);

		START_LOG( clog, L"ó�� ���" )
			<< BUILD_LOG( info.m_iType )
			<< BUILD_LOG( info.m_iStartTime )
			<< BUILD_LOG( info.m_iDuration )
			<< END_LOG;

		kPacket.m_vecPunishment.push_back( info );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_PUNISHMENT_ACK, kPacket );
}
#endif SERV_NEW_PUNISHMENT

//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
_IMPL_ON_FUNC( DBE_GET_ANTI_ADDICTION_INFO_REQ, UidType )
{
	KDBE_GET_ANTI_ADDICTION_INFO_ACK kPacket;
	kPacket.m_iPlayTime = 0;
	kPacket.m_iRestTime = 0;

	DO_QUERY( L"exec dbo.mup_get_anti_addiction_info", L"%d", % kPacket_ );

	if( m_kODBC.Fetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iPlayTime
			>> kPacket.m_iRestTime
			>> kPacket.m_wstrLastLogoutTime );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( clog, L"��ħ�� ����� ����" )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_ANTI_ADDICTION_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UPDATE_ANTI_ADDICTION_INFO )
{
	int iOK = NetError::ERR_ODBC_01;

	START_LOG( clog, L"��ħ�� ������ ������Ʈ" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< BUILD_LOG( kPacket_.m_iRestTime )
		<< BUILD_LOG( kPacket_.m_wstrLastLogoutTime )
		<< END_LOG;

	DO_QUERY( L"exec dbo.mup_update_anti_addiction_info", L"%d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iPlayTime
		% kPacket_.m_iRestTime
		% kPacket_.m_wstrLastLogoutTime );

	if( m_kODBC.Fetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		iOK = NetError::ERR_HACK_USER_01;

		START_LOG( cerr, L"��ħ�� ������ ������Ʈ ����.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iPlayTime )
			<< BUILD_LOG( kPacket_.m_iRestTime )
			<< BUILD_LOG( kPacket_.m_wstrLastLogoutTime )
			<< END_LOG;
	}

end_proc:
	return;
}

#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ )
{
	// ��Ŀ ���� ���� �б�
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> kPacket_.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ )
{
	// ��Ŀ ���� ���� �б�
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> kPacket_.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ )
{
	// ��Ŀ ���� ���� �б�
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> kPacket_.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ )
{
	KDBE_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
	kPacket.m_iOK	= NetError::ERR_ODBC_01;
	kPacket.m_kInfo	= kPacket_;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_MER", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteFilter
		% kPacket_.m_iCategory
		% kPacket_.m_byteGender
		% kPacket_.m_iBirth
		% kPacket_.m_wstrProfile
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteFilter
		% kPacket_.m_iCategory
		% kPacket_.m_byteGender
		% kPacket_.m_iBirth
		% kPacket_.m_wstrProfile
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrUpdated	// DB���� �־��� �ð��� ���� ��´�.
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-22	// �ڼ���
IMPL_ON_FUNC( DBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
	
	CTime			tCurrentTime				= CTime::GetCurrentTime();
	std::wstring	wstrBlockCheckUpdateDate	= tCurrentTime.Format( _T("%Y-%m-%d %H:%M:%S") );

	DO_QUERY( L"exec dbo.P_FieldBlock_CountCheck_INT_UPD", L"%d, %d, N\'%s\', N\'%s\'",
		% FIRST_SENDER_UID
		% kPacket_.m_byteBlockCheckCount
		% wstrBlockCheckUpdateDate
		% kPacket_.m_wstrBlockCheckResetDate
		);
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	return;
}

IMPL_ON_FUNC_NOPARAM( DBE_BLOCK_COUNT_CHECK_INFO_READ_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
	KDBE_BLOCK_COUNT_CHECK_INFO_READ_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_FieldBlock_CountCheck_SEL", L"%d", % FIRST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_FieldBlock_CountCheck_SEL", L"%d", % FIRST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> kPacket.m_byteBlockCheckCount
			>> kPacket.m_wstrBlockCheckResetDate
			);

		m_kODBC.EndFetch();
	}
	
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_BLOCK_COUNT_CHECK_INFO_READ_ACK, kPacket );
}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-02	// �ڼ���
IMPL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_INFO_REQ )
{
	KDBE_HACKING_USER_CHECK_COUNT_INFO_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_byteType		= kPacket_.m_byteType;
	kPacket.m_wstrReason	= kPacket_.m_wstrReason;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_SEL", L"%d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteType
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_SEL", L"%d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteType
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_byteBlockCheckCount
			>> kPacket.m_wstrLastResetDate
			);

		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_HACKING_USER_CHECK_COUNT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT )
{
	std::map<byte, KHackingUserCheckCountDB>::const_iterator it;
	for( it = kPacket_.m_mapHackingUserCheckCountDB.begin(); it != kPacket_.m_mapHackingUserCheckCountDB.end(); ++it )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_MER", L"%d, %d, %d, N\'%s\'",
			% kPacket_.m_iUserUID
			% it->first
			% it->second.m_byteBlockCheckCount
			% it->second.m_wstrLastResetDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_INT_UPD", L"%d, %d, %d, N\'%s\'",
			% kPacket_.m_iUserUID
			% it->first
			% it->second.m_byteBlockCheckCount
			% it->second.m_wstrLastResetDate
			);
#endif //SERV_ALL_RENEWAL_SP
	}

end_proc:
	return;
}

IMPL_ON_FUNC( DBE_ACCOUNT_BLOCK_NOT )
{
	// DB�� Account Block User ���!
	int iOK = NetError::ERR_ODBC_01;
	const byte	byteBlockLevel = ( kPacket_.m_wstrEndDate.empty() == true ) ? 1 : 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MAccountBlockList_MER", L"%d, %d, %d, N\'%s\', N\'%s\'",
		% kPacket_.m_iUserUID
		% static_cast<int>( kPacket_.m_byteType )
		% static_cast<int>( byteBlockLevel )
		% kPacket_.m_wstrReason
		% kPacket_.m_wstrEndDate
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MAccountBlockList_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\'",
		% kPacket_.m_iUserUID
		% static_cast<int>( kPacket_.m_byteType )
		% static_cast<int>( byteBlockLevel )
		% kPacket_.m_wstrReason
		% kPacket_.m_wstrEndDate
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Account Block User ��� ����.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_byteType )
			<< BUILD_LOG( kPacket_.m_wstrReason )
			<< BUILD_LOG( kPacket_.m_wstrEndDate )
			<< END_LOG;
	}
}
#endif // SERV_HACKING_USER_CHECK_COUNT