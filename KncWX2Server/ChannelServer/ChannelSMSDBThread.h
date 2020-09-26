#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


/** 
@class : KChannelSMSDBThread
@brief : SMS���ڸ� �����ϱ� ���� �����ϴ� SMS DB�� ���� ��û�� ����ϴ� ThreadŬ����
@date : 2012/09/28
*/
class KChannelSMSDBThread : public KSubThread
{
	DeclareDBThread( KChannelSMSDBThread );

public:
	KChannelSMSDBThread();
	virtual ~KChannelSMSDBThread(void);

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:

	// event handler
	DECL_ON_FUNC( DBE_SEND_PHONE_MSG_NOT );

};
