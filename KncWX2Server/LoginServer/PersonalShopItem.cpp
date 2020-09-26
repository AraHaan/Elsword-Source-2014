#include ".\PersonalShopItem.h"
#include "X2Data/XSLSquareUnit.h"

//{{ 2011. 04. 25	������	�븮����
#ifdef SERV_PSHOP_AGENCY


KPersonalShopItem::KPersonalShopItem( const KSellPersonalShopItemInfo& kInfo )
{
	// �ʱ�ȭ
	Clear();

	// ������ ���� ������Ʈ
	SetSellPersonalShopItemInfo( kInfo );
}

KPersonalShopItem::KPersonalShopItem( const KSellPShopItemBackupData& kInfo )
{
	// �ʱ�ȭ
	Clear();

	// DB��������� ������Ʈ
	SetSellPersonalShopItemInfo( kInfo.m_kSellPShopItemInfo );

	// PShopItemUID����
	SetPShopAgencyItemUID( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID );

	// ���� ������ ������ 0���� �� �ȸ��Ŵ�!
	if( GetQuantity() == 0 )
	{
		SetShopItemState( SIS_SOLD_OUT );
	}

	m_iTotalSoldItemQuantity = kInfo.m_iTotalSoldItemQuantity;
	m_iTotalSellEDIn		 = kInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn;
	m_iTotalSellCommissionED = kInfo.m_iTotalSellCommissionED;

	// �븮���� DB ���̺��� �ҷ����� ������ �̹Ƿ� �븮���� ���� ItemUID��߸� �մϴ�.
	if( IsPShopAgencyItemUID() == false )
	{
		START_LOG( cerr, L"DB���� �ҷ��Դµ� �� �븮���� ���� ItemUID�� �ƴ���?" )
			<< BUILD_LOG( GetItemUID() )
			<< END_LOG;
	}
}

KPersonalShopItem::~KPersonalShopItem(void)
{
}

void KPersonalShopItem::Clear()
{
	m_iTotalSoldItemQuantity		 = 0;
	m_iTotalSellEDIn				 = 0;
	m_iTotalSellCommissionED		 = 0;
	m_eItemState					 = SIS_SALE;
	m_iWaitForConfirmQuantity		 = 0;
	m_iWaitForConfirmSellED			 = 0;
	m_iWaitForConfirmSellCommisionED = 0;
}

void KPersonalShopItem::GetSellPersonalShopItemInfo( OUT KSellPersonalShopItemInfo& kInfo ) const
{
    kInfo = m_kSellItemInfo;
	kInfo.m_iTotalSellEDIn = m_iTotalSellEDIn;
	kInfo.m_iTotalSoldItemQuantity = m_iTotalSoldItemQuantity;
}

void KPersonalShopItem::SetPShopAgencyItemUID( IN const UidType iItemUID )
{
	// �븮���� ���� ItemUID�� �޽��ϴ�
	m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID = iItemUID;
}

bool KPersonalShopItem::BuyItem( IN const char cPersonalShopType, 
								 IN const int iBuyQuantity, 
								 OUT int& iSellUnitEDIn,
								 OUT int& iPriceUnitEDOUT, 
								 OUT int& iPriceCommissionED, 
								 OUT KInventoryItemInfo& kPriceIntemInfoIN,
								 OUT KSellPShopItemDBUpdateInfo& kSellItemDBUpdateInfo )
{
	if( IsPShopAgencyItemUID() == false )
	{
		START_LOG( cerr, L"�� �������� ���� �븮���� ���� ItemUID�� �߱޵��� �ʾҽ��ϴ�. ����� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetItemUID() )
			<< END_LOG;
		return false;
	}

	if( GetShopItemState() != SIS_SALE )
	{
		START_LOG( cerr, L"�ش� �������� ���� ���Ű����� ���°� �ƴմϴ�!" )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( GetShopItemState() )
			<< END_LOG;
		return false;
	}

	if( iBuyQuantity > GetQuantity() )
	{
		START_LOG( cerr, L"�����Ϸ��� ������ŭ ������ ������ �������� �ʽ��ϴ�! �Ͼ�� ���� ����!" )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( iBuyQuantity )
			<< BUILD_LOG( GetQuantity() )
			<< END_LOG;
		return false;
	}

	// 1. �Ǹ��ڸ� ���� �ش� ��ǰ�� �Ǹ� �ݾ��� ���Ѵ�
	int iResultSellUnitEDIN = GetPricePerOne() * iBuyQuantity;			// �Ǹ��ڰ� ���� �ݾ�
	int iiSellCommissionED = 0;

	// �Ǹ��ڰ� ���� �ݾ� ����
	iSellUnitEDIn = iResultSellUnitEDIN;

	// 2. �����ڸ� ���� ���� ����
	iPriceUnitEDOUT = m_kSellItemInfo.TotalPricePerOne() * iBuyQuantity;		// �����ڰ� �����ؾ��� �ݾ�
#ifdef DEF_TRADE_BOARD	
	//{{ 2011. 07. 19	������	�븮���� ���� ������ ���� �̺�Ʈ
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	// �̺�Ʈ �Ⱓ ���ȿ��� �����ᰡ ����!
	iPriceCommissionED = 0;
#else
	//iPriceCommissionED = static_cast<int>(iPriceUnitEDOUT * TD_COMMISSION);		// �ŷ��Խ��� ������
	//iPriceUnitEDOUT += iPriceCommissionED;										// ���� �ݾ׿� ������ �����ϱ�
    iPriceCommissionED = 0;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}	
#endif DEF_TRADE_BOARD
	kPriceIntemInfoIN = m_kSellItemInfo.m_kInventoryItemInfo;					// �����ڿ��� ���� ���� ������ ����
	kPriceIntemInfoIN.m_kItemInfo.m_iQuantity = iBuyQuantity;

	//////////////////////////////////////////////////////////////////////////
	// �Ǹž����� ����ó�� & �������� �����鿡�� �ǸŻ�ǰ ���� ������Ʈ ó��.

	// ���� �Ǳ� ���� ���� �����صд�.
	m_iWaitForConfirmQuantity = iBuyQuantity;
	m_iWaitForConfirmSellED = iResultSellUnitEDIN;
	m_iWaitForConfirmSellCommisionED = iiSellCommissionED;

	// �Ǹŵǰ� �ִ� ������ ������ ��´�!
	const int iBeforeQuantity = GetQuantity();
	m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity -= iBuyQuantity;
	LIF( m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity >= 0 );

	// �Ǹ��ڰ� ���� �ݾ��� �����Ѵ�.
	m_iTotalSoldItemQuantity += iBuyQuantity;
	m_iTotalSellEDIn += iResultSellUnitEDIN;
	m_iTotalSellCommissionED += iiSellCommissionED;

	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	//////////////////////////////////////////////////////////////////////////	
	kSellItemDBUpdateInfo.m_iItemUID = GetItemUID();
	kSellItemDBUpdateInfo.m_iQuantity = GetQuantity() - iBeforeQuantity; // ���а�!
	kSellItemDBUpdateInfo.m_iTotalSellEDIn = m_iTotalSellEDIn;
	kSellItemDBUpdateInfo.m_iTotalSoldItemQuantity = m_iTotalSoldItemQuantity;
	kSellItemDBUpdateInfo.m_iTotalSellCommissionED = m_iTotalSellCommissionED;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
    
	// �Ǹ� ó�� �� ���·� ����!
	SetShopItemState( SIS_PROCESSING );
	return true;
}

void KPersonalShopItem::Confirm( IN const bool bSuccess )
{
	LIF( IsPShopAgencyItemUID() );

	if( bSuccess == false )
	{
		// ������ �����ϸ� �ѹ�ó��!
		m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity += m_iWaitForConfirmQuantity;
		m_iTotalSoldItemQuantity -= m_iWaitForConfirmQuantity;
		m_iTotalSellEDIn -= m_iWaitForConfirmSellED;
		m_iTotalSellCommissionED -= m_iWaitForConfirmSellCommisionED;
	}

	// ���� ��� �����ʹ� �ʱ�ȭ!
	m_iWaitForConfirmQuantity = 0;
	m_iWaitForConfirmSellED = 0;
	m_iWaitForConfirmSellCommisionED = 0;

	// �ٽ� �Ǹ� ���·� ����!
	if( GetQuantity() > 0 )
	{
		SetShopItemState( SIS_SALE );
	}
	else
	{
		SetShopItemState( SIS_SOLD_OUT );
	}
}


#endif SERV_PSHOP_AGENCY
//}}

