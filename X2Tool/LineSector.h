#pragma once


// CLineSector ��ȭ �����Դϴ�.

class CLineSector : public CDialogEx
{
	DECLARE_DYNAMIC(CLineSector)

public:
	CLineSector(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLineSector();

	afx_msg BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditSectorWidth();
	afx_msg void OnEnChangeEditSectorHeight();
	afx_msg void OnEnChangeEditSectorDepth();
	afx_msg void OnBnClickedButton1();
};
