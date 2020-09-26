#pragma once


#ifdef ITEM_VIEW_CHEAT

class ItemViewListDialog
{

public: 
	ItemViewListDialog() 
		: m_pDLGItemViewList( NULL )
		, m_bSortItemViewList( false )
	{
	}

	~ItemViewListDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGItemViewList );
	}

	bool IsOpen()
	{
		if( m_pDLGItemViewList == NULL )
			return false;

		if( false == m_pDLGItemViewList->GetShow() )
			return false;
		else
			return true;
	}

	void Close()
	{
		if( m_pDLGItemViewList != NULL )
		{
			m_pDLGItemViewList->SetShowEnable( false, false );
		}
	}

	void Open( CKTDXStage* pStage )
	{
		if( m_pDLGItemViewList == NULL )
		{
			m_pDLGItemViewList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemViewList );

			Update( m_bSortItemViewList );
		}

		if( m_pDLGItemViewList != NULL )
		{
			m_pDLGItemViewList->SetStage( pStage );
			m_pDLGItemViewList->SetShowEnable( true, true );
		}
	}

	void Update( bool bSortAsscending );

	void KeyProcess( CKTDXStage* pStage );
	void OnCommand( const int& ItemID );
	void OnChangeSorting()
	{
		m_bSortItemViewList = !m_bSortItemViewList;
		Update( m_bSortItemViewList );
	}

	CKTDGUIDialogType GetDialog() { return m_pDLGItemViewList; };

protected:
	CKTDGUIDialogType m_pDLGItemViewList;

	bool m_bSortItemViewList;


};

#endif ITEM_VIEW_CHEAT




#ifdef CREATE_MONSTER_LIST_TEST
class CreateMonsterListDialog
{
public: 
	CreateMonsterListDialog() 
		: m_pDLGCreateMonsterList( NULL )
		, m_bSortMonsterList( false )
	{
	}

	~CreateMonsterListDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGCreateMonsterList );
	}

	bool IsOpen()
	{
		if( m_pDLGCreateMonsterList == NULL )
			return false;

		if( false == m_pDLGCreateMonsterList->GetShow() )
			return false;
		else
			return true;
	}

	void Close()
	{
		if( m_pDLGCreateMonsterList != NULL )
		{
			m_pDLGCreateMonsterList->SetShowEnable( false, false );
		}
	}

	void Open( CKTDXStage* pStage )
	{
		if( m_pDLGCreateMonsterList == NULL )
		{
			m_pDLGCreateMonsterList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateMonsterList );

			Update( m_bSortMonsterList );
		}

		if( m_pDLGCreateMonsterList != NULL )
		{
			m_pDLGCreateMonsterList->SetStage( pStage );
			m_pDLGCreateMonsterList->SetShowEnable( true, true );
		}
	}

	void Update( bool bSortAlphabetical );

	void KeyProcess( CKTDXStage* pStage );
	void OnCommand( const std::wstring& monsterName );
	void OnChangeSorting()
	{
		m_bSortMonsterList = !m_bSortMonsterList;
		Update( m_bSortMonsterList );
	}

	CKTDGUIDialogType GetDialog() { return m_pDLGCreateMonsterList; }

protected:
	CKTDGUIDialogType m_pDLGCreateMonsterList;
	
	bool m_bSortMonsterList;

}; // class CreateMonsterListDialog

#endif CREATE_MONSTER_LIST_TEST




#ifdef MONSTER_STATE_LIST_TEST
// class MonsterStateListDialog
// {
// public: 
// 	MonsterStateListDialog() 
// 		: m_pDLGMonsterStateList( NULL )
// 		, m_NPCID( CX2UnitManager::NUI_NONE )
// 	{
// 		m_vecStateName.reserve( 1024 ); 
// 	}
// 
// 	~MonsterStateListDialog()
// 	{
// 		SAFE_DELETE_DIALOG( m_pDLGMonsterStateList );
// 	}
// 
// 	bool IsOpen()
// 	{
// 		if( m_pDLGMonsterStateList == NULL )
// 			return false;
// 
// 		if( false == m_pDLGMonsterStateList->GetShow() )
// 			return false;
// 		else
// 			return true;
// 	}
// 
// 	void Close()
// 	{
// 		if( m_pDLGMonsterStateList != NULL )
// 		{
// 			m_pDLGMonsterStateList->SetShowEnable( false, false );
// 		}
// 	}
// 
// 	void Open( CKTDXStage* pStage )
// 	{
// 		if( m_pDLGMonsterStateList == NULL )
// 		{
// 			m_pDLGMonsterStateList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMonsterStateList );
// 		}
// 
// 		if( m_pDLGMonsterStateList != NULL )
// 		{
// 			m_pDLGMonsterStateList->SetStage( pStage );
// 			m_pDLGMonsterStateList->SetShowEnable( true, true );
// 
// 			Update();
// 		}
// 	}
// 
// 	void Update();
// 
// 	void KeyProcess( CKTDXStage* pStage );
// 
// 	void OnCommand( const std::wstring& monsterName );
// 
// 
// 	CKTDGUIDialogType GetDialog() { return m_pDLGMonsterStateList; }
// 
// 
// 
// protected:
// 	CKTDGUIDialogType m_pDLGMonsterStateList;
// 	std::vector< std::wstring > m_vecStateName; 
// 	CX2UnitManager::NPC_UNIT_ID m_NPCID;
// 
// }; // class MonsterStateListDialog
// 
#endif MONSTER_STATE_LIST_TEST



/** @class  BadAttitudeUserWarningDialog  
    @brief  �ҷ� ������ ���� ��� �޽��� ���̾˷α� 

    @desc   Instance �� ���۵Ǿ� �ִ�.
			DungeonGame ���¿����� ����ǹǷ� StateDungeonGame �� ��ġ���ִ�.
			DLG �� �׻� �����ϴ� ���� �ƴϴ� ���� �������� �Ҵ��� �� �ְ� �����ұ� ��� ���̴�.
			
    @date   ������ on 2013-01-24, 14:32 
*/

#ifndef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
class BadAttitudeUserWarningDialog
{
public: 

	BadAttitudeUserWarningDialog() 
		: m_pDLGWarningState ( NULL )
		, m_fWarningRemainingTime (0.f)
		, m_bIsStartFade ( false )
		, m_bIsEndFade ( false ) 
	{
	}

	~BadAttitudeUserWarningDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGWarningState );	// Safe Delete Dialog
	}

	bool IsOpen()								// �����ֳ�?
	{
		if( m_pDLGWarningState == NULL )		// �������� �ʾ���
		{
			return false;
		}
	
		if( false == m_pDLGWarningState->GetShow() )	// �����Ǿ����� �������� ���´� �ƴ�
		{
			return false;
		}
		else								// �����Ǿ� �ִ�.
		{
			return true;
		}
	}

	void Close()							// �ش� Dlg �� �ݴ´�.
	{										// �� ��ü�� instance ���̶� �������� �ʴ´�.
		if( m_pDLGWarningState != NULL )
		{
			m_pDLGWarningState->SetShowEnable( false, false );	// �������� ���� �� Ȱ��ȭ ���¸� ����
			Init ();											// �����͸� �ʱ�ȭ �Ѵ�. ( ����ð�, Fade bool �� )
		}
	}

	void Init ()												// �����͸� �ʱ�ȭ �Ѵ�.
	{															// �׸� �����ʹ� Instance ���� ��, �����صд�.
		m_fWarningRemainingTime = 0.f;						// ���� �ð�
		m_bIsStartFade = false;								// Fadein �� �ߴ°�?
		m_bIsEndFade = false;								// FadeOut �� �ߴ°�?
	}


	
	void Open( CKTDXStage* pStage )								// �ش� Dlg �� ���ٸ� �����ϰ� �ٽ� �������� �Ѵ�.
	{
		if( m_pDLGWarningState == NULL )						// ���� �������� �ʾҴٸ�
		{
			m_pDLGWarningState = new CKTDGUIDialog( pStage, L"DLG_Bad_Attribute_Player_Warning.lua" );	// Lua ������ �о� �����Ѵ�.
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarningState );				// DialogManager �� ����Ѵ�.
		}
		else													// �����߾��ٸ�
		{
			m_pDLGWarningState->SetStage( pStage );				// ���� Stage �� �����Ѵ�.
			m_pDLGWarningState->SetShowEnable( true, true );	// �������� �ϰ� Ȱ��ȭ�Ѵ�.
		}
				
		CKTDGUIStatic* pStaticWarningDlg = static_cast <CKTDGUIStatic*> ( m_pDLGWarningState->GetControl(L"DLG_Bad_Attribute_Player_Warning") );		// �ش� Static �� �ҷ��ͼ�

		if ( NULL != pStaticWarningDlg )			
		{
			m_pButtonExitWarningDlg = static_cast <CKTDGUIButton*> ( m_pDLGWarningState->GetButton_LUA("_exit") );

			if ( true == m_VecDlgPictureDataPointer.empty ()  )				// �׸� �����Ͱ� ����ִٸ�
			{	
				// 0�� �׸� �����͸� �����ͼ� m_VecDlgPictureDataPointer �� �־�д�.
				// �ϵ� �ڵ�..�׸��� �߰��Ǹ� ���⼭ push_back �ϵ��� ����
				m_VecDlgPictureDataPointer.push_back(pStaticWarningDlg->GetPicture(0));		// �ٽ� �ҷ��´�.
			}
		}
		Init ();	// �����͸� �ʱ�ȭ

	}

	void					SetIsFadeIn ( const bool bStartFade_ )			{ m_bIsStartFade = bStartFade_; }	// FadeIn Bool ���� ����
	bool					GetIsFadeIn () const							{ return m_bIsStartFade; }			// FadeIn Bool ���� �޴´�.

	void					SetIsFadeOut ( const bool bEndFade_ )			{ m_bIsEndFade = bEndFade_; }		// FadeOut Bool ���� ����
	bool					GetIsFadeOut () const							{ return m_bIsEndFade; }			// FadeOut Bool ���� �޴´�.

	CKTDGUIDialogType		GetDialog() const { return m_pDLGWarningState; }			// �ش� Dlg �����͸� �޴´�.

	void					SetFadeInDlg ( const float fTime_ );					// Dlg �� Fade in ����
	void					SetFadeOutDlg ( const float fTime_ );					// Dlg �� Fade Out ����

	void					Update ( float fElapsedTime_ );							// ������ �ð��� �°� �ൿ		

protected:
	CKTDGUIDialogType						m_pDLGWarningState;				// DLG State, LUA ���� �ҷ��� ���� �ִ´�.
	float									m_fWarningRemainingTime;		// DLG ���� ���� �����ߴ� �ð�, 0�� ���� ŭ = FadeIn, 5�� ���� ŭ = FadeOut, 6�ʺ��� ŭ = Destroy
	bool									m_bIsStartFade;					// true �̸� Fade-in �� �����߾���
	bool									m_bIsEndFade;					// true �̸� Fade-out �� �����߾���
	CKTDGUIButton*							m_pButtonExitWarningDlg;		// Fade-In, out �� ����� ��ư ������
	vector<CKTDGUIControl::CPictureData*>   m_VecDlgPictureDataPointer;		// Fade-in, out �� ����� �׸� ���ϵ�
};

#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG




class CX2StateDungeonGame : public CX2StateMenu
{
public:

	enum DUNGEON_GAME_UI_CUSTOM_MSG
	{
		DGUCM_GAME_LOADING = 0,
		DGUCM_GAME_START,
		DGUCM_GAME_EXIT_OK,

		DGUCM_GAME_EXIT_CANCEL,

		DGUCM_SUMMON_SELECTED_MONSTER_DEBUG,
		DGUCM_SORT_MONSTER_LIST_DEBUG,

		DGUCM_GOTO_FIRST_DUNGEON_OK,
		DGUCM_GOTO_FIRST_DUNGEON_CANCEL,

		DGUCM_TOTORIAL_EXIT,

		DGUCM_WANT_TO_ENTER_SECRET_STAGE_OK,
		DGUCM_WANT_TO_ENTER_SECRET_STAGE_CANCEL,

		DGUCM_CLOSE_BAD_ATTITUDE_WARNING_DIALOG,

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // ���볯¥: 2013-04-22
		DGUCM_AGREE_ENTER_GATE_OF_DARKNESS,		/// ����� �� ���� ����
		DGUCM_NOT_AGREE_ENTER_GATE_OF_DARKNESS,	/// ����� �� ���� �ź�
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

	};
// ��ũ��Ʈ�� �ε��� ����� ��. DLG_PVP_GAME_LOADING_STAT_USER.lua
	enum LOADING_USER_INFO
	{
		LUI_ELSWORD_COLOR = 0,
		LUI_ARME_COLOR,
		LUI_LIRE_COLOR,
		LUI_RAVEN_COLOR,
		LUI_EVE_COLOR,
		LUI_CHUNG_COLOR,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_COLOR,
#endif


		LUI_ELSWORD_BLACK,
		LUI_ARME_BLACK,
		LUI_LIRE_BLACK,
		LUI_RAVEN_BLACK,
		LUI_EVE_BLACK,
		LUI_CHUNG_BLACK,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_BLACK,
#endif
//#ifdef NEW_CHARACTER_EL 
		LUI_EL_COLOR,
		LUI_EL_BLACK,
//#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		LUI_ADD_COLOR,
		LUI_ADD_BLACK,
#endif //SERV_9TH_NEW_CHARACTER
		LUI_EMPTY,
	};

	struct DungeonMapUI
	{
	public:
		struct SubStageNode
		{
			vector< std::pair<int, int> >		m_vecNextSubStageIndex;
		};

	public:
        DungeonMapUI( CX2Dungeon* pDungeon, CX2State* pNowState )
		{
			m_bShow = false;

			m_pDungeon = pDungeon;
			CreatStageGraph( pDungeon );

			m_pNowState = pNowState;
			m_pDLGDungeonMap		= NULL;
			m_pDLGDungeonMap		= new CKTDGUIDialog( (CKTDXStage*) m_pNowState, L"DLG_DungeonMap_UI.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonMap );
			m_vPos = m_pDLGDungeonMap->GetPos();

			SetShow( m_bShow );
		}

		~DungeonMapUI()
		{
            SAFE_DELETE_DIALOG( m_pDLGDungeonMap );
		}

		bool GetShow() const { return m_bShow; }
		void SetShow( bool bShow ) 
		{ 
			m_bShow = bShow; 

			if( m_pDLGDungeonMap != NULL )
			{
				m_pDLGDungeonMap->SetShowEnable( m_bShow, m_bShow );
			}
		}

		void CreatStageGraph( CX2Dungeon* pDungeon );
		void CreateDungeonMapUI();
		void ResizeBackground();
		void OnFirstSubStage();

		void SetToOriginalPos()
		{
			if( m_pDLGDungeonMap == NULL )
				return;

			m_pDLGDungeonMap->Move( m_vPos, D3DXCOLOR(1, 1, 1, 1), 0.01f, true );
		}

	private:
		CKTDGUIDialogType			m_pDLGDungeonMap;
		CX2State*				m_pNowState;		
		CX2Dungeon*				m_pDungeon;

		map< std::pair<int, int>, SubStageNode> m_mapStageGraph;

		bool					m_bShow;

		D3DXVECTOR2				m_vPos;
	};


	struct DungeonEndingEvent
	{
		DungeonEndingEvent();
		~DungeonEndingEvent();

		void OnFrameMove( double fTime, float fElapsedTime );

		//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void SetDungeonClearTexture( CKTDGUIStatic* pStaticClearTexture_ );
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG

		void Start()
		{
			m_bStarted = true;
			m_fElapsedTime = 0.f;
			m_bEnd = false;
		}

		void StartChapter2()
		{
			m_bStartChapter2 = true;			
		}

	public:
		CKTDGUIDialogType m_pDLGSpeechBalloon;
		CKTDGUIDialogType m_pDLGCharacterPicture;

		float		m_fElapsedTime;
		bool		m_bStarted;
		bool		m_bEnd;

		bool		m_bEndChapter1;
		bool		m_bStartChapter2;
		float		m_fSpeechElapsedTime;

		int			m_iIndexSpeech1;
		int			m_iIndexSpeech2;


		float		m_fElapsedTime_ShowClearEffect;
		float		m_fElapsedTime_WinEffect;

		std::vector< wstring > m_vecSpeech1;
		std::vector< wstring > m_vecSpeech2;

	};



#ifdef HENIR_TEST

	struct SecretStageEnteringEvent
	{
		SecretStageEnteringEvent();
		~SecretStageEnteringEvent();

		void OnFrameMove( double fTime, float fElapsedTime );

		void Start()
		{
			SAFE_DELETE_DIALOG( m_pDLGSpeechBalloon );
			SAFE_DELETE_DIALOG( m_pDLGCharacterPicture );

			m_bStarted = true;
			m_fElapsedTime = 0.f;
			m_bEnd = false;
			m_bCreatedChallengeParticle = false;
		}

		void End()
		{
			m_fElapsedTime = 0.f;
			m_bStarted = false;
			m_bEnd = true;

			if( m_pDLGCharacterPicture != NULL )
				m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
			if( m_pDLGSpeechBalloon != NULL )
				m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
		}
		
	public:
		CKTDGUIDialogType m_pDLGSpeechBalloon;
		CKTDGUIDialogType m_pDLGCharacterPicture;

		float		m_fElapsedTime;
		bool		m_bStarted;
		bool		m_bEnd;

		float		m_fSpeechElapsedTime;
		int			m_iIndexSpeech1;

		float		m_fElapsedTime_ShowClearEffect;
        
		std::vector< wstring > m_vecSpeech1;
		CX2UnitManager::NPC_UNIT_ID m_eBossNPCID; 

		bool m_bCreatedChallengeParticle;
	};


#endif HENIR_TEST
	struct DRAWFACE_RHW_VERTEX
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};

#ifdef REFORM_ENTRY_POINT
	struct DUNGEON_LOADING_DATA
	{
		int iLoadingPercent;
		UidType uidUnitUID;

		DUNGEON_LOADING_DATA()
		{
			iLoadingPercent = 0;
			uidUnitUID = -1;
		}
	};
#endif //REFORM_ENTRY_POINT

public:
	CX2StateDungeonGame(void);
	~CX2StateDungeonGame(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual void SetShowStateDLG( bool bShow ) {};


	bool DungeonStageLoadReq( int stageNum );
	bool EndDungeonGameReq( CX2Room::TEAM_NUM winTeam );		// ������ ���������� ��ģ���


	void OpenExitDLG( const WCHAR* wszMsg );

	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

	DungeonMapUI* GetDungeonMapUI() const { return m_pDungeonMapUI; }

	CX2StateDungeonGame::DungeonEndingEvent* GetDungeonEndingEvent() { return &m_DungeonEndingEvent; }



#ifdef HENIR_TEST
	CX2StateDungeonGame::SecretStageEnteringEvent* GetSecretStageEnteringEvent() { return &m_SecretStageEnteringEvent; }
#endif HENIR_TEST

#ifdef OLD_COMBO_LIST_UI
	void CreateComboListForTutorial();
#endif OLD_COMBO_LIST_UI


	// 09.03.23 �¿� : public���� �̵�
	void ToggleDungeonMapUI();

#ifdef HENIR_TEST

	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ();
	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Send_EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT( bool bWantToEnterSecretStage );
	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#endif HENIR_TEST

#ifdef NEW_HENIR_TEST
	int GetStartSecretStageEnteringEvent(){ return m_iStartSecretStageEnteringEvent; } 
	void InitStartSecretStageEnteringEvent(){ m_iStartSecretStageEnteringEvent = 0; }
	bool SecretDungeonStageLoadReq(int iVal);
#endif NEW_HENIR_TEST

	bool Handler_EGS_DUNGEON_KILLALLNPC_CHECK_REQ();	
	bool Handler_EGS_DUNGEON_KILLALLNPC_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef SERV_EVENT_VALENTINE_DUNGEON
	void NotfiyCreateValentineCupCake();
	void Send_EGS_SYNC_DUNGEON_TIMER_NOT();
	bool Handler_EGS_SYNC_DUNGEON_TIMER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	void				AllocateFaultyPlayerWarningDlg ();							// DLG �Ҵ�
	void				DeallocateFaultyPlayerWarningDlg ();						// DLG ����
	void				ReservingWarningDlgDelete ();								// DLG ���� ����
#endif	// FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-09
	bool Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UPDATE_INVENTORY_SLOT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // ���볯¥: 2013-04-22
	bool Handler_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Sned_Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT( bool bAgree );
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

	/// ����� �� ���۱����� ��� �ð�
	float GetWaitDefenceDungeonStartTime() const { return m_fWaitDefenceDungeonStartTime; }
	void SetWaitDefenceDungeonStartTime(float val) { m_fWaitDefenceDungeonStartTime = val; }

	void WaitDefenceDungeonStart( const float fElapsedTime );		/// Ư�� �ð� ���� ����� �� ������Ʈ ������ ó���ϴ� �Լ�

#endif // SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES // ������
	bool Handler_EGS_DUNGEON_KILLALLNPC_CHECK_NOT();
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES

#ifdef PLAY_EMOTION_BY_USER_SELECT
	virtual void PlayEmotionByUserSelect();
#endif // PLAY_EMOTION_BY_USER_SELECT

protected:

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 09.03.30 �¿�
	virtual void SetShowCommonBG(){};

	//�� ���� ���� ����
	bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_GAME_LOADING_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_STATE_CHANGE_RESULT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_STATE_CHANGE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool GameLoadingReq( int percent );
	bool GameLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool GameLoadingNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GameLoadingAllUnitOkNot();

	bool PlayStartNot();

	bool BroadCastDungeonRoomSlotInfoNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_END_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual bool StateChangeFieldReq();
	virtual bool StateChangeLocalMapReq();
	bool StateChangeLocalMapAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool StateChangeResultReq();
	bool StateChangeResultAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool StateChangeResultNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool LoadNextStageOrSubStageReq( string nextStageID = "" );
	bool LoadNextStageOrSubStageAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool LoadNextStageOrSubStageNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool StageLoadingReq();
	bool StageLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool StageLoadingCompleteAllUnitNot();

	bool NPCUnitCreateAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool NPCUnitCreateNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool NPCUnitDieReq();
	bool NPCUnitDieAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool NPCUnitDieNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool DungeonStageLoadAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DungeonStageLoadNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool DungeonStageLoadCompleteReq( bool bIsSecretStage );
	bool DungeonStageLoadCompleteAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DungeonStageLoadCompleteNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool DungeonStageStartNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	

	bool Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
	bool Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif DUNGEON_TIME_OUT_FRAMEMOVE

	bool Handler_EGS_PLAY_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	bool Handler_EGS_USER_GAME_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	bool	Handler_EGS_BAD_ATTITUDE_USER_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
	bool Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_STAGE_CLEAR_IN_SERVER
#ifdef SERV_CATCH_HACKUSER_INFO
	bool Handler_EGS_CATCH_HACKUSER_INFO_NOT( KEGS_CATCH_HACKUSER_INFO_NOT& kEGS_CATCH_HACKUSER_INFO_NOT );
#endif SERV_CATCH_HACKUSER_INFO

	void LoadUI();
	void CreateGame();
	void GameLoadingStart();


	virtual bool QuitGame();


	// tutorial ����
	void ClearTutorialInputTreeParticle();
	HRESULT TutorialFrameMove( double fTime, float fElapsedTime );

	void TextOutSkillList( const char unitStateID );
	void CreateSkillSuccessEffect() { m_bCreateCommandSuccessSplash = true; }

	virtual bool ShortCutKeyProcess();

	virtual void ProcessShowMiniMap( const bool bHide_ );



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void					MoveToRubenVillage( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void					MoveToRubenVillage();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void					DrawMovingSmallBar();
	void					CreateMovingSmallBar();
	void					DestroyMovingSmallBar();
	void					DrawFace( const float fX_, const float fY_, 
									const CKTDGUIControl::UITextureData& texData_, 
									D3DCOLOR color_ /* = 0xffffffff */, const float fWidthPercent_ = 1.0f );

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	void ValentineDungeonTimer_OnFrameMove( float fElapsedTime_ );
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-09
	/// ����� �� ���� ó�� ����
	bool GetEnterDefenceDungeon() const { return m_bEnterDefenceDungeon; }
	void SetEnterDefenceDungeon(bool val) { m_bEnterDefenceDungeon = val; }
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef REFORM_ENTRY_POINT
	void SetLoadingGageBar( int iLoadingPercent );
#endif //REFORM_ENTRY_POINT

private:
	CKTDGUIDialogType					m_pDLGExitMsgBox;
	CKTDGUIDialogType					m_pDLGLoadingState;
	CKTDGUIDialogType					m_pDLGTutorialInputControl;

	CKTDGUIDialogType					m_pDLGGoToFirstDungeon;
	CKTDGUIDialogType					m_pDLGInputUI;
	CKTDGUIDialogType					m_pDLGTutorialExitButton;

	//CKTDGUIDialogType					m_pDLGF1Help;		
	CKTDGUIDialogType					m_pDLGF1Help2;

	CKTDGUIDialogType					m_pDLGArcadeFrame;




	CX2DungeonGame*					m_pDungeonGame;
	bool							m_bLoadingOK;
	CX2GameLoadingTip*				m_pCX2GameLoadingTip;

	wstring							m_HostIP;
	int								m_HostPort;


	bool							m_bSubStageLoading;
	int								m_SubStageLoadingRenderCount;

	bool							m_bResultStateReq;
	bool							m_bGameEnd;
	float							m_fGameEndWaitTime;



	CKTDXDeviceTexture*				m_pLoadingOutLineTex;
	CKTDXDeviceTexture*				m_pLoadingBasicTex;


	int								m_LoadingPercent;
	int								m_LoadingRenderCount;
	CX2Game::CONNECT_TYPE			m_ConnectType;

	int								m_NextStageNum;
	int								m_RenderCount;

#ifdef REFORM_ENTRY_POINT
	CKTDGUIDialogType				m_pDLGLoadingFront;
#else //REFORM_ENTRY_POINT
	vector< CKTDGUIDialogType >		m_DLGLoadingStateUnitInfoList;
#endif //REFORM_ENTRY_POINT

	// tutorial ����
	char							m_TutorialBeforeUserStateID;
	vector< CKTDGParticleSystem::CParticleEventSequenceHandle  >	m_vecTutorialInputTreeParticle;
	D3DXVECTOR3						m_vTutorialParticlePos;
	bool							m_bShowSkilList;
	CKTDGFontManager::CUKFont*		m_pFontForSkillList;


	char							m_HighLightSkillID; 
	float							m_fTimeLeftForHighlight;
	D3DXCOLOR						m_coHighlight;
	D3DXCOLOR						m_coHighLightIncrement;
	int								m_iTutorialMsgFlashCount;

	bool							m_bCreateCommandSuccessSplash;

	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hTutorialMissionSplashParticle;

	map< CX2Unit::UNIT_CLASS, map<char, wstring> > m_mapUnitCommandList;


	DungeonMapUI*					m_pDungeonMapUI;

	bool							m_bWasQuestUIMinimized;
	bool							m_bWasMiniMapMinimized;




	bool							m_bSend_KEGS_END_GAME_REQ;

	bool							m_bStateChangeLocalMap;

	bool							m_bSend_KEGS_LEAVE_ROOM_REQ;
	bool							m_bReceive_KEGS_LEAVE_ROOM_ACK;
	bool							m_bReceive_KEGS_UNIT_INFO_UPDATE;

	bool							m_bLeaveRoomAtTutorial;

	DungeonEndingEvent				m_DungeonEndingEvent;
#ifdef HENIR_TEST
	SecretStageEnteringEvent		m_SecretStageEnteringEvent;
#endif HENIR_TEST
#ifdef NEW_HENIR_TEST
	int								m_iStartSecretStageEnteringEvent;
#endif NEW_HENIR_TEST
	CKTDGUIDialogType					m_pDLGArcadeMission;
	float							m_fTimeShowArcadeMission;
	bool							m_bAutoShowOffArcadeMission;


#ifdef CREATE_MONSTER_LIST_TEST
	CreateMonsterListDialog	m_CreateMonsterListDialog;
#endif CREATE_MONSTER_LIST_TEST

#ifdef MONSTER_STATE_LIST_TEST
	MonsterStateListDialog m_MonsterStateListDialog;
#endif MONSTER_STATE_LIST_TEST

#ifndef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	BadAttitudeUserWarningDialog m_BadAttitudeUserWarningDialog;
#endif FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

	CKTDXCheckElapsedTime				m_TimerWaitingPortal;
	CKTDGUIControl::UITextureData		m_TexDataMovingGageBG;
	CKTDGUIControl::UITextureData		m_TexDataMovingGage;
	CKTDGStateManager::KStateID			m_RenderStateID;	/// �������̽� ����� ���� 2D ����Լ�

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	CKTDGUIDialogType						m_pDLGFaultyPlayerWarning;
	bool									m_bFaultyPlayerWarningDlgExitButtonPressed;	// DLG �� ���� �Ǿ�� �ϴ°�?
#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	CKTDGUIDialogType					m_pDLGValentineTimer;	/// �߷�Ÿ�� ���� Ÿ�̸� UI
	float								m_fValentineDungeonRemainTime;	/// �߷�Ÿ�� ���� Ÿ�̸� UI
	int									m_iValentineDungeonStage;		/// �߷�Ÿ�� ���� ��������
	bool								m_bIsValentineDungeon;
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hStageStartEffect1;		/// �������� ����� �߻��Ǵ� �ȳ� ���� ����Ʈ 1
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hStageStartEffect2;		/// �������� ����� �߻��Ǵ� �ȳ� ���� ����Ʈ 2
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-04-09
	bool								m_bEnterDefenceDungeon;				/// ����� �� ���� ó�� ����
	float								m_fWaitDefenceDungeonStartTime;		/// ����� �� ���۱����� ��� �ð�
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
	SHORT								m_SumDelta;		/// ���콺 �� ���尪
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef REFORM_ENTRY_POINT
	vector<DUNGEON_LOADING_DATA>		m_vecDungeonLoadingData; //���� �ε� ������. ���� �ε����°� �������� �ϳ��� ��ȭ�Ǹ� 
																 //�� �ۼ�Ʈ �������� ���� �ε��� ���� ������ �Ǿ��ؼ� �߰��� ����ü ����. (���� �۾� ����)
#endif //REFORM_ENTRY_POINT

};

