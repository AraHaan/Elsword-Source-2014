#include "ActorManager.h"
#include "NexonBillingTCP.h"

#include "NexonBillingTCPPacket.h"
#include "NexonBillingTCPManager.h"
#include "NetError.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLSkillTree.h"

//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}

#define CLASS_TYPE  KNexonBillingTCP

KNexonBillingTCP::KNexonBillingTCP( void )
{
}

KNexonBillingTCP::~KNexonBillingTCP( void )
{
	End( 15000 );
}

void KNexonBillingTCP::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
    CASE( ENX_BT_INITIALIZE_ACK );
    CASE( ENX_BT_HEART_BEAT_ACK );
    CASE( ENX_BT_CATEGORY_INQUIRY_ACK );
    CASE( ENX_BT_PRODUCT_INQUIRY_ACK );
    CASE( ENX_BT_CHECK_BALANCE_REQ );
    CASE( ENX_BT_CHECK_BALANCE_ACK );
    CASE( ENX_BT_PURCHASE_ITEM_REQ );
    CASE( ENX_BT_PURCHASE_ITEM_ACK );
    CASE( ENX_BT_PURCHASE_GIFT_REQ );
    CASE( ENX_BT_PURCHASE_GIFT_ACK );
	//{{ 2009. 10. 14  ������	�ڵ�����
	CASE( ENX_BT_CHECK_AUTO_PAYMENT_USER_REQ );
	CASE( ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK );
	//}}
    CASE( ENX_BT_COUPON_INQUIRY_REQ );
    CASE( ENX_BT_COUPON_INQUIRY_ACK );
	//{{ 2009. 3. 24  ������	������뿩�� ��ȸ
	CASE( ENX_BT_COUPON_INQUIRY_NEW_REQ );
	CASE( ENX_BT_COUPON_INQUIRY_NEW_ACK );
	//}}
    CASE( ENX_BT_COUPON_USING_REQ );
    CASE( ENX_BT_COUPON_USING_ACK );	
    CASE( ENX_BT_NISMS_INVENTORY_INQUIRY_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_INQUIRY_ACK );
	//{{ 2008. 6. 20  ������  �����ϱ�
	CASE( ENX_BT_NISMS_INVENTORY_CHECK_REQ ); 
	CASE( ENX_BT_NISMS_INVENTORY_CHECK_ACK );
	CASE( ENX_BT_NISMS_INVENTORY_READABLE_REQ );
	CASE( ENX_BT_NISMS_INVENTORY_READABLE_ACK );
	//}}
    CASE( ENX_BT_NISMS_INVENTORY_READ_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_READ_ACK );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ACK );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK );
	//{{ 2008. 6. 16  ������  �ؽ����� ��Ű�� �Ⱦ�
	CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ );
	CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK );
	//}}
	//{{ 2010. 11. 17	������	�ؽ� ���� û�� öȸ
#ifdef SERV_NX_BILLING_REFUND
	CASE( ENX_BT_REFUND_REQ );
	CASE( ENX_BT_REFUND_ACK );
#endif SERV_NX_BILLING_REFUND
	//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-21	// �ڼ���
	CASE( ENX_BT_NISMS_ITEM_COUPON_USING_REQ );
	CASE( ENX_BT_NISMS_ITEM_COUPON_USING_ACK );

	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ );
	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK );

	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ );
	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK );
#endif // SERV_NEXON_COUPON_SYSTEM

	default:
		START_LOG( cerr, L"�ڵ鷯�� �������� ���� �̺�Ʈ." )
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

IMPL_ON_FUNC( ENX_BT_INITIALIZE_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );
    SiKNexonBillingTCPManager()->HandleInitializeAck( bSucceed );

    if( bSucceed )
    {
        KENX_BT_CATEGORY_INQUIRY_REQ kPacketCIR;
        kPacketCIR.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
        kPacketCIR.m_bytePacketType = KNexonBillingTCPPacket::CATEGORY_INQUIRY;

        boost::shared_ptr< KNexonBillingTCPPacket > spPacketCIR( new KNexonBillingTCPPacket );
        spPacketCIR->Write( kPacketCIR );
        SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketCIR );
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_HEART_BEAT_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    if( kPacket_.m_ulResult == 0x11 )
    {
		//////////////////////////////////////////////////////////////////////////		
		//{{ 2010. 01. 28  ������	��������ó��
#ifdef SERV_BILLING_PRODUCT
		if( SiKNexonBillingTCPManager()->GetProductPageNumber() != 0 )
		{
			START_LOG( cout, L"�̹� ��ǰ ����Ʈ�� �ٽ� �޴� ���Դϴ�. ��ǰ ����Ʈ�� ��� ���������� ��ǰ ���� ���� ��û�� ���õ˴ϴ�." )
				<< BUILD_LOG( SiKNexonBillingTCPManager()->GetProductPageNumber() );
			return;
		}
		
		// ��ǰ ������ �ʱ�ȭ �Ѵ�.
		SiKNexonBillingTCPManager()->ResetProductInfo();
#endif SERV_BILLING_PRODUCT
		//}}
		//////////////////////////////////////////////////////////////////////////

        // ��ǰ������ �� �ٽ� �޴´�.		
        SiKNexonBillingTCPManager()->ResetProductPageNumber();

        KENX_BT_PRODUCT_INQUIRY_REQ kPacket;
        kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
        kPacket.m_bytePacketType = KNexonBillingTCPPacket::PRODUCT_INQUIRY;
        kPacket.m_ulPageIndex = SiKNexonBillingTCPManager()->GetNextProductPageNumber();        
		kPacket.m_ulRowPerPage = ROW_PER_PAGE;

        boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
        spPacket->Write( kPacket );
        SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );

		START_LOG( cout, L"��ǰ������ �ٲ�����ϴ�, ��ǰ����Ʈ�� �ٽ� �޽��ϴ�." );
		return;
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_CATEGORY_INQUIRY_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    if( bSucceed )
    {
        KENX_BT_PRODUCT_INQUIRY_REQ kPacket;
        kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
        kPacket.m_bytePacketType = KNexonBillingTCPPacket::PRODUCT_INQUIRY;
        kPacket.m_ulPageIndex = SiKNexonBillingTCPManager()->GetNextProductPageNumber();		
		kPacket.m_ulRowPerPage = ROW_PER_PAGE;

        boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
        spPacket->Write( kPacket );
        SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_PRODUCT_INQUIRY_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    if( bSucceed )
    {
        // ���� ������ ƽ�� �ٸ��� ��ǰ ������ ���ŵǾ��ٴ� ���̴�. �ֱ� ���� �� �����.
        if( !SiKNexonBillingTCPManager()->SetReleaseTick( kPacket_.m_iReleaseTick ) )
        {
			//////////////////////////////////////////////////////////////////////////			
			//{{ 2010. 01. 28  ������	��������ó��
#ifdef SERV_BILLING_PRODUCT
			START_LOG( cout, L"Release Tick ����" )
				<< BUILD_LOG( kPacket_.m_iReleaseTick );
#else
			SiKNexonBillingTCPManager()->ResetProductInfo();
#endif SERV_BILLING_PRODUCT
			//}}
			//////////////////////////////////////////////////////////////////////////			
        }

        std::vector< KNXBTProductInfo >::iterator vit;
        for( vit = kPacket_.m_vecNXBTProductInfo.begin(); vit != kPacket_.m_vecNXBTProductInfo.end(); ++vit )
        {
            SiKNexonBillingTCPManager()->AddProductInfo( *vit );
        }

        if( !kPacket_.m_vecNXBTProductInfo.empty() )
        {
            KENX_BT_PRODUCT_INQUIRY_REQ kPacket;
            kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
            kPacket.m_bytePacketType = KNexonBillingTCPPacket::PRODUCT_INQUIRY;
            kPacket.m_ulPageIndex = SiKNexonBillingTCPManager()->GetNextProductPageNumber();
			kPacket.m_ulRowPerPage = ROW_PER_PAGE;

            boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
            spPacket->Write( kPacket );
            SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
        }
        else
        {
			SiKNexonBillingTCPManager()->BuildProductPage();
			SiKNexonBillingTCPManager()->ResetProductPageNumber();

			// Ŭ���̾�Ʈ�� ��ǰ ������ �ٽ� �� ������� �˸���.
			KEvent kEvent;
			UidType anTrace[2] = { -1, -1 };
			kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
			KActorManager::GetKObj()->SendToAll( kEvent );

			START_LOG( cout, L"���������κ��� ��� ��ǰ ����Ʈ�� �� �޾ҽ��ϴ�!" )
				<< BUILD_LOG( SiKNexonBillingTCPManager()->GetTotalProductCount() );
        }
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_CHECK_BALANCE_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_CHECK_BALANCE_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;

        return;
    }

    KEGS_CHECK_BALANCE_ACK kPacket;
    switch( kPacket_.m_ulResult )
    {
    case 1:
        kPacket.m_iOK = NetError::NET_OK;
        kPacket.m_ulBalance = kPacket_.m_ulBalance;
        break;
    case 12001:
        kPacket.m_iOK = NetError::ERR_NX_SHOP_06;
        break;
    case 12002:
        kPacket.m_iOK = NetError::ERR_NX_SHOP_05;
        break;
		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
    default:
        kPacket.m_iOK = NetError::ERR_NX_SHOP_04;
		START_LOG( cerr, L"�ܾ� ��ȸ ���ϴ� ������ �����ΰ�?" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( kPacket_.m_ulResult );
        break;
    }

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { iUserUID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_CHECK_BALANCE_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_PURCHASE_ITEM_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_PURCHASE_ITEM_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;

        return;
    }

    KEGS_BUY_CASH_ITEM_ACK kPacket;

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	kPacket.m_bUseCoupon = ( kPacket_.m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON ) ? true : false;
#endif // SERV_NEXON_COUPON_SYSTEM

    switch( kPacket_.m_ulResult )
    {
    case 1:
        kPacket.m_iOK = NetError::NET_OK;
        break;
    case 0:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
        break;
    case 2:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_02;
        break;
    case 3:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
        break;
    case 4:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_04;
        break;
    case 5:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_05;
        break;

	case 10:
	case 11:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_07;	// ���� �����Դϴ�.
		break;

    case 21:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_06;
        break;
    case 22:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_07;
        break;
    case 23:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
        break;
    case 24:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_09;
        break;
    case 25:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_10;
        break;
    case 26:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_11;
        break;
    case 27:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_12;
        break;
    case 28:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_13;
        break;
		//{{ 2010. 10. 06	������	���� ����
	case 81:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_20;	// ���� �� ��� Ƚ���� �ʰ��Ͽ� ����Ͻ� �� �����ϴ�.
		break;
	case 99:
		START_LOG( cout, L"�ؽ� ���� DB������ ���� ���� ����!" )
			<< BUILD_LOG( kPacket_.m_ulOrderNo );
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		break;
		//}}
    case 12001:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_14;
        break;
    case 12002:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_15;
        break;
    case 12040:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_16;
        break;
    case 255:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_17;
        break;
		//{{ 2010. 02. 16  ������	���� ���� �ڵ� �߰�
	case 11001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_42;
		break;
	case 11002:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_43;
		break;
		//}}
		//{{ 2010. 8. 18	������	���� ���� �ڵ� �߰�
	case 120062:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_44;
		break;
		//}}
		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
    default:
		START_LOG( cerr, L"���ǵ��� ���� ĳ�������� ���� ���� �ڵ��Դϴ�." )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
        break;
    }

    if( bSucceed )
    {
        std::vector< KNXBTPurchaseAckInfo >::const_iterator vit;
        for( vit = kPacket_.m_vecNXBTPurchaseInfo.begin(); vit != kPacket_.m_vecNXBTPurchaseInfo.end(); ++vit )
        {
            // ��Ȱ���̸� �ٷ� �����´�. ����ǰ Ȯ�嵵 �ٷ� �����´�.
            const int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
            if( iItemID <= 0 )
            {
                START_LOG( cerr, L"������ ID ��ȯ ����." )
                    << BUILD_LOG( vit->m_ulProductNo )
                    << BUILD_LOG( iItemID )
                    << END_LOG;

                continue;
            }

			// [����] ��Ȱ���� �κ��丮 Ȯ���� ��� ���������� �������� �����Ƿ� �ٷ� PICK_UP��Ŷ�� ������.
			switch( iItemID )
			{
			case CXSLItem::CI_RESURRECTION_STONE:
			case CXSLItem::CI_EXPAND_INVENTORY:
			case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
			case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
			case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
				//{{ 2012. 12. 19	������	�ƶ� ���� ĳ����
#ifdef SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
#endif SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
				//}}
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#ifdef SERV_CASH_ITEM_FOR_ADD
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ADD:
#endif	// SERV_CASH_ITEM_FOR_ELESIS

				//{{ 2009. 8. 7  ������		����
			case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
			case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
			case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
			case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
			case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
			case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
				//}}
				//{{ 2010. 04. 27  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
			case CXSLItem::CI_CASH_SKILL_NOTE_ITEM:
#endif SERV_SKILL_NOTE
				//}}
				//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
			case CXSLItem::CI_EXPAND_QUICK_SLOT:
			case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#endif SERV_EXPAND_QUICK_SLOT

#ifdef SERV_SKILL_PAGE_SYSTEM
			case CXSLItem::CI_EXPAND_SKILL_PAGE:
#endif // SERV_SKILL_PAGE_SYSTEM

				//}}
				{
					KENX_BT_NISMS_INVENTORY_PICK_UP_REQ kPacketReq;
					kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
					kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
					kPacketReq.m_ulOrderNo = kPacket_.m_ulOrderNo;
					kPacketReq.m_ulProductNo = vit->m_ulProductNo;
					kPacketReq.m_usOrderQuantity = vit->m_usOrderQuantity;

					SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacketReq.m_ulPacketNo, iUserUID );
					boost::shared_ptr< KNexonBillingTCPPacket > spPacketReq( new KNexonBillingTCPPacket );
					spPacketReq->Write( kPacketReq );
					SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketReq );
				}
				break;

				//{{ 2009. 8. 4  ������		���ν�ų
			default:
				{
					// ���� ��ų ���� ������ or ���� ĳ����
                    if( SiCXSLSkillTree()->IsUnsealItemID( iItemID ) == true  ||
						//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
						CXSLItem::IsClassChangeCashItem( iItemID ) == true	  ||
#endif SERV_UNIT_CLASS_CHANGE_ITEM
						//}}
						CXSLItem::IsJobChangeCashItem( iItemID ) == true )
					{
						KENX_BT_NISMS_INVENTORY_PICK_UP_REQ kPacketReq;
						kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
						kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
						kPacketReq.m_ulOrderNo = kPacket_.m_ulOrderNo;
						kPacketReq.m_ulProductNo = vit->m_ulProductNo;
						kPacketReq.m_usOrderQuantity = vit->m_usOrderQuantity;

						SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacketReq.m_ulPacketNo, iUserUID );
						boost::shared_ptr< KNexonBillingTCPPacket > spPacketReq( new KNexonBillingTCPPacket );
						spPacketReq->Write( kPacketReq );
						SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketReq );
					}
				}
				break;
				//}}
			}

            // ������ ������ ack �� �ش�.
            KNXBTPurchaseReqInfo kPRInfo;
            kPRInfo.m_ulProductNo = vit->m_ulProductNo;
            kPRInfo.m_usOrderQuantity = vit->m_usOrderQuantity;
            kPacket.m_vecPurchaseReqInfo.push_back( kPRInfo );
        }
    }
	else
	{
		std::vector< KNXBTPurchaseAckInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecNXBTPurchaseInfo.begin(); vit != kPacket_.m_vecNXBTPurchaseInfo.end(); ++vit )
		{
			// ������ ������ ack �� �ش�.
			KNXBTPurchaseReqInfo kPRInfo;
			kPRInfo.m_ulProductNo = vit->m_ulProductNo;
			kPRInfo.m_usOrderQuantity = vit->m_usOrderQuantity;
			kPacket.m_vecPurchaseReqInfo.push_back( kPRInfo );
		}

		START_LOG( cerr, L"������ ���� ����" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		BOOST_TEST_FOREACH( const KNXBTPurchaseAckInfo&, kProductAckInfo, kPacket_.m_vecNXBTPurchaseInfo )
		{
			KNXBTProductInfo kProductInfo;
			if( SiKNexonBillingTCPManager()->GetProductInfo( kProductAckInfo.m_ulProductNo, kProductInfo ) == false )
			{
				START_LOG( cerr, L"ProductInfo�� ã�� �� �����ϴ�." )
					<< BUILD_LOG( kProductInfo.m_ulProductNo )
					<< END_LOG;
				continue;
			}

			// ��ǰ ������ID ���
			int iItemID = _wtoi( kProductInfo.m_wstrProductID.c_str() );
			if( iItemID <= 0 )
			{
				START_LOG( cerr, L"������ ID ��ȯ ����." )
					<< BUILD_LOG( kProductInfo.m_ulProductNo )
					<< BUILD_LOG( kProductInfo.m_wstrProductID )
					<< BUILD_LOG( iItemID )
					<< END_LOG;
				continue;
			}

			START_LOG( cerr, L"���� ���� ��ǰ ����" )
				<< BUILD_LOG( kProductAckInfo.m_ulProductNo )
				<< BUILD_LOG( kProductAckInfo.m_usOrderQuantity )
				<< BUILD_LOG( kProductInfo.m_wstrProductID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
		}
	}

    //KEventPtr spEvent( new KEvent );
    //UidType anTrace[2] = { iUserUID, -1 };
    //spEvent->SetData( PI_GS_PROXY, anTrace, ENX_BT_PURCHASE_ITEM_ACK, kPacket );
    //KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	
	//{{ 2008. 5. 13  ������  GSUser�� �ѹ� ���ļ� ������ ����
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_PURCHASE_ITEM_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	//}}

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

//{{ 2008. 6. 12  ������  �����ϱ�
IMPL_ON_FUNC( ENX_BT_PURCHASE_GIFT_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_PURCHASE_GIFT_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	KEGS_PRESENT_CASH_ITEM_ACK kPacket;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	kPacket.m_bUseCoupon = ( kPacket_.m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON ) ? true : false;
#endif // SERV_NEXON_COUPON_SYSTEM

	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;
	case 0:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		break;
	case 2:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_02;
		break;
	case 3:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
		break;
	case 4:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_04;
		break;
	case 5:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_05;
		break;
	case 10:
	case 11:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_07;	// ���� �����Դϴ�.
		break;
	case 21:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_06;
		break;
	case 22:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_07;
		break;
	case 23:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
		break;
	case 24:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_09;
		break;
	case 25:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_10;
		break;
	case 26:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_11;
		break;
	case 27:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_12;
		break;
	case 28:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_13;
		break;
	case 81:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_20;	// ���� �� ��� Ƚ���� �ʰ��Ͽ� ����Ͻ� �� �����ϴ�.
		break;
	case 1000:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_24;
		break;
		//{{ 2008. 6. 26  ������  �����ϱ�
	case 1001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_28;
		break;
		//}}
	case 12001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_14;
		break;
	case 12002:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_15;
		break;
	case 12040:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_16;
		break;
	case 255:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_17;
		break;
		//{{ 2010. 02. 16  ������	���� ���� �ڵ� �߰�
	case 11001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_42;
		break;
	case 11002:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_43;
		break;
		//}}
		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"���ǵ��� ���� ���� ���� ���� �ڵ��Դϴ�." )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		break;
	}

	if( bSucceed )
	{
		std::vector< KNXBTPurchaseAckInfo >::iterator vit;
		for( vit = kPacket_.m_vecNXBTPurchaseInfo.begin(); vit != kPacket_.m_vecNXBTPurchaseInfo.end(); vit++ )
		{
			// ��Ȱ���̸� �ٷ� �����´�. ����ǰ Ȯ�嵵 �ٷ� �����´�.
			//int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
			//if( iItemID <= 0 )
			//{
			//	START_LOG( cerr, L"������ ID ��ȯ ����." )
			//		<< BUILD_LOG( vit->m_ulProductNo )
			//		<< BUILD_LOG( iItemID )
			//		<< END_LOG;

			//	continue;
			//}

			// [����] �����ϱⰡ �Ұ����� ��ǰ�� �̸� �˻縦 �߱⶧���� ���⼭ ���� ó���� �ʿ�� ����.

			// �����忡 �������� ���� �ڵ� [���� ȣ���� ������ ���µ�.. �̹� ���Ŵ� �̷�� ���µ���..]
			//const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
			//if( pItemTemplet == NULL )
			//{
			//    START_LOG( cerr, L"������ ���ø��� ���� ����." )
			//        << BUILD_LOG( iItemID )
			//        << END_LOG;

			//    continue;
			//}

			// [����] ��Ȱ���� �κ��丮 Ȯ���� ��� ���������� �������� �����Ƿ� �ٷ� PICK_UP��Ŷ�� ������.

			//if( iItemID == CXSLItem::CI_RESURRECTION_STONE ||
			//	iItemID == CXSLItem::CI_EXPAND_INVENTORY )
			//{
			//	KENX_BT_NISMS_INVENTORY_PICK_UP_REQ kPacketReq;
			//	kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
			//	kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
			//	kPacketReq.m_ulOrderNo = kPacket_.m_ulOrderNo;
			//	kPacketReq.m_ulProductNo = vit->m_ulProductNo;
			//	kPacketReq.m_usOrderQuantity = vit->m_usOrderQuantity;

			//	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacketReq.m_ulPacketNo, iUserUID );
			//	boost::shared_ptr< KNexonBillingTCPPacket > spPacketReq( new KNexonBillingTCPPacket );
			//	spPacketReq->Write( kPacketReq );
			//	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketReq );
			//}

			// ������ ������ ack �� �ش�.
			KNXBTPurchaseReqInfo kPRInfo;
			kPRInfo.m_ulProductNo = vit->m_ulProductNo;
			kPRInfo.m_usOrderQuantity = vit->m_usOrderQuantity;
			kPacket.m_vecPurchaseReqInfo.push_back( kPRInfo );
		}
	}
	else
	{
		START_LOG( cerr, L"������ ���� ����" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
	}

	//KEventPtr spEvent( new KEvent );
	//UidType anTrace[2] = { iUserUID, -1 };
	//spEvent->SetData( PI_GS_PROXY, anTrace, EGS_BUY_CASH_ITEM_ACK, kPacket );
	//KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	//{{ 2008. 5. 13  ������  GSUser�� �ѹ� ���ļ� ������ ����
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_PURCHASE_GIFT_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	//}}

	//LOG_SUCCESS( bSucceed )
	//	<< BUILD_LOG( kPacket_.m_ulResult )
	//	<< END_LOG;
}
//}}

//{{ 2009. 10. 14  ������	�ڵ�����
IMPL_ON_FUNC( ENX_BT_CHECK_AUTO_PAYMENT_USER_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK )
{
	//bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )			
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		return;
	}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK, kPacket_ );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	//LOG_SUCCESS( bSucceed )
	//	<< BUILD_LOG( kPacket_.m_ulResult )
	//	<< END_LOG;
}
//}}

IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_REQ )
{
	// ���� ���ҵ忡���� ������� �ʴ� ����Դϴ�.
}

IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_ACK )
{
	// ���� ���ҵ忡���� ������� �ʴ� ����Դϴ�.
}

//{{ 2009. 3. 24  ������	������뿩�� ��ȸ
IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_NEW_REQ )
{
	// ���� ���ҵ忡���� ������� �ʴ� ����Դϴ�.
}

IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_NEW_ACK )
{
	// ���� ���ҵ忡���� ������� �ʴ� ����Դϴ�.
}
//}}

IMPL_ON_FUNC( ENX_BT_COUPON_USING_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_COUPON_USING_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 1 );

    KEGS_APPLY_COUPON_ACK kPacket;
    switch( kPacket_.m_ulResult )
    {
    case 1:
        kPacket.m_iOK = NetError::NET_OK;
        break;
	case 5:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_05;
		break;
    case 25:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_03;
        break;
    case 51:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_00;
        break;
    case 52:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_01;
        break;
	case 61:		
    case 62:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
        break;
		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
    default:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_04;
		START_LOG( cerr, L"���ǵ��� ���� ���� �����ڵ��Դϴ�." )
		    << BUILD_LOG( kPacket_.m_ulResult )
		    << END_LOG;
        break;
    }

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;
        
        return;
    }

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { iUserUID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_APPLY_COUPON_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_INQUIRY_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_INQUIRY_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    KEGS_PURCHASED_CASH_ITEM_LIST_ACK kPacket;
    kPacket.m_iOK = bSucceed ? NetError::NET_OK : NetError::ERR_NX_SHOP_00;
    kPacket.m_vecOrderInfo	 = kPacket_.m_vecNXBTOrderInfo;	
	kPacket.m_vecPackageInfo = kPacket_.m_vecNXBTPackageInfo;

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;

        return;
    }

	//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_INQUIRY_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
#else
    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { iUserUID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_PURCHASED_CASH_ITEM_LIST_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
#endif SERV_EVENT_BINGO
	//}}

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

//{{ 2008. 6. 20  ������  �����ϱ�
IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_CHECK_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_CHECK_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	if( bSucceed  &&  kPacket_.m_byteOrderType == 2 )
	{
		if( kPacket_.m_ulTotalCount > 0 )
		{
			KEGS_NEW_PRESENT_CASH_INVENTORY_NOT kPacket;
			kPacket.m_iPresentCount = kPacket_.m_ulTotalCount;

			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { iUserUID, -1 };
			spEvent->SetData( PI_GS_PROXY, anTrace, EGS_NEW_PRESENT_CASH_INVENTORY_NOT, kPacket );
			KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
		}
	}
	else
	{
		//START_LOG( cerr, L"�̻��� ĳ�� �κ� üũ ������ �Խ��ϴ�." )
		//	<< BUILD_LOG( iUserUID )
		//	<< BUILD_LOG( kPacket_.m_ulResult )
		//	<< BUILD_LOG( kPacket_.m_byteOrderType )
		//	<< BUILD_LOG( kPacket_.m_ulTotalCount )
		//	<< END_LOG;
	}

	//LOG_SUCCESS( bSucceed )
	//    << BUILD_LOG( kPacket_.m_ulResult )
	//    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READABLE_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READABLE_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	if( bSucceed  &&  kPacket_.m_byteOrderType == 2 )
	{
		if( kPacket_.m_ulTotalCount > 0 )
		{
			KEGS_NEW_PRESENT_CASH_INVENTORY_NOT kPacket;
			kPacket.m_iPresentCount = kPacket_.m_ulTotalCount;

			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { iUserUID, -1 };
			spEvent->SetData( PI_GS_PROXY, anTrace, EGS_NEW_PRESENT_CASH_INVENTORY_NOT, kPacket );
			KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
		}
	}
	else
	{
		//START_LOG( cerr, L"�̻��� ĳ�� �κ� üũ ������ �Խ��ϴ�." )
		//	<< BUILD_LOG( iUserUID )
		//	<< BUILD_LOG( kPacket_.m_ulResult )
		//	<< BUILD_LOG( kPacket_.m_byteOrderType )
		//	<< BUILD_LOG( kPacket_.m_ulTotalCount )
		//	<< END_LOG;
	}

	//LOG_SUCCESS( bSucceed )
	//    << BUILD_LOG( kPacket_.m_ulResult )
	//    << END_LOG;
}
//}}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READ_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READ_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	//if( iUserUID <= 0 )
	//{
	//	START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
	//		<< BUILD_LOG( kPacket_.m_ulPacketNo )
	//		<< BUILD_LOG( kPacket_.m_ulOrderNo )
	//		<< BUILD_LOG( kPacket_.m_ulProductNo )
	//		<< END_LOG;

	//	return;
	//}

	//KEventPtr spEvent( new KEvent );
	//spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ACK, kPacket_ );
	//KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	LOG_SUCCESS( bSucceed )
	    << BUILD_LOG( kPacket_.m_ulResult )
	    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << BUILD_LOG( kPacket_.m_ulOrderNo )
            << BUILD_LOG( kPacket_.m_ulProductNo )
            << END_LOG;

        return;
    }

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ACK, kPacket_ );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << BUILD_LOG( kPacket_.m_ulOrderNo )
            << BUILD_LOG( kPacket_.m_ulProductNo )
            << END_LOG;

        return;
    }

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK, kPacket_ );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_SEND;
	kNot.m_iUserUID				= FIRST_SENDER_UID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << BUILD_LOG( kPacket_.m_ulOrderNo )
            << BUILD_LOG( kPacket_.m_ulProductNo )
            << END_LOG;

        return;
    }

	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�	
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_RECV;
	kNot.m_iUserUID				= iUserUID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK, kPacket_ );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;	
}

//{{ 2008. 6. 16  ������  �ؽ����� ��Ű�� �Ⱦ�
IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_SEND;
	kNot.m_iUserUID				= FIRST_SENDER_UID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );	
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< BUILD_LOG( kPacket_.m_ulOrderNo )
			<< BUILD_LOG( kPacket_.m_ulPackageProductNo )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< END_LOG;

		return;
	}

	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�	
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_RECV;
	kNot.m_iUserUID				= iUserUID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK, kPacket_ );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	LOG_SUCCESS( bSucceed )
		<< BUILD_LOG( kPacket_.m_ulResult )
		<< END_LOG;
}
//}}

//{{ 2010. 11. 17	������	�ؽ� ���� û�� öȸ
#ifdef SERV_NX_BILLING_REFUND
IMPL_ON_FUNC( ENX_BT_REFUND_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );    
}

IMPL_ON_FUNC( ENX_BT_REFUND_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	KEGS_CASH_ITEM_REFUND_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;
	case 0: // �ý��� ����
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_45;
		break;
	case 2: // ȯ���� �Ұ����� ��ǰ �Դϴ�.
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_53; 
		break;
	case 3: // ȯ�Ұ��� �������� ��û������ ŭ
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_46;
		break;
	case 4: // ȯ�ҺҰ��� ( �������� 7���� �ʰ��� ��ǰ )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_47;
		break;
	case 5: // ȯ�ҺҰ��� ( ��Ű���� �Ϻθ� ������ )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_48;
		break;
	case 6: // ȯ�ҺҰ��� ( ������ 0���� ��ǰ )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_49;
		break;
	case 7: // �������� �ʴ� OrderNo&ProductNo
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_52;
		break;
	case 8: // ȯ�ҺҰ��� ( �������� )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_54;
		break;
		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}

	default:
		START_LOG( cerr, L"���ǵ��� ���� û��öȸ ���� �ڵ��Դϴ�." )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_45;
		break;
	}

	//{{ 2008. 5. 13  ������  GSUser�� �ѹ� ���ļ� ������ ����
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_REFUND_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	//}}

	//LOG_SUCCESS( bSucceed )
	//    << BUILD_LOG( kPacket_.m_ulResult )
	//    << END_LOG;
}
#endif SERV_NX_BILLING_REFUND
//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-21	// �ڼ���
IMPL_ON_FUNC( ENX_BT_NISMS_ITEM_COUPON_USING_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_ITEM_COUPON_USING_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 1 );

	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( clog, L"���� �����ڵ�" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
	}

	KEGS_USE_ITEM_COUPON_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;
	case 81:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_20;	// ���� �� ��� Ƚ���� �ʰ��Ͽ� ����Ͻ� �� �����ϴ�.
		break;

	case 51:
	case 101:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// �̹� ���� �����Դϴ�.
		break;

	case 52:
	case 54:
	case 65:
	case 102:
	case 103:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// �Ⱓ�� ����� �����Դϴ�.
		break;

	case 61:		
	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
		break;

	case 100:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_08;	// PC�濡���� ����� �� �ִ� �����Դϴ�.
		break;

		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_04;
		break;
	}

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { iUserUID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_USE_ITEM_COUPON_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK )
{
	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( clog, L"���� �����ڵ�" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
	}

	KEGS_DISCOUNT_COUPON_INQUIRY_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;

	case 51:
	case 101:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// �̹� ���� �����Դϴ�.
		break;

	case 52:
	case 54:
	case 65:
	case 102:
	case 103:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// �Ⱓ�� ����� �����Դϴ�.
		break;

	case 61:		
	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
		break;

	case 100:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_08;	// PC�濡���� ����� �� �ִ� �����Դϴ�.
		break;

		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
		break;
	}

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	kPacket.m_vecDiscountCoupon	= kPacket_.m_vecDiscountCoupon;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { iUserUID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_DISCOUNT_COUPON_INQUIRY_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ )
{
	//{{ 2008. 8. 13  ������	�ʱ�ȭ ���� �ʾҴٸ� ������ �ʴ´�.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK )
{
	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( clog, L"���� �����ڵ�" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
	}

	KEGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;

	case 51:
	case 101:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// �̹� ���� �����Դϴ�.
		break;

	case 52:
	case 54:
	case 65:
	case 102:
	case 103:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// �Ⱓ�� ����� �����Դϴ�.
		break;

	case 61:		
	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
		break;

	case 100:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_08;	// PC�濡���� ����� �� �ִ� �����Դϴ�.
		break;

		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap, ���� �ɹ���
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"�ؽ� ���ոɹ��� ���� ���� ��û" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// ���� ���� �б⿡ �����Ͽ����ϴ�.
		break;
	}

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID�� ã�� ����." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	kPacket.m_vecDiscountCoupon	= kPacket_.m_vecDiscountCoupon;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { iUserUID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}
#endif // SERV_NEXON_COUPON_SYSTEM
