#pragma once


// CToolStartNotice ��ȭ �����Դϴ�.

class CToolStartNotice : public CDialogEx
{
	DECLARE_DYNAMIC(CToolStartNotice)

public:
	CToolStartNotice(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CToolStartNotice();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_START_NOTICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOknotice();
};
