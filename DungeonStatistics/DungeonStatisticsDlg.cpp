// DungeonStatisticsDlg.cpp : ���� ����
//

#include "stdafx.h"

#include <algorithm>
#include <functional>

#include "DungeonStatistics.h"
#include "DungeonStatisticsDlg.h"
#include ".\dungeonstatisticsdlg.h"

#include "SRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND hEdit;
bool bGroup1 = false;
bool bGroup2 = false;


// �ݹ��Լ� ����
static int CALLBACK MyCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CSortData *pLc1 = (CSortData*)lParam1;
    CSortData *pLc2 = (CSortData*)lParam2;
    int nReturn = pLc1->m_strText.Compare(pLc2->m_strText);

    if(lParamSort)
        return nReturn;
    else
        return -nReturn;
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


// CDungeonStatisticsDlg ��ȭ ����



CDungeonStatisticsDlg::CDungeonStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDungeonStatisticsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    for(int i=0; i<3; ++i)
        m_Statistics1[i] = m_Statistics2[i] = NULL;    

    m_nLimit = 5;
}

CDungeonStatisticsDlg::~CDungeonStatisticsDlg()
{
    
    //delete m_pDlgList1;
    
    for(int i=0; i<3; ++i)
    {
        if(m_Statistics1[i])
            delete m_Statistics1[i];

        if(m_Statistics2[i])
            delete m_Statistics2[i];

    }

    m_vecRecord.clear();
    m_vecRecordGroup.clear();

    if(m_pResultDlg)
        delete m_pResultDlg;
}

void CDungeonStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDungeonStatisticsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()   
    ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
    ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchangeCombo3)
    ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)    
    ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnClickListData)
    ON_CBN_SELCHANGE(IDC_COMBO_MIN, OnCbnSelchangeComboMin)
    ON_CBN_SELCHANGE(IDC_COMBO_MAX, OnCbnSelchangeComboMax)
    ON_CBN_SELCHANGE(IDC_COMBO_MIN2, OnCbnSelchangeComboMin2)
    ON_CBN_SELCHANGE(IDC_COMBO_MAX2, OnCbnSelchangeComboMax2)
    ON_CBN_SELCHANGE(IDC_COMBO_MIN3, OnCbnSelchangeComboMin3)
    ON_CBN_SELCHANGE(IDC_COMBO_MAX3, OnCbnSelchangeComboMax3)
END_MESSAGE_MAP()


// CDungeonStatisticsDlg �޽��� ó����

BOOL CDungeonStatisticsDlg::OnInitDialog()
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
	// Load Config
    LoadConfig();
      
    m_pResultDlg = new CResultInfo();
    
    
    for(int i=0; i<m_vecDungeonData.size(); ++i)
    {
        CComboBox *dlg = (CComboBox*)GetDlgItem(IDC_COMBO1);
        DungeonData node = m_vecDungeonData[i];
        dlg->AddString(node.m_name);
    }

    for(int i=0; i<m_vecCharType.size(); ++i)
    {
        CharType node = m_vecCharType[i];
        CComboBox *dlg = (CComboBox*)GetDlgItem(IDC_COMBO2);        
        dlg->AddString(node.m_name);
    }
    CComboBox *dlg1 = (CComboBox*)GetDlgItem(IDC_COMBO2);
    dlg1->SetCurSel(m_vecCharType.size() - 1);

    for(int i=0; i<m_vecClassType.size(); ++i)
    {
        ClassType node = m_vecClassType[i];
        CComboBox *dlg = (CComboBox*)GetDlgItem(IDC_COMBO3);        
        dlg->AddString(node.m_name);
    }
    CComboBox *dlg2 = (CComboBox*)GetDlgItem(IDC_COMBO3); 
    dlg2->SetCurSel(m_vecClassType.size() - 1);

    char tempbuf[3];
    sprintf(tempbuf, "0");
    GetDlgItem(IDC_EDIT1)->SetWindowText(tempbuf);

    m_bSort = false;
    m_pDlgList1 = (CListCtrl *)GetDlgItem(IDC_LIST1);
    m_pDlgList2 = NULL;

    m_pDlgList1->InsertColumn(0, "Unit", LVCFMT_CENTER, 50, 1);
    m_pDlgList1->InsertColumn(1, "Class", LVCFMT_CENTER, 100, 2);
    m_pDlgList1->InsertColumn(2, "Level", LVCFMT_CENTER, 50, 3);
    m_pDlgList1->InsertColumn(3, "���̵�", LVCFMT_CENTER, 50, 4);
    m_pDlgList1->InsertColumn(4, "������", LVCFMT_CENTER, 100, 5);
    m_pDlgList1->InsertColumn(5, "������", LVCFMT_CENTER, 100, 6);
    m_pDlgList1->InsertColumn(6, "��Ȱ��", LVCFMT_CENTER, 100, 7);
    m_pDlgList1->InsertColumn(7, "PT", LVCFMT_CENTER, 100, 8);
    m_pDlgList1->InsertColumn(8, "����", LVCFMT_CENTER, 50, 9);
    m_pDlgList1->InsertColumn(9, "����", LVCFMT_CENTER, 50, 10);
    m_pDlgList1->InsertColumn(10, "����", LVCFMT_CENTER, 50, 11);
    m_pDlgList1->InsertColumn(11, "��Ȱ", LVCFMT_CENTER, 50, 12);

    m_pDlgList1->SetExtendedStyle(LVS_EX_FULLROWSELECT);
    DragAcceptFiles();       

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CDungeonStatisticsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDungeonStatisticsDlg::OnPaint() 
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
HCURSOR CDungeonStatisticsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDungeonStatisticsDlg::OnBnClickedButton1()
{
    // File Open
    OPENFILENAME OFN;
    char str[32000] = "";
    char lpstrFile[10000] = "";
    char Name[MAX_PATH];

    char *p;
    

    memset(&OFN, 0, sizeof(OPENFILENAME));
    
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = NULL;
    OFN.lpstrFilter = "�ؽ�Ʈ����(*.txt)\0*.txt\0";
    OFN.lpstrFile = lpstrFile;
    OFN.nMaxFile = 10000;
    OFN.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

    if(GetOpenFileName(&OFN) != 0)
    {
        m_vecDungeonName.clear();
        m_vecRecord.clear();
        m_vecRecordGroup.clear();
        
        for(int i=0; i<3; ++i)
            m_vecPt[i].clear();
        ClearMinMax();
        


        p = lpstrFile;
        strcpy(Name, p);

        if(*(p+strlen(Name)+1) == 0)
        {
            // ���� ���� ����           
                
                AddRecord(Name);

                DungeonName node;
                strcpy(node.m_name, Name);
                m_vecDungeonName.push_back(node);
        }
        else
        {
            // �������� ���� ����
            while(1)
            {
                p = p+strlen(Name) + 1;
                if(*p == 0)
                    break;

                strcpy(Name, p);

                AddRecord(Name);

                DungeonName node;
                strcpy(node.m_name, Name);
                m_vecDungeonName.push_back(node);
            }
        }
    }

    char strCount[100];
    sprintf(strCount, "%d", m_vecRecord.size());
    GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strCount);

    ViewMinMax(m_nLimit);

    ProcessStatistics();
    ViewStatistics();
}

CStatistics *CDungeonStatisticsDlg::ProcessStatisticsGroup(std::vector<SRecord*> &vecGroup, int clv, int dlv)
{
    SRecord *node;
    CStatistics *snode;
    int sCount;
    
    sCount = 0;
    snode = new CStatistics();  

    GetMinMax();

    for(DWORD i = 0; i<vecGroup.size(); ++i)
    {        
        node = vecGroup[i];

        if(node->m_Time > m_nMax[dlv] || node->m_Time < m_nMin[dlv])
            continue;

        if(clv > 0 && node->m_LV != clv)
            continue;

        if(node->m_Diffculty != dlv)
            continue;

        snode->m_is0 += 1;
        if(node->m_ED > 0)
        {
            snode->m_is1 += 1;           
        }
        else
        {
            snode->m_is2 += 1;
        }

        
        snode->m_is3 += node->m_Continue;

        if(node->m_ED > 0 && node->m_EXP > 0)
        {
            ++sCount;

            snode->m_fs4 += node->m_Time;
            snode->m_fs5 += node->m_EXP;
            snode->m_fs6 += node->m_ED;
        }
        
    }

    if(snode->m_is0 > 0 && sCount > 0)
    {
        snode->m_fs1 = ((float)snode->m_is1 / (float)snode->m_is0) * 100.0f;
        snode->m_fs2 = ((float)snode->m_is2 / (float)snode->m_is0) * 100.0f;
        snode->m_fs3 = ((float)snode->m_is3 / (float)snode->m_is0) * 100.0f;
        snode->m_fs4 = ((double)snode->m_fs4 / (double)sCount);
        snode->m_fs5 = ((double)snode->m_fs5 / (double)sCount);
        snode->m_fs6 = ((double)snode->m_fs6 / (double)sCount);
    }    

    return snode;
}

void CDungeonStatisticsDlg::ProcessStatistics()
{
    SRecord *node;
    CStatistics *snode;
    int sCount[2][3];

    GetMinMax();

    for(int i=0; i<2; ++i)
        for(int j=0; j<3; ++j)
            sCount[i][j] = 0;

    for(int i=0; i<3; ++i)
    {
        if(m_Statistics1[i] != NULL)
            delete m_Statistics1[i];
        if(m_Statistics2[i] != NULL)
            delete m_Statistics2[i];
    }
    

    for(int i=0; i<3; ++i)
    {
        m_Statistics1[i] = new CStatistics();  // �̱� �⺻���
        m_Statistics2[i] = new CStatistics();  // ��Ƽ �⺻���
    }
    

    for(DWORD i = 0; i<m_vecRecord.size(); ++i)
    {        
        node = m_vecRecord[i];

        if(node->m_Time > m_nMax[node->m_Diffculty] || node->m_Time < m_nMin[node->m_Diffculty])
            continue;

        if(node->m_Party == 1)
        {
            // �̱�
            if(node->m_Diffculty < 0 || node->m_Diffculty >= 3)
                continue;

            if(node->m_Clear == 1)
                ++sCount[0][node->m_Diffculty]; 
            snode = m_Statistics1[node->m_Diffculty];
        }
        else
        {
            // ��Ƽ
            if(node->m_Diffculty < 0 || node->m_Diffculty >= 3)
                continue;
            
            if(node->m_Clear == 1)
                ++sCount[1][node->m_Diffculty];
            snode = m_Statistics2[node->m_Diffculty];
        }

        snode->m_is0 += 1;
        if(node->m_Clear == 1) 
        {
            snode->m_is1 += 1;
        }
        else
        {
            snode->m_is2 += 1;
        }

        snode->m_is3 += node->m_Continue;

        if(node->m_Clear == 1)
        {                       
            snode->m_fs4 += node->m_Time;
            snode->m_fs5 += node->m_EXP;
            snode->m_fs6 += node->m_ED;
        }        
        
    }

    // �̱� �������
    for(int i=0; i<3; ++i)
    {
        if(m_Statistics1[i]->m_is0 > 0 && sCount[0][i] > 0) 
        {
            m_Statistics1[i]->m_fs1 = ((float)m_Statistics1[i]->m_is1 / (float)m_Statistics1[i]->m_is0) * 100.0f;
            m_Statistics1[i]->m_fs2 = ((float)m_Statistics1[i]->m_is2 / (float)m_Statistics1[i]->m_is0) * 100.0f;
            m_Statistics1[i]->m_fs3 = ((float)m_Statistics1[i]->m_is3 / (float)m_Statistics1[i]->m_is0) * 100.0f;
            m_Statistics1[i]->m_fs4 = ((double)m_Statistics1[i]->m_fs4 / (double)sCount[0][i]);
            m_Statistics1[i]->m_fs5 = ((double)m_Statistics1[i]->m_fs5 / (double)sCount[0][i]);
            m_Statistics1[i]->m_fs6 = ((double)m_Statistics1[i]->m_fs6 / (double)sCount[0][i]);
        }    

        // ��Ƽ �������
        if(m_Statistics2[i]->m_is0 > 0 && sCount[1][i] > 0)
        {
            m_Statistics2[i]->m_fs1 = ((float)m_Statistics2[i]->m_is1 / (float)m_Statistics2[i]->m_is0) * 100.0f;
            m_Statistics2[i]->m_fs2 = ((float)m_Statistics2[i]->m_is2 / (float)m_Statistics2[i]->m_is0) * 100.0f;
            m_Statistics2[i]->m_fs3 = ((float)m_Statistics2[i]->m_is3 / (float)m_Statistics2[i]->m_is0) * 100.0f;
            m_Statistics2[i]->m_fs4 = ((double)m_Statistics2[i]->m_fs4 / (double)sCount[1][i]);
            m_Statistics2[i]->m_fs5 = ((double)m_Statistics2[i]->m_fs5 / (double)sCount[1][i]);
            m_Statistics2[i]->m_fs6 = ((double)m_Statistics2[i]->m_fs6 / (double)sCount[1][i]);
        }    
    }
    
}

void CDungeonStatisticsDlg::ViewStatistics()
{
    char buffer[3][6][100];
    char bufferParty[3][6][100];

    // �̱� 
    // �븻
    sprintf(buffer[0][0], "%5.2f%%", m_Statistics1[0]->m_fs1);
    GetDlgItem(IDC_STATIC1)->SetWindowText(buffer[0][0]);
    sprintf(buffer[0][1], "%5.2f%%", m_Statistics1[0]->m_fs2);
    GetDlgItem(IDC_STATIC2)->SetWindowText(buffer[0][1]);
    sprintf(buffer[0][2], "%5.2f%%", m_Statistics1[0]->m_fs3);
    GetDlgItem(IDC_STATIC3)->SetWindowText(buffer[0][2]);
    sprintf(buffer[0][3], "%10.2f", m_Statistics1[0]->m_fs4);
    GetDlgItem(IDC_STATIC4)->SetWindowText(buffer[0][3]);
    sprintf(buffer[0][4], "%10.2f", m_Statistics1[0]->m_fs5);
    GetDlgItem(IDC_STATIC5)->SetWindowText(buffer[0][4]);
    sprintf(buffer[0][5], "%10.2f", m_Statistics1[0]->m_fs6);
    GetDlgItem(IDC_STATIC6)->SetWindowText(buffer[0][5]);

    // �ϵ�
    sprintf(buffer[1][0], "%5.2f%%", m_Statistics1[1]->m_fs1);
    GetDlgItem(IDC_STATIC13)->SetWindowText(buffer[1][0]);
    sprintf(buffer[1][1], "%5.2f%%", m_Statistics1[1]->m_fs2);
    GetDlgItem(IDC_STATIC14)->SetWindowText(buffer[1][1]);
    sprintf(buffer[1][2], "%5.2f%%", m_Statistics1[1]->m_fs3);
    GetDlgItem(IDC_STATIC15)->SetWindowText(buffer[1][2]);
    sprintf(buffer[1][3], "%10.2f", m_Statistics1[1]->m_fs4);
    GetDlgItem(IDC_STATIC16)->SetWindowText(buffer[1][3]);
    sprintf(buffer[1][4], "%10.2f", m_Statistics1[1]->m_fs5);
    GetDlgItem(IDC_STATIC17)->SetWindowText(buffer[1][4]);
    sprintf(buffer[1][5], "%10.2f", m_Statistics1[1]->m_fs6);
    GetDlgItem(IDC_STATIC18)->SetWindowText(buffer[1][5]);

    // �ͽ���Ʈ
    sprintf(buffer[2][0], "%5.2f%%", m_Statistics1[2]->m_fs1);
    GetDlgItem(IDC_STATIC19)->SetWindowText(buffer[2][0]);
    sprintf(buffer[2][1], "%5.2f%%", m_Statistics1[2]->m_fs2);
    GetDlgItem(IDC_STATIC20)->SetWindowText(buffer[2][1]);
    sprintf(buffer[2][2], "%5.2f%%", m_Statistics1[2]->m_fs3);
    GetDlgItem(IDC_STATIC21)->SetWindowText(buffer[2][2]);
    sprintf(buffer[2][3], "%10.2f", m_Statistics1[2]->m_fs4);
    GetDlgItem(IDC_STATIC22)->SetWindowText(buffer[2][3]);
    sprintf(buffer[2][4], "%10.2f", m_Statistics1[2]->m_fs5);
    GetDlgItem(IDC_STATIC23)->SetWindowText(buffer[2][4]);
    sprintf(buffer[2][5], "%10.2f", m_Statistics1[2]->m_fs6);
    GetDlgItem(IDC_STATIC24)->SetWindowText(buffer[2][5]);



    // ��Ƽ
    // �븻
    sprintf(bufferParty[0][0], "%5.2f%%", m_Statistics2[0]->m_fs1);
    GetDlgItem(IDC_STATIC7)->SetWindowText(bufferParty[0][0]);
    sprintf(bufferParty[0][1], "%5.2f%%", m_Statistics2[0]->m_fs2);
    GetDlgItem(IDC_STATIC8)->SetWindowText(bufferParty[0][1]);
    sprintf(bufferParty[0][2], "%5.2f%%", m_Statistics2[0]->m_fs3);
    GetDlgItem(IDC_STATIC9)->SetWindowText(bufferParty[0][2]);
    sprintf(bufferParty[0][3], "%10.2f", m_Statistics2[0]->m_fs4);
    GetDlgItem(IDC_STATIC10)->SetWindowText(bufferParty[0][3]);
    sprintf(bufferParty[0][4], "%10.2f", m_Statistics2[0]->m_fs5);
    GetDlgItem(IDC_STATIC11)->SetWindowText(bufferParty[0][4]);
    sprintf(bufferParty[0][5], "%10.2f", m_Statistics2[0]->m_fs6);
    GetDlgItem(IDC_STATIC12)->SetWindowText(bufferParty[0][5]);

    // �ϵ�
    sprintf(bufferParty[1][0], "%5.2f%%", m_Statistics2[1]->m_fs1);
    GetDlgItem(IDC_STATIC25)->SetWindowText(bufferParty[1][0]);
    sprintf(bufferParty[1][1], "%5.2f%%", m_Statistics2[1]->m_fs2);
    GetDlgItem(IDC_STATIC26)->SetWindowText(bufferParty[1][1]);
    sprintf(bufferParty[1][2], "%5.2f%%", m_Statistics2[1]->m_fs3);
    GetDlgItem(IDC_STATIC27)->SetWindowText(bufferParty[1][2]);
    sprintf(bufferParty[1][3], "%10.2f", m_Statistics2[1]->m_fs4);
    GetDlgItem(IDC_STATIC28)->SetWindowText(bufferParty[1][3]);
    sprintf(bufferParty[1][4], "%10.2f", m_Statistics2[1]->m_fs5);
    GetDlgItem(IDC_STATIC29)->SetWindowText(bufferParty[1][4]);
    sprintf(bufferParty[1][5], "%10.2f", m_Statistics2[1]->m_fs6);
    GetDlgItem(IDC_STATIC30)->SetWindowText(bufferParty[1][5]);

    // �ͽ���Ʈ
    sprintf(bufferParty[2][0], "%5.2f%%", m_Statistics2[2]->m_fs1);
    GetDlgItem(IDC_STATIC31)->SetWindowText(bufferParty[2][0]);
    sprintf(bufferParty[2][1], "%5.2f%%", m_Statistics2[2]->m_fs2);
    GetDlgItem(IDC_STATIC32)->SetWindowText(bufferParty[2][1]);
    sprintf(bufferParty[2][2], "%5.2f%%", m_Statistics2[2]->m_fs3);
    GetDlgItem(IDC_STATIC33)->SetWindowText(bufferParty[2][2]);
    sprintf(bufferParty[2][3], "%10.2f", m_Statistics2[2]->m_fs4);
    GetDlgItem(IDC_STATIC34)->SetWindowText(bufferParty[2][3]);
    sprintf(bufferParty[2][4], "%10.2f", m_Statistics2[2]->m_fs5);
    GetDlgItem(IDC_STATIC35)->SetWindowText(bufferParty[2][4]);
    sprintf(bufferParty[2][5], "%10.2f", m_Statistics2[2]->m_fs6);
    GetDlgItem(IDC_STATIC36)->SetWindowText(bufferParty[2][5]);

    


    // �о���� �������� ����Ʈ�� ���
    char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];

    CComboBox *comboDlg = (CComboBox*)GetDlgItem(IDC_COMBO4);
    CComboBox *comboDlglv = (CComboBox*)GetDlgItem(IDC_COMBO5);
    comboDlglv->AddString("Normal");
    comboDlglv->AddString("Hard");
    comboDlglv->AddString("Expert");
    comboDlglv->SetCurSel(0);

    for(int i=comboDlg->GetCount()-1; i>=0; --i)
        comboDlg->DeleteString(i);
    comboDlg->Clear();

    for(int i=0; i<m_vecDungeonName.size(); ++i)
    {
        DungeonName node = m_vecDungeonName[i];
        _splitpath(node.m_name, drive, dir, fname, ext); 
        comboDlg->AddString(fname);
    }

    comboDlg->SetCurSel(0);
    
}

int CDungeonStatisticsDlg::AddRecord(char *fileName)
{
    FILE *fp = NULL;
    int result = 0;
    int count = 0; 
    char buffer[25][100]; 
    SRecord *record;
    int i;

    fp = fopen(fileName, "r");
    if(fp == NULL)
    {
        char buf[1000] = "";
        wsprintf(buf, "%s ������ �ε��� �� �����ϴ�.", fileName);
        MessageBox(buf, "Error!", MB_OK);
        return 0;
    }

    for(i=0; i<28; ++i)
    {
        fscanf(fp, "%s", buffer[0]);
    }

    while(1)
    {
        for(int i=0; i<25; ++i)
        {
            result = fscanf(fp, "%s", buffer[i]);
            if(result == EOF)
            {
                count = m_vecRecord.size();
                return count;
            }
        }
        
        int tempCount;

        if(atoi(buffer[7]) > 0)
        {
            record = new SRecord();

			tempCount     = atoi(buffer[0]);

            strcpy(record->m_UserName, buffer[1]);
            record->m_Diffculty     = atoi(buffer[2]);
            record->m_Char          = atoi(buffer[3]);
            record->m_Clear         = atoi(buffer[4]);
            record->m_Party         = atoi(buffer[5]);
            record->m_LV            = atoi(buffer[6]);
            record->m_Time          = atof(buffer[7]);
            record->m_EXP           = atoi(buffer[8]);
            record->m_ED            = atoi(buffer[9]);
            record->m_ComboRank     = buffer[10][0];
            tempCount               = atoi(buffer[11]); //
            record->m_TechnicalRank = buffer[12][0];
            tempCount               = atoi(buffer[13]); //
            record->m_TimeRank      = buffer[14][0];
            record->m_DamagedRank   = buffer[15][0];
            tempCount               = atoi(buffer[16]); //
            record->m_Rank          = buffer[17][0];
            record->m_Continue      = atoi(buffer[18]);
            record->m_PassStage     = atoi(buffer[19]);

            tempCount     = atoi(buffer[20]);
            tempCount     = atoi(buffer[21]);
            tempCount     = atoi(buffer[22]);
            tempCount     = atoi(buffer[23]);

			tempCount     = atoi(buffer[24]);

            m_vecRecord.push_back(record);        


            m_vecPt[atoi(buffer[2])].push_back(atof(buffer[7]));
        }        
    }


    return count;
}

bool CDungeonStatisticsDlg::LoadConfig()
{
    int result = 0;
    FILE *fp = fopen("config.txt", "r");
    char buffer[100];

    if(fp == NULL)
    {
        MessageBox("ȯ�������� �ε��� �� �����ϴ�. Config.txt������ �ִ��� Ȯ�����ּ���", "ERROR!", MB_OK);
        return false;
    }

    
    result = fscanf(fp, "%s", buffer);
    
    
    if(strcmp(buffer, "Character") == 0)
    {
        // Character Type
        while(1)
        {
            CharType node;
            int index;

            fscanf(fp, "%s", node.m_name);
            if(strcmp(node.m_name, "Class") == 0)
            {                
                strcpy(buffer, node.m_name);
                strcpy(node.m_name, "None");
                index = 0;
                node.m_vecCharType.push_back(index);
                m_vecCharType.push_back(node); 

                break;
            }

            fscanf(fp, "%d", &index);
            while(index != 0)
            {
                node.m_vecCharType.push_back(index);
                fscanf(fp, "%d", &index);
            }

            m_vecCharType.push_back(node);  
        }                      
    }

    if(strcmp(buffer, "Class") == 0)
    {
        // Class Type
        while(1)
        {
            ClassType node;
            
            fscanf(fp, "%s", node.m_name);
            if(strcmp(node.m_name, "Dungeon") == 0)
            {
                strcpy(buffer, node.m_name);
                strcpy(node.m_name, "None");
                node.m_index = 0;
                m_vecClassType.push_back(node);

                break;
            }

            fscanf(fp, "%d", &(node.m_index));
            m_vecClassType.push_back(node);
        }
    }
    
    if(strcmp(buffer, "Dungeon") == 0)
    {
        // Dungeon Data        
        while(1)
        {
            DungeonData node;

            result = fscanf(fp, "%s%d", node.m_name, &(node.m_index));
            if(result == EOF)
            {        
                return true;
            }
            
            m_vecDungeonData.push_back(node);
        }
    }        
    
}



void CDungeonStatisticsDlg::OnBnClickedCancel()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    OnCancel();
}

void CDungeonStatisticsDlg::OnCbnSelchangeCombo2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // ĳ���� Ÿ�� ����
    
    char buffer1[100];
    int nSelected1, nLV, nid;
    CharType charNode;
    bool bFindChar;
    CString lvBuffer;

    m_vecRecordGroup.clear();

    bFindChar = false;
    GetDlgItem(IDC_EDIT1)->GetWindowText(lvBuffer);
    nLV = atoi(lvBuffer.GetString());

    CComboBox *comDlg1 = (CComboBox *)GetDlgItem(IDC_COMBO2);
    nSelected1 = comDlg1->GetCurSel();
    comDlg1->GetLBText(nSelected1, buffer1);

    CComboBox *dlg2 = (CComboBox*)GetDlgItem(IDC_COMBO3); 
    dlg2->SetCurSel(m_vecClassType.size() - 1);   

    for(int i=0; i<m_vecCharType.size(); ++i)
    {
        charNode = m_vecCharType[i];
        if(strcmp(charNode.m_name, buffer1) == 0)
        {
            bFindChar = true;
            break;
        }
    }        
    

    for(int i=0; i<m_vecRecord.size(); ++i)
    {
        SRecord *node;
        node = m_vecRecord[i];        

        if(bFindChar == false)
        {            
            if(nLV <= 0 || node->m_LV == nLV)
            {
                m_vecRecordGroup.push_back(node);
            }
        }
        else
        {
            for(int j=0; j<charNode.m_vecCharType.size(); ++j)
            {
                nid = charNode.m_vecCharType[j];
                if(nid == 0 || node->m_Char == nid)
                {
                    if(nLV <= 0 || node->m_LV == nLV)
                    {
                        m_vecRecordGroup.push_back(node);
                    }
                }
            }
        }
    }
   
    bGroup1 = true;
}

void CDungeonStatisticsDlg::OnCbnSelchangeCombo3()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // ĳ���� Ŭ���� Ÿ�� ����

    char buffer1[100];
    int nSelected1, nLV, nid;
    ClassType ClassNode;
    bool bFindChar;
    CString lvBuffer;

    m_vecRecordGroup.clear();

    bFindChar = false;
    GetDlgItem(IDC_EDIT1)->GetWindowText(lvBuffer);
    nLV = atoi(lvBuffer.GetString());

    CComboBox *comDlg1 = (CComboBox *)GetDlgItem(IDC_COMBO3);
    nSelected1 = comDlg1->GetCurSel();
    comDlg1->GetLBText(nSelected1, buffer1);

    CComboBox *dlg2 = (CComboBox*)GetDlgItem(IDC_COMBO2); 
    dlg2->SetCurSel(m_vecCharType.size() - 1);   

    for(int i=0; i<m_vecClassType.size(); ++i)
    {
        ClassNode = m_vecClassType[i];
        if(strcmp(ClassNode.m_name, buffer1) == 0)
        {
            bFindChar = true;
            break;
        }
    }        
    nid = ClassNode.m_index;

    for(int i=0; i<m_vecRecord.size(); ++i)
    {
        SRecord *node;
        node = m_vecRecord[i];        

        if(bFindChar == false)
        {            
            if(nLV <= 0 || node->m_LV == nLV)
            {
                m_vecRecordGroup.push_back(node);
            }
        }
        else
        {               
            if(nid == 0 || node->m_Char == nid)
            {
                if(nLV <= 0 || node->m_LV == nLV)
                {
                    m_vecRecordGroup.push_back(node);
                }
            }            
        }
    }

    bGroup2 = true;
}

void CDungeonStatisticsDlg::OnBnClickedButton2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    m_pDlgList1->DeleteAllItems();    
}

void CDungeonStatisticsDlg::OnBnClickedOk()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //OnOK();
    
    CStatistics *snode;    
    char temp[3000] = "";

    memset(resultBuffer, 0, 10000);

    snode = m_Statistics1[0];
    sprintf(temp, "-- �̱�(Normal) --\r\n������ : %5.2f%%\r\n������ : %5.2f%%\r\n��Ȱ�� : %5.2f%%\r\n���PT : %10.2f\r\n���EXP : %10.2f\r\n���ED : %10.2f",
        snode->m_fs1, snode->m_fs2, snode->m_fs3, snode->m_fs4, snode->m_fs5, snode->m_fs6);
    strcpy(resultBuffer, temp);

    snode = m_Statistics1[1];
    sprintf(temp, "\r\n\r\n-- �̱�(Hard) --\r\n������ : %5.2f%%\r\n������ : %5.2f%%\r\n��Ȱ�� : %5.2f%%\r\n���PT : %10.2f\r\n���EXP : %10.2f\r\n���ED : %10.2f",
        snode->m_fs1, snode->m_fs2, snode->m_fs3, snode->m_fs4, snode->m_fs5, snode->m_fs6);
    strcat(resultBuffer, temp);
    
    snode = m_Statistics1[2];
    sprintf(temp, "\r\n\r\n-- �̱�(Expert) --\r\n������ : %5.2f%%\r\n������ : %5.2f%%\r\n��Ȱ�� : %5.2f%%\r\n���PT : %10.2f\r\n���EXP : %10.2f\r\n���ED : %10.2f",
        snode->m_fs1, snode->m_fs2, snode->m_fs3, snode->m_fs4, snode->m_fs5, snode->m_fs6);
    strcat(resultBuffer, temp);


    snode = m_Statistics2[0];
    sprintf(temp, "\r\n\r\n-- ��Ƽ(Normal) --\r\n������ : %5.2f%%\r\n������ : %5.2f%%\r\n��Ȱ�� : %5.2f%%\r\n���PT : %10.2f\r\n���EXP : %10.2f\r\n���ED : %10.2f",
        snode->m_fs1, snode->m_fs2, snode->m_fs3, snode->m_fs4, snode->m_fs5, snode->m_fs6);
    strcat(resultBuffer, temp);

    snode = m_Statistics2[1];
    sprintf(temp, "\r\n\r\n-- ��Ƽ(Hard) --\r\n������ : %5.2f%%\r\n������ : %5.2f%%\r\n��Ȱ�� : %5.2f%%\r\n���PT : %10.2f\r\n���EXP : %10.2f\r\n���ED : %10.2f",
        snode->m_fs1, snode->m_fs2, snode->m_fs3, snode->m_fs4, snode->m_fs5, snode->m_fs6);
    strcat(resultBuffer, temp);

    snode = m_Statistics2[2];
    sprintf(temp, "\r\n\r\n-- ��Ƽ(Expert) --\r\n������ : %5.2f%%\r\n������ : %5.2f%%\r\n��Ȱ�� : %5.2f%%\r\n���PT : %10.2f\r\n���EXP : %10.2f\r\n���ED : %10.2f",
        snode->m_fs1, snode->m_fs2, snode->m_fs3, snode->m_fs4, snode->m_fs5, snode->m_fs6);
    strcat(resultBuffer, temp);

    //m_pResultDlg = new CResultInfo();
    //m_pResultDlg->ShowWindow(SW_SHOW);

    m_pResultDlg->SetText(resultBuffer);
    m_pResultDlg->DoModal();
    

    
}

void CDungeonStatisticsDlg::OnEnChangeEdit1()
{
    // TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
    // CDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
    // CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
    // �� �˸��� ������ �ʽ��ϴ�.

    // TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //MessageBox("����", "Info!", MB_OK);
}

void CDungeonStatisticsDlg::OnBnClickedButton3()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString lvBuffer;
    char buffer1[100], buffer2[100];
    int nLV, dLV, nSelected1;

    CComboBox *comboDlglv = (CComboBox*)GetDlgItem(IDC_COMBO5);
    nSelected1 = comboDlglv->GetCurSel();
    comboDlglv->GetLBText(nSelected1, buffer1);
    if(strcmp(buffer1, "Normal") == 0)
        dLV = 0;
    else if(strcmp(buffer1, "Hard") == 0)
        dLV = 1;
    else if(strcmp(buffer1, "Expert") == 0)
        dLV = 2;

    CComboBox *comDlg1 = (CComboBox *)GetDlgItem(IDC_COMBO2);
    nSelected1 = comDlg1->GetCurSel();
    comDlg1->GetLBText(nSelected1, buffer1);

    CComboBox *comDlg2 = (CComboBox *)GetDlgItem(IDC_COMBO3);
    nSelected1 = comDlg2->GetCurSel();
    comDlg2->GetLBText(nSelected1, buffer2);

    GetDlgItem(IDC_EDIT1)->GetWindowText(lvBuffer);
    nLV = atoi(lvBuffer.GetString());

    if(bGroup1 == false && bGroup2 == false)
    {
        MessageBox("�׷����� �׸��� �����ϼ���", "Info!", MB_OK);
        return;
    }

    CStatistics *result;
    result = ProcessStatisticsGroup(m_vecRecordGroup, nLV, dLV);
    char buf[12][100];
    int nCount = m_pDlgList1->GetItemCount();    

    if(strcmp(buffer1, "None") == 0)
        sprintf(buf[0], "��ü");
    else
        sprintf(buf[0], buffer1);

    if(strcmp(buffer2, "None") == 0)
        sprintf(buf[1], "��ü");
    else
        sprintf(buf[1], buffer2);

    if(nLV == 0)
        sprintf(buf[2], "��ü");
    else
        sprintf(buf[2], "%d", nLV);

    if(dLV == 0)
        sprintf(buf[3], "Normal");
    else if(dLV == 1)
        sprintf(buf[3], "Hard");
    else if(dLV == 2)
        sprintf(buf[3], "Expert");

    sprintf(buf[4], "%5.2f%%", result->m_fs1);
    sprintf(buf[5], "%5.2f%%", result->m_fs2);
    sprintf(buf[6], "%5.2f%%", result->m_fs3);
    sprintf(buf[7], "%10.2f", result->m_fs4);
    sprintf(buf[8], "%d", result->m_is0);
    sprintf(buf[9], "%d", result->m_is1);
    sprintf(buf[10], "%d", result->m_is2);
    sprintf(buf[11], "%d", result->m_is3);

    m_pDlgList1->InsertItem(0, buf[0]);
    for(int i=1; i<12; ++i)
        m_pDlgList1->SetItemText(0, i, buf[i]);
}

void CDungeonStatisticsDlg::OnDropFiles(HDROP hDropInfo)
{
    int nFiles;
    TCHAR szPathName[MAX_PATH];

    nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH);

    m_vecDungeonName.clear();
    m_vecRecord.clear();
    m_vecRecordGroup.clear();    
    m_pDlgList1->DeleteAllItems();
    for(int i=0; i<3; ++i)
        m_vecPt[i].clear();
    ClearMinMax();


    CString strFullPath;
    for(int index = 0; index < nFiles; ++index)
    {
        DragQueryFile(hDropInfo, index, szPathName, MAX_PATH);
        
        DungeonName node;
        strcpy(node.m_name, szPathName);
        m_vecDungeonName.push_back(node);

        AddRecord(szPathName);
        
    }
    
    DragFinish(hDropInfo);
    UpdateData(FALSE);

    ViewMinMax(m_nLimit);

    ProcessStatistics();
    ViewStatistics();

    return;
}
void CDungeonStatisticsDlg::OnBnClickedButton4()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    int result;
    
    result = MessageBox("���� �ʱ�ȭ �Ͻðڽ��ϱ�?", "Question!", MB_YESNO);

    if(result == IDYES)
    {
        m_vecDungeonName.clear();
        m_vecRecord.clear();
        m_vecRecordGroup.clear();
        for(int i=0; i<3; ++i)
            m_vecPt[i].clear();
        ClearMinMax();
        
        for(int i=0; i<3; ++i)
        {
            if(m_Statistics1[i] != NULL)
            {
                delete m_Statistics1[i];
                m_Statistics1[i] = NULL;
            }
            if(m_Statistics2[i] != NULL)
            {
                delete m_Statistics2[i];
                m_Statistics2[i] = NULL;
            }
        }
        

        m_pDlgList1->DeleteAllItems();

        //ViewMinMax(m_nLimit);

        ProcessStatistics();
        ViewStatistics();
    }    
}

void CDungeonStatisticsDlg::OnColumnClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW *pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(!SortData(pNMListView->iSubItem, m_bSort))
        return;

    m_bSort = !m_bSort;
    *pResult = 0;
}

BOOL CDungeonStatisticsDlg::SortData(int nCol, BOOL bSort)
{
    CListCtrl *pLc = (CListCtrl*)GetDlgItem(IDC_LIST1);
    int totitems = pLc->GetItemCount();

    for(int i=0; i<totitems; ++i)
    {
        DWORD dw = pLc->GetItemData(i);
        CString txt = pLc->GetItemText(i, nCol);
        pLc->SetItemData(i, (DWORD) new CSortData(txt, dw));
    }

    pLc->SortItems(MyCompare, bSort);

    for(i=0; i<totitems; ++i)
    {
        CSortData *pItem = (CSortData*)pLc->GetItemData(i);
        ASSERT(pItem);
        pLc->SetItemData(i, pItem->m_dw);
        delete pItem;
    }

    return TRUE;
}


void CDungeonStatisticsDlg::ViewMinMax(int nLimit)
{
    char minMaxBuffer[23];

    ClearMinMax();

    if(nLimit <= 0)
        return;

    CComboBox *comboDlgMin = (CComboBox*)GetDlgItem(IDC_COMBO_MIN);
    CComboBox *comboDlgMax = (CComboBox*)GetDlgItem(IDC_COMBO_MAX);

    CComboBox *comboDlgMin2 = (CComboBox*)GetDlgItem(IDC_COMBO_MIN2);
    CComboBox *comboDlgMax2 = (CComboBox*)GetDlgItem(IDC_COMBO_MAX2);

    CComboBox *comboDlgMin3 = (CComboBox*)GetDlgItem(IDC_COMBO_MIN3);
    CComboBox *comboDlgMax3 = (CComboBox*)GetDlgItem(IDC_COMBO_MAX3);

    if(m_vecPt[0].size() > 0)
        sort(m_vecPt[0].begin(), m_vecPt[0].end());

    for(int i=0; i<m_vecPt[0].size(); ++i)
    {
        sprintf(minMaxBuffer, "%10.2f", m_vecPt[0][i]);
        comboDlgMin->AddString(minMaxBuffer);
    }
    comboDlgMin->SetCurSel(nLimit-1);
    
    for(int i=m_vecPt[0].size()-1; i>=0; --i)
    {
        sprintf(minMaxBuffer, "%10.2f", m_vecPt[0][i]);
        comboDlgMax->AddString(minMaxBuffer);
    }
    comboDlgMax->SetCurSel(nLimit-1);

    if(m_vecPt[1].size() > 0)
        sort(m_vecPt[1].begin(), m_vecPt[1].end());

    for(int i=0; i<m_vecPt[1].size(); ++i)
    {
        sprintf(minMaxBuffer, "%10.2f", m_vecPt[1][i]);
        comboDlgMin2->AddString(minMaxBuffer);
    }
    comboDlgMin2->SetCurSel(nLimit-1);

    for(int i=m_vecPt[1].size()-1; i>=0; --i)
    {
        sprintf(minMaxBuffer, "%10.2f", m_vecPt[1][i]);
        comboDlgMax2->AddString(minMaxBuffer);
    }
    comboDlgMax2->SetCurSel(nLimit-1);


    if(m_vecPt[2].size() > 0)
        sort(m_vecPt[2].begin(), m_vecPt[2].end());

    for(int i=0; i<m_vecPt[2].size(); ++i)
    {
        sprintf(minMaxBuffer, "%10.2f", m_vecPt[2][i]);
        comboDlgMin3->AddString(minMaxBuffer);
    }
    comboDlgMin3->SetCurSel(nLimit-1);

    for(int i=m_vecPt[2].size()-1; i>=0; --i)
    {
        sprintf(minMaxBuffer, "%10.2f", m_vecPt[2][i]);
        comboDlgMax3->AddString(minMaxBuffer);
    }
    comboDlgMax3->SetCurSel(nLimit-1);
}


void CDungeonStatisticsDlg::GetMinMax()
{
    CString strMin, strMax;
    int nSelected;
    CComboBox *comboDlgMin = (CComboBox*)GetDlgItem(IDC_COMBO_MIN);
    CComboBox *comboDlgMax = (CComboBox*)GetDlgItem(IDC_COMBO_MAX);

    CComboBox *comboDlgMin2 = (CComboBox*)GetDlgItem(IDC_COMBO_MIN2);
    CComboBox *comboDlgMax2 = (CComboBox*)GetDlgItem(IDC_COMBO_MAX2);

    CComboBox *comboDlgMin3 = (CComboBox*)GetDlgItem(IDC_COMBO_MIN3);
    CComboBox *comboDlgMax3 = (CComboBox*)GetDlgItem(IDC_COMBO_MAX3);

    if(comboDlgMax->GetCount() > 0 && comboDlgMin->GetCount() > 0)
    {
        nSelected = comboDlgMin->GetCurSel();
        comboDlgMin->GetLBText(nSelected, strMin);
        m_nMin[0] = atof(strMin.GetString());

        nSelected = comboDlgMax->GetCurSel();
        comboDlgMax->GetLBText(nSelected, strMax);
        m_nMax[0] = atof(strMax.GetString());
    }

    if(comboDlgMax2->GetCount() > 0 && comboDlgMin2->GetCount() > 0)
    {
        nSelected = comboDlgMin2->GetCurSel();
        comboDlgMin2->GetLBText(nSelected, strMin);
        m_nMin[1] = atof(strMin.GetString());

        nSelected = comboDlgMax2->GetCurSel();
        comboDlgMax2->GetLBText(nSelected, strMax);
        m_nMax[1] = atof(strMax.GetString());
    }

    if(comboDlgMax3->GetCount() > 0 && comboDlgMin3->GetCount() > 0)
    {
        nSelected = comboDlgMin3->GetCurSel();
        comboDlgMin3->GetLBText(nSelected, strMin);
        m_nMin[2] = atof(strMin.GetString());

        nSelected = comboDlgMax3->GetCurSel();
        comboDlgMax3->GetLBText(nSelected, strMax);
        m_nMax[2] = atof(strMax.GetString());
    }

}

void CDungeonStatisticsDlg::ClearMinMax()
{
    

    CComboBox *comboDlgMin = (CComboBox*)GetDlgItem(IDC_COMBO_MIN);
    for(int i=comboDlgMin->GetCount()-1; i>=0; --i)
        comboDlgMin->DeleteString(i);
    comboDlgMin->Clear();

    CComboBox *comboDlgMax = (CComboBox*)GetDlgItem(IDC_COMBO_MAX);
    for(int i=comboDlgMax->GetCount()-1; i>=0; --i)
        comboDlgMax->DeleteString(i);
    comboDlgMax->Clear();

    CComboBox *comboDlgMin2 = (CComboBox*)GetDlgItem(IDC_COMBO_MIN2);
    for(int i=comboDlgMin2->GetCount()-1; i>=0; --i)
        comboDlgMin2->DeleteString(i);
    comboDlgMin2->Clear();

    CComboBox *comboDlgMax2 = (CComboBox*)GetDlgItem(IDC_COMBO_MAX2);
    for(int i=comboDlgMax2->GetCount()-1; i>=0; --i)
        comboDlgMax2->DeleteString(i);
    comboDlgMax2->Clear();

    CComboBox *comboDlgMin3 = (CComboBox*)GetDlgItem(IDC_COMBO_MIN3);
    for(int i=comboDlgMin3->GetCount()-1; i>=0; --i)
        comboDlgMin3->DeleteString(i);
    comboDlgMin3->Clear();

    CComboBox *comboDlgMax3 = (CComboBox*)GetDlgItem(IDC_COMBO_MAX3);
    for(int i=comboDlgMax3->GetCount()-1; i>=0; --i)
        comboDlgMax3->DeleteString(i);
    comboDlgMax3->Clear();
}

void CDungeonStatisticsDlg::OnCbnSelchangeComboMin()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessStatistics();
    ViewStatistics();
}

void CDungeonStatisticsDlg::OnCbnSelchangeComboMax()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessStatistics();
    ViewStatistics();
}


void CDungeonStatisticsDlg::OnCbnSelchangeComboMin2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessStatistics();
    ViewStatistics();
}

void CDungeonStatisticsDlg::OnCbnSelchangeComboMax2()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessStatistics();
    ViewStatistics();
}

void CDungeonStatisticsDlg::OnCbnSelchangeComboMin3()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessStatistics();
    ViewStatistics();
}

void CDungeonStatisticsDlg::OnCbnSelchangeComboMax3()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ProcessStatistics();
    ViewStatistics();
}
