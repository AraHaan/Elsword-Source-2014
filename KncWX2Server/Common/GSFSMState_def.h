#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )     // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
_ENUM( S_EXIT )

_ENUM( S_UID_UPDATED )
_ENUM( S_LOGINED )

//{{ 2011. 06. 23	������	2�� ��� ����ó��
//#ifdef SERV_CHECK_SECOND_PW_STATE
_ENUM( S_CHECK_SECOND_PW )
//#endif SERV_CHECK_SECOND_PW_STATE
//}}
_ENUM( S_SERVER_SELECT )	// ��������� ���� �ٲ��� ���ƾ� �Ѵ�. ���� ������ ������� ���������� �귯�´�.

_ENUM( S_FIELD_MAP )
_ENUM( S_ROOM )

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

// S_LOGINED ���� STATE_SENTINEL ������ ������ �����̴�.