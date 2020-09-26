// WGameServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\DBConnectionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


IMPLEMENT_DYNAMIC(CDBConnectionDlg, CDialog)
CDBConnectionDlg::CDBConnectionDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDBConnectionDlg::IDD, pParent)
{
	m_pEditList = NULL;
	m_bCheckOK = false;
}

CDBConnectionDlg::~CDBConnectionDlg()
{
}

void CDBConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBConnectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDBConnectionDlg �޽��� ó�����Դϴ�.

void CDBConnectionDlg::OnBnClickedOk()
{
	if( m_pEditList != NULL )
	{
		// �ʱ�ȭ
		WCHAR editBuf[256];

		// ID���� �����
		ZeroMemory( editBuf, 256 );
        GetDlgItemText( IDC_EDIT1, editBuf, 256 );
		m_pEditList->push_back( KClearString< std::wstring >( editBuf ) );

		// ���������
		//memset( editBuf, 5, wcslen( editBuf ) );
		//SetDlgItemText( IDC_EDIT1, editBuf );

		// PW���� �����
		ZeroMemory( editBuf, 256 );
		GetDlgItemText( IDC_EDIT2, editBuf, 256 );
		m_pEditList->push_back( KClearString< std::wstring >( editBuf ) );

		// ���������
		//memset( editBuf, 5, wcslen( editBuf ) );
		//SetDlgItemText( IDC_EDIT2, editBuf );
	}
	else
	{
		MessageBoxW( L"m_pEditList is NULL Pointer!" );
	}

	m_bCheckOK = true;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CDBConnectionDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}