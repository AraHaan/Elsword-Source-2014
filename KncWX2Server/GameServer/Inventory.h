#pragma once

#include <map>
#include <vector>
#include "KncUidType.h"
#include "InventoryItem.h"
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLRoom.h"
#include "TimerManager.h"
//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
	#include "InventoryLocker.h"
#endif SERV_INVENTORY_LOCK
//}}
//{{ 2012. 10. 04	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	#include "ItemQuantityVariation.h"
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}

//{{ 2012. 10. 09	��μ�	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
#include "X2Data/xslitem.h"
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
//}}

SmartPointer(KGSUser);

class KGSUser;
class KInventory
{
public:
    KInventory();
    virtual ~KInventory();

    void Clear();
    void SetOwner( IN KGSUser* pUser );
	//{{ 2010. 8. 3	������	�� �ý���
//#ifdef SERV_PET_SYSTEM
	bool Init( IN const std::map< int, int >& mapSlotSize, 
			   IN const std::map< UidType, KInventoryItemInfo >& mapInventoryItem, 
			   OUT std::map< UidType, KItemPosition >& mapWrongPosItem, 
			   IN const bool bIsChannelChange );
//#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 01. 04  ������	�κ��丮 ��ɰ���
	void GetSelectUnitInventoryInfo( OUT std::vector< KEGS_SELECT_UNIT_INVENTORY_INFO_NOT >& vecInventoryInfo );
	void GetInventorySlotSize( OUT std::map< int, int >& mapInventorySlotSize );
	//}}
	//{{ 2009. 8. 7  ������		���� ���� ���
	void GetInventoryInfo( IN const int iCategory, OUT std::map< UidType, KInventoryItemInfo >& mapInventoryItem, IN const bool bForce = false );
	void GetInventoryInfo( IN const int iCategory, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, IN const bool bForce = false );
	//}}

    int  GetFirstCategory() const			{ return CXSLInventory::ST_EQUIP; }
    int  GetSentinelCategory() const		{ return CXSLInventory::ST_END; }

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
	void GetItemIDSet( OUT std::set< int >& setItemID, bool bOnlyInventory = false );
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

    int  GetBaseSlotSize( IN const int iCategory ) const;
    int  GetMaxSlotSize( IN const int iCategory ) const;
    int  GetSlotSize( IN const int iCategory ) const;
	
#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX
	char GetItemState( IN const UidType iItemUID ) const;
#endif //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
    bool IsExist( IN const int iItemID, IN  const bool bOnlyInventory ) const;
	bool IsExistOnlyInventory( IN  const int iItemID ) const;
	bool IsExistWithBank( IN const int iItemID ) const;
    bool IsExist( IN const UidType iItemUID ) const;
	bool IsExistInBank( IN const UidType iItemUID ) const;
	bool IsEquippedItem( IN const int iItemID ) const;

    bool IsEmptySlot( IN const int iCategory, IN const int iSlotID ) const;
    bool IsEnoughItemExist( IN const std::map< int, int >& mapRequiredItem, IN const bool bOnlyInventory ) const;
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// �۾���¥: 2013-06-25	// �ڼ���
	bool IsEnoughItemExist( IN const std::map< int, int >& mapRequiredItem
		, IN const bool bOnlyInventory
		, IN const bool bOnlyInfinite	// ���� ������ ���� ��
		) const;
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
	bool IsEnoughItemExistOnlyInventory( IN const std::map< int, int >& mapRequiredItem );
    bool IsEnoughItemExist( IN const std::map< UidType, int >& mapRequiredItem );
    bool IsSuitableSlot( IN int iItemID, IN int iCategory, IN int iSlotID );
    bool IsAbleToEquip( IN int iItemID );
    bool IsEnoughSpaceExist( IN const std::map< int, int >& mapItem ); // ����� �˻�� ����, ��������, ������ ������
	//{{ 2010. 05. 03  ������	���� ����
#ifdef SERV_EMPTY_SLOT
	bool IsEnoughEmptySlot( IN int iCategory, IN int iEmptySlotNum );
#endif SERV_EMPTY_SLOT
	//}}
	bool IsPeriodExpired( IN UidType iItemUID );
	bool IsPeriodExpired( IN int iCategory, IN int iSlotID );
	//{{ 2008. 12. 21  ������	���� ������
	bool IsBrokenItem( IN UidType iItemUID );
	//}}
	//{{ 2009. 8. 7  ������		����
	CXSLInventory::MEMBERSHIP_PRIVATE_BANK GetBankMemberShip();
	int  GetMyNextUpgradeBankItemID();
	//}}
	//{{ 2009. 8. 27  ������	����
	bool IsSealedItem( IN UidType iItemUID );
	//}}
	//{{ 2010. 01. 04  ������	PC�� �����̾�
	bool IsDBUpdateItem( IN UidType iItemUID ) const;
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
    bool GetCategorySlot( IN const UidType iItemUID, OUT int& iCategory, OUT int& iSlotID ) const;
    UidType GetItemUID( IN const int iCategory, IN const int iSlotID ) const;

    int  GetItemID( IN const UidType iItemUID ) const;
    int  GetItemID( IN const int iCategory, IN const int iSlotID ) const;

    bool GetUsageType( IN const UidType iItemUID, OUT int& iUsageType ) const;
    bool GetUsageType( IN const int iCategory, IN const int iSlotID, OUT int& iUsageType ) const;

	bool GetQuantity( IN const int iItemID, OUT int& iQuantity ) const;
    bool GetQuantity( IN const UidType iItemUID, OUT int& iQuantity ) const;
    bool GetQuantity( IN const int iCategory, IN const int iSlotID, OUT int& iQuantity ) const;

	bool GetEndurance( IN const UidType iItemUID, OUT int& iEndurance ) const;
	bool GetEndurance( IN const int iCategory, IN const int iSlotID, OUT int& iEndurance )  const;
	
	bool GetEnchantLevel( IN UidType iItemUID, OUT int& iEnchantLevel );
	bool GetEnchantLevel( IN int iCategory, IN int iSlotID, OUT int& iEnchantLevel );
	
	//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	bool GetAttribEnchant( IN UidType iItemUID, OUT KItemAttributeEnchantInfo& kInfo );
	bool GetAttribEnchant( IN int iCategory, IN int iSlotID, OUT KItemAttributeEnchantInfo& kInfo );
	//}}

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetItemSocketList( IN UidType iItemUID, OUT std::vector< int >& vecItemSocket );
	bool GetItemSocketList( IN int iCategory, IN int iSlotID, OUT std::vector< int >& vecItemSocket );
//#else
//	bool GetItemSocketList( IN UidType iItemUID, OUT std::vector< short >& vecItemSocket );
//	bool GetItemSocketList( IN int iCategory, IN int iSlotID, OUT std::vector< short >& vecItemSocket );
//#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	//{{ 2010. 04. 15  ������	���� ����
//#ifdef SERV_SOCKET_NEW
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetItemSocketList( IN UidType iItemUID, OUT std::map< int, int >& mapItemSocket );
//#else
//	bool GetItemSocketList( IN UidType iItemUID, OUT std::map< int, short >& mapItemSocket );
//#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
//#endif SERV_SOCKET_NEW
	//}}
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetItemSocketOption( IN UidType iItemUID, IN const int iSocketIndex, OUT int& iSocketOption );
//#else
//	bool GetItemSocketOption( IN UidType iItemUID, IN const int iSocketIndex, OUT short& sSocketOption );
//#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	bool GetCountAssignedItemSocket( IN UidType iItemUID, OUT int& iAssignedSocketCount );

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	bool GetRandomSocketList( IN const UidType iItemUID, OUT std::vector< int >& vecRandomSocket );
	bool GetRandomSocketList( IN const int iCategory, IN const int iSlotID, OUT std::vector< int >& vecRandomSocket );
	bool GetRandomSocketList( IN const UidType iItemUID, OUT std::map< int, int >& mapRandomSocket );
	bool GetRandomSocketOption( IN const UidType iItemUID, IN const int iSocketIndex, OUT int& iSocketOptionID );
	bool GetCountAssignedRandomSocket( IN const UidType iItemUID, OUT int& iAssignedSocketCount );

	bool SetRandomSocketOption( IN const UidType iItemUID, IN const std::vector< int >& vecRandomSocket );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

    bool GetItemInfo( IN UidType iItemUID, OUT KItemInfo& kInfo ) const;
    bool GetItemInfo( IN int iCategory, IN int iSlotID, OUT KItemInfo& kInfo ) const;

    bool GetInventoryItemInfo( IN const UidType iItemUID, OUT KInventoryItemInfo& kInventoryItemInfo ) const;
    bool GetInventoryItemInfo( IN const int iCategory, IN const int iSlotID, OUT KInventoryItemInfo& kInventoryItemInfo ) const;
    bool GetInventorySlotInfo( IN const int iCategory, IN const int iSlotID, OUT KInventoryItemInfo& kInventoryItemInfo ) const;
	bool GetInventoryItemSimpleInfo( IN const UidType iItemUID, OUT KInventoryItemSimpleInfo& kInventoryItemInfo ) const;
	bool GetInventoryItemSimpleInfo( IN const int iCategory, IN const int iSlotID, OUT KInventoryItemSimpleInfo& kInventoryItemInfo ) const;

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
	void GetSpecialItem( OUT std::map< int, KInventoryItemInfo >& mapSpecialItem ) const;
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

    void GetEquippedItem( OUT std::map< int, KInventoryItemInfo >& mapEquippedItem ) const;
	void GetEquippedItem( OUT std::vector< KInventoryItemSimpleInfo >& vecEquippedItem ) const;

	//{{ 2012. 10. 09	��μ�	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
	void GetEquippedItem( IN std::set< CXSLItem::ITEM_TYPE >& setItemType, IN bool bFashion, OUT std::map< int, KInventoryItemInfo >& mapEquippedItem ) const;
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

	//{{ 2010. 10. 18	������	�� ���� ��ų
//#ifdef SERV_PET_AURA_SKILL
	//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
//#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
#ifdef SERV_EVENT_VALENTINE_RING_IS_DUNGEON
	void GetEquippedStat( IN const bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel, IN int iTempDungeonID = 0 );
#else 
	void GetEquippedStat( IN const bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel );
#endif SERV_EVENT_VALENTINE_RING_IS_DUNGEON
//#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 
//#endif SERV_PET_AURA_SKILL
	//}}
	//{{ 2009. 7. 28  ������	GM���⸸
	void GetEquippedStatOnlyGMWeapon( OUT KStat& kStat );
	//}}
	//{{ 2009. 2. 2  ������		���Ͽɼ�
	void GetEquippedItemBonusRate( IN const bool bIsDungeonRoom, OUT float& fAddTitleExpRate, OUT float& fAddTitleEDRate );
	//}}
	//{{ 2011. 10. 18	������	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
	bool GetItemLevel( IN const UidType iItemUID, OUT int& iItemLevel );
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ������ ���� �� ���� ��ȭ
    void IncreaseQuantity( IN UidType iItemUID, IN int iQuantity, OUT int& iIncreased );
    void IncreaseQuantity( IN int iCategory, IN int iSlotID, IN int iQuantity, OUT int& iIncreased );
	//{{ 2009. 12. 15  ������	������ ��������	
    void DecreaseQuantity( IN UidType iItemUID, IN int iQuantity, OUT int& iDecreased, IN KDeletedItemInfo::DELETE_REASON eReason );
    void DecreaseQuantity( IN int iCategory, IN int iSlotID, IN int iQuantity, OUT int& iDecreased, IN KDeletedItemInfo::DELETE_REASON eReason );
	//}}

	//{{ 2012. 05. 08	��μ�       ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	// DecreaseQuantity() �ʹ� ����� ���������� ���� �Ķ���Ϳ� �����Ǵ� ������������ �Ѱ� �ش�.
	void DecreaseQuantityOrDelete( IN UidType iItemUID, IN int iQuantity, OUT int& iDecreased, IN KDeletedItemInfo::DELETE_REASON eReason, OUT KInventoryItemInfo& kDeleteItemInfo );
	// �߰��� ������ �� ������ ������ �� ���� ���� �����Ѵ�.
	void FixUpInventoryItem( IN OUT std::vector< KInventoryItemInfo >& vecInsertedItem, OUT KItemQuantityUpdate& kDBUpdate, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
	//}}

    void IncreaseEndurance( IN UidType iItemUID, IN int iEndurance, OUT int& iIncreased );
    void IncreaseEndurance( IN int iCategory, IN int iSlotID, IN int iEndurance, OUT int& iIncreased );
    void DecreaseEndurance( IN UidType iItemUID, IN int iEndurance, OUT int &iDecreased );
    void DecreaseEndurance( IN int iCategory, IN int iSlotID, IN int iEndurance, OUT int& iDecreased );	
	void IncreaseEnchantLevel( IN UidType iItemUID, IN int iEnchantLevel, OUT int& iIncreased );
	void IncreaseEnchantLevel( IN int iCategory, IN int iSlotID, IN int iEnchantLevel, OUT int& iIncreased );
	void DecreaseEnchantLevel( IN UidType iItemUID, IN int iEnchantLevel, OUT int &iDecreased );
	void DecreaseEnchantLevel( IN int iCategory, IN int iSlotID, IN int iEnchantLevel, OUT int& iDecreased );

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool SetItemSocketOption( IN UidType iItemUID, IN const int iSocketIndex, IN const int iSocketOption );
	bool SetItemSocketOption( IN int iCategory, IN int iSlotID, IN const int iSocketIndex, IN const int iSocketOption );	
//#else
//	bool SetItemSocketOption( IN UidType iItemUID, IN const int iSocketIndex, IN const short sSocketOption );
//	bool SetItemSocketOption( IN int iCategory, IN int iSlotID, IN const int iSocketIndex, IN const short sSocketOption );	
//#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	
	bool SetAttribEnchant( IN UidType iItemUID, IN char cAttribEnchantSlotID, IN char cAttribEnchantType );
	bool SetAttribEnchant( IN int iCategory, IN int iSlotID, IN char cAttribEnchantSlotID, IN char cAttribEnchantType );

	//{{ 2009. 8. 27  ������	����
	bool Seal( IN UidType iItemUID );
	bool Unseal( IN UidType iItemUID );
	//}}

    bool InsertItem( IN UidType iItemUID, IN const KItemInfo& kItemInfo, OUT KInventoryItemInfo& kInventorySlotInfo );
    bool InsertItem( IN const std::map< UidType, KItemInfo >& mapItem, OUT std::vector< KInventoryItemInfo >& vecInserted );

#ifdef SERV_SHARING_BANK_TEST
	bool			IsEmptyShareBankSlot();
	bool			IsEmptyShareItem();

	void			SetShareBankSize( int iBankSize )				{ m_iShareBankSize = iBankSize; }
	void			SetShareUnitUID( UidType iShareUnitUID )		{ m_iShareUnitUID = iShareUnitUID; }

	void			SetShareNickname( std::wstring wstrNickname )	{ m_wstrShareNickname = wstrNickname; }
	std::wstring	GetShareNickname()								{ return m_wstrShareNickname; }

	int				GetShareBankSize()								{ return m_iShareBankSize; }
	UidType			GetShareUnitUID()								{ return m_iShareUnitUID; }
	bool			GetIsShareBank()								{ return (m_iShareUnitUID > 0); }
	void			GetShareItemMap( OUT std::map< UidType, KInventoryItemInfo > &mapItem );

	bool			InsertShareItem( IN const UidType iItemUID, IN const KItemInfo& kItemInfo, IN const char cSlotCategory, IN const short sSlotID );
	bool			UpdateShareItemPosition( IN const KTradeShareItemResult& kShareResult );

	//bool			GetShareItemNeedDBUpdate( OUT KDBE_UPDATE_SHARE_ITEM_REQ &kReq );
	bool			GetShareItemNeedDBUpdate( IN const std::set< UidType >& setMovedItemUID, OUT KDBE_UPDATE_SHARE_ITEM_REQ &kReq );

	void			ClearShareBank();

	bool			IsShareItem( UidType iItemUID );
#endif SERV_SHARING_BANK_TEST

	//{{ 2009. 12. 15  ������	������ ��������	
	bool DeleteItem( IN UidType iItemUID, OUT KInventoryItemInfo& kInventorySlotInfo, IN KDeletedItemInfo::DELETE_REASON eReason );
	bool DeleteItem( IN int iCategory, IN int iSlotID, OUT KInventoryItemInfo& kInventorySlotInfo, IN KDeletedItemInfo::DELETE_REASON eReason );
	bool DeleteItemOnlyInventory( IN const std::map< int, int >& mapToDelete, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, IN KDeletedItemInfo::DELETE_REASON eReason );
	//}}
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	void DeleteItemAll( IN int iItemID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2009. 5. 13  ������	�����۹�����
	bool DiscardItem( IN UidType iItemUID, IN int iQuantity, OUT KInventoryItemInfo& kInventorySlotInfo, IN bool bDebug = false );
	//}}

	bool UseQuickSlotItem( IN int iSlotID, OUT int& iItemID, OUT KInventoryItemInfo& kInventorySlotInfo );
	//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool UseDefenceDungeonQuickSlotItem( IN const int iSlotID, OUT int& iItemID, OUT KInventoryItemInfo& kInventorySlotInfo );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
 
	void ResetQuickSlotCoolTime();

#ifdef SERV_SHARING_BANK_TEST
#ifdef SERV_PERSONAL_SHOP_NO_MOVE
	bool MoveItem( IN UidType iItemUID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq, IN std::vector< KSellPersonalShopItemInfo > vecPersonalShopItemInfo );
	bool MoveItem( IN int iSrcCategory, IN int iSrcSlotID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq, IN std::vector< KSellPersonalShopItemInfo > vecPersonalShopItemInfo );
#else
	bool MoveItem( IN UidType iItemUID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq );
	bool MoveItem( IN int iSrcCategory, IN int iSrcSlotID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged, OUT KDBE_SEAL_ITEM_REQ &kSealReq );
#endif SERV_PERSONAL_SHOP_NO_MOVE
#else SERV_SHARING_BANK_TEST
    bool MoveItem( IN UidType iItemUID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged );
    bool MoveItem( IN int iSrcCategory, IN int iSrcSlotID, IN int iDestCategory, IN int iDestSlotID, IN bool bCoolTimeCheck, OUT std::vector< KInventoryItemInfo >& vecChanged );
#endif SERV_SHARING_BANK_TEST

	//{{ 2009. 6. 1  ������		�κ��丮 ����
	bool SortInventory( IN int iCategory, OUT std::vector< KInventoryItemInfo >& vecChanged );
	//}}

	//{{ 2011. 06. 18	������	ED������ ���� üũ
//#ifdef SERV_CHECK_BUY_ED_ITEM
	bool PrepareBuy( IN const std::map< int, int >& mapItem, IN const bool bDebug, IN const short sPeriod, OUT int& iTotalED, OUT int& iTotalPvPPoint, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KBuyGPItemInfo >& vecNewItem );
//#endif SERV_CHECK_BUY_ED_ITEM
	//}}    
    bool ProcessSell( IN const UidType iItemUID, IN const int iQuantity, OUT int& iItemID, OUT int& iED, OUT KInventoryItemInfo& kInventoryItemInfo );
    bool PrepareInsert( IN std::map< int, KItemInfo >& mapReward, OUT std::map< int, int >& mapReceived, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KItemInfo >& vecNewItem, IN bool bTransaction = false );
	//{{ 2009. 3. 16  ������	�������� �ƴ� �����ۿ� ���� prepareinsert
	bool PrepareInsertForTrade( IN const std::vector< KInventoryItemInfo >& vecReward, OUT std::map< int, int >& mapReceived, OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, OUT std::vector< KItemInfo >& vecNewItem );
	//}}

	//{{ 2009. 12. 15  ������	������ ��������	
    bool DeleteAndInsert( IN const std::map< int, int >& mapToDelete, 
						  IN const std::map< int, KItemInfo >& mapToInsert, 
						  OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, 
						  OUT std::vector< KItemInfo >& vecNewItem, 
						  IN bool bDeleteOnlyInventory, 
						  IN KDeletedItemInfo::DELETE_REASON eReason
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// �۾���¥: 2013-07-01	// �ڼ���
						  , IN const bool bOnlyInfinite = false	// ���� ������ ���� ��
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
						  );

    bool DeleteAndInsert( IN const std::map< UidType, int >& mapToDelete, 
						  IN const std::map< int, KItemInfo >& mapToInsert, 
						  OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, 
						  OUT std::vector< KItemInfo >& vecNewItem, 
						  IN KDeletedItemInfo::DELETE_REASON eReason
						  );

    bool DeleteAndInsert( IN const std::map< int, int >& mapToDeleteByItemID, 
						  IN OUT std::map< UidType, int >& mapToDeleteByItemUID, 
						  IN const std::map< int, KItemInfo >& mapToInsert, 
						  OUT std::vector< KInventoryItemInfo >& vecUpdatedItem, 
						  OUT std::vector< KItemInfo >& vecNewItem, 
						  IN bool bDeleteOnlyInventory, 
						  IN KDeletedItemInfo::DELETE_REASON eReason
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// �۾���¥: 2013-07-01	// �ڼ���
						  , IN const bool bOnlyInfinite = false	// ���� ������ ���� ��
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
						  );
	//}}

    void FlushQuantityChange( OUT std::map< UidType, int >& mapChanged );
    void RollBackInitQuantity( IN const std::map< UidType, int >& mapChanged );
	void FlushEnduranceChange( OUT std::map< UidType, int >& mapChanged );
	void RollBackInitEndurance( IN const std::map< UidType, int >& mapChanged );
	//{{ 2010. 8. 3	������	�� �ý���
//#ifdef SERV_PET_SYSTEM
	void FlushPositionChange( OUT std::map< UidType, KItemPosition >& mapChanged );
	void PositionUpdateComplete( IN const std::map< UidType, KItemPosition >& mapChanged );
//#else
//	void FlushPositionChange( OUT std::map< UidType, std::pair< int, int > >& mapChanged );
//	void PositionUpdateComplete( IN const std::map< UidType, std::pair< int, int > >& mapChanged );
//#endif SERV_PET_SYSTEM
	//}}	
	//{{ 2009. 12. 15  ������	������ ��������
	void FlushDeletedItem( OUT std::vector< KDeletedItemInfo >& vecDeleted );
	void RollBackDeletedItem( IN const std::vector< KDeletedItemInfo >& vecDeleted );
	//}}

	//������ �������� �������� ����Ʈ����.
	void DecreaseEquippedItemEnduranceInPvp();
	void DecreaseEquippedItemEnduranceInDungeon( IN const int iExpDropNpcNum, 
												 IN const int iPartyNumMember, 
												 OUT std::vector< KDecreaseEnduranceInfo >& vecSlotInfo, 
												 OUT std::vector< KInventoryItemInfo >& vecZeroEndurance );
	
	void DecreaseEquippedItemEndurance( IN const int iEnduranceDamage, 
										IN const int iExpDropNpcNum, 
										IN const int iPartyNumMember, 
										OUT std::vector< KDecreaseEnduranceInfo >& vecSlotInfo, 
										OUT std::vector< KInventoryItemInfo >& vecZeroEndurance );

	//{{ 2012. 03. 06	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void DecreaseEquippedItemEnduranceInBattleField( IN const int iExpDropNpcNum, 
													 IN const int iPartyNumMember, 
													 OUT std::vector< KDecreaseEnduranceInfo >& vecSlotInfo, 
													 OUT std::vector< KInventoryItemInfo >& vecZeroEndurance );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	bool RepairItem( IN const std::vector< UidType > vecItemUID, OUT int& iED, OUT int &iVP, OUT std::vector< KInventoryItemInfo >& vecUpdated, IN KGSUserPtr pUser );
#endif SERV_SERVER_BUFF_SYSTEM
		//}

	bool GetZeroOfEndurancebyEquippedItem( IN std::vector<UidType>& vecItemUID, OUT std::vector<UidType>& vecZeroItemUID );	//������ �������� �������̰� �������� 0�� �������� ã�´�. 

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT //SERV_ALL_RENEWAL_SP
	//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	bool OpenRandomItem( IN char cUnitClass, IN UidType iItemUID, IN const int iEventItemID, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem, OUT int& iRessurectionCount, OUT int& iRestoreSpirit, OUT int& iED, OUT bool& bCharmItem );
#else
	bool OpenRandomItem( IN char cUnitClass, IN UidType iItemUID, IN const int iEventItemID, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem, OUT int& iRessurectionCount, OUT int& iRestoreSpirit, OUT bool& bCharmItem );
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}
#else //SERV_ALL_RENEWAL_SP
	bool OpenRandomItem( IN char cUnitClass, IN UidType iItemUID, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem, OUT int& iRessurectionCount, OUT int& iRestoreSpirit, OUT int& iED, OUT bool& bCharmItem );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT //SERV_ALL_RENEWAL_SP
	//}}    

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
	bool OpenRandomItem_DEV( IN char cUnitClass, IN const int UsediItemID, IN const int iItemKeyID, OUT std::map< int, int >& mapInsertedItem, OUT int& iRessurectionCount, OUT int& iRestoreSpirit, OUT bool& bCharmItem );
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_MANUFACTURE_FIX
	bool ItemManufacture( IN int iManufactureID, IN int iQuantity, OUT int& iED, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem );
#else
	bool ItemManufacture( IN int iManufactureID, OUT int& iED, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem );
#endif

	bool ResolveItem( IN UidType iItemUID, IN OUT bool& bJackpot, OUT bool& bFashion, OUT int& iResolveQuantity, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem, IN bool bForceInsertItem = false );
	//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
	bool ResolveAvatarItem( IN OUT bool& bJackpot, IN std::map< int, UidType >& mapResolveItem , OUT int& iED, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem );
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
	bool EnchantItem( IN UidType iItemUID, IN bool bIsRareEnchantStone, IN bool bIsNewEnchant, IN bool bIsSupportMaterial, IN bool bIsDestroyGuard, IN bool bIsEnchantPlus, IN bool bDebug, OUT int& iEnchantResult, OUT int& iLevelAfterEnchant, OUT int& iED, OUT std::vector< KInventoryItemInfo >& vecUpdated );
#else
	bool EnchantItem( IN UidType iItemUID, IN bool bIsRareEnchantStone, IN bool bIsNewEnchant, IN bool bIsSupportMaterial, IN bool bDebug, OUT int& iEnchantResult, OUT int& iLevelAfterEnchant, OUT int& iED, OUT std::vector< KInventoryItemInfo >& vecUpdated );
#endif
	//}}
	//{{ 2008. 12. 21  ������	��ȭ ����
	bool RestoreItem( IN UidType iItemUID, OUT int& iLevelAfterEnchant, OUT std::vector< KInventoryItemInfo >& vecUpdated );
	//}}
	//{{ 2010. 04. 06  ������	������ ����
//#ifdef SERV_SOCKET_NEW
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool SocketItem( IN UidType iItemUID
				   , IN const std::map< int, UidType >& mapSocketInfo
				   , IN bool bCheat
				   , OUT int& iTargetItemID
				   , OUT int& iED
				   , OUT std::vector< int >& vecSocketResult
				   , OUT std::vector< KInventoryItemInfo >& vecUpdated
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
				   , OUT byte& byteExpandedSocketNum
#endif // SERV_BATTLE_FIELD_BOSS
				   );
//#endif SERV_ITEM_OPTION_DATA_SIZE
//	//}} 
//#endif SERV_SOCKET_NEW
	//}}
	bool AttribEnchantItem( IN UidType iItemUID, IN char cAttribEnchantSlotNo, IN char& cAttribEnchantType, OUT std::vector< KInventoryItemInfo >& vecUpdated );
	bool IdentifyItem( IN UidType iItemUID, IN int iQuantity, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem );
	bool UseItemInInventory( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, IN bool bNotDeleteItem = false );

	//{{ 2013. 02. 19   ��ȯ �α� �߰� - ��μ�
#ifdef SERV_EXCHANGE_LOG
#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	bool ItemExchangeCheck( IN const int iHouseID
		, IN const UidType iSourceItemUID
		, IN const int iSourceItemID
		, IN const int iSourceQuantity
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
		, IN int iEventMoney
#endif // SERV_EVENT_MONEY
		, IN const int iDestItemID
		);
#endif // SERV_ITEM_EXCHANGE_LIMIT

	bool ItemExchange( IN const int iHouseID, 
		IN const UidType iSourceItemUID, 
		IN const int iSourceItemID, 
		IN const int iSourceQuantity, 
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
		IN OUT int& iEventMoney, 
#endif // SERV_EVENT_MONEY
		IN const int iDestItemID, 
		OUT std::map< int, int >& mapInsertedItem, 
		OUT std::vector< KInventoryItemInfo >& vecUpdated, 
		OUT std::vector< KItemInfo >& vecNewItem,
		OUT char& cExchangeType	);
#else
/*
//{{ 2011. 08. 16	������	������ ��ȯ ����
//#ifdef SERV_ITEM_EXCHANGE_NEW
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	bool ItemExchange( IN const int iHouseID, 
						IN const UidType iSourceItemUID, 
						IN const int iSourceItemID, 
						IN const int iSourceQuantity, 
						IN const int iDestItemID, 
						OUT std::map< int, int >& mapInsertedItem, 
						OUT std::vector< KInventoryItemInfo >& vecUpdated, 
						OUT std::vector< KItemInfo >& vecNewItem,
						//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ ��ȯ �α�
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
						IN int&	iEventMoney,
						OUT std::vector<int>& vecDestItem
#else
						IN int&	iEventMoney
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
						//}}
						);
#else // SERV_EVENT_MONEY
	bool ItemExchange( IN const int iHouseID, 
						IN const UidType iSourceItemUID, 
						IN const int iSourceItemID, 
						IN const int iSourceQuantity, 
						IN const int iDestItemID, 
						OUT std::map< int, int >& mapInsertedItem, 
						OUT std::vector< KInventoryItemInfo >& vecUpdated, 
						OUT std::vector< KItemInfo >& vecNewItem );
#endif // SERV_EVENT_MONEY
//#else
//	bool ItemExchange( IN const int iItemID, IN const int iHouseID, IN const int iSelectedItemID, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem );
//#endif SERV_ITEM_EXCHANGE_NEW
	//}}
*/
#endif SERV_EXCHANGE_LOG
	//}
		
	//{{ 2008. 12. 25  ������	�ο�
	bool EnchantAttachItem( IN UidType iDestItemUID, IN UidType iAttachItemUID, OUT int& iLevelAfterEnchant, OUT std::vector< KInventoryItemInfo >& vecUpdated );
	//}}
	//{{ 2009. 8. 27  ������	����
	bool SealItem( IN UidType iDestItemUID, IN UidType iSealItemUID, OUT u_char& ucBeforeSealCount, OUT u_char& ucSealResult, OUT std::vector< KInventoryItemInfo >& vecUpdated );
	bool UnsealItem( IN UidType iDestItemUID, OUT u_char& ucSealResult, OUT std::vector< KInventoryItemInfo >& vecUpdated );
	//}}

	//{{ 2009. 9. 22  ������	���
#ifdef GUILD_TEST
	bool UseGuildFoundationItemCheckOnly( IN UidType iItemUID );
	bool UseGuildFoundationItem( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecUpdated, IN bool bCheckOnly );
#endif GUILD_TEST
	//}}

	//{{ 2009. 11. 24  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	bool ResetGuildSkillItemCheckOnly();
	bool ResetGuildSkillItem( OUT std::vector< KInventoryItemInfo >& vecInventorySlotnfo );
	bool InitGuildSkillItemCheckOnly( IN UidType iItemUID );
	bool InitGuildSkillItem( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotnfo );
#endif GUILD_SKILL_TEST
	//}}

	int  CheckItemForTrade( IN UidType iItemUID, IN int iQuantity, OUT KInventoryItemInfo& kInventoryItemInfo );
#ifdef SERV_DELETE_CUBE_GIVE_POST
	void ExpireItem( OUT std::vector< UidType >& vecItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, OUT std::vector< KInventoryItemInfo >& vecDeleteItem);	
#else SERV_DELETE_CUBE_GIVE_POST
    void ExpireItem( OUT std::vector< UidType >& vecItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );	
#endif SERV_DELETE_CUBE_GIVE_POST
	
	bool ResetSkillItem( OUT KInventoryItemInfo& kInventoryItemInfo );

    bool CompareUnitClass( IN int iItemID );
    bool IsAbleToExpandSlot( IN int nSlot );

	//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
	bool IsAbleToExpandSlot( IN int iCategory, IN int nSlot );			// Ȯ��� ������ �������� Ȯ�ο�( nSlot ��ŭ �������� �ִ°�)
#endif SERV_EXPAND_QUICK_SLOT
	//}}

	//{{ 2009. 8. 7  ������		����
	bool IsAbleToExpandBank( IN int nSlot );
	//}}
    void ExpandSlot( IN int iCategory, IN int iSlot, OUT int& iExpanded );

	//{{ 2009. 2. 10  ������	���λ��� ���׿���
	bool CheckTradeItemOut( IN const std::vector< KInventoryItemInfo >& vecItemOut, OUT KItemQuantityUpdate& kUpdateTemp, IN CXSLRoom::ROOM_TYPE eType );
	bool CheckTradeItemIn( IN const std::vector< KInventoryItemInfo >& vecItemIn, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp, IN CXSLRoom::ROOM_TYPE eType );

	bool TrialPrepareInsert( IN const std::vector< KInventoryItemInfo >& vecReward, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp );
	bool TrialPrepareInsert( IN const std::map< int, KItemInfo >& mapReward, OUT KItemQuantityUpdate& kUpdateTemp, OUT std::vector< KItemInfo >& vecUpdateTemp );
	//}}

	//{{ 2010. 02. 08  ������	PC�� ä�� �̵� ���� ����
	void GetChannelChangeInventoryInfo( OUT std::vector< KInventoryItemInfo >& vecPcBangItemInfo );
	void SetChannelChangeInventoryInfo( IN const std::vector< KInventoryItemInfo >& vecPcBangItemInfo, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	//}}

	//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	void InitPetInventory( IN UidType iSummonedPetUID, IN int iPetCategorySlotSize, IN const std::map< UidType, KInventoryItemInfo >& mapPetItem, OUT std::vector< KInventoryItemInfo >& vecPetInventorySlotInfo );
	void ResetPetInventorySize( IN int iPetCategorySlotSize );
	void RemovePetInventory( OUT std::map< UidType, KInventoryItemInfo >& mapRemovedPetItem );	
	bool FeedPetInInventory( IN UidType iItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	bool AutoFeedInPetInventory( IN u_char ucLevel, IN bool bOnlyEatSpecial, OUT int& iFeedItemID, OUT std::vector< KInventoryItemInfo >& vecPetInventorySlotInfo );
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2010. 8. 16	������	�Ⱓ ���� ������ �̺�Ʈ
#ifdef SERV_RESET_PERIOD_EVENT
	void GetResetPeriodItemUIDList( OUT std::map< UidType, short >& mapExpandPeriodItemList ) const;
	bool UpdatePeriod( IN UidType iItemUID, IN short sPeriod, IN const std::wstring& wstrExpirationDate, OUT KInventoryItemInfo& kInventorySlotInfo );
#endif SERV_RESET_PERIOD_EVENT
	//}}

	//{{ 2011. 01. 04  ��μ�	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM
	bool AttribAttachItem( IN UidType iDestItemUID, IN UidType iAttachItemUID, OUT KItemAttributeEnchantInfo& kAttribAttachResult, OUT std::vector< KInventoryItemInfo >& vecUpdated );
#endif SERV_ATTRIBUTE_CHARM
	//}}

	//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	bool DeleteItemForPersonalShopAgency( IN const std::vector< KSellPersonalShopItemInfo >& vecSellItemInfo, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2011. 01. 06  ��μ�  ��ų����ü���� üũ(�κ��丮-�Ⱓ��) ��� ����
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
	bool GetSlotChangeBEndDate( IN const int iItemID, IN OUT std::wstring& wstrExpirationDate );
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY

	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	bool GiveRetainingRewardItem( IN const KRewardItem& kItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 

	//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void SetClassChangeItem(  IN std::map< UidType, int >& mapItem, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo  );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

//	//{{ 2012. 02. 21	��μ�	�κ��丮 ����
//#ifdef SERV_SORT_CATEGORY_ITEM 
//	//{{ 2012. 02. 27	��μ�	�κ��丮 ����	- ���ſ� ����� ������ ����
//#ifdef SERV_SORT_CATEGORY_ITEM_FIX
	typedef std::pair< int, UidType >	KInventoryItemSortInfo;		// < itemid, itemuid >

#ifdef SERV_PERSONAL_SHOP_NO_MOVE
	bool SortCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot, OUT int& iNetError, IN const std::vector< KSellPersonalShopItemInfo > vecPersonalShopSell_ItemInfo );
#else //SERV_PERSONAL_SHOP_NO_MOVE
	bool SortCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot, OUT int& iNetError );
#endif //SERV_PERSONAL_SHOP_NO_MOVE
	void SortEquipCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot );
	void SortConsumptionCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot );
	void SortNormalCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot );
//#else
//	bool SortCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot, OUT int& iNetError );
//	void SortEquipCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot );
//	void SortConsumptionCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot );
//	void SortNormalCategory( IN const int iCategory, OUT std::vector< UidType >& vecUpdatedInventorySlot );
//#endif SERV_SORT_CATEGORY_ITEM_FIX
//	//}}
//#endif SERV_SORT_CATEGORY_ITEM 
	//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	void	SetUseItemNeedPayment(bool bVal ) {	m_bUseItemNeedPayment = bVal; }
	bool	GetUseItemNeedPayment() const { return m_bUseItemNeedPayment; }
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

	//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
	bool IsLocked()																						{ return m_kInventoryLocker.IsLocked(); }
	bool Lock( IN const KInventoryLocker::LOCK_TYPE eLockType, IN const double fLockTime = 99999.0 )	{ return m_kInventoryLocker.Lock( eLockType, fLockTime ); }
	void Unlock()																						{ m_kInventoryLocker.Unlock(); }
#endif SERV_INVENTORY_LOCK
	//}}

	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	bool CheckPetToyItem( void );
#endif SERV_PET_TOY
	//}}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	bool CheckKeepItem( const int nProductID );
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP
	//{{ 2012. 08. 23	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	bool FixUpInventoryItemForLetter( IN const std::vector< KInventoryItemInfo >& vecCreatedItem
		, OUT KItemQuantityUpdate& kDBUpdate
		, OUT std::vector< KInventoryItemInfo >& vecMoveItemSlotInfo
		, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo
		);
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}

	//{{ 2012. 10. 04	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	const KItemQuantityVariation& GetItemQuantityVariation() { return m_kItemQuantityVariation; }
	void ClearItemQuantityVariation()						 { m_kItemQuantityVariation.Clear(); }
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
#ifdef SERV_REFORM_INVENTORY_TEST
	int		GetExpandedSlotSize( IN const int iCategory ) const	{ return GetSlotSize( iCategory ) - GetBaseSlotSize( iCategory ); }
#endif SERV_REFORM_INVENTORY_TEST
	//}}
	//{{ 2011.05.04   �ӱԼ� �ƹ�Ÿ �ռ� �ý���
#ifdef SERV_SYNTHESIS_AVATAR
	bool OpenSynthesisItem( IN UidType iKeyItemUID, IN int iSocketNo , IN std::map<int,UidType> mapSynthesisData , OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT std::vector< KItemInfo >& vecNewItem, OUT std::set< int > &setSealCashItem);
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	bool CheckKeepItem( void );
	bool CheckKeepItem2( void );
	bool CheckKeepItem3( void );
	bool CheckKeepItem4( void );
	bool CheckKeepItem5( void );
	bool CheckKeepItem6( void );
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_READY_TO_SOSUN_EVENT
	bool ExchangeToEvent( IN int iDeleteItemID, IN int iDeleteItemCount, IN int iInsertItemID, IN int iInsertItemCount, OUT std::vector< KInventoryItemInfo >& vecUpdatedInventorySlot, OUT std::vector< KItemInfo >& vecNewItemInfo, IN int iFirstUnitClass  );
#endif SERV_READY_TO_SOSUN_EVENT

//{{ 2010. 10. 27	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	void GetItemUIDListThisItem( IN int iItemID, OUT std::vector< UidType >& vecItemUIDList, IN bool bOnlyInventory );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	void	SetupDefenceDungeonQuickSlotItems( IN const int iDefenceDungeonPhase, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	bool	ItemEvaluateCheck( IN const UidType iItemUID, OUT std::vector< int >& vecRandomSocket, OUT int& iCost );
	bool	ItemEvaluateResult( IN const UidType iItemUID, IN const std::vector< int >& vecRandomSocket, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );

	bool	RestoreItemEvaluateCheck( IN const UidType iSupportItemUID, IN const UidType iTargetItemUID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	bool	RestoreItemEvaluateResult( IN const UidType iTargetItemUID, IN OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo );

	bool	ItemConvert( IN const UidType& iItemUID, IN const int& iItemQuantity_, OUT std::map< int, int >& mapInsertedItem, OUT std::vector< KItemInfo >& vecNewItem, OUT std::vector< KInventoryItemInfo >& vecUpdated, OUT int& iCommissionED );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

#ifdef SERV_RECRUIT_EVENT_BASE
	void DeleteItemAll( IN int iItemID, OUT std::vector< KInventoryItemInfo >& vecInventorySlotInfo, IN KDeletedItemInfo::DELETE_REASON eReason );
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
	bool IsPossibleExtractItem( const int iItemID );

	bool ItemExtract( IN const UidType iSourceItemUID, 
		IN const int iSourceQuantity, 
		OUT std::map< int, int >& mapInsertedItem, 
		OUT std::vector< KInventoryItemInfo >& vecUpdated, 
		OUT std::vector< KItemInfo >& vecNewItem	);

	bool UseFinalitySkill( IN const UidType iSourceItemUID, 
		IN const int iSourceQuantity, 
		OUT std::map< int, int >& mapInsertedItem, 
		OUT std::vector< KInventoryItemInfo >& vecUpdated, 
		OUT std::vector< KItemInfo >& vecNewItem	);
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_GOOD_ELSWORD
    int GetNextUpgradeBankED( IN const int& iNextGrade_ );
    int GetNextUpgradeInventoryED( IN const int& iNextGrade_ );

#endif // SERV_GOOD_ELSWORD

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	bool	UpdateExpandedSocketNum( IN UidType iItemUID, IN const byte byteExpandedSocketNum );
#endif // SERV_BATTLE_FIELD_BOSS


protected:
    bool VerifyCategorySlotID( IN const int iCategory, IN const int iSlotID ) const;
	bool VerifyInventoryCategory( IN int iItemID, IN int iCategory );
    int  GetRemainedQuantityCapacity( IN UidType iItemUID );
    int  GetRemainedQuantityCapacity( IN int iCategory, IN int iSlotID );
    int  GetEnduranceToRestore( IN UidType iItemUID );
    int  GetEnduranceToRestore( IN int iCategory, IN int iSlotID );
    bool CompareLevel( IN int iItemID );
	//{{ 2012. 07. 2	�ڼ���	�Һ� ������ ȹ�� �� ������ �ڵ� ���
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	class GatheringClass
	{
	private:
		int									m_iAmountOfQuantity;
		int									m_iDecreaseQuantity;
		std::vector< KInventoryItemInfo >	m_vecInventoryItemInfo;

	public:
		GatheringClass();
		void push_back( IN const KInventoryItemInfo& kInventoryItemInfo );
		int GetAmountOfQuantity( void );
		void DecreaseTheAmountOfQuantity_Buffer( IN const int& iDecreaseQuantity );
		void DecreaseTheAmountOfQuantity( IN const int& iDecreaseQuantity );
		void FlushDecreaseQuantity( void );
		std::vector< KInventoryItemInfo >& GetVecInventoryItemInfo( void );
	};

	void GetSuitableCategory( IN int iItemID, OUT std::vector<int>& vecCategoryID );
	bool IsValidCategory( IN const int& iCategoryID );
	bool IsValidCategory( IN const int& iCategoryID, IN const int& iItemID );

	int GetEmptySlotNum( IN const int& iCategoryID );
	
	// ī�װ� ����
	bool FillTheUnFullSlot( IN const int iCategory
						  , IN const int iItemID
						  , IN int iQuantity
						  , IN OUT std::vector< UidType >* const p_vecInventorySlot
						  , IN OUT std::map< UidType, KInventoryItem >& mapItem
						  , IN OUT KItemQuantityUpdate* const p_kDBUpdate
						  , IN OUT std::map< UidType, KInventoryItemInfo >* const p_mapSlotInfo
						  , IN OUT std::vector< KInventoryItemInfo >* const p_vecUpdatedItem
						  , OUT int& iFilledQuantity
						  );
	
	// ���纻�� ���Ƿ� ä���ֱ� ���� �׽�Ʈ�� �Լ�.
	// ��¥ m_vecInventorySlot�� m_mapItem�� ������ ���ڷ� �־ �ȵȴ�.
	bool InsertToEmptySlotForTest( IN const int iCategory
								 , IN const int iItemID
								 , IN int iQuantity
								 , IN const KItemInfo* const p_kItemInfo
								 , IN const bool* const p_bDebug
								 , IN const short* const p_sPeriod
								 , IN OUT std::vector< UidType >* const p_vecInventorySlot
								 , IN OUT std::map< UidType, KInventoryItem >& mapItem
								 , IN OUT std::vector< KItemInfo >* const p_vecNewItem
								 , IN OUT std::vector< KBuyGPItemInfo >* const p_vecNewEDItem
								 , OUT int& iInsertedQuantity
								 );
#else
    int  GetSuitableCategory( IN int iItemID );
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

    bool GetEmptyCategorySlotForItem( IN int iItemID, OUT int& iCategory, OUT int& iSlotID );
	void GetNumFreeSlot( OUT std::map< int, int >& mapNumFreeSlot );
    void GetInventoryItemInfoContainingThisItem( IN const int iItemID, OUT std::vector< KInventoryItemInfo >& vecInventoryItemInfo, const IN bool bOnlyInventory );
#ifdef SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX// �۾���¥: 2013-07-01	// �ڼ���
	void GetInventoryItemInfoContainingThisItem( IN const int iItemID
		, OUT std::vector< KInventoryItemInfo >& vecInventoryItemInfo
		, IN const bool bOnlyInventory
		, IN const bool bOnlyInfinite	// ���� ������ ���� ��
		);
#endif // SERV_PERIOD_ITEM_MANUFACTURE_EXCHANGE_FIX
	float GetRepairItemDiscount();
	int	 GetEquippedSetItemCount( IN int iSetID );

	//{{ 2011. 01. 20	������	������ ���� ����
#ifdef SERV_ITEM_RESOLVE_NEW
	bool GetSellPrice( IN const UidType iItemUID, IN const int iQuantity, OUT int& iSellPrice );
#endif SERV_ITEM_RESOLVE_NEW
	//}}

#ifdef	SERV_SHARING_BANK_TEST
	bool CheckCanMoveShareItem( IN UidType iItemUID, OUT KDBE_SEAL_ITEM_REQ &kSealReq );		//	������� ������ �̵����� üũ
#endif	SERV_SHARING_BANK_TEST

	//{{ 2011. 09. 26  �ӱԼ� Īȣ ������ ���� �Ӽ� ����
#ifdef SERV_TITLE_REPAIR_DISCOUNT
	float GetRepairTitleDiscount( IN const int iTitleID );

#endif SERV_TITLE_REPAIR_DISCOUNT
	//}}

protected:
    std::map< UidType, KInventoryItem >			m_mapItem;									// �κ��丮 ������ ����
	std::vector< KDeletedItemInfo >				m_vecDeletedItem;							// ���� ������ ������ ����(���� ���� ����)
    std::vector< UidType >						m_vecInventorySlot[CXSLInventory::ST_END];	// �κ��丮 ���� ����
    std::map< int, KTimerManager >				m_mapItemCoolTime;							// ������ ��Ÿ��
    KGSUser*						            m_pUser;
	std::set< int >								m_setGetMyInventory;						// Ŭ���̾�Ʈ�� �κ��丮 ���� ���������� Ȯ���ϱ� ���� ����

#ifdef	SERV_SHARING_BANK_TEST
	int											m_iShareBankSize;
	UidType										m_iShareUnitUID;
	std::wstring								m_wstrShareNickname;						//	������� �ϰ��ִ� �г���

	std::set< UidType >							m_setShareItem;								//	������� �뵵�� ������ ������
	std::vector< UidType >						m_vecDeletedShareItem;						//	���࿡�� ���������� �����Ǿ�� �� ������ �߰�
#endif	SERV_SHARING_BANK_TEST

	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	std::map< UidType, KInventoryItem >			m_mapDeletedItem;
#endif SERV_GUILD_CHANGE_NAME
	//}}
	//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
	KInventoryLocker							m_kInventoryLocker;							// �κ��丮 ��� ��ġ
#endif SERV_INVENTORY_LOCK
	//}}
	//{{ 2012. 10. 04	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	KItemQuantityVariation						m_kItemQuantityVariation;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	bool										m_bUseItemNeedPayment;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
};


