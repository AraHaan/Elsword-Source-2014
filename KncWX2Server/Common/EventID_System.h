#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( E_HEART_BEAT = 0 )
	_ENUM( E_ACCEPT_CONNECTION_NOT )    // server -> client connect ��û�� ���� ���� ���ο� ����Ű�� ����.
	_ENUM( E_CONNECTION_LOST_NOT )
	_ENUM( E_UDP_PORT_NOT )             // server -> client UDP ��Ŷ�� ���� ��Ʈ��ȣ�� TCP�� �˷���.
	_ENUM( E_DISABLE_HB_CHECK_REQ )
	_ENUM( E_LOG_NOT )
	_ENUM( E_RESERVE_DESTROY )

	//{{ 2009. 12. 16  ������	������
	_ENUM( E_TOOL_GET_CCU_INFO_REQ )
	_ENUM( E_TOOL_GET_CCU_INFO_ACK )
	//}}

	//{{ 2011. 01. 13 ��μ�	���� ����͸� ��	
	_ENUM( E_TOOL_CHECK_LOGIN_REQ )
	_ENUM( E_TOOL_CHECK_LOGIN_ACK )

	_ENUM( E_TOOL_SERVER_LIST_REQ )
	_ENUM( E_TOOL_SERVER_LIST_ACK )
	//}}

	_ENUM( E_CHECK_SEQUENCE_COUNT_NOT )
	//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
//#ifdef SERV_UDP_RELAY_CHECKER
	_ENUM( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT )
	_ENUM( E_CONNECT_RELAY_ACK )
//#endif SERV_UDP_RELAY_CHECKER
	//}}
	//{{ 2012. 07. 18	������		������ ����
//#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	_ENUM( E_CHECK_DDOS_GUARD_REQ )
	_ENUM( E_CHECK_DDOS_GUARD_ACK )
	_ENUM( E_CH_CONNECTION_LOST_FOR_DDOS_GUARD_NOT )
	_ENUM( E_GS_CONNECTION_LOST_FOR_DDOS_GUARD_NOT )
//#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}

	//=============================================================//
	//{{ 2013.02.27 ��ȿ��	�ؿ� �⺻ ���� �۾� (SERV_GLOBAL_BASE)
#   include "OnlyGlobal/EventID_System_global.h"
	//}}
	//=============================================================//

	_ENUM( E_SYSTEM_EVENT_ID_END )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
