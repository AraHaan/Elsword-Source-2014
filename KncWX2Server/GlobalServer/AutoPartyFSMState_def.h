#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )             // �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
_ENUM( S_CLOSE )
_ENUM( S_AUTO_PARTY_MAKING )
_ENUM( S_CHECK_ACCEPT )
_ENUM( S_REGROUP_PARTY )
_ENUM( S_PLAY )

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif