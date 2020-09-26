#include ".\personalshopitemlist.h"

//{{ 2010. 12. 16  ��ȿ��	���� ���� ������ �˻� �� ��ҹ��� ����
#ifdef SERV_STRING_CHECK_IGNORE_CASE
#include <boost/algorithm/string/find.hpp>
#endif SERV_STRING_CHECK_IGNORE_CASE
//}}
#include <boost/test/utils/foreach.hpp>
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLItemManager.h"
#include "NetError.h"

//{{ 2010. 03. 24  ������	defineó��
#ifdef DEF_TRADE_BOARD

ImplementSingleton( KPersonalShopItemList );

KPersonalShopItemList::KPersonalShopItemList(void)
{
}

KPersonalShopItemList::~KPersonalShopItemList(void)
{
}

ImplToStringW( KPersonalShopItemList )
{
	stm_ << L"KPersonalShopItemList dump." << std::endl
		;

	return stm_;
}

bool KPersonalShopItemList::AddItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & _kNot )
{
	// ����Ʈ�� ��ϵ� ������ ����ŭ �ݺ�
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, kAddItemInfo, _kNot.m_vecItemInfo )
	{
		// ������ ����������� ����
		KTradeBoardItemInfo kInsertItem;
		kInsertItem.m_iPersonalShopUID = _kNot.m_iPSUID;
		//{{ 2011. 05. 03	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		kInsertItem.m_iHostUnitUID = _kNot.m_iHostUnitUID;
#endif SERV_PSHOP_AGENCY
		//}}
		kInsertItem.m_wstrSellerNickName = _kNot.m_wstrSellerNickName;
		kInsertItem.m_kSellItemInfo = kAddItemInfo;

		// ī�װ� Ű �˻�.
		if( false == CheckSlotCategoryID(kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory) )
		{
			START_LOG( cerr, L"ī�װ� Ű �̻�.!" )
				<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			continue;
		}

		std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
		mit = m_mapItemInfo.find( InvSlotCategoryID(kInsertItem.m_kSellItemInfo) );

		// ī�װ��� ���������.
		if( mit == m_mapItemInfo.end() )
		{
			std::list<KTradeBoardItemInfo> kItemList;
			kItemList.push_back(kInsertItem);
			m_mapItemInfo.insert( std::make_pair(InvSlotCategoryID(kInsertItem.m_kSellItemInfo), kItemList) );

			START_LOG( clog, L"ī�װ� ����� ����." )
				<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( kInsertItem.m_wstrSellerNickName );
			continue;
		} // if
		else
		{
			bool bIsInsert = false;

			// ����Ʈ�� �˻� �����Ͽ� ����ֱ�.
			std::list<KTradeBoardItemInfo>::iterator lit;
			for( lit = mit->second.begin(); lit != mit->second.end(); ++lit )
			{
				bIsInsert = false;

				const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );
				const CXSLItem::ItemTemplet* pInsertItem = SiCXSLItemManager()->GetItemTemplet( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

				// ������ ���̵� üũ.
				if( pListItem == NULL || pInsertItem == NULL )
				{
					START_LOG( cerr, L"�߸��� ������ ���̵�.!" )
						<< BUILD_LOG( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						<< END_LOG;
					break;
				}

				//## Ű���� �켱������ üũ�Ͽ� ���� ����

				//# ������
				if( InvEqipPosID(pListItem) == InvEqipPosID(pInsertItem) )
				{
					//# ���� ��
					if( pListItem->m_UseLevel == pInsertItem->m_UseLevel )					
					{
						//# ĳ���� ��
						if( InvUnitTypeID(pListItem) == InvUnitTypeID(pInsertItem) )
						{
                            //# ��� ��
							if( InvItemGradeID(pListItem) == InvItemGradeID(pInsertItem) )
							{
								//# ���� ��
								if( (*lit).m_kSellItemInfo.TotalPricePerOne() > kInsertItem.m_kSellItemInfo.TotalPricePerOne() )
								{
									mit->second.insert( lit, kInsertItem );
									bIsInsert = true;

									START_LOG( clog, L"���ݺ� ������ ���� ����" )
										<< BUILD_LOG( pInsertItem->m_Name )
										<< BUILD_LOG( pInsertItem->m_ItemID );
									break;
								}
							}
							else if( InvItemGradeID(pListItem) > InvItemGradeID(pInsertItem) )
							{
								mit->second.insert( lit, kInsertItem );
								bIsInsert = true;

								START_LOG( clog, L"��޺� ������ ���� ����" )
									<< BUILD_LOG( pInsertItem->m_Name )
									<< BUILD_LOG( pInsertItem->m_ItemID );
								break;
							}
						}
						else if( InvUnitTypeID(pListItem) > InvUnitTypeID(pInsertItem) )
						{
							mit->second.insert( lit, kInsertItem );
							bIsInsert = true;

							START_LOG( clog, L"ĳ���ͺ� ������ ���� ����" )
								<< BUILD_LOG( pInsertItem->m_Name )
								<< BUILD_LOG( pInsertItem->m_ItemID );
							break;
						}	
					}					
					else if( pListItem->m_UseLevel < pInsertItem->m_UseLevel )
					{
						mit->second.insert( lit, kInsertItem );
						bIsInsert = true;

						START_LOG( clog, L"������ ������ ���� ����" )
							<< BUILD_LOG( pInsertItem->m_Name )
							<< BUILD_LOG( pInsertItem->m_ItemID );
						break;
					}
				}
				else if( InvEqipPosID(pListItem) > InvEqipPosID(pInsertItem) )
				{ 
					mit->second.insert( lit, kInsertItem );
					bIsInsert = true;

					START_LOG( clog, L"������ ������ ���� ����" )
						<< BUILD_LOG( pInsertItem->m_Name )
						<< BUILD_LOG( pInsertItem->m_ItemID );
					break;
				}
			} // for

			if( bIsInsert == false )
			{
				mit->second.push_back(kInsertItem);

				START_LOG( clog, L"push_back ������ ���� ����" )
					<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( kInsertItem.m_wstrSellerNickName );
			}
		} // else

	} // boost_test_foreach

	return true;
}

bool KPersonalShopItemList::DelItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & _kNot )
{
	// ������ ������ ����ŭ �ݺ�
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, kDelItemInfo, _kNot.m_vecItemInfo )
	{
		// ī�װ� Ű �˻�.
		if( false == CheckSlotCategoryID(kDelItemInfo.m_kInventoryItemInfo.m_cSlotCategory) )
		{
			START_LOG( cerr, L"ī�װ� Ű �̻�.!" )
				<< BUILD_LOG( kDelItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kDelItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kDelItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			continue;
		}

		std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
		mit = m_mapItemInfo.find( InvSlotCategoryID(kDelItemInfo) );

		// ī�װ� ����Ʈ�� ���ٸ�..
		if( mit == m_mapItemInfo.end() )
		{
			START_LOG( cerr, L"ī�׷θ� ����Ʈ�� �������� ����." )
				<< BUILD_LOGc( kDelItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< BUILD_LOG( _kNot.m_wstrSellerNickName )
				<< END_LOG;
			continue;
		}
		// ī�װ� ����Ʈ�� �ִٸ� ��ȸ�Ͽ� �˻��� ����.
		else
		{
			std::list<KTradeBoardItemInfo>::iterator lit;
			for( lit = mit->second.begin(); lit != mit->second.end(); ++lit )
			{
				if( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID ==
					kDelItemInfo.m_kInventoryItemInfo.m_iItemUID )
				{
					START_LOG( clog, L"������ ����Ʈ ��������." )
						<< BUILD_LOG( (*lit).m_wstrSellerNickName )
						<< BUILD_LOG( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						;

					mit->second.erase(lit);
					break;
				}
			} // for
		}
	} // boost_test_foreach
	return true;
}

bool KPersonalShopItemList::DelItemInfo( const UidType & _iCenterUID )
{
	int iDelCnt = 0;
	// ī�װ� �� ����Ʈ�� ��ȸ..
	std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
	for( mit = m_mapItemInfo.begin(); mit != m_mapItemInfo.end(); ++mit )
	{
		std::list<KTradeBoardItemInfo> & listItemInfo = mit->second;

		// ������ ���� ����Ʈ�� ��ȸ�ϸ鼭
		// _iCenterUID ���������� ������ ���� �������� ����.
		std::list<KTradeBoardItemInfo>::iterator lit;
		for( lit = listItemInfo.begin(); lit != listItemInfo.end(); ++lit )
		{
			if( KncUid::ExtractServerID((*lit).m_iPersonalShopUID) == _iCenterUID )
			{
				lit = listItemInfo.erase(lit);
				--lit;

				++iDelCnt;
			}
		}
	}

	START_LOG( clog, L"���Ͱ� �˴ٿ�Ǿ� ������ ������ ���� ����." )
		<< BUILD_LOG( iDelCnt )
		;

	return true;
}

bool KPersonalShopItemList::ChangeItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & _kNot )
{
	// ������ ������ ������ ����ŭ �ݺ�
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, kChangeItemInfo, _kNot.m_vecItemInfo )
	{
		// ī�װ� Ű �˻�.
		if( false == CheckSlotCategoryID(kChangeItemInfo.m_kInventoryItemInfo.m_cSlotCategory) )
		{
			START_LOG( cerr, L"ī�װ� Ű �̻�.!" )
				<< BUILD_LOG( kChangeItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kChangeItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kChangeItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			continue;
		}

		std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
		mit = m_mapItemInfo.find( InvSlotCategoryID(kChangeItemInfo) );

		// ī�װ� ����Ʈ�� ���ٸ�..
		if( mit == m_mapItemInfo.end() )
		{
			START_LOG( cerr, L"ī�װ� ����Ʈ�� �������� ����." )
				<< BUILD_LOGc( kChangeItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< BUILD_LOG( _kNot.m_wstrSellerNickName )
				<< END_LOG;
			continue;
		}
		// ī�װ� ����Ʈ�� �ִٸ� ��ȸ�Ͽ� �˻��� ����..
		else
		{
			std::list<KTradeBoardItemInfo>::iterator lit;
			for( lit = mit->second.begin(); lit != mit->second.end(); ++lit )
			{
				if( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID ==
					kChangeItemInfo.m_kInventoryItemInfo.m_iItemUID )
				{
					START_LOG( clog, L"������ ����Ʈ ��������." )
						<< BUILD_LOG( (*lit).m_wstrSellerNickName )
						<< BUILD_LOG( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						;

					(*lit).m_kSellItemInfo = kChangeItemInfo;
					break;
				}
			} // for
		}
	} // boost_test_foreach
	return true;
}

void KPersonalShopItemList::GetSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck )
{
	kAck.m_iOK = NetError::NET_OK;

	// ī�װ� Ű �˻�.
	if( false == CheckSlotCategoryID( kReq.m_cSlotCategory ) )
	{
		if( kReq.m_wstrFinder.empty() == true )
		{
			START_LOG( cerr, L"ī�װ� Ű �̻�." )
				<< BUILD_LOGc( kReq.m_cSlotCategory )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_29;
		}
		// ��Ʈ�������� �˻��ϴ� ���.
		else
		{
			//BOOST_TEST_FOREACH( std::list<KTradeBoardItemInfo>, TradeBoardItemList, m_mapItemInfo )
			std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
			for( mit = m_mapItemInfo.begin(); mit != m_mapItemInfo.end(); ++mit )
			{
				BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )//TradeBoardItemList )
				{
					const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

					if( NULL == pListItem )
					{
						START_LOG( cerr, L"������ ���̵� �߸��Ǿ���." )
							<< BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
							<< BUILD_LOG( kItemInfo.m_wstrSellerNickName )
							<< END_LOG;

						continue;
					} // if

					// ��Ʈ�� �˻�
					//{{ 2010. 12. 16  ��ȿ��	���� ���� ������ �˻� �� ��ҹ��� ����
#ifdef SERV_STRING_CHECK_IGNORE_CASE
					if( boost::ifind_first(pListItem->m_Name, kReq.m_wstrFinder) )
#else SERV_STRING_CHECK_IGNORE_CASE
					if( NULL != StrStrW( pListItem->m_Name.c_str(), kReq.m_wstrFinder.c_str() ) )
#endif SERV_STRING_CHECK_IGNORE_CASE
					//}}
					{
#ifdef SERV_FIX_SEARCH_WITH_EMPTY_TAB
						bool bIsInsert = true;
						if( false == kReq.m_vecItemGrade.empty() )
						{
							bIsInsert = false;
							for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
							{
								if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] ) // ��ȸ�ϸ鼭 ã��
								{
									bIsInsert = true;
									break;
								}
							}
						} // if

						if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
						{
							//�ּҷ��� ���̸� ��� ����
							if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
								pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
								bIsInsert = false;
							////�ְ��� ���ĸ� �˻�����
							//else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
							//	break;
						} // if

						if( bIsInsert == true )
#endif SERV_FIX_SEARCH_WITH_EMPTY_TAB
						kAck.m_vecItemInfo.push_back(kItemInfo);

						if( kAck.m_vecItemInfo.size() >= 100 ) // 100 �� ����.
							return;
					}
				}
			}
		}

		// �˻��� ������ ����Ʈ�� ������ �޼��� ����
		// des : �˻��ҷ��� ������ ������ �����ϴ�.
		if( true == kAck.m_vecItemInfo.empty() )
			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;

		return;
	}

	//## �˻��� ī�װ� ����Ʈ�� ã�´�.
	std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
	mit = m_mapItemInfo.find( InvSlotCategoryID(kReq.m_cSlotCategory) );

	//# ī�װ� ����Ʈ�� ���ٸ�..
	if( mit == m_mapItemInfo.end() )
	{
		START_LOG( clog, L"�˻��ҷ��� ī�װ� ������ ����Ʈ�� ����." )
			<< BUILD_LOGc( kReq.m_cSlotCategory )
			<< END_LOG;
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
		return;
	}

	//# �˻� ����
	BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )
	{
		bool bIsInsert = true;

		const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

		if( NULL == pListItem )
		{
			START_LOG( cerr, L"������ ���̵� �߸��Ǿ���." )
				<< BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( kItemInfo.m_wstrSellerNickName )
				<< END_LOG;

			continue;
		} // if

		//# ��Ʈ�� �˻�
		if( false == kReq.m_wstrFinder.empty() )
		{
			//if( (pListItem->m_Name.find(kReq.m_wstrFinder.c_str())) < 0 )
			//int iTemp = pListItem->m_Name.find(kReq.m_wstrFinder.c_str());

			//{{ 2010. 12. 16  ��ȿ��	���� ���� ������ �˻� �� ��ҹ��� ����
#ifdef SERV_STRING_CHECK_IGNORE_CASE
			if( !(boost::ifind_first(pListItem->m_Name, kReq.m_wstrFinder) ) )	// �� ã���� ��
#else SERV_STRING_CHECK_IGNORE_CASE
			if( StrStrW( pListItem->m_Name.c_str(), kReq.m_wstrFinder.c_str() ) == NULL )
#endif SERV_STRING_CHECK_IGNORE_CASE
			//}}
				bIsInsert = false;
		}

		//# ���� ������ ���� üũ
		if( -1 != kReq.m_cEqipPosition )
		{
			//���� ������ ������ ��� ����
			if( InvEqipPosID(pListItem) < InvEqipPosID(kReq.m_cEqipPosition) )
				bIsInsert = false;
			//���� ���� ������ �Ѱ����� �˻�����
			else if( InvEqipPosID(pListItem->m_EqipPosition) > InvEqipPosID(kReq.m_cEqipPosition) )
				break;
		} // if

		//# ĳ���ͺ� üũ
		if( -1 != kReq.m_cUnitType )
		{
			//���� �����̸� �������
			//if( InvUnitTypeID(pListItem) < InvUnitTypeID(kReq.m_cUnitType) )
			//	bIsInsert = false;
			//���� �����̸�..
			if( pListItem->m_UnitType == kReq.m_cUnitType )
			{
				//����Ŭ���� �˻�..
				if( -1 != kReq.m_cUnitClass )
				{
					//���� Ŭ������ �ƴϸ� �������
					if( pListItem->m_UnitClass != kReq.m_cUnitClass )
						bIsInsert = false;
				}
			}
			else
				bIsInsert = false;
		} // if

		//# �������� üũ
		if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
		{
			//�ּҷ��� ���̸� ��� ����
			if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
				pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
				bIsInsert = false;
			////�ְ��� ���ĸ� �˻�����
			//else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
			//	break;
		} // if

		//# ������ ���.
		//if( -1 != kReq.m_cItemGrade )
		if( false == kReq.m_vecItemGrade.empty() )
		{
			////���� ����̸� �������.
			//if( InvItemGradeID(pListItem->m_ItemGrade) < InvItemGradeID(kReq.m_cItemGrade) )
			//	bIsInsert = false;
			////���� ����̸� �˻�����.
			//else if( InvItemGradeID(pListItem->m_ItemGrade) > InvItemGradeID(kReq.m_cItemGrade) )
			//	break;
			bool bIsCheck = false;
			for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
			{
				if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] )
				{
					bIsCheck = true;
					break;
				}
			}

			if( bIsCheck == false )
				bIsInsert = false;
		} // if

		if( kAck.m_vecItemInfo.size() >= 100 )
			break;

		if( true == bIsInsert )
		{
			kAck.m_vecItemInfo.push_back(kItemInfo);
		} // if
	}// BOOST_TEST_FOREACH

	//{{ oasis907 : ����� [2010.3.31] // 
	// �˻��� ������ ����Ʈ�� ������ �޼��� ����
	// des : �˻��ҷ��� ������ ������ �����ϴ�.
	if( true == kAck.m_vecItemInfo.empty() )
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
	//}}

}

void KPersonalShopItemList::GetStrictSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck )
{
    kAck.m_iOK = NetError::NET_OK;

    // ī�װ� Ű �˻�.
    if( false == CheckSlotCategoryID( kReq.m_cSlotCategory ) )
    {
        if( kReq.m_wstrFinder.empty() == true )
        {
            START_LOG( cerr, L"ī�װ� Ű �̻�." )
                << BUILD_LOGc( kReq.m_cSlotCategory )
                << END_LOG;

            kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_29;
        }
        // ��Ʈ�������� �˻��ϴ� ���.
        else
        {
            //BOOST_TEST_FOREACH( std::list<KTradeBoardItemInfo>, TradeBoardItemList, m_mapItemInfo )
            std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
            for( mit = m_mapItemInfo.begin(); mit != m_mapItemInfo.end(); ++mit )
            {
                BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )//TradeBoardItemList )
                {
                    const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

                    if( NULL == pListItem )
                    {
                        START_LOG( cerr, L"������ ���̵� �߸��Ǿ���." )
                            << BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
                            << BUILD_LOG( kItemInfo.m_wstrSellerNickName )
                            << END_LOG;

                        continue;
                    } // if

                    // ��Ʈ�� �˻�
                    if( pListItem->m_Name == kReq.m_wstrFinder ) // str1���� str2 �� �˻��Ͽ� ���� ���� ��Ÿ���� ���� ��ġ�� ����. ������ �� ������ ����
                    {
#ifdef SERV_FIX_SEARCH_WITH_EMPTY_TAB
                        bool bIsInsert = true;
                        if( false == kReq.m_vecItemGrade.empty() )
                        {
                            bIsInsert = false;
                            for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
                            {
                                if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] ) // ��ȸ�ϸ鼭 ã��
                                {
                                    bIsInsert = true;
                                    break;
                                }
                            }
                        } // if

                        if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
                        {
                            //�ּҷ��� ���̸� ��� ����
                            if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
                                pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
                                bIsInsert = false;
                            ////�ְ��� ���ĸ� �˻�����
                            //else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
                            //	break;
                        } // if

                        if( bIsInsert == true )
#endif SERV_FIX_SEARCH_WITH_EMPTY_TAB
                            kAck.m_vecItemInfo.push_back(kItemInfo);

                        if( kAck.m_vecItemInfo.size() >= 100 ) // 100 �� ����.
                            return;
                    }
                }
            }
        }

        // �˻��� ������ ����Ʈ�� ������ �޼��� ����
        // des : �˻��ҷ��� ������ ������ �����ϴ�.
        if( true == kAck.m_vecItemInfo.empty() )
            kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;

        return;
    }

    //## �˻��� ī�װ� ����Ʈ�� ã�´�.
    std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
    mit = m_mapItemInfo.find( InvSlotCategoryID(kReq.m_cSlotCategory) );

    //# ī�װ� ����Ʈ�� ���ٸ�..
    if( mit == m_mapItemInfo.end() )
    {
        START_LOG( clog, L"�˻��ҷ��� ī�װ� ������ ����Ʈ�� ����." )
            << BUILD_LOGc( kReq.m_cSlotCategory )
            << END_LOG;
        kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
        return;
    }

    //# �˻� ����
    BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )
    {
        bool bIsInsert = true;

        const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

        if( NULL == pListItem )
        {
            START_LOG( cerr, L"������ ���̵� �߸��Ǿ���." )
                << BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
                << BUILD_LOG( kItemInfo.m_wstrSellerNickName )
                << END_LOG;

            continue;
        } // if

        //# ��Ʈ�� �˻�
        if( false == kReq.m_wstrFinder.empty() )
        {
            //if( (pListItem->m_Name.find(kReq.m_wstrFinder.c_str())) < 0 )
            //int iTemp = pListItem->m_Name.find(kReq.m_wstrFinder.c_str());

            if( StrStrW( pListItem->m_Name.c_str(), kReq.m_wstrFinder.c_str() ) == NULL )
                bIsInsert = false;
        }

        //# ���� ������ ���� üũ
        if( -1 != kReq.m_cEqipPosition )
        {
            //���� ������ ������ ��� ����
            if( InvEqipPosID(pListItem) < InvEqipPosID(kReq.m_cEqipPosition) )
                bIsInsert = false;
            //���� ���� ������ �Ѱ����� �˻�����
            else if( InvEqipPosID(pListItem->m_EqipPosition) > InvEqipPosID(kReq.m_cEqipPosition) )
                break;
        } // if

        //# ĳ���ͺ� üũ
        if( -1 != kReq.m_cUnitType )
        {
            //���� �����̸� �������
            //if( InvUnitTypeID(pListItem) < InvUnitTypeID(kReq.m_cUnitType) )
            //	bIsInsert = false;
            //���� �����̸�..
            if( pListItem->m_UnitType == kReq.m_cUnitType )
            {
                //����Ŭ���� �˻�..
                if( -1 != kReq.m_cUnitClass )
                {
                    //���� Ŭ������ �ƴϸ� �������
                    if( pListItem->m_UnitClass != kReq.m_cUnitClass )
                        bIsInsert = false;
                }
            }
            else
                bIsInsert = false;
        } // if

        //# �������� üũ
        if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
        {
            //�ּҷ��� ���̸� ��� ����
            if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
                pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
                bIsInsert = false;
            ////�ְ��� ���ĸ� �˻�����
            //else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
            //	break;
        } // if

        //# ������ ���.
        //if( -1 != kReq.m_cItemGrade )
        if( false == kReq.m_vecItemGrade.empty() )
        {
            ////���� ����̸� �������.
            //if( InvItemGradeID(pListItem->m_ItemGrade) < InvItemGradeID(kReq.m_cItemGrade) )
            //	bIsInsert = false;
            ////���� ����̸� �˻�����.
            //else if( InvItemGradeID(pListItem->m_ItemGrade) > InvItemGradeID(kReq.m_cItemGrade) )
            //	break;
            bool bIsCheck = false;
            for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
            {
                if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] )
                {
                    bIsCheck = true;
                    break;
                }
            }

            if( bIsCheck == false )
                bIsInsert = false;
        } // if

        if( kAck.m_vecItemInfo.size() >= 100 )
            break;

        if( true == bIsInsert )
        {
            kAck.m_vecItemInfo.push_back(kItemInfo);
        } // if
    }// BOOST_TEST_FOREACH

    //{{ oasis907 : ����� [2010.3.31] // 
    // �˻��� ������ ����Ʈ�� ������ �޼��� ����
    // des : �˻��ҷ��� ������ ������ �����ϴ�.
    if( true == kAck.m_vecItemInfo.empty() )
        kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
    //}}

}
char KPersonalShopItemList::InvSlotCategoryID( KSellPersonalShopItemInfo & _kInfo )
{
	return InvSlotCategoryID(_kInfo.m_kInventoryItemInfo.m_cSlotCategory);
}

char KPersonalShopItemList::InvSlotCategoryID( char _cID )
{
	char cRet = 7;	// ����� ����Ʈ�� ���� ���� Ű����.

	switch( _cID )
	{
	case CXSLInventory::ST_AVARTA: cRet = 1; break;
	case CXSLInventory::ST_EQUIP: cRet = 2; break;
	case CXSLInventory::ST_ACCESSORY: cRet = 3; break;
	case CXSLInventory::ST_QUICK_SLOT: cRet = 4; break;
	case CXSLInventory::ST_SPECIAL: cRet = 5; break;
	case CXSLInventory::ST_MATERIAL: cRet = 6; break;
	case CXSLInventory::ST_QUEST: cRet = 7; break;
	}

	return cRet;
}

char KPersonalShopItemList::InvEqipPosID( const CXSLItem::ItemTemplet* _pInfo )
{
	if( _pInfo == NULL )
		return 15;

	return InvEqipPosID(_pInfo->m_EqipPosition);
}

char KPersonalShopItemList::InvEqipPosID( char _cID )
{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
	char cRet = 16;
#else //SERV_NEW_ONE_PIECE_AVATAR_SLOT
	char cRet = 15;		// ����� ���� �Ǽ��縮�� ���� Ű����.
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

	switch( _cID )
	{
	case CXSLUnit::EP_WEAPON_HAND: cRet = 1; break;
	case CXSLUnit::EP_DEFENCE_BODY: cRet = 2; break;
	case CXSLUnit::EP_DEFENCE_LEG: cRet = 3; break;
	case CXSLUnit::EP_DEFENCE_HAND: cRet = 4; break;
	case CXSLUnit::EP_DEFENCE_FOOT: cRet = 5; break;
	case CXSLUnit::EP_DEFENCE_HAIR: cRet = 6; break;
	case CXSLUnit::EP_AC_FACE1: cRet = 7; break;
	case CXSLUnit::EP_AC_FACE2: cRet = 8; break;
	case CXSLUnit::EP_AC_FACE3: cRet = 9; break;
	case CXSLUnit::EP_AC_BODY: cRet = 10; break;
	case CXSLUnit::EP_AC_LEG: cRet = 11; break;
	case CXSLUnit::EP_AC_ARM: cRet = 12; break;
	case CXSLUnit::EP_AC_RING: cRet = 13; break;
	case CXSLUnit::EP_AC_NECKLESS: cRet = 14; break;
	case CXSLUnit::EP_AC_WEAPON: cRet = 15; break;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
	case CXSLUnit::EP_ONEPIECE_FASHION: cRet = 16; break;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
	}

	return cRet;
}

char KPersonalShopItemList::InvUnitTypeID( const CXSLItem::ItemTemplet* _pInfo )
{
	if( _pInfo == NULL )
		return 5;

	return InvUnitTypeID(_pInfo->m_UnitType);
}

char KPersonalShopItemList::InvUnitTypeID( char _cID )
{
	char cRet = UTSI_CHUNG;

	switch( _cID )
	{
	case CXSLUnit::UT_ELSWORD:	cRet = UTSI_ELSWORD;	break;
	case CXSLUnit::UT_LIRE:		cRet = UTSI_LENA;		break;
	case CXSLUnit::UT_ARME:		cRet = UTSI_AISHA;	break;
	case CXSLUnit::UT_RAVEN:	cRet = UTSI_RAVEN;	break;
	case CXSLUnit::UT_EVE:		cRet = UTSI_EVE;		break;
	// kimhc // 2010-12-23 �� �߰��� ��ĳ���� û (�ϴ� ���ҵ�� ���� ��)
	case CXSLUnit::UT_CHUNG:	cRet = UTSI_CHUNG;	break;
	case CXSLUnit::UT_ARA:		cRet = UTSI_ARA;	break;
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UT_ELESIS:	cRet = UTSI_ELESIS;	break;
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CXSLUnit::UT_ADD:	cRet = UTSI_NEW_CHARACTER;	break;
#endif //SERV_9TH_NEW_CHARACTER
	}

	return cRet;
}

char KPersonalShopItemList::InvItemGradeID( const CXSLItem::ItemTemplet* _pInfo )
{
	if( _pInfo == NULL )
		return 5;

	return InvItemGradeID(_pInfo->m_ItemGrade);
}

char KPersonalShopItemList::InvItemGradeID( char _cID )
{
	char cRet = 5;

	switch( _cID )
	{
	case CXSLItem::IG_UNIQUE: cRet = 1; break;
	case CXSLItem::IG_ELITE: cRet = 2; break;
	case CXSLItem::IG_RARE: cRet = 3; break;
	case CXSLItem::IG_NORMAL: cRet = 4; break;
	case CXSLItem::IG_LOW: cRet = 5; break;
	}

	return cRet;
}

bool KPersonalShopItemList::CheckSlotCategoryID( char _cID )
{
	switch( _cID )
	{
	case CXSLInventory::ST_AVARTA: break;
	case CXSLInventory::ST_EQUIP: break;
	case CXSLInventory::ST_ACCESSORY: break;
	case CXSLInventory::ST_QUICK_SLOT: break;
	case CXSLInventory::ST_SPECIAL: break;
	case CXSLInventory::ST_MATERIAL: break;
	case CXSLInventory::ST_QUEST: break;

	default:
		return false;
	}
	return true;
}

#endif DEF_TRADE_BOARD
//}}
































