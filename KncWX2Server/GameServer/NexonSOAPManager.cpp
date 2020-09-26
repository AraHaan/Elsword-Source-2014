#include <WinSock2.h>
#include "NexonSOAPManager.h"
#include "NexonSOAP.h"
#include "BaseServer.h"

//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP

ImplToStringW( KNexonSOAPManager )
{
	return stm_;
}

ImplPfID( KNexonSOAPManager, PI_GS_NX_SOAP );

ImplementSingleton( KNexonSOAPManager );

KNexonSOAPManager::KNexonSOAPManager()
{
	m_strWsdlPath = "";
}

KNexonSOAPManager::~KNexonSOAPManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>

void KNexonSOAPManager::RegToLua()
{
	lua_tinker::class_add<KNexonSOAPManager>( g_pLua, "KNexonAuthManager" );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "Init",				 &KNexonSOAPManager::Init );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "dump",				 &KNexonSOAPManager::Dump );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "AddWebServiceAddress", &KNexonSOAPManager::AddWebServiceAddress );
	lua_tinker::class_def<KNexonSOAPManager>( g_pLua, "AddWebMethod",	&KNexonSOAPManager::AddWebMethod );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KNexonSOAP::##id );
#   include "NexonSOAP_def.h"


	lua_tinker::decl( g_pLua, "NexonSOAP", this );
}

void KNexonSOAPManager::Init( IN int nThreadNum
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
							, IN char* szSoapServerAddressForUser
							, IN char* szSoapServerAddressForCoupon
#endif // SERV_NEXON_COUPON_SYSTEM
							)
{
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
	KNexonSOAP::SetSoapServerAddressForUser( std::string( szSoapServerAddressForUser ) );
	KNexonSOAP::SetSoapServerAddressForCoupon( std::string( szSoapServerAddressForCoupon ) );
#endif // SERV_NEXON_COUPON_SYSTEM

	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� CreateThread���� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.
	KThreadManager::Init( nThreadNum );
}

//    KSimLayer::GetKObj()->Init(); �ɶ� BeginThread ȣ���


void KNexonSOAPManager::AddWebServiceAddress( const char* szWSDL )
{
	m_strWsdlPath = szWSDL;

	START_LOG( cout, L"Nexon SOAP �ּ� ���!" )
		<< BUILD_LOG( m_strWsdlPath );
}

void KNexonSOAPManager::AddWebMethod( int iSoapAction, const char* szMethod )
{
	KNexonSOAP::AddWebMethod( iSoapAction, szMethod );
}


KThread* KNexonSOAPManager::CreateThread()
{
	// Init ȣ�� ������ �׻� AddWebServiceAddress �����Ŀ��� �� //
	// �� ������ �ٷ� �Ʒ� �����ڿ� sdlFile�� smlFile�� �ԷµǾ�� �ϱ� �����̴�.


	return new KNexonSOAP(m_strWsdlPath.c_str());
}

#endif SERV_NEXON_AUTH_SOAP
//}}
