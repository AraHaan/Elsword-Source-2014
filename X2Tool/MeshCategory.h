#pragma once
#include "afxcmn.h"


// CMeshCategory ��ȭ �����Դϴ�.

class CMeshCategory : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshCategory)

public:
	CMeshCategory(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMeshCategory();

	void UpdateTree(wstring wstrPath, HTREEITEM hParent);
	void UpdateLib();
	void UpdateList(wstring wstrLib);
	void ClearSelected();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listObject;
	vector<wstring> m_vecFileName;
	bool m_bCanUseLib;
	
	CTreeCtrl m_treeLib;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditLibraryPath();
	afx_msg void OnLvnItemchangedListObject(NMHDR *pNMHDR, LRESULT *pResult);
};
