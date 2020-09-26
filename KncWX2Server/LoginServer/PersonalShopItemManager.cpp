#include "LoginServer.h"
#include ".\PersonalShopItemManager.h"
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLItem.h"
#include "X2Data/XSLSquareUnit.h"
#include "Enum/Enum.h"

#include "NetError.h"


//{{ 2011. 04. 20	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY


KPersonalShopItemManager::KPersonalShopItemManager(void)
{
}

KPersonalShopItemManager::~KPersonalShopItemManager(void)
{
	Clear();
}

void KPersonalShopItemManager::Clear()
{
	m_vecSellItemInfo.clear();
	m_mapSellItemInfo.clear();
	m_mapSoldPShopAgencyItemInfo.clear();
}

bool KPersonalShopItemManager::IsExistItem( IN const UidType iItemUID ) const
{
    return ( m_mapSellItemInfo.find( iItemUID ) != m_mapSellItemInfo.end() );
}

//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
//////////////////////////////////////////////////////////////////////////
void KPersonalShopItemManager::GetSellPersonalShopItemInfoList( OUT std::vector< KSellPersonalShopItemInfo >& vecItemList, IN const ITEM_LIST_TYPE eType /*= ITEM_LIST_TYPE::ILT_ONLY_ON_SALE_ITEMS*/ )
{
	vecItemList.clear();

	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"������ ������ �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴմϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( spItemInfo->GetItemUID() )
				<< END_LOG;
			continue;
		}

		switch( eType )
		{
		case ILT_ALL_ITEMS:
			break;

		case ILT_ONLY_ON_SALE_ITEMS:
			{
				// �� �ȸ����� ����Ʈ�� ���� �ʴ´�!
				if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SOLD_OUT  ||
					spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
					continue;
			}
			break;

		case ILT_ONLY_SOLD_OUT_ITEMS:
			{
				// �� �ȸ����� ����Ʈ�� ���� �ʴ´�!
				if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SALE  ||
					spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PROCESSING )
					continue;
			}
			break;

		default:
			{
				START_LOG( cerr, L"��ǰ ����Ʈ ��û Ÿ���� �̻��մϴ�!" )
					<< BUILD_LOG( eType )
					<< END_LOG;
			}
			return;
		}

		KSellPersonalShopItemInfo kInfo;
		spItemInfo->GetSellPersonalShopItemInfo( kInfo );
		vecItemList.push_back( kInfo );
	}
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
void KPersonalShopItemManager::GetSellPersonalShopItemInfoList( OUT std::vector< KSellPersonalShopItemInfo >& vecItemList, IN const bool bAllItems /*= false*/ )
{
	vecItemList.clear();

	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"������ ������ �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴմϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( spItemInfo->GetItemUID() )
				<< END_LOG;
			continue;
		}

		if( bAllItems == false )
		{
			// �� �ȸ����� ����Ʈ�� ���� �ʴ´�!
			if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SOLD_OUT )
				continue;
		}

		KSellPersonalShopItemInfo kInfo;
		spItemInfo->GetSellPersonalShopItemInfo( kInfo );
		vecItemList.push_back( kInfo );
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_PSHOP_AGENCY_NEW
//}}

bool KPersonalShopItemManager::GetSellPersonalShopItemInfo( IN const UidType iItemUID, OUT KSellPersonalShopItemInfo& kItemInfo )
{
	KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"�������� �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	spItemInfo->GetSellPersonalShopItemInfo( kItemInfo );
	return true;
}

KPersonalShopItemPtr KPersonalShopItemManager::GetPersonalShopItem( IN const UidType iItemUID )
{
	std::map< UidType, KPersonalShopItemPtr >::iterator mit;
	mit = m_mapSellItemInfo.find( iItemUID );
	if( mit == m_mapSellItemInfo.end() )
	{
		START_LOG( cwarn, L"������ ������ �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( m_mapSellItemInfo.size() );

		return KPersonalShopItemPtr();
	}

	return mit->second;
}

bool KPersonalShopItemManager::AddSellItemInfo( IN const KSellPersonalShopItemInfo& kInfo )
{
	if( IsExistItem( kInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
	{
		START_LOG( cerr, L"�̹� �����ϴ� ������ �Դϴ�!" )
			<< BUILD_LOG( kInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;
	}

	KPersonalShopItemPtr spItemInfo = KPersonalShopItemPtr( new KPersonalShopItem( kInfo ) );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"������ ���� ������ �����Ͽ����ϴ�." )
			<< BUILD_LOG( kInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;        
	}

	m_mapSellItemInfo.insert( std::make_pair( kInfo.m_kInventoryItemInfo.m_iItemUID, spItemInfo ) );
	m_vecSellItemInfo.push_back( spItemInfo );
	return true;
}

bool KPersonalShopItemManager::AddSellItemInfo( IN const KSellPShopItemBackupData& kInfo )
{
	if( IsExistItem( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
	{
		START_LOG( cerr, L"�̹� �����ϴ� ������ �Դϴ�!" )
			<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;
	}

	LIF( kInfo.m_kSellPShopItemInfo.m_cPShopItemType == KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY );

	KPersonalShopItemPtr spItemInfo = KPersonalShopItemPtr( new KPersonalShopItem( kInfo ) );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"������ ���� ������ �����Ͽ����ϴ�." )
			<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;
	}

	m_mapSellItemInfo.insert( std::make_pair( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID, spItemInfo ) );
	m_vecSellItemInfo.push_back( spItemInfo );
	return true;
}

void KPersonalShopItemManager::DelSellItemInfo( IN const UidType iItemUID )
{
	LIF( IsExistItem( iItemUID ) );
    
	// vector�����̳ʿ��� ����
	std::vector< KPersonalShopItemPtr >::iterator vit;
	for( vit = m_vecSellItemInfo.begin(); vit != m_vecSellItemInfo.end(); ++vit )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"������ ������ ���� �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		if( (*vit)->GetItemUID() == iItemUID )
			break;
	}

	if( vit != m_vecSellItemInfo.end() )
	{
		m_vecSellItemInfo.erase( vit );
	}

	// map�����̳ʿ��� ����
    m_mapSellItemInfo.erase( iItemUID );
}

void KPersonalShopItemManager::UpdateSellItemList( IN const std::vector< KSellPersonalShopItemInfo >& vecSellItemList )
{
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kInfo, vecSellItemList )
	{
		if( IsExistItem( kInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
		{
			START_LOG( cerr, L"�ߺ��� �������� �ֽ��ϴ�. ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( kInfo.m_kInventoryItemInfo.m_iItemUID )
				<< END_LOG;			
			continue;
		}

		AddSellItemInfo( kInfo );
	}
}

void KPersonalShopItemManager::UpdateSellItemBackupDataList( IN const std::vector< KSellPShopItemBackupData >& vecSellItemList )
{
	BOOST_TEST_FOREACH( const KSellPShopItemBackupData&, kInfo, vecSellItemList )
	{
		if( IsExistItem( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
		{
			START_LOG( cerr, L"DB�κ��� ���� ��� ������ �����͸� �ִµ� �ߺ��Ȱ� �ִ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< END_LOG;
			continue;
		}

		if( kInfo.m_kSellPShopItemInfo.m_cPShopItemType != KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUIDŸ���� �ƴմϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOGc( kInfo.m_kSellPShopItemInfo.m_cPShopItemType )
				<< END_LOG;
            continue;
		}

		AddSellItemInfo( kInfo );
	}
}

bool KPersonalShopItemManager::PrepareForBuyItem( IN const UidType iHostUnitUID, IN const char cPersonalShopType, IN const KERM_BUY_PERSONAL_SHOP_ITEM_REQ& kReq, OUT KDBE_BUY_PSHOP_AGENCY_ITEM_REQ& kResult )
{
	SET_ERROR( NET_OK );

	// ����� �������� ã�´�.
	KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( kReq.m_iBuyItemUID );
	if( spItemInfo == NULL )
	{
		// �ð����� �߻��� �� �ִ� ������!
		START_LOG( cwarn, L"�������� �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( kReq.m_iBuyItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_15 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �غ�

	// 1. �븮���� ���� ItemUID�� �´��� �˻�
	if( spItemInfo->IsPShopAgencyItemUID() == false )
	{
		START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴմϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOGc( spItemInfo->GetItemUID() )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_15 );
		return false;
	}

	// 1. �ش� �������� ���� ���� ������ �������� �˻�
	if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_SALE )
	{
		START_LOG( clog, L"���� �ش� �������� ���� ������ ���°� �ƴմϴ�!" )
			<< BUILD_LOGc( spItemInfo->GetShopItemState() )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_25 );
		return false;
	}

	// 1. �ŷ� ������ ī�װ��� ����������?
	if( CXSLInventory::IsTradePossibleCategory( spItemInfo->GetSlotCategory() ) == false )
	{
		START_LOG( cerr, L"�Ǹ����� �������� �ŷ������� ī�װ��� �ƴϴ�." )
			<< BUILD_LOGc( spItemInfo->GetSlotCategory() )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_17 );
		return false;
	}

	// 2. ���� ���� ��û �˻�
	if( spItemInfo->GetUsageType() != CXSLItem::PT_QUANTITY )
	{
		if( kReq.m_iBuyQuantity > 1 )
		{
			START_LOG( cerr, L"������ �������� �ƴѵ� ��� ������ 1�̻���. �����ΰ�? ��ŷ�ΰ�?" )
				<< BUILD_LOGc( spItemInfo->GetUsageType() )
				<< BUILD_LOG( kReq.m_iBuyQuantity )
				<< END_LOG;
		}
	}

	// 3. ���� �˻�
	if( spItemInfo->GetQuantity() < kReq.m_iBuyQuantity )
	{
		START_LOG( cerr, L"�ǸŹ�ǰ�� ������." )
			<< BUILD_LOG( spItemInfo->GetQuantity() )
			<< BUILD_LOG( kReq.m_iBuyQuantity )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_14 );
		return false;
	}

	// 4. ���� �׼��� �Ѱ�ġ �˻�
	__int64 iPricePerOne  = static_cast<__int64>(spItemInfo->GetTotalPricePerOne());
	__int64 iItemQuantity = static_cast<__int64>(kReq.m_iBuyQuantity);
	__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

	if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > SEnum::UI_MAX_ED_CHAR_HAVE )
	{
		START_LOG( cout, L"���� �׼����� �Ѱ�ġ�� �Ѿ��. ���������� �ǽɵ�.." )
			<< BUILD_LOG( iPricePerOne )
			<< BUILD_LOG( iItemQuantity )
			<< BUILD_LOG( iCheckTotalPrice );

		SET_ERROR( ERR_PERSONAL_SHOP_24 );
		return false;
	}

	// 5. ���� �˻�..
	int iTotalPrice = (spItemInfo->GetTotalPricePerOne() * kReq.m_iBuyQuantity);
#ifdef DEF_TRADE_BOARD	
	//{{ 2011. 07. 19	������	�븮���� ���� ������ ���� �̺�Ʈ
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	// �̺�Ʈ �Ⱓ ���ȿ��� �����ᰡ ����!
#else
	//iTotalPrice += static_cast<int>(iTotalPrice * TD_COMMISSION); // �ŷ��Խ����� ���ؼ� ������ ���� ������ 3%�� ���δ�.
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}	
#endif DEF_TRADE_BOARD

	// 6. �����ڰ� ��ǰ�� ED�� �ִ��� �˻�.
	if( iTotalPrice > kReq.m_iBuyUnitNowED )
	{
		START_LOG( clog, L"ED�� �����Ͽ� ��ǰ�� �����Ҽ� ����.!" )
			<< BUILD_LOG( iTotalPrice )
			<< BUILD_LOG( kReq.m_iBuyUnitNowED );

		SET_ERROR( ERR_PERSONAL_SHOP_16 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Temp���� ����
	{
		//#3. GSUser����� ���� ó�� ����
		kResult.m_kItemQuantityUpdateTemp = kReq.m_kItemQuantityUpdateTemp;

		// 7. ���ż������� ED���� ���Ͽ� �ӽ� ����
		kResult.m_iEDTemp = kReq.m_iBuyUnitNowED - iTotalPrice;

		// 8. �ѹ��� ���Ű����� �������� �ִ� 1������
		kResult.m_vecItemInfoTemp = kReq.m_vecItemInfoTemp;

		if( kResult.m_vecItemInfoTemp.size() > 1 )
		{
			START_LOG( cerr, L"�ѹ��� ���Ű����� ������ ������ ���������ε�? �Ͼ�� ���� ����!" )
				<< BUILD_LOG( kResult.m_vecItemInfoTemp.size() )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_05 );
			return false;
		}
		else if( kResult.m_vecItemInfoTemp.size() == 1 )
		{
			//{{ 2009. 2. 12  ������	���λ��� ED���� ����
			bool bIsBuyItemExist = false;
			BOOST_TEST_FOREACH( KItemInfo&, kBuyItemInfoTemp, kResult.m_vecItemInfoTemp )
			{
				if( kBuyItemInfoTemp.m_iItemID == spItemInfo->GetItemID() )
				{
					// GSUser�Ҹ�� ���� ������Ʈ������� DB�� �� ������ ���� ����
					spItemInfo->GetItemInfo( kBuyItemInfoTemp );

					// ������ ����
					kBuyItemInfoTemp.m_iQuantity = kReq.m_iBuyQuantity;

					bIsBuyItemExist = true;
					break;
				}
			}

			if( bIsBuyItemExist == false )
			{
				START_LOG( cerr, L"�������� �������� ����." )
					<< BUILD_LOG( kReq.m_iBuyQuantity )
					<< BUILD_LOG( kReq.m_iBuyItemUID )
					<< END_LOG;

				SET_ERROR( ERR_PERSONAL_SHOP_05 );
				return false;
			}
			//}}
		}
	}

	// ������ ���� ó��
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	KSellPShopItemDBUpdateInfo kSellItemTemp;
	if( spItemInfo->BuyItem( cPersonalShopType, 
		kReq.m_iBuyQuantity, 
		kResult.m_iSellUnitEDIn, 
		kResult.m_iPriceUnitEDOUT, 
		kResult.m_iPriceCommissionED, 
		kResult.m_kPriceIntemInfoIN,
		kSellItemTemp ) == false )
#else
	if( spItemInfo->BuyItem( cPersonalShopType, 
		kReq.m_iBuyQuantity, 
		kResult.m_iSellUnitEDIn, 
		kResult.m_iPriceUnitEDOUT, 
		kResult.m_iPriceCommissionED, 
		kResult.m_kPriceIntemInfoIN,
		kResult.m_kSellItemDBUpdate ) == false )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	{
		START_LOG( cerr, L"�������� �������� ����." )
			<< BUILD_LOG( kReq.m_iBuyQuantity )
			<< BUILD_LOG( kReq.m_iBuyItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_05 );
		return false;
	}

	return true;
}

bool KPersonalShopItemManager::Confirm( IN const UidType iItemUID, IN const bool bSuccess )
{
	// ����� �������� ã�´�.
	KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"�������� �������� �ʽ��ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}
	
	spItemInfo->Confirm( bSuccess );
    return true;
}

bool KPersonalShopItemManager::IsAllItemSoldOut() const
{
	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"������ ������ �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_SOLD_OUT )
            return false;
	}

	return true;
}

//{{ 2011. 05. 24	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
bool KPersonalShopItemManager::IsAllItemOnSale() const
{
	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"������ ������ �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SOLD_OUT  ||
			spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
			return false;
	}

	return true;
}
#endif SERV_PSHOP_AGENCY_NEW
//}}

bool KPersonalShopItemManager::PrepareForPickUp( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ& kResult )
{
	SET_ERROR( NET_OK );

	kResult.m_vecPickUpItemList = vecPickUpItemList;
	kResult.m_mapPickUpItemInfo.clear();
	kResult.m_iTotalAddED = 0;

	// �� ������ �������� ������!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"�������� �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_52 );
			return false;
		}

		// �븮���� ���� ItemUID�� �ƴ϶�� PickUp�Ҽ� ����!
		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴմϴ�! ���� �Ͼ�� ���� ����!" ) 
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_47 );
			return false;
		}

		// ���� �ǸŰ� �������� �������̶�� �����ü� ����!
		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PROCESSING )
		{
            SET_ERROR( ERR_PERSONAL_SHOP_48 );
			return false;
		}

		// �׿ܿ� �����ü� ���� ����
		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_49 );
			return false;
		}

		// ������ ���� ���
		const int iItemID = spItemInfo->GetItemID();
		const int iQuantity = spItemInfo->GetQuantity();

		std::pair< std::map< int, int >::const_iterator, bool > pairResult;
		pairResult = kResult.m_mapPickUpItemInfo.insert( std::make_pair( iItemID, iQuantity ) );
		if( pairResult.second == false )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_53 );
			return false;
		}

		kResult.m_iTotalAddED += spItemInfo->GetTotalSellEDIn();
	}

	return true;
}

bool KPersonalShopItemManager::PickUp( IN const std::vector< UidType >& vecPickUpItemList, OUT KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ& kResult )
{
	SET_ERROR( NET_OK );

	// �� ������ �������� ������!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"�������� �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_52 );
			return false;
		}

		// �븮���� ���� ItemUID�� �ƴ϶�� �����ü� ����!
		if( spItemInfo->IsPShopAgencyItemUID() == false  )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴѵ� �������⸦ �Ϸ��� �մϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_47 );
			return false;
		}

		// ���� �ǸŰ� �������� �������̶�� �����ü� ����!		
		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PROCESSING )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_48 );
			return false;
		}

		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_49 );
			return false;
		}
	}

	// pick up ���·� �ٲ���!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"�������� �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}
		
		spItemInfo->SetShopItemState( KPersonalShopItem::SIS_PICK_UP );

		//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		KSellPersonalShopItemInfo kItem;
		spItemInfo->GetSellPersonalShopItemInfo( kItem );
		kResult.m_vecPickUpItemList.push_back( kItem );
#else
		kResult.m_vecPickUpItemList.push_back( iItemUID );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
	}

	return true;
}

void KPersonalShopItemManager::UnPickUp( IN const std::vector< UidType >& vecPickUpFailList )
{
	// �� ������ �������� ������!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpFailList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"�������� �������� �ʽ��ϴ�! �̹� �˻� �ߴµ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// �븮���� ���� ItemUID�� �ƴ϶�� PickUp�Ҽ� ����!
		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴմϴ�! ���� �Ͼ�� ���� ����!" ) 
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
		}

		// ���� �ǸŰ� �������� �������̶�� �����ü� ����!
		if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_PICK_UP )
		{
			START_LOG( cerr, L"Pick Up ���°� �ƴѵ� Pick Up��Ҹ� ��û�ϳ�?" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// ���� ������ ���¿� �°� ��������!
		if( spItemInfo->GetQuantity() > 0 )
		{
			spItemInfo->SetShopItemState( KPersonalShopItem::SIS_SALE );
		}
		else
		{
			spItemInfo->SetShopItemState( KPersonalShopItem::SIS_SOLD_OUT );
		}
	}
}

//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
void KPersonalShopItemManager::PickUpComplete( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK& kResult, IN const ITEM_LIST_TYPE eType )
#else
void KPersonalShopItemManager::PickUpComplete( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK& kResult )
#endif SERV_PSHOP_AGENCY_NEW
//}}
{
	kResult.m_iTotalSellEDIn = 0;
	kResult.m_vecPickUpSuccessItemList.clear();
	kResult.m_vecSellItemInfo.clear();

	// �� ������ �������� ������!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"�������� �������� �ʽ��ϴ�! �̹� �˻� �ߴµ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// �븮���� ���� ItemUID�� �ƴ϶�� PickUp�Ҽ� ����!
		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"�븮���� ���� ItemUID�� �ƴմϴ�! ���� �Ͼ�� ���� ����!" ) 
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
		}

		// ���� �ǸŰ� �������� �������̶�� �����ü� ����!
		if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_PICK_UP )
		{
			START_LOG( cerr, L"Pick Up ���°� �ƴѵ� Pick Up��Ҹ� ��û�ϳ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// �ǸŴ���� ��������!
		kResult.m_iTotalSellEDIn += spItemInfo->GetTotalSellEDIn();

		// ������ ������ ����!
		KSellPersonalShopItemInfo kInfo;
		spItemInfo->GetSellPersonalShopItemInfo( kInfo );
		//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		DelSellItemInfo( iItemUID );
#else
		kResult.m_vecPickUpSuccessItemList.push_back( kInfo );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
	}

	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	// ������ ��� �����۵��� ��������!
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellItemInfo, kResult.m_vecPickUpSuccessItemList )
	{
		DelSellItemInfo( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID );
	}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// ���� �Ǹ����� ������ ������ ����!
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW	
	GetSellPersonalShopItemInfoList( kResult.m_vecSellItemInfo, eType );
#else
	GetSellPersonalShopItemInfoList( kResult.m_vecSellItemInfo, true );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	
}

#endif SERV_PSHOP_AGENCY
//}}


