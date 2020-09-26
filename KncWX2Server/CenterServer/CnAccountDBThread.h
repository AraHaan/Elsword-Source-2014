#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KCnAccountDBThread : public KSubThread
{
    DeclareDBThread( KCnAccountDBThread );
    DeclPfID;
public:

    KCnAccountDBThread();   // �����δ� ����. ������� �ʴ´�. (���� linking error ����)
    virtual ~KCnAccountDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
};