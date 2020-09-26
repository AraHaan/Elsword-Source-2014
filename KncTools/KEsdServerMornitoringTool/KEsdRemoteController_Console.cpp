// KEsdRemoteController_Console.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KEsdRemoteController_ConsoleDlg.h"
#include "KEsdRemoteController_ConsoleLoginDlg.h"
#include <vld.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

lua_State* g_pLua	= NULL;


// CKEsdRemoteController_ConsoleApp

BEGIN_MESSAGE_MAP(CKEsdRemoteController_ConsoleApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKEsdRemoteController_ConsoleApp ����

CKEsdRemoteController_ConsoleApp::CKEsdRemoteController_ConsoleApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CKEsdRemoteController_ConsoleApp ��ü�Դϴ�.

CKEsdRemoteController_ConsoleApp theApp;


// CKEsdRemoteController_ConsoleApp �ʱ�ȭ

BOOL CKEsdRemoteController_ConsoleApp::InitInstance()
{
	CWinApp::InitInstance();

	//lua init
	{
		g_pLua = lua_open();

		if( g_pLua == NULL )
		{
			KDbgStm << "��� ���� ����.!";
			return FALSE;
		}

		//luaopen_base( g_pLua );  // ����Լ� tonumber�� ����ؾ� �Ѵ�.
		//luaopen_table( g_pLua );
		//luaopen_io( g_pLua );
		//luaopen_string( g_pLua );
		//luaopen_math( g_pLua );
		//lua_settop( g_pLua, 0 );

		luaL_openlibs( g_pLua );

		lua_settop( g_pLua, 0 );
	}

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	// config���� �ε�
	m_kImp.LoadConfig();
	
	CKEsdRemoteController_ConsoleLoginDlg loginDlg;

#ifdef COMPILE_NO_LOGIN
	INT_PTR nResponse = IDD_LOGIN;
#else
	INT_PTR nResponse = loginDlg.DoModal();
#endif

	if (nResponse == IDD_LOGIN)
	{
		CKEsdRemoteController_ConsoleDlg cDialog;
		m_pMainWnd = &cDialog;

		// AuthLevel�� ���� �������
#ifdef COMPILE_NO_LOGIN
		cDialog.SetAuthLevel( 1 );
#else
		cDialog.SetAuthLevel( m_kImp.GetAuthLevel() );
#endif

		INT_PTR nResponse2 = cDialog.DoModal();
		if (nResponse2 == IDOK)
		{
			// TODO: ���⿡ ��ȭ ���ڰ� Ȯ���� ���� �������� ��� ó����
			// �ڵ带 ��ġ�մϴ�.
		}
		else if (nResponse2 == IDCANCEL)
		{
			// TODO: ���⿡ ��ȭ ���ڰ� ��Ҹ� ���� �������� ��� ó����
			// �ڵ带 ��ġ�մϴ�.
		}
	}
	else if (nResponse == IDCANCEL)
	{
		lua_close( g_pLua );
		return FALSE;
	}	

	//lua shutdown
	{
		lua_close( g_pLua );
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�
	// ���� ���α׷��� ���� �� �ֵ��� FALSE�� ��ȯ�մϴ�.
	return FALSE;
}
