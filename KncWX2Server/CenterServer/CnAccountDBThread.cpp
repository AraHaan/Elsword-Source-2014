#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "CnAccountDBThread.h"
#include "CnServer.h"
#include "ActorManager.h"
#include "NetError.h"

#define CLASS_TYPE KCnAccountDBThread
ImplPfID( KCnAccountDBThread, PI_CN_ACCOUNT_DB );

KCnAccountDBThread::~KCnAccountDBThread(void)
{
}

void KCnAccountDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    //switch( spEvent_->m_usEventID )
    //{
    //default:
    //    START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    //}
}
