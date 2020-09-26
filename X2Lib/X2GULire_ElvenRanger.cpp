#include "StdAfx.h"
#include ".\x2gulire_elvenranger.h"

#ifdef SERV_RENA_NIGHT_WATCHER

namespace _CONST_RENA_
{
	const float START_OF_DELAYED_FIRING_DELAY_ACTIVE_TIME	= 10.f;	/// ������ ��ȣź ���� �ð�
	const int	START_OF_DELAYED_FIRING_DELAY_MAX_COUNT		= 6;	/// ������ ��ȣź �ִ� ��ø ����

#ifdef BALANCE_PATCH_20131107
	const int   MAX_ARROW_OF_EXPLOSION	= 5;							/// ������ ȭ�� �ִ� ���� ��
#else //BALANCE_PATCH_20131107
	const int   MAX_ARROW_OF_EXPLOSION	= 4;							/// ������ ȭ�� �ִ� ���� ��
#endif //BALANCE_PATCH_20131107

	const int   MAX_PRIOR_PLANNED_BLOW_ACTIVE_NUM = 14;				/// ���� �ϰ� �ִ� ��ø ��
	const int   MIN_PRIOR_PLANNED_BLOW_ACTIVE_NUM = 5;				/// ���� �ϰ� �ּ� ��ø ��
	const int	PRIOR_PLANNED_BLOW_ACTIVE_LEVEL_1 = 5;				/// ���� �ϰ� ����Ʈ ���� 1
	const int	PRIOR_PLANNED_BLOW_ACTIVE_LEVEL_2 = 9;				/// ���� �ϰ� ����Ʈ ���� 2
	const int	PRIOR_PLANNED_BLOW_ACTIVE_LEVEL_3 = 14;				/// ���� �ϰ� ����Ʈ ���� 3

#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���
	const float TIME_GAP_OF_CREATE_CRYOTRON_BOLT_GUIDE_SHOT = 0.1358f / 2;  // ũ���ƿ�Ʈ�� ��Ʈ ���� �߻�ü��
																		// �ֱ������� ������ ����Ʈ�� �����ϴ�
																		// �ð� ����
	const float START_TIME_OF_CRYOTRON_BOLT_CREATE_GUIDE_SHOT = 1.144f;	// ũ���ƿ�Ʈ�� ��Ʈ ���� �߻�ü
																		// ������ �����ϴ� �ð�
	const float END_TIME_OF_CRYOTRON_BOLT_CREATE_GUIDE_SHOT	  = 2.502f;	// ũ���ƿ�Ʈ�� ��Ʈ ���� �߻�ü
																		// ������ ������ �ð�

	/* ũ���ƿ�Ʈ�� ��Ʈ �߻�ü ������ = ( �� �ð� - ���� �ð� ) / �ð� ���� */
	const int MANY_CRYOTRON_BOLT_GUILDE_SHOT = 
		static_cast <int> ( ( END_TIME_OF_CRYOTRON_BOLT_CREATE_GUIDE_SHOT - START_TIME_OF_CRYOTRON_BOLT_CREATE_GUIDE_SHOT ) / TIME_GAP_OF_CREATE_CRYOTRON_BOLT_GUIDE_SHOT );
	
#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

}

#endif  SERV_RENA_NIGHT_WATCHER




#ifdef SERV_APRIL_FOOLS_DAY
static const CX2GUUser::SkillCutIn s_SkillCutInSetFoolsDay = CX2GUUser::SkillCutIn( L"HQ_CutIn_Lena_Gura.dds");
#endif //APRIL_FOOLS_DAY

// �ʻ�� �� �� cut in image 
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] = 
{
	// ����, �Ĺ�, ����
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lena_1.dds"),// D3DXVECTOR2(655.0f,655.0f), D3DXVECTOR3(192.0f,496.0f,0.0f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lena_2.dds"),// D3DXVECTOR2(691.0f,691.0f), D3DXVECTOR3(202.0f,423.0f,0.0f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Lena_3.dds"),// D3DXVECTOR2(670.0f,670.0f), D3DXVECTOR3(275.0f,471.0f,0.0f) ),
	},


	// ���� ����Ŀ
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Wind_Sneaker_1.dds"),// D3DXVECTOR2( 677.f, 677.f ), D3DXVECTOR3( 221.f, 436.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Wind_Sneaker_1.dds"),// D3DXVECTOR2( 677.f, 677.f ), D3DXVECTOR3( 221.f, 436.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Wind_Sneaker_1.dds"),// D3DXVECTOR2( 677.f, 677.f ), D3DXVECTOR3( 221.f, 436.f, 0.f ) ),
	},

	// �׷��� ��ó
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Grand_Archer_1.dds"),//D3DXVECTOR2( 682.f, 682.f ), D3DXVECTOR3( 342.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Grand_Archer_1.dds"),//D3DXVECTOR2( 682.f, 682.f ), D3DXVECTOR3( 342.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Grand_Archer_1.dds"),//D3DXVECTOR2( 682.f, 682.f ), D3DXVECTOR3( 342.f, 452.f, 0.f ) ),
	},

#ifdef SERV_RENA_NIGHT_WATCHER
	// ����Ʈ ����
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Night_Watcher_1.dds"),//D3DXVECTOR2( 682.f, 682.f ), D3DXVECTOR3( 342.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Night_Watcher_1.dds"),//D3DXVECTOR2( 682.f, 682.f ), D3DXVECTOR3( 342.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Rena_Night_Watcher_1.dds"),//D3DXVECTOR2( 682.f, 682.f ), D3DXVECTOR3( 342.f, 452.f, 0.f ) ),
	},
#endif SERV_RENA_NIGHT_WATCHER
};









CX2GULire_ElvenRanger::CX2GULire_ElvenRanger( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD 
	int frameBufferNum, CX2Unit* pUnit )
: CX2GUUser( unitIndex, teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD 
	frameBufferNum, pUnit )
, m_hWindSneakerFoot( INVALID_EFFECTSET_HANDLE )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hDESlideKick( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pDESlideKick( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_iArrowMeshPlayerIndex( 0 )
#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���
, m_fPreRenderParamColorA (1.f)
, m_fRenderParamColorA ( 1.f )
, m_fSpiralStrikeRemainingTime ( 5.5f )
#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
, m_fPriorPlannedBlowIncreaseHPRate ( 0.f )
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
{
	m_bReAttackZ1Right		= false;
	m_fReAttackZ1Time		= 0.0f;

	m_JumpXCount			= 0;
	m_fJumpXTime			= 0.0f;

#ifdef OLD_HAND_SLASH_TRACE
//{{AFX
	m_bFootSlash			= false;
	m_pFootSlashTrace		= NULL;
	m_pFootSlashTraceTip	= NULL;
//}}AFX
#endif OLD_HAND_SLASH_TRACE


	m_pMPChargeA			= NULL;
	m_pMPChargeB			= NULL;
	m_pMPChargeC			= NULL;

	m_hSeqMPEnergy				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPEnergyCenter		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPChargeWave			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPChargeFeather		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPCharge2Up			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPCharge2UpHigh		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPChargePlus			= INVALID_PARTICLE_SEQUENCE_HANDLE;

	InitializeLireMajorParticleArray();
	InitializeLireMinorParticleArray();

	m_hMeshSpinningKick1				= INVALID_MESH_INSTANCE_HANDLE;
	m_hSeqSpinningKick1					= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqSpinningKick2					= INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ���̺� ű ���� ���� �ҽ� ���� ( ĳ���� ��ũ��Ʈ �� ����Ʈ ������ �̰� )
	m_hMeshDiveKickBombing				= INVALID_MESH_INSTANCE_HANDLE;
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	


	m_fArrowRangeRate				= 1.f;
	m_fMPConsumeRateSiegeArrow		= 1.f;
	m_fAttackPowerRateArrow			= 1.f;
	m_fDamageRelSiegeArrow			= 1.f;
	m_iAddHitCountArrow = 0;
	m_fHitGapArrow = -1.f;
	m_nBackShotCount = 0;
	m_fHitAddMp = -1.f;

#ifdef SKILL_30_TEST
	m_MaxEntangleTrapCount = 0;
	m_fEntangleTrapAttackDuration = 0.f;
	m_iDoubleJumpCount = 0;

#endif SKILL_30_TEST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	m_MaxAngerOfElfTrapCount = 0;
	m_vecAngerOfElfTrapUID.clear();
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.3] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
	m_pFriendshipNatureData = NULL;
#endif NEW_SKILL_2010_11

#ifdef SKILL_BALANCE_20110728
	m_bNatureForcePossible = false;
#endif

#ifdef SPECIAL_USE_ITEM
	m_fThrowStartTime		= 0.6666f;
	m_vThrowStartPosOffset	= D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity	= D3DXVECTOR3(1600.f, 200.f, 0.f);
#endif SPECIAL_USE_ITEM

#ifdef	SERV_TRAPPING_RANGER_TEST
	m_hTrapBlade					= INVALID_MESH_INSTANCE_HANDLE;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_hEffectTrapArrow				= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hEffectCallOfRuin				= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectTrapArrow				= NULL;
	m_pEffectCallOfRuin				= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	m_fDamageDataChangeTime			= 0.f;
#ifdef SERV_RENA_NIGHT_WATCHER
	m_fDamageDataChangeTime1		= 0.f;
#endif SERV_RENA_NIGHT_WATCHER
	m_fDamageDataChangeTime2		= 0.f;

	m_bUseTrapBlade					= false;

	m_vecOptrEvoke.clear();
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef BALANCE_PATCH_20120329
	m_bIsSiegeSkill = false;
	m_iSiegeAttackCount = 0;
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
	m_bAccelDashEventFlag						= false;							/// �����ð� �̻� ��ø� �� ��, �׼� ��ø� ����� �� �ִ�.( �ӽ� )

	m_bUsePriorPlannedBlow						= false;							/// ���� �ϰ� �ɸ� �� ���� ����( ��ų�� ������� )
	m_fPriorPlannedBlowDownValue				= 0.f;								/// ���� �ϰ� �ٿ� ��ġ ������
	m_fPriorPlannedBlowIncreaseDamage			= 0.f;								/// ���� �ϰ� ���� ������ ��
	m_bActivePriorPlannedBlow					= false;							/// ���� �ϰ� ȿ�� �ߵ� ����
	m_bActivePriorPlannedBlowAttack				= false;							/// ���� �ϰ��� ����� ���� �Ǻ�
	m_bActivePriorPlannedBlowWalkCancelChange	= false;
	m_iPriorPlannedBlowHitCount					= 0;								/// ���� �ϰ� �޺� ���� ��
	m_bPossibleHitCount							= false;							/// ���� �ϰ� �޺� ���� �� ���� ���� ����( �� State�� �������� Hit ���ѵ� �ѹ��� Count �Ǿ�� �Ѵ�. )
	m_fPriorPlannedBlowTime						= 0.f;								/// ���� �ϰ� �޺� ���� �ð�
	m_hPriorPlannedBlowEffect					= INVALID_EFFECTSET_HANDLE;		/// ���� �ϰ� ����Ʈ �ʱ�ȭ
	m_iNowPriorPlannedBlowLevel					= 0;								/// ���� ����Ǿ� �ִ� ���� �ϰ� ����Ʈ ����

	m_bActiveStartOfDelayedFiring				= false;							/// ������ ��ȣź ���� ����
	m_fStartOfDelayedFiringIncreaseDamage		= 0.f;								/// ������ ��ȣź �߰� ������
	m_fStartOfDelayedFiringCoolTime				= 0.f;								/// ������ ��ȣź ���� �ð�
	m_hActiveStartOfDelayedFiring				= INVALID_EFFECTSET_HANDLE;		/// ������ ��ȣź ������ Ȯ�� ����Ʈ
	m_fStartOfDelayedFiringPowerRate			= 0.f;

	m_bShowTrapBladeToWinMotion					= false;							/// �¸��� ������ ǥ��

	m_iIndexNum									= 0;								/// ������ ��ȣź ������ ����Ʈ �ε��� ������

	CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( GetUnitUID() );
	if( NULL != pSlotData && NULL != pSlotData->m_pUnitViewer )
		pSlotData->m_pUnitViewer->SetHideSelectUnitWeapon();					/// �ɸ��� ���� ȭ���� ������ ���� �����
#endif SERV_RENA_NIGHT_WATCHER

	m_bDownForce = false;
	m_fChangeMPTime = 0.f;
	m_fMPChargeSoundTime = 0.f;
	m_fMPChargeWaitTime = 0.f;

	m_delegateFriendshipOfNature = DelegateFriendshipOfNature();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectTrapArrowFungus					= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectTrapArrowFungus					= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_bAbleAgileMovement						= false;
	m_fSiegeDamageByPassive						= 1.f;
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���
	m_hSpiralStrikeMiddleEffect = INVALID_EFFECTSET_HANDLE;
	m_hSpiralStrikeMiddleLightEffect = INVALID_EFFECTSET_HANDLE;
	m_vecCryotronBoltDamageEffect.clear();
#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

#ifdef ADD_RENA_SYSTEM //��â��
	m_fNaturalForceValueCharge = 0.f;
	m_hNaturalForceEffect = INVALID_EFFECTSET_HANDLE;
	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;

	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	if( GetAccessNowDamageRelateSkillData().m_byteRelateData >= 10 )
		GetAccessNowDamageRelateSkillData().m_byteRelateData = 0;
	m_iBeforeNaturalForceCount = 0;
#endif //ADD_RENA_SYSTEM
}



CX2GULire_ElvenRanger::~CX2GULire_ElvenRanger(void)
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindSneakerFoot );

#ifdef OLD_HAND_SLASH_TRACE
//{{AFX
	SAFE_DELETE_KTDGOBJECT( m_pFootSlashTrace );
	SAFE_DELETE_KTDGOBJECT( m_pFootSlashTraceTip );
//}}AFX
#endif OLD_HAND_SLASH_TRACE



	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPEnergy );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPEnergyCenter );

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPChargeWave );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPChargeFeather );

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPCharge2Up );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPCharge2UpHigh );

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPChargePlus );

	SAFE_DELETE_KTDGOBJECT( m_pMPChargeA );
	SAFE_DELETE_KTDGOBJECT( m_pMPChargeB );
	SAFE_DELETE_KTDGOBJECT( m_pMPChargeC );


	DeleteLireMajorParticle();
	DeleteLireMinorParticle();

	//���̳�
	g_pX2Game->GetMinorXMeshPlayer()->DestroyInstanceHandle( m_hMeshSpinningKick1 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqSpinningKick1 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqSpinningKick2 );
#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ���̺� ű ���� ���� �ҽ� ���� ( ĳ���� ��ũ��Ʈ �� ����Ʈ ������ �̰� )
	g_pX2Game->GetMinorXMeshPlayer()->DestroyInstanceHandle( m_hMeshDiveKickBombing );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����


#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.4] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
	SAFE_DELETE(m_pFriendshipNatureData);
#endif NEW_SKILL_2010_11

#ifdef	SERV_TRAPPING_RANGER_TEST
	DeleteTrapBlade();
	m_vecOptrEvoke.clear();
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	DeletePriorPlannedBlowEffect();												/// ���� �ϰ� ����Ʈ ����

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiring );	/// ������ ��ȣź ������ Ȯ�� ����Ʈ
#endif SERV_RENA_NIGHT_WATCHER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	m_vecAngerOfElfTrapUID.clear();
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���
	m_vecCryotronBoltDamageEffect.clear();
#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

#ifdef ADD_RENA_SYSTEM //��â��
	m_fNaturalForceValueCharge = 0.f;
	SetNaturalForceBuff(false);
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hNaturalForceEffect );
#endif //ADD_RENA_SYSTEM
}

void CX2GULire_ElvenRanger::ReInit( bool bRandom, int startPosIndex )
{
	CX2GUUser::ReInit( bRandom, startPosIndex );

	if( GetNowMp() < 50.f )
		SetNowMp( 50.f );

#ifdef SKILL_30_TEST
	MakeRoomForEntangleTrap( 0, true );
#endif SKILL_30_TEST

#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.4] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
	ReInitFriendshipNature();
#endif NEW_SKILL_2010_11

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	MakeRoomForAngerOfElfTrap( 0, true );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_RENA_SYSTEM //��â��
	/// �ڿ��� ��� ���� ����Ʈ �ʱ�ȭ
	m_fNaturalForceValueCharge = 0.f;
	/// �ڿ��� ��� ����ȭ
	if ( true == IsMyUnit() )
		m_FrameDataFuture.syncData.m_CannonBallCount = GetNaturalForceCount();

	if( GetNaturalForceCount() >= NATURAL_FORCE_BUFF )
		SetNaturalForceBuff( true );
	else
		SetNaturalForceBuff( false );

	SetNaturalForceEffect();

	//�⺻ ���ùڽ��� 1.3�谡 ��
	SetSphereAttackBoxScale( L"Lfoot", 1.3f );
	SetSphereAttackBoxScale( L"Rfoot", 1.3f );

	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;

	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	if( GetAccessNowDamageRelateSkillData().m_byteRelateData >= 10 )
		GetAccessNowDamageRelateSkillData().m_byteRelateData = 0;
#endif //ADD_RENA_SYSTEM
}

void CX2GULire_ElvenRanger::DamageReact( CX2DamageManager::DamageData* pDamageData )
{
	KTDXPROFILE();
	CX2GUUser::DamageReact( pDamageData );
	CX2GUUser::DamageReactStateChange( pDamageData );

#ifdef NEW_SKILL_2010_11
	// oasis907 : ����� [2010.11.4] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
	if( true == IsDamagedReaction( pDamageData ) && pDamageData->m_fHpPercentUp <= 0.f && pDamageData->m_fMpPercentUp <= 0.f )
	{
		if( m_delegateFriendshipOfNature )
			m_delegateFriendshipOfNature();
	}
#endif NEW_SKILL_2010_11

}

#ifdef NEW_SKILL_2010_11
//{{ oasis907 : ����� [2010.11.4] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
void CX2GULire_ElvenRanger::ProcessFriendshipNature()
{
	if(m_pFriendshipNatureData == NULL)
		return;

	if(m_pFriendshipNatureData->CanActivate() == false)
		return;

	if(GetRandomFloat() > m_pFriendshipNatureData->m_fInvokeRate)
		return;
	
	ReInitFriendshipNature();

	CX2Unit* pUnit = GetUnit();
	if ( NULL != pUnit )
	{
		const CX2UserSkillTree& cUserSkillTree = pUnit->GetUnitData().m_UserSkillTree;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		const int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LWS_FRIENDSHIP_OF_NATURE, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LWS_FRIENDSHIP_OF_NATURE );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( iSkillLevel > 0 )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LWS_FRIENDSHIP_OF_NATURE );
	#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LWS_FRIENDSHIP_OF_NATURE, iSkillLevel );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}


	m_pFriendshipNatureData->m_bCanActivate	= false;
	m_pFriendshipNatureData->m_fTimeElapsed = 0.f;
}

void CX2GULire_ElvenRanger::ReInitFriendshipNature()
{
	if(m_pFriendshipNatureData == NULL)
		return;

	m_pFriendshipNatureData->m_bCanActivate	= true;
	m_pFriendshipNatureData->m_fTimeElapsed = 0.f;
	return;
}
//}} oasis907 : ����� [2010.11.4] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
#endif NEW_SKILL_2010_11

void CX2GULire_ElvenRanger::Win()
{
	CX2GUUser::Win();

#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bShowTrapBladeToWinMotion )
		CreateTrapBlade();		
#endif SERV_RENA_NIGHT_WATCHER

#ifdef RIDING_SYSTEM
	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		StateChange( USI_RIDING_WAIT_HABIT );
	else
		StateChange( USI_WIN );
#else
	StateChange( USI_WIN );
#endif //RIDING_SYSTEM
}

void CX2GULire_ElvenRanger::Lose()
{
	CX2GUUser::Lose();

	if( IsMyUnit() == true && GetNowHp() > 0.0f )
	{
#ifdef RIDING_SYSTEM
		if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
			StateChange( USI_RIDING_WAIT );
		else
			StateChange( USI_LOSE );
#else
		StateChange( USI_LOSE );
#endif //RIDING_SYSTEM
	}
}

void CX2GULire_ElvenRanger::Draw()
{
	CX2GUUser::Draw();

#ifdef RIDING_SYSTEM
	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		StateChange( USI_RIDING_WAIT_HABIT );
	else
		StateChange( USI_WIN );
#else
	StateChange( USI_WIN );
#endif //RIDING_SYSTEM
}


#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GULire_ElvenRanger::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GULire_ElvenRanger::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GULire_ElvenRanger::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForBowToDeviceList( listInOut_, eDamageType_ );
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

void CX2GULire_ElvenRanger::InitEffect()
{
	CX2GUUser::InitEffect();
	
	CreateMajorParticleArray();
	CreateMinorParticleArray();
}





/*virtual*/ void CX2GULire_ElvenRanger::InitEnchantWeaponEffectUnitSpecific() 
{
	for( UINT i=0; i<m_vecpWeapon.size(); ++i )
	{
		Weapon* pWeapon = m_vecpWeapon[i];
		pWeapon->InitEnchantParticleForBow();
	}




	// ���� Ʈ���� �Ӽ� ���� ���ÿ� ȭ�� ����Ʈ. ������ ���Ⱑ �ϳ� ���� ������ �����Ѵ�.
	ASSERT( 1 == m_vecpWeapon.size() );
	CX2EnchantItem::ItemEnchantedAttribute enchantedAttribute;
	CX2Item* pItemNotFashion = GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
	if( NULL != pItemNotFashion )
	{
		enchantedAttribute = pItemNotFashion->GetItemData().m_EnchantedAttribute;
	}


#ifdef RENA_SECOND_CLASS_CHANGE


	CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( enchantedAttribute );
	switch( extraDamageType )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			m_iArrowMeshPlayerIndex = 1;
		} break;
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			m_iArrowMeshPlayerIndex = 2;
		} break;
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			m_iArrowMeshPlayerIndex = 3;
		} break;
	default:
		{
			m_iArrowMeshPlayerIndex = 0;
		} break;
	}

#endif RENA_SECOND_CLASS_CHANGE


}


void CX2GULire_ElvenRanger::DeleteLireMajorParticle()
{
	CX2GUUser::DeleteGUUserMajorParticle();

	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();

	for ( int index = 0; index < RENA_MAJOR_PII_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahRenaMajorParticleInstance[index] );
	}
}

void CX2GULire_ElvenRanger::DeleteLireMinorParticle()
{
	CX2GUUser::DeleteGUUserMinorParticle();

	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMinorParticle();

	for ( int index = 0; index < RENA_MINOR_PII_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahRenaMinorParticleInstance[index] );
	}
}

void CX2GULire_ElvenRanger::InitializeLireMajorParticleArray()
{
	CX2GUUser::InitializeGUUserMajorParticleArray();

	for ( int index = 0; index < RENA_MAJOR_PII_END; index++ )
	{
		m_ahRenaMajorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GULire_ElvenRanger::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMajorParticle() == NULL )
		return;

	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpact" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactCore" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactSlash" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactRingRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactCoreRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactSlashRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Leaf_Lire_Nature_Force_Impact01" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"PhoenixStartWave" );
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

/*virtual*/ void CX2GULire_ElvenRanger::CreateMajorParticleArray()
{
	CX2GUUser::CreateMajorParticleArray();

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	// small damage Ÿ���� ���� �� ������ ��ƼŬ
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT,				L"FireDamageImpact",				2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE,			L"FireDamageImpactCore",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH,			L"FireDamageImpactSlash",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	// AttackResult�� switch case���� default:�� ó���Ǵ� Ÿ�ݿ� ���� ��ƼŬ (����� small �̿��� ��ƼŬ)
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,			L"FireDamageImpactRed",				2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED,		L"FireDamageImpactRingRed",			1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED,		L"FireDamageImpactCoreRed",			1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED,		L"FireDamageImpactSlashRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	// ������ ���� ��ƼŬ
	pSeq = SetRenaMajorParticleByEnum( RENA_MAJOR_PII_WIND_SNEAKER_IMPACT_LEAF,		L"Leaf_Lire_Nature_Force_Impact01", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetRenaMajorParticleByEnum( RENA_MAJOR_PII_PHOENIX_START_WAVE,				L"PhoenixStartWave",				-1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
}

CKTDGParticleSystem::CParticleEventSequence* CX2GULire_ElvenRanger::SetRenaMajorParticleByEnum( RENA_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ /*= -1 */ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleRenaMajorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), 0,0,0, 0, 0, iDrawCount_, 0 );

		SetHandleRenaMajorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleRenaMajorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GULire_ElvenRanger::InitializeLireMinorParticleArray()
{
	CX2GUUser::InitializeGUUserMinorParticleArray();

	for ( int index = 0; index < RENA_MINOR_PII_END; index++ )
	{
		m_ahRenaMinorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GULire_ElvenRanger::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMinorParticle() == NULL )
		return;

	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"ImpactTickBlue" );
}

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


/*virtual*/ void CX2GULire_ElvenRanger::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	pSeq = SetRenaMinorParticleByEnum( RENA_MINOR_PII_IMPACT_TICK_BLUE, L"ImpactTickBlue", 2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
}

CKTDGParticleSystem::CParticleEventSequence* CX2GULire_ElvenRanger::SetRenaMinorParticleByEnum( RENA_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ /*= -1 */ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleRenaMinorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), 0,0,0, 0, 0, iDrawCount_, 0 );

		SetHandleRenaMinorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleRenaMinorParticleByEnum( eVal_) );

	return pSeq;
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GULire_ElvenRanger::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMinorParticle() != NULL )
	{
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"MPEnergy" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"MPEnergyCenter" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"MPChargeWave" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"ImpactTickFeather" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"MPCharge2Up" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"MPCharge2UpHigh" );
	}
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GULire_ElvenRanger::InitComponent()
{
	CX2GUUser::InitComponent();

	if ( IsMyUnit() )
	{
		ASSERT( NULL != g_pX2Game );
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				m_pGageData->SetNowMp( 50.f );
#ifdef ADD_RENA_SYSTEM //��â��
				SetNaturalForceCount(0);
#endif //ADD_RENA_SYSTEM
			} break;
		default:
			break;
		}
	}
	m_hSeqMPEnergy			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPEnergy", 0,0,0, 0,0 );
	m_hSeqMPEnergyCenter	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPEnergyCenter", 0,0,0, 0,0 );

	CKTDGParticleSystem::CParticleEventSequence* pSeqEnergy = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
	if( pSeqEnergy != NULL )
	{
		pSeqEnergy->UseLookPoint( true );
	}

	m_hSeqMPChargeWave			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPChargeWave", 0,0,0, 0,0 );
	m_hSeqMPChargeFeather		= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"ImpactTickFeather", 0,0,0, 0,0 );

	m_hSeqMPCharge2Up			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPCharge2Up", 0,0,0, 0,0 );
	m_hSeqMPCharge2UpHigh		= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPCharge2UpHigh", 0,0,0, 0,0 );




	if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
	{
#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
		AddUnitSlashData( CX2UnitSlashTraceManager::STC_NONE );
		AddUnitSlashData( CX2UnitSlashTraceManager::STC_RENA_NATURE_FORCE );
#endif UNIT_SLASH_TRACE_MANAGER_TEST

		m_LuaManager.EndTable();
	}

}

void CX2GULire_ElvenRanger::ParseCommonRandomState()
{
	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{

		const char* tableName = "";
		switch( GetUnit()->GetClass() )
		{
		case CX2Unit::UC_LIRE_ELVEN_RANGER:
			{
				tableName = "LIRE_ELVEN_RANGER";
			} break;

		case CX2Unit::UC_LIRE_COMBAT_RANGER:
			{
				tableName = "LIRE_COMBAT_RANGER";
			} break;


		case CX2Unit::UC_LIRE_SNIPING_RANGER:
			{
				tableName = "LIRE_SNIPING_RANGER";
			} break;

		case CX2Unit::UC_LIRE_WIND_SNEAKER:
			{
				tableName = "LIRE_WIND_SNEAKER";
			} break;


		case CX2Unit::UC_LIRE_GRAND_ARCHER:
			{
				tableName = "LIRE_GRAND_ARCHER";
			} break;

#ifdef	SERV_TRAPPING_RANGER_TEST
		case CX2Unit::UC_LIRE_TRAPPING_RANGER:
			{
				tableName = "LIRE_TRAPPING_RANGER";
			} break;
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef	SERV_RENA_NIGHT_WATCHER
		case CX2Unit::UC_LIRE_NIGHT_WATCHER:
			{
				tableName = "LIRE_NIGHT_WATCHER";
			} break;
#endif	SERV_RENA_NIGHT_WATCHER

		default:
			{
				ASSERT( !"NO" );
			} break;
		}

		CX2GUUser::ParseCommonRandomState(tableName);

		m_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE
	}
}




void CX2GULire_ElvenRanger::InitStateID()
{
	m_CommonState.m_Wait			= USI_WAIT;
	m_ChargeMpState			= LESI_CHARGE_MP;

//#ifdef PVP_BOSS_COMBAT_TEST
//
//	m_FrozenState = LESI_FROZEN;
//
//#endif PVP_BOSS_COMBAT_TEST

//#ifdef TRANSFORMER_TEST
//	m_CommonState.m_Transformed			= LESI_TRANSFORMED;
//#endif TRANSFORMER_TEST




}






void CX2GULire_ElvenRanger::InitStateCommon()
{
	UserUnitStateData stateData;




	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "LESI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "LESI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DieBackEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "LESI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "LESI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, WaitStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, WaitFrameMoveFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, WaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "LESI_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser,			  WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, WalkEventProcess );
#ifdef ADD_RENA_SYSTEM //��â��
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, WalkStart );
#endif //ADD_RENA_SYSTEM
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "LESI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "LESI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LESI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LESI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;


#ifdef SERV_RENA_NIGHT_WATCHER
	if( GetUnit()->GetClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER )		//�׼� ��� ������ �߰��Ǿ��ִ� DashState
	{
		stateData.Init();
		stateData.stateID			= USI_DASH;
		m_LuaManager.MakeTableReference( "LESI_DASH", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_StateStartFuture );
#ifdef ADD_RENA_SYSTEM //��â��
		stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashStart );
#else //ADD_RENA_SYSTEM
		stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
#endif //ADD_RENA_SYSTEM
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_FrameMoveFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEventProcess );
		m_StateList[stateData.stateID] = stateData;
	}
	else																//�Ϲ� DashState
	{
		stateData.Init();
		stateData.stateID			= USI_DASH;
		m_LuaManager.MakeTableReference( "LESI_DASH", stateData.stateID );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
#ifdef ADD_RENA_SYSTEM //��â��
		stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashStart );
#else //ADD_RENA_SYSTEM
		stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
#endif //ADD_RENA_SYSTEM
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEventProcess );
		m_StateList[stateData.stateID] = stateData;
	}
#else  SERV_RENA_NIGHT_WATCHER
	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "LESI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
#ifdef ADD_RENA_SYSTEM //��â��
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashStart );
#else //ADD_RENA_SYSTEM
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
#endif //ADD_RENA_SYSTEM
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif SERV_RENA_NIGHT_WATCHER

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "LESI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEndStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEndFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "LESI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_DOUBLE_JUMP;
	m_LuaManager.MakeTableReference( "LESI_DOUBLE_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_DASH_DOUBLE_JUMP;
	m_LuaManager.MakeTableReference( "LESI_DASH_DOUBLE_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashDoubleJumpStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashDoubleJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashDoubleJumpEventProcess );
	m_StateList[stateData.stateID] = stateData;






	stateData.Init();
	stateData.stateID			= LESI_CHARGE_MP;
	m_LuaManager.MakeTableReference( "LESI_CHARGE_MP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ChargeMPStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, ChargeMPStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ChargeMPFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ChargeMPEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ChargeMPEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, ChargeMPEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "LESI_HYPER_MODE", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, HyperModeInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser,			  HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser,			  HyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser,			  HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser,			  HyperModeEnd );
#endif HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageSmallFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageSmallBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageBigFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageBigFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageBigBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageBigBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageAirDownLandingInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "LESI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "LESI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontFrontEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontFrontEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "LESI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontBackEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontBackEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "LESI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackFrontEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackFrontEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "LESI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackBackEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackBackEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "LESI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpAttackFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "LESI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpAttackBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpAttackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpAttackBackEnd );
	m_StateList[stateData.stateID] = stateData;
	
	stateData.Init();
	stateData.stateID			= LESI_SIEGE_WAIT;
	m_LuaManager.MakeTableReference( "LESI_SIEGE_WAIT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeWaitStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeWaitFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_SIEGE_ATTACK;
	m_LuaManager.MakeTableReference( "LESI_SIEGE_ATTACK", stateData.stateID );	
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger,  SiegeAttackFrontStateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackFrontStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_SIEGE_ATTACK_X;
	m_LuaManager.MakeTableReference( "LESI_SIEGE_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackXFrontStateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackXFrontStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackXFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef BALANCE_PATCH_20120329	
	stateData.Init();
	stateData.stateID			= LESI_SIEGE_END;
	m_LuaManager.MakeTableReference( "LESI_SIEGE_END", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif



	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "LESI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "LESI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "LESI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "LESI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LESI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef SPECIAL_USE_ITEM
	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "LESI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;
#endif SPECIAL_USE_ITEM

#ifdef RIDING_SYSTEM
	#pragma region LESI_RIDING_START
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "LESI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_ON
	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "LESI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_OFF
	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "LESI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_WAIT_HABIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "LESI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_WAIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "LESI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_WALK
	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "LESI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
	
	#pragma region LESI_RIDING_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "LESI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "LESI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LESI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DASH
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DASH_END
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DASH_JUMP
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DASH_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DAMAGE_FRONT
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DAMAGE_BACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_DIE
	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "LESI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "LESI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_JUMP_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "LESI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_ATTACK_X
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "LESI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_SPECIAL_ATTACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "LESI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region LESI_RIDING_SPECIAL_MOVE
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "LESI_RIDING_SPECIAL_MOVE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEndFutrue );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
#endif //RIDING_SYSTEM

#ifdef MODIFY_RIDING_PET_AWAKE
	stateData.Init();
	stateData.stateID			= USI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "LESI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, RidingHyperModeInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser,			  RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser,			  RidingHyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser,			  RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser,			  RidingHyperModeEnd );
#endif // HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;
#endif // MODIFY_RIDING_PET_AWAKE

#ifdef WALL_JUMP_TEST
	stateData.Init();
	stateData.stateID			= LESI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "LESI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;
#endif WALL_JUMP_TEST




//#ifdef PVP_BOSS_COMBAT_TEST
//
//	stateData.Init();
//	stateData.stateID			= LESI_FROZEN;
//	m_LuaManager.MakeTableReference( "LESI_FROZEN", stateData.stateID );
//	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, Frozen_StateStart ); 
//	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, Frozen_EventProcess );
//	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, Frozen_StateEnd ); 
//	m_StateList[stateData.stateID] = stateData;
//
//
//#endif PVP_BOSS_COMBAT_TEST



//#ifdef TRANSFORMER_TEST
//
//	stateData.Init();
//	stateData.stateID			= LESI_TRANSFORMED;
//	m_LuaManager.MakeTableReference( "LESI_TRANSFORMED", stateData.stateID );
//	m_StateList[stateData.stateID] = stateData;
//
//
//#endif TRANSFORMER_TEST






}


void CX2GULire_ElvenRanger::InitState()
{

	ASSERT( NULL != GetUnit() );

	const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_COMBAT_RANGER:
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
		{
			m_iSkillCutInSetIndex = 0;
		} break;
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			m_iSkillCutInSetIndex = 1;
		} break;
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			m_iSkillCutInSetIndex = 2;
		} break;
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			m_iSkillCutInSetIndex = 3;
		} break;
#endif SERV_RENA_NIGHT_WATCHER
	}

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_LIRE_COMBAT_RANGER:
		{
			m_iSkillCutInSetSubIndex = 1;
		} break;
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
		{
			m_iSkillCutInSetSubIndex = 2;
		} break;
	default:
		{
			m_iSkillCutInSetSubIndex = 0;
		}
		break;
	}



	InitStateID();


	InitStateCommon();


	// �������� ���� ������ ���� start, win, lose ���� �ʱ�ȭ
	std::string tableNameStartUTF8;
	std::string tableNameWinUTF8;
	std::string tableNameLoseUTF8;
	InitStateCommonRandom( tableNameStartUTF8, tableNameWinUTF8, tableNameLoseUTF8 );




	UserUnitStateData stateData;


	stateData.Init();
	stateData.stateID			= USI_START;
	m_LuaManager.MakeTableReference( tableNameStartUTF8.c_str(), stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_WIN;
	m_LuaManager.MakeTableReference( tableNameWinUTF8.c_str(), stateData.stateID );
#ifdef SERV_PET_SYSTEM
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, WinStateStart );	
#endif
	m_StateList[stateData.stateID] = stateData;

#ifdef SERV_RENA_NIGHT_WATCHER
	if( "LESI_RNW_WIN2" == tableNameWinUTF8 )
		m_bShowTrapBladeToWinMotion = true;
#endif SERV_RENA_NIGHT_WATCHER


	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( tableNameLoseUTF8.c_str(), stateData.stateID );
#ifdef SERV_PET_SYSTEM
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );	
#endif
	m_StateList[stateData.stateID] = stateData;







	//////////////////////////////////////////////////////////////////////////
	switch( GetUnitClass() )
	{
	default:
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{
			stateData.Init();
			stateData.stateID			= LESI_DOUBLE_JUMP_ATTACK_Z;
			m_LuaManager.MakeTableReference( "LESI_DOUBLE_JUMP_ATTACK_Z", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpAttackZStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpAttackZFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpAttackZEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;


	case CX2Unit::UC_LIRE_COMBAT_RANGER:
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DOUBLE_JUMP_ATTACK_Z;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP_EventProcess );
			m_StateList[stateData.stateID] = stateData;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			//-----����� ���
			stateData.Init();
			stateData.stateID			= LESI_P_LE_AGILE_MOVEMENT_FRONT_FRONT;
			m_LuaManager.MakeTableReference( "LESI_P_LE_AGILE_MOVEMENT_FRONT_FRONT", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontFrontEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontFrontEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_P_LE_AGILE_MOVEMENT_FRONT_BACK;
			m_LuaManager.MakeTableReference( "LESI_P_LE_AGILE_MOVEMENT_FRONT_BACK", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontBackEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingFrontBackEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_P_LE_AGILE_MOVEMENT_BACK_FRONT;
			m_LuaManager.MakeTableReference( "LESI_P_LE_AGILE_MOVEMENT_BACK_FRONT", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackFrontEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackFrontEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_P_LE_AGILE_MOVEMENT_BACK_BACK;
			stateData.stateID			= LESI_P_LE_AGILE_MOVEMENT_BACK_BACK;
			m_LuaManager.MakeTableReference( "LESI_P_LE_AGILE_MOVEMENT_BACK_BACK", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackBackEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, StandUpRollingBackBackEnd );
			m_StateList[stateData.stateID] = stateData;
			//����� ���-----
#endif //UPGRADE_SKILL_SYSTEM_2013

		} break;
	}




	stateData.Init();
	stateData.stateID			= LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X", stateData.stateID );
#ifdef ADD_RENA_SYSTEM //��â��
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_StateStart );
#endif //ADD_RENA_SYSTEM
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef ADD_RENA_SYSTEM //��â��
	stateData.Init();
	stateData.stateID			= LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX;
	m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif //ADD_RENA_SYSTEM

	stateData.Init();
	stateData.stateID			= LESI_DOUBLE_JUMP_ATTACK_Z_LANDING;
	m_LuaManager.MakeTableReference( "LESI_DOUBLE_JUMP_ATTACK_Z_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpAttackZLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpAttackZLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DoubleJumpAttackZLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_COMBO_Z1;
	m_LuaManager.MakeTableReference( "LESI_COMBO_Z1", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ1FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ1EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_COMBO_Z2;
	m_LuaManager.MakeTableReference( "LESI_COMBO_Z2", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ2FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ2EventProcess );
	m_StateList[stateData.stateID] = stateData;

	switch( GetUnit()->GetClass() )
	{
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	default:
		{
			stateData.Init();
			stateData.stateID			= LESI_COMBO_Z3;
			m_LuaManager.MakeTableReference( "LESI_COMBO_Z3", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ3FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ3FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ3EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBO_Z4;
			m_LuaManager.MakeTableReference( "LESI_COMBO_Z4", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{
			stateData.Init();
			stateData.stateID			= LESI_COMBO_RTR_ZZZ;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RTR_ZZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBO_RTR_ZZZ_DZ;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RTR_ZZZ_DZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_DZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_DZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_DZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZ_DZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBO_RTR_ZZZZ;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RTR_ZZZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBO_RTR_ZZZZZ;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RTR_ZZZZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RTR_ZZZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif	SERV_TRAPPING_RANGER_TEST
	}


	stateData.Init();
	stateData.stateID			= LESI_COMBO_Z4a;
	m_LuaManager.MakeTableReference( "LESI_COMBO_Z4a", stateData.stateID );
	//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4aFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4aFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4aEventProcess );
	//stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4aEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_COMBO_Z4UP;
	m_LuaManager.MakeTableReference( "LESI_COMBO_Z4UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4UpFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4UpFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4UpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_COMBO_X1;
	m_LuaManager.MakeTableReference( "LESI_COMBO_X1", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX1FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX1EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef BALANCE_PATCH_20120329
	if( GetUnit()->GetClass() == CX2Unit::UC_LIRE_SNIPING_RANGER || GetUnit()->GetClass() == CX2Unit::UC_LIRE_GRAND_ARCHER )
	{
		stateData.Init();
		stateData.stateID			= LESI_COMBO_X2;
		m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_COMBO_X2", stateData.stateID );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX2FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX2EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LESI_COMBO_X3;
		m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_COMBO_X3", stateData.stateID );
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX3FrameMoveFuture );		
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX3FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX3EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LESI_COMBO_X4;
		m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_COMBO_X4", stateData.stateID );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX4FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SnipingRangerComboX4EventProcess );
		m_StateList[stateData.stateID] = stateData;
	}
	else
#endif
	{
		stateData.Init();
		stateData.stateID			= LESI_COMBO_X2;
		m_LuaManager.MakeTableReference( "LESI_COMBO_X2", stateData.stateID );
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX2FrameMoveFuture );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX2FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX2EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LESI_COMBO_X3;
		m_LuaManager.MakeTableReference( "LESI_COMBO_X3", stateData.stateID );
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX3FrameMoveFuture );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX3FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX3EventProcess );
		m_StateList[stateData.stateID] = stateData;

		stateData.Init();
		stateData.stateID			= LESI_COMBO_X4;
		m_LuaManager.MakeTableReference( "LESI_COMBO_X4", stateData.stateID );
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4FrameMoveFuture );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4EventProcess );
		m_StateList[stateData.stateID] = stateData;
	}
	

	stateData.Init();
	stateData.stateID			= LESI_COMBO_X4UP;
	m_LuaManager.MakeTableReference( "LESI_COMBO_X4UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4UpFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4UpFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4UpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_COMBO_X4DOWN;
	m_LuaManager.MakeTableReference( "LESI_COMBO_X4DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4DownFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4DownFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboX4DownEventProcess );
	m_StateList[stateData.stateID] = stateData;





	switch( GetUnit()->GetClass() )
	{
	default:
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_COMBO_Z1;
			m_LuaManager.MakeTableReference( "LESI_DASH_COMBO_Z1", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ1StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ1FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ1EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;


	case CX2Unit::UC_LIRE_COMBAT_RANGER:
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_COMBO_Z1;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_DASH_COMBO_Z1", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_COMBO_Z1_StartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_COMBO_Z1_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBAT_RANGER_DASH_COMBO_Z2;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_DASH_COMBO_Z2", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_COMBO_Z2_StartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_COMBO_Z2_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_COMBO_Z1;
			m_LuaManager.MakeTableReference( "LESI_DASH_COMBO_Z1", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ1StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ1FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ1EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_DASH_COMBO_RTR_ZX;
			m_LuaManager.MakeTableReference( "LESI_DASH_COMBO_RTR_ZX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_COMBO_RTR_ZX_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_COMBO_RTR_ZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_COMBO_RTR_ZX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_COMBO_RTR_ZX_StateEnd );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_COMBO_RTR_ZX_StateEndFuture );
			m_StateList[stateData.stateID] = stateData;

		} break;
#endif	SERV_TRAPPING_RANGER_TEST
	}




	stateData.Init();
	stateData.stateID			= LESI_DASH_COMBO_Z2;
	m_LuaManager.MakeTableReference( "LESI_DASH_COMBO_Z2", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ2StartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashComboZ2EventProcess );
	m_StateList[stateData.stateID] = stateData;




	switch( GetUnitClass() )
	{
	default:
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_COMBAT_RANGER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{

			stateData.Init();
			stateData.stateID			= LESI_DASH_ATTACK_X;
			m_LuaManager.MakeTableReference( "LESI_DASH_ATTACK_X", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashAttackXStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashAttackXFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashAttackXEventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashAttackXEndFuture );
			m_StateList[stateData.stateID] = stateData;

		} break;


	case CX2Unit::UC_LIRE_SNIPING_RANGER:
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{

			stateData.Init();
			stateData.stateID			= LESI_DASH_ATTACK_X;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_COMBO_X1", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_EndFuture );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_SNIPING_RANGER_DASH_COMBO_X2;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_COMBO_X2", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

#ifndef BALANCE_GRAND_ARCHER_20121213
			stateData.Init();
			stateData.stateID			= LESI_SNIPING_RANGER_DASH_COMBO_X3;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_COMBO_X3", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X3_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X3_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_SNIPING_RANGER_DASH_COMBO_X3_UP;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_COMBO_X3_UP", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X3_UP_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X3_UP_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN_EventProcess );
			m_StateList[stateData.stateID] = stateData;
#endif //BALANCE_GRAND_ARCHER_20121213

		} break;

	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_ATTACK_X;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_COMBO_X1", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_DASH_X_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_COMBO_X1_EndFuture );
			m_StateList[stateData.stateID] = stateData;




			stateData.Init();
			stateData.stateID			= LESI_LWS_DASH_XZ;
			m_LuaManager.MakeTableReference( "LESI_LWS_DASH_XZ", stateData.stateID );
			//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_DASH_XZ_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_DASH_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_LWS_DASH_XZZ;
			m_LuaManager.MakeTableReference( "LESI_LWS_DASH_XZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_DASH_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
		
	}







	switch( GetUnit()->GetClass() )
	{
	default:
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
		{
			stateData.Init();
			stateData.stateID			= LESI_JUMP_ATTACK_Z;
			m_LuaManager.MakeTableReference( "LESI_JUMP_ATTACK_Z", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackZFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackZFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackZEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{
			stateData.Init();
			stateData.stateID			= LESI_JUMP_ATTACK_Z;
			m_LuaManager.MakeTableReference( "LESI_JUMP_ATTACK_RTR_Z", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_JUMP_ATTACK_RTR_Z_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_JUMP_ATTACK_RTR_Z_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_JUMP_ATTACK_RTR_Z_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_JUMP_ATTACK_RTR_Z_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_JUMP_ATTACK_RTR_Z_StateEnd );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif	SERV_TRAPPING_RANGER_TEST
	}

	stateData.Init();
	stateData.stateID			= LESI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "LESI_JUMP_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackXStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackXFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, JumpAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;








	switch( GetUnitClass() )
	{
	default:
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_ATTACK_Z;
			m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_ATTACK_Z", stateData.stateID );
			//stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZStartFuture );
			//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZFrameMoveFuture );
			//stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_LIRE_COMBAT_RANGER:
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_ATTACK_Z;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;


	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			stateData.Init();
			stateData.stateID			= LESI_LGA_DASH_JUMP_Z;
			m_LuaManager.MakeTableReference( "LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_DASH_JUMP_Z_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_LGA_DASH_JUMP_ZX;
			m_LuaManager.MakeTableReference( "LESI_LGA_DASH_JUMP_ZX", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_DASH_JUMP_ZX_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_DASH_JUMP_ZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_DASH_JUMP_ZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;

#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_ATTACK_RTR_Z;
			m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_ATTACK_RTR_Z", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_Z_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_Z_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_Z_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_Z_StateEnd );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_ATTACK_RTR_ZZ;
			m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_ATTACK_RTR_ZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_ZZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_ZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_ZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_DASH_JUMP_ATTACK_RTR_ZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_SA_RTR_FATALITY_SUCCESS;
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_FATALITY_SUCCESS", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_SUCCESS_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_SA_RTR_FATALITY_FAIL;
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_FATALITY_FAIL", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_FAIL_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif	SERV_TRAPPING_RANGER_TEST

	}





	stateData.Init();
	stateData.stateID			= LESI_DASH_JUMP_ATTACK_Z_LANDING;
	m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_ATTACK_Z_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpAttackZLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= LESI_DASH_JUMP_COMBO_X1;
	m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_COMBO_X1", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX1FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX1EventProcess );
	m_StateList[stateData.stateID] = stateData;






	switch( GetUnitClass() )
	{
	default:
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_COMBAT_RANGER:
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_COMBO_X2;
			m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_COMBO_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX2FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX2EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_COMBO_X3;
			m_LuaManager.MakeTableReference( "LESI_DASH_JUMP_COMBO_X3", stateData.stateID );
#ifdef ADD_RENA_SYSTEM // ����ȯ
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX3StateStart );
#endif // ADD_RENA_SYSTEM
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX3FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX3FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX3EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_LIRE_SNIPING_RANGER:		
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			stateData.Init();
			stateData.stateID			= LESI_DASH_JUMP_COMBO_X2;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3;
			m_LuaManager.MakeTableReference( "LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3", stateData.stateID );
#ifdef ADD_RENA_SYSTEM // ����ȯ
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, DashJumpComboX3StateStart );
#endif // ADD_RENA_SYSTEM
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
	}











#ifdef RENA_SECOND_CLASS_CHANGE
	
	switch( GetUnitClass() )
	{
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			stateData.Init();
			stateData.stateID			= LESI_LWS_ZZfrontZ;
			m_LuaManager.MakeTableReference( "LESI_LWS_ZZfrontZ", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZ_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;



			stateData.Init();
			stateData.stateID			= LESI_LWS_ZZfrontZZ;
			m_LuaManager.MakeTableReference( "LESI_LWS_ZZfrontZZ", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZZ_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_LWS_ZZfrontZZZ;
			m_LuaManager.MakeTableReference( "LESI_LWS_ZZfrontZZZ", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZZZ_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_LWS_ZZfrontZZZ_Landing;
			m_LuaManager.MakeTableReference( "LESI_LWS_ZZfrontZZZ_Landing", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LWS_ZZfrontZZZ_Landing_EventProcess );
			m_StateList[stateData.stateID] = stateData;






		} break;

	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
#ifdef BALANCE_GRAND_ARCHER_20121213
			stateData.Init();
			stateData.stateID			= LESI_LGA_ZZX;
			m_LuaManager.MakeTableReference( "LESI_LGA_ZZX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_LGA_ZZXX;
			m_LuaManager.MakeTableReference( "LESI_LGA_ZZXX", stateData.stateID );
#ifdef ADD_RENA_SYSTEM //��â��
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZXX_StateStart );
#endif //ADD_RENA_SYSTEM
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
#else
			stateData.Init();
			stateData.stateID			= LESI_LGA_ZZZX;
			m_LuaManager.MakeTableReference( "LESI_LGA_ZZZX", stateData.stateID );
			//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4aFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZZX_EventProcess );
			//stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, ComboZ4aEndFuture );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= LESI_LGA_ZZZXX;
			m_LuaManager.MakeTableReference( "LESI_LGA_ZZZXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZZXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_LGA_ZZZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
#endif //BALANCE_GRAND_ARCHER_20121213

		} break;

#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			//���� 2�� ���� ����Ʈ ���� - ZX �޺�
			stateData.Init();
			stateData.stateID			= LESI_COMBO_RNW_ZZX;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RNW_ZZX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZX_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZX_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - ZXX �޺�
			stateData.Init();
			stateData.stateID			= LESI_COMBO_RNW_ZZXX;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RNW_ZZXX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXX_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXX_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - ZXXX �޺�
			stateData.Init();
			stateData.stateID			= LESI_COMBO_RNW_ZZXXX;
			m_LuaManager.MakeTableReference( "LESI_COMBO_RNW_ZZXXX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXXX_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXXX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_COMBO_RNW_ZZXXX_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - �׼� ���
			stateData.Init();
			stateData.stateID			= LESI_RNW_ACCEL_DASH;
			m_LuaManager.MakeTableReference( "LESI_RNW_ACCEL_DASH", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_StateStartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_StateStart );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - �׼� ��� ����
			stateData.Init();
			stateData.stateID			= LESI_RNW_ACCEL_DASH_JUMP;
			m_LuaManager.MakeTableReference( "LESI_RNW_ACCEL_DASH_JUMP", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_StateStartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_StateStart );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_FrameMoveFuture );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_StateEndFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - �׼� ��� ���� ����
			stateData.Init();
			stateData.stateID			= LESI_RNW_ACCEL_DASH_JUMP_LANDING;
			m_LuaManager.MakeTableReference( "LESI_RNW_ACCEL_DASH_JUMP_LANDING", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_LANDING_StateStartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_LANDING_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_LANDING_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - �׼� ��� ���� Z			
			stateData.Init();
			stateData.stateID			= LESI_RNW_ACCEL_DASH_JUMP_Z;
			m_LuaManager.MakeTableReference( "LESI_RNW_ACCEL_DASH_JUMP_Z", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_StateStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_StateEndFuture );
			m_StateList[stateData.stateID] = stateData;

			//���� 2�� ���� ����Ʈ ���� - �׼� ��� ���� ZZ
			stateData.Init();
			stateData.stateID			= LESI_RNW_ACCEL_DASH_JUMP_ZZ;
			m_LuaManager.MakeTableReference( "LESI_RNW_ACCEL_DASH_JUMP_ZZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_StateStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_FrameMove );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_StateEndFuture );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif SERV_RENA_NIGHT_WATCHER

	}

#endif RENA_SECOND_CLASS_CHANGE

#ifdef	SERV_TRAPPING_RANGER_TEST
	stateData.Init();
	stateData.stateID			= LESI_SI_A_LE_RISING_FALCON_LANDING;
	m_LuaManager.MakeTableReference( "LESI_A_LE_RISING_FALCON_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_RISING_FALCON_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif	SERV_TRAPPING_RANGER_TEST










	InitPassiveSkillState();
	InitEquippedActiveSkillState();







	CX2GUUser::InitState();
#ifdef	SERV_TRAPPING_RANGER_TEST
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	StateInitTrapBlade();
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
#endif	SERV_TRAPPING_RANGER_TEST


#ifdef ADD_RENA_SYSTEM //��â��
	if( GetNaturalForceCount() >= NATURAL_FORCE_BUFF )
		SetNaturalForceBuff( true );
#endif //ADD_RENA_SYSTEM
}



void CX2GULire_ElvenRanger::InitEquippedActiveSkillState(bool bOnlySkillLevel /* = false  */)
{
	CX2GUUser::InitEquippedActiveSkillState(bOnlySkillLevel);
}


/*virtual*/ void CX2GULire_ElvenRanger::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();

	m_fArrowRangeRate		= 1.f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	m_fSiegeDamageByPassive	= 1.f;

	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LSR_HAWK_EYE, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LSR_HAWK_EYE );

		if( NULL != pSkillTemplet )
		{
			/// ȭ�� ��Ÿ� ����
			m_fArrowRangeRate		*= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillLevel );
			/// ���� ��� ������ ����
			m_fSiegeDamageByPassive	=  pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );

			/// ���� ��� ������ ���� ó�� ( ���� ��� ������ 0�̸�, 1���� �������� ���� )
			int iSiegeSkillLevel = max( GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LE_SIEGEMODE, true ), 1 );

			SetSiegeModeEffectiveValue( iSiegeSkillLevel );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_HAWK_EYE );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_HAWK_EYE, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_fArrowRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef BALANCE_PATCH_20110303
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LWS_POWERFUL_SHOT, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LWS_POWERFUL_SHOT );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LWS_POWERFUL_SHOT );
		if( NULL != pSkillTemplet )
		{
			m_fArrowRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillLevel );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LWS_POWERFUL_SHOT, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			m_fArrowRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_SHARPEN_ARROW, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_SHARPEN_ARROW );

		if( NULL != pSkillTemplet )
		{
			m_fArrowRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillLevel );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_SHARPEN_ARROW );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_SHARPEN_ARROW, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_fArrowRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif	SERV_TRAPPING_RANGER_TEST


	
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	m_fMPConsumeRateSiegeArrow	= 1.f;
	m_fDamageRelSiegeArrow		= 1.f;

	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_SIEGEMODE_UP );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_SIEGEMODE_UP, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			m_fMPConsumeRateSiegeArrow *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );
			m_fDamageRelSiegeArrow		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
	
	m_fAttackPowerRateArrow = 1.f;
	m_iAddHitCountArrow = 0;
	m_fHitGapArrow = -1.f;
	m_fHitAddMp = -1.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LSR_POWERFUL_BOWSTRING, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LSR_POWERFUL_BOWSTRING );

		if( NULL != pSkillTemplet )
		{
			m_fAttackPowerRateArrow *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );

	#ifndef BALANCE_CODE_NEMESIS_20121213
			m_iAddHitCountArrow = (int) pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING );
			const float MAGIC_ARROW_HIT_GAP = 2.f;
			m_fHitGapArrow = MAGIC_ARROW_HIT_GAP;
	#endif //BALANCE_CODE_NEMESIS_20121213
	#ifdef SKILL_BALANCE_PATCH
			if(m_iAddHitCountArrow > 0)
				m_fHitAddMp = 3.f;
	#endif
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_POWERFUL_BOWSTRING );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_POWERFUL_BOWSTRING, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_fAttackPowerRateArrow *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );

	#ifndef BALANCE_CODE_NEMESIS_20121213
			m_iAddHitCountArrow = (int) pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING );
			const float MAGIC_ARROW_HIT_GAP = 2.f;
			m_fHitGapArrow = MAGIC_ARROW_HIT_GAP;
	#endif //BALANCE_CODE_NEMESIS_20121213
	#ifdef SKILL_BALANCE_PATCH
			if(m_iAddHitCountArrow > 0)
				m_fHitAddMp = 3.f;
	#endif
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
	
	// kimhc // 2009-11-27 // �нú� ��ų �ʱ�ȭ �ÿ� �׵������ �ʱ�ȭ
	m_SkillRelatedData.SetForceDownValueRateOfRenaArrowAttack( 1.f );
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LSR_SHARP_ARROW, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LSR_SHARP_ARROW );
		if( NULL != pSkillTemplet )
		{
			m_SkillRelatedData.SetForceDownValueRateOfRenaArrowAttack( m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack() * 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_RANGE_REL, iSkillLevel ) );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_SHARP_ARROW );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_SHARP_ARROW, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			m_SkillRelatedData.SetForceDownValueRateOfRenaArrowAttack( m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack() * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_RANGE_REL ) );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef RENA_SECOND_CLASS_CHANGE

	const CX2SkillTree::SkillTemplet* pMySkillTemplet = NULL;
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_LWS_POWERFUL_SHOT );
	if( NULL != pMySkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if ( NULL == GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pMySkillTemplet->m_eID, true ) );	/// ��ų ����
	
		m_ArrowData.m_fAddBackSpeedX = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_BACK_SPEED_X, iSkillTempletLevel );
		m_fAttackPowerRateArrow *= pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_ArrowData.m_fAddBackSpeedX = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_BACK_SPEED_X );
		m_fAttackPowerRateArrow *= pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}


#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	m_ChargeMPData.m_fChargeStateMP = 0.f;
#endif //UPGRADE_SKILL_SYSTEM_2013
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_LGA_COMMUNE_OF_NATURE );
	if( NULL != pMySkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if ( NULL == GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pMySkillTemplet->m_eID, true ) );	/// ��ų ����

		m_ChargeMPData.m_fChargeStateMP = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_ChargeMPData.m_fChargeStateMP = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}


#endif RENA_SECOND_CLASS_CHANGE

#ifdef NEW_SKILL_2010_11
	//{{ oasis907 : ����� [2010.11.3] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_LWS_FRIENDSHIP_OF_NATURE );

	if ( NULL != pMySkillTemplet )
		m_delegateFriendshipOfNature = DelegateFriendshipOfNature::from_method<CX2GULire_ElvenRanger, &CX2GULire_ElvenRanger::ProcessFriendshipNature>( this );
	else
		m_delegateFriendshipOfNature = DelegateFriendshipOfNature();

	if( NULL != pMySkillTemplet )
	{
		if(m_pFriendshipNatureData == NULL)
		{
			m_pFriendshipNatureData = new FriendshipNatureData;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pMySkillTemplet->m_eID, true ) );	/// ��ų ����

			m_pFriendshipNatureData->m_fInvokeRate			= pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_pFriendshipNatureData->m_fInvokeRate			= pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	//}}


#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	m_ArrowData.m_fVibrationShootingInvokeRate = 0.f;
	m_ArrowData.m_fVibrationShootingDamageRate = 0.f;
#endif //UPGRADE_SKILL_SYSTEM_2013
	//{{ oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_LGA_VIBRATION_SHOOTING );
	if( NULL != pMySkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if ( NULL == GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pMySkillTemplet->m_eID, true ) );	/// ��ų ����

		m_ArrowData.m_fVibrationShootingInvokeRate = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillTempletLevel );
		m_ArrowData.m_fVibrationShootingDamageRate = pMySkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_ArrowData.m_fVibrationShootingInvokeRate = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE );
		m_ArrowData.m_fVibrationShootingDamageRate = pMySkillTemplet->m_fPowerRate;
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	//}}
#endif NEW_SKILL_2010_11

	//{{ �޼���� by ������
#ifdef SERV_TRAPPING_RANGER_TEST
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_LNW_VITALPOINT_PIERCING );
#else //UPGRADE_SKILL_SYSTEM_2013
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RTR_VITALPOINT_PIERCING );
#endif //UPGRADE_SKILL_SYSTEM_2013
	if ( NULL != pMySkillTemplet )
		m_delegateVitalPointPiercing = DelegateVitalPointPiercing::from_method<CX2GULire_ElvenRanger, &CX2GULire_ElvenRanger::ProcessVPP>( this );
	else
		m_delegateVitalPointPiercing = DelegateVitalPointPiercing();
#endif SERV_TRAPPING_RANGER_TEST
	//}}
	
#ifdef SERV_RENA_NIGHT_WATCHER

	/// ���� 2�� ���� ����Ʈ ��ó - ���� �ϰ�
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_LNW_PRIOR_PLANNED_BLOW );

	if( NULL != pMySkillTemplet )
	{
		if ( NULL == GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pMySkillTemplet->m_eID, true ) );	/// ��ų ����

		m_fPriorPlannedBlowDownValue	  = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_ABS, iSkillTempletLevel );	/// �ٿ� ��ġ ����
		m_fPriorPlannedBlowIncreaseDamage = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillTempletLevel );		/// ���� ������ ��ġ
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		m_fPriorPlannedBlowIncreaseHPRate = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_HP_INTAKE_REL_DAMAGE, iSkillTempletLevel );		/// ���� ������ ��ġ
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

		m_bUsePriorPlannedBlow			= true;
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	pMySkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RNW_PRIOR_PLANNED_BLOW );

	if( NULL != pMySkillTemplet )
	{
		m_fPriorPlannedBlowDownValue	  = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_ABS );	/// �ٿ� ��ġ ����
		m_fPriorPlannedBlowIncreaseDamage = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );		/// ���� ������ ��ġ

		m_bUsePriorPlannedBlow			= true;
	}
#endif //UPGRADE_SKILL_SYSTEM_2013


#endif SERV_RENA_NIGHT_WATCHER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	//������ ���
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LCR_SOFTBODY, true );
	CX2Unit::UNIT_CLASS eUnitClass = GetUnit()->GetClass();

#ifndef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
	if( iSkillLevel > 0 && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ) )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LCR_SOFTBODY );
		if( NULL != pSkillTemplet )
		{
			float fAvoidancePercent = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel );
			m_AdditionalAvoidance.SetPassiveMultiple( fAvoidancePercent );
		}
	}
#endif //FIX_SKILL_BALANCE_AISHA_LENA

	//����� ���
	m_bAbleAgileMovement = false;
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LCR_AGILE_MOVEMENT, true );
	if( iSkillLevel > 0 && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ) )
	{
		m_bAbleAgileMovement = true;
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

}

/*virtual*/ void CX2GULire_ElvenRanger::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
{
	if ( iSlotIndex_ < 0 || iSlotIndex_ >= ARRAY_SIZE( _CONST_GUUSER_::aNormalSkillStateID ) )
		return;

	int iNormalStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aNormalSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aNormalSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& normalStateData = m_StateList[ iNormalStateID ];
	normalStateData.Init();
	normalStateData.stateID = iNormalStateID;

	int iHyperStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aHyperSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aHyperSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& hyperStateData = m_StateList[ iHyperStateID ];
	hyperStateData.Init();	
	hyperStateData.m_bHyperState = true;
	hyperStateData.stateID = iHyperStateID;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID_ );
	if( NULL == pSkillTemplet )
		return;

	switch( pSkillTemplet->m_eType )
	{	
	case CX2SkillTree::ST_BUFF:
		{	// ���� �ʻ��
			m_LuaManager.MakeTableReference( "LESI_SI_LE_COMMON_BUFF", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, COMMON_BUFF_FrameMove );	
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	

			m_LuaManager.MakeTableReference( "LESI_SI_LE_COMMON_BUFF_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, COMMON_BUFF_FrameMove );	
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );

			return;
		} break;
	case CX2SkillTree::ST_ACTIVE:
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
		{
			// ST_ACTIVE, ST_SPECIAL_ACTIVE�� �Ʒ� �������� ����
		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		{
			switch( eSkillID_ )
			{
			case CX2SkillTree::SI_ETC_WS_COMMON_LOVE:
				{
					m_LuaManager.MakeTableReference( "LESI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

					m_LuaManager.MakeTableReference( "LESI_THROW_ITEM", hyperStateData.stateID );
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
				} break;
			}
			return;
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		{
			return;
		} break;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// ���⼭ ���� ACTIVE, SPECIAL ACTIVE ��ų//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	switch( eSkillID_ )
	{
	default:
		{
			m_LuaManager.MakeTableReference( "LESI_SI_LE_POWER_ATTACK1", normalStateData.stateID );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_LE_POWER_ATTACK_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_LE_POWER_ATTACK_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_LE_POWER_ATTACK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI_LE_POWER_ATTACK1", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_LE_POWER_ATTACK_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_LE_POWER_ATTACK_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_LE_POWER_ATTACK_HYPER_EventProcess );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LE_RAIL_STINGER:		//** Code ���� �̸��� ���Ǵ� �̸��� �ٸ� : �Լ��� ���� �ʿ�
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LE_RAIL_STINGER:		//** Code ���� �̸��� ���Ǵ� �̸��� �ٸ� : �Լ��� ���� �ʿ�
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SI1_LE_PIERCE_ARROW", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI1_LE_PIERCE_ARROW_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_HYPER_Init );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_PIERCE_ARROW_HYPER_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LE_ASSAULT_KICK:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LE_ASSAULT_KICK:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SI1_LE_ASSAULT_KICK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI1_LE_ASSAULT_KICK_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LE_PERFECT_STORM:
		{
			m_LuaManager.MakeTableReference( "LESI_SI2_LE_PERFECT_STORM", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI2_LE_PERFECT_STORM_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_PERFECT_STORM_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LE_MULTIPLE_STINGER:	//** Code ���� �̸��� ���Ǵ� �̸��� �ٸ� : �Լ��� ���� �ʿ�
		{
			// ��Ƽ�� ���ð� ����
  			m_LuaManager.MakeTableReference( "LESI_SI2_LE_DIVISION_BLAST", normalStateData.stateID );
  			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_Init );
  			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_Start );
  			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_FrameMoveFuture );
 			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_FrameMove );
 			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_CameraMove );
  			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_EventProcess );
  
  			m_LuaManager.MakeTableReference( "LESI_SI2_LE_DIVISION_BLAST_HYPER", hyperStateData.stateID );
  			hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_HYPER_Init );
  			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_HYPER_Start );
  			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_HYPER_FrameMoveFuture );
  			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_HYPER_FrameMove );
  			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_HYPER_CameraMove );
  			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_DIVISION_BLAST_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LE_AERO_TORNADO:
		{
			m_LuaManager.MakeTableReference( "LESI_SI3_LE_AERO_TORNADO", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI3_LE_AERO_TORNADO_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_AERO_TORNADO_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LE_PHOENIX_STRIKE:
		{

			m_LuaManager.MakeTableReference( "LESI_SI3_LE_PHOENIX_STRIKE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI3_LE_PHOENIX_STRIKE_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_HYPER_Init );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LE_PHOENIX_STRIKE_HYPER_EventProcess );
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	case CX2SkillTree::SI_A_LCR_MIDDLE_KICK:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LCR_MIDDLE_KICK", normalStateData.stateID ); 		
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LCR_MIDDLE_KICK_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LCR_MIDDLE_KICK_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LCR_MIDDLE_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LCR_MIDDLE_KICK", hyperStateData.stateID ); 
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_A_LWS_BACK_KICK:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LWS_BACK_KICK", normalStateData.stateID ); 		
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_BACK_KICK_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_BACK_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LWS_BACK_KICK", hyperStateData.stateID ); 
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_A_LWS_HIGH_KICK:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LWS_HIGH_KICK", normalStateData.stateID ); 		
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_HIGH_KICK_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_HIGH_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LWS_HIGH_KICK", hyperStateData.stateID ); 
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_SA_LWS_AIRELINNA_SYLPH:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LWS_AIRELINNA_SYLPH", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_AIRELINNA_SYLPH_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_AIRELINNA_SYLPH_FrameMove );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS


			m_LuaManager.MakeTableReference( "LESI_SA_LWS_AIRELINNA_SYLPH", hyperStateData.stateID ); 
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
	#endif CONVERSION_VS
		} break;

	case CX2SkillTree::SI_A_LSR_SHOOTING_MAGNUM:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LSR_SHOOTING_MAGNUM", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LSR_SHOOTING_MAGNUM_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LSR_SHOOTING_MAGNUM_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LSR_SHOOTING_MAGNUM_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LSR_SHOOTING_MAGNUM", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

		} break;

	case CX2SkillTree::SI_A_LGA_ARC_SHOT:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LGA_ARC_SHOT", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_ARC_SHOT_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_ARC_SHOT_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_ARC_SHOT_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LGA_ARC_SHOT", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_A_LGA_RAPID_SHOT:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LGA_RAPID_SHOT_READY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_READY_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_READY_StartFuture );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_READY_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LGA_RAPID_SHOT_READY", hyperStateData.stateID );
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= LESI_A_LGA_RAPID_SHOT_ATTACK;
				m_LuaManager.MakeTableReference( "LESI_A_LGA_RAPID_SHOT_ATTACK", stateData.stateID );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_ATTACK_EventProcess );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_ATTACK_FrameMove );
				m_StateList[stateData.stateID]	= stateData;

				stateData.Init();
				stateData.stateID				= LESI_A_LGA_RAPID_SHOT_FINISH;
				m_LuaManager.MakeTableReference( "LESI_A_LGA_RAPID_SHOT_FINISH", stateData.stateID );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_FINISH_EventProcess );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_RAPID_SHOT_FINISH_FrameMove );
				m_StateList[stateData.stateID]	= stateData;
			}
#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			if( 0 > m_RapidShotDataPtr.use_count() )
				m_RapidShotDataPtr.reset();
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

			m_RapidShotDataPtr = RapidShotData::CreateRapidShotData();
			m_RapidShotDataPtr->SetSlotID_RapidShot( static_cast<LIRE_ELVENRANGER_STATE_ID>( iNormalStateID ) );
			m_RapidShotDataPtr->SetSlotB( bSlotB_ );

		} break;

	case CX2SkillTree::SI_A_LGA_FREEZING_BOLT:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LGA_FREEZING_BOLT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_FREEZING_BOLT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_FREEZING_BOLT_FrameMove );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "LESI_A_LGA_FREEZING_BOLT", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS
		} break;

	case CX2SkillTree::SI_A_LGA_WIND_BLAST:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LGA_WIND_BLAST", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_WIND_BLAST_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_WIND_BLAST_FrameMove );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "LESI_A_LGA_WIND_BLAST", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS
		} break;

	case CX2SkillTree::SI_SA_LTR_TRAPPING_ARROW_FUNGUS:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_TRAPPING_ARROW_FUNGUS", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_StateEnd );

			m_LuaManager.MakeTableReference( "LESI_SA_RTR_TRAPPING_ARROW_FUNGUS", hyperStateData.stateID );
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;
		} break;

	case CX2SkillTree::SI_A_RNW_THORNS_TRAP:
		{
			m_LuaManager.MakeTableReference( "LESI_A_RNW_THORNS_TRAP", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_THORNS_TRAP_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_THORNS_TRAP_StartFuture );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_THORNS_TRAP_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_THORNS_TRAP_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_THORNS_TRAP_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_THORNS_TRAP_EndFuture );

			m_LuaManager.MakeTableReference( "LESI_A_RNW_THORNS_TRAP", hyperStateData.stateID );
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMoveFuture	= normalStateData.OnFrameMoveFuture;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEndFuture		= normalStateData.StateEndFuture;
		} break;

	case CX2SkillTree::SI_A_RNW_SEED_OF_RUIN:
		{
			m_LuaManager.MakeTableReference( "LESI_P_RNW_SEED_OF_RUIN", normalStateData.stateID );
			normalStateData.StateInit				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_P_RNW_SEED_OF_RUIN_Init );
			normalStateData.StateStartFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_P_RNW_SEED_OF_RUIN_StartFuture );
			normalStateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_P_RNW_SEED_OF_RUIN_FrameMoveFuture );
			normalStateData.OnFrameMove				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_P_RNW_SEED_OF_RUIN_FrameMove );
			normalStateData.OnEventProcess			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_P_RNW_SEED_OF_RUIN_EventProcess );
			normalStateData.StateEndFuture			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_P_RNW_SEED_OF_RUIN_EndFuture );

			m_LuaManager.MakeTableReference( "LESI_P_RNW_SEED_OF_RUIN", hyperStateData.stateID );
			hyperStateData.StateInit				= normalStateData.StateInit;
			hyperStateData.StateStartFuture			= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMoveFuture		= normalStateData.OnFrameMoveFuture;
			hyperStateData.OnFrameMove				= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess			= normalStateData.OnEventProcess;
			hyperStateData.StateEndFuture			= normalStateData.StateEndFuture;
		} break;

	case CX2SkillTree::SI_A_RNW_ANGER_OF_ELF:
		{
			m_LuaManager.MakeTableReference( "LESI_A_RNW_ANGER_OF_ELF", normalStateData.stateID );
			normalStateData.StateInit				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ANGER_OF_ELF_Init );
			normalStateData.StateStart				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ANGER_OF_ELF_StateStart );
			normalStateData.OnFrameMove				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ANGER_OF_ELF_FrameMove );
			normalStateData.OnEventProcess			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ANGER_OF_ELF_EventProcess );
			normalStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ANGER_OF_ELF_StateEnd );

			m_LuaManager.MakeTableReference( "LESI_A_RNW_ANGER_OF_ELF", hyperStateData.stateID );
			hyperStateData.m_bHyperState			= true;
			hyperStateData.StateInit				= normalStateData.StateInit;
			hyperStateData.StateStart				= normalStateData.StateStart;
			hyperStateData.OnFrameMove				= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess			= normalStateData.OnEventProcess;
			hyperStateData.StateEnd					= normalStateData.StateEnd;
		} break;

	case CX2SkillTree::SI_A_RNW_ROSEBUSH_TRAP:
		{
			m_LuaManager.MakeTableReference( "LESI_A_RNW_ROSEBUSH_TRAP", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ROSEBUSH_TRAP_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ROSEBUSH_TRAP_StartFuture );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ROSEBUSH_TRAP_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ROSEBUSH_TRAP_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ROSEBUSH_TRAP_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_ROSEBUSH_TRAP_EndFuture );

			m_LuaManager.MakeTableReference( "LESI_A_RNW_ROSEBUSH_TRAP", hyperStateData.stateID );
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMoveFuture	= normalStateData.OnFrameMoveFuture;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEndFuture		= normalStateData.StateEndFuture;
		} break;

	case CX2SkillTree::SI_A_LWS_ASSAULT_IMPACT:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LWS_ASSAULT_IMPACT", normalStateData.stateID );
			normalStateData.StateInit				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_ASSAULT_IMPACT_Init );
			normalStateData.OnEventProcess			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_ASSAULT_IMPACT_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LWS_ASSAULT_IMPACT", hyperStateData.stateID );
			hyperStateData.StateInit				= normalStateData.StateInit;
			hyperStateData.OnEventProcess			= normalStateData.OnEventProcess;

		} break;

	case CX2SkillTree::SI_A_LTR_ENTANGLE:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LTR_ENTANGLE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LTR_ENTANGLE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LTR_ENTANGLE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LTR_ENTANGLE_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LTR_ENTANGLE", hyperStateData.stateID );
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LCR_SPINNING_KICK:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LCR_SPINNING_KICK:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SI1_LCR_SPINNING_KICK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI1_LCR_SPINNING_KICK_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LCR_SPINNING_KICK_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LCR_CRESCENT_KICK:
		{
			m_LuaManager.MakeTableReference( "LESI_SI2_LCR_CRESCENT_KICK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LCR_CRESCENT_KICK_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LCR_CRESCENT_KICK_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LCR_CRESCENT_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI2_LCR_CRESCENT_KICK_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LCR_CRESCENT_KICK_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LCR_CRESCENT_KICK_HYPER_EventProcess );
		} break;

	case CX2SkillTree::SI_SA_LCR_DIVE_KICK_BOMBING:
		{
			m_LuaManager.MakeTableReference( "LESI_SI3_LCR_DIVE_KICK_BOMBING", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_EventProcess );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LSR_HUMMING_WIND:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LSR_HUMMING_WIND:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SI1_LE_HUMMING_WIND", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI1_LE_HUMMING_WIND_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_HUMMING_WIND_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LSR_GUIDED_ARROW:
		{ 
			m_LuaManager.MakeTableReference( "LESI_SI2_LE_GUIDED_ARROW", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI2_LE_GUIDED_ARROW_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI2_LE_GUIDED_ARROW_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_LSR_CRAZY_SHOT:
		{
			m_LuaManager.MakeTableReference( "LESI_SI3_LSR_CRAZY_SHOT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LSR_CRAZY_SHOT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LSR_CRAZY_SHOT_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LSR_CRAZY_SHOT_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI3_LSR_CRAZY_SHOT_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LSR_CRAZY_SHOT_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI3_LSR_CRAZY_SHOT_HYPER_EventProcess );

		} break;


	case CX2SkillTree::SI_A_LE_SIEGEMODE:
		{
			m_LuaManager.MakeTableReference( "LESI_SIEGE_READY", normalStateData.stateID );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeReadyStateStartFuture );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeReadyStateStart );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeReadyEventProcess );

			m_LuaManager.MakeTableReference( "LESI_SIEGE_READY", hyperStateData.stateID );
			hyperStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeReadyStateStartFuture );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeReadyStateStart );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, SiegeReadyEventProcess );

		} break;


	case CX2SkillTree::SI_A_LE_BACKJUMP_SHOT:
		{
			m_LuaManager.MakeTableReference( "LESI_SI_A_LE_BACKJUMP_SHOT", normalStateData.stateID ); 
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_Start );				
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_FrameMoveFuture );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_EventProcess );
			normalStateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_EndFuture );

			m_LuaManager.MakeTableReference( "LESI_SI_A_LE_BACKJUMP_SHOT", hyperStateData.stateID ); 
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_Start );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_FrameMoveFuture );	
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_EventProcess );
#ifdef FIX_BACKJUMP_SHOT_FREEZE
			hyperStateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_EndFuture );
#else FIX_BACKJUMP_SHOT_FREEZE
			normalStateData.StateEndFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_BACKJUMP_SHOT_EndFuture );
#endif FIX_BACKJUMP_SHOT_FREEZE
		} break;

	case CX2SkillTree::SI_A_LE_REFLEX_MAGIC:
		{				
			m_LuaManager.MakeTableReference( "LESI_SI_A_LE_REFLEX_MAGIC_READY", normalStateData.stateID ); 
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_REFLEX_MAGIC_READY_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SI_A_LE_REFLEX_MAGIC_READY", hyperStateData.stateID ); 
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_REFLEX_MAGIC_READY_EventProcess );


			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= LESI_SI_A_LE_REFLEX_MAGIC;
				m_LuaManager.MakeTableReference( "LESI_SI_A_LE_REFLEX_MAGIC", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_REFLEX_MAGIC_Init );
				stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_REFLEX_MAGIC_Start );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI_A_LE_REFLEX_MAGIC_EventProcess );

				m_StateList[stateData.stateID] = stateData;
			}
		} break;


#ifdef SKILL_30_TEST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			case CX2SkillTree::SI_SA_LWS_SHARPFALL:
#else //UPGRADE_SKILL_SYSTEM_2013
			case CX2SkillTree::SI_SA_LCR_SHARPFALL:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LCR_SHARPFALL", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_Init );				
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_EventProcess );				


			m_LuaManager.MakeTableReference( "LESI_SA_LCR_SHARPFALL_HYPER", hyperStateData.stateID ); 
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_HYPER_EventProcess );	





			{
				UserUnitStateData stateData;


				stateData.Init();
				stateData.stateID			= LESI_SA_LCR_SHARPFALL_LANDING;
				m_LuaManager.MakeTableReference( "LESI_SA_LCR_SHARPFALL_LANDING", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_LANDING_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_SHARPFALL_LANDING_EventProcess );	
				m_StateList[stateData.stateID] = stateData;
			}


		} break;



	case CX2SkillTree::SI_SA_LSR_WINDWARD:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LSR_WINDWARD", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_StateEnd );


			m_LuaManager.MakeTableReference( "LESI_SA_LSR_WINDWARD_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_WINDWARD_HYPER_StateEnd );


		} break;


	case CX2SkillTree::SI_A_LSR_ENTANGLE:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LSR_ENTANGLE", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_StateEnd );
			normalStateData.StateStartFuture= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_StateStartFuture );

			m_LuaManager.MakeTableReference( "LESI_SA_LSR_ENTANGLE", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_StateEnd );
			hyperStateData.StateStartFuture = SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_ENTANGLE_StateStartFuture );
		} break;

	case CX2SkillTree::SI_A_LCR_LOW_KICK:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LCR_LOW_KICK", normalStateData.stateID ); 		
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_Init );
#ifndef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - �ο�ű �޸� �̵�ű���� �����Ǹ鼭, Start ������Ʈ ����
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_StateStart );
#endif // UPGRADE_SKILL_SYSTEM_2013 
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_EventProcess );


			m_LuaManager.MakeTableReference( "LESI_SA_LCR_LOW_KICK", hyperStateData.stateID ); 
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_Init );
#ifndef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - �ο�ű �޸� �̵�ű���� �����Ǹ鼭, Start ������Ʈ ����
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_StateStart );
#endif // UPGRADE_SKILL_SYSTEM_2013 
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_LOW_KICK_EventProcess );
		} break;

#endif SKILL_30_TEST		

#ifdef SKILL_CASH_10_TEST
	case CX2SkillTree::SI_SA_LSR_GUNGNIR:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LSR_GUNGNIR", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_StateEnd );


			m_LuaManager.MakeTableReference( "LESI_SA_LSR_GUNGNIR_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LSR_GUNGNIR_HYPER_StateEnd );
		} break;

	case CX2SkillTree::SI_SA_LCR_VIOLENT_ATTACK:
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LCR_VIOLENT_ATTACK_READY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMove );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMoveFuture );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_StateEnd );	

			m_LuaManager.MakeTableReference( "LESI_SA_LCR_VIOLENT_ATTACK_READY_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_Init );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_Hyper_FrameMove );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMoveFuture );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_READY_StateEnd );

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= LESI_SA_LCR_VIOLENT_ATTACK;
				m_LuaManager.MakeTableReference( "LESI_SA_LCR_VIOLENT_ATTACK", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= LESI_SA_LCR_VIOLENT_ATTACK_HYPER;
				m_LuaManager.MakeTableReference( "LESI_SA_LCR_VIOLENT_ATTACK_HYPER", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_StateEnd );					
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= LESI_SA_LCR_VIOLENT_ATTACK_LANDING;
				m_LuaManager.MakeTableReference( "LESI_SA_LCR_VIOLENT_ATTACK_LANDING", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_StateEnd );					
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= LESI_SA_LCR_VIOLENT_ATTACK_LANDING_HYPER;
				m_LuaManager.MakeTableReference( "LESI_SA_LCR_VIOLENT_ATTACK_LANDING_HYPER", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Hyper_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LCR_VIOLENT_ATTACK_LANDING_StateEnd );					
				m_StateList[stateData.stateID] = stateData;
			}

		} break;
#endif


#ifdef RENA_SECOND_CLASS_CHANGE

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LWS_SLIDE_KICK:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LWS_SLIDE_KICK:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{

			m_LuaManager.MakeTableReference( "LESI_SA_LWS_SLIDE_KICK", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_StateEnd );


			m_LuaManager.MakeTableReference( "LESI_SA_LWS_SLIDE_KICK", hyperStateData.stateID ); 				
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_EventProcess );
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_StateEnd );



			{
				UserUnitStateData stateData;


				stateData.Init();
				stateData.stateID			= LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH;
				m_LuaManager.MakeTableReference( "LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH", stateData.stateID );
				//stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH_EventProcess );	
				m_StateList[stateData.stateID] = stateData; 


				stateData.Init();
				stateData.stateID			= LESI_SA_LWS_SLIDE_KICK_FINISH;
				m_LuaManager.MakeTableReference( "LESI_SA_LWS_SLIDE_KICK_FINISH", stateData.stateID );
#ifdef CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
#else CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
#endif CONVERSION_VS
				m_StateList[stateData.stateID] = stateData; 


				//stateData.Init();
				//stateData.stateID			= LESI_SA_LWS_SLIDE_KICK_FINISH_HYPER;
				//m_LuaManager.MakeTableReference( "LESI_SA_LWS_SLIDE_KICK_FINISH_HYPER", stateData.stateID );
				//stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_FINISH_HYPER_FrameMove );	
				//stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
				//m_StateList[stateData.stateID] = stateData;


				stateData.Init();
				stateData.stateID			= LESI_SA_LWS_SLIDE_KICK_CANCEL;
				m_LuaManager.MakeTableReference( "LESI_SA_LWS_SLIDE_KICK_CANCEL", stateData.stateID );
				//stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_CANCEL_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_SLIDE_KICK_CANCEL_EventProcess );	
				m_StateList[stateData.stateID] = stateData; 


			}


		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LGA_FREEZING_ARROW:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LGA_FREEZING_ARROW:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LGA_FREEZING_ARROW", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_FREEZING_ARROW_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_FREEZING_ARROW_FrameMove );	
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "LESI_SA_LGA_FREEZING_ARROW_HYPER", hyperStateData.stateID ); 
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_FREEZING_ARROW_HYPER_FrameMove );
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
	#endif CONVERSION_VS

		} break;

	case CX2SkillTree::SI_A_LWS_NATURE_FORCE:
		{


			m_LuaManager.MakeTableReference( "LESI_A_LWS_NATURE_FORCE", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_NATURE_FORCE_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_NATURE_FORCE_StateStart );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );				
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS


			m_LuaManager.MakeTableReference( "LESI_A_LWS_NATURE_FORCE_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LWS_NATURE_FORCE_HYPER_StateStart );
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
	#endif CONVERSION_VS



		} break;

	case CX2SkillTree::SI_A_LGA_STIGMA_ARROW:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LGA_STIGMA_ARROW", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_STIGMA_ARROW_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_STIGMA_ARROW_FrameMove );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS


			m_LuaManager.MakeTableReference( "LESI_A_LGA_STIGMA_ARROW_HYPER", hyperStateData.stateID ); 
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LGA_STIGMA_ARROW_FrameMove );
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
	#endif CONVERSION_VS
		} break;

		//{{ ����� 2010.11.2
#ifdef NEW_SKILL_2010_11
	case CX2SkillTree::SI_SA_LGA_AERO_STRAFE:
		{ // ����� ��Ʈ������
			m_LuaManager.MakeTableReference( "LESI_SA_LGA_AERO_STRAFE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_StateEndFurtue );

			m_LuaManager.MakeTableReference( "LESI_SA_LGA_AERO_STRAFE_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LGA_AERO_STRAFE_HYPER_EventProcess );
			hyperStateData.StateEndFuture		= normalStateData.StateEndFuture;

		} break;
#endif NEW_SKILL_2010_11
		//}} ����� 2010.11.2

#endif RENA_SECOND_CLASS_CHANGE
#ifdef NEW_SKILL_2010_11
		//{{ oasis907 : ����� [2010.11.5] // ���� ����Ŀ - ���̷�����
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_LWS_AIRELINNA_NYMPH:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_LWS_AIRELINNA:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_LWS_AIRELINNA", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_AIRELINNA_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_AIRELINNA_FrameMove );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );				
	#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "LESI_SA_LWS_AIRELINNA_HYPER", hyperStateData.stateID ); 
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_LWS_AIRELINNA_FrameMove );
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
	#else CONVERSION_VS
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericSpecialActiveSkillEventProcess );	
	#endif CONVERSION_VS
		} break;
		//}} oasis907 : ����� [2010.11.5] // ���� ����Ŀ - ���̷�����
#endif NEW_SKILL_2010_11

#ifdef	SERV_TRAPPING_RANGER_TEST
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LTR_EVOKE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RTR_EVOKE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_EVOKE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_FrameMove );
	#ifdef CONVERSION_VS
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
	#else CONVERSION_VS
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericActiveSkillEventProcess );
	#endif CONVERSION_VS
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_StateEnd );

			m_LuaManager.MakeTableReference( "LESI_SA_RTR_EVOKE", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_HYPER_FrameMove );
	#ifdef CONVERSION_VS
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
	#else CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, GenericActiveSkillEventProcess );
	#endif CONVERSION_VS
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_EVOKE_StateEnd );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LTR_EXPLOSION_TRAP:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_RTR_EXPLOSION_TRAP:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_A_RTR_EXPLOSION_TRAP", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_StartFuture );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_Start );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_EndFuture );

			m_LuaManager.MakeTableReference( "LESI_A_RTR_EXPLOSION_TRAP", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_Init );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_StartFuture );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_Start );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_EventProcess );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RTR_EXPLOSION_TRAP_EndFuture );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_LTR_TRAPPING_ARROW_BURST:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RTR_TRAPING_ARROW:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_TRAPING_ARROW", normalStateData.stateID );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_StateStart );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_StateEnd );

			m_LuaManager.MakeTableReference( "LESI_SA_RTR_TRAPING_ARROW", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_StateStart );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_TRAPING_ARROW_StateEnd );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_LTR_KARMA:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RTR_KARMA:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_KARMA", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_StateEnd );

			m_LuaManager.MakeTableReference( "LESI_SA_RTR_KARMA", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_KARMA_StateEnd );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_LTR_FATALITY:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RTR_FATALITY:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_FATALITY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateStart );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateStartFuture );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateEnd );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateEndFuture );

			m_LuaManager.MakeTableReference( "LESI_SA_RTR_FATALITY", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateStart );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateStartFuture );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateEnd );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_FATALITY_StateEndFuture );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_LTR_CALL_OF_RUIN:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RTR_CALL_OF_RUIN:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "LESI_SA_RTR_CALL_OF_RUIN", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_StateStart );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_StateEnd );

			m_LuaManager.MakeTableReference( "LESI_SA_RTR_CALL_OF_RUIN", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_StateStart );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RTR_CALL_OF_RUIN_StateEnd );

		} break;

	case CX2SkillTree::SI_A_LE_RISING_FALCON:
		{
			m_LuaManager.MakeTableReference( "LESI_A_LE_RISING_FALCON", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_LE_RISING_FALCON", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_LE_RISING_FALCON_EventProcess );

		} break;
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LNW_FURIOUS_ENGAGE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RNW_FURIOUS_ENGAGE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			//���� 2�� ���� ����Ʈ ���� - ǻ��� �ΰ�����
			m_LuaManager.MakeTableReference( "LESI_SA_RNW_FURIOUS_ENGAGE", normalStateData.stateID );
			//normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SI1_LE_ASSAULT_KICK_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_StateStart );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SA_RNW_FURIOUS_ENGAGE", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_StateStart );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_FURIOUS_ENGAGE_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_LNW_GLIDING_STRIKE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RNW_GLIDING_STRIKE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			//���� 2�� ���� ����Ʈ ���� - �۶��̵� ��Ʈ����ũ
			m_LuaManager.MakeTableReference( "LESI_SA_RNW_GLIDING_STRIKE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_StateStartFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_FrameMove );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_FrameMoveFuture );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_StateEndFuture );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_SA_RNW_GLIDING_STRIKE", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_Init );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_StateStartFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_FrameMove );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_FrameMoveFuture );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_StateEndFuture );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_SA_RNW_GLIDING_STRIKE_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LNW_DELAYED_FIRING:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			/// ������ ��ȣź State
			m_LuaManager.MakeTableReference( "LESI_A_RNW_START_OF_DELAYED_FIRING", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_StateStart );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_StateStartFuture );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMove );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_StateEnd );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_EventProcess );

			m_LuaManager.MakeTableReference( "LESI_A_RNW_START_OF_DELAYED_FIRING", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_StateStart );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_StateStartFuture );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMove );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_StateEnd );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_EventProcess );


			/// ��ü ���� State
			/*UserUnitStateData stateData;

			stateData.Init();
			stateData.stateID = LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE;
			m_LuaManager.MakeTableReference( "LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_StateStart );	
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_FrameMove );	
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_EventProcess );
			m_StateList[stateData.stateID] = stateData;*/


			/// ������ ��ȣź ���� ������ ��ġ ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			const CX2SkillTree::SkillTemplet* pMySkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LNW_DELAYED_FIRING );

			if( NULL != pMySkillTemplet )
			{
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pMySkillTemplet->m_eID ) );	/// ��ų ����

				m_fStartOfDelayedFiringIncreaseDamage = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillTempletLevel );	/// ���� ������ ��ġ
			}
#else //UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pMySkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING );

			if( NULL != pMySkillTemplet )
				m_fStartOfDelayedFiringIncreaseDamage = pMySkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );	/// ���� ������ ��ġ
#endif //UPGRADE_SKILL_SYSTEM_2013

		} break;
#endif SERV_RENA_NIGHT_WATCHER


#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

		case CX2SkillTree::SI_HA_LWS_SPIRAL_STRIKE :
			{
				// �����̷� ��Ʈ����ũ
				m_LuaManager.MakeTableReference( "LESI_HA_LWS_SPIRAL_STIKE_READY", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_READY_Init );			
				normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_READY_StateStart );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_READY_EventProcess );

				m_LuaManager.MakeTableReference( "LESI_HA_LWS_SPIRAL_STIKE_READY", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_READY_Init );			
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_READY_StateStart );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_READY_EventProcess );

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= LESI_HA_LWS_SPIRAL_STIKE_SPINING;
					m_LuaManager.MakeTableReference( "LESI_HA_LWS_SPIRAL_STIKE_SPINING", stateData.stateID );

					stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_SPINING_StateStart );			
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_SPINING_FrameMove );			
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_SPINING_EventProcess );
					stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_SPINING_StateEnd );

						
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK;
					m_LuaManager.MakeTableReference( "LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK", stateData.stateID );
					stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_Init );					
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_FrameMove );	
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_EventProcess );	
					m_StateList[stateData.stateID] = stateData;
				}	
			} break;

		case CX2SkillTree::SI_HA_LGA_CRYOTRON_BOLT :
			{
				// ũ���ƿ�Ʈ�� ��Ʈ
				m_LuaManager.MakeTableReference( "LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_Init );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_StateStart );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_EventProcess );

				m_LuaManager.MakeTableReference( "LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_Init );
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_StateStart );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_EventProcess );
				{

					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT;
					m_LuaManager.MakeTableReference( "LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT_EventProcess );
					m_StateList[stateData.stateID] = stateData;


					stateData.Init();
					stateData.stateID			= LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING;
					m_LuaManager.MakeTableReference( "LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING_EventProcess );
					m_StateList[stateData.stateID] = stateData;

				}	
			} break;

		case CX2SkillTree::SI_HA_LNW_INNOCENT:
			{
				//����Ʈ ���� - �̳뼾Ʈ
				m_LuaManager.MakeTableReference( "LESI_HA_LNW_INNOCENT_START", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LNW_INNOCENT_START_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LNW_INNOCENT_START_EventProcess );

				m_LuaManager.MakeTableReference( "LESI_HA_LNW_INNOCENT_START", hyperStateData.stateID );
				hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= LESI_HA_LNW_INNOCENT_END;
					m_LuaManager.MakeTableReference( "LESI_HA_LNW_INNOCENT_END", stateData.stateID );
					stateData.StateInit			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LNW_INNOCENT_END_Init );
					stateData.StateStart		= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LNW_INNOCENT_END_StateStart );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LNW_INNOCENT_END_EventProcess );
					stateData.StateEnd			= SET_CB_FUNC( CX2GULire_ElvenRanger, LESI_HA_LNW_INNOCENT_END_StateEnd );
					m_StateList[stateData.stateID] = stateData;
				}
			} break;
#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���
	}
}

/*virtual*/ void CX2GULire_ElvenRanger::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	if ( NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
#endif // UPGRADE_SKILL_SYSTEM_2013

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LE_REFLEX_MAGIC == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LE_REFLEX_MAGIC );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			UserUnitStateData& stateData = m_StateList[ LESI_SI_A_LE_REFLEX_MAGIC ];
			stateData.m_SPLevel		= iSkillTempletLevel;
			stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateData = m_StateList[ LESI_SI_A_LE_REFLEX_MAGIC ];
			stateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			stateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_LWS_SHARPFALL == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_LCR_SHARPFALL == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LWS_SHARPFALL );

		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			UserUnitStateData& stateData = m_StateList[ LESI_SA_LCR_SHARPFALL_LANDING ];
			stateData.m_SPLevel		= iSkillTempletLevel;
			stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateData.m_eSkillID	= pSkillTemplet->m_eID;

		}
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LCR_SHARPFALL );

		if( NULL != pSkillTemplet )
		{
			UserUnitStateData& stateData = m_StateList[ LESI_SA_LCR_SHARPFALL_LANDING ];
			stateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			stateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			stateData.m_eSkillID	= pSkillTemplet->m_eID;

		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LSR_ENTANGLE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LSR_ENTANGLE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			m_MaxEntangleTrapCount = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel );
			m_fEntangleTrapAttackDuration = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_MaxEntangleTrapCount = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT );
			m_fEntangleTrapAttackDuration = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013

			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO12 ) == true )
			{
				m_MaxEntangleTrapCount += 1;
			}
		}
	}
	
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_LCR_VIOLENT_ATTACK == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LCR_VIOLENT_ATTACK );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			{
				UserUnitStateData& normalStateData = m_StateList[ LESI_SA_LCR_VIOLENT_ATTACK ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ LESI_SA_LCR_VIOLENT_ATTACK_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}		
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ LESI_SA_LCR_VIOLENT_ATTACK ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ LESI_SA_LCR_VIOLENT_ATTACK_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}		
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	
#ifdef RENA_SECOND_CLASS_CHANGE

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LWS_SLIDE_KICK == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LWS_SLIDE_KICK );

		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

			UserUnitStateData& normalStateData = m_StateList[ LESI_SA_LWS_SLIDE_KICK_FINISH ];
			normalStateData.m_SPLevel		= iSkillTempletLevel;
			normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_LWS_SLIDE_KICK == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LWS_SLIDE_KICK );

		if( NULL != pSkillTemplet )
		{
			UserUnitStateData& normalStateData = m_StateList[ LESI_SA_LWS_SLIDE_KICK_FINISH ];
			normalStateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			normalStateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#endif RENA_SECOND_CLASS_CHANGE


#ifdef BALANCE_PATCH_20120329
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LE_RISING_FALCON == eSkillID_ )
	{

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LE_RISING_FALCON );

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

			UserUnitStateData& normalStateData = m_StateList[ LESI_SI_A_LE_RISING_FALCON_LANDING ];
			normalStateData.m_SPLevel		= iSkillTempletLevel;
			normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillTemplet )
		{
			UserUnitStateData& normalStateData = m_StateList[ LESI_SI_A_LE_RISING_FALCON_LANDING ];
			normalStateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			normalStateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013

	}
#endif BALANCE_PATCH_20120329

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LGA_RAPID_SHOT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LGA_RAPID_SHOT );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			{
				UserUnitStateData& normalStateData = m_StateList[ LESI_A_LGA_RAPID_SHOT_ATTACK ];
				normalStateData.m_SPLevel		= iSkillTempletLevel;
				normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ LESI_A_LGA_RAPID_SHOT_FINISH ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_RNW_ANGER_OF_ELF == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RNW_ANGER_OF_ELF );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			m_MaxAngerOfElfTrapCount = static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ) );
		}
	}

	/// ���� ���
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_LE_SIEGEMODE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LE_SIEGEMODE );
		if( NULL != pSkillTemplet )
		{
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

			SetEquippedSkillLevelStateData( pSkillTemplet, LESI_SIEGE_READY );
			SetEquippedSkillLevelStateData( pSkillTemplet, LESI_SIEGE_WAIT );
			SetEquippedSkillLevelStateData( pSkillTemplet, LESI_SIEGE_ATTACK );
			SetEquippedSkillLevelStateData( pSkillTemplet, LESI_SIEGE_ATTACK_X );
			SetEquippedSkillLevelStateData( pSkillTemplet, LESI_SIEGE_END );

			SetSiegeModeEffectiveValue( iSkillTempletLevel );	/// ���� ��带 ���� �����Ǵ� ��ġ ����
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013


#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���
	
	// �����̷� ��Ʈ����ũ, ���� ���� ó��
	if ( true == bChangeAll_ || CX2SkillTree::SI_HA_LWS_SPIRAL_STRIKE == eSkillID_ )
	{

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_HA_LWS_SPIRAL_STRIKE );

		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

			{
				UserUnitStateData& normalStateData = m_StateList[ LESI_HA_LWS_SPIRAL_STIKE_SPINING ];
				normalStateData.m_SPLevel		= iSkillTempletLevel;
				normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK ];
				normalStateData.m_SPLevel		= iSkillTempletLevel;
				normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
			}
		}
	}

	// ũ���ƿ�Ʈ�� ��Ʈ, ���� ���� ó��
	if ( true == bChangeAll_ || CX2SkillTree::SI_HA_LGA_CRYOTRON_BOLT == eSkillID_ )
	{

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_HA_LGA_CRYOTRON_BOLT );

		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
			
			
			{

				UserUnitStateData& normalStateData = m_StateList[ LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT ];
				normalStateData.m_SPLevel		= iSkillTempletLevel;
				normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
			}
			{

				UserUnitStateData& normalStateData = m_StateList[ LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING ];
				normalStateData.m_SPLevel		= iSkillTempletLevel;
				normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
			}

		
		}
	}

	//�̳뼾Ʈ (��â��)
	if ( true == bChangeAll_ || CX2SkillTree::SI_HA_LNW_INNOCENT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_HA_LNW_INNOCENT );

		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

			UserUnitStateData& normalStateData = m_StateList[ LESI_HA_LNW_INNOCENT_END ];
			normalStateData.m_SPLevel		= iSkillTempletLevel;
			normalStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			normalStateData.m_eSkillID		= pSkillTemplet->m_eID;
		}
	}

#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

}


#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GULire_ElvenRanger::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Lire_ChargeSoulA.X" );
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Lire_ChargeSoulB.X" );
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Lire_ChargeSoulA_1.X" );
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GULire_ElvenRanger::InitDevice()
{
	CX2GUUser::InitDevice();


	CKTDXDeviceXSkinMesh* pChargeMPA = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Lire_ChargeSoulA.X" );
	CKTDXDeviceXSkinMesh* pChargeMPB = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Lire_ChargeSoulB.X" );
	CKTDXDeviceXSkinMesh* pChargeMPA1 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Lire_ChargeSoulA_1.X" );

	DeviceReady( pChargeMPA  );
	DeviceReady( pChargeMPB  );
	DeviceReady( pChargeMPA1 );


	m_pMPChargeA			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pMPChargeB			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pMPChargeC			= CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMPChargeA );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMPChargeB );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMPChargeC );

	m_pMPChargeA->SetAnimXSkinMesh( pChargeMPA );
	m_pMPChargeA->AddModelXSkinMesh( pChargeMPA );
	m_pMPChargeB->SetAnimXSkinMesh( pChargeMPB );
	m_pMPChargeB->AddModelXSkinMesh( pChargeMPB );
	m_pMPChargeC->SetAnimXSkinMesh( pChargeMPA1 );
	m_pMPChargeC->AddModelXSkinMesh( pChargeMPA1 );

	m_pMPChargeA->SetAlphaObject( true );
	m_pMPChargeA->SetLayer( XL_EFFECT_0 );

	m_pMPChargeB->SetAlphaObject( true );
	m_pMPChargeB->SetLayer( XL_EFFECT_0 );

	m_pMPChargeC->SetAlphaObject( true );
	m_pMPChargeC->SetLayer( XL_EFFECT_0 );


	m_pMPChargeA->GetRenderParam()->bAlphaBlend		= true;
	//m_pMPChargeA->GetRenderParam()->bZEnable		= false;
	m_pMPChargeA->GetRenderParam()->bZWriteEnable	= false;
	m_pMPChargeA->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMPChargeA->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMPChargeA->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;

	m_pMPChargeB->GetRenderParam()->bAlphaBlend		= true;
	//m_pMPChargeB->GetRenderParam()->bZEnable		= false;
	m_pMPChargeB->GetRenderParam()->bZWriteEnable	= false;
	m_pMPChargeB->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMPChargeB->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMPChargeB->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;

	m_pMPChargeC->GetRenderParam()->bAlphaBlend		= true;
	//m_pMPChargeC->GetRenderParam()->bZEnable		= false;
	m_pMPChargeC->GetRenderParam()->bZWriteEnable	= false;
	m_pMPChargeC->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMPChargeC->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMPChargeC->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;
}


void CX2GULire_ElvenRanger::CommonStateStartFuture()
{
	CX2GUUser::CommonStateStartFuture();

}

void CX2GULire_ElvenRanger::CommonStateStart()
{
#ifdef SERV_TRAPPING_RANGER_TEST
	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME",				m_fDamageDataChangeTime,			0.f );

#ifdef SERV_RENA_NIGHT_WATCHER
	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME1",			m_fDamageDataChangeTime1,			0.f );
#endif SERV_RENA_NIGHT_WATCHER

	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME2",			m_fDamageDataChangeTime2,			0.f );
#endif SERV_TRAPPING_RANGER_TEST

#ifdef ADD_RENA_SYSTEM //��â��
	//��ų�� ���(m_NowStateData.m_SPLevel > 0)���� �ش�� (������ ��Ƽ��� �ش� �ȵ�)
	//�� ��ų�ε� state�� �������� ��� �ش籸���� ������ ���۵Ǵ°��� ���� ���� m_eSaveStateSkillId�� �߰�
	//�����ص� ��ų�� id�� �ٸ��� �ٸ� ��ų�� �����Ѵٰ� �Ǵ�.
	//�ϳ��� ��ų�� ������ ��ų ĵ���� ���� �ٷ� ���������� ���� ���� ��ų�� ����Ǵ� ���� ������� ����.(��Ÿ���� �����ϹǷ�)
	//wait, walk, dash���� �ʱ�ȭ ��. (��ų�� �����Ŀ� �Ѿ �� �ִ� state��� ����)
	//�������� ��� �ٷ� ���������� ����� �����ϹǷ� SiegeReadyStateStart�Լ��� ����ó�� �߰�
	if( m_NowStateData.m_SPLevel > 0 )
	{
		CX2SkillTree::SKILL_ID eNowStateSkillId = GetNowStateSkillID();

		if( eNowStateSkillId != m_eSaveStateSkillId )
		{
			if( IsHyperActiveSkill( eNowStateSkillId ) == false )
			{
				int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( eNowStateSkillId, true );

				float fMPConsume = GetActualMPConsume( eNowStateSkillId, iSkillLevel );
				Byte byteConsumeNFBySkill = static_cast<Byte>( ( abs( fMPConsume - 50.f ) * 0.01f ) + 1 );
				SetNowDamageRelateSkillData( byteConsumeNFBySkill );

				// MP �Ҹ� ���� �ڿ��� ��� ���� ���� ȸ���ϴ� ��� �߰�. 2014.01.28
				if( IsFullNaturalForce() == false )
				{
					//���� �Ҹ��� 0.2�� * ( 5MP�� 1NFP �̹Ƿ� 0.2�� )
					m_fNaturalForceValueCharge += ( fMPConsume * 0.2f * 0.2f );

					//���� ���� �����Ϸᰪ�� �����ϸ� �������� �ϳ� ����.
					int iUpNaturalForce = static_cast<int>( m_fNaturalForceValueCharge / NATURAL_FORCE_VALUE_PER_HIT );
					if( iUpNaturalForce > 0 )
					{
						UpNaturalForce( iUpNaturalForce );
						m_fNaturalForceValueCharge -= static_cast<float>( iUpNaturalForce * NATURAL_FORCE_VALUE_PER_HIT );
					}
				}
			}

			m_eSaveStateSkillId = eNowStateSkillId;		
		}
	}
#endif //ADD_RENA_SYSTEM

	CX2GUUser::CommonStateStart();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LCR_SHARP_KICK, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LCR_SHARP_KICK );

		if( NULL != pSkillTemplet )
		{
			if( m_DamageData.m_bMeleeAttack &&
				CX2DamageManager::AT_SPECIAL != m_DamageData.attackType )
			{
				m_DamageData.fForceDownValue *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_MELEE_REL, iSkillLevel );
			}
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_SHARP_KICK );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_SHARP_KICK, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			if( m_DamageData.m_bMeleeAttack &&
				CX2DamageManager::AT_SPECIAL != m_DamageData.attackType )
			{
				m_DamageData.fForceDownValue *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_MELEE_REL );
			}
		}
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef RENA_SECOND_CLASS_CHANGE

#ifdef	SERV_TRAPPING_RANGER_TEST
	if( IsUseTrapBalde() == true )	//	�� ��� �� �ٿ��ġ ����
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY, true );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

			if( NULL != pSkillTemplet )
			{
				m_DamageData.fForceDownValue *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_REL, iSkillLevel );
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				m_DamageData.fForceDownValue *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_REL );
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
#endif	SERV_TRAPPING_RANGER_TEST
	
#endif RENA_SECOND_CLASS_CHANGE

}

void CX2GULire_ElvenRanger::CommonFrameMoveFuture()
{
	m_fReAttackZ1Time -= m_fElapsedTime;
	if( m_fReAttackZ1Time < 0.0f )
		m_fReAttackZ1Time = 0.0f;

	m_fJumpXTime -= m_fElapsedTime;
	if( m_fJumpXTime < 0.0f )
	{
		m_JumpXCount = 0;
		m_fJumpXTime = 0.0f;
	}


	if( true == IsOnSomethingFuture() &&
		m_iDoubleJumpCount != 0 )
	{
		m_iDoubleJumpCount = 0;
	}

#ifdef ADD_RENA_SYSTEM //��â��
	/// �ڿ��� ��� ����ȭ
	m_FrameDataFuture.syncData.m_CannonBallCount = GetNaturalForceCount();
#endif //ADD_RENA_SYSTEM

	CX2GUUser::CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::CommonFrameMove()
{
	CX2GUUser::CommonFrameMove();

#ifdef RENA_SECOND_CLASS_CHANGE
	//m_WindSneakerData.OnFrameMove( m_fTime, m_fElapsedTime );
#endif RENA_SECOND_CLASS_CHANGE

#ifdef ADD_RENA_SYSTEM //��â��
	/// Ÿ Ŭ���̾�Ʈ�� �ڿ��� ��� ����ȭ
	if ( false == IsMyUnit() )
	{
		SetNaturalForceCount( m_FrameDataNow.syncData.m_CannonBallCount );
	}

	/// �ڿ��� ��� ����Ʈ ����
	UpdateNaturalForceEffect();

#endif //ADD_RENA_SYSTEM

	if( m_pMPChargeA != NULL && m_pMPChargeB != NULL && m_pMPChargeC != NULL )
	{
		if( m_pMPChargeA->GetRenderParam()->color.a > 0.0f )
		{
			if( m_FrameDataNow.syncData.nowState == LESI_CHARGE_MP )
			{
				m_pMPChargeA->GetMatrix().Move( GetMatrix().GetPos() );
				m_pMPChargeA->GetMatrix().Rotate( GetMatrix().GetRotate() );
			}
			else
			{
				m_pMPChargeA->GetRenderParam()->color.a = m_pMPChargeA->GetRenderParam()->color.a - (m_fElapsedTime * 2.0f);
			}
			m_pMPChargeA->OnFrameMove( m_fTime, m_fElapsedTime );
		}
		else
		{
			m_pMPChargeA->Stop();
		}
		if( m_pMPChargeB->GetRenderParam()->color.a > 0.0f )
		{
			if( m_FrameDataNow.syncData.nowState == LESI_CHARGE_MP )
			{
				m_pMPChargeB->GetMatrix().Move( GetMatrix().GetPos() );
				m_pMPChargeB->GetMatrix().Rotate( GetMatrix().GetRotate() );
			}
			else
			{
				m_pMPChargeB->GetRenderParam()->color.a = m_pMPChargeB->GetRenderParam()->color.a - (m_fElapsedTime * 2.0f);
			}
			m_pMPChargeB->OnFrameMove( m_fTime, m_fElapsedTime );
		}
		else
		{
			m_pMPChargeB->Stop();
		}
		if( m_pMPChargeC->GetRenderParam()->color.a )
		{
			if( m_FrameDataNow.syncData.nowState == LESI_CHARGE_MP )
			{
				m_pMPChargeC->GetMatrix().Move( GetMatrix().GetPos() );
				//m_pMPChargeC->GetMatrix().Rotate( GetMatrix().GetRotate() );
			}
			else
			{
				m_pMPChargeC->GetRenderParam()->color.a = m_pMPChargeC->GetRenderParam()->color.a - (m_fElapsedTime * 2.0f);
			}
			m_pMPChargeC->OnFrameMove( m_fTime, m_fElapsedTime );
		}
		else
		{
			m_pMPChargeC->Stop();
		}
	}

#ifdef NEW_SKILL_2010_11
	//{{ oasis907 : ����� [2010.11.3] // ���� ����Ŀ - �ڿ����� ģȭ(�нú�)
	if( NULL != m_pFriendshipNatureData )
	{
		m_pFriendshipNatureData->OnFrameMove( m_fTime, m_fElapsedTime );
	}
	//}}
#endif NEW_SKILL_2010_11

#ifdef	SERV_TRAPPING_RANGER_TEST
	if( m_hTrapBlade != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hTrapBlade );
		if( NULL != pMeshInst )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
		}
		else
		{
			m_hTrapBlade = INVALID_MESH_INSTANCE_HANDLE;
		}
	}
#endif	SERV_TRAPPING_RANGER_TEST

//{{ mauntain : ����ȯ [2012.05.21] ���� 2�� ���� ����Ʈ ���� - ���� �ϰ�, ������ ȭ��, ������ ��ȣź ���� �ð� ����
#ifdef SERV_RENA_NIGHT_WATCHER
	/// ���� �ϰ� ���� �ð� ����
	if( 0.f < m_fPriorPlannedBlowTime )
	{
		m_fPriorPlannedBlowTime -= m_fElapsedTime;
 
		if( 0.f >= m_fPriorPlannedBlowTime )
			EndPriorPlannedBlow();						/// ���� �ϰ� ����
	}
#endif SERV_RENA_NIGHT_WATCHER
//}}

}

void CX2GULire_ElvenRanger::CommonStateEndFuture()
{
	CX2GUUser::CommonStateEndFuture();
}

void CX2GULire_ElvenRanger::CommonStateEnd()
{
	CX2GUUser::CommonStateEnd();
}

//{{ robobeg : 2008-10-28
//void CX2GULire_ElvenRanger::CommonRender()
/*virtual*/
RENDER_HINT CX2GULire_ElvenRanger::CommonRender_Prepare()
{
// 	if( GetRemainHyperModeTime() > 0.0f )
// 	{
// 		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
// 		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_RED;
// 		m_RenderParam.outLineColor		= 0xff11ff11;
// 	}
// 	else
// 	{
// 		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
// 		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
// 		m_RenderParam.outLineColor		= 0xffffffff;
// 	}

//{{ robobeg : 2008-10-28
	//CX2GUUser::CommonRender();
    return CX2GUUser::CommonRender_Prepare();
//}} robobeg : 2008-10-28
}

/*virtual*/ bool CX2GULire_ElvenRanger::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition /*= CX2SkillTree::ASUT_ONLY_ON_LINE*/ )
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif


	KTDXPROFILE();

	const int INVALID_SKILL_SLOT_INDEX = -1;

	int iPressedSkillSlotIndex = INVALID_SKILL_SLOT_INDEX;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;
	CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree; // ������ ��� ��ų Ʈ��

	if ( false == CommonSpecialAttackEventProcess( accessUserSkillTree, pSkillSlotData, iPressedSkillSlotIndex ) )
		return false;

	if( NULL == pSkillSlotData )
		return false;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );
	if( NULL == pSkillTemplet )
		return false;

#ifdef SERV_RENA_NIGHT_WATCHER
	/// ������ ��ȣź ��ü ���� ���� ���߿��� ��� ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if( CX2SkillTree::SI_A_LNW_DELAYED_FIRING == pSkillTemplet->m_eID && true == GetActiveStartOfDelayedFiring() )
#else //UPGRADE_SKILL_SYSTEM_2013
	if( CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING == pSkillTemplet->m_eID && true == GetActiveStartOfDelayedFiring() )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
		if( CX2SkillTree::ASUT_ANYTIME != eActiveSkillUseCondition)
		{
			eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND;
		}
	}
#endif SERV_RENA_NIGHT_WATCHER

#ifdef ADD_MEMO_1ST_CLASS //��â��
	const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseConditionBySkillTemplet = GetSkillUseCondition( pSkillTemplet );

	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, eActiveSkillUseConditionBySkillTemplet ) )
		return false;	

#else //ADD_MEMO_1ST_CLASS
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet ) )
		return false;
#endif //ADD_MEMO_1ST_CLASS

	//��ȯ �ʻ�� ������ ������ ���� ���¿��� ����� �� ����.
	if( false == CheckSummonSpecialAttack( pSkillTemplet->m_eID ) )
		return false;

	// ���Ĵ������� ��� ������ ��ų���� �˻�
	if( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
		return false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

	float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );


#else // UPGRADE_SKILL_SYSTEM_2013
	float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, pSkillTemplet->m_iSkillLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef TOGGLE_UNLIMITED_SKILL_USE
#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ )
	if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_OPERATOR ) || false == g_pMain->IsUnlimitedSkillUse() )
#endif //defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ )
#else //TOGGLE_UNLIMITED_SKILL_USE
#ifndef _SERVICE_
	if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
#endif _SERVICE_
#endif //TOGGLE_UNLIMITED_SKILL_USE
	{
		if( pSkillSlotData->m_fCoolTimeLeft > 0.f )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_226 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}

		if ( GetNowMp() < fMPConsume )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}

#ifdef FINALITY_SKILL_SYSTEM //JHKang
		if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
		{
			const int iItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

			if( iItemNum <= 0 
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
				&& false == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence()
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
				)
			{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_26119 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}
		}
#endif //FINALITY_SKILL_SYSTEM
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	/// ��ųâ���� ��ų �������̸�, ��ų ��� �Ұ�
	if (	NULL != g_pData &&
		NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() &&
		true == g_pData->GetUIManager()->GetUISkillTree()->GetNowLearnSkill() )
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_25110 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	//{{ JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
#ifdef DUNGEON_RANK_NEW
	CountUsedSkill( iPressedSkillSlotIndex );
#endif DUNGEON_RANK_NEW
	//}} JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����

	UpNowMp( -fMPConsume );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
	{
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		if ( true == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence() )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ();
		else
		{
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

		if ( NULL != pItem )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
	}
#endif //FINALITY_SKILL_SYSTEM

#ifdef BALANCE_PATCH_20120329

	#ifdef SERV_RENA_NIGHT_WATCHER

		#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if( pSkillTemplet->m_eID != CX2SkillTree::SI_A_LE_SIEGEMODE && pSkillTemplet->m_eID != CX2SkillTree::SI_A_LNW_DELAYED_FIRING )
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) );
		#else //UPGRADE_SKILL_SYSTEM_2013
		if( pSkillTemplet->m_eID != CX2SkillTree::SI_A_LE_SIEGEMODE && pSkillTemplet->m_eID != CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING )
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
		#endif //UPGRADE_SKILL_SYSTEM_2013
		
	#else  SERV_RENA_NIGHT_WATCHER
	if( pSkillTemplet->m_eID != CX2SkillTree::SI_A_LE_SIEGEMODE )
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
	#endif SERV_RENA_NIGHT_WATCHER

#else
	accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
#endif


#ifdef SERV_SKILL_NOTE
	if( (GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO5 ) == true && pSkillTemplet->m_eID == CX2SkillTree::SI_A_LE_BACKJUMP_SHOT) ||
		(GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO6 ) == true && pSkillTemplet->m_eID == CX2SkillTree::SI_A_LE_REFLEX_MAGIC) )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) - 1.f );

	#else // UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime - 1.f );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO12 ) == true && pSkillTemplet->m_eID == CX2SkillTree::SI_A_LSR_ENTANGLE )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) - 2.f );

	#else // UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime - 2.f );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}


	if( false == SpecialAttackNoStageChange( pSkillTemplet ) )
	{
#ifdef SERV_RENA_NIGHT_WATCHER
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if( true == GetActiveStartOfDelayedFiring() && pSkillTemplet->m_eID == CX2SkillTree::SI_A_LNW_DELAYED_FIRING )		/// ��ü ���� ��� FrameData ���� �������� �߻� ����
#else //UPGRADE_SKILL_SYSTEM_2013
		if( true == GetActiveStartOfDelayedFiring() && pSkillTemplet->m_eID == CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING )		/// ��ü ���� ��� FrameData ���� �������� �߻� ����
#endif //UPGRADE_SKILL_SYSTEM_2013
			return false;
#endif SERV_RENA_NIGHT_WATCHER

		if( GetRemainHyperModeTime() <= 0.f )
		{
			if( iPressedSkillSlotIndex > 3 ) //���� B�϶�
			{
				StateChange( _CONST_GUUSER_::aNormalSkillStateIDSlotB[iPressedSkillSlotIndex-4] );
			}
			else
			{
				StateChange( _CONST_GUUSER_::aNormalSkillStateID[iPressedSkillSlotIndex] );
			}
		}
		else
		{
			if( iPressedSkillSlotIndex > 3 ) //���� B�϶�
			{
				StateChange( _CONST_GUUSER_::aHyperSkillStateIDSlotB[iPressedSkillSlotIndex-4] );
			}
			else
			{
				StateChange( _CONST_GUUSER_::aHyperSkillStateID[iPressedSkillSlotIndex] );
			}
		}
	}

	m_iNowSpecialAttack = iPressedSkillSlotIndex + 1;
	m_bSpecialAttackEventProcessedAtThisFrame = true;


#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

#ifdef SERV_SKILL_USE_SUBQUEST
	Send_SKILL_USE_REQ( pSkillTemplet->m_eID );
#endif SERV_SKILL_USE_SUBQUEST

	return true;

}


// ���º�ȭ�� ���� �ʻ��� �ϵ��ڵ��ؼ� ���� ó�� �Ѵ�
/*virtual*/ bool CX2GULire_ElvenRanger::SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet )
{
	if ( pSkillTemplet == NULL )
		return false;

#ifdef SERV_RENA_NIGHT_WATCHER
	switch ( pSkillTemplet->m_eID )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LNW_DELAYED_FIRING:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			if( true == GetActiveStartOfDelayedFiring() )
			{
				m_FrameDataFuture.syncData.nowAction = UAI_START_OF_DELAYED_FIRING;
				return true;
			}
		} break;
	}
#endif SERV_RENA_NIGHT_WATCHER

	return false;
}




/*virtual*/ void CX2GULire_ElvenRanger::NoStateChangeActionFrameMove()
{

#ifdef RENA_SIEGE_MODE_ANGLE_TEST
	const float MAGIC_SIEGE_FIRE_ANGLE_DELTA = 2.f;

	switch( m_FrameDataNow.syncData.nowAction )
	{
	case UAI_SIEGE_FIRE_ANGLE_UP:
		{
			m_fLookAtIKAngle_Spine += MAGIC_SIEGE_FIRE_ANGLE_DELTA;
		} break;

	case UAI_SIEGE_FIRE_ANGLE_DOWN:
		{
			m_fLookAtIKAngle_Spine -= MAGIC_SIEGE_FIRE_ANGLE_DELTA;
		} break;
	}
#endif RENA_SIEGE_MODE_ANGLE_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	switch ( m_FrameDataNow.syncData.nowAction )
	{
	case UAI_START_OF_DELAYED_FIRING:
		{
#ifdef RIDING_SYSTEM
			if ( true == GetRidingOn() )
				break;
#endif //RIDING_SYSTEM

			if( false == GetActiveStartOfDelayedFiring() )
				return;
			
			EntireExplosion();

			g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiring );	/// ������ ��ȣź ������ Ȯ�� ����Ʈ

			CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			accessUserSkillTree.SetSkillCoolTimeLeft( CX2SkillTree::SI_A_LNW_DELAYED_FIRING, m_fStartOfDelayedFiringCoolTime );
#else //UPGRADE_SKILL_SYSTEM_2013
			accessUserSkillTree.SetSkillCoolTimeLeft( CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING, m_fStartOfDelayedFiringCoolTime );
#endif //UPGRADE_SKILL_SYSTEM_2013

			m_fStartOfDelayedFiringCoolTime = 0.f;

			SetActiveStartOfDelayedFiring( false );		/// ������ ��ȣź ���� ����

			if ( IsMyUnit() )
				g_pX2Game->UpdateSkillSlotUI();				/// ��ų ������ ����			
		} break;
	}
#endif SERV_RENA_NIGHT_WATCHER
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//							������Ʈ �Լ� ����

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//LESI_DIE_FRONT
void CX2GULire_ElvenRanger::DieFrontStartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GULire_ElvenRanger::DieFrontStart()
{
	CommonStateStart();
	DyingStart();

	m_pXSkinAnim->SetPlaySpeed( 0.5f );

#ifdef SERV_RENA_NIGHT_WATCHER
	DeletePriorPlannedBlowEffect();												/// ���� �ϰ� ����Ʈ ����

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiring );	/// ������ ��ȣź ������ Ȯ�� ����Ʈ
	SetActiveStartOfDelayedFiring( false );										/// ������ ��ȣź ���� ����
	
	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();		/// ��ų ������ ����

	ClearStartOfDelayedFiringData();											/// ��� Ȥ�� ��Ż��, �ش� ������ ������ ������ ��ȣź ���� ����
#endif SERV_RENA_NIGHT_WATCHER
}

void CX2GULire_ElvenRanger::DieFrontFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// ���Ⱑ �߻��Ǿ��ϴ� �ִϸ��̼� �ð�
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// ������ ����� ����Ǿ�� �ϴ� �ִϸ��̼� �ð�

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GULire_ElvenRanger::DieFrontEventProcess()
{
	if( EventTimer( 4.0f, true ) == true )
	{
		StateChange( GUSI_DIE );
	}
}


//LESI_DIE_BACK
void CX2GULire_ElvenRanger::DieBackStartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GULire_ElvenRanger::DieBackStart()
{
	CommonStateStart();
	DyingStart();

	m_pXSkinAnim->SetPlaySpeed( 0.5f );

#ifdef SERV_RENA_NIGHT_WATCHER
	DeletePriorPlannedBlowEffect();												/// ���� �ϰ� ����Ʈ ����

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiring );	/// ������ ��ȣź ������ Ȯ�� ����Ʈ
	SetActiveStartOfDelayedFiring( false );										/// ������ ��ȣź ���� ����
	
	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();												/// ��ų ������ ����

	ClearStartOfDelayedFiringData();											/// ��� Ȥ�� ��Ż��, �ش� ������ ������ ������ ��ȣź ���� ����
#endif SERV_RENA_NIGHT_WATCHER
}

void CX2GULire_ElvenRanger::DieBackFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.32f;		// ���Ⱑ �߻��Ǿ��ϴ� �ִϸ��̼� �ð�
	const float TIME_SACRIFICE_OF_HERO = 0.9f;		// ������ ����� ����Ǿ�� �ϴ� �ִϸ��̼� �ð�

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GULire_ElvenRanger::DieBackEventProcess()
{
	if( EventTimer( 4.0f, true ) == true )
	{
		StateChange( GUSI_DIE );
	}
}


#pragma DMLEE_NOTE( "�� �Լ� �� ĳ���� ���� �����ϰ� �����Ǹ� GUUSER�� �Űܼ� �ϳ��� ��������" )

//LESI_WAIT
void CX2GULire_ElvenRanger::WaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
	m_fChangeMPTime = 0.0f;


}

void CX2GULire_ElvenRanger::WaitStart()
{
	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );

#ifdef BALANCE_PATCH_20120329
	m_bIsSiegeSkill = false;	
#endif

#ifdef ADD_RENA_SYSTEM //��â��
	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;

	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	if( GetAccessNowDamageRelateSkillData().m_byteRelateData >= 10 )
		GetAccessNowDamageRelateSkillData().m_byteRelateData = 0;
#endif //ADD_RENA_SYSTEM
}

void CX2GULire_ElvenRanger::WaitFrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_fChangeMPTime += m_fElapsedTime;
#ifdef BALANCE_PATCH_20120329
	m_iSiegeAttackCount = 0;
#endif
}

void CX2GULire_ElvenRanger::WaitEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( LESI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( LESI_COMBO_X1 );
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f, LESI_SIEGE_READY, ShowSiegeFailGuide() )
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
#ifdef SERV_RENA_NIGHT_WATCHER
	else if( m_fChangeMPTime >= 1.0f && GetNowMp() != GetMaxMp() && 
	g_pX2Game->GetSlideShot()->IsPresentNow() == false && false == GetActiveStartOfDelayedFiring() )
#else  SERV_RENA_NIGHT_WATCHER
	else if( m_fChangeMPTime >= 1.0f && GetNowMp() != GetMaxMp() && g_pX2Game->GetSlideShot()->IsPresentNow() == false )
#endif SERV_RENA_NIGHT_WATCHER
	{
#ifdef UNIT_EMOTION
		// �̸�� ���۽ÿ��� charge_mp ��ҽ�Ű�� �̸�� ���۽�Ų��.
		if(m_ePlayedEmotion == CX2Unit::ET_NONE)
#endif
			StateChange( LESI_CHARGE_MP );
	}
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::WalkEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( LESI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( LESI_COMBO_X1 );
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f, LESI_SIEGE_READY, ShowSiegeFailGuide() )
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
	{
		StateChange( USI_WAIT );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}
#ifdef ADD_RENA_SYSTEM //��â��
void CX2GULire_ElvenRanger::WalkStart()
{
	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;

	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	if( GetAccessNowDamageRelateSkillData().m_byteRelateData >= 10 )
		GetAccessNowDamageRelateSkillData().m_byteRelateData = 0;
	CommonStateStart();
}
#endif //ADD_RENA_SYSTEM

//LESI_JUMP_READY
void CX2GULire_ElvenRanger::JumpReadyEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true
		|| m_InputData.pureRight == true
		|| m_InputData.pureLeft == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::JumpUpEventProcess()
{
#ifdef WALL_JUMP_TEST
	if( m_InputData.oneUp == true && m_iDoubleJumpCount == 0 && m_FrameDataFuture.unitCondition.bFootOnWall == false )
#else
	if( m_InputData.oneUp == true && m_iDoubleJumpCount == 0 )
#endif WALL_JUMP_TEST
	{
		StateChange( LESI_DOUBLE_JUMP );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	else if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
#endif //ADD_RENA_SYSTEM
#ifdef NEW_SKILL_TREE
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#endif NEW_SKILL_TREE
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_JUMP_ATTACK_Z );
	}
#ifdef SKILL_BALANCE_20110728
	else if( m_InputData.oneX == true && (m_JumpXCount < 3 || m_fJumpXTime <= 0.0f) )
#else
	else if( m_InputData.oneX == true && FlushMp( 2.0f ) == true  && (m_JumpXCount < 3 || m_fJumpXTime <= 0.0f) )
#endif
	{
		StateChange( LESI_JUMP_ATTACK_X );
	}
	else if( m_PhysicParam.nowSpeed.y <= 0.0f )
	{
		StateChange( USI_JUMP_DOWN );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::JumpDownEventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}


#ifdef WALL_JUMP_TEST
	if( m_FrameDataFuture.unitCondition.bFootOnWall == true && m_iDoubleJumpCount == 0 &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( LESI_WALL_LANDING );
	}
	else if( m_InputData.oneUp == true && m_iDoubleJumpCount == 0 )
#else
	if( m_InputData.oneUp == true && m_iDoubleJumpCount == 0 )
#endif WALL_JUMP_TEST
	{
		StateChange( LESI_DOUBLE_JUMP );
	}
	else if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			if( m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else if( m_InputData.pureRight == true )
			{
				StateChangeDashIfPossible();
			}
			else
			{
				StateChange( USI_JUMP_LANDING );
			}
		}
		else
			StateChange( USI_JUMP_LANDING );
	}
#ifdef NEW_SKILL_TREE
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#endif NEW_SKILL_TREE
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_JUMP_ATTACK_Z );
	}
#ifdef SKILL_BALANCE_20110728
	else if( m_InputData.oneX == true && (m_JumpXCount < 3 || m_fJumpXTime <= 0.0f) )
#else
	else if( m_InputData.oneX == true && FlushMp( 2.0f ) == true  && (m_JumpXCount < 3 || m_fJumpXTime <= 0.0f) )
#endif
	{
		StateChange( LESI_JUMP_ATTACK_X );
	}
	else if( m_PhysicParam.nowSpeed.y >= 0.0f )
	{
		StateChange( USI_JUMP_UP );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
#ifdef ADD_RENA_SYSTEM //��â��
	else if( (m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true ) &&
		GetEnableDash() == true )
	{
		StateChange( USI_DASH_JUMP );
	}
#endif //ADD_RENA_SYSTEM
	CommonEventProcess();
}

void CX2GULire_ElvenRanger::JumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( LESI_COMBO_Z1 );
	}
#ifdef SKILL_BALANCE_20110728
	else if( m_InputData.oneX == true )
#else
	else if( m_InputData.oneX == true && FlushMp( 2.0f ) == true )
#endif
	{
		StateChange( LESI_COMBO_X1 );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#ifdef SERV_RENA_NIGHT_WATCHER
void CX2GULire_ElvenRanger::LESI_DASH_StateStartFuture()
{
	if( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER )
	{
		//�׼� ��� ��� ���� �ʱ�ȭ( �ӽ� )
		m_bAccelDashEventFlag = false;
	}

	DashStartFuture();
}

void CX2GULire_ElvenRanger::LESI_DASH_FrameMoveFuture()
{
	if( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER )
	{
		//0.13�� �̻� ��ø� �� ��, �׼� ��ø� ����� �� �ִ�.( �ӽ� )
		const float iAccelDashTime = 0.0666f;
		if( m_FrameDataNow.unitCondition.fStateTime >= iAccelDashTime )
		{
			m_bAccelDashEventFlag = true;
		}
	}

	DashFrameMoveFuture();
}
#endif SERV_RENA_NIGHT_WATCHER

void CX2GULire_ElvenRanger::DashEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_DASH_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		//switch( GetUnit()->GetClass() )
		//{
		//case CX2Unit::UC_LIRE_WIND_SNEAKER:
		//	{
		//		StateChange( LESI_LWS_DASH_X );
		//	} break;

		//default:
		//	{
				StateChange( LESI_DASH_ATTACK_X );
		//	} break;
		//}
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}
#ifdef ADD_RENA_SYSTEM //��â��
void CX2GULire_ElvenRanger::DashStart()
{
	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;

	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	if( GetAccessNowDamageRelateSkillData().m_byteRelateData >= 10 )
		GetAccessNowDamageRelateSkillData().m_byteRelateData = 0;
	CommonStateStart();
}
#endif //ADD_RENA_SYSTEM

//LESI_DASH_END
void CX2GULire_ElvenRanger::DashEndStartFuture()
{
	CommonStateStartFuture();
}

void CX2GULire_ElvenRanger::DashEndStart()
{
	CommonStateStart();

}

void CX2GULire_ElvenRanger::DashEndFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.3f )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DashEndFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck(0.4f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashEndEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 8.0f )
		{
			StateChange( LESI_COMBO_Z1 );
		}
		else
		{
			StateChange( LESI_DASH_COMBO_Z1 );
		}
	}
	else if( m_InputData.oneX == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 8.0f )
		{
			StateChange( LESI_COMBO_X1 );
		}
		else
		{
			//switch( GetUnit()->GetClass() )
			//{
			//case CX2Unit::UC_LIRE_WIND_SNEAKER:
			//	{
			//		StateChange( LESI_LWS_DASH_X );
			//	} break;

			//default:
			//	{
					StateChange( LESI_DASH_ATTACK_X );
			//	} break;
			//}
		}
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f, LESI_SIEGE_READY, ShowSiegeFailGuide() )
	else if( SpecialAttackEventProcess() == true)
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	//3Ű ���
	else if( m_InputData.oneRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
#ifdef SERV_RENA_NIGHT_WATCHER
		//���� 2�� ���� ����Ʈ ���� - �׼� ���
		if( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER && true == m_bAccelDashEventFlag &&
			m_pXSkinAnimFuture->GetNowAnimationTime() > 0.1f )
		{
			m_bAccelDashEventFlag = false;
			StateChange( LESI_RNW_ACCEL_DASH );
		}
		else
		{
			StateChangeDashIfPossible();
		}
#else  SERV_RENA_NIGHT_WATCHER
		StateChangeDashIfPossible();
#endif SERV_RENA_NIGHT_WATCHER
	}
	else if( m_InputData.oneLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
#ifdef SERV_RENA_NIGHT_WATCHER
		//���� 2�� ���� ����Ʈ ���� - �׼� ���
		if( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER && true == m_bAccelDashEventFlag &&
			m_pXSkinAnimFuture->GetNowAnimationTime() > 0.1f )
		{
			m_bAccelDashEventFlag = false;
			StateChange( LESI_RNW_ACCEL_DASH );
		}
		else
		{
			StateChangeDashIfPossible();
		}
#else  SERV_RENA_NIGHT_WATCHER
		StateChangeDashIfPossible();
#endif SERV_RENA_NIGHT_WATCHER
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}


//LESI_DASH_JUMP
void CX2GULire_ElvenRanger::DashJumpStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * 1.1f;
	m_bDownForce					= false;
}

void CX2GULire_ElvenRanger::DashJumpStart()
{
	CommonStateStart();


	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
	{
		CreateStepDust();
	}
}

void CX2GULire_ElvenRanger::DashJumpFrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight	== true )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.3f;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight	== false )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.3f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DashJumpEventProcess()
{

#ifdef WALL_JUMP_TEST
	if( m_FrameDataFuture.unitCondition.bFootOnWall == true && m_iDoubleJumpCount == 0 &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( LESI_WALL_LANDING );
	}
	else if( m_InputData.oneUp == true )
#else
	if( m_InputData.oneUp == true && m_iDoubleJumpCount == 0 )
#endif WALL_JUMP_TEST
	{
		StateChange( LESI_DASH_DOUBLE_JUMP );
	}
	else if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( USI_DASH_JUMP );
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else
		{
			if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else
				StateChange( USI_DASH_JUMP_LANDING );
			m_bDownForce = false;
		}
	}
#ifdef NEW_SKILL_TREE
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#endif NEW_SKILL_TREE
	else if( m_InputData.oneZ == true )
	{
		switch( GetUnitClass() )
		{
		default: 
			{
				StateChange( LESI_DASH_JUMP_ATTACK_Z );
			} break;

		case CX2Unit::UC_LIRE_GRAND_ARCHER:
			{
				StateChange( LESI_LGA_DASH_JUMP_Z );
			} break;

#ifdef	SERV_TRAPPING_RANGER_TEST
		case CX2Unit::UC_LIRE_TRAPPING_RANGER:
#ifdef SERV_RENA_NIGHT_WATCHER			
		case CX2Unit::UC_LIRE_NIGHT_WATCHER:
#endif SERV_RENA_NIGHT_WATCHER
			{
				StateChange( LESI_DASH_JUMP_ATTACK_RTR_Z );
			} break;
#endif	SERV_TRAPPING_RANGER_TEST
		}
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( LESI_DASH_JUMP_COMBO_X1 );
	}
	else if( m_InputData.pureRight == true )
	{
		if( m_FrameDataFuture.syncData.bIsRight == false )
		{
			if( m_PhysicParam.nowSpeed.y > GetJumpSpeed() )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( m_PhysicParam.nowSpeed.y > 0.0f )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if( m_InputData.pureLeft == true )
	{
		if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_PhysicParam.nowSpeed.y > GetJumpSpeed() )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( m_PhysicParam.nowSpeed.y > 0.0f )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM


	if( m_InputData.oneUp == true && m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y < LINE_RADIUS * 2.0f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}


	if( m_InputData.oneDown == true )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
		m_bDownForce = true;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}


	CommonEventProcess();
}

void CX2GULire_ElvenRanger::DashJumpEndFuture()
{
	CommonStateEnd();
	m_PhysicParam.fDownAccel	= 0.0f;
}

void CX2GULire_ElvenRanger::DashJumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( LESI_COMBO_Z1 );
	}
#ifdef SKILL_BALANCE_20110728
	else if( m_InputData.oneX == true )
#else
	else if( m_InputData.oneX == true && FlushMp( 2.0f ) == true )
#endif
	{
		StateChange( LESI_COMBO_X1 );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}		
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//LESI_DOUBLE_JUMP
void CX2GULire_ElvenRanger::DoubleJumpStartFuture()
{
	CommonStateStartFuture();
	m_iDoubleJumpCount += 1;
}



void CX2GULire_ElvenRanger::DoubleJumpFrameMoveFuture()
{
	if( m_InputData.pureRight == true )
	{
		m_FrameDataFuture.syncData.bIsRight = true;
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true )
	{
		m_FrameDataFuture.syncData.bIsRight = false;
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	//m_FrameDataFuture.syncData.position.y	+= 110.0f * m_pXSkinAnimFuture->GetNowAnimationTime();
	//m_PhysicParam.nowSpeed.x				+= 250.0f * m_pXSkinAnimLocal->GetNowAnimationTime();

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DoubleJumpEventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			if( m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else if( m_InputData.pureRight == true )
			{
				StateChangeDashIfPossible();
			}
			else
			{
				StateChange( USI_JUMP_DOWN );
			}
		}
		else
			StateChange( USI_JUMP_DOWN );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_DOUBLE_JUMP_ATTACK_Z );
		m_pComboGuide->ShowComboCommand(L"j", L"JJZ");
	}
	else if( m_InputData.oneX == true )
	{
		switch( GetUnitClass() )
		{
		case CX2Unit::UC_LIRE_SNIPING_RANGER:
		case CX2Unit::UC_LIRE_GRAND_ARCHER:
			{
				StateChange( LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X );
			} break;

		default:
			{
#ifdef ADD_RENA_SYSTEM //��â��
				StateChange( LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X );
#else //ADD_RENA_SYSTEM
				StateChange( LESI_DOUBLE_JUMP_ATTACK_Z );
#endif //ADD_RENA_SYSTEM
				m_pComboGuide->ShowComboCommand(L"j", L"JJX");
			} break;
		}
	}

	CommonEventProcess();
}

//LESI_DASH_DOUBLE_JUMP
void CX2GULire_ElvenRanger::DashDoubleJumpStartFuture()
{
	CommonStateStartFuture();
	m_iDoubleJumpCount += 1;
}

void CX2GULire_ElvenRanger::DashDoubleJumpFrameMoveFuture()
{
	if( m_InputData.pureRight == true )
	{
		m_FrameDataFuture.syncData.bIsRight = true;
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
	else if( m_InputData.pureLeft == true )
	{
		m_FrameDataFuture.syncData.bIsRight = false;
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	//m_FrameDataFuture.syncData.position.y	+= 120.0f * m_pXSkinAnimFuture->GetNowAnimationTime();
	//m_PhysicParam.nowSpeed.x				+= 250.0f * m_pXSkinAnimFuture->GetNowAnimationTime();

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DashDoubleJumpEventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}


	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			if( m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else if( m_InputData.pureRight == true )
			{
				StateChangeDashIfPossible();
			}
			else
			{
				StateChange( USI_JUMP_DOWN );
			}
		}
		else
			StateChange( USI_JUMP_DOWN );
	}
#ifdef NEW_SKILL_TREE
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#endif NEW_SKILL_TREE
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_DOUBLE_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{

		switch( GetUnitClass() )
		{
		case CX2Unit::UC_LIRE_SNIPING_RANGER:
		case CX2Unit::UC_LIRE_GRAND_ARCHER:
			{
				StateChange( LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X );
			} break;

		default:
			{
#ifdef ADD_RENA_SYSTEM //��â��
				StateChange( LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X );
#else //ADD_RENA_SYSTEM
				StateChange( LESI_DOUBLE_JUMP_ATTACK_Z );
#endif //ADD_RENA_SYSTEM
			} break;
		}
	}

	CommonEventProcess();
}

//LESI_DOUBLE_JUMP_ATTACK_Z
void CX2GULire_ElvenRanger::DoubleJumpAttackZStartFuture()
{
	CommonStateStartFuture();
}

void CX2GULire_ElvenRanger::DoubleJumpAttackZFrameMove()
{


	if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
	{
		m_FrameDataNow.unitCondition.bHit = true;
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DoubleJumpAttackZEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DOUBLE_JUMP_ATTACK_Z_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true && false == IsOnSomethingFuture() )
	{
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS/2.0f;
	}

	CommonEventProcess();
}


//LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP );
		}
		else
		{
			StateChange( LESI_DOUBLE_JUMP_ATTACK_Z_LANDING );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true && false == IsOnSomethingFuture() )
	{
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS/2.0f;
	}

#ifdef ADD_RENA_SYSTEM //��â��
	const float fNowAnimationTime = m_pXSkinAnimFuture->GetNowAnimationTime();
	if( m_InputData.oneZ == true && ( fNowAnimationTime > m_fEventTime[0].keyInputStart && fNowAnimationTime < m_fEventTime[0].keyInputEnd ) )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#else //ADD_RENA_SYSTEM
	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.65f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.185f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.185f ) == true && EventCheck( 0.185f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.y = GetJumpSpeed() / 4.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.18f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.18f ) == true && EventCheck( 0.18f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 degree = GetRotateDegree();
#ifdef ADD_RENA_SYSTEM //��â��
		if( FlushMp( 6.0f ) == true )
#else //ADD_RENA_SYSTEM
		if( FlushMp( 20.0f ) == true )
#endif //ADD_RENA_SYSTEM
		{
			CX2DamageEffect::CEffect* pDE = NULL;
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			degree.z = -90.0f;
#ifdef ADD_RENA_SYSTEM //��â��
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_JUMP_X_ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#else //ADD_RENA_SYSTEM
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif //ADD_RENA_SYSTEM
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
			
			degree.z = -70.0f;
#ifdef ADD_RENA_SYSTEM //��â��
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_JUMP_X_ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#else //ADD_RENA_SYSTEM
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif //ADD_RENA_SYSTEM
			if( NULL != pDE )
			{
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
			}
			
			degree.z = -110.0f;
#ifdef ADD_RENA_SYSTEM //��â��
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_JUMP_X_ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#else //ADD_RENA_SYSTEM
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif //ADD_RENA_SYSTEM
			if( NULL != pDE )
			{
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
			}

		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -90.f );
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, (GetUnitClass() == CX2Unit::UC_LIRE_SNIPING_RANGER || GetUnitClass() == CX2Unit::UC_LIRE_GRAND_ARCHER) && m_InputData.oneX == true, 
		LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX )
	else if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DOUBLE_JUMP_ATTACK_Z_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true && false == IsOnSomethingFuture() )
	{
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS/2.0f;
	}
#else //ADD_RENA_SYSTEM
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DOUBLE_JUMP_ATTACK_Z_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true && false == IsOnSomethingFuture() )
	{
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS/2.0f;
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

#ifdef ADD_RENA_SYSTEM //��â��
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_StateStart()
{
	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	SetNowDamageRelateSkillData( 100 );

	CommonStateStart();
}
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DOUBLE_JUMP_ATTACK_Z_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true && false == IsOnSomethingFuture() )
	{
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS/2.0f;
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.18f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.18f ) == true && EventCheck( 0.18f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 degree = GetRotateDegree();

		if( FlushMp( 6.0f ) == true )
		{
			CX2DamageEffect::CEffect* pDE = NULL;
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			degree.z = -90.0f;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_JUMP_XX_ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

			degree.z = -70.0f;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_JUMP_XX_ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
			}

			degree.z = -110.0f;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_JUMP_XX_ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
			}

		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -90.f );
		}
	}

	CommonFrameMove();
}

#endif //ADD_RENA_SYSTEM

//LESI_DOUBLE_JUMP_ATTACK_Z_LANDING
void CX2GULire_ElvenRanger::DoubleJumpAttackZLandingStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.x /= 1.5f;
}

void CX2GULire_ElvenRanger::DoubleJumpAttackZLandingStart()
{
	CommonStateStart();

	CreateStepDust();
}

void CX2GULire_ElvenRanger::DoubleJumpAttackZLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
#ifdef ADD_RENA_SYSTEM //��â��
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, (GetUnitClass() == CX2Unit::UC_LIRE_COMBAT_RANGER || GetUnitClass() == CX2Unit::UC_LIRE_WIND_SNEAKER) && m_InputData.oneZ == true, 
		LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP )
#endif //ADD_RENA_SYSTEM
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}		
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//LESI_CHARGE_MP
void CX2GULire_ElvenRanger::ChargeMPStartFuture()
{
	CommonStateStartFuture();

	float fAddMPChangeRate = 0.f;
	LUA_GET_VALUE( m_LuaManager, "ADD_MP_CHANGE_RATE", fAddMPChangeRate, 0.f );

	//{{ kimhc // 2010.6.14	// �巡��м�Ʈ ȿ�� ���� ���� ���� 
	// GetOriginalMPChangeRate() ���� �巡��м�Ʈ ȿ���� ���� ���� �������� �Ǵ���
#ifdef	SERV_SECRET_HELL
	ResetMPChangeRate( ( GetOriginalMPChangeRate() + fAddMPChangeRate + m_ChargeMPData.m_fChargeStateMP ) * (1.f + m_SocketData.m_fSpeedUpManaGather) );
#else	SERV_SECRET_HELL
	ResetMPChangeRate( (m_fOriginalMPChangeRate + fAddMPChangeRate + m_ChargeMPData.m_fChargeStateMP ) * (1.f + m_SocketData.m_fSpeedUpManaGather) );
#endif	SERV_SECRET_HELL
	//}} kimhc // 2010.6.14 // �巡��м�Ʈ ȿ�� ���� ���� ����

	if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD )
	{
		SetChangeRateMp( GetChangeRateMp() * 0.8f );
	}
}

void CX2GULire_ElvenRanger::ChargeMPStart()
{
	CommonStateStart();

	m_pMPChargeA->ChangeAnim( L"ChargeSoulA" );
	m_pMPChargeA->SetPlaySpeed( 1.5f );
	m_pMPChargeA->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_pMPChargeB->ChangeAnim( L"ChargeSoulB" );
	m_pMPChargeB->SetPlaySpeed( 1.0f );
	m_pMPChargeB->GetMatrix().Scale( 1.0f, 0.0f, 1.0f );
	//m_pMPChargeB->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_pMPChargeC->ChangeAnim( L"ChargeSoulA_1" );
	m_pMPChargeC->SetPlaySpeed( 1.0f );
	m_pMPChargeC->GetRenderParam()->color.a = 0.0f;
	m_pMPChargeC->GetMatrix().Scale( 1.2f, 1.3f, 1.2f );
	m_pMPChargeC->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_fMPChargeSoundTime = 1.5f;

	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2UpHigh	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2UpHigh );



	if( NULL != pSeqMPCharge2Up )
	{
		pSeqMPCharge2Up->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		pSeqMPCharge2Up->SetEmitRate( CMinMax<float>(10.0f,10.0f) );
	}

	if( NULL != pSeqMPCharge2UpHigh )
	{
		pSeqMPCharge2UpHigh->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		//m_pMPCharge2UpHigh->SetEmitRate( CMinMax<float>(10.0f,10.0f) );
	}
	
}

void CX2GULire_ElvenRanger::ChargeMPFrameMove()
{
	D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
	pos.y += 30.0f;

	m_pMPChargeC->GetRenderParam()->color.a += 0.2f * m_fElapsedTime;
	if( m_pMPChargeC->GetRenderParam()->color.a > 1.0f )
		m_pMPChargeC->GetRenderParam()->color.a = 1.0f;
	m_pMPChargeC->GetMatrix().RotateRel( 0.0f, -D3DXToRadian(300.0f * m_fElapsedTime), 0.0f );


	//if( m_hSeqMPEnergy != NULL && m_pMPEnergyCenter != NULL && m_pMPChargeWave != NULL && m_pMPChargeFeather != NULL )
	//{

	if( m_hSeqMPEnergy != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqMPEnergyCenter != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqMPChargeWave != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqMPChargeFeather != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergy			= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergyCenter 	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergyCenter );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeWave		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeWave );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeFeather	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeFeather );

		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2UpHigh	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2UpHigh );



		if( NULL != pSeqMPEnergy )
		{
			pSeqMPEnergy->SetPosition( pos );
			pSeqMPEnergy->SetBlackHolePosition( pos );
			pSeqMPEnergy->SetLookPoint( pos );
		}

		if( NULL != pSeqMPEnergyCenter )
		{
			pSeqMPEnergyCenter->SetPosition( pos );
		}


		D3DXVECTOR3 pos2 = GetMatrix().GetPos();
		pos2.y += 10.0f;
		if( NULL != pSeqMPChargeWave )
		{
			pSeqMPChargeWave->SetPosition( pos2 );
		}

		if( NULL != pSeqMPChargeFeather )
		{
			pSeqMPChargeFeather->SetPosition( pos2 );
		}



		if( pSeqMPCharge2Up != NULL )
			pSeqMPCharge2Up->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		if( pSeqMPCharge2UpHigh != NULL )
			pSeqMPCharge2UpHigh->SetPosition( m_FrameDataNow.unitCondition.landPosition );
	}

	float alpha = m_pMPChargeA->GetNowAnimationTime() / m_pMPChargeA->GetMaxAnimationTime();
	m_pMPChargeA->GetRenderParam()->color.a = 1.0f - alpha;
	m_pMPChargeA->GetRenderParam()->color.a *= 2.0f;
	if( m_pMPChargeA->GetRenderParam()->color.a > 1.0f )
		m_pMPChargeA->GetRenderParam()->color.a = 1.0f;
	
	if( m_FrameDataNow.unitCondition.fStateTime < 5.0f )
	{
		m_pMPChargeA->GetMatrix().Scale( 1.0f, 1.0f, 1.0f );
		//m_pMPChargeC->GetMatrix().Scale( 1.1f, 1.1f, 1.1f );
	}
	else
	{
		m_pMPChargeA->GetMatrix().Scale( 1.0f, 1.2f, 1.0f );
		//m_pMPChargeC->GetMatrix().Scale( 1.2f, 1.3f, 1.2f );
		//if( GetDistance(GetPos(), g_pX2Game->GetMyUnit()->GetPos() ) < 500.0f )
		//	g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( 2.0f, 0.1f );
	}

	if( EventTimer(5.0f, false) == true )
	{
		m_pMPChargeB->SetPlaySpeed( 1.5f );
		//m_pMPChargeC->SetPlaySpeed( 0.3f );

		if ( IsMyUnit() )
		{
			UpDownCrashCamera( 5.0f, 0.2f );
			g_pX2Game->GetWorld()->SetWorldColor( 0xff333333 );
			g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 2.0f );
		}
		
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergy			= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergyCenter 	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergyCenter );
		//CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeWave		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeWave );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeFeather	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeFeather );

		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2UpHigh	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2UpHigh );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleRenaMinorParticleByEnum( RENA_MINOR_PII_IMPACT_TICK_BLUE ) );


		//m_pMPChargeWave->SetEmitRate( CMinMax<float>(3.0f,5.0f) );
		if( NULL != pSeqMPChargeFeather )
			pSeqMPChargeFeather->SetEmitRate( CMinMax<float>(8.0f,15.0f) );
		if( NULL != pSeqMPCharge2Up )
			pSeqMPCharge2Up->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
		if( NULL != pSeqMPCharge2UpHigh )
			pSeqMPCharge2UpHigh->SetEmitRate( CMinMax<float>(10.0f,10.0f) );

		if( NULL != pSeqMPEnergy )
			pSeqMPEnergy->SetEmitRate( CMinMax<float>(5.0f,15.0f) );
		if( NULL != pSeqMPEnergyCenter )
			pSeqMPEnergyCenter->SetEmitRate( CMinMax<float>(5.0f,15.0f) );

		if( NULL != pSeqImpactTickBlue )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200, 200, 20 );
			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pKTDXApp->GetDGManager()->ClearScreen();
		
		PlaySound( L"Break.ogg" );
		PlaySound( L"Charge.ogg" );
	}

	m_fMPChargeSoundTime -= m_fElapsedTime;
	if( m_fMPChargeSoundTime <= 0.0f )
	{
		if( m_FrameDataNow.unitCondition.fStateTime > 5.0f )
		{
			PlaySound( L"Charge.ogg" );
		}
		PlaySound( L"wind.ogg" );
		m_fMPChargeSoundTime = 1.5f;
	}

	//GetGageManager()->SoulUp();




#ifdef RENA_SECOND_CLASS_CHANGE
	if( m_ChargeMPData.m_fChargeStateMP > 0.f )
	{
		D3DXVECTOR3 vPos = GetPos() + D3DXVECTOR3( 0.f, 55.f + 30.f, 0.f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargePlus );
		if( NULL == pSeq )
		{
			pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"LightParticle_Lire_Plus_ChargeSoul01", vPos );
			if( NULL != pSeq )
			{
				m_hSeqMPChargePlus = pSeq->GetHandle();
			}
		}

		if( NULL != pSeq )
		{
			pSeq->SetPosition( vPos );
		}
	}
#endif RENA_SECOND_CLASS_CHANGE







	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ChargeMPEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( LESI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( LESI_COMBO_X1 );
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f, LESI_SIEGE_READY, ShowSiegeFailGuide() )
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )	
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( GetNowMp() == GetMaxMp() )
	{	
		StateChange( USI_WAIT );
	}
	else if( g_pX2Game->GetSlideShot()->IsPresentNow() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	//else if( m_InputData.pureZ == true || m_InputData.pureX == true ||
	//	m_InputData.pureUp == true || 
	//	m_InputData.pureLeft == true || m_InputData.pureRight == true )
	//{	
	//	StateChange( LESI_WAIT );
	//}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::ChargeMPEndFuture()
{
	CommonStateEndFuture();

	//{{ kimhc // 2010.6.14	// �巡��м�Ʈ ȿ�� ���� ���� ���� 
	// GetOriginalMPChangeRate() ���� �巡��м�Ʈ ȿ���� ���� ���� �������� �Ǵ���
#ifdef	SERV_SECRET_HELL
	ResetMPChangeRate( GetOriginalMPChangeRate() );
#else	SERV_SECRET_HELL
	ResetMPChangeRate( m_fOriginalMPChangeRate );
#endif	SERV_SECRET_HELL
	//}} kimhc // 2010.6.14 // �巡��м�Ʈ ȿ�� ���� ���� ����
}

void CX2GULire_ElvenRanger::ChargeMPEnd()
{
	CommonStateEnd();





#ifdef RENA_SECOND_CLASS_CHANGE
	if( m_ChargeMPData.m_fChargeStateMP > 0.f )
	{
		g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPChargePlus );
	}
#endif RENA_SECOND_CLASS_CHANGE






	CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergy			= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergyCenter 	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergyCenter );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeWave		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeWave );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeFeather	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeFeather );

	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2UpHigh	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2UpHigh );


	if( NULL != pSeqMPEnergy ) 
		pSeqMPEnergy->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( NULL != pSeqMPEnergyCenter ) 
		pSeqMPEnergyCenter->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( NULL != pSeqMPChargeWave ) 
		pSeqMPChargeWave->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( NULL != pSeqMPChargeFeather ) 
		pSeqMPChargeFeather->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( NULL != pSeqMPCharge2Up ) 
		pSeqMPCharge2Up->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( NULL != pSeqMPCharge2UpHigh ) 
		pSeqMPCharge2UpHigh->SetEmitRate( CMinMax<float>(0.0f,0.0f) );

	//m_pSoundChargeWind->Stop();
	g_pKTDXApp->GetDeviceManager()->StopSound( L"wind.ogg" );
	//m_pSoundCharge->Stop();
	g_pKTDXApp->GetDeviceManager()->StopSound( L"Charge.ogg" );

	if( GetNowMp() == GetMaxMp() )
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		pos.y += 30.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleRenaMinorParticleByEnum( RENA_MINOR_PII_IMPACT_TICK_BLUE ) );
		if( NULL != pSeq )
		{
			PlaySequenceByTriggerCount( pSeq, pos, 200, 200, 20 );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}


		g_pKTDXApp->GetDGManager()->ClearScreen();

		PlaySound( L"Break.ogg" );

		if ( IsMyUnit() )
		{
			UpDownCrashCamera( 5.0f, 0.2f );
			g_pX2Game->GetWorld()->SetWorldColor( 0xff333333 );
			g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 2.0f );
		}
	}
}




//LESI_HYPER_MODE
void CX2GULire_ElvenRanger::HyperModeInit()
{
	TextureReadyInBackground( L"ColorBallBlue.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
	TextureReadyInBackground( L"WhiteCircle.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"ToKang.dds" );
	TextureReadyInBackground( L"Energy.dds" );
}

void CX2GULire_ElvenRanger::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove( 0.2f, 0.75f );
#else
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck(0.2f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck(0.75f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 20.0f, 0.3f );
		g_pKTDXApp->GetDGManager()->ClearScreen();
		
		ShowMinorParticleHyperModeTrace();
		
		ApplyHyperModeBuff();
	}

	CommonFrameMove();
#endif // MODIFY_RIDING_PET_AWAKE

}

////LESI_DAMAGE_GROGGY
//void CX2GULire_ElvenRanger::LESI_DAMAGE_GROGGY_FrameMoveFuture()
//{
//	if( m_fGroggyTime > 0.0f )
//	{
//		m_fGroggyTime -= m_fElapsedTime;
//		if( m_fGroggyTime < 0.0f )
//			m_fGroggyTime = 0.0f;
//	}
//	CommonFrameMoveFuture();
//}
//
//void CX2GULire_ElvenRanger::LESI_DAMAGE_GROGGY_EventProcess()
//{
//	if( m_fGroggyTime <= 0.0f )
//	{
//		StateChange( LESI_WAIT );
//	}
//	CommonEventProcess();
//}

//LESI_DAMAGE_SMALL_FRONT
void CX2GULire_ElvenRanger::DamageSmallFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}

//LESI_DAMAGE_SMALL_BACK
void CX2GULire_ElvenRanger::DamageSmallBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//LESI_DAMAGE_BIG_FRONT
void CX2GULire_ElvenRanger::DamageBigFrontStart()
{
	CommonStateStart();

	CreateStepDust();
}

void CX2GULire_ElvenRanger::DamageBigFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//LESI_DAMAGE_BIG_BACK
void CX2GULire_ElvenRanger::DamageBigBackStart()
{
	CommonStateStart();

	CreateStepDust();
}

void CX2GULire_ElvenRanger::DamageBigBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//LESI_DAMAGE_DOWN_FRONT
void CX2GULire_ElvenRanger::DamageDownFrontFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.19f ) == true && EventCheck( 0.19f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		CreateStepDust();
	}

	CommonFrameMove();
}


void CX2GULire_ElvenRanger::DamageDownFrontEventProcess()
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	//����� ����� ����ٸ� ������Ʈ���� ���� �� �ð����� ����
	float fInputStartTime = ( true == m_bAbleAgileMovement ) ? m_fEventTime[0].keyInputStart : 0.66f;
	CX2Unit::UNIT_CLASS eUnitClass = GetUnit()->GetClass();
#endif //UPGRADE_SKILL_SYSTEM_2013
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[4] == true )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( LESI_STANDUP_ATTACK_FRONT ); // fix!!
			}
			else
			{
				StateChange( LESI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( LESI_STANDUP_ATTACK_FRONT );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( LESI_STANDUP_ROLLING_FRONT_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( LESI_STANDUP_ROLLING_FRONT_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	ELSE_IF_STATE_CHANGE_ON_EX_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
		&& true == m_FrameDataFuture.stateParam.bEventFlagList[2] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_P_LE_AGILE_MOVEMENT_FRONT_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
		&& true == m_FrameDataFuture.stateParam.bEventFlagList[3] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_STANDUP_ROLLING_FRONT_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
		&& true == m_FrameDataFuture.stateParam.bEventFlagList[1] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_STANDUP_ATTACK_FRONT )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else //UPGRADE_SKILL_SYSTEM_2013
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5 )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
		if( m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= true;
		}
		else if( m_InputData.oneZ == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
		}
		else if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
		else
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::DamageDownBackFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.32f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.32f ) == true && EventCheck(0.32f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		CreateStepDust();
	}

	CommonFrameMove();
}


void CX2GULire_ElvenRanger::DamageDownBackEventProcess()
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	//����� ����� ����ٸ� ������Ʈ���� ���� �� �ð����� ����
	float fInputStartTime = ( true == m_bAbleAgileMovement ) ? m_fEventTime[0].keyInputStart : 0.66f;
	CX2Unit::UNIT_CLASS eUnitClass = GetUnit()->GetClass();
#endif //UPGRADE_SKILL_SYSTEM_2013
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[4] == true )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( LESI_STANDUP_ATTACK_BACK );	// fix!!
			}
			else
			{
				StateChange( LESI_STANDUP_ATTACK_BACK );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( LESI_STANDUP_ATTACK_BACK );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( LESI_STANDUP_ROLLING_BACK_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( LESI_STANDUP_ROLLING_BACK_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_BACK );
	}
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	ELSE_IF_STATE_CHANGE_ON_EX_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
		&& true == m_FrameDataFuture.stateParam.bEventFlagList[2] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_P_LE_AGILE_MOVEMENT_BACK_FRONT )
		ELSE_IF_STATE_CHANGE_ON_EX_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
		&& true == m_FrameDataFuture.stateParam.bEventFlagList[3] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_STANDUP_ROLLING_BACK_BACK )
		ELSE_IF_STATE_CHANGE_ON_EX_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
		&& true == m_FrameDataFuture.stateParam.bEventFlagList[1] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_STANDUP_ATTACK_BACK )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else //UPGRADE_SKILL_SYSTEM_2013
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5 )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
		if( m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= true;
		}
		else if( m_InputData.oneZ == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
		}
		else if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
		else
		{
			if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
	}

	CommonEventProcess();
}

//LESI_DAMAGE_AIR_DOWN_LANDING
void CX2GULire_ElvenRanger::DamageAirDownLandingInit()
{
	TextureReadyInBackground( L"stone.dds" );
	XSkinMeshReadyInBackground( L"Effect_ElSword_dropdown.X" );
}

void CX2GULire_ElvenRanger::DamageAirDownLandingEventProcess()
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	//����� ����� ����ٸ� ������Ʈ���� ���� �� �ð����� ����
	float fInputStartTime = ( true == m_bAbleAgileMovement ) ? m_fEventTime[0].keyInputStart : 0.66f;
	CX2Unit::UNIT_CLASS eUnitClass = GetUnit()->GetClass();
#endif //UPGRADE_SKILL_SYSTEM_2013

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
			StateChange( LESI_STANDUP_ATTACK_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( LESI_STANDUP_ROLLING_FRONT_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( LESI_STANDUP_ROLLING_FRONT_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// AirDownLanding �� ����� ����� ���� �� �߰�
	ELSE_IF_STATE_CHANGE_ON_EX_( 4, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
	&& true == m_FrameDataFuture.stateParam.bEventFlagList[2] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_P_LE_AGILE_MOVEMENT_FRONT_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
	&& true == m_FrameDataFuture.stateParam.bEventFlagList[3] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_P_LE_AGILE_MOVEMENT_FRONT_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, true == m_bAbleAgileMovement
	&& true == m_FrameDataFuture.stateParam.bEventFlagList[1] && ( CX2Unit::UC_LIRE_COMBAT_RANGER == eUnitClass || CX2Unit::UC_LIRE_WIND_SNEAKER == eUnitClass ), LESI_STANDUP_ATTACK_FRONT )
else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	{
		if( m_InputData.oneZ == true || m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
		}
		else if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
			}
		}
		else
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
			}
		}
	}

	CommonEventProcess();
}

//LESI_STANDUP_ROLLING_FRONT_FRONT
void CX2GULire_ElvenRanger::StandUpRollingFrontFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT, false );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::StandUpRollingFrontFrontEndFuture()
{
	m_FrameDataFuture.syncData.bIsRight = false;

	CommonStateEndFuture();
}

void CX2GULire_ElvenRanger::StandUpRollingFrontFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//LESI_STANDUP_ROLLING_FRONT_BACK
void CX2GULire_ElvenRanger::StandUpRollingFrontBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::StandUpRollingFrontBackEndFuture()
{
	m_FrameDataFuture.syncData.bIsRight = true;

	CommonStateEndFuture();
}

void CX2GULire_ElvenRanger::StandUpRollingFrontBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//LESI_STANDUP_ROLLING_BACK_FRONT
void CX2GULire_ElvenRanger::StandUpRollingBackFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::StandUpRollingBackFrontEndFuture()
{
	m_FrameDataFuture.syncData.bIsRight = false;

	CommonStateEndFuture();
}

void CX2GULire_ElvenRanger::StandUpRollingBackFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//LESI_STANDUP_ROLLING_BACK_BACK
void CX2GULire_ElvenRanger::StandUpRollingBackBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::StandUpRollingBackBackEndFuture()
{
	m_FrameDataFuture.syncData.bIsRight = true;

	CommonStateEndFuture();
}

void CX2GULire_ElvenRanger::StandUpRollingBackBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//LESI_STANDUP_ATTACK_FRONT
void CX2GULire_ElvenRanger::StandUpAttackFrontFrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true || m_pXSkinAnim->EventTimer( 0.8f ) == true )
	{
		//m_pSoundSlash->Set3DPosition( m_FrameDataNow.syncData.position );
		//m_pSoundSlash->Play();
	}

	if( (m_pXSkinAnim->GetNowAnimationTime() > 0.15f && m_pXSkinAnim->GetNowAnimationTime() < 0.5f) )
	{
		g_pData->GetDamageManager()->DamageCheck( &m_DamageData );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::StandUpAttackFrontEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GULire_ElvenRanger::StandUpAttackFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//LESI_STANDUP_ATTACK_BACK
void CX2GULire_ElvenRanger::StandUpAttackBackFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.43f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.43f ) == true && EventCheck( 0.43f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//m_pSoundSlash->Set3DPosition( m_FrameDataNow.syncData.position );
		//m_pSoundSlash->Play();
	}

	if( (m_pXSkinAnim->GetNowAnimationTime() > 0.15f && m_pXSkinAnim->GetNowAnimationTime() < 0.5f) )
	{
		g_pData->GetDamageManager()->DamageCheck( &m_DamageData );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::StandUpAttackBackEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GULire_ElvenRanger::StandUpAttackBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}










//LESI_SIEGE_READY
void CX2GULire_ElvenRanger::SiegeReadyStateStartFuture()
{
	CommonStateStartFuture();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LE_SIEGEMODE );
	
	if( NULL != pSkillTemplet )
	{
		int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LE_SIEGEMODE );

		float fAnimSpeedRate		= 1.f / pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_READY_TIME_REL, iSkillLevel );

	#ifdef	ADD_ANIM_SPEED_FUTURE
		ResetAnimSpeed( m_fAnimSpeedFuture, true, true, fAnimSpeedRate );
	#else	ADD_ANIM_SPEED_FUTURE
		ResetAnimSpeed( m_fAnimSpeed, true, true, fAnimSpeedRate );
	#endif // ADD_ANIM_SPEED_FUTURE
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_SIEGEMODE_UP );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_SIEGEMODE_UP, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			float fAnimSpeedRate = 1.f / pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_READY_TIME_REL );

	#ifdef	ADD_ANIM_SPEED_FUTURE
			ResetAnimSpeed( m_fAnimSpeedFuture, true, true, fAnimSpeedRate );
	#else	ADD_ANIM_SPEED_FUTURE
			ResetAnimSpeed( m_fAnimSpeed, true, true, fAnimSpeedRate );
	#endif // ADD_ANIM_SPEED_FUTURE
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef BALANCE_PATCH_20120329
	m_iSiegeAttackCount = 0;
#endif
}

void CX2GULire_ElvenRanger::SiegeReadyStateStart()
{
#ifdef ADD_RENA_SYSTEM //��â��
	//���� ����� ��쿡 ���� ���� ó��
	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;
#endif //ADD_RENA_SYSTEM

	CommonStateStart();	

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LE_SIEGEMODE );

	if( NULL != pSkillTemplet )
	{
		int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LE_SIEGEMODE );

		float fAnimSpeedRate		= 1.f / pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_READY_TIME_REL, iSkillLevel );
		ResetAnimSpeed( m_fAnimSpeed, true, false, fAnimSpeedRate );
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_SIEGEMODE_UP );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_SIEGEMODE_UP, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			float fAnimSpeedRate = 1.f / pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_READY_TIME_REL );
			ResetAnimSpeed( m_fAnimSpeed, true, false, fAnimSpeedRate );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef RENA_SIEGE_MODE_ANGLE_TEST

	m_fLookAtIKAngle_Spine = 0.f;

#endif RENA_SIEGE_MODE_ANGLE_TEST
	
#ifdef BALANCE_PATCH_20120329
	m_bIsSiegeSkill = true;
#endif 
}



void CX2GULire_ElvenRanger::SiegeReadyEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;

#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
#ifdef BALANCE_GRAND_ARCHER_20121213
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		ELSE_IF_ZX_PRESSED_AFTER( m_fAttackCancelAfter )
	{
		StateChange( LESI_SIEGE_WAIT );
	}
#endif //BALANCE_GRAND_ARCHER_20121213
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_SIEGE_WAIT );
	}

	CommonEventProcess();
}



void CX2GULire_ElvenRanger::SiegeWaitStateStart()
{
	CommonStateStart();

#ifdef RENA_SIEGE_MODE_ANGLE_TEST
	m_bEnableLookAtIK_Spine = true;
#endif RENA_SIEGE_MODE_ANGLE_TEST


}

void CX2GULire_ElvenRanger::SiegeWaitFrameMove()
{
#ifdef BALANCE_PATCH_20120329
	#ifndef BALANCE_GRAND_ARCHER_20121213
	//������� ������ MP �Ҹ� �ǵ��� ����
	if( m_bIsSiegeSkill == true )
	#endif //BALANCE_GRAND_ARCHER_20121213
		UpNowMp(-2.f * m_fElapsedTime);
#endif

	CommonFrameMove();
}

//LESI_SIEGE_WAIT
void CX2GULire_ElvenRanger::SiegeWaitEventProcess()
{
	const float MAGIC_LOOK_AT_IK_ANGLE_MAX = 35.f;


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;

#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
#ifdef BALANCE_PATCH_20120329
	else if( GetNowMp() < 4.f ||  m_iSiegeAttackCount >= 5 )
	{
		StateChange( LESI_SIEGE_END );

#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
#endif
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_SIEGE_ATTACK );
#ifdef BALANCE_PATCH_20120329
		++m_iSiegeAttackCount;
#endif
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( LESI_SIEGE_ATTACK_X );
#ifdef BALANCE_PATCH_20120329
		++m_iSiegeAttackCount;
#endif
	}
#ifdef RENA_SIEGE_MODE_ANGLE_TEST
	else if( m_InputData.pureA == true || m_InputData.pureS == true ||
		m_InputData.pureD == true || m_InputData.pureC == true ||
		m_InputData.pureLeft == true || m_InputData.pureRight == true )
	{
		StateChange( USI_WAIT );
	}
	//else if( m_InputData.pureLeft == true )
	//{
	//	m_FrameDataFuture.syncData.bIsRight = false;
	//}
	//else if( m_InputData.pureRight == true )
	//{
	//	m_FrameDataFuture.syncData.bIsRight = true;
	//}
	else if( m_InputData.pureUp == true &&
		m_fLookAtIKAngle_Spine < MAGIC_LOOK_AT_IK_ANGLE_MAX )
	{
		m_FrameDataFuture.syncData.nowAction = UAI_SIEGE_FIRE_ANGLE_UP;
	}
	else if( m_InputData.pureDown == true &&
		m_fLookAtIKAngle_Spine > -MAGIC_LOOK_AT_IK_ANGLE_MAX )
	{
		m_FrameDataFuture.syncData.nowAction = UAI_SIEGE_FIRE_ANGLE_DOWN;
	}
#else RENA_SIEGE_MODE_ANGLE_TEST

#ifdef BALANCE_PATCH_20120329
	else if( m_InputData.pureLeft == true || m_InputData.pureRight == true ||
		m_InputData.pureUp == true || m_InputData.pureDown == true )
	{	
		SetSiegeModeCoolTime();
		StateChange( USI_WAIT );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
		SetSiegeModeCoolTime();
	} 
#else //BALANCE_PATCH_20120329
	// �ʻ�� Ű�߿��� �ƹ��ų� ������ ������ Ǯ��
	else if( m_InputData.pureA == true || m_InputData.pureS == true ||
		m_InputData.pureD == true || m_InputData.pureC == true ||
		m_InputData.pureLeft == true || m_InputData.pureRight == true ||
		m_InputData.pureUp == true || m_InputData.pureDown == true )
	{
		StateChange( USI_WAIT );
	}
#endif //BALANCE_PATCH_20120329
#endif RENA_SIEGE_MODE_ANGLE_TEST


	CommonEventProcess();
}



//LESI_SIEGE_ATTACK
void CX2GULire_ElvenRanger::SiegeAttackFrontStateStartFuture()
{
	CommonStateStartFuture();

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO7 ) == true )
	{
#ifdef ADD_MEMO_1ST_CLASS //��â��
		ResetAnimSpeed( m_fAnimSpeedFuture, true, true, 1.305f );
#else //ADD_MEMO_1ST_CLASS
	#ifdef	ADD_ANIM_SPEED_FUTURE
			ResetAnimSpeed( m_fAnimSpeedFuture, true, true, 1.105f );
	#else	ADD_ANIM_SPEED_FUTURE
			ResetAnimSpeed( m_fAnimSpeed, true, true, 1.105f );
	#endif // ADD_ANIM_SPEED_FUTURE		
#endif //ADD_MEMO_1ST_CLASS
	}
#endif
}
void CX2GULire_ElvenRanger::SiegeAttackFrontStateStart()
{
	CommonStateStart();

#ifdef RENA_SIEGE_MODE_ANGLE_TEST
	m_bEnableLookAtIK_Spine = true;
#endif RENA_SIEGE_MODE_ANGLE_TEST

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO7 ) == true )
	{
#ifdef ADD_MEMO_1ST_CLASS //��â��
		ResetAnimSpeed( m_fAnimSpeedFuture, true, true, 1.305f );
#else //ADD_MEMO_1ST_CLASS
		ResetAnimSpeed( m_fAnimSpeed, true, false, 1.105f );
#endif //ADD_MEMO_1ST_CLASS
	}
#endif
}

void CX2GULire_ElvenRanger::SiegeAttackFrontFrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.07f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.07f ) == true && EventCheck( 0.07f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{
#ifdef BALANCE_PATCH_20120329
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ
		if( FlushMp( 20.f * m_fMPConsumeRateSiegeArrow ) == true )
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( FlushMp( 21.4f * m_fMPConsumeRateSiegeArrow ) == true )
	#endif // UPGRADE_SKILL_SYSTEM_2013
#else
		if( FlushMp( 6.f * m_fMPConsumeRateSiegeArrow ) == true )
#endif
		{
			float fPowerRate = GetPowerRate();
			D3DXVECTOR3 vRotateDegree = GetRotateDegree();
#ifdef RENA_SIEGE_MODE_ANGLE_TEST
			vRotateDegree.z += m_fLookAtIKAngle_Spine;
#endif RENA_SIEGE_MODE_ANGLE_TEST

#ifndef UPGRADE_SKILL_SYSTEM_2013 //��â�� //�ڵ�󿡼� ���� ��ũ��Ʈ���� ������.
#ifdef BALANCE_PATCH_20120329
			fPowerRate = 1.54f;
#endif
#endif //UPGRADE_SKILL_SYSTEM_2013

			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SIEGE_ARROW", fPowerRate *  m_fDamageRelSiegeArrow, 
				posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

#ifdef BALANCE_PATCH_20120329			
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SIEGE_ARROW", fPowerRate * m_fDamageRelSiegeArrow, 
				posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y, true, 0.12f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
#endif
		}
		else
		{
#ifdef BALANCE_PATCH_20120329
	#ifdef BALANCE_GRAND_ARCHER_20121213
			if( FlushMp( 3.f ) == true )
	#else
			if( FlushMp( 4.f ) == true )
	#endif //BALANCE_GRAND_ARCHER_20121213
			{
				D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
				CX2DamageEffect::CEffect* pDE = NULL;

				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", 1.54f * m_fDamageRelSiegeArrow, 
					posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}
			}			
#else
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );


#ifdef RENA_SIEGE_MODE_ANGLE_TEST
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, m_fLookAtIKAngle_Spine );
#else RENA_SIEGE_MODE_ANGLE_TEST
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
#endif RENA_SIEGE_MODE_ANGLE_TEST
#endif //BALANCE_PATCH_20120329

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::SiegeAttackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;

#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef BALANCE_PATCH_20120329

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ
		if( GetNowMp() < 20.f * m_fMPConsumeRateSiegeArrow )
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( GetNowMp() < 21.4f * m_fMPConsumeRateSiegeArrow )
	#endif // UPGRADE_SKILL_SYSTEM_2013
		{
			StateChange( LESI_SIEGE_END );

			SetSiegeModeCoolTime();
		}
		else
#endif
		{
			StateChange( LESI_SIEGE_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SIEGE_ATTACK_X
void CX2GULire_ElvenRanger::SiegeAttackXFrontStateStartFuture()
{
	CommonStateStartFuture();


#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO7 ) == true )
	{
#ifdef ADD_MEMO_1ST_CLASS //��â��
		ResetAnimSpeed( m_fAnimSpeedFuture, true, true, 1.305f );
#else //ADD_MEMO_1ST_CLASS
	#ifdef	ADD_ANIM_SPEED_FUTURE
			ResetAnimSpeed( m_fAnimSpeedFuture, true, true, 1.105f );
	#else	ADD_ANIM_SPEED_FUTURE
			ResetAnimSpeed( m_fAnimSpeed, true, true, 1.105f );
	#endif // ADD_ANIM_SPEED_FUTURE
#endif //ADD_MEMO_1ST_CLASS
	}
#endif
}

void CX2GULire_ElvenRanger::SiegeAttackXFrontStateStart()
{
	CommonStateStart();

#ifdef RENA_SIEGE_MODE_ANGLE_TEST
	m_bEnableLookAtIK_Spine = true;
#endif RENA_SIEGE_MODE_ANGLE_TEST

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO7 ) == true )
	{
#ifdef ADD_MEMO_1ST_CLASS //��â��
		ResetAnimSpeed( m_fAnimSpeedFuture, true, true, 1.305f );
#else //ADD_MEMO_1ST_CLASS
		ResetAnimSpeed( m_fAnimSpeed, true, false, 1.105f );
#endif //ADD_MEMO_1ST_CLASS
	}
#endif
}


void CX2GULire_ElvenRanger::SiegeAttackXFrontFrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bAimingTarget = false;
		D3DXVECTOR3 vAimingTargetPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 vAimingTargetLandPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		float fMinCoefficient = -1.f * tan( D3DX_PI * 30.f / 180.f );
		float fMaxCoefficient = tan( D3DX_PI * 60.f / 180.f);
		D3DXVECTOR3 vCoefficient = D3DXVECTOR3(fMaxCoefficient, fMinCoefficient, D3DX_PI * 45.f / 180.f);
		float fDegree = ( DoAimingTarget( 1000.f, vCoefficient, bAimingTarget, vAimingTargetPos, vAimingTargetLandPos ) * 180.f ) / D3DX_PI;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ
		if( FlushMp( 27.f * m_fMPConsumeRateSiegeArrow ) == true )
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( FlushMp( 28.57f * m_fMPConsumeRateSiegeArrow ) == true )
	#endif // UPGRADE_SKILL_SYSTEM_2013
		{
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			float fDist = 0.f;
			if( bAimingTarget == true )
			{
				D3DXVECTOR3 vDist = vAimingTargetPos - GetPos();
				fDist = D3DXVec3Length( &vDist );
			}

			D3DXVECTOR3 vTargetPosTemp = GetDirVector();			

			if( bAimingTarget == false || fDist <= 400.f )
			{				
				if( GetIsRight() == false )
					vTargetPosTemp = vTargetPosTemp * -1.f;

				vAimingTargetPos = GetPos() + vTargetPosTemp * 700.f;
				vAimingTargetPos.y = vAimingTargetPos.y + 300.f;
			}

			for(int i=0; i<5; ++i)
			{
				D3DXVECTOR3 vTargetPos = vAimingTargetPos;

				vTargetPos = vAimingTargetPos + (vTargetPosTemp * (GetRandomInt(i) % 100 * 4.f - 200.f));

				float FlightDistance = GetDistance( GetPos(), vTargetPos );
				float FlightTime = FlightDistance / 3000.f;
				
				float fRandT = (float)(GetRandomInt(i+1) % 100 % 5);
				fRandT = fRandT / 10.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
				pDE = g_pX2Game->GetDamageEffect()->CreateInstanceParabolic( this, L"ARROW_SIEGE_X", GetPowerRate() * m_fDamageRelSiegeArrow, posR, vTargetPos, D3DXVECTOR3(0.f, -1100.f, 0.f), FlightTime + fRandT, 10.f, m_iArrowMeshPlayerIndex );
#else
				pDE = g_pX2Game->GetDamageEffect()->CreateInstanceParabolic( this, L"ARROW_SIEGE_X", 1.154f * m_fDamageRelSiegeArrow, posR, vTargetPos, D3DXVECTOR3(0.f, -1100.f, 0.f), FlightTime + fRandT, 10.f, m_iArrowMeshPlayerIndex );
#endif //UPGRADE_SKILL_SYSTEM_2013
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}
			}			
		}
		else
		{
	#ifdef BALANCE_GRAND_ARCHER_20121213
			if( FlushMp( 3.f ) == true )
	#else
			if( FlushMp( 4.f ) == true )
	#endif //BALANCE_GRAND_ARCHER_20121213
			{
				D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
				CX2DamageEffect::CEffect* pDE = NULL;

				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", 1.154f * m_fDamageRelSiegeArrow, 
					posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}
			}
		}
	}
#else //BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.07f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.07f ) == true && EventCheck( 0.07f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		if( FlushMp( 18.f * m_fMPConsumeRateSiegeArrow ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();


#ifdef RENA_SIEGE_MODE_ANGLE_TEST
			degree.z += m_fLookAtIKAngle_Spine;
#endif RENA_SIEGE_MODE_ANGLE_TEST


			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SIEGE_ARROW", GetPowerRate() * m_fAttackPowerRateArrow * m_fDamageRelSiegeArrow,
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

			degree.z += 25.0f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SIEGE_ARROW", GetPowerRate() * m_fAttackPowerRateArrow * m_fDamageRelSiegeArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex  );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}


			degree.z -= 50.0f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SIEGE_ARROW", GetPowerRate() * m_fAttackPowerRateArrow * m_fDamageRelSiegeArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );



#ifdef RENA_SIEGE_MODE_ANGLE_TEST
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, m_fLookAtIKAngle_Spine );
#else RENA_SIEGE_MODE_ANGLE_TEST
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
#endif RENA_SIEGE_MODE_ANGLE_TEST

		}
	}
#endif //BALANCE_PATCH_20120329
	
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::SiegeAttackXEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef BALANCE_PATCH_20120329
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ
		if( GetNowMp() < 27.f * m_fMPConsumeRateSiegeArrow )
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( GetNowMp() < 28.57f * m_fMPConsumeRateSiegeArrow )
	#endif // UPGRADE_SKILL_SYSTEM_2013
		{
			StateChange( LESI_SIEGE_END );
#ifdef BALANCE_PATCH_20120329
			SetSiegeModeCoolTime();
#endif
		}
		else
#endif
		{
			StateChange( LESI_SIEGE_WAIT );
		}
	}

	CommonEventProcess();
}



// note!! ���� siegemode Ǫ�� ������ ������ �ʰ� �ֽ��ϴ�
//LESI_SIEGE_END
void CX2GULire_ElvenRanger::SiegeEndEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
#ifdef BALANCE_PATCH_20120329
		SetSiegeModeCoolTime();
#endif
	}
#ifdef BALANCE_GRAND_ARCHER_20121213
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		ELSE_IF_ZX_PRESSED_AFTER( m_fAttackCancelAfter )
	{
		StateChange( USI_WAIT );
	}
#endif //BALANCE_GRAND_ARCHER_20121213


	CommonEventProcess();
}



//LESI_COMBO_Z1
void CX2GULire_ElvenRanger::ComboZ1FrameMoveFuture()
{
	m_PhysicParam.nowSpeed.x = 0.0f;
	m_PhysicParam.nowSpeed.y = 0.0f;

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboZ1FrameMove()
{

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.1f && m_pXSkinAnim->GetNowAnimationTime() < 0.24f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboZ1EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		m_fCanNotAttackTime = 0.1f;
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER && m_InputData.oneX == true, LESI_COMBO_RNW_ZZX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_COMBO_Z2 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",	fSkipTime,		0.6f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime,	0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_Z2 );
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef SERV_RENA_NIGHT_WATCHER
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.4, 0.4, GetUnit()->GetClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER && m_InputData.oneX == true, LESI_COMBO_RNW_ZZX )
#endif SERV_RENA_NIGHT_WATCHER
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			m_fCanNotAttackTime = 0.15f;
			StateChange( USI_WAIT );
		}
	}


	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.1f;
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_Z2
void CX2GULire_ElvenRanger::ComboZ2FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.3f ) == true && EventCheck( 0.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboZ2FrameMove()
{

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.31f && m_pXSkinAnim->GetNowAnimationTime() < 0.4f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;		
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboZ2EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, GetUnitClass() == CX2Unit::UC_LIRE_WIND_SNEAKER && m_InputData.oneZ == true && 
		( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ||
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) ), LESI_LWS_ZZfrontZ )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, ( GetUnitClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER || 
		GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER ) && m_InputData.oneZ == true, LESI_COMBO_RTR_ZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true, LESI_COMBO_Z3 )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 3, 3, m_InputData.oneX == true && GetUnitClass() == CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_LGA_ZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime,		0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",		fChangeTime,	0.0f );

	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
#ifdef RENA_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_( 0, fChangeTime, fChangeTime, GetUnit()->GetClass() == CX2Unit::UC_LIRE_WIND_SNEAKER && m_InputData.oneZ == true && 
		( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ||
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) ), LESI_LWS_ZZfrontZ )
#endif RENA_SECOND_CLASS_CHANGE

#ifdef	SERV_TRAPPING_RANGER_TEST
 #ifdef	SERV_RENA_NIGHT_WATCHER
 		ELSE_IF_STATE_CHANGE_ON_( 2, fChangeTime, fChangeTime, ( GetUnit()->GetClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER || 
 													GetUnit()->GetClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER ) && m_InputData.oneZ == true, LESI_COMBO_RTR_ZZZ )
#else 	SERV_RENA_NIGHT_WATCHER
		ELSE_IF_STATE_CHANGE_ON_( 2, fChangeTime, fChangeTime, GetUnit()->GetClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER && m_InputData.oneZ == true, LESI_COMBO_RTR_ZZZ )
#endif	SERV_RENA_NIGHT_WATCHER
#endif	SERV_TRAPPING_RANGER_TEST

	ELSE_IF_STATE_CHANGE_ON_( 1, fChangeTime, fChangeTime, m_InputData.oneZ == true, LESI_COMBO_Z3 )
#ifdef BALANCE_GRAND_ARCHER_20121213
	ELSE_IF_STATE_CHANGE_ON_( 3, fChangeTime, fChangeTime, m_InputData.oneX == true && GetUnit()->GetClass() == CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_LGA_ZZX )
#endif //BALANCE_GRAND_ARCHER_20121213
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( fSkipTime )
	{
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_Z3
void CX2GULire_ElvenRanger::ComboZ3FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.125f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.125f ) == true && EventCheck( 0.125f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboZ3FrameMove()
{

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.125f && m_pXSkinAnim->GetNowAnimationTime() < 0.21f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;		
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboZ3EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 4 )

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, m_InputData.pureUp == true && m_InputData.oneZ == true, LESI_COMBO_Z4UP )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_COMBO_Z4 )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneX == true && GetUnitClass() != CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_COMBO_Z4a )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 3, 3, m_InputData.oneX == true && GetUnitClass() == CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_LGA_ZZX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	m_bReAttackZ1Right = m_FrameDataFuture.syncData.bIsRight;
	m_fReAttackZ1Time = 0.4f;

	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );



	CANCEL_COMBO_ON_DIRECTION_CHANGE( 4 )


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.1f;
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		m_fCanNotAttackTime = 0.2f;
		StateChange( LESI_COMBO_Z4UP );
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneZ == true && m_InputData.pureUp == true 
		&& m_pXSkinAnimFuture->GetNowAnimationTime() <= fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[3] = false;
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, fChangeTime, fChangeTime, m_InputData.oneZ == true, LESI_COMBO_Z4 )
	ELSE_IF_STATE_CHANGE_ON_( 1, fChangeTime, fChangeTime, m_InputData.oneX == true && GetUnit()->GetClass() != CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_COMBO_Z4a )
	#ifdef RENA_SECOND_CLASS_CHANGE
		#ifdef BALANCE_GRAND_ARCHER_20121213
		ELSE_IF_STATE_CHANGE_ON_( 3, fChangeTime, fChangeTime, m_InputData.oneX == true && GetUnit()->GetClass() == CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_LGA_ZZX )
		#else
		ELSE_IF_STATE_CHANGE_ON_( 3, fChangeTime, fChangeTime, m_InputData.oneX == true && GetUnit()->GetClass() == CX2Unit::UC_LIRE_GRAND_ARCHER, LESI_LGA_ZZZX )
		#endif //BALANCE_GRAND_ARCHER_20121213
	#endif RENA_SECOND_CLASS_CHANGE
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( fSkipTime )
	{
		StateChange( USI_WAIT );
	}
#ifdef BALANCE_CODE_NEMESIS_20121213
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //BALANCE_CODE_NEMESIS_20121213

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_Z4
void CX2GULire_ElvenRanger::ComboZ4FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.28f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.28f ) == true && EventCheck( 0.28f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboZ4FrameMove()
{

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.28f && m_pXSkinAnim->GetNowAnimationTime() < 0.37f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboZ4EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );
	

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_Z4a
//void CX2GULire_ElvenRanger::ComboZ4aFrameMoveFuture()
//{
//	CommonFrameMoveFuture();
//}

void CX2GULire_ElvenRanger::ComboZ4aFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.y += 180;
			CX2DamageEffect::CEffect* pDE = NULL;

#ifdef SERV_RENA_NIGHT_WATCHER	//���� 2�� ���� ����Ʈ ���� - ������ ȭ��
			if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnitClass() )
			{
				if( true == GetActiveStartOfDelayedFiring() )		//������ ��ȣź�� ������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION_DELAY", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY );	//������ ȭ�� ����
						pDE->SetIndex( CreateDamageEffectIndex() );
					}
				}
				else												//������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION );	//������ ȭ�� ����
					}
				}
			}
			else
			{
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG_P", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

#ifdef ADD_RENA_SYSTEM //��â��
				if( NULL != pDE )
					pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
#else  SERV_RENA_NIGHT_WATCHER
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG_P", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif SERV_RENA_NIGHT_WATCHER	

			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboZ4aEventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//void CX2GULire_ElvenRanger::ComboZ4aEndFuture()
//{
//	m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
//
//	CommonStateEndFuture();
//}



//LESI_COMBO_Z4UP
void CX2GULire_ElvenRanger::ComboZ4UpFrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboZ4UpFrameMove()
{

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.24f && m_pXSkinAnim->GetNowAnimationTime() < 0.35f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboZ4UpEventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X1
void CX2GULire_ElvenRanger::ComboX1FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboX1FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.24f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			//SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( (CKTDGObject*) this,  this, L"ARROW_GV", 0.7f, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );

			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboX1EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_COMBO_X2 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

#ifdef BALANCE_PATCH_20120329
	if( GetUnit()->GetClass() == CX2Unit::UC_LIRE_SNIPING_RANGER || GetUnit()->GetClass() == CX2Unit::UC_LIRE_GRAND_ARCHER )
	{
		fChangeTime = 0.3f;
	}
	else
	{
		LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );
	}
#else
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );
#endif
	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= fChangeTime )
	{
		StateChange( LESI_COMBO_X2 );
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X2
void CX2GULire_ElvenRanger::ComboX2FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboX2FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.28f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.28f ) == true && EventCheck( 0.28f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboX2EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_COMBO_X3 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true	&& m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X3 );
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X3
void CX2GULire_ElvenRanger::ComboX3FrameMoveFuture()
{
#ifdef BALANCE_PATCH_20120329
	if(m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.5f )
#else
	if(m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.6f )
#endif
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboX3FrameMove()
{
	float fFlushMp = 6.f;

#ifdef BALANCE_PATCH_20120329
	#ifdef BALANCE_GRAND_ARCHER_20121213
		fFlushMp = 1.5f;
	#else
		fFlushMp = 4.f;
	#endif //BALANCE_GRAND_ARCHER_20121213
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.466f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.466f ) == true && EventCheck( 0.466f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.44f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.44f ) == true && EventCheck( 0.44f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{
		if( FlushMp( fFlushMp ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

#ifndef BALANCE_PATCH_20120329
			degree.z = -23;
#endif

			CX2DamageEffect::CEffect* pDE = NULL;

#ifdef SERV_RENA_NIGHT_WATCHER	//���� 2�� ���� ����Ʈ ���� - ������ ȭ��
			if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnitClass() )
			{
				if( true == GetActiveStartOfDelayedFiring() )		//������ ��ȣź�� ������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION_DELAY", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY );	//������ ȭ�� ����
						pDE->SetIndex( CreateDamageEffectIndex() );
					}
				}
				else												//������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION );	//������ ȭ�� ����
					}
				}
			}
			else
			{
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X3", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			}
#else  SERV_RENA_NIGHT_WATCHER
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X3", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif SERV_RENA_NIGHT_WATCHER

			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( fFlushMp ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;


#ifdef SERV_RENA_NIGHT_WATCHER	//���� 2�� ���� ����Ʈ ���� - ������ ȭ��
			if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnitClass() )
			{
				if( true == GetActiveStartOfDelayedFiring() )		//������ ��ȣź�� ������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION_DELAY", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY );	//������ ȭ�� ����
						pDE->SetIndex( CreateDamageEffectIndex() );
					}
				}
				else												//������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION );	//������ ȭ�� ����
					}
				}
			}
			else
			{
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X3", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			}
#else  SERV_RENA_NIGHT_WATCHER
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X3", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif SERV_RENA_NIGHT_WATCHER
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
		
	}
#endif

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboX3EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false; //X4
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false; //X4up
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false; //X4down
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, m_InputData.pureDown && m_InputData.oneX == true, LESI_COMBO_X4DOWN )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.pureUp && m_InputData.oneX == true, LESI_COMBO_X4UP )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_COMBO_X4 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false; //X4
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false; //X4up
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false; //X4down
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X4 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X4UP );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X4DOWN );
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
#ifdef BALANCE_PATCH_20120329
	else if( m_InputData.oneX == true && m_InputData.pureDown && 
		m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.4666f && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
#else
	else if( m_InputData.oneX == true && m_InputData.pureDown && m_pXSkinAnimFuture->GetNowAnimationTime() <= fChangeTime )
#endif
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
	}
#ifdef BALANCE_PATCH_20120329
	else if( m_InputData.oneX == true && m_InputData.pureUp && 
		m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.4666f && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
#else
	else if( m_InputData.oneX == true && m_InputData.pureUp && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
#endif
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneX == true && 
		m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.4666f && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X4
void CX2GULire_ElvenRanger::ComboX4FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboX4FrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.366f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.366f ) == true && EventCheck( 0.366f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{
	#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
	#else
		if( FlushMp( 4.f ) == true )
	#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );

			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboX4EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X4UP
void CX2GULire_ElvenRanger::ComboX4UpFrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboX4UpFrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{
		if( FlushMp( 15.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE1_Z", degree.z, 0.0f );

			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE3_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboX4UpEventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X4DOWN
void CX2GULire_ElvenRanger::ComboX4DownFrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::ComboX4DownFrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{

#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 12.f ) == true )
#else
		if( FlushMp( 15.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE1_Z", degree.z, 0.0f );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE3_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::ComboX4DownEventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_DASH_COMBO_Z1
void CX2GULire_ElvenRanger::DashComboZ1StartFuture()
{
	CommonStateStartFuture();
	if( m_FrameDataFuture.unitCondition.bHit == true )
	{
		if( m_PhysicParam.nowSpeed.x > GetDashSpeed() )
			m_PhysicParam.nowSpeed.x = GetDashSpeed() / 2.0f;
	}
}

void CX2GULire_ElvenRanger::DashComboZ1FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashComboZ1EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
			m_fCanNotAttackTime = 0.2f;
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, ( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER || GetUnitClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER ) 
		&& m_InputData.oneX == true, LESI_DASH_COMBO_RTR_ZX )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_DASH_COMBO_Z2 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
			m_fCanNotAttackTime = 0.2f;
		}
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, fChangeTime, fChangeTime, m_InputData.oneZ == true, LESI_DASH_COMBO_Z2 )

#ifdef SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RENA_NIGHT_WATCHER
		ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		( GetUnit()->GetClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER || GetUnit()->GetClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER ) 
		&& m_InputData.oneX == true, LESI_DASH_COMBO_RTR_ZX )
#else  SERV_RENA_NIGHT_WATCHER
		ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetUnit()->GetClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER && m_InputData.oneX == true, LESI_DASH_COMBO_RTR_ZX )
#endif SERV_RENA_NIGHT_WATCHER
#endif SERV_TRAPPING_RANGER_TEST

	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
				m_fCanNotAttackTime = 0.2f;
				StateChange( USI_WAIT );
			}
		}
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_COMBAT_RANGER_DASH_COMBO_Z1
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z1_StartFuture()
{
	CommonStateStartFuture();
	if( m_FrameDataFuture.unitCondition.bHit == true )
	{
		if( m_PhysicParam.nowSpeed.x > GetDashSpeed() )
			m_PhysicParam.nowSpeed.x = GetDashSpeed() / 2.0f;
	}
}

void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z1_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
			m_fCanNotAttackTime = 0.2f;
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_COMBAT_RANGER_DASH_COMBO_Z2 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBAT_RANGER_DASH_COMBO_Z2 );
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
				m_fCanNotAttackTime = 0.2f;
				StateChange( USI_WAIT );
			}
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
			m_fCanNotAttackTime = 0.2f;
		}
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_COMBAT_RANGER_DASH_COMBO_Z2
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z2_StartFuture()
{
	CommonStateStartFuture();
	if( m_FrameDataFuture.unitCondition.bHit == true )
	{
		if( m_PhysicParam.nowSpeed.x > GetDashSpeed() )
			m_PhysicParam.nowSpeed.x = GetDashSpeed() / 2.0f;
	}
}

void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z2_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
			m_fCanNotAttackTime = 0.2f;
		}
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_DASH_COMBO_Z2 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_DASH_COMBO_Z2 );
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
				m_fCanNotAttackTime = 0.2f;
				StateChange( USI_WAIT );
			}
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
			m_fCanNotAttackTime = 0.2f;
		}
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_DASH_COMBO_Z2
void CX2GULire_ElvenRanger::DashComboZ2StartFuture()
{
	CommonStateStartFuture();
}

void CX2GULire_ElvenRanger::DashComboZ2FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashComboZ2EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_DASH_ATTACK_X
void CX2GULire_ElvenRanger::DashAttackXStartFuture()
{
	CommonStateStartFuture();
	//m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
}

void CX2GULire_ElvenRanger::DashAttackXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 6.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			posR.y -= 20.0f;
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.y += 180;

			CX2DamageEffect::CEffect* pDE = NULL;

#ifdef SERV_RENA_NIGHT_WATCHER	//���� 2�� ���� ����Ʈ ���� - ������ ȭ��
			if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnitClass() )
			{
				if( true == GetActiveStartOfDelayedFiring() )		//������ ��ȣź�� ������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION_DELAY", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY );	//������ ȭ�� ����
						pDE->SetIndex( CreateDamageEffectIndex() );
					}
				}
				else												//������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION );	//������ ȭ�� ����
					}
				}
			}
			else
			{
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			}
#else  SERV_RENA_NIGHT_WATCHER
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif SERV_RENA_NIGHT_WATCHER	

			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashAttackXEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::DashAttackXEndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight				= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y		+= 180.0f;
}


//LESI_SNIPING_RANGER_DASH_COMBO_X1
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X1_StartFuture()
{
	CommonStateStartFuture();
	//m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X1_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			posR.y -= 20.0f;
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.y += 180;

			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X1_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_SNIPING_RANGER_DASH_COMBO_X2 )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true && GetUnitClass() == CX2Unit::UC_LIRE_WIND_SNEAKER, LESI_LWS_DASH_XZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fChangeTime;
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",		fChangeTime,	0.0f );

	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, fChangeTime, fChangeTime, m_InputData.oneX == true, LESI_SNIPING_RANGER_DASH_COMBO_X2 )
#ifdef RENA_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_( 1, fChangeTime, fChangeTime, m_InputData.oneZ == true && GetUnitClass() == CX2Unit::UC_LIRE_WIND_SNEAKER, LESI_LWS_DASH_XZ )
#endif RENA_SECOND_CLASS_CHANGE

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();

}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X1_EndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight				= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y		+= 180.0f;
}


//LESI_SNIPING_RANGER_DASH_COMBO_X2
#ifdef BALANCE_GRAND_ARCHER_20121213
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.29f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.29f ) == true && EventCheck( 0.29f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 3.f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
	
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X2_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( LESI_SIEGE_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
#endif //ADD_RENA_SYSTEM
	ELSE_IF_ZX_PRESSED_AFTER( m_fAttackCancelAfter )
	{
		StateChange( LESI_SIEGE_WAIT );
	}

	CommonEventProcess();
}

#else
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 4.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );

			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X2_EventProcess()
{
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false; //X4
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false; //X4up
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false; //X4down
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_SNIPING_RANGER_DASH_COMBO_X3 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_SNIPING_RANGER_DASH_COMBO_X3 );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_SNIPING_RANGER_DASH_COMBO_X3 );
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneX == true && m_InputData.pureDown && m_pXSkinAnimFuture->GetNowAnimationTime() <= fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
	}
	else if( m_InputData.oneX == true && m_InputData.pureUp && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#endif //BALANCE_GRAND_ARCHER_20121213

#ifndef BALANCE_GRAND_ARCHER_20121213
//LESI_SNIPING_RANGER_DASH_COMBO_X3
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.29f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.29f ) == true && EventCheck( 0.29f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_SNIPING_RANGER_DASH_COMBO_X3_UP
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3_UP_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.31f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.31f ) == true && EventCheck( 0.31f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 15.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE1_Z", degree.z, 0.0f );

			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE3_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );




		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3_UP_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.32f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.32f ) == true && EventCheck( 0.32f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 15.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;


			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE1_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}


			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE3_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE2_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );


		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}
#endif //BALANCE_GRAND_ARCHER_20121213

//LESI_JUMP_ATTACK_Z
void CX2GULire_ElvenRanger::JumpAttackZFrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( m_InputData.pureDown == true && m_InputData.pureUp == false )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::JumpAttackZFrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::JumpAttackZEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true
		&& true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

//LESI_JUMP_ATTACK_X
void CX2GULire_ElvenRanger::JumpAttackXStartFuture()
{
	m_JumpXCount++;
	m_fJumpXTime = 0.3f;
	CommonStateStartFuture();
}

void CX2GULire_ElvenRanger::JumpAttackXFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.13f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.26f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}
	else
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = GetWalkSpeed();
		}
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::JumpAttackXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.21f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.21f ) == true && EventCheck( 0.21f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::JumpAttackXEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}


//LESI_DASH_JUMP_ATTACK_Z
//void CX2GULire_ElvenRanger::DashJumpAttackZStartFuture()
//{
//	CommonStateStartFuture();
//}

//void CX2GULire_ElvenRanger::DashJumpAttackZFrameMoveFuture()
//{
//	CommonFrameMoveFuture();
//}

//void CX2GULire_ElvenRanger::DashJumpAttackZFrameMove()
//{
//	CommonFrameMove();
//}

void CX2GULire_ElvenRanger::DashJumpAttackZEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}

	CommonEventProcess();
}

//LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2 )
#else //ADD_RENA_SYSTEM
	ELSE_IF_STATE_CHANGE_ON_( 0, 100.f, 0.25f, m_InputData.oneZ == true, LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2 )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3 )
#else //ADD_RENA_SYSTEM
	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.3f )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3 );
		}
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3
void CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}

	CommonEventProcess();
}


//LESI_DASH_JUMP_ATTACK_Z_LANDING
void CX2GULire_ElvenRanger::DashJumpAttackZLandingStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 1.5f;
}

void CX2GULire_ElvenRanger::DashJumpAttackZLandingStart()
{
	CommonStateStart();

	CreateStepDust();
}

void CX2GULire_ElvenRanger::DashJumpAttackZLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true && FlushMp( 2.0f ) == true )
	{
		StateChange( LESI_COMBO_X1 );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // ����ȯ
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	//else if( m_InputData.oneDown == true && 
	//	g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, 
	//	&m_FrameDataFuture.unitCondition.landPosition, 
	//	&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	//{
	//	StateChange( LESI_JUMP_DOWN );
	//	m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	//}	

	CommonEventProcess();
}


//LESI_DASH_JUMP_COMBO_X1
void CX2GULire_ElvenRanger::DashJumpComboX1FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.5f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DashJumpComboX1FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.23f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.23f ) == true && EventCheck( 0.23f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
			CX2DamageEffect::CEffect* pDE = NULL;
			
#ifdef SERV_RENA_NIGHT_WATCHER	//���� 2�� ���� ����Ʈ ���� - ������ ȭ��
			if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnitClass() )
			{
				posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );	//���� ������ �ؼ� ��ġ ������
				posR.x += 30.f;
				posR.y += 20.f;

				if( true == GetActiveStartOfDelayedFiring() )			//������ ��ȣź�� ������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION_DOWN_DELAY", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetIndex( CreateDamageEffectIndex() );						//�ش� ȭ�쿡 Index�� �ο��Ѵ�.( ���� ���� �� ������ )
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY );	//������ ȭ�� ����
					}
				}
				else																//������ ȭ��
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_OF_EXPLOSION_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

					if( NULL != pDE )
					{
						pDE->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION );		//�ش� ȭ�쿡 Type�� �ο��Ѵ�.( ������ ȭ��  �νĿ� )
					}
				}
			}
			else
			{
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			}
#else  SERV_RENA_NIGHT_WATCHER
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
#endif SERV_RENA_NIGHT_WATCHER

			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );


		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashJumpComboX1EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_DASH_JUMP_COMBO_X2 )
#else //ADD_RENA_SYSTEM
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.4f )
	{
		StateChange( LESI_DASH_JUMP_COMBO_X2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.4f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_DASH_JUMP_COMBO_X2
void CX2GULire_ElvenRanger::DashJumpComboX2FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.5f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DashJumpComboX2FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );

			CX2DamageEffect::CEffect* pDE = NULL;



			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashJumpComboX2EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_DASH_JUMP_COMBO_X3 )
#else //ADD_RENA_SYSTEM
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.35f )
	{
		StateChange( LESI_DASH_JUMP_COMBO_X3 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.35f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_DASH_JUMP_COMBO_X3
#ifdef ADD_RENA_SYSTEM // ����ȯ
void CX2GULire_ElvenRanger::DashJumpComboX3StateStart()
{
	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	SetNowDamageRelateSkillData( 100 );

	CommonStateStart();
}
#endif // ADD_RENA_SYSTEM

void CX2GULire_ElvenRanger::DashJumpComboX3FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.3f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::DashJumpComboX3FrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	#ifdef ADD_RENA_SYSTEM //��â��
		if( m_pXSkinAnim->EventTimerOneshot( 0.233f ) )
	#else //ADD_RENA_SYSTEM
		if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
	#endif //ADD_RENA_SYSTEM
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	#ifdef ADD_RENA_SYSTEM //��â��
		if( m_pXSkinAnim->EventTimer( 0.233f ) == true && EventCheck( 0.233f, false ) == true )
	#else //ADD_RENA_SYSTEM
		if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
	#endif //ADD_RENA_SYSTEM
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 6.f ) == true )
#else
		if( FlushMp( 12.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
			//degree.z = -5;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;				
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			degree.z -= 20.f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}


			degree.z -= 20.f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -25.f );

		}
	}
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 4.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;


			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );

		}
	}
#endif
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::DashJumpComboX3EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}


	CommonEventProcess();
}

//LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.5f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			CX2DamageEffect::CEffect* pDE = NULL;

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, fDegreeZ );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3 )
#else //ADD_RENA_SYSTEM
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.35f )
	{
		StateChange( LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.35f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3
void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.3f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 12.0f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );

			CX2DamageEffect::CEffect* pDE = NULL;

			//degree.z = -5;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			degree.z -= 20.f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}


			degree.z -= 20.f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -25.f );

		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}


	CommonEventProcess();
}

void CX2GULire_ElvenRanger::ShowSiegeFailGuide()
{
	if( m_fElaspedTimeLastGuideMessage > 2.f )
	{
		m_fElaspedTimeLastGuideMessage = 0.f;
	}
}

////////////////////////////////////////////////////////////////////////
//LESI_SI_LE_POWER_ATTACK
void CX2GULire_ElvenRanger::LESI_SI_LE_POWER_ATTACK_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.28f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.28f ) == true && EventCheck( 0.28f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI_LE_POWER_ATTACK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );
	
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI_LE_POWER_ATTACK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


//LESI_SI_LE_POWER_ATTACK_HYPER
void CX2GULire_ElvenRanger::LESI_SI_LE_POWER_ATTACK_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.28f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.28f ) == true && EventCheck( 0.28f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI_LE_POWER_ATTACK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI_LE_POWER_ATTACK_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_SI1_LE_PIERCE_ARROW
void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO1 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SONIC_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SONIC_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}		
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SONIC_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.5f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



//LESI_SI1_LE_PIERCE_ARROW_HYPER
void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_HYPER_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO1 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SONIC_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SONIC_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SONIC_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.5f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_PIERCE_ARROW_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//LESI_SI2_LE_DIVISION_BLAST
void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_Start()
{
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO8 ) == true )
		{
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// ���� ���� ���� -45 ~ 45 -> -30 ~ 30
			degree.z = 30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = -30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			degree.z = 45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = -45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		}
		else
		{
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// ���� ���� ���� -45 ~ 45 -> -30 ~ 30
			degree.z = 30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = -30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			degree.z = 45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			degree.z = -45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.6f ) == true && EventCheck( 1.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();
		degree.z = 20.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		degree.z = 0.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		degree.z = -20.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	}
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.3f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.6f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



//LESI_SI2_LE_DIVISION_BLAST_HYPER
void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_HYPER_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_HYPER_Start()
{
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO8 ) == true )
		{
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// ���� ���� ���� -45 ~ 45 -> -30 ~ 30
			degree.z = 30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

			degree.z = -30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			degree.z = 45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

			degree.z = -45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		}
		else
		{
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			degree.z = 45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

			degree.z = 0.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

			degree.z = -45.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.6f ) == true && EventCheck( 1.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

		degree.z = 20.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

		degree.z = 0.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

		degree.z = -20.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MULTI_ARROW_REAL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	}
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_HYPER_CameraMove()
{
	CommonCameraMove();
	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.3f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.5f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_DIVISION_BLAST_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}




//LESI_SI3_LE_PHOENIX_STRIKE
void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleRenaMajorParticleByEnum( RENA_MAJOR_PII_PHOENIX_START_WAVE ) );
		if( NULL != pSeq )
		{
			PlaySequenceByTriggerCount( pSeq, pos, 5, 10, 2 );
			pSeq->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
		}

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO2 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHOENIX_ARROW_MEMO", GetPowerRate(), pos, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHOENIX_ARROW", GetPowerRate(), pos, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}		
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHOENIX_ARROW", GetPowerRate(), pos, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 0.8f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}			
		}
		break;		
	}
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



//LESI_SI3_LE_PHOENIX_STRIKE_HYPER
void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_HYPER_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

		
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleRenaMajorParticleByEnum( RENA_MAJOR_PII_PHOENIX_START_WAVE ) );
		if( NULL != pSeq )
		{
			PlaySequenceByTriggerCount( pSeq, pos, 5, 10, 2 );
			pSeq->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
		}

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO2 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHOENIX_ARROW_REAL_MEMO", GetPowerRate(), pos, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHOENIX_ARROW_REAL", GetPowerRate(), pos, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
		}		
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHOENIX_ARROW_REAL", GetPowerRate(), pos, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}


	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 0.8f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}			
		}
		break;		
	}
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_PHOENIX_STRIKE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#ifdef SERV_RENA_NIGHT_WATCHER

//LESI_COMBO_RNW_ZZX
void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZX_StateStart()
{
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZX_FrameMove()
{
	if( true == m_bUsePriorPlannedBlow )
	{
		CheckPriorPlannedBlow();
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZX_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX == true, LESI_COMBO_RNW_ZZXX )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZX_StateEnd()
{
	DeleteTrapBlade();
	CommonStateEnd();
}

//LESI_COMBO_RNW_ZZXX
void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXX_StateStart()
{
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXX_FrameMove()
{
	if( true == m_bUsePriorPlannedBlow )
	{
		CheckPriorPlannedBlow();
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXX_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX == true, LESI_COMBO_RNW_ZZXXX )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	BWALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXX_StateEnd()
{
	DeleteTrapBlade();
	CommonStateEnd();
}

//LESI_COMBO_RNW_ZZXXX
void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXXX_StateStart()
{
	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXXX_FrameMove()
{
	if( true == m_bUsePriorPlannedBlow )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( true == m_bActivePriorPlannedBlow )			/// ���� �ϰ� ��� ������ ������ ��
			{
				/// ���� �ϰ� ����� ������ ����( ������ 50% ���� * ��ų ���� �� )
			
				m_DamageData.damage.fPhysic		*= ( 1.f + m_fPriorPlannedBlowIncreaseDamage * m_iPriorPlannedBlowHitCount );
				m_bActivePriorPlannedBlowAttack = true;		/// �ش� Ÿ���� ���� �ϰ��� ����� Ÿ��( ������ Ÿ�� )
			}
			else
			{
				m_bPossibleHitCount = true;
			}
		}

		CheckPriorPlannedBlow();
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXXX_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RNW_ZZXXX_StateEnd()
{
	DeleteTrapBlade();
	CommonStateEnd();
}

//LESI_RNW_ACCEL_DASH
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_StateStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = m_PhysicParam.nowSpeed.x + m_fWalkCancelAfter;
	//m_PhysicParam.passiveSpeed.x = m_PhysicParam.nowSpeed.x;
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_StateStart()
{
	CommonStateStart();
	CreateStepDust();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_FrameMoveFuture()
{

	if( m_PhysicParam.nowSpeed.x >= GetDashSpeed() )
	{
		m_PhysicParam.nowSpeed.x += -1600.f * m_fElapsedTime;
	}
	else
	{
		//m_PhysicParam.passiveSpeed.x = GetDashSpeed();
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	if( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER )
	{
		//0.13�� �̻� ��ø� �� ��, �׼� ��ø� ����� �� �ִ�.( �ӽ� )
		const float iAccelDashTime = 0.13f;
		if( m_FrameDataNow.unitCondition.fStateTime >= iAccelDashTime )
		{
			m_bAccelDashEventFlag = true;
		}
	}

	CommonFrameMoveFuture();
}
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_DASH_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		//switch( GetUnit()->GetClass() )
		//{
		//case CX2Unit::UC_LIRE_WIND_SNEAKER:
		//	{
		//		StateChange( LESI_LWS_DASH_X );
		//	} break;

		//default:
		//	{
		StateChange( LESI_DASH_ATTACK_X );
		//	} break;
		//}
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // ����ȯ
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_InputData.oneUp == true )
	{
		//�׼� ��� ����
		StateChange( LESI_RNW_ACCEL_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = m_PhysicParam.GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChangeDashIfPossible();
		}
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChangeDashIfPossible();
		}
	}
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}	

	CommonEventProcess();
}

//LESI_RNW_ACCEL_DASH_JUMP
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_StateStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.fReloadAccel		= m_fSkillCancelAfter;
	m_PhysicParam.nowSpeed.x		= GetDashSpeed() * 2.25f;
	m_bDownForce					= false;

}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_StateStart()
{
	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
	{
		CreateStepDust();
	}

	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_StateEndFuture()
{
	m_PhysicParam.fReloadAccel	= m_OrgPhysicParam.fReloadAccel;
	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_EventProcess()
{
	if( m_InputData.oneUp == true && m_iDoubleJumpCount == 0 )
	{
		StateChange( LESI_DASH_DOUBLE_JUMP );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_RNW_ACCEL_DASH_JUMP_LANDING );
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( LESI_RNW_ACCEL_DASH_JUMP_Z );
	}

	CommonEventProcess();
}

//LESI_RNW_ACCEL_DASH_JUMP_LANDING
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_LANDING_StateStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 1.5f;
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_LANDING_StateStart()
{
	CommonStateStart();
	CreateStepDust();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // ����ȯ
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.oneDown == true 
			 && g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
															  LINE_RADIUS, 
															  &m_FrameDataFuture.unitCondition.landPosition, 
															  &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}		
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//LESI_RNW_ACCEL_DASH_JUMP_Z
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_StateStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x		= GetDashSpeed() * 2.45f;
	m_PhysicParam.nowSpeed.y		= 0.f;
	m_PhysicParam.fReloadAccel		= m_fSkillCancelAfter;
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_RNW_ACCEL_DASH_JUMP_LANDING );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_RNW_ACCEL_DASH_JUMP_ZZ )
#else //ADD_RENA_SYSTEM
	ELSE_IF_STATE_CHANGE_ON_( 0, 100.f, 0.25f, m_InputData.oneZ == true, LESI_RNW_ACCEL_DASH_JUMP_ZZ )
	//ELSE_IF_STATE_CHANGE_ON_( 0, 100.f, 0.25f, m_InputData.oneZ == true, LESI_DASH_JUMP_ATTACK_RTR_ZZ )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_StateEndFuture()
{
	CommonStateEndFuture();
	m_PhysicParam.fReloadAccel	= m_OrgPhysicParam.fReloadAccel;
}

//LESI_RNW_ACCEL_DASH_JUMP_ZZ
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_StateStartFuture()
{
	
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x		= GetDashSpeed() * 2.35f;
	m_PhysicParam.nowSpeed.y		= m_PhysicParam.GetJumpSpeed() * 0.5f;
	m_PhysicParam.fReloadAccel		= m_fSkillCancelAfter;
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_FrameMoveFuture()
{	
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_RNW_ACCEL_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_StateEndFuture()
{
	
	CommonStateEndFuture();
	m_PhysicParam.fReloadAccel	= m_OrgPhysicParam.fReloadAccel;
}

//LESI_SA_RNW_FURIOUS_ENGAGE
void CX2GULire_ElvenRanger::LESI_SA_RNW_FURIOUS_ENGAGE_StateStart()
{
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;			//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;		//���� �ϰ��� ����� Ÿ�� �к�
	}

	CreateTrapBlade();

	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_FURIOUS_ENGAGE_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_FURIOUS_ENGAGE_FrameMove()
{	
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );

	//������ ���� 2Ÿ
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) == true && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}

		if( true == m_bUsePriorPlannedBlow )
			m_bPossibleHitCount = true;		//���� Ÿ������ ����Ǿ�����, ���� �ϰ� Count ���� �����ϰ� ����

	}
	//������ ���� 3Ÿ
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime1 ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime1 ) == true && EventCheck( m_fDamageDataChangeTime1, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_THIRD" );
			m_LuaManager.EndTable();		
		}

		if( true == m_bUsePriorPlannedBlow )
			m_bPossibleHitCount = true;		//���� Ÿ������ ����Ǿ�����, ���� �ϰ� Count ���� �����ϰ� ����
	}
	//�߷� ���� ���� 4Ÿ
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime2 ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime2 ) == true && EventCheck( m_fDamageDataChangeTime2, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		DeleteTrapBlade();

		//�����߷� ����.
		SetEnableAttackBox( L"Rfoot", true );

		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_LAST" );
			m_LuaManager.EndTable();		
		}
	}

	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();	//���� �ϰ� ���� ���� ���� �˻�

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_FURIOUS_ENGAGE_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_FURIOUS_ENGAGE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}
#endif SERV_RENA_NIGHT_WATCHER

//LESI_SI1_LE_ASSAULT_KICK
void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_Init()
{
	XSkinMeshReadyInBackground( L"AssaultKick01.X" );
	XSkinMeshReadyInBackground( L"AssaultKick02.X" );
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_FrameMove()
{	
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// Damage Data Next ������� ����

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.reActType			= CX2DamageManager::RT_UP;
		m_DamageData.backSpeed.x		= 750.0f;	//m_OrgPhysicParam.fRunSpeed * 1.8f;
		m_DamageData.backSpeed.y		= 1600.0f;	//m_OrgPhysicParam.fJumpSpeed * 1.3f;
		m_DamageData.fStopTimeAtt		= 0.0f;
		m_DamageData.fStopTimeDef		= 0.0f;
		m_DamageData.fCameraCrashGap	= 10.0f;
		m_DamageData.fCameraCrashTime	= 0.2f;
	}
	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����	
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick01", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick01", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.96f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.96f ) == true && EventCheck( 0.96f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick01", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick02", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.3f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI1_LE_ASSAULT_KICK_HYPER
void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// DAMAGE_DATA_NEXT ������� ����

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.reActType			= CX2DamageManager::RT_UP;

		m_DamageData.backSpeed.x		= 750.0f;	//m_OrgPhysicParam.fRunSpeed * 1.8f;
		m_DamageData.backSpeed.y		= 1600.0f;	//m_OrgPhysicParam.fJumpSpeed * 1.3f;
		m_DamageData.fStopTimeAtt		= 0.0f;
		m_DamageData.fStopTimeDef		= 0.0f;
		m_DamageData.fCameraCrashGap	= 10.0f;
		m_DamageData.fCameraCrashTime	= 0.2f;
	}
	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick01Red", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick01Red", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.96f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.96f ) == true && EventCheck( 0.96f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick01Red", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AssaultKick02Red", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_ASSAULT_KICK_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.3f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI1_LE_HUMMING_WIND
void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_Init()
{
	TextureReadyInBackground( L"HummingWind04.dds" );
	XSkinMeshReadyInBackground( L"HummingWind_AttackBox.X" );
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_FrameMoveFuture()
{
	CommonFrameMoveFuture();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.15f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.15f ) == true && EventCheck( 0.15f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.88f )
	{
		m_PhysicParam.nowSpeed.y = 0.0f;
	}
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"HummingWindShot", posR );
		if( pSeq != NULL )
		{
			pSeq->SetBlackHolePosition( GetPos() );
		}
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO10 ) == true )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeqHummingWind = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"HummingWind", GetPos() );
			if( pSeqHummingWind != NULL )
			{
				pSeqHummingWind->SetScaleFactor( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) );
			}
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HUMMING_WIND_MEMO", GetPowerRate(), GetPos(), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"HummingWind", GetPos() );
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HUMMING_WIND", GetPowerRate(), GetPos(), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.7f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI1_LE_HUMMING_WIND_HYPER
void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.15f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.15f ) == true && EventCheck( 0.15f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.88f )
	{
		m_PhysicParam.nowSpeed.y = 0.0f;
	}
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"HummingWindShot_Hyper", posR );
		if( pSeq != NULL )
		{
			pSeq->SetBlackHolePosition( GetPos() );
		}
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO10 ) == true )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeqHummingWind = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"HummingWind_Hyper", GetPos() );
			if( pSeqHummingWind != NULL )
			{
				pSeqHummingWind->SetScaleFactor( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) );
			}
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HUMMING_WIND_HYPER_MEMO", GetPowerRate(), GetPos(), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"HummingWind_Hyper", GetPos() );
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HUMMING_WIND_HYPER", GetPowerRate(), GetPos(), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LE_HUMMING_WIND_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.7f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI2_LE_GUIDED_ARROW
void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_Init()
{
	//TextureReadyInBackground( L"GuideArrow01.dds" );
	//XSkinMeshReadyInBackground( L"GuideArrow_AttackBox.X" );
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		CreateGuideArrow( true == GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO4 ) );
#else
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef BALANCE_PATCH_20120329
		degree.z -= 45.f;
#endif

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO4 ) == true )
		{
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NONE, 4 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 30.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 1, CX2DamageEffect::LOT_NONE, 4 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 30.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 2, CX2DamageEffect::LOT_NONE, 4 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 30.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 3, CX2DamageEffect::LOT_NONE, 4 );
		}
		else
		{
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NONE, 3 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 45.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 1, CX2DamageEffect::LOT_NONE, 3 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 45.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 2, CX2DamageEffect::LOT_NONE, 3 );
		}
#else
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 1 );
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 2 );
#endif
#endif //BALANCE_GRAND_ARCHER_20121213
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.8f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}



//LESI_SI2_LE_GUIDED_ARROW_HYPER
void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		CreateGuideArrow( true == GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO4 ) , true );
#else
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef BALANCE_PATCH_20120329
		degree.z -= 45.f;
#endif

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO4 ) == true )
		{
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NONE, 4 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 30.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 1, CX2DamageEffect::LOT_NONE, 4 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 30.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 2, CX2DamageEffect::LOT_NONE, 4 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 30.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_MEMO_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 3, CX2DamageEffect::LOT_NONE, 4 );
		}
		else
		{
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NONE, 3 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 45.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 1, CX2DamageEffect::LOT_NONE, 3 );
#ifdef BALANCE_PATCH_20120329
			degree.z += 45.f;
#endif
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_HYPER", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y ), 2, CX2DamageEffect::LOT_NONE, 3 );
		}
#else
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 1 );
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDED_ARROW_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 2 );
#endif
#endif //BALANCE_GRAND_ARCHER_20121213
	}
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_GUIDED_ARROW_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.8f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI2_LE_PERFECT_STORM
void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_Init()
{
	XSkinMeshReadyInBackground( L"PerfectStorm01.X" );
	XSkinMeshReadyInBackground( L"PerfectStorm02.X" );
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 1 );
#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.533f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.533f ) == true && EventCheck( 0.533f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		m_PerfectStormPos = GetPos();
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 100.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 100.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PERFECT_STORM", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 60.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 60.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"PerfectStorm02", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.633f ) == true && EventCheck( 0.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 140.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 140.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"PerfectStorm02", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.666f ) == true && EventCheck( 0.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 100.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 100.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"PerfectStorm03", pos, GetRotateDegree(), GetRotateDegree() );
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ĵ�� ����Ʈ ����
	SKILL_CANCEL_AFTER ( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.7f )
		{
			StateChange( USI_WAIT );
		}
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	CommonEventProcess();
}


//LESI_SI2_LE_PERFECT_STORM_HYPER
void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 1 );
#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.533f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.533f ) == true && EventCheck( 0.533f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		m_PerfectStormPos = GetPos();
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 100.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 100.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PERFECT_STORM_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 60.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 60.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"PerfectStorm02Hyper", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.633f ) == true && EventCheck( 0.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 140.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 140.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"PerfectStorm02Hyper", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.666f ) == true && EventCheck( 0.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 100.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 100.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"PerfectStorm03Hyper", pos, GetRotateDegree(), GetRotateDegree() );
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LE_PERFECT_STORM_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ĵ�� ����Ʈ ����
	SKILL_CANCEL_AFTER ( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.7f )
		{
			StateChange( USI_WAIT );
		}
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

	
	CommonEventProcess();
}


//LESI_SI3_LE_AERO_TORNADO
void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_Init()
{
	XSkinMeshReadyInBackground( L"AeroTornado01.X" );
	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"AeroTornado07.dds" );
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.19f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.833f ) == true && EventCheck( 0.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PerfectStormPos = GetBonePos( L"Bip01_Footsteps" );
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 430.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 430.0f;
		pos.y -= 200.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AERO_TORNADO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 2.8f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI3_LE_AERO_TORNADO_HYPER
void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.19f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.833f ) == true && EventCheck( 0.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PerfectStormPos = GetBonePos( L"Bip01_Footsteps" );
		D3DXVECTOR3 pos;
		if( GetIsRight() == true )
			pos = m_PerfectStormPos + GetDirVector() * 430.0f;
		else
			pos = m_PerfectStormPos - GetDirVector() * 430.0f;
		pos.y -= 200.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AERO_TORNADO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LE_AERO_TORNADO_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 2.8f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI1_LCR_SPINNING_KICK
void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_Init()
{
	XMeshReadyInBackground( L"SpinningKick02" );
}

void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Bip01_Pelvis" );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpinningKick01", pos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMeshSpinningKick1 = pMeshInst->GetHandle();
		}

		m_hSeqSpinningKick1 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SpinningKick02", pos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick1 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		m_hSeqSpinningKick2 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_SpinningKick03", pos );
		pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick2 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}


	if( m_hMeshSpinningKick1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshSpinningKick1 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Pelvis" ) );
			pMeshInst->SetRotateDegree( GetRotateDegree() );
			pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );
		}
		else
		{
			m_hMeshSpinningKick1 = INVALID_MESH_INSTANCE_HANDLE;
		}		
	}



	if( m_hSeqSpinningKick1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick1 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_Pelvis" ) );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqSpinningKick1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}		
	}


	if( m_hSeqSpinningKick2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick2 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_Pelvis" ) );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqSpinningKick2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}		
	}


#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO9 ) == true )
	{	// �޽� �޸� ���� ��, DamageDatachangeTime �� �����Ѵ�.
		m_fCommonDamageChangeTime = -1.f; 
	}
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	bool bActiveDown = true;

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO9 ) == true && GetRandomFloat() <= 0.3f )
	{
		bActiveDown = false;
	}
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.6f )
	{
		if( bActiveDown == true )
			m_DamageData.reActType = CX2DamageManager::RT_DOWN;
		else
			m_DamageData.reActType = CX2DamageManager::RT_BIG_DAMAGE;
	}

#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����


	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI1_LCR_SPINNING_KICK_HYPER
void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_HYPER_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->EventTimer( 0.46f ) == true && EventTimer( 0.46f, true ) == true )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.5f;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Bip01_Pelvis" );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpinningKick01", pos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMeshSpinningKick1 = pMeshInst->GetHandle();
		}


		m_hSeqSpinningKick1 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SpinningKick02", pos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick1 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		m_hSeqSpinningKick2 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_SpinningKick03", pos );
		pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick2 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	if( m_hMeshSpinningKick1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshSpinningKick1 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Pelvis" ) );
			pMeshInst->SetRotateDegree( GetRotateDegree() );
			pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );
		}
		else
		{
			m_hMeshSpinningKick1 = INVALID_MESH_INSTANCE_HANDLE;
		}		
	}

	if( m_hSeqSpinningKick1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick1 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_Pelvis" ) );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqSpinningKick1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}		
	}


	if( m_hSeqSpinningKick2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqSpinningKick2 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_Pelvis" ) );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqSpinningKick2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}		
	}



#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO9 ) == true )
	{	// �޽� �޸� ���� ��, DamageDatachangeTime �� �����Ѵ�.
		m_fCommonDamageChangeTime = -1.f; 
	}
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	bool bActiveDown = true;
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO9 ) == true && GetRandomFloat() <= 0.3f )
	{
		bActiveDown = false;
	}
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.6f )
	{
		if( bActiveDown == true )
			m_DamageData.reActType = CX2DamageManager::RT_DOWN;
		else
			m_DamageData.reActType = CX2DamageManager::RT_BIG_DAMAGE;
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI1_LCR_SPINNING_KICK_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI2_LCR_CRESCENT_KICK
void CX2GULire_ElvenRanger::LESI_SI2_LCR_CRESCENT_KICK_Init()
{
	TextureReadyInBackground( L"Lire_02_CrescentKick02.dds" );
	XSkinMeshReadyInBackground( L"CrescentKick01.X" );
	XSkinMeshReadyInBackground( L"CrescentKick02.X" );
	XSkinMeshReadyInBackground( L"CrescentKick03.X" );
}

void CX2GULire_ElvenRanger::LESI_SI2_LCR_CRESCENT_KICK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.14f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.166f ) == true && EventCheck( 0.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 70.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"CrescentKick01", GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO3 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.766f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.766f ) == true && EventCheck( 0.766f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 70.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"CrescentKick02", GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO3 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.566f ) == true && EventCheck( 1.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 70.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"CrescentKick03", GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO3 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_B_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_B", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_B", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LCR_CRESCENT_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 2.5f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI2_LCR_CRESCENT_KICK_HYPER
void CX2GULire_ElvenRanger::LESI_SI2_LCR_CRESCENT_KICK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.166f ) == true && EventCheck( 0.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 70.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"CrescentKick01", GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO3 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_MEMO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.766f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.766f ) == true && EventCheck( 0.766f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 70.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"CrescentKick02", GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO3 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_MEMO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else		
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_A_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.566f ) == true && EventCheck( 1.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 70.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"CrescentKick03", GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO3 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_B_MEMO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_B_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else		
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRESCENT_KICK_B_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI2_LCR_CRESCENT_KICK_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 2.5f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//LESI_SI3_LCR_DIVE_KICK_BOMBING
void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_Init()
{
	XSkinMeshReadyInBackground( L"DiveKickBombing01.x" );
	XSkinMeshReadyInBackground( L"DiveKickBombing02.X" );
}

void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.29f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.29f ) == true && EventCheck( 0.29f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.5f;
	}

	if( m_PhysicParam.nowSpeed.x < GetWalkSpeed() / 2.0f )
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();

	if( m_pXSkinAnimFuture ->GetNowAnimationTime() > 3.666f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
	}
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Bip01_R_Foot", 0.14f, 2 );
#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ���̺� ű ���� ���� �ҽ� ���� ( ĳ���� ��ũ��Ʈ �� ����Ʈ ������ �̰� )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 3.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 3.666f ) == true && EventCheck( 3.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Bip01_Pelvis" );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"DiveKinkBombing01", pos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMeshDiveKickBombing = pMeshInst->GetHandle();
		}
	}

	if( m_pXSkinAnim->GetNowAnimationTime() > 3.5f )
	{
		m_DamageData.reActType = CX2DamageManager::RT_DOWN;
	}

	if( m_hMeshDiveKickBombing != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshDiveKickBombing );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Pelvis" ) );
		}
		else
		{
			m_hMeshDiveKickBombing = INVALID_MESH_INSTANCE_HANDLE;
		}
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER
void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.29f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.29f ) == true && EventCheck( 0.29f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.5f;
	}

	if( m_PhysicParam.nowSpeed.x < GetWalkSpeed() / 2.0f )
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();

	if( m_pXSkinAnimFuture ->GetNowAnimationTime() > 3.666f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
	}
	CommonFrameMoveFuture();
}
void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.14f, 2 );
#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ���̺� ű ���� ���� �ҽ� ���� ( ĳ���� ��ũ��Ʈ �� ����Ʈ ������ �̰� )
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 3.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 3.666f ) == true && EventCheck( 3.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Bip01_Pelvis" );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"DiveKinkBombing01", pos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMeshDiveKickBombing = pMeshInst->GetHandle();
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 4.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 4.55f ) == true && EventCheck( 4.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DIVE_KICK_FINAL", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}


	if( m_hMeshDiveKickBombing != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshDiveKickBombing );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Pelvis" ) );
		}
		else
		{
			m_hMeshDiveKickBombing = INVALID_MESH_INSTANCE_HANDLE;
		}
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����



	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//LESI_SI3_LSR_CRAZY_SHOT
void CX2GULire_ElvenRanger::LESI_SI3_LSR_CRAZY_SHOT_Init()
{
	TextureReadyInBackground( L"Lire_03_CrazyShot01.dds" );
}

void CX2GULire_ElvenRanger::LESI_SI3_LSR_CRAZY_SHOT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.16f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Lire_CrazyShot04", pos );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.05f ) == true && EventCheck( 1.05f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO20 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRAZY_SHOT_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRAZY_SHOT", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LSR_CRAZY_SHOT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//LESI_SI3_LSR_CRAZY_SHOT_HYPER
void CX2GULire_ElvenRanger::LESI_SI3_LSR_CRAZY_SHOT_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Lire_CrazyShot04", pos );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.05f ) == true && EventCheck( 1.05f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO20 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRAZY_SHOT_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"CRAZY_SHOT_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI3_LSR_CRAZY_SHOT_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

/** @function : COMMON_BUFF_FrameMove
@brief : ���������� ���� ���� ��ų�� FrameMove
*/
/*virtual*/ void CX2GULire_ElvenRanger::COMMON_BUFF_FrameMove()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 // ���� ��ų ����, ������
	// ���� ���� �ƴ����� ���� World�� ����
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.04f, 0 );
#endif // UPGRADE_SKILL_SYSTEM_2013 // ���� ��ų ����, ������


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		PlayCommonBuffMinorParticle();
		UpDownCrashCamera( 20.0f, 0.3f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.77f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.77f ) == true && EventCheck( 0.77f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		int	iSkillSlotIndex = 0;
		bool bSlotB = false;

		if ( true == GetSkillSlotIndexUsed( iSkillSlotIndex, bSlotB ) )
		{
			const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();

			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = pUnitData->m_UserSkillTree.GetSkillSlot( iSkillSlotIndex, bSlotB );
			CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE;

			if( NULL != pSkillSlotData )
				eSkillID = pSkillSlotData->m_eID;

			ProcessAcceleraterBuff( eSkillID, IsHyperState() );
		}
	}

	CommonFrameMove();
}

#ifndef SERV_9TH_NEW_CHARACTER // ����ȯ
/// �ٸ� ĳ���͵� ���� �Ȱ��� �Լ��� ���� ������, X2GUUser�� �ű���.
void CX2GULire_ElvenRanger::CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ )
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
	if( NULL != pSeq )
	{
		PlaySequenceByTriggerCount( pSeq, vPos, 100, 100, 1 );
		D3DXVECTOR3 vAngle = GetRotateDegree();
		vAngle += D3DXVECTOR3( fDegreeX, fDegreeY, fDegreeZ );

		pSeq->SetAxisAngle( vAngle );
		pSeq->SetAddRotate( vAngle );
	}
}
#endif // SERV_9TH_NEW_CHARACTER

void CX2GULire_ElvenRanger::LESI_SI_A_LE_BACKJUMP_SHOT_Init()
{
	XSkinMeshReadyInBackground( L"Lena_SI_A_LE_BACKJUMP_SHOT01.X" );
	XMeshReadyInBackground( L"Lire_FeatherA.Y" );
	TextureReadyInBackground( L"Smoke.dds" );
}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_BACKJUMP_SHOT_Start()
{
	CommonStateStart();

	m_nBackShotCount = 0;
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LE_BACKJUMP_SHOT );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if ( NULL == GetUnit() )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
		m_nBackShotCount = (int) pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_nBackShotCount = (int) pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
}


void CX2GULire_ElvenRanger::LESI_SI_A_LE_BACKJUMP_SHOT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2DamageEffect::CEffect* pDE = NULL;

		
		D3DXVECTOR3 vDegree = GetRotateDegree();
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );

#ifdef BALANCE_GRAND_ARCHER_20121213
		switch(m_nBackShotCount)
		{
		case 1:
			{
				D3DXVECTOR3 degree = vDegree;
				degree.z = -45.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				}
			}
			break;
		case 2:
			{
				D3DXVECTOR3 degree = vDegree;
				degree.z = -45.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				}

				degree = vDegree;
				degree.z = -30.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				}	
			}
			break;
		case 3:
			{
				D3DXVECTOR3 degree = vDegree;
				degree.z = -15.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				}
				degree = vDegree;
				degree.z = -30.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				}
				degree = vDegree;
				degree.z = -45.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				}
			}
			break;				
		}
#else
		switch(m_nBackShotCount)
		{
		case 1:
			{

				D3DXVECTOR3 degree = vDegree;
				degree.z = -45.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}
			}
			break;
		case 2:
			{
				D3DXVECTOR3 degree = vDegree;
				degree.z = -45.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}

				degree = vDegree;
				degree.z = -30.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}	
			}
			break;
		case 3:
			{
				D3DXVECTOR3 degree = vDegree;
				degree.z = -15.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}
				degree = vDegree;
				degree.z = -30.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}


				degree = vDegree;
				degree.z = -45.0f;
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_BACKJUMP", GetPowerRate() * m_fAttackPowerRateArrow, posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
				if( NULL != pDE )
				{
					pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
					// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
					pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
					pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				}
			}
			break;				
		}
#endif //BALANCE_GRAND_ARCHER_20121213
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_BACKJUMP_SHOT_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.4f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.71f)
	{
		m_PhysicParam.nowSpeed.y = 0.f;
		m_bDisableGravity = true;
	}
	else
	{
		m_bDisableGravity = false;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_BACKJUMP_SHOT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );			
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_BACKJUMP_SHOT_EndFuture()
{		
	m_bDisableGravity = false;
	CommonStateEndFuture();
}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_REFLEX_MAGIC_Init()
{
	XSkinMeshReadyInBackground( L"Lena_SI_A_LE_Reflex_Magic_Mesh01.X" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_REFLEX_MAGIC_Start()
{

	CommonStateStart();

}

void CX2GULire_ElvenRanger::LESI_SI_A_LE_REFLEX_MAGIC_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );			
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.7f )
		{
			StateChange( USI_WAIT );
		}
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}	

void CX2GULire_ElvenRanger::LESI_SI_A_LE_REFLEX_MAGIC_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_SI_A_LE_REFLEX_MAGIC );
	}

	CommonEventProcess();
}









void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_SA_SharpFall_Mesh01.X" );

	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"Whitecircle02.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( LESI_SA_LCR_SHARPFALL_LANDING );
	}

	CommonEventProcess();
}


void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

	CommonFrameMove();
}




void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( LESI_SA_LCR_SHARPFALL_LANDING );
	}

	CommonEventProcess();
}





void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_LANDING_FrameMove()
{

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_SHARPFALL_LANDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}







void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_Init()
{

	XSkinMeshReadyInBackground( L"Lena_SI_A_LE_BACKJUMP_SHOT01.X" );
	XSkinMeshReadyInBackground( L"WindWard_XMesh01.X" );

	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"AeroTornado04.dds" );

}


void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_StateStart()
{
	CommonStateStart();
	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}


void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LSR_WINDWARD );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_WindWard", (CX2GameUnit*) this, NULL, false, GetPowerRate(), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel ),
				D3DXVECTOR3( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel ) ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_WindWard", (CX2GameUnit*) this, NULL, false, GetPowerRate(), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME ),
				D3DXVECTOR3( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE ) ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}



	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_HYPER_StateStart()
{
	CommonStateStart();
	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}


void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LSR_WINDWARD );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_WindWard", (CX2GameUnit*) this, NULL, true, GetPowerRate(), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel ),
				D3DXVECTOR3( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel ) ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_WindWard", (CX2GameUnit*) this, NULL, true, GetPowerRate(), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME ),
				D3DXVECTOR3( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE ), 
				pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE ) ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}


	CommonFrameMove();
}




void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


void CX2GULire_ElvenRanger::LESI_SA_LSR_WINDWARD_HYPER_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}


// iMaxEntangleTrapCount��ŭ�� ����� ������ entangle trap�� �����Ѵ�. ��, ���������� ���� entangle trap�� ������ �� �ִ�.
bool CX2GULire_ElvenRanger::MakeRoomForEntangleTrap( int iMaxEntangleTrapCount, bool bForce )
{
	if( true == m_vecEntagleTrapUID.empty() )
		return true;

	int iRemoveCount = (int) m_vecEntagleTrapUID.size() - iMaxEntangleTrapCount;
	if( iRemoveCount <= 0 )
		return true;


	for( int i=0; i<(int)m_vecEntagleTrapUID.size(); i++ )
	{
		if( iRemoveCount <= 0 )
		{
			return true;
		}

		UidType& npcUID = m_vecEntagleTrapUID[i];
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( (int) npcUID );
		if( NULL != pNPC )
		{
			ASSERT( CX2UnitManager::NUI_ENTANGLE_TRAP_RENA == pNPC->GetNPCTemplet().m_nNPCUnitID );

			if( false == bForce && 
				1.f == pNPC->GetNowHp() ) // �������� Ʈ���� �� ���ְ�
				continue;

			pNPC->SetNowHp( 0.f );
		}

		m_vecEntagleTrapUID.erase( m_vecEntagleTrapUID.begin() + i );
		--i;
		--iRemoveCount;
	}

	return false;
}

void CX2GULire_ElvenRanger::CreateEntangleTrap()
{
	MakeRoomForEntangleTrap( m_MaxEntangleTrapCount - 1, false );

	if( true == g_pX2Game->IsHost() )
	{
		D3DXVECTOR3 vPos = GetLandPosition();
		int iHyperMode = 0;
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_ENTANGLE_TRAP_RENA, iHyperMode, true, vPos, 
			GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
	}
}

void CX2GULire_ElvenRanger::AddEntangleTrap( CX2GUNPC* pEntangle )
{
	if( pEntangle == NULL )
		return;

	m_vecEntagleTrapUID.push_back( pEntangle->GetUID() );

	// �������� �߰��� ���� �غ�
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LSR_ENTANGLE );
#else // UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LSR_ENTANGLE );
#endif // UPGRADE_SKILL_SYSTEM_2013	

	pEntangle->SetRealatedSkillLevel( iSkillLevel );

	//const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LSR_ENTANGLE );
	//CX2BuffFactorPtr ptrBuffFactor = pSkillTemplet->GetCloneBuffFactorPtr( 0 );
	//if ( NULL != ptrBuffFactor )
	//{
	//	ptrBuffFactor->id
	//	pNPC->GetDamageData().PushBuffFactor( ptrBuffFactor );
	//}
}


void CX2GULire_ElvenRanger::LESI_SA_LSR_ENTANGLE_Init()
{
	XSkinMeshReadyInBackground( L"SI_A_LSR_ENTANGLE_Effect_Seed.x" );
}



void CX2GULire_ElvenRanger::LESI_SA_LSR_ENTANGLE_StateStart()
{
	CommonStateStart();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO12 ) == true )
		m_pXSkinAnim->SetPlaySpeed( 2.f );
	else
		m_pXSkinAnim->SetPlaySpeed( 1.f );
}


void CX2GULire_ElvenRanger::LESI_SA_LSR_ENTANGLE_StateStartFuture()
{
	CommonStateStartFuture();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO12 ) == true )
		m_pXSkinAnimFuture->SetPlaySpeed( 2.f );
	else
		m_pXSkinAnimFuture->SetPlaySpeed( 1.f );
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_ENTANGLE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateEntangleTrap();
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_ENTANGLE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_ENTANGLE_StateEnd()
{
	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_LOW_KICK_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds" );
}

#ifndef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - �ο�ű �޸� �̵�ű���� �����Ǹ鼭, Start ������Ʈ ����
void CX2GULire_ElvenRanger::LESI_SA_LCR_LOW_KICK_StateStart()
{
	CommonStateStart();

	float fAnimSpeed = 1.f;
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LCR_LOW_KICK );
	if( iSkillLevel > 0 )
	{
#ifdef PVP_SEASON2
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LCR_LOW_KICK );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LCR_LOW_KICK,
			min( iSkillLevel + GetSkillLevelUpNum( CX2SkillTree::SI_A_LCR_LOW_KICK ), 10 ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013
#else
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LCR_LOW_KICK, iSkillLevel );
#endif

		/// �ο�ű �޸� ȿ���� �̵�ű���� ����
		if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO11 ) && pSkillTemplet->m_vecBuffFactorPtr.size() > 1 )
				m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1] );
			else
				m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
		}
	}
}
#endif //UPGRADE_SKILL_SYSTEM_2013

void CX2GULire_ElvenRanger::LESI_SA_LCR_LOW_KICK_FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_LOW_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}




void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Violent_Attack_Mesh01.X" );
	XSkinMeshReadyInBackground( L"AssaultKick03.X" );
	XSkinMeshReadyInBackground( L"SI_SA_LCR_VIOLENT_ATTACK_Effect_Wave_Bottom.X" );	
	TextureReadyInBackground( L"GroundShockWave02.dds" );
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_StateStart()
{
	CommonStateStart();

	// to do.

}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 2 );

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_Hyper_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 2 );

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.8f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.83f)
	{
		m_PhysicParam.nowSpeed.y = 1000.f;
		m_PhysicParam.passiveSpeed.x = 500.f;
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.2f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 1.23f)
	{
		m_PhysicParam.nowSpeed.y = 1100.f;
		m_PhysicParam.passiveSpeed.x = 600.f;
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.533f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 1.566f)
	{
		m_PhysicParam.nowSpeed.y = 1200.f;
		m_PhysicParam.nowSpeed.x = 0.f;
		m_PhysicParam.passiveSpeed.x = 0.f;

	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_SA_LCR_VIOLENT_ATTACK );
	}
	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_READY_StateEnd()
{

	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_Init()
{
	XSkinMeshReadyInBackground( L"SI_SA_LCR_VIOLENT_ATTACK_Effect_Wave_Rotate01.X" );
	XSkinMeshReadyInBackground( L"SI_SA_LCR_VIOLENT_ATTACK_Effect_Wave_Rotate02.X" );
	XSkinMeshReadyInBackground( L"SI_SA_LCR_VIOLENT_ATTACK_Effect_Wave_Rotate03.X" );
	XSkinMeshReadyInBackground( L"SI_SA_LCR_VIOLENT_ATTACK_Effect_Wave_Rotate_Guide.X" );
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_EventProcess()
{		
	if( true == IsOnSomethingFuture() )
		StateChange( LESI_SA_LCR_VIOLENT_ATTACK_LANDING );

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_StateEnd()
{
	CommonStateEnd();
}


void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Init()
{
	XSkinMeshReadyInBackground( L"SI_SA_LCR_VIOLENT_ATTACK_Effect_Dust.X" );
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_LANDING_FrameMove()
{

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_LANDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.733f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Hyper_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LCR_VIOLENT_ATTACK_LANDING_StateEnd()
{

	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_Init()
{
	TextureReadyInBackground( L"Gungnir01.tga" );

	XSkinMeshReadyInBackground( L"Lire_SI_SA_Gungnir_Mesh05.X" );
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Gungnir_Mesh00.X" );
	XSkinMeshReadyInBackground( L"Wind_Liner01.X" );

	XMeshReadyInBackground( L"Lire_FeatherA.Y" );
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	
#ifdef BALANCE_PATCH_20110303
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.1f ) == true && EventCheck( 2.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{

		m_bDisableGravity = false;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LSR_GUNGNIR );
		if( NULL != pSkillTemplet )
		{
			// kimhc // â�� ������ �����̶�� ��ȹ�� ������ �ڵ�
			const int	NUM_OF_SPEARS						= 8;		// â�� ����( ���� )
			const int	MAX_POS_Y							= 4000;		// �������� ���� �ִ� ���� 4000
			const int	MIN_POS_Y							= 1000;		// �������� ���� �ּ� ���� 1000
			const int	MAGIC_NUMBER_ARG					= 100;		// 
			const float MAGIC_NUMBER_NOT_CHECK_LAND_TIME	= 0.3f;
			const int	START_POS_X[NUM_OF_SPEARS]			= { -800, -500, -320, -90, 90, 320, 500, 800 };
			const int	RANDOM_GAP							= ( MAX_POS_Y - MIN_POS_Y ) / MAGIC_NUMBER_ARG;

			std::multimap< float, D3DXVECTOR3 > mapGungnirPos;		// �������� ���� Spear�� ��ġ
			D3DXVECTOR3 vStartPos( GetPos() );
			D3DXVECTOR3 vTempPos( 0.0f, 0.0f, 0.0f );

#ifdef CONVERSION_VS
			int i = 0;
			for( i = 0; i < NUM_OF_SPEARS; i++ )
#else CONVERSION_VS
			for ( int i = 0; i < NUM_OF_SPEARS; i++ )
#endif CONVERSION_VS
			{
				D3DXVECTOR3 vDirVector = GetDirVector();

				vTempPos = vStartPos + vDirVector * static_cast< float >( START_POS_X[i] );
				vTempPos.y += ( GetRandomInt( i * 2 + 3 ) % ( RANDOM_GAP ) ) * MAGIC_NUMBER_ARG + MIN_POS_Y;

				mapGungnirPos.insert( std::make_pair( vTempPos.y, vTempPos ) );
			}

			std::multimap< float, D3DXVECTOR3 >::iterator mItrStartPos = mapGungnirPos.begin();
			CX2DamageEffect::CEffect* pPEffectInDamage = NULL;

#ifdef ADD_MEMO_1ST_CLASS //��â��
			wstring wstrEffectName = L"RENA_GUNGNIR_SPEAR";

			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO21 ) == true )
				wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

			while ( mItrStartPos != mapGungnirPos.end() )
			{
#ifdef ADD_MEMO_1ST_CLASS //��â��
				pPEffectInDamage = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrEffectName.c_str(), GetPowerRate(), mItrStartPos->second, 
					GetRotateDegree(), GetRotateDegree(), vStartPos.y, true, GetRandomInt( i*4 + 3 ) / 100.f * 0.8f );
#else //ADD_MEMO_1ST_CLASS
				pPEffectInDamage = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RENA_GUNGNIR_SPEAR", GetPowerRate(), mItrStartPos->second, 
					GetRotateDegree(), GetRotateDegree(), vStartPos.y, true, GetRandomInt( i*4 + 3 ) / 100.f * 0.8f );
#endif //ADD_MEMO_1ST_CLASS


				if ( pPEffectInDamage != NULL )
				{
					float addNotCheckLandTime = ( ( mItrStartPos->second.y - vStartPos.y - MIN_POS_Y ) / 1000 ) * MAGIC_NUMBER_NOT_CHECK_LAND_TIME;

                    if ( CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pPEffectInDamage->GetMainEffect() )
                    {
					    pMeshInstance->SetNotCheckLandTime( 
						    pMeshInstance->GetNotCheckLandTime() +  addNotCheckLandTime );
                    }
					pPEffectInDamage = NULL;
				}
				mItrStartPos++;
			}
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_HYPER_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

#ifdef BALANCE_PATCH_20110303
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.1f ) == true && EventCheck( 2.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif
	{

		m_bDisableGravity = false;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LSR_GUNGNIR );
		if( NULL != pSkillTemplet )
		{
			// kimhc // â�� ������ �����̶�� ��ȹ�� ������ �ڵ�
			const int	NUM_OF_SPEARS						= 8;	// â�� ����( ���� )
			const int	MAX_POS_Y							= 4000;
			const int	MIN_POS_Y							= 1000;
			const int	MAGIC_NUMBER_ARG					= 100;
			const float MAGIC_NUMBER_NOT_CHECK_LAND_TIME	= 0.3f;
			const int	START_POS_X[NUM_OF_SPEARS]			= { -750, -450, -250, -75, 75, 250, 450, 750 };
			const int	RANDOM_GAP							= ( MAX_POS_Y - MIN_POS_Y ) / MAGIC_NUMBER_ARG;

			std::multimap< float, D3DXVECTOR3 > mapGungnirPos;		// �������� ���� Spear�� ��ġ
			D3DXVECTOR3 vStartPos( GetPos() );
			D3DXVECTOR3 vTempPos( 0.0f, 0.0f, 0.0f );

#ifdef CONVERSION_VS
			int i = 0;
			for( i = 0; i < NUM_OF_SPEARS; ++i )
#else CONVERSION_VS
			for ( int i = 0; i < NUM_OF_SPEARS; i++ )
#endif CONVERSION_VS
			{
				D3DXVECTOR3 vDirVector = GetDirVector();

				vTempPos = vStartPos + vDirVector * static_cast< float >( START_POS_X[i] );
				vTempPos.y += ( GetRandomInt( i * 2 + 3 ) % ( RANDOM_GAP ) ) * MAGIC_NUMBER_ARG + MIN_POS_Y;

				mapGungnirPos.insert( std::make_pair( vTempPos.y, vTempPos ) );
			}

			std::multimap< float, D3DXVECTOR3 >::iterator mItrStartPos = mapGungnirPos.begin();
			CX2DamageEffect::CEffect* pPEffectInDamage = NULL;

#ifdef ADD_MEMO_1ST_CLASS //��â��
			wstring wstrEffectName = L"RENA_GUNGNIR_SPEAR_HYPER";

			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO21 ) == true )
				wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

			while ( mItrStartPos != mapGungnirPos.end() )
			{
#ifdef ADD_MEMO_1ST_CLASS //��â��
				pPEffectInDamage = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrEffectName.c_str(), GetPowerRate(), mItrStartPos->second, 
					GetRotateDegree(), GetRotateDegree(), vStartPos.y, true, GetRandomInt( i*4 + 3 ) / 100.f * 0.8f );
#else //ADD_MEMO_1ST_CLASS
				pPEffectInDamage = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RENA_GUNGNIR_SPEAR_HYPER", GetPowerRate(), mItrStartPos->second, 
					GetRotateDegree(), GetRotateDegree(), vStartPos.y, true, GetRandomInt( i*4 + 3 ) / 100.f * 0.8f );
#endif //ADD_MEMO_1ST_CLASS

				if ( pPEffectInDamage != NULL )
				{
					float addNotCheckLandTime = ( ( mItrStartPos->second.y - vStartPos.y - MIN_POS_Y ) / 1000 ) * MAGIC_NUMBER_NOT_CHECK_LAND_TIME;

                    if ( CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pPEffectInDamage->GetMainEffect() )
                    {
					    pMeshInstance->SetNotCheckLandTime( 
						    pMeshInstance->GetNotCheckLandTime() +  addNotCheckLandTime );
                    }

					pPEffectInDamage = NULL;
				}
				mItrStartPos++;
			}
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LSR_GUNGNIR_HYPER_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}










//#ifdef RENA_SECOND_CLASS_CHANGE
void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZ_FrameMoveFuture()
{
	// ��� �ٲ�� ��
	//if( m_pXSkinAnimFuture->EventTimer( 0.43f ) == true && EventCheck( 0.43f, true ) == true )
	//{
	//	m_PhysicParam.nowSpeed.x = GetDashSpeed();
	//}

	CommonFrameMoveFuture();
}



void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZ_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_LWS_ZZfrontZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	//else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.78f && (m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true) ) { StateChangeDashIfPossible(); }
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.76f, 0.76f, m_InputData.oneZ == true, LESI_LWS_ZZfrontZZ )
	//ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.78f )
	//{	
	//	StateChange( LESI_WAIT );
	//}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}




void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZZ_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.3f ) == true && EventCheck( 0.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}


void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZZ_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		StateChange( USI_WAIT );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.83f && (m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true) ) { StateChangeDashIfPossible(); }
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_LWS_ZZfrontZZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	//CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	//else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.67f && (m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true) ) { StateChangeDashIfPossible(); }
	//ELSE_IF_STATE_CHANGE_ON_( 0, 0.53f, 0.53f, m_InputData.oneZ == true, LESI_LWS_ZZfrontZZZ )
	//ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.67f )
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.83f && (m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true) ) { StateChangeDashIfPossible(); }
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.6f, 0.6f, m_InputData.oneZ == true, LESI_LWS_ZZfrontZZZ )
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.83f )
	{
		StateChange( USI_WAIT );
	}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZZZ_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.54f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.54f ) == true && EventCheck( 0.54f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() + 400.f;
	}

	CommonFrameMoveFuture();
}


void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZZZ_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.516f &&
		false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( LESI_LWS_ZZfrontZZZ_Landing );
	}

	CommonEventProcess();
}


void CX2GULire_ElvenRanger::LESI_LWS_ZZfrontZZZ_Landing_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( 0.3f )
	{
		StateChange( USI_WAIT );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}








void CX2GULire_ElvenRanger::LESI_LWS_DASH_X_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneZ == true, LESI_LWS_DASH_XZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM

	float fChangeTime;
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",		fChangeTime,	0.0f );

	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	ELSE_IF_STATE_CHANGE_ON_( 1, fChangeTime, fChangeTime, m_InputData.oneZ == true, LESI_LWS_DASH_XZ )

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();

}


//void CX2GULire_ElvenRanger::LESI_LWS_DASH_XZ_FrameMoveFuture()
//{
//	if( m_pXSkinAnimFuture->EventTimer( 0.388f ) == true && EventCheck( 0.388f, true ) == true )
//	{
//		m_PhysicParam.nowSpeed.x = GetDashSpeed() + 600.f;
//	}
//
//	CommonFrameMoveFuture();
//}

void CX2GULire_ElvenRanger::LESI_LWS_DASH_XZ_EventProcess()
{
	//CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

#ifdef ADD_RENA_SYSTEM //��â��
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_LWS_DASH_XZZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.63f, 0.63f, m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.3f, LESI_LWS_DASH_XZZ )
	//else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.67f && 
	//	true == IsOnSomethingFuture() &&
	//	(m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true) ) 
	//{ 
	//	StateChangeDashIfPossible(); 
	//}
	//ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.67f )
	//{
	//	if( true == IsOnSomethingFuture() )
	//	{
	//		StateChange( LESI_WAIT );
	//	}
	//}

#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}




void CX2GULire_ElvenRanger::LESI_LWS_DASH_XZZ_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_WAIT );
		}
		else
		{
			StateChange( USI_JUMP_DOWN );
		}
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}


#ifdef BALANCE_GRAND_ARCHER_20121213
void CX2GULire_ElvenRanger::LESI_LGA_ZZX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 6.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_Small_Humming", (CX2GameUnit*) this );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );
		}
	}
	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_LGA_ZZX_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( LESI_SIEGE_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_LGA_ZZXX )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.67f, 0.47f, m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.3f , LESI_LGA_ZZXX )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		ELSE_IF_ZX_PRESSED_AFTER( m_fAttackCancelAfter )
	{
		StateChange( LESI_SIEGE_WAIT );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( LESI_SIEGE_WAIT );
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

#ifdef ADD_RENA_SYSTEM //��â��
void CX2GULire_ElvenRanger::LESI_LGA_ZZXX_StateStart()
{
	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	SetNowDamageRelateSkillData( 100 );

	CommonStateStart();
}
#endif //ADD_RENA_SYSTEM
void CX2GULire_ElvenRanger::LESI_LGA_ZZXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 18.f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();
			degree.y += 180.f;

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN_WIND_SNEAKER", GetPowerRate() * m_fAttackPowerRateArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}

			degree.z += 15.0f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN_WIND_SNEAKER", GetPowerRate() * m_fAttackPowerRateArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}


			degree.z -= 30.0f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN_WIND_SNEAKER", GetPowerRate() * m_fAttackPowerRateArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, fDegreeZ );
		}
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_LGA_ZZXX_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

#else
void CX2GULire_ElvenRanger::LESI_LGA_ZZZX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f ) == true )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_Small_Humming", (CX2GameUnit*) this );
		}
		else
		{
			//ShowPlayFailGuide( CX2Game::PGT_CANNOT_FIRE_ARROW );
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}




void CX2GULire_ElvenRanger::LESI_LGA_ZZZX_EventProcess()
{
	float fSkipTime = -1.f;
	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",	fSkipTime,		-1.f );


	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( m_CommonState.m_Wait );
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.67f, 0.47f, m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.3f , LESI_LGA_ZZZXX )
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( fSkipTime )
	{
		StateChange( m_CommonState.m_Wait );
	}

	CommonEventProcess();

}





void CX2GULire_ElvenRanger::LESI_LGA_ZZZXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 18.f ) == true )
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();
			degree.y += 180.f;

			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
			CX2DamageEffect::CEffect* pDE = NULL;



			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN_WIND_SNEAKER", GetPowerRate() * m_fAttackPowerRateArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}

			degree.z += 15.0f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN_WIND_SNEAKER", GetPowerRate() * m_fAttackPowerRateArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}


			degree.z -= 30.0f;
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_DOWN_WIND_SNEAKER", GetPowerRate() * m_fAttackPowerRateArrow, 
				posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}



			
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			float fDegreeZ = 0.f;
			LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", fDegreeZ, 0.0f );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, fDegreeZ );
		}
	}

	CommonFrameMove();
}


void CX2GULire_ElvenRanger::LESI_LGA_ZZZXX_EventProcess()
{
	//if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	//{
	//	StateChange( LESI_JUMP_DOWN );
	//}


	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

#endif //BALANCE_GRAND_ARCHER_20121213



void CX2GULire_ElvenRanger::LESI_LGA_DASH_JUMP_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 100.f, 0.25f, m_InputData.oneX == true, LESI_LGA_DASH_JUMP_ZX )

	CommonEventProcess();
}



void CX2GULire_ElvenRanger::LESI_LGA_DASH_JUMP_ZX_FrameMoveFuture()
{
	if( true == m_pXSkinAnimFuture->GetNowAnimationTime() > 0.72f )
	{
		m_PhysicParam.passiveSpeed.x = -1.f;
	}
	
	
	//if( m_pXSkinAnimFuture->EventTimer( 0.8f ) == true && EventCheck( 0.8f, true ) == true )
	//{
	//	m_PhysicParam.nowSpeed.y = -1500.f;
	//}



	CommonFrameMoveFuture();
}


void CX2GULire_ElvenRanger::LESI_LGA_DASH_JUMP_ZX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.492f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.492f ) == true && EventCheck( 0.492f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 6.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_Small_Humming_Down", (CX2GameUnit*) this );
		}
		else
		{
			//ShowPlayFailGuide( CX2Game::PGT_CANNOT_FIRE_ARROW );
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -90.f );

		}
	}

	CommonFrameMove();
}





void CX2GULire_ElvenRanger::LESI_LGA_DASH_JUMP_ZX_EventProcess()
{
	//if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	//{
	//	StateChange( LESI_JUMP_DOWN );
	//}


	//if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
	//	true == IsOnSomethingFuture() )
	if( true == m_pXSkinAnimFuture->GetNowAnimationTime() > 0.8f && 
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}









// ���� 2������ �ʻ��
// �����̵� ���� ű
void CX2GULire_ElvenRanger::LESI_SA_LWS_SLIDE_KICK_Init()
{
	XSkinMeshReadyInBackground( L"Slide_Ground_Wind01.X" );
	XSkinMeshReadyInBackground( L"Wind_Liner02.X" );
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Slide_Kick_Mesh03.X" );
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Slide_Kick_Mesh02.X" );

	TextureReadyInBackground( L"Ground_Wind_Effect02.tga" );
	TextureReadyInBackground( L"SlideKick_Impact01.tga" );
}



void CX2GULire_ElvenRanger::LESI_SA_LWS_SLIDE_KICK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.13f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.13f ) == true && EventCheck( 0.13f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_R_Foot" );
		if( true == GetIsRight() )
		{
			vBonePos += GetDirVector() * 50.f;
		}
		else 
		{
			vBonePos -= GetDirVector() * 50.f;
		}
		vBonePos.y += 20.f;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hDESlideKick = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pDESlideKick = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            this, L"RENA_SLIDE_KICK_SLIDING", GetPowerRate(), vBonePos, GetRotateDegree(), GetRotateDegree() );
	}


	if( m_pXSkinAnim->GetNowAnimationTime() > 0.13f )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pDESlideKick = g_pX2Game->GetDamageEffect()->GetInstance( m_hDESlideKick ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pDESlideKick = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pDESlideKick ) ? m_pDESlideKick : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{

			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_R_Foot" );
			if( true == GetIsRight() )
			{
				vBonePos += GetDirVector() * 50.f;
			}
			else 
			{
				vBonePos -= GetDirVector() * 50.f;
			}
			vBonePos.y += 20.f;

			pDESlideKick->SetPos( vBonePos );
		}

	}

	CommonFrameMove();
}



void CX2GULire_ElvenRanger::LESI_SA_LWS_SLIDE_KICK_EventProcess()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pDESlideKick = g_pX2Game->GetDamageEffect()->GetInstance( m_hDESlideKick );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pDESlideKick = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pDESlideKick ) ? m_pDESlideKick : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( pDESlideKick != NULL && pDESlideKick->GetDamageData().hitUnitList.size() > 0 )
	{
		StateChange( LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( LESI_SA_LWS_SLIDE_KICK_CANCEL );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_A_LWS_SLIDE_KICK );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_SA_LWS_SLIDE_KICK );
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillSlotData )
		{
			float fMPConsume = pSkillSlotData->m_fMPConsumption;
			UpNowMp( fMPConsume * 0.5f );


			if( NULL != g_pData->GetPicCharBlue() )
			{
				WCHAR wszText[64] = L"";
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", (int) (fMPConsume * 0.5f) );

				D3DXVECTOR3 pos = GetPos();
				pos.y += 50.f;

				g_pData->GetPicCharBlue()->DrawText( wszText, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
			}

		}
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LWS_SLIDE_KICK_StateEnd()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hDESlideKick = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pDESlideKick = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CommonStateEnd();

}






void CX2GULire_ElvenRanger::LESI_SA_LWS_SLIDE_KICK_CANCEL_EventProcess()
{

	float fSkipTime = -1.f;
	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",	fSkipTime,		-1.f );


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( fSkipTime )
	{
		StateChangeDashIfPossible();
	}

	CommonEventProcess();

}






void CX2GULire_ElvenRanger::LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH_EventProcess()
{

	float fChangeTime = 0.f;
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime,	0.1f );


	if( m_pXSkinAnimFuture->IsAnimationEnd() == true ||
		m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{	
		if( true == IsOnSomethingFuture() )
		{
			StateChange( LESI_SA_LWS_SLIDE_KICK_FINISH );
		}
		//else
		//{
		//	StateChange( LESI_JUMP_DOWN );
		//}
	}

	CommonEventProcess();
}





void CX2GULire_ElvenRanger::LESI_SA_LGA_FREEZING_ARROW_Init()
{
	XSkinMeshReadyInBackground( L"Freezing_Arrow01.X" );
	TextureReadyInBackground( L"Freezing_Arrow01.dds" );
}



void CX2GULire_ElvenRanger::LESI_SA_LGA_FREEZING_ARROW_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	CommonFrameMove();
}



void CX2GULire_ElvenRanger::LESI_SA_LGA_FREEZING_ARROW_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	CommonFrameMove();
}



void CX2GULire_ElvenRanger::LESI_A_LWS_NATURE_FORCE_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_A_Lws_Nature_Force_Mesh01.X" );

	XMeshReadyInBackground( L"Lire_Leaf01.Y" );
	TextureReadyInBackground( L"Lire_Leaf01.tga" );
}

void CX2GULire_ElvenRanger::LESI_A_LWS_NATURE_FORCE_StateStart()
{
	CommonStateStart();


	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LWS_NATURE_FORCE );
	if( NULL != pSkillTemplet )
	{
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		SetBuffFactorToGameUnit( pSkillTemplet, 1 );
	}
}

void CX2GULire_ElvenRanger::LESI_A_LWS_NATURE_FORCE_HYPER_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LWS_NATURE_FORCE );
	if( NULL != pSkillTemplet )
	{
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		SetBuffFactorToGameUnit( pSkillTemplet, 1 );
	}
}


void CX2GULire_ElvenRanger::LESI_A_LGA_STIGMA_ARROW_Init()
{
	TextureReadyInBackground( L"Death_Stigma01.dds" );
}

void CX2GULire_ElvenRanger::LESI_A_LGA_STIGMA_ARROW_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet
			= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LGA_STIGMA_ARROW );
		if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
		{
			D3DXVECTOR3 posR = GetBonePos( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE 
				= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RENA_STIGMA_ARROW", GetPowerRate() , 
				posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 
				m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex );

			if ( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if ( NULL == GetUnit() )
					return;

				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

				pDE->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
				pDE->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		}			
	}

	CommonFrameMove();
}


#ifdef NEW_SKILL_2010_11
//{{ oasis907 : ����� [2010.11.5] // ���� ����Ŀ - ���̷�����
void CX2GULire_ElvenRanger::LESI_SA_LWS_AIRELINNA_Init()
{
	TextureReadyInBackground( L"Moon_MagicSquare01.dds" );
}

void CX2GULire_ElvenRanger::LESI_SA_LWS_AIRELINNA_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_AIRELINNA", this );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7333f ) == true && EventCheck( 0.7333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LWS_AIRELINNA_NYMPH );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LWS_AIRELINNA );
#endif //UPGRADE_SKILL_SYSTEM_2013

		if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
		{
			D3DXVECTOR3 DamageEffectPos = GetPos();
			DamageEffectPos.y += (200.f * GetScaleByUnit().y) ;
			DamageEffectPos += ( GetIsRight() ? (120.f * GetDirVector() * GetScaleByUnit().x) : (-120.f * GetDirVector() * GetScaleByUnit().x) );

			CX2DamageEffect::CEffect* pDE 
				= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AIRELINNA_ATTACK_TWO", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree());
			if( NULL != pDE )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				if ( NULL == GetUnit()  )
				return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
				const float fEffectiveTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );	
	#else // UPGRADE_SKILL_SYSTEM_2013
				const float fEffectiveTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );	
	#endif // UPGRADE_SKILL_SYSTEM_2013		
                if ( CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pDE->GetMainEffect() )
				    pMeshInstance->SetMaxLifeTime(fEffectiveTime);
			}
		}
	}

	CommonFrameMove();
}

//}} oasis907 : ����� [2010.11.5] // ���� ����Ŀ - ���̷�����
#endif NEW_SKILL_2010_11


//{{ ����� 2010.11.2
#ifdef NEW_SKILL_2010_11

//LESI_SI3_LE_AERO_STRAFE
void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_Init()
{
	XSkinMeshReadyInBackground( L"AERO_STRAFE_Arrow.X" );
	XSkinMeshReadyInBackground( L"AERO_STRAFE_Light.X" );
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_FrameMoveFuture()
{
	CommonFrameMoveFuture();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (  m_pXSkinAnimFuture->EventTimerOneshot( 0.15f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (  m_pXSkinAnimFuture->EventTimer( 0.15f ) == true && EventCheck( 0.15f, true ) == true  )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = -GetDashSpeed()*3/2+100;
		m_PhysicParam.nowSpeed.y = 1400.f;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (  m_pXSkinAnimFuture->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (  m_pXSkinAnimFuture->EventTimer( 0.55f ) == true && EventCheck( 0.55f, true ) == true  )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed.x = -35.f;
		m_PhysicParam.nowSpeed.y = 0;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (  m_pXSkinAnimFuture->EventTimerOneshot( 0.95f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (  m_pXSkinAnimFuture->EventTimer( 0.95f ) == true && EventCheck( 0.95f, true ) == true  )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
		m_PhysicParam.nowSpeed.x = -1000.f;
		m_PhysicParam.nowSpeed.y = 800.f;
	}
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy2_Lhand" );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AERO_STRAFE", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
 		if( false == IsOnSomethingFuture() )
 			StateChange( USI_JUMP_DOWN );
 		else if ( true == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_StateEndFurtue()
{
	CommonStateEndFuture();
	// ������ // 2013-8-22
	// ��ų ���� �� �׶��Ƽ�� false�� �������� �ʰ� ����Ǿ��� �� �߻��ϴ� ���� �ذ��� ���� EndFurture �߰�
	m_bDisableGravity = false;
}

//LESI_SA_LGA_AERO_STRAFE_HYPER
void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_HYPER_Init()
{
	XSkinMeshReadyInBackground( L"AERO_STRAFE_Arrow.X" );
	XSkinMeshReadyInBackground( L"AERO_STRAFE_Light.X" );
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (  m_pXSkinAnimFuture->EventTimerOneshot( 0.15f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (  m_pXSkinAnimFuture->EventTimer( 0.15f ) == true && EventCheck( 0.15f, true ) == true  )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = -GetDashSpeed()*3/2+100;
		m_PhysicParam.nowSpeed.y = 1400.f;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (  m_pXSkinAnimFuture->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (  m_pXSkinAnimFuture->EventTimer( 0.55f ) == true && EventCheck( 0.55f, true ) == true  )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed.x = -35.f;
		m_PhysicParam.nowSpeed.y = 0;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (  m_pXSkinAnimFuture->EventTimerOneshot( 0.95f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (  m_pXSkinAnimFuture->EventTimer( 0.95f ) == true && EventCheck( 0.95f, true ) == true  )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
		m_PhysicParam.nowSpeed.x = -1000.f;
		m_PhysicParam.nowSpeed.y = 800.f;
	}
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.001f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy2_Lhand" );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AERO_STRAFE", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GULire_ElvenRanger::LESI_SA_LGA_AERO_STRAFE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else if ( true == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

#endif NEW_SKILL_2010_11
//}} ����� 2010.11.2

#ifdef	SERV_TRAPPING_RANGER_TEST

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
void	CX2GULire_ElvenRanger::StateInitTrapBlade()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	wstring wstrTRapBladeName;

	if( CX2Unit::UC_LIRE_TRAPPING_RANGER == GetUnit()->GetClass() )
		wstrTRapBladeName = L"Mesh_RTR_Lire_Sword";		/// Ʈ���� ������ ���� ������ ����
	else
		wstrTRapBladeName = L"Mesh_RNW_Lire_Sword";		/// ����Ʈ ��ó ���� ������ ����

	g_pX2Game->GetMajorXMeshPlayer()->AppendToDeviceList( AccessReadyDeviceList(), wstrTRapBladeName );
#else	SERV_RENA_NIGHT_WATCHER
	g_pX2Game->GetMajorXMeshPlayer()->AppendToDeviceList( AccessReadyDeviceList(), L"Mesh_RTR_Lire_Sword" );
#endif	SERV_RENA_NIGHT_WATCHER
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

void CX2GULire_ElvenRanger::CreateTrapBlade()
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	TextureReady( L"Rena_SlashTrace_Texture_WindSneaker.dds" );		//	Slash Trace Texture
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if( m_hTrapBlade != INVALID_MESH_INSTANCE_HANDLE )
	{
		DeleteTrapBlade();
	}

#ifdef SERV_RENA_NIGHT_WATCHER
	wstring wstrTRapBladeName;

	if( CX2Unit::UC_LIRE_TRAPPING_RANGER == GetUnit()->GetClass() )
		wstrTRapBladeName = L"Mesh_RTR_Lire_Sword";		/// Ʈ���� ������ ���� ������ ����
	else
		wstrTRapBladeName = L"Mesh_RNW_Lire_Sword";		/// ����Ʈ ��ó ���� ������ ����

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  wstrTRapBladeName.c_str(), GetPos(), GetRotateDegree(), GetRotateDegree() );
#else  SERV_RENA_NIGHT_WATCHER
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Mesh_RTR_Lire_Sword", GetPos(), GetRotateDegree(), GetRotateDegree() );
#endif SERV_RENA_NIGHT_WATCHER

	if( pMeshInst != NULL )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		m_hTrapBlade = pMeshInst->GetHandle();
		pMeshInst->SetBoundingRadius( 0.0f );

		CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
		pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
		pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );

		SetSubAttackList( pMeshInst->GetXSkinAnim() );

		m_bUseTrapBlade = true;
	}
}

void CX2GULire_ElvenRanger::DeleteTrapBlade()
{
	m_SubAttackListSet.clear();

	if( INVALID_MESH_INSTANCE_HANDLE != m_hTrapBlade )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hTrapBlade );
	}

	m_bUseTrapBlade = false;
}

// LESI_COMBO_RTR_ZZZ
void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_FrameMove()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	//ELSE_IF_STATE_CHANGE_ON_( 0, fChangeTime, fChangeTime, m_InputData.oneZ == true && m_InputData.pureDown == true, LESI_COMBO_RTR_ZZZ_DZ )
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ == true && m_InputData.pureUp == true, LESI_COMBO_Z4UP )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, m_InputData.oneZ == true, LESI_COMBO_RTR_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange, m_InputData.oneX == true, LESI_COMBO_Z4a )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

// LESI_COMBO_RTR_ZZZ_DZ
void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_DZ_StateStart()
{
	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_DZ_FrameMove()
{
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_DZ_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime,		0.0f );
	//CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( fSkipTime )
	{
		StateChange( USI_WAIT );
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZ_DZ_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

// LESI_COMBO_RTR_ZZZZ
void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZ_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZ_FrameMove()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZ_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ == true, LESI_COMBO_RTR_ZZZZZ )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZ_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

// LESI_COMBO_RTR_ZZZZZ
void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZZ_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}
	m_bActivePriorPlannedBlowWalkCancelChange = false;
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) == true && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );

#ifdef SERV_RENA_NIGHT_WATCHER
			if( true == m_bUsePriorPlannedBlow )			/// ���� �ϰ� �нú� ������
			{
				if( true == m_bActivePriorPlannedBlow )		/// ���� �ϰ� ��� ������ ������ ��
				{
					/// ���� �ϰ� ����� �ٿ� ��ġ �� ������ ���� ����
					m_DamageData.reActType			=  CX2DamageManager::RT_BIG_DAMAGE;
					m_DamageData.fForceDownValue	-= ( m_fPriorPlannedBlowDownValue * (m_iPriorPlannedBlowHitCount - _CONST_RENA_::MIN_PRIOR_PLANNED_BLOW_ACTIVE_NUM ) );

					m_bActivePriorPlannedBlowAttack = true;	/// �ش� Ÿ���� ���� �ϰ��� ����� Ÿ��( ������ Ÿ�� )
					m_bActivePriorPlannedBlowWalkCancelChange = true;
				}
			}
#endif SERV_RENA_NIGHT_WATCHER

			m_LuaManager.EndTable();		
		}

#ifdef SERV_RENA_NIGHT_WATCHER
		if( true == m_bUsePriorPlannedBlow )
		{
			if( false == m_bActivePriorPlannedBlowAttack )	/// ���� �ϰ� ��� ������ ���°� �ƴ� ��, �޺��� ����
				m_bPossibleHitCount = true;
		}
#endif SERV_RENA_NIGHT_WATCHER
	}

#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZZ_EventProcess()
{
	//CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bActivePriorPlannedBlowWalkCancelChange )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.3f && ( true == m_InputData.pureRight || true == m_InputData.pureLeft ) )
		{ 
			StateChange( USI_WAIT ); 
		}
	}
#endif SERV_RENA_NIGHT_WATCHER

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_COMBO_RTR_ZZZZZ_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

// LESI_JUMP_ATTACK_RTR_Z
void CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_RTR_Z_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_RTR_Z_FrameMove()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_RTR_Z_FrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( m_InputData.pureDown == true && m_InputData.pureUp == false )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}


void CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_RTR_Z_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true
		&& true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_DOWN );
	}
	else if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_RTR_Z_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

// LESI_DASH_COMBO_RTR_ZX
void CX2GULire_ElvenRanger::LESI_DASH_COMBO_RTR_ZX_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnit()->GetClass() && true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;				//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;			//���� �ϰ��� ����� Ÿ�� �к�
	}
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_DASH_COMBO_RTR_ZX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.29f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.29f ) == true && EventCheck( 1.29f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		DeleteTrapBlade();
#ifdef SERV_RENA_NIGHT_WATCHER
		if( CX2Unit::UC_LIRE_NIGHT_WATCHER == GetUnit()->GetClass() && true == m_bUsePriorPlannedBlow )
			m_bPossibleHitCount = false;					//���� �ϰ� Count ����
#endif SERV_RENA_NIGHT_WATCHER
	}

#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_DASH_COMBO_RTR_ZX_EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime, 0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime, 0.0f );

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > fSkipTime )
		{
			StateChange( USI_WAIT );
		}
	}
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_DASH_COMBO_RTR_ZX_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

void CX2GULire_ElvenRanger::LESI_DASH_COMBO_RTR_ZX_StateEndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight				= !(m_FrameDataFuture.syncData.bIsRight);
	m_FrameDataFuture.unitCondition.dirDegree.y		+= 180.0f;
}

// LESI_DASH_JUMP_ATTACK_RTR_Z
void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_Z_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_Z_FrameMove()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}
#ifdef ADD_RENA_SYSTEM //��â��
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneZ == true, LESI_DASH_JUMP_ATTACK_RTR_ZZ )
#else //ADD_RENA_SYSTEM
	ELSE_IF_STATE_CHANGE_ON_( 0, 100.f, 0.25f, m_InputData.oneZ == true, LESI_DASH_JUMP_ATTACK_RTR_ZZ )
#endif //ADD_RENA_SYSTEM

		CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_Z_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

// LESI_DASH_JUMP_ATTACK_RTR_ZZ
void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_ZZ_StateStart()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( GetUnitClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER && true == m_bUsePriorPlannedBlow )
	{
		m_bPossibleHitCount				= true;		//���� �ϰ� �޺� ���� �� ���� ���� ����
		m_bActivePriorPlannedBlowAttack = false;	//���� �ϰ��� ����� Ÿ�� �к�
	}
#endif SERV_RENA_NIGHT_WATCHER

	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_ZZ_FrameMove()
{
#ifdef SERV_RENA_NIGHT_WATCHER
	if( true == m_bUsePriorPlannedBlow )
		CheckPriorPlannedBlow();
#endif SERV_RENA_NIGHT_WATCHER

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_ZZ_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( LESI_DASH_JUMP_ATTACK_Z_LANDING );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_RTR_ZZ_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_SUCCESS_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_FAIL_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


void CX2GULire_ElvenRanger::LESI_SI_A_LE_RISING_FALCON_LANDING_EventProcess()
{
	float fSkipTime;
	float fChangeTime;

	LUA_GET_VALUE( m_LuaManager, "SKIP_TIME",		fSkipTime,		0.0f );
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",		fChangeTime,	0.0f );

	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( fSkipTime )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

// LESI_SA_RTR_EVOKE
void CX2GULire_ElvenRanger::LESI_SA_RTR_EVOKE_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"rena_A_evoke_Dummy.X" );
	XSkinMeshReadyInBackground( L"rena_A_explosionTrap_circle.X" );
	TextureReadyInBackground( L"rena_A_evoke_energy.dds" );
	TextureReadyInBackground( L"rena_A_trapingArrow_circle.dds" );
	TextureReadyInBackground( L"rana_A_explosionTrap_leaf.dds" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"rena_A_evoke_Dummy.X" );
	XSkinMeshReady( L"rena_A_explosionTrap_circle.X" );
	TextureReady( L"rena_A_evoke_energy.dds" );
	TextureReady( L"rena_A_trapingArrow_circle.dds" );
	TextureReady( L"rana_A_explosionTrap_leaf.dds" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_EVOKE_StateStart()
{
	CreateTrapBlade();
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_EVOKE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const int MAGIC_MAX_EVOKE_COUNT	= 1;	//	�������� �̺�ũ ����

		CreateEvoke( MAGIC_MAX_EVOKE_COUNT );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_EVOKE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef REGULATE_EVOKE_HYPER_MODE
		const int MAGIC_MAX_EVOKE_COUNT	= 1;	//	�������� �̺�ũ ����

		CreateEvoke( MAGIC_MAX_EVOKE_COUNT );
#else  REGULATE_EVOKE_HYPER_MODE
		// ���ο� ���� ��ȯ
		if( true == g_pX2Game->IsHost() )
		{
			D3DXVECTOR3 vPos = GetPos();
			if( true == GetIsRight() )
			{
				vPos += GetDirVector() * 100.f;
			}
			else
			{
				vPos -= GetDirVector() * 100.f;
			}

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO14 ) == true )
			{
	#ifdef	SERV_INSERT_GLOBAL_SERVER
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO, 1, true, vPos, 
					GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO, 1, true, vPos, 
					GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
			}
			else
			{
	#ifdef	SERV_INSERT_GLOBAL_SERVER
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 1, true, vPos, 
					GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 1, true, vPos, 
					GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
			}
#else
	#ifdef	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 1, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 1, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
#endif //ADDITIONAL_MEMO
		}
#endif REGULATE_EVOKE_HYPER_MODE
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_EVOKE_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}



//	LESI_A_RTR_EXPLOSION_TRAP
void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_Init()
{
}
void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed.x = 0.f;
	m_PhysicParam.nowSpeed.y = 0.f;
}
void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_Start()
{
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.66f )
	{			
		m_bDisableGravity = false;
	}

	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_FrameMove()
{
	D3DXVECTOR3 pos;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fPowerRate = GetPowerRate();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )		
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{	
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY );	//	��ɲ��� ���

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013

		pos = GetPos();
		pos.y += 200.f;

		//int iRandomSeed = GetRandomInt() % 8;	// �� 3�� ��ġ�� ���� 3��Ʈ ���

		//int iIsRealTrap1 = iRandomSeed & 1;		// 0x0001
		//int iIsRealTrap2 = iRandomSeed & 2;		// 0x0010
		//int iIsRealTrap3 = iRandomSeed & 4;		// 0x0100

		int iTrapRate = GetRandomInt() % 10;
#ifdef ADDITIONAL_MEMO
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO13) == true )
			iTrapRate = 9;
#endif

		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

		//switch( m_sExplosionTrapData.m_ChargeRank )
		//{
		//case CR_RANK_3:
		//	{
		//		vRotateDegree.z = 45.f;
		//		if( m_FrameDataFuture.syncData.bIsRight == false )	vRotateDegree.z *= -1;

		//		g_pX2Game->GetDamageEffect()->CreateInstance( this, (iIsRealTrap3 != 0 ? L"EXPLOSION_TRAP_FLY" : L"EXPLOSION_TRAP_FLY_FAKE"), GetPowerRate(), pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		//	}	//	CR_RANK_2 ����
		//case CR_RANK_2:
		//	{
		//		vRotateDegree.z = 15.f;
		//		if( m_FrameDataFuture.syncData.bIsRight == false )	vRotateDegree.z *= -1;

		//		g_pX2Game->GetDamageEffect()->CreateInstance( this, (iIsRealTrap2 != 0 ? L"EXPLOSION_TRAP_FLY" : L"EXPLOSION_TRAP_FLY_FAKE"), GetPowerRate(), pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		//	}	//	CR_RANK_1 ����
		//case CR_RANK_1:
		//	{

				//g_pX2Game->GetDamageEffect()->CreateInstance( this, (iIsRealTrap1 != 0 ? L"EXPLOSION_TRAP_FLY" : L"EXPLOSION_TRAP_FLY_FAKE"), fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#ifdef BALANCE_PATCH_20120329

#ifdef SERV_RENA_NIGHT_WATCHER
				if( true == GetActiveStartOfDelayedFiring() )	//������ ��ȣź�� ������ ��
				{
					CX2DamageEffect::CEffect* pDE = NULL;

					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, (iTrapRate >= 2 ? L"EXPLOSION_TRAP_FLY_DELAY" : L"EXPLOSION_TRAP_FLY_FAKE_DELAY"), fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );

					if( NULL != pDE )
					{
						pDE->SetIndex( CreateDamageEffectIndex() );	
						pDE->SetType( CX2DamageEffect::DET_EXPLOSION_TRAP );				//�ش� ����Ʈ�� Type�� �ο��Ѵ�.( ���� ���߿� ������ �� �νĿ� )
					}
				}
				else
#endif SERV_RENA_NIGHT_WATCHER
					g_pX2Game->GetDamageEffect()->CreateInstance( this, (iTrapRate >= 2 ? L"EXPLOSION_TRAP_FLY" : L"EXPLOSION_TRAP_FLY_FAKE"), fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#else
					g_pX2Game->GetDamageEffect()->CreateInstance( this, (iTrapRate >= 3 ? L"EXPLOSION_TRAP_FLY" : L"EXPLOSION_TRAP_FLY_FAKE"), fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
		//	} break;
		//}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_EventProcess()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_A_RTR_EXPLOSION_TRAP_EndFuture()
{
	m_bDisableGravity = false;
	CommonStateEndFuture();
}

//	LESI_SA_RTR_TRAPING_ARROW
void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPING_ARROW_StateStart()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectTrapArrow = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectTrapArrow = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPING_ARROW_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPING_ARROW_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

	bool bTrapDrop = false;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( INVALID_DAMAGE_EFFECT_HANDLE == m_hEffectTrapArrow )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL == m_pEffectTrapArrow )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			D3DXVECTOR3 posR = GetPos();
			D3DXVECTOR3 posHand = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			posR.y = posHand.y;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect*
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pEffectTrapArrow = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hEffectTrapArrow = ( m_pEffectTrapArrow != NULL ) ? m_pEffectTrapArrow->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_TRAPING_ARROW", this );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimerOneshot( 1.40f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.50f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.60f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.70f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.80f ) == true )	bTrapDrop = true;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.40f ) == true && EventCheck( 1.40f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.50f ) == true && EventCheck( 1.50f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.60f ) == true && EventCheck( 1.60f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.70f ) == true && EventCheck( 1.70f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.80f ) == true && EventCheck( 1.80f, false ) == true )	bTrapDrop = true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	if( bTrapDrop == true && 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hEffectTrapArrow != INVALID_DAMAGE_EFFECT_HANDLE
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_pEffectTrapArrow != NULL 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        )
	{
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

		float fPowerRate = GetPowerRate();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )		// ��ɲ��� ���, ���������� ���� �����ϵ��� �߰�
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY );	//	��ɲ��� ���

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013

		//	Ʈ�� ����
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffectTrapArrow = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectTrapArrow );
        CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = ( pEffectTrapArrow != NULL ) ? pEffectTrapArrow->GetMainEffect() : NULL;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = m_pEffectTrapArrow->GetMainEffect();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( pMainEffect != NULL )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
			if( true == GetActiveStartOfDelayedFiring() ) //������ ��ȣź�� ����� �������� üũ
			{
				CX2DamageEffect::CEffect* pDE = NULL;
#ifdef ADDITIONAL_MEMO
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO15 ) == true )
				{			
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_MEMO_DELAY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );				
				}
				else
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_DELAY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
				}
#else //ADDITIONAL_MEMO
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_DELAY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADDITIONAL_MEMO

				pDE->SetIndex( CreateDamageEffectIndex() );
				pDE->SetType( CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR );
			}
			else
			{
#ifdef ADDITIONAL_MEMO
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO15 ) == true )
				{			
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_MEMO", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );				
				}
				else
				{
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
				}
#else //ADDITIONAL_MEMO
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADDITIONAL_MEMO
			}
#else //UPGRADE_SKILL_SYSTEM_2013
#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO15 ) == true )
			{			
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_MEMO", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );				
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPING_ARROW_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

	bool bTrapDrop = false;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( m_hEffectTrapArrow == INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL == m_pEffectTrapArrow )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			D3DXVECTOR3 posR = GetPos();
			D3DXVECTOR3 posHand = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			posR.y = posHand.y;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hEffectTrapArrow = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pEffectTrapArrow = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                this, L"TRAPING_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_TRAPING_ARROW", this );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimerOneshot( 1.40f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.50f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.60f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.70f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.80f ) == true )	bTrapDrop = true;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.40f ) == true && EventCheck( 1.40f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.50f ) == true && EventCheck( 1.50f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.60f ) == true && EventCheck( 1.60f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.70f ) == true && EventCheck( 1.70f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.80f ) == true && EventCheck( 1.80f, false ) == true )	bTrapDrop = true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pEffectTrapArrow = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectTrapArrow );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pEffectTrapArrow = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pEffectTrapArrow ) ? m_pEffectTrapArrow : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( bTrapDrop == true && pEffectTrapArrow != NULL )
	{
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

		float fPowerRate = GetPowerRate();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY );	//	��ɲ��� ���

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013

		//	Ʈ�� ����
        if ( CKTDGXMeshPlayer::CXMeshInstance* pEffectTrapArrowMainEffect = pEffectTrapArrow->GetMainEffect() )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
			if( true == GetActiveStartOfDelayedFiring() ) //������ ��ȣź�� ����� �������� üũ
			{
				CX2DamageEffect::CEffect* pDE = NULL;
#ifdef ADDITIONAL_MEMO
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO15 ) == true )
				{			
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_MEMO_DELAY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );				
				}
				else
				{
					pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_DELAY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
				}
#else //ADDITIONAL_MEMO
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_DELAY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADDITIONAL_MEMO

				pDE->SetIndex( CreateDamageEffectIndex() );
				pDE->SetType( CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR );
			}
			else
			{
#ifdef ADDITIONAL_MEMO
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO15 ) == true )
				{				
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_MEMO", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
				}
				else
				{
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
				}
#else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
			}
#else //UPGRADE_SKILL_SYSTEM_2013
#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO15 ) == true )
			{				
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY_MEMO", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FLY", fPowerRate, pEffectTrapArrowMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPING_ARROW_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPING_ARROW_StateEnd()
{
	CommonStateEnd();

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectTrapArrow = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectTrapArrow = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

//	LESI_SA_RTR_KARMA
void CX2GULire_ElvenRanger::LESI_SA_RTR_KARMA_Init()
{
	//TextureReady( L"Gungnir01.tga" );
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_KARMA_StateStart()
{
	CommonStateStart();

#ifdef ADD_MEMO_1ST_CLASS //��â��
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO27 ) == true )
	{
		float fAniSpeed = m_pXSkinAnimFuture->GetPlaySpeed();
		m_pXSkinAnimFuture->SetPlaySpeed( fAniSpeed * 1.5f );
	}
#endif //ADD_MEMO_1ST_CLASS
	//m_bDisableGravity = true;
	//m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	//m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_KARMA_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//m_bDisableGravity = false;
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_KARMA_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//m_bDisableGravity = false;
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_KARMA_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_KARMA_StateEnd()
{
	//m_bDisableGravity = false;
	CommonStateEnd();
}

//	LESI_SA_RTR_FATALITY
void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_Init()
{
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_StateStart()
{
	CreateTrapBlade();
	CommonStateStart();

	//	������ ����
	SetFatalityData();
	m_NowStateData.m_fPowerRate *= m_sFatalityData.GetPowerRate();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_StateStartFuture()
{
	CommonStateStartFuture();

	m_bApplyMotionOffset = false;
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}


void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.03f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.04f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.04f ) == true && EventCheck( 0.04f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		wstring wstrEffectSetName = L"EffectSet_FATALITY_SPARK_FAIL";
		wstring wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_FAIL_DAMAGE";
		switch( m_sFatalityData.m_eRating )
		{
		case FRR_FAIL:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK_FAIL";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_FAIL_DAMAGE";
			}
			break;
		case FRR_POOR:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK_POOR";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_POOR_DAMAGE";
			}
			break;
		case FRR_NORMAL:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_DAMAGE";
			}
			break;
		case FRR_LUCKY:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK_LUCKY";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_LUCKY_DAMAGE";
			}
			break;
		}
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO22) == true )
			wstrEffectSetDamageName += L"_MEMO";

		CX2EffectSet::Handle hEffectHandle =
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetDamageName.c_str(), this, this );
		g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetName.c_str(), this, this );

#ifdef ADD_MEMO_1ST_CLASS //��â��
		m_vecEffectSetToDeleteOnStateEnd.push_back(hEffectHandle);
#endif //ADD_MEMO_1ST_CLASS
	}

	CommonFrameMove();

}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.03f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.04f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.04f ) == true && EventCheck( 0.04f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		wstring wstrEffectSetName = L"EffectSet_FATALITY_SPARK_FAIL";
		wstring wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_FAIL_DAMAGE";
		switch( m_sFatalityData.m_eRating )
		{
		case FRR_FAIL:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK_FAIL";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_FAIL_DAMAGE";
			}
			break;
		case FRR_POOR:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK_POOR";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_POOR_DAMAGE";
			}
			break;
		case FRR_NORMAL:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_DAMAGE";
			}
			break;
		case FRR_LUCKY:
			{
				wstrEffectSetName = L"EffectSet_FATALITY_SPARK_LUCKY";
				wstrEffectSetDamageName = L"EffectSet_FATALITY_SPARK_LUCKY_DAMAGE";
			}
			break;
		}
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO22) == true )
			wstrEffectSetDamageName += L"_MEMO";

		CX2EffectSet::Handle hEffectHandle =
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetDamageName.c_str(), this, this );
		g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetName.c_str(), this, this );

#ifdef ADD_MEMO_1ST_CLASS //��â��
		m_vecEffectSetToDeleteOnStateEnd.push_back(hEffectHandle);
#endif //ADD_MEMO_1ST_CLASS
	}

	CommonFrameMove();

}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		m_PhysicParam.passiveSpeed.x = 0.f;

		if( m_sFatalityData.m_eRating != FRR_FAIL )
			StateChange( LESI_SA_RTR_FATALITY_SUCCESS );
		else
			StateChange( LESI_SA_RTR_FATALITY_FAIL );
	}
	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_FATALITY_StateEndFuture()
{
	CommonStateEndFuture();

	m_bApplyMotionOffset = true;
}

//	LESI_SA_RTR_CALL_OF_RUIN
void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_Init()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectCallOfRuin = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectCallOfRuin = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	XSkinMeshReady( L"rena_A_ruin_obje.X" );
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_StateStart()
{
	CreateTrapBlade();
	CommonStateStart();

	//	������ ����
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}


void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.04f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.09f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.09f ) == true && EventCheck( 0.09f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 130.f;
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO26 ) == true )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_hEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			this, L"CALL_OF_RUIN_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		else
#endif //ADD_MEMO_1ST_CLASS
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            this, L"CALL_OF_RUIN", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//	Ÿ�� �ߴ�
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectCallOfRuin ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = m_pEffectCallOfRuin )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			pEffectCallOfRuin->SetAttackTime( D3DXVECTOR2(0.1f, 0.1f) );
		}

		//	���� �ӵ� ����
		m_PhysicParam.passiveSpeed.x = 400.f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.6f ) == true && EventCheck( 1.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 0.f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.3f ) == true && EventCheck( 2.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//	Ÿ�� �����
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectCallOfRuin ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = m_pEffectCallOfRuin )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			pEffectCallOfRuin->SetAttackTime( D3DXVECTOR2(2.3f, 2.5f) );
		}
	}

	CommonFrameMove();

}

void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.04f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.09f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.09f ) == true && EventCheck( 0.09f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 130.f;

#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO26 ) == true )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_hEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			this, L"CALL_OF_RUIN_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		else
#endif //ADD_MEMO_1ST_CLASS
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            this, L"CALL_OF_RUIN", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//	Ÿ�� �ߴ�
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectCallOfRuin ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = m_pEffectCallOfRuin )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			pEffectCallOfRuin->SetAttackTime( D3DXVECTOR2(0.1f, 0.1f) );
		}

		//	���� �ӵ� ����
		m_PhysicParam.passiveSpeed.x = 400.f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.6f ) == true && EventCheck( 1.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 0.f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.3f ) == true && EventCheck( 2.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//	Ÿ�� �����
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectCallOfRuin ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pEffectCallOfRuin = m_pEffectCallOfRuin )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			pEffectCallOfRuin->SetAttackTime( D3DXVECTOR2(2.3f, 2.5f) );
		}
	}

	CommonFrameMove();

}

void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_SA_RTR_CALL_OF_RUIN_StateEnd()
{
	CommonStateEnd();

	DeleteTrapBlade();

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectCallOfRuin = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectCallOfRuin = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

//	LESI_A_LE_RISING_FALCON
void CX2GULire_ElvenRanger::LESI_A_LE_RISING_FALCON_Init()
{
}

void CX2GULire_ElvenRanger::LESI_A_LE_RISING_FALCON_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_A_LE_RISING_FALCON_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) == true && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime2 ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime2 ) == true && EventCheck( m_fDamageDataChangeTime2, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_LAST" );
			m_LuaManager.EndTable();		
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_A_LE_RISING_FALCON_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true
		&& true == IsOnSomethingFuture() )
	{
		StateChange( LESI_SI_A_LE_RISING_FALCON_LANDING );
	}

	CommonEventProcess();
}

#ifdef SERV_RENA_NIGHT_WATCHER
//LESI_A_RNW_START_OF_DELAYED_FIRING
void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_Init()
{

}

void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_StateStart()
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LNW_DELAYED_FIRING );

	if ( NULL == GetUnit()  || NULL == pSkillTemplet )
		return;
	
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
	m_fStartOfDelayedFiringCoolTime = pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel );	/// ��Ÿ�� ����
#else //UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING );
	m_fStartOfDelayedFiringCoolTime = pSkillTemplet->m_fSkillCoolTime;		/// ��Ÿ�� ����
#endif //UPGRADE_SKILL_SYSTEM_2013

	m_fStartOfDelayedFiringPowerRate = GetPowerRate();
	SetActiveStartOfDelayedFiring( true );

	CommonStateStart();
}

void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_StateStartFuture()
{
	//m_bApplyMotionOffset = false;
	CommonStateStartFuture();
}

void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMoveFuture()
{
	m_bApplyMotionOffset = false;
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 degree = GetRotateDegree();
		D3DXVECTOR3 posR   = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );	/// ���� ������ �ؼ� ��ġ ������
		
// 		if( GetIsRight() == true )
// 			posR.x += 45.f;
// 		else
// 			posR.x -= 45.f;

		posR.y += 50.f;

		LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );
		CX2DamageEffect::CEffect* pDE = NULL;
		   
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"START_OF_DELAYED_FIRING_DELAY", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );

		if( NULL != pDE )
		{
			pDE->SetIndex( CreateDamageEffectIndex() );								/// �ش� ȭ�쿡 Index�� �ο��Ѵ�.( ���� ���� �� ������ )
			pDE->SetType( CX2DamageEffect::DET_START_OF_DELAYED_FIRING );			/// �ش� ȭ�쿡 Type�� �ο��Ѵ�. ( ������ ��ȣź )
			pDE->GetDamageData().m_ExtraDamage.m_iLevel = 1;						/// ������ ��ȣź �νĿ�
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_StateEnd()
{
	/// ������ ��ȣź ������ Ȯ�� ����Ʈ
	m_hActiveStartOfDelayedFiring = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Rena_Night_Watcher_ExplosionArrow_Active", (CX2GameUnit*) this );

	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();		/// ��ų ������ ����

	CommonStateEnd();
}

void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.f )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

//LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE
//void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_StateStart()
//{
//	CommonStateStart();
//}
//
//void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_FrameMove()
//{
//	if( m_pXSkinAnim->EventTimer( 0.05f ) == true && EventCheck( 0.05f, false ) == true )		/// ���� ������ �˷��ִ� ������ ����Ʈ
//	{
//		//D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
//
//		//g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"rana_explosion_Range", posR );
//	}
//
//	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
//	{
//		EntireExplosion();
//
//		g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiring );	/// ������ ��ȣź ������ Ȯ�� ����Ʈ
//
//		CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;
//		accessUserSkillTree.SetSkillCoolTimeLeft( CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING, m_fStartOfDelayedFiringCoolTime );
//
//		m_fStartOfDelayedFiringCoolTime = 0.f;
//
//		SetActiveStartOfDelayedFiring( false );		/// ������ ��ȣź ���� ����
//
//		g_pX2Game->UpdateSkillSlotUI();				/// ��ų ������ ����
//	}
//
//	CommonFrameMove();
//}
//
//void CX2GULire_ElvenRanger::LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_EventProcess()
//{
//	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
//	{
//		if( false == IsOnSomethingFuture() )
//			StateChange( USI_JUMP_DOWN );
//		else
//			StateChange( USI_WAIT );
//	}
//	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
//
//	CommonEventProcess();
//}

//LESI_SA_RNW_GLIDING_STRIKE
void CX2GULire_ElvenRanger::LESI_SA_RNW_GLIDING_STRIKE_Init()
{

}

void CX2GULire_ElvenRanger::LESI_SA_RNW_GLIDING_STRIKE_StateStartFuture()
{	
	CommonStateStartFuture();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_GLIDING_STRIKE_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_GLIDING_STRIKE_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );

	D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
	D3DXVECTOR3 posL = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
	D3DXVECTOR3 degree = GetRotateDegree();

	D3DXVECTOR3 posRorg = posR;
	D3DXVECTOR3 posLorg = posL;

	LUA_GET_VALUE( m_LuaManager, "ARROW_DEGREE_Z", degree.z, 0.0f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vOriginPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		D3DXVECTOR3 vEffectPos = vOriginPos;
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -10.f );
		if(GetIsRight() == true)
			vEffectPos += (10.f * GetDirVector() );
		else
			vEffectPos -= (10.f * GetDirVector() );
		vRotateDegree.z = -50;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_DUMMY_01", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );

		vEffectPos = vOriginPos;
		vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -11.f );
		if(GetIsRight() == true)
			vEffectPos += (14.f * GetDirVector() );
		else
			vEffectPos -= (14.f * GetDirVector() );
		vRotateDegree.z = -67.5;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_DUMMY_01", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );

		vEffectPos = vOriginPos;
		vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -10.f );
		if(GetIsRight() == true)
			vEffectPos += (10.f * GetDirVector() );
		else
			vEffectPos -= (10.f * GetDirVector() );
		vRotateDegree.z = -77.5;

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_DUMMY_02", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );

		vEffectPos = vOriginPos;
		vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -11.f );
		if(GetIsRight() == true)
			vEffectPos += (14.f * GetDirVector() );
		else
			vEffectPos -= (14.f * GetDirVector() );
		vRotateDegree.z = -87.5;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_DUMMY_02", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );

		vEffectPos = vOriginPos;
		vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -11.f );
		if(GetIsRight() == true)
			vEffectPos += (-14.f * GetDirVector() );
		else
			vEffectPos -= (-14.f * GetDirVector() );
		vRotateDegree.z = -57.5;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_DUMMY_03", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );

		vEffectPos = vOriginPos;
		vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -11.f );
		if(GetIsRight() == true)
			vEffectPos += (-14.f * GetDirVector() );
		else
			vEffectPos -= (-14.f * GetDirVector() );
		vRotateDegree.z = -64;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_DUMMY_03", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );

		vEffectPos = vOriginPos;
		vRotateDegree = GetRotateDegree();
		vEffectPos.y += ( -11.f );
		if(GetIsRight() == true)
			vEffectPos += (-14.f * GetDirVector() );
		else
			vEffectPos -= (-14.f * GetDirVector() );
		vRotateDegree.z = -57.5;
		
		if( GetRemainHyperModeTime() > 0.f )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY_HYPER", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GLIDING_STRIKE_ARROW_FLY", GetPowerRate(), vEffectPos, vRotateDegree, vRotateDegree );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_GLIDING_STRIKE_StateEndFuture()
{
	CommonStateEndFuture();
	m_PhysicParam.fDownAccel	= 0.0f;
}

void CX2GULire_ElvenRanger::LESI_SA_RNW_GLIDING_STRIKE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_JUMP_LANDING );
	}
	CommonEventProcess();
}
#endif SERV_RENA_NIGHT_WATCHER

void CX2GULire_ElvenRanger::ProcessVPP()
{
	if( IsUseTrapBalde() == false )
		return;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LNW_VITALPOINT_PIERCING, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LNW_VITALPOINT_PIERCING );

		if( NULL != pSkillTemplet )
		{
			if( GetRandomFloat(CKTDXRandomNumbers::SRO_VITALPOINT_PIERCING) <= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel ) )
			{
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
				// ���� ��� ȸ�� ��� �߰�
				float fIncreaseMPValue = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_RETAKE_ABS, iSkillLevel );
				UpNowMp( fIncreaseMPValue );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			}
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_VITALPOINT_PIERCING );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_VITALPOINT_PIERCING, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			if( GetRandomFloat(CKTDXRandomNumbers::SRO_VITALPOINT_PIERCING) <= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE ) )
			{
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
}

void CX2GULire_ElvenRanger::SetFatalityData()
{
	m_sFatalityData.Init();

	float fRate[4];

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_LTR_FATALITY );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_SA_LTR_FATALITY );
		
		if( NULL != pSkillTemplet )
		{
			fRate[0] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_01_RATE, iSkillLevel );	//	Lucky
			fRate[1] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_02_RATE, iSkillLevel );	//	Normal
			fRate[2] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_03_RATE, iSkillLevel );	//	Poor
			fRate[3] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_04_RATE, iSkillLevel );	//	Fail

			float fValue = GetRandomFloat();
			float fSum = 0.f;
			m_sFatalityData.m_eRating = FRR_FAIL;

			for(int i = 0; i < ARRAY_SIZE(fRate); i++)
			{
				fSum += fRate[i];

				if( fSum > fValue )
				{
					m_sFatalityData.m_eRating = static_cast<FATALITY_RATING_RANK>( FRR_LUCKY + i );
					break;
				}
			}

			m_sFatalityData.m_fPowerRate = pSkillTemplet->GetSkillAbilityValue( static_cast<CX2SkillTree::SKILL_ABILITY_TYPE>( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_01 + 
				m_sFatalityData.m_eRating ), iSkillLevel );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_RTR_FATALITY );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_SA_RTR_FATALITY, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			fRate[0] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_01_RATE );	//	Lucky
			fRate[1] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_02_RATE );	//	Normal
			fRate[2] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_03_RATE );	//	Poor
			fRate[3] = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_04_RATE );	//	Fail

			float fValue = GetRandomFloat();
			float fSum = 0.f;
			m_sFatalityData.m_eRating = FRR_FAIL;

			for(int i = 0; i < ARRAY_SIZE(fRate); i++)
			{
				fSum += fRate[i];

				if( fSum > fValue )
				{
					m_sFatalityData.m_eRating = static_cast<FATALITY_RATING_RANK>( FRR_LUCKY + i );
					break;
				}
			}

			m_sFatalityData.m_fPowerRate = pSkillTemplet->GetSkillAbilityValue( static_cast<CX2SkillTree::SKILL_ABILITY_TYPE>( CX2SkillTree::SA_SKILLDAMAGE_MULTIPLE_01 + 
				m_sFatalityData.m_eRating ) );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
}

void CX2GULire_ElvenRanger::CreateEvoke( const int MAX_COUNT )
{
	if( MAX_COUNT <= 0 )
		return;

	// �̺�ũ �ִ� ��� ���ں��� ���� ��� ���� ó��
	int iSize = m_vecOptrEvoke.size();
	int i = 0;

	while( iSize - i >= MAX_COUNT )
	{
		CX2GameUnitoPtr pGameUnitEvoke = *( m_vecOptrEvoke.begin() );
		
		if ( null != pGameUnitEvoke )
			pGameUnitEvoke->SetNowHp( 0.0f );

		m_vecOptrEvoke.erase( m_vecOptrEvoke.begin() );
		i++;
	}

	if( true == g_pX2Game->IsHost() )
	{
		// ���ο� ���� ��ȯ
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * 100.f;
		}
		else
		{
			vPos -= GetDirVector() * 100.f;
		}

#ifdef REGULATE_EVOKE_HYPER_MODE
		int bNPCLevel = 0;		/// �Ϲ� ���´� ���� 0

		if( GetRemainHyperModeTime() > 0.f )
			bNPCLevel = 1;		/// ���� ������ ��, ������ 1�̴�.

	#ifdef ADDITIONAL_MEMO
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO14 ) == true )
		{
	#ifdef	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO, bNPCLevel, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_ALLY, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO, bNPCLevel, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
		}
		else
		{
	#ifdef	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, bNPCLevel, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_ALLY, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, bNPCLevel, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
		}
	#else //ADDITIONAL_MEMO
	#ifdef	SERV_INSERT_GLOBAL_SERVER
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, bNPCLevel, true, vPos, 
			GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, bNPCLevel, true, vPos, 
			GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
	#endif //ADDITIONAL_MEMO

#else  REGULATE_EVOKE_HYPER_MODE

	#ifdef ADDITIONAL_MEMO
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO14 ) == true )
		{
	#ifdef	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO, 0, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO, 0, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
		}
		else
		{
	#ifdef	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 0, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 0, true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
		}
	#else //ADDITIONAL_MEMO
	#ifdef	SERV_INSERT_GLOBAL_SERVER
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 0, true, vPos, 
			GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), false, (CX2Room::TEAM_NUM) GetTeam() );
	#else	SERV_INSERT_GLOBAL_SERVER
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_EVOKE_TRAP_LIRE, 0, true, vPos, 
			GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) CX2Room::TN_NONE, CX2NPCAI::NAT_NORMAL, GetUnitUID(), (CX2Room::TEAM_NUM) GetTeam() );
	#endif	SERV_INSERT_GLOBAL_SERVER
	#endif //ADDITIONAL_MEMO

#endif REGULATE_EVOKE_HYPER_MODE
	}
}

void CX2GULire_ElvenRanger::InsertEvokeNpc( CX2GameUnit* pGameUnitEvoke_ )
{
	m_vecOptrEvoke.push_back( CX2GameUnitoPtr(pGameUnitEvoke_) );
}

void CX2GULire_ElvenRanger::DeleteEvokeNPCUID( UidType iNPCUID )
{
	for( int i = 0; i < static_cast<int>( m_vecOptrEvoke.size() ); i++ )
	{
		if( m_vecOptrEvoke[i] != null && iNPCUID == m_vecOptrEvoke[i]->GetUnitUID() )
		{
			m_vecOptrEvoke.erase( m_vecOptrEvoke.begin() + i );
			i--;
		}
	}
}
#endif	SERV_TRAPPING_RANGER_TEST

//#endif RENA_SECOND_CLASS_CHANGE

/*virtual*/ void CX2GULire_ElvenRanger::DeleteMinorParticle()
{
	CX2GUUser::DeleteMinorParticle();

	DeleteLireMinorParticle();
}

void CX2GULire_ElvenRanger::ShowActiveSkillCutInAndLight( const WCHAR* szBoneName_, const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ /*= false */ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTimeToShow_ ) == true && EventCheck( fTimeToShow_, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( GetShowCutInAndChangeWorldColor() && GetShowActiveSkillShow() )
#ifdef SERV_APRIL_FOOLS_DAY
			( true == m_bIsFoolsDay )?
			ActiveSkillShow( szBoneName_, s_SkillCutInSetFoolsDay.fileName.c_str(), s_SkillCutInSetFoolsDay.vSize, s_SkillCutInSetFoolsDay.vPosition, IsHyperState(), bOnlyLight_ ) :
#endif //APRIL_FOOLS_DAY
			ActiveSkillShow( szBoneName_, s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].fileName.c_str(), s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].vSize, s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].vPosition, IsHyperState(), bOnlyLight_ );
	}
}
#ifdef BALANCE_PATCH_20120329
//LESI_COMBO_X2
void CX2GULire_ElvenRanger::SnipingRangerComboX2FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 1.5f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X2_SR", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.733f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.733f ) == true && EventCheck( 0.733f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 1.5f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X2_SR", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::SnipingRangerComboX2EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_COMBO_X3 )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else //ADD_RENA_SYSTEM
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true	&& m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
	{
		StateChange( LESI_COMBO_X3 );
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.9f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X3
void CX2GULire_ElvenRanger::SnipingRangerComboX3FrameMoveFuture()
{
#ifdef BALANCE_PATCH_20120329
	if(m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.5f )
#else
	if(m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.6f )
#endif
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}
void CX2GULire_ElvenRanger::SnipingRangerComboX3FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.45f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.45f ) == true && EventCheck( 0.45f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X3", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.933f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.933f ) == true && EventCheck( 0.933f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_X3", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::SnipingRangerComboX3EventProcess()
{
#ifdef ADD_RENA_SYSTEM //��â��
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false; //X4
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false; //X4up
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false; //X4down
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 1, 1, m_InputData.oneX == true && m_InputData.pureUp, LESI_COMBO_X4UP )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 2, 2, m_InputData.oneX == true && m_InputData.pureDown, LESI_COMBO_X4DOWN )
	ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( 0, 0, m_InputData.oneX == true, LESI_COMBO_X4 )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
#else //ADD_RENA_SYSTEM
	float fChangeTime = 0.f;
	LUA_GET_VALUE( m_LuaManager, "CHANGE_TIME",	fChangeTime,	0.0f );

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false; //X4
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false; //X4up
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false; //X4down
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X4 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X4UP );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeTime )
	{
		StateChange( LESI_COMBO_X4DOWN );
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneX == true && m_InputData.pureDown && 
		m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
	}
	else if( m_InputData.oneX == true && m_InputData.pureUp && 
		m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneX == true && 
		m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f && m_pXSkinAnimFuture->GetNowAnimationTime() < fChangeTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}	
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

//LESI_COMBO_X4
void CX2GULire_ElvenRanger::SnipingRangerComboX4FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_GRAND_ARCHER_20121213
		if( FlushMp( 3.f ) == true )
#else
		if( FlushMp( 4.f ) == true )
#endif //BALANCE_GRAND_ARCHER_20121213
		{
			//ȭ����
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			CX2DamageEffect::CEffect* pDE = NULL;

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARROW_LONG_SR_XXXX", GetPowerRate() * m_fAttackPowerRateArrow, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, m_SkillRelatedData.GetForceDownValueRateOfRenaArrowAttack(), m_fArrowRangeRate, m_iAddHitCountArrow, m_fHitGapArrow, m_iArrowMeshPlayerIndex, m_fHitAddMp );
			if( NULL != pDE )
			{
				pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11 
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				pDE->GetDamageData().m_fHitDamageEffectInvokeRate = m_ArrowData.m_fVibrationShootingInvokeRate;		
				pDE->GetDamageData().m_fHitDamageEffectDamageRate = m_ArrowData.m_fVibrationShootingDamageRate;	
#endif NEW_SKILL_2010_11
				pDE->SetDamageTime( pDE->GetDamageTime() + 1 );

#ifdef ADD_RENA_SYSTEM //��â��
				pDE->GetDamageData().m_NaturalForceType = CX2DamageManager::NFT_COMBO_BOMB;
#endif //ADD_RENA_SYSTEM
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::SnipingRangerComboX4EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( LESI_SIEGE_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
#endif //ADD_RENA_SYSTEM
#ifdef BALANCE_GRAND_ARCHER_20121213
	ELSE_IF_ZX_PRESSED_AFTER( m_fAttackCancelAfter )
	{
		StateChange( LESI_SIEGE_WAIT );
	}
#endif //BALANCE_GRAND_ARCHER_20121213
	CommonEventProcess();
}

void CX2GULire_ElvenRanger::SetSiegeModeCoolTime()
{
	if( m_bIsSiegeSkill == false )
		return;

	CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;	// ������ ��� ��ų Ʈ��		
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LE_SIEGEMODE );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if ( NULL == GetUnit()  )
			return;
	
		const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
		accessUserSkillTree.SetSkillCoolTimeLeft( CX2SkillTree::SI_A_LE_SIEGEMODE, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( CX2SkillTree::SI_A_LE_SIEGEMODE, pSkillTemplet->m_fSkillCoolTime );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
}
#endif


#ifdef SERV_RENA_NIGHT_WATCHER
void CX2GULire_ElvenRanger::SetPriorPlannedBlow()		/// ���� �ϰ� ���� �� ���� �Լ�
{
	m_fPriorPlannedBlowTime		=  10.0f;				/// �޺� ���� �ð� ����
	m_iPriorPlannedBlowHitCount += 1;					/// �޺� ����

	/// ���� �ϰ� �ߵ� �ּ� �޺� ������ �̻��̰�, �ش� �нú� ��ų�� �ߵ����� �ƴ� ��, ȿ�� �߻�
	if( _CONST_RENA_::MIN_PRIOR_PLANNED_BLOW_ACTIVE_NUM <= m_iPriorPlannedBlowHitCount )
	{
		InsertPriorPlannedBlowEffect( m_iPriorPlannedBlowHitCount );	/// ���� �ϰ� ����Ʈ ����

		m_bActivePriorPlannedBlow = true;	/// ���� �ϰ� ����

		if( _CONST_RENA_::MAX_PRIOR_PLANNED_BLOW_ACTIVE_NUM <= m_iPriorPlannedBlowHitCount )	/// �ִ� �Ѱ�ġ���� �� ���� �Ǿ��ٸ�, ����ġ ����
			m_iPriorPlannedBlowHitCount = _CONST_RENA_::MAX_PRIOR_PLANNED_BLOW_ACTIVE_NUM;
	}
}

void CX2GULire_ElvenRanger::EndPriorPlannedBlow()		/// ���� �ϰ� �ʱ�ȭ �� ���� �Լ�
{
	DeletePriorPlannedBlowEffect();						/// ����Ʈ ����

	m_bActivePriorPlannedBlow		= false;			/// ���� �ϰ� ����
	m_bActivePriorPlannedBlowAttack = false;			/// �ش� ������ ���� �ϰ� ����
	m_fPriorPlannedBlowTime			= 0.0f;				/// �޺� ���� �ð� �ʱ�ȭ
	m_iPriorPlannedBlowHitCount		= 0;				/// �޺� ���� �ʱ�ȭ
}

void CX2GULire_ElvenRanger::CheckPriorPlannedBlow()		/// ���� �ϰ� ���� �Ǵ� �Լ�
{
	if( true == GetAttackSuccess( false ) )
	{
		if( true == m_bActivePriorPlannedBlowAttack )	/// ���� �ϰ��� ����� Ÿ���� ��
		{
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			/// ���� �ϰ� ���� �� m_fPriorPlannedBlowIncreaseHPRate ( SA_HP_INTAKE_REL_DAMAGE ) �� Ƚ���� ���Ͽ� hp �� ȸ���Ѵ�.
			UpNowHp ( GetMaxHp() * ( m_fPriorPlannedBlowIncreaseHPRate * static_cast<float> ( m_iPriorPlannedBlowHitCount - _CONST_RENA_::MIN_PRIOR_PLANNED_BLOW_ACTIVE_NUM + 1 ) ) );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

			EndPriorPlannedBlow();						/// ���� �ϰ� �ʱ�ȭ
		}
		else if( true == m_bPossibleHitCount )			/// ���� �ϰ� ���� Count ������ Ÿ���� ��
		{
			m_bPossibleHitCount = false;				/// ���� ���� �� Count �� ��, �ش� ���ݿ� ���ؼ��� �翬�� ����( �ټ� ��ǥ ���� �� �ϳ��� Count �ǵ��� )

			SetPriorPlannedBlow();						/// ���� �ϰ� ����

			m_FrameDataNow.unitCondition.bHit = false;	/// ���� ���� �� �ʱ�ȭ
		}
	}
}

void CX2GULire_ElvenRanger::InsertPriorPlannedBlowEffect(  int iActiveNum  )	/// ���� �ϰ� ����Ʈ ���� �Լ�
{
	int iActiveLevel = 0;

	/// ���� �������� ���� ���� �ϰ� ����Ʈ ���� ����
	if( iActiveNum >= _CONST_RENA_::PRIOR_PLANNED_BLOW_ACTIVE_LEVEL_3 )			/// 20			���� 3
		iActiveLevel = 3;
	else if( iActiveNum >= _CONST_RENA_::PRIOR_PLANNED_BLOW_ACTIVE_LEVEL_2 )	/// 13 ~ 19		���� 2
		iActiveLevel = 2;
	else																		///  5 ~ 12		���� 1
		iActiveLevel = 1;

	if( iActiveLevel != m_iNowPriorPlannedBlowLevel )		/// ���� ������ ����Ʈ�� �����ؾ� �� ����Ʈ�� �ٸ� ��, ����
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hPriorPlannedBlowEffect );		/// ���� ����Ʈ ����

		WCHAR wszControlName[70] = L"";
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"EffectSet_Rena_Night_Watcher_Prior_Planned_Blow_Level_%d", iActiveLevel );

		m_hPriorPlannedBlowEffect = g_pX2Game->GetEffectSet()->PlayEffectSet( wszControlName, (CX2GameUnit*)this );

		m_iNowPriorPlannedBlowLevel = iActiveLevel;
	}
}

void CX2GULire_ElvenRanger::DeletePriorPlannedBlowEffect()		/// ���� �ϰ� ����Ʈ �Ҹ� �Լ�
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hPriorPlannedBlowEffect );		/// ���� ����Ʈ ����

	m_hPriorPlannedBlowEffect = INVALID_EFFECTSET_HANDLE;

	m_iNowPriorPlannedBlowLevel = 0;
}

void CX2GULire_ElvenRanger::UpdateStartOfDelayedFiring( bool bInsertUnit/*= true*/ )	/// �����ڰ� �߻��� ������ ��ȣź ���� ����
{
	vector<CX2DamageManager::StartOfDelayedFiringData*> vTempArrowList;
	int													iArrowByUnitNum	= 0;
	int													iArrowByGroundNum = 0;

	/// �����ڰ� �����Ų ��� ȭ�� ��ȯ
	if( false == CreateStartOfDelayedFiringDataVector( vTempArrowList, GetUnitIndex() ) )
		return;

	/// ȭ�� ��Ÿ�� ���� �� ȭ���� ����� ���� ����
	for( UINT i = 0; i < vTempArrowList.size(); ++i )
	{
		if( -1 == vTempArrowList[i]->m_iArrowIndex )
		{
			vTempArrowList[i]->m_fCoolTime = _CONST_RENA_::START_OF_DELAYED_FIRING_DELAY_ACTIVE_TIME;

#ifdef BALANCE_PATCH_20131107
			++iArrowByUnitNum;	/// ���ֿ� ����� ȭ��
#else //BALANCE_PATCH_20131107
			if( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY == vTempArrowList[i]->m_iType )
			{
				++iArrowByUnitNum;	/// ���ֿ� ����� ȭ��
			}
			else
			{
				vTempArrowList.erase( vTempArrowList.begin() + i );		/// ������ ��ȣź�� ��Ÿ�Ӹ� ���Ž�Ű��, ���� ���� ���꿡�� ���ܽ�Ų��.

				--i;
			}
#endif //BALANCE_PATCH_20131107

		}
		else
		{
			++iArrowByGroundNum;	/// ������ ����� ȭ��
		}
	}

	/// �߻�� ������� ����
	std::sort( vTempArrowList.begin(), vTempArrowList.end(), CX2DamageManager::StartOfDelayedFiringDataSort() );

	/// ȭ�� �߻� ���Ѽ� �ʰ��� ���� ���� �߻��� ȭ�� ����
	if( _CONST_RENA_::MAX_ARROW_OF_EXPLOSION < vTempArrowList.size() )
	{
		if( ( 1 >= iArrowByGroundNum && true != bInsertUnit ) || 
			( 0 == iArrowByGroundNum && true == bInsertUnit ) )		/// ������ ����� ȭ���� ���ٸ�, �������� ����� ȭ���� �����Ѵ�.
		{
			DeleteStartOfDelayedFiring( vTempArrowList, GetUnitIndex(), true );
		}
		else														/// ������ ����� ȭ���� �ִٸ�, �켱������ �����Ѵ�.
		{
			DeleteStartOfDelayedFiring( vTempArrowList, GetUnitIndex(), false );
		}
	}

	/// ȭ�� ���� ��ȣ �缳��
	SetStartOfDelayedFiringOrderNum( vTempArrowList );

	/// �ӽ� ���� ����
	for( UINT i = 0; i < vTempArrowList.size(); ++i )
	{
		if( -1 != vTempArrowList[i]->m_iArrowIndex )
		{
			CX2DamageManager::StartOfDelayedFiringData* pData = vTempArrowList[i];
			SAFE_DELETE( pData );
		}
	}

	vTempArrowList.erase( vTempArrowList.begin(), vTempArrowList.end() );
};

/// ���� ���߿� �߻��� ȭ�� ���� �Լ�
void CX2GULire_ElvenRanger::DeleteStartOfDelayedFiring( vector<CX2DamageManager::StartOfDelayedFiringData*>& vTempArrowList, int iAttackerIndex, bool bDeleteUnitArrow )
{
	for( UINT i = 0; i < vTempArrowList.size(); ++i )
	{
		/// ���ֿ� ����� ȭ�쿡 ���� ���� ����
		if( -1 == vTempArrowList[i]->m_iArrowIndex && true == bDeleteUnitArrow )
		{
            unsigned uUnitNum = g_pX2Game->GetUnitNum();
			const vector<CX2GameUnit*>& vecUnitList = g_pX2Game->GetUnitList();

			for( UINT j = 0; j < uUnitNum; ++j )								/// ���ֿ��� ����� ȭ�� ���� �˻�
			{
                CX2GameUnit* pGameUnit = g_pX2Game->GetUnit( j );
		        if( pGameUnit == NULL )
                    continue;

                vector<CX2DamageManager::StartOfDelayedFiringData*>& vecStart = pGameUnit->AccessStartOfDelayedFiringData();

				if( true == vecStart.empty() )		/// ���� ȭ�� ������ �����ϴ� �����϶��� �˻�
					continue;

				for( UINT k = 0; k < vecStart.size(); ++k )
				{
					int	iArrowOrderNum = vTempArrowList[i]->m_iArrowOrderNumber;		/// �ش� ȭ���� ���� ����

					CX2DamageManager::StartOfDelayedFiringData* pArrowData = vecStart[k];

					/// �߻��� ������ �߻� ���� ��ȣ�� ��ġ�ϴ� ȭ�쿡 ���� ����
					if( pArrowData != NULL && iAttackerIndex == pArrowData->m_iAttackerIndex && 
						iArrowOrderNum == pArrowData->m_iArrowOrderNumber )
					{
						vTempArrowList.erase( vTempArrowList.begin() + i );	/// �ӽ� ���� ����

						pArrowData->m_fCoolTime = 0.01f;

						return;
					}
				}
			}
		}

		/// ������ ����� ȭ�쿡 ���� ���� ����
		else if( -1 != vTempArrowList[i]->m_iArrowIndex && false == bDeleteUnitArrow )
		{
			CX2DamageEffect::CEffect* pCEffect = g_pX2Game->GetDamageEffect()->GetInstanceByIndex( vTempArrowList[i]->m_iArrowIndex );

			if( NULL != pCEffect )
			{
				if( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND == pCEffect->GetType() )
				{
					g_pX2Game->GetDamageEffect()->DestroyInstance( pCEffect );	/// ȭ�� ����Ʈ ���� ó��

					SAFE_DELETE( vTempArrowList[i] );						/// �ӽ� ���� ������ ������ ���� ȭ�� ��ü�� new�� ���������Ƿ�, ����

					vTempArrowList.erase( vTempArrowList.begin() + i );		/// �ӽ� ���� ����

					return;
				}
			}
		}
	}
}

/// �߻� ���� ����
void CX2GULire_ElvenRanger::SetStartOfDelayedFiringOrderNum( vector<CX2DamageManager::StartOfDelayedFiringData*>& vTempArrowList )
{
	for( UINT i = 0; i < vTempArrowList.size(); ++i )
	{
		if( -1 == vTempArrowList[i]->m_iArrowIndex )		/// ���ֿ� �������� ȭ��
		{
			vTempArrowList[i]->m_iArrowOrderNumber = i;
		}
		else												/// ������ �������� ȭ��
		{
			CX2DamageEffect::CEffect* pCEffect = g_pX2Game->GetDamageEffect()->GetInstanceByIndex( vTempArrowList[i]->m_iArrowIndex );

			if( NULL != pCEffect && CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND != pCEffect->GetType() )
				pCEffect->SetOrderNum( i );
		}
	}
}

/// ������ �ε����� �����Ų ��� ȭ�� ��ü�� ���Ϳ� �־ ��ȯ
bool CX2GULire_ElvenRanger::CreateStartOfDelayedFiringDataVector( OUT vector<CX2DamageManager::StartOfDelayedFiringData*>& vArrowData, int iAttackerIndex )
{
	/// �������� ����� ȭ�� �˻�
    unsigned    uNumUnit = g_pX2Game->GetUnitNum();

	for( UINT i = 0; i < uNumUnit; ++i )
	{
        CX2GameUnit* pGameUnit = g_pX2Game->GetUnit( i );
		/// ���� ȭ�� ������ �����ϴ� �����϶��� �˻�
		if( pGameUnit == NULL )
            continue;

        vector<CX2DamageManager::StartOfDelayedFiringData*>& vecStart = pGameUnit->AccessStartOfDelayedFiringData();

		if ( vecStart.empty() == true )
			continue;

		/// �ش� �����ڰ� �����Ų ȭ�� ���� ����
		for( UINT j = 0; j < vecStart.size(); ++j )
		{
            CX2DamageManager::StartOfDelayedFiringData* pArrowData = vecStart[j];
			if( pArrowData != NULL && iAttackerIndex == pArrowData->m_iAttackerIndex &&
				CX2DamageEffect::DET_ARROW_OF_EXPLOSION_READY != pArrowData->m_iType )	/// ��ü ���� ������� ȭ���� ����
			{
				vArrowData.push_back( pArrowData );
			}
		}
	}

	/// ������ ����� ȭ�� �˻�
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    auto CreateStartOfDelayedFiringDataVectorCB = [&vArrowData,iAttackerIndex]( CX2DamageEffect::CEffect& kEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	vector<CX2DamageEffect::CEffect*> vecDamageEffect = g_pX2Game->GetDamageEffect()->GetInstanceList();
	for( UINT i = 0; i < vecDamageEffect.size(); ++i )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( null == kEffect.GetDamageData().optrAttackerGameUnit )
		    return;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffect = vecDamageEffect[i];
		if( NULL == pEffect || 
			false == g_pX2Game->GetDamageEffect()->IsLiveInstance( pEffect ) ||
            null == pEffect->GetDamageData().optrAttackerGameUnit )
		    continue;
        CX2DamageEffect::CEffect& kEffect = *pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		/// �ش� �����ڰ� ���� �����Ų ������ ȭ�� ����
		if( iAttackerIndex == kEffect.GetDamageData().optrAttackerGameUnit->GetUnitIndex() && 
			CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND == kEffect.GetType() )
		{
			if( -1 == kEffect.GetOrderNum() )
            {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                return;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                continue;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            }

			CX2DamageManager::StartOfDelayedFiringData* pData = new CX2DamageManager::StartOfDelayedFiringData( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND, 
				iAttackerIndex, kEffect.GetOrderNum(), 
                kEffect.GetIndex(), 
                kEffect.GetDamageData().optrAttackerGameUnit->GetPowerRate() );

			vArrowData.push_back( pData );
		}
    };
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    g_pX2Game->GetDamageEffect()->ApplyFunctionToLiveInstances( CreateStartOfDelayedFiringDataVectorCB );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	if( true == vArrowData.empty() )
		return false;		/// �����ڰ� �����Ų ȭ���� ���ٸ�, ȭ�� ���� ������ ���� �ʴ´�.
	else
		return true;
}

/// ��ü ���� ��� ��ü ���� ���� ����
bool CX2GULire_ElvenRanger::CreateEntireExplosionDataVector( OUT vector<CX2DamageManager::StartOfDelayedFiringData*>& vArrowData )
{
    unsigned    uNumUnits = g_pX2Game->GetUnitNum();

	/// ���ֿ� ����� ȭ�� ���� �˻�
	for( UINT i = 0; i < uNumUnits; ++i )
	{
        CX2GameUnit* pGameUnit = g_pX2Game->GetUnit( i );
		if( pGameUnit == NULL )
            continue;

        vector<CX2DamageManager::StartOfDelayedFiringData*>& vecStart = pGameUnit->AccessStartOfDelayedFiringData();
		if ( true == vecStart.empty() )
			continue;

		//if( 9999.f >= GetDistance( GetPos(), vecUnitList[i]->GetPos() ) )		/// ���� �Ÿ� ����( ��ȹ���� ���ܵ� )
		{
			int			iCount		 = 0;			/// ���� ��ġ ������ �ε��� ī��Ʈ
			float		fCoolTime	 = 0.01f;		/// ���� ��ü ���� �ð� ����
			bool		bIsExplosion = false;		/// EDT �̸�Ƽ�� ���� ���� ����

			D3DXVECTOR3 vEffPos[] = 
			{ 
				D3DXVECTOR3(   0.f,   0.f,   0.f ),
				D3DXVECTOR3(  50.f,  50.f,  50.f ),
				D3DXVECTOR3( -55.f, -50.f, -55.f ),
				D3DXVECTOR3( -70.f,  60.f, -70.f ),
				D3DXVECTOR3(  50.f, -15.f, -50.f ),
				D3DXVECTOR3(  50.f,  50.f,  50.f ),
				D3DXVECTOR3( -55.f, -50.f, -55.f ),
				D3DXVECTOR3( -70.f,  60.f, -70.f ),
				D3DXVECTOR3(  50.f, -15.f,  50.f ),
			};

			for( UINT j = 0; j < vecStart.size(); ++j )		/// �����ڰ� ������ ȭ�� ���� �˻�
			{
				CX2DamageManager::StartOfDelayedFiringData* pArrowData = vecStart[j];

				if( GetUnitIndex() == pArrowData->m_iAttackerIndex )
				{
					if( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY == pArrowData->m_iType )
						pArrowData->m_iType = CX2DamageEffect::DET_ARROW_OF_EXPLOSION_READY;			/// ���� ��� Ÿ�� ����( ������ ȭ�� )
					else if( CX2DamageEffect::DET_START_OF_DELAYED_FIRING == pArrowData->m_iType )
						pArrowData->m_iType = CX2DamageEffect::DET_START_OF_DELAYED_FIRING_READY;	/// ���� ��� Ÿ�� ����( ������ ��ȣź )
					
					pArrowData->m_fCoolTime = fCoolTime;						/// ���� �ð� ����

					pArrowData->m_vExplosionOffSet = vEffPos[iCount];		/// ���� ��ġ ����
					++iCount;

					if( 8 < iCount )		/// �ִ� ���� ��ġ �ε��� ī��Ʈ ����
						iCount = 1;

					fCoolTime += 0.1f;											/// ���� ��ü ���� �ð� ����

					bIsExplosion = true;										/// EDT �̸�Ƽ�� ���� ���� ����

					vArrowData.push_back( pArrowData );						/// ���� ����Ʈ ���Ϳ� ����
				}
			}

			if( true == bIsExplosion )			/// ���ߵ� ������ �ִٸ�, EDT �̸�Ƽ�� ���� ó��
			{
				pGameUnit->SetStartOfDelayedFiringEmoticon();
			}
		}
	}

	/// ������ ����� ȭ�� �˻�
    int iUnitIndex = GetUnitIndex();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    auto CreateEntireExplosionDataVectorCB = [&vArrowData,iUnitIndex]( CX2DamageEffect::CEffect& kEffect ) 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	vector<CX2DamageEffect::CEffect*> vInstaceList = g_pX2Game->GetDamageEffect()->GetInstanceList();
	for( UINT i = 0; i < vInstaceList.size(); ++i )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( null == kEffect.GetDamageData().optrAttackerGameUnit )
            return;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffect = vInstaceList[i];
		if( NULL == pEffect || 
			false == g_pX2Game->GetDamageEffect()->IsLiveInstance( pEffect ) ||
            null == pEffect->GetDamageData().optrAttackerGameUnit )
		    continue;
        CX2DamageEffect::CEffect& kEffect = *pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


		switch( kEffect.GetType() )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		case CX2DamageEffect::DET_SEED_OF_RUIN_GROUND:					/// ������ ����� ������ ����
		case CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR_GROUND:			/// ������ ����� Ʈ���� �ַο� - ������
		case CX2DamageEffect::DET_TRAPPING_ARROW_FUNGUS_GROUND:			/// ������ ����� Ʈ���� �ַο� - ��Ž�
#endif //UPGRADE_SKILL_SYSTEM_2013
		case CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND:		/// ������ ����� ������ ȭ��
		case CX2DamageEffect::DET_START_OF_DELAYED_FIRING_GROUND:		/// ������ ����� ������ ��ȣź
		case CX2DamageEffect::DET_EXPLOSION_TRAP_GROUND:				/// ������ ����� ������ ��
			{
				/// �����ڰ� ��ġ�ϰ�, ���� �Ÿ� �ȿ� �ִ� ������ ����Ʈ �˻�
				if( iUnitIndex == kEffect.GetDamageData().optrAttackerGameUnit->GetUnitIndex() )
				{
					//if( 9999.f >= GetDistance( GetPos(), kEffect.GetPos() ) )			/// ���� �Ÿ� ����( ��ȹ���� ���ܵ� )
					{
						/// �ӽ� ���Ϳ� ���� ȭ�� ���� ����
						CX2DamageManager::StartOfDelayedFiringData* pData = new CX2DamageManager::StartOfDelayedFiringData( kEffect.GetType(), 
							iUnitIndex, kEffect.GetOrderNum(), 
                            kEffect.GetIndex(), 
                            0.f, kEffect.GetDamageData().damage.fPhysic );

						vArrowData.push_back( pData );
					}
				} 
			} break;
		}
    };
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    g_pX2Game->GetDamageEffect()->ApplyFunctionToLiveInstances( CreateEntireExplosionDataVectorCB );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	if( true == vArrowData.empty() )
		return false;
	else
		return true;
}

/// ������ ��ȣź ��ü ����
void CX2GULire_ElvenRanger::EntireExplosion()		/// ������ ��ȣź ��ü ����
{
	vector<CX2DamageManager::StartOfDelayedFiringData*> vExplosionArrowList;

	if( false == CreateEntireExplosionDataVector( vExplosionArrowList ) )		/// ���߽�ų ȭ�� ���� ����
		return;

	
#ifdef FIX_DELAYED_FIRING_DELAY_ADD_DAMAGE // ����ȯ
	/// ������ ����Ʈ ����ü �� ����
	int	iCountExplosionObjectNum = vExplosionArrowList.size();

	/// NPC ����ü �� ����
	BOOST_FOREACH( UidType iNPCUID, m_vecAngerOfElfTrapUID )
	{
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( static_cast<int>(iNPCUID) );

		/// ��������� �г� ( ������ ��ȣź ���� )
		if( NULL != pNPC && CX2UnitManager::NUI_RENA_ANGER_OF_ELF_DELAY == pNPC->GetNPCTemplet().m_nNPCUnitID )
			++iCountExplosionObjectNum;
	}

	if( 0 >= iCountExplosionObjectNum )		/// ���߽�ų ��ü�� ���ٸ� return
		return;

	/// �Ѱ�ġ ����
	if ( iCountExplosionObjectNum > _CONST_RENA_::START_OF_DELAYED_FIRING_DELAY_MAX_COUNT )
		iCountExplosionObjectNum = _CONST_RENA_::START_OF_DELAYED_FIRING_DELAY_MAX_COUNT;
#else // FIX_DELAYED_FIRING_DELAY_ADD_DAMAGE
	/// ����ü �� ����
	int	  iCountExplosionObjectNum = vExplosionArrowList.size();

	if( 0 >= iCountExplosionObjectNum )		/// ���߽�ų ��ü�� ���ٸ� return
		return;
#endif // FIX_DELAYED_FIRING_DELAY_ADD_DAMAGE

	/// ���ߵǴ� ���߹� ��ü�� * �ɸ��� ���� ���ݷ��� SA_DAMAGE_REL%
	float fIncreaseDamage	= (iCountExplosionObjectNum - 1) * m_fStartOfDelayedFiringIncreaseDamage;

	/// ������ ����Ʈ�� �� ���߹� ���� ����
	for( UINT i = 0; i < vExplosionArrowList.size(); ++i )
	{
		vExplosionArrowList[i]->m_fSkillPowerRate = m_fStartOfDelayedFiringPowerRate;

		if( -1 == vExplosionArrowList[i]->m_iArrowIndex )				/// ���ֿ� ����� ���� ����
		{
			vExplosionArrowList[i]->m_fIncreaseDamage = fIncreaseDamage;
		}
		else															/// ������ ����� ���� ����
		{
			CX2DamageEffect::CEffect* pCEffect = g_pX2Game->GetDamageEffect()->GetInstanceByIndex( vExplosionArrowList[i]->m_iArrowIndex );
			if( pCEffect !=	NULL && true == g_pX2Game->GetDamageEffect()->IsLiveInstance( pCEffect ) )
			{
				if( vExplosionArrowList[i]->m_iType == CX2DamageEffect::DET_EXPLOSION_TRAP_GROUND )		/// ������ ���� ��Ÿ ���� �̹Ƿ�, ������ ����
				{
					pCEffect->SetAddDieEffectDamage( fIncreaseDamage / 5 );
				}
				else
				{
					pCEffect->SetAddDieEffectDamage( fIncreaseDamage );	/// ���� ����Ʈ�� ���ݷ��� ������Ų��.
				}

				g_pX2Game->GetDamageEffect()->DestroyInstance( pCEffect );
			}
		}
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
	/// NPC�� ������ ��������� �г� ���� ����
	for( UINT i = 0; i <m_vecAngerOfElfTrapUID.size(); i++ )
	{
		UidType& npcUID = m_vecAngerOfElfTrapUID[i];
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( static_cast<int>(npcUID) );
		if( NULL != pNPC && CX2UnitManager::NUI_RENA_ANGER_OF_ELF_DELAY == pNPC->GetNPCTemplet().m_nNPCUnitID )
		{
			pNPC->SetNowHp( 1.f );

	#ifdef FIX_DELAYED_FIRING_DELAY_ADD_DAMAGE // ����ȯ
			/// ������ ��ȣź ��ø���� ���� ������ ����ġ ���� ( NPC��, PowerRate�� ���� )
			pNPC->SetDefaultPowerRate( pNPC->GetPowerRate() + fIncreaseDamage );
	#endif // FIX_DELAYED_FIRING_DELAY_ADD_DAMAGE

			m_vecAngerOfElfTrapUID.erase( m_vecAngerOfElfTrapUID.begin() + i );
			--i;
		}	
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

	/// �ӽ� ���� ����
	for( UINT i = 0; i < vExplosionArrowList.size(); ++i )
	{
		if( -1 != vExplosionArrowList[i]->m_iArrowIndex )
		{
			CX2DamageManager::StartOfDelayedFiringData* pData = vExplosionArrowList[i];
			SAFE_DELETE( pData );
		}
	}

	vExplosionArrowList.erase( vExplosionArrowList.begin(), vExplosionArrowList.end() );

}

void CX2GULire_ElvenRanger::ClearStartOfDelayedFiringData()		/// ��� Ȥ�� ��Ż��, �ش� ������ ������ ������ ��ȣź ���� ����
{
    unsigned    uNumUnit = g_pX2Game->GetUnitNum();

	/// ���ֿ� ����� ȭ�� ���� �˻�
	for( UINT i = 0; i < uNumUnit; ++i )
	{
        CX2GameUnit* pGameUnit = g_pX2Game->GetUnit( i );
		if( pGameUnit == NULL )
            continue;
        vector<CX2DamageManager::StartOfDelayedFiringData*>& vecStart = pGameUnit->AccessStartOfDelayedFiringData();
		if ( true == vecStart.empty() )
			continue;

		float fCoolTime	   = 0.01f;
		bool  bIsExplosion = false;

		for( UINT j = 0; j < vecStart.size(); ++j )		/// �����ڰ� ������ ȭ�� ���� �˻�
		{
            CX2DamageManager::StartOfDelayedFiringData* pArrowData = vecStart[j];
			if( pArrowData != NULL && GetUnitIndex() == pArrowData->m_iAttackerIndex )
			{
				pArrowData->m_fCoolTime = 0.01f;							/// ���� �ð� ����
			}
		}
	}

	/// ������ ����� ȭ�� �˻�
    int iUnitIndex = GetUnitIndex();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    auto ClearStartOfDelayedFiringDataCB = [iUnitIndex]( CX2DamageEffect::CEffect& kEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    std::vector<CX2DamageEffect::CEffectHandle>  vecHandle;
    auto ClearStartOfDelayedFiringDataCB = [&vecHandle,iUnitIndex]( CX2DamageEffect::CEffect& kEffect )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	vector<CX2DamageEffect::CEffect*> vInstaceList = g_pX2Game->GetDamageEffect()->GetInstanceList();
	for( UINT i = 0; i < vInstaceList.size(); ++i )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( null == kEffect.GetDamageData().optrAttackerGameUnit )
            return;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffect = vInstaceList[i];
		if( NULL == pEffect || 
			false == g_pX2Game->GetDamageEffect()->IsLiveInstance( pEffect ) ||
            null == pEffect->GetDamageData().optrAttackerGameUnit )
		    continue;
        CX2DamageEffect::CEffect& kEffect = *pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


		switch( kEffect.GetType() )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		case CX2DamageEffect::DET_SEED_OF_RUIN_GROUND:					/// ������ ����� ������ ����
		case CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR_GROUND:			/// ������ ����� Ʈ���� �ַο� - ������
		case CX2DamageEffect::DET_TRAPPING_ARROW_FUNGUS_GROUND:			/// ������ ����� Ʈ���� �ַο� - ��Ž�
#endif //UPGRADE_SKILL_SYSTEM_2013
		case CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND:		/// ������ ����� ������ ȭ��
		case CX2DamageEffect::DET_START_OF_DELAYED_FIRING_GROUND:		/// ������ ����� ������ ��ȣź
		case CX2DamageEffect::DET_EXPLOSION_TRAP_GROUND:				/// ������ ����� ������ ��
			{
				/// �����ڰ� ��ġ�ϰ�, ���� �Ÿ� �ȿ� �ִ� ������ ����Ʈ �˻�
				if( iUnitIndex == kEffect.GetDamageData().optrAttackerGameUnit->GetUnitIndex() )
				{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
                    g_pX2Game->GetDamageEffect()->DestroyInstanceHandleSilently( kEffect.GetHandle() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
                    vecHandle.push_back( kEffect.GetHandle() );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				} 
			} break;
		}
    };
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    g_pX2Game->GetDamageEffect()->ApplyFunctionToLiveInstances( ClearStartOfDelayedFiringDataCB );
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    BOOST_FOREACH( CX2DamageEffect::CEffectHandle handle, vecHandle )
    {
        g_pX2Game->GetDamageEffect()->DestroyInstanceHandleSilently( handle );
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

int CX2GULire_ElvenRanger::CreateDamageEffectIndex()	/// ������ ȭ�� DamageEffect�� Index ���� �Լ�
{
	int	iIndex = GetUnitIndex();						/// �߻��� Unit�� UnitIndex�� ����

	if( 100 <= m_iIndexNum )							/// 0 ~ 99
		m_iIndexNum = 0;

	++m_iIndexNum;

	iIndex = ( iIndex * 100 ) + m_iIndexNum;			/// 0 = 0 ~ 99,	1 = 100 ~ 199, ...

	return iIndex;
}
#endif SERV_RENA_NIGHT_WATCHER


void CX2GULire_ElvenRanger::UpNowMpPerHitOthers( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ )
{
#ifdef	BALANCE_PATCH_20120329
	const float fRel = ( false == IsNatureForce() ? fRel_ : fRel_ * 0.5f );

	switch ( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_PVP:
		{
			UpNowMp( ( fHitAddMp_ + (3.0f * iAddValueByTeam_) ) * fRel );

		} break;

	case CX2Game::GT_DUNGEON:
	case CX2Game::GT_BATTLE_FIELD:
		{
			UpNowMp( ( fHitAddMp_ + (3.0f * iAddValueByTeam_) ) * 0.8f * fRel );
		} break;

	default:
		ASSERT( !L"wrong path" );
		break;
	}
#else	BALANCE_PATCH_20120329
	if ( false == IsNatureForce() )
	{
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				UpNowMp( ( fHitAddMp_ + (3.0f * iAddValueByTeam_) ) * fRel_ );

			} break;

		case CX2Game::GT_DUNGEON:
		case CX2Game::GT_BATTLE_FIELD:
			{
				UpNowMp( ( fHitAddMp_ + (3.0f * iAddValueByTeam_) ) * 0.8f * fRel_ );
			} break;

		default:
			ASSERT( !L"wrong path" );
			break;
		}
	}
#endif	BALANCE_PATCH_20120329
}

/** @function : HyperModeBuffEffectStart
	@brief : ���� �� ���;��ϴ� ����Ʈ�� ����
*/
/*virtual*/ void CX2GULire_ElvenRanger::HyperModeBuffEffectStart()
{
	CX2GUUser::HyperModeBuffEffectStart();

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightGreen",	0,0,0, 0, 0 );
	
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftGreen",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightGreen",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftGreen",	0,0,0, 0, 0 );
}

/** @function : CX2GetActualMPConsume
	@brief : ��ų�� MP �Ҹ��� ����
	@param : ��ų�� ID, ����� ��ų�� ����
	@return : �Ҹ�� MP ��ġ
*/
/*virtual*/ float CX2GULire_ElvenRanger::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );

	switch ( eSkillID_ )
	{
	case CX2SkillTree::SI_A_LSR_ENTANGLE:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO12 ) )
				fMpConsumption -= 20.f;
		} break;

#ifdef SERV_RENA_NIGHT_WATCHER
		/// ������ ��ȣź ��ü ���� ���� ���� �Ҹ� ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_LNW_DELAYED_FIRING: 
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING: 
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			if( GetActiveStartOfDelayedFiring() )
				fMpConsumption = 0.f;
		} break;
#endif SERV_RENA_NIGHT_WATCHER



#ifdef ADDITIONAL_MEMO
	case CX2SkillTree::SI_A_LTR_EXPLOSION_TRAP:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO13 ) )// ���߹� ��� ����
			{
				fMpConsumption -= 5.f;
			}
		} break;
#endif

#ifdef ADD_MEMO_1ST_CLASS //��â��
	case CX2SkillTree::SI_A_LTR_ENTANGLE:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO23 ) == true )
				fMpConsumption *= 0.9f;

		} break;

	case CX2SkillTree::SI_SA_LE_MULTIPLE_STINGER:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO8 ) == true )
				fMpConsumption *= 0.9f;
		} break;

#endif //ADD_MEMO_1ST_CLASS
	default:
		break;
	}

	if ( 0.0f > fMpConsumption )
		fMpConsumption = 0.f;

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT	
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID_ );
	float fMpDecreaseRate = 1.0f;
	if( NULL != pSkillTemplet )
		fMpDecreaseRate  =  g_pData->GetMyUser()->GetSelectUnit()->GetSkillMpDecreaseRate(eSkillID_, pSkillTemplet->m_eType);

	return fMpConsumption * fMpDecreaseRate;
#else SERV_BALANCE_FINALITY_SKILL_EVENT
	return fMpConsumption;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
}

/** @function : AddUnitSlashData
	@brief : ���ֿ� Ʈ���̽� ���̱� (������ �ƴ�)
	@param : Ʈ���̽� ����(eSlashTraceCondition_)
	@return: ����Ʈ���̽�����Ÿ�� ����ִ� ������ ������(UINT)
*/
/*virtual*/ void CX2GULire_ElvenRanger::AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ )
{
#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
	if( NULL == m_pUnitSlashTraceManager )
		m_pUnitSlashTraceManager = CX2UnitSlashTraceManager::CreateUnitSlashTraceManager( this, GetXSkinAnimPtr() );

	if( NULL != m_pUnitSlashTraceManager )
	{
		const int vertexNum = (IsMyUnit() ? 400 : 150);

		switch ( eSlashTraceCondition_ )
		{
		case CX2UnitSlashTraceManager::STC_NONE:
			{
				m_pUnitSlashTraceManager->AddSlashTraceData( L"Bip01_R_Toe0", L"Bip01_R_Foot", 
					D3DXCOLOR(0xcc5555ff), D3DXCOLOR(0xccff3333), D3DXCOLOR(0xff5555ff), D3DXCOLOR(0xffff3333),
					vertexNum, 0.4f, 1, vertexNum, 0.5f, 3, 
					false, 0.f, L"" );
			} break;

		case CX2UnitSlashTraceManager::STC_RENA_NATURE_FORCE:
			{
				m_pUnitSlashTraceManager->AddSlashTraceData( L"Bip01_L_Toe0", L"Bip01_L_Foot", 
					D3DXCOLOR(0xcc5555ff), D3DXCOLOR(0xccff3333), D3DXCOLOR(0xff5555ff), D3DXCOLOR(0xffff3333),
					vertexNum, 0.4f, 1, vertexNum, 0.5f, 3, 
					true, 1.f, L"Rena_SlashTrace_Texture_WindSneaker.dds",
					CKTDGSlashTrace::CT_TOUGH_CURVE );


				m_pUnitSlashTraceManager->AddSlashTraceData( L"Bip01_R_Toe0", L"Bip01_R_Foot", 
					D3DXCOLOR(0xcc5555ff), D3DXCOLOR(0xccff3333), D3DXCOLOR(0xff5555ff), D3DXCOLOR(0xffff3333),
					vertexNum, 0.4f, 1, vertexNum, 0.5f, 3, 
					true, 1.f, L"Rena_SlashTrace_Texture_WindSneaker.dds",
					CKTDGSlashTrace::CT_TOUGH_CURVE );
			} break;

		default:
			break;
		}
	}
#endif UNIT_SLASH_TRACE_MANAGER_TEST	
}

#ifdef MODIFY_SET_DAMAGE_DATA
void CX2GULire_ElvenRanger::SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_,OUT float& fPowerRate)
{
	CX2GUUser::SetDamageExceptionProcess(pDamageData_, fPowerRate);
	int iSkillLevel = 0;
#ifdef BALANCE_GRAND_ARCHER_20121213	
	if( CX2DamageManager::HT_ARROW_HIT == pDamageData_->hitType )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LSR_POWERFUL_BOWSTRING, true );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = 
				g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LSR_POWERFUL_BOWSTRING );

			if ( NULL != pSkillTemplet )
			{
				if( GetRandomFloat() <= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, 
					min( iSkillLevel, MAX_LIMITED_SKILL_LEVEL ) ) )
				{
					pDamageData_->m_bIgnoreDefence = true;
					g_pData->GetGameEffectSet()->PlayEffectSet( L"EFFECTSET_DEBUFF_POWERFUL_BOWSTRING", this );
				}
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LE_POWERFUL_BOWSTRING );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = 
				g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LE_POWERFUL_BOWSTRING, min( iSkillLevel + GetSkillLevelUpNum( CX2SkillTree::SI_P_LE_POWERFUL_BOWSTRING ), 10 ) );

			if( GetRandomFloat() <= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE ) )
			{
				pDamageData_->m_bIgnoreDefence = true;
				g_pData->GetGameEffectSet()->PlayEffectSet( L"EFFECTSET_DEBUFF_POWERFUL_BOWSTRING", this );
				//  			if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
				//  			{
				// 				pDamageData_->PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
				//  			}
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
#endif //BALANCE_GRAND_ARCHER_20121213

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ���� Ʈ���� Ÿ���� DTT_SHARP_KICK_RELATE_ACTIVE_SKILL_INCREASE_MP_RATE �� ��, ������ ������ �нú� ������ ����
	if( CX2DamageManager::DTT_SHARP_KICK_RELATE_ACTIVE_SKILL_INCREASE_MP_RATE == pDamageData_->m_eDamageTrigger )
	{
		iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LCR_SHARP_KICK );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = 
				g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LCR_SHARP_KICK );

			if ( NULL != pSkillTemplet )
			{
				float fIncreaseSkillSkillMPRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillLevel );
				{
					pDamageData_->fHitAddMP = pDamageData_->fHitAddMP * fIncreaseSkillSkillMPRate;
				}
			}
		}
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
}
#endif //MODIFY_SET_DAMAGE_DATA


#ifdef BALANCE_GRAND_ARCHER_20121213
void CX2GULire_ElvenRanger::CreateGuideArrow( bool bMemo_, bool bHyperMode_/* = false*/)
{
	D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
	D3DXVECTOR3 degree = GetRotateDegree();
	degree.z -= 45.f;

	float fDegreeZ = 45.f;
	if( true == bMemo_ )
	{
		fDegreeZ = 30.f;
	}

	wstring EffectName = L"";

	if( true == bHyperMode_ )
	{
		if( true == bMemo_ )
			EffectName = L"GUIDED_ARROW_MEMO_HYPER";
		else
			EffectName = L"GUIDED_ARROW_HYPER";
	}
	else
	{
		if( true == bMemo_ )
			EffectName = L"GUIDED_ARROW_MEMO";
		else
			EffectName = L"GUIDED_ARROW";
	}

	//���̵� �ַο� ����Ʈ ����
	CX2DamageEffect::CEffect* aDamageEffect[4];
    ZeroMemory( aDamageEffect, sizeof(0) );
    int iNumDe = 0;
	aDamageEffect[iNumDe++] = g_pX2Game->GetDamageEffect()->CreateInstance( this, EffectName.c_str() , GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	degree.z += fDegreeZ;
	aDamageEffect[iNumDe++] = g_pX2Game->GetDamageEffect()->CreateInstance( this, EffectName.c_str(), GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	degree.z += fDegreeZ;
	aDamageEffect[iNumDe++] = g_pX2Game->GetDamageEffect()->CreateInstance( this, EffectName.c_str(), GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	degree.z += fDegreeZ;

	//�޸� ����ÿ��� �ѹ� �� ���
	if( true == bMemo_ )
		aDamageEffect[iNumDe++] = g_pX2Game->GetDamageEffect()->CreateInstance( this, EffectName.c_str(), GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );

	//Ÿ�� ����
	//������ �ִ� ������� 1�߾� Ÿ�� ������, ��ü������ ȭ���� ���� ��� ù Ÿ�� ������ �ٽ� Ÿ����
	std::map<float, UidType> mapNearUnitUid;
	g_pX2Game->GetNearUnitUidList( static_cast<CX2Room::TEAM_NUM>(GetTeam()), GetPos(), mapNearUnitUid);

	int iOffset = 0;
	std::map<float, UidType>::iterator it = mapNearUnitUid.begin();
	for( UINT i=0; i< (UINT) iNumDe; ++i )
	{				
		//Ÿ�� ��ü �� ����, ȭ���� ������
		if( (i - iOffset) >= mapNearUnitUid.size() ) 
		{
			//ù�� ° ���� ���� �ٽ� �����ϵ��� ������ ����
			iOffset = i;	
			it = mapNearUnitUid.begin();
		}
        if ( aDamageEffect[i] != NULL )
        {
		    if( NULL != g_pX2Game->GetNPCUnitByUID(static_cast<int>(it->second)) )
			    aDamageEffect[i]->SetLockOnNPCUID( static_cast<int>(it->second) );
		    else
			    aDamageEffect[i]->SetLockOnUnitUID( it->second );
		    //Ÿ���� �Ҿ��� ������ ���� ������ �ִ� Ÿ���� ��� ���� ����Ÿ�� ����
		    aDamageEffect[i]->SetLockOnType(CX2DamageEffect::LOT_NEARST_UID_VECTOR);
        }
		++it;
	}
}
#endif //BALANCE_GRAND_ARCHER_20121213

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
//-----------------------LESI_A_LCR_MIDDLE_KICK-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LCR_MIDDLE_KICK_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds" );
}

void CX2GULire_ElvenRanger::LESI_A_LCR_MIDDLE_KICK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		/// �̵�ű �޸�
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO11 ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_A_LCR_MIDDLE_KICK_DAMAGE", this, NULL, true );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_A_LCR_MIDDLE_KICK_DAMAGE", this, NULL );
	}

	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_A_LCR_MIDDLE_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//-----------------------LESI_A_LWS_BACK_KICK-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LWS_BACK_KICK_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LWS_BACK_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//-----------------------LESI_A_LWS_HIGH_KICK-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LWS_HIGH_KICK_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LWS_HIGH_KICK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//-----------------------LESI_SA_LWS_AIRELINNA_SYLPH-----------------------//
void CX2GULire_ElvenRanger::LESI_SA_LWS_AIRELINNA_SYLPH_Init()
{
	TextureReadyInBackground( L"Moon_MagicSquare02.tga" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Condense_pulse01.dds" );
	TextureReadyInBackground( L"Moon_MagicSquare01.dds" );
	TextureReadyInBackground( L"LWS_AIRELINNA_Fairy.tga" );
	TextureReadyInBackground( L"LWS_AIRELINNA_Fairy.tga" );
	XMeshReadyInBackground( L"Lire_Uplight01.Y" );
	XMeshReadyInBackground( L"RespawnLanding03.Y" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.x" );
	
	/*XSkinMeshReadyInBackground( L"item_Summon_Dummy_Mesh01.X" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"AeroTornado04.dds" );*/
}
void CX2GULire_ElvenRanger::LESI_SA_LWS_AIRELINNA_SYLPH_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_AIRELINNA", this );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7333f ) == true && EventCheck( 0.7333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_LWS_AIRELINNA_SYLPH );

		if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
		{
			D3DXVECTOR3 DamageEffectPos = GetPos();
			DamageEffectPos.y += (200.f * GetScaleByUnit().y) ;
			DamageEffectPos += ( GetIsRight() ? (120.f * GetDirVector() * GetScaleByUnit().x) : (-120.f * GetDirVector() * GetScaleByUnit().x) );

			CX2DamageEffect::CEffect* pDE 
				= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AIRELINNA_SYLPH_ATTACK_TWO", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree());
			if( NULL != pDE )
			{
                if ( CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pDE->GetMainEffect() )
				    pMeshInstance->SetMaxLifeTime(1.1f);
			}
		}
	}

	CommonFrameMove();
}


//-----------------------LESI_A_LSR_SHOOTING_MAGNUM-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LSR_SHOOTING_MAGNUM_Init()
{
	XSkinMeshReadyInBackground( L"Lire_Arrow.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XMeshReadyInBackground( L"Ring_Plane.y" );
	XMeshReadyInBackground( L"Ring_Plane.Y" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );	
	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"ColorBallGray.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LSR_SHOOTING_MAGNUM_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//ȭ����
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();
		if( GetIsRight() == true )
			posR = posR + GetDirVector() * 20.0f;
		else
			posR = posR - GetDirVector() * 20.0f;

		CX2DamageEffect::CEffect* pDE = NULL;
		m_wstrShootingMagumRandomArrow = L"DamageEffect_LSR_Shooting_Magnum";
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo(CX2SkillTree::SMI_RENA_MEMO19) == true )
		{
			int iRandomindex = static_cast<int>(GetRandomFloat() * 10.f);
			iRandomindex = iRandomindex % 3;
			switch(iRandomindex)
			{
			case 0:
				m_wstrShootingMagumRandomArrow += L"_Memo_Ice";
				break;
			case 1:
				m_wstrShootingMagumRandomArrow += L"_Memo_Poison";
				break;
			case 2:
				m_wstrShootingMagumRandomArrow += L"_Memo_Fire";
				break;
			}

			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
				posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
			posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );

		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.637f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.637f ) == true && EventCheck( 0.637f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//ȭ����
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();
		if( GetIsRight() == true )
			posR = posR + GetDirVector() * 20.0f;
		else
			posR = posR - GetDirVector() * 20.0f;

		CX2DamageEffect::CEffect* pDE = NULL;

#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo(CX2SkillTree::SMI_RENA_MEMO19) == true )
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
				posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
			posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.772f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.772f ) == true && EventCheck( 0.772f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//ȭ����
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );		
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();
		if( GetIsRight() == true )
			posR = posR + GetDirVector() * 20.0f;
		else
			posR = posR - GetDirVector() * 20.0f;

		CX2DamageEffect::CEffect* pDE = NULL;

#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo(CX2SkillTree::SMI_RENA_MEMO19) == true )
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
				posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
			posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//ȭ����
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );		
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();
		if( GetIsRight() == true )
			posR = posR + GetDirVector() * 20.0f;
		else
			posR = posR - GetDirVector() * 20.0f;

		CX2DamageEffect::CEffect* pDE = NULL;

#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo(CX2SkillTree::SMI_RENA_MEMO19) == true )
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
				posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, m_wstrShootingMagumRandomArrow.c_str(), GetPowerRate(), 
			posR, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_LSR_SHOOTING_MAGNUM_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )


	CommonEventProcess();
}


//-----------------------LESI_A_LGA_ARC_SHOT-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LGA_ARC_SHOT_Init()
{
	XSkinMeshReadyInBackground( L"Lire_Arrow.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XMeshReadyInBackground( L"Ring_Plane.y" );
	XMeshReadyInBackground( L"Ring_Plane.Y" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );	
	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"ColorBallGray.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LGA_ARC_SHOT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.52f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.52f ) == true && EventCheck( 0.52f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		CX2DamageEffect::CEffect* pDE = NULL;
		D3DXVECTOR3 vRotateArrow  = GetRotateDegree();
		float fPowerrate = GetPowerRate();

		vRotateArrow.z += 30.f;
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Arc_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
		vRotateArrow.z -= 15.f;
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Arc_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
		vRotateArrow.z -= 15.f;
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Arc_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
		vRotateArrow.z -= 15.f;
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Arc_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
		vRotateArrow.z -= 15.f;
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Arc_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_LGA_ARC_SHOT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}


//-----------------------LESI_A_LGA_RAPID_SHOT-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_READY_Init()
{
	XSkinMeshReadyInBackground( L"Lire_Arrow.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XMeshReadyInBackground( L"Ring_Plane.y" );
	XMeshReadyInBackground( L"Ring_Plane.Y" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );	
	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"ColorBallGray.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_READY_StartFuture()
{
	CommonStateStartFuture();

	switch( m_RapidShotDataPtr->GetSlotID_RapidShot() )
	{
	case USI_SPECIAL_ATTACK_1:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureA );
		break;

	case USI_SPECIAL_ATTACK_2:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureS );
		break;

	case USI_SPECIAL_ATTACK_3:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureD );
		break;

	case USI_SPECIAL_ATTACK_4:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureC );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_1:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureQ );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_2:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureW );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_3:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureE );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_4:
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureR );
		break;

	default:
		ASSERT( !L"LESI_A_LGA_RAPID_SHOT_READY_StartFuture" );
		m_RapidShotDataPtr->SetSkillKeyPointer( &m_InputData.pureA );
		break;
	}
}
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_READY_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( LESI_A_LGA_RAPID_SHOT_ATTACK );
		m_RapidShotDataPtr->SetTimeAfterStart( 0.f );
	}

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_ATTACK_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		const float MIN_TIME_AFTER_START = 1.2f;
		const float MAX_TIME_AFTER_START = 2.9f;

		if( ( m_RapidShotDataPtr->GetTimeAfterStart() > MIN_TIME_AFTER_START ) && 
			( false == m_RapidShotDataPtr->GetSkillKey() || m_RapidShotDataPtr->GetTimeAfterStart() >= MAX_TIME_AFTER_START ) )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else
				StateChange( LESI_A_LGA_RAPID_SHOT_FINISH );
		}
		else
		{
			StateChange( LESI_A_LGA_RAPID_SHOT_ATTACK );
		}
#ifndef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_EventTimeStampNow.clear();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_ATTACK_FrameMove()
{
	m_RapidShotDataPtr->SetTimeAfterStart( m_RapidShotDataPtr->GetTimeAfterStart() + m_fElapsedTime );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.034f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.034f ) == true && EventCheck( 0.034f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		CX2DamageEffect::CEffect* pDE = NULL;
		D3DXVECTOR3 vRotateArrow  = GetRotateDegree();
		float fPowerrate = GetPowerRate();

		float ArrowHigh = RandomFloat(-20.f,20.f);
		posR.y += ArrowHigh;

		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Rapid_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}

	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_A_LGA_RAPID_SHOT_FINISH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.054f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.054f ) == true && EventCheck( 0.054f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		CX2DamageEffect::CEffect* pDE = NULL;
		D3DXVECTOR3 vRotateArrow  = GetRotateDegree();
		float fPowerrate = GetPowerRate();

		float ArrowHigh = RandomFloat(-20.f,20.f);
		posR.y += ArrowHigh;

		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Rapid_Shot", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, 1.f, -1, -1.f, m_iArrowMeshPlayerIndex );
		if( NULL != pDE )
		{
			pDE->GetDamageData().backSpeed.x += m_ArrowData.m_fAddBackSpeedX;
		}
	}

	CommonFrameMove();
}

//-----------------------LESI_A_LGA_FREEZING_BOLT-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LGA_FREEZING_BOLT_Init()
{
	XSkinMeshReadyInBackground( L"WindWard_XMesh01.X" );
	XSkinMeshReadyInBackground( L"Freezing_Arrow01.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"steam_BP.dds" );
	TextureReadyInBackground( L"Ice_Crystal01.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"Gear_Water.tga" );
	TextureReadyInBackground( L"Sudden_Buster_P04.dds" );
	TextureReadyInBackground( L"Ice_Particle01.dds" );
	TextureReadyInBackground( L"Ice_Particle01.dds" );
	TextureReadyInBackground( L"Smoke.dds" );
	TextureReadyInBackground( L"Freezing_Arrow01.dds" );

}
void CX2GULire_ElvenRanger::LESI_A_LGA_FREEZING_BOLT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	CommonFrameMove();
}

//-----------------------LESI_A_LGA_WIND_BLAST-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LGA_WIND_BLAST_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_SI_A_ES_WINDMILL_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Lena_SI_A_LE_BACKJUMP_SHOT01.X" );
	XSkinMeshReadyInBackground( L"WindWard_XMesh01.X" );
	XSkinMeshReadyInBackground( L"A_LGA_WIND_BLAST_ARROW.X" );
	XMeshReadyInBackground( L"Lire_FeatherA.Y" );
	TextureReadyInBackground( L"steam_BP.dds" );
	TextureReadyInBackground( L"Lire_Leaf01.tga" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackE_light.dds" );	
	TextureReadyInBackground( L"Smoke.dds" );	
	TextureReadyInBackground( L"AeroTornado04.dds" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackE_light.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LGA_WIND_BLAST_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ������ ����Ʈ -> ���� ��ũ��Ʈ �� ����Ʈ ������ ���� 

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		D3DXVECTOR3 vRotateArrow  = GetRotateDegree();
		float fPowerrate = GetPowerRate();

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Wind_Blast_Main_Damage", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y );

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_LGA_Wind_Blast_Sub_Damage", fPowerrate, 
			posR, vRotateArrow, vRotateArrow, m_FrameDataNow.unitCondition.landPosition.y );
	}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

	CommonFrameMove();
}

//-----------------------LESI_SA_RTR_TRAPPING_ARROW_FUNGUS-----------------------//
void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_Init()
{
	XSkinMeshReadyInBackground( L"rena_A_trapingArrow_arrow.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XSkinMeshReadyInBackground( L"rena_A_explosionTrap_trapAttackSphere.X" );
	XSkinMeshReadyInBackground( L"rena_A_explosionTrap_circle.X" );
	XSkinMeshReadyInBackground( L"rena_A_explosionTrap_circle02.X" );
	XSkinMeshReadyInBackground( L"FRIENDSHIPOFNATURE.X" );
	TextureReadyInBackground( L"state_potion_flare.dds" );
	TextureReadyInBackground( L"smoke02.dds" );
	TextureReadyInBackground( L"Poison02.dds" );
	TextureReadyInBackground( L"rena_A_trapingArrow_circle.dds" );
	TextureReadyInBackground( L"rena_A_trapingArrow_trap.dds" );
	TextureReadyInBackground( L"rena_A_explosionTrap_trapLight.dds" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.DDS" );
	TextureReadyInBackground( L"soket_earth_quake_stone01.dds" );
	TextureReadyInBackground( L"Mesh_Common_AC_Face1_81160_Sorted_P02.DDS" );
	TextureReadyInBackground( L"Mesh_Common_AC_Face1_81160_Sorted_P02.DDS" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackA_P01.DDS" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackB_Cloud.DDS" );
	TextureReadyInBackground( L"Sudden_Buster_P02.dds" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackB_smoke.dds" );
	TextureReadyInBackground( L"rena_A_explosionTrap_spark.DDS" );
	TextureReadyInBackground( L"rena_A_explosionTrap_leafBoom.DDS" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Mesh_Common_AC_Face1_81160_Sorted_P02.DDS" );
	TextureReadyInBackground( L"Darksmoke01.dds" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackA_P01.DDS" );
	TextureReadyInBackground( L"Mesh_Common_AC_Face1_81160_Sorted_P02.DDS" );
	TextureReadyInBackground( L"state_potion_flare.DDS" );
	TextureReadyInBackground( L"Enchant_HenireCube_Green_P01.dds" );
}
void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_StateStart()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectTrapArrowFungus = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectTrapArrowFungs = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CommonStateStart();
}
void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

	bool bTrapDrop = false;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( INVALID_DAMAGE_EFFECT_HANDLE == m_hEffectTrapArrowFungus )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL == m_pEffectTrapArrowFungus )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			D3DXVECTOR3 posR = GetPos();
			D3DXVECTOR3 posHand = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			D3DXVECTOR3 degree = GetRotateDegree();

			posR.y = posHand.y;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hEffectTrapArrowFungus = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pEffectTrapArrowFungus = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                this, L"DamageEffect_LNW_Fungus_Trap_Arrow", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_TRAPING_ARROW", this );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimerOneshot( 1.40f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.50f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.60f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.70f ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimerOneshot( 1.80f ) == true )	bTrapDrop = true;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.40f ) == true && EventCheck( 1.40f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.50f ) == true && EventCheck( 1.50f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.60f ) == true && EventCheck( 1.60f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.70f ) == true && EventCheck( 1.70f, false ) == true )	bTrapDrop = true;
	else if( m_pXSkinAnim->EventTimer( 1.80f ) == true && EventCheck( 1.80f, false ) == true )	bTrapDrop = true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pEffectTrapArrowFungus = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectTrapArrowFungus );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pEffectTrapArrowFungus = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pEffectTrapArrowFungus ) ? m_pEffectTrapArrowFungus : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( bTrapDrop == true && pEffectTrapArrowFungus != NULL )
	{
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

		float fPowerRate = GetPowerRate();

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���
			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );
				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
		//	Ʈ�� ����
        if ( CKTDGXMeshPlayer::CXMeshInstance* pEffectTrapArrowFungusMainEffect = pEffectTrapArrowFungus->GetMainEffect() )
		{
			CX2DamageEffect::CEffect* pDE = NULL;

			if( true == GetActiveStartOfDelayedFiring() ) //������ ��ȣź�� ����� �������� üũ
			{
				pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FUNGUS_FLY_DELAY", fPowerRate, pEffectTrapArrowFungusMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
				
				if( NULL != pDE )
				{
					pDE->SetIndex( CreateDamageEffectIndex() );
					pDE->SetType( CX2DamageEffect::DET_TRAPPING_ARROW_FUNGUS );
				}
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRAPING_ARROW_FUNGUS_FLY", fPowerRate, pEffectTrapArrowFungusMainEffect->GetPos(), vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}	
		}
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_StateEnd()
{
	CommonStateEnd();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectTrapArrowFungus = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectTrapArrowFungs = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}


//-----------------------LESI_A_RNW_THORNS_TRAP-----------------------//
void CX2GULire_ElvenRanger::LESI_A_RNW_THORNS_TRAP_Init()
{
	XSkinMeshReadyInBackground( L"A_RNW_THORNS_TRAP.X" );
	XSkinMeshReadyInBackground( L"rena_A_explosionTrap_trapLineLight.X" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.DDS" );
	TextureReadyInBackground( L"soket_earth_quake_stone01.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_RNW_THORNS_TRAP_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed.x = 0.f;
	m_PhysicParam.nowSpeed.y = 0.f;
}
void CX2GULire_ElvenRanger::LESI_A_RNW_THORNS_TRAP_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.66f )
	{			
		m_bDisableGravity = false;
	}

	CommonFrameMoveFuture();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_THORNS_TRAP_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fPowerRate = GetPowerRate();
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );
				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}

		D3DXVECTOR3 pos = GetPos();
		pos.y += 200.f;

		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO24) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"THORNS_TRAP_FLY_MEMO", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"THORNS_TRAP_FLY", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );

	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_THORNS_TRAP_EventProcess()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_THORNS_TRAP_EndFuture()
{
	m_bDisableGravity = false;
	CommonStateEndFuture();
}



//-----------------------LESI_P_RNW_SEED_OF_RUIN-----------------------//
void CX2GULire_ElvenRanger::LESI_P_RNW_SEED_OF_RUIN_Init()
{
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"GuideArrow02.dds" );
	XSkinMeshReadyInBackground( L"P_RNW_SEED_OF_RUIN_OBJ.X" );
	TextureReadyInBackground( L"Poison02.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	XSkinMeshReadyInBackground( L"P_RNW_SEED_OF_RUIN_OBJ_ATTACK.X" );
	TextureReadyInBackground( L"rena_A_ruin_leafWinter.DDS" );
	TextureReadyInBackground( L"state_potion_flare.dds" );
}
void CX2GULire_ElvenRanger::LESI_P_RNW_SEED_OF_RUIN_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed.x = 0.f;
	m_PhysicParam.nowSpeed.y = 0.f;
}
void CX2GULire_ElvenRanger::LESI_P_RNW_SEED_OF_RUIN_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.66f )
	{			
		m_bDisableGravity = false;
	}

	CommonFrameMoveFuture();
}
void CX2GULire_ElvenRanger::LESI_P_RNW_SEED_OF_RUIN_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fPowerRate = GetPowerRate();
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );
				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
		
		D3DXVECTOR3	pos = GetPos();
		pos.y += 200.f;

		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

		if( true == GetActiveStartOfDelayedFiring() )
		{
			CX2DamageEffect::CEffect* pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SEED_OF_RUIN_DELAY", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
			if( NULL != pDE )
			{
				pDE->SetType( CX2DamageEffect::DET_SEED_OF_RUIN );
				pDE->SetIndex( CreateDamageEffectIndex() );
			}
		}
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SEED_OF_RUIN", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_P_RNW_SEED_OF_RUIN_EventProcess()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_P_RNW_SEED_OF_RUIN_EndFuture()
{
	m_bDisableGravity = false;
	CommonStateEndFuture();
}



//-----------------------LESI_A_RNW_ANGER_OF_ELF-----------------------//
void CX2GULire_ElvenRanger::LESI_A_RNW_ANGER_OF_ELF_Init()
{
	XSkinMeshReadyInBackground( L"SI_A_LSR_ENTANGLE_Effect_Seed.x" );
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ANGER_OF_ELF_StateStart()
{
	CreateTrapBlade();	/// ������ ����
	CommonStateStart();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ANGER_OF_ELF_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateAngerOfElfTrap();
	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ANGER_OF_ELF_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ANGER_OF_ELF_StateEnd()
{
	DeleteTrapBlade();	/// ������ ����
	CommonStateEnd();
}
bool CX2GULire_ElvenRanger::MakeRoomForAngerOfElfTrap( int iMaxAngerOfElfTrapCount, bool bForce )
{
	if( true == m_vecAngerOfElfTrapUID.empty() )
		return true;

	int iRemoveCount = (int) m_vecAngerOfElfTrapUID.size() - iMaxAngerOfElfTrapCount;
	if( iRemoveCount <= 0 )
		return true;


	for( int i=0; i<(int)m_vecAngerOfElfTrapUID.size(); i++ )
	{
		if( iRemoveCount <= 0 )
		{
			return true;
		}

		UidType& npcUID = m_vecAngerOfElfTrapUID[i];
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( (int) npcUID );
		if( NULL != pNPC )
		{
			ASSERT( CX2UnitManager::NUI_RENA_ANGER_OF_ELF == pNPC->GetNPCTemplet().m_nNPCUnitID || CX2UnitManager::NUI_RENA_ANGER_OF_ELF_DELAY == pNPC->GetNPCTemplet().m_nNPCUnitID);

			if( false == bForce && 
				1.f == pNPC->GetNowHp() ) // �������� Ʈ���� �� ���ְ�
				continue;

			pNPC->SetNowHp( 1.f );
		}

		m_vecAngerOfElfTrapUID.erase( m_vecAngerOfElfTrapUID.begin() + i );
		--i;
		--iRemoveCount;
	}

	return false;
}
void CX2GULire_ElvenRanger::CreateAngerOfElfTrap()
{
	MakeRoomForAngerOfElfTrap( m_MaxAngerOfElfTrapCount - 1, false );

	if( true == g_pX2Game->IsHost() )
	{
		D3DXVECTOR3 vPos = GetLandPosition();

		if( true == GetActiveStartOfDelayedFiring() )
		{
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_RENA_ANGER_OF_ELF_DELAY, IsHyperState(), true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
		}
		else
		{
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_RENA_ANGER_OF_ELF, IsHyperState(), true, vPos, 
				GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
		}
	}
}
void CX2GULire_ElvenRanger::AddAngerOfElfTrap( CX2GUNPC* pAngerOfElfTrap )
{
	if( pAngerOfElfTrap == NULL )
		return;

	m_vecAngerOfElfTrapUID.push_back( pAngerOfElfTrap->GetUID() );
}
void CX2GULire_ElvenRanger::DeleteAngerOfElfTrapUID( UidType iNPCUID )
{
	std::vector<UidType>::iterator itr = m_vecAngerOfElfTrapUID.begin();
	for( ; itr != m_vecAngerOfElfTrapUID.end(); )
	{
		if( iNPCUID == (*itr) )
			itr = m_vecAngerOfElfTrapUID.erase(itr);
		else
			itr++;
	}
}

//-----------------------LESI_A_RNW_ROSEBUSH_TRAP-----------------------//
void CX2GULire_ElvenRanger::LESI_A_RNW_ROSEBUSH_TRAP_Init()
{
	XSkinMeshReadyInBackground( L"A_RNW_THORNS_TRAP.X" );
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ROSEBUSH_TRAP_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed.x = 0.f;
	m_PhysicParam.nowSpeed.y = 0.f;
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ROSEBUSH_TRAP_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.66f )
	{			
		m_bDisableGravity = false;
	}

	CommonFrameMoveFuture();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ROSEBUSH_TRAP_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fPowerRate = GetPowerRate();
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
		D3DXVECTOR3 pos = GetPos();
		pos.y += 200.f;

		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO25 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ROSEBUSH_TRAP_FLY_MEMO", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );

			if( GetRandomFloat() <= 0.2f )
			{
				pos.x += 100.f;
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ROSEBUSH_TRAP_FLY_MEMO", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ROSEBUSH_TRAP_FLY", fPowerRate, pos, vRotateDegree, vRotateDegree, m_FrameDataNow.unitCondition.landPosition.y );

	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ROSEBUSH_TRAP_EventProcess()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_A_RNW_ROSEBUSH_TRAP_EndFuture()
{
	m_bDisableGravity = false;
	CommonStateEndFuture();
}


//-----------------------LESI_A_LWS_ASSAULT_IMPACT-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LWS_ASSAULT_IMPACT_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LWS_ASSAULT_IMPACT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}



//-----------------------LESI_A_LTR_ENTANGLE-----------------------//
void CX2GULire_ElvenRanger::LESI_A_LTR_ENTANGLE_Init()
{
	XSkinMeshReadyInBackground( L"SI_A_LSR_ENTANGLE_Effect_Seed.x" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.x" );
	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
	TextureReadyInBackground( L"HummingWind04.dds" );
}
void CX2GULire_ElvenRanger::LESI_A_LTR_ENTANGLE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fThrowStartTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fThrowStartTime ) == true && EventCheck( m_fThrowStartTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vEffectPos = GetPos();
		D3DXVECTOR3 vRotate = GetRotateDegree();

		vEffectPos.y += m_vThrowStartPosOffset.y;

		if(GetIsRight() == true)
			vEffectPos += ( m_vThrowStartPosOffset.x * GetDirVector() );
		else
			vEffectPos -= ( m_vThrowStartPosOffset.x * GetDirVector() );

		float fPowerRate = GetPowerRate();
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��ɲ��� ���, �� ������ ���� ���������� ���� �����ϵ��� �߰�
		// ������ ���� / ���� ��� ����!
		if ( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )	
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		{
			const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );	//	��ɲ��� ���
			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );
				if( NULL != pSkillTemplet )
				{
					fPowerRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}
#ifdef ADD_MEMO_1ST_CLASS //��â��
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RENA_MEMO23 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_LENA_TRAPPING_RANGER_ENTANGLE_MEMO", fPowerRate, vEffectPos, vRotate, vRotate, m_FrameDataNow.unitCondition.landPosition.y );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_LENA_TRAPPING_RANGER_ENTANGLE", fPowerRate, vEffectPos, vRotate, vRotate, m_FrameDataNow.unitCondition.landPosition.y );

	}

	CommonFrameMove();
}
void CX2GULire_ElvenRanger::LESI_A_LTR_ENTANGLE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( m_CommonState.m_Wait );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/** @function	: SetEquippedSkillLevelStateData
	@brief		: ������ ��ų�� �������� ����� �� ���� ����Ǿ�� �� ������Ʈ ����Ÿ
	@param		: const CX2SkillTree::SkillTemplet* pSkillTemplet, ARA_STATE_ID eStateID
*/
void CX2GULire_ElvenRanger::SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, LIRE_ELVENRANGER_STATE_ID eStateID )
{
	if ( NULL != pSkillTemplet )
		SetSkillLevelStateData( pSkillTemplet, m_StateList[eStateID] );
}

void CX2GULire_ElvenRanger::SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ )
{
	if ( NULL == GetUnit()  )
		return;
	
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// ��ų ����
	
	stateData_.m_SPLevel	= iSkillTempletLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );

	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
}

/** @function	: SetSiegeModeEffectiveValue
	@brief		: ���� ��带 ���� �����Ǵ� ��ġ ����
	@param		: ���� ��� ����
*/
void CX2GULire_ElvenRanger::SetSiegeModeEffectiveValue( IN const int iSkillLevel )
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LE_SIEGEMODE );

	if( NULL != pSkillTemplet )
	{
		m_fDamageRelSiegeArrow		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel ) * m_fSiegeDamageByPassive;
		m_fMPConsumeRateSiegeArrow	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevel );
	}
}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef RIDING_SYSTEM
/*virtual*/ bool CX2GULire_ElvenRanger::CanRidingState()
{
	switch ( GetNowStateID() )
	{
	case CX2GUUser::USI_WAIT:
	case CX2GUUser::USI_WALK:
	case CX2GUUser::USI_DASH:
	case CX2GUUser::USI_DASH_END:
	case CX2GUUser::USI_RIDING_WAIT:
	case CX2GUUser::USI_RIDING_WALK:
	case CX2GUUser::USI_RIDING_WAIT_HABIT:
	case CX2GUUser::USI_RIDING_DASH:
	case CX2GUUser::USI_RIDING_DASH_END:
	case CX2GULire_ElvenRanger::LESI_CHARGE_MP:
		break;

	default:
		return false;
	}
	
	return true;
}
#endif //RIDING_SYSTEM

/*virtual*/ void CX2GULire_ElvenRanger::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ )
{
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", fTimeToShow_, m_iSkillCutInSetSubIndex, bOnlyLight_ );
}


#ifdef FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

// ���� �ñر� �����̷� ��Ʈ����ũ
void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_READY_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"rana_A_explosionTrap_leaf.DDS");
	TextureReadyInBackground(L"secretVelder_alchemystBoss_magicAttackE_light.dds");
	TextureReadyInBackground(L"stone_AirShip.dds");
	TextureReadyInBackground(L"title_bloodAlliance_line_p03.dds");

	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	XSkinMeshReadyInBackground(L"HA_LWS_Ultimate_Middle_FX_NEW_B.X");
	XSkinMeshReadyInBackground(L"Karoo_Special_Attack_A_Tornado02.x");
	XSkinMeshReadyInBackground(L"Karoo_Special_Attack_A_Tornado03.x");
	XSkinMeshReadyInBackground(L"Karoo_Special_Attack_A_Tornado04.x");
	XSkinMeshReadyInBackground(L"rena_A_explosionTrap_circle.X");
	TextureReadyInBackground(L"stone_AirShip.dds");

}

void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_READY_StateStart()
{
	m_fSpiralStrikeRemainingTime = 5.5f;
	CommonStateStart();
}
void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_HA_LWS_SPIRAL_STIKE_SPINING );
	}

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_SPINING_StateStart()
{
	SetInvisibility( true );
//	TransformScale( PROTECT_VECTOR3( 0.001f, 0.001f, 0.001f ) );
	m_fRenderParamColorA = 0.f;
	
	if ( NULL != g_pX2Game->GetEffectSet() )
	{
		m_hSpiralStrikeMiddleEffect = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_HA_LWS_Ultimate_Middle_FX", this );
		if ( m_hSpiralStrikeMiddleLightEffect != INVALID_EFFECTSET_HANDLE )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet(m_hSpiralStrikeMiddleLightEffect );
		}
	}
}


void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_SPINING_FrameMove()
{
	if ( m_fSpiralStrikeRemainingTime > 0 )
	{
		m_fSpiralStrikeRemainingTime -= m_fElapsedTime;
	}
		
	if ( m_fSpiralStrikeRemainingTime <= 0 ) 
	{		
		m_fSpiralStrikeRemainingTime = 0;
	}
	
	else if ( m_fSpiralStrikeRemainingTime <= 0.5f )
	{ 		
		if ( m_hSpiralStrikeMiddleLightEffect == INVALID_EFFECTSET_HANDLE )
		{
			m_hSpiralStrikeMiddleLightEffect = 
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_HA_LWS_Ultimate_Finish_Light_FX", this );		
		}	
	}
	
	CommonFrameMove();
}

void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_SPINING_EventProcess()
{
	if ( m_fSpiralStrikeRemainingTime <= 0 ) 
	{		
		StateChange( LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK );
	}
	CommonEventProcess();
}



void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_SPINING_StateEnd()
{
	SetInvisibility( false );
	CommonStateEnd();
}



void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_Init()
{

	XSkinMeshReadyInBackground(L"Bust_Wolf_M02.X");
	XSkinMeshReadyInBackground(L"Bust_Wolf_M03.X");
	XSkinMeshReadyInBackground(L"CDC_AS_FIRE_EX_WIND02.x");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	XSkinMeshReadyInBackground(L"Gliter_Hammer_JumpAttack01.X");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	XSkinMeshReadyInBackground(L"LUNATIC_FURY_FireWave.X");
	XSkinMeshReadyInBackground(L"LUNATIC_FURY_FireWave02.X");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh03.X");
	TextureReadyInBackground(L"Particle_Blur.dds");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_FIRE01.X");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_FIRE02.X");
	XSkinMeshReadyInBackground(L"UnoHound_AttackB_Mesh01.X");
	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");
	XSkinMeshReadyInBackground(L"elsword_SA_SwordBlastiong_slash.X");
	XSkinMeshReadyInBackground(L"rena_A_explosionTrap_trapLineLight.X");
}


void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 2.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 2.16f ) == true && EventCheck( 2.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		if ( m_hSpiralStrikeMiddleEffect != INVALID_EFFECTSET_HANDLE )
		{
			if ( NULL != g_pX2Game->GetEffectSet() )
			{		
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hSpiralStrikeMiddleEffect );
			}
		}
	}
	CommonFrameMove();	
}

void CX2GULire_ElvenRanger::LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )	
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}



void CX2GULire_ElvenRanger::LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_Init()
{
	XSkinMeshReadyInBackground(L"Cryotron_Bolt_Weapon_Redy.X");
	XSkinMeshReadyInBackground(L"Cryotron_Bolt_Weapon_Redy2.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	TextureReadyInBackground(L"Heal01.dds");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"PulseWave01.dds");
	TextureReadyInBackground(L"rana_A_explosionTrap_leaf.DDS");

	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh01.X");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	TextureReadyInBackground(L"rena_A_evoke_energy.DDS");
	TextureReadyInBackground(L"rena_A_explosionTrap_leafBoom.DDS");
}

void CX2GULire_ElvenRanger:: LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT );
	}
	CommonEventProcess();
}


void CX2GULire_ElvenRanger:: LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_StateStart()
{	
	m_vecCryotronBoltDamageEffect.clear();
	CommonStateStart();

}

void CX2GULire_ElvenRanger:: LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_FrameMove()
{	
	for ( int i = 0; i < _CONST_RENA_::MANY_CRYOTRON_BOLT_GUILDE_SHOT; ++i )
	{
		float fCreateTimeOfGuideShot = _CONST_RENA_::START_TIME_OF_CRYOTRON_BOLT_CREATE_GUIDE_SHOT + i * _CONST_RENA_::TIME_GAP_OF_CREATE_CRYOTRON_BOLT_GUIDE_SHOT;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fCreateTimeOfGuideShot ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( fCreateTimeOfGuideShot ) == true && EventCheck( fCreateTimeOfGuideShot, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{

			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );
			D3DXVECTOR3 degree = GetRotateDegree();
			degree.z = (i * ( 180.f / _CONST_RENA_::MANY_CRYOTRON_BOLT_GUILDE_SHOT ) );
			// ũ���ƿ�Ʈ�� ��Ʈ ���� �߻�ü
			// 30' ~ 150' �� ������ ����

			//���̵� �ַο� ����Ʈ ����

            CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( 
                this, L"DamageEffect_Cryotron_Bolt_GUIDED_ARROW", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
            if ( pEffect != NULL )
            {
			    m_vecCryotronBoltDamageEffect.push_back( 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    pEffect->GetHandle()
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    pEffect
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    );
            }
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.730f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.730f ) == true && EventCheck( 2.730f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		//Ÿ�� ����
		//������ �ִ� ������� 1�߾� Ÿ�� ������, ��ü������ ȭ���� ���� ��� ù Ÿ�� ������ �ٽ� Ÿ����
		std::map<float, UidType> mapNearUnitUid;
		g_pX2Game->GetNearUnitUidList( static_cast<CX2Room::TEAM_NUM>(GetTeam()), GetPos(), mapNearUnitUid);

		int iOffset = 0;
		std::map<float, UidType>::iterator it = mapNearUnitUid.begin();
		for( UINT i=0; i<m_vecCryotronBoltDamageEffect.size(); ++i )
		{				
			//Ÿ�� ��ü �� ����, ȭ���� ������
			if( (i - iOffset) >= mapNearUnitUid.size() ) 
			{
				//ù�� ° ���� ���� �ٽ� �����ϵ��� ������ ����
				iOffset = i;	
				it = mapNearUnitUid.begin();
			}
            if ( CX2DamageEffect::CEffect* pCryotronBoltDamageEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_vecCryotronBoltDamageEffect[i] ) )
            {
			    if( NULL != g_pX2Game->GetNPCUnitByUID(static_cast<int>(it->second)) )
				    pCryotronBoltDamageEffect->SetLockOnNPCUID( static_cast<int>(it->second) );
			    else
				    pCryotronBoltDamageEffect->SetLockOnUnitUID( it->second );
			    //Ÿ���� �Ҿ��� ������ ���� ������ �ִ� Ÿ���� ��� ���� ����Ÿ�� ����
			    pCryotronBoltDamageEffect->SetLockOnType(CX2DamageEffect::LOT_NEARST_UID_VECTOR);
            }
			++it;
		}
	}
	CommonFrameMove();
}


void CX2GULire_ElvenRanger:: LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING );
	}
	CommonEventProcess();
}


void CX2GULire_ElvenRanger:: LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )	
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::SetInvisibility(bool bVal_)
{
	m_bInvisibility					= bVal_;
	m_bAbsoluteInvisibility			= bVal_;

	if( false == m_bInvisibility )
		m_RenderParam.color.a = m_fPreRenderParamColorA;

	m_fPreRenderParamColorA = m_RenderParam.color.a;
	m_fRenderParamColorA	= 1.f;

	m_RenderParam.bAlphaBlend = m_bInvisibility;
	SetAlphaObject( m_bInvisibility );

	m_pXSkinAnim->SetLayer(1);
	// ��� ������ ��ƼŬ
	BOOST_TEST_FOREACH( CX2EqipPtr, pEquip, m_ViewEqipList )
	{
		if( pEquip != NULL )
		{
			pEquip->SetShowAttachedParticle( !m_bInvisibility );
			pEquip->SetShowObject ( !m_bInvisibility, true );
		}
	}
	
	// ���� �Ӽ� ��æƮ ��ƼŬ
	SetShowEnchantWeaponParticle( !m_bInvisibility );
	// ������ ��
	if ( false == IsMyUnit() && false == IsNullGageUI() )
	{
		if( true == m_bInvisibility )
			m_pGageUI->SetAlpha( 0.f );
		else
			m_pGageUI->SetAlpha( 255.f );
	}

	// ���� ���¶�� ���� ��ƼŬ �Ѱ�/����
	if( GetRemainHyperModeTime() > 0.f )
	{
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostRFoot )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_RFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRFoot );
			if( NULL != pSeq_RFoot )
				pSeq_RFoot->SetShowObject( !m_bInvisibility );
		}
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostLFoot )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_LFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLFoot );
			if( NULL != pSeq_LFoot )
				pSeq_LFoot->SetShowObject( !m_bInvisibility );
		}
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostRArm )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_RArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRArm );
			if( NULL != pSeq_RArm )
				pSeq_RArm->SetShowObject( !m_bInvisibility );
		}
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostLArm )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_LArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLArm );
			if( NULL != pSeq_LArm )
				pSeq_LArm->SetShowObject( !m_bInvisibility );
		}
	}
}



/*virtual*/ void CX2GULire_ElvenRanger::ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
#ifdef RIDING_SYSTEM
	if ( m_bPassDash )
		return;
#endif //RIDING_SYSTEM

	if( true == m_bInvisibility )
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON;
		pRenderParam_->outLineColor.a	= m_fRenderParamColorA;
		m_RenderParam.color.a			= m_fRenderParamColorA;
		m_RenderParam.bAlphaBlend = true;
	}
	else 
		CX2GameUnit::ApplyRenderParam( pRenderParam_ );
}



// ����Ʈ ���� �ñر� - �̳뼾Ʈ
void CX2GULire_ElvenRanger::LESI_HA_LNW_INNOCENT_START_Init()
{
	TextureReadyInBackground(L"Arme_Critical2.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Arme_Critical2.dds");
	TextureReadyInBackground(L"LGA_shot_Ring_01.tga");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"LNW_Innocent_Shine_01.tga");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Explosion_Sphere_01.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	XMeshReadyInBackground(L"Particel_Motion_Flame01.Y");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"WindWard_XMesh01.X");		
}
void CX2GULire_ElvenRanger::LESI_HA_LNW_INNOCENT_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( LESI_HA_LNW_INNOCENT_END );
	}

	CommonEventProcess();
}

void CX2GULire_ElvenRanger::LESI_HA_LNW_INNOCENT_END_Init()
{
	TextureReadyInBackground(L"Mesh_Lire_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"rena_C_QuickDashJump_line.dds");
}
void CX2GULire_ElvenRanger::LESI_HA_LNW_INNOCENT_END_StateStart()
{
	CreateTrapBlade();
	CommonStateStart();
}
void CX2GULire_ElvenRanger::LESI_HA_LNW_INNOCENT_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )	
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#ifdef ADD_RENA_SYSTEM //��â��
	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	ZX_CANCEL_AFTER_EX( m_fAttackCancelAfterEx )
#endif //ADD_RENA_SYSTEM

	CommonEventProcess();
}
void CX2GULire_ElvenRanger::LESI_HA_LNW_INNOCENT_END_StateEnd()
{
	DeleteTrapBlade();
	CommonStateEnd();
}
#endif // FINALITY_SKILL_SYSTEM // ������, �ñر� �ý���

#ifdef MODIFY_RIDING_PET_AWAKE
void CX2GULire_ElvenRanger::RidingHyperModeInit()
{
	HyperModeInit();
}
void CX2GULire_ElvenRanger::RidingHyperModeFrameMove()
{
	CommonHyperModeFrameMove(0.f, 0.1f, true);
}
void CX2GULire_ElvenRanger::CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ /*= false*/  )
{
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTime1_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime1_ ) == true && EventCheck(fTime1_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( fTime2_ ) == true && EventCheck(fTime2_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 20.0f, 0.3f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();

		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}

	CommonFrameMove();
}
#endif // MODIFY_RIDING_PET_AWAKE

#ifdef ADD_MEMO_1ST_CLASS //��â��
const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION CX2GULire_ElvenRanger::GetSkillUseCondition(const CX2SkillTree::SkillTemplet* pSkillTemplet_)
{
	CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = pSkillTemplet_->m_eActiveSkillUseCondtion;

	switch (pSkillTemplet_->m_eID)
	{
	case CX2SkillTree::SI_SA_LTR_FATALITY:
		{
			if( GetEqippedSkillMemo(CX2SkillTree::SMI_RENA_MEMO22) == true )
				eActiveSkillUseCondition = CX2SkillTree::ASUT_AIR;
		} break;
	}


	return eActiveSkillUseCondition;
}
#endif //ADD_MEMO_1ST_CLASS


#ifdef ADD_RENA_SYSTEM //��â��
/** @function 	: CreateGageData
	@brief 		: GageData ����
*/
/*virtual*/ CX2GageData* CX2GULire_ElvenRanger::CreateGageData()
{
	if ( IsMyUnit() )
	{
		const CX2RenaGageData* pRenaGageData 
			= static_cast<const CX2RenaGageData*>( CX2GageManager::GetInstance()->GetMyGageData() );

		if ( NULL != pRenaGageData )
			return new CX2RenaGageData( *pRenaGageData );
		else
			return new CX2RenaGageData();
	}
	else
		return new CX2RenaGageData();
}

/** @function 	: AttackResultByType
	@brief 		: AT_NORMAL Ÿ������ �ڿ��� ����� ����
*/
/*virtual*/ void CX2GULire_ElvenRanger::AttackResultByType( CX2DamageManager::DamageData &pDamageData )
{
	CX2GUUser::AttackResultByType( pDamageData );


	switch( pDamageData.m_NaturalForceType )
	{
	//�޺��� ���
	case CX2DamageManager::NFT_COMBO:
	case CX2DamageManager::NFT_COMBO_BOMB:
		{
			//�� Ÿ�ݿ� �������� �´��� �� �� ù��° Ÿ���϶��� ����. 
			if( pDamageData.m_eFirstAttack == CX2DamageManager::FAC_FIRST_ATTACK )
			{
				bool bIsUpNaturalForce = false;

				//NF �������� �ִ� ���°� �ƴҶ�
				if( IsFullNaturalForce() == false )
				{
					//NF ������ ������ ���� Ÿ�� Ƚ�� ����
					//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó�� 2014.01.28
					if( pDamageData.m_RelateSkillData.m_byteRelateData >= 10 )	
					{
						if( pDamageData.m_RelateSkillData.m_byteRelateData == 100 && CheckDamageRelateComboData( pDamageData.m_RelateSkillData ) )
						{
							++m_fNaturalForceValueCharge;
						}
					}
					else
						++m_fNaturalForceValueCharge;

					if( m_fNaturalForceValueCharge >= NATURAL_FORCE_VALUE_PER_HIT )
						bIsUpNaturalForce = true;
				}

				//NF �������� �ִ��� üũ
				if( pDamageData.m_NaturalForceType == CX2DamageManager::NFT_COMBO_BOMB && IsEmptyNaturalForce() == false )
				{
					bool bCreate = true;

					//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
					if( pDamageData.m_RelateSkillData.m_byteRelateData >= 10 )
					{
						if( CheckDamageRelateSkillData( pDamageData.m_RelateSkillData ) )
						{
							DeleteDamageRelateSkillData( pDamageData.m_RelateSkillData );
						}
						else
							bCreate = false;
					}
					
					//���� ����Ʈ�� �����ϰ� NF�������� �ϳ� �Ҹ��Ѵ�.
					if( bCreate == true )
					{
						CX2DamageEffect::CEffect* pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"NATURAL_FORCE_EXPLOSION_BOOM_COMBO", 1.f, pDamageData.impactPoint, GetRotateDegree(), GetRotateDegree() );
						ConsumeNaturalForce(1);	
					}
			
				}

				//NF ������ ������ ���� Ÿ�� Ƚ���� ����ϴٸ� NF �������� �����ϰ� Ÿ�� Ƚ�� ����
				if( bIsUpNaturalForce == true )
				{
					m_fNaturalForceValueCharge -= NATURAL_FORCE_VALUE_PER_HIT;

					UpNaturalForce(1);
				}

				//ù��° Ÿ��üũ�� ���̻� ���� �ʵ��� ����
				pDamageData.m_eFirstAttack = CX2DamageManager::FAC_NOT_CHECK;
			}

		} break;

	//��ų�� ���
	case CX2DamageManager::NFT_ACTIVE:
	case CX2DamageManager::NFT_SPECIAL_ACTIVE:
		{
			//�����ص� ��ų ���� �����Ͱ� �߿� Damagedata�� ���� ���� �ִ��� üũ
			//��ġ�ϴ� ���� �ְ� NF�������� �ϳ� �̻� �ִٸ� 
			if( CheckDamageRelateSkillData( pDamageData.m_RelateSkillData ) == true && IsEmptyNaturalForce() == false )
			{
				//�ִ�� �Ҹ��ϴ� NF������ �� �޾ƿ�.
				Byte byteConsumeNFBySkill = pDamageData.m_RelateSkillData.m_byteRelateData;

				//��ġ�� ���� �����ص� Vector���� ����
				DeleteDamageRelateSkillData( pDamageData.m_RelateSkillData );
				CX2DamageEffect::CEffect* pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"NATURAL_FORCE_EXPLOSION_BOOM_SKILL", 1.f, pDamageData.impactPoint, GetRotateDegree(), GetRotateDegree() );

				//���� NF ������ ���� ��ų���� �ִ�� �Ҹ�Ǵ� NF ������ �� �� �ּҰ��� ����.
				byteConsumeNFBySkill = min( GetNaturalForceCount(), byteConsumeNFBySkill );
				if( pDE != NULL )
				{				
					//NF�������� �Ҹ�Ǵ� ������ ���� �������� �����ش�
					pDE->GetDamageData().damage.fMagic *= byteConsumeNFBySkill;
					pDE->GetDamageData().damage.fPhysic *= byteConsumeNFBySkill;
				}
				ConsumeNaturalForce(byteConsumeNFBySkill);	
			}

		} break;

	default:
		break;

	}
}

/** @function 	: ConsumeNaturalForce
	@brief 		: �ڿ��� ����� �Ҹ���
*/
void CX2GULire_ElvenRanger::ConsumeNaturalForce( int iCount_ )
{
	SetNaturalForceCount( GetNaturalForceCount() - iCount_ );

	//���� ������ ���� ��ġ�� ������� ������ ���� ��Ű�� �ʴ´�.
	if( (GetRemainHyperModeTime() <= 0.f) && (GetNaturalForceCount() < NATURAL_FORCE_BUFF && GetNaturalForceBuff() == true) )
		SetNaturalForceBuff( false );
}

/** @function	: GetNaturalForce
	@brief		: ��� Ȯ��
*/
__forceinline int CX2GULire_ElvenRanger::GetNaturalForceCount() 
{ 
	return GetRenaGageData()->GetNowNaturalForce(); 
}

/** @function 	: UpNaturalForce
	@brief 		: �ڿ��� ��� ����
*/
__forceinline void CX2GULire_ElvenRanger::UpNaturalForce( int iUpCount_ )
{
	SetNaturalForceCount( GetNaturalForceCount() + iUpCount_ );

	if( GetNaturalForceCount() >= NATURAL_FORCE_BUFF && GetNaturalForceBuff() == false )
		SetNaturalForceBuff( true );
}

/** @function 	: IsEmptyNaturalForce
	@brief 		: �ڿ��� ����� ������� Ȯ��
*/
__forceinline bool CX2GULire_ElvenRanger::IsEmptyNaturalForce()
{
	return GetRenaGageData()->IsEmptyNaturalForce();
}

/** @function 	: IsFullNaturalForce
	@brief 		: �ڿ��� ����� ���� á���� Ȯ��
*/
__forceinline bool CX2GULire_ElvenRanger::IsFullNaturalForce()
{
	return GetRenaGageData()->IsFullNaturalForce();
}

/** @function 	: SetNaturalForceCount
	@brief 		: �ڿ��� ��� ���� ����
*/
void CX2GULire_ElvenRanger::SetNaturalForceCount( const int iNaturalForceCount_ ) 
{ 
	GetRenaGageData()->SetNaturalForceChanged( true );
	GetRenaGageData()->SetNowNaturalForce( iNaturalForceCount_ ); 
}

/** @function 	: SetNaturalForceBuff
	@brief 		: NF ���� ��� ����
*/
void CX2GULire_ElvenRanger::SetNaturalForceBuff( const bool bBuffOn_ )
{
	GetRenaGageData()->SetNFBuffMode( bBuffOn_ );

	if( bBuffOn_ == true )
	{
		SetBuffFactorToGameUnitByBuffFactorID(BFI_BUFF_NATURAL_FORCE);
	}
	else
	{
		EraseBuffTempletFromGameUnit(BTI_BUFF_NATURAL_FORCE);
	}
}

/** @function 	: GetNaturalForceBuff
	@brief 		: NF ���� ��� ����
*/
const bool CX2GULire_ElvenRanger::GetNaturalForceBuff()
{
	return GetRenaGageData()->GetNFBuffMode();
}

/** @function 	: Init
	@brief 		: �ʱ�ȭ �Լ�. �ڿ��� ��� ����ȭ
*/
/*virtual*/ void CX2GULire_ElvenRanger::Init( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::Init( bUseTeamPos_, iStartPosIndex_ );

#ifdef ADD_RENA_SYSTEM //��â��
	/// �ڿ��� ��� ����ȭ
	if ( true == IsMyUnit() )
		m_FrameDataFuture.syncData.m_CannonBallCount = GetNaturalForceCount();

	m_eSaveStateSkillId = CX2SkillTree::SI_NONE;

	//���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó��
	if( GetAccessNowDamageRelateSkillData().m_byteRelateData >= 10 )
		GetAccessNowDamageRelateSkillData().m_byteRelateData = 0;
#endif //ADD_RENA_SYSTEM

	SetSphereAttackBoxScale( L"Lfoot", 1.3f );
	SetSphereAttackBoxScale( L"Rfoot", 1.3f );
}

/** @function	: SetSpecificValueByBuffTempletID
	@brief		: �� ���ָ��� Ư�� ������ ���� �� �� �����ؾ� �ϴ� �Լ� ����(ex: �����ø���ũ���� �������̶�� �÷��� ���� ��...)
	@param		: �������ø�ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GULire_ElvenRanger::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_HYPER_MODE:
		{
			SetNaturalForceBuff(true);
		} break;

	default:
		break;
	}
}

/** @function	: UnSetSpecificValueByBuffTempletID
	@brief		: �� ���ָ��� Ư�� ������ ���� �� �� �����ؾ� �ϴ� �Լ� ����(ex: �����ø���ũ���� ���� ��ٴ� �÷��� ���� ��...)
	@param		: �������ø�ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GULire_ElvenRanger::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_HYPER_MODE:
		{
			if( GetNaturalForceCount() < NATURAL_FORCE_BUFF )
			{
				GetRenaGageData()->SetNFBuffMode( false );
				ReserveToFinishBuffTempletFromGameUnit(BTI_BUFF_NATURAL_FORCE);
			}
		} break;

	default:
		break;
	}
}

/** @function 	: AdjustDamageDataBeforeDamageReact
	@brief 		: DamageReact�� �����ϱ� ���� �ش� DamageData�� ������ �� �ִ� �Լ�
*/
/*virtual*/ void CX2GULire_ElvenRanger::AdjustDamageDataBeforeDamageReact( CX2DamageManager::DamageData* pDamageData )
{
	//RT_DOWN������ �������� �ʵ��� BIG_DAMAGE�� REACT�� �ٲ��ش�.
	if( pDamageData->m_NaturalForceType == CX2DamageManager::NFT_COMBO_BOMB && IsEmptyNaturalForce() == false )
	{
		pDamageData->reActType = CX2DamageManager::RT_BIG_DAMAGE;
		pDamageData->reActResult = CX2DamageManager::RT_BIG_DAMAGE;
	}
}

/** @function 	: SetNaturalForceEffect
	@brief 		: NF������ ���ڿ� ��ġ�ϴ� ����Ʈ ����
*/
void CX2GULire_ElvenRanger::SetNaturalForceEffect()
{
	if( NULL == g_pX2Game->GetEffectSet() )
		return;

	if( INVALID_EFFECTSET_HANDLE != m_hNaturalForceEffect )
		g_pX2Game->GetEffectSet()->StopEffectSet(m_hNaturalForceEffect);
	
	//wstring wstrEffectName = L"";
    const wchar_t* wszEffectName = NULL;

	switch( GetNaturalForceCount() )
	{
	case 1:
		wszEffectName = L"EffectSet_Rena_NF_Loop01";
		break;

	case 2:
		wszEffectName = L"EffectSet_Rena_NF_Loop02";
		break;

	case 3:
		wszEffectName = L"EffectSet_Rena_NF_Loop03";
		break;

	case 4:
		wszEffectName = L"EffectSet_Rena_NF_Loop04";
		break;

	case 5:
		wszEffectName = L"EffectSet_Rena_NF_Loop_ALL";
		break;

	default:
		break;
	}

	//if( L"" != wszEffectName.c_str() )
    if ( wszEffectName != NULL )
		m_hNaturalForceEffect = g_pX2Game->GetEffectSet()->PlayEffectSet( wszEffectName, (CX2GameUnit*) this );
}

/** @function	: UpdateNaturalForceEffect
	@brief		: NF ����Ʈ�� ���� ����Ʈ ���� �Լ�
*/
void CX2GULire_ElvenRanger::UpdateNaturalForceEffect()
{
	/// ������ NF ����Ʈ�� �ٸ��ٸ�, ����Ʈ ����
	if ( m_iBeforeNaturalForceCount != GetNaturalForceCount() )
	{
		SetNaturalForceEffect();
		m_iBeforeNaturalForceCount = GetNaturalForceCount();
	}
}

/** @function	: IsHyperSkill
	@brief		: ���ڷ� �Ѿ�� SKILL ID�� ������ ��Ƽ������ üũ
*/
bool CX2GULire_ElvenRanger::IsHyperActiveSkill( CX2SkillTree::SKILL_ID eSkillId_ )
{
	switch( eSkillId_ )
	{
	case CX2SkillTree::SI_HA_LWS_SPIRAL_STRIKE:
	case CX2SkillTree::SI_HA_LGA_CRYOTRON_BOLT:
	case CX2SkillTree::SI_HA_LNW_INNOCENT:
		{
			return true;
		}
	}

	return false;
}

/** @function 	: CheckDamageRelateSkillData
	@brief 		: ������ �����Ϳ� ����ִ� ���� �����ص� �����Ͱ� ��ġ�ϴ� ���� �ִ��� üũ
*/
/*virtual*/ bool CX2GULire_ElvenRanger::CheckDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ )
{
	CX2GUUser::AllDamageRelateSkillData sAllDamageRelateSkillData = GetAccessAllDamageRelateSkillData();
	std::vector<CX2DamageManager::DamageRelateSkillData>::iterator itr = sAllDamageRelateSkillData.m_vecSaveData.begin();
	for( ; itr != sAllDamageRelateSkillData.m_vecSaveData.end(); )
	{
		if( sData_.m_byteSkillIndex == (*itr).m_byteSkillIndex )
		{
			return true;
		}
		else
			itr++;
	}

	return false;
}

/** @function 	: CheckDamageRelateComboData
	@brief 		: ���������� ������ ����Ʈ�� 2�� �̻� �����ϴ� �޺��� ���� ����ó�� 2014.01.28
*/
bool CX2GULire_ElvenRanger::CheckDamageRelateComboData( const CX2DamageManager::DamageRelateSkillData sData_ )
{
	CX2GUUser::AllDamageRelateSkillData& sAllDamageRelateSkillData = GetAccessAllDamageRelateSkillData();
	std::vector<CX2DamageManager::DamageRelateSkillData>::iterator itr = sAllDamageRelateSkillData.m_vecSaveData.begin();

	for( ; itr != sAllDamageRelateSkillData.m_vecSaveData.end(); )
	{
		if( sData_.m_byteSkillIndex == (*itr).m_byteSkillIndex && sData_.m_byteRelateData == (*itr).m_byteRelateData )
		{
			(*itr).m_byteRelateData -= 1;
			return true;
		}
		else
			itr++;
	}

	return false;
}

/** @function 	: DeleteDamageRelateSkillData
	@brief 		: ��ġ�Ѵ� ������ ���� �����ص� vector���� ����
*/
/*virtual*/ void CX2GULire_ElvenRanger::DeleteDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ )
{
	CX2GUUser::AllDamageRelateSkillData& sAllDamageRelateSkillData = GetAccessAllDamageRelateSkillData();
	std::vector<CX2DamageManager::DamageRelateSkillData>::iterator itr = sAllDamageRelateSkillData.m_vecSaveData.begin();
	for( ; itr != sAllDamageRelateSkillData.m_vecSaveData.end(); )
	{
		if( sData_.m_byteSkillIndex == (*itr).m_byteSkillIndex )
		{
			itr = sAllDamageRelateSkillData.m_vecSaveData.erase(itr);
			break;
		}
		else
			itr++;
	}
}
#endif //ADD_RENA_SYSTEM