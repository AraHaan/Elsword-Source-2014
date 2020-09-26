#ifdef NEW_VILLAGE_UI

#pragma once

//class CX2StateMenu;

class CX2UIShop : public CX2ItemSlotManager
{
public:

	struct BuySlot
	{
		CKTDGUIButton*			m_pBuyButton;
		CKTDGUIStatic*			m_pEDStatic;
		CKTDGUIStatic*			m_pEDVPStatic;
        const CX2Item::ItemTemplet*    m_pItemTemplet;

		BuySlot()
		{
			m_pBuyButton = NULL;
			m_pEDStatic = NULL;
			m_pEDVPStatic = NULL;
			m_pItemTemplet = NULL;
		}
		~BuySlot()
		{
			m_pItemTemplet = NULL;
		}
	};

	enum UI_SHOP_CUSTOM_MSG
	{
		USCM_EXIT				= 20000,

		USCM_CATEGORY_WEAPON,
		USCM_CATEGORY_COAT,
		USCM_CATEGORY_PANTS,
		USCM_CATEGORY_GLOVE,
		USCM_CATEGORY_SHOES,
		USCM_CATEGORY_SPECIAL,
		USCM_CATEGORY_ACCESSORY,
		USCM_CATEGORY_QUICKSLOT,
        
		USCM_SHOW_OTHER_CHARACTER_EQUIP_ON,
		USCM_SHOW_OTHER_CHARACTER_EQUIP_OFF,

		USCM_PREV_PAGE,
		USCM_NEXT_PAGE,

		//////////////////////////////////////////////////////////////////////////

		USCM_BUY,

		USCM_BUY_ITEM_CONFIRM_OK,
		USCM_BUY_ITEM_CONFIRM_CANCLE,

		USCM_BUY_ITEM_QUANTITY_CONFIRM,
		USCM_BUY_ITEM_QUANTITY_CANCLE,
		USCM_BUY_ITEM_QUANTITY_PLUS,
		USCM_BUY_ITEM_QUANTITY_MINUS,
		USCM_BUY_ITEM_QUANTITY_UPDATE,

		//////////////////////////////////////////////////////////////////////////

		USCM_SELL_ITEM_CONFIRM_OK,
		USCM_SELL_ITEM_CONFIRM_CANCLE,

		USCM_SELL_ITEM_QUANTITY_CONFIRM,
		USCM_SELL_ITEM_QUANTITY_CANCLE,
		USCM_SELL_ITEM_QUANTITY_PLUS,
		USCM_SELL_ITEM_QUANTITY_MINUS,
		USCM_SELL_ITEM_QUANTITY_UPDATE,

		//////////////////////////////////////////////////////////////////////////

		USCM_REPAIR_ITEM_CONFIRM_OK,

		USCM_REPAIR_ITEM_ALL,
		USCM_REPAIR_ITEM_ALL_CONFIRM_OK,
		USCM_REPAIR_ITEM_ALL_CONFIRM_CANCLE,

		//////////////////////////////////////////////////////////////////////////

		USCM_ENCHANT_ITEM_EXIT,
		USCM_ENCHANT_ITEM_OLD_OK,
		USCM_ENCHANT_ITEM_NEW_OK,

		USCM_ENCHANT_ITEM_RESULT_OK,

		USCM_ENCHANT_ADJUVANT_CHECKBOX,
		
		//////////////////////////////////////////////////////////////////////////

		USCM_ENCHANT_ATTRIBUTE_ITEM_EXIT,
		USCM_ENCHANT_ATTRIBUTE_ITEM_SELECT,
		USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE,
		USCM_ENCHANT_ATTRIBUTE_ITEM_CONFIRM,
		USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE_CONFIRM,
		//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰� 
//#ifdef SERV_DESTROY_GUARD_ITEM
		USCM_ENCHANT_PLUS_CHECKBOX,
//#endif // SERV_DESTROY_GUARD_ITEM
		//}}
		//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
//#ifdef SERV_DESTROY_GUARD_ITEM
		USCM_ENCHANT_DESTROY_GUARD_CHECKBOX,
//#endif // SERV_DESTROY_GUARD_ITEM
		//}}
//#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		USCM_EVENT_EXCHANGE_ITEM,
//#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	};


	enum BUY_ITEM_CATEGORY
	{
		BIC_WEAPON,
		BIC_COAT,
		BIC_TROUSERS,
		BIC_HAND,
		BIC_FOOTWEAR,
		BIC_SPECIAL,
		BIC_ACCESSORY,
		BIC_QUICK_SLOT,
		BIC_COUNT,
	};



public:

	CX2UIShop( CKTDXStage* pNowStage, const WCHAR* pFileName );
	~CX2UIShop();
	
	// �⺻������ �� ��� �ϴ� ��
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//���� ����
	virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );
	// 
	// 	void DrawSlotMouseOverImage();
	// 	wstring GetSlotItemDesc();


	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow( bool val );
	
	void SetLayer(X2_DIALOG_LAYER layer);
	void SetPosition(D3DXVECTOR2 vec);

	void SetHouseID(int HouseID){ m_HouseID = HouseID; }

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

	// ��������
	void SellItem( UidType sellItemUID, D3DXVECTOR2 pos = D3DXVECTOR2(300, 300) );
	void RepairItem( UidType RepairItemUID );
	void RepairAllEquippedItem();
	void EnchantItem( UidType EnchantItemUID, D3DXVECTOR2 pos = D3DXVECTOR2(300, 300) );
	void EnchantAttributeItem( UidType AttributeEnchantItemUID, D3DXVECTOR2 pos = D3DXVECTOR2(300, 300) );
	
#ifdef AUTH_AUTO_SELL_ITEM
	void AuthAutoSellItem( CX2Inventory::SORT_TYPE SortType );
#endif

protected:

	// �Ǹ�
	void OpenItemSellNumDLG( D3DXVECTOR2 pos );
	void UpdateItemSellNumDLG( bool bReadIME, bool bTextSelected = false );
	void OpenSellItemConfirmDLG();
	bool Handler_EGS_SELL_ITEM_REQ();
	bool Handler_EGS_SELL_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ����
	bool Handler_EGS_REPAIR_ITEM_REQ( UidType ItemUID );
	bool Handler_EGS_REPAIR_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void AllEquippingItemRepairREQ();


	// ����
	void ResetBuySlotList( int nPage );
	void BuyUISetting();
	void OpenItemBuyNumDLG( D3DXVECTOR2 pos );
	void UpdateItemBuyNumDLG( bool bReadIME, bool bTextSelected = false );
	void OpenBuyItemConfirmDLG();
	virtual bool Handler_EGS_BUY_ED_ITEM_REQ();
	virtual bool Handler_EGS_BUY_ED_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	int PreCheckShopBuyItem();

	//��ȭ
	void OpenEnchantDLG( D3DXVECTOR2 pos);
	void UpdateEnchantWindow();
	int GetEnchantStoneLevel( int iItemLevel );
	bool Handler_EGS_ENCHANT_ITEM_REQ( bool bSpecialEnchant );
	bool Handler_EGS_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OpenEnchantResultWindow( KEGS_ENCHANT_ITEM_ACK& kEGS_ENCHANT_ITEM_ACK );
	

#ifdef FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING	
	void SplitEnchantDLGStringSet ( CKTDGUIStatic*  pStatic, wstring strItemName );
#endif // FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING	

	//��ȭ����
	void GetRequiredEnchantAdjuvantItemID( IN const int iItemLevel_, OUT vector<int>& vecAdjuvantIdList_ );
	int GetNumOfRequiredEnchantAdjuvantItem( IN const vector<int>& vecAdjuvantIdList_ );
	//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
	void GetRequiredEnchantPlusItemID( const int iItemLevel, OUT vector<int>& vecPlusIdList_ );
	int GetNumOfRequiredEnchantPlusItem( IN const vector<int>& vecPlusIdList_ );
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰� 
#ifdef SERV_DESTROY_GUARD_ITEM
	void GetRequiredDestroyGuardItemID( const int iItemLevel, OUT vector<int>& vecGuardIdList_ );
	int GetNumOfRequiredDestroyGuardItem( IN const vector<int>& vecGuardIdList_ );
#endif // SERV_DESTROY_GUARD_ITEM
	//}}
	
#ifdef SERV_CATCH_HACKUSER_INFO
	bool Handler_EGS_CATCH_HACKUSER_INFO_NOT( KEGS_CATCH_HACKUSER_INFO_NOT& kEGS_CATCH_HACKUSER_INFO_NOT );
#endif SERV_CATCH_HACKUSER_INFO	

	//�Ӽ� ��ȭ ����
	void ResetAtrribEnchantWindow();
	bool Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( int slotNum, int enchantID, bool bDebug );
	bool Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ���� ��ƿ��Ƽ �Լ�
	CKTDGUIRadioButton* GetRadioButtonNameByCategory( BUY_ITEM_CATEGORY buyItemCategory );
    bool IsPossibleUsedByMyCharacter( const CX2Item::ItemTemplet* pItemTemplet );

	//{{ kimhc // 2009-09-16 // ��� â�� �㰡�� or ��� Ȯ����� ������ �� �� �ִ°�?
#ifdef	GUILD_MANAGEMENT
	bool CanBuyItemToCreateGuild() const;
	bool CanBuyItemToExpandGuild() const;
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-09-16 // ��� â�� �㰡�� or ��� Ȯ����� ������ �� �� �ִ°�?
	
	//{{ JHKang / ������ / 2010/10/12
#ifdef MODIFY_SHOP_BUY_ITEM
	CX2Inventory::SORT_TYPE GetCurrInventoryType( CX2Item::ITEM_TYPE type );
#endif MODIFY_SHOP_BUY_ITEM
	//}} JHKang / ������ / 2010/10/12

#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	bool Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ( );
	bool Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	bool Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
private:

	// ����
	bool					m_bShow;
	D3DXVECTOR2				m_MovedPosition;
	int						m_HouseID;			// X2LocationManager::HOUSE_ID
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	// ���̾�α�
	CKTDGUIDialogType			m_pDLGShop;

	CKTDGUIDialogType			m_pDLGSellItemNum;
	CKTDGUIDialogType			m_pDLGSellItemConfirm;

	CKTDGUIDialogType			m_pDLGRepairItemConfirm;
	
	CKTDGUIDialogType			m_pDLGBuyItemNum;
	CKTDGUIDialogType			m_pDLGBuyItemConfirm;

	CKTDGUIDialogType			m_pDLGEnchantItem;
	CKTDGUIDialogType			m_pDLGEnchantItemResult;

	CKTDGUIDialogType			m_pDLGAttribEnchantItem;
	CKTDGUIDialogType			m_pDLGAttribEnchantItemConfirm;


	// - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���� ������ ����

	// ( ȭ�鿡 8���� ���̴� ) ������ ����Ʈ
	// ȭ�鿡 ���̴� ������ ������ LUA�� �ִ� ���� ���� �Ǵ��Ѵ� -> m_SlotList.size()
	vector<BuySlot*>		m_vecBuySlot;
	int						m_nNowPage;
	int						m_nMaxPage;
	bool					m_bShowAllCharItem;

	// ����
	int						m_BuyItemNum;
	int						m_BuyItemSlotNum;
	
	// �� �� �ִ� ������ ��ü ����Ʈ, �������� 
    vector<const CX2Item::ItemTemplet*> m_vecItemList[BIC_COUNT];
	BUY_ITEM_CATEGORY		m_NowBuyItemCategory;

	// �Ǹ�
	UidType					m_SellItemUID;
	int						m_SellItemNum;
	bool					m_bQuickSell;

	// ����
	UidType					m_RepairItemUID;
	vector< UidType >		m_vecAllEquippingItem;

	// ��ȭ
	UidType					m_EnchantItemUID;
	bool					m_bUseEnchantAdjuvant;
	int						m_EnchantLevelBefore;
	//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
	bool					m_bUseEnchantPlus;
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
		//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
	bool					m_bUseDestroyGuard;
#endif // SERV_DESTROY_GUARD_ITEM
		//}}
	
	// �Ӽ�
	UidType					m_AttributeEnchantItemUID;
	int						m_AttribEnchantID;
	int						m_AttribEnchantSlotID;

	// ���콺 �� ����
	SHORT					m_SumDelta;

#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT	
	int						m_iAddEnchantLevel;	/// MAGIC_ENCHANT_LEVEL_LIMIT �⺻ ��ȭ ���� ������ �߰� ��(�̺�Ʈ ��)
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
};

#endif
