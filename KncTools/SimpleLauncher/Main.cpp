#include <windows.h>
#include <tchar.h>
#include <process.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArg, int nCmdShow)
{
    if( __argc < 2 ) return 0;

    STARTUPINFO si = {0, };
    si.cb = sizeof(si);
    si.dwFlags = 0;
    PROCESS_INFORMATION pi;

    BOOL bReturn = CreateProcess( NULL, __argv[1], NULL, NULL, FALSE,
        0, NULL, NULL, &si, &pi );

    if( !bReturn )
    {
        // GetLastError�� ���� ������ �޽��� �ڽ��� ���.
        TCHAR szMsg[MAX_PATH];
        FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            szMsg, MAX_PATH, NULL );

        _tcscat( szMsg, "\n �������ϸ� : " );
        _tcscat( szMsg, __argv[1] );

        // Display the string.
        MessageBox( NULL, szMsg, __argv[1], MB_OK | MB_ICONINFORMATION );
    }
    else
    {
        // Close process and thread handles. 
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
    }

    return 0;
}