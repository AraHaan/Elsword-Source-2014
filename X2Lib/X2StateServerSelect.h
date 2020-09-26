#pragma once

class CX2StateServerSelect : public CX2State
{
public:
	friend class CX2StateAutoChanger;
	enum STATE_SERVER_SELECT_UI_CUSTOM_MSG
	{
		SUSUCM_UNIT_BUTTON_OVER = 0,
		SUSUCM_UNIT_BUTTON_UP,

		SUSUCM_BACK_TO_LOGIN,
		SUSUCM_CREATE_UNIT,
		SUSUCM_DELETE_UNIT,
		SUSUCM_SELECT_UNIT,

		SUSUCM_UNIT_ANIMATION_DOWN,
		SUSUCM_UNIT_ANIMATION_UP,
		SUSUCM_UNIT_ANIMATION_OVER,

		SUSUCM_DELETE_UNIT_CHECK,
		SUSUCM_TUTORIAL,

		SUSUCM_RENAME_OK,
		SUSUCM_RENAME_CANCEL,

		SUSUCM_RENAME_CONFIRM_OK,
		SUSUCM_RENAME_CONFIRM_CANCEL,

		SUSUCM_RENAME_SUCCESS_OK,

		SUSUCM_START_TUTORIAL_OK,
		SUSUCM_START_TUTORIAL_CANCEL,

		SUSUCM_START_FIRST_DUNGEON_OK,
		SUSUCM_START_FIRST_DUNGEON_CANCEL,

		SUSUCM_RECOMMEND_OK,
		SUSUCM_RECOMMEND_CANCEL,

		SUSUCM_PREV,
		SUSUCM_NEXT,

		SSSUCM_SERVER_CONNECT0,			// ä�� ����Ʈ ��ư Ŭ������ ��
		SSSUCM_SELECT_SERVER_GROUP,		// ������ ��ư�� Ŭ������ ��
		SSSUCM_SERVER_CONNECT2,				// fix!! remove

		SSSUCM_HACK_USER_PROCESS_CHECK_OK,
		SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL,

		SSSUCM_SERVER_SELECT_EXIT,
		SSSUCM_UNIT_SELECT_EXIT,
		SSSUCM_ATL_LIMIT_CHECK_OK,		// ������ // 2012-07-04 // ATL1.0 ������ ���� ���� ���� Ȯ��
		SSSUCM_EVENT_GUIDE_OK,			// ������ // 2012-08-14 // �̺�Ʈ ���̵� Ȯ��
//#ifdef SERV_RECOMMEND_LIST_EVENT
		SUSUCM_RECOMMEND_NEXT,			//������ ��õ�ϱ�
//#endif //SERV_RECOMMEND_LIST_EVENT
		
//#ifdef SHOW_ONLY_ADVERTISEMENT
		SSSUCM_HIDE_UI_MOUSE_OVER,
		SSSUCM_HIDE_UI_MOUSE_OUT,
//#endif //SHOW_ONLY_ADVERTISEMENT

// #ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		SSSUCM_JUMPING_CHARACTER_OK,	// ���� ĳ���� �ȳ� �˾�â OK ��ư ����
// #endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		SSSUCM_UNIT_VIEWER_UP,
		SSSUCM_UNIT_VIEWER_DOWN,
		SSSUCM_UNIT_VIEWER_OVER,
		SUSUCM_SELECT_CHANNEL_LIST_STEP,
		SUSUCM_SECURITY_PAD,
		SUSUCM_SELECT_UNIT_LIST_STEP,
		SUSUCM_PREV_CHANNEL_LIST,	
		SUSUCM_NEXT_CHANNEL_LIST,
		SUSUCM_CHANNEL_BUTTON_UP,
		SUSUCM_SECURITY_PAD_CREATE,
// #endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
// #ifdef FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
		SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK,
// #endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��


//#ifdef SERV_UNIT_WAIT_DELETE
		SUSUCM_FINAL_DELETE_UNIT = 48,
		SUSUCM_FINAL_DELETE_UNIT_CHECK = 49,
		SUSUCM_RESTORE_UNIT = 50,
		SUSUCM_RESTORE_UNIT_CHECK = 51,
//#endif SERV_UNIT_WAIT_DELETE

//#ifdef SIMPLE_BUG_FIX
		SSSUCM_DOUBLE_CONNECT = 52,
//#endif SIMPLE_BUG_FIX

//#ifdef NEW_OPTION_SECURITYPAD
		SSSUCM_SECURITYPAD	= 53,
//#endif //NEW_OPTION_SECURITYPAD

	};

#ifdef ADD_PLAY_SOUND //��â��
	enum SOUND_PLAY_TYPE
	{
		SPT_UNIT_SELECT = 0,
		SPT_SERVER_SELECT,
	};
#endif //ADD_PLAY_SOUND

	struct UnitButton
	{
		int				unitIndex;
		CKTDGUIButton*	pButton;

		UnitButton()
		{
			unitIndex	= 0;
			pButton		= NULL;
		}
	};
	


	//struct LessChannelIndex
	//{
	//	bool operator()( const KChannelInfo& lhs, const KChannelInfo& rhs )
	//	{
	//		if( lhs.m_iChannelID < rhs.m_iChannelID )
	//			return true;

	//		return false;
	//	}
	//};

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	struct UnitInfoAddData
	{
		double					m_dwLastLoginTime;
		SEnum::SERVER_GROUP_ID  m_eServerGroupID;


#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, ���� ���� ����, 8�� ���� ���� 
		// ���� ���� �� �����ϴ� Key ���� ������ �α��� �ð��� �� ������ ���ŵʿ� ���� ����� ���� ����
		// m_bIsCreatedUnit ������ True �� ��� �α��� �ð����� ��� ���� ������ ������ �����Ѵ�.
		bool					m_bIsCreatedUnit;
		UnitInfoAddData( double dwLastLogin, SEnum::SERVER_GROUP_ID eServerGroupID, bool bIsCreatedUnit )
		{
			m_dwLastLoginTime	= dwLastLogin;
			m_eServerGroupID	= eServerGroupID;
			m_bIsCreatedUnit	= bIsCreatedUnit;
		}
		bool operator > ( const UnitInfoAddData & rhs) const
		{
			if ( true == m_bIsCreatedUnit && true == rhs.m_bIsCreatedUnit)
				return m_dwLastLoginTime > rhs.m_dwLastLoginTime;

			else if ( true == m_bIsCreatedUnit )	
				return true;
			
			else if ( true == rhs.m_bIsCreatedUnit )
				return false;

			return m_dwLastLoginTime > rhs.m_dwLastLoginTime;
			
		}

#else  // FIX_REFORM_ENTRY_POINT_8TH				
		UnitInfoAddData( double dwLastLogin, SEnum::SERVER_GROUP_ID eServerGroupID )
		{
			m_dwLastLoginTime	= dwLastLogin;
			m_eServerGroupID	= eServerGroupID;
		}
		bool operator > ( const UnitInfoAddData & rhs) const
		{
			return m_dwLastLoginTime > rhs.m_dwLastLoginTime;
		}
#endif // FIX_REFORM_ENTRY_POINT_8TH				
	};
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

public:
	CX2StateServerSelect(void);
	virtual ~CX2StateServerSelect(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	void OnFrameMove_GameServerConnect( double fTime, float fElapsedTime );



	bool EnterTutorial();

	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();

	bool Handler_EGS_STATE_CHANGE_FIELD_REQ(bool bForceMove = false);
	bool Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//bool Handler_EGS_NEW_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void ClearUnitViewerList();
	CX2UnitViewerUI* GetUnitViewer( CX2Unit* pUnit );

#ifdef SERV_SECOND_SECURITY
	void SetEnableUnitSelect(bool _bVal){ m_bEnableUnitSelectBySecondSecurity = _bVal; }
#endif SERV_SECOND_SECURITY

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	bool Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
	bool Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE

#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
	bool Handler_EGS_SELECT_UNIT_REQ( UidType unitUID = -1, int iUnitLevel = -1 );
#endif  // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )

protected:
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void CreateUnitButton();

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	bool ClearUnitButton( bool bIsClearUnitViewer_ = true );
	void CreateUnitButtonNew();
	void ChangeUnitButtonInfo ();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	bool ClearUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

	void ClearBeforeUnit( CX2Unit* pUnit );
	void UnitButtonUp( CX2Unit* pUnit );

	bool IsChannelSelectMode();

#ifndef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
	bool Handler_EGS_SELECT_UNIT_REQ( UidType unitUID, int iUnitLevel );
#endif // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )

#ifdef SERV_SELECT_UNIT_PACKET_DIVISION		/// ��Ŷ�� ũ�Ⱑ �ִ� ��� ũ�⸦ �Ѱܹ�����, ����
	bool Handler_EGS_SELECT_UNIT_1_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ������ ����, �κ��丮
	bool Handler_EGS_SELECT_UNIT_2_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ����Ʈ
	bool Handler_EGS_SELECT_UNIT_3_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// Ÿ��Ʋ, ��ų, ��
	bool Handler_EGS_SELECT_UNIT_4_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ���� �̺�Ʈ, ����, ����, ��ŷ
	bool Handler_EGS_SELECT_UNIT_5_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ETC, 1ȸ�� �̺�Ʈ
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	bool Handler_EGS_SELECT_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
	bool Handler_EGS_SELECT_UNIT_INVENTORY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_GET_MY_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

	bool Handler_EGS_GET_MY_MESSENGER_SN_REQ( UidType unitUID );
	bool Handler_EGS_GET_MY_MESSENGER_SN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DELETE_UNIT_REQ();
	bool Handler_EGS_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_UNIT_WAIT_DELETE
	bool Handler_EGS_FINAL_DELETE_UNIT_REQ();
	bool Handler_EGS_FINAL_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_RESTORE_UNIT_REQ();
	bool Handler_EGS_RESTORE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_UNIT_WAIT_DELETE
	
#ifdef SERVER_GROUP_UI_ADVANCED
	bool Handler_ECH_GET_SERVERGROUP_LIST_REQ();
	bool Handler_ECH_GET_SERVERGROUP_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_ECH_GET_SERVERGROUP_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void UpdateServerGroupListUI();
	void CreateServerGroupButton();
	void ClearServerGroupButton();
#endif SERVER_GROUP_UI_ADVANCED	

	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	


	// �̸� �ٲٱ�
	bool Handler_EGS_CHANGE_NICK_NAME_REQ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly );
	bool Handler_EGS_CHANGE_NICK_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void OpenRenameMsgBox( bool bOpen );
	void SetRenameMsgBoxError( const wstring& wstrErrorMessage );


	bool Handler_EGS_RECOMMEND_USER_REQ( const WCHAR* wszNickName, bool bCancel = false );
	bool Handler_EGS_RECOMMEND_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OpenRecommendMsgBox( bool bOpen );

	//bool Handler_EGS_GET_POST_LETTER_LIST_REQ();
	//bool Handler_EGS_GET_POST_LETTER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );




	bool Handler_EGS_SELECT_SERVER_SET_REQ();
	bool Handler_EGS_SELECT_SERVER_SET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ();
	bool Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_AGREE_HACK_USER_REQ();
	bool Handler_EGS_AGREE_HACK_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
	bool Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );




	//{{ dmlee 2009.3.5 statelogin�� �ִ� �� �̵�
	bool Handler_EGS_CONNECT_REQ( const wstring& wstrGameServerIP, const int iGameServerPort );
	bool Handler_EGS_CONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_VERIFY_ACCOUNT_REQ();
	bool Handler_EGS_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_ENX_USER_LOGIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_KXPT_PORT_CHECK_REQ();
	bool Handler_KXPT_PORT_CHECK_ACK( const KXPT_PORT_CHECK_ACK& kXPT_PORT_CHECK_ACK );

	virtual bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
	virtual bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_GET_SERVER_SET_DATA_REQ();
	//bool Handler_EGS_GET_SERVER_SET_DATA_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CURRENT_TIME_REQ();
	bool Handler_EGS_CURRENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool LoginErrorCheck();
	//bool LoginNexonKorea();

	//}} dmlee 2009.3.5 statelogin�� �ִ� �� �̵�



	bool Handler_ECH_GET_CHANNEL_LIST_REQ();
	bool Handler_ECH_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_ECH_GET_CHANNEL_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
	bool Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_ECH_DISCONNECT_REQ();
	bool Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_ECH_VERIFY_ACCOUNT_REQ();
	bool Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_SECOND_SECURITY
	bool Handler_EGS_SECOND_SECURITY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SECOND_SECURITY

	bool Handler_EGS_KEYBOARD_MAPPING_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SettingUserKeyboardMappingInfo( const KKeyboardMappingInfo& kKeyboardMappingInfo_ );
#ifdef SERV_KEY_MAPPING_INT
	void SettingUserGamePadMappingInfo( const KKeyboardMappingInfo& kKeyboardMappingInfo_ );
#endif //SERV_KEY_MAPPING_INT

	bool Handler_EGS_CHAT_OPTION_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	bool Handler_EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif //SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
	bool Handler_EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	bool Handler_EGS_JUMPING_CHARACTER_GUIDE_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ���� ĳ���� �˾�â ����� �������� �ִ� not
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����


	void UpdateChannelListUI();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	bool ConnectToChannelServer( bool bIsReversingConnect = false );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	bool ConnectToChannelServer();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	
	void UnitSelectExit();

#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28
	void AddMeshAdvertisement_Back();
	void AddMeshAdvertisement_Front();
#endif // MODIFY_ADVERTISEMENT


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void HideAllUnitViewerUI ();
	void CreateServerSelectUnitViewerUI ( CX2Unit * pUnit_ );
	void SetFocusUnitButtonByIndex ( const int iIndex_ );
	void SetRightSideFlagColor ( D3DXCOLOR colorFlag );
	void SetShowServerStateButtonSet ( bool bIsShowUnitListState, bool bIsShowChannelListState ); 
	void SetBackgroundImageBySelectUnitType ( CX2Unit::UNIT_TYPE eUnitType_ );
	void SetShowSelectUnitInfo( CX2Unit * pUnit );
	void CreateChannelButton( bool bSelectTopChannelButton = false );
	bool Handler_EGS_CHARACTER_LIST_REQ ();
	bool Handler_EGS_CHARACTER_LIST_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ ();
	bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
	bool Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly, SEnum::SERVER_GROUP_ID eGroupID );

#ifdef ADD_PLAY_SOUND //��â��
	void PlaySoundSelect( CX2Unit::UNIT_TYPE eUnitType, SOUND_PLAY_TYPE eSoundPlayType );
#endif //ADD_PLAY_SOUND

	#ifdef FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG
	void InitChannelInfo();
	#endif // FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
#ifdef FIX_REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	void FindAndConnectLowerUserCountGameServer ( const vector< KChannelInfo > & vecChannelInfo );
#endif // FIX_REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
	int FindAndConnectMatchingLevelGameServer ( int iMatchingUnitLevel, const map<int, KChannelBonusInfo > & mapChannelBonusInfo, const vector< KChannelInfo > & vecChannelInfo );
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����

protected:

	void UnitHandling( double fTime, float fElapsedTime );

	void DownAnimFrameMove( double fTime, float fElapsedTime );
	void PlayDownAnim();

#ifdef SHOW_ONLY_ADVERTISEMENT
	void SetShowServerSelectUI( bool bVal_ );
#endif // SHOW_ONLY_ADVERTISEMENT


	virtual bool LastMsgByESC();
	virtual void LastMsgPopOk();

public:
	//{{ kimhc // 2009-12-15 // ������ �߰�
#ifdef	ADD_SERVER_GROUP
	bool OpenScriptServerGroupFile();			// ������ �÷��� �ߴ� ������ �б�
#ifdef SERVER_GROUP_UI_ADVANCED
	void UpdateServerSelectButton( int iID );
#else
	void UpdateServerSelectButton( SERVER_GROUP_ID eID );
#endif SERVER_GROUP_UI_ADVANCED
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // ������ �߰�

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	void ClearChannelButton();
	void ChangeSelectUnitStandMeshInstance ( CX2Unit::UNIT_TYPE );
	void MoveToCharacterSelectUI();
	bool ChannelButtonUp( CKTDGUIButton * pButton );
	#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
	void ConnectToLowerUserCountGameServer ( bool bIsConnectAnotherServerGroup = false );
	#else  // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
	void ConnectToLowerUserCountGameServer ();
	#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	
#ifdef FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
	void ProcessIntroMovieEnd ();		// ��Ʈ�� ���� ( �ֵ� �߰� ������ �� ) �� ������ ���� ó��
#endif // FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��

#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, ���� ���� ����, 8�� ���� ���� 
										// ( ���� ���� �� �����ϴ� Key ���� ������ �α��� �ð��� �� ������ ���ŵʿ� ���� ����� ���� ���� )
	UidType GetUidCreatedUnit() const		{ return m_uidCreatedUnit; }
	void	SetUidCreatedUnit(UidType val)	{ m_uidCreatedUnit = val; }
#endif // FIX_REFORM_ENTRY_POINT_8TH				


public:
	static bool				m_sbSelectedServerSet;				// �� ������ �������� ��������ȭ������, ĳ���� ����ȭ������ ������ �� �ִ�.
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	static bool				m_bCanNotConenctToBusyServer;				// �� ������ �������� ��������ȭ������, ĳ���� ����ȭ������ ������ �� �ִ�.
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	
protected:
	CKTDGUIDialogType			m_pDLGServerSelectBack;

	//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
	CKTDGUIDialogType			m_pDLGChannelSelectBack;
	int							m_iChannelNum;				// ä�� ����
	//{{ ����� : [2011/6/20] //        ä�� ���� ������� ����
	bool                                               m_bSelectChannel;
	//}} ����� : [2011/6/20] //        ä�� ���� ������� ����
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�

	CKTDGUIDialogType			m_pDLGCheckHack;

	vector<CKTDGUIDialogType>	m_vecUnitSlot;

	CKTDGUIDialogType			m_pDLGUnitSelectBack;
	CKTDGUIDialogType			m_pDLGUnitSelectFront;

	CKTDGUIDialogType			m_pDLGDeleteUnitCheck;
	CKTDGUIDialogType			m_pDLGLoadingState;

	CKTDGUIDialogType			m_pDLGRenameMsgBox;
	CKTDGUIDialogType			m_pDLGRenameConfirmMsgBox;
	CKTDGUIDialogType			m_pDLGRenameSuccessMsgBox;

	CKTDGUIDialogType			m_pDLGTutorialMsgBox;
	CKTDGUIDialogType			m_pDLGGoToFirstDungeon;

	CKTDGUIDialogType			m_pDLGRecommend;
	CKTDGUIDialogType			m_pDLGRecommendSuccess;

#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
	CKTDGUIDialogType			m_pDLGATLLimitCheckMsgBox;
#endif //SERV_CHANGE_NEXON_AUTH_ATL_LEVEL

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	CKTDGUIDialogType			m_pDLGEventGuide;
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE


	vector<UnitButton*>		m_UnitButtonList;
	vector<CX2UnitViewerUI*> m_UnitViewerUIList;

	CX2Unit*				m_pSelectUnit;
	CKTDGUIButton*			m_pSelectButton;

	bool					m_bUnitClicked;
	float					m_fMouseSensitivity;

	float					m_fRemainRotY;
	bool					m_bRotateReturn;
	float					m_fRotSpeed;

	CKTDGUIButton*			m_pOveredButton;
	CKTDGUIStatic*			m_pOveredName;
	
	bool					m_bOneTimeInit;
	bool					m_bStateChangingToUnitCreate;

	bool					m_bPlayDownAnim;
	float					m_fDownAnimTime;

	int						m_MaxUnitNum;

	bool					m_bReserveEnterTutorial;

	bool					m_bCheckRecommend;
	bool					m_bCheckRecommendSuccess;

	int						m_NowPage;
	int						m_MaxPage;





	bool			m_bPortCheckWait;
	float			m_fPortCheckWaitTime;
	int				m_PortCheckRetryTime;

	bool			m_bENX_USER_LOGIN_NOT;
	bool			m_bEGS_VERIFY_ACCOUNT_ACK;
	bool			m_bIsGameBang;
	bool			m_bPortCheckReq;

	bool			m_bEGS_VERIFY_ACCOUNT_REQ;
	float			m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime;

#ifdef SERVER_GROUP_UI_ADVANCED
	CKTDGUIDialogType			m_pDLGServerGroupSelectBack;
	bool			m_bReceivedServerGroupList;
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_UNIT_WAIT_DELETE //2012.02.28 lygan_������ // ��ư�� unitUID�� �����Ͽ� ĳ���� ���� �Ҷ� ����� �뵵
	UidType			m_iSelectUnitUID;
	bool			m_bMaintainNowUnitButton;								// ���� ��û �� ������ ù ��° ĳ���� �������� ��Ŀ�� ��� ���� ���� �뵵
#endif SERV_UNIT_WAIT_DELETE

	bool			m_bReceivedChannelList;

	bool			m_bSentEGS_CONNECT_REQ;
	bool			m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK;		

	bool			m_bReservedConnectionToChannelServer;					// �ٸ� ä�� ������ ������ ���� �Ŀ� ���ο� ä�� ������ ��������

	bool			m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK;

#ifndef CHANGE_CHANNEL
	std::vector< KChannelInfo > m_vecChannelInfo;
#endif
	int				m_iTryConnectChannelID;				// ���� ���ӽõ����� ä�� ID



	bool			m_bJustChangedNickName;			// �г����� �ٲ��� EGS_SELECT_UNIT_ACK�� ������ �ؽ� �޽����ʿ� �г����� �ٲ���ٴ� �̺�Ʈ�� ������



	UidType			m_NickNameChangeUnitUID;		// �г��� ������ ���� UID
	wstring			m_NickNameChangeUnitNickName;	// �г��� ������ ������ ���ο� �г���


	bool			m_bRemoveUnitAfterReceiveNMSerialNum;	// ĳ���� ������ �ؽ� �޽��� �ø��� �ѹ��� ���� �Ŀ� ĳ���� ���� �õ�	
#ifdef SERV_SECOND_SECURITY
	bool			m_bEnableUnitSelectBySecondSecurity;
#endif SERV_SECOND_SECURITY

	bool			m_bRecivedKeyMap;
	
#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshAddAdvertisement_Back;
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshAddAdvertisement_Front;
#endif // MODIFY_ADVERTISEMENT

#ifdef SHOW_ONLY_ADVERTISEMENT
	CKTDGUIDialogType			m_pDLGShowOnlyAdvertise;
	bool						m_bHideServerSelectUI;
#endif // SHOW_ONLY_ADVERTISEMENT

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	CKTDGUIDialogType			m_pDLGInfoCreateJumpingChar;		// ���� ĳ���� DLG
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	// ������, �ִ� ���� ����, ���� ���� �ִ� ����

	// ���� ������ �ڵ� �����ؾ� �� �� EGS_DISCONNECT_FOR_SERVER_SELECT_ACK ��Ŷ�� ���´ٴ� �÷���
	// �⺻ �� false �̸� ���� ���� ������ ���� �ٸ� ���� ������ ���� �� true �� ����
#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
	static bool						m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection;
#else  // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
	bool						m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection;
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����
		
	// ���� ������ �ڵ� �����ؾ� �� �� EGS_SELECT_UNIT_REQ ��Ŷ�� ���´ٴ� �÷���
	// �⺻ �� false �̸� �ڵ� ���� �� �������� ������ �� true �� ����
	bool						m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection;						// 


	// ���� ���, ���� ���� �������� �ϳ��� ����
	CX2UnitViewerUI*			m_pUnitViewerUI;	

	// ä�� ������ �߰��� ���� ���� ������, �ִ� ������ �߰�
	int							m_iChannelNowPage;
	int							m_iChannelMaxPage;
	
	// EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK �� �޾��� ��, ���� ������ ������ �����Ѵ�.
	// �ش� ���ڰ� �Ϸ� ���� ������ ���ڸ� �Ѿ�ٸ� �� �̻� ���� �� ���� ( ĳ���� ���� �Ұ� )
	int							m_iCreateUnitTodayCount;		
	
	vector<CKTDGUIDialogType>	m_vecChannelSlot;			 // ä�� ���� DLG
	CKTDGUIDialogType			m_pDLGShowBeforeConnect;	//  ���� â�� �����ִ� DLG
	std::vector<CKTDGXMeshPlayer::CXMeshInstanceHandle>		m_vecStandMeshInst;	// ĳ���� �� ���ĵ�
	
	CX2EffectSet::Handle		m_hStandEffect;								// ĳ���� ����Ʈ
	float						m_fFadeTitleControlValue;					// ��� �˸��� Fade ����, 0 ~ 2 �� Fade �Ǹ� 0 ~ 1 �� value = ����, 1 ~ 2 �� 2 - value = ���� 
	int							m_iReservedEntryPointServerChannelIndex;	// ���� �������� ������ ������ ��, �ٽ� �����ؾ� �� ���� ������ Index
	vector< KChannelInfo >		m_vecGaiaChannelInfo;						// ���̾� ������ ä�� ����
	vector< KChannelInfo >		m_vecSolesChannelInfo;						// �ַ��� ������ ä�� ����
	map< int, KChannelBonusInfo > m_mapGaiaChannelBonusInfo;				// ���̾� ������ ���ʽ� ä�� ���� ( ���� ���� ��.. )
	map< int, KChannelBonusInfo > m_mapSolesChannelBonusInfo;				// �ַ��� ������ ���ʽ� ä�� ���� ( ���� ���� ��.. )
	CKTDGUIButton*				m_pSelectChannelButton;						// ���� ������ ä�� ��ư
	bool						m_bIsAbledStateCreateUnitButton;			// ���� ĳ���� ���� ��ư�� Ŭ���� �� �ִ� �����ΰ�?
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef ADD_PLAY_SOUND //��â��
	CKTDXDeviceSound*			m_pSound;
#endif //ADD_PLAY_SOUND

#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
	CKTDGUIDialogType			m_pDLGReConnectChannelServer;				// ���� â�� �����ִ� DLG
	bool						m_bReservedSecurityPad;						// ���� �е�â�� ������ ���� ����� �� �ֵ��� �Ѵ�.
	bool						m_bReservedHackingUserAgreeDlg;				// ��ŷ ���� �ȳ�â�� ������ ���� ����� �� �ֵ��� �Ѵ�.
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	bool						m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp;			// ä�� ��ư�� ������ ���� ������ ������ �õ� �� ��, ���� ������ ���� ������ ��� ���� ó���� �����Ѵ�.
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
private:
	void _SendSystemInfo();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef PLAY_PROMOTION_MOVIE //JHKang
	bool	m_bOrgSound;	/// �⺻ ȿ���� ���� �ӽ� ����
	bool	m_bOrgMusic;	/// �⺻ ����� ���� �ӽ� ����
#endif //PLAY_PROMOTION_MOVIE

#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, ���� ���� ����, 8�� ���� ���� 
										// ( ���� ���� �� �����ϴ� Key ���� ������ �α��� �ð��� �� ������ ���ŵʿ� ���� ����� ���� ���� )
	UidType	m_uidCreatedUnit;			// X2StateCreateUnit ���� ������ ����
#endif // FIX_REFORM_ENTRY_POINT_8TH				

#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
	float				m_fElapsedTime_WaitingCharacterList;		// ĳ���� ������ �޾ƿ��´�� ���� �ð� ( m_pDLGWaitingChannelList �� ���� ��, �ð� ���� )
	CKTDGUIDialogType	m_pDLGWaitingCharacterList;					// ĳ���� ������ �޾ƿ��� �� �Դϴ�.
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����

#ifdef CLIENT_PORT_CHANGE_REQUEST
	int				m_iUDPRequestCount;
#endif //CLIENT_PORT_CHANGE_REQUEST
};
