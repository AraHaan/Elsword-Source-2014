#include "dxstdafx.h"
#include ".\x2scenariodlgproc.h"
#include "../resource.h"

namespace _DLG_PROC_
{
	//## �׼� ��Ʈ�� ���� �Լ� ����..
	//# 1. �ؽ��� ��� ���� ���� �Լ���..
	void AddAction01ComboboxItem( WCHAR* wszItem );
	//# 2. �ؽ��� ȿ�� 

	//{{ ���ҽ� ���� ���ν���.
	BOOL CALLBACK ResourceDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_INITDIALOG:
		case WM_CREATE:
			{
				//////////////////////////////////////////////////////////////////////////
				//# ����Ʈ �� �ʱ�ȭ
				LVCOLUMN col;

				// ����߰�.
				col.mask		= LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
				col.fmt			= LVCFMT_LEFT;
				col.cx			= 250;
				col.pszText		= L"���ϸ�";
				col.iSubItem	= 0;
				::SendMessage(::GetDlgItem(hDlg,IDC_LIST1), LVM_INSERTCOLUMN,0,(LPARAM)&col);

				//col.mask		= LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
				//col.fmt			= LVCFMT_LEFT;
				////col.cx			= 50;
				//col.pszText		= L"-";
				//col.iSubItem	= 1;
				//::SendMessage(::GetDlgItem(hDlg,IDC_LIST1), LVM_INSERTCOLUMN,1,(LPARAM)&col);

				// ����Ʈ���� ��� ���ڸ� ǥ�� | �� �� ��ü ����
				ListView_SetExtendedListViewStyle(::GetDlgItem(hDlg,IDC_LIST1), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
				//# ����Ʈ �� �ʱ�ȭ
				//////////////////////////////////////////////////////////////////////////
				return TRUE;
			}
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
				return TRUE;
			}

		case WM_DROPFILES:
			{
				WCHAR FullFileName[MAX_PATH] = L"";
				WCHAR FileName[256] = L"";
				WCHAR FullDir[MAX_PATH] = L"";
				WCHAR drive[_MAX_DRIVE] = L"";
				WCHAR dir[_MAX_DIR] = L"";
				WCHAR fname[_MAX_FNAME] = L"";
				WCHAR ext[_MAX_EXT] = L"";

				DragQueryFile((HDROP) wParam, 0, FullFileName, MAX_PATH);
				_wsplitpath( FullFileName, drive, dir, fname, ext);

				wcscat( FileName, fname);
				wcscat( FileName, ext);

				wcscat( FullDir, drive );
				wcscat( FullDir, dir );

				//����Ʈ�信 �����̸��� ������ �߰�.
				LVFINDINFO plvfi;
				plvfi.flags	= LVFI_STRING;
				plvfi.psz	= FileName;
				if(ListView_FindItem(::GetDlgItem(hDlg,IDC_LIST1), -1, &plvfi) == -1)
				{
					// add file name by listbox
					LVITEM li;
					li.mask = LVIF_TEXT|LVIF_IMAGE;

					li.iSubItem	= 0;
					li.iItem	= 0;
					li.pszText	= FileName;
					::SendMessage(::GetDlgItem(hDlg,IDC_LIST1),LVM_INSERTITEM,0,(LPARAM)&li);

					// add file name by action01 of combo box
					AddAction01ComboboxItem(FileName);

					// add dir by engine
					std::string strDataDir;
					ConvertWCHARToChar( strDataDir, FullDir );
					g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDataDir );
				}
				return TRUE;
			}

		//����Ʈ��� WM_NOTIFY �� �޼����� ���ƿ´�.
		case WM_NOTIFY:
			{				
				if(((LPNMHDR)lParam)->hwndFrom == ::GetDlgItem(hDlg,IDC_LIST1))
				{
					switch(((LPNMHDR)lParam)->code)
					{
					case NM_DBLCLK:
						{
							//������ ����ó��.
							int iSelectItem = ListView_GetNextItem(::GetDlgItem(hDlg,IDC_LIST1), -1, LVNI_ALL|LVNI_SELECTED);
							if(iSelectItem != -1)
							{
								ListView_DeleteItem(::GetDlgItem(hDlg,IDC_LIST1), iSelectItem);
							}
						}
						return TRUE;
					}
				}
				return TRUE;
			}
		}

		return FALSE;
	}
	//}} ���ҽ� ���� ���ν���.

	//{{ Action list
	BOOL CALLBACK ActionListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_CREATE:
		case WM_INITDIALOG:
			{
				//////////////////////////////////////////////////////////////////////////
				//# ����Ʈ �� �ʱ�ȭ
				LVCOLUMN col;

				// ����߰�.
				col.mask		= LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
				col.fmt			= LVCFMT_LEFT;
				col.cx			= 350;
				col.pszText		= L"�׼� ����Ʈ.";
				col.iSubItem	= 0;
				::SendMessage(::GetDlgItem(hDlg,IDC_LIST_ACTION), LVM_INSERTCOLUMN,0,(LPARAM)&col);

				// ����Ʈ���� ��� ���ڸ� ǥ�� | �� �� ��ü ���� | üũ�ڽ� �ޱ�
				ListView_SetExtendedListViewStyle(::GetDlgItem(hDlg,IDC_LIST_ACTION), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP);
				//# ����Ʈ �� �ʱ�ȭ
				//////////////////////////////////////////////////////////////////////////

				LVITEM li;
				li.mask = LVIF_TEXT|LVIF_IMAGE;

				li.iSubItem	= 0;
				li.iItem	= 0;
				li.pszText	= L"Test";
				for(int i = 0; i < 10; ++i)
				{
					::SendMessage(::GetDlgItem(hDlg,IDC_LIST_ACTION),LVM_INSERTITEM,0,(LPARAM)&li);
				}
			}
			return TRUE;
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
				return TRUE;
			}
		}
		return FALSE;
	}
	//}} Action list
	
	//{{ �׼ǰ��� ���ν���.
	//# �׼ǿ� ���Ǵ� UI ���ν��� ����..

	//## �׼� ��Ʈ�� ���� �Լ� ����..
	//# 1. �ؽ��� ��� ���� ���� �Լ���..
	void AddAction01ComboboxItem( WCHAR* wszItem )
	{
		SendMessage(GetScenarioTool()->GetDlgItem(DH_TEX_IO, IDC_COMBO_SEL_IMAGE), CB_ADDSTRING, 0, (LPARAM)wszItem);
	}

	BOOL CALLBACK ActionTexIOProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK ActionTexture02Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	// �׼��� ��ɵ��� ������ �����ϴ� �θ� ���̾�α�..
	// : �޴� �� �� �׼� ���̾�α� �߰��� �̰�����..
	BOOL CALLBACK ActionDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_INITDIALOG:
		case WM_CREATE:
			{
				//## �� ��Ʈ�� �߰�.
				TCITEM ti;
				ti.mask = TCIF_TEXT;
				//#�̹��� ���.
				ti.pszText = L"�̹��� ��/���";
				TabCtrl_InsertItem(::GetDlgItem(hDlg, IDC_TAB1), DH_TEX_IO, &ti);
				//#�̹��� ȿ��.
				ti.pszText = L"�̹��� ȿ��";
				TabCtrl_InsertItem(::GetDlgItem(hDlg, IDC_TAB1), DH_TEX_02, &ti);

				//## �׼� ������ ����.
				GetScenarioTool()->SetDlgHwnd(DH_TEX_IO, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_ACTION_TEX_IO), hDlg, ActionTexIOProc));
				::ShowWindow(GetScenarioTool()->GetDlgHwnd(DH_TEX_IO), SW_SHOW);

				GetScenarioTool()->SetDlgHwnd(DH_TEX_02, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_ACTION_TEXTURE_02), hDlg, ActionTexture02Proc));
			}
			return TRUE;
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
			}
			return TRUE;

		case WM_NOTIFY:
			{
				switch(((LPNMHDR)lParam)->code)
				{
				case TCN_SELCHANGE:
					{
						GetScenarioTool()->HideActionSubDlg();

						int iSelTabCtrl = TabCtrl_GetCurSel(::GetDlgItem(hDlg, IDC_TAB1));
						if(iSelTabCtrl >= 0 && iSelTabCtrl < DH_END)
							GetScenarioTool()->ShowActionSubDlg(iSelTabCtrl);
					}
				}
				return TRUE;
			}
		}

		return FALSE;
	}
	// �ؽ��� ��/���..
	BOOL CALLBACK ActionTexIOProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_CREATE:
		case WM_INITDIALOG:
			{
				////# test
				//WCHAR wszTest[MAX_PATH] = L"TEST";
				//for(int i = 0; i < 10; ++i)
				//	SendMessage(::GetDlgItem(hDlg, IDC_COMBO_SEL_IMAGE), CB_ADDSTRING, 0, (LPARAM)wszTest);
			}
			return TRUE;
		}
		return FALSE;
	}
	// �ؽ��� �ִϸ��̼�..
	BOOL CALLBACK ActionTexture02Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}
	//}} �׼ǰ��� ���ν���.
};

