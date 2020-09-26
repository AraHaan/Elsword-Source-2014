// stdafx.h : �� ������� �ʰ� ���� ����ϴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ������� ���� ������ �ʴ� ������ ���ܽ�ŵ�ϴ�.
#endif

// �Ʒ� ������ �÷������� �켱�ϴ� �÷����� ������� �ϴ� ��� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER				// Windows 95 �� Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define WINVER 0x0400		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINNT 0x0400		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINDOWS 0x0410 // Windows Me ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_IE			// IE 4.0 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_IE 0x0400	// IE 5.0 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ���
#include <afxext.h>         // MFC �ͽ��ټ�

#include <afxdtctl.h>		// Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC ����
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows ���� ��Ʈ�ѿ� ���� MFC ����
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxole.h>

#include <CdErr.h>
#include <shlwapi.h>

extern UINT g_uCustomClipbrdFormat;
extern bool g_bNT;

// To do..
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "zlibd.lib" )
#else
#pragma comment( lib, "zlib.lib" )
#endif

#pragma comment( lib, "zlibstat.lib" )

#include <process.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>


inline void MakeUpperCase(char* str) 
{
	for( char* i = str; (*i) != 0; i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(std::string &str) 
{
	for( std::string::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(WCHAR* str) 
{
	for( WCHAR* i = str; (*i) != 0; i++ ) 
		*i = (WCHAR)towupper(*i);
}

inline void MakeUpperCase(std::wstring &str) 
{
	for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (WCHAR)towupper(*i);
}

inline void ConvertCharToWCHAR( std::wstring& dest, const char* pSrc )
{
	if ( pSrc == NULL )
		return;

	int iSize = strlen( pSrc );
	if ( iSize <= 0 )
		return;

	WCHAR* wcharBuf = (WCHAR*) alloca( (iSize + 1) * sizeof( WCHAR ) );
	ZeroMemory( wcharBuf, (iSize + 1) * sizeof( WCHAR ) );

	MultiByteToWideChar( CP_ACP, 0, pSrc, -1, wcharBuf, iSize );
	dest = wcharBuf;
}
inline void ConvertWCHARToChar( std::string& dest, const WCHAR* pSrc )
{
	if ( pSrc == NULL )
		return;

	int iSize = wcslen( pSrc ) * sizeof( WCHAR );
	if ( iSize <= 0 )
		return;

	char* charBuf = (char*) alloca( iSize + 1 );
	ZeroMemory( charBuf, iSize + 1 );

	WideCharToMultiByte( CP_ACP, 0, pSrc, -1, charBuf, iSize, NULL, NULL );
	dest = charBuf;
}

#define ERRORMSG( s )	::MessageBox( NULL, s, L"ERROR.", MB_OK | MB_ICONERROR )
#define INFORMSG( s )	::MessageBox( NULL, s, L"Information.!", MB_OK | MB_ICONINFORMATION )

#include "zlib.h"

#include "KGCBufferManager.h"
#include "MassFile.h"

#include "X2KomFileViewer.h"
#include "X2KomFileViewerDlg.h"
#include "LuaCompileUtil.h"
#include "X2KomFileManager.h"
#include <afxdhtml.h>

//lua ����� ���� 
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "lualibd.lib" )
#else
#pragma comment( lib, "lualib.lib" )
#endif
//#include <ImportKncLua.h>
#include <lua_tinker.h>
#include "KLuaManager.h"


extern lua_State* g_pLua;
