#pragma once
#include "afxcmn.h"

#include "SkinDialog.h"
#include "SkinButton.h"
#include "XListCtrl.h"

// CToolDungeonMap ��ȭ �����Դϴ�.

class CToolDungeonMap : public CDialogEx
{
	DECLARE_DYNAMIC(CToolDungeonMap)

public:
	CToolDungeonMap(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CToolDungeonMap();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	void UpdateDungeonMap();
	void OnPaintMap();
public:
	CXListCtrl m_listDungeonMap;

	afx_msg LRESULT OnEditChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnComboChange(WPARAM wParam, LPARAM lParam);
};
