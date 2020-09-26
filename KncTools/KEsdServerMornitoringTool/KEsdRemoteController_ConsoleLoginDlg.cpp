// KEsdRemoteController_ConsoleLoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KEsdRemoteController_ConsoleLoginDlg.h"
#include ".\kesdremotecontroller_consolelogindlg.h"
#include "CreateDBConnFileDlg.h"


// KEsdRemoteController_ConsoleLoginDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CKEsdRemoteController_ConsoleLoginDlg, CDialog)
CKEsdRemoteController_ConsoleLoginDlg::CKEsdRemoteController_ConsoleLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKEsdRemoteController_ConsoleLoginDlg::IDD, pParent)
{
}

CKEsdRemoteController_ConsoleLoginDlg::~CKEsdRemoteController_ConsoleLoginDlg()
{
}

void CKEsdRemoteController_ConsoleLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_EDIT, m_IDEdit);
	DDX_Control(pDX, IDC_PW_EDIT, m_passwordEdit);
	DDX_Control(pDX, IDC_AUTO_LOGIN, m_SaveLoginInfo);
}

BOOL CKEsdRemoteController_ConsoleLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectory( MAX_PATH, buff );
	std::wstring strIniFilePath = buff;
	strIniFilePath += L"\\";
	strIniFilePath += L"config.ini";

	::GetPrivateProfileString( L"MornitoringTool", L"CHECK", 0, buff, MAX_PATH, strIniFilePath.c_str() );
	if( CString( buff ) == L"TRUE" )
	{
		m_SaveLoginInfo.SetCheck( TRUE );
	}
	
	::GetPrivateProfileString( L"MornitoringTool", L"ID", 0, buff, MAX_PATH, strIniFilePath.c_str() );
	m_IDEdit.SetWindowText( CString( buff ) );
	::GetPrivateProfileString( L"MornitoringTool", L"PW", 0, buff, MAX_PATH, strIniFilePath.c_str() );
	m_passwordEdit.SetWindowText( CString( buff ) );

	return FALSE;
}


BEGIN_MESSAGE_MAP(CKEsdRemoteController_ConsoleLoginDlg, CDialog)	
	ON_BN_CLICKED(IDC_LOGIN, OnBnClickedLogin)
	ON_BN_CLICKED(IDCREATEDBCONN, OnBnClickedCreatedbconn)
END_MESSAGE_MAP()


// KEsdRemoteController_ConsoleLoginDlg �޽��� ó�����Դϴ�.
void CKEsdRemoteController_ConsoleLoginDlg::OnOK()
{
	if (!UpdateData(TRUE))
	{
		TRACE(traceAppMsg, 0, "UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	ASSERT(::IsWindow(m_hWnd));

	if (m_nFlags & (WF_MODALLOOP|WF_CONTINUEMODAL))
		EndModalLoop(IDD_LOGIN);
}

void CKEsdRemoteController_ConsoleLoginDlg::OnBnClickedLogin()
{
	CString id, pw, message;

	m_IDEdit.GetWindowText( id );
	m_passwordEdit.GetWindowText( pw );

	if( id.GetLength() > ID_SIZE )
	{
		message.Format( L"ID���̰� %d�� �̻� �Ѿ �� �����ϴ�!", ID_SIZE );
		MessageBox( message.GetBuffer(), L"�˸�", MB_OK );
		return;
	}

	if( pw.GetLength() > PW_SIZE )
	{
		message.Format( L"��ȣ���̰� %d�� �̻� �Ѿ �� �����ϴ�!", PW_SIZE );
		MessageBox( message.GetBuffer(), L"�˸�", MB_OK );
		return;
	}

	if( id.IsEmpty() )
	{
		MessageBox( L"ID�� �Է��ϼ���!", L"�˸�", MB_OK );
		return;
	}
	else if( pw.IsEmpty() )
	{
		MessageBox( L"Password�� �Է��ϼ���!", L"�˸�", MB_OK );
		return;
	}

	if( theApp.m_kImp.DoQueryLogin( id.GetBuffer(), pw.GetBuffer() ) != -1 )
	{		
		OnOK();

		//////////////////////////////////////////////////////////////////////////
		// config.ini ���
		
		wchar_t buff[MAX_PATH] = {0};
		::GetCurrentDirectory( MAX_PATH, buff );
		std::wstring strIniFilePath = buff;
		strIniFilePath += L"\\";
		strIniFilePath += L"config.ini";

		CString strTemp;
		if( m_SaveLoginInfo.GetCheck() == TRUE )
		{
			m_IDEdit.GetWindowText( strTemp );
			::WritePrivateProfileString( L"MornitoringTool", L"CHECK", L"TRUE", strIniFilePath.c_str() );

			m_IDEdit.GetWindowText( strTemp );
			::WritePrivateProfileString( L"MornitoringTool", L"ID", strTemp.GetBuffer(), strIniFilePath.c_str() );

			m_passwordEdit.GetWindowText( strTemp );
			::WritePrivateProfileString( L"MornitoringTool", L"PW", strTemp.GetBuffer(), strIniFilePath.c_str() );
		}
		else
		{
			m_IDEdit.GetWindowText( strTemp );
			::WritePrivateProfileString( L"MornitoringTool", L"CHECK", L"FALSE", strIniFilePath.c_str() );

			m_IDEdit.GetWindowText( strTemp );
			::WritePrivateProfileString( L"MornitoringTool", L"ID", L"", strIniFilePath.c_str() );

			m_passwordEdit.GetWindowText( strTemp );
			::WritePrivateProfileString( L"MornitoringTool", L"PW", L"", strIniFilePath.c_str() );
		}
	}
	else
	{
		MessageBox( L"�α����� �����Ͽ����ϴ�!", L"�˸�", MB_OK );
		return;
	}
}
void CKEsdRemoteController_ConsoleLoginDlg::OnBnClickedCreatedbconn()
{	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CCreateDBConnFileDlg kCreateDBConnFileDlg;
	kCreateDBConnFileDlg.DoModal();
}
