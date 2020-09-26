#pragma once
#include "SocketObject.h"

SmartPointer( KSession );
SmartPointer( KSkSession );

class KSkSession : public KSocketObject
{
public:
    void OnRecvCompleted( DWORD dwTransfered );
    void OnSocketError();
    void OnAcceptConnection();
    KSessionPtr GetSessionPtr();

    boost::weak_ptr< KSession > m_pkSession;
};

class KSession : public KPerformer, public KThread
{
	NiDeclareRTTI;
	DeclToStringW;

	KSession(); // �����δ� ����. ���Ǹ� �ȵȴ�. (����ϸ� ��ũ������.)
	KSession(const KSession& right);
	KSession& operator=(const KSession& right);


public:
	enum
    { 
		SECURE_KEY_WAITING_TIME     = 5000, // ���ӽ� ����Ű�� �ޱ���� ����ϴ� �ִ�ð�
	};

	KSession( bool bIsProxy );
	virtual ~KSession( void );

    virtual bool Init( bool bUseIocp );

	// derived from KThread
	virtual bool Begin();

	// derived from KPerformer
    virtual void Tick();    // reserve destroy, check heart beat

    void ReserveDestroy();  // OnDestory ����

	// 060321. florist. RefCount�� �����־� �������� ���� ���ɼ��� �ִ� ��츦 �˻��Ѵ�.
	virtual bool CheckExceedRefCount( int nCount );

	template < typename T > 
    bool SendPacket( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = true );
//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//	bool SendPacket( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID, char* pBuffer, int iBufferSize );
//#endif//X2OPTIMIZE_TCP_RELAY_TEST
    bool SendPacket( IN const KEvent& kEvent );
    bool SendID( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID );
	//bool InitRecv();

    bool IsConnected() const			{ _JIF( m_spSockObj, return false ); return m_spSockObj->IsConnected(); }
    unsigned int GetIP() const			{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetIP(); }
    const char* GetIPStr() const		{ _JIF( m_spSockObj, return NULL ); return m_spSockObj->GetIPStr(); }
    unsigned short GetPort() const		{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetPort(); }
    KSkSessionPtr GetSockObj()          { return m_spSockObj; }
    bool GetKey( DWORD& dwKey ) const   { _JIF( m_spSockObj, return false ); dwKey = m_spSockObj->GetKey(); return true; }
    bool SetKey( DWORD dwKey )          { _JIF( m_spSockObj, return false ); m_spSockObj->SetKey( dwKey ); return true; }

	static void DumpMaxSendData( std::wostream& stm );
	static void ResetMaxSendData();

	// use proxy-child only.
	virtual bool Connect( const char* szIP, unsigned short usPort );

	// user unproxy-child only.
	virtual void OnAcceptConnection(); // accept ���ڸ��� ȣ��. �̰����� ����Ű�� ������ Ŭ���̾�Ʈ�� �����Ѵ�.

	void			SetP2PIP( std::wstring wstrIP ){ m_wstrIP = wstrIP; }
	std::wstring	GetP2PIP(){ return m_wstrIP; }
	void	SetP2PPort( int nPort ){ m_nPort = nPort; }
	int		GetP2PPort(){ return m_nPort; }
    void SetSendQueueSize( int nSize ) { JIF( m_spSockObj ); m_spSockObj->SetSendQueueSize( nSize ); }	

	// Disconnect Reason Function
	int GetDisconnectReason() { _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetDisconnectReason(); }
	void SetDisconnectReason( int iDisconnectReason ) { JIF( m_spSockObj ); m_spSockObj->SetDisconnectReason( iDisconnectReason ); }
	
	//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	KncSecurity::KSADatabase* GetSADatabase();
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

protected:
    // util function
	//void OnSendCompleted( DWORD dwTransfered );
    //void OnRecvCompleted( DWORD dwTransfered );
    void OnRecvCompleted( IN const KncSecurity::KByteStream& payload );
    // derived from KThread
    virtual void Run();
    // derive to child.
    virtual void OnSocketError();   ///< 050129. WSASend, IOCP��� ���� ������ �߰��� ��� ȣ��.
    virtual void OnDestroy();       ///< 050129. multi thread safe�ϵ��� ������
	virtual void OnAuthenticFailed(); // ��Ŷ ������ ���� ������ ȣ��Ǵ� �����Լ�, ���� ������ ������ �Ͽ� ����Ұ�!

protected:
    KSkSessionPtr       m_spSockObj;
    bool                m_bUseIocp;     ///< if use Iocp
    bool				m_bIsProxy;		///< Proxy�� ���, H.B�� ������, Connect�� �� �� �ִ�.
    DWORD               m_dwHBTick;     // heart bit tick    
    bool                m_bDestroyReserved;     ///< 050129. ������ �����忡�� �Ҹ� ó��.

    static struct KMaxPacketInfo {
        KncCriticalSection  m_cs;           // lua script������ �۾��� ���� ��ȣ�Ѵ�
        std::wstring        m_strName;
        std::wstring        m_strDataDesc;
        size_t              m_nDataSize;    // ���� ���� �����͸� ����� ������
        size_t              m_nPacketSize;  // ��ȣȭ ó���� ��ģ ��, ���� ���۵Ǵ� ������ ������
        CTime               m_cTime;
    } ms_kMaxPacketInfo;

    // proxy-child only.
    enum ENUM_KEventType {    
        EVENT_DISCONNECTED,
        EVENT_RECV_COMPLETED,        
        EVENT_SEND_COMPLETED,        
        EVENT_MAX_VALUE
    };
    HANDLE              m_hEvents[EVENT_MAX_VALUE];

    friend class KIOThread;         // access On(Recv/Send)oCompleted()
    friend class KSkSession;        // m_nSPIndex, OnRecvCompleted, OnSocketError

    bool                m_bAuthKeyRecved;   // ���� Ű �޾Ҵ��� Ȯ�� �ϴ� ����.
    SpiType             m_nSPIndex;
    bool                m_bCheckHBTick;     // florist. debug�� ���Ǹ� ���� hb üũ on/off�� �����ϰ� ��.

	//p2p����� ���� ip/port
	std::wstring					m_wstrIP;
	int								m_nPort;	

	//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	KncSecurity::KSADatabase*					m_pSADatabase;
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

};

template < typename T > 
bool KSession::SendPacket( DWORD dwDestPI_, UidType nTo_, UidType anTrace_[], unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_ )
{
    KEvent kEvent;
    kEvent.SetData( dwDestPI_, anTrace_, usEventID_, data_ );
    LIF( kEvent.m_kDestPerformer.AddUID( nTo_ ) );

    if( bCompress_ ) kEvent.m_kbuff.Compress();   // ������ ���, KEvent�� buff���� �����Ѵ�.

    // send packet
    if( ! KSession::SendPacket( kEvent ) )
        return false;

#ifdef _IN_HOUSE_
	if( g_bIsSERVICE == false )
	{
		if( bLogging_ )
			dbg::clog << L"�� " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;
	}
#endif

    return true;
}