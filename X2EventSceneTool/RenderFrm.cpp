#include "dxstdafx.h"
#include ".\renderfrm.h"
#include ".\resource.h"

CRenderFrm::CRenderFrm(void)
{
	//{{Begin} �ɼ� ���̾�α� ����
	//�̺�Ʈ ����Ʈ DLG
	SetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST,
				CreateDialog(g_pKTDXApp->GetHInstance(),
				MAKEINTRESOURCE(IDD_EVENTSCENE_LIST),
				g_pKTDXApp->GetHWND(),
				&_DLG_PROC_::EventSceneListDlgProc));
	::ShowWindow(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), SW_SHOW);
	//�̺�Ʈ ������ DLG
	SetDlgHwnd(_EST_OBJECT_::DH_EVENTSCRIPT_EDIT,
				CreateDialog(g_pKTDXApp->GetHInstance(),
				MAKEINTRESOURCE(IDD_EVENTSCENE_EDIT),
				g_pKTDXApp->GetHWND(),
				&_DLG_PROC_::EventSceneEditerDlgProc));
	::ShowWindow(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCRIPT_EDIT), SW_SHOW);

	//{{End} �ɼ� ���̾�α� ����

//  	//{{begin} CX2EventScene ���� �ʿ��� �Լ� ���ε� �� ��� ������ ó��
//  	if( g_pKTDXApp->GetDeviceManager()->LoadLuaTinker(L"Enum.lua") == false )
//  	{
//  		ERRORMSG(L"Enum.lua parsing fail.!");
//  	}
//  	//{{end} CX2EventScene ���� �ʿ��� �Լ� ���ε� �� ��� ������ ó��
}

CRenderFrm::~CRenderFrm(void)
{
	for( int i = 0; i < _EST_OBJECT_::DH_END; ++i )
		::DestroyWindow(m_hWnd[i]);

	g_pRenderFrm = NULL;
}


HRESULT CRenderFrm::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->OnFrameMove( fTime, fElapsedTime );

	m_EventScene.OnFrameMove(fTime, fElapsedTime);

	return S_OK;
}

HRESULT CRenderFrm::OnFrameRender()
{
	g_pKTDXApp->GetDGManager()->ObjectChainSort();

	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	g_pKTDXApp->GetDGManager()->FrontUIRender();

	return S_OK;
}

HRESULT CRenderFrm::OnResetDevice()
{
	return S_OK;
}

HRESULT CRenderFrm::OnLostDevice()
{

	return S_OK;
}

bool CRenderFrm::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	m_EventScene.MsgProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_CREATE:
		::InitCommonControls();
		return true;

	case WM_LBUTTONDBLCLK:
		GetRenderFrm()->SetFocusPlaySceneID();
		return true;
	}

	return false;
}

void CRenderFrm::SetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType, HWND hWnd )
{
	if(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END)
		m_hWnd[dlgType] = hWnd;
	else
		assert(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END);
}

HWND CRenderFrm::GetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType )
{
	if(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END)
		return m_hWnd[dlgType];
	else
		assert(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END);

	return NULL;
}

HWND CRenderFrm::GetDlgItem( _EST_OBJECT_::DLG_HWND dlgType, int iDlgItem )
{
	if(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END)
		return ::GetDlgItem(m_hWnd[dlgType], iDlgItem);
	else
		assert(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END);

	return NULL;
}

void CRenderFrm::SetFocusPlaySceneID()
{
	if( false == GetLastPlaySceneID().empty() )
	{
		HWND hWndList = ::GetDlgItem(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), IDC_EVENTID_LIST);
		if(NULL == hWndList)
			return;

		WCHAR wszID[256] = L"";
		wcscpy(wszID, GetLastPlaySceneID().c_str());

		// ��Ŀ���� EventList â���� �ȱ�
		::SetFocus(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST));

		LVFINDINFO plvfi;
		plvfi.flags	= LVFI_STRING;
		plvfi.psz	= wszID;

		int iSelIndex = ListView_FindItem(hWndList, -1, &plvfi);

		if( -1 != iSelIndex )
		{
			// ���û��·� �ʱ�ȭ
			ListView_SetItemState(hWndList, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);

			// ���õ� �׸����� ��Ŀ�� �̵�
			ListView_SetItemState(hWndList,         // handle to listview
				iSelIndex,
				LVIS_FOCUSED | LVIS_SELECTED,         // index to listview item
				LVIS_FOCUSED | LVIS_SELECTED // item state
				);

			//���ý�Ų �׸��� ȭ�鿡 �������� ���� �ʴٸ� ���̵��� ��ũ��.
			ListView_EnsureVisible(hWndList, iSelIndex, FALSE);
		}
	}
}