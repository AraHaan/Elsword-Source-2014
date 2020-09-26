// GCFileDiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCFileDiff.h"
#include "GCFileDiffDlg.h"
#include "EnumFile.h"
#include "KMsgBox.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffDlg dialog

CGCFileDiffDlg::CGCFileDiffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCFileDiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGCFileDiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCFileDiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGCFileDiffDlg)
	DDX_Control(pDX, IDC_EDIT_NEW_PATH, m_editNewPath);
	DDX_Control(pDX, IDC_EDIT_OLD_PATH, m_editOldPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGCFileDiffDlg, CDialog)
	//{{AFX_MSG_MAP(CGCFileDiffDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OLD_PATH, OnBtnOldPath)
	ON_BN_CLICKED(IDC_BTN_NEW_PATH, OnBtnNewPath)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffDlg message handlers

BOOL CGCFileDiffDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGCFileDiffDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGCFileDiffDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGCFileDiffDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//------------------------------------------------------------------------------
// �ۿ� �ҽ� ����.
//------------------------------------------------------------------------------

//******************************************************************************
// Function name     - GetFolder
// Description       - Get a folder path
// ���� ����������?  - 25.09.2000
// ��?�������������� - S. Sokolenko
// In                - strSelectedFolder -
//                         reference to string for store folder path
// Out               - lpszTitle - title for caption
//                     hwndOwner - reference to parent window 
//                     strRootFolder - root folder 
//                     strStartFolder - current foldet
// Return            - TRUE if user select OK, else FALSE.
//******************************************************************************

CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    TCHAR szDir[MAX_PATH];

    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        if (lpData)
        {
            strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
            SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
        }
        break;
    case BFFM_SELCHANGED:
        {
            if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir))
            {
                SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
            }
            break;
        }
    default:
        break;
    }

    return 0;
}

BOOL GetFolder(CString* strSelectedFolder,
               const char* lpszTitle,
               const HWND hwndOwner, 
               const char* strRootFolder, 
               const char* strStartFolder)
{
    char pszDisplayName[MAX_PATH];
    LPITEMIDLIST lpID;
    BROWSEINFOA bi;

    bi.hwndOwner = hwndOwner;
    if (strRootFolder == NULL)
    {
        bi.pidlRoot = NULL;
    }
    else
    {
        LPITEMIDLIST  pIdl = NULL;
        IShellFolder* pDesktopFolder;
        char          szPath[MAX_PATH];
        OLECHAR       olePath[MAX_PATH];
        ULONG         chEaten;
        ULONG         dwAttributes;

        strcpy(szPath, (LPCTSTR)strRootFolder);
        if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
        {
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
            pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
            pDesktopFolder->Release();
        }
        bi.pidlRoot = pIdl;
    }
    bi.pszDisplayName = pszDisplayName;
    bi.lpszTitle = lpszTitle;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
    bi.lpfn = BrowseCallbackProc;
    if (strStartFolder == NULL)
    {
        bi.lParam = FALSE;
    }
    else
    {
        strTmpPath.Format("%s", strStartFolder);
        bi.lParam = TRUE;
    }
    bi.iImage = NULL;
    lpID = SHBrowseForFolderA(&bi);
    if (lpID != NULL)
    {
        BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
        if (b == TRUE)
        {
            strSelectedFolder->Format("%s",pszDisplayName);
            return TRUE;
        }
    }
    else
    {
        strSelectedFolder->Empty();
    }
    return FALSE;
}

//------------------------------------------------------------------------------
// �ۿ� �ҽ� ��.
//------------------------------------------------------------------------------

void CGCFileDiffDlg::OnBtnOldPath(void)
{
    CString strFolderPath;
    if (TRUE == GetFolder(&strFolderPath,
                          "���� ��θ� ������ �ּ���.",
                          m_hWnd,
                          NULL,
                          NULL))
    {
        if (FALSE == strFolderPath.IsEmpty())
        {
            m_editOldPath.SetWindowText(strFolderPath);
        }
    }
}

void CGCFileDiffDlg::OnBtnNewPath(void)
{
    CString strFolderPath;
    if (TRUE == GetFolder(&strFolderPath,
                          "���� ��θ� ������ �ּ���.",
                          m_hWnd,
                          NULL,
                          NULL))
    {
        if (FALSE == strFolderPath.IsEmpty())
        {
            m_editNewPath.SetWindowText(strFolderPath);
        }
    }
}

void CGCFileDiffDlg::OnOk(void)
{
    CString cstrOldPath;
    CString cstrNewPath;

    m_editOldPath.GetWindowText(cstrOldPath);
    m_editNewPath.GetWindowText(cstrNewPath);

    std::string strOldPath = cstrOldPath;
    std::string strNewPath = cstrNewPath;

	
	int iError = CreateNewVersionFile( strOldPath, strNewPath );
	switch( iError )
	{
	case 0:
		{
			KMsgBox::Open("���ŵ� NewVer.xml ���� ������", "�Ϸ��߽��ϴ�.", m_hWnd, KMsgBox::MBOX_OK);
			CDialog::OnOK();
		} break;

	case 1:
		{
			KMsgBox::Open("���� ��ΰ� �߸��Ǿ����ϴ�.", "���� ��θ� �ٽ� �������ֽʽÿ�.", m_hWnd, KMsgBox::MBOX_OK);
		} break;

	case 2:
		{
			KMsgBox::Open("���ŵ� NewVer.xml ������", "������ �� �����ϴ�.", m_hWnd, KMsgBox::MBOX_OK);
		} break;

	case 3:
		{
			KMsgBox::Open("���� �ʱ�ȭ��", "����ϼ̽��ϴ�.", m_hWnd, KMsgBox::MBOX_OK);
		} break;
	}
}


int CGCFileDiffDlg::CreateNewVersionFile( std::string strOldPath, std::string strNewPath )
{
	if( true == strOldPath.empty() || true == strNewPath.empty() )
		return 1; 



	KEnumFile kef;


	if ('\\' != strOldPath[strOldPath.size() - 1]) strOldPath.append("\\");
	if ('\\' != strNewPath[strNewPath.size() - 1]) strNewPath.append("\\");


	// ���� ���� NewVer.xml ������ ��� ����.
	std::string strOldFileName = strOldPath;
	strOldFileName.append("NewVer.xml");

	// ���� ���� NewVer.xml ������ �����ϴ��� Ȯ���Ͽ�
	// ������ �ʱ�ȭ�� ������ �����.
	if (-1 == ::_access(strOldFileName.c_str(), 0))
	{
		if (IDCANCEL == KMsgBox::Open("���� ������ NewVer.xml�� �������.",
			"������ �ʱ�ȭ�ϰڽ��ϱ�?", m_hWnd,
			KMsgBox::MBOX_OKCANCEL))
		{
			return 3;
		}
	}

	// ���� ������ NewVer.xml�� �о� ����.
	TiXmlDocument xmlOldDoc_;
	TiXmlDocument xmlOldDoc(strOldFileName.c_str());
	if (false == xmlOldDoc_.LoadFile(strOldFileName.c_str()))
	{
		kef.CreateFileList(strOldPath.c_str(), xmlOldDoc);
	}
	else
	{
		xmlOldDoc.LoadFile();
	}

	// ��ȿ�� üũ�� �߰�.
	KEnumFile::Signing(xmlOldDoc);
	xmlOldDoc.SaveFile();

	// ó�� ������ �˷��ִ� ���̾�α� ����.
	KMsgBox::Open("���ŵ� NewVer.xml ���� ��", "��ٷ� �ֽʽÿ�.", m_hWnd);

	// �� ���� NewVer.xml ������ ��� ����.
	std::string strNewFileName = strNewPath;
	strNewFileName.append("NewVer.xml");

	// �̹� �� ���� NewVer.xml�� �����ϸ� �����.
	if (0 == ::_access(strNewFileName.c_str(), 0))
	{
		::DeleteFile(strNewFileName.c_str());
	}

	// �� ���� ��ο��� file list�� ����.
	TiXmlDocument xmlNewDoc;
	kef.CreateFileList(strNewPath.c_str(), xmlNewDoc);

	// ���� �� ���Ϸκ��� patch ������ ���ŵ� updated list�� ����
	TiXmlDocument xmlUpdatedDoc(strNewFileName.c_str());
	if (true == kef.CompareFileVersion(strOldPath.c_str(),
		xmlOldDoc,
		strNewPath.c_str(),
		xmlNewDoc,
		xmlUpdatedDoc))
	{
		KEnumFile::Signing(xmlUpdatedDoc);
		xmlUpdatedDoc.SaveFile();
	}
	else
	{
		// newver.xml ���� ���� ����
		KMsgBox::Close();
		return 2;
	}

	// �Ϸ������� �˸��� �����Ѵ�.
	KMsgBox::Close();
	return 0;

}