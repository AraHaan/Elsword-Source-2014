#ifdef NEW_VILLAGE_UI

#pragma once

//class CX2StateMenu;
class CX2UIPersonalShop : public CX2UIDragable
{
public:
	enum UI_PERSONAL_SHOP_IS_FOR_BUY
	{
		OPENFORSELL = -1,
		OPENFROMBOARD = -2, // oasis907 : ����� [2010.3.25] // 
#ifdef SERV_PSHOP_AGENCY
		OPENFORAGENCYTRADER = -3,
		OPENMYSHOPFROMBOARD = -4,
		OPENTAKESHOPAGNECY	= -5,
#endif
	};

#ifdef DEF_TRADE_BOARD
//{{ oasis907 : ����� [2010.3.25] // 
	enum STAGE_KEY
	{
		SK_SQUARE = 0,
		SK_BOARD,
	};
//}}
#endif DEF_TRADE_BOARD

	enum UI_PERSONAL_SHOP_CUSTOM_MSG
	{
		UPSCM_SELL_EXIT		= 16000,
		UPSCM_SELL_EXIT_CONFIRM,
		UPSCM_SELL_HELPTOGGLE,
		UPSCM_SELL_MODIFY,
		UPSCM_SELL_PREV_PAGE,
		UPSCM_SELL_NEXT_PAGE,
		UPSCM_SELL_BEGIN,
		UPSCM_SELL_BEGIN_CONFIRM,

		UPSCM_BUY_EXIT,
		UPSCM_BUY_BUYBUTTON,
		UPSCM_BUY_PREV_PAGE,
		UPSCM_BUY_NEXT_PAGE,
		UPSCM_BUY_CONFIRM,

        UPSCM_ENROLL_LEFTARROW,
		UPSCM_ENROLL_RIGHTARROW,
		UPSCM_ENROLL_REGISTER,
		UPSCM_ENROLL_CANCLE,
		UPSCM_ENROLL_DLG_CHANGE,

		UPSCM_BUY_QUANTITY_CONFIRM,
		UPSCM_BUY_QUANTITY_CANCLE,
		UPSCM_BUY_QUANTITY_PLUS,
		UPSCM_BUY_QUANTITY_MINUS,
		UPSCM_BUY_QUANTITY_UPDATE,

		//{{ kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�
		UPSCM_BUY_SEALED_ITEM_CONFIRM,
		UPSCM_BUY_SEALED_ITEM_CANCEL,
		//}} kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�

		UPSCM_USE_MEGAPHONE1,
		UPSCM_USE_MEGAPHONE2,

#ifdef SERV_PSHOP_AGENCY
		UPSCM_DIALOG_EXIT,
		UPSCM_AGENCY_SELL_BEGIN,
		UPSCM_AGENCY_TAKE_ITEM,
#endif

//#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		UPSCM_BUY_CANCLE,
//#endif //SERV_UPGRADE_TRADE_SYSTEM
	};

	enum X2_PERSONAL_SHOP_STATE
	{
		XPSS_CLOSE = 0,	/// ���� ����
		XPSS_WAIT,		/// ���� �� �Ǹ� �غ���
		XPSS_SELL,		/// �Ǹ���
		XPSS_BUY,		/// �����

#ifdef SERV_PSHOP_AGENCY
		XPSS_AGENCY_WAIT,
		XPSS_AGENCY_SELL,
		XPSS_AGENCY_TAKE,
#endif
	};


	struct SlotInfo
	{
		UidType			m_ItemUID;
		int				m_ItemID;
		int				m_Quantity;
		int				m_PricePerOne;
		int				m_ItemFee;	//������

#ifdef SERV_PSHOP_AGENCY
		int					m_iTotalSellEDIn;
		int					m_iTotalSoldItemQuantity;
#endif
		
		SlotInfo()
		{
			Init();
		}

		SlotInfo& operator=( const SlotInfo& data )
		{
			if ( this == &data ) 
			{ 
				return *this; 
			}

			m_ItemUID		= data.m_ItemUID;
			m_ItemID		= data.m_ItemID;
			m_Quantity		= data.m_Quantity;
			m_PricePerOne	= data.m_PricePerOne;
			m_ItemFee		= data.m_ItemFee;

#ifdef SERV_PSHOP_AGENCY
			m_iTotalSellEDIn = data.m_iTotalSellEDIn;
			m_iTotalSoldItemQuantity = data.m_iTotalSoldItemQuantity;
#endif
			return *this;
		}

		void Init()
		{
			m_ItemUID		= 0;
			m_ItemID		= 0;
			m_Quantity		= 0;
			m_PricePerOne	= 0;
			m_ItemFee		= 0;	//������

#ifdef SERV_PSHOP_AGENCY
			m_iTotalSellEDIn = 0;
			m_iTotalSoldItemQuantity = 0;
#endif
		}

		void SetData( KSellPersonalShopItemInfo& ItemInfo )
		{
			m_ItemUID		= ItemInfo.m_kInventoryItemInfo.m_iItemUID;
			m_ItemID		= ItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
			m_Quantity		= ItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
			m_PricePerOne	= ItemInfo.m_iPricePerOne;
			m_ItemFee		= ItemInfo.m_iFeePerOne;	//������

#ifdef SERV_PSHOP_AGENCY			
			m_iTotalSellEDIn = ItemInfo.m_iTotalSellEDIn;
			m_iTotalSoldItemQuantity = ItemInfo.m_iTotalSoldItemQuantity;
#endif
		}


	};


public:

	CX2UIPersonalShop( CKTDXStage* pNowStage, const WCHAR* pFileName );
	~CX2UIPersonalShop(void);

 	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
 	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void BreakShop();

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

	void SetPosition(D3DXVECTOR2 vec);
	void SetLayer(X2_DIALOG_LAYER layer);
	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );

	bool Handler_EGS_CREATE_PERSONAL_SHOP_REQ();
	bool Handler_EGS_JOIN_PERSONAL_SHOP_REQ( UidType unitUID );

	// oasis907 : ����� [2010.3.29] // 
	void UpdateMegaphoneButton();

	
#ifdef DEF_TRADE_BOARD
//{{ oasis907 : ����� [2010.3.25] // 
	bool Handler_EGS_JOIN_PERSONAL_SHOP_FROM_BOARD_REQ();
//}}
#endif DEF_TRADE_BOARD
	CX2UIPersonalShop::X2_PERSONAL_SHOP_STATE GetPersonalShopState() const { return m_NowShopState; }
	wstring GetSlotItemDesc();
	wstring GetPersonalShopSlotItemDesc( UidType itemUID );

	void EnrollItemByUid( UidType itemUid, int SlotIndex = -1 );

	bool CheckEdLimit();		// ���� �ʹ� ���Ƽ� ���λ����� ������ �� ���ٸ� false�� return�Ѵ�

#ifdef SERV_PSHOP_AGENCY	
	bool CreateAgencyShop();
	bool RegisterAgencyShopItem();
	bool JoinMyAgencyShop();
	bool TakeMyAgencyShop();
	bool CloseAgencyShop();
	
	bool Handler_EGS_CREATE_PSHOP_AGENCY_REQ();
	bool Handler_EGS_CREATE_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REG_PSHOP_AGENCY_ITEM_REQ();
	bool Handler_EGS_REG_PSHOP_AGENCY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ(bool bOnlyView = true);
	bool Handler_EGS_JOIN_MY_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_STOP_SALE_PSHOP_AGENCY_REQ();
	bool Handler_EGS_STOP_SALE_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_PSHOP_AGENCY_REQ();
	bool Handler_EGS_LEAVE_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ(int iSlotInx);
	bool Handler_EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#endif

protected:
	bool Handler_EGS_CREATE_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_REG_PERSONAL_SHOP_ITEM_REQ();
	bool Handler_EGS_REG_PERSONAL_SHOP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_JOIN_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
public:
	bool Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// �ܺ� ������ ����, Public���� ����
	bool CheckDoubleShopItem( UidType itemUID, int SlotIdToSetting ); //�ߺ�üũ

	/// �ŷ� ������ ����Ʈ���� ������ ������
	UidType  GetSelectedItemID() const { return m_uidSelectedItemUID; }
	void	 SetSelectedItemID( UidType val_ ) { m_uidSelectedItemUID = val_; }

	/// �������� ���� ������ ����� ������ �ε���
	int		GetPickedShopItemIndex() const { return m_PickedShopItemIndex; }
	void	SetPickedShopItemIndex(int val_) { m_PickedShopItemIndex = val_; }

	const int GetMaxItemPageIndex();

	const bool CheckCanPaymentRegistTex();
#endif //SERV_UPGRADE_TRADE_SYSTEM

protected:
	bool Handler_EGS_LEAVE_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BUY_PERSONAL_SHOP_ITEM_REQ( UidType itemUID, int itemNum = 1);
	bool Handler_EGS_BUY_PERSONAL_SHOP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

 	bool Handler_BREAK_PERSONAL_SHOP_REQ();
 	bool Handler_BREAK_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
 	bool Handler_BREAK_PERSONAL_SHOP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetDialog( CX2UIPersonalShop::X2_PERSONAL_SHOP_STATE personalShopState );

	void CreateShop();		//���� ����
 	void RegiserShop();		// Register And Sell ����
 	void JoinShop();		//�����ڰ� ���� ����
 	void CloseShop();		//�Ǹ��ڿ��� �Ǹ� ����, �����ڿ��� ���� ����

	void ResetSlotList();
	void ResetSlotList( vector<KSellPersonalShopItemInfo>& vecSellItemInfo );
	void ResetSlot( UidType itemUID );
	void ResetSellSlotUIList( int iPageIndex = 1 );
	void ResetBuySlotUIList( int iPageIndex = 1 );

	wstring GetPersonalShopSlotItemFullName( UidType itemUID );

	CX2UIPersonalShop::SlotInfo* GetShopSlot( int slotIndex );
	CX2UIPersonalShop::SlotInfo* GetShopSlot( UidType itemUID );

	void RegisterShopItem();

#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// �ܺ� ������ ����, Public���� �̵�
	bool CheckDoubleShopItem( UidType itemUID, int SlotIdToSetting ); //�ߺ�üũ
#endif //SERV_UPGRADE_TRADE_SYSTEM

	bool CheckShopItemListToReg(); //����� ������ ����Ʈ�� ���������� Ȯ�� ( ��, ����ҷ��� �������� �Ѱ� �̻��� �־�߰���? )

	void OpenDLGItemEnroll( UidType itemUID );
	void CloseDLGItemEnroll();
	void ResetDLGItemEnroll();
	void UpdateDLGItemEnroll();

	void OpenBuyQuantityDLG( D3DXVECTOR2 pos );
	void UpdateBuyQuantityDLG( bool bReadIME = true );
	
	//{{ kimhc // 2009-09-07 // ������ �������� ���ε� ���������� Ȯ��
#ifdef	SEAL_ITEM
	bool CheckIsSealedItem( UidType itemUID );
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-07 // ������ �������� ���ε� ���������� Ȯ��


private:
	bool						m_bShow;
	X2_PERSONAL_SHOP_STATE		m_NowShopState;
	D3DXVECTOR2					m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

#ifdef DEF_TRADE_BOARD
	bool						m_bOpenFromBoard;
#endif DEF_TRADE_BOARD

	CKTDGUIDialogType							m_pDLGUIPersonalShop;
	CKTDGUIDialogType							m_pDLGHelp;
	CKTDGUIDialogType							m_pDLGItemEnroll;
	CKTDGUIDialogType							m_pDLGCloseShopCheck;
	CKTDGUIDialogType							m_pDLGBuyConfirm;
	CKTDGUIDialogType							m_pDLGBuyQuantity;

	//{{ kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�
#ifdef	SEAL_ITEM
	CKTDGUIDialogType							m_pDLGBuySealedItemConfirm;
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�

	vector< CX2UIPersonalShop::SlotInfo* >	m_vecSlotInfo;
	// vector< CX2UIPersonalShop::SlotInfoUI* >	m_vecSlotInfoUI;

	//CKTDGUIDialogType				m_pDLGDecideItemED;

	wstring						m_ShopName;
	wstring						m_ShopKeeperName;

	//3�� ������ ���° ���Կ��ٰ� �������� �巡���ؼ� �÷����°�? 0~2��
	int							m_PickedShopItemIndex;				
	
	int							m_RegShopItemSellEDPerOne;
	int							m_RegShopItemNum;
	UidType						m_RegShopItemUID;
	int							m_RegShopItemFee;
	int							m_RegShopItemID;

//	CKTDGUIDialogType				m_pDLGCloseShopCheck;

	vector<KSellPersonalShopItemInfo> m_vecPersonalShopSlotItem;

// 	CKTDGUIDialogType				m_pDLG_EGS_REG_PERSONAL_SHOP_ITEM_REQ;
// 	CKTDGUIDialogType				m_pDLG_EGS_BUY_PERSONAL_SHOP_ITEM_REQ;
	UidType						m_ItemUIDToBuy;
	int							m_ItemNumToBuy;

	// �����̾� ���� ���� �߰� �ڵ� 
	int							m_iNowPageIndex;		// [1,3]
	int							m_iMaxPageIndex;
	bool						m_bIsMyShopPremium;

	X2_DIALOG_LAYER				m_Layer;

#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	bool						m_bIsPShopAgency;		// ���� ������ �븮�������� �Ϲݻ������� ����
#endif

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	UidType						m_uidSelectedItemUID;		/// ������ ���� ������ ������ ����ũ���̵�
#endif //SERV_UPGRADE_TRADE_SYSTEM
};

#endif
