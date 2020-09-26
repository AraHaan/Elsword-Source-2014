#pragma once

#include "KBrowserWrapper.h"
#include "BannerDlg.h"
#include "NoticeDlg.h"

// CMainDlg ��ȭ �����Դϴ�.
class CButtonST;

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();

	void NotifyStartButtonEnable(BOOL bEnable);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);

	//�ּ�ȭ ��ư
	afx_msg void OnMinimizeButtonClicked();
	afx_msg void OnCloseButtonClicked();

	//���� ��ư

	//�޴� ��ư
	afx_msg void OnHomeButtonClicked();
	afx_msg void OnGuideButtonClicked();
	afx_msg void OnPaidButtonClicked();
	afx_msg void OnSupportButtonClicked();
	afx_msg void OnBBSButtonClicked();

	//���� ��ư
	afx_msg void OnAccountButtonClicked();
	//���� ��ư
	afx_msg void OnStartButtonClicked();

	//������ġ ��ư
	afx_msg void OnManualPatchButtonClicked();

	LRESULT OnLoadServerList(WPARAM wParam, LPARAM lParam);

private:
	//�ּ�ȭ ��ư
	CButtonST* m_pMinimizeButton;
	//�ݱ� ��ư
	CButtonST* m_pCloseButton;


	//�޴� ��ư
	CButtonST* m_pHomeButton;
	CButtonST* m_pGuideButton;
	CButtonST* m_pPaidButton;
	CButtonST* m_pSupportButton;
	CButtonST* m_pBBSButton;

	//���� ��ư
	CButtonST* m_pAccountButton;
	//���� ��ư
	CButtonST* m_pStartButton;

	//��ũ�� �� ��ư
	//CBitmapButton m_ScrollBarButton;

	//������ġ ��ư
	CButtonST* m_pManualPatchButton;

	//��ġ ���� bitmap
	CBitmap m_PatchCurProgressBG;
	CBitmap m_PatchCurProgressBitmap;
	CBitmap m_PatchAllProgressBG;
	CBitmap m_PatchAllProgressBitmap;

	//
	KBrowserWrapper m_browser; ///< HTML file browser
	//KBrowserWrapper m_NoticeBrowser; ///< HTML file browser

	CBannerDlg m_BannerDlg;
	//CNoticeDlg m_NoticeDlg;

	CRect m_WindowRect;
	BOOL m_bMinimize;
public:
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

private:
	void OnPaintPatchCurProgress(CPaintDC& dc);
	void OnPaintPatchAllProgress(CPaintDC& dc);

	bool SetGameGuardPatchServer(int iLine);
};
