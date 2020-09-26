#pragma once
#include "afxwin.h"
#include <boost/timer.hpp>


// KOptionDlg ��ȭ �����Դϴ�.

class KOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(KOptionDlg)

public:
	KOptionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~KOptionDlg();

	void SetGraphMaxValue( int iGraphMaxValue );
	int GetGraphMaxValue() { return m_iGraphMaxValue; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:	
	CEdit m_graphMaxValueEdit;
	afx_msg void OnBnClickedOk();

private:
	int m_iGraphMaxValue;
	boost::timer m_clickTimer;
};
