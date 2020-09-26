// X2EncryptFileViewerView.cpp : CX2EncryptFileViewerView Ŭ������ ����
//

#include "stdafx.h"
#include "X2EncryptFileViewer.h"

#include "X2EncryptFileViewerDoc.h"
#include "X2EncryptFileViewerView.h"
#include "FileCrypt.h"
#include <fstream>
#include <string>
#include ".\x2encryptfileviewerview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2EncryptFileViewerView

IMPLEMENT_DYNCREATE(CX2EncryptFileViewerView, CEditView)

BEGIN_MESSAGE_MAP(CX2EncryptFileViewerView, CEditView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_DECRYPT_SAVE, OnDecryptSave)
END_MESSAGE_MAP()

// CX2EncryptFileViewerView ����/�Ҹ�

CX2EncryptFileViewerView::CX2EncryptFileViewerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_cBufPointer = NULL;
}

CX2EncryptFileViewerView::~CX2EncryptFileViewerView()
{
	if( m_cBufPointer != NULL )
		delete []m_cBufPointer;
}

void CX2EncryptFileViewerView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	DragAcceptFiles();
}

BOOL CX2EncryptFileViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// �ڵ� �ٹٲ��� ����մϴ�.

	return bPreCreated;
}


// CX2EncryptFileViewerView �μ�

BOOL CX2EncryptFileViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� CEditView �غ�
	return CEditView::OnPreparePrinting(pInfo);
}

void CX2EncryptFileViewerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// �⺻ CEditView ���� �μ�
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CX2EncryptFileViewerView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// �⺻ CEditView ���� �μ�
	CEditView::OnEndPrinting(pDC, pInfo);
}

void CX2EncryptFileViewerView::OnDropFiles( HDROP hDropInfo )
{
	int nFiles;
	char szPathName[MAX_PATH];
	CString strFileName;

	// ��ӵ� ������ ����
	nFiles = ::DragQueryFileA( hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH );

	for(int i = nFiles-1 ; i >= 0; i--)
	{
		// ������ ��� ����
		::DragQueryFileA( hDropInfo, i, szPathName, MAX_PATH );		
	}

	::DragFinish( hDropInfo );

	//////////////////////////////////////////////////////////////////////////

	std::ifstream ifs;
	ifs.open( szPathName, std::ios::binary );
	if( ifs.is_open() == false )
	{
		AfxMessageBox( "���� ���� ����!" );
		return;
	}

	std::string strBuf;
	char cReadCur;
	while( !ifs.eof() )
	{
		ifs.get( cReadCur );
		strBuf.append( 1, cReadCur );
	}	

	if( m_cBufPointer != NULL )
		delete []m_cBufPointer;

	m_cBufPointer = CFileCrypt::FileDecrypt( theApp.GetEncryptionKeyList(), strBuf.c_str(), strBuf.size() );

	//std::wstring wstrBuf;
	//ConvertCharToWCHAR( wstrBuf, m_cBufPointer );

	GetEditCtrl().SetWindowText( m_cBufPointer );

	//////////////////////////////////////////////////////////////////////////	

	CEditView::OnDropFiles( hDropInfo );
}

// CX2EncryptFileViewerView �޽��� ó����
void CX2EncryptFileViewerView::OnDecryptSave()
{
	// ���� ��� ���
	CHAR StartPath[128];
	CHAR folder[128];

	// ���� ����
	if( BrowseFolder( this->m_hWnd, TEXT("������ ������ �����ϼ���"), StartPath, folder ) == false )
		return;

	std::ofstream ofs;	
	std::string strEncryptedFileName = StartPath;
	strEncryptedFileName += "\\test.lua";

	//std::string strTemp;
	//ConvertWCHARToChar( strTemp, strEncryptedFileName.c_str() );

	ofs.open( strEncryptedFileName.c_str(), std::ios::binary );

	if( ofs.is_open() == false || m_cBufPointer == NULL )
		return;

	ofs.write( m_cBufPointer, strlen( m_cBufPointer ) );

	ofs.close();

	MessageBox( "���强��" );
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

bool CX2EncryptFileViewerView::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
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


// CX2EncryptFileViewerView ����

#ifdef _DEBUG
void CX2EncryptFileViewerView::AssertValid() const
{
	CEditView::AssertValid();
}

void CX2EncryptFileViewerView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CX2EncryptFileViewerDoc* CX2EncryptFileViewerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CX2EncryptFileViewerDoc)));
	return (CX2EncryptFileViewerDoc*)m_pDocument;
}
#endif //_DEBUG



