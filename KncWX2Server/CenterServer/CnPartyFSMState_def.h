#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )             	// �ʱ� ���´� ������ 1�̾�� �Ѵ�. 0�� ������ ������ ��� ���. �ʱ⿡ 1�� �ʱ�ȭ��.
_ENUM( S_CLOSE )
_ENUM( S_MAKING_PARTY )			// ��Ƽ�� ���� ���� ����
_ENUM( S_WAIT )					// ��Ƽ�� ���� �Ϸ�ǰ� �� default����
_ENUM( S_CHECK_PLAY_CONDITION ) // ���� �������� ���� üũ
_ENUM( S_CHECK_AUTO_PARTY )		// �ڵ� ��Ƽ ��û�� ���� üũ ���� ����
_ENUM( S_ACCEPT_FOR_PLAY )		// ��Ƽ ������ �ϱ� ���� accept�ǰ��� ���� ����
_ENUM( S_PREPARE_FOR_PLAY )		// ��� ��Ƽ������ accept�ǰ��� Ȯ���߰� ���� ������ �غ��ϴ� ����

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif