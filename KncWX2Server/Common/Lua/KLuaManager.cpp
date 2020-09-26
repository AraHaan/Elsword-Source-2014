/** @file
@author cruise@kogsoft.com
*/

const static char* cvsid = "$Id: KLuaManager.cpp,v 1.5 2003/02/17 03:50:55 cvs Exp $";

#include <sstream>
#include "KLuaManager.h"

/** Default constructor. */
KLuaManager::KLuaManager(void)
: m_iTableDepth(0), m_bLuaTinkerHelper(false)
{
    //m_pkLuaState = lua_open(0);
    m_pkLuaState = lua_open();

    luaopen_base(m_pkLuaState);
}

/** Constructor with initial internal stack size.
@param iStackSize [in] LUA stack�� �ʱ� ũ��
*/
KLuaManager::KLuaManager(int iStackSize)
: m_iTableDepth(0), m_bLuaTinkerHelper(false)
{
    //m_pkLuaState = lua_open(iStackSize);
    m_pkLuaState = lua_open();    
}

KLuaManager::KLuaManager( lua_State* pLuaState )
: m_iTableDepth(0), m_bLuaTinkerHelper(false)
{
    m_pkLuaState = pLuaState;
    m_bLuaTinkerHelper = true;
}

/** Destructor. */
KLuaManager::~KLuaManager(void)
{
    if( !m_bLuaTinkerHelper )
    {
        if (m_pkLuaState != NULL)
        {
            lua_close(m_pkLuaState);
            m_pkLuaState = NULL;
        }
    }
    m_iTableDepth = 0;
}

/** String representation. */
std::string KLuaManager::ToString(void) const
{
    std::ostringstream s;
    s << "KLuaManager (luaState=" << m_pkLuaState;
    s << ",tableDepth=" << m_iTableDepth << ")";
    return s.str();
}

/** string���� �־��� LUA ����� �����Ѵ�.
@param pszStr [in] LUA ����� ��� �ִ� string
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure
*/
HRESULT KLuaManager::DoString(const char* pszStr, bool bLogOutput)
{
    if (luaL_dostring(m_pkLuaState, pszStr) == 0)
    {
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
}

/** file�� �־��� LUA ����� �����Ѵ�.
@param pszFileName [in] LUA ����� ��� �ִ� file�� �̸�
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure.
*/
HRESULT KLuaManager::DoFile(const char* pszFileName, bool bLogOutput)
{
    if (LUA_DOFILE(m_pkLuaState, pszFileName) == 0)
    {
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
}

/** Internal function: pszName�� �̸��� ������ ����/table�� LUA stack���� �����´�.
@param pszName [in] ����/table�� �̸�
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure.
*/
HRESULT KLuaManager::Get(const char* pszName)
{
    if (m_iTableDepth > 0 || m_bLuaTinkerHelper )
    {
        lua_pushstring(m_pkLuaState, pszName);
        lua_gettable(m_pkLuaState, -2);
    }
    else
    {
        lua_getglobal(m_pkLuaState, pszName);
    }
    return S_OK;
}

/** Internal function: (table ������) iIndex��° ����/table�� LUA stack���� �����´�.
@param iIndex [in] ����/table�� index (1���� ������)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (���� table �ȿ� ���� ���� ����
���).
*/
HRESULT KLuaManager::Get(int iIndex)
{
    if (m_iTableDepth <= 0 && !m_bLuaTinkerHelper) return E_FAIL;
    lua_pushnumber(m_pkLuaState, iIndex);
    lua_gettable(m_pkLuaState, -2);
    return S_OK;
}

/** pszName�� �̸��� ������ table�� LUA stack���� �����´�.
@param pszName [in] table�� �̸�
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� table�� ���� ���).
*/
HRESULT KLuaManager::BeginTable(const char* pszName, bool bLogOutput)
{
    if (SUCCEEDED(Get(pszName)) && lua_istable(m_pkLuaState, -1) != 0)
    {
        m_iTableDepth++;
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return E_FAIL;
    }
}

/** (table ������) iIndex��° table�� LUA stack���� �����´�.
@param iIndex [in] table�� index (1���� ������)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (���� table �ȿ� ���� ���� ����
���, �ش� table�� ���� ���).
*/
HRESULT KLuaManager::BeginTable(int iIndex, bool bLogOutput)
{
    if (SUCCEEDED(Get(iIndex)) && lua_istable(m_pkLuaState, -1) != 0)
    {
        m_iTableDepth++;
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return E_FAIL;
    }
}

/** LUA stack���� ���� �ֱٿ� ����� table�� �����Ѵ�.
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (table�� ����� ���� ���).
*/
HRESULT KLuaManager::EndTable(bool bLogOutput)
{
    if (m_iTableDepth > 0)
    {
        lua_pop(m_pkLuaState, 1);
        m_iTableDepth--;
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param bValue [out] ���� ������ 0�̸� false, 0�� �ƴϸ� true (���� �ô� ��ȭ
����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, bool& bValue, bool bLogOutput)
{
    if (SUCCEEDED(Get(pszName)) && lua_isnumber(m_pkLuaState, -1) != 0)
    {
        bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
        lua_pop(m_pkLuaState, 1);
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return E_FAIL;
    }
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param bValue [out] ���� ������ 0�̸� false, 0�� �ƴϸ� true (���� �ô� ��ȭ
����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, bool& bValue, bool bLogOutput)
{
    if (SUCCEEDED(Get(iIndex)) && lua_isnumber(m_pkLuaState, -1) != 0)
    {
        bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
        lua_pop(m_pkLuaState, 1);
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return E_FAIL;
    }
}

#define GET_BY_NAME(var,type) \
    if (SUCCEEDED(Get(pszName)) && lua_isnumber(m_pkLuaState, -1) != 0) \
{ \
    var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
    lua_pop(m_pkLuaState, 1); \
    return S_OK; \
} \
    else \
{ \
    lua_pop(m_pkLuaState, 1); \
    return E_FAIL; \
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param sValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, short& sValue, bool bLogOutput)
{
    GET_BY_NAME(sValue, short);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param iValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, int& iValue, bool bLogOutput)
{
    GET_BY_NAME(iValue, int);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param lValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, long& lValue, bool bLogOutput)
{
    GET_BY_NAME(lValue, long);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param usValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput)
{
    GET_BY_NAME(usValue, unsigned short);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param uiValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput)
{
    GET_BY_NAME(uiValue, unsigned int);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param ulValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, unsigned long& ulValue, bool bLogOutput)
{
    GET_BY_NAME(ulValue, unsigned long);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param fValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, float& fValue, bool bLogOutput)
{
    GET_BY_NAME(fValue, float);
}

/** pszName�� �̸��� ������ ���� ������ ���� �����´�.
@param pszName [in] ���� ������ �̸�
@param dValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, double& dValue, bool bLogOutput)
{
    GET_BY_NAME(dValue, double);
}

#define GET_BY_NUM(var,type) \
    if (SUCCEEDED(Get(iIndex)) && lua_isnumber(m_pkLuaState, -1) != 0) \
{ \
    var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
    lua_pop(m_pkLuaState, 1); \
    return S_OK; \
} \
    else \
{ \
    lua_pop(m_pkLuaState, 1); \
    return E_FAIL; \
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param sValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, short& sValue, bool bLogOutput)
{
    GET_BY_NUM(sValue, short);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param iValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, int& iValue, bool bLogOutput)
{
    GET_BY_NUM(iValue, int);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param lValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, long& lValue, bool bLogOutput)
{
    GET_BY_NUM(lValue, long);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param usValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, unsigned short& usValue, bool bLogOutput)
{
    GET_BY_NUM(usValue, unsigned short);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param uiValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput)
{
    GET_BY_NUM(uiValue, unsigned int);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param ulValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput)
{
    GET_BY_NUM(ulValue, unsigned long);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param fValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, float& fValue, bool bLogOutput)
{
    GET_BY_NUM(fValue, float);
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param dValue [out] ���� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, double& dValue, bool bLogOutput)
{
    GET_BY_NUM(dValue, double);
}

/** pszName�� �̸��� ������ ���ڿ� ������ ���� �����´�.
@param pszName [in] ���ڿ� ������ �̸�
@param strValue [out] ���ڿ� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���ڿ� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, std::string& strValue, bool bLogOutput)
{
    if (SUCCEEDED(Get(pszName)) && lua_isstring(m_pkLuaState, -1) != 0)
    {
        strValue = lua_tostring(m_pkLuaState, -1);
        lua_pop(m_pkLuaState, 1);
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return E_FAIL;
    }
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param strValue [out] ���ڿ� ������ �� (���нô� ��ȭ ����)
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���ڿ� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, std::string& strValue, bool bLogOutput)
{
    if (SUCCEEDED(Get(iIndex)) && lua_isstring(m_pkLuaState, -1) != 0)
    {
        strValue = lua_tostring(m_pkLuaState, -1);
        lua_pop(m_pkLuaState, 1);
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return E_FAIL;
    }
}

/** pszName�� �̸��� ������ ���ڿ� ������ ���� �����´�.
@param pszName [in] ���ڿ� ������ �̸�
@param pszValue [out] ���ڿ� ������ ���� ����� �ּ� (���нô� ��ȭ ����)
@param iMaxLen [in] pszValue�� �ִ� ���� ������ ������ ��
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���ڿ� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput)
{
    if (SUCCEEDED(Get(pszName)) && lua_isstring(m_pkLuaState, -1) != 0)
    {
#ifdef _CONVERT_VS_2010
		strncpy_s(pszValue, iMaxLen+1, lua_tostring(m_pkLuaState, -1), iMaxLen);
#else
		strncpy(pszValue, lua_tostring(m_pkLuaState, -1), iMaxLen);
#endif _CONVERT_VS_2010
        lua_pop(m_pkLuaState, 1);
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);        
        return E_FAIL;
    }
}

/** (table ������) iIndex��° ������ ���� �����´�.
@param iIndex [in] ������ index (1���� ������)
@param pszValue [out] ���ڿ� ������ ���� ����� �ּ� (���нô� ��ȭ ����)
@param iMaxLen [in] pszValue�� �ִ� ���� ������ ������ ��
@param bLogOutput [in] false�� �����ϸ� log message�� ������ ����; true��
default��.
@return S_OK for success, E_FAIL for failure (�ش� ������ ���ų�, ���ڿ� ������
�ƴ� ���).
*/
HRESULT KLuaManager::GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput)
{
    if (SUCCEEDED(Get(iIndex)) && lua_isstring(m_pkLuaState, -1) != 0)
    {
#ifdef _CONVERT_VS_2010
		strncpy_s(pszValue, iMaxLen+1, lua_tostring(m_pkLuaState, -1), iMaxLen);
#else
		strncpy(pszValue, lua_tostring(m_pkLuaState, -1), iMaxLen);
#endif _CONVERT_VS_2010
        lua_pop(m_pkLuaState, 1);
        return S_OK;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);        
        return E_FAIL;
    }
}

HRESULT KLuaManager::GetValue(const char* pszName, char& cValue, bool bLogOutput)
{
    GET_BY_NAME( cValue, char );
}

HRESULT KLuaManager::GetValue(int iIndex, char& cValue, bool bLogOutput)
{
    GET_BY_NUM( cValue, char );
}

bool KLuaManager::IsNumber( const char* pszName )
{
    if( Get( pszName ) == E_FAIL )
        return false;

    // Ÿ��üũ
    int iValue = lua_isnumber( m_pkLuaState, -1 );
    lua_pop(m_pkLuaState, 1);

    if( iValue != 0 )
        return true;

    return false;
}

bool KLuaManager::IsNumber( int iIndex )
{
    if( Get( iIndex ) == E_FAIL )
        return false;

    // Ÿ��üũ
    int iValue = lua_isnumber( m_pkLuaState, -1 );
    lua_pop(m_pkLuaState, 1);

    if( iValue != 0 )
        return true;

    return false;
}

bool KLuaManager::IsString( const char* pszName )
{
    if( Get( pszName ) == E_FAIL )
        return false;

    // Ÿ��üũ
    int iValue = lua_isstring( m_pkLuaState, -1 );
    lua_pop(m_pkLuaState, 1);

    if( iValue != 0 )
        return true;

    return false;
}

bool KLuaManager::IsString( int iIndex )
{
    if( Get( iIndex ) == E_FAIL )
        return false;

    // Ÿ��üũ
    int iValue = lua_isstring( m_pkLuaState, -1 );
    lua_pop(m_pkLuaState, 1);

    if( iValue != 0 )
        return true;

    return false;
}
