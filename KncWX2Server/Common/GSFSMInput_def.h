#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( I_TO_INIT )     // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
_ENUM( I_TO_EXIT )

_ENUM( I_TO_UID_UPDATED )
_ENUM( I_TO_LOGINED )

_ENUM( I_TO_SERVER_SELECT )
//{{ 2011. 06. 23	������	2�� ��� ����ó��
//#ifdef SERV_CHECK_SECOND_PW_STATE
_ENUM( I_TO_CHECK_SECOND_PW )
//#endif SERV_CHECK_SECOND_PW_STATE
//}}

_ENUM( I_TO_FIELD_MAP )
_ENUM( I_TO_ROOM )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
