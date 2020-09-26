#pragma once


#ifdef NEW_CHARACTER_EL

class CX2GUEL : public CX2GUUser
{
public:
	enum EL_STATE_ID
	{
		//�� ĳ���� ���� ������Ʈ
		ESI_BASE = USI_END + 1,
		ESI_STANDUP_ROLLING_FRONT_FRONT,
		ESI_STANDUP_ROLLING_FRONT_BACK,
		ESI_STANDUP_ROLLING_BACK_FRONT,
		ESI_STANDUP_ROLLING_BACK_BACK,
		ESI_STANDUP_ATTACK_FRONT,
		ESI_STANDUP_ATTACK_BACK,
		ESI_STANDUP_ATTACK_FRONT_NO_DOWN,
		ESI_STANDUP_ATTACK_BACK_NO_DOWN,
		ESI_DASH_JUMP_POWER_LANDING,
		ESI_WALL_LANDING,

		//EL ������Ʈ
		///////////////////////////
		ESI_COMBO_Z,			/// �޺� Z
		ESI_COMBO_ZZ,			/// �޺� ZZ
		ESI_COMBO_ZZZ,			/// �޺� ZZZ
		ESI_COMBO_ZZZZ,			/// �޺� ZZZZ
		ESI_COMBO_ZZZX,			/// �޺� ZZZX

		ESI_COMBO_X,			/// �޺� X
		ESI_COMBO_XX,			/// �޺� XX
		ESI_COMBO_XXX,			/// �޺� XXX
		ESI_COMBO_XXZ,			/// �޺� XXZ

		ESI_JUMP_ATTACK_Z,		/// ���� ���� Z
		ESI_JUMP_ATTACK_X,		/// ���� ���� X

		ESI_DASH_COMBO_Z,		/// �뽬 �޺� Z
		ESI_DASH_COMBO_ZZ,		/// �뽬 �޺� ZZ

		ESI_DASH_COMBO_X,		/// �뽬 �޺� X

		ESI_DASH_JUMP_COMBO_Z,	/// �뽬 ���� �޺� Z
		ESI_DASH_JUMP_COMBO_ZZ,	/// �뽬 ���� �޺� ZZ
		ESI_DASH_JUMP_COMBO_ZZZ,/// �뽬 ���� �޺� ZZZ

		ESI_DASH_JUMP_COMBO_X,	/// �뽬 ���� �޺� X
		ESI_DASH_JUMP_COMBO_XX, /// �뽬 ���� �޺� XX
		ESI_DASH_JUMP_COMBO_XX_LANDING, /// �뽬 ���� �޺� XX Landing

		ESI_QUICK_STANDUP_ATTACK_FRONT,	// ��ȸ�� ���� STAND_FRONT
		ESI_QUICK_STANDUP_ATTACK_BACK,	// ��ȸ�� ���� STAND_BACK 


		ESI_CAN_NOT_ATTACK_JUMP_DOWN,		/// ���� �� �� ���� ���� �ٿ� �߰�
		///////////////////////////			
		
		ESI_SWORD_WAIT,			/// ���� �� ���

/////////////////////////////////////////////////////////////////////////////////////////////////
// ����Ʈ
		ESI_SI_A_EL_DODGE_AND_SLASH,			/// ����&������
		ESI_SI_A_EL_DODGE_AND_SLASH_SECOND,		/// ����&������ 2ȸ �Է�
		ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z,	/// ����&������ Z����

		ESI_SI_A_EL_LEAP_CRASH,					// ����
		ESI_SI_A_EL_LEAP_CRASH_JUMP,
		ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING,

		ESI_SI_A_EL_RUSHING_SWORD,				/// ���� �ҵ�
		ESI_SI_A_EL_RUSHING_SWORD_2nd,
		ESI_SI_A_EL_RUSHING_SWORD_3rd,

		ESI_SI_A_EL_KICK,						/// ������

		ESI_SI_A_EL_MEGA_SLASH,					/// �ް� ������

		ESI_SI_A_EL_SONIC_ASSAULT_STAB,			/// �Ҵ� ���Ʈ - ����
		ESI_SI_A_EL_SONIC_ASSAULT_STING,		/// �Ҵ� ���Ʈ - ����

		ESI_SI_SA_EL_WILD_SHOCK,				/// ���ϵ� ��ũ

		ESI_SI_SA_EL_POWER_BURSTER,				/// �Ŀ� ������

		ESI_SI_SA_EL_MEGA_BURSTER,				/// �ް� ������

		ESI_SI_SA_EL_UNLIMITED_BLADE,			/// �𸮹�Ƽ�� ���̵�
/////////////////////////////////////////////////////////////////////////////////////////////////
// 1�� ����
		ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE,		/// �ر� - ��(˭)
		ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO,/// �ر� - ��(˭) �޸�

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���̹� ����Ʈ

		ESI_DASH_COMBO_SSK_ZZ,					/// ��� �޺� ZZ
		ESI_DASH_COMBO_SSK_ZZZ,					/// ��� �޺� ZZZ
		ESI_DASH_COMBO_SSK_ZZX,					/// ��� �޺� ZZX
		ESI_DASH_JUMP_COMBO_SSK_XX,				/// ��� ���� �޺� XX
		ESI_DASH_JUMP_COMBO_SSK_XXX,			/// ��� ���� �޺� XXX

		ESI_SI_A_SSK_CROSS_SLASH,				/// ũ�ν� ������

		ESI_SI_A_SSK_POWER_BREAK,				/// �Ŀ� �극��ũ
		ESI_SI_A_SSK_HEAVY_STUNNER,				/// ��� ���ͳ�

		ESI_SI_SA_SSK_SPIRAL_BLAST,				/// �����̷� ����Ʈ
		
		ESI_SI_SA_SSK_EXTINCTION_START,			/// ���̹� - �ͽ��ü�
		ESI_SI_SA_SSK_EXTINCTION_CHARGE,		
		ESI_SI_SA_SSK_EXTINCTION_ATTACK,
		ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP,
		ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN,

		ESI_SI_SA_SSK_VICTORIOUS_SWORD,			/// �¸��� ��

		ESI_SI_SA_SSK_JUDGEMENT_DRIVE,			/// ������Ʈ ����̺�

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���̷� ����Ʈ

		ESI_DASH_COMBO_SPK_ZX,					/// ��� �޺� ZX
		ESI_DASH_COMBO_SPK_ZZ,					/// ��� �޺� ZZZ
		ESI_DASH_COMBO_SPK_ZZZ,					/// ��� �޺� ZZZ
		ESI_DASH_JUMP_COMBO_SPK_XZ,				/// ��� ���� �޺� XZ
		ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK,	/// ��� �޺� ZX �߰� ����

		ESI_SI_A_SPK_SWORD_ERUPTION,			/// �ҵ� �̷���

		ESI_SI_A_SPK_BIG_BURST,					/// ������

		ESI_SI_SA_SPK_BURST_RISING,				/// ����Ʈ ����¡
		
		ESI_SI_SA_SPK_BURST_WAVE,				/// ����Ʈ ���̺�

		ESI_SI_SA_SPK_SWORDFIRE,				/// �ҵ� ���̾�

		ESI_SI_SA_SPK_INFERNAL_BLADE,			/// ���丣�� ���̵�

		ESI_SI_SA_SPK_ETERNAL_FIRE,				/// ���ͳ� ���̾�

/////////////////////////////////////////////////////////////////////////////////////////////////
// �׷��� ������

// #ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// ������, �����ý� 1-2 �׷��� ������
		// Ŀ�ǵ�
		ESI_DASH_JUMP_COMBO_SGM_XZ,					/// ��� ���� �޺� XZ
		ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP,			/// ��� ���� �޺� XZ �ٿ�
		ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN,			/// ��� ���� �޺� XZ �ٿ�

		ESI_COMBO_SGM_XX_DOWN_X_START,				/// �޺� XX �ٿ� X ����
		ESI_COMBO_SGM_XX_DOWN_X_LOOP,				/// �޺� XX �ٿ� X ����
		ESI_COMBO_SGM_XX_DOWN_X_END,				/// �޺� XX �ٿ� X ��

		ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED,		/// ��� ��� ����, ������� �Ծ� ������ �ڸ� �� ���� �Է� ��, �ش� State �� ��ȯ
		ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED,	/// ��� ��� ����, ������� �Ծ� �ڷ� �ڸ� �� ���� �Է� ��, �ش� State �� ��ȯ
		
		// ��Ƽ��
		ESI_A_SGM_PROVOKE,							/// ���� ���� 
		ESI_A_SGM_PROVOKE_REVENGE,					/// ���� ���� �� �ݰ�

		ESI_A_SGM_DRASTIC_CHARGE_START,				/// �巡��ƽ ���� ���� 
		ESI_A_SGM_DRASTIC_CHARGE_LOOP,				/// �巡��ƽ ���� ����
		ESI_A_SGM_DRASTIC_CHARGE_END,				/// �巡��ƽ ���� �� 

		ESI_A_SGM_SNAP_CRASHER_START,					/// ���� ũ����, ���߿��� ���
		ESI_A_SGM_SNAP_CRASHER_LOOP,					/// ���� ũ����, ���߿��� ��� �� ������
		ESI_A_SGM_SNAP_CRASHER_END,						/// ���� ũ����, ���߿��� ��� �� �����ͼ� �� ���
		ESI_A_SGM_SNAP_CRASHER_GROUND,					/// ���� ũ����, ���󿡼� ���

		// ����� ��Ƽ��
		ESI_SA_SGM_SONIC_BLADE,						/// �Ҵ� ���̵�

		ESI_SA_SGM_CRITICAL_DIVE,					/// ũ��Ƽ�� ���̺�
		ESI_SA_SGM_CRITICAL_DIVE_HIT,				/// ũ��Ƽ�� ���̺� ��� �� ���� �¾��� ��

		ESI_SA_SGM_JUGGERNAUT_BUSTER_START,			/// ���ų� ������, ����
		ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP,			/// ���ų� ������, ����
		ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST,		/// ���ų� ������, ������ ����
		ESI_SA_SGM_JUGGERNAUT_BUSTER_END,			/// ���ų� ������, ��

		ESI_SA_SGM_WAR_PRELUDE,						/// ������ ����

		// ������ ��Ƽ��
		ESI_HA_SGM_SWORD_OF_RELICS,					/// �ҵ� ���� ������	

// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// ������, �����ý� 1-2 �׷��� ������

/////////////////////////////////////////////////////////////////////////////////////////////////
// ����¡ ��Ʈ

		//Ŀ�ǵ�
		ESI_COMBO_SBH_XZ,							/// �޺� XZ
		ESI_COMBO_SBH_XZZ,							/// �޺� XZZ
		ESI_COMBO_SBH_XZZZ,							/// �޺� XZZZ
		ESI_DASH_COMBO_SBH_X,						/// ��� �޺� X
		ESI_DASH_COMBO_SBH_XX,						/// ��� �޺� XX
		ESI_DASH_COMBO_SBH_XXX,						/// ��� �޺� XXX
		ESI_DASH_COMBO_SBH_XXXZ,					///��� �޺� X �� Z

		//��Ƽ��
		ESI_SI_A_SBH_FLAME_SWORD,					/// ȫ���� ��
		ESI_SI_A_SBH_FLAME_SWORD_FINISH,			/// ȫ���� �� ���ᵿ��
		ESI_SI_A_SBH_FIREWORKS,						/// �ҳ���
		ESI_SI_A_SBH_SCALET_ROSE,					/// �Ҳ� ���
		ESI_SI_A_SBH_SCALET_ROSE_SUCCESS,			/// �Ҳ� ��� Ÿ�� ����

		//����� ��Ƽ��
		ESI_SI_SA_SBH_SCALDIC_SWORD,				/// ��Į�� �ҵ�
		ESI_SI_SA_SBH_WALL_OF_PROMINENCE,			/// �Ҳ��� ���
		ESI_SI_SA_SBH_BLAZE_WING,					/// ������ ��
		ESI_SI_SA_SBH_BLAZING_DANCE_START,			/// ����¡ �� ����
		ESI_SI_SA_SBH_BLAZING_DANCE_1ST,			/// ����¡ �� ù��° Ÿ��
		ESI_SI_SA_SBH_BLAZING_DANCE_2ND,			/// ����¡ �� �ι�° Ÿ��
		ESI_SI_SA_SBH_BLAZING_DANCE_3RD,			/// ����¡ �� ����° Ÿ��
		ESI_SI_SA_SBH_BLAZING_DANCE_4TH,			/// ����¡ �� �׹�° Ÿ��
		ESI_SI_SA_SBH_BLAZING_DANCE_FINISH,			/// ����¡ �� ������ Ÿ��

		//������ ��Ƽ��
		ESI_SI_HA_SBH_ANCIENT_FIRE,					/// ���μ�Ʈ ���̾�

/////////////////////////////////////////////////////////////////////////////////////////////////

	};
	class CFirstStrikeData							// �����ý� �׷��� ������ �нú�, ���� �ʽ� ���� ������
	{
		private :
			float	m_fFirstStrikeAddStopTimeDef;			// �����ý� �׷��� ������ �нú�, ���� �ʽ� ( Ŀ�ǵ� ���� �ð� ���� ) 
			float   m_fFirstStrikeAddDamage;				// �����ý� �׷��� ������ �нú�, ���� �ʽ� ( Ŀ�ǵ� ������ ���� ) 
			float   m_fFirstStrikeAddIncreaseMPRate;		// �����ý� �׷��� ������ �нú�, ���� �ʽ� ( Ŀ�ǵ� MP ȸ���� ����) 
		public :
			CFirstStrikeData ( void ) : m_fFirstStrikeAddStopTimeDef ( 0 ),	
				m_fFirstStrikeAddDamage ( 0 ),	m_fFirstStrikeAddIncreaseMPRate ( 0 ) {}

			CFirstStrikeData ( const float fAddStopTime_,  const float fAddDamage_, const float fAddMPRate_ ) :
				m_fFirstStrikeAddStopTimeDef ( fAddStopTime_ ), m_fFirstStrikeAddDamage ( fAddDamage_ ), m_fFirstStrikeAddIncreaseMPRate ( fAddMPRate_ ) {}					

			void SetFirstStrikeData ( const float fAddStopTime_, const float fAddDamage_, const float fAddMPRate_ )
			{
				m_fFirstStrikeAddStopTimeDef	 = fAddStopTime_;
				m_fFirstStrikeAddDamage			 = fAddDamage_;
				m_fFirstStrikeAddIncreaseMPRate	 = fAddMPRate_;
			}

			float GetFirstStrikeAddStopTimeDef ( void ) const		{ return m_fFirstStrikeAddStopTimeDef; }
			float GetFirstStrikeAddDamage ( void ) const			{ return m_fFirstStrikeAddDamage > 0 ? m_fFirstStrikeAddDamage : 1.f; }
			float GetFirstStrikeAddIncreaseMPRate ( void ) const	{ return m_fFirstStrikeAddIncreaseMPRate > 0 ? m_fFirstStrikeAddIncreaseMPRate : 1.f; }
	};


private:
	CX2GUEL( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit );
	virtual ~CX2GUEL(void);

protected:

	virtual void Init( bool bUseTeamPos_, int iStartPosIndex_ );
	virtual void ParseCommonRandomState();
	virtual void InitState();
	virtual void InitComponent();
	virtual void InitPassiveSkillState();
	virtual void SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );
	virtual void SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );
				 
	virtual void CommonStateStartFuture();
	virtual void CommonStateStart();
				 
	virtual void CommonFrameMoveFuture();
	virtual void CommonFrameMove();
				 
	virtual void CommonStateEndFuture();
	virtual void CommonStateEnd();

	virtual void DetonationStateChange( CX2DamageManager::DamageData* pDamageData );

	virtual RENDER_HINT	CommonRender_Prepare();

	virtual bool SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ = CX2SkillTree::ASUT_GROUND );
	virtual void NoStateChangeActionFrameMove();
	virtual float GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

	virtual float GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const;

	virtual CX2GageData* CreateGageData();

	virtual	bool ShouldResetNotBusyTimer();

	void InitStateID();
	void InitStateCommon();
	void InitStateByUnitClass();

	void SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, EL_STATE_ID eStateID_ );
	void SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );


	void ShowActiveSkillCutInAndLight( float fTimeToShow_, bool bOnlyLight_ = false );
	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

	bool CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
	void StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
#ifdef FIX_NO_STATE_SKILL_BUG
	virtual bool IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ );
#endif // FIX_NO_STATE_SKILL_BUG

	CX2ElesisGageData* GetELGageData() { return static_cast<CX2ElesisGageData*>( GetGageData() ); }
	const CX2ElesisGageData* GetELGageData() const { return static_cast<const CX2ElesisGageData*>( GetGageData() ); }
////////////////////////////////////////////////
//���� ��
#ifdef ELSWORD_WAY_OF_SWORD
	void SwordWaitStartFuture();
	void SwordWaitStart();
	void SwordWaitEventProcess();
	void PlayAnimationSwordWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ );	

	void WayOfSwordStateChange();
	void GaleBuffFrameMove();

	bool CanSpecialReactProcess( CX2DamageManager::REACT_TYPE eReActType_ );
#endif // ELSWORD_WAY_OF_SWORD

	float GetFireBallPowerRate() const { return m_fFireBallPowerRate; }
	void SetFireBallPowerRate(float val) { m_fFireBallPowerRate = val; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ������Ʈ �Լ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ESI_DIE_FRONT
	void DieFrontStartFuture();
	void DieFrontStart();
	void DieFrontFrameMove();
	void DieFrontEventProcess();

	//ESI_DIE_BACK
	void DieBackStartFuture();
	void DieBackStart();
	void DieBackFrameMove();
	void DieBackEventProcess();

	//ESI_WAIT
	void WaitStartFuture();
	void WaitStart();
	void WaitEventProcess();

	//ESI_WALK
	void WalkEventProcess();

	//ESI_JUMP_READY
	void JumpReadyEventProcess();

	//ESI_JUMP_UP
	void JumpUpEventProcess();

	//ESI_JUMP_DOWN
	void JumpDownEventProcess();

	//ESI_JUMP_LANDING
	void JumpLandingStartFuture();
	void JumpLandingEventProcess();

	//ESI_DASH
	void DashEventProcess();

	//ESI_DASH_END
	void DashEndStartFuture();
	void DashEndFrameMove();
	void DashEndEventProcess();

	//ESI_DASH_JUMP
	void DashJumpStartFuture();
	void DashJumpStart();
	void DashJumpFrameMoveFuture();
	void DashJumpEventProcess();
	void DashJumpEndFuture();

	//ESI_DASH_JUMP_LANDING
	void DashJumpLandingEventProcess();
	void DashJumpLandingStartFuture();

	//ESI_DAMAGE_SMALL_FRONT
	//USI_DAMAGE_SMALL_BACK
	void DamageSmallEventProcess();

	//ESI_DAMAGE_BIG_FRONT
	//ESI_DAMAGE_BIG_BACK
	void DamageBigStart();
	void DamageBigEventProcess();

	//ESI_DAMAGE_DOWN_FRONT
	void DamageDownFrontFrameMove();
	void DamageDownFrontEventProcess();

	//ESI_DAMAGE_DOWN_BACK
	void DamageDownBackFrameMove();
	void DamageDownBackEventProcess();

	//ESI_DAMAGE_AIR_DOWN_LANDING
	void DamageAirDownLandingEventProcess();

	//ESSI_STANDUP_ROLLING_FRONT_FRONT
	void StandUpRollingFrontFrontEventProcess();
	void StandUpRollingFrontFrontEnd();

	//ESSI_STANDUP_ROLLING_FRONT_BACK
	void StandUpRollingFrontBackEventProcess();
	void StandUpRollingFrontBackEnd();

	//ESSI_STANDUP_ROLLING_BACK_FRONT
	void StandUpRollingBackFrontEventProcess();
	void StandUpRollingBackFrontEnd();

	//ESSI_STANDUP_ROLLING_BACK_BACK
	void StandUpRollingBackBackEventProcess();
	void StandUpRollingBackBackEnd();

	//ESI_STANDUP_ATTACK_FRONT
	void StandUpAttackFrontEventProcess();
	void StandUpAttackFrontEnd();

	//ESI_STANDUP_ATTACK_BACK
	void StandUpAttackBackEventProcess();
	void StandUpAttackBackEnd();
	void StandUpAttackBackStateEndFuture();

	//ESI_STANDUP_ATTACK_FRONT_NO_DOWN
	void StandUpAttackFrontNoDownEventProcess();
	void StandUpAttackFrontNoDownEnd();

	//ESI_STANDUP_ATTACK_BACK_NO_DOWN
	void StandUpAttackBackNoDownEventProcess();
	void StandUpAttackBackNoDownEnd();

	//ESI_DASH_JUMP_POWER_LANDING
	void DashJumpPowerLandingStartFuture();
	void DashJumpPowerLandingStart();
	void DashJumpPowerLandingEventProcess();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ������Ʈ �Լ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ComboZEventProcess();
	void ComboZZEventProcess();

	void ComboZZZEventProcess();
	void ComboZZZZEventProcess();
	void ComboZZZXEventProcess();

	void ComboXEventProcess();
	void ComboXXEventProcess();
	void ComboXXXEventProcess();
	void ComboXXZEventProcess();

	void JumpAttackZEventProcess();
	void JumpAttackZFrameMoveFuture();

	void JumpAttackXEventProcess();
	void JumpAttackXFrameMoveFuture();

	void DashComboZEventProcess();
	void DashComboZZEventProcess();

	void DashComboXEventProcess();

	void DashJumpComboZEventProcess();
	void DashJumpComboZZEventProcess();
	void DashJumpComboZZZEventProcess();

	void DashJumpComboXEventProcess();
	void DashJumpComboXXEventProcess();
	void DashJumpComboXXLandingEventProcess();

#ifdef ADD_CAN_NOT_ATTACK_JUMP_DOWN
	void ESI_CAN_NOT_ATTACK_JUMP_DOWN_EventProcess();
#endif // ADD_CAN_NOT_ATTACK_JUMP_DOWN
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����Ʈ ������Ʈ �Լ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ESI_SI_A_EL_DODGE_AND_SLASH_StartFuture();			// ���� & ������
	void ESI_SI_A_EL_DODGE_AND_SLASH_Init();
	void ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove();
	void ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess();
	void ESI_SI_A_EL_DODGE_AND_SLASH_SECOND_StartFuture();
	void ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_EventProcess();

	void ESI_SI_A_EL_LEAP_CRASH_Init();						// ����
	void ESI_SI_A_EL_LEAP_CRASH_EventProcess();
	void ESI_SI_A_EL_LEAP_CRASH_JUMP_EventProcess();
	void ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING_EventProcess();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	void ESI_SI_A_EL_RUSHING_SWORD_StateStart ();
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	void ESI_SI_A_EL_RUSHING_SWORD_StartFuture();			// ���� �ҵ�
	void ESI_SI_A_EL_RUSHING_SWORD_Init();
	void ESI_SI_A_EL_RUSHING_SWORD_EventProcess();
	void ESI_SI_A_EL_RUSHING_SWORD_2nd_EventProcess();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	void ESI_SI_A_EL_RUSHING_SWORD_2nd_StartFuture();
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	void ESI_SI_A_EL_RUSHING_SWORD_3rd_EventProcess();

	void ESI_SI_A_EL_KICK_Init();							// ������
	void ESI_SI_A_EL_KICK_Start();
	void ESI_SI_A_EL_KICK_EventProcess();
	
	void ESI_SI_A_EL_MEGA_SLASH_Init();						// �ް� ������
	void ESI_SI_A_EL_MEGA_SLASH_EventProcess();

	void ESI_SI_A_EL_SONIC_ASSAULT_STAB_Init();				// �Ҵ� ���Ʈ - ����
	void ESI_SI_A_EL_SONIC_ASSAULT_STAB_EventProcess();

	void ESI_SI_A_EL_SONIC_ASSAULT_STING_Init();			// �Ҵ� ���Ʈ - ����
	void ESI_SI_A_EL_SONIC_ASSAULT_STING_EventProcess();

	void ESI_SI_SA_EL_WILD_SHOCK_Init();					// ���ϵ� ��ũ
	void ESI_SI_SA_EL_WILD_SHOCK_EventProcess();

	void ESI_SI_SA_EL_POWER_BURSTER_Init();					// �Ŀ� ������	
	void ESI_SI_SA_EL_POWER_BURSTER_EventProcess();

	void ESI_SI_SA_EL_MEGA_BURSTER_Init();					// �ް� ������
	void ESI_SI_SA_EL_MEGA_BURSTER_EventProcess();

	void ESI_SI_SA_EL_UNLIMITED_BLADE_Init();				// �𸮹�Ƽ�� ���̵�
	void ESI_SI_SA_EL_UNLIMITED_BLADE_EventProcess();

	void ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess();	// �ر� - ��(˭)
	void ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init();
	void ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart();
	void DoStateEndurancePowerRage(bool bIsWayOfSword_ );
	void ProcessEndurancePowerSkill();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̹� ����Ʈ ������Ʈ �Լ�
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ESI_DASH_COMBO_SSK_ZZ_EventProcess();				/// DASH_COMBO_ZZ
	void ESI_DASH_COMBO_SSK_ZZZ_EventProcess();				/// DASH_COMBO_ZZZ
	void ESI_DASH_COMBO_SSK_ZZX_EventProcess();				/// DASH_COMBO_ZZX
	void ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess();			/// DASH_JUMP_COMBO_XX
	void ESI_DASH_JUMP_COMBO_SSK_XXX_EventProcess();		/// DASH_JUMP_COMBO_XXX

	void ESI_SI_A_SSK_CROSS_SLASH_Init();					// ũ�ν� ������
	void ESI_SI_A_SSK_CROSS_SLASH_EventProcess();

	void ESI_SI_A_SSK_POWER_BREAK_Init();					// �Ŀ� �극��ũ
	void ESI_SI_A_SSK_POWER_BREAK_EventProcess();

	void ESI_SI_A_SSK_HEAVY_STUNNER_Init();					// ��� ���ͳ�
	void ESI_SI_A_SSK_HEAVY_STUNNER_EventProcess();

	void ESI_SI_SA_SSK_SPIRAL_BLAST_Init();					// �����̷� ����Ʈ
	void ESI_SI_SA_SSK_SPIRAL_BLAST_EventProcess();

	void ESI_SI_SA_SSK_EXTINCTION_START_Init();				// ���̹� - �ͽ��ü�
	void ESI_SI_SA_SSK_EXTINCTION_START_Start();
	void ESI_SI_SA_SSK_EXTINCTION_START_StartFuture();
	void ESI_SI_SA_SSK_EXTINCTION_START_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMove();
	void ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMoveFuture();
	void ESI_SI_SA_SSK_EXTINCTION_CHARGE_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_ATTACK_StartFuture();		// �߻� ������Ʈ
	void ESI_SI_SA_SSK_EXTINCTION_ATTACK_FrameMove();		
	void ESI_SI_SA_SSK_EXTINCTION_ATTACK_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMove();	// ���� ����
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMoveFuture();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMove();	
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMoveFuture();
	void InitExtinction();									// �ͽ��üǿ� ���Ǵ� ���� �ʱ�ȭ
	void Extinction_FrameMoveFuture();						// �ͽ��ü� ���� �����ӹ���
	void CreateExctionUIEffect();
	void DestoryExctionUIEffect();

	void ESI_SI_SA_SSK_VICTORIOUS_SWORD_Init();				// �¸��� ��
	void ESI_SI_SA_SSK_VICTORIOUS_SWORD_EventProcess();
	void ESI_SI_SA_SSK_VICTORIOUS_SWORD_FrameMove();

	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_Init();				// ������Ʈ ����̺�
#ifdef ADD_MEMO_1ST_CLASS //��â��
	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStartFuture();
	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStart();
#endif //ADD_MEMO_1ST_CLASS
	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_EventProcess();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̷� ����Ʈ ������Ʈ �Լ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ESI_DASH_COMBO_SPK_ZX_EventProcess();				// DASH_COMBO_ZX
	void ESI_DASH_COMBO_SPK_ZX_FrameMove();
	void ESI_DASH_COMBO_SPK_ZZ_EventProcess();				// DASH_COMBO_ZZ
	void ESI_DASH_COMBO_SPK_ZZZ_EventProcess();				// DASH_COMBO_ZZZ
	void ESI_DASH_COMBO_SPK_ZZZ_FrameMove();
	void ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess();			// DASH_JUMP_COMBO_XZ
	void ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove();
	void ESI_DASH_JUMP_COMBO_SPK_XZ_StartFurture();
	void ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK_StartFurture();

	void ESI_SI_A_SPK_SWORD_ERUPTION_Init();				// �ҵ� �̷���
	void ESI_SI_A_SPK_SWORD_ERUPTION_EventProcess();
	
	void ESI_SI_A_SPK_BIG_BURST_Init();						// ������
	void ESI_SI_A_SPK_BIG_BURST_EventProcess(); 

	void ESI_SI_SA_SPK_BURST_RISING_Init();					// ����Ʈ ����¡
	void ESI_SI_SA_SPK_BURST_RISING_EventProcess();

	void ESI_SI_SA_SPK_BURST_WAVE_Init();					// ����Ʈ ���̺�
	void ESI_SI_SA_SPK_BURST_WAVE_EventProcess();

	void ESI_SI_SA_SPK_SWORDFIRE_Init();					// �ҵ� ���̾�
	void ESI_SI_SA_SPK_SWORDFIRE_EventProcess();

	void ESI_SI_SA_SPK_INFERNAL_BLADE_Init();				// ���丣�� ���̵�
	void ESI_SI_SA_SPK_INFERNAL_BLADE_EventProcess();

	void ESI_SI_SA_SPK_ETERNAL_FIRE_Init();					// ���ͳ� ���̾�
	void ESI_SI_SA_SPK_ETERNAL_FIRE_EventProcess();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �׷��� ������ ������Ʈ �Լ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ESI_DASH_JUMP_COMBO_SGM_XZ_Init ();						// ��� ���� �޺� XZ
	void ESI_DASH_JUMP_COMBO_SGM_XZ_EventProcess();			 	

	void ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP_EventProcess();			// ��� ���� �޺� XZ ����
			
	void ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN_EventProcess();			// ��� ���� �޺� XZ �ٿ�
	

	void ESI_COMBO_SGM_XX_DOWN_X_START_Init ();						// �޺� XX �ٿ� X ����
	void ESI_COMBO_SGM_XX_DOWN_X_START_FrameMove();
	void ESI_COMBO_SGM_XX_DOWN_X_START_StateStart ();					
	void ESI_COMBO_SGM_XX_DOWN_X_START_EventProcess();			 

	void ESI_COMBO_SGM_XX_DOWN_X_LOOP_EventProcess();				// �޺� XX �ٿ� X ����

	void ESI_COMBO_SGM_XX_DOWN_X_END_EventProcess();				// �޺� XX �ٿ� X ��


	void ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED_EventProcess();	// ��� ��� ����, ������� �Ծ� �ڷ� �ڸ� �� ���� �Է� ��, �ش� State �� ��ȯ

	void ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED_EventProcess();		// ��� ��� ����, ������� �Ծ� ������ �ڸ� �� ���� �Է� ��, �ش� State �� ��ȯ


	void ESI_A_SGM_PROVOKE_Init ();									// ���� ����
	void ESI_A_SGM_PROVOKE_StateStart ();
	void ESI_A_SGM_PROVOKE_EventProcess();			 
	void ESI_A_SGM_PROVOKE_StateEnd();

	void ESI_A_SGM_PROVOKE_REVENGE_EventProcess();					// ���� ���� �� �ݰ�


	void ESI_A_SGM_DRASTIC_CHARGE_START_Init ();					// �巡��ƽ ���� ���� 
	void ESI_A_SGM_DRASTIC_CHARGE_START_StateStart ();
	void ESI_A_SGM_DRASTIC_CHARGE_START_FrameMove();
	void ESI_A_SGM_DRASTIC_CHARGE_START_EventProcess();			 
	void ESI_A_SGM_DRASTIC_CHARGE_START_StartFurture();

	void ESI_A_SGM_DRASTIC_CHARGE_LOOP_EventProcess();				// �巡��ƽ ���� ����
	void ESI_A_SGM_DRASTIC_CHARGE_LOOP_FrameMove();

	void ESI_A_SGM_DRASTIC_CHARGE_END_StateStart ();				// �巡��ƽ ���� �� 
	void ESI_A_SGM_DRASTIC_CHARGE_END_EventProcess();			 


	void ESI_A_SGM_SNAP_CRASHER_START_Init ();						// ���� ũ����, ���߿��� ���
	void ESI_A_SGM_SNAP_CRASHER_START_EventProcess();			 
	void ESI_A_SGM_SNAP_CRASHER_START_StateEnd();

	void ESI_A_SGM_SNAP_CRASHER_LOOP_StateStart ();					// ���� ũ����, ���߿��� ��� �� ������
	void ESI_A_SGM_SNAP_CRASHER_LOOP_EventProcess();			 

	void ESI_A_SGM_SNAP_CRASHER_END_StateStart();					// ���� ũ����, ���߿��� ��� �� �����ͼ� �� ���
	void ESI_A_SGM_SNAP_CRASHER_END_EventProcess();			 
	

	void ESI_SA_SGM_SONIC_BLADE_Init ();							// �Ҵ� ���̵�
	void ESI_SA_SGM_SONIC_BLADE_EventProcess();			 
	

	void ESI_SA_SGM_CRITICAL_DIVE_Init ();							// ũ��Ƽ�� ���̺�
	void ESI_SA_SGM_CRITICAL_DIVE_EventProcess();			 

	void ESI_SA_SGM_CRITICAL_DIVE_HIT_EventProcess();				// ũ��Ƽ�� ���̺� ��� �� ���� �¾��� ��
	

	void ESI_SA_SGM_JUGGERNAUT_BUSTER_START_Init ();				// ���ų� ������, ����
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_START_StateStart();			
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_START_EventProcess();			 
		
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_EventProcess();			// ���ų� ������, ����
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_FrameMove();
	
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_EventProcess();		// ���ų� ������, ���� ��
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_StartEnd();

	void ESI_SA_SGM_JUGGERNAUT_BUSTER_END_EventProcess();			// ���ų� ������, ��
	

	void ESI_SA_SGM_WAR_PRELUDE_Init ();							// ������ ����
	void ESI_SA_SGM_WAR_PRELUDE_EventProcess();			 
	

	void ESI_HA_SGM_SWORD_OF_RELICS_Init ();						// �ҵ� ���� ������
	void ESI_HA_SGM_SWORD_OF_RELICS_EventProcess();			 
// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����¡ ��Ʈ ������Ʈ �Լ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void ESI_COMBO_SBH_XZ_EventProcess();					// �޺� XZ
	void ESI_COMBO_SBH_XZ_FrameMove();
	void ESI_COMBO_SBH_XZ_Init();

	void ESI_COMBO_SBH_XZZ_EventProcess();					// �޺� XZZ
	void ESI_COMBO_SBH_XZZ_FrameMove();
	void ESI_COMBO_SBH_XZZ_Init();

	void ESI_COMBO_SBH_XZZZ_EventProcess();					// �޺� XZZZ
	void ESI_COMBO_SBH_XZZZ_FrameMove();
	void ESI_COMBO_SBH_XZZZ_Init();

	void ESI_DASH_COMBO_SBH_X_EventProcess();				// ��� �޺� X
	void ESI_DASH_COMBO_SBH_X_FrameMove();
	void ESI_DASH_COMBO_SBH_X_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_X_Init();

	void ESI_DASH_COMBO_SBH_XX_EventProcess();				// ��� �޺� XX	
	void ESI_DASH_COMBO_SBH_XX_FrameMove();
	void ESI_DASH_COMBO_SBH_XX_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_XX_Init();

	void ESI_DASH_COMBO_SBH_XXX_EventProcess();				// ��� �޺� XXX
	void ESI_DASH_COMBO_SBH_XXX_FrameMove();
	void ESI_DASH_COMBO_SBH_XXX_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_XXX_Init();

	void ESI_DASH_COMBO_SBH_XXXZ_EventProcess();			// ��� �޺� X �� Z
	void ESI_DASH_COMBO_SBH_XXXZ_FrameMove();
	void ESI_DASH_COMBO_SBH_XXXZ_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_XXXZ_Init();

	void ESI_SI_A_SBH_FLAME_SWORD_Init();					// ȫ���� ��
	void ESI_SI_A_SBH_FLAME_SWORD_EventProcess();
	void ESI_SI_A_SBH_FLAME_SWORD_FINISH_EventProcess();

	void ESI_SI_A_SBH_FIREWORKS_Init();						// �ҳ���
	void ESI_SI_A_SBH_FIREWORKS_EventProcess();				

	void ESI_SI_A_SBH_SCALET_ROSE_Init();					// �Ҳ� ���
	void ESI_SI_A_SBH_SCALET_ROSE_EventProcess();
	void ESI_SI_A_SBH_SCALET_ROSE_FrameMove();
	void ESI_SI_A_SBH_SCALET_ROSE_StateEnd();
	void ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_EventProcess();

	void ESI_SI_SA_SBH_SCALDIC_SWORD_Init();				// ��Į�� �ҵ�
	void ESI_SI_SA_SBH_SCALDIC_SWORD_EventProcess();		

	void ESI_SI_SA_SBH_WALL_OF_PROMINENCE_Init();			// �Ҳ��� ���
	void ESI_SI_SA_SBH_WALL_OF_PROMINENCE_EventProcess();
	void ESI_SI_SA_SBH_WALL_OF_PROMINENCE_FrameMove();

	void ESI_SI_SA_SBH_BLAZE_WING_Init();					// ������ ��
	void ESI_SI_SA_SBH_BLAZE_WING_EventProcess();

	void ESI_SI_SA_SBH_BLAZING_DANCE_START_Init();				// ����¡ ��
	void ESI_SI_SA_SBH_BLAZING_DANCE_START_StateStart();
	void ESI_SI_SA_SBH_BLAZING_DANCE_START_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_1ST_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_1ST_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_2ND_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_2ND_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_3RD_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_3RD_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_4TH_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_4TH_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StateEnd();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_FrameMoveFuture();
	
	void ESI_SI_HA_SBH_ANCIENT_FIRE_Init();					// ���μ�Ʈ ���̾�
	void ESI_SI_HA_SBH_ANCIENT_FIRE_EventProcess();
	void ESI_SI_HA_SBH_ANCIENT_FIRE_FrameMove();
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	//~ protected �Լ�
public:
	static CX2GUEL* CreateGUEL( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit );

	static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );

	// Ż ��
	virtual bool CanRidingState();

	virtual void ReInit( bool bTeamPos = false, int iStartPosIndex = -1 );
	virtual void DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ );

	virtual void Win();
	virtual void Lose();
	virtual void Draw();

	virtual void InitEffect();

	virtual void CreateMajorParticleArray();
	virtual void CreateMinorParticleArray();

	static void AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );

	virtual bool SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
	virtual void COMMON_BUFF_FrameMove();

	virtual void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
	void RidingHyperModeFrameMove();
	void CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ = false );
#endif //MODIFY_RIDING_PET_AWAKE
	virtual void HyperModeBuffEffectStart();

	virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );

	virtual void InitEnchantWeaponEffectUnitSpecific();

	virtual int GetComboZStateID() const { return ESI_COMBO_Z; }
	virtual int GetComboXStateID() const { return ESI_COMBO_X; }
	////////////////////////////////////////////////
	//���� ��
#ifdef ELSWORD_WAY_OF_SWORD
	virtual int			GetWayOfSwordState() const;
	void SetWayOfSwordState( const int iWayOfSwordState_ );

	virtual float		GetWayOfSwordPoint() const;
	virtual void		SetWayOfSwordPoint( float fWayOfSwordPoint_ );

	void				Start_SwordmanVigor();
	void				End_SwordmanVigor();
	void				Start_SwordmanDestruction();
	void				End_SwordmanDestruction();

	virtual float GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ );
	virtual void SetCriticalPercent_WayOfSword( IN CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_,
		OUT float& fCriticalPercent_);
#endif // ELSWORD_WAY_OF_SWORD
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	const bool GetUnitClassLine( UNIT_CLASS_LINE UnitClassLine_ );
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

	////////////////////////////////////////////////////
	/// �нú� ���� �Լ�
	// ����� ���� �нú� ó�� ������ ����
	void ChivalryProc_FrameMove( );

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	//// �Ҳ��� ���� �нú� ////
	void StigmaOfFireProc_FrameMove();													// �Ҳ��� ���� ����ð� üũ ������ ����	
	void ApplyDebuffStigmaOfFire( const CX2DamageManager::DamageData& damageData_ );	// �Ҳ��� ���� ���� Ÿ���� üũ�ؼ� ������ �Ŵ� Delegate �Լ�
	
	//// ȫ���� �� ////
	void ApplyDebuffFlameSword( const CX2DamageManager::DamageData& damageData_ );		// ȫ���� �� ���� Ÿ���� üũ�ؼ� ������ �Ŵ� Delegate �Լ�

	//// �Ҳ� ��ȭ ////
	void FireBlossomsProc_FrameMove();													// �Ҳ� ��ȭ ����ð� üũ ������ ����

	//// ����¡ �� ////
	void BlazingDanceFindTargetAndMove();												// Ÿ�� ��ġ�� ã�� �̵���Ű�� �Լ�.
	const bool BlazindDanceCheckTargetPosition( CX2GameUnit* pUnit_, const CKTDGLineMap::LineData* pLineData_ );			// ������ Ÿ�� ��ġ���� üũ�ϴ� �Լ�.

	CFirstStrikeData & GetFirstStrikeData ( void ) { return m_cFirstStrikeData; }

	bool  IsFirstStrikeRelateState ( void )
	{	
		switch ( GetNowStateID() )
		{
			case ESI_COMBO_Z: 
			case ESI_COMBO_X :
			case ESI_JUMP_ATTACK_Z :
			case ESI_JUMP_ATTACK_X :
			case ESI_DASH_COMBO_Z :
			case ESI_DASH_COMBO_X :
			case ESI_DASH_JUMP_COMBO_Z : 
			case ESI_DASH_JUMP_COMBO_X : 
				return true;

			default:
				return false;
		}
		return false;
	}

	bool GetIsProvokeStateChange () const	 { return m_bIsProvokeStateChange; }
	void SetIsProvokeStateChange ( bool val ) { m_bIsProvokeStateChange = val; }

	virtual void ChangeDamageData ();
	
	void DrasticChargeProc_FrameMove ();			// �巡��ƽ ���� ����Ʈ
	void XX_DOWN_XProc_FrameMove ();				// XX �� X ����Ʈ
	void SnapCrasherProc_FrameMove ();			// �巡��ƽ ���� ����Ʈ
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef MODIFY_SET_DAMAGE_DATA
	virtual	void SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ ,OUT float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA


	//~ Public �Լ�
protected:
	// ��ų ��� ����
	const CX2SkillTree::SkillTemplet*	m_pSkillTemplet;	/// ��ų ���ø� ������
	int		m_iPressedSkillSlotIndex;
	float	m_fMPConsume;

#ifdef ELSWORD_WAY_OF_SWORD
	int		m_iPreComboCount;
#endif // ELSWORD_WAY_OF_SWORD

	// ����&������ ���� Ƚ��
	UINT m_uiDodgeAndSlashMaxReuseCount;	// �ִ� ���� Ƚ��
	UINT m_uiDodgeAndSlashReuseCount;		// ���� ���� ���� Ƚ��

	/////////////////////////////////////////////////////
	/// �нú� ��ų ����

	// ���̷� ����Ʈ - ���� ��ü
	float m_fFireBallPowerRate; /// ���̾ ��ȭ ����. �⺻�� 1.f

	// ���̹� ����Ʈ - ����� ����
	float m_fChivalryBuffCheckTime;		// ����� ���� ���� ���� üũ Ÿ��

	UINT m_uiChivalryAttackBuffFactorID;  // ���ݷ� ���� ���� ID
	UINT m_uiChivalryDefenceBuffFactorID; // ���� ���� ���� ID
	UINT m_uiChivalryLevel;				  // ����� ���� ��ų ����

	// �����ý� ���� - �ر� - ��(˭)
	bool m_bIsActiveEndurancePower;		/// Ȱ��ȭ ����

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	// ����¡ ��Ʈ - �Ҳ��� ����
	float m_fStigmaOfFireCoolTime;		// ��ų ��Ÿ��
	bool m_bIsStigmaOfFire;				// Ȱ��ȭ ����
	UINT m_uiSkillLevelStigmaOfFire;	// ��ų ����

	// ����¡ ��Ʈ - �Ҳ� ��ȭ 
	BUFF_TEMPLET_ID m_eFireBlossomsSaveDebuffID;	// ���� �� ����� ID
	float m_fFireBlossomsCoolTime;					// ��ų ��Ÿ��
	float m_fFireBlossomsCheckTime;					// ��ų üũŸ��. ��ų ������ ���� �޶���.
	bool m_bIsFireBlossoms;							// Ȱ��ȭ ����
	UINT m_uiFireBlossomsSkillLevel;				// ��ų ����

	// ����¡ ��Ʈ - ������ �ʴ� �Ҳ�
	bool m_bIsLearnUnextinguishableFire;			// ������� ����
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

	/////////////////////////////////////////////////////
	/// ��Ƽ��, ����� ��Ƽ�� ��ų ����


	// ���̹� - �ͽ��ü�
	float m_fExtinctionRemainTime;		/// �ͽ��ü� ���� �ð� üũ
	float m_fExtinctionDegreeZ;			/// �ͽ��ü� �߻� ����

	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hExctionBG;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hExtinctionArrow;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CKTDGParticleSystem::CParticleHandle				m_hExtinctionArrowParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticle*						m_pExtinctionArrow;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	// �¸��� ��
	bool m_bIsApplyVictoriousSword;		/// �¸��� �� ���� ����. ����&������ ���� ȿ�� �����ϱ� ���� ����
	float m_fDodgeAndSlashCoolTimeDecreaseRate; /// ����&������ ��Ÿ�� ���� ����

	// ESI_DASH_JUMP_COMBO_SPK_XZ
	bool m_bIsDashAttackPossible;		// ��� ���� �Ұ� ���� // ����ZX ���� ��� �ϴ� ���� ���� ���� �ڵ�
	UINT m_uiSPKXZReattakCount;
	//~ protected ����

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

	// �����ý� �׷��� ������ ��� ���� �޺� XZ 
	// �����ý� �׷��� ������ �޺� XX �� X
	int		m_iComboXXdownXNowLoopCount;				// ���� ī��Ʈ ����
	int		m_iComboXXdownXMaxLoopCount;				// �ִ� ī��Ʈ ����

	// �����ý� �׷��� ������ �нú�, ��� ��� ����
	bool	m_bAbleQuickStand;							// ����� ��� ���� ����
	bool	m_bAbleQuickStandAttack;					// ����� ��� �� ��ȸ�� ���� ���� ����

	// �����ý� �׷��� ������ �нú�, ���� ���� 	
	int		m_iRushingSwordAddLoopValue;				// �߰��� �ݺ� Ƚ��
	int		m_iRushingSwordNowLoopValue;				// ���� �ݺ� Ƚ��
	int		m_iDecreaseMPRushingSword;					// ���� MP ����
	
	// �����ý� �׷��� ������ �нú�, ���� �ʽ�
	CFirstStrikeData  m_cFirstStrikeData;				// ���� �ʽ¿� ����ü
	
	// �����ý� �׷��� ������ ��Ƽ��, �巡��ƽ ����
	float	m_fDrasticChargeNowLoopTime;				// ���� �ð�
	float	m_fDrasticChargeLoopDecreaseMPValue;		// MP �Ҹ�

	// �����ý� �׷��� ������ ��Ƽ�� ����
	bool	m_bIsProvokeStateChange;					// ���� ���·� �����ؾ� �ϴ°�?, GUuser �� DamageReact ���� �ش� Boolean ���� üũ�� �� 
														// CX2GUEl �� DamageReact ���� ���¸� �����Ѵ�

	// �����ý� �׷��� ������ ��Ƽ��, ���� ũ����
	CX2EffectSet::Handle	m_hSnapCresherLoop;	// ����ũ���� ���� ����Ʈ
	CX2EffectSet::Handle	m_hSnapCreaherLoopAttack; // ���� ũ���� ���� ���� ����Ʈ ( ���������� ���� ���� ����Ʈ���� �������� ���� �и��� )


	// �����ý� �׷��� ������ �޺�, XX �� X
	CX2EffectSet::Handle	m_hXXDownXLoop;				// XX �� X ���� ����Ʈ �ڵ�

	// �����ý� �׷��� ������ ��Ƽ��, �巡��ƽ ����
	CX2EffectSet::Handle	m_hDrasticChargeLoop;				// �巡��ƽ ���� ���� ����Ʈ �ڵ�, ���� �ٴ� ���׶��
	CX2EffectSet::Handle	m_hDrasticChargeGroundFire;			// �巡��ƽ ���� ���� ����Ʈ �ڵ�, Į�� �ٴ� ��

	// �����ý� �׷��� ������ ����, ���ų� ������
	float	m_fJuggernautBusterNowLoopTime;				// ���� ���� �ð�
	bool	m_bJuggernautBusterIsStartDirectionRight;	// ��� ���� ���� ����



	// ����¡ ��Ʈ - �Ҳ� ���
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle       m_hSBHScaletRoseDamageEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*	m_pSBHScaletRoseDamageEffect;			// ����Ʈ �ӽ� ����
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	// ����¡ ��Ʈ - ����¡ ��
	std::vector<UidType>		m_vecBlazingDanceTargetUid;				// Ÿ�� Unit Uid
	bool						m_bIsThereBlazingDanceTarget;			// ������ Ÿ���� �ִ��� üũ
	bool						m_bIsBlazingDanceLastAttack;			// ������ Ÿ������ üũ(���� ��ȯ�� ����)
	D3DXVECTOR3					m_vec3BlazingDanceTargetPos;			// ������ Ÿ�� ��ġ(���� ��ȯ�� ����)

	// ����¡ ��Ʈ - ȫ���� ��
	bool						m_bIsFrameSword;						// Ȱ��ȭ ����
	UINT						m_uiSkillLevelFlameSword;				// ��ų ����

	// ����¡ ��Ʈ - ���̾� ��
	float						m_fWallOfProminenceLifeTime;			// ���̾� �� ���� �ð�

	// ����¡ ��Ʈ - �Ҳ��� ���� 
	float						m_fStigmaOfFireDamageValue;				// ������ ���� ���� ������ ������. �нú� ��ų�̶� ���� ����.

	// ����¡ ��Ʈ - ��� �޺� ���� ������ ��
	float						m_fDashComboStartDelay;					// ��� �޺��� ������( XXX�� XXXZ ) �Ŀ� �ٽ� ��� �޺��� �����ϱ� ������ �ð�

	// ����¡ ��Ʈ - ���μ�Ʈ ���̾� 
	float						m_fAncientFireRenderTime;				// ������ ���ϴ� �ð�

#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

public:

	//~ Public ����
};

#endif // NEW_CHARACTER_EL