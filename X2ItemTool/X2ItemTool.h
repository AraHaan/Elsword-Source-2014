// X2ItemTool.h : X2ItemTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CX2ItemToolApp:
// �� Ŭ������ ������ ���ؼ��� X2ItemTool.cpp�� �����Ͻʽÿ�.
//

class CX2ItemToolApp : public CWinApp
{
public:
	CX2ItemToolApp();


// ������
public:
	virtual BOOL InitInstance();

// ����
	COleTemplateServer m_server;
		// ���� ����⿡ ���� ���� ��ü�Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CX2ItemToolApp theApp;
