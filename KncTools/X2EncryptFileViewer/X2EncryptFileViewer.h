// X2EncryptFileViewer.h : X2EncryptFileViewer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ
#include <vector>


// CX2EncryptFileViewerApp:
// �� Ŭ������ ������ ���ؼ��� X2EncryptFileViewer.cpp�� �����Ͻʽÿ�.
//

class CX2EncryptFileViewerApp : public CWinApp
{
public:
	CX2EncryptFileViewerApp();
	std::vector< int >& GetEncryptionKeyList() { return m_vecEncryptionKey; }

// ������
public:
	virtual BOOL InitInstance();
	

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()



private:
	void LoadConfig();
	bool AddEncryptionKey_Lua();

	PCHAR* CommandLineToArgvA( PCHAR CmdLine, int* _argc );
	
	std::vector< int > m_vecEncryptionKey;
};

extern CX2EncryptFileViewerApp theApp;
