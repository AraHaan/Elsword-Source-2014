

//{{ dmlee 2008.03.26
// �ؽ� ĳ�ð��� ������ â ���� �۾���. 
// ��üȭ�鿡�� win32 window �߰� �ϱ� ���ؼ� g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE ), â�� ������ (FALSE) ����� (performance ���� ������)
// f_pWrapper ��� ������ static global �̶� KBrowserWrapper�� �ϳ��� ������ ��. ��, singleton
// KTDXLIB�� KBrowserWrapper�� ������ KBrowserWrapper�� ���� ��� dialog, bitmap�� resource�� x2/resource.h�� ����.
// KBrowserWrapper�� ��ü�� CX2Main�� �����
//}} dmlee 2008.03.26


#include "stdafx.h"
#include "KBrowserWrapper.h"


static KBrowserWrapper* f_pWrapper = NULL;


BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
	{
	case WM_INITDIALOG:
		{
			// ������ ����
			f_pWrapper->OpenBrowser( hDlg );

			//// ���콺 Ŭ��
			//RECT rc;
			//::GetWindowRect( hDlg, &rc );
			//::ClipCursor( &rc );

			return TRUE;
		} break;

	case WM_COMMAND:
		{
			// ������ �ݱ� �� ���콺 Ŭ�� ����
			//switch ( LOWORD( wParam ) )
			//{
			//case IDOK:
			//	{
			//		if( false == f_pWrapper->IsClosed() )
			//		{
			//			f_pWrapper->CloseBrowser();
			//		}
			//		::EndDialog( hDlg, IDOK );
			//		::ClipCursor( NULL );

			//		//{{ dmlee 2008.03.26
			//		g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			//		//}} dmlee 2008.03.26

			//		return TRUE;
			//	}
			//case IDCANCEL:
			//	{
			//		if( false == f_pWrapper->IsClosed() )
			//		{
			//			f_pWrapper->CloseBrowser();
			//		}
			//		::EndDialog( hDlg, IDCANCEL );
			//		::ClipCursor( NULL );

			//		//{{ dmlee 2008.03.26
			//		g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			//		//}} dmlee 2008.03.26

			//		return TRUE;
			//	}
			//}
			return FALSE;
		} break;

	case WM_CLOSE:
		{
			if( false == f_pWrapper->IsClosed() )
			{
				f_pWrapper->CloseBrowser();
			}
			::EndDialog( hDlg, IDCANCEL );
			::ClipCursor( NULL );

			//{{ dmlee 2008.03.26
			g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			//}} dmlee 2008.03.26

			return TRUE;
		} break;
	}

    return FALSE;
}

KBrowserWrapper::KBrowserWrapper( void )
{
    f_pWrapper  = this;

    ::ZeroMemory( &m_rect, sizeof( RECT ) );
    m_bClosed   = true;
	m_bCheckClosed = true;
    m_hDlg      = NULL;
	m_pPCHBrowser = NULL;
}

KBrowserWrapper::~KBrowserWrapper( void )
{
    DeleteBrowserObj();
    f_pWrapper = NULL;
}

#ifdef SEND_NEXON_WISE_LOG_BY_URL // ����ȯ
BOOL KBrowserWrapper::Create( RECT& rect, HWND hWndParent, DWORD dwStyle /*= SW_SHOW*/ )
#else // SEND_NEXON_WISE_LOG_BY_URL
BOOL KBrowserWrapper::Create( RECT& rect, HWND hWndParent )
#endif //  SEND_NEXON_WISE_LOG_BY_URL
{
    m_rect      = rect;
    m_bClosed   = false;
	m_bCheckClosed = true;

	//{{ dmlee 2008.03.24
	const int MAGIC_IDD_BROWSER_DIALOG = 115;					// note!! x2/resource.h�� �ִ� IDD_BROWSER_DIALOG
	HINSTANCE hInst = ::GetModuleHandle( NULL );
	LPTSTR szRes = MAKEINTRESOURCE( MAGIC_IDD_BROWSER_DIALOG );
	m_hDlg = ::CreateDialog( hInst, szRes, hWndParent, DlgProc );
	if( NULL == m_hDlg )
	{
		return FALSE;
	}

#ifdef SEND_NEXON_WISE_LOG_BY_URL // ����ȯ
	::ShowWindow( m_hDlg, dwStyle );	/// ǥ�� ����
#endif //  SEND_NEXON_WISE_LOG_BY_URL

	//}} dmlee 2008.03.24

    return TRUE;
}

bool KBrowserWrapper::OpenBrowser( HWND hDlg )
{
    if ( false == f_pWrapper->AllocateBrowserObj() )
    {
        return false;
    }

    int w = m_rect.right - m_rect.left;
    int h = m_rect.bottom - m_rect.top;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = w - 5;
    rect.bottom = h;

	//���ҽ� ���� �ʿ�
	/*
    if ( FALSE == m_pPCHBrowser->Create( WS_VISIBLE|WS_CHILD, rect, hDlg, 10000 ) )
    {
        ::MessageBox( hDlg,
                        g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TEXT ).c_str(),
                        g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ).c_str(),
                        MB_OK );
    }

    ::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
    ::ShowWindow( hDlg, SW_SHOW );

    // �ݱ� ��ư ��ġ ���� �� �̸� ����
    int btnx = ( w - 100 ) / 2;
    int btny = h - 25;
    ::MoveWindow( ::GetDlgItem( hDlg, IDOK ), btnx, btny, 80, 20, TRUE );
    ::SetWindowText( ::GetDlgItem( hDlg, IDOK ),
                     g_pkStrLoader->GetString( STR_ID_CASH_BUY_CLOSE_BUTTON ).c_str() );
					 */



	//{{ dmlee 2008.03.24
#ifdef CONVERSION_VS
	const int MAGIC_CHILD_WINDOW_ID = 5678; // note!! m_pPCHBrowser->m_hWrapperWnd�� ID
#else CONVERSION_VS
	const MAGIC_CHILD_WINDOW_ID = 5678; // note!! m_pPCHBrowser->m_hWrapperWnd�� ID
#endif CONVERSION_VS
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;
	if ( FALSE == m_pPCHBrowser->Create( dwStyle, rect, hDlg, MAGIC_CHILD_WINDOW_ID ) )
	{
		::MessageBox( hDlg,
			L"open browser failed",
			L"error",
			MB_OK );
	}

	::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
	::ShowWindow( hDlg, SW_SHOW );

	// �ݱ� ��ư ��ġ 
	//int btnx = ( w - 100 ) / 2;
	//int btny = h - 40;
	//::MoveWindow( ::GetDlgItem( hDlg, IDOK ), btnx, btny, 80, 20, TRUE );
	//}} dmlee 2008.03.24


    return true;
}

void KBrowserWrapper::CloseBrowser( void )
{
    m_bClosed = true;
	m_bCheckClosed = false;
	if( NULL != m_pPCHBrowser )
		m_pPCHBrowser->Destroy();

    DeleteBrowserObj();
}