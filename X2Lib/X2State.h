#pragma once
#include "KTDXStage.h"
#include "X2Cursor.h"

#ifdef CLIENT_PORT_CHANGE_REQUEST
#include <boost/timer.hpp>
#endif //CLIENT_PORT_CHANGE_REQUEST

class CX2StateAutoChanger;
class CX2State : public CKTDXStage
{
	public:
		enum STATE_UI_CUSTOM_MSG
		{
			SUCM_EXIT = 3000,
			SUCM_GAME_EDIT_COMMAND,
			SUCM_EXIT_WINDOW,

			SUCM_LAST_MSG_BY_ESC_OK,
			SUCM_LAST_MSG_BY_ESC_CANCEL,

			SUCM_EVENT_CLOCK_MOUSE_OVER,
			SUCM_EVENT_CLOCK_MOUSE_OUT,

			SUCM_GUEST_USER_GOTO_UNIT_SELECT,

			SUCM_ESC_PUSH,	//�̰� �ٲܷ��� DXUT�ʿ� SC_CLOSE �ʵ� ���� �ٲ������.
			SUCM_USER_INFO_OK,			

			//{{ kimhc // 2010-03-26 // ���� ��ų ���� ����	
			SUCM_UNREADY_OK,		// ������°� Ǯ���� ��ų ������ �����
			SUCM_UNREADY_CANCEL,	// ���� ���°� ��Ǯ���� ��ų ������ ���õ�
			//}} kimhc // 2010-03-26 // ���� ��ų ���� ����

			SUCM_PC_BANG_GUIDE_OK,	
			
			SUCM_RESURRECT_OK,
			SUCM_RESURRECT_CANCEL,
			SUCM_RESURRECT_USE_CASH_OK,
			SUCM_RESURRECT_USE_CASH_CANCEL,

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
			SUCM_AGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER,		/// ������ ��ȯ ��û�� �¶�
			SUCM_DISAGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER,		/// ������ ��ȯ ��û�� �ź�
			SUCM_AGREE_SUMMON_ROOM_BY_MARRIEGED_USER,			/// �ʵ�� ��ȯ ��û�� �¶�
			SUCM_DISAGREE_SUMMON_ROOM_BY_MARRIEGED_USER,		/// �ʵ�� ��ȯ ��û�� �ź�
#endif ADDED_RELATIONSHIP_SYSTEM

//#ifdef SERV_ADVERTISEMENT_EVENT
			SUCM_CLOSE_ADVERTISEMENT = 3021, //�ؿ��� �߰��� ���� UI�� ����ϴ� �ݱ��ư 2013.07.03 ��â��
//#endif SERV_ADVERTISEMENT_EVENT

			SUCM_STATE_CHANGE_POPUP_OK,

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
			// ���� ĳ���� �˸� DLG �ݱ�
			SUCM_JUMPING_CHARACTER_DLG_CLOSE,		
		
			// ���� ĳ���� ���� DLG ����
			SUCM_JUMPING_CHARACTER_CLASS_CHANGE_DLG_POPUP,

			// ���� ĳ���� ���� DLG �ݱ�
			SUCM_JUMPING_CHARACTER_CLASS_CHANGE_DLG_CLOSE,

			// ���� ĳ���� ����!
			SUCM_JUMPING_CHARACTER_CLASS_CHANGE_OK,

// 			// ���� ĳ���� ���� ����
// 			SUCM_JUMPING_CHARACTER_CLASS_FORCE_FOCUS_ON,
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
			SUCM_EVENT_CHARACTER_QUEST_RANKING_CLOSE = 3028,
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
			SUCM_EVENT_NOTE_VIEW_OK = 3029,
			SUCM_EVENT_NOTE_VIEW_CANCLE = 3030,
#endif SERV_ELESIS_UPDATE_EVENT

//#if defined( SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL ) || defined( ACCOUNT_BLOCK_MESSAGE_RENEWAL_KR )
			SUCM_BLOCK_ACCOUNT_CONNECT = 3031,
//#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL // ACCOUNT_BLOCK_MESSAGE_RENEWAL_KR
		};

	
		// �ʺ��� ���̵�뵵�� ��ư Ŭ���� �����ϴ� ��� ȭ��ǥ
		struct MouseClickUI 
		{
			enum MOUSE_CLICK_STYLE
			{
				MCS_FROM_RIGHT_BOTTOM,
				MCS_FROM_LEFT_BOTTOM,
				MCS_FROM_LEFT_TOP,
				MCS_FROM_RIGHT_TOP,
//#ifdef SERV_NEW_DEFENCE_DUNGEON
				MCS_FROM_RIGHT_TOP_NORMAL, // Ŭ�� �ѱ� ������ ����
//#endif // SERV_NEW_DEFENCE_DUNGEON

			};

			enum HIDE_REASON
			{
				HR_COUPON_BOX,
				HR_CHARACTER_ROOM,
				HR_CASH_SHOP,
			};

		public:
			MouseClickUI( CKTDXStage* pStage, D3DXVECTOR2 vPos, float fTime, int nCount, MOUSE_CLICK_STYLE eStyle );
			~MouseClickUI()
			{
				SAFE_DELETE_DIALOG( m_pDLGMouseClick );
			}

			void SetShow( bool bShow )
			{
				if( m_pDLGMouseClick != NULL )
				{					
					m_pDLGMouseClick->SetShow( bShow );					
				}
			}

			bool GetShow()
			{
				if( m_pDLGMouseClick == NULL )
					return false;

				return m_pDLGMouseClick->GetShow();
			}

			void MoveDialogToTop()
			{
				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGMouseClick, true );
			}


		private:
			CKTDGUIDialogType	m_pDLGMouseClick;
		};

#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

		struct FadeInOutNotice		/// ����� �� ó�� ���̵� �� ~ ���̵� �ƿ� �Ǵ� ���� ��¿� ���� ���� ����ü
		{
		public:
			FadeInOutNotice( const WCHAR* pFadeInOutNoticeText_, float fFadeInOutNoticeTime_ )
#ifdef COPY_STRING_FADE_IN_OUT_NOTICE		//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����
				: m_wstrFadeInOutNoticeText ( pFadeInOutNoticeText_ )
#else // COPY_STRING_FADE_IN_OUT_NOTICE		//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����
				:m_pFadeInOutNoticeText( pFadeInOutNoticeText_ )
#endif // COPY_STRING_FADE_IN_OUT_NOTICE	//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����
				,m_fFadeInOutNoticeTime( fFadeInOutNoticeTime_ )
				,m_fFadeInOutNoticeMaxTime( fFadeInOutNoticeTime_ )
				,m_vOffsetPos(D3DXVECTOR2(0.f,0.f))
			{
				if( 5.f > fFadeInOutNoticeTime_ )	/// �ϴ� �ּ� ��� �ð��� 5�ʷ� ����
				{
					fFadeInOutNoticeTime_		= 5.f;
					m_fFadeInOutNoticeMaxTime	= fFadeInOutNoticeTime_;
				}
			}
#ifdef COPY_STRING_FADE_IN_OUT_NOTICE		//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����		
			const WCHAR*				GetFadeInOutNoticeText() { return m_wstrFadeInOutNoticeText.c_str(); }			
#else // COPY_STRING_FADE_IN_OUT_NOTICE		//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����	
			const WCHAR*				GetFadeInOutNoticeText() { return m_pFadeInOutNoticeText; }
#endif // COPY_STRING_FADE_IN_OUT_NOTICE	//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����
			float						GetFadeInOutNoticeMaxTime() { return m_fFadeInOutNoticeMaxTime; }
			float						GetFadeInOutNoticeTime() { return m_fFadeInOutNoticeTime; }
			void						SetFadeInOutNoticeTime( float fFadeInOutNoticeTime_ ) { m_fFadeInOutNoticeTime = fFadeInOutNoticeTime_; }

			const D3DXVECTOR2&			GetOffsetPos(){ return m_vOffsetPos;}
			void						SetOffsetPosX( float fOffsetX_ ){ m_vOffsetPos.x = fOffsetX_; }
			void						SetOffsetPosY( float fOffsetY_ ){ m_vOffsetPos.y = fOffsetY_; }
		private:
			float						m_fFadeInOutNoticeTime;		/// ��µ� �ؽ�Ʈ ǥ�� �ð�
			float						m_fFadeInOutNoticeMaxTime;	/// ��µ� �ؽ�Ʈ �ִ� ǥ�� �ð�
#ifdef COPY_STRING_FADE_IN_OUT_NOTICE		//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����	
			wstring						m_wstrFadeInOutNoticeText;
#else // COPY_STRING_FADE_IN_OUT_NOTICE		//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����
			const WCHAR*				m_pFadeInOutNoticeText;		/// ����� �� ���̵� ����� ȭ�鿡 ��µ� �ؽ�Ʈ
#endif // COPY_STRING_FADE_IN_OUT_NOTICE	//// kimjh: Fade-In, Out �Ǵ� ���� ��¿� ����ü ����, �ּҿ��� �����ؼ� ���� �ֵ��� ����

			D3DXVECTOR2					m_vOffsetPos;

		};

#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

	public:
		CX2State(void);
		virtual ~CX2State(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

#ifdef BANDICAM_HIDE_RECORDING_PARTICLE
		void ShowBandicamRecParticle( bool bShow );
#endif BANDICAM_HIDE_RECORDING_PARTICLE

		virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

        virtual void	SetLuaFrameMoveFunc_LUA( const char* pFuncName )  { m_LuaFrameMoveFuncName = ( pFuncName ) ? pFuncName : ""; }



		void ResetUnitClassInFieldSquare( UidType unitUID, CX2Unit::UNIT_CLASS eUnitClass );
		void ResetUnitViewerInFieldSquare( UidType unitUID, CX2Unit::UNIT_CLASS eUnitClass );
		//{{ ����� : [2009/10/9] //	��� �̸� ������Ʈ
#ifdef GUILD_MANAGEMENT
		void ResetUnitGuildNameInFieldSquare( UidType unitUID, wstring wstrGuildName );
		
#endif	//	GUILD_MANAGEMENT
		//}} ����� : [2009/10/9] //

		CX2Cursor* GetCursor() { return m_pCursor; }

		// 09.03.23 ���¿� : �̺�Ʈ Ÿ�̸� ��ġ �̵�
		// void RenderMarketingEventTimer();

		bool GetPersonalTrade() { return m_bPersonalTrade; }
		void SetPersonalTrade(bool val) { m_bPersonalTrade = val; }		




		void CreateMouseClickUI( CKTDXStage* pStage, D3DXVECTOR2 vPos, float fTime = 0.5f, int nCount = 1000,
			MouseClickUI::MOUSE_CLICK_STYLE eStyle = MouseClickUI::MCS_FROM_RIGHT_BOTTOM )
		{
			DeleteMouseClickUI();
			m_pMouseClickUI = new MouseClickUI( pStage, vPos, fTime, nCount, eStyle );
		}
		void DeleteMouseClickUI()
		{
			SAFE_DELETE( m_pMouseClickUI );
		}
		bool GetShowMouseClickUI() 
		{ 
			if(m_pMouseClickUI != NULL)
				return m_pMouseClickUI->GetShow(); 
			return false;
		}
		void SetShowMouseClickUI( bool bShow, MouseClickUI::HIDE_REASON eHideReason );
		void MoveMouseClickUIToTop()
		{
			if( NULL != m_pMouseClickUI )
				m_pMouseClickUI->MoveDialogToTop();
		}


		//////////////////////////////////////////////////////////////////////////
		virtual bool	Handler_EGS_NOTIFY_MSG_REQ_LUA( int noticeCount, const char* pMsg, bool bIsAllServers = false );
		virtual bool	Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ_LUA( const char* pUnitNickName, int changeLevel, int iChangeEXP = 0 );
		virtual bool	Handler_EGS_ADMIN_CHANGE_ED_REQ_LUA( const char* pUnitNickName, int changeED );

		virtual bool 	Handler_EGS_ADMIN_CHANGE_AUTH_LEVEL_REQ( const WCHAR* userID, CX2User::X2_USER_AUTH_LEVEL level );
		virtual bool 	Handler_EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool 	Handler_EGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ( CX2User::X2_USER_AUTH_LEVEL level );
		virtual bool 	Handler_EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		

		virtual bool Handler_EGS_CHANGE_OPTION_PLAY_GUIDE_REQ();
		virtual bool Handler_EGS_CHANGE_OPTION_PLAY_GUIDE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_ADMIN_CHANGE_SKILL_POINT_REQ( int iSP );
		virtual bool Handler_EGS_ADMIN_CHANGE_SKILL_POINT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef GUILD_SKILL
		//{{ oasis907 : ����� [2009.11.30] // ��� ��ų ����Ʈ ġƮ
		virtual bool Handler_EGS_ADMIN_GET_GUILD_SKILL_POINT_REQ( int iSP );
		virtual bool Handler_EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}} oasis907 : ����� [2009.11.30] // 
#endif GUILD_SKILL


		virtual bool Handler_EGS_ADMIN_INIT_SKILL_TREE_REQ();
		virtual bool Handler_EGS_ADMIN_INIT_SKILL_TREE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_ADMIN_KICK_USER_REQ_LUA( const char* szUserNickName, bool bIsUserID = false );

#ifdef SERV_PC_BANG_TYPE
		virtual bool Handler_EGS_ADMIN_SET_PC_BANG_REQ( bool bEnable, int iPcBangType = 0 );
#else
		virtual bool Handler_EGS_ADMIN_SET_PC_BANG_REQ( bool bEnable );
#endif SERV_PC_BANG_TYPE

		virtual bool Handler_EGS_CHECK_BALANCE_REQ();

		//{{ kimhc // 2009-10-14 // �ڵ����� �Ϸ��� ĳ���κ� ����
#ifdef	AUTO_PAYMENT
		virtual bool Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ();
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-14 // �ڵ����� �Ϸ��� ĳ���κ� ����

		bool Handler_EGS_OPTION_UPDATE_REQ();
		bool Handler_EGS_OPTION_UPDATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		bool Handler_EGS_STATE_CHANGE_FIELD_REQ(int iVillageId = 0);
		bool Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_SEARCH_UNIT_REQ( const WCHAR* wszNickName );
		bool Handler_EGS_SEARCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_LOCAL_RANKING_SYSTEM
		bool Handler_EGS_LOCAL_RANKING_WATCH_UNIT_REQ( UidType	_iUnitUID );
		bool Handler_EGS_LOCAL_RANKING_WATCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_LOCAL_RANKING_SYSTEM

		bool Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( const WCHAR* wszNickName );
		bool Handler_EGS_GET_CONNECTION_UNIT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void UpdateUserInfoUI( KEGS_SEARCH_UNIT_ACK &kEvent );

#ifdef CHANGE_CHANNEL
		bool Handler_EGS_GET_CHANNEL_LIST_REQ();
		bool Handler_EGS_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHECK_CHANNEL_CHANGE_REQ(int iChannelId);
		bool Handler_EGS_CHECK_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ();
		bool Handler_EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CONNECT_CHANNEL_CHANGE_REQ();
		bool Handler_EGS_CONNECT_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ();
		bool Handler_EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_CC_KXPT_PORT_CHECK_REQ();
		bool Handler_CC_KXPT_PORT_CHECK_ACK( const KXPT_PORT_CHECK_ACK& kXPT_PORT_CHECK_ACK );
		void OnFrameMove_GameServerConnect( double fTime, float fElapsedTime );
#endif

#ifdef CLIENT_PORT_CHANGE_REQUEST
		void OnFrameMove_UdpChangeRequest( double fTime, float fElapsedTime );
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifdef SERV_ID_NETMARBLE_PCBANG
		void OnFrameMove_Public_IP_Check( double fTime, float fElapsedTime );
#endif //SERV_ID_NETMARBLE_PCBANG

		//{{ kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#ifdef	SERV_DAY_QUEST
		bool Handler_EGS_EVENT_QUEST_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_CHECK_TIME_QUEST
		bool Handler_EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_CHECK_TIME_QUEST
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#if defined( SERV_HACKING_TOOL_LIST )
		bool Handler_EGS_HACKING_TOOL_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		bool Handler_ECH_HACKING_TOOL_LIST_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_ECH_HACKING_TOOL_LIST_ACK();
#endif

#ifdef SERV_SKILL_NOTE
		bool Handler_EGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_REG_SKILL_NOTE_MEMO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

		//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
		bool CheckIsReady();					// ���� ���� �����̸� true, ���� ���°� �ƴϸ� false �� ����
		void ShowOkAndCancelMsgForUnReady( int iStringID_ );	// ���� ���¸� Ǯ�ڳĴ� ok, cancel �޽��� �ڽ�
		void ShowOkAndCancelMsgForUnReadyByEquip(); // ���� ���¸� Ǯ�ڳĴ� ok, cancel �޽��� �ڽ�
		void ShowOkAndCancelMsgForUnReadyBySkill(); // ���� ���¸� Ǯ�ڳĴ� ok, cancel �޽��� �ڽ�

#endif MODIFY_INFINITE_SKILL_BUG
		//}} kimhc // 2010.3.26 // ���� ��ų ���� ����
		
		//{{ ����� : [2011/3/23/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
		void MoveShowEvent(float fTime);

		void OpenStartMission();
		void OpenResultMission( bool bIsClear );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/23/] //	���� �̼�

#ifdef SERV_PSHOP_AGENCY
		bool Handler_EGS_PSHOP_AGENCY_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
#ifdef NEW_HENIR_TEST
		bool Handler_EGS_HENIR_REWARD_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif NEW_HENIR_TEST
#ifdef SERV_RANDOM_DAY_QUEST
		bool Handler_EGS_CHANGE_RANDOM_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RANDOM_DAY_QUEST
		bool Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#if defined(SERV_MACHINE_ID_BLOCK) && defined(MACHINE_ID)
		void Handler_EGS_CHECK_MACHINE_ID_REQ();
		bool Handler_EGS_CHECK_MACHINE_ID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

// ��μ�
#ifdef SERV_SHUTDOWN_SYSTEM_AUTO_CHECK
		bool Handler_EGS_SHUT_DOWN_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
		// {{ kimhc // 2012-06-18 // ������ �̵��ϱ⸸ �ص� �Ϸ�Ǵ� ����Ʈ �۾�
#ifdef	SERV_ENTER_FIELD_QUEST_CLEAR
		void CompleteQuestToVisitVillage( const CX2QuestManager::QuestTemplet* pQuestTemplet_ );
		KEGS_QUEST_COMPLETE_REQ CompleteQuestToVisitVillageList( const CX2QuestManager::QuestTemplet* pQuestTemplet_ );
		std::map<int,int> GetSelectRewardItemIDList(const CX2QuestManager::QuestTemplet* pQuestTemplet_ );
#endif	SERV_ENTER_FIELD_QUEST_CLEAR
		// }} kimhc // 2012-06-18 // ������ �̵��ϱ⸸ �ص� �Ϸ�Ǵ� ����Ʈ �۾�

		// {{ ����ȯ // 2012-06-25 // ������ �˴ٿ��� �ȳ�
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		bool Handler_EGS_BEFORE_SHUT_DOWN_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		// }} ����ȯ // 2012-06-25 // ������ �˴ٿ��� �ȳ�
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL 
		bool Handler_EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ();
		bool Handler_EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_ADMIN_CHEAT_GET_ALL_SKILL 

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		bool Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ();
		bool Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_ADMIN_GET_SKILL_REQ( const int iSkillID, const int iSkillLevel );
		bool Handler_EGS_ADMIN_GET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		bool Handler_EGS_UDP_KICK_GAMEEDIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //UDP_CAN_NOT_SEND_USER_KICK

#ifdef SERV_2012_PVP_SEASON2_EVENT
		void OpenTimeOfGlory();		/// ������ �ð� ���̾�α� Ȱ��
		void MoveTimeOfGlory();		/// ������ �ð� ���̾�α� ����
#endif SERV_2012_PVP_SEASON2_EVENT

		void Handler_EGS_JOIN_BATTLE_FIELD_REQ( const UINT uiBattleFieldId_, const USHORT usBattleFieldLineIndex_, const USHORT usBattleFieldPositionValue_, const bool bMoveToMyParty_ = false );
		void Handler_EGS_JOIN_BATTLE_FIELD_REQ( const UINT uiBattleFieldId_, const USHORT usBattleFieldStartLineIndex_ ) const;
		bool Handler_EGS_JOIN_BATTLE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_JOIN_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_LEAVE_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
		bool	Handler_EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�

		void ReturnToPlaceWhereBeforeDungeonStart();
		
		virtual bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
		virtual bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void Send_EGS_UPDATE_PLAY_STATUS_NOT();

		void Send_EGS_USE_SKILL_BUFF_IN_PLAY_NOT( const int iSkillID_, const bool bOn_ );
		bool Handler_EGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
		bool Handler_EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_BATTLE_FIELD_DANGEROUS_CHEAT

		bool Handler_EGS_SURVEY_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ���� �߿� �ʵ� 10���� Ű�� ���� ���� ����
		bool GetEnableShortCutKey()const { return m_bEnableShortCutKey; }
		void SetEnableShortCutKey(bool bVal){ m_bEnableShortCutKey = bVal; }
		
		bool GetWaitGameStart() const { return m_bWaitGameStart; }

#ifdef SERV_HERO_PVP_MANAGE_LIST
#ifdef ADD_HERO_MATCH_NOTICE	/// ���� ���� ��� ���� ���
		/// ���� �׷쿡�� �Է��� ���� ����
	//	bool Handler_EGS_ADMIN_REMOVE_HERO_PVP_USER_REQ( UidType kpiServerGroupID_, const char* pNickName_ );
		bool Handler_EGS_ADMIN_REMOVE_HERO_PVP_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		/// ��� ���� ���
		bool Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ( const WCHAR* chMessage_, const WCHAR* chNickName_, int iServerGroupID_ );
		bool Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ_LUA( const char* chMessage_, const char* chNickName_, int iServerGroupID_ );
		bool Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//bool Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ_LUA();
#endif ADD_HERO_MATCH_NOTICE
#endif //SERV_HERO_PVP_MANAGE_LIST

		//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
		bool Handler_EGS_ADMIN_OBSERVER_MODE_REQ( bool bMode );
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
		//}}

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		bool Handler_EGS_TCP_PING( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
		bool Handler_EGS_GET_ACTIVE_LAGCHECK_REQ();
		bool Handler_EGS_GET_ACTIVE_LAGCHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//		bool Handler_EGS_TCP_RELAY_TEST2( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//#endif//X2OPTIMIZE_TCP_RELAY_TEST

#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
		void SetFadeInOutNotice( const WCHAR* wszMsg_, float fVisibleTime_ = 5.f, float fOffsetPosX_ = 0.f , float fOffsetPosY_ = 0.f );		/// ȭ�鿡 ���̵� �� ~ ���̵� �ƿ� �Ǵ� ���� ���� �Լ�
		void ProcessFadeInOutNotice();													/// ȭ�鿡 ���̵� �� ~ ���̵� �ƿ� �Ǵ� ���� ���� �Լ�
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef SERV_INVISIBLE_GM
		bool Handler_EGS_TOGGLE_INVISIBLE_REQ();
#endif SERV_INVISIBLE_GM
		//{{ ���� - ���� ���� ����Ʈ, ĳ�� �۾� 
#ifdef SERV_SHARING_BANK_QUEST_CASH
		bool Handler_EGS_SHARING_BACK_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
		//}}

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		bool Handler_EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// DamagedMap ���� REQ
		bool Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// DamagedMap ���� NOT
		bool Handler_EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// ���赵 ���� ġƮ
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS	

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-11
		bool Handler_EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
		bool Handler_EGS_CALL_MY_LOVER_REQ( int iMapID, D3DXVECTOR3 vPos, unsigned char ucLastTouchLineIndex );
		bool Handler_EGS_CALL_MY_LOVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CALL_MY_LOVER_VILLAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT( bool bAgree );
		bool Handler_EGS_CALL_MY_LOVER_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT( bool bAgree );
		bool Handler_EGS_CALL_MY_LOVER_AGREE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_ADMIN_CHANGE_COUPLE_DATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_JOIN_WEDDING_HALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false;};
		
#endif // ADDED_RELATIONSHIP_SYSTEM

		virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
			const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ ) {};

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		void SetShowJumpingCharacterClassChangeDlg ( const bool bIsShow_ );		// ���� ĳ���� ���� UI 
		void SetShowJumpingCharacterClassNoticeChangeDlg ( bool bIsShow_ );		// ���� ĳ���� ���� �˸� UI

		bool GetIsJumpingCharacterDlgPopup() const;		// ���� ĳ���� Dlg �� ���� ���ΰ�?
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		wstring GetExpirationDataDesc( IN const SEnum::AGENCY_SHOP_TYPE eAgencyShopType_, IN const wstring& wstrAgencyShopExpirationDate_ );
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef SERV_CATCH_HACKUSER_INFO
		bool Handler_EGS_CATCH_HACKUSER_INFO_NOT( int iCrashType );
#endif SERV_CATCH_HACKUSER_INFO

#ifdef SERV_RELATIONSHIP_EVENT_INT
		bool Handler_EGS_DIVORCE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RELATIONSHIP_EVENT_INT

	protected:
		virtual bool UIEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UICustomFuncProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerTimeOutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		virtual bool TCPRelayEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual void LuaFrameMove( double fTime, float fElapsedTime );
		//void		 LuaFuncCall( const WCHAR* pFuncName );

		virtual bool RegisterLuaBind();

				//nProtect ���� ���� ó��
				bool Handler_EGS_KNPROTECT_AUTH_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
				bool Handler_EGS_KNPROTECT_USER_SHOWDOWN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_NPROTECT_CS_AUTH_30
				bool Handler_EGS_NPROTECT_CSAUTH3_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_NPROTECT_CS_AUTH_30

		//{{ ����� : [2009/8/19] //	�ٽ��� Ȯ�弭������ ��Ŷ ó��
#ifdef HACK_SHIELD
		bool Handler_EGS_HACKSHIELD_CHECK_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
		//}} ����� : [2009/8/19] //	�ٽ��� Ȯ�弭������ ��Ŷ ó��
#ifdef CLIENT_USE_XTRAP	// XTRAP Ŭ���̾�Ʈ - ���� ��û �޴� �Լ�
		bool Handler_EGS_XTRAP_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif CLIENT_USE_XTRAP

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		bool Handler_EGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_NEW_UNIT_TRADE_LIMIT

		virtual bool Handler_EGS_NOTIFY_MSG_REQ( int noticeCount, const WCHAR* pMsg, bool bIsAllServers = false );
		virtual bool Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_ADMIN_SERVER_STATUS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		virtual bool Handler_EGS_CLIENT_QUIT_REQ();
		virtual bool Handler_EGS_CLIENT_QUIT_ACK();


		virtual bool Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ( const WCHAR* pUnitNickName, int changeLevel, int iChangeEXP );
		virtual bool Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_ADMIN_CHANGE_ED_REQ( const WCHAR* pUnitNickName, int changeED );
		virtual bool Handler_EGS_ADMIN_CHANGE_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		virtual bool Handler_EGS_CHECK_INVALID_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		virtual bool Handler_EGS_UPDATE_QUEST_REQ();
		virtual bool Handler_EGS_UPDATE_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_UPDATE_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_CHECK_SUM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_ADMIN_KICK_USER_REQ( const WCHAR* szUserNickName, bool bIsUserID = false );
		virtual bool Handler_EGS_ADMIN_KICK_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//{{ 2008. 1. 2  ������  �ټ��� ġƮ
		virtual bool Handler_EGS_ADMIN_CHANGE_SPIRIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}}
		
		virtual bool Handler_EGS_ADMIN_SET_PC_BANG_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//{{ 2008. 1. 25  ������  VP ġƮ
		virtual bool Handler_EGS_ADMIN_CHANGE_VSPOINT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}}

		//{{ 2008. 2. 18  ������  ���� Ŭ���� ġƮ
		virtual bool Handler_EGS_ADMIN_CHANGE_UNIT_CLASS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}}

		//{{ 2008. 10. 24  ������	Ÿ��Ʋ ġƮ
		virtual bool Handler_EGS_ADMIN_GET_TITLE_CHEAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}}

		//virtual bool Handler_EGS_REWARD_TO_TUTORIAL_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//virtual bool Handler_EGS_REWARD_TO_RECOMMEND_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_REWARD_MARKETING_EVENT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_RECOMMEND_USER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
		virtual bool Handler_EGS_TIME_EVENT_INFO_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ

#ifdef WEB_POINT_EVENT // oasis907 : ����� [2010.2.24] // 
		virtual bool Handler_EGS_GET_WEB_POINT_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif WEB_POINT_EVENT		
		virtual bool Handler_EGS_CHECK_BALANCE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_CASH_PRODUCT_INFO_CHANGED_NOT();

		virtual bool Handler_EGS_GUEST_USER_LIMIT_LEVEL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_NEW_PRESENT_CASH_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		virtual bool Handler_EGS_PRESENT_MESSAGE_TO_RECEIVER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		//virtual bool Handler_EGS_GET_POST_LETTER_LIST_REQ();
		//virtual bool Handler_EGS_GET_POST_LETTER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_GET_RANKING_INFO_REQ();
		virtual bool Handler_EGS_GET_RANKING_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		bool Handler_EGS_DELETE_NICK_NAME_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//{{ 2009. 7. 29  ������	��ŷ ��� �˾�
		bool Handler_EGS_NEW_RECORD_RANKING_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}}

		bool Handler_EGS_CHANGE_JOB_CASH_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		bool Handler_EGS_CHANGE_MY_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
		bool Handler_EGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	//	AUTO_PAYMENT
		//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��

#ifdef DEFEND_TRAFFIC_ATTACK
		bool Handler_EGS_TRAFFIC_ATTACKER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif DEFEND_TRAFFIC_ATTACK


		//{{ oasis907 : ����� [2009.11.19] //
		bool Handler_EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}} oasis907 : ����� [2009.11.19] //

#ifdef PC_BANG_SANG
		// oasis907 : ����� [2010.1.29] //  PC�� ���ȭ
		virtual bool Handler_EGS_PC_BANG_AUTH_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif PC_BANG_SANG

		//{{ oasis907 : ����� [2010.5.6] // ���� ���� ���� ������ �̸� ǥ��
		bool Handler_EGS_ERROR_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//}}

		//{{ kimhc // 2011-08-10 // ä�θ� SOAP
#ifdef	SERV_CHANNELING_SOAP
		bool Handler_EGS_TOONILAND_USER_ID_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_CHANNELING_SOAP
		//}} kimhc // 2011-08-10 // ä�θ� SOAP
#ifdef UNIT_CLASS_CHANGE_ITEM
		bool Handler_EGS_UNIT_CLASS_CHANGE_CASH_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef SERV_EVENT_MONEY
		bool Handler_EGS_UPDATE_EVENT_MONEY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_EVENT_MONEY

		bool Handler_EGS_DECREASE_SPIRIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_DROP_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_GET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_GET_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_GET_ITEM_REALTIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_USER_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_USER_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_UPDATE_PARTY_MEMBER_STATUS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_GAME_START_TIME_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void SetShowPictureGameStartCount( const bool bIsDungeonWait, const int iCount_ );

#ifdef	SERV_ACCOUNT_BLOCK_USER_POPUP_TEMP
		void Handler_EGS_ACCOUNT_BLOCK_USER_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_ACCOUNT_BLOCK_USER_POPUP_TEMP

		// ĳ���ͼ���
		virtual bool HandleMsgByESC();
		virtual bool HandleMsgByESCImp();
		virtual bool PutOffPopUpDialog();
		virtual bool QuitGame();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		virtual void OpenLastMsgPopUp( const WCHAR* pMsg, wstring wstrCustomLuaFileName = L"", wstring wstrPlaySoundFileName = L"" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		virtual void OpenLastMsgPopUp( const WCHAR* pMsg );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����


		//ESCŰ ������ ���� ���� ���� �ٸ� �޽����� ���� �� ��� ����ؼ� �������̵� ������� �Լ��� =_=a
		virtual bool LastMsgByESC();
		virtual void LastMsgPopOk();
		virtual void LastMsgPopCancel(); //ĵ���� �׳� ���̾�α� ���ִ°͸��� Ư���Ѱ� ���ְ� ������ ��ӹ޾Ƽ� ���ٶ�

		virtual bool ShortCutKeyProcess();

		virtual void UpdateCashUI();
		
//{{ kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ)
#ifdef SERV_SECRET_HELL
		void CheckNRefreshDailyQuest();
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ)
		

		// 09.03.23 ���¿� : �̺�Ʈ Ÿ�̸� ��ġ �̵�
		//void CloseGuestUserEventTimerPopup();
		//void OpenGuestUserEventTimerPopup();
		bool Handler_EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#if defined(SERV_PROTECT_AUTO_HACK) && defined(SERV_SERVER_CHECK_HACK_USER_REQ_ACK)
		bool Handler_EGS_SERVER_CHECK_HACK_USER_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_SERVER_CHECK_HACK_USER_ACK();
#endif

		//{{ kimhc // 2010-08-16 // ������ �ϸ� �Ⱓ�� �þ�� �̺�Ʈ ���� ������ �ý����� ������ �� �Դϴ�.
#ifdef	SERV_RESET_PERIOD_EVENT
		bool Handler_EGS_RESET_PERIOD_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_RESET_PERIOD_EVENT
		//}} kimhc // 2010-08-16 // ������ �ϸ� �Ⱓ�� �þ�� �̺�Ʈ ���� ������ �ý����� ������ �� �Դϴ�.

#ifdef SERV_ADMIN_SHOW_ED
		bool Handler_EGS_ADMIN_SHOW_ED_CHEAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
#ifdef SERV_DLL_MANAGER
		void SendLoadedModuleList(bool bForce = false);
#endif

		//{{ 2012. 07. 09	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
		bool Handler_ECH_WAIT_QUEUE_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

#ifdef SERV_ADVERTISEMENT_EVENT
		void OpenAdvertisementDialog( const wstring& wstrURL );
		virtual bool Handler_EGS_ADVERTISEMENT_EVENT_INFO_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_NEW_EVENT_TYPES
		bool Handler_EGS_UPDATE_MAX_LEVEL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
		bool Handler_EGS_UPDATE_CODE_EVENT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_CODE_EVENT

		//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		bool Handler_EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_FORCE_RELAY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		bool Handler_EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //UDP_CAN_NOT_SENDk_USER_KICK


#ifdef	SERV_FIELD_WORKINGS_BLOCK_LOG
		void Send_EGS_FIELD_WORKINGS_BLOCK_LOG_ACK();
#endif	SERV_FIELD_WORKINGS_BLOCK_LOG

		bool Handler_EGS_GET_ITEM_TEMP_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON
		bool Handler_EGS_SHOW_DISCONNECT_REASON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		bool Handler_ESG_REWARD_DB_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

		virtual void UserAndPetListPopUpMenuProcess() {}
#ifdef PLAY_EMOTION_BY_USER_SELECT
		virtual void PlayEmotionByUserSelect(){}
#endif // PLAY_EMOTION_BY_USER_SELECT

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void CheckAndSendingPlayStatus( float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void CheckAndSendingPlayStatus();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		
		bool ProcessGameScoreAndPostEffect();
		virtual void ProcessShowMiniMap( const bool bHide_ ) {};

#ifdef SERV_ID_NETMARBLE_PCBANG
		bool Handler_CC_KXPT_PUBLIC_IP_CHECK_REQ();
#endif //SERV_ID_NETMARBLE_PCBANG
		
#ifdef SERV_ID_NETMARBLE_PCBANG
		std::wstring GetMACAddress() const;
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		void Handler_EGS_JUMPING_CHARACTER_REQ( const int iUnitClassID_ );			// ���� �� ĳ������ Unit Class �� ��� �����Ѵ�.
		bool Handler_EGS_JUMPING_CHARACTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// ���� �� ĳ������ ������ �޴´�.
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

	public:
#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
		vector<FadeInOutNotice*> m_vecFadeInOutNotice;	/// ���̵� �� ~ ���̵� �ƿ� ���� ���� ����� �����̳�
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef _IN_HOUSE_
#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
		bool Handler_EGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
#endif _IN_HOUSE_

#ifdef SERV_RECRUIT_EVENT_BASE
		virtual bool Handler_EGS_REGISTER_RECRUITER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
		void Handler_EGS_EGS_GET_EVENT_INFO_REQ();
		virtual bool Handler_EGS_GET_EVENT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
		void SetShowNoteUI( bool bShow ) { m_bShowNoteUI = bShow; }
		void ProcessElesisEvent( UCHAR ucLevel, int iNoteViewCount );
		void ReadyToShowEventNoteUI();
		void Handler_EGS_EVENT_NOTE_VIEW_REQ();
		bool Handler_EGS_EVENT_NOTE_VIEW_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		bool Handler_EGS_EVENT_CHUNG_GIVE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		bool Handler_EGS_EVENT_COBO_DUNGEON_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		bool Handler_EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
		bool Handler_EGS_CASH_DIRECT_CHARGE_CN_REQ();
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
	protected:

		float			m_fTime;
		float			m_fElapsedTime;

		CX2Cursor*		m_pCursor;

		bool			m_bFailToConnectServer;
		CKTDGUIDialogType	m_pMsgOkFailToConnectServer;
#ifdef PC_BANG_SANG	
		//{{ oasis907 : ����� [2010.2.3] // 
		bool			m_bFailPCBANGAuth;
		CKTDGUIDialogType	m_pMsgOkFailPCBANGAuth;
		//}}
#endif PC_BANG_SANG	
		string			m_LuaFrameMoveFuncName;

		CKTDGUIDialogType	m_pMsgBoxQuitGame;		/// ���� ���� ��ȭ ����
		bool			m_bOpenMsgBoxQuitGame;
		bool			m_bEnableQuitGame;

		CKTDGUIDialogType	m_pDLGMsgBox;


		bool			m_bOneTimeProcAfterContructor;
		
		bool			m_bIgnoreAllUIEvent;
		float			m_fIgnoreTime;

		CKTDGUIDialogType	m_pDLGLastMsgPopUp;

		bool			m_bEnableShortCutKey;


		CKTDGUIDialogType	m_pDLGEventClock;
		CKTDGUIDialogType	m_pDLGEventClockPopup;
		CKTDGUIDialogType	m_pDLGGuestUserGoUnitSelect;


		CKTDGUIDialogType	m_pDLGRecommendReward;
		CKTDGUIDialogType	m_pDLGTutorReward;
		CKTDGUIDialogType	m_pDLGGuestUserReward;

		bool			m_bCheckPostItem;

        bool            m_bPersonalTrade;

		CKTDGUIDialogType	m_pDLGUserInfo;

#ifdef CHANGE_CHANNEL
		bool			m_bConnectedGameServer;
		bool			m_bPortCheckReq;
		bool			m_bPortCheckWait;
		float			m_fPortCheckWaitTime;
		int				m_PortCheckRetryTime;
#endif
		int				m_iChangeChannelId;

		bool			m_bConnectFullChannel;		// Full ä�ο� ���ӽ� ���ӽ����Ͽ� ������ ������ "�����κ��� ������ �����ϴ�." �޼��� �ȶ��� ���� �÷���



		MouseClickUI*								m_pMouseClickUI;
		map< MouseClickUI::HIDE_REASON, bool >		m_mapHideMouseClickUI;


#ifdef RESET_INVALID_UNIT_POSITION_TEST
		CKTDXTimer m_TimerReplaceMisplacedUnit;
#endif RESET_INVALID_UNIT_POSITION_TEST

		//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
		CKTDGUIDialogType		m_pDLGUnReady;
#endif MODIFY_INFINITE_SKILL_BUG
		//}} kimhc // 2010.3.26 // ���� ��ų ���� ����		

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
		bool				m_bIdentityConfirmCheck;
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

		//{{ ����� : [2011/3/24/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
		float							m_fShowTimeForWorldMissionBuffName;
		CKTDGUIDialogType				m_pDLGWorldMissionBuffName;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/24/] //	���� �̼�

#ifdef DIALOG_SHOW_TOGGLE
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticleLogo;
#endif

		CKTDGUIDialogType	m_pDlgGameStartCount;
		
#ifdef BANDICAM_RECORDING
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticleRec;
#endif

		CKTDGUIDialogType	m_pDLGStateChangePopup;

#ifdef PC_BANG_BENEFIT_GUIDE
		CKTDGUIDialogType				m_pDLGPCBangBenefitGuide;
#endif //PC_BANG_BENEFIT_GUIDE

#ifdef SERV_2012_PVP_SEASON2_EVENT
		float							m_fTimeOfGloryRemainTime;	/// ������ �ð� ���̾�α� Ȱ�� �ð�
		CKTDGUIDialogType				m_pDLGTimeOfGlory;			/// ������ �ð� ���̾�α�
#endif SERV_2012_PVP_SEASON2_EVENT

		bool							m_bWaitGameStart;

#ifdef SERV_ADVERTISEMENT_EVENT
		CKTDGUIDialogType	m_pDlgAdvertisement;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef CLIENT_PORT_CHANGE_REQUEST
		int							m_iUDPRequestCount_X2State;		// 2013.05.10 lygan_������ // X2State ������ ����ϴ� UDP ���� ī��Ʈ üũ ����
		boost::timer                m_TimerUDPRequestNotice;		// 2013.05.14 lygan_������ // UDP ��Ʈ ���� �Ȱ��� ã�� ������ �������� 5�� �ֱ�� �˸����Ѵ�.
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
		/// �������� ����ϴ� ���ڰ� ����
		UidType							m_uiRoomUID;
		UidType							m_iCalledUserUID;
		UidType							m_iCalledGSUID;
		char							m_cCallType;
		int								m_iMapID;
		D3DXVECTOR3						m_vPos;
		unsigned char					m_LastTouchLineIndex;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		CKTDGUIDialogType				m_pDlgJumpingCharacterClassChange;		// ���� ĳ���Ϳ� Ŭ���� ���� Dlg
		CKTDGUIDialogType				m_pDlgJumpingCharacterClassChangeNotice;// ���� ĳ���͸� ���� �����մϴ�! Dlg
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		bool							m_bReConnectChannelServer;		// ä�� ���� ��, �����ϸ� �ٸ� ä���� ã�ƺ� ���ΰ�?
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef SERV_CHECK_TIME_QUEST
		bool							m_bAlreadyShowErrorMessage;
#endif //SERV_CHECK_TIME_QUEST

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
		CKTDGUIDialogType				m_pDlgEventCharacterRanking;
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
		bool						m_bShowNoteUI;
		bool						m_bPlayNoteUI;
		bool						m_bProcessNoteView;
		CKTDGUIDialogType			m_pDlgEventElesisNote;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstEventNote;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstEventNoteStart;		
		float						m_fPlayTime;
		int							m_iRewardTitleIndex;
#endif SERV_ELESIS_UPDATE_EVENT
};
