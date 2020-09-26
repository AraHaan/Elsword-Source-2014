// KEsdRemoteController_Server.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Server.h"
#include "KEsdRemoteController_ServerDlg.h"
#include <vld.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKEsdRemoteController_ServerApp

BEGIN_MESSAGE_MAP(CKEsdRemoteController_ServerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKEsdRemoteController_ServerApp ����

CKEsdRemoteController_ServerApp::CKEsdRemoteController_ServerApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CKEsdRemoteController_ServerApp ��ü�Դϴ�.

CKEsdRemoteController_ServerApp theApp;


// CKEsdRemoteController_ServerApp �ʱ�ȭ

BOOL CKEsdRemoteController_ServerApp::InitInstance()
{
	CWinApp::InitInstance();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	HWND hFindWindow = FindWindow( NULL, L"ESServerRemoteControl" );
	if( hFindWindow != NULL )
	{
		::MessageBox( NULL, L"�̹� ������������ �����ֽ��ϴ�! �� �ӽſ� �� ���� ���� �Ӽ� �ֽ��ϴ�. �ٸ� ���ǿ� ������������ �����ִ��� Ȯ�����ּ���!", L"����!", 0 );
		return FALSE;
	}

	CKEsdRemoteController_ServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� Ȯ���� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� ��Ҹ� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�
	// ���� ���α׷��� ���� �� �ֵ��� FALSE�� ��ȯ�մϴ�.
	return FALSE;
}



