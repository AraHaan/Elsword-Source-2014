// MainDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2ItemTool.h"
#include "MainDlg.h"
#include ".\maindlg.h"


// CMainDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDlg �޽��� ó�����Դϴ�.

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	g_pItemToolCommon = new CX2ItemToolCommon;


	//GridCtrl Setting
	GetDlgItem(IDC_GRID_CTRL)->GetWindowRect(&m_rcGrid);
	ScreenToClient(&m_rcGrid);
	m_GridCtrl.Create(m_rcGrid, this, IDC_GRID_CTRL, WS_CHILD | WS_BORDER| WS_TABSTOP | WS_VISIBLE);
	m_GridCtrl.SetEditable(TRUE);
	m_GridCtrl.EnableDragAndDrop(TRUE);
	m_GridCtrl.SetTextBkColor(RGB(255, 255, 225));
	m_GridCtrl.SetRowResize(TRUE);
	m_GridCtrl.SetColumnResize(TRUE);
	m_GridCtrl.SetListMode(FALSE);
	m_GridCtrl.SetSingleRowSelection(FALSE);
	m_GridCtrl.SetHeaderSort(FALSE);




	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	SAFE_DELETE( g_pItemToolCommon );
}




void CMainDlg::OpenLuaFile( const WCHAR* wszFileName )
{
	g_pItemToolCommon->GetItemManager()->OpenScriptFile( wszFileName );


	CX2ItemManager* pItemManager = g_pItemToolCommon->GetItemManager();
	map<int, CX2Item::ItemTemplet*>& mapItemTemplet = pItemManager->GetItemTempletIDMap();
	map<int, CX2Item::ItemTemplet*>::iterator it;


	int nRow = (int)mapItemTemplet.size() + 1;
	m_GridCtrl.SetRowCount( nRow );
	m_GridCtrl.SetColumnCount( 50 );


	int iRow = 1;

	for( it=mapItemTemplet.begin(); it!=mapItemTemplet.end(); it++ )
	{
		CX2Item::ItemTemplet* pItemTemplet = (CX2Item::ItemTemplet*) it->second;
        
		for( int iCol=1; iCol<4; iCol++ )
		{
			//m_GridCtrl.SetCellType( iRow, iCol, RUNTIME_CLASS(CGridCell) );
			m_GridCtrl.SetItemText( iRow, iCol, pItemTemplet->m_Name.c_str() );
			//m_GridCtrl.SetItemState( iRow, iCol, m_GridCtrl.GetItemState(iRow,iCol) & ~GVIS_READONLY );
			m_GridCtrl.Invalidate();
		}

		iRow++;
	}
}


void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//m_GridCtrl.MoveWindow( 0, 0, cx, cy, true );
}

void CMainDlg::ResizeGridCtrl(int cx, int cy)
{
	m_GridCtrl.MoveWindow( 0, 0, cx, cy, true );
}