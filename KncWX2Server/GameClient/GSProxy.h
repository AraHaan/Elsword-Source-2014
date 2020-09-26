#pragma once
#include "ActorProxy.h"

#include <windows.h>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KNCSingleton.h"
#include "Socket/NetCommon.h"
#include "ClientPacket.h"

#include <queue>
#include <boost/shared_ptr.hpp>

SmartPointer( KGSProxy );

class KGSProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KGSProxy(void);
    virtual ~KGSProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );

    template < typename T >
    bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false )
    {
        return KSession::SendPacket( PI_GS_USER, 0, NULL, usEventID, data, bLogging, bCompress );
    }

    bool SendID( unsigned short usEventID )
    {
        return KSession::SendID( PI_GS_USER, 0, NULL, usEventID );
    }
        
    //////////////////////////////////////////////////////////////////////////
    // Interfaces for Client Programmer.

    //virtual bool Init( bool bUseIOCP );   // �κ��� �����ϴ� ��� iocp�� ����ϵ��� ���� ����.
    //virtual bool Connect( const TCHAR* szIP, unsigned short usPort ); // ���� ���Ӹ� �Ѵ�.

    //int Connect( IN const KServerInfo& kServerInfo, IN const KEventPtr spEvent, IN DWORD dwTimeOut );

    // 060305. florist. ���� �̵��� cached data�� ������ ������, ��ũ����� �߰���.
    // 060412. florist. ���� �̵� ���� ��ȭ. Ȯ���� req/ack ó���� �����Ѵ�.
    //bool RequestServerMigration( IN const KServerInfo& kInfo, DWORD dwTimeOut = 3000 );

protected:  // Recv Msg Handler & Util Function
    virtual void OnSocketError();
   // DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK );
   // DECL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK );
   //_DECL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int );

	//DECL_ON_FUNC( EGS_CREATE_UNIT_ACK );
    DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );
};