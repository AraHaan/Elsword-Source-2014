//////////////////////////////////////////////////////////////////////////
// �κ� ���� FSM class

// 2008.12.06   ������   GSFSM���� �ű��� ������ �κ����� �����ϱ⿡ �Ѱ谡 �־� RBFSM�� ���� ����



#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( STATE_ZERO_NO_USE )

	_ENUM( S_INIT )     // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
	_ENUM( S_EXIT )

	_ENUM( S_CHANNEL_SELECT )

	_ENUM( S_UID_UPDATED )
	_ENUM( S_LOGINED )

	_ENUM( S_SERVER_SELECT )
	_ENUM( S_UNIT_SELECT )

	_ENUM( S_FIELD_MAP )
	
	_ENUM( S_PVP_LOBBY )
	_ENUM( S_PARTY )
	_ENUM( S_TRAINING_CENTER )

	_ENUM( S_ROOM )

	_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

// S_LOGINED ���� STATE_SENTINEL ������ ������ �����̴�.