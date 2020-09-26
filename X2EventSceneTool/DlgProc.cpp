#include "dxstdafx.h"
#include ".\dlgproc.h"
#include ".\resource.h"

namespace _DLG_PROC_
{
	// EventScene List �� Search ��� ������ Search Edit �� ����Ű ��� ������ ����
	WNDPROC OldSearchEditProc;

	//{{Begin} message handler for event lst
	BOOL CALLBACK EventSceneListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
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
				col.pszText		= L"EVENT ID";
				col.iSubItem	= 0;
				::SendMessage(::GetDlgItem(hDlg,IDC_LIST1), LVM_INSERTCOLUMN,0,(LPARAM)&col);

				// ����Ʈ���� ��� ���ڸ� ǥ�� | �� �� ��ü ����
				ListView_SetExtendedListViewStyle(::GetDlgItem(hDlg,IDC_LIST1), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
				//# ����Ʈ �� �ʱ�ȭ
				//////////////////////////////////////////////////////////////////////////

				//# Search edit binding for subproc
				OldSearchEditProc = (WNDPROC)(::SetWindowLongPtr(::GetDlgItem(hDlg,IDC_EDIT_SEARCH_ID), GWLP_WNDPROC, (LONG)EventSceneListSearchEditSubProc));
				return TRUE;
			}
		case WM_CLOSE:
		case WM_DESTROY:
			{
				::SetWindowLongPtr(::GetDlgItem(hDlg,IDC_EDIT_SEARCH_ID), GWLP_WNDPROC, (LONG)(OldSearchEditProc));

				DestroyWindow(hDlg);
				hDlg = NULL;
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
							int iSelectItem = ListView_GetNextItem(::GetDlgItem(hDlg,IDC_LIST1), -1, LVNI_ALL|LVNI_SELECTED);
							if(iSelectItem != -1)
							{
								// ����Ʈ���� ���õ� �׸� �ؽ�Ʈ�� ������
								WCHAR wszSelectItemText[256] = L"";
								ListView_GetItemText(::GetDlgItem(hDlg,IDC_EVENTID_LIST), iSelectItem, 0, wszSelectItemText, 255);

								// UI ����Ʈ â�� ��ũ��Ʈ�� �ѷ��ش�.
								std::wstring wstrUTF8Script;
								if( GetRenderFrm()->GetEventScriptParsing()->GetUTF8byEventScriptData(wszSelectItemText, wstrUTF8Script) == true )
								{
									SetWindowText(::GetDlgItem(GetRenderFrm()->GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCRIPT_EDIT), IDC_EDIT1), wstrUTF8Script.c_str());
								}
							}
						}return TRUE;

					case NM_RDBLCLK:
						{
							int iSelectItem = ListView_GetNextItem(::GetDlgItem(hDlg,IDC_LIST1), -1, LVNI_ALL|LVNI_SELECTED);
							if(iSelectItem != -1)
							{
								// ����Ʈ���� ���õ� �׸� �ؽ�Ʈ�� ������
								WCHAR wszSelectItemText[256] = L"";
								ListView_GetItemText(::GetDlgItem(hDlg,IDC_EVENTID_LIST), iSelectItem, 0, wszSelectItemText, 255);

								// �÷���~~
								if( true == GetRenderFrm()->GetX2EventScene()->PlayEventScene(wszSelectItemText) )
									GetRenderFrm()->SetLastPlaySceneID(wszSelectItemText);
							}
						}return TRUE;
					}
				}
				return TRUE;
			}

		case WM_COMMAND:
			{
				int wmID, wmEvent;
				wmID = LOWORD(wParam);
				wmEvent = HIWORD(wParam);

				// parse the menu selections
				switch(wmID)
				{
				case IDC_BUT_OPEN:
					{
						std::wstring wstrFileName;
						if( true == _EST_OBJECT_::GetDlgFileName(g_pKTDXApp->GetHWND(), wstrFileName, true) )
						{
							GetRenderFrm()->GetEventScriptParsing()->Clear();
							GetRenderFrm()->GetX2EventScene()->Clear();

							GetRenderFrm()->GetEventScriptParsing()->OpenScriptFile(wstrFileName.c_str());

							if( GetRenderFrm()->GetX2EventScene()->OpenScriptFile(L"EventSceneObject.lua") == false )
							{
								std::wstring wstrMSG;
								wstrMSG = L"��� �Ľ� ����.!";
								wstrMSG += L"EventSceneObject.lua";
								ERRORMSG(wstrMSG.c_str());
							}
							WCHAR wszPath[MAX_PATH] = L"";
							WCHAR wszFileName[MAX_PATH] = L"";
							// Ǯ�н��� ��ο� �����̸����� �и�
							if( true == _EST_OBJECT_::DecomposePathAndFileName(wszPath, wszFileName, wstrFileName.c_str()) )
							{
								if( GetRenderFrm()->GetX2EventScene()->OpenScriptFile(wszFileName) == false )
								{
									std::wstring wstrMSG;
									wstrMSG = L"��� �Ľ� ����.!";
									wstrMSG += wszFileName;
									ERRORMSG(wstrMSG.c_str());
								}
							}

							_EST_OBJECT_::EventIDList eventIDList;
							GetRenderFrm()->GetEventScriptParsing()->GetEventIDList(eventIDList);

							// ����Ʈ�信 �̺�Ʈ ���̵� �߰�
							for( int i = 0; i < static_cast<int>(eventIDList.size()); ++i )
							{
								WCHAR wstrTemp[256] = L"";
								::MultiByteToWideChar( CP_ACP, 0, eventIDList[i].c_str(), -1, wstrTemp, 255 );

								LVFINDINFO plvfi;
								plvfi.flags	= LVFI_STRING;
								plvfi.psz	= wstrTemp;

								// ���̵� �߰��ϸ鼭 ���� �̸��� ���̵� �ִ��� üũ
								if(ListView_FindItem(::GetDlgItem(hDlg,IDC_EVENTID_LIST), -1, &plvfi) == -1)
								{
									// add file name by listbox
									LVITEM li;
									li.mask = LVIF_TEXT|LVIF_IMAGE;

									li.iSubItem	= 0;
									li.iItem	= 0;
									li.pszText	= wstrTemp;
									::SendMessage(::GetDlgItem(hDlg,IDC_EVENTID_LIST),LVM_INSERTITEM,0,(LPARAM)&li);
								}
							}
						}
					}break;

				case IDC_BUT_SAVE:
					{
						GetRenderFrm()->GetEventScriptParsing()->SaveScript();
					}break;

				case IDC_BUT_ADD:
					{
					}break;
				}

				return TRUE;
			}
		}

		return FALSE;
	}
	//{{End} message handler for event list

	//{{Begin} message handler for event editer
	BOOL CALLBACK EventSceneEditerDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_CREATE:
		case WM_INITDIALOG:
			{
			}
			return TRUE;
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
				return TRUE;
			}

		case WM_COMMAND:
			{
				int wmID, wmEvent;
				wmID = LOWORD(wParam);
				wmEvent = HIWORD(wParam);

				// parse the menu selections
				switch(wmID)
				{
				case IDC_BUT_REFRESH:
					{
						int iScriptSize = 0;
						WCHAR *pwszBuffer = new WCHAR[MAX_SCRIPT_BUFFER];

						// edit â�� �ִ� �����͸� �о��..
						iScriptSize = ::GetWindowText(::GetDlgItem(hDlg, IDC_EDIT1),pwszBuffer,MAX_SCRIPT_BUFFER);

						if( iScriptSize > 0 )
						{
							// ���� �Ľ��� �ϵ��� �Ѵ�.
							bool bResult = false;
							bResult = GetRenderFrm()->GetEventScriptParsing()->ScriptRefresh(pwszBuffer, iScriptSize + 1); // +1 �� ���ִ� ������ ���� ���� ����Ʈ���� �ҵ��� �ϱ�����
						}

						SAFE_DELETE_ARRAY(pwszBuffer); 
					}break;
				}

				return TRUE;
			}
		}
		return FALSE;
	}
	//{{End} message handler for event editer

	//{{Begin} message handler for search edit by event list
	LRESULT	CALLBACK EventSceneListSearchEditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		if( iMessage == WM_KEYDOWN && wParam == VK_RETURN )
		{
			HWND hWndList = ::GetDlgItem(GetRenderFrm()->GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), IDC_EVENTID_LIST);
			HWND hWndStatic = ::GetDlgItem(GetRenderFrm()->GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), IDC_STATIC_SEARCH_MSG);

			if(NULL != hWndList)
			{
				WCHAR wszID[256] = L"";
				::GetWindowText(hWnd, wszID, 256);

				LVFINDINFO plvfi;
				plvfi.flags	= LVFI_STRING;
				plvfi.psz	= wszID;

				//int iSelIndex = ListView_FindItem(hWndList, -1, &plvfi);

				//# �κм��� �˻��� ���� ������ �׸��� �̾� ���� �˻�
				int iSelIndex = -1;
				int iMaxListCount = ListView_GetItemCount(hWndList);
				WCHAR wszTemp[256] = L"";
				for( int i = 0; i < iMaxListCount; ++i )
				{
					ListView_GetItemText(hWndList, i, 0, wszTemp, 256);
					if( 0 == wcsncmp(wszID, wszTemp, wcslen(wszID)) )
					{
						iSelIndex = i;
						break;
					}
				}

				if( -1 != iSelIndex )
				{
					::SetFocus(hWndList);

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

					std::wstring wstrMsg = L"ã�⼺��";
					wstrMsg += L"(";
					wstrMsg += wszID;
					wstrMsg += L")";
					::SetWindowText(hWndStatic, wstrMsg.c_str());
				}
				else
				{
					std::wstring wstrMsg = L"ã�����";
					wstrMsg += L"(";
					wstrMsg += wszID;
					wstrMsg += L")";
					::SetWindowText(hWndStatic, wstrMsg.c_str());
				}
			}
		}

		return CallWindowProc( OldSearchEditProc, hWnd, iMessage, wParam, lParam );
	}
	//{{End} message handler for search edit by event list
};

