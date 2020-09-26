// KEsdRemoteController_ServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "KEsdRemoteController_Server.h"
#include "KEsdRemoteController_ServerDlg.h"
#include ".\kesdremotecontroller_serverdlg.h"
#include "DBConnectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

lua_State* g_pLua	= NULL;


// CKEsdRemoteController_ServerDlg ��ȭ ����



CKEsdRemoteController_ServerDlg::CKEsdRemoteController_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKEsdRemoteController_ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKEsdRemoteController_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SMS_CHECK, m_checkSMS);
}

BEGIN_MESSAGE_MAP(CKEsdRemoteController_ServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_PATCH, OnBnClickedButtonPatch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH, OnBnClickedButtonLaunch)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnBnClickedButtonShutdown)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_DBG, OnBnClickedButtonLaunchDbg)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO, OnBnClickedButtonLaunchCo)
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO_DBG, OnBnClickedButtonLaunchCoDbg)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_DBG, OnBnClickedButtonLoginDbg)
	ON_BN_CLICKED(IDC_SMS_CHECK, OnBnClickedSmsCheck)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL, OnBnClickedButtonChannel)
	ON_BN_CLICKED(IDC_BUTTON_IDPW, OnBnClickedButtonIdpw)
	ON_BN_CLICKED(IDC_BUTTON_HSB_UPDATE, OnBnClickedButtonHsbUpdate)
	ON_BN_CLICKED(IDC_BUTTON_SVN_UPDATE, OnBnClickedButtonSvnUpdate)
	ON_BN_CLICKED(IDC_BUTTON_WINDOWS_UPDATE, OnBnClickedButtonWindowsUpdate)
	ON_BN_CLICKED(IDC_BUTTON_VIRUS_CHECK, OnBnClickedButtonVirusCheck)
	ON_BN_CLICKED(IDC_UI_LOCK, OnBnClickedUiLock)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CKEsdRemoteController_ServerDlg �޽��� ó����

BOOL CKEsdRemoteController_ServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ������ ���� ��ġ�� ���� : ������ ��ܿ� ��ġ�ϵ���!
	RECT rect;
	GetWindowRect( &rect );
	MoveWindow( GetSystemMetrics( SM_CXSCREEN ) - 270, 20, ( rect.right - rect.left ), ( rect.bottom - rect.top ) );

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	g_pLua = lua_open();

	if( g_pLua == NULL )
	{
		KDbgStm << "��� ���� ����.!";
		return FALSE;
	}

	//luaopen_base( g_pLua );  // ����Լ� tonumber�� ����ؾ� �Ѵ�.
	//luaopen_table( g_pLua );
	//luaopen_io( g_pLua );
	//luaopen_string( g_pLua );
	//luaopen_math( g_pLua );
	//lua_settop( g_pLua, 0 );

	luaL_openlibs( g_pLua );
	lua_settop( g_pLua, 0 );

	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetTimer( 0, 1, NULL );

	theApp.m_kImp.Initialize();
	
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CKEsdRemoteController_ServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CKEsdRemoteController_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKEsdRemoteController_ServerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if ( nIDEvent == 0 )
	{
		theApp.m_kImp.Update();
	}

	CDialog::OnTimer(nIDEvent);
}

int CKEsdRemoteController_ServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.CheckAlreadyOnServer();

	return 0;
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonPatch()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	theApp.m_kImp.OnEsdSVNUpdate();
}

void CKEsdRemoteController_ServerDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lua_close( g_pLua );

	KillTimer( 0 );

	CDialog::OnClose();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunch()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	theApp.m_kImp.OnEsdLaunch();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonShutdown()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	theApp.m_kImp.OnEsdShutdown();	
}

BOOL CKEsdRemoteController_ServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if( pMsg->message == WM_KEYDOWN )
	{       
		if ( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunchDbg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ���� ���� �ʽ��ϴ�!
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunchCo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.OnEsdLaunch_CenterServer();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLaunchCoDbg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ���� ���� �ʽ��ϴ�.
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLogin()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.OnEsdLaunch_LoginServer();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonLoginDbg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ���� ���� �ʽ��ϴ�.
}

void CKEsdRemoteController_ServerDlg::OnBnClickedSmsCheck()
{
	bool bCheck = ( m_checkSMS.GetCheck() == 0 ? false : true );
	theApp.m_kImp.SetPhoneEnable( bCheck );
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonChannel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.OnEsdLaunch_ChannelServer();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonIdpw()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( theApp.m_kImp.IsDBConnStr() )
	{
		theApp.m_kImp.OpenDBConnectionDlg();
	}
	else
	{
		::MessageBox( NULL, L"DB Connection String�� ������� �ʵ��� ���õǾ� �ֽ��ϴ�!", L"", 0 );
	}
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonHsbUpdate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.OnEsdHSBUpdate();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonSvnUpdate()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdSVNUpdate();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonWindowsUpdate()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdWindowsUpdate();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedButtonVirusCheck()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdVirusCheck();
}

void CKEsdRemoteController_ServerDlg::OnBnClickedUiLock()
{
	// TODO: Add your control notification handler code here

	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH, OnBnClickedButtonLaunch)
	//ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnBnClickedButtonShutdown)
	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH_DBG, OnBnClickedButtonLaunchDbg)
	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO, OnBnClickedButtonLaunchCo)
	//ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO_DBG, OnBnClickedButtonLaunchCoDbg)
	//ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnBnClickedButtonLogin)
	//ON_BN_CLICKED(IDC_BUTTON_LOGIN_DBG, OnBnClickedButtonLoginDbg)
	//ON_BN_CLICKED(IDC_SMS_CHECK, OnBnClickedSmsCheck)
	//ON_BN_CLICKED(IDC_BUTTON_CHANNEL, OnBnClickedButtonChannel)
	//ON_BN_CLICKED(IDC_BUTTON_IDPW, OnBnClickedButtonIdpw)
	//ON_BN_CLICKED(IDC_BUTTON_HSB_UPDATE, OnBnClickedButtonHsbUpdate)
	//ON_BN_CLICKED(IDC_BUTTON_SVN_UPDATE, OnBnClickedButtonSvnUpdate)
	//ON_BN_CLICKED(IDC_BUTTON_WINDOWS_UPDATE, OnBnClickedButtonWindowsUpdate)
	//ON_BN_CLICKED(IDC_BUTTON_VIRUS_CHECK, OnBnClickedButtonVirusCheck)	
}


void CKEsdRemoteController_ServerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdLaunch_GlobalServer();
}
