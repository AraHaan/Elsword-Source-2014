#ifdef SERV_SKILL_NOTE

#pragma once
class CX2UISkillNote
{
public:
	enum UI_SKILL_NOTE_MSG
	{
		USNM_EXIT = 39000,		
		USNM_REGISTER_VIEW_SLOT,
		USNM_SLOT_PAGE_LEFT,
		USNM_SLOT_PAGE_RIGHT,
		USNM_REGISTER_OK,
		USNM_REGISTER_CANCEL,
		USNM_PAGE_PREV,
		USNM_PAGE_NEXT,

		USNM_USE_MEMO,						// �޸� ���
		USNM_BUY_MEMO,						// �޸� ����
		USNM_HAS_MEMO_MOUSE_OVER,			// ���� �޸� ����
		USNM_NOT_HAS_MEMO_MOUSE_OVER,		// �̺��� �޸� ����
	};

public:

	CX2UISkillNote( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UISkillNote(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetShow(bool val);
	bool GetShow(){ return m_bShow; }
	
	bool OpenDialog();

	void SetLayer(X2_DIALOG_LAYER layer);

	void RegisterMemo(UidType iMemoId);
	void SetRegisterMemo(bool bVal) { m_bRegisterMemo = bVal; }
	bool GetRegisterMemo() { return m_bRegisterMemo; }
    
	void UpdateMemoSlot(int iPage = 0);
	void UpdateMemo(bool bVal, char cPage);	

	char GetUpdatePage() { return m_iUpdatePage; }
#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
	bool GetUpdateMemo() { return m_bUpdateMemo; }
	float GetWriteMemoTime() { return m_fWriteMemoTime; }
#endif // REFORM_SKILL_NOTE_UI
	void ResetSkillNote();


#ifdef REFORM_SKILL_NOTE_UI
	// ���� ���� ����. (����� ��Ʈ�� ������ ���¿����� �ҷ� �� �� �ִ� )
	void UpdateMemoList_MemoItemNum();	
	
	void ResetMemoListDLG();
#endif // REFORM_SKILL_NOTE_UI

private:	
	void ViewMemo(int iPage);
	void Handler_EGS_REG_SKILL_NOTE_MEMO_REQ(char iPage);	
	void CheckRegisterMemo();
	void UpdateMemoEffect();	
    wstring ReplaceDesc_(const wchar_t* pwstrOri, const wchar_t* pwstrPattern, const wchar_t* pwstrReplace);

#ifdef REFORM_SKILL_NOTE_UI
	// �޸� ����
	void GetMemoDesc( OUT wstring& wstrMemoDesc_, int iMemoItemID_ = 0, bool bHas_ = false);


	// �޸� ����Ʈ ����
	void				ResetMemoList();
	void				ResetMemoListUI();

	void				SetMemoListData_MemoData( CKTDGUIStatic *pStaticMemo_, int iSkillID_, int iMemoItemID_, int iMemoItemNum_ );
	void				SetMemoListData_MemoButton( CKTDGUIButton *pButton_, int iMemoItemID_, int iMemoItemNum_ );
#endif // REFORM_SKILL_NOTE_UI

private:
	char					m_iRegisterPage;
	char					m_iCurrentPage;	
	char					m_iUpdatePage;
	bool					m_bShow;
	bool					m_bRegisterMemo;
	UidType					m_iRegisterMemo;
	int						m_iViewPage;

	CKTDGUIDialogType		m_pDLGSkillNote;
	CKTDGUIDialogType		m_pDLGQuery;
	CKTDGUIDialogType		m_pDLGInfoBox;
#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
	CKTDGUIDialogType		m_pDLGWrittingNote;
	float					m_fWriteMemoTime;
	bool					m_bUpdateMemo;	
	std::vector<wstring>	m_vecSlotLv;
#endif // REFORM_SKILL_NOTE_UI

	CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticle1;	
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticle2;	

#ifdef REFORM_SKILL_NOTE_UI
	SHORT					m_SumDelta;
	/* #NOTE : ����� ��Ʈ UI�� �и� ��Ŵ.
	�и� ��Ų ����  
	//1. ��Ʈ�� ����Ʈ���� ��Ʈ���� ĳ���Ͱ� ����Ǹ� �����ؾ� �ϴµ�, ���Ű� ���� ��.	
	//2. ���콺 ������ �� �����ؾߵǴµ�, ���� ���콺 ������ ���̾�α� �����θ� ���� ���� ��.	*/	
	CKTDGUIDialogType		m_pDLGMemoList;	
#endif // REFORM_SKILL_NOTE_UI

};

#endif
