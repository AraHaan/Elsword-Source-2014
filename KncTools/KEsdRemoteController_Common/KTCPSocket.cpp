#include "stdafx.h"

#include "KTCPSocket.h"
#include "KNetUtil.h"

KTCPSocket::KTCPSocket()
{
}

KTCPSocket::KTCPSocket( SOCKET kSocket )
{
	SetSocket( kSocket );
}

KTCPSocket::KTCPSocket( SOCKET kSocket, SOCKADDR_IN kSockAddr_In )
{
	SetSocket( kSocket );
	SetSockAddr( kSockAddr_In );

	m_sSockAddr.sin_family   = AF_INET;

	m_ovlRecv.m_eIOMode     = KOVERLAPPED::IO_RECV;
	m_ovlSend.m_eIOMode     = KOVERLAPPED::IO_SEND;
}

KTCPSocket::~KTCPSocket()
{
}

bool KTCPSocket::CreateSocket( int iPort /*= 0 */ )
{
	m_hSocket = socket( AF_INET, SOCK_STREAM, 0 );

	if ( m_hSocket == INVALID_SOCKET )
	{
		KDbgStm << "Socket ���� ����!" << trace;
		CloseSocket();
		return false;
	}

	::ZeroMemory( &m_sSockAddr, sizeof(m_sSockAddr) );
	m_sSockAddr.sin_family = AF_INET;
	m_sSockAddr.sin_addr.S_un.S_addr = ADDR_ANY;

	if( iPort == 0 )
	{
		iPort = _KNetUtil()->GetRandomPort();
	}
	m_sSockAddr.sin_port = htons( iPort );

	if ( bind( m_hSocket, (SOCKADDR*) &m_sSockAddr, sizeof(m_sSockAddr) ) == SOCKET_ERROR )
	{
		KDbgStm << "Bind ����!" << trace;
		return false;
	}

	if ( listen( m_hSocket, 1 ) == SOCKET_ERROR )
	{
		KDbgStm << "Listen ����!" << trace;
		CloseSocket();
		return false;
	}

	return true;
}


void KTCPSocket::CloseSocket()
{
	closesocket( m_hSocket );
}

bool KTCPSocket::Connect( const char* pcAddress, int iPort )
{
	// try to connect
	m_hSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if (m_hSocket == INVALID_SOCKET)
	{
		KDbgStm << "Socket ���� ����!" << trace;
		CloseSocket();
		return false;
	}

	::ZeroMemory( &m_sSockAddr, sizeof(m_sSockAddr) );
	m_sSockAddr.sin_family = AF_INET;
	m_sSockAddr.sin_addr.S_un.S_addr = inet_addr(pcAddress);
	m_sSockAddr.sin_port = htons( iPort );

	if ( 0 != connect( m_hSocket, (SOCKADDR*) &m_sSockAddr, sizeof( m_sSockAddr ) ) )
	{
		KDbgStm << "Connect ����!" << trace;
		CloseSocket();
		return false;
	}

	return true;
}

SOCKET KTCPSocket::Accept( SOCKADDR* sSockAddr /*= NULL*/, int* iSockAddrSize /*= NULL */ )
{
	SOCKET kOutputSocket = INVALID_SOCKET;
	kOutputSocket = accept( m_hSocket, sSockAddr, iSockAddrSize );

	if (INVALID_SOCKET == kOutputSocket)
	{
		KDbgStm << "Accept ����!" << trace;
		return NULL;
	}

	return kOutputSocket;
}

int KTCPSocket::SendTo( SOCKET kSocket, const char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return ::send( kSocket, pcBuffer, iBufferSize, iFlag );
}

int KTCPSocket::RecvFrom( SOCKET kSocket, char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return ::recv( kSocket, pcBuffer, iBufferSize, iFlag );
}

int KTCPSocket::Send( const char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return SendTo( m_hSocket, pcBuffer, iBufferSize, iFlag );
}

int KTCPSocket::Recv( char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return RecvFrom( m_hSocket, pcBuffer, iBufferSize, iFlag );
}

void KTCPSocket::SetSocket( SOCKET kSocket )
{
	m_hSocket = kSocket;
}

SOCKET KTCPSocket::GetSocket()
{
	return m_hSocket;
}

SOCKADDR_IN& KTCPSocket::GetSockAddr()
{
	return m_sSockAddr;
}

void KTCPSocket::SetSockAddr( SOCKADDR_IN val )
{
	m_sSockAddr = val;
}

void KTCPSocket::OnSendCompleted( DWORD dwTransfered_ )
{
	return; // �ӽ� ó��

	//KLocker lock( m_csSock );

	if( m_hSocket == INVALID_SOCKET ) return;

	m_ovlSend.m_bSending    = false;    // 050127. �� ���´��� ���θ� ������ �׻� false. �������� ��� �Ʒ����� �ٽ� true����.

	if( dwTransfered_ >= m_ovlSend.m_dwLeft )   // ��û�� �����͸� ��� �� ����
	{
		m_ovlSend.m_dwLeft      = 0;        
	}
	else    // �����͸� �� ���� : �ٽ� ������ ��û 
	{
		m_ovlSend.m_dwLeft -= m_ovlSend.InternalHigh;
		memmove( &m_ovlSend.m_pBuffer[0], &m_ovlSend.m_pBuffer[dwTransfered_], m_ovlSend.m_dwLeft );
	}

	MovDataQue2Buf();   // send-queue�� ������� �����Ͱ� �ִٸ� ���۷� �ű��.

	if( m_ovlSend.m_dwLeft == 0 )   // ������ �� �����Ͱ� ���̻� �������� �ʴٸ� ����.
		return;

	m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

	DWORD dwWrite = 0;
	int ret = ::WSASend( m_hSocket,    // socket
		&m_ovlSend.m_wsaBuffer,     // ���� ������
		1,                          // WSABUF�� ����
		&dwWrite,                   // [out] ��� ���۵� ��
		0,                          // Flag
		&m_ovlSend,                 // OVERLAPPED
		NULL );

	if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // ���� pending ��..
	{
		m_ovlSend.m_bSending   = true;            
		return;
	} 

	if( ret != SOCKET_ERROR ) // ȣ�� ���� ���� �� ����.
	{
		m_ovlSend.m_bSending   = true;
		return;
	}

	//START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG );

	OnSocketError();

	return;
}

void KTCPSocket::MovDataQue2Buf()
{
	//KLocker lock( m_csSock );
	//KLocker lock2( m_csSendQueue );

	//int nSize = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
	//nSize = m_kSendQueue.Pop( nSize, &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft] );
	//m_ovlSend.m_dwLeft += nSize;
}

bool KTCPSocket::InitRecv()
{
	//KSimpleLocker lock( m_csSock );

	if( m_hSocket == INVALID_SOCKET ) return false;

	DWORD dwRead = 0;
	DWORD dwFlag = 0;    

	// Note: Win2000������ �Ʒ��� WSARecv() ��� ReadFile()�� ����� �� �ִ�.
	//       Win98������ ReadFile()�� ����� �� ����.

	m_ovlRecv.Clear();
	
	ZeroMemory( m_ovlRecv.m_pBuffer, MAX_PACKET_SIZE );
	m_ovlRecv.m_eIOMode		  = KOVERLAPPED::IO_RECV;
	m_ovlRecv.m_wsaBuffer.buf = &m_ovlRecv.m_pBuffer[m_ovlRecv.m_dwLeft];
	m_ovlRecv.m_wsaBuffer.len = MAX_PACKET_SIZE - m_ovlRecv.m_dwLeft;

	int ret = ::WSARecv( m_hSocket,                    // socket
		&m_ovlRecv.m_wsaBuffer,     // buffer pointer, size
		1,                          // lpBuffers �迭�� �ִ� WSABUF ����ü�� ��.
		&dwRead,                    // I/O �۾��� �����ڸ��� ������ ����Ʈ ��
		&dwFlag,                    // [in,out] Option Flag
		&m_ovlRecv,                 // struct LPWSAOVERLAPPED
		NULL);                      // Recv �Ϸ�� ȣ��� �Լ� ������

	if( SOCKET_ERROR == ret )
	{
		switch( ::GetLastError() ) {
		case WSA_IO_PENDING:    // the overlapped operation has been successfully initiated            
			return true;

		case WSAECONNRESET:     // The virtual circuit was reset by the remote side.
			OnSocketError();
			return false;

		default:
			OnSocketError();
			return false;
		}
	} // if

	// If no error occurs and the receive operation has completed immediately, WSARecv returns zero.
	if( ret == 0 )
	{        
		return true;
	}
	return false;
}

void KTCPSocket::OnSocketError()
{
	// ���� ���� ó��
}
