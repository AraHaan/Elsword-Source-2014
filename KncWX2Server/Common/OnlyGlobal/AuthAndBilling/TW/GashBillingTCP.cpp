
#include "ActorManager.h"
#include "GashBillingTCP.h"

#include "KncSend.h"
#include "GashBillingTCPManager.h"
#include "NetError.h"
//#include "BaseServer.h"
//#include "GameServer.h"

#ifdef SERV_COUNTRY_TWHK

//{{ 2011.7.12 ���� : Gash ����Ʈ ����
#ifdef SERV_BILLING_ADD_GASH_POINT


#define CLASS_TYPE  KGASHBillingTCP

KGASHBillingTCP::KGASHBillingTCP( void )
{
}

KGASHBillingTCP::~KGASHBillingTCP( void )
{
	End( 15000 );
}

void KGASHBillingTCP::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_BILLING_BALANCE_REQ );
		CASE( EGASH_GET_GASHPOINT_ACK );

		CASE( EBILL_BUY_PRODUCT_REQ );
		CASE( EBILL_GIFT_ITEM_REQ );
		CASE( EGASH_DECREASE_GASHPOINT_ACK );

		CASE( EBILL_USE_COUPON_REQ );			// ���� ������ (�ۺ� üũ���� �ٷ� ���)

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


void KGASHBillingTCP::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KGASHBillingTCP::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID )
{
	SendToKOGBillingDB( nUserUID, usEventID, char() );
}

void KGASHBillingTCP::SendToGASHBillingDB( UidType nUserUID, unsigned short usEventID )
{
	SendToGASHBillingDB( nUserUID, usEventID, char() );
}


IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
	START_LOG( clog, L"Get gashpoint req" );

	KEGASH_GET_GASHPOINT_REQ kPacketToGashTCP;
	kPacketToGashTCP.m_iActorUID = kPacket_.m_uiActorUID;
	kPacketToGashTCP.m_strServiceCode = SiKGASHBillingTCPManager()->GetServiceCode();
	kPacketToGashTCP.m_strServiceRegion = SiKGASHBillingTCPManager()->GetServiceRegion();
	kPacketToGashTCP.m_strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrAccount );
	kPacketToGashTCP.m_strServicePassword = "0";
	kPacketToGashTCP.m_strAuthenticationFlag = "0";
	kPacketToGashTCP.m_strType = "P";
	kPacketToGashTCP.m_strDuplicateCheck = "0";
	kPacketToGashTCP.m_strIPAddress = KncUtil::toNarrowString(kPacket_.m_wstrIP);
	char szUID[30];
	_i64toa(kPacketToGashTCP.m_iActorUID, szUID, 10);
	kPacketToGashTCP.m_strPacketID = szUID;

	boost::shared_ptr< std::string > spPacket( new std::string );
	kPacketToGashTCP.GetGashPacket( *spPacket );

	SiKGASHBillingTCPManager()->QueueingSendPacket( KGASHBillingTCPManager::GP_AUTHORIZATION, spPacket );
}

IMPL_ON_FUNC( EGASH_GET_GASHPOINT_ACK )
{
	START_LOG( clog, L"Get gash point result" )
		<< BUILD_LOG( kPacket_.m_strResultCode )
		<< BUILD_LOG( kPacket_.m_strPacketID )
		<< END_LOG;

	UidType iActorUID = _atoi64(kPacket_.m_strPacketID.c_str());
	if( iActorUID <= 0 )
	{
		START_LOG(cerr, L"GASH ���� : �ܾ� ��ȸ ���� ��Ŷ�� UID�� �߸� ��ܿԴ�")
			<< BUILD_LOG( kPacket_.m_strResultCode )
			<< BUILD_LOG( kPacket_.m_strPacketID )
			<< END_LOG;
	}

	START_LOG( clog, L"Rev Packet Actor UID" )
		<< BUILD_LOG( iActorUID )
		<< END_LOG;

	KEPUBLISHER_BILLING_BALANCE_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
	kPacketAck.m_ulBalance = 0;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// Gash ĳ���� �ۺ��� ĳ���� ���� 
	kPacketAck.m_iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	int iResult = ::atoi( kPacket_.m_strResultCode.c_str() );
	switch( iResult )
	{
	case 1:     // success
		{
			// �ܿ� GASH ����Ʈ�� �޾����� �̰� Ŭ��� �Ѱ�����.
			kPacketAck.m_ulBalance = ::atoi(kPacket_.m_strOutString.c_str());
			kPacketAck.m_iOK = NetError::NET_OK;

			// �α� ���ܾ� �ϱ� ������ �ܾ� ������ ������� ����.
			SiKGASHBillingTCPManager()->InsertHaveGashPoint(iActorUID, kPacketAck.m_ulBalance);			
		}
		break;
	case 0:     // fail
		{
			// �ƿ���Ʈ���� ���� ������ ���� �� ���µ�, ���� �κп����� �̷� �κ��� ���� ó���� ���س���.
			// ���п� ���� neterr ��Ʈ���� ���µ� �߰��� �� ��� �ڴ�.
			if( kPacket_.m_strOutString.compare( "No_Such_ServiceAccount" ) == 0 )
			{
				START_LOG( cerr, L"GASH ���� : �ܾ� ��ȸ ���� ���� ã��" )
					<< BUILD_LOG( kPacket_.m_strAccountID.c_str() )
					<< BUILD_LOG( kPacket_.m_strPacketID.c_str() )
					<< BUILD_LOG( "ServiceAccount_Not_Login" )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_GASH_02;
			}
			else if( kPacket_.m_strOutString.compare( "Invalid_Length" ) == 0 )
			{
				START_LOG( cerr, L"GASH ���� : �ܾ� ��ȸ ���� ���� ã��" )
					<< BUILD_LOG( kPacket_.m_strAccountID.c_str() )
					<< BUILD_LOG( kPacket_.m_strPacketID.c_str() )
					<< BUILD_LOG( "Invalid_Length" )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_GASH_03;
			}
			else if( kPacket_.m_strOutString.compare( "Invalid_Character" ) == 0 )
			{
				START_LOG( cerr, L"GASH ���� : �ܾ� ��ȸ ���� ���� ã��" )
					<< BUILD_LOG( kPacket_.m_strAccountID.c_str() )
					<< BUILD_LOG( kPacket_.m_strPacketID.c_str() )
					<< BUILD_LOG( "Invalid_Character" )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_GASH_03;
			}
			else if( kPacket_.m_strOutString.compare( "Service_Not_Available" ) == 0 )
			{
				START_LOG( cerr, L"GASH ���� : �ܾ� ��ȸ ���� ���� ã��" )
					<< BUILD_LOG( kPacket_.m_strAccountID.c_str() )
					<< BUILD_LOG( kPacket_.m_strPacketID.c_str() )
					<< BUILD_LOG( "Service_Not_Available" )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_GASH_01;
			}
			else if( kPacket_.m_strOutString.compare( "No_Such_Service" ) == 0 )
			{
				START_LOG( cerr, L"GASH ���� : �ܾ� ��ȸ ���� ���� ã��" )
					<< BUILD_LOG( kPacket_.m_strAccountID.c_str() )
					<< BUILD_LOG( kPacket_.m_strPacketID.c_str() )
					<< BUILD_LOG( "No_Such_Service" )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_GASH_18;
			}
			else if( kPacket_.m_strOutString.compare( "Unexpected_Error" ) == 0 )
			{
				START_LOG( cerr, L"GASH ���� : �ܾ� ��ȸ ���� ���� ã��" )
					<< BUILD_LOG( kPacket_.m_strAccountID.c_str() )
					<< BUILD_LOG( kPacket_.m_strPacketID.c_str() )
					<< BUILD_LOG( "Unexpected_Error" )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_GASH_19;
			}
			else
			{
				START_LOG( cerr, L"GASH ���� : ����Ʈ ��� ���� - ���ǵ��� ���� ���� �߻� -" )
					<< BUILD_LOG( kPacket_.m_strResultCode )
					<< BUILD_LOG( iResult )
					<< BUILD_LOG( kPacket_.m_strOutString )
					<< BUILD_LOG( kPacket_.m_strPacketID )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
			}
		}
		break;
	case -1:    // system error
		{
			kPacketAck.m_iOK = NetError::ERR_GASH_00;

			START_LOG( cerr, L"GASH ���� : ����Ʈ ��� ���� - �ý��� ���� �߻�" )
				<< BUILD_LOG( kPacket_.m_strResultCode )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( kPacket_.m_strOutString )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< END_LOG;
		}
		break;
	default:
		{
			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

			START_LOG( cerr, L"GASH ���� : ����Ʈ ��� ���� - ���ǵ��� ���� ��� ��" )
				<< BUILD_LOG( kPacket_.m_strResultCode )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( kPacket_.m_strOutString )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< END_LOG;
		}
		break;
	}

	SendToUser(iActorUID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{
	START_LOG( clog, L"EBILL_BUY_PRODUCT_REQ Decrease gash  req" );


	if(!SiKGASHBillingTCPManager()->InsertBuyPacket( kPacket_.m_iUserUID, kPacket_))
	{
		// ���� : �� ���� - �̹� �������� ���� ��Ŷ�� �ִ�? ���� �߰������ �Ѵ�.
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_23;
		SendToUser( LAST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacketAck );
		return;
	}


	// ���� �ݾ��� ����Ѵ�.
	int iTotalPrice = 0;
	std::string strProNo = "";					// ���� �ƴѰ�� �ʹ� ����
	std::vector< KBillBuyInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
	{
		iTotalPrice += vit->m_iPoint;
		// ����ϸ鼭, Product no �� �����Ѵ�.
		char szProNum[10];
		::itoa( vit->m_iProductNo, szProNum, 10 );
		strProNo += szProNum;
		//strProNo += "/";
	}

	// �ܾ��� Ȯ���Ѵ�. -> �����ϸ� �˾Ƽ� ���� �´�.

	KEGASH_DECREASE_GASHPOINT_REQ kPacketToGashTCP;
	kPacketToGashTCP.m_iActorUID = FIRST_SENDER_UID;
	//kPacketReq.m_iActorUID = GetUID();
	kPacketToGashTCP.m_strServiceCode = SiKGASHBillingTCPManager()->GetServiceCode();
	kPacketToGashTCP.m_strServiceRegion = SiKGASHBillingTCPManager()->GetServiceRegion();
	kPacketToGashTCP.m_strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrUserID );
	kPacketToGashTCP.m_strType = "P";
	CTime kShopTime = CTime::GetCurrentTime();
	kPacketToGashTCP.m_strShopTime = KncUtil::toNarrowString((const wchar_t*)kShopTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	char szPrice[10];
	::itoa( iTotalPrice, szPrice, 10 );
	kPacketToGashTCP.m_strPoints = szPrice;
	kPacketToGashTCP.m_strIPAddress = kPacket_.m_strUserIP;

	//kPacketToGashTCP.m_strMemo = strProNo;	// ������ produnt no
	kPacketToGashTCP.m_strMemo = KncUtil::toNarrowString(kPacket_.m_wstrTotalOrderID);// memo �� ���� ��ȣ�� ����.
	START_LOG( clog, L"���� IP" )
		<< BUILD_LOG( kPacketToGashTCP.m_strIPAddress );
	char szUID[30];
	//::itoa( static_cast<long>(kPacketReq.m_iActorUID), szUID, 10 );
	_i64toa(kPacketToGashTCP.m_iActorUID, szUID, 10);
	kPacketToGashTCP.m_strPacketID = szUID;


	boost::shared_ptr< std::string > spPacket( new std::string );
	kPacketToGashTCP.GetGashPacket( *spPacket );

	SiKGASHBillingTCPManager()->QueueingSendPacket( KGASHBillingTCPManager::GP_ACCOUNTING, spPacket );
}

IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
	START_LOG( clog, L"EBILL_GIFT_ITEM_REQ Decrease gash  req" );


	if(!SiKGASHBillingTCPManager()->InsertGiftPacket( kPacket_.m_iSenderUserUID, kPacket_))
	{
		// ���� : �� ���� - �̹� �������� ���� ��Ŷ�� �ִ�? ���� �߰������ �Ѵ�.
		KEBILL_GIFT_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_23;
		SendToUser( LAST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacketAck );
		return;
	}


	// ���� �ݾ��� ����Ѵ�.
	int iTotalPrice = 0;
	std::string strProNo = "Gift/";				// ���� ���� ǥ�� 
	std::vector< KBillBuyInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
	{
		iTotalPrice += vit->m_iPoint;
		// ����ϸ鼭, Product no �� �����Ѵ�.
		char szProNum[10];
		::itoa( vit->m_iProductNo, szProNum, 10 );
		strProNo += szProNum;
		//strProNo += "/";
	}

	// �ܾ��� Ȯ���Ѵ�. -> �����ϸ� �˾Ƽ� ���� �´�.
	KEGASH_DECREASE_GASHPOINT_REQ kPacketToGashTCP;
	kPacketToGashTCP.m_iActorUID = kPacket_.m_iSenderUserUID;
	kPacketToGashTCP.m_strServiceCode = SiKGASHBillingTCPManager()->GetServiceCode();
	kPacketToGashTCP.m_strServiceRegion = SiKGASHBillingTCPManager()->GetServiceRegion();
	kPacketToGashTCP.m_strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrSenderUserID );
	kPacketToGashTCP.m_strType = "P";
	CTime kShopTime = CTime::GetCurrentTime();
	kPacketToGashTCP.m_strShopTime = KncUtil::toNarrowString((const wchar_t*)kShopTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	char szPrice[10];
	::itoa( iTotalPrice, szPrice, 10 );
	kPacketToGashTCP.m_strPoints = szPrice;
	kPacketToGashTCP.m_strIPAddress = KncUtil::toNarrowString(kPacket_.m_wstrUserIP);
	//kPacketToCashTCP.m_strMemo = strProNo;
	kPacketToGashTCP.m_strMemo = KncUtil::toNarrowString(kPacket_.m_wstrTotalOrderID);// memo �� ���� ��ȣ�� ����.
	START_LOG( clog, L"���� IP" )
		<< BUILD_LOG( kPacketToGashTCP.m_strIPAddress );
	char szUID[30];
	//::itoa( static_cast<long>(kPacketReq.m_iActorUID), szUID, 10 );
	_i64toa(kPacketToGashTCP.m_iActorUID, szUID, 10);
	kPacketToGashTCP.m_strPacketID = szUID;


	boost::shared_ptr< std::string > spPacket( new std::string );
	kPacketToGashTCP.GetGashPacket( *spPacket );

	SiKGASHBillingTCPManager()->QueueingSendPacket( KGASHBillingTCPManager::GP_ACCOUNTING, spPacket );
}

IMPL_ON_FUNC( EGASH_DECREASE_GASHPOINT_ACK )
{
	START_LOG( clog, L"Decrease gash result" )
		<< BUILD_LOG( kPacket_.m_strResultCode )
		<< BUILD_LOG( kPacket_.m_strPacketID )
		<< END_LOG;

	UidType iActorUID = _atoi64(kPacket_.m_strPacketID.c_str());

	// �׷��� UID�� ���������� ������ �ַ� �α� �����.
	if( 0 >= iActorUID )
	{
		START_LOG( clog, L"GASH ���� : UID �� �߸��Ǿ���. �̷� ���� ���� ���� ���µ�..." )
			<< BUILD_LOG( kPacket_.m_strAccountID )
			<< BUILD_LOG( kPacket_.m_strPacketID )
			<< BUILD_LOG( kPacket_.m_strOutString )
			<< END_LOG;

		// ���� ���� ��Ŷ�� �Դٸ�, �ش� ������ ���� �����Ƿ� ���� �α� ��� �� �д�.
		if( atoi( kPacket_.m_strResultCode.c_str() ) == 1 )
		{
			// �α� ��Ŷ ������.
			KDBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT kPacketToLog;
			kPacketToLog.m_iResultCode = 1;
			kPacketToLog.m_iUserUID = iActorUID;
			kPacketToLog.m_strOutString = kPacket_.m_strOutString;

			SendToKOGBillingDB( iActorUID, DBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT, kPacketToLog );
		}

		return;
	}

	
	KEBILL_BUY_PRODUCT_REQ kPacketReqBuy;
	kPacketReqBuy.m_wstrTotalOrderID = L"";
	kPacketReqBuy.m_iServerGroupID = 0;
	kPacketReqBuy.m_wstrUserID = L"";
	kPacketReqBuy.m_wstrNickName = L"";
	kPacketReqBuy.m_vecBillBuyInfo.clear();

	KEBILL_GIFT_ITEM_REQ kPacketReqGift;
	kPacketReqGift.m_wstrTotalOrderID = L"";
	kPacketReqGift.m_iServerGroupID = 0;
	kPacketReqGift.m_wstrSenderUserID = L"";
	kPacketReqGift.m_wstrSenderNickName = L"";
	kPacketReqGift.m_vecBillBuyInfo.clear();
	
	// �븸��񿡵� Gash ���� �ߴٰ� �α� ��������.
	KEGASH_USE_GASH_LOG_NOT kPacketLog;
	kPacketLog.m_iFlag = 0;	// �츮�� ������ 0

	int iResult = ::atoi( kPacket_.m_strResultCode.c_str() );

	// �׽�Ʈ
	//iResult = 0;

	bool bIsGift = false;	// ������ ����Ȱų�??

	// ���ŷ� ���� �����, ������ ��������� Ȯ������.
	if(SiKGASHBillingTCPManager()->IsHaveBuyPacket(iActorUID))
	{
		// ���� //

		// ���� �� Ack�� ��Ŷ //
		KEBILL_BUY_PRODUCT_ACK kPacketBuyAck;
		kPacketBuyAck.m_iOK = NetError::ERR_UNKNOWN;

		
		if(!SiKGASHBillingTCPManager()->GetBuyPacket(iActorUID, kPacketReqBuy))		
		{
			START_LOG( cerr, L"GASH ���� : ���� �����ߴµ�, ������Ŷ�� ����. �̷��� �������� ū��!!" )
				<< BUILD_LOG( kPacket_.m_strResultCode )
				<< BUILD_LOG( kPacket_.m_strOutString)
				<< BUILD_LOG( kPacket_.m_strAccountID )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< BUILD_LOG( iActorUID )
				<< END_LOG;
			
			// ���� ���� ��Ŷ�� �Դٸ�, �ش� ������ ���� �����Ƿ� ���� �α� ��� �� �д�.
			if( iResult == 1 )
			{
				// �α� ��Ŷ ������.
				KDBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT kPacketToLog;
				kPacketToLog.m_iResultCode = iResult;
				kPacketToLog.m_iUserUID = iActorUID;
				kPacketToLog.m_strOutString = kPacket_.m_strOutString;

				SendToKOGBillingDB( iActorUID, DBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT, kPacketToLog );
			}

			// �����؊x�� ��Ŷ ������ ���� �븸�� �α� ������ �ʴ´�.
			return;
		}
		else
		{
			switch( iResult )
			{
			case 1:     // success
				{
					// ���� ���� ������, ���� ��Ŷ�� ���� �ͼ�, �κ��� ���� �ִ� �۾��� �ϵ��� ��Ŷ�� ������
					START_LOG( clog, L"GASH ���� : ����, ��Ŷ �������� ��� ���� ��, ���� ��Ŷ�� ������." )
						<< BUILD_LOG( kPacket_.m_strResultCode )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< BUILD_LOG( iActorUID )
						<< END_LOG;
					//kPacketReq.m_bIsGamePoint = false;// �α׿�
					kPacketLog.m_iFlag = 1;
					kPacketBuyAck.m_iOK = NetError::NET_OK;
					SendToKOGBillingDB( iActorUID, EBILL_BUY_PRODUCT_REQ, kPacketReqBuy );
					
				}
				break;
			case 0:     // fail
				{
					if( kPacket_.m_strOutString.compare( "Contract_Block" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "Contract_Block" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_17;
						kPacketLog.m_iFlag = 9;
					}
					else if( kPacket_.m_strOutString.compare( "No_Enough_Point" ) == 0 )
					{
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_09;
						kPacketLog.m_iFlag = 2;// 0 ���Ͽ��� ������ �����������??
					}
					else if( kPacket_.m_strOutString.compare( "Invalid_Time_Format" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "Invalid_Time_Format" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_20;
						kPacketLog.m_iFlag = 9;
					}
					else if( kPacket_.m_strOutString.compare( "ServiceAccount_Locked" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "ServiceAccount_Locked" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_01;
						kPacketLog.m_iFlag = 9;
					}
					else if( kPacket_.m_strOutString.compare( "Service_Not_Available" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "Service_Not_Available" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_01;
						kPacketLog.m_iFlag = 9;
					}
					else if( kPacket_.m_strOutString.compare( "No_Such_Service" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "No_Such_Service" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_18;
						kPacketLog.m_iFlag = 9;
					}
					else if( kPacket_.m_strOutString.compare( "Invalid_Accounting_Type" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "Invalid_Accounting_Type" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_21;
						kPacketLog.m_iFlag = 9;
					}
					else if( kPacket_.m_strOutString.compare( "Unexpected_Error" ) == 0 )
					{
						START_LOG( cerr, L"GASH ���� : �������� ���� ã��" )
							<< BUILD_LOG( "Unexpected_Error" )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_GASH_19;
						kPacketLog.m_iFlag = 9;
					}
					else
					{
						START_LOG( cerr, L"GASH ���� : �������� - ���ǵ��� ���� ���� �߻� -" )
							<< BUILD_LOG( kPacket_.m_strResultCode )
							<< BUILD_LOG( iResult )
							<< BUILD_LOG( kPacket_.m_strOutString )
							<< BUILD_LOG( kPacket_.m_strPacketID )
							<< END_LOG;
						kPacketBuyAck.m_iOK = NetError::ERR_UNKNOWN;
						kPacketLog.m_iFlag = 9;
					}
				}
				break;
			case -1:    // system error
				{
					START_LOG( cerr, L"GASH ���� : ��������!! �̰� �ý��� ����??" )
						<< BUILD_LOG( iResult )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( iActorUID )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< END_LOG;
					kPacketBuyAck.m_iOK = NetError::ERR_GASH_10;

					kPacketLog.m_iFlag = 9;
			
				}
				break;
			default:
				{
					START_LOG( cerr, L"GASH ���� : gash accounting result error" )
						<< BUILD_LOG( kPacket_.m_strResultCode )
						<< BUILD_LOG( iResult )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< END_LOG;
				
					kPacketBuyAck.m_iOK = NetError::ERR_UNKNOWN;

					kPacketLog.m_iFlag = 9;
				}
				break;
			}	
		}

		// GSUser�� KEBILL_BUY_PRODUCT_ACK ������ Ŭ���̾�Ʈ�� ���� ���� EGS_BUY_CASH_ITEM_ACK �޴´�.
		if(kPacketBuyAck.m_iOK != NetError::NET_OK)
		{
			SendToUser( iActorUID, EBILL_BUY_PRODUCT_ACK, kPacketBuyAck );
		}
	}
	else if(SiKGASHBillingTCPManager()->IsHaveGiftPacket(iActorUID))
	{
		// ���� //
		bIsGift = true;

		// ���� �� Ack�� ��Ŷ //
		KEBILL_GIFT_ITEM_ACK kPacketGiftAck;
		kPacketGiftAck.m_iOK = NetError::ERR_UNKNOWN;

		
		if(!SiKGASHBillingTCPManager()->GetGiftPacket(iActorUID, kPacketReqGift))
		{
			START_LOG( cerr, L"GASH ���� : ���� �����ߴµ�, ������Ŷ�� ����. �̷��� �������� ū��!!" )
				<< BUILD_LOG( kPacket_.m_strResultCode )
				<< BUILD_LOG( kPacket_.m_strOutString )
				<< BUILD_LOG( kPacket_.m_strAccountID )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< BUILD_LOG( iActorUID )
				<< END_LOG;

			// ���� ���� ��Ŷ�� �Դٸ�, �ش� ������ ���� �����Ƿ� ���� �α� ��� �� �д�.
			if( iResult == 1 )
			{
				// �α� ��Ŷ ������.
				KDBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT kPacketToLog;
				kPacketToLog.m_iResultCode = iResult;
				kPacketToLog.m_iUserUID = iActorUID;
				kPacketToLog.m_strOutString = kPacket_.m_strOutString;

				SendToKOGBillingDB( iActorUID, DBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT, kPacketToLog );
			}
			
			// �����؊x�� ��Ŷ ������ ���� �븸�� �α� ������ �ʴ´�.
			return;
		}
		else
		{
			switch( iResult )
			{
			case 1:     // success
				{
					// ���� ���� ������, ���� ��Ŷ�� ���� �ͼ�, �κ��� ���� �ִ� �۾��� �ϵ��� ��Ŷ�� ������
					START_LOG( clog, L"GASH ���� : ����, ��Ŷ �������� ��� ���� ��, ���� ��Ŷ�� ������." )
						<< BUILD_LOG( kPacket_.m_strResultCode )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< BUILD_LOG( iActorUID )
						<< END_LOG;

					//Gash TCP ��� ĳ�� ������ ���� �ۺ��� ĳ�� �����̴�.
					kPacketReqGift.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;
					kPacketGiftAck.m_iOK = NetError::NET_OK;
					kPacketLog.m_iFlag = 1;
					SendToKOGBillingDB( iActorUID, EBILL_GIFT_ITEM_REQ, kPacketReqGift );
				
				}
				break;
			case 0:     // fail
				{
					START_LOG( cerr, L"GASH ���� : ���� -��������!!" )
						<< BUILD_LOG( iResult )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< BUILD_LOG( iActorUID )
						<< END_LOG;
					kPacketGiftAck.m_iOK = NetError::ERR_GASH_09;
					kPacketLog.m_iFlag = 2;
					
				}
				break;
			case -1:    // system error
				{
					START_LOG( cerr, L"GASH ���� : ���� -��������!! �̰� �ý��� ����??" )
						<< BUILD_LOG( iResult )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< BUILD_LOG( iActorUID )
						<< END_LOG;
					kPacketGiftAck.m_iOK = NetError::ERR_GASH_10;
					kPacketLog.m_iFlag = 9;
				
				}
				break;
			default:
				{
					START_LOG( cerr, L"GASH ���� : gash accounting result error" )
						<< BUILD_LOG( kPacket_.m_strResultCode )
						<< BUILD_LOG( iResult )
						<< BUILD_LOG( kPacket_.m_strOutString )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< BUILD_LOG( kPacket_.m_strAccountID )
						<< BUILD_LOG( kPacket_.m_strPacketID )
						<< END_LOG;

					kPacketGiftAck.m_iOK = NetError::ERR_UNKNOWN;
					kPacketLog.m_iFlag = 9;
				}
				break;
			}	
		}


		// GSUser�� KEBILL_GIFT_ITEM_ACK ������ Ŭ���̾�Ʈ�� ���� ���� KEGS_PRESENT_CASH_ITEM_ACK �޴´�.
		if(kPacketGiftAck.m_iOK != NetError::NET_OK)
		{
			SendToUser( iActorUID, EBILL_GIFT_ITEM_ACK, kPacketGiftAck );
		}
	}
	else
	{
		// ���ŵ� �ƴϰ�, ������ �ƴϴ�. �̷����� ������ �ȵ�
		START_LOG( cerr, L"GASH ���� ���� : ���� �����ߴµ�, ����, ������Ŷ�� ����. ��??" )
			<< BUILD_LOG( kPacket_.m_strResultCode )
			<< BUILD_LOG( kPacket_.m_strOutString )
			<< BUILD_LOG( kPacket_.m_strAccountID )
			<< BUILD_LOG( kPacket_.m_strPacketID )
			<< BUILD_LOG( iActorUID )
			<< END_LOG;


		// Ȥ�� �𸣴� �˸��� ������ Buy Product �� ����
		KEBILL_BUY_PRODUCT_ACK kPacketBuyAck;
		kPacketBuyAck.m_iOK = NetError::ERR_UNKNOWN;
		SendToUser( iActorUID, EGS_BUY_CASH_ITEM_ACK, kPacketBuyAck );

		// ���� ���� ��Ŷ�� �Դٸ�, �ش� ������ ���� �����Ƿ� ���� �α� ��� �� �д�.
		if( iResult == 1 )
		{
			// �α� ��Ŷ ������.
			KDBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT kPacketToLog;
			kPacketToLog.m_iResultCode = iResult;
			kPacketToLog.m_iUserUID = iActorUID;
			kPacketToLog.m_strOutString = kPacket_.m_strOutString;

			SendToKOGBillingDB( iActorUID, DBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT, kPacketToLog );
		}

		// �����؊x�� ��Ŷ ������ ���� �븸�� �α� ������ �ʴ´�.
		return;
	}

	

	int iHaveGash = 0;
	SiKGASHBillingTCPManager()->GetHaveGashPoint(iActorUID, iHaveGash);
	if(bIsGift)
	{
		kPacketLog.m_wstrTotalOrderID	= kPacketReqGift.m_wstrTotalOrderID;
		kPacketLog.m_iServerGroupID		= kPacketReqGift.m_iServerGroupID;
		kPacketLog.m_wstrUserID			= kPacketReqGift.m_wstrSenderUserID;
		kPacketLog.m_iUserUID			= iActorUID;
		kPacketLog.m_wstrNickName		= kPacketReqGift.m_wstrSenderNickName;
		kPacketLog.m_iBeforeCash		= 0;
		kPacketLog.m_iAfterGash			= 0;
		// ���� �ݾ��� ����Ѵ�.
		int iTotalPrice = 0;
		std::vector< KBillBuyInfo >::iterator vit;
		for( vit = kPacketReqGift.m_vecBillBuyInfo.begin(); vit != kPacketReqGift.m_vecBillBuyInfo.end(); vit++ )
		{
			iTotalPrice += vit->m_iPoint;
		}
		kPacketLog.m_iGashPoint			= iTotalPrice;
		kPacketLog.m_iBeforeCash		= iHaveGash;
		if(kPacketLog.m_iFlag == 1 || kPacketLog.m_iFlag == 3)
			kPacketLog.m_iAfterGash			= iHaveGash - iTotalPrice;
		else
			kPacketLog.m_iAfterGash		= 0;
		if(kPacketReqGift.m_vecBillBuyInfo.empty() == false)
		{
			kPacketLog.m_iItemID		= kPacketReqGift.m_vecBillBuyInfo.begin()->m_iProductID;
			kPacketLog.m_iProductID		= kPacketReqGift.m_vecBillBuyInfo.begin()->m_iProductNo;
		}
		//kPacketLog.m_iFlag			= 1;
		char szUID[30];
		//::itoa( kPacketReqGift.m_iReceiverUID, szUID, 10 );
		_i64toa( kPacketReqGift.m_iReceiverUserUID, szUID, 10 );		
		kPacketLog.m_wstrMemo		= L"Gift," + KncUtil::toWideString(szUID);
		//in_addr sIP;
		//sIP.s_addr = ( u_long )kPacketReqGift.m_iUserIP;
		kPacketLog.m_wstrUserIP			= L"NoData";
	}
	else
	{
		kPacketLog.m_wstrTotalOrderID	= kPacketReqBuy.m_wstrTotalOrderID;
		kPacketLog.m_iServerGroupID		= kPacketReqBuy.m_iServerGroupID;
		kPacketLog.m_wstrUserID			= kPacketReqBuy.m_wstrUserID;
		kPacketLog.m_iUserUID			= iActorUID;
		kPacketLog.m_wstrNickName		= kPacketReqBuy.m_wstrNickName;

		// ���� �ݾ��� ����Ѵ�.
		int iTotalPrice = 0;
		std::vector< KBillBuyInfo >::iterator vit;
		for( vit = kPacketReqBuy.m_vecBillBuyInfo.begin(); vit != kPacketReqBuy.m_vecBillBuyInfo.end(); vit++ )
		{
			iTotalPrice += vit->m_iPoint;
		}
		kPacketLog.m_iGashPoint			= iTotalPrice;
		kPacketLog.m_iBeforeCash		= iHaveGash;
		if(kPacketLog.m_iFlag == 1 || kPacketLog.m_iFlag == 3)
			kPacketLog.m_iAfterGash			= iHaveGash - iTotalPrice;
		else
			kPacketLog.m_iAfterGash		= 0;
		if(kPacketReqBuy.m_vecBillBuyInfo.empty() == false)
		{
			kPacketLog.m_iItemID			= kPacketReqBuy.m_vecBillBuyInfo.begin()->m_iProductID;
			kPacketLog.m_iProductID			= kPacketReqBuy.m_vecBillBuyInfo.begin()->m_iProductNo;
		}
		//kPacketLog.m_iFlag				= 1;
		kPacketLog.m_wstrMemo			= L"Buy";
		
		kPacketLog.m_wstrUserIP			= KncUtil::toWideString(kPacketReqBuy.m_strUserIP);
	}

	SendToGASHBillingDB( iActorUID, EGASH_USE_GASH_LOG_NOT, kPacketLog );	
}



IMPL_ON_FUNC( EBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// �Ϻ� ���� ��� ����

	START_LOG( cerr, L"�븸/ȫ���� �ۺ� ���� ���� �� �ޱ� ������ ���� �ڵ� Ÿ�� �ȵ�!!!!!!" )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSerialCode)
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_USE_COUPON_ACK, kPacketAck );
}


#endif // SERV_BILLING_ADD_GASH_POINT
//}}

#endif // SERV_COUNTRY_TWHK