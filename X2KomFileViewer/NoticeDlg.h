#pragma once
#include "afxwin.h"


// CNoticeDlg ��ȭ �����Դϴ�.

class CNoticeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoticeDlg)

public:
	CNoticeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNoticeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NOTICE_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:

	void ChangeText( std::wstring& wstrText ) { m_wstrText = wstrText; }
	CStatic m_noticeStatic;	
	std::wstring m_wstrText;
};
