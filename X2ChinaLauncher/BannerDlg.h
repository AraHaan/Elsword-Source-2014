#pragma once


// CBannerDlg ��ȭ �����Դϴ�.

class CBannerDlg : public CDialog
{
	DECLARE_DYNAMIC(CBannerDlg)

public:
	CBannerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBannerDlg();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BANNER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
