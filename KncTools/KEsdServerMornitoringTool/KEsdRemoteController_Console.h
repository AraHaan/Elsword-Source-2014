// KEsdRemoteController_Console.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ	

#include "CommonDefine.h"
#include "KEsdRemoteController_ConsoleImp.h"

// CKEsdRemoteController_ConsoleApp:
// �� Ŭ������ ������ ���ؼ��� KEsdRemoteController_Console.cpp�� �����Ͻʽÿ�.
//

class CKEsdRemoteController_ConsoleApp : public CWinApp
{
public:
	CKEsdRemoteController_ConsoleApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()

public:
	KEsdRemoteController_ConsoleImp m_kImp;

};

extern CKEsdRemoteController_ConsoleApp theApp;
