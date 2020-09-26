/**@file : X2BuffBehaviorTemplet.cpp
   @breif : ������ �ൿ�� �����ϴ� Templet ����
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffBehaviorTemplet.h"

#define CASE_BEHAVIOR_TEMPLET( class_, type_ ) \
case type_: \
	{ \
		CX2BuffBehaviorTempletPtr ptrBehaviorTemplet = class_::CreateBuffBehaviorTempletPtr(); \
		if ( NULL != ptrBehaviorTemplet && ptrBehaviorTemplet->ParsingBehaviorTemplateMethod( luaManager_, STRINGIZEA2(type_) ) ) \
		{ \
			ptrBehaviorTemplet->SetType( type_ ); \
			vecBuffBehaviorTempletPtr_.push_back( ptrBehaviorTemplet ); \
		} \
		else \
			return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

/** @function : ParsingCombinationBehavior
	@brief : ������ �ൿ�� ������� �����Ǿ� �ִ��� �˾Ƴ���, ������ Ÿ�Ժ��� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� BehaviorTempletPtr�� ���� vector(vecBuffBehaviorTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*static*/ bool CX2BuffBehaviorTemplet::ParsingCombinationBehavior( KLuaManager& luaManager_, OUT vector<CX2BuffBehaviorTempletPtr>& vecBuffBehaviorTempletPtr_ )
{
	vector<UINT> vecBehaviorType;

#ifdef EXCEPTION_BUFF_FACTOR_VER2
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR_VER2

	/// Behavior�� � �ൿ��ҵ��� �������� �̷�� ������ ����
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecBehaviorType ) )
	{
		vector<UINT>::iterator vItr = vecBehaviorType.begin();
		while ( vecBehaviorType.end() != vItr )		/// ���տ� ���Ե� �ൿ��ҵ��� �Ľ�
		{
#ifdef EXCEPTION_BUFF_FACTOR_VER2
			if( static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BBT_REFLECT_MAGIC )
			{
				bExceptionCase = false;
			}
#endif // EXCEPTION_BUFF_FACTOR_VER2

			switch ( static_cast<BUFF_BEHAVIOR_TYPE>( *vItr ) )
			{
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeNowHpPerSecondBehaviorTemplet, BBT_CHANGE_NOW_HP_PER_SECOND )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeNowMpPerSecondBehaviorTemplet, BBT_CHANGE_NOW_MP_PER_SECOND )
			CASE_BEHAVIOR_TEMPLET( CX2BuffReverseLeftRightBehaviorTemplet, BBT_REVERSE_LEFT_RIGHT )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_DASH_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_ATTACK_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAX_HP )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_PHYSIC_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAGIC_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_PHYSIC_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAGIC_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ANIM_SPEED )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_WALK_RUN_SPEED )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_JUMP_POWER )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ACCURACY )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_AVOIDANCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_CRITICAL_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_CRITICAL_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ANTI_CRITICAL_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_CRITICAL_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ADDITIONAL_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ADDITIONAL_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffCreateBuffFactortBehaviorTemplet, BBT_CREATE_BUFF_FACTOR )
			CASE_BEHAVIOR_TEMPLET( CX2BuffAbsorbEffectAttackBehaviorTemplet, BBT_ABSORB_EFFECT_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHARGE_MP_HIT_MELEE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHARGE_MP_HIT_RANGE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHARGE_MP_HITTED )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeHyperModeBehaviorTemplet, BBT_CHANGE_HYPER_MODE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_SUPER_ARMOR )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffResetSkillCoolTimeBehaviorTemplet, BBT_RESET_SKILL_COOL_TIME )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeUnitScaleBehaviorTemplet, BBT_CHANGE_UNIT_SCALE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffCustomFunctionBehaviorTemplet, BBT_CUSTOM_FUNCTION )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_DRAIN_HP_NORMAL_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffEffectSetWithDamageBehaviorTemplet, BBT_EFFECT_SET_WITH_DAMAGE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffStunBehaviorTemplet, BBT_STUN )
			CASE_BEHAVIOR_TEMPLET( CX2BuffFinishOtherBuffDebuffBehaviorTemplet, BBT_FINISH_OTHER_BUFF_DEBUFF )
			CASE_BEHAVIOR_TEMPLET( CX2BuffFreezeBehaviorTemplet, BBT_FROZEN )
			CASE_BEHAVIOR_TEMPLET( CX2BuffCanPassUnitBehaviorTemplet, BBT_CAN_PASS_UNIT )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_ATTACK_ALL_TEAM )
			CASE_BEHAVIOR_TEMPLET( CX2BuffNeverMoveBehaviorTemplet, BBT_NEVER_MOVE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_MAX_MP )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeAttackByTypeBehaviorTemplet, BBT_CHANGE_ATTACK_BY_TYPE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ENCHANT_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffDoubleAttackBehaviorTemplet, BBT_DOUBLE_ATTACK )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeAttackSphereScaleBehaviorTemplet, BBT_CHANGE_ATTACK_SPHERE_SCALE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_DOWN_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffAddSkillLevelBehaviorTemplet, BBT_ADD_SKILL_LEVEL )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_SKILL_ATTACK_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_ZX_ATTACK_IMPOSSIBLE )
	#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			CASE_BEHAVIOR_TEMPLET( CX2BuffReflectMagicBehaviorTemplet, BBT_REFLECT_MAGIC )
	#endif // SERV_ARA_CHANGE_CLASS_SECOND
	#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeConsumeMpRate, BBT_CHANGE_CONSUME_MP_RATE )
	#endif //FIX_SKILL_BALANCE_AISHA_LENA
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// Z Ű ��� X Ű ��� �Ұ�, ���� Ÿ�� �߰�
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_Z_ATTACK_IMPOSSIBLE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffIdentityBehaviorTemplet, BBT_X_ATTACK_IMPOSSIBLE )
			
			// �� �Ӽ� �� ��æƮ Ȯ�� ����
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE )
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeEncahntAttackRateBehaviorTemplet, BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE )
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
			CASE_BEHAVIOR_TEMPLET( CX2BuffChangeStatBehaviorTemplet, BBT_CHANGE_HYPER_CHARGE_SPEED )
#endif //SERV_ADD_LUNATIC_PSYKER

			case BBT_EMPTY:	/// �ൿ�� �������� �ʰڴٰ� ��������� �ؾ���
				return true;
#ifdef EXCEPTION_BUFF_FACTOR_VER2
#ifndef SERV_ARA_CHANGE_CLASS_SECOND
			case BBT_REFLECT_MAGIC:
				++vItr;
				continue;
#endif SERV_ARA_CHANGE_CLASS_SECOND
#endif //EXCEPTION_BUFF_FACTOR_VER2
				break;

			default:
				return DISPLAY_ERROR( L"Unknown Behavior Templet Type" );	/// false ����
				break;
			}

			++vItr;
		}
	}

	if ( vecBuffBehaviorTempletPtr_.empty() 
#ifdef EXCEPTION_BUFF_FACTOR_VER2
		&& !bExceptionCase
#endif //EXCEPTION_BUFF_FACTOR_VER2
		)
	{
		return DISPLAY_ERROR( L"BEHAVIOR Templet is empty" );
	}
	else
		return true;
}

/** @function : ParsingBehaviorTemplateMethod
	@brief : �� �Ļ�Ŭ�������� ������ ParsingBehavior�� �Ȱ��� ���� ó�� ���̿��� ȣ���ϱ� ���� ���ø��޼ҵ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� BehaviorTempletPtr�� ���� vector(vecBuffBehaviorTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
bool CX2BuffBehaviorTemplet::ParsingBehaviorTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		return ParsingBehavior( luaManager_ );
	}
	else
		return DISPLAY_ERROR( pszTableName_ );	
}

/** @function : SetFactorFromPacketTemplateMothod
	@brief : ��Ŷ���� ���� ���� ���� ������ �� �ൿ�� SetFactorFromPacket�� ���� ��Ű�� ���ø� �޼ҵ�
	@param : ������ ���� ���� ��������(kBuffFactor_), ������ ��������(pGameUnit_)
	@return : ����(true), ����(false)
*/
bool CX2BuffBehaviorTemplet::SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ )
{
	BOOST_FOREACH( const KBuffBehaviorFactor& kFactor_, kBuffFactor_.m_vecBuffBehaviorFactors )
	{
		if ( GetType() == kFactor_.m_uiType )
		{
			SetFactorFromPacket( kBuffFactor_, kFactor_, pGameUnit_ );
			return true;
		}	
	}

	return DISPLAY_ERROR( L"SetFactorFromPacket Error" );
}

/** @function : ParsingBehavior
	@brief :�ʴ� HP�� �����Ǵ� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeNowHpPerSecondBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	float fTimeInterval = 0.0f;	/// ���ʰ������� HP�� ����Ǵ°�
	LUA_GET_VALUE_RETURN( luaManager_, "TIME_INTERVAL", fTimeInterval, 0.0f, return DISPLAY_ERROR( L"TIME_INTERVAL" ) );
	m_CheckElapsedTimeForInterval.SetTargetTime( fTimeInterval );

	LUA_GET_VALUE_RETURN( luaManager_, "DRAW_TEXT", m_bDrawText, false, return DISPLAY_ERROR( L"DRAW_TEXT" ) );
	return true;
}

/** @function : OnFrameMove
	@brief : interval Ÿ�̸� Update, HP ����
	@param : HP�� ����Ǿ� �ϴ� ��������(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_CheckElapsedTimeForInterval.OnFrameMove( fElapsedTime_ );	/// Interval Ÿ�̸� ����
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove();	/// Interval Ÿ�̸� ����
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateChangeHpByTypeFunc( pGameUnit_ );		/// HP ����
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeNowHpPerSecondBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		const BUFF_CHANGE_TYPE eChangeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] );

		if ( eChangeType == m_eChangeType )
		{
			/// ����Ÿ��(CHANGE_TYPE)�� ���� ���� �Ǿ��� �Լ��� ����
			SelectDelegate();
			/// ������ �����ϴ� �͵� �ƴϰ� ������ ���� ������ �ִ� ��� ���װ��� ����

// 			const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 				0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

			const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

			// ���� ȿ���� ���밪
			const float fNewValueOrPercent = m_delegateGetValueOrPercentPerSecondFunc( pBehaviorFactor, fRegist );

			if ( DidStart() )	/// ��ø �Ǵ� ��ü���
			{
				/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
				const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
				if ( uiAccumulationCountBefore > 0 )	/// ��ø �̶��
				{
#ifdef ADJUST_BUFF_ACCMULATION_CALCULATE
					if( eChangeType == BCT_PERCENT )
					{
						//�ٲ� ������ ù ȿ���� 1��ø�� �� ���̶�� �����ϰ� �ۼ���.
						//���� uiAccumulationCountBefore�� 1�� ������.
						m_fResultValue = 1 + ( ( fNewValueOrPercent - 1.0f ) * ( uiAccumulationCountBefore + 1 ) );
						if( m_fResultValue < 0.0f )
						{
							//m_fREsultValue���� 0���� ���� �ʵ��� ������ ����ؼ� ����� �Ѵ�.(��ȹ��Ʈ���� ����)
							DISPLAY_ERROR( L"m_fResultValue error" );
							m_fResultValue = max( m_fResultValue, 0.0f );
						}
					}
					else
#endif //ADJUST_BUFF_ACCMULATION_CALCULATE
					{		
						// ��ø�Ǿ��� �� ������ ����
						const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
						// ���� ȿ���� ���밪
						const float fOldValueOrPercent = m_fResultValue / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
						/// ���� ȿ���� ���� ȿ�� �� ���밪 ��ġ�� �� ���� ���� ����
						m_fResultValue = ( abs(fNewValueOrPercent) > abs(fOldValueOrPercent) ? fNewValueOrPercent : fOldValueOrPercent );
						/// ��ø ����
						m_fResultValue *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// ������ ��øØ�� - 1�� ������� �ϴ� Before�� ����-1�� �����Ƿ� �׳� ���Ѵ�.
					}
				}
				else	/// ��ü ���
					m_fResultValue = ( abs(fNewValueOrPercent) > abs(m_fResultValue) ? fNewValueOrPercent : m_fResultValue );
			}
			else	/// ��ø, ��ü�� �ƴ϶��(ù���� �Ǵ� �ߺ�)
			{
				m_fResultValue = fNewValueOrPercent;
			}
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeNowHpPerSecondBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowHpPerSecondBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ʴ� Hp�� ���� ��Ű�� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeNowHpPerSecondBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fResultValue = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : �ʴ� ����Ǿ��ϴ� Ÿ�Կ� ���� �����ؾ��ϴ� �Լ��� ������
	@param : �ൿ����� ������(pBehaviorFactor_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::SelectDelegate()
{
	switch ( m_eChangeType )
	{
	case BCT_FIX_VALUE:			/// ���� ���� �����̸� �ش� ��������Ʈ ����
		{
			m_delegateChangeHpByTypeFunc = DelegateChangeHpByType::from_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByValue>( this );
			m_delegateGetValueOrPercentPerSecondFunc = DelegateGetValueOrPercentPerSecond::from_const_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetFixValuePerSecond>( this );
		} break;
	case BCT_RELATION_VALUE:	/// ���ݷ¿� ���� �����̸� �ش� ��������Ʈ ����
		{
			m_delegateChangeHpByTypeFunc = DelegateChangeHpByType::from_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByValue>( this );
			m_delegateGetValueOrPercentPerSecondFunc = DelegateGetValueOrPercentPerSecond::from_const_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetValuePerSecond>( this );
		} break;

	case BCT_PERCENT:	/// Percent�� ���� �����̸� �ش� ��������Ʈ ����
		{
			m_delegateChangeHpByTypeFunc = DelegateChangeHpByType::from_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByPercent>( this );
			m_delegateGetValueOrPercentPerSecondFunc = DelegateGetValueOrPercentPerSecond::from_const_method<CX2BuffChangeNowHpPerSecondBehaviorTemplet, &CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetPercentPerSecondByPercent>( this );
		} break;

	default:
		DISPLAY_ERROR( L"ChangeType Invalid" );
		break;
	}
}

/** @function : ChangeHpByValue
	@brief : ���� ���� HP ����
	@param : HP�� ����� ����(pGameUnit_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByValue( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
#ifdef FIX_RECOVERY_HP
		if( pGameUnit_->GetNowHp() <= 0 )
			return;
#endif // FIX_RECOVERY_HP

		pGameUnit_->UpNowHp( m_fResultValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, m_fResultValue );
	}
}

/** @function : ChangeHpByPercent
	@brief : %�� ���� HP ����
	@param : HP�� ����� ����(pGameUnit_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::ChangeHpByPercent( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
#ifdef FIX_RECOVERY_HP
		if( pGameUnit_->GetNowHp() <= 0 )
			return;
#endif // FIX_RECOVERY_HP

		const float fDeltaValue = pGameUnit_->GetMaxHp() * m_fResultValue;
		pGameUnit_->UpNowHp( fDeltaValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, fDeltaValue );
	}
}

/** @function : GetValuePerSecond
	@brief : ���� ���� ����� �ʴ� ���淮 ����
	@param : ���׷�(fRegist_)
	@return : ���淮(float)
*/
float CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetFixValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{	
	/// ��ġ * ����
	return (*pBehaviorFactor_)[FO_MULTIPLIER_OR_VALUE] * ( 1.0f - fRegist_ );
}

/** @function : GetValuePerSecond
	@brief : ���� ���� ����� �ʴ� ���淮 ����
	@param : ���׷�(fRegist_)
	@return : ���淮(float)
*/
float CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{	
	/// ��ġ * ���� * ����
	return (*pBehaviorFactor_)[FO_ATTACK_VALUE] * (*pBehaviorFactor_)[FO_MULTIPLIER_OR_VALUE] * ( 1.0f - fRegist_ );	/// ���ݷ� * ����
}

/** @function : GetPercentPerSecondByPercent
	@brief : ���� ���� ����� �ʴ� ���� % ����
	@param : ���׷�(fRegist_)
	@return : �����(float)
*/
float CX2BuffChangeNowHpPerSecondBehaviorTemplet::GetPercentPerSecondByPercent( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{
	/// ���� * ����
	return (*pBehaviorFactor_)[FO_PERCENT] * ( 1.0f - fRegist_ );
}

/** @function : DrawText
	@brief : HP�� ��ȭ���� ����ϴ� �Լ�
	@param : ��������(pGameUnit_), ��ȭ��(fDeltaValue_)
*/
void CX2BuffChangeNowHpPerSecondBehaviorTemplet::DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ )
{
	const UINT uiSize = 20;
	WCHAR wstrText[uiSize] = {0, };
	D3DXVECTOR3 numPos = pGameUnit_->GetHeadBonePos();
	numPos.y += -50.0f;

	if ( fDeltaValue_ > 0.0f )
	{
		StringCchPrintf( wstrText, uiSize, L"HP +%d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharRed() )
			g_pData->GetPicCharGreen()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}
	else
	{
		StringCchPrintf( wstrText, uiSize, L"HP %d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharRed() )
			g_pData->GetPicCharRed()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : �ʴ� MP ������ �̷������ ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeNowMpPerSecondBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	float fTimeInterval = 0.0f;	/// ���ʰ������� Mp�� ����Ǵ°�
	LUA_GET_VALUE_RETURN( luaManager_, "TIME_INTERVAL", fTimeInterval, 0.0f, return DISPLAY_ERROR( L"TIME_INTERVAL" ) );
	m_CheckElapsedTimeForInterval.SetTargetTime( fTimeInterval );

	LUA_GET_VALUE_RETURN( luaManager_, "DRAW_TEXT", m_bDrawText, false, return DISPLAY_ERROR( L"DRAW_TEXT" ) );	
	return true;
}

/** @function : OnFrameMove
	@brief : interval Ÿ�̸� Update, Mp ����
	@param : Mp�� ����Ǿ� �ϴ� ��������(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove( fElapsedTime_ );	/// Interval Ÿ�̸� ����
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove();	/// Interval Ÿ�̸� ����
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateChangeMpByTypeFunc( pGameUnit_ );		/// Mp ����
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeNowMpPerSecondBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		const BUFF_CHANGE_TYPE eChangeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] );

		if ( eChangeType == m_eChangeType )
		{
			/// ����Ÿ��(CHANGE_TYPE)�� ���� ���� �Ǿ��� �Լ��� ����
			SelectDelegate();
			/// ������ �����ϴ� �͵� �ƴϰ� ������ ���� ������ �ִ� ��� ���װ��� ����

// 			const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 				0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

			const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

			// ���� ȿ���� ���밪
			const float fNewValueOrPercent = GetValueOrPercentPerSecond( pBehaviorFactor, fRegist );

			if ( DidStart() )	/// ��ø �Ǵ� ��ü���
			{
				/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
				const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
				if ( uiAccumulationCountBefore > 0 )	/// ��ø �̶��
				{
#ifdef ADJUST_BUFF_ACCMULATION_CALCULATE
					if( eChangeType == BCT_PERCENT )
					{
						//�ٲ� ������ ù ȿ���� 1��ø�� �� ���̶�� �����ϰ� �ۼ���.
						//���� uiAccumulationCountBefore�� 1�� ������.
						m_fResultValue = 1 + ( ( fNewValueOrPercent - 1.0f ) * ( uiAccumulationCountBefore + 1 ) );

						if( m_fResultValue < 0.0f )
						{
							//m_fREsultValue���� 0���� ���� �ʵ��� ������ ����ؼ� ����� �Ѵ�.(��ȹ��Ʈ���� ����)
							DISPLAY_ERROR( L"m_fResultValue error" );
							m_fResultValue = max( m_fResultValue, 0.0f );
						}
					}
					else
#endif //ADJUST_BUFF_ACCMULATION_CALCULATE
					{
						// ��ø�Ǿ��� �� ������ ����
						const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
						// ���� ȿ���� ���밪
						const float fOldValueOrPercent = m_fResultValue / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
						/// ���� ȿ���� ���� ȿ�� �� ���밪 ��ġ�� �� ���� ���� ����
						m_fResultValue = ( abs(fNewValueOrPercent) > abs(fOldValueOrPercent) ? fNewValueOrPercent : fOldValueOrPercent );
						/// ��ø ����
						m_fResultValue *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// ������ ��øØ�� - 1�� ������� �ϴ� Before�� ����-1�� �����Ƿ� �׳� ���Ѵ�.
					}
				}
				else	/// ��ü ���
					m_fResultValue = ( abs(fNewValueOrPercent) > abs(m_fResultValue) ? fNewValueOrPercent : m_fResultValue );
			}
			else	/// ��ø, ��ü�� �ƴ϶��(ù���� �Ǵ� �ߺ�)
			{
				m_fResultValue = fNewValueOrPercent;
			}
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeNowMpPerSecondBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeNowMpPerSecondBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowMpPerSecondBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ʴ� MP�� ���� ��Ű�� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeNowMpPerSecondBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fResultValue = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : �ʴ� ����Ǿ��ϴ� Ÿ�Կ� ���� �����ؾ��ϴ� �Լ��� ������
	@param : �ൿ����� ������(pBehaviorFactor_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::SelectDelegate()
{
	switch ( m_eChangeType )
	{
	case BCT_FIX_VALUE:	/// ���ݷ¿� ���� �����̸� �ش� ��������Ʈ ����
		{
			m_delegateChangeMpByTypeFunc = DelegateChangeMpByType::from_method<CX2BuffChangeNowMpPerSecondBehaviorTemplet, &CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByValue>( this );
		} break;

	case BCT_PERCENT:	/// Percent�� ���� �����̸� �ش� ��������Ʈ ����
		{
			m_delegateChangeMpByTypeFunc = DelegateChangeMpByType::from_method<CX2BuffChangeNowMpPerSecondBehaviorTemplet, &CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByPercent>( this );
		} break;

	default:
		DISPLAY_ERROR( L"ChangeType Invalid" );
		break;
	}
}

/** @function : ChangeMpByValue
	@brief : ���� ���� Mp ����
	@param : Mp�� ����� ����(pGameUnit_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByValue( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
		pGameUnit_->UpNowMp( m_fResultValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, m_fResultValue );
	}
}

/** @function : ChangeMpByPercent
	@brief : %�� ���� Mp ����
	@param : Mp�� ����� ����(pGameUnit_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::ChangeMpByPercent( CX2GameUnit* pGameUnit_ )
{
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
	{
		const float fDeltaValue = pGameUnit_->GetMaxMp() * m_fResultValue;
		pGameUnit_->UpNowMp( fDeltaValue );
		if ( m_bDrawText )
			DrawText( pGameUnit_, fDeltaValue );
	}
}

/** @function : GetValueOrPercentPerSecond
	@brief : ���� ���� ����� �ʴ� ���� ��ġ or %�� ����
	@param : ���׷�(fRegist_)
	@return : �����(float)
*/
float CX2BuffChangeNowMpPerSecondBehaviorTemplet::GetValueOrPercentPerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const
{
	/// ���� * ����
	return (*pBehaviorFactor_)[FO_VALUE_OR_PERCENT] * ( 1.0f - fRegist_ );
}

/** @function : DrawText
	@brief : MP�� ��ȭ���� ����ϴ� �Լ�
	@param : ��������(pGameUnit_), ��ȭ��(fDeltaValue_)
*/
void CX2BuffChangeNowMpPerSecondBehaviorTemplet::DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ )
{
	const UINT uiSize = 20;
	WCHAR wstrText[uiSize] = {0, };
	D3DXVECTOR3 numPos = pGameUnit_->GetHeadBonePos();
	numPos.y += -50.0f;

	if ( fDeltaValue_ > 0.0f )
	{
		StringCchPrintf( wstrText, uiSize, L"MP +%d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}
	else
	{
		StringCchPrintf( wstrText, uiSize, L"MP %d", static_cast<int>( fDeltaValue_ ) );
		if( NULL != g_pData->GetPicCharRed() )
			g_pData->GetPicCharRed()->DrawText( wstrText, numPos, pGameUnit_->GetDirVector(), CKTDGPicChar::AT_CENTER );
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffReverseLeftRightBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	float fTimeInterval = 0.0f;	/// ���� �������� �¿찡 ���� �Ǵ°�?
	LUA_GET_VALUE_RETURN( luaManager_, "TIME_INTERVAL", fTimeInterval, 0.0f, return DISPLAY_ERROR( L"TIME_INTERVAL" ) );

	m_CheckElapsedTimeForInterval.SetTargetTime( fTimeInterval );
	return true;
}

/** @function : OnFrameMove
	@brief : interval Ÿ�̸� Update �� �¿���� ����
	@param : �¿������ ���ŵǾ� �ϴ� ��������(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_CheckElapsedTimeForInterval.OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_CheckElapsedTimeForInterval.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_CheckElapsedTimeForInterval.CheckAndResetElapsedTime() )
		pGameUnit_->ToggleReverseLeftRightByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffReverseLeftRightBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
		if ( !DidStart() )	/// ��ø ���� ����
		{
			// ��ü ���� ����(��ü�� ȿ���� �޶����°� �ƴϱ� ������ ������ �ʿ� ����)
			pGameUnit_->CreateAndInsertReverseLeftRightByBuff( pBuffTemplet_->GetBuffIdentity() ); /// �¿���� �ν��Ͻ� ���� �� ����
		}	
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffReverseLeftRightBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffReverseLeftRightBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffReverseLeftRightBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �¿������ �����Ű�� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	pGameUnit_->CreateAndInsertReverseLeftRightByBuff( kBuffFactor_.m_BuffIdentity );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffReverseLeftRightBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseReverseLeftRightByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffIdentityBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
		if ( !DidStart() )	/// ��ø ���� ����
		{
			// ��ü ���� ����(��ü�� ȿ���� �޶����°� �ƴϱ� ������ ������ �ʿ� ����)
			pGameUnit_->CreateAndInsertBuffIdentity( GetType(), pBuffTemplet_->GetBuffIdentity() ); /// ���ݺҰ� �ν��Ͻ� ���� �� ����
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffAttackImpossibleBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffIdentityBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffIdentityBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffIdentityBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ����(�޺�, ��ų)�� ���ϰ� �ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffIdentityBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	pGameUnit_->CreateAndInsertBuffIdentity( GetType(), kBuffFactor_.m_BuffIdentity );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffIdentityBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseBuffIdentity( GetType(), pBuffTemplet_->GetBuffIdentity() );		
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief :����(�̵��ӵ�, ũ��Ƽ��Ȯ����� ����)�� ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeStatBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_RELATION_TYPE", m_eRelationType, 
		BUFF_RELATION_TYPE, BRT_END, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeStatBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_RELATION_VALUE != m_eChangeType || STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, (*pBehaviorFactor)[FO_RELATION_TYPE] ) == m_eRelationType )
			{
				/// ������ �����ϴ� �͵� �ƴϰ� ������ ���� ������ �ִ� ��� ���װ��� ����

// 				const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 					0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

				const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

				const float fNewValue = GetValue( pBehaviorFactor, pGameUnit_, fRegist );
				if ( DidStart() )	/// ��ø �Ǵ� ��ü���
				{
					/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
					const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
					if ( uiAccumulationCountBefore > 0 )	/// ��ø �̶��
					{
						// ��ø�Ǿ��� �� ������ ����
						const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();

						if ( IsSamef( fAccumulationMultiflier ) )
							return DISPLAY_ERROR( L"AccumulationMultiflier must not be 0" );

						switch ( m_eChangeType )
						{
						case BCT_FIX_VALUE:
						case BCT_RELATION_VALUE:
							{
								// ���� ȿ���� ���밪	/// ù ��ø�̸�(uiAccumulationCountBefore <= 1) ������ ���밪�� old ������ �״�� ���
								const float fOldValue =
									(uiAccumulationCountBefore <= 1) ? m_fResultValue : ( m_fResultValue / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) ) );

								/// ���� ȿ���� ���� ȿ�� �� ���밪 ��ġ�� �� ���� ���� ���� (���̳ʽ��� ��쿡�� ȿ���� ū��)
								m_fResultValue = ( abs(fNewValue) > abs(fOldValue) ? fNewValue : fOldValue );
								/// ��ø ����
								/// ������ ��øȽ�� - 1 �� ���� �� ������� �ϴ� uiAccumulationCountBefore�� �̹� ������øȽ��-1�� �����Ƿ� �׳� ���Ѵ�.
								m_fResultValue += m_fResultValue * fAccumulationMultiflier * uiAccumulationCountBefore;	
							} break;

						case BCT_PERCENT:
							{
#ifdef ADJUST_BUFF_ACCMULATION_CALCULATE
								//�ٲ� ������ ù ȿ���� 1��ø�� �� ���̶�� �����ϰ� �ۼ���.
								//���� uiAccumulationCountBefore�� 1�� ������.
								m_fResultValue = 1 + ( ( fNewValue - 1.0f ) * ( uiAccumulationCountBefore + 1 ) );

								if( m_fResultValue < 0.0f )
								{
									//m_fREsultValue���� 0���� ���� �ʵ��� ������ ����ؼ� ����� �Ѵ�.(��ȹ��Ʈ���� ����)
									DISPLAY_ERROR( L"m_fResultValue error" );
									m_fResultValue = max( m_fResultValue, 0.0f );
								}
#else //ADJUST_BUFF_ACCMULATION_CALCULATE
								// ���� ȿ���� ���밪	/// ù ��ø�̸�(uiAccumulationCountBefore <= 1) ������ ���밪�� old ������ �״�� ���
								const float fOldValue =
									(uiAccumulationCountBefore <= 1) ? m_fResultValue : ( m_fResultValue / pow( fAccumulationMultiflier, static_cast<float>(uiAccumulationCountBefore - 1) ) );

								/// ���� ȿ���� ���� ȿ�� �� ���밪 ��ġ�� �� ���� ���� ���� (1.0 ���� ���� ��� 0.8 �� ���� 0.7 �谡 �� ���� ȿ���� �ִ� ���̹Ƿ� 0.7�� ����)
								m_fResultValue = ( abs(fNewValue - 1.0f) > abs(fOldValue - 1.0f) ? fNewValue: fOldValue );
								/// ��ø ����
								/// ������ ��øȽ�� - 1 �� ���� �� ������� �ϴ� uiAccumulationCountBefore�� �̹� ������øȽ��-1�� �����Ƿ� �׳� ���Ѵ�.
								m_fResultValue *= pow( fAccumulationMultiflier, static_cast<float>( uiAccumulationCountBefore ) );
								//m_fResultValue += 1.0f;
#endif //ADJUST_BUFF_ACCMULATION_CALCULATE
							} break;

						default:	/// ġȯ�� ��� �ൿ��Ҵ� �ߺ� ���� ����
							break;
						}
					}
					else	/// ��ü ���
						m_fResultValue = ( abs(fNewValue) > abs(m_fResultValue) ? fNewValue : m_fResultValue );

					pGameUnit_->UpdateAdditionalStat( GetType(), m_eChangeType, pBuffTemplet_->GetBuffIdentity(), m_fResultValue );
				}
				else	/// ��ø, ��ü�� �ƴ϶��(ù���� �Ǵ� �ߺ�)
				{
					m_fResultValue = fNewValue;
					pGameUnit_->InsertAdditionalStat( GetType(), m_eChangeType, pBuffTemplet_->GetBuffIdentity(), m_fResultValue );
				}
				return true;
			}
			else
				return DISPLAY_ERROR( L"MISS MATCH BUFF_RELATION_TYPE" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeStatBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeStatBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeStatBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeStatBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fResultValue = kFactor_[0];
	pGameUnit_->InsertAdditionalStat( GetType(), m_eChangeType, kBuffFactor_.m_BuffIdentity, m_fResultValue );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeStatBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseAdditionalStat( GetType(), m_eChangeType, pBuffTemplet_->GetBuffIdentity() );		
}

/** @function : GetValue
	@brief : ChangeType �� ���� ���� �ٸ� �������� ���氪�� ���� �Լ�
	@param : �ൿ���(pBehaviorFactor_), ������ ����� ��������(pGameUnit_), ����(fRegist_)
	@return : ���氪(float)
*/
float CX2BuffChangeStatBehaviorTemplet::GetValue( const KBuffBehaviorFactor* pBehaviorFactor_, CX2GameUnit* pGameUnit_, const float fRegist_ ) const
{
	const float fValue = (*pBehaviorFactor_)[FO_VALUE];

	switch ( m_eChangeType )
	{
	case BCT_SWAP_VALUE:
		{
			return fValue;
		} break;

	case BCT_FIX_VALUE:
		{
			if ( MustUseOptionData() )
			{
				const UINT uiLevel = pGameUnit_->GetUnitLevel();
				return fValue * ( 1.5f * (uiLevel - 1) + 9.5f ) * (1.0f - fRegist_);	/// �ɼ� ��ġȭ ��ũ��Ʈ�� ����� ����
			}
			else
			{
				return fValue * (1.0f - fRegist_);
			}
		} break;

	case BCT_RELATION_VALUE:
		{
			return fValue * (*pBehaviorFactor_)[FO_MULTIPLIER] * (1.0f - fRegist_);
		} break;

	case BCT_PERCENT:
		{
			/// �������� �����Ǵ� ���� ��� (fValue <1.0f && fValue >0.0f)
			/// fRegist_ ����			
			if ( fValue >= 1.0f )
				return fValue;
			else if ( fValue > 0.0f )
				return 1.0f - ( (1.0f - fValue) * (1.0f - fRegist_) );
			else
				return fValue;
		} break;

	default:
		return 0.0f;
		break;
	}
}

/** @function : MustUseOptionData
	@brief : �ɼ� ��ġȭ�� ����ؾ� �ϴ� �ൿ������ ����
	@return : �ɼ� ��ġ�� ����ؾ� �ϸ� true, �׷��� ������ false
*/
bool CX2BuffChangeStatBehaviorTemplet::MustUseOptionData() const
{
	switch ( GetType() )
	{
	case BBT_CHANGE_ANIM_SPEED:
	case BBT_CHANGE_WALK_RUN_SPEED:
	case BBT_CHANGE_JUMP_POWER:
	case BBT_CHANGE_ACCURACY:
	case BBT_CHANGE_AVOIDANCE:
	case BBT_CHANGE_CRITICAL_RATE:
	case BBT_CHANGE_ADDITIONAL_ATTACK:
	case BBT_CHANGE_ADDITIONAL_DEFENCE:
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case BBT_CHANGE_HYPER_CHARGE_SPEED:
#endif //SERV_ADD_LUNATIC_PSYKER
		return true;
		break;
		
	default:
		return false;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffCreateBuffFactortBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		m_uiBuffFactorId = static_cast<UINT>( (*pBehaviorFactor)[FO_BUFF_FACTOR_ID] );
		if ( 0 < m_uiBuffFactorId )
		{
			if ( DidStart() )	/// ������ �۵� ���� ���̾����� ������ ȿ�� ����
				pGameUnit_->EraseTemporaryBuffFactor( pBuffTemplet_->GetBuffIdentity() );

		#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			m_uiSkillId = static_cast<UINT>( (*pBehaviorFactor)[FO_SKILL_ID] );
			if ( 0 > m_uiSkillId )
				return DISPLAY_ERROR( L"SKILL_ID doesn't exist" );
		#endif //UPGRADE_SKILL_SYSTEM_2013

			CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( m_uiBuffFactorId );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( pGameUnit_->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				CX2GUUser* pUser = reinterpret_cast<CX2GUUser*>( pGameUnit_ );
				
				if ( NULL == pUser->GetUnit() )
					return false;

				const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SKILL_ID( m_uiSkillId ) ) );	/// ��ų ����
				pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, pBuffTemplet_->GetBuffIdentity(), iSkillTempletLevel );

				return true;
			}
#else //UPGRADE_SKILL_SYSTEM_2013
			pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, pBuffTemplet_->GetBuffIdentity() );
#endif //UPGRADE_SKILL_SYSTEM_2013
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_FACTOR_ID doesn't exist" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCreateBuffFactortBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffCreateBuffFactortBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffCreateBuffFactortBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffCreateBuffFactortBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_uiBuffFactorId ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ٸ� ���ֿ��� ������ ���� �Ǵ� ������� ������ �� �ִ� �ൿ ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffCreateBuffFactortBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_uiBuffFactorId = static_cast<UINT>( kFactor_[0] );
	CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( m_uiBuffFactorId );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( pGameUnit_->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser* pUser = reinterpret_cast<CX2GUUser*>( pGameUnit_ );
				
		if ( NULL == pUser->GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SKILL_ID( m_uiSkillId ) ) );	/// ��ų ����

		pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, kBuffFactor_.m_BuffIdentity, iSkillTempletLevel );
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	pGameUnit_->CreateAndInsertTemporaryBuffFactor( *ptrBuffFactor, kBuffFactor_.m_BuffIdentity );
#endif //UPGRADE_SKILL_SYSTEM_2013
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffCreateBuffFactortBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseTemporaryBuffFactor( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffAbsorbEffectAttackBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( DidStart() )	/// ��ø �Ǵ� ��ü
		{
			const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
			
			if ( uiAccumulationCountBefore > 1 )	/// ��ø �̶��
				m_fAbsorbValue += (*pBehaviorFactor)[FO_ABSORB_VALUE];
			else
				m_fAbsorbValue = (*pBehaviorFactor)[FO_ABSORB_VALUE];

			pGameUnit_->UpdateAbsorbEffectAttack( pBuffTemplet_->GetBuffIdentity(), m_fAbsorbValue );
		}
		else	/// ùȿ�� �Ǵ� �ߺ�
		{
			m_fAbsorbValue = (*pBehaviorFactor)[FO_ABSORB_VALUE];
			pGameUnit_->CreateAndInsertAbsorbEffectAttack( pBuffTemplet_->GetBuffIdentity(), m_fAbsorbValue );
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffAbsorbEffectAttackBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffAbsorbEffectAttackBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffAbsorbEffectAttackBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffAbsorbEffectAttackBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
	
	BehaviorFactor.m_vecValues.push_back( m_fAbsorbValue );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ����Ʈ�� ����Ͽ� MP�� ��ȯ�ϴ� �ൿ ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffAbsorbEffectAttackBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fAbsorbValue = kFactor_[0];
	pGameUnit_->CreateAndInsertAbsorbEffectAttack( kBuffFactor_.m_BuffIdentity, m_fAbsorbValue );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAbsorbEffectAttackBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseAbsorbEffectAttack( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// /** @function : SetFactor
// 	@brief : BuffFactor�� ����� ������ Templet ����
// 	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
// */
// /*virtual*/ bool CX2BuffChangeMpHitBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
// {
// 	const KBuffBehaviorFactor* pBehaviorFactor = NULL;
// 
// 	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
// 	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
// 	{
// 		/// ������ �����ϴ� �͵� �ƴϰ� ������ ���� ������ �ִ� ��� ���װ��� ����
// 		const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 			0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );
// 		// ���� ȿ���� ���밪
// 		const float fNewMultiplier = (*pBehaviorFactor)[FO_MULTIPLIER] * (1.0f - fRegist);
// 
// 		if ( DidStart() )	/// ��ø �Ǵ� ��ü���
// 		{
// 			/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
// 			const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
// 			if ( uiAccumulationCountBefore > 0 )	/// ��ø �̶��
// 			{
// 				// ��ø�Ǿ��� �� ������ ����
// 				const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
// 				// ���� ȿ���� ���밪
// 				const float fOldMultiplier = ( m_fMultiplier - 1.0f ) / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
// 
// 				/// ���� ȿ���� ���� ȿ�� �� ���밪 ��ġ�� �� ���� ���� ����
// 				m_fMultiplier 
// 					= ( abs(fNewMultiplier - 1.0f) > abs(fOldMultiplier) ? fNewMultiplier - 1.0f : fOldMultiplier );
// 				/// ��ø ����
// 				m_fMultiplier *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// ������ ��øØ�� - 1�� ������� �ϴ� Before�� ����-1�� �����Ƿ� �׳� ���Ѵ�.
// 				m_fMultiplier += 1.0f;
// 			}
// 			else	/// ��ü ���
// 				m_fMultiplier = ( abs(fNewMultiplier) > abs(m_fMultiplier) ? fNewMultiplier : m_fMultiplier );
// 			
// 			pGameUnit_->UpdateAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		else	/// ùȿ�� �Ǵ� �ߺ�
// 		{
// 			m_fMultiplier = fNewMultiplier;
// 			pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		return true;
// 	}
// 	else
// 		return DISPLAY_ERROR( L"CX2BuffChangeMpHitBehaviorTemplet GetBehaviorFactor didn't work" );
// }
// 
// /** @function : GetClonePtr
// 	@return : CX2BuffBehaviorTempletPtr
// */
// /*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeMpHitBehaviorTemplet::GetClonePtr() const
// {
// 	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHitBehaviorTemplet( *this ) );
// }
// 
// /** @function : GetFactor
// 	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
// 	@param : ������� �ν��Ͻ�(factor_)
// */
// /*virtual*/ void CX2BuffChangeMpHitBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
// {
// 	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
// 	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
// 
// 	BehaviorFactor.m_vecValues.push_back( m_fMultiplier );
// }
// 
// /** @function : SetFactorFromPacket
// 	@brief : ��Ŷ���� ���� ���� ���� ������ Ÿ�ݽ� MP ȸ������ �����ϴ� �ൿ ���ø��� ����
// 	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
// */
// /*virtual*/ void CX2BuffChangeMpHitBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
// {
// 	m_fMultiplier = kFactor_[0];
// 	pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, kBuffFactor_.m_BuffIdentity, m_fMultiplier );
// }
// 
// /** @function : DoFinish
// 	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
// 	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
// */
// /*virtual*/ void CX2BuffChangeMpHitBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
// {
// 	pGameUnit_->EraseAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity() );
// }
// 
// //////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// /** @function : SetFactor
// 	@brief : BuffFactor�� ����� ������ Templet ����
// 	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
// */
// /*virtual*/ bool CX2BuffChangeMpHittedBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
// {
// 	const KBuffBehaviorFactor* pBehaviorFactor = NULL;
// 
// 	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
// 	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
// 	{
// 		/// ������ �����ϴ� �͵� �ƴϰ� ������ ���� ������ �ִ� ��� ���װ��� ����
// 		const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 			0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );
// 		// ���� ȿ���� ���밪
// 		const float fNewMultiplier = (*pBehaviorFactor)[FO_MULTIPLIER] * (1.0f - fRegist);
// 
// 		if ( DidStart() )	/// ��ø �Ǵ� ��ü���
// 		{
// 			/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
// 			const UINT uiAccumulationCountBefore = pBuffTemplet_->GetAccumulationCountNow();
// 			if ( uiAccumulationCountBefore > 0 )	/// ��ø �̶��
// 			{
// 				// ��ø�Ǿ��� �� ������ ����
// 				const float fAccumulationMultiflier = pBuffTemplet_->GetAccumulationMultiflier();
// 				// ���� ȿ���� ���밪
// 				const float fOldMultiplier = ( m_fMultiplier - 1.0f ) / ( 1 + fAccumulationMultiflier * (uiAccumulationCountBefore - 1) );
// 
// 				/// ���� ȿ���� ���� ȿ�� �� ���밪 ��ġ�� �� ���� ���� ����
// 				m_fMultiplier 
// 					= ( abs(fNewMultiplier - 1.0f) > abs(fOldMultiplier) ? fNewMultiplier - 1.0f : fOldMultiplier );
// 				/// ��ø ����
// 				m_fMultiplier *= ( 1 + fAccumulationMultiflier * uiAccumulationCountBefore );	/// ������ ��øØ�� - 1�� ������� �ϴ� Before�� ����-1�� �����Ƿ� �׳� ���Ѵ�.
// 				m_fMultiplier += 1.0f;
// 			}
// 			else	/// ��ü ���
// 				m_fMultiplier = ( abs(fNewMultiplier) > abs(m_fMultiplier) ? fNewMultiplier : m_fMultiplier );
// 
// 			pGameUnit_->UpdateAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		else	/// ùȿ�� �Ǵ� �ߺ�
// 		{
// 			m_fMultiplier = fNewMultiplier;
// 			pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity(), m_fMultiplier );
// 		}
// 		return true;
// 	}
// 	else
// 		return DISPLAY_ERROR( L"CX2BuffChangeMpHittedBehaviorTemplet GetBehaviorFactor didn't work" );
// }
// 
// /** @function : GetClonePtr
// 	@return : CX2BuffBehaviorTempletPtr
// */
// /*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeMpHittedBehaviorTemplet::GetClonePtr() const
// {
// 	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHittedBehaviorTemplet( *this ) );
// }
// 
// /** @function : GetFactor
// 	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
// 	@param : ������� �ν��Ͻ�(factor_)
// */
// /*virtual*/ void CX2BuffChangeMpHittedBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
// {
// 	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
// 	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
// 
// 	BehaviorFactor.m_vecValues.push_back( m_fMultiplier );
// }
// 
// /** @function : SetFactorFromPacket
// 	@brief : ��Ŷ���� ���� ���� ���� ������ �ǰݽ� ȸ���Ǵ� MP���� ������Ű�� �ൿ ���ø��� ����
// 	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
// */
// /*virtual*/ void CX2BuffChangeMpHittedBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
// {
// 	m_fMultiplier = kFactor_[0];
// 	pGameUnit_->InsertAdditionalStat( GetType(), BCT_SWAP_VALUE, kBuffFactor_.m_BuffIdentity, m_fMultiplier );
// }
// 
// /** @function : DoFinish
// 	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
// 	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
// */
// /*virtual*/ void CX2BuffChangeMpHittedBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
// {
// 	pGameUnit_->EraseAdditionalStat( GetType(), BCT_SWAP_VALUE, pBuffTemplet_->GetBuffIdentity() );		
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : OnFrameMove
	@brief : �����ֱ�� ���� �ð��� ����
	@param : �������� ����(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( 0.0f < m_fRemainTime )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_fRemainTime -= fElapsedTime_;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_fRemainTime -= g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_fRemainTime = max( m_fRemainTime, 0.0f );
		pGameUnit_->SetRemainHyperModeTime( m_fRemainTime );
// robobeg : frame move ȣ�� �󵵿� ���� ����Ʈ�� �޶��� ���� �ִ�. --;
		pGameUnit_->HyperModeBuffEffectOnFrameMove();
	}
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeHyperModeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( 0 == m_uiHyperModeCount )	/// �簢�� ���� �ƴ� ��쿡�� ����(3���� �� 1ī��Ʈ�� �簢�� ���� ��  3������ 1�������� ������� �ʵ���!)
			m_uiHyperModeCount = max( m_uiHyperModeCount, static_cast<UINT>( (*pBehaviorFactor)[FO_HYPER_MODE_COUNT] ) );

		if ( static_cast<UINT>(CX2GageUI::PGUWO_ORB3) < m_uiHyperModeCount )
			pGameUnit_->SetFullHyperMode( true );

		m_fRemainTime = m_fRemainTime + (*pBehaviorFactor)[FO_HYPER_MODE_TIME];
		pGameUnit_->SetRemainHyperModeTime( m_fRemainTime );
		pGameUnit_->HyperModeBuffEffectStart();
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeHyperModeBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeHyperModeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeHyperModeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_uiHyperModeCount ) );
	BehaviorFactor.m_vecValues.push_back( m_fRemainTime );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� �ൿ ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_uiHyperModeCount = static_cast<UINT>( kFactor_[0] );
	if ( static_cast<UINT>(CX2GageUI::PGUWO_ORB3) < m_uiHyperModeCount )
		pGameUnit_->SetFullHyperMode( true );

	m_fRemainTime = kFactor_[1];
	pGameUnit_->SetRemainHyperModeTime( m_fRemainTime );
	pGameUnit_->HyperModeBuffEffectStart();
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeHyperModeBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	CX2GageData* pGageData = CX2GageManager::GetInstance()->GetMyGageData();
	if ( NULL != pGageData )
		pGageData->SetHyperModeRemainTime( 0.0f );

	pGameUnit_->SetRemainHyperModeTime( 0.0f );
	pGameUnit_->HyperModeBuffEffectEnd();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : OnFrameMove
	@brief : ��ų ��Ÿ�� �ʱ�ȭ
	@param : ������ �ɸ� ��������(pGameUnit_), ������ ���ø�����(pBuffTemplet_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffResetSkillCoolTimeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffResetSkillCoolTimeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( DidStart() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// ����ȯ

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
		pGameUnit_->ResetLeftSkillCoolTimeBySkillType( static_cast<CX2SkillTree::SKILL_TYPE>( m_eSkillTypeToResetCoolTime ), m_fCoolTime, false, m_eChangeType, m_fCoolTimeRate, m_bIsRelativeAllSkill );
#else // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
		pGameUnit_->ResetLeftSkillCoolTimeBySkillType( static_cast<CX2SkillTree::SKILL_TYPE>( m_eSkillTypeToResetCoolTime ), m_fCoolTime, false, m_eChangeType );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
		
#else // UPGRADE_SKILL_SYSTEM_2013
		pGameUnit_->ResetLeftSkillCoolTimeBySkillType( static_cast<CX2SkillTree::SKILL_TYPE>( m_eSkillTypeToResetCoolTime ) );
#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffResetSkillCoolTimeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// ����ȯ
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			switch ( m_eChangeType )
			{
			case BCT_FIX_VALUE:
			case BCT_RELATION_VALUE:
				{
					m_fCoolTime = (*pBehaviorFactor)[FO_VALUE];
					return true;

				} break;
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
			case BCT_PERCENT :
				{
					m_fCoolTimeRate = (*pBehaviorFactor)[FO_VALUE];

					if ( m_fCoolTimeRate < 0 || m_fCoolTimeRate > 1 ) 
						return DISPLAY_ERROR( L"MISS MATCH BCT_PERCENT" );

					return true;			
				} break;
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
			
			default:
				{
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME				// ������, ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
					return DISPLAY_ERROR( L"BCT_FIX_VALUE or BCT_RELATION_VALUE or BCT_PERCENT" );
#else  // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
					return DISPLAY_ERROR( L"BCT_FIX_VALUE or BCT_RELATION_VALUE" );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������, ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
				} break;
			}
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
#endif // UPGRADE_SKILL_SYSTEM_2013
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffResetSkillCoolTimeBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffResetSkillCoolTimeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffResetSkillCoolTimeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffResetSkillCoolTimeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
}

/** @function : ParsingBehavior
	@brief : �������ø� ��ũ��Ʈ�� ���� �ʱ�ȭ �� ��ų Ÿ���� �о����
	@param : ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffResetSkillCoolTimeBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// ����ȯ
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
#endif // UPGRADE_SKILL_SYSTEM_2013

	LUA_GET_VALUE_RETURN( luaManager_, "SKILL_TYPE", m_eSkillTypeToResetCoolTime, 0, return DISPLAY_ERROR( L"SKILL_TYPE") );

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
	LUA_GET_VALUE ( luaManager_, "RELATIVE_ALL_SKILL", m_bIsRelativeAllSkill, 0 );	
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeUnitScaleBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )	/// ó�� ����
		{
			pGameUnit_->CreateAndInsertChangeUnitScaleByBuff( pBuffTemplet_->GetBuffIdentity(), m_vScale );
		}
#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
		else				/// ��ø �Ǵ� ��ü
		{
			pGameUnit_->UpdateChangeUnitScaleByBuff( pBuffTemplet_->GetBuffIdentity(), m_vScale,
													pBuffTemplet_->GetAccumulationLimit());
		}
#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeUnitScaleBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeUnitScaleBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeUnitScaleBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeUnitScaleBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �������� �����ϴ� �ൿ ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeUnitScaleBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertChangeUnitScaleByBuff( kBuffFactor_.m_BuffIdentity, m_vScale );
}

/** @function : ParsingBehavior
	@brief : ������ �������� �����ϴ� �ൿ ���ø��� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeUnitScaleBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "SCALE_X", m_vScale.x, 0.0f, return DISPLAY_ERROR( L"SCALE_X" ) );
	LUA_GET_VALUE_RETURN( luaManager_, "SCALE_Y", m_vScale.y, 0.0f, return DISPLAY_ERROR( L"SCALE_Y" ) );
	LUA_GET_VALUE_RETURN( luaManager_, "SCALE_Z", m_vScale.z, 0.0f, return DISPLAY_ERROR( L"SCALE_Z" ) );

	/// �������� ������� ���ư� �� ������ �ð����ݰ� ����� ����� ���ΰ�
	LUA_GET_VALUE_RETURN( luaManager_, "USE_TIME_TO_RETURN", m_bUseTimeToReturn, false, return DISPLAY_ERROR( L"USE_TIME_TO_RETURN" ) );

	return true;
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeUnitScaleBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseChangeUnitScaleByBuff( pBuffTemplet_->GetBuffIdentity(), m_bUseTimeToReturn );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffCustomFunctionBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )
		{
			m_kBuffBehaviorFactor = *pBehaviorFactor;
			pGameUnit_->CreateAndInsertCustomFunctionByBuff( pBuffTemplet_->GetBuffIdentity(), *pBehaviorFactor );
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCustomFunctionBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffCustomFunctionBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffCustomFunctionBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffCustomFunctionBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( m_kBuffBehaviorFactor );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ CustomFunction�� ���ֿ� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffCustomFunctionBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertCustomFunctionByBuff( kBuffFactor_.m_BuffIdentity, kFactor_ );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffCustomFunctionBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseCustomFunctionByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffEffectSetWithDamageBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( NULL != g_pX2Game && !DidStart() )
		{
			m_bNotUse = ( 0.0f < (*pBehaviorFactor)[0] ? true : false );
			m_fPowerRate = pGameUnit_->GetPowerRate();	/// ��� ����� PowerRate ����
			if ( !m_bNotUse )	/// FACTOR���� ������� �ʰڴٰ� �����Ѱ��� �ƴϸ�
				PlayEffectSet( pGameUnit_ );
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffEffectSetWithDamageBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/CX2BuffBehaviorTempletPtr CX2BuffEffectSetWithDamageBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffEffectSetWithDamageBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/void CX2BuffEffectSetWithDamageBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( ( true == m_bNotUse ? 1.0f : 0.0f ) );
	BehaviorFactor.m_vecValues.push_back( m_fPowerRate );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �������� ����Ʈ���� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/void CX2BuffEffectSetWithDamageBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_bNotUse = ( 0.0f < kFactor_[0] ? true : false );
	m_fPowerRate = kFactor_[1];
	if ( !m_bNotUse )	/// FACTOR���� ������� �ʰڴٰ� �����Ѱ��� �ƴϸ�
		PlayEffectSet( pGameUnit_ );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/void CX2BuffEffectSetWithDamageBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && DidStart() )
	{
		BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
		}
	}
}

/** @function : ParsingBehavior
	@brief : ������ �������� �����ϴ� �ൿ ���ø��� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/bool CX2BuffEffectSetWithDamageBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	int iIndex = 1;

	wstring wstrEffectSetName;
	while( luaManager_.GetValue( iIndex, wstrEffectSetName ) )
	{
		m_vecWstrEffectSetName.push_back( wstrEffectSetName );
		m_vecHandleEffectSet.push_back( INVALID_EFFECTSET_HANDLE );
		wstrEffectSetName.resize(0);
		++iIndex;
	}

	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : PlayEffectSet
	@brief : ���Ϳ� �ִ� ����Ʈ �µ��� ����
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_)
*/
void CX2BuffEffectSetWithDamageBehaviorTemplet::PlayEffectSet( CX2GameUnit* pGameUnit_ )
{
	if ( NULL != g_pX2Game && !DidStart() )
	{
		UINT uiSize = m_vecWstrEffectSetName.size();
		for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
		{
			m_vecHandleEffectSet[uiIndex] 
			= g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecWstrEffectSetName[uiIndex], pGameUnit_,
				NULL, ( 0.0f < pGameUnit_->GetRemainHyperModeTime() ), m_fPowerRate, -1.0f);//, pGameUnit_->GetScaleByUnit() );
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffStunBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
		if ( !DidStart() )	/// ��ø ���� ����
		{
			// ��ü ���� ����(��ü�� ȿ���� �޶����°� �ƴϱ� ������ ������ �ʿ� ����)
			pGameUnit_->CreateAndInsertStunByBuff( pBuffTemplet_->GetBuffIdentity() ); /// �¿���� �ν��Ͻ� ���� �� ����

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT		/// ���� ��� �׷α� ���·� ���� ( CommonFrameMove���� ó�� ����ÿ� ������ �׷α� ���� �����ϵ��� ���� )
			if( CX2GameUnit::GUT_USER ==  pGameUnit_->GetGameUnitType() )
			{
				CX2GUUser *pUser = static_cast<CX2GUUser*>( pGameUnit_ );
				if( NULL != pUser )
					pUser->SetChangeGroggyState( true );
			}
#endif NOT_CANCEL_BBT_STUN_BY_HIT
		}	
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffStunBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/CX2BuffBehaviorTempletPtr CX2BuffStunBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffStunBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/void CX2BuffStunBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �������� ����Ʈ���� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/void CX2BuffStunBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	pGameUnit_->CreateAndInsertStunByBuff( kBuffFactor_.m_BuffIdentity );

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT		/// ���� ��� �׷α� ���·� ���� ( CommonFrameMove���� ó�� ����ÿ� ������ �׷α� ���� �����ϵ��� ���� )
	if( CX2GameUnit::GUT_USER ==  pGameUnit_->GetGameUnitType() )
	{
		CX2GUUser *pUser = static_cast<CX2GUUser*>( pGameUnit_ );
		if( NULL != pUser )
			pUser->SetChangeGroggyState( true );
	}
#endif NOT_CANCEL_BBT_STUN_BY_HIT
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/void CX2BuffStunBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseStunByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffFinishOtherBuffDebuffBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		EraseBuffTempletFromGameUnit( pGameUnit_, BuffFactor_.GetBuffTempletID() );
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffFinishOtherBuffDebuffBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffFinishOtherBuffDebuffBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffFinishOtherBuffDebuffBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffFinishOtherBuffDebuffBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ Ư�� �������� ���� ��Ŵ
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
///*virtual*/ void CX2BuffFinishOtherBuffDebuffBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
// {
// 	EraseBuffTempletFromGameUnit( pGameUnit_ );
// }

/** @function : ParsingBehavior
	@brief : ���� ��ų ������ ���ø�ID�� ��ƽ�ũ��Ʈ�� ���� �о���δ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffFinishOtherBuffDebuffBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	int iIndex = 1;

	UINT uiBuffTempletID = 0;
	while( luaManager_.GetValue( iIndex, uiBuffTempletID ) )
	{
		m_vecBuffTempletIDToBeFinished.push_back( static_cast<BUFF_TEMPLET_ID>( uiBuffTempletID ) );
		++iIndex;
	}

	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : EraseBuffTempletFromGameUnit
	@brief : Ư�� ������ �ִ� ��� �����Ų��
	@param : ������ �ɸ� ���� ����(pGameUnit_), �������� ������ BuffTempletID�� �ڱ��ڽ��� �����ϴ� ��츦 �����ϱ� ���� �ڽ��� BuffTempletID�� �־��ش�.
*/
void CX2BuffFinishOtherBuffDebuffBehaviorTemplet::EraseBuffTempletFromGameUnit( CX2GameUnit* pGameUnit_, const BUFF_TEMPLET_ID eExceptBuffTempletID_ )
{
	BOOST_FOREACH( const BUFF_TEMPLET_ID eBuffTempletID_, m_vecBuffTempletIDToBeFinished )
	{
		if ( eExceptBuffTempletID_ != eBuffTempletID_ )
			pGameUnit_->EraseBuffTempletFromGameUnit( eBuffTempletID_ );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffFreezeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		/// ��ø�� �����ϴٸ� ���� �󸶳� ��ø�Ǿ� �ִ����� �� (ù ȿ�� or ��ø�� �ȵǸ� 0 �̴�)
		if ( !DidStart() )	/// ��ø ���� ����
		{
			// ��ü ���� ����(��ü�� ȿ���� �޶����°� �ƴϱ� ������ ������ �ʿ� ����)
			pGameUnit_->CreateAndInsertFreezeByBuff( pBuffTemplet_->GetBuffIdentity() ); /// �¿���� �ν��Ͻ� ���� �� ����
			m_fAnimCurrentTime = pGameUnit_->GetNowAnimationTime();
			pGameUnit_->AnimStop();
		}	
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffStunBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/CX2BuffBehaviorTempletPtr CX2BuffFreezeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffFreezeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/void CX2BuffFreezeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_fAnimCurrentTime );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �������� ����Ʈ���� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/void CX2BuffFreezeBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fAnimCurrentTime = kFactor_[0];
	pGameUnit_->SetNowAnimationTime( m_fAnimCurrentTime );
	pGameUnit_->AnimStop();
	//pGameUnit_->CreateAndInsertFreezeByBuff( kBuffFactor_.m_BuffIdentity );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/void CX2BuffFreezeBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseFreezeByBuff( pBuffTemplet_->GetBuffIdentity() );

	pGameUnit_->SetStateToNowAnimation( CKTDGXSkinAnim::XAS_PLAYING );

	pGameUnit_->AnimPlay();

	if( true == m_bIsDown && CX2GameUnit::GUT_USER == pGameUnit_->GetGameUnitType())		/// ���� ����� �ٿ��ų�� ���� ( NPC�� ���, �ٿ� ��Ű�� �ʴ´�. )
	{
#ifdef RIDING_SYSTEM
		/// Ż�� ž�����̸�, Ż�� ������ ������Ʈ�� ��ȯ
		if ( true == pGameUnit_->GetRidingOn() )
			pGameUnit_->StateChange( CX2GUUser::USI_RIDING_DAMAGE_FRONT, true );
		else
#endif // RIDING_SYSTEM
			pGameUnit_->StateChange( CX2GUUser::USI_DAMAGE_DOWN_FRONT, true );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffFreezeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffFreezeBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( DidStart() )
		pGameUnit_->AnimStop();
}

/** @function : ParsingBehavior
	@brief : ������ ���ߴ� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffFreezeBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "IS_DOWN", m_bIsDown, false, return DISPLAY_ERROR( L"IS_DOWN" ) );

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffCanPassUnitBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCanPassUnitBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffCanPassUnitBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffCanPassUnitBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffCanPassUnitBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : OnFrameMove
	@brief : �� ������ SetCanPassUnit ����
	@param : ������ �ɸ� ��������(pGameUnit_), ������ ���ø�����(pBuffTemplet_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffCanPassUnitBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffCanPassUnitBehaviorTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( DidStart() )
		pGameUnit_->SetCanPassUnit( m_bCanPassUnit );
}

/** @function : ParsingBehavior
	@brief : ���� ��ų ������ ���ø�ID�� ��ƽ�ũ��Ʈ�� ���� �о���δ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffCanPassUnitBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "CAN_PASS_UNIT", m_bCanPassUnit, 
		false, return DISPLAY_ERROR( L"CAN_PASS_UNIT" ) );
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NEVER_MOVE_GRAVITY_APPLY
/** @function : ParsingBehavior
	@brief : �߷��� ����Ǵ°��� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffNeverMoveBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "IS_GRAVITY", m_bIsGravity, false, return DISPLAY_ERROR( L"IS_GRAVITY" ) );

	return true;
}
#endif //NEVER_MOVE_GRAVITY_APPLY


/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffNeverMoveBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )
		{
			m_vPosition = pGameUnit_->GetPos(true);
#ifdef NEVER_MOVE_GRAVITY_APPLY
			pGameUnit_->CreateAndInsertNeverMoveByBuff( pBuffTemplet_->GetBuffIdentity(), m_vPosition, m_bIsGravity );
#else
			pGameUnit_->CreateAndInsertNeverMoveByBuff( pBuffTemplet_->GetBuffIdentity(), m_vPosition );
#endif //NEVER_MOVE_GRAVITY_APPLY
		}
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffNeverMoveBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffNeverMoveBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffNeverMoveBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffNeverMoveBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( m_vPosition.x );
	BehaviorFactor.m_vecValues.push_back( m_vPosition.y );
	BehaviorFactor.m_vecValues.push_back( m_vPosition.z );
#ifdef NEVER_MOVE_GRAVITY_APPLY
	BehaviorFactor.m_vecValues.push_back( ( true == m_bIsGravity ? 1.0f : 0.0f ) );
#endif //NEVER_MOVE_GRAVITY_APPLY
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ൿ ���ø� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffNeverMoveBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_vPosition.x = kFactor_[0];
	m_vPosition.y = kFactor_[1];
	m_vPosition.z = kFactor_[2];
#ifdef NEVER_MOVE_GRAVITY_APPLY
	m_bIsGravity  = ( 0.0f < kFactor_[3] ? true : false );
#endif //NEVER_MOVE_GRAVITY_APPLY

#ifdef NEVER_MOVE_GRAVITY_APPLY
	pGameUnit_->CreateAndInsertNeverMoveByBuff( kBuffFactor_.m_BuffIdentity, m_vPosition, m_bIsGravity );
#else
	pGameUnit_->CreateAndInsertNeverMoveByBuff( kBuffFactor_.m_BuffIdentity, m_vPosition );
#endif //NEVER_MOVE_GRAVITY_APPLY
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffNeverMoveBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseNeverMoveByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : Ÿ�Կ� ���� ���ݷ� ��ġ�� ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeAttackByTypeBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeAttackByTypeBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_PERCENT == m_eChangeType )	/// �ϴ� ������ ����
			{
				/*if ( DidStart() )
				{*/
					m_fResultValue = (*pBehaviorFactor)[FO_VALUE];
					m_eAttackType = STATIC_CAST_FLOAT_TO_ENUM( CX2DamageManager::ATTACK_TYPE, (*pBehaviorFactor)[FO_ATTACK_TYPE] );

					pGameUnit_->CreateAndInsertChangeAttackByType( pBuffTemplet_->GetBuffIdentity(), m_eAttackType, m_fResultValue );

					return true;
				//}
			}
			else
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeAttackByTypeBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackByTypeBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeAttackByTypeBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eAttackType ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeAttackByTypeBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( kFactor_.m_vecValues.size() >= static_cast<UINT>( FO_END ) )
	{
		m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
		m_fResultValue	= kFactor_.m_vecValues[FO_VALUE];
		m_eAttackType	= STATIC_CAST_FLOAT_TO_ENUM( CX2DamageManager::ATTACK_TYPE, kFactor_.m_vecValues[FO_ATTACK_TYPE] );

		pGameUnit_->CreateAndInsertChangeAttackByType( kBuffFactor_.m_BuffIdentity, m_eAttackType, m_fResultValue );
	}
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeAttackByTypeBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseChangeAttackByType( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : ���� �Ӽ� ���� �ߵ� Ȯ���� ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeEncahntAttackRateBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ���� Ÿ�Կ� ���� ���� �����ϵ��� �߰�
	LUA_GET_VALUE_ENUM ( luaManager_, "ENCHANT_ATTRIBUTE_TYPE", m_eEnchantAttributeType, 
		BUFF_ENCHANT_ATTRIBUTE_TYPE, BEAT_ALL );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	return true;
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeEncahntAttackRateBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;


	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE �� Ư�� �Ӽ��� ��ȭ ��Ű�� BBT ��
			// m_eEnchantAttributeType ���� �����ؼ� �ִ´�.
			// �Ŀ� Ȯ�� ��� �� �ش� �Ӽ��� ���ؼ��� üũ�ϰ� ��
			switch ( GetType() )
			{
			case BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_BLAZE;
				break;
			case BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_FROZEN;
				break;
			case BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_POSION;
				break;
			case BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_SHOCK;
				break;
			case BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_SNATCH;
				break;
			case BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE :
				m_eEnchantAttributeType = BEAT_PIERCING;			
				break;
			}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����


			if ( BCT_PERCENT == m_eChangeType ) /// �ϴ� ������ ����
			{
				/*if ( DidStart() )
				{*/
					m_fRate = (*pBehaviorFactor)[FO_VALUE];
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
					// ���� Ÿ�� �� ���� ���ǿ� ���� �߰�
					pGameUnit_->CreateAndInsertChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity(), m_fRate, m_eEnchantAttributeType, m_eChangeType );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����					
					pGameUnit_->CreateAndInsertChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity(), m_fRate );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����					

					return true;
				//}
			}
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// FIX_VALUE ���� Ȯ��, ��æƮ �Ӽ� �ߵ� Ȯ���� 0 �� �Ѿ���� ����ȴ�.
			else if ( BCT_FIX_VALUE == m_eChangeType )		
			{
				m_fRate = (*pBehaviorFactor)[FO_VALUE];
				// ���� Ÿ�� �� ���� ���ǿ� ���� �߰�
				pGameUnit_->CreateAndInsertChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity(), m_fRate, m_eEnchantAttributeType, m_eChangeType );
				return true;
			}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			else
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT OR BCT_FIX_VALUE" );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeEncahntAttackRateBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeEncahntAttackRateBehaviorTemplet( *this ) );

}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeEncahntAttackRateBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );

	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fRate );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeEncahntAttackRateBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( kFactor_.m_vecValues.size() >= static_cast<UINT>( FO_END ) )
	{
		m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
		m_fRate = kFactor_.m_vecValues[FO_VALUE];

		pGameUnit_->CreateAndInsertChangeEnchantAttackRate( kBuffFactor_.m_BuffIdentity, m_fRate );
	}
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeEncahntAttackRateBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseChangeEnchantAttackRate( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingBehavior
	@brief : ���� �Ӽ� ���� �ߵ� Ȯ���� ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffDoubleAttackBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffDoubleAttackBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_PERCENT == m_eChangeType ) /// �ϴ� ������ ����
			{
				/*if ( DidStart() )
				{*/
					m_fRate = (*pBehaviorFactor)[FO_VALUE];

					pGameUnit_->CreateAndInsertDoubleAttackByBuff( pBuffTemplet_->GetBuffIdentity(), m_fRate );

					return true;
				//}
			}
			else
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffDoubleAttackBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffDoubleAttackBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffDoubleAttackBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );

	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fRate );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffDoubleAttackBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	if ( kFactor_.m_vecValues.size() >= static_cast<UINT>( FO_END ) )
	{
		m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
		m_fRate = kFactor_.m_vecValues[FO_VALUE];
		
		pGameUnit_->CreateAndInsertDoubleAttackByBuff( kBuffFactor_.m_BuffIdentity, m_fRate );
	}
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffDoubleAttackBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseDoubleAttackByBuff( pBuffTemplet_->GetBuffIdentity() );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeAttackSphereScaleBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )
		{
			BOOST_FOREACH( const AttackSphereNameAndScale& attackSphereNameAndScale, m_vecAttackSphereNameAndScale )
			{
				float fScale = pGameUnit_->GetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName );
				fScale += attackSphereNameAndScale.m_fScale;

				if ( fScale < 0.0f )	/// fScale�� ���̳ʽ��� ������ �ƴ϶�� �Ǵ��Ͽ�
					fScale = 1.0f;		/// 1.0 (�⺻��)���� ����

				pGameUnit_->SetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName, fScale );
			}
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeUnitScaleBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : ParsingBehavior
	@brief : ���ùڽ� ũ�⸦ ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeAttackSphereScaleBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	int iIndex = 1;
	while ( luaManager_.BeginTable( iIndex ) )
	{
		AttackSphereNameAndScale attackSphereNameAndScale;

		LUA_GET_VALUE( luaManager_, "ATTACK_SPHERE_NAME", attackSphereNameAndScale.m_wstrAttackSphereName, L"" );
		LUA_GET_VALUE( luaManager_, "SCALE", attackSphereNameAndScale.m_fScale, 1.0f );

		m_vecAttackSphereNameAndScale.push_back( attackSphereNameAndScale );
		++iIndex;

        luaManager_.EndTable();
	}

	return true;
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeAttackSphereScaleBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackSphereScaleBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeAttackSphereScaleBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeAttackSphereScaleBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// ���۵� �� ������ ���� ��... // ����ö // 2013-07-18
	if ( !DidStart() )
	{
		BOOST_FOREACH( const AttackSphereNameAndScale& attackSphereNameAndScale, m_vecAttackSphereNameAndScale )
		{
			float fScale = pGameUnit_->GetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName );
			fScale += attackSphereNameAndScale.m_fScale;

			if ( fScale < 0.0f )	/// fScale�� ���̳ʽ��� ������ �ƴ϶�� �Ǵ��Ͽ�
				fScale = 1.0f;		/// 1.0 (�⺻��)���� ����

			pGameUnit_->SetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName, fScale );
		}
	}
	
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeAttackSphereScaleBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	BOOST_FOREACH( const AttackSphereNameAndScale& attackSphereNameAndScale, m_vecAttackSphereNameAndScale )
	{
		float fScale = pGameUnit_->GetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName );
		fScale -= attackSphereNameAndScale.m_fScale;

		if ( fScale < 0.0f )	/// fScale�� ���̳ʽ��� ������ �ƴ϶�� �Ǵ��Ͽ�
			fScale = 1.0f;		/// 1.0 (�⺻��)���� ����

		pGameUnit_->SetSphereAttackBoxScale( attackSphereNameAndScale.m_wstrAttackSphereName, fScale );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffAddSkillLevelBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( !DidStart() )	/// �ϴ� ��ø�� ��� ����
		{
			m_iAddSkillLevel = static_cast<int>( (*pBehaviorFactor)[FO_SKILL_LEVEL] );
			pGameUnit_->CreateAndInsertAddSkillLevelByBuff( pBuffTemplet_->GetBuffIdentity(), m_iAddSkillLevel );
			pGameUnit_->UpdatePassiveAndActiveSkillState();
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffAddSkillLevelBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffAddSkillLevelBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffAddSkillLevelBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffAddSkillLevelBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_iAddSkillLevel ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffAddSkillLevelBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_iAddSkillLevel = static_cast<int>( kFactor_.m_vecValues[FO_SKILL_LEVEL] );
	pGameUnit_->CreateAndInsertAddSkillLevelByBuff( kBuffFactor_.m_BuffIdentity, m_iAddSkillLevel );
	pGameUnit_->UpdatePassiveAndActiveSkillState();
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAddSkillLevelBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseAddSkillLevelByBuff( pBuffTemplet_->GetBuffIdentity() );
	pGameUnit_->UpdatePassiveAndActiveSkillState();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ

/** @function : ParsingBehavior
	@brief : Ÿ�Կ� ���� ���ݷ� ��ġ�� ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffReflectMagicBehaviorTemplet::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffReflectMagicBehaviorTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		if ( STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pBehaviorFactor)[FO_CHANGE_TYPE] ) == m_eChangeType )
		{
			if ( BCT_PERCENT == m_eChangeType )	/// ������ ����
			{
				if ( DidStart() )	/// ������ �۵� ���� ���̾����� ������ ȿ�� ����
					pGameUnit_->EraseReflectMagicByBuff( pBuffTemplet_->GetBuffIdentity() );

				/// ȿ�� ����
				m_fResultValue = (*pBehaviorFactor)[FO_VALUE];

				pGameUnit_->CreateAndInsertReflectMagicByBuff( pBuffTemplet_->GetBuffIdentity(), m_fResultValue );

				return true;
			}
			else
				return DISPLAY_ERROR( L"ONLY BCT_PERCENT" );
		}
		else
			return DISPLAY_ERROR( L"MISS MATCH BUFF_CHANGE_TYPE" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffChangeStatBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffReflectMagicBehaviorTemplet::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffReflectMagicBehaviorTemplet( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffReflectMagicBehaviorTemplet::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();

	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ �����ϴ� ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffReflectMagicBehaviorTemplet::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
	m_fResultValue = kFactor_.m_vecValues[FO_VALUE];

	pGameUnit_->CreateAndInsertReflectMagicByBuff( kBuffFactor_.m_BuffIdentity, m_fResultValue );
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffReflectMagicBehaviorTemplet::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseReflectMagicByBuff( pBuffTemplet_->GetBuffIdentity() );
}

#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
/** @function : ParsingBehavior
	@brief : Ÿ�Կ� ���� ���ݷ� ��ġ�� ���� ��Ű�� ���� �ൿ�� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffChangeConsumeMpRate::ParsingBehavior( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	return true;
}

/** @function : SetFactor
	@brief : BuffFactor�� ����� ������ Templet ����
	@param : �������(BuffFactor_), ������ ������ ���� ��������(pGameUnit_), ������ ������ �ִ� �Ǵ� ���ο� �������ø� ������(pBuffTemplet_)
*/
/*virtual*/ bool CX2BuffChangeConsumeMpRate::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffBehaviorFactor* pBehaviorFactor = NULL;

	/// ������ ������ �ִ� ���� ��� �� ���� Ŭ������ �ش� �ϴ� ������ ����
	if ( BuffFactor_.GetBehaviorFactor( GetType(), &pBehaviorFactor ) )
	{
		float fMPConsume = 0.f;
		m_uiSkillId = static_cast<UINT>( (*pBehaviorFactor)[FO_SKILL_ID] );
		
		if ( 0 > m_uiSkillId )
			return DISPLAY_ERROR( L"SKILL_ID doesn't exist" );

		if ( pGameUnit_->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			CX2GUUser* pUser = reinterpret_cast<CX2GUUser*>( pGameUnit_ );

			if ( NULL == pUser->GetUnit() )
				return false;

			const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SKILL_ID( m_uiSkillId ) ) );
						
			if ( DidStart() )
			{
				m_fResultValue = m_fResultValue + m_fInitMp * (*pBehaviorFactor)[FO_VALUE];
			}
			else
			{	/// ��ø, ��ü�� �ƴ϶��(ù���� �Ǵ� �ߺ�)
				m_fInitMp = pUser->GetActualMPConsume( CX2SkillTree::SKILL_ID( m_uiSkillId ), iSkillTempletLevel );
				m_fResultValue = m_fInitMp + m_fInitMp * (*pBehaviorFactor)[FO_VALUE];
			}
		}

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffCreateBuffFactortBehaviorTemplet GetBehaviorFactor didn't work" );
}

/** @function : GetClonePtr
	@return : CX2BuffBehaviorTempletPtr
*/
/*virtual*/ CX2BuffBehaviorTempletPtr CX2BuffChangeConsumeMpRate::GetClonePtr() const
{
	return CX2BuffBehaviorTempletPtr( new CX2BuffChangeConsumeMpRate( *this ) );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffChangeConsumeMpRate::GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const
{
	vecFactors_.push_back( KBuffBehaviorFactor( GetType() ) );
	KBuffBehaviorFactor& BehaviorFactor = vecFactors_.back();
	
	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_eChangeType ) );
	BehaviorFactor.m_vecValues.push_back( static_cast<float>( m_uiSkillId ) );
	BehaviorFactor.m_vecValues.push_back( m_fResultValue );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ٸ� ���ֿ��� ������ ���� �Ǵ� ������� ������ �� �ִ� �ൿ ���ø��� ����
	@param : ������ ���� ���� �ൿ���(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffChangeConsumeMpRate::SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_eChangeType	= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, kFactor_.m_vecValues[FO_CHANGE_TYPE] );
	m_uiSkillId		= static_cast<UINT>( kFactor_[FO_SKILL_ID] );
	m_fResultValue	= kFactor_.m_vecValues[FO_VALUE];
}

/** @function : DoFinish
	@brief : ������ ������� �� �����ؾ��ϴ� �ൿ���� ó��
	@param : @param : �ش� ������ �ɷȴ� ��������(pGameUnit_), �������ø�(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffChangeConsumeMpRate::DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseTemporaryBuffFactor( pBuffTemplet_->GetBuffIdentity() );
}
#endif //FIX_SKILL_BALANCE_AISHA_LENA