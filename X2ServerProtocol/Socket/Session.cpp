#include "StdAfx.h"

#include "Session.h"


//#ifdef KNC_USE_IOCP
#include "IOCP.h"
//#endif

void KSkSession::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" );
        OnSocketError();
        return;
    }

    USHORT          usPacketLen = 0;

    m_ovlRecv.m_dwLeft += dwTransfered_;

    // ��Ŷ ���� ���෮���� ũ�ٸ� ��� �� ��Ŷ ���ɼ��� üũ�Ѵ�.
    while( m_ovlRecv.m_dwLeft >= sizeof(usPacketLen) )
    {
        // ��Ŷ ���̸� �˾Ƴ���.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );

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
				
				//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
				if( !kSecBuff.IsAuthentic_Local( spSession->GetSADatabase() ) ) // Ŭ���̾�Ʈ������ IsAuthentic()�� �Ἥ sequence number�� üũ ����.
#else
                if( !kSecBuff.IsAuthentic_notRWM( true ) ) // Ŭ���̾�Ʈ������ IsAuthentic()�� �Ἥ sequence number�� üũ ����.
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
					//}}
                {
					static int iCount = 0;
					++iCount;

					if( iCount >= 10 )
					{						
						spSession->OnAuthenticFailed();
						iCount = 0;
					}
				}
				else
				{
					// ���� �� ��ȣȭ ó���� ��ġ�� ���� payload���� ����.
					KncSecurity::KByteStream payload;
					//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
					JIF( kSecBuff.GetPayload_Local( payload, spSession->GetSADatabase() ) );
#else
					JIF( kSecBuff.GetPayload( payload ) );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
					//}}

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

KSession::KSession( bool bIsProxy ) 
:
    m_spSockObj         ( new KSkSession ),
    m_bIsProxy          ( bIsProxy ),
    m_bUseIocp          ( true ),
    m_bDestroyReserved  ( false ),
    m_nSPIndex          ( 0 ),
    m_bAuthKeyRecved    ( false ),
    m_bCheckHBTick      ( true )
{
    m_dwHBTick = GetTickCount();

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;

	//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	m_pSADatabase = NULL;
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}
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
		//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
#ifdef  X2OPTIMIZE_SESSION_THREAD_SAFETY
        KncSecurity::KSADatabase* pSADatabase = static_cast<KncSecurity::KSADatabase*>( InterlockedExchangePointer( (void**) &m_pSADatabase, NULL ) );
        if ( pSADatabase != NULL )
        {
			pSADatabase->Delete( m_nSPIndex );
			delete pSADatabase;
			pSADatabase = NULL;
        }
#else   X2OPTIMIZE_SESSION_THREAD_SAFETY
		if( m_pSADatabase != NULL )
		{
			m_pSADatabase->Delete( m_nSPIndex );
			delete m_pSADatabase;
			m_pSADatabase = NULL;
		}
#endif  X2OPTIMIZE_SESSION_THREAD_SAFETY
#else
		KncSecurity::GetSADB().Delete( m_nSPIndex );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
		//}}
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

bool KSession::Init( bool bUseIocp_  )
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
        if( m_bCheckHBTick && dwCurrentTick - m_dwHBTick > 60000 )  // 60s ������ heart-bit check
        {
            START_LOG( cwarn, L"Reguard " << m_strName 
                << L" as a Zombie. (tick: " 
                << dwCurrentTick - m_dwHBTick 
                << L" )" );

            ReserveDestroy();
            //SetDisconnectReason( KStatistics::eSIColDR_Heart_Beat );
            //OnDestroy();
        }
    }
}

void KSession::ReserveDestroy()
{
    KEventPtr spEvent( new KEvent );
    spEvent->m_usEventID = E_RESERVE_DESTROY;
//    spEvent->m_kDestPerformer.m_dwPerformerID = GetPfID();

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

	//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
#ifdef  X2OPTIMIZE_SESSION_THREAD_SAFETY
	_JIF( kSecBuff.Create_Local( bsbuff, GetSADatabase()
        ), return false );				// ��ȣȭ �� ���� ���� ó��.
#else   X2OPTIMIZE_SESSION_THREAD_SAFETY
	_JIF( kSecBuff.Create_Local( bsbuff, m_pSADatabase
        ), return false );				// ��ȣȭ �� ���� ���� ó��.
#endif  X2OPTIMIZE_SESSION_THREAD_SAFETY

#else
	_JIF( kSecBuff.Create( bsbuff ), return false );				// ��ȣȭ �� ���� ���� ó��.
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

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

    return m_spSockObj->SendData( ( const char* )bsbuff.data(), bsbuff.length() );
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

    if( g_bIsSERVICE == false )
        dbg::clog << L"�� " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << dbg::endl;//END_LOG;

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

	const KncSecurity::KSecurityAssociation* pkSA = NULL;
	SpiType nSpiNew = 0;
	//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	pkSA = GetSADatabase()->CreateNewSA( nSpiNew );
#else
	pkSA = KncSecurity::GetSADB().CreateNewSA( nSpiNew );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

	KSerializer ks;
	KEvent      kEvent;

	kEvent.m_usEventID  = E_ACCEPT_CONNECTION_NOT;
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( nSpiNew );
	ks.Put( *pkSA );
	ks.EndWriting();

	SendPacket( kEvent );    // ���Ű�� ���� ��,
	m_nSPIndex = nSpiNew;   // �� Ű�� �������ش�.
}

void KSession::OnRecvCompleted( IN const KncSecurity::KByteStream& payload_ )
{
    KSerializer     ks;
    KSerBuffer      kbuff;

    // payload�� KEvent �������� ��ȯ�Ѵ�.
    kbuff.Write( payload_.data(), payload_.length() );
    KEventPtr spEvent( new KEvent );
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

            KncSecurity::KSecurityAssociation kSA;

            ks.BeginReading( &spEvent->m_kbuff );
            ks.Get( m_nSPIndex );
            ks.Get( kSA );
            ks.EndReading();

			//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
			GetSADatabase()->Insert( m_nSPIndex, kSA );
#else
			KncSecurity::GetSADB().Insert( m_nSPIndex, kSA );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
			//}}
            START_LOG( clog, m_strName << L", ����Ű ����. spi:" << m_nSPIndex  );

            m_bAuthKeyRecved = true;
        }
        break;
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
#ifdef _IN_HOUSE_
                _JIF( m_spSockObj, break );
#else
				if( !m_spSockObj )
					break;
#endif
                m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_RECV );
            }
            break;
        case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
            {
#ifdef _IN_HOUSE_
				_JIF( m_spSockObj, break );
#else
				if( !m_spSockObj )
					break;
#endif
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

void KSession::OnAuthenticFailed()
{
	// SessionŬ���������� �ƹ������� ���� �ʽ��ϴ�.
}

//{{ 2012. 03. 19	��μ�		KSADatabase �� Session �� ����� �̵�(Ŭ���̾�Ʈ��)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
KncSecurity::KSADatabase* KSession::GetSADatabase()
{
#ifdef  X2OPTIMIZE_SESSION_THREAD_SAFETY
	if( m_pSADatabase == NULL )
	{
        KncSecurity::KSADatabase* pSADatabase = new KncSecurity::KSADatabase();
        KncSecurity::KSADatabase* pSADatabaseRet = static_cast<KncSecurity::KSADatabase*>
            ( InterlockedCompareExchangePointer( (volatile PVOID*) &m_pSADatabase, pSADatabase, NULL ) );
        if ( pSADatabaseRet == NULL )
        {
            return pSADatabase;
        }
        else
        {
            delete pSADatabase;
            pSADatabase = NULL;
            return pSADatabaseRet;
        }
	}
	return m_pSADatabase;
#else   X2OPTIMIZE_SESSION_THREAD_SAFETY
	if( m_pSADatabase == NULL )
	{
		m_pSADatabase = new KncSecurity::KSADatabase();
		return m_pSADatabase;
	}
	return m_pSADatabase;
#endif  X2OPTIMIZE_SESSION_THREAD_SAFETY
}
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
//}}