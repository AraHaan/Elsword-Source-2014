// TriggerFunc.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "TriggerFunc.h"
#include "afxdialogex.h"


// CTriggerFunc ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTriggerFunc, CDialogEx)

CTriggerFunc::CTriggerFunc(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTriggerFunc::IDD, pParent)
{

}

CTriggerFunc::~CTriggerFunc()
{
}

void CTriggerFunc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CTriggerFunc::SetFunctionData(wstring wstrFunctionData)
{
	//m_richFunction.SetWindowText(wstrFunctionData.c_str());
	GetDlgItem(IDC_EDIT_TRIGGER_FUNC)->SetWindowText(wstrFunctionData.c_str());
}

BEGIN_MESSAGE_MAP(CTriggerFunc, CDialogEx)
END_MESSAGE_MAP()


// CTriggerFunc �޽��� ó�����Դϴ�.
