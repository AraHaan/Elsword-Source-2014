#pragma once

#include "afxdialogex.h"
#include "afxwin.h"

// CDungeonDataDlg ��ȭ �����Դϴ�.

class CDungeonDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CDungeonDataDlg)

public:
	CDungeonDataDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDungeonDataDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckUserCamera();
	CButton m_CheckUserCamera;
	afx_msg void OnEnChangeEditToolWidth();
	afx_msg void OnEnChangeEditToolHeight();
	afx_msg void OnBnClickedButtonChangeSize();
	CEdit m_editLineName;
	CEdit m_editWorldName;
	CEdit m_editCameraEye;
	CEdit m_editCameraLookat;
	afx_msg void OnBnClickedRadioQuality();
	CEdit m_editCameraSpeed;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheckCanplay();
	CButton m_checkPlay;
	CEdit m_editDungeonName;
	afx_msg void OnBnClickedButton1();
};
