#pragma once


// CMainDlg ��ȭ �����Դϴ�.

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


protected:
	CGridCtrl		m_GridCtrl;
	CRect           m_rcGrid;
public:
	afx_msg void OnDestroy();

	void OpenLuaFile( const WCHAR* wszFileName );
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ResizeGridCtrl( int cx, int cy );
};
