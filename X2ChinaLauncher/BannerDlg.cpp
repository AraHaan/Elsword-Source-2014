// BannerDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "BannerDlg.h"
#include ".\bannerdlg.h"


// CBannerDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBannerDlg, CDialog)
CBannerDlg::CBannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBannerDlg::IDD, pParent)
{
}

CBannerDlg::~CBannerDlg()
{
}

BOOL CBannerDlg::OnInitDialog()
{
	//LONG temp = GetWindowLong( m_hWnd, GWL_EXSTYLE );
	//temp = temp & ~WS_EX_LAYERED;

	//SetWindowLong( m_hWnd, GWL_EXSTYLE, GetWindowLong( m_hWnd, GWL_EXSTYLE ) & ~WS_EX_LAYERED );

	return TRUE;
}

BOOL CBannerDlg::PreTranslateMessage(MSG* pMsg)
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

void CBannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBannerDlg, CDialog)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CBannerDlg �޽��� ó�����Դϴ�.

void CBannerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBannerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return FALSE;
	//return CDialog::OnEraseBkgnd(pDC);
}
