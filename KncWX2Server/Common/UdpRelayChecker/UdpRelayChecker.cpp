#include <winsock2.h>
#include "UdpRelayChecker.h"
#include <dbg/dbg.hpp>
#include "BaseServer.h"
#include "NetError.h"
#include "socket/NetCommon.h"
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "KAutoPath.h"



//{{ 2012. 04. 23	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER

#define CLASS_TYPE  KUdpRelayChecker
ImplPfID( KUdpRelayChecker, PI_NULL );
ImplInstanceKeeping( KUdpRelayChecker );


KUdpRelayChecker::KUdpRelayChecker(void) :
m_bCheckerOn( false ),
m_bPrintLog( false ),
m_sUdpRelayCheckerPort( 0 ),
m_fRelayCheckPacketGap( 5.0 ), // ����Ʈ�� 5�ʰ���
m_fDBUpdateGap( 60.0 * 5 ) // ����Ʈ�� 5�а���
{
}

KUdpRelayChecker::~KUdpRelayChecker(void)
{
}

ImplToStringW( KUdpRelayChecker )
{
	stm_	<< L"----------[ Udp Relay Checker ]----------" << std::endl
		<< TOSTRINGW( m_bCheckerOn )
		<< TOSTRINGW( m_bPrintLog )
		<< TOSTRINGW( m_sUdpRelayCheckerPort )
		<< TOSTRINGW( m_fRelayCheckPacketGap )
		<< TOSTRINGW( m_fDBUpdateGap )
		<< TOSTRINGW( m_mapUdpRelayServerInfo.size() )
		;

	return stm_;
}

void KUdpRelayChecker::OpenScriptFile()
{
	KAutoPath		kAutoPath;
	std::string		strFile;

	strFile = "UdpRelayCheckerConfig.lua";
	kAutoPath.GetPullPath( strFile );

	if( LUA_DOFILE( g_pLua, strFile.c_str() ) == 0 )
	{
		START_LOG( cout, "UdpRelayChecker��ũ��Ʈ ���� �ε� ����!" );
	}
	else
	{
		START_LOG( cout, "UdpRelayChecker��ũ��Ʈ ���� �ε� ����!" )
			<< BUILD_LOG( KncUtil::toWideString( lua_tostring( g_pLua, -1 ) ) );
	}
}

void KUdpRelayChecker::RegToLua()
{
	lua_tinker::class_add<KUdpRelayChecker>( g_pLua, "KUdpRelayChecker" );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerOn",			&KUdpRelayChecker::SetUdpRelayCheckerOn_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "AddUdpRelayCheckerOnServerClass", &KUdpRelayChecker::AddUdpRelayCheckerOnServerClass_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerPrintLog",		&KUdpRelayChecker::SetUdpRelayCheckerPrintLog_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerPort",			&KUdpRelayChecker::SetUdpRelayCheckerPort_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerTimeGap",		&KUdpRelayChecker::SetUdpRelayCheckerTimeGap_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetDBUpdateTimeGap",				&KUdpRelayChecker::SetDBUpdateTimeGap_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "dump",							&KUdpRelayChecker::Dump );

	lua_tinker::decl( g_pLua, "UdpRelayChecker", this );
}

void KUdpRelayChecker::SetUdpRelayCheckerOn_LUA( bool bOn )
{
	m_bCheckerOn = bOn;

	if( m_bCheckerOn )
	{
		START_LOG( cout, L"UDP Relay Checker�� �մϴ�!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() );
	}
	else
	{
		START_LOG( cout, L"UDP Relay Checker�� ���ϴ�!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() );
	}
}

void KUdpRelayChecker::AddUdpRelayCheckerOnServerClass_LUA( int iServerClass )
{
	switch( iServerClass )
	{
	case KBaseServer::ESC_CHANNEL:
		{
			START_LOG( cout, L"UDP Relay Checker�� ä�μ������� �մϴ�!" );
		}
		break;

	case KBaseServer::ESC_GAME:
		{
			START_LOG( cout, L"UDP Relay Checker�� ���Ӽ������� �մϴ�!" );
		}
		break;

	default:
		{
			START_LOG( cout, L"�ش� ������ UDP Relay Checker�� �ѵ��� ������ �ʾҽ��ϴ�." )
				<< BUILD_LOG( iServerClass );
		}
		return;
	}

	m_setInitServerClass.insert( iServerClass );
}

void KUdpRelayChecker::SetUdpRelayCheckerPrintLog_LUA( bool bOn )
{
	m_bPrintLog = bOn;

	START_LOG( cout, L"UDP Relay Checker Print Log : " << m_bPrintLog );
}

void KUdpRelayChecker::SetUdpRelayCheckerPort_LUA( short sPort )
{
    m_sUdpRelayCheckerPort = sPort;

	START_LOG( cout, L"UDP Relay Checker Port : " << sPort );
}

void KUdpRelayChecker::SetUdpRelayCheckerTimeGap_LUA( double fGap )
{
	m_fRelayCheckPacketGap = fGap;

	START_LOG( cout, L"UDP Relay Check Packet Time Gap : " << fGap );
}

void KUdpRelayChecker::SetDBUpdateTimeGap_LUA( double fGap )
{
	m_fDBUpdateGap = fGap;

	START_LOG( cout, L"UDP Relay Check DB Update Time Gap : " << fGap );
}

void KUdpRelayChecker::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KUdpRelayChecker::Init()
{
	// ��ũ��Ʈ �ε�
	RegToLua();
	OpenScriptFile();

	// ������ ���� �������� Ȯ��!
	if( m_setInitServerClass.find( KBaseServer::GetKObj()->GetServerClass() ) == m_setInitServerClass.end() )
		return;

	// UdpCheckerSession�� ���� UDP���� ����!
	if( m_spUdpCheckerSession != NULL )
	{
		START_LOG( cerr, L"�̹� UdpCheckerSession�� ������ �Ǿ� �ֳ�? �̷��� ����!" )
			<< END_LOG;
	}
	else
	{
		__int64 iUID = LLONG_MAX;
		KncUid::SetServerGroupID( iUID, KBaseServer::GetKObj()->GetServerGroupID() );

		m_spUdpCheckerSession = boost::shared_ptr<KUdpCheckerSession>( new KUdpCheckerSession( m_sUdpRelayCheckerPort ) );
		m_spUdpCheckerSession->Init( this, iUID );

		// udp checker session recv thread ����!
		m_spUdpCheckerSession->Begin();
	}

	// UdpRelayChecker Tick�� ���� thread����!
	Begin();

	// DB�κ��� üũ�ؾ��� ���ͼ��� ����Ʈ�� �޾ƿ���!	
	SendToLogDB( DBE_GET_CENTER_SERVER_LIST_REQ );
}

void KUdpRelayChecker::ShutDown()
{
	// udp checker session�� ���� ����!
	if( IS_NOT_NULL( m_spUdpCheckerSession ) )
	{
		m_spUdpCheckerSession->ShutDown();
	}	

	// Tick thread����!
	End();
}

void KUdpRelayChecker::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

void KUdpRelayChecker::Tick()
{
	KPerformer::Tick();

	// Tick���鼭 UDPüũ�� �ǽ��Ѵ�.
	CheckUdpServer();
}

void KUdpRelayChecker::CheckUdpServer()
{
	if( !m_bCheckerOn )
		return;

	KDBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT kPacketNot;

	std::map< std::wstring, SUdpRelayServerInfo >::iterator mit;
	for( mit = m_mapUdpRelayServerInfo.begin(); mit != m_mapUdpRelayServerInfo.end(); ++mit )
	{
		SUdpRelayServerInfo& sUdpRelayServerInfo = mit->second;

		//////////////////////////////////////////////////////////////////////////
		// Relay Check Packet ���� ���� : 5�ʸ��� üũ!
		if( sUdpRelayServerInfo.m_tRelayCheckTimer.elapsed() > m_fRelayCheckPacketGap )
		{
			sUdpRelayServerInfo.m_tRelayCheckTimer.restart();

			// UDP ������ ������ ����� �������� Ȯ��! �ȵǾ��ִٸ� ���� �õ�!
			if( sUdpRelayServerInfo.m_bRelayConnected == false )
			{
				m_spUdpCheckerSession->ConnectRelayServer( sUdpRelayServerInfo.m_wstrServerIP, sUdpRelayServerInfo.m_sServerPort );

				if( IsPrintLog() )
				{
					START_LOG( cout, L"[�˸�] UDP Relay Checker Send Connect Packet!" )
						<< BUILD_LOG( sUdpRelayServerInfo.m_wstrServerIP );
				}
				continue;
			}

			// ���� üũ
			if( sUdpRelayServerInfo.m_bSendRelayPacket )
			{
				if( IsPrintLog() )
				{
					START_LOG( cout, L"[�˸�] 5�ʵ��� ������ ��Ŷ�� ���ƿ��� �ʾҴ�! �ٽ� UDP ���� �õ��� ����!" )
						<< BUILD_LOG( sUdpRelayServerInfo.m_wstrServerIP );
				}

				// 30�ʵ��� ������ ��Ŷ�� ���ƿ��� �ʾҴ�. peer�� �������ɱ�? �ٽ� ������ó���� �غ���!
				sUdpRelayServerInfo.m_bRelayConnected = false;
				sUdpRelayServerInfo.m_bSendRelayPacket = false;
				continue;
			}

			// UDP ������ ������ ����� ���¶�� �׽�Ʈ ��Ŷ�� ������!
			KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT m_kTestPacket;
			KSerBuffer buff;
			Serialize( &buff, &m_kTestPacket );
			m_spUdpCheckerSession->SetRelayIPPort( sUdpRelayServerInfo.m_wstrServerIP, sUdpRelayServerInfo.m_sServerPort );

			// ��Ŷ ������ ���� TickCount
			//sUdpRelayServerInfo.m_dwLastTickCount = ::GetTickCount();
			sUdpRelayServerInfo.m_dwLastTimeGetTime = ::timeGetTime();
			//m_kQPCounter.GetQPCounter( sUdpRelayServerInfo.m_qwLastQPCounter );

			// ��Ŷ ����!
			m_spUdpCheckerSession->BroadCast( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT, (char*)buff.GetData(), buff.GetLength() );

			if( IsPrintLog() )
			{
				START_LOG( cout, L"[�˸�] UDP Relay Server Check Packet ����!" )
					<< BUILD_LOG( sUdpRelayServerInfo.m_wstrServerIP );
			}

			// ���� üũ!
			sUdpRelayServerInfo.m_bSendRelayPacket = true;
		}

		//////////////////////////////////////////////////////////////////////////
		// ��� ���� DB������Ʈ! : 5�и��� üũ!
		if( sUdpRelayServerInfo.m_tDBUpdateTimer.elapsed() > m_fDBUpdateGap )
		{
			sUdpRelayServerInfo.m_tDBUpdateTimer.restart();

			CTime tCurTime = CTime::GetCurrentTime();

			KUdpRelayCheckLog kInfo;
			kInfo.m_iServerUID			= sUdpRelayServerInfo.m_iServerUID;
			kInfo.m_wstrServerIP		= sUdpRelayServerInfo.m_wstrServerIP;
			if( sUdpRelayServerInfo.m_dwCountTimeGetTime != 0 )
			{
				kInfo.m_dwAvgDelayTime	= sUdpRelayServerInfo.m_dwTotalTimeGetTime / sUdpRelayServerInfo.m_dwCountTimeGetTime;
			}
			kInfo.m_dwMaxDelayTime		= sUdpRelayServerInfo.m_dwMaxTimeGetTime;
			kInfo.m_wstrRegDate			= (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			kPacketNot.m_vecUdpRelayCheckLog.push_back( kInfo );

			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"5�е��� ������ UDP Relay Check ���� ����!" )
				<< BUILD_LOG( kInfo.m_wstrServerIP )
				<< BUILD_LOG( sUdpRelayServerInfo.m_dwTotalTimeGetTime )
				<< BUILD_LOG( sUdpRelayServerInfo.m_dwCountTimeGetTime )
				<< BUILD_LOG( kInfo.m_dwAvgDelayTime )
				<< BUILD_LOG( kInfo.m_dwMaxDelayTime );
			//////////////////////////////////////////////////////////////////////////			

			// �� �ʱ�ȭ!
            sUdpRelayServerInfo.ClearStatistics();
		}
	}

	// DB������Ʈ
	if( kPacketNot.m_vecUdpRelayCheckLog.empty() == false )
	{
        SendToLogDB( DBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT, kPacketNot );
	}
}

void KUdpRelayChecker::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

	switch( spEvent_->m_usEventID )
	{
		CASE( DBE_GET_CENTER_SERVER_LIST_ACK );
		CASE( E_CONNECT_RELAY_ACK );
		CASE( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT );

	default:
		dbg::cerr << L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr();
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC( DBE_GET_CENTER_SERVER_LIST_ACK )
{
	switch( KBaseServer::GetKObj()->GetServerClass() )
	{
	case KBaseServer::ESC_CHANNEL:
		{
			BOOST_TEST_FOREACH( const KServerInfo&, kCnServerInfo, kPacket_.m_vecCenterServerList )
			{
				//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
				START_LOG( cout, L"UDP Relay Server ����͸� ��� ���! : " << kCnServerInfo.m_wstrPublicIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrPublicIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrPublicIP, sInsertInfo ) );
#else
				START_LOG( cout, L"UDP Relay Server ����͸� ��� ���! : " << kCnServerInfo.m_wstrIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrIP, sInsertInfo ) );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
				//}}
			}
		}
		break;

	case KBaseServer::ESC_GAME:
		{
			BOOST_TEST_FOREACH( const KServerInfo&, kCnServerInfo, kPacket_.m_vecCenterServerList )
			{
				//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
				// ���Ӽ����� ���� IP�� üũ�մϴ�!
				if( NetCommon::GetLocalIPW() != kCnServerInfo.m_wstrPublicIP )
					continue;

				START_LOG( cout, L"UDP Relay Server ����͸� ��� ���! : " << kCnServerInfo.m_wstrPublicIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrPublicIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrPublicIP, sInsertInfo ) );
#else
				// ���Ӽ����� ���� IP�� üũ�մϴ�!
				if( NetCommon::GetLocalIPW() != kCnServerInfo.m_wstrIP )
					continue;

				START_LOG( cout, L"UDP Relay Server ����͸� ��� ���! : " << kCnServerInfo.m_wstrIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrIP, sInsertInfo ) );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
				//}}
			}            
		}
		break;

	default:
		{
			START_LOG( cerr, L"�� ���������� Udp Relay Checker�� �����ϸ� �ȵ˴ϴ�!" )
				<< BUILD_LOG( KBaseServer::GetKObj()->GetServerClass() )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC( E_CONNECT_RELAY_ACK )
{
	std::map< std::wstring, SUdpRelayServerInfo >::iterator mit;
	mit = m_mapUdpRelayServerInfo.find( kPacket_.m_wstrRelayServerIP );
	if( mit == m_mapUdpRelayServerInfo.end() )
	{
		START_LOG( cerr, L"��ϵ��� ���� IP�Դϴ�!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() )
			<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
			<< END_LOG;
		return;
	}

	// ������ ���� ���� ó��!
	mit->second.m_bRelayConnected = true;

	if( IsPrintLog() )
	{
		START_LOG( cout, L"[�˸�] UDP Relay Checker Connect Success!" )
			<< BUILD_LOG( mit->second.m_wstrServerIP );
	}
}

IMPL_ON_FUNC( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT )
{
	std::map< std::wstring, SUdpRelayServerInfo >::iterator mit;
	mit = m_mapUdpRelayServerInfo.find( kPacket_.m_wstrRelayServerIP );
	if( mit == m_mapUdpRelayServerInfo.end() )
	{
		START_LOG( cerr, L"��ϵ��� ���� IP�Դϴ�!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() )
			<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
			<< END_LOG;
		return;
	}

	SUdpRelayServerInfo& sUdpRelayServerInfo = mit->second;

	// ������ ��Ŷ�� ���������Ƿ� üũ ����
	sUdpRelayServerInfo.m_bSendRelayPacket = false;

	//{
	//	const DWORD dwUdpRelayLatency = kPacket_.m_dwEndTickCount - mit->second.m_dwLastTickCount;

	//	START_LOG( cout, L"[�׽�Ʈ�α�] UDP Relay Server ���� �ð� üũ!" )
	//		<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
	//		<< BUILD_LOG( mit->second.m_dwLastTickCount )
	//		<< BUILD_LOG( kPacket_.m_dwEndTickCount )
	//		<< BUILD_LOG( dwUdpRelayLatency );
	//}
	//
	{
		const DWORD dwUdpRelayLatency = kPacket_.m_dwEndTimeGetTime - sUdpRelayServerInfo.m_dwLastTimeGetTime;

		//START_LOG( cout, L"[�׽�Ʈ�α�] UDP Relay Server ���� �ð� üũ!" )
		//	<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
		//	<< BUILD_LOG( sUdpRelayServerInfo.m_dwLastTimeGetTime )
		//	<< BUILD_LOG( kPacket_.m_dwEndTimeGetTime )
		//	<< BUILD_LOG( dwUdpRelayLatency );

		// ���
        if( sUdpRelayServerInfo.m_dwMaxTimeGetTime < dwUdpRelayLatency )
		{
            sUdpRelayServerInfo.m_dwMaxTimeGetTime = dwUdpRelayLatency;
		}

		sUdpRelayServerInfo.m_dwTotalTimeGetTime += dwUdpRelayLatency;
		++sUdpRelayServerInfo.m_dwCountTimeGetTime;

		if( IsPrintLog() )
		{
			START_LOG( cout, L"[�˸�] UDP Relay Check Packet ����!" )
				<< BUILD_LOG( mit->second.m_wstrServerIP )
				<< BUILD_LOG( dwUdpRelayLatency );
		}
	}

	//{
	//	const float fUdpRelayLatency = m_kQPCounter.GetElapsedTime( mit->second.m_qwLastQPCounter );

	//	START_LOG( cout, L"[�׽�Ʈ�α�] UDP Relay Server ���� �ð� üũ!" )
	//		<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
	//		<< BUILD_LOG( fUdpRelayLatency );
	//}
}



#endif SERV_UDP_RELAY_CHECKER
//}}


