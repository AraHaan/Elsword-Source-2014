//{{ kimhc // 2009-07-25 // ��ϸ��� �ð��� �߰��Ǵ� ITEM_EXCHANGE_SHOP �۾�
#ifdef	ITEM_EXCHANGE_SHOP

#pragma once

namespace		_CONST_UI_ITEM_EXCHANGE_SHOP_
{
	const int	g_iNumOfSlotSet				=	5;		// �������� �� ���Լ�Ʈ�� ����
#ifdef SERV_9TH_NEW_CHARACTER // �ؿ��� �ڵ� �߰�

#ifdef SERV_LIMIT_UNIT
#if defined( SERV_COUNTRY_TWHK ) || defined( SERV_COUNTRY_JP ) || defined( SERV_COUNTRY_EU ) || defined( SERV_COUNTRY_US ) || defined( SERV_COUNTRY_CN )
	const int	g_iNumOfSlotInSlotSet = 8; // �����ý�
#elif defined( SERV_COUNTRY_TH ) || defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_BR ) || defined( SERV_COUNTRY_PH ) || defined( SERV_COUNTRY_IN )
	const int	g_iNumOfSlotInSlotSet = 7; // û
#else
	const int	g_iNumOfSlotInSlotSet = 9;
#endif //SERV_COUNTRY_XX
#else //SERV_LIMIT_UNIT
	const int	g_iNumOfSlotInSlotSet		=	9;		// �� ���� ��Ʈ �� ��ȯ�����۽����� ����(������ ���� ������ ���� ����)	( ���� ���� : 6 -> 7 )
#endif //SERV_LIMIT_UNIT
	
#else
#if defined(SERV_NEW_CHARACTER_EL) && !defined(SERV_NEW_CHARACTER_EL_DISABLE)
	const int	g_iNumOfSlotInSlotSet		=	8;		// �� ���� ��Ʈ �� ��ȯ�����۽����� ����(������ ���� ������ ���� ����)	( ���� ���� : 6 -> 7 )
#else
	const int	g_iNumOfSlotInSlotSet		=	7;		// �� ���� ��Ʈ �� ��ȯ�����۽����� ����(������ ���� ������ ���� ����)	( ���� ���� : 6 -> 7 )
#endif SERV_NEW_CHARACTER_EL
#endif SERV_9TH_NEW_CHARACTER
}
class CX2UIItemExchangeShop : public CX2ItemSlotManager
{
public:

	enum	ITEM_EXCHANGE_SHOP_CUSTOM_MSG
	{
		IESCM_EXIT					= 33000,
		IESCM_PUSH_PREV_PAGE,
		IESCM_PUSH_NEXT_PAGE,
		IESCM_FIRST_EXCHANGE_BUTTON,
		IESCM_SECOND_EXCHANGE_BUTTON,
		IESCM_THIRD_EXCHANGE_BUTTON,
		IESCM_FOURTH_EXCHANGE_BUTTON,
		IESCM_FIFTH_EXCHANGE_BUTTON,
		IESCM_OK,
		IESCM_CANCEL,
	};

	struct	SlotSet
	{
		CKTDGUIStatic*			m_pStaticSlotSet;
		CKTDGUIButton*			m_pButtonForExchange;
		bool					m_bUserHave;			// ��ȯ ��� �������� ������ ������ �ִ°�?
		int						m_eType;				// invalid, all, random, select ����
		int						m_iSrcItemID;			// ��ȯ ��� itemID

		//int						m_iStartIndexForExchangeData;				// ���� ���Լ�Ʈ�� ���ϴ� element�� m_vecItemExchangeData, CX2SlotItem ���� �������� ������ġ
		//int						m_iEndIndexForExchangeData;					// ���� ���Լ�Ʈ�� ���ϴ� element�� m_vecItemExchangeData, CX2SlotItem ���� �������� ��������ġ
		//int						m_iStartIndexForSlotList;					// ���� ���Լ�Ʈ�� ���ϴ� element�� m_vecItemExchangeData, CX2SlotItem ���� �������� ������ġ
		//int						m_iEndIndexForSlotList;						// ���� ���Լ�Ʈ�� ���ϴ� element�� m_vecItemExchangeData, CX2SlotItem ���� �������� ��������ġ

		SlotSet()
		{
			m_pStaticSlotSet						= NULL;
			m_pButtonForExchange					= NULL;
			m_bUserHave								= false;
			m_eType									= CX2ItemManager::IET_INVALID;
			m_iSrcItemID							= -1;
			/*m_iStartIndexForExchangeData			= 0;
			m_iEndIndexForExchangeData				= 0;
			m_iStartIndexForSlotList				= 0;
			m_iEndIndexForSlotList					= 0;*/
		}
	};

public:
	CX2UIItemExchangeShop( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	virtual ~CX2UIItemExchangeShop();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );
	void				SetHouseID( int iHouseID );
	virtual		bool	MouseDown( D3DXVECTOR2 mousePos );
#ifdef SERV_ITEM_EXCHANGE_NEW // ������ �� �� �θ� �� �ؿ��� ����
	void				InvalidLastSrcItem();
	void				InvalidLastSelectExchange();
	void				PlayWarningSelectExchange();
	void				StopWarningSelectExchange();
	bool				SelectExchangeSrcItem( UidType iSrcItemUID );
	bool				CheckExchangeUseItem( int itemID );
	bool				Handler_EGS_ITEM_EXCHANGE_REQ( UidType iSrcItemUID, int iSrcItemID, int iSrcQuantity, int iSelectedItemID = 0);
#ifdef EXCHANGE_OPEN_IMAGE
	bool				Handler_EGS_ITEM_EXCHANGE_REQ() { return Handler_EGS_ITEM_EXCHANGE_REQ( m_iSrcItemUID, m_iSrcItemID, m_iSrcQuantity, m_iDestItemID ); }
#endif EXCHANGE_OPEN_IMAGE
#endif SERV_ITEM_EXCHANGE_NEW

private:
	// �ڵ鷯
	bool				Handler_EGS_ITEM_EXCHANGE_REQ( int iSrcItemID, int iSelectedItemID = 0);
	bool				Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


private:
	// ����Լ�
	bool				InitItemExchangeData();
	bool				InitNowPageAndMaxPage();
	bool				UpdatePageString();
	bool				UpdatePageUI();
	bool				UpdateSlotSetUI( CKTDGUIStatic* pStaticSlotSet, CX2ItemManager::ITEM_EXCHANGE_TYPE eType );
	bool				UpdateSlotList();
	void				LostItemSlotList();
	//void				DestoryUIItemExchangeShop();
	int					SearchItemInUserInventory( int iItemID );	// �κ��丮�� �ش� �������� ���� ��� -1 ����
#ifdef SERV_ITEM_EXCHANGE_NEW // ������ �� �� �θ� �� �ؿ��� ����
	bool				PushedExchangeButton( int iCustomMsg, bool bPopupCenter = false );
#else
	bool				PushedExchangeButton( int iCustomMsg );
#endif SERV_ITEM_EXCHANGE_NEW

    const wchar_t*      GetItemFullName_( int iItemID );
	//	bool				CompareVector( std::vector< CX2ItemManager::ItemExchangeData >& vFirst, std::vector< CX2ItemManager::ItemExchangeData >& vSecond );

private:
	// �������
	bool								m_bShow;
	const CKTDXStage* const				m_pNowStage;
	int									m_iHouseID;				// X2LocationManager::HOUSE_ID
	CKTDGUIDialogType						m_pDlgBackground;		// ������ ��ȯ ���̾�α�
	//CKTDGUIDialogType						m_pDlgSelectedSlot;		// m_pDlgSelectedItem�� mousemove�� ���ǰ�, �̰��� mousedown�� ����
	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;
	UINT								m_uiNowPage;			// ���� �������� ������ ��
	UINT								m_uiMaxPage;			// ���� ������ �� �ִ� �ִ� ������ ��
	UINT								m_uiSelectedSlotID;		// ���õ� ������ ID
#ifdef SERV_ITEM_EXCHANGE_NEW // ������ �� �� �θ� �� �ؿ��� ����
	int									m_iLastCustomMsg;
	int									m_iLastSrcItemID;
	UidType								m_iLastSelectSrcItemUID;
	CKTDGUIDialogType					m_pDLGWarningSelectExchange;
#endif SERV_ITEM_EXCHANGE_NEW
	CKTDGUIDialogType						m_pDlgExchangeConfirm;	// ��ȯ Ȯ�� ���̾�α�

	SlotSet								m_SlotSet[_CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet];
	
	//std::vector< CX2ItemManager::ItemExchangeData > m_vecTempItemExchangeData;
	std::vector< std::vector< CX2ItemManager::ItemExchangeData > > m_vecItemExchangeData;

#ifdef SERV_EVENT_MONEY
	// EGS_UPDATE_EVENT_MONEY_NOT�� EGS_ITEM_EXCHANGE_ACK���� ������ ��Ŷ���� ������ ���� �� ���� ������
	// EGS_UPDATE_EVENT_MONEY_NOT�� �ޱ� ���� EGS_ITEM_EXCHANGE_ACK�� ���� ���� ���
	// ���� ���� Ŭ�󿡼� ���� ���� �ֱ� ���� ���� ������ ����ϴ� ����
	int									m_iFeatherResultQuantity; 
#endif //SERV_EVENT_MONEY

#ifdef EXCHANGE_OPEN_IMAGE
	UidType		m_iSrcItemUID;
	int			m_iSrcItemID;
	int			m_iSrcQuantity;
	int			m_iDestItemID;
#endif EXCHANGE_OPEN_IMAGE
};

#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // ��ϸ��� �ð��� �߰��Ǵ� ITEM_EXCHANGE_SHOP �۾�