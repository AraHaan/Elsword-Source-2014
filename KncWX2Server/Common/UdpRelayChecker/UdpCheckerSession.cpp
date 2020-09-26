#include "UdpCheckerSession.h"
#include "../socket/NetCommon.h"  // CLOSE_SOCKET
#include <dbg/dbg.hpp>
#include "../socket/Session.h"    // EVENT_UDP_PORT_NOT
#include "KncSend.h"
#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "UdpRelayChecker.h"

#ifdef SERV_UDP_RELAY_CHECKER


KUdpCheckerSession::KUdpCheckerSession( short sUdpPort ) : 
CKTDNUDP( sUdpPort ), // �θ� ������ ȣ��!
m_pUdpRelayChecker( NULL )
{
}

KUdpCheckerSession::~KUdpCheckerSession(void)
{
}

void KUdpCheckerSession::Init( KUdpRelayChecker* pUdpRelayChecker, const __int64 iPeerUID )
{
	m_pUdpRelayChecker = pUdpRelayChecker;

	// UID�� ����
	SetMyUID( iPeerUID );

	// �׽�Ʈ�� ���� peer�߰�
	AddPeerForRelayTest( iPeerUID, NetCommon::GetLocalIPW().c_str(), GetMyPort() );
}

void KUdpCheckerSession::SetRelayIPPort( const std::wstring& wstrRelayServIP, const short sRelayServPort )
{
	SetRelayIP( wstrRelayServIP.c_str() );
	SetRelayPort( sRelayServPort );
}

void KUdpCheckerSession::ConnectRelayServer( const std::wstring& wstrRelayServIP, const short sRelayServPort )
{
    SetRelayIPPort( wstrRelayServIP, sRelayServPort );

	// ������ ���� ���� �õ�!
	ConnectTestToRelay();
}

void KUdpCheckerSession::Run()
{
    START_LOG( clog, L"Enter Run()" );

    DWORD ret;

    while( true )
    {
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		OnFrameMove();

		ret = ::WaitForSingleObjectEx( m_hKillEvent, INFINITE, TRUE );     // sleep Ÿ���� �ָ� �ȵȴ�.

		if( ret == WAIT_OBJECT_0 ) 
			break;

		_ProcessRecvData();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        ret = ::WaitForSingleObject( m_hKillEvent, 1 );     // sleep Ÿ���� �ָ� �ȵȴ�.
//
//        if( ret == WAIT_OBJECT_0 ) break;
//
//        if( ret == WAIT_TIMEOUT ) OnFrameMove( 0.0, 0.f ); // �������ڴ� dummy��.
//
//        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    }// while
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void KUdpCheckerSession::OnFrameMove()
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void KUdpCheckerSession::OnFrameMove( double fTime, float fElapsedTime )
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	RecvForRelayTest();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	WSANETWORKEVENTS	netEvent;
//	RecvData kRecvData;
//	bool bRecvSuccess = false;
//
//	//��Ŷ ���ú�
//	while( true )
//	{
//		//Event select
//		::ZeroMemory( &netEvent, sizeof(netEvent) );
//
//		::WSAEnumNetworkEvents( GetSocketHandle(), GetRecvEvent(), &netEvent );
//
//		if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
//			bRecvSuccess = RecvForRelayTest( kRecvData );
//		else
//			break;
//	}
//
//	//bRecvSuccess = RecvForRelayTest( kRecvData );
//	if( bRecvSuccess )
//	{
//		switch( kRecvData.m_ID )
//		{
//		case SP_CONNECT_RELAY_ACK: // for client
//			{
//				// UDP Relay ������ peer�� �����Ǿ��ٸ� �� ��Ŷ�� �����Ұ��̴�!
//				if( m_pUdpRelayChecker->IsPrintLog() )
//				{
//					START_LOG( cout, L"[�˸�] Recv Packet : SP_CONNECT_RELAY_ACK" )
//						<< BUILD_LOG( kRecvData.m_SenderIP );
//				}
//
//				// �ش� Relay���������� ���� ����� �����ص���!
//				KE_CONNECT_RELAY_ACK kRelayTestPacket;
//				kRelayTestPacket.m_wstrRelayServerIP = kRecvData.m_SenderIP;
//
//				KEventPtr spEvent( new KEvent );
//				spEvent->SetData( PI_NULL, NULL, E_CONNECT_RELAY_ACK, kRelayTestPacket );
//				m_pUdpRelayChecker->QueueingEvent( spEvent );
//			}
//			break;
//
//		case E_UDP_RELAY_SERVER_CHECK_PACKET_NOT:
//			{
//				if( m_pUdpRelayChecker->IsPrintLog() )
//				{
//					START_LOG( cout, L"[�˸�] Recv Packet : E_UDP_RELAY_SERVER_CHECK_PACKET_NOT" )
//						<< BUILD_LOG( kRecvData.m_SenderIP );
//				}
//
//				KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT kRelayTestPacket;
//				//kRelayTestPacket.m_dwEndTickCount = ::GetTickCount();
//				kRelayTestPacket.m_dwEndTimeGetTime = ::timeGetTime();
//				kRelayTestPacket.m_wstrRelayServerIP = kRecvData.m_SenderIP;
//				
//				KEventPtr spEvent( new KEvent );
//				spEvent->SetData( PI_NULL, NULL, E_UDP_RELAY_SERVER_CHECK_PACKET_NOT, kRelayTestPacket );
//				m_pUdpRelayChecker->QueueingEvent( spEvent );
//			}
//			break;
//
//		default:
//			{
//                START_LOG( cerr, L"�̻��� ��Ŷ�� �����Ͽ����ϴ�!" )
//					<< BUILD_LOG( kRecvData.m_ID )
//					<< END_LOG;
//			}
//			break;
//		}
//	}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

void KUdpCheckerSession::ShutDown()
{
	// thread����!
    End();

    ::WSACleanup();
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void KUdpCheckerSession::_ProcessRecvData()
{
	list<RecvData>::iterator itRecvData;
	while( GetFrontRecvData( itRecvData ) )
	{
		switch( itRecvData->m_ID )
		{
		case SP_CONNECT_RELAY_ACK: // for client
			{
				// UDP Relay ������ peer�� �����Ǿ��ٸ� �� ��Ŷ�� �����Ұ��̴�!
				if( m_pUdpRelayChecker->IsPrintLog() )
				{
					START_LOG( cout, L"[�˸�] Recv Packet : SP_CONNECT_RELAY_ACK" )
						<< BUILD_LOG( itRecvData->m_SenderIP );
				}

				// �ش� Relay���������� ���� ����� �����ص���!
				KE_CONNECT_RELAY_ACK kRelayTestPacket;
				kRelayTestPacket.m_wstrRelayServerIP = itRecvData->m_SenderIP;

				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_NULL, NULL, E_CONNECT_RELAY_ACK, kRelayTestPacket );
				m_pUdpRelayChecker->QueueingEvent( spEvent );
			}
			break;

		case E_UDP_RELAY_SERVER_CHECK_PACKET_NOT:
			{
				if( m_pUdpRelayChecker->IsPrintLog() )
				{
					START_LOG( cout, L"[�˸�] Recv Packet : E_UDP_RELAY_SERVER_CHECK_PACKET_NOT" )
						<< BUILD_LOG( itRecvData->m_SenderIP );
				}

				KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT kRelayTestPacket;
				//kRelayTestPacket.m_dwEndTickCount = ::GetTickCount();
				kRelayTestPacket.m_dwEndTimeGetTime = ::timeGetTime();
				kRelayTestPacket.m_wstrRelayServerIP = itRecvData->m_SenderIP;

				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_NULL, NULL, E_UDP_RELAY_SERVER_CHECK_PACKET_NOT, kRelayTestPacket );
				m_pUdpRelayChecker->QueueingEvent( spEvent );
			}
			break;

		default:
			{
				START_LOG( cerr, L"�̻��� ��Ŷ�� �����Ͽ����ϴ�!" )
					<< BUILD_LOG( itRecvData->m_ID )
					<< END_LOG;
			}
			break;
		}

		ReturnRecvData( itRecvData );
	}
}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif  SERV_UDP_RELAY_CHECKER
