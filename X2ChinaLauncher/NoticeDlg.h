#pragma once


// CNoticeDlg ��ȭ �����Դϴ�.

class CNoticeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoticeDlg)

public:
	CNoticeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNoticeDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NOTICE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
