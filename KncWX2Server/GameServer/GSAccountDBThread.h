#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  ������	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KGSAccountDBThread : public KSubThread
{
    DeclareDBThread( KGSAccountDBThread );
	//{{ 2009. 8. 17  ������	sp profiler
	DeclareSPProfiler;
	//}}
    DeclPfID;

public:
    KGSAccountDBThread();
    virtual ~KGSAccountDBThread(void);

protected:
    // derived from KSubThread
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

	int  GetMessengerSN( IN UidType iUnitUID, OUT u_int& uiKNMSerialNum ); //ret val : iOK

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	bool CheckIsComeBackUser( IN std::wstring& wstrLastDate, OUT int& iRewardStep, OUT std::wstring& wstrEndBuffDate, IN std::map< int, int >& mapComeBackRewardCondition );
	bool CheckComeBackFirst( IN std::wstring& wstrEndDate );
#endif SERV_COME_BACK_USER_REWARD
	//}} 

    // event handler
	//{{ 2009. 12. 22  ������	������Ȯ��
	DECL_ON_FUNC( ELOG_STAT_PLAY_TIME );
	//}}

    DECL_ON_FUNC_NOPARAM( DBE_UPDATE_LOGIN_TIME_NOT );
    DECL_ON_FUNC( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ );

   _DECL_ON_FUNC( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, KEGS_ADMIN_CHANGE_AUTH_LEVEL_REQ );
   _DECL_ON_FUNC( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ );

   _DECL_ON_FUNC( DBE_ACCOUNT_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_ACK );
   //{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
   _DECL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_REQ, KDBE_SELECT_UNIT_ACK );
#else
   _DECL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_ACK );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
   //}}
	//{{ 2009. 6. 4  ������	ĳ���� ����
   _DECL_ON_FUNC( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_REQ, KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK );
	//}}

#ifdef SERV_UNIT_WAIT_DELETE
   _DECL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_ACK );
#else //SERV_UNIT_WAIT_DELETE
   _DECL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_ACK );
#endif //SERV_UNIT_WAIT_DELETE

	//{{ 2008. 3. 28  ������  ��õ��
	DECL_ON_FUNC_NOPARAM( DBE_RECOMMEND_USER_NOT );
	//}}

   _DECL_ON_FUNC( DBE_AGREE_HACK_USER_REQ, UidType );
   _DECL_ON_FUNC( DBE_REPORT_HACK_USER_NOT, UidType );

   _DECL_ON_FUNC( DBE_GET_MY_MESSENGER_SN_REQ, KEGS_GET_MY_MESSENGER_SN_REQ );	

   //{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
	DECL_ON_FUNC( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT );
#endif SERV_IP_ACCOUNT_LOG
   //}}

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	DECL_ON_FUNC( DBE_GET_SECOND_SECURITY_INFO_REQ );
	DECL_ON_FUNC( DBE_SEUCCESS_SECOND_SECURITY_REQ );
	_DECL_ON_FUNC( DBE_FAILED_SECOND_SECURITY_REQ, UidType );
	DECL_ON_FUNC( DBE_CREATE_SECOND_SECURITY_REQ );
	DECL_ON_FUNC( DBE_DELETE_SECOND_SECURITY_REQ );
	DECL_ON_FUNC( DBE_CHANGE_SECOND_SECURITY_PW_REQ );
	DECL_ON_FUNC( DBE_UPDATE_SECURITY_TYPE_NOT );
#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	DECL_ON_FUNC( DBE_WRITE_COME_BACK_REWARD_NOT );
	DECL_ON_FUNC( DBE_WRITE_COME_BACK_END_NOT );
	DECL_ON_FUNC( DBE_WRITE_COME_BACK_LOGOUT_NOT );	
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	DECL_ON_FUNC( DBE_CHECK_RETAINING_USER_REQ );
	DECL_ON_FUNC( DBE_INSERT_RETAINING_USER_REQ );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	_DECL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_NOT, KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	DECL_ON_FUNC( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}

	//{{ 2012. 05. 17	�ڼ���	Ű���� ���� ������ DB���� �а� ����
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
	DECL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ );
	_DECL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_READ_REQ, UidType );
#endif SERV_KEYBOARD_MAPPING_INFO_RW
	//}}

	//{{ 2012. 05. 29	�ڼ���	ä�� �ɼ� ������ DB���� �а� ����
#ifdef SERV_CHAT_OPTION_INFO_RW
	DECL_ON_FUNC( DBE_CHAT_OPTION_INFO_WRITE_REQ );
	_DECL_ON_FUNC( DBE_CHAT_OPTION_INFO_READ_REQ, UidType );
#endif SERV_CHAT_OPTION_INFO_RW
	//}}
	
	//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	DECL_ON_FUNC( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	DECL_ON_FUNC( DBE_UPDATE_EVENT_MONEY_REQ );
#endif // SERV_EVENT_MONEY

	//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	_DECL_ON_FUNC( DBE_GET_ANTI_ADDICTION_INFO_REQ, UidType );
	DECL_ON_FUNC( DBE_UPDATE_ANTI_ADDICTION_INFO );
#endif SERV_ANTI_ADDICTION_SYSTEM
	//}}

	//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	void ReadBingoEvent( IN const UidType anTrace_[], OUT KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket );
	//DECL_ON_FUNC_NOPARAM( DBE_BINGO_EVENT_INFO_READ_REQ );
	DECL_ON_FUNC( DBE_BINGO_EVENT_INFO_WRITE_REQ );
#endif SERV_EVENT_BINGO
	//}}

	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	DECL_ON_FUNC_NOPARAM( DBE_2012_WINTER_VACATION_EVENT_NOT );
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	_DECL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_REQ, KDBE_NEW_QUEST_REQ );
	_DECL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_REQ, KDBE_QUEST_COMPLETE_REQ );
	DECL_ON_FUNC( DBE_ACCOUNT_QUEST_UPDATE_NOT );
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	//{{ 2012. 12. 19  ��ȯ(���� ����) - ��μ�
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
	DECL_ON_FUNC( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ );
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
	//}}
	
	//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	DECL_ON_FUNC( DBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT );
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

#ifdef SERV_NEW_PUNISHMENT
	_DECL_ON_FUNC( DBE_GET_PUNISHMENT_REQ, UidType );
#endif SERV_NEW_PUNISHMENT

	//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ );
	DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ );
	DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ );
	DECL_ON_FUNC( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}
	
#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-22	// �ڼ���
	DECL_ON_FUNC( DBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT );
	DECL_ON_FUNC_NOPARAM( DBE_BLOCK_COUNT_CHECK_INFO_READ_REQ );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-02	// �ڼ���
	DECL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_INFO_REQ );
	DECL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT );
	DECL_ON_FUNC( DBE_ACCOUNT_BLOCK_NOT );
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_ENTRY_POINT
    DECL_ON_FUNC( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ );
#endif SERV_ENTRY_POINT
};

template < class T >
void KGSAccountDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_ACCOUNT_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_ACCOUNT_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}
