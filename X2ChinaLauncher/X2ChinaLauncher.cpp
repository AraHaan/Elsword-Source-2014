// X2ChinaLauncher.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "X2ChinaLauncherDlg.h"
#include "tinyxml/tinyxml.h"
#include <wininet.h>
#include "Config.h"
#include "define.h"
//#include "adler32.h"
#include "patchthread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#define MULTI_CLIENT_OFF

// CX2ChinaLauncherApp

BEGIN_MESSAGE_MAP(CX2ChinaLauncherApp, CWinApp)
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// ������ġ ���� ����� ������ �Ǻ�
bool g_bPatcherPatched = false;


// CX2ChinaLauncherApp ����

CX2ChinaLauncherApp::CX2ChinaLauncherApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CX2ChinaLauncherApp ��ü�Դϴ�.

CX2ChinaLauncherApp theApp;


// CX2ChinaLauncherApp �ʱ�ȭ

BOOL CX2ChinaLauncherApp::InitInstance()
{
#if 0
#include <crtdbg.h>
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	_CrtDumpMemoryLeaks();
	_CrtSetBreakAlloc(369);
#endif

	//��ó �ʱ�ȭ
	InitLauncher();

	// ���� ��ġ �� ����� �� �� ǥ��
	if( *m_lpCmdLine == '1' )
	{
		g_bPatcherPatched = true;
	}

	//�̹� ��ó�� ���������� Ȯ���Ѵ�.
	{
		DWORD dwStart = GetTickCount();
		while( true )
		{
			// ���� ���μ����� �������� Ȯ��
			g_hMutex = ::CreateMutex( NULL, TRUE, _T("ChinaLauncher") );
			if( GetLastError() != ERROR_ALREADY_EXISTS )
				break;

			::ReleaseMutex( g_hMutex );
			::CloseHandle( g_hMutex );

			// 1�� �������� Ȯ��
			DWORD dwNow = GetTickCount();
			if( dwNow - dwStart > 1000 )
			{
				//"�̹� ��ó�� �������Դϴ�." ���� ���
				CString strMessage;
				strMessage.LoadString(IDS_ALREADYRUNNINGLAUNCHER_MESSAGE);

				AfxMessageBox(strMessage, MB_OK);

				return FALSE; 
			}
		}
	}

#ifdef MULTI_CLIENT_OFF
#ifndef _IN_HOUSE_
	HANDLE hClientMutex = ::CreateMutex( NULL, TRUE, L"ElswordX2" );

	if( GetLastError() == ERROR_ALREADY_EXISTS ) 
	{ 
		//"Ŭ���̾�Ʈ�� �����Ͻð� ��ó�� ������ֽʽÿ�." ���� ���
		CString strMessage;
		strMessage.LoadString(IDS_ALREADYRUNNINGCLIENT_MESSAGE);

		AfxMessageBox(strMessage, MB_OK);

		//::ReleaseMutex( g_hLauncherMutex );
		//::CloseHandle( g_hLauncherMutex );
		//g_hLauncherMutex = NULL;

		return FALSE; 
	} 
	else
	{
		::ReleaseMutex( hClientMutex );
	}
#endif _IN_HOUSE_
#endif //MULTI_CLIENT_OFF

#if 0
	if(!LoadLauncherPatchServer())
	{
		//"������ ������ �� �����ϴ�" ��� ���� ���
		CString strMessage;
		strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

		AfxMessageBox(strMessage, MB_OK);

		//::ReleaseMutex( g_hLauncherMutex );
		//::CloseHandle( g_hLauncherMutex );
		//g_hLauncherMutex = NULL;

		return FALSE;
	}

	unsigned int uiChecksum;
	int iSelect = rand()%m_vecAddr.size();
	std::string strAddr = m_vecAddr[iSelect].strValue;
	std::string strPath = m_vecAddr[iSelect].strPath;

	{
		int retry = 0;

		//��ó ������ ���캻��.
		while(!GetVersionInServer(strAddr, strPath+"/patchinfo.xml", uiChecksum))
		{
			iSelect = rand()%m_vecAddr.size();
			strAddr = m_vecAddr[iSelect].strValue;
			strPath = m_vecAddr[iSelect].strPath;

			if(MAX_RETRY == ++retry)
			{
				//"������ ������ �� �����ϴ�" ��� ���� ���
				CString strMessage;
				strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

				AfxMessageBox(strMessage, MB_OK);

				//::ReleaseMutex( g_hLauncherMutex );
				//::CloseHandle( g_hLauncherMutex );
				//g_hLauncherMutex = NULL;

				return FALSE;
			}
		}
	}

	if(IsNewVersion(uiChecksum))
	{
		int retry = 0;

		while(!PatchLauncher(strAddr, strPath+"/"+"ElswordLauncher.exe", "ElswordLauncher.exe"))
		{
			iSelect = rand()%m_vecAddr.size();
			strAddr = m_vecAddr[iSelect].strValue;
			strPath = m_vecAddr[iSelect].strPath;

			if(MAX_RETRY == ++retry)
			{
				//"������ ������ �� �����ϴ�" ��� ���� ���
				CString strMessage;
				strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

				AfxMessageBox(strMessage, MB_OK);

				//::ReleaseMutex( g_hLauncherMutex );
				//::CloseHandle( g_hLauncherMutex );
				//g_hLauncherMutex = NULL;

				return FALSE;
			}
			::Sleep( MAX_DELAY );
		}

		//::ReleaseMutex( g_hLauncherMutex );
		//::CloseHandle( g_hLauncherMutex );
		//g_hLauncherMutex = NULL;

		ReRunLauncher();
		return FALSE;
	}
#endif

	// ���ؽ��� ���� Ŭ���̾�Ʈ�� �̹� ���� ������ Ȯ���Ѵ�.
	//g_hMutex = ::CreateMutex( NULL, TRUE, _T("ElswordX2") );

	//if( GetLastError() == ERROR_ALREADY_EXISTS ) 
	//{ 
	//	AfxMessageBox( _T("������ �̹� �������Դϴ�.")); 
	//	DestroyWindow();
	//	return FALSE; 
	//}

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
	InitCommonControls();

	CWinApp::InitInstance();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CX2ChinaLauncherDlg dlg;
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

int CX2ChinaLauncherApp::ExitInstance()
{
	if(g_hMutex)
	{
		::ReleaseMutex( g_hMutex );
		::CloseHandle( g_hMutex );
		g_hMutex = NULL;
	}

	return CWinApp::ExitInstance(); 
}

void CX2ChinaLauncherApp::InitLauncher()
{
	char path[2048];
	GetModuleFileNameA(NULL,path,2048);
	std::string pathstr = path;
	std::transform(pathstr.begin(),pathstr.end(),pathstr.begin(),tolower);

	int last = pathstr.find_last_of('\\');
	m_strLauncherName = pathstr.substr(last+1);
	
	LPSTR fileptr;	
	GetFullPathNameA(".",2048,path,&fileptr);
	m_strLocalPath = path;
	std::transform(m_strLocalPath.begin(), m_strLocalPath.end(), m_strLocalPath.begin(),tolower);
	if(m_strLocalPath.find_last_of('\\') +1 != m_strLocalPath.length())
		m_strLocalPath.append("\\");	

	std::string tempfilename = m_strLocalPath+m_strLauncherName+".tmp";
	DeleteFileForce(tempfilename);
}

BOOL CX2ChinaLauncherApp::DeleteFileForce(const std::string filename)
{
	if(DeleteFileA(filename.c_str()) == 0)
	{
		if(GetLastError() == ERROR_ACCESS_DENIED)
		{
			if(GetFileAttributesA(filename.c_str()) & FILE_ATTRIBUTE_READONLY)
			{
				SetFileAttributesA(filename.c_str(),FILE_ATTRIBUTE_NORMAL);
				if(DeleteFileA(filename.c_str()) != 0)				
					return true;
			}
		}
		else if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;

}

