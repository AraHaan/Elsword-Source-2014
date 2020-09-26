#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;


//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT

#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" ��ũ��Ʈ ������ �Ϸ��Ͽ����ϴ�!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT


class KChannelScriptRefreshManager : public KThreadManager
{
	DeclareSingleton( KChannelScriptRefreshManager );
	DeclPfID;

public:
	KChannelScriptRefreshManager();
	virtual ~KChannelScriptRefreshManager();

	void RegToLua( lua_State* pLuaState );

	// ��ũ��Ʈ ���� ��û lua�Լ�
	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	void SmsManager_Lua( void );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	void LogManager_Lua();
#endif SERV_LOG_SYSTEM_NEW
	//}}

	// derived from KThreadManager
	virtual void Init();
	virtual KThread* CreateThread();

	lua_State* GetRefreshLuaState() { return m_pRefreshLuaState; }

protected:
	lua_State* m_pRefreshLuaState;
};

DefSingletonInline( KChannelScriptRefreshManager );


#endif SERV_REALTIME_SCRIPT
//}}

#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}