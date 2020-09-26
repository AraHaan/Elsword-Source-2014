#include "LevelUpAuthManager.h"

#ifdef SERV_COUNTRY_BR

#include "KncSend.h"
#include "BaseServer.h"
#include "LevelUpAuth.h"


ImplToStringW( KLevelUpAuthManager )
{
	return stm_;
}

ImplPfID( KLevelUpAuthManager, PI_NULL );

ImplementSingleton( KLevelUpAuthManager );

KLevelUpAuthManager::KLevelUpAuthManager()
{
	m_strWsdlPath = "";
}

KLevelUpAuthManager::~KLevelUpAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
#include "BaseServer.h"

void KLevelUpAuthManager::RegToLua()
{
	lua_tinker::class_add<KLevelUpAuthManager>( g_pLua, "KLevelUpAuthManager" );
	lua_tinker::class_def<KLevelUpAuthManager>( g_pLua, "Init",				 &KLevelUpAuthManager::Init );
	lua_tinker::class_def<KLevelUpAuthManager>( g_pLua, "dump",				 &KLevelUpAuthManager::Dump );
	lua_tinker::class_def<KLevelUpAuthManager>( g_pLua, "AddWebServiceAddress", &KLevelUpAuthManager::AddWebServiceAddress );
	lua_tinker::class_def<KLevelUpAuthManager>( g_pLua, "AddWebMethod",	&KLevelUpAuthManager::AddWebMethod );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KLevelUpAuth::##id );
#   include "LevelUpAuth_def.h"

	
	lua_tinker::decl( g_pLua, "LevelUpAuthManager", this );
}

void KLevelUpAuthManager::Init( int nThreadNum )
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� CreateThread���� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.
	KThreadManager::Init( nThreadNum );
}

//    KSimLayer::GetKObj()->Init(); �ɶ� BeginThread ȣ���


void KLevelUpAuthManager::AddWebServiceAddress( const char* szWSDL )
{
	m_strWsdlPath = szWSDL;
}

void KLevelUpAuthManager::AddWebMethod( int iSoapAction, const char* szMethod )
{
	KLevelUpAuth::AddWebMethod( iSoapAction, szMethod );
}


KThread* KLevelUpAuthManager::CreateThread()
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� �����ڿ� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.
	
	return new KLevelUpAuth(m_strWsdlPath.c_str());
}

#endif // SERV_COUNTRY_BR
