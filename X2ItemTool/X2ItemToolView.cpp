// X2ItemToolView.cpp : CX2ItemToolView Ŭ������ ����
//

#include "stdafx.h"
#include "X2ItemTool.h"

#include "X2ItemToolDoc.h"
#include "X2ItemToolView.h"
#include ".\x2itemtoolview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ItemToolView

IMPLEMENT_DYNCREATE(CX2ItemToolView, CView)

BEGIN_MESSAGE_MAP(CX2ItemToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CX2ItemToolView ����/�Ҹ�

CX2ItemToolView::CX2ItemToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CX2ItemToolView::~CX2ItemToolView()
{
}

BOOL CX2ItemToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CX2ItemToolView �׸���

void CX2ItemToolView::OnDraw(CDC* /*pDC*/)
{
	CX2ItemToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CX2ItemToolView �μ�

BOOL CX2ItemToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CX2ItemToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CX2ItemToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CX2ItemToolView ����

#ifdef _DEBUG
void CX2ItemToolView::AssertValid() const
{
	CView::AssertValid();
}

void CX2ItemToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CX2ItemToolDoc* CX2ItemToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CX2ItemToolDoc)));
	return (CX2ItemToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CX2ItemToolView �޽��� ó����

void CX2ItemToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_MainDlg.DestroyWindow();
}

int CX2ItemToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_MainDlg.Create( IDD_MAIN_DIALOG, this );

	return 0;
}

void CX2ItemToolView::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	//this->UpdateData();

	//CFile f;

	WCHAR strFilter[] = { L"lua Files (*.lua)|*.lua|All Files (*.*)|*.*||" };
	CFileDialog fileDlg( true, L".lua", NULL, 0, strFilter);

	if( fileDlg.DoModal() == IDOK )
	{
		CString fileName = fileDlg.GetFileName();
		LPWSTR wszFileName = fileName.GetBuffer( fileName.GetLength() );

		m_MainDlg.OpenLuaFile( wszFileName );
		GetDocument()->SetTitle( wszFileName );

		//g_pItemToolCommon->GetItemManager()->OpenScriptFile( wszFileName );
		

		//f.Open(FileDlg.GetFileName(), CFile::modeCreate | CFile::modeWrite);
		//CArchive ar(&f, CArchive::store);

		//ar << m_Make << m_Model << m_Year << m_Mileage << m_Owner;
		//ar.Close();
	}

	//f.Close();
}

void CX2ItemToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_MainDlg.MoveWindow( 0, 0, cx, cy, true );
	m_MainDlg.ResizeGridCtrl( cx, cy );
	m_MainDlg.Invalidate();
	//m_MainDlg.OnSize( nType, cx, cy );
}
