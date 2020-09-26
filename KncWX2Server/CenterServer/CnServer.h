#pragma once
#include "BaseServer.h"
//#include "KncSend.h"

class KCnServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;
    DeclPfID;

public:
    KCnServer(void);
    virtual ~KCnServer(void);

	int  GetProxyID()								{ return m_iProxyID; }
	void SetProxyID( int iProxyID )					{ m_iProxyID = iProxyID; }

    // derived from KBaseServer
    virtual bool Init();
    virtual void RegToLua();
    virtual void ShutDown();
    virtual bool DestroyAllSession();         // �������� ������ ����.

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void Tick();

    //////////////////////////////////////////////////////////////////////////

protected:
	virtual void OnServerReadyComplete();

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	void SetActiveLagCheck_LUA( bool bActiveLagCheck );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

	template < class T > void BroadCastAllGS( unsigned short usEventID, const T& data );
	//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	template < class T > void BroadCastSameGroupGS( unsigned short usEventID, const T& data );
#endif SERV_INTEGRATION
	//}}
	//{{ 2009. 6. 1  ������		ä���̵�
	template < class T > void SendToGSServer( UidType nGS, unsigned short usEventID, const T& data );
	//}}
	//{{ 2009. 5. 23  ������	�ð����
	template < class T > void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	//}}

   _DECL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_CENTER_SERVER_ON_ACK );
   _DECL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
   _DECL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT );    
	//{{ 2009. 4. 22  ������	������������Ʈ
	DECL_ON_FUNC( ECN_NEW_PVP_USER_INFO_NOT );
	DECL_ON_FUNC( ECN_UPDATE_PVP_USER_INFO_NOT );
   _DECL_ON_FUNC( ECN_CHECK_INVALID_PVP_USER_NOT, UidType );
	//}}
	//{{ 2009. 5. 23  ������	�ð����	
	DECL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_ACK );
	//}}
	//{{ 2009. 6. 1  ������		ä���̵�
	DECL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_REQ );
	DECL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_ACK );
	//}}
	//{{ 2009. 6. 3  ������		Ȯ����
   _DECL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT );
	//}}

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
   _DECL_ON_FUNC( ESR_ORDER_TO_CENTER_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );   
#endif SERV_REALTIME_SCRIPT
	//}}
   //{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_LOG_SYSTEM_NEW
   //}}

   //{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	DECL_ON_FUNC( DBE_RELEASE_TICK_UPDATE_ACK );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	_DECL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	_DECL_ON_FUNC( ECN_ADD_COMMON_FLAG_NOT, KECN_ADD_COMMON_FLAG_NOT );   
	_DECL_ON_FUNC( ECN_DEL_COMMON_FLAG_NOT, KECN_DEL_COMMON_FLAG_NOT );   
#endif SERV_MODFIY_FLAG_REALTIME_PATCH

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
protected:
	int								m_iProxyID;	

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	boost::timer					m_tTimeProcessCommunicationONOFF;
	bool							m_bServerRunningProcessCommunicationOnOff;
#endif //SERV_PROCESS_COMMUNICATION_KSMS
};

DefKObjectInline( KCnServer, KBaseServer );


template < class T >
void KCnServer::BroadCastAllGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}

//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
template < class T >
void KCnServer::BroadCastSameGroupGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0, anTrace, usEventID, data );
}
#endif SERV_INTEGRATION
//}}

//{{ 2009. 6. 1  ������		ä���̵�
template < class T >
void KCnServer::SendToGSServer( UidType nGS, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, nGS, anTrace, usEventID, data );
}
//}}

//{{ 2009. 5. 23  ������	�ð����
template < class T >
void KCnServer::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_CN_SERVER, GetUID(), PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}
//}}


