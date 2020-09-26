#include "CorruptionMonitor.h"
#include "BaseServer.h"

//{{ 2012. 10. 15	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


ImplementRefreshSingleton( KCorruptionMonitor );

KCorruptionMonitor::KCorruptionMonitor(void)
{
}

KCorruptionMonitor::~KCorruptionMonitor(void)
{
}

ImplToStringW( KCorruptionMonitor )
{
	stm_	<< L"----------[ KCorruptionMonitor ]----------" << std::endl
			<< TOSTRINGW( m_bMornitoringOn )
			<< TOSTRINGW( m_mapEDMornitoring.size() )
			<< TOSTRINGW( m_mapItemMornitoring.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KCorruptionMonitor )
{
	lua_tinker::class_add<KCorruptionMonitor>( GetLuaState(), "KCorruptionMonitor" );

	lua_tinker::class_def<KCorruptionMonitor>( GetLuaState(), "SetMornitoringOn",		&KCorruptionMonitor::SetMornitoringOn_LUA );
	lua_tinker::class_def<KCorruptionMonitor>( GetLuaState(), "dump",					&KCorruptionMonitor::Dump );
	
	lua_tinker::decl( GetLuaState(), "g_pKCorruptionMonitor", this );
}

void KCorruptionMonitor::SetMornitoringOn_LUA( bool bVal )
{
	m_bMornitoringOn = bVal;

	if( m_bMornitoringOn )
	{
		START_LOG( cout, L"ED&������ ����͸� On" )
			<< BUILD_LOG( m_bMornitoringOn );
	}
	else
	{
		START_LOG( cout, L"ED&������ ����͸� Off" )
			<< BUILD_LOG( m_bMornitoringOn );
	}
}

void KCorruptionMonitor::Init()
{
	// �ʱ�ȭ ������ ��������!
    SendToLogDB( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_REQ );
}

void KCorruptionMonitor::Update( IN const KDBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK& kInfo )
{
    
}

void KCorruptionMonitor::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}



