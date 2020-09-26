// DBConnectionDlg.cpp : ���� �����Դϴ�.
//
#include "stdafx.h"
#include "KEsdRemoteController_Server.h"
#include "DBConnectionDlg.h"
#include ".\dbconnectiondlg.h"


// DBConnectionDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DBConnectionDlg, CDialog)
DBConnectionDlg::DBConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DBConnectionDlg::IDD, pParent)
{
}

DBConnectionDlg::~DBConnectionDlg()
{
}

void DBConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_IDEditBox);
	DDX_Control(pDX, IDC_EDIT2, m_PWEditBox);
}


BEGIN_MESSAGE_MAP(DBConnectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// DBConnectionDlg �޽��� ó�����Դϴ�.

void DBConnectionDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( m_pVecDBConnIDPW == NULL )
	{
		::MessageBoxW( NULL, L"DB Connection String Option Error!", L"", 0 );
		OnOK();
		return;
	}

	CString cstrBuffer;
	m_IDEditBox.GetWindowText( cstrBuffer );
	m_pVecDBConnIDPW->push_back( KClearString( cstrBuffer.GetBuffer() ) );

	m_PWEditBox.GetWindowText( cstrBuffer );
	m_pVecDBConnIDPW->push_back( KClearString( cstrBuffer.GetBuffer() ) );

	// comment : this is fake!
	m_IDEditBox.GetWindowText( cstrBuffer );

	OnOK();
}
