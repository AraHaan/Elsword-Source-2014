#pragma once

/** @class CX2StateDungeonResult
	@brief Doxygen �ּ� �߰�
	@date 2011/02/09
*/
class CX2StateDungeonResult : public CX2StateCommonBG
{
	public:
//{{ Public Enum, Struct Start
		/// ���� ��� UI ����� ���� �޽���
		enum DUNGEON_RESULT_UI_CUSTOM_MSG
		{
			DRUCM_CHOICE_ITEM_BOX = 0,			/// 
			DRUCM_GO_TRAINING_SCHOOL_OK,		/// 
			DRUCM_GO_TRAINING_SCHOOL_CANCEL,	/// 

			DRUCM_ESCAPE_DUNGEON_RESULT_OK,		/// ���� ���â ��Ż �˾� Ȯ��
			DRUCM_ESCAPE_DUNGEON_RESULT_CANCEL,	/// ���� ���â ��Ż �˾� ĵ��
		};

		/// ���� ��� �ִϸ��̼� ���
		enum DUNGEON_RESULT_ANIMATE_ORDER
		{
			DRAO_NONE = 0,						/// ����
			DRAO_DUNGEON_PICTURE_BG,			/// ���� �׸� ���

			//{{ kimhc // 2011-04-18 // ����, ���潺 ���� // #define SERV_INSERT_GLOBAL_SERVER 
			DRA0_DUNGEON_DEFENCE_RESULT,		/// ���潺 �������� ������ ���� ���� ����
			//}} kimhc // 2011-04-18 // ����, ���潺 ���� // #define SERV_INSERT_GLOBAL_SERVER 

			DRAO_TOTAL_SCORE_BG,				/// ���� ���
			DRAO_TIME_AND_DAMAGED_SCORE_BG,		/// �ð�, ���� ���� ���
			DRAO_COMBO_SCORE,					/// �޺� ����
			DRAO_TECHNICAL_SCORE,				/// ��� ����
			DRAO_TIME,							/// �ð�
			DRAO_DAMAGED,						/// ����
			DRAO_TOTAL_SCORE,					/// ����
			DRAO_TOTAL_SCORE_RANK,				/// ���� ���
			DRAO_OTHERS_RANK,					/// Ÿ�� ���
			DRAO_WAIT_INFO_FADE_OUT,			/// 
			DRAO_FIRST_RESULT_INFO_FADE_OUT,	/// 
			DRAO_CHOICE_REWARD,					/// ���� ����
			DRAO_ANIMATE_BOX,					/// �ִϸ���Ʈ �ڽ�?
			DRAO_ED_BG,							/// ED(���ҵ� ��) ���
			DRAO_ITEM_BG,						/// ������ ���
			DRAO_EXP_BG,						/// ����ġ ���
			DRAO_ED,							/// ED(���ҵ� ��)
			DRAO_ITEM,							/// ������
			DRAO_EXP,							/// ����ġ
			DRAO_BONUS_EXP,						/// �������
			DRAO_EXP_BAR,						/// ����ġ �����
// #ifdef SERV_DUNGEON_MVP
			DRAO_PICKED_MVP_PLAYER,				/// MVP Player �� ������ ����
// #endif // SERV_DUNGEON_MVP

			DRAO_END,
		};

		/// ��� ����
		enum RANK_SCORE
		{
			RS_A = 0,	/// A
			RS_B,		/// B
			RS_C,		/// C
			RS_A_PLUS,	/// D
			RS_B_PLUS,	/// E
			RS_C_PLUS,	/// F
			RS_D,		/// D
			RS_E,		/// E
			RS_F,		/// F
			RS_S,		/// S
			RS_SS,		/// SS
			RS_DEAD,	/// DEAD
			RS_END,
		};

		/// ���ʽ� EXP ����
		enum BonusEXPType
		{
			BET_CLEAR = 0,		/// �Ϸ�?
			BET_RANK,			/// ���
			BET_PARTY,			/// ��Ƽ �÷���
			BET_MVP,			/// MVP
			BET_GAMEBANG,		/// PC ��
			BET_PREMIUM,		/// �����̾�?
            BET_EVENT,			/// �̺�Ʈ
			BET_CHANNEL_BONUS,	/// ä�� ���ʽ�
		};

		/** @struct BonusEXPData
			@brief ���ʽ� ����ġ ���¿� ����ġ�� ����
			@date 2011/02/09
		*/
		struct BonusEXPData
		{
			BonusEXPType bonusExpType;
			int	bonusExp;
		};
//{{ Public Enum, Struct End

	public:
//{{ Public Function Start
		CX2StateDungeonResult(void);
		virtual ~CX2StateDungeonResult(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		//{{ kimhc // 2011-04-18 // ����, ���潺 ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
		bool GetShowNumOfElCrystalsProtected() const { return m_bShowNumOfElCrystalsProtected; }
		void SetShowNumOfElCrystalsProtected( bool bShowNumOfElCrystalsProtected_ ) { m_bShowNumOfElCrystalsProtected = bShowNumOfElCrystalsProtected_; }
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-18 // ����, ���潺 ����

//{{ Public Function End

	protected:
//{{ Protected Function Start
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool LastMsgByESC();

		//bool Handler_EGS_REFRESH_ROOM_REQ();
		//bool Handler_EGS_REFRESH_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_RESULT_SUCCESS_REQ();
		bool Handler_EGS_RESULT_SUCCESS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_START_REWARD_BOX_SELECT_REQ();
		bool Handler_EGS_START_REWARD_BOX_SELECT_ACK();
		bool Handler_EGS_START_REWARD_BOX_SELECT_NOT();

		bool Handler_EGS_SELECT_REWARD_BOX_REQ( int boxNum );
		bool Handler_EGS_SELECT_REWARD_BOX_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_SELECT_REWARD_BOX_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool InitUI();

		void UnitViewerProcess( CX2UnitViewerUI* pViewer );

		RANK_SCORE Get_UI_RANK_SCORE( CX2DungeonRoom::RANK_TYPE rankType );

		void ShowBonusEXPEvent( CX2StateDungeonResult::BonusEXPType bonusEXPType, int bonusEXP );

		virtual void PicCharFrameMove( double fTime, float fElapsedTime );
		void PicCharDrawText();

		bool GetPressEnterKeyOrZKeyForSkip() const { return m_bPressEnterKeyOrZKeyForSkip; }
		void SetPressEnterKeyOrZKeyForSkip(bool val) { m_bPressEnterKeyOrZKeyForSkip = val; }
		//}} Protected Function End

	protected:
//{{ Protected Variable Start
		CKTDGUIDialogType		m_pDLGBack;		/// ���̾�α� ���
		CKTDGUIDialogType		m_pDLGFront;	/// ���̾�α� ����
		CKTDGUIDialogType		m_pDLGMsgBox;	/// ���̾�α� �޽��� �ڽ�

		float				m_fPlayBGM;		/// 

		bool				m_bStartTimer;			/// ���� Ÿ�Ӹ�
		float				m_fRemainTime;			/// ���� �ð�
		bool				m_bSendRefreshRoomReq;	/// �� ����

		float				m_fTotalScore;			/// ����

		float				m_fComboScore;			/// �޺� ����
		float				m_fTechnicalScore;		/// ��ũ���� ����

		float				m_fHour;				/// �ð�
		float				m_fMinute;				/// ��
		float				m_fSec;					/// ��

		float				m_fDamaged;				/// �ǰ� ������


		DUNGEON_RESULT_ANIMATE_ORDER m_DungeonResultAniOrder;	/// ��� �ִϸ���Ʈ ���

		bool				m_bInitUIFail;			/// UI �ʱ�ȭ ���� ����


		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hParticleElMark;	/// ?
		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hParticleRank;		/// ��� ��ƼŬ?

		CKTDXDeviceSound*					m_pSoundElMark;

	
		CKTDGPicChar*						m_pPicCharScore;				/// ���� �׸� ����
		CKTDGPicChar*						m_pPicCharEXP;					/// ����ġ
		CKTDGPicChar*						m_pPicCharBonusEXP;				/// ���ʽ� ����ġ
		CKTDGPicChar*						m_pPicCharComboAndTechScore;	/// �޺� �� ��ũ���� ����
		CKTDGPicChar*						m_pPicCharED;					/// ED(���ҵ� ��)

		bool								m_bAnimateScoreBG;				/// �ִϸ���Ʈ ���� ���

		float								m_fStartWaitRemainTime;			/// ���۴������ �ð�
		bool								m_bUIAnimStart;					/// UI �ִϸ��̼� ����

		float								m_fMyTotalRankWaitRemainTime;	/// 
		bool								m_bUIMyTotalRankStart;			/// 

		float								m_fWaitForFadeOutRemainTime;	/// 
		bool								m_bWaitForFadeOut;				/// 

		int									m_ChoiceItemID;					/// ���� ������ ID
		float								m_fED;							/// ED
		float								m_fEXP;							/// ����ġ
		float								m_fBonusEXP;					/// ���ʽ� ����ġ
		float								m_fEXPSize;						/// ����ġ ũ��

		float								m_fAnimatBoxTime;				/// �ִϸ���Ʈ �ڽ� �ð�?

		bool								m_bSendStartRewardReq;			/// 
		bool								m_bStartRewardBoxSelectNot;		/// 

		vector< CKTDGXMeshPlayer::CXMeshInstanceHandle > m_vecRewardXMeshInst;
		vector< CKTDGParticleSystem::CParticleEventSequenceHandle  > m_vecBonusStamp;

		vector< BonusEXPData >				m_vecBonusExpType;
		float								m_fTimeBonusEXPShowEvent;
		bool								m_bCheckBonusEXP;

		bool								m_bSendedRegHack;
		//{{ kimhc // 2011-04-18 // ����, ���潺 ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
		bool								m_bShowNumOfElCrystalsProtected;	/// ��ȣ�� ���� ������ ��� �� ���ΰ�? (���潺 �������� �¸��� ��츸 ���)
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-18 // ����, ���潺 ����

		bool								m_bPressEnterKeyOrZKeyForSkip;		/// ��ŵ�� ���� Enter �Ǵ� Z Ű�� �������� ����

		//}} Protected Variable End
};
