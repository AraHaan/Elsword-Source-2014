// X2ChinaLauncherDlg.h : ��� ����
//

#pragma once

#include "MainDlg.h"

// CX2ChinaLauncherDlg ��ȭ ����
class CX2ChinaLauncherDlg : public CDialog
{
// ����
public:
	CX2ChinaLauncherDlg(CWnd* pParent = NULL);	// ǥ�� ������

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ��ȭ ���� ������
	enum { IDD = IDD_X2CHINALAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	HBITMAP Create32BitBitmap( HDC hDCSource, int cx, int cy );

	// ��������ų ���̾�α�
	CMainDlg m_MainDlg;

public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);


	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
//	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	afx_msg void OnDestroy();
};
