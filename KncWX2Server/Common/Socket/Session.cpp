#include "Session.h"
#include "Event.h"
#include "Thread/Locker.h"
#include "dbg/dbg.hpp"
#include "NetCommon.h"
#include "Statistics.h"
#include <time.h>   // localtime

//#ifndef KNC_DONT_USE_IOCP
#include "IOCP.h"
//#endif

void KSkSession::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" );

        SetDisconnectReason( KStatistics::eSIColDR_Session_Close );
        OnSocketError();
        return;
    }

    USHORT usPacketLen = 0;
    m_ovlRecv.m_dwLeft += dwTransfered_;

    // ��Ŷ ���� ���෮���� ũ�ٸ� ��� �� ��Ŷ ���ɼ��� üũ�Ѵ�.
    while( m_ovlRecv.m_dwLeft >= sizeof( usPacketLen ) )
    {
        // ��Ŷ ���̸� �˾Ƴ���.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof( usPacketLen ) );

        // ���ۿ� �޾Ƶ� �����ͷ� ������ packet�� ������ ��� �������� �����Ѵ�.
        if( m_ovlRecv.m_dwLeft >= usPacketLen )
        {
            KSessionPtr spSession = GetSessionPtr();
            if( spSession )
            {
                // ���̸� ������ �����͸� ���ۿ� ��´�.
                KncSecurity::KSecureBuffer kSecBuff( spSession->m_nSPIndex, m_ovlRecv.m_pBuffer + sizeof( usPacketLen ),
                    usPacketLen - sizeof( usPacketLen ) );
                //std::cout << "RECV " << kSecBuff << std::endl;

				// ���� : dos������ ���´ٸ� �ᱹ �����α׸� ���ݷ� ��ŭ �����̱� ������ ������ ���������°� ����������.
                if( !kSecBuff.IsAuthentic_notRWM( spSession->IsCheckSequenceNum() ) )
                {
                    START_LOG( cwarn, L"packet authentication failed." )
                        << BUILD_LOG( kSecBuff.GetSize() )
                        << BUILD_LOG( spSession->GetName() );

					// ��Ŷ ������ �����ϸ� ����ī��Ʈ�� �ø���.
					spSession->IncreasePacketAuthFailCnt();

					//{{ 2009. 10. 21  ������	��Ŷ ���� ���а� Ư������ �̻� �߻��� ���� ����
					if( spSession->GetPacketAuthFailCnt() > spSession->GetPacketAuthFailLimit() )
					{
						START_LOG( clog, L"Find Traffic Attacker!" );

						SetDisconnectReason( KStatistics::eSIColDR_Traffic_Attack );
						OnSocketError();

						// ���⼭ return�ؾ߸� InitRecv()�� ȣ������ �����Ƿ� IOCP�Ϸ� �̺�Ʈ�� ���̻� ���� �ʴ´�.
						return;
					}
					//}}
                }
				else
				{
					// ���� �� ��ȣȭ ó���� ��ġ�� ���� payload���� ����.
					KncSecurity::KByteStream payload;
					JIF( kSecBuff.GetPayload_notRWM( payload, false ) ); // ������ �̹� IsAuthentic()�� ȣ�������Ƿ� �ι�° ���ڴ� false�� ����!

					spSession->OnRecvCompleted( payload );
				}
            }
            else
            {
                START_LOG( clog, L"���� ��ü�� �Ҹ�Ǿ���." );
            }

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );
        }
		//{{ 2012. 02. 01	������	recv buffer full
#ifdef SERV_CHECK_RECV_BUFFER_FULL
		// ���ŵ� ��Ŷ�� ���̰� Max Packet Size�� ������ �����Ű��!
		else if( usPacketLen > MAX_PACKET_SIZE )
		{
			START_LOG( cout, L"[Disconnect] Recv Buffer Full!" )
				<< BUILD_LOG( usPacketLen );

			SetDisconnectReason( KStatistics::eSIColDR_RecvBufferFull );
			OnSocketError();
			return;
		}
#endif SERV_CHECK_RECV_BUFFER_FULL
		//}}
        else
        {
            break;     // ������ ��Ŷ�� �����ϱ⿡ ����� �����Ͱ� ����. loop�� ����������.
        }
    }

    // recv function call
    LIF( InitRecv() );
}

void KSkSession::OnSocketError()
{
    KSessionPtr spSession = GetSessionPtr();
    if( spSession )
    {
        spSession->OnSocketError();
    }
    else
    {
        START_LOG( clog, L"Session ��ü�� �̹� ������." )
            << END_LOG;
    }
}

void KSkSession::OnAcceptConnection()
{
    KSessionPtr spSession = GetSessionPtr();
    if( spSession )
    {
        spSession->OnAcceptConnection();
    }
    else
    {
        START_LOG( clog, L"Session ��ü�� �̹� ������." )
            << END_LOG;
    }
}

KSessionPtr KSkSession::GetSessionPtr()
{
    KSessionPtr spSession = m_pkSession.lock();

    if( spSession )
    {
        return spSession;
    }
    else
    {
        return KSessionPtr();
    }
}

NiImplementRTTI( KSession, KPerformer );

#define PROXY_ONLY cassert( m_bIsProxy )(m_strName).fatal()
#define UNPROXY_ONLY cassert( !m_bIsProxy )(m_strName).fatal()

KSession::KMaxPacketInfo KSession::ms_kMaxPacketInfo = {
    KncCriticalSection(), L"�ʱ����", L"�ʱ����", 0, 0, CTime::GetCurrentTime()
};

//{{ 2009. 10. 18  ������	SequenceNum
KSession::KPacketAuthInfo KSession::ms_kPacketAuthInfo = {
	KncCriticalSection(), L"������Ŷ����", 0, 0, CTime::GetCurrentTime()
};
//}}

KSession::KSession( bool bIsProxy ) 
:
    m_spSockObj         ( new KSkSession ),
    m_bIsProxy          ( bIsProxy ),
    m_bUseIocp          ( true ),
    m_bDestroyReserved  ( false ),
    m_nSPIndex          ( 0 ),
    m_bAuthKeyRecved    ( false ),
    m_bCheckHBTick      ( true ),
	//{{ 2009. 10. 18  ������	Sequence Number
	m_bCheckSequenceNum ( false ),
	m_uiPacketAuthFailLimit( PACKET_AUTH_FAIL_DEFAULT ),
	m_uiPacketAuthFailCnt ( 0 ),
	//}}
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	m_eSessionType		( SST_NONE ),
#endif SERV_KEVENT_FROM
	//}}
	//{{ 2011. 10. 31	������	��Ʈ��Ʈ ���� ���� �м�
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	m_dwHBTickDump		( 0 ),
	m_dwCurrentTickDump	( 0 ),
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	//}}
	//{{ 2009. 8. 10  ������	send buffer full ���� ã��		
	m_usLastSendEventID ( EVENT_EVENTID_SENTINEL )
	//}}
{
    m_dwHBTick = GetTickCount();

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;
}

KSession::~KSession(void)
{
	if( m_spSockObj )
	{
		m_spSockObj->CloseSocket();
	}
	else
	{
		START_LOG( cerr, L"���Ͽ�����Ʈ ������ �̻�." )
			<< END_LOG;
	}

    if( !m_bUseIocp )
    {
        End();  // thread use

        ::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
        ::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
    }

    if( m_nSPIndex != 0 )
    {
        START_LOG( clog, L"SA ����. Name : " << m_strName << L", SPI : " << m_nSPIndex );
        KncSecurity::GetSADB().Delete( m_nSPIndex );
        m_nSPIndex = 0;
    }
}

ImplToStringW( KSession )
{
    return START_TOSTRING_PARENTW( KPerformer )
        << TOSTRINGW( m_dwHBTick )
        << TOSTRINGW( m_bDestroyReserved )
        << TOSTRINGW( m_nSPIndex )
        << TOSTRINGW( m_bIsProxy )
        << TOSTRINGW( m_bUseIocp )
        << TOSTRINGW( m_bCheckHBTick );
}

bool KSession::Init( bool bUseIocp_ )
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkSession = GetThisPtr< KSession >();

    if( m_bIsProxy )
    {
        m_bUseIocp = bUseIocp_;

        if( !m_bUseIocp )
        {
            m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
            m_spSockObj->SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
            m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
            m_spSockObj->SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );
        }

        return true;
    }
    else
    {
        LIF( bUseIocp_ );
        return true;
    }
}

bool KSession::Begin()  // KThread�� �̺�Ʈ ��ü�� �迭�� ������, WaitMultipleObject�� ����Ѵ�.
{
    if( KThread::Begin() )
    {
        m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
        return true;
    }

    return false;
}

void KSession::Tick()
{
    KPerformer::Tick();

    //////////////////////////////////////////////////////////////////////////
    // ���� ó��
    if( m_bDestroyReserved )    // ���ᰡ ����� ���¶�� �ٽ��ѹ� ����ó���� �õ��Ѵ�.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart beat

	// [����] -- m_dwHBTick ������ ����ȭ ó���� �ȵǼ� �߸��� ���� ���� ����.

	// KSession::Tick() �Լ����� ::GetTickCount()�� ȣ���Ͽ� ������ ����Ǿ��� m_dwHBTick������ ���� ��Ʈ��Ʈ üũ�� �ϴµ�
	// �Ʒ��� ::GetTickCount()�� ȣ��ǰ� ���� m_dwHBTick���� ���ο� ::GetTickCount()���� ��������
	// ������ Tick���� �ƴ϶� �� �ֱ��� Tick���� �ǹ����� �ᱹ ���̳ʽ� ���� ������ �ȴ�.

	//{{ 2013. 01. 15	������	���� ��Ʈ��Ʈ üũ ���� ����
#ifdef SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
	const DWORD dwOldHBTick = m_dwHBTick;
#endif SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
	//}}

    DWORD dwCurrentTick = ::GetTickCount();
    if( m_bIsProxy )
    {
        if( IsConnected() == false )    // UserProxy�� ���, ������ ��� ����ְų� Ȥ�� ������ �� ���� �ִ� (Robot)
            return;

        if( dwCurrentTick - m_dwHBTick <= 15000 ) // 15s ������ heart-beat ����
            return;

        if( !m_bAuthKeyRecved )   // ���� ����Ű�� �ޱ� �����̴�. �̶� heart-beat�� ������ �ʴ´�.
            return;

        //START_LOG( clog, L" Heart-beat " << m_strName << L"  :" << ::GetTickCount() - m_dwHBTick );
        m_dwHBTick = dwCurrentTick;

		KEvent kEvent;
        kEvent.m_usEventID = E_HEART_BEAT;
        SendPacket( kEvent );
    }
    else    // un-proxy heart-beat check
    {
		//{{ 2013. 01. 15	������	���� ��Ʈ��Ʈ üũ ���� ����
		//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
		//////////////////////////////////////////////////////////////////////////
		if( m_bCheckHBTick && dwCurrentTick - dwOldHBTick > 60000 )  // 60s ������ heart-bit check
		{
			//{{ 2011. 10. 31	������	��Ʈ��Ʈ ���� ���� �м�
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			m_dwHBTickDump = dwOldHBTick;
			m_dwCurrentTickDump = dwCurrentTick;
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			//}}

			START_LOG( cwarn, L"Reguard " << m_strName 
				<< L" as a Zombie. (tick: " 
				<< dwCurrentTick - dwOldHBTick 
				<< L" )" );

			SetDisconnectReason( KStatistics::eSIColDR_Heart_Beat );
			ReserveDestroy();
			//OnDestroy();
		}
		//////////////////////////////////////////////////////////////////////////
#else
		//////////////////////////////////////////////////////////////////////////
		if( m_bCheckHBTick && dwCurrentTick - m_dwHBTick > 60000 )  // 60s ������ heart-bit check
		{
			//{{ 2011. 10. 31	������	��Ʈ��Ʈ ���� ���� �м�
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			m_dwHBTickDump = m_dwHBTick;
			m_dwCurrentTickDump = dwCurrentTick;
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			//}}

			START_LOG( cwarn, L"Reguard " << m_strName 
				<< L" as a Zombie. (tick: " 
				<< dwCurrentTick - m_dwHBTick 
				<< L" )" );

			SetDisconnectReason( KStatistics::eSIColDR_Heart_Beat );
			ReserveDestroy();
			//OnDestroy();
		}
		//////////////////////////////////////////////////////////////////////////
#endif SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
		//////////////////////////////////////////////////////////////////////////
		//}}
    }
}

void KSession::ReserveDestroy()
{
	KEventPtr spEvent( new KEvent );
    spEvent->m_usEventID = E_RESERVE_DESTROY;
    spEvent->m_kDestPerformer.m_dwPerformerID = GetPfID();

    QueueingEvent( spEvent );
}

bool KSession::CheckExceedRefCount( int nCount_ )
{
    // �Է¹��� ���� ���ų� ���� ���� ������ ����Ű�� �����Ƿ�, true�� �����Ѵ�.
    // false�� ���ϵ� ���, �α���� ���� �߰��۾��� ���ؼ� child Ŭ�������� ��ӹ޾� �����Ѵ�.
    return ( GetRefCount() <= nCount_ );
}

bool KSession::SendPacket( IN const KEvent& kEvent )
{
    //if( m_bDestroyReserved ) return true;       ///< ���ᰡ ����Ǿ����� send �õ�. �̷� ��찡 �ٺ��ϹǷ�, true ����.
    _JIF( m_spSockObj, return false );
    _JIF( m_spSockObj->IsConnected(), return false );    ///< ������ ��ȿ���� ����.

    // serialize - full event object
    KSerializer ks;
    KSerBuffer kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( kEvent );
    ks.EndWriting();

    // ��ȣȭ�Ѵ�.
    KncSecurity::KByteStream bsbuff;
    KncSecurity::KSecureBuffer kSecBuff( m_nSPIndex );

    bsbuff.assign( (byte*)kbuff.GetData(), kbuff.GetLength() );		// ���� ������ ������ �ֱ�.
    _JIF( kSecBuff.Create_notRWM( bsbuff ), return false );			// ��ȣȭ �� ���� ���� ó��.

    // bsbuff�� ��Ȱ����, ��ȣȭ�� ������ �տ� �� ���̸� ������ byte stream�� �����.
    bsbuff.clear();
    bsbuff.reserve( kSecBuff.GetSize() + sizeof(USHORT) );   // ��Ŷ �� ������[4] + ������ ������[����]
    bsbuff.Assign( kSecBuff.GetSize() + sizeof(USHORT), sizeof(USHORT) );
    bsbuff.append( (byte*)kSecBuff.GetDataPtr(), kSecBuff.GetSize() );

    {   // locking scope

        KLocker lock2( ms_kMaxPacketInfo.m_cs );

        if( ms_kMaxPacketInfo.m_nPacketSize < bsbuff.length() )
        {
            ms_kMaxPacketInfo.m_strName     = m_strName;
            ms_kMaxPacketInfo.m_strDataDesc = kEvent.GetIDStr();
            ms_kMaxPacketInfo.m_cTime       = CTime::GetCurrentTime();
            ms_kMaxPacketInfo.m_nDataSize   = kbuff.GetLength();
            ms_kMaxPacketInfo.m_nPacketSize = bsbuff.length();
            START_LOG( clog2, L"��Ŷ �ִ������ ����. Name : " << m_strName )
                << BUILD_LOG( ms_kMaxPacketInfo.m_strDataDesc )
                << BUILD_LOG( ms_kMaxPacketInfo.m_nDataSize )
                << BUILD_LOG( ms_kMaxPacketInfo.m_nPacketSize );
        }

    }   // locking scope

    if( kEvent.m_usEventID != E_HEART_BEAT )
        dbg::clog << L"�� " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

	//{{ 2009. 8. 10  ������	Send Buffer Full����ã��
	m_usLastSendEventID = kEvent.m_usEventID;
	//}}
	//{{ 2012. 02. 01	������	recv buffer full
#ifdef SERV_CHECK_RECV_BUFFER_FULL
	if( bsbuff.length() > MAX_PACKET_SIZE )
	{
		START_LOG( cout, L"[check] max packet size�� �Ѿ�� ��Ŷ�� ���۵Ǿ����ϴ�. recv buffer full�� ����˴ϴ�!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kEvent.GetIDStr() )
			<< BUILD_LOG( bsbuff.length() );
		// �ƹ��� ó���� ���� �ʰ� �α׸� ����ϴ�!
	}
#endif SERV_CHECK_RECV_BUFFER_FULL
	//}}

#ifdef SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL// �۾���¥: 2013-09-23	// �ڼ���
	const bool bResult = m_spSockObj->SendData( ( const char* )bsbuff.data(), bsbuff.length() );

	if( m_bIsProxy == true )
	{
		if( 30 < m_queEventID.size() )
		{
			m_queEventID.pop();
		}
		m_queEventID.push( TYPE_SEND_BUFFER_FULL_LOG::value_type( kEvent.m_usEventID, CTime::GetCurrentTime() ) );

		if( ( bResult == false ) && ( GetDisconnectReason() == KStatistics::eSIColDR_SendBufferFull ) )
		{
			while( m_queEventID.empty() == false )
			{
				const TYPE_PAIR_USHORT_CTIME& sInfo = m_queEventID.front();
				START_LOG( cerr, L"Send Buffer Full. Latest Packet ID : " << (LPCTSTR)sInfo.second.Format( KNC_TIME_FORMAT ) << L", " << KEvent::GetIDStr( sInfo.first ) );
				m_queEventID.pop();
			}
		}
	}

	return bResult;
#else // SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL
	return m_spSockObj->SendData( ( const char* )bsbuff.data(), bsbuff.length() );
#endif // SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL
}

bool KSession::SendID( DWORD dwDestPI_, UidType nTo_, UidType anTrace[], unsigned short usEventID_ )
{
    // check valid state	
	KEvent kEvent;
    kEvent.SetData( dwDestPI_, anTrace, usEventID_ );
    LIF( kEvent.m_kDestPerformer.AddUID( nTo_ ) );

    // send packet
    if( ! KSession::SendPacket( kEvent ) )
        return false;

    dbg::clog << L"�� " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

    return true;
}

void KSession::DumpMaxSendData( std::wostream& stm_ )
{
    KLocker lock( ms_kMaxPacketInfo.m_cs );

    stm_ << L" -- Max size Data sending -- " << std::endl
        << L"  user name   : " << ms_kMaxPacketInfo.m_strName << std::endl
        << L"  data desc   : " << ms_kMaxPacketInfo.m_strDataDesc << std::endl
        << L"  data size   : " << ms_kMaxPacketInfo.m_nDataSize 
        << L" (" << ms_kMaxPacketInfo.m_nDataSize / 1024.0f << " Kb)" << std::endl
        << L"  packet size : " << ms_kMaxPacketInfo.m_nPacketSize
        << L" (" << ms_kMaxPacketInfo.m_nPacketSize / 1024.0f << " Kb)" << std::endl
        << L"  send time   : " << (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format( KNC_TIME_FORMAT ) << std::endl;
}

void KSession::ResetMaxSendData()
{
    KLocker lock( ms_kMaxPacketInfo.m_cs );

    ms_kMaxPacketInfo.m_strDataDesc.clear();
    ms_kMaxPacketInfo.m_nDataSize = 0;
    ms_kMaxPacketInfo.m_nPacketSize = 0;
    ms_kMaxPacketInfo.m_cTime = CTime::GetCurrentTime();
    ms_kMaxPacketInfo.m_strName = (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format( KNC_TIME_FORMAT );
    ms_kMaxPacketInfo.m_strName.append( L" �� ������ �ʱ�ȭ �Ǿ����ϴ�." );
}

//{{ 2009. 10. 18  ������	SequenceNum
void KSession::DumpPacketAuthInfo( std::wostream& stm_ )
{
	KLocker lock( ms_kPacketAuthInfo.m_cs );

	stm_ << L" -- Packet Auth Fail Max Cnt -- " << std::endl
		<< L"  User Name   : " << ms_kPacketAuthInfo.m_strName << std::endl
		<< L"  Fail Count  : " << ms_kPacketAuthInfo.m_uiMaxFailCnt << std::endl
		<< L"  Total Fail  : " << ms_kPacketAuthInfo.m_uiTotalFailCnt << std::endl
		<< L"  Reg Time    : " << (LPCWSTR)ms_kPacketAuthInfo.m_cRegTime.Format( KNC_TIME_FORMAT ) << std::endl;
}
//}}

bool KSession::Connect( const char* szIP_, unsigned short usPort_ )
{
    PROXY_ONLY;
    _JIF( m_spSockObj, return false );

    std::vector< std::pair<int,int> > vecOpt;
    vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // ������� address�� ������ �� �ְ� ���. 

    START_LOG( clog, L"Connect To : " << szIP_ << L":" << usPort_ );

    _JIF( m_spSockObj->Connect( szIP_, usPort_, m_bUseIocp, &vecOpt ), return false );

    if( !m_bUseIocp )   // no use iocp
    {
        Begin();    // thread run
    }

    // 050520. Ű�� �޾� ������ ������ ����Ѵ�.
    DWORD dwWaitStartTick = ::GetTickCount();

    Sleep( 20 );

    // ���ð��� �ʰ����� �ʾҰ� ���� �޼����� �ƹ��͵� ���� ������
    while( ::GetTickCount() - dwWaitStartTick < SECURE_KEY_WAITING_TIME && !m_bAuthKeyRecved )
        Sleep( 20 );    // ��� ��ٸ���.

    if( !m_bAuthKeyRecved )  // Ű�� ���� ���ߴٸ�
    {
        START_LOG( cerr, L"�������� ���� Ű�� ���� ���߽��ϴ�. name:" << m_strName << L", ���ð�:" 
            << ::GetTickCount() - dwWaitStartTick );
        ReserveDestroy();
        return false;
    }

    return true;
}

void KSession::OnAcceptConnection()
{
    UNPROXY_ONLY;

	// 2009-10-16  ������ comment
	// 1. Ŭ���̾�Ʈ�� ������ �����ϸ� ȣ���
	// 2. ��Ŷ ������ ���� KSecurityAssociation��ü�� ���⼭ ������ (Session�� �ϳ�������)
	// 3. Ŭ���̾�Ʈ�� KSecurityAssociation��ü ������ ������ ���������� ����ȭ��Ű���� ������

    const KncSecurity::KSecurityAssociation* pkSA = NULL;
    SpiType nSpiNew = 0;
    pkSA = KncSecurity::GetSADB().CreateNewSA( nSpiNew );

    KSerializer ks;	
	KEvent kEvent;

    kEvent.m_usEventID  = E_ACCEPT_CONNECTION_NOT;
    ks.BeginWriting( &kEvent.m_kbuff );
    ks.Put( nSpiNew );
    ks.Put( *pkSA );
    ks.EndWriting();

    SendPacket( kEvent );   // ���Ű�� ���� ��,
    m_nSPIndex = nSpiNew;   // �� Ű�� �������ش�.
}

void KSession::OnRecvCompleted( IN const KncSecurity::KByteStream& payload_ )
{
    KSerializer ks;
    KSerBuffer kbuff;

    // payload�� KEvent �������� ��ȯ�Ѵ�.
    kbuff.Write( payload_.data(), payload_.length() );
	KEventPtr spEvent( new KEvent );
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	if( GetSessionType() == SST_CLIENT )
	{
		// ���� Ŭ���̾�Ʈ�� ���� ������ ��Ŷ�̶�� ǥ���ص���!
		spEvent->SetFromType( KEvent::EFT_CLIENT );
	}
#endif SERV_KEVENT_FROM
	//}}
    ks.BeginReading( &kbuff );
    ks.Get( *spEvent );
    ks.EndReading();

    // ���� �� ��츸 �ڵ����� �����Ѵ�.
    spEvent->m_kbuff.UnCompress();

    switch( spEvent->m_usEventID )
    {
    case E_HEART_BEAT:                  // heart bit filtering - no queueing
        UNPROXY_ONLY;
        break;
    case E_ACCEPT_CONNECTION_NOT:
        {
            PROXY_ONLY;

			// 2009-10-16  ������ comment
			// 1. Ŭ���̾�Ʈ�� ���������� �����ϸ� ���� ó�� �����κ��� �޴� ��Ŷ��
			// 2. ��Ŷ ������ ���� KSecurityAssociation��ü�� �����κ��� �޾Ƽ� SADB�� ������
			// 3. �����κ��� ������ SPIndex�� ������ ����
			// 4. Ŭ���̾�Ʈ������ �ߺ������ʴ� SPIndex�� �����ϰ� �����Ͽ� ��� ( KSecurityAssociation�� �����κ��� ������ �״�� ����� )

            KncSecurity::KSecurityAssociation kSA;

            ks.BeginReading( &spEvent->m_kbuff );
            ks.Get( m_nSPIndex );
            ks.Get( kSA );
            ks.EndReading();

            KncSecurity::GetSADB().Insert( m_nSPIndex, kSA );
            START_LOG( clog, m_strName << L", ����Ű ����. spi:" << m_nSPIndex  );			

            m_bAuthKeyRecved = true;
        }
        break;
		//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	case E_CHECK_DDOS_GUARD_REQ:
		{
			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"[�׽�Ʈ�α�] DDOS Guard�� ����߽��ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetIPStr() );
			//////////////////////////////////////////////////////////////////////////

			KEvent kEvent;
			kEvent.SetData( PC_USER, NULL, E_CHECK_DDOS_GUARD_ACK, char() );
			SendPacket( kEvent );
		}
		break;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
		//}}
    default:
        QueueingEvent( spEvent );
    }

    // renew m_dwHBRecvTick
    if( !m_bIsProxy )
        m_dwHBTick = ::GetTickCount();
}

void KSession::Run()
{
    PROXY_ONLY;

    DWORD ret       = NULL;
    bool bLoop      = true;

    while( bLoop )
    {
        ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE,
            m_hEvents, 
            false, 
            100 );     // 0.1s ����

        switch( ret )
        {
        case EVENT_DISCONNECTED + WAIT_OBJECT_0:    // ������ ����
            bLoop = false;
            break;
        case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
            {
                _JIF( m_spSockObj, break );
                m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_RECV );
            }
            break;
        case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
            {
                _JIF( m_spSockObj, break );
                m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_SEND );
            }
            break;
        case WAIT_TIMEOUT:
            break;

        case WAIT_FAILED:
            START_LOG( cerr, GET_WSA_MSG );
            break;
        }

        Tick(); // 050214. IOCP�� ������� ���� ���, Tick�� ���ο��� ó���Ѵ�.

    } // while

}

void KSession::OnSocketError()
{	
    ReserveDestroy();
}

void KSession::OnDestroy()
{
    m_bDestroyReserved = false;     // ������ ���ᰡ ���� �Ǿ��� �ƴϵ�, �ѹ� Destory ȣ���� �����ϸ� ��������.

    // 050323. floris. IOCP ��� ���� ���, ReserveDestroy ȣ���ϸ� Thread �����.
    if( !m_bUseIocp ) 
    {
        SetEvent( m_hKillEvent );
    }
    else
    {
		//{{ 2009. 10. 18  ������	SequenceNum
		const unsigned int uiPacketAuthFailCnt = GetPacketAuthFailCnt();
		if( uiPacketAuthFailCnt > 0 )
		{
			KLocker lock( ms_kPacketAuthInfo.m_cs );

			ms_kPacketAuthInfo.m_uiTotalFailCnt += uiPacketAuthFailCnt; // ��ü fail count�� ����.

			if( ms_kPacketAuthInfo.m_uiMaxFailCnt < uiPacketAuthFailCnt ) // �ִ� fail count�� ����.
			{
				ms_kPacketAuthInfo.m_strName	  = m_strName;
				ms_kPacketAuthInfo.m_cRegTime	  = CTime::GetCurrentTime();
				ms_kPacketAuthInfo.m_uiMaxFailCnt = uiPacketAuthFailCnt;
				START_LOG( clog2, L"Packet Auth Fail ����. Name : " << m_strName )
					<< BUILD_LOG( ms_kPacketAuthInfo.m_uiMaxFailCnt );
			}
		}
		//}}

        DWORD dwKey;
        if( GetKey( dwKey ) )
        {
            SiKIocp()->DeleteCompKey( dwKey );
        }
        else
        {
            START_LOG( cerr, L"���Ͽ�����Ʈ�� Ű �� ��� ����." )
                << BUILD_LOG( GetUID() )
                << BUILD_LOG( GetName() )
                << END_LOG;
        }
    }
}

void KSession::SetP2PIP( const std::wstring& wstrIP )
{ 
    m_wstrIP = wstrIP; 
}

//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	void			KSession::SetP2PInternalIP( const std::wstring& wstrIP )
    { m_wstrInternalIP = wstrIP; }
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP