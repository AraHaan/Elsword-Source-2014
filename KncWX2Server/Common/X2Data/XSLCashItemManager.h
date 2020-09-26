#pragma once

#include <KNCSingleton.h>
#include "XSLItem.h"

#ifdef SERV_CASHITEM_RESOLVE_ADD
#include "Lottery.h"
#endif

class CXSLCashItemManager
{
	DeclareSingleton( CXSLCashItemManager );
	DeclDump;
	DeclToStringW;

public:

#ifdef SERV_EXCEPT_NO_SELL
	enum CASH_SHOP_DB_CATEGORY
	{
		CSDC_FASHION_WEAPON = 11,
		CSDC_FASHION_BODY,
		CSDC_FASHION_LEG,
		CSDC_FASHION_HAIR,
		CSDC_FASHION_HAND,
		CSDC_FASHION_FOOT,

		// CSC_ACCESSORY
		CSDC_ACCESSORY_WEAPON = 21,
		CSDC_ACCESSORY_FACE,
		CSDC_ACCESSORY_BODY,
		CSDC_ACCESSORY_LEG,
		CSDC_ACCESSORY_ARM,
		CSDC_ACCESSORY_RING,
		CSDC_ACCESSORY_NECKLACE,

		CSDC_CONSUME_RESTORE = 31,
		CSDC_CONSUME_PREMIUM,
		CSDC_CONSUME_INIT,
		CSDC_CONSUME_ETC,

		CSDC_INSTALL_ADD_SLOT = 41,
		CSDC_INSTALL_PERSONAL_SHOP,
		CSDC_INSTALL_ETC,

		CSDC_EVENT_PACKAGE = 51,

		CSDC_PET_PET = 61,
		CSDC_PET_CONSUME,

		CSDC_COUPON = 71,
		CSDC_NO_SELL_IN_PACKAGE,
	};
#endif //SERV_EXCEPT_NO_SELL

	struct PackageItemData
	{
		int				m_iItemID;
		unsigned short  m_usProductPieces;
		bool			m_bShowItem;
	};
	//{{ 2011.2.27 ���� : ĳ�� �ƹ�Ÿ ���ؽ� �߰� ������ �������� �ϱ�
#ifdef SERV_CASHITEM_RESOLVE_ADD
	struct ResolveAddInfo
	{
		int iItemID;
		int iQuantity;
		KLottery kLottery;
	};
	typedef std::vector<int> VecInt;
	typedef std::vector<ResolveAddInfo> VecResolveInfo;

	enum RESOLVE_RESULT { RESOLVE_SUCCESS = 1};
#endif
	//}}
public:
	CXSLCashItemManager(void);
	~CXSLCashItemManager(void);

	bool OpenScriptFile( const char* pFileName );
	bool AddPackageItemData_LUA( int iPackageItemID, int iItemID, unsigned short usProductPieces, bool bShowItem );
	bool AddCashItemPrice_LUA( int iItemID, int iPrice );
	//{{ 2011.2.27 ���� : ĳ�� �ƹ�Ÿ ���ؽ� �߰� ������ �������� �ϱ�
#ifdef SERV_CASHITEM_RESOLVE_ADD
	// ĳ�� ������ ���ؿ� ��� �� �׷� ���
	bool AddCashItemGroup_LUA( int iGroupID, int iItemID);
	// ������ �׷� ���ؽ� ���� ������ ���
	bool AddCashItemResolveAdd_LUA( int iGroupID, int iItemID, int iQuantity, float fProbability);
	
	bool GetCashItemResolveAddFromScript( int iItemID, VecResolveInfo& vecItem );
#endif
	//}}
	//{{ 2009. 12. 19  ������	������ Ȯ��
	bool AddCashItemServerGroupData_LUA( int iServerGroupID, int iItemID );
	//}}

	//{{ 2011. 01. 04  ��μ�	product no �� ������ Ȯ��
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	bool AddCashItemProductNoData_LUA( int iServerGroupID, unsigned long ulProductNo );
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	//}}

	bool GetPackageData( int iPackageItemID, std::vector< PackageItemData >& vecPackageItem ) const;
	//{{ 2008. 5. 15  ������  �Ǹ����� ��ǰ���� �˻� ItemID
	void UpdateProductList( std::vector< int >& vecProductItemID, std::vector< KCashItemResolveInfo >& vecCashResolveInfo, unsigned long ulProductNoResStone );
	bool IsBuyAvailable( int iItemID );
	//}}

	//{{ 2008. 6. 10  ������  �ǽð� ��Ȱ��
	unsigned long GetProductNoResStone() { return m_ulProductNoResStone; }
	//}}

	//{{ 2008. 9. 23  ������	ĳ������
	//bool GetCashItemPrice( int iItemID, int& iPrice );
	bool GetCashItemPriceFromScript( int iItemID, int& iPrice );
	//}}

	//{{ 2009. 12. 19  ������	������ Ȯ��
	bool IsSellItemInThisServer( IN int iItemID ) { return ( m_setNotSellCashItem.find( iItemID ) == m_setNotSellCashItem.end() ); }
	//}}

	//{{ 2011. 01. 04  ��μ�	product no �� ������ Ȯ��
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	bool IsSellItemInThisProductNo( IN unsigned long ulProductNo ) { return ( m_setNotSellCashItemProductNo.find( ulProductNo ) == m_setNotSellCashItemProductNo.end() ); }
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO	
	//}}
	
private:	
	std::map< int, std::vector< PackageItemData > > m_mapPackageData;

	//{{ 2008. 5. 15  ������  �Ǹ����� ��ǰ ����Ʈ ItemID
	std::set< int >									m_setProductItemID;
	bool											m_bIsUpdate;
	//}}

	//{{ 2008. 6. 10  ������  �ǽð� ��Ȱ��
	unsigned long									m_ulProductNoResStone;
	//}}

	//{{ 2008. 9. 23  ������	ĳ����������
	std::map< int, int >							m_mapCashItemPrice;
	std::map< int, int >							m_mapCashItemPriceFromScript;
	//}}

	//{{ 2009. 12. 19  ������	������ Ȯ��
	std::set< int >									m_setNotSellCashItem;
	//}}

	//{{ 2011. 01. 04  ��μ�	product no �� ������ Ȯ��
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	std::set< unsigned long >						m_setNotSellCashItemProductNo;
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO	
	//}}
	//{{ 2011.2.27 ���� : ĳ�� �ƹ�Ÿ ���ؽ� �߰� ������ �������� �ϱ�
#ifdef SERV_CASHITEM_RESOLVE_ADD
	// ĳ�� ������ �׷� key �� ������ID, value �� �׷�ID
	std::map<int, int>			m_mapCashItemGroup;
	// key : �׷�ID
	std::map<int, VecResolveInfo>	m_mapCashItemResolveGroup;
#endif
	//}}
};

DefSingletonInline( CXSLCashItemManager );
