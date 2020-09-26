// CreateDBConnFileDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "CreateDBConnFileDlg.h"
#include ".\createdbconnfiledlg.h"


// CCreateDBConnFileDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCreateDBConnFileDlg, CDialog)
CCreateDBConnFileDlg::CCreateDBConnFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateDBConnFileDlg::IDD, pParent)
{
}

CCreateDBConnFileDlg::~CCreateDBConnFileDlg()
{
}

void CCreateDBConnFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editBoxFilePath);
}


BEGIN_MESSAGE_MAP(CCreateDBConnFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateDBConnFileDlg �޽��� ó�����Դϴ�.

void CCreateDBConnFileDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString cstrFilePath;
	m_editBoxFilePath.GetWindowText( cstrFilePath );
	if( theApp.m_kImp.CreateDBConnectionFile( std::wstring( cstrFilePath.GetBuffer() ) ) )
	{
		::MessageBox( NULL, L"DB Connection File ���� ����!", L"Ȯ��!", NULL );
	}
	else
	{
		::MessageBox( NULL, L"DB Connection File ���� ����!", L"����!", NULL );
	}

	OnOK();
}
