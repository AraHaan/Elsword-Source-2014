#pragma once


// CPatchReadyDlg ��ȭ �����Դϴ�.

class CPatchReadyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPatchReadyDlg)

public:
	enum 
	{
		WM_CLOSE_EVENT = WM_USER+1,
	};

	CPatchReadyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPatchReadyDlg();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATCHREADY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg LRESULT OnCloseEvent(WPARAM wParam, LPARAM lParam);

private:
	CStatic m_Static1;
	CStatic m_Static2;
};
