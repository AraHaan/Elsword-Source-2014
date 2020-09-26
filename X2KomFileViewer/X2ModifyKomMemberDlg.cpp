// X2ModifyKomMemberDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
//#include "X2OldKomFileViewer.h"
#include "X2ModifyKomMemberDlg.h"
#include ".\x2modifykommemberdlg.h"
#include "NoticeDlg.h"


// CX2ModifyKomMemberDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CX2ModifyKomMemberDlg, CDialog)
CX2ModifyKomMemberDlg::CX2ModifyKomMemberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX2ModifyKomMemberDlg::IDD, pParent)
{
}

CX2ModifyKomMemberDlg::~CX2ModifyKomMemberDlg()
{
}

void CX2ModifyKomMemberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAVE_DIR_EDIT, m_cSaveDirEdit);
	//DDX_Control(pDX, IDC_MODIFY_FILE_EDIT, m_cModifyFileEdit);
	DDX_Control(pDX, IDC_MODIFY_LIST, m_cModifyListBox);
	DDX_Control(pDX, IDC_CHECK_C, m_cLuaCompileCheck);
	DDX_Control(pDX, IDC_CHECK_E, m_cScriptEncryptCheck);
}

BOOL CX2ModifyKomMemberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ����Ʈ ��Ʈ�� �ʱ�ȭ
	InitListControl();

	// üũ�ڽ� �ʱ�ȭ
	m_cLuaCompileCheck.SetCheck( TRUE );
	m_cScriptEncryptCheck.SetCheck( TRUE );

	return TRUE;
}

void CX2ModifyKomMemberDlg::InitListControl()
{
	// kom member file list
	m_cModifyListBox.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );


	m_cModifyListBox.InsertColumn ( 0, _T("KomNum"), LVCFMT_LEFT, 0, 0 );
	m_cModifyListBox.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	m_cModifyListBox.InsertColumn ( 2, _T("File name"), LVCFMT_LEFT, 0, 2 );
	m_cModifyListBox.InsertColumn ( 3, _T("Full Path"), LVCFMT_LEFT, 0, 3 );	

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	/*m_cModifyListBox.InsertColumn ( 0, _T("KomNum"), LVCFMT_LEFT, 0, 0 );
	m_cModifyListBox.InsertColumn ( 1, _T("File name"), LVCFMT_LEFT, 0, 1 );
	m_cModifyListBox.InsertColumn ( 2, _T("Full Path"), LVCFMT_LEFT, 0, 2 );	

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );*/

	m_cModifyListBox.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT );
}


BEGIN_MESSAGE_MAP(CX2ModifyKomMemberDlg, CDialog)
	ON_BN_CLICKED(IDFINDSAVEPATH, OnBnClickedFindsavepath)
	ON_BN_CLICKED(IDFINDMODIFYFILE, OnBnClickedFindmodifyfile)
	ON_BN_CLICKED(IDMODIFYBEGIN, OnBnClickedModifybegin)
	ON_BN_CLICKED(IDDELMODIFYFILE, OnBnClickedDelmodifyfile)
END_MESSAGE_MAP()


// CX2ModifyKomMemberDlg �޽��� ó�����Դϴ�.

int CALLBACK BrowseCallbackProcMKMDlg( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	switch( uMsg )
	{
	case BFFM_INITIALIZED:
		if( lpData != NULL )
			::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData );
		break;
	}
	return 0;
}

bool CX2ModifyKomMemberDlg::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner	  = hParent;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle	  = szTitle;
	bi.ulFlags		  = 0;
	bi.lpfn			  = BrowseCallbackProcMKMDlg;
	bi.lParam		  = (LPARAM)StartPath;

	pidl = SHBrowseForFolder( &bi );

	if( pidl == NULL )
		return false;

	SHGetPathFromIDList( pidl, szFolder );

	if( SHGetMalloc( &pMalloc ) != NOERROR )
		return false;

	pMalloc->Free( pidl );
	pMalloc->Release();
	return true;
}

void CX2ModifyKomMemberDlg::OnBnClickedFindsavepath()	// ������� ������ ���� ����
{
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( BrowseFolder( this->m_hWnd, TEXT("������ ������� ������ ������ �����ϼ���."), StartPath, folder ) == false )
		return;

	m_cSaveDirEdit.SetWindowText( folder );
	m_wstrKomSavePath = folder;
}

void CX2ModifyKomMemberDlg::OnBnClickedFindmodifyfile() // ������ ���ҽ� ã��
{
	CFileDialog Files( TRUE, L"*.*", NULL, OFN_ENABLESIZING, L"All Files (*.*)|*.*|" );
	if( Files.DoModal() == IDCANCEL ) return;

	std::wstring wstrFileFullPath = Files.GetPathName().GetBuffer();
	std::wstring wstrFileName = Files.GetFileName().GetBuffer();

	//////////////////////////////////////////////////////////////////////////	
	std::wstring wstrKomString;
	std::wstring wstrType;
	int iIndex;
	if( theApp.m_komFileManager.MemberFileFind( wstrFileName, true, wstrKomString, iIndex ) == false )	// �߰� �۾� �̶��
	{
		if( MessageBox( L"Kom������ ��������� �̸��� ��ġ�ϴ� ������ �����ϴ�!\n���� �߰� �Ͻðڽ��ϱ�?", NULL, MB_YESNO ) == IDNO )
			return;
		else
		{
			// to do: listbox ���
			//CX2OldKomFileViewer oldKomFileViewrDlg;		// DLG Ŭ����
		
			//if ( oldKomFileViewrDlg.DoModal() == IDOK )
			//{
			//	wstrKomString	= oldKomFileViewrDlg.GetSelectedItemText();
			//	wstrType		= L"�߰�";
			//}
			//else
			//	return;
			
		}
	}
	else		// ��ü �۾� �̶��
	{
		wstrType		= L"��ü";
	}

	if( m_mapModifyData.find( wstrFileName ) != m_mapModifyData.end() )
	{
		MessageBox( L"�̹� ��ϵ� �����Դϴ�." );
		return;
	}

	ModifyData modifyData;
	modifyData.m_wstrFileFullPath = wstrFileFullPath;
	modifyData.m_wstrKomString	  = wstrKomString;

	m_mapModifyData.insert( std::make_pair( wstrFileName, modifyData ) );

	//////////////////////////////////////////////////////////////////////////

	int iCount = m_cModifyListBox.GetItemCount();
	m_cModifyListBox.InsertItem( iCount, wstrKomString.c_str() );
	m_cModifyListBox.SetItemText( iCount, 1, wstrType.c_str() );
	m_cModifyListBox.SetItemText( iCount, 2, wstrFileName.c_str() );
	m_cModifyListBox.SetItemText( iCount, 3, wstrFileFullPath.c_str() );

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );

	/*m_cModifyListBox.SetItemText( iCount, 1, wstrFileName.c_str() );
	m_cModifyListBox.SetItemText( iCount, 2, wstrFileFullPath.c_str() );

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );*/
	
}

void CX2ModifyKomMemberDlg::OnBnClickedDelmodifyfile()			// ����Ʈ���� ����
{
	int iIndex = m_cModifyListBox.GetSelectionMark();
	CString cstrFileName = m_cModifyListBox.GetItemText( iIndex, 0 );
	m_cModifyListBox.DeleteItem( iIndex );
	m_mapModifyData.erase( std::wstring( cstrFileName.GetBuffer() ) );
}

static HWND g_repackagingMBHwnd;

UINT WINAPI RepackagingMessageBoxThread( LPVOID arg )
{
	CNoticeDlg noticeDlg;
	std::wstring wstrMessage = L"���ҽ� ��ü��...";
	noticeDlg.ChangeText( wstrMessage );
	g_repackagingMBHwnd = noticeDlg.m_hWnd;
	noticeDlg.DoModal();
	return 0;
}

void CX2ModifyKomMemberDlg::OnBnClickedModifybegin()
{
	if( m_wstrKomSavePath.empty() )
	{
		MessageBox( L"������� ������ ��θ� �������ּ���!" );
		return;
	}

	if( m_mapModifyData.empty() )
	{
		MessageBox( L"�����Ͻ� ���ҽ��� �����ϴ�!" );
		return;
	}

	// ��Ȳ�˸� ������ ����
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, RepackagingMessageBoxThread, (LPVOID)NULL, NULL, NULL );

	bool bLuaScriptCompile = ( m_cLuaCompileCheck.GetCheck() == TRUE );
	bool bScriptEncrypt = ( m_cScriptEncryptCheck.GetCheck() == TRUE );
	if( theApp.m_komFileManager.RepackagingKomFile( m_wstrKomSavePath, m_mapModifyData, bLuaScriptCompile, bScriptEncrypt ) )
	{
		::SendMessage( g_repackagingMBHwnd, WM_CLOSE, 0, 0 );
		TerminateThread( hNoticeThread, 0 );
		MessageBox( L"���ҽ� ���Ƴ���� ����!" );
		return;
	}

	::SendMessage( g_repackagingMBHwnd, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );
}


