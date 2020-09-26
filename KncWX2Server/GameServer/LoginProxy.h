#pragma once
#include "ActorProxy.h"
#include <windows.h>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

SmartPointer( KLoginProxy );

class KLoginProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KLoginProxy(void);
    virtual ~KLoginProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );

    // derived from KSession
    virtual void OnDestroy();

protected:

	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
   _DECL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
    DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

    bool RoutePacket( const KEventPtr& spEvent );
	void ProcbyNotExistUser( const KEventPtr& spEvent, std::vector<UidType>& vecUID );

protected:
};