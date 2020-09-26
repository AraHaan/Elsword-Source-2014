#include "ProxyManager.h"
#include "ChannelScriptRefreshThread.h"
#include "ChannelScriptRefreshManager.h"
#include "KncSend.h"
#include "KAutoPath.h"


//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT

//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

ImplPfID( KChannelScriptRefreshManager, PI_NULL );

ImplementSingleton( KChannelScriptRefreshManager );

KChannelScriptRefreshManager::KChannelScriptRefreshManager() :
m_pRefreshLuaState( NULL )
{
}

KChannelScriptRefreshManager::~KChannelScriptRefreshManager()
{
	if( m_pRefreshLuaState != NULL )
	{
		lua_close( m_pRefreshLuaState );
		//{{ 2010. 8. 30	������	���� ���� ���� ó��		
		m_pRefreshLuaState = NULL;
		//}}
	}
}

void KChannelScriptRefreshManager::RegToLua( lua_State* pLuaState )
{
	lua_tinker::class_add<KChannelScriptRefreshManager>( pLuaState, "KChannelScriptRefreshManager" );

	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	lua_tinker::class_def<KChannelScriptRefreshManager>( pLuaState, "SmsManager",	&KChannelScriptRefreshManager::SmsManager_Lua );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	lua_tinker::class_def<KChannelScriptRefreshManager>( pLuaState, "LogManager",	&KChannelScriptRefreshManager::LogManager_Lua );
#endif SERV_LOG_SYSTEM_NEW
	//}}

	lua_tinker::decl( pLuaState, "ScriptRefresh", this );
}

void KChannelScriptRefreshManager::Init()
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
			START_LOG( cerr, L"GSScriptRefreshManager : Enum ���� �ε� ����.!" );
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
			START_LOG( cerr, L"GSScriptRefreshManager : DungeonEnum ���� �ε� ����.!" );
			return;
		}
	}

	// ������� ������ 1���� ����!
	KThreadManager::Init( 1 );

	// ������ ����!
	KThreadManager::BeginThread();

	START_LOG( cout, L"��ũ��Ʈ �������� ������ ����!" );
}

KThread* KChannelScriptRefreshManager::CreateThread()
{
	return new KChannelScriptRefreshThread;
}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
void KChannelScriptRefreshManager::SmsManager_Lua( void )
{
	START_LOG( cout, L"KSMSPhoneNumberManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");

	// ä�� ������ SMSPhoneNumberManager ���� ��û!
	KEventPtr spEvent( new KEvent );
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CH_SMS_MANAGER;

	spEvent->SetData( PI_CHANNEL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kNot );
	SiKChannelScriptRefreshManager()->QueueingEvent( spEvent );
}
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
void KChannelScriptRefreshManager::LogManager_Lua()
{
	START_LOG( cout, L"KLogManager Ŭ���� ��ũ��Ʈ �ǽð� ��ġ!");

	// ä�� ������ SMSPhoneNumberManager ���� ��û!
	KEventPtr spEvent( new KEvent );
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CH_LOG_MANAGER;

	spEvent->SetData( PI_CHANNEL_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kNot );
	SiKChannelScriptRefreshManager()->QueueingEvent( spEvent );
}
#endif SERV_LOG_SYSTEM_NEW
//}}

#endif SERV_REALTIME_SCRIPT
//}}

#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}
