#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( I_TO_INIT )     // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
	_ENUM( I_TO_EXIT )

	_ENUM( I_TO_CHANNEL_SELECT )

	_ENUM( I_TO_UID_UPDATED )
	_ENUM( I_TO_LOGINED )

	_ENUM( I_TO_SERVER_SELECT )
	_ENUM( I_TO_UNIT_SELECT )

	_ENUM( I_TO_FIELD_MAP )

	_ENUM( I_TO_PVP_LOBBY )
	_ENUM( I_TO_PARTY )
	_ENUM( I_TO_TRAINING_CENTER )

	_ENUM( I_TO_ROOM )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
