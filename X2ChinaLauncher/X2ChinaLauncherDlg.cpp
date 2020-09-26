// X2ChinaLauncherDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "X2ChinaLauncherDlg.h"
#include "PngWindow/PngWindow.h"
#include "ServerManager.h"
#include "patchthread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ChinaLauncherDlg ��ȭ ����

void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits);

CX2ChinaLauncherDlg::CX2ChinaLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX2ChinaLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CX2ChinaLauncherDlg::PreTranslateMessage(MSG* pMsg)
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
				m_MainDlg.PreTranslateMessage(pMsg);
				return TRUE;
			}
			break;
	}

	//alt+print screen ����
	if( GetKeyState(VK_MENU) && pMsg->wParam == VK_SNAPSHOT )
	{
		::OpenClipboard(NULL);
		::EmptyClipboard();
		::CloseClipboard();
	}

	return CDialog::PreTranslateMessage(pMsg);	
}

void CX2ChinaLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CX2ChinaLauncherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOMMAND()
	ON_WM_NCHITTEST()
	ON_WM_MOVING()
//	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CX2ChinaLauncherDlg �޽��� ó����

BOOL CX2ChinaLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ���� ���� ������ó��
	WindowAlphaBlendFromPNG("res/LauncherBackground.png", m_hWnd);
	//DragAcceptFiles();

	// �׸��� ������ ��ǥ�� ������ ó��!
	m_MainDlg.Create(CMainDlg::IDD);
	//m_MainDlg.ShowWindow(SW_SHOW);

	CString strCaption;
	strCaption.LoadString(IDS_ELSWORD);
	SetWindowText( strCaption );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CX2ChinaLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

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
HCURSOR CX2ChinaLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBITMAP CX2ChinaLauncherDlg::Create32BitBitmap( HDC hDCSource, int cx, int cy )
{
	// Create 32-bit bitmap
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = cx;
	bi.bmiHeader.biHeight = cy;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	return ::CreateDIBSection( hDCSource, &bi, DIB_RGB_COLORS, NULL, NULL, 0);
}

void CX2ChinaLauncherDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// ���ϵ������� ũ�� ����
	if (m_MainDlg)
		m_MainDlg.SetWindowPos(&wndTop, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, SWP_SHOWWINDOW);
}

void CX2ChinaLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnSysCommand(nID, lParam);
}

LRESULT CX2ChinaLauncherDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return HTCAPTION;
}

void CX2ChinaLauncherDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CX2ChinaLauncherDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CX2ChinaLauncherDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CX2ChinaLauncherDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
