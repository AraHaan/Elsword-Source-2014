#include "TRUser.h"
//#include "TRIOCP.h"
#include "Socket/IOCP.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
//#include "RelayData.h"

#include "TRServer.h"
#include "TRUserFSM.h"

#include "Socket/NetCommon.h"
#include "Statistics.h"

//////////////////////////////////////////////////////////////////////////
//des : KSkTRUser

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
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof( usPacketLen ) );

        //��Ŷ�� �ּ� 3byte �̻��� ���̸� ������ �Ѵ�.
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
            // get recvCnt
            //char cRecvCnt = -99; // ������� �ʴ� ������ �ʱ�ȭ.
            //memcpy( &cRecvCnt, &m_ovlRecv.m_pBuffer[ sizeof(usPacketLen) ], sizeof(cRecvCnt) );

            // get (recvUIDn, data)
            //KSerBuffer buffer;
            //buffer.Write( &m_ovlRecv.m_pBuffer[MINIMUM_PACKET_SIZE], usPacketLen - MINIMUM_PACKET_SIZE );
            //}}AFX

            KTRUserPtr spTRUser = GetTRUserPtr();
            if( spTRUser )
            {
                KSerBuffer buffer;
                buffer.Write( &m_ovlRecv.m_pBuffer[sizeof(usPacketLen)], usPacketLen - sizeof(usPacketLen) );

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
        }
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

void KSkTRUser::OnAcceptConnection()
{
    KTRUserPtr spTRUser = GetTRUserPtr();
    if( spTRUser )
    {
        spTRUser->OnAcceptConnection();
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

//{{ 2009. 3. 12  ������	�������Ϸ� üũ�� �Լ�
bool KSkTRUser::SendDataWithProfile( const char* szData_, int iSize_ )
{
	KLocker lock( m_csSock );

	_JIF( m_sock != INVALID_SOCKET, return false );    // ������ ��ȿ���� ����.
	_JIF( iSize_ > 0, return false );                  // 0 byte ���� �õ��� ����ó��.
	_JIF( szData_ != NULL, return false );

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"MovDataQue2Buf ó��" );
	//////////////////////////////////////////////////////////////////////////

	MovDataQue2Buf(); // send-queue�� ������� �����Ͱ� �ִٸ� ���۷� �ű��.

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"MovDataQue2Buf ó��" );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"���ۿ� �� ������ �ִٸ� �켱 ���� �õ�" );
	//////////////////////////////////////////////////////////////////////////

	// input data�� ó�� - 1. ���ۿ� �� ������ �ִٸ� �켱 ���� �õ�.
	int iFreeSpace = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
	if( iFreeSpace > 0 )
	{
		int iCopySize = ( iFreeSpace < iSize_ ? iFreeSpace : iSize_ );
		::memcpy( &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft], szData_, iCopySize );
		m_ovlSend.m_dwLeft += iCopySize;
		szData_ += iCopySize;
		iSize_ -= iCopySize;
	}

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"���ۿ� �� ������ �ִٸ� �켱 ���� �õ�" );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"���� �����Ͱ� ���� �ִٸ� SendQueue�� ���� �õ�." );
	//////////////////////////////////////////////////////////////////////////

	// input data�� ó�� - 2. ���� �����Ͱ� ���� �ִٸ� SendQueue�� ���� �õ�.
	if( iSize_ > 0 )
	{
		KLocker lock2( m_csSendQueue );

		iSize_ -= m_kSendQueue.Push( szData_, iSize_ );

		ms_kMaxSizeInfo.Check( m_kSendQueue.GetDataSize(), m_kSendQueue.size() );
	}

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"���� �����Ͱ� ���� �ִٸ� SendQueue�� ���� �õ�." );
	//////////////////////////////////////////////////////////////////////////

	// input data�� ó�� - 3. �׷��� ���� �������� ���� �����Ͱ� ���� �ִٸ� buffer full.
	if( iSize_ > 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		//BEGIN_PROFILE( L"�׷��� ���� �������� ���� �����Ͱ� ���� �ִٸ� buffer full." );
		//////////////////////////////////////////////////////////////////////////

		START_LOG( cerr, L"Send Buffer Full. data size : " << iSize_
			<< L", left : " << m_ovlSend.m_dwLeft
			<< L", bSending : " << m_ovlSend.m_bSending );

		// �����͸� ������ �ʾ���. �ٽ� �������� SendPakcet �ѹ� �� ȣ���ؾ� ��.
		m_ovlSend.m_dwLeft = 0;
		m_ovlSend.m_bSending = false;

		SetDisconnectReason( KStatistics::eSIColDR_SendBufferFull );
		OnSocketError();

		//////////////////////////////////////////////////////////////////////////
		//END_PROFILE( L"�׷��� ���� �������� ���� �����Ͱ� ���� �ִٸ� buffer full." );
		//////////////////////////////////////////////////////////////////////////

		return false;
	}

	// ���������� ������ �ٽ��ѹ� WsaSend�� ȣ�����ش�.
	if( m_ovlSend.m_bSending == false )
	{
		//////////////////////////////////////////////////////////////////////////
		//BEGIN_PROFILE( L"���������� ������ �ٽ��ѹ� WsaSend�� ȣ�����ش�." );
		//////////////////////////////////////////////////////////////////////////

		DWORD dwWrite = 0;
		m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

		int ret = ::WSASend( m_sock,    // socket
			&m_ovlSend.m_wsaBuffer,     // ���� ������
			1,                          // WSABUF�� ����
			&dwWrite,                   // [out] ��� ���۵� ��
			0,                          // Flag
			&m_ovlSend,                 // OVERLAPPED
			NULL );

		//////////////////////////////////////////////////////////////////////////
		//END_PROFILE( L"���������� ������ �ٽ��ѹ� WsaSend�� ȣ�����ش�." );
		//////////////////////////////////////////////////////////////////////////

		//std::cout << "raw length : " << kbuff_.GetLength() << std::endl << "SEND " << bsbuff << std::endl;

		if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // ���� pending ��..
		{
			m_ovlSend.m_bSending   = true;            
			return true;
		} 

		if( ret != SOCKET_ERROR ) // ȣ�� ���� ���� �� ����.
		{
			m_ovlSend.m_bSending   = true;
			return true;
		}

		START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG << ret );

		SetDisconnectReason( KStatistics::eSIColDR_WSASendFailed );
		OnSocketError();
		return false;
	}

	return true;
}
//}}

NiImplementRootRTTI(KTRUser);

KTRUser::KTRUser(void)
:
m_spSockObj(new KSkTRUser),
m_iUnitUID(-1),
m_bDestroyReserved( false )
{
	for( int i = 0; i < TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}
}

KTRUser::~KTRUser(void)
{
}

bool KTRUser::Init()
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkTRUser = shared_from_this();
	return true;
}

void KTRUser::Tick()
{
	if( GetStateID() == KTRUserFSM::STATE_EXIT )
	{
		START_LOG( cout, "���� ���¿��� �ٽ� Tick ���� ��� �Դ�" );
		return ;
	}

	if( m_bDestroyReserved == true )    // ���ᰡ ����� ���¶�� �ٽ��ѹ� ����ó���� �õ��Ѵ�.
	{
		OnDestroy();            // derived class call
		return;
	}

	switch( GetStateID() )
	{
	case KTRUserFSM::STATE_CONNECTED:

		if( ::GetTickCount() - GetTick( CONNECT_TICK ) > 15000) // Connect ���� 15�ʰ� UID����� ����.
		{
			START_LOG( cwarn, L"Reguard " << m_iUnitUID 
				<< L" as a Zombie.(Reason : Not Regist UID) (tick: " 
				<< ::GetTickCount() - GetTick( CONNECT_TICK ) 
				<< L" )" );

			ReserveDestroy();
		}
		break;

	case KTRUserFSM::STATE_REGISTERING:
        {
            KTRUserPtr spTRUser = shared_from_this();
		    if( !SiKTRServer()->GetTRUserManager().Insert( spTRUser ) )     // ����Ѵ�.
		    {
			    START_LOG( cwarn, L"Unit UID ��Ͻ���.!" )
				    << BUILD_LOG( m_iUnitUID )
				    << END_LOG;

			    ReserveDestroy();
		    }
		    else
			    StateTransition( KTRUserFSM::INPUT_END_REG_UID );
        }
		break;

	case KTRUserFSM::STATE_REGISTERED:
		//�κ� �׽�Ʈ������ ����.
		if( ::GetTickCount() - GetTick( HB_TICK ) > 600000 ) // 60s ������ heart-bit check
		{
			START_LOG( cwarn, L"Reguard " << m_iUnitUID 
				<< L" as a Zombie.(TRUser) (tick: " 
				<< ::GetTickCount() - GetTick( HB_TICK ) 
				<< L")" );

			ReserveDestroy(); 
		}

		break;

	case KTRUserFSM::STATE_EXIT:
		break;
	} // end switch
}
//{{AFX
//bool KTRUser::Send( IN const std::vector<unsigned char>& buff_ )
//{
//	KncSecurity::KByteStream buff;
//	// ��Ŷ �� ������ + ������ ��Ŷ�� �ǹ��ϴ� recvCnt + ������ ������
//	buff.Append( sizeof(USHORT) + sizeof(char) + buff_.size(), sizeof(USHORT) );
//	buff.Append( EVENTID_RELAYED, sizeof(char) );
//	buff.append( &buff_[0] , buff_.size() );
//
//	return m_spSockObj->SendData( (const char*)buff.data(), buff.length() );
//}
//}}AFX

void KTRUser::OnSocketError()
{
	m_bDestroyReserved = true;
}

void KTRUser::OnAcceptConnection()
{
	SetTick( CONNECT_TICK );
}

void KTRUser::OnRecvCompleted( IN KSerBuffer& buff_ )
{
	KSerializer ks;
	KEventPtr spEvent( new KEvent );
	ks.BeginReading( &buff_ );
	ks.Get( *spEvent );
	ks.EndReading();

	spEvent->m_kbuff.UnCompress();

	dbg::clog << L"�ި�" << spEvent->GetIDStr() << L"TRU Recv" << END_LOG;

	switch( spEvent->m_usEventID )
	{
	case E_HEART_BEAT: //heart bit.
		break;

	case ETR_REG_UID:	// Regist UID. TRUserManager�� ��ϵ� �ؾ� ��.
		{
			//�ϴ� üũ�����̹Ƿ� ������ �������ؼ� UID �� Ȯ���Ѵ�.
			KSerializer ks;
			UidType iUID;

			ks.BeginReading( &spEvent->m_kbuff );
			if( !ks.Get(iUID) )
			{
				START_LOG( cerr, L"deserialze failed.	ETR_REG_UID - UidType" );
			}
            ks.EndReading();

			if( m_iUnitUID != -1 )
			{
				START_LOG( cwarn, L"Duplicate UID regist. Current UID : " << m_iUnitUID 
					<< L"Recv UID : " << iUID << dbg::endl );
				break;
			}

			if( iUID <= -1 )
			{
				START_LOG( cerr, L"Recv Invalid UID: " << iUID ); 
				break;
			}

			m_iUnitUID = iUID;
			START_LOG( clog, L"Regist UID : " << m_iUnitUID );

			StateTransition( KTRUserFSM::INPUT_START_REG_UID );
		}
		break;

	case ETR_UNIT_DISCONNECT:
		{
			START_LOG( clog, L"closed by unit disconnect.!" )
				<< BUILD_LOG( m_iUnitUID )
				<< END_LOG;

			ReserveDestroy();
			return;
		}
		break;

		//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	case E_CHECK_DDOS_GUARD_REQ:
		{
			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"[�׽�Ʈ�α�] DDOS Guard�� ����߽��ϴ�!" )
				<< BUILD_LOG( GetUID() );
			//////////////////////////////////////////////////////////////////////////
			
			KncSecurity::KByteStream kByteStream;
			char cDummy = 0;
			KTRUser::MakeByteStream( kByteStream, E_CHECK_DDOS_GUARD_ACK, cDummy );
			SendPacket( kByteStream );
		}
		break;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
		//}}

	default:
		if( spEvent->m_usEventID < E_HEART_BEAT )
		{
			START_LOG( cerr, L"Invalide cRecvCnt field value. data size : " << spEvent->m_kbuff.GetLength() )
				<< BUILD_LOGc( spEvent->m_usEventID )
				<< END_LOG;
			return;
		}

		SiKTRServer()->QueueingRelayData( spEvent );
	}


	//{{AFX
	//switch( cRecvCnt_ ) 
	//{
	//case EVENTID_HEART_BIT: // heart bit.
	//	break;

	//case EVENTID_REG_UID:   // Regist UID. TRUserManager�� ��ϵ� �ؾ� �Ѵ�.
	//	{
	//		UidType iUID;
	//		buff_.Read( &iUID, sizeof(UidType) );

	//		if( m_iUnitUID != 0 )
	//		{
	//			START_LOG( cerr, L"Duplicate UID regist. Current UID : " << m_iUnitUID 
	//				<< L"Recv UID : " << iUID << dbg::endl );
	//			break;
	//		}

	//		if( iUID <= 0 )
	//		{
	//			START_LOG( cerr, L"Recv Invalid UID: " << iUID ); 
	//			break;
	//		}

	//		m_iUnitUID = iUID;
	//		START_LOG( clog, L"Regist UID : " << m_iUnitUID );

	//		StateTransition( KTRUserFSM::INPUT_START_REG_UID );
	//	}        
	//	break;

	//default:
	//	if( cRecvCnt_ < 0 ) // ������ ���͸� ���� ���� ���� ����. ������ �ִ�.
	//	{
	//		START_LOG( cerr, L"Invalide cRecvCnt field value. data size : " << buff_.GetLength() )
	//			<< BUILD_LOGc( cRecvCnt_ )
	//			<< END_LOG;
	//		return;
	//	}

	//	if( buff_.GetLength() < cRecvCnt_ * sizeof(UidType) )
	//	{
	//		START_LOG( cerr, L"Invalide packet received. data size : " << buff_.GetLength() )
	//			<< BUILD_LOGc( cRecvCnt_ )
	//			<< END_LOG;
	//		OnSocketError();
	//	}

	//	KRelayDataPtr spData( new KRelayData );
	//	spData->m_cRecvCnt = cRecvCnt_;

	//	// cRecvCnt_�� ��� ����ŭ DWORD�� �о��.
	//	spData->m_vecUID.resize( cRecvCnt_ );
	//	for( int i = 0 ; i < cRecvCnt_ ; ++i )
	//	{
	//		buff_.Read( &spData->m_vecUID[i], sizeof(UidType) );
	//	}

	//	spData->m_buff.resize( buff_.GetReadLength() );
	//	buff_.Read( &spData->m_buff[0], buff_.GetReadLength() );

	//	SiKTRServer()->QueueingRelayData( spData );
	//}
	//}}AFX

	// renew m_dwHBRecvTick
	SetTick( HB_TICK );
}

void KTRUser::OnDestroy()
{
	m_bDestroyReserved = false;     // ������ ���ᰡ ���� �Ǿ��� �ƴϵ�, �ѹ� Destory ȣ���� �����ϸ� ��������.

	switch( GetStateID() ) 
	{
	case KTRUserFSM::STATE_INIT:
		StateTransition( KTRUserFSM::INPUT_CONNECT );
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_CONNECTED:
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_REGISTERING:
		StateTransition( KTRUserFSM::INPUT_REG_UID_FAILED );
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_REGISTERED:
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_EXIT:
		break;
	}

    JIF( m_spSockObj );
    m_spSockObj->CloseSocket();
	//{{ 2009. 3. 13  ������	�׳�iocp����
    //SiKTRIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	SiKIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	//}}
}

void KTRUser::SetSendQueueSize( int iQueueSize )
{
    JIF( m_spSockObj );
    m_spSockObj->SetSendQueueSize( iQueueSize );
}

bool KTRUser::SendPacket( const KncSecurity::KByteStream& kByteStream )
{
    _JIF( m_spSockObj, return false );
	//if( m_bDestroyReserved ) return true;///< ���ᰡ ����Ǿ����� send �õ�. �̷� ��찡 �ٺ��ϹǷ�, true ����.
	_JIF( m_spSockObj->IsConnected(), return false );    ///< ������ ��ȿ���� ����.

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"send size ���" );
	//////////////////////////////////////////////////////////////////////////

	// send size
	//SiKTRServer()->UpdateSendPacketInfo( kByteStream.length() );

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"send size ���" );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"SocketObject���� SendData�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	//bool bRet = m_spSockObj->SendDataWithProfile( (const char*)kByteStream.data(), kByteStream.length() );
	bool bRet = m_spSockObj->SendData( (const char*)kByteStream.data(), kByteStream.length() );

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"SocketObject���� SendData�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	return bRet;
}



