// NoticeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "NoticeDlg.h"


// CNoticeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNoticeDlg, CDialog)
CNoticeDlg::CNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeDlg::IDD, pParent)
{
}

CNoticeDlg::~CNoticeDlg()
{
}

BOOL CNoticeDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
		//alt�� ��������
	case WM_SYSKEYDOWN:
		{
			if(pMsg->wParam == VK_F4)
				return TRUE;
		}
		break;
		// Ű�� ��������
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
			// ����Ű\tab 
		case VK_RETURN:
			return TRUE;

			// ESCŰ
		case VK_ESCAPE:
			return TRUE;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoticeDlg, CDialog)
END_MESSAGE_MAP()


// CNoticeDlg �޽��� ó�����Դϴ�.
