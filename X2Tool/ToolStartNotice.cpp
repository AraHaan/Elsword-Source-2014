// ToolStartNotice.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "ToolStartNotice.h"
#include "afxdialogex.h"


// CToolStartNotice ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CToolStartNotice, CDialogEx)

CToolStartNotice::CToolStartNotice(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolStartNotice::IDD, pParent)
{

}

CToolStartNotice::~CToolStartNotice()
{
}

void CToolStartNotice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToolStartNotice, CDialogEx)
	ON_BN_CLICKED(IDOKNOTICE, &CToolStartNotice::OnBnClickedOknotice)
END_MESSAGE_MAP()


// CToolStartNotice �޽��� ó�����Դϴ�.


void CToolStartNotice::OnBnClickedOknotice()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	MessageBox(L"���� ���� ������...\n������ �̰� �ƴ϶���...\n\n���� �˷����ڸ�, ESC�� ������!", L"�˸�", MB_OK);
}
