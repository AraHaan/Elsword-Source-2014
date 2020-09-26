// X2UISkillTree.h : �Ÿ��� �� ��ųƮ�� UI
// ���� ��ǥ - ��� Data�� X2UserSkillTree�� X2SkillTree���� ������ �� : UI Class�� UI ���Ҹ� �Ѵ�!




#ifdef NEW_SKILL_TREE_UI

#pragma once


#ifdef GUILD_SKILL
// oasis907 : ����� [2009.11.11] //  CX2UISkillTree�κ��� �����Ͽ� �۾� ����
class CX2GuildSkillTreeSlotData;

class CX2UIGuildSkillTree
{
	enum GUILD_SKILL_TREE_UI_CUSTOM_MSG
	{
		GSTUCM_EXIT	= 36000,
	
		GSTUCM_BUTTON_SKILL_LEVEL_PLUS,
		GSTUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOVER,
		GSTUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOUT,

		GSTUCM_BUTTON_SKILL_LEVEL_MINUS,
		GSTUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOVER,
		GSTUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOUT,

		GSTUCM_BUTTON_SKILL_MASTER_MOUSEOVER,
		GSTUCM_BUTTON_SKILL_MASTER_MOUSEOUT,

		GSTUCM_EQUIPSLOT_DROPPED,
		GSTUCM_EQUIPSLOT_RMOUSEUP,
		GSTUCM_EQUIPSLOT_MOUSEIN,
		GSTUCM_EQUIPSLOT_MOUSEOUT,	

		GSTUCM_SKILLTREESLOT_RMOUSEUP,
		GSTUCM_SKILLTREESLOT_MOUSEIN,
		GSTUCM_SKILLTREESLOT_MOUSEOUT,

		GSTUCM_SKILL_RESET_MODE_CHANGE,
		GSTUCM_SKILL_RESET_MODE_CHANGE_MOUSEOVER,
		GSTUCM_SKILL_RESET_MODE_CHANGE_MOUSEOUT,

		GSTUCM_CONTROLLIST_SCROLL,

		GSTUCM_NAVI_LEFT_CLICKED,
		GSTUCM_NAVI_DRAGGING,

		//GSTUCM_INGAME_ICON_MOUSE_OVER,
		//GSTUCM_INGAME_ICON_MOUSE_OUT,

		GSTUCM_FIRST_GUIDE_CLOSE,
		GSTUCM_FIRST_GUIDE_DONOTSHOW,

		GSTUCM_CSP_GUIDE_MOUSEIN,
		GSTUCM_CSP_GUIDE_MOUSEOUT,
		GSTUCM_SKILLTREESLOTB_GUIDE_MOUSEIN,
		GSTUCM_SKILLTREESLOTB_GUIDE_MOUSEOUT,

		GSTUCM_UNSEAL_SKILL_CANCLE,
		GSTUCM_UNSEAL_SKILL_BUY,

		GSTUCM_GUIDE_BOOK,
		
	
	};

public:

	struct SkillSlotUI
	{
		CKTDGUIStatic*	m_pStaticBackGround;
		CKTDGUISlot*	m_pSlot;
		CKTDGUIButton*	m_pButtonPlus;
		CKTDGUIButton*	m_pButtonMinus;
		CKTDGUIButton*	m_pButtonMaster;
		CKTDGUIStatic*	m_pStaticLevelString;
		CKTDGUIStatic*	m_pStaticActiveMark;
		CKTDGUIStatic*	m_pStaticLock;				// ���ν�ų ǥ�� �ڹ���


		SkillSlotUI()
		{
			m_pStaticBackGround = NULL;
			m_pSlot = NULL;
			m_pButtonPlus = NULL;
			m_pButtonMinus = NULL;
			m_pButtonMaster = NULL;
			m_pStaticLevelString = NULL;
			m_pStaticActiveMark = NULL;
			m_pStaticLock = NULL;
		}
		// �Ҹ��ڴ� �ʿ���� : ���� Dialog �Ҹ��ڿ��� �����
	};

	struct ArrowUI
	{
		CKTDGUIStatic*			m_pStaticArrowEnd;
		vector<CKTDGUIStatic*>	m_vpStaticArrowLine;
		CKTDGUIStatic*			m_pStaticArrowHead;

		ArrowUI()
		{
			m_pStaticArrowEnd = NULL;
			m_pStaticArrowHead = NULL;
		}
		// �Ҹ��ڴ� �ʿ���� : ���� Dialog �Ҹ��ڿ��� �����
	};

public:

	CX2UIGuildSkillTree( CKTDXStage* pNowStage );
	virtual ~CX2UIGuildSkillTree(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool GetShow() { return m_bShow; }
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );

	void InitSkillTreeUI();
	void UpdateSPInfo();
	

	void CreateSkillTree( CX2Unit::UNIT_CLASS eUnitClass );
	void UpdateSkillTree();
	void UpdateSkillTree( CX2SkillTree::SKILL_ID eSkillID );		// Tree���� �ش� Slot�� ������Ʈ�Ѵ�
	void UpdateBlind();

	void UpdateUI( bool bEquipped, bool bSP = true, bool bSkillTree = false );
    
	void CreateSlotUIPreset(SkillSlotUI& UISet, int eSkillID );
	void CreateVerticalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID );	// iLineLength : Head�� Tail�� ������ ����.
	void CreateHorizonalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID, bool bRight );

	const map<CX2SkillTree::SKILL_ID, SkillSlotUI>& GetMapSkillSlotUISet(){ return m_mapSkillSlotUISet; }

	//SkillSlotUI& GetSkillSlotUI( CX2SkillTree::SKILL_ID eSkillID );
	//ArrowUI& GetArrowUI( CX2SkillTree::SKILL_ID eSkillID );
	void SetArrowColor( CX2SkillTree::SKILL_ID eSkillID, D3DXCOLOR cCol );

	bool GetRemoveMode(){ return m_bSkillRemoveMode; }


	//void InitEquippedSkillSlotData();

	//////////////////////////////////////////////////////////////////////////

	//** fix!! ��ų �������� ���̾�α׸� ���� �Լ�. �߰� �۾� �ʿ��մϴ�.
	void OpenSkillUnsealInformDLG( const CX2SkillTree::SkillTemplet* pSkillTemplet, D3DXVECTOR2 SlotCenterPos );	

	void OpenDLGSkillUnsealed( bool bOpen );
	void UpdateDLGSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID );


	
	//////////////////////////////////////////////////////////////////////////

	void ShowSkillDesc( bool bShow, CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE, D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
						D3DXVECTOR2 size = D3DXVECTOR2(0,0), bool bEquipped = false );
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

	void GetPrecedingSkillDesc( OUT wstring& wstrDesc, CX2SkillTree::SKILL_ID ePrecedingSkill ) const;
	void GetSkillEffectiveDesc( OUT wstring& wstrDesc, IN int iSkillLevel, IN const CX2SkillTree::SkillTemplet* pSkillTemplet ) const;
	void GetSkillTitleDesc( OUT wstring& wstrTitleDesc, IN const CX2SkillTree::SkillTemplet* pSkillTemplet, 
		OUT const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet,IN int iSkillLevel ) const;
	void GetSkillSubDesc( IN const CX2SkillTree::SkillTemplet* pSkillTemplet, OUT wstring& wstrSubDesc ) const;

#else // UPGRADE_SKILL_SYSTEM_2013

	wstring GetSkillDesc( CX2SkillTree::SKILL_ID eSkillID, bool bEquipped = false );

#endif // UPGRADE_SKILL_SYSTEM_2013
	
	void ShowUIDesc( bool bShow, wstring wstr = L"", D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
					 CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT = CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT,
					 D3DXCOLOR coTextColor = D3DXCOLOR(0,0,0,1), D3DXCOLOR coBackColor = D3DXCOLOR(1, 0.96f, 0.6f, 1 ) );
	//wstring GetExpirationDateDesc( const wstring& wstrExpirationDate );	// kimhc // KTDXCommonFunc�� �ű� // 2009-06-16
	
	//////////////////////////////////////////////////////////////////////////

private:
	bool					m_bShow;
	bool					m_bSkillRemoveMode;
	bool					m_bClassInfoVaild;
	//D3DXVECTOR2				m_MovedPosition;
	//D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUISkillTree;   
	CKTDGUIDialogType			m_pDLGSkillToolTip;	
	CKTDGUIDialogType			m_pDLGSkillUnsealInform;	
	CKTDGUIDialogType			m_pDLGSkillUnsealed;		// ��ų �������� �Ϸ� �˾�


	

	map<CX2SkillTree::SKILL_ID, SkillSlotUI>	m_mapSkillSlotUISet;
	std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>		m_mapArrowUISet;			// ȭ��ǥ�� ���۵Ǵ� ��ų(���� ��ų) ID�� key�� ��´�. �� ��ų�� ���� ��ų�� ������ �� �����Ƿ� ��Ƽ��.

	const CX2UserSkillTree*		m_pUserSkillTree;
		
	// ���콺 �� ����
	SHORT				m_SumDelta;
	int					m_iPrevSP;	// UpdateSP�� �� �ٶ� ���� SP ���¿� ���ؼ� Tree Update�� �ʿ����� �����Ѵ�


	// ToolTip : TalkBox �̿��ؼ�.
	CX2TalkBoxManager*		m_pTalkBoxManager;
	bool					m_bUsingCSP;

	
};


//////////////////////////////////////////////////////////////////////////

class CX2GuildSkillTreeSlotData : public CKTDGUISlotData
{

public:

	enum SKILL_TREE_SLOT_ADD_PICTURE_TYPE
	{
		STSAPT_DISABLE = 0,
		STSAPT_EQUIPPED,
		STSAPT_SEALED,
		STSAPT_LEVEL,
		STSAPT_CANLEARN,

		STSAPT_END,
	};

	CX2GuildSkillTreeSlotData();
	virtual ~CX2GuildSkillTreeSlotData();

	virtual void UpdateUI( CKTDGUISlot* pSlot );
	void ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_TREE_SLOT_ADD_PICTURE_TYPE eType, int val = -1 );


	//CX2SkillTree::SkillTreeTemplet*	pSkillTemplet;
	CX2SkillTree::SKILL_ID m_eSkillID;
};


#endif GUILD_SKILL

#endif NEW_SKILL_TREE_UI



