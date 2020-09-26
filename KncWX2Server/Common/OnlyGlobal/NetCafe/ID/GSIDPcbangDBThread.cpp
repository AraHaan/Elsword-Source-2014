
//#ifdef SERV_ID_NETMARBLE_PCBANG

#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSIDPcbangDBThread.h"
#include "GameServer.h"
#include "NetError.h"

//{{ 2010.11.26   ��ȿ��    SQL_Injection ���� �ذ� 
#include <boost/algorithm/string/replace.hpp>
//}}

ImplementDBThread( KGSIDPcbangDBThread );
ImplPfID( KGSIDPcbangDBThread, PI_GS_ID_PUBLISHER_PCBANG_DB );

#define CLASS_TYPE KGSIDPcbangDBThread

KGSIDPcbangDBThread::~KGSIDPcbangDBThread()
{
}

void KGSIDPcbangDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{

	switch( spEvent_->m_usEventID )
	{

#ifdef SERV_ID_NETMARBLE_PCBANG
		CASE( DBE_ID_PCBANG_CEHCK_AUTH_REQ );
#endif //SERV_ID_NETMARBLE_PCBANG
		
		

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}



void KGSIDPcbangDBThread::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KGSIDPcbangDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

#ifdef SERV_ID_NETMARBLE_PCBANG

IMPL_ON_FUNC( DBE_ID_PCBANG_CEHCK_AUTH_REQ )
{

	KDBE_ID_PCBANG_CEHCK_AUTH_ACK kPacket;

	DO_QUERY_NO_PROFILE( L"exec dbo.up_get_check_macaddr_RS", L"N\'%s\', N\'%s\'",
		% kPacket_.m_wstrMacAdress
		% kPacket_.m_wstrPublicIP
		);

	int iRet = -2;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet );						
		m_kODBC.EndFetch();
	}

	kPacket.m_bCheckClinetDirectPcBang = kPacket_.m_bCheckClinetDirectPcBang;
	kPacket.m_iIsGameBangType = kPacket_.m_iIsGameBangType;
	kPacket.m_iIsNetmarbleGameBangType = iRet;

	START_LOG( clog, L"�ݸ��� PC �� ���� ���" )
		<< BUILD_LOG( kPacket_.m_wstrMacAdress )
		<< BUILD_LOG( kPacket_.m_wstrPublicIP )
		<< BUILD_LOG( iRet )
		<< END_LOG;

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_ID_PCBANG_CEHCK_AUTH_ACK, kPacket );

}


#endif //SERV_ID_NETMARBLE_PCBANG