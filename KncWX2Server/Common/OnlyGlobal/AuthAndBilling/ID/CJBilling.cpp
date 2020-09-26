#include "ActorManager.h"
#include "CJBilling.h"

#ifdef SERV_COUNTRY_ID

#include "KncSend.h"
#include "CJBillingManager.h"
#include "NetError.h"

#include "BillManager.h"

#include <iomanip>
#include <KncUtil.h>

#include "GLBNMCashDll.h"
#pragma comment( lib, "GLBNMCashDll_x86_MD.lib" )

using namespace MarbleWorks::GlobalBillCash::API;

#define CJ_SHOP_CODE	( "ELSWORD" )
#define M_SUCC			(        0 )
#define	M_SUCC__S		(   "0000" )

#define	M_LEN__RESCD	(        4 )
#define M_LEN__BUFF		(     1024 )



#define CLASS_TYPE  KCJBilling


KCJBilling::KCJBilling( void )
{
}

KCJBilling::~KCJBilling( void )
{
	End( 15000 );
}

void KCJBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();
	
	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_BILLING_BALANCE_REQ );
		CASE( EBILL_BUY_PRODUCT_REQ );

		CASE( EBILL_GIFT_ITEM_REQ );	// ���� ������
		CASE( EBILL_USE_COUPON_REQ );	// ���� ������ (�ۺ� üũ���� �ٷ� ���)
	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cerr, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime )
			<< END_LOG;
	}

	return;
}


void KCJBilling::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KCJBilling::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KCJBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID )
{
	SendToKOGBillingDB( nUserUID, usEventID, char() );
}

IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
	KEPUBLISHER_BILLING_BALANCE_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
	kPacketAck.m_ulBalance = 0;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// ĳ�� ������ �ϳ��� ���°��� �Ϲ������� �ۺ�ĳ���� ����.
	kPacketAck.m_iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	// �ʿ��� ���� ����
	std::string strUserNo = GetCNString( kPacket_.m_uiPublisherUID );
	std::vector< char > veccUserNo( strUserNo.size() + 1 );
	std::copy( strUserNo.begin(), strUserNo.end(), veccUserNo.begin() );

	//���� ����
	char szRemainAmount	[ M_LEN__BUFF  + 1 ];
	char szCashType		[ M_LEN__BUFF  + 1 ];	
	char szResCD		[ M_LEN__RESCD + 1 ];

	// OUTPUT ���� �ʱ�ȭ
	ZeroMemory( szRemainAmount, sizeof( szRemainAmount ) );	
	ZeroMemory( szCashType,     sizeof( szCashType     ) );	
	ZeroMemory( szResCD,        sizeof( szResCD        ) );	

	START_LOG( clog, L"REQ_CIS__Balance �ӽ÷α�" )
		<< BUILD_LOG( strUserNo )
		<< END_LOG;

	//char* szUserNo			= "ID0000000033";

	int iOK = 0;
	iOK = REQ_CIS__Balance( CJ_SHOP_CODE,		&veccUserNo[0],
							szRemainAmount,		sizeof( szRemainAmount ),
							szCashType,			sizeof( szCashType ),
							szResCD,			sizeof( szResCD ) );
						

	START_LOG( clog, L"REQ_CIS__Balance ȣ��" )
		<< BUILD_LOG( strUserNo )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( szRemainAmount )
		<< BUILD_LOG( szCashType )
		<< BUILD_LOG( szResCD )
		<< END_LOG;

	if( iOK == M_SUCC && strncmp( szResCD, M_SUCC__S, M_LEN__RESCD ) == 0)
	{
		kPacketAck.m_ulBalance = atoi( szRemainAmount );
		kPacketAck.m_iOK = NetError::NET_OK;

		START_LOG( clog, L"REQ_CIS__Balance ȣ�� ����" )
			<< BUILD_LOG( strUserNo )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( szRemainAmount )
			<< BUILD_LOG( szCashType )
			<< BUILD_LOG( szResCD )
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"REQ_CIS__Balance ȣ�� ����" )
			<< BUILD_LOG( strUserNo )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( szRemainAmount )
			<< BUILD_LOG( szCashType )
			<< BUILD_LOG( szResCD )
			<< END_LOG;
	}

	SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{
	KEBILL_BUY_PRODUCT_ACK kPacketErr;

	// �ʿ��� ���� ����
	std::string strDeductType;
	if( kPacket_.m_vecBillBuyInfo.size() > 1 )
		strDeductType = "ND";
	else
		strDeductType = "ID";
	std::vector< char > veccDeductType( strDeductType.size() + 1 );
	std::copy( strDeductType.begin(), strDeductType.end(), veccDeductType.begin() );

	std::wstring wstrDeductList;
	{
		std::wstringstream wstrstm;
		std::vector< KBillBuyInfo >::iterator vit;
		for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); ++vit )
		{
			if( vit != kPacket_.m_vecBillBuyInfo.begin() )
				wstrstm << "=";

			wstrstm << vit->m_iProductNo << "+"
				<< vit->m_iProductID << "+"
				<< L"ELSWORD" << L"+"
				<< L"ELSWORD" << L"+"
				<< vit->m_iPoint << L"+"
				<< vit->m_wstrMemo;
		}
		wstrDeductList = wstrstm.str();
	}
	std::vector< wchar_t > veccDeductList( wstrDeductList.size() + 1 );
	std::copy( wstrDeductList.begin(), wstrDeductList.end(), veccDeductList.begin() );

	std::string strOrderNo		= KncUtil::toNarrowString( kPacket_.m_wstrTotalOrderID );
	std::vector< char > veccOrderNo( strOrderNo.size() + 1 );
	std::copy( strOrderNo.begin(), strOrderNo.end(), veccOrderNo.begin() );

	std::string strUserNo		= GetCNString( kPacket_.m_uiPublisherUID );
	std::vector< char > veccUserNo( strUserNo.size() + 1 );
	std::copy( strUserNo.begin(), strUserNo.end(), veccUserNo.begin() );

	std::vector< wchar_t > veccUserID( kPacket_.m_wstrUserID.size() + 1 );
	std::copy( kPacket_.m_wstrUserID.begin(), kPacket_.m_wstrUserID.end(), veccUserID.begin() );

	std::string strUserIP		= kPacket_.m_strUserIP;
	std::vector< char > veccUserIP( strUserIP.size() + 1 );
	std::copy( strUserIP.begin(), strUserIP.end(), veccUserIP.begin() );

	// ���� ����
	char szDeductNoList [ M_LEN__BUFF  + 1 ];
	char szResCD		[ M_LEN__RESCD + 1 ];

	// OUTPUT ���� �ʱ�ȭ
	ZeroMemory( szDeductNoList, sizeof( szDeductNoList     ) );	
	ZeroMemory( szResCD,        sizeof( szResCD            ) );

	START_LOG( clog, L"REQ_CIS__Deduct �ӽ÷α�" )
		<< BUILD_LOG( strOrderNo )
		<< BUILD_LOG( strDeductType )
		<< BUILD_LOG( strUserNo )
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( strUserIP )
		<< BUILD_LOG( wstrDeductList )
		<< END_LOG;


	//CTime tOld = CTime::GetCurrentTime();

	int iOK = 0;
	iOK = REQ_CIS__Deduct( CJ_SHOP_CODE,			&veccOrderNo[0],		&veccDeductType[0],
		&veccUserNo[0],		&veccUserID[0],			&veccUserIP[0],
		"ID",					"",					"",
		L"",					L"",				&veccDeductList[0],
		szDeductNoList,		sizeof( szDeductNoList ),
		szResCD,				sizeof( szResCD ) );

	//CTime tCurrent = CTime::GetCurrentTime();

	/*if( tCurrent.GetTime() - tOld.GetTime() > CTime( 0, 0, 0, 0, 0, 3 ).GetTime() )
	{
		START_LOG( cerr, L"REQ_CIS__Deduct ó���� 3�� �̻��� �ɷȽ��ϴ�." )
			<< BUILD_LOG( ( CStringW )tOld.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) )
			<< BUILD_LOG( ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) )
			<< BUILD_LOG( ( CStringW )( tCurrent - tOld ).Format( _T( "%Y-%m-%d %H:%M:%S" ) ) )
			<< BUILD_LOG( strOrderNo )
			<< BUILD_LOG( strDeductType )
			<< BUILD_LOG( strUserNo )
			<< BUILD_LOG( kPacket_.m_kBillingPacket.m_wstrUserID )
			<< BUILD_LOG( strUserIP )
			<< BUILD_LOG( wstrDeductList )
			<< END_LOG;
	}*/

	if( iOK == M_SUCC && strncmp( szResCD, M_SUCC__S, M_LEN__RESCD ) == 0)
	{
		START_LOG( clog, L"REQ_CIS__Deduct ȣ�� ����" )
			<< BUILD_LOG( szResCD )
			<< END_LOG;

		// ���� �� KOG Biilling �� ������ �־�޶�� ��û
		SendToKOGBillingDB( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_REQ, kPacket_ );
	}
	else
	{
		START_LOG( cerr, L"REQ_CIS__Deduct ȣ�� ����" )
			<< BUILD_LOG( strOrderNo )
			<< BUILD_LOG( strDeductType )
			<< BUILD_LOG( strUserNo )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( strUserIP )
			<< BUILD_LOG( wstrDeductList )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( szDeductNoList )
			<< BUILD_LOG( szResCD )
			<< END_LOG;

		kPacketErr.m_iOK = GetNetError( szResCD );
		SendToUser( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_ACK, kPacketErr );
	}

}


IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
	KEBILL_GIFT_ITEM_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// �Ϻ� ���� ��� ����

	START_LOG( cerr, L"�ε��׽þƴ� ���� ��� ���� ������ ���� �ڵ� Ÿ�� �ȵ�!!!!!!" )
		<< BUILD_LOG( kPacket_.m_iSenderUserUID )
		<< BUILD_LOG( kPacket_.m_iSenderUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
		<< BUILD_LOG( kPacket_.m_iReceiverUserUID )
		<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// �Ϻ� ���� ��� ����

	START_LOG( cerr, L"�ε��׽þƴ� �ۺ� ���� ���� �� �ޱ� ������ ���� �ڵ� Ÿ�� �ȵ�!!!!!!" )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSerialCode)
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_USE_COUPON_ACK, kPacketAck );
}



std::string KCJBilling::GetCNString( unsigned int uiCjUID )
{
	std::stringstream strstm;
	strstm << "ID" << std::setw( 10 ) << std::setfill( '0' ) << uiCjUID;

	return strstm.str();
}

int KCJBilling::GetNetError( const char* szResCD )
{
	int iResCD = atoi( szResCD );

	switch( iResCD )
	{
	case 1000:
		return NetError::ERR_BUY_CASH_ITEM_07;
	case 1001:
	case 1005:
		return NetError::ERR_BUY_CASH_ITEM_14;
	case 1002:
	case 1003:
	case 1004:
		return NetError::ERR_BUY_CASH_ITEM_00;
	case 1007:
		return NetError::ERR_BUY_CASH_ITEM_16;
	case 1020:
	case 9997:
		return NetError::ERR_BUY_CASH_ITEM_02;


	case 9995:
	case 9996:
	case 9999:
		return NetError::ERR_BUY_CASH_ITEM_17;


	case 1006:
	case 1008:
	case 1009:
	case 1010:
	case 1011:
	case 1012:
	case 1013:
	case 1014:
	case 1015:
	case 1016:
	case 1017:
	case 1018:
	case 1019:
	case 1021:
	case 1022:
	case 1023:
	case 1024:
	case 1025:
	case 1026:
	case 1027:
		return NetError::ERR_BUY_CASH_ITEM_01;

		break;
	}
}

#endif //SERV_COUNTRY_ID