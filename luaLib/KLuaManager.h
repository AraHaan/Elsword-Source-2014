/** @file
@author cruise@kogsoft.com
*/

#ifndef _KOG_KLUAMANAGER_H_
#define _KOG_KLUAMANAGER_H_ "$Id: KLuaManager.h,v 1.6 2003/02/17 03:50:55 cvs Exp $"
#pragma once // for _MSC_VER > 1000

#include <string>
#include <sstream> 
#include <malloc.h>


#include "lua_tinker.h"


#include <windows.h>
#include <assert.h>
#include <WTypes.h>

#ifndef STRSAFE_NO_DEPRECATE
#	define STRSAFE_NO_DEPRECATE
#endif
#include <strsafe.h>

/** LUA manager class.
LUA���� �ۼ��� configuration file�� �о� ����, �� ������ C++
���α׷����� �������� ���� class. LUA�� �⺻���� number (float ���·�
����), string 2���� ���̰�, �̵��� table (structure ����� ����)��
������ �� �ִ�. table ���� table�� �����ȴ�. �ڼ��� ������ LUA manual��
�����ϱ� �ٶ���.
 
@verbinclude testKLuaManager.cpp
*/




//#if defined(DEBUG) || defined(_DEBUG)
//#	define //CHECK_LUA_THREAD_ID ASSERT( KLuaManager::m_ThreadIDofObjectLoaderInSimulationServer != ::GetCurrentThreadId() );
//#else
//#	define //CHECK_LUA_THREAD_ID
//#endif 





#ifndef LUA_RESULT
#	define LUA_RESULT bool
#	define LUA_OK true
#	define LUA_FAIL false
#endif // LUA_RESULT





//{{ robobeg : 2008-10-29

//------------------------------------------------------------------------------
// _DEBUG
//
#if defined(_DEBUG)

#ifndef _INC_CRTDBG
#define _ASSERTE    assert
#endif // _INC_CRTDBG

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp_)         _ASSERTE(exp_)
#endif // #if !defined(ASSERT)

#else // not defined _DEBUG ////////////////////////////////////////////////////

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp)
#endif // #if !defined(ASSERT)

#endif // defined(_DEBUG)





#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))
#endif ARRAY_SIZE


// Lua stack �۾��� �� stack push/pop ��ġ�� üũ�ϱ� ����
/** _L�� ����� ������ ��������� ����� ������� �� �̸� �浹�� ���� �����Ϸ� ������ ���� ���� ���̴�.
    ����ڰ� _L�̸��� ������� �ʴ´ٰ� �����Ѵ�.
    - jintaeks on 2008-05-29, 10:28 */
#define KLUABEGIN( stack )   {  ASSERT( (stack) != NULL ); lua_State* _L = stack; lua_State* L = _L; unsigned _iCurTop, _iLuaTop; _iCurTop = _iLuaTop = (unsigned) lua_gettop( L );
#define KLUAEND()               ASSERT( L != NULL ); _iCurTop = (unsigned) lua_gettop( L ); ASSERT( _iLuaTop == _iCurTop ); }
#define KLUAENDWITHPUSH( arg )  ASSERT( L != NULL ); _iCurTop = (unsigned) lua_gettop( L ); ASSERT( _iLuaTop + (arg) == _iCurTop ); }
#define KLUAENDWITHPOP( arg )  ASSERT( L != NULL ); _iCurTop = (unsigned) lua_gettop( L ); ASSERT( _iLuaTop - (arg) == _iCurTop ); }
#define KLUAENDUNWIND()         ASSERT( L != NULL ); lua_settop( L, _iLuaTop ); }


//}} robobeg : 2008-10-29



//#ifndef PROFILE_BUILD
//	#define KTDXPROFILE()
//	#define KTDXPROFILE2(name)
//	#define KTDXPROFILE_BEGIN(name)
//	#define KTDXPROFILE_END()
//#endif


#define LUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == false ) \
		(dest) = (initValue); \
}

#define LUA_GET_VALUE_UTF8( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager).GetValueUtf8( (key), (dest) ) == false ) \
		(dest) = (initValue); \
}

#define LUA_GET_CHARARRAY( luaManager, key, dest, size, initValue ) \
{ \
	if( dest != NULL && (luaManager).GetValue( (key), (dest), (size) ) == false ) \
		StringCchCopyA( (dest), (size), (initValue) ); \
}

#define LUA_GET_CHARARRAY_UTF8( luaManager, key, dest, size, initValue ) \
{ \
	if( dest != NULL && (luaManager).GetValueUtf8( (key), (dest), (size) ) == false ) \
		StringCchCopyA( (dest), (size), (initValue) ); \
}


#define LUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
}

#define LUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == false ) \
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
	if( (luaManager).GetValue( (key), intBuffer ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
	else \
	{ \
		dest = (destType)(intBuffer); \
	} \
}


//pointer
#define PLUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == false ) \
		(dest) = (initValue); \
}

#define PLUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
}

#define PLUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager)->GetValue( (key), intBuffer ) == false ) \
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
	if( (luaManager)->GetValue( (key), intBuffer ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
	else \
	{ \
		dest = (destType)(intBuffer); \
	} \
}


  
//{{ 2008. 3. 18  ������  ���� ��Ʈ��
#define LUA_GET_VALUE_RETURN_ERR( luaManager, key, dest, initValue, errStr, returnCode ) \
{ \
 if( (luaManager).GetValue( (key), (dest) ) == LUA_FAIL ) \
 { \
 (dest) = (initValue); \
 (errStr) = (key); \
 returnCode; \
 } \
}
//}}




//{{ 2008. 3. 18  ������  ���� ��Ʈ��
#define LUA_GET_VALUE_RETURN_ENUM_ERR( luaManager, key, dest, destType, initValue, errStr, returnCode ) \
{ \
 int intBuffer = 0; \
 if( (luaManager).GetValue( (key), intBuffer ) == LUA_FAIL ) \
 { \
 (dest) = (initValue); \
 (errStr) = (key); \
 returnCode; \
 } \
 else \
 { \
 dest = (destType)(intBuffer); \
 } \
}
//}} 



//{{ dmlee 2008.06.01
//
//#define LUA_GET_NODEFAULT_( from, key, ret ) \
//	KNewLuaManager::Get_( (from), (key), (ret) )
//
//#define LUA_GET_( from, key, ret, def ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (ret) ) ) \
//	{ (ret) = (def); } \
//} 
//
//#define LUA_GET_RETURN_( from, key, ret, def, returnCode ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (ret) ) )	\
//	{ (ret) = (def); returnCode; } \
//}
//
//
//
//
//#define LUA_GET_SUFFIX_NODEFAULT_( from, key, suffix, ret ) \
//	KNewLuaManager::Get_( (from), (key), (suffix), (ret) )
//
//#define LUA_GET_SUFFIX_( from, key, suffix, ret, def ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (suffix), (ret) ) ) \
//	{ (ret) = (def); } \
//}
//
//#define LUA_GET_SUFFIX_RETURN_( from, key, suffix, ret, def, returnCode ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (suffix), (ret) ) ) \
//	{ (ret) = (def); returnCode; } \
//}
//
//
//class KNewLuaManager
//{
//public:
//	// global���� index�� �� �б�
//	template< typename T >
//		static inline bool Get_( lua_State* pLuaState, const int iIndex, T& ret )
//	{		
//		return lua_tinker::get_( pLuaState, iIndex, ret );
//	}
//
//	template<>
//		static inline bool Get_( lua_State* pLuaState, const int iIndex, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = lua_tinker::get_<std::string>( pLuaState, iIndex, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//
//	// global���� key name���� �� �б�
//	template< typename T >
//		static inline bool Get_( lua_State* pLuaState, const char* szName, T& ret )
//	{		
//		return lua_tinker::get_( pLuaState, szName, ret );
//	}
//
//	template<>
//		static inline bool Get_( lua_State* pLuaState, const char* szName, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = lua_tinker::get_<std::string>( pLuaState, szName, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//
//
//	// global���� key name���� �� �б� (����)
//	template< typename T >
//		static inline bool Get_( lua_State* pLuaState, const char* szName, const int iSuffix, T& ret )
//	{	
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		return lua_tinker::get_( pLuaState, strm.str().c_str(), ret );
//	}
//
//	template<>
//		static inline bool Get_( lua_State* pLuaState, const char* szName, const int iSuffix, std::wstring& ret )
//	{		
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		std::string strBuf = "";
//		bool bOK = lua_tinker::get_<std::string>( pLuaState, strm.str().c_str(), strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//
//
//
//	// lua table���� index�� �� �б�
//	template< typename T >
//		static inline bool Get_(lua_tinker::table& luaTable, const int iIndex, T& ret )
//	{		
//		return luaTable.get_( iIndex, ret );
//	}
//
//	template<>
//		static inline bool Get_(lua_tinker::table& luaTable, const int iIndex, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = luaTable.get_( iIndex, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//	// lua table���� key name���� �� �б�
//	template< typename T >
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, T& ret )
//	{		
//		return luaTable.get_( szName, ret );
//	}
//
//	template<>
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = luaTable.get_<std::string>( szName, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//	// lua table���� key name���� �� �б� ( ���� )
//	template< typename T >
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, const int iSuffix, T& ret )
//	{		
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		return luaTable.get_( strm.str().c_str(), ret );
//	}
//
//	template<>
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, const int iSuffix, std::wstring& ret )
//	{		
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		std::string strBuf = "";
//		bool bOK = luaTable.get_( strm.str().c_str(), strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//private:
//	static inline void _ConvertCharToWCHAR( std::wstring& dest, const char* pSrc )
//	{
//		dest.clear();
//		if ( pSrc == NULL )
//			return;
//
//		int iSrcLength = (int)strlen( pSrc );
//		if ( iSrcLength <= 0 )
//		{
//			dest = L"";
//			return;
//		}
//
//		int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, NULL, 0 );
//		if ( iDestSize <= 0 )
//		{
//			dest = L"";
//			return;
//		}
//
//		WCHAR* wcharBuf = (WCHAR*) _alloca( iDestSize * sizeof( WCHAR ) );
//		int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
//		dest.assign( wcharBuf, iNewSize );
//	}
//};

//}} dmlee 2008.06.01


class KLuaManager
{
public:

//{{ robobeg : 2008-10-28

// KLuaManager �� ������ �� �׻� �ܺο��� lua_State �� �Ѱܹ޵��� ����
// �� bInNewEnv_ = true �� ��쿡�� lua_State�� global environment ���
// �������� �� global environment �� ���ȴ�.
//
// �� KLuaManager �� ���� ���̺귯���� global variable���� ������ ��
// SetAsTemplateEnvironment ����� �����ϰ� ����
// ���� �����Ǵ� KLuaManager ��ü���� ��� �� environment �� �⺻������ ��ӹ޴´�.
//
// ������ KLuaManager(void) ������ constructor �� �����Ǵ� constructor argument�� 
//   KLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true )
// �̴�.

//{{ robobeg : 2012-11-17
	// �ƹ� lua state �� �Ҵ���� ���� ����. �� �༮�� ����ϸ� ũ���� �߻��Ѵ�!
	KLuaManager();
//}} robobeg : 2012-11-17


    /// Default constructor.
    //KLuaManager(void);
    KLuaManager( lua_State* pLuaState, int iTableDepth = 1, bool bInNewEnv_ = false );

    /// Constructor with initial internal stack size.
    //KLuaManager(int iStackSize);

	/// Constructor for LuaTinker Helper
	//KLuaManager(lua_State* pLuaState, int iTableDepth = 1);

//}} robobeg : 2008-10-28

    /// Destructor.
    ~KLuaManager(void);

//{{ robobeg : 2012-11-17
	// ������ lua state �� �����ϰ� �ʱ� ���·� �����. 
	void	DetachLuaState();
	void	AssignNewLuaState( lua_State* pLuaState, int iTableDepth, bool bInNewEnv_ );
//}} robobeg : 2012-11-17

private:
    /// Copy constructor: DISABLED.
    KLuaManager(const KLuaManager&);
    /// Assignment operator: DISABLED.
    void operator=(const KLuaManager&);
public:

//{{ robobeg : 2008-10-28
    void    Reset();

    /// ���� KLuaManager �� environment�� ���� ��� KLuaManager ��ü���� ��ӹ޵��� �Ѵ�.
    //HRESULT SetAsTemplateEnvironment();

    /// ���� environment �� �ε�� global variables ���� original lua state �� global environment �� �����Ѵ�.
    //HRESULT ExportEnvironment();

	bool	ExportFunctionsToGlobalEnv();

//}} robobeg : 2008-10-28


    /// String representation.
    std::string ToString(void) const;
    /// Get the low-level LUA state.
    inline lua_State* GetLuaState(void) { return m_pkLuaState; }

    // COMMAND EXECUTION ***********************************************

    /// string���� �־��� LUA ����� �����Ѵ�.
    bool DoString(const char* pszStr, bool bLogOutput = true);
	/// �޸� �ε�
	bool DoMemory( const char* pRealFile, const long nSize, bool bLogOutput = true )
	{
		//CHECK_LUA_THREAD_ID;


#ifdef _ENCRIPT_SCRIPT_
		pRealFile = XORDecrypt( pRealFile, nSize );
#endif

		bool bRetVal = DoMemoryNotEncript( pRealFile, nSize );

#ifdef _ENCRIPT_SCRIPT_
		SAFE_DELETE_ARRAY( pRealFile );
#endif
		return bRetVal;
	}
	bool DoMemoryNotEncript( const char* pRealFile, const long nSize, bool bLogOutput = true )
    {
		//CHECK_LUA_THREAD_ID;

        //{{ 2009. 07. 17. jseop
        ASSERT( pRealFile );
        if( !pRealFile )
        {
            return false;
        }

        bool bOK;

        if( lua_getencoding( GetLuaState() ) == LUA_ENCODING_UTF8 )
        {
            ASSERT( nSize > 0 );
            if( nSize <= 0 )
            {
                return false;
            }

            if( pRealFile[0] == ( char )033 )
            {
                bOK = lua_tinker::dobuffer( m_pkLuaState, pRealFile, nSize );
            }
            else
            {
                ASSERT( nSize >= 3 );
                if( nSize < 3 )
                {
                    return false;
                }

                ASSERT( ( pRealFile[0] == ( char )0xef ) && ( pRealFile[1] == ( char )0xbb ) && ( pRealFile[2] == ( char )0xbf ) );
                if( ( pRealFile[0] != ( char )0xef ) || ( pRealFile[1] != ( char )0xbb ) || ( pRealFile[2] != ( char )0xbf ) )
                {
                    return false;
                }

                bOK = lua_tinker::dobuffer( m_pkLuaState, pRealFile + 3, nSize - 3 );
            }
        }
        else
        {
            bOK = lua_tinker::dobuffer( m_pkLuaState, pRealFile, nSize );
        }
        //{{

        return bOK;
    }

    /// file�� �־��� LUA ����� �����Ѵ�.
    bool DoFile(const char* pszFileName, bool bLogOutput = true);

    // TABLE HANDLING **************************************************

    /// pszName�� �̸��� ������ table�� LUA stack���� �����´�.
	bool BeginTable(const char* pszName, int index, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
		char charBuf[255] = {0,};
		StringCchPrintfA( charBuf, ARRAY_SIZE(charBuf), "%s%d", pszName, index );
		return BeginTable( charBuf, bLogOutput );
	}
	
	bool BeginTable(const wchar_t* pszName, int index, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		char charBuf2[255] = {0,};
		StringCchPrintfA( charBuf2, ARRAY_SIZE(charBuf2), "%s%d", charBuf, index );
		return BeginTable( charBuf2, bLogOutput );
	}
	
	bool BeginTable(const wchar_t* pszName, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return BeginTable( charBuf, bLogOutput );
	}

    bool BeginTable(const char* pszName, bool bLogOutput = true);
    /// (table ������) iIndex��° table�� LUA stack���� �����´�.
    bool BeginTable(int iIndex, bool bLogOutput = true);
    /// LUA stack���� ���� �ֱٿ� ����� table�� �����Ѵ�.
    bool EndTable(bool bLogOutput = true);

//{{ robobeg : 2008-10-28

	bool MakeTableReference( const wchar_t* pszName, int iInRef_, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return MakeTableReference( charBuf, iInRef_, bLogOutput );
	}//

    bool MakeTableReference( const char* pszName, int iInRef_, bool bLogOutput = true);

    bool BeginTableByReference( int iInRef_, bool bLogOutput = true );

//}} robobeg : 2008-10-28

//{{ robobeg : 2013-01-17
	int	GetLuaGeneralRef()	{ return m_iLuaGeneralRef; }
	int	MakeGeneralReference( const char* pszName, int type, bool bRawGlobal = false );

	int MakeFunctionReference( const std::wstring& funcName, bool bRawGlobal = false );
	int MakeFunctionReference( const wchar_t* funcName, bool bRawGlobal = false );
	int MakeFunctionReference( const char* funcName, bool bRawGlobal = false );
//}} robobeg : 2013-01-17

//{{ robobeg : 2013-10-24
    int GetNumIndexedValues();
    int GetNumIndexedValues( const char* pszName );
    int GetNumIndexedTables();
    int GetNumIndexedTables( const char* pszTableName );
//}} robobeg : 2013-10-24

    // GET BOOL-TYPE VALUE *********************************************

    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, bool& bValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, bValue, bLogOutput );
	}
    bool GetValue(const char* pszName, bool& bValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, bool& bValue, bool bLogOutput = true);

    // GET INTEGER-TYPE VALUE ******************************************

    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, short& sValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, sValue, bLogOutput );
	}
    bool GetValue(const char* pszName, short& sValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, short& sValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, int& iValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, iValue, bLogOutput );
	}
    bool GetValue(const char* pszName, int& iValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, int& iValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* szName, long& lValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, lValue, bLogOutput );
	}
    bool GetValue(const char* szName, long& lValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, long& lValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, unsigned short& usValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, usValue, bLogOutput );
	}
    bool GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, unsigned short& usValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, unsigned int& uiValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, uiValue, bLogOutput );
	}
    bool GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* szName, unsigned long& ulValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, ulValue, bLogOutput );
	}
    bool GetValue(const char* szName, unsigned long& ulValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput = true);

    // GET FLOAT-TYPE VALUE ********************************************

    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, float& fValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    bool GetValue(const char* pszName, float& fValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, float& fValue, bool bLogOutput = true);
    /// pszName�� �̸��� ������ ���� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, double& fValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    bool GetValue(const char* pszName, double& fValue, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, double& fValue, bool bLogOutput = true);





    // GET STRING-TYPE VALUE *******************************************

    /// pszName�� �̸��� ������ ���ڿ� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, std::wstring& wstrValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        wstrValue.clear();

		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
		std::string strBuf;
		bool retval = GetValueUtf8( charBuf, strBuf, bLogOutput );
		if( retval == false )
			return retval;

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* ) _alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        wstrValue = wszBuffer;

		return true;
	}
	bool GetValue(const char* pszName, std::wstring& wstrValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        wstrValue.clear();

		std::string strBuf;
		bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
		if( retval == false )
			return retval;

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* ) _alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        wstrValue = wszBuffer;

		return true;
	}
	bool GetValue(const wchar_t* pszName, std::string& strValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        strValue.clear();

		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
        std::string strBuf;
		bool retval = GetValueUtf8( charBuf, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strValue = szBuffer;

        return true;
	}
    bool GetValue(const char* pszName, std::string& strValue, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();
        strValue.clear();

        std::string strBuf;
        bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strValue = szBuffer;

        return true;
    }
    /// (table ������) iIndex��° ������ ���� �����´�.
	bool GetValue(int iIndex, std::wstring& wstrValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        wstrValue.clear();

		std::string strBuf;
		bool retval = GetValueUtf8( iIndex, strBuf, bLogOutput );
		if( retval == false )
			return retval;

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* ) _alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        wstrValue = wszBuffer;

		return true;
	}
    bool GetValue(int iIndex, std::string& strValue, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();
        strValue.clear();

        std::string strBuf;
        bool retval = GetValueUtf8( iIndex, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strValue = szBuffer;

        return true;
    }
    /// pszName�� �̸��� ������ ���ڿ� ������ ���� �����´�.
	bool GetValue(const wchar_t* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();

        char charBuf[511] = {0,};
        WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
        std::string strBuf;
        bool retval = GetValueUtf8( charBuf, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strncpy( pszValue, szBuffer, iMaxLen );

        return true;
	}
    bool GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();

        std::string strBuf;
        bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strncpy( pszValue, szBuffer, iMaxLen );

        return true;
    }

	bool GetValue( const char* pszName, WCHAR* wszValue, int bufferSize, bool bLogOutput = true )
	{
		//CHECK_LUA_THREAD_ID;

		ASSERT( NULL != wszValue );

		std::string strBuf;
		bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
		if( retval == false )
			return retval;

		::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszValue, ( int )bufferSize-1 );
		wszValue[ bufferSize-1 ] = NULL;
		return true;
	}



    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();

        std::string strBuf;
        bool retval = GetValueUtf8( iIndex, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strncpy( pszValue, szBuffer, iMaxLen );

        return true;
    }




private:
    bool GetValueUtf8(const char* pszName, std::string& strValue, bool bLogOutput = true);
    bool GetValueUtf8(int iIndex, std::string& strValue, bool bLogOutput = true);
    bool GetValueUtf8(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// (table ������) iIndex��° ������ ���� �����´�.
    bool GetValueUtf8(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// Internal function: pszName�� �̸��� ������ ����/table�� LUA stack���� �����´�.
    bool Get(const char* pszName);
    /// Internal function: (table ������) iIndex��° ����/table�� LUA stack���� �����´�.
    bool Get(int iIndex);

	void _GenerateLuaTableRef();
	void _GenerateLuaGeneralRef();

private:
    lua_State* m_pkLuaState; ///< LUA state: ���� ������ ���.
    int m_iTableDepth; ///< LUA stack�� �ö� table�� ����.

//{{ robobeg : 2008-10-28
	//bool m_bLuaTinkerHelper;	// Lua Tinker�� ����ϱ� ���ѰŶ��...m_pkLuaState�� �������� �������� �ʰ�, ��븸 ��
    lua_State*  m_pkLuaStateOriginal;
    int         m_iLuaStateRef;
    // m_iLuaStateRef == LUA_REFNIL �̸� ������ m_bLuaTinkerHelper == true �� ����
//}} robobeg : 2008-10-28
//{{ robobeg : 2012-11-21
	int			m_iLuaTableRef;
//}} robobeg : 2012-11-21
//{{ robobeg : 2013-01-17
	int			m_iLuaGeneralRef;
//}] robobeg : 2013-01-17
	
//private:
//    int m_iLogLevel;



//public: 
//	static DWORD m_ThreadIDofObjectLoaderInSimulationServer;

};

#endif // _KOG_KLUAMANAGER_H_
