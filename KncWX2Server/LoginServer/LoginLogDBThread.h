#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  ������	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KLoginLogDBThread : public KSubThread
{
    DeclareDBThread( KLoginLogDBThread );
	//{{ 2009. 8. 17  ������	sp profiler	
	DeclareSPProfiler;
	//}}
    DeclPfID;
public:

    KLoginLogDBThread();  // �����δ� ����. ������� �ʴ´�. (���� linking error ����)
    virtual ~KLoginLogDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );
    void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );
	
    //void WriteServerInfoToDB( IN KServerInfo kPacket_, OUT KServerList& kPacket );

    // event handler
   _DECL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress );
   //{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
   //#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING   
   //{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
   //}}
   //#endif SERV_SERVER_CONNECT_CODE_REFACTORING
   //}}
    //{{ 2008. 5. 27  ������  ü��ID ����
    DECL_ON_FUNC( DBE_UPDATE_SERVER_INFO_REQ );
    //}}   
	//{{ 2009. 7. 6  ������		��ŷ ����	
	DECL_ON_FUNC_NOPARAM( DBE_GET_HENIR_RANKING_INFO_REQ );
	DECL_ON_FUNC( DBE_DELETE_RANKING_INFO_NOT );
	DECL_ON_FUNC( DBE_BACKUP_RANKING_INFO_NOT );
	//}}
	//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
	DECL_ON_FUNC( DBE_UPDATE_CCU_INFO_NOT );
#endif SERV_CCU_NEW
	//}}
	//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	DECL_ON_FUNC_NOPARAM( DBE_GET_MODULE_INFO_LIST_REQ );
   _DECL_ON_FUNC( DBE_UPDATE_MODULE_INFO_LIST_REQ, KDBE_GET_MODULE_INFO_LIST_ACK );
#endif SERV_DLL_MANAGER
	//}}
	//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	DECL_ON_FUNC_NOPARAM( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_REQ );
	DECL_ON_FUNC( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}
	//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	DECL_ON_FUNC( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2013. 02. 13	������	Tick�����ս� DB�α� ���
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	DECL_ON_FUNC( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}
	//{{ 2013. 02. 21	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	DECL_ON_FUNC( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
	//}}

    DECL_ON_FUNC_NOPARAM( DBE_SERVER_OFF_NOT );
};

template < class T >
void KLoginLogDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginLogDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KLoginLogDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}
