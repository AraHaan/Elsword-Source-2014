#include "StdAfx.h"

#include "TRUser.h"

void KSkTRUser::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" );
        OnSocketError();
        return;
    }

    USHORT usPacketLen = 0;

    m_ovlRecv.m_dwLeft += dwTransfered_;

    // ��Ŷ ���� ���෮���� ũ�ٸ� ��� �� ��Ŷ ���ɼ��� üũ�Ѵ�.
    while( m_ovlRecv.m_dwLeft >= sizeof( usPacketLen ) )
    {
        // ��Ŷ ���̸� �˾Ƴ���.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );

        // ��Ŷ�� �ּ� 3byte �̻��� ���̸� ������ �Ѵ�.
        if( usPacketLen < MINIMUM_PACKET_SIZE )
        {
            START_LOG( cerr, L"invalid packet size - " << usPacketLen );
            OnSocketError();
            return;
        }

        // ���ۿ� �޾Ƶ� �����ͷ� ������ packet�� ������ ��� �������� �����Ѵ�.
        if( m_ovlRecv.m_dwLeft >= usPacketLen )
        {
            //{{AFX
            //// get recvCnt
            //char cRecvCnt = -99; // ������� �ʴ� ������ �ʱ�ȭ.
            //memcpy( &cRecvCnt, &m_ovlRecv.m_pBuffer[ sizeof(usPacketLen) ], sizeof(cRecvCnt) );

            //// get (recvUIDn, data)
            //KSerBuffer buffer;
            //buffer.Write( &m_ovlRecv.m_pBuffer[MINIMUM_PACKET_SIZE], usPacketLen - MINIMUM_PACKET_SIZE );
            //}}AFX

            KTRUserPtr spTRUser = GetTRUserPtr();
            if( spTRUser )
            {
                //���̸� ������ �����͸� ���ۿ� ��´�.
                KSerBuffer buffer;
                buffer.Write( &m_ovlRecv.m_pBuffer[sizeof(usPacketLen)], usPacketLen - sizeof(usPacketLen) );

                //m_pkTRUser->OnRecvCompleted( cRecvCnt, buffer );
                spTRUser->OnRecvCompleted( buffer );
            }
            else
            {
                START_LOG( clog, L"TRUser ��ü�� �̹� ������." )
                    << END_LOG;
            }

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );            

        } // if( m_ovlRecv.m_dwLeft >= dwHeadPacketLen )    // ������ ��Ŷ�� ������ ���
        else
        {
            break;     // ������ ��Ŷ�� �����ϱ⿡ ����� �����Ͱ� ����. loop�� ����������.
        }
    } // while( m_ovlRecv.m_dwLeft >= sizeof(DWORD) )

    // recv function call
    LIF( InitRecv() );
}

void KSkTRUser::OnSocketError()
{
    KTRUserPtr spTRUser = GetTRUserPtr();
    if( spTRUser )
    {
        spTRUser->OnSocketError();
    }
    else
    {
        START_LOG( clog, L"TRUser ��ü�� �̹� ������." )
            << END_LOG;
    }
}

KTRUserPtr KSkTRUser::GetTRUserPtr()
{
    KTRUserPtr spTRUser = m_pkTRUser.lock();
    if( spTRUser )
    {
        return spTRUser;
    }
    else
    {
        return KTRUserPtr();
    }
}

NiImplementRootRTTI( KTRUser );

KTRUser::KTRUser(void)
:
m_spSockObj( new KSkTRUser ),
m_iUnitUID( 0L ),
m_bDestroyReserved( false ),
m_ServerMsgID( 0 ),
m_pSendGameMessage(NULL)
{
	for( int i = 0; i < KTRUser::TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}

	m_hEvents[EVENT_RECV_COMPLETED] = NULL;
	m_hEvents[EVENT_SEND_COMPLETED] = NULL;
}

KTRUser::~KTRUser(void)
{
	End( INFINITE );
	OnDestroy();

	::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
	::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
}

bool KTRUser::Begin()
{
    if( KThread::Begin() )
    {
        m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
        return true;
    }

    return false;
}

void KTRUser::Tick()
{
    if( m_bDestroyReserved )    // ���ᰡ ����� ���¶�� �ٽ��ѹ� ����ó���� �õ��Ѵ�.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart bit
    if( !IsConnected() )    // UserProxy�� ���, ������ ��� ����ְų� Ȥ�� ������ �� ���� �ִ� (Robot)
        return;

    if( ::GetTickCount() - GetTick( KTRUser::HB_TICK ) <= 30000 ) // 30s ������ heart-bit ����
        return;

    JIF( m_spSockObj );
#ifdef _IN_HOUSE_
    START_LOG( clog, L" Heart-bit " << m_iUnitUID << L"  :" << ::GetTickCount() - GetTick( KTRUser::HB_TICK ) );
#endif
    SetTick( KTRUser::HB_TICK );

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT) + sizeof(char), sizeof(USHORT) );
    buff.Append( E_HEART_BEAT, sizeof(char) );

#ifdef _IN_HOUSE_
	LIF( m_spSockObj->SendData( ( const char* )buff.data(), buff.length() ) );
#else
	m_spSockObj->SendData( ( const char* )buff.data(), buff.length() );    
#endif
}

bool KTRUser::Init()
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkTRUser = shared_from_this();

	m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	m_spSockObj->SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
	m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	m_spSockObj->SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );

	return true;
}

bool KTRUser::Connect( const char* szIP_, unsigned short usPort_, const UidType& iUnitUID_ )
{
	std::vector< std::pair<int,int> > vecOpt;
	vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // ������� address�� ������ �� �ְ� ���. 
	vecOpt.push_back( std::make_pair( IPPROTO_TCP, TCP_NODELAY ) ); // Nagle option�� ����. �����͸� ��Ƽ� ������ �ʰ� �� ��� �����ϵ��� �Ѵ�.

    _JIF( m_spSockObj, return false );
	_JIF( m_spSockObj->Connect( szIP_, usPort_, false, &vecOpt ), false );

	Begin();    // thread run

	// ���� �� ��� UID�� ����Ѵ�.
	m_iUnitUID = iUnitUID_;

	//{{AFX : ���� ������ Ŭ���̾�Ʈ ���� ���� UID ��� ��Ŷ�� �����ش�.
	//KncSecurity::KByteStream buff;
	//buff.Append( sizeof(USHORT) + sizeof(char) + sizeof(iUnitUID_), sizeof(USHORT) );
	//buff.Append( 0, sizeof(char) );
	//buff.Append( iUnitUID_ );
	//LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length() ) );
	//}}AFX

	START_LOG( clog, "Send UID : " << iUnitUID_ );

	return true;
}

bool KTRUser::SendID(unsigned short usEventID, bool bCompress_ /* = true  */)
{
    _JIF( m_spSockObj, return false );
	//if( m_bDestroyReserved ) return true;///< ���ᰡ ����Ǿ����� send �õ�. �̷� ��찡 �ٺ��ϹǷ�, true ����.
	_JIF( m_spSockObj->IsConnected(), return false );    ///< ������ ��ȿ���� ����.

	KEvent kEvent;
	kEvent.SetData( 0, NULL, usEventID );

	if( kEvent.m_kbuff.GetLength() > KSkTRUser::MAXIMUM_PACKET_SIZE ) return false;

	if( bCompress_ ) kEvent.m_kbuff.Compress();

	// serialize - full event object
	KSerializer     ks;
	KSerBuffer      kbuff;
	ks.BeginWriting( &kbuff );
	ks.Put( kEvent );
	ks.EndWriting();

	KncSecurity::KByteStream    bsbuff;
	bsbuff.reserve( kbuff.GetLength() + sizeof(USHORT) );
	bsbuff.Assign( kbuff.GetLength() + sizeof(USHORT), sizeof(USHORT) );
	bsbuff.append( (byte*)kbuff.GetData(), kbuff.GetLength() );   // ���� ������ ������ �ֱ�.

	if( kEvent.m_usEventID != E_HEART_BEAT )
		dbg::clog << L"�ߨ� " << kEvent.GetIDStr() << L" - TR SEND" << dbg::endl;//END_LOG;

	return m_spSockObj->SendData( (const char*)bsbuff.data(), bsbuff.length() );
}

void KTRUser::OnSocketError()
{
    m_bDestroyReserved = true;
}

void KTRUser::Run()
{
	DWORD ret       = NULL;
	bool bLoop      = true;

	while( bLoop )
	{
		ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE, m_hEvents, false, 100 );     // 0.1s ����

		switch( ret )
		{
		case EVENT_DISCONNECTED + WAIT_OBJECT_0:
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
		}

		Tick();
	}
}

//void KTRUser::OnRecvCompleted( IN const char cRecvCnt_, IN KSerBuffer& buff_ )
void KTRUser::OnRecvCompleted( IN KSerBuffer& buffer_ )
{
    //Client ���� �ý����� �̺�Ʈ �ĺ��� �ʿ���� ������ �ٷ� ������.
    KSerializer ks;
    KEventPtr spEvent( new KEvent );
    ks.BeginReading( &buffer_ );
    ks.Get( *spEvent );
    ks.EndReading();

    spEvent->m_kbuff.UnCompress();

    KSerBuffer* pBuff = new KSerBuffer( spEvent->m_kbuff );

    dbg::clog << L"�ި� " << spEvent->GetIDStr() << L" - TRU Recv" << dbg::endl;//END_LOG;

    if( m_pSendGameMessage != NULL )
        m_pSendGameMessage(
        m_ServerMsgID,
        static_cast<WPARAM>(spEvent->m_usEventID),
        reinterpret_cast<LPARAM>(pBuff),
        false 
        );

    //{{AFX
    //switch( cRecvCnt_ ) 
    //{
    //case EVENTID_RELAYED:   // relay. ������ �޽����� �����͸� ������.
    //	if( IsWindow( m_hWnd ) )
    //	{
    //		// SendMessage�ϸ� �޼����� ó���ɶ����� ��ٸ�����, PostMessage�̹Ƿ� ť�׸� �Ѵ�.
    //		// cmd : WM_APP + Event ID
    //		// wParam : KTRUser ��ü�� ������
    //		// lParam : data �� ������

    //		//KSerBuffer* pBuff = new KSerBuffer( buff_ );
    //		//::PostMessage( m_hWnd, WM_APP + KUserEvent::EVENT_P2P_RELAY_ACK,
    //		//	reinterpret_cast<WPARAM>(this),
    //		//	reinterpret_cast<LPARAM>(pBuff) );
    //	}
    //	break;

    //default:
    //	if( cRecvCnt_ < 0 ) // ������ ���͸� ���� ���� ���� ����. ������ �ִ�.
    //	{
    //		START_LOG( cerr, "Invalide cRecvCnt field value. data size : " << buff_.GetLength() )
    //			<< BUILD_LOGc( cRecvCnt_ )
    //			<< END_LOG;
    //		return;
    //	}
    //}
    //}}AFX
}

void KTRUser::OnDestroy()
{
	m_bDestroyReserved = false;     // ������ ���ᰡ ���� �Ǿ��� �ƴϵ�, �ѹ� Destory ȣ���� �����ϸ� ��������.

	SetEvent( m_hKillEvent );
    if( m_spSockObj )
    {
        m_spSockObj->CloseSocket();
    }
    else
    {
        START_LOG( cerr, L"���Ͽ�����Ʈ ������ �̻�." )
            << END_LOG;
    }
}

//{{AFX
//bool KTRUser::Send( IN const std::vector<UidType> vecUID_, IN const void* pvData_, IN const size_t dwDataSize_ )
//{
//	//PROXY_ONLY;
//
//	if( vecUID_.empty() ) return false;
//	if( dwDataSize_ > KSkTRUser::MAXIMUM_PACKET_SIZE ) return false;
//
//	KncSecurity::KByteStream buff;
//	buff.Append( sizeof(USHORT)                 // ��Ŷ �� ������
//		+ sizeof(char)                          // recvCnt (���۴���� ��)
//		+ sizeof(m_iUnitUID) * vecUID_.size()	// ���۹��� ����� UIDs...
//		+ dwDataSize_                           // ������ ������
//		, sizeof(USHORT) );
//
//	buff.Append( vecUID_.size(), sizeof(char) );
//
//	std::vector<UidType>::const_iterator vit;
//	for( vit = vecUID_.begin(); vit != vecUID_.end(); vit++ )
//	{
//		buff.Append( *vit );
//	}
//
//	buff.append( (byte*)pvData_, dwDataSize_ );
//
//	return m_kSockObj.SendData( (const char*)buff.data(), buff.length() );
//}
//}}AFX