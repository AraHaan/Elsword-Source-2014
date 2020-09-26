// KOptionDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KOptionDlg.h"
#include ".\koptiondlg.h"


// KOptionDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(KOptionDlg, CDialog)
KOptionDlg::KOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KOptionDlg::IDD, pParent)
{
}

KOptionDlg::~KOptionDlg()
{
}

void KOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_graphMaxValueEdit);
}

BOOL KOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString temp;
	temp.Format( L"%d", m_iGraphMaxValue );
	m_graphMaxValueEdit.SetWindowText( temp );

	return TRUE;
}


BEGIN_MESSAGE_MAP(KOptionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// KOptionDlg �޽��� ó�����Դϴ�.

void KOptionDlg::OnBnClickedOk()
{
	if( m_clickTimer.elapsed() < 3 /*��*/ )
		return;

	CString temp;
	m_graphMaxValueEdit.GetWindowText( temp );
	m_iGraphMaxValue = _wtoi( temp.GetBuffer() );	
	OnOK();

	m_clickTimer.restart();
}

void KOptionDlg::SetGraphMaxValue( int iGraphMaxValue )
{
	m_iGraphMaxValue = iGraphMaxValue;	
}