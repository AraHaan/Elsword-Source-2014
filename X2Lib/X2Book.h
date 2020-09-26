#pragma once

class CX2Book
{
public:

	// �����ϸ� �ٷ� å�� �߰�, Object ��ü�� Delete �ϸ� ����ϰ� �������.
	// EXIT�� �ݾ��� �� ��� ó���ؾ� �ұ�..
	// Ȯ���ϱ� ����.. -> ���� å�ý��� ���� ��κ��� �ڵ尡 ���� �����ϵ��� �غ���
	// å data�� �ִ��� ��ũ��Ʈ�� �̾Ƶ��� : ���� ���� �߰��Ҷ� �ڵ�� �ִ��� ���� ���� �� �� �ֵ���.

	enum BOOK_UI_CUSTOM_MESSAGE
	{
		BUCM_EXIT			= 28000,
		BUCM_PREV_PAGE,
		BUCM_NEXT_PAGE,
		BUCM_GOTO_PAGE,       
	};

	struct PageData
	{
		wstring m_PageLuaFileName;
		//wstring m_string;
		//wstring m_CoverLuaFileName;
	};
	typedef map<int, PageData>::iterator PageDataIterator;

	CX2Book( const char* bookTableName );
	CX2Book();
	virtual ~CX2Book(void);
	
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow(bool bOpen);

	bool SetBookTable( const char* bookTableName );

	void NextPage();
	void PrevPage();
	void GotoPage(int iTargetPage);

private:

	bool			m_bShow;
	CKTDGUIDialogType	m_pDLGBook;
	CKTDGUIDialogType	m_pDLGBookCoverDefault;

	int				m_iCurrentPage;
	map<int, PageData> m_mapPageData;

};