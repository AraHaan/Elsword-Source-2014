#pragma once

//////////////////////////////////////////////////////////////////////////
//## Main stage
//## Desc : ������ ��ϵǾ� ���� ���� ������
//##		- ��Ʈ�� �����͸� ���� & �ó����� ���������� ��� ����
//////////////////////////////////////////////////////////////////////////

enum DLG_HWND
{
	DH_RESOURCE = 0,

	DH_ACTION,
	DH_TEX_IO,
	DH_TEX_02,

	DH_ACTION_LIST,

	DH_END,
};

class CX2ScenarioToolStage : public CKTDXStage
{
public:
	CX2ScenarioToolStage(void);
	virtual ~CX2ScenarioToolStage(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	void	CreateDlg();
	// ���̾�α� �� �ش� ��Ʈ�� �ڵ� ���� �Լ���..
	void	SetDlgHwnd( DLG_HWND dlgType, HWND hWnd );
	HWND	GetDlgHwnd( DLG_HWND dlgType );
	HWND	GetDlgItem( DLG_HWND dlgType, int iDlgItem );
	void	HideActionSubDlg();
	void	ShowActionSubDlg(int iSelSubDlg);

private:

	CKTDGScenarioManager		m_cScenarioManager;

private:

	HWND	m_hWnd[DH_END];
};
