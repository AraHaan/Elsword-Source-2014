
#include "ServerDefine.h"

//////////////////////////////////////////////////////////////////////////
// for visual assist

#ifndef EVENT_HANDLER
#define EVENT_HANDLER( eventid )
#endif

#ifndef EVENT_HANDLER_NOPARAM
#define EVENT_HANDLER_NOPARAM( eventid )
#endif

#ifndef _EVENT_HANDLER
#define _EVENT_HANDLER( eventid, structtype )
#endif


#ifdef SERV_COUNTRY_CN

//////////////////////////////////////////////////////////////////////////
// ���⼭ ���� �̺�Ʈ �ڵ鷯 ����!

// 2010. 07. 26.	�ڱ���		���ϼ� ����
EVENT_HANDLER( EGIANT_ROLEREG_CREATE_UNIT_ACK );
EVENT_HANDLER( EGIANT_ROLEREG_CREATE_GUILD_ACK );

//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
EVENT_HANDLER( DBE_GET_ANTI_ADDICTION_INFO_ACK );
EVENT_HANDLER( EGS_ANTI_ADDICTION_NOT );
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

#ifdef SERV_NEW_PUNISHMENT
EVENT_HANDLER( EGIANT_INFO_PUNISH_REQ );
EVENT_HANDLER( DBE_GET_PUNISHMENT_ACK );
#endif SERV_NEW_PUNISHMENT

#ifdef SERV_INFOSERVER_ADD_WORK
EVENT_HANDLER( EGIANT_INFO_REQUESTID_REQ );
#endif //SERV_INFOSERVER_ADD_WORK


#endif SERV_COUNTRY_CN