#pragma once

//////////////////////////////////////////////////////////////////////////
//## UI msg proc
//## Desc : UI �� ��Ʈ�� �� �޼��� ���ν���.
//##		- ���ͷ��� ����, �ʱ�ȭ, ��,���
//////////////////////////////////////////////////////////////////////////

namespace _DLG_PROC_
{
	BOOL CALLBACK ResourceDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK ActionListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK ActionDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
