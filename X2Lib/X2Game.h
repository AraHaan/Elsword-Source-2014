#pragma once

#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GUUser.h"
#endif REDUCED_PRECOMPILED_HEADER_TEST

//{{ 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include <boost/timer.hpp>
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}




class CX2Transfer;
class CX2Packet;
class CX2GUNPCFunc;

//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ��
#ifdef	REAL_TIME_ELSWORD
namespace	_CONST_X2GAME_
{
	const	UINT		g_iMaxED		= 2000000000;	// ������ �ִ� �ѵ��� ���� �� �ִ� ED��(20��)

// USE_MAXLEVEL_LIMIT_VAL ������ �ؿ��� �ּ� ó��
/*#ifdef _MAXLEVEL_LIMIT_30_
	const	int			g_iMaxLevel		= 30;
#elif defined _MAXLEVEL_LIMIT_40_
	const	int			g_iMaxLevel		= 40;
#else
	const	int			g_iMaxLevel		= 65;
#endif*/
	USE_MAXLEVEL_LIMIT_VAL
}
#endif	REAL_TIME_ELSWORD
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ��

class CX2Game : public CKTDXStage
{
	public:
#ifdef KHELP_SYSTEM_TEST
		DECLARE_HELP_SYSTEM( CX2Game );
#endif KHELP_SYSTEM_TEST

		void Debug_RenderHelp( int ix_, int iy_ );



	public:
		enum GAME_STATE
		{
			GS_INIT,
			GS_LOADING,
			GS_PLAY,
			GS_END,
		};

		enum GAME_TYPE
		{
			GT_NONE = 0,
			GT_PVP,
			GT_DUNGEON,
			GT_BATTLE_FIELD,
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			GT_NUMS
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		};

		enum CONNECT_TYPE
		{
			CT_CONNECT_SUCCESS,
			CT_START_GAME,
			CT_INTRUDE,
			CT_CONNECT_TO_INTRUDER,
		};

		enum ALARM_COLOR_TYPE
		{
			ACT_BLACK = 0,
			ACT_RED,
			ACT_BLUE,
			ACT_YELLOW,
			ACT_ORANGE,
			ACT_MAGENTA,
		};

		struct DangerAlarm
		{
			UidType				m_uid;			
			int					m_iStateId;
			float				m_fDelayTime;
			float				m_fShowTime;			
			ALARM_COLOR_TYPE	m_eColor;
			bool				m_bDanger;
			bool				m_bRepeat;
			wstring				m_wstrMessage;
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
			X2_UK_FONT			m_eFontType;
#endif //SERV_HALLOWEEN_EVENT_2013


			DangerAlarm()
			{
				Init();
			}

			void Init()
			{
				m_uid = -1;
				m_bDanger = false;
				m_wstrMessage = L"";
				m_fDelayTime = 0.f;
				m_bRepeat = false;
				m_eColor = ACT_BLACK;
				m_fShowTime = 0.f;
				m_iStateId = -1;
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
				m_eFontType = XUF_DODUM_20_BOLD;
#endif //SERV_HALLOWEEN_EVENT_2013
			}
		};

	public:
		struct CreateNPCData
		{
			CX2UnitManager::NPC_UNIT_ID		m_NPCID;
			int								m_Level;
			bool							m_bActive;
			int								m_UID;
			D3DXVECTOR3						m_UnitPos;
			bool							m_bRight;
			float							m_DelayTime;		
			int								m_iKeyCode;

			CX2Room::TEAM_NUM				m_eTeamNum;
			CX2NPCAI::NPC_AI_TYPE			m_eAIType;
			UidType							m_AllyUID;
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
			CX2GUNPC::NPC_CREATE_TYPE		m_eCreateMonsterType;	/// ���� ���� Ÿ��
	#else // SERV_NEW_DEFENCE_DUNGEON
			bool							m_bIsSummonMonster;		//���� ī��� ��ȯ�Ǿ������� ���� ����
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif //SUMMON_MONSTER_CARD_SYSTEM
		};

		struct NewGameUser
		{
			UidType		m_UnitUID;
			float		m_fNewUserTime;
			bool		m_bStart;
			int			m_StartPosIndex;

			NewGameUser()
			{
				m_UnitUID			= -1;
				m_fNewUserTime		= 0.0f;
				m_bStart			= false;
				m_StartPosIndex		= 0;
			}
		};

		struct ScoreTime
		{
			KProtectedType<int>		m_Score;
			float					m_fTime;

			ScoreTime()
			{
				m_Score = 0;
				m_fTime = 0.0f;
			}
		};


		// ����Ʈ ������ ���� ������ ȭ�鿡 �ǽð����� �����ֱ� ���� 
		struct InfoTextManager
		{
		public:
			vector<CX2TextManager::CX2Text*>		m_vecpText;

			int				m_iMaxLine;
			D3DXVECTOR2		m_vPos;


		public:
			InfoTextManager();
			~InfoTextManager();

			void ClearAllText();
			void RemoveText( CX2TextManager::CX2Text* pText );
			void MakeEmptyLine( bool bScrollUpside, int nCount );
			CX2TextManager::CX2Text* PushText( int fontIndex, const WCHAR* pTextMsg, 
					D3DXCOLOR color, D3DXCOLOR outlineColor, 
					DWORD dtType /*= DT_LEFT*/, float fLineSpace /*= 1.0f*/, float fLifeTime /*= 1.0f*/ );
		};

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD	
		class IntrudeData : public KJohnThread
		{
		public:

			IntrudeData()
			{
				m_pSlotData		= NULL;
				m_UnitUID		= 0;
				m_StartPosIndex = 0;
				m_pGame			= NULL;
			}
			virtual ~IntrudeData()
			{

			}

			virtual bool BeginThread()
			{
				return KJohnThread::BeginThread();
			}//BeginThread()

			virtual void EndThread(DWORD dwTimeout_ = INFINITE)
			{
				KJohnThread::EndThread( dwTimeout_ );
			}//EndThread()

			virtual DWORD RunThread()
			{
				m_pGame->AddUserUnitIntruder( m_pSlotData, m_pSlotData->m_Index );
				m_pGame->IntruderStart( m_UnitUID, m_StartPosIndex );
				m_pGame->GetMinorParticle()->CreateSequence( NULL,  L"INTRUDE_NOT", 0,0,0, 1000,1000, -1, 1 );
				m_pGame->GetMinorParticle()->CreateSequence( NULL,  L"INTRUDE_NOT_ADD", 0,0,0, 1000,1000, -1, 1 );

				m_pGame->InvalidEventThread( GetThreadHandle() );
				
				return 0;
			}//RunThread()

			CX2Room::SlotData*	m_pSlotData;
			UidType				m_UnitUID;
			int					m_StartPosIndex;
			CX2Game*			m_pGame;

		};//class TThread : public KJohnThread
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD		



#ifdef REVERSE_GRAVITY_TEST
		struct ReverseGravityData
		{

			float m_fTimeLeft;
			float m_fGravity;
			D3DXVECTOR3 m_vCenterPos;
			float m_fRange;
			int m_iTeamNumber;


			ReverseGravityData()
			: m_fTimeLeft( 0.f )
			, m_fGravity( 0.f )
			, m_vCenterPos( 0, 0, 0 )
			, m_fRange( 0.f )
			, m_iTeamNumber( 0 )
			{
			}

			bool IsInRange( 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                float   fElapsedTime,
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                const D3DXVECTOR3& vPos, float& fSpeeYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber );

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_fTimeLeft > 0.f )
				{
					m_fTimeLeft -= fElapsedTime;
					if( m_fTimeLeft < 0.f )
					{
						m_fTimeLeft = 0.f;
					}
				}
			}



		};

#endif REVERSE_GRAVITY_TEST

#ifdef	ELOG_STATISTICS_NEW_DATA
		struct  KFPS_STATISTICS
		{
			float       m_fAverageFps;
			unsigned    m_uNumOfFps;

			KFPS_STATISTICS()
				: m_fAverageFps( 0.f )
				, m_uNumOfFps( 0 )
			{
			}//KFPS_STATISTICS

			void        AddNewFps( float fFPS );
		};//struct  KFPS_STATISTICS

		struct  KGAME_STATISTICS
		{
			KFPS_STATISTICS
				m_kAverageFps;

			KGAME_STATISTICS()
				: m_kAverageFps()
			{
			}

			void        Init()
			{
				m_kAverageFps = KFPS_STATISTICS();
			}

			void        Send_EGS_FRAME_AVERAGE_REQ( int iGameType );
			void        GatherPeriodicStatistics();
		};//KGAME_STATISTICS

#endif	ELOG_STATISTICS_NEW_DATA


#ifdef MODIFY_DUNGEON_STAGING
		struct CinematicUI
		{  //ȭ�� ��,�Ʒ� �˰� ����� UI
			bool						m_bStart;				
			bool						m_bEnd;
			float						m_fTimer;
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			float						m_fDeleteTime;	// -1 �� �ƴϸ� �ش� �ʿ� �°� ����, �׷��ٸ� ���� �ҽ���� ����
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCinematicTop;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCinematicBottom;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticleHandle				m_hCinematicTopParticle;			
			CKTDGParticleSystem::CParticleHandle				m_hCinematicBottomParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle*						m_pCinematicTopParticle;			
			CKTDGParticleSystem::CParticle*						m_pCinematicBottomParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			HRESULT OnFrameMove ( double fTime, float fElapsedTime );

			CinematicUI();
			~CinematicUI();

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			void Init()
			{
				m_bStart					= false;
				m_bEnd						= false;
				m_fTimer					= -1;
				m_fDeleteTime				= -1;	
			}
			bool GetIsFadeInState () { return m_bStart; }
			bool GetIsFadeOutState () { return m_bEnd; }
			bool GetIsCinematicUIActive () { return ( false == m_bStart  && false  == m_bEnd );  }
			
			void SetCinematicUI( bool bVal, float fTime_ = -1.0f );	// CinematicUI �� ��Ȱ�� ���̸� fTime ��ŭ Ȱ��ȭ, fTime �� ������� ������ ���� CinematicUI �� ����
#else // SERV_BATTLEFIELD_MIDDLE_BOSS
			void SetCinematicUI(bool bVal);


#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		};
#endif //MODIFY_DUNGEON_STAGING
	public:
		CX2Game();
		virtual ~CX2Game(void);

		virtual void Release() = NULL;		/// X2Game�� �Ҹ��ڿ��� ����Ǿ� �Ѵ� �͵��� �̰����� �Űܼ� ������
		//virtual void				P2PSetPeer();
		//virtual void				P2PSetPeer( CX2Unit* pUnit );
		//virtual void				P2PDeletePeer( UidType unitUID );
		//virtual void				P2PConnectTest();
		//virtual bool				P2PConnectTestResult();

		void						SetBuffInfoPacketToNpc( const KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT& kEvent_ );
		void						SetBuffInfoPacketToGUUser( const KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT& kEvent_ );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        WORD                        Handler_P2P_UnitUserSyncPack( const BYTE* pbyData, BYTE byNumPacks, WORD wMaxSize, UidType uidUnitUID, DWORD dwFrameMoveCount );
		WORD                        Handler_P2P_UnitNpcSyncPack( const BYTE* pData, BYTE byNumNPCs, WORD wMaxSize, UidType uidUnitUID, DWORD dwFrameMoveCount );
		WORD                        Handler_P2P_UnitPetSyncPack( const BYTE* pData, BYTE byNumNPCs, WORD wMaxSize );
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        WORD                        Handler_P2P_UnitNpcMiniSyncPack( const BYTE* pData, BYTE byNumNPCs, WORD wMaxSize, UidType uidUnitUID, DWORD dwFrameMoveCount );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual void				Handler_P2P_UnitNpcSyncPack( CKTDNUDP::RecvData* pRecvData, bool bFirst_ = false );
//		virtual void				Handler_P2P_UnitPetSyncPack( CKTDNUDP::RecvData* pRecvData );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		virtual void				P2PPacketHandler();
		virtual bool				P2PPacketHandler( CKTDNUDP::RecvData* pRecvData );

#ifdef	BATTLE_FIELD_TEST
		void						WriteBroadCastUser();
#endif	BATTLE_FIELD_TEST

		virtual void				GameLoading( CX2Room* pRoom );
		virtual void				SystemLoading();
		virtual void				WorldLoading(){}
		virtual void				UnitLoading();
		virtual void				EtcLoading();

		CX2GUUser*					CreateGUUser( const CX2Room::SlotData* pSlotData_, int iSlotIndex_ );
		virtual	void				AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual	CX2GUUser*			AddUserUnitIntruder( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual	void				AddUserUnitIntruderByLoader( CX2Unit* pUnit );
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		virtual	void				IntruderStart( CX2GUUser* pX2User, int startPosIndex = -1 );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		virtual	void				IntruderStart( UidType unitUID, int startPosIndex = -1 );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		virtual void				GameStart();
		virtual void				GameEnd();

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnFrameRender();
		virtual bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		virtual bool				DetermineLastKill() = NULL;

		virtual void				StopOtherUnit( CX2GameUnit* pNonStopUnit, float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, bool bOtherTeam = false );
		virtual void				StopAllUnit( float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, CX2GameUnit* pMyUnit = NULL, float fMyStopTime = -1.0f );

		virtual void				Stop2OtherUnit( CX2GameUnit* pNonStopUnit, float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, bool bOtherTeam = false );
		virtual void				Stop2AllUnit( float fStopTime, D3DXVECTOR3* pPos = NULL, float fDistance = -1, CX2GameUnit* pMyUnit = NULL, float fMyStopTime = -1.0f );

		virtual void				ChangeFocusUnit(CX2GameUnit* pGameUnit = NULL);	// �� ������ ���� ���� �� �����༭ ��Ŀ�� ����������..
		virtual void				ResetFocusUnit();
		virtual void				SetTimerFocusUnit( CX2GameUnit* pFocusUnit, float fFocusTime = 0.f, float fDistacne = -1.f );
#ifdef INIT_FOCUS_UNIT
		virtual void				InitFocusUnit();
#endif // INIT_FOCUS_UN

		CKTDGLineMap*				GetLineMap()				
		{ 
			if( NULL != GetWorld() ) 
				return GetWorld()->GetLineMap(); 

			return NULL;
		}
#ifdef X2TOOL
		void						SetWorld(CX2World* pWorld)	{ m_pWorld = pWorld; }
#endif
		virtual CX2World*			GetWorld()					{ return m_pWorld; }
		virtual CX2Camera*			GetX2Camera()				{ return m_pCamera; }
		CX2DamageEffect*	        GetDamageEffect()			{ return m_pDamageEffect; }
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        bool				        IsHost()					{ return m_uidHostSlotUID != 0 && m_uidHostSlotUID == m_uidMySlotUID; }
        UidType                     GetHostSlotUID()            { return m_uidHostSlotUID; }
        UidType                     GetMySlotUID()              { return m_uidMySlotUID; }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual bool				IsHost()					{ return m_bHost; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        bool                        IsPacketSendFrame()         { return m_iPacketSendFrameMoveCount == 0; }
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
		
		virtual CX2GUUser*			GetMyUnit()	const			{ return ( null != m_optrMyUnit ? static_cast<CX2GUUser*>( m_optrMyUnit.GetObservable() ) : NULL ); }
		virtual CX2GameUnit*		GetFocusUnit() const		{ return ( null != m_optrFocusUnit ? m_optrFocusUnit.GetObservable() : NULL ); }
		D3DXVECTOR3					GetFocusUnitPos_LUA();
		
		virtual CX2GUUser*			GetUserUnitByUID( UidType unitUID );
		virtual CX2GUNPC*			GetNPCUnitByType( int iNPCID );
		virtual CX2GUNPC*			GetNPCUnitByUID( int npcUID );
		virtual CX2GUNPC*			GetNPCUnitByKeyCode( int iKeyCode );
		virtual CX2GameUnit*		GetUnit( int index );	
		virtual CX2GUUser*			GetUserUnit( int index );	
		virtual CX2GUNPC*			GetNPCUnit( int index );
		
		virtual void				SetNPCUnit(int index, CX2GUNPC* pCX2GUNPC){ m_NPCUnitList[index] = pCX2GUNPC; };
		virtual void				AddUnit( CX2GameUnit* pGameUnit ){ m_UnitList.push_back(pGameUnit); }
		void						KillUnit( bool bNPCKiller, UidType killerUID, UidType maxDamageKillerUID, bool bNPCKilled, UidType killedUID );
		void						SetKillScore( UidType userUnitUID, int killNum, int killedNum, int mdKillNum );
		int							GetKillCount( UidType userUnitUID );
		int							GetKillAssistCount( UidType userUnitUID );


		void						ChangeUserUnitIndex( const UINT uiFirstIndex_, const UINT uiSecondIndex_ );
		virtual bool				DeleteUserUnit( int index );
		virtual bool				DeleteUserUnitByUID( UidType unitUID );
		virtual bool				DeleteUserUnitByData( CX2GameUnit* pGameUnit );
		virtual bool				DeleteNPCUnit( int index );
		virtual bool				DeleteNPCUnitByUID( const UINT uiNpcUid_ );
		virtual bool				DeleteAllNPCUnit();
		
		void                        AddNPCResourcesToDeviceList( CX2UnitManager::NPC_UNIT_ID npcID );
		void                        RemoveAllNPCResourcesFromUnitLoader();

		virtual int					GetUserUnitListSize()	{ return m_UserUnitList.size(); }
		virtual int					GetNPCUnitListSize()	{ return m_NPCUnitList.size(); }
		virtual int					GetUnitNum()			{ return (int)m_UnitList.size(); }
		virtual int					GetUserUnitNum();
		virtual int					GetNPCUnitNum();
		virtual int					GetEmptyNPCIndex();
		int							LiveUserUnitNum();
		int							LiveActiveNPCNum();
		int							LiveNPCNumType_LUA( CX2UnitManager::NPC_UNIT_ID unitID );
		bool						IsKillNPCKeyCode_LUA( int keyCode );
		int							GetKilledKeyCodeNPCCount( int keyCode );
		int							LiveKeyCodeNPCNum( int keyCode );

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-02
		int							LiveKeyCodeNPCNumFreeTeam( int keyCode );
#endif //SERV_NEW_DEFENCE_DUNGEON

		int							GetAliveActiveNPCCount();

#ifdef SERV_INSERT_GLOBAL_SERVER
		int							GetEliteNPCUnitNum(); // oasis907 : ����� [2011.4.19]
		int							GetEliteKeyCodeNPCUnitNum( int keyCode );
#endif SERV_INSERT_GLOBAL_SERVER

#ifdef NEW_CHARACTER_EL
		virtual bool				GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const;
#endif // NEW_CHARACTER_EL


		CX2GUNPC*					GetKeyCodeNPC( int keyCode );
		CX2GUNPC*					GetFoucusCameraNPC();
		bool						IsValidUnit( CX2GameUnit* pUnit );

		bool						IsAllUserDead();
		bool 						IsAllUserHPPercentMoreThan( float fHPPercent );
		void 						PartyFeverAllUser( float fAddHyperModeTime );

		void						UpdateNpcNameColor( const int iMyLevel_ );

		bool						RebirthUserUnit( UidType unitUID, int startPosIndex );
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
		CX2GUNPC*
#else SERV_CREATE_ELITE_FOR_ADMIN
		void
#endif SERV_CREATE_ELITE_FOR_ADMIN

#ifdef SERV_INSERT_GLOBAL_SERVER
		CreateNPC( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, int uid, const D3DXVECTOR3& vPos, bool bRight, 
			float fDelayTime, int iKeyCode, 
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1, KAttribEnchantNpcInfo* _kAttribEnchantNpcInfo = NULL
	#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
	#endif	SERV_TRAPPING_RANGER_TEST
	#ifdef SUMMON_MONSTER_CARD_SYSTEM
		#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
			, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType = CX2GUNPC::NCT_NONE		/// ���� ���� Ÿ��
		#else // SERV_NEW_DEFENCE_DUNGEON
			, bool bIsMonsterCard = false		//���� ī��� ��ȯ�Ǿ������� ���� ����
		#endif // SERV_NEW_DEFENCE_DUNGEON
	#endif SUMMON_MONSTER_CARD_SYSTEM
			);
#else //SERV_INSERT_GLOBAL_SERVER
		CreateNPC( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, int uid, D3DXVECTOR3 vPos, bool bRight, 
			float fDelayTime, int iKeyCode, 
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1
#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
#endif	SERV_TRAPPING_RANGER_TEST
			);
#endif SERV_INSERT_GLOBAL_SERVER
		void SetUserSummonedNPCInfo( CX2NPCAI::NPC_AI_TYPE eAIType, UidType iAllyUID, CX2GUNPC* pNPC, int iHyperMode, CX2UnitManager::NPC_UNIT_ID unitID );

#ifdef CHRISTMAS_EVENT_2013
		void Handler_EGS_NPC_UNIT_CREATE_REQ( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, 
			D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode,
			CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_NONE*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
			UidType iAllyUID /*= -1*/, bool bElite /* = false */
			, CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */ 
			, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType /*= false */			//���� ī��� ��ȯ�Ǿ������� ���� ����
			);
#endif CHRISTMAS_EVENT_2013

#ifdef SERV_INSERT_GLOBAL_SERVER		
		void						CreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, D3DXVECTOR3 vPos,
			bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode = -1,
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1, bool bElite = false
	#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
	#endif	SERV_TRAPPING_RANGER_TEST
	#ifdef SUMMON_MONSTER_CARD_SYSTEM

		#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
			, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType = CX2GUNPC::NCT_NONE		/// ���� ���� Ÿ��
		#else // SERV_NEW_DEFENCE_DUNGEON
			, bool bIsMonsterCard = false		//���� ī��� ��ȯ�Ǿ������� ���� ����
		#endif // SERV_NEW_DEFENCE_DUNGEON

	#endif SUMMON_MONSTER_CARD_SYSTEM
			);
#else
		void						CreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, D3DXVECTOR3 vPos,
			bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode = -1,
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1
#ifdef	SERV_TRAPPING_RANGER_TEST
			, CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE
#endif	SERV_TRAPPING_RANGER_TEST
			);
#endif SERV_INSERT_GLOBAL_SERVER

		void						CreateNPCReq_LUA( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop );
#ifdef SERV_INSERT_GLOBAL_SERVER
		void						CreateNPCReqElite( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, bool bElite = false );
#endif SERV_INSERT_GLOBAL_SERVER	
#ifdef SEASON3_MONSTER_2011_7
		void						CreateNPCReqAlly( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, CX2Room::TEAM_NUM eTeam = CX2Room::TN_RED );
#endif SEASON3_MONSTER_2011_7
		
		void						CreateNPCReq_LUA2( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop );
		
		void						PushCreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, D3DXVECTOR3 vPos,
			bool bRight, float fDelayTime, bool bNoDrop, int iKeycode = 0,
			CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER, CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL, UidType iAllyUID = -1 );
		void						PushCreateNPCReq_Lua( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode );
		void						FlushCreateNPCReq();

#ifdef CREATE_NPC_REQ_FULL_ARGUMENTS
		void						CreateNPCReq_LUA3( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, int iTeam, int iAIType, UidType iAllyUID );
		void						PushCreateNPCReq_LUA3( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, int iTeam, int iAIType, UidType iAllyUID );
#endif CREATE_NPC_REQ_FULL_ARGUMENTS

		D3DXVECTOR3					GetNPCStartPos( int key )
		{
			if( m_pWorld != NULL && m_pWorld->GetLineMap() != NULL )
			{
				return m_pWorld->GetLineMap()->GetStartPosition( key );
			}
			return D3DXVECTOR3(0,0,0);
		}

		virtual GAME_TYPE			GetGameType()				{ return m_GameType; }
		virtual void				SetGameType( GAME_TYPE gameType ) { m_GameType = gameType; }
		virtual GAME_STATE			GetGameState()				{ return m_GameState; }
		virtual void				SetGameState( GAME_STATE gs ) { m_GameState = gs; }

#ifdef EFFECT_TOOL
		virtual/*FORCEINLINE*/ CKTDGParticleSystem*		GetMajorParticle();			//{ return g_pData->GetGameMajorParticle(); }
		virtual/*FORCEINLINE*/ CKTDGParticleSystem*		GetMinorParticle();			//{ return g_pData->GetGameMinorParticle(); }
		virtual/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMajorXMeshPlayer();		//{ return g_pData->GetGameMajorXMeshPlayer(); }
		virtual/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMinorXMeshPlayer();		//{ return g_pData->GetGameMinorXMeshPlayer(); }
#else
		/*FORCEINLINE*/ CKTDGParticleSystem*		GetMajorParticle();			//{ return g_pData->GetGameMajorParticle(); }
		/*FORCEINLINE*/ CKTDGParticleSystem*		GetMinorParticle();			//{ return g_pData->GetGameMinorParticle(); }
		/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMajorXMeshPlayer();		//{ return g_pData->GetGameMajorXMeshPlayer(); }
		/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMinorXMeshPlayer();		//{ return g_pData->GetGameMinorXMeshPlayer(); }
#endif // EFFECT_TOOL
		/*FORCEINLINE*/ CX2EffectSet*				GetEffectSet();				//{ return g_pData->GetGameEffectSet(); }


		CKTDXDeviceRenderTargetTexture* GetLastKillShot()		{ return m_pRenderTargetLastkillShot; }

		bool 						GetEnableAllKeyProcess() const { return m_bEnableAllKeyProcess; }
		void 						SetEnableAllKeyProcess(bool val) { m_bEnableAllKeyProcess = val; }
		void						SetEnableCommandKeyProcess( bool bEnable ) { m_bEnableCommandKeyProcess = bEnable; }

		bool						CheckEnableKeyProcess();
				

		virtual int					GetWinTeam()	{ return -1; }
		void						SetLastKillCheck( bool bLastKillCheck ) { m_bLastKillCheck = bLastKillCheck; }
		bool						GetLastKillCheck() { return m_bLastKillCheck; }

		int							GetLoadingPercent() { return m_LoadingPercent; }
		bool						GetBGMOn() { return m_bBGMOn; }		

		void						SetWorldColor_LUA( D3DXCOLOR color )
		{ 
			if( NULL != GetWorld() )
			{
				GetWorld()->SetWorldColor( color ); 
			}
		}		
		
		void BroadCast_XPT_UNIT_NPC_SYNC_PACK();




		CKTDGFontManager::CUKFont*	GetFontForUnitName() { return m_pFontForUnitName; }

		static void					SetRenderNPCName(bool bRenderNPCName) { m_sbRenderNPCName = bRenderNPCName; }
		static bool					GetRenderNPCName(){ return m_sbRenderNPCName; }

		KGAME_STATISTICS&           AccessGameStatistics()          { return    m_kGameStatistics; }

		virtual bool				Handler_EGS_JOIN_ROOM_NOT( KEGS_JOIN_ROOM_NOT& kEGS_JOIN_ROOM_NOT );

		virtual bool				Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason = NetError::NOT_LEAVE_ROOM_REASON_00 );
		virtual bool				Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK );
		virtual bool				Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );

		virtual bool				Handler_EGS_LEAVE_GAME_REQ();
		virtual bool				Handler_EGS_LEAVE_GAME_ACK( KEGS_LEAVE_GAME_ACK& kEGS_LEAVE_GAME_ACK );
		virtual bool				Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT );

		virtual bool				Handler_EGS_STATE_CHANGE_RESULT_REQ();
		virtual bool				Handler_EGS_STATE_CHANGE_RESULT_ACK( KEGS_STATE_CHANGE_RESULT_ACK& kEGS_STATE_CHANGE_RESULT_ACK );
		virtual bool				Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT );


		virtual bool				Handler_EGS_PLAY_START_NOT( KEGS_PLAY_START_NOT& kEGS_PLAY_START_NOT );

		virtual bool				Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( KEGS_STATE_CHANGE_GAME_INTRUDE_NOT& kEGS_STATE_CHANGE_GAME_INTRUDE_NOT );
		virtual bool				Handler_EGS_INTRUDE_START_REQ();
		virtual bool				Handler_EGS_INTRUDE_START_ACK( KEGS_INTRUDE_START_ACK& kEGS_INTRUDE_START_ACK );
		virtual bool				Handler_EGS_INTRUDE_START_NOT( KEGS_INTRUDE_START_NOT& kEGS_INTRUDE_START_NOT );

		virtual void				Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( KEGS_CURRENT_TEAM_KILL_SCORE_NOT& kEGS_CURRENT_TEAM_KILL_SCORE_NOT );

		virtual bool				Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT );


		virtual bool				Handler_EGS_DROP_ITEM_NOT( KEGS_DROP_ITEM_NOT& kEGS_DROP_ITEM_NOT );

		virtual bool				Handler_EGS_GET_ITEM_REQ( UidType itemUID );
		virtual bool				Handler_EGS_GET_ITEM_ACK( KEGS_GET_ITEM_ACK& kEGS_GET_ITEM_ACK );
		virtual bool				Handler_EGS_GET_ITEM_NOT( KEGS_GET_ITEM_NOT& kEGS_GET_ITEM_NOT );

		virtual bool 				Handler_EGS_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool 				Handler_EGS_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		bool						Handler_EGS_USE_FINALITY_SKILL_REQ();
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
		bool						Handler_EGS_USE_FINALITY_SKILL_REQ( UidType itemUID_ );
		virtual bool				Handler_EGS_USE_FINALITY_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_INSERT_GLOBAL_SERVER
		virtual bool 				Handler_EGS_CREATE_ATTRIB_NPC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_INSERT_GLOBAL_SERVER

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		virtual bool				Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// �߰� ���� ���� ACK
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		virtual bool				Handler_EGS_NPC_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool				Handler_EGS_NPC_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool						NPCUnitDieReq( const KEGS_NPC_UNIT_DIE_REQ& kPacket_ );
//{{ 09.05.12 �¿� : �������� ��� �������� �۾�
#ifdef BONUS_DROP
		bool						NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, bool bIsAllyNPC, bool bBounsDrop = false );
		bool						NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, KEGS_NPC_UNIT_DIE_REQ::NPC_DIE_STATE eDieState );
#else
		bool						NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, bool bIsAllyNPC );
#endif

		virtual bool				Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT );

		virtual bool				Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ();
		virtual bool				Handler_EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT();

		virtual bool				Handler_EGS_ADD_ON_STAT_ACK( KEGS_ADD_ON_STAT_ACK& kEGS_ADD_ON_STAT_ACK );
		virtual bool				Handler_EGS_ADD_ON_STAT_NOT( KEGS_ADD_ON_STAT_NOT& kEGS_ADD_ON_STAT_NOT );



		virtual void				UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );
		virtual bool				Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( KEGS_USER_UNIT_DIE_COMPLETE_NOT& kEGS_USER_UNIT_DIE_COMPLETE_NOT );

		//bool						Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		CX2TalkBoxManager*			GetTalkBoxManager() { return m_pTalkBoxMgr; }

		float						GetElapsedTimeAfterLastKeyboardInput() { return m_fElapsedTimeAfterLastKeyboardInput; }
		bool						IsKeyPressed() { return m_InputData.IsKeyPressed(); }
		bool						IsSkipKeyPressed(){ return m_bSkipKeyPressed; }
		void						SetSkipKeyPress(bool bVal){m_bSkipKeyPressed=bVal;}
		CX2GUUser::InputData*		GetInputData() { return &m_InputData; }

		virtual bool				GameChatInfoNot( KEGS_CHAT_NOT& kKEGS_CHAT_NOT ) { return false; }

		virtual bool				Handler_EGS_UPDATE_UNIT_INFO_NOT( KEGS_UPDATE_UNIT_INFO_NOT& kEGS_UPDATE_UNIT_INFO_NOT );
		//virtual bool				Handler_EGS_NOTIFY_MSG_NOT( KEGS_NOTIFY_MSG_NOT& kEGS_NOTIFY_MSG_NOT );

		void						ProcessExpListByNpcUnitDie( const KEGS_NPC_UNIT_DIE_NOT& kEvent );
#ifdef ARA_CHARACTER_BASE
		void						ProcessIKillThisNpc( const KEGS_NPC_UNIT_DIE_NOT& kEvent );
#endif

#ifdef FIX_ARA_AWAKE_ABILITY_BUG
		void						ProcessIKillThisUser( const KEGS_USER_UNIT_DIE_NOT& kEvent );
#endif // FIX_ARA_AWAKE_ABILITY_BUG


		bool						CheckNewUser( UidType unitUID );

		float						GetGameTime(){ return m_fGameTime; }

		void						SetShowF1Help( bool bShow ){ m_bShowF1Help = bShow; }
		bool						GetShowF1Help(){ return m_bShowF1Help; }
		void						RenderF1Help();
		void						InitF1HelpString();

		
		bool						GetEnableNPCTalkBox() { return m_bEnableNPCTalkBox; }

#ifdef CHECK_CLEAR_SCORE
		int							GetGameScore() { return m_GameScore; }
		int							GetGameScoreBack() { return m_GameScoreBack; }
#endif
#ifdef DIALOG_SHOW_TOGGLE
		void						SetGameScore();
#endif
		void						SetGameScore( int gameScore );
		void						AddGameScore( int gameScore );
		void						AddGameScore( int gameScore, float fTime );

		void						SetTechPoint( int techPoint );	/// ��� ����
		void						AddTechPoint( int techPoint );
		
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM	
		int							GetSubStageTechPoint () const;			//	�ش� SubStage �� ��ũ�� ����Ʈ�� ���´�.
		void						ResetSubStageTechPoint ();				//	
#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM


		bool						GetOpenChatBox() { return m_bOpenChatBox; }
		void						SetOpenChatBox( bool bCheck ) { m_bOpenChatBox = bCheck; }

		void						PauseNPCAI( float fTime );
		void						KillAllNPC_LUA( bool bActiveOnly = true );
		void						KillAllNPC( bool bActiveOnly = true, CX2GameUnit* pAttackerGameUnit_ = NULL, bool bExceptGate = false );
		void						KillNPC( const CX2UnitManager::NPC_UNIT_ID eNPCID, int nCount );
		void						EnableAllNPCAI( bool bEnable );
		void						EnableAllNPCEventProcess( bool bEnable );
		void						SetEnableAllNPCConditionTable( bool bEnable);
		void						SetAllNPCMPFull();


		void						SetFreeCamera( bool bFreeCamera );
		bool						GetFreeCamera() { return m_bFreeCamera; }
#ifdef KEYFRAME_CAMERA
		bool						GetWorldCameraEdit(){ return m_bWorldCameraEdit; }
		void						SetWorldCameraEdit( bool bEnable);
#endif KEYFRAME_CAMERA
		// oasis907 : ����� [2010.12.13] // ���� �Ľ� ���� ������ ���� ����
		void						PlayWorldCamera( int iWorldCameraID );

		int							GetTeamVPMaxAdvRed(){ return m_TeamVPMaxAdvRed; }
		int							GetTeamVPMaxAdvBlue(){ return m_TeamVPMaxAdvBlue; }

		int							GetRedTeamKill(){ return m_RedTeamKill; }
		int							GetBlueTeamKill(){ return m_BlueTeamKill; }

		void						SetAllowFrameCount( int allowFrameCount )
		{ 
			if( m_AllowFrameCount >= 0 )
				m_AllowFrameCount += allowFrameCount;
			else
				m_AllowFrameCount = allowFrameCount; 
		}

		bool						GoStage( int iStageIndex, int iSubStageIndex = 0 );

#ifdef SECRET_VELDER
		int		GetNowStageIndex();
#endif

		int							GetGetEXPInGame() const { return m_GetEXPInGame; }

		//{{ kimhc // 2010-12-17 // ���� ����� ��븦 ���� �� �� �ֵ��� �ϱ� ���� ���� Ÿ�� �߰�
		// Nearst�� ã�� ���� NPC�� Type�� ���� ����ó���� �߰��ؾ� �ϱ� ������ �̰��� �̿��ϱ�� ��
//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		UidType						GetLockOnNearstTarget( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT bool& bUserUnit_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG
#ifdef FIX_PET_ATTACK
		UidType						GetLockOnNearstTargetOnlyBasic( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT bool& bUserUnit_ );		
#endif
		//}} kimhc // 2010-12-17 // ���� ����� ��븦 ���� �� �� �ֵ��� �ϱ� ���� ���� Ÿ�� �߰�
		UidType						GetLockOnTarget( CX2Room::TEAM_NUM teamNum, int seedNum, bool& bUserUnit, int iModulusFactor = 30  );

		//{{ JHKang / ������ / 2011.7.103 / ������ ����(min - max) �߰� ����
#ifdef CHUNG_SECOND_CLASS_CHANGE
#ifdef	NEW_CHARACTER_CHUNG
		UidType						GetLockOnNearstTargetInRange( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, IN D3DXVECTOR2& vRange_, OUT bool& bUserUnit_ 
	#ifdef BALANCE_DEADLY_CHASER_20130214
																, bool bIsNotTargetPreLockOnTarget = false
	#endif //BALANCE_DEADLY_CHASER_20130214
																);
#endif	NEW_CHARACTER_CHUNG
#endif
		//}}

#ifdef GET_DUNGEON_ID_IN_LUA
		virtual int					GetDungeonID() { return 0; }
#endif GET_DUNGEON_ID_IN_LUA
		virtual int					GetDungeonType() { return 0; }
		virtual int					GetGameLevel() { return 0; }
		CX2SlideShot*				GetSlideShot();

		void						SetShowAllUnitGageBar( bool bShow );
		bool						GetShowAllUnitGageBar() { return m_bShowAllUnitGageBar; }

		void						SetGameEnd(bool val) { m_bGameEnd = val; }
		bool						GetGameEnd() { return m_bGameEnd; }

		CX2DropItemManager*			GetDropItemManager() const { return m_pDropItemManager; }
		bool						AnyUserUnitInRange( D3DXVECTOR3 pos, float fRange );
		bool						IsThereAnyUserUnitOnLine( int iLineIndex );
		bool						IsThereAnyUserUnitUsingSkill( CX2SkillTree::SKILL_ID eSkillID );


		void						CreateStageLoadingTeleportEffectForAllUser( bool bEnterPortal );

		//{{oasis:�����////2009-10-7////
		D3DXVECTOR3					GetFixedDistancePosition_LUA( D3DXVECTOR3 pvPos, float fDist, bool bIsRight );
		float						GetDist_LUA( const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2 );
		bool						IsNearNPCUnitUID_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUNPC*					GetNearNoneAINPCUnitUID( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUUser*					GetNearestUserUnit( const D3DXVECTOR3& vPos_ );
		UidType						GetNearestUserUnitUID_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
		D3DXVECTOR3					GetNearestUserUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
		//}}oasis:�����////2009-10-7////
		D3DXVECTOR3					GetNearestNPCUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
#ifdef SEASON3_MONSTER_2011_7
		UidType						GetFarthestUserUnitUID_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
#endif SEASON3_MONSTER_2011_7
#ifdef VELDER_BOSS_5
		//{{ oasis907 : ����� [2010.7.30] // 
		D3DXVECTOR3					GetFarthestUserUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange = 9999 );
		//}}
#endif VELDER_BOSS_5
		int							GetNearNPCUnitUIDInStart_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUNPC*					GetNearNPCUnitInStart( const D3DXVECTOR3 vPos_, const CX2UnitManager::NPC_UNIT_ID eUnitID_, const float fMaxRange = 9999 );


        int							GetNearNPCUnitUID_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange = 9999 );
		CX2GUNPC*					GetNearestNpcInSpecificRangeByNpcId( const D3DXVECTOR3& vPos_, const CX2UnitManager::NPC_UNIT_ID eUnitID_, const float fMaxRange_ = 9999 );

		int							GetNearNPCUnitUIDEx( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, int npcUIDToExcept, float fMaxRange = 9999 );
		int							GetNearNPCUnitUIDEx( D3DXVECTOR3 pos, const vector< CX2UnitManager::NPC_UNIT_ID >& vecUnitID, int npcUIDToExcept, float fMaxRange = 9999 );
		int							GetNearestNPCUnitUID( D3DXVECTOR3 pos, int npcUIDToExcept, float fMaxRange = 9999 );
		CX2GUUser* GetNearestUserUnitInSpecificRange( const D3DXVECTOR3& vPos_, const float fMinRange_, const float fMaxRange_ = 9999 );
		
		CX2GameUnit*				GetFrontNearestEnemyUnit( CX2GameUnit* pMyUnit, float& fDistance );

#ifdef NEAREST_NPC_ON_SAME_DIRECTION
		D3DXVECTOR3					GetNearestNPCUnitPosOnSameDirection_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange, D3DXVECTOR3 vDir );
		CX2GameUnit*				GetNearestNPCUnitOnSameDirection_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange, D3DXVECTOR3 vDir );
#endif NEAREST_NPC_ON_SAME_DIRECTION

		bool						ChangeRebirthStateNPCUnit_LUA( int NPCUID );


		int							GetNPCCountAt( D3DXVECTOR3 vPosition, float fRadius = 30.f, CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
		D3DXVECTOR3					GetEmptyStartPos();
#ifdef SKILLSTRING_IN_SKILLTEMPLET
		void						SetStateString( const wstring pStr ){ m_StateString = pStr; }
#else //SKILLSTRING_IN_SKILLTEMPLET
		void						SetStateString( const WCHAR* pStr ){ m_StateString = pStr; }
#endif //#ifdef SKILLSTRING_IN_SKILLTEMPLET

		CX2Game::InfoTextManager&	GetInfoTextManager() { return m_InfoTextManager; }
		CX2GUNPCFunc*				GetNPCFunc(){ return m_pNPCFunc; }


		void SetRenderUserName(bool val) { m_bRenderUserName = val; } // tutorial���� ĳ���� �̸� �� �׸���

		void						UseItemSpecialAbility( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser );
		void						ApplyBuffFactorToGUUser( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser );

#ifdef PET_ITEM_GM
		void						UseItemPetAbility( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser );
#endif //PET_ITEM_GM

		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ��� �� �ƹ�Ÿ ��ü
#ifdef REAL_TIME_ELSWORD
		bool						RemoveSpecialAbilityInEquip( const CX2Item::ItemTemplet* const pItemTemplet, CX2GUUser* const pCXGUUser );
		bool						SetSpecialAbilityInEquip( const CX2Item::ItemTemplet* const pItemTemplet, CX2GUUser* const pCXGUUser );
#endif REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ��� �� �ƹ�Ÿ ��ü

		//{{ kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08
		void						SetCanUseEscFlag( bool flag ) { m_bCanUseESC = flag; }
		bool						GetCanUseEscFlag() const { return m_bCanUseESC; }
		//}} kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08

		void						UpdateSkillSlotUI(bool bUpdateEquipSkill = false);

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		void						InvalidEventThread( HANDLE hValue );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


		void						SetLastkillShotTime(float val) { m_fLastkillShotTime = val; }

		void						BuffAllNPC( int statType, float fStatVal, float fStatTime );


#ifdef RESET_INVALID_UNIT_POSITION_TEST
		bool CheckMisplacedUnit();
		void ReplaceMisplacedUnit();
#endif RESET_INVALID_UNIT_POSITION_TEST



#ifdef BUBBLE_BOBBLE_TEST
		bool GetFixedCamera() const { return m_bFixedCamera; }
		void SetFixedCamera(bool val) { m_bFixedCamera = val; }
#endif BUBBLE_BOBBLE_TEST


		bool GetEnableMonsterFly() { return m_bEnableMonsterFlyTest; }


#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
		void EnableManualCameraPosition( bool bEnabled );
#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST




#ifdef REVERSE_GRAVITY_TEST

		void ReverseGravityFrameMove( double fTime, float fElapsedTime );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bool IsInReverseGravityRegion( float fElapsedTime, const D3DXVECTOR3& vPos, float& fSpeedYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool IsInReverseGravityRegion( const D3DXVECTOR3& vPos, float& fSpeedYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void AddReverseGravityRegion( const D3DXVECTOR3& vPos, const float fRange, const float fTime, const float fGravity, int iTeamNumber );

#endif REVERSE_GRAVITY_TEST

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		std::vector<UidType>& GetVecUserUIDforSyncPacket() { return m_vecUserUIDforSyncPacket; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		void						ResetReBirthStoneNumUI(); // protected���� �Űܿ�

#ifdef DUNGEON_ITEM

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER		/// ���� �� ��ȯ���� ����
		bool
#else	ADD_VELDER_ACADEMY_EVENT_MONSTER
		void
#endif	ADD_VELDER_ACADEMY_EVENT_MONSTER		
		ProcSpecialItem( 
			const CX2Item::ItemTemplet* pItemTemplet, 
			CX2GUUser* pCX2GUUser);
		CX2Item::SPECIAL_ITEM_TYPE GetSpecialItemGroupType( const CX2Item::ItemTemplet* pItemTemplet);
		wstring SummonSpiritEffectName(int iSpirit);
		void ClearSpirit();
		void SummonSpiritEffect(int iEffectIndex);
#ifdef SPECIAL_USE_ITEM
		void SpecialItemEffect(int iEffectIndex);
#endif
		void SummonMonster(CX2GUUser* pCX2GUUser, CX2UnitManager::NPC_UNIT_ID npcId, D3DXVECTOR3 vPos);
		
		void SideEffectGenius(CX2GUUser* pCX2GUUser);
#endif

#ifdef DAMAGE_HISTORY
		void SetSendedLeaveRoom(bool bVal) { m_bSendLeaveRoom = bVal; }
		bool GetSendedLeaveRoom() { return m_bSendLeaveRoom; }
#endif

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
		bool Send_EGS_ADMIN_NPC_UNIT_CREATE_REQ_LUA( int unitID, int level, D3DXVECTOR3 vPos, bool bRight, int iAttrib1, int iAttrib2, int iSA1, int iSA2 );
		virtual bool Handler_EGS_ADMIN_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_ADMIN_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef KEYFRAME_CAMERA
		CFirstPersonCamera* GetFPSCamera(){	return &m_FPSCamera; }
#endif KEYFRAME_CAMERA

#ifdef SERV_WORLD_TRIGGER_RELOCATION
		bool Handler_EGS_WORLD_TRIGGER_RELOCATION_REQ( int iTriggerId );
		bool Handler_EGS_WORLD_TRIGGER_RELOCATION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_WORLD_TRIGGER_RELOCATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

//{{ ����� : [2011/3/4/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
		//{{ ����� : [2011/3/2/] //	������ �߰�
		int GetRandomValue() { return rand(); }
		float GetDungeonProgress();

		int	GetNPCUnitNumByNPCID( int iNPCID );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/4/] //	���� �̼�

#ifdef ADDITIONAL_MEMO
		CX2GameUnit *GetNearestUnit( CX2GameUnit* pGameUnit, int iFlag );
#endif

#ifdef SERV_SHARING_BANK_TEST
		wstring GetUserName(int inx);	// ���� ������ ���� �߰�
#endif SERV_SHARING_BANK_TEST

		//{{ kimhc // 2011-07-28 // �ɼǼ�ġȭ
		// 1���� ���� �����ΰ�?
		bool IsDamageFreeGame() const { return m_bIsDamageFreeGame; }
		void SetIsDamageFreeGame( const bool bIsDamageFreeGame_ ) { m_bIsDamageFreeGame = bIsDamageFreeGame_; }	
		//}} kimhc // 2011-07-28 // �ɼǼ�ġȭ
#ifdef NEW_HENIR_TEST
		int							GetStartSecretStageEnteringEvent();
		bool						SecretDungeonStageLoadReq(int iVal);

#endif NEW_HENIR_TEST
#ifdef ELSWORD_WAY_OF_SWORD
		void						UpdateElswordSkillSlotEffect();
#endif ELSWORD_WAY_OF_SWORD
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		bool IsFront( CX2GameUnit *pMyUnit, CX2GameUnit *pTargetUnit );
#endif
		void						UpdateEveElectraSkillSlotIcon();
	#ifdef SERV_RENA_NIGHT_WATCHER
		void						UpdateNightWatcherSkillSlotIcon();
	#endif //SERV_RENA_NIGHT_WATCHER
	#ifdef SERV_RAVEN_VETERAN_COMMANDER
		void						UpdateVeteranCommanderSkillSlotIcon();
	#endif //SERV_RAVEN_VETERAN_COMMANDER
	#ifdef FINALITY_SKILL_SYSTEM //��â��
		void						UpdateCodeEmpressSkillSlotIcon();
	#endif //FINALITY_SKILL_SYSTEM
	#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		void						UpdateAddNasodRulerSkillSlotIcon();
	#endif //SERV_9TH_NEW_CHARACTER			
		void ShowDangerAlarm_LUA( int iAlarmId, float fShowTime, ALARM_COLOR_TYPE eColor, bool bDanger, int iStringIndex );
		void ShowDangerAlarm( int iIndex );
		int LoadDangerAlarm( KLuaManager &luaManager, UidType iUid, int iStateId = -1 );
		int GetDangerAlarm( UidType iUid, int iStateId );
		void ClearDangerAlarm()
		{
			for(UINT i=0; i<m_vecDangerAlarm.size(); ++i)
			{
				SAFE_DELETE( m_vecDangerAlarm[ i ] );
				m_vecDangerAlarm.erase( m_vecDangerAlarm.begin() + i);
				--i;
			}
			m_vecDangerAlarm.clear();

			CKTDGParticleSystem::CParticleEventSequence* pSeq = GetMajorParticle()->GetInstanceSequence( m_hDangerAlarm );
			if( pSeq != NULL )
			{
				pSeq->SetShowObject(false);
			}
		}
		bool GetShowAlarm(int iIndex)
		{
			if( iIndex < 0 )
				return false;
			if( m_vecDangerAlarm[iIndex] == m_pShowAlarm )
				return true;
			return false;
		}
		bool GetAlarmRepeat(int iIndex)
		{
			if( iIndex >= 0 )
				return m_vecDangerAlarm[iIndex]->m_bRepeat;

			return false;
		}
		float GetAlarmDelayTime(int iIndex)
		{
			if( iIndex >= 0 )
				return m_vecDangerAlarm[iIndex]->m_fDelayTime;

			return 0.f;
		}

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
void ShowTimerAlarm_LUA( int iAlarmId, float fShowTime, ALARM_COLOR_TYPE eColor, bool bDanger, float fValue_ );
void ShowTimerAlarm( int iIndex, float fValue_ );
#endif //SERV_HALLOWEEN_EVENT_2013

#ifdef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void AddRemoveDamageEffect( CX2DamageEffect::CEffectHandle hDamage )
		{
			m_vecRemoveDamageEffect.push_back( hDamage );
		}
		void RemoveDamageEffect()
		{
			for(UINT i=0; i<m_vecRemoveDamageEffect.size(); ++i)
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if ( CX2DamageEffect::CEffect* pDamage = ( GetDamageEffect() != NULL ) 
                    ? GetDamageEffect()->GetInstance( m_vecRemoveDamageEffect[i] ) : NULL )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if ( CX2DamageEffect::CEffect* pDamage = ( GetDamageEffect() != NULL && GetDamageEffect()->IsLiveInstance( m_vecRemoveDamageEffect[i] ) == true )
                    ? m_vecRemoveDamageEffect[i] : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				{
					for ( UINT j = 0; j < pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect.size(); ++j )
					{
						CX2DamageEffect::DamageEffectData* pData = pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect[j];
						if ( NULL != pData && true == pData->bAutoDie )
							GetDamageEffect()->DestroyInstanceHandleSilently(m_vecRemoveDamageEffect[i]); //DestroyInstance( pDamage );
					}
				}
			}
			m_vecRemoveDamageEffect.clear();
		}
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void AddRemoveDamageEffect( CX2DamageEffect::CEffect *pDamage )
		{
			m_vecRemoveDamageEffect.push_back( pDamage );
		}
		void RemoveDamageEffect()
		{
			for(UINT i=0; i<m_vecRemoveDamageEffect.size(); ++i)
			{
				CX2DamageEffect::CEffect *pDamage = m_vecRemoveDamageEffect[i];
				if( pDamage != NULL && GetDamageEffect() != NULL && GetDamageEffect()->IsLiveInstance( pDamage ) == true )
				{
					for ( UINT j = 0; j < pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect.size(); ++j )
					{
						CX2DamageEffect::DamageEffectData* pData = pDamage->GetDamageEffectDataInLua().m_vecCreateDamageEffect[j];

						if ( NULL != pDamage && NULL != pData && true == pData->bAutoDie )
							GetDamageEffect()->DestroyInstanceSilently(pDamage); //DestroyInstance( pDamage );
					}
				}
			}
			m_vecRemoveDamageEffect.clear();
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif  ADD_GAME_STAGE_DELETE_DAMAGEEFFECT


#ifdef NEXON_QA_CHEAT_REQ
		void AddHyperModeCount();		//���� ������ �ϳ��� ä����
		void AddAraGage();				//�ƶ� ��� �������� �ϳ��� ä����
		void AddChungCannonBall();		//û ��ź �������� ä����
		void AddChungExtraCannonBall();	//û Ư�� ��ź �������� ä����
#endif //NEXON_QA_CHEAT_REQ


#ifdef SERV_RENA_NIGHT_WATCHER
		const vector<CX2GameUnit*>& GetUnitList() const { return m_UnitList; }							//Unitlist ��ȯ
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		void SetEventMonsterOut( UidType UnitUIDOut ){ m_UnitUIDOut = UnitUIDOut; }
		UidType GetEventMonsterOut() { return m_UnitUIDOut; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SUMMON_MONSTER_CARD_SYSTEM
		int  GetSummonMonsterCardNPCLevel( CX2GUNPC* pNPC, int iLevel );							//�� ������ ���� ��ȯ ������ ���� ��ȯ �Լ�
		void CreateSummonMonsterCardNPC( const CX2Item::SpecialAbility* pSa, CX2GUUser* pCX2GUUser );		//�����ۿ��� �޾ƿ� ���� �̿��� ���� ��ȯ ����
#endif SUMMON_MONSTER_CARD_SYSTEM


		void						SendNpcUnitFirstSyncPacketImmediateForce( vector<UidType>& vecNonNpcSyncUserList_, OUT KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT& kPacket_ );

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		void						SetPetFirstSyncPacketImmediateForce( vector<UidType>& vecNonNpcSyncUserList_ );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef	BATTLE_FIELD_TEST
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		CX2GUUser* GetHostGameUnit() const { return ( null != m_optrHostGameUnit ? m_optrHostGameUnit.GetObservable() : NULL ); }
		void SetHostGameUnit( CX2GUUser* pHostGameUnit_ )
		{
			if ( NULL != pHostGameUnit_ )
				m_optrHostGameUnit = pHostGameUnit_; 
		}
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		CX2GameUnit* GetHostGameUnit() const { return ( null != m_optrHostGameUnit ? m_optrHostGameUnit.GetObservable() : NULL ); }
		void SetHostGameUnit( CX2GameUnit* pHostGameUnit_ )
		{
			if ( NULL != pHostGameUnit_ )
				m_optrHostGameUnit = pHostGameUnit_; 
		}
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		void ResetHostGameUnit() { m_optrHostGameUnit.Reset(); }
//#endif	BATTLE_FIELD_TEST

		virtual void			ResurrectOtherUser() {}
		void					ResurrectOtherUserOK();
		void					ResurrectOtherUserCancel();

		bool CheckAndWarningBusyStateNow( const bool bCheckDead_ = true );		// �ʵ忡���� �ٻۻ��� üũ �� ���
		bool					Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( bool bStop );
		bool					Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK& kKEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );
		virtual bool			Handler_EGS_SKIP_CONTINUE_DUNGEON_NOT();

		virtual bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( UidType unitUID, bool bAllowUseCash = false );
		virtual	bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );
		virtual	bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT );

		void					SetShowSmallGageAndNameForAllGameUnit( const bool bShow_ );

#ifdef MODIFY_DUNGEON_STAGING
		CKTDGUIDialogType		GetDLGMyScore(){return m_pDLGMyScore;}
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		void					SetCinematicUI(bool bVal, float fTime_ = -1.0f ) const
		{ 	
			if( NULL != m_pCinematicUI) 
			{
				if ( fTime_ > 0 )
					m_pCinematicUI->SetCinematicUI(bVal, fTime_);
				else
					m_pCinematicUI->SetCinematicUI(bVal);
			}
		}
#else // SERV_BATTLEFIELD_MIDDLE_BOSS
		void					SetCinematicUI(bool bVal){ if( NULL != m_pCinematicUI) m_pCinematicUI->SetCinematicUI(bVal);}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		void					CanNotInputAndDisalbeNPCAI(bool bVal);
#endif //MODIFY_DUNGEON_STAGING

		void					InitExpandSkillCoolTime();
#ifdef X2TOOL
	bool DungeonStageLoad( int stageNum );
	int	 GetNpcUID4Tool()
	{
		static int iUid = 0;
		return ++iUid;
	}
#endif

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		void						EraseAbuserUserCheckByUID( UidType iUnitUID_ );
#endif //UDP_CAN_NOT_SEND_USER_KICK


#ifdef GUIDE_QUEST_MONSTER
		void					SetQuestMonster( CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ , bool bShow = true);
#endif //GUIDE_QUEST_MONSTER

#ifdef FIX_OBSERVER_MODE
		/// ������ ����� �� PVP ���� UI ǥ�� ���� ������ �����
		UINT GetPositionIndexRed() const { return m_uiPositionIndexRed; }
		void SetPositionIndexRed( const UINT uiPosIndex_ ) { m_uiPositionIndexRed	= uiPosIndex_; }

		UINT GetPositionIndexBlue() const { return m_uiPositionIndexBlue; }
		void SetPositionIndexBlue( const UINT uiPosIndex_ ) { m_uiPositionIndexBlue	= uiPosIndex_; }

		CX2Game::CinematicUI* CX2Game::GetCinematicUI( );

		FORCEINLINE void InitPositionIndexPVPUserGageForObserver() 
		{
			m_uiPositionIndexRed  = 0;
			m_uiPositionIndexBlue = 0;
		};
#endif FIX_OBSERVER_MODE

#ifdef BALANCE_GRAND_ARCHER_20121213
		bool GetNearUnitUidList( IN CX2Room::TEAM_NUM eMyTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT std::map<float, UidType>& mapNearUnitUid_, bool bIgnoreDistanceLimit = false);
#endif //BALANCE_GRAND_ARCHER_20121213

		void SetNpcHardLevel( CX2GUNPC* pNpcUnit_, const int iLevel_ );


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		virtual bool				Send_EGS_DEFENCE_DUNGEON_WAVE_ID_NOT( const int iDungeonWaveID_ ) { return false; };
#endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_EVENT_VALENTINE_DUNGEON
		void						NotfiyCreateValentimeCupCake();
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef EFFECT_TOOL
		void RefreshDamageEffectScript();
#endif //EFFECT_TOOL

#ifdef ADDED_GET_SUBSTAGE_INDEX_IN_SCRIPT
		int		GetNowSubStageIndex();
#endif // ADDED_GET_SUBSTAGE_NUMBER_IN_SCRIPT

#ifdef ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR
		void    DeleteRemainingNpcWhenSubStageClear ();
#endif // ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void                        UpdateHostGameUnit();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		// ������ ���� ���� UnitUID �� ������ ���� ����� NPC �� ������
		// ������ -1 �� �����ϸ� ���� üũ�� ���� �ʰ� ��� NPC �� �˻���
		// MyTeam �� �����ϸ� �ش� Team �� ������ �˻����� ����
		// Check Invincible �� �����ϸ� ���� ���¸� �˻����� ����
		// ����, ����ε� ī�� NPC ���� �Լ�, kimjh
		CX2GUNPC*	GetNearestNpcInSpecificRangeAndExceptUnitUID( const D3DXVECTOR3& vPos_, const UidType UnitUid_, const float fMaxRange_ /* = 1000 */, const int iMyTeam /* = -1*/ , const bool bCheckInvincible /* = false */ );

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
		/// �طο� ȿ����� ���ǵ� �������� ����� ����Ʈ ( ��ȹ ��Ʈ���� ���� )
		const vector<BUFF_TEMPLET_ID>& GetOfficiallyDebuffList() const { return m_vecPossibleOfficiallyDebuffList; }
		void SetOfficiallyDebuffList();

		void DisplayAddHPInformation( IN const float fAddHPValue, IN const D3DXVECTOR3 vPos, IN const D3DXVECTOR3 vDirVector, IN bool bDisplayPercent = true );		/// HP ȸ���� ǥ��
		void DisplayAddMPInformation( IN const float fAddMPValue, IN const D3DXVECTOR3 vPos, IN const D3DXVECTOR3 vDirVector );		/// MP ȸ���� ǥ��
#endif // HAMEL_SECRET_DUNGEON

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		void DisplayDamageInformation( IN const float fDamageValue_, IN const D3DXVECTOR3 vHeadBonePos_, IN const D3DXVECTOR3 vDirVector );	/// ������ ǥ��
#endif //SERV_9TH_NEW_CHARACTER

#ifdef NOT_RENDER_NPC_GAME_EDIT
		bool						GetShowNpcByGameEdit() const { return m_bShowNpcByGameEdit; }
		void						SetShowNpcByGameEdit(bool val)
		{ 
			m_bShowNpcByGameEdit = val; 

			for( UINT i = 0; i < m_NPCUnitList.size(); ++i )
			{
				CX2GUNPC* pGUNPC = m_NPCUnitList[i];
				if( NULL != pGUNPC )
				{
					pGUNPC->SetShowObject( val, true );
				}
			}
		}
#endif // NOT_RENDER_NPC_GAME_EDIT

#ifdef MODFIY_LOG_IN_NPC_SCRIPT_FUNCTION
		void ChatBoxLog( const CHAR* pLog, float fLog );
#endif // MODFIY_LOG_IN_NPC_SCRIPT_FUNCTION

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        void FlushSendFrameAverage();
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		void SendFrameAverage();
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		void SetAlreadySendingFrame(bool bVal_) { m_bIsAlreadySendingFrame = bVal_; }
		bool GetAlreadySendingFrame() const  { return m_bIsAlreadySendingFrame; }
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_CODE_EVENT
		bool						IsEnableCodeEvent_LUA( int iScriptID );
#endif SERV_CODE_EVENT

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
		void SetValentineEventTrigger_LUA( D3DXVECTOR3 vPos );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

protected:

#ifdef	SHOW_UDP_NETWORK_INFO_IN_ROOM
		void						IncreaseUserPacketCountToReceive( const UidType uidGameUnit_ );
#endif	// SHOW_UDP_NETWORK_INFO_IN_ROOM

		void						KeyProcess();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void						UpdateMyGameUnit( const CX2Room::SlotData* pSlotData_, CX2GUUser* pUser );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        void                        CheckAmIHost();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		virtual void				UpdateUnitPointer();
		virtual void				AddUserUnit();

		void						CreateDropIem();
		virtual void				Verify();

		//void						ResetReBirthStoneNumUI(); // public ������ �ű�
	
	
		void						UpdateSkillCoolTime( float fElapsedTime );

		void					NoticeContinue() 
		{ 
			m_fResurrectionRemainTime = 10.0f; 
			if( GetMyUnit() != NULL )
				GetMyUnit()->SetResurrectionTimeOut(false); 
		}

		void					ResurrectionFrameMove( double fTime, float fElapsedTime );

		virtual void			ResurrectionKeyProcess();

		float						m_fResurrectionRemainTime;	/// ��Ȱ �ð� ī��Ʈ (���� ������ε� ���)
		bool						m_bIsSendResurrectionReq;	/// ��Ȱ �ϰڴٴ� EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ�� ���� �Ͽ��ٴ� �÷��׷�, ��Ȱ �ϰڴٴ� Ű�� ���� �� �� ���� �� ������ ����
		bool						m_bResurrectionCountStart;	/// ��Ȱ ī��Ʈ ���� ����
		float						m_fAutoResurrectionTime;	/// �ʵ忡�� DIE���·� 5���̻� ����� �ڵ����� ������ �̵��ϱ� ���� ī��Ʈ

#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		void						InsertGUUserToCheckUDPTimer( const UidType uidCheckUser_ )
		{
			boost::timer timerFirst;
			timerFirst.restart();
			m_mapCheckUDPTimer.insert( std::make_pair( uidCheckUser_, timerFirst ) );
		}
		void				AbuserUserCheck( const CX2GUUser* pCheckUser_ );
		bool				CanCheckAbuser( const CX2GUUser* pCheckUser_, std::map< UidType, boost::timer >::iterator mit_ );
#else

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		void						AbuserUserCheck(UidType iUnitUID_);
	#ifdef UDP_DOWNLOAD_BLOCK_CHECK
		void						AbuserUserReport();
	#endif //UDP_DOWNLOAD_BLOCK_CHECK
#endif //UDP_CAN_NOT_SEND_USER_KICK

#endif // SERV_FIX_SYNC_PACKET_USING_RELAY


		void						AddChatLogByGetItem(const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser,
														bool bIsEDItem);
		void						ProcessGetOutlayItem(const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser,
														int DropItemUID, bool bIsItemSuccess );
		void						ProcessGetEquipItem(const CX2Item::ItemTemplet* pItemTemplet);
		void						ProcessGetEDItem(CX2GUUser* pCX2GUUser, bool bIsEDItem,
													KDungeonRewardED DungeonRewardED, UidType uiGetUnitUID);




		CKTDGUIDialogType			m_pDLGResurrect;		/// �ٸ� ������ ��Ȱ ��ų �������� ���� OK, Cancle���̾�α� 
		UidType						m_UnitUIDToResurrect;		/// ��Ȱ ��ų �ٸ� ������ UID
		CKTDGUIDialogType			m_pDLGContinue;			/// ��Ȱ �� �������� ���� ���̾�α�
		CKTDXDeviceSound*			m_pSoundContinue;		/// ��Ȱ �� ������ ����
		CKTDGUIDialogType			m_pDLGCheckResurrectByUseCash;	/// ��Ȱ�� ĳ�ø� ����ϰڴٰ� Z�� ������ �� ������ ��Ȱ�� ���� �˾�â
		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ m_ReservedPacket;	/// ���� ��Ȱ�� ����� ����ϱ� ���� �뵵, �׷��� UID�� �ʿ��ѵ� �� ��Ŷ�� ����� ����ϴ� ���� �����ε�, �׸��� ������ �ٸ� ������ ��Ȱ ���״ٸ� ĳ�ø� �ڵ����� �ٸ� ���� �츮�µ� ����� �� ��
		float						m_fTimeWaitStopContinue;	/// �ǽð� ĳ�� ��Ȱ�� ���ΰ� ������...ĳ�ú�Ȱ�� ��� â�� ��µǰ� �ִ� ��� 9�ʵ��ȸ� ����� ���� �Ϸ��� �뵵, �� ���Ŀ��� ĳ�� ��Ȱ�� ��� â�� ����
		WCHAR						m_StopContinueAlarmText[MAX_PATH];	/// ĳ�� ��Ȱ�� ���� ���� ��
		bool						m_bStopRebirthCount;	/// true�� �Ǹ� m_fResurrectionRemaintime�� ���� ��Ű�� �ʴ´�.

		float						m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ;	/// 
		bool						m_bEnableSendKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ;	/// �̰� ���� KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ �� ������ �ִ� ����... KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ �� ������ �ִ� ����...

		//{{ kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾�
		bool						m_bCanUseESC;		/// �̷��� ��� ���ӿ� �����ص� ���� ������..?
		//}} kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾�

		GAME_TYPE					m_GameType;			/// ���� Ÿ��
		GAME_STATE					m_GameState;		/// ���� ���� (INIT, PLAY, LOADING, END)
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        UidType                     m_uidMySlotUID;
        UidType                     m_uidHostSlotUID;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		bool						m_bHost;			/// �ټ��� ������ �����ϰԵ� �ʵ��� �ʿ������
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		CX2Room*					m_pRoom;			/// ���� �����ؾ��ϴ� ��� �����͸���...������..
		float						m_fTime;			/// �ʿ� ����
		float						m_fElapsedTime;		/// �ʿ����
		float						m_fGameTime;		/// m_AverageRenderFrameRate(����)�� ���ϱ� ���� ���(PVP������ �̰��� ��� �ϳ�, ���������� SubStage�� �ִ� ���� ��� ��... �ʵ忡���� ����ؾ� �� ��)
		bool						m_bGameWait;		/// ���񽺰� �ƴҶ� Control Ű�� ���� ���� �����

		//char						m_NextInGameID;
		CX2GameUnitoPtr				m_optrMyUnit;			/// ����..���� �ᵵ �� ��?? �ʵ�ý��ۿ����� ���ӽ��۰� ���ÿ� �ڽ��� GUUser�� ������ ���̶�� ������ ������ �ְų� �ص�...
		CX2GameUnitoPtr				m_optrFocusUnit;		/// Ư���� �̽��� ���ٸ� ī�޶� ���ϰ� �־�� �ϴ� ����
		float						m_fFocusTime;		/// ���� �ð����� Ư�� ���ֿ��� ī�޶� ��Ŀ���� ���ϰ� �ֵ��� �Ϸ��� �ϴ� �� ������... UpdateUnitPointer�� ���� ����� �۵� �ϴ°ǰ�...?

		vector<CX2GameUnit*>		m_UnitList;			/// ����, ���� ����� �����͸� �� ������ �ִ� ���� // �ٵ�.. ���� �̰��� ������ �־�� �� ������ ���µ�...
		vector<CX2GUUser*>			m_UserUnitList;		/// ������ �����͸� ������ �ִ� ����
		vector<CX2GUNPC*>			m_NPCUnitList;		/// ���͵� �����͸� ������ �ִ� ����
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		vector<CX2GUUser*>			m_IntruderList;		/// ������ ���� ������ ����
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		CX2GUUser::InputData		m_InputData;	/// ����

		CX2World*					m_pWorld;	/// ����
		CX2Camera*					m_pCamera;	/// ����
		CX2DamageEffect*			m_pDamageEffect;	/// �̸� ����?

 		//CKTDGParticleSystem*		m_pMajorParticle;
 		//CKTDGParticleSystem*		m_pMinorParticle;
 		//CKTDGXMeshPlayer*			m_pMajorXMeshPlayer;
 		//CKTDGXMeshPlayer*			m_pMinorXMeshPlayer;
 		//CX2EffectSet				m_EffectSet;



		bool						m_bBGMOn;	/// ��� ���� (��� true ��)
		bool						m_bLastKillCheck;	/// ���� ���� �ָ� ������ ������ ���̶��..(������ PVP���� ���̶�°� ���������� ������ �ʵ忡�� �������� ����..)
		float						m_fLastKillWaitTime;	/// �� ���� 0�� �� ���ĺ��� lastKillCheck�� true�� ���� false�� �� ���� �Ǵ� ��
		float						m_fLastKillWaitTimeAfterRebirth; /// ��Ȱ �� ����� ��Ȱ ���Ŀ��� HP�� 0�� �Ǿ� ������ m_bLastKillCheck�� true�� �Ǵµ� �� ��츦 �����ϱ� ���� Ÿ�̸�

		bool						m_bEnableCommandKeyProcess;			/// �Ʒü�, ���ʱ۵ ���ؼ� ����Ű �Է� ���ϰ� �� �� 
		bool						m_bEnableAllKeyProcess;				/// �Ʒü�, Extra ������ � ���� Ű�Է��� ���� �� ���� �ϴ� �뵵�� ���

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        int                         m_iPacketSendFrameMoveCount;
        int                         m_iNPCRobustPacketSendIndex;
        DWORD                       m_dwNPCRobustPacketSendBaseFrameMoveCount;
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//		float						m_NPCPacketSendInterval;			/// NPC Packet ���� ����
//		float						m_NPCPacketSendIntervalNow;			/// NPC Packet ���� Ÿ�̸�
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

		float						m_LineSyncPacketSendInterval;		/// MOVING_LINE_MAP_TEST ��Ȱ������ ������� ����
		float						m_LineSyncPacketSendIntervalNow;	/// MOVING_LINE_MAP_TEST ��Ȱ������ ������� ����


		CX2UnitLoader				m_UnitLoader;						/// X2Game ����
		int							m_LoadingPercent;					/// ��� ����
		vector<CreateNPCData*>		m_CreateNPCDataList;				/// CreateNPC()�� ���� �����Ͱ� push_back �ǰ� m_bLastKill�� false�� ��쿡 OnFrameMove���� ������
		KEGS_NPC_UNIT_CREATE_REQ	m_PushCreateNPCReqList;				/// PushCreateNPCReq() �Լ��� ���� NPC�� ������ ������ ���.. ä����, FlushCreateNPCReq()�� ���� SendPacket�� ����

		CX2DropItemManager*			m_pDropItemManager;					/// Dungeon, PVP, BattleField���� �� ��� �� ��
		
		
		CKTDGFontManager::CUKFont*	m_pFontForUnitName;					/// X2Game ����

		bool						m_bStopUnitChain;					/// true�� ��� NPC�� �׾��ٴ� ��Ŷ�� ��� ����, ��� Unit�� Pet�� ���� FrameMove ó���� ���� ���� (StageLoding �� true��, subStage�� Start �� �� false�� �ȴ�. ���� ����ϴ� ���� ���������� ��� ��)

		float							m_fLastkillShotTime;			/// ���� �������� Ŭ���� �ÿ� ���������� ��� (������ ���������� ����ϴ� ��ó�� ���̴µ�...)
		CKTDXDeviceRenderTargetTexture*	m_pRenderTargetLastkillShot;	/// ���� �������� Ŭ���� �ÿ� ���� ���

		
		bool						m_bOpenChatBox;	/// ä�� �ڽ� on/off
		CX2TalkBoxManager*			m_pTalkBoxMgr;	/// X2Game ����

		float						m_fElapsedTimeAfterLastKeyboardInput;	/// Ʃ�丮�󿡼� ���

		vector<NewGameUser>			m_NewGameUserList;		///������ ���� ������ ��������Ʈ

		int							m_RedTeamKill;		/// RedTeamKill �� Ƚ��, TeamDeath������ ���
		int							m_BlueTeamKill;		/// RedTeamKill �� Ƚ��, TeamDeath������ ��


		bool						m_bRenderUserName;	/// ������ ĳ������ ������ ���� ����
		static bool					m_sbRenderNPCName; /// ctrl-key�� ������ ������ npc�̸��� ȭ�鿡 ���(��ư N���� �Ѱ� �����ְ� �Ǿ� ������, ����� �̰���, ó���� StateDungeon���� �Ǿ�����)
		float						m_fSkipTime;		/// �� ����ϴ��� �𸣰ڴ�... 300�� ������ �� ��ŵ�ϴ� ���ϱ�? ��׶��带 ���ؼ�??

		bool						m_bShowF1Help;		/// ��� ����
		vector<wstring>				m_vecF1HelpString;	/// ��� ����
		CKTDXDeviceTexture*			m_pTextureF1Help;	/// ��� ����
		CKTDGFontManager::CUKFont*	m_pFontF1Help;		/// ��� ����

		bool						m_bEnableNPCTalkBox;	/// NPC�� TalkBox�� CommonStateStart���� ���� ���� ����, subStageOpen �ϰ� 2�� �Ŀ� true�� ��
		float						m_fElapsedTimeAfterDisableNPCTalkBox; /// NPC TalkBox�� Disable�� �ð�

		KProtectedType<int>			m_GameScore;		/// ���������� ���
		KProtectedType<int>			m_GameScoreBack;	/// �ʵ� ���� ����� ����
		vector<ScoreTime*>			m_ScoreTimeList;	/// �ʵ� ���� ����� ����

		float											m_fTechPointViewTime;	/// ��ũ����Ʈ�� ���̴� �ð� (�� 1.5�ʷ� �����Ǿ� �� ���̿� �� �ٸ� ��ũ����Ʈ�� �߰��ϸ� �߰��Ǿ� ��������, �߰����� ������ ������� score�� ������)
		KProtectedType<int>								m_TechPoint;	/// ��ũ����Ʈ��  m_fTechPointViewTime ���� TechPoint�� �߰� ���� ������ 0���� �ʱ�ȭ ��
		KProtectedType<int>								m_TechPointFull;	/// ���� ��ũ ����Ʈ
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / ���� �÷��� ��, �ҷ� ���� ���� ���� �ý���
		KProtectedType<int>								m_iSubStageTechPoint;	/// ���� �������� ��ũ ����Ʈ
#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqTech;		/// ��ũ����Ʈ UI �ڵ�


		vector<CX2TextManager::CX2Text*>		m_vecpGuideText;	/// ��� ����
		CX2TextManager::CX2Text*				m_pTextGuideHelp;	/// ��� ����
		float						m_fTimePauseNPCAI;				/// �������ӵ��� ���ö� AI�� ��� ������ų �ð�...(�ٵ� �̰� ����ߴٰ� �ð� �ٵǸ� ���� AI�� false�� �ֵ鵵... AI�� true�� �ǰڳ�..)
		
		bool						m_bFreeCamera;					/// X2Game���� ����� ġƮ������ ����ϴ� ��...
		CFirstPersonCamera			m_FPSCamera;					/// X2Game���� bFreeCamera�� false�� ���� �ʴµ�...

#ifdef KEYFRAME_CAMERA
		bool						m_bWorldCameraEdit;				/// ����ī�޶�
#endif KEYFRAME_CAMERA

		KProtectedType<int>			m_iNpcDiePacket;

		CKTDGUIDialogType				m_pDLGMyScore;				/// ��Ȱ�� �� Score�� ��Ÿ���� ���̾�α�
		CKTDGUIDialogType				m_pDLGSkillSlot;			/// ��ų �����ε�... Gage�Ŵ����� �ִ°� �ƴϾ���...
		
		int							m_TeamScoreAdvRed;				/// ��� ����
		int							m_TeamScoreAdvBlue;				/// ��� ����
		int							m_TeamVPMaxAdvRed;				/// ��� ����
		int							m_TeamVPMaxAdvBlue;				/// ��� ����

		int							m_AllowFrameCount;				/// EGS_REMAINING_TIME_NOT ��Ŷ�� ���� ���� �Ǹ�, �� �ð����� �ش� ��Ŷ�� �ٽ� ���� ���ؼ� 0�̵Ǹ� Ű�Է��� �Ҽ����� ��
		int							m_SpeedHackCheck;				/// AllowFrameCout�� 0�� ��� EGS_REMAINING_TIME_NOT���� �� ���� ���� ��Ű�� 10�� �ʰ��� ��� ��ŷ ������ �Ǵ�

		map<int, int>				m_mapKilledNPCKeyCode;			/// ���� npc�� Ű�ڵ带 ������ ����, ���꽺������ ������ �� �ʱ�ȭ ��, ���������� �ʿ��� ��

		int							m_GetEXPInGame;					/// ��� ����(�ǽð� ���� ���X)

		bool						m_bGameEnd;		/// ������ �����ٴ� �÷���, ������ �ʴ� ���������� �ʿ� ���� ��
		
		bool						m_bShowAllUnitGageBar;		/// �����̵� ���� ������ �������� ������ �ʵ��� false�� ��
#ifdef	ELOG_STATISTICS_NEW_DATA
		KGAME_STATISTICS            m_kGameStatistics;
#else	ELOG_STATISTICS_NEW_DATA
		int							m_AverageRenderFrameRate;	/// ��� ������ ���. �ʵ嵵 �ʿ�
#endif	ELOG_STATISTICS_NEW_DATA

		wstring						m_StateString;				/// ���� ���� ������Ʈ�� ������Ʈ���� �ִ� ��� ��� ���ش�.
		InfoTextManager				m_InfoTextManager;			/// ������ ���� �ϴٴ� �� ȭ�� ����� ������ �޽���
		bool						m_bSkipKeyPressed;			/// Ʃ�丮�󿡼� ���
		
		CX2GUNPCFunc*				m_pNPCFunc;					/// �̰� ����ϴ� NPC�� �ٽ� ���� �ϰ� ������

		// ������ '�̼�' ����
		CKTDGUIDialogType				m_pDLGMissionOpening;	/// ��� ����
		CKTDGUIDialogType				m_pDLGMissionProgress;	/// ��� ����
		
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CRITICAL_SECTION			m_csGameIntruder;	/// ���� ���Կ� ���� ũ��Ƽ�� ����
		HANDLE						m_hEvents[X2_INTRUDE_EVENT_MAX_VALUE];	/// ��� ����
		vector<	IntrudeData* >		m_vecIntrudeData;	/// ��� ����
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD



#ifdef BUBBLE_BOBBLE_TEST
		bool						m_bFixedCamera;		/// ����� ���������� ��� �ǰ� ����...
#endif BUBBLE_BOBBLE_TEST


		bool						m_bEnableMonsterFlyTest;	/// �����ؾ��ҵ�, true �ܿ� �����ϴ� ���� ����

		float						m_arraySkillCoolTime[3];	/// ��� ����
#ifdef NEW_SKILL_TREE
		bool						m_bSkillSlotJustChanged;	/// �������� �� �Ҹ�Ǿ��ų�, ��� �ٲ���ų�, Space Ű�� Slot�� change �� �� ���� UpdateSkillCoolTime�� �������� ���� ��� true
#endif



		
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		KXPT_UNIT_NPC_SYNC_PACK		m_kXPT_UNIT_NPC_SYNC_PACK;	/// npc��ũ��Ŷ�� ��� ���� ����ü
//		std::vector<UidType>		m_vecUserUIDforSyncPacket;	/// �� ������ ��������Ʈ�� �ִ� UID�� �����ͼ� Broad ĳ��Ʈ �� ��� ��
//
//#ifdef SERV_PET_SYSTEM
//		KXPT_UNIT_PET_SYNC_PACK		m_kXPT_UNIT_PET_SYNC_PACK;	/// Pet ��ũ ��Ŷ�� ��� ���� ����ü
//#endif
//#endif SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		bool m_bShowTeleportEffectOnStageStart;		/// ���꽺������, �������� ���� �ִ� ���������� �ʿ��� ����... ���꽺������ Ŭ���� �� �ٸ� ���θ����� ������ ��Żȿ���� ������ (�׿� ���ÿ� �������� ������ �ʴٰ� substagestart �� �� �ٽ� ���̵��� ó����)


#ifdef FPS_SHOOTING_TEST
		int m_iFPSShootingBulletType;
#endif FPS_SHOOTING_TEST




#ifdef RESET_INVALID_UNIT_POSITION_TEST

		std::set< UidType > m_setMisplacedUser;		/// �系������ ���
		std::set< UidType > m_setMisplacedNPC;		/// �系������ ���

#endif RESET_INVALID_UNIT_POSITION_TEST

#ifdef DUNGEON_ITEM
		std::vector<CX2EffectSet::Handle> m_vechEffectSetSpirit;	/// ���� ������ ������ ����Ʈ �ڵ�, �Ծ����� �����ǰ�, ���꽺������ ����ÿ� ����..(��������, ����̾��, ���, �ϸ��Ǿ� ��)
#endif


#ifdef REVERSE_GRAVITY_TEST
		std::vector< ReverseGravityData > m_vecReverseGravityData;	/// ���� ����ϴ� ������ �׷���Ƽ... �����Ͱ� �ƴϱ�..
#endif REVERSE_GRAVITY_TEST

#ifdef DUNGEON_ITEM
		D3DXVECTOR3 m_vDropItemPos;		/// ���� ���� ���ӿ��� ��� �ʿ��ҵ�..

		CX2GameUnitoPtr		m_optrItemDamageEffectGameUnit;	/// ����, ���� ��ȯ �������� ���� ������ ������
		float		m_fItemDamageEffectTime;	/// ��������, ����� �г� ó�� �������� �ִ� ���� ��� �� 0.85�� ���Ŀ� ����ǵ��� �̰����� DamageEffecttime�� ����
		int			m_iItemDamageEffectIndex;	/// ���� ��ȯ ���� (1�� ��������, 3�� ���)
		vector<D3DXVECTOR3> m_vecEffectPos;		/// �ʿ����. ���ɼ�ȯ��Ÿ�ݴ��� �ֵ��� ��ġ�� ��´�.. �׷��� ����� ������ ���� ������ ���� ����. �׳� ���÷� �������־ ��
#endif

#ifdef DAMAGE_HISTORY
		bool		m_bSendLeaveRoom;		// ����ϴ� �� ����
#endif
		
		//{{ kimhc // 2011-07-28 // �ɼǼ�ġȭ
		bool		m_bIsDamageFreeGame;	/// �� ���� ������ ���� ���ľ� ��. 1���� ���� �����ΰ�?
		//}} kimhc // 2011-07-28 // �ɼǼ�ġȭ

		float m_fShowAlarmTime;
		std::vector<DangerAlarm*> m_vecDangerAlarm;
		DangerAlarm *m_pShowAlarm;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hDangerAlarm;
//#ifdef	BATTLE_FIELD_TEST
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        CX2GUUseroPtr			m_optrHostGameUnit;		// �ʵ� �׽�Ʈ��
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		CX2GameUnitoPtr			m_optrHostGameUnit;		// �ʵ� �׽�Ʈ��
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#endif	BATTLE_FIELD_TEST
		
#ifdef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        std::vector<CX2DamageEffect::CEffectHandle>      m_vecRemoveDamageEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		std::vector<CX2DamageEffect::CEffect *> m_vecRemoveDamageEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif

		//{{ 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		std::map< UidType, boost::timer >		m_mapCheckUDPTimer;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}
		
#ifdef MODIFY_DUNGEON_STAGING		
		CinematicUI	*m_pCinematicUI; //ȭ�� ��,�Ʒ� �˰� ����� UI
		bool		m_bCanNotInputAndPauseNPCAI;
#endif //MODIFY_DUNGEON_STAGING

		/** @function : InpuData
			@brief : Ű�Է� üũ
		*///������//2012-10-17//�ڵ� ���� �����̶� ���̱� ���� �߰�
		inline void InputData( IN GAME_ACTION eGameKey_, OUT bool& pureDouble, OUT bool& oneDouble, OUT bool& pure, OUT bool& one )
		{
			if( GET_DOUBLEKEYPURE_STATE( eGameKey_ ) == TRUE )
			{
				pureDouble = true;
			}
			if( GET_DOUBLEKEY_STATE( eGameKey_ ) == TRUE )
			{
				oneDouble = true;
			}
			if( GET_PUREKEY_STATE( eGameKey_ ) == TRUE )
			{
				pure = true;
			}
			if( GET_KEY_STATE( eGameKey_ ) == TRUE )
			{
				one = true;
			}
		}

#ifdef FIX_OBSERVER_MODE
		UINT m_uiPositionIndexRed;	/// ������ ȭ�鿡 ǥ�õ� PVP ���� �� ���� UI ǥ�� ���� �ε���
		UINT m_uiPositionIndexBlue;	/// ������ ȭ�鿡 ǥ�õ� PVP ��� �� ���� UI ǥ�� ���� �ε���
#endif FIX_OBSERVER_MODE

#ifdef BALANCE_DEADLY_CHASER_20130214
		std::set<UidType>	m_setPreLockOnTargetList;
#endif //BALANCE_DEADLY_CHASER_20130214

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
		vector<BUFF_TEMPLET_ID> m_vecPossibleOfficiallyDebuffList;	/// �طο� ȿ����� ���ǵ� �������� ����� ����Ʈ ( ��ȹ ��Ʈ���� ���� )
#endif // HAMEL_SECRET_DUNGEON

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		bool				m_bIsAlreadySendingFrame;
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		UidType m_UnitUIDOut;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	public:
		void StartTcpPingCheck();

		void Handler_EGS_TCP_PING( KEGS_TCP_PING& kEvent );

	private:

		typedef std::map< UINT, KEGS_TCP_PING > KTcpPingMap;

		KTcpPingMap m_mapTcpPing;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

public:
    CX2FrameUDPPack&    GetFrameUDPPack() { return m_kFrameUDPPack; }

protected:

    CX2FrameUDPPack m_kFrameUDPPack;

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


#ifdef NOT_RENDER_NPC_GAME_EDIT
	bool			m_bShowNpcByGameEdit;		///	kimhc // 20131011 // GameEdit ���� ���͵� ���̰� �Ǵ� �Ⱥ��̰� �� �� �ִ� ġƮ
#endif // NOT_RENDER_NPC_GAME_EDIT
};
