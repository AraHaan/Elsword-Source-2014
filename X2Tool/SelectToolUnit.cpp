// SelectToolUnit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "SelectToolUnit.h"
#include "afxdialogex.h"


// CSelectToolUnit ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSelectToolUnit, CDialogEx)

CSelectToolUnit::CSelectToolUnit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectToolUnit::IDD, pParent)
{

}

CSelectToolUnit::~CSelectToolUnit()
{
}

void CSelectToolUnit::InitToolunit()
{
	// Unit Class
	//map<CX2Unit::UNIT_CLASS, CX2Unit::UnitTemplet*> mapUnitTemplet = g_pData->GetUnitManager()->GetUnitTempletMap4Tool();
	//map<CX2Unit::UNIT_CLASS, CX2Unit::UnitTemplet*>::iterator mit = mapUnitTemplet.begin();
	CX2UnitManager::UnitTempletMap& mapUnitTemplet = g_pData->GetUnitManager()->GetUnitTempletMap4Tool();
	CX2UnitManager::UnitTempletMap::iterator mit = mapUnitTemplet.begin();

	while( mit != mapUnitTemplet.end() )
	{
		const CX2Unit::UnitTemplet* pUnitTemplet = mit->second;
		wstring wstrUnitName = pUnitTemplet->m_Description;
		CX2Unit::UNIT_CLASS unitClass = pUnitTemplet->m_UnitClass;

		m_comboUnitClass.AddString( wstrUnitName.c_str() );

		++mit;
	}

	wstring wstrDefaultname = L"���ҵ�";
	CString csUnitName;

	int iDefaultIndex = m_comboUnitClass.FindString( 0, wstrDefaultname.c_str() );
	m_comboUnitClass.SetCurSel(iDefaultIndex);
	m_comboUnitClass.GetLBText(iDefaultIndex, csUnitName);
	
	mit = mapUnitTemplet.begin();
	while( mit != mapUnitTemplet.end() )
	{
		const CX2Unit::UnitTemplet* pUnitTemplet = mit->second;
		wstring wstrUnitName = pUnitTemplet->m_Description;
		CX2Unit::UNIT_CLASS unitClass = pUnitTemplet->m_UnitClass;

		if( csUnitName.Compare(wstrUnitName.c_str()) == 0 )
		{
			m_eUnitClass = pUnitTemplet->m_UnitClass;
			break;
		}
		++mit;
	}

	// Unit Skill
	ChangeSkill();
	
	// Unit Name
	m_wstrUnitName = L"���ҵ�";

	// Unit Level
	m_iLevel = 1;

	// unit Stat
	m_uHp = 11250;
	m_iAtkPhysic = 360;
	m_iAtkMagic = 296;
	m_iDefPhysic = 80;
	m_iDefMagic = 66;
	
	m_editUnitName.SetWindowTextW(m_wstrUnitName.c_str());
	WCHAR wBuf[100];
	wsprintfW(wBuf, L"%d", m_iLevel);
	m_editLevel.SetWindowText(wBuf);
	wsprintfW(wBuf, L"%d", m_uHp);
	m_editHp.SetWindowTextW(wBuf);
	wsprintfW(wBuf, L"%d", m_iAtkPhysic);
	m_editAtkPhysic.SetWindowTextW(wBuf);
	wsprintfW(wBuf, L"%d", m_iAtkMagic);
	m_editAtkMagic.SetWindowTextW(wBuf);
	wsprintfW(wBuf, L"%d", m_iDefPhysic);
	m_editDefPhysic.SetWindowTextW(wBuf);
	wsprintfW(wBuf, L"%d", m_iDefMagic);
	m_editDefMagic.SetWindowTextW(wBuf);
}

void CSelectToolUnit::ChangeSkill()
{
	m_comboSkillA.ResetContent();
	m_comboSkillS.ResetContent();
	m_comboSkillD.ResetContent();
	m_comboSkillC.ResetContent();

	std::map< CX2SkillTree::SKILL_ID, CX2SkillTree::SkillTreeTemplet > mapSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTempletMap4Tool( (int)m_eUnitClass );
	std::map<CX2SkillTree::SKILL_ID, CX2SkillTree::SkillTreeTemplet>::iterator mit = mapSkillTreeTemplet.begin();

	while(mit != mapSkillTreeTemplet.end() )
	{
		CX2SkillTree::SKILL_ID iSkillId = (CX2SkillTree::SKILL_ID)mit->first;
		CX2SkillTree::SkillTreeTemplet skillTreeTemplet = (CX2SkillTree::SkillTreeTemplet)mit->second;
		KUserSkillData userSkillData;
#ifdef UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet *pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( iSkillId );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet *pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(iSkillId);
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( pSkillTemplet != NULL && 
			( pSkillTemplet->m_eType == CX2SkillTree::ST_ACTIVE 
			|| pSkillTemplet->m_eType == CX2SkillTree::ST_SPECIAL_ACTIVE 
#ifdef ADDED_RELATIONSHIP_SYSTEM
			|| pSkillTemplet->m_eType == CX2SkillTree::ST_RELATIONSHIP_SKILL
#endif // ADDED_RELATIONSHIP_SYSTEM
			) )
		{
			m_comboSkillA.AddString( pSkillTemplet->m_wstrName.c_str() );
			m_comboSkillS.AddString( pSkillTemplet->m_wstrName.c_str() );
			m_comboSkillD.AddString( pSkillTemplet->m_wstrName.c_str() );
			m_comboSkillC.AddString( pSkillTemplet->m_wstrName.c_str() );
						
			m_iSkillId[0] = (int)pSkillTemplet->m_eID;
			m_iSkillId[1] = (int)pSkillTemplet->m_eID;
			m_iSkillId[2] = (int)pSkillTemplet->m_eID;
			m_iSkillId[3] = (int)pSkillTemplet->m_eID;
			
			int iComboIndex = m_comboSkillA.FindString(0, pSkillTemplet->m_wstrName.c_str());
			m_comboSkillA.SetCurSel(iComboIndex);
			m_comboSkillS.SetCurSel(iComboIndex);
			m_comboSkillD.SetCurSel(iComboIndex);
			m_comboSkillC.SetCurSel(iComboIndex);
		}
		++mit;
	}
}

BOOL CSelectToolUnit::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{		
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSelectToolUnit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UNITCLASS, m_comboUnitClass);
	DDX_Control(pDX, IDC_COMBO_SKILLA, m_comboSkillA);
	DDX_Control(pDX, IDC_COMBO_SKILLS, m_comboSkillS);
	DDX_Control(pDX, IDC_COMBO_SKILLD, m_comboSkillD);
	DDX_Control(pDX, IDC_COMBO_SKILLC, m_comboSkillC);
	DDX_Control(pDX, IDC_EDIT3, m_editHp);
	DDX_Control(pDX, IDC_EDIT4, m_editAtkPhysic);
	DDX_Control(pDX, IDC_EDIT5, m_editAtkMagic);
	DDX_Control(pDX, IDC_EDIT6, m_editDefPhysic);
	DDX_Control(pDX, IDC_EDIT7, m_editDefMagic);
	DDX_Control(pDX, IDC_EDIT_UNITLEVEL, m_editLevel);
	DDX_Control(pDX, IDC_EDIT_UNITNAME, m_editUnitName);
}

BEGIN_MESSAGE_MAP(CSelectToolUnit, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_UNITCLASS, &CSelectToolUnit::OnCbnSelchangeComboUnitclass)
	ON_CBN_SELCHANGE(IDC_COMBO_SKILLA, &CSelectToolUnit::OnCbnSelchangeComboSkilla)
	ON_CBN_SELCHANGE(IDC_COMBO_SKILLS, &CSelectToolUnit::OnCbnSelchangeComboSkills)
	ON_CBN_SELCHANGE(IDC_COMBO_SKILLD, &CSelectToolUnit::OnCbnSelchangeComboSkilld)
	ON_CBN_SELCHANGE(IDC_COMBO_SKILLC, &CSelectToolUnit::OnCbnSelchangeComboSkillc)
	ON_EN_CHANGE(IDC_EDIT_UNITNAME, &CSelectToolUnit::OnEnChangeEditUnitname)
	ON_EN_CHANGE(IDC_EDIT_UNITLEVEL, &CSelectToolUnit::OnEnChangeEditUnitlevel)
	ON_EN_CHANGE(IDC_EDIT3, &CSelectToolUnit::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CSelectToolUnit::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CSelectToolUnit::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CSelectToolUnit::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CSelectToolUnit::OnEnChangeEdit7)
END_MESSAGE_MAP()


// CSelectToolUnit �޽��� ó�����Դϴ�.


void CSelectToolUnit::OnCbnSelchangeComboUnitclass()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString csUnitName;
	int iIndex = m_comboUnitClass.GetCurSel();
	m_comboUnitClass.GetLBText(iIndex, csUnitName);

	//map<CX2Unit::UNIT_CLASS, CX2Unit::UnitTemplet*> mapUnitTemplet = g_pData->GetUnitManager()->GetUnitTempletMap4Tool();
	//map<CX2Unit::UNIT_CLASS, CX2Unit::UnitTemplet*>::iterator mit = mapUnitTemplet.begin();

	CX2UnitManager::UnitTempletMap& mapUnitTemplet = g_pData->GetUnitManager()->GetUnitTempletMap4Tool();
	CX2UnitManager::UnitTempletMap::iterator mit = mapUnitTemplet.begin();

	while( mit != mapUnitTemplet.end() )
	{
		const CX2Unit::UnitTemplet* pUnitTemplet = mit->second;
		wstring wstrUnitName = pUnitTemplet->m_Description;
		CX2Unit::UNIT_CLASS unitClass = pUnitTemplet->m_UnitClass;

		if( csUnitName.Compare(wstrUnitName.c_str()) == 0 )
		{
			m_eUnitClass = pUnitTemplet->m_UnitClass;
			break;
		}
		++mit;
	}

	ChangeSkill();
}

int CSelectToolUnit::GetSkillId(wstring wstrSkillName)
{
	std::map< CX2SkillTree::SKILL_ID, CX2SkillTree::SkillTreeTemplet > mapSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTempletMap4Tool( (int)m_eUnitClass );
	std::map<CX2SkillTree::SKILL_ID, CX2SkillTree::SkillTreeTemplet>::iterator mit = mapSkillTreeTemplet.begin();

	while(mit != mapSkillTreeTemplet.end() )
	{
		CX2SkillTree::SKILL_ID iSkillId = (CX2SkillTree::SKILL_ID)mit->first;
		CX2SkillTree::SkillTreeTemplet skillTreeTemplet = (CX2SkillTree::SkillTreeTemplet)mit->second;
		KUserSkillData userSkillData;
#ifdef UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet *pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( iSkillId );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet *pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(iSkillId);
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( pSkillTemplet != NULL && 
			( pSkillTemplet->m_eType == CX2SkillTree::ST_ACTIVE 
			|| pSkillTemplet->m_eType == CX2SkillTree::ST_SPECIAL_ACTIVE 
		#ifdef ADDED_RELATIONSHIP_SYSTEM
			|| pSkillTemplet->m_eType == CX2SkillTree::ST_RELATIONSHIP_SKILL
		#endif // ADDED_RELATIONSHIP_SYSTEM

			) 
			&&
			wstrSkillName.compare(pSkillTemplet->m_wstrName) == 0 )
		{
			return (int)pSkillTemplet->m_eID;
		}
		++mit;
	}

	return -1;
}

void CSelectToolUnit::OnCbnSelchangeComboSkilla()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csSkillName;
	int iIndex = m_comboSkillA.GetCurSel();
	m_comboSkillA.GetLBText(iIndex, csSkillName);

	int iSkillId = GetSkillId(csSkillName.GetString());
	if( iSkillId >= 0 )
		m_iSkillId[0] = iSkillId;
}


void CSelectToolUnit::OnCbnSelchangeComboSkills()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csSkillName;
	int iIndex = m_comboSkillS.GetCurSel();
	m_comboSkillS.GetLBText(iIndex, csSkillName);

	int iSkillId = GetSkillId(csSkillName.GetString());
	if( iSkillId >= 0 )
		m_iSkillId[1] = iSkillId;
}


void CSelectToolUnit::OnCbnSelchangeComboSkilld()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csSkillName;
	int iIndex = m_comboSkillD.GetCurSel();
	m_comboSkillD.GetLBText(iIndex, csSkillName);

	int iSkillId = GetSkillId(csSkillName.GetString());
	if( iSkillId >= 0 )
		m_iSkillId[2] = iSkillId;
}


void CSelectToolUnit::OnCbnSelchangeComboSkillc()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csSkillName;
	int iIndex = m_comboSkillC.GetCurSel();
	m_comboSkillC.GetLBText(iIndex, csSkillName);

	int iSkillId = GetSkillId(csSkillName.GetString());
	if( iSkillId >= 0 )
		m_iSkillId[3] = iSkillId;
}


void CSelectToolUnit::OnEnChangeEditUnitname()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csUnitName;
	m_editUnitName.GetWindowTextW(csUnitName);

	m_wstrUnitName = csUnitName.GetString();
}


void CSelectToolUnit::OnEnChangeEditUnitlevel()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csLv;
	m_editLevel.GetWindowTextW(csLv);
	m_iLevel = _ttoi(csLv.GetString());
}


void CSelectToolUnit::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csHp;
	m_editHp.GetWindowTextW(csHp);
	m_uHp = (UINT)_ttoi(csHp.GetString());
}


void CSelectToolUnit::OnEnChangeEdit4()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	m_editAtkPhysic.GetWindowTextW(csTemp);
	m_iAtkPhysic = _ttoi(csTemp.GetString());
}


void CSelectToolUnit::OnEnChangeEdit5()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	m_editAtkMagic.GetWindowTextW(csTemp);
	m_iAtkMagic = _ttoi(csTemp.GetString());
}


void CSelectToolUnit::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	m_editDefPhysic.GetWindowTextW(csTemp);
	m_iDefPhysic = _ttoi(csTemp.GetString());
}


void CSelectToolUnit::OnEnChangeEdit7()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString csTemp;
	m_editDefMagic.GetWindowTextW(csTemp);
	m_iDefMagic = _ttoi(csTemp.GetString());
}
