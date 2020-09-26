#pragma once

//////////////////////////////////////////////////////////////////////////
//## Main stage
//## Desc : ������ ��ϵǾ� ���� ���� ������
//##		- ��Ʈ�� �����͸� ���� & EventScene ���������� ��� ����
//////////////////////////////////////////////////////////////////////////

class CRenderFrm : public CKTDXStage
{
public:
	CRenderFrm(void);
	virtual ~CRenderFrm(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	void	SetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType, HWND hWnd );
	HWND	GetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType );
	HWND	GetDlgItem( _EST_OBJECT_::DLG_HWND dlgType, int iDlgItem );

	CEventScriptParsing * GetEventScriptParsing(){ return &m_EventScriptParsing; }

	CX2EventScene * GetX2EventScene(){ return &m_EventScene; }

	void			SetLastPlaySceneID(WCHAR* wszID){m_wstrLastPlaySceneID = wszID;}
	std::wstring	GetLastPlaySceneID(){return m_wstrLastPlaySceneID;}

public:
	//# �� �ɼǱ��

	//## ������ ȭ�� ����Ŭ���� EventList ���� �������� ����Ʈ�� ��Ŀ�� ���߱�
	void	SetFocusPlaySceneID();

private:
	HWND	m_hWnd[_EST_OBJECT_::DH_END];
	CEventScriptParsing	m_EventScriptParsing;

	CX2EventScene	m_EventScene;

	std::wstring m_wstrLastPlaySceneID;
};
