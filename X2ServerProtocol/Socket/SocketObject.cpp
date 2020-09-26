#include "StdAfx.h"

#include "SocketObject.h"

//#ifdef KNC_USE_IOCP
#include "Iocp.h"
//#endif
KSocketObject::KMaxSizeInfo KSocketObject::ms_kMaxSizeInfo = {
    KncCriticalSection(), 0, 0, CTime::GetCurrentTime()
};

void KSocketObject::KMaxSizeInfo::Check( int iDataSize, int iQueueSize )
{
    KLocker lock( m_cs );

    if( iDataSize > m_iMaxDataSize )
    {
        m_iMaxDataSize  = iDataSize;
        m_iQueueSize    = iQueueSize;
        m_cTime         = CTime::GetCurrentTime();

        START_LOG( cout, L"Send-Queue log update." )
            << BUILD_LOG( m_iMaxDataSize )
            << BUILD_LOG( m_iQueueSize )
            << BUILD_LOG( (LPCTSTR)m_cTime.Format( KNC_TIME_FORMAT ) );
    }

}

void KSocketObject::KMaxSizeInfo::Dump( std::wostream& stm )
{
    KLocker lock( m_cs );

    stm << L" -> Send Q. log " << std::endl
        << TOSTRINGW( m_iMaxDataSize )
        << TOSTRINGW( m_iQueueSize )
        << TOSTRINGW( (LPCTSTR)m_cTime.Format( KNC_TIME_FORMAT ) );
}

KSocketObject::KSocketObject() : m_sock( INVALID_SOCKET ), m_dwKey( 0 )
{
	m_sockaddr.sin_family   = AF_INET;

	m_ovlRecv.m_eIOMode     = KOVERLAPPED::IO_RECV;
	m_ovlSend.m_eIOMode     = KOVERLAPPED::IO_SEND;

	m_ovlRecv.hEvent        = NULL;
	m_ovlSend.hEvent        = NULL;

	m_iDisconnectReason		= 0;

    ::InitializeCriticalSection( &m_csSock );
    ::InitializeCriticalSection( &m_csSendQueue );
}

void KSocketObject::SetIOEvent( KOVERLAPPED::ENUM_IO_MODE eMode_, HANDLE hEvent_ )
{
	// 060227. florist. ������ GetIOEvent��� �Լ��� ������ ���ο��� �̺�Ʈ ��ü�� �����߾���.
	// ������ WaitFor...�ϴ� �κ��� SocketObject �ܺο� �ֱ� ������, ����ó���� �����尡 ���� �� ��
	// �̺�Ʈ�� ���� ���������� ������ �����. �̶����� �ڵ尡 �� ��������� �����尡 �ִ� �ڵ忡��
	// �̺�Ʈ ��ü�� ���� & �����ϵ��� �����Ѵ�. SetIOEvent�� �ܺο��� ������ Event�� �޾ƿ��⸸ �Ѵ�.

	LPWSAOVERLAPPED povl    = NULL;

	switch( eMode_ )
	{
	case KOVERLAPPED::IO_SEND: povl = &m_ovlSend;   break;
	case KOVERLAPPED::IO_RECV: povl = &m_ovlRecv;   break;
	default:
		return;
	}

	povl->hEvent = hEvent_;
}


KSocketObject::~KSocketObject(void)
{
    { // locking scope
        KLocker lock( m_csSock );

        CLOSE_SOCKET( m_sock );
    } // locking scope

    DeleteCriticalSection( &m_csSock );
    DeleteCriticalSection( &m_csSendQueue );
}

void KSocketObject::CloseSocket()
{
    KLocker lock( m_csSock );
    
    CLOSE_SOCKET( m_sock );
}

bool KSocketObject::Connect( const char* szIP_, 
							unsigned short usPort_, 
							bool bUseIocp_,
							std::vector< std::pair<int,int> >* vecOpt_ )
{
    KLocker lock( m_csSock );
	if( IsConnected() )
	{
		START_LOG( cwarn, L"Already Connected. Key : " << m_dwKey );
		return true;
	}

	int            ret;   

	m_sock = ::WSASocket( AF_INET, 
		SOCK_STREAM, 
		IPPROTO_TCP, 
		NULL,
		0, 
		WSA_FLAG_OVERLAPPED ); // overlapped I/O

	if( m_sock == INVALID_SOCKET )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;
		return false;
	}

	if( vecOpt_ != NULL )
	{
		int sock_opt = 1;
		std::vector< std::pair<int,int> >::iterator vit;
		for( vit = vecOpt_->begin(); vit != vecOpt_->end(); vit++ )
		{
			if( ::setsockopt( m_sock, vit->first, vit->second, (char *)&sock_opt,	sizeof (sock_opt) ) == SOCKET_ERROR )
			{
				START_LOG( cerr, GET_WSA_MSG )
					<< BUILD_LOG( WSAGetLastError() )
					<< END_LOG;
				return false;
			}            
		}
	}

    if( szIP_ )         SetRemoteIP( szIP_ );
    if( usPort_ != 0 )  SetRemotePort( usPort_ );

    ret = ::connect( m_sock, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr) );

    if( SOCKET_ERROR == ret )
    {
        // MUST: ��Ȥ �ٸ� ��(ȸ�ǽ�)���� WSAENOBUFS�� Error�� ������ ��찡 �����Ѵ�.
        // TCP�� buffer space�� ���ڶ� ����� �� ����.
        // client�� 100�� �̸� �ڿ� 5�� ������ �� error�� �߻���Ų��.
        // ���߿� �����Ǿ�� �� �� �̴�.
        START_LOG( cerr, GET_WSA_MSG )
            << BUILD_LOG( WSAGetLastError() )
            << BUILD_LOG( m_dwKey )
            << BUILD_LOG( szIP_ )
            << BUILD_LOG( usPort_ )
            << END_LOG;

        CLOSE_SOCKET( m_sock );
        return false;
    }

    KSocketObjectPtr spSockObj = GetThisPtr< KSocketObject >();
    if( bUseIocp_ )
    {
        //#ifndef KNC_DONT_USE_IOCP
        _JIF( SiKIocp()->AssociateSocket( spSockObj ), return false );
        //#endif
    }

    _JIF( spSockObj->InitRecv(), return false );

    return  true;
}

void KSocketObject::OnSocketError()
{
    START_LOG( cout, L"pure-virtual function called." );
}

bool KSocketObject::InitRecv()
{
    KLocker lock( m_csSock );

	if( m_sock == INVALID_SOCKET ) return false;

	DWORD dwRead = 0;
	DWORD dwFlag = 0;    

	// Note: Win2000������ �Ʒ��� WSARecv() ��� ReadFile()�� ����� �� �ִ�.
	//       Win98������ ReadFile()�� ����� �� ����.

	m_ovlRecv.Clear();

	m_ovlRecv.m_wsaBuffer.buf = &m_ovlRecv.m_pBuffer[m_ovlRecv.m_dwLeft];
	m_ovlRecv.m_wsaBuffer.len = MAX_PACKET_SIZE - m_ovlRecv.m_dwLeft;

	int ret = ::WSARecv( m_sock,                    // socket
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
			START_LOG( cwarn, L"The virtual circuit was reset by the remote side." );
			OnSocketError();
			return false;

		default:
			START_LOG( cerr, L"WSARecv() Failed." )
				<< BUILD_LOG( m_dwKey )
				<< BUILD_LOG( GET_WSA_MSG )
				<< BUILD_LOG( ret );
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

void KSocketObject::OnIOCompleted( KOVERLAPPED::ENUM_IO_MODE eMode_ )
{
	DWORD dwTransfered      = 0;
	DWORD dwFlag            = 0;
	LPWSAOVERLAPPED povl    = NULL;

	switch( eMode_ )
	{
	case KOVERLAPPED::IO_SEND: povl = &m_ovlSend;   break;
	case KOVERLAPPED::IO_RECV: povl = &m_ovlRecv;   break;
	default:
		return;
	}

    { // locking scope

        KLocker lock( m_csSock );

        ::WSAGetOverlappedResult( m_sock, povl, &dwTransfered, FALSE, &dwFlag );

    } // locking scope
	if( eMode_ == KOVERLAPPED::IO_SEND )
		OnSendCompleted( dwTransfered );
	else
		OnRecvCompleted( dwTransfered );
}

void KSocketObject::MovDataQue2Buf()
{
    KLocker lock( m_csSock );
    KLocker lock2( m_csSendQueue );

    int nSize = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
    nSize = m_kSendQueue.Pop( nSize, &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft] );
    m_ovlSend.m_dwLeft += nSize;
}


void KSocketObject::OnSendCompleted( DWORD dwTransfered_ )
{
    KLocker lock( m_csSock );

	if( m_sock == INVALID_SOCKET ) return;

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
    int ret = ::WSASend( m_sock,    // socket
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

#ifdef _IN_HOUSE_
	START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG );
#endif

	OnSocketError();

	return;
}

bool KSocketObject::SendData( const char* szData_, int iSize_ )
{
    KLocker lock( m_csSock );
	_JIF( m_sock != INVALID_SOCKET, return false );    // ������ ��ȿ���� ����.
	_JIF( iSize_ > 0, return false );                  // 0 byte ���� �õ��� ����ó��.
	_JIF( szData_ != NULL, return false );

    MovDataQue2Buf(); // send-queue�� ������� �����Ͱ� �ִٸ� ���۷� �ű��.

    // input data�� ó�� - 1. ���ۿ� �� ������ �ִٸ� �켱 ���� �õ�.
    int iFreeSpace = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
    if( iFreeSpace > 0 )
    {
        int iCopySize = min( iFreeSpace, iSize_ );
        ::memcpy( &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft], szData_, iCopySize );
        m_ovlSend.m_dwLeft += iCopySize;
        szData_ += iCopySize;
        iSize_ -= iCopySize;
    }

    // input data�� ó�� - 2. ���� �����Ͱ� ���� �ִٸ� SendQueue�� ���� �õ�.
    if( iSize_ > 0 )
    {
        KLocker lock2( m_csSendQueue );

        iSize_ -= m_kSendQueue.Push( szData_, iSize_ );

        ms_kMaxSizeInfo.Check( m_kSendQueue.GetDataSize(), m_kSendQueue.size() );
    }

    // input data�� ó�� - 3. �׷��� ���� �������� ���� �����Ͱ� ���� �ִٸ� buffer full.
    if( iSize_ > 0 )
    {
#ifdef _IN_HOUSE_
        START_LOG( cerr, L"Send Buffer Full. data size : " << iSize_
			<< L", left : " << m_ovlSend.m_dwLeft
			<< L", bSending : " << m_ovlSend.m_bSending );
#endif

		// �����͸� ������ �ʾ���. �ٽ� �������� SendPakcet �ѹ� �� ȣ���ؾ� ��.
		m_ovlSend.m_dwLeft = 0;
		m_ovlSend.m_bSending = false;

		OnSocketError();

		return false;
	}

	// ���������� ������ �ٽ��ѹ� WsaSend�� ȣ�����ش�.
	if( m_ovlSend.m_bSending == false )
	{
		DWORD dwWrite = 0;
		m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

		int ret = ::WSASend( m_sock,                     // socket
			&m_ovlSend.m_wsaBuffer,     // ���� ������
			1,                          // WSABUF�� ����
			&dwWrite,                   // [out] ��� ���۵� ��
			0,                          // Flag
			&m_ovlSend,                 // OVERLAPPED
			NULL );

		//std::cout << "raw length : " << kbuff_.GetLength() << std::endl << "SEND " << bsbuff << std::endl;

		if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // ���� pending ��..
		{
			m_ovlSend.m_bSending   = true;            
			return true;
		} 

		if( ret != SOCKET_ERROR ) // ȣ�� ���� ���� �� ����.
		{
			m_ovlSend.m_bSending   = true;
			return true;
		}

#ifdef _IN_HOUSE_
        START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG << ret );
#endif

		OnSocketError();
		return false;
	}

	return true;
}