//{{ kimhc // 2009-08-04 // ĳ���ͺ� ����
#ifdef	PRIVATE_BANK

#pragma once

namespace		_CONST_UI_PRIVATE_BANK_
{
	const	int		g_iBankQuestID		= 7035;		// ������ Ȯ�� �����ִ� ����Ʈ ID
	const	int		g_iBankQuestItemID	= 99380;	// ������ Ȯ�� �����ִ� ����Ʈ ItemID
}

class CX2UIPrivateBank : public CX2UIDragable
{
public:

	enum	UI_PRIVATE_BANK_CUSTOM_MSG
	{
		UPBCM_EXIT			= 34000,
		UPBCM_CASHSHOP,
//#ifdef GOOD_ELSWORD //JHKang	// �ؿ��� �ּ�
		UPBCM_ED,
		UPBCM_BUY_CONFIRM,
		UPBCM_BUY_CANCEL,
//#endif //GOOD_ELSWORD
//#ifdef SERV_SHARING_BANK_TEST
		UPBCM_USER_CHANGE,
		UPBCM_USER_SELECT,
		UPBCM_CHANGE_INVENTORY_SLOT_ITEM_OK,
		UPBCM_WANT_BUY_SHARING_CASH_ITEM,		// ���� : ���� �� �վ��� ���, ���� �Ͻðڽ��ϱ�? ������ OK ������ ȣ��
//#endif SERV_SHARING_BANK_TEST
	};
	
	CX2UIPrivateBank( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	virtual	~CX2UIPrivateBank();

	bool				InitUIPrivateBank( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	void				DestoryPrivateBank();

	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual bool		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const { return m_bShow; }
	void				SetShow( bool bShow );

	void				SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2			GetDLGSize() const { return m_vDlgSize; }
	void				SetPosition( D3DXVECTOR2 vec, bool bAbsolute = true );
    //const CX2Inventory*		GetInventory() const;								// �Ŀ� const�� �ٿ�����!!
	const CX2Inventory*		GetMyInventory() const;								// �Ŀ� const�� �ٿ�����!!
    CX2Inventory*		AccessMyInventory();								// �Ŀ� const�� �ٿ�����!!
	std::wstring		GetStrMembership( CX2Inventory::MEMBERSHIP_PRIVATE_BANK membershipGrade ) const;	// membership�� ���� ȸ����� string
#ifdef GOOD_ELSWORD //JHKang
	UINT				GetGradeSize( IN CX2Inventory::MEMBERSHIP_PRIVATE_BANK membershipGrade_ ) const;		// ���� ��޿� ���� ���� ��� ������ ���
#endif //GOOD_ELSWORD
	void				UpdateUICashShopButton();					// ĳ�ü����� �̵��� �� �ִ� ��ư �̹��� ������Ʈ
	void				UpdateUIStrMembership();					// (�Ϲ� ȸ��), (��� ȸ��) ���� string ���
	void				UpdateUIUsedAndMaxNumOfSlot();				// ������� ������ ������ ���� ��޿��� �ִ� ��� �� �� �ִ� ������ ����
	void				UpdateSlotList();
	void				UpdateUI();
	void				LostItemSlotList( bool bShow = false, bool bEnable = false );
	
#ifdef	SERV_SHARING_BANK_TEST
	void				ResetPrivateBank();

	void				SetShareNickName( wstring wstrNickName ) { m_wstrShareNickName = wstrNickName; }
	void				SetIsShareBank( bool bEnable ) { m_bIsOpenedShareBank = bEnable; }
	void				SetShareBankSize( int iSize ) { m_iBankSize = iSize; }
	void				SetShareUnitUID( UidType iShareUnitUID ) { m_ShareUnitUID = iShareUnitUID; }

	std::wstring		GetShareNickName() { return m_wstrShareNickName; }
	bool				GetIsShareBank() { return m_bIsOpenedShareBank; }
	int					GetShareBankSize() { return m_iBankSize; }
	UidType				GetShareUnitUID() { return m_ShareUnitUID; }

	bool				Handler_EGS_GET_SHARE_BANK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_SHARING_BANK_TEST

#ifdef	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
	bool				Handler_EGS_SHARE_BANK_UPDATE_OK_NOT();
#endif	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE

	virtual void		DrawSlotMouseOverImage();
	virtual wstring		GetSlotItemDesc();

	virtual bool		MouseDown( D3DXVECTOR2 mousePos );
	virtual bool		MouseUp( D3DXVECTOR2 mousePos );
	virtual bool		OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual bool		OnRClickedItem( D3DXVECTOR2 mousePos );

	//void				ToggleCashShop();
	
protected:
	bool				Handler_EGS_GET_MY_BANK_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef GOOD_ELSWORD //JHKang
#ifndef NO_GOOD_ELSWORD_INT
	bool	Handler_EGS_GET_NEXT_BANK_ED_REQ();
	bool	Handler_EGS_GET_NEXT_BANK_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_EXPAND_BANK_SLOT_ED_REQ();
	bool	Handler_EGS_EXPAND_BANK_SLOT_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif NO_GOOD_ELSWORD_INT
#endif //GOOD_ELSWORD

private:
	std::wstring					m_wstrDlgFileName;
	CKTDGUIDialogType				m_pDlgPrivateBank;
	bool							m_bShow;
	D3DXVECTOR2						m_vDlgSize;
	int								m_iMaxNumOfSlot;			// ���� ��޿��� ����� �� �ִ� �ִ� ������ ����
	int								m_iUsedNumOfSlot;			// ������� ������ ����
	//CX2CashShop*					m_pCashShop;

	//std::wstring					m_wstrNickName;				// ĳ���� �г���

#ifdef GOOD_ELSWORD //JHKang
	int					m_iConfirmED;
	CKTDGUIDialogType	m_pDLGBuyConfirm;
#endif //GOOD_ELSWORD

#ifdef SERV_SHARING_BANK_TEST
	bool							m_bIsOpenedShareBank;		// ���� ���� ���� �ִ���
	int								m_iBankSize;				// ���� ũ��
	std::wstring					m_wstrShareNickName;		// ���� �����ִ� ĳ���� �г���
	UidType							m_ShareUnitUID;
	bool							m_bReserveResetDialog;		// true �� ��� UI����
	CKTDGUIDialogType				m_pDlgWantBuySharingBank;	// ���� ���� �첨��? �޽��� ��� DLG
#endif SERV_SHARING_BANK_TEST

#ifdef SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
	CKTDGUIDialogType				m_pDLGWaitingMsgBox;
#endif SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
};

#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // ĳ���ͺ� ����