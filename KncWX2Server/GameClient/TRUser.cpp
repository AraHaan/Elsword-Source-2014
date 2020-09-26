#include "TRUser.h"
#include "Socket/IOCP.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
//#include "TRUserManager.h"

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
                //spTRUser->OnRecvCompleted( buffer );
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

KTRUser::KTRUser(void)
:
m_spSockObj( new KSkTRUser ),
m_iUnitUID( -1 ),
m_bDestroyReserved( false ),
m_ServerMsgID( 0 ),
ret( NULL ),
bLoop( true )
{
	for( int i = 0; i < ENUM_TICKS::TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}

	//m_hEvents[EVENT_RECV_COMPLETED] = NULL;
	//m_hEvents[EVENT_SEND_COMPLETED] = NULL;

	START_LOG( clog, L"���� ����.!" ) << END_LOG;
}

KTRUser::~KTRUser(void)
{
	//::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
	//::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
}

void KTRUser::Tick()
{
    //if( bLoop == false )
    //	return;

    //ret = NULL;
    //ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE, m_hEvents, false, 1 );     // 0.1s ����

    //switch( ret )
    //{
    //case EVENT_DISCONNECTED + WAIT_OBJECT_0:
    //	bLoop = false;
    //	break;

    //case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
    //	{
    //		m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_RECV );
    //	}
    //	break;
    //case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
    //	{
    //		m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_SEND );
    //	}
    //	break;
    //case WAIT_TIMEOUT:
    //	break;
    //}

    if( m_iUnitUID == -1 )
    {
        START_LOG( cwarn, "���� ���¿��� �ٽ� Tick ���� ��� �Դ�" );
        return;
    }

    if( m_bDestroyReserved )    // ���ᰡ ����� ���¶�� �ٽ��ѹ� ����ó���� �õ��Ѵ�.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart bit
    if( !IsConnected() )    // UserProxy�� ���, ������ ��� ����ְų� Ȥ�� ������ �� ���� �ִ� (Robot)
        return;

    if( ::GetTickCount() - GetTick( ENUM_TICKS::HB_TICK ) <= 30000 ) // 30s ������ heart-bit ����
        return;

    JIF( m_spSockObj );
    START_LOG( clog, L" Heart-bit " << m_iUnitUID << L"  :" << ::GetTickCount() - GetTick( ENUM_TICKS::HB_TICK ) );
    SetTick( ENUM_TICKS::HB_TICK );

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT) + sizeof(char), sizeof(USHORT) );
    buff.Append( E_HEART_BEAT, sizeof(char) );
    LIF( m_spSockObj->SendData( ( const char* )buff.data(), buff.length() ) );
}

bool KTRUser::Init()
{
	//m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	//m_kSockObj.SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
	//m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	//m_kSockObj.SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );

    m_spSockObj->m_pkTRUser = shared_from_this();
	return true;
}

bool KTRUser::Connect( const char* szIP_, unsigned short usPort_, const UidType iUnitUID_ )
{
	std::vector< std::pair<int,int> > vecOpt;
	vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // ������� address�� ������ �� �ְ� ���. 
	vecOpt.push_back( std::make_pair( IPPROTO_TCP, TCP_NODELAY ) ); // Nagle option�� ����. �����͸� ��Ƽ� ������ �ʰ� �� ��� �����ϵ��� �Ѵ�.

    _JIF( m_spSockObj, return false );
	_JIF( m_spSockObj->Connect( szIP_, usPort_, false, &vecOpt ), false );

	//Begin();    // thread run

	// ���� �� ��� UID�� ����Ѵ�.
	m_iUnitUID = iUnitUID_;

	// ������ �����ϸ� IOCP�� ��� ��Ų��.
	//LIF( SiKTRIocp()->AssociateSocket( &m_kSockObj ) );
	//_JIF( SiKTRIocp()->AssociateSocket( &m_kSockObj ), return false );
	if( SiKIocp()->AssociateSocket( m_spSockObj ) == false )
	{
		START_LOG(cerr, L"IOCP ��Ͻ���.!")
			<< END_LOG;
	}

	//{{AFX : ���� ������ Ŭ���̾�Ʈ ���� ���� UID ��� ��Ŷ�� �����ش�.
	//KncSecurity::KByteStream buff;
	//buff.Append( sizeof(USHORT) + sizeof(char) + sizeof(iUnitUID_), sizeof(USHORT) );
	//buff.Append( 0, sizeof(char) );
	//buff.Append( iUnitUID_ );
	//LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length() ) );
	//}}AFX

	//SendPacket( ETR_REG_UID, m_iUnitUID );

	//START_LOG( clog, "Send UID : " << iUnitUID_ );

	return true;
}

void KTRUser::OnSocketError()
{
    m_bDestroyReserved = true;
}

//bool KTRUser::Begin()
//{
//	if( KThread::Begin() )
//	{
//		m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
//		return true;
//	}
//
//	return false;
//}

//void KTRUser::Run()
//{
//	DWORD ret       = NULL;
//	bool bLoop      = true;
//
//	while( bLoop )
//	{
//		ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE, m_hEvents, false, 100 );     // 0.1s ����
//
//		switch( ret )
//		{
//		case EVENT_DISCONNECTED + WAIT_OBJECT_0:
//			bLoop = false;
//			break;
//		case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
//			{
//				m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_RECV );
//			}
//			break;
//		case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
//			{
//				m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_SEND );
//			}
//			break;
//		case WAIT_TIMEOUT:
//			break;
//		}
//
//		Tick();
//	}
//
//}

void KTRUser::OnRecvCompleted( IN KSerBuffer& buffer_ )
{
	//Client ���� �ý����� �̺�Ʈ �ĺ��� �ʿ���� ������ �ٷ� ������.
	KSerializer ks;
	//KEventPtr spEvent( new KEvent );
	KEvent kEvent;
	ks.BeginReading( &buffer_ );
	ks.Get( kEvent );
	ks.EndReading();

	kEvent.m_kbuff.UnCompress();

	KSerBuffer* pBuff = new KSerBuffer( kEvent.m_kbuff );

	dbg::clog << L"�ި� " << kEvent.GetIDStr() << L" - TRU Recv" << END_LOG;

	//if( m_pSendGameMessage != NULL )
	//	m_pSendGameMessage(
	//	m_ServerMsgID,
	//	static_cast<WPARAM>(spEvent->m_usEventID),
	//	reinterpret_cast<LPARAM>(pBuff),
	//	false 
	//	);
}

void KTRUser::OnDestroy()
{
    m_bDestroyReserved = false;     // ������ ���ᰡ ���� �Ǿ��� �ƴϵ�, �ѹ� Destory ȣ���� �����ϸ� ��������.

    //SetEvent( m_hKillEvent );

    //SiKTRUserManager()->DeleteRobot(GetUID());

    m_iUnitUID = -1;

    START_LOG( cout, L"Closed by OnDestroy" );

    JIF( m_spSockObj );
    m_spSockObj->CloseSocket();
    SiKIocp()->DeleteCompKey( m_spSockObj->GetKey() );
}
