// BugTrapWinServer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CBugTrapWinServerApp:
// �� Ŭ������ ������ ���ؼ��� BugTrapWinServer.cpp�� �����Ͻʽÿ�.
//

class CBugTrapWinServerApp : public CWinApp
{
public:
	CBugTrapWinServerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CBugTrapWinServerApp theApp;
