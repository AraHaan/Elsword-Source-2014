#include "ActorManager.h"
#include "BRPayletterBilling.h"

#ifdef SERV_COUNTRY_BR

#include "KncSend.h"
#include "BRPayletterBillingManager.h"
#include "NetError.h"

#include "BillManager.h"



#define CLASS_TYPE  KBRPayletterBilling

KBRPayletterBilling::KBRPayletterBilling( void )
{
	int a = 0;
	a = 1;
}

KBRPayletterBilling::~KBRPayletterBilling( void )
{
	End( 15000 );
}

void KBRPayletterBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_BILLING_BALANCE_REQ );
		CASE( EPL_BR_BALANCE_CHECK_ACK );

		CASE( EBILL_BUY_PRODUCT_REQ );
		CASE( EPL_BR_PURCHASEITEM_ACK );
		
	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
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


void KBRPayletterBilling::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KBRPayletterBilling::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KBRPayletterBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID )
{
	SendToKOGBillingDB( nUserUID, usEventID, char() );
}

IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKBRPayletterBillingManager()->IsInitialized() ) 
	{
		KEPUBLISHER_BILLING_BALANCE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_28;// ���� ������ �ʱ�ȭ ���� �ʾҽ��ϴ�.	
		kPacketAck.m_ulBalance = 0;
		SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
		return;
	}
	//}}
	
	KEPL_BR_BALANCE_CHECK_REQ kPacket;
	kPacket.m_usReqLen = sizeof(KEPL_BR_BALANCE_CHECK_REQ);
	kPacket.m_usReqType = KBRPayletterBillingPacket::BALANCE_CHECK;
	kPacket.m_ulUserNo = kPacket_.m_uiPublisherUID;		// �ۺ��� ����ũ Ű
	kPacket.m_wstrUserID = kPacket_.m_wstrAccount;

	SiKBRPayletterBillingManager()->InsertUniqueKeyUserUID( kPacket.m_ulUserNo, FIRST_SENDER_UID );

	boost::shared_ptr< KBRPayletterBillingPacket > spPacket( new KBRPayletterBillingPacket );
	spPacket->Write( kPacket );
	SiKBRPayletterBillingManager()->QueueingSendPacket( spPacket );
}


IMPL_ON_FUNC( EPL_BR_BALANCE_CHECK_ACK )
{
	bool bSucceed = ( kPacket_.m_usRetCode == 0 );

	UidType iUserUID = SiKBRPayletterBillingManager()->GetUniqueKeyUserUID( kPacket_.m_ulUserNo );
	SiKBRPayletterBillingManager()->DeleteUniqueKeyUserUID( kPacket_.m_ulUserNo );

	if( iUserUID <= 0 || kPacket_.m_ulUserNo <= 0 )
	{
		START_LOG( cerr, L"�ۺ��� ����ũ Ű�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulUserNo )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		return;
	}

	KEPUBLISHER_BILLING_BALANCE_ACK kPacketAck;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// ĳ�� ������ �ϳ��� ���°��� �Ϲ������� �ۺ�ĳ���� ����.
	kPacketAck.m_iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	switch( kPacket_.m_usRetCode )
	{
		// Success
	case 0:	kPacketAck.m_iOK = NetError::NET_OK;	break;

		// Fail
	case 5001:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_01;	break;
	case 5002:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_02;	break;
	case 5011:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_03;	break;
	case 5012:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_04;	break;
	case 5013:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_05;	break;
	case 5014:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_06;	break;
	case 5017:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_07;	break;
	case 5018:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_08;	break;
	case 5021:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_09;	break;
	case 5041:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_10;	break;
	case 5049:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_11;	break;
	case 5050:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_12;	break;
	case 5051:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_13;	break;
	case 5052:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_14;	break;
	case 5053:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_15;	break;
	case 5054:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_16;	break;
	case 5055:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_17;	break;
	case 5056:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_18;	break;
	case 5057:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_19;	break;
	case 5060:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_20;	break;
	case 5070:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_21;	break;
	case 5550:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_22;	break;
	case 5558:	kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_23;	break;
	default:	
		kPacketAck.m_iOK = NetError::ERR_NX_SHOP_04;	
		break;
	}

	if(kPacketAck.m_iOK == NetError::NET_OK)
	{
		START_LOG( clog2, L"�ܾ���ȸ ����" )						
			<< END_LOG;	
	}
	else
	{
		START_LOG( cerr, L"�ܾ���ȸ ����" )			
			<< BUILD_LOG(  kPacket_.m_usRetCode )
			<< BUILD_LOG( kPacket_.m_wstrRetMsg )
			<< END_LOG;	
	}

	// ĳ�� ���� ����
	kPacketAck.m_ulBalance = kPacket_.m_ulRealCash + kPacket_.m_ulBonusCash;
	SendToUser( iUserUID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
}


IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKBRPayletterBillingManager()->IsInitialized() )
	{
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_28;// ���� ������ �ʱ�ȭ ���� �ʾҽ��ϴ�.
		SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacketAck );

		return;
	}
	//}}

	// ������� ����ϱ� ���ؼ� ��Ŷ�� ������� �д�.
	bool bRet = false;
	SiKBRPayletterBillingManager()->InsertBuyPacket(kPacket_, bRet);
	if( false == bRet )
	{
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_29;// ���� ������ �ʱ�ȭ ���� �ʾҽ��ϴ�.
		SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacketAck );

		return;
	}

	boost::shared_ptr< KBRPayletterBillingPacket > spPacket( new KBRPayletterBillingPacket );
	spPacket->Write( kPacket_ );
	SiKBRPayletterBillingManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( EPL_BR_PURCHASEITEM_ACK )
{
	if( kPacket_.m_usRetCode == NetError::NET_OK )
	{
		KEBILL_BUY_PRODUCT_REQ kPacket;
		bool bRet = false;
		SiKBRPayletterBillingManager()->GetBuyPacket(kPacket_.m_ulReqKey, kPacket, bRet );
		if( true == bRet )
		{
			SendToKOGBillingDB( kPacket.m_iUserUID, EBILL_BUY_PRODUCT_REQ, kPacket);
		}
		else
		{
			START_LOG( cerr, L"[����] ���� ���� ��Ŷ�� �Դµ� �������� ��Ŷ�� ����?!, �̷��� ������ �ǰ� ������ �������� ���ѷ� ��ġ��" )
				<< BUILD_LOG( kPacket_.m_ulReqKey )
				<< BUILD_LOG( kPacket_.m_wstrOrderID )
				<< BUILD_LOG( kPacket.m_iUserUID )
				<< END_LOG;

			KEBILL_BUY_PRODUCT_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_PAYLETTER_BILLING_30;// ���� ������ �ʱ�ȭ ���� �ʾҽ��ϴ�.
			SendToUser( kPacket.m_iUserUID, EBILL_BUY_PRODUCT_ACK, kPacketAck );
		}		
	}
	else
	{
		// ���� ��Ŷ ó���� �Ʒ� �Լ����� �Ѵ�
		_ProcessErrorPacket( kPacket_ );
	}
}

void KBRPayletterBilling::_ProcessErrorPacket( IN const KEPL_BR_PURCHASEITEM_ACK kPacket_ )
{
	START_LOG( cerr, L"[����] ���ſ���" )
		<< BUILD_LOG( kPacket_.m_ulReqKey )
		<< BUILD_LOG( kPacket_.m_usRetCode )
		<< BUILD_LOG( kPacket_.m_wstrRetMsg )
		<< BUILD_LOG( kPacket_.m_wstrUserID.c_str() )
		<< BUILD_LOG( kPacket_.m_bResurrectionStone )
		<< END_LOG;

	int iOK = _GetErrorPacketType( kPacket_.m_usRetCode );

	if( iOK != NetError::NET_OK )
	{
		SiKBRPayletterBillingManager()->EraseBuyPacket( kPacket_.m_ulReqKey );

		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = iOK;// ���� ������ �ʱ�ȭ ���� �ʾҽ��ϴ�.
		SendToUser( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_ACK, kPacketAck );
	}
	else
	{
		// ���� ó�� �Դµ� ���� ���� �����̴�?
		START_LOG( cerr, L"[����] ��Ŷ ���� ó�� �ϴµ�, �Ѿ�� ���� �������� �ƴϴ�? ���� �� ���� ����" )
			<< END_LOG;
	}
}

int KBRPayletterBilling::_GetErrorPacketType( IN const unsigned short usRetCode_ )
{
	switch( usRetCode_ )
	{
		// ���� ó�� �ϴµ� ���� ��Ŷ�� ���Դ�!?
	case 0:	break;

	case 5001:		
		return NetError::ERR_PAYLETTER_BILLING_01;
	case 5002:
		return NetError::ERR_PAYLETTER_BILLING_02;
	case 5005:
		return NetError::ERR_PAYLETTER_BILLING_24;
	case 5007:
		return NetError::ERR_PAYLETTER_BILLING_25;
	case 5008:
		return NetError::ERR_PAYLETTER_BILLING_27;
	case 5011:
		return NetError::ERR_PAYLETTER_BILLING_03;
	case 5012:
		return NetError::ERR_PAYLETTER_BILLING_04;
	case 5013:
		return NetError::ERR_PAYLETTER_BILLING_05;
	case 5014:
		return NetError::ERR_PAYLETTER_BILLING_06;
	case 5017:
		return NetError::ERR_PAYLETTER_BILLING_07;
	case 5018:
		return NetError::ERR_PAYLETTER_BILLING_08;
	case 5021:
		return NetError::ERR_PAYLETTER_BILLING_09;
	case 5041:
		return NetError::ERR_PAYLETTER_BILLING_10;
	case 5049:
		return NetError::ERR_PAYLETTER_BILLING_11;
	case 5051:
		return NetError::ERR_PAYLETTER_BILLING_13;
	case 5052:
		return NetError::ERR_PAYLETTER_BILLING_14;
	case 5053:
		return NetError::ERR_PAYLETTER_BILLING_15;
	case 5054:
		return NetError::ERR_PAYLETTER_BILLING_16;
	case 5055:
		return NetError::ERR_PAYLETTER_BILLING_17;
	case 5056:
		return NetError::ERR_PAYLETTER_BILLING_18;
	case 5057:
		return NetError::ERR_PAYLETTER_BILLING_19;
	case 5060:
		return NetError::ERR_PAYLETTER_BILLING_20;
	case 5550:
		return NetError::ERR_PAYLETTER_BILLING_22;//ERR_PAYLETTER_BILLING_12
	case 5558:
	case 5094:
		return NetError::ERR_PAYLETTER_BILLING_23;
	case 5570:
		return NetError::ERR_PAYLETTER_BILLING_26;//ERR_PAYLETTER_BILLING_21	
	default:
		return NetError::ERR_BUY_CASH_ITEM_01;
	}

	START_LOG( cerr, L"[����] ��Ŷ ���� ó�� �ϴµ�, �Ѿ�� ���� �������� �ƴϴ�? ���� �� ���� ����" )
		<< BUILD_LOG( usRetCode_ )
		<< END_LOG;

	return NetError::NET_OK;
}

#endif //SERV_COUNTRY_BR