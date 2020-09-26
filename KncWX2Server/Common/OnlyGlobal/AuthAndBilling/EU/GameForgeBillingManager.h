#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#ifdef SERV_COUNTRY_EU

#include "Event.h"

class KGameForgeBillingManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGameForgeBillingManager );

public:
	KGameForgeBillingManager(void);
	~KGameForgeBillingManager(void);

	void RegToLua();

	void Init( int nThreadNum );

	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
	//	virtual void BeginThread();

	// ���� ������ �ϴ� �� ����. KPerformer Tick�� Ȥ�ó� ȣ��Ǿ� �̺�Ʈ ť �ǵ��̴� ���� ���� ���� �������� //
	// ���� ChannelSimLayer Tick()���� SiKGameForgeAuthManager()->Tick();�ϴ°� �ּ� ó����
	virtual void Tick() { };		


protected:
	std::string m_strWsdlFile;
};

DefSingletonInline( KGameForgeBillingManager );

#endif //SERV_COUNTRY_EU