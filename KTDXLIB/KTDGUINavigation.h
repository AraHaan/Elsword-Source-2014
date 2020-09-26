#pragma once
#include "ktdguicontrol.h"



class CKTDGUINavigation : public CKTDGUIControl
{
public:
	enum NAVIGATION_EVENT_MSG
	{
		NEM_LBUTTON_DOWN = 0,
		NEM_MOUSE_DRAGGING,
	};

public:
	CKTDGUINavigation(void);
	virtual ~CKTDGUINavigation(void);

	virtual	HRESULT	OnFrameRender();
	virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );

	virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual	bool	ContainsPoint( POINT pt );

	void FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData* point );
	void FromUIPointToRect( RECT* pRect, const CKTDGUIControl::UIPointData* pPointData );
	void BoundRectInRect( RECT* pInsideRect, const RECT& outsideRect );
	
	bool SetBGTex( const WCHAR* pFileName, const WCHAR* key );
	bool SetWindowTex( const WCHAR* pFileName, const WCHAR* key );
	
	void SetBGTex_LUA( const char* pFileName, const char* pKey );
	void SetWindowTex_LUA( const char* pFileName, const char* pKey );

	void SetBGPoint( CKTDGUIControl::UIPointData* pUIPointData );
	void SetWindowPoint( CKTDGUIControl::UIPointData* pUIPointData );

	void SetBGPoint_LUA();
	void SetWindowPoint_LUA();

	void SetCustomMsgLButtonDown(int val) { m_CustomMsgLButtonDown = val; }
	void SetCustomMsgMouseDragging(int val) { m_CustomMsgMouseDragging = val; }

	D3DXVECTOR2 GetRelativeWindowPos(); // navigation ��ü�������� ��ũ�ѵ� window ������ left top�� ��ġ�� ��� ��ǥ��, ������ lefttop, 0~1 ������ ��
	void SetRelativeWindowPos( D3DXVECTOR2 vRelativePos );
	void SetRelativeWindowSize( D3DXVECTOR2 vSize );

	bool GetDraggingWindow() { return m_bDraggingWindow; }

#ifdef DLL_BUILD
protected:
	virtual bool IsSelectByEditGui( POINT pt ) override
	{
		return ContainsPoint(pt);
	}

	virtual void MoveControl( float fx, float fy ) override;

	virtual void SetEditGUI( bool bEdit ) override;		// GUI ����Ʈ ��� ����


	virtual D3DXVECTOR2 GetPos() override;		/// ��Ʈ���� ��ġ
	virtual vector<D3DXVECTOR2> GetPosList() override;		// ��Ʈ�� ���� pictures�� ��ġ ����
	virtual D3DXVECTOR2 GetPos(wstring name) override;
#endif


	
	
private:
	CKTDGUIControl::VERTEX_UI			m_Vertex[4];
	
	CKTDGUIControl::UITextureData*		m_pBGTex;
	CKTDGUIControl::UITextureData*		m_pWindowTex;

	CKTDGUIControl::UIPointData*		m_pBGPoint;
	CKTDGUIControl::UIPointData*		m_pWindowPoint;

	RECT m_rcBG;				// ��ü navi ����
	RECT m_rcWindow;			// ��ü �������� ���� ���õ� ����

	bool m_bDraggingWindow;
	POINT m_ptLastMouse;		// last mouse position

	CKTDXDeviceSound*				m_pSndMouseDown;

	int		m_CustomMsgLButtonDown;
	int		m_CustomMsgMouseDragging;
	
	
	
};


