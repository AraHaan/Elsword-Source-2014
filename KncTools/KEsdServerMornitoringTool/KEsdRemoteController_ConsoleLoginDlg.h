#pragma once
#include "afxwin.h"


// CKEsdRemoteController_ConsoleLoginDlg ��ȭ �����Դϴ�.

class CKEsdRemoteController_ConsoleLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CKEsdRemoteController_ConsoleLoginDlg)

public:
	CKEsdRemoteController_ConsoleLoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CKEsdRemoteController_ConsoleLoginDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_IDEdit;
	CEdit m_passwordEdit;
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedLogin();	
	CButton m_SaveLoginInfo;
	afx_msg void OnBnClickedCreatedbconn();
};