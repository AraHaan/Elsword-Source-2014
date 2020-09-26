#include "ActorManager.h"
#include "NexonSOAP.h"

#include "NetError.h"


#include "../Common/SOAP/soapH.h" // or whatever it is called, you must already have it
#include "../Common/SOAP/elswordgameSoap.nsmap" // this is what you have to add to fix the problem
//#include "../Common/SOAP/elswordgameSoap12.nsmap" // this is what you have to add to fix the problem
//#include "../Common/SOAP/soapStub.h" // or whatever it is called, you must already have it

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-08-22	// �ڼ���
	#include "..\Common\Enum\Enum.h"
	#include <boost/test/utils/foreach.hpp>
#endif // SERV_NEXON_COUPON_SYSTEM

//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP

#define CLASS_TYPE  KNexonSOAP

std::map< int, KNexonSOAP::KWebMethod >  KNexonSOAP::ms_mapWebMethodPath;

_ImplementException( KNexonSOAP );

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
const	std::string	KNexonSOAP::m_strCouponSystemServiceCode( "SVG026" );
std::string	KNexonSOAP::m_strSoapServerAddressForUser( "http://api.user.nexon.com/soap/elswordgame.asmx" );
std::string	KNexonSOAP::m_strSoapServerAddressForCoupon( "http://testsoap.nexon.com/ESB.WS/NICMS2008/UseService.WebService.asmx" );
#endif // SERV_NEXON_COUPON_SYSTEM


KNexonSOAP::KNexonSOAP(std::string strWsdlPath)
{
	m_strWsdlPath = strWsdlPath;
//	m_WsmlFile = WsmlFile;

	p_nx_soap = NULL;
}

KNexonSOAP::~KNexonSOAP()
{
	End( 1500 );
}

// 2010.10.24 ��ȿ��. 
// ���� GameForgeAuth�� �Ŵ����� ��ϵ� ���� KChannelSimLayer�� m_vecpThreadMgr �̹Ƿ�
// ������ Lua�� AddWebMethod�� KGameForgeAuth::Run() ���� ������ ����ȴ�. 
void KNexonSOAP::Run()     // derived from KThread
{
	std::cout << "KSubThread::Run(), thread start." << std::endl;
	START_LOG( cout2, L"[Test Log] KNexonSOAP Start" )
		<< END_LOG;


	srand( (unsigned) time( NULL ) );

	// �߰��� �ڵ� //
	SoapInitialize();
	//


	START_LOG( cout2, L"KNexonSOAP Soap Initialized" )
		<< END_LOG;
	 
	while( true )
	{
		//{{ 2010/11/11 ��ȿ�� SOAP ��� ���� ���� ���� �� �ٽ� ������ ���� //
		if( p_nx_soap == NULL ){
			SoapInitialize();
		}
		//}}

		DWORD ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

	}

	// �߰��� �ڵ� //
	SoapUninitialize();
	//

	START_LOG( cout2, L"KNexonSOAP Soap End" )
		<< END_LOG;
}

 
void KNexonSOAP::SoapInitialize()
{
	p_nx_soap = new ElswordGameSoapProxy();
	if( !p_nx_soap )	//// couldn't allocate: stop
	{
		p_nx_soap = NULL;

		START_LOG( cerr, L"Soap_new Error" )
			<< END_LOG;

		return;
	}
}


void KNexonSOAP::SoapUninitialize()
{
	if( p_nx_soap != NULL )
	{
		soap_destroy(p_nx_soap);
		soap_end(p_nx_soap);
		p_nx_soap = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
void KNexonSOAP::AddWebMethod( int iSoapAction, std::string szMethod )
{
	if( iSoapAction < 0 || iSoapAction >= NXSOAP_NUM )
	{
		START_LOG( cerr, L"�̻��� SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}


	if( szMethod == "" )
	{
		START_LOG( cerr, L"method �̻�." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	KWebMethod kMethod;
	kMethod.m_iSoapAction = iSoapAction;
	kMethod.m_strMethodPath = szMethod;

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit != ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"�̹� �����ϴ� soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	// ������ �ִ´�.
	ms_mapWebMethodPath[iSoapAction] = kMethod;
}

bool KNexonSOAP::GetWebMethod( int iSoapAction, KWebMethod& kInfo )
{
	if( iSoapAction < 0 || iSoapAction >= NXSOAP_NUM )
	{
		START_LOG( cerr, L"�̻��� SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit == ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	kInfo = mit->second;

	return true;
}


//=========================================================================//
//			Process Event handle			
//=========================================================================//

void KNexonSOAP::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	
		//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
		CASE( SOAP_GET_TOONILAND_ID_REQ );
#endif SERV_CHANNELING_SOAP
		//}}

		CASE( SOAP_CHECK_NEXON_ID_REQ );
#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-27
		CASE( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ );
		CASE( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ );
#endif	// SERV_LOCAL_RANKING_SYSTEM

		//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT
		CASE( SOAP_CHECK_SAME_USER_REQ );
#endif SERV_RECOMMEND_LIST_EVENT
		//}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
		CASE( SOAP_COUPON_LIST_REQ );
		CASE( SOAP_REGIST_COUPON_REQ );
		CASE( SOAP_COUPON_TARGET_ITEM_REQ );
#endif // SERV_NEXON_COUPON_SYSTEM

	default:
		START_LOG( cerr, "�ڵ鷯�� �������� ���� �̺�Ʈ." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}


//////////////////////////////////////////////////////





//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
IMPL_ON_FUNC( SOAP_GET_TOONILAND_ID_REQ )
{
	if( kPacket_.m_wstrNexonID.empty() == true )
	{
		START_LOG( cerr, L"�ӽ� NexonID�� �����. ���� �� ���� ��Ȳ." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_wstrNexonID )
			<< END_LOG;
		return;
	}

	int iResult = 0;

	const std::string strTemp = KncUtil::toNarrowString( kPacket_.m_wstrNexonID );
	char arrNexonID[100];
	strncpy( arrNexonID, strTemp.c_str(), strTemp.size() );

	_ns1__GetChannelingInfo				ChannelingInfo;
	_ns1__GetChannelingInfoResponse		ChannelingInfoResponse;

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
	ChannelingInfo.n4ServiceCode = KNexonSOAP::NSC_ELSWORD;
#else // SERV_NEXON_COUPON_SYSTEM
	ChannelingInfo.n4ServiceCode = 5059;
#endif // SERV_NEXON_COUPON_SYSTEM
	ChannelingInfo.strNexonID = arrNexonID;

	iResult = soap_call___ns1__GetChannelingInfo( p_nx_soap, NULL, NULL, &ChannelingInfo, &ChannelingInfoResponse  );

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_wstrNexonID )
		<< BUILD_LOG( ChannelingInfo.n4ServiceCode )
		<< BUILD_LOG( ChannelingInfo.strNexonID )
		<< BUILD_LOG( ChannelingInfoResponse.strChannelingUID )
		<< BUILD_LOG( ChannelingInfoResponse.strChannelingID )
		<< BUILD_LOG( iResult )
		<< END_LOG;

	KSOAP_GET_TOONILAND_ID_ACK kPacketAck;
	kPacketAck.m_iOK = iResult;
	kPacketAck.m_wstrChannelingUID = KncUtil::toWideString( ChannelingInfoResponse.strChannelingUID );
	kPacketAck.m_wstrToonilandID = KncUtil::toWideString( ChannelingInfoResponse.strChannelingID );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_GET_TOONILAND_ID_ACK, kPacketAck );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}
#endif SERV_CHANNELING_SOAP
//}}

IMPL_ON_FUNC( SOAP_CHECK_NEXON_ID_REQ )
{
	if( kPacket_.m_strPassword.empty() == true )
	{
		START_LOG( cerr, L"��й�ȣ�� �����. ���� �� ���� ��Ȳ." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_strPassword )
			<< END_LOG;

		KSOAP_CHECK_NEXON_ID_ACK kPacketAck;

		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_NEXON_ID_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

	int iResult = 0;
	_ns1__CheckPasswordByMemberSN				kInfo;
	_ns1__CheckPasswordByMemberSNResponse		iResponse;

	iResponse.CheckPasswordByMemberSNResult = -1;
	kInfo.n4ServiceCode = kPacket_.m_iServiceCode;
	kInfo.memberSN = kPacket_.m_iNexonSN;
	kInfo.password = &kPacket_.m_strPassword;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		KSOAP_CHECK_NEXON_ID_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_NEXON_ID_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
	iResult = p_nx_soap->CheckPasswordByMemberSN( m_strSoapServerAddressForUser.c_str(), NULL, &kInfo, &iResponse );
#else // SERV_NEXON_COUPON_SYSTEM
	iResult = p_nx_soap->CheckPasswordByMemberSN( &kInfo, &iResponse );
#endif // SERV_NEXON_COUPON_SYSTEM

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kInfo.memberSN )
		<< BUILD_LOG( kInfo.n4ServiceCode )
		<< BUILD_LOG( iResponse.CheckPasswordByMemberSNResult )
		<< BUILD_LOG( iResult )
		<< END_LOG;

	if( 0 != iResult )
	{
		KSOAP_CHECK_NEXON_ID_ACK kPacketAck;

		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_NEXON_ID_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );

		START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kInfo.memberSN )
			<< BUILD_LOG( kInfo.n4ServiceCode )
			<< BUILD_LOG( iResponse.CheckPasswordByMemberSNResult )
			<< BUILD_LOG( iResult )
			<< END_LOG;

		return;
	}

	KSOAP_CHECK_NEXON_ID_ACK kPacketAck;

	kPacketAck.m_iOK = iResponse.CheckPasswordByMemberSNResult;
	kPacketAck.m_wstrLoginID = kPacket_.m_wstrLoginID;
	kPacketAck.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacketAck.m_wstrMessage = kPacket_.m_wstrMessage;
	kPacketAck.m_vecPurchaseReqInfo = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketAck.m_bUseCoupon		= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_NEXON_ID_ACK, kPacketAck );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-27
IMPL_ON_FUNC( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ )
{
	KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_LOCALRANKING_07;

	if( kPacket_.m_strPassword.empty() == true )
	{
		START_LOG( cerr, L"��й�ȣ�� �����. ���� �� ���� ��Ȳ." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_strPassword )
			<< END_LOG;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

	int iResult = 0;
	_ns1__CheckPasswordByMemberSN				kInfo;
	_ns1__CheckPasswordByMemberSNResponse		iResponse;

	iResponse.CheckPasswordByMemberSNResult = -1;
#ifdef	SERV_LOCAL_RANKING_SYSTEM_INHOUSE// ���볯¥: �系 ����
	iResult = 0;
#else	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE
	kInfo.n4ServiceCode = KNexonSOAP::NSC_ELSWORD;
	kInfo.memberSN		= kPacket_.m_iNexonSN;
	kInfo.password		= &kPacket_.m_strPassword;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
	iResult = p_nx_soap->CheckPasswordByMemberSN( m_strSoapServerAddressForUser.c_str(), NULL, &kInfo, &iResponse );
#else // SERV_NEXON_COUPON_SYSTEM
	iResult = p_nx_soap->CheckPasswordByMemberSN( &kInfo, &iResponse );
#endif // SERV_NEXON_COUPON_SYSTEM
#endif	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kInfo.memberSN )
		<< BUILD_LOG( kInfo.n4ServiceCode )
		<< BUILD_LOG( iResponse.CheckPasswordByMemberSNResult )
		<< BUILD_LOG( iResult )
		<< END_LOG;

	if( 0 != iResult )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );

		START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kInfo.memberSN )
			<< BUILD_LOG( kInfo.n4ServiceCode )
			<< BUILD_LOG( iResponse.CheckPasswordByMemberSNResult )
			<< BUILD_LOG( iResult )
			<< END_LOG;
		return;
	}

#ifdef	SERV_LOCAL_RANKING_SYSTEM_INHOUSE// ���볯¥: �系 ����
	kPacketAck.m_iOK = NetError::NET_OK;
#else	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE
	kPacketAck.m_iOK = iResponse.CheckPasswordByMemberSNResult;
#endif	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK, kPacketAck );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}

IMPL_ON_FUNC( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ )
{
	KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_LOCALRANKING_07;

	if( kPacket_.m_strPassword.empty() == true )
	{
		START_LOG( cerr, L"��й�ȣ�� �����. ���� �� ���� ��Ȳ." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_strPassword )
			<< END_LOG;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

	int iResult = 0;
	_ns1__CheckPasswordByMemberSN				kInfo;
	_ns1__CheckPasswordByMemberSNResponse		iResponse;

	iResponse.CheckPasswordByMemberSNResult = -1;
#ifdef	SERV_LOCAL_RANKING_SYSTEM_INHOUSE// ���볯¥: �系 ����
	iResult = 0;
#else	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE
	kInfo.n4ServiceCode = KNexonSOAP::NSC_ELSWORD;
	kInfo.memberSN = kPacket_.m_iNexonSN;
	kInfo.password = &kPacket_.m_strPassword;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
	iResult = p_nx_soap->CheckPasswordByMemberSN( m_strSoapServerAddressForUser.c_str(), NULL, &kInfo, &iResponse );
#else // SERV_NEXON_COUPON_SYSTEM
	iResult = p_nx_soap->CheckPasswordByMemberSN( &kInfo, &iResponse );
#endif // SERV_NEXON_COUPON_SYSTEM
#endif	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kInfo.memberSN )
		<< BUILD_LOG( kInfo.n4ServiceCode )
		<< BUILD_LOG( iResponse.CheckPasswordByMemberSNResult )
		<< BUILD_LOG( iResult )
		<< END_LOG;

	if( 0 != iResult )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );

		START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kInfo.memberSN )
			<< BUILD_LOG( kInfo.n4ServiceCode )
			<< BUILD_LOG( iResponse.CheckPasswordByMemberSNResult )
			<< BUILD_LOG( iResult )
			<< END_LOG;
		return;
	}

#ifdef	SERV_LOCAL_RANKING_SYSTEM_INHOUSE// ���볯¥: �系 ����
	kPacketAck.m_iOK	= NetError::NET_OK;
#else	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE
	kPacketAck.m_iOK	= iResponse.CheckPasswordByMemberSNResult;
#endif	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE
	kPacketAck.m_kInfo	= kPacket_.m_kInfo;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK, kPacketAck );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT
IMPL_ON_FUNC( SOAP_CHECK_SAME_USER_REQ )
{
	START_LOG( clog, L"SOAP_CHECK_SAME_USER_REQ ���ú� �Ϸ�" )
		<< END_LOG;

	KSOAP_CHECK_SAME_USER_ACK kPacketAck;

	kPacketAck.m_iOK = NetError::ERR_ODBC_00;
	kPacketAck.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacketAck.m_wstrNickName = kPacket_.m_wstrNickName;

	if( kPacket_.m_iNexonSN_A <= 0 || kPacket_.m_iNexonSN_B <= 0 )
	{
		START_LOG( cerr, L"�ؽ� OID ���� �̻��մϴ�." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iNexonSN_A )
			<< BUILD_LOG( kPacket_.m_iNexonSN_B )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_ODBC_00;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_SAME_USER_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

	int iResult = 0;

	_ns1__CheckSameUser				kInfo;
	_ns1__CheckSameUserResponse		iResponse;

	iResponse.CheckSameUserResult = -1;

	kInfo.n4ServiceCode = kPacket_.m_iServiceCode;
	kInfo.memberSN = kPacket_.m_iNexonSN_A;
	kInfo.compareMemberSN = kPacket_.m_iNexonSN_B;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_SAME_USER_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		return;
	}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-11-06	// �ڼ���
	iResult = p_nx_soap->CheckSameUser( m_strSoapServerAddressForUser.c_str(), NULL, &kInfo, &iResponse );
#else // SERV_NEXON_COUPON_SYSTEM
	iResult = p_nx_soap->CheckSameUser( &kInfo, &iResponse );
#endif // SERV_NEXON_COUPON_SYSTEM

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kInfo.memberSN )
		<< BUILD_LOG( kInfo.compareMemberSN )
		<< BUILD_LOG( kInfo.n4ServiceCode )
		<< BUILD_LOG( iResponse.CheckSameUserResult )
		<< BUILD_LOG( iResult )
		<< END_LOG;

	if( iResult != 0 )
	{
		kPacketAck.m_iOK = -1;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_SAME_USER_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );

		START_LOG( cerr, L"SOAP �Լ� �O�� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kInfo.memberSN )
			<< BUILD_LOG( kInfo.compareMemberSN )
			<< BUILD_LOG( kInfo.n4ServiceCode )
			<< BUILD_LOG( iResponse.CheckSameUserResult )
			<< BUILD_LOG( iResult )
			<< END_LOG;

		return;
	}

	kPacketAck.m_iOK = iResponse.CheckSameUserResult;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_CHECK_SAME_USER_ACK, kPacketAck );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
		
	START_LOG( clog, L"SOAP �Լ� �O�� ����!" )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kInfo.memberSN )
		<< BUILD_LOG( kInfo.compareMemberSN )
		<< BUILD_LOG( kInfo.n4ServiceCode )
		<< BUILD_LOG( iResponse.CheckSameUserResult )
		<< BUILD_LOG( iResult )
		<< END_LOG;
}
#endif SERV_RECOMMEND_LIST_EVENT
//}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
void StringPointerToWstring( IN const std::string* pstr, OUT std::wstring& wstr )
{
	if( pstr != NULL )
	{
		int iLen = ::MultiByteToWideChar( CP_UTF8, 0, pstr->c_str(), -1, NULL, 0 );
		wstr.resize( iLen + 1 );
		::MultiByteToWideChar( CP_UTF8, 0, pstr->c_str(), -1, const_cast<wchar_t*>( wstr.c_str() ), sizeof( wchar_t ) * iLen );
	}
}

IMPL_ON_FUNC( SOAP_COUPON_LIST_REQ )
{
	KSOAP_COUPON_LIST_ACK kPacket;
	kPacket.m_iOK				= NetError::NET_OK;
	kPacket.m_bUsed				= kPacket_.m_bUsed;
	kPacket.m_sCouponBoxType	= kPacket_.m_sCouponBoxType;
	kPacket.m_iStartRowIndex	= kPacket_.m_iStartRowIndex;

	if( kPacket_.m_bUsed == true )
	{
		// ���� ���� ī��Ʈ ���
		{
			_ns1__GetCouponBoxUsedListCountByNexonIDForUser			kInfo;
			_ns1__GetCouponBoxUsedListCountByNexonIDForUserResponse	iResponse;
			std::string	strCouponSystemServiceCode = m_strCouponSystemServiceCode;

			kInfo.nexonID		= &kPacket_.m_strNexonID;
			kInfo.serviceCode	= &strCouponSystemServiceCode;
			kInfo.couponboxType	= kPacket_.m_sCouponBoxType;

			iResponse.GetCouponBoxUsedListCountByNexonIDForUserResult = 0;

			if( p_nx_soap == NULL )
			{
				START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			const int iResult = p_nx_soap->GetCouponBoxUsedListCountByNexonIDForUser( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

			START_LOG( clog, L"SOAP �Լ� ��� ����" )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kInfo.couponboxType )
				<< BUILD_LOG( iResponse.GetCouponBoxUsedListCountByNexonIDForUserResult )
				<< END_LOG;

			if( iResult != 0 )
			{
				START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
					<< BUILD_LOG( iResult )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( *kInfo.nexonID )
					<< BUILD_LOG( *kInfo.serviceCode )
					<< BUILD_LOG( kInfo.couponboxType )
					<< BUILD_LOG( iResponse.GetCouponBoxUsedListCountByNexonIDForUserResult )
					<< END_LOG;
				
				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			kPacket.m_iCouponCount = iResponse.GetCouponBoxUsedListCountByNexonIDForUserResult;
		}

		// ���� ���� ����Ʈ ���
		if( kPacket.m_iStartRowIndex < kPacket.m_iCouponCount )	// ������ �������� �ش��ϴ� ���� ������ ���ٸ� ���� ����Ʈ�� ������ �ʿ䰡 ����.
		{
			_ns1__SelectCouponBoxUsedListByNexonIDForUser			kInfo;
			_ns1__SelectCouponBoxUsedListByNexonIDForUserResponse	iResponse;
			std::string strCouponSystemServiceCode = m_strCouponSystemServiceCode;

			kInfo.nexonID		= &kPacket_.m_strNexonID;
			kInfo.serviceCode	= &strCouponSystemServiceCode;
			kInfo.startRowIndex	= kPacket_.m_iStartRowIndex;
			kInfo.maximumRows	= SEnum::CL_INDEX_PER_PAGE;
			kInfo.couponboxType	= kPacket_.m_sCouponBoxType;

			iResponse.SelectCouponBoxUsedListByNexonIDForUserResult = NULL;

			if( p_nx_soap == NULL )
			{
				START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			const int iResult = p_nx_soap->SelectCouponBoxUsedListByNexonIDForUser( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

			START_LOG( clog, L"SOAP �Լ� ��� ����" )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kInfo.startRowIndex )
				<< BUILD_LOG( kInfo.maximumRows )
				<< BUILD_LOG( kInfo.couponboxType )
				<< END_LOG;

			if( iResult != 0 )
			{
				START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
					<< BUILD_LOG( iResult )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( *kInfo.nexonID )
					<< BUILD_LOG( *kInfo.serviceCode )
					<< BUILD_LOG( kInfo.couponboxType )
					<< END_LOG;
				
				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			if( iResponse.SelectCouponBoxUsedListByNexonIDForUserResult != NULL )
			{
				BOOST_TEST_FOREACH( const ns1__ClientCouponBox*, pClientCouponBox, iResponse.SelectCouponBoxUsedListByNexonIDForUserResult->ClientCouponBox )
				{
					if( pClientCouponBox == NULL )
						continue;

					KClientCouponBox kClientCouponBox;
					kClientCouponBox.m_iContractDetailNo	= pClientCouponBox->ContractDetailNo;
					kClientCouponBox.m_iCouponCardNo		= pClientCouponBox->CouponCardNo;
					kClientCouponBox.m_iCouponCardStatus	= pClientCouponBox->CouponCardStatus;

					StringPointerToWstring( pClientCouponBox->ContractDetailName,	kClientCouponBox.m_wstrContractDetailName		);
					StringPointerToWstring( pClientCouponBox->CouponBoxType,		kClientCouponBox.m_wstrCouponBoxType			);
					StringPointerToWstring( pClientCouponBox->CouponBoxTypeText,	kClientCouponBox.m_wstrCouponBoxTypeText		);
					StringPointerToWstring( pClientCouponBox->CouponCardStatusText,	kClientCouponBox.m_wstrCouponCardStatusText		);
					StringPointerToWstring( pClientCouponBox->DiscountValue,		kClientCouponBox.m_wstrDiscountValue			);
					StringPointerToWstring( pClientCouponBox->ExpireDateTime,		kClientCouponBox.m_wstrExpireDateTime			);
					StringPointerToWstring( pClientCouponBox->ObtainDateTime,		kClientCouponBox.m_wstrObtainDateTime			);
					StringPointerToWstring( pClientCouponBox->UsedDateTime,			kClientCouponBox.m_wstrUsedDateTime				);

					kPacket.m_vecCouponList.push_back( kClientCouponBox );
				}
			}
		}
	}
	else
	{
		// �̻�� ���� ī��Ʈ ���
		{
			_ns1__GetCouponBoxListCountByNexonIDForUser			kInfo;
			_ns1__GetCouponBoxListCountByNexonIDForUserResponse	iResponse;
			std::string strCouponSystemServiceCode = m_strCouponSystemServiceCode;

			kInfo.nexonID		= &kPacket_.m_strNexonID;
			kInfo.serviceCode	= &strCouponSystemServiceCode;
			kInfo.couponboxType	= kPacket_.m_sCouponBoxType;

			iResponse.GetCouponBoxListCountByNexonIDForUserResult = 0;

			if( p_nx_soap == NULL )
			{
				START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			const int iResult = p_nx_soap->GetCouponBoxListCountByNexonIDForUser( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

			START_LOG( clog, L"SOAP �Լ� ��� ����" )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kInfo.couponboxType )
				<< BUILD_LOG( iResponse.GetCouponBoxListCountByNexonIDForUserResult )
				<< END_LOG;

			if( iResult != 0 )
			{
				START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
					<< BUILD_LOG( iResult )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( *kInfo.nexonID )
					<< BUILD_LOG( *kInfo.serviceCode )
					<< BUILD_LOG( kInfo.couponboxType )
					<< BUILD_LOG( iResponse.GetCouponBoxListCountByNexonIDForUserResult )
					<< END_LOG;
				
				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			kPacket.m_iCouponCount = iResponse.GetCouponBoxListCountByNexonIDForUserResult;
		}

		// �̻�� ���� ����Ʈ ���
		if( kPacket.m_iStartRowIndex < kPacket.m_iCouponCount )	// ������ �������� �ش��ϴ� ���� ������ ���ٸ� ���� ����Ʈ�� ������ �ʿ䰡 ����.
		{
			_ns1__SelectCouponBoxListByNexonIDForUser			kInfo;
			_ns1__SelectCouponBoxListByNexonIDForUserResponse	iResponse;
			std::string strCouponSystemServiceCode = m_strCouponSystemServiceCode;

			kInfo.nexonID		= &kPacket_.m_strNexonID;
			kInfo.serviceCode	= &strCouponSystemServiceCode;
			kInfo.startRowIndex	= kPacket_.m_iStartRowIndex;
			kInfo.maximumRows	= SEnum::CL_INDEX_PER_PAGE;
			kInfo.couponboxType	= kPacket_.m_sCouponBoxType;

			iResponse.SelectCouponBoxListByNexonIDForUserResult = NULL;

			if( p_nx_soap == NULL )
			{
				START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			const int iResult = p_nx_soap->SelectCouponBoxListByNexonIDForUser( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

			START_LOG( clog, L"SOAP �Լ� ��� ����" )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kInfo.startRowIndex )
				<< BUILD_LOG( kInfo.maximumRows )
				<< BUILD_LOG( kInfo.couponboxType )
				<< END_LOG;

			if( iResult != 0 )
			{
				START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
					<< BUILD_LOG( iResult )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( *kInfo.nexonID )
					<< BUILD_LOG( *kInfo.serviceCode )
					<< BUILD_LOG( kInfo.couponboxType )
					<< END_LOG;
				
				kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
				goto end_proc;
			}

			if( iResponse.SelectCouponBoxListByNexonIDForUserResult != NULL )
			{
				BOOST_TEST_FOREACH( const ns1__ClientCouponBox*, pClientCouponBox, iResponse.SelectCouponBoxListByNexonIDForUserResult->ClientCouponBox )
				{
					if( pClientCouponBox == NULL )
						continue;

					KClientCouponBox kClientCouponBox;
					kClientCouponBox.m_iContractDetailNo	= pClientCouponBox->ContractDetailNo;
					kClientCouponBox.m_iCouponCardNo		= pClientCouponBox->CouponCardNo;
					kClientCouponBox.m_iCouponCardStatus	= pClientCouponBox->CouponCardStatus;

					StringPointerToWstring( pClientCouponBox->ContractDetailName,	kClientCouponBox.m_wstrContractDetailName		);
					StringPointerToWstring( pClientCouponBox->CouponBoxType,		kClientCouponBox.m_wstrCouponBoxType			);
					StringPointerToWstring( pClientCouponBox->CouponBoxTypeText,	kClientCouponBox.m_wstrCouponBoxTypeText		);
					StringPointerToWstring( pClientCouponBox->CouponCardStatusText,	kClientCouponBox.m_wstrCouponCardStatusText		);
					StringPointerToWstring( pClientCouponBox->DiscountValue,		kClientCouponBox.m_wstrDiscountValue			);
					StringPointerToWstring( pClientCouponBox->ExpireDateTime,		kClientCouponBox.m_wstrExpireDateTime			);
					StringPointerToWstring( pClientCouponBox->ObtainDateTime,		kClientCouponBox.m_wstrObtainDateTime			);
					StringPointerToWstring( pClientCouponBox->ValidDateRange,		kClientCouponBox.m_wstrValidDateRange			);

					kPacket.m_vecCouponList.push_back( kClientCouponBox );
				}
			}
		}
	}

	// ������ ������ �� �ִ� ��� ������ ���� �б�
	{
		int iTargetItemCount;

		BOOST_TEST_FOREACH( KClientCouponBox&, kInfo, kPacket.m_vecCouponList )
		{
			// ���� ���
			if( GetCouponTargetItemCount( LAST_SENDER_UID, kInfo.m_iCouponCardNo, iTargetItemCount ) != NetError::NET_OK )
			{
				START_LOG( cwarn, L"������ ������ �� �ִ� ��� ������ ���� ���� �б� ����" )
					<< END_LOG;
				continue;
			}

			// ������ ������ ������ ���ٸ� �õ� �� �ʿ䰡 ����.
			if( iTargetItemCount <= 0 )
				continue;

			// ���� ���
			if( GetCouponTargetItem( LAST_SENDER_UID, kInfo.m_iCouponCardNo, 0, iTargetItemCount, kInfo.m_vecTargetItem ) != NetError::NET_OK )
			{
				START_LOG( cwarn, L"������ ������ �� �ִ� ��� ������ ���� �б� ����" )
					<< END_LOG;
			}
		}
	}

end_proc:
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_COUPON_LIST_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}

IMPL_ON_FUNC( SOAP_REGIST_COUPON_REQ )
{
	KEGS_REGIST_COUPON_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	_ns1__AddCouponBoxByCouponPinByNexonId			kInfo;
	_ns1__AddCouponBoxByCouponPinByNexonIdResponse	iResponse;
	std::string strCouponSystemServiceCode = m_strCouponSystemServiceCode;

	kInfo.nexonId		= &kPacket_.m_strNexonID;
	kInfo.serviceCode	= &strCouponSystemServiceCode;
	kInfo.couponPin		= &kPacket_.m_strCouponPin;

	iResponse.AddCouponBoxByCouponPinByNexonIdResult = -1;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_10;	// ���� ��Ͽ� �����Ͽ����ϴ�.
		goto end_proc;
	}

	const int iResult = p_nx_soap->AddCouponBoxByCouponPinByNexonId( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( *kInfo.couponPin )
		<< BUILD_LOG( iResponse.AddCouponBoxByCouponPinByNexonIdResult )
		<< END_LOG;

	if( iResult != 0 )
	{
		START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( *kInfo.nexonId )
			<< BUILD_LOG( *kInfo.serviceCode )
			<< BUILD_LOG( *kInfo.couponPin )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_10;	// ���� ��Ͽ� �����Ͽ����ϴ�.
		goto end_proc;
	}

	switch( iResponse.AddCouponBoxByCouponPinByNexonIdResult )
	{
	case 0:
		break;

	case 1:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// �̹� ���� �����Դϴ�.
		goto end_proc;

	case 2:
	case 15:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// �Ⱓ�� ����� �����Դϴ�.
		goto end_proc;

	case 4:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_11;	// ��ҵ� �����Դϴ�.
		goto end_proc;

	case 12:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_12;	// ��ȿ���� ���� ���� ��ȣ�Դϴ�. �ٽ� Ȯ���Ͻ� �� �Է����ּ���.
		goto end_proc;

	case 31:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_13;	// ��� ���� ������ �ʰ��Ͽ����ϴ�.
		goto end_proc;

	case 50:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_15;	// PC���� �ƴմϴ�.
		goto end_proc;

	case 60:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_16;	// ���� ���� ���� �ݾ� �ʰ��Դϴ�.
		goto end_proc;

	case 61:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_17;	// ���� ���� ���� �ݾ� �̸��Դϴ�.
		goto end_proc;

	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_18;	// ���� ��� �������� �ƴմϴ�.
		goto end_proc;

	case 71:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_19;	// �̹� ��ϵ� �����Դϴ�.
		goto end_proc;

	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( iResponse.AddCouponBoxByCouponPinByNexonIdResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_10;	// ���� ��Ͽ� �����Ͽ����ϴ�.
		goto end_proc;
	}

end_proc:
	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { LAST_SENDER_UID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_REGIST_COUPON_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}

IMPL_ON_FUNC( SOAP_COUPON_TARGET_ITEM_REQ )

{
	KSOAP_COUPON_TARGET_ITEM_ACK kPacket;
	kPacket.m_iCouponCardNo		= kPacket_.m_iCouponCardNo;
	kPacket.m_iStartRowIndex	= kPacket_.m_iStartRowIndex;

	// ���� ���
	kPacket.m_iOK = GetCouponTargetItemCount( LAST_SENDER_UID, kPacket_.m_iCouponCardNo, kPacket.m_iTargetItemCount );
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"������ ������ �� �ִ� ��� ������ ���� ���� �б� ����" )
			<< END_LOG;
		goto end_proc;
	}

	// ���� ���
	kPacket.m_iOK = GetCouponTargetItem( LAST_SENDER_UID, kPacket_.m_iCouponCardNo, kPacket_.m_iStartRowIndex, kPacket.m_iTargetItemCount, kPacket.m_vecTargetItem );
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"������ ������ �� �ִ� ��� ������ ���� �б� ����" )
			<< END_LOG;
		goto end_proc;
	}

end_proc:
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, SOAP_COUPON_TARGET_ITEM_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( LAST_SENDER_UID, spEvent );
}

int	KNexonSOAP::GetCouponTargetItemCount( IN const UidType iUnitUID
										, IN const int iCouponCardNo
										, OUT int& iTargetItemCount
										) const
{
	// ������ ����Ʈ���� ���õ� ������ ��ȿ�� ������ ī��Ʈ ���
	_ns1__GetCouponBoxTargetItemListCount			kInfo;
	_ns1__GetCouponBoxTargetItemListCountResponse	iResponse;

	kInfo.couponCardNo = iCouponCardNo;

	iResponse.GetCouponBoxTargetItemListCountResult = 0;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		return NetError::ERR_NX_COUPON_14;	// �ش� ������ ���� ���� ���� ������ ��� �б⿡ �����Ͽ����ϴ�.
	}

	const int iResult = p_nx_soap->GetCouponBoxTargetItemListCount( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOG( kInfo.couponCardNo )
		<< BUILD_LOG( iResponse.GetCouponBoxTargetItemListCountResult )
		<< END_LOG;

	if( iResult != 0 )
	{
		START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( kInfo.couponCardNo )
			<< BUILD_LOG( iResponse.GetCouponBoxTargetItemListCountResult )
			<< END_LOG;

		return NetError::ERR_NX_COUPON_14;	// �ش� ������ ���� ���� ���� ������ ��� �б⿡ �����Ͽ����ϴ�.
	}

	iTargetItemCount = iResponse.GetCouponBoxTargetItemListCountResult;

	return NetError::NET_OK;
}

int	KNexonSOAP::GetCouponTargetItem( IN const UidType iUnitUID
								   , IN const int iCouponCardNo
								   , IN const int iStartRowIndex
								   , IN const int iMaximumRows
								   , OUT std::vector<KCouponBoxTargetItem>& vecTargetItem
								   ) const
{
	// ������ ����Ʈ���� ���õ� ������ ��ȿ�� ������ ����Ʈ ���
	_ns1__SelectCouponBoxTargetItemList			kInfo;
	_ns1__SelectCouponBoxTargetItemListResponse	iResponse;

	kInfo.couponCardNo	= iCouponCardNo;
	kInfo.startRowIndex	= iStartRowIndex;
	kInfo.maximumRows	= iMaximumRows;

	iResponse.SelectCouponBoxTargetItemListResult = NULL;

	if( p_nx_soap == NULL )
	{
		START_LOG( cerr, L"soap�� ����� �� ���� �����Դϴ�." )
			<< END_LOG;

		return NetError::ERR_NX_COUPON_14;	// �ش� ������ ���� ���� ���� ������ ��� �б⿡ �����Ͽ����ϴ�.
	}

	const int iResult = p_nx_soap->SelectCouponBoxTargetItemList( m_strSoapServerAddressForCoupon.c_str(), NULL, &kInfo, &iResponse );

	START_LOG( clog, L"SOAP �Լ� ��� ����" )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOG( kInfo.couponCardNo )
		<< BUILD_LOG( kInfo.startRowIndex )
		<< BUILD_LOG( kInfo.maximumRows )
		<< END_LOG;

	if( iResult != 0 )
	{
		START_LOG( cerr, L"SOAP �Լ� ȣ�� ����!" )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( kInfo.couponCardNo )
			<< BUILD_LOG( kInfo.startRowIndex )
			<< BUILD_LOG( kInfo.maximumRows )
			<< END_LOG;

		return NetError::ERR_NX_COUPON_14;	// �ش� ������ ���� ���� ���� ������ ��� �б⿡ �����Ͽ����ϴ�.
	}

	if( iResponse.SelectCouponBoxTargetItemListResult != NULL )
	{
		BOOST_TEST_FOREACH( const ns1__CouponBoxTargetItem*, pCouponBoxTargetItem, iResponse.SelectCouponBoxTargetItemListResult->CouponBoxTargetItem )
		{
			if( pCouponBoxTargetItem == NULL )
				continue;

			KCouponBoxTargetItem kCouponBoxTargetItem;
			kCouponBoxTargetItem.m_iItemQuantity	= pCouponBoxTargetItem->ItemQuantity;
			kCouponBoxTargetItem.m_sItemUseDuration	= pCouponBoxTargetItem->ItemUseDuration;
			StringPointerToWstring( pCouponBoxTargetItem->ItemCode, kCouponBoxTargetItem.m_wstrItemCode );
			StringPointerToWstring( pCouponBoxTargetItem->ItemName, kCouponBoxTargetItem.m_wstrItemName );
			vecTargetItem.push_back( kCouponBoxTargetItem );
		}
	}

	return NetError::NET_OK;
}
#endif // SERV_NEXON_COUPON_SYSTEM

#endif SERV_NEXON_AUTH_SOAP
//}}