#pragma once
#include "afxwin.h"


// DBConnectionDlg ��ȭ �����Դϴ�.

class DBConnectionDlg : public CDialog
{
	DECLARE_DYNAMIC(DBConnectionDlg)

public:
	DBConnectionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DBConnectionDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

	void SetEditList( std::vector< KClearString >* pVecDBConnIDPW ) { m_pVecDBConnIDPW = pVecDBConnIDPW; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	std::vector< KClearString >* m_pVecDBConnIDPW;
public:
	afx_msg void OnBnClickedOk();
protected:
	CEdit m_IDEditBox;
	CEdit m_PWEditBox;
};
