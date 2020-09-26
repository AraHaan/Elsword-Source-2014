#pragma once

class CX2SlotItem;

class CX2SlotManager
{
public:

	class CX2Slot
	{
	public:

		enum SLOT_TYPE
		{
			ST_NONE,
			ST_EQUIPPED,
			ST_INVENTORY,
			ST_BANK,			//kimhc // 2009-08-04 ST_REPOSITORY -> ST_BANK�� ���� 
			ST_SHOP_BUY,
			ST_SHOP_PREVIEW,
			ST_RECIPE,
			ST_SUPPLEMENT, //������
			ST_MATERIAL,
			ST_CASH_SHOP_MAIN_NEW,
			ST_CASH_SHOP_MAIN_HOT,
			ST_CASH_SHOP_MAIN_RECOMMEND,
			ST_CASH_SHOP_SUB,
			ST_CASH_SHOP_SUB_SPECIAL,
			ST_CASH_SHOP_DEPOSIT,	//ĳ�� ������ ������ 
			ST_CASH_SHOP_CHOICED,	//��
            ST_POST_SEND,
            ST_POST_RECEIVE,
			ST_UI_WATCH,			// ������â ���� �߰� : 2008.11.19 ���¿�
			ST_PET,					// oasis907 : ����� [2010.9.2] // �� �κ��丮
			ST_RESOLVE,				// #ifdef SERV_SYNTHESIS_AVATAR
			ST_SYNTHESIS,			// #ifdef SERV_MULTI_RESOLVE
			ST_END,
		};

	public:

		CX2Slot(void);
		virtual ~CX2Slot(void);

		void SetSlotManager( CX2SlotManager* pSlotManager );
		CX2SlotManager* GetSlotManager() { return m_pSlotManager; }

		virtual void SetEnable( bool bEnable, bool bEffectItemImage = true );
		virtual void SetShow( bool bShow, bool bEffectItemImage = true );

		virtual bool GetClickable() const;
		virtual void SetClickable(bool val);

		bool GetEnable() { return m_bEnable; }

		SLOT_TYPE GetSlotType() { return m_SlotType; }
		int	GetSlotID() { return m_SlotID; }

		D3DXVECTOR2 GetPos();
		void SetPos( D3DXVECTOR2 pos );
		D3DXVECTOR2 GetSize() { return m_Size; }
#ifdef SERV_NEW_DEFENCE_DUNGEON
		void SetSize(D3DXVECTOR2 val) { m_Size = val; }
#endif //SERV_NEW_DEFENCE_DUNGEON

		virtual void SetSlotData_LUA();

		virtual bool IsInMousePoint( D3DXVECTOR2 mousePos );

		virtual	void SetDialog( CKTDGUIDialogType pDLGSlot );
		virtual CKTDGUIDialogType GetDialog() { return m_pDLGSlot; }

		virtual CKTDGUIDialogType GetDLGSlotFixBack() { return m_pDLGSlotFixBack; }

		virtual void ResetPos();
#ifdef NEW_VILLAGE_UI
		// 2008.11.13 ���¿�
		bool	IsItem();
#endif
		//{{ 09.03.18 ���¿�
		X2_DIALOG_LAYER			GetNormalLayer(){ return m_NormalLayer; }
		void					SetNormalLayer( X2_DIALOG_LAYER layer ){ m_NormalLayer = layer; }

		//}}

	protected:

		virtual void LuaGetValue( KLuaManager& luaManager );
		//virtual void LuaTinkerCall();

		int						m_SlotID;

		CX2SlotManager*			m_pSlotManager;

		bool					m_bEnable;
		bool					m_bShow;
		bool					m_bClickable;

		SLOT_TYPE				m_SlotType;
		D3DXVECTOR2				m_Pos;
		D3DXVECTOR2				m_Size;

		CKTDGUIDialogType			m_pDLGSlotFixBack;
		CKTDGUIDialogType			m_pDLGSlot;

		//{{ 09.03.18 ���¿�
		X2_DIALOG_LAYER			m_NormalLayer;
		//}}
	};

public:

	CX2SlotManager( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2SlotManager(void);

	virtual void SetSlotManagerLayer( int slotManagerLayer );
	virtual int	GetSlotManagerLayer() { return m_SlotManagerLayer; }
	// 2008.12.16 ���¿� : ��ƿ��Ƽ �Լ� �߰�. 
	virtual void SetSlotManagerChangeSequence( bool bFront ); // �ش� ���̾�� ���� �ڷ� ������ ������ ������
	////

	//////////////////////////////////////////////////////////////////////////
	//��ӹ��� Ŭ�������� �� �ҷ�����ϴ� �Լ���

	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//////////////////////////////////////////////////////////////////////////
	

	//kimhc // 2009-08-04 // ��ƿ��� slot ���� �� ���� �ִ� Dummy ��
	void	SetDummyValue( int iDummyValue ) { m_iDummyValue = iDummyValue; }
	int		GetDummyValue_LUA() const { return m_iDummyValue; }
	virtual void AddSlot( CX2Slot* pSlot );
	
	CX2SlotItem* CreateItemSlot();

	virtual void SetEnable( CX2Slot::SLOT_TYPE slotType, bool bEnable );
	virtual void SetShow( CX2Slot::SLOT_TYPE slottype, bool bShow );

	virtual void SetEnable( bool bEnable );
	virtual void SetShow( bool bShow );

	virtual void SetPos( const D3DXVECTOR2& pos ) { m_Pos = pos; }
	virtual D3DXVECTOR2 GetPos() { return m_Pos; }

	virtual CX2Slot* GetSlot( int index );
	virtual CX2Slot* GetSlot( CX2Slot::SLOT_TYPE slotType, int slotID );
	virtual int GetSlotMaxNum() { return (int)m_SlotList.size(); }
#ifdef NEW_VILLAGE_UI
	static CX2Slot* GetSlotBeforeDragging() { return *m_pSlotBeforeDragging; }
	bool GetIsNowDraggingItem() { return (*m_pSlotBeforeDragging != NULL); }
#else
	static CX2Slot* GetSlotBeforeDragging() { return m_pSlotBeforeDragging; }
	bool GetIsNowDraggingItem() { return (m_pSlotBeforeDragging != NULL); }
#endif


	virtual void SetNowDragItemReturn();

	virtual wstring GetSlotItemDesc() = NULL;
	virtual void DrawSlotDesc() = NULL;

	void SetNowOverItemSlotAndDesc( CX2SlotItem* pSlotItem ) 
	{ 
		KTDXPROFILE();

		m_pNowOverItemSlot = pSlotItem;
		
		if( NULL != pSlotItem )
		{
			DrawSlotDesc();
		}
		else
		{
			if ( m_pDLGItemDesc != NULL )
				m_pDLGItemDesc->SetShow( false );

			if ( m_pDLGItemDescCompare != NULL )
				m_pDLGItemDescCompare->SetShow( false );

#ifdef FIX_TOOLTIP
			if ( m_pDLGItemDescAdd != NULL )
				m_pDLGItemDescAdd->SetShow( false );

			if ( m_pDLGItemDescCompareAdd != NULL )
				m_pDLGItemDescCompareAdd->SetShow( false );
#endif FIX_TOOLTIP

		}
	}
	
	//{{ mauntain : ����ȯ [2012.07.16] �κ��丮 �� ������ ����� ������ ���� ���� ����
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int GetUsedSlotMaxNum();	/// ���� ��� ������ ������ ����
	int GetUsedSlotNum();		/// ���� ������� ������ ����
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ

	CX2SlotItem* GetSlotByItemUID( IN UidType uidItemUID_ );		/// ItemUID�� ���� SlotItem ��ü�� ��ȯ�ϴ� �Լ�

#endif //SERV_UPGRADE_TRADE_SYSTEM

protected:

	//////////////////////////////////////////////////////////////////////////
	//Ŭ���� �̸��� �°� ������ ���̹����� �������̵��ؼ� ����ϼ���.
	virtual void RegisterLuaBind();
	//////////////////////////////////////////////////////////////////////////
	

	virtual bool MouseDown(  D3DXVECTOR2 mousePos );
	virtual bool MouseUp(  D3DXVECTOR2 mousePos );
	virtual void MouseMove( D3DXVECTOR2 mousePos );

	virtual CX2Slot* GetSlotInMousePos( D3DXVECTOR2 mousePos );

	bool OpenScriptFile( const WCHAR* pFilename );

protected:

	CX2State*				m_pNowState;

	vector< CX2Slot* >		m_SlotList;

	D3DXVECTOR2				m_Pos;
#ifdef NEW_VILLAGE_UI
	//2008.11.13 ���¿� : Global ���ϵ��� �����մϴ�
	static CX2Slot**		m_pSlotBeforeDragging;	
#else
	static CX2Slot*			m_pSlotBeforeDragging;	
#endif
	CX2SlotItem*			m_pNowOverItemSlot;

	int						m_SlotManagerLayer;
	
	int						m_iDummyValue;				//kimhc // 2009-08-04 // ��ƿ��� slot ���� �� ���� �ִ� Dummy ��


	CKTDGUIDialogType		m_pDLGItemDesc;
	CKTDGUIDialogType		m_pDLGItemDescCompare;

#ifdef FIX_TOOLTIP
	CKTDGUIDialogType		m_pDLGItemDescAdd;
	CKTDGUIDialogType		m_pDLGItemDescCompareAdd;
#endif FIX_TOOLTIP
};
