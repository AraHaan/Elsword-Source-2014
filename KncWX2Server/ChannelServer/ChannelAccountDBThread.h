#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

//{{ 2009. 8. 13  ������	sp profiler
#include "odbc/DBUtil.h"
//}}

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

class KChannelAccountDBThread : public KSubThread
{
    DeclareDBThread( KChannelAccountDBThread );
    DeclareSPProfiler;
    DeclPfID;
public:

    KChannelAccountDBThread();  // �����δ� ����. ������� �ʴ´�. (���� linking error ����)
    virtual ~KChannelAccountDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );    
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

    //void WriteServerInfoToDB( IN KServerInfo kPacket_, OUT KServerList& kPacket );

    // event handler
#ifdef SERV_KOG_OTP_VERIFY   
	DECL_ON_FUNC( DBE_CH_USER_GENERAL_LOGIN_REQ );
	DECL_ON_FUNC( DBE_CH_USER_KOGOTP_LOGIN_REQ );
#endif // SERV_KOG_OTP_VERIFY

	//DECL_ON_FUNC( DBE_GASH_USER_LOGIN_REQ );

	//{{ 2011. 01. 13 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL
	DECL_ON_FUNC( DBE_GET_CONCURRENT_USER_LOGIN_REQ );
#endif SERV_CCU_MONITORING_TOOL
	//}}	
	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	DECL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	DECL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ );
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	DECL_ON_FUNC( DBE_CHECK_SERVER_SN_REQ );
	DECL_ON_FUNC( DBE_CREATE_SERVER_SN_REQ );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}

	
};

template < class T >
void KChannelAccountDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CHANNEL_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KChannelAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CHANNEL_USER, nTo, NULL, usEventID, data );
}



