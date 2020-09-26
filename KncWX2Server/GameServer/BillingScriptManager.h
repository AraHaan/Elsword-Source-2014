#pragma once
#include <map>
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"


//{{ 2011. 12. 07	������	���� ��ũ��Ʈ �Ŵ���
//#ifdef SERV_BILLING_SCRIPT_MANAGER


class KBillingScriptManager
{
	DeclToStringW;
	DeclDump;
public:
	struct KCategoryInfo
	{
        int			m_iServiceCategory;
		int			m_iTestServCategory;

		KCategoryInfo()
		{
			m_iServiceCategory = 0;
			m_iTestServCategory = 0;
		}
	};

public:
	KBillingScriptManager(void);
	~KBillingScriptManager(void);

	// for lua
	bool OpenScriptFile( IN const char* pszFileName );
	bool AddProductCategoryModifyForItemID_LUA( int iItemID, int iServiceCategoryNo, int iTestServCategoryNo );

	// function
	bool IsExistProductCategoryModifyInfoForItemID( IN const int iItemID ) const		{ return ( m_mapProductCategoryModifyForItemID.find( iItemID ) != m_mapProductCategoryModifyForItemID.end() ); }

	bool GetModifiedProductCategoryForItemID( IN const int iItemID, IN const bool bService, OUT unsigned long& ulModifiedCategoryNo ) const;
	
private:
	std::map< int, KCategoryInfo >		m_mapProductCategoryModifyForItemID; // Ư�� ��ǰ�� ī�װ� ���� ���� ����
};


//#endif SERV_BILLING_SCRIPT_MANAGER
//}}