#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( I_TO_INIT )
_ENUM( I_TO_CLOSE )
_ENUM( I_TO_WAIT )
//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
_ENUM( I_TO_TIME_COUNT )
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
_ENUM( I_TO_LOAD )
_ENUM( I_TO_PLAY )
_ENUM( I_TO_RESULT )
//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
_ENUM( I_TO_RETURN_TO_FIELD )
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 04. 15	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
_ENUM( I_TO_WAIT_FOR_DEFENCE )
//#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif