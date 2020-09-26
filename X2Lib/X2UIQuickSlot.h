#ifdef NEW_VILLAGE_UI

#pragma once

// StateMenu���� �и���Ŵ
class CX2UIQuickSlot : public CX2UIDragable
{
public:
	CX2UIQuickSlot( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuickSlot(void);

	
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool KeyEventProcess();

	//virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool GetShow(){ return m_bShow; }
	void SetShow(bool val);
	bool GetEnable(){ return m_bEnable; }

#ifdef SERV_NEW_DEFENCE_DUNGEON
	void SetEnable(bool val, bool bDirectEnable = false);
#else //SERV_NEW_DEFENCE_DUNGEON
	void SetEnable(bool val);
#endif //SERV_NEW_DEFENCE_DUNGEON

	void SetLayer(X2_DIALOG_LAYER layer);

	void ResetQuickSlotUI();
	CX2SlotItem* GetEmptyslot();


	bool GetWaiting_EGS_USE_QUICK_SLOT_ACK() const { return m_bWaiting_EGS_USE_QUICK_SLOT_ACK; }
#ifdef SPECIAL_USE_ITEM
	void SetWaiting_EGS_USE_QUICK_SLOT_ACK( bool bWaiting_EGS_USE_QUICK_SLOT_ACK ) { m_bWaiting_EGS_USE_QUICK_SLOT_ACK = bWaiting_EGS_USE_QUICK_SLOT_ACK; }
#endif SPECIAL_USE_ITEM

	//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ������ ��ü
#ifdef REAL_TIME_ELSWORD
	void UpdateCoolTime( int itemID );
	bool ResetCoolTime();				// ������ ���� ��� �������� ��Ÿ�� �ʱ�ȭ
#endif REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ������ ��ü

	//{{ kimhc // 2009-08-08 //	���࿡ �ִ� item, drag & drop �� UIDragable�� �ִ� ���� �״�� ����� �ÿ�
	//							�����߻��ǹǷ� ������ ��
#ifdef	PRIVATE_BANK
	//virtual void	DrawSlotMouseOverImage();
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-08 // ���࿡ �ִ� item, drag & drop �� UIDragable�� �ִ� ���� �״�� ����� �ÿ�
	//							�����߻��ǹǷ� ������ ��

#ifdef	SERV_EXPAND_QUICK_SLOT
	void SetExpandQuickSlot( int iMaxNumOfSlot );
#endif  SERV_EXPAND_QUICK_SLOT

	void InitMyQuickSlotCoolTimeFromGageManager( const map<int, int>& mapQuickSlotCoolTime_ );
	void GetQuickSlotStringByIndex( IN int iIndex_, OUT wstring& strQuickSlot_ ) const;
	void UpdateSlotKey();
	

#ifdef CREATE_ALLY_NPC_BY_MONSTER
	void UpdateCoolTimeByGroupID( int iCoolTimeGroupID );
#endif //CREATE_ALLY_NPC_BY_MONSTER

#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool GetIsSummonCardSlot() const { return m_bIsSummonCardSlot; }	/// ����� �� ���� ī��� ������ ���� ��ȯ
	void SetIsSummonCardSlot(bool val) { m_bIsSummonCardSlot = val;}	/// ����� �� ���� ī��� ������ ���� ����
#endif //SERV_NEW_DEFENCE_DUNGEON

protected:
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem );
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );
	bool Handler_EGS_USE_QUICK_SLOT_REQ( int slotID );
	bool Handler_EGS_USE_QUICK_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_USE_QUICK_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );
	virtual bool MouseDown(D3DXVECTOR2 mousePos);
#ifdef NEW_ITEM_NOTICE
	virtual wstring GetSlotItemDesc();
#endif //NEW_ITEM_NOTICE

#ifdef SERV_NEW_DEFENCE_DUNGEON
	void SetSlotItemIcon( vector< CX2Slot* >& m_SlotList, const CX2Inventory& kInventory, CKTDGUIStatic* pStatic );			/// ������ ������ ���� �Լ�
	void SetSummonMonsterItemIcon( vector< CX2Slot* >& m_SlotList, const CX2Inventory& kInventory, CKTDGUIStatic* pStatic );	/// ���� ��ȯ ī�� ���� ������ ���� �Լ�

	void InputSummonCardSlotKey( const CX2Inventory& kInventory, CX2Item** pQuickItem, int& slotID );		/// ���� ��ȯ ī�� ���� Ű���� �Է� �Լ�
	void InputQuickSlotKey( const CX2Inventory& kInventory, CX2Item** pQuickItem, int& slotID );			/// ������ Ű���� �Է� �Լ�

	bool Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ( int slotID );											/// ���� ī�� ������ ��� ��û �Լ�
	bool Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ���� ī�� ������ ��� ���� �Լ�
#endif //SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
	void OnFrameMove_CoolTime( CX2Item* pItem_, CX2SlotItem* pSlotItem_ );
#endif //SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP

private:

	bool				m_bShow;
	bool				m_bEnable;
	CKTDGUIDialogType		m_pDLGQuickSlot;

	bool						m_bWaiting_EGS_USE_QUICK_SLOT_ACK;	

	// ������ // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
	// ��� ������ �۾����� int�� ItemID���� GroupID�� ������
	map < int, CKTDXTimer >		m_mapQuickSlotItemCoolTime;

#ifdef NEW_ITEM_NOTICE
	CKTDGUIDialogType		m_pDLGItemHighLight;
#endif //NEW_ITEM_NOTICE

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-03-25
	bool					m_bIsSummonCardSlot;		/// ����� �� ���� ī��� ������ ����
#endif //SERV_NEW_DEFENCE_DUNGEON
};

#endif
