#ifdef NEW_VILLAGE_UI

#pragma once

//class CX2StateMenu;

class CX2UIManufacture : public CX2ItemSlotManager
{
public:

	struct ManufactureSlot
	{
		CKTDGUIStatic*			m_pStatic;
		CKTDGUIButton*			m_pButton;
		int						m_iManufactureID;

		ManufactureSlot()
		{
			m_pStatic = NULL;
			m_pButton = NULL;
			m_iManufactureID = -1;
		}
		~ManufactureSlot()
		{
		}
	};


	enum UI_MANUFACTURE_CUSTOM_MSG
	{
		UMCM_EXIT	= 21000,
		UMCM_CATEGORY_WEAPON,
		UMCM_CATEGORY_COAT,
		UMCM_CATEGORY_PANTS,
		UMCM_CATEGORY_GLOVE,
		UMCM_CATEGORY_SHOES,
		UMCM_CATEGORY_QUICKSLOT,
		UMCM_CATEGORY_ETC,

		UMCM_SHOW_OTHER_CHARACTER_EQUIP_ON,
		UMCM_SHOW_OTHER_CHARACTER_EQUIP_OFF,

		UMCM_PREV_PAGE,
		UMCM_NEXT_PAGE,

		UMCM_SELECT_ITEM,		// �� ����..
		UMCM_MAKE,

		UMCM_RESULT_CLOSE,
		UMCM_MAKE_OK,
#ifdef SERV_MANUFACTURE_FIX
		UMCM_REGISTER_QUANTITY_PLUS,
		UMCM_REGISTER_QUANTITY_MINUS,
		UMCM_REGISTER_QUANTITY_UPDATE,
		UMCM_REGISTER_QUANTITY_CONFIRM,
		UMCM_REGISTER_QUANTITY_CANCEL,
#endif //SERV_MANUFACTURE_FIX
	};

	enum MANUFACTURE_TYPE
	{
		MT_NONE = 0,
		MT_WEAPON,
		MT_DEFENCE_BODY,
		MT_DEFENCE_LEG,
		MT_DEFENCE_HAND,
		MT_DEFENCE_FOOT,
		MT_QUICK_SLOT,
		MT_ETC,
		MT_END,
	};


	CX2UIManufacture( CKTDXStage* pNowStage, const WCHAR* pFileName );
	~CX2UIManufacture();

	// �⺻������ �� ��� �ϴ� ��
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//���� ����
	//virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );
	// 
	// 	void DrawSlotMouseOverImage();
	// 	wstring GetSlotItemDesc();

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow( bool val );

	//{{ kimhc // 2009-11-23 // ���� UI ����
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
	void SetHouseID( int iHouseID ) { m_iNowOpenHouseID = iHouseID; }	// ���� ������ NPC ���� �����ϵ���
	CKTDGUIRadioButton* GetRadioButtonByType( MANUFACTURE_TYPE eType ) const;		// ���� Ÿ�Ժ� ��ư ������ ������
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-11-23 // ���� UI ����

#ifdef SERV_MANUFACTURE_PERIOD_FIX
	void SetPeriodGroup( std::map< int, int > mPeriodGroup ) { m_mapPeriodGroup = mPeriodGroup; }
#endif //SERV_MANUFACTURE_PERIOD_FIX

	void SetLayer(X2_DIALOG_LAYER layer);
	void SetPosition(D3DXVECTOR2 vec);

	//{{ ����� : [2009/7/29] //	��� ��� �����ֱ� üũ�ڽ� üũ���� ���� �Լ�
	void SetShowAllItem( bool bShow ) { m_bShowAllCharItem = bShow; }
	//}} ����� : [2009/7/29] //	��� ��� �����ֱ� üũ�ڽ� üũ���� ���� �Լ�

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

protected:
	bool Handler_EGS_ITEM_MANUFACTURE_REQ();
	bool Handler_EGS_ITEM_MANUFACTURE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ���� ��ƿ��Ƽ �Լ�
 	CKTDGUIRadioButton* GetRadioButtonNameByCategory( MANUFACTURE_TYPE ItemCategory );
 	bool IsPossibleUsedByMyCharacter( const CX2Item::ItemTemplet* pItemTemplet );

	void GetManufactureList();
	int GetSlotperPage();
	void ChangeManufactureTab( int nPage );
	CX2UIManufacture::MANUFACTURE_TYPE GetManufactureTypeByItemID( int itemID );
#ifdef SERV_MANUFACTURE_FIX
	bool CheckIsThereAllMaterial( int manufactureID, int nQuantity  = 1);
	bool CheckIsEnoughED( int manufactureID, int nQuantity = 1 );
#else
	bool CheckIsThereAllMaterial( int manufactureID );
	bool CheckIsEnoughED( int manufactureID );
#endif //SERV_MANUFACTURE_FIX
	bool CheckIsThereAllMaterialNoEquipped( int manufactureID );

	void SelectRecipe( int iSlotNum );	
	void ResetRecipeMaterial();
	wstring GetSlotItemDesc();

	void AttachSelectedPicture( CX2SlotItem* pItemSlot );
	void SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow );
	
	bool CheckManufacture();

#ifdef SERV_MANUFACTURE_FIX
	bool CheckIsQuantity( int manufactureID );	// ���� ����� �������̳�?
	bool CheckCanMake( int manufactureID, int nQuantity = 1);		// ������ �����ϳ�?
	// ���� : �ִ� ������ ���ϱ�
	int GetMaxMakeNum( int manufactureID);

	void OpenRegisterQuantityDLG();
	void UpdateRegisterQuantityDLG( bool bReadIME );
#endif

private:
 
 	// ����
 	bool					m_bShow;
	bool					m_bWaitForManufactureResult;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;
 	
	// ���̾�α�
	CKTDGUIDialogType			m_pDLGManufacture;
	CKTDGUIDialogType			m_pDLGManufactureResult;
	CKTDGUIDialogType			m_pDLGMakeQuery;


// 	// ( ȭ�鿡 8���� ���̴� ) ������ ����Ʈ
 	vector<ManufactureSlot*>		m_vecRecipeSlot;
	int						m_nNowPage;
	int						m_nMaxPage;
	bool					m_bShowAllCharItem;

	int						m_SelectedRecipeID;

	// ������ �� �ִ� ������ ��ü ����Ʈ, �������� 
	// array of vector<int> -_-
	vector< int >			m_vecManufactureIDList[MT_END];
	//vector< int >			m_vecSortedManufacture;
	MANUFACTURE_TYPE		m_NowItemCategory;


	//{{ kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
	int						m_iNowOpenHouseID;	// ���� ���� ���� NPC�� HOUSE_ID
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef SERV_MANUFACTURE_PERIOD_FIX
	std::map< int, int >				m_mapPeriodGroup;
#endif //SERV_MANUFACTURE_PERIOD_FIX

#ifdef SERV_MANUFACTURE_FIX
	CKTDGUIDialogType			m_pDLGItemEnroll;
	UidType						m_EnrollItemUid;
	int							m_nQuantity;
#endif //SERV_MANUFACTURE_FIX

};

#endif
