#pragma once

#define MAX_INVENTORY_SIZE 24
const static int s_iMaxInventorySize = 64;

class CX2Unit;
class CX2Inventory
{
	public:
		enum SORT_TYPE
		{
			ST_NONE = 0,
			ST_EQUIP,		// [1]  ���
			ST_ACCESSORY,	// [2]  �Ǽ��縮
			ST_MATERIAL,	// [3]  ���
			ST_SPECIAL,		// [4]  Ư��
			ST_QUEST,		// [5]  ����Ʈ			
			ST_QUICK_SLOT,	// [6]  �Һ�
			ST_AVARTA,		// [7]  �ƹ�Ÿ
			//{{ 2009. 8. 3  ������		����ī�װ�
			ST_BANK,		// [8]  ����
			//}}
			ST_E_EQUIP,		// [9]  �������� ���
			ST_E_SKILL,		// [10] �������� ��ų(�Ⱦ�)
			ST_E_QUICK_SLOT,// [11] �������� �Һ���
			ST_PC_BANG,		// [12] PC�� ���� �κ�
			//{{ 2010. 8. 2	������	�� �ý���
			ST_PET,			// [13] �� �κ��丮
			//}}
			//#ifdef	SERV_SHARING_BANK_TEST
			ST_SHARE_BANK,	// [14] ���� ����
			//#endif	SERV_SHARING_BANK_TEST
			//{{ 2013. 04. 10	������	����� �� ����
			//#ifdef SERV_NEW_DEFENCE_DUNGEON
			ST_E_DEFENCE_QUICK_SLOT, // [14] ����ǹ� ���� ������
			//#endif SERV_NEW_DEFENCE_DUNGEON
			//}}
			
			ST_END,
		};

		enum	MEMBERSHIP_PRIVATE_BANK
		{
			MPB_NORMAL		=	8,
			MPB_SILVER		=	16,
			MPB_GOLD		=	24,
			MPB_EMERALD		=	32,
			MPB_DIAMOND		=	40,
			MPB_PLATINUM	=	48,
		};

#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
		enum SEAL_COUNT
		{
			SEAL_COUNT_MAX = 255,
		};
#endif //SERV_REMOVE_SEAL_COUNT_DECREASE

private:
		struct InvenSlotData
		{
			CX2Inventory::SORT_TYPE	m_SortType;
			int						m_SlotID;
			
			CX2Item*	m_pItem;

			InvenSlotData()
			{
				m_SortType	= CX2Inventory::ST_NONE;
				m_SlotID	= -1;
				m_pItem		= NULL;
			}
			~InvenSlotData();
		};

	public:
//{{ robobeg : 2013-11-01
		//CX2Inventory( CX2Unit* pOwnerUnit );
        CX2Inventory();
        void        SetOwnerUnit( CX2Unit* pOwnerUnit ) { m_pOwnerUnit = pOwnerUnit; }
//}} robobeg : 2013-11-01
		~CX2Inventory(void);

		void		Clear();

		void		Reset( std::map< int, int >& mapInventorySlotSize, std::map< UidType, KInventoryItemInfo >& mapItem );

		//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
		void		ResetSize( const std::map<int, int>& mapInventorySize );
		void		ResetItems( const std::map< UidType, KInventoryItemInfo >& mapItem );
		void		RemovePCBangEquips();		// ���� �������� �����۵� �߿� PC�� �� ����!!
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
		
		//{{ kimhc // 2009-08-07 // ĳ���ͺ� ����
#ifdef	PRIVATE_BANK	
		void		SetBank( std::map< UidType, KInventoryItemInfo >& mapItem );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-07 // ĳ���ͺ� ����

#ifdef SERV_PET_SYSTEM
		void		RemovePetInventory();
		void		SetPetInventory( const std::vector<KInventoryItemInfo>& vecInventorySlotInfo, int iItemMaxNum );
#endif SERV_PET_SYSTEM

		void		UpdateInventorySlotList( const std::vector<KInventoryItemInfo>& vecInventorySlotInfo );

		//{{ 2012. 02. 21	��μ�	�κ��丮 ����
#ifdef SERV_SORT_CATEGORY_ITEM 
		bool		UpdateCategorySlotList( SORT_TYPE sortType, std::vector<UidType>& vecUpdatedInventorySlot );
#endif SERV_SORT_CATEGORY_ITEM 
		//}}

		bool		AddItem( SORT_TYPE sortType, int slotID, CX2Item* pItem );
		bool		AddItem( SORT_TYPE sortType, int slotID, CX2ItemData_Base& kItemData );
	
		int			GetNumItemByTID( int itemTID, bool bExcludeEquipped = false, bool bIncludeBankType = false ) const;				// TID�� �ش��ϴ� �������� ������ return

		//{{ mauntain : ����ȯ [2012.07.16] �κ��丮 �� ������ ����� ������ ���� ���� ����
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		int			GetNumSlotByTID( int itemTID ) const;			/// �ش� ������ ���̵� �ִ� Slot�� ������ ��ȯ
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
		//}}

		CX2Item*	GetItemByTID( int itemTID, bool bIncludeBankType = false, bool bIncludeEEquipType =  true ) const;
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
		int			GetItemByTIDCheckAll( int itemTID, bool bIncludeBankType = false );											// TID�� �ش��ϴ� �������� ������ return�ϵ�, ���δ��ؼ� ����
#endif SERV_EVENT_TEAR_OF_ELWOMAN
		CX2Item*	GetItem( UidType itemUID, bool bIncludeBankType	= false ) const;
		CX2Item*	GetItem( SORT_TYPE sortType, int slotID ) const;

		SORT_TYPE	GetItemSortType( UidType itemUID ) const;

		void		SetItemMaxNum( SORT_TYPE sortType, int maxNum );
		// GetItemMaxNum�� ���ڷ� ST_BANK�� �������ָ� ������ ����� �˼� �ְ�, �� ������ ���� ȸ�� ����� ����!!
		int			GetItemMaxNum( SORT_TYPE sortType ) const { return ( sortType >= SORT_TYPE(0) && sortType < ST_END ) ? m_MaxSize[sortType] : 0; }
		int			GetUsedSlotNum( SORT_TYPE sortType ) const;

		bool		RemoveItem( UidType itemUID );	//	������ ����� �� ����
		bool		RemoveItem( SORT_TYPE sortType, int slotID );
		
		CX2Unit*	GetOwnerUnit() const { return m_pOwnerUnit; }
        SORT_TYPE	GetSortTypeByItemTemplet( const CX2ItemTemplet_Base* pItemTemplet ) const ;
		SORT_TYPE	GetSortTypeByID( int itemID ) const;
		
		bool		IsPossibleAddItem( SORT_TYPE sortType ) const;

		wstring		GetInvenSortTypeName( SORT_TYPE sortType ) const;
		CX2Item*	GetEquippingItemByEquipPos( CX2Unit_PreHeader::EQIP_POSITION equipPos, bool bFashion ) const;

#ifdef MODIFY_INFORMER_INVEN
		void		GetItemIDAndNum( OUT set<int>& setItemPackage ) const;	//������ �ִ� itemID, ����
#else
		void		GetItemIDAndNum( map<int, int>& mapItemPackage ) const;	//������ �ִ� itemID, ����
#endif //MODIFY_INFORMER_INVEN

		int			GetEqiuppingSetItemNum( int setID ) const;
#ifdef ITEM_EXPIRE_USING_ITEM_UID
		bool		CheckEquippingItem( int itemID, UidType itemUID = -1 ) const;
#else ITEM_EXPIRE_USING_ITEM_UID
		bool		CheckEquippingItem( int itemID ) const;
#endif ITEM_EXPIRE_USING_ITEM_UID

		CX2Stat::Stat GetSetItemOptionStat() const;

		//{{ kimhc // 2009-10-19 // �ִ� MP ���� �� ������
		float		GetAddMaxMPValue() const;
		//}} kimhc // 2009-10-19 // �ִ� MP ���� �� ������

		//{{ kimhc // 2009-08-07 // ĳ���ͺ� ����
#ifdef	PRIVATE_BANK	
		int			GetCoutForGetMyBankInfoReq() const { return m_iCoutForGetMyBankInfoReq; }
		void		SetCountForGetMyBankInfoReq( int value ) { m_iCoutForGetMyBankInfoReq = value; }

		CX2Inventory::MEMBERSHIP_PRIVATE_BANK GetBankMembershipGrade() const { return m_eBankMembershipGrade; }

#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-07 // ĳ���ͺ� ����

#ifdef	SERV_SHARING_BANK_TEST
		void SetShareBank( std::map< UidType, KInventoryItemInfo >& mapItem );
		int	GetUsedShareBankNum() const { return m_ItemMap[ST_SHARE_BANK].size(); }

		void ClearShareBank();
		bool UpdateItemUID( UidType iBeforeUID, UidType iNewUID );
#endif	SERV_SHARING_BANK_TEST

#ifdef NEW_ITEM_NOTICE
		bool		IsNewItem( SORT_TYPE sortType, UidType iItemUID) const;
		void		ResetNewItem();
		bool		EraseNewItem( UidType iItemUID );
#endif //NEW_ITEM_NOTICE

#ifdef PET_DROP_ITEM_PICKUP
		UidType GetItemUIDBySortTypeAndItemID( SORT_TYPE sortType_, int ItemID_ ) const;
#endif //PET_DROP_ITEM_PICKUP

#ifdef ADDED_RELATIONSHIP_SYSTEM			
		bool		GetRelationItemInfoFromServer () const;
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef GOOD_ELSWORD //JHKang
		bool	IsAbleToExpandSlot() const;
#endif //GOOD_ELSWORD

	private:
		const InvenSlotData* GetInvenSlot( SORT_TYPE sortType, int slotID ) const;
	
private:
		CX2Unit*						m_pOwnerUnit;
		int								m_MaxSize[ST_END];
		map<UidType,InvenSlotData*>		m_ItemMapTotal;
		map<int, InvenSlotData*>		m_ItemMap[ST_END]; //���� ���̵�, ������
		
		//{{ kimhc // 2009-08-07 // ĳ���ͺ� ����
#ifdef	PRIVATE_BANK	
		int								m_iCoutForGetMyBankInfoReq;		// ���������� �޾ƿ� Ƚ��
		CX2Inventory::MEMBERSHIP_PRIVATE_BANK	m_eBankMembershipGrade;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-07 // ĳ���ͺ� ����

#ifdef NEW_ITEM_NOTICE
		//�κ��丮�� �������� ������ ������ ����, ���� �� �߰��ϱ� ������, �߰� �����δ� '���Ӱ� ���� ������'�� ���� �Ǻ��� �ȵ�.
		//�߰��� ������ ��, ������ ������ ��Ͽ� ���� �������̶�� '���Ӱ� ���� ������'�̶�� �Ǵ�.
		set<UidType>					m_SetRemoveItem;
		set<UidType>					m_SetNewItem;
#endif //NEW_ITEM_NOTICE
};
