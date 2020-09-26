/**
	@file	X2EnumManager.h
	@author	Wonpok
	@brief	CX2EnumManager�� ������ ��� ����
		
	@date	JHKang on 2013/02/21,
			- ��� �ľ�
			- ���� �۾�( �ּ�ó��, �� ������, ���� ���� ���� )
			- Ŭ������ ���� ( KX2EnumManager --> CX2EnumManager )
*/

#include "stdafx.h"
#include "X2EnumManager.h"

#pragma region CX2EnumManager
CX2EnumManager::CX2EnumManager(void)
{
	m_mapWorldEnum.clear();
	m_mapNpcEnum.clear();
	m_mapNpcEnumString.clear();
	m_errorStr = L"";
}
#pragma endregion ������

#pragma region ~CX2EnumManager
CX2EnumManager::~CX2EnumManager(void)
{
	m_mapWorldEnum.clear();
	m_mapNpcEnum.clear();
	m_mapNpcEnumString.clear();
	m_errorStr = L"";
}
#pragma endregion �Ҹ���

#pragma region OpenScriptFile
bool CX2EnumManager::OpenScriptFile( const WCHAR* pFileName )
{
	// ���δ��� ���
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pEnumManager", this );

	// ���� �ε�
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR0, strFileName.c_str() );

		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR72, strFileName.c_str() );

		return false;
	}

	return true;
}
#pragma endregion EnumString.lua ������ �Ľ�

#pragma region AddWorldEnumData_LUA
bool CX2EnumManager::AddWorldEnumData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
	
	std::wstring		 wsWorldEnum;
	CX2World::WORLD_ID	 worldID;

	LUA_GET_VALUE_ENUM(	luaManager, L"ENUM_ID",			worldID,		CX2World::WORLD_ID,	CX2World::WI_NONE );
	LUA_GET_VALUE(		luaManager, L"ENUM_STRING",		wsWorldEnum,	L"" );
	
	m_mapWorldEnum.insert( std::make_pair( worldID, wsWorldEnum ) );
	return true;
}
#pragma endregion World Enum Data �߰�

#pragma region AddNpcEnumData_LUA
bool CX2EnumManager::AddNpcEnumData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	std::wstring				 wsNpcEnum;
	CX2UnitManager::NPC_UNIT_ID	 npcID;

	LUA_GET_VALUE_ENUM(	luaManager, L"ENUM_ID",			npcID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE );
	LUA_GET_VALUE(		luaManager, L"ENUM_STRING",		wsNpcEnum,	L"" );

	if( npcID >= CX2UnitManager::NUI_NPC_END )
		return true;

	m_mapNpcEnum.insert( std::make_pair( npcID, wsNpcEnum ) );
	m_mapNpcEnumString.insert( std::make_pair( wsNpcEnum, npcID ) );
	
	return true;
}
#pragma endregion NPC Enum Data �߰�

#pragma region GetWorldEnumString
std::wstring& CX2EnumManager::GetWorldEnumString( CX2World::WORLD_ID worldID )
{
	WCHAR buf[128] = L"";
	m_iterWorldEnum mit = m_mapWorldEnum.find( worldID );
	if( mit == m_mapWorldEnum.end() )
	{
		wsprintf( buf, L"WOLRD_ID[%d]�� ���ڿ��� ��ϵ��� �ʾ���", (int)worldID );
		return m_errorStr = std::wstring( buf );
	}

	return mit->second;
}
#pragma endregion World ID�� World�� string ��ȯ, ������ ���� ���

#pragma region GetNpcEnumString
std::wstring& CX2EnumManager::GetNpcEnumString( CX2UnitManager::NPC_UNIT_ID npcID )
{
	WCHAR buf[128] = L"";
	m_iterNpcEnum mit = m_mapNpcEnum.find( npcID );
	
	if( mit == m_mapNpcEnum.end() )
	{		
		wsprintf( buf, L"NPC_UNIT_ID[%d]�� ���ڿ��� ��ϵ��� �ʾ���", (int)npcID );
		MessageBox(NULL, buf, L"Error!", MB_OK);
		
		return m_errorStr = std::wstring( buf );
	}

	return mit->second;
}
#pragma endregion NPC UID�� NPC�� string ��ȯ, ������ ���� ���

#pragma region GetNpcEnum
CX2UnitManager::NPC_UNIT_ID CX2EnumManager::GetNpcEnum(wstring wstrNpcEnumString)
{
	std::map< std::wstring, CX2UnitManager::NPC_UNIT_ID >::iterator mit = m_mapNpcEnumString.find(wstrNpcEnumString);
	if( mit == m_mapNpcEnumString.end() )
	{
		return CX2UnitManager::NUI_NONE; 
	}

	return mit->second;
}
#pragma endregion NPC string���� NPC ID ��ȯ, ������ NUI_NONE ��ȯ