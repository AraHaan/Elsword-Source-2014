#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"

#ifdef SERV_COUNTRY_BR

class KLevelUpAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KLevelUpAuthManager );

public:
	KLevelUpAuthManager();
	virtual ~KLevelUpAuthManager();

	void RegToLua();
    
	void Init( int nThreadNum );
	
	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
	
	// ���� ������ �ϴ� �� ����. KPerformer Tick�� Ȥ�ó� ȣ��Ǿ� �̺�Ʈ ť �ǵ��̴� ���� ���� ���� �������� //
	// ���� ChannelSimLayer Tick()���� SiKGameForgeAuthManager()->Tick();�ϴ°� �ּ� ó����
	virtual void Tick() { };		

protected:
	std::string m_strWsdlPath;
	
};

DefSingletonInline( KLevelUpAuthManager );

#endif // SERV_COUNTRY_BR

