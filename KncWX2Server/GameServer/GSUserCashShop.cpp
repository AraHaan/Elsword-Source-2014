#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "NexonBillingTCPManager.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"

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
#ifdef SERV_NAVER_CHANNELING
    #include "NaverSOAPManager.h"
    #include "NaverSoap.h"
#endif SERV_NAVER_CHANNELING

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

#include "X2Data/XSLSquareUnit.h"

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////
#include "boost/lexical_cast.hpp"

#define CLASS_TYPE      KGSUser

#ifndef SERV_GLOBAL_BILLING

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

	KENX_BT_CHECK_BALANCE_REQ kPacket;
	kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();

	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	kPacket.m_bytePacketType = KNexonBillingTCPPacket::CHECK_BALANCE;

	if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // ���Ϸ��� ä�θ� ����
	{
		kPacket.m_ulPaymentType = 14001;
	}
	else
	{
		kPacket.m_ulPaymentType = 13001;
	}
#else
	kPacket.m_bytePacketType = KNexonBillingTCPPacket::CHECK_BALANCE;
#endif SERV_TOONILAND_CHANNELING
	//}}

	//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
	if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // ���Ϸ��� ä�θ� ����
	{
		GetToonilandID( kPacket.m_wstrUserID );

		if( kPacket.m_wstrUserID.empty() )
		{
			START_LOG( cerr, L"���Ϸ���ID�� ����? ���� ���� ����?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< END_LOG;

			KEGS_CHECK_BALANCE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_CHECK_BALANCE_ACK, kPacketAck );
			return;
		}
	}
	else
	{
		kPacket.m_wstrUserID = GetName();
	}
#else
	kPacket.m_wstrUserID = GetName();
#endif SERV_CHANNELING_SOAP
	//}}

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_CHECK_BALANCE_REQ, kPacket );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
}

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

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}

    // ���� �ø����� 35��
    if( kPacket_.m_wstrCouponSerial.size() != 35 )
    {
        KEGS_APPLY_COUPON_ACK kPacketAck;
        kPacketAck.m_iOK = NetError::ERR_NX_WEB_SERVICE_01;
        SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
        return;
    }

    KENX_BT_COUPON_USING_REQ kPacket;
    kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
    kPacket.m_bytePacketType = KNexonBillingTCPPacket::COUPON_USING;
    kPacket.m_ulRemoteIP = ( unsigned long )GetIP();
    kPacket.m_byteReason = 0;//m_bIsGameBang ? 3 : 0;
    kPacket.m_wstrGameID = GetName();
    kPacket.m_byteIsCafe = 0;//( ( m_bytePCBangAuthResult == 1 ) && m_bIsGameBang ) ? 1 : 0; -- �̰� ���̷�?
    kPacket.m_wstrUserID = GetName();
    kPacket.m_ulUserOID = ( unsigned long )m_kNexonAccountInfo.m_uiNexonSN;
    kPacket.m_wstrUserName = GetUserName();
    kPacket.m_wstrCouponString = kPacket_.m_wstrCouponSerial;

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_COUPON_USING_REQ, kPacket );
    SiKNexonBillingTCPManager()->QueueingEvent( spEvent );

/*
    KPurchaseCoupon kPacket;
    kPacket.m_uiNexonSN = m_kNexonAccountInfo.m_uiNexonSN;
    kPacket.m_wstrUserID = GetName();
    kPacket.m_wstrUserName = GetUserName();
    kPacket.m_wstrCouponSerial = kPacket_.m_wstrCouponSerial;
    kPacket.m_iGameType = 29;       // ���ҵ�� 29
    kPacket.m_iServerDomain = KBaseServer::GetKObj()->GetServerGroupID();
    kPacket.m_wstrUnitNickName = GetCharName();
    kPacket.m_iReason = 1;          // 1 - ����, 2 - Web
    kPacket.m_wstrCafeNo.clear();

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData( PI_GS_NX_BILLING, anTrace, EGS_APPLY_COUPON_REQ, kPacket );

    SiKNexonBillingManager()->QueueingEvent( spEvent );
*/
    START_LOG( clog, L"���� �Է�." )
        << BUILD_LOG( kPacket_.m_wstrCouponSerial );
}

#endif // ndef SERV_GLOBAL_BILLING

IMPL_ON_FUNC( EGS_PURCHASED_CASH_ITEM_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PURCHASED_CASH_ITEM_LIST_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ĳ���� �κ��丮 ���� ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_PURCHASED_CASH_ITEM_LIST_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

    KENX_BT_NISMS_INVENTORY_INQUIRY_REQ kPacket;
    kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
    kPacket.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_INQUIRY;
    kPacket.m_wstrGameID = GetName();
    kPacket.m_byteShowInventory = 10;       // 10 : true, 30 : false
    kPacket.m_ulPageIndex = kPacket_.m_iCurrentPage;
    kPacket.m_ulRowPerPage = kPacket_.m_nItemPerPage;

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_INVENTORY_INQUIRY_REQ, kPacket );
    SiKNexonBillingTCPManager()->QueueingEvent( spEvent );

/*
    KDBE_PURCHASED_CASH_ITEM_LIST_REQ kPacket;
    kPacket.m_iCurrentPage = kPacket_.m_iCurrentPage;
    kPacket.m_nItemPerPage = kPacket_.m_nItemPerPage;
    kPacket.m_wstrUnitNickName = GetCharName();

	SendToNXShopDB( EGS_PURCHASED_CASH_ITEM_LIST_REQ, kPacket );
 */
}

#ifndef SERV_GLOBAL_BILLING

IMPL_ON_FUNC( EGS_GET_PURCHASED_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_PURCHASED_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ��ǰ pick up ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2013. 2. 5	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	if( m_kGSBingoEvent.IsCurrentOrderNoItem( kPacket_.m_ulOrderNo ) == false )
	{
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BINGO_03;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_EVENT_BINGO
	//}}

	// 1. ��ǰ ������ID ���
	int iItemID = _wtoi( kPacket_.m_wstrProductID.c_str() );
	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"������ ID ��ȯ ����." )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< BUILD_LOG( kPacket_.m_wstrProductID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
		return;
	}

	std::map< int, int > mapItem; // �κ��丮 �˻��
	
	// 2. ��ǰ ������ �˻��Ѵ�.
	if( kPacket_.m_byteProductKind != 0 )
	{
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
		return;
	}
	
	// 3. ���� ������ Ŭ�������� �˻��Ѵ�.
	if( !m_kInventory.CompareUnitClass( iItemID ) )
	{
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 4. ���������� �������� �κ��丮 �Ǵ� ���࿡ �����鼭 �Ⱓ���̸� ���´�.
	if( m_kInventory.IsExistWithBank( iItemID ) )
	{
		if( kPacket_.m_usProductExpire > 0 )
		{
			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_08;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}
	}


	switch ( GetUnitType() )
	{
	
#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ARA
	case CXSLUnit::UT_ARA:
#endif // SERV_CAN_NOT_GET_CASH_ITEM_ARA

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ELESIS
	case CXSLUnit::UT_ELESIS:
#endif // SERV_CAN_NOT_GET_CASH_ITEM_ELESIS

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ADD
	case CXSLUnit::UT_ADD:
#endif // SERV_CAN_NOT_GET_CASH_ITEM_ADD
		{
			switch( iItemID )
			{
			case 160570:		// ���� ���� ���� ť��
			case 160571:		// ���� ���� ���� ť��
			case 160572:		// ���� ���� ���� ť��
			case 160573:		// ���� ���� �尩 ť��
			case 160574:		// ���� ���� �Ź� ť��
			case 160575:		// ���� ���� ��� ť��
				{
					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					return;
				}break;
			}

		} break;

	default:
		break;
	}

	//{{ 2012. 12. 14  ĳ�� �κ��丮���� �ƶ� ��� �Ұ����� ������ �������°� ���� ��� - ��μ�
#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ARA
	switch( iItemID )
	{
		case 261590:		// õ�� ������ ��ų ���� ü���� �޴� ��Ű��	
		case 261600:		// õ�� ������ ������ �����(II) ��Ű��	
		case 261610:		// õ�� ������ ������ Ȯ��� ��Ű��	
		case 261620:		// õ�� ������ �ູ ��Ű��	
		{
			if( GetUnitType() != CXSLUnit::UT_ARA )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
		}break;
	}
#endif SERV_CAN_NOT_GET_CASH_ITEM_ARA
	//}}

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ELESIS	// ���볯¥: 2013-07-11
	if( GetUnitType() == CXSLUnit::UT_ELESIS )
	{
		switch( iItemID )
		{
		case 160570:		// ���� ���� ���� ť��
		case 160571:		// ���� ���� ���� ť��
		case 160572:		// ���� ���� ���� ť��
		case 160573:		// ���� ���� �尩 ť��
		case 160574:		// ���� ���� �Ź� ť��
		case 160575:		// ���� ���� ��� ť��
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}break;
		}
	}

	switch( iItemID )
	{
	case 261664:		//	�����ý� ���� ���� ��Ű��
	case 261665:		//	��ų ���� ü���� �޴�(�����ý� ����) ��Ű��
	case 261666:		//	������ �����(II)(�����ý� ����) ��Ű��
	case 261667:		//	������ Ȯ���(�����ý� ����) ��Ű��
		{
			if( GetUnitType() != CXSLUnit::UT_ELESIS )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
		}break;
	}
#endif	// SERV_CAN_NOT_GET_CASH_ITEM_ELESIS

	// ĳ���� �κ������� ���������� �������� �ʴ� ĳ�þ������� ��쿡 �ߺ����ſ� ���� ���� ó��
	switch( iItemID )
	{
	case CXSLItem::CI_EXPAND_SKILL_SLOT: // 4.1 ���������� �������� ��ų����B(�Ⱓ��) �̸�, �̹� ��ų����B(�Ⱓ��, Ȥ�� ����)�� ������̸� ������ �� ���� ���´�
		{
			KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE eSkillSlotBExpireState = m_kSkillTree.GetSkillSlotBExpirationState();
			if( KUserSkillTree::SSBES_EXPIRED != eSkillSlotBExpireState )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
		} break;


		// 4.2 ĳ�ý�ų����Ʈ�� �̹� ����ϰ� �ִµ� ����Ʈ�� �ٸ� ĳ�ý�ų����Ʈ�� �����ͼ� �Ⱓ ������ �Ϸ��� �ϸ� ���´�
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013// �۾���¥: 2013-06-25	// �ڼ���
	case CXSLItem::CI_CASH_SKILL_POINT_60_15:
	case CXSLItem::CI_CASH_SKILL_POINT_60_30:
	case CXSLItem::CI_CASH_SKILL_POINT_30_15:
	case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
	case CXSLItem::CI_CASH_SKILL_POINT_30_7:
	case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
	case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
	case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
#else
/*
	case CXSLItem::CI_CASH_SKILL_POINT_5:
	case CXSLItem::CI_CASH_SKILL_POINT_10:
*/
#endif // SERV_UPGRADE_SKILL_SYSTEM_2013
		{
			if( false == m_kSkillTree.IsCashSkillPointExpired() )
			{
				const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( iItemID );

				if( iCSPoint <= 0 )
				{
					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_RESET_SKILL_01;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					return;
				}

				if( iCSPoint != m_kSkillTree.GetMaxCSPoint() )
				{
					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;				// fix!! �����޼��� ����
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
		} break;

		//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
#ifdef SERV_PET_CHANGE_NAME
	case CXSLItem::CI_PET_NAME_CHANGE:
		if( m_kUserPetManager.GetExistPetNum() == 0 )
		{
			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_PET_21;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}
		break;
#endif SERV_PET_CHANGE_NAME
		//}}

		//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	case CXSLItem::CI_GUILD_NAME_CHANGE:
		if( m_kUserGuildManager.IsGuildMaster() == true )
		{
			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_iExceptionProcessItemID = iItemID;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}
		else
		{
			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_67;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}
		break;
#endif SERV_GUILD_CHANGE_NAME
		//}}

	default:
		//{{ 2012. 06. 11	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		if( SiKDailyGiftBoxManager()->IsDailyGiftBoxItemID( iItemID ) == true )
		{
			std::pair<std::multimap<int, KDailyGiftBoxInfo>::const_iterator, std::multimap<int, KDailyGiftBoxInfo>::const_iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( iItemID );
			for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
			{
				if( pairFindResult.first->second.m_iUnitUID != GetCharUID() )
					continue;

				switch( pairFindResult.first->second.m_iState )
				{
				case KDailyGiftBoxManager::PURCHASED_REFUND:
					{
						KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_52;
						SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
						return;
					}
					break;
				case KDailyGiftBoxManager::PURCHASED_CONFIRM:
					{
						KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
						SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
						return;
					}
					break;
				}
				break;
			}
		}
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
		break;
	}
	
	// �κ��丮 ���� ���� �˻�
	switch( iItemID )
	{
	case CXSLItem::CI_EXPAND_SKILL_SLOT:
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013// �۾���¥: 2013-06-25	// �ڼ���
	case CXSLItem::CI_CASH_SKILL_POINT_60_15:
	case CXSLItem::CI_CASH_SKILL_POINT_60_30:
	case CXSLItem::CI_CASH_SKILL_POINT_30_15:
	case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
	case CXSLItem::CI_CASH_SKILL_POINT_30_7:
	case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
	case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
	case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
#else
/*
	case CXSLItem::CI_CASH_SKILL_POINT_5:
	case CXSLItem::CI_CASH_SKILL_POINT_10:
*/
#endif // SERV_UPGRADE_SKILL_SYSTEM_2013

		//{{ 2012. 10. 12	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	case KGSBingoEvent::BSV_MIX_ITEM_ID:
	case KGSBingoEvent::BSV_RESET_ITEM_ID:
#endif SERV_EVENT_BINGO
		//}}
#ifdef SERV_ADD_WARP_BUTTON
	case CXSLItem::CI_WARP_VIP_ITEM:
#ifdef SERV_VIP_SYSTEM
	case CXSLItem::SI_USE_FREE_BY_FIELD_VIP:
#endif SERV_VIP_SYSTEM
#endif // SERV_ADD_WARP_BUTTON
		{
			// ĳ���κ����� �� ĳ���ͷ� ������ �� �� �κ��� ���������� ���� �ʴ� �͵��� �κ��丮 �������� �˻縦 ���� �ʴ´�.
			
		} break;

	default:
		{
			// 5. �������� �˻縦 ���� �غ�
			int iQuantity = int( kPacket_.m_usProductPieces ) * kPacket_.m_iQuantity;
			mapItem.insert( std::make_pair( iItemID, iQuantity ) );	

			// 6. �κ��丮�� �������� �˻�
			if( !m_kInventory.IsEnoughSpaceExist( mapItem ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_02;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
		} break;
	}

	// 7. �����ۿ� �ο��� ���Ͽɼ�
	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	if( m_kUserCashInventory.SetSocketForCash( kPacket_.m_mapSocketForCash, kPacket_.m_mapSocketGroupIDForCash ) == false )
	{
		// ���������� �ʴ� ���� ������ �����Ϸ� �մϴ�.
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SOCKET_ITEM_09;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
		return;
	}
#else
	m_kUserCashInventory.SetSocketForCash( kPacket_.m_mapSocketForCash );
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}


	//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	if( m_kGSBingoEvent.IsPresentedItem( kPacket_.m_ulOrderNo ) == true )
	{
		m_kGSBingoEvent.AddOrderNoForPresentedItem( kPacket_.m_ulOrderNo );
	}
#endif SERV_EVENT_BINGO
	//}}

    KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ kPacket;
    kPacket.m_ulPacketNo	  = SiKNexonBillingTCPManager()->GetNextPacketNo();
    kPacket.m_bytePacketType  = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
    kPacket.m_ulOrderNo		  = kPacket_.m_ulOrderNo;
    kPacket.m_ulProductNo	  = kPacket_.m_ulProductNo;
    kPacket.m_usOrderQuantity = kPacket_.m_iQuantity;

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ, kPacket );
    SiKNexonBillingTCPManager()->QueueingEvent( spEvent );

    /*
    KDBE_GET_PURCHASED_CASH_ITEM_REQ kPacket;
    kPacket.m_iOrderNo			= kPacket_.m_iOrderNo;
    kPacket.m_iPackageProductNo = kPacket_.m_iPackageProductNo;
    kPacket.m_iProductNo		= kPacket_.m_iProductNo;
    kPacket.m_nQuantity			= kPacket_.m_nQuantity;
    kPacket.m_iItemID			= iItemID;

    SendToNXShopDB( DBE_GET_PURCHASED_CASH_ITEM_REQ, kPacket );
    */
}

#endif // ndef SERV_GLOBAL_BILLING

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ACK )
{
    //if( kPacket_.m_ulResult != 1 )
    //{
    //    START_LOG( cerr, L"ĳ�� ������ ���� ���� ����." )
    //        << BUILD_LOG( GetUID() )
    //        << BUILD_LOG( GetName() )
    //        << BUILD_LOG( kPacket_.m_ulResult )
    //        << END_LOG;

    //    KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
    //    kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
    //    SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
    //    return;
    //}

    //int iItemID = SiKNexonBillingTCPManager()->GetItemID( kPacket_.m_ulProductNo );
    //if( iItemID <= 0 )
    //{
    //    START_LOG( cerr, L"������ ID ��ȯ ����." )
    //        << BUILD_LOG( kPacket_.m_ulProductNo )
    //        << BUILD_LOG( iItemID )
    //        << END_LOG;

    //    KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
    //    kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
    //    SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
    //    return;
    //}

    //std::map< int, KItemInfo > mapItem;

    //CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
    //if( pItemTemplet == NULL )
    //{
    //    START_LOG( cerr, L"������ ���ø��� ���� ����." )
    //        << BUILD_LOG( iItemID )
    //        << END_LOG;

    //    KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
    //    kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
    //    SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
    //    return;
    //}

    //// ��Ȱ���̸� �ٷ� ä���ش�.
    //if( pItemTemplet->m_Name.compare( L"��Ȱ��" ) == 0 )
    //{
    //    m_iNumResurrectionStone += pItemTemplet->m_Quantity;
    //    KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
    //    kPacket.m_iOK = NetError::NET_OK;
    //    kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
    //    SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
    //    return;
    //}

    //KItemInfo kItemInfo;
    //kItemInfo.m_iItemID = iItemID;
    //kItemInfo.m_iUsageType = pItemTemplet->m_PeriodType;
    //kItemInfo.m_iQuantity = ( int )kPacket_.m_usOrderQuantity;
    //kItemInfo.m_iEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
    //kItemInfo.m_cEnchantLevel = 0;
    //std::map< int, int >::iterator mitSocket;
    //mitSocket = m_mapSocketForCash.find( iItemID );
    //if( mitSocket != m_mapSocketForCash.end() )
    //{
    //    kItemInfo.m_vecItemSocket.push_back( mitSocket->second );
    //}
    //mapItem.insert( std::make_pair( iItemID, kItemInfo ) );

    //KDBE_INSERT_ITEM_REQ kPacketReq;
    //m_kInventory.PrepareInsert( mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

    //if( !kPacketReq.m_vecItemInfo.empty() )
    //{
    //    kPacketReq.m_iUnitUID = GetCharUID();
    //    m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
    //    m_kInventory.FlushEnduranceChange( kPacketReq.m_kItemEnduranceUpdate );
    //    m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
    //    SendToGameDB( DBE_INSERT_PURCHASED_CASH_ITEM_REQ, kPacketReq );
    //}
    //else
    //{
    //    KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
    //    kPacket.m_iOK = NetError::NET_OK;
    //    kPacket.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
    //    SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
    //}
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK )
{
	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( cerr, L"ĳ�� ������ ���� ���� ����." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
		if( m_kUserGuildManager.CheckGuildNameChanging() == true )
		{
			KELG_CHANGE_GUILD_NAME_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
			kPacket.m_iUnitUID = GetCharUID();
			SendToLoginServer( DBE_CHANGE_GUILD_NAME_ACK, kPacket );
			return;
		}
		//}}

		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
		return;
	}

	std::map< int, KItemInfo > mapItem; // �κ��丮�� �� �����۵�
	//{{ 2009. 9. 2  ������		�к�
	std::set< int > setSealCashItem;
	//}}

	//////////////////////////////////////////////////////////////////////////	
	// ��Ű�� ��ǰ
	if( kPacket_.m_byteProductKind == 1 )
	{
		if( kPacket_.m_vecNXBTSubProductInfo.empty() )
		{
			START_LOG( cerr, L"��Ű�� ��ǰ�ε� ��ǰ������ ����?" )
				<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< END_LOG;			
		}

		int iPackageItemID = _wtoi( kPacket_.m_wstrProductID.c_str() );
		if( iPackageItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< BUILD_LOG( iPackageItemID )
				<< END_LOG;

			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}

		// ��Ű�� ĳ�� ������ �������� �α�
		KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iItemID = iPackageItemID;
		kPacketNot.m_sPeriod = kPacket_.m_usProductExpire;
		kPacketNot.m_iQuantity = kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
		kPacketNot.m_ucLevel = GetLevel();
		kPacketNot.m_cUnitClass = GetUnitClass();
		SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );

		std::vector< KNXBTSubProductInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecNXBTSubProductInfo.begin(); vit != kPacket_.m_vecNXBTSubProductInfo.end(); ++vit )
		{
			const KNXBTSubProductInfo& subProductInfo = *vit;

			int iItemID = _wtoi( subProductInfo.m_wstrProductID.c_str() );
			if( iItemID <= 0 )
			{
				START_LOG( cerr, L"������ ID ��ȯ ����." )
					<< BUILD_LOG( kPacket_.m_ulProductNo )
					<< BUILD_LOG( iItemID )
					<< END_LOG;

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ���ø��� ���� ����." )
					<< BUILD_LOG( iItemID )
					<< END_LOG;

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}

			// ĳ�� ������ �������� �α�
			KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_iItemID = iItemID;
			//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
			kPacketNot.m_sPeriod = subProductInfo.GetProductExpire();
			//}}
			kPacketNot.m_iQuantity = subProductInfo.m_usProductPieces * kPacket_.m_usOrderQuantity;
			kPacketNot.m_ucLevel = GetLevel();
			kPacketNot.m_cUnitClass = GetUnitClass();
			SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );

			//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			// ���� �̺�Ʈ ���� ������, ���� ���� �������� ��ȸ�� ���� ������ �ʴ´�.
			if( ( SiKGSBingoEventInfo()->IsBingoPresent( subProductInfo.m_ulProductNo ) != true ) &&
				( m_kGSBingoEvent.CheckAndRemoveIsPresentedItem( kPacket_.m_ulOrderNo ) != true )
				)
			{
				KNXBTProductInfo kProductInfo;
				if( SiKNexonBillingTCPManager()->GetProductInfo( subProductInfo.m_ulProductNo, kProductInfo ) == true )
				{
					START_LOG( clog, L"��ǰ ���� ����" )
						<< BUILD_LOG( subProductInfo.m_ulProductNo )
						<< BUILD_LOG( subProductInfo.m_wstrProductName )
						<< BUILD_LOG( subProductInfo.m_wstrProductID )
						<< BUILD_LOG( subProductInfo.m_usProductPieces )
						<< BUILD_LOG( kProductInfo.m_usProductPieces )
						<< BUILD_LOG( kPacket_.m_usProductPieces )
						<< BUILD_LOG( kPacket_.m_usOrderQuantity )
						<< END_LOG;

					SiKGSBingoEventInfo()->GetPackageItemPrice( subProductInfo.m_ulProductNo, kProductInfo.m_ulSalePrice );
					m_kGSBingoEvent.AddBalance( kProductInfo.m_ulSalePrice );
					BingoEventDBWrite();
				}
				else
				{
					START_LOG( clog, L"��Ű�� ��ǰ ���� ������ ����." )
						<< BUILD_LOG( subProductInfo.m_ulProductNo )
						<< BUILD_LOG( subProductInfo.m_wstrProductName )
						<< BUILD_LOG( subProductInfo.m_wstrProductID )
						<< END_LOG;
				}
			}
#endif SERV_EVENT_BINGO
			//}}

			switch( pItemTemplet->m_ItemID )
			{
			case CXSLItem::CI_RESURRECTION_STONE: // ��Ȱ��
				{
					// ��Ȱ���̸� �ٷ� ä���ش�.
					m_iNumResurrectionStone += kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;

					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::NET_OK;
					kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					continue;
				}
				break;

			case CXSLItem::CI_EXPAND_INVENTORY: // ����ǰ Ȯ��
				{
					//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_EQUIP ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_ACCESSORY ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_MATERIAL ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_SPECIAL ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUEST ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUICK_SLOT ) ) ) );
					//}}
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;

				//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
			case CXSLItem::CI_EXPAND_QUICK_SLOT:
				{
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;

			case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
				{
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:
				{
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;
#endif	// SERV_CASH_ITEM_FOR_ELESIS
				//}}

			case CXSLItem::CI_EXPAND_SKILL_SLOT: // ��ų ���� ü���� �޴�
				{
					KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
					kPacket.m_iUnitUID		= GetCharUID();
					kPacket.m_iPeriodExpire = kPacket_.m_usProductExpire;

#ifdef SERV_SKILL_PAGE_SYSTEM
					kPacket.m_usTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

					//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
					kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
					//}}
					SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
					continue; // ������ �������� �ʰ� �ٷ� ����
				}
				break;

			case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
				//{{ 2012. 12. 19	������	�ƶ� ���� ĳ����
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
				//}}
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#ifdef SERV_CASH_ITEM_FOR_ADD
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ADD:
#endif	// SERV_CASH_ITEM_FOR_ADD
				{
					KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
					kPacket.m_iUnitUID		= GetCharUID();
					kPacket.m_iPeriodExpire = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
					kPacket.m_usTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

					//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
					kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
					//}}
					SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
					continue; // ������ �������� �ʰ� �ٷ� ����
				} break;

			case CXSLItem::CI_CASH_SKILL_POINT_60_15:
			case CXSLItem::CI_CASH_SKILL_POINT_60_30:
			case CXSLItem::CI_CASH_SKILL_POINT_30_15:
			case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
			case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
			case CXSLItem::CI_CASH_SKILL_POINT_30_7:
			case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
			case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
			case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
			case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
			case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
			case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
			case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
			case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
			case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
			case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
			case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
			case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
			case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
			case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
			case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
			case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
			case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
			case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
			case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
			case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
				{
					const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( pItemTemplet->m_ItemID );

					if( iCSPoint <= 0 )
					{
						KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_RESET_SKILL_01;
						SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
						return;
					}

					if( true == m_kSkillTree.IsCashSkillPointExpired() )
					{
						KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
						kPacket.m_iUnitUID	= GetCharUID();
						kPacket.m_iCSPoint	= iCSPoint;
						//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
						kPacket.m_iPeriod	= subProductInfo.GetProductExpire();
						//}}
						kPacket.m_bUpdateEndDateOnly = false;
						//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
						kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
						//}}
						SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
					}
					else
					{
						if( iCSPoint == m_kSkillTree.GetMaxCSPoint() )
						{
							KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
							kPacket.m_iUnitUID = GetCharUID();
							kPacket.m_iCSPoint = iCSPoint;
							//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
							kPacket.m_iPeriod = subProductInfo.GetProductExpire();
							//}}
							kPacket.m_bUpdateEndDateOnly = true;
							//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
							kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
							//}}
							SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
						}
						else
						{
							START_LOG( cerr, L"���� ����ϰ� �ִ� ĳ�ý�ų����Ʈ�� �ٸ� ������ �Ⱓ���� �Ϸ�����!!" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( pItemTemplet->m_ItemID )
								//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
								<< BUILD_LOG( subProductInfo.GetProductExpire() )
								//}}
								<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
								<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
								<< END_LOG;

							KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
							kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
							SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
							return;
						}
					}

					return; // ������ �������� �ʰ� �ٷ� ����
				} break;

				//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
			case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
			case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
				{
					KDBE_EXPAND_CHAR_SLOT_REQ kPacket;
					kPacket.m_iExpandSlotSize = ( pItemTemplet->m_ItemID == CXSLItem::CI_EXPAND_CHAR_SLOT_1 ? 1 : 2 );
					kPacket.m_iCharSlotMax = CXSLUnit::CHAR_SLOT_MAX;
					SendToGameDB( DBE_EXPAND_CHAR_SLOT_REQ, kPacket );
					continue;
				}
				break;
				//}}

				//{{ 2009. 8. 7  ������		����
			case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
			case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
			case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
			case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
			case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
				{
					// ���� Ȯ���Ϸ� ����!
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;

			case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
				{
					// ���� Ȯ���Ϸ� ����!
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, ( CXSLInventory::SLOT_COUNT_ONE_LINE * 5 ) ) );
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;
				//}}

				//{{ 2013. 2. 28	�ڼ���	 ���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			case CXSLItem::CI_BINGO_MIX_ITEM:
				{
					m_kGSBingoEvent.AddMixChance( kPacket_.m_usProductPieces );
					BingoEventDBWrite();
					continue;
				}
				break;

			case CXSLItem::CI_BINGO_RESET_ITEM:
				{
					m_kGSBingoEvent.AddResetChance( kPacket_.m_usProductPieces );
					BingoEventDBWrite();
					continue;
				}
				break;
#endif SERV_EVENT_BINGO
				//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
			case CXSLItem::CI_EXPAND_SKILL_PAGE:
				{			
					SendExpandSkillPageReqToGameDB( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK, kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity );
					continue;
				}
				break;
#endif	// SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ADD_WARP_BUTTON
			case CXSLItem::CI_WARP_VIP_ITEM:
#ifdef SERV_VIP_SYSTEM
			case CXSLItem::SI_USE_FREE_BY_FIELD_VIP:
#endif SERV_VIP_SYSTEM
				{
					KDBE_INSERT_WARP_VIP_REQ kPacket;
					kPacket.m_iUnitUID	= GetCharUID();
					kPacket.m_iPeriod	= 30;		
					SendToGameDB( DBE_INSERT_WARP_VIP_REQ, kPacket );

					return; // ������ �������� �ʰ� �ٷ� ����
				}
				break;
#endif // SERV_ADD_WARP_BUTTON

			default:
				{
					//{{ 2009. 8. 4  ������		���ν�ų
					std::vector<int> vecSkillID;
					if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, GetUnitClass(), vecSkillID ) == true )
					{
						BOOST_TEST_FOREACH( int, iSkillID, vecSkillID )
						{
							KDBE_UNSEAL_SKILL_REQ kPacketToDB;
							kPacketToDB.m_iUnitUID = GetCharUID();
							kPacketToDB.m_iSkillID = iSkillID;
							SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );
						}
						continue;
					}
				}
			}

			KItemInfo kItemInfo;
			kItemInfo.m_iItemID		= iItemID;
			kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
			kItemInfo.m_iQuantity	= subProductInfo.m_usProductPieces * kPacket_.m_usOrderQuantity; // ��ǰ �⺻ ���� * ���� ����
			kItemInfo.m_sEndurance	= ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
			//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
			kItemInfo.m_sPeriod		= subProductInfo.GetProductExpire();
			//}}
			//{{ 2009. 8. 27  ������	�к�
			if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) == true )
			{
				setSealCashItem.insert( pItemTemplet->m_ItemID );
			}
			//}}

			//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
			int iSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
			{
				int iItemType = static_cast<int>( pItemTemplet->m_ItemType );

				if( SiCXSLSocketItem()->CheckCashItemSocketID( iItemType, iSocketOption ) == true )
				{
					kItemInfo.m_vecItemSocket.push_back( iSocketOption );
				}
			}

			// ��Ű�� ��ǰ�� ���ӵ� �����۵��� �ߺ��Ǵ°� ���°��� ������.
			mapItem.insert( std::make_pair( iItemID, kItemInfo ) );
		}
	}
	//////////////////////////////////////////////////////////////////////////	
	// ���� ��ǰ
	else
	{
		const int iItemID = _wtoi( kPacket_.m_wstrProductID.c_str() );
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø��� ���� ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}

		// 20080416. ������� ���� ���������� �������� ä�������. �ڵ� ������ �� �� ���� ���.

		// ĳ�� ������ �������� �α�
		KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iItemID = iItemID;
		kPacketNot.m_sPeriod = kPacket_.m_usProductExpire;
		kPacketNot.m_iQuantity = kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
		kPacketNot.m_ucLevel = GetLevel();
		kPacketNot.m_cUnitClass = GetUnitClass();
		SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );

		//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
		if( ( SiKGSBingoEventInfo()->IsBingoPresent( kPacket_.m_ulProductNo ) != true ) &&
			( m_kGSBingoEvent.CheckAndRemoveIsPresentedItem( kPacket_.m_ulOrderNo ) != true )
			)
		{
			KNXBTProductInfo kProductInfo;
			if( SiKNexonBillingTCPManager()->GetProductInfo( kPacket_.m_ulProductNo, kProductInfo ) == true )
			{
				START_LOG( clog, L"��ǰ ���� ����" )
					<< BUILD_LOG( kPacket_.m_ulProductNo )
					<< BUILD_LOG( kPacket_.m_wstrProductName )
					<< BUILD_LOG( kPacket_.m_wstrProductID )
					<< BUILD_LOG( kProductInfo.m_usProductPieces )
					<< BUILD_LOG( kPacket_.m_usProductPieces )
					<< BUILD_LOG( kPacket_.m_usOrderQuantity )
					<< END_LOG;

				m_kGSBingoEvent.AddBalance( kProductInfo.m_ulSalePrice );
				BingoEventDBWrite();
			}
			else
			{
				START_LOG( clog, L"��Ű�� ��ǰ ���� ������ ����." )
					<< BUILD_LOG( kPacket_.m_ulProductNo )
					<< BUILD_LOG( kPacket_.m_wstrProductName )
					<< BUILD_LOG( kPacket_.m_wstrProductID )
					<< END_LOG;
			}
		}
#endif SERV_EVENT_BINGO
		//}}

		switch( pItemTemplet->m_ItemID )
		{
		case CXSLItem::CI_RESURRECTION_STONE: // ��Ȱ��
			{
				// ��Ȱ���̸� �ٷ� ä���ش�.
				m_iNumResurrectionStone += kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );

				// �ǽð� ��Ȱ��
				if( m_kUserCashInventory.IsCashResurrectionStone() )
				{
					KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ kPacketReq;
					kPacketReq.m_iUnitUID		= m_kUserCashInventory.GetCashRStoneUnitUID();
					kPacketReq.m_iUsedByUnitUID = GetCharUID();
					SendToCnRoom( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, kPacketReq );
				}
				return;
			}
			break;

		case CXSLItem::CI_EXPAND_INVENTORY: // ����ǰ Ȯ��
			{
				//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_EQUIP ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_ACCESSORY ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_MATERIAL ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_SPECIAL ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUEST ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUICK_SLOT ) ) ) );
				//}}
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;

#ifdef SERV_REFORM_INVENTORY_INT_CASH_ITEM
		case CXSLItem::CI_EXPAND_INVENTORY_EQUIP: // ����ǰ Ȯ��( ��� )
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_EQUIP ) ) ) );

				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_ACCESSORY: // ����ǰ Ȯ��( �Ǽ��縮 )
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_ACCESSORY ) ) ) );

				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_QUICK_SLOT: // ����ǰ Ȯ��( �Һ� )
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUICK_SLOT ) ) ) );

				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_MATERIAL: // ����ǰ Ȯ��( ��� )
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_MATERIAL ) ) ) );

				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_QUEST: // ����ǰ Ȯ��( ����Ʈ )
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUEST ) ) ) );

				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_SPECIAL: // ����ǰ Ȯ��( Ư�� )
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_SPECIAL ) ) ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
#endif  SERV_REFORM_INVENTORY_INT_CASH_ITEM
			//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
		case CXSLItem::CI_EXPAND_QUICK_SLOT:
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;

		case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
		case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
#endif	// SERV_CASH_ITEM_FOR_ELESIS
			//}}

		case CXSLItem::CI_EXPAND_SKILL_SLOT: // ��ų ���� ü���� �޴�
			{
				KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
				kPacket.m_iUnitUID		= GetCharUID();
				kPacket.m_iPeriodExpire = kPacket_.m_usProductExpire;
#ifdef SERV_SKILL_PAGE_SYSTEM
				kPacket.m_usTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM
				//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
				kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
				//}}
				SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;

		case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
			//{{ 2012. 12. 19	������	�ƶ� ���� ĳ����
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
			//}}
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#ifdef SERV_CASH_ITEM_FOR_ADD
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ADD:
#endif	// SERV_CASH_ITEM_FOR_ADD

			{
				KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
				kPacket.m_iUnitUID		= GetCharUID();
				kPacket.m_iPeriodExpire = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
				kPacket.m_usTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM
				//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
				kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
				//}}
				SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;

		case CXSLItem::CI_CASH_SKILL_POINT_60_15:
		case CXSLItem::CI_CASH_SKILL_POINT_60_30:
		case CXSLItem::CI_CASH_SKILL_POINT_30_15:
		case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
		case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
		case CXSLItem::CI_CASH_SKILL_POINT_30_7:
		case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
		case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
		case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
		case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
		case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
		case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
		case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
		case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
		case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
		case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
		case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
		case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
		case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
		case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
			{
				const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( pItemTemplet->m_ItemID );

				if( iCSPoint <= 0 )
				{
					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_RESET_SKILL_01;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					return;
				}

				if( true == m_kSkillTree.IsCashSkillPointExpired() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID	= GetCharUID();
					kPacket.m_iCSPoint	= iCSPoint;
					kPacket.m_iPeriod	= kPacket_.m_usProductExpire;		
					kPacket.m_bUpdateEndDateOnly = false;
					//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
					kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
					//}}
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					if( iCSPoint == m_kSkillTree.GetMaxCSPoint() )
					{
						KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
						kPacket.m_iUnitUID = GetCharUID();
						kPacket.m_iCSPoint = iCSPoint;
						kPacket.m_iPeriod = kPacket_.m_usProductExpire;
						kPacket.m_bUpdateEndDateOnly = true;
						//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
						kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
						//}}
						SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
					}
					else
					{
						START_LOG( cerr, L"���� ����ϰ� �ִ� ĳ�ý�ų����Ʈ�� �ٸ� ������ �Ⱓ���� �Ϸ�����!!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( pItemTemplet->m_ItemID )
							<< BUILD_LOG( kPacket_.m_usProductExpire )
							<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
							<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
							<< END_LOG;

						KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
						SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
						return;
					}
				}

				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;

			//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
		case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
		case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
			{
				KDBE_EXPAND_CHAR_SLOT_REQ kPacket;
				kPacket.m_iExpandSlotSize = ( pItemTemplet->m_ItemID == CXSLItem::CI_EXPAND_CHAR_SLOT_1 ? 1 : 2 );
				kPacket.m_iCharSlotMax = CXSLUnit::CHAR_SLOT_MAX;
				SendToGameDB( DBE_EXPAND_CHAR_SLOT_REQ, kPacket );
				return;
			}
			break;
			//}}

			//{{ 2009. 8. 7  ������		����
		case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
		case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
		case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
		case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
		case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
			{
				// ���� Ȯ���Ϸ� ����!
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;

		case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
			{
				// ���� Ȯ���Ϸ� ����!
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, ( CXSLInventory::SLOT_COUNT_ONE_LINE * 5 ) ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
			//}}

			//{{ 2010. 04. 09  ������	����� ��Ʈ
		case CXSLItem::CI_CASH_SKILL_NOTE_ITEM:
			{
				char cSkillNoteMaxPageNum = 0;
				if( m_kSkillTree.GetExpandSkillNotePage( GetLevel(), cSkillNoteMaxPageNum ) == true )
				{
					// �ϴ� ����� ��Ʈ ������ ���� ��������!
					m_kSkillTree.UpdateSkillNoteMaxPageNum( cSkillNoteMaxPageNum );

					// ����� ��Ʈ�� ȹ�� �Ϸ� ����!
					KDBE_EXPAND_SKILL_NOTE_PAGE_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = GetCharUID();
					kPacketToDB.m_cExpandedMaxPageNum = cSkillNoteMaxPageNum;
					kPacketToDB.m_cRollBackMaxPageNum = 0;
					SendToGameDB( DBE_EXPAND_SKILL_NOTE_PAGE_REQ, kPacketToDB );
					return;
				}
				else
				{
					START_LOG( cerr, L"����� ��Ʈ�� �����Ϸ��� �ϴµ� ������ �� ��⸦ �����Ͽ����ϴ�." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetLevel() )
						<< END_LOG;
				}
			}
			break;
			//}}

#ifdef SERV_SHARING_BANK_QUEST_CASH
		case CXSLItem::CI_CASH_SHARING_BACK_OPEN:
			{
				// ĳ���� ���� ����~
				KDBE_SHARING_BACK_OPEN_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_iUserUID = GetUID();
				kPacketToDB.m_iOpenType = KDBE_SHARING_BACK_OPEN_REQ::SBOT_CASH;
				SendToGameDB( DBE_SHARING_BACK_OPEN_REQ, kPacketToDB );

				return;
			}
			break;
#endif SERV_SHARING_BANK_QUEST_CASH
			//{{ 2011. 04. 14	������	�븮 ����
		case CXSLItem::CI_PERSONAL_SHOP_AGENCY:
			{
				KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_sAgencyPeriod = kPacket_.m_usProductExpire;
				kPacketToDB.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
                kPacketToDB.m_cShopType = SEnum::AST_PREMIUM;
#else //SERV_UPGRADE_TRADE_SYSTEM
				kPacketToDB.m_cShopType = CXSLSquareUnit::PST_PREMIUM;
#endif //SERV_UPGRADE_TRADE_SYSTEM

				SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
				return;
			}
			break;
			//}}

#ifdef SERV_GUILD_CHANGE_NAME
			//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
		case CXSLItem::CI_GUILD_NAME_CHANGE:
			{
				KELG_CHANGE_GUILD_NAME_REQ kPacket;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_iGuildUID = GetGuildUID();
				kPacket.m_wstrOldGuildName = m_kUserGuildManager.GetGuildName();
				// m_wstrNewGuildName�� LoginServer �ʿ��� ���� ���̴�.
				SendToLoginServer( ELG_CHANGE_GUILD_NAME_REQ, kPacket );
				return;
			}
			break;
#endif SERV_GUILD_CHANGE_NAME

			//{{ 2013. 2. 28	�ڼ���	 ���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
		case CXSLItem::CI_BINGO_MIX_ITEM:
			{
				m_kGSBingoEvent.AddMixChance( kPacket_.m_usProductPieces );
				BingoEventDBWrite();

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
			break;

		case CXSLItem::CI_BINGO_RESET_ITEM:
			{
				m_kGSBingoEvent.AddResetChance( kPacket_.m_usProductPieces );
				BingoEventDBWrite();

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
			break;
#endif SERV_EVENT_BINGO
			//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
		case CXSLItem::CI_EXPAND_SKILL_PAGE:
			{
				SendExpandSkillPageReqToGameDB( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK, kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity );
				return;
			}
			break;
#endif	// SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_ADD_WARP_BUTTON
		case CXSLItem::CI_WARP_VIP_ITEM:
#ifdef SERV_VIP_SYSTEM
		case CXSLItem::SI_USE_FREE_BY_FIELD_VIP:
#endif SERV_VIP_SYSTEM
			{
				KDBE_INSERT_WARP_VIP_REQ kPacket;
				kPacket.m_iUnitUID	= GetCharUID();
				kPacket.m_iPeriod	= 30;		
				SendToGameDB( DBE_INSERT_WARP_VIP_REQ, kPacket );

				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;
#endif SERV_ADD_WARP_BUTTON

		default:
			//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			if( SiKDailyGiftBoxManager()->IsDailyGiftBoxItemID( pItemTemplet->m_ItemID ) == true )
			{
				CTime tCurrentTime = CTime::GetCurrentTime();

				// ���ϸ��� ���� ���� ��Ű�� ȹ�� ������ ����Ϸ� ����!
				KDBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT kPacketToDB;
				kPacketToDB.m_iUserUID		= GetUID();
				kPacketToDB.m_iUnitUID		= GetCharUID();
				kPacketToDB.m_iItemID		= pItemTemplet->m_ItemID;
				kPacketToDB.m_iState		= KDailyGiftBoxManager::PURCHASED_CONFIRM;
				kPacketToDB.m_wstrRegDate	= tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
				SendToAccountDB( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, kPacketToDB );

				std::pair<std::multimap<int, KDailyGiftBoxInfo>::iterator, std::multimap<int, KDailyGiftBoxInfo>::iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( pItemTemplet->m_ItemID );
				for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
				{
					if( pairFindResult.first->second.m_iUnitUID == GetCharUID() )
					{
						if( pairFindResult.first->second.m_iState == KDailyGiftBoxManager::PURCHASED_BUY )
						{
							pairFindResult.first->second.m_iState		= KDailyGiftBoxManager::PURCHASED_CONFIRM;
							pairFindResult.first->second.m_wstrRegDate	= kPacketToDB.m_wstrRegDate;
							break;
						}
					}
					else if( pairFindResult.first->second.m_iState == KDailyGiftBoxManager::PURCHASED_BUY )
					{
						// �ٸ� ĳ���Ͱ� ���Ÿ� �Ѱ��̴�. �� ĳ������ ������ �ʱ�ȭ ����!
						kPacketToDB.m_iUnitUID	= pairFindResult.first->second.m_iUnitUID;
						kPacketToDB.m_iState	= KDailyGiftBoxManager::PURCHASED_NOT;
						SendToAccountDB( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, kPacketToDB );

						// ���� ������ �ִ� ���������� �ʱ�ȭ ����!
						pairFindResult.first->second.m_iState		= KDailyGiftBoxManager::PURCHASED_NOT;
						pairFindResult.first->second.m_wstrRegDate	= kPacketToDB.m_wstrRegDate;

						// �ش��ϴ� ItemID�� ���� ������ �̹� ������ �ִ��� �˻��ϱ� ���� �ӽ÷� ����� ������ �̸��� ���� ����
						std::pair<std::multimap<int, KDailyGiftBoxInfo>::iterator, std::multimap<int, KDailyGiftBoxInfo>::iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( pItemTemplet->m_ItemID );
						for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
						{
							if( pairFindResult.first->second.m_iUnitUID == GetCharUID() )
								break;
						}

						if( pairFindResult.first != pairFindResult.second )
						{
							pairFindResult.first->second.m_iState = KDailyGiftBoxManager::PURCHASED_CONFIRM;
						}
						else
						{
							// ���� ������ �ִ� ������ ȹ�� ������ �����Ͽ� ����.
							KDailyGiftBoxInfo kDailyGiftBoxInfo;
							kDailyGiftBoxInfo.m_iUnitUID	= GetCharUID();
							kDailyGiftBoxInfo.m_iState		= KDailyGiftBoxManager::PURCHASED_CONFIRM;
							kDailyGiftBoxInfo.m_wstrRegDate = kPacketToDB.m_wstrRegDate;
							m_mmapDailyGiftBoxList.insert( std::multimap<int, KDailyGiftBoxInfo>::value_type( pItemTemplet->m_ItemID, kDailyGiftBoxInfo ) );
						}
						break;
					}
				}

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK						= NetError::NET_OK;
				kPacket.m_iNumResurrectionStone		= m_iNumResurrectionStone;
				kPacket.m_iExceptionProcessItemID	= pItemTemplet->m_ItemID;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}

			{
				//{{ 2009. 9. 22  ������	����ĳ��
				if( CXSLItem::IsJobChangeCashItem( iItemID ) )
				{
					// ���� �Ϸ� ����!
					KDBE_CHANGE_UNIT_CLASS_REQ kPacket;
					kPacket.m_usEventID = EGS_BUY_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_cUnitClass = CXSLItem::GetCashItemChangeUnitClass( pItemTemplet->m_ItemID );

#ifdef SERV_SKILL_PAGE_SYSTEM
					kPacket.m_iTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM
					
					int iDefaultSkill[6] = {0,};
					if( SiCXSLSkillTree()->GetUnitClassDefaultSkill( kPacket.m_cUnitClass, iDefaultSkill[0], iDefaultSkill[1], iDefaultSkill[2], iDefaultSkill[3], iDefaultSkill[4], iDefaultSkill[5] ) == false )
					{
						START_LOG( cerr, L"������ �����Ϸ��� �⺻ ��ų�� �̻���" )
							<< BUILD_LOG( kPacket.m_iUnitUID )
							<< BUILD_LOGc( kPacket.m_cUnitClass )
							<< END_LOG;
					}

					if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket.m_cUnitClass) ) == true )
					{
						kPacket.m_iNewDefaultSkill1 = iDefaultSkill[0];
						kPacket.m_iNewDefaultSkill2 = iDefaultSkill[1];
					}
					else if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket.m_cUnitClass) ) == true )
					{
						kPacket.m_iNewDefaultSkill1 = iDefaultSkill[2];
						kPacket.m_iNewDefaultSkill2 = iDefaultSkill[3];
					}
					else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket.m_cUnitClass) ) == true )
					{
						kPacket.m_iNewDefaultSkill1 = iDefaultSkill[4];
						kPacket.m_iNewDefaultSkill2 = iDefaultSkill[5];
					}

					SendToGameDB( DBE_CHANGE_UNIT_CLASS_REQ, kPacket );
					return;
				}
				//}}
				//{{ 2011. 11. 21  ��μ�	���� ���� ������
				int iUnitClass = 0;
				if( CXSLItem::GetClassChangeCashItem( iItemID ) != CXSLUnit::UC_NONE )
				{
					KDBE_BUY_UNIT_CLASS_CHANGE_REQ kPacketToDB;

					if( GetUnitClassChangeInfo( iItemID, kPacketToDB ) == true )
					{
						SendToGameDB( DBE_BUY_UNIT_CLASS_CHANGE_REQ, kPacketToDB );
					}
					else
					{
						START_LOG( cerr, L"ĳ�� ������ �Ⱦ� �������� ���а� ����! ���� �ؾ� �� �� �ѵ�!!!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( iItemID )
							<< END_LOG;
					}
					return;
				}
				//}}

				//{{ 2009. 8. 4  ������		���ν�ų
				std::vector<int> vecSkillID;
				if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, GetUnitClass(), vecSkillID ) == true )
				{
					BOOST_TEST_FOREACH( int, iSkillID, vecSkillID )
					{
						KDBE_UNSEAL_SKILL_REQ kPacketToDB;
						kPacketToDB.m_iUnitUID = GetCharUID();
						kPacketToDB.m_iSkillID = iSkillID;
						SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );
					}
					return;
				}
				//}}
			}			
		}

		KItemInfo kItemInfo;
		kItemInfo.m_iItemID		= iItemID;
		kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
		kItemInfo.m_iQuantity	= kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
		kItemInfo.m_sEndurance	= ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
		kItemInfo.m_sPeriod		= kPacket_.m_usProductExpire;
		//{{ 2009. 8. 27  ������	�к�
		if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) == true )
		{
			setSealCashItem.insert( pItemTemplet->m_ItemID );
		}
		//}}
		//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
		int iSocketOption = 0;
		if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
		{
			int iItemType = static_cast<int>( pItemTemplet->m_ItemType );

			if( SiCXSLSocketItem()->CheckCashItemSocketID( iItemType, iSocketOption ) == true )
			{
				kItemInfo.m_vecItemSocket.push_back( iSocketOption );
			}
		}
		//}} 
		mapItem.insert( std::make_pair( iItemID, kItemInfo ) );
	}

	// �κ��丮�� ������ ���� �غ�
	KDBE_INSERT_ITEM_REQ kPacketReq;
	//{{ 2010. 9. 8	������	������ ȹ�� ����
	kPacketReq.m_cGetItemReason = SEnum::GIR_BUY_CASH_ITEM;

	m_kInventory.PrepareInsert( mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		//{{ 2009. 9. 2  ������		����
		kPacketReq.m_setSealCashItem = setSealCashItem;
		//}}
		kPacketReq.m_iUnitUID = GetCharUID();
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushEnduranceChange( kPacketReq.m_kItemEnduranceUpdate );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_INSERT_PURCHASED_CASH_ITEM_REQ, kPacketReq );
	}
	else
	{
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
		kPacket.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
	}

	// ĳ�� ������ ���
	std::map< int, int >::const_iterator mit;
	for( mit = kPacketReq.m_mapInsertedItem.begin(); mit != kPacketReq.m_mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_MALL, mit->second );
		//{{ 2011. 04. 13	������	������ ��� DB���
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_MALL, mit->second );
	}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_CASH_ITEM, kPacketReq.m_mapInsertedItem );
	//}}
}
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK )
{
    if( kPacket_.m_ulResult != 1 )
    {
        START_LOG( cerr, L"ĳ�� ������ ���� ���� ����." )
            << BUILD_LOG( GetUID() )
            << BUILD_LOG( GetName() )
            << BUILD_LOG( kPacket_.m_ulResult )
            << END_LOG;

		//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
		if( m_kUserGuildManager.CheckGuildNameChanging() == true )
		{
			KELG_CHANGE_GUILD_NAME_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
			kPacket.m_iUnitUID = GetCharUID();
			SendToLoginServer( DBE_CHANGE_GUILD_NAME_ACK, kPacket );
			return;
		}
#endif SERV_GUILD_CHANGE_NAME
		//}}

        KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
        SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
        return;
    }
	
	std::map< int, KItemInfo > mapItem; // �κ��丮�� �� �����۵�
	//{{ 2009. 9. 2  ������		�к�
	std::set< int > setSealCashItem;
	//}}

	//////////////////////////////////////////////////////////////////////////	
	// ��Ű�� ��ǰ
	if( kPacket_.m_byteProductKind == 1 )
	{
		if( kPacket_.m_vecNXBTSubProductInfo.empty() )
		{
			START_LOG( cerr, L"��Ű�� ��ǰ�ε� ��ǰ������ ����?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< END_LOG;			
		}
		
		int iPackageItemID = _wtoi( kPacket_.m_wstrProductID.c_str() );
		if( iPackageItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< BUILD_LOG( iPackageItemID )
				<< END_LOG;

			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}

		// ��Ű�� ĳ�� ������ �������� �α�
		KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iItemID = iPackageItemID;
		kPacketNot.m_sPeriod = kPacket_.m_usProductExpire;
		kPacketNot.m_iQuantity = kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
		kPacketNot.m_ucLevel = GetLevel();
		kPacketNot.m_cUnitClass = GetUnitClass();
		SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );
		
		std::vector< KNXBTSubProductInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecNXBTSubProductInfo.begin(); vit != kPacket_.m_vecNXBTSubProductInfo.end(); ++vit )
		{
			const KNXBTSubProductInfo& subProductInfo = *vit;
			
			int iItemID = _wtoi( subProductInfo.m_wstrProductID.c_str() );
			if( iItemID <= 0 )
			{
				START_LOG( cerr, L"������ ID ��ȯ ����." )
					<< BUILD_LOG( kPacket_.m_ulProductNo )
					<< BUILD_LOG( iItemID )
					<< END_LOG;

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ���ø��� ���� ����." )
					<< BUILD_LOG( iItemID )
					<< END_LOG;

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}

			// ĳ�� ������ �������� �α�
			KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_iItemID = iItemID;
			//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
			kPacketNot.m_sPeriod = subProductInfo.GetProductExpire();
			//}}
			kPacketNot.m_iQuantity = subProductInfo.m_usProductPieces * kPacket_.m_usOrderQuantity;
			kPacketNot.m_ucLevel = GetLevel();
			kPacketNot.m_cUnitClass = GetUnitClass();
			SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );
			
			//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			// ���� �̺�Ʈ ���� ������, ���� ���� �������� ��ȸ�� ���� ������ �ʴ´�.
			if( ( SiKGSBingoEventInfo()->IsBingoPresent( subProductInfo.m_ulProductNo ) != true ) &&
				( m_kGSBingoEvent.CheckAndRemoveIsPresentedItem( kPacket_.m_ulOrderNo ) != true )
				)
			{
				KNXBTProductInfo kProductInfo;
				if( SiKNexonBillingTCPManager()->GetProductInfo( subProductInfo.m_ulProductNo, kProductInfo ) == true )
				{
					START_LOG( clog, L"��ǰ ���� ����" )
						<< BUILD_LOG( subProductInfo.m_ulProductNo )
						<< BUILD_LOG( subProductInfo.m_wstrProductName )
						<< BUILD_LOG( subProductInfo.m_wstrProductID )
						<< BUILD_LOG( subProductInfo.m_usProductPieces )
						<< BUILD_LOG( kProductInfo.m_usProductPieces )
						<< BUILD_LOG( kPacket_.m_usProductPieces )
						<< BUILD_LOG( kPacket_.m_usOrderQuantity )
						<< END_LOG;

					SiKGSBingoEventInfo()->GetPackageItemPrice( subProductInfo.m_ulProductNo, kProductInfo.m_ulSalePrice );
					m_kGSBingoEvent.AddBalance( kProductInfo.m_ulSalePrice );
					BingoEventDBWrite();
				}
				else
				{
					START_LOG( clog, L"��Ű�� ��ǰ ���� ������ ����." )
						<< BUILD_LOG( subProductInfo.m_ulProductNo )
						<< BUILD_LOG( subProductInfo.m_wstrProductName )
						<< BUILD_LOG( subProductInfo.m_wstrProductID )
						<< END_LOG;
				}
			}
#endif SERV_EVENT_BINGO
			//}}

			switch( pItemTemplet->m_ItemID )
			{
			case CXSLItem::CI_RESURRECTION_STONE: // ��Ȱ��
				{
					// ��Ȱ���̸� �ٷ� ä���ش�.
					m_iNumResurrectionStone += kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;

					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::NET_OK;
					kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					continue;
				}
				break;

			case CXSLItem::CI_EXPAND_INVENTORY: // ����ǰ Ȯ��
				{
					//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
#ifdef SERV_REFORM_INVENTORY_TEST
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_EQUIP ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_ACCESSORY ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_MATERIAL ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_SPECIAL ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUEST ) ) ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUICK_SLOT ) ) ) );
#else
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usOrderQuantity;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot ) );
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot ) );
#endif SERV_REFORM_INVENTORY_TEST
					//}}
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;

				//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
			case CXSLItem::CI_EXPAND_QUICK_SLOT:
				{
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;

			case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
				{
					// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
					int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;
#endif SERV_EXPAND_QUICK_SLOT
				//}}
	
			case CXSLItem::CI_EXPAND_SKILL_SLOT: // ��ų ���� ü���� �޴�
				{
					KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
					kPacket.m_iUnitUID		= GetCharUID();
					kPacket.m_iPeriodExpire = kPacket_.m_usProductExpire;
					//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
					kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
#endif SERV_ADD_PACKAGE_PRODUCT
					//}}
					SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
					continue; // ������ �������� �ʰ� �ٷ� ����
				}
				break;

			case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
				//{{ 2012. 12. 19	������	�ƶ� ���� ĳ����
#ifdef SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
#endif SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
				//}}
				{
					KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
					kPacket.m_iUnitUID		= GetCharUID();
					kPacket.m_iPeriodExpire = 0;
					//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
					kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
#endif SERV_ADD_PACKAGE_PRODUCT
					//}}
					SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
					continue; // ������ �������� �ʰ� �ٷ� ����
				} break;

			case CXSLItem::CI_CASH_SKILL_POINT_5:
			case CXSLItem::CI_CASH_SKILL_POINT_10:
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013// �۾���¥: 2013-06-25	// �ڼ���
			case CXSLItem::CI_CASH_SKILL_POINT_60_15:
			case CXSLItem::CI_CASH_SKILL_POINT_60_30:
			case CXSLItem::CI_CASH_SKILL_POINT_30_15:
			case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#endif // SERV_UPGRADE_SKILL_SYSTEM_2013
				{
					const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( pItemTemplet->m_ItemID );

					if( iCSPoint <= 0 )
					{
						KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_RESET_SKILL_01;
						SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
						return;
					}

					if( true == m_kSkillTree.IsCashSkillPointExpired() )
					{
						KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
						kPacket.m_iUnitUID	= GetCharUID();
						kPacket.m_iCSPoint	= iCSPoint;
						//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
						kPacket.m_iPeriod	= subProductInfo.GetProductExpire();
						//}}
						kPacket.m_bUpdateEndDateOnly = false;
						//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
						kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
						//}}
						SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
					}
					else
					{
						if( iCSPoint == m_kSkillTree.GetMaxCSPoint() )
						{
							KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
							kPacket.m_iUnitUID = GetCharUID();
							kPacket.m_iCSPoint = iCSPoint;
							//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
							kPacket.m_iPeriod = subProductInfo.GetProductExpire();
							//}}
							kPacket.m_bUpdateEndDateOnly = true;
							//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
							kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
							//}}
							SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
						}
						else
						{
							START_LOG( cerr, L"���� ����ϰ� �ִ� ĳ�ý�ų����Ʈ�� �ٸ� ������ �Ⱓ���� �Ϸ�����!!" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( pItemTemplet->m_ItemID )
								//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
								<< BUILD_LOG( subProductInfo.GetProductExpire() )
								//}}
								<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
								<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
								<< END_LOG;

							KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
							kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
							SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
							return;
						}
					}

					return; // ������ �������� �ʰ� �ٷ� ����
				} break;

				//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
			case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
			case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
				{
                    KDBE_EXPAND_CHAR_SLOT_REQ kPacket;
					kPacket.m_iExpandSlotSize = ( pItemTemplet->m_ItemID == CXSLItem::CI_EXPAND_CHAR_SLOT_1 ? 1 : 2 );
					kPacket.m_iCharSlotMax = CXSLUnit::CHAR_SLOT_MAX;
					SendToGameDB( DBE_EXPAND_CHAR_SLOT_REQ, kPacket );
					continue;
				}
				break;
				//}}

				//{{ 2009. 8. 7  ������		����
			case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
			case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
			case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
			case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
			case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
				{
					// ���� Ȯ���Ϸ� ����!
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;

			case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
				{
					// ���� Ȯ���Ϸ� ����!
					KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
					kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, ( CXSLInventory::SLOT_COUNT_ONE_LINE * 5 ) ) );
					SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
					continue;
				}
				break;
				//}}

				//{{ 2013. 2. 28	�ڼ���	 ���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			case CXSLItem::CI_BINGO_MIX_ITEM:
				{
					m_kGSBingoEvent.AddMixChance( kPacket_.m_usProductPieces );
					BingoEventDBWrite();
					continue;
				}
				break;

			case CXSLItem::CI_BINGO_RESET_ITEM:
				{
					m_kGSBingoEvent.AddResetChance( kPacket_.m_usProductPieces );
					BingoEventDBWrite();
					continue;
				}
				break;
#endif SERV_EVENT_BINGO
				//}}

			default:
				{
					//{{ 2009. 8. 4  ������		���ν�ų
					int iSkillID = 0;
					char cUnitClass = 0;
					if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, iSkillID, cUnitClass ) )
					{
						KDBE_UNSEAL_SKILL_REQ kPacketToDB;
						kPacketToDB.m_iUnitUID = GetCharUID();
						kPacketToDB.m_iSkillID = iSkillID;
						SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );
						continue;
					}
				}
			}

			KItemInfo kItemInfo;
			kItemInfo.m_iItemID		= iItemID;
			kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
			kItemInfo.m_iQuantity	= subProductInfo.m_usProductPieces * kPacket_.m_usOrderQuantity; // ��ǰ �⺻ ���� * ���� ����
			kItemInfo.m_sEndurance	= ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
			//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
			kItemInfo.m_sPeriod		= subProductInfo.GetProductExpire();
			//}}
			//{{ 2009. 8. 27  ������	�к�
			if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) == true )
			{
				setSealCashItem.insert( pItemTemplet->m_ItemID );
			}
			//}}
			
			//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
			int iSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
			{
				int iItemType = static_cast<int>( pItemTemplet->m_ItemType );

				if( SiCXSLSocketItem()->CheckCashItemSocketID( iItemType, iSocketOption ) == true )
				{
					kItemInfo.m_vecItemSocket.push_back( iSocketOption );
				}
			}
#else
			//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			int iSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
			{
				kItemInfo.m_vecItemSocket.push_back( iSocketOption );
			}
#else
			short sSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, sSocketOption ) )
			{
				kItemInfo.m_vecItemSocket.push_back( sSocketOption );
			}
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}}
#endif SERV_CASH_ITEM_SOCKET_OPTION
			//}
			
			// ��Ű�� ��ǰ�� ���ӵ� �����۵��� �ߺ��Ǵ°� ���°��� ������.
			mapItem.insert( std::make_pair( iItemID, kItemInfo ) );
		}
	}
	//////////////////////////////////////////////////////////////////////////	
	// ���� ��ǰ
	else
	{
		const int iItemID = _wtoi( kPacket_.m_wstrProductID.c_str() );
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø��� ���� ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
			return;
		}

		// 20080416. ������� ���� ���������� �������� ä�������. �ڵ� ������ �� �� ���� ���.

		// ĳ�� ������ �������� �α�
		KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iItemID = iItemID;
		kPacketNot.m_sPeriod = kPacket_.m_usProductExpire;
		kPacketNot.m_iQuantity = kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
		kPacketNot.m_ucLevel = GetLevel();
		kPacketNot.m_cUnitClass = GetUnitClass();
		SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );

		//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
		if( ( SiKGSBingoEventInfo()->IsBingoPresent( kPacket_.m_ulProductNo ) != true ) &&
			( m_kGSBingoEvent.CheckAndRemoveIsPresentedItem( kPacket_.m_ulOrderNo ) != true )
			)
		{
			KNXBTProductInfo kProductInfo;
			if( SiKNexonBillingTCPManager()->GetProductInfo( kPacket_.m_ulProductNo, kProductInfo ) == true )
			{
				START_LOG( clog, L"��ǰ ���� ����" )
					<< BUILD_LOG( kPacket_.m_ulProductNo )
					<< BUILD_LOG( kPacket_.m_wstrProductName )
					<< BUILD_LOG( kPacket_.m_wstrProductID )
					<< BUILD_LOG( kProductInfo.m_usProductPieces )
					<< BUILD_LOG( kPacket_.m_usProductPieces )
					<< BUILD_LOG( kPacket_.m_usOrderQuantity )
					<< END_LOG;

				m_kGSBingoEvent.AddBalance( kProductInfo.m_ulSalePrice );
				BingoEventDBWrite();
			}
			else
			{
				START_LOG( clog, L"��Ű�� ��ǰ ���� ������ ����." )
					<< BUILD_LOG( kPacket_.m_ulProductNo )
					<< BUILD_LOG( kPacket_.m_wstrProductName )
					<< BUILD_LOG( kPacket_.m_wstrProductID )
					<< END_LOG;
			}
		}
#endif SERV_EVENT_BINGO
		//}}

		switch( pItemTemplet->m_ItemID )
		{
		case CXSLItem::CI_RESURRECTION_STONE: // ��Ȱ��
			{
				// ��Ȱ���̸� �ٷ� ä���ش�.
				m_iNumResurrectionStone += kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );

				// �ǽð� ��Ȱ��
				if( m_kUserCashInventory.IsCashResurrectionStone() )
				{
					KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ kPacketReq;
					kPacketReq.m_iUnitUID		= m_kUserCashInventory.GetCashRStoneUnitUID();
					kPacketReq.m_iUsedByUnitUID = GetCharUID();
					SendToCnRoom( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, kPacketReq );
				}
				return;
			}
			break;

		case CXSLItem::CI_EXPAND_INVENTORY: // ����ǰ Ȯ��
			{
				//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
#ifdef SERV_REFORM_INVENTORY_TEST
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_EQUIP ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_ACCESSORY ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_MATERIAL ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_SPECIAL ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUEST ) ) ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUICK_SLOT ) ) ) );
#else
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE * kPacket_.m_usOrderQuantity;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot ) );
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot ) );
#endif SERV_REFORM_INVENTORY_TEST
				//}}
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;

			//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
		case CXSLItem::CI_EXPAND_QUICK_SLOT:
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;

		case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
			{
				// ����ǰ Ȯ���̸� DB�� �̺�Ʈ�� ������.
				int nSlot = CXSLInventory::EXPAND_QUICK_SLOT;
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, nSlot ) );					
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
#endif SERV_EXPAND_QUICK_SLOT
			//}}

		case CXSLItem::CI_EXPAND_SKILL_SLOT: // ��ų ���� ü���� �޴�
			{
				KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
				kPacket.m_iUnitUID		= GetCharUID();
				kPacket.m_iPeriodExpire = kPacket_.m_usProductExpire;
				//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
				kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
#endif SERV_ADD_PACKAGE_PRODUCT
				//}}
				SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;

		case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
			//{{ 2012. 12. 19	������	�ƶ� ���� ĳ����
#ifdef SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
#endif SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
			//}}
			{
				KDBE_EXPAND_SKILL_SLOT_REQ kPacket;
				kPacket.m_iUnitUID		= GetCharUID();
				kPacket.m_iPeriodExpire = 0;
				//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
				kPacket.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
#endif SERV_ADD_PACKAGE_PRODUCT
				//}}
				SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacket );
				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;

		case CXSLItem::CI_CASH_SKILL_POINT_5:
		case CXSLItem::CI_CASH_SKILL_POINT_10:
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013// �۾���¥: 2013-06-25	// �ڼ���
		case CXSLItem::CI_CASH_SKILL_POINT_60_15:
		case CXSLItem::CI_CASH_SKILL_POINT_60_30:
		case CXSLItem::CI_CASH_SKILL_POINT_30_15:
		case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#endif // SERV_UPGRADE_SKILL_SYSTEM_2013
			{
				const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( pItemTemplet->m_ItemID );

				if( iCSPoint <= 0 )
				{
					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_RESET_SKILL_01;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					return;
				}

				if( true == m_kSkillTree.IsCashSkillPointExpired() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID	= GetCharUID();
					kPacket.m_iCSPoint	= iCSPoint;
					kPacket.m_iPeriod	= kPacket_.m_usProductExpire;		
					kPacket.m_bUpdateEndDateOnly = false;
					//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
					kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
					//}}
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					if( iCSPoint == m_kSkillTree.GetMaxCSPoint() )
					{
						KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
						kPacket.m_iUnitUID = GetCharUID();
						kPacket.m_iCSPoint = iCSPoint;
						kPacket.m_iPeriod = kPacket_.m_usProductExpire;
						kPacket.m_bUpdateEndDateOnly = true;
						//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
						kPacket.m_iSkillPointItemID = pItemTemplet->m_ItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
						//}}
						SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
					}
					else
					{
						START_LOG( cerr, L"���� ����ϰ� �ִ� ĳ�ý�ų����Ʈ�� �ٸ� ������ �Ⱓ���� �Ϸ�����!!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( pItemTemplet->m_ItemID )
							<< BUILD_LOG( kPacket_.m_usProductExpire )
							<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
							<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
							<< END_LOG;

						KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
						SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
						return;
					}
				}

				return; // ������ �������� �ʰ� �ٷ� ����
			}
			break;

			//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
		case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
		case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
			{
				KDBE_EXPAND_CHAR_SLOT_REQ kPacket;
				kPacket.m_iExpandSlotSize = ( pItemTemplet->m_ItemID == CXSLItem::CI_EXPAND_CHAR_SLOT_1 ? 1 : 2 );
				kPacket.m_iCharSlotMax = CXSLUnit::CHAR_SLOT_MAX;
				SendToGameDB( DBE_EXPAND_CHAR_SLOT_REQ, kPacket );
				return;
			}
			break;
			//}}

			//{{ 2009. 8. 7  ������		����
		case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
		case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
		case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
		case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
		case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
			{
				// ���� Ȯ���Ϸ� ����!
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;

		case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
			{
				// ���� Ȯ���Ϸ� ����!
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, ( CXSLInventory::SLOT_COUNT_ONE_LINE * 5 ) ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
				return;
			}
			break;
			//}}

			//{{ 2010. 04. 09  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
		case CXSLItem::CI_CASH_SKILL_NOTE_ITEM:
			{
				char cSkillNoteMaxPageNum = 0;
				if( m_kSkillTree.GetExpandSkillNotePage( GetLevel(), cSkillNoteMaxPageNum ) == true )
				{
					// �ϴ� ����� ��Ʈ ������ ���� ��������!
					m_kSkillTree.UpdateSkillNoteMaxPageNum( cSkillNoteMaxPageNum );

					// ����� ��Ʈ�� ȹ�� �Ϸ� ����!
					KDBE_EXPAND_SKILL_NOTE_PAGE_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = GetCharUID();
					kPacketToDB.m_cExpandedMaxPageNum = cSkillNoteMaxPageNum;
					kPacketToDB.m_cRollBackMaxPageNum = 0;
					SendToGameDB( DBE_EXPAND_SKILL_NOTE_PAGE_REQ, kPacketToDB );
					return;
				}
				else
				{
					START_LOG( cerr, L"����� ��Ʈ�� �����Ϸ��� �ϴµ� ������ �� ��⸦ �����Ͽ����ϴ�." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetLevel() )
						<< END_LOG;
				}
			}
			break;
#endif SERV_SKILL_NOTE
			//}}

			//{{ 2011. 04. 14	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
		case CXSLItem::CI_PERSONAL_SHOP_AGENCY:
			{
                KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_sAgencyPeriod = kPacket_.m_usProductExpire;
				kPacketToDB.m_usEventID = EGS_GET_PURCHASED_CASH_ITEM_REQ;
				SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
				return;
			}
			break;
#endif SERV_PSHOP_AGENCY
			//}}

			//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
		case CXSLItem::CI_GUILD_NAME_CHANGE:
			{
				KELG_CHANGE_GUILD_NAME_REQ kPacket;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_iGuildUID = GetGuildUID();
				kPacket.m_wstrOldGuildName = m_kUserGuildManager.GetGuildName();
				// m_wstrNewGuildName�� LoginServer �ʿ��� ���� ���̴�.
				SendToLoginServer( ELG_CHANGE_GUILD_NAME_REQ, kPacket );
				return;
			}
			break;
#endif SERV_GUILD_CHANGE_NAME
			//}}

			//{{ 2013. 2. 28	�ڼ���	 ���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
		case CXSLItem::CI_BINGO_MIX_ITEM:
			{
				m_kGSBingoEvent.AddMixChance( kPacket_.m_usProductPieces );
				BingoEventDBWrite();

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
			break;

		case CXSLItem::CI_BINGO_RESET_ITEM:
			{
				m_kGSBingoEvent.AddResetChance( kPacket_.m_usProductPieces );
				BingoEventDBWrite();

				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
			break;
#endif SERV_EVENT_BINGO
			//}}
			
		default:
			//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			if( SiKDailyGiftBoxManager()->IsDailyGiftBoxItemID( pItemTemplet->m_ItemID ) == true )
			{
				CTime tCurrentTime = CTime::GetCurrentTime();

				// ���ϸ��� ���� ���� ��Ű�� ȹ�� ������ ����Ϸ� ����!
				KDBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT kPacketToDB;
				kPacketToDB.m_iUserUID		= GetUID();
				kPacketToDB.m_iUnitUID		= GetCharUID();
				kPacketToDB.m_iItemID		= pItemTemplet->m_ItemID;
				kPacketToDB.m_iState		= KDailyGiftBoxManager::PURCHASED_CONFIRM;
				kPacketToDB.m_wstrRegDate	= tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
				SendToAccountDB( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, kPacketToDB );

				std::pair<std::multimap<int, KDailyGiftBoxInfo>::iterator, std::multimap<int, KDailyGiftBoxInfo>::iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( pItemTemplet->m_ItemID );
				for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
				{
					if( pairFindResult.first->second.m_iUnitUID == GetCharUID() )
					{
						if( pairFindResult.first->second.m_iState == KDailyGiftBoxManager::PURCHASED_BUY )
						{
							pairFindResult.first->second.m_iState		= KDailyGiftBoxManager::PURCHASED_CONFIRM;
							pairFindResult.first->second.m_wstrRegDate	= kPacketToDB.m_wstrRegDate;
							break;
						}
					}
					else if( pairFindResult.first->second.m_iState == KDailyGiftBoxManager::PURCHASED_BUY )
					{
						// �ٸ� ĳ���Ͱ� ���Ÿ� �Ѱ��̴�. �� ĳ������ ������ �ʱ�ȭ ����!
						kPacketToDB.m_iUnitUID	= pairFindResult.first->second.m_iUnitUID;
						kPacketToDB.m_iState	= KDailyGiftBoxManager::PURCHASED_NOT;
						SendToAccountDB( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, kPacketToDB );

						// ���� ������ �ִ� ���������� �ʱ�ȭ ����!
						pairFindResult.first->second.m_iState		= KDailyGiftBoxManager::PURCHASED_NOT;
						pairFindResult.first->second.m_wstrRegDate	= kPacketToDB.m_wstrRegDate;

						// �ش��ϴ� ItemID�� ���� ������ �̹� ������ �ִ��� �˻��ϱ� ���� �ӽ÷� ����� ������ �̸��� ���� ����
						std::pair<std::multimap<int, KDailyGiftBoxInfo>::iterator, std::multimap<int, KDailyGiftBoxInfo>::iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( pItemTemplet->m_ItemID );
						for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
						{
							if( pairFindResult.first->second.m_iUnitUID == GetCharUID() )
								break;
						}

						if( pairFindResult.first != pairFindResult.second )
						{
							pairFindResult.first->second.m_iState = KDailyGiftBoxManager::PURCHASED_CONFIRM;
						}
						else
						{
							// ���� ������ �ִ� ������ ȹ�� ������ �����Ͽ� ����.
							KDailyGiftBoxInfo kDailyGiftBoxInfo;
							kDailyGiftBoxInfo.m_iUnitUID	= GetCharUID();
							kDailyGiftBoxInfo.m_iState		= KDailyGiftBoxManager::PURCHASED_CONFIRM;
							kDailyGiftBoxInfo.m_wstrRegDate = kPacketToDB.m_wstrRegDate;
							m_mmapDailyGiftBoxList.insert( std::multimap<int, KDailyGiftBoxInfo>::value_type( pItemTemplet->m_ItemID, kDailyGiftBoxInfo ) );
						}
						break;
					}
				}
				
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK						= NetError::NET_OK;
				kPacket.m_iNumResurrectionStone		= m_iNumResurrectionStone;
				kPacket.m_iExceptionProcessItemID	= pItemTemplet->m_ItemID;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}

			{
				//{{ 2009. 9. 22  ������	����ĳ��
				if( CXSLItem::IsJobChangeCashItem( iItemID ) )
				{
					// ���� �Ϸ� ����!
					KDBE_CHANGE_UNIT_CLASS_REQ kPacket;
					kPacket.m_usEventID = EGS_BUY_CASH_ITEM_REQ;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_cUnitClass = CXSLItem::GetCashItemChangeUnitClass( pItemTemplet->m_ItemID );

					SendToGameDB( DBE_CHANGE_UNIT_CLASS_REQ, kPacket );
					return;
				}
				//}}
		//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
				int iUnitClass = 0;
				if( CXSLItem::GetClassChangeCashItem( iItemID ) != CXSLUnit::UC_NONE )
				{
					KDBE_BUY_UNIT_CLASS_CHANGE_REQ kPacketToDB;

					if( GetUnitClassChangeInfo( iItemID, kPacketToDB ) == true )
					{
						SendToGameDB( DBE_BUY_UNIT_CLASS_CHANGE_REQ, kPacketToDB );
					}
					else
					{
						START_LOG( cerr, L"ĳ�� ������ �Ⱦ� �������� ���а� ����! ���� �ؾ� �� �� �ѵ�!!!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( iItemID )
							<< END_LOG;
					}
					return;
				}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
				//}}

				//{{ 2009. 8. 4  ������		���ν�ų
				int iSkillID = 0;
				char cUnitClass = 0;
				if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, iSkillID, cUnitClass ) )
				{
					KDBE_UNSEAL_SKILL_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = GetCharUID();
					kPacketToDB.m_iSkillID = iSkillID;
					SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );
					return;
				}
			}			
		}

		KItemInfo kItemInfo;
		kItemInfo.m_iItemID		= iItemID;
		kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
		kItemInfo.m_iQuantity	= kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
		kItemInfo.m_sEndurance	= ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
		kItemInfo.m_sPeriod		= kPacket_.m_usProductExpire;
		//{{ 2009. 8. 27  ������	�к�
		if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) == true )
		{
			setSealCashItem.insert( pItemTemplet->m_ItemID );
		}
		//}}
		//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
		int iSocketOption = 0;
		if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
		{
			int iItemType = static_cast<int>( pItemTemplet->m_ItemType );

			if( SiCXSLSocketItem()->CheckCashItemSocketID( iItemType, iSocketOption ) == true )
			{
				kItemInfo.m_vecItemSocket.push_back( iSocketOption );
			}
		}
#else
		//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		int iSocketOption = 0;
		if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
		{
			kItemInfo.m_vecItemSocket.push_back( iSocketOption );
		}
#else
		short sSocketOption = 0;
		if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, sSocketOption ) )
		{
			kItemInfo.m_vecItemSocket.push_back( sSocketOption );
		}
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
#endif SERV_CASH_ITEM_SOCKET_OPTION
		//}} 
		mapItem.insert( std::make_pair( iItemID, kItemInfo ) );
	}

	// �κ��丮�� ������ ���� �غ�
	KDBE_INSERT_ITEM_REQ kPacketReq;
	//{{ 2010. 9. 8	������	������ ȹ�� ����
#ifdef SERV_GET_ITEM_REASON
	kPacketReq.m_cGetItemReason = SEnum::GIR_BUY_CASH_ITEM;
#endif SERV_GET_ITEM_REASON
	//}}
	m_kInventory.PrepareInsert( mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		//{{ 2009. 9. 2  ������		����
		kPacketReq.m_setSealCashItem = setSealCashItem;
		//}}
		kPacketReq.m_iUnitUID = GetCharUID();
        m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
        m_kInventory.FlushEnduranceChange( kPacketReq.m_kItemEnduranceUpdate );
        m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
        SendToGameDB( DBE_INSERT_PURCHASED_CASH_ITEM_REQ, kPacketReq );
    }
    else
    {
        KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
        kPacket.m_iOK = NetError::NET_OK;
        kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
        kPacket.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
        SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
    }

	// ĳ�� ������ ���
	std::map< int, int >::const_iterator mit;
	for( mit = kPacketReq.m_mapInsertedItem.begin(); mit != kPacketReq.m_mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_MALL, mit->second );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_MALL, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_CASH_ITEM, kPacketReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


IMPL_ON_FUNC( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ��Ű�� ���� ��ǰ pick up ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2013. 2. 5	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	if( m_kGSBingoEvent.IsCurrentOrderNoItem( kPacket_.m_ulOrderNo ) == false )
	{
		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BINGO_03;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_EVENT_BINGO
	//}}

	// 1. ���� ���� �����۾��� ������ �ʾҴ�.
	if( !m_kUserCashInventory.IsEmptyPickUpPackageInfo() )
	{
		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_09;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 2. ��Ű�� ��ǰ�� ItemID���
#ifdef SERV_GLOBAL_CASH_PACKAGE
	int iItemID = kPacket_.m_iProductID;
	m_veciPackageBuyCheck.clear();
#else // SERV_GLOBAL_CASH_PACKAGE
	int iItemID = _wtoi( kPacket_.m_wstrPackageProductID.c_str() );
#endif // SERV_GLOBAL_CASH_PACKAGE
	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"������ ID ��ȯ ����." )
			<< BUILD_LOG( kPacket_.m_ulPackageProductNo )
			<< BUILD_LOG( kPacket_.m_wstrPackageProductID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
		return;
	}

	switch ( GetUnitType() )
	{

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ARA
	case CXSLUnit::UT_ARA:
#endif // SERV_CAN_NOT_GET_CASH_ITEM_ARA

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ELESIS
	case CXSLUnit::UT_ELESIS:
#endif // SERV_CAN_NOT_GET_CASH_ITEM_ELESIS

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ADD
	case CXSLUnit::UT_ADD:
#endif // SERV_CAN_NOT_GET_CASH_ITEM_ADD
		{
			switch( iItemID )
			{
			case 160570:		// ���� ���� ���� ť��
			case 160571:		// ���� ���� ���� ť��
			case 160572:		// ���� ���� ���� ť��
			case 160573:		// ���� ���� �尩 ť��
			case 160574:		// ���� ���� �Ź� ť��
			case 160575:		// ���� ���� ��� ť��
				{
					KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
					SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
					return;
				}break;
			}

		} break;

	default:
		break;
	}

	//{{ 2012. 12. 14  ĳ�� �κ��丮���� �ƶ� ��� �Ұ����� ������ �������°� ���� ��� - ��μ�
#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ARA

	switch( iItemID )
	{
		case 261590:		// õ�� ������ ��ų ���� ü���� �޴� ��Ű��	
		case 261600:		// õ�� ������ ������ �����(II) ��Ű��	
		case 261610:		// õ�� ������ ������ Ȯ��� ��Ű��	
		case 261620:		// õ�� ������ �ູ ��Ű��	
		{
			if( GetUnitType() != CXSLUnit::UT_ARA )
			{
				KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
				return;
			}
		}break;
	}
#endif SERV_CAN_NOT_GET_CASH_ITEM_ARA
	//}}

#ifdef SERV_CAN_NOT_GET_CASH_ITEM_ELESIS	// ���볯¥: 2013-07-11
	if( GetUnitType() == CXSLUnit::UT_ELESIS )
	{
		switch( iItemID )
		{
		case 160570:		// ���� ���� ���� ť��
		case 160571:		// ���� ���� ���� ť��
		case 160572:		// ���� ���� ���� ť��
		case 160573:		// ���� ���� �尩 ť��
		case 160574:		// ���� ���� �Ź� ť��
		case 160575:		// ���� ���� ��� ť��
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}break;
		}
	}

	switch( iItemID )
	{
	case 261664:		//	�����ý� ���� ���� ��Ű��
	case 261665:		//	��ų ���� ü���� �޴�(�����ý� ����) ��Ű��
	case 261666:		//	������ �����(II)(�����ý� ����) ��Ű��
	case 261667:		//	������ Ȯ���(�����ý� ����) ��Ű��
		{
			if( GetUnitType() != CXSLUnit::UT_ELESIS )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
				return;
			}
		}break;
	}
#endif	// SERV_CAN_NOT_GET_CASH_ITEM_ELESIS

	std::map< int, int > mapItem; // �κ��丮 �˻��

	// 3. ��Ű�� ��ǰ���� ProductKind�� �˻��Ѵ�.
	if( kPacket_.m_byteProductKind != 1  ||  kPacket_.m_vecPickUpPackageInfo.empty() )
	{
		START_LOG( cerr, L"��Ű�� ��ǰ�� �ƴѵ� ��Ű�� ��Ŷ���� �Դ�?" )
			<< BUILD_LOG( kPacket_.m_ulPackageProductNo )
			<< BUILD_LOG( kPacket_.m_byteProductKind )
			<< END_LOG;

		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
		return;
	}
	else
	{
		std::vector< KNXBTPickUpPackageInfo >::const_iterator vitPU;
		for( vitPU = kPacket_.m_vecPickUpPackageInfo.begin(); vitPU != kPacket_.m_vecPickUpPackageInfo.end(); ++vitPU )
		{
			const KNXBTPickUpPackageInfo& kPickUpPackageInfo = *vitPU;

			// 4. ������ ��ũ��Ʈ �˻�
#ifdef SERV_GLOBAL_CASH_PACKAGE
			int iMemberItemID = kPickUpPackageInfo.m_iSubProductNo;
#else // SERV_GLOBAL_CASH_PACKAGE
			int iMemberItemID = _wtoi( kPickUpPackageInfo.m_wstrProductID.c_str() );
#endif // SERV_GLOBAL_CASH_PACKAGE

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iMemberItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ���ø��� ���� ����." )
					<< BUILD_LOG( iMemberItemID )
					<< END_LOG;

				KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
				SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 5. ���� ������ Ŭ�������� �˻��Ѵ�.
			if( !m_kInventory.CompareUnitClass( iMemberItemID ) )
			{
				KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_07;
				SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 6. ���������� �������� �κ��丮�� �����鼭 �Ⱓ���̸� ���´�.
			if( m_kInventory.IsExistWithBank( iMemberItemID ) )
			{
				// ���� ��ǰ�߿� ������ �������� ���� �� �����Ƿ� �Ʒ��� ���� ����ó����.
				//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���				
				if( kPickUpPackageInfo.GetProductExpire() > 0  &&  pItemTemplet->m_PeriodType != CXSLItem::PT_QUANTITY )
				//}}
				{
					KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_NX_SHOP_08;
					SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			
			// ĳ���� �κ������� ���������� �������� �ʴ� ĳ�þ������� ��쿡 �ߺ����ſ� ���� ���� ó��
			switch( iMemberItemID )
			{
				//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
			case CXSLItem::CI_EXPAND_SKILL_SLOT_IN_PACKAGE: // 4.1 ���������� �������� ��ų����B(�Ⱓ��) �̸�, �̹� ��ų����B(�Ⱓ��, Ȥ�� ����)�� ������̸� ������ �� ���� ���´�			
				{
					KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE eSkillSlotBExpireState = m_kSkillTree.GetSkillSlotBExpirationState();
					if( KUserSkillTree::SSBES_EXPIRED != eSkillSlotBExpireState )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_56;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}
				break;

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
#endif	// SERV_CASH_ITEM_FOR_ADD
				{
					KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE eSkillSlotBExpireState = m_kSkillTree.GetSkillSlotBExpirationState();
					if( KUserSkillTree::SSBES_PERMANENT == eSkillSlotBExpireState )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_57;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}
				break;
#endif SERV_ADD_PACKAGE_PRODUCT
				//}}

				//{{ 2011. 12. 21	������	���� Ȯ�� ��Ű�� ���� ��ǰ
#ifdef SERV_EXPAND_BANK_ITEM_IN_PACKAGE
			case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
				{
					// �Ϲ� ������� �ƴϸ� ������ �� ����!
					if( m_kInventory.GetBankMemberShip() != CXSLInventory::MPB_NORMAL )
					{
						START_LOG( cwarn, L"���� ������ ����� �ƴմϴ�." )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( GetCharName() )
							<< BUILD_LOG( m_kInventory.GetBankMemberShip() )
							<< END_LOG;

						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_36;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}

					// �κ��丮�� �縮�� �ſ뺸������ ������ ������ ȸ������ ������ �� ����!
					if( m_kInventory.IsExistWithBank( CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE ) == true )
					{
						START_LOG( cwarn, L"�縮�� �ſ뺸������ �κ��丮�� ������ ������ ȸ������ ���Ÿ���." )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( GetCharName() )
							<< END_LOG;

						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}

					KNXBTProductInfo kProductInfo;
					if( !SiKNexonBillingTCPManager()->GetProductInfo( kPickUpPackageInfo.m_ulProductNo, kProductInfo ) )
					{
						START_LOG( cerr, L"���δ�Ʈ ���� ���� ����." )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( GetCharName() )
							<< BUILD_LOG( kPickUpPackageInfo.m_ulProductNo )
							<< END_LOG;

						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}

					// �ֹ������� �Ѱ� �̻��� �ȵ�!
					//if( vit->m_usOrderQuantity > 1 )
					//{
					//	START_LOG( cerr, L"���� ȸ������ 1���̻� �ֹ��� �� ����." )
					//		<< BUILD_LOG( GetCharUID() )
					//		<< BUILD_LOG( GetCharName() )
					//		<< BUILD_LOG( vit->m_usOrderQuantity )
					//		<< END_LOG;

					//	KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
					//	kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
					//	SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
					//	return;
					//}

					// Ȯ�尡������ �˻�����!
					const int iBankSlotToExpand = CXSLInventory::SLOT_COUNT_ONE_LINE * 5; // 8ĭ�� 5��

					if( m_kInventory.IsAbleToExpandBank( iBankSlotToExpand ) == false )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_36;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}
				break;
				//}}
#endif SERV_EXPAND_BANK_ITEM_IN_PACKAGE
				//}}
				//{{ 2012. 2. 15  ��μ�	�� ���� Ȯ�� ������ - ��Ű����ǰ
#ifdef SERV_EXPAND_QUICK_SLOT
			case CXSLItem::CI_EXPAND_QUICK_SLOT:
				{
					int iInventorySlotToExpand = 1;
					if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_E_QUICK_SLOT, iInventorySlotToExpand ) )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_66;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}break;

			case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
				{
					int iInventorySlotToExpand = 1;
					if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_E_QUICK_SLOT, iInventorySlotToExpand ) )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_66;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}break;
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:	// �����ý��� ��� ������ �� ���� Ȯ�� ������
				{
					int iInventorySlotToExpand = 1;
					if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_E_QUICK_SLOT, iInventorySlotToExpand ) )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_66;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}break;
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#endif SERV_EXPAND_QUICK_SLOT
					//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
			case CXSLItem::CI_EXPAND_SKILL_PAGE:
				{
					if ( !m_kSkillTree.CanExpandSkillPage() )
					{
						KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_SKILL_PAGE_02;
						SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
						return;
					}
				}break;
#endif // SERV_SKILL_PAGE_SYSTEM
			}

			// 8. �κ��丮�� �������� �˻縦 ���� �غ�
			std::map< int, int >::iterator mitFind;
			mitFind = mapItem.find( iMemberItemID );
			if( mitFind == mapItem.end() )
			{				
				mapItem.insert( std::make_pair( iMemberItemID, kPickUpPackageInfo.m_usProductPieces ) );
			}
			else
			{
				mitFind->second += kPickUpPackageInfo.m_usProductPieces;
			}
		}
	}

	// 9. �κ��丮�� �������� �˻�
	if( !m_kInventory.IsEnoughSpaceExist( mapItem ) )
	{
		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_SHOP_02;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 10. �����ۿ� �ο��� ���Ͽɼ�
	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	if( m_kUserCashInventory.SetSocketForCash( kPacket_.m_mapSocketForCash, kPacket_.m_mapSocketGroupIDForCash ) == false )
	{
		// ���������� �ʴ� ���� ������ �����Ϸ� �մϴ�.
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SOCKET_ITEM_09;
		SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
		return;
	}
#else
	m_kUserCashInventory.SetSocketForCash( kPacket_.m_mapSocketForCash );
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}

	//////////////////////////////////////////////////////////////////////////
	m_kUserCashInventory.ClearPickUpPackageInfo();

	std::vector< KNXBTPickUpPackageInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecPickUpPackageInfo.begin(); vit != kPacket_.m_vecPickUpPackageInfo.end(); ++vit )
	{
		const KNXBTPickUpPackageInfo& kPickUpPackageInfo = *vit;

#ifdef SERV_GLOBAL_BILLING

		KEBILL_PICK_UP_REQ kPacket;
		kPacket.m_iTransNo = kPacket_.m_ulOrderNo;
		kPacket.m_iProductNo = kPacket_.m_ulPackageProductNo;
		kPacket.m_iUnitUID = GetCharUID();
		kPacket.m_mapSocketForCash =kPacket_.m_mapSocketForCash;
#ifdef SERV_GLOBAL_CASH_PACKAGE
		kPacket.m_byteProductKind = kPacket_.m_byteProductKind;
		kPacket.m_vecPickUpPackageInfo = kPacket_.m_vecPickUpPackageInfo;
		kPacket.m_iSubProductNo = kPickUpPackageInfo.m_ulProductNo;
		kPacket.m_iSubProductID = kPickUpPackageInfo.m_iSubProductNo;

		if ( std::find( m_veciPackageBuyCheck.begin(), m_veciPackageBuyCheck.end(), kPickUpPackageInfo.m_iSubProductNo ) == m_veciPackageBuyCheck.end() )
		{

			m_veciPackageBuyCheck.push_back( kPickUpPackageInfo.m_iSubProductNo );

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013	// �ؿ��� �߰� ��ȫ��
			switch( kPickUpPackageInfo.m_iSubProductNo )
			{
				case CXSLItem::CI_CASH_SKILL_POINT_60_15:
				case CXSLItem::CI_CASH_SKILL_POINT_60_30:
				case CXSLItem::CI_CASH_SKILL_POINT_30_15:
				case CXSLItem::CI_CASH_SKILL_POINT_30_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
				case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
				case CXSLItem::CI_CASH_SKILL_POINT_30_7:
				case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
				case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
				case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
				case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
				case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
				case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
				case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
				case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
				case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
				case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
				case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
				case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
				case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
				case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
				case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
				case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
				case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
				case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
				case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
				case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
					{
						if( false == m_kSkillTree.IsCashSkillPointExpired() )
						{
							const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( kPickUpPackageInfo.m_iSubProductNo );

							if( iCSPoint <= 0 )
							{
								KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_RESET_SKILL_01;
								SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
								return;
							}

							if( iCSPoint != m_kSkillTree.GetMaxCSPoint() )
							{
								KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;				// fix!! �����޼��� ����
								SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
								return;
							}
						}
					}
			}
#else
			if( CXSLItem::CI_CASH_SKILL_POINT_5 == kPickUpPackageInfo.m_iSubProductNo && (false == m_kSkillTree.IsCashSkillPointExpired() || 5 != m_kSkillTree.GetMaxCSPoint()) )
			{
				if(  (5 != m_kSkillTree.GetMaxCSPoint() && m_kSkillTree.GetMaxCSPoint() != 0) )
				{
					KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_SKILL_23;
					SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
					continue;
				}
			}
			else if(CXSLItem::CI_CASH_SKILL_POINT_10 == kPickUpPackageInfo.m_iSubProductNo)
			{
				if( (10 != m_kSkillTree.GetMaxCSPoint() && m_kSkillTree.GetMaxCSPoint() != 0) )
				{
					KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_SKILL_23;
					SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
					continue;
				}
			}
#endif SERV_UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			continue;
		}

		m_kUserCashInventory.AddPickUpPackageInfo( kPickUpPackageInfo.m_ulProductNo,  kPickUpPackageInfo );
#endif //SERV_GLOBAL_CASH_PACKAGE
		SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacket );

#else // SERV_GLOBAL_BILLING

		//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
		if( m_kGSBingoEvent.IsPresentedItem( kPacket_.m_ulOrderNo ) == true )
		{
			m_kGSBingoEvent.AddOrderNoForPresentedItem( kPacket_.m_ulOrderNo );
		}
#endif SERV_EVENT_BINGO
		//}}


		KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ kPacket;
		kPacket.m_ulPacketNo		 = SiKNexonBillingTCPManager()->GetNextPacketNo();
		kPacket.m_bytePacketType	 = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_FOR_PACKAGE;
		kPacket.m_ulOrderNo			 = kPacket_.m_ulOrderNo;
		kPacket.m_ulPackageProductNo = kPacket_.m_ulPackageProductNo;
		kPacket.m_ulProductNo		 = kPickUpPackageInfo.m_ulProductNo;

		// �ӽ� ����
		m_kUserCashInventory.AddPickUpPackageInfo( kPickUpPackageInfo.m_ulProductNo, kPickUpPackageInfo );

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ, kPacket );
		SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
#endif // SERV_GLOBAL_BILLING
	}
	//////////////////////////////////////////////////////////////////////////

	/*
	KDBE_GET_PURCHASED_CASH_ITEM_REQ kPacket;
	kPacket.m_iOrderNo			= kPacket_.m_iOrderNo;
	kPacket.m_iPackageProductNo = kPacket_.m_iPackageProductNo;
	kPacket.m_iProductNo		= kPacket_.m_iProductNo;
	kPacket.m_nQuantity			= kPacket_.m_nQuantity;
	kPacket.m_iItemID			= iItemID;

	SendToNXShopDB( DBE_GET_PURCHASED_CASH_ITEM_REQ, kPacket );
	*/
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK )
{
	KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
	KNXBTPickUpPackageInfo kPickUpPackageInfo;
	KNXBTProductInfo kMemberProductInfo;
	KDBE_INSERT_PURCHASED_CASH_PACKAGE_REQ kPacketReq;
	std::map< int, KItemInfo > mapItem; // �κ��丮�� �� �����۵�

	// 1. ���� ��� üũ
	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( cerr, L"ĳ�� ������ ���� ���� ����." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
		goto end_proc;
	}

	// 2. �ӽ� ���� ��Ű�� ���� ���
	if( !m_kUserCashInventory.GetPickUpPackageInfo( kPacket_.m_ulProductNo, kPickUpPackageInfo ) )
	{
		START_LOG( cerr, L"�ӽ� ������ ��Ű�� ������ǰ ������ �����ϴ�." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_NX_SHOP_03;
		goto end_proc;
	}

	// 3. ItemID ��ȯ
#ifdef SERV_GLOBAL_CASH_PACKAGE
	int iItemID = kPacket_.m_ulSubProductNo;
#else // SERV_GLOBAL_CASH_PACKAGE
	int iItemID = _wtoi( kPickUpPackageInfo.m_wstrProductID.c_str() );
#endif // SERV_GLOBAL_CASH_PACKAGE
	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"������ ID ��ȯ ����." )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
				
		kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
		goto end_proc;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
		goto end_proc;
	}
	
	// 4. ���� ��Ȳ ����. ��Ű�� ��ǰ�� ����� �ƴѵ� ���ԵǾ� �ִ� ��� ������ ���� ó���Ѵ�
	switch( pItemTemplet->m_ItemID )
	{
	case CXSLItem::CI_EXPAND_INVENTORY:
#ifdef SERV_GLOBAL_BILLING
	case CXSLItem::CI_EXPAND_INVENTORY_EQUIP:
	case CXSLItem::CI_EXPAND_INVENTORY_ACCESSORY:
	case CXSLItem::CI_EXPAND_INVENTORY_QUICK_SLOT:
	case CXSLItem::CI_EXPAND_INVENTORY_MATERIAL:
	case CXSLItem::CI_EXPAND_INVENTORY_QUEST:
	case CXSLItem::CI_EXPAND_INVENTORY_SPECIAL:
#endif //SERV_GLOBAL_BILLING
	case CXSLItem::CI_EXPAND_SKILL_SLOT:
		//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
#else
	case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}	
	case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
	case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013// �۾���¥: 2013-06-25	// �ڼ���
#else
#ifdef SERV_USE_GNOSIS_IN_PACKAGE
#else
	case CXSLItem::CI_CASH_SKILL_POINT_5:  
	case CXSLItem::CI_CASH_SKILL_POINT_10:
#endif //SERV_USE_GNOSIS_IN_PACKAGE

#endif // SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
	case CXSLItem::CI_EXPAND_SKILL_PAGE:
#endif // SERV_SKILL_PAGE_SYSTEM
		{
			kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
			goto end_proc;
		}
		break;

		//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
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
#endif	// SERV_CASH_ITEM_FOR_ADD

		{
			if( kPickUpPackageInfo.GetProductExpire() != 0 )
			{
				kPacket.m_iOK = NetError::ERR_NX_SHOP_01;
				goto end_proc;
			}
		}
		break;
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
	}



	// 20080416. ������� ���� ���������� �������� ä�������. �ڵ� ������ �� �� ���� ���.

	//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	if( ( SiKGSBingoEventInfo()->IsBingoPresent( kPacket_.m_ulProductNo ) != true ) &&
		( m_kGSBingoEvent.CheckAndRemoveIsPresentedItem( kPacket_.m_ulOrderNo ) != true )
		)
	{
		KNXBTProductInfo kProductInfo;
		if( SiKNexonBillingTCPManager()->GetProductInfo( kPacket_.m_ulProductNo, kProductInfo ) == true )
		{
			START_LOG( clog, L"��ǰ ���� ����" )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< BUILD_LOG( kPacket_.m_ulPackageProductNo )
				<< BUILD_LOG( kProductInfo.m_wstrProductID )
				<< BUILD_LOG( kProductInfo.m_usProductPieces )
				<< END_LOG;

			SiKGSBingoEventInfo()->GetPackageItemPrice( kProductInfo.m_ulProductNo, kProductInfo.m_ulSalePrice );
			m_kGSBingoEvent.AddBalance( kProductInfo.m_ulSalePrice );
			BingoEventDBWrite();
		}
		else
		{
			START_LOG( clog, L"��Ű�� ��ǰ ���� ������ ����." )
				<< BUILD_LOG( kPacket_.m_ulProductNo )
				<< BUILD_LOG( kPickUpPackageInfo.m_wstrProductID )
				<< END_LOG;
		}
	}
#endif SERV_EVENT_BINGO
	//}}

	// 5. ĳ�� ������ �������� �α�
	{
		KELOG_GET_PURCHASED_CASH_ITEM_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iItemID = iItemID;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
		kPacketNot.m_sPeriod = kPickUpPackageInfo.GetProductExpire();
		//}}
		kPacketNot.m_iQuantity = kPickUpPackageInfo.m_usProductPieces;
		kPacketNot.m_ucLevel = GetLevel();
		kPacketNot.m_cUnitClass = GetUnitClass();
		SendToLogDB( ELOG_GET_PURCHASED_CASH_ITEM_NOT, kPacketNot );
	}

	
	switch( pItemTemplet->m_ItemID )
	{
	case CXSLItem::CI_RESURRECTION_STONE:
		{
			// ��Ȱ���̸� �ٷ� ä���ش�.
			m_iNumResurrectionStone += kPickUpPackageInfo.m_usProductPieces;

			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;

#ifdef GIANT_RESURRECTION_CASHSTONE
			KDBE_RESURRECTION_CASHSTONE_NOT kPacketReq2;
			kPacketReq2.m_iUnitUID = GetCharUID();
			kPacketReq2.m_iNumResurrection_CashStone = kPickUpPackageInfo.m_usProductPieces;
			SendToGameDB(DBE_RESURRECTION_CASHSTONE_NOT,kPacketReq2 );
#endif //GIANT_RESURRECTION_CASHSTONE

			goto end_proc;
		}
		break;

		//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
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
#endif	// SERV_CASH_ITEM_FOR_ADD

		{
			KDBE_EXPAND_SKILL_SLOT_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID		= GetCharUID();
			kPacketToDB.m_iPeriodExpire = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
			kPacketToDB.m_usTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM
			kPacketToDB.m_usEventID		= ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
			SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacketToDB );

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}
		break;

	case CXSLItem::CI_EXPAND_SKILL_SLOT_IN_PACKAGE: // ��ų ���� ü���� �޴�
		{
			KDBE_EXPAND_SKILL_SLOT_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID		= GetCharUID();
			kPacketToDB.m_iPeriodExpire = kPickUpPackageInfo.GetProductExpire();
#ifdef SERV_SKILL_PAGE_SYSTEM
			kPacketToDB.m_usTheNumberOfSkillPagesAvailable = m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM
			kPacketToDB.m_usEventID		= ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
			SendToGameDB( DBE_EXPAND_SKILL_SLOT_REQ, kPacketToDB );

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}
		break;
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}

		//{{ 2011. 12. 21	������	���� Ȯ�� ��Ű�� ���� ��ǰ
#ifdef SERV_EXPAND_BANK_ITEM_IN_PACKAGE
	case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
		{
			// ���� Ȯ���Ϸ� ����!
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;
			kPacketToDB.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, ( CXSLInventory::SLOT_COUNT_ONE_LINE * 5 ) ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}
		break;
#endif SERV_EXPAND_BANK_ITEM_IN_PACKAGE
		//}}
		//{{ 2012. 2. 15  ��μ�	�� ���� Ȯ�� ������ - ��Ű����ǰ
#ifdef SERV_EXPAND_QUICK_SLOT
	case CXSLItem::CI_EXPAND_QUICK_SLOT:
		{
			// ���� Ȯ���Ϸ� ����!
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;
			kPacketToDB.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, ( CXSLInventory::EXPAND_QUICK_SLOT ) ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}break;

	case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
		{
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;
			kPacketToDB.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, ( CXSLInventory::EXPAND_QUICK_SLOT ) ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}break;

#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
	case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:	// �����ý��� ��� ������ �� ���� Ȯ�� ������
		{
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;
			kPacketToDB.m_usEventID = ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_E_QUICK_SLOT, ( CXSLInventory::EXPAND_QUICK_SLOT ) ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}break;
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#endif SERV_EXPAND_QUICK_SLOT
		//}}

		//{{ 2013. 2. 28	�ڼ���	 ���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	case CXSLItem::CI_BINGO_MIX_ITEM:
		{
			m_kGSBingoEvent.AddMixChance( kPickUpPackageInfo.m_usProductPieces );
			BingoEventDBWrite();
			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}
		break;

	case CXSLItem::CI_BINGO_RESET_ITEM:
		{
			m_kGSBingoEvent.AddResetChance( kPickUpPackageInfo.m_usProductPieces );
			BingoEventDBWrite();
			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}
		break;
#endif SERV_EVENT_BINGO
		//}}
#ifdef SERV_USE_GNOSIS_IN_PACKAGE
	case CXSLItem::CI_CASH_SKILL_POINT_30_15:
	case CXSLItem::CI_CASH_SKILL_POINT_30_30:
	case CXSLItem::CI_CASH_SKILL_POINT_60_15:
	case CXSLItem::CI_CASH_SKILL_POINT_60_30:
		{
			const int iCSPoint = SiCXSLItemManager()->GetItemCSPoint( iItemID );
			if( iCSPoint <= 0 )
			{
				kPacket.m_iOK =  NetError::ERR_RESET_SKILL_01;
				goto end_proc;
			}

			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID	= GetCharUID();
				kPacketToDB.m_iCSPoint	= iCSPoint;;
				kPacketToDB.m_iPeriod	= kPickUpPackageInfo.GetProductExpire();
				kPacketToDB.m_bUpdateEndDateOnly = false;
				kPacketToDB.m_iSkillPointItemID = iItemID;
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacketToDB );
			}
			else
			{
				if( iCSPoint == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = GetCharUID();
					kPacketToDB.m_iCSPoint = iCSPoint;
					kPacketToDB.m_iPeriod = kPickUpPackageInfo.GetProductExpire();
					kPacketToDB.m_bUpdateEndDateOnly = true;
					kPacketToDB.m_iSkillPointItemID = iItemID;
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacketToDB );
				}
				else
				{
					START_LOG( cerr, L"���� ����ϰ� �ִ� ĳ�ý�ų����Ʈ�� �ٸ� ������ �Ⱓ���� �Ϸ�����!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( pItemTemplet->m_ItemID )
						<< BUILD_LOG( kPickUpPackageInfo.GetProductExpire() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					kPacket.m_iOK =  NetError::ERR_NX_SHOP_01;
					goto end_proc;
				}
			}

			kPacket.m_iOK = NetError::NET_OK;
			goto end_proc;
		}
		break;
#endif // SERV_USE_GNOSIS_IN_PACKAGE

	default:
		// 7. �κ��丮�� �ֱ�
		{
			KItemInfo kItemInfo;
			kItemInfo.m_iItemID	   = iItemID;
			kItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kItemInfo.m_iQuantity  = kPickUpPackageInfo.m_usProductPieces;
			kItemInfo.m_sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
			//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
			kItemInfo.m_sPeriod    = kPickUpPackageInfo.GetProductExpire();
			//}}
			//{{ 2009. 9. 2  ������		�к�
			if( SiCXSLItemManager()->IsCanSealForCashItem( iItemID ) == true )
			{
				kPacketReq.m_setSealCashItem.insert( pItemTemplet->m_ItemID );
			}
			//}}
			//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
			int iSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
			{
				int iItemType = static_cast<int>( pItemTemplet->m_ItemType );

				if( SiCXSLSocketItem()->CheckCashItemSocketID( iItemType, iSocketOption ) == true )
				{
					kItemInfo.m_vecItemSocket.push_back( iSocketOption );
				}
			}
#else
			//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			int iSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, iSocketOption ) )
			{
				kItemInfo.m_vecItemSocket.push_back( iSocketOption );
			}
#else
			short sSocketOption = 0;
			if( m_kUserCashInventory.GetSocketOptionForCash( iItemID, sSocketOption ) )
			{
				kItemInfo.m_vecItemSocket.push_back( sSocketOption );
			}
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}}
#endif SERV_CASH_ITEM_SOCKET_OPTION
			//}

			

			mapItem.insert( std::make_pair( iItemID, kItemInfo ) );	

			// �κ��丮�� ������ ���� �غ�
			m_kInventory.PrepareInsert( mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );
		}
		break;
	}

	// ĳ�� ������ ���
	{
		std::map< int, int >::const_iterator mit;
		for( mit = kPacketReq.m_mapInsertedItem.begin(); mit != kPacketReq.m_mapInsertedItem.end(); ++mit )
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( mit->first );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_MALL, mit->second );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_MALL, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ������ �����
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_CASH_ITEM, kPacketReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
	}
#ifdef SERV_VIP_SYSTEM
	if(iItemID == 80000115 && kPickUpPackageInfo.GetProductExpire() != 0)
	{
		KDBE_INSERT_TITLE_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_iTitleID = 10390;
		kPacketToDB.m_sPeriod  = (short)kPickUpPackageInfo.GetProductExpire();
		kPacketToDB.m_bGameServerEvent = false;
		kPacketToDB.m_bExpandPeriod = false;
		SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
	}
#endif //SERV_VIP_SYSTEM

	// 8. DB������Ʈ �Ǵ� Ŭ���̾�Ʈ �뺸
	if( !kPacketReq.m_vecItemInfo.empty() )
	{	
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_ulProductNo = kPacket_.m_ulProductNo;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushEnduranceChange( kPacketReq.m_kItemEnduranceUpdate );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_INSERT_PURCHASED_CASH_PACKAGE_REQ, kPacketReq );
		return;
	}
	else
	{		
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
	}

end_proc:
	// ������ǰ�� ���� �˻�
	m_kUserCashInventory.DelPickUpPackageInfo( kPacket_.m_ulProductNo );
	m_kUserCashInventory.AddUpdatedInventorySlot( kPacketReq.m_vecUpdatedInventorySlot );	

	if( !m_kUserCashInventory.IsEmptyPickUpPackageInfo() )
		return;

	m_kUserCashInventory.GetUpdatedInventorySlot( kPacket.m_vecInventorySlotInfo );
	SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_INSERT_PURCHASED_CASH_ITEM_ACK, KDBE_INSERT_ITEM_ACK )
{
    // DB�� �߰��� �������� �κ��丮�� �ְ� �������� ������ �����͸� �Ѱ��ش�.
    KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
    kPacket.m_iOK = NetError::NET_OK;
    kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;

    m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
    kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	
#ifdef SERV_GLOBAL_BILLING
	SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
#else // SERV_GLOBAL_BILLING
    SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
#endif // SERV_GLOBAL_BILLING
}

IMPL_ON_FUNC( DBE_INSERT_PURCHASED_CASH_PACKAGE_ACK )
{
	// DB�� �߰��� �������� �κ��丮�� �ְ� �������� ������ �����͸� �Ѱ��ش�.
	KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;

	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	// ������ǰ�� ���� �˻�
	m_kUserCashInventory.DelPickUpPackageInfo( kPacket_.m_ulProductNo );
	m_kUserCashInventory.AddUpdatedInventorySlot( kPacket.m_vecInventorySlotInfo );

	if( !m_kUserCashInventory.IsEmptyPickUpPackageInfo() )
		return;

#ifdef SERV_GLOBAL_CASH_PACKAGE
	m_veciPackageBuyCheck.clear();
#endif //SERV_GLOBAL_CASH_PACKAGE

	m_kUserCashInventory.GetUpdatedInventorySlot( kPacket.m_vecInventorySlotInfo );
	SendPacket( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_CASH_PRODUCT_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_CASH_PRODUCT_INFO_ACK kPacket;

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ��ǰ ����Ʈ ��û" )
			<< BUILD_LOG( GetName() );

		kPacket.m_bFinal = true;
		SendPacket( EGS_CASH_PRODUCT_INFO_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID�� ��� ��ǰ ����Ʈ�� ���� �ʴ´�.
	if( IsGuestUser() )
	{
		kPacket.m_bFinal = true;
		SendPacket( EGS_CASH_PRODUCT_INFO_ACK, kPacket );
		return;
	}

	//{{ 2010. 04. 26  ������	PC�� ���� ĳ����
	kPacket.m_bFinal = SiKNexonBillingTCPManager()->GetProductPage( kPacket_.m_iPage, IsPcBang(), kPacket.m_vecProductInfo );
	//}}
	SendPacket( EGS_CASH_PRODUCT_INFO_ACK, kPacket );
}

#ifndef SERV_GLOBAL_BILLING
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

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	// ��Ȱ�� �ѵ��� ���� ���ϰ� �Ѵ�.
	// �κ��丮 ���� ������ ���� �ִ� �Ѱ踦 ���Ѱ� �Ѵ�.	
	int iStoneToBuy = 0;
	int iInventorySlotToExpand = 0;
	int iBankSlotToExpand = 0;
	int iCharSlotExpand = 0;
	int iExpandSkillSlot = 0;

	std::vector< KNXBTPurchaseReqInfo >::iterator vit;
	for( vit = kPacket_.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_vecPurchaseReqInfo.end(); ++vit )
	{
		const int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"������ ID ��ȯ ����." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø��� ���� ����." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}

		switch(	pItemTemplet->m_ItemID )
		{
		case CXSLItem::CI_RESURRECTION_STONE: // ��Ȱ��
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

				iStoneToBuy += kProductInfo.m_usProductPieces * vit->m_usOrderQuantity;
			}
			break;

		case CXSLItem::CI_EXPAND_INVENTORY: // ����ǰ Ȯ��
			{
				KNXBTProductInfo kProductInfo;
				if( !SiKNexonBillingTCPManager()->GetProductInfo( vit->m_ulProductNo, kProductInfo ) )
				{
					START_LOG( cerr, L"���δ�Ʈ ���� ���� ����." )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT						
						<< BUILD_LOG( vit->m_ulProductNo )
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
#ifdef SERV_REFORM_INVENTORY_TEST
				iInventorySlotToExpand += 8 * kProductInfo.m_usProductPieces * vit->m_usOrderQuantity;
#else
				iInventorySlotToExpand += 8 * vit->m_usOrderQuantity;
#endif SERV_REFORM_INVENTORY_TEST
				//}}
			}
			break;

//--------------------------------------------------------------------------------------------------//
#ifdef SERV_REFORM_INVENTORY_INT_CASH_ITEM
		case CXSLItem::CI_EXPAND_INVENTORY_EQUIP: // ����ǰ Ȯ��( ��� )
			{
				int iInventorySlotToExpand = 1;
				if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_EQUIP, iInventorySlotToExpand ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_ACCESSORY: // ����ǰ Ȯ��( �Ǽ��縮 )
			{
				int iInventorySlotToExpand = 1;
				if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_ACCESSORY, iInventorySlotToExpand ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_QUICK_SLOT: // ����ǰ Ȯ��( �Һ� )
			{
				int iInventorySlotToExpand = 1;
				if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUICK_SLOT, iInventorySlotToExpand ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_MATERIAL: // ����ǰ Ȯ��( ��� )
			{
				int iInventorySlotToExpand = 1;
				if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_MATERIAL,iInventorySlotToExpand ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_QUEST: // ����ǰ Ȯ��( ����Ʈ )
			{
				int iInventorySlotToExpand = 1;
				if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUEST,iInventorySlotToExpand ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			break;
		case CXSLItem::CI_EXPAND_INVENTORY_SPECIAL: // ����ǰ Ȯ��( Ư�� )
			{
				int iInventorySlotToExpand = 1;
				if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_SPECIAL, iInventorySlotToExpand ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			break;
#endif  SERV_REFORM_INVENTORY_INT_CASH_ITEM

#ifdef SERV_SHARING_BANK_QUEST_CASH
		case CXSLItem::CI_CASH_SHARING_BACK_OPEN:
			{
				// ĳ���� ������ 15���� �̸��̸� ���� �Ұ�!
				/*if( GetLevel() < 15 )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_SHARING_BANK_01;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}*/
			}
			break;
#endif
//--------------------------------------------------------------------------------------------------//

		case CXSLItem::CI_EXPAND_CHAR_SLOT_1: // ĳ���� ���� 1�� Ȯ��
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

				iCharSlotExpand += 1;
			}
			break;

		case CXSLItem::CI_EXPAND_CHAR_SLOT_2: // ĳ���� ���� 2�� Ȯ��
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

				iCharSlotExpand += 2;
			}
			break;			

		case CXSLItem::CI_EXPAND_SKILL_SLOT: // ��ų ���� Ȯ��
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
#endif	// SERV_CASH_ITEM_FOR_ADD

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

				iExpandSkillSlot += 1;
			}
			break;

			//{{ 2009. 8. 7  ������		����
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
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( m_kInventory.GetMyNextUpgradeBankItemID() )
						<< BUILD_LOG( pItemTemplet->m_ItemID )
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
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
#ifndef SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
							<< END_LOG;

						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}
				}
				//////////////////////////////////////////////////////////////////////////

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

				// �ֹ������� �Ѱ� �̻��� �ȵ�!
				if( vit->m_usOrderQuantity > 1 )
				{
					START_LOG( cerr, L"���� ȸ������ 1���̻� �ֹ��� �� ����." )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( vit->m_usOrderQuantity )
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// Ȯ�尡������ �˻�����!
				iBankSlotToExpand = CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
			break;

		case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
			{
				// �Ϲ� ������� �ƴϸ� ������ �� ����!
				if( m_kInventory.GetBankMemberShip() != CXSLInventory::MPB_NORMAL )
				{
					START_LOG( cerr, L"���� ������ ����� �ƴմϴ�." )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( m_kInventory.GetBankMemberShip() )						
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_38;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// �κ��丮�� �縮�� �ſ뺸������ ������ ������ ȸ������ ������ �� ����!
				if( m_kInventory.IsExistWithBank( CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE ) == true )
				{
					START_LOG( cerr, L"�縮�� �ſ뺸������ �κ��丮�� ������ ������ ȸ������ ���Ÿ���." )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

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

				// �ֹ������� �Ѱ� �̻��� �ȵ�!
				if( vit->m_usOrderQuantity > 1 )
				{
					START_LOG( cerr, L"���� ȸ������ 1���̻� �ֹ��� �� ����." )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( vit->m_usOrderQuantity )
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// Ȯ�尡������ �˻�����!
				iBankSlotToExpand = CXSLInventory::SLOT_COUNT_ONE_LINE * 5; // 8ĭ�� 5��
			}
			break;
			//}}

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
			//}}
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

		case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:	// �ƶ� ��� ������ �� ���� Ȯ�� ������
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

#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
		case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:	// �����ý��� ��� ������ �� ���� Ȯ�� ������
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
#endif	// SERV_CASH_ITEM_FOR_ELESIS
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

#ifdef SERV_CASHITEM_PURCHASE_RESTRICTION
		case CXSLItem::CI_ICE_BURNER:
		case CXSLItem::CI_OFFICER_EVE_ICE_BURNER:
		case CXSLItem::CI_OFFICER_CHUNG_ICE_BURNER:
		case CXSLItem::CI_PREMIUM_SHOP:
		case CXSLItem::CI_NASOD_MEGAPHONE:
		case CXSLItem::CI_HIGH_NASOD_MEGAPHONE:
			{
				// ���� �˻�
				if( GetLevel() <= 20 )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_62;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			} break;
#endif SERV_CASHITEM_PURCHASE_RESTRICTION			
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

					break;
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

					//{{ 2012. 06. 20	��μ�       ���� ����� ���� ����
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM_EXCEPTOIN_DELETE
					char cItemUnitClass = static_cast<char>(iNewUnitClass);

					if( cItemUnitClass == CXSLUnit::UC_NONE )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}

					if( GetUnitType() == CXSLUnit::UT_ELSWORD 
					 || GetUnitType() == CXSLUnit::UT_ARME
					 || GetUnitType() == CXSLUnit::UT_LIRE
					 || GetUnitType() == CXSLUnit::UT_RAVEN
					 || GetUnitType() == CXSLUnit::UT_EVE
					 || GetUnitType() == CXSLUnit::UT_CHUNG
#ifdef SERV_ARA_MARTIAL_ARTIST
					 || GetUnitType() == CXSLUnit::UT_ARA
#endif
					 )
					{
						if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true ) // 2�� �����̶�� 2���� �����Ѵ�.
						{
							if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(cItemUnitClass) ) == false ) // 2���� �ƴϴ�.
							{
								KEGS_BUY_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
								SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
								return;
							}
						}
						else if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true ) // 1�� �����̶�� 1���� �����Ѵ�.
						{
							if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(cItemUnitClass) ) == false ) // 1���� �ƴϴ�.
							{
								KEGS_BUY_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
								SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
								return;
							}
						}
					}
#endif SERV_UNIT_CLASS_CHANGE_ITEM_EXCEPTOIN_DELETE
					//}}
				}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
				//}}

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
						break;
					}
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

				//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
				//if( SiKDailyGiftBoxManager()->IsDailyGiftBoxItemID( pItemTemplet->m_ItemID ) == true )
				//{
					std::pair<std::multimap<int, KDailyGiftBoxInfo>::const_iterator, std::multimap<int, KDailyGiftBoxInfo>::const_iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( pItemTemplet->m_ItemID );

					for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
					{
						switch( pairFindResult.first->second.m_iState )
						{
						case KDailyGiftBoxManager::PURCHASED_BUY:
						case KDailyGiftBoxManager::PURCHASED_REFUND:
							{
								// �̹� ĳ�� �κ��丮�� �ش� ��Ű���� �����մϴ�.
								KEGS_BUY_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
								SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
								return;
							}

						case KDailyGiftBoxManager::PURCHASED_CONFIRM:
							if( pairFindResult.first->second.m_iUnitUID == GetCharUID() )
							{
								// �� ĳ���ʹ� �̹� �ش� ��Ű���� ������ �����Դϴ�.
								KEGS_BUY_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
								SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
								return;
							}
							break;
						}
					}
				//}
#endif SERV_EVENT_DAILY_GIFT_BOX
				//}}
			}
			break;			
		}
	}

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

	// Ȯ�� ���� ������ 0���� Ŭ���� �˻�
	if( iInventorySlotToExpand > 0 )
	{
		if( !m_kInventory.IsAbleToExpandSlot( iInventorySlotToExpand ) )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}

#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	if(iInventorySlotToExpand <= 0)  // ī�װ��� �κ��丮 Ȯ�� ���� ����
		return;
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

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
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_68;
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

	//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	// ������ ��Ŷ�̶�� ��� ��Ų��.
	if( m_kGSBingoEvent.BingoEventRewardRepeater( kPacket_.m_vecPurchaseReqInfo ) == false )
	{
		BOOST_TEST_FOREACH( const KNXBTPurchaseReqInfo&, kInfo, kPacket_.m_vecPurchaseReqInfo )
		{
			if( ( m_kGSBingoEvent.CheckBingoEventReward( kInfo.m_ulProductNo ) == false ) || ( kInfo.m_usOrderQuantity != 1 ) )
			{
				START_LOG( cerr, L"���� �̺�Ʈ ���� �������� ���Դµ� �������� ��Ʈ�� �ƴ� �� �����ϴ�!!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( kInfo.m_ulProductNo )
					<< BUILD_LOG( kInfo.m_usOrderQuantity )
					<< END_LOG;

				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
	}
#endif SERV_EVENT_BINGO
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ������ ���� ó��

	KENX_BT_PURCHASE_ITEM_REQ kPacketReq;
	kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	kPacketReq.m_bytePacketType = ( kPacket_.m_bUseCoupon == false ) ? KNexonBillingTCPPacket::PURCHASE_ITEM : KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON;
#else // SERV_NEXON_COUPON_SYSTEM
	kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::PURCHASE_ITEM;
#endif // SERV_NEXON_COUPON_SYSTEM
	kPacketReq.m_ulRemoteIP = GetIP();
	kPacketReq.m_byteReason = 1;            // Game Client
	//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
	if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // ���Ϸ��� ä�θ� ����
	{
		GetToonilandID( kPacketReq.m_wstrGameID );
		GetToonilandID( kPacketReq.m_wstrUserID );

		if( kPacketReq.m_wstrGameID.empty() )
		{
			START_LOG( cerr, L"���Ϸ���ID�� ����? ���� ���� ����?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< END_LOG;

			KEGS_BUY_CASH_ITEM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacketAck );
			return;
		}
	}
	else
	{
		kPacketReq.m_wstrGameID = GetName();
		kPacketReq.m_wstrUserID = GetName();
	}
#else
	kPacketReq.m_wstrGameID = GetName();    // �κ��丮 ������ ���� Nexon ID�� ����.
	kPacketReq.m_wstrUserID = GetName();
#endif SERV_CHANNELING_SOAP
	//}}
	kPacketReq.m_ulUserOID = m_kNexonAccountInfo.m_uiNexonSN;
	kPacketReq.m_wstrUserName = GetName();
	kPacketReq.m_byteUserAge = BYTE( m_kNexonAccountInfo.m_uiAge );
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	kPacketReq.m_byteIsCafe	= ( IsPcBang() == true ) ? 1 : 0;
#endif // SERV_NEXON_COUPON_SYSTEM
	CTime tCurrent = CTime::GetCurrentTime();
	kPacketReq.m_wstrOrderID = ( CStringW )tCurrent.Format( _T( "%Y%m%d%H%M%S" ) );
	wchar_t wszNumber[32];
#ifdef _CONVERT_VS_2010
	_i64tow_s( KBaseServer::GetKObj()->GetUID(), wszNumber, 32, 10 );
#else
	_i64tow( KBaseServer::GetKObj()->GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacketReq.m_wstrOrderID += L"_";
	kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
	_i64tow_s( GetUID(), wszNumber, 32, 10 );
#else
	_i64tow( GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacketReq.m_wstrOrderID += L"_";
	kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
	_i64tow_s( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 32, 10 );
#else
	_i64tow( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacketReq.m_wstrOrderID += L"_";
	kPacketReq.m_wstrOrderID += wszNumber;

	START_LOG( clog, L"�ֹ���ȣ" )
		<< BUILD_LOG( kPacketReq.m_wstrOrderID );

	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // ���Ϸ��� ä�θ� ����
	{
		kPacketReq.m_ulPaymentType = 14001;
	}
	else
	{
		kPacketReq.m_ulPaymentType = 13001;
	}
#else
	kPacketReq.m_ulPaymentType = 13001;
#endif SERV_TOONILAND_CHANNELING
	//}}

	if( !SiKNexonBillingTCPManager()->GetTotalPrice( kPacket_.m_vecPurchaseReqInfo, kPacketReq.m_ulTotalAmount ) )
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}
	kPacketReq.m_vecNXBTPurchaseInfo = kPacket_.m_vecPurchaseReqInfo;

	// �򿡼� ĳ������
	m_kUserWishList.SetBuyCashItemInWishList( kPacket_.m_bIsWishList );

	// �ǽð� ��Ȱ��
	m_kUserCashInventory.SetCashResurrectionStone( false );

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_PURCHASE_ITEM_REQ, kPacketReq );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
}
#endif // ndef SERV_GLOBAL_BILLING

_IMPL_ON_FUNC( ENX_BT_PURCHASE_ITEM_ACK, KEGS_BUY_CASH_ITEM_ACK )
{
	//{{ 2013. 2. 18	�ڼ���	���� ���� ������ ���� ���� �� ������ ó��
#ifdef SERV_EVENT_BINGO
	if( SiKGameSysVal()->GetBingoEventPayoutRepeateUse() == true )
	{
		kPacket_.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;

		if( SiKGameSysVal()->GetBingoEventPayoutRepeateUse2() == true )
		{
			SiKGameSysVal()->SetBingoEventPayoutRepeateUse_Lua( false );
		}
	}

	// _IMPL_ON_FUNC( ENX_BT_PURCHASE_ITEM_ACK, KEGS_BUY_CASH_ITEM_ACK )�� ù �ٿ��� ó�� �Ǿ�� �Ѵ�.

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( m_kGSBingoEvent.IsBingoEventRewardRepeater( kPacket_.m_vecPurchaseReqInfo ) == true )
		{
			// ���� ����
			KEGS_BUY_CASH_ITEM_REQ kPacketForPresent;
			kPacketForPresent.m_bIsWishList			= kPacket_.m_bIsWishList;
			kPacketForPresent.m_vecPurchaseReqInfo	= kPacket_.m_vecPurchaseReqInfo;

			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { GetUID(), -1 };
			spEvent->SetData( PI_GS_USER, anTrace, EGS_BUY_CASH_ITEM_REQ, kPacketForPresent );
			QueueingEvent( spEvent );
			return;
		}
		else
		{
			kPacket_.m_vecPurchaseReqInfo.clear();
		}
	}
#endif SERV_EVENT_BINGO
	//}}

	//{{ 2012. 06. 12	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		CTime			tCurrentTime = CTime::GetCurrentTime();
		std::wstring	wstrCurrentTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

		BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo&, kNXBTPurchaseReqInfo, kPacket_.m_vecPurchaseReqInfo )
		{
			const int iItemID = SiKNexonBillingTCPManager()->GetItemID( kNXBTPurchaseReqInfo.m_ulProductNo );
			if( SiKDailyGiftBoxManager()->IsDailyGiftBoxItemID( iItemID ) == true )
			{
				// ������ ���ϸ��� ���� ���� ��Ű���� ����Ϸ� ����!
				KDBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT kPacketToDB;
				kPacketToDB.m_iUserUID		= GetUID();
				kPacketToDB.m_iUnitUID		= GetCharUID();
				kPacketToDB.m_iItemID		= iItemID;
				kPacketToDB.m_iState		= KDailyGiftBoxManager::PURCHASED_BUY;
				kPacketToDB.m_wstrRegDate	= wstrCurrentTime;
				SendToAccountDB( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, kPacketToDB );

				std::pair<std::multimap<int, KDailyGiftBoxInfo>::iterator, std::multimap<int, KDailyGiftBoxInfo>::iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( iItemID );
				for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
				{
					if( pairFindResult.first->second.m_iUnitUID == GetCharUID() )
					{
						break;
					}
				}

				if( pairFindResult.first == pairFindResult.second )
				{
					KDailyGiftBoxInfo kDailyGiftBoxInfo;
					kDailyGiftBoxInfo.m_iUnitUID	= GetCharUID();
					kDailyGiftBoxInfo.m_iState		= KDailyGiftBoxManager::PURCHASED_BUY;
					kDailyGiftBoxInfo.m_wstrRegDate	= wstrCurrentTime;
					m_mmapDailyGiftBoxList.insert( std::multimap<int, KDailyGiftBoxInfo>::value_type( iItemID, kDailyGiftBoxInfo ) );
				}
				else
				{
					pairFindResult.first->second.m_iState		= KDailyGiftBoxManager::PURCHASED_BUY;
					pairFindResult.first->second.m_wstrRegDate	= wstrCurrentTime;
				}
			}
		}
	}
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	unsigned int iCount = 0;
	int	iBingoItemID;
	BOOST_TEST_FOREACH( const KNXBTPurchaseReqInfo&, kProductInfo, kPacket_.m_vecPurchaseReqInfo )
	{
		iBingoItemID = SiKGSBingoEventInfo()->GetBingoEventItemID( kProductInfo.m_ulProductNo );
		if( iBingoItemID != 0 )
		{
			++iCount;

			KEGS_NOTIFY_MSG_NOT	kPacket;
			if( SiCXSLRandomItemManager()->MakeNotifyMsgAttrationItem( iBingoItemID, GetCharName(), kPacket.m_wstrMSG ) == true )
			{
				kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // ������ ȹ�� ����
				kPacket.m_Count = 1;
				//{{ ������ ȹ�� ������ ť�� �̸� ��� - ��μ�
#ifdef SERV_GET_ITEM_NOTIFY
				kPacket.m_iRandomItem = KGSBingoEvent::BSV_MIX_ITEM_ID;
#endif SERV_GET_ITEM_NOTIFY
				//}}
				SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
			}
		}
	}
	
	if( iCount == 0 )
	{
		// ��� ���� ������ ���� ������
		kPacket_.m_bIsWishList = m_kUserWishList.GetBuyCashItemInWishList();
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket_ );
	}
	else if( iCount < kPacket_.m_vecPurchaseReqInfo.size() )
	{
		START_LOG( cerr, L"���� ������ ���� �̺�Ʈ ���� �������� ���� �ֽ��ϴ�??" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_vecPurchaseReqInfo.size() )
			<< BUILD_LOG( iCount )
			<< END_LOG;
	}
#else
	// ��� ���� ������ ���� ������
	kPacket_.m_bIsWishList = m_kUserWishList.GetBuyCashItemInWishList();
	SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket_ );
#endif SERV_EVENT_BINGO
	//}}

	// �ǽð� ��Ȱ��
	if( m_kUserCashInventory.IsCashResurrectionStone() == true  &&  kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
	}

	//{{ 2011. 02. 08	������	ĳ���� ���� �α�
#ifdef SERV_BUY_CASH_ITEM_LOG
	BOOST_TEST_FOREACH( const KNXBTPurchaseReqInfo&, kReq, kPacket_.m_vecPurchaseReqInfo )
	{
		KNXBTProductInfo kProductInfo;
		if( SiKNexonBillingTCPManager()->GetProductInfo( kReq.m_ulProductNo, kProductInfo ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� ��ǰ�� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kReq.m_ulProductNo )
				<< END_LOG;
			continue;
		}

		// ĳ���� ���� ī��Ʈ üũ
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM );
		m_kUserStatistics.IncreaseAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM );

		// �ش� ĳ���Ͱ� ���ʷ� ĳ���� �����Ѱ��̶��?
		if( m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM ) == 1 )
		{
			//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
			WriteCharacterLogToDBWithProductNo( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_BUY_CASH_ITEM, kReq.m_ulProductNo );
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
		kPacketToLog.m_cBuyType			= KELOG_BUY_CASH_ITEM_LOG_NOT::CIBT_BUY;
		kPacketToLog.m_iProductNo		= kProductInfo.m_ulProductNo;
		kPacketToLog.m_iBuyQuantity		= kReq.m_usOrderQuantity;
		kPacketToLog.m_iTotalSalePrice	= kReq.m_usOrderQuantity * kProductInfo.m_ulSalePrice;
		kPacketToLog.m_wstrRegDate		= ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( ELOG_BUY_CASH_ITEM_LOG_NOT, kPacketToLog );

#ifdef SERV_NAVER_CHANNELING
        // TODO : ���̹� ä�θ� , ĳ�� ������ ���� ����.
        if( GetChannelCode() == KNexonAccountInfo::CE_NAVER_ACCOUNT )  // ���̹� ä�θ� ����
        {
            int iItemID = SiKNexonBillingTCPManager()->GetItemID( kProductInfo.m_ulProductNo );
            if( iItemID <= 0 )
            {
                START_LOG( cerr, L"������ ID ��ȯ ����." )
                    << BUILD_LOG( kProductInfo.m_ulProductNo )
                    << BUILD_LOG( iItemID )
                    << END_LOG;
                continue;
            }
            
            std::wstring wstrMessage = std::wstring();
            const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
            if( pItemTemplet == NULL )
            {
                START_LOG( cerr, L"������ ���ø��� ���� ����." )
                    << BUILD_LOG( iItemID )
                    << END_LOG;
                continue;
            }
            std::wstring wstrItemName = pItemTemplet->m_Name;

            wstrMessage = L"���Ӹ�: ���ҵ� \n";
            wstrMessage += L"�����۸�: " + wstrItemName + L"\n"
            //wstrMessage += wstrItemName + L"\n";
                + L"����: " + boost::lexical_cast<std::wstring>(kProductInfo.m_ulSalePrice) + L"\n"
                + L"���Լ���: " + boost::lexical_cast<std::wstring>(kProductInfo.m_usProductPieces * kReq.m_usOrderQuantity) + L"\n" 
                + L"���ų�¥: " + kPacketToLog.m_wstrRegDate + L"\n" 
                + L"��ۿ���: �Ϸ�" + L"\n" 
                + L"û��öȸ: \n"
                + L"-��ȭ(������)�� ������ ���� ���κ��� 7�� �̳� û�� öȸ ����(��, �Ϻ� �������� ��� û��öȸ�� �Ұ���) \n"
                + L"-�� ���� Ȯ�� �ϱ�( http://help.nexon.com/faq/31/2/4156 ) \n"
                + L"�ؽ� �̿��� �� ĳ�� �̿��� Ȯ���ϱ�( https://member.nexon.com/policy/stipulation.aspx ) \n"
                + L"���� ���� �� ������ �̿���( http://help.nexon.com/help/page/nx.aspx?url=qna/qnaguide )";

            KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
            kPacketToDB.m_iFromUnitUID = GetCharUID();
            kPacketToDB.m_iToUnitUID   = GetCharUID();
            kPacketToDB.m_iRewardType  = KPostItemInfo::LT_MESSAGE; // ���� Ÿ��
            kPacketToDB.m_iRewardID	   = 0;
            kPacketToDB.m_wstrMessage  = wstrMessage; // TODO : ���� ���� ����
            SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
        }

#endif SERV_NAVER_CHANNELING
	}
#endif SERV_BUY_CASH_ITEM_LOG
	//}}
}

IMPL_ON_FUNC( EGS_PRESENT_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PRESENT_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ��ǰ �����ϱ� ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_PRESENT_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

#ifdef SERV_COUNTRY_PH
	if ( GetLevel() < 15)
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_40;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacketAck );
		return;
	}

	KEBILL_GARENA_PREPARE_PRESENT_CHECK_REQ kPacketCheckReq;
	kPacketCheckReq.m_kEGSPresentCashItemREQ = kPacket_;
	kPacketCheckReq.m_iUserUID = GetUID();

	SendToKOGBillingDB( EBILL_GARENA_PREPARE_PRESENT_CHECK_REQ, kPacketCheckReq );

	return; // 2013.09.11 lygan_������ // �����ƽþƴ� ������ ���� ���ѿ� �ɸ����� üũ�� �ؾ� �Ѵ�. ������ ���� ����ϴ� cpp�̰� �� �Ʒ��� ���� ���� ������ #else �ɾ����� �ڵ� ���⿡ �����Ƽ� �׳� �ڵ�� ���⼭ return ���� ��������
#endif //SERV_COUNTRY_PH

	//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	BOOST_TEST_FOREACH( const KNXBTPurchaseReqInfo&, kInfo, kPacket_.m_vecPurchaseReqInfo )
	{
		if( SiKGSBingoEventInfo()->IsBingoPresent( kInfo.m_ulProductNo ) == true )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ���� �������� ���� �� �� ����." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kInfo.m_ulProductNo )
				<< BUILD_LOG( kInfo.m_usOrderQuantity )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}
	}
#endif SERV_EVENT_BINGO
	//}}

	//{{ 2009. 3. 24  ������	GM����
	if( kPacket_.m_wstrReceiverNickName == NetError::GetErrStr( NetError::STR_GM_NICKNAME_1 )  ||
		kPacket_.m_wstrReceiverNickName == NetError::GetErrStr( NetError::STR_GM_NICKNAME_2 ) )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GM_CHAR_DENY_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

#ifndef SERV_GLOBAL_BILLING
	// ��й�ȣ �Է� üũ
	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	if( GetChannelCode() == KNexonAccountInfo::CE_NEXON_ACCOUNT )
#endif SERV_TOONILAND_CHANNELING
	//}}
	if( kPacket_.m_wstrPassword.empty() )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_25;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// �ؽ����� ��й�ȣ �ؽ��� üũ
	//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
#else
		//{{2012. 04. 06	��μ�		�ؽ� SSO ��� �� �������� ���Ͽ� ��й�ȣ �˻� ��� ����
#ifdef SERV_DELETE_PW_CHECK
		START_LOG( clog, L"�ؽ� SSO ���������� ���Ͽ� PwdHash ���� ���������� ���� ������ �ʴ´�." );
#else
	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	if( GetChannelCode() != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
#endif SERV_TOONILAND_CHANNELING
		//}}
		if( m_kNexonAccountInfo.m_uiPwdHash == 0 )
		{
			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif SERV_DELETE_PW_CHECK
	//}}
#endif SERV_NEXON_AUTH_SOAP
	//}
#endif // ndef SERV_GLOBAL_BILLING

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrReceiverNickName ) )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// ���ο��� �����Ϸ��°��� üũ�ϱ�
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrReceiverNickName, GetCharName()) )
#else 
	if( kPacket_.m_wstrReceiverNickName == GetCharName() )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_20;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// �������� ��ǰ���� �˻� - ���Ŀ� �ʿ伺 üũ
	std::vector< KNXBTPurchaseReqInfo >::iterator vit;
	for( vit = kPacket_.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_vecPurchaseReqInfo.end(); vit++ )
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
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#ifdef SERV_CASH_ITEM_FOR_ADD
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ADD:
#endif	// SERV_CASH_ITEM_FOR_ADD


#ifdef SERV_SKILL_PAGE_SYSTEM
		case CXSLItem::CI_EXPAND_SKILL_PAGE:
#endif // SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		case CXSLItem::CI_UNLIMITED_CHANGE_SECOND_JOB: //������ 2�� ���� ������
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
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
	m_kUserWishList.SetBuyCashItemInWishList( kPacket_.m_bIsWishList );

	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING
	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
	kPacketToDB.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacketToDB.m_wstrMessage		   = kPacket_.m_wstrMessage;
	kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_iUseCashType))
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iUseCashType = kPacket_.m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );

#else // SERV_GLOBAL_BILLING

	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	if( GetChannelCode() != KNexonAccountInfo::CE_NEXON_ACCOUNT )  // ���Ϸ��� ä�θ� ����
	{
		KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
		kPacketToDB.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
		kPacketToDB.m_wstrMessage		   = kPacket_.m_wstrMessage;
		kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
		kPacketToDB.m_bUseCoupon			= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
		SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
		return;
	}
#endif SERV_TOONILAND_CHANNELING
	//}}

	//{{ 2012. 04. 06	��μ�		�ؽ� SSO ��� �� �������� ���Ͽ� ��й�ȣ �˻� ��� ����
//#ifdef SERV_DELETE_PW_CHECK
	//KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
	//kPacketToDB.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	//kPacketToDB.m_wstrMessage		   = kPacket_.m_wstrMessage;
	//kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
//#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
//	kPacketToDB.m_bUseCoupon			= kPacket_.m_bUseCoupon;
//#endif // SERV_NEXON_COUPON_SYSTEM
	//SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
	//return;
//#endif SERV_DELETE_PW_CHECK
	//}}

	//{{ 2013. 2. 4	�ڼ���	�系 �����ϱ� ��ɿ��� SOAP üũ ����
#ifdef SERV_SOAP_CHECK_OFF_FOR_PRESENT_TEST
	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
	kPacketToDB.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacketToDB.m_wstrMessage		   = kPacket_.m_wstrMessage;
	kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketToDB.m_bUseCoupon			= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
	SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
#else // SERV_SOAP_CHECK_OFF_FOR_PRESENT_TEST
	//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	// soap �� �̿��Ͽ� ������ ��й�ȣ Ȯ���� ����
	KSOAP_CHECK_NEXON_ID_REQ kPacketToSoap;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
	kPacketToSoap.m_iServiceCode = KNexonSOAP::NSC_ELSWORD;
#else // SERV_NEXON_COUPON_SYSTEM
	kPacketToSoap.m_iServiceCode = 5059;
#endif // SERV_NEXON_COUPON_SYSTEM
	kPacketToSoap.m_iGameCode = KNexonSOAP::NGC_ELSWORD;
	kPacketToSoap.m_iNexonSN = ( __int64 )m_kNexonAccountInfo.m_uiNexonSN;
	kPacketToSoap.m_wstrLoginID = GetName();
	kPacketToSoap.m_strPassword = KncUtil::toNarrowString( kPacket_.m_wstrPassword );
	kPacketToSoap.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacketToSoap.m_wstrMessage		   = kPacket_.m_wstrMessage;
	kPacketToSoap.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketToSoap.m_bUseCoupon	= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_NULL, anTrace, SOAP_CHECK_NEXON_ID_REQ, kPacketToSoap );

	SiKNexonSOAPManager()->QueueingEvent( spEvent );
#else
	// �α��μ������� ������ �Է��� ��й�ȣ �����ϱ�
	KELG_PRESENT_CASH_ITEM_REQ kPacketToLg;
	kPacketToLg.m_uiPwdHash			   = m_kNexonAccountInfo.m_uiPwdHash; // password hash value
	kPacketToLg.m_wstrPassword		   = kPacket_.m_wstrPassword;
	kPacketToLg.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacketToLg.m_wstrMessage		   = kPacket_.m_wstrMessage;
	kPacketToLg.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketToLg.m_bUseCoupon			= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
	SendToLoginServer( ELG_PRESENT_CASH_ITEM_REQ, kPacketToLg );
#endif SERV_NEXON_AUTH_SOAP
	//}
#endif SERV_SOAP_CHECK_OFF_FOR_PRESENT_TEST
	//}}
#endif // SERV_GLOBAL_BILLING
}

IMPL_ON_FUNC( ELG_PRESENT_CASH_ITEM_ACK )
{
	// ���� ������?

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ��й�ȣ ��������
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
	kPacketToDB.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacketToDB.m_wstrMessage		   = kPacket_.m_wstrMessage;
	kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketToDB.m_bUseCoupon			= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
	SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
}

#ifndef SERV_GLOBAL_BILLING
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

	KENX_BT_PURCHASE_GIFT_REQ kPacketReq;
	kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketReq.m_bytePacketType = ( kPacket_.m_bUseCoupon == false ) ? KNexonBillingTCPPacket::PURCHASE_GIFT : KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON;
#else // SERV_NEXON_COUPON_SYSTEM
	kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::PURCHASE_GIFT;
#endif // SERV_NEXON_COUPON_SYSTEM
	kPacketReq.m_ulRemoteIP = GetIP();
	kPacketReq.m_byteReason = 1;            // Game Client

	// ������ ���
	kPacketReq.m_wstrSenderGameID	= GetName();    // �κ��丮 ������ ���� Nexon ID�� ����.
	kPacketReq.m_wstrSenderUserID	= GetName();
	kPacketReq.m_ulSenderUserOID	= m_kNexonAccountInfo.m_uiNexonSN;
	kPacketReq.m_wstrSenderUserName = GetName();
	kPacketReq.m_byteSenderUserAge	= BYTE( m_kNexonAccountInfo.m_uiAge );

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	kPacketReq.m_byteIsCafe			= ( IsPcBang() == true ) ? 1 : 0;
#endif // SERV_NEXON_COUPON_SYSTEM

	// �޴� ���
	kPacketReq.m_byteReceiverServerNo = static_cast<BYTE>( SiKNexonBillingTCPManager()->GetBillingServerNo() ); // ������
	kPacketReq.m_wstrReceiverGameID	  = kPacket_.m_wstrUserID;

	CStringW cwstrMessage;
	cwstrMessage.Format( L"[%s] %s", GetCharName().c_str(), kPacket_.m_wstrMessage.c_str() ); // �޽��� ���ڿ� ����
	kPacketReq.m_wstrMessage = static_cast<LPCTSTR>(cwstrMessage);

	CTime tCurrent = CTime::GetCurrentTime();
	kPacketReq.m_wstrOrderID = ( CStringW )tCurrent.Format( _T( "%Y%m%d%H%M%S" ) );
	wchar_t wszNumber[32];
#ifdef _CONVERT_VS_2010
	_i64tow_s( KBaseServer::GetKObj()->GetUID(), wszNumber, 32, 10 );
#else
	_i64tow( KBaseServer::GetKObj()->GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacketReq.m_wstrOrderID += L"_";
	kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
	_i64tow_s( GetUID(), wszNumber, 32, 10 );
#else
	_i64tow( GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacketReq.m_wstrOrderID += L"_";
	kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
	_i64tow_s( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 32, 10 );
#else
	_i64tow( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacketReq.m_wstrOrderID += L"_";
	kPacketReq.m_wstrOrderID += wszNumber;

	START_LOG( clog, L"�����ϱ� ���� ����" )
		<< BUILD_LOG( GetCharName() )
		<< BUILD_LOG( kPacketReq.m_byteReceiverServerNo )
		<< BUILD_LOG( kPacketReq.m_wstrOrderID );

	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // ���Ϸ��� ä�θ� ����
	{
		kPacketReq.m_ulPaymentType = 14001;
	}
	else
	{
		kPacketReq.m_ulPaymentType = 13001;
	}
#else
	kPacketReq.m_ulPaymentType = 13001;
#endif SERV_TOONILAND_CHANNELING
	//}}
	if( !SiKNexonBillingTCPManager()->GetTotalPrice( kPacket_.m_vecPurchaseReqInfo, kPacketReq.m_ulTotalAmount ) )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}
	kPacketReq.m_vecNXBTPurchaseInfo = kPacket_.m_vecPurchaseReqInfo;	

	// �ǽð� ��Ȱ��
	m_kUserCashInventory.SetCashResurrectionStone( false );

	m_kUserCashInventory.SetReceiverUserID( kPacket_.m_wstrUserID ); // ������ UserID �ӽ� ����
	m_kUserCashInventory.SetMessage( kPacket_.m_wstrMessage );		 // �����ϱ� �޽��� �ӽ� ����

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_PURCHASE_GIFT_REQ, kPacketReq );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
}
#endif // SERV_GLOBAL_BILLING

//{{ 2009. 10. 6  ������	�ڵ�����
IMPL_ON_FUNC( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK );

	KEGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK kPacket;
	KNXBTProductInfo kProductInfo;
	CTime tCurrent;

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ �ڵ����� ��ǰ üũ ��û" )
			<< BUILD_LOG( GetName() );

		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		goto end_proc;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}

	// �����ϴ� ��ǰ���� �˻�	
	if( !SiKNexonBillingTCPManager()->GetProductInfo( kPacket_.m_ulProductNo, kProductInfo ) )
	{
		START_LOG( cerr, L"���δ�Ʈ ���� ���� ����." )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
		goto end_proc;
	}

	// Ŭ���̾�Ʈ���� ���������� ���� ���Ű����ϵ��� ������ ����!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiUserSN = m_kNexonAccountInfo.m_uiNexonSN;
	kPacket.m_byDomainType = static_cast<BYTE>( SiKNexonBillingTCPManager()->GetBillingServerNo() ); // ����������
	kPacket.m_ulProductNo = kPacket_.m_ulProductNo;
	kPacket.m_ulSalePrice = kProductInfo.m_ulSalePrice;
	kPacket.m_usOrderQuantity = kPacket_.m_usOrderQuantity;

	tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrTransactionID = ( CStringW )tCurrent.Format( _T( "%Y%m%d%H%M%S" ) );
	wchar_t wszNumber[32];
#ifdef _CONVERT_VS_2010
	_i64tow_s( KBaseServer::GetKObj()->GetUID(), wszNumber, 32, 10 );
#else
	_i64tow( KBaseServer::GetKObj()->GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacket.m_wstrTransactionID += L"_";
	kPacket.m_wstrTransactionID += wszNumber;
#ifdef _CONVERT_VS_2010
	_i64tow_s( GetUID(), wszNumber, 32, 10 );
#else
	_i64tow( GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacket.m_wstrTransactionID += L"_";
	kPacket.m_wstrTransactionID += wszNumber;
#ifdef _CONVERT_VS_2010
	_i64tow_s( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 32, 10 );
#else
	_i64tow( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 10 );
#endif _CONVERT_VS_2010
	kPacket.m_wstrTransactionID += L"_";
	kPacket.m_wstrTransactionID += wszNumber;

	START_LOG( clog, L"�ڵ����� ��ǰ ���� ���� ���" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket.m_byDomainType )
		<< BUILD_LOG( kPacket.m_ulProductNo )
		<< BUILD_LOG( kPacket.m_wstrTransactionID );

end_proc:
	SendPacket( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK, kPacket );
}

IMPL_ON_FUNC( ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK )
{
#ifdef AP_RESTONE
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ����� ���� ��Ȱ�� ���� ó�� : result���� 1�̸� �ڵ����� ������!
	if( kPacket_.m_ulResult == 1 )
	{
		// �ڵ����� ��Ȱ�� ������ 0���϶��� 1�� ä����.
		if( m_iNumAutoPaymentResStone == 0 )
		{
			START_LOG( clog, L"�ڵ����� ��Ȱ���� �����մϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( m_iNumAutoPaymentResStone );

			m_iNumAutoPaymentResStone += 1; // ��ǰ ������ ���� ó��

			// ĳ���� ������ ������Ʈ ������! GUnit ���̺��� lastdate�� ���ŵǵ���
			WriteUnitInfoToDB( false );

			//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			BingoEventDBWrite();
#endif SERV_EVENT_BINGO
			//}}

			KEGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT kPacketNot;
			kPacketNot.m_iNumAutoPaymentResStone = m_iNumAutoPaymentResStone;
			SendPacket( EGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT, kPacketNot );
		}
	}
	else
	{
		START_LOG( clog, L"�ڵ����� ������ �ƴѰ�����!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_ulResult );
	}
#endif AP_RESTONE
}
//}}

_IMPL_ON_FUNC( ENX_BT_PURCHASE_GIFT_ACK, KEGS_PRESENT_CASH_ITEM_ACK )
{
#ifndef SERV_GLOBAL_BILLING
	// ���� ������?

	kPacket_.m_bIsWishList = m_kUserWishList.GetBuyCashItemInWishList();
	SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket_ );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ���� ������ �������� ã�Ƽ� �޽��� �˷��ֱ�
		KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT kPacketToLg;
		kPacketToLg.m_wstrSenderNickName = GetCharName();
		kPacketToLg.m_wstrReceiverUserID = m_kUserCashInventory.GetReceiverUserID();
		kPacketToLg.m_wstrMessage		 = m_kUserCashInventory.GetMessage();

		std::vector< KNXBTPurchaseReqInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_vecPurchaseReqInfo.end(); ++vit )
		{
			SiKNexonBillingTCPManager()->GetProductInfo( vit->m_ulProductNo, kPacketToLg.m_kNXBTProductInfo );

			//{{ 2013. 1. 28	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			// ���� �̺�Ʈ ���� ������, ���� ���� �������� ��ȸ�� ���� ������ �ʴ´�.
			if( SiKGSBingoEventInfo()->IsBingoPresent( vit->m_ulProductNo ) == false )
			{
				KNXBTProductInfo kProductInfo;
				if( SiKNexonBillingTCPManager()->GetProductInfo( vit->m_ulProductNo, kProductInfo ) == true )
				{
					START_LOG( clog, L"��ǰ ���� ����" )
						<< BUILD_LOG( kProductInfo.m_ulProductNo )
						<< BUILD_LOG( kProductInfo.m_wstrProductID )
						<< BUILD_LOG( kProductInfo.m_usProductPieces )
						<< BUILD_LOG( vit->m_usOrderQuantity )
						<< END_LOG;

					SiKGSBingoEventInfo()->GetPackageItemPriceForGift( kProductInfo.m_ulProductNo, kProductInfo.m_ulSalePrice );
					m_kGSBingoEvent.AddBalance( kProductInfo.m_ulSalePrice );
					BingoEventDBWrite();
				}
				else
				{
					START_LOG( clog, L"��Ű�� ��ǰ ���� ������ ����." )
						<< BUILD_LOG( vit->m_ulProductNo )
						<< BUILD_LOG( vit->m_usOrderQuantity )
						<< END_LOG;
				}
			}
#endif SERV_EVENT_BINGO
			//}}
		}

		SendToLoginServer( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, kPacketToLg );
	}

	//{{ 2011. 02. 08	������	ĳ���� ���� �α�
#ifdef SERV_BUY_CASH_ITEM_LOG
	BOOST_TEST_FOREACH( const KNXBTPurchaseReqInfo&, kReq, kPacket_.m_vecPurchaseReqInfo )
	{
		KNXBTProductInfo kProductInfo;
		if( SiKNexonBillingTCPManager()->GetProductInfo( kReq.m_ulProductNo, kProductInfo ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� ��ǰ�� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kReq.m_ulProductNo )
				<< END_LOG;
			continue;
		}

		// ĳ���� ���� ī��Ʈ üũ
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM );
		m_kUserStatistics.IncreaseAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM );

		// �ش� ĳ���Ͱ� ���ʷ� ĳ���� �����Ѱ��̶��?
		if( m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM ) == 1 )
		{
			//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
			WriteCharacterLogToDBWithProductNo( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_BUY_CASH_ITEM, kReq.m_ulProductNo );
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
		kPacketToLog.m_iProductNo		= kProductInfo.m_ulProductNo;
		kPacketToLog.m_iBuyQuantity		= kReq.m_usOrderQuantity;
		kPacketToLog.m_iTotalSalePrice	= kReq.m_usOrderQuantity * kProductInfo.m_ulSalePrice;
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
#endif // SERV_GLOBAL_BILLING
}

IMPL_ON_FUNC( EGS_CHECK_PRESENT_CASH_INVENTORY_NOT )
{
	// ���� ������..?

	KENX_BT_NISMS_INVENTORY_READ_REQ kPacketReq;
	kPacketReq.m_ulPacketNo	  	   = SiKNexonBillingTCPManager()->GetNextPacketNo();
	kPacketReq.m_bytePacketType	   = KNexonBillingTCPPacket::NISMS_INVENTORY_READ;
	kPacketReq.m_ulOrderNo		   = kPacket_.m_ulOrderNo;
	kPacketReq.m_ulProductNo	   = kPacket_.m_ulProductNo;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_INVENTORY_READ_REQ, kPacketReq );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT )
{
	// ���� ������?

	SendPacket( EGS_PRESENT_MESSAGE_TO_RECEIVER_NOT, kPacket_ );
}
//}}

IMPL_ON_FUNC( DBE_INSERT_CASH_SKILL_POINT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );


	KEGS_UPDATE_CASH_SKILL_POINT_NOT kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iCSPoint = kPacket_.m_iCSPoint;
	kPacket.m_wstrCSPointEndDate = kPacket_.m_wstrEndDate;
	kPacket.m_bUpdateEndDateOnly = kPacket_.m_bUpdateEndDateOnly;
	//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	kPacket.m_iSkillPointItemID = kPacket_.m_iSkillPointItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
	//}}

	KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacketToClient;
	KELOG_INSERT_CASH_SKILL_POINT_NOT kPacketToLogDB;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ�� ��ų ����Ʈ ���� DB ������Ʈ ����!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iCSPoint )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_23;
		goto end_proc;
	}


	if( false == kPacket_.m_bUpdateEndDateOnly )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // ����ö // �׳�ý� �ູ ��� ��, ĳ�� ��ų ����Ʈ��
		// ��� ���������� ���� �Ǿ�� �Ѵ�
		m_kSkillTree.SetCSPointEveryPage( kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
		
		m_kSkillTree.SetMaxCSPoint( kPacket_.m_iCSPoint );
	}
	m_kSkillTree.SetCSPointEndDate( kPacket_.m_wstrEndDate );
	
	switch( kPacket_.m_iSkillPointItemID )
	{
		//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		case CXSLItem::EI_SKILL_POINT_30_USE_INVEN:
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
		case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
		case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
		case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
		case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
		case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
		case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
		case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		case CXSLItem::EI_SKILL_POINT_5_USE_INVEN:
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		case CXSLItem::CI_SKILL_POINT_30_USE_INVEN:
		case CXSLItem::CI_SKILL_POINT_60_USE_INVEN:
		case CXSLItem::CI_SKILL_POINT_60_USE_INVEN_ARA:
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		case CXSLItem::CI_SKILL_POINT_5_USE_INVEN:
		case CXSLItem::CI_SKILL_POINT_10_USE_INVEN:
		case CXSLItem::CI_SKILL_POINT_10_USE_INVEN_ARA:
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		//{{ 2013. 03. 27	 �׳�ý��� �ູ 5point 7�� �߰� - ��μ�
#ifdef SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		case CXSLItem::EI_SKILL_POINT_30_DAY_7_USE_INVEN:
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		case CXSLItem::EI_SKILL_POINT_5_DAY_7_USE_INVEN:
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
#ifdef SERV_GNOSIS_BR
		case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
		case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
		//}
		break;

	default:
		{
			// �������� �����Դٴ� ��Ŷ�� Ŭ���̾�Ʈ�� ������
			kPacketToClient.m_iOK = NetError::NET_OK;
			kPacketToClient.m_iNumResurrectionStone = m_iNumResurrectionStone;
#ifdef SERV_GLOBAL_BILLING
			SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacketToClient );
#else // SERV_GLOBAL_BILLING
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacketToClient );
#endif // SERV_GLOBAL_BILLING

			// logdb�� ��� ������
			kPacketToLogDB.m_iUnitUID		= GetCharUID();
			kPacketToLogDB.m_iUnitLevel		= GetLevel();
			kPacketToLogDB.m_iMaxCSPoint	= kPacket_.m_iCSPoint;
			kPacketToLogDB.m_iPeriod		= kPacket_.m_iPeriod;
			kPacketToLogDB.m_bUpdateEndDateOnly = kPacket_.m_bUpdateEndDateOnly;
			SendToLogDB( ELOG_INSERT_CASH_SKILL_POINT_NOT, kPacketToLogDB );
		}
		break;
	}

end_proc:
	SendPacket( EGS_UPDATE_CASH_SKILL_POINT_NOT, kPacket );
}

IMPL_ON_FUNC( DBE_EXPIRE_CASH_SKILL_POINT_ACK )
{
	VERIFY_FORBIDDEN_STATE( ( 3, KGSFSM::S_UID_UPDATED, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT ) );


	// fix!! DB�۾��� ������ ��쿡 ��� ���� ó�������� ���ؼ� �� �� ����غ�����
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		
			
#ifdef SERV_SKILL_PAGE_SYSTEM
			int iSumSPoint = 0;
			for ( UINT i = 0; i < kPacket_.m_vecRetrievedSPoint.size(); i++ )
				iSumSPoint += kPacket_.m_vecRetrievedSPoint[i];
			
			START_LOG( cerr, L"cash skill expire ����, ������ ���� ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iSumSPoint )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< END_LOG;
#else // SERV_SKILL_PAGE_SYSTEM
		START_LOG( cerr, L"cash skill expire ����, ������ ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
#endif // SERV_SKILL_PAGE_SYSTEM
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.AddSPointEveryPage( kPacket_.m_vecRetrievedSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	m_iSPoint += kPacket_.m_iRetrievedSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	m_kSkillTree.ExpireCashSkillPoint();

	//{{ 2010. 07. 04  ������	��ų ����Ʈ ���� Ȯ��
#ifdef SERV_CHECK_SKILL_POINT
	if( CheckSkillPointError() == false )
	{
		if( GetAuthLevel() < SEnum::UAL_GM  )
		{
			START_LOG( cerr, L"ĳ�� ��ų ����Ʈ �Ⱓ ���� ��� ó�������� ��ų ����Ʈ ���� �������Դϴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				//<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
				<< END_LOG;
		}
	}
#endif SERV_CHECK_SKILL_POINT
	//}}	

	KEGS_EXPIRE_CASH_SKILL_POINT_NOT kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_vecRetrievedSPoint = kPacket_.m_vecRetrievedSPoint;
#else // SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iRetrievedSPoint = kPacket_.m_iRetrievedSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	// kimhc // ����ö // 2013-11-21 // ��� ��ų ������ ���� ��� ��ų ���� ��������!
	m_kSkillTree.GetHaveSkillList( kPacket.m_vecMapHaveSKill );
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.GetHaveSkillList( kPacket.m_mapHaveSKill );
#endif // SERV_SKILL_PAGE_SYSTEM


#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	SendPacket( EGS_EXPIRE_CASH_SKILL_POINT_NOT, kPacket );


	// ����� ��� �ٸ������鿡�Ե� ���� ( ������ų ���� ����)
	if( kPacket_.m_iOK == NetError::NET_OK && GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			break;

		default:
			return;
		}

		// ������ ���� ���� ������Ʈ
		KRoomUserInfo kInfo;
		GetRoomUserInfo( kInfo );
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
	}

}

IMPL_ON_FUNC( DBE_GET_WISH_LIST_ACK )
{
	// ���´�..?

	// ��ٱ��� �ʱ�ȭ
	m_kUserWishList.InitWishList( kPacket_.m_mapWishList );
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_WISH_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_WISH_LIST_ACK kPacket;
	m_kUserWishList.GetWishList( kPacket.m_mapWishList );
	SendPacket( EGS_GET_WISH_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_MODIFY_WISH_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_MODIFY_WISH_LIST_ACK );

	KEGS_MODIFY_WISH_LIST_ACK kPacket;
	kPacket.m_iOK				= NetError::NET_OK;
	kPacket.m_vecWishListItemID = kPacket_.m_vecWishListItemID;
	kPacket.m_bIsDelete			= kPacket_.m_bIsDelete;

	if( !kPacket_.m_bIsDelete )
	{
		BOOST_TEST_FOREACH( const int, iWishItemID, kPacket_.m_vecWishListItemID )
		{
			// ��ٱ��� �߰�
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iWishItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� �������� ��ٱ��Ͽ� ����Ϸ� ����." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( iWishItemID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_WISH_LIST_00;
				goto end_proc;
			}

			//{{ ĳ�ü� ���ϱ� ���� �ӽ� ó��
#ifdef DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM
			if( CXSLItem::IsBankExpandItem( pItemTemplet->m_ItemID )
#else //DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM
			
#ifdef SERV_SKILL_PAGE_SYSTEM
			if( CXSLItem::IsChangeJobItem( pItemTemplet->m_ItemID )  
				||  CXSLItem::IsBankExpandItem( pItemTemplet->m_ItemID )
				|| CXSLItem::IsItemToMakeSkillPageExpanded( pItemTemplet->m_ItemID )
#else // SERV_SKILL_PAGE_SYSTEM
			if( CXSLItem::IsChangeJobItem( pItemTemplet->m_ItemID )  ||  CXSLItem::IsBankExpandItem( pItemTemplet->m_ItemID )
#endif // SERV_SKILL_PAGE_SYSTEM
#endif //DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM
#ifdef SERV_WISH_ITEM_DISABLE
				|| CXSLItem::IsPossibleCart( pItemTemplet->m_ItemID) == false
#endif // SERV_WISH_ITEM_DISABLE
#ifdef SERV_EVENT_BUY_FAKE_ITEM
				|| pItemTemplet->m_ItemID == CXSLItem::CI_BUY_FAKE_ITEM
#endif //SERV_EVENT_BUY_FAKE_ITEM
				)
			{
				kPacket.m_iOK = NetError::ERR_WISH_LIST_00;
				goto end_proc;
			}
			//}}

			if( !m_kUserWishList.AddWish( iWishItemID ) )
			{
				START_LOG( cerr, L"��ٱ��Ͽ� ��ǰ ��� ����." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( iWishItemID )
					<< BUILD_LOG( NetError::GetLastErrMsg() )
					<< END_LOG;

				kPacket.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}		
	}
	else
	{
		std::vector< int >::const_iterator vit;
		for( vit = kPacket_.m_vecWishListItemID.begin(); vit != kPacket_.m_vecWishListItemID.end(); ++vit )		
		{
			// ��ٱ��� ����
			if( !m_kUserWishList.DelWish( *vit ) )
			{
				START_LOG( cerr, L"��ٱ��Ͽ� ��ǰ ������ �̻�߻�." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( *vit )
					<< BUILD_LOG( NetError::GetLastErrMsg() )
					<< END_LOG;

				kPacket.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
	}

end_proc:
	SendPacket( EGS_MODIFY_WISH_LIST_ACK, kPacket );
}

//{{ 2010. 11. 17	������	�ؽ� ���� û�� öȸ
#ifdef SERV_NX_BILLING_REFUND
IMPL_ON_FUNC( EGS_CASH_ITEM_REFUND_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CASH_ITEM_REFUND_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ û�� öȸ ��û" )
			<< BUILD_LOG( GetName() );

		KEGS_CASH_ITEM_REFUND_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_CASH_ITEM_REFUND_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	//////////////////////////////////////////////////////////////////////////
	std::wstring wstrRequestID;
	if( SiKNexonBillingTCPManager()->GetCashItemRefundRequestID( wstrRequestID ) == false )
	{
		START_LOG( cerr, L"û�� öȸ ��� �����߿� RequestID�߱��� �����Ͽ����ϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_CASH_ITEM_REFUND_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_45;
		SendPacket( EGS_CASH_ITEM_REFUND_ACK, kPacketAck );
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	KENX_BT_REFUND_REQ kPacketReq;
	kPacketReq.m_ulPacketNo		= SiKNexonBillingTCPManager()->GetNextPacketNo();
	kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::REFUND;
	kPacketReq.m_ulRemoteIP		= GetIP();
	kPacketReq.m_wstrRequestID	= wstrRequestID;
	//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
	if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
	{
		GetToonilandID( kPacketReq.m_wstrGameID );
		if( kPacketReq.m_wstrGameID.empty() )
		{
			START_LOG( cerr, L"���Ϸ���ID�� ����? ���� ���� ����?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< END_LOG;

			KEGS_CASH_ITEM_REFUND_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_CASH_ITEM_REFUND_ACK, kPacketAck );
			return;
		}
	}
	else
	{
		kPacketReq.m_wstrGameID		= GetName();
	}
#else
	kPacketReq.m_wstrGameID		= GetName();
#endif SERV_CHANNELING_SOAP
	//}}

	// ������ ��ȣ�� ���� �˻� (unsined ���� sined ������ ����)
	int iOrderNo = static_cast<int>( kPacket_.m_ulOrderNo );
	int iProductNo = static_cast<int>( kPacket_.m_ulProductNo );
	
	if( iOrderNo > 0 && iProductNo > 0 && kPacket_.m_iQuantity > 0 )
	{
		kPacketReq.m_ulOrderNo		= kPacket_.m_ulOrderNo;
		kPacketReq.m_ulProductNo	= kPacket_.m_ulProductNo;
		kPacketReq.m_usQuantity		= static_cast<u_short>(kPacket_.m_iQuantity);
	}
	else
	{
		START_LOG( cerr, L"û�� öȸ ��û �� Ŭ�� �ִ� ���� �̻��ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_ulOrderNo )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< BUILD_LOG( kPacket_.m_iQuantity )
			<< END_LOG;

		KEGS_CASH_ITEM_REFUND_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_53;
		SendPacket( EGS_CASH_ITEM_REFUND_ACK, kPacketAck );
		return;
	}
	
	//{{ 2012. 06. 12	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	{
		const int iItemID = SiKNexonBillingTCPManager()->GetItemID( kPacket_.m_ulProductNo );
		if( SiKDailyGiftBoxManager()->IsDailyGiftBoxItemID( iItemID ) == true )
		{
			std::pair<std::multimap<int, KDailyGiftBoxInfo>::iterator, std::multimap<int, KDailyGiftBoxInfo>::iterator> pairFindResult = m_mmapDailyGiftBoxList.equal_range( iItemID );

			for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
			{
				if( pairFindResult.first->second.m_iState == KDailyGiftBoxManager::PURCHASED_BUY )
				{
					pairFindResult.first->second.m_iState = KDailyGiftBoxManager::PURCHASED_REFUND;
					break;
				}
			}
		}
	}
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_REFUND_REQ, kPacketReq );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( ENX_BT_REFUND_ACK, KEGS_CASH_ITEM_REFUND_ACK )
{
	//{{ 2012. 06. 12	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		std::wstring wstrCurrentTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

		std::multimap<int, KDailyGiftBoxInfo>::iterator it = m_mmapDailyGiftBoxList.begin();
		for( ; it != m_mmapDailyGiftBoxList.end(); ++it )
		{
			if( it->second.m_iState == KDailyGiftBoxManager::PURCHASED_REFUND )
			{
				it->second.m_iState = KDailyGiftBoxManager::PURCHASED_NOT;
				
				// û�� öȸ�� ���ϸ��� ���� ���� ��Ű���� ����Ϸ� ����!
				KDBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT kPacketToDB;
				kPacketToDB.m_iUserUID		= GetUID();
				kPacketToDB.m_iUnitUID		= it->second.m_iUnitUID;
				kPacketToDB.m_iItemID		= it->first;
				kPacketToDB.m_iState		= KDailyGiftBoxManager::PURCHASED_NOT;
				kPacketToDB.m_wstrRegDate	= wstrCurrentTime;
				SendToAccountDB( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, kPacketToDB );
				break;
			}
		}
	}
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	// ACK�� ���¸� ���� ����.

	SendPacket( EGS_CASH_ITEM_REFUND_ACK, kPacket_ );
}
#endif SERV_NX_BILLING_REFUND
//}}

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
_IMPL_ON_FUNC( DBE_BUY_UNIT_CLASS_CHANGE_ACK, KDBE_BUY_UNIT_CLASS_CHANGE_REQ )
{
// 	if( kPacket_.m_iOK != NetError::NET_OK )
// 	{
// 		START_LOG( cerr, L"���� �ʱ�ȭ ���� ��� ����!" )
// 			<< BUILD_LOG( kPacket_.m_iOK )
// 			<< BUILD_LOG( GetCharName() )
// 			<< END_LOG;
// 
 		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;				// �ٵ� �̰� �ؿ��� ��Ŷ�� �´� �ɷ� �ٲ���ϴ°� �ƴѰ�?
 		kPacket.m_iOK = NetError::NET_OK;
        kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
 		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
// 		return;
// 	}

	KEGS_BUY_UNIT_CLASS_CHANGE_NOT kNot;

	// �Է� ���� ������ ���� ����
	SetUnitClass( static_cast<char>(kPacket_.m_iNewUnitClass) );
	ResetStat();
	m_kSkillTree.SetUnitClass( (int) GetUnitClass() );
	
	// ��ų ����.. ������ų ����
#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.ResetEveryPage();
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	//��ų����Ʈ ���� ����.
	m_kSkillTree.SetSPointEveryPage( kPacket_.m_iSPoint );
	m_kSkillTree.SetCSPointEveryPage( kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	//��ų����Ʈ ���� ����.
	m_iSPoint.SetValue( kPacket_.m_iSPoint );
	m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

	// �⺻ ��ų �ֱ�
#ifdef SERV_SKILL_PAGE_SYSTEM
	// kimhc // ����ö // 2013-11-17
	// ���� ���� �ÿ��� ��� ��ų �������� �ʱ�ȭ ���� �ǵ��� ��

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID1, 1, 0 );
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID2, 1, 0 );
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID3, 1, 0 );
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID4, 1, 0 );
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID5, 1, 0 );
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID6, 1, 0 );
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID, 1, 0 );
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iDefaultSkillID2, 1, 0 );
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


#else // SERV_SKILL_PAGE_SYSTEM
	
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID1, 1, 0 );
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID2, 1, 0 );
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID3, 1, 0 );
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID4, 1, 0 );
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID5, 1, 0 );
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID6, 1, 0 );
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID, 1, 0 );
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID2, 1, 0 );
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#endif // SERV_SKILL_PAGE_SYSTEM

	// skill ����
	m_kSkillTree.SetClassChangeSkill( kPacket_.m_mapChangeSkill );

#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
	// memo ����
	m_kSkillTree.SetClassChangeMemo( kPacket_.m_mapChangeMemo );
#endif // SERV_REFORM_SKILL_NOTE

	// item ����
	m_kInventory.SetClassChangeItem( kPacket_.m_mapChangeItem, kNot.m_vecChangeItem );

	// Quest ����
	m_kUserQuestManager.SetClassChangeQuest( GetCharUID(), kPacket_.m_mapChangeCompleteQuest, kPacket_.m_mapChangeInProgressQuest );
	// ������ Quest ����
	m_kUserQuestManager.SetClassChangeDeleteQuest( GetCharUID(), kPacket_.m_vecDeleteCompleteQuest, kPacket_.m_vecDeleteInProgressQuest );

	// Ŭ���̾�Ʈ���� �˷�����
	
	kNot.m_iUnitUID						= kPacket_.m_iUnitUID;
	kNot.m_iNewUnitClass				= kPacket_.m_iNewUnitClass;
	
#ifdef SERV_REFORM_SKILL_NOTE
#else // EGS_BUY_UNIT_CLASS_CHANGE_NOT
	m_kSkillTree.GetMemoList( kNot.m_mapChangeMemo );
#endif // SERV_REFORM_SKILL_NOTE
	
	GetUnitInfo( kNot.m_kUnitInfo );
	m_kUserQuestManager.GetCompleteChangeClassQuest( kNot.m_vecChangeCompleteQuest );
	m_kUserQuestManager.GetInProgressClassChangeQuest( kNot.m_vecChangeInProgressQuest );
	m_kSkillTree.GetUnSealedSkillList( kNot.m_vecSkillUnsealed );

	SendPacket( EGS_BUY_UNIT_CLASS_CHANGE_NOT, kNot );

	KDBE_UPDATE_UNIT_CLASS_NOT kPacketNot;
	kPacketNot.m_iUnitUID = GetCharUID();
	kPacketNot.m_cUnitClass = GetUnitClass();

	SendToLogDB( DBE_UPDATE_UNIT_CLASS_NOT, kPacketNot );

	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// �ʵ忡 ������� ���� ���� �����ϱ�
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
			}
			break;

		default:
			{
				START_LOG( cerr, L"�̻��� �� Ÿ��." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}

	//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

	//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
	SendUpdateUnitInfoToRelayDB( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT::UT_UNITCLASS_CHANGE );
#endif SERV_RELAY_DB_CONNECTION
	//}}
#ifdef SERV_ARA_LITTLE_DEVIL_YAMA_RAJA_EVENT// �۾���¥: 2013-08-08	// �ڼ���
	if( GetUnitClass() == CXSLUnit::UC_ARA_LITTLE_DEVIL )
	{
		// �̺�Ʈ ������ ����!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_iRewardID	   = 10588;	// ī�ж��� ������ (�ʱ�)
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
	else if( GetUnitClass() == CXSLUnit::UC_ARA_YAMA_RAJA )
	{
		// �̺�Ʈ ������ ����!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_iRewardID	   = 10589;	// ī�ж��� ������ (�ʱ�), ī�ж��� ������ (�߱�)
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
#endif SERV_ARA_LITTLE_DEVIL_YAMA_RAJA_EVENT
	//}}
#ifdef SERV_ARA_LITTLE_DEVIL_YAMA_RAJA_EVENT// �۾���¥: 2013-08-08	// �ڼ���
	if( GetUnitClass() == CXSLUnit::UC_ARA_LITTLE_DEVIL )
	{
		// �̺�Ʈ ������ ����!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_iRewardID	   = 10588;	// ī�ж��� ������ (�ʱ�)
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
	else if( GetUnitClass() == CXSLUnit::UC_ARA_YAMA_RAJA )
	{
		// �̺�Ʈ ������ ����!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_iRewardID	   = 10589;	// ī�ж��� ������ (�ʱ�), ī�ж��� ������ (�߱�)
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
#endif // SERV_ARA_LITTLE_DEVIL_YAMA_RAJA_EVENT

#ifdef SERV_ARA_NEW_FIRST_CLASS_EVENT
	IF_EVENT_ENABLED( CEI_ARA_NEW_FIRST_CLASS_EVENT )
	{
		if( GetUnitClass() == CXSLUnit::UC_ARA_LITTLE_DEVIL )
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_FIRST_CLASS_EVENT_REWARD_ID::iClassChangeReward;	// ���� ���� ����� �������� ����. �򰥸��� �� ��
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif SERV_ARA_NEW_FIRST_CLASS_EVENT

#ifdef SERV_ELESIS_NEW_FIRST_CLASS_EVENT
	IF_EVENT_ENABLED( CEI_ELESIS_NEW_FIRST_CLASS_EVENT )
	{
		if( CXSLUnit::UC_ELESIS_SABER_KNIGHT == GetUnitClass() || CXSLUnit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass() )
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = _CONST_ELESIS_NEW_FIRST_CLASS_EVENT_REWARD_ID::iClassChangeReward;	// ���� ���� ����� �������� ����. �򰥸��� �� ��
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif SERV_ELESIS_NEW_FIRST_CLASS_EVENT

#ifdef SERV_EVENT_CHANGE_CLASS
	IF_EVENT_ENABLED( CEI_EVENT_CHANGE_CLASS )
	{
		if( CXSLUnit::UC_ARME_BATTLE_MAGICIAN == GetUnitClass() )		
		{
			//���� ����� ������ ����
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID    = _CONST_EVENT_CHANGE_CLASS_ITEM_INT_::iTransFormItem; //��Ʈ������ ť��(��Ʋ������) ���̵�� �ٲ����
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif SERV_EVENT_CHANGE_CLASS
	//////////////////////////////////////////////////////////////////////////
	// ���θ�� �ƹ�Ÿ ť�� ����
	CStringW cwstrItemID;

	// ���� ������� ���� �ߴµ�, ���� ������ �̸��� �������� �ִ�. ��Ĩ�ô�.
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
	cwstrItemID.Format( L"%d", CXSLItem::GetClassChangeCashItemIDByUnitClass( GetUnitClass() ) );
#else
	cwstrItemID.Format( L"%d", CXSLItem::GetCashItemByUnitClass( GetUnitClass() ) );
#endif SERV_UNIT_CLASS_CHANGE_ITEM

	if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true 
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		&& kPacket_.m_bUnlimitedSecondJobItem == false //������ ���� ���������� ���� �ÿ��� ���� ���� �ʴ´�.
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
		)
	{
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_MESSAGE;
		kPacketToDB.m_iRewardID	   = CXSLItem::SI_SECOND_CHANGE_JOB_CUBE;
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_wstrMessage  = cwstrItemID.GetBuffer();
		kPacketToDB.m_bGameServerEvent = false;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
	else if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			&& kPacket_.m_bUnlimitedSecondJobItem == false //������ ���� ���������� ���� �ÿ��� ���� ���� �ʴ´�.
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
			)
	{
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_MESSAGE;
		kPacketToDB.m_iRewardID	   = CXSLItem::SI_FIRST_CHANGEJOB_CUBE;
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_wstrMessage  = cwstrItemID.GetBuffer();
		kPacketToDB.m_bGameServerEvent = false;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
IMPL_ON_FUNC( SOAP_CHECK_NEXON_ID_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
		kPacketToDB.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
		kPacketToDB.m_wstrMessage		   = kPacket_.m_wstrMessage;
		kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
		kPacketToDB.m_bUseCoupon			= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
		SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
	}
	else
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_26;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );

		START_LOG( cerr, L"Soap�� �̿��� ���� ��й�ȣ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
	}
}
#endif SERV_NEXON_AUTH_SOAP
//}

//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
IMPL_ON_FUNC( EGS_VISIT_CASH_SHOP_NOT )
{
	if( m_kUserUnitManager.GetEnterCashShop() != kPacket_.m_bEnterCashShop )
	{
		m_kUserUnitManager.SetEnterCashShop( kPacket_.m_bEnterCashShop );
	}
	else
	{
		m_kUserUnitManager.SetEnterCashShop( kPacket_.m_bEnterCashShop );
	}

	// ���� �� ���¶��
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
			// ��Ʋ �ʵ� �϶��� �˷�����
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				kPacket_.m_iUnitUID = GetCharUID();
				SendToCnRoom( ERM_VISIT_CASH_SHOP_NOT, kPacket_ );
			}
			break;

		default:
			return;
		}
	}
}
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
IMPL_ON_FUNC_NOPARAM( EGS_BINGO_EVENT_INFO_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BINGO_EVENT_INFO_REQ, EGS_BINGO_EVENT_INFO_ACK );

	KEGS_BINGO_EVENT_INFO_ACK kPacket;

	switch( m_kGSBingoEvent.GetBingoEventInfo( kPacket ) )
	{
	case KGSBingoEvent::BS_OK:
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_BINGO_EVENT_INFO_ACK, kPacket );
		break;

	case KGSBingoEvent::BS_NONEDATA:
		SendToAccountDB( DBE_BINGO_EVENT_INFO_READ_REQ );
		break;

	default:
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_BINGO_EVENT_INFO_ACK, kPacket );
		break;
	}
}

int KGSUser::SetBingoEventInfo( IN const KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket_ )
//IMPL_ON_FUNC( DBE_BINGO_EVENT_INFO_READ_ACK )
{
	KEGS_BINGO_EVENT_INFO_ACK kPacket;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KGSBingoEvent::BingoState eResult = m_kGSBingoEvent.SetBingoEventInfo( GetCharUID(), kPacket_ );
		
		switch( eResult )
		{
		case KGSBingoEvent::BS_OK:
			
			// ����?
			// ��ȸ ����
			if( KGSBingoEvent::BSV_LOTTERY_BALANCE <= m_kGSBingoEvent.GetBalance() )
			{
				START_LOG( cout, L"m_iBalance ���� BSV_LOTTERY_BALANCE ���� Ů�ϴ�. ��ȸ ������ �õ��մϴ�.")
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( m_kGSBingoEvent.GetBalance() )
					<< END_LOG;
				m_kGSBingoEvent.AddBalance( 0 );
				BingoEventDBWrite();
			}

			/*if( m_kGSBingoEvent.GetBingoEventInfo( kPacket ) != KGSBingoEvent::BS_OK )
			{
				START_LOG( cerr, L"DB���� �����͸� �о�������� ������ �б⿡ �����Ͽ����ϴ�.")
					<< BUILD_LOG( GetUID() )
					<< END_LOG;
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
			}*/
			break;

		case KGSBingoEvent::BS_UNKNOWN:
			START_LOG( cerr, L"�� �� ���� ������ DB�κ��� ���Խ��ϴ�!!")
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			break;

		/*case KGSBingoEvent::BS_ERR_EXISTDATA:
			START_LOG( cerr, L"�̹� ���� ������ ������ �ֽ��ϴ�.")
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			break;*/
		}
	}
	else
	{
		kPacket.m_iOK = kPacket_.m_iOK;
	}

	return kPacket.m_iOK;
	//SendPacket( EGS_BINGO_EVENT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_INFO_WRITE_ACK )
{
	m_kGSBingoEvent.Complete( kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_BINGO_EVENT_LOTTERY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kGSBingoEvent.GetLotteryNum( GetCharUID() ) == KGSBingoEvent::BS_BUSY )
	{
		KEGS_BINGO_EVENT_LOTTERY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BINGO_01;
		SendPacket( EGS_BINGO_EVENT_LOTTERY_ACK, kPacket );
		return;
	}

	KDBE_BINGO_EVENT_INFO_WRITE_REQ kPacket;
	m_kGSBingoEvent.GetBingoEventInfoForDB( DBE_BINGO_EVENT_LOTTERY_ACK, kPacket );
	SendToAccountDB( DBE_BINGO_EVENT_INFO_WRITE_REQ, kPacket );
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_LOTTERY_ACK )
{
	KEGS_BINGO_EVENT_LOTTERY_ACK& kInfo = m_kGSBingoEvent.GetLotteryAck();
	m_kGSBingoEvent.SetPresentInfoForBuy( kInfo.m_vecAcquiredPresent );
	//kInfo.m_vecAcquiredPresent.clear();
	//m_kGSBingoEvent.GetPresentInfoForBuy( kInfo.m_vecAcquiredPresent );
	m_kGSBingoEvent.Complete( kPacket_ );
	if( kInfo.m_vecAcquiredPresent.empty() == false )
	{
		// ���� ����
		KEGS_BUY_CASH_ITEM_REQ kPacketForPresent;
		KNXBTPurchaseReqInfo kProduct;
		kProduct.m_usOrderQuantity = 1;
		BOOST_TEST_FOREACH( byte, iPresentPos, kInfo.m_vecAcquiredPresent )
		{
			if( m_kGSBingoEvent.GetProductNo( iPresentPos, kProduct.m_ulProductNo ) == KGSBingoEvent::BS_OK )
			{
				kPacketForPresent.m_vecPurchaseReqInfo.push_back( kProduct );
			}
			else
			{
				START_LOG( cerr, L"�������� �ʴ� ���� �����Դϴ�.")
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( m_kGSBingoEvent.GetProductItemID( iPresentPos ) )
					<< END_LOG;
			}
		}
		kPacketForPresent.m_bIsWishList = false;

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData( PI_GS_USER, anTrace, EGS_BUY_CASH_ITEM_REQ, kPacketForPresent );
		QueueingEvent( spEvent );
	}

	SendPacket( EGS_BINGO_EVENT_LOTTERY_ACK, m_kGSBingoEvent.GetLotteryAck() );
}

IMPL_ON_FUNC_NOPARAM( EGS_BINGO_EVENT_MIX_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	switch( m_kGSBingoEvent.MixTheBoard( GetCharUID() ) )
	{
	case KGSBingoEvent::BS_OK:
		{
			KDBE_BINGO_EVENT_INFO_WRITE_REQ kPacket;
			m_kGSBingoEvent.GetBingoEventInfoForDB( DBE_BINGO_EVENT_MIX_ACK, kPacket );
			SendToAccountDB( DBE_BINGO_EVENT_INFO_WRITE_REQ, kPacket );
			return;
		}

	case KGSBingoEvent::BS_BUSY:
		{
			KEGS_BINGO_EVENT_MIX_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BINGO_01;
			SendPacket( EGS_BINGO_EVENT_MIX_ACK, kPacket );
			return;
		}

	default:
		{
			KEGS_BINGO_EVENT_MIX_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_BINGO_EVENT_MIX_ACK, kPacket );
			return;
		}
	}
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_MIX_ACK )
{
	KEGS_BINGO_EVENT_MIX_ACK& kInfo = m_kGSBingoEvent.GetMixAck();
	m_kGSBingoEvent.SetPresentInfoForBuy( kInfo.m_vecAcquiredPresent );
	//kInfo.m_vecAcquiredPresent.clear();
	//m_kGSBingoEvent.GetPresentInfoForBuy( kInfo.m_vecAcquiredPresent );
	m_kGSBingoEvent.Complete( kPacket_ );
	if( kInfo.m_vecAcquiredPresent.empty() == false )
	{
		// ���� ����
		KEGS_BUY_CASH_ITEM_REQ kPacketForPresent;
		KNXBTPurchaseReqInfo kProduct;
		kProduct.m_usOrderQuantity = 1;
		BOOST_TEST_FOREACH( byte, iPresentPos, kInfo.m_vecAcquiredPresent )
		{
			if( m_kGSBingoEvent.GetProductNo( iPresentPos, kProduct.m_ulProductNo ) == KGSBingoEvent::BS_OK )
			{
				kPacketForPresent.m_vecPurchaseReqInfo.push_back( kProduct );
			}
			else
			{
				START_LOG( cerr, L"�������� �ʴ� ���� �����Դϴ�.")
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( m_kGSBingoEvent.GetProductItemID( iPresentPos ) )
					<< END_LOG;
			}
		}
		kPacketForPresent.m_bIsWishList = false;

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData( PI_GS_USER, anTrace, EGS_BUY_CASH_ITEM_REQ, kPacketForPresent );
		QueueingEvent( spEvent );
	}

	SendPacket( EGS_BINGO_EVENT_MIX_ACK, m_kGSBingoEvent.GetMixAck() );
}

IMPL_ON_FUNC_NOPARAM( EGS_BINGO_EVENT_RESET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	switch( m_kGSBingoEvent.ResetTheBoard( GetCharUID() ) )
	{
	case KGSBingoEvent::BS_OK:
		{
			KDBE_BINGO_EVENT_INFO_WRITE_REQ kPacket;
			m_kGSBingoEvent.GetBingoEventInfoForDB( DBE_BINGO_EVENT_RESET_ACK, kPacket );
			SendToAccountDB( DBE_BINGO_EVENT_INFO_WRITE_REQ, kPacket );
			return;
		}

	case KGSBingoEvent::BS_BUSY:
		{
			KEGS_BINGO_EVENT_RESET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BINGO_01;
			SendPacket( EGS_BINGO_EVENT_RESET_ACK, kPacket );
			return;
		}
		
	default:
		{
			KEGS_BINGO_EVENT_RESET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_BINGO_EVENT_RESET_ACK, kPacket );
			return;
		}
	}
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_RESET_ACK )
{
	m_kGSBingoEvent.Complete( kPacket_ );
	SendPacket( EGS_BINGO_EVENT_RESET_ACK, m_kGSBingoEvent.GetResetAck() );
}

IMPL_ON_FUNC_NOPARAM( EGS_BINGO_EVENT_INIT_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BINGO_EVENT_INIT_REQ, EGS_BINGO_EVENT_INIT_ACK );

	if( m_kGSBingoEvent.IsBingoComplete() == false )
	{
		KEGS_BINGO_EVENT_INIT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BINGO_02;
		SendPacket( EGS_BINGO_EVENT_INIT_ACK, kPacket );
		return;
	}

	if( m_kGSBingoEvent.InitTheBoard( GetCharUID() ) == KGSBingoEvent::BS_OK )
	{
		KDBE_BINGO_EVENT_INFO_WRITE_REQ kPacket;
		m_kGSBingoEvent.GetBingoEventInfoForDB( DBE_BINGO_EVENT_INIT_ACK, kPacket );
		SendToAccountDB( DBE_BINGO_EVENT_INFO_WRITE_REQ, kPacket );
		return;
	}
	else
	{
		KEGS_BINGO_EVENT_INIT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_BINGO_EVENT_INIT_ACK, kPacket );
		return;
	}
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_INIT_ACK )
{
	m_kGSBingoEvent.Complete( kPacket_ );
	SendPacket( EGS_BINGO_EVENT_INIT_ACK, m_kGSBingoEvent.GetInitAck() );
}

_IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_INQUIRY_ACK, KEGS_PURCHASED_CASH_ITEM_LIST_ACK )
{
	if( kPacket_.m_vecOrderInfo.empty() == true )
	{
		SendPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK, kPacket_ );
		return;
	}

	m_kGSBingoEvent.ClearCurrentOrderNoList();
	m_kGSBingoEvent.ClearOrderNoListForPresentedItem();

	BOOST_TEST_FOREACH( const KNXBTOrderInfo&, kNXBTOrderInfo, kPacket_.m_vecOrderInfo )
	{
		m_kGSBingoEvent.AddCurrentOrderNo( kNXBTOrderInfo.m_ulOrderNo );

		if( kNXBTOrderInfo.m_byteIsPresent == 1 )
		{
			m_kGSBingoEvent.AddOrderNoListForPresentedItem( kNXBTOrderInfo.m_ulOrderNo );
		}
	}

	SendPacket( EGS_PURCHASED_CASH_ITEM_LIST_ACK, kPacket_ );
}
#endif SERV_EVENT_BINGO
//}}

#ifdef SERV_ADD_WARP_BUTTON
IMPL_ON_FUNC( DBE_INSERT_WARP_VIP_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_UPDATE_WARP_VIP_NOT kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_trWarpVipEndDate = 0LL;
	
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KUnitInfo kUnitInfo;
		GetUnitInfo(kUnitInfo);

		CTime ctVipEndDate;
		if( KncUtil::ConvertStringToCTime(  kPacket_.m_wstrEndDate, ctVipEndDate ) == true )
		{
			SetWarpVipEndDate( ctVipEndDate.GetTime() );
			kPacket.m_trWarpVipEndDate = ctVipEndDate.GetTime();

			START_LOG( clog, L"[TEST] ���� VIP ���� ���� �ð� ���� ����" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_wstrEndDate.c_str() )
				<< BUILD_LOG( ctVipEndDate.GetTime() )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"���� VIP ���� ���� �ð� �̻�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_wstrEndDate.c_str() )
				<< END_LOG;

			// �Ⱓ ���� �� ������ �߻��߽��ϴ�
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}
	}
	else
	{
		START_LOG( cerr, L"���� VIP ���� DB ������Ʈ ����!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_wstrEndDate.c_str() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		// �Ⱓ ���� �� ������ �߻��߽��ϴ�
		kPacket.m_iOK = NetError::ERR_SKILL_23;
	}

	SendPacket( EGS_UPDATE_WARP_VIP_NOT, kPacket );
}
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
IMPL_ON_FUNC( EGS_COUPON_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_COUPON_LIST_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ����Ʈ ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_COUPON_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_COUPON_LIST_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_COUPON_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_COUPON_LIST_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_sCouponBoxType )
	{
	case SEnum::NCBT_WHOLE:
	case SEnum::NCBT_ITEM:
	case SEnum::NCBT_DISCOUNT:
		break;
	
	default:
		{
			KEGS_COUPON_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_NX_COUPON_06;	// �߸��� ���� �ڽ� Ÿ���Դϴ�.
			SendPacket( EGS_COUPON_LIST_ACK, kPacket );
			return;
		}
		break;
	}

	KSOAP_COUPON_LIST_REQ kPacketToSoap;
	kPacketToSoap.m_bUsed			= kPacket_.m_bUsed;
	kPacketToSoap.m_strNexonID		= KncUtil::toNarrowString( GetName() );
	kPacketToSoap.m_sCouponBoxType	= kPacket_.m_sCouponBoxType;
	kPacketToSoap.m_iStartRowIndex	= kPacket_.m_usPageNum * SEnum::CL_INDEX_PER_PAGE;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_NULL, anTrace, SOAP_COUPON_LIST_REQ, kPacketToSoap );
	SiKNexonSOAPManager()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( SOAP_COUPON_LIST_ACK )
{
	KEGS_COUPON_LIST_ACK kPacket;
	kPacket.m_iOK				= kPacket_.m_iOK;
	kPacket.m_bUsed				= kPacket_.m_bUsed;
	kPacket.m_sCouponBoxType	= kPacket_.m_sCouponBoxType;
	kPacket.m_usPageNum			= kPacket_.m_iStartRowIndex / SEnum::CL_INDEX_PER_PAGE;
	kPacket.m_iCouponCount		= kPacket_.m_iCouponCount;
	kPacket.m_vecCouponList		= kPacket_.m_vecCouponList;

	SendPacket( EGS_COUPON_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_REGIST_COUPON_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REGIST_COUPON_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ��� ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_REGIST_COUPON_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_REGIST_COUPON_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_REGIST_COUPON_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_REGIST_COUPON_ACK, kPacket );
		return;
	}

	// ���� �ø����� 30��
	if( kPacket_.m_strCouponPin.size() != 35 )
	{
		KEGS_REGIST_COUPON_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NX_WEB_SERVICE_01;	//���� ��ȣ 30�ڸ� �Է����ּ���.
		SendPacket( EGS_REGIST_COUPON_ACK, kPacket );
		return;
	}

	KSOAP_REGIST_COUPON_REQ kPacketToSoap;
	kPacketToSoap.m_strCouponPin	= kPacket_.m_strCouponPin;
	kPacketToSoap.m_strNexonID		= KncUtil::toNarrowString( GetName() );

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_NULL, anTrace, SOAP_REGIST_COUPON_REQ, kPacketToSoap );
	SiKNexonSOAPManager()->QueueingEvent( spEvent );
};

IMPL_ON_FUNC( EGS_USE_ITEM_COUPON_REQ )
{
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_ITEM_COUPON_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ������ ���� ��� ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_USE_ITEM_COUPON_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_USE_ITEM_COUPON_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_USE_ITEM_COUPON_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_USE_ITEM_COUPON_ACK, kPacket );
		return;
	}

    KENX_BT_NISMS_ITEM_COUPON_USING_REQ kPacket;
    kPacket.m_ulPacketNo		= SiKNexonBillingTCPManager()->GetNextPacketNo();
    kPacket.m_bytePacketType	= KNexonBillingTCPPacket::NISMS_ITEM_COUPON_USING;
    kPacket.m_ulRemoteIP		= static_cast<unsigned long>( GetIP() );
	kPacket.m_byteReason		= ( IsPcBang() == true ) ? KNexonBillingTCPManager::NCT_GAME_CLIENT_CAFE : KNexonBillingTCPManager::NCT_GAME_CLIENT;
    kPacket.m_wstrGameID		= GetName();
	kPacket.m_byteIsCafe		= ( IsPcBang() == true ) ? 1 : 0;
    kPacket.m_wstrUserID		= GetName();
    kPacket.m_ulUserOID			= static_cast<unsigned long>( m_kNexonAccountInfo.m_uiNexonSN );
    kPacket.m_wstrUserName		= GetUserName();
    kPacket.m_iCouponCardNo		= kPacket_.m_iCouponCardNo;

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_ITEM_COUPON_USING_REQ, kPacket );
    SiKNexonBillingTCPManager()->QueueingEvent( spEvent );

    START_LOG( clog, L"���� �Է�." )
        << BUILD_LOG( kPacket_.m_iCouponCardNo );
}

IMPL_ON_FUNC( EGS_COUPON_TARGET_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_COUPON_TARGET_ITEM_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ��� ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_COUPON_TARGET_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_COUPON_TARGET_ITEM_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_COUPON_TARGET_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_COUPON_TARGET_ITEM_ACK, kPacket );
		return;
	}

	KSOAP_COUPON_TARGET_ITEM_REQ kPacketToSoap;
	kPacketToSoap.m_iCouponCardNo	= kPacket_.m_iCouponCardNo;
	kPacketToSoap.m_iStartRowIndex	= kPacket_.m_usPageNum * SEnum::CL_INDEX_PER_PAGE;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_NULL, anTrace, SOAP_COUPON_TARGET_ITEM_REQ, kPacketToSoap );
	SiKNexonSOAPManager()->QueueingEvent( spEvent );
};

IMPL_ON_FUNC( SOAP_COUPON_TARGET_ITEM_ACK )
{
	KEGS_COUPON_TARGET_ITEM_ACK kPacket;
	kPacket.m_iOK				= kPacket_.m_iOK;
	kPacket.m_iCouponCardNo		= kPacket_.m_iCouponCardNo;
	kPacket.m_usPageNum			= kPacket_.m_iStartRowIndex / SEnum::CL_INDEX_PER_PAGE;
	kPacket.m_iTargetItemCount	= kPacket_.m_iTargetItemCount;
	kPacket.m_vecTargetItem		= kPacket_.m_vecTargetItem;
	SendPacket( EGS_COUPON_TARGET_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_DISCOUNT_COUPON_INQUIRY_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DISCOUNT_COUPON_INQUIRY_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ���� ��ȸ ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_DISCOUNT_COUPON_INQUIRY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_DISCOUNT_COUPON_INQUIRY_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_DISCOUNT_COUPON_INQUIRY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_DISCOUNT_COUPON_INQUIRY_ACK, kPacket );
		return;
	}

    KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ kPacket;
    kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
    kPacket.m_bytePacketType = KNexonBillingTCPPacket::NISMS_DISCOUNT_COUPON_INQUIRY;
    kPacket.m_ulRemoteIP = static_cast<unsigned long>( GetIP() );
    kPacket.m_wstrUserID = GetName();
    kPacket.m_ulUserOID = static_cast<unsigned long>( m_kNexonAccountInfo.m_uiNexonSN );
	kPacket.m_ulProductNo = kPacket_.m_ulProductNo;
	kPacket.m_usOrderQuantity = kPacket_.m_usOrderQuantity;

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ, kPacket );
    SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
};

IMPL_ON_FUNC( EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK );

	//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"ĳ���� ������ ���� ���� ��ȸ ��û" )
			<< BUILD_LOG( GetName() );			

		KEGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK, kPacket );
		return;
	}

	KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ kPacket;
	kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
	kPacket.m_bytePacketType = KNexonBillingTCPPacket::NISMS_DISCOUNT_COUPON_INQUIRY_LIST;
	kPacket.m_ulRemoteIP = static_cast<unsigned long>( GetIP() );
	kPacket.m_wstrUserID = GetName();
	kPacket.m_ulUserOID = static_cast<unsigned long>( m_kNexonAccountInfo.m_uiNexonSN );
	kPacket.m_vecDiscountCouponInquiryInfo = kPacket_.m_vecDiscountCouponInquiryInfo;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ, kPacket );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
};
#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_NAVER_CHANNELING
IMPL_ON_FUNC_NOPARAM( EGS_GET_NAVER_ACCESS_TOKEN_REQ )
{
    if( GetChannelCode() != KNexonAccountInfo::CE_NAVER_ACCOUNT )
    {
        KEGS_GET_NAVER_ACCESS_TOKEN_ACK kPacket;
        kPacket.m_iOK = -99;
        SendPacket( EGS_GET_NAVER_ACCESS_TOKEN_ACK, kPacket );
        return;
    }
    KNAVER_SOAP_GET_TOKEN_REQ kSOAPReq;
    kSOAPReq.m_iNaverSN = static_cast<__int64>(m_kNexonAccountInfo.m_uiNexonSN);
    
    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { GetUID(), -1 };
    spEvent->SetData(PI_NULL, anTrace, NAVER_SOAP_GET_TOKEN_REQ, kSOAPReq );

    SiKNaverSOAPManager()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( NAVER_SOAP_GET_TOKEN_ACK )
{
    KEGS_GET_NAVER_ACCESS_TOKEN_ACK kPacket;
    kPacket.m_iOK = kPacket_.m_iOK;
    kPacket.m_strAccessToken = kPacket_.m_strAccessToken;

    LOG_SUCCESS( kPacket_.m_iOK == 0 )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetCharName() )
        << BUILD_LOG( kPacket_.m_iOK )
        << BUILD_LOG( kPacket_.m_strAccessToken )
        << END_LOG;

    SendPacket( EGS_GET_NAVER_ACCESS_TOKEN_ACK, kPacket );
}
#endif SERV_NAVER_CHANNELING

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////
