// X2KomFileViewer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ
#include "X2KomFileManager.h"


// CX2KomFileViewerApp:
// �� Ŭ������ ������ ���ؼ��� X2KomFileViewer.cpp�� �����Ͻʽÿ�.
//

class CX2KomFileViewerApp : public CWinApp
{
public:
	CX2KomFileViewerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()

private:
	bool CommandLineExecute();

public:
	CX2KomFileManager				m_komFileManager;	
};

extern CX2KomFileViewerApp theApp;
