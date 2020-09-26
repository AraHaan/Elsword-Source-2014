#pragma once

//////////////////////////////////////////////////////////////////////////
//## common object by event scene tool
//## Des : �̺�Ʈ�� ���� Ŭ���� & ����ü & Ű ����.
//////////////////////////////////////////////////////////////////////////

namespace _EST_OBJECT_
{
	enum DLG_HWND
	{
		DH_EVENTSCENE_LIST,
		DH_EVENTSCRIPT_EDIT,

		DH_END,
	};

	// dlg �� �̿��� file open & file save 
	inline bool GetDlgFileName( IN HWND hWndOwner, OUT std::wstring & wstrFileName, bool bIsOpen )
	{
		OPENFILENAME ofn;
		WCHAR wszFileName[512];  // path���� ������ ���� �̸�

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize		= sizeof(ofn);
		ofn.hwndOwner		= hWndOwner; 
		ofn.lpstrFile		= (LPWSTR)wszFileName;
		ofn.lpstrFile[0]	= '\0';
		ofn.nMaxFile		= sizeof(wszFileName);
		ofn.lpstrFilter		= L"lua script\0*.lua\0";
		ofn.nFilterIndex	= 1;
		ofn.lpstrFileTitle	= NULL;
		ofn.nMaxFileTitle	= 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if( bIsOpen == true )
		{
			if( TRUE == GetOpenFileName( &ofn ) )
			{
				wstrFileName = ofn.lpstrFile;
				return true;
			}
		}
		else
		{
			if( TRUE == GetSaveFileName( &ofn ) )
			{
				wstrFileName = ofn.lpstrFile;
				return true;
			}
		}

		return false;
	}

	// fullpath �� �޾� path, filename �� �и� 
	inline bool DecomposePathAndFileName( OUT WCHAR* wszPath, OUT WCHAR* wszFileName, IN const WCHAR* wszFullPath )
	{
		if( NULL == wszFullPath )
			return false;

		WCHAR drive[_MAX_DRIVE] = L"";
		WCHAR dir[_MAX_DIR] = L"";
		WCHAR fname[_MAX_FNAME] = L"";
		WCHAR ext[_MAX_EXT] = L"";

		_wsplitpath( wszFullPath, drive, dir, fname, ext);
		if( NULL != wszFileName )
		{
			wcscat( wszFileName, fname);
			wcscat( wszFileName, ext);	
		}
		if( NULL != wszPath )
		{
			wcscat( wszPath, drive );
			wcscat( wszPath, dir );
		}

		return true;
	}

	inline void LV_SetfocusAndFindItem(IN HWND hWnd, IN const WCHAR *wszFindItem)
	{
		//if( hWnd == NULL || wszFindItem == NULL )
		//	return;

		//// ��Ŀ���� EventList â���� �ȱ�
		//::SetFocus(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST));

		//LVFINDINFO plvfi;
		//plvfi.flags	= LVFI_STRING;
		//plvfi.psz	= wszFindItem;

		//int iSelIndex = ListView_FindItem(hWndList, -1, &plvfi);

		//if( -1 != iSelIndex )
		//{
		//	// ���û��·� �ʱ�ȭ
		//	ListView_SetItemState(hWndList, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);

		//	// ���õ� �׸����� ��Ŀ�� �̵�
		//	ListView_SetItemState(hWndList,         // handle to listview
		//		iSelIndex,
		//		LVIS_FOCUSED | LVIS_SELECTED,         // index to listview item
		//		LVIS_FOCUSED | LVIS_SELECTED // item state
		//		);

		//	//���ý�Ų �׸��� ȭ�鿡 �������� ���� �ʴٸ� ���̵��� ��ũ��.
		//	ListView_EnsureVisible(hWndList, iSelIndex, FALSE);
		//}
	}

	// key 1 : EVENT_SCENE_ID
	// key 2 : script of g_pEventScene:AddEventScene_LUA
	typedef std::map< std::string, std::string > EventSceneData;

	// key : EVENT_SCENE_ID
	typedef std::vector< std::string > EventIDList;
}












