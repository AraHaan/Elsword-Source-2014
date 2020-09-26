#include "InteriorUdpSession.h"

//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include "..\Common\socket\NetCommon.h"  // CLOSE_SOCKET
#include "..\Common\socket\Session.h"    // EVENT_UDP_PORT_NOT
#include <dbg/dbg.hpp>
#include <boost/test/utils/foreach.hpp>
#include "KncSend.h"
#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "..\Common\Room\RoomManager.h"
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#include "NetLayer.h"
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

ImplInstanceKeeping( KInteriorUdpSession );

KInteriorUdpSession::KInteriorUdpSession()
{
}

KInteriorUdpSession::~KInteriorUdpSession(void)
{
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK  
//	if( m_Socket != INVALID_SOCKET )
//	{
//		shutdown( m_Socket, SD_BOTH );
//		closesocket( m_Socket );
//		m_Socket = INVALID_SOCKET;
//	}
//	WSACloseEvent( m_RecvEvent );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

void KInteriorUdpSession::Init( IN u_short usTargetPort )
{
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	char strTemp[256];
//	::gethostname( strTemp, 256 );
//
//	HOSTENT*	hostent = ::gethostbyname( strTemp );
//	string		IP		= ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );
//
//	m_Socket		= socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
//	int sndBuf		= 65534;
//	int rcvBuf		= 65534;
//	setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf) );
//	setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, sizeof(rcvBuf) );
//
//	SOCKADDR_IN localAddr;
//	localAddr.sin_family		= AF_INET;
//	localAddr.sin_port			= htons( CKTDNUDP::IUS_PORT );
//	localAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
//	int ret	= bind( m_Socket, ( SOCKADDR* )&localAddr, sizeof( localAddr ) );
//
//	if( ret != 0 )
//	{
//		START_LOG( cerr, L"UDP: ���!! ���� üũ�� UDP ��Ʈ bind�� ���� �Ͽ����ϴ�." )
//			<< BUILD_LOG( ret )
//			<< END_LOG;
//	}
//
//	// IP üũ�� ���� ����
//	m_ulLocalIP1 = inet_addr( IP.c_str() );
//	m_ulLocalIP2 = inet_addr( "127.0.0.1" );
//
//	m_RecvEvent = WSACreateEvent();
//	::WSAEventSelect( m_Socket, m_RecvEvent, FD_READ | FD_CLOSE );
//
//	ZeroMemory( m_pRecvBuffer, RECV_BUFFER_SIZE );
//	m_RecvDataList.reserve( 1000 );
//
//	// send info
//	//������ ����
//	m_DesAddr.sin_family		= AF_INET;
//	m_DesAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
//	m_DesAddr.sin_port			= htons( usTargetPort );  //��Ʈ��ȣ
//
//	KThread::Begin();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void KInteriorUdpSession::Run( void )
//{
//	START_LOG( clog, L"Enter Run()" );
//
//	DWORD ret;
//
//	while( true )
//	{
//		ret = ::WaitForSingleObject( m_hKillEvent, 1 );     // sleep Ÿ���� �ָ� �ȵȴ�.
//
//		if( ret == WAIT_OBJECT_0 ) break;
//
//		if( ret == WAIT_TIMEOUT ) OnFrameMove();
//
//		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;
//
//	}// while
//}
//
//bool KInteriorUdpSession::Recv( void )
//{
//	KTDXPROFILE();
//
//	SOCKADDR_IN SenderAddr;
//
//	int SenderAddrSize	= sizeof(SenderAddr);
//	int recvSize		= recvfrom( m_Socket, m_pRecvBuffer, RECV_BUFFER_SIZE, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize );
//
//	if( recvSize == SOCKET_ERROR )
//	{
//		return false;
//	}
//	else if( recvSize == 0 )
//	{
//		// ������ �����? UDP�ε� �̷� �� �ֳ�?
//		START_LOG( cerr, L"UDP ���� ��Ŷ ������ 0" )
//			<< END_LOG;
//		return false;
//	}
//	else if( recvSize < 0 || recvSize > RECV_BUFFER_SIZE )
//	{
//		// ������ SOCKET_ERROR �� �����ϰ�� �̻��� ����̴�. ����� �ʹ� Ŀ�� �̻��� ����̴�.
//		START_LOG( cerr, L"UDP ���� ��Ŷ ����� ����." )
//			<< BUILD_LOG( recvSize )
//			<< END_LOG;
//		return false;
//	}
//
//	if( m_pRecvBuffer[0] != CKTDNUDP::SP_MORNITORING_NOT )
//	{
//		START_LOG( cwarn, L"�̻��� ��Ŷ�� �����Ͽ����ϴ�!" )
//			<< BUILD_LOG( m_pRecvBuffer[0] )
//			<< END_LOG;
//		return false;
//	}
//
//	if( IsLocal( SenderAddr.sin_addr.s_addr ) == false )
//	{
//		// ���� IP���� �귯�� ������ �ƴϴ�?
//		START_LOG( cwarn, L"UDP: ���� ��ſ����� ���Ǵ� ��Ŷ�� �ܺηκ��� ���� �Ǿ����ϴ�." )
//			<< BUILD_LOG( SenderAddr.sin_addr.s_net )
//			<< BUILD_LOG( SenderAddr.sin_addr.s_host )
//			<< BUILD_LOG( SenderAddr.sin_addr.s_lh )
//			<< BUILD_LOG( SenderAddr.sin_addr.s_impno )
//			<< END_LOG;
//		return false;
//	}
//	
//	unsigned long uncompressSize = RECV_BUFFER_SIZE;
//	if( uncompress( (BYTE*)m_pUncompressBuffer, &uncompressSize, (BYTE*)(&m_pRecvBuffer[1]), recvSize-1 ) != Z_OK )
//	{
//		return false;
//	}
//
//	if( ( uncompressSize % sizeof( CKTDNUDP::LB_INFO_STRUCT ) ) != 0 )
//	{
//		START_LOG( cwarn, L"UDP: SP_MORNITORING_CONFIRM_NOT ��Ŷ ����� �̻��մϴ�." )
//			<< BUILD_LOG( uncompressSize )
//			<< BUILD_LOG( sizeof( CKTDNUDP::LB_INFO_STRUCT ) )
//			<< END_LOG;
//		return false;
//	}
//
//	CKTDNUDP::LB_INFO_STRUCT* pRecvInfo = reinterpret_cast<CKTDNUDP::LB_INFO_STRUCT*>( m_pUncompressBuffer );
//	size_t iSize = uncompressSize / sizeof( CKTDNUDP::LB_INFO_STRUCT );
//
//	for( size_t i=0; i < iSize; ++i )
//	{
//		m_RecvDataList.push_back( pRecvInfo[i] );
//	}
//	return true;
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void KInteriorUdpSession::OnFrameMove( void )
{
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	WSANETWORKEVENTS	netEvent;
//
//	//Event select
//	::ZeroMemory( &netEvent, sizeof(netEvent) );
//
//	::WSAEnumNetworkEvents( m_Socket, m_RecvEvent, &netEvent );
//
//	if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
//	{
//		Recv();
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    BOOST_TEST_FOREACH( const CKTDNUDP::LB_INFO_STRUCT&, recvData, m_vecStorage )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	BOOST_TEST_FOREACH( const CKTDNUDP::LB_INFO_STRUCT&, recvData, m_RecvDataList )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	{
		switch( recvData.m_eEventID )
		{
		case CKTDNUDP::LEI_CHECK_CANCLE_NOT:
			{
				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_CN_ROOM, NULL, ERM_REMOVE_LANBUG_CHECK_USER_NOT,	recvData.m_iUnitUID );
				spEvent->m_kDestPerformer.AddUID( recvData.m_iRoomUID );
				KRoomManager::GetKObj()->QueueingEventToRoom( spEvent );
			}
			break;

		case CKTDNUDP::LEI_CONFIRM_NOT:
			{
				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_CN_ROOM, NULL, ERM_ADD_UDP_CHECK_KICK_USER_NOT,	recvData.m_iUnitUID );
				spEvent->m_kDestPerformer.AddUID( recvData.m_iRoomUID );
				KRoomManager::GetKObj()->QueueingEventToRoom( spEvent );
			}
			break;

		case CKTDNUDP::LEI_VERIFY_CANCLE_NOT:
			{
				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_CN_ROOM, NULL, ERM_REMOVE_LANBUG_VERIFY_USER_NOT,	recvData.m_iUnitUID );
				spEvent->m_kDestPerformer.AddUID( recvData.m_iRoomUID );
				KRoomManager::GetKObj()->QueueingEventToRoom( spEvent );
			}
			break;
		default:
			continue;
			break;
		}
	}
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_vecStorage.resize(0);
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_RecvDataList.clear();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//void KInteriorUdpSession::ShutDown( void )
//{
//	// thread����!
//	KThread::End();
//
//	::WSACleanup();
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void KInteriorUdpSession::SendAndFlush( void )
{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    if ( CKTDNUDPPtr spUDP = KNetLayer::GetKObj()->GetKTUDP() )
    {
        spUDP->ThreadSafeSwapLBInfo( m_vecStorage );
    }
#else   SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    KNetLayer::GetKObj()->GetKTUDP()->ThreadSafeSwapLBInfo( m_vecStorage );
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    OnFrameMove();

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//
//	if( m_vecStorage.empty() == true )
//	{
//		return;
//	}
//
//	size_t iNumOnce		= RECV_BUFFER_SIZE / sizeof( CKTDNUDP::LB_INFO_STRUCT );
//	size_t iNumSended	= 0;
//	size_t iNumRemain	= m_vecStorage.size();
//	size_t iNumStorage	= 0;
//
//	// ����ҿ� �����Ͱ� ���� �ִٸ�,
//	while( 0 < iNumRemain )
//	{
//		CKTDNUDP::LB_INFO_STRUCT* pSendBuffer = reinterpret_cast<CKTDNUDP::LB_INFO_STRUCT*>( m_pSendBuffer );
//
//		// �ѹ��� ���� �� �ִ� �� ��ŭ �غ�����.
//		size_t iSize = ( iNumRemain < iNumOnce ) ? iNumRemain : iNumOnce;
//
//		for( size_t i=0; i < iSize; ++i )
//		{
//			pSendBuffer[i] = m_vecStorage[i+iNumSended];
//		}
//
//		iNumRemain -= iSize;
//		iNumSended += iSize;
//
//		// ����
//		unsigned long compressSize = RECV_BUFFER_SIZE;
//		if( CKTDNUDP::Compress( CKTDNUDP::SP_MORNITORING_NOT, m_pCompressBuffer, compressSize, m_pSendBuffer, iSize * sizeof( CKTDNUDP::LB_INFO_STRUCT ) ) == false )
//		{
//			START_LOG( cwarn, L"UDP: Compress ����" )
//				<< BUILD_LOG( iSize )
//				<< END_LOG;
//			continue;
//		}
//
//		sendto( m_Socket, m_pCompressBuffer, compressSize, 0, ( SOCKADDR* )&m_DesAddr, sizeof( SOCKADDR_IN ) );
//	}
//	m_vecStorage.clear();
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
