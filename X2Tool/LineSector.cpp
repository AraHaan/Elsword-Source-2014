// LineSector.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "LineSector.h"
#include "afxdialogex.h"


// CLineSector ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLineSector, CDialogEx)

CLineSector::CLineSector(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineSector::IDD, pParent)
{

}

CLineSector::~CLineSector()
{
}

BOOL CLineSector::OnInitDialog()
{
	GetDlgItem(IDC_EDIT_SECTOR_WIDTH)->SetWindowTextW(L"768");
	GetDlgItem(IDC_EDIT_SECTOR_HEIGHT)->SetWindowTextW(L"768");
	GetDlgItem(IDC_EDIT_SECTOR_DEPTH)->SetWindowTextW(L"768");

	return TRUE;
}

void CLineSector::OnShowWindow( BOOL bShow, UINT nStatus )
{	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bTileRender = bShow == TRUE ? true : false;
}

void CLineSector::OnClose()
{
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bTileRender = false;

	CDialogEx::OnClose();
}

void CLineSector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLineSector, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_SECTOR_WIDTH, &CLineSector::OnEnChangeEditSectorWidth)
	ON_EN_CHANGE(IDC_EDIT_SECTOR_HEIGHT, &CLineSector::OnEnChangeEditSectorHeight)
	ON_EN_CHANGE(IDC_EDIT_SECTOR_DEPTH, &CLineSector::OnEnChangeEditSectorDepth)
	ON_BN_CLICKED(IDC_BUTTON1, &CLineSector::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLineSector �޽��� ó�����Դϴ�.


void CLineSector::OnEnChangeEditSectorWidth()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	GetDlgItem(IDC_EDIT_SECTOR_WIDTH)->GetWindowText(csTemp);	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iSectorWidth = _ttoi(csTemp.GetString());
}


void CLineSector::OnEnChangeEditSectorHeight()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	GetDlgItem(IDC_EDIT_SECTOR_HEIGHT)->GetWindowText(csTemp);	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iSectorHeight = _ttoi(csTemp.GetString());
}


void CLineSector::OnEnChangeEditSectorDepth()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	GetDlgItem(IDC_EDIT_SECTOR_DEPTH)->GetWindowText(csTemp);	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iSectorDepth = _ttoi(csTemp.GetString());
}


void CLineSector::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->CreateSector();

	wstring wstrSectorInfo = pMain->GetSectorInfo();
	GetDlgItem(IDC_EDIT_SECTOR_INFO)->SetWindowTextW(wstrSectorInfo.c_str());
}
