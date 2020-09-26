#include ".\gameforgebillingmanager.h"

#ifdef SERV_COUNTRY_EU

#include "KncSend.h"
#include "BaseServer.h"
#include "GameForgeBilling.h"


ImplToStringW( KGameForgeBillingManager )
{
	return stm_;
}


ImplPfID( KGameForgeBillingManager, PI_GS_PUBLISHER_BILLING );

ImplementSingleton( KGameForgeBillingManager );

KGameForgeBillingManager::KGameForgeBillingManager(void)
{
	m_strWsdlFile = "";
}

KGameForgeBillingManager::~KGameForgeBillingManager(void)
{
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KGameForgeBillingManager::RegToLua()
{
	lua_tinker::class_add<KGameForgeBillingManager>( g_pLua, "KGameForgeBillingManager" );
	lua_tinker::class_def<KGameForgeBillingManager>( g_pLua, "Init",				 &KGameForgeBillingManager::Init );
	lua_tinker::class_def<KGameForgeBillingManager>( g_pLua, "dump",				 &KGameForgeBillingManager::Dump );
	lua_tinker::class_def<KGameForgeBillingManager>( g_pLua, "AddWebServiceAddress", &KGameForgeBillingManager::AddWebServiceAddress );
	lua_tinker::class_def<KGameForgeBillingManager>( g_pLua, "AddWebMethod",	&KGameForgeBillingManager::AddWebMethod );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KGameForgeBilling::##id );
#   include "GameForgeBilling_def.h"


	lua_tinker::decl( g_pLua, "GFBillingManager", this );
}

void KGameForgeBillingManager::Init( int nThreadNum )
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� CreateThread���� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.
	KThreadManager::Init( nThreadNum );
}

//    KSimLayer::GetKObj()->Init(); �ɶ� BeginThread ȣ���


void KGameForgeBillingManager::AddWebServiceAddress( const char* szWSDL )
{
	m_strWsdlFile = szWSDL;
}

void KGameForgeBillingManager::AddWebMethod( int iSoapAction, const char* szMethod )
{
	KGameForgeBilling::AddWebMethod( iSoapAction, szMethod );
}


KThread* KGameForgeBillingManager::CreateThread()
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� �����ڿ� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.


	return new KGameForgeBilling(m_strWsdlFile.c_str());
}


// void KGameForgeAuthManager::BeginThread()
// {
// 	KThreadManager::BeginThread();        
//}

#endif //SERV_COUNTRY_EU