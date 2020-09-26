#include "GarenaBillingUser.h"
#include "GarenaBillingIOCP.h"
//#include "Socket/IOCP.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
//#include "RelayData.h"

#include "GarenaBillingUserFSM.h"
#include "GarenaBillingServer.h"

#include "Socket/NetCommon.h"
#include "Statistics.h"
#include "KncSend.h"

//////////////////////////////////////////////////////////////////////////
//des : KSkTRUser


//{{ 2013. 04. 08	������	�ؽ� ���� ��� �̵����
#ifdef SERV_COUNTRY_PH


void KSkGarenaBillingUser::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" )
			<< BUILD_LOG( ::WSAGetLastError() );

        OnSocketError();
        return;
    }

    m_ovlRecv.m_dwLeft += dwTransfered_;

    // ��Ŷ ���� ���෮���� ũ�ٸ� ��� �� ��Ŷ ���ɼ��� üũ�Ѵ�.
    while( m_ovlRecv.m_dwLeft >= HEADER_SIZE_NGMT )
    {
        // ��Ŷ ���̸� �˾Ƴ���.
		unsigned long ulLength = 0;
		::memcpy( &ulLength, m_ovlRecv.m_pBuffer, sizeof( ulLength ) );

        //��Ŷ�� �ִ� 20byte �̻��� ���̸� ������ �Ѵ�.
		//ulLength = ::ntohl( ulLength ); -- �ؽ��� ��Ŷ�� little endian���� �ٷ� ������ �־ �ϴ� �ּ�ó��!
		unsigned long ulTotalPacketSize = ulLength + HEADER_SIZE_NGMT;
        if( ulTotalPacketSize > MAX_PACKET_SIZE_NGMT )
        {
			START_LOG( cerr, L"������ ��Ŷ ������ �̻�." )
				<< BUILD_LOG( ulTotalPacketSize )
				<< BUILD_LOG( MAX_PACKET_SIZE_NGMT )
				<< END_LOG;

            OnSocketError();
            return;
        }

        // ������ ��Ŷ�� �����ϱ⿡ ����� �����Ͱ� ����. loop�� ����������.
        if( m_ovlRecv.m_dwLeft < ulTotalPacketSize )
		{
			break;
		}

		// ���ۿ� �޾Ƶ� �����ͷ� ������ packet�� ������ ��� �������� �����Ѵ�.
        KGarenaBillingUserPtr spUser = GetGarenaBillingUserPtr();
        if( IS_NULL( spUser ) )
		{
			START_LOG( clog, L"TRUser ��ü�� �̹� ������." )
				<< END_LOG;

			OnSocketError();
			break;
		}

        KGarenaBillingPacket kPacket;
		if( kPacket.ReadFromBuffer( m_ovlRecv.m_pBuffer ) == false )
		{
			START_LOG( cerr, L"���ۿ��� �б� ����." )
				<< END_LOG;
		}
		else
		{
			spUser->OnRecvCompleted( kPacket );
		}

        // buffer management
        m_ovlRecv.m_dwLeft -= ulTotalPacketSize;
        memmove( &m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[ulTotalPacketSize], m_ovlRecv.m_dwLeft );
    }

    // recv function call
    LIF( InitRecv() );
}

void KSkGarenaBillingUser::OnSocketError()
{
    KGarenaBillingUserPtr spTRUser = GetGarenaBillingUserPtr();
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

void KSkGarenaBillingUser::OnAcceptConnection()
{
    KGarenaBillingUserPtr spTRUser = GetGarenaBillingUserPtr();
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

KGarenaBillingUserPtr KSkGarenaBillingUser::GetGarenaBillingUserPtr()
{
    KGarenaBillingUserPtr spTRUser = m_pkTRUser.lock();

    if( spTRUser )
    {
        return spTRUser;
    }
    else
    {
        return KGarenaBillingUserPtr();
    }
}

//{{ 2009. 3. 12  ������	�������Ϸ� üũ�� �Լ�
bool KSkGarenaBillingUser::SendDataWithProfile( const char* szData_, int iSize_ )
{
	KLocker lock( m_csSock );

	_JIF( m_sock != INVALID_SOCKET, return false );    // ������ ��ȿ���� ����.
	_JIF( iSize_ > 0, return false );                  // 0 byte ���� �õ��� ����ó��.
	_JIF( szData_ != NULL, return false );

	MovDataQue2Buf(); // send-queue�� ������� �����Ͱ� �ִٸ� ���۷� �ű��.

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

	// input data�� ó�� - 2. ���� �����Ͱ� ���� �ִٸ� SendQueue�� ���� �õ�.
	if( iSize_ > 0 )
	{
		KLocker lock2( m_csSendQueue );

		iSize_ -= m_kSendQueue.Push( szData_, iSize_ );

		ms_kMaxSizeInfo.Check( m_kSendQueue.GetDataSize(), m_kSendQueue.size() );
	}

	// input data�� ó�� - 3. �׷��� ���� �������� ���� �����Ͱ� ���� �ִٸ� buffer full.
	if( iSize_ > 0 )
	{
		START_LOG( cerr, L"Send Buffer Full. data size : " << iSize_
			<< L", left : " << m_ovlSend.m_dwLeft
			<< L", bSending : " << m_ovlSend.m_bSending );

		// �����͸� ������ �ʾ���. �ٽ� �������� SendPakcet �ѹ� �� ȣ���ؾ� ��.
		m_ovlSend.m_dwLeft = 0;
		m_ovlSend.m_bSending = false;

		SetDisconnectReason( KStatistics::eSIColDR_SendBufferFull );
		OnSocketError();
		return false;
	}

	// ���������� ������ �ٽ��ѹ� WsaSend�� ȣ�����ش�.
	if( m_ovlSend.m_bSending == false )
	{
		DWORD dwWrite = 0;
		m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

		int ret = ::WSASend( m_sock,    // socket
			&m_ovlSend.m_wsaBuffer,     // ���� ������
			1,                          // WSABUF�� ����
			&dwWrite,                   // [out] ��� ���۵� ��
			0,                          // Flag
			&m_ovlSend,                 // OVERLAPPED
			NULL );

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

NiImplementRootRTTI(KGarenaBillingUser);

KGarenaBillingUser::KGarenaBillingUser(void)
:
m_spSockObj(new KSkGarenaBillingUser),
m_bDestroyReserved( false )
{
	for( int i = 0; i < ENUM_TICKS::TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}
}

KGarenaBillingUser::~KGarenaBillingUser(void)
{
}

bool KGarenaBillingUser::Init()
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkTRUser = shared_from_this();
	return true;
}

void KGarenaBillingUser::Tick()
{
	if( GetStateID() == KGarenaBillingUserFSM::STATE_EXIT )
	{
		START_LOG( cout, "���� ���¿��� �ٽ� Tick ���� ��� �Դ�" );
		return ;
	}

	if( m_bDestroyReserved == true )    // ���ᰡ ����� ���¶�� �ٽ��ѹ� ����ó���� �õ��Ѵ�.
	{
		OnDestroy();            // derived class call
		return;
	}
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

void KGarenaBillingUser::OnSocketError()
{
	m_bDestroyReserved = true;
}

void KGarenaBillingUser::OnAcceptConnection()
{
	SetTick( ENUM_TICKS::CONNECT_TICK );
}

void KGarenaBillingUser::OnRecvCompleted( KGarenaBillingPacket& kPacket )
{
	//////////////////////////////////////////////////////////////////////////	
	START_LOG( cwarn, L"[�׽�Ʈ�α�] ��Ŷ ���� ��ü!" )
		<< BUILD_LOG( GetUID() );
	//////////////////////////////////////////////////////////////////////////	

	// ��Ŷ�� �ϳ� ������� �̰� �Ľ��ϱ� ���ؼ� �ٸ����� ������!
	switch( kPacket.GetPacketType() )
	{
#   undef  _ENUM
#   define _ENUM( name, id ) \
			case id: \
		{ \
		K##EJSON_GN_##name##_REQ kPacketReq; \
		kPacket.Read( kPacketReq ); \
		KEventPtr spEvent( new KEvent ); \
		UidType anTrace[2] = { GetUID(), -1 }; \
		spEvent->SetData( PI_GS_PUBLISHER_BILLING, anTrace, EJSON_GN_##name##_REQ, kPacketReq ); \
		SiKGarenaBillingServer()->QueueingEvent( spEvent ); \
		} \
		break;
#   include "GarenaBilling_def.h"
			default:
				START_LOG( cerr, L"��Ŷ Ÿ���� �̻���." )
					<< BUILD_LOG( kPacket.GetPacketType() )
					<< END_LOG;
				break;
	}

	// renew m_dwHBRecvTick
	SetTick( ENUM_TICKS::HB_TICK );
}

void KGarenaBillingUser::OnDestroy()
{
	//////////////////////////////////////////////////////////////////////////	
	START_LOG( cwarn, L"[�׽�Ʈ�α�] ���� ���� ����!" )
		<< BUILD_LOG( GetUID() );
	//////////////////////////////////////////////////////////////////////////	

	m_bDestroyReserved = false;     // ������ ���ᰡ ���� �Ǿ��� �ƴϵ�, �ѹ� Destory ȣ���� �����ϸ� ��������.

	switch( GetStateID() ) 
	{
	case KGarenaBillingUserFSM::STATE_INIT:
		StateTransition( KGarenaBillingUserFSM::INPUT_CONNECT );
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_CONNECTED:
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_REGISTERING:
		StateTransition( KGarenaBillingUserFSM::INPUT_REG_UID_FAILED );
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_REGISTERED:
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_EXIT:
		break;
	}

    JIF( m_spSockObj );
    m_spSockObj->CloseSocket();
	//{{ 2009. 3. 13  ������	�׳�iocp����
    SiKGarenaBillingIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	//SiKIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	//}}
}

void KGarenaBillingUser::SetSendQueueSize( int iQueueSize )
{
    JIF( m_spSockObj );
    m_spSockObj->SetSendQueueSize( iQueueSize );
}

bool KGarenaBillingUser::SendPacket( const KncSecurity::KByteStream& kByteStream )
{
    _JIF( m_spSockObj, return false );
	//if( m_bDestroyReserved ) return true;///< ���ᰡ ����Ǿ����� send �õ�. �̷� ��찡 �ٺ��ϹǷ�, true ����.
	_JIF( m_spSockObj->IsConnected(), return false );    ///< ������ ��ȿ���� ����.

	// send size
	//SiKTRServer()->UpdateSendPacketInfo( kByteStream.length() );

	//bool bRet = m_spSockObj->SendDataWithProfile( (const char*)kByteStream.data(), kByteStream.length() );
	bool bRet = m_spSockObj->SendData( (const char*)kByteStream.data(), kByteStream.length() );

	return bRet;
}


#endif SERV_COUNTRY_PH
//}}
