#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )             // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
_ENUM( S_CLOSE )
_ENUM( S_WAIT )
//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
_ENUM( S_TIME_COUNT )
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
_ENUM( S_LOAD )
_ENUM( S_PLAY )
_ENUM( S_RESULT )
//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
_ENUM( S_RETURN_TO_FIELD )
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 04. 15	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
_ENUM( S_WAIT_FOR_DEFENCE )
//#endif SERV_NEW_DEFENCE_DUNGEON
//}}

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif