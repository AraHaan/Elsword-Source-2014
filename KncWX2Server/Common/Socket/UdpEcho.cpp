#include <WinSock2.h>
#include "UdpEcho.h"
#include "NetCommon.h"  // CLOSE_SOCKET
#include <dbg/dbg.hpp>
#include "Session.h"    // EVENT_UDP_PORT_NOT
#include "KncSend.h"

#define MAX_UDP_PACKET_SIZE 1024
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))

KUdpEcho::KUdpEcho(void) : m_sock( INVALID_SOCKET )
{
}

KUdpEcho::~KUdpEcho(void)
{
}

bool KUdpEcho::Init( USHORT usPort_ )
{
    // ���� �ʱ�ȭ�� �̹� ���� �Ǿ��ٰ� �����ϰ� �����Ѵ�.

    m_sock = ::socket( AF_INET, SOCK_DGRAM, 0 );    // ���� ����

    if( INVALID_SOCKET == m_sock )
    {
        START_LOG( cerr, NetCommon::GetWsaMsg() );
        ::WSACleanup();
        return false;
    }

    SOCKADDR_IN local;
    local.sin_family        = AF_INET;
    local.sin_addr.s_addr   = INADDR_ANY;
    local.sin_port     = htons( usPort_ );
	ZeroMemory( local.sin_zero, sizeof(char) * ARRAY_SIZE(local.sin_zero ) );

    // usPort_�� UDP ��Ʈ�� �����Ѵ�.
    if( ::bind( m_sock, (SOCKADDR *)&local, sizeof(local) ) == SOCKET_ERROR)
    {
        START_LOG( cerr, NetCommon::GetWsaMsg() );
        ::WSACleanup();
        return false;
    }

    return true;
}

void KUdpEcho::Run()
{
    START_LOG( clog, L"Enter Run()" );

    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 0 );     // sleep Ÿ���� �ָ� �ȵȴ�.

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Loop();

        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

    }// while

}

void KUdpEcho::Loop()
{
    int ret;
    char recvbuff[MAX_UDP_PACKET_SIZE] = {0};
    SOCKADDR_IN sender;
    int nSenderSize = sizeof(sender);

    ret = ::recvfrom( m_sock,
                      recvbuff,
                      MAX_UDP_PACKET_SIZE,
                      0,
                      (SOCKADDR*)&sender,
                      &nSenderSize );

    if( ret == SOCKET_ERROR )
    {
        START_LOG( cerr, NetCommon::GetWsaMsg() );
        return;
    }

    if( ret != sizeof( KUdpToServer ) ) // �׻� KUdpToServer ����ü���� �޵��� �����Ѵ�.
    {
        START_LOG( cerr, L"���޹��� ������ ũ�� �̻�. wanted : " << sizeof(KUdpToServer) )
            << BUILD_LOG( ret )
            << BUILD_LOG( inet_ntoa(sender.sin_addr) )
            << BUILD_LOG( ntohs(sender.sin_port) )
            << END_LOG;
        return;
    }

    KUdpToServer    kData;
    USHORT          usPort = ntohs( sender.sin_port );
    ::memmove( &kData, recvbuff, sizeof(kData) );

    START_LOG( clog, L"UID : " << kData.m_nUserUID
        << L", sock addr : " << inet_ntoa(sender.sin_addr) << L":" << usPort )
        << BUILD_LOG( kData.m_usPort );

    SendToClient( kData.m_nUserUID, E_UDP_PORT_NOT, usPort );
}

template < class T >
void KUdpEcho::SendToClient( UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nTo, -1 };
    KncSend( PI_GS_SERVER, 0, PI_GS_PROXY, nTo, anTrace, usEventID, data );
}

void KUdpEcho::SendToClient( UidType nTo, unsigned short usEventID )
{
    SendToClient( nTo, usEventID, char() );
}

void KUdpEcho::ShutDown()
{
    CLOSE_SOCKET( m_sock );     // blocked mode. ������ ���� �ݾ� �����尡 ������ ����ǵ��� �����Ѵ�. (������� ����.)

    End();

    ::WSACleanup();
}

#undef MAX_UDP_PACKET_SIZE // 1024
