#pragma once

#ifdef PET_PREVIEW
class CX2NPCUnitViewerUI;
#endif //PET_PREVIEW

#ifdef COUPON_SYSTEM
class CX2UICouponBox;
#endif // COUPON_SYSTEM


class CX2CashShop : public CX2ItemSlotManager
{
	public:

		enum CASH_SHOP_CUSTOM_UI_MSG
		{
			CSCUM_CASH_CHARGE = 1800,
			CSCUM_BUY,
			CSCUM_PRESENT,
			CSCUM_STORE,	//JJIM
			
			CSCUM_MAIN_NEW_PREV_PAGE,
			CSCUM_MAIN_NEW_NEXT_PAGE,
			CSCUM_MAIN_HOT_PREV_PAGE,
			CSCUM_MAIN_HOT_NEXT_PAGE,
			CSCUM_MAIN_RECOMMEND_PREV_PAGE,
			CSCUM_MAIN_RECOMMEND_NEXT_PAGE,

			CSCUM_BUY_POP_UP_PREV_PAGE,			// �����ϱ� UI, ��ǰ ������ 1 ����
			CSCUM_BUY_POP_UP_NEXT_PAGE,			// �����ϱ� UI, ��ǰ ������ 1 ����
			CSCUM_BUY_POP_UP_ALL_CHECK_CHANGED,
			CSCUM_BUY_POP_UP_EACH_CHECK_CHANGED,
			CSCUM_BUY_POP_UP_PRICE_CHANGED,		//����Ʈ �ڽ����� ���� ����
			CSCUM_BUY_POP_UP_OK,
			CSCUM_BUY_POP_UP_CANCEL,

			CSCUM_CASH_ITEM_DEPOSIT_PREV_PAGE,
			CSCUM_CASH_ITEM_DEPOSIT_NEXT_PAGE,

			CSCUM_CATEGORY_CHANGE,
			CSCUM_SUB_CATEGORY_CHANGE,

			CSCUM_SUB_PREV_PAGE,
			CSCUM_SUB_NEXT_PAGE,
			CSCUM_SUB_SEE_ALL_ITEM_OPTION_CHANGE,

			CSCUM_TAKE_OFF_ALL_ITEM,

			CSCUM_ITEM_TO_INVEN_POP_UP_PREV_PAGE,
			CSCUM_ITEM_TO_INVEN_POP_UP_NEXT_PAGE,
			CSCUM_ITEM_TO_INVEN_POP_UP_OPTION_CHANGED,
			CSCUM_ITEM_TO_INVEN_POP_UP_OK,
			CSCUM_ITEM_TO_INVEN_POP_UP_CANCEL,

			CSCUM_ITEM_TO_INVEN_CHECK_OK,
			CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK,

			CSCUM_BUY_ALL_EQUIPPING_ITEMS,

			CSCUM_CHOICED_BUY,
			CSCUM_CHOICED_PRESENT,
			CSCUM_CHOICED_REMOVE_IN_LIST,
			CSCUM_CHOICED_PRE_EQUIPPING,
			CSCUM_CHOICED_CLOSE,
			CSCUM_CHOICED_BUY_ALL,

			CSCUM_PRESENT_POP_UP_CHANGED_CASH_PRODUCT,
			CSCUM_PRESENT_POP_UP_CAHNGED_FRIEND,
			CSCUM_PRESENT_OK,
			CSCUM_PRESENT_CANCEL,

			CSCUM_BUY_POP_UP_OVERLAP_OK,
			CSCUM_BUY_POP_UP_OVERLAP_CANCEL,

			CSCUM_ITEM_TO_INVEN_POP_UP_ALL_CHECK_CHANGED,
			CSCUM_ITEM_TO_INVEN_POP_UP_EACH_CHECK_CHANGED,
			CSCUM_ITEM_TO_INVEN_OVERLAP_CHECK_OK_PACKAGE,

			CSCUM_BUY_PLATINUM_CARD_OK,
			CSCUM_BUY_PLATINUM_CARD_CANCEL,
//#ifdef CASH_ITEM_REFUND
			CSCUM_CASH_ITEM_REFUND,
			CSCUM_CASH_ITEM_REFUND_OK,
//#endif CASH_ITEM_REFUND

#ifdef CASH_INVEN_PICKUP_ALL
			CSCUM_PICKUP_ALL							= 1856,
#endif //CASH_INVEN_PICKUP_ALL

#ifdef CHINA_CASH_CHANGE //2011.02.22 lygan_������ // �߱� �������� UI ����
			CSCUM_CASH_CHARGE_CN						= 1880,
			CSCUM_CASH_CHARGE_TAP_ELSWORD_CN			= 1881,
			CSCUM_CASH_CHARGE_TAP_GIANT_CN				= 1882,
			CSCUM_CASH_CHARGE_TAP_EXIT_CN				= 1883,
			CSCUM_CASH_ELSWORD_RADIO_UP_CN				= 1884,
			CSCUM_CASH_ELSWORD_RADIO_DOWN_CN			= 1885,
			CSCUM_CASH_ELSWORD_OK_CN					= 1886,
			CSCUM_CASH_ELSWORD_CANCEL_CN				= 1887,
			CSCUM_CASH_ELSWORD_CHANGE_CN				= 1888,
			CSCUM_CASH_GIANT_FILLING_CN					= 1889,
			CSCUM_CASH_GIANT_TAP_EXIT_CN				= 1890,
#endif CHINA_CASH_CHANGE

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
			CSCUM_USE_BONUS_SELECT						= 1891,
			CSCUM_USE_GAMEPOINT_SELECT					= 1892,
			CSCUM_PRESENT_USE_BONUS_SELECT				= 1893,
			CSCUM_PRESENT_USE_GAMEPOINT_SELECT			= 1894,
			CSCUM_USE_GASH_SELECT						= 1895,
			CSCUM_PRESENT_USE_GASH_SELECT				= 1896,
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
			CSCUM_CASH_CHARGE_TAP_DIRECT_CHARGE_CN		= 1897,
			CSCUM_CASH_DIRECT_CHARGE_CN					= 1898,
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

//#ifdef SERV_NEXON_AUTH_SOAP
			CSUCM_CHECK_AUTHORITY_OK,
//#endif //SERV_NEXON_AUTH_SOAP
//#ifdef SERV_EVENT_DAILY_GIFT_BOX
			CSUCM_CLOSE_DAILY_GIFT_BOX,
//#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}
			CSCUM_TAKE_OFF_ALL_ITEM_AND_INVEN_EQUIP_ITEM,
//#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
			CSCUM_RADIO_BUTTON_PRE_EQUIP_SET,
//#endif //SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
			CSCUM_BUY_COUPON_COMBOBOX_CHANGED, // ���� �޺��ڽ� ����

//#ifdef COUPON_SYSTEM
			CSCUM_BUY_POP_UP_TO_FIRST_PAGE,			// �����ϱ� UI, ��ǰ ������ ù �������� ����
			CSCUM_BUY_POP_UP_TO_LAST_PAGE,			// �����ϱ� UI, ��ǰ ������ ù �������� ����
//#endif // COUPON_SYSTEM

		};

		enum CASH_SHOP_STATE
		{
			CSS_MAIN = 0,
			CSS_SUB,
		};



		enum CASH_SHOP_CATEGORY
		{
			CSC_NONE	= 0,
			CSC_FASHION,
			CSC_ACCESSORY,
			CSC_CONSUME,
			CSC_INSTALL,
			CSC_PET,
			CSC_EVENT,
			//{{ kimhc // 2009-09-29 // �ڵ����� ī�װ� �߰�
			CSC_AUTO_PAYMENT,
			//}} kimhc // 2009-09-29 // �ڵ����� ī�װ� �߰�
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
			CSC_EVENT_2,
#endif // ADD_CASH_SHOP_CATEGORY_EVENT_2
			CSC_END,
		};

		enum CASH_SHOP_SUB_CATEGORY
		{
			// CSC_FASHION
			CSSC_ALL			= 0,

			CSSC_FASHION_WEAPON = 1,
			CSSC_FASHION_BODY,
			CSSC_FASHION_LEG,
			CSSC_FASHION_HAIR,
			CSSC_FASHION_HAND,
			CSSC_FASHION_FOOT,
//#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			CSSC_FASHION_ONE_PIECE,
//#endif SERV_NEW_ONE_PIECE_AVATAR_SLOT
			
			// CSC_ACCESSORY
			CSSC_ACCESSORY_FACE = 50,
			CSSC_ACCESSORY_BODY,
			CSSC_ACCESSORY_LEG,
			CSSC_ACCESSORY_ARM,
			CSSC_ACCESSORY_RING,
			CSSC_ACCESSORY_NECKLACE,
			CSSC_ACCESSORY_WEAPON,

			CSSC_CONSUME_RESTORE = 100,
			CSSC_CONSUME_PREMIUM,
			CSSC_CONSUME_INIT,
			CSSC_CONSUME_ETC,

			CSSC_INSTALL_ADD_SLOT = 150,
			CSSC_INSTALL_PERSONAL_SHOP,
			CSSC_INSTALL_ETC,

			CSSC_PET_PET = 200,
			CSSC_PET_CONSUME,
//#ifdef RIDING_SYSTEM
			CSSC_PET_RIDING,
//#endif //RIDING_SYSTEM

			CSSC_EVENT_PACKAGE = 250,
			CSSC_EVENT_FASHION,
			CSSC_EVENT_ACCESSORY,
			CSSC_EVENT_CONSUME,
			CSSC_EVENT_INSTALL,
			CSSC_EVENT_PET,

			CSSC_AUTO_PAYMENT_BASIC = 300,

			CSSC_EVENT_2_EVENT		= 400,//ADD_CASH_SHOP_CATEGORY_EVENT_2
		};

		//{{ kimhc // 2009-10-14 // �������� ����
#ifdef	AUTO_PAYMENT
		enum CASH_SHOP_WEB_TYPE
		{
			CSWT_INVALID		= 0,
			CSWT_CASH_CHARGE,		// ĳ������ ��������
			CSWT_CASH_AUTO_PAYMENT,	// �ڵ����� ��������
		};
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-14 // �������� ����

		struct CashShopCategory
		{
			D3DXVECTOR2				m_CategoryID;		//x�� ����, y�� ����
			vector< D3DXVECTOR2 >	m_vecSubCategoryID;
		};

		struct CashShopSlot
		{
			int				m_ItemID;
			CKTDGUIDialogType	m_pDLGCashShopSlot;

			CX2Slot::SLOT_TYPE m_SlotType;
			int m_SlotIndex;

			CashShopSlot( CX2Slot::SLOT_TYPE cashShopSlotType, int slotIndex );
			~CashShopSlot();

			void SetItemID( int itemID ) { m_ItemID = itemID; }
			int	GetItemID() { return m_ItemID; }

			void SetShow( bool bShow );
			//{{ kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�
//#ifdef	AUTO_PAYMENT
			void SetInfo( bool bAutoPayment = false );
//#else	AUTO_PAYMENT
//			void SetInfo();
//#endif	AUTO_PAYMENT
			//}} kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�
			wstring GetCutString( const WCHAR* wszItemName, int maxItemNameLen );

			//{{ kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�
#ifdef	AUTO_PAYMENT
			void SetShowPresentAndCartButton( bool bShow ); // ����, �� ��ư
#endif	AUTO_PAYMENT
			//}} kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�

#ifdef SERV_USE_ENABLE_GIFT
			void SetShowPresentButton( bool bShow );
#endif SERV_USE_ENABLE_GIFT

#ifdef SERV_WISH_LIST_NO_ITEM
			void SetShowCartButton( bool bShow );
#endif SERV_WISH_LIST_NO_ITEM
		};		

#ifdef COUPON_SYSTEM
		struct CouponNoAndName
		{
			wstring m_wstrCouponName;
			int m_iCouponNo;

			CouponNoAndName( int iCoupon_, const wstring& CouponName_ ):
			m_wstrCouponName(CouponName_), m_iCouponNo(iCoupon_)
			{}
		};

		// ���� ���� ����
		struct CouponDiscountInfo
		{
			bool m_bIsRateDiscount; // Percent ���� ����
			float m_fDiscountValue;

			CouponDiscountInfo( bool bIsRateDiscount_, int iDiscountValue_ ):
			m_bIsRateDiscount(bIsRateDiscount_)
			{
				// ���� : 0~100%
				if( true == m_bIsRateDiscount )
					m_fDiscountValue = static_cast<float>(iDiscountValue_) / 100.f;
				else
					m_fDiscountValue = static_cast<float>(iDiscountValue_);
			}

			CouponDiscountInfo()
			{}
		};
#endif // COUPON_SYSTEM

		class CashShopSlotManager
		{
			public:

				CashShopSlotManager( CX2Slot::SLOT_TYPE cashShopSlotType );
				~CashShopSlotManager();

				//{{ kimhc // 2009-10-12 // �ڵ����� �� ĳ�� - �� ���� ǥ��				
				void Reset( vector<int>& vecItemID, bool bAutoPayment = false, bool bPossibleItemOnly = false );
				//}} kimhc // 2009-10-12 // �ڵ����� �� ĳ�� - �� ���� ǥ��
				
				//{{ kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�
//#ifdef	AUTO_PAYMENT
				void SetOpen( bool bOpen, bool bAutoPayment = false );
//#else	AUTO_PAYMENT
//				void SetOpen( bool bOpen );
//#endif	AUTO_PAYMENT
				//}} kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�

				int	GetNowPage() { return m_NowPage; }
				int GetMaxPage() { return m_MaxPage; }

				//{{ kimhc // 2009-10-12 // �ڵ����� �� ĳ�� - �� ���� ǥ��
				bool NextPage( bool bAutoPayment = false );
				bool PrevPage( bool bAutoPayment = false );
				//}} kimhc // 2009-10-12 // �ڵ����� �� ĳ�� - �� ���� ǥ��

				vector<int> GetNowShowItemIDList();//���� �������� ������ ���̵� ����Ʈ
				int	 GetItemID( CKTDGUIDialogType pDialog );

				//{{ kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�
#ifdef	AUTO_PAYMENT
				void SetShowPresentAndCartButton( bool bShow ); // ����, �� ��ư
#endif	AUTO_PAYMENT
				//}} kimhc // 2009-10-12 // �ڵ����� �� ����, �� ��ư �Ⱥ��̰� �ϱ� ���� �߰�

#ifdef REFORM_SKILL_NOTE_UI
				void ResetNowPage( int iVal_);
				const CX2CashShop::CashShopSlot* GetCashShopSlot( int iItemID_ );
#endif // REFORM_SKILL_NOTE_UI

#ifdef SERV_USE_ENABLE_GIFT
				void SetShowPresentButton();
				bool IsPossiblePresent( int itemID );
#endif SERV_USE_ENABLE_GIFT

#ifdef SERV_WISH_LIST_NO_ITEM
				void SetShowCartButton();
				bool IsNotPossibleWishList( int itemID );
#endif SERV_WISH_LIST_NO_ITEM
			protected:

				//{{ kimhc // 2009-10-12 // �ڵ����� �� ĳ�� - �� ���� ǥ��
//#ifdef	AUTO_PAYMENT
				void ResetNowPage( bool bAutoPayment = false );
//#else	AUTO_PAYMENT
//				void ResetNowPage();
//#endif	AUTO_PAYMENT
				//}} kimhc // 2009-10-12 // �ڵ����� �� ĳ�� - �� ���� ǥ��
				
				bool IsPossibleUsedByMyBankMembership( int itemID ) const;

				bool m_bOpen;

				int m_NowPage;
				int m_MaxPage;

				int	m_MaxSlotNumPerPage;

				vector< int	>			m_vecItemID;
				vector< CashShopSlot* > m_vecCashShopSlot;
		};

		class BuyItemPopupSlot : public CX2PageMgrItem::CX2PageSlotItem
		{
			public:

				BuyItemPopupSlot();
				BuyItemPopupSlot( int index, const WCHAR* pFileName );
				~BuyItemPopupSlot();
		
				void UpdateUI();
				void SetCheck( bool bCheck );
				void SetCash( int comboBoxIndex );
#ifdef COUPON_SYSTEM // ��ǰ ��ȣ�� ���� ��� ������ ���� ���� ����Ʈ ����
				void UpdateCouponList( const vector<CouponNoAndName>& vecCouponList_, const int iSelectedCouponNo_ = -1 ); 
#endif // COUPON_SYSTEM

				CX2PageSlot* Clone( int index, const WCHAR* pFileName ) { return new BuyItemPopupSlot( index, pFileName ); }

	
		};

		class CashItemToInvenPopupSlot : public CX2PageMgrItem::CX2PageSlotItem
		{
			public:
				CashItemToInvenPopupSlot();
				CashItemToInvenPopupSlot( int index, const WCHAR* pFileName );
				~CashItemToInvenPopupSlot();


				void UpdateUI();

#ifdef SERV_GLOBAL_BILLING
				void SetKBillOrderInfo( KBillOrderInfo& kKBillOrderInfo ) { m_KBillOrderInfo = kKBillOrderInfo; }
				void SetKBillPackageInfo( KBillPackageInfo& kKBillPackageInfo ) { m_KBillPackageInfo = kKBillPackageInfo; }
				void AddItemToComboBox_KOG( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_ );
#else // SERV_GLOBAL_BILLING
				void AddItemToComboBox( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_ );
				void SetKNXBTOrderInfo( KNXBTOrderInfo& kKNXBTOrderInfo ) { m_KNXBTOrderInfo = kKNXBTOrderInfo; }
				void SetKNXBTPackageInfo( KNXBTPackageInfo& kKNXBTPackageInfo ) { m_KNXBTPackageInfo = kKNXBTPackageInfo; }
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#ifdef SERV_GLOBAL_BILLING
				void AddSocketIdListToComboBox( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_, const int iSocketItemGroupID_ );
#else //SERV_GLOBAL_BILLING
				void AddSocketIdListToComboBox( CKTDGUIComboBox* pComboBox_, const CX2Item::ItemTemplet* pItemTemplet_, const WCHAR* wszSocketItemGroupID_ );
#endif //SERV_GLOBAL_BILLING
#endif // SERV_CASH_ITEM_SOCKET_OPTION


				void SetOption( int comboBoxIndex );

				void SetCheck( bool bCheck );

				CX2PageSlot* Clone( int index, const WCHAR* pFileName ) { return new CashItemToInvenPopupSlot( index, pFileName ); }
				
			protected:

#ifdef SERV_GLOBAL_BILLING
				KBillOrderInfo m_KBillOrderInfo;
				KBillPackageInfo m_KBillPackageInfo;
#else // SERV_GLOBAL_BILLING
				KNXBTOrderInfo m_KNXBTOrderInfo;
				KNXBTPackageInfo m_KNXBTPackageInfo;
#endif // SERV_GLOBAL_BILLING
		};

		class BuyItemPopup : public CX2PageMgrItem
		{
			public:

				BuyItemPopup( const WCHAR* pFileName) : 
				CX2PageMgrItem( pFileName )
				, m_bChoicedItemBuy( false )
				{
#if defined (SERV_COUNTRY_CN) || defined(CLIENT_COUNTRY_PH)
					m_iUseCashType = KGlobalCashInfo::GCT_KOG_ELSWORD_CASH;
#elif defined(SERV_COUNTRY_TWHK)
					m_iUseCashType = KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH;
#else // SERV_COUNTRY_XX
					m_iUseCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;// �̱�, �Ϻ� ����
#endif // SERV_COUNTRY_XX
				}

				~BuyItemPopup() {}

				
				void SetCheckAllItem( bool bCheck );
				void ChangeCheckItem( CKTDGUIDialogType pDialog, bool bCheck );

				
#ifdef COUPON_SYSTEM
				// ���� ���� ��� ����
				void UpdateCouponList();

				// '��ǰ ��ȣ' �� ���� ���� ���
				void InsertCouponNoAndDiscountInfo(const int iCouponNo_, const CouponDiscountInfo sDiscountInfo_ );

				// '��ǰ ��ȣ' �� ��� ���� �� '���� ��ȣ' ���
				void InsertCouponNoAndName( const unsigned long ulProductNo_, const int iCouponNo_, wstring& wstrCouponName_ );

				// '��ǰ ��ȣ' �� ��� ���� �� '���� ��ȣ' �ʱ�ȭ
				void InitProductAndCouponNo( unsigned long ulProductNo_ );

				// ��ǰ ��ȣ, ���� ��ȣ ���
				bool GetCheckedProductAndCouponNoList(OUT vector< std::pair<unsigned long, int> >& vecProductAndCouponNoList_ );

				// ��ǰ ��ȣ ���� �� ���� �� ��ǰ ��ȣ ��ȯ
				unsigned long ChangeItemPeriod( CKTDGUIDialogType pDialog, int comboBoxIndex );
				// ���� ����
				void ChangeItemCoupon( CKTDGUIDialogType pDialog, int comboBoxIndex );

				void InitCouponList();

				// ���� ���� ���� ���� üũ
				bool IsHavePrdouctCouponInfo( const unsigned long ulProductNo_ );
#else
				void ChangeItemPeriod( CKTDGUIDialogType pDialog, int comboBoxIndex ); // �޺��ڽ����� �� ĳ�� ����
				bool GetCheckedProductNoList(OUT vector<unsigned long>& vecProductNoList );
#endif // COUPON_SYSTEM
				void UpdateCashUI();

				void SetChoicedItemBuy( bool bCheck ) { m_bChoicedItemBuy = bCheck; }
				bool GetChoicedItemBuy() { return m_bChoicedItemBuy; }

//{{ ����� 2010.11.1
#ifdef REST_CASH_CHECK
				bool RestCashCheck();
#endif 
//}} ����� 2010.11.1

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				void SetUseCashType(int iCashType)
				{
#if defined(CLIENT_COUNTRY_CN)
					if(iCashType == static_cast<int>(KGlobalCashInfo::GCT_KOG_ELSWORD_CASH))
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_Bonus" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(false);
						CKTDGUIRadioButton* pRadioEnableGamePoint = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_GamePoint" );
						if(pRadioEnableGamePoint != NULL)
							pRadioEnableGamePoint->SetChecked(true);

						m_iUseCashType = iCashType;
					}
					else
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_Bonus" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(true);
						CKTDGUIRadioButton* pRadioEnableGamePoint = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_GamePoint" );
						if(pRadioEnableGamePoint != NULL)
							pRadioEnableGamePoint->SetChecked(false);

						m_iUseCashType = static_cast<int>(KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT);
					}
#elif defined CLIENT_COUNTRY_TW
					if(iCashType == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH))
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_Gash" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(false);
						CKTDGUIRadioButton* pRadioEnableCash = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_GamePoint" );
						if(pRadioEnableCash != NULL)
							pRadioEnableCash->SetChecked(true);

						m_iUseCashType = iCashType;
					}
					else
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_Gash" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(true);
						CKTDGUIRadioButton* pRadioEnableCash = (CKTDGUIRadioButton*)m_pDialog->GetControl( L"Use_GamePoint" );
						if(pRadioEnableCash != NULL)
							pRadioEnableCash->SetChecked(false);

						m_iUseCashType = static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_CASH);
					}
#else
					m_iUseCashType = iCashType;
#endif //CLIENT_COUNTRY_CN
				}

				int GetUseCashType(){ return m_iUseCashType; }
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef CASH_ITEM_REFUND
				int	GetCheckedItemListSize(){ return (int)m_vecCheckedItem.size(); }
				bool NonRefundableItemInclude();
				void SetStringRefundableItem();
				void SetStringNonRefundableItem();
				void SetStringNonRefundableItemInclude();
#endif CASH_ITEM_REFUND



			protected:

				void InitbyOpen();

				void ResetNowPage();

				vector<int>		m_vecCheckedItem;
				map< int, int > m_mapSelectedProductNo;		//itemID, CashData�� KNXBTProductInfo ���� �ε���
				bool			m_bChoicedItemBuy;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				int				m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef COUPON_SYSTEM 
				// ��ǰ ��ȣ �� ��� ������ ���� ����Ʈ,( ���� ����Ʈ : ���� ��ȣ, ���� �� )
				map< unsigned long, vector<CouponNoAndName> > m_mapProductCouponNo; 
				
				// ��ǰ��ȣ�� ���� �� ���� m_iCouponCardNo
				map< unsigned long, int > m_mapSelectedCouponNo;

				// ���� ��ȣ �� ���� ����
				map< int, CouponDiscountInfo > m_mapCouponDiscountInfo;

				CKTDGUIDialogType m_pDialogSelectedPage; // ���� �� ������ ������ ����
				unsigned long	  m_ulSelectedProductNo; // ���� �� ��ǰ ��ȣ
#endif // COUPON_SYSTEM

		};

		class CashItemToInvenPopup : public CX2PageMgrItem
		{
		public:

			CashItemToInvenPopup( const WCHAR* pFileName) : CX2PageMgrItem( pFileName ) {}
			~CashItemToInvenPopup() {}

#ifdef SERV_GLOBAL_BILLING
			void Reset( vector< int >& vecItemID, vector< KBillOrderInfo >& vecKBillOrderInfo,
						vector< KBillPackageInfo >& vecKBillPackageInfo );			
#else // SERV_GLOBAL_BILLING
			void Reset( vector< int >& vecItemID, vector< KNXBTOrderInfo >& vecKNXBTOrderInfo,
						vector< KNXBTPackageInfo >& vecKNXBTPackageInfo );
#endif // SERV_GLOBAL_BILLING
			void ChangeItemOption( CKTDGUIDialogType pDialog, int comboBoxIndex ); // �޺��ڽ����� �ɼ� ������
			
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
			bool GetSelectedOptionListAndGroupID( OUT map<int, int>& mapSelectedOptionList_, OUT map<int, int>& mapOptionGroupIdList_ );
#else	SERV_CASH_ITEM_SOCKET_OPTION
			bool CX2CashShop::CashItemToInvenPopup::GetSelectedOptionList( map< int, int >& mapSelectedOptionList );
#endif	SERV_CASH_ITEM_SOCKET_OPTION
			
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#ifdef SERV_GLOBAL_BILLING
			std::pair<int, int> GetSelectedOptionList( const int iSocketItemGroupID_, const int iSelectedIndex_, const int iItemID_ );
#else //SERV_GLOBAL_BILLING
			std::pair<int, int> GetSelectedOptionList( const WCHAR* wszSocketItemGroupID_, const int iSelectedIndex_, const int iItemID_ );
#endif //SERV_GLOBAL_BILLING
#endif // SERV_CASH_ITEM_SOCKET_OPTION

#ifdef SERV_GLOBAL_BILLING
			vector< KBillOrderInfo >&	GetOrderInfoList() { return m_vecKBillOrderInfo; }
#ifdef SERV_GLOBAL_CASH_PACKAGE
			void GetCheckedPackageInfo( vector< KBillPackageInfo >& vecPackageInfo );
#endif // SERV_GLOBAL_CASH_PACKAGE
#else //SERV_GLOBAL_BILLING
			vector< KNXBTOrderInfo >&	GetOrderInfoList() { return m_vecKNXBTOrderInfo; }

			void GetCheckedPackageInfo( vector< KNXBTPackageInfo >& vecPackageInfo );
#endif //SERV_GLOBAL_BILLING			

			void SetCheckAllItem( bool bCheck );
			void ChangeCheckItem( CKTDGUIDialogType pDialog, bool bCheck );

			bool IsEmptyCheckedItemList() { return m_vecCheckedItem.empty(); }

		protected:

			void InitbyOpen();
			void ResetNowPage();

#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_GLOBAL_CASH_PACKAGE
			void GetPackageInfo( unsigned long ulTranNo, vector< KBillPackageInfo >& vecKBillPackageInfo );
#endif //SERV_GLOBAL_CASH_PACKAGE
			vector< KBillOrderInfo > m_vecKBillOrderInfo;
			vector< KBillPackageInfo > m_vecKBillPackageInfo;
#else // SERV_GLOBAL_BILLING
			void GetPackageInfo( unsigned long ulProductNo, vector< KNXBTPackageInfo >& vecKNXBTPackageInfo );
			
			vector< KNXBTOrderInfo > m_vecKNXBTOrderInfo;
			vector< KNXBTPackageInfo > m_vecKNXBTPackageInfo;
#endif // SERV_GLOBAL_BILLING
			map< int, int > m_mapSelectedOption;		//itemID �� �ɼ� ��ȣ.

			vector<int>		m_vecCheckedItem;
		};

		class PresentItemPopup
		{
			public:

				PresentItemPopup();
				~PresentItemPopup();

				void SetOpen( bool bOpen, int itemID );
				bool GetOpen();

				void UpdateCash();
				unsigned long GetSelectedProductNo();
 
				void SetSelectFriend( int index );
				wstring GetNickNameToPresent();
				wstring	GetPresentMsg();
				wstring GetPassword();

				void SetChoicedItem( bool bChoicedItem ) { m_bChoicedItem = bChoicedItem; }
				bool GetChoicedItem() { return m_bChoicedItem; }

#ifdef SERV_COUNTRY_PH
				bool RestPresentItemCashCheck( int iProductNo ); // 2013.09.17 lygan_������ // �����ϱ� ������ �ܾ� üũ�� �ؼ� �ܾ��� ���ڶ�� ������ REQ ������ �ʰ� �ϱ�
#endif //SERV_COUNTRY_PH

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				void SetUseCashType(int iCashType)
				{
#ifdef CLIENT_COUNTRY_CN
					if(iCashType == static_cast<int>(KGlobalCashInfo::GCT_KOG_ELSWORD_CASH))
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_Bonus" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(false);
						CKTDGUIRadioButton* pRadioEnableGamePoint = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_GamePoint" );
						if(pRadioEnableGamePoint != NULL)
							pRadioEnableGamePoint->SetChecked(true);

						m_iUseCashType = iCashType;
					}
					else
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_Bonus" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(true);
						CKTDGUIRadioButton* pRadioEnableGamePoint = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_GamePoint" );
						if(pRadioEnableGamePoint != NULL)
							pRadioEnableGamePoint->SetChecked(false);

						m_iUseCashType = static_cast<int>(KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT);
					}
#elif defined CLIENT_COUNTRY_TW
					if(iCashType == static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH))
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_Gash" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(false);
						CKTDGUIRadioButton* pRadioEnableCash = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_GamePoint" );
						if(pRadioEnableCash != NULL)
							pRadioEnableCash->SetChecked(true);

						m_iUseCashType = iCashType;
					}
					else
					{
						CKTDGUIRadioButton* pRadioEnableGash = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_Gash" );
						if(pRadioEnableGash != NULL)
							pRadioEnableGash->SetChecked(true);
						CKTDGUIRadioButton* pRadioEnableCash = (CKTDGUIRadioButton*)m_pDLGPresent->GetControl( L"Use_GamePoint" );
						if(pRadioEnableCash != NULL)
							pRadioEnableCash->SetChecked(false);

						m_iUseCashType = static_cast<int>(KGlobalCashInfo::GCT_PUBLISHER_CASH);
					}
#else
					m_iUseCashType = iCashType;
#endif //CLIENT_COUNTRY_CN
				}

				int GetUseCashType(){ return m_iUseCashType; }
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
			protected:

				void InitUI();
				

				bool			m_bOpen;
				int				m_ItemID;
				unsigned long	m_SelectedProductNum;
				CKTDGUIDialogType	m_pDLGPresent;

				bool			m_bChoicedItem;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				int				m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		};

	public:

		CX2CashShop( CKTDXStage* pStage, const WCHAR* wszFileName );
		virtual ~CX2CashShop(void);

		bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		wstring GetSlotCashItemPeriodDesc( int itemTID );
		wstring GetSlotCashItemDesc( int itemTID );

		virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

		void MouseMove( D3DXVECTOR2 mousePos );

		void SetOpen( bool bOpen );
		bool GetOpen() { return m_bOpen; }

		void ResetUnitViewer( CX2Unit* pUnit );
		void ResetUnitViewerEquip();

		void UpdateCashUI();

		bool GetAutoOpen() { return m_bAutoOpen; }
		void SetAutoOpen( bool bCheck ) { m_bAutoOpen = bCheck; }

		CX2UnitViewerUI* GetUnitViewerUI() { return m_pUnitViewerUI; }


		bool Handler_EGS_GET_WISH_LIST_REQ();
#ifdef SERV_GLOBAL_BILLING
		bool Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( int iPageIndex, int nItemPerPage );
#else // SERV_GLOBAL_BILLING
		bool Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( int iPageIndex, int nItemPerPage );
#endif // SERV_GLOBAL_BILLING

		//{{ kimhc // 2009-08-10 // UI���� ĳ�ü��� Ư�������� �ٷ� ���� ���� �߰�
#ifdef	PRIVATE_BANK
		void	SetMenuTypeCallingCashShop( int	iMenuType ) { m_iMenuTypeCallingCashShop = iMenuType; } // ĳ������ �θ� UI �޴��� Ÿ��
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-10 // UI���� ĳ�ü��� Ư�������� �ٷ� ���� ���� �߰�

#ifdef FIX_MISSING_MAIL
		void AddDelayedMail( KEGS_RECEIVE_LETTER_NOT& kEvent );
		void PushDelayedMails();
#endif FIX_MISSING_MAIL

#ifdef	SERV_GUILD_CHANGE_NAME
		//{{ ������ // 2012-03-14 // ���Ŵ������� ��� �����Ѵٸ�, ������ ������ �ʿ��� ������ �����ֱ� ���� �߰�
#ifdef SERV_GLOBAL_BILLING
		KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ* GetPurchasedCashItemInfo() { return &m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ; }
#else // SERV_GLOBAL_BILLING
		KEGS_GET_PURCHASED_CASH_ITEM_REQ* GetPurchasedCashItemInfo() { return &m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ; }
#endif // SERV_GLOBAL_BILLING
		//}} ������ // 2012-03-14 // ���Ŵ������� ��� �����Ѵٸ�, ������ ������ �ʿ��� ������ �����ֱ� ���� �߰�
#endif	//CHANGE_GUILD_NAME

#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD		
		//�ʵ� �����, �ش� �ʵ� ���� ĳ���� �������� ������ ���� ó��
		//���� ������ X2Game�� �����Ǿ� ���� �ʱ� ������ �ӽ� ����
		void SetEtnerCashShopUserList( std::vector< UidType >& EnterCashShopUserList_ ) { m_vecEnterCashShopUser.clear(); m_vecEnterCashShopUser = EnterCashShopUserList_; }
		void GetEnterCashShopUserList( OUT std::vector< UidType >& EnterCashShopUserList_ );
		void ClearEnterCashShopUserList(){m_vecEnterCashShopUser.clear();}
#endif //SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
		void ShowEventCategoryInCashShop();
		void OffEventCategoryInCashShop();
#endif ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef ADJUST_UNIT_CLASS_CHANGE_ITEM_REPETITION
		// �ش� ������ ���� �������� ���ϱ� ��Ͽ��� ������.
		void RemoveUnitClassChangeItemInChoice( const CX2Unit::UNIT_CLASS eUnitClass_ );
		// �ش� ���ֿ� �ش��ϴ� ���� ������ ID�� �޾ƿ�.
		const int GetUnitClassChangeItemID( const CX2Unit::UNIT_CLASS eUnitClass_ );
#endif //ADJUST_UNIT_CLASS_CHANGE_ITEM_REPETITION
		
#ifdef REFORM_SKILL_NOTE_UI
		void SetItemIDShowBuyUIAfterEnter(int iVal_) { m_iItemIDShowBuyUIAfterEnter = iVal_; }
#endif // REFORM_SKILL_NOTE_UI

	protected:

		void InitUI( CKTDXStage* pStage );

		void CategoryDataParsing( const WCHAR* pFileName );

#ifdef COUPON_SYSTEM
		bool Handler_EGS_BUY_CASH_ITEM_REQ( vector< std::pair<unsigned long, int> > vecProductAndCouponNoList, bool bChoicedItemBuy = false );
#else
		bool Handler_EGS_BUY_CASH_ITEM_REQ( vector< unsigned long >& vecProductNoList, bool bChoicedItemBuy = false );
#endif // COUPON_SYSTEM
		bool Handler_EGS_BUY_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING
		// 		bool Handler_EBILL_BUY_PRODUCT_REQ( vector< int >& vecProductNoList, bool bChoicedItemBuy = false );
		// 		bool Handler_EBILL_BUY_PRODUCT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_BILL_INVENTORY_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption, const map<int, int>& mapGroupdIdList_ );
#else	//SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption );
#endif	//SERV_CASH_ITEM_SOCKET_OPTION

		bool Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ();
		bool Handler_EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_GLOBAL_CASH_PACKAGE
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption, const map<int, int>& mapGroupdIdList_, vector< KBillPackageInfo >& vecPackageInfo );
#else	//SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( KBillOrderInfo& kKBillOrderInfo, map< int, int >& mapOption, vector< KBillPackageInfo >& vecPackageInfo );
#endif	//SERV_CASH_ITEM_SOCKET_OPTION

#endif //SERV_GLOBAL_CASH_PACKAGE

#else // SERV_GLOBAL_BILLING
		
		bool Handler_EGS_PURCHASED_CASH_ITEM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_, const map<int, int>& mapGroupdIdList_ );
#else	SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_GET_PURCHASED_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_ );
#endif	// SERV_CASH_ITEM_SOCKET_OPTION

		bool Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ();
		bool Handler_EGS_GET_PURCHASED_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef	SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_, const map<int, int>& mapGroupdIdList_, const vector< KNXBTPackageInfo >& vecPackageInfo_ );
#else	SERV_CASH_ITEM_SOCKET_OPTION
		bool Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REP( const KNXBTOrderInfo& kKNXBTOrderInfo_, const map< int, int >& mapOption_, const vector< KNXBTPackageInfo >& vecPackageInfo_ );
#endif	// SERV_CASH_ITEM_SOCKET_OPTION
#endif // SERV_GLOBAL_BILLING
		//////////////////////////////////////////////////////////////////////////

		bool Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ();
		bool Handler_EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		
		bool Handler_EGS_GET_WISH_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_MODIFY_WISH_LIST_REQ( bool bAdd, vector< int >& vecItemID );
		bool Handler_EGS_MODIFY_WISH_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_PRESENT_CASH_ITEM_REQ( const WCHAR* wszReceiverNickName, const WCHAR* wszMessage,
												const WCHAR* wszPassword, unsigned long ulProductNo, bool bChoicedItem = false );
		bool Handler_EGS_PRESENT_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHECK_PRESENT_CASH_INVENTORY_NOT( unsigned long ulOrderNo, unsigned long ulProductNo );

		bool Handler_EGS_EXPAND_CHAR_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void Send_EGS_VISIT_CASH_SHOP_NOT( const bool bOpen_ ) const;

#ifdef CASH_ITEM_REFUND
		bool Handler_EGS_CASH_ITEM_REFUND_REQ();
		bool Handler_EGS_CASH_ITEM_REFUND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		void SetStringBuyItemRefund();
#endif CASH_ITEM_REFUND

#ifdef COUPON_SYSTEM
		void Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ( const unsigned long ulProductNo_ );
		bool Handler_EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // COUPON_SYSTEM

		bool MouseUp(  D3DXVECTOR2 mousePos );
		bool MouseRButtonUp( D3DXVECTOR2 mousePos );
		bool MouseLRButtonUpEquip( CX2SlotItem* pItemSlot );

		void SetCashShopState( CASH_SHOP_STATE cashShopState, CASH_SHOP_CATEGORY cashShopCate = CSC_NONE );
		
		void ResetItemSlot( CX2Slot::SLOT_TYPE slotType, vector<int>& vecItemID );
#ifdef SERV_GLOBAL_BILLING
		void ResetDepositItemSlot( CX2Slot::SLOT_TYPE slotType, vector< KBillOrderInfo >& vecKBillOrderInfo );
#else // SERV_GLOBAL_BILLING
		void ResetDepositItemSlot( CX2Slot::SLOT_TYPE slotType, vector< KNXBTOrderInfo >& vecKNXBTOrderInfo );
#endif // SERV_GLOBAL_BILLING
		void DestroyItemSlotByType( const CX2Slot::SLOT_TYPE eSlotType_ );
		void ResetMainPageNumUI();
		void ResetSubPageNumUI();

		int GetItemID( CKTDGUIDialogType pDialog );

		void UnitHandling( double fTime, float fElapsedTime );
#ifdef PET_PREVIEW
		void MyUniyHandling( double fTime, float fElapsedTime );
		void PetHandling( double fTime, float fElapsedTime );		

		void UpdateDLGPetViewerUI();
		void ResetPetViewer(int iiPetID_ = -1, int iItemID = - 1, CX2ItemManager::PREVIEW_PET_TYPE ePreviewPetType_ = CX2ItemManager::PPT_PET);
		void PetPreview( int iItemID_ )	;
#endif //PET_PREVIEW
		bool IsInUnitClickReact( D3DXVECTOR2 mousePos );

		vector<int> GetItemByCategory( CASH_SHOP_CATEGORY categoryID );
		vector<int> GetItemByCategory( CASH_SHOP_CATEGORY categoryID, CASH_SHOP_SUB_CATEGORY subCategoryID );

		void ChangeCategory( CKTDGUIRadioButton* pRadioButton );
		void ChangeSubCategory( CKTDGUIRadioButton* pRadioButton );
		void ChangeSubCategory( CX2CashShop::CASH_SHOP_SUB_CATEGORY eSubCategory );

		static bool IsPossibleUsedByMyCharacter( int itemID );

		vector<int> GetHotItemList( vector<int>& vecItemID, int maxItemNum );

		bool CheckIsPossibleEqiupSlot( CX2Slot::SLOT_TYPE slotType );

		bool CheckPreEqipItem( int itemTID );
		void RemovePreEquipItemByEquipPos( int itemTID );
		bool InsertPreEquipItem( int itemTID, bool bForce = false );
 		void PreEquipping();

		void OnRevertItem( int iItemTID );
		void ResetNowEquipUI();
		void ClearNowEquipUI();
		void ResetNowEquipUIByInven( const CX2Unit::UnitData& kMyUnitData, const CX2Inventory& kInventory);
		void ResetNowEquipUIByShopEquip();
		void ResetSellCashItemByPreEquip();
		void UpdateShopEquipedPicture();
		void AttachShopEquipedPicture( CX2SlotItem* pItemSlot );
		void SetShowShopEquipedPicture( CX2SlotItem* pItemSlot, bool bShow );
	
		void AddChoicedItem( int itemID );
		void RemoveChoicedItem( int itemID );

		void SetEnableBuyEquippingItem( bool bEnable );
#ifndef IMPOSSIBLE_PRESENT_CASH_ITEM
		bool IsPossiblePresent( int itemID );
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM

		void ProcessBuyPopUpOk();
		
		//{{ kimhc // 2009-08-10 // �ڽ��� ���� ȸ�� ��޿� ���� ������ ������ �ʵ��� ��
#ifdef	PRIVATE_BANK
		bool IsPossibleUsedByMyBankMembership( int itemID ) const;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-10 // �ڽ��� ���� ȸ�� ��޿� ���� ������ ������ �ʵ��� ��

		static int GetClassChangeItemLevel( int itemID );
		const wstring GetClassNameByClassChangeItem( int itemID );

		//{{ kimhc // 2009-10-06 // �ڵ����� ó��
#ifdef	AUTO_PAYMENT
		void ProcessAutoPayment( int iItemID );
		bool Handler_EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ( UINT ulProductNo, USHORT usQuantity ) const;
		bool Handler_EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-06 // �ڵ����� ó��

#ifdef SERV_SKILL_NOTE
		static bool GetCanByeSkillNoteItem( int itemID );
#endif

#ifdef CHINA_CASH_CHANGE // 2011.02.25 lygan_������ // �߱� ���� ĳ�� ��ȯ UI��		
		void ExchangeChinaCash(int _icash);
		void ChinaCash_Tap_Change(bool _bShow);

		bool Handler_EGS_EXCHANGE_REQ(); // 2011.03.08 lygan_������ // ���̾�Ʈ ĳ�� ��ȯ ����
		bool Handler_EGS_EXCHANGE_ACK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // 2011.03.08 lygan_������ // ���̾�Ʈ ĳ�� ��ȯ ����
#endif //CHINA_CASH_CHANGE

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
		void ChinaCash_Tap_Change_Direct_Charge(bool _bShow);
		void SetShowChinaCash_Tap_Elsword(bool _bShow);
		void SetShowChinaCash_Tap_Giant(bool _bShow);
		void SetShowChinaCash_Tap_DirectCharge(bool _bShow);

		bool Handler_EGS_CASH_DIRECT_CHARGE_CN_REQ();
		bool Handler_EGS_CASH_DIRECT_CHARGE_CN_ACK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void ExecuteEpaySystem( std::wstring wstrToken = L"" );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_COUNTRY_PH
		bool Handler_EGS_EXCHANGE_CASH_CLIENT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // 2011.03.08 lygan_������ // ���̾�Ʈ ĳ�� ��ȯ ����
#endif //SERV_COUNTRY_PH

		void SetOpenNexonCashChargePage( OUT string& strURL_, OUT RECT& rcWebPage_ );	// �ؽ� ĳ�� ���� ��������

#ifdef SERV_NAVER_CHANNELING
		void SetOpenNaverCashChargePage( OUT string& strURL_, OUT RECT& rcWebPage_ );
#endif // SERV_NAVER_CHANNELING


		//{{ kimhc // 2011-08-10 // ä�θ�
#ifdef	SERV_TOONILAND_CHANNELING
		void SetOpenTooniCashChargePage( OUT string& strURL_, OUT RECT& rcWebPage_ );	// ���� ĳ�� ���� ��������
#endif	SERV_TOONILAND_CHANNELING
		//}} kimhc // 2011-08-10 // ä�θ�

		//{{ ������ // 2012-04-09 // �Ǹ�����
#ifdef SERV_NEXON_AUTH_SOAP
		void SetOpenCheckAuthority( OUT string& strURL_, OUT RECT& rcWebPage_ );	// �ؽ� �Ǹ����� ��������
#endif //SERV_NEXON_AUTH_SOAP
		//}} ������ // 2012-04-09 // �Ǹ�����

#ifdef UNIT_CLASS_CHANGE_ITEM
		static int GetUnitClassChangeItemLevel( int itemID );
		const wstring GetClassNameByUnitClassChangeItem( int itemID );
#endif

#ifdef	SERV_EXPAND_QUICK_SLOT
		static bool GetCahBuyExpandQuickSlotItem( int itemID );
#endif  SERV_EXPAND_QUICK_SLOT

#ifdef SERV_EVENT_DAILY_GIFT_BOX
		bool SetDialyGiftBoxPopup( int itemID );
#endif SERV_EVENT_DAILY_GIFT_BOX

		void SetShowOtherUI( bool bShowCashop_ );

#ifdef CJ_ID_WEB_BILLING
		bool InitCJBillingWebBrowser();
#endif //CJ_ID_WEB_BILLING
#ifdef LINK_CASH_SHOP_WHEN_JOB_CHANGE
		void JobChangeDirectLink( int iEventID );
#endif LINK_CASH_SHOP_WHEN_JOB_CHANGE

#ifdef SERV_SHARING_BANK_QUEST_CASH
		static bool GetCahBuySharingBankItem( int itemID );
#endif // SERV_SHARING_BANK_QUEST_CASH

#ifdef SERV_NOTIFY_BONUS_CASH
		bool Handler_EGS_BONUS_CASH_EVENT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // 2011.10.05 lygan_������ // ���ʽ� ĳ�� �̺�Ʈ �˸���
#endif //SERV_NOTIFY_BONUS_CASH

#ifdef TAKE_OFF_ALL_ITEM
		void ShowInvenEquipUI( bool bVal_ );
#endif // TAKE_OFF_ALL_ITEM


#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		void ResizePreEquipItemID();
		void BuyAllEquippingItems();
		void InitPreEquipItemID();
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST

#ifdef ADDED_RELATIONSHIP_SYSTEM
		bool CheckBuyPossibleRelationItem( int iItemID );
		bool CheckBuyPossibleRelationItem( const vector<int>& vecItemIDList );
#endif //ADDED_RELATIONSHIP_SYSTEM

		bool ShowPopUpGuideByItemID( const int iItemID_ );

#ifdef REFORM_SKILL_NOTE_UI
		void OpenBuyPopupByReserve();
#endif // REFORM_SKILL_NOTE_UI

		bool					m_bOpen;
		CASH_SHOP_STATE			m_CashShopState;
		CASH_SHOP_CATEGORY		m_CashShopCategory;
		CASH_SHOP_SUB_CATEGORY	m_CashShopSubCategory;

		CX2UnitViewerUI*		m_pUnitViewerUI;
#ifdef PET_PREVIEW
		CX2NPCUnitViewerUI*		m_pNPCViewerUI;
		int						m_iPrePetItemID;
#endif //PET_PREVIEW
		float				m_fRemainRotY;
		float				m_fMouseSensitivity;
		bool				m_bRotateReturn;

		D3DXVECTOR2			m_UnitClickPos;
		D3DXVECTOR2			m_UnitClickSize;

		CKTDGUIDialogType		m_pDLGBack;
		CKTDGUIDialogType		m_pDLGFront;
#ifdef CHINA_CASH_CHANGE // 2011.02.19 lygan_������ // �߱� ���� ĳ�� ��ȯ UI��
		CKTDGUIDialogType		m_pDLGCashchange;
		CKTDGUIDialogType		m_pDLGMsgBox;
		bool					m_bLowCashCheck;
		bool					m_bCashlistCheck;
		unsigned long			m_uiSubcash;
#endif //CHINA_CASH_CHANGE 

		CKTDGUIDialogType		m_pDLGMainPage;

		CKTDGUIDialogType		m_pDLGSubPage;
		CKTDGUIDialogType		m_pDLGSubPageMenu[CSC_END];		//���� ���������� �Žñ⿡ �°Բ� �޴� ǥ��..
#ifdef PET_PREVIEW
		CKTDGUIDialogType		m_pDLGPetPreview;
#endif //PET_PREVIEW
		bool				m_bAllCharItem;

		CashShopSlotManager* m_pMainNewSM;					
		CashShopSlotManager* m_pMainHotSM;
		CashShopSlotManager* m_pMainRecommendSM;

		CashShopSlotManager* m_pSubSM;						//�Ϲ� ������
		CashShopSlotManager* m_pSubSpeicialSM;

		BuyItemPopup*			m_pBuyItemPopUp;
		CashItemToInvenPopup*	m_pCashItemToInvenPopup;
		PresentItemPopup*		m_pPresentItemPopup;

#ifdef SERV_GLOBAL_BILLING
		vector< KBillOrderInfo >	m_vecKBillOrderInfo;
		vector< KBillPackageInfo >	m_vecKBillPackageInfo;
		KBillOrderInfo				m_TempCashItemToInven;
#else  //SERV_GLOBAL_BILLING
		vector< KNXBTOrderInfo >	m_vecKNXBTOrderInfo;
		vector< KNXBTPackageInfo >	m_vecKNXBTPackageInfo;
		KNXBTOrderInfo			m_TempCashItemToInven;
#endif //SERV_GLOBAL_BILLING
		int m_iNowDepositPageIndex;
		int m_iReqDepositPageIndex;

		vector< CashShopCategory* > m_vecCashShopCateList;
		
		bool						m_bAutoOpen;

		
		CKTDGUIDialogType			m_pDLGCheckCashItemToInven;

#ifdef SERV_GLOBAL_BILLING
		KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ			m_TempKEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ;
#else // SERV_GLOBAL_BILLING
		KEGS_GET_PURCHASED_CASH_ITEM_REQ			m_TempKEGS_GET_PURCHASED_CASH_ITEM_REQ;
#endif // SERV_GLOBAL_BILLING

		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ	m_TempKEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ;
		CKTDGUIDialogType			m_pDLGOverlapCheckCashItemToInven;

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		map<int, vector<int >>	m_mapPreEquipItemID;
#else
		vector<	int >			m_vecPreEquipItemID;
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST


		vector< int >			m_vecChoicedItemID;
		CKTDGUIDialogType			m_pDLGChoicedItemMenu;
		int						m_ChoicedItemMenuItemID;

		bool					m_bSend_EGS_BUY_CASH_ITEM_REQ;

		KEGS_BUY_CASH_ITEM_REQ  m_ReservedKEGS_BUY_CASH_ITEM_REQ;

#ifdef SERV_NOTIFY_BONUS_CASH
		bool						m_bCheckCashEvent;
		D3DXCOLOR					m_nowColor;
		float						m_fBlinkTime;
#endif SERV_NOTIFY_BONUS_CASH

		//{{ kimhc // 2009-08-10 // ���࿡�� ĳ�ü��� ��ġ������ �ٷ� ���� ���� �߰�
#ifdef	PRIVATE_BANK
		int						m_iMenuTypeCallingCashShop;		// ĳ������ �θ� UI �޴��� Ÿ��
		bool					m_bCheckDialogWarningPlatinumMembership;	// �÷�Ƽ�� ���Ž� ���̾�α׷� �ٽ� �ѹ� Ȯ�� �ؾ��ϴ°�?
		//KNXBTProductInfo		m_KNXBTProductInfoForAutoPayment;			// �����ϱ�� ������ �ڵ����� ��ǰ
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-10 // ���࿡�� ĳ�ü��� ��ġ������ �ٷ� ���� ���� �߰�

#ifdef CASH_ITEM_REFUND
		CKTDGUIDialogType			m_pDLGCheckCashItemRefund;
		unsigned long				m_RefundProductNo;
		unsigned long				m_RefundOrderNo;
		int							m_RefundQuantity;
#endif CASH_ITEM_REFUND

#ifdef SERV_EPAY_SYSTEM
		bool						m_bRunEpay;	// 2011.12.13 lygan_������ //epay�� ���������� Ȯ���ϴ� ��
		boost::timer                m_EpayTimer;
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_PSHOP_AGENCY
		bool						m_bUsePShopAgencyItem;
#endif SERV_PSHOP_AGENCY

#ifdef FIX_MISSING_MAIL
		std::vector< KEGS_RECEIVE_LETTER_NOT >	m_vecDelayedMail;
#endif FIX_MISSING_MAIL

#ifdef UIRRL_CASH_SHOP
		bool						m_bCreateDialog;
#endif //UIRRL_CASH_SHOP

		//{{ mauntain : ����ȯ [2012.06.12] ���ϸ��� ���� ���� ���� Dialog 
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		CKTDGUIDialogType			m_pDlgDailyGiftBox;
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
#ifdef SERV_EVENT_BINGO
		CX2UIBingo*					m_pBingo;
#endif //SERV_EVENT_BINGO


#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		//�ʵ� �����, �ش� �ʵ� ���� ĳ���� �������� ������ ���� ó��
		//���� ������ X2Game�� �����Ǿ� ���� �ʱ� ������ �ӽ� ����
		std::vector< UidType >			m_vecEnterCashShopUser;
#endif //SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD

//{{ 2013.03.13 ��ȿ��	ĳ������ ��ư�� ������ ���������� �����
#ifdef CASH_CHARGE_FROM_WEB
	std::wstring GetWstrConnectChargePage();
#endif CASH_CHARGE_FROM_WEB
//}}

#ifdef TAKE_OFF_ALL_ITEM
		bool						m_bShowInvenEquip;
#endif //TAKE_OFF_ALL_ITEM

#ifdef SERV_PVP_NEW_SYSTEM
		bool						m_bShowPartyUIOld;	// ĳ���� �������� ��ƼUI�� �����ְ� �־���
#endif //SERV_PVP_NEW_SYSTEM

#ifdef SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST
		UINT						m_uiSelectPreEquipSet;
		static const int			m_iMaxPreEquipSetNum = 3;
#endif // SAVE_CASH_SHOP_PRE_EQUIP_ITEM_LIST

#ifdef COUPON_SYSTEM
		CX2UICouponBox*				m_pUICouponBox;
		unsigned long				m_ulTempSelectedProductNo;
#endif // COUPON_SYSTEM

#ifdef REFORM_SKILL_NOTE_UI
		// ĳ�ü� ���� ���� �ٷ� �����˾� ��� ��ų ������ ID
		int							m_iItemIDShowBuyUIAfterEnter;
#endif // REFORM_SKILL_NOTE_UI

};
