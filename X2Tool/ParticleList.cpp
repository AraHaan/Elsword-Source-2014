// ParticleList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "MainFrm.h"
#include "ParticleList.h"
#include "afxdialogex.h"


// ParticleList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(ParticleList, CDialogEx)

ParticleList::ParticleList(CWnd* pParent /*=NULL*/)
	: CDialogEx(ParticleList::IDD, pParent)
{

	//EnableAutomation();
	m_iSelCombo = -1;
	m_hSeqPreview			= INVALID_PARTICLE_HANDLE;
}

ParticleList::~ParticleList()
{
	CKTDGParticleSystem *pParticle = GetParticleSystem();
	if( pParticle != NULL )
	{
		pParticle->DestroyInstanceHandle( m_hSeqPreview );
	}	
}

void ParticleList::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialogEx::OnFinalRelease();
}

void ParticleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PARTICLE_TYPE, m_comboParticleType);
	DDX_Control(pDX, IDC_LIST_PARTICLE, m_listParticle);
}


BEGIN_MESSAGE_MAP(ParticleList, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_PARTICLE_TYPE, &ParticleList::OnCbnSelchangeComboParticleType)
	ON_LBN_SELCHANGE(IDC_LIST_PARTICLE, &ParticleList::OnLbnSelchangeListParticle)
	ON_BN_CLICKED(IDC_BUTTON1, &ParticleList::OnBnClickedButton1)
END_MESSAGE_MAP()

// BEGIN_DISPATCH_MAP(ParticleList, CDialogEx)
// END_DISPATCH_MAP()

// ����: IID_IParticleList�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {D3427BFF-E249-4A39-BBD4-9B392D0671E9}
// static const IID IID_IParticleList =
// { 0xD3427BFF, 0xE249, 0x4A39, { 0xBB, 0xD4, 0x9B, 0x39, 0x2D, 0x6, 0x71, 0xE9 } };

// BEGIN_INTERFACE_MAP(ParticleList, CDialogEx)
// 	INTERFACE_PART(ParticleList, IID_IParticleList, Dispatch)
// END_INTERFACE_MAP()


// ParticleList �޽��� ó�����Դϴ�.
BOOL ParticleList::OnInitDialog()
{	
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PARTICLE_TYPE);
	pCombo->AddString(L"Game Major");
	pCombo->AddString(L"Game Minor");
	pCombo->AddString(L"UI Major");
	pCombo->AddString(L"UI Minor");
	
	
	if( g_pX2Game == NULL || g_pData == NULL )
		return TRUE;

	pCombo->SetCurSel(m_iSelCombo);	
	UpdateParticleList(m_iSelCombo);

	return TRUE;
}
void ParticleList::OnShowWindow(BOOL bShow, UINT nStatus)
{		
	if( bShow == false )
	{
		CKTDGParticleSystem *pParticle = GetParticleSystem();
		if( pParticle != NULL )
			pParticle->DestroyInstanceHandle( m_hSeqPreview );	
	}
}

CKTDGParticleSystem* ParticleList::GetParticleSystem()
{
	CKTDGParticleSystem *pParticle = NULL;
	switch(m_iSelCombo)
	{
	case 0:
		pParticle = g_pX2Game->GetMajorParticle();
		break;
	case 1:
		pParticle = g_pX2Game->GetMinorParticle();
		break;
	case 2:
		pParticle = g_pData->GetUIMajorParticle();
		break;
	case 3:
		pParticle = g_pData->GetUIMinorParticle();
		break;
	default:
		break;
	}

	return pParticle;
}

void ParticleList::OnCbnSelchangeComboParticleType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PARTICLE_TYPE);
	int iCurSel = pCombo->GetCurSel();

	if( m_iSelCombo == iCurSel )
		return;

	m_iSelCombo = iCurSel;

	UpdateParticleList(iCurSel);
}

void ParticleList::UpdateParticleList(int iType)
{
	if( g_pX2Game == NULL || g_pData == NULL )
		return;

	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST_PARTICLE);
	if( pList == NULL )
		return;

	pList->ResetContent();
	
	CKTDGParticleSystem *pParticle = GetParticleSystem();
	if( pParticle == NULL )
		return;
		
	if( pParticle != NULL )
		pParticle->DestroyInstanceHandle( m_hSeqPreview );

	const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = pParticle->GetTempletSequences();
	map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
	for( it=templetSeq.begin(); it != templetSeq.end(); ++it )
	{
		wstring& wstrName = (wstring)it->first;
		pList->AddString( wstrName.c_str() );
	}	
}

void ParticleList::OnLbnSelchangeListParticle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST_PARTICLE);
	int iCurSel = pList->GetCurSel();
	CString csSelected;
	pList->GetText(iCurSel, csSelected);

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	m_wstrParticleName = csSelected.GetString();

	CKTDGParticleSystem *pParticle = GetParticleSystem();
	if( pParticle != NULL )
		pParticle->DestroyInstanceHandle( m_hSeqPreview );
	
	CRect rectClient;
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	::GetClientRect(pMainFrame->GetSafeHwnd(), &rectClient );	
	D3DXVECTOR3 vPos = g_pKTDXApp->GetUnProj3DPos(D3DXVECTOR3(rectClient.Width()/2.f, rectClient.Height()/2.f, 0.9993f));
	
	if( pParticle != NULL && m_wstrParticleName.empty() == false )
	{
		m_hSeqPreview = pParticle->CreateSequenceHandle( NULL,  m_wstrParticleName.c_str(), vPos.x, vPos.y, vPos.z,
			-1, -1, // emit rate
			1, -1, -1, // draw count, trigger
			false, 0.f );
	}	
}


void ParticleList::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_DlgWorldEditor.AddParticle(m_iSelCombo, m_wstrParticleName);

	ShowWindow(FALSE);
}
