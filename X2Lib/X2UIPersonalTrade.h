#ifdef NEW_VILLAGE_UI

#pragma once

class CX2UIPersonalTrade : public CX2UIDragable
{
public:
	enum UI_PERSONAL_TRADE_CUSTOM_MSG
	{
		UPTCM_TRADE_INVITE_ACCEPT	= 19000,
		UPTCM_TRADE_INVITE_REFUSE,
		
		UPTCM_TRADE_ITEM_REGISTER,
		UPTCM_TRADE_ITEM_ACCEPT,
		UPTCM_TRADE_ITEM_CANCEL,
		UPTCM_TRADE_ITEM_CANCEL_OK,

		UPTCM_TRADE_ITEM_ED_CHANGED,

		UPTCM_REGISTER_QUANTITY_CONFIRM,
		UPTCM_REGISTER_QUANTITY_CANCLE,
		UPTCM_REGISTER_QUANTITY_PLUS,
		UPTCM_REGISTER_QUANTITY_MINUS,
		UPTCM_REGISTER_QUANTITY_UPDATE,

		//{{ kimhc // 2009-09-07 // Ʈ���̵� ǰ�� ���ξ����� �� �ִ� ���
		UPTCM_TRADE_SEALED_ITEM_ACCEPT,		// ���ξ����� Ʈ���̵� Ȯ��
		UPTCM_TRADE_SEALED_ITEM_CANCEL,
		//}} kimch // 2009-09-07 // Ʈ���̵� ǰ�� ���ξ����� �� �ִ� ���

		//kimhc // 2009-11-10 // ED �Է½� �߻��ϴ� MSG
		UPTCM_TRADE_ITEM_ED_CHANGING,

		// JHKang / 2010-09-14 / IMEDIT FocusIn �߻� ED ���� ����(ȭ�� ���� --> ���� ����)
#ifdef NUMBER_TO_LANGUAGE
		UPTCM_TRADE_ITEM_ED_FOCUSIN
#endif NUMBER_TO_LANGUAGE

	};

	enum TRADE_STEP
	{
		TS_BEFORE_REGISTER,
		TS_AFTER_REGISTER,
		TS_BEFORE_APPROVE,
		TS_AFTER_APPROVE,
	};

	enum PERSONAL_TRADE_SLOT_ID
	{
		PTSI_MY_SLOT_0 = 0,
		PTSI_MY_SLOT_1,
		PTSI_MY_SLOT_2,
		PTSI_MY_SLOT_3,
		PTSI_MY_SLOT_4,
		PTSI_MY_SLOT_5,
		PTSI_MY_SLOT_6,
		PTSI_MY_SLOT_7,
		PTSI_MY_SLOT_8,
		PTSI_MY_SLOT_9,
		
		PTSI_OPPONENT_SLOT_0 = 100,
		PTSI_OPPONENT_SLOT_1,
		PTSI_OPPONENT_SLOT_2,
		PTSI_OPPONENT_SLOT_3,
		PTSI_OPPONENT_SLOT_4,
		PTSI_OPPONENT_SLOT_5,
		PTSI_OPPONENT_SLOT_6,
		PTSI_OPPONENT_SLOT_7,
		PTSI_OPPONENT_SLOT_8,
		PTSI_OPPONENT_SLOT_9,
		
	};

	struct TradeInvited
	{
	public:
		CKTDGUIDialogType			m_pDLGInvited;
		UidType					m_iInviterUID;
		wstring					m_wstrInviterName;
		float					m_fTimeLeftForReplyInvite;

	public:
		TradeInvited( UidType iInviterUID, wstring& wstrInviterName, 
			float fTimeLeft, int YesMsgID, int NoMsgID, CKTDXStage* pNowStage );
		~TradeInvited();
		void OnFrameMove( double fTime, float fElapsedTime );
	};

	struct TradeInviting
	{
	public:
		CKTDGUIDialogType			m_pDLGInviting;
		wstring					m_wstrInviteeName;
		float					m_fTimeLeftForWaitingReply;

	public:
		TradeInviting( wstring& wstrInviteeName, float fTimeLeft, CKTDXStage* pNowStage );
		~TradeInviting();
		void OnFrameMove( double fTime, float fElapsedTime );
	};

	CX2UIPersonalTrade(CKTDXStage* pNowStage, const WCHAR* pFileName);
	~CX2UIPersonalTrade();

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }

	bool IsTrading() { return m_bTrading; }
	void BreakTrade();								// �ŷ��� �����Ѵ�.
	
	void SetPosition(D3DXVECTOR2 vec);
	void SetLayer(X2_DIALOG_LAYER layer);
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

	void SetOpponentNickName(wstring val);
	void OpenPersonalTradeWindow();
	void ClosePersonalTradeWindow();

	bool OnRegisterMyItem(UidType ItemUid );
	bool RegisterMyItem( UidType ItemUid, int iCount );
	bool OnRemoveItem( UidType EnrollItemUid );
	
	void ResetMyTradeSlot();
	void SetMyTradeED( int iED );

	void ResetOpponentTradeSlot();
	void SetOpponentTradeED( int iED );

// 	bool SetMyTradeItem( int ItemTid, int iCount );
// 	bool SetOpponentTradeItem( int ItemTid, int iCount );
// 	//bool SetOpponentTradeItem( KItemInfo ItemInfo );
// 
// 	CX2SlotItem* GetMyTradeSlot();
// 	CX2SlotItem* GetOpponentTradeSlot();

	bool OnDropAnyItem( D3DXVECTOR2 mousePos );
	bool OnRClickedItem( D3DXVECTOR2 mousePos );
	
	void OpenRegisgerQuantityDLG( D3DXVECTOR2 pos );
	void UpdateRegisterQuantityDLG( bool bReadIME = true );
	
	// �ŷ���û
	bool Handler_EGS_REQUEST_TRADE_REQ( UidType iUnitUID );
	bool Handler_EGS_REQUEST_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void DestoryAllInviteMessage();

#ifdef NUMBER_TO_LANGUAGE
	CX2TalkBoxManager *GetTalkBoxManager() { return m_pTalkBoxMgr; }
#endif NUMBER_TO_LANGUAGE

protected:
	void SetMyTradeButton( TRADE_STEP eTradeStep );
	void SetOpponentTradeButton( TRADE_STEP eTradeStep );
	void SetTradeButtonRegisterState( const bool bTempRegistered, const bool bTempOpponentRegistered );

	// ��û�� �Խ��ϴ�
	bool Handler_EGS_REQUEST_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	// ��û�� ����
	bool Handler_EGS_REQUEST_TRADE_REPLY_NOT( UidType iUnitUID, bool bAccept );
	// ���������� ^^;
	bool Handler_EGS_REQUEST_TRADE_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_CREATE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BREAK_TRADE_REQ( int iReason );
	bool Handler_EGS_BREAK_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BREAK_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_TRADE_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_JOIN_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UPDATE_TRADE_ITEM_REQ( UidType iItemUID, int iQuantity, bool bDelete );
	bool Handler_EGS_UPDATE_TRADE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UPDATE_TRADE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UPDATE_TRADE_ED_REQ( int iED );
	bool Handler_EGS_UPDATE_TRADE_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UPDATE_TRADE_ED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_OFFER_TRADE_REQ();
	bool Handler_EGS_OFFER_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_OFFER_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_APPROVE_TRADE_REQ();
	bool Handler_EGS_APPROVE_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_APPROVE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_TRADE_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ kimhc // 2009-09-07 // Ʈ���̵� ǰ�� �߿� ���� �������� �ִ� ��츦 üũ
#ifdef	SEAL_ITEM
	bool CheckIsSealedItemInTradeSlot();
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-07 // Ʈ���̵� ǰ�� �߿� ���� �������� �ִ� ��츦 üũ
		
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

	//{{ JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)
#ifdef NUMBER_TO_LANGUAGE
	void PopPersonalTradeTalkBox( UidType iUnitUID, int iED, D3DXVECTOR2 vPos, D3DXCOLOR color = 0xff000000, D3DXCOLOR outcolor = (DWORD)0x00000000, D3DXCOLOR backColor = (DWORD)0xffffffff );
	//}} JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)
#endif NUMBER_TO_LANGUAGE

private:

	CKTDGUIDialogType				m_pDLGPersonalTrade;
	CKTDGUIDialogType				m_pDLGItemEnroll;
	CKTDGUIDialogType				m_pDLGMsgBox;

	//{{ JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)
#ifdef NUMBER_TO_LANGUAGE
	CX2TalkBoxManager*				m_pTalkBoxMgr;
	const int						m_iChangingEDLimit;		//< 9  (��)
	const int						m_iChangedEDLimit;		//< 11 (�ڸ��� ���� �޸� ����)
#endif NUMBER_TO_LANGUAGE
	//}} JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)

	//{{ kimhc // 2009-09-07 // ���ε� �������� �ŷ� �ϰڳİ� ���� ���̾�α�
#ifdef	SEAL_ITEM
	CKTDGUIDialogType				m_pDLGTradeSealedItem;
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-07 // ���ε� �������� �ŷ� �ϰڳİ� ���� ���̾�α�
	
	bool						m_bShow;
	bool						m_bTrading;
	D3DXVECTOR2					m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;
#ifdef ADJUST_THIRD_PERSON_BUG
	bool						m_bTradeRequesting;	//�ŷ��� ��û�� ���� Ȯ���ϴ� ����.
#endif //ADJUST_PERSONAL_TRADE_BUG
	// �ŷ���û
	vector<TradeInvited*>	m_vecTradeInvited;
	TradeInviting*			m_pTradeInviting;

	// �ŷ�����
	UidType					m_iTradeUID;				// �ŷ����̶�� �ŷ� UID
	UidType					m_iOpponentUID; 			// �ŷ����̶�� ��� UID
	wstring					m_wstrOpponentNickName;

	bool					m_bOpponentRegisteredTrade;
	bool					m_bRegisteredTrade;
	bool					m_bOpponentApprovedTrade;

	TRADE_STEP				m_eNowMyTradeButtonStep;
	TRADE_STEP				m_eNowOpponentTradeButtonStep;

	// ��� �õ�
	CX2SlotItem*			m_pTargetDragItemSlot; 
	int						m_iItemEnrollNum;
	UidType					m_EnrollItemUid;
	

	// ������
	vector< std::pair<UidType, int> >	m_vMyItemSlot;
	vector< std::pair<UidType, KItemInfo> >	m_vOpponentItemSlot;
	int						m_iMyED;
	int						m_iOpponentED;

};

#endif

