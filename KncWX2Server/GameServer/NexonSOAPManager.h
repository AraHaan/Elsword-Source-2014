

#pragma once

#include <KNCSingleton.h>
#include <Thread/KCSLockType.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP

class KNexonSOAPManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KNexonSOAPManager );

public:
	KNexonSOAPManager();
	virtual ~KNexonSOAPManager();

	void RegToLua();

	void Init( IN int nThreadNum
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
			 , IN char* szSoapServerAddressForUser
			 , IN char* szSoapServerAddressForCoupon
#endif // SERV_NEXON_COUPON_SYSTEM
			 );

	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
	//	virtual void BeginThread();

	// ���� ������ �ϴ� �� ����. KPerformer Tick�� Ȥ�ó� ȣ��Ǿ� �̺�Ʈ ť �ǵ��̴� ���� ���� ���� �������� //
	// ���� ChannelSimLayer Tick()���� SiKGameForgeAuthManager()->Tick();�ϴ°� �ּ� ó����
	virtual void Tick() { };		


protected:
	std::string m_strWsdlPath;

};

DefSingletonInline( KNexonSOAPManager );

#endif SERV_NEXON_AUTH_SOAP
//}}
