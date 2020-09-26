// ServerSideAutomation.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ServerSideAutomation.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.
#include "AutoManager.h"
#include "MailSender.h"
#include "KncFirewall.h"
CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
		nRetCode = 1;
	}
	else
	{
        HRESULT hrComInit = CoInitializeEx( 0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE );
        if( hrComInit != RPC_E_CHANGED_MODE && FAILED(hrComInit) )
        {
            printf( "Failed to init COM: 0x%08lx\n", hrComInit );
            return 1; 
        }//end if

        char pathbuff[MAX_PATH]     = {0};
        ::GetModuleFileName( NULL, pathbuff, MAX_PATH );            // get file name (full path)

        // install...
        bool bReturn = KncFirewall::OnInstallApplication( pathbuff, pathbuff );
        ::printf( "Install - bReturn : %d \n\n", bReturn );
        //::system( "pause" );

        // on execute...
        int nReturn = KncFirewall::GuarantedFireWallException( pathbuff, pathbuff );
        ::printf( "\nGuaranted Firewall Exception - nReturn : %d \n\n", nReturn );
        //::system( "pause" );

        CAutoManager autoManager;        
        autoManager.Process( argc, argv );

        // on uninstall...
        bReturn = KncFirewall::OnUninstallApplication( pathbuff );
        ::printf( "\nUninstall - bReturn : %d \n\n", bReturn );
        //::system( "pause" );

     
	}

	return nRetCode;
}
