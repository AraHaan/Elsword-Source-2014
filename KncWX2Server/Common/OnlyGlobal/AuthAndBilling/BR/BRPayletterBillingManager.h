//////////////////////////////////////////////////////////////////////////
//	PayletterBillling.h
//
//	Payletter ���� ���� �޴���
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <KNCSingleton.h>
#include <Thread/KCSLockType.h>
#include "ThreadManager.h"
#include "BRPayletterBillingPacket.h"

#ifdef SERV_COUNTRY_BR

class KBRPayletterBillingManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KBRPayletterBillingManager );

public:
	KBRPayletterBillingManager();
	virtual ~KBRPayletterBillingManager();

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

	void QueueingSendPacket( const KBRPayletterBillingPacketPtr& spPacket );

	bool GetSendPacket( KBRPayletterBillingPacketPtr& spPacket );
	void MakeEventFromReceivedPacket();
	void DumpBuffer( BYTE* buffer, bool bError );

	bool IsInitialized()									{ return m_bInitialized; }

	// �ְ� ���� �Լ�
	void InsertUniqueKeyUserUID( UidType iUniqueKey, UidType iUserUID );
	void DeleteUniqueKeyUserUID( UidType iUniqueKey );
	UidType GetUniqueKeyUserUID( UidType iUniqueKey );

	void InsertBuyPacket( IN KEBILL_BUY_PRODUCT_REQ kPacket_, OUT bool& bRet_ );
	void GetBuyPacket( IN const UidType iUniqueKey, OUT KEBILL_BUY_PRODUCT_REQ& kPacket_, OUT bool& bRet_ );
	void EraseBuyPacket( IN const UidType iUniqueKey );

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

	boost::shared_ptr< KTThread< KBRPayletterBillingManager > >    m_spThreadRecv;			// Server Init Thread
	boost::shared_ptr< KTThread< KBRPayletterBillingManager > >    m_spThreadSend;			// Server Init Thread
	//{{ 2009. 11. 21  ������	���������ӱ�������
	boost::shared_ptr< KTThread< KBRPayletterBillingManager > >    m_spThreadKeepConnect;	// Server Init Thread
	bool														m_bFirstConnectSucc;	// KeepConnectionThread, SendThread, RecvThread
	//}}

	KCSLOCK_DECLARE( std::queue< KBRPayletterBillingPacketPtr >,	m_kSendQueue );			// SendThread, TCPBillingProcessThread

	bool                                                        m_bInitialized;			// KeepConnectionThread, TCPBillingProcessThread
	boost::shared_ptr< void >                                   m_spEvent;				// KeepConnectionThread, TCPBillingProcessThread

	char                                                        m_cRecvBuffer[MAX_PACKET_SIZE_PLB]; // RecvThread
	unsigned long                                               m_ulRecvCP;				// KeepConnectionThread, RecvThread

	bool														m_bSocketConnect;		// ���� : ó�� ���� ���� ����� üũ

	KCSLOCK_DECLARE( std::map< UidType KCOMMA() UidType >, m_mapUniqueKeyUserUID );// TCPBillingProcessThread
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

	enum PL_BILLING_ENUM
	{
		PLBE_PACKET_DELETE_MIN = 1,
	};

	KCSLOCK_DECLARE( std::map<UidType KCOMMA() KPayletterBuyPacket>,	m_mapBuyPacket );

	typedef std::map<UidType, KPayletterBuyPacket>::iterator MAP_BUYPACKET_ITOR;
    
};

DefSingletonInline( KBRPayletterBillingManager );

#endif SERV_COUNTRY_BR