//////////////////////////////////////////////////////////////////////////
//	PayletterBillling.h
//
//	Payletter ���� ���� �޴���
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <KNCSingleton.h>
#include <Thread/KCSLockType.h>
#include "ThreadManager.h"
#include "PayletterBillingPacket.h"

#ifdef SERV_COUNTRY_US

class KPayletterBillingManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KPayletterBillingManager );

public:
	KPayletterBillingManager();
	virtual ~KPayletterBillingManager();

	void RegToLua();

	void InitPayletterBillingInfo(
		const char*     szNexonAuthIP,
		unsigned short  usPort,
		int             iDomain );

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();

	bool Connect();

	bool IsConnected();
	void KeepConnection();
	void CheckConnection();

	void HandleInitializeAck( bool bSucceed );

	void OnDisconnectBillingServer( IN const std::wstring& wstrReason );


	void Recv();
	void Send();
	//{{ 2009. 11. 21  ������	���������ӱ�������
	void KeepConnectionThread();
	//}}

	//{{ 2009. 11. 21  ������	���������ӱ�������	
	bool IsFirstConnectSucc() { return m_bFirstConnectSucc; }
	void FirstConnectSucc() { m_bFirstConnectSucc = true; }
	//}}


	void QueueingSendPacket( const KPayletterBillingPacketPtr& spPacket );

	bool GetSendPacket( KPayletterBillingPacketPtr& spPacket );
	void MakeEventFromReceivedPacket();
	void DumpBuffer( BYTE* buffer, bool bError );

	void InsertPacketNoUserUID( unsigned long ulPacketNo, UidType iUserUID );
	void DeletePacketNoUserUID( unsigned long ulPacketNo );
	UidType GetCorrespondingUserUID( unsigned long ulPacketNo );

	bool IsInitialized()									{ return m_bInitialized; }

	unsigned long GetNextPacketNo();

	// �ְ� ���� �Լ�
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void InsertBuyPacket( IN const KEBILL_BUY_PRODUCT_REQ kPacket_, OUT KEBILL_BUY_PRODUCT_REQ& kPacketDeleted_, OUT bool& bRet_ );
#else //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void InsertBuyPacket( IN const KEBILL_BUY_PRODUCT_REQ kPacket_, OUT bool& bRet_ );
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void GetBuyPacket( IN const UidType iUserUID_, OUT KEBILL_BUY_PRODUCT_REQ& kPacket_, OUT bool& bRet_ );
	void EraseBuyPacket( IN const UidType iUserUID_ );

	// �ְ� ���� �Լ�-�����ϱ�
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void InsertGiftPacket( IN const KEBILL_GIFT_ITEM_REQ kPacket_, OUT KEBILL_GIFT_ITEM_REQ& kPacketDeleted_, OUT bool& bRet_ );
#else //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void InsertGiftPacket( IN const KEBILL_GIFT_ITEM_REQ kPacket_, OUT bool& bRet_ );
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void GetGiftPacket( IN const UidType iUserUID_, OUT KEBILL_GIFT_ITEM_REQ& kPacket_, OUT bool& bRet_ );
	void EraseGiftPacket( IN const UidType iUserUID_ );

	// �ְ� ���� �Լ�-����
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void InsertCouponPacket( IN const KEBILL_USE_COUPON_REQ kPacket_, OUT KEBILL_USE_COUPON_REQ& kPacketDeleted_, OUT bool& bRet_ );
#else //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void InsertCouponPacket( IN const KEBILL_USE_COUPON_REQ kPacket_, OUT bool& bRet_ );
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
	void GetCouponPacket( IN const UidType iUserUID_, OUT KEBILL_USE_COUPON_REQ& kPacket_, OUT bool& bRet_ );
	void EraseCouponPacket( IN const UidType iUserUID_ );

	// 

protected:

	struct KPayletterBillingInfo
	{
		std::string         m_strIP;
		unsigned short      m_usPort;
		int                 m_iDomain;
	} m_kPayletterBillingInfo;

	DWORD				m_dwLastHeartBeatTick;											// KeepConnectionThread
	DWORD				m_dwLastConnectionCheckTick;									// KeepConnectionThread
	static DWORD		ms_dwHeartBeatGap;												// KeepConnectionThread
	static DWORD		ms_dwConnectionCheckGap;										// KeepConnectionThread

	SOCKET				m_sock;

	boost::shared_ptr< KTThread< KPayletterBillingManager > >    m_spThreadRecv;			// Server Init Thread
	boost::shared_ptr< KTThread< KPayletterBillingManager > >    m_spThreadSend;			// Server Init Thread
	//{{ 2009. 11. 21  ������	���������ӱ�������
	boost::shared_ptr< KTThread< KPayletterBillingManager > >    m_spThreadKeepConnect;	// Server Init Thread
	bool														m_bFirstConnectSucc;	// KeepConnectionThread, SendThread, RecvThread
	//}}

	KCSLOCK_DECLARE( std::queue< KPayletterBillingPacketPtr >,	m_kSendQueue );			// SendThread, TCPBillingProcessThread

	bool                                                        m_bInitialized;			// KeepConnectionThread, TCPBillingProcessThread
	boost::shared_ptr< void >                                   m_spEvent;				// KeepConnectionThread, TCPBillingProcessThread

	char                                                        m_cRecvBuffer[MAX_PACKET_SIZE_PLB]; // RecvThread
	unsigned long                                               m_ulRecvCP;				// KeepConnectionThread, RecvThread

	bool														m_bSocketConnect;		// ���� : ó�� ���� ���� ����� üũ

	KCSLOCK_DECLARE( unsigned long,								m_ulCurrentPacketNo );	// KeepConnectionThread, TCPBillingProcessThread, SimLayerThread
	KCSLOCK_DECLARE( std::map< unsigned long KCOMMA() UidType >, m_mapPacketNoUserUID );// TCPBillingProcessThread

	// ���� : ������ ���� �� ���� ��Ŷ �־�δ� ��.
	//		  TCP ����̶� ��ť�� �ȳ�����, ���� �� ������ ������ ���� �� �� �ʿ䰡 �ִ� ��.
	//		  Ű�� ����UID �� ����Ѵ�.
	
	struct KPayletterBuyPacket
	{
		CTime m_ctTime;
		KEBILL_BUY_PRODUCT_REQ m_kPacket;

		KPayletterBuyPacket( CTime ctTime_, KEBILL_BUY_PRODUCT_REQ kPacket_ )
		{
			m_ctTime = ctTime_;
			m_kPacket = kPacket_;
		}
	};

	struct KPayletterGiftPacket
	{
		CTime m_ctTime;
		KEBILL_GIFT_ITEM_REQ m_kPacket;

		KPayletterGiftPacket( CTime ctTime_, KEBILL_GIFT_ITEM_REQ kPacket_ )
		{
			m_ctTime = ctTime_;
			m_kPacket = kPacket_;
		}
	};

	struct KPayletterCouponPacket
	{
		CTime m_ctTime;
		KEBILL_USE_COUPON_REQ m_kPacket;

		KPayletterCouponPacket( CTime ctTime_, KEBILL_USE_COUPON_REQ kPacket_ )
		{
			m_ctTime = ctTime_;
			m_kPacket = kPacket_;
		}
	};

	enum PL_BILLING_ENUM
	{
		PLBE_PACKET_DELETE_MIN = 2,
	};

	KCSLOCK_DECLARE( std::map<UidType KCOMMA() KPayletterBuyPacket>,	m_mapBuyPacket );
	KCSLOCK_DECLARE( std::map<UidType KCOMMA() KPayletterGiftPacket>,	m_mapGiftPacket );
	KCSLOCK_DECLARE( std::map<UidType KCOMMA() KPayletterCouponPacket>,	m_mapCouponPacket );

	typedef std::map<UidType, KPayletterBuyPacket>::iterator MAP_BUYPACKET_ITOR;
	typedef std::map<UidType, KPayletterGiftPacket>::iterator MAP_GIFTPACKET_ITOR;
	typedef std::map<UidType, KPayletterCouponPacket>::iterator MAP_COUPONPACKET_ITOR;

    
};

DefSingletonInline( KPayletterBillingManager );

#endif SERV_COUNTRY_US