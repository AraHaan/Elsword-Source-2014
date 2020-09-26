/** @file
@author cruise@kogsoft.com
*/

#ifndef _KLUAMANAGER_H_
#define _KLUAMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <KncLua.h>
//#include "LUA/lua.h"
//#include "LUA/lauxlib.h"
//#include "LUA/lualib.h"
#include <windows.h>
#include <assert.h>

/** LUA manager class.
LUA���� �ۼ��� configuration file�� �о� ����, �� ������ C++
���α׷����� �������� ���� class. LUA�� �⺻���� number (float ���·�
����), string 2���� ���̰�, �̵��� table (structure ����� ����)��
������ �� �ִ�. table ���� table�� �����ȴ�. �ڼ��� ������ LUA manual��
�����ϱ� �ٶ���.

@verbinclude testKLuaManager.cpp
*/

#define LUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == E_FAIL ) \
	(dest) = (initValue); \
}

#define LUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
}

//{{ 2008. 3. 18  ������  ���� ��Ʈ��
#define LUA_GET_VALUE_RETURN_ERR( luaManager, key, dest, initValue, errStr, returnCode ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	(errStr) = (key); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
}
//}}

#define LUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	} \
	else \
	{ \
	(dest) = (destType)intBuffer; \
	} \
}

#define LUA_GET_VALUE_RETURN_ENUM( luaManager, key, dest, destType, initValue, returnCode ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
	else \
	{ \
	dest = (destType)(intBuffer); \
	} \
}

//{{ 2008. 3. 18  ������  ���� ��Ʈ��
#define LUA_GET_VALUE_RETURN_ENUM_ERR( luaManager, key, dest, destType, initValue, errStr, returnCode ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	(errStr) = (key); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
	else \
	{ \
	dest = (destType)(intBuffer); \
	} \
}
//}}


//pointer
#define PLUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == E_FAIL ) \
	(dest) = (initValue); \
}

#define PLUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	returnCode; \
	} \
}

#define PLUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager)->GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	} \
	else \
	{ \
	(dest) = (destType)intBuffer; \
	} \
}

#define PLUA_GET_VALUE_RETURN_ENUM( luaManager, key, dest, destType, initValue, returnCode ) \
{ \
	int intBuffer = 0; \
	if( (luaManager)->GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	returnCode; \
	} \
	else \
	{ \
	dest = (destType)(intBuffer); \
	} \
}

//typedef long HRESULT;

class KLuaManager
{
public:
    /// Default constructor.
    KLuaManager(void);
    /// Constructor with initial internal stack size.
    KLuaManager(int iStackSize);
    /// Constructor for LuaTinker Helper
    KLuaManager(lua_State* pLuaState);
    /// Destructor.
    ~KLuaManager(void);
private:
    /// Copy constructor: DISABLED.
    KLuaManager(const KLuaManager&);
    /// Assignment operator: DISABLED.
    void operator=(const KLuaManager&);
public:
    /// String representation.
    std::string ToString(void) const;
    /// Get the low-level LUA state.
    inline lua_State* GetLuaState(void) { return m_pkLuaState; }

    // CHECK TYPE
    bool IsNumber( const char* pszName );
    bool IsNumber( int iIndex );
    bool IsString( const char* pszName );
    bool IsString( int iIndex );

    // COMMAND EXECUTION ***********************************************

    /// string���� �־��� LUA ����� �����Ѵ�.
    HRESULT DoString(const char* pszStr, bool bLogOutput = true);
    /// file�� �־��� LUA ����� �����Ѵ�.
    HRESULT DoFile(const char* pszFileName, bool bLogOutput = true);

    // TABLE HANDLING **************************************************

    /// pszName�� �̸��� ������ table�� LUA stack���� �����´�.
    HRESULT BeginTable(const char* pszName, bool bLogOutput = true);
	HRESULT BeginTable(const WCHAR* pszName, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return BeginTable( charBuf, bLogOutput );
	}
    /// (table ������) iIndex��° table�� LUA stack���� �����´�.
    HRESULT BeginTable(int iIndex, bool bLogOutput = true);
    /// LUA stack���� ���� �ֱٿ� ����� table�� �����Ѵ�.
    HRESULT EndTable(bool bLogOutput = true);

    // GET BOOL-TYPE VALUE *********************************************

    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
    HRESULT GetValue(const char* pszName, bool& bValue, bool bLogOutput = true);
	HRESULT GetValue(const WCHAR* pszName, bool& bValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, bValue, bLogOutput );
	}
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, bool& bValue, bool bLogOutput = true);

    // GET INTEGER-TYPE VALUE ******************************************

    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, short& sValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, sValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, short& sValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, short& sValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
    HRESULT GetValue(const char* pszName, int& iValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, int& iValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, iValue, bLogOutput );
	}
    HRESULT GetValue(int iIndex, int& iValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
    HRESULT GetValue(const char* szName, long& lValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* szName, long& lValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, lValue, bLogOutput );
	}
    HRESULT GetValue(int iIndex, long& lValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
    HRESULT GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, unsigned short& usValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, usValue, bLogOutput );
	}
    HRESULT GetValue(int iIndex, unsigned short& usValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, unsigned int& uiValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, uiValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* szName, unsigned long& ulValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, ulValue, bLogOutput );
	}
    HRESULT GetValue(const char* szName, unsigned long& ulValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput = true);

    HRESULT GetValue(const char* pszName, char& cValue, bool bLogOutput = true);
    HRESULT GetValue(int iIndex, char& cValue, bool bLogOutput = true);

    // GET FLOAT-TYPE VALUE ********************************************

    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, float& fValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, float& fValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, float& fValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, double& fValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, double& fValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, double& fValue, bool bLogOutput = true);

    // GET STRING-TYPE VALUE *******************************************

	HRESULT GetValue(const WCHAR* pszName, std::wstring& wstrValue, bool bLogOutput = true)
	{
		char charBuf[511] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 511, NULL, NULL );
		std::string strBuf;
		HRESULT retval = GetValue( charBuf, strBuf, bLogOutput );
		if( retval == E_FAIL )
			return retval;

		WCHAR wcharBuf[511] = {0,};
		::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wcharBuf, 511 );
		wstrValue = wcharBuf;

		return retval;
	}
    /// pszName�� �̸��� ������ ���ڿ� ������ ���� �����´�.
    HRESULT GetValue(const char* pszName, std::string& strValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
	HRESULT GetValue(int iIndex, std::wstring& wstrValue, bool bLogOutput = true)
	{
		std::string strBuf;
		HRESULT retval = GetValue( iIndex, strBuf, bLogOutput );
		if( retval == E_FAIL )
			return retval;

		WCHAR wcharBuf[511] = {0,};
		::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wcharBuf, 511 );
		wstrValue = wcharBuf;

		return retval;
	}
    HRESULT GetValue(int iIndex, std::string& strValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���ڿ� ������ ���� �����´�.
	HRESULT GetValue(const WCHAR* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true)
	{
		char charBuf[511] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 511, NULL, NULL );
		return GetValue( charBuf, pszValue, iMaxLen, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    HRESULT GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true);

    //private:
    /// Internal function: pszName�� �̸��� ������ ����/table�� LUA stack���� �����´�.
    HRESULT Get(const char* pszName);
    /// Internal function: (table ������) iIndex��° ����/table�� LUA stack���� �����´�.
    HRESULT Get(int iIndex);

private:
    lua_State* m_pkLuaState; ///< LUA state: ���� ������ ���.
    int m_iTableDepth; ///< LUA stack�� �ö� table�� ����.
    bool m_bLuaTinkerHelper;	// Lua Tinker�� ����ϱ� ���ѰŶ��...m_pkLuaState�� �������� �������� �ʰ�, ��븸 ��

private:    
    int m_iLogLevel;
};

#endif // _KLUAMANAGER_H_
