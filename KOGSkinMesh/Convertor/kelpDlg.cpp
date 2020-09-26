// kelpDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "kelp.h"
#include "kelpDlg.h"
#include ".\kelpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL g_bThread = false;
CkelpDlg *g_cwnd;
std::vector<CString> m_convertFileList;

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CkelpDlg ��ȭ ����



CkelpDlg::CkelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CkelpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CkelpDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_LIST1, m_fileListBox);
}

BEGIN_MESSAGE_MAP(CkelpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
   ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
   ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkList1)
   ON_BN_CLICKED(IDOK, OnBnClickedOk)
   ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CkelpDlg �޽��� ó����

BOOL CkelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	GetDrive();

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CkelpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CkelpDlg::OnPaint() 
{
   outputList();

	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

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
HCURSOR CkelpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CkelpDlg::GetDrive()
{
   // �ý����� ����̺� ��Ʈ ����ũ�� ��´�
   DWORD dwDrive = ::GetLogicalDrives();
   int i, nDriveCount = 0;

   // �ý����� ����̺� ������ ����
   for(i = 0; i < 32; i++)
   {
      if(dwDrive & ((DWORD)0x01 << i))nDriveCount++;
   }

   // ����̺� �̸��� ���� ���۸� �����
   m_pDriveName = new char[nDriveCount * 4 + 1];
   // ����̺� �̸� �迭�� ��´�
   ::GetLogicalDriveStrings(nDriveCount * 4 + 1, m_pDriveName);

   UINT nDriveType;
   SHFILEINFO ShellFileInfo;

   m_nDriveCnt = nDriveCount;
   // ��� ����̺꿡 ����
   for(i = 0; i < nDriveCount; i++)
   {
      // ����̺��� ������ ��´�
      ::SHGetFileInfo(m_pDriveName + i * 4, FILE_ATTRIBUTE_DIRECTORY,
         &ShellFileInfo, sizeof(SHFILEINFO),
         SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

      nDriveType = ::GetDriveType(m_pDriveName + i * 4);
      if(nDriveType == DRIVE_REMOVABLE)
      {
         if(stricmp(m_pDriveName + i * 4, "A:\\") == 0) continue;
         if(stricmp(m_pDriveName + i * 4, "B:\\") == 0) continue;

         // pDriveName�� ����̺� �̸��� �����Ƿ�
         // ���⼭ USB ������ġ�� ������ �����ϰų� �۾��� �մϴ�.
      }
   }

   // ����̺� �̸� ���۸� �����Ѵ�
   //delete[] pDriveName;

   for(i=0; i<nDriveCount; ++i)
      m_fileList.push_back(m_pDriveName+i*4);
}

int CkelpDlg::MoveDirectroy(CString &strPath)
{
   BOOL ret;
   CFileFind ff;
   CString efound, efront, epath;  //�迭����
   epath = strPath+"\\";

   int ei = 0, ecount, enchanged=1;

   ecount = 0;
   m_fileList.clear();

   BOOL ebWorking = ff.FindFile( epath+CString(_T("\\*")) );

   if(m_pathStack.size() >= 2)
   {   
      m_fileList.push_back("..");
      ++ecount;
   }

   while (ebWorking)
   {      
      ebWorking = ff.FindNextFile();  
      efront = ff.GetFileName();   
      if(strcmp(efront.GetString() + strlen(efront.GetString())-2, ".x") == 0 || 
         strcmp(efront.GetString() + strlen(efront.GetString())-2, ".X") == 0 ||
         strchr(efront.GetString(), '.') == NULL)
      {      
         efound = epath+efront;
         m_fileList.push_back(efound);
         ++ecount;
      }
      ++ei;
   }      

   return ecount;
}

void CkelpDlg::outputList()
{   
   int cnt;

   cnt = m_fileListBox.GetCount();

   for(int i=cnt; i>=0; --i)
      m_fileListBox.DeleteString(i);

   if(m_pathStack.size() == 1)
      m_fileListBox.AddString("..");

   for(int i=0; i<m_nDriveCnt; ++i)
      m_fileListBox.AddString(m_fileList[i]);
}

void CkelpDlg::OnLbnSelchangeList1()
{
   // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
   
}

void CkelpDlg::OnLbnDblclkList1()
{
   // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
   CString getStr;
   int sel;

   m_oldcnt = m_nDriveCnt;

   sel = m_fileListBox.GetCurSel();
   m_fileListBox.GetText(sel, getStr);

   m_fileList.clear();

   if(strcmp(getStr.GetString(), "..") == 0) // ����������...
   {      
      if(m_pathStack.size() <= 1)
      {         
         m_pathStack.pop();
         GetDrive();
      }
      else
      {      
         m_pathStack.pop();
         m_currentPath = m_pathStack.top();
         m_pathStack.pop();
         m_nDriveCnt = MoveDirectroy(m_currentPath);
      }
      
   }
   else if(strcmp(getStr.GetString(), ".") == 0)
   {
      // ���� ���丮
   }
   else
   {      
      m_currentPath = getStr;
      m_pathStack.push(m_currentPath);

      m_nDriveCnt = MoveDirectroy(m_currentPath);
   }
     

   GetDlgItem(IDC_STATIC)->SetWindowText(m_currentPath.GetString());
   OnPaint();



   //m_fileListBox.UpdateData();
   //m_fileListBox.Invalidate();
   //MessageBox(getStr.GetString(), "info");
}

void CkelpDlg::OnBnClickedOk()
{
   // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
   //OnOK();
   int ret;
   int count = 0;
   
   CString strSel;

   //GetDlgItem(IDOK)->EnableWindow(false);

   for(int i=0; i< m_fileListBox.GetCount(); ++i)
   {
      ret = m_fileListBox.GetSel(i);
      if(ret)
      {
         m_fileListBox.GetText(i, strSel);

         if(strcmp(strSel.GetString() + strlen(strSel.GetString())-2, ".x") == 0 || 
            strcmp(strSel.GetString() + strlen(strSel.GetString())-2, ".X") == 0)
         {
#if 0 
            ++count;
            if(count == 1)
            {
                char buf[1024];
                sprintf(buf, "%s", m_currentPath);
                strcat(buf, "Result");
                CreateDirectory(buf, NULL);
            }
#endif
            m_convertFileList.push_back(strSel);
         }
         else
         {
            addFolder(strSel);
         }
      }
   }   

   g_cwnd = this;
    
   AfxBeginThread(threadFunc, NULL);
   g_bThread = true;
   // ��ȯ�� ���ϵ� ���
      
#if 0 
   m_fileList.clear();   
   for(int i=0; i<m_convertFileList.size(); ++i)
   {
      m_fileList.push_back(m_convertFileList[i]);
   }

   m_nDriveCnt = m_convertFileList.size();
   OnPaint();
#endif
   
   m_nDriveCnt = m_convertFileList.size();   

   //m_nDriveCnt = MoveDirectroy(m_currentPath);
}

void CkelpDlg::addFolder(CString strPath)
{
   BOOL ret;
   CFileFind ff;
   CString efound, efront, epath;  //�迭����
   epath = strPath+"\\";
   int count;
   int ei = 0, ecount, enchanged=1;

   count = ecount = 0;  

   BOOL ebWorking = ff.FindFile( epath+CString(_T("\\*")) );   

   while (ebWorking)
   {      
      ebWorking = ff.FindNextFile();  
      efront = ff.GetFileName();
      efound = epath+efront;

      if(strcmp(efront.GetString() + strlen(efront.GetString())-2, ".x") == 0 || 
         strcmp(efront.GetString() + strlen(efront.GetString())-2, ".X") == 0)         
      {            
#if 0 
          ++count;
          if(count == 1)
          {
              char buf[1024];
              sprintf(buf, "%s", epath);
              strcat(buf, "Result");
              CreateDirectory(buf, NULL);
          }
#endif

         m_convertFileList.push_back(efound);         
      }
      else if(strchr(efront.GetString(), '.') == NULL)
      {
         addFolder(efound = epath+efront);
      }

      ++ei;
   }      
}

UINT threadFunc(LPVOID ss)
{
   char strCmd[1024];

   for(int i=0; i<m_convertFileList.size(); ++i)
   {
      if(g_bThread)
      {      
        sprintf(strCmd, "%s [%d]", m_convertFileList[i].GetString(), i+1);
        g_cwnd->GetDlgItem(IDC_STATIC)->SetWindowText(strCmd);
        sprintf(strCmd, "KOGSkinMesh.exe %s %s", m_convertFileList[i].GetString(), m_convertFileList[i].GetString());
        system(strCmd);
      }
      else
          return 0;
   }

   m_convertFileList.clear();
   return 0;
}


void CkelpDlg::OnBnClickedCancel()
{
   // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
   if(g_bThread)
   {
      g_bThread = false;
      //AfxEndThread(0);
      Sleep(500);
   }
   OnCancel();
}
