#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( STATE_ZERO_NO_USE )

	_ENUM( S_INIT )             // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
	_ENUM( S_CLOSE )
	_ENUM( S_WAIT )
	_ENUM( S_SHOP )
	//{{ 2011. 04. 12	������	�븮 ����
//#ifdef SERV_PSHOP_AGENCY	
	_ENUM( S_STOP_SALE )
//#endif SERV_PSHOP_AGENCY
	//}}

	_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif