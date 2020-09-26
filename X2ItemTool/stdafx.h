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
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ����

#include <afxdtctl.h>		// Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC ����
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows ���� ��Ʈ�ѿ� ���� MFC ����
#endif // _AFX_NO_AFXCMN_SUPPORT



#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::wstring;
using std::string;
using std::wstringstream;
using std::stringstream;



#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "luaLibD.lib" )
#else
#pragma comment( lib, "luaLib.lib" )
#endif


#include "../luaLib/KLuaManager.h"
#include "../luaLib/KLuaBinder.h"

inline void TableBind( KLuaManager* pLuaManager, KLuabinder* pLuaBinder )
{
	bool	retVal = true;
	int		index = 1;
	string	buffer;

	retVal = pLuaManager->GetValue(index,buffer);
	while( retVal == true )
	{
		pLuaBinder->DoString( buffer.c_str() );
		retVal = pLuaManager->GetValue(index,buffer);
		index++;
	}
}











#include "zlib/zlib.h"
#pragma comment( lib, "zlib/zlibstat.lib" )


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif    


typedef __int64 UidType;




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

inline void ConvertCharToWCHAR( wstring& dest, const char* pSrc )
{
	KTDXPROFILE();

//{{ robobeg : 2008-01-08
	//if ( pSrc == NULL )
	//	return;

	//int iSize = strlen( pSrc );
	//if ( iSize <= 0 )
	//	return;

	//WCHAR* wcharBuf = (WCHAR*) alloca( (iSize + 1) * sizeof( WCHAR ) );
	//ZeroMemory( wcharBuf, (iSize + 1) * sizeof( WCHAR ) );

	//MultiByteToWideChar( CP_ACP, 0, pSrc, -1, wcharBuf, iSize );
	//dest = wcharBuf;

    dest.clear();
    if ( pSrc == NULL )
        return;

    int iSrcLength = strlen( pSrc );
    if ( iSrcLength <= 0 )
    {
        dest = L"";
        return;
    }//if

    int iDestSize = ::MultiByteToWideChar( CP_ACP, 0, pSrc, iSrcLength, NULL, 0 );
    if ( iDestSize <= 0 )
    {
        dest = L"";
        return;
    }//if

	WCHAR* wcharBuf = (WCHAR*) _alloca( iDestSize * sizeof( WCHAR ) );
    ASSERT( wcharBuf != NULL );
    int iNewSize = ::MultiByteToWideChar( CP_ACP, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
    ASSERT( iNewSize == iDestSize );
    dest.assign( wcharBuf, iNewSize );
//}} robobeg : 2008-01-08
}
inline void ConvertWCHARToChar( string& dest, const WCHAR* pSrc )
{
	KTDXPROFILE();

//{{ robobeg : 2008-01-08
	//if ( pSrc == NULL )
	//	return;

	//int iSize = wcslen( pSrc ) * sizeof( WCHAR );
	//if ( iSize <= 0 )
	//	return;

	//char* charBuf = (char*) alloca( iSize + 1 );
	//ZeroMemory( charBuf, iSize + 1 );

	//WideCharToMultiByte( CP_ACP, 0, pSrc, -1, charBuf, iSize, NULL, NULL );
	//dest = charBuf;

    dest.clear();
    if ( pSrc == NULL )
        return;

    int iSrcLength = wcslen( pSrc );
    if ( iSrcLength <= 0 )
    {
        dest = "";
        return;
    }//if

    int iDestSize = ::WideCharToMultiByte( CP_ACP, 0, pSrc, iSrcLength, NULL, 0, NULL, NULL );
    if ( iDestSize <= 0 )
    {
        dest = "";
        return;
    }//if

    char* pszConv = (char*) _alloca( iDestSize * sizeof(char) );
    ASSERT( pszConv != NULL );
    int iNewSize = ::WideCharToMultiByte( CP_ACP, 0, pSrc, iSrcLength, pszConv, iDestSize, NULL, NULL );
    ASSERT( iNewSize == iDestSize );
    dest.assign( pszConv, iNewSize );
//}} robobeg : 2008-01-08
}



#include "KGCMassFileManager.h"

#include "x2itemtoolX2Item.h"
#include "x2itemtoolX2ItemManager.h"

#include "GridCtrl/GridCtrl.h"
#include "X2ItemToolCommon.h"

extern CX2ItemToolCommon* g_pItemToolCommon;

