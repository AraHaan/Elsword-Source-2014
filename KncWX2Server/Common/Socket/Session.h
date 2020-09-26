#pragma once
#include "ServerDefine.h"
#include <WinSock2.h>
#include "Performer.h"
#include "Thread/Thread.h"
#include "SocketObject.h"
#include "KncSecurity/KncSecurity.h"
#include "KncSend.h"
//#include "ServerDefine.h"

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
        SECURE_KEY_WAITING_TIME     = 5000,		// ���ӽ� ����Ű�� �ޱ���� ����ϴ� �ִ�ð�
		PACKET_AUTH_FAIL_DEFAULT	= 100,		// ��Ŷ ĸ�� ���� �Ǵ� ���� default��
    };

	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	enum SESSION_TYPE
	{
		SST_NONE = 0,
		SST_SERVER,
		SST_CLIENT,
	};
#endif SERV_KEVENT_FROM
	//}}

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
    bool SendPacket( IN const KEvent& kEvent );
    bool SendID( DWORD dwDestPI, UidType nTo, UidType anTrace[], unsigned short usEventID );

    bool IsConnected() const			{ _JIF( m_spSockObj, return false ); return m_spSockObj->IsConnected(); }
    unsigned int GetIP() const			{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetIP(); }
    const char* GetIPStr() const		{ _JIF( m_spSockObj, return NULL ); return m_spSockObj->GetIPStr(); }
    unsigned short GetPort() const		{ _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetPort(); }
    KSkSessionPtr GetSockObj()          { return m_spSockObj; }
    bool GetKey( DWORD& dwKey ) const   { _JIF( m_spSockObj, return false ); dwKey = m_spSockObj->GetKey(); return true; }
    bool SetKey( DWORD dwKey )          { _JIF( m_spSockObj, return false ); m_spSockObj->SetKey( dwKey ); return true; }

    static void DumpMaxSendData( std::wostream& stm );
    static void ResetMaxSendData();
	//{{ 2009. 10. 18  ������	SequenceNum
	static void DumpPacketAuthInfo( std::wostream& stm );
	//}}

    // use proxy-child only.
    virtual bool Connect( const char* szIP, unsigned short usPort );

    // user unproxy-child only.
    virtual void OnAcceptConnection(); // accept ���ڸ��� ȣ��. �̰����� ����Ű�� ������ Ŭ���̾�Ʈ�� �����Ѵ�.

	void			SetP2PIP( const std::wstring& wstrIP );//{ m_wstrIP = wstrIP; }
	std::wstring	GetP2PIP(){ return m_wstrIP; }
	void	SetP2PPort( int nPort ){ m_nPort = nPort; }
	int		GetP2PPort(){ return m_nPort; }
    void SetSendQueueSize( int nSize ) { JIF( m_spSockObj ); m_spSockObj->SetSendQueueSize( nSize ); }

	//{{ 2013. 1. 8	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	void			SetP2PInternalIP( const std::wstring& wstrIP ); //{ m_wstrInternalIP = wstrIP; }
	std::wstring	GetP2PInternalIP(){ return m_wstrInternalIP; }
	void			SetP2PInternalPort( USHORT usPort ){ m_usInternalPort = usPort; }
	USHORT			GetP2PInternalPort(){ return m_usInternalPort; }
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	// Disconnect Reason Function
	int GetDisconnectReason() { _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetDisconnectReason(); }
	void SetDisconnectReason( int iDisconnectReason ) { _JIF( m_spSockObj, return ); m_spSockObj->SetDisconnectReason( iDisconnectReason ); }

	//{{ 2009. 9. 1  ������		IOCP�������
	DWORD GetLastIocpFailedError() { _JIF( m_spSockObj, return 0 ); return m_spSockObj->GetLastIocpFailedError(); }
	//}}
	//{{ 2009. 10. 18  ������	Sequence Number
	void InitCheckSequenceNumInfo( bool bIsCheckSequenceNum, u_int uiLimitCount ) { m_bCheckSequenceNum = bIsCheckSequenceNum; m_uiPacketAuthFailLimit = uiLimitCount; }	
	u_int GetPacketAuthFailLimit() const			{ return m_uiPacketAuthFailLimit; }
	bool IsCheckSequenceNum()						{ return m_bCheckSequenceNum; }
	void IncreasePacketAuthFailCnt()				{ KLocker lock( m_csPacketAuthFailCnt ); ++m_uiPacketAuthFailCnt; }
	unsigned int GetPacketAuthFailCnt()				{ KLocker lock( m_csPacketAuthFailCnt ); return m_uiPacketAuthFailCnt; }
	//}}

	//{{ 2009. 8. 10  ������	Send Buffer Full
	unsigned short GetLastSendEventID() { return m_usLastSendEventID; }
	//}}
	//{{ 2009. 9. 29  ������	��Ʈ��Ʈ
	DWORD GetHBTick() { return m_dwHBTick; }
	//}}
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	SESSION_TYPE GetSessionType() const { return m_eSessionType; }
	void SetSessionType( const SESSION_TYPE eType ) { m_eSessionType = eType; }
#endif SERV_KEVENT_FROM
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

	//{{ 2009. 10. 18  ������	SequenceNum	
	static struct KPacketAuthInfo {
		KncCriticalSection  m_cs;			// lua script������ �۾��� ���� ��ȣ�Ѵ�
		std::wstring        m_strName;
		unsigned int		m_uiMaxFailCnt;
		unsigned int		m_uiTotalFailCnt;		
		CTime               m_cRegTime;
	} ms_kPacketAuthInfo;
	//}}

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
	//{{ 2009. 10. 18  ������	Sequence Number
	bool				m_bCheckSequenceNum;
	unsigned int		m_uiPacketAuthFailLimit;
	unsigned int		m_uiPacketAuthFailCnt;
	KncCriticalSection  m_csPacketAuthFailCnt;
	//}}

	//p2p����� ���� ip/port
	std::wstring		m_wstrIP;
	int					m_nPort;

	//{{ 2013. 1. 8	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	std::wstring		m_wstrInternalIP;
	USHORT				m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	//{{ 2009. 8. 10  ������	send buffer full����ã��
	unsigned short		m_usLastSendEventID;
	//}}

	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	SESSION_TYPE		m_eSessionType;
#endif SERV_KEVENT_FROM
	//}}
	//{{ 2011. 10. 31	������	��Ʈ��Ʈ ���� ���� �м�
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	DWORD				m_dwHBTickDump;
	DWORD				m_dwCurrentTickDump;
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	//}}

#ifdef SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL// �۾���¥: 2013-09-23// �ڼ���
	typedef std::pair< unsigned short, CTime >		TYPE_PAIR_USHORT_CTIME;
	typedef	std::queue< TYPE_PAIR_USHORT_CTIME >	TYPE_SEND_BUFFER_FULL_LOG;
	TYPE_SEND_BUFFER_FULL_LOG	m_queEventID;
#endif // SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL
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

    if( bLogging_ )
        dbg::clog << L"�� " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

    return true;
}