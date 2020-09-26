#include <windows.h>
#include <tchar.h>
#include <process.h>

void RunProcess( char* pcFile, char* pcPath )
{
	STARTUPINFO si = {0, };
	si.cb = sizeof(si);
	si.dwFlags = 0;
	PROCESS_INFORMATION pi;

	BOOL bReturn = CreateProcess( NULL, pcFile, NULL, NULL, FALSE,
		0, NULL, pcPath, &si, &pi );

	if( !bReturn )
	{
		// GetLastError�� ���� ������ �޽��� �ڽ��� ���.
		TCHAR szMsg[MAX_PATH];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			szMsg, MAX_PATH, NULL );

		_tcscat( szMsg, "\n �������ϸ� : " );
		_tcscat( szMsg, pcFile );

		// Display the string.
		MessageBox( NULL, szMsg, pcFile, MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArg, int nCmdShow)
{
	if( __argc < 5 ) return 0;
	
	RunProcess( __argv[1], __argv[2] );

	for ( int iT = 0; iT < 1000; ++iT )
	{
		HWND hProcessCenterServer = FindWindow( NULL, "CenterServer_Ready" ); 
		if ( hProcessCenterServer != NULL )
		{
			RunProcess( __argv[3], __argv[4] );
			return 0;
		}

		Sleep( 50 );
	}

	return 0;
}