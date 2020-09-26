#pragma once
#include "afxcmn.h"


// CNpcList ��ȭ �����Դϴ�.

class CNpcList : public CDialogEx
{
	DECLARE_DYNAMIC(CNpcList)

public:
	CNpcList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNpcList();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_NPC_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void InitNpcData();

	afx_msg void OnBnClickedOk();
	CListCtrl m_listNpcData;
	vector<CX2UnitManager::NPC_UNIT_ID> m_vecNpc;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
