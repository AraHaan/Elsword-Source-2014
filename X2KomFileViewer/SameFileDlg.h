#pragma once
#include "afxcmn.h"


// CSameFileDlg ��ȭ �����Դϴ�.

class CSameFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CSameFileDlg)

public:
	CSameFileDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSameFileDlg();
	virtual BOOL OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };
public:
	void SetSameFileList( std::map< std::wstring, std::vector< std::wstring > >& mapSameList ) { m_mapSameFileList = mapSameList; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_sameList;
	std::map< std::wstring, std::vector< std::wstring > > m_mapSameFileList;
	afx_msg void OnBnClickedOk();
};
