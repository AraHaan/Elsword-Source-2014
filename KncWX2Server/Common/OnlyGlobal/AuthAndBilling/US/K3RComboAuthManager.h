#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"

#ifdef SERV_COUNTRY_US

class KK3RComboAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KK3RComboAuthManager );

public:
	KK3RComboAuthManager();
	virtual ~KK3RComboAuthManager();

	void RegToLua();

	void Init( int nThreadNum );

	void AddWebServiceAddress( const char* szWSDL );
	void AddWebMethod( int iSoapAction, const char* szMethod );

	// derived from KThreadManager
	virtual KThread* CreateThread();
	//	virtual void BeginThread();

	// ���� ������ �ϴ� �� ����. KPerformer Tick�� Ȥ�ó� ȣ��Ǿ� �̺�Ʈ ť �ǵ��̴� ���� ���� ���� �������� //
	// ���� ChannelSimLayer Tick()���� SiKGameForgeAuthManager()->Tick();�ϴ°� �ּ� ó����
	virtual void Tick();


protected:
	std::string m_strWsdlPath;

};

DefSingletonInline( KK3RComboAuthManager );

#endif // SERV_COUNTRY_US