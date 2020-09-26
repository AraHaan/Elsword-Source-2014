// BugTrapWinServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BugTrapWinServer.h"
#include "BugTrapWinServerDlg.h"
#include ".\bugtrapwinserverdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define countof(arr) (sizeof(arr)/sizeof((arr)[0]))


static int GetTextLenEx(CListBox& box, LPCTSTR lpszText)
{
    CSize size;
    CDC *pDC = box.GetDC();

    // ���� ����Ʈ�ڽ��� ��Ʈ�� ���� DC�� �����Ų��.
    CFont* pOld = pDC->SelectObject(box.GetFont());

    // ��Ÿ�Ͽ� ���� �ణ�� �����V ���̰� �ִ�.
    if ((box.GetStyle() & LBS_USETABSTOPS) == 0)
    {
        size = pDC->GetTextExtent(lpszText, _tcslen(lpszText));
        size.cx += 3;
    }
    else
    {
        size = pDC->GetTabbedTextExtent(lpszText, _tcslen(lpszText), 0, NULL);
        size.cx += 2;
    }
    pDC->SelectObject(pOld);
    box.ReleaseDC(pDC);

    // ���� ���ڿ��� Pixel ������ �ѱ��.
    return size.cx;
}



bool resultSort(const LOG_RESULT *fi, const LOG_RESULT *se)
{
    if(fi->count > se->count)
        return true;

    return false;
}


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


// CBugTrapWinServerDlg ��ȭ ����



CBugTrapWinServerDlg::CBugTrapWinServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBugTrapWinServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBugTrapWinServerDlg::~CBugTrapWinServerDlg()
{
    releaseFileNameList();
    ReleaseMap();
    ReleaseResult();
    ReleaseLineInfo();
}

void CBugTrapWinServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_fNameList);
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_progressText);
    DDX_Control(pDX, IDC_STATIC_PROGRESS2, m_textProgress);
    DDX_Control(pDX, IDC_EDIT1, m_textAddress);
    DDX_Control(pDX, IDC_LIST_CRASH_LIST, m_crashList);
    DDX_Control(pDX, IDC_LIST_CRASH_SUBLIST, m_crashSubList);
    DDX_Control(pDX, IDC_STATIC_VERSION, m_versionText);
    DDX_Control(pDX, IDC_EDIT2, m_bugReportText);
    DDX_Control(pDX, IDC_STATIC_VERSION2, m_logFileText);
}

BEGIN_MESSAGE_MAP(CBugTrapWinServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
    ON_LBN_SELCHANGE(IDC_LIST_CRASH_LIST, OnLbnSelchangeListCrashList)
    ON_LBN_DBLCLK(IDC_LIST_CRASH_SUBLIST, OnLbnDblclkListCrashSublist)
    ON_LBN_SELCHANGE(IDC_LIST_CRASH_SUBLIST, OnLbnSelchangeListCrashSublist)
END_MESSAGE_MAP()


// CBugTrapWinServerDlg �޽��� ó����

BOOL CBugTrapWinServerDlg::OnInitDialog()
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
	
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CBugTrapWinServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBugTrapWinServerDlg::OnPaint() 
{
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
HCURSOR CBugTrapWinServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBugTrapWinServerDlg::releaseFileNameList()
{
    for(unsigned int i=0; i<m_vecFileName.size(); ++i)
    {
        if(m_vecFileName[i] != NULL)
            delete m_vecFileName[i];
    }

    m_vecFileName.clear();
}


std::string CBugTrapWinServerDlg::GetUndecoratedFunctionName(string m_strFunction) const
{
    /*
    * Name decoration usually refers to C++ naming conventions, but can apply
    * to a number of C cases as well. By default, C++ uses the function name,
    * parameters, and return type to create a linker name for the function.
    * Consider the following function:
    *
    * void CALLTYPE test(void)
    *
    * The following table shows the linker name for various calling conventions.
    *
    *  Calling convention                          | extern "C" or .c file | .cpp, .cxx or /TP
    * ---------------------------------------------+-----------------------+-------------------
    *  C naming convention (__cdecl)               | _test                 | ?test@@ZAXXZ
    *  Fast-call naming convention (__fastcall)    | @test@0               | ?test@@YIXXZ
    *  Standard Call naming convention (__stdcall) | _test@0               | ?test@@YGXXZ
    */
    char chFirstChar = m_strFunction[0];
    if (chFirstChar == '_' || chFirstChar == '@')
    {
        std::string strUndecoratedName, strDecoratedName = m_strFunction.substr(1);
        size_t nAtPos = strDecoratedName.find('@');
        if (nAtPos != std::string::npos)
        {
            strDecoratedName.resize(nAtPos);
            if (chFirstChar == '_')
                strUndecoratedName = "__stdcall ";
            else /* if (chFirstChar == '@') */
                strUndecoratedName = "__fastcall ";
        }
        else
            strUndecoratedName = "__cdecl ";
        strUndecoratedName += strDecoratedName;
        strUndecoratedName += "()";
        return strUndecoratedName;
    }
    else
    {
        CHAR szNameBuffer[2048];
        if (UnDecorateSymbolName(m_strFunction.c_str(), szNameBuffer, countof(szNameBuffer), UNDNAME_COMPLETE))
            return std::string(szNameBuffer);
        else
            return m_strFunction;
    }
}


void CBugTrapWinServerDlg::OnBnClickedOk()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //OnOK();

    // �Ľ��� ���׷α� ������ �����Ѵ�.
    BROWSEINFO  biBrowseInfo;
    TCHAR   szDisplayName[MAX_PATH];
    CString   strInitFolder;
    LPITEMIDLIST lpItemIDList;



    // BROWSEINFO ����ü ���� : ��Ÿ�� ����
    ZeroMemory(&biBrowseInfo, sizeof(biBrowseInfo));
    biBrowseInfo.hwndOwner  = AfxGetMainWnd ()->GetSafeHwnd();
    biBrowseInfo.pidlRoot  = NULL;
    biBrowseInfo.pszDisplayName = szDisplayName;
    biBrowseInfo.lpszTitle  = "Select X2 BugLog folder.";
    // biBrowseInfo.lpfn   = BrowseCallbackProc; // �ݹ��Լ�, �ٷ� ���ʿ� ������.
    biBrowseInfo.ulFlags  = 0; // �Լ��� ���޵Ǿ��� ����.
    strInitFolder    = AfxGetApp()->GetProfileString(_T("FolderPath"), _T("LogFolder"), _T("C:\\"));
    biBrowseInfo.lParam   = (LPARAM)(LPCTSTR)strInitFolder; // �������� ��ȭ���ڰ� �ʱ�ȭ�ɶ� ���õǴ� �������� ����
    biBrowseInfo.iImage   = 0;

    // ���̾�α� ǥ�� & ������ ������ ���� itemIDList ȹ��
    lpItemIDList    = SHBrowseForFolder(&biBrowseInfo);

    if (lpItemIDList != NULL)
    {
        // ���ð�� ȹ��
        HANDLE hSrch;
        WIN32_FIND_DATA wfd;
        BOOL bResult = TRUE;
        char szPath[MAX_PATH];
        char szSear[MAX_PATH];
        char fname[MAX_PATH];
        char szName[MAX_PATH], szExt[10];
        char szCurrentDir[MAX_PATH];
        string resultLog;

        SHGetPathFromIDList(lpItemIDList, szPath);  // �н��� ���´�.

        // ���� ����Ʈ�� �ʱ�ȭ�Ѵ�.        
        releaseFileNameList();
        ReleaseMap();
        ReleaseResult();  
        ReleaseLineInfo();
        ClearListBox();

        // ������ ��θ� �����Ѵ�.
        m_strFullPath = szPath;

        // �������� �����´�.
        char *ptVersion;
        int offsetVersion = strlen(szPath) -2;
        while(szPath[offsetVersion] != '\\')
        {
            --offsetVersion;
        }

        ptVersion = szPath + offsetVersion+1;
        m_versionText.SetWindowText(ptVersion);

        // ���õ� ��ο��� MAP������ ã�´�.
        bool bFindMap = false;

        bResult = FALSE;
        sprintf(wfd.cFileName, "");        
        strcpy(szSear, szPath);
        strcat(szSear, "\\X2.map");
        
        while(!bFindMap)
        {
            hSrch = FindFirstFile(szSear, &wfd);
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // ���丮
            }
            else
            {
                // ����
                wsprintf(fname, "%s", wfd.cFileName);
                if(strcmp(fname, "X2.map") == 0)
                {
                    bFindMap = true;
                }
                else
                {
                    MessageBox("X2.map������ �������� �ʽ��ϴ�.", "Info!", MB_OK);
                    return;
                }

            }
        }

        // map ������ �ε��Ѵ�.
        if(LoadMapFile(szSear) == false)
        {
            MessageBox("X2.map������ �ε��� �� �����ϴ�.", "Error!", MB_OK);
            return;
        }       

        // ��������ڷḦ �ε��Ѵ�.
        LoadTotalLog(szPath);

        // ���õ� ��ο��� ����� �α����ϵ��� ã�Ƽ� �����Ѵ�.
        bResult = TRUE;
        strcpy(szSear, szPath);
        strcat(szSear, "\\*.zip");
        hSrch = FindFirstFile(szSear, &wfd);
        while(bResult)
        {
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // ���丮
            }
            else
            {
                // ����
                wsprintf(fname, "%s", wfd.cFileName);

                if(strstr(fname, ".zip") != NULL)
                {
                    _splitpath(fname, NULL, NULL, szName, szExt);

                    FILE_NAME *fInfo = new FILE_NAME();
                    fInfo->fileName = szName;
                    fInfo->fullFileName = fname;

                    m_vecFileName.push_back(fInfo);
                }                
            }
            
            bResult = FindNextFile(hSrch, &wfd);
        }

        char sysCommand[2048], sysCommand2[2048], szProgress[200], szProgress2[200];
        float fProgress = 0.0f;
        
        // ���� ���������� �����ϴ� ������ġ�� ���´�.
        GetCurrentDirectory(MAX_PATH, szCurrentDir);
        float stepProgress = 5.0f;

        for(unsigned int i=0; i<m_vecFileName.size(); ++i)
        {
            // ������
            fProgress = ((float)(i+1) / (float)(m_vecFileName.size())) * 100.0f;

            strcpy(szProgress, "");
            for(int j=0; j<(int)fProgress; ++j)
            {
                if(j % 2 == 0)
                    strcat(szProgress, "��");
            }
            
                
            

            // ���������� �����ϴ� ������ ������ Ǭ��.            
            //sprintf(sysCommand, "7za -y e -o%s\\%s %s\\%s", szPath, m_vecFileName[i]->fileName.c_str(), szPath, m_vecFileName[i]->fullFileName.c_str());
            //system(sysCommand);
            sprintf(sysCommand, " -y e -o%s\\%s %s\\%s", szPath, m_vecFileName[i]->fileName.c_str(), szPath, m_vecFileName[i]->fullFileName.c_str());            
            STARTUPINFO             sui;
            PROCESS_INFORMATION     pi;
            DWORD                   Ret;

            memset(&sui, 0x00, sizeof(STARTUPINFO));
            sui.cb = sizeof(STARTUPINFO);

            Ret = CreateProcess("7za.exe", sysCommand, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL,&sui, &pi);
            if (Ret) 
            {
                WaitForSingleObject(pi.hProcess, 0xffffffff);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }

            
            // ���� ������ �α������� backup������ �Űܵд�.
            sprintf(sysCommand, "%s\\BackupLog", szPath);
            CreateDirectory(sysCommand, NULL);  // backup ���� ����
            sprintf(sysCommand, "%s\\%s", szPath, m_vecFileName[i]->fullFileName.c_str());
            sprintf(sysCommand2, "%s\\BackupLog\\%s", szPath, m_vecFileName[i]->fullFileName.c_str());
            MoveFile(sysCommand, sysCommand2);  // backup ������ �α����� �̵�
            
            // ���� ������ �α������� �����.
            //char delFile[MAX_PATH];
            //sprintf(delFile, "%s\\%s", szPath, m_vecFileName[i]->fullFileName.c_str());
            //DeleteFile(delFile);

            // �׽�Ʈ�� ���ؼ� ����� �α����� ����Ʈ�� ����Ѵ�.
            //m_fNameList.AddString(sysCommand);
            strcat(sysCommand, " �м����Դϴ�...");
            m_logFileText.SetWindowText(sysCommand);

            // �������� �����ش�.            
            m_progressText.SetWindowText(szProgress);

            sprintf(szProgress2, "(%5.2f%%)", fProgress);
            m_textProgress.SetWindowText(szProgress2);


            // ���������� �� �α׵��� �Ľ��Ͽ� �߰���踦 ���� �÷��� üũ�Ѵ�. (�ٽ� ��踦 ���� ���� �α��ڷ�� ������� �ʵ��� �Ѵ�.)
            sprintf(sysCommand, "%s\\%s\\", szPath, m_vecFileName[i]->fileName.c_str());
            LoadLog(sysCommand, (char*)m_vecFileName[i]->fileName.c_str());
        }        

        // ���� ����ڷḦ �����Ѵ�.
        SaveTotalLog(szPath);

        // ���� ����ڷ� ����Ѵ�.
        OutputLog();

        m_logFileText.SetWindowText("");
    }
}

void CBugTrapWinServerDlg::OutputLog()
{
    LOG_RESULT *node;    
    float logAvg;
    char buf[2048];

    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        node = m_vecResultLog[i];
        logAvg = (float)(node->count / (float)m_totalLogCnt) * 100.0f;
        
        sprintf(buf, "%5d/%5d (%05.2f%%) %s", node->count, m_totalLogCnt, logAvg, node->name);
        m_crashList.AddString(buf);

        // ���̸� ����Ͽ� ���� ���̺��� ������ ���ο� ���̸� �����Ų��.
        int iExt = GetTextLenEx(m_crashList, buf);
        if (iExt > m_crashList.GetHorizontalExtent())
            m_crashList.SetHorizontalExtent(iExt);


    }    
}

void CBugTrapWinServerDlg::LoadTotalLog(char *szPath)
{
    int logCount, logNameCount;
    LOG_RESULT *node;
    char buf[2048], szLog[MAX_PATH];

    sprintf(szLog, "%s\\DumpLog.log", szPath);
    FILE *fp = fopen(szLog, "r");
    if(fp == NULL)
    {
        return;
    }

    ReleaseResult();

    fscanf(fp, "%d", &logCount);    
    for(int i=0; i<logCount; ++i)
    {
        fgets(buf, 2047, fp); // ���� ����
        fgets(buf, 2047, fp);
        fscanf(fp, "%d", &logNameCount);
        
        buf[strlen(buf)-1] = 0;

        node = new LOG_RESULT();
        node->name = new char[strlen(buf) + 1];
        strcpy(node->name, buf);
        node->count = logNameCount;

        for(int j=0; j<logNameCount; ++j)
        {
            fscanf(fp, "%s", buf);
            node->m_fileName.push_back(buf);
        }

        m_vecResultLog.push_back(node);
   }

    fclose(fp);
        
}

void CBugTrapWinServerDlg::SaveTotalLog(char *szPath)
{
    LOG_RESULT *node;
    char szLog[MAX_PATH];

    m_totalLogCnt = 0;

    sprintf(szLog, "%s\\DumpLog.log", szPath);
    
    FILE *fp = fopen(szLog, "w");
    if(fp == NULL)
    {
        MessageBox("��������� ������ �� �����ϴ�.", MB_OK);
        return;
    }

    sort(m_vecResultLog.begin(), m_vecResultLog.end(), resultSort);

    fprintf(fp, "%d\n", m_vecResultLog.size());
    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        node = m_vecResultLog[i];
        m_totalLogCnt += node->count;

        fprintf(fp, "%s\n%d\n", node->name, node->count);

        for(unsigned int j=0; j<node->m_fileName.size(); ++j)
        {
            string logName = node->m_fileName[j];
            fprintf(fp, "%s ", logName.c_str());
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void CBugTrapWinServerDlg::ReleaseLineInfo()
{
    for(unsigned int i=0; i<m_vecLineInfo.size(); ++i)
    {
        if(m_vecLineInfo[i])
        {
            delete m_vecLineInfo[i];
        }
    }

    m_vecLineInfo.clear();
}

void CBugTrapWinServerDlg::ReleaseResult()
{
    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        LOG_RESULT *node = m_vecResultLog[i];

        if(node->name)
            delete node->name;
        delete node;
    }

    m_vecResultLog.clear();
}

void CBugTrapWinServerDlg::ReleaseMap()
{
    for(unsigned int i=0; i<m_vecMapInfo.size(); ++i)
    {
        MAP_INFO *mapInfo = m_vecMapInfo[i];

        if(mapInfo->name)
            delete mapInfo->name;
        delete mapInfo;
    }

    m_vecMapInfo.clear();
}

void CBugTrapWinServerDlg::addResult(MAP_INFO *mapInfo, char*szName)
{    
    LOG_RESULT *logResult, *node;

    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        node = m_vecResultLog[i];
        if(strcmp(node->name, mapInfo->name) == 0)
        {
            node->m_fileName.push_back(szName);
            node->count += 1;
            return;
        }
    }

    logResult = new LOG_RESULT();
    logResult->name = new char[strlen(mapInfo->name) + 1];
    strcpy(logResult->name, mapInfo->name);
    logResult->count = 1;
    logResult->m_fileName.push_back(szName);

    m_vecResultLog.push_back(logResult);
}

void CBugTrapWinServerDlg::LoadLog(char *szPath, char*szName)
{
    int addr, offset, nCount;
    MAP_INFO *mapInfo;
    char *ptName, *ptSear, *ptAddr;
    char strLog[MAX_PATH], strDump[MAX_PATH], strLine[2048], tempBuf[2048];
    sprintf(strLog, "%serrorlog.log", szPath);
    sprintf(strDump, "%sdump.log", szPath);

    // �ش� �н����� log������ �ε��Ѵ�.
    FILE *fpLog = fopen(strLog, "r");
    FILE *fpDump = fopen(strDump, "w");

    if(fpLog == NULL || fpDump == NULL)
    {
        return;
    }

    nCount = 0;

    while(1)
    {
        if(feof(fpLog)) 
        {            
            break;
        }

        fgets(strLine, 2047, fpLog);

        ptSear = strstr(strLine, " at ");
        if(ptSear != NULL)
        {
            ptAddr = ptSear + 9;
            ptName = strchr(ptAddr, ',');
        }
        
        if(ptSear == NULL || ptName != NULL)
        {
            fputs(strLine, fpDump);
            continue;
        }

        sscanf(ptAddr, "%x", &addr);
        mapInfo = GetMapInfo(addr, offset);
        if(mapInfo == NULL)
        {
            fputs(strLine, fpDump);
            continue;
        }
        
        // ��������
        string lineFileName;
        char *tempfileName;
        int lineNum = 0;
        tempfileName = GetLineInfo(addr, lineNum);
        if(tempfileName == NULL)
        {
            lineFileName = "Not Search";
        }
        else
        {
            lineFileName = tempfileName;
        }

        unsigned int strInx = 0;
        while(strLine[strInx] != 0)
        {
            if(strLine[strInx] == 10)
            {
                strLine[strInx] = 0;
                break;
            }
            ++strInx;
        }
        //strLine[strlen(strLine)-1] = 0;
        sprintf(tempBuf, "%s, %s+%d byte(s) (%s + %d line)\n", strLine, mapInfo->name, offset, lineFileName.c_str(), lineNum);
        fputs(tempBuf, fpDump);

        if(nCount == 0)
            addResult(mapInfo, szName);        
        ++nCount;
    }

    fclose(fpLog);
    fclose(fpDump);    
}
unsigned int CBugTrapWinServerDlg::GetAddressValue(char *token)
{
    unsigned int address = 0;
    int addr1, addr2;
    char ptAddr1[100], ptAddr2[100];
    char *ptTemp, strTemp[1000];;

    address = m_preferredAddress;

    strcpy(ptAddr1, token);
    for(int i=0; i<strlen(ptAddr1); ++i)
    {
        if(ptAddr1[i] == ':')
        {
            ptAddr1[i] = 0;
            break;
        }
    }
    addr1 = atoi(ptAddr1);
    sprintf(strTemp, "%d", addr1 * 1000);
    sscanf(strTemp, "%x", &addr1);

    strcpy(ptAddr2, strchr(token, ':')+1);
    sscanf(ptAddr2, "%x", &addr2);

    address += (addr1 + addr2);

    return address;
}

bool CBugTrapWinServerDlg::LoadMapFile(const char *szFile)
{
    // Map������ �ε��Ѵ�.
    LINE_INFO *lineInfo;
    FILE *fpMap = NULL;
    fpMap = fopen(szFile, "r");
    if(fpMap == NULL)
    {
        MessageBox("X2.map������ �ε��� �� �����ϴ�.", "Error!", MB_OK);
        return false;
    }

    int tempCount = 0;
    unsigned int addr;
    char buf[2048], name[2048];
    m_preferredAddress = 0;    
    
    m_progressText.SetWindowText("Map �Ľ� ���Դϴ�. ��ø� ��ٸ�����!");

    while(fscanf(fpMap, "%s", buf) != EOF)
    {
        switch(tempCount)
        {
        case 0:
            if(strcmp(buf, "Preferred") == 0)
            {
                tempCount = 1;                
            }
            break;
        case 1:
            if(strcmp(buf, "is") == 0)
            {
                fscanf(fpMap, "%x", &addr);
                m_preferredAddress = addr;
                tempCount = 2;
            }
            break;
        case 2:
            if(strcmp(buf, "Rva+Base") == 0)
            {
                tempCount = 3;
            }
            break;
        case 3:
            {
                int tokenCount = 0;
                char*  token  =  NULL;
                char sep[] = " \t\n";

                // ��������
                fgets(buf, 2047, fpMap); 
                fgets(buf, 2047, fpMap); 

                while(1)
                {               
                    tokenCount = 0;
                    // ���� �����͸� �����´�.
                    fgets(buf, 2047, fpMap);                   

                    token = strtok(buf, sep);
                    if(token == NULL || token[0] == 10)
                    {
                        tempCount = 4;
                        break;
                    }

                    while(token != NULL)
                    {
                        // �и��� ��ū
                        ++tokenCount;

                        if(tokenCount == 2)
                        {
                            // Publics by Value
                            string strFuncName = GetUndecoratedFunctionName(token);
                            strcpy(name, strFuncName.c_str());

                        }
                        else if(tokenCount == 3)
                        {
                            // Rva+Base
                            sscanf(token, "%x", &addr);
                        }
                        token = strtok(NULL, sep);
                    }

                    MAP_INFO *mapInfo = new MAP_INFO();
                    mapInfo->address = addr;
                    mapInfo->name = new char[strlen(name) + 1];
                    strcpy(mapInfo->name, name);
                    m_vecMapInfo.push_back(mapInfo);
                }
            }            
            break;
        case 4:
            // line ������ �Ľ�
            while(1)
            {
                char *ptFileName, *tempPt;
                if(feof(fpMap)) 
                {          
                    tempCount = 100;
                    break;
                }

                fgets(buf, 2047, fpMap);
                if(strstr(buf, "numbers for ") != NULL)
                {
                    ptFileName = strchr(buf, '(') + 1;
                    tempPt = strchr(buf, ')');
                    tempPt[0]  = 0;     
                    tempCount = 5;

                    lineInfo = new LINE_INFO();
                    lineInfo->fileName = ptFileName;

                    m_vecLineInfo.push_back(lineInfo);            

                    break;
                }                
            }
            fgets(buf, 2047, fpMap); // ��������
            
            if(tempCount == 100)
                break;

        case 5:
            // Line
            {
            LINE_SUBINFO subNode;

            int tokenCount = 0;
            char*  token  =  NULL;
            char sep[] = " \t\n";

            while(1)
            {   
                tokenCount = 0;
                // ���� �����͸� �����´�.
                if(feof(fpMap)) 
                {          
                    tempCount = 100;
                    break;
                }
                fgets(buf, 2047, fpMap);                   

                token = strtok(buf, sep);
                if(token == NULL || token[0] == 10)
                {
                    tempCount = 4;
                    break;
                }

                while(token != NULL)
                {
                    // �и��� ��ū
                    ++tokenCount;

                    if(tokenCount % 2 == 1)
                    {
                        // line Value
                        subNode.lineNum = atoi(token);
                    }
                    else 
                    {
                        // address Value
                        subNode.Address = GetAddressValue(token);
                        lineInfo->m_vecAddress.push_back(subNode);
                    }
                    token = strtok(NULL, sep);
                }
            }
            }
            break;
        case 100:
            fclose(fpMap);
            m_progressText.SetWindowText("Map �Ľ� �Ϸ�!");            
            //sort(m_vecMapInfo.begin(), m_vecMapInfo.end(), mapSort);
            return true;
        }       
    }

    fclose(fpMap);
    m_progressText.SetWindowText("Map �Ľ� �Ϸ�!"); 

    return true;
}

char *CBugTrapWinServerDlg::GetLineInfo(unsigned int address, int &lineNum)
{
    char *strFileName;
    unsigned int step = 0xffffffff;
    LINE_INFO *node, *preNode;
    LINE_SUBINFO subNode, preSubNode;

    strFileName = new char[2048];
    preNode = NULL;

    for(unsigned int i=0; i<m_vecLineInfo.size(); ++i)
    {
        node = m_vecLineInfo[i];

        for(unsigned int j=0; j<node->m_vecAddress.size(); ++j)
        {
            subNode = node->m_vecAddress[j];

            if(address == subNode.Address)
            {
                if(j+1 < node->m_vecAddress.size())
                    subNode = node->m_vecAddress[j+1];

                lineNum = subNode.lineNum;
                strFileName = new char[node->fileName.length() + 1];
                strcpy(strFileName, node->fileName.c_str());
                return strFileName;
            }
            if(address < subNode.Address)
            {
                if(j==0)
                {
                    if(step >= subNode.Address - address)
                    {
                        step = subNode.Address - address;

                        if(preNode != NULL)
                        {
                            lineNum = preSubNode.lineNum;                        
                            strcpy(strFileName, preNode->fileName.c_str());
                        }                        
                    }                    

                    //return strFileName;
                }
                else
                {
                    if(step >= subNode.Address - address)
                    {
                        step = subNode.Address - address;

                        if(node != NULL)
                        {
                            lineNum = preSubNode.lineNum;                    
                            strcpy(strFileName, node->fileName.c_str());
                        }
                    }
                    //return strFileName;
                }
                
            }

            preSubNode = subNode;
        }

        preNode = node;
    }

    if(step < 0xffffffff)
    {
        return strFileName;
    }

    return NULL;
}

MAP_INFO* CBugTrapWinServerDlg::GetMapInfo(unsigned int address, int &offset)
{
    MAP_INFO *preMapInfo, *nowMapInfo;

    preMapInfo = nowMapInfo = NULL;
    for(unsigned int i=0; i<m_vecMapInfo.size(); ++i)
    {
        nowMapInfo = m_vecMapInfo[i];

        if(nowMapInfo->address == address)
        {
            offset = address - nowMapInfo->address;
            return nowMapInfo;
        }

        if(nowMapInfo->address > address)
        {
            offset = address - preMapInfo->address;
            return preMapInfo;
        }

        preMapInfo = nowMapInfo;
    }

    return NULL;
}

void CBugTrapWinServerDlg::ClearListBox()
{
    m_fNameList.ResetContent();
    m_crashList.ResetContent();
    m_crashSubList.ResetContent();
}

void CBugTrapWinServerDlg::OnEnChangeEdit1()
{
    // �׽�Ʈ��
    unsigned int address;
    int offset;
    CString strAddress;
    m_textAddress.GetWindowText(strAddress);

    sscanf(strAddress.GetString(), "%x", &address);

    MAP_INFO *mapInfo;
    mapInfo = GetMapInfo(address, offset);

    if(mapInfo == NULL)
    {
        MessageBox("MAP���� �ش� �ּ��� �Լ��� ã�� ���߽��ϴ�.", "Info!", MB_OK);
        return;
    }

    MessageBox(mapInfo->name, "Info!", MB_OK);
}

void CBugTrapWinServerDlg::OnLbnSelchangeListCrashList()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // crash list item ����
    m_crashSubList.ResetContent();

    int selIndex = -1;   
    selIndex = m_crashList.GetCurSel();

    LOG_RESULT *node = m_vecResultLog[selIndex];

    for(unsigned int i=0; i<node->m_fileName.size(); ++i)
    {
        m_crashSubList.AddString(node->m_fileName[i].c_str());
    }
}

void CBugTrapWinServerDlg::OnLbnDblclkListCrashSublist()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // carsh sub list item ����Ŭ����

    char fullPath[MAX_PATH];
    CString bufReport;
    int selIndex = -1;
    selIndex = m_crashSubList.GetCurSel();

    m_crashSubList.GetText(selIndex, bufReport);

    sprintf(fullPath, "%s\\%s", m_strFullPath.c_str(), bufReport.GetString());
    
    OutPutImage(fullPath);

}

void CBugTrapWinServerDlg::OutPutImage(char *fullPath)
{
    char pathName[MAX_PATH];
    sprintf(pathName, "%s\\Crash_ScreenShot.jpg", fullPath);

    m_imageDlg = new CCrashImageDlg(this);
    m_imageDlg->Create(IDD_DIALOG1);
    m_imageDlg->ShowWindow(SW_SHOW);
    m_imageDlg->LoadImage(pathName);
    m_imageDlg->Draw();
    
}


void CBugTrapWinServerDlg::OnLbnSelchangeListCrashSublist()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    char fullPath[MAX_PATH];
    CString bufReport;
    int selIndex = -1;
    selIndex = m_crashSubList.GetCurSel();

    m_crashSubList.GetText(selIndex, bufReport);

    sprintf(fullPath, "%s\\%s", m_strFullPath.c_str(), bufReport.GetString());

    OutPutReport(fullPath);
}

void CBugTrapWinServerDlg::OutPutReport(char *fullPath)
{    
    string strReport;
    char tempBuf[2049], buf[2049];
    char pathName[MAX_PATH];
    sprintf(pathName, "%s\\dump.log", fullPath);

    FILE *fp = fopen(pathName, "rt");
    strReport = "";
    while(1)
    {
        if(feof(fp))
        {
            break;
        }
        
        
        fgets(tempBuf, 2048, fp);               
        strReport += "\r\n";
        strReport += tempBuf;        
    }

    fclose(fp);
   
    
    m_bugReportText.SetWindowText(strReport.c_str());
}


