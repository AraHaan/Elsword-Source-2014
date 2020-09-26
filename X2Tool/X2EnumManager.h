#pragma once

/**
	@file	X2EnumManager.h
	@author	Wonpok
	@brief	CX2EnumManager�� ������ ��� ����
		
	@date	JHKang on 2013/02/21,
			- ��� �ľ�
			- ���� �۾�( �ּ�ó��, �� ������, ���� ���� ���� )
			- Ŭ������ ���� ( KX2EnumManager --> CX2EnumManager )
*/

#pragma region CX2EnumManager
/** @class : CX2EnumManager
	@brief : World, NPC�� EnumStirng�� �Ľ�, ������ ���� ��� ���
	@date  : 2013/02/21
*/
class CX2EnumManager
{
	#pragma region public
public:
	CX2EnumManager(void);
	virtual ~CX2EnumManager(void);

	bool OpenScriptFile( const WCHAR* pFileName );
	bool AddWorldEnumData_LUA();
	bool AddNpcEnumData_LUA();

	std::wstring& GetWorldEnumString( CX2World::WORLD_ID worldID );
	std::wstring& GetNpcEnumString( CX2UnitManager::NPC_UNIT_ID npcID );
	CX2UnitManager::NPC_UNIT_ID GetNpcEnum( wstring wstrNpcEnumString );
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > GetMapNpcEnum() { return m_mapNpcEnum; }

	#pragma endregion function

	#pragma region private
private:
	std::map< CX2World::WORLD_ID, std::wstring >			m_mapWorldEnum;			/// World map ( Key : WorldID, Value : string ) 
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >	m_mapNpcEnum;			/// NPC map ( Key : NPC UID, Value : string )
	std::map< std::wstring, CX2UnitManager::NPC_UNIT_ID >	m_mapNpcEnumString;		/// NPC map ( Key : string, Value : NPC_UID )
	std::wstring m_errorStr;

	typedef std::map< CX2World::WORLD_ID, std::wstring >::iterator m_iterWorldEnum;			/// World map �ݺ���
	typedef std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring >::iterator m_iterNpcEnum;	/// NPC map �ݺ���

	#pragma endregion function
};
#pragma endregion 