// PathDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "PathDialog.h"
#include ".\pathdialog.h"
#include "resource.h"


// CPathDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathDialog, CDialog)
CPathDialog::CPathDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPathDialog::IDD, pParent)
{
}

CPathDialog::~CPathDialog()
{
}

void CPathDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OLD_PATH_EDIT, m_oldPathEdit);
	DDX_Control(pDX, IDC_NEW_PATH_EDIT, m_newPathEdit);
	DDX_Control(pDX, IDC_COMPILE_CHECK, m_checkCompile);
	DDX_Control(pDX, IDC_COMPILE_CHECK2, m_checkCrypt);
	DDX_Control(pDX, IDC_COMPILE_CHECK3, m_checkXConvert);
}

BOOL CPathDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_checkCompile.SetCheck( TRUE );
	m_bDoCompile = true;

	m_checkCrypt.SetCheck( TRUE );
	m_bDoCrypt = true;

	//m_checkXConvert.SetCheck( TRUE );
	m_bDoXConvert = false;

	return TRUE;
}


BEGIN_MESSAGE_MAP(CPathDialog, CDialog)
	ON_BN_CLICKED(ID_OLD_PATH, OnBnClickedOldPath)
	ON_BN_CLICKED(ID_NEW_PATH, OnBnClickedNewPath)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_COMPILE_CHECK, OnBnClickedCompileCheck)
	ON_BN_CLICKED(IDC_COMPILE_CHECK2, OnBnClickedCryptCheck)
	ON_BN_CLICKED(IDC_COMPILE_CHECK3, OnBnClickedXFileConvertCheck)
END_MESSAGE_MAP()


// CPathDialog �޽��� ó�����Դϴ�.

void CPathDialog::OnBnClickedOldPath()
{
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( BrowseFolder( this->m_hWnd, TEXT("������ ������ �����ϼ���"), StartPath, folder ) == false )
		return;
	
	m_oldPathEdit.SetWindowText( folder );
	m_wstrOldPath = folder;
}

void CPathDialog::OnBnClickedNewPath()
{
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( BrowseFolder( this->m_hWnd, TEXT("�Ź��� ������ �����ϼ���"), StartPath, folder ) == false )
	return;

	m_newPathEdit.SetWindowText( folder );
	m_wstrNewPath = folder;
}
void CPathDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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

bool CPathDialog::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner	  = hParent;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle	  = szTitle;
	bi.ulFlags		  = 0;
	bi.lpfn			  = BrowseCallbackProc;
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
void CPathDialog::OnBnClickedCompileCheck()
{
	m_bDoCompile = ( m_checkCompile.GetCheck() == 0 ? false : true );
}

void CPathDialog::OnBnClickedCryptCheck()
{
	m_bDoCrypt = ( m_checkCrypt.GetCheck() == 0 ? false : true );
}

void CPathDialog::OnBnClickedXFileConvertCheck()
{
	MessageBox( L"���� ����Ͻ� �� �����ϴ�!" );
	//m_bDoXConvert = ( m_checkXConvert.GetCheck() == 0 ? false : true );

	m_checkXConvert.SetCheck( FALSE );
}
