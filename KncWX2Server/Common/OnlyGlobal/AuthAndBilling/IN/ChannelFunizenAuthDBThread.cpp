#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "ChannelFunizenAuthDBThread.h"
#include "ChannelServer.h"
#include "NetError.h"

#ifdef SERV_COUNTRY_IN
#define CLASS_TYPE KChannelFunizenAuthDBThread
ImplPfID( KChannelFunizenAuthDBThread, PI_CHANNEL_AUTH_DB );

KChannelFunizenAuthDBThread::~KChannelFunizenAuthDBThread(void)
{
}

void KChannelFunizenAuthDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {       
	CASE( DBE_PUBLISHER_AUTHENTICATION_REQ );
    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    }
}

void KChannelFunizenAuthDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KChannelFunizenAuthDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

IMPL_ON_FUNC( DBE_PUBLISHER_AUTHENTICATION_REQ )
{
	KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
	kPacketAck.m_wstrID = kPacket_.m_wstrServiceAccountID;
	kPacketAck.m_iResult = -1;

	// CALL�� ���� ȣ���� ��� ��ȣ�� �� �־��־����
	DO_QUERY( L"CALL CheckLogin(", L"\'%s\', \'%s\')", % kPacket_.m_wstrServiceAccountID % kPacket_.m_wstrServicePassword );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacketAck.m_uiPublisherUID
			>> kPacketAck.m_iResult );

		m_kODBC.EndFetch();
	}

	switch( kPacketAck.m_iResult )
	{
	case 0:	// ���� ����
		//kPacketAck.m_iOK = NetError::ERR_FUNIZEN_00;		// ������ �������� �ʽ��ϴ�.
		break;
	case 1:	// ����
		kPacketAck.m_iOK = NetError::NET_OK;
		break;
	case 2:	// ��й�ȣ Ʋ��
		//kPacketAck.m_iOK = NetError::ERR_FUNIZEN_01;		// ��й�ȣ�� Ʋ���ϴ�.
		break;
	default:
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		break;
	}
end_proc:
	START_LOG( cerr, L"���� DB ��ȯ�� Ȯ��" )
		<< BUILD_LOG( kPacketAck.m_iResult )
		<< BUILD_LOG( kPacketAck.m_iOK )
		<< BUILD_LOG( kPacketAck.m_uiPublisherUID )
		<< END_LOG;

	SendToUser( FIRST_SENDER_UID, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
}
#endif SERV_COUNTRY_IN