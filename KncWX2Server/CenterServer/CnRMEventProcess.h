#pragma once

#include "Performer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"


class KCnRMEventProcess : public KPerformer
{
	//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	DeclToStringW;
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}	
	DeclPfID;

public:
	KCnRMEventProcess();
	virtual ~KCnRMEventProcess();

	// override.	
	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );

	DECL_ON_FUNC( DBE_GET_TIME_DROP_INFO_ACK );
	DECL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_ACK );
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	DECL_ON_FUNC( DBE_ABUSER_LIST_ACK );
	DECL_ON_FUNC( DBE_EVENT_UPDATE_ACK );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	DECL_ON_FUNC( DBE_GET_DUNGEON_EVENT_INFO_ACK );
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_EVENT_DB_SCRIPT_ACK );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

protected:	
};