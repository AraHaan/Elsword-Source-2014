#include "LoginScriptRefreshThread.h"
#include "LoginScriptRefreshManager.h"
#include "KncSend.h"
#include "KAutoPath.h"


//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

ImplPfID( KLoginScriptRefreshManager, PI_NULL );

ImplementSingleton( KLoginScriptRefreshManager );

KLoginScriptRefreshManager::KLoginScriptRefreshManager() :
m_pRefreshLuaState( NULL )
{
}

KLoginScriptRefreshManager::~KLoginScriptRefreshManager()
{
	if( m_pRefreshLuaState != NULL )
	{
		lua_close( m_pRefreshLuaState );
	}
}

void KLoginScriptRefreshManager::Init()
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
			START_LOG( cerr, L"LoginScriptRefreshManager : Enum ���� �ε� ����.!" );
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
			START_LOG( cerr, L"LoginScriptRefreshManager : DungeonEnum ���� �ε� ����.!" );
			return;
		}
	}

	// ������� ������ 1���� ����!
	KThreadManager::Init( 1 );

	// ������ ����!
	KThreadManager::BeginThread();

	START_LOG( cout, L"��ũ��Ʈ �������� ������ ����!" );
}

KThread* KLoginScriptRefreshManager::CreateThread()
{
	return new KLoginScriptRefreshThread;
}

#endif SERV_REALTIME_SCRIPT
//}}


