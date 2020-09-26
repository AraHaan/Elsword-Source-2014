#ifndef _ENUM
#define _ENUM(VAL) VAL,
#define ___ENUM___
enum
{
#endif

	_ENUM( STATE_ZERO_NO_USE )

	_ENUM( STATE_INIT = 1 )         // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
	_ENUM( STATE_CONNECTED = 2 )    // �������� ������ ���� �����Ѵ�.
	_ENUM( STATE_EXIT = 3 )
	_ENUM( STATE_REGISTERING )
	_ENUM( STATE_REGISTERED )

	_ENUM( STATE_SENTINEL )

#ifdef ___ENUM___
};
#undef ___ENUM___
#undef _ENUM
#endif