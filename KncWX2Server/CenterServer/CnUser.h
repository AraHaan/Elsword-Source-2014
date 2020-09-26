#pragma once
#include "Actor.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "ActorManager.h"

#include <windows.h>    // ::GetTickCount()


SmartPointer(KCnUser);

class KCnUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KCnUser(void);
    virtual ~KCnUser(void);

    // override
    virtual void Tick();

protected:  // util function, Packet Handling
    bool AuthenticateUser( const wchar_t* szPassport, KNexonAccountInfo& kInfo );

    // packet send function
    template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    template < class T > void _SendToAccountDB( unsigned short usEventID_, UidType anTrace[], const T& data_ );
    template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToGSUser( UidType nToGSUID, UidType nToGSUSer, unsigned short usEventID, const T& data );
    void SendToAccountDB( unsigned short usEventID );
    void SendToGSUser( UidType nTo, unsigned short usEventID );	

	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	DECL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _DECL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	//}}
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
   _DECL_ON_FUNC( ECN_UPDATE_SERVER_INFO_ACK, KServerList );
    DECL_ON_FUNC_NOPARAM( EGS_GET_SERVER_SET_DATA_REQ );
   _DECL_ON_FUNC( ECN_UPDATE_SERVER_INFO_REQ, KServerInfo );	

	DECL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ );
	DECL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ );
	DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

    bool RoutePacket( const KEvent* pkEvent );   // CnUser, GSUser�� ���� ���������� ����� �ƴ�.
    //                          ^ KEvent�� �������� �ʾƾ� �Ѵ�. (SmartPtr�δ� �Ұ���)

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED ���·� ����������, DB data update�� ��û�Ѵ� 

	//{{ 2010. 04. 16  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
private:
	bool IsInitServerGroup()			{ return ( m_iCnServerGroupID != -1  &&  m_iGsServerGroupID != -1 ); }
	virtual bool IsSameServerGroup()	{ return ( m_iCnServerGroupID == m_iGsServerGroupID ); }
private:
	int			m_iCnServerGroupID;
	int			m_iGsServerGroupID;
#endif SERV_INTEGRATION
	//}}
};

template < typename T >
bool KCnUser::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
{
    return KSession::SendPacket( PI_CN_PROXY, GetUID(), NULL, usEventID, data, bLogging, bCompress );
}

template < class T >
void KCnUser::_SendToAccountDB( unsigned short usEventID_, UidType anTrace_[], const T& data_ )
{
    UidType anTrace[2] = { -1, -1 };
    if( anTrace_ != NULL )  // �ܺο��� �Ѿ�� ���� �ִٸ� �����Ѵ�.
    {
        ::memcpy( anTrace, anTrace_, sizeof( anTrace ) );
    }

    if( anTrace[0] == -1 )
        anTrace[0] = GetUID();
    else
        anTrace[1] = GetUID();

    KncSend( GetPfID(), GetUID(), PI_ACCOUNT_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KCnUser::SendToAccountDB( unsigned short usEventID, const T& data )
{
    _SendToAccountDB( usEventID, NULL, data );
}

template < class T >
void KCnUser::SendToLogDB( unsigned short usEventID_, const T& data_ )
{
    UidType anTrace[2] = { -1, -1 };

    if( anTrace[0] == -1 )
        anTrace[0] = GetUID();
    else
        anTrace[1] = GetUID();

    KncSend( GetPfID(), GetUID(), PI_LOG_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KCnUser::SendToGSUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KEvent kEvent;
    kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
    LIF( kEvent.m_kDestPerformer.AddUID( nTo ) );
    KSession::SendPacket( kEvent );
}
//�ٸ� ���Ӽ����� �ڷḦ ������ ����
template < class T >
void KCnUser::SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"Ÿ ���Ӽ����� ��Ŷ ������ ����.? �ֿ�" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToGSUser )
			<< BUILD_LOG( usEventID )
			<< END_LOG;
	}
}




