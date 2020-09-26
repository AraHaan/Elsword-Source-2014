/**
	@file	MainFrm.cpp
	@author	Wonpok
	@brief	CMainFrame Ŭ������ ����
		
	@date	JHKang on 2013/02/19,
			- ��� �ľ�
			- ���� �۾�( �ּ�ó��, �� ������, ���� ���� ���� )
*/

#include "stdafx.h"
#include "X2Tool.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

#pragma region ���� ����
const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	//	ID_USER_STRING1,
	// 	ID_INDICATOR_CAPS,
	// 	ID_INDICATOR_NUM,
	// 	ID_INDICATOR_SCRL,
};
#pragma endregion iMaxUserToolbars, uiFirstUserToolBarId, uiLastUserToolBarId, indicators

#pragma region MESSAGE_MAP
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_EDIT_LINEMAP, &CMainFrame::OnEditLinemap)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LINEMAP, &CMainFrame::OnUpdateEditLinemap)
	ON_COMMAND(ID_EDIT_WORLD, &CMainFrame::OnEditWorld)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WORLD, &CMainFrame::OnUpdateEditWorld)
	ON_COMMAND(ID_VIEW_LINE, &CMainFrame::OnViewLine)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE, &CMainFrame::OnUpdateViewLine)
	ON_COMMAND(ID_VIEW_LINEINFO, &CMainFrame::OnViewLineInfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINEINFO, &CMainFrame::OnUpdateViewLineInfo)
	ON_COMMAND(ID_VIEW_WORLD, &CMainFrame::OnViewWorld)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORLD, &CMainFrame::OnUpdateViewWorld)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_WORLDOBJECT_MOVE, &CMainFrame::OnWorldObjectMove)
	ON_COMMAND(ID_WORLDOBJECT_ROTATION, &CMainFrame::OnWorldobjectRotation)
	ON_COMMAND(ID_WORLDOBJECT_SCALE, &CMainFrame::OnWorldobjectScale)
	ON_COMMAND(ID_VIEW_WORLDOBJECT, &CMainFrame::OnViewWorldObject)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORLDOBJECT, &CMainFrame::OnUpdateViewWorldobject)
	ON_COMMAND(ID_EDIT_DUNGEON, &CMainFrame::OnEditDungeon)
	ON_COMMAND(ID_VIEW_LINE_NUMBER, &CMainFrame::OnViewLineNumber)
	ON_COMMAND(ID_VIEW_LINE_SET_NUMBER, &CMainFrame::OnViewLineSetNumber)
	ON_COMMAND(ID_VIEW_LINE_CAMERA_NUMBER, &CMainFrame::OnViewLineCameraNumber)
	ON_COMMAND(ID_VIEW_LINE_TRIGGER_NUMBER, &CMainFrame::OnViewLineTriggerNumber)
	ON_COMMAND(ID_VIEW_WATER_INFO, &CMainFrame::OnViewWaterInfo)
	ON_COMMAND(ID_TRIGGER_BCK, &CMainFrame::OnTriggerBck)
END_MESSAGE_MAP()
#pragma endregion �޽��� ��

#pragma region CMainFrame
CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}
#pragma endregion ������, �⺻���� Frame ���¸� ����(Visual Studio 2008 Frame)

#pragma region ~CMainFrame
CMainFrame::~CMainFrame()
{
}
#pragma endregion Empty �Ҹ���

#pragma region OnCreate
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CFrameWndEx::OnCreate(lpCreateStruct) == -1 )
		return -1;

	BOOL bNameValid;
	
	OnApplicationLook(theApp.m_nAppLook);	// ������ ���� ���� ���־� ������ �� ��Ÿ���� �����մϴ�.

	#pragma region m_wndMenuBar
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	#pragma endregion �޴��� ����

	#pragma region m_wndToolBar
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME) )
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;
	}

	m_wndToolBar.SetSizes( CSize(24, 38), CSize(24, 38) );
	m_wndToolBar.SetHeight( 38 );

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// ����� ���� ���� ���� �۾��� ����մϴ�.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	#pragma endregion ���� ����

	#pragma region m_wndStatusBar
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	#pragma endregion ���� ǥ���� ����

	#pragma region Docking
	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Ž�� â�� ���ʿ� ��������Ƿ� �Ͻ������� ���ʿ� ��ŷ�� ��Ȱ��ȭ�մϴ�.
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);
	#pragma endregion �Ӽ�

	#pragma region �߰� ������ ���̾�α� 
	// �޴� �׸� �̹����� �ε��մϴ�(ǥ�� ���� ������ ����).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	
	// �Ϲ� ���̾�α� �ʱ�ȭ�Ѵ�.
	m_DlgLineBaseEditor.Create(IDD_DIALOG_EDIT_LINE, this);	
	m_DlgLineBaseEditor.SetWindowPos( &wndTop, 1000, 200, 1, 1, SWP_NOMOVE | SWP_NOSIZE );
	
	m_DlgWorldEditor.Create(IDD_DIALOG_EDIT_WORLD, this);
	m_DlgWorldEditor.SetWindowPos( &wndTop, 1000, 200, 1, 2, SWP_NOMOVE | SWP_NOSIZE );
	
	m_DlgDungeonEditor.Create(IDD_DIALOG_DUNGEON_EDITOR, this);
	m_DlgDungeonEditor.SetWindowPos( &wndTop, 1000, 200, 1, 3, SWP_NOMOVE | SWP_NOSIZE );
	m_DlgDungeonEditor.ShowWindow(SW_SHOW);

	m_DlgSelectUnit.Create(IDD_DIALOG_SELECT_UNIT, this);
	m_DlgSelectUnit.SetWindowPos( &wndTop, 1000, 200, 1, 3, SWP_NOMOVE | SWP_NOSIZE );
	m_DlgSelectUnit.ShowWindow(SW_HIDE);

	m_DlgTriggerEdit.Create( IDD_DIALOG_TRIGGER, this );
	m_DlgTriggerEdit.SetWindowPos( &wndTop, 1000, 200, 1, 3, SWP_NOMOVE | SWP_NOSIZE );

	CreateDockingWindowAfter();
	
#if 0
	// ��ŷ â�� ����ϴ�.
	if (!CreateDockingWindows())
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
#endif

	#pragma endregion ���� ������, ���� ������, ���� ������, ���� ����â ����
	
	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// ����� ���� ���� ���� �̹����� �ε��մϴ�.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	#pragma region �ֱ� ����� ���
	// �޴� ���� ������ Ȱ��ȭ�մϴ�(���� �ֱٿ� ����� ���).
	// TODO: ������� �⺻ ����� �����Ͽ� �� Ǯ�ٿ� �޴��� �ϳ� �̻��� �⺻ ����� ���Խ�ŵ�ϴ�.

	/*CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
 	lstBasicCommands.AddTail(ID_APP_ABOUT);
 	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
 	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
 	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
 	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
 	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
 	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
 	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);
	lstBasicCommands.AddTail(ID_EDIT_LINEMAP);
	lstBasicCommands.AddTail(ID_EDIT_WORLD);
	lstBasicCommands.AddTail(ID_VIEW_LINEINFO);
	lstBasicCommands.AddTail(ID_VIEW_LINE);
	lstBasicCommands.AddTail(ID_VIEW_WORLD);
	lstBasicCommands.AddTail(ID_VIEW_WORLDOBJECT);
	
	CMFCToolBar::SetBasicCommands(lstBasicCommands);*/
	#pragma endregion ������ ����̶� ����

	/* JHKang / 2013.2.19 / �̸� ��ü��
	   X2GameTool --> Elsword Map Tool
	*/
	this->SetWindowTextW(L"Elsword Map Tool");
	return 0;
}
#pragma endregion �⺻���� UI ����

#pragma region PreCreateWindow
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}
#pragma endregion �� ����

#pragma region CreateDockingWindows
BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	#pragma region m_wndClassView
	CString strClassView = L"���� ����Ʈ";
 	bNameValid = strClassView.LoadString( IDS_CLASS_VIEW );
 	ASSERT( bNameValid );
 	if ( !m_wndClassView.Create( strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW,
							    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI ) )
 	{
 		TRACE0( "Ŭ���� �� â�� ������ ���߽��ϴ�.\n" );
 		return FALSE;
 	}
	#pragma endregion Ŭ���� �� ����
	
	#pragma region m_wndProperties
	CString strPropertiesWnd =  L"�Ӽ�";
 	bNameValid = strPropertiesWnd.LoadString( IDS_PROPERTIES_WND );
 	ASSERT( bNameValid );
	if ( !m_wndProperties.Create( strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND,
								 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "�Ӽ� â�� ������ ���߽��ϴ�.\n" );
		return FALSE;
	}
	#pragma endregion �Ӽ� â ����

	#pragma region m_wndTriggerProperties
	CString strTriggerPropertiesWnd =  L"Ʈ���� �Ӽ�";
	bNameValid = strPropertiesWnd.LoadString( IDS_PROPERTIES_WND );
	ASSERT( bNameValid );
	if ( !m_wndTriggerProperties.Create( strTriggerPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, 4,
										 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "Ʈ���� �Ӽ� â�� ������ ���߽��ϴ�.\n" );
		return FALSE;
	}
	#pragma endregion Ʈ���� �Ӽ� â ����
	
	#pragma region m_wndDungeonInfo
	if ( !m_wndDungeonInfo.CreateEx( NULL, _T("���� ����"), this, CRect(0,0,100,100), true, 1,
									 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "���� ���� â�� ������ ���߽��ϴ�.\n" );
		return FALSE;
	}
	
	m_wndDungeonInfo.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndDungeonInfo );
	#pragma endregion ���� ���� â ����

	#pragma region m_wndCameraInfo
	if ( !m_wndCameraInfo.CreateEx( NULL, _T("ī�޶�"), this, CRect(0,0,100,100), true, 2,
									WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "ī�޶� ���� â�� ������ ���߽��ϴ�.\n" );
		return FALSE;
	}

	m_wndCameraInfo.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCameraInfo);
	#pragma endregion ī�޶� ���� â ����

	#pragma region m_wndStartPosition
	if ( !m_wndStartPosition.CreateEx( NULL, _T("���� ��ġ"), this, CRect(0,0,100,100), true, 3,
								 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI ) )
	{
		TRACE0( "���� ��ġ ���� â�� ������ ���߽��ϴ�.\n" );
		return FALSE;
	}

	m_wndStartPosition.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndStartPosition);
	#pragma endregion ���� ��ġ ���� â ����
	
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}
#pragma endregion ��ŷ ������ ����

#pragma region SetDockingWindowIcons
void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hTriggerPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTriggerProperties.SetIcon(hTriggerPropertiesBarIcon, FALSE);
}
#pragma endregion ��ŷ ������ ������ ����

#pragma region AssertValid, Dump
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG
#pragma endregion CMainFrame ����


// CMainFrame �޽��� ó����

#pragma region OnViewCustomize
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}
#pragma endregion ����� ���� ����

#pragma region OnToolbarCreateNew
LRESULT CMainFrame::OnToolbarCreateNew( WPARAM wp,LPARAM lp )
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}
#pragma endregion ���� ����

#pragma region OnApplicationLook
void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}
#pragma endregion ��Ų ����

#pragma region OnUpdateApplicationLook
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
#pragma endregion ��Ų ����

#pragma region LoadFrame
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// ��� ����� ���� ������ ����� ���� ���߸� Ȱ��ȭ�մϴ�.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}
#pragma endregion ����� ���� ���� �θ���

#pragma region OnSettingChange
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}
#pragma endregion ���� ���� �� �߻�

#pragma region CreateDockingWindowAfter
void CMainFrame::CreateDockingWindowAfter()
{
	// ��ŷ â�� ����ϴ�.
	if (!CreateDockingWindows())
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
		return;
	}
		
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	CDockablePane* pTabbedBar = NULL;
	DockPane(&m_wndClassView);
	
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	m_wndTriggerProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTriggerProperties);
}
#pragma endregion ��ŷ â Ȱ��ȭ

#pragma region OnResetMain
void CMainFrame::OnResetMain( bool bEditLine, bool bEditWorld, bool bViewLine, bool bViewWorld, bool bEditTrigger )
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();

	UINT editLine	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT editWorld	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT viewLine	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT viewWorld	= MF_BYCOMMAND | MF_UNCHECKED;
	UINT editTrigger = MF_BYCOMMAND | MF_UNCHECKED;

	if( bEditLine )
		editLine	= MF_BYCOMMAND | MF_CHECKED;

	if( bEditWorld )
		editWorld	= MF_BYCOMMAND | MF_CHECKED;

	if( bViewLine )
		viewLine	= MF_BYCOMMAND | MF_CHECKED;

	if( bViewWorld )
		viewWorld	= MF_BYCOMMAND | MF_CHECKED;

	if ( bEditTrigger )
		editTrigger = MF_BYCOMMAND | MF_CHECKED;
	
	::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, editLine);
	::CheckMenuItem(hMenu, ID_EDIT_WORLD, editWorld);
	::CheckMenuItem(hMenu, ID_VIEW_LINE, viewLine);
	::CheckMenuItem(hMenu, ID_VIEW_WORLD, viewWorld);
	::CheckMenuItem( hMenu, ID_TRIGGER_BCK, editTrigger );

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);

	if( bEditLine == false )
		m_DlgLineBaseEditor.ShowWindow(SW_HIDE);
	else
		m_DlgLineBaseEditor.ShowWindow(SW_SHOW);

	if ( bEditTrigger == false )
		m_DlgTriggerEdit.ShowWindow( SW_HIDE );
	else
		m_DlgTriggerEdit.ShowWindow( SW_SHOW );

	if( bEditWorld == false )
	{
		m_DlgWorldEditor.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgWorldEditor.ShowWindow(SW_SHOW);
		m_DlgWorldEditor.ResetWorldEditor();
	}

	if ( m_DlgDungeonEditor )
		m_DlgDungeonEditor.ShowWindow(SW_SHOW);
}
#pragma endregion Main ���� �� ó���� �͵�

#pragma region OnEditLinemap
void CMainFrame::OnEditLinemap()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND);
	bool bEdit = false;
	if( iChecked & MF_CHECKED )
	{
		::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = false;
	}
	else
	{	
		::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND | MF_CHECKED);
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = true;
	}
		
	bool bVertexEdit = true;
	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;		
		pMain->m_bEditLine = bEdit;
		pMain->m_bEditWorld = false;
		pMain->SetFreeCamera();
		bVertexEdit = pMain->m_bEditLineVertex;
		
		CKTDGLineMap *pLinemap = g_pX2Game->GetWorld()->GetLineMap();
		for(int i=0; i<pLinemap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = pLinemap->AccessLineData(i);
			pLineData->m_bOver = false;
			pLineData->m_bSelected = false;
			pLineData->bEnable = true;
		}
	}

	if( bEdit == true )
	{
		m_DlgLineBaseEditor.ShowWindow(SW_SHOW);
		if( bVertexEdit == true)
			m_wndProperties.UpdateProp(1);
		else
			m_wndProperties.UpdateProp(2);

		m_DlgWorldEditor.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgLineBaseEditor.ShowWindow(SW_HIDE);
		m_wndProperties.UpdateProp(0);
	}
}
#pragma endregion ���θ� ������ Ȱ��ȭ

#pragma region OnUpdateEditLinemap
void CMainFrame::OnUpdateEditLinemap(CCmdUI *pCmdUI)
{
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND);
	int iCheck = 1;

	if( iChecked & MF_CHECKED )		
		iCheck = 1;
	else
		iCheck = 0;

	pCmdUI->SetCheck(iCheck);
	//if( ::IsWindow(pCmdUI->m_pOther->GetSafeHwnd()) )
	//	pCmdUI->m_pOther->SendMessage(TB_CHECKBUTTON, pCmdUI->m_nID, MAKELONG(iCheck,0));
}
#pragma endregion ���θ� ������ ���� ��

#pragma region OnEditWorld
void CMainFrame::OnEditWorld()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bEdit = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_CHECKED);
		::CheckMenuItem(hMenu, ID_EDIT_LINEMAP, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = true;
	}
	
	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bEditWorld = bEdit;
		pMain->m_bEditLine = false;
		pMain->SetFreeCamera();
	}

	if( bEdit == true )
	{
		m_DlgWorldEditor.ShowWindow(SW_SHOW);
		m_DlgWorldEditor.ResetWorldEditor();
		m_wndProperties.UpdateProp(3);

		m_DlgLineBaseEditor.ShowWindow(SW_HIDE);
		
	}
	else
	{
		m_DlgWorldEditor.ShowWindow(SW_HIDE);
		m_wndProperties.UpdateProp(0);
	}
}
#pragma endregion ���� ������ Ȱ��ȭ

#pragma region OnUpdateEditWorld
void CMainFrame::OnUpdateEditWorld(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND);
	
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
		iCheck = 1;
	else
		iCheck = 0;

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion ���� ������ ����

#pragma region OnViewLine
void CMainFrame::OnViewLine()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bViewLine = bView;
	}
}
#pragma endregion ���κ� Ȱ��ȭ

#pragma region OnUpdateViewLine
void CMainFrame::OnUpdateViewLine(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE, MF_BYCOMMAND);
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
	{	
		iCheck = 1;
	}
	else
	{
		iCheck = 0;
	}

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion ���κ� ����

#pragma region OnViewLineInfo
void CMainFrame::OnViewLineInfo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem( hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_UNCHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_UNCHECKED );
		bView = false;
	}
	else
	{
		::CheckMenuItem( hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_CHECKED );
		::CheckMenuItem( hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_CHECKED );
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineInfo = bView;
		pMain->m_bShowLineNumber = bView;
		pMain->m_bShowLineSetNumber = bView;
		pMain->m_bShowLineCameraNumber = bView;
		pMain->m_bShowLineTriggerNumber = bView;
		pMain->m_bShowInnerWaterInfo = bView;
	}
}
#pragma endregion ���κ� ���� Ȱ��ȭ

#pragma region OnUpdateViewLineInfo
void CMainFrame::OnUpdateViewLineInfo(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINEINFO, MF_BYCOMMAND);
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
	{	
		iCheck = 1;
	}
	else
	{
		iCheck = 0;
	}

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion ���κ� ���� ����

#pragma region OnViewWorld
void CMainFrame::OnViewWorld()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bViewWorld = bView;
	}
}
#pragma endregion ����� Ȱ��ȭ

#pragma region OnUpdateViewWorld
void CMainFrame::OnUpdateViewWorld(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_WORLD, MF_BYCOMMAND);
	int iCheck = 1;
	if( iChecked & MF_CHECKED )		
	{	
		iCheck = 1;
	}
	else
	{
		iCheck = 0;
	}

	pCmdUI->SetCheck(iCheck);
}
#pragma endregion ����� ����

#pragma region OnDropFiles
void CMainFrame::OnDropFiles( HDROP hDropInfo )
{
	WCHAR wBuf[MAX_PATH];
	UINT uDropCount = ::DragQueryFile( hDropInfo, 0xffffffff, wBuf, 1024 );

	for( UINT i=0; i<uDropCount; ++i )
	{
		::DragQueryFile( hDropInfo, i, wBuf, MAX_PATH );

		// drop �� ���� ó��
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{			
			wstring wFile = wBuf;
			MakeUpperCase(wBuf);
			if( StrStrW(wBuf, L"LINEMAP") != NULL )
			{
				// linemap script
				pMain->LoadLineMap( wBuf );
			}
			else if( StrStrW(wBuf, L".X") != NULL || StrStrW(wBuf, L".Y") != NULL )
			{
				// world object
			}
			else
			{
				// world script
				pMain->LoadWorld( wBuf );
			}
		}
	}

	::DragFinish( hDropInfo );
}
#pragma endregion ��ӵ� ������ ���� ��

#pragma region OnWorldObjectMove
void CMainFrame::OnWorldObjectMove()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
				
		pMain->SetRotationWorldObject(false);
		pMain->SetScaleWorldObject(false);
		pMain->SetMoveWorldObject(true);
	}
}
#pragma endregion ���� ��ü �̵� ��

#pragma region OnWorldobjectRotation
void CMainFrame::OnWorldobjectRotation()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{	
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

		pMain->SetMoveWorldObject(false);
		pMain->SetScaleWorldObject(false);
		pMain->SetRotationWorldObject(true);
	}
}
#pragma endregion ���� ������Ʈ ȸ�� ��

#pragma region OnWorldobjectScale
void CMainFrame::OnWorldobjectScale()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

		pMain->SetMoveWorldObject(false);
		pMain->SetRotationWorldObject(false);
		pMain->SetScaleWorldObject(true);
	}
}
#pragma endregion ���� ������Ʈ ũ�� ���� ��

#pragma region OnViewWorldobject
void CMainFrame::OnViewWorldObject()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bViewSelectedWorldobject = !pMain->m_bViewSelectedWorldobject;
		bool bView = pMain->m_bViewSelectedWorldobject;
		HMENU hMenu = GetMenuBar()->GetHMenu();
		
		if( bView == false )
		{		
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_CHECKED);
		}

		m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);

		m_wndProperties.SetPropValue( PI_EW_WORLD_SELECTED, (_variant_t)(bool)bView );
	}
}
#pragma endregion ���� ������Ʈ �� Ȱ��ȭ

#pragma region OnUpdateViewWorldobject
void CMainFrame::OnUpdateViewWorldobject(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		bool bCheck = pMain->m_bViewSelectedWorldobject;
		int iCheck = 0;
		if( bCheck )
			iCheck = 1;
		else
			iCheck = 0;

		pCmdUI->SetCheck(iCheck);
	}
}
#pragma endregion ���� ������Ʈ �� ����

#pragma region OnViewWorldObjectSelected
void CMainFrame::OnViewWorldObjectSelected()
{
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;		
		bool bView = pMain->m_bViewSelectedWorldobject;
		HMENU hMenu = GetMenuBar()->GetHMenu();

		if( bView == false )
		{		
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			::CheckMenuItem(hMenu, ID_VIEW_WORLDOBJECT, MF_BYCOMMAND | MF_CHECKED);
		}

		m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	}
}
#pragma endregion ���� ������Ʈ �� ���� ��

#pragma region OnEditDungeon
void CMainFrame::OnEditDungeon()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_DlgDungeonEditor.ShowWindow(SW_SHOW);
}
#pragma endregion ���� ������ Ȱ��ȭ

#pragma region OnViewLineNumber
void CMainFrame::OnViewLineNumber()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineNumber = bView;
	}
}
#pragma endregion ���� ��ȣ ���

#pragma region OnViewLineSetNumber
void CMainFrame::OnViewLineSetNumber()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_SET_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineSetNumber = bView;
	}
}
#pragma endregion ���μ� ��ȣ ���

#pragma region OnViewLineCameraNumber
void CMainFrame::OnViewLineCameraNumber()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_CAMEAR_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_CAMERA_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineCameraNumber = bView;
	}
}
#pragma endregion ���� ī�޶� ��ȣ ���

#pragma region OnViewLineTriggerNumber
void CMainFrame::OnViewLineTriggerNumber()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_LINE_TRIGGER_NUMBER, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowLineTriggerNumber = bView;
	}
}
#pragma endregion ���� Ʈ���� ��ȣ ���

#pragma region OnViewWaterInfo
void CMainFrame::OnViewWaterInfo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	bool bView = false;
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND);
	if( iChecked & MF_CHECKED )
	{		
		::CheckMenuItem(hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_UNCHECKED);
		bView = false;
	}
	else
	{
		::CheckMenuItem(hMenu, ID_VIEW_WATER_INFO, MF_BYCOMMAND | MF_CHECKED);
		bView = true;
	}

	m_wndToolBar.OnUpdateCmdUI((CFrameWnd*)this, TRUE);
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		pMain->m_bShowInnerWaterInfo = bView;
	}
}
#pragma endregion ���� ����

void CMainFrame::OnTriggerBck()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	HMENU hMenu = GetMenuBar()->GetHMenu();
	UINT iChecked = ::GetMenuState(hMenu, ID_TRIGGER_BCK, MF_BYCOMMAND);
	bool bEdit = false;
	if( iChecked & MF_CHECKED )
	{
		::CheckMenuItem(hMenu, ID_TRIGGER_BCK, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = false;
	}
	else
	{	
		::CheckMenuItem(hMenu, ID_TRIGGER_BCK, MF_BYCOMMAND | MF_CHECKED);
		::CheckMenuItem(hMenu, ID_EDIT_WORLD, MF_BYCOMMAND | MF_UNCHECKED);
		bEdit = true;
	}
	
	if( bEdit == true )
	{
		m_DlgTriggerEdit.ShowWindow(SW_SHOW);
		m_DlgWorldEditor.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgTriggerEdit.ShowWindow(SW_HIDE);
		m_wndProperties.UpdateProp(0);
	}
}
