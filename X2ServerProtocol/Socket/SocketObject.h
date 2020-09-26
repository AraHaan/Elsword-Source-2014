#pragma once

// 060116. florist. ���� ������ event ��ü�� �����ϵ��� �Ǿ��ִ�.
// �̸� ���� �����ϰ� ���� �� ��, KSockBase�� rename�ؼ�
// ��� ���� �۾��� Ȱ�� �����ϵ��� Ȯ���� ������ ���� �� ����.
// (Ȯ�� : IO�� ���� ����, TCP/UDP ���� ����, ���� socket option ���� ����...)

SmartPointer( KSocketObject );

class KSocketObject : public KSimObject
{
public:
	KSocketObject(const KSocketObject& right);              // do not use (no implement.)
	KSocketObject& operator=(const KSocketObject& right);   // do not use (no implement.)

	KSocketObject(void);
	virtual ~KSocketObject(void);

public:
	void CloseSocket();
	bool Connect( const char* szIP = NULL, 
		unsigned short usPort = 0, 
		bool bUseIocp = true,
		std::vector< std::pair<int,int> >* vecOpt = NULL );
	bool SendData( const char* szData, int iSize );
	bool InitRecv();
	void OnIOCompleted( KOVERLAPPED::ENUM_IO_MODE eMode ); // IOCP�� ������� �ʴ� ���.
	void OnSendCompleted( DWORD dwTransfered );

	// ������ ������ �ּҸ� �̸� ������ �� ���� �ְ�, Connect �Լ� ȣ��ÿ� ������ �� ���� �ִ�.
	void SetRemoteIP( const char* szIP ) { m_sockaddr.sin_addr.s_addr   = inet_addr( szIP ); }
	void SetRemotePort( u_short usPort ) { m_sockaddr.sin_port          = htons( usPort ); }
    void SetSocketInfo( SOCKET socket, const SOCKADDR_IN& sockAddr ) { m_sock = socket; m_sockaddr = sockAddr; }

	virtual void OnRecvCompleted( DWORD dwTransfered ) = 0;
    virtual void OnSocketError();
	virtual void OnAcceptConnection() {} // accept ���ڸ��� ȣ��.

	bool    IsConnected() const     { return m_sock != INVALID_SOCKET; }
	bool    IsSending() const       { return m_ovlSend.m_bSending; }
	void    SetKey( DWORD dwKey )   { m_dwKey = dwKey; }
	DWORD   GetKey() const          { return m_dwKey; }
	void    SetIOEvent( KOVERLAPPED::ENUM_IO_MODE eMode, HANDLE hEvent );   // �ܺο��� ������ �̺�Ʈ�� ����.
	BOOL    CancelIo()              { return ::CancelIo( (HANDLE)m_sock ); }
    void    SetSendQueueSize( int nSize )   { m_kSendQueue.Resize( nSize ); }

	unsigned int GetIP() const			{ return m_sockaddr.sin_addr.S_un.S_addr; }
	const char* GetIPStr() const		{ return inet_ntoa(m_sockaddr.sin_addr); }
	unsigned short GetPort() const		{ return ntohs(m_sockaddr.sin_port); }

    static void DumpSendQueueLog( std::wostream& stm ) { ms_kMaxSizeInfo.Dump( stm ); }
	int GetDisconnectReason() { return m_iDisconnectReason; }
	void SetDisconnectReason( int iDisconnectReason );
	SOCKET&			GetSocket(){return m_sock;}
	KOVERLAPPED&	GetovlRecv(){return m_ovlRecv;}
	KOVERLAPPED&	GetovlSend(){return m_ovlSend;}
protected:
    inline void MovDataQue2Buf();   // send-queue�� ������� �����Ͱ� �ִٸ� ���۷� �ű��.

protected:
	DWORD               m_dwKey;
	SOCKADDR_IN         m_sockaddr;
	KOVERLAPPED			m_ovlRecv;
	KOVERLAPPED			m_ovlSend;
	SOCKET				m_sock;
    CRITICAL_SECTION    m_csSock;

    KCircularQueue      m_kSendQueue;
    CRITICAL_SECTION    m_csSendQueue;

	// socket object ���� ����
	int					m_iDisconnectReason;

    static struct KMaxSizeInfo {
        mutable KncCriticalSection  m_cs;
        int                         m_iMaxDataSize;
        int                         m_iQueueSize;
        CTime                       m_cTime;
        void Check( int iDataSize, int iQueueSize );
        void Dump( std::wostream& stm );
    } ms_kMaxSizeInfo;
	friend class KIocp;
};

//#define KNC_DONT_USE_IOCP