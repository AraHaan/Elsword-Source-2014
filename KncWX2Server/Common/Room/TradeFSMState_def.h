#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( STATE_ZERO_NO_USE )

	_ENUM( S_INIT )             // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
	_ENUM( S_CLOSE )
	_ENUM( S_WAIT )
	_ENUM( S_CONFIRM )
	//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
//#ifdef SERV_TRADE_DDOS_DEFENCE
	_ENUM( S_DB_UPDATE )
//#endif SERV_TRADE_DDOS_DEFENCE
	//}}

	_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif