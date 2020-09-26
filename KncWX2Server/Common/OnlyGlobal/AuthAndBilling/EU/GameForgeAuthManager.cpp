#include "gameforgeauthmanager.h"

#ifdef SERV_COUNTRY_EU

#include "KncSend.h"
#include "BaseServer.h"
#include "GameForgeAuth.h"


ImplToStringW( KGameForgeAuthManager )
{
	return stm_;
}

//ImplPfID( KGameForgeAuthManager, PI_CHANNEL_GF_AUTH );
ImplPfID( KGameForgeAuthManager, PI_NULL );

ImplementSingleton( KGameForgeAuthManager );

KGameForgeAuthManager::KGameForgeAuthManager()
{
	m_strWsdlPath = "";
}

KGameForgeAuthManager::~KGameForgeAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
//using namespace lua_tinker;
#include "BaseServer.h"

void KGameForgeAuthManager::RegToLua()
{
	lua_tinker::class_add<KGameForgeAuthManager>( g_pLua, "KGameForgeAuthManager" );
	lua_tinker::class_def<KGameForgeAuthManager>( g_pLua, "Init",				 &KGameForgeAuthManager::Init );
	lua_tinker::class_def<KGameForgeAuthManager>( g_pLua, "dump",				 &KGameForgeAuthManager::Dump );
	lua_tinker::class_def<KGameForgeAuthManager>( g_pLua, "AddWebServiceAddress", &KGameForgeAuthManager::AddWebServiceAddress );
	lua_tinker::class_def<KGameForgeAuthManager>( g_pLua, "AddWebMethod",	&KGameForgeAuthManager::AddWebMethod );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KGameForgeAuth::##id );
#   include "GameForgeAuth_def.h"

	
	lua_tinker::decl( g_pLua, "GFAuthManager", this );
}

void KGameForgeAuthManager::Init( int nThreadNum )
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� CreateThread���� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.
	KThreadManager::Init( nThreadNum );
}

//    KSimLayer::GetKObj()->Init(); �ɶ� BeginThread ȣ���


void KGameForgeAuthManager::AddWebServiceAddress( const char* szWSDL )
{
	m_strWsdlPath = szWSDL;
}

void KGameForgeAuthManager::AddWebMethod( int iSoapAction, const char* szMethod )
{
	KGameForgeAuth::AddWebMethod( iSoapAction, szMethod );
}


KThread* KGameForgeAuthManager::CreateThread()
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� �����ڿ� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.
	

	return new KGameForgeAuth(m_strWsdlPath.c_str());
}


// void KGameForgeAuthManager::BeginThread()
// {
// 	KThreadManager::BeginThread();        
//}


//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
void KGameForgeAuthManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

void KGameForgeAuthManager::SendSoapLoginFuncLog(std::wstring wstrUserID, CTime StartFuncTime, DWORD dwDiffTime, int iCase)
{

	std::wstring strTmpStartFuncTion = (std::wstring)StartFuncTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );


	KDBE_SOAP_LOGIN_FUNC_LOG kPacketReq;
	kPacketReq.m_wstrUserID = wstrUserID;
	kPacketReq.m_wstrStartFuncTime = strTmpStartFuncTion;
	kPacketReq.m_dwDiffTime = dwDiffTime;
	kPacketReq.m_iCase = iCase;
	kPacketReq.m_iQueueSize = GetQueueSize();


	SendToLogDB( DBE_SOAP_LOGIN_FUNC_LOG, kPacketReq );
	
}
#endif SERV_SOAP_LOGIN_FUNC_LOG
//}}

#endif // SERV_COUNTRY_EU
