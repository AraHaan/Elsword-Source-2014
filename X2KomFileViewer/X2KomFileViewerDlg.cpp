// X2KomFileViewerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2KomFileViewerDlg.h"
#include ".\x2komfileviewerdlg.h"
#include "UncompressDlg.h"
#include "X2ModifyKomMemberDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2KomFileViewerDlg ��ȭ ����



CX2KomFileViewerDlg::CX2KomFileViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX2KomFileViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsKomParsingOnly = false;
}

void CX2KomFileViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_memberList);
	DDX_Control(pDX, IDC_INSERTED_LIST, m_insertedListCtrl);
	DDX_Control(pDX, IDC_DELETED_LIST, m_deletedListCtrl);	
	DDX_Control(pDX, IDC_MEMBER_COUNT, m_memberCount);
	DDX_Control(pDX, IDC_MEMBER_COUNT2, m_komSizeEdit);
	DDX_Control(pDX, IDC_KOM_LIST, m_komList);
	DDX_Control(pDX, IDC_FILE_FIND_EDIT, m_editFileFind);
	DDX_Control(pDX, IDC_MT_CHECK, m_checkMT);
}

BEGIN_MESSAGE_MAP(CX2KomFileViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, OnNMCustomdrawList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST, OnLvnItemActivateList)
	ON_BN_CLICKED(IDC_CREATE_KOM, OnBnClickedCreateKom)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_KOM_LIST, OnLvnItemActivateKomList)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_FIND_FILE, OnBnClickedFindFile)
	ON_BN_CLICKED(IDC_UNCOMPRESS, OnBnClickedUncompress)
	ON_BN_CLICKED(IDC_MODIFY_KOM, OnBnClickedModifyKom)
END_MESSAGE_MAP()


// CX2KomFileViewerDlg �޽��� ó����

BOOL CX2KomFileViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// ����Ʈ �ڽ� �ʱ�ȭ
	InitListControl();

	// ����Ʈ �ڽ� ������ �߰�
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( theApp.m_komFileManager.GetMapNewMemberFile().begin()->first, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else	// ������ ���� �н��� ����������
	{
		InsertKomList( theApp.m_komFileManager.GetMapOldMemberFile().begin()->first, theApp.m_komFileManager.GetMapOldMemberFile() );
	}	

	m_komList.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );

	m_memberList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_memberList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );

	m_insertedListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_insertedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	m_deletedListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_deletedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	
	//ShowWindow(SW_SHOWMAXIMIZED);
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CX2KomFileViewerDlg::OnPaint() 
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
HCURSOR CX2KomFileViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CX2KomFileViewerDlg::InitListControl()
{
	SHFILEINFO rInfo;
	HIMAGELIST hImglist;

	hImglist = (HIMAGELIST) SHGetFileInfo ( _T(""), 0, &rInfo, sizeof(SHFILEINFO), 
		SHGFI_ICON | SHGFI_SMALLICON | 
		SHGFI_SYSICONINDEX );

	m_imglist.Attach ( hImglist );

	// kom member file list
	m_komList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_komList.InsertColumn ( 0, _T("Kom Name"), LVCFMT_LEFT, 0, 0 );
	m_komList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_komList.SetExtendedStyle ( LVS_EX_INFOTIP );

	// kom member file list
	m_memberList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_memberList.InsertColumn ( 0, _T("KomNum"), LVCFMT_LEFT, 0, 0 );
	m_memberList.InsertColumn ( 1, _T("Filename"), LVCFMT_LEFT, 0, 1 );
	m_memberList.InsertColumn ( 2, _T("Type"), LVCFMT_LEFT, 0, 2 );
	m_memberList.InsertColumn ( 3, _T("Size"), LVCFMT_LEFT, 0, 3 );

	m_memberList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );
	
	m_memberList.SetExtendedStyle ( LVS_EX_INFOTIP );

	// kom member file list
	m_insertedListCtrl.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_insertedListCtrl.InsertColumn ( 0, _T("Filename"), LVCFMT_LEFT, 0, 0 );
	m_insertedListCtrl.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	m_insertedListCtrl.InsertColumn ( 2, _T("Size"), LVCFMT_LEFT, 0, 2 );

	m_insertedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );	

	m_insertedListCtrl.SetExtendedStyle ( LVS_EX_INFOTIP );

	// kom member file list
	m_deletedListCtrl.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );
	
	m_deletedListCtrl.InsertColumn ( 0, _T("Filename"), LVCFMT_LEFT, 0, 0 );
	m_deletedListCtrl.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	m_deletedListCtrl.InsertColumn ( 2, _T("Size"), LVCFMT_LEFT, 0, 2 );

	m_deletedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );	

	m_deletedListCtrl.SetExtendedStyle ( LVS_EX_INFOTIP );
}

void CX2KomFileViewerDlg::InsertKomList( const std::wstring& wstrKomName, const std::map< std::wstring, std::vector< MemberFileInfo > >& mapMemberFile )
{
	m_komList.DeleteAllItems();
	m_memberList.DeleteAllItems();

	// ���� ��� ���� ����Ʈ �˻�	
	if( mapMemberFile.empty() == true )
		return;

	if( theApp.m_komFileManager.IsCompareKom() == true )			// ������, ������ �Ѵ� ������ ��
	{
		// ������, �Ź��� �Ѵ� ������ �� �������� ���� �߰��� ���� ����Ʈ ���
        InsertInsertedFileList( theApp.m_komFileManager.GetInsertedFile() );

		// ������ �������� �ִµ� �Ź��� �������� ���� ������ ���� ����Ʈ ���
		std::vector< MemberFileInfo >& vecDeletedFile = theApp.m_komFileManager.GetDeletedFile();
		InsertDeletedFileList( vecDeletedFile );
		
		std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit = mapMemberFile.begin();
		for( ; mit != mapMemberFile.end(); mit++ )
		{
			m_komList.InsertItem( m_komList.GetItemCount(), mit->first.c_str() );

			// member list
			if( mit->first == wstrKomName )		// data0 ��
			{
				std::vector< MemberFileInfo >::const_iterator vit = mit->second.begin();
				for( ; vit != mit->second.end(); vit++ )		// �Ź��� ����
				{
					const MemberFileInfo& memberFile = *vit;
					CString Temp;
					int iCount = m_memberList.GetItemCount();
					m_memberList.InsertItem( iCount, mit->first.c_str() );
					m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
					m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
					Temp.Format( L"%d KB", memberFile.m_iFileSize );
					m_memberList.SetItemText( iCount, 2, L"kom(����)" );
					m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
				}

				for( vit = vecDeletedFile.begin(); vit != vecDeletedFile.end(); vit++ )	// ������ ������ �ְ� �Ź��� ������ ���� ������ �ϴ� �͵�(data0?)
				{
					const MemberFileInfo& memberFile = *vit;
					if( memberFile.m_wstrKomString == mit->first )
					{
						CString Temp;
						int iCount = m_memberList.GetItemCount();
						m_memberList.InsertItem( iCount, mit->first.c_str() );
						m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
						m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
						Temp.Format( L"%d KB", memberFile.m_iFileSize );
						m_memberList.SetItemText( iCount, 2, L"kom(����)" );
						m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
					}
				}
			}
		}

		// �����Ǿ��µ� kom������ ���°��϶�
		std::vector< MemberFileInfo >::const_iterator vitDel;
		for( vitDel = vecDeletedFile.begin(); vitDel != vecDeletedFile.end(); vitDel++ )	// data0 ���� �͵�?
		{
			const MemberFileInfo& memberFile = *vitDel;
			std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit = mapMemberFile.find( memberFile.m_wstrKomString );
			if( mit == mapMemberFile.end() )
			{
				CString Temp;
				int iCount = m_memberList.GetItemCount();
				m_memberList.InsertItem( iCount, memberFile.m_wstrKomString.c_str() );
				m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
				m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
				Temp.Format( L"%d KB", memberFile.m_iFileSize );
				m_memberList.SetItemText( iCount, 2, L"kom(����)" );
				m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
			}
		}

		CString temp;
		temp.Format( L"%d", m_memberList.GetItemCount() );
		m_memberCount.SetWindowText( temp );
	}
	else				// ������ or ������ �� �ϳ��� ������ ��
	{
		// kom list
		std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit = mapMemberFile.begin();
		for( ; mit != mapMemberFile.end(); mit++ )
		{			
			m_komList.InsertItem( m_komList.GetItemCount(), mit->first.c_str() );	// Kom File List

			// member list
			if( mit->first == wstrKomName )		// data0 ��??
			{
				InsertMemberList( mit->first, mit->second );
			}
		}

		CString temp;
		temp.Format( L"%d", m_memberList.GetItemCount() );
		m_memberCount.SetWindowText( temp );
	}
}

void CX2KomFileViewerDlg::InsertMemberList( const std::wstring& wstrKomName, const std::vector< MemberFileInfo >& vecMemberFileList )
{
	m_memberList.DeleteAllItems();

	std::vector< MemberFileInfo >::const_iterator vit = vecMemberFileList.begin();
	for( ; vit != vecMemberFileList.end(); vit++ )
	{
		const MemberFileInfo& memberFile = *vit;
		CString Temp;
		int iCount = m_memberList.GetItemCount();
		m_memberList.InsertItem( iCount, wstrKomName.c_str() );
		m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
		m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
		Temp.Format( L"%d KB", memberFile.m_iFileSize );
		m_memberList.SetItemText( iCount, 2, L"kom(����)" );
		m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
	}
}

void CX2KomFileViewerDlg::InsertInsertedFileList( const std::vector< MemberFileInfo >& vecMemberFileList )
{
	m_insertedListCtrl.DeleteAllItems();

	std::vector< MemberFileInfo >::const_iterator vit = vecMemberFileList.begin();
	for( ; vit != vecMemberFileList.end(); vit++ )
	{
		const MemberFileInfo& memberFile = *vit;
		CString Temp;
		int iCount = m_insertedListCtrl.GetItemCount();
		m_insertedListCtrl.InsertItem( iCount, memberFile.m_wstrFileName.c_str() );
		m_insertedListCtrl.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
		Temp.Format( L"%d KB", memberFile.m_iFileSize );
		m_insertedListCtrl.SetItemText( iCount, 1, L"kom(����)" );
		m_insertedListCtrl.SetItem( iCount, 2, LVIF_TEXT, Temp, 0, 0, 0, 0 );
	}
}

void CX2KomFileViewerDlg::InsertDeletedFileList( std::vector< MemberFileInfo >& vecMemberFileList )
{
	m_deletedListCtrl.DeleteAllItems();


	WCHAR temp[100] = L"Motion_Arme_VioletMage.x";
	std::wstring a = temp;
	std::vector< MemberFileInfo >::iterator vit2 = std::find_if( vecMemberFileList.begin(), vecMemberFileList.end(), MemberFileInfo::FIND_IF( a ) );
	if ( vit2 != vecMemberFileList.end() )
	{
		MemberFileInfo i = (*vit2);
		int k = 100;
	}

	std::vector< MemberFileInfo >::const_iterator vit = vecMemberFileList.begin();
	for( vit = vecMemberFileList.begin(); vit != vecMemberFileList.end(); vit++ )
	{
		const MemberFileInfo& memberFile = *vit;
		CString Temp;
		int iCount = m_deletedListCtrl.GetItemCount();
		m_deletedListCtrl.InsertItem( iCount, memberFile.m_wstrFileName.c_str() );
		m_deletedListCtrl.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
		Temp.Format( L"%d KB", memberFile.m_iFileSize );
		m_deletedListCtrl.SetItemText( iCount, 1, L"kom(����)" );
		m_deletedListCtrl.SetItem( iCount, 2, LVIF_TEXT, Temp, 0, 0, 0, 0 );
	}
}

void CX2KomFileViewerDlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{	
	NMLVCUSTOMDRAW* pNMCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT /*| CDDS_SUBITEM*/) == pNMCD->nmcd.dwDrawStage )
	{
		DWORD dwFileState = m_memberList.GetItemData( pNMCD->nmcd.dwItemSpec );
		switch( dwFileState )
		{
		case FS_INSERTED:
			pNMCD->clrText = RGB( 0, 0, 255 );
			break;

		case FS_CHANGED:
			pNMCD->clrText = RGB( 0, 255, 0 );
			break;

		case FS_DELETED:
			pNMCD->clrText = RGB( 255, 0, 0 );
			break;

		default:
			pNMCD->clrText = RGB( 0, 0, 0 );
			break;
		}

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
}

void CX2KomFileViewerDlg::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
}

void CX2KomFileViewerDlg::OnBnClickedCreateKom()	// �����ϱ�
{
	// ���� ��� ���
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( m_bIsKomParsingOnly )		
	{
		MessageBox( L"�����Ҽ� �ִ� ��尡 �ƴմϴ�!" );	// ������ ������ ������ ��
		return;
	}

	if( BrowseFolder( this->m_hWnd, TEXT("������ ������ �����ϼ���"), StartPath, folder ) == false )
		return;

	// Is Multi Thread ?
	bool bIsMultiThreadPacking = ( m_checkMT.GetCheck() == TRUE ? true : false );

	if( theApp.m_komFileManager.CreateKomFile( std::wstring( folder ), bIsMultiThreadPacking ) == false )
	{
		MessageBox( L"kom���� ���� ����!" );
	}
	else
	{
		MessageBox( L"kom���� ���� ����!" );
	}
}

int CALLBACK BrowseCallbackProc2( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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

bool CX2KomFileViewerDlg::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner	  = hParent;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle	  = szTitle;
	bi.ulFlags		  = 0;
	bi.lpfn			  = BrowseCallbackProc2;
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
void CX2KomFileViewerDlg::OnLvnItemActivateKomList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString temp = m_komList.GetItemText( m_komList.GetSelectionMark(), 0 );
	std::wstring wstrKomName = temp.GetBuffer();
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( wstrKomName, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else
	{
		InsertKomList( wstrKomName, theApp.m_komFileManager.GetMapOldMemberFile() );        
	}	

	int iKomSize = theApp.m_komFileManager.GetKomSize( std::wstring( temp.GetBuffer() ) );
	WCHAR buf[256] = L"";
	wsprintf( buf, L"%d mb", iKomSize/1024 );
	m_komSizeEdit.SetWindowText( buf );
	
	*pResult = 0;
}

#include "PathDialog.h"
#include "NoticeDlg.h"

static HWND g_noticeHwnd2;

// notice thread
UINT WINAPI NoticeThread2( LPVOID arg )
{
	CNoticeDlg noticeDlg;	
	g_noticeHwnd2 = noticeDlg.m_hWnd;	
	noticeDlg.DoModal();
	return 0;
}

void CX2KomFileViewerDlg::OnBnClickedButton1()
{
	// ��� ���� ���
	CPathDialog pathDlg;
	if( pathDlg.DoModal() != IDOK )
		return;

	// ���� ��η� ���� kom���� �Ľ��ϱ�
	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == true )
	{
		::MessageBox( NULL, L"��θ� �Է����� �ʾҽ��ϴ�!", L"����!", MB_OK );
		return;
	}

	// ��Ȳ�˸� ������ ����
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread2, (LPVOID)NULL, NULL, NULL );

	// Kom ���� �Ľ� & ��� ���� �ε�
	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == false )
	{
		// new member file obtain
		if( theApp.m_komFileManager.ParsingMemberFile( pathDlg.GetNewPath(), std::map< std::wstring, MemberFileInfo >() ) == false )
		{
			::MessageBox( NULL, L"member���� �м� ����!", L"����!", MB_OK );
			return;
		}
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == false )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( pathDlg.GetOldPath(), pathDlg.GetNewPath(), std::map< std::wstring, MemberFileInfo >() ) == false )
		{
			::MessageBox( NULL, L"member���� �м� ����!", L"����!", MB_OK );
			return;
		}
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == true )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFile( pathDlg.GetOldPath() ) == false )
		{
			::MessageBox( NULL, L"kom���� �м� ����!", L"����!", MB_OK );
			return;
		}
	}

	// ��Ȳ�˸� ��ȭ���� ����
	::SendMessage( g_noticeHwnd2, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );

	// ����Ʈ �ڽ� ������ �߰�
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( theApp.m_komFileManager.GetMapNewMemberFile().begin()->first, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else
	{
		InsertKomList( theApp.m_komFileManager.GetMapNewMemberFile().begin()->first, theApp.m_komFileManager.GetMapOldMemberFile() );
	}	

	m_komList.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );

	m_memberList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );

	m_insertedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	m_deletedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
}

void CX2KomFileViewerDlg::OnBnClickedFindFile()
{
	CString strText;
	m_editFileFind.GetWindowText( strText );

	int iFindIndex = 0;
	std::wstring wstrMemberFileName = strText.GetBuffer();
	std::wstring wstrKomString;
	if( theApp.m_komFileManager.MemberFileFind( wstrMemberFileName, m_bIsKomParsingOnly, wstrKomString, iFindIndex ) == false )
	{
		MessageBox( L"�ش� ������ ã�� �� �����ϴ�!" );
		return;
	}

	// list �ҷ�����
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( wstrKomString, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else
	{
		InsertKomList( wstrKomString, theApp.m_komFileManager.GetMapOldMemberFile() );
	}

	// ã�� ���� �����ϱ�
	ListView_SetItemState (m_memberList.GetSafeHwnd(), iFindIndex, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
	m_memberList.EnsureVisible( iFindIndex, FALSE );
}

static HWND g_uncompressMBHwnd;

UINT WINAPI UncompressMessageBoxThread( LPVOID arg )
{
	CNoticeDlg noticeDlg;
	std::wstring wstrMessage = L"�����������Դϴ�";
	noticeDlg.ChangeText( wstrMessage );
	g_uncompressMBHwnd = noticeDlg.m_hWnd;
	noticeDlg.DoModal();
	return 0;
}

void CX2KomFileViewerDlg::OnBnClickedUncompress() // ����Ǯ��
{
	// ���� ��� ���
	WCHAR StartPath[128];
	WCHAR folder[128];
	
	if( m_bIsKomParsingOnly == false )			// �������� ������ �ƴ϶��
	{
		MessageBox( L"������ Ǯ�� �ִ� ��尡 �ƴմϴ�!" );
		return;
	}

	// ���� ���� ��� ����
	CUncompressDlg uncompressDlg;
	uncompressDlg.SetKomList( theApp.m_komFileManager.GetKomRealPathList() );
	if( uncompressDlg.DoModal() == IDCANCEL )
		return;

	// ���� ����
	if( BrowseFolder( this->m_hWnd, TEXT("���������� ������ �����ϼ���"), StartPath, folder ) == false )
		return;
	
	std::wstring wstrCurKomName;
	std::vector< std::wstring > vecSeletedFileList;	
	switch( uncompressDlg.GetCompressMode() )
	{
	case UM_SEL_FILE:
		{
			// ���õ� ���ҽ� ���� ���
			if( m_memberList.GetSelectedCount() != 0 )
			{
				CString strCurKomName = m_memberList.GetItemText( 0, 0 );
				wstrCurKomName = strCurKomName.GetBuffer();

				POSITION pos = m_memberList.GetFirstSelectedItemPosition();
				while( pos )
				{
					int nItem;
					nItem = m_memberList.GetNextSelectedItem( pos );
					CString strTemp = m_memberList.GetItemText( nItem, 1 );
					std::wstring wstrTemp = strTemp.GetBuffer();
					vecSeletedFileList.push_back( wstrTemp );
				}
			}
		}
		break;

	case UM_SEL_KOM:
		{
			// ���õ� KOM���� ���
			vecSeletedFileList = uncompressDlg.GetSeletedFileList();
		}
		break;

	default:
		break;
	}
	
	// ��Ȳ�˸� ������ ����
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, UncompressMessageBoxThread, (LPVOID)NULL, NULL, NULL );
	
	std::map< std::wstring, std::vector< std::wstring > > mapDummy;
	if( theApp.m_komFileManager.UncompressKomFile( std::wstring( folder ), vecSeletedFileList, wstrCurKomName, uncompressDlg.GetCompressMode(), false, mapDummy ) == false )
	{
		::SendMessage( g_uncompressMBHwnd, WM_CLOSE, 0, 0 );
		TerminateThread( hNoticeThread, 0 );

		MessageBox( L"���� Ǯ�� ����!" );
		return;
	}

	::SendMessage( g_uncompressMBHwnd, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );

	MessageBox( L"���� ���� �Ϸ�!" );
}

void CX2KomFileViewerDlg::OnBnClickedModifyKom()	// ���ҽ� ���Ƴ����
{
	CX2ModifyKomMemberDlg mkmDlg;
	mkmDlg.DoModal();
}
