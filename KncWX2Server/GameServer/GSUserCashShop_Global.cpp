#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "NexonBillingTCPManager.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"


//{{ 2010. 11. 15  ��ȿ��	include �߰�
#include "GSSimLayer.h"
//}}

//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#include "ClassChangeTable.h"
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	#include "NexonSOAPManager.h"
	#include "NexonSOAP.h"
#endif SERV_NEXON_AUTH_SOAP
//}}

//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	#include "..\Common\GameEvent\DailyGiftBoxManager.h"
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 10. 2	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	#include "..\Common\X2Data\XSLRandomItemManager.h"
	#include "..\Common\GameSysVal\GameSysVal.h"
#endif SERV_EVENT_BINGO
//}}

//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#include "X2Data/XSLSocketItem.h"
#endif SERV_CASH_ITEM_SOCKET_OPTION
//}

#ifdef SERV_EXCEPT_NO_SELL
#include "X2Data/XSLCashItemManager.h"
#endif //SERV_EXCEPT_NO_SELL


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//======================================================================//
#ifdef SERV_GLOBAL_BILLING

IMPL_ON_FUNC_NOPARAM( EGS_CHECK_BALANCE_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHECK_BALANCE_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ �ܾ� ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_CHECK_BALANCE_ACK kAck;
		kAck.m_iOK = NetError::NET_OK;
		kAck.m_ulBalance = 0;
		SendPacket( EGS_CHECK_BALANCE_ACK, kAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	kPacket.m_ulBalance = 0;
	// kPacket.m_GlobalCashInfo �����ڿ��� �ʱ�ȭ��

	DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
	DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
	DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();

	if (dwBillingFlag == KSimLayer::BF_NONE || dwBillingFlag == KSimLayer::BF_INTERNAL)
	{
		// �系 ������ ���� �߱� ���� ĳ�� ���� ���μ����� Ȱ����
		KEBILL_CHECK_BALANCE_REQ kPacket;
		kPacket.m_iPublisherCashBalance = 0;
		SendToKOGBillingDB( EBILL_CHECK_BALANCE_REQ, kPacket );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
	{
		// �븸, ȫ�� ���̳ʸ� ������ ���鼭 ���еǾ�� �ؼ� �����÷��׷� ���
		// �븸�� TCP ����, DB���� �� �����. ���ų� ���� �ÿ��� ���� üũ �� ���� �ϳ� ����

		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiActorUID = GetUID();
		kPacket.m_wstrAccount = GetName();
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );;

		// DB Billing
		SendToPublisherBillingDB( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );

		// TCP Billing
		SendToPublisherBilling( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
	else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
	{
		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiActorUID = GetUID();
		kPacket.m_wstrAccount = GetName();
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );;
		SendToPublisherBillingDB( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
#ifdef SERV_COUNTRY_CN
	else if( dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL )
	{
		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_wstrAccount = GetName();
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );;

		START_LOG( clog, L"EPUBLISHER_BILLING_BALANCE_REQ" )
			<< BUILD_LOG( kPacket.m_uiPublisherUID )
			<< BUILD_LOG( kPacket.m_wstrAccount )
			<< END_LOG;

		SendToLoginServer( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
#endif // SERV_COUNTRY_CN
#ifdef SERV_COUNTRY_PH
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_PH)
	{		
		// �����ƽþƴ� ���ҵ� ĳ�� KOG���� ���� 
		KEBILL_CHECK_BALANCE_REQ kPacket;
		kPacket.m_iPublisherCashBalance = 0;
		SendToKOGBillingDB( EBILL_CHECK_BALANCE_REQ, kPacket );
	}
#endif //SERV_COUNTRY_PH
	else // if(dwPublisherBillingConnectType == KSimLayer::PBCT_NORMAL)
	{
		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiActorUID = GetUID();
		kPacket.m_wstrAccount = GetName();
		kPacket.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );
#ifdef SERV_COUNTRY_IN
		kPacket.m_wstrNickName = GetCharName();
#endif SERV_COUNTRY_IN
		SendToPublisherBilling( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHECK_BALANCE_ACK );

#ifdef SERV_COUNTRY_CN
	// �߱��� ���� ĳ�� + �系 ��ȯ�� ĳ�� �Ѵ� ������� �ϹǷ�
	START_LOG(clog, L"���̾�Ʈ ĳ�� �ܾ� ��ȸ")
		<< BUILD_LOG( kPacket_.m_ulBalance )
		<< END_LOG;

	KEBILL_CHECK_BALANCE_REQ kPacket;
	kPacket.m_iPublisherCashBalance = kPacket_.m_ulBalance;
	SendToKOGBillingDB( EBILL_CHECK_BALANCE_REQ, kPacket );
	//m_GlobalCashInfo �� ���� ĳ�ð� �� ���� �� ä����

#else // SERV_COUNTRY_CN
    KEGS_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_ulBalance = kPacket_.m_ulBalance;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// EPUBLISHER_BILLING_BALANCE_ACK�� �Ѱ��� Ÿ�Կ� ���� ĳ����.
	// �߱��� ���� ��ó�� Ư�� ó�� �� �Ʒ��� _IMPL_ON_FUNC( EBILL_CHECK_BALANCE_ACK, KEGS_CHECK_BALANCE_ACK )���� ó����
	
	if(kPacket_.m_iCashType >= 0 && kPacket_.m_iCashType < KGlobalCashInfo::GCT_TOTAL_NUM)
	{
		kPacket.m_bOnlyType = true;
		kPacket.m_iCashType = kPacket_.m_iCashType;

		kPacket.m_GlobalCashInfo.m_ulCash[kPacket_.m_iCashType] = kPacket_.m_ulBalance;

		m_GlobalCashInfo.m_ulCash[kPacket_.m_iCashType] = kPacket_.m_ulBalance;
	}
	else if(kPacket_.m_iOK == NetError::NET_OK)
	{
		START_LOG( cerr, L"���� Ÿ���� ĳ�ð� �ƴѵ� ��� NET_OK�� ������!!!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_iCashType )
			<< BUILD_LOG( kPacket_.m_ulBalance)
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendPacket( EGS_CHECK_BALANCE_ACK, kPacket );
#endif // SERV_COUNTRY_CN
}

// �߱� ���� ���� �÷ο쿡�� ����ϴ� �Լ�(�系 ������ �ش� �÷ο� ���)
_IMPL_ON_FUNC( EBILL_CHECK_BALANCE_ACK, KEGS_CHECK_BALANCE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHECK_BALANCE_ACK );

	m_GlobalCashInfo = kPacket_.m_GlobalCashInfo;

	SendPacket( EGS_CHECK_BALANCE_ACK, kPacket_ );
}
//}}

IMPL_ON_FUNC( EGS_APPLY_COUPON_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_APPLY_COUPON_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ��� ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	START_LOG( clog, L"���� �Է�." )
		<< BUILD_LOG( kPacket_.m_wstrCouponSerial );

	KEGS_APPLY_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		kPacketAck.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}


#if defined (SERV_COUNTRY_TWHK)
	const int iCouponSerialSize = 20;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_TH)
	const int iCouponSerialSize = 15;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_CN)
	const int iCouponSerialSize = 20;
	int iSerialSizeNetError = NetError::ERR_GIANT_BILLING_03;
#elif defined (SERV_COUNTRY_JP)
	const int iCouponSerialSize = 30;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_US)
	const int iCouponSerialSize = 16;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_EU) || defined (SERV_COUNTRY_PH)
	const int iCouponSerialSize = 0;
	int iSerialSizeNetError = NetError::ERR_UNKNOWN;	// ������ ���� ����� �ȵ�
#else	
	const int iCouponSerialSize = 20;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#endif // SERV_COUNTRY_TWHK


	// ���� �� ���� üũ
	if( kPacket_.m_wstrCouponSerial.size() != iCouponSerialSize 
		|| iCouponSerialSize == 0 )
	{
		kPacketAck.m_iOK = iSerialSizeNetError;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}

	START_LOG( clog, L"���� ��� ��û" )
		<< BUILD_LOG( kPacket_.m_wstrCouponSerial )
		<< END_LOG;


	// ������ �ٸ����� ���� ���� ������ �����ϰ� �������� ���� �������� �߿���
	KEBILL_USE_COUPON_REQ kPacketReq;
	kPacketReq.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketReq.m_PurchaserInfo.m_iUserUID = GetUID();
	kPacketReq.m_PurchaserInfo.m_wstrUserName = GetName();
	kPacketReq.m_PurchaserInfo.m_iUnitUID = GetCharUID();
	kPacketReq.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
	kPacketReq.m_PurchaserInfo.m_ucLevel = GetLevel();
	kPacketReq.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
	kPacketReq.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
	kPacketReq.m_PurchaserInfo.m_iChannelingCode = GetChannelCode();
	kPacketReq.m_wstrSerialCode = kPacket_.m_wstrCouponSerial;


//////////////////////////////////////////////////////////////////////////
#if defined(SERV_COUNTRY_CN)
	switch( kPacket_.m_iCouponType )
	{
	case KEGS_APPLY_COUPON_REQ::CT_ITEM_CARD:
		{
			kPacketReq.m_iCouponType = KEGS_APPLY_COUPON_REQ::CT_ITEM_CARD;
			SendToLoginServer( ELG_USE_COUPON_REQ, kPacketReq );
			return;
		}
		break;
		//case KEGS_APPLY_COUPON_REQ::CT_ZONE_CARD:
		//	break;
	default:
		{
			START_LOG(cerr, L"�߸��� ī�� Ÿ��.")
				<< BUILD_LOG( kPacket_.m_iCouponType )
				<< BUILD_LOG( kPacket_.m_wstrCouponSerial )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GIANT_BILLING_03;
			SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
			return;
		}
		break;
	}
#else //SERV_COUNTRY_CN
	// �系 ���� �׽�Ʈ�� ���ؼ� //
	DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
	if (dwBillingFlag == KSimLayer::BF_NONE || dwBillingFlag == KSimLayer::BF_INTERNAL)
	{
		SendToKOGBillingDB( EBILL_USE_COUPON_REQ, kPacketReq );
		return;
	}
		
	if ( KSimLayer::GetKObj()->IsCheckCouponByPublisher() )
	{
		// ���� üũ �ۺ��� �ϰ��� ������ ó���� �� (���� �ø��� �ڵ� ��Ī�� �� �� ���� ��)

		// �� ��� KOG������ ������� ���� -> �ۺ� ���� ���� -> KOG���� ���� ������ ó�� ������ ��
		SendToKOGBillingDB( EBILL_USE_COUPON_RESERVE_REQ, kPacketReq );
	}
	else
	{
		// �ۺ� üũ�� �ƴϱ⿡ �ٷ� ��� ��û
		SendToKOGBillingDB( EBILL_USE_COUPON_REQ, kPacketReq );
	}
#endif //SERV_COUNTRY_CN
//////////////////////////////////////////////////////////////////////////

}

IMPL_ON_FUNC( EBILL_USE_COUPON_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"should be NET_OK" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_APPLY_COUPON_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
		return;
	}

	// �Ʒ� DirectPickup ���� �� EBILL_USE_COUPON_RESULT_ACK���� ��������� ��

	std::vector< KBillCouponItemInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillCouponItemInfo.begin(); vit != kPacket_.m_vecBillCouponItemInfo.end(); vit++ )
	{
		KBillCouponItemInfo& refCouponItemInfo = *vit;

		int iProductNo = refCouponItemInfo.m_iProductNO;
		KBillProductInfo kInfo;
		if( !SiKBillManager()->GetProductInfo( iProductNo, kInfo ) )
		{
			START_LOG( cerr, L"coupon error" )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			continue;
		}

		int iItemID = kInfo.m_iProductID;
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		//{{ �ٷ� PickUp �ؾ� �ϴ� �����۵� PickUp ��û��
		if ( IsDirectPickUpItem(iItemID) )
		{
			KEBILL_PICK_UP_REQ kPacketReq;
			kPacketReq.m_iTransNo = refCouponItemInfo.m_iTranNo;
			kPacketReq.m_iProductNo = refCouponItemInfo.m_iProductNO;
			kPacketReq.m_iUnitUID = GetCharUID();
#ifdef SERV_GLOBAL_CASH_PACKAGE
			// �������� ������ ������ Direct Pickup�� ��Ű�� ��ǰ�� ���� �ʴ´�.
			// ������ PickUp �ؾ���
			kPacketReq.m_byteProductKind = 0;
#endif //SERV_GLOBAL_CASH_PACKAGE
			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
		}
		//}}
	}


	KEGS_APPLY_COUPON_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_RESERVE_ACK )
{
#ifdef SERV_COUNTRY_CN
	// �߱��� ���� ������ �ȵ� ���� ��Ȳ��
	START_LOG( cerr, L"�߱��� ���� �ڵ� Ÿ�� �ȵ�!!! LoginServer �ʰ� Ÿ����." )
		<< BUILD_LOGc( kPacket_.m_iOK )
		<< END_LOG;
	kPacket_.m_iOK = NetError::ERR_UNKNOWN;
#endif //SERV_COUNTRY_CN

	//{{ megagame / 2011.08.04 / �ֹ� ���� ���нÿ� ���� ��� ��û�ϸ� �ȵ�. ����!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}
	//}}

#if defined(SERV_COUNTRY_TWHK)
	// �븸, ȫ�� ���� ó���� ���� �븸 ���� ������ Ư���ؼ� �̷��� ���� �и�
	SendToPublisherBillingDB( EBILL_USE_COUPON_REQ, kPacket_.m_kPacketReq );
#else //SERV_COUNTRY_XX
	// �ش� �ڵ�� �ۺ��� ���� üũ�� ������ Ÿ����
	if ( KSimLayer::GetKObj()->IsCheckCouponByPublisher() )
	{
		DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();
		if(dwPublisherBillingConnectType == KSimLayer::PBCT_NORMAL)
		{
			SendToPublisherBilling( EBILL_USE_COUPON_REQ, kPacket_.m_kPacketReq );
		}
		else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
		{
			SendToPublisherBillingDB( EBILL_USE_COUPON_REQ, kPacket_.m_kPacketReq );
		}
		else
		{
			START_LOG( cerr, L"���� ���� ������ ������ ����" )
				<< BUILD_LOG( dwPublisherBillingConnectType )
				<< END_LOG;
			
			KEGS_APPLY_COUPON_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_17;
			SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
			return;
		}

	}
	else
	{
		START_LOG( cerr, L"�ۺ� ���� ���� �ƴѵ� �� ����� ������???" )
			<< BUILD_LOG( kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kPacketReq.m_wstrSerialCode )
			<< END_LOG;
	}
#endif //defined(SERV_COUNTRY_TWHK)

}


IMPL_ON_FUNC( EBILL_USE_COUPON_RESULT_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ��� ����." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_APPLY_COUPON_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
		return;
	}


	// �Ʒ� DirectPickup ���� �� EBILL_USE_COUPON_RESULT_ACK���� ��������� ��

	std::vector< KBillCouponItemInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillCouponItemInfo.begin(); vit != kPacket_.m_vecBillCouponItemInfo.end(); vit++ )
	{
		KBillCouponItemInfo& refCouponItemInfo = *vit;

		int iProductNo = refCouponItemInfo.m_iProductNO;
		KBillProductInfo kInfo;
		if( !SiKBillManager()->GetProductInfo( iProductNo, kInfo ) )
		{
			START_LOG( cerr, L"coupon error" )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			continue;
		}

		int iItemID = kInfo.m_iProductID;
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		//{{ �ٷ� PickUp �ؾ� �ϴ� �����۵� PickUp ��û��
		if ( IsDirectPickUpItem(iItemID) )
		{
			KEBILL_PICK_UP_REQ kPacketReq;
			kPacketReq.m_iTransNo = refCouponItemInfo.m_iTranNo;
			kPacketReq.m_iProductNo = refCouponItemInfo.m_iProductNO;
			kPacketReq.m_iUnitUID = GetCharUID();
#ifdef SERV_GLOBAL_CASH_PACKAGE
			// �������� ������ ������ Direct Pickup�� ��Ű�� ��ǰ�� ���� �ʴ´�.
			// ������ PickUp �ؾ���
			kPacketReq.m_byteProductKind = 0;
#endif //SERV_GLOBAL_CASH_PACKAGE
			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
		}
		//}}
	}


	KEGS_APPLY_COUPON_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_GET_PURCHASED_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_PURCHASED_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ��� ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ kPacketDummy;
	int iRet = OnGetPurchasedCashItemReq( kPacket_, kPacketDummy );

	if( iRet != NetError::NET_OK )
	{
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = iRet;

		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
	}
}


IMPL_ON_FUNC( EGS_BUY_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ��ǰ ���� ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_BUY_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_BUY_CASH_ITEM_ACK kPacket;

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	// ��Ȱ�� �ѵ��� ���� ���ϰ� �Ѵ�.
	// �κ��丮 ���� ������ ���� �ִ� �Ѱ踦 ���Ѱ� �Ѵ�.	
	int iStoneToBuy = 0;
	int iInventorySlotToExpand = 1;
	std::map< int, int > mapInventorySlotToExpand;
	int iBankSlotToExpand = 0;
	int iCharSlotExpand = 0;
	int iExpandSkillSlot = 0;
	int iTotalLimitBuyCashItemCount = 0;

	//#ifdef SERV_VIP_SYSTEM
	//	int iVIPItemCount = 0;
	//#endif //SERV_VIP_SYSTEM

	std::vector< KNXBTPurchaseReqInfo >::iterator vit;
	for( vit = kPacket_.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_vecPurchaseReqInfo.end(); ++vit )
	{
		KBillProductInfo kBillProductInfo;
		int iItemID = 0;

		// {{ �������� �� �� �ִ��� ���� �˻�
		if( !SiKBillManager()->GetProductInfo( ( int )vit->m_ulProductNo, kBillProductInfo ) )
		{
			START_LOG( cerr, L"ProductInfo error." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//�����Ϸ��� ��ǰ ������ �̻��մϴ�.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}

		if( kBillProductInfo.m_bSale == false )
		{
			START_LOG( cerr, L"not sale" )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			return;
		}

		//{{ 2010. 12. 08  ������	Ư�� ��ǰ ���� ���� ����
#ifdef SERV_BUY_CASH_ITEM_LIMIT
		if( SiCXSLCashItemManager()->IsBuyCashItemLimitList( vit->m_ulProductNo ) == true )
		{
			const int iLimitBuyCashItemCount = m_kUserCashInventory.GetLimitBuyCashItemCount() + iTotalLimitBuyCashItemCount;
			if( iLimitBuyCashItemCount >= SiCXSLCashItemManager()->GetBuyCashItemLimitMaxCount() )
			{
				START_LOG( clog, L"Ư�� ��ǰ ���� ���� ���ѿ� �ɷ��� ��ǰ ���� ���ؿ�!" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_ulProductNo )
					<< BUILD_LOG( m_kUserCashInventory.GetLimitBuyCashItemCount() )
					<< BUILD_LOG( SiCXSLCashItemManager()->GetBuyCashItemLimitMaxCount() );

				// ���̻� �ش� ��ǰ�� ������ �� ����!
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_55;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			++iTotalLimitBuyCashItemCount;
		}
#endif SERV_BUY_CASH_ITEM_LIMIT
		//}}

#ifdef SERV_EXCEPT_NO_SELL
		if( kBillProductInfo.m_cCategoryNo == CXSLCashItemManager::CSDC_COUPON 
			|| kBillProductInfo.m_cCategoryNo == CXSLCashItemManager::CSDC_NO_SELL_IN_PACKAGE )
		{
			START_LOG( cerr, L"������ ��Ű�� �� �Ǹ� ���ϴ� ������" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_60;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif //SERV_EXCEPT_NO_SELL

#ifdef SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
		if( ( kBillProductInfo.m_iProductID == 60007354 || kBillProductInfo.m_iProductID == 60007355 || kBillProductInfo.m_iProductID == 60007356) 
			&& CXSLUnit::UC_ARA_LITTLE_HSIEN != GetUnitClass() )
		{
			// ���� ��ǰ�� ���� �� �� ���� �������̴�!
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM

		//{{ ����� : [2012/12/5] //		Ư�� ������ ������ �־�� ĳ�ü����� ���� ����
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		if( SiCXSLItemManager()->IsKeepItemShowItem( kBillProductInfo.m_iProductID ) == true 
			&& m_kInventory.CheckKeepItem( kBillProductInfo.m_iProductID) == false )
		{			
			// ���� ��ǰ�� ���� �� �� ���� �������̴�!
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
		//}} ����� : [2012/12/5] //		Ư�� ������ ������ �־�� ĳ�ü����� ���� ����s

		iItemID = kBillProductInfo.m_iProductID;

		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//�����Ϸ��� ��ǰ ������ �̻��մϴ�.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}


		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø��� ���� ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//�����Ϸ��� ��ǰ ������ �̻��մϴ�.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
		// }} �������� �� �� �ִ��� ���� �˻�

		switch(	pItemTemplet->m_ItemID )
		{
#ifdef SERV_EVENT_BUY_FAKE_ITEM
		case CXSLItem::CI_BUY_FAKE_ITEM:
			{
				unsigned long uiProductNo = vit->m_ulProductNo;
				kPacket_.m_vecPurchaseReqInfo.erase( vit-- );

				// ���� ���� Ƚ�� Ȯ��
				KEBILL_CHECK_BUY_FAKE_ITEM_REQ kPacketReq;
				kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
#ifdef SERV_EVENT_BUY_FAKE_ITEM_UNIT
				kPacketReq.m_iUserUID = GetCharUID();
#else //SERV_EVENT_BUY_FAKE_ITEM_UNIT
				kPacketReq.m_iUserUID = GetUID();
#endif //SERV_EVENT_BUY_FAKE_ITEM_UNIT
				kPacketReq.m_ulProductNo = uiProductNo;
				SendToKOGBillingDB( EBILL_CHECK_BUY_FAKE_ITEM_REQ, kPacketReq );

				START_LOG( clog, L"�輮��_������Ŷ��������" )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< BUILD_LOG( kPacketReq.m_ulProductNo)
					<< BUILD_LOG( kPacketReq.m_iUserUID)
					<< BUILD_LOG( kPacketReq.m_iServerGroupID)
					<< END_LOG;
			} continue;
#endif //SERV_EVENT_BUY_FAKE_ITEM



	case CXSLItem::CI_RESURRECTION_STONE: // ��Ȱ��
		iStoneToBuy += ( int )kBillProductInfo.m_cQuantity * ( int )vit->m_usOrderQuantity;
		break;
	case CXSLItem::CI_EXPAND_INVENTORY: // ����ǰ Ȯ��
		{
			START_LOG( cerr, L"This item is invalid" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_EQUIP: // ����ǰ Ȯ��( ��� )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_EQUIP, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// ���� Ȯ�� üũ �ϴ� �κ�. �̺κ��� �� �� ���� �߰� �ؾ� �Ѵ�.

			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_EQUIP, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_ACCESSORY: // ����ǰ Ȯ��( �Ǽ��縮 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_ACCESSORY, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// ���� Ȯ�� üũ �ϴ� �κ�. �̺κ��� �� �� ���� �߰� �ؾ� �Ѵ�.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_ACCESSORY, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_QUICK_SLOT: // ����ǰ Ȯ��( �Һ� )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUICK_SLOT, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// ���� Ȯ�� üũ �ϴ� �κ�. �̺κ��� �� �� ���� �߰� �ؾ� �Ѵ�.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_QUICK_SLOT, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_MATERIAL: // ����ǰ Ȯ��( ��� )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_MATERIAL, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// ���� Ȯ�� üũ �ϴ� �κ�. �̺κ��� �� �� ���� �߰� �ؾ� �Ѵ�.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_MATERIAL, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_QUEST: // ����ǰ Ȯ��( ����Ʈ )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUEST, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// ���� Ȯ�� üũ �ϴ� �κ�. �̺κ��� �� �� ���� �߰� �ؾ� �Ѵ�.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_QUEST, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_SPECIAL: // ����ǰ Ȯ��( Ư�� )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_SPECIAL, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// ���� Ȯ�� üũ �ϴ� �κ�. �̺κ��� �� �� ���� �߰� �ؾ� �Ѵ�.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_SPECIAL, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_CHAR_SLOT_1:	// �ɸ��� ���� Ȯ��
		iCharSlotExpand += 1;
		break;
	case CXSLItem::CI_EXPAND_CHAR_SLOT_2:	// �ɸ��� ���� Ȯ��
		iCharSlotExpand += 2;
		break;
	case CXSLItem::CI_EXPAND_SKILL_SLOT:	// ��ų ���� Ȯ��
	case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
		iExpandSkillSlot += 1;
		break;

		// ���� Ȯ��
	case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
	case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
	case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
	case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
	case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
		{
			// ���� �̰� ���� �Ҽ� �ִ� ������� ����!
			if( m_kInventory.GetMyNextUpgradeBankItemID() != pItemTemplet->m_ItemID )
			{
				START_LOG( cerr, L"���� ������ ����� �ƴմϴ�." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( m_kInventory.GetMyNextUpgradeBankItemID() )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_38;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			//////////////////////////////////////////////////////////////////////////
			if( CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM == iItemID )
			{
				// �κ��丮�� �縮�� �ſ뺸������ ������ ������ ȸ������ ������ �� ����!
				if( m_kInventory.IsExistWithBank( CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE ) == true )
				{
					START_LOG( cerr, L"�縮�� �ſ뺸������ �κ��丮�� ������ ������ ȸ������ ���Ÿ���." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			//////////////////////////////////////////////////////////////////////////

			// �ֹ������� �Ѱ� �̻��� �ȵ�!
			if( vit->m_usOrderQuantity > 1 )
			{
				START_LOG( cerr, L"���� ȸ������ 1���̻� �ֹ��� �� ����." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_usOrderQuantity )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// Ȯ�尡������ �˻�����!
			iBankSlotToExpand += CXSLInventory::SLOT_COUNT_ONE_LINE;
		}
		break;

	case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
		{
			// �Ϲ� ������� �ƴϸ� ������ �� ����!
			if( m_kInventory.GetBankMemberShip() != CXSLInventory::MPB_NORMAL )
			{
				START_LOG( cerr, L"���� ������ ����� �ƴմϴ�." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( m_kInventory.GetBankMemberShip() )						
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_38;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// �κ��丮�� �縮�� �ſ뺸������ ������ ������ ȸ������ ������ �� ����!
			if( m_kInventory.IsExistWithBank( CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE ) == true )
			{
				START_LOG( cerr, L"�縮�� �ſ뺸������ �κ��丮�� ������ ������ ȸ������ ���Ÿ���." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// �ֹ������� �Ѱ� �̻��� �ȵ�!
			if( vit->m_usOrderQuantity > 1 )
			{
				START_LOG( cerr, L"���� ȸ������ 1���̻� �ֹ��� �� ����." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_usOrderQuantity )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// Ȯ�尡������ �˻�����!
			iBankSlotToExpand += CXSLInventory::SLOT_COUNT_ONE_LINE * 5; // 8ĭ�� 5��
		}
		break;

		//{{ 2010. 03. 22  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
	case CXSLItem::CI_CASH_SKILL_NOTE_ITEM:
		{
			// ���� ����� ��Ʈ�� �����ϰ� �ִٸ�?
			if( m_kSkillTree.IsHaveSkillNote() == true )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SKILL_NOTE_06;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// ĳ���� ������ 20���� �̸��̸� ���� �Ұ�!
			if( GetLevel() < 20 )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SKILL_NOTE_07;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_SKILL_NOTE
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	case CXSLItem::CI_UNLIMITED_CHANGE_SECOND_JOB:
		{
			// ���� ������ ������ �����ΰ�? - Normal ������ ���� �� �� ����.
			if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			CXSLUnit::UNIT_CLASS cCurrentUnitClass = static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass());

			if( cCurrentUnitClass == CXSLUnit::UC_NONE || CXSLUnit::IsSecondChangeJob( cCurrentUnitClass ) == false )	// Ŭ���� ������ ���ų� 2�� ������ �ƴϸ�
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			//Ư������ �� ��������
			std::map< int, int > mapItem; // �κ��丮 �˻��
			// 5. �������� �˻縦 ���� �غ�
			mapItem.insert( std::make_pair( pItemTemplet->m_ItemID, 1 ) );	

			// 6. �κ��丮�� �������� �˻�
			if( !m_kInventory.IsEnoughSpaceExist( mapItem ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_02;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
			// 7. �������� �����ϸ� ����� �Ѵ�. 
			if( m_kInventory.IsExistWithBank( pItemTemplet->m_ItemID ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_CLASS_CHANGE_EVENT_01;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_UNLIMITED_SKILL_RESET_ITEM
	case CXSLItem::CI_UNLIMITED_SKILL_RESET_ITEM:
		{
			//Ư������ �� ��������
			std::map< int, int > mapItem; // �κ��丮 �˻��
			// 5. �������� �˻縦 ���� �غ�
			mapItem.insert( std::make_pair( pItemTemplet->m_ItemID, 1 ) );	

			// 6. �κ��丮�� �������� �˻�
			if( !m_kInventory.IsEnoughSpaceExist( mapItem ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_02;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
			// 7. �������� �����ϸ� ����� �Ѵ�. 
			if( m_kInventory.IsExistWithBank( pItemTemplet->m_ItemID ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_CLASS_CHANGE_EVENT_01;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		} break;
#endif SERV_UNLIMITED_SKILL_RESET_ITEM
		//}}
		//#ifdef SERV_VIP_SYSTEM
		//		case 202970: // VIP ��Ű�� ������ ���� ����
		//			{
		//				if(m_kInventory.IsExistWithBank( 202970 ))
		//				{
		//					KEGS_BUY_CASH_ITEM_ACK kPacket;
		//					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_10;
		//					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		//					
		//					iVIPItemCount += 1;
		//
		//					break;
		//				}
		//			}
		//			break;
		//#endif //SERV_VIP_SYSTEM
		//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
	case CXSLItem::CI_EXPAND_QUICK_SLOT:
		{
			// �ϳ��� ������ ���������� ���� �����ϵ��� �Ѵ�.
			int iQuickSlotToExpand = 1; 
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_E_QUICK_SLOT, iQuickSlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_EXPAND_QUICK_SLOT
		//}}
#ifdef SERV_SKILL_PAGE_SYSTEM
	case CXSLItem::CI_EXPAND_SKILL_PAGE:
		{
			KNXBTProductInfo kProductInfo;
			if( !SiKNexonBillingTCPManager()->GetProductInfo( vit->m_ulProductNo, kProductInfo ) )
			{
				START_LOG( cerr, L"���δ�Ʈ ���� ���� ����." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_ulProductNo )
					<< END_LOG;

				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			switch ( GetStateID() )
			{
			case KGSFSM::S_FIELD_MAP:
				break;

			default:
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_08;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				} break;
			}

			/// ������ Ȯ��� ������ �ִ�� Ȯ���� �� �ִ� ������ ũ�� ���� ����
			const int iTheNumberOfSkillPagesToBeAdded
				= kProductInfo.m_usProductPieces * vit->m_usOrderQuantity;

			if ( !m_kSkillTree.CanExpandSkillPage( iTheNumberOfSkillPagesToBeAdded ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;

				const int iTheNumberOfSkillPagesRemainedToLimit
					= KUserSkillTree::MAX_COUNT_OF_PAGES_AVAILABLE - static_cast<int>( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() );

				// 2������ ����� �ϴµ� 1�������� Ȯ�� ���� �� ��
				if ( iTheNumberOfSkillPagesToBeAdded == 2 &&
					iTheNumberOfSkillPagesRemainedToLimit == 1 )
					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_09;
				else
					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_02;

				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}break;
#endif // SERV_SKILL_PAGE_SYSTEM
	default:
		{
			// ���� ĳ�� ������
			if( CXSLItem::IsJobChangeCashItem( iItemID ) )
			{
				// 1. ���� ������ ���� �˻�
				if( GetLevel() < pItemTemplet->m_UseLevel )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = ( pItemTemplet->m_UseLevel <= 15 ? NetError::ERR_BUY_CASH_ITEM_40 : NetError::ERR_BUY_CASH_ITEM_41 );
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// 2. ���� ������ ���� Ÿ�԰˻�
				const char cChangeJobUnitClass = CXSLItem::GetCashItemChangeUnitClass( iItemID );
				if( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(cChangeJobUnitClass) ) != GetUnitType() )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// 3. ���� ������ ���� Ŭ���� �˻�
				if( CXSLUnit::GetUnitClassDownGrade( static_cast<CXSLUnit::UNIT_CLASS>(cChangeJobUnitClass) ) != GetUnitClass() )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}


				//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
				// ���� ���� ������ ClassChange
				if( CXSLItem::IsClassChangeCashItem( iItemID ) == true )
				{
					// ���� ������ ������ �����ΰ�? - Normal ������ ���� �� �� ����.
					if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}

					// ������ UnitClass Group ���� Ȯ�� ����
					int iNewUnitClass = CXSLItem::GetClassChangeCashItem( iItemID );
					int iUnitClass = static_cast<int>(GetUnitClass());
					if( SiKClassChangeTable()->CompareUnitClass( iUnitClass, iNewUnitClass ) == false )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}
				}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
				//}}
				break;
			}

			//{{ 2009. 8. 4  ������		��ų ���� ���� ĳ����
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			// ��� ������ �����ΰ�?
			if( SiCXSLSkillTree()->IsUnsealItemID( iItemID ) == true )
			{
				if( isCanUseUnsealCashSkillItem( iItemID ) == false )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				std::vector<int> vecSkillID;
				if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, GetUnitClass(), vecSkillID ) == true )
				{
					BOOST_TEST_FOREACH( int, iSkillID, vecSkillID )
					{
						// �̹� ���������� ��ų�ΰ�?
						if( m_kSkillTree.IsSkillUnsealed( iSkillID ) )
						{
							KEGS_BUY_CASH_ITEM_ACK kPacket;
							kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_34;
							SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
							return;
						}

						// Ȥ�ó��ؼ� ��ų��������� �˻��غ�
#ifdef SERV_SKILL_PAGE_SYSTEM
						if ( m_kSkillTree.IsExistOnUsedPage( iSkillID ) )
#else // SERV_SKILL_PAGE_SYSTEM
						if( m_kSkillTree.IsExist( iSkillID ) )
#endif // SERV_SKILL_PAGE_SYSTEM
						{
							START_LOG( cerr, L"���������� �ȵǾ��µ� ��ų�� ������? ���� �Ͼ���� �ȵǴ� ����!" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( iSkillID )
								<< END_LOG;

							// �����α׸� ����!
						}

						std::vector< int > vecItemList;
						if( SiCXSLSkillTree()->GetUnsealSkillItemList( iSkillID, vecItemList ) == false )
						{
							START_LOG( cerr, L"���ν�ų ������ ����Ʈ�� ����?" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( iSkillID )
								<< END_LOG;

							KEGS_BUY_CASH_ITEM_ACK kPacket;
							kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
							SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
							return;
						}

						std::vector< int >::const_iterator vit;
						for( vit = vecItemList.begin(); vit != vecItemList.begin(); ++vit )
						{
							if( m_kInventory.IsExistWithBank( iItemID ) == true )
							{
								KEGS_BUY_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_35;
								SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
								return;
							}
						}
					}
				}
				break;
			}
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			int iSkillID = 0;
			char cUnitClass = 0;
			if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, iSkillID, cUnitClass ) == true )
			{
				// �ش� ������ �´°�?
				if( CXSLSkillTree::CheckUnitClassForUnsealSkill( GetUnitClass(), cUnitClass ) == false )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// �̹� ���������� ��ų�ΰ�?
				if( m_kSkillTree.IsSkillUnsealed( iSkillID ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_34;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// Ȥ�ó��ؼ� ��ų��������� �˻��غ�
				if( m_kSkillTree.IsExist( iSkillID ) )
				{
					START_LOG( cerr, L"���������� �ȵǾ��µ� ��ų�� ������? ���� �Ͼ���� �ȵǴ� ����!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iSkillID )
						<< END_LOG;

					// �����α׸� ����!
				}

				std::vector< int > vecItemList;
				if( SiCXSLSkillTree()->GetUnsealSkillItemList( iSkillID, vecItemList ) == false )
				{
					START_LOG( cerr, L"���ν�ų ������ ����Ʈ�� ����?" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iSkillID )
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				std::vector< int >::const_iterator vit;
				for( vit = vecItemList.begin(); vit != vecItemList.begin(); ++vit )
				{
					if( m_kInventory.IsExistWithBank( iItemID ) == true )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_35;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}
				}

				break;
			}
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			//}}
		}
		break;			
		}

#ifdef CASH_SHOP_LIMIT_LEVEL
		if( kBillProductInfo.m_cLimitLevel > GetLevel() )
		{
			START_LOG( clog, L"���� ���� ����" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetLevel() )
				<< BUILD_LOG( kBillProductInfo.m_cLimitLevel )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_58;	// ������ �����Ͽ� ������ �� �����ϴ�.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif CASH_SHOP_LIMIT_LEVEL
	}

#ifdef SERV_EVENT_BUY_FAKE_ITEM
	if( kPacket_.m_vecPurchaseReqInfo.empty() == true )
		return;
#endif //SERV_EVENT_BUY_FAKE_ITEM


	if( iStoneToBuy > 0 )
	{
		if( m_iNumResurrectionStone + iStoneToBuy > 2000 )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_18;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}

	//#ifdef SERV_VIP_SYSTEM
	//	if(iVIPItemCount > 0)
	//	{
	//		return;
	//
	//	}
	//#endif //SERV_VIP_SYSTEM	

	if(iInventorySlotToExpand <= 0) //2011.07.22 lygan_������ // ī�װ��� �κ��丮 Ȯ�� ���� ����
	{
		return;
	}

	//{{ 2009. 8. 7  ������		����
	if( iBankSlotToExpand > 0 )
	{		
		if( !m_kInventory.IsAbleToExpandBank( iBankSlotToExpand ) )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_36;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}
	//}}

	//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
	if( iCharSlotExpand > 0 )
	{
		if( ( m_nUnitSlot + iCharSlotExpand ) > CXSLUnit::CHAR_SLOT_MAX )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_30;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}
	//}}

	if( iExpandSkillSlot > 0 )
	{
		KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE eExpirationState = m_kSkillTree.GetSkillSlotBExpirationState();

		switch( eExpirationState )
		{
		case KUserSkillTree::SSBES_EXPIRED:
		case KUserSkillTree::SSBES_NOT_EXPIRED:
			{
				// �������� �� �� �ִ�

			} break;

		default: 
		case KUserSkillTree::SSBES_PERMANENT:
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			} break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ ���� ó��
	KEBILL_BUY_PRODUCT_REQ kPacketReq;
	kPacketReq.m_wstrTotalOrderID = MakeOrderID();	// TotalOrderID�� ���� �߱�

	std::vector< KNXBTPurchaseReqInfo >::const_iterator vitReqInfo;
	for( vitReqInfo = kPacket_.m_vecPurchaseReqInfo.begin(); vitReqInfo != kPacket_.m_vecPurchaseReqInfo.end(); vitReqInfo++ )
	{
		KBillProductInfo kBillProductInfo;
		if( !SiKBillManager()->GetProductInfo( ( int )vitReqInfo->m_ulProductNo, kBillProductInfo ) )
		{
			START_LOG( cerr, L"ProductInfo error." )
				<< BUILD_LOG( vitReqInfo->m_ulProductNo )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//�����Ϸ��� ��ǰ ������ �̻��մϴ�.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}

		KBillBuyInfo kBuyInfo;
		kBuyInfo.m_wstrOrderID = MakeOrderID();
		kBuyInfo.m_iProductNo = ( int )vitReqInfo->m_ulProductNo;
		kBuyInfo.m_iPoint = kBillProductInfo.m_iSalePrice;
		kBuyInfo.m_iProductID = kBillProductInfo.m_iProductID;
		kBuyInfo.m_iCategory = ( int )kBillProductInfo.m_cCategoryNo;
		kBuyInfo.m_wstrMemo = kBillProductInfo.m_wstrProductName;
		kBuyInfo.m_usOrderQuantity = kBillProductInfo.m_cQuantity;	// ȿ�� �߰�
		kPacketReq.m_vecBillBuyInfo.push_back( kBuyInfo );
	}
	
	kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketReq.m_iChannelingCode = GetChannelCode();

	kPacketReq.m_wstrUserID = GetName();
	kPacketReq.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;

	kPacketReq.m_iUserUID = GetUID();
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_wstrNickName = GetCharName();
	kPacketReq.m_strUserIP = GetIPStr();

	//{{ ����� : [2010/7/29/] //	ĳ�� �α�
	kPacketReq.m_BuyCharInfo.m_cLevel = GetLevel();
	kPacketReq.m_BuyCharInfo.m_iUnitClass = GetUnitClass();
	//}} ����� : [2010/7/29/] //	ĳ�� �α�



#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_iUseCashType))
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUseCashType = kPacket_.m_iUseCashType;

	kPacketReq.m_iUserIP = GetIP();

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	//	�������� üũ
	SendToKOGBillingDB( EBILL_PREPARE_BUY_PRODUCT_REQ, kPacketReq );

	// �򿡼� ĳ������
	m_kUserWishList.SetBuyCashItemInWishList( kPacket_.m_bIsWishList );

	// �ǽð� ��Ȱ��
	m_kUserCashInventory.SetCashResurrectionStone( false );
}


//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( EGS_BILL_PRODUCT_INFO_REQ )
{
	KEGS_BILL_PRODUCT_INFO_ACK kPacket;

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ��ǰ ����Ʈ ��û" )
			<< BUILD_LOG( GetName() );

		kPacket.m_bFinal = true;
		SendPacket( EGS_BILL_PRODUCT_INFO_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	SiKBillManager()->GetBillProductPage( kPacket_.m_iPage, kPacket.m_vecProductInfo, kPacket.m_bFinal );

#ifdef SERV_QUICK_CASH_SHOP
	for( int i = 0; i < kPacket.m_vecProductInfo.size(); ++i )
	{
		kPacket.m_vecProductInfo[i].m_wstrProductName.resize( 0 );
		kPacket.m_vecProductInfo[i].m_wstrEndDate.resize( 0 );

#ifdef SERV_NET_CAFE_CASHSHOP
		if( CXSLItem::IsPcBangOnlyCashItem( kPacket.m_vecProductInfo[i].m_iProductID ) == true 
			&& IsPcBang() == false )
		{			
			kPacket.m_vecProductInfo.erase(&kPacket.m_vecProductInfo[i--]);	
		}
#endif //SERV_NET_CAFE_CASHSHOP
	}
#endif SERV_QUICK_CASH_SHOP

#ifdef SERV_NOTIFY_BONUS_CASH
 	KEGS_BONUS_CASH_EVENT_NOT kPacketNot;
 	kPacketNot.m_bEventStart = SiKBillManager()->GetBonusCashEvent();
 	SendPacket( EGS_BONUS_CASH_EVENT_NOT, kPacketNot );
#endif //SERV_NOTIFY_BONUS_CASH
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	kPacket.m_mapKeepShowItem = SiCXSLItemManager()->m_mapKeepShowItem;
	kPacket.m_DisCountInfoMap = SiCXSLItemManager()->m_DisCountInfoMap;
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#ifdef SERV_WISH_LIST_NO_ITEM
	kPacket.m_setWishListNoItemList = SiCXSLItemManager()->m_setWishListNoItemList;
#endif SERV_WISH_LIST_NO_ITEM
	SendPacket( EGS_BILL_PRODUCT_INFO_ACK, kPacket );

	START_LOG( clog2, L"product info ack" )
		<< BUILD_LOG( kPacket_.m_iPage )
		<< BUILD_LOG( kPacket.m_vecProductInfo.size() )
		<< BUILD_LOG( kPacket.m_bFinal )
// #ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
// 		<< BUILD_LOG( kPacketNot.m_bEventStart )
// #endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< END_LOG;
}

IMPL_ON_FUNC( EGS_BILL_INVENTORY_INQUIRY_REQ )
{
	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ĳ���� �κ��丮 ���� ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_PURCHASED_CASH_ITEM_LIST_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_BILL_INVENTORY_INQUIRY_ACK kPacket;
	if( kPacket_.m_iCurrentPage < 1 )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket );
		return;
	}

	if( kPacket_.m_nItemPerPage < 1 )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket );
		return;
	}

	SendToKOGBillingDB( EBILL_INVENTORY_INQUIRY_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EBILL_INVENTORY_INQUIRY_ACK, KEGS_BILL_INVENTORY_INQUIRY_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ ����� : [2010/8/19/] //	���� ��� ����
		std::vector< UidType > vecNeedNickName;
		//}} ����� : [2010/8/19/] //	���� ��� ����

		std::vector< KBillOrderInfo >::iterator vit;
		for( vit = kPacket_.m_vecOrderInfo.begin(); vit != kPacket_.m_vecOrderInfo.end(); vit++ )
		{
			int iProductNo = vit->m_kBillProductInfo.m_iProductNo;
			if( !SiKBillManager()->GetProductInfo( iProductNo, vit->m_kBillProductInfo ) )
			{
				START_LOG( cerr, L"product info error" )
					<< BUILD_LOG( iProductNo )
					<< END_LOG;

				vit->m_kBillProductInfo.m_iProductNo = 0;
			}

			KBillProductAttribute kAttribute;
			if( !SiKBillManager()->GetProductAttribute( vit->m_kBillProductInfo.m_iProductNo, kAttribute ) )
			{
				vit->m_vecSocketOption.clear();
			}
			else
			{
				vit->m_vecSocketOption = kAttribute.m_vecAttribute;
			}

			//////////////////////////////////////////////////////////////////////////
			//{{ ����� : [2010/8/19/] //	���� ��� ����

			if( vit->m_iFromUnitUID != 0 ) //	���� �� ���
			{
				vit->m_byteType = KBillOrderInfo::BCIT_GIFT;
				vecNeedNickName.push_back( vit->m_iFromUnitUID );
			}
		}

#ifdef SERV_GLOBAL_CASH_PACKAGE

		for( int i = 0; i < kPacket_.m_vecSubPackageInfo.size(); i++)
		{
			
			KBillPackageInfo tempSubPackageInfo;
			KBillProductInfo tempSubPackProInfo;
			KBillProductAttribute tempAttribute;
			CTime tExpirationTime;

			SiKBillManager()->GetProductInfo(kPacket_.m_vecSubPackageInfo[i], tempSubPackProInfo);

			SiKBillManager()->GetProductAttribute(kPacket_.m_vecSubPackageInfo[i], tempAttribute);

			tempSubPackageInfo.m_iPackageNo	= kPacket_.m_vecSubPackageTrans[i];
			tempSubPackageInfo.m_ulProductNo = tempSubPackProInfo.m_iProductNo;
			tempSubPackageInfo.m_usProductPieces = tempSubPackProInfo.m_cQuantity;
			tempSubPackageInfo.m_vecSocketOption = tempAttribute.m_vecAttribute;
			tempSubPackageInfo.m_wstrProductID = tempSubPackProInfo.m_wstrProductName;
			tempSubPackageInfo.m_iProductID = tempSubPackProInfo.m_iProductID;
			tempSubPackageInfo.m_sProductExpire = tempSubPackProInfo.m_cPeriod;

			kPacket_.m_vecPackageInfo.push_back(tempSubPackageInfo);
		}

#endif //SERV_GLOBAL_CASH_PACKAGE

			//	�г��� ������	��������
		if( !vecNeedNickName.empty() )
		{
			KDBE_GET_NICKNAME_BY_UNITUID_REQ kPacketReq;
			kPacketReq.m_vecBillOrderInfo = kPacket_.m_vecOrderInfo;
			kPacketReq.m_vecUID = vecNeedNickName;
#ifdef SERV_GLOBAL_CASH_PACKAGE
			kPacketReq.m_vecPackageInfo = kPacket_.m_vecPackageInfo;
			kPacketReq.m_vecSubPackageInfo = kPacket_.m_vecSubPackageInfo;
			kPacketReq.m_vecSubPackageTrans = kPacket_.m_vecSubPackageTrans;
#endif //SERV_GLOBAL_CASH_PACKAGE

			SendToGameDB( DBE_GET_NICKNAME_BY_UNITUID_REQ, kPacketReq );
			return;
		}
		//}} ����� : [2010/8/19/] //	���� ��� ����
		//////////////////////////////////////////////////////////////////////////

	}

	SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket_ );
}


//{{ 2009-11-18  ������  ������ ĳ���κ�üũ
IMPL_ON_FUNC( EBILL_PREPARE_BUY_PRODUCT_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;

		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	DWORD dwBillingFlag = GetKGSSimLayer()->GetBillingFlag();
	if (dwBillingFlag == KSimLayer::BF_NONE)
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;

		kPacket.m_iOK = NetError::ERR_GASH_10;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}
	
	DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
	DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();
	
#ifdef SERV_COUNTRY_CN
	// �߱��� ĳ�� ������ �츮�� ó����
	if ( true )
#else SERV_COUNTRY_CN
	if (dwBillingFlag == KSimLayer::BF_INTERNAL)
#endif SERV_COUNTRY_CN
	{
		// �系�� ���� �ٷ� ������ ���� �� �ְ� ��
		SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
	{	
		// �����ؼ� DB�� ������ TCP���� �� �����ؾ� ��

		// �Ϲ� ĳ�ô� TCP �� ����
		if (kPacket_.m_kEBILL_BUY_PRODUCT_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_CASH)
			SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
		// ���ҵ� ��ȯ ĳ�ô� DB�� ����
		else if (kPacket_.m_kEBILL_BUY_PRODUCT_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH)
			SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
		else
		{
			START_LOG( cerr, L"���� UseCashType�� ������ �ֽ��ϴ�. ���� �ƴ�" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_kEBILL_BUY_PRODUCT_REQ.m_iUseCashType )
				<< END_LOG;
		}

	}
	else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
	{
		// �ۺ��� ĳ�� ó���� DB ���� �ý����� �� ���
		SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_PH)
	{
		SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}
	else
	{
		// �ۺ��� ĳ�� ó���� TCP ����(SOAP����) �ý����� �� ���
		SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}

}
//}}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_ACK )
{
	std::vector< KBillOrderInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillOrderInfo.begin(); vit != kPacket_.m_vecBillOrderInfo.end(); vit++ )
	{
		int iProductNo = vit->m_kBillProductInfo.m_iProductNo;
		if( !SiKBillManager()->GetProductInfo( iProductNo, vit->m_kBillProductInfo ) )
		{
			START_LOG( cerr, L"error" )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( vit->m_wstrOrderID )
				<< END_LOG;

			continue;
		}		

		//{{ 2010. 12. 08  ������	Ư�� ��ǰ ���� ���� ����
#ifdef SERV_BUY_CASH_ITEM_LIMIT
		if( SiCXSLCashItemManager()->IsBuyCashItemLimitList( iProductNo ) == true )
		{
			m_kUserCashInventory.IncreaseBuyCashItemCount();

			START_LOG( clog, L"Ư�� ��ǰ ���� �����Ͽ� ī��Ʈ �ø���!" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( m_kUserCashInventory.GetLimitBuyCashItemCount() );
		}
#endif SERV_BUY_CASH_ITEM_LIMIT
		//}}

		int iItemID = vit->m_kBillProductInfo.m_iProductID;
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		//{{ �ٷ� PickUp �ؾ� �ϴ� �����۵� PickUp ��û��
		if ( IsDirectPickUpItem(iItemID) )
		{
			KEBILL_PICK_UP_REQ kPacketReq;
			kPacketReq.m_iTransNo = vit->m_iTransNo;
			kPacketReq.m_iProductNo = vit->m_kBillProductInfo.m_iProductNo;
			kPacketReq.m_iUnitUID = GetCharUID();
#ifdef SERV_GLOBAL_CASH_PACKAGE
			kPacketReq.m_byteProductKind = vit->m_byteProductKind;
#endif //SERV_GLOBAL_CASH_PACKAGE
			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
		}
		//}}
	}

	KEGS_BUY_CASH_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	for( vit = kPacket_.m_vecBillOrderInfo.begin(); vit != kPacket_.m_vecBillOrderInfo.end(); vit++ )
	{
		KNXBTPurchaseReqInfo kInfo;
		kInfo.m_ulProductNo = ( unsigned long )vit->m_kBillProductInfo.m_iProductNo;
		kInfo.m_usOrderQuantity = 1;
		kPacket.m_vecPurchaseReqInfo.push_back( kInfo );
	}
	kPacket.m_bIsWishList = m_kUserWishList.GetBuyCashItemInWishList();
	SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );

	// �ǽð� ��Ȱ��
	if( m_kUserCashInventory.IsCashResurrectionStone() == true  &&  kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;		
		SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
	}

	//{{ 2011. 02. 08	������	ĳ���� ���� �α�
#ifdef SERV_BUY_CASH_ITEM_LOG
	BOOST_TEST_FOREACH( const KBillOrderInfo&, kReq, kPacket_.m_vecBillOrderInfo )
	{
		KBillProductInfo kProductInfo;
		if( SiKBillManager()->GetProductInfo( kReq.m_kBillProductInfo.m_iProductNo, kProductInfo ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� ��ǰ�� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kReq.m_kBillProductInfo.m_iProductNo )
				<< END_LOG;
			continue;
		}

		// ĳ���� ���� ī��Ʈ üũ
#ifdef SERV_ACCOUNT_COUNT		
		m_kUserStatistics.IncreaseAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM );
#endif
		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM );
		// �ش� ĳ���Ͱ� ���ʷ� ĳ���� �����Ѱ��̶��?
		if( m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM ) == 1 )
		{
			WriteCharacterLogToDBWithProductNo( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_BUY_CASH_ITEM,kReq.m_kBillProductInfo.m_iProductNo );
		}
#endif SERV_CHAR_LOG
		//}}

		// ĳ���� ���� �α� �����
		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_BUY_CASH_ITEM_LOG_NOT kPacketToLog;
		kPacketToLog.m_iUserUID			= GetUID();
		kPacketToLog.m_iServerGroupID	= KBaseServer::GetKObj()->GetServerGroupID();
		kPacketToLog.m_iUnitUID			= GetCharUID();
		kPacketToLog.m_cUnitClass		= GetUnitClass();
		kPacketToLog.m_ucLevel			= GetLevel();
#ifdef SERV_ACCOUNT_COUNT
		kPacketToLog.m_bIsFirstBuy		= m_kUserStatistics.IsFirstBuy();
#else
		kPacketToLog.m_bIsFirstBuy		= false;
#endif
		kPacketToLog.m_cBuyType			= KELOG_BUY_CASH_ITEM_LOG_NOT::CIBT_BUY;
		kPacketToLog.m_iProductNo		= kProductInfo.m_iProductNo;
		kPacketToLog.m_iBuyQuantity		= ( int )kReq.m_kBillProductInfo.m_cQuantity;
		//kPacketToLog.m_iTotalSalePrice	= kReq.m_kBillProductInfo.m_cQuantity * kProductInfo.m_iRealPrice;
		kPacketToLog.m_iTotalSalePrice	= kProductInfo.m_iRealPrice;
		kPacketToLog.m_wstrRegDate		= ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( ELOG_BUY_CASH_ITEM_LOG_NOT, kPacketToLog );
	}
#endif SERV_BUY_CASH_ITEM_LOG
	//}}
}

IMPL_ON_FUNC( EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ )
{
	KEGS_GET_PURCHASED_CASH_ITEM_REQ kPacketDummy;
	int iRet = OnGetPurchasedCashItemReq( kPacketDummy, kPacket_ );

	if( iRet != NetError::NET_OK )
	{
		KEGS_BILL_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = iRet;

		SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
	}
}

IMPL_ON_FUNC( EBILL_PICK_UP_ACK )
{
	KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK kPacketDummy;
	KEGS_BILL_GET_PURCHASED_CASH_ITEM_ACK kPacket;
	kPacket.m_iOK = OnPickUpAck( kPacketDummy, kPacket_, kPacket.m_vecInventorySlotInfo );
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
	}

	if( kPacket.m_iOK == NetError::ERR_NX_SHOP_09 )
	{
		return;
	}

	SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
}


IMPL_ON_FUNC( EBILL_GIFT_ITEM_ACK )
{
	// ���� ������?
	SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket_ );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ���� ������ �������� ã�Ƽ� �޽��� �˷��ֱ�
		KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT kPacketToLg;
		kPacketToLg.m_wstrSenderNickName = GetCharName();
		kPacketToLg.m_wstrReceiverUserID = m_kUserCashInventory.GetReceiverUserID();
		kPacketToLg.m_wstrMessage		 = m_kUserCashInventory.GetMessage();

		std::vector< KBillOrderInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecBillOrderInfo.begin(); vit != kPacket_.m_vecBillOrderInfo.end(); ++vit )
		{
			SiKBillManager()->GetProductInfo( vit->m_kBillProductInfo.m_iProductNo, kPacketToLg.m_kBillProductInfo );
		}

		SendToLoginServer( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, kPacketToLg );
	}

	//{{ 2011. 02. 08	������	ĳ���� ���� �α�
#ifdef SERV_BUY_CASH_ITEM_LOG
	BOOST_TEST_FOREACH( const KBillOrderInfo&, order, kPacket_.m_vecBillOrderInfo )
	{
		/*
		KNXBTProductInfo kProductInfo;
		if( SiKNexonBillingTCPManager()->GetProductInfo( kReq.m_ulProductNo, kProductInfo ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� ��ǰ�� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kReq.m_ulProductNo )
				<< END_LOG;
			continue;
		}
		}*/
		const KBillProductInfo& kProductInfo = order.m_kBillProductInfo;

		// ĳ���� ���� ī��Ʈ üũ
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM );
		m_kUserStatistics.IncreaseAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM );

		// �ش� ĳ���Ͱ� ���ʷ� ĳ���� �����Ѱ��̶��?
		if( m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM ) == 1 )
		{
			//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
			WriteCharacterLogToDBWithProductNo( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_BUY_CASH_ITEM, order.m_iTransNo );
#endif SERV_CHAR_LOG
			//}}
		}

		// ĳ���� ���� �α� �����
		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_BUY_CASH_ITEM_LOG_NOT kPacketToLog;
		kPacketToLog.m_iUserUID			= GetUID();
		kPacketToLog.m_iServerGroupID	= KBaseServer::GetKObj()->GetServerGroupID();
		kPacketToLog.m_iUnitUID			= GetCharUID();
		kPacketToLog.m_cUnitClass		= GetUnitClass();
		kPacketToLog.m_ucLevel			= GetLevel();
		kPacketToLog.m_bIsFirstBuy		= m_kUserStatistics.IsFirstBuy();
		kPacketToLog.m_cBuyType			= KELOG_BUY_CASH_ITEM_LOG_NOT::CIBT_PRESENT;
		kPacketToLog.m_iProductNo		= kProductInfo.m_iProductNo;
		kPacketToLog.m_iBuyQuantity		= kProductInfo.m_cQuantity;
		kPacketToLog.m_iTotalSalePrice	= kProductInfo.m_cQuantity * kProductInfo.m_iSalePrice;
		kPacketToLog.m_wstrRegDate		= ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		//{{ 2011. 10. 11    ��μ�    ĳ�� ������ ���� �� �� �޴� ������ ���� DB�� ����ϱ�
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
		kPacketToLog.m_wstrReceiverUserID = m_kUserCashInventory.GetReceiverUserID();
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
		//}} 

		START_LOG( clog, L"�����ϱ� �α� Ȯ��" )
			<< BUILD_LOG( m_kUserCashInventory.GetReceiverUserID() );

		SendToLogDB( ELOG_BUY_CASH_ITEM_LOG_NOT, kPacketToLog );
	}
#endif SERV_BUY_CASH_ITEM_LOG
	//}}
}


//{{ ����� : [2010/8/19/] //	���� ��� ����
IMPL_ON_FUNC( DBE_GET_NICKNAME_BY_UNITUID_ACK )
{
	KEGS_BILL_INVENTORY_INQUIRY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecOrderInfo = kPacket_.m_vecBillOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	kPacket.m_vecPackageInfo = kPacket_.m_vecPackageInfo;
	kPacket.m_vecSubPackageInfo = kPacket_.m_vecSubPackageInfo;
	kPacket.m_vecSubPackageTrans = kPacket_.m_vecSubPackageTrans;
#endif //SERV_GLOBAL_CASH_PACKAGE
	

	//	����
	std::vector< KBillOrderInfo >::iterator vit;
	for( vit = kPacket.m_vecOrderInfo.begin(); vit != kPacket.m_vecOrderInfo.end(); vit++ )
	{
		std::map< UidType, std::wstring >::iterator mit = kPacket_.m_mapNickName.find( vit->m_iFromUnitUID );

		if( mit != kPacket_.m_mapNickName.end() )
		{
			vit->m_wstrSenderNickName = mit->second;
		}
	}

	SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket );
}
//}} ����� : [2010/8/19/] //	���� ��� ����


IMPL_ON_FUNC( EGS_EXCHANGE_CASH_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EXCHANGE_CASH_ACK );

	if(kPacket_.m_uiExchangeValue > 0)
	{
#ifdef SERV_COUNTRY_CN
		KELG_EXCHANGE_CASH_REQ kPacket;
		kPacket.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacket.m_PurchaserInfo.m_iUserUID = GetUID();
		kPacket.m_PurchaserInfo.m_wstrUserName = GetName();
		kPacket.m_PurchaserInfo.m_iUnitUID = GetCharUID();
		kPacket.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
		kPacket.m_PurchaserInfo.m_ucLevel = GetLevel();
		kPacket.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
		kPacket.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_iPoint = static_cast<int>(kPacket_.m_uiExchangeValue);

		SendToLoginServer( ELG_EXCHANGE_CASH_REQ, kPacket );
#else // SERV_COUNTRY_CN
		KEGS_EXCHANGE_CASH_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		kPacket.m_iPoint = 0;

		SendPacket( EGS_EXCHANGE_CASH_ACK, kPacket );
#endif // SERV_COUNTRY_CN
	}
	else
	{
		KEGS_EXCHANGE_CASH_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_EXCHANGE_CASH;
		kPacket.m_iPoint = 0;

		SendPacket( EGS_EXCHANGE_CASH_ACK, kPacket );
	}
}

IMPL_ON_FUNC( EBILL_EXCHANGE_CASH_ACK )
{
	KEGS_EXCHANGE_CASH_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iPoint = kPacket_.m_iChargeCash;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_AddBonusCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT] = kPacket_.m_iChargeBonus;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendPacket( EGS_EXCHANGE_CASH_ACK, kPacket );
}


IMPL_ON_FUNC( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK )
{
	// ���� ������?

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// �г��� �˻� ���
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// ���ο��� �����Ϸ��°��� üũ�ϱ�
	if( kPacket_.m_wstrUserID == GetName() )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_20;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// �޴»���� ü��ID���� �˻�
	int iFindSize = static_cast<int>(kPacket_.m_wstrUserID.find( '/' ));
	if( iFindSize >= 0 )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_23;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}


	KEBILL_GIFT_ITEM_REQ kPacketReq;
	kPacketReq.m_wstrTotalOrderID = MakeOrderID();	// TotalOrderID�� ���� �߱�

	std::vector< KNXBTPurchaseReqInfo >::const_iterator vitReqInfo;
	for( vitReqInfo = kPacket_.m_vecPurchaseReqInfo.begin(); vitReqInfo != kPacket_.m_vecPurchaseReqInfo.end(); vitReqInfo++ )
	{
		KBillProductInfo kBillProductInfo;
		if( !SiKBillManager()->GetProductInfo( ( int )vitReqInfo->m_ulProductNo, kBillProductInfo ) )
		{
			START_LOG( cerr, L"ProductInfo error." )
				<< BUILD_LOG( vitReqInfo->m_ulProductNo )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}

		KBillBuyInfo kBuyInfo;
		kBuyInfo.m_wstrOrderID = MakeOrderID();
		kBuyInfo.m_iProductNo = ( int )vitReqInfo->m_ulProductNo;
		kBuyInfo.m_iPoint = kBillProductInfo.m_iSalePrice;
		kBuyInfo.m_iProductID = kBillProductInfo.m_iProductID;
		kBuyInfo.m_iCategory = ( int )kBillProductInfo.m_cCategoryNo;
		kBuyInfo.m_wstrMemo = kBillProductInfo.m_wstrProductName;
		kBuyInfo.m_usOrderQuantity = kBillProductInfo.m_cQuantity;	// ȿ�� �߰�
		kPacketReq.m_vecBillBuyInfo.push_back( kBuyInfo );
	}	
	
	kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketReq.m_iChannelingCode = GetChannelCode();

	kPacketReq.m_wstrSenderUserID = GetName();
	kPacketReq.m_uiSenderPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
	
	kPacketReq.m_wstrSenderNickName = GetCharName();

	kPacketReq.m_wstrReceiverUserID = kPacket_.m_wstrUserID;
	kPacketReq.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	
	kPacketReq.m_iSenderUserUID = GetUID();
	kPacketReq.m_iSenderUnitUID = GetCharUID();

	kPacketReq.m_iReceiverUserUID = kPacket_.m_iReceiverUserUID;
	kPacketReq.m_iReceiverUnitUID = kPacket_.m_iReceiverUnitUID;
	
	kPacketReq.m_wstrPresentMessage = kPacket_.m_wstrMessage;

	kPacketReq.m_BuyCharInfo.m_cLevel = GetLevel();
	kPacketReq.m_BuyCharInfo.m_iUnitClass = GetUnitClass();
	kPacketReq.m_RecvCharInfo.m_cLevel = kPacket_.m_cReceiverLevel;
	kPacketReq.m_RecvCharInfo.m_iUnitClass = kPacket_.m_iReceiverUnitClass;


	m_kUserCashInventory.SetReceiverUserID( kPacket_.m_wstrUserID ); // ������ UserID �ӽ� ����
	m_kUserCashInventory.SetMessage( kPacket_.m_wstrMessage );		 // �����ϱ� �޽��� �ӽ� ����

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_iUseCashType))
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUseCashType = kPacket_.m_iUseCashType;

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	kPacketReq.m_wstrUserIP = KncUtil::toWideString( GetIPStr() );

	// ���� ���� ���� ���� ���� üũ�� (���� ���� ���)
	SendToKOGBillingDB( EBILL_PREPARE_GIFT_ITEM_REQ, kPacketReq );
}

IMPL_ON_FUNC( EBILL_PREPARE_GIFT_ITEM_ACK )
{
	if(kPacket_.m_iOK != NetError::NET_OK)
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
	// �߱��� �系���� �� ó����
	SendToKOGBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
#else // SERV_COUNTRY_CN
	DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
	DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
	DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();

	if(KSimLayer::BF_INTERNAL == dwBillingFlag)
	{
		SendToKOGBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
	{
		// �����ؼ� DB�� ������ TCP�� ������ �����ؾ� ��

		// �Ϲ� ĳ�ô� TCP �� ����
		if (kPacket_.m_kEBILL_GIFT_ITEM_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_CASH)
			SendToPublisherBilling( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
		// ���ҵ� ��ȯ ĳ�ô� DB�� ����
		else if (kPacket_.m_kEBILL_GIFT_ITEM_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH)
			SendToPublisherBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
		else
		{
			START_LOG( cerr, L"���� UseCashType�� ������ �ֽ��ϴ�. ���� �ƴ�" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_kEBILL_GIFT_ITEM_REQ.m_iUseCashType )
				<< END_LOG;
		}
	}
	else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
	{
		SendToPublisherBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
	}
	else
	{
		SendToPublisherBilling( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
	}
#endif // SERV_COUNTRY_CN
}

#ifdef SERV_EVENT_BUY_FAKE_ITEM
IMPL_ON_FUNC( EBILL_CHECK_BUY_FAKE_ITEM_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_iRewardID	   = 834;	// �� ���߾�� ����! ��������

		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );

		//START_LOG( clog, L"�輮��_������ �������� ����������?" )
		//	<< BUILD_LOG( kPacketToDB.m_iFromUnitUID )
		//	<< BUILD_LOG( kPacketToDB.m_iToUnitUID )
		//	<< BUILD_LOG( kPacketToDB.m_iRewardType )
		//	<< BUILD_LOG( kPacketToDB.m_sQuantity )
		//	<< BUILD_LOG( kPacketToDB.m_iRewardID )
		//	<< END_LOG;
	}
	else
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
	}

}
#endif //SERV_EVENT_BUY_FAKE_ITEM


#ifdef SERV_COUNTRY_PH
IMPL_ON_FUNC( EJSON_GN_CHANGE_GAME_CURRENCY_REQ )
{
	KEBILL_GN_CHANGE_GAME_CURRENCY_REQ kPacket;	

	if (m_kNexonAccountInfo.m_uiNexonSN == kPacket_.m_uiGarenaUID)
	{		
		kPacket.m_kGarenaREQ = kPacket_;
		kPacket.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacket.m_PurchaserInfo.m_iUserUID = GetUID();
		kPacket.m_PurchaserInfo.m_wstrUserName = GetName();
		kPacket.m_PurchaserInfo.m_iUnitUID = GetCharUID();
		kPacket.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
		kPacket.m_PurchaserInfo.m_ucLevel = GetLevel();
		kPacket.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
		kPacket.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_iResult = 0;
		//SendToKOGBillingDB( EBILL_GN_CHANGE_GAME_CURRENCY_REQ, kPacket);		

		START_LOG( clog, L"Garena ���� ĳ�� ��ȯ�� ���� ���� ��ȸ ��û" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kNexonAccountInfo.m_uiNexonSN )
			<< BUILD_LOG( kPacket_.m_uiGameCurrency_Amount )
			<< BUILD_LOG( kPacket_.m_uiShell_Amount )
			<< BUILD_LOG( kPacket_.m_strGarenaTransactionID )
			<< END_LOG;
	}
	else
	{
		kPacket.m_iResult = -1;
		kPacket.m_kGarenaREQ.m_iSessionUID = kPacket_.m_iSessionUID;
		START_LOG( cerr, L"Garena���� ĳ�� ��ȯ ��û ��� �Դµ� �츮���� ���� ������!!" )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID)
			<< END_LOG;
	}

	SendToLoginServer( ELG_GN_CHANGE_GAME_CURRENCY_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_EXCHANGE_CASH_NOT )
{
	KEGS_EXCHANGE_CASH_CLIENT_NOT kPacket;

	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iPoint = kPacket_.m_iPoint;
	kPacket.m_AddBonusCashInfo = kPacket_.m_AddBonusCashInfo;

	SendPacket( EGS_EXCHANGE_CASH_CLIENT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EBILL_GARENA_PREPARE_PRESENT_CHECK_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// �г��� �˻� ���
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName ) )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}
	
	// ���ο��� �����Ϸ��°��� üũ�ϱ�
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName, GetCharName()) )
#else 
	if( kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName == GetCharName() )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_20;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// �������� ��ǰ���� �˻� - ���Ŀ� �ʿ伺 üũ
	std::vector< KNXBTPurchaseReqInfo >::iterator vit;
	for( vit = kPacket_.m_kEGSPresentCashItemREQ.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_kEGSPresentCashItemREQ.m_vecPurchaseReqInfo.end(); vit++ )
	{
#ifdef SERV_GLOBAL_BILLING
		int iItemID = SiKBillManager()->GetItemID( vit->m_ulProductNo );;
#else // SERV_GLOBAL_BILLING
		int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
#endif // SERV_GLOBAL_BILLING
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø��� ���� ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}

#ifdef SERV_USE_ENABLE_GIFT
		KBillProductInfo kBillProductInfo2;
		if(SiKBillManager()->GetProductInfo((int)vit->m_ulProductNo, kBillProductInfo2))
		{
			if(kBillProductInfo2.m_bEnableGift == false)
			{
				START_LOG( cerr, L"������ �� ���� ������" )
					<< BUILD_LOG( vit->m_ulProductNo )
					<< BUILD_LOG( iItemID )
					<< END_LOG;

				KEGS_PRESENT_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_11;
				SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
#else SERV_USE_ENABLE_GIFT
		// ����ǰ Ȯ��� ���� Ȯ���� ���� �Ұ���
		switch( pItemTemplet->m_ItemID )
		{
		case CXSLItem::CI_EXPAND_INVENTORY:
		case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
		case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
		case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
			//{{ 2012. 12. 19	������	�ƶ� ���� ĳ����
#ifdef SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
#endif SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
			//}}
			{
				KEGS_PRESENT_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_27;
				SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
				return;
			}
			break;
		}
#endif SERV_USE_ENABLE_GIFT
	}

	// �򿡼� ĳ������
	m_kUserWishList.SetBuyCashItemInWishList( kPacket_.m_kEGSPresentCashItemREQ.m_bIsWishList );

	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
	kPacketToDB.m_wstrReceiverNickName = kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName;
	kPacketToDB.m_wstrMessage		   = kPacket_.m_kEGSPresentCashItemREQ.m_wstrMessage;
	kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_kEGSPresentCashItemREQ.m_vecPurchaseReqInfo;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_kEGSPresentCashItemREQ.m_iUseCashType))
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iUseCashType = kPacket_.m_kEGSPresentCashItemREQ.m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
}
#endif //SERV_COUNTRY_PH


#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
IMPL_ON_FUNC( EGS_CASH_DIRECT_CHARGE_CN_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CASH_DIRECT_CHARGE_CN_ACK );

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH_TEST_MODE
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"[CN] Direct Charge �ּ� ��û�� ���ڵ� �ƴѵ� �ߴ�!!." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		KEGS_CASH_DIRECT_CHARGE_CN_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket(EGS_CASH_DIRECT_CHARGE_CN_ACK, kPacketAck);

		return;
	}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

	if( kPacket_.m_iUserUID == GetUID())
	{
		START_LOG( cout, L"[TEST] Direct Charge �ּ� ��û." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		// �α��� ������ ���� (������ ������ �α��μ����� ä���� ������ ����) //

		KELG_CASH_DIRECT_CHARGE_CN_REQ kPacketReq;
		kPacketReq.m_iUserUID = GetUID();
		kPacketReq.m_wstrIP = KncUtil::toWideString( GetIPStr() );;
		kPacketReq.m_wstrServiceAccountID = GetName();
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_wstrUnitNickName = GetCharName();

		SendToLoginServer( ELG_CASH_DIRECT_CHARGE_CN_REQ, kPacketReq );	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	}
	else
	{
		START_LOG( cerr, L"[CN] Direct Charge �ּ� ��û ���� ������ ���� ������ Ʋ����!?." )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		// ���� ó��
		KEGS_CASH_DIRECT_CHARGE_CN_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket(EGS_CASH_DIRECT_CHARGE_CN_ACK, kPacketAck);
	}
}

_IMPL_ON_FUNC( ELG_CASH_DIRECT_CHARGE_CN_ACK, KEGS_CASH_DIRECT_CHARGE_CN_ACK)
{
	START_LOG( cout, L"[TEST] Direct Charge �ּ� ��û ���� �Դ�!" )
		<< BUILD_LOG( kPacket_.m_iOK )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() )
		<< END_LOG;

	SendPacket(EGS_CASH_DIRECT_CHARGE_CN_ACK, kPacket_);
}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#endif SERV_GLOBAL_BILLING

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


