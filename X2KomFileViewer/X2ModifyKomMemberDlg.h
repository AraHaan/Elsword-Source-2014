#pragma once
#include "afxwin.h"
#include "afxcmn.h"


struct ModifyData
{
	std::wstring m_wstrKomString;
	std::wstring m_wstrFileFullPath;
};
// CX2ModifyKomMemberDlg ��ȭ �����Դϴ�.

class CX2ModifyKomMemberDlg : public CDialog
{
	DECLARE_DYNAMIC(CX2ModifyKomMemberDlg)

public:
	CX2ModifyKomMemberDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CX2ModifyKomMemberDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MODIFY_KOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );
	afx_msg void OnBnClickedFindsavepath();
	afx_msg void OnBnClickedFindmodifyfile();
	afx_msg void OnBnClickedModifybegin();
	void InitListControl();

private:
	std::wstring m_wstrKomSavePath;
	std::map< std::wstring, ModifyData > m_mapModifyData;	// �����ϱ�� ������ ���ҽ�

public:
	CEdit m_cSaveDirEdit;
	//CEdit m_cModifyFileEdit;
	CListCtrl m_cModifyListBox;
	afx_msg void OnBnClickedDelmodifyfile();
	CButton m_cLuaCompileCheck;
	CButton m_cScriptEncryptCheck;
};
