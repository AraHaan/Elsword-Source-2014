#pragma once

//////////////////////////////////////////////////////////////////////////
//## UI msg proc
//## Desc : UI �� ��Ʈ�� �� �޼��� ���ν���.
//##		- ���ͷ��� ����, �ʱ�ȭ, ��,���
//////////////////////////////////////////////////////////////////////////

namespace _DLG_PROC_
{
	BOOL CALLBACK EventSceneListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK EventSceneEditerDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	LRESULT	CALLBACK EventSceneListSearchEditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
