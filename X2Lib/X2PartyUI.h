#pragma once

class CX2PartyUI
: public CX2ItemSlotManager
{
public:
	enum PARTY_UI_CUSTOM_MSG
	{
		// ��Ƽ ����Ʈ UI
		PUCM_PARTY_LIST_DUNGEON_SELECT = 14000,		//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_LIST_PREV_PAGE,					//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_LIST_NEXT_PAGE,					//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_LIST_JOIN_PARTY,					//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_LIST_CLOSE_WINDOW,				//���� ������� �ʴ� ���//2012-10-23


		// ��Ƽ ����Ʈ�� ��Ƽ ���Ե�
		PUCM_PARTY_LIST_SLOT_CLICKED,				//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_LIST_SLOT_DOUBLE_CLICKED,		//���� ������� �ʴ� ���//2012-10-23


		// ��Ƽ ����� UI
		PUCM_PARTY_NAME,
		PUCM_PARTY_DIFFICULTY_LEFT,					//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_DIFFICULTY_RIGHT,				//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_MAX_SLOT_LEFT,					//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_MAX_SLOT_RIGHT,					//���� ������� �ʴ� ���//2012-10-23
		PUCM_PARTY_QUICK_PARTY,						// ���� ��û(��Ī)
		PUCM_DUNGEON_PARTY_MATCH_CANCEL,			/// ������Ƽ ��Ī ���� �� ���,
		PUCM_PARTY_VIEW_PARTY_LIST,
		PUCM_PARTY_GAME_START,						/// ���� �ο����� �����ϱ�
		PUCM_PARTY_GAME_READY,						/// ���� ������� �ʴ� ��� //2012-10-23
		PUCM_PARTY_CLOSE_WINDOW,
		PUCM_PARTY_PUBLIC,
		PUCM_PARTY_LEAVE_PARTY,
		PUCM_PARTY_OPEN_LOCAL_MAP,

		PUCM_PARTY_LEAVE_DUNGEON_LOUNGE,
		PUCM_PARTY_GAME_RESTART,					// fix!! ������� ����


		PUCM_PARTY_ITEM_GET_RANDOM,
		PUCM_PARTY_ITEM_GET_EACH,


		PUCM_PARTY_LEADER_ACCEPT_JOIN_REQUEST,
		PUCM_PARTY_LEADER_REJECT_JOIN_REQUEST,

		PUCM_ACCEPT_INVITE_REQUEST,					// �������� ��Ƽ �ʴ뿡 ����
		PUCM_REJECT_INVITE_REQUEST,					// �������� ��Ƽ �ʴ뿡 ����


		PUCM_PARTY_MEMBER_CLICKED,

		PUCM_PARTY_PLAY_SOLO,

		// ��Ƽ ����Ʈ�� ��Ƽ ���Ե� ��Ŭ��
		PUCM_PARTY_LIST_SLOT_RCLICKED,

		PUCM_PARTY_LEVEL_LIMIT,
		PUCM_CREATE_LEVEL_UNLIMIT_PARTY_OK,
		PUCM_CREATE_LEVEL_UNLIMIT_PARTY_CANCEL,

		// ������Ī �� ��Ƽâ ����
		PUCM_SELECT_DUNGEON_MENU,
		PUCM_SELECT_PVP_MENU,
		PUCM_PVP_SETTING,
		PUCM_PVP_SETTING_CLOSE,
		PUCM_PVP_MAP_SETTING,
		PUCM_PVP_ETC_SETTING,
		PUCM_PVP_SETTING_SAVE_CLOSE,
		PUCM_PVP_MAP_SELECT,
		PUCM_PVP_MAP_CHECK,		
		PUCM_PVP_SETTING_1,
		PUCM_PVP_SETTING_2,
		PUCM_PVP_CREATE_PARTY,
		PUCM_PVP_START_MATCH,
		PUCM_PVP_CANCEL_MATCH,
		PUCM_PVP_CHECK_START_OK,						// ���� ��Ī �Ϸ� Ȯ��
		PUCM_PVP_CHECK_START_CANCEL,					// ���� ��Ī �Ϸ� ���
		PUCM_PVP_LEAVE_PARTY,
		PUCM_PVP_LEAVE_PARTY_OK,
		PUCM_PVP_LEAVE_PARTY_CANCEL,
		PUCM_PVP_CANCEL_MATCH_OK,
		PUCM_PVP_CANCEL_MATCH_CANCEL,

		// �ʵ� ���� ��Ƽ ����
		PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_OK,		/// ���� ���� �� �غ� �Ǿ���
		PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_CANCEL,	/// ���� ���� �� �غ� ���� �ʾ���
		PUCM_REWARD_ITEM_MOUSE_OVER,					// ���� ���� ������ ����
		PUCM_REWARD_ITEM_MOUSE_OUT,						// ���� ���� ������ ����
		PUCM_DUNGEON_DIFFICULTY_CHECKED,				// ���� ���̵� ����
		PUCM_DUNGEON_DIFFICULTY_UNCHECKED,				// ���� ���̵� ����
	};

	enum GAME_TOOL_BUTTON_STATE
	{
		BUTTON_START,
		BUTTON_WAIT,
		BUTTON_CANCEL
	};
#ifdef SERV_PVP_NEW_SYSTEM
	struct PVP_MAP_INFO
	{
		int					m_WorldID;

		wstring				m_MapName;
		wstring				m_MapSize;
		wstring				m_MapDesc;

		wstring				m_MapTexName;
		wstring				m_MapTexKeyName;
		

		PVP_MAP_INFO()
		{
			m_WorldID	= 0;
		}
	};
#endif

public:
	CX2PartyUI( CKTDXStage* pNowStage );
	virtual ~CX2PartyUI(void);

public:
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	
	//2008.12.14 ���¿� : UIManager ���� �۾�
    void SetLayer(X2_DIALOG_LAYER layer);
	bool GetShow();


	void OnChangeState();	 
	void OnJoinParty();
	void UpdatePartyMenu();

	void OpenPartyDLG( bool bOpen );
	void UpdatePartyDLG( bool bRenamePartyTitle = false );
	void CreatePartyDlg();

	void OpenLocalMapDlg();


	bool IsGameStartable( CX2LocationManager::LOCAL_MAP_ID eCurrLocalMapID, const CX2Dungeon::DungeonData* pDungeonData, bool bHaveParty, bool bPartyLeader );

	//void OpenPartyMemberDLG( bool bOpen );
	//void UpdatePartyMemberDLG();

	void OpenPartyListMemberPreviewDLG( bool bOpen, int iPartyIndex = 0 );
	void UpdatePartyListMemberPreviewDLG( int iPartyIndex );


#ifdef HENIR_TEST
	void OpenPartyFeverDLG( bool bOpen );
	void UpdatePartyFeverDLG();
#endif HENIR_TEST



	// 09.03.06 ���¿�
	void EnableDialog(bool bEnable);

	void SetStage(CKTDXStage* val);

	void SetPartyListPageNumber( int iCurr, int iMax )
	{
		m_iPartyListPageNumber = iCurr;
		m_iMaxPartyListPageNumber = iMax;
	}

#ifndef REFORM_NOVICE_GUIDE
	void SetNoviceGuideStep(int iStep);
#endif //REFORM_NOVICE_GUIDE
	bool GetShowLocalMap();

	// 09.04.01 �¿�
	void PlayWarningAllReady();
	void StopWarningAllReady();
	bool IsReadyPossible();			// ���� ��Ȳ�� ���� �ص� �Ǵ���
	
#ifdef SERV_PVP_NEW_SYSTEM
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void InitPvpSetting();
	void OpenPartyMenu(bool bOpen);
	void OpenPvpSetting(bool bOpen, bool bMap = true);	
	bool GetShowPartyMenu() 
	{ 
		if( m_pDLGPartyMenu != NULL )
			return m_pDLGPartyMenu->GetShow(); 
		return false;
	}

	bool IsMouseOverPartyUI()
	{
		if( m_pDLGParty != NULL && m_pDLGParty->GetShow() == true )
			return m_pDLGParty->GetIsMouseOver();
		else if( m_pDLGPartyPVP != NULL && m_pDLGPartyPVP->GetShow() == true )
			return m_pDLGPartyPVP->GetIsMouseOver();
		else if( m_pDlgPartyPvpMapSetting != NULL && m_pDlgPartyPvpMapSetting->GetShow() == true )
			return m_pDlgPartyPvpMapSetting->GetIsMouseOver();

		return false;
	}

	void ResetePvpMapSetting();
	void SavePvpMapSetting();
	void LoadPvpMap();
	void LoadPvpMapUI();
	void AddPvpMapList(CKTDGUIContolList *pControlList, int row, int pvpMapInfoIndex);

	bool GetDungeonPartyMenu() { return m_bPartyDungeon; }
	bool GetPvpPartyMenu() { return m_bPartyPVP; }
	void SetDungeonPartyMenu(bool bVal) { m_bPartyDungeon = bVal; m_bPartyPVP = !bVal; }
	void SetPvpPartyMenu(bool bVal) { m_bPartyPVP = bVal; m_bPartyDungeon = !bVal; }
		
	void UpdateMapInfo(int iMapIndex);
	void UpdatePvpPartyDlg();
		
	void CheckPvpStart();
	void GetSelectedPvpMap(std::set<short> &selectedMap);

	char GetPvpMatchMode();	
	char GetPvpMatchPlayerNum();	
	void SetPvpMatchPlayerNum(int iVal);
	void ShowPvpModeDesc();

	void SetEstimatedTime(int iVal) { m_iEstimatedTime = iVal; m_fWaitPvpTime = 0.f; }
#endif


#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	CKTDGUIDialogType CreateRewardItemInfoDlg (  const WCHAR * wDlgName );										// RewardItemDlg �� ����
	void ChangeRewardItemInfoDlgLayer ( bool bOpen, int iLayerToChange );	// �ش� Dlg �� Layer �� ����
	void SetShowEnableRewardItemInfoDlg ( bool bOpenMajor, bool bOpenMinor, bool bEnableFlag );			// NullCheck ����, bEnableFlag �� true �̸� SetShowEnable ȣ��, false �� SetShow ȣ��
#endif	// FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION


	void StartToWaitForDungeonPartyMatching();
	void CancelToWaitForDungeonPartyMatching();
	void StartToWaitForPvpMatching();
	void CancelToWaitForPvpMatching();
	void OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID);



	void SetBossDropItemTexture( int iIndex_, int iItemID_ = -1, bool bShow_ = false);

	//{����, ��Ƽ ��ư ���� ��� ����
	void SetPVPButton(GAME_TOOL_BUTTON_STATE eButtonState_ = BUTTON_START, bool bChecked_ = true, bool bForceDisable = false);
	void SetDungeonButton(GAME_TOOL_BUTTON_STATE eButtonState_ = BUTTON_START, bool bChecked_ = true, bool bForceDisable = false);


	//{����, ��Ƽ ��ư ���� ��� ����}

	bool GetShowDungeonMenu()
	{ 
		if( NULL != m_pDLGParty) 
			return m_pDLGParty->GetShow(); 
		else
			return false; 
	}
	bool GetShowPVPMenu()
	{ 
		if( NULL != m_pDLGParty) 
			return m_pDLGPartyPVP->GetShow(); 
		else
			return false; 
	}
	 
	CX2LocalMapUI* GetLocalMapUI(){return m_pLocalMapUI; }
	void	UpdateDungeonRequestButton();

#ifdef NEW_DEFENCE_DUNGEON
	void SetChangePartyUI(bool bChangePartyUI) { m_bChangePartyUI = bChangePartyUI; } //����� �� ���̵��� ���� �Ǿ��� ��, EGS_PARTY_CHANGE_DUNGEON_REQ �� ȣ���ϵ��� �ִ� ��ȣ
#endif NEW_DEFENCE_DUNGEON

	void UpdateDifficultyUI(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_, 
						CX2Dungeon::DUNGEON_TYPE eDungeonType_ = CX2Dungeon::DT_NORMAL, 
						bool pNormalOnly = false ); //��������â UI ����
	void UpdateDifficultyButton(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_); //����, �����, �ſ����� ��ư ����
	void UpdateDifficultyButtonForSpecialDungeon(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_, CX2Dungeon::DUNGEON_TYPE eDungeonType_); // ��, ���� ��ư ����(�ð�,���)
	void UpdateDungeonTypeButton(CX2Dungeon::DUNGEON_TYPE eDungeonType_); //�ð�, ���, �Ϲݴ����� ���̵� ��ư ���� ����
	void UpdateAdequateMemberCount(CX2Dungeon::DUNGEON_TYPE eDungeonType_, CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_); //���� �ο��� ����
	void UpdateNormalOnly(CX2Dungeon::DUNGEON_TYPE eDungeonType_, bool bNormalOnly_); //�����ư�� �ִ� ��쿡 ���� ����
	void UpdateAdequateItemLevel(int iItemLevel_);
	void UpdateDungeonThumbnailQuestMark();


	void UpdateBossItemList(const CX2Dungeon::DungeonData::mapBossDropItem& BossDropItemList_, bool bIsHenirChallenge = false );

	void SetCurrentWaitUserCount( int iWaitUserCount_, bool bShow_ = true );
	void GameStartCurrentMember();
	
#ifdef SERV_NEW_EVENT_TYPES
	void ClearLocalMapDLGs();
#endif SERV_NEW_EVENT_TYPES		
private:
	void ShortCutKeyProcess();
	void UpdateMatchingUI(float fElapsedTime_);
#ifdef QUEST_GUIDE
	void UpdateQuestGuide();
#endif //QUEST_GUIDE
#ifdef REFORM_NOVICE_GUIDE
	void UpdateNoviceGuide();
#endif //REFORM_NOVICE_GUIDE

	bool SelectDungeonDifficulty( const int iSelectDungeonDifficulty_);
	static const int					MAX_PARTY_COUNT_PER_PAGE	= 12;	// ��Ƽ����Ʈ�� �ѹ��� 12���� ����
	static const double 				PARTY_LIST_REFRESH_INTERVAL;

	CKTDXStage*							m_pCurrStage;

	CKTDGUIDialogType						m_pDLGPartyList;			// ��Ƽ����Ʈ
	CKTDGUIDialogType						m_pDLGParty;				// ��Ƽ �⺻ ���, ��Ƽ�����, �����ϱ� ����� ��ư�� �ִ�
	//CKTDGUIDialogType						m_pDLGPartyMember;			// �� ��Ƽ�� ����� ����
	CKTDGUIDialogType						m_pDLGPartyListMeberPreview;	// ��Ƽ����Ʈ���� ��Ƽ�� ���� �̸�����
	CKTDGUIDialogType						m_pDLGWarningAllReady;		// (��Ƽ�� ����) ��� ��Ƽ���� ���� ������ �˸��� ���
	CKTDGUIDialogType						m_pDLGPartyFever;

	CKTDGUIDialogType						m_pDLGQuestRewardItemInfo;
#ifdef 	FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	CKTDGUIDialogType						m_pDLGQuestRewardItemInfoAdd;
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

#ifdef SERV_PVP_NEW_SYSTEM
	std::vector<PVP_MAP_INFO>				m_vecPvpMapInfo;
	std::vector<short>						m_vecDefaultMap;
	SHORT									m_SumDelta;
	bool									m_bPartyDungeon;			// ���� ���õ� ��Ƽ�� �������� ����
	bool									m_bPartyPVP;				// ���� ���õ� ��Ƽ�� �������� ����
	CKTDGUIDialogType						m_pDLGPartyMenu;			// ����/���� ��Ƽ ���� ��ư�޴�
	CKTDGUIDialogType						m_pDLGPartyPVP;
	CKTDGUIDialogType						m_pDLGPartyPVPSetting;
	CKTDGUIDialogType						m_pDLGPartyPVPSettingButton;
	CKTDGUIDialogType						m_pDlgPartyPvpMapSetting;
	//CKTDGUIDialogType						m_pDlgPartyPvpEtcSetting;
	CKTDGUIDialogType						m_pDlgPvpConfirm;	

	int										m_iEstimatedTime;
	float									m_fWaitPvpTime;	
#endif
	
	CX2LocationManager::LOCAL_MAP_ID	m_ePartyListLocalMapID;		// ��Ƽ����Ʈ â�� ���� ��Ƽ����Ʈ�� ���� ���� ID 
	int									m_iPartyListDungeonID;		// ��Ƽ����Ʈ���� ���õ� ���� ID	

	int									m_iPartyListPageNumber;		// [1,x]
	int									m_iMaxPartyListPageNumber;	//

	int									m_iPickedPartyListSlotIndex;	// [0, MAX_PARTY_COUNT_PER_PAGE]

	CX2PartyManager::PartyData			m_aPartyData[ MAX_PARTY_COUNT_PER_PAGE ];		// ��Ƽ����Ʈ�� ���� ��Ƽ data

	CKTDXTimer							m_timerPartyListRefresh;

	CX2LocalMapUI*						m_pLocalMapUI;
	int									m_iNoviceGuideStep;

	float								m_fElapsedTime;				// ��Ƽ���� ��Ÿ��

	std::map<wstring, int>				m_mapBossItemList;
	bool								m_bShowQuestMark;				// ���� ���� ��ư // �������� ��ư ���� Q��ũ ���� ����
	// ������ // 2012-10-12 // ����ϴ� ���� �������� ��� ������ ������ �ֵ��� ����
	CKTDGUICheckBox*					m_pDungeonCheckBox;		// ���� ���� ��ư
	CKTDGUICheckBox*					m_pPVPCheckBox;	

#ifdef NEW_DEFENCE_DUNGEON
	bool								m_bChangePartyUI;			//����� �� ���̵��� ���� �Ǿ��� ��, EGS_PARTY_CHANGE_DUNGEON_REQ �� ȣ���ϵ��� �ִ� ��ȣ
#endif NEW_DEFENCE_DUNGEON

	GAME_TOOL_BUTTON_STATE				m_eDungeonButton;
	GAME_TOOL_BUTTON_STATE				m_ePVPButton;

#ifdef FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN
	LPARAM								m_lParam;	// ���� ��Ŀ�� ���� ���� ������ ��ư�� ��Ƶ�
#endif // FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hValentineHeart1;		/// �߷�Ÿ�� ���� �ȳ� ����Ʈ 1
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hValentineHeart2;		/// �߷�Ÿ�� ���� �ȳ� ����Ʈ 2
#endif SERV_EVENT_VALENTINE_DUNGEON
};
