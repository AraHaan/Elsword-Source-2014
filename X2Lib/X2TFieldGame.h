#pragma once

class CX2TFieldGame
{
	public:
		enum MARKET_LIST_UI_MSG
		{
			MLUI_CLOSE		= 15100,
			MLUI_PREV,
			MLUI_NEXT,
			MLUI_REFRESH,
			MLUI_ENTER,
		};

#ifdef SERV_READY_TO_SOSUN_EVENT
		enum READY_TO_SOSUN_EVENT_UI_MSG
		{
			RTSEUM_EVENT_OK = 99100,
			RTSEUM_EVENT_CANCEL = 99101,
		};
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_MOMOTI_EVENT
		enum MOMOTI_EVENT_UI_MSG
		{
			SHOW_MOMOTI_URL_EVENT			= 99200,
			SHOW_MOMOTI_QUIZ_EVENT			= 99201,
			SHOW_MOMOTI_QUIZ_EVENT_OK		= 99202,
			SHOW_MOMOTI_QUIZ_EVENT_CANCEL 	= 99203,
			SHOW_MOMOTI_QUIZ_EVENT_ENTER	= 99204,
#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
			SHOW_MOMOTI_QUIZ_EVENT_ADDQUIZ  = 99205,
#endif //SERV_MOMOTI_EVENT_ADDQUIZ
		};
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		enum RELATIONSHIP_EVENT_UI_MSG
		{
			REUM_OK = 99110,
			REUM_CANCEL = 99111,
		};
#endif SERV_RELATIONSHIP_EVENT_INT

		struct SquareSlot
		{
			UidType				m_iSquareUID;
			CKTDGUIDialogType		m_pDLGSquareSlot;

			int					m_JoinNum;
			int					m_JoinMaxNum;

			SquareSlot()
			{
				m_iSquareUID		= -1;
				m_pDLGSquareSlot	= NULL;

				m_JoinNum			= 0;
				m_JoinMaxNum		= 0;
			}

			~SquareSlot()
			{
				SAFE_DELETE_DIALOG( m_pDLGSquareSlot );
			}
		};

		struct SquareUI
		{
			CKTDXStage*					m_pStage;
			CKTDGUIDialogType				m_pDLGSquareListBG;
			CKTDGUIDialogType				m_pDLGCreateSquare;
			CKTDGUIDialogType				m_pDLGSquareTypeCombo;

			vector<SquareSlot*> 		m_vecpSquareSlot;

			int							m_iNowPage;
			int							m_iMaxPage;
			CX2SquareGame::SQUARE_TYPE	m_eSquareType;

			int							m_iNowIndex;
			bool						m_bShow;

			SquareUI( CKTDXStage* pStage );
			~SquareUI();

			void CreateSlotDialogs();
			void DeleteSlotDialogs();
			void CloseSquareListDLG();
			void OpenSquareListDLG();

			void SetShow( bool bShow );
			bool GetShow() { return m_bShow; }

			void UpdateSquareSlotList( KEGS_SQUARE_LIST_ACK& kEvent );
			//void CloseCreateSquareDLG();
			//void OpenCreateSquareDLG();

			CX2SquareGame::SQUARE_TYPE GetSquareType() const { return m_eSquareType; }
			void SetSquareType(CX2SquareGame::SQUARE_TYPE val) { m_eSquareType = val; }

			SquareSlot* GetSlotSquareSlot( UidType iSquareUID );
			void SetPos(D3DXVECTOR2 vPos);

			void SetFocus(int iFocus);
			bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		};


		struct SquareData
		{
			UidType		m_UID;
			wstring		m_SquareName;
			int			m_MaxUser;
			int			m_NowUser;

			wstring		m_RelayIP;
			int			m_Port;

			SquareData()
			{
				m_UID			= 0;
				m_MaxUser		= 0;
				m_NowUser		= 0;
				m_Port			= 0;
			}
			void CopySquareData( SquareData* pSquareData )
			{
				if( pSquareData != NULL )
				{
					m_UID			= pSquareData->m_UID;
					m_SquareName	= pSquareData->m_SquareName;
					m_MaxUser		= pSquareData->m_MaxUser;
					m_NowUser		= pSquareData->m_NowUser;
					m_RelayIP		= pSquareData->m_RelayIP;
					m_Port			= pSquareData->m_Port;
				}				
			}
			void Set_KSquareInfo( KSquareInfo* pKSquareInfo )
			{
				if( pKSquareInfo != NULL )
				{
					m_UID			= pKSquareInfo->m_iSquareUID;
					m_SquareName	= pKSquareInfo->m_wstrRoomName;
					m_MaxUser		= pKSquareInfo->m_MaxSlot;
					m_NowUser		= pKSquareInfo->m_JoinSlot;
					m_RelayIP		= pKSquareInfo->m_RelayIP;
					m_Port			= pKSquareInfo->m_Port;
				}				
			}
		};

		//struct GarbageAnim
		//{
		//	vector<CKTDGXSkinAnim*>		m_GarbageAnim;

		//	~GarbageAnim()
		//	{
		//		for( int i = 0; i < (int)m_GarbageAnim.size(); i++ )
		//		{
		//			CKTDGXSkinAnim* pCKTDGXSkinAnim = m_GarbageAnim[i];
		//			SAFE_DELETE( pCKTDGXSkinAnim );
		//		}
		//		m_GarbageAnim.clear();
		//	}
		//};

		// npc ����� ����
		struct NpcJoinShadow
		{
			bool						m_bStart;
			bool						m_bEnd;
			float						m_fTimer;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcShadowTop;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNpcShadowBottom;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticleHandle				m_hNpcShadowTopParticle;			
			CKTDGParticleSystem::CParticleHandle				m_hNpcShadowBottomParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle*						m_pNpcShadowTopParticle;			
			CKTDGParticleSystem::CParticle*						m_pNpcShadowBottomParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			HRESULT OnFrameMove ( double fTime, float fElapsedTime );

			NpcJoinShadow();
			~NpcJoinShadow();

			void OpenShadow();
			void CloseShadow();
		};
//#ifdef MODIFY_PORTAL_GATE
		struct PortalGate
		{
			// ���� ��Ż����Ʈ
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate1;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate2;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate3;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate4;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate5;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticleHandle				m_hPortalGateParticle1;
			CKTDGParticleSystem::CParticleHandle				m_hPortalGateParticle2;
			CKTDGParticleSystem::CParticleHandle				m_hPortalGateParticle3;
			CKTDGParticleSystem::CParticleHandle				m_hPortalGateParticle4;
			CKTDGParticleSystem::CParticleHandle				m_hPortalGateParticle5;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle*						m_pPortalGateParticle1;
			CKTDGParticleSystem::CParticle*						m_pPortalGateParticle2;
			CKTDGParticleSystem::CParticle*						m_pPortalGateParticle3;
			CKTDGParticleSystem::CParticle*						m_pPortalGateParticle4;
			CKTDGParticleSystem::CParticle*						m_pPortalGateParticle5;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			PortalGate():
			m_hPortalGate1(INVALID_PARTICLE_SEQUENCE_HANDLE),
			m_hPortalGate2(INVALID_PARTICLE_SEQUENCE_HANDLE),
			m_hPortalGate3(INVALID_PARTICLE_SEQUENCE_HANDLE),
			m_hPortalGate4(INVALID_PARTICLE_SEQUENCE_HANDLE),
			m_hPortalGate5(INVALID_PARTICLE_SEQUENCE_HANDLE),
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_hPortalGateParticle1(INVALID_PARTICLE_HANDLE),
			m_hPortalGateParticle2(INVALID_PARTICLE_HANDLE),
			m_hPortalGateParticle3(INVALID_PARTICLE_HANDLE),
			m_hPortalGateParticle4(INVALID_PARTICLE_HANDLE),
			m_hPortalGateParticle5(INVALID_PARTICLE_HANDLE)
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pPortalGateParticle1(NULL),
			m_pPortalGateParticle2(NULL),
			m_pPortalGateParticle3(NULL),
			m_pPortalGateParticle4(NULL),
			m_pPortalGateParticle5(NULL)
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{}
		};
//#endif //MODIFY_PORTAL_GATE
	public:
		CX2TFieldGame( CX2World::WORLD_ID worldID, SquareData* pSquareData = NULL );
		virtual ~CX2TFieldGame(void);		

		void						SetShowField(bool val);

		void						SetAddedMyUnit(bool val) { m_bAddMyUnit = val; }
		void						JoinFieldUnit( KFieldUserInfo* pKFieldUserInfo );
		CX2SquareUnit*				AddUnit( CX2Unit* pUnit, bool bMyUnit, bool bInit );
		void						RemoveUnit( UidType unitUID );

//{{ robobeg : 2011-02-18

#ifdef ADDED_RELATIONSHIP_SYSTEM		
		bool                        AddUnitToDeviceList( CX2Unit* pUnit, const D3DXVECTOR3& vPos, UidType uidParty, UidType uidRelation = 0 );
#else // ADDED_RELATIONSHIP_SYSTEM
		bool                        AddUnitToDeviceList( CX2Unit* pUnit, const D3DXVECTOR3& vPos, UidType uidParty );
#endif // ADDED_RELATIONSHIP_SYSTEM

		
		//}} robobeg : 2011-02-18
		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnFrameRender();
		virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		CX2Camera*					GetCamera(){ return m_pCamera; }
		CX2SquareUnit*				GetMyUnit() const { return m_pMyUnit.get(); }
#ifdef HANABI_VILLAGE
		CX2SquareUnitPtr			GetMyUnitPtr() const { return m_pMyUnit; }
#endif //HANABI_VILLAGE
		int							GetSquareUnitNum(){ return m_UserUnitList.size(); }
		CX2SquareUnit*				GetSquareUnit( int index );
		CX2SquareUnit*				GetSquareUnitByUID( UidType UID );
		CX2SquareUnit*				GetSquareUnitByNickName( const WCHAR* wszNickName );
#ifdef SERV_INVISIBLE_GM
		bool						IsInvisible( UidType UID ) const;
#endif SERV_INVISIBLE_GM

//{{ robobeg : 2011-02-18
		CX2Unit*				    GetSquareUnitUnitByUID( UidType UID );
		CX2Unit*				    GetSquareUnitUnitByNickName( const WCHAR* wszNickName );
//}} robobeg : 2011-02-18

		vector<CX2SquareUnit*>		GetUnitList( int& iPage, int iUnitNumPerPage = 10 );
		void						SetShowUserUnit( bool bShow );

		//void						SetEndableKey(bool val) { m_bEnableKey = val; }
		void 						SetEnableKeyProcess(bool val) { m_bEnableKeyProcess = val; }
		void 						SetEnableCameraProcess(bool val) { m_bEnableCameraProcess = val; }

		const CX2TFieldGame::SquareData& GetSquareData() const { return m_SquareData; }

		CX2World*					GetWorld(){ return m_pWorld; }
		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }		

		// �����Ʒ�
		bool						Handler_EGS_CREATE_TC_ROOM_REQ( int iTrainingID );
		bool						Handler_EGS_CREATE_TC_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void						Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( KEGS_SQUARE_UNIT_SYNC_DATA_NOT& kEGS_SQUARE_UNIT_SYNC_DATA_NOT );
		bool						Handler_EGS_JOIN_SQUARE_NOT( KEGS_JOIN_SQUARE_NOT& kEGS_JOIN_SQUARE_NOT );
		bool						Handler_EGS_LEAVE_SQUARE_NOT( KEGS_LEAVE_SQUARE_NOT& kEGS_LEAVE_SQUARE_NOT );
		bool						Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT( KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT& kEGS_CHANGE_EQUIPPED_ITEM_NOT );
		

		void						Handler_EGS_SQUARE_LIST_REQ();
		bool						Handler_EGS_SQUARE_LIST_REQ(int iPage);
		bool						Handler_EGS_SQUARE_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool						Handler_EGS_JOIN_SQUARE_REQ( UidType iSquareUID );
		bool						Handler_EGS_JOIN_SQUARE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool						Handler_EGS_OPEN_PVP_ROOM_LIST_REQ();
		bool						Handler_EGS_OPEN_PVP_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		

		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ������, �ǽð� ������ ����
#ifdef	REAL_TIME_ELSWORD
		bool						Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ������, �ǽð� ������ ����

		//{{ kimhc // 2009-12-08 // ũ�������� Ʈ�� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
		bool Handler_EGS_CHECK_TIME_EVENT_COMPLETE_REQ();
		bool Handler_EGS_CHECK_TIME_EVENT_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-08 // ũ�������� Ʈ�� �̺�Ʈ
#ifdef WEB_POINT_EVENT	// oasis907 : ����� [2010.2.24] // 
		bool						Handler_EGS_ATTENDANCE_CHECK_REQ();
		bool						Handler_EGS_ATTENDANCE_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	
#endif WEB_POINT_EVENT

#ifdef SERV_INVISIBLE_GM
		bool						Handler_EGS_TOGGLE_INVISIBLE_NOT( KEGS_TOGGLE_INVISIBLE_NOT& kEGS_TOGGLE_INVISIBLE_NOT );
#endif SERV_INVISIBLE_GM

#ifdef SERV_READY_TO_SOSUN_EVENT
		bool						Handler_EGS_READY_TO_SOSUN_EVENT_REQ();
		bool						Handler_EGS_READY_TO_SOSUN_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_MOMOTI_EVENT
		void SetShowMomotiQuizEvent();
#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
		void SetShowMomotiQuizEventAddQuiz();
#endif //SERV_MOMOTI_EVENT_ADDQUIZ
		void SetShowMomotiQuizReplyEvent();
		bool Handler_EGS_MOMOTI_QUIZ_EVENT_REQ();
		bool Handler_EGS_MOMOTI_QUIZ_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		bool						Handler_EGS_EVENT_PROPOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool						Send_EGS_EVENT_PROPOSE_AGREE_NOT( CX2RelationshipManager::PROPOSE_RETURNED_MESSAGE eAgreeCouple);
		bool						Handler_EGS_EVENT_PROPOSE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool						Handler_EGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RELATIONSHIP_EVENT_INT

		// 1.5�ʽ�ũ ó��
		bool						ReceiveSyncBroadNot(KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT& syncBroadNot);

		void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
			const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

		CKTDGParticleSystem*		GetUiParticle()				{ return m_pUiParticle; }
		CKTDGXMeshPlayer*			GetUiXMeshPlayer()			{ return m_pUiXMeshPlayer; }
		//CKTDGParticleSystem*		GetMajorParticle()			{ return m_pMajorParticle; }
		//CKTDGParticleSystem*		GetMinorParticle()			{ return m_pMinorParticle; }
		//CKTDGXMeshPlayer*			GetMajorXMeshPlayer()		{ return m_pMajorXMeshPlayer; }
		//CKTDGXMeshPlayer*			GetMinorXMeshPlayer()		{ return m_pMinorXMeshPlayer; }

		void						SetFreeCamera( bool bFreeCamera );

		CX2TalkBoxManager*			GetTalkBoxManager() { return m_pTalkBoxManager; }
		
		bool ConnectTR();
		
		int GetVillageStartPosIdToBattleField()	const { return m_iVillageStartPosIdToBattleField; }
		void SetVillageStartPosIdToBattleField( int iFieldIdToMove_ ) { m_iVillageStartPosIdToBattleField = iFieldIdToMove_; }

		bool IsNearPortalToBattleField() const { return m_bNearPortalToBattleField; }
		void SetNearPortalToBattleField( const bool bNearPortalToBattleField_ ) { 
			m_bNearPortalToBattleField = bNearPortalToBattleField_; }
		
		void AddFieldNpc();			
//#ifdef MODIFY_PORTAL_GATE
		void CreatePortalGate();
		void SetShowPortalGate(bool bShow, UINT uiPortalIndex, D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f));
//#else
//		void CreatePortalGate();
//		void SetShowPortalGate(bool bShow, D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f));
//#endif //MODIFY_PORTAL_GATE

		void ByeNpc();
		bool TalkNpc();
		bool TalkNpc(int iNpc);
		bool GetTalkNpc() { return m_bJoinNpc; }
		bool GetJoinNpc();
		int	 GetJoinNpcId();
		int	 GetJoinNpcIndex();

		void SetShowNpcMessage(bool val);
		void ShowJoinMarket(bool val, D3DXVECTOR3 vPos);
		
		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void DeleteWorld() { SAFE_DELETE(m_pWorld); }
		bool GetEnableKey();
		void SetStage(CKTDXStage *pStage);		

		int GetLastPos() { return m_iLastPos; }
		void OpenSubAni();

		NpcJoinShadow* GetNpcShadow() { return m_pNpcShadow; }


		CX2TFieldNpc* GetFieldNPC( int index )
		{
			if( index< 0 || index >= (int) m_vecFieldNpc.size() )
				return NULL;

			return m_vecFieldNpc[index].get();
		}

		//{{ JHKang / ������ / 2011.8.28 / ���� GetFieldNPC�� ���� �ʵ��� NPC Size�� ����
#ifdef TALK_BOX_VILLAGE_NPC
		CX2TFieldNpc* GetHouseFieldNPC( IN int iHouseID_ )
		{
			int iSize = m_vecFieldNpc.size();

			for ( int i = 0; i < iSize; ++i )
			{
				if ( m_vecFieldNpc[i]->m_NpcHouseID == iHouseID_ )
					return m_vecFieldNpc[i].get();
			}

			return NULL;
		}
#endif

		int GetFieldNPCCount()
		{
			return (int) m_vecFieldNpc.size();
		}
		
		void ToggleEnableNPC()
		{
			for( UINT i=0; i < m_vecFieldNpc.size(); i++ )
			{
				CX2TFieldNpcPtr pFieldNPC = m_vecFieldNpc[i];
				if( pFieldNPC != NULL )
				{
					pFieldNPC->SetEnable( !pFieldNPC->GetEnable() );
				}
			}
		}

		void SetLoadComplete(bool val);
		bool GetShowMarketList() { return m_bShowMarketList; }
		void CloseMarketList();

		void SetQuestDesc(int npcId, const wstring &title, const wstring &message);
		void SetNpcMessage(int npcId, const wstring &npcMessage);		
		void ChangeNpcType(CX2TFieldNpcShop::NPC_SHOP_BUTTON_TYPE eType);

		//{{ dmlee 2009.1.14 �̴ϸʿ� ��Ż, ���� ��ġ ǥ�ø� ����
		int GetGatePortalCount() { return (int)m_vecLinkedPos.size(); }
		int GetMarketEnteranceCount() { return (int)m_vecMarketPos.size(); }
		const D3DXVECTOR3 GetGatePortalPos( int index );
		const D3DXVECTOR3 GetMarketEnterancePos( int index );
		//}} dmlee 2009.1.14 �̴ϸʿ� ��Ż, ���� ��ġ ǥ�ø� ����

		void UpdateMarketList(KEGS_JOIN_SQUARE_ACK &kEvent);

		bool GetInArea(int iHouseID);
		bool GetIsRightNpc(int iHouseID);
#ifndef REFORM_NOVICE_GUIDE
		CX2NoviceGuide* GetNoviceGuide() { return m_pNoviceGuide; }
#endif //REFORM_NOVICE_GUIDE
		void SetUserHide(bool val) { m_bUserHide = val; }

		// �ʵ��̸� ����
		void CloseFieldName(float fTime);
#ifdef COME_BACK_REWARD
		void CloseBuffName(float fTime);
#ifdef BUFF_NAME_CASH_SHOP_BUG_FIX
		void CloseBuffNameForce();
#endif // BUFF_NAME_CASH_SHOP_BUG_FIX
#endif //COME_BACK_REWARD
		bool GetShowLoadUi() 
		{
			if(m_pDLGLoadingState == NULL)
				return false;
			return true;
		}

		// �ʵ����� ��Ƽ����
		void UpdateUserPartyUid(KEGS_UPDATE_PARTY_UID_NOT partyInfo);
		UidType GetUserPartyUid(UidType iUid);

		float GetCameraDistance() { return m_fCameraDistance; }

		// �ʵ峻 ��� �ڽ��� ������ ��� ������ �����Ѵ�.
		void DeleteAllFieldUser();

		void LoadUI();

		void OpenFieldName();
		void OpenBuffName( const bool bShowFieldCreated_ = true, const int iIndex_ = 0 );
		
		void ResetNpcType( CX2LocationManager::HOUSE_ID houseID );
		void AddShopType( int iNpcUnitID, int iType );

#ifdef QUEST_GUIDE
		bool CX2TFieldGame::GetLinkVillageInfo( OUT map<int, D3DXVECTOR3>& mapVillageInfo_);
		bool CX2TFieldGame::GetLinkVillageList( OUT  vector<int>& vecVillageID_ );
		CX2NPCIndicator*		GetNpcIndicator(){return m_pNPCIndicator;}
#endif //QUEST_GUIDE

	
		bool		GetFreeCamera() const { return m_bFreeCamera; }
		CFirstPersonCamera* GetFPSCamera(){	return &m_FPSCamera; }

#ifdef ADDED_RELATIONSHIP_SYSTEM

		CX2SquareUnitPtr GetMyUnitBySquareUnitPtr() const { return m_pMyUnit; }
		const vector<CX2SquareUnitPtr> & GetUserUnitListBySquareUnitPtr() { return m_UserUnitList; } //�ּ� �޵��� ������ ��!

#endif // ADDED_RELATIONSHIP_SYSTEM

	private:				
		void LoadFieldImage(wstring &imgL, wstring &imgR);
		void OpenSubAniSoundPlay();
		float GetVillageCameraDistance();
		float GetVillageCameraEyeHeight();
		float GetVillageCameraLookAtHeight();

		//{{ kimhc // 2009-10-09 // ������ ���̵��� �߰�
#ifdef	GUILD_MANAGEMENT
		bool CanNotShowThisUnit( UidType iUnitUID ) const;
#endif	GUILD_MANAGEMENT
		// }}kimhc // 2009-10-09 // ������ ���̵��� �߰�

//		//{{ kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ ����
//#ifdef	ADD_SERVER_GROUP
//		bool SaveScriptServerGroupFile();
//#endif	ADD_SERVER_GROUP
//		//}}  kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ ����		

//{{ kimhc // 2010.7.14 // �ǽð� ���ҵ� �� ���������� �������� ����Ʈ ȿ��
#ifdef	REAL_TIME_ELSWORD
		void DisplayLevelUpEffect( UidType Uid );
#endif	REAL_TIME_ELSWORD
//}} kimhc // 2010.7.14 // �ǽð� ���ҵ� �� ���������� �������� ����Ʈ ȿ����

#ifdef RIDING_SYSTEM
		void SetOrClearRidingPetInfo( CX2Unit* pUnit_, const KFieldUserInfo& kFieldUserInfo_ );
#endif // RIDING_SYSTEM
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
		void SendTimeControlItemListTalk();
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	private:		
		void						InitKey();
		void						KeyProcess();
		void						SyncUnitFrame();
		
		NpcJoinShadow				*m_pNpcShadow;

		CKTDGUIDialogType				m_pDLGFieldName;
#ifdef COME_BACK_REWARD
		CKTDGUIDialogType				m_pDLGBuffName;
#endif
		CKTDGUIDialogType				m_pDLGMsgBox;

		SquareUI*					m_pSquareUI;
		KEGS_SQUARE_LIST_ACK		m_LastSquareListAck;
	
		SquareData					m_SquareData;
		
		CX2World*					m_pWorld;	/// X2Game ����
		CX2Camera*					m_pCamera;	/// X2Game ����

		CX2GUUser::InputData		m_InputData; /// X2Game ����
		//{{ seojt // 2009-1-13, 17:19
		CX2SquareUnitPtr            m_pMyUnit;
		vector<CX2SquareUnitPtr>    m_UserUnitList;
		//}} seojt // 2009-1-13, 17:19

		CKTDGParticleSystem*		m_pUiParticle;		/// �ʵ忡���� ���ҵ� ���۽� ������ ���� ���̱� ������ �ʿ� ����
		CKTDGXMeshPlayer*			m_pUiXMeshPlayer;	/// �ʵ忡���� ���ҵ� ���۽� ������ ���� ���̱� ������ �ʿ� ����
		//CKTDGParticleSystem*		m_pMajorParticle;
		//CKTDGParticleSystem*		m_pMinorParticle;
		//CKTDGXMeshPlayer*			m_pMajorXMeshPlayer;
		//CKTDGXMeshPlayer*			m_pMinorXMeshPlayer;

		float						m_fLoadUiTime;
		float						m_fFieldNameTime;
#ifdef COME_BACK_REWARD
		float						m_fBuffNameTime;
#endif
		int							m_MaxCount;		/// SD ��尡 �ƴѰ����� ǥ�� �� �� �ִ� �ִ� ����
		CKTDGFontManager::CUKFont*	m_pFontForUnitName;	/// X2Game ����
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT		
		CKTDGFontManager::CKTDGFont*m_pFont;
#endif
		float						m_fMarketTime;	/// �����Ա��� ȭ��ǥ ǥ�� �����

		bool						m_bEnableKeyProcess;	/// Ű�Է� ��� ����
		bool						m_bEnableCameraProcess;	/// ī�޶� FrameMove�� �������� ����

		CX2TalkBoxManager*			m_pTalkBoxManager;	/// X2Game ���� ���
		
		bool						m_bFreeCamera;		/// X2Game ����
		CFirstPersonCamera			m_FPSCamera;		/// X2Game ����

		bool						m_bAddMyUnit;	/// �̰� ���� �ʿ��ұ� �ʹ�...

		float						m_fStateSyncTime;		/// 2�� ���� ��ũ�� ���
		float						m_fMarketRefresh;		/// �ŷ����� ����Ʈ ���ſ� ���
		
		bool						m_bForceSync;			/// �̰� true�� �� �����ӿ����� ������ ��Ŷ ���� �� // �������� �������� ù ���� �� �������� �ʰ� Wait ���·� �ִ��� �ϴ��� �ѹ� Packet�� ������ ���� ����ϴ� �뵵�ΰ� ���� ��
		bool						m_bNearPortalToBattleField;			/// �̵� ��Ż�� ��Ҵ��� ����
		
		std::vector<int>			m_vecLinkedPos;			/// ��Ż���� ��ġ
		std::vector<int>			m_vecMarketPos;			/// ���� �Ա� ��ġ
		int							m_iVillageStartPosIdToBattleField;				/// ����� ������ ���� ������ ����
		int							m_iLastPos;				/// ����� ������ ���� ������ ����

		bool						m_bJoinNpc;				/// NPC ��ȭ��
		int							m_iJoinNpcIndex;		/// ��ȭ���� NPC Index

		bool						m_bJoinMarket;			/// ������ ���� ��Ż�� �Ÿ��� �����Ÿ� ���ϸ�
		bool						m_bShowMarketList;		/// ���� ���� ����Ʈ�� ���̴����� ����
		int							m_iJoinMarketIndex;		/// �̰͵� �־�� �ϳ�...?

		float						m_fCameraDistance;		/// dlg_map_village.lua�� ������ �� �������� ������ ī�޶� ��
		float						m_fLookAtHeight;		/// dlg_map_village.lua�� ������ �� �������� ������ ī�޶� ��
		float						m_fEyeHeight;			/// dlg_map_village.lua�� ������ �� �������� ������ ī�޶� ��

		//{{ seojt // 2009-1-14, 22:35
		std::vector<CX2TFieldNpcPtr>    m_vecFieldNpc;		/// npc�� ��ü
		//}} seojt // 2009-1-14, 22:35

		bool						m_bLoadComplete;		/// �ε��� �Ϸ� �ߴ��� ����...
		// ���� �����				
		int								m_iSubAniTime;		/// SubAniXSkinAnim���� ������ ���� ���ʴ� �ѹ� �ִϸ��̼��� ������ ������ ����
        /// ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
        /// - jintaeks on 2009-01-12, 16:49
		CKTDGXSkinAnim*					m_pSubAniXSkinAnim;	/// ����� SubSkinAnim
		CKTDXDeviceXSkinMesh*			m_pSubAniMesh;		/// ����� SubModel�� SubSkinAnim ����
		CKTDXDeviceSound*				m_pSubSound;		/// ��� ����

		/// ����Ʈ ������ ����°� ���� ��
		// ���� ��Ż����Ʈ
//#ifndef MODIFY_PORTAL_GATE
//		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate1;
//		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate2;
//		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate3;
//		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate4;
//		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPortalGate5;
//		CKTDGParticleSystem::CParticle*						m_pPortalGateParticle1;
//		CKTDGParticleSystem::CParticle*						m_pPortalGateParticle2;
//		CKTDGParticleSystem::CParticle*						m_pPortalGateParticle3;
//		CKTDGParticleSystem::CParticle*						m_pPortalGateParticle4;
//		CKTDGParticleSystem::CParticle*						m_pPortalGateParticle5;
//#endif  MODIFY_PORTAL_GATE

//#ifdef MODIFY_PORTAL_GATE
		vector<PortalGate>			m_vecPortalGateParticle;
		UINT						m_uiPortalCount;
//#endif //MODIFY_PORTAL_GATE

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hMarketInArea;	/// ���� ȭ��ǥ ����Ʈ
		//CKTDGParticleSystem::CParticle*						m_pInAreaParticle;	

		// ������ üũ
		float						m_fOtherLineTime;		/// ������ ȭ��ǥ�ð� ���Ʒ��� ������ �� �ʿ��� �ð�
		bool						m_bFindOtherLine;		/// true�� �������� ó���� ���� �ʰ� �������� ������
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hOtherLine;		/// �������� ���ΰ���, �Ʒ��ΰ��� ����Ʈ
		//CKTDGParticleSystem::CParticle*						m_pOtherLineParticle;

		// �ʽ��� ���̵�
		bool			m_bUserHide;						/// �ٸ� �������� ������ �ʰ� ó������ ����
#ifndef REFORM_NOVICE_GUIDE
		CX2NoviceGuide	*m_pNoviceGuide;					/// �ʽ��ڵ� �ƴѵ� �ʽ��� ���̵带....??
#endif //REFORM_NOVICE_GUIDE

		float												m_fSlidePartyInfoTime;	/// PartyLeaderTalkBox�� ������� ���� ���̱� ������ �ʿ� ������...

		// ���� �ε�ȭ�� (������ �̵��Ǿ��� ���� ���)
		CKTDGUIDialogType										m_pDLGLoadingState;		/// ������ �̵� ���� �� ���� �Ǵ� �ε�â
		CX2GameLoadingTip*									m_pCX2GameLoadingTip;		/// ������ �̵� ���� �� ���� �Ǵ� �ε�â�� Tip // Dungeon�̳� PVP�� StateŬ������ ����

#ifdef NEW_VILLAGE_AUTO_SD_UNIT
		float												m_fAutoSDUnitTime;	/// �ʵ� ���� �� ���� �ð� (5�� �� �������� 10 ���ϸ� SD����..)
#endif
		



#ifdef APINK_NPC
		CKTDXDeviceSound* m_pAPinkSound;
#endif

#ifdef MINI_GAME_1
			CX2MiniGame01 *m_pMiniGame01;
#endif
#ifdef QUEST_GUIDE
		CX2NPCIndicator*									m_pNPCIndicator;
#endif //QUEST_GUIDE
#ifdef SIGN_OVERRAP_BUG_FIX
		float												m_fDisSignToSign;
		bool												m_bSignOverRapCheck;
#endif SIGN_OVERRAP_BUG_FIX
//{{ robobeg : 2011-03-18
        CX2UnitLoader                                       m_UnitLoader;		/// X2Game ����
        
        struct  AddUserInUnitLoader
        {
            CX2Unit*        m_pUnit;
            D3DXVECTOR3     m_vPos;
            UidType         m_iPartyUID;

#ifdef ADDED_RELATIONSHIP_SYSTEM
			UidType			m_iRelationshipPartnerUID;
#endif // ADDED_RELATIONSHIP_SYSTEM
            AddUserInUnitLoader()
                : m_pUnit( NULL )
                , m_vPos( 0, 0, 0 )
                , m_iPartyUID( 0 ) 
#ifdef ADDED_RELATIONSHIP_SYSTEM
				, m_iRelationshipPartnerUID ( 0 )
#endif // ADDED_RELATIONSHIP_SYSTEM
			{}

            ~AddUserInUnitLoader()
            {
                SAFE_DELETE( m_pUnit );
            }
        };//struct  AddUserInUnitLoader

        typedef std::list<AddUserInUnitLoader>  AddUserInUnitLoaderList;
        AddUserInUnitLoaderList                 m_listAddUserInUnitLoader;

//}} robobeg : 2011-03-18

#ifdef SERV_MOMOTI_EVENT
		CKTDGUIDialogType		m_pDLGMomotiQuizEvent;
		wstring					m_MomotiQuizEventMsg;
		int						m_iInputDLGQuiz;
#endif //SERV_MOMOTI_EVENT
};

