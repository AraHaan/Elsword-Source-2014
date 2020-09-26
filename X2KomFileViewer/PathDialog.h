#pragma once
#include "afxwin.h"


// CPathDialog ��ȭ �����Դϴ�.

class CPathDialog : public CDialog
{
	DECLARE_DYNAMIC(CPathDialog)

public:
	CPathDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOldPath();
	afx_msg void OnBnClickedNewPath();
	afx_msg void OnBnClickedOk();
	

public:
	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );
	std::wstring& GetOldPath() { return m_wstrOldPath; }
	std::wstring& GetNewPath() { return m_wstrNewPath; }
	bool GetLuaScriptCompileFlag() { return m_bDoCompile; }
	bool GetLuaScriptCryptFlag() { return m_bDoCrypt; }
	bool GetXFileConvertFlag() { return m_bDoXConvert; }

private:
	std::wstring m_wstrOldPath;		// ������ ���� ���
	std::wstring m_wstrNewPath;		// �Ź��� ���� ���
	bool m_bDoCompile;
	bool m_bDoCrypt;
	bool m_bDoXConvert;
public:
	CEdit m_oldPathEdit;
	CEdit m_newPathEdit;
	CButton m_checkCompile;
	CButton m_checkCrypt;
	CButton m_checkXConvert;
	afx_msg void OnBnClickedCompileCheck();
	afx_msg void OnBnClickedCryptCheck();
	afx_msg void OnBnClickedXFileConvertCheck();	
};
