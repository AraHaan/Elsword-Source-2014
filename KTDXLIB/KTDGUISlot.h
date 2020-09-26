#pragma once
#ifdef NEW_SKILL_TREE_UI

//Ex,.
namespace _CONST_SLOT_ 
{
	const int g_iMaxData = 100;
}

class CKTDGUISlot;
class CKTDGUISlotData		// Slot ���ο� ��� Data. ��ӹ޾Ƽ� ����.
{
public:
	enum SLOT_DATA_TYPE
	{
		SDT_NONE,
		SDT_SKILL_TREE,
		SDT_SKILL_EQUIP,
		SDT_USE_KEY,
		SDT_DISUSE_KEY,
		SDT_END,
	};

	virtual void UpdateUI( CKTDGUISlot* pSlot ) = 0;

	bool m_bDataUpdated;
	SLOT_DATA_TYPE	m_eSlotDataType;
//	CKTDGUISlot* m_pSlot;

	CKTDGUISlotData()
	{
		m_bDataUpdated = false;
		m_eSlotDataType = SDT_NONE;
//		m_pSlot = NULL;
	}
	virtual	~CKTDGUISlotData(){};
};

class CKTDGUISlot : public CKTDGUIControl
{
	friend class CKTDGUIDialogManager;
	friend class CKTDGUIDialog;
public:

	
	enum SLOT_CONTROL_STATE
	{
		SCS_NORMAL,
		SCS_DOWN,
		SCS_MOUSEOVER,
		SCS_ONDRAG,
		SCS_DISABLE,

		SCS_END,
	};

// 	enum SCS_EVENT_MSG
// 	{
// 		SCS_BUTTON_MOUSEOVER = 0,
// 		SCS_BUTTON_MOUSEDOWN,
// 		SCS_BUTTON_MOUSEUP,
// 	};



public:
	CKTDGUISlot();
	virtual	~CKTDGUISlot(void);
	
	virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual	HRESULT	OnFrameRenderEnd();

	HRESULT	CKTDGUISlot::RenderDragSlot();

	virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	//virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void LButtonMouseUp();

	virtual void	SetEnable( bool bEnable );
	virtual void	SetShow( bool bShow );
	void SetDragable( bool bDragable ){ m_bDragable = bDragable; }
	bool GetDragable(){ return m_bDragable; }
	D3DXVECTOR2 GetSize();
	virtual D3DXVECTOR2 GetPos() override;

	virtual D3DXVECTOR2 GetGuideDescPos();

	virtual	bool	ContainsPoint( POINT pt );
		
	virtual void	OnFocusOut();
	virtual void	ChangeState( SLOT_CONTROL_STATE state, bool bForce = false );
	SLOT_CONTROL_STATE GetState(){ return m_eControlState; }


// 	void SetEtcPicture( CKTDGUIControl::CPictureData* pPicture ){ SAFE_DELETE(m_pEtcPicture); m_pEtcPicture = pPicture; }
// 	void SetEtcTex( const WCHAR* pFileName, const WCHAR* key );
// 	void SetEtcTex( const WCHAR* pFileName );

/*	void SetColorFactor(D3DXCOLOR cColor);*/

	static void ReturnDraggingItem();
	
	void SetDraggingSlotColor(D3DXCOLOR cColor){ m_cDraggingSlotColor = cColor; }
	void SetDraggedSlotColor(D3DXCOLOR cColor){ m_cDraggedSlotColor = cColor; }
	D3DXCOLOR GetDraggingSlotColor(){ return m_cDraggingSlotColor; }
	D3DXCOLOR GetDraggedSlotColor(){ return m_cDraggedSlotColor; }

	CKTDGUISlot* GetDroppedSlot(){ return m_pDroppedSlot; }
	void AddPicture( int key, CKTDGUIControl::CPictureData* pPicture ){ m_mapAdditionalPictureDataList.insert(std::make_pair(key, pPicture)); }	
	CKTDGUIControl::CPictureData* GetPicture( int index );
	void RemoveAllPicture();
	void DrawEdge( bool bDrawOut, int edgeWidth, D3DXCOLOR edgeColor, CKTDGUIControl::UIPointData* pPoint );

	void SetCustomMsgMouseOut( int msg ) { m_CustomMsgMouseOut = msg; }
	void SetCustomMsgMouseOver( int msg ){ m_CustomMsgMouseOver = msg; }
	void SetCustomMsgMouseDown( int msg ){ m_CustomMsgMouseDown = msg; }
	void SetCustomMsgMouseUp( int msg ){ m_CustomMsgMouseUp = msg; }
	void SetCustomMsgMouseRightUp( int msg ){ m_CustomMsgMouseRightUp = msg; }
	void SetCustomMsgDropped( int msg ){ m_CustomMsgDropped = msg; }

	void SetItemTex( const WCHAR* pFileName );
	void SetItemTex( const WCHAR* pFileName, const WCHAR* key );
	void SetItemTex_LUA( const char* pFileName, const char* key );
	void SetItemTex_LUA2( const char* pFileName );
	void SetMouseOverTex( const WCHAR* pFileName, const WCHAR* key );
	void SetMouseOverTex_LUA( const char* pFileName, const char* key  );
	
	void SetString( CKTDGUIControl::UIStringData* pString ){ SAFE_DELETE(m_pStringData); m_pStringData = pString; }
	CKTDGUIControl::UIStringData* GetString(){ return m_pStringData; }
	void SetString_LUA();

	// �̰� �ʼ���..
	void SetPoint( CKTDGUIControl::UIPointData* pUIPointData );
	void SetPoint_LUA();
	void SetMouseOverPoint( CKTDGUIControl::UIPointData* pUIPointData );
	void SetMouseOverPoint_LUA();
	
	void SetAllPointDatabyNormalPoint();

	// ������ ����..
	CKTDGUISlotData* GetSlotData(){ return m_pSlotData; }
	void SetSlotData( CKTDGUISlotData* pSlotData );
	void SwapSlotData( CKTDGUISlot* pSlot );			// ���� �� ���� �����͸� �ٲ۴�

	void SetID(int id){ m_iID = id; }
	int GetID(){ return m_iID; }

protected:
		
	//void ShowGuideDesc( bool bOpen );
	void UpdateDraggingVertex( VERTEX_UI& vertexLT, VERTEX_UI& vertexRT, VERTEX_UI& vertexLB, VERTEX_UI& vertexRB, const UIPointData& pointData  );

#ifdef DLL_BUILD
	virtual bool IsSelectByEditGui( POINT pt ) override
	{
		return ContainsPoint(pt);
	}

	virtual void MoveControl( float fx, float fy ) override;
	virtual void MoveSubControl( float fx, float fy, wstring subControlName ) override;

	virtual void SetEditGUI( bool bEdit ) override;		// GUI ����Ʈ ��� ����

	virtual void ShowSubView( wstring name, bool bView ) override;
	virtual vector<D3DXVECTOR2> GetPosList() override;		// ��Ʈ�� ���� pictures�� ��ġ ����
	virtual D3DXVECTOR2 GetPos(wstring name) override;

	void DrawEditEdge();		// UITool���� ���������� ���ȴ�.

protected:
	bool m_bEditEdge;
	CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif


private:
	CKTDGUIControl::UITextureData*	m_pItemTex;				// ������ �׸�
	//CKTDGUIControl::UITextureData*	m_pDraggingItemTex;		// �巡�� ���� ������ �׸�
 	CKTDGUIControl::UITextureData*	m_pMouseOverTex;		// ���콺 �ø��� ǥ�õ� �׸�

	CKTDGUIControl::UIStringData*		m_pStringData;

	CKTDGUIControl::UIPointData*		m_pNormalPoint;
	CKTDGUIControl::UIPointData*		m_pMouseOverPoint;
	CKTDGUIControl::UIPointData*		m_pDraggingPoint;
	
	CKTDGUIControl::VERTEX_UI			m_Vertex[4];
	CKTDGUIControl::VERTEX_UI			m_DragVertex[4];

	static CKTDGUISlot*				s_pDragSlot;					// static. ���� �巡�� �ǰ� �ִ� ����

	bool							m_bDragable;
	D3DXCOLOR						m_cDraggingSlotColor;			// �巡�� ���� �� ���콺 ���� �ߴ� �׸��� ����
	D3DXCOLOR						m_cDraggedSlotColor;			// �巡�� ���� �� ���� ��ġ�� ���� �׸��� ����
	D3DXCOLOR						m_cNormalSlotColor;				// ���붧 ���� �÷�
	D3DXCOLOR						m_cMouseOverColor;				// ���콺 ���� �׸��� �÷�

	std::map<int, CKTDGUIControl::CPictureData*>			m_mapAdditionalPictureDataList;		// �߰������� ����ִ� Picture. �������� �뵵�� �׸��� ���� ���� �� ���� ����ؼ� map���� ���� key-picture�� ����� �� �ְ� ��.


	CKTDGUISlot*					m_pDroppedSlot;					// �ڽſ��� Drop�� Slot. Drop�� ���ÿ� s_pDragSlot�� NULL�� ����� ������ �ʿ�. �ش� �޽����� �޾��� �� �ܿ��� �������� �ʵ��� ����!
	SLOT_CONTROL_STATE				m_eControlState;

	CKTDXDeviceSound*					m_pSndMouseOver;
	CKTDXDeviceSound*					m_pSndMouseUp;
	CKTDXDeviceSound*					m_pSndMouseDown;

	int									m_CustomMsgMouseOut;
	int									m_CustomMsgMouseOver;
	int									m_CustomMsgMouseDown;
	int									m_CustomMsgMouseUp;
	int									m_CustomMsgMouseRightUp;
	int									m_CustomMsgDropped;				// �ٸ� � Slot�� �� �Ӹ����� Drop �Ǿ��� �� ���� �޽���
	//int								m_CustomMsgMouseDblClk;

	bool								m_bMouseOut;	// ���콺�� over�� ���¿��� normal ���·� �ٲ����

	CKTDGUISlotData*					m_pSlotData;
	int									m_iID;

	CKTDXDeviceTexture*					m_pEdgeTexture;

};

#endif
