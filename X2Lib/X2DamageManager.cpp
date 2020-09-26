#include "stdafx.h"
#include ".\x2damagemanager.h"

CX2DamageManager::CX2DamageManager(void)
{
	m_pCollision = g_pKTDXApp->GetCollision();


	for( int i=0; i<(int)HIT_TYPE_COUNT; i++ )
	{
		for( int j=0; j<(int)HITTED_TYPE_COUNT; j++ )
		{
			m_DamageSoundName[i][j] = L"";
		}
	}
#ifndef WORLD_TOOL
	OpenDamageSoundScript( L"DamageSound.lua" );
#ifdef ELSWORD_WAY_OF_SWORD
	OpenWayOfSwordScript( L"WayOfSword.lua" );
#endif ELSWORD_WAY_OF_SWORD
#endif //WORLD_TOOL
}

CX2DamageManager::~CX2DamageManager(void)
{
	m_pCollision = NULL;

	for( UINT i=0; i<m_pvecReadySound.size(); i++ )
	{
		CKTDXDeviceSound* pDeviceSound = m_pvecReadySound[i];
		SAFE_CLOSE(pDeviceSound);
	}
}

#ifdef SKILL_30_TEST
void CX2DamageManager::SetExtraDamageData( DamageData* pDamageData, bool bApplyEffect,
										  CX2DamageManager::EXTRA_DAMAGE_TYPE edtType, float fRate, float fTime, float fDamage, float fExtraDamage, 
										  int iLevel, bool bIgnoreResist, float fDamageRate, float fRunJumpRate, float fAnimSpeedRate )
{
	if(pDamageData == NULL)
		return;

	pDamageData->m_bApplyExtraDamage = bApplyEffect;
	pDamageData->m_BufExtraDamage.m_ExtraDamageType = edtType;
	pDamageData->m_BufExtraDamage.m_fRate = fRate;
#ifdef EXTRADAMAGE_RATE_BUG_FIX		
	pDamageData->m_BufExtraDamage.m_fPureRate = fRate;
#endif EXTRADAMAGE_RATE_BUG_FIX		
	pDamageData->m_BufExtraDamage.m_fTime = fTime;
	pDamageData->m_BufExtraDamage.m_DamagePerSec = fDamage;
	pDamageData->m_BufExtraDamage.m_Damage = fExtraDamage;

#ifdef NEW_EXTRA_DAMAGE
	pDamageData->m_BufExtraDamage.m_iLevel = iLevel;
	pDamageData->m_BufExtraDamage.m_bIgnoreResist = bIgnoreResist;
#endif

#ifdef SKILL_CASH_10_TEST
	pDamageData->m_BufExtraDamage.m_fDamageRate = fDamageRate;
	pDamageData->m_BufExtraDamage.m_fRunJumpRate = fRunJumpRate;
	pDamageData->m_BufExtraDamage.m_fAnimSpeedRate = fAnimSpeedRate;
#endif
}
#endif

#ifdef ADD_MEMO_1ST_CLASS
bool CX2DamageManager::SetDamageDataFromLUA( DamageData* pDamageData, KLuaManager& luaManager, const char* pTableName, float fPowerRate, const bool IsEqippedMemo_ )
#else //ADD_MEMO_1ST_CLASS
bool CX2DamageManager::SetDamageDataFromLUA( DamageData* pDamageData, KLuaManager& luaManager, const char* pTableName, float fPowerRate )
#endif //ADD_MEMO_1ST_CLASS
{
	if( luaManager.BeginTable( pTableName ) == true )
	{
		LUA_GET_VALUE_ENUM( luaManager, "DAMAGE_TYPE",		pDamageData->damageType,		DAMAGE_TYPE,	DT_PHYSIC );
		LUA_GET_VALUE_ENUM( luaManager, "ATTACK_TYPE",		pDamageData->attackType,		ATTACK_TYPE,	AT_NORMAL );
		LUA_GET_VALUE_ENUM( luaManager, "HIT_TYPE",			pDamageData->hitType,			HIT_TYPE,		HT_DEFAULT );
		LUA_GET_VALUE_ENUM( luaManager, "REACT_TYPE",		pDamageData->reActType,			REACT_TYPE,		RT_NO_DAMAGE );
		LUA_GET_VALUE(		luaManager, "MELEE_ATTACK",		pDamageData->m_bMeleeAttack,	pDamageData->m_bMeleeAttack );	/// �����Ǿ� ���� ���� ��� �ʱ�ȭ �� ��� ����
		LUA_GET_VALUE(		luaManager, "DRAIN",			pDamageData->m_bDrain,			false );
		LUA_GET_VALUE(		luaManager, "DRAIN_MANA",		pDamageData->m_bDrainMana,		false );

#ifndef MODIFY_SET_DAMAGE_DATA
		//������������
		if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->ShouldAttackAllTeam() )
			pDamageData->m_bAttackAllTeam = true;
		else
#endif //MODIFY_SET_DAMAGE_DATA
		{
			LUA_GET_VALUE(		luaManager, "ATTACK_ALL_TEAM",	pDamageData->m_bAttackAllTeam,	false );
		}

		LUA_GET_VALUE(		luaManager, "DUNGEON_RATE",		pDamageData->m_fDungeonRate,	1.0f );
		
		LUA_GET_VALUE(		luaManager, "CREATE_DAMAGE_EFFECT_NAME",		pDamageData->m_wstrCreateDamageEffectName,	L"" );

#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
		// RT_CREATE_DAMAGE_EFFECT �� �ƴҰ�� Ȯ�� �����ϵ��� �Ѵ�. (���� ������ �����Ǿ� ����)
		LUA_GET_VALUE(		luaManager, "CREATE_DAMAGE_EFFECT_RATE",		pDamageData->m_fCreateDamageEffectRate,	0.f );

		
#endif

		LUA_GET_VALUE(		luaManager, "CREATE_EFFECT_SET_NAME",			pDamageData->m_wstrCreateEffectSetName,		L"" );
		LUA_GET_VALUE(		luaManager, "CREATE_EFFECT_SET_TRACE_UNIT",		pDamageData->m_bCreateEffectSetTraceUnit,	false );

#ifdef TRACE_UNIT_DIE_DELETE_EFFECTSET
		LUA_GET_VALUE(		luaManager, "TRACE_UNIT_DIE_DELETE_EFFECT_SET",	pDamageData->m_bTraceUnitDieDeleteEffectSet, true );
#endif //TRACE_UNIT_DIE_DELETE_EFFECTSET

#ifdef ELSWORD_WAY_OF_SWORD
		LUA_GET_VALUE_ENUM( luaManager, "WAY_OF_SWORD_TYPE",			pDamageData->m_WayofSwordType,		WAY_OF_SWORD_TYPE,			WST_NONE );
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		/// DP ���� ����
		LUA_GET_VALUE_ENUM( luaManager, "DYNAMO_PARTICLE_RATE_TYPE",	pDamageData->m_DPRateType,			DYNAMO_PARTICLE_RATE_TYPE,	DPRT_NONE );
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //��â��
		LUA_GET_VALUE_ENUM( luaManager, "NATURAL_FORCE_TYPE",			pDamageData->m_NaturalForceType,	NATURAL_FORCE_TYPE,			NFT_NONE );
#endif //ADD_RENA_SYSTEM

		if( luaManager.BeginTable( "DAMAGE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "PHYSIC",		pDamageData->damage.fPhysic,	0.0f );
			LUA_GET_VALUE( luaManager, "MAGIC",		pDamageData->damage.fMagic,		0.0f );

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( "ATTACKER_GET" ) == true )
		{
			LUA_GET_VALUE( luaManager, "HP",		pDamageData->attackerGet.m_fHP,		0.0f );
			LUA_GET_VALUE( luaManager, "MP",		pDamageData->attackerGet.m_fMP,		0.0f );

			luaManager.EndTable();
		}

		ParsingBuffFactor( luaManager, pDamageData );
		PushSocketAndTemporaryBuffFactor( pDamageData );

#ifdef DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL
	#ifdef ADD_MEMO_1ST_CLASS //��â��
			ParsingBuffFactorID( luaManager, pDamageData, IsEqippedMemo_ );
	#else //ADD_MEMO_1ST_CLASS
			ParsingBuffFactorID( luaManager, pDamageData );
	#endif //ADD_MEMO_1ST_CLASS
#endif //DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL

#ifdef PVP_SEASON2
		LUA_GET_VALUE( luaManager, "PVP_RATE", pDamageData->m_fPvpRate, 1.0f );
#endif

		if( luaManager.BeginTable( "EXTRA_DAMAGE" ) == true )
		{
			LUA_GET_VALUE_ENUM( luaManager, "EXTRA_DAMAGE_TYPE",	pDamageData->m_ExtraDamage.m_ExtraDamageType,	EXTRA_DAMAGE_TYPE,	EDT_NONE );


			float fValue = 0.f;
			LUA_GET_VALUE( luaManager, "RATE",				fValue,	1.f );
			pDamageData->m_ExtraDamage.m_fRate = fValue;
#ifdef EXTRADAMAGE_RATE_BUG_FIX				
			pDamageData->m_ExtraDamage.m_fPureRate = fValue;
#endif EXTRADAMAGE_RATE_BUG_FIX	

			LUA_GET_VALUE( luaManager, "TIME",				fValue,	0.f );
			pDamageData->m_ExtraDamage.m_fTime = fValue;

			LUA_GET_VALUE( luaManager, "DAMAGE_PER_SEC",		fValue,	0.f );
			pDamageData->m_ExtraDamage.m_DamagePerSec = fValue;

			LUA_GET_VALUE( luaManager, "DAMAGE",			fValue,	0.f );
			pDamageData->m_ExtraDamage.m_Damage = fValue;
#ifndef BUFF_TEMPLET_SYSTEM		//���� �ɼ��� ����� ��Ƽ�� ��ų ���ݷ� ������ PowerRate�� ���� ��Ű�� ������, �ؿ� �������� PowerRate�� ���� ��Ų �� ���� �ϱ� ���� ���� �̵�
#ifdef BALANCE_PATCH_20120329
			if( pDamageData->m_ExtraDamage.m_ExtraDamageType == EDT_PAIN )
				pDamageData->m_ExtraDamage.m_Damage = fPowerRate;
#endif
#endif BUFF_TEMPLET_SYSTEM

#ifdef NEW_EXTRA_DAMAGE			
			int		iLevel = -1;
			bool	bIgnoreResist = false;			
			
			LUA_GET_VALUE( luaManager, "LEVEL",			iLevel,	-1 );
			pDamageData->m_ExtraDamage.m_iLevel = iLevel;
			LUA_GET_VALUE( luaManager, "ADD_LEVEL",			iLevel,	0 );
			pDamageData->m_ExtraDamage.m_iAddLevel = iLevel;
			LUA_GET_VALUE( luaManager, "IGNORE_RESIST",	bIgnoreResist,	false );			
			pDamageData->m_ExtraDamage.m_bIgnoreResist = bIgnoreResist;
#endif
#ifdef ADDITIONAL_MEMO
			bool bDefenderLevel = false;
			LUA_GET_VALUE( luaManager, "DEFENDER_LEVEL",	bDefenderLevel,	false );			
			pDamageData->m_ExtraDamage.m_bDefenderLevel = bDefenderLevel;			
#endif
#ifdef SKILL_CASH_10_TEST
			LUA_GET_VALUE( luaManager, "DAMAGE_RATE",			fValue,	0.f );
			pDamageData->m_ExtraDamage.m_fDamageRate = fValue;
			LUA_GET_VALUE( luaManager, "RUNJUMP_SPEED_RATE",		fValue,	0.f );
			pDamageData->m_ExtraDamage.m_fRunJumpRate = fValue;
			LUA_GET_VALUE( luaManager, "ANIMSPEED_RATE",			fValue,	0.f );
			pDamageData->m_ExtraDamage.m_fAnimSpeedRate = fValue;			
#endif

			LUA_GET_VALUE( luaManager, "FIXED_DAMAGE",	pDamageData->m_ExtraDamage.m_bFixedDamage,	false );		

#ifdef CHUNG_SECOND_CLASS_CHANGE
			LUA_GET_VALUE( luaManager, "JUMP_SPEED_RATE",		fValue,	0.f );
			pDamageData->m_ExtraDamage.m_fJumpRate = fValue;
#endif
			luaManager.EndTable();
		}

		if( luaManager.BeginTable( "ENABLE_ATTACKBOX" ) == true )
		{
			int index = 1;
			wstring wname;
			while( luaManager.GetValue( index, wname ) == true )
			{
				if( null != pDamageData->optrAttackerGameUnit )
					pDamageData->optrAttackerGameUnit->SetEnableAttackBox( wname.c_str(), true );

				index++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( "DISABLE_ATTACKBOX" ) == true )
		{
			int index = 1;
			wstring wname;
			while( luaManager.GetValue( index, wname ) == true )
			{
				if( null != pDamageData->optrAttackerGameUnit )
					pDamageData->optrAttackerGameUnit->SetEnableAttackBox( wname.c_str(), false );

				index++;
			}

			luaManager.EndTable();
		}


		{
			int index = 0;
			std::wstring wstrParticleName = L"";
			pDamageData->m_vecHitParticleName.resize(0);
			while( luaManager.BeginTable( "CUSTOM_HIT_PARTICLE", index ) == true )
			{
				LUA_GET_VALUE( luaManager, "PARTICLE_NAME",		wstrParticleName, 		L""			);
				if( false == wstrParticleName.empty() )
				{
					pDamageData->m_vecHitParticleName.push_back( wstrParticleName );
				}
				index++;
				luaManager.EndTable();
			}
		}


		LUA_GET_VALUE( luaManager, "TECH_POINT",			pDamageData->techPoint,				0	 );        
		LUA_GET_VALUE( luaManager, "BACK_SPEED_W",			pDamageData->m_bWBackSpeed,			false );
		LUA_GET_VALUE( luaManager, "BACK_SPEED_X",			pDamageData->backSpeed.x,			0.0f );
		LUA_GET_VALUE( luaManager, "BACK_SPEED_Y",			pDamageData->backSpeed.y,			0.0f );

//{{ kimhc // 2010.2.17 //
#ifdef	APPLY_MOTION_OFFSET
		LUA_GET_VALUE( luaManager, "APPLY_MOTION_OFFSET",	pDamageData->m_bApplyMotionOffset,	true );
#endif	APPLY_MOTION_OFFSET
//}} kimhc // 2010.2.17 //

		LUA_GET_VALUE( luaManager, "FORCE_DOWN",			pDamageData->fForceDownValue,		0.0f );
		LUA_GET_VALUE( luaManager, "GROGGY_TIME",			pDamageData->fGroggyTime,			0.0f );

		LUA_GET_VALUE( luaManager, "HIT_ADD_MP",			pDamageData->fHitAddMP,				6.0f );
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		/// ���� ������, �������� ü�� ȸ�� ( ��ü ü���� ȸ�� ���� )
		LUA_GET_VALUE( luaManager, "HIT_ADD_HP_PERCENT",	pDamageData->fHitAddHPPer,				0.0f );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		LUA_GET_VALUE( luaManager, "STOP_TIME_ATT",			pDamageData->fStopTimeAtt,			0.0f );
		LUA_GET_VALUE( luaManager, "STOP_TIME_DEF",			pDamageData->fStopTimeDef,			0.0f );

		LUA_GET_VALUE_ENUM( luaManager, "CAMERA_SHAKE_TYPE",		pDamageData->m_eCameraShakeType,		CKTDGCamera::SHAKE_TYPE, CKTDGCamera::DECT_UP_DOWN );
		LUA_GET_VALUE( luaManager, "CAMERA_CRASH_GAP",		pDamageData->fCameraCrashGap,		0.0f );
		LUA_GET_VALUE( luaManager, "CAMERA_CRASH_TIME",		pDamageData->fCameraCrashTime,		0.0f );

		LUA_GET_VALUE( luaManager, "CLEAR_SCREEN",			pDamageData->clearScreen,			0	 );
		LUA_GET_VALUE( luaManager, "CLEAR_SCREEN_COLOR_A",	pDamageData->clearScreenColor.a,	0.0f );
		LUA_GET_VALUE( luaManager, "CLEAR_SCREEN_COLOR_R",	pDamageData->clearScreenColor.r,	1.0f );
		LUA_GET_VALUE( luaManager, "CLEAR_SCREEN_COLOR_G",	pDamageData->clearScreenColor.g,	1.0f );
		LUA_GET_VALUE( luaManager, "CLEAR_SCREEN_COLOR_B",	pDamageData->clearScreenColor.b,	1.0f );
#ifdef ARRANGED_FLY_DAMAGE_TEST
		//{{ oasis907 : ����� [2010.2.11] // 
		LUA_GET_VALUE( luaManager, "ARRANGED_FLY",			pDamageData->bArrangedFly,				false );
		//}}
#endif ARRANGED_FLY_DAMAGE_TEST
		LUA_GET_VALUE( luaManager, "FORCE_FLY",				pDamageData->bForceFly,				false );
		LUA_GET_VALUE( luaManager, "CAN_REVENGE",			pDamageData->bCanRevenge,			true );
		LUA_GET_VALUE( luaManager, "CAN_REFLEX",				pDamageData->bCanReflexMagic,		false );
		
		LUA_GET_VALUE( luaManager, "RE_ATTACK",				pDamageData->bReAttack,				false );
		LUA_GET_VALUE( luaManager, "HIT_GAP",				pDamageData->fHitGap,				0.0f );
#ifdef GROUND_HIT 
		LUA_GET_VALUE( luaManager, "DAMAGE_LINEGROUP",		pDamageData->DamageLineGroup,		false );
		LUA_GET_VALUE( luaManager, "DAMAGE_DISTANCE",		pDamageData->DamageDistance,		99999.f );
#endif
		LUA_GET_VALUE( luaManager, "FLIP_DIR",				pDamageData->m_bFlipDirOnHit,		false );

#ifdef AREA_HIT
		LUA_GET_VALUE_ENUM( luaManager, "DAMAGE_AREA_TYPE",	pDamageData->m_eDamageAreaType,	DAMAGE_AREA_TYPE,	DAT_ALL_UNIT );
		LUA_GET_VALUE( luaManager, "DAMAGE_UNIT_NUM",		pDamageData->m_iDamageUnitNum,		0 );		
#endif
#ifdef SKILL_30_TEST
		LUA_GET_VALUE( luaManager, "NO_DAMAGE",				pDamageData->m_bNoDamage,			false );		
#endif
		LUA_GET_VALUE( luaManager, "DRAIN_HP_RATE",			pDamageData->m_fDrainHpRate,		0.f );

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		LUA_GET_VALUE( luaManager, "DRAIN_MP_REACT",			pDamageData->m_fDrainMpByReact,		0.f );
		LUA_GET_VALUE( luaManager, "DRAIN_MP_RATE_REACT",	pDamageData->m_fDrainMpRate,		1.f );
		LUA_GET_VALUE( luaManager, "LEVEL_DAMAGE",			pDamageData->m_bLvDamage,			false );		
#endif

#ifdef NO_DETONATION
		LUA_GET_VALUE( luaManager, "NO_DETONATION",			pDamageData->m_bNoDetonation,		false );
#endif
#ifdef SKILL_CASH_10_TEST		
		LUA_GET_VALUE( luaManager, "DRAIN_HP_PARTICLE_WEAPON",		pDamageData->m_bDrainHpParticle,	false );
#endif

#ifdef DUNGEON_ITEM
		LUA_GET_VALUE( luaManager, "RESIST",					pDamageData->m_ResistType, 0 );
		LUA_GET_VALUE( luaManager, "ATTACK_OUR_TEAM",		pDamageData->m_bAttackOurTeam, false );
		LUA_GET_VALUE( luaManager, "HP_PERCENT_UP",			pDamageData->m_fHpPercentUp, 0 );
		LUA_GET_VALUE( luaManager, "MP_PERCENT_UP",			pDamageData->m_fMpPercentUp, 0 );
#endif

		// ���� DamageEffect.lua �ѹ����� �ʰ� �ݿ���Ű�� ���� �ּ� ����
		// oasis907 : ����� [2010.11.26] // �׷��� ��ó - ���� ���
		LUA_GET_VALUE( luaManager, "HIT_DAMAGE_EFFECT_INVOKE_RATE",	pDamageData->m_fHitDamageEffectInvokeRate, 1.f );


#ifdef SERV_SKILL_NOTE
		LUA_GET_VALUE( luaManager, "CRITICAL_RATE",			pDamageData->m_fCriticalRate, 0.f );
		LUA_GET_VALUE( luaManager, "DECREASE_FORCE_DOWN",	pDamageData->m_fDecreaseForceDown, 0.f );
#endif

		LUA_GET_VALUE( luaManager, "NO_BUFF",				pDamageData->m_bNoBuff,		false );		

#ifdef DAMAGEDATA_RATE_MODIFIER
		LUA_GET_VALUE( luaManager, "RATE_MODIFIER",			pDamageData->m_fRateModifier,	1.f );
#endif DAMAGEDATA_RATE_MODIFIER

		LUA_GET_VALUE( luaManager, "IGNORE_DEFENCE",			pDamageData->m_bIgnoreDefence,	false );				

#ifdef WIDE_BUFF_ANI_SPEED_UP
		LUA_GET_VALUE( luaManager, "ANIMATION_SPEED_UP",			pDamageData->m_bAnimationSpeedUp, false );
		LUA_GET_VALUE( luaManager, "ANIMATION_SPEED_UP_RATE",	pDamageData->m_fAnimationSpeedUpRate, 1.0f );
#endif
		
#ifdef UPGRADE_RAVEN
		LUA_GET_VALUE_ENUM( luaManager, "ATTACK_TRIGGER_TYPE",	pDamageData->m_eDamageTrigger,		DAMAGE_TRIGGER_TYPE,	DTT_NONE );
		LUA_GET_VALUE( luaManager, "ONLY_BASIC",		pDamageData->m_bOnlyAttackBasic,	false );
		LUA_GET_VALUE( luaManager, "IGNORE_STATE_DEFENCE",			pDamageData->m_bIgnoreStateDefence,	false );
		LUA_GET_VALUE( luaManager, "NO_MISS",			pDamageData->m_bNoMiss,	false );
#endif

#ifdef ADD_BACKATTACK
		LUA_GET_VALUE( luaManager, "APPLY_BACKATTACK",		pDamageData->m_bApplyBackAttack,	true );
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		LUA_GET_VALUE( luaManager, "INCREASE_MP",			pDamageData->m_fIncreaseMP,			0.0f );		/// ��Ƽ�� �ʵ� ���� ȸ����
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef PLAY_EMOTION_BY_DAMAGE_EFFECT
		LUA_GET_VALUE( luaManager, "LAUGHTER_DAMAGE",		pDamageData->m_bIsLaughterDamage,	false );
#endif // PLAY_EMOTION_BY_DAMAGE_EFFECT

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		/// Ÿ�ݽ� �������� ��� ȸ��
		LUA_GET_VALUE( luaManager, "HIT_ADD_FORCE_POWER",	pDamageData->m_iHitAddForcePower,	0 );

		/// �ش� ������ �����͸� ������ ��ų ���̵� ( ���� ������ ��� �ؿ�... )
		LUA_GET_VALUE_ENUM( luaManager, "USED_SKILL_ID",	pDamageData->m_eUsedSkillID,	CX2SkillTree::SKILL_ID,		CX2SkillTree::SI_NONE );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        // robobeg : 2013-07-18
        // ����� �ùķ��̼ǽ� Raven POWER_ASSAULT �� ���� �󵵷� DAMAGE REACTION �� �߻��ϴ� ���
        // ������ ���׼� üũ�� ���� �ʴ´�. ( ���׼� üũ ���п� ���� ���׼� ����� 2�� �߻��ϴ� ��� ���� )
        LUA_GET_VALUE( luaManager, "RELAX_NPC_REACTION_STATE_CHECK",		pDamageData->m_bRelaxNPCReactionStateCheck,	false );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef ADD_MEMO_1ST_CLASS //��â��
		LUA_GET_VALUE( luaManager, "APPLY_CRITICAL_DAMAGE_PERCENT",			pDamageData->m_fApplyCriticalDamage,	0.0f );
		LUA_GET_VALUE( luaManager, "HIT_ADD_HP_PERCENT_ATTACKPOWER",		pDamageData->fHitAddHPbyAttackPower,	0.0f );
#endif //ADD_MEMO_1ST_CLASS

		luaManager.EndTable();

#ifndef MODIFY_SET_DAMAGE_DATA
		//������������
		if ( null != pDamageData->optrAttackerGameUnit )
		{
			if ( pDamageData->m_bMeleeAttack )	/// ���� �������� ����
				pDamageData->fForceDownValue *= pDamageData->optrAttackerGameUnit->GetAdditionalForceDownMultiplierMeleeAttack();
			else	/// ���Ÿ� �������� ����
				pDamageData->fForceDownValue *= pDamageData->optrAttackerGameUnit->GetAdditionalForceDownMultiplierRangeAttack();
		}
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef MODIFY_SET_DAMAGE_DATA
		if ( null != pDamageData->optrAttackerGameUnit )
		{
			pDamageData->optrAttackerGameUnit->SetDamageExceptionProcess( pDamageData, fPowerRate );
		}
#endif //MODIFY_SET_DAMAGE_DATA
	}
	else
	{
		return false;
	}
	ASSERT( 0.f != fPowerRate );

#ifndef MODIFY_SET_DAMAGE_DATA
	// ���� ��������
#ifdef RAVEN_SECOND_CLASS_CHANGE
	if( null != pDamageData->optrAttackerGameUnit != NULL && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = static_cast<CX2GUUser*>( pDamageData->optrAttackerGameUnit.GetObservable() );
		DAMAGE_TYPE eDamageType;
		float fDamageRel;		

		pUser->SetHitNasodWeapon(false);

		bool bChange = pUser->GetChangeDamageType(eDamageType, fDamageRel);
		if( bChange == true )
		{
			float fDamageRelOri = 1.f;

			switch(pDamageData->damageType)
			{
			case DT_PHYSIC:
				fDamageRelOri = pDamageData->damage.fPhysic;
				break;
			case DT_MAGIC:
				fDamageRelOri = pDamageData->damage.fMagic;
				break;
			default:
				break;
			}

			pDamageData->damageType = eDamageType;
			switch(eDamageType)
			{
			case DT_PHYSIC:
				pDamageData->damage.fPhysic = fDamageRelOri * fDamageRel;
				break;
			case DT_MAGIC:
				pDamageData->damage.fMagic = fDamageRelOri * fDamageRel;
				break;

	//{{ kimhc // 2010.7.30 // ������ ������ ���⸸�� ������
			case DT_WEAPON_PHYSIC:
				pDamageData->damage.fPhysic = fDamageRelOri * fDamageRel;
				break;
			case DT_WEAPON_MAGIC:
				pDamageData->damage.fMagic = fDamageRelOri * fDamageRel;
				break;
	//}} kimhc // 2010.7.30 // ������ ������ ���⸸�� ������
			default:
				break;
			}
		}
	}
#endif
#endif //MODIFY_SET_DAMAGE_DATA

#ifndef MODIFY_SET_DAMAGE_DATA
//���̻���
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
if( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
{
 	CX2GUUser *pUser = static_cast<CX2GUUser*>( pDamageData->optrAttackerGameUnit.GetObservable() );
 	int iSkillLevelFitness = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ABM_FITNESS );
 	if( iSkillLevelFitness > 0 )
 	{
 		const CX2SkillTree::SkillTemplet* pSkillTempletFitness = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ABM_FITNESS, iSkillLevelFitness );
 		if( NULL != pSkillTempletFitness && pDamageData->damageType == CX2DamageManager::DT_PHYSIC )
 		{
 			pDamageData->fForceDownValue = pDamageData->fForceDownValue * pSkillTempletFitness->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_RANGE_REL );
 			pDamageData->fHitAddMP = pDamageData->fHitAddMP * pSkillTempletFitness->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
 		}
 	}
} 
#endif
#endif //MODIFY_SET_DAMAGE_DATA

#ifndef MODIFY_SET_DAMAGE_DATA
// ���� ��������
#ifdef BUFF_TEMPLET_SYSTEM	//��ų ������ ���� ����
	if ( null != pDamageData->optrAttackerGameUnit && 
		CX2GameUnit::GUT_USER == pDamageData->optrAttackerGameUnit->GetGameUnitType() )
	{
		if( pDamageData->attackType == CX2DamageManager::AT_SPECIAL ) /// ����� ��Ƽ�� ��ų�� ��
		{
			CX2GUUser *pAttackerUser = static_cast<CX2GUUser*>( pDamageData->optrAttackerGameUnit.GetObservable() );

			const float	fSkillDamage  = pAttackerUser->GetSocketData().m_fSkillDamageUpRate;

			fPowerRate *= 1.f + fSkillDamage;

			fPowerRate *= pDamageData_->optrAttackerGameUnit->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
		}
		else		/// �Ϲ� ��ų�� ��
		{
			fPowerRate *= pDamageData_->optrAttackerGameUnit->GetAdditionalAttackByType( CX2DamageManager::AT_NORMAL );
		}
	}
#endif BUFF_TEMPLET_SYSTEM
#endif //MODIFY_SET_DAMAGE_DATA

#ifndef MODIFY_SET_DAMAGE_DATA
//������������
#ifdef BALANCE_PATCH_20120329	//PowerRate�� ����� �� �����Ƿ�, ���� �ִ� ������ �̰����� �̵��Ͽ� ����� PowerRate�� ����

	if( pDamageData->m_ExtraDamage.m_ExtraDamageType == EDT_PAIN )
		pDamageData->m_ExtraDamage.m_Damage = fPowerRate;
#endif

	if ( null != pDamageData->optrAttackerGameUnit )
	{
		if ( pDamageData->m_bMeleeAttack )
		{
			pDamageData->optrAttackerGameUnit->SetBasicStatValueChargeMpHitMeleeAttack( pDamageData->fHitAddMP );
			pDamageData->optrAttackerGameUnit->CheckAndUpdateAdditionalChargeMpHitMeleeAttack();
			pDamageData->fHitAddMP = pDamageData->optrAttackerGameUnit->GetChargeMpHitMeleeAttack();

		}
		else
		{
			pDamageData->optrAttackerGameUnit->SetBasicStatValueChargeMpHitRangeAttack( pDamageData->fHitAddMP );
			pDamageData->optrAttackerGameUnit->CheckAndUpdateAdditionalChargeMpHitRangeAttack();
			pDamageData->fHitAddMP = pDamageData->optrAttackerGameUnit->GetChargeMpHitRangeAttack();
		}
	}

	pDamageData->damage.fPhysic					*= fPowerRate;
	pDamageData->damage.fMagic					*= fPowerRate;		

	if( pDamageData->m_ExtraDamage.m_bFixedDamage == false )
	{
		pDamageData->m_ExtraDamage.m_DamagePerSec	*= fPowerRate;
		pDamageData->m_ExtraDamage.m_Damage			*= fPowerRate;
	}	
#endif //MODIFY_SET_DAMAGE_DATA


#ifdef ADD_RENA_SYSTEM //��â��
	//DamageData�� ������ ���� ��ų���� ������ ���� ����
	if ( null != pDamageData->optrAttackerGameUnit && CX2GameUnit::GUT_USER == pDamageData->optrAttackerGameUnit->GetGameUnitType() )
	{
		CX2GUUser *pUser = static_cast<CX2GUUser*>( pDamageData->optrAttackerGameUnit.GetObservable() );
		if( NULL != pUser )
		{
			pDamageData->m_RelateSkillData = pUser->GetNowDamageRelateSkillData();
		}
	}
#endif //ADD_RENA_SYSTEM

	return true;
}

bool CX2DamageManager::DamageCheck( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID )
{
	bool retval = false;
	


#ifdef AREA_HIT
	if( pDamageData->hitType == HT_AREA_HIT )
	{
		retval = AreaHit(pDamageData, bAttackOnlyThisUnit, attackUnitUID);
	}
#endif

	if( retval == false)
	{
		switch( pDamageData->attackerType )
		{
		case AT_UNIT:
			if( null != pDamageData->optrAttackerGameUnit )
			{
				retval = UnitToUnit( pDamageData, bAttackOnlyThisUnit, attackUnitUID );
				UnitToEffect( pDamageData );
			}			
			break;

		case AT_EFFECT:
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if( NULL != g_pX2Game->GetDamageEffect()->GetInstance( pDamageData->hAttackerEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( g_pX2Game->GetDamageEffect()->IsLiveInstance( pDamageData->pAttackerEffect )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{
				retval = EffectToUnit( pDamageData );
			}
			break;

		case AT_MESH:
			if( null != pDamageData->optrAttackerGameUnit )
			{
				retval = MeshToUnit( pDamageData );
			}
			break;

		case AT_MANUAL_COLLISION_DATA:
			if( null != pDamageData->optrAttackerGameUnit )
			{
				retval = CollisionDataToUnit( pDamageData );
			}
			break;
			/*
			case AT_MANUAL:
			//retval = ManualToUnit( pDamageData );
			break;
			*/
		}
	}

#ifdef GROUND_HIT
    if( pDamageData->hitType == HT_GROUND_HIT )
    {
        retval = GroundHit(pDamageData, bAttackOnlyThisUnit, attackUnitUID);             
    }
#endif

// 	if( retval == true )
// 	{
// 		if( pDamageData->optrAttackerGameUnit->IsMyUnit() 
// 			&& pDamageData->defenderType == AT_UNIT
// 			&& null != pDamageData->optrDefenderGameUnit
// 			&& pDamageData->optrDefenderGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER 
// #ifdef DUNGEON_ITEM
// 			&& pDamageData->m_fHpPercentUp <= 0.f
// 			&& pDamageData->m_fMpPercentUp <= 0.f
// #endif
// 			)
// 		{
// 			CX2GameUnit* pDefenderGameUnit = pDamageData->optrDefenderGameUnit.GetObservable();
// 			pDefenderGameUnit->GetGageManager()->DamagedFaceDraw( ( static_cast<CX2GUUser*>( pDefenderGameUnit ) )->GetUnit()->GetClass() );
// 		}
// 	}


	return retval;
}

wstring CX2DamageManager::GetExtraDamageName( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType, bool bTwoLine )
{
	int iStringIndex = STR_ID_EMPTY;



	switch( extraDamageType )
	{
	case EDT_ENCHANT_BLAZE:
        iStringIndex = STR_ID_112;
		break;

	case EDT_ENCHANT_FROZEN:
		iStringIndex = STR_ID_113;
		break;

	case EDT_ENCHANT_POISON:
		iStringIndex = STR_ID_114;
		break;

	case EDT_ENCHANT_PIERCING:
		iStringIndex = STR_ID_115;
		break;

	case EDT_ENCHANT_SHOCK:
		iStringIndex = STR_ID_116;
		break;

	case EDT_ENCHANT_SNATCH:
		iStringIndex = STR_ID_117;
		break;

	case EDT_ENCHANT_BLAZE_MASTER:
		{
			iStringIndex = STR_ID_118;
		}
		break;
		

	case EDT_ENCHANT_FROZEN_MASTER:
		{
			iStringIndex = STR_ID_119;
		}
		break;

	case EDT_ENCHANT_POISON_MASTER:
		{
			iStringIndex = STR_ID_120;
		}
		break;

	case EDT_ENCHANT_MASTER_PIERCING:
		{
			iStringIndex = STR_ID_121;
		}
		break;

	case EDT_ENCHANT_MASTER_SHOCK:
		{
			iStringIndex = STR_ID_122;
		}
		break;

	case EDT_ENCHANT_MASTER_SNATCH:
		{
			iStringIndex = STR_ID_123;
		}
		break;


	case EDT_ENCHANT_BLAZE_PIERCING:
		{
			iStringIndex = STR_ID_124;
		}
		break;

	case EDT_ENCHANT_BLAZE_SHOCK:
		{
			iStringIndex = STR_ID_125;
		}
		break;

	case EDT_ENCHANT_BLAZE_SNATCH:
		{
			iStringIndex = STR_ID_126;
		}
		break;





	case EDT_ENCHANT_FROZEN_PIERCING:
		{
			iStringIndex = STR_ID_127;
		}
		break;

	case EDT_ENCHANT_FROZEN_SHOCK:
		{
			iStringIndex = STR_ID_128;
		}
		break;

	case EDT_ENCHANT_FROZEN_SNATCH:
		{
			iStringIndex = STR_ID_129;
		}
		break;

	case EDT_ENCHANT_POISON_PIERCING:
		{
			iStringIndex = STR_ID_130;
		}
		break;

	case EDT_ENCHANT_POISON_SHOCK:
		{
			iStringIndex = STR_ID_131;
		}
		break;

	case EDT_ENCHANT_POISON_SNATCH:
		{
			iStringIndex = STR_ID_132;
		}
		break;


#ifdef TRIPLE_ENCHANT_TEST


	case EDT_ENCHANT_BLAZE_DOUBLE_MASTER:				iStringIndex = STR_ID_4542; 		break;			
	case EDT_ENCHANT_FROZEN_DOUBLE_MASTER:              iStringIndex = STR_ID_4543; 		break;			
	case EDT_ENCHANT_POISON_DOUBLE_MASTER:              iStringIndex = STR_ID_4544; 		break;			
	case EDT_ENCHANT_DOUBLE_MASTER_PIERCING:            iStringIndex = STR_ID_4545; 		break;			
	case EDT_ENCHANT_DOUBLE_MASTER_SHOCK:               iStringIndex = STR_ID_4546; 		break;			
	case EDT_ENCHANT_DOUBLE_MASTER_SNATCH:              iStringIndex = STR_ID_4547; 		break;			
	
	case EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:             iStringIndex = STR_ID_4548; 		break;			
	case EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:                iStringIndex = STR_ID_4549; 		break;			
	case EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:               iStringIndex = STR_ID_4550; 		break;			
	case EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:            iStringIndex = STR_ID_4554; 		break;			
	case EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:               iStringIndex = STR_ID_4555; 		break;			
	case EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:              iStringIndex = STR_ID_4556; 		break;			
	
	case EDT_ENCHANT_DOUBLE_POISON_PIERCING:            iStringIndex = STR_ID_4560; 		break;			
	case EDT_ENCHANT_DOUBLE_POISON_SHOCK:               iStringIndex = STR_ID_4561; 		break;			
	case EDT_ENCHANT_DOUBLE_POISON_SNATCH:              iStringIndex = STR_ID_4562; 		break;			
	case EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:             iStringIndex = STR_ID_4551; 		break;			
	case EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:                iStringIndex = STR_ID_4552; 		break;			
	case EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:               iStringIndex = STR_ID_4553; 		break;			

	case EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:            iStringIndex = STR_ID_4557; 		break;			
	case EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:               iStringIndex = STR_ID_4558; 		break;			
	case EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:              iStringIndex = STR_ID_4559; 		break;			
	case EDT_ENCHANT_POISON_DOUBLE_PIERCING:            iStringIndex = STR_ID_4563; 		break;			
	case EDT_ENCHANT_POISON_DOUBLE_SHOCK:               iStringIndex = STR_ID_4564; 		break;			
	case EDT_ENCHANT_POISON_DOUBLE_SNATCH:              iStringIndex = STR_ID_4565; 		break;			


#endif TRIPLE_ENCHANT_TEST


		
	}



	if ( bTwoLine == true )
	{
		wstring extraDamageName = GET_STRING( iStringIndex );
		std::replace( extraDamageName.begin(), extraDamageName.end(), ' ', '\n' );
		return extraDamageName;
	}
	else
	{
		return GET_STRING( iStringIndex );
	}
}

#ifdef GROUND_HIT
bool CX2DamageManager::GroundHit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect *pAttackerEffect = g_pX2Game->GetDamageEffect()->GetInstance( pDamageData->hAttackerEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect *pAttackerEffect = (CX2DamageEffect::CEffect*)pDamageData->pAttackerEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2GameUnit*	pAttacker = pDamageData->optrAttackerGameUnit.GetObservable();
    CX2GameUnit*	pDefender = NULL;
    bool firstHit = true;

	int nHitCount = 0;

    if(pAttacker == NULL && pAttackerEffect == NULL)
        return false;

    int defenderLineIndex = -1;    
    int attackLineIndex;


    CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
    ASSERT( pLineMap != NULL );
    if ( pLineMap == NULL )
        return false;

	if(pAttackerEffect != NULL)
	{
		attackLineIndex = pLineMap->GetNearestLine( pAttackerEffect->GetPos() );
	}
	else if(pAttacker != NULL)
	{
		if(pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER)
		{
			CX2GUUser *pUser = (CX2GUUser*)pAttacker;
			attackLineIndex = pLineMap->GetNearestLine( pUser->GetLandPosition() );
		}
		else
		{
			CX2GUNPC *pNPC = (CX2GUNPC*)pAttacker;
			attackLineIndex = pLineMap->GetNearestLine( pNPC->GetLandPosition_LUA() );
		}
	}
	else
	{
		return false;
	}
    


#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    if ( attackLineIndex >= pLineMap->GetNumLineData() )
        return false;
    CKTDGLineMap::LineGroupID attackLineGroup = pLineMap->GetLineGroupIncludesLineData(attackLineIndex);
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(attackLineIndex);
    if(pLineData == NULL)
        return false;
    CKTDGLineMap::LineGroup* attackLineGroup = pLineMap->GetLineGroupIncludesLineData(pLineData);
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

    for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
    {
        firstHit = true;
        pDefender = g_pX2Game->GetUnit( i );
		
		if ( NULL != pDefender )
		{
			if(pAttackerEffect != NULL)
			{
				if(GetDistance(pAttackerEffect->GetPos(), pDefender->GetPos()) > pDamageData->DamageDistance)
					continue;
			}
			else if ( NULL != pAttacker )
			{
				if(GetDistance(pAttacker->GetPos(), pDefender->GetPos()) > pDamageData->DamageDistance)
					continue;
			}

#ifdef DUNGEON_ITEM
			if( pDamageData->m_bAttackOurTeam == true )
			{
				if( NULL != pAttacker && pAttacker->GetTeam() != pDefender->GetTeam() && bAttackOnlyThisUnit == false )
					continue;

				if( pDefender->GetUnitUID() != attackUnitUID && bAttackOnlyThisUnit == true )
					continue;
			}
			else
#endif
				if( pDamageData->m_bAttackAllTeam == false )
				{
					if( NULL != pAttacker && pAttacker->GetTeam() == pDefender->GetTeam() && bAttackOnlyThisUnit == false )
						continue;

					if( pDefender->GetUnitUID() != attackUnitUID && bAttackOnlyThisUnit == true )
						continue;
				}
				else
				{
					if( pAttacker == pDefender )
						continue;
				}

//#ifdef PVP_BOSS_COMBAT_TEST
//				if( pDefender->GetTeam() != pDefender->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//				{
//					if( NULL != pAttacker && pDefender->GetTeamNumOriginal() != pAttacker->GetTeam() )
//					{
//						continue;
//					}
//				}
//#endif PVP_BOSS_COMBAT_TEST


				if( pDefender->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
					continue;

				for( int j = 0; j < (int)pDamageData->hitUnitList.size(); j++ )
				{
					HitUnit* pHitUnit = &pDamageData->hitUnitList[j];
					if( pHitUnit->optrHitGameUnit.GetObservable() == pDefender )
					{
						firstHit = false;
						break;
					}
				}
				if( firstHit == false )
					continue;            

				if(pDefender->GetGameUnitType() == CX2GameUnit::GUT_USER)
				{
					CX2GUUser *pUser = (CX2GUUser*)pDefender;
					if( pUser->GetUnitCondition().bFootOnLine == false ) 
						continue;            

					if( pDamageData->DamageLineGroup == true )
					{
						defenderLineIndex = pLineMap->GetNearestLine( pUser->GetLandPosition() );                
					}
				}
				else
				{
					CX2GUNPC *pNPC = (CX2GUNPC*)pDefender;
					if( pNPC->GetUnitCondition().bFootOnLine == false )
						continue;

					if( pNPC->GetNPCTemplet().m_ClassType == CX2UnitManager::NCT_THING_TRAP || 
						pNPC->GetNPCTemplet().m_ClassType == CX2UnitManager::NCT_THING_DEVICE 
						|| pNPC->GetNPCTemplet().m_ClassType == CX2UnitManager::NCT_THING_CHECKER 
						)
						continue;

					// ���������� ������ ������ �ʵ��� ����
					if( pNPC->GetInvincible() == true)
						continue;

					if( pDamageData->DamageLineGroup == true )
					{
						defenderLineIndex = pLineMap->GetNearestLine( pNPC->GetLandPosition_LUA() );                
					}
				}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                if( pDamageData->DamageLineGroup == true && attackLineGroup.IsValid() == true )
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
				if( pDamageData->DamageLineGroup == true && attackLineGroup != NULL )
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
				{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                    if ( defenderLineIndex >= pLineMap->GetNumLineData() )
                        return false;
                    CKTDGLineMap::LineGroupID defenderLineGroup = pLineMap->GetLineGroupIncludesLineData(defenderLineIndex);
                    if ( defenderLineGroup.IsValid() == false )
                        return false;
                    if ( defenderLineGroup != attackLineGroup )
                        continue;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
					CKTDGLineMap::LineData* pLineDataTemp = pLineMap->GetLineData(defenderLineIndex);
					if(pLineDataTemp == NULL)
						return false;

					CKTDGLineMap::LineGroup* defenderLineGroup = pLineMap->GetLineGroupIncludesLineData(pLineDataTemp);
					if(defenderLineGroup == NULL)
						return false;

					if(defenderLineGroup != attackLineGroup)
						continue;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
				}


				pDamageData->impactPoint	= pDefender->GetPos();
				pDamageData->optrDefenderGameUnit	= pDefender;
				pDamageData->reActResult	= pDamageData->reActType;            
#ifdef ADD_RENA_SYSTEM //��â��
				//ù��° Ÿ������ üũ
				//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
				if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK )
					pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
				HitUnit hitUnit;
				hitUnit.fRemainGap	= pDamageData->fHitGap;
				hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
				pDamageData->hitUnitList.push_back( hitUnit );

				pDefender->DamageReact( pDamageData );
				++nHitCount;
		}
    }   

	if(nHitCount > 0)
		return true;

	return false;
}
#endif

#ifdef AREA_HIT
bool AreaHitCmpNear(CX2DamageManager::sAreaHitUnit a1, CX2DamageManager::sAreaHitUnit a2)
{
	return a1.fDistance < a2.fDistance;
}
bool AreaHitCmpFar(CX2DamageManager::sAreaHitUnit a1, CX2DamageManager::sAreaHitUnit a2)
{
	return a1.fDistance > a2.fDistance;
}

bool CX2DamageManager::AreaHit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect *pAttackerEffect = g_pX2Game->GetDamageEffect()->GetInstance( pDamageData->hAttackerEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect *pAttackerEffect = (CX2DamageEffect::CEffect*)pDamageData->pAttackerEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2GameUnit*	pAttacker = pDamageData->optrAttackerGameUnit.GetObservable();
	CX2GameUnit*	pDefender = NULL;
	bool firstHit = true;

	if(pAttacker == NULL && pAttackerEffect == NULL)
		return false;	
	
	if( pDamageData->hitUnitList.size() >= (UINT)pDamageData->m_iDamageUnitNum )
		return false;

	std::vector<sAreaHitUnit> vecAreaHitUnitIndex;

	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		float fDistance = 9999.f;
		firstHit = true;
		pDefender = g_pX2Game->GetUnit( i );


		if(pAttackerEffect != NULL)
		{
			fDistance = GetDistance(pAttackerEffect->GetPos(), pDefender->GetPos());
			if(fDistance > pDamageData->DamageDistance)
				continue;
		}
		else 
		{
			fDistance = GetDistance(pAttacker->GetPos(), pDefender->GetPos());
			if(fDistance > pDamageData->DamageDistance)
				continue;
		}

#ifdef DUNGEON_ITEM
		if( pDamageData->m_bAttackOurTeam == true )
		{
			if( pAttacker->GetTeam() != pDefender->GetTeam() && bAttackOnlyThisUnit == false )
				continue;

			if( pDefender->GetUnitUID() != attackUnitUID && bAttackOnlyThisUnit == true )
				continue;
		}
		else
#endif
		if( pDamageData->m_bAttackAllTeam == false )
		{
			if( pAttacker->GetTeam() == pDefender->GetTeam() && bAttackOnlyThisUnit == false )
				continue;

			if( pDefender->GetUnitUID() != attackUnitUID && bAttackOnlyThisUnit == true )
				continue;
		}
		else
		{
			if( pAttacker == pDefender )
				continue;
		}

//#ifdef PVP_BOSS_COMBAT_TEST
//		if( pDefender->GetTeam() != pDefender->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//		{
//			if( pDefender->GetTeamNumOriginal() != pAttacker->GetTeam() )
//			{
//				continue;
//			}
//		}
//#endif PVP_BOSS_COMBAT_TEST


		if( pDefender->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
			continue;

		for( int j = 0; j < (int)pDamageData->hitUnitList.size(); j++ )
		{
			HitUnit* pHitUnit = &pDamageData->hitUnitList[j];
			if( pHitUnit->optrHitGameUnit.GetObservable() == pDefender )
			{
				firstHit = false;
				break;
			}
		}
		if( firstHit == false )
			continue;            

		if(pDefender->GetGameUnitType() == CX2GameUnit::GUT_NPC)
		{
			CX2GUNPC *pNPC = (CX2GUNPC*)pDefender;

			if( pNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
				continue;
			if( pNPC->IsImmuneToExtraDamage() == true && 
				( pDamageData->m_ExtraDamage.m_ExtraDamageType != CX2DamageManager::EDT_NONE || pDamageData->m_BufExtraDamage.m_ExtraDamageType != CX2DamageManager::EDT_NONE ) )
				continue;
			if( pNPC->GetInvincible() == true )
				continue;
		}

		sAreaHitUnit AreaHitUnit;
		AreaHitUnit.unitIndex = i;
		AreaHitUnit.fDistance = fDistance;		

		vecAreaHitUnitIndex.push_back(AreaHitUnit);
	}	

	switch(pDamageData->m_eDamageAreaType)
	{
	case DAT_NEAR_UNIT:
		std::sort(vecAreaHitUnitIndex.begin(), vecAreaHitUnitIndex.end(), AreaHitCmpNear);
		break;
	case DAT_FAR_UNIT:
		std::sort(vecAreaHitUnitIndex.begin(), vecAreaHitUnitIndex.end(), AreaHitCmpFar);
		break;
	}

	int nCount = 0;
	for(UINT i=0; i<vecAreaHitUnitIndex.size(); ++i)
	{
		if((int)nCount >= pDamageData->m_iDamageUnitNum)
			break;

		sAreaHitUnit AreaHitUnit = vecAreaHitUnitIndex[i];
		pDefender = g_pX2Game->GetUnit( AreaHitUnit.unitIndex );

		if( pDefender != NULL )
		{
			pDamageData->impactPoint	= pDefender->GetPos();
			pDamageData->optrDefenderGameUnit	= pDefender;
			pDamageData->reActResult	= pDamageData->reActType;            

#ifdef ADD_RENA_SYSTEM //��â��
			//ù��° Ÿ������ üũ
			//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
			if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK )
				pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
			HitUnit hitUnit;
			hitUnit.fRemainGap	= pDamageData->fHitGap;
			hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
			pDamageData->hitUnitList.push_back( hitUnit );

			pDamageData->m_bHitedManaDamage = false;
			pDefender->DamageReact( pDamageData );

			if(pDamageData->m_bDrainMana == true)
			{
				if(pDefender->GetDrainMana() == true)
					++nCount;
			}
			else
			{
				++nCount;
			}
		}		
	}   	

	if( pAttacker != NULL && pDamageData->m_bDrainMana == true )
	{
		std::wstringstream strStream;
		float fUpMP = (pDamageData->m_ExtraDamage.m_Damage * nCount);

		if( (int)fUpMP > 0 )
		{
			strStream << L"MP +" << (int)fUpMP;
			D3DXVECTOR3 pos = pAttacker->GetPos();
			pos.y += 50.0f + (0 * 30.0f);
			if( NULL != g_pData->GetPicCharGreen() )
				g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pAttacker->GetDirVector(), CKTDGPicChar::AT_CENTER );
			
			if( pAttacker != NULL && pAttacker->GetGageData() != NULL )
				pAttacker->UpNowMp( fUpMP );
		}		
	}

	if(nCount > 0)
	{
		return true;
	}

	return false;
}
#endif

bool CX2DamageManager::UnitToUnit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID )
{
	bool			retval = false;

	D3DXVECTOR3		impactPos;
	CX2GameUnit*	pAttacker = pDamageData->optrAttackerGameUnit.GetObservable();
	CX2GameUnit*	pDefender = NULL;

	//{{ 09.05.07 �¿� : Condition check�� �߰�
	if( NULL == pAttacker )
	{
		return retval;
	}
	//}}




#ifdef GRAPPLING_TEST

	switch( pDamageData->reActType )
	{
	case RT_GRAPPLE:
		{
			if( NULL != pAttacker->GetUnitGrappledByMe( false ) )
			{
				return false;
			}
		} break;

	case RT_THROW:
		{
			pDefender = pAttacker->GetUnitGrappledByMe( false );
			if( NULL == pDefender )
				return false;

			pDamageData->impactPoint	= pDefender->GetPos( false );
			pDamageData->optrDefenderGameUnit	= pDefender;
			pDamageData->reActResult	= pDamageData->reActType;

			pDefender->DamageReact( pDamageData );

			HitUnit hitUnit;
			hitUnit.fRemainGap	= pDamageData->fHitGap;
			hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
			pDamageData->hitUnitList.push_back( hitUnit );

			pAttacker->AttackResult();

			return true;
		}
	}

#endif GRAPPLING_TEST



	bool firstHit = true;
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		firstHit = true;
		pDefender = g_pX2Game->GetUnit( i );
		
		//{{ 09.05.07 �¿� : Condition check�� �߰�
		if( pDefender == NULL )
			continue;

// 		if( false == g_pX2Game->IsValidUnit( pDefender ) )
// 		{
// 			continue;
// 		}
		//}}


#ifdef UPGRADE_RAVEN
		if( pDamageData->m_bOnlyAttackBasic == true )
		{
			if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				CX2GUNPC* pNpc = (CX2GUNPC*)pDefender;
				if( pNpc != NULL && pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
					continue;
			}
		}
#endif




#ifdef GRAPPLING_TEST


		if( CX2DamageManager::RT_GRAPPLE == pDamageData->reActType )
		{
			if( NULL != pDefender->GetUnitGrapplingMe( false ) )
			{
				// �������� ���� ������ �ٽ� ���� �� ����
				continue;
			}
		}

#endif GRAPPLING_TEST



#ifdef DUNGEON_ITEM
		if( pDamageData->m_bAttackOurTeam == true )
		{
			if( pAttacker->GetTeam() != pDefender->GetTeam() && bAttackOnlyThisUnit == false )
				continue;

			if( pDefender->GetUnitUID() != attackUnitUID && bAttackOnlyThisUnit == true )
				continue;
		}
		else
#endif
		if( pDamageData->m_bAttackAllTeam == false )
		{
			if( pAttacker->GetTeam() == pDefender->GetTeam() && bAttackOnlyThisUnit == false )
				continue;

			if( pDefender->GetUnitUID() != attackUnitUID && bAttackOnlyThisUnit == true )
				continue;

#ifdef	SERV_TRAPPING_RANGER_TEST
			if( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_NPC && pAttacker->GetAllyTeam() == pDefender->GetTeam() )
			{
				continue;
			}
#endif	SERV_TRAPPING_RANGER_TEST
		}
		else
		{
			if( pAttacker == pDefender )
				continue;
		}


//#ifdef PVP_BOSS_COMBAT_TEST
//		if( pDefender->GetTeam() != pDefender->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//		{
//			if( pDefender->GetTeamNumOriginal() != pAttacker->GetTeam() )
//			{
//				continue;
//			}
//		}
//#endif PVP_BOSS_COMBAT_TEST

		


		if( pDefender->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
			continue;

		for( int j = 0; j < (int)pDamageData->hitUnitList.size(); j++ )
		{
			HitUnit* pHitUnit = &pDamageData->hitUnitList[j];
			if( pHitUnit->optrHitGameUnit.GetObservable() == pDefender )
			{
				firstHit = false;
				break;
			}
		}
		if( firstHit == false )
			continue;

		//{{ 09.05.07 �¿� : Condition check�� �߰�
		if ( m_pCollision == NULL )
			continue;
		//}}



        CKTDXCollision::CollisionType collType; 
		const CKTDXCollision::CollisionDataListSet& setDefenderCollisionDataList
			= pDefender->GetCollisionListSet();
		if( CollisionDataCheckFromUnitToUnit( pAttacker, setDefenderCollisionDataList, collType, &impactPos ) )
		{
			pDamageData->impactPoint	= impactPos;
			pDamageData->optrDefenderGameUnit	= pDefender;
            pDamageData->reActResult	= pDamageData->reActType;

            if(collType == CKTDXCollision::CT_GUARD)
                pDamageData->m_bGuard = true;
            
			bool bCheckMiss = false;
			float fHitPercent = CalcHitDodgePercent( pAttacker, pDefender );
#ifdef UPGRADE_RAVEN
			if( pDamageData->m_bNoMiss == true )
				fHitPercent = 1.f;
#endif

#ifdef DUNGEON_ITEM
			bool bMiss = false;

#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// ��Ƽ�� �ʵ� �߰�
			if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f || 
				pDamageData->m_fIncreaseMP > 0.f )
#else  SERV_CHUNG_TACTICAL_TROOPER
			if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f )
#endif SERV_CHUNG_TACTICAL_TROOPER
			{
				bMiss = false;
				bCheckMiss = false;
			}
			else
			{
				if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
				{
					bCheckMiss = true;
					bMiss = true;
				}
			}

			if( bMiss == false )
			{
#ifdef ADD_RENA_SYSTEM //��â��
				//DamageReact �Լ��� ����Ǿ� State���� �ٲ�� ���� ����� DamageData�� ����
				if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
				{
					CX2GUUser* pUser = static_cast<CX2GUUser*>(pAttacker);
					if( NULL != pUser )
						pUser->AdjustDamageDataBeforeDamageReact(pDamageData);
				}
#endif //ADD_RENA_SYSTEM
				pDefender->DamageReact( pDamageData );
			}
#else
			if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
			{
				bCheckMiss = true;
			}
			else
			{
				pDefender->DamageReact( pDamageData );
			}
#endif	
			
			if( pDamageData->reActResult == CX2DamageManager::RT_NO_DAMAGE )
				continue;
			else
			{
				if ( bCheckMiss == true )
				{
					D3DXVECTOR3 numPos = pDefender->GetBonePos( pDefender->GetHeadBoneName().c_str() );
					numPos.y += 70.0f;

					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefender ),  L"EvasionHit01", numPos );
					
					pDamageData->reActResult = CX2DamageManager::RT_NO_DAMAGE;

					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ
#ifdef	GUILD_SKILL_PART_2
					if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pDefender );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}

					if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}
#endif	GUILD_SKILL_PART_2
					//}} kimhc // 2009-11-18 // ��彺ų ������ ��ȸ

#ifdef ADD_RENA_SYSTEM //��â��
					//ù��° Ÿ������ üũ
					//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
					if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK)
						pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
					HitUnit hitUnit;
					hitUnit.fRemainGap	= pDamageData->fHitGap;
					hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
					pDamageData->hitUnitList.push_back( hitUnit );

					continue;
				}


				retval = true;
			}

#ifdef ADD_RENA_SYSTEM //��â��
			//ù��° Ÿ������ üũ
			//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
			if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK)
				pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
			HitUnit hitUnit;
			hitUnit.fRemainGap	= pDamageData->fHitGap;
			hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
			pDamageData->hitUnitList.push_back( hitUnit );

			pAttacker->AttackResult();

#ifdef UPGRADE_RAVEN
			if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker );
				pUser->AttackResultByType(*pDamageData);

#ifdef	SERV_TRAPPING_RANGER_TEST
				pUser->ProcessVPP();
#endif	SERV_TRAPPING_RANGER_TEST
			}
#endif
#ifdef ADD_RENA_SYSTEM //��â��
			else
			{
				CX2GUNPC* pNpc = static_cast<CX2GUNPC*>( pAttacker );
				pNpc->AttackResultByType(*pDamageData);
			}
#endif //ADD_RENA_SYSTEM

#ifdef GRAPPLING_TEST

			if( CX2DamageManager::RT_GRAPPLE == pDamageData->reActResult )
			{
				// �ѹ��� �Ѹ����� ���ָ� ���� �� �ִ� ������.
				return retval;
			}

#endif GRAPPLING_TEST


		}
	}

	return retval;
}

bool CX2DamageManager::EffectToUnit( DamageData* pDamageData )
{
	bool			retval = false;

	D3DXVECTOR3		impactPos;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect *pAttacker = g_pX2Game->GetDamageEffect()->GetInstance( pDamageData->hAttackerEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect *pAttacker = ( g_pX2Game->GetDamageEffect()->IsLiveInstance( (CX2DamageEffect::CEffect*)pDamageData->pAttackerEffect ) == true )
        ? (CX2DamageEffect::CEffect*)pDamageData->pAttackerEffect : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( pAttacker == NULL )
		return retval;

	CX2GameUnit*				pDefender = NULL;

	bool firstHit = true;
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		firstHit = true;
		pDefender = g_pX2Game->GetUnit( i );

		if( pDefender == NULL )
			continue;

		if( NULL == pAttacker->GetOwnerUnit() )
		{
			continue;
		}

#ifdef UPGRADE_RAVEN
		if( pDamageData->m_bOnlyAttackBasic == true )
		{
			if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				CX2GUNPC* pNpc = (CX2GUNPC*)pDefender;
				if( pNpc != NULL && pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
					continue;
			}
		}
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		CX2GameUnit* pAttackerGameUnit = pDamageData->optrAttackerGameUnit.GetObservable();

		if ( NULL == pAttacker->GetOwnerUnit()  )
			continue;

		if( CX2GameUnit::GUT_NPC == pAttacker->GetOwnerUnit()->GetGameUnitType() )
		{
			CX2GUNPC* pNPC = static_cast< CX2GUNPC* >( pAttacker->GetOwnerUnit() );

			if( NULL != pNPC )
				pNPC->AttackResultByType(*pDamageData);
		}
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef DUNGEON_ITEM
		CX2GameUnit *pAttackUnit = pAttacker->GetOwnerUnit();
		if( pDamageData->m_bAttackOurTeam == true )
		{			
			if( pAttackUnit->GetTeam() != pDefender->GetTeam() )
				continue;			
		}
		else
#endif
		if( pDamageData->m_bAttackAllTeam == false )
		{
			if( pAttacker->GetOwnerUnit()->GetTeam() == pDefender->GetTeam() )
				continue;

#ifdef	SERV_TRAPPING_RANGER_TEST
			if( pAttackUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC && pAttackUnit->GetAllyTeam() == pDefender->GetTeam() )
			{
#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
				/// ���� ���� ���� �� �ִ� NPC���� �˻� ( ���� NPC ���� üũ�� - �ʵ忡�� ��ȯ�Ǵ� ���ʹ� AllyTeam�� AllyUnitUid�� ���� �־��༭, ������ üũ �н��Ǵ� ���� ���� )
				if( false == GetPossibleAttackOurTeamID( pAttackUnit ) )
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
					continue;
			}
#endif	SERV_TRAPPING_RANGER_TEST
		}
		else
		{
			if( pAttacker->GetOwnerUnit() == pDefender )
				continue;
		}

#ifdef ADDED_RELATIONSHIP_SYSTEM
		// �ο� ��ų�� �ڱ��ڽ� or �ο����Ը� ȿ���� �����.
		if( true == pAttacker->GetOnlyRelationShip() )
		{
			CX2GUUser* pUser = static_cast< CX2GUUser* >( pDefender );
			if ( pUser == NULL ) 
				continue;
			
			if ( pUser->GetRelationshipUnitUID() != pAttacker->GetOwnerUnit()->GetUnitUID() && 
				 pAttacker->GetOwnerUnit() !=  pDefender ) 	
				continue;
		}
#endif //ADDED_RELATIONSHIP_SYSTEM

#ifdef LOCK_ON_USER_ONLY_ON
		if( true == pAttacker->GetLockOnData().m_bIsOnlyTargetAttack )
		{
			CX2GUUser* pUser = static_cast<CX2GUUser*>( pDefender );
			if( pUser == NULL )
				continue;

			if( pUser->GetUnitUID() != pAttacker->GetLockOnUnitUID() )
				continue;
		}
#endif //LOCK_ON_USER_ONLY_ON
//#ifdef PVP_BOSS_COMBAT_TEST
//		if( pDefender->GetTeam() != pDefender->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//		{
//			if( pDefender->GetTeamNumOriginal() != pAttacker->GetOwnerUnit()->GetTeam() )
//			{
//				continue;
//			}
//		}
//#endif PVP_BOSS_COMBAT_TEST		

		if( pDefender->GetGameUnitState() == CX2GameUnit::GUSI_DIE
			|| pAttacker->GetLive() == false )
			continue;

		for( int j = 0; j < (int)pDamageData->hitUnitList.size(); j++ )
		{
			HitUnit* pHitUnit = &pDamageData->hitUnitList[j];
			if( pHitUnit->optrHitGameUnit.GetObservable() == pDefender )
			{
				firstHit = false;
				break;
			}
		}
		if( firstHit == false )
			continue;

#ifdef FIX_EFFECTTOUNIT01
        CKTDGXMeshPlayer::CXMeshInstance *pAttackerMainEffect = ( pAttacker != NULL ) ? pAttacker->GetMainEffect() : NULL;
		if( pAttackerMainEffect == NULL || pAttackerMainEffect->GetXSkinAnim() == NULL )
			continue;
#endif

        CKTDXCollision::CollisionType collType;
		if( m_pCollision->CollisionDataCheck( pAttackerMainEffect->GetXSkinAnim()->GetAttackDataList(), 
												pDefender->GetCollisionListSet(), collType, &impactPos ) == true )
		{
			pDamageData->impactPoint	= impactPos;
			pDamageData->optrDefenderGameUnit	= pDefender;
            pDamageData->reActResult	= pDamageData->reActType;

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE

			/// �׷� ������ �������� ��, ������ ó���� �ؾ� �ϴ����� ���� �˻� �Լ�
			if( true == pAttacker->GetIsGroupDamage() )
			{ 
				if( true == CheckActiveGroupDamage( pAttacker->GetName(), pDamageData ) )		///	���� �׷� ������ ������
					continue;		/// �ش� ������ ������ ó�� �н�
				else																			/// �׷� ������ ��������
					SetGroupDamage( pAttacker->GetName(), pDamageData );	/// �׷� ������ ���� �����ϰ�, ������ ó�� ����
			}

#endif DAMAGE_EFFECT_GROUP_DAMAGE

            if(collType == CKTDXCollision::CT_GUARD)
                pDamageData->m_bGuard = true;

			bool bCheckMiss = false;
			float fHitPercent = CalcHitDodgePercent( pAttacker->GetOwnerUnit(), pDefender );
#ifdef UPGRADE_RAVEN
			if( pDamageData->m_bNoMiss == true )
				fHitPercent = 1.f;
#endif

#ifdef DUNGEON_ITEM
			bool bMiss = false;

#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// ��Ƽ�� �ʵ� �߰�
			if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f || 
				pDamageData->m_fIncreaseMP > 0.f )
#else  SERV_CHUNG_TACTICAL_TROOPER
			if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f )
#endif SERV_CHUNG_TACTICAL_TROOPER
			{
				bMiss = false;
				bCheckMiss = false;
			}
			else
			{
				if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
				{
					bCheckMiss = true;
					bMiss = true;
				}
			}

			if( bMiss == false )
			{
#ifdef ADD_RENA_SYSTEM //��â��
				//DamageReact �Լ��� ����Ǿ� State���� �ٲ�� ���� ����� DamageData�� ����
				if ( pAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
				{
					CX2GUUser* pUser = static_cast<CX2GUUser*>(pAttackerGameUnit);
					if( NULL != pUser )
						pUser->AdjustDamageDataBeforeDamageReact(pDamageData);
				}
#endif //ADD_RENA_SYSTEM
				pDefender->DamageReact( pDamageData );
			}
#else
			if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
			{
				bCheckMiss = true;
			}
			else
			{
				pDefender->DamageReact( pDamageData );
			}
#endif

			if( pDamageData->reActResult == CX2DamageManager::RT_NO_DAMAGE )
				continue;
			else
			{

				if ( bCheckMiss == true )
				{
					D3DXVECTOR3 numPos = pDefender->GetBonePos( pDefender->GetHeadBoneName().c_str() );
					numPos.y += 70.0f;

					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefender ),  L"EvasionHit01", numPos );
					/*
					if( NULL != g_pData->GetPicChar() )
						g_pData->GetPicChar()->DrawText( L"Miss!", numPos,  pDefender->GetDirVector(), CKTDGPicChar::AT_CENTER );

					*/
					pDamageData->reActResult = CX2DamageManager::RT_NO_DAMAGE;

					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ
#ifdef	GUILD_SKILL_PART_2
					if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pDefender );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}

					if ( pAttacker->GetOwnerUnit()->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker->GetOwnerUnit() );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}
#endif	GUILD_SKILL_PART_2
					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ

					HitUnit hitUnit;
					hitUnit.fRemainGap	= pDamageData->fHitGap;
					hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
#ifdef EVE_ELECTRA
					if( pDamageData->m_iLaserGroupID > 0 )
					{
						g_pX2Game->GetDamageEffect()->AddLaserGroupHitUnit( pDamageData->m_iLaserGroupID, hitUnit );
					}
					else
					{
#ifdef ADD_RENA_SYSTEM //��â��
						//ù��° Ÿ������ üũ
						//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
						if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK )
							pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
						pDamageData->hitUnitList.push_back( hitUnit );
					}
#else
					pDamageData->hitUnitList.push_back( hitUnit );
#endif EVE_ELECTRA
					continue;
				}


				retval = true;
			}

			HitUnit hitUnit;
			hitUnit.fRemainGap	= pDamageData->fHitGap;
			hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
#ifdef EVE_ELECTRA
			if( pDamageData->m_iLaserGroupID > 0 )
			{
				g_pX2Game->GetDamageEffect()->AddLaserGroupHitUnit( pDamageData->m_iLaserGroupID, hitUnit );
			}
			else
			{
#ifdef ADD_RENA_SYSTEM //��â��
				//ù��° Ÿ������ üũ
				//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
				if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK)
					pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
				pDamageData->hitUnitList.push_back( hitUnit );
			}
#else
			pDamageData->hitUnitList.push_back( hitUnit );
#endif EVE_ELECTRA

			if( pAttacker != NULL && g_pX2Game->GetDamageEffect()->IsLiveInstance( pAttacker ) == true )				
				pAttacker->AttackResult();
		}
	}

	return retval;
}

bool CX2DamageManager::UnitToEffect( DamageData* pDamageData )
{
	bool						retval = false;

	CX2GameUnit*				pAttacker = pDamageData->optrAttackerGameUnit.GetObservable();
	//{{ 09.05.07 �¿� : ����üũ ��ġ ����
#ifdef  X2OPTIMIZE_EFFECT_TO_UNIT_BUG_FIX
	if( NULL == pAttacker )
	{
		return retval;
	}
#else   X2OPTIMIZE_EFFECT_TO_UNIT_BUG_FIX
	if( NULL != pAttacker )
	{
		return retval;
	}
#endif  X2OPTIMIZE_EFFECT_TO_UNIT_BUG_FIX
	//}}

	//bool firstHit = true;
    CKTDXCollision*			pCollision = m_pCollision;
    if ( pCollision == NULL )
        return retval;
#ifdef  LAMBDA_RETURN_OR_LOOP_CONTINUE      
#undef  LAMBDA_RETURN_OR_LOOP_CONTINUE      
#endif  LAMBDA_RETURN_OR_LOOP_CONTINUE      
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    #define LAMBDA_RETURN_OR_LOOP_CONTINUE return
    auto UnitToEffectCB = [&retval, pDamageData, pAttacker, pCollision]( CX2DamageEffect::CEffect& kEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    #define LAMBDA_RETURN_OR_LOOP_CONTINUE continue
	for( int i = 0; i < g_pX2Game->GetDamageEffect()->GetInstanceNum(); i++ )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		//firstHit = true;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect*	pDefender = &kEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*   pDefender = g_pX2Game->GetDamageEffect()->GetInstance( i );
		if( pDefender == NULL )
			LAMBDA_RETURN_OR_LOOP_CONTINUE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		if( NULL == pDefender->GetOwnerUnit() )
		{
            LAMBDA_RETURN_OR_LOOP_CONTINUE;
		}
		
#ifdef UPGRADE_RAVEN
		if( pDamageData->m_bOnlyAttackBasic == true )
		{
			if ( pDefender->GetOwnerUnit()->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				CX2GUNPC* pNpc = (CX2GUNPC*)pDefender->GetOwnerUnit();
				if(  pNpc != NULL && pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
                    LAMBDA_RETURN_OR_LOOP_CONTINUE;
			}
		}
#endif

#ifdef DUNGEON_ITEM
		if( pDamageData->m_bAttackOurTeam == true )
		{
			if( pAttacker->GetTeam() != pDefender->GetOwnerUnit()->GetTeam() )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
		}
		else
#endif
		if( pDamageData->m_bAttackAllTeam == false )
		{
			if( pAttacker->GetTeam() == pDefender->GetOwnerUnit()->GetTeam() )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
		}
		else
		{
			if( pAttacker == pDefender->GetOwnerUnit() )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
		}

//#ifdef PVP_BOSS_COMBAT_TEST
//		if( pDefender->GetOwnerUnit()->GetTeam() != pDefender->GetOwnerUnit()->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//		{
//			if( pDefender->GetOwnerUnit()->GetTeamNumOriginal() != pAttacker->GetTeam() )
//			{
//				continue;
//			}
//		}
//#endif PVP_BOSS_COMBAT_TEST



		if( pDefender->GetLive() == false
			|| pDefender->GetAttackedByUnit() == false )
			LAMBDA_RETURN_OR_LOOP_CONTINUE;

        CKTDGXMeshPlayer::CXMeshInstance *pDefenderManiEffect = pDefender->GetMainEffect();
		if ( pDefenderManiEffect == NULL || pDefenderManiEffect->GetXSkinAnim() == NULL )
			LAMBDA_RETURN_OR_LOOP_CONTINUE;

        CKTDXCollision::CollisionType collType;
		const CKTDXCollision::CollisionDataList& listDefenderCollisionData 
			= pDefenderManiEffect->GetXSkinAnim()->GetCollisionDataList();

        // CollisionDataCheckFromUnitToEffect ������ Ǯ����ħ, lambda function ���� Ȱ���� ����, robobeg 
	    D3DXVECTOR3					impactPos;
        bool bCollisionDataCheckFromUnitToEffect = false;
	    if( pCollision->CollisionDataCheck( pAttacker->GetAttackListSet(), listDefenderCollisionData, collType, &impactPos ) )
		    bCollisionDataCheckFromUnitToEffect = true;
	    else if ( pCollision->CollisionDataCheck( pAttacker->GetSubAttackListSet(), listDefenderCollisionData, collType, &impactPos ) )
		    bCollisionDataCheckFromUnitToEffect = true;
	    else
	    {
		    const UINT uiSize = pAttacker->GetSizeOfPairSubAttackListSet();
		    for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
		    {
			    const CKTDXCollision::CollisionDataListSet* pSetSubAttackList = NULL;

			    if ( pAttacker->GetSubAttackListGetFromPair( uiIndex, &pSetSubAttackList ) 
				     && pCollision->CollisionDataCheck( *pSetSubAttackList, listDefenderCollisionData, collType, &impactPos ) )
                {
                    bCollisionDataCheckFromUnitToEffect = true;
                    break;
                }
		    }			
	    }
		//if( CollisionDataCheckFromUnitToEffect( pAttacker, listDefenderCollisionData, collType, &impactPos ) )
        if ( bCollisionDataCheckFromUnitToEffect == true )
		{
			pDamageData->impactPoint	= impactPos;

			CX2GameUnit* pDefenderOwnerGameUnit = pDefender->GetOwnerUnit();
			ASSERT( NULL != pDefenderOwnerGameUnit );

			if ( NULL != pDefenderOwnerGameUnit)
				pDamageData->optrDefenderGameUnit = pDefenderOwnerGameUnit;

			pDamageData->reActResult = pDamageData->reActType;
			pDefender->DamageReact( pDamageData );

			if( pDamageData->reActResult == CX2DamageManager::RT_NO_DAMAGE )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
			else
				retval = true;
		}
	};
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    g_pX2Game->GetDamageEffect()->ApplyFunctionToLiveInstances( UnitToEffectCB );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	return retval;
}

bool CX2DamageManager::MeshToUnit( DamageData* pDamageData )
{
    //{{ seojt // 2009-1-12, 16:16
    if( pDamageData == NULL )
    {
        return false;
    }//if
    //}} seojt // 2009-1-12, 16:16

	bool		retval = false;

	D3DXVECTOR3		impactPos;
	CX2GameUnit*	pAttacker = pDamageData->optrAttackerGameUnit.GetObservable();
	CX2GameUnit*	pDefender = NULL;

	bool firstHit = true;
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		firstHit = true;
		pDefender = g_pX2Game->GetUnit( i );

		if( pDefender == NULL )
			continue;

#ifdef UPGRADE_RAVEN
		if( pDamageData->m_bOnlyAttackBasic == true )
		{
			if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				CX2GUNPC* pNpc = (CX2GUNPC*)pDefender;
				if(  pNpc != NULL && pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
					continue;
			}
		}
#endif

#ifdef DUNGEON_ITEM
		if( pDamageData->m_bAttackOurTeam == true )
		{
			if( pAttacker->GetTeam() != pDefender->GetTeam() )
				continue;
		}
		else
#endif
		if( pDamageData->m_bAttackAllTeam == false )
		{
			if( pAttacker->GetTeam() == pDefender->GetTeam() )
				continue;
		}
		else
		{
			if( pAttacker == pDefender )
				continue;
		}


//#ifdef PVP_BOSS_COMBAT_TEST
//		if( pDefender->GetTeam() != pDefender->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//		{
//			if( pDefender->GetTeamNumOriginal() != pAttacker->GetTeam() )
//			{
//				continue;
//			}
//		}
//#endif PVP_BOSS_COMBAT_TEST





		if( pDefender->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
			continue;

		for( int j = 0; j < (int)pDamageData->hitUnitList.size(); j++ )
		{
			HitUnit* pHitUnit = &pDamageData->hitUnitList[j];
			if( pHitUnit->optrHitGameUnit.GetObservable() == pDefender )
			{
				firstHit = false;
				break;
			}
		}
		if( firstHit == false )
			continue;

        CKTDXCollision::CollisionType collType;
		if( pDamageData->pAttackerAnim != NULL
            && m_pCollision->CollisionDataCheck( pDamageData->pAttackerAnim->GetAttackDataList(), pDefender->GetCollisionListSet(), collType, &impactPos ) == true )
		{
			pDamageData->impactPoint	= impactPos;
			pDamageData->optrDefenderGameUnit	= pDefender;
            pDamageData->reActResult = pDamageData->reActType;

            if(collType == CKTDXCollision::CT_GUARD)
                pDamageData->m_bGuard = true;			    

			bool bCheckMiss = false;
			float fHitPercent = CalcHitDodgePercent( pAttacker, pDefender );
#ifdef UPGRADE_RAVEN
			if( pDamageData->m_bNoMiss == true )
				fHitPercent = 1.f;
#endif

#ifdef DUNGEON_ITEM
			bool bMiss = false;

#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// ��Ƽ�� �ʵ� �߰�
			if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f || 
				pDamageData->m_fIncreaseMP > 0.f )
#else  SERV_CHUNG_TACTICAL_TROOPER
			if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f )
#endif SERV_CHUNG_TACTICAL_TROOPER
			{
				bMiss = false;
				bCheckMiss = false;
			}
			else
			{
				if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
				{
					bCheckMiss = true;
					bMiss = true;
				}
			}

			if( bMiss == false )
			{
#ifdef ADD_RENA_SYSTEM //��â��
				//DamageReact �Լ��� ����Ǿ� State���� �ٲ�� ���� ����� DamageData�� ����
				if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
				{
					CX2GUUser* pUser = static_cast<CX2GUUser*>(pAttacker);
					if( NULL != pUser )
						pUser->AdjustDamageDataBeforeDamageReact(pDamageData);
				}
#endif //ADD_RENA_SYSTEM
				pDefender->DamageReact( pDamageData );
			}
#else
			if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
			{
				bCheckMiss = true;
			}
			else
			{
				pDefender->DamageReact( pDamageData );
			}
#endif

			if( pDamageData->reActResult == CX2DamageManager::RT_NO_DAMAGE )
				continue;
			else
			{


				if ( bCheckMiss == true )
				{
					D3DXVECTOR3 numPos = pDefender->GetBonePos( pDefender->GetHeadBoneName().c_str() );
					numPos.y += 70.0f;

					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefender ),  L"EvasionHit01", numPos );
					/*
					if( NULL != g_pData->GetPicChar() )
						g_pData->GetPicChar()->DrawText( L"Miss!", numPos,  pDefender->GetDirVector(), CKTDGPicChar::AT_CENTER );

						*/

					pDamageData->reActResult = CX2DamageManager::RT_NO_DAMAGE;

					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ
#ifdef	GUILD_SKILL_PART_2
					if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pDefender );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}

					if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}
#endif	GUILD_SKILL_PART_2
					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ

#ifdef ADD_RENA_SYSTEM //��â��
					//ù��° Ÿ������ üũ
					//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
					if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK )
						pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
					HitUnit hitUnit;
					hitUnit.fRemainGap	= pDamageData->fHitGap;
					hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
					pDamageData->hitUnitList.push_back( hitUnit );

					continue;
				}

				retval = true;
			}

#ifdef ADD_RENA_SYSTEM //��â��
			//ù��° Ÿ������ üũ
			//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
			if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK)
				pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
			HitUnit hitUnit;
			hitUnit.fRemainGap	= pDamageData->fHitGap;
			hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
			pDamageData->hitUnitList.push_back( hitUnit );

			pAttacker->AttackResult();
#ifdef UPGRADE_RAVEN
			if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker );
				pUser->AttackResultByType(*pDamageData);
			}
#endif
		}
	}

	return retval;
}

bool CX2DamageManager::CollisionDataToUnit( DamageData* pDamageData )
{
	bool		retval = false;

	D3DXVECTOR3		impactPos;
	CX2GameUnit*	pAttacker = pDamageData->optrAttackerGameUnit.GetObservable();
	CX2GameUnit*	pDefender = NULL;

	bool firstHit = true;
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		firstHit = true;
		pDefender = g_pX2Game->GetUnit( i );

		if( pDefender == NULL )
			continue;

#ifdef UPGRADE_RAVEN
		if( pDamageData->m_bOnlyAttackBasic == true )
		{
			if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				CX2GUNPC* pNpc = (CX2GUNPC*)pDefender;
				if(  pNpc != NULL && pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
					continue;
			}
		}
#endif

#ifdef DUNGEON_ITEM
		if( pDamageData->m_bAttackOurTeam == true )
		{
			if( pAttacker->GetTeam() != pDefender->GetTeam() )
				continue;
		}
		else
#endif
		if( pDamageData->m_bAttackAllTeam == false )
		{
			if( pAttacker->GetTeam() == pDefender->GetTeam() )
				continue;
		}
		else
		{
			if( pAttacker == pDefender )
				continue;
		}


//#ifdef PVP_BOSS_COMBAT_TEST
//		if( pDefender->GetTeam() != pDefender->GetTeamNumOriginal() )		// ������ ���ӿ��� ������ ����� �ڱ��� ���� �� �ִ�
//		{
//			if( pDefender->GetTeamNumOriginal() != pAttacker->GetTeam() )
//			{
//				continue;
//			}
//		}
//#endif PVP_BOSS_COMBAT_TEST






		if( pDefender->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
			continue;

		for( int j = 0; j < (int)pDamageData->hitUnitList.size(); j++ )
		{
			HitUnit* pHitUnit = &pDamageData->hitUnitList[j];
			if( pHitUnit->optrHitGameUnit.GetObservable() == pDefender )
			{
				firstHit = false;
				break;
			}
		}
		if( firstHit == false )
			continue;

        CKTDXCollision::CollisionType collType;
		if( m_pCollision->CollisionDataCheck( pDefender->GetCollisionListSet(), pDamageData->pCollisionData, collType, &impactPos ) == true )
		{
			pDamageData->impactPoint	= impactPos;
			pDamageData->optrDefenderGameUnit	= pDefender;
            pDamageData->reActResult	= pDamageData->reActType;

            if(collType == CKTDXCollision::CT_GUARD)
                pDamageData->m_bGuard = true;			

			pDefender->DamageReact( pDamageData );

			if( pDamageData->reActResult == CX2DamageManager::RT_NO_DAMAGE )
				continue;
			else
			{

				float fHitPercent = CalcHitDodgePercent( pAttacker, pDefender );

#ifdef UPGRADE_RAVEN
				if( pDamageData->m_bNoMiss == true )
					fHitPercent = 1.f;
#endif

#ifdef DUNGEON_ITEM
				bool bMiss = false;

#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// ��Ƽ�� �ʵ� �߰�
				if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f || 
					pDamageData->m_fIncreaseMP > 0.f  )
#else  SERV_CHUNG_TACTICAL_TROOPER
				if( pDamageData->m_fHpPercentUp > 0.f || pDamageData->m_fMpPercentUp > 0.f )
#endif SERV_CHUNG_TACTICAL_TROOPER
				{
					bMiss = false;
				}
				else
				{
					if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
					{
						bMiss = true;
					}
				}

#else
				if ( pDefender->GetRandomFloat( CKTDXRandomNumbers::SRO_HIT_DODGE ) > fHitPercent )
				{
					bMiss = true;
				}
#endif

				if ( bMiss == true )
				{
					D3DXVECTOR3 numPos = pDefender->GetBonePos( pDefender->GetHeadBoneName().c_str() );
					numPos.y += 70.0f;


					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefender ),  L"EvasionHit01", numPos );
					/*
					if( NULL != g_pData->GetPicChar() )
						g_pData->GetPicChar()->DrawText( L"Miss!", numPos,  pDefender->GetDirVector(), CKTDGPicChar::AT_CENTER );
*/
					pDamageData->reActResult = CX2DamageManager::RT_NO_DAMAGE;

					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ
#ifdef	GUILD_SKILL_PART_2
					if ( pDefender->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pDefender );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}

					if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
					{
						CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker );
						pUser->GuildSkillProcess( CX2SkillTree::SI_GP_COMMON_CHANCE_TO_REVERSE );
					}
#endif	GUILD_SKILL_PART_2
					//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ

#ifdef ADD_RENA_SYSTEM //��â��
					//ù��° Ÿ������ üũ
					//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
					if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK )
						pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
					HitUnit hitUnit;
					hitUnit.fRemainGap	= pDamageData->fHitGap;
					hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
					pDamageData->hitUnitList.push_back( hitUnit );

					continue;
				}


				retval = true;
			}

#ifdef ADD_RENA_SYSTEM //��â��
			//ù��° Ÿ������ üũ
			//UnitList�� ����ִ� ���¿��� FAC_NOT_CHECK ���¶�� ù��° Ÿ��
			if( pDamageData->hitUnitList.empty() == true && pDamageData->m_eFirstAttack != FAC_NOT_CHECK )
				pDamageData->m_eFirstAttack = FAC_FIRST_ATTACK;
#endif //ADD_RENA_SYSTEM
			HitUnit hitUnit;
			hitUnit.fRemainGap	= pDamageData->fHitGap;
			hitUnit.optrHitGameUnit	= pDamageData->optrDefenderGameUnit;
			pDamageData->hitUnitList.push_back( hitUnit );

			pAttacker->AttackResult();
#ifdef UPGRADE_RAVEN
			if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				CX2GUUser* pUser = static_cast< CX2GUUser* >( pAttacker );
				pUser->AttackResultByType(*pDamageData);
			}
#endif
		}
	}

	return retval;
}



bool CX2DamageManager::OpenDamageSoundScript( const WCHAR* wszFileName )
{
	if ( NULL == wszFileName || NULL == wszFileName[0] )
		return false;

//{{ robobeg : 2008-10-28
	//KLuaManager luaManager;
    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
// 	if( false == g_pKTDXApp->LoadLuaTinker( wszFileName ) )
// 		return false;
	//if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, L"Enum.lua" ) )
		//return false;
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, wszFileName ) )
		return false;

	if ( false == luaManager.ExportFunctionsToGlobalEnv() )
		return false;

//}} robobeg : 2008-10-28


	ParseDamageSoundBlock( luaManager, HT_DEFAULT,			"HT_DEFAULT" );
	ParseDamageSoundBlock( luaManager, HT_SWORD_SLASH,		"HT_SWORD_SLASH" );
	ParseDamageSoundBlock( luaManager, HT_SWORD_SLASH2,		"HT_SWORD_SLASH2" );
	ParseDamageSoundBlock( luaManager, HT_SWORD_HIT,		"HT_SWORD_HIT" );
	ParseDamageSoundBlock( luaManager, HT_SWORD_CRASH,		"HT_SWORD_CRASH" );
	ParseDamageSoundBlock( luaManager, HT_ROD_SLASH,		"HT_ROD_SLASH" );
	ParseDamageSoundBlock( luaManager, HT_ROD_SLASH2,		"HT_ROD_SLASH2" );
	ParseDamageSoundBlock( luaManager, HT_ROD_HIT,			"HT_ROD_HIT" );
	ParseDamageSoundBlock( luaManager, HT_ROD_CRASH,		"HT_ROD_CRASH" );
	ParseDamageSoundBlock( luaManager, HT_PUNCH_HIT,		"HT_PUNCH_HIT" );
	ParseDamageSoundBlock( luaManager, HT_PUNCH_HIT2,		"HT_PUNCH_HIT2" );
	ParseDamageSoundBlock( luaManager, HT_PUNCH_CRASH,		"HT_PUNCH_CRASH" );
	ParseDamageSoundBlock( luaManager, HT_KICK_SLASH,		"HT_KICK_SLASH" );
	ParseDamageSoundBlock( luaManager, HT_KICK_SLASH2,		"HT_KICK_SLASH2" );
	ParseDamageSoundBlock( luaManager, HT_KICK_HIT,			"HT_KICK_HIT" );
	ParseDamageSoundBlock( luaManager, HT_KICK_CRASH,		"HT_KICK_CRASH" );
	ParseDamageSoundBlock( luaManager, HT_ARROW_HIT,		"HT_ARROW_HIT" );
	ParseDamageSoundBlock( luaManager, HT_ARROW_HIT2,		"HT_ARROW_HIT2" );
	ParseDamageSoundBlock( luaManager, HT_ARROW_CRASH,		"HT_ARROW_CRASH" );
	ParseDamageSoundBlock( luaManager, HT_FIRE,				"HT_FIRE" );
	ParseDamageSoundBlock( luaManager, HT_FIRE2,			"HT_FIRE2" );
	ParseDamageSoundBlock( luaManager, HT_BOMB,				"HT_BOMB" );
	ParseDamageSoundBlock( luaManager, HT_WOOD,				"HT_WOOD" );
	ParseDamageSoundBlock( luaManager, HT_LIGHTNING,		"HT_LIGHTNING" );
	ParseDamageSoundBlock( luaManager, HT_NASOD_KING,		"HT_NASOD_KING" );
	ParseDamageSoundBlock( luaManager, HT_METAL_PUNCH_HIT,	"HT_METAL_PUNCH_HIT" );
#ifdef GROUND_HIT
	ParseDamageSoundBlock( luaManager, HT_GROUND_HIT,		"HT_GROUND_HIT" );
#endif GROUND_HIT
	ParseDamageSoundBlock( luaManager, HT_SLAP,				"HT_SLAP" );
	ParseDamageSoundBlock( luaManager, HT_EVE_SLASH1, 		"HT_EVE_SLASH1" );
	ParseDamageSoundBlock( luaManager, HT_EVE_SLASH2, 		"HT_EVE_SLASH2" );

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	ParseDamageSoundBlock( luaManager, HT_CHUNG_SLASH1,		"HT_CHUNG_SLASH1" );
	ParseDamageSoundBlock( luaManager, HT_CHUNG_SLASH2, 	"HT_CHUNG_SLASH2" );
	ParseDamageSoundBlock( luaManager, HT_CHUNG_SLASH3, 	"HT_CHUNG_SLASH3" );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2011.1.21 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	ParseDamageSoundBlock( luaManager, HT_HANDGUN_HIT1,		"HT_HANDGUN_HIT1" );
	ParseDamageSoundBlock( luaManager, HT_HANDGUN_HIT2, 	"HT_HANDGUN_HIT2" );
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // û 1�� ����

#ifdef AREA_HIT
	ParseDamageSoundBlock( luaManager, HT_AREA_HIT,			"HT_AREA_HIT" );
#endif AREA_HIT
	ParseDamageSoundBlock( luaManager, HT_SPEAR_HIT,		"HT_SPEAR_HIT" );

#ifdef ARA_CHARACTER_BASE	/// �ƶ� ���� ����
	ParseDamageSoundBlock( luaManager, HT_POLE_BRANDISH_WEAK,	"HT_POLE_BRANDISH_WEAK"	);
	ParseDamageSoundBlock( luaManager, HT_POLE_BRANDISH_STRONG,	"HT_POLE_BRANDISH_STRONG"	);
	ParseDamageSoundBlock( luaManager, HT_SHORTSWORD_SLASH,		"HT_SHORTSWORD_SLASH"		);
	ParseDamageSoundBlock( luaManager, HT_SHORTSWORD_PIERCE,	"HT_SHORTSWORD_PIERCE"		);
#endif

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
	ParseDamageSoundBlock( luaManager, HT_ELECTRIC_1,			"HT_ELECTRIC_1"			);
	ParseDamageSoundBlock( luaManager, HT_ELECTRIC_2,			"HT_ELECTRIC_2"			);
	ParseDamageSoundBlock( luaManager, HT_PLASMA_1,				"HT_PLASMA_1"				);
	ParseDamageSoundBlock( luaManager, HT_PLASMA_2,				"HT_PLASMA_2"				);
	ParseDamageSoundBlock( luaManager, HT_PRESSURE_1,			"HT_PRESSURE_1"			);
	ParseDamageSoundBlock( luaManager, HT_PRESSURE_2,			"HT_PRESSURE_2"			);
	ParseDamageSoundBlock( luaManager, HT_WATER_1,				"HT_WATER_1"				);
	ParseDamageSoundBlock( luaManager, HT_WATER_2,				"HT_WATER_2"				);
	ParseDamageSoundBlock( luaManager, HT_WATER_3,				"HT_WATER_3"				);
	ParseDamageSoundBlock( luaManager, HT_ICE,					"HT_ICE"					);
	ParseDamageSoundBlock( luaManager, HT_METAL,				"HT_METAL"					);
	ParseDamageSoundBlock( luaManager, HT_WIND,					"HT_WIND"					);
	ParseDamageSoundBlock( luaManager, HT_STONE,				"HT_STONE"					);
#endif //SERV_9TH_NEW_CHARACTER


	for( int i=0; i<(int)HIT_TYPE_COUNT; i++ )
	{
		for( int j=0; j<(int)HITTED_TYPE_COUNT; j++ )
		{
			std::wstring& wstrSoundName = m_DamageSoundName[i][j];
			if( 0 != wstrSoundName.length() )
			{
				CKTDXDeviceSound* pDeviceSound = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrSoundName.c_str() );
				if( NULL != pDeviceSound )
				{
					m_pvecReadySound.push_back( pDeviceSound );
				}
			}
		}
	}

	return true;
	
}




bool CX2DamageManager::ParseDamageSoundBlock( KLuaManager& luaManager, HIT_TYPE eHitType, const char* pszTableNameUTF8 )
{
	if( pszTableNameUTF8 == NULL || pszTableNameUTF8[0] == NULL )
		return false;

	if( true == luaManager.BeginTable( pszTableNameUTF8 ) )
	{
		LUA_GET_VALUE( luaManager, "HTD_DEFAULT",		m_DamageSoundName[eHitType][HTD_DEFAULT],		L"" );
		LUA_GET_VALUE( luaManager, "HTD_MEAT",			m_DamageSoundName[eHitType][HTD_MEAT],			L"" );
		LUA_GET_VALUE( luaManager, "HTD_WOOD",			m_DamageSoundName[eHitType][HTD_WOOD],			L"" );

		LUA_GET_VALUE( luaManager, "HTD_STONE",			m_DamageSoundName[eHitType][HTD_STONE],			L"" );
		LUA_GET_VALUE( luaManager, "HTD_STONE2",		m_DamageSoundName[eHitType][HTD_STONE2],		L"" );
		
		LUA_GET_VALUE( luaManager, "HTD_METAL",			m_DamageSoundName[eHitType][HTD_METAL],			L"" );
		LUA_GET_VALUE( luaManager, "HTD_METAL2",		m_DamageSoundName[eHitType][HTD_METAL2],		L"" );

		LUA_GET_VALUE( luaManager, "HTD_BEEHOUSE",		m_DamageSoundName[eHitType][HTD_BEEHOUSE],		L"" );

//{{ kimhc // 2011.1.21 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		LUA_GET_VALUE( luaManager, "HTD_GUARD_1",		m_DamageSoundName[eHitType][HTD_GUARD_1],		L"" );
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // û 1�� ����

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		LUA_GET_VALUE( luaManager, "HTD_GUARD_2",		m_DamageSoundName[eHitType][HTD_GUARD_2],		L"" );
#endif //UPGRADE_SKILL_SYSTEM_2013

		luaManager.EndTable();
	}
	else
	{
		return false;
	}
	return true;
}

const WCHAR* CX2DamageManager::GetDamageSound( HIT_TYPE hitType, HITTED_TYPE hittedType )
{	
	if( HT_NO_SOUND == hitType ||
		HTD_NO_SOUND == hittedType )
	{
		return NULL;
	}


	std::wstring& wstrSoundName = m_DamageSoundName[hitType][hittedType];
	if( 0 == wstrSoundName.length() )
	{
		std::wstring& wstrSoundName2 = m_DamageSoundName[hitType][HTD_DEFAULT];
		if( 0 == wstrSoundName2.length() )
		{
			std::wstring& wstrSoundName3 = m_DamageSoundName[HT_DEFAULT][HTD_DEFAULT];
			if( 0 == wstrSoundName3.length() )
			{
				return NULL;
			}
			else
			{
				return wstrSoundName3.c_str();
			}
		}
		else
		{
			return wstrSoundName2.c_str();
		}
	}
	else
	{
		return wstrSoundName.c_str();
	}
}



// ���� Ȯ���� ����ؼ� 0~1������ ���� return
float CX2DamageManager::CalcHitDodgePercent( CX2GameUnit* pAttacker, CX2GameUnit* pDefender )
{
	// ���� ���� Ȯ�� = (100 + attacker �߰� ����) - defender�� ȸ��		

	if( NULL == pAttacker )
		return 1.f;

	if( NULL == pDefender )
		return 1.f;

#ifdef NOMISS_REVENGE
	if( pDefender->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = (CX2GUUser*)pDefender;
		
		const CX2GUUser::FrameData& kFrameData = pUser->GetNowFrameData();
		const CKTDGXSkinAnim *pSkinAnim = pUser->GetXSkinAnim();
		if( pSkinAnim == NULL )
			return 1.f;

		// �ݰݿ��ΰ˻�
		if( kFrameData.stateParam.fRevengeStartTime < pSkinAnim->GetNowAnimationTime() 
			&& kFrameData.stateParam.fRevengeEndTime > pSkinAnim->GetNowAnimationTime() 
			&& kFrameData.unitCondition.bAttackerFront == true )
		{
			return 1.f;
		}	
		// �����ݻ翩�� �˻�
		if( kFrameData.stateParam.fReflexMagicStartTime < pSkinAnim->GetNowAnimationTime() 
			&& kFrameData.stateParam.fReflexMagicEndTime > pSkinAnim->GetNowAnimationTime() 
			&& kFrameData.unitCondition.bAttackerFront == true )
		{
			return 1.f;
		}
	}	
#endif

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	/// ������ ���� ����ü �ݻ�
	if ( false == pDefender->GetVecReflectMagicByBuffPtr().empty() )
		return 1.f;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	// ������ ���Ϳ� ���� ȸ�� Ȯ��
	float fExtraEvadeRate = 0.f;
	if ( pAttacker != NULL && pAttacker->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		if ( pDefender != NULL )
		{
			CX2GUNPC* pGUNPC = (CX2GUNPC*)pAttacker;
			fExtraEvadeRate = pDefender->GetEvadeUpPerBySMA( pGUNPC->GetNPCTemplet().m_nNPCUnitID );
		}
	}
	
	float fAttackerHitRate = 1.f;
	fAttackerHitRate *= pAttacker->GetAccuracyPercent();
#ifdef NEW_SKILL_2010_11
	// oasis907 : ����� [2010.11.8] // �ε� ����Ʈ - ���� ���� - EDT_BLIND
	fAttackerHitRate *= pAttacker->GetAttackRateByAbnormalityBlind();
#endif NEW_SKILL_2010_11

	/// �⺻ ȸ������ 0�� �⺻�̳� Avoidance��ġ�� 1.0�� �⺻�̱� ������ 1.0�� ���ش�.
	float fDefenderEvadeRate = ( pDefender->GetAvoidancePercent() + fExtraEvadeRate - 1.0f );
	float fHitRate = fAttackerHitRate - fDefenderEvadeRate;

	//{{ JHKang / ������ / 2011.7.4 / û 2�� ���� ���鸮 ü�̼� ������ ��ݼ�(���߷� ����)
#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = (CX2GUUser*)pAttacker;

		if ( pUser->GetUnitClass() == CX2Unit::UC_CHUNG_DEADLY_CHASER )
		{
			const CX2Unit::UnitData*	pUnitData	= &pUser->GetUnit()->GetUnitData();

		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			int iSkillLevel = pUnitData->m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP, true );
			if ( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP );

				if ( NULL != pSkillTemplet )
				{
					float fAccurateMarkmanshipRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ACCURACY_PERCENT_ABS, iSkillLevel );
					fHitRate += fAccurateMarkmanshipRate;
				}
			}
		#else // UPGRADE_SKILL_SYSTEM_2013
			int iSkillLevel = pUnitData->m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP );
			if ( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP, iSkillLevel );

				if ( NULL != pSkillTemplet )
				{
					float fAccurateMarkmanshipRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ACCURACY_PERCENT_ABS );
					fHitRate += fAccurateMarkmanshipRate;
				}
			}
		#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif
	//}}
	
#ifdef UPGRADE_SKILL_SYSTEM_2013
	// Raven ��ų ����, ������
	if ( pAttacker->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = (CX2GUUser*)pAttacker;

		if ( pUser->GetUnitClass() == CX2Unit::UC_RAVEN_BLADE_MASTER )
		{
			const CX2Unit::UnitData*	pUnitData	= &pUser->GetUnit()->GetUnitData();
			// Raven ��ų ����, ������ �˼�
			// ������ ����
			int iSkillLevel = pUnitData->m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RST_EXQUISITE_SWORDMANSHIP, true );
			if ( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RST_EXQUISITE_SWORDMANSHIP );

				if ( NULL != pSkillTemplet )
				{
					float fAccurateSwordmanshipRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ACCURACY_PERCENT_ABS, iSkillLevel );
					fHitRate += fAccurateSwordmanshipRate;
				}
			}
		}
	}

	//��â��
	if( CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType() )
	{
		CX2GUUser* pUser = static_cast<CX2GUUser*>(pAttacker);
		
		if ( NULL != pUser->GetUnit() )
		{
			// ĳ���Ͱ� ���̺�, ���ҵ� �� �� �ش� ���� �����ϵ��� ����, kimjh
	//		if ( CX2Unit::UT_ELSWORD == pUser->GetUnit()->GetType() || CX2Unit::UT_RAVEN == pUser->GetUnit()->GetType() )
	//		if( CX2Unit::UC_ELSWORD_INFINITY_SWORD == pUser->GetUnitClass() || CX2Unit::UC_ELSWORD_SHEATH_KNIGHT == pUser->GetUnitClass() )
			{
				const CX2Unit::UnitData* pUnitData = &pUser->GetUnit()->GetUnitData();

				int iSkillLevel = pUnitData->m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_GET_CHANCE, true );
				if( iSkillLevel > 0 )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_GET_CHANCE );
					if( NULL != pSkillTemplet )
					{
						float fRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ACCURACY_PERCENT_ABS, iSkillLevel );
						fHitRate += fRate;
					}
				}
			}
		}
	}

#endif //UPGRADE_SKILL_SYSTEM_2013
	
#ifdef NEW_CHARACTER_EL
	if ( CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType())
	{
		CX2GUUser* pGUUser = static_cast<CX2GUUser*>( pAttacker );
		if( NULL != pGUUser )
		{
			const float fAddAccuaracyPercent = pGUUser->GetAddAccuaracyPercent();
			if( 0 < fAddAccuaracyPercent )
				fHitRate += fAddAccuaracyPercent ;
		}
	}
#endif //NEW_CHARACTER_EL

	if ( pDefender->GetInvincible() == true )
		fHitRate = 1.f;

	return fHitRate;
}

#ifdef NEW_EXTRA_DAMAGE
float CX2DamageManager::ExtraDamageData::GetCalcPerDamage(int iUnitLevel, float fResist) const
{	
	// �ʴ� ������ ���� (������/Ÿ�Ժ� �������� ��ȯ)
	float fPerDamage = 0.f;	
	int iLevel = m_iLevel;

	if(m_iLevel < 0)
		return m_DamagePerSec;

	if(m_iLevel == 0)
	{
		iLevel = iUnitLevel;
		iLevel += m_iAddLevel;
	}
		
	 iLevel -= 1;
	 if(iLevel < 0)
		 return m_DamagePerSec;

	switch(m_ExtraDamageType)
	{
	case CX2DamageManager::EDT_POISON:	
		fPerDamage = 80.f;
		break;
	case CX2DamageManager::EDT_FIRE:
		fPerDamage = 145.f;
		break;
	case CX2DamageManager::EDT_COLD:
		fPerDamage = 60.f;
		break;
	case CX2DamageManager::EDT_LEG_WOUND:
		fPerDamage = 82.f;
		break;
	}

	if(g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_PVP)
	{
		if( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_FREE ||
			g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_PLAY ||
			g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_TOURNAMENT )
		{
			// ����ä��,����ä��,��ȸä�� : iLevel�� 1�� ���� (�ᱹ�� 0)
			fPerDamage = fPerDamage;
		}
		else
		{
			// ������ ä���� iLevel�� 0.1�� �����ش�.
			fPerDamage = (fPerDamage + (fPerDamage * 0.2f * ( iLevel * 0.1f )));
		}
	}
	else
	{
		fPerDamage = (fPerDamage + (fPerDamage * 0.2f * iLevel));
	}	

	fPerDamage *= (1.f - (fResist / CX2EnchantItem::EAR_MAX_VALUE));

	return fPerDamage;	

	// IgnoreResist�� ���� Ȯ�� ����
	// ���ӽð� (���װ� ���� ���� �ʵ��� ����)
	// Ȯ���� Ÿ�ݽ� ������ ����, ����� ����/���װ��� ���� ����
}
#endif

#ifdef ELSWORD_WAY_OF_SWORD

float CX2DamageManager::CalcWayOfSwordPoint( WAY_OF_SWORD_TYPE wayOfSwordType_, float fDamageRate_, int iWayOfSwordState )
{
	float fWSPGain = 0.f;

	float fSkillTypeConst = 0.f;
	bool bVigor = false;
	switch(wayOfSwordType_)
	{
	case WST_NONE:
		fSkillTypeConst = 0.f;
		break;
	case WST_VIGOR_NORMAL:
		{
			bVigor = true;
			fSkillTypeConst = m_WayOfSwordConstTable.m_fSkillTypeConst_VigorCommand;
		}
		break;
	case WST_VIGOR_ACTIVE:
		{
			bVigor = true;
			fSkillTypeConst = m_WayOfSwordConstTable.m_fSkillTypeConst_VigorActive;
		}
		break;
	case WST_VIGOR_SPECIAL:
		{
			bVigor = true;
			fSkillTypeConst = m_WayOfSwordConstTable.m_fSkillTypeConst_VigorSpecialActive;
		}
		break;
	case WST_DESTRUCTION_NORMAL:
		{
			bVigor = false;
			fSkillTypeConst = m_WayOfSwordConstTable.m_fSkillTypeConst_DestCommand;
		}
		break;
	case WST_DESTRUCTION_ACTIVE:
		{
			bVigor = false;
			fSkillTypeConst = m_WayOfSwordConstTable.m_fSkillTypeConst_DestActive;
		}
		break;
	case WST_DESTRUCTION_SPECIAL:
		{
			bVigor = false;
			fSkillTypeConst = m_WayOfSwordConstTable.m_fSkillTypeConst_DestSpecialActive;
		}
		break;
	default:
		fSkillTypeConst = 0.f;
	}

	float fStateConst = 0.f;
	switch(iWayOfSwordState)
	{
	case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
		{
			if( bVigor == true )
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_VigorSkill_InDestState;
			else
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_DestSkill_InDestState;
		}
		break;
	case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
		{
			if( bVigor == true )
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_VigorSkill_TowrdDestState;
			else
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_DestSkill_TowrdDestState;
		}
		break;
	case 0: // CX2GUElsword_SwordMan::WSS_CENTER
		{
			if( bVigor == true )
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_VigorSkill_CenterState;
			else
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_DestSkill_CenterState;
		}
		break;
	case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
		{
			if( bVigor == true )
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_VigorSkill_TowardVigorState;
			else
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_DestSkill_TowardVigorState;
		}
		break;
	case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
		{
			if( bVigor == true )
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_VigorSkill_InVigorState;
			else
				fStateConst = m_WayOfSwordConstTable.m_fStateConst_DestSkill_InVigorState;
		}
		break;
	default:
		fStateConst = 0.f;
	}
#ifdef BALANCE_PATCH_20120329
	float fGameModeConst = 1.f;
#else
	float fGameModeConst = 0.f;
	if(g_pX2Game != NULL)
	{
		if(g_pX2Game->GetGameType() == CX2Game::GT_PVP)
			fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_PVP_Hit;
		else
			fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_NPC_Hit;
	}
#endif BALANCE_PATCH_20120329

	// WSP ����
	fWSPGain = fDamageRate_ * fSkillTypeConst * fStateConst * fGameModeConst;

	return fWSPGain;
}


float CX2DamageManager::CalcWayOfSwordPointOnDamageReact( float fDamageRate_, int iWayOfSwordState )
{
	float fWSPGain = 0.f;

	float fSkillTypeConst	= m_WayOfSwordConstTable.m_fSkillTypeConst_DamageReact;

	float fStateConst = 0.f;
	switch(iWayOfSwordState)
	{
	case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
		{
			fStateConst = m_WayOfSwordConstTable.m_fStateConst_DamageReact_InDestState;
		}
		break;
	case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
		{
			fStateConst = m_WayOfSwordConstTable.m_fStateConst_DamageReact_TowrdDestState;
		}
		break;
	case 0: // CX2GUElsword_SwordMan::WSS_CENTER
		{
			fStateConst = m_WayOfSwordConstTable.m_fStateConst_DamageReact_CenterState;
		}
		break;
	case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
		{
			fStateConst = m_WayOfSwordConstTable.m_fStateConst_DamageReact_TowardVigorState;
		}
		break;
	case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
		{ // ���� ���Ǳ��� ��ǳ ���¿��� �Ҹ��Ű�� ����.
			fStateConst = m_WayOfSwordConstTable.m_fStateConst_DamageReact_InVigorState;
		}
		break;
	default:
		fStateConst = 0.f;
	}
#ifdef BALANCE_PATCH_20120329
	float fGameModeConst = 1.f;
#else
	float fGameModeConst = 0.f;
	if(g_pX2Game != NULL)
	{
		if(g_pX2Game->GetGameType() == CX2Game::GT_PVP)
			fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_PVP_Hit;
		else
			fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_NPC_Hit;
	}
#endif BALANCE_PATCH_20120329

	// WSP ���� On DamageReact
	fWSPGain = fSkillTypeConst * fStateConst * fGameModeConst;
	return fWSPGain;
}

float CX2DamageManager::CalcVigorMpUpOnAttack_Elesis( WAY_OF_SWORD_TYPE wayOfSwordType_, int iWayOfSwordState )
{
	if( NULL == g_pX2Game )
		return 0.f;

	float fVigorMpUp = 1.f;

	if( iWayOfSwordState != 2) // CX2GUUser::WSS_VIGOR
		return fVigorMpUp;

	switch(wayOfSwordType_)
	{
	case WST_VIGOR_NORMAL:
		{
			if( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
				fVigorMpUp = 1.25f;
			else
				fVigorMpUp = 1.5f;
		} break;
	default:
		{
			fVigorMpUp = 1.f;
		} break;
	}
	return fVigorMpUp;
}

float CX2DamageManager::CalcVigorMpUpOnAttack( WAY_OF_SWORD_TYPE wayOfSwordType_, int iWayOfSwordState )
{
	float fVigorMpUp = 1.f;

	if( iWayOfSwordState != 2) // CX2GUElsword_SwordMan::WSS_VIGOR
		return fVigorMpUp;

	switch(wayOfSwordType_)
	{
	case WST_VIGOR_NORMAL:
		fVigorMpUp = 3.f;
		break;
	case WST_VIGOR_ACTIVE:
		fVigorMpUp = 4.f;
		break;
	case WST_VIGOR_SPECIAL:
		fVigorMpUp = 4.f;
		break;
	default:
		fVigorMpUp = 1.f;
		break;
	}
	return fVigorMpUp;
}

float CX2DamageManager::CalcVigorMpUpOnDamageReact( int iWayOfSwordState )
{
	float fVigorMpUp = 1.f;

	if( iWayOfSwordState == 2) // CX2GUElsword_SwordMan::WSS_VIGOR
		fVigorMpUp = 2.f;

	return fVigorMpUp;
}

float CX2DamageManager::GetDestRageWSPGain()
{
	float fGameModeConst = 1.f;
	if(g_pX2Game->GetGameType() == CX2Game::GT_PVP)
		fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_PVP_Hit;
	else
		fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_NPC_Hit;

	return m_WayOfSwordConstTable.m_fConst_DestRageWSPGain * fGameModeConst;
}
float CX2DamageManager::GetDestRageWSPGain_Elesis()
{
	float fGameModeConst = 1.f;
	if(g_pX2Game->GetGameType() == CX2Game::GT_PVP)
		fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_PVP_Hit;
	else
		fGameModeConst = m_WayOfSwordConstTable.m_fGameModeConst_NPC_Hit;

	return m_WayOfSwordConstTable.m_fConst_DestRageWSPGain_Elesis * fGameModeConst;
}
bool CX2DamageManager::OpenWayOfSwordScript( const WCHAR* wszFileName )
{
	if ( NULL == wszFileName || NULL == wszFileName[0] )
		return false;

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( false == g_pKTDXApp->LoadLuaTinker( wszFileName ) )
		return false;

	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, wszFileName ) )
		return false;
	
	if( true == luaManager.BeginTable( "WAY_OF_SWORD" ) )
	{
		LUA_GET_VALUE( luaManager, "SkillTypeConst_VigorCommand",			m_WayOfSwordConstTable.m_fSkillTypeConst_VigorCommand,			-96.f	);
		LUA_GET_VALUE( luaManager, "SkillTypeConst_VigorActive",			m_WayOfSwordConstTable.m_fSkillTypeConst_VigorActive,			-160.f	);
		LUA_GET_VALUE( luaManager, "SkillTypeConst_VigorSpecialActive",		m_WayOfSwordConstTable.m_fSkillTypeConst_VigorSpecialActive,	-72.f	);

		LUA_GET_VALUE( luaManager, "SkillTypeConst_DestCommand",			m_WayOfSwordConstTable.m_fSkillTypeConst_DestCommand,			96.f	);
		LUA_GET_VALUE( luaManager, "SkillTypeConst_DestActive",				m_WayOfSwordConstTable.m_fSkillTypeConst_DestActive,			160.f	);
		LUA_GET_VALUE( luaManager, "SkillTypeConst_DestSpecialActive",		m_WayOfSwordConstTable.m_fSkillTypeConst_DestSpecialActive,		72.f	);

		LUA_GET_VALUE( luaManager, "SkillTypeConst_DamageReact",			m_WayOfSwordConstTable.m_fSkillTypeConst_DamageReact,			10.f	);

		LUA_GET_VALUE( luaManager, "StateConst_DestSkill_TowardVigorState",		m_WayOfSwordConstTable.m_fStateConst_DestSkill_TowardVigorState,	1.f		);
		LUA_GET_VALUE( luaManager, "StateConst_VigorSkill_TowardVigorState",	m_WayOfSwordConstTable.m_fStateConst_VigorSkill_TowardVigorState,	0.1f	);

		LUA_GET_VALUE( luaManager, "StateConst_DestSkill_TowardDestState",		m_WayOfSwordConstTable.m_fStateConst_DestSkill_TowrdDestState,		0.1f	);
		LUA_GET_VALUE( luaManager, "StateConst_VigorSkill_TowardDestState",		m_WayOfSwordConstTable.m_fStateConst_VigorSkill_TowrdDestState,		1.f		);

		LUA_GET_VALUE( luaManager, "StateConst_DestSkill_InVigorState",			m_WayOfSwordConstTable.m_fStateConst_DestSkill_InVigorState,		0.f		);
		LUA_GET_VALUE( luaManager, "StateConst_VigorSkill_InVigorState",		m_WayOfSwordConstTable.m_fStateConst_VigorSkill_InVigorState,		1.5f	);

		LUA_GET_VALUE( luaManager, "StateConst_DestSkill_InDestState",			m_WayOfSwordConstTable.m_fStateConst_DestSkill_InDestState,			1.5f	);
		LUA_GET_VALUE( luaManager, "StateConst_VigorSkill_InDestState",			m_WayOfSwordConstTable.m_fStateConst_VigorSkill_InDestState,		0.f		);

		LUA_GET_VALUE( luaManager, "StateConst_DestSkill_CenterState",			m_WayOfSwordConstTable.m_fStateConst_DestSkill_CenterState,			1.f		);
		LUA_GET_VALUE( luaManager, "StateConst_VigorSkill_CenterState",			m_WayOfSwordConstTable.m_fStateConst_VigorSkill_CenterState,		1.f		);


		LUA_GET_VALUE( luaManager, "StateConst_DamageReact_TowardVigorState",	m_WayOfSwordConstTable.m_fStateConst_DamageReact_TowardVigorState,		0.33f	);
		LUA_GET_VALUE( luaManager, "StateConst_DamageReact_TowrdDestState",		m_WayOfSwordConstTable.m_fStateConst_DamageReact_TowrdDestState,		-0.33f	);
		LUA_GET_VALUE( luaManager, "StateConst_DamageReact_InVigorState",		m_WayOfSwordConstTable.m_fStateConst_DamageReact_InVigorState,			0.f	);
		LUA_GET_VALUE( luaManager, "StateConst_DamageReact_InDestState",		m_WayOfSwordConstTable.m_fStateConst_DamageReact_InDestState,			0.f	);
		LUA_GET_VALUE( luaManager, "StateConst_DamageReact_CenterState",		m_WayOfSwordConstTable.m_fStateConst_DamageReact_CenterState,			0.f		);


		LUA_GET_VALUE( luaManager, "GameModeConst_NPC_Hit",				m_WayOfSwordConstTable.m_fGameModeConst_NPC_Hit,				1.f		);
		LUA_GET_VALUE( luaManager, "GameModeConst_PVP_Hit",				m_WayOfSwordConstTable.m_fGameModeConst_PVP_Hit,				1.5f		);

		LUA_GET_VALUE( luaManager, "Const_VigorStateThreshold",			m_WayOfSwordConstTable.m_fConst_VigorStateThreshold,			4800.f	);
		LUA_GET_VALUE( luaManager, "Const_DestStateThreshold",			m_WayOfSwordConstTable.m_fConst_DestStateThreshold,				-4800.f	);

		LUA_GET_VALUE( luaManager, "Const_DestRageWSPGain",				m_WayOfSwordConstTable.m_fConst_DestRageWSPGain,				1500.f	);
		LUA_GET_VALUE( luaManager, "Const_DestRageWSPGain_Elesis",		m_WayOfSwordConstTable.m_fConst_DestRageWSPGain_Elesis,			2000.f	);
		luaManager.EndTable();
	}
	else
	{
		return false;
	}

	return true;

}
#endif ELSWORD_WAY_OF_SWORD


bool CX2DamageManager::CollisionDataCheckFromUnitToUnit( CX2GameUnit* pAttackerGameUnit_, 
	const CKTDXCollision::CollisionDataListSet& setDefenderCollisionDataList_,
	CKTDXCollision::CollisionType& collisionType_, D3DXVECTOR3* pImpactPos_ )
{
	if( m_pCollision->CollisionDataCheck( pAttackerGameUnit_->GetAttackListSet(), setDefenderCollisionDataList_, collisionType_, pImpactPos_ ) )
		return true;
	else if ( m_pCollision->CollisionDataCheck( pAttackerGameUnit_->GetSubAttackListSet(), setDefenderCollisionDataList_, collisionType_, pImpactPos_ ) )
		return true;
	else
	{
		const UINT uiSize = pAttackerGameUnit_->GetSizeOfPairSubAttackListSet();
		for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
		{
			const CKTDXCollision::CollisionDataListSet* pSetSubAttackList = NULL;

			if ( pAttackerGameUnit_->GetSubAttackListGetFromPair( uiIndex, &pSetSubAttackList ) 
				 && m_pCollision->CollisionDataCheck( *pSetSubAttackList, setDefenderCollisionDataList_, collisionType_, pImpactPos_ ) )
				return true;
		}			
	}
	
	return false;
}

//bool CX2DamageManager::CollisionDataCheckFromUnitToEffect( CX2GameUnit* pAttackerGameUnit_,
//	const CKTDXCollision::CollisionDataList& listDefenderCollisionData_,
//	CKTDXCollision::CollisionType& collisionType_, D3DXVECTOR3* pImpactPos_ )
//{
//	if( m_pCollision->CollisionDataCheck( pAttackerGameUnit_->GetAttackListSet(), listDefenderCollisionData_, collisionType_, pImpactPos_ ) )
//		return true;
//	else if ( m_pCollision->CollisionDataCheck( pAttackerGameUnit_->GetSubAttackListSet(), listDefenderCollisionData_, collisionType_, pImpactPos_ ) )
//		return true;
//	else
//	{
//		const UINT uiSize = pAttackerGameUnit_->GetSizeOfPairSubAttackListSet();
//		for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
//		{
//			const CKTDXCollision::CollisionDataListSet* pSetSubAttackList = NULL;
//
//			if ( pAttackerGameUnit_->GetSubAttackListGetFromPair( uiIndex, &pSetSubAttackList ) 
//				 && m_pCollision->CollisionDataCheck( *pSetSubAttackList, listDefenderCollisionData_, collisionType_, pImpactPos_ ) )
//				return true;
//		}			
//	}
//
//	return false;
//}

/** @function : ParsingBuffFactor
	@brief : DamageData���� BUFF_FACTOR ���̺� �Ľ�
	@param : ��ƸŴ���, ������������
*/
void CX2DamageManager::ParsingBuffFactor( KLuaManager& luaManager_, DamageData* pDamageData_ )
{
	if ( luaManager_.BeginTable( "BUFF_FACTOR" ) )
	{
		CX2BuffFactorPtr ptrBuffFactor = CX2BuffFactor::CreateBuffFactorPtr();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( ptrBuffFactor->ParsingBuffFactor( luaManager_, false ) )
#else //UPGRADE_SKILL_SYSTEM_2013
		if ( ptrBuffFactor->ParsingBuffFactor( luaManager_ ) )
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ptrBuffFactor->SetGameUnitBuffFactor( pDamageData_->optrAttackerGameUnit.GetObservable() );
			pDamageData_->m_vecBuffFactorPtr.push_back( ptrBuffFactor );
		}

		luaManager_.EndTable();
	}
}

/** @function : PushSocketAndTemporaryBuffFactor
	@brief : ���� �ɼǿ� ���� �ٸ� �������� �� ����/������� ����, �ڽſ��� �ɸ� ������ ���� �ٸ� ������� �� ����/������� �ִ� ���(�ҵ����̾�) ���
	@param : ������ ����Ʈ
*/
/*static*/ void CX2DamageManager::PushSocketAndTemporaryBuffFactor( DamageData* pDamageData_ )
{
	/// ����Ÿ���� NORMAL�� ���
	if ( AT_NORMAL == pDamageData_->attackType )
	{
		if ( null != pDamageData_->optrAttackerGameUnit )
		{
			CX2GameUnit* pAttackerGameUnit = pDamageData_->optrAttackerGameUnit.GetObservable();

			/// �������� �ִ� ������ ���� ����
			if ( RT_DUMMY_DAMAGE != pDamageData_->reActType && RT_NO_DAMAGE != pDamageData_->reActType )
			{
				/// ���� �ɼǵ��� ���ؼ� �����
				const vector<CX2BuffFactorPtr>& vecAttackerGameUnitBuffFactorPtr = pAttackerGameUnit->GetBuffFactorList();
				BOOST_FOREACH( CX2BuffFactorPtr ptrBuffFactor, vecAttackerGameUnitBuffFactorPtr )
				{
					if ( NULL != ptrBuffFactor )
					{
						ptrBuffFactor->SetGameUnitBuffFactor( pAttackerGameUnit );
						pDamageData_->m_vecBuffFactorPtr.push_back( ptrBuffFactor );
					}
				}
			}

			/// ������ ���� �Ͻ������� ����� ����
			const vector<CX2BuffFactorPtr>& vecAttackerGameUnitTemporaryBuffFactorPtr = pAttackerGameUnit->GetTemporaryBuffFactorList();
			BOOST_FOREACH( CX2BuffFactorPtr ptrBuffFactor, vecAttackerGameUnitTemporaryBuffFactorPtr )
			{
				if ( NULL != ptrBuffFactor )
				{
					ptrBuffFactor->SetGameUnitBuffFactor( pAttackerGameUnit );
					pDamageData_->m_vecBuffFactorPtr.push_back( ptrBuffFactor );
				}
			}
		}		
	}
}

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE

/** @function : GetBehaviorFactor
	@brief : �׷� ������ Ȱ���� ���� �˻�
	@param : �˻��� ������ ����Ʈ �̸�(wstrDamageEffectName_), �˻��� ������ ����(pDamageData_)
	@return : �׷� ������ �������̶�� true, �ƴ϶�� false
*/
bool CX2DamageManager::CheckActiveGroupDamage( const wstring& wstrDamageEffectName_, CX2DamageManager::DamageData* pDamageData_ )
{
	if( NULL == pDamageData_ )
		return true;

	/// �ǰ����� �׷� ������ ���� �����̳�
	const vector<CX2GameUnit::DamageEffectGroupDataPtr>& vecDamageEffectGroupData	= pDamageData_->optrDefenderGameUnit->GetDamageEffectGroupData();
	/// ������ ���� Ÿ��
	CX2GameUnit::GAME_UNIT_TYPE					  eAttackerGameUnitType		= pDamageData_->optrAttackerGameUnit->GetGameUnitType();
	/// ������ ���� ���̵�
	UidType										  uidAttackerUID			= pDamageData_->optrAttackerGameUnit->GetUnitUID();

	BOOST_FOREACH( const CX2GameUnit::DamageEffectGroupDataPtr& pDamageEffectGroupData, vecDamageEffectGroupData )
	{
		if( NULL					!= pDamageEffectGroupData &&
			uidAttackerUID			== pDamageEffectGroupData->m_uidAttackerUID &&			/// �������� ���� ���̵� ������
			eAttackerGameUnitType	== pDamageEffectGroupData->m_eAttackerGameUnitType &&	/// �������� ���� Ÿ�Ե� ����
			wstrDamageEffectName_	== pDamageEffectGroupData->m_wstrDamageEffectName &&	/// �׷� ������ �������� ������ ����Ʈ �̸��� ����
			0.f						<  pDamageEffectGroupData->m_fHitGFap )					/// �ǰ� ������ ���� �����ִٸ�, �׷� �������� �������̴�
		{
			return true;	/// ���� �������� �׷� �������� �־, ����ġ üũ �н�
		}
	}

	return false;			/// ���� �������� �׷� �������� ���ٸ�, ������ ó�� ����
}

/** @function : GetBehaviorFactor
	@brief : �׷� ������ ����
	@param : �˻��� ������ ����Ʈ �̸�(wstrDamageEffectName_), �˻��� ������ ����(pDamageData_)
*/
void CX2DamageManager::SetGroupDamage( const wstring& wstrDamageEffectName_, CX2DamageManager::DamageData* pDamageData_ )
{
	if( NULL == pDamageData_ )
		return;

	/// �׷� ������ ���� �����̳�
	vector<CX2GameUnit::DamageEffectGroupDataPtr>&	vecDamageEffectGroupData	= pDamageData_->optrDefenderGameUnit->AccessDamageEffectGroupData();
	/// ������ ���� Ÿ��
	CX2GameUnit::GAME_UNIT_TYPE						eAttackerGameUnitType		= pDamageData_->optrAttackerGameUnit->GetGameUnitType();
	/// ������ ���� ���̵�
	UidType											uideAttackerUID				= pDamageData_->optrAttackerGameUnit->GetUnitUID();
	/// ������ ����Ʈ �ǰ� ����
	float											fHitGap						= pDamageData_->fHitGap;

	/// �׷� ������ ���� �����̳ʿ� ���� ���� �߰�
	CX2GameUnit::DamageEffectGroupDataPtr pDamageEffectGroupDataPtr =  
			CX2GameUnit::DamageEffectGroupDataPtr( new CX2GameUnit::DamageEffectGroupData( wstrDamageEffectName_, uideAttackerUID, eAttackerGameUnitType, fHitGap ) );

	vecDamageEffectGroupData.push_back( pDamageEffectGroupDataPtr );		/// ���� ����
}

#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER

/** @function : GetPossibleAttackOurTeamID
	@brief : ���� ���� ���� �� �ִ� NPC���� �˻� ( ���� NPC ���� üũ�� - �ʵ忡�� ��ȯ�Ǵ� ���ʹ� AllyTeam�� AllyUnitUid�� ���� �־��༭, ������ üũ �н��Ǵ� ���� ���� )
	@param : ������ NPC
	@return	: ���� ���� ����
*/
bool CX2DamageManager::GetPossibleAttackOurTeamID( CX2GameUnit* pNPCUnit_ )
{
	if( NULL != pNPCUnit_ && CX2GameUnit::GUT_NPC == pNPCUnit_->GetGameUnitType() )
	{
		CX2GUNPC* pNPC = static_cast<CX2GUNPC*>( pNPCUnit_ );

		if( NULL != pNPC )
		{
			switch( pNPC->GetNPCTemplet().m_nNPCUnitID )
			{
			case CX2UnitManager::NUI_EVENT_TEACHER_HAGERS:
				{
					return true;
				} break;
			}
		}
	}

	return false;
}

#endif ADD_VELDER_ACADEMY_EVENT_MONSTER

#ifdef DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL
/** @function : SetBuffFactorToDamageDataByBuffFactorID
	@brief : ID���� �̿��� �������� ��ü�� ����� DamageData�� �ֱ�
*/
bool CX2DamageManager::SetBuffFactorToDamageDataByBuffFactorID( OUT DamageData* pDamageData_, IN const UINT uiBuffFactorId_, IN const UINT uiLevel_ /*= 1*/ )
{
	CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( uiBuffFactorId_ );
	if ( NULL != ptrBuffFactor )
	{
		CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr( uiLevel_ );
		if ( NULL != ptrBuffFactorClone )
		{
			pDamageData_->PushBuffFactor( ptrBuffFactorClone, false );
			return true;
		}
	}
	return false;
}
/** @function : ParsingBuffFactorID
	@brief : DamageData�� ID�� ���ǵ� �������� �Ľ�
*/
#ifdef ADD_MEMO_1ST_CLASS
void CX2DamageManager::ParsingBuffFactorID( KLuaManager& luaManager_, DamageData* pDamageData_, const bool IsEqippedMemo_ /*= false*/ )
#else //ADD_MEMO_1ST_CLASS
void CX2DamageManager::ParsingBuffFactorID( KLuaManager& luaManager_, DamageData* pDamageData_ )
#endif //ADD_MEMO_1ST_CLASS
{
	// DamageData�� �߰� �� BuffFactorID �Ľ�
	vector<UINT> vecUiBuffFactor;

#ifdef BALANCE_PATCH_20131107

#ifdef ADD_MEMO_1ST_CLASS
	bool bMemoCheck = IsEqippedMemo_;
#else //ADD_MEMO_1ST_CLASS
	bool bMemoCheck = false;
#endif //ADD_MEMO_1ST_CLASS

	// ���� �� ��� �������� ���� �޸� üũ
	if( null != pDamageData_->optrAttackerGameUnit &&
		CX2GameUnit::GUT_USER == pDamageData_->optrAttackerGameUnit->GetGameUnitType() 
#ifdef ADD_MEMO_1ST_CLASS
		&& bMemoCheck == false
#endif //ADD_MEMO_1ST_CLASS
		)
	{
		const CX2GUUser* pGUUser = static_cast<CX2GUUser*>(pDamageData_->optrAttackerGameUnit.GetObservable());
		if( NULL != pGUUser )
		{
			CX2SkillTree::SKILL_MEMO_ID eMemoID = CX2SkillTree::SMI_NONE;
			LUA_GET_VALUE_ENUM( luaManager_, "BUFF_FACTOR_RELATIVE_MEMO_ID", eMemoID,	CX2SkillTree::SKILL_MEMO_ID,	CX2SkillTree::SMI_NONE );

			if( CX2SkillTree::SMI_NONE != eMemoID )
			{
				if( NULL != pGUUser->GetUnit() )
				{
					// �޸� ���� ����  
					bMemoCheck =  pGUUser->GetUnit()->GetUnitData().m_UserSkillTree.GetEqipSkillMemo( eMemoID );
				}
			}
		}
	}

	if( true == bMemoCheck )
	{
		if( luaManager_.BeginTable( "BUFF_FACTOR_ID_MEMO" ) == true )
		{
			int index = 1;
			UINT uiBuffFactorID = 0;
			while( true == luaManager_.GetValue( index, uiBuffFactorID ) )
			{
				if( 0 != uiBuffFactorID )
				{
					vecUiBuffFactor.push_back( uiBuffFactorID );
				}
				index++;
			}
			luaManager_.EndTable();
		}
	}
	else
#endif //BALANCE_PATCH_20131107
	{
		if( luaManager_.BeginTable( "BUFF_FACTOR_ID" ) == true )
		{
			int index = 1;
			UINT uiBuffFactorID = 0;
			while( true == luaManager_.GetValue( index, uiBuffFactorID ) )
			{
				if( 0 != uiBuffFactorID )
				{
					vecUiBuffFactor.push_back( uiBuffFactorID );
				}
				index++;
			}
			luaManager_.EndTable();
		}
	}


	if( false == vecUiBuffFactor.empty() )
	{
		// ���� ���� ���� ���� �� ���
		UINT iBuffFactorLV = 1;

		// ���� �� ��� �������� ���� ��ų üũ
		if( null != pDamageData_->optrAttackerGameUnit &&
			CX2GameUnit::GUT_USER == pDamageData_->optrAttackerGameUnit->GetGameUnitType() )
		{
			const CX2GUUser* pGUUser = static_cast<CX2GUUser*>(pDamageData_->optrAttackerGameUnit.GetObservable());
			if( NULL != pGUUser )
			{
				// BuffFactor ���� ���� ��ų ID �Ľ�
				CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE;
				LUA_GET_VALUE_ENUM( luaManager_, "BUFF_FACTOR_RELATIVE_SKILL_ID", eSkillID,	CX2SkillTree::SKILL_ID,	pGUUser->GetNowStateSkillID() );
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
				//������ �ش� skill�� ��������� üũ�ϴ� �����Դϴ�.
				//������ �ش� skill�� ����� �ʾҴٸ� buff�� 1�����̶� �������� �ʽ��ϴ�.
				bool CheckSkillLevel = false;
				LUA_GET_VALUE( luaManager_, "RELATIVE_SKILL_LEARN_CHECK", CheckSkillLevel,	false );
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

				if( CX2SkillTree::SI_NONE != eSkillID )
				{
					if( NULL != pGUUser->GetUnit() )
					{
						// ��ųƮ���� ���� üũ.( ��ų ���� ���� ) 
						const UINT iSkillLevel = static_cast<UINT>( pGUUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( eSkillID ) );

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
						//CheckSkillLevel ���� false�̰ų� ��ų�� ����ٸ� iBuffFactorLV�� ���� ��ų������ �����ŵ�ϴ�.
						//�ƴ϶�� iBuffFactorLV�� 0���� ����� ������ �����Ű�� �ʽ��ϴ�.
						if( false == CheckSkillLevel || 1 <= iSkillLevel )
							iBuffFactorLV = 0 < iSkillLevel ? iSkillLevel : 1;
						else
							iBuffFactorLV = 0;
#else //SERV_ELESIS_SECOND_CLASS_CHANGE
						iBuffFactorLV = 0 < iSkillLevel ? iSkillLevel : 1;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
					}
				}
			}
		}



#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// BUFF_FACTOR_RELATIVE_SKILL_ID �� true �� ���, Master Unit �� UID �� �޾� �ش� ��ų�� ������� �ϰ�
		// ����ٸ�, �ش� ���� ������ ������ ���� �ɴϴ�. Ex. �̺�:�ڵ� �׸޽ý�, ����� �ǵ�
		else if( null != pDamageData_->optrAttackerGameUnit &&
			CX2GameUnit::GUT_NPC == pDamageData_->optrAttackerGameUnit->GetGameUnitType() )
		{
			bool bBuffFactorRelativeMasterUnit = false;
			CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE;
			LUA_GET_VALUE_ENUM( luaManager_, "BUFF_FACTOR_RELATIVE_SKILL_ID", eSkillID,	CX2SkillTree::SKILL_ID,	CX2SkillTree::SI_NONE );
			LUA_GET_VALUE( luaManager_, "BUFF_FACTOR_RELATIVE_MASTER_UNIT", bBuffFactorRelativeMasterUnit, false );
			

			if ( true == bBuffFactorRelativeMasterUnit && eSkillID != CX2SkillTree::SI_NONE )
			{	// ��ų ID �� �����Ǿ� �ְ�, NPC Unit �� AllyUnitUID �� ������ ��
				// ������ ������ ã�Ƽ� �ش� ���� ������ ������ �޾ƿ´�.
				CX2GUNPC* pGUNPC = static_cast<CX2GUNPC*>(pDamageData_->optrAttackerGameUnit.GetObservable());
				if ( NULL != pGUNPC )
				{	
					CX2AllyNPCAI* pAllyAI = static_cast<CX2AllyNPCAI*> ( pGUNPC->GetNPCAI() );
			
					if( NULL != pAllyAI && pAllyAI->GetAllyUnitUID() > -1 )
					{
						CX2GUUser * pMasterUser = g_pX2Game->GetUserUnitByUID( pAllyAI->GetAllyUnitUID() );
			
						if ( NULL != pMasterUser && NULL != pMasterUser->GetUnit() )
						{
							const UINT iSkillLevel = static_cast<UINT>( pMasterUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( eSkillID ) );
							iBuffFactorLV = 0 < iSkillLevel ? iSkillLevel : 0;
						}
					}
				}
			}
		}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
		if( iBuffFactorLV > 0 )
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		BOOST_FOREACH( UINT uiBuffFactorID, vecUiBuffFactor )
		{
			SetBuffFactorToDamageDataByBuffFactorID( pDamageData_, uiBuffFactorID, iBuffFactorLV );
		}
	}


}
#endif // DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
bool CX2DamageManager::GetIsPossibleProvokeExtraDamage ( DamageData * pDamageData )
{	// Extra ������ �� �����ý��� ���߿� �����ϴ� ������ ���� �ƴ����� ó���ϴ� �Լ�
	if ( NULL != pDamageData )
	{
		switch( pDamageData->m_ExtraDamage.m_ExtraDamageType )
		{
			case EDT_MANA_DAMAGE:
			case EDT_DAMAGE_RATE:
			case EDT_WATER_HOLD:
			case EDT_ATTACK_ALL_TEAM:
				return false;
		}
	}
	return true;
}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

//{{ robobeg : 2013-09-17
bool    CX2DamageManager::DamageData::IsAttackedByMyUnit()
{
    CX2GameUnit*    pAttackerUnit = ( null != optrAttackerGameUnit )
        ? optrAttackerGameUnit.GetObservable() : NULL;

    if ( pAttackerUnit != NULL )
    {
        return  pAttackerUnit->IsMyUnit();
    }
    else
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect*   pAttackerEffect = g_pX2Game->GetDamageEffect()->GetInstance( hAttackerEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( pAttackerEffect != NULL && pAttackerEffect->GetOwnerUnit() != NULL )
        {
            return  pAttackerEffect->GetOwnerUnit()->IsMyUnit();
        }
    }
    return false;
}
//}} robobeg : 2013-09-17

