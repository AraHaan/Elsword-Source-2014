
#include "stdafx.h"

#include "Resource.h"
#include "MainFrm.h"
#include "X2Tool.h"
#include "PropertiesWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma region strLineType
EnumStringTable strLineType[11] =
{
	{ L"LT_NORMAL",			0 },
	{ L"LT_WALL",			1 },
	{ L"LT_POTAL",			2 },
	{ L"LT_HOLE",			3 },
	{ L"LT_JUMP_UP_REL",	4 },
	{ L"LT_JUMP_UP_ABS",	5 },
	{ L"LT_BUNGEE",			6 },
	{ L"LT_MONSTER_ROAD",	7 },
	{ L"LT_UNIT_ROAD",		8 },
	{ L"LT_OTHER_ROAD",		9 },
	{ L"LT_CLIFF",			10 },
};
#pragma endregion ���� ����

#pragma region strJumpDir
EnumStringTable strJumpDir[3] = 
{
	{ L"LDO_NONE",	0 },
	{ L"LDO_LEFT",	1 },
	{ L"LDO_RIGHT",	2 },
};
#pragma endregion ���� ����

#pragma region strDustType
EnumStringTable strDustType[2] =
{
	{ L"LDT_DEFALT_DUST",	0 },
	{ L"LDT_WATER_SPLASH",	1 },
};
#pragma endregion ���� ����

#pragma region wstrWorldLayer
wstring wstrWorldLayer[] =
{
	L"XL_NONE",	// 0
	L"XL_GAME_BASE",
	L"XL_SKY_DOME_0",
	L"XL_SKY_DOME_1",
	L"XL_SKY_DOME_2",
	L"XL_SKY_WORLD_BASE_0",
	L"XL_SKY_WORLD_BASE_1",
	L"XL_SKY_WORLD_BASE_2",
	L"XL_SKY_WORLD_OBJECT_0",
	L"XL_SKY_WORLD_OBJECT_1",
	L"XL_SKY_WORLD_OBJECT_2",
	L"XL_UNIT_0",
	L"XL_UNIT_1",
	L"XL_UNIT_2",
	L"XL_EFFECT_0",
	L"XL_EFFECT_1",
	L"XL_EFFECT_2",
	L"XL_SKY_WORLD_OBJECT_OVER_EFFECT_0",
	L"XL_SKY_WORLD_OBJECT_OVER_EFFECT_1",
	L"XL_SKY_WORLD_OBJECT_OVER_EFFECT_2",
	L"XL_SKY_WORLD_OBJECT_OVER_EFFECT_3",
	L"XL_EFFECT_OVER_ALL_WORLD_OBJECT_0",
	L"XL_EFFECT_OVER_ALL_WORLD_OBJECT_1",
	L"XL_LENS_FLARE",
	L"XL_MINIMAP",
};
#pragma endregion ���̾�

#pragma region wstrlayerBledType
wstring wstrlayerBledType[] = 
{
	L"����: Normal",
	L"����: lighten/screen",
	L"����: darken/multiply",
	L"����: Spot",
	L"����: overlay",
	L"����: Inverse",
};
#pragma endregion ȥ�� ���� ���

#pragma region wstrBlend
wstring wstrBlend[] = 
{
	L"D3DBLEND_ZERO", //1
	L"D3DBLEND_ONE",
	L"D3DBLEND_SRCCOLOR",
	L"D3DBLEND_INVSRCCOLOR",
	L"D3DBLEND_SRCALPHA",
	L"D3DBLEND_INVSRCALPHA",
	L"D3DBLEND_DESTALPHA",
	L"D3DBLEND_INVDESTALPHA",
	L"D3DBLEND_DESTCOLOR",
	L"D3DBLEND_INVDESTCOLOR",
	L"D3DBLEND_SRCALPHASAT",
	L"D3DBLEND_BOTHSRCALPHA",
	L"D3DBLEND_BOTHINVSRCALPHA",
};
#pragma endregion ȥ�� ����

#pragma region wstrRenderType
wstring wstrRenderType[] = 
{
	L"RT_REAL_COLOR", // = 0,			/// ��ü ����
	L"RT_CARTOON_BLACK_EDGE",		/// ī�� + ���� �� �ܰ���
	L"RT_CARTOON_COLOR_EDGE",		/// ī�� + �ٸ� �� �ܰ���
	L"RT_CARTOON",					/// ī��
	L"RT_ADD_CARTOON_BLACK_EDGE",	/// ī�� + ���� �� �ܰ���
	L"RT_ADD_CARTOON_COLOR_EDGE",	/// ī�� + �ٸ� �� �ܰ���
	L"RT_ADD_CARTOON",				/// ī��
	L"RT_BLACK_EDGE",				/// ���� �� �ܰ���
	L"RT_COLOR_EDGE",				/// �ٸ� �� �ܰ���
};
#pragma endregion ������ ����

#pragma region wstrBillBoardType
wstring wstrBillBoardType[] = 
{
	L"BT_NONE",
	L"BT_ALL",
	L"BT_X",
	L"BT_Y",
	L"BT_Z",
};
#pragma endregion ������ ����

#pragma region wstrWorldSkinPlayType
wstring wstrWorldSkinPlayType[] = 
{
	L"WOSMPY_ONE",
	L"WOSMPY_ONE_WAIT",
	L"WOSMPY_LOOP",
	L"WOSMPY_PINGPONG",
	L"WOSMPY_OCCASIONAL_ONE",
	L"WOSMPY_OCCASIONAL_ONE_WAIT",
};
#pragma endregion ��� ����

#pragma region wstrParticleType
wstring wstrParticleType[] =
{
	L"PT_3D_PLANE",
	L"PT_2D_PLANE",
	L"PT_MESH",
	L"PT_SKINMESH",
};
#pragma endregion �޽� ����

#pragma region wstrParticle
wstring wstrParticle[] =
{
	L"GameMajor",
	L"GameMinor",
	L"UIMajor",
	L"UIMinor",
};
#pragma endregion ��ƼŬ ����

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar �޽��� ó����

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �޺� ���ڸ� ����ϴ�.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("�Ӽ� �޺� ���ڸ� ������ ���߽��ϴ�. \n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndObjectCombo.AddString(_T("���ؽ� �Ӽ�"));
	m_wndObjectCombo.AddString(_T("���� �Ӽ�"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("�Ӽ� ǥ�� ������ ���߽��ϴ�. \n");
		return -1;      // ������ ���߽��ϴ�.
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ��� */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ��� */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// ��� ����� �θ� �������� �ƴ� �� ��Ʈ���� ���� ����õ˴ϴ�.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxMessageBox(L"Vertex Property", MB_OK);
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxMessageBox(L"Line Property", MB_OK);
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	//CMFCPropertyGridProperty::m_strFormatFloat = _T("%5.2f");
	// 
	CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("Land Height"), (_variant_t) (float)0.f, _T("ī�޶� ���̸� �����մϴ�."), PI_EL_LAND_HEIGHT );
	m_wndPropList.AddProperty(pItem);

	// Property Index 0
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Vertex Position"), PI_EL_VERTEX_GROUP);
	{
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T("���õ� ���ؽ��� X��ġ�� �����մϴ�."), PI_EL_VERTEX_X));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T("���õ� ���ؽ��� Y��ġ�� �����մϴ�."), PI_EL_VERTEX_Y));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T("���õ� ���ؽ��� Z��ġ�� �����մϴ�."), PI_EL_VERTEX_Z));		
	}	
	m_wndPropList.AddProperty(pGroup1);

	

	#pragma region PL_EL_LINE_ATTRIBUTE_1
	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Type, Position"), PL_EL_LINE_ATTRIBUTE_1);
	{
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Line Type"), _T("LT_NORMAL"), _T("line type�� �����մϴ�."), PI_EL_LINE_TYPE);
		for(int i=0; i<ARRAY_SIZE(strLineType); ++i)
		{
			pProp->AddOption(strLineType[i].strName);
		}
		pProp->AllowEdit(FALSE);
		pGroup2->AddSubItem( pProp );

		CMFCPropertyGridProperty* pProp2 = new CMFCPropertyGridProperty(_T("Jump_Up_Rel_Dir"), _T("LDO_NONE"), _T("JUMP_UP_REL�� ��� ������ �����մϴ�."), PI_EL_LINE_JUMP_DIR );
		for( int i = 0; i < ARRAY_SIZE( strJumpDir ); ++i )
		{
			pProp2->AddOption(strJumpDir[i].strName);
		}
		pProp2->AllowEdit(FALSE);
		pGroup2->AddSubItem( pProp2 );
		pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Key Stop Time"), (_variant_t) (float)0.f, _T("�����ϴ� ���� �ش� �ð���ŭ Ű �Է��� �����մϴ�."), PI_EL_KEY_LOCK_TIME));

		pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Before Line"), (_variant_t)(long)-1, _T("���� ���ι�ȣ�� �����մϴ�."), PI_EL_BEFORE_LINE));
		pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Next Line"), (_variant_t)(long)-1, _T("���� ���ι�ȣ�� �����մϴ�."), PI_EL_NEXT_LINE));
		pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Line Set"), (_variant_t)(long)-1, _T("�帷ó���� ���� �׷��ȣ�� �����մϴ�."), PI_EL_LINE_SET));
				
		CMFCPropertyGridProperty* pGroup21 = new CMFCPropertyGridProperty(_T("Start Pos"), PI_EL_START_POS_GROUP);		
		{
			pGroup21->AddSubItem(new CMFCPropertyGridProperty(_T("Start X"), (_variant_t) (float)0.f, _T("������ ���� X��ġ�� �����մϴ�."), PI_EL_START_POS_X));
			pGroup21->AddSubItem(new CMFCPropertyGridProperty(_T("Start Y"), (_variant_t) (float)0.f, _T("������ ���� Y��ġ�� �����մϴ�."), PI_EL_START_POS_Y));
			pGroup21->AddSubItem(new CMFCPropertyGridProperty(_T("Start Z"), (_variant_t) (float)0.f, _T("������ ���� Z��ġ�� �����մϴ�."), PI_EL_START_POS_Z));
		}
		pGroup2->AddSubItem( pGroup21 );

		CMFCPropertyGridProperty* pGroup22 = new CMFCPropertyGridProperty(_T("End Pos"), PI_EL_END_POS_GROUP);		
		{
			pGroup22->AddSubItem(new CMFCPropertyGridProperty(_T("End X"), (_variant_t) (float)0.f, _T("������ �� X��ġ�� �����մϴ�."), PI_EL_END_POS_X));
			pGroup22->AddSubItem(new CMFCPropertyGridProperty(_T("End Y"), (_variant_t) (float)0.f, _T("������ �� Y��ġ�� �����մϴ�."), PI_EL_END_POS_Y));
			pGroup22->AddSubItem(new CMFCPropertyGridProperty(_T("End Z"), (_variant_t) (float)0.f, _T("������ �� Z��ġ�� �����մϴ�."), PI_EL_END_POS_Z));
		}
		pGroup2->AddSubItem( pGroup22 );
	}
	m_wndPropList.AddProperty(pGroup2);
	#pragma endregion ����, ��ġ
	
	#pragma region PL_EL_LINE_ATTRIBUTE_2
	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("�ڷ���Ʈ, ī�޶�, Ʈ����"), PL_EL_LINE_ATTRIBUTE_2);
	{
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Teleport ID"), (_variant_t)(long)-1, _T("�ڷ���Ʈ�� ���ι�ȣ�� �����մϴ�."), PI_EL_TELEPORT_LINE));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Camera ID"), (_variant_t)(long)-1, _T("����ī�޶� ��ȣ�� �����մϴ�."), PI_EL_CAMERA_ID));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Trigger ID"), (_variant_t)(long)-1, _T("����Ʈ���� ��ȣ�� �����մϴ�."), PI_EL_TRIGGER_ID));
		pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Trigger On Line"), (_variant_t)(bool)true, _T("Ʈ���� �۵������ �����մϴ�."), PI_EL_TRIGGER_ON_LINE));
	}
	m_wndPropList.AddProperty(pGroup3);
	#pragma endregion �ڷ���Ʈ, ī�޶�, Ʈ����

	#pragma region PL_EL_LINE_ATTRIBUTE_3
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("���� ����"), PL_EL_LINE_ATTRIBUTE_3);
	{
		pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Enable"), (_variant_t)(bool)true, _T("���� Ȱ��ȭ ���θ� �����մϴ�."), PI_EL_ENABLE));
		pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Can Down"), (_variant_t)(bool)true, _T("???"), PI_EL_CAN_DOWN));
		pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Can Pass"), (_variant_t)(bool)true, _T("???"), PI_EL_CAN_PASS));		
		pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Monster Allowed"), (_variant_t)(bool)true, _T("???"), PI_EL_MONSTER_ALLOWED));
		pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Enable Wall Jump"), (_variant_t)(bool)false, _T("???"), PI_EL_ENABLE_WALL_JUMP));
		pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("Other Road Up"), (_variant_t)(bool)false, _T("???"), PI_EL_OTHER_ROAD_UP));
	}
	m_wndPropList.AddProperty(pGroup4);
	#pragma endregion ���� ���� ����

	#pragma region PL_EL_LINE_ATTRIBUTE_4
	CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(_T("����, ���� �Ӽ�"), PL_EL_LINE_ATTRIBUTE_4);
	{
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Dust Type"), _T("LDT_DEFALT_DUST"), _T("dust type�� �����մϴ�."), PI_EL_DUST_TYPE);
		for(int i=0; i<ARRAY_SIZE(strDustType); ++i)
		{
			pProp->AddOption(strDustType[i].strName);
		}
		pProp->AllowEdit(FALSE);
		pGroup5->AddSubItem( pProp );

		pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("Stop Factor"), (_variant_t) (float)1.f, _T("?"), PI_EL_STOP_FACTOR));
		pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("Speed"), (_variant_t) (float)0.f, _T("?"), PI_EL_SPEED));
		pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("Jump Speed X"), (_variant_t) (float)0.f, _T("?"), PI_EL_JUMP_SPEED_X));
		pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("Jump Speed Y"), (_variant_t) (float)0.f, _T("?"), PI_EL_JUMP_SPEED_Y));
		pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("Cliff Height"), (_variant_t) (float)0.f, _T("?"), PI_EL_CLIFF_HEIGHT));
	}
	m_wndPropList.AddProperty(pGroup5);
	#pragma endregion ����, ���� �Ӽ�

	CMFCPropertyGridProperty* pGroup6 = new CMFCPropertyGridProperty(_T("���߶��� �Ӽ�"), PL_EL_LINE_ATTRIBUTE_5);
	{
		pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("���� ����"), (_variant_t)(bool)false, _T("�ش� ������ ���߶��� ���θ� �����մϴ�.\n���߶����� �ѹ��� �������ּ���"), PI_EL_UNDER_WATER));		
		pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("���� ����"), (_variant_t) (float)0.f, _T("�ش� ������ ���� ���̸� �����մϴ�."), PI_EL_UNDERWATER_HEIGHT));
		pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("�η°�"), (_variant_t) (float)0.f, _T("���� �η°��� �����մϴ�."), PI_EL_UNDERWATER_BUOYANCY));
		pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("�̵� ���װ�"), (_variant_t) (float)0.f, _T("���� ���װ��� �����մϴ�."), PI_EL_UNDERWATER_RESISTANCE));
		pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("�ӵ� ���װ�"), (_variant_t) (float)1.f, _T("���� �ӵ����ϰ��� �����մϴ�."), PI_EL_UNDERWATER_ANIMSPEED));
		pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("��� �Ҹ�"), (_variant_t) (float)0.f, _T("���� ��� �Ҹ𵵸� �����մϴ�.\n�Ҹ�:-1, ����:3"), PI_EL_UNDERWATER_VARIANCE_OXYEN));
		CMFCPropertyGridProperty* pGroup61 = new CMFCPropertyGridProperty(_T("Unit Color"), PI_EL_UNITCOLOR_GROUP);
		{
			pGroup61->AddSubItem(new CMFCPropertyGridProperty(_T("Red"), (_variant_t) (float)0.f, _T("�ش� ���λ󿡼��� ���ֻ���(Red)�� �����մϴ�.\n������̰� ����� ��� �Ź� �����ؾ��ϹǷ� ������ �ٸ� ������ ������ �������� �������ּ���.\n(0~1)"), PI_EL_UNITCOLOR_R));
			pGroup61->AddSubItem(new CMFCPropertyGridProperty(_T("Green"), (_variant_t) (float)0.f, _T("�ش� ���λ󿡼��� ���ֻ���(Green)�� �����մϴ�.\n������̰� ����� ��� �Ź� �����ؾ��ϹǷ� ������ �ٸ� ������ ������ �������� �������ּ���.\n(0~1)"), PI_EL_UNITCOLOR_G));
			pGroup61->AddSubItem(new CMFCPropertyGridProperty(_T("Blue"), (_variant_t) (float)0.f, _T("�ش� ���λ󿡼��� ���ֻ���(Blue)�� �����մϴ�.\n������̰� ����� ��� �Ź� �����ؾ��ϹǷ� ������ �ٸ� ������ ������ �������� �������ּ���.\n(0~1)"), PI_EL_UNITCOLOR_B));
			pGroup61->AddSubItem(new CMFCPropertyGridProperty(_T("Force Change Color"), (_variant_t)(bool)false, _T("Unit�� ���� UnitColor���� ������ ������ ������ ������ �� �ֵ��� �����մϴ�."), PI_EL_FORCE_CHANGE_COLOR));				
		}
		pGroup6->AddSubItem(pGroup61);
		
	}
	m_wndPropList.AddProperty(pGroup6);

	// view selected worldobject
	CMFCPropertyGridProperty *pViewSelectedObject = new CMFCPropertyGridProperty(_T("���尴ü ǥ��"), (_variant_t)(bool)true, _T("���õ� Skydome, WorldObject�� ǥ���մϴ�."), PI_EW_WORLD_SELECTED);
	m_wndPropList.AddProperty(pViewSelectedObject);

	// world base attribute
	CMFCPropertyGridProperty* pGroup9 = new CMFCPropertyGridProperty(_T("World"), PI_EW_ATTRIBUTE_3);
	{
		CUserPropertyGridFile* pWorldCamera = new CUserPropertyGridFile(_T("World Camera"), TRUE, _T(""), _T(""), 0, _T("lua script (*.lua)|*.lua|"), _T("����ī�޶� �����մϴ�."), PI_EW_WORLD_CAMERA);
		pGroup9->AddSubItem(pWorldCamera);
		CUserPropertyGridFile* pWorldSound = new CUserPropertyGridFile(_T("BGM"), TRUE, _T(""), _T(""), 0, _T("*.ogg|*.wav|*.mp3"), _T("������� �����մϴ�."), PI_EW_WORLD_SOUND_NAME);
		pGroup9->AddSubItem(pWorldSound);
		CMFCPropertyGridProperty* pGroup90 = new CMFCPropertyGridProperty(_T("Unit Color"), PI_EW_WORLD_UNIT_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_UNIT_COLOR_R );
			pColorPropr->SetRange( 0, 100 );
			pColorPropr->SetFloatValue( 1.f, 0.f );
			pGroup90->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_UNIT_COLOR_G );
			pColorPropg->SetRange( 0, 100 );
			pColorPropg->SetFloatValue( 1.f, 0.f );
			pGroup90->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_UNIT_COLOR_B );
			pColorPropb->SetRange( 0, 100 );
			pColorPropb->SetFloatValue( 1.f, 0.f );
			pGroup90->AddSubItem(pColorPropb);
		}
		pGroup9->AddSubItem( pGroup90 );

		CMFCPropertyGridProperty* pGroup91 = new CMFCPropertyGridProperty(_T("World Color"), PI_EW_WORLD_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_COLOR_R );
			pColorPropr->SetRange( 0, 100 );
			pColorPropr->SetFloatValue( 1.f, 0.f );
			pGroup91->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_COLOR_G );
			pColorPropg->SetRange( 0, 100 );
			pColorPropg->SetFloatValue( 1.f, 0.f );
			pGroup91->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_COLOR_B );
			pColorPropb->SetRange( 0, 100 );
			pColorPropb->SetFloatValue( 1.f, 0.f );
			pGroup91->AddSubItem(pColorPropb);
		}
		pGroup9->AddSubItem( pGroup91 );

		CMFCPropertyGridProperty* pGroup92 = new CMFCPropertyGridProperty(_T("World Light Position"), PI_EW_WORLD_LIGHT_POS_GROUP);
		{
			pGroup92->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T("������ ���� X��ġ�� �����մϴ�."), PI_EW_WORLD_LIGHT_POS_X));
			pGroup92->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T("������ ���� Y��ġ�� �����մϴ�."), PI_EW_WORLD_LIGHT_POS_Y));
			pGroup92->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T("������ ���� Z��ġ�� �����մϴ�."), PI_EW_WORLD_LIGHT_POS_Z));
		}
		pGroup9->AddSubItem( pGroup92 );
	}
	m_wndPropList.AddProperty(pGroup9);

	// world fog
	CMFCPropertyGridProperty* pGroup8 = new CMFCPropertyGridProperty(_T("World Fog"), PI_EW_ATTRIBUTE_2);
	{
		pGroup8->AddSubItem(new CMFCPropertyGridProperty(_T("Enable"), (_variant_t)(bool)false, _T("Fog ��뿩�θ� �����մϴ�. ������ ��� object���� fog ��뿩�ΰ� �ڵ� �����˴ϴ�."), PI_EW_WORLD_FOG_ENABLE));

		float fVal = 0.f;
		CUserPropertyGridSlider* pColorProprNX = new CUserPropertyGridSlider(&m_wndPropList, L"X Near", fVal, PI_EW_WORLD_FOG_NEAR_X );
		pColorProprNX->SetRange( 0, 10000000 );
		pColorProprNX->SetFloatValue( 200000.f, -100000.f );
		pGroup8->AddSubItem(pColorProprNX);
		CUserPropertyGridSlider* pColorProprFX = new CUserPropertyGridSlider(&m_wndPropList, L"X Far", fVal, PI_EW_WORLD_FOG_FAR_X );
		pColorProprFX->SetRange( 0, 10000000 );
		pColorProprFX->SetFloatValue( 200000.f, -100000.f );
		pGroup8->AddSubItem(pColorProprFX);

		CUserPropertyGridSlider* pColorProprNY = new CUserPropertyGridSlider(&m_wndPropList, L"Y Near", fVal, PI_EW_WORLD_FOG_NEAR_Y );
		pColorProprNY->SetRange( 0, 10000000 );
		pColorProprNY->SetFloatValue( 200000.f, -100000.f );
		pGroup8->AddSubItem(pColorProprNY);
		CUserPropertyGridSlider* pColorProprFY = new CUserPropertyGridSlider(&m_wndPropList, L"Y Fay", fVal, PI_EW_WORLD_FOG_FAR_Y );
		pColorProprFY->SetRange( 0, 10000000 );
		pColorProprFY->SetFloatValue( 200000.f, -100000.f );
		pGroup8->AddSubItem(pColorProprFY);

		CMFCPropertyGridProperty* pGroup81 = new CMFCPropertyGridProperty(_T("Fog Color"), PI_EW_WORLD_FOG_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorProprR = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_FOG_COLOR_R );
			pColorProprR->SetRange( 0, 100 );
			pColorProprR->SetFloatValue( 1.f, 0.f );
			pGroup81->AddSubItem(pColorProprR);
			CUserPropertyGridSlider* pColorProprG = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_FOG_COLOR_G );
			pColorProprG->SetRange( 0, 100 );
			pColorProprG->SetFloatValue( 1.f, 0.f );
			pGroup81->AddSubItem(pColorProprG);
			CUserPropertyGridSlider* pColorProprB = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_FOG_COLOR_B );
			pColorProprB->SetRange( 0, 100 );
			pColorProprB->SetFloatValue( 1.f, 0.f );
			pGroup81->AddSubItem(pColorProprB);
		}
		pGroup8->AddSubItem(pGroup81);

		float fValDensity = 1.f;
		CUserPropertyGridSlider* pColorProprDensity = new CUserPropertyGridSlider(&m_wndPropList, L"Density", fValDensity, PI_EW_WORLD_FOG_DENSITY );
		pColorProprDensity->SetRange( 0, 1000 );
		pColorProprDensity->SetFloatValue( 1.f, 0.f );		
		pGroup8->AddSubItem(pColorProprDensity);
	}
	m_wndPropList.AddProperty(pGroup8);

	//world layer
	CMFCPropertyGridProperty* pGroup7 = new CMFCPropertyGridProperty(_T("World Layer"), PI_EW_ATTRIBUTE_1);
	{
		CUserPropertyGridFile* pWorldLayerTex = new CUserPropertyGridFile(_T("���̾� �ؽ���"), TRUE, _T(""), _T(""), 0, _T(""), _T(""), PI_EW_WORLD_LAYER_TEXTURE);
		pGroup7->AddSubItem(pWorldLayerTex);

		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Layer"), _T("XL_LENS_FLARE"), _T("Layer�� �����մϴ�."), PI_EW_WORLD_LAYER_LAYER);
		for(int i=0; i<ARRAY_SIZE(wstrWorldLayer); ++i)
		{
			pProp->AddOption( wstrWorldLayer[i].c_str() );
		}
		pProp->AllowEdit(FALSE);
		pGroup7->AddSubItem( pProp );

		pProp = new CMFCPropertyGridProperty(_T("BlendType"), _T("����: Normal"), _T("BlendType�� �����մϴ�."), PI_EW_WORLD_LAYER_BLENDTYPE);
		for(int i=0; i<ARRAY_SIZE(wstrlayerBledType); ++i)
		{
			pProp->AddOption( wstrlayerBledType[i].c_str() );
		}
		pProp->AllowEdit(FALSE);
		pGroup7->AddSubItem( pProp );
		
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Layer Position X"), (_variant_t) (float)0.f, _T("Layer ���� X��ġ (ȭ������� ���� 0~1)"), PI_EW_WORLD_LAYER_POS_X));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Layer Position Y"), (_variant_t) (float)0.f, _T("Layer ���� Y��ġ (ȭ������� ���� 0~1)"), PI_EW_WORLD_LAYER_POS_Y));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Layer Width"), (_variant_t) (float)1.f, _T("Layer Width (ȭ������� ���� 0~1)"), PI_EW_WORLD_LAYER_WIDTH));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Layer Height"), (_variant_t) (float)1.f, _T("Layer Height (ȭ������� ���� 0~1)"), PI_EW_WORLD_LAYER_HEIGHT));

		CMFCPropertyGridProperty* pGroup71 = new CMFCPropertyGridProperty(_T("Layer Color"), PI_EW_ATTRIBUTE_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_LAYER_COLOR_R );
			pColorPropr->SetRange( 0, 100 );
			pColorPropr->SetFloatValue( 1.f, 0.f );
			pGroup71->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_LAYER_COLOR_G );
			pColorPropg->SetRange( 0, 100 );
			pColorPropg->SetFloatValue( 1.f, 0.f );
			pGroup71->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_LAYER_COLOR_B );
			pColorPropb->SetRange( 0, 100 );
			pColorPropb->SetFloatValue( 1.f, 0.f );
			pGroup71->AddSubItem(pColorPropb);

			CUserPropertyGridSlider* pColorPropa = new CUserPropertyGridSlider(&m_wndPropList, L"Alpha", fVal, PI_EW_WORLD_LAYER_COLOR_A );
			pColorPropa->SetRange( 0, 100 );
			pColorPropa->SetFloatValue( 1.f, 0.f );
			pGroup71->AddSubItem(pColorPropa);
		}
		pGroup7->AddSubItem( pGroup71 );

		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Texture U"), (_variant_t) (float)0.f, _T("Layer Texture UV��ǥ�� �����մϴ�.(u)"), PI_EW_WORLD_LAYER_TEX_U));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Texture V"), (_variant_t) (float)0.f, _T("Layer Texture UV��ǥ�� �����մϴ�.(v)"), PI_EW_WORLD_LAYER_TEX_V));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("FeedBack U"), (_variant_t) (float)0.f, _T("FeedBack�� ���� Layer Texture UV��ǥ�� �����մϴ�.(u)"), PI_EW_WORLD_LAYER_FEEDBACK_U));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("FeedBack V"), (_variant_t) (float)0.f, _T("FeedBack�� ���� Layer Texture UV��ǥ�� �����մϴ�.(v)"), PI_EW_WORLD_LAYER_FEEDBACK_V));
		pGroup7->AddSubItem(new CMFCPropertyGridProperty(_T("Show"), (_variant_t) (bool)true, _T("Layer �ʱ� Show���¸� �����մϴ�. "), PI_EW_WORLD_LAYER_SHOW));
	}	
	m_wndPropList.AddProperty(pGroup7);

	// world skydome
	CMFCPropertyGridProperty* pGroup10 = new CMFCPropertyGridProperty(_T("Sky Dome"), PI_EW_ATTRIBUTE_4);
	{
		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("Show Object"), (_variant_t)(bool)true, _T("����,���� ���ɿ��θ� �����մϴ�."), PI_EW_WORLD_SKYDOME_SHOWOBJECT));
		
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Layer"), _T("XL_SKY_DOME_0"), _T("Layer�� �����մϴ�."), PI_EW_WORLD_SKYDOME_LAYER);
		for(int i=0; i<ARRAY_SIZE(wstrWorldLayer); ++i)
		{
			pProp->AddOption( wstrWorldLayer[i].c_str() );
		}
		pProp->AllowEdit(FALSE);
		pGroup10->AddSubItem( pProp );

		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Position"), PI_EW_WORLD_SKYDOME_POSITION_GROUP);
		{
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_SKYDOME_POSITION_X));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_SKYDOME_POSITION_Y));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_SKYDOME_POSITION_Z));
		}
		pGroup10->AddSubItem( pGroup1 );

		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("Rotation Speed"), (_variant_t) (float)0.f, _T("SkyDome Mesh Rotate Speed�� �����մϴ�."), PI_EW_WORLD_SKYDOME_ROTATION_SPEED));

		CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("TexUV Speed"), PI_EW_WORLD_SKYDOME_TEXUV_SPEED_GROUP);
		{
			pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("U"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_SKYDOME_TEXUV_SPEED_U));
			pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("V"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_SKYDOME_TEXUV_SPEED_V));
		}
		pGroup10->AddSubItem( pGroup2 );
		
		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("View Trace"), (_variant_t)(bool)true, _T("View Trace ���θ� �����մϴ�."), PI_EW_RORLD_SKYDOME_VIEW_TRACE));		
		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("AlphaObject"), (_variant_t)(bool)true, _T("Alpha ���θ� �����մϴ�."), PI_EW_WORLD_SKYDOME_ALPHAOBJECT));
		
		// renderType
		CMFCPropertyGridProperty* pPropRenderType = new CMFCPropertyGridProperty(_T("Render Type"), _T("RT_REAL_COLOR"), _T("Render Type�� �����մϴ�."), PI_EW_WORLD_SKYDOME_RENDERTYPE);
		for(int i=0; i<ARRAY_SIZE(wstrRenderType); ++i)
		{
			pPropRenderType->AddOption( wstrRenderType[i].c_str() );
		}
		pPropRenderType->AllowEdit(FALSE);
		pGroup10->AddSubItem( pPropRenderType );
		// srcBlend
		CMFCPropertyGridProperty* pPropSrcBlend = new CMFCPropertyGridProperty(_T("SrcBlend"), _T("D3DBLEND_SRCALPHA"), _T("SrcBlend Type�� �����մϴ�."), PI_EW_WORLD_SKYDOME_SRCBLEND);
		for(int i=0; i<ARRAY_SIZE(wstrBlend); ++i)
		{
			pPropSrcBlend->AddOption( wstrBlend[i].c_str() );
		}
		pPropSrcBlend->AllowEdit(FALSE);
		pGroup10->AddSubItem( pPropSrcBlend );
		// destBlend
		CMFCPropertyGridProperty* pPropDestBlend = new CMFCPropertyGridProperty(_T("DestBlend"), _T("D3DBLEND_INVSRCALPHA"), _T("DestBlend Type�� �����մϴ�."), PI_EW_WORLD_SKYDOME_DESTBLEND);
		for(int i=0; i<ARRAY_SIZE(wstrBlend); ++i)
		{
			pPropDestBlend->AddOption( wstrBlend[i].c_str() );
		}
		pPropDestBlend->AllowEdit(FALSE);
		pGroup10->AddSubItem( pPropDestBlend );

		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("Use WorldColor"), (_variant_t)(bool)false, _T("����Į�� ������ ������ �����մϴ�."), PI_EW_WORLD_SKYDOME_USE_WORLDCOLOR));

		CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Color"), PI_EW_WORLD_SKYDOME_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_SKYDOME_COLOR_R );
			pColorPropr->SetRange( 0, 100 );
			pColorPropr->SetFloatValue( 1.f, 0.f );
			pGroup3->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_SKYDOME_COLOR_G );
			pColorPropg->SetRange( 0, 100 );
			pColorPropg->SetFloatValue( 1.f, 0.f );
			pGroup3->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_SKYDOME_COLOR_B );
			pColorPropb->SetRange( 0, 100 );
			pColorPropb->SetFloatValue( 1.f, 0.f );
			pGroup3->AddSubItem(pColorPropb);

			CUserPropertyGridSlider* pColorPropa = new CUserPropertyGridSlider(&m_wndPropList, L"Alpha", fVal, PI_EW_WORLD_SKYDOME_COLOR_A );
			pColorPropa->SetRange( 0, 100 );
			pColorPropa->SetFloatValue( 1.f, 0.f );
			pGroup3->AddSubItem(pColorPropa);
		}
		pGroup10->AddSubItem( pGroup3 );

		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("Fog"), (_variant_t)(bool)true, _T("�ش� SkyDome�� Fog�� ������ ������ ���θ� �����մϴ�."), PI_EW_WORLD_SKYDOME_FOG));
		pGroup10->AddSubItem(new CMFCPropertyGridProperty(_T("Use Object Fog"), (_variant_t)(bool)true, _T("�ش� SkyDome�� Fog Color�� World Fog Color�� ������ ������ ���θ� �����մϴ�.\n�����Ǿ� �־�� fog color ���� ��ȿ�մϴ�."), PI_EW_WORLD_SKYDOME_FOG_USEOBJECT));		
		CMFCPropertyGridProperty* pGroupFogSkyDome = new CMFCPropertyGridProperty(_T("Object Fog"), PI_EW_WORLD_SKYDOME_FOG_GROUP);
		{
			float fVal = 0.f;
			CUserPropertyGridSlider* pColorProprNX = new CUserPropertyGridSlider(&m_wndPropList, L"X Near", fVal, PI_EW_WORLD_SKYDOME_FOG_NEAR_X );
			pColorProprNX->SetRange( 0, 10000000 );
			pColorProprNX->SetFloatValue( 200000.f, -100000.f );
			pGroupFogSkyDome->AddSubItem(pColorProprNX);
			CUserPropertyGridSlider* pColorProprFX = new CUserPropertyGridSlider(&m_wndPropList, L"X Far", fVal, PI_EW_WORLD_SKYDOME_FOG_FAR_X );
			pColorProprFX->SetRange( 0, 10000000 );
			pColorProprFX->SetFloatValue( 200000.f, -100000.f );
			pGroupFogSkyDome->AddSubItem(pColorProprFX);

			CUserPropertyGridSlider* pColorProprNY = new CUserPropertyGridSlider(&m_wndPropList, L"Y Near", fVal, PI_EW_WORLD_SKYDOME_FOG_NEAR_Y );
			pColorProprNY->SetRange( 0, 10000000 );
			pColorProprNY->SetFloatValue( 200000.f, -100000.f );
			pGroupFogSkyDome->AddSubItem(pColorProprNY);
			CUserPropertyGridSlider* pColorProprFY = new CUserPropertyGridSlider(&m_wndPropList, L"Y Fay", fVal, PI_EW_WORLD_SKYDOME_FOG_FAR_Y );
			pColorProprFY->SetRange( 0, 10000000 );
			pColorProprFY->SetFloatValue( 200000.f, -100000.f );
			pGroupFogSkyDome->AddSubItem(pColorProprFY);

			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Fog Color"), PI_EW_WORLD_SKYDOME_FOG_COLOR_GROUP);
			{
				float fVal = 1.f;
				CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_SKYDOME_FOG_COLOR_R );
				pColorPropr->SetRange( 0, 100 );
				pColorPropr->SetFloatValue( 1.f, 0.f );
				pGroup4->AddSubItem(pColorPropr);

				CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_SKYDOME_FOG_COLOR_G );
				pColorPropg->SetRange( 0, 100 );
				pColorPropg->SetFloatValue( 1.f, 0.f );
				pGroup4->AddSubItem(pColorPropg);

				CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_SKYDOME_FOG_COLOR_B );
				pColorPropb->SetRange( 0, 100 );
				pColorPropb->SetFloatValue( 1.f, 0.f );
				pGroup4->AddSubItem(pColorPropb);
			}
			pGroupFogSkyDome->AddSubItem( pGroup4 );
		}
		pGroup10->AddSubItem( pGroupFogSkyDome );

		float fDensity = 1.f;
		CUserPropertyGridSlider* pColorPropDensity = new CUserPropertyGridSlider(&m_wndPropList, L"Fog Density", fDensity, PI_EW_WORLD_SKYDOME_FOG_DENSITY );
		pColorPropDensity->SetRange( 0, 100 );
		pColorPropDensity->SetFloatValue( 1.f, 0.f );
		pGroup10->AddSubItem(pColorPropDensity);
	}

	m_wndPropList.AddProperty(pGroup10);
	
	// world object
	CMFCPropertyGridProperty* pGroupWorldObject = new CMFCPropertyGridProperty(_T("World Object"), PI_EW_ATTRIBUTE_5);
	{
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Show Object"), (_variant_t)(bool)true, _T("������Ʈ ����,���� ���ɿ��θ� �����մϴ�."), PI_EW_WORLD_OBJECT_SHOWOBJECT));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Freezing"), (_variant_t)(bool)false, _T("������Ʈ ����,����(�̵�/ȸ��/ũ��) ���� ���θ� �����մϴ�."), PI_EW_WORLD_OBJECT_FREEZING));

		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Layer"), _T("XL_SKY_DOME_0"), _T("Layer�� �����մϴ�."), PI_EW_WORLD_OBJECT_LAYER);
		for(int i=0; i<ARRAY_SIZE(wstrWorldLayer); ++i)
		{
			pProp->AddOption( wstrWorldLayer[i].c_str() );
		}
		pProp->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pProp );

		// particle
		CMFCPropertyGridProperty* pPropParticleSystem = new CMFCPropertyGridProperty(_T("Particle System"), _T("UIMinor"), _T("��ƼŬ �ý����� �����մϴ�."), PI_EW_WORLD_PARTICLE_TYPE);
		for(int i=0; i<ARRAY_SIZE(wstrParticle); ++i)
		{
			pPropParticleSystem->AddOption( wstrParticle[i].c_str() );
		}
		pPropParticleSystem->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropParticleSystem );
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Particle Name"), _T(""), _T("Particle �̸��� �����մϴ�."), PI_EW_WORLD_PARTICLE_NAME));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Delay Time"), (_variant_t) (float)0.f, _T("���� ������ ��ƼŬ ���� �ð��� �����Ѵ�."), PI_EW_WORLD_PARTICLE_DELAYTIME));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Check Cull"), (_variant_t)(bool)true, _T("��ƼŬ �ø����θ� �����Ѵ�."), PI_EW_WORLD_PARTICLE_CULLCHECK));
		CMFCPropertyGridProperty* pPropParticleType = new CMFCPropertyGridProperty(_T("Type"), _T("PT_3D_PLANE"), _T("��ƼŬ Ÿ���� �����Ѵ�."), PI_EW_WORLD_PARTICLE_PARTICLE_TYPE);
		for(int i=0; i<ARRAY_SIZE(wstrParticleType); ++i)
		{
			pPropParticleType->AddOption( wstrParticleType[i].c_str() );
		}
		pPropParticleType->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropParticleType );
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Trigger Time"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_PARTICLE_TRIGGERITME));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Trigger Count"), (_variant_t) (long)0, _T(""), PI_EW_WORLD_PARTICLE_TRIGGERCOUNT));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("EmitMin"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_PARTICLE_EMITMIN));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("EmitMax"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_PARTICLE_EMITMAX));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Draw Count"), (_variant_t) (long)0, _T(""), PI_EW_WORLD_PARTICLE_DRAWCOUNT));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("latency"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_PARTICLE_LATENCY));

		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Position"), PI_EW_WORLD_OBJECT_POSITION_GROUP);
		{
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_POSITION_X));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_POSITION_Y));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_POSITION_Z));
		}
		pGroupWorldObject->AddSubItem( pGroup1 );
		CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Rotation"), PI_EW_WORLD_OBJECT_ROTATION_GROUP);
		{
			pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_ROTATION_X));
			pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_ROTATION_Y));
			pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_ROTATION_Z));
		}
		pGroupWorldObject->AddSubItem( pGroup2 );
		CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Scale"), PI_EW_WORLD_OBJECT_SCALE_GROUP);
		{
			pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_SCALE_X));
			pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_SCALE_Y));
			pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_SCALE_Z));
		}
		pGroupWorldObject->AddSubItem( pGroup3 );
		CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("TexUV Speed"), PI_EW_WORLD_OBJECT_TEXUV_SPEED_GROUP);
		{
			pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("U"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_TEXUV_SPEED_U));
			pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("V"), (_variant_t) (float)0.f, _T(""), PI_EW_WORLD_OBJECT_TEXUV_SPEED_V));
		}
		pGroupWorldObject->AddSubItem( pGroup4 );
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Use WorldColor"), (_variant_t)(bool)true, _T("worldcolor���� ������� �����Ѵ�."), PI_EW_WORLD_OBJECT_USE_WORLDCOLOR));
		CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(_T("Color"), PI_EW_WORLD_OBJECT_COLOR_GROUP);
		{
			float fVal = 1.f;
			CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_OBJECT_COLOR_R );
			pColorPropr->SetRange( 0, 100 );
			pColorPropr->SetFloatValue( 1.f, 0.f );
			pGroup5->AddSubItem(pColorPropr);

			CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_OBJECT_COLOR_G );
			pColorPropg->SetRange( 0, 100 );
			pColorPropg->SetFloatValue( 1.f, 0.f );
			pGroup5->AddSubItem(pColorPropg);

			CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_OBJECT_COLOR_B );
			pColorPropb->SetRange( 0, 100 );
			pColorPropb->SetFloatValue( 1.f, 0.f );
			pGroup5->AddSubItem(pColorPropb);

			CUserPropertyGridSlider* pColorPropa = new CUserPropertyGridSlider(&m_wndPropList, L"Alpha", fVal, PI_EW_WORLD_OBJECT_COLOR_A );
			pColorPropa->SetRange( 0, 100 );
			pColorPropa->SetFloatValue( 1.f, 0.f );
			pGroup5->AddSubItem(pColorPropa);
		}
		pGroupWorldObject->AddSubItem( pGroup5 );		
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Force Show"), (_variant_t)(bool)false, _T("�ɼǿ� ������� ������ �������ǵ��� �����Ѵ�."), PI_EW_WORLD_OBJECT_FORCESHOW));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Can Hide"), (_variant_t)(bool)false, _T("�þ߿� ��������� ����� �ֵ��� �����Ѵ�."), PI_EW_WORLD_OBJECT_CANHIDE));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("NotRenderWhenHidden"), (_variant_t)(bool)false, _T("�þ߿� ��������� ����� �ֵ��� �����Ѵ�."), PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Hide Far Distance"), (_variant_t)(float)0.f, _T("ī�޶���� �Ÿ��� ������ ������ Ŭ ��� ���⵵�� �Ѵ�."), PI_EW_WORLD_OBJECT_HIDE_FAR_DISTANCE));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Hide Near Distance"), (_variant_t)(float)0.f, _T("ī�޶���� �Ÿ��� ������ ������ ���� ��� ���⵵�� �Ѵ�."), PI_EW_WORLD_OBJECT_HIDE_NEAR_DISTANCE));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("2Side Render"), (_variant_t)(bool)false, _T("2Side Render ���θ� �����Ѵ�."), PI_EW_WORLD_OBJECT_NONCULL));
		CMFCPropertyGridProperty* pPropBillBoardType = new CMFCPropertyGridProperty(_T("BillBoard"), _T("BT_NONE"), _T("BillBoard Type�� �����մϴ�."), PI_EW_WORLD_OBJECT_BILLBOARD);
		for(int i=0; i<ARRAY_SIZE(wstrBillBoardType); ++i)
		{
			pPropBillBoardType->AddOption( wstrBillBoardType[i].c_str() );
		}
		pPropBillBoardType->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropBillBoardType );
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Alpha Test"), (_variant_t)(bool)false, _T("Alpha Test ���θ� �����մϴ�."), PI_EW_WORLD_OBJECT_ALPHATEST));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Alpha Blend"), (_variant_t)(bool)false, _T("Alpha Blend ���θ� �����մϴ�."), PI_EW_WORLD_OBJECT_ALPHAOBJECT));
		// renderType
		CMFCPropertyGridProperty* pPropRenderType = new CMFCPropertyGridProperty(_T("Render Type"), _T("RT_REAL_COLOR"), _T("Render Type�� �����մϴ�."), PI_EW_WORLD_OBJECT_RENDERTYPE);
		for(int i=0; i<ARRAY_SIZE(wstrRenderType); ++i)
		{
			pPropRenderType->AddOption( wstrRenderType[i].c_str() );
		}
		pPropRenderType->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropRenderType );
		// srcBlend
		CMFCPropertyGridProperty* pPropSrcBlend = new CMFCPropertyGridProperty(_T("SrcBlend"), _T("D3DBLEND_SRCALPHA"), _T("SrcBlend Type�� �����մϴ�."), PI_EW_WORLD_OBJECT_SRCBLEND);
		for(int i=0; i<ARRAY_SIZE(wstrBlend); ++i)
		{
			pPropSrcBlend->AddOption( wstrBlend[i].c_str() );
		}
		pPropSrcBlend->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropSrcBlend );
		// destBlend
		CMFCPropertyGridProperty* pPropDestBlend = new CMFCPropertyGridProperty(_T("DestBlend"), _T("D3DBLEND_INVSRCALPHA"), _T("DestBlend Type�� �����մϴ�."), PI_EW_WORLD_OBJECT_DESTBLEND);
		for(int i=0; i<ARRAY_SIZE(wstrBlend); ++i)
		{
			pPropDestBlend->AddOption( wstrBlend[i].c_str() );
		}
		pPropDestBlend->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropDestBlend );

		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Fog"), (_variant_t)(bool)true, _T("�ش� WorldObject�� Fog�� ������ ������ ���θ� �����մϴ�."), PI_EW_WORLD_OBJECT_FOG));
		pGroupWorldObject->AddSubItem(new CMFCPropertyGridProperty(_T("Use Object Fog"), (_variant_t)(bool)true, _T("�ش� worldObject�� Fog Color�� World Fog Color�� ������ ������ ���θ� �����մϴ�.\n�����Ǿ� �־�� fog color ���� ��ȿ�մϴ�."), PI_EW_WORLD_OBJECT_FOG_USEOBJECT));
		CMFCPropertyGridProperty* pGroupFog = new CMFCPropertyGridProperty(_T("Object Fog"), PI_EW_WORLD_OBJECT_FOG_GROUP);
		{
			float fVal = 0.f;
			CUserPropertyGridSlider* pColorProprNX = new CUserPropertyGridSlider(&m_wndPropList, L"X Near", fVal, PI_EW_WORLD_OBJECT_FOG_NEAR_X );
			pColorProprNX->SetRange( 0, 10000000 );
			pColorProprNX->SetFloatValue( 200000.f, -100000.f );
			pGroupFog->AddSubItem(pColorProprNX);
			CUserPropertyGridSlider* pColorProprFX = new CUserPropertyGridSlider(&m_wndPropList, L"X Far", fVal, PI_EW_WORLD_OBJECT_FOG_FAR_X );
			pColorProprFX->SetRange( 0, 10000000 );
			pColorProprFX->SetFloatValue( 200000.f, -100000.f );
			pGroupFog->AddSubItem(pColorProprFX);

			CUserPropertyGridSlider* pColorProprNY = new CUserPropertyGridSlider(&m_wndPropList, L"Y Near", fVal, PI_EW_WORLD_OBJECT_FOG_NEAR_Y );
			pColorProprNY->SetRange( 0, 10000000 );
			pColorProprNY->SetFloatValue( 200000.f, -100000.f );
			pGroupFog->AddSubItem(pColorProprNY);
			CUserPropertyGridSlider* pColorProprFY = new CUserPropertyGridSlider(&m_wndPropList, L"Y Fay", fVal, PI_EW_WORLD_OBJECT_FOG_FAR_Y );
			pColorProprFY->SetRange( 0, 10000000 );
			pColorProprFY->SetFloatValue( 200000.f, -100000.f );
			pGroupFog->AddSubItem(pColorProprFY);

			CMFCPropertyGridProperty* pGroupFogColor = new CMFCPropertyGridProperty(_T("Fog Color"), PI_EW_WORLD_OBJECT_FOG_COLOR_GROUP);
			{
				float fVal = 1.f;
				CUserPropertyGridSlider* pColorPropr = new CUserPropertyGridSlider(&m_wndPropList, L"Red", fVal, PI_EW_WORLD_OBJECT_FOG_COLOR_R );
				pColorPropr->SetRange( 0, 100 );
				pColorPropr->SetFloatValue( 1.f, 0.f );
				pGroupFogColor->AddSubItem(pColorPropr);

				CUserPropertyGridSlider* pColorPropg = new CUserPropertyGridSlider(&m_wndPropList, L"Green", fVal, PI_EW_WORLD_OBJECT_FOG_COLOR_G );
				pColorPropg->SetRange( 0, 100 );
				pColorPropg->SetFloatValue( 1.f, 0.f );
				pGroupFogColor->AddSubItem(pColorPropg);

				CUserPropertyGridSlider* pColorPropb = new CUserPropertyGridSlider(&m_wndPropList, L"Blue", fVal, PI_EW_WORLD_OBJECT_FOG_COLOR_B );
				pColorPropb->SetRange( 0, 100 );
				pColorPropb->SetFloatValue( 1.f, 0.f );
				pGroupFogColor->AddSubItem(pColorPropb);
			}
			pGroupFog->AddSubItem( pGroupFogColor );


		}		
		pGroupWorldObject->AddSubItem(pGroupFog);

		float fDensity = 1.f;
		CUserPropertyGridSlider* pColorPropDensity = new CUserPropertyGridSlider(&m_wndPropList, L"Fog Density", fDensity, PI_EW_WORLD_OBJECT_FOG_DENSITY );
		pColorPropDensity->SetRange( 0, 100 );
		pColorPropDensity->SetFloatValue( 1.f, 0.f );
		pGroupWorldObject->AddSubItem(pColorPropDensity);

		CMFCPropertyGridProperty* pPropAnimName = new CMFCPropertyGridProperty(_T("Animation Name"), _T("Animation Name"), _T("Skin Mesh�� animName�� �����մϴ�."), PI_EW_WORLD_OBJECT_ANIM_NAME);	
		pPropAnimName->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropAnimName );
		CMFCPropertyGridProperty* pPropPlayType = new CMFCPropertyGridProperty(_T("Play Type"), _T("WOSMPY_ONE_WAIT"), _T("Play Type�� �����մϴ�."), PI_EW_WORLD_OBJECT_ANIM_TYPE);
		for(int i=0; i<ARRAY_SIZE(wstrWorldSkinPlayType); ++i)
		{
			pPropPlayType->AddOption( wstrWorldSkinPlayType[i].c_str() );
		}
		pPropPlayType->AllowEdit(FALSE);
		pGroupWorldObject->AddSubItem( pPropPlayType );
	}
	m_wndPropList.AddProperty(pGroupWorldObject);
	
	UpdateProp(0);


#if 0
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D ���"), (_variant_t) false, _T("â�� ���� �۲��� �ƴ� �۲��� �����ϰ�, ��Ʈ�ѿ� 3D �׵θ��� �����մϴ�.")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�׵θ�"), _T("Dialog Frame"), _T("None, Thin, Resizable �Ǵ� Dialog Frame �� �ϳ��� �����մϴ�."));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("ĸ��"), (_variant_t) _T("����"), _T("â�� ���� ǥ���ٿ� ǥ�õǴ� �ؽ�Ʈ�� �����մϴ�.")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("â ũ��"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("����"), (_variant_t) 250l, _T("â�� ���̸� �����մϴ�."));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("�ʺ�"), (_variant_t) 150l, _T("â�� �ʺ� �����մϴ�."));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("�۲�"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("���� ���"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("�۲�"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("â�� �⺻ �۲��� �����մϴ�.")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�ý��� �۲��� ����մϴ�."), (_variant_t) true, _T("â���� MS Shell Dlg �۲��� ����ϵ��� �����մϴ�.")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("��Ÿ"));
	pProp = new CMFCPropertyGridProperty(_T("(�̸�)"), _T("���� ���α׷�"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("â ����"), RGB(210, 192, 254), NULL, _T("â�� �⺻ ������ �����մϴ�."));
	pColorProp->EnableOtherButton(_T("��Ÿ..."));
	pColorProp->EnableAutomaticButton(_T("�⺻��"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("������ ����(*.ico)|*.ico|��� ����(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("������"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("â �������� �����մϴ�.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("����"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("����"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("ù��° ���� ����"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�� ��° ���� ����"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�׸� 1"), (_variant_t) _T("�� 1"), _T("�����Դϴ�.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�׸� 2"), (_variant_t) _T("�� 2"), _T("�����Դϴ�.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�׸� 3"), (_variant_t) _T("�� 3"), _T("�����Դϴ�.")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
#endif
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();

	AfxMessageBox(lpszSection, MB_OK);
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}

CMFCPropertyGridProperty* CPropertiesWnd::GetProp(int iData)
{
	return m_wndPropList.FindItemByData(iData);	
}

int CPropertiesWnd::GetEnumIndex(ENUM_TYPE enumType, wstring wstrEnum)
{
	switch(enumType)
	{
	case ET_LINE_TYPE:
		{
			for(int i=0; i<ARRAY_SIZE(strLineType); ++i)
			{
				if( wstrEnum.compare( strLineType[i].strName ) == 0 )
					return strLineType[i].inx;
			}
		} break;

	case ET_DUST_TYPE:
		{
			for(int i=0; i<ARRAY_SIZE(strDustType); ++i)
			{
				if( wstrEnum.compare( strDustType[i].strName ) == 0 )
					return strDustType[i].inx;
			}
		} break;

	case ET_JUMP_DIR:
		{
			for ( int i = 0; i < ARRAY_SIZE( strJumpDir ); ++i )
			{
				if ( wstrEnum.compare( strJumpDir[i].strName ) == 0 )
					return strJumpDir[i].inx;
			}
		} break;

	default:
		break;
	}

	return -1;
}

WCHAR* CPropertiesWnd::GetEnumString(ENUM_TYPE enumType, int iVal)
{
	switch(enumType)
	{
	case ET_LINE_TYPE:
		{
			for(int i=0; i<ARRAY_SIZE(strLineType); ++i)
			{
				if( strLineType[i].inx == iVal )
					return strLineType[i].strName;
			}			
		} break;

	case ET_DUST_TYPE:
		{
			for(int i=0; i<ARRAY_SIZE(strDustType); ++i)
			{
				if( strDustType[i].inx == iVal )
					return strDustType[i].strName;
			}
		} break;

	case ET_JUMP_DIR:
		{
			for ( int i = 0; i < ARRAY_SIZE( strJumpDir ); ++i )
			{
				if ( strJumpDir[i].inx == iVal )
					return strJumpDir[i].strName;
			}
		} break;

	default:
		break;
	}

	return NULL;
}

void CPropertiesWnd::UpdateProp(int iFlag)
{
	switch(iFlag)
	{
	default:
	case 0:
		{
			// None
			GetProp(PI_EL_LAND_HEIGHT)->Show(FALSE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(FALSE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(FALSE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(FALSE);
		}
		break;
	case 1:
		{
			// line vertex
			GetProp(PI_EL_LAND_HEIGHT)->Show(TRUE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(TRUE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(FALSE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(FALSE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(FALSE);
		}
		break;
	case 2:
		{
			// line segment
			GetProp(PI_EL_LAND_HEIGHT)->Show(TRUE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(TRUE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(TRUE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(TRUE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(TRUE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(TRUE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(FALSE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(FALSE);
		}
		break;
	case 3:
		{
			// world base
			GetProp(PI_EL_LAND_HEIGHT)->Show(FALSE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(FALSE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(FALSE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(FALSE);
		}
		break;
	case 4:
		{
			// world layer
			GetProp(PI_EL_LAND_HEIGHT)->Show(FALSE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(FALSE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(FALSE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(FALSE);
		}
		break;
	case 5:
		{
			// world skydome
			GetProp(PI_EL_LAND_HEIGHT)->Show(FALSE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(FALSE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(FALSE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(TRUE);
		}
		break;
	case 6:
		{
			// world object
			GetProp(PI_EL_LAND_HEIGHT)->Show(FALSE);
			GetProp(PI_EL_VERTEX_GROUP)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_2)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_3)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PL_EL_LINE_ATTRIBUTE_5)->Show(FALSE);

			GetProp(PI_EW_ATTRIBUTE_3)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_2)->Show(TRUE);
			GetProp(PI_EW_ATTRIBUTE_1)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_4)->Show(FALSE);
			GetProp(PI_EW_ATTRIBUTE_5)->Show(TRUE);
			GetProp(PI_EW_WORLD_SELECTED)->Show(TRUE);
		}
	}
	
}

LRESULT CPropertiesWnd::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;
	if( !pProp ) 
		return 0;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

	PROPERTY_ID eID = (PROPERTY_ID)pProp->GetData();
	switch( eID )
	{		
	// Line Vertex
	case PI_EL_VERTEX_X:
		{
			pMain->SetVertexPos( D3DXVECTOR3(pProp->GetValue().fltVal, 0.f, 0.f), D3DXVECTOR3(1.f, 0.f, 0.f), true );
		}
		break;
	case PI_EL_VERTEX_Y:
		{
			pMain->SetVertexPos( D3DXVECTOR3(0.f, pProp->GetValue().fltVal, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f), true );
		}
		break;
	case PI_EL_VERTEX_Z:
		{
			pMain->SetVertexPos( D3DXVECTOR3(0.f, 0.f, pProp->GetValue().fltVal), D3DXVECTOR3(0.f, 0.f, 1.f), true );
		}
		break;
	// Land Height
	case PI_EL_LAND_HEIGHT:
		pMain->SetLineProperty(eID, 0, pProp->GetValue().fltVal, L"");
		break;
		
	// Line Segment
	case PI_EL_LINE_TYPE:
		{
			int iEnumLineType = GetEnumIndex( ET_LINE_TYPE, (WCHAR*)pProp->GetValue().bstrVal);
			pMain->SetLineProperty(eID, iEnumLineType, 0.f, L"");
		}
		break;

	case PI_EL_LINE_JUMP_DIR:
		{
			int iEnumJumpDir = GetEnumIndex( ET_JUMP_DIR, (WCHAR*)pProp->GetValue().bstrVal );
			pMain->SetLineProperty( eID, iEnumJumpDir, 0.f, L"" );
		} break;

	case PI_EL_KEY_LOCK_TIME:
		{
			pMain->SetLineProperty( eID, 0, pProp->GetValue().fltVal, L"" );
		} break;

	case PI_EL_BEFORE_LINE:
		pMain->SetLineProperty(eID, pProp->GetValue().intVal, 0.f, L"");
		break;
	case PI_EL_NEXT_LINE:
		pMain->SetLineProperty(eID, pProp->GetValue().intVal, 0.f, L"");
		break;
	case PI_EL_LINE_SET:
		pMain->SetLineProperty(eID, pProp->GetValue().intVal, 0.f, L"");
		break;
	case PI_EL_START_POS_X:
	case PI_EL_START_POS_Y:
	case PI_EL_START_POS_Z:
	case PI_EL_END_POS_X:
	case PI_EL_END_POS_Y:
	case PI_EL_END_POS_Z:
		pMain->SetLineProperty(eID, 0, pProp->GetValue().fltVal, L"");
		break;

	case PI_EL_TELEPORT_LINE:
		pMain->SetLineProperty(eID, pProp->GetValue().intVal, 0.f, L"");
		break;
	case PI_EL_CAMERA_ID:
		pMain->SetLineProperty(eID, pProp->GetValue().intVal, 0.f, L"");
		break;
	case PI_EL_TRIGGER_ID:
		pMain->SetLineProperty(eID, pProp->GetValue().intVal, 0.f, L"");
		break;
	case PI_EL_TRIGGER_ON_LINE:
		pMain->SetLineProperty(eID, 0, 0.f, L"",  pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;

	case PI_EL_ENABLE:
	case PI_EL_CAN_DOWN:
	case PI_EL_CAN_PASS:
	case PI_EL_ENABLE_WALL_JUMP:
	case PI_EL_MONSTER_ALLOWED:
	case PI_EL_OTHER_ROAD_UP:
		pMain->SetLineProperty(eID, 0, 0.f, L"",  pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;

	case PI_EL_DUST_TYPE:
		{
			int iEnumDustType = GetEnumIndex( ET_DUST_TYPE, (WCHAR*)pProp->GetValue().bstrVal);
			pMain->SetLineProperty(eID, iEnumDustType, 0.f, L"");
		}
		break;		
	case PI_EL_STOP_FACTOR:
	case PI_EL_SPEED:
	case PI_EL_JUMP_SPEED_X:
	case PI_EL_JUMP_SPEED_Y:
	case PI_EL_CLIFF_HEIGHT:
		pMain->SetLineProperty( eID, 0, pProp->GetValue().fltVal, L"" );
		break;

	case PI_EL_UNDER_WATER:
		pMain->SetLineProperty( eID, 0, 0.f, L"",  pProp->GetValue().boolVal  == VARIANT_TRUE ? true : false );
		break;
	case PI_EL_UNDERWATER_HEIGHT:
	case PI_EL_UNDERWATER_BUOYANCY:
	case PI_EL_UNDERWATER_RESISTANCE:
	case PI_EL_UNDERWATER_ANIMSPEED:
	case PI_EL_UNDERWATER_VARIANCE_OXYEN:
	case PI_EL_UNITCOLOR_R:
	case PI_EL_UNITCOLOR_G:
	case PI_EL_UNITCOLOR_B:
		pMain->SetLineProperty(eID, 0, pProp->GetValue().fltVal, L"");
		break;
	case PI_EL_FORCE_CHANGE_COLOR:
		pMain->SetLineProperty( eID, 0, 0.f, L"",  pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;

	case PI_EW_WORLD_SELECTED:
		{			
			pMain->m_bViewSelectedWorldobject = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
			CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->OnViewWorldObjectSelected();
		}		
		break;

	// world layer
	case PI_EW_WORLD_LAYER_TEXTURE:
		{			
			pMain->SetWorldlayerProperty(eID, 0, 0.f, (WCHAR*)pProp->GetValue().bstrVal);
			CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->m_DlgWorldEditor.ResetWorldLayer(false);
		}		
		break;	
	case PI_EW_WORLD_LAYER_LAYER:
		{
			wstring wstrLayer = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrWorldLayer); ++j)
			{
				if( wstrLayer.compare(wstrWorldLayer[j]) == 0 )
				{
					pMain->SetWorldlayerProperty(eID, j, 0.f, L"");
					CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
					pMainFrame->m_DlgWorldEditor.ResetWorldLayer(false);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_LAYER_BLENDTYPE:
		{
			wstring wstrbt = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrlayerBledType); ++j)
			{
				if( wstrbt.compare( wstrlayerBledType[j] ) == 0 )
				{
					pMain->SetWorldlayerProperty(eID, j, 0.f, L"");
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_LAYER_POS_X:
	case PI_EW_WORLD_LAYER_POS_Y:
	case PI_EW_WORLD_LAYER_WIDTH:
	case PI_EW_WORLD_LAYER_HEIGHT:
	case PI_EW_WORLD_LAYER_TEX_U:
	case PI_EW_WORLD_LAYER_TEX_V:
	case PI_EW_WORLD_LAYER_FEEDBACK_U:
	case PI_EW_WORLD_LAYER_FEEDBACK_V:
		pMain->SetWorldlayerProperty(eID, 0, pProp->GetValue().fltVal, L"");
		break;
	case PI_EW_WORLD_LAYER_SHOW:
		pMain->SetWorldlayerProperty(eID, 0, 0.f, L"",  pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;
	case PI_EW_WORLD_LAYER_COLOR_R:
	case PI_EW_WORLD_LAYER_COLOR_G:
	case PI_EW_WORLD_LAYER_COLOR_B:
	case PI_EW_WORLD_LAYER_COLOR_A:
		{
			CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
			pPropSlider->UpdateSlider();
			pMain->SetWorldlayerProperty(eID, 0, pPropSlider->GetValue().fltVal, L"");
		}
		break;

	// world fog
	case PI_EW_WORLD_FOG_ENABLE:
		pMain->SetEnableWorldFog( pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;
	case PI_EW_WORLD_FOG_NEAR_X:
	case PI_EW_WORLD_FOG_FAR_X:
	case PI_EW_WORLD_FOG_NEAR_Y:
	case PI_EW_WORLD_FOG_FAR_Y:
	case PI_EW_WORLD_FOG_COLOR_R:
	case PI_EW_WORLD_FOG_COLOR_G:
	case PI_EW_WORLD_FOG_COLOR_B:
	case PI_EW_WORLD_FOG_DENSITY:
		{
			CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
			pPropSlider->UpdateSlider();
			float fVal = pPropSlider->GetValue().fltVal;
			pMain->SetWorldFogProperty(eID, fVal);
		}
		break;

	// world base
	case PI_EW_WORLD_CAMERA:
		{
			string strCamera;
			ConvertWCHARToChar(strCamera, (WCHAR*)pProp->GetValue().bstrVal);
			if( strCamera.empty() == false )
				g_pX2Game->GetWorld()->SetWorldCamera_LUA( strCamera.c_str() );
		}
		break;
	case PI_EW_WORLD_SOUND_NAME:
		{
			pMain->m_wstrSoundName = (WCHAR*)pProp->GetValue().bstrVal;
			string strBGMName;
			ConvertWCHARToChar(strBGMName, pMain->m_wstrSoundName.c_str());
			g_pX2Game->GetWorld()->SetBGM_LUA(strBGMName.c_str());
			g_pX2Game->GetWorld()->StopBGM();
			g_pX2Game->GetWorld()->PlayBGM();
		}
		break;
	case PI_EW_WORLD_UNIT_COLOR_R:
	case PI_EW_WORLD_UNIT_COLOR_G:
	case PI_EW_WORLD_UNIT_COLOR_B:	
	case PI_EW_WORLD_COLOR_R:
	case PI_EW_WORLD_COLOR_G:
	case PI_EW_WORLD_COLOR_B:
		{
			CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
			pPropSlider->UpdateSlider();
			float fVal = pPropSlider->GetValue().fltVal;
			pMain->SetWorldBaseProperty(eID, fVal);
		}
		break;
	case PI_EW_WORLD_LIGHT_POS_X:
	case PI_EW_WORLD_LIGHT_POS_Y:
	case PI_EW_WORLD_LIGHT_POS_Z:
		pMain->SetWorldBaseProperty(eID, (float)pProp->GetValue().fltVal);
		break;

	case PI_EW_WORLD_SKYDOME_LAYER:
		{
			wstring wstrLayer = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrWorldLayer); ++j)
			{
				if( wstrLayer.compare(wstrWorldLayer[j]) == 0 )
				{
					pMain->SetSkyDomeProperty(eID, j);
					break;
				}
			}
			CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->m_DlgWorldEditor.ResetWorldSkyDome(false);
		}
		break;
	case PI_EW_WORLD_SKYDOME_RENDERTYPE:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrRenderType); ++j)
			{
				if( wstrType.compare(wstrRenderType[j]) == 0 )
				{
					pMain->SetSkyDomeProperty(eID, j);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_SKYDOME_SRCBLEND:
	case PI_EW_WORLD_SKYDOME_DESTBLEND:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrBlend); ++j)
			{
				if( wstrType.compare(wstrBlend[j]) == 0 )
				{
					pMain->SetSkyDomeProperty(eID, j+1);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_SKYDOME_POSITION_X:
	case PI_EW_WORLD_SKYDOME_POSITION_Y:
	case PI_EW_WORLD_SKYDOME_POSITION_Z:
	case PI_EW_WORLD_SKYDOME_ROTATION_SPEED:
	case PI_EW_WORLD_SKYDOME_TEXUV_SPEED_U:
	case PI_EW_WORLD_SKYDOME_TEXUV_SPEED_V:	
	case PI_EW_WORLD_SKYDOME_COLOR_R:
	case PI_EW_WORLD_SKYDOME_COLOR_G:
	case PI_EW_WORLD_SKYDOME_COLOR_B:
	case PI_EW_WORLD_SKYDOME_COLOR_A:	
	case PI_EW_WORLD_SKYDOME_FOG_COLOR_R:
	case PI_EW_WORLD_SKYDOME_FOG_COLOR_G:
	case PI_EW_WORLD_SKYDOME_FOG_COLOR_B:
	case PI_EW_WORLD_SKYDOME_FOG_NEAR_X:
	case PI_EW_WORLD_SKYDOME_FOG_FAR_X:
	case PI_EW_WORLD_SKYDOME_FOG_NEAR_Y:
	case PI_EW_WORLD_SKYDOME_FOG_FAR_Y:
	case PI_EW_WORLD_SKYDOME_FOG_DENSITY:
		pMain->SetSkyDomeProperty( eID, (float)pProp->GetValue().fltVal );
		break;
	case PI_EW_WORLD_SKYDOME_SHOWOBJECT:
		{
			pMain->SetSkyDomeProperty( eID, pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
			CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->m_DlgWorldEditor.ResetWorldSkyDome( false );
		}
	case PI_EW_RORLD_SKYDOME_VIEW_TRACE:
	case PI_EW_WORLD_SKYDOME_ALPHAOBJECT:
	case PI_EW_WORLD_SKYDOME_USE_WORLDCOLOR:
	case PI_EW_WORLD_SKYDOME_FOG:
		pMain->SetSkyDomeProperty( eID, pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;
	case PI_EW_WORLD_SKYDOME_FOG_USEOBJECT:
		{
			bool bShow = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
			pMain->SetSkyDomeProperty( eID, bShow );
			
			if( bShow )
				GetProp(PI_EW_WORLD_SKYDOME_FOG_GROUP)->Show(TRUE);
			else
			{
				GetProp(PI_EW_WORLD_SKYDOME_FOG_GROUP)->Show(FALSE);

			}
		}		
		break;

	// world object
	case PI_EW_WORLD_OBJECT_LAYER:
		{
			wstring wstrLayer = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrWorldLayer); ++j)
			{
				if( wstrLayer.compare(wstrWorldLayer[j]) == 0 )
				{
					pMain->SetWorldObjectProperty(eID, j);
					break;
				}
			}
			CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->m_DlgWorldEditor.ResetWorldObject(false);
		}
		break;
	case PI_EW_WORLD_OBJECT_BILLBOARD:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrBillBoardType); ++j)
			{
				if( wstrType.compare(wstrBillBoardType[j]) == 0 )
				{
					pMain->SetWorldObjectProperty(eID, j);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_OBJECT_RENDERTYPE:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrRenderType); ++j)
			{
				if( wstrType.compare(wstrRenderType[j]) == 0 )
				{
					pMain->SetWorldObjectProperty(eID, j);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_OBJECT_SRCBLEND:
	case PI_EW_WORLD_OBJECT_DESTBLEND:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrBlend); ++j)
			{
				if( wstrType.compare(wstrBlend[j]) == 0 )
				{
					pMain->SetWorldObjectProperty(eID, j+1);
					break;
				}
			}
		}
		break;	
	case PI_EW_WORLD_OBJECT_ANIM_TYPE:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrWorldSkinPlayType); ++j)
			{
				if( wstrType.compare(wstrWorldSkinPlayType[j]) == 0 )
				{
					pMain->SetWorldObjectProperty(eID, j);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_PARTICLE_PARTICLE_TYPE:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			for(int j=0; j<ARRAY_SIZE(wstrParticleType); ++j)
			{
				if( wstrType.compare(wstrParticleType[j]) == 0 )
				{
					pMain->SetWorldObjectProperty(eID, j);
					break;
				}
			}
		}
		break;
	case PI_EW_WORLD_PARTICLE_TYPE:
		{
			AfxMessageBox(L"�ش��׸��� �����Ͻ� �� �����ϴ�.", MB_OK);
		}
		break;
	case PI_EW_WORLD_PARTICLE_TRIGGERCOUNT:
	case PI_EW_WORLD_PARTICLE_DRAWCOUNT:
		pMain->SetWorldObjectProperty(eID, (int)pProp->GetValue().intVal);
		break;
	case PI_EW_WORLD_PARTICLE_NAME:
	case PI_EW_WORLD_OBJECT_ANIM_NAME:
		{
			wstring wstrType = (WCHAR*)pProp->GetValue().bstrVal;
			pMain->SetWorldObjectProperty(eID, wstrType);
		}		
		break;
	case PI_EW_WORLD_OBJECT_POSITION_X:
	case PI_EW_WORLD_OBJECT_POSITION_Y:
	case PI_EW_WORLD_OBJECT_POSITION_Z:			
	case PI_EW_WORLD_OBJECT_ROTATION_X:
	case PI_EW_WORLD_OBJECT_ROTATION_Y:
	case PI_EW_WORLD_OBJECT_ROTATION_Z:			
	case PI_EW_WORLD_OBJECT_SCALE_X:
	case PI_EW_WORLD_OBJECT_SCALE_Y:
	case PI_EW_WORLD_OBJECT_SCALE_Z:			
	case PI_EW_WORLD_OBJECT_TEXUV_SPEED_U:
	case PI_EW_WORLD_OBJECT_TEXUV_SPEED_V:
	case PI_EW_WORLD_OBJECT_COLOR_R:
	case PI_EW_WORLD_OBJECT_COLOR_G:
	case PI_EW_WORLD_OBJECT_COLOR_B:
	case PI_EW_WORLD_OBJECT_COLOR_A:
	case PI_EW_WORLD_OBJECT_HIDE_FAR_DISTANCE:
	case PI_EW_WORLD_OBJECT_HIDE_NEAR_DISTANCE:
	case PI_EW_WORLD_OBJECT_FOG_COLOR_R:
	case PI_EW_WORLD_OBJECT_FOG_COLOR_G:
	case PI_EW_WORLD_OBJECT_FOG_COLOR_B:
	case PI_EW_WORLD_OBJECT_FOG_DENSITY:
	case PI_EW_WORLD_PARTICLE_DELAYTIME:
	case PI_EW_WORLD_PARTICLE_TRIGGERITME:
	case PI_EW_WORLD_PARTICLE_EMITMIN:
	case PI_EW_WORLD_PARTICLE_EMITMAX:
	case PI_EW_WORLD_PARTICLE_LATENCY:
	case PI_EW_WORLD_OBJECT_FOG_NEAR_X:
	case PI_EW_WORLD_OBJECT_FOG_FAR_X:
	case PI_EW_WORLD_OBJECT_FOG_NEAR_Y:
	case PI_EW_WORLD_OBJECT_FOG_FAR_Y:
		pMain->SetWorldObjectProperty(eID, (float)pProp->GetValue().fltVal);
		break;
	case PI_EW_WORLD_OBJECT_SHOWOBJECT:
	case PI_EW_WORLD_OBJECT_FREEZING:
		{
			pMain->SetWorldObjectProperty( eID, pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
			CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->m_DlgWorldEditor.ResetWorldObject(false);
		}
		break;
	case PI_EW_WORLD_OBJECT_USE_WORLDCOLOR:
	case PI_EW_WORLD_OBJECT_FORCESHOW:
	case PI_EW_WORLD_OBJECT_CANHIDE:
	case PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN:
	case PI_EW_WORLD_OBJECT_NONCULL:
	case PI_EW_WORLD_OBJECT_ALPHATEST:
	case PI_EW_WORLD_OBJECT_ALPHAOBJECT:
	case PI_EW_WORLD_OBJECT_FOG:	
	case PI_EW_WORLD_PARTICLE_CULLCHECK:
		pMain->SetWorldObjectProperty( eID, pProp->GetValue().boolVal == VARIANT_TRUE ? true : false );
		break;
	
	case PI_EW_WORLD_OBJECT_FOG_USEOBJECT:
		{
			bool bShow = pProp->GetValue().boolVal == VARIANT_TRUE ? true : false;
			pMain->SetWorldObjectProperty(eID, (bool)bShow);
			if( bShow )
				GetProp(PI_EW_WORLD_OBJECT_FOG_GROUP)->Show(TRUE);
			else
				GetProp(PI_EW_WORLD_OBJECT_FOG_GROUP)->Show(FALSE);
		}
		break;
	default:
		break;		
	}

	return 0;
}

void CPropertiesWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	CDockablePane::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPropertiesWnd::UpdateWorldlayer()
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldLayer*> vecWorldlayer = pWorld->GetWorldLayerList();

	for(UINT i=0; i<vecWorldlayer.size(); ++i)
	{									
		CX2WorldLayer *pWorldLayer = vecWorldlayer[i];
		if( pWorldLayer != NULL && pWorldLayer->m_bSelected == true )
		{
			SetPropValue( PI_EW_WORLD_LAYER_TEXTURE, (_variant_t)pWorldLayer->GetTexName().c_str() );
			for(int j=0; j<ARRAY_SIZE( wstrWorldLayer ); ++j)
			{
				if( j== pWorldLayer->GetLayer() )
				{
					SetPropValue( PI_EW_WORLD_LAYER_LAYER, (_variant_t)wstrWorldLayer[j].c_str() );
					break;
				}
			}
			for(int j=0; j<ARRAY_SIZE( wstrlayerBledType ); ++j)
			{
				if( j == pWorldLayer->GetBlendType() )
				{
					SetPropValue( PI_EW_WORLD_LAYER_BLENDTYPE, (_variant_t)wstrlayerBledType[j].c_str() );
				}
			}
			float fx, fy, fw, fh;
			pWorldLayer->GetPos(&fx, &fy, &fw, &fh);
			SetPropValue( PI_EW_WORLD_LAYER_POS_X, (_variant_t)(float)fx );
			SetPropValue( PI_EW_WORLD_LAYER_POS_Y, (_variant_t)(float)fy );
			SetPropValue( PI_EW_WORLD_LAYER_WIDTH, (_variant_t)(float)fw );
			SetPropValue( PI_EW_WORLD_LAYER_HEIGHT, (_variant_t)(float)fh );
			D3DXCOLOR vColor = pWorldLayer->GetColor();
			SetPropValue( PI_EW_WORLD_LAYER_COLOR_R, (_variant_t)(float)vColor.r );
			SetPropValue( PI_EW_WORLD_LAYER_COLOR_G, (_variant_t)(float)vColor.g );
			SetPropValue( PI_EW_WORLD_LAYER_COLOR_B, (_variant_t)(float)vColor.b );
			SetPropValue( PI_EW_WORLD_LAYER_COLOR_A, (_variant_t)(float)vColor.a );
			D3DXVECTOR2 vTexUV = pWorldLayer->GetTexUV();
			SetPropValue( PI_EW_WORLD_LAYER_TEX_U, (_variant_t)(float)vTexUV.x );
			SetPropValue( PI_EW_WORLD_LAYER_TEX_V, (_variant_t)(float)vTexUV.y );

			D3DXVECTOR2 vFeedBackUV = pWorldLayer->GetFeedBackUV();
			SetPropValue( PI_EW_WORLD_LAYER_FEEDBACK_U, (_variant_t)(float)vFeedBackUV.x );
			SetPropValue( PI_EW_WORLD_LAYER_FEEDBACK_V, (_variant_t)(float)vFeedBackUV.y );
						
			SetPropValue( PI_EW_WORLD_LAYER_SHOW, (_variant_t)(bool)pWorldLayer->GetInitShow() );

			break;
		}
	}
}

void CPropertiesWnd::UpdateWorldBase()
{
	CX2World *pWorld = g_pX2Game->GetWorld();

	SetPropValue( PI_EW_WORLD_CAMERA, (_variant_t)pWorld->GetWorldCamera().c_str() );	
	SetPropValue( PI_EW_WORLD_SOUND_NAME, (_variant_t)pWorld->GetBGMName().c_str() );

	D3DXCOLOR vUnitColor = pWorld->GetLightColor();
	SetPropValue( PI_EW_WORLD_UNIT_COLOR_R, (_variant_t)(float)vUnitColor.r );
	SetPropValue( PI_EW_WORLD_UNIT_COLOR_G, (_variant_t)(float)vUnitColor.g );
	SetPropValue( PI_EW_WORLD_UNIT_COLOR_B, (_variant_t)(float)vUnitColor.b );

	D3DXCOLOR vWorldColor = pWorld->GetOriginColor();
	SetPropValue( PI_EW_WORLD_COLOR_R, (_variant_t)(float)vWorldColor.r );
	SetPropValue( PI_EW_WORLD_COLOR_G, (_variant_t)(float)vWorldColor.g );
	SetPropValue( PI_EW_WORLD_COLOR_B, (_variant_t)(float)vWorldColor.b );

	D3DXVECTOR3 vLightPos = pWorld->GetLightPos();
	SetPropValue( PI_EW_WORLD_LIGHT_POS_X, (_variant_t)(float)vLightPos.x );
	SetPropValue( PI_EW_WORLD_LIGHT_POS_Y, (_variant_t)(float)vLightPos.y );
	SetPropValue( PI_EW_WORLD_LIGHT_POS_Z, (_variant_t)(float)vLightPos.z );
}

void CPropertiesWnd::UpdateWorldFog()
{
	CX2World *pWorld = g_pX2Game->GetWorld();

	SetPropValue( PI_EW_WORLD_FOG_ENABLE, (_variant_t)(bool)pWorld->m_bFogShow );
	SetPropValue( PI_EW_WORLD_FOG_NEAR_X, (_variant_t)(float)pWorld->m_fNearX );
	SetPropValue( PI_EW_WORLD_FOG_FAR_X, (_variant_t)(float)pWorld->m_fFarX );
	SetPropValue( PI_EW_WORLD_FOG_NEAR_Y, (_variant_t)(float)pWorld->m_fNearY );
	SetPropValue( PI_EW_WORLD_FOG_FAR_Y, (_variant_t)(float)pWorld->m_fFarY );
	SetPropValue( PI_EW_WORLD_FOG_COLOR_R, (_variant_t)(float)pWorld->m_FogColor.r );
	SetPropValue( PI_EW_WORLD_FOG_COLOR_G, (_variant_t)(float)pWorld->m_FogColor.g );
	SetPropValue( PI_EW_WORLD_FOG_COLOR_B, (_variant_t)(float)pWorld->m_FogColor.b );
	SetPropValue( PI_EW_WORLD_FOG_DENSITY, (_variant_t)(float)pWorld->m_fDensity );	
}

void CPropertiesWnd::UpdateWorldSkyDome()
{
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CKTDGSkyDome*> vecSkyDome = pWorld->GetSkyDomeList();

	CX2ToolMain *pToolMain = (CX2ToolMain *)g_pMain;
	pToolMain->m_bViewSelectedWorldobject = true;
	SetPropValue( PI_EW_WORLD_SELECTED, (_variant_t)(bool)pToolMain->m_bViewSelectedWorldobject );	

	for(UINT i=0; i<vecSkyDome.size(); ++i)
	{									
		CKTDGSkyDome *pSkyDome = vecSkyDome[i];
		if( pSkyDome != NULL && pSkyDome->m_bSelected == true )
		{
			SetPropValue( PI_EW_WORLD_SKYDOME_SHOWOBJECT, (_variant_t)(bool)pSkyDome->m_bShowObject );

			for(int j=0; j<ARRAY_SIZE( wstrWorldLayer ); ++j)
			{
				if( j==  pSkyDome->GetLayer() )
				{
					SetPropValue( PI_EW_WORLD_SKYDOME_LAYER, (_variant_t)wstrWorldLayer[j].c_str() );
					break;
				}
			}

			SetPropValue( PI_EW_WORLD_SKYDOME_POSITION_X, (_variant_t)(float)pSkyDome->GetMatrix().GetXPos() );
			SetPropValue( PI_EW_WORLD_SKYDOME_POSITION_Y, (_variant_t)(float)pSkyDome->GetMatrix().GetYPos() );
			SetPropValue( PI_EW_WORLD_SKYDOME_POSITION_Z, (_variant_t)(float)pSkyDome->GetMatrix().GetZPos() );
			SetPropValue( PI_EW_WORLD_SKYDOME_ROTATION_SPEED, (_variant_t)(float)pSkyDome->GetRotationSpeed() );
			SetPropValue( PI_EW_WORLD_SKYDOME_TEXUV_SPEED_U, (_variant_t)(float)pSkyDome->GetTex0UVSpeed().x );
			SetPropValue( PI_EW_WORLD_SKYDOME_TEXUV_SPEED_V, (_variant_t)(float)pSkyDome->GetTex0UVSpeed().y );
			SetPropValue( PI_EW_RORLD_SKYDOME_VIEW_TRACE, (_variant_t)(bool)pSkyDome->GetViewTrace() );
			SetPropValue( PI_EW_WORLD_SKYDOME_ALPHAOBJECT, (_variant_t)(bool)pSkyDome->IsAlphaObject() );

			for(int j=0; j<ARRAY_SIZE( wstrRenderType ); ++j)
			{
				if( j ==  (int)pSkyDome->GetOriRenderParam()->renderType )
				{
					SetPropValue( PI_EW_WORLD_SKYDOME_RENDERTYPE, (_variant_t)wstrRenderType[j].c_str() );
					break;
				}
			}			
			for(int j=0; j<ARRAY_SIZE( wstrBlend ); ++j)
			{
				if( j + 1 ==  (int)pSkyDome->GetOriRenderParam()->srcBlend )
				{
					SetPropValue( PI_EW_WORLD_SKYDOME_SRCBLEND, (_variant_t)wstrBlend[j].c_str() );
					break;
				}
			}
			for(int j=0; j<ARRAY_SIZE( wstrBlend ); ++j)
			{
				if( j + 1 ==  (int)pSkyDome->GetOriRenderParam()->destBlend )
				{
					SetPropValue( PI_EW_WORLD_SKYDOME_DESTBLEND, (_variant_t)wstrBlend[j].c_str() );
					break;
				}
			}

			SetPropValue( PI_EW_WORLD_SKYDOME_USE_WORLDCOLOR, (_variant_t)(bool)pSkyDome->GetUseWorldColor() );
			SetPropValue( PI_EW_WORLD_SKYDOME_COLOR_R, (_variant_t)(float)pSkyDome->GetSkyDomeColor().r );
			SetPropValue( PI_EW_WORLD_SKYDOME_COLOR_G, (_variant_t)(float)pSkyDome->GetSkyDomeColor().g );
			SetPropValue( PI_EW_WORLD_SKYDOME_COLOR_B, (_variant_t)(float)pSkyDome->GetSkyDomeColor().b );
			SetPropValue( PI_EW_WORLD_SKYDOME_COLOR_A, (_variant_t)(float)pSkyDome->GetSkyDomeColor().a );

			SetPropValue( PI_EW_WORLD_SKYDOME_FOG, (_variant_t)(bool)pSkyDome->IsEnableObjectFog() );
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_USEOBJECT, (_variant_t)(bool)pSkyDome->m_bUseObjectFogProperty );			
			
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_COLOR_R, (_variant_t)(float)pSkyDome->GetRenderParam()->fogColor.r );
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_COLOR_G, (_variant_t)(float)pSkyDome->GetRenderParam()->fogColor.g );
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_COLOR_B, (_variant_t)(float)pSkyDome->GetRenderParam()->fogColor.b );

			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_NEAR_X, (_variant_t)(float)pSkyDome->GetRenderParam()->fogNearX );
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_FAR_X, (_variant_t)(float)pSkyDome->GetRenderParam()->fogFarX );
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_NEAR_Y, (_variant_t)(float)pSkyDome->GetRenderParam()->fogNearY );
			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_FAR_Y, (_variant_t)(float)pSkyDome->GetRenderParam()->fogFarY );			

			if( pSkyDome->m_bUseObjectFogProperty == false )
			{
				GetProp(PI_EW_WORLD_SKYDOME_FOG_GROUP)->Show(FALSE);
			}

			SetPropValue( PI_EW_WORLD_SKYDOME_FOG_DENSITY, (_variant_t)(float)pSkyDome->GetRenderParam()->fogDensity );

			break;
		}
	}
}

void CPropertiesWnd::UpdateWorldObject()
{
	bool bFind = false;
	static int iViewIndex = -1;
	CX2World *pWorld = g_pX2Game->GetWorld();
	vector<CX2WorldObjectPtr> vecWorldObjectList = pWorld->GetWorldObjectList4Tool();

	CX2ToolMain *pToolMain = (CX2ToolMain *)g_pMain;
	pToolMain->m_bViewSelectedWorldobject = true;
	SetPropValue( PI_EW_WORLD_SELECTED, (_variant_t)(bool)pToolMain->m_bViewSelectedWorldobject );	
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnViewWorldObjectSelected();
	
	if( iViewIndex >= 0 && iViewIndex < (int)vecWorldObjectList.size() )
	{
		CX2WorldObject *pWorldObject = vecWorldObjectList[iViewIndex].get();
		if( pWorldObject->m_bSelected == true )
			return;
	}

	for(UINT i=0; i<vecWorldObjectList.size(); ++i)
	{	
		bool bParticleObject = false;
		CX2WorldObject *pWorldObject = vecWorldObjectList[i].get();
		if( pWorldObject != NULL && pWorldObject->m_bSelected == true )
		{
			if( pWorldObject->GetObjectType() == CX2WorldObject::OT_PARTICLE )
				bParticleObject = true;

			if( bParticleObject == false )
			{
				bool bStatic = true;
				if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					bStatic = false;

				// static/skin mesh
				GetProp(PI_EW_WORLD_OBJECT_SHOWOBJECT)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_FREEZING)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_LAYER)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_POSITION_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_ROTATION_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_SCALE_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_TEXUV_SPEED_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_USE_WORLDCOLOR)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_COLOR_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_FORCESHOW)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_CANHIDE)->Show(TRUE);
				if( bStatic == true )
					GetProp(PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN)->Show(TRUE);
				else
					GetProp(PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_HIDE_FAR_DISTANCE)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_HIDE_NEAR_DISTANCE)->Show(TRUE);
				if( bStatic == true )
					GetProp(PI_EW_WORLD_OBJECT_NONCULL)->Show(TRUE);
				else
					GetProp(PI_EW_WORLD_OBJECT_NONCULL)->Show(FALSE);
				if( bStatic == true )
					GetProp(PI_EW_WORLD_OBJECT_BILLBOARD)->Show(TRUE);
				else 
					GetProp(PI_EW_WORLD_OBJECT_BILLBOARD)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_ALPHATEST)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_ALPHAOBJECT)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_RENDERTYPE)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_SRCBLEND)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_DESTBLEND)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_FOG)->Show(TRUE);				
				GetProp(PI_EW_WORLD_OBJECT_FOG_USEOBJECT)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_FOG_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_FOG_DENSITY)->Show(TRUE);
				if( bStatic == true )
				{
					GetProp(PI_EW_WORLD_OBJECT_ANIM_NAME)->Show(FALSE);
					GetProp(PI_EW_WORLD_OBJECT_ANIM_TYPE)->Show(FALSE);
				}
				else
				{
					GetProp(PI_EW_WORLD_OBJECT_ANIM_NAME)->Show(TRUE);
					GetProp(PI_EW_WORLD_OBJECT_ANIM_TYPE)->Show(TRUE);
				}				

				GetProp(PI_EW_WORLD_PARTICLE_TYPE)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_NAME)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_DELAYTIME)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_CULLCHECK)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_PARTICLE_TYPE)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_TRIGGERITME)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_TRIGGERCOUNT)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_EMITMIN)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_EMITMAX)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_DRAWCOUNT)->Show(FALSE);
				GetProp(PI_EW_WORLD_PARTICLE_LATENCY)->Show(FALSE);
			}
			else
			{
				// particle
				GetProp(PI_EW_WORLD_OBJECT_SHOWOBJECT)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_FREEZING)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_LAYER)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_POSITION_GROUP)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_ROTATION_GROUP)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_SCALE_GROUP)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_TEXUV_SPEED_GROUP)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_USE_WORLDCOLOR)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_COLOR_GROUP)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_FORCESHOW)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_CANHIDE)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN)->Show(FALSE);				
				GetProp(PI_EW_WORLD_OBJECT_HIDE_FAR_DISTANCE)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_HIDE_NEAR_DISTANCE)->Show(TRUE);
				GetProp(PI_EW_WORLD_OBJECT_NONCULL)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_BILLBOARD)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_ALPHATEST)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_ALPHAOBJECT)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_RENDERTYPE)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_SRCBLEND)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_DESTBLEND)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_FOG)->Show(FALSE);				
				GetProp(PI_EW_WORLD_OBJECT_FOG_USEOBJECT)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_FOG_GROUP)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_FOG_DENSITY)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_ANIM_NAME)->Show(FALSE);
				GetProp(PI_EW_WORLD_OBJECT_ANIM_TYPE)->Show(FALSE);

				GetProp(PI_EW_WORLD_PARTICLE_TYPE)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_NAME)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_DELAYTIME)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_CULLCHECK)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_PARTICLE_TYPE)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_TRIGGERITME)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_TRIGGERCOUNT)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_EMITMIN)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_EMITMAX)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_DRAWCOUNT)->Show(TRUE);
				GetProp(PI_EW_WORLD_PARTICLE_LATENCY)->Show(TRUE);
			}


			SetPropValue( PI_EW_WORLD_OBJECT_SHOWOBJECT, (_variant_t)(bool)pWorldObject->m_bShowObject );
			SetPropValue( PI_EW_WORLD_OBJECT_FREEZING, (_variant_t)(bool)pWorldObject->m_bFreezing );

			for(int j=0; j<ARRAY_SIZE( wstrWorldLayer ); ++j)
			{
				if( j ==  pWorldObject->GetLayer() )
				{
					SetPropValue( PI_EW_WORLD_OBJECT_LAYER, (_variant_t)wstrWorldLayer[j].c_str() );
					break;
				}
			}

			if( bParticleObject == true )
			{
				CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle *)pWorldObject;
								
				SetPropValue( PI_EW_WORLD_PARTICLE_TYPE, (_variant_t)wstrParticle[pObject->m_eParticleType].c_str() );
				GetProp(PI_EW_WORLD_PARTICLE_TYPE)->Enable(FALSE);

				SetPropValue( PI_EW_WORLD_PARTICLE_NAME, (_variant_t)(WCHAR*)pObject->GetParticleName().c_str() );
				SetPropValue( PI_EW_WORLD_PARTICLE_DELAYTIME, (_variant_t)(float)pObject->GetDelayTime() );
				SetPropValue( PI_EW_WORLD_PARTICLE_CULLCHECK, (_variant_t)(float)pObject->GetCullCheck() );
				if( pObject->m_bChangeType == true )
				{
					for(int j=0; j<ARRAY_SIZE( wstrParticleType ); ++j)
					{
						if( j ==  pObject->m_ptType )
						{
							SetPropValue( PI_EW_WORLD_PARTICLE_PARTICLE_TYPE, (_variant_t)wstrParticleType[j].c_str() );
							break;
						}
					}
				}
				SetPropValue( PI_EW_WORLD_PARTICLE_TRIGGERITME, (_variant_t)(float)pObject->m_fTriggerTime );
				SetPropValue( PI_EW_WORLD_PARTICLE_TRIGGERCOUNT, (_variant_t)(long)pObject->m_iTriggerCount );
				SetPropValue( PI_EW_WORLD_PARTICLE_EMITMIN, (_variant_t)(float)pObject->m_fEmitMin );
				SetPropValue( PI_EW_WORLD_PARTICLE_EMITMAX, (_variant_t)(float)pObject->m_fEmitMax );
				SetPropValue( PI_EW_WORLD_PARTICLE_DRAWCOUNT, (_variant_t)(long)pObject->m_iDrawCount );
				SetPropValue( PI_EW_WORLD_PARTICLE_LATENCY, (_variant_t)(float)pObject->m_fLatency );
			}

			CKTDGMatrix matrix = pWorldObject->GetMatrix();
			SetPropValue( PI_EW_WORLD_OBJECT_POSITION_X, (_variant_t)(float)matrix.GetPos().x );
			SetPropValue( PI_EW_WORLD_OBJECT_POSITION_Y, (_variant_t)(float)matrix.GetPos().y );
			SetPropValue( PI_EW_WORLD_OBJECT_POSITION_Z, (_variant_t)(float)matrix.GetPos().z );

			if( bParticleObject == false )
			{
				SetPropValue( PI_EW_WORLD_OBJECT_ROTATION_X, (_variant_t)(float)matrix.GetXRotateDegree() );
				SetPropValue( PI_EW_WORLD_OBJECT_ROTATION_Y, (_variant_t)(float)matrix.GetYRotateDegree() );
				SetPropValue( PI_EW_WORLD_OBJECT_ROTATION_Z, (_variant_t)(float)matrix.GetZRotateDegree() );
				SetPropValue( PI_EW_WORLD_OBJECT_SCALE_X, (_variant_t)(float)matrix.GetXScale() );
				SetPropValue( PI_EW_WORLD_OBJECT_SCALE_Y, (_variant_t)(float)matrix.GetYScale() );
				SetPropValue( PI_EW_WORLD_OBJECT_SCALE_Z, (_variant_t)(float)matrix.GetZScale() );
			
				D3DXVECTOR2 vUVSpeed = D3DXVECTOR2(0.f, 0.f); 
				if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
				{
					CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
					vUVSpeed = pObject->GetTex0UVSpeed();
				}
				else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
				{
					CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
					vUVSpeed = pObject->GetTex0UVSpeed();
				}
				SetPropValue( PI_EW_WORLD_OBJECT_TEXUV_SPEED_U, (_variant_t)(float)vUVSpeed.x );
				SetPropValue( PI_EW_WORLD_OBJECT_TEXUV_SPEED_V, (_variant_t)(float)vUVSpeed.y );

				SetPropValue( PI_EW_WORLD_OBJECT_USE_WORLDCOLOR, (_variant_t)(bool)pWorldObject->GetUseWorldColor() );
				SetPropValue( PI_EW_WORLD_OBJECT_COLOR_R, (_variant_t)(float)pWorldObject->GetOriginalColorFactor().r );
				SetPropValue( PI_EW_WORLD_OBJECT_COLOR_G, (_variant_t)(float)pWorldObject->GetOriginalColorFactor().g );
				SetPropValue( PI_EW_WORLD_OBJECT_COLOR_B, (_variant_t)(float)pWorldObject->GetOriginalColorFactor().b );
				SetPropValue( PI_EW_WORLD_OBJECT_COLOR_A, (_variant_t)(float)pWorldObject->GetOriginalColorFactor().a );

				SetPropValue( PI_EW_WORLD_OBJECT_FORCESHOW, (_variant_t)(bool)pWorldObject->GetForceShow() );
			}

			bool bCanHide = false;
			bool bNotRenderWhenHidden = false;
			if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
			{
				CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
				bCanHide = pObject->GetCanHide();
				bNotRenderWhenHidden = pObject->GetNotRenderWhenHidden();
			}
			else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
			{
				CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
				bCanHide = pObject->GetCanHide();				
			}
			else if( bParticleObject == true )
			{
				CX2WorldObjectParticle *pObject = (CX2WorldObjectParticle*)pWorldObject;
				bCanHide = pObject->GetCanHide();
			}
			SetPropValue( PI_EW_WORLD_OBJECT_CANHIDE, (_variant_t)(bool)bCanHide );

			if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
			{
				GetProp(PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN)->Show(FALSE);
			}
			else
			{
				SetPropValue( PI_EW_WORLD_OBJECT_NOTRENDER_HIDDEN, (_variant_t)(bool)bNotRenderWhenHidden );
			}

			SetPropValue( PI_EW_WORLD_OBJECT_HIDE_FAR_DISTANCE, (_variant_t)(float)pWorldObject->GetHideDistance() );
			SetPropValue( PI_EW_WORLD_OBJECT_HIDE_NEAR_DISTANCE, (_variant_t)(float)pWorldObject->GetHideNearDistance() );

			if( bParticleObject == false )
			{
				bool bNotCull = false;
				if( pWorldObject->GetRenderParam()->cullMode == D3DCULL_NONE )
					bNotCull = true;
				SetPropValue( PI_EW_WORLD_OBJECT_NONCULL, (_variant_t)(bool)bNotCull );

				CKTDGMatrix::BILLBOARD_TYPE billboardType = CKTDGMatrix::BT_NONE;
				if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
				{
					CX2WorldObjectMesh *pObject = (CX2WorldObjectMesh*)pWorldObject;
					billboardType = pObject->GetBillboardType();
				}
				else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
				{
					CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
					billboardType = pObject->GetBillboardType();
				}
				for(int j=0; j<ARRAY_SIZE( wstrBillBoardType ); ++j)
				{
					if( j ==  (int)billboardType )
					{
						SetPropValue( PI_EW_WORLD_OBJECT_BILLBOARD, (_variant_t)wstrBillBoardType[j].c_str() );
						break;
					}
				}

				bool bAlphaTest = pWorldObject->GetRenderParam()->bAlphaTestEnable;
				SetPropValue( PI_EW_WORLD_OBJECT_ALPHATEST, (_variant_t)(bool)bAlphaTest );
				SetPropValue( PI_EW_WORLD_OBJECT_ALPHAOBJECT, (_variant_t)(bool)pWorldObject->IsAlphaObject() );

				for(int j=0; j<ARRAY_SIZE( wstrRenderType ); ++j)
				{
					if( j ==  (int)pWorldObject->GetOriRenderParam()->renderType )
					{
						SetPropValue( PI_EW_WORLD_OBJECT_RENDERTYPE, (_variant_t)wstrRenderType[j].c_str() );
						break;
					}
				}			
				for(int j=0; j<ARRAY_SIZE( wstrBlend ); ++j)
				{
					if( j + 1 ==  (int)pWorldObject->GetOriRenderParam()->srcBlend )
					{
						SetPropValue( PI_EW_WORLD_OBJECT_SRCBLEND, (_variant_t)wstrBlend[j].c_str() );
						break;
					}
				}
				for(int j=0; j<ARRAY_SIZE( wstrBlend ); ++j)
				{
					if( j + 1 ==  (int)pWorldObject->GetOriRenderParam()->destBlend )
					{
						SetPropValue( PI_EW_WORLD_OBJECT_DESTBLEND, (_variant_t)wstrBlend[j].c_str() );
						break;
					}
				}

				SetPropValue( PI_EW_WORLD_OBJECT_FOG, (_variant_t)(bool)pWorldObject->IsEnableObjectFog() );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_USEOBJECT, (_variant_t)(bool)pWorldObject->m_bUseObjectFogProperty );		

				SetPropValue( PI_EW_WORLD_OBJECT_FOG_NEAR_X, (_variant_t)(float)pWorldObject->GetRenderParam()->fogNearX );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_FAR_X, (_variant_t)(float)pWorldObject->GetRenderParam()->fogFarX );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_NEAR_Y, (_variant_t)(float)pWorldObject->GetRenderParam()->fogNearY );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_FAR_Y, (_variant_t)(float)pWorldObject->GetRenderParam()->fogFarY );

				SetPropValue( PI_EW_WORLD_OBJECT_FOG_COLOR_R, (_variant_t)(float)pWorldObject->GetRenderParam()->fogColor.r );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_COLOR_G, (_variant_t)(float)pWorldObject->GetRenderParam()->fogColor.g );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_COLOR_B, (_variant_t)(float)pWorldObject->GetRenderParam()->fogColor.b );
				SetPropValue( PI_EW_WORLD_OBJECT_FOG_DENSITY, (_variant_t)(float)pWorldObject->GetRenderParam()->fogDensity );

				if( pWorldObject->m_bUseObjectFogProperty == false )
				{
					GetProp(PI_EW_WORLD_OBJECT_FOG_GROUP)->Show(FALSE);
				}

				if( pWorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
				{
					GetProp(PI_EW_WORLD_OBJECT_ANIM_NAME)->Show(FALSE);
					GetProp(PI_EW_WORLD_OBJECT_ANIM_TYPE)->Show(FALSE);
				}
				else if( pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
				{
					GetProp(PI_EW_WORLD_OBJECT_ANIM_TYPE)->Show(TRUE);
					CMFCPropertyGridProperty *pPropAnimName = GetProp(PI_EW_WORLD_OBJECT_ANIM_NAME);
					pPropAnimName->Show(TRUE);				

					CX2WorldObjectSkinMesh *pObject = (CX2WorldObjectSkinMesh*)pWorldObject;
					std::vector<wstring> vecAnimList;
					vecAnimList.clear();
					pObject->GetXSkinAnim()->GetAnimNameList(vecAnimList);
					pPropAnimName->RemoveAllOptions();
					for(UINT j=0; j<vecAnimList.size(); ++j)
					{					
						pPropAnimName->AddOption( vecAnimList[j].c_str() );
					}
					SetPropValue( PI_EW_WORLD_OBJECT_ANIM_NAME, (_variant_t)pObject->m_wstrOriAnimName.c_str() );

					for(int j=0; j<ARRAY_SIZE( wstrWorldSkinPlayType ); ++j)
					{
						if( j ==  (int)pObject->m_iOriPlayType )
						{
							SetPropValue( PI_EW_WORLD_OBJECT_ANIM_TYPE, (_variant_t)wstrWorldSkinPlayType[j].c_str() );
							break;
						}
					}
				}
			}
			
			bFind = true;
			iViewIndex = i;
			break;
		}
	}

	if( bFind == false )
	iViewIndex = -1;
}