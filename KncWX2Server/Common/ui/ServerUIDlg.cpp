// WGameServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerUIDlg.h"
#include "Event.h"
#include "HookStdio.h"
#include "../BaseServer.h"
#include ".\serveruidlg.h"
#include ".\dbconnectiondlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


KBaseServer* g_pkServer = NULL;
CHookStdio   HookStdio(STD_OUTPUT_HANDLE);
CHookStdio   HookError(STD_ERROR_HANDLE);

UINT Thread_STDIO( LPVOID pParam )
{
    char Buf[1024];
    DWORD dwRead;
    CServerUIDlg* pDlg = ((CServerUIDlg*)pParam);
    std::string strBuf;
	bool bIsReady = false;

    while(true)
    {
        if( dwRead = HookStdio.Read(Buf,sizeof(Buf)-2) )
        {           
            Buf[dwRead] = 0;
            Buf[dwRead+1] = 0;
            pDlg->m_EditLogger.AddText( Buf, true );            
        }
        if( dwRead = HookError.Read(Buf,sizeof(Buf)-2) )
        {           
            Buf[dwRead] = 0;
            Buf[dwRead+1] = 0;
            pDlg->m_EditLogger.AddText( Buf, true );            
        }

		if ( g_pkServer->GetServerReady() == true )
		{			
			if( bIsReady == false )
			{
				WCHAR acBuf[256] = { 0 };
				pDlg->GetWindowText( acBuf, 256 );

				std::wstring strText = acBuf;
				int iPos = strText.find( L"_" );
				strText = strText.substr( 0, iPos );

				strText += L"_Ready";
				pDlg->SetWindowText( strText.c_str() );

				bIsReady = true;
			}			
		}

        Sleep(5);

        if( !g_pkServer ) // ������ ����Ǹ� �����嵵 ����.
            AfxEndThread( 0 );
    }   
}

// CServerUIDlg dialog
CServerUIDlg::CServerUIDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CServerUIDlg::IDD, pParent), 
      m_EditStrBuf( &m_EditLogger ),
      m_EditStrBufW( &m_EditLogger ),
      m_EditStrBufError( &m_EditLogger ),
      m_EditStrBufWError( &m_EditLogger ),
      m_strInput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
    m_iInputCursor = -1;
}

void CServerUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_INPUT, m_strInput);
    DDX_Control(pDX, IDC_INPUT, m_editInput);
    DDX_Control(pDX, IDC_OUTPUT_EDIT, m_EditLog);   
}

BEGIN_MESSAGE_MAP(CServerUIDlg, CDialog)
	ON_WM_PAINT()
		// ON_WM_CLOSE() - wParam ���ڸ� �ޱ� ���� �Ʒ��� ���� Ǯ�� ��
		{
			WM_CLOSE, 0, 0, 0, AfxSig_vw,
			(AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT_PTR) > (OnClose))
		},
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_TIMER()
//  ON_LBN_SELCHANGE(IDC_OUTPUT, OnLbnSelchangeOutput)
//  ON_WM_CTLCOLOR()
//ON_WM_LBUTTONDBLCLK()
//ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CServerUIDlg message handlers

BOOL CServerUIDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 6. 15  ������	cmd args
	GetCommandLineString();
	//}}
	//////////////////////////////////////////////////////////////////////////

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here  

    m_pOldBuf       = std::cout.rdbuf( &m_EditStrBuf ); 
    m_pOldBufW      = std::wcout.rdbuf( &m_EditStrBufW );
    m_pOldBufError  = std::cerr.rdbuf( &m_EditStrBufError );
    m_pOldBufWError = std::wcerr.rdbuf( &m_EditStrBufWError );  

    // Set the Edit-control as logger destination
    m_EditLogger.SetEditCtrl( m_EditLog.m_hWnd );
    
    SetTimer(0,1000,NULL);
    //SetTimer(1,1,NULL);

	// ������ Ÿ��Ʋ ����
	{
		WCHAR acBuf[256] = { 0 };
		GetWindowText( acBuf, 256 );

		std::wstring strText = acBuf;
		strText += L"_Loading";
		SetWindowText( strText.c_str() );
	}

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerUIDlg::OnClose(UINT nForceClose) 
{
#ifdef SERV_MANAGEMENT_TOOL
		CDialog::OnClose(); 
#else //SERV_MANAGEMENT_TOOL
	// ���� ����
	if (nForceClose != NULL)
	{
		CDialog::OnClose();
		return;
	}

	int iRet = MessageBox( L"Ȯ���� �����Ͻðڽ��ϱ� ?", L"Ȯ��", MB_OKCANCEL );
	if (iRet == IDOK)
		CDialog::OnClose();
#endif //SERV_MANAGEMENT_TOOL
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerUIDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerUIDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CServerUIDlg::OnBnClickedCancel()
{
    //if( g_pkServer )
    //{
    //    if( AfxMessageBox(L"��Shutdown Server��",MB_YESNO) == IDYES )
    //    {
    //        ShutdownServer();
    //        if( AfxMessageBox(L"��Restart Server?��",MB_YESNO) == IDYES )
    //        {
    //            StartupServer();
    //            return;
    //        }
    //    }
    //    else
    //        return;
    //}
    CDialog::OnCancel();        
}

void CServerUIDlg::OnBnClickedOk()
{   
    UpdateData();

    if( m_strInput.GetLength() != 0 )
    {       
        std::string strLua = KncUtil::toNarrowString( (LPCTSTR)m_strInput );
        luaL_dostring( g_pLua, strLua.c_str() );

        if( m_iInputCursor == -1 )          
        {   
            m_vecInputHistory.push_back(m_strInput);
            m_iInputCursor = m_vecInputHistory.size();
        }
        else if( m_iInputCursor < (int)m_vecInputHistory.size() )
        {
            if( m_vecInputHistory[m_iInputCursor] != m_strInput )
            {
                m_vecInputHistory.push_back(m_strInput);
                m_iInputCursor = m_vecInputHistory.size();
            }
            else
            {
                m_iInputCursor++;
            }
        }
    }

    m_strInput = "";
    UpdateData(FALSE);
    m_editInput.SetFocus();
}

BOOL CServerUIDlg::DestroyWindow()
{   
    ShutdownServer();
    
    cout.rdbuf( m_pOldBuf );
    wcout.rdbuf( m_pOldBufW );
    std::cerr.rdbuf( m_pOldBufError );
    std::wcerr.rdbuf( m_pOldBufWError );    
    
    /*
    cerr.rdbuf( m_pOldBufError );   
    wcerr.rdbuf( m_pOldBufErrorW );
    */

    return CDialog::DestroyWindow();
}

void CServerUIDlg::GetCommandLineString()
{
	int nNumArgs = 0;
	WCHAR** pstrArgList = CommandLineToArgvW( GetCommandLine(), &nNumArgs );

	bool bDBConnStrPopUp = false;
	for( int i = 0; i < nNumArgs; ++i )
	{
		//{{ 2009. 7. 13  ������	-s �ɼ��� �������ڷ� ������ db connection popup�� �����!
		if( std::wstring( pstrArgList[i] ) == L"-popup" )
		{
			bDBConnStrPopUp = true;
			continue; // �������� ����Ʈ������ ����!
		}
		//}}

		m_vecCmdArgs.push_back( pstrArgList[i] );
		//ZeroMemory( pstrArgList[i], wcslen( pstrArgList[i] ) ); - �ʱ�ȭ�� �ǹ̰� ���� �ٸ������ ã�ƺ���
	}

	if( bDBConnStrPopUp == true )
	{
		// Dialog�� ID & PW�� �Է¹���!
		CDBConnectionDlg dbConnDlg;
		dbConnDlg.SetEditList( &m_vecCmdArgs );
		dbConnDlg.DoModal();

		if( dbConnDlg.IsOK() == false )
		{
			// id & pw�� �Էµ��� �ʾҴٸ� �����Ų��
			::exit( 0 );
		}
	}
}

extern KBaseServer* CreateKNC();

void CServerUIDlg::StartupServer()
{

    //------------------------------------------------------------------
    g_pkServer = CreateKNC();
    g_pkServer->SetHwnd( m_hWnd );
	//{{ 2009. 6. 15  ������	cmd args
	g_pkServer->SetCmdArgs( m_vecCmdArgs );
	m_vecCmdArgs.clear();
	//}}

    if( g_pkServer->Init() )
    {
        g_pkServer->Run( false );
    }
    //------------------------------------------------------------------

    m_pThreadStdIO = AfxBeginThread( Thread_STDIO, this );
}

void CServerUIDlg::ShutdownServer()
{
    if( g_pkServer == NULL )
        return;

    g_pkServer->ShutDown();
    KBaseServer::ReleaseKObj();
    g_pkServer = NULL;
}

void CServerUIDlg::OnTimer(UINT nIDEvent)
{
    if( nIDEvent == 0 )
    {       
        StartupServer();        
        KillTimer(nIDEvent);        
    }
    else
    {
        static int iCount = 0;

        wcout << L"=====================================================================Memory ��뷮 �׽�Ʈ ���Դϴ� : " << iCount++ << endl;
    }

    CDialog::OnTimer(nIDEvent);
}

BOOL CServerUIDlg::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYDOWN )
    {       
		if ( pMsg->wParam == VK_ESCAPE )
		{
			 return TRUE;
		}
        else if( pMsg->wParam == VK_UP )
        {
            if( m_vecInputHistory.size() == 0 ) 
                return CDialog::PreTranslateMessage(pMsg);
            
            m_iInputCursor--;
            if( m_iInputCursor < 0 )
                m_iInputCursor = 0;

            m_strInput = m_vecInputHistory[m_iInputCursor];
            UpdateData(FALSE);
            m_editInput.SetSel(-1,-1);
            return TRUE;
        }
        else if( pMsg->wParam == VK_DOWN )
        {
            if( m_vecInputHistory.size() == 0 ) 
                return CDialog::PreTranslateMessage(pMsg);

            if( ++m_iInputCursor >= (int)m_vecInputHistory.size() )
                m_iInputCursor = m_vecInputHistory.size() -1;

            m_strInput = m_vecInputHistory[m_iInputCursor];
            UpdateData(FALSE);
            m_editInput.SetSel(-1,-1);
            return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CServerUIDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
    if( message == WM_APP )
    {
        //switch( wParam ){
        //default:
        //    break;
        //}
    }

    return CDialog::WindowProc(message, wParam, lParam);
}

//void CServerUIDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//
//	CDialog::OnLButtonDblClk(nFlags, point);
//}

//void CServerUIDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//
//	CDialog::OnLButtonDblClk(nFlags, point);
//}
