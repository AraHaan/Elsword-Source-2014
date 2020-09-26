#include "KncFirewall.h"

#include <cstdio>       // printf
#include <process.h>    // system
#define _WIN32_DCOM     // windows.h �� ��Ŭ��� �Ǳ� ���� _WIN32_COM�� ���ǵǾ����� ������
                        // CoInitializeEx�� ����� �� ����.
#include <windows.h>    // GetModuleFileName, MAX_PATH

void main()
{
    // 050222. init COM. ���ο��� COM �ʱ�ȭ ���� �ʴ´�. ���� ���־�� �Ѵ�.
    HRESULT hrComInit = CoInitializeEx( 0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE );
    if( hrComInit != RPC_E_CHANGED_MODE && FAILED(hrComInit) )
    {
        printf( "Failed to init COM: 0x%08lx\n", hrComInit );
        return; 
    }

    char pathbuff[MAX_PATH]     = {0};
    ::GetModuleFileName( NULL, pathbuff, MAX_PATH );            // get file name (full path)

    // install...
    bool bReturn = KncFirewall::OnInstallApplication( pathbuff, "KNC��ȭ���׽�Ʈ" );
    ::printf( "Install - bReturn : %d \n\n", bReturn );
    ::system( "pause" );

    // on execute...
    int nReturn = KncFirewall::GuarantedFireWallException( pathbuff, "KNC��ȭ���׽�Ʈ" );
    ::printf( "\nGuaranted Firewall Exception - nReturn : %d \n\n", nReturn );
    ::system( "pause" );

    // on uninstall...
    bReturn = KncFirewall::OnUninstallApplication( pathbuff );
    ::printf( "\nUninstall - bReturn : %d \n\n", bReturn );
    ::system( "pause" );
}