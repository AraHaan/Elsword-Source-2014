// KEsdRemoteController_ServerDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CKEsdRemoteController_ServerDlg ��ȭ ����
class CKEsdRemoteController_ServerDlg : public CDialog
{
// ����
public:
	CKEsdRemoteController_ServerDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_KESDREMOTECONTROLLER_SERVER_DIALOG };

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
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonPatch();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonLaunch();
	afx_msg void OnBnClickedButtonShutdown();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLaunchDbg();
	afx_msg void OnBnClickedButtonLaunchCo();
	afx_msg void OnBnClickedButtonLaunchCoDbg();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLoginDbg();
	CButton m_checkSMS;
	afx_msg void OnBnClickedSmsCheck();
	afx_msg void OnBnClickedButtonChannel();
	afx_msg void OnBnClickedButtonIdpw();
	afx_msg void OnBnClickedButtonHsbUpdate();
	afx_msg void OnBnClickedButtonSvnUpdate();
	afx_msg void OnBnClickedButtonWindowsUpdate();
	afx_msg void OnBnClickedButtonVirusCheck();
	afx_msg void OnBnClickedUiLock();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
