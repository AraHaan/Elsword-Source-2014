#ifndef SERV_EPIC_QUEST
#pragma once

//class CX2StateMenu;
class CX2UIQuest : public CX2ItemSlotManager
{
	enum QUEST_UI_CUSTOM_MESSAGE
	{
		QUCM_EXIT					= 25000,
		QUCM_SELECT_QUEST,
		QUCM_CATEGORY_BUTTON,
		QUCM_LIST_UP,
		QUCM_LIST_DOWN,
		QUCM_COMMENT_UP,
		QUCM_COMMENT_DOWN,
		QUCM_QUEST_GIVEUP,
		QUCM_QUEST_GIVEUP_CONFIRM,
		QUCM_QUEST_GIVEUP_CANCLE,

		QUCM_LIST_NAVI_LCLICKED,
		QUCM_LIST_NAVI_DRAGGING,
		
		QUCM_COMMENT_NAVI_LCLICKED,
		QUCM_COMMENT_NAVI_DRAGGING,

		QUCM_QUICK_QUEST_CHECKBOX,
		QUCM_QUICK_QUEST_SIZE_BUTTON,

	};
	
	// ���ĵ� ����Ʈ + ī�װ� ����Ʈ�� ���� struct. ���Ϳ� ���ϴ�.
	struct MyQuestList
	{
		bool				m_bIsCategory;
		bool				m_bIsOpen;
		int					m_QuestID;
		int					m_DungeonID;

		MyQuestList()
		{
			m_bIsCategory	= false;
			m_bIsOpen		= false;
			m_QuestID		= -1;
			m_DungeonID		= 0;
		}
	};

	// ��ư + ����ƽ ������ ���� struct. ���Ϳ� ���ϴ�.
	struct MyQuestListSlot
	{		
		CKTDGUIButton*		m_pCategoryButton;
		CKTDGUIStatic*		m_pCategoryStatic;
		CKTDGUIRadioButton*	m_pListButton;
		CKTDGUIStatic*		m_pListStatic;
		int					m_iQuestIndex;
		MyQuestListSlot()
		{
			m_pListButton = NULL;
			m_pListStatic = NULL;
			m_pCategoryButton = NULL;
			m_pCategoryStatic = NULL;
			m_iQuestIndex = 0;
		}
	};
public:

	CX2UIQuest( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuest(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );
	

	void ResetQuestUI( bool bResetTopIndex = true );
	void ClearQuestSlotList();
	void UpdateQuestList();
	void UpdateQuestUI( bool bUpdateNavi = true );
	void UpdateQuestListNavi();
	
	void UpdateQuestDesc( const wstring& wstrDesc );
	void UpdateQuestDescPage( bool bUpdateNavi = true );
	void UpdateQuestCommentNavi();
	wstring CreateQuestDesc();	
	
	void SelectQuest( int SelectedQuestID );
	void SetRewardItemSlot();
	void SelectCategory( int SelectedCategoryDID, bool forOpen );

	void SetNextTopIndex(bool isUp);

	// ������ �ޱ�~
// 	wstring GetVillageIconFileName( int LocalMapID );
// 	wstring GetVillageIconKey( int LocalMapID );

	// ���콺 �� ����
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool MouseOnQuestListArea( D3DXVECTOR2 mousePos );
	bool MouseOnQuestCommentArea( D3DXVECTOR2 mousePos );



	// ������Ʈ ����
	void SetShowQuickQuestDLG( bool forOpen );
	bool GetShowQuickQuestDLG();
	void UpdateQuickQuestDLG();
	void OpenUpQuickQuesstDLG( bool forOpen );
	bool GetQuickQuestDLGOpenedUP(){ return m_bQuickQuestOpenedUP; }

	// ��������Ʈ ID���� �ش� ����Ʈ�� ����Ǵ� ���� ID�� ���Ѵ� ( ������ DI_END return )
	int GetSubQuestDungeonID( int iSubQuestID );

	bool GetOverMouseQuest();

	// ��������� ���� ����ó��
	virtual wstring GetSlotItemDesc();

private:
	int						m_iQuestPerPage;
	int						m_iRewardSlotNum;

	int						m_iQuestListSize;
	int						m_iQuestListUnderTop;
	
	bool					m_bShow;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUIQuest;   
	CKTDGUIDialogType			m_pDLGMsgBox;

	vector<MyQuestListSlot>			m_vQuestListSlot;
	vector<MyQuestList>				m_vQuestList;
	int						m_iTopIndex;
	int						m_iPickedQuestID;
	int						m_iQuestNum;

	// ���콺 �� ����
	SHORT				m_SumDelta;

	// ����Ʈ ���� ����
	D3DXVECTOR2			m_vQuestDescSize;				// ����Ʈ ���� ���ڰ� ���� â�� ũ��, ���� �κи�
	vector<wstring>		m_vecQuestDesc;					// ���õ� ����Ʈ�� ����
	int					m_iQuestDescIndex;
	int					m_iQuestDescLinePerPage;


	// �� ����Ʈ â ����
	CKTDGUIDialogType			m_pDLGQuickQuest;
	bool					m_bQuickQuestOpenedUP;
	bool					m_bQuickQuestOpen;

	// ����Ʈ ���� ����..
	map< int, int >		m_mapItemIDAndSocketID;			// ���� ���� �������� ������ �ִ� �׷� ���� ������

};
#endif SERV_EPIC_QUEST
