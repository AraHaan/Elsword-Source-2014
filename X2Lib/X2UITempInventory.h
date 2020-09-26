//{{ kimhc	// �ǽð� ���ҵ� �� �ǽð� ������ ȹ�� �� ������ �ӽ� �κ��丮 Ŭ����
// ���� ������ ������ ȹ��� �κ��丮�� ���� á�� ��� �ӽ� �κ��丮�� ������
#ifdef	REAL_TIME_ELSWORD

#pragma once

class CX2UITempInventory : public CX2ItemSlotManager
{
	// Ŭ���� ���� Message ���� �κ�
public:

	enum UI_TEMP_INVENTORY_CUSTOM_MSG
	{
		UTICM_EXIT			= 30000,
		UTICM_GET_ITEM,
		UTICM_DELETE_ITEM,
		UTICM_PREV_ITEM,
		UTICM_NEXT_ITEM
	};

	// ������, �Ҹ��� ���� �κ�
public:

	CX2UITempInventory( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	virtual ~CX2UITempInventory();

	// ��� �Լ�
	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual bool		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const { return m_bShow; }
	void				SetShow( bool val );

	int					GetNumberOfItem() const { return m_iNumberOfItem; }


	void				SetLayer(X2_DIALOG_LAYER layer);
	D3DXVECTOR2			GetDLGSize() const { return m_vSizeDlg; }
	void				AddTempItem( const KTempInventoryItemInfo& kTempInventoryItemInfo ); 
	void				CreateTempInventory(  const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	void				ResetTempInventoryInfo();

private:
	void				AddItemToInventory( const std::vector< KInventoryItemInfo >& vecKInventoryItem );
	bool				RemoveTempItem( int iItemUID );
	void				DesotryTempInventory();
	void				LostTempInventory();
	bool				IsInventoryChangedShowState();
	bool				UpdateSlotList();
	void				UpdatePageUI() const;
	void				DisplayTitle() const;
	void				MovePrevItem();
	void				MoveNextItem();
	void				MovePosition( D3DXVECTOR2 vec );


private:
	// �ڵ鷯
	// REQ
	bool				Hander_EGS_GET_TEMP_ITEM_REQ();
	bool				Hander_EGS_DELETE_TEMP_ITEM_REQ();

	// ACK
	bool				Hander_EGS_GET_TEMP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Hander_EGS_DELETE_TEMP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// NOT


	// ��� ����
private:
	int										m_iNowItemIndex;
	int										m_iNumberOfItem;
	bool									m_bInventoryShow;
	bool									m_bShow;
	D3DXVECTOR2								m_vSizeDlg;
	D3DXVECTOR2								m_vPosDlg;
	CKTDGUIDialogType							m_pDlgTempInventory;
	std::vector< KTempInventoryItemInfo >	m_vecTempInventoryInfo;
};

#endif	REAL_TIME_ELSWORD
//}} kimhc	// �ǽð� ���ҵ� �� �ǽð� ������ ȹ�� �� ������ �ӽ� �κ��丮 Ŭ����