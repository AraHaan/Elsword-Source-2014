#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif
	// ��� ���
	// Collect Server <-> Client
	_ENUM( ECL_HEART_BEAT = 0 )
	_ENUM( ECL_ACCEPT_CONNECTION_NOT )    // server -> client connect ��û�� ���� ���� ���ο� ����Ű�� ����.
	_ENUM( ECL_CONNECTION_LOST_NOT )
	_ENUM( ECL_UDP_PORT_NOT )             // server -> client UDP ��Ŷ�� ���� ��Ʈ��ȣ�� TCP�� �˷���.
	_ENUM( ECL_DISABLE_HB_CHECK_REQ )
	_ENUM( ECL_LOG_NOT )
	_ENUM( ECL_RESERVE_DESTROY )

	//{{ 2009. 12. 16  ������	������
	_ENUM( ECL_TOOL_GET_CCU_INFO_REQ )
	_ENUM( ECL_TOOL_GET_CCU_INFO_ACK )
	//}}

	//{{ 2011. 01. 13 ��μ�	���� ����͸� ��	
	_ENUM( ECL_TOOL_CHECK_LOGIN_REQ )
	_ENUM( ECL_TOOL_CHECK_LOGIN_ACK )

	_ENUM( ECL_TOOL_SERVER_LIST_REQ )
	_ENUM( ECL_TOOL_SERVER_LIST_ACK )
	//}}

	_ENUM( ECL_CHECK_SEQUENCE_COUNT_NOT )
	
	_ENUM( ECL_SYSTEM_EVENT_ID_END )
	//////////////////////////////////////////////////////////////////////////
	
	// ��Ʈ�� ���� ���ۿ�
	_ENUM( ECL_CRASH_INFO_REQ )		// Cilent -> Server
	_ENUM( ECL_CRASH_INFO_ACK )		// Server -> Cilent

	// ���� ���ۿ�
	_ENUM( ECL_CRASH_IMAGE_INFO_REQ )		// Cilent -> Server
	_ENUM( ECL_CRASH_IMAGE_INFO_ACK )		// Server -> Cilent

	_ENUM( ECL_DISCONNECT_REQ )		// Cilent -> Server
	_ENUM( ECL_DISCONNECT_ACK )		// Server -> Cilent





	//��Ŷ�� ��
	_ENUM( COLLECT_EVENTID_SENTINEL ) //��Ŷ�� ��


#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
