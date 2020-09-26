#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <iostream>
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArg, int nCmdShow)
{
    if( __argc < 2 ) return 0;

	HWND hProg; 
	hProg = FindWindow( NULL, __argv[1] ); 
	if ( hProg != NULL )
	{
		SendMessage( hProg, WM_CLOSE, 0, 0 );
	}
	else
	{
		// Display the string.
		cout << "Terminate File! - �ش� �����츦 �˻��Ҽ� �����ϴ�" << std::endl;
		return -1;
	}

    return 1;
}