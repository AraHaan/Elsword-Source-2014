#include "CnScriptRefreshThread.h"
#include "CnScriptRefreshManager.h"
#include "KncSend.h"
#include "KAutoPath.h"


//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

ImplPfID( KCnScriptRefreshManager, PI_NULL );

ImplementSingleton( KCnScriptRefreshManager );

KCnScriptRefreshManager::KCnScriptRefreshManager() :
m_pRefreshLuaState( NULL )
{
}

KCnScriptRefreshManager::~KCnScriptRefreshManager()
{
	if( m_pRefreshLuaState != NULL )
	{
		lua_close( m_pRefreshLuaState );
		//{{ 2010. 8. 30	������	���� ���� ���� ó��		
		m_pRefreshLuaState = NULL;
		//}}
	}
}

void KCnScriptRefreshManager::Init()
{
	// ��ũ��Ʈ ���� ���� lua_state ����!
	m_pRefreshLuaState = lua_open();

	_JIF( m_pRefreshLuaState != NULL, return );

	luaL_openlibs( m_pRefreshLuaState );
	lua_settop( m_pRefreshLuaState, 0 );

	{
		// enum.lua �Ľ�
		std::string strFile = "Enum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"CnScriptRefreshManager : Enum ���� �ε� ����.!" );
			return;
		}
	}

	{
		// enum.lua �Ľ�
		std::string strFile = "DungeonEnum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"CnScriptRefreshManager : DungeonEnum ���� �ε� ����.!" );
			return;
		}
	}

	// ������� ������ 1���� ����!
	KThreadManager::Init( 1 );

	// ������ ����!
	KThreadManager::BeginThread();

	START_LOG( cout, L"��ũ��Ʈ �������� ������ ����!" );
}

KThread* KCnScriptRefreshManager::CreateThread()
{
	return new KCnScriptRefreshThread;
}

#endif SERV_REALTIME_SCRIPT
//}}


