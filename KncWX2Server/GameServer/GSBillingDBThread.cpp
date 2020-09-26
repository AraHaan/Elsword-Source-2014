#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSBillingDBThread.h"
#include "GameServer.h"
#include "NetError.h"

//{{ 2010.11.26   ��ȿ��    SQL_Injection ���� �ذ� 
#include <boost/algorithm/string/replace.hpp>
//}}

ImplementDBThread( KGSBillingDBThread );
ImplPfID( KGSBillingDBThread, PI_GS_KOG_BILLING_DB );

#define CLASS_TYPE KGSBillingDBThread

KGSBillingDBThread::~KGSBillingDBThread()
{
}

void KGSBillingDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	KSerializer ks;
	ks.BeginReading( &spEvent_->m_kbuff );

	switch( spEvent_->m_usEventID )
	{
		CASE( EBILL_CHECK_PRODUCT_INFO_REQ );
		//{{ 2011. 03. 03	������	�߱� ���� ����.�系 ������ �ش� �÷ο� ���
		CASE( EBILL_CHECK_BALANCE_REQ );
		//}}
		_CASE( EBILL_INVENTORY_INQUIRY_REQ, KEGS_BILL_INVENTORY_INQUIRY_REQ );
		
		// ���� �� ���� ���� ���ǵ� üũ
		_CASE( EBILL_PREPARE_BUY_PRODUCT_REQ, KEBILL_BUY_PRODUCT_REQ );
		CASE( EBILL_BUY_PRODUCT_REQ );
		CASE( EBILL_PICK_UP_REQ );

		// ���� �� ���� ���� ���ǵ� üũ
		_CASE( EBILL_PREPARE_GIFT_ITEM_REQ, KEBILL_GIFT_ITEM_REQ );
		CASE( EBILL_GIFT_ITEM_REQ );

		// ���� �ý��� (���� KOG ����)
		CASE( EBILL_USE_COUPON_REQ );

		// ���� �ý��� (���� �ۺ��� ����)
		_CASE( EBILL_USE_COUPON_RESERVE_REQ, KEBILL_USE_COUPON_REQ );
		CASE( EBILL_USE_COUPON_RESULT_REQ );

#ifdef SERV_EVENT_BUY_FAKE_ITEM
		CASE( EBILL_CHECK_BUY_FAKE_ITEM_REQ );
#endif //SERV_EVENT_BUY_FAKE_ITEM

		//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
		CASE( EBILL_REWARD_COUPON_JP_EVENT_REQ );
#endif SERV_RELAY_DB_CONNECTION
		//}}

#ifdef SERV_CONTENT_MANAGER_INT
		CASE( DBE_GET_CASHSHOP_ON_OFF_INFO_REQ );
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_COUNTRY_PH
		CASE( EBILL_GARENA_PREPARE_PRESENT_CHECK_REQ );
#endif //SERV_COUNTRY_PH

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

void KGSBillingDBThread::SendToBillManager( unsigned short usEventID )
{
	SendToBillManager( usEventID, char() );
}

void KGSBillingDBThread::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KGSBillingDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}


#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
int KGSBillingDBThread::DoQuery_GetReleaseTick( OUT std::map<int, int> & mapReleaseTick )
#else
int KGSBillingDBThread::DoQuery_GetReleaseTick( int& iReleaseTick )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
{
	
#ifndef SERV_SUPPORT_SEVERAL_CASH_TYPES
	iReleaseTick = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_ReleaseTick_SEL" );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	
	while( m_kODBC.Fetch() )
	{
		int releaseTick = 0;
		int relaseTickType = 0;

		FETCH_DATA( releaseTick 
			>>relaseTickType );

		
		mapReleaseTick.insert(std::make_pair(relaseTickType, releaseTick ) );

		START_LOG( clog2, L"ReleaseTick ���� ����" )
			<< BUILD_LOG( releaseTick )
			<< BUILD_LOG( relaseTickType );
	}
	
#else
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iReleaseTick );

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;

	return iRet;
}

int KGSBillingDBThread::DoQuery_GetProductList( std::map< int, KBillProductInfo >& mapBillProductInfo )
{
	mapBillProductInfo.clear();

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_Product_SEL" );

	while( m_kODBC.Fetch() )
	{
		KBillProductInfo kInfo;


		FETCH_DATA( kInfo.m_iProductNo
			>> kInfo.m_iProductID
			>> kInfo.m_bSale
			>> kInfo.m_bEvent
			>> kInfo.m_bShow
			>> kInfo.m_bIsCashInvenSkip
			>> kInfo.m_wstrProductName
			>> kInfo.m_cCategoryNo
			>> kInfo.m_iSalePrice
			>> kInfo.m_iRealPrice
			>> kInfo.m_cPeriod
			>> kInfo.m_cQuantity
			>> kInfo.m_bEnableGift
			>> kInfo.m_cLimitLevel
			>> kInfo.m_wstrEndDate
			);


		mapBillProductInfo.insert( std::make_pair( kInfo.m_iProductNo, kInfo ) );
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( mapBillProductInfo.size() )
		<< END_LOG;
	return iRet;
}

int KGSBillingDBThread::DoQuery_GetProductAttributeList( OUT std::map< int, KBillProductAttribute >& mapBillProductAttribute )
{
	mapBillProductAttribute.clear();

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_ProductAttribute_SEL" );

	while( m_kODBC.Fetch() )
	{
		KBillProductAttribute kInfo;
		const int ciNumAttr = 5;
		int iAttr[ciNumAttr];

		FETCH_DATA( kInfo.m_iProductNo
			>> iAttr[0]
			>> iAttr[1]
			>> iAttr[2]
			>> iAttr[3]
			>> iAttr[4]
			);

			for( int i = 0; i < ciNumAttr; i++ )
			{
				kInfo.m_vecAttribute.push_back( iAttr[i] );
			}
			mapBillProductAttribute.insert( std::make_pair( kInfo.m_iProductNo, kInfo ) );
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;
	return iRet;
}

int KGSBillingDBThread::DoQuery_GetPackageInfoList( std::map< int, KBillPackageInfo >& mapBillPackageInfo )
{
	mapBillPackageInfo.clear();

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_ProductPackage_SEL" );

	while( m_kODBC.Fetch() )
	{
		int iPackageNo;
		int iProductNo;
		FETCH_DATA( iPackageNo
			>> iProductNo
			);

		std::map< int, KBillPackageInfo >::iterator mit;
		mit = mapBillPackageInfo.find( iPackageNo );
		if( mit == mapBillPackageInfo.end() )
		{
			KBillPackageInfo kInfo;
			kInfo.m_iPackageNo = iPackageNo;
			kInfo.m_vecProductNo.push_back( iProductNo );
			mapBillPackageInfo.insert( std::make_pair( iPackageNo, kInfo ) );
		}
		else
		{
			mit->second.m_vecProductNo.push_back( iProductNo );
		}
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;
	return iRet;
}

#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
int KGSBillingDBThread::DoQuery_GetProductSalePeriodList( std::map< int, KBillProductInfo >& mapBillProductInfo )
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_ProductShowManager_SEL" );

	while( m_kODBC.Fetch() )
	{
		int iProductNo;
		std::wstring wstrEventTime;
		CTime tEventTime;
		bool bShow;
		
		FETCH_DATA( iProductNo
			>> wstrEventTime
			>> bShow
			);

		if( KncUtil::ConvertStringToCTime( wstrEventTime, tEventTime ) == false )
		{
			START_LOG( cerr, L"Sale Period Time �Ľ� ����!" )
				<< BUILD_LOG( wstrEventTime )
				<< END_LOG;

			continue;
		}

		std::map< int, KBillProductInfo >::iterator mit = mapBillProductInfo.find( iProductNo );
		if( mit != mapBillProductInfo.end() )
		{
			mit->second.m_setSalePeriod.insert( std::make_pair( tEventTime.GetTime(), bShow ) );
		}
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;

	return iRet;
}
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
void KGSBillingDBThread::DoQuery_GetCheckBonusEvent( OUT std::vector<std::wstring> &vecStartEvent,  OUT std::vector<std::wstring> &vecEndEvent)
{
	DO_QUERY_NO_ARG( L"exec dbo.EBP_BillEvent_SEL" );

	while( m_kODBC.Fetch() )
	{
		std::wstring _wsStartEvent = L"";
		std::wstring _wsEndEvent = L"";

		FETCH_DATA( _wsStartEvent
			>> _wsEndEvent
			);
		vecStartEvent.push_back(_wsStartEvent);
		vecEndEvent.push_back(_wsEndEvent);
	}

end_proc:
	START_LOG( clog, L"���ʽ� ĳ�� �̺�Ʈ ����" )
		<< END_LOG;
}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


//{{ 2011. 03. 03	������	�߱� ���� ����. �系 ������ �� �÷ο� �����
int KGSBillingDBThread::DoQuery_GetCashPointByUserUID( IN const UidType iUserUID, 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
										OUT KGlobalCashInfo& RemainCashInfo
#else
										OUT unsigned long& ulTotalCash 
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
										)
{
	// ���� ���� �ڵ�� �������� ����� �߱��� ����ϰ� �ȴ�.
	// ������ �系 ������ ��� ĳ�ø� �����ַ��� ���� ���μ����� ������ ����Ѵ�.
	
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	unsigned long ulTotalCash = 0;
#else
	ulTotalCash = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_Cash_Total_USERUID_SEL", L"%d", % iUserUID );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if( m_kODBC.BeginFetch() )
	{
#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
		// 2013.08.07 lygan_������ �����ƽþ� ���񽺿� ����
		FETCH_DATA( iRet 
			>> ulTotalCash
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH]			// KOG �������� �����ϴ� ĳ��
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT]	// KOG �������� �����ϴ� ���ʽ�
			);
#else
		// �系����. Ŭ���̾�Ʈ ��� ó�� �ڵ� ������ GCT_KOG_ELSWORD_CASH, GCT_KOG_ELSWORD_BONUS_POINT�� �״�� ����� �� ������.
		FETCH_DATA( iRet 
			>> ulTotalCash
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_CASH]			// KOG �������� �����ϴ� ĳ��
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH]	// KOG �������� �����ϴ� ���ʽ�
			);
#endif //defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)

		m_kODBC.EndFetch();
	}
#else
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet 
			>> ulTotalCash
			);

		m_kODBC.EndFetch();
	}

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	else
	{
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;

	return iRet;
}
//}}

int KGSBillingDBThread::DoQuery_GetMyOrderList( IN const UidType iUserUID, IN const int iServerGroupID, OUT std::vector< KBillOrderInfo >& vecOrderInfo )
{
	vecOrderInfo.clear();

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyCashItemLocker_SEL", L"%d, %d", % iUserUID % iServerGroupID );
	while( m_kODBC.Fetch() )
	{
		KBillOrderInfo kInfo;

        FETCH_DATA( kInfo.m_iTransNo
            >> kInfo.m_kBillProductInfo.m_iProductNo
			>> kInfo.m_kBillProductInfo.m_cQuantity
			>> kInfo.m_iFromUnitUID
			>> kInfo.m_wstrSenderPresentMessage
			>> kInfo.m_byteProductKind
            );

		vecOrderInfo.push_back( kInfo );
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;
	return iRet;
}

// ������ �������� ���� ���� ������ üũ��
int KGSBillingDBThread::DoQuery_Buy_Product_CheckCondition( IN const std::wstring& wstrOrderID, 
														   IN const std::wstring& wstrTotalOrderID, 
														   IN const UidType iUserUID,
														   IN const std::wstring& wstrUserID,
														   IN const int iServerGroupID,
														   IN const int iChannelingCode,
														   IN const UidType iUnitUID,
														   IN const char cLevel,
														   IN const int iProductNo, 
														   IN const int iQuantity,
														   IN const int iPrice, 
														   IN const bool bItemKeep,
														   IN const UidType iToUserUID,
														   IN const UidType iToUnitUID,
														   IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
														   IN const int iCashType
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
														   )
{

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_Prepare_Buy_Product_CHK", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
															% wstrOrderID
															% wstrTotalOrderID
															% iUserUID
															% wstrUserID
															% iServerGroupID
															% iUnitUID
															% (int)cLevel
															% iProductNo
															% iQuantity
															% iPrice
															% bItemKeep
															% iToUserUID
															% iToUnitUID
															% wstrGiftMessage
															% iCashType
															% iChannelingCode
															);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet
			);

		m_kODBC.EndFetch();
	}
	
	if(iRet != NetError::NET_OK)
	{
		switch( iRet )
		{
		case -23:	// �Ǹ� �Ⱓ�� �ƴѰ� -23
			iRet = NetError::ERR_BUY_CASH_ITEM_00;	// �����Ϸ��� ��ǰ ������ �̻��մϴ�.
			break;
		case -29:	// �Ǹ� ��ǰ�� �ƴѰ� -29
			iRet = NetError::ERR_BUY_CASH_ITEM_01;	// ��ǰ�� ������ �� �����ϴ�.
			break;
		default:
			iRet = NetError::ERR_BUY_CASH_ITEM_03;
			break;
		}

		goto end_proc;
	}
end_proc:
	return iRet;
}

// ������ ���� ���� ���� ���� ������ üũ��
int KGSBillingDBThread::DoQuery_Gift_Item_CheckCondition( IN const std::wstring& wstrOrderID, 
														 IN const std::wstring& wstrTotalOrderID, 
														 IN const UidType iUserUID,
														 IN const std::wstring& wstrUserID,
														 IN const int iServerGroupID,
														 IN const int iChannelingCode,
														 IN const UidType iUnitUID,
														 IN const char cLevel,
														 IN const int iProductNo, 
														 IN const int iQuantity,
														 IN const int iPrice, 
														 IN const bool bItemKeep,
														 IN const UidType iToUserUID,
														 IN const UidType iToUnitUID,
														 IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
														 IN const int iCashType
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
														 )
{

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_Prepare_Gift_Product_CHK", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
															% wstrOrderID
															% wstrTotalOrderID
															% iUserUID
															% wstrUserID
															% iServerGroupID
															% iUnitUID
															% (int)cLevel
															% iProductNo
															% iQuantity
															% iPrice
															% bItemKeep
															% iToUserUID
															% iToUnitUID
															% wstrGiftMessage
															% iCashType
															% iChannelingCode
															);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet
			);

		m_kODBC.EndFetch();
	}

	if(iRet != NetError::NET_OK)
	{
		switch( iRet )
		{
		case -23:	// �Ǹ� �Ⱓ�� �ƴѰ� -23
			iRet = NetError::ERR_BUY_CASH_ITEM_00;	// �����Ϸ��� ��ǰ ������ �̻��մϴ�.
			break;
		case -29:	// �Ǹ� ��ǰ�� �ƴѰ� -29
			iRet = NetError::ERR_BUY_CASH_ITEM_01;	// ��ǰ�� ������ �� �����ϴ�.
			break;
		default:
			iRet = NetError::ERR_BUY_CASH_ITEM_03;
			break;
		}

		goto end_proc;
	}
end_proc:
	return iRet;
}


// ������ ���� ���� ���� ���� ������ üũ��
int KGSBillingDBThread::DoQuery_Insert_Buy_Product_Log( IN const std::wstring& wstrOrderID, 
														IN const std::wstring& wstrTotalOrderID, 
														IN const UidType iUserUID,
														IN const std::wstring& wstrUserID,
														IN const int iServerGroupID,
														IN const int iChannelingCode,
														IN const UidType iUnitUID,
														IN const char cLevel,
														IN const int iProductNo, 
														IN const int iQuantity,
														IN const int iPrice, 
														IN const bool bItemKeep,
														IN const UidType iToUserUID,
														IN const UidType iToUnitUID,
														IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
														IN const int iCashType
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
														)
{

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyItemLog_INS", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
														% wstrOrderID
														% wstrTotalOrderID
														% iUserUID
														% wstrUserID
														% iServerGroupID
														% iUnitUID
														% (int)cLevel
														% iProductNo
														% iQuantity
														% iPrice
														% bItemKeep
														% iToUserUID
														% iToUnitUID
														% wstrGiftMessage
														% iCashType
														% iChannelingCode
														);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet
			);

		m_kODBC.EndFetch();
	}

end_proc:
	return iRet;
}

int KGSBillingDBThread::DoQuery_BuyProduct( IN const std::wstring& wstrOrderID, 
										    IN const std::wstring& wstrTotalOrderID, 
											IN const UidType iUserUID,
											IN const std::wstring& wstrUserID,
											IN const int iServerGroupID,
											IN const int iChannelingCode,
											IN const UidType iUnitUID,
											IN const char cLevel,
											IN const int iProductNo, 
											IN const int iQuantity,
											IN const int iPrice, 
											IN const bool bItemKeep,
											IN const UidType iToUserUID,
											IN const UidType iToUnitUID,
											IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
											IN const int iCashType,
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
											OUT int& iTotalCash,
											OUT UidType& iTransNo 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
											,OUT KGlobalCashInfo& RemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
											)
{
    int iRet = NetError::ERR_ODBC_01;
	int iOK = 0;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_ALL_RENEWAL_SP

	int iUse_Self_Billing = 0;
#if defined( SERV_COUNTRY_CN ) || defined( SERV_COUNTRY_PH )
	iUse_Self_Billing = 1;
#endif

	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyItem", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, %d",
		% wstrOrderID
		% wstrTotalOrderID
		% iUserUID
		% wstrUserID
		% iServerGroupID
		% iUnitUID
		% (int)cLevel
		% iProductNo
		% iQuantity
		% iPrice
		% bItemKeep
		% iToUserUID
		% iToUnitUID
		% wstrGiftMessage
		% iCashType
		% iChannelingCode
		% iUse_Self_Billing
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyItem", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
		% wstrOrderID
		% wstrTotalOrderID
		% iUserUID
		% wstrUserID
		% iServerGroupID
		% iUnitUID
		% (int)cLevel
		% iProductNo
		% iQuantity
		% iPrice
		% bItemKeep
		% iToUserUID
		% iToUnitUID
		% wstrGiftMessage
		% iCashType
		% iChannelingCode
		);
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTotalCash
			>> iTransNo 
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH]			// KOG �������� �����ϴ� ĳ��
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT]	// KOG �������� �����ϴ� ���ʽ�
			);

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}
#else
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyItem", L"N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'",
		% wstrOrderID
		% iUserUID
		% wstrUserID
		% iServerGroupID
		% iUnitUID
		% (int)cLevel
		% iProductNo
		% iQuantity
		% iPrice
		% bItemKeep
		% iToUserUID
		% iToUnitUID
		% wstrGiftMessage
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTotalCash
			>> iTransNo );
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


	if( iOK != 0 )
	{
		switch( iOK )
		{
		case -5:
			iRet = NetError::ERR_GASH_14;
			break;
		default:
			iRet = NetError::ERR_GASH_10;
			break;
		}
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;

	return iRet;
}

int KGSBillingDBThread::DoQuery_PickUpProductInMyCashInventory( IN const UidType iTransNo, 
																IN const UidType iUserUID, 
																IN const int iServerGroupID, 
																IN const UidType iUnitUID )
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyCashItemTake_SET", L"%d, %d, %d, %d", % iTransNo % iUserUID % iServerGroupID % iUnitUID );

	int iOK;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_NX_SHOP_03;
		goto end_proc;
	}

	//if( iProductNoIn != iProductNoOut )
	//{
	//	iRet = NetError::ERR_NX_SHOP_01;
	//	goto end_proc;
	//}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iTransNo )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOG( iOK )
		<< END_LOG;

	return iRet;
}

#ifdef SERV_GLOBAL_CASH_PACKAGE
int KGSBillingDBThread::DoQuery_PickUpPACKAGEProductInMyCashInventory( IN const UidType iTransNo, 
															   IN const UidType iUserUID, 
															   IN const int iServerGroupID, 
															   IN const UidType iUnitUID,
															   IN const int iProductPackageNUM)
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyCashItemTake_SET", L"%d, %d, %d, %d, %d", % iTransNo % iUserUID % iServerGroupID % iUnitUID % iProductPackageNUM );

	int iOK;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_NX_SHOP_03;
		goto end_proc;
	}

	//if( iProductNoIn != iProductNoOut )
	//{
	//	iRet = NetError::ERR_NX_SHOP_01;
	//	goto end_proc;
	//}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iTransNo )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOG( iProductPackageNUM )
		<< BUILD_LOG( iOK )
		<< END_LOG;

	return iRet;
}

#endif //SERV_GLOBAL_CASH_PACKAGE


#ifdef SERV_GLOBAL_CASH_PACKAGE
int KGSBillingDBThread::DoQuery_GetSubPackageInfoList(IN const UidType iUserUID ,IN const int iServerGroupID,OUT std::vector<int>& _subPackageTrans, OUT std::vector<int>& _subPackageInfo)
{
	int iRet = NetError::ERR_ODBC_01;

	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyCashItemLocker_Package_SEL", L"%d, %d", % iUserUID % iServerGroupID );

	while( m_kODBC.Fetch() )
	{
		int iTranNum;
		int iProductNo;
		FETCH_DATA( iTranNum
			>> iProductNo
			);
		_subPackageTrans.push_back(iTranNum);
		_subPackageInfo.push_back(iProductNo);
	}


	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;

	return iRet;
}
#endif //SERV_GLOBAL_CASH_PACKAGE

// KOG���� ������Ī ���� ������ kPacket�� kCouponMatchingInfo ������ ������
int KGSBillingDBThread::UseCoupon( IN KEBILL_USE_COUPON_REQ kPacket, OUT std::vector< KBillCouponItemInfo > &vecItemInfo )
{
	int iRet = NetError::ERR_ODBC_01;

	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_CouponOrder_USE", L"\'%s\', %d, \'%s\', %d, %d, %d, %d, %d",
															% kPacket.m_wstrSerialCode
															% kPacket.m_PurchaserInfo.m_iUserUID 
															% kPacket.m_PurchaserInfo.m_wstrUserName
															% kPacket.m_PurchaserInfo.m_iServerGroupID 
															% kPacket.m_PurchaserInfo.m_iUnitUID 
															% kPacket.m_PurchaserInfo.m_ucLevel
															% kPacket.m_PurchaserInfo.m_iChannelingCode
															% 1	// ���� Count �� 1���� ���� ����
															);

	int iReturnValue, iTotalCash, iChargeItem, iChargeBonus, iChargeCash, iRemainCash, iRemainBonus;
	UidType iTransNum = -1;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iReturnValue
			>> iTotalCash
			>> iChargeItem
			>> iChargeBonus
			>> iChargeCash
			>> iTransNum
			>> iRemainCash
			>> iRemainBonus
			);
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	switch(iReturnValue)
	{
	// ����
	case 0:		iRet = NetError::NET_OK;	
		break;	
	// ����	
	case -11:	iRet = NetError::ERR_NX_COUPON_00;	break;	//�̹� ����� �����Դϴ�
	case -23:	iRet = NetError::ERR_NX_COUPON_01;	break;	//��� �Ⱓ �ƴ�
	case -29:	iRet = NetError::ERR_NX_COUPON_02;	break;	//���� ��ȣ �߸���
	case -38:	iRet = NetError::ERR_NX_COUPON_03;	break;	//���� ���� �׷� �ߺ� ��뿡��
	default:
		iRet = NetError::ERR_NX_COUPON_04;	//�μ�Ʈ ����
	}
	
	if (iTransNum <= 0)
	{
		if (NetError::NET_OK == iRet)
			iRet = NetError::ERR_ODBC_01;	//�μ�Ʈ ����
	}
	else
	{
		KBillCouponItemInfo sInfo;
		sInfo.m_iProductNO = iChargeItem;
		sInfo.m_iCount = 1;	// �߿� : ���� ������ ������ �Ѱ�¥���� ������ (ĳ�� �κ��� ��������)
		sInfo.m_iTranNo = iTransNum;

		vecItemInfo.push_back(sInfo);
	}


end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( kPacket.m_wstrSerialCode )
		<< BUILD_LOG( kPacket.m_PurchaserInfo.m_iUnitUID )		
		<< BUILD_LOG( iReturnValue )		
		<< END_LOG;

	return iRet;
}

//////////////////////////////////////////////////////////////////////////

int KGSBillingDBThread::DoQuery_TransactionNumberServerGet( OUT __int64& iTranNo, OUT time_t& iTime )
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_TranNumberServer_INT" );

	int iOK;
	__int64 _time;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTranNo
			>> _time );
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_GIANT_BILLING_08;
		goto end_proc;
	}

	iRet = NetError::NET_OK;
	iTime = static_cast<time_t>(_time);

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iTranNo )
		<< BUILD_LOG( iTime )
		<< END_LOG;

	return iRet;
}

void KGSBillingDBThread::MakeTID(__int64 iTransactionNo, time_t iTime, std::wstring& wstrTransaction)
{
	boost::wformat wfmt(L"%02x%03x%08x%08x");

#ifdef SERV_COUNTRY_CN
	START_LOG( cerr, L"�߱��� ���� MakeTID �� �� �ڵ� ���� Ÿ�� �ȵ�!!!! LoginServer�� Ÿ����!! ������Ȳ Ȯ�� �ٶ�!!!" )
		<< END_LOG;
#endif //SERV_COUNTRY_CN
	
	wstrTransaction = boost::str(wfmt
		% 1
		% 1
		% iTime
		% iTransactionNo );
}

int KGSBillingDBThread::DoQuery_CouponOrder( IN const std::wstring& wstrCouponID,
											   IN const std::wstring& wstrOrderID,
											   IN const UidType iUserUID,
											   IN const std::wstring& wstrUserID,
											   IN const int iServerGroupID,
											   IN const UidType iUnitUID,
											   IN const char cLevel,
											   IN const std::wstring& wstrUserIP,
											   IN const char cOrderType )
{
	int iOK = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_CouponOrder_SET", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, N\'%s\', %d",
		% wstrCouponID
		% wstrOrderID
		% iUserUID
		% wstrUserID
		% iServerGroupID
		% iUnitUID
		% (int)cLevel
		% wstrUserIP
		% (int)cOrderType
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	switch( iOK )
	{
	case NetError::NET_OK:	// ����
		break;
	case -11:	// �̹� ����� �����Դϴ�.
		iOK = NetError::ERR_NX_COUPON_00;
		break;
	default:
		iOK = NetError::ERR_NX_COUPON_04;
		break;
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( wstrCouponID )
		<< BUILD_LOG( wstrOrderID )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( iServerGroupID )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOGc( cLevel )
		<< BUILD_LOG( wstrUserIP )
		<< BUILD_LOGc( cOrderType )
		<< BUILD_LOG( iOK )
		<< END_LOG;

	return iOK;
}

int KGSBillingDBThread::DoQuery_CouponOrderResult( IN const std::wstring& wstrOrderID,
													 IN const int iOrderResult,
													 IN int& iChargeItem,
													 IN int& iChargeCash,
													 IN int& iChargeBonus,
													 OUT int& iTotalCash )

{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_CouponOrder_Result", L"N\'%s\', %d, %d, %d, %d",
		% wstrOrderID
		% iOrderResult
		% iChargeItem
		% iChargeCash
		% iChargeBonus
		);

	int iOK;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTotalCash );
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_GIANT_BILLING_05;
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( wstrOrderID )
		<< BUILD_LOG( iOrderResult )
		<< BUILD_LOG( iTotalCash )
		<< BUILD_LOG( iChargeItem )
		<< BUILD_LOG( iChargeCash )
		<< BUILD_LOG( iChargeBonus )
		<< END_LOG;

	return iRet;
}
//////////////////////////////////////////////////////////////////////////


IMPL_ON_FUNC( EBILL_CHECK_PRODUCT_INFO_REQ )
{
	KEBILL_CHECK_PRODUCT_INFO_ACK kPacket;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_mapReleaseTick = kPacket_.m_mapReleaseTick;
	std::map<int, int>	map_ReleaseTick;
#else
	kPacket.m_iReleaseTick = kPacket_.m_iReleaseTick;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES	
	if( DoQuery_GetReleaseTick( map_ReleaseTick) != NetError::NET_OK )
#else
	int iCurrentReleaseTick;
	if( DoQuery_GetReleaseTick( iCurrentReleaseTick ) != NetError::NET_OK )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	{
#ifndef SERV_SUPPORT_SEVERAL_CASH_TYPES
		kPacket.m_iReleaseTick = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

		SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
		return;
	}

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_vecStartEvent.clear();
	kPacket.m_vecEndEvent.clear();
	std::map<int, int>::const_iterator sitBonus = map_ReleaseTick.find(CSRTT_BONUS_EVENT);
	std::map<int, int>::const_iterator sitBonusReq = kPacket_.m_mapReleaseTick.find(CSRTT_BONUS_EVENT);

	std::map<int, int>::const_iterator sitNormal = map_ReleaseTick.find(CSRTT_NORMAL);
	std::map<int, int>::const_iterator sitNormalReq = kPacket_.m_mapReleaseTick.find(CSRTT_NORMAL);

	kPacket.m_mapReleaseTick = map_ReleaseTick;

	if(sitBonus != map_ReleaseTick.end() && sitBonusReq != kPacket_.m_mapReleaseTick.end())
	{
		if( sitBonus->second != sitBonusReq->second )
		{
			DoQuery_GetCheckBonusEvent(kPacket.m_vecStartEvent, kPacket.m_vecEndEvent );
		}
	}

	if(sitNormal != map_ReleaseTick.end() && sitNormalReq != kPacket_.m_mapReleaseTick.end())
	{
		if( sitNormal->second == sitNormalReq->second )
		{
			SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
			return;
		}
	}	
#else
	if( iCurrentReleaseTick == kPacket_.m_iReleaseTick )
	{
		SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
		return;
	}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES	

	if( DoQuery_GetProductList( kPacket.m_mapBillProductInfo ) != NetError::NET_OK )
	{
		kPacket.m_mapBillProductInfo.clear();
		SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
		return ;
	}

#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
	if( DoQuery_GetProductSalePeriodList( kPacket.m_mapBillProductInfo ) != NetError::NET_OK )
	{
		kPacket.m_mapBillProductInfo.clear();
		SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
		return ;
	}
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION

	if( DoQuery_GetProductAttributeList( kPacket.m_mapBillProductAttribute ) != NetError::NET_OK )
	{
		kPacket.m_mapBillProductInfo.clear();
		kPacket.m_mapBillProductAttribute.clear();
		SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
		return ;
	}

	if( DoQuery_GetPackageInfoList( kPacket.m_mapBillPackageInfo ) != NetError::NET_OK )
	{
		kPacket.m_mapBillProductInfo.clear();
		kPacket.m_mapBillProductAttribute.clear();
		kPacket.m_mapBillPackageInfo.clear();
		SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
		return ;
	}

#ifndef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_iReleaseTick = iCurrentReleaseTick;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	
	SendToBillManager( EBILL_CHECK_PRODUCT_INFO_ACK, kPacket );
}

//{{ 2011. 03. 03	������	�߱� ���� ����. �系 ������ �ش� �÷ο� ���
IMPL_ON_FUNC( EBILL_CHECK_BALANCE_REQ )
{
	KEGS_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	kPacket.m_ulBalance = 0;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_GlobalCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_CASH] = kPacket_.m_iPublisherCashBalance;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


	kPacket.m_iOK = DoQuery_GetCashPointByUserUID( FIRST_SENDER_UID
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
													, kPacket.m_GlobalCashInfo 
#else // SERV_SUPPORT_SEVERAL_CASH_TYPES
													, kPacket.m_ulBalance
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
													);

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_ulBalance = kPacket.m_GlobalCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH];
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


	SendToUser( FIRST_SENDER_UID, EBILL_CHECK_BALANCE_ACK, kPacket );
}

//}}

_IMPL_ON_FUNC( EBILL_INVENTORY_INQUIRY_REQ, KEGS_BILL_INVENTORY_INQUIRY_REQ )
{
	KEGS_BILL_INVENTORY_INQUIRY_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	kPacket.m_vecOrderInfo.clear();

	if( kPacket_.m_iCurrentPage <= 0 )
	{
		START_LOG( cerr, L"������ �� �̻�" )
			<< BUILD_LOG( kPacket_.m_iCurrentPage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToUser( FIRST_SENDER_UID, EBILL_INVENTORY_INQUIRY_ACK, kPacket );
		return;
	}

	if( kPacket_.m_nItemPerPage <= 0 )
	{
		START_LOG( cerr, L"������ �� ������ �� �̻�" )
			<< BUILD_LOG( kPacket_.m_nItemPerPage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToUser( FIRST_SENDER_UID, EBILL_INVENTORY_INQUIRY_ACK, kPacket );
		return;
	}

	std::vector< KBillOrderInfo > vecOrderInfo;
	kPacket.m_iOK = DoQuery_GetMyOrderList( FIRST_SENDER_UID, KBaseServer::GetKObj()->GetServerGroupID(), vecOrderInfo );

	int iStart = kPacket_.m_nItemPerPage * ( kPacket_.m_iCurrentPage - 1 );
	int iEnd;
	if( iStart < ( int )vecOrderInfo.size() )
	{
		if( ( int )vecOrderInfo.size() < iStart + kPacket_.m_nItemPerPage )
		{
			iEnd = ( int )vecOrderInfo.size();
		}
		else
		{
			iEnd = iStart + kPacket_.m_nItemPerPage;
		}

		kPacket.m_vecOrderInfo.insert( kPacket.m_vecOrderInfo.begin(), vecOrderInfo.begin() + iStart, vecOrderInfo.begin() + iEnd );
	}

#ifdef SERV_GLOBAL_CASH_PACKAGE
	DoQuery_GetSubPackageInfoList( FIRST_SENDER_UID,  KBaseServer::GetKObj()->GetServerGroupID(),kPacket.m_vecSubPackageTrans, kPacket.m_vecSubPackageInfo);

#endif //SERV_GLOBAL_CASH_PACKAGE

	SendToUser( FIRST_SENDER_UID, EBILL_INVENTORY_INQUIRY_ACK, kPacket );
}


// ���� �� ���� ���� ���ǵ� üũ
_IMPL_ON_FUNC( EBILL_PREPARE_BUY_PRODUCT_REQ, KEBILL_BUY_PRODUCT_REQ )
{
	KEBILL_PREPARE_BUY_PRODUCT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kEBILL_BUY_PRODUCT_REQ = kPacket_;

	
		
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyCashItemLockerCount_CHK", L"%d, %d, %d, %d",
																% kPacket_.m_iUserUID
																% KBaseServer::GetKObj()->GetServerGroupID()
																% (int)kPacket_.m_vecBillBuyInfo.size()
																% 1			// SP ������ ȣ���ϴ� �Ͱ� ��������
																);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			);

		m_kODBC.EndFetch();
	}

	if(kPacket.m_iOK != NetError::NET_OK)
	{
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
		goto end_proc;
	}
	


	//{{2012.02.16 lygan_������ // �������� �����ϱ� ���� �ش� �������� ���� ���� ���ǿ� �����ϴ��� Ȯ�ο�
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		int iOK = DoQuery_Buy_Product_CheckCondition(kBillBuyInfo.m_wstrOrderID,
													kPacket_.m_wstrTotalOrderID,
													kPacket_.m_iUserUID,
													kPacket_.m_wstrUserID,
													KBaseServer::GetKObj()->GetServerGroupID(),
													kPacket_.m_iChannelingCode,
													kPacket_.m_iUnitUID,
													kPacket_.m_BuyCharInfo.m_cLevel,										
													kBillBuyInfo.m_iProductNo,
													kBillBuyInfo.m_usOrderQuantity,
													kBillBuyInfo.m_iPoint,
													true,
													kPacket_.m_iUserUID,
													kPacket_.m_iUnitUID,
													L"",
										#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
													kPacket_.m_iUseCashType
										#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
													);

		if(iOK != NetError::NET_OK)
		{
			// ������ �Ѳ����� ���� �� �ϳ��� ���� ���� üũ �������� ���� ���ϰ� ��
			kPacket.m_iOK = iOK;
			goto end_proc;
		}
	}
	//}}

	//{{ ĳ�� �������� �α� ����
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		int iOK = DoQuery_Insert_Buy_Product_Log(kBillBuyInfo.m_wstrOrderID,
												kPacket_.m_wstrTotalOrderID,
												kPacket_.m_iUserUID,
												kPacket_.m_wstrUserID,
												KBaseServer::GetKObj()->GetServerGroupID(),
												kPacket_.m_iChannelingCode,
												kPacket_.m_iUnitUID,
												kPacket_.m_BuyCharInfo.m_cLevel,										
												kBillBuyInfo.m_iProductNo,
												kBillBuyInfo.m_usOrderQuantity,
												kBillBuyInfo.m_iPoint,
												true,
												kPacket_.m_iUserUID,
												kPacket_.m_iUnitUID,
												L"",
									#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
												kPacket_.m_iUseCashType
									#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
												);

		if(iOK != NetError::NET_OK)
		{
			START_LOG( cerr, L"Buy_Product ���� üũ�� ����ε� �� �α� ����ٰ� ������ ����?" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kBillBuyInfo.m_wstrOrderID )
				<< BUILD_LOG( kPacket_.m_wstrTotalOrderID )
				<< BUILD_LOG( kBillBuyInfo.m_iProductNo )
				<< BUILD_LOG( kBillBuyInfo.m_iProductID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = iOK;
			goto end_proc;
		}
	}
	//}}

end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_PREPARE_BUY_PRODUCT_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{
    KEBILL_BUY_PRODUCT_ACK kPacket;
    kPacket.m_iOK = NetError::NET_OK;

	// ���� ���� ���� üũ�� EBILL_PREPARE_BUY_PRODUCT_REQ ���� �����

	

	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
    {
		UidType iTrans = -1;
	
		int iRet = DoQuery_BuyProduct(  kBillBuyInfo.m_wstrOrderID,
										kPacket_.m_wstrTotalOrderID,
										kPacket_.m_iUserUID,
										kPacket_.m_wstrUserID,
										KBaseServer::GetKObj()->GetServerGroupID(),
										kPacket_.m_iChannelingCode,
										kPacket_.m_iUnitUID,
										kPacket_.m_BuyCharInfo.m_cLevel,										
										kBillBuyInfo.m_iProductNo,
										kBillBuyInfo.m_usOrderQuantity,
										kBillBuyInfo.m_iPoint,
										true,
										kPacket_.m_iUserUID,
										kPacket_.m_iUnitUID,
										L"",
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
										kPacket_.m_iUseCashType,
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
										kPacket.m_iTotalCash,
										iTrans 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
										, kPacket.m_RemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
										);

        if( iRet != NetError::NET_OK )
        {
            START_LOG( cerr, L"insert cash inventory error" )
                << BUILD_LOG( iRet )
                << BUILD_LOG( kBillBuyInfo.m_wstrOrderID )
				<< BUILD_LOG( kPacket_.m_wstrTotalOrderID )
                << BUILD_LOG( kBillBuyInfo.m_iProductNo )
                << BUILD_LOG( kBillBuyInfo.m_iProductID )
                << BUILD_LOG( FIRST_SENDER_UID )
                << END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
        }
		else
		{
			// ���Ű� �����ߴٸ� Ŭ���̾�Ʈ�� ����� �����ϵ��� ����!
			KBillOrderInfo kOrderInfo;
			kOrderInfo.m_iTransNo					= iTrans;
			kOrderInfo.m_wstrOrderID				= kBillBuyInfo.m_wstrOrderID;
			kOrderInfo.m_kBillProductInfo.m_iProductNo = kBillBuyInfo.m_iProductNo;
			kOrderInfo.m_vecSocketOption;
			kOrderInfo.m_byteType					= KBillOrderInfo::BCIT_BUY_ITEM;
			kOrderInfo.m_wstrSenderNickName			= L"";
			kOrderInfo.m_wstrSenderPresentMessage	= L"";
			kOrderInfo.m_iFromUnitUID				= kPacket_.m_iUnitUID;
			kPacket.m_vecBillOrderInfo.push_back( kOrderInfo );
		}
    }

	START_LOG( clog2, L"ebill buy product req" )
		<< BUILD_LOG( kPacket.m_vecBillOrderInfo.size() )
		<< END_LOG;

end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacket );
}


IMPL_ON_FUNC( EBILL_PICK_UP_REQ )
{
	KEBILL_PICK_UP_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	kPacket.m_iProductNo = kPacket_.m_iProductNo;
	kPacket.m_mapSocketForCash = kPacket_.m_mapSocketForCash;

#ifdef SERV_GLOBAL_CASH_PACKAGE
	if(kPacket_.m_byteProductKind == 1)
	{

		kPacket.m_iOK = DoQuery_PickUpPACKAGEProductInMyCashInventory( kPacket_.m_iTransNo, 
			FIRST_SENDER_UID, 
			KBaseServer::GetKObj()->GetServerGroupID(), 
			kPacket_.m_iUnitUID,
			kPacket_.m_iSubProductNo
			);

	}
	else
	{
		kPacket.m_iOK = DoQuery_PickUpProductInMyCashInventory( kPacket_.m_iTransNo, 
			FIRST_SENDER_UID, 
			KBaseServer::GetKObj()->GetServerGroupID(), 
			kPacket_.m_iUnitUID );
	}
#else
	kPacket.m_iOK = DoQuery_PickUpProductInMyCashInventory( kPacket_.m_iTransNo, 
		FIRST_SENDER_UID, 
		KBaseServer::GetKObj()->GetServerGroupID(), 
		kPacket_.m_iUnitUID );
#endif //SERV_GLOBAL_CASH_PACKAGE

#ifdef SERV_GLOBAL_CASH_PACKAGE
	if(kPacket_.m_byteProductKind != 1)
	{

		SendToUser( FIRST_SENDER_UID, EBILL_PICK_UP_ACK, kPacket );

	}
	else
	{
		KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK kPacket2;

		kPacket2.m_bytePacketType = kPacket_.m_byteProductKind;
		kPacket2.m_ulOrderNo = kPacket_.m_iTransNo;
		kPacket2.m_ulPackageProductNo = kPacket_.m_iProductNo;
		kPacket2.m_ulProductNo = kPacket_.m_iSubProductNo;
		kPacket2.m_ulSubProductNo = kPacket_.m_iSubProductID;
		if(kPacket.m_iOK == 0)
		{
			kPacket2.m_ulResult = 1;
		}
		SendToUser( FIRST_SENDER_UID, ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK, kPacket2 );
	}
	
#else
	SendToUser( FIRST_SENDER_UID, EBILL_PICK_UP_ACK, kPacket );
#endif //SERV_GLOBAL_CASH_PACKAGE

	
}


_IMPL_ON_FUNC( EBILL_PREPARE_GIFT_ITEM_REQ, KEBILL_GIFT_ITEM_REQ )
{
	KEBILL_PREPARE_GIFT_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kEBILL_GIFT_ITEM_REQ = kPacket_;


	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BuyCashItemLockerCount_CHK", L"%d, %d, %d, %d",
																	% kPacket_.m_iReceiverUserUID
																	% KBaseServer::GetKObj()->GetServerGroupID()
																	% (int)kPacket_.m_vecBillBuyInfo.size()
																	% 1			// SP ������ ȣ���ϴ� �Ͱ� ��������
																	);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			);

		m_kODBC.EndFetch();
	}

	if(kPacket.m_iOK != NetError::NET_OK)
	{
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
		goto end_proc;
	}


	//{{ 2010.11.26 ��ȿ�� SQL_Injection ���� �ذ� 
	//std::wstring wstr_tmp = kPacket_.m_wstrPresentMessage; 
	//boost::replace_all( wstr_tmp, L"'",L"''"); 
	//}}

	//{{2012.02.16 lygan_������ // �������� �����ϱ� ���� �ش� �������� ���� ���� ���ǿ� �����ϴ��� Ȯ�ο�
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		int iOK = DoQuery_Gift_Item_CheckCondition( kBillBuyInfo.m_wstrOrderID,
													kPacket_.m_wstrTotalOrderID,
													kPacket_.m_iSenderUserUID,
													kPacket_.m_wstrSenderUserID,
													KBaseServer::GetKObj()->GetServerGroupID(),
													kPacket_.m_iChannelingCode,
													kPacket_.m_iSenderUnitUID,
													kPacket_.m_RecvCharInfo.m_cLevel,										
													kBillBuyInfo.m_iProductNo,
													kBillBuyInfo.m_usOrderQuantity,
													kBillBuyInfo.m_iPoint,
													true,
													kPacket_.m_iReceiverUserUID,
													kPacket_.m_iReceiverUnitUID,
													L"", //wstr_tmp
										#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
													kPacket_.m_iUseCashType
										#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
													);

		if(iOK != NetError::NET_OK)
		{
			// ������ �Ѳ����� ���� �� �ϳ��� ���� ���� üũ �������� ���� ���ϰ� ��
			kPacket.m_iOK = iOK;
			goto end_proc;
		}
	}
	//}}


	//{{2012.02.16 lygan_������ // �������� �����ϱ� ���� �ش� �������� ���� ���� ���ǿ� �����ϴ��� Ȯ�ο�
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		int iOK = DoQuery_Gift_Item_CheckCondition( kBillBuyInfo.m_wstrOrderID,
													kPacket_.m_wstrTotalOrderID,
													kPacket_.m_iSenderUserUID,
													kPacket_.m_wstrSenderUserID,
													KBaseServer::GetKObj()->GetServerGroupID(),
													kPacket_.m_iChannelingCode,
													kPacket_.m_iSenderUnitUID,
													kPacket_.m_RecvCharInfo.m_cLevel,										
													kBillBuyInfo.m_iProductNo,
													kBillBuyInfo.m_usOrderQuantity,
													kBillBuyInfo.m_iPoint,
													true,
													kPacket_.m_iReceiverUserUID,
													kPacket_.m_iReceiverUnitUID,
													L"", //wstr_tmp
										#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
													kPacket_.m_iUseCashType
										#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
													);


		if(iOK != NetError::NET_OK)
		{
			START_LOG( cerr, L"Gift_Item ���� üũ�� ����ε� �� �α� ����ٰ� ������ ����?" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kBillBuyInfo.m_wstrOrderID )
				<< BUILD_LOG( kPacket_.m_wstrTotalOrderID )
				<< BUILD_LOG( kBillBuyInfo.m_iProductNo )
				<< BUILD_LOG( kBillBuyInfo.m_iProductID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = iOK;
			goto end_proc;
		}
	}
	//}}

end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_PREPARE_GIFT_ITEM_ACK, kPacket );
}


IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
	KEBILL_GIFT_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2010.11.26 ��ȿ�� SQL_Injection ���� �ذ� 
	std::wstring wstr_tmp = kPacket_.m_wstrPresentMessage; 
	boost::replace_all( wstr_tmp, L"'",L"''"); 
	//}}

	// ���� ���� ���� üũ�� EBILL_PREPARE_GIFT_ITEM_REQ ���� �����

	

	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		UidType iTrans = -1;
		
		int iRet = DoQuery_BuyProduct(  kBillBuyInfo.m_wstrOrderID,
										kPacket_.m_wstrTotalOrderID,
										kPacket_.m_iSenderUserUID,
										kPacket_.m_wstrSenderUserID,
										KBaseServer::GetKObj()->GetServerGroupID(),
										kPacket_.m_iChannelingCode,
										kPacket_.m_iSenderUnitUID,
										kPacket_.m_BuyCharInfo.m_cLevel,
										kBillBuyInfo.m_iProductNo, 
										kBillBuyInfo.m_usOrderQuantity,
										kBillBuyInfo.m_iPoint,
										true,
										kPacket_.m_iReceiverUserUID,
										kPacket_.m_iReceiverUnitUID,
										wstr_tmp,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
										kPacket_.m_iUseCashType,
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
										kPacket.m_iTotalCash,
										iTrans 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
										, kPacket.m_RemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
										);
		if( iRet != NetError::NET_OK )
		{
			START_LOG( cerr, L"insert cash inventory by gift error" )
				<< BUILD_LOG( iRet )
				<< BUILD_LOG( kBillBuyInfo.m_wstrOrderID )
				<< BUILD_LOG( kPacket_.m_wstrTotalOrderID )
				<< BUILD_LOG( kPacket_.m_wstrSenderUserID )
				<< BUILD_LOG( KBaseServer::GetKObj()->GetServerGroupID() )
				<< BUILD_LOG( kPacket_.m_iSenderUnitUID )
				<< BUILD_LOGc( kPacket_.m_BuyCharInfo.m_cLevel )
				<< BUILD_LOG( kPacket_.m_iReceiverUserUID )
				<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
				<< BUILD_LOG( kBillBuyInfo.m_iProductNo )
				<< BUILD_LOG( kBillBuyInfo.m_usOrderQuantity )
				<< BUILD_LOG( kBillBuyInfo.m_iPoint )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_27;
		}
		else
		{
			// ���Ű� �����ߴٸ� Ŭ���̾�Ʈ�� ����� �����ϵ��� ����!
			KBillOrderInfo kOrderInfo;
			kOrderInfo.m_iTransNo					= iTrans;
			kOrderInfo.m_wstrOrderID				= kBillBuyInfo.m_wstrOrderID;
			kOrderInfo.m_kBillProductInfo.m_iProductNo = kBillBuyInfo.m_iProductNo;
			kOrderInfo.m_vecSocketOption;
			kOrderInfo.m_byteType					= KBillOrderInfo::BCIT_GIFT;
			kOrderInfo.m_wstrSenderNickName			= kPacket_.m_wstrSenderNickName;
			kOrderInfo.m_wstrSenderPresentMessage	= kPacket_.m_wstrPresentMessage;
			kOrderInfo.m_iFromUnitUID				= kPacket_.m_iSenderUnitUID;
			kPacket.m_vecBillOrderInfo.push_back( kOrderInfo );
		}
	}

	START_LOG( clog2, L"ebill buy product req" )
		<< BUILD_LOG( kPacket.m_vecBillOrderInfo.size() )
		<< END_LOG;

end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;

	kPacket.m_vecBillCouponItemInfo.clear();
	kPacket.m_iOK = UseCoupon( kPacket_, kPacket.m_vecBillCouponItemInfo );

	SendToUser( FIRST_SENDER_UID, EBILL_USE_COUPON_ACK, kPacket );
}


_IMPL_ON_FUNC( EBILL_USE_COUPON_RESERVE_REQ, KEBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_RESERVE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kPacketReq = kPacket_;

	// Ʈ����� ����
	__int64 iTransactionNo = 0;
	time_t iTime = 0;
	kPacket.m_iOK = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_GIANT_BILLING_05;
			break;
		}

		START_LOG( cerr, L"���� Ʈ����� ��ȣ ���� ���� (����)" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		goto end_proc;
	}

	MakeTID( iTransactionNo, iTime, kPacket.m_kPacketReq.m_wstrTransaction );

	// �ֹ� ���
	kPacket.m_iOK = DoQuery_CouponOrder( kPacket_.m_wstrSerialCode,
		kPacket.m_kPacketReq.m_wstrTransaction, 
		kPacket_.m_PurchaserInfo.m_iUserUID,
		kPacket_.m_PurchaserInfo.m_wstrUserName,
		kPacket_.m_PurchaserInfo.m_iServerGroupID,
		kPacket_.m_PurchaserInfo.m_iUnitUID,
		kPacket_.m_PurchaserInfo.m_ucLevel,
		kPacket_.m_PurchaserInfo.m_wstrIP//,
		//1			//KGiantBillingPacket::GB_RT_USE_ITEM_CARD // �߱� ���� ���� Default OrderType 1�� ������
		);

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�ۺ��� ���� �ֹ� ��� ����." )			
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrSerialCode )
			<< BUILD_LOG( kPacket.m_kPacketReq.m_wstrTransaction )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrUserName )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iServerGroupID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_PurchaserInfo.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrIP )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, EBILL_USE_COUPON_RESERVE_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_RESULT_REQ )
{
	KEBILL_USE_COUPON_RESULT_ACK kPacket;

	// m_iChargeItemCnt�� �ް� ������ ���� ������. ������ 1�� ó����

	kPacket.m_iOK = DoQuery_CouponOrderResult( kPacket_.m_wstrTransaction,
		kPacket_.m_iRet,
		kPacket_.m_iChargeItem,
		kPacket_.m_iChargeCash,
		kPacket_.m_iChargeBonus,
		kPacket.m_iTotalCash );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�ۺ��� ���� ��� DB ��� ����." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;
	}

	switch( kPacket_.m_iRet )
	{
	case KEBILL_USE_COUPON_REQ::GB_RCT_FAIL:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_09;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ACCOUNT_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_01;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_CASH_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_02;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_03;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_NOT_AREA:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_04;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_DUPLICATE:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_05;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ZONE_CARD_ID_ERROR:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_06;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ZONE_CARD_PASSWORD_ERROR:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_07;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_DB_ERROR:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_08;
		break;
	}

	START_LOG( clog, L"���� ��� ���" )
		<< BUILD_LOG( kPacket_.m_wstrTransaction )
		<< BUILD_LOG( kPacket_.m_iRet )
		<< BUILD_LOG( kPacket_.m_iChargeItem )
		<< BUILD_LOG( kPacket_.m_iChargeCash )
		<< BUILD_LOG( kPacket_.m_iChargeBonus )
		<< BUILD_LOG( kPacket.m_iOK )
		<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
		<< BUILD_LOG( kPacket.m_iTotalCash )
		<< END_LOG;

	SendToUser( kPacket_.m_iUserUID, EBILL_USE_COUPON_RESULT_ACK, kPacket );
}



#ifdef SERV_EVENT_BUY_FAKE_ITEM
IMPL_ON_FUNC( EBILL_CHECK_BUY_FAKE_ITEM_REQ )
{
	KEBILL_CHECK_BUY_FAKE_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_EVENT_BUY_FAKE_ITEM_UNIT
	DO_QUERY_NO_PROFILE( L"exec dbo.bup_get_Unit_Purchase_info", L"%d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_ulProductNo
		);

	int nPurchaseCount = 0;
	if( m_kODBC.BeginFetch() )
	{		
		FETCH_DATA( nPurchaseCount );
		m_kODBC.EndFetch();
	}

	START_LOG( clog, L"�輮��_ĳ���Ͱ�¥������1" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_ulProductNo )
		<< BUILD_LOG( nPurchaseCount )
		<< END_LOG;

	if( nPurchaseCount == 0 )
	{
		DO_QUERY_NO_PROFILE( L"exec dbo.bup_Update_Unit_Purchase_info_new", L"%d, %d, %d",
			% kPacket_.m_iUserUID
			% kPacket_.m_ulProductNo
			% 1
			);

		int iRet = -1;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iRet );						
			m_kODBC.EndFetch();
		}

		if( iRet == 0 )
			kPacket.m_iOK = NetError::NET_OK;

		START_LOG( clog, L"�輮��_ĳ���Ͱ�¥������2" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< BUILD_LOG( iRet )
			<< END_LOG;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
	}
#else //SERV_EVENT_BUY_FAKE_ITEM_UNIT
	DO_QUERY_NO_PROFILE( L"exec dbo.bup_get_User_Purchase_info", L"%d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_ulProductNo
		);

	int nPurchaseCount = 0;
	if( m_kODBC.BeginFetch() )
	{		
		FETCH_DATA( nPurchaseCount );
		m_kODBC.EndFetch();
	}

	START_LOG( clog, L"�輮��_������¥������1" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_ulProductNo )
		<< BUILD_LOG( nPurchaseCount )
		<< END_LOG;

	if( nPurchaseCount == 0 )
	{
		DO_QUERY_NO_PROFILE( L"exec dbo.bup_Update_User_Purchase_info_new", L"%d, %d, %d",
			% kPacket_.m_iUserUID
			% kPacket_.m_ulProductNo
			% 1
			);

		int iRet = -1;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iRet );						
			m_kODBC.EndFetch();
		}

		if( iRet == 0 )
			kPacket.m_iOK = NetError::NET_OK;

		START_LOG( clog, L"�輮��_������¥������2" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< BUILD_LOG( iRet )
			<< END_LOG;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
	}
#endif //SERV_EVENT_BUY_FAKE_ITEM_UNIT

end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_CHECK_BUY_FAKE_ITEM_ACK, kPacket );
}
#endif //SERV_EVENT_BUY_FAKE_ITEM


//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
IMPL_ON_FUNC( EBILL_REWARD_COUPON_JP_EVENT_REQ )
{
	KEBILL_REWARD_COUPON_JP_EVENT_ACK kPacket;
	std::wstring wstrTransaction;

	//////////////////////////////////////////////////////////////////////////
	// ������ ��ü������ ��������!
	{
		kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_ODBC_01;
		kPacket.m_kRewardInfo = kPacket_.m_kRewardInfo;

		// Ʈ����� ����
		__int64 iTransactionNo = 0;
		time_t iTime = 0;
		kPacket.m_kUseCouponPacket.m_iOK = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

		if( kPacket.m_kUseCouponPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_kUseCouponPacket.m_iOK )
			{
			case -1:
				kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_05;
				break;
			}

			START_LOG( cerr, L"���� Ʈ����� ��ȣ ���� ���� (����)" )
				<< BUILD_LOG( kPacket.m_kUseCouponPacket.m_iOK )
				<< END_LOG;
			goto end_proc;
		}		

		MakeTID( iTransactionNo, iTime, wstrTransaction );

		// �ֹ� ���
		kPacket.m_kUseCouponPacket.m_iOK = DoQuery_CouponOrder( kPacket_.m_kUseCouponPacket.m_wstrSerialCode,
			wstrTransaction, 
			kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_iUserUID,
			kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_wstrUserName,
			kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_iServerGroupID,
			kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_iUnitUID,
			kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_ucLevel,
			kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_wstrIP//,
			//1			//KGiantBillingPacket::GB_RT_USE_ITEM_CARD // �߱� ���� ���� Default OrderType 1�� ������
			);

		if( kPacket.m_kUseCouponPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"�ۺ��� ���� �ֹ� ��� ����." )			
				<< BUILD_LOG( kPacket.m_kUseCouponPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_wstrSerialCode )
				<< BUILD_LOG( wstrTransaction )
				<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_wstrUserName )
				<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_iServerGroupID )
				<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_iUnitUID )
				<< BUILD_LOGc( kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_ucLevel )
				<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_PurchaserInfo.m_wstrIP )
				<< END_LOG;

			goto end_proc;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���� ����� ����!
	{
		// m_iChargeItemCnt�� �ް� ������ ���� ������. ������ 1�� ó����
		int iRet = KEBILL_USE_COUPON_REQ::GB_RCT_SUCCESS;
		int iTotalCash = 0;
		int iDummyZero = 0;

		kPacket.m_kUseCouponPacket.m_iOK = DoQuery_CouponOrderResult( wstrTransaction,
			iRet,								// �ֹ� ���
			kPacket_.m_kRewardInfo.m_iRewardID,	// ���� ������ ������ID
			iDummyZero,							// ĳ�� ���� ����
			iDummyZero,							// ������ ���ʽ� ����
			iTotalCash );						// ��ü ĳ�� ������ ����

		if( kPacket.m_kUseCouponPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"�ۺ��� ���� ��� DB ��� ����." )
				<< BUILD_LOG( kPacket.m_kUseCouponPacket.m_iOK )
				<< BUILD_LOG( NetError::GetErrStr( kPacket.m_kUseCouponPacket.m_iOK ) )
				<< END_LOG;
		}

		switch( iRet )
		{
		case KEBILL_USE_COUPON_REQ::GB_RCT_FAIL:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_09;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_ACCOUNT_NOT_EXIST:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_01;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_CASH_NOT_EXIST:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_02;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_NOT_EXIST:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_03;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_NOT_AREA:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_04;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_DUPLICATE:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_05;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_ZONE_CARD_ID_ERROR:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_06;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_ZONE_CARD_PASSWORD_ERROR:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_07;
			break;
		case KEBILL_USE_COUPON_REQ::GB_RCT_DB_ERROR:
			kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_GIANT_BILLING_08;
			break;
		}

		START_LOG( clog, L"���� ��� ���" )
			<< BUILD_LOG( wstrTransaction )
			<< BUILD_LOG( iRet )
			<< BUILD_LOG( kPacket_.m_kRewardInfo.m_iRewardID )
			<< BUILD_LOG( kPacket.m_kUseCouponPacket.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr(kPacket.m_kUseCouponPacket.m_iOK) )
			<< END_LOG;
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ �ٷ� �������!
	//{
	//	KEBILL_REWARD_COUPON_JP_EVENT_ACK kPacket;
	//	kPacket.m_kRewardInfo = kPacket_.m_kRewardInfo;
	//	kPacket.m_kUseCouponPacket.m_iOK = NetError::ERR_UNKNOWN;

	//	kPacket.m_kUseCouponPacket.m_vecBillCouponItemInfo.clear();
	//	kPacket.m_kUseCouponPacket.m_iOK = UseCoupon( kPacket_.m_kUseCouponPacket, kPacket.m_kUseCouponPacket.m_vecBillCouponItemInfo );
	//}	
	
end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_REWARD_COUPON_JP_EVENT_ACK, kPacket );
}
#endif SERV_RELAY_DB_CONNECTION
//}}

#ifdef SERV_CONTENT_MANAGER_INT
IMPL_ON_FUNC( DBE_GET_CASHSHOP_ON_OFF_INFO_REQ )
{
	KDBE_GET_CASHSHOP_ON_OFF_INFO_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_ODBC_01;
	kPacketAck.m_iReleaseTick = kPacket_.m_iReleaseTick;
	kPacketAck.m_bEnableCashshop = kPacket_.m_bEnableCashshop;

	DO_QUERY_NO_ARG( L"exec dbo.EBP_ReleaseTick_SEL" );

	while( m_kODBC.Fetch() )
	{
		int iReleaseTickType = 0;
		int iReleaseTick = 0;

		FETCH_DATA( iReleaseTick
			>> iReleaseTickType );

		if( iReleaseTickType != CSRTT_CASHSHOP_ON_OFF )
			continue;

		if( iReleaseTick > kPacket_.m_iReleaseTick )
		{
			kPacketAck.m_iReleaseTick = iReleaseTick;
		}
	}

	if( kPacketAck.m_iReleaseTick > kPacket_.m_iReleaseTick || kPacket_.m_bFirstCashshopOnOffCheck == true )
	{
		DO_QUERY_NO_ARG( L"exec dbo.EBP_CashshopOnOff_SEL" );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacketAck.m_bEnableCashshop );
			m_kODBC.EndFetch();
		}
	}

	kPacketAck.m_iOK = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( kPacketAck.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( kPacketAck.m_iOK )
		<< END_LOG;

	SendToServer( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK, kPacketAck );
}
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_COUNTRY_PH

IMPL_ON_FUNC( EBILL_GARENA_PREPARE_PRESENT_CHECK_REQ )
{
	KEBILL_GARENA_PREPARE_PRESENT_CHECK_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;


	int iPreparePresentCheck = -1;

	DO_QUERY( L"exec dbo.EBP_GiftLimit_CHK",  L"%d", % kPacket_.m_iUserUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iPreparePresentCheck
			);

		m_kODBC.EndFetch();
	}

	if (iPreparePresentCheck != NetError::NET_OK)
	{
		kPacket.m_iOK = NetError::ERR_GARENA_BILLING_01;
		
	}
	else
	{
		kPacket.m_kEGSPresentCashItemREQ = kPacket_.m_kEGSPresentCashItemREQ;
		kPacket.m_iOK = NetError::NET_OK;
	}



end_proc:
	SendToUser( FIRST_SENDER_UID, EBILL_GARENA_PREPARE_PRESENT_CHECK_ACK, kPacket );
}

#endif //SERV_COUNTRY_PH