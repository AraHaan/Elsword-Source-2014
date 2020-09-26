#ifdef DEF_TRADE_BOARD

#pragma once

namespace		_CONST_UIPERSONALSHOPBOARD_INFO_
{
	const	int		g_iNumItemPerPage				= 7;
	const	int		g_iNumPagePerBoard				= 10;
	const	float	g_fWidthPage					= 100.f;
	const	double	g_dPersonalShopBoardFee			= 0.03;
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	const	int		g_iNextActiveIntervalFrame		= 5;		/// ���� ���۱����� ���� ( ������ )
	const	float	g_fSearchKeywordIntervalTime	= 0.2f;		/// ���� �ڵ� �ϼ� ��� ���۱����� ����	
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

class CX2UIPersonalShopBoard : public CX2ItemSlotManager
{
public:

	enum UI_PERSONAL_SHOP_BOARD_CUSTOM_MSG
	{
		UPSBCM_EXIT							= 38000,
		UPSBCM_COMBO_FIRST,
		UPSBCM_COMBO_SECOND,
		UPSBCM_COMBO_CHARACTER,
		UPSBCM_COMBO_CLASS,
		UPSBCM_CHECK_UNIQUE,
		UPSBCM_CHECK_ELITE,
		UPSBCM_CHECK_RARE,
		UPSBCM_CHECK_NORMAL,
		UPSBCM_CHECK_LOW,
		UPSBCM_CHECK_USABLE,
		UPSBCM_ENTER_ITEM_NAME,
		UPSBCM_ENTER_MIN_LEVEL,
		UPSBCM_ENTER_MAX_LEVEL,
		UPSBCM_PUSH_PAGE_NUM1,
		UPSBCM_PUSH_PAGE_NUM2,
		UPSBCM_PUSH_PAGE_NUM3,
		UPSBCM_PUSH_PAGE_NUM4,
		UPSBCM_PUSH_PAGE_NUM5,
		UPSBCM_PUSH_PAGE_NUM6,
		UPSBCM_PUSH_PAGE_NUM7,	
		UPSBCM_PUSH_PAGE_NUM8,
		UPSBCM_PUSH_PAGE_NUM9,
		UPSBCM_PUSH_PAGE_NUM10,
		UPSBCM_PUSH_PREV_BOARD,
		UPSBCM_PUSH_PREV_PAGE,
		UPSBCM_PUSH_NEXT_PAGE,
		UPSBCM_PUSH_NEXT_BOARD,
		UPSBCM_PUSH_SEARCH_ITEM,
		UPSBCM_PUSH_REFRESH,
		UPSBCM_PUSH_INIT_SEARCH_CONDITION,
		UPSBCM_PUSH_BUY_ITEM,
		UPSBCM_MOUSE_OUT_ITEM_NAME,
//#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		UPSBCM_CHANGE_ITEM_NAME,
		UPSBCM_SELECT_SEARCH_KEYWORD,
		UPSBCM_CHECK_STRICT_SEARCH_OPTION,
		UPSBCM_CHECK_ALL
//#endif //SERV_UPGRADE_TRADE_SYSTEM
	};


public:
	CX2UIPersonalShopBoard( CKTDXStage* pNowStage, const WCHAR* pFileName );
	~CX2UIPersonalShopBoard();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );

	UidType				GetSelectedPersonalShopUid(){ return m_iSelectedPersonalShopUid; }

	wstring				GetSlotItemDesc();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	void				AddBanItemNameList_LUA();
#endif //SERV_UPGRADE_TRADE_SYSTEM


private:
	void				InitSearchCondition();


	void				SetIMMEditMinLevel(int iMinLevel);
	void				SetIMMEditMaxLevel(int iMaxLevel);

	void				UpdateItemList();

	void				EnableButtonSearchItem( bool bEnable );
	void				UpdateButtonSearchItem();
	void				UpdateCheckBoxColor();

	void				UsableFilter();
	void				UsableUnFilter();

	//{{ kimhc // 2011-03-27 // �����˻����� �̸� �Ծ��
#ifdef	PRE_EQUIP_PERSONAL_SHOP_BOARD
	bool				MouseRButtonUp( const D3DXVECTOR2& v2MousePos );
#endif	PRE_EQUIP_PERSONAL_SHOP_BOARD
	//}} kimhc // 2011-03-27 // �����˻����� �̸� �Ծ��

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	void				UpdateSearchKewordList( IN const wstring& wstrSearchKeyword_ );
	void				SetSearchKeywordList( IN const wstring wstrSearchKeyword, OUT vector<wstring>& vevSearchKeyword_ );
	void				SetShowSearchKeywordList( IN bool bShow_ );
	void				SetSearchKeywordBySelectedItem();
	void				SetFocusOutSearchKeyword();
	bool				CheckSelectCategoryOption( IN const CX2Item::ItemTemplet* pItemTemplet_ );
	bool				OpenScriptFile( const WCHAR* pFileName );
#endif //SERV_UPGRADE_TRADE_SYSTEM
	

public:

	bool				Handler_EGS_SEARCH_TRADE_BOARD_REQ();
	bool				Handler_EGS_SEARCH_TRADE_BOARD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	bool								m_bShow;

	bool								m_bOldApplyDelete;
	bool								m_bRegMyGuildAd; 

	std::wstring						m_wstrLastMessage;
	std::wstring						m_wstrLastADMessage;

	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgPersonalShopBoard;
									

	UINT								m_uiNowPageJoinList;					// ���� �������� ������ �� (���� ��û ���)
	UINT								m_uiMaxPageJoinList;					// ���� ������ �� �ִ� �ִ� ������ ��(���� ��û ���)


	float								m_fCurrentTime;
	float								m_fLastSearchTime;



	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;
	//////////////////////////////////////////////////////////////////////////


	UINT								m_uiFirstPageinCurrentBoard;
	UINT								m_uiNowPage;
	UINT								m_uiMaxPage;
	
	
	UINT								m_iComboFirstindex;
	UINT								m_iComboSecondindex;
	UINT								m_iComboCharacterindex;
	UINT								m_iComboClassindex;

	UINT								m_iIMEEditMinLevel;
	UINT								m_iIMEEditMaxLevel;

	bool								m_bCheckUnique;
	bool								m_bCheckElite;
	bool								m_bCheckRare;
	bool								m_bCheckNormal;
	bool								m_bCheckLow;
	
	bool								m_bCheckUsable;

	wstring								m_wstrFinder;

	std::vector<KTradeBoardItemInfo>	m_vecItemInfo;
	std::vector<KTradeBoardItemInfo>	m_vecItemInfoBackup;

	CKTDGUIButton*						m_pButtonPageNum[_CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumPagePerBoard];
	
	CKTDGUIStatic*						m_pStaticPageNum;
	
	CKTDGUIButton*						m_pButtonPrevBoard;
	CKTDGUIButton*						m_pButtonPrevPage;

	CKTDGUIButton*						m_pButtonNextPage;
	CKTDGUIButton*						m_pButtonNextBoard;

	CKTDGUIButton*						m_pButtonSearchItem;
	CKTDGUIButton*						m_pButtonRefresh;
	CKTDGUIButton*						m_pButtonInitSearchCondition;
	CKTDGUIButton*						m_pButtonBuyItem;

	CKTDGUIComboBox*					m_pComboFirst;
	CKTDGUIComboBox*					m_pComboSecond;
	CKTDGUIComboBox*					m_pComboCharacter;
	CKTDGUIComboBox*					m_pComboClass;

	CKTDGUICheckBox*					m_pCheckUnique;
	CKTDGUICheckBox*					m_pCheckElite;
	CKTDGUICheckBox*					m_pCheckRare;
	CKTDGUICheckBox*					m_pCheckNormal;
	CKTDGUICheckBox*					m_pCheckLow;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	CKTDGUICheckBox*					m_pCheckALL;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	CKTDGUICheckBox*					m_pCheckUsable;

	CKTDGUIIMEEditBox*					m_pIMEEditItemName;
	CKTDGUIIMEEditBox*					m_pIMEEditMinLevel;
	CKTDGUIIMEEditBox*					m_pIMEEditMaxLevel;

	CKTDGUIDialogType					m_pItemList[_CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage];
	//////////////////////////////////////////////////////////////////////////

	UidType								m_iSelectedPersonalShopUid;


	UidType								m_iPersonalShopUidList[_CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage];
#ifdef SERV_PSHOP_AGENCY
	UidType								m_iShopOwnerUidList[_CONST_UIPERSONALSHOPBOARD_INFO_::g_iNumItemPerPage];
#endif

	// oasis907 : ����� [2009.12.29] //
	CKTDGUIDialogType					m_pDlgGuildAdvertiseMessage;
	CKTDGUIDialogType					m_pDlgGuildJoinMessage;



//}} oasis907 : ����� [2009.12.28] //

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ

	CKTDGUIDialogType					m_pDLGAutoSearchKeyword;			/// �˻��� �ڵ� �ϼ�â ���̾�α�
	CKTDGUIListBox*						m_pListboxSearchKeyword;			/// �˻��� �ڵ� �ϼ�â ����Ʈ �ڽ� ��ü
	int									m_iCloseSearchKeywordListTime;		/// �˻��� �ڵ� �ϼ�â �ݱ������ ��� �ð� ( ������ ���� )
	int									m_iFocusOutSearchKeywordBoxTime;	/// �˻��� �Է�â ��Ŀ�� ��Ż��, ���� ó�������� ��� �ð� ( ������ ���� )
	int									m_iFocusInSearchKeywordBoxTime;		/// �˻��� �Է�â ��Ŀ�� ���������� ��� �ð� ( ������ ���� )
	float								m_fActiveSearchKeywordListTime;		/// �˻��� �ڵ� �ϼ�â ���� ����  ( �� ���� )

	CKTDGUICheckBox*					m_pStrictSearchOption;				/// ��ġ�ϴ� ������ �˻� üũ �ڽ�
	bool								m_bIsStrictSearchOption;			/// ��ġ�ϴ� ������ �˻� ���� ����

	vector<wstring>					m_vecBanItemList;					/// �˻��� �ڵ� �ϼ� â�� ������ �ʵǴ� ������ ����Ʈ
#endif //SERV_UPGRADE_TRADE_SYSTEM
};
#endif DEF_TRADE_BOARD
