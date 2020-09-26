#pragma once


#include "./X2TrainingGameUI.h"

/** @class	CX2DungeonGame
	@brief  ���� ���� ���� Ŭ����
	@date
*/
class CX2DungeonGame : public CX2Game
{
	public:
//{{ Public enum, struct Start
		/** @struct	DungeonResultUnitInfo
			@brief  ���� ��� ���� ����
			@date
		*/

	
		struct DungeonResultUnitInfo  
		{
			UidType	m_UnitUID;			/// ���� ID

			bool	m_bIsDie;			/// ���� Ȯ��
			int		m_nTotalScore;		/// ����
			int		m_nComboScore;		/// �޺� ����
			int		m_nTechnicalScore;	/// ��ũ���� ����
			int		m_nDamaged;			/// �ǰ� ī��Ʈ
			int		m_nNPCKillNum;		/// ���� ���� ��
			int		m_nED;				/// ȹ�� ED
			int		m_nEXP;				/// ȹ�� ����ġ

			//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED �� ����ġ ȹ��
#ifdef	REAL_TIME_ELSWORD
			int		m_nOldED;			// ������ ���ö� ������ �ִ� ED�� ��
			int		m_nOldEXP;			// ������ ���ö� ������ �ִ� ����ġ
#endif	REAL_TIME_ELSWORD
			//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED �� ����ġ ȹ��


			int		m_nClearBonusEXP;		/// �Ϸ� ���ʽ� ����ġ
			int		m_nRankBonusEXP;		/// ��ũ ���ʽ� ����ġ
			int		m_nPartyBonusEXP;		/// ��Ƽ ���ʽ� ����ġ
			int		m_nMvpBonusEXP;			/// MVP ���ʽ� ����ġ
			int		m_nGameBangBonusEXP;	/// PC �� ���ʽ� ����ġ
			int		m_nPremiumBonusEXP;		/// �����̾� ���ʽ� ����ġ
            int     m_nEventBonusExp;		/// �̺�Ʈ ���ʽ� ����ġ
			int		m_nChannelBonusEXP;		/// ä�� ���ʽ� ����ġ

			float	m_fRankBonus;	/// ��ũ ���ʽ�
			float	m_fPartyBonus;	/// ��Ƽ ���ʽ�


#ifdef SERV_DUNGEON_MVP
			bool	m_bIsMVPPlayer;			// MVP Player �ΰ�?
#endif // SERV_DUNGEON_MVP

			CX2DungeonRoom::RANK_TYPE	m_cComboRank;		/// �޺� ��ũ
			CX2DungeonRoom::RANK_TYPE	m_cTechnicalRank;	/// ��ũ���� ��ũ
			CX2DungeonRoom::RANK_TYPE	m_cDamagedRank;		/// ������ ��ũ
			CX2DungeonRoom::RANK_TYPE	m_cTimeRank;		/// �ð� ��ũ
			CX2DungeonRoom::RANK_TYPE	m_cTotalRank;		/// �� ��ũ
				

            DungeonResultUnitInfo();		/// ������
            int GetTotalExp();				/// �� ����ġ ���
		};

		/** @struct	DungeonResultInfo
			@brief	���� ��� ���� 
			@date
		*/
		struct DungeonResultInfo
		{
			int									m_Sec;							/// �ð�
			bool								m_bWin;							/// ���� ����
			vector< DungeonResultUnitInfo* >	m_DungeonResultUnitInfoList;	/// ���� ��� ���� ���� ���

// 			std::vector<KArcadeRankingInfo>		m_vecTodayRankingInfo;			/// ���� ��ŷ ����
// 			KArcadeRankingInfo					m_kTopRankingInfo;				/// �ְ� ��ŷ ����
// 			KArcadeRankingInfo					m_kResultRankingInfo;			/// ��� ��ŷ ����
// 			KArcadeRankingInfo					m_kMyTopRankingInfo;			/// �ڽ��� �ְ� ��ŷ ����
//			int									m_iRank;						/// ��ũ
			//{{ kimhc // 2011-04-18 // ����, ���潺 ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
			unsigned char						m_ucNumOfProtectedSmallCrystals;	/// ��ȣ�� ���� ũ����Ż ��
#endif	SERV_INSERT_GLOBAL_SERVER
			//}} kimhc // 2011-04-18 // ����, ���潺 ����

			DungeonResultInfo( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket );
			~DungeonResultInfo();
			
#ifdef	SERV_DUNGEON_MVP
			UidType GetMvpPlayerUID ();
#endif // SERV_DUNGEON_MVP

		};
//}} Public enum, struct End
	
	public:
//{{ Public Function Start
		CX2DungeonGame(void);
		virtual ~CX2DungeonGame(void);
		
		virtual void			Release();
		virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT			OnFrameRender();

		virtual bool			MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		virtual void			GameLoading( CX2Room* pRoom );
		virtual void			GameStart();

		virtual void			StageLoading( int stageNum );
		virtual void			StageStart();

		virtual void			SubStageOpen( int subStageNum );
		virtual void			SubStageLoading( int subStageNum );
		virtual void			SubStageStart();

		CX2Dungeon*				GetDungeon() { return m_pDungeon; }		

		virtual bool			DetermineLastKill();	

		int						GetWinTeam() { return m_WinTeam; }
		void					SetWindTeam( CX2Room::TEAM_NUM winTeam ) { m_WinTeam = winTeam; }
        virtual					void ResurrectOtherUser();
		

		virtual bool			Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK );
		virtual bool			Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_REQ( int iClearConditionIndex );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK& kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK );
#endif // SERV_STAGE_CLEAR_IN_SERVER

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_OPEN_REQ( int subStageNum );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( KEGS_DUNGEON_SUB_STAGE_OPEN_ACK& kEGS_DUNGEON_SUB_STAGE_OPEN_ACK );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( KEGS_DUNGEON_SUB_STAGE_OPEN_NOT& kEGS_DUNGEON_SUB_STAGE_OPEN_NOT );

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ( int beforeStage, int nextStage );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK& kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT& kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT );

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ();
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK& kEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( KEGS_DUNGEON_SUB_STAGE_START_NOT& kEGS_DUNGEON_SUB_STAGE_START_NOT );

//		bool					StateChangeResultNot();
		bool					Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket );


		virtual bool			Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT );


		//virtual bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( UidType unitUID, bool bAllowUseCash = false );
		//virtual bool		    Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );
		virtual	bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT );

		virtual bool			Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT );

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
//{{ oasis907 : ����� [2010.5.6] // 
		bool					Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT& kEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT );
		bool					Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( KEGS_DUNGEON_EFFECT_TIME_OUT_NOT& kEGS_DUNGEON_EFFECT_TIME_OUT_NOT );

//}}
#endif DUNGEON_TIME_OUT_FRAMEMOVE
		bool					GetShowTutorialUI() { return m_bShowTutorialUI; }
		void					TextOutTutorial_LUA( int nMsg, float fTime=3.f );
		bool					GetIsThereTutorialMessage() { return m_bIsThereTutorialMsg; }		
		void					SetIsThereTutorialMessage(bool val) { m_bIsThereTutorialMsg = val; }
		bool					IsTutorialMessage_LUA( int nMsg )
		{
			if( 0 == m_wstrTutorialMsg.compare( GET_STRING( nMsg ) ) )
				return true;
			else 
				return false;
		}

		void					ShowSubStageOpenGuide();
		/*void					SetIsSendResurrectionReq( bool bCheck ) { m_bIsSendResurrectionReq = bCheck; }*/

#ifdef GET_DUNGEON_ID_IN_LUA
		int						GetDungeonID();
#endif GET_DUNGEON_ID_IN_LUA
		int						GetDungeonType();
		int						GetGameLevel() { return (int)m_pDungeonRoom->GetDifficulty(); }
		void					ClearSubStage( int clearType, int nextStageNum, int nextSubStageNum );
		
		
		// �Ʒü� ���� �Լ�
		bool 					GetStartRealTraining() const { return m_bStartRealTraining; }	
		void 					SetStartRealTraining(bool val);
		void					CreateTrainingGameUI( CX2State* pNowState );
		CX2TrainingGameUI*		GetTrainingGameUI() const { return m_pTrainingGameUI; }


		float					GetRemainPlayTime() const { return m_fRemainPlayTime; }
		void					SetRemainPlayTime(float val) { m_fRemainPlayTime = val; }


		bool 					GetEnableRemainTime() const { return m_bEnableRemainTime; }
		void 					SetEnableRemainTime(bool val) { m_bEnableRemainTime = val; }

		void					ShowBossName_LUA( char* pFileName );
		void					ShowBossName( const WCHAR* wszFileName );

		

		//bool					UseQuickSlot();
		void					SetHideDungeonPlayTimeAfterLoading(bool val) { m_bHideDungeonPlayTimeAfterLoading = val; }
		void					ResetUnitPositionAtTrainingGame();

		void					SetClearEffect( float fDelayTime ){ m_bShowClear = true; m_fClearEffDelayTime = fDelayTime; }

		CKTDXTimer*				GetTimerManyPurpose1() { return &m_TimerManyPurpose1; }
		CKTDXTimer*				GetTimerManyPurpose2() { return &m_TimerManyPurpose2; }

		bool					GetKilledBoss() const { return m_bKilledBoss; }

		// UNIT_TYPE �� �������� ������ ã���ֱ�..
		//bool					GetUserUnitListByUnitType( UNIT_TYPE unitType, std::vector<CX2GUUser*>& vecGUUserList );

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
		//{{ oasis907 : ����� [2010.4.26] // 
		void					CreateDamageDataForDungeonTimeOut();
		void					DungeonTimeOutFrameMove( double fTime, float fElapsedTime );
		//}}
#endif DUNGEON_TIME_OUT_FRAMEMOVE

		//{{ ����� : [2011/3/4/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
		float					GetPlayTimeLimit();
#endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/4/] //	���� �̼�
#ifdef COME_BACK_REWARD
		void					CloseDungeonName( float fTime );
#endif

		bool					IsNearPortalLineMap();
#ifdef MODIFY_DUNGEON_STAGING
		void					SetShowMonsterIndicator(bool bVal){ if( NULL != m_pMonsterIndicator) m_pMonsterIndicator->SetShow(bVal);}
#endif //MODIFY_DUNGEON_STAGING

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		
		virtual bool			Send_EGS_DEFENCE_DUNGEON_WAVE_ID_NOT( const int iDungeonWaveID_ );	
		bool					Send_EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT ( const int iStageIndex_, const int iSubStageIndex_ );		// ������ �ش� ���� �������� �޺�, ��ũ�� ������ ������ �����Ѵ�.
		bool					Handler_EGS_BAD_ATTITUDE_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// �ҷ� ������ UID �� �����Ѵ�.		
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		virtual void AbuserUserCheck( const CX2GUUser* pCheckUser_ ) {}	/// ���������� �۵� ����
#endif	SERV_FIX_SYNC_PACKET_USING_RELAY

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		void CreateAllyEventMonster();
		void ReCreateAllyEventMonster( UidType EventOwnUnitUID );
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

	protected:
//{{ Protected Function Start
		void					DungeonLoading(bool bIsNpcLoad = false);

		
		void					ShowClearEffect();
		virtual void			UpdateUnitPointer();


		void					CountTimeSubStageFrameMove( double fTime, float fElapsedTime );
		void					TutorialFrameMove( double fTime, float fElapsedTime );
		
		bool					GetResurrectionOperationCond();

		bool					DetermineShowDungeonPlayTime();

#ifdef EXTRACT_ALL_DUNGEON_NPC_INFO
		void					ExtractAllDungeonNPCInfo_DevOnly( const char* szFileName );
#endif EXTRACT_ALL_DUNGEON_NPC_INFO
//}} Protected Function End
		
#ifdef X2TOOL
	public:
		void ChangeStage(int iNextStage, int iNextSubStage = 0);
#endif



	protected:
//{{ Protected Variable Start
		CX2DungeonRoom*             m_pDungeonRoom;	/// ���� ID�� Difficulty�� ��°� ����� X2Game�� Room�� ��� �ϸ� �Ǳ� ������ ��� �� ��...
		CX2Dungeon*					m_pDungeon;	/// �׳� �ν��Ͻ��� �ص� ����...

		CX2Room::TEAM_NUM			m_WinTeam;	/// X2PVPGame���� ����

		bool						m_bGoNextPacketSended;	/// EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ �� ������ Ÿ�̹��� üũ�� �뵵�� ���
		
		bool						m_bShowClear;		/// Clear ����Ʈ�� ������ ���ΰ�?
		float						m_fClearEffDelayTime;	/// �����شٸ� ���� �Ŀ� ������ ���ΰ�?

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE	// oasis907 : ����� [2010.4.26] // 
		bool							m_bShowDungeonClearLimitTimeOut; /// �ð� ���� �������� Ÿ�̸Ӹ� ǥ������ // FieldFix: �ʱ�ȭ�� �ȵǾ� ����
		float							m_fDungeonClearLimitTimeOut;	/// �����ð� ��¿�
		bool							m_bDungeonClearLimitTimeOut; /// �ð� ���� �������� ���� �ð��� �Ѿ����
		CX2DamageManager::DamageData*	m_pDamageDataForDungeonTimeOut;	/// �ð� ������ �Ѱ��� �� ������ �� ������
#endif DUNGEON_TIME_OUT_FRAMEMOVE
		bool						m_bCountTimeLeftAfterSubStageOpen;	/// ���꽺������ ���� ��� ���� �ð��� ������ ������ ����
		float						m_fTimeLeftAfterSubStageOpen;		/// ���꽺������ ���� ��� ���� �ð� (���� 20��)
		CKTDGFontManager::CUKFont*	m_pFontForTime;		// ���� ��������, ���� Ÿ�� �ƿ��� �ð� ǥ�ÿ�
		//CKTDGFontManager::CUKFont*	m_pFontForQuest;

		bool						m_bIsSendSkipContinueNot;	/// ������
		
		// Ʃ�丮��
		bool						m_bIsThereTutorialMsg;		/// Ʃ�丮�� �޽����� �ִ��� ����
		CKTDGFontManager::CUKFont*	m_pFontForTutorialMsg;		/// Ʃ�丮�� �޽����� ��Ÿ���µ� ���
		wstring						m_wstrTutorialMsg;			/// Ʃ�丮�� �޽���
		float						m_fTimeLeftForTutorialMessage;	/// Ʃ�丮�� �޽��� ���� �ð� (-1�̸� ��� ���̰� �Ǵ� ��...)
		D3DXCOLOR					m_coTutorialMsg;			/// Ʃ�丮�� �޽����� Į��
		D3DXCOLOR					m_coTutorialMsgIncrement;	/// Ʃ�丮�� �޽��� Į���� ���� ��
		int							m_iTutorialMsgFlashCount;	/// Ʃ�丮�� �޽����� ������ flash �ϴ°�ó�� count��� �Ǿ������� �����δ� �ѹ��ۿ� ���ϴ� ��... (fade out->in)
		bool						m_bShowTutorialUI;	// Ʃ�丮�� UI�� ������ ������ ����

		bool						m_bEnableRemainTime;	/// true�� m_fRemainPlayTime�� ���� ���� �ȴ�. (TrainingGame������ ���Ǵ� ��)
		float						m_fRemainPlayTime;		/// ���ѽð��� �ִ� ������ �����ð�..�������� �ִµ� �������� �ʴµ�, TrainingGame�� DefenceDungeon���� ���Ǵµ�(DungeonRoom�� ������� �������� ������� ���� ���� �ʿ��� ��..)

		/// �Ʒü� ���� (������ �ƷüҰ� ���µ�...), �����Ʒõ� TrainingGame�� ����ϰ� ������ �� �����ؾ� �� ��
		bool						m_bStartRealTraining;	/// �Ʒü� ���� REQ�� ��������� �÷���
		CX2TrainingGameUI*			m_pTrainingGameUI;		/// �Ʒü� UI

		bool						m_bShowDungeonRemainTime;	/// m_fRemainPlayTime�� ǥ���� ������.. (�ð� ������ �ִ� ������ ��� ǥ
		bool						m_bShowDungeonPlayTimeForQuest;		/// ��ϸ� �Ǵ� ���� �ð� ���� ����Ʈ�� ������ �ִ� ��� ���� ���� �ð��� ǥ��
		bool						m_bHideDungeonPlayTimeAfterLoading; /// �������� �ε��� ��� ������ ���� �÷��� �ð� ǥ������ �ʵ���
#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
		bool						m_bShowStageReaminTime;
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION

		CKTDXTimer					m_TimerManyPurpose1;	/// ��� ����(�����̵忡���� ����߾���)
		CKTDXTimer					m_TimerManyPurpose2;	/// ��� ����(�����̵忡���� ����߾���)

		CX2MonsterIndicator*		m_pMonsterIndicator;	/// ���������� ����ϱ�� ��

		bool						m_bKilledBoss;		/// ������ ������ �׿�����
		
#ifdef CHECK_SUB_STAGE_GO_NEXT
		KProtectedType<bool>		m_bCheckSubStageGoNext;	/// ��ŷ üũ��
#endif

#ifdef COME_BACK_REWARD
		CKTDGUIDialogType			m_pDLGTitle;	/// �޸麹��, ������ ����� ����ϰ� ������... �� ������ �ٲپ�� �ҵ�..
		float						m_fDungeonNameTime;	/// m_pDLGTitle ���ſ�...
#endif
	
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hBossName;

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		int							m_iDefenceDungeonWaveID;		// ����� �� ���꽺������( Wave )
#endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM




//}} Protected Variable End		

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	public:
		bool IsDungeonEscape() { return m_bEscape; }

	private:
		bool m_bEscape;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};
