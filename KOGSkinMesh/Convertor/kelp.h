// kelp.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CkelpApp:
// �� Ŭ������ ������ ���ؼ��� kelp.cpp�� �����Ͻʽÿ�.
//

class CkelpApp : public CWinApp
{
public:
	CkelpApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CkelpApp theApp;
