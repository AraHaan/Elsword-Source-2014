#pragma once

#include "./X2RidingPet.h"

//{{ seojt // 2009-1-13, 17:11
class CX2SquareUnit;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2SquareUnit>    CX2SquareUnitPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2SquareUnit>    CX2SquareUnitPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 17:11

/** @class : CX2SquareUnit
	@brief : 
	@date	JHKang on 2013-03-26,
		- �ּ� �߰�
	@date	JHKang on 2013-03-27
		- ���ʿ��� Enum( SYNC_UNIT_STATE ), ���� �Լ�( SetActId_LUA() ) ����
	@date	JHKang on 2013-03-28
		- Riding -> RidingPet���� ����
		- Riding ���� State �߰� �� �Ľ� ó��
	@date	JHKang on 2013-04-02
		- Ű�� Ÿ�� ���·� �����ϴ� ��� �߰�
		- ���� ��� ����� ���� RandomSeed �߰�
	@date	JHKang on 2013-04-05
		- Ż �� �߰� �ڵ� �ۼ�
	@date	JHKang on 2013-04-10
		- ������ Ż �� ���忡 �ø��� ���� �ڵ� �ۼ�
		- ������ �ȱ� ���� ���� �߰�
	@date	JHKang on 2013-04-11
		- Ÿ�� �ִ� ���¿� ���� ���� �Լ� ����
		- Square lua ���� ����( �ϳ��� ���� )
	@date	JHKang on 2013-04-12
		- ���� �ɱ� ����ȭ ���� ����
		- Ż �� �ɱ� ���� �� ��� �߰�
*/
class CX2SquareUnit : public CKTDGObject
{
	public:
		/// ���� ���� ����
		enum PERSONAL_SHOP_STATE
		{
			PSS_NONE	= 0,
			PSS_WAIT,
			PSS_SHOP,
		};

		/// ���� ���� ����
#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		enum PERSONAL_SHOP_TYPE
		{
			PST_NONE = 0,

			PST_NORMAL,
			PST_PREMIUM,

			PST_END,
		};
#endif //SERV_UPGRADE_TRADE_SYSTEM

		/// Unit ���� ����ȭ ��
		enum SYNC_UNIT_STATE
		{			
			SUS_W		= 0,	/// wait
			SUS_L		= 1,	/// left
			SUS_R		= 2,	/// right
			SUS_DL		= 4,	/// dash_left
			SUS_DR		= 8,	/// dash_right
			SUS_U		= 16,	/// up
			SUS_D		= 32,	/// down
			SUS_DU		= 64,	/// double_up
			SUS_DD		= 128,	/// double_down
			SUS_DUMY	= 255,	/// none
		};

		/// ����ȭ�� ���� ��
		struct SyncUnitState
		{			
			u_char	m_eventID;		/// ������Ʈ ���̵�
			u_char	m_elapsedFrame;	/// ���� ������Ʈ�� ������ ������ �� (ī��Ʈ ��)		
		};

		/// ����ȭ ������
		struct SyncData
		{
			u_char					m_StateID;
			char					m_StateChangeNum;
			D3DXVECTOR3				m_Pos;
			unsigned char			m_LastTouchLineIndex;

			bool					m_bIsRight;

#ifdef RIDING_SYSTEM
			bool			m_bRidingOn;				/// ž��?
			D3DXVECTOR3		m_vSaddlePos;				/// ���� ��ġ
			D3DXVECTOR3		m_vDirDegree;				/// ���� ȸ�� ��
			wstring			m_wstrRidingMotionName;		/// ��� �̸�
#endif //RIDING_SYSTEM

			SyncData()
			{
				Init();
			}
			void Init()
			{
				m_StateID				= -1;
				m_StateChangeNum		= -1;
				INIT_VECTOR3( m_Pos,	0,0,0 )
				m_LastTouchLineIndex	= 0;
				m_bIsRight				= true;
#ifdef RIDING_SYSTEM
				m_bRidingOn				= false;
				INIT_VECTOR3( m_vSaddlePos, 0, 0, 0 )
				INIT_VECTOR3( m_vDirDegree, 0, 0, 0 )
				m_wstrRidingMotionName	= L"";
#endif //RIDING_SYSTEM
			}
		};

		/// ���� ������
		struct StateData
		{
			string				m_StateName;
			bool				m_bLandConnect;
			string				m_FuncStateStart;
			string				m_FuncFrameMove;
			string				m_FuncEventProcess;
			string				m_FuncStateEnd;

			StateData()
			{
				m_bLandConnect = true;
			}
		};

		/// ���� ����
		struct CommonState
		{
			int m_Wait;				/// ���
			int m_Walk;				/// �ȱ� 
			int m_JumpUp;			/// ���� ��
			int m_JumpDown;			/// ���� �ٿ�
			int m_Dash;				/// �޸���
			int m_DashJumpUp;		/// �޸��� ���� ��
			int m_DashJumpDown;		/// �޸��� ���� �ٿ�

#ifdef RIDING_SYSTEM
			int m_iRidingOn;			/// Ż �� Ÿ��
			int m_iRidingOff;			/// ������
			int	m_iRidingWait;			/// ���
			int m_iRidingWaitHabit;		/// ����
			int m_iRidingWalk;			/// �ȱ�
			int m_iRidingJumpUp;		///	���� ��
			int m_iRidingJumpDown;		/// ���� �ٿ�
			int m_iRidingDash;			/// �޸���
			int m_iRidingDashJumpUp;	/// �޸��� ���� ��
			int m_iRidingDashJumpDown;	/// �޸��� ���� �ٿ�
#endif //RIDING_SYSTEM

			CommonState()
			: m_Wait( 0 )
			, m_Walk( 0 )
			, m_JumpUp( 0 )
			, m_JumpDown( 0 )
			, m_Dash( 0 )
			, m_DashJumpUp( 0 )
			, m_DashJumpDown( 0 )
#ifdef RIDING_SYSTEM
			, m_iRidingOn( 0 )
			, m_iRidingOff( 0 )
			, m_iRidingWait( 0 )
			, m_iRidingWaitHabit( 0 )
			, m_iRidingWalk( 0 )
			, m_iRidingJumpUp( 0 )
			, m_iRidingJumpDown( 0 )
			, m_iRidingDash( 0 )
			, m_iRidingDashJumpUp( 0 )
			, m_iRidingDashJumpDown( 0 )
#endif //RIDING_SYSTEM
			{
			}
		};

	private:
		//CX2SquareUnit() { ASSERT( !"invalid" ); }
		CX2SquareUnit( bool bMyUnit, CX2Unit* pUnit, const WCHAR* pScriptName, bool bTFieldUnit = false
#ifdef ADDED_RELATIONSHIP_SYSTEM
			, char cWeddingStatus = 0
#endif //ADDED_RELATIONSHIP_SYSTEM
			);
		virtual ~CX2SquareUnit(void);

	public: 
		//{{ seojt // 2009-1-14, 21:31
		static CX2SquareUnitPtr CreateSquareUnit( bool bMyUnit, CX2Unit* pUnit, const WCHAR* pScriptName, bool bTFieldUnit = false 
											#ifdef ADDED_RELATIONSHIP_SYSTEM
												, char cWeddingStatus = 0
											#endif //ADDED_RELATIONSHIP_SYSTEM
			)
		{
			CX2SquareUnitPtr    ptrSquareUnit( new CX2SquareUnit( bMyUnit, pUnit, pScriptName, bTFieldUnit
															#ifdef ADDED_RELATIONSHIP_SYSTEM
																, cWeddingStatus
															#endif //ADDED_RELATIONSHIP_SYSTEM
																)
#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                                                , CKTDGObject::KTDGObjectDeleter()
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR                                                
                                                );
            return ptrSquareUnit;
		}//CreateSquareUnit()
		//}} seojt // 2009-1-14, 21:31

//{{ robobeg : 2011-03-18
        static void         AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pUnit, bool bTField = false, CX2UnitViewerUI::UNIT_VIEWER_STATE eState = CX2UnitViewerUI::UVS_NONE );
//}} robobeg : 2011-03-18


//{{ robobeg : 2008-10-18
		virtual void		NotifyShowObjectChanged();
//}} robobeg : 2008-10-18

		bool				GetInit(){ return m_bInit; }
		void				Init();
		void				InitViewer();
		void				InitSystem();
		void				InitState();
	#ifdef RIDING_SYSTEM
		void	InitParticle();
		void	InitWaitMotion();
	#endif //RIDING_SYSTEM


		void SetScriptFileName(const wstring& val) { m_ScriptFileName = val; }




		UidType				GetUID(){ return m_UnitUID; }
		wstring				GetNickName()
		{ 
			if( NULL != m_pUnit )
			{
				return m_pUnit->GetNickName(); 
			}
			else
			{
				return wstring();
			}
		}
		CX2Unit*			GetUnit(){ return m_pUnit; }
		HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		CX2UnitViewerUI*	GetUnitViewer(){ return m_pUnitViewer; }

//{{ robobeg : 2008-10-18
        virtual RENDER_HINT     OnFrameRender_Prepare();
		virtual void            OnFrameRender_Draw();
//}} robobeg : 2008-10-18

		void				RenderRank(int iFinalLeft, int iFinalTop);
#ifdef SERV_INTEGRATION
//{{ oasis907 : ����� [2010.5.17] // 
		void				RenderServer(int iFinalLeft, int iFinalTop);
//}}
#endif SERV_INTEGRATION
		void				RenderName();

// X2UnitVIewerUI �� KTDGManager �� ���, ������ �� ���� ��ǰ�鵵 ���� ���, �����ϱ� ���� ����
        virtual void    NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17
		
		bool		IsAnimationEnd_LUA();
		void		StateChange( char stateID );
		void		StateChange( const char* pStateName );
		void		StateChange_LUA( const char* pStateName );

		u_char		GetStateID( const char* pStateName );		
		u_char		GetNowState() { return m_SyncData.m_StateID; }
		int			GetLastTouchLineIndex() { return m_SyncData.m_LastTouchLineIndex; }
				
		void		SetPos(D3DXVECTOR3 vPos) { m_SyncData.m_Pos = vPos; }
		D3DXVECTOR3	GetPos(){ return m_SyncData.m_Pos; }
        float       GetXPos() { return m_SyncData.m_Pos.x; }
        float       GetYPos() { return m_SyncData.m_Pos.y; }
        float       GetZPos() { return m_SyncData.m_Pos.z; }
		D3DXVECTOR3 GetLandPosition() const { return m_LandPosition; }
#ifdef RIDING_SYSTEM
		D3DXVECTOR3	GetSaddlePos(){ return m_SyncData.m_vSaddlePos; }
		void		SetSaddlePos( IN D3DXVECTOR3 vPos_ ) { m_SyncData.m_vSaddlePos = vPos_; }
		D3DXVECTOR3	GetSaddleDegree(){ return m_SyncData.m_vDirDegree; }
		void		SetSaddleDegree( IN D3DXVECTOR3 vDegree_ ) { m_SyncData.m_vDirDegree = vDegree_; }
		void		SetRidingMotionName( IN const wstring& wstrName_ ) { m_SyncData.m_wstrRidingMotionName = wstrName_; }

		bool GetRidingOn() const { return m_SyncData.m_bRidingOn; }
		void SetRidingOn( IN bool bVal_ ) { m_SyncData.m_bRidingOn = bVal_; }
		void ProcessRidingOn( IN bool bVal_, IN const KRidingPetInfo& _Info = KRidingPetInfo() );
		bool GetCheckWaitHabitElapsedTime() { return m_ElapsedTimeRidingWaitHabit.CheckElapsedTime(); }
		void ResetWaitHabitElapsedTime() { m_ElapsedTimeRidingWaitHabit.ResetSumOfElapsedTime(); }
#endif //RIDING_SYSTEM

		D3DXVECTOR3	GetScale(){ return m_Scale; }
		D3DXVECTOR3	GetDirDegree(){ return m_DirDegree; }
		D3DXVECTOR3	GetDirVector(){ return m_DirVector; }
		bool		GetIsRight(){ return m_SyncData.m_bIsRight; }
		
#ifdef RELATIONSHIP_EFFECT_NOT_DRAW_WHEN_TARGET_POSITION_DATA_IS_MISSING
		// �߰�, �������� 0, 0, 0 ��ǥ�� Ŀ�� ��ƼŬ ����Ʈ��
		// ���̴� ��� ����
		void		SetPlanRender( bool bPlanRender );
#else // RELATIONSHIP_EFFECT_NOT_DRAW_WHEN_TARGET_POSITION_DATA_IS_MISSING
		void		SetPlanRender( bool bPlanRender ){ m_bPlanRender = bPlanRender; }
#endif // RELATIONSHIP_EFFECT_NOT_DRAW_WHEN_TARGET_POSITION_DATA_IS_MISSING
		bool        GetPlanRender() { return m_bPlanRender; }

	

		D3DXVECTOR3	GetBonePos( const WCHAR* pBoneName )		
		{ 
			if( NULL != m_pUnitViewer )
			{
				return m_pUnitViewer->GetXSkinAnim()->GetCloneFramePosition( pBoneName );
			}

			return D3DXVECTOR3(0,0,0);
		}


		D3DXVECTOR3	GetZVector()
		{
			D3DXVECTOR3 vDirVector = GetDirVector();
			if( vDirVector.y != 0.f )
			{
				vDirVector.y = vDirVector.z;
				vDirVector.z = vDirVector.x;
				vDirVector.x = -vDirVector.y;
				vDirVector.y = 0.f;
				D3DXVec3Normalize( &vDirVector, &vDirVector );
			}
			else
			{
				vDirVector.y = vDirVector.z;
				vDirVector.z = vDirVector.x;
				vDirVector.x = -vDirVector.y;
				vDirVector.y = 0.f;
			}
			return vDirVector;
		}




		const CX2GUUser::InputData&		GetInputData() { return (const CX2GUUser::InputData&)m_InputData; }
		void							SetInputData( CX2GUUser::InputData* pInputData );

		bool GetPureLeft_LUA()			{ return m_InputData.pureLeft; }
		bool GetOneLeft_LUA()			{ return m_InputData.oneLeft; }
		bool GetPureDoubleLeft_LUA()	{ return m_InputData.pureDoubleLeft; }
		bool GetOneDoubleLeft_LUA()		{ return m_InputData.oneDoubleLeft; }

		bool GetPureRight_LUA()			{ return m_InputData.pureRight; }
		bool GetOneRight_LUA()			{ return m_InputData.oneRight; }
		bool GetPureDoubleRight_LUA()	{ return m_InputData.pureDoubleRight; }
		bool GetOneDoubleRight_LUA()	{ return m_InputData.oneDoubleRight; }

		bool GetPureUp_LUA()			{ return m_InputData.pureUp; }
		bool GetOneUp_LUA()				{ return m_InputData.oneUp; }
		bool GetPureDoubleUp_LUA()		{ return m_InputData.pureDoubleUp; }
		bool GetOneDoubleUp_LUA()		{ return m_InputData.oneDoubleUp; }

		bool GetPureDown_LUA()			{ return m_InputData.pureDown; }
		bool GetOneDown_LUA()			{ return m_InputData.oneDown; }
		bool GetPureDoubleDown_LUA()	{ return m_InputData.pureDoubleDown; }
		bool GetOneDoubleDown_LUA()		{ return m_InputData.oneDoubleDown; }

		void SetRight_LUA( bool bRight )		{ m_SyncData.m_bIsRight = bRight; }
		bool GetRight_LUA()						{ return m_SyncData.m_bIsRight; }
		bool GetFootOnLine_LUA( bool bRight )	{ return m_bFootOnLine; }
		bool CanDown_LUA();

		void AddPosY_LUA( float fPosY )			{ m_SyncData.m_Pos.y += fPosY; }
		float GetYSpeed_LUA()					{ return m_PhysicParam.nowSpeed.y; }

		void SetXSpeed_LUA( float fSpeedX )	
		{
#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
			if( false == m_bForceMoveSpeed )
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
				m_PhysicParam.nowSpeed.x = fSpeedX; 
		}
		void SetYSpeed_LUA( float fSpeedY )	{ m_PhysicParam.nowSpeed.y = fSpeedY; }

		void SendPacket( int sendCount = 5 );
		void RecvPacket( KEGS_SQUARE_UNIT_SYNC_DATA_NOT& kEGS_SQUARE_UNIT_SYNC_DATA_NOT );
		void RecvTRPacket( KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT& kEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT );

#ifdef ADDED_RELATIONSHIP_SYSTEM
		// BroadCast �Լ��� �Ű����� Ÿ�� ������ IN������ Const ������ ����.
		void SendPacketP2PForWedding( IN vector<UidType>& vecUnitUIDList_ );
		void GetPacketP2PForWedding( OUT KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL& kPacket_ );
		void RecvPacketP2PForWedding( OUT KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL& kPacket_ );
#endif // ADDED_RELATIONSHIP_SYSTEM

		//void GetPacketP2P( KXPT_SQUARE_UNIT_SYNC& kXPT_SQUARE_UNIT_SYNC );
		//void SendPacketP2P( UidType unitUID = 0, int sendCount = 20 );
		//void RecvPacketP2P( KXPT_SQUARE_UNIT_SYNC& kXPT_SQUARE_UNIT_SYNC );

		//void GetPacketRightP2P( KXPT_SQUARE_UNIT_SYNC_RIGHT& kXPT_SQUARE_UNIT_SYNC_RIGHT );
		//void SendPacketRightP2P();
		//void RecvPacketRightP2P( KXPT_SQUARE_UNIT_SYNC_RIGHT& kXPT_SQUARE_UNIT_SYNC_RIGHT );

		CX2SquareUnit::PERSONAL_SHOP_STATE GetPersonalShopState() const { return m_PersonalShopState; }
		void SetPersonalShopState( CX2SquareUnit::PERSONAL_SHOP_STATE personalShopState ) { m_PersonalShopState = personalShopState; }

		wstring GetPersonalShopName() { return m_PersonalShopName; }
		void SetPersonalShopName( const WCHAR* wszShopName ) { m_PersonalShopName = wszShopName; } 

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		SEnum::AGENCY_SHOP_TYPE GetShopType() const { return m_eShopType; }
		void SetShopType(SEnum::AGENCY_SHOP_TYPE val) { m_eShopType = val; }
#else // SERV_UPGRADE_TRADE_SYSTEM
		CX2SquareUnit::PERSONAL_SHOP_TYPE GetShopType() const { return m_eShopType; }
		void SetShopType(CX2SquareUnit::PERSONAL_SHOP_TYPE val) { m_eShopType = val; }
#endif // SERV_UPGRADE_TRADE_SYSTEM

		void UpdateEquippedEmblem();
				
		void AddHistoryUnitState();
		D3DXVECTOR3 GetHistorySummary(std::vector<SyncUnitState> &vecSyncData);
#ifndef RIDING_SYSTEM
		void SetActId_LUA( SYNC_UNIT_STATE actId );		/// 2013.03.27 ���� ������� ����
#endif //RIDING_SYSTEM

		void SetFieldUnit() { m_bTFieldUnit = true; }
		bool GetFieldUnit_LUA() { return m_bTFieldUnit; }

		void SetFirstGetPacket(bool val) { m_bFirstGetPacket = val; }
		void SetFirstSync(bool val) { m_bFirstSync = val; }
		void SetSyncOUStartPos(D3DXVECTOR3 vPos) { m_SyncOUStartPos = vPos; }

		//bool GetIsOtherLine() { return m_bFindOtherLine; }
		//D3DXVECTOR3 GetOtherLinePos() { return m_vOtherLinePos; }
		void DoJumpUp();
		bool IsMyUnit() const { return m_bMyUnit; }

		bool GetNowJump();
		UidType GetPartyUid() { return m_iPartyUID; }
		void SetPartyUid(UidType iPartyUid) { m_iPartyUID = iPartyUid; }

		const CX2SquareUnit::CommonState& GetStateID() const { return m_StateID; }

		CKTDXDeviceSound* PlaySound_LUA( const char* szSoundFileName, bool b3DSound );		

#ifdef UNIT_EMOTION
		bool PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionType);
		CX2Unit::EMOTION_TYPE GetNowEmotionId() { return m_ePlayedEmotion; }
#endif

#ifdef SERV_ED_MONITORING_IN_GAME
		int GetED() { return m_iED; }
#endif

#ifdef RIDING_SYSTEM
		static const WCHAR* GetSquareUnitScriptFileName( CX2Unit::UNIT_CLASS eUnitClass );
#else //RIDING_SYSTEM
		static const std::wstring GetSquareUnitScriptFileName( CX2Unit::UNIT_CLASS eUnitClass );
#endif //RIDING_SYSTEM

#ifdef HEAD_ATTACHED_CAMERA_TEST

		bool BoneAttachedCamera( const wstring& boneName, const D3DXVECTOR3& vEyeOffset /*= D3DXVECTOR3(0, 0, 0) */ );
		bool BoneAttachedCamera( const CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vEyeOffset /*= D3DXVECTOR3(0, 0, 0) */ );
		bool GetOnRollerCoaster() const { return m_bOnRollerCoaster; }

#endif HEAD_ATTACHED_CAMERA_TEST

#ifdef SERV_PET_SYSTEM
		const CX2GameUnit::PhysicParam& GetPhysicParam() const { return m_PhysicParam; }
#endif

#ifdef ADDED_RELATIONSHIP_SYSTEM
		SEnum::WEDDING_STATUS GetWeddingStatus() const { return m_eWeddingStatus; }
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		// ���� �̺�Ʈ DLG ���� ��, �Է��� ���� �ʱ� ���� proteced ���� public ���� ����
		void		SetCanNotInputTime( float fTime_ ){ m_fCanNotInputTime = fTime_; }
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
		bool		IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId );
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�


	protected:
		void		PhysicProcess();

		
		void		StateStart();
		void		StateFrameMove();
		void		StateEventProcess();
		void		StateEnd();

		void		SetSyncEventId(u_char eventId);
		void		SetSyncEventProcess();
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool		EventCheck( float fTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

#ifndef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		void		SetCanNotInputTime( float fTime_ ){ m_fCanNotInputTime = fTime_; }
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

        bool                            m_bBackgroundLoad;

		bool							m_bFirstGetPacket;	/// ��� �ɵ� (false���� �ƴ����� üũ�ϴµ�, false�� ���Ǵ� ��찡 ����)
		bool							m_bInit;			/// Init()�Լ��� ����Ǿ����� ���� (�ȵǾ��ٸ�, FrameMove/Render ���� ����ȵ�)
		bool							m_bMyUnit;
		
		UidType							m_UnitUID;
		CX2Unit*						m_pUnit;			/// �� ������ �ƴ� ��쿡�� �Ҹ��ڿ��� ���� �Ѵ�.
		wstring							m_ScriptFileName;		
		KLuaManager						m_LuaManager;

		float							m_fTime;			/// UnitViewer�� OnFrameMove�� �ѱ�� �뵵(���� ������ ���� �ʰ� ������ �Ѱܵ� �ɵ�)
		float							m_fElapsedTime;		/// ��� �̰͵� ������ ���� �ʿ䰡 ���� (g_pKTDXApp->GetElapsedTime()�� ����ϸ� ��)
		bool							m_bShow;			/// ���� ���̴� �� ����

		CX2GUUser::InputData			m_BeforeInputData;	/// ���� ���̴� �� ����
		CX2GUUser::InputData			m_InputData;
		CX2GUUser::InputData			m_OrgInputData;		/// ���� ���̴� ���� ����

		CX2UnitViewerUI*				m_pUnitViewer;		/// SD�� �ƴ� ĳ������ �𵨸�
        
		CX2GameUnit::PhysicParam		m_OrgPhysicParam;	/// Script�� ����Ǿ� �ִ� �״���� ��
		CX2GameUnit::PhysicParam		m_PhysicParam;		/// ���� ��

		D3DXVECTOR3						m_Scale;		/// ������ ������ UnitViwer�� Scale�� ���� �ʿ�
		D3DXVECTOR3						m_DirDegree;	/// UnitViewer�� Rotate�� ���� �ʿ�. (UnitViwer �Ⱦ��� �ʿ䰡 ��������...?)
		D3DXVECTOR3						m_DirVector;	/// ���⺤��

		SyncData						m_SyncBefore;	/// ������ ������� �ʾƵ� ��
		SyncData						m_SyncData;		/// KEGS_SQUARE_UNIT_SYNC_DATA_REQ�� �׳� ����� �������ִٸ� �̰� �� ��������... ������ ������ VECTOR3�� �ٸ���...
		StateData						m_StateData;	/// ������Ʈ���� ������ (StateStart���� ��ũ��Ʈ�κ��� �о����)
		//bool							m_bFall;
		bool							m_bFootOnLine;	/// true �� ���⺤���� Y���� ����
		D3DXVECTOR3						m_LandPosition;		/// ���� ���� �������̶� ������ �״�� �ʿ��Ѱ�...�׳� ���� POS�� Land Y���� ���� ������ �־ �� �� �ѵ�... �ƴϸ� �� �ݴ뵵 ������ ��
		
		map<char,string>				m_StateNameMap;		/// ����ID, ���¸�
		map<string,char>				m_StateIDMap;		/// ���¸�, ���� ID

		CommonState						m_StateID;			/// Wait, Walk, JumpUp, JumpDown, Dash, DashJumpUp, DashJumpDown�� ���� ���̵� ������ �ִ� ����ü
		


		int								m_SendPacketCount;	/// �ŷ� ���忡�� ����ϴ� ��Ŷ�� ���� Ƚ�� (�ʴ� 100�� ����)
		float							m_fPacketTime;	/// �ŷ� ���忡�� ����ϴ� �ʴ� ��Ŷ�� 100�� ���Ϸθ� �������� ��Ʈ�� �ϱ� ���� second�� �����ϴ� ����

		bool							m_bPlanRender;	/// true �� SD ���� ����
        //CKTDXDeviceXMesh*				m_pPlanMesh;
		//CKTDXDeviceXET*					m_pPlanMeshXET;

        /// ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
        /// - jintaeks on 2009-01-12, 16:47
		CKTDGXSkinAnim*					m_pXSkinAnim;		/// SD ĳ���� Skin Anim
		CKTDXDeviceXSkinMesh*			m_pMesh;			/// SD ĳ���� Skin mesh	

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHeadMarkerMy;		/// ��Ƽ�� �ƴҶ��� �ڽ��� ĳ���͸� ��Ÿ���� ������ ȭ��(?)ǥ��
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHeadMarkerPartyMy;	/// ��Ƽ�� ���� �ڽ��� ĳ���͸� ��Ÿ���� ���� �� ȭ��(?) ǥ��
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHeadMarkerParty;		/// ��Ƽ���� ��Ÿ���� �Ķ� ȭ��(?) ǥ��

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEmblem;			/// Īȣ

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticleHandle			m_hHeadMarkerParticleMy;	/// ��Ƽ�� �ƴҶ��� �ڽ��� ĳ���͸� ��Ÿ���� ������ ȭ��(?)ǥ��
		CKTDGParticleSystem::CParticleHandle			m_hHeadMarkerParticlePartyMy;	/// ��Ƽ�� ���� �ڽ��� ĳ���͸� ��Ÿ���� ���� �� ȭ��(?) ǥ��
		CKTDGParticleSystem::CParticleHandle			m_hHeadMarkerParticleParty;	/// ��Ƽ���� ��Ÿ���� �Ķ� ȭ��(?) ǥ��

		CKTDGParticleSystem::CParticleHandle			m_hPart_Emblem_200;			/// Īȣ
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticle*					m_pHeadMarkerParticleMy;	/// ��Ƽ�� �ƴҶ��� �ڽ��� ĳ���͸� ��Ÿ���� ������ ȭ��(?)ǥ��
		CKTDGParticleSystem::CParticle*					m_pHeadMarkerParticlePartyMy;	/// ��Ƽ�� ���� �ڽ��� ĳ���͸� ��Ÿ���� ���� �� ȭ��(?) ǥ��
		CKTDGParticleSystem::CParticle*					m_pHeadMarkerParticleParty;	/// ��Ƽ���� ��Ÿ���� �Ķ� ȭ��(?) ǥ��

		CKTDGParticleSystem::CParticle*					m_pPart_Emblem_200;			/// Īȣ
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE




		PERSONAL_SHOP_STATE				m_PersonalShopState;	/// ���뺸�ٴ� ���忡���� �ʿ��� ��

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		SEnum::AGENCY_SHOP_TYPE			m_eShopType;			/// ���뺸�ٴ� ���忡���� �ʿ��� ��
#else // SERV_UPGRADE_TRADE_SYSTEM
		PERSONAL_SHOP_TYPE				m_eShopType;			/// ���뺸�ٴ� ���忡���� �ʿ��� ��
#endif // SERV_UPGRADE_TRADE_SYSTEM
		
		wstring							m_PersonalShopName;		/// ���뺸�ٴ� ���忡���� �ʿ��� ��

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hShop;		/// ���뺸�ٴ� ���忡���� �ʿ��� ��
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPremiumShop;	/// ���뺸�ٴ� ���忡���� �ʿ��� ��
//#ifdef TITLE_SYSTEM
        int m_iTitleId;	/// ���� ���� ���� Īȣ�� ���̵�
//#endif

		CKTDGParticleSystem *m_pParticleSystem;		/// ���������� g_pTFieldGame->GetUiParticle(), ���忡���� g_pSquareGame->GetMajorParticle()
		

		// �� ������Ʈ�� ������Ʈ �̺�Ʈ ����
		u_char		m_iActID;						/// ���� Ű���� �Է¿� ���� ������ƮID (��Ʈ����ũ)

		// �ڽ��� ������Ʈ �����丮�� �����ϱ����� ����		
		std::vector<SyncUnitState>		m_vecSyncUnitState;		/// Ű���� �Է��� �޾Ƽ� State�� ��ȯ�� ������ ����

		// Ÿ������ ������Ʈ �����丮�� �����ϱ����� ����
		D3DXVECTOR3						m_SyncOUStartPos_Old;	// Packet�� �ޱ� ������ m_SyncOUStartPos ����
		D3DXVECTOR3						m_SyncOUStartPos;		/// Packet���� ���� �����͸� m_SyncData.m_Pos �� ���� �� �� ���� ������ ���� �뵵�� ���
		std::vector< KFieldFrameInfo >	m_vecFieldFrame;		/// �ٸ� ������ �����ߴ� ������Ʈ ���� (Packet�� ���� ���� ����)
		u_char							m_iOUFrameCount;		/// ���� Sync ��Ŷ�� ���� ���� ���� ���� Sync ��Ŷ�� ���� ������ ����� FrameCount
		u_char							m_iOUNextFrameCount;	/// m_vecFieldFrame�� m_iNowStateIndex �� �ش� �ϴ� ������Ʈ�� �� �������̳� ���� �ؾ� ���� ������Ʈ�� �Ѿ �� �ִ� ���� ���� ����
		int								m_iNowStateIndex;		/// m_vecFieldFrame��  �� ���� ���� ���� index
		bool							m_SyncIsRight_Old;		/// Packet�� �ޱ� ������ m_SyncIsRight
		bool							m_SyncIsRight;			/// Packet���� ���� �����͸� m_SyncData.m_bIsRight �� ���� �� ������ ������ ���� �뵵�� ���
		bool							m_bFirstSync;			/// ��Ŷ ó�� �� ��Ŷ�� �޾Ҵ��� ����

		bool							m_bTFieldUnit;			/// ���� �������� �������������� �˱����� ����

		CKTDXDeviceTexture*				m_pTextureRank;			/// ���� ��ũ ǥ�� �ؽ���
#ifdef SERV_INTEGRATION
		//{{ oasis907 : ����� [2010.5.17] // ���� ���� ���� ���� ������
		CKTDXDeviceTexture*				m_pTextureServer;		/// ���� ������� ��쿡�� �������� ĳ���� �� ���� ������ ������
		//}}
#endif SERV_INTEGRATION

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		map<float,bool>							m_EventTimeStamp;	/// ���� �÷��� �������� EventCheck�� ���� TimeStamp (SOUND_PLAY�� ����ϴ� ���� ���ٸ� �̰͵� ����� ������ ������...)
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<CX2GameUnit::SoundPlayData>		m_vecSoundPlayData;	/// �� ������Ʈ Start���� �ε��� SOUND_PLAY �� ���� ������ (����ϴ� ���� ����)

		UidType							m_iPartyUID;		/// ��Ƽ Unique ���̵�

#ifdef UNIT_EMOTION
		CX2Unit::EMOTION_TYPE			m_ePlayedEmotion;	/// �÷��̵� �̸��
#endif

		bool m_bMixedEmotion[AVATAR_EMOTION_NUM];		/// �̸�� X ���� �̹� Mix �Ǿ����� �ƴ��� �Ǵ�
		CKTDXDeviceSound*				m_pAvatarEmotionSound;	/// Ư�� �ƹ�Ÿ ����� �̸�� ����ϸ� ������ ����

#ifdef HEAD_ATTACHED_CAMERA_TEST
		bool m_bOnRollerCoaster; 		
#endif HEAD_ATTACHED_CAMERA_TEST

#ifdef SERV_ED_MONITORING_IN_GAME
		int m_iED;
#endif

#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
		bool		m_bForceMoveSpeed;
		float		m_fCanNotInputTime;
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE

#ifdef HANABI_VILLAGE
		float m_fRedShowTimer;
		float m_fBlueShowTimer;
#endif HANABI_VILLAGE

#ifdef FIELD_CHARACTER_SCREEN_SHOT
		bool		m_bAlphaScreenShot;
#endif

#ifdef RIDING_SYSTEM
		wstring			m_wstrLobbyWaitMotionName;
		CKTDXCheckElapsedTime m_ElapsedTimeRidingWaitHabit;		/// ���� ���� ��� �ð� ����
		//CX2RidingPetPtr	m_ptrRidingPet;		/// Ż �� ����Ʈ ����Ʈ
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
		SEnum::WEDDING_STATUS m_eWeddingStatus;
#endif //ADDED_RELATIONSHIP_SYSTEM

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
		bool		m_bIsPlayAvatarEmotionSoundWithoutEmotion;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�

#ifdef SERV_GROW_UP_SOCKET
		int		m_iTitleLevel;
#endif SERV_GROW_UP_SOCKET
};
