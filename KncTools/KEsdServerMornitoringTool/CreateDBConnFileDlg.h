#pragma once
#include "afxwin.h"


// CCreateDBConnFileDlg ��ȭ �����Դϴ�.

class CCreateDBConnFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateDBConnFileDlg)

public:
	CCreateDBConnFileDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCreateDBConnFileDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CREATE_DBCONNFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_editBoxFilePath;
};
