#pragma once

#include <KNCSingleton.h>
#include <Thread/KCSLockType.h>
#include "ThreadManager.h"
#include "NexonBillingTCPPacket.h"
//{{ 2011. 12. 07	������	���� ��ũ��Ʈ �Ŵ���
#ifdef SERV_BILLING_SCRIPT_MANAGER
	#include "BillingScriptManager.h"
#endif SERV_BILLING_SCRIPT_MANAGER
//}}
//{{ 2013. 01. 10	������	���� TCP ��Ŷ ũ�� ����ó��
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
	#include <VariableSizeBuffer.h>
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//}}

class KNexonBillingTCPManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KNexonBillingTCPManager );

public:
	//{{ 2009. 12. 14  ������	�����ڵ�����
	enum NEXON_BILLING_ENUM
	{
		AUTO_PAYMENT_TYPE = 13002,
	};
	//}}

	//{{ 2010. 04. 26  ������	PC�� ���� ĳ����
	enum CASH_ITEM_LIST_TYPE
	{
		CILT_NORMAL = 0,
		CILT_PC_BANG,
        
		CILT_MAX,
	};
	//}}

	//{{ 2012. 02. 21	��μ�	ĳ�ü� ��Ŷ ������ ����
#ifdef SERV_QUICK_CASH_SHOP
	enum NEXON_BILLING_COUNT
	{
		NBC_CASH_ITEM_MAX = 200,
	};
#endif SERV_QUICK_CASH_SHOP
	//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-20	// �ڼ���
	enum NEXON_CLIENT_TYPE
	{
		NCT_GAME_CLIENT			= 1,
		NCT_WEB					= 2,
		NCT_GAME_CLIENT_CAFE	= 3,
		NCT_EWEB_CAFE			= 4,
	};
#endif // SERV_NEXON_COUPON_SYSTEM

	KNexonBillingTCPManager();
	virtual ~KNexonBillingTCPManager();

	void RegToLua();

	void InitNexonBillingTCPInfo(
		const char*     szNexonAuthIP,
		unsigned short  usPort,
        int             iDomain );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread( DWORD dwTimeOut = 10000 );

    void QueueingSendPacket( const KNexonBillingTCPPacketPtr& spPacket );

    __int64 GetReleaseTick();
    bool SetReleaseTick( const __int64& iReleaseTick );

    unsigned long GetNextPacketNo();
    unsigned long GetNextPurchaseNo();

    void InsertPacketNoUserUID( unsigned long ulPacketNo, UidType iUserUID );
    void DeletePacketNoUserUID( unsigned long ulPacketNo );
    UidType GetCorrespondingUserUID( unsigned long ulPacketNo );

    void AddProductInfo( KNXBTProductInfo kInfo );
    int GetItemID( unsigned long ulProductNo );

	//{{ 2010. 01. 28  ������	��������ó��
	int GetProductPageNumber();
	//}}
    int GetNextProductPageNumber();
    void BuildProductPage();
    bool GetProductPage( IN int iPage, IN bool bIsPcBang, OUT std::vector< KNXBTProductInfo >& vecProductInfo );

	//{{ 2010. 01. 14  ������	���� ����
	int GetTotalProductCount()								{ KLocker lock( m_csProductInfo ); return static_cast<int>(m_mapProductInfo.size()); }
	//}}
    bool GetProductInfo( unsigned long ulProductNo, KNXBTProductInfo& kProductInfo );
    bool GetTotalPrice( const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo, unsigned long& ulPrice );

    void HandleInitializeAck( bool bSucceed );
    void ResetProductPageNumber();
    void ResetProductInfo();

	//{{ 2008. 8. 13  ������	���� �ʱ�ȭ
	bool IsInitialized()									{ return m_bInitialized; }
	//}}

	//{{ 2009. 11. 21  ������	���������ӱ�������	
	bool IsFirstConnectSucc() { return m_bFirstConnectSucc; }
	void FirstConnectSucc() { m_bFirstConnectSucc = true; }
	//}}

	//{{ 2009. 6. 3  ������		������
	int	 GetBillingServerNo()								{ return m_kNexonBillingTCPInfo.m_iDomain; }
	//}}

	//{{ 2009. 10. 14  ������	�ڵ�������ǰ����Ʈ
	void GetAutoPaymentProductList( std::vector< unsigned long >& vecProductList );
	//}}

	//{{ 2010. 11. 22	������	û�� öȸ
#ifdef SERV_NX_BILLING_REFUND
	bool GetCashItemRefundRequestID( OUT std::wstring& wstrRequestID );
#endif SERV_NX_BILLING_REFUND
	//}}

	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	void GetCashProductList( OUT std::vector< KCashPruductInfo >& vecCashProductList ) const;
#endif SERV_CASH_ITEM_LIST
	//}}

	//{{ 2013. 01. 08	������	���� ��Ŷ ��ġ�� ���� ã��
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
	bool CheckPacketMaxSizeOverUser( IN const char* pBuffer, OUT UidType& iUserUID, OUT BYTE& bytePacketType, OUT std::wstring& wstrPacketTypeName );
#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
	//}}

	//{{ 2013. 01. 23	������	���� ��Ŷ ���� ó�� ���� ����
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	void BeginCheckPerformance();
	void EndCheckPerformance();
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	//}}

protected:
	void Recv();
	void Send();
	//{{ 2009. 11. 21  ������	���������ӱ�������
	void KeepConnectionThread();
	//}}
	//{{ 2010. 10. 13	������	�ؽ� ���� ���� ����	
	void OnDisconnectBillingServer( IN const std::wstring& wstrReason );
	//}}

	bool Connect();
	bool IsConnected();
	void KeepConnection();
	void CheckConnection();

    bool GetSendPacket( KNexonBillingTCPPacketPtr& spPacket );
    void MakeEventFromReceivedPacket();
    void DumpBuffer( const BYTE* buffer, bool bError );

public:

protected:
	//////////////////////////////////////////////////////////////////////////
	// �� ��� ������ �޷��ִ� �ּ��� ������ �����ϴ� Thread�� ����

	//{{ 2013. 01. 23	������	���� ��Ŷ ���� ó�� ���� ����
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	struct KPerformanceDump
	{
		DWORD				m_dwLastTickCount;
		DWORD				m_dwMinLatency;
		DWORD				m_dwMaxLatency;
		std::wstring		m_wstrMinRegDate;
		std::wstring		m_wstrMaxRegDate;
		
		KPerformanceDump()
		{
			m_dwLastTickCount	= 0;
			m_dwMinLatency		= UINT_MAX;
			m_dwMaxLatency		= 0;
		}
	};	
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	//}}

	struct KNexonBillingTCPInfo
	{
		std::string         m_strIP;
		unsigned short      m_usPort;
        int                 m_iDomain;
	} m_kNexonBillingTCPInfo;															// KeepConnectionThread

	DWORD				m_dwLastHeartBeatTick;											// KeepConnectionThread
	DWORD				m_dwLastConnectionCheckTick;									// KeepConnectionThread
	static DWORD		ms_dwHeartBeatGap;												// KeepConnectionThread
	static DWORD		ms_dwConnectionCheckGap;										// KeepConnectionThread

	SOCKET				m_sock;															// KeepConnectionThread, SendThread, RecvThread

	boost::shared_ptr< KTThread< KNexonBillingTCPManager > >    m_spThreadRecv;			// Server Init Thread
	boost::shared_ptr< KTThread< KNexonBillingTCPManager > >    m_spThreadSend;			// Server Init Thread
	//{{ 2009. 11. 21  ������	���������ӱ�������
	boost::shared_ptr< KTThread< KNexonBillingTCPManager > >    m_spThreadKeepConnect;	// Server Init Thread
	bool														m_bFirstConnectSucc;	// KeepConnectionThread, SendThread, RecvThread
	//}}

	KCSLOCK_DECLARE( std::queue< KNexonBillingTCPPacketPtr >,	m_kSendQueue );			// SendThread, TCPBillingProcessThread

    char                                                        m_cRecvBuffer[MAX_PACKET_SIZE_NBT]; // RecvThread
    unsigned long                                               m_ulRecvCP;				// KeepConnectionThread, RecvThread
    
	KCSLOCK_DECLARE( __int64,									m_iReleaseTick );		// KeepConnectionThread, TCPBillingProcessThread
	KCSLOCK_DECLARE( unsigned long,								m_ulCurrentPacketNo );	// KeepConnectionThread, TCPBillingProcessThread, SimLayerThread
	KCSLOCK_DECLARE( unsigned long,								m_ulCurrentPurchaseNo );// SimLayerThread
	KCSLOCK_DECLARE( std::map< unsigned long KCOMMA() UidType >, m_mapPacketNoUserUID );// TCPBillingProcessThread

    bool                                                        m_bInitialized;			// KeepConnectionThread, TCPBillingProcessThread
    boost::shared_ptr< void >                                   m_spEvent;				// KeepConnectionThread, TCPBillingProcessThread
	
    std::map< unsigned long, KNXBTProductInfo >                 m_mapProductInfo;		// TCPBillingProcessThread, SimLayerThread [����ȭ��ü����]
	//{{ 2012. 03. 21	��μ�		ĳ�� ��ǰ ����Ʈ ������ �������� DB�� ���
#ifdef SERV_CASH_ITEM_LIST_ADD
	std::map< unsigned long, KNXBTProductInfo >                 m_mapProductInfo_Excepted;		// TCPBillingProcessThread, SimLayerThread [����ȭ��ü����] ���� ���
#endif SERV_CASH_ITEM_LIST_ADD
	//}}
    std::vector< std::vector< KNXBTProductInfo > >              m_vecProductPage[CILT_MAX];	// TCPBillingProcessThread, SimLayerThread [����ȭ��ü����]
    mutable KncCriticalSection                                  m_csProductInfo;

	KCSLOCK_DECLARE( int,										m_ulCurrentProductPage );// TCPBillingProcessThread
	
	int															m_iBillingServerNo;		// Server Init Thread // ����������

	//{{ 2009. 10. 14  ������	�ڵ�������ǰ����Ʈ
	KCSLOCK_DECLARE( std::vector< unsigned long >,				m_vecAutoPaymentProductList );// SimLayerThread
	//}}
	//{{ 2010. 11. 22	������	û�� öȸ
#ifdef SERV_NX_BILLING_REFUND
	KCSLOCK_DECLARE( int,										m_iCashItemRefundRequestID );
#endif SERV_NX_BILLING_REFUND
	//}}
	//{{ 2011. 12. 07	������	���� ��ũ��Ʈ �Ŵ���
#ifdef SERV_BILLING_SCRIPT_MANAGER
	KBillingScriptManager										m_kBillingScriptManager;
#endif SERV_BILLING_SCRIPT_MANAGER
	//}}
	//{{ 2013. 01. 10	������	���� TCP ��Ŷ ũ�� ����ó��
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
	KVariableSizeBuffer											m_kRecvVariableBuffer;
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
	//}}
	//{{ 2013. 01. 23	������	���� ��Ŷ ���� ó�� ���� ����
#ifdef SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	KPerformanceDump											m_kRecvPerformanceDump;
#endif SERV_BILLING_PACKET_RECV_PERFORMANCE_CHECK
	//}}
};

DefSingletonInline( KNexonBillingTCPManager );


