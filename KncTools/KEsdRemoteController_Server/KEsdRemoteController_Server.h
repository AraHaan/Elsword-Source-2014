// KEsdRemoteController_Server.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ

#include "KEsdRemoteController_ServerImp.h"


// CKEsdRemoteController_ServerApp:
// �� Ŭ������ ������ ���ؼ��� KEsdRemoteController_Server.cpp�� �����Ͻʽÿ�.
//

class CKEsdRemoteController_ServerApp : public CWinApp
{
public:
	CKEsdRemoteController_ServerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()

public:
	KEsdRemoteController_ServerImp m_kImp;

};

extern CKEsdRemoteController_ServerApp theApp;