#include ".\userwishlist.h"
#include <dbg/dbg.hpp>
#include "NetError.h"
#include "../common/X2Data/XSLCashItemManager.h"

#ifdef SERV_WISH_LIST_NO_ITEM
#include "X2Data/XSLItemManager.h"
#endif //SERV_WISH_LIST_NO_ITEM		

#ifdef SERV_WISH_LIST_BUG_FIX
#include "BillManager.h"
#endif //SERV_WISH_LIST_BUG_FIX

KUserWishList::KUserWishList(void)
{
	m_bBuyCashItemInWishList = false;
}

KUserWishList::~KUserWishList(void)
{	
}

void KUserWishList::InitWishList( const std::map< int, int >& mapWishList )
{
	std::vector< int > vecNoSaleProduct; // �Ǹ������� ��ǰ ����Ʈ
	std::map< int, int >::const_iterator mit;
	for( mit = mapWishList.begin(); mit != mapWishList.end(); ++mit )
	{
		if( mit->first < 0  ||  mit->first >= SLOT_MAX )
			continue;

		m_arrWishList[mit->first].SetValue( mit->second );

		// �Ǹ������� ��ǰ ����Ʈ �˻�
		if( !SiCXSLCashItemManager()->IsBuyAvailable( mit->second ) )
		{
			vecNoSaleProduct.push_back( mit->second );
		}
#ifdef SERV_WISH_LIST_NO_ITEM
		// �򸮽�Ʈ�Ұ� �������̸� �򸮽�Ʈ���� �����Ѵ�.
		if( SiCXSLItemManager()->IsWishListNoItem( mit->second ) )
		{
			vecNoSaleProduct.push_back( mit->second );
		}
#endif //SERV_WISH_LIST_NO_ITEM
	}

	//{{ 2008. 5. 15  ������  �Ǹ����� ��ǰ�� �ƴҰ�� �򿡼� ���� �۾�
	std::vector< int >::const_iterator vit;
	for( vit = vecNoSaleProduct.begin(); vit != vecNoSaleProduct.end(); ++vit )
	{
		if( !DelWish( *vit ) )
		{
			START_LOG( cerr, L"�Ǹ������� ��ǰ�� �򸮽�Ʈ���� �����ϱ� ����!" )
				<< BUILD_LOG( *vit )
				<< END_LOG;
		}
	}
	//}}
}

void KUserWishList::GetWishList( std::map< int, int >& mapWishList )
{
#ifdef SERV_WISH_LIST_BUG_FIX
	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() <= 0 )
			continue;

		int nProductID = m_arrWishList[iIdx].GetSlotData();

		std::set<int> setSellCashItemList = SiKBillManager()->GetSellCashItemList();	// ���� �Ȱ� �ִ� ĳ�������� ����Ʈ�� ������.

		if( setSellCashItemList.find( nProductID ) == setSellCashItemList.end()
#ifdef SERV_WISH_LIST_NO_ITEM
			|| SiCXSLItemManager()->IsWishListNoItem( nProductID )
#endif //SERV_WISH_LIST_NO_ITEM			
			)
		{
			if( !DelWish( nProductID ) )												// ����! DelWish()�ϰ� ���� m_arrWishList[iIdx].GetSlotData()�� ���� �ٲ�.
			{																			// ��) m_arrWishList[1]�� �����Ͱ� m_arrWishList[0]���� �̵���.
				START_LOG( cerr, L"�Ǹ������� ��ǰ�� �򸮽�Ʈ���� �����ϱ� ����!" )
					<< BUILD_LOG( nProductID )
					<< END_LOG;
			}

			--iIdx;																		// �򸮽�Ʈ�� ���� �մ�������Ƿ� for ���� �ε����� �մ��.
		}
	}

	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		mapWishList.insert( std::make_pair( iIdx, m_arrWishList[iIdx].GetSlotData() ) );
	}
#else //SERV_WISH_LIST_BUG_FIX
	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() <= 0 )
			continue;

		mapWishList.insert( std::make_pair( iIdx, m_arrWishList[iIdx].GetSlotData() ) );
	}
#endif //SERV_WISH_LIST_BUG_FIX
}

void KUserWishList::FlushWishListChange( std::map< int, int >& mapWishList )
{
	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].IsChanged() )
		{
			int iTemp = m_arrWishList[iIdx].GetSlotData();
			
			m_arrWishList[iIdx].SetValue( iTemp );
			
			mapWishList.insert( std::make_pair( iIdx, iTemp ) );
		}
	}
}

bool KUserWishList::AddWish( int iItemID )
{
	SET_ERROR( NET_OK );

	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() == iItemID )
		{
			START_LOG( cerr, L"�̹� �ִ°� ����Ϸ��� �ϳ�? �ϴ��� �����Ű��." )
				<< BUILD_LOG( iIdx )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			return true;
		}

		if( m_arrWishList[iIdx].GetSlotData() == 0 )
		{
			m_arrWishList[iIdx] = iItemID;
			return true;
		}	
	}

	SET_ERROR( ERR_WISH_LIST_02 );
	return false;
}

bool KUserWishList::DelWish( int iItemID )
{
	SET_ERROR( NET_OK );

	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() == iItemID )
		{
			for( int iCnt = iIdx; iCnt < SLOT_MAX; ++iCnt )
			{
				// ������ü�� ������ �ε������..
				if( iCnt == (SLOT_MAX - 1) )
				{
					if( m_arrWishList[iCnt].GetSlotData() == 0 )
						break;

					m_arrWishList[iCnt] = 0;
					break;
				}

				// ��ϵ� ��ǰ�� ������ �ε���..
				if( m_arrWishList[iCnt].GetSlotData() == 0 )
					break;

				// ��ĭ�� ����!				
				m_arrWishList[iCnt] = m_arrWishList[iCnt + 1].GetSlotData();
			}
			return true;
		}
	}

	START_LOG( cerr, L"��ϵ����� ������ �����Ϸ��� �ϳ�.. �ϴ��� �����Ű��." ) 
		<< BUILD_LOG( iItemID )
		<< END_LOG;
	return false;
}

