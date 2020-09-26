#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "X2Data/XSLItem.h"


//{{ 2010. 03. 24  ������	defineó��
#ifdef DEF_TRADE_BOARD


class KPersonalShopItemList
{
	DeclareSingleton( KPersonalShopItemList );
	DeclDump;
	DeclToStringW;

	enum UNIT_TYPE_SORT_INDEX
	{
		UTSI_ELSWORD = 1,
		UTSI_LENA,
		UTSI_AISHA,
		UTSI_RAVEN,
		UTSI_EVE,
		UTSI_CHUNG,
#ifdef SERV_ARA_MARTIAL_ARTIST
		UTSI_ARA,
#endif
#ifdef SERV_NEW_CHARACTER_EL
		UTSI_ELESIS,
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		UTSI_NEW_CHARACTER,
#endif //SERV_9TH_NEW_CHARACTER
	};

public:
	KPersonalShopItemList(void);
	~KPersonalShopItemList(void);

	bool	AddItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & kNot );
	bool	DelItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & kNot );
	bool	DelItemInfo( const UidType & iCenterUID );
	bool	ChangeItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & kNot );

	void	GetSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck );
    void    GetStrictSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck );

private:
	//## Inv �Լ��� ��ȹ������ �ǵ��� Ű������ �����ϴ� �Լ�
	//#  : �ڵ�Ű���� ��ȹ�ǵ� Ű���� �����Ͽ� ���ϰ��谡 ���� �ʾ�
	//#    Ű�� �񱳽� �������� �־� �ۼ��ϴ� Invert �Լ�
	//#  Ex. SORT_TYPE::ST_AVATRA == 7  =>  return 1;
    char	InvSlotCategoryID( KSellPersonalShopItemInfo & kInfo );
	char	InvSlotCategoryID( char cID );
	char	InvEqipPosID( const CXSLItem::ItemTemplet* pInfo );
	char	InvEqipPosID( char cID );
	char	InvUnitTypeID( const CXSLItem::ItemTemplet* pInfo );
	char	InvUnitTypeID( char cID );
	char	InvItemGradeID( const CXSLItem::ItemTemplet* pInfo );
	char	InvItemGradeID( char cID );

	bool	CheckSlotCategoryID( char cID );

private:
	std::map< char, std::list<KTradeBoardItemInfo> > m_mapItemInfo;
};

DefSingletonInline( KPersonalShopItemList );


#endif DEF_TRADE_BOARD
//}}