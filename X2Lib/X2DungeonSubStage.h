#pragma once

class CX2DungeonSubStage
{
	public:
		enum CLEAR_TYPE
		{
			CT_SUB_STAGE = 0,
			CT_STAGE,
			CT_STAGE_FORCE,
			CT_STAGE_SECRET,
			CT_GAME,
			CT_FAILURE = 5, //�ؿ��� Ŭ���� ������ �޼��ϸ� ���� �Ǵ� Ŭ���� Ÿ�� 2013.07.03 ��â��
		};

		enum CLEAR_CONDITION
		{
			CC_KILL_ALL_ACTIVE_NPC,
			CC_KILL_KEYCODE_NPC,
			CC_KILL_KEYCODE_NPC_ANY,
			CC_KILL_NPC_TYPE,
			CC_KILL_KEYCODE_NPC_COUNT,
			CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC,
			CC_TIME,
		};

		enum GO_TYPE
		{
			GT_NONE = 0,
			GT_RIGHT,
			GT_LEFT,
			GT_UP,
			GT_DOWN,
		};

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
		enum SECRET_STAGE_PAD
		{
			SSP_NONE	= 0,
			SSP_SECRET	= 1,
			SSP_ELITE	= 2,
			SSP_NORMAL	= 3,
		};
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef X2TOOL
		enum MONSTER_GRADE
		{
			MG_NORMAL_NPC				= 0,
			MG_BOSS_NPC					= 1,
			MG_MIDDLE_BOSS_NPC			= 2,
#ifdef SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM// �۾���¥: 2013-08-28	// �ڼ���
			MG_DEFINITELY_DROP_HP_BALL	= 3,
#endif // SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM
		};
#endif

		struct NextStageData
		{
			CLEAR_TYPE m_eClearType;
			int m_iStageIndex;
			int m_iSubStageIndex;
			int m_iRate;

			int m_iDungeonEndingSpeechTableIndex;	// clear type == CT_GAME�� ��쿡�� ��ȿ
		};


		struct ClearCondtionData
		{
			struct MonsterCount
			{
				int m_iKey;				// could be npckeycode, or npcid
				int m_iOriginalCount;	// substage �����Ҷ� ������ ��
				int m_iObjectiveCount;	// ��ǥ ų �� - note!! �ϴ� ������ CC_KILL_KEYCODE_NPC_COUNT�� ��쿡�� ���

				MonsterCount()
				{
					m_iKey = 0;
					m_iOriginalCount = 0;
					m_iObjectiveCount = 0;
				}
			};

			bool						m_bEnable;
			bool						m_bChecked;				// �� clear cond check�� �����ؼ� �̹� ó���Ǿ���
			CLEAR_CONDITION				m_eClearCondition;
			std::vector<MonsterCount>	m_vecNPCKeyCode;
			std::vector<MonsterCount>	m_vecNPCID;
			std::vector<int>			m_vecPortalLineIndex;
			bool						m_bCreatedPortalEffect;		// CC_PORTAL_AFTER_KILL_ALL_ACTIVE_NPC�� ��쿡 ���� �� ������ ��Ż ��ġ ǥ���ϱ� ���ؼ� 
			float						m_fClearTime;
			std::vector<NextStageData>	m_vecNextStage;
			float						m_fDelayTimeClearSubStage;

		public:
			ClearCondtionData()
			{
				m_bEnable = true;
				m_bChecked = false;
				m_bCreatedPortalEffect = false;
				m_fClearTime = -1.f;
				m_eClearCondition = CC_KILL_ALL_ACTIVE_NPC;
				m_fDelayTimeClearSubStage = 0.0f;
			}
		};

		struct CurtainData
		{
			int			m_LineIndex;
			bool		m_bAtStart;
			bool		m_bLookLeft;
		};	

		struct NPCData
		{
			int								m_UID;
			CX2UnitManager::NPC_UNIT_ID		m_UnitID;
			int								m_KeyCode;
			bool							m_bFocusCamera;

			std::vector<int>				m_vecStartPos;
			int								m_StartPos;

			bool							m_bActive;
			bool							m_bShowGage;
			D3DXVECTOR3						m_AddPos;

			int								m_Level;
			//{{ 2007. 9. 6  ������  
			int								m_Rate;
			//}}			

			bool							m_bHasBossGage;
			bool							m_bShowBossName;
			bool							m_bShowSubBossName;
			bool							m_bSiegeMode;
			//{{ 2007. 10. 26  ������  
			bool							m_bIsRight;
			//}}

			//{{ 2007. 9. 6  ������  
			bool							m_bSubNpc;
#ifdef X2TOOL
			int								m_iLevel4Tool;
			int								m_iSubNPCIndex;
			int								m_iSubNPCRate;

			MONSTER_GRADE					m_MonsterGrade;
#endif
			//}}		

			bool							m_bNoDrop;
			float							m_fUnitScale;

			//{{ JHKang / ������ / 2011/01/26 / ���� HP �� ���� ���� ����
#ifdef SERV_BOSS_GAUGE_HP_LINES
			u_short							m_usBossGaugeHPLines;			
#endif SERV_BOSS_GAUGE_HP_LINES
			//{{ JHKang / ������ / 2011/01/26 / ���� HP �� ���� ���� ����
			//{{ 2010. 03. 29  �տ���	���� �� ����
#ifdef SERV_ALLY_NPC
			bool							m_bAllyNpc;
#endif SERV_ALLY_NPC
			//}}
#ifdef MODIFY_DUNGEON_STAGING
			float							m_fShowBossNameDelayTime;
			bool							m_bStopAtStartState;
#endif //MODIFY_DUNGEON_STAGING


#ifdef X2TOOL
			NPCData( CX2UnitManager::NPC_UNIT_ID unitId = CX2UnitManager::NUI_NONE )
			{
				m_UID = 0;
				m_UnitID = unitId;
				m_KeyCode = 0;
				m_bFocusCamera = false;
				m_StartPos = -1;
				m_bActive = true;
				m_bShowGage = true;
				m_AddPos = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_iLevel4Tool = 0;
				m_Level = 0;
				m_Rate = 100;
				m_bHasBossGage = false;
				m_bShowBossName = false;
				m_bShowSubBossName = false;
				m_bSiegeMode = false;
				m_bIsRight = false;
				m_bSubNpc = false;
				m_iSubNPCIndex = -1;
				m_iSubNPCRate = 100;
				m_bNoDrop = false;
				m_fUnitScale = -1.f;
				m_usBossGaugeHPLines = 1;
				m_bAllyNpc = false;
				m_MonsterGrade = CX2DungeonSubStage::MG_NORMAL_NPC;
			}
#endif

		};

		struct Trigger
		{
			bool		m_bHost;
			bool		m_bOneTime;
			bool		m_bReacted;
			float		m_Interval;
			float		m_NowInterval;
			string		m_ConditionFunc;
			string		m_ReactFunc;

#ifdef X2TOOL
			Trigger()
			{
				m_bHost = false;
				m_bOneTime = true;
				m_bReacted = false;
				m_Interval = 0.f;
				m_NowInterval = 0.f;
				m_ConditionFunc = "";
				m_ReactFunc = "";
			}
#endif
		};

		struct SubStageData 
		{
			int						m_StartLineSet;
			int						m_EndLineSet;
			int						m_MainLineSet;
			vector<CurtainData*>	m_CurtainDataList;
			vector<NPCData*>		m_NPCDataList;
			vector<Trigger*>		m_TriggerList;
			//vector<CX2GUNPC*>		m_NPCList;

			CX2DungeonSubStage::GO_TYPE	m_GoType;
			vector<ClearCondtionData>	m_vecClearCondData;

			map < int, string >	m_mapKeyCodeNStartState;


			typedef std::vector< wstring > SecretStageEnteringSpeech;
			SecretStageEnteringSpeech m_SecretStageEnteringSpeech;

			CX2UnitManager::NPC_UNIT_ID m_eSecretStageNPCID;
			
//{{ kimhc // 2010.8.10 // Ư�� ������ ��ġ ���� ��Ȱ ��ġ�� ���
			bool					m_bUsePosOfMonsterForRebirth;
			int						m_iKeyCodeOfMonsterForRebirth;
//}} kimhc // 2010.8.10 // Ư�� ������ ��ġ ���� ��Ȱ ��ġ�� ���

			SubStageData()
				: m_StartLineSet( -1 )
				, m_EndLineSet( -1 )
				, m_MainLineSet( 0 )
				, m_CurtainDataList()
				, m_NPCDataList()
				, m_TriggerList()
				//, m_NPCList()
				, m_GoType( GT_NONE )
				, m_vecClearCondData()
				, m_mapKeyCodeNStartState()
				, m_SecretStageEnteringSpeech()
				, m_eSecretStageNPCID( CX2UnitManager::NUI_NONE )
				, m_bUsePosOfMonsterForRebirth( false )
				, m_iKeyCodeOfMonsterForRebirth( -1 )
			{
			}

			~SubStageData()
			{
				for( int i = 0; i < (int)m_CurtainDataList.size(); i++ )
				{
					CurtainData* pCurtainData = m_CurtainDataList[i];
					SAFE_DELETE( pCurtainData );
				}
				m_CurtainDataList.clear();

				for( int i = 0; i < (int)m_NPCDataList.size(); i++ )
				{
					NPCData* pNPCData = m_NPCDataList[i];
					SAFE_DELETE( pNPCData );
				}
				m_NPCDataList.clear();

				for( int i = 0; i < (int)m_TriggerList.size(); i++ )
				{
					Trigger* pTrigger = m_TriggerList[i];
					SAFE_DELETE( pTrigger );
				}
				m_TriggerList.clear();

// 				for( int i = 0; i < (int)m_NPCList.size(); i++ )
// 				{
// 					CX2GUNPC* pCX2GUNPC = m_NPCList[i];
// 					SAFE_DELETE_KTDGOBJECT( pCX2GUNPC );
// 				}
// 				m_NPCList.clear();
			}

			bool LoadData( KLuaManager& luaManager, bool bIsNpcLoad = false );
			//void CreateNPC();
			void AddNPCData( KNPCUnitReq& kNPCUnitReq );

			//{{ kimhc // 2010.8.10 // Ư�� ������ ��ġ ���� ��Ȱ ��ġ�� ���
			bool GetUsePosOfMonsterForRebirth() const { return m_bUsePosOfMonsterForRebirth; }
			void SetUsePosOfMonsterForRebirth(bool val) { m_bUsePosOfMonsterForRebirth = val; }
			int	 GetKeyCodeOfMonsterForRebirth() const { return m_iKeyCodeOfMonsterForRebirth; }
			void SetKeyCodeOfMonsterForRebirth(int val) { m_iKeyCodeOfMonsterForRebirth = val; }
			//}} kimhc // 2010.8.10 // Ư�� ������ ��ġ ���� ��Ȱ ��ġ�� ���

		private:
			void LoadCurtain( KLuaManager& luaManager );
			void LoadNPCData( KLuaManager& luaManager );			
			void LoadTrigger( KLuaManager& luaManager );
			void LoadSecretStageEnteringSpeech( KLuaManager& luaManager );


			void FetchNPCData( KLuaManager& luaManager, CX2DungeonSubStage::NPCData* pNPCData );
			
#ifdef X2TOOL
		public:
			vector<NPCData*> m_vecSubStageNpcData;
			bool FetchNPCData4Tool( KLuaManager& luaManager, NPCData *pNPCData );
			void LoadNpcData4Tool( KLuaManager& luaManager );
			vector<CX2DungeonSubStage::NPCData*> SummonNpcData4Tool();
#endif
		};

	public:
		CX2DungeonSubStage( int subStageIndex, SubStageData* pSubStageData );
		~CX2DungeonSubStage(void);

		void				Start()
		{ 
			m_bStart = true; 
			ToggleStartLineMapSet(true); 
			ToggleMainLineMapSet(true); 
			ToggleEndLineMapSet(true);

			dbg::clog << L"SubStageStart! Index: " << m_SubStageIndex << dbg::endl;
		}

		void				OnFrameMove( double fTime, float fElapsedTime );

		
		void				CurtainFrameMove( double fTime, float fElapsedTime );

		void				ToggleStartLineMapSet( bool bToggle );
		void				ToggleEndLineMapSet( bool bToggle );
		void				ToggleMainLineMapSet( bool bToggle );
		
		bool				TouchStartLineMapSet( int lineNum );
		bool				TouchEndLineMapSet( int lineNum );
		bool				TouchMainLineMapSet( int lineNum );

		SubStageData*		GetSubStageData(){ return m_pSubStageData; }
		int					GetSubStageIndex(){ return m_SubStageIndex; }
		
		void				SetMonsterCountForSubStageClearCheck();
#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
		void				PrepareClearSubStage( int iClearConditionIndex );
#endif // SERV_STAGE_CLEAR_IN_SERVER
		void				ClearSubStage( int clearType, int nextStageNum, int nextSubStageNum );
		void				CheckIfSubStageCleared();
		void				CheckIfSubStageCleared_NotHost();
#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
		float				GetRemainTimeInClearCondition();
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION


		void				DeleteAllNPCUnit();

		static void			DeleteAllNPCUnit( const vector<NPCData*>& vecNPCData_ );

		void				SwapToGameNPCUnit();
		bool				IsNPCLoadComplete();

		void				CreateCurtain();
		void				DeleteCurtain();
		void				BreakCurtain();
		void				OpenCurtain();

		CX2Curtain*			GetCurtain( int index ) { return m_CurtainList[index]; }
		int					GetCurtainNum() { return (int)m_CurtainList.size(); }

		bool 				GetWinWhenTimeOut() const { return m_bWinWhenTimeOut; }
		void 				SetWinWhenTimeOut(bool val) { m_bWinWhenTimeOut = val; }

		bool				GetStarted() const { return m_bStart; }

		float				GetEndGameDelay() const { return m_fEndGameDelay; }
		void				ShowBossNameAtStart( float fElapsedTimeSubStage );
		float				GetSubStageTime(){ return m_fElapsedTimeSubStage; }

		bool				GetIsBossStage() const { return m_bIsBossStage; }
		bool				GetBossStageHaveManyNextStage() const { return m_bBossStageHaveManyNextStage; }

		bool				GetCheckedClearSubStage() const { return m_bCheckedClearSubStage; }

		void				SetEnableClearCondition( int iIndex, bool bEnable );

		bool				GetDoneClearSubStage() const { return m_bDoneClearSubStage; }


#ifdef HENIR_TEST
		int GetNextSecretStageIndex() const { return m_iNextSecretStageIndex; }
		int GetNextNormalStageIndex() const { return m_iNextNormalStageIndex; }
#endif HENIR_TEST
#ifdef NEW_HENIR_TEST
		int GetNextEliteStageIndex() const { return m_iNextEliteStageIndex; }
#endif NEW_HENIR_TEST
#ifdef DELETE_NPC_BY_SUB_STAGE_END
		void DeleteNPCBySubStageEnd();
#endif DELETE_NPC_BY_SUB_STAGE_END

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG
		char GetClearConditionCheckCount() const { return m_cClearConditionCheckCount; }
		void SetClearConditionCheckCount(char val) { m_cClearConditionCheckCount = val; }
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

		void				ClearDungeonGame();

		static void CreateAndAppendGUNPC( std::vector<CX2GUNPC*>& vecInOutNPCs_, const vector<NPCData*>& vecNPCData_ );

		static CX2GUNPC*    CreateGUNPC( const NPCData* pNpcData_ );

#ifdef DYNAMIC_PORTAL_LINE_MAP
		void				ProcessAfterSubStageClear_NotHost();
#endif // DYNAMIC_PORTAL_LINE_MAP
	protected:		
		void				ProcessTrigger();

#ifdef DYNAMIC_PORTAL_LINE_MAP
		void				ChangeLineTypeAfterStageClear();
#endif // DYNAMIC_PORTAL_LINE_MAP

		
	
		SubStageData*		m_pSubStageData;
		int					m_SubStageIndex;
		float				m_fElapsedTime;

		bool				m_bStart;	
		
		int					m_LoadNPCIndex;

		vector<CX2Curtain*>	m_CurtainList;

		float				m_fEndGameDelay;

		bool				m_bWinWhenTimeOut;			// �Ʒü� ������
		bool				m_bBossNameWasShown;

		float				m_fElapsedTimeSubStage;		// substage�� ���۵� �ķ� �帥 �ð�
		bool				m_bDoneClearSubStage;		// ClearSubStage() �Լ��� ȣ���ߴ���
		
		bool				m_bIsBossStage;
		bool				m_bBossStageHaveManyNextStage;	// 2�� �̻��� next stage data�� �����Ѵ�
		
		bool				m_bCheckedClearSubStage;		// clear condition üũ �Ϸ��ϰ� ���� ���������� �Ѿ�� ���� ��� ���		
		CKTDXTimer			m_TimerDelayClearSubstage;		// clear condition�� ���������� ��� ������ ���ؼ� substage Ȥ�� stage �Ѿ�� ���� ��ٸ���.
		float				m_fDelayTimeClearSubstage;

		bool				m_bCheckedClearSubStage_NotHost;	// ���� �ƴ� Ŭ���̾�Ʈ�� ����Ʈ ǥ�ø� ���ؼ� ���� �������� Ŭ���� ó�� �Ǿ����� üũ�ϵ��� �ϱ� ���ؼ�


#ifdef HENIR_TEST
		int m_iNextSecretStageIndex;	// ��� ���������� �ִ� ��쿡 �������� ��ȣ�� �����ϱ� ���� ����
		int m_iNextNormalStageIndex;		
#endif HENIR_TEST
#ifdef NEW_HENIR_TEST
		int m_iNextEliteStageIndex;
#endif NEW_HENIR_TEST

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG
		char				m_cClearConditionCheckCount;
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

		CKTDGParticleSystem::CParticleEventSequenceHandle m_hBossDanger;
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hBossDangerAdd;

};


