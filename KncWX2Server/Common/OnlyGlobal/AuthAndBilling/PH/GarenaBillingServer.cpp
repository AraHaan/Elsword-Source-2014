#include "GarenaBillingServer.h"
#include "GarenaBillingIOCP.h"
//#include "Socket/IOCP.h"
#include "Socket/NetCommon.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include "lua/KLuaManager.h"
#include "BaseServer.h"
#include "GarenaBillingWorkerThread.h"
//#include "GarenaBillingMiddleware.h"


//{{ 2013. 04. 08	������	�ؽ� ���� ��� �̵����
#ifdef SERV_COUNTRY_PH


KThread* KGarenaBillingThreadManager::CreateThread()
{
	return new KGarenaBillingWorkerThread;
}

ImplPfID( KGarenaBillingThreadManager, PI_NULL );


ImplementSingleton( KGarenaBillingServer );

KGarenaBillingServer::KGarenaBillingServer()
{
	m_kMaxTimeInfo.m_dwTerm	= 0;
	m_kMaxTimeInfo.m_cTime	= CTime::GetCurrentTime();
	m_iWorkerThreadCount	= 1; // default
}

KGarenaBillingServer::~KGarenaBillingServer()
{
}

ImplToStringW( KGarenaBillingServer )
{
	return stm_;
}

void KGarenaBillingServer::RegToLua()
{
	lua_tinker::class_add<KGarenaBillingServer>( g_pLua, "KGarenaBillingServer" );
	lua_tinker::class_def<KGarenaBillingServer>( g_pLua, "WorkerThreadCount",	&KGarenaBillingServer::SetWorkerThreadCount );
	lua_tinker::class_def<KGarenaBillingServer>( g_pLua, "dump",				&KGarenaBillingServer::Dump );

	lua_tinker::decl( g_pLua, "KGarenaBillingServer", this );
}

bool KGarenaBillingServer::Init()
{
	m_kAccepter.Init( 9900, KGarenaBillingServer::OnAccept, false );	// Nagle off

	//�����ڵ� �ʱ�ȭ�� NetLayer���� �ϱ⶧���� ���� ����.

	//TCP IOCP START.

	//{{ 2009. 3. 13  ������	�׳� Iocp�� ����	
	SiKGarenaBillingIocp()->Init( 4 );
	SiKGarenaBillingIocp()->BeginThread();
	START_LOG( cout2, L"=== GarenaBilling IOCP START..! ===" );
	//}}

    //TCP ACCEPTER START.
	if( m_kAccepter.Begin() == false )
	{
		START_LOG( cerr, L"=== GarenaBilling ACCEPTER START FAILED.! ===" )
			<< END_LOG;

		return false;
	}
	else
	{
		START_LOG( cout2, L"=== GarenaBilling ACCEPTER START..! ===" );
	}

	m_kThreadManager.Init( m_iWorkerThreadCount );
	m_kThreadManager.BeginThread();

	START_LOG( cout2, L"=== GarenaBilling WORKER THREAD CREATED.! ===" )
		<< BUILD_LOG( m_iWorkerThreadCount );

	return true;
}

void KGarenaBillingServer::ShutDown()
{
	// accepter down
	m_kAccepter.End();	//������� ���ο� ������ ����.
	
	// iocp down
	//{{ 2009. 3. 13  ������	�׳� iocp����
	SiKGarenaBillingIocp()->EndThread();
	Sleep(100);
	KGarenaBillingIocp::ReleaseInstance();
	//}}

	m_kThreadManager.EndThread();
}

void KGarenaBillingServer::QueueingEvent( const KEventPtr& spEvent_ )
{
	m_kThreadManager.QueueingEvent( spEvent_ );
}

void KGarenaBillingServer::OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	SiKGarenaBillingServer()->OnAccept_( sock, sinRemote );
}

void KGarenaBillingServer::OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	KGarenaBillingUserPtr spTRUser = m_kGarenaBillingUserManager.CreateGarenaBillingUser();
	if( !spTRUser )
	{
		START_LOG( cerr, L"Create GarenaBillingUser Failed" );
		CLOSE_SOCKET( sock );
		return;
	}

    KSocketObjectPtr spSockObj = spTRUser->GetSkGarenaBillingUserPtr();
    JIF( spSockObj );

	spSockObj->SetSocketInfo( sock, sinRemote );

	//{{ 2009. 3. 13  ������	�׳� iocp����
	LIF( SiKGarenaBillingIocp()->AssociateSocket( spSockObj ) );
	//LIF( SiKIocp()->AssociateSocket( spSockObj ) );
	//}}

	// ����Ʈ�� �ٷ� �� ���!
	spSockObj->SetUID( spSockObj->GetKey() );
	m_kGarenaBillingUserManager.Insert( spTRUser );

	spSockObj->InitRecv();
	spSockObj->OnAcceptConnection();

	DWORD dwElapTime = ::GetTickCount();

	//�Ŵ����� ��������� iocp�� ���� uid ����� ������ ����.
	//KTRUser::OOnRecvCompleted() ��Ŷ�� ���� reg���·� ������ Tick()���� ���.
	//KActorManager::GetKObj()->ReserveAdd( spActor );

	dwElapTime = ::GetTickCount() - dwElapTime;

	{   // locking scope
		KLocker lock( m_kMaxTimeInfo.m_cs );

		if( dwElapTime > m_kMaxTimeInfo.m_dwTerm )
		{
			m_kMaxTimeInfo.m_strIP    = KncUtil::toWideString(inet_ntoa(sinRemote.sin_addr));
			m_kMaxTimeInfo.m_cTime    = CTime::GetCurrentTime();
			m_kMaxTimeInfo.m_dwTerm   = dwElapTime;

			START_LOG( clog2, L"accepter ��ü ���� ���ð� ����. Remote IP : " << m_kMaxTimeInfo.m_strIP )
				<< BUILD_LOG( m_kMaxTimeInfo.m_dwTerm );
		}
	}   // locking scope
}

void KGarenaBillingServer::Tick()
{
	m_kGarenaBillingUserManager.Tick();
}


void KGarenaBillingServer::SetmapChangeGameCurrencyFailed( KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPacket )
{

	KLocker lock( m_csGameCurrencyFailed );

	std::map<std::string, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK>::iterator mit = m_mapChangeGameCurrencyFailed.find( kPacket.m_strGarenaTransactionID );

	if (mit == m_mapChangeGameCurrencyFailed.end() )
	{
		m_mapChangeGameCurrencyFailed.insert(std::make_pair(kPacket.m_strGarenaTransactionID, kPacket));
	}
}


void KGarenaBillingServer::GetmapChangeGameCurrencyFailed( IN KEJSON_GN_TRANSACTION_REQ kPacket, OUT KEJSON_GN_TRANSACTION_ACK& kPacketAck )
{

	KLocker lock( m_csGameCurrencyFailed );

	std::map<std::string, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK>::iterator mit = m_mapChangeGameCurrencyFailed.find( kPacket.m_strGarenaTransactionID );

	if (mit != m_mapChangeGameCurrencyFailed.end() )
	{
		if ( mit->second.m_uiGarenaUID == kPacket.m_uiGarenaUID)
		{
			kPacketAck.m_iResult					= 0;
			kPacketAck.m_strDateTime				= mit->second.m_strDateTime;
			kPacketAck.m_int64GameTransactionID		= mit->second.m_int64GameTransactionID;
			kPacketAck.m_strGarenaTransactionID		= mit->second.m_strGarenaTransactionID;
			kPacketAck.m_uiGameCurrency_Amount		= mit->second.m_uiGameCurrency_BeforeAmount;
			kPacketAck.m_uiShell_Amount				= mit->second.m_uiShell_Amount;
			kPacketAck.m_uiGarenaUID				= mit->second.m_uiGarenaUID;

		}
		else
		{
			kPacketAck.m_iResult					= 1;
		}

	}
	else
	{
		kPacketAck.m_iResult					= 99;
	}

	if (kPacketAck.m_iResult != 0)
	{
		kPacketAck.m_strDateTime				= "";
		kPacketAck.m_int64GameTransactionID	= 0LL;
		kPacketAck.m_strGarenaTransactionID	= "";
		kPacketAck.m_uiGameCurrency_Amount		= 0;
		kPacketAck.m_uiShell_Amount			= 0;
		kPacketAck.m_uiGarenaUID				= kPacket.m_uiGarenaUID;
	}
	else
	{
		m_mapChangeGameCurrencyFailed.erase(kPacket.m_strGarenaTransactionID);
	}
}


bool KGarenaBillingServer::GetCheckUserInfoByPublisherUID( IN unsigned int uiPublisherUID, OUT KUserList::KGSUserInfo& kInfo )
{

	KLocker lock( m_csCheckUserInfoByPublisherUID );

	if ( false ==  GetKLoginSimLayer()->GetGSUserInfoByPublisherUID( uiPublisherUID, kInfo ) )
	{
		return false;
	}

	return true;
}

#endif SERV_COUNTRY_PH
//}}