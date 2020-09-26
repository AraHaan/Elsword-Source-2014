// WGameServerDlg.h : header file
//

#pragma once

#include "afxwin.h"

#include <vector>
#include "SecurityString.h"

// DlgTest ��ȭ �����Դϴ�.

class CDBConnectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBConnectionDlg)

public:
	CDBConnectionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDBConnectionDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DBCONNDLG };

	void SetEditList( std::vector< KClearString< std::wstring > >* pEditList ) { m_pEditList = pEditList; }
	std::vector< KClearString< std::wstring > >* m_pEditList;

	bool IsOK() { return m_bCheckOK; }
	bool m_bCheckOK;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
