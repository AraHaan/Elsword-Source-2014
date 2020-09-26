#pragma once

#include <ToString.h>
#include <KNCSingleton.h>
#include <Windows.h>
#include "ClientPacket.h"

//##des :
//## 1 : ������ ���� ��������Ʈ�� ����
//## 2 : ���� ���Ӽ����� �ִ� ������������ ��Ŷ�� ����
//���� ����� �����ϱ� ���� Ŭ����..

class KTutorialManager
{
	DeclareSingleton( KTutorialManager );
	DeclToStringW;
	DeclDump;

public:
	enum
	{
		UNIT_LEVEL_STUDENT_MAX = 10,
		UNIT_LEVEL_TEACHER_MIN = 20,
		STUDENT_DISCONNECT_DAY = 7,
		STUDENT_PAGE_NUM = 5,
		STUDENT_NUM = 3,
		TEACHER_NUM = 1,
	};

	enum TUTORIAL_DEL_REASON
	{
		TDR_DISCONNECT_DAY = 0,
		TDR_LEVEL_UP_END,	//������ �Ѱ��.
		TDR_DELETE_UNIT,	//������ ������ ���
		TDR_REFUSAL_USER,	//������ ������ ��û�� ���

		TDR_TOTAL_NUM,
	};

	//{{ 2008. 9. 8  ������		���� ���� ����
	enum TUTOR_REWARD_ID
	{
        TRI_GRADUATION = 6,
	};
	//}}

public:
	~KTutorialManager(void);

	bool	IsExistbylist( UidType iUnitUID );
	
	bool	AddUnit( KTutorialUnitInfo& kInfo );	//���� 10������ ������ �����Ҷ� �߰�..
	bool	DeleteUnit( UidType iUnitUID );

	bool	GetTutorPageInfo( IN KEGS_TUTORIAL_STUDENT_LIST_REQ& kReq, OUT KEGS_TUTORIAL_STUDENT_LIST_ACK& kAck );
	void	SetLevelData( UidType iUnitUID, u_char ucLevel );

	//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	int GetDelReason( TUTORIAL_DEL_REASON eReason );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	std::wstring GetDelReason( TUTORIAL_DEL_REASON eReason );	
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}

private:
	KTutorialManager(void);

	//std::map<UidType,KTutorialUnitInfo>		m_mapUnitInfo;	//10���������� ���ָ���Ʈ.
	std::vector<KTutorialUnitInfo>			m_vecUnitInfo;	//��������.

	//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	int m_iDelReason[TDR_TOTAL_NUM];	
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	std::wstring	m_wstrDelReason[TDR_TOTAL_NUM];
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}
};

DefSingletonInline( KTutorialManager );
