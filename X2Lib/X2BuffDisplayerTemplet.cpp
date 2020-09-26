/**@file : X2BuffBehaviorTemplet.cpp
   @breif : ������ �ൿ�� �����ϴ� Templet ����
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffDisplayerTemplet.h"

#define CASE_DISPLAYER_TEMPLET( class_, type_ ) \
case type_: \
	{ \
	CX2BuffDisplayerTempletPtr ptrDisplayerTemplet = class_::CreateBuffDisplayerTempletPtr(); \
	if ( NULL != ptrDisplayerTemplet && ptrDisplayerTemplet->ParsingDisplayerTemplateMethod( luaManager_, STRINGIZEA2(type_) ) ) \
		{ \
		ptrDisplayerTemplet->SetType( type_ ); \
		vecBuffDisplayerTempletPtr_.push_back( ptrDisplayerTemplet ); \
		} \
		else \
		return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

/** @function : ParsingCombinationDisplayer
	@brief : ������ ǥ���� � ������ �����Ǿ� �ִ��� �˾Ƴ��� ������ Ÿ�Ժ��� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� BehaviorTempletPtr�� ���� vector(vecBuffBehaviorTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*static*/ bool CX2BuffDisplayerTemplet::ParsingCombinationDisplayer( KLuaManager& luaManager_, OUT vector<CX2BuffDisplayerTempletPtr>& vecBuffDisplayerTempletPtr_ )
{

	vector<UINT> vecDisplayerType;

#ifdef EXCEPTION_BUFF_FACTOR
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR
#ifdef EXCEPTION_BUFF_FACTOR_VER2
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR_VER2
	
	/// Displayer�� � ǥ����ҵ��� �������� �̷�� ������ ����
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecDisplayerType ) )
	{
		vector<UINT>::iterator vItr = vecDisplayerType.begin();
		while ( vecDisplayerType.end() != vItr )		/// ���տ� ���Ե� �ൿ��ҵ��� �Ľ�
		{

#ifdef EXCEPTION_BUFF_FACTOR
			if(static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BDT_EFFECT_SET_CREATE_GAP && 
				static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BDT_EFFECT_SET_NORMAL_ATTACK)
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR
#ifdef EXCEPTION_BUFF_FACTOR_VER2
			if( static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) != BDT_EFFECT_SET_ONLY_MY_CLASS )
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR_VER2
			
			switch ( static_cast<BUFF_DISPLAYER_TYPE>( *vItr ) )
			{
			CASE_DISPLAYER_TEMPLET( CX2BuffRenderParamDisplayerTemplet, BDT_RENDER_PARAM )
			CASE_DISPLAYER_TEMPLET( CX2BuffRenderParamByUnitTypeDisplayerTemplet, BDT_RENDER_PARAM_BY_UNIT_TYPE )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetDisplayerTemplet, BDT_EFFECT_SET )
			CASE_DISPLAYER_TEMPLET( CX2BuffUnitSlashTraceDisplayerTemplet, BDT_UNIT_SLASH_TRACE )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetImpactPointDisplayerTemplet, BDT_EFFECT_SET_MELEE_IMPACT_POINT )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetImpactPointDisplayerTemplet, BDT_EFFECT_SET_HITTED )
			CASE_DISPLAYER_TEMPLET( CX2BuffWeaponParticleDisplayerTemplet, BDT_WEAPON_PARTICLE )
			CASE_DISPLAYER_TEMPLET( CX2BuffAfterImageDisplayerTemplet, BDT_AFTER_IMAGE )
			CASE_DISPLAYER_TEMPLET( CX2BuffSoundDisplayTemplet, BDT_SOUND )
#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE 
			CASE_DISPLAYER_TEMPLET( CX2BuffWeaponAfterImageDisplayerTemplet, BDT_WEAPON_AFTER_IMAGE )
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

//#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-10	// �ؿ��� �ּ� ó��
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetCreateGapDisplayerTemplet, BDT_EFFECT_SET_CREATE_GAP )
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetImpactPointDisplayerTemplet, BDT_EFFECT_SET_NORMAL_ATTACK )
//#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
			CASE_DISPLAYER_TEMPLET( CX2BuffEffectSetOnlyMyClassDisplayerTemplet, BDT_EFFECT_SET_ONLY_MY_CLASS )
#endif // HAMEL_SECRET_DUNGEON

#ifdef EXCEPTION_BUFF_FACTOR
			case BDT_EFFECT_SET_CREATE_GAP:
			case BDT_EFFECT_SET_NORMAL_ATTACK:
				++vItr;
				continue;
#endif // EXCEPTION_BUFF_FACTOR
			default:
				return DISPLAY_ERROR( L"Unknown Displayer Templet Type" );	/// false ����
				break;
			}

			++vItr;
		}
	}

	if ( vecBuffDisplayerTempletPtr_.empty() 
#ifdef EXCEPTION_BUFF_FACTOR
			&& !bExceptionCase 
#endif // EXCEPTION_BUFF_FACTOR
#ifdef EXCEPTION_BUFF_FACTOR_VER2
			&& !bExceptionCase
#endif EXCEPTION_BUFF_FACTOR_VER2
			)
	{
		return DISPLAY_ERROR( L"DISPLAYER Templet is empty" );
	}
	else
		return true;
}

/** @function : ParsingDisplayerTemplateMethod
	@brief : �� �Ļ�Ŭ�������� ������ ParsingDisplayer�� �Ȱ��� ���� ó�� ���̿��� ȣ���ϱ� ���� ���ø��޼ҵ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� DisplayerTempletPtr�� ���� vector(vecBuffDisplayerTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
bool CX2BuffDisplayerTemplet::ParsingDisplayerTemplateMethod( KLuaManager& luaManager_, const char* pszTableNameUTF8_ )
{
	if ( luaManager_.BeginTable( pszTableNameUTF8_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		return ParsingDisplayer( luaManager_ );
	}
	else
		return DISPLAY_ERROR( pszTableNameUTF8_ );	
}

/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ RenderParam ���� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffRenderParamDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_ENUM( luaManager_, "RENDER_TYPE", m_stBuffRenderParam.m_eRenderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_REAL_COLOR );
	LUA_GET_VALUE_ENUM( luaManager_, "CARTOON_TEX_TYPE", m_stBuffRenderParam.m_eCartoonTexType, CKTDGXRenderer::CARTOON_TEX_TYPE, CKTDGXRenderer::CTT_NORMAL );
	LUA_GET_VALUE( luaManager_, "OUT_LINE_WIDE", m_stBuffRenderParam.m_fOutLineWide, 0.0f );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR_OUT_LINE", m_stBuffRenderParam.m_d3dxColorOutLine, D3DXCOLOR(1,1,1,1) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", m_stBuffRenderParam.m_d3dxColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_stBuffRenderParam.m_d3dxColorOutLine 
		= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR_OUT_LINE" );	
	m_stBuffRenderParam.m_d3dxColor	
		= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE( luaManager_, "ALPHA_BLEND", m_stBuffRenderParam.m_bAlphaBlend, false );
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	LUA_GET_VALUE( luaManager_, "FLICKER_COLOR_GAP", m_stBuffRenderParam.m_fFlickerColorGap, 0.f );
#endif //SERV_ADD_LUNATIC_PSYKER

	return true;
}

/** @function : StartDisplayer
	@brief : RenderParam ������ ���� �ϴ� �Լ�
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )	/// ��ø �� ���̶�� �������� �ʴ´�.
		pGameUnit_->CreateAndInsertRenderParamByBuff( pBuffTemplet_->GetBuffIdentity(), m_stBuffRenderParam );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffRenderParamDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffRenderParamDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ���������� ����� RenderParam ������ �����ϴ� �ռ�
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseRenderParamByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingDisplayer
	@brief : Ÿ�Ժ��� �ٸ� RenderParam�� �������ֿ� ������ ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffRenderParamByUnitTypeDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	int iTableIndex = 1;

	/// �� ���� Ÿ�Ժ��� RenderParam ������ �о����
	while ( luaManager_.BeginTable( iTableIndex ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

		StBuffRenderParamPtr ptrStBuffRenderParam( new StBuffRenderParam );
		if ( NULL != ptrStBuffRenderParam )
		{
			CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;
			LUA_GET_VALUE_ENUM( luaManager_, "UNIT_TYPE", eUnitType, CX2Unit::UNIT_TYPE, CX2Unit::UT_NONE );

			LUA_GET_VALUE_ENUM( luaManager_, "RENDER_TYPE", ptrStBuffRenderParam->m_eRenderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_REAL_COLOR );
			LUA_GET_VALUE_ENUM( luaManager_, "CARTOON_TEX_TYPE", ptrStBuffRenderParam->m_eCartoonTexType, CKTDGXRenderer::CARTOON_TEX_TYPE, CKTDGXRenderer::CTT_NORMAL );
			LUA_GET_VALUE( luaManager_, "OUT_LINE_WIDE", ptrStBuffRenderParam->m_fOutLineWide, 0.0f );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR_OUT_LINE", ptrStBuffRenderParam->m_d3dxColorOutLine, D3DXCOLOR(1,1,1,1) );
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", ptrStBuffRenderParam->m_d3dxColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
			ptrStBuffRenderParam->m_d3dxColorOutLine 
				= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR_OUT_LINE" );	
			ptrStBuffRenderParam->m_d3dxColor	
				= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

			LUA_GET_VALUE( luaManager_, "ALPHA_BLEND", ptrStBuffRenderParam->m_bAlphaBlend, false );

			pair<map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>::iterator, bool> pairReturn 
				= m_mapStBuffRenderParamPtr.insert( make_pair( eUnitType, ptrStBuffRenderParam ) );

			/// ���� Ű���� ������ �ִ� ���� �̹� �����ϸ� ����ó��
			if ( false == pairReturn.second )
				return DISPLAY_ERROR( L"UNIT TYPE already existed" );
		}	
		++iTableIndex;
	}		

	/// ���� RenderParam ���� ������ �ȵ�!
	if ( m_mapStBuffRenderParamPtr.end() == m_mapStBuffRenderParamPtr.find( CX2Unit::UT_NONE ) )
		return DISPLAY_ERROR( L"UT_NONE Type's RenderParam Must exist" );
	
	return true;
}

/** @function : StartDisplayer
	@brief : ������ Ÿ�Ժ� RenderParam�� �����ϴ� ǥ����� ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamByUnitTypeDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )	/// ��ø �� ���̶�� �������� �ʴ´�.
		pGameUnit_->CreateAndInsertRenderParamByBuffUnitType( pBuffTemplet_->GetBuffIdentity(), m_mapStBuffRenderParamPtr );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffRenderParamByUnitTypeDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffRenderParamByUnitTypeDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : �ش� ������ ���� RenderParam ���� ����
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffRenderParamByUnitTypeDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->EraseRenderParamByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ ����Ʈ�� ���� ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffEffectSetDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
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
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// ��ü �Ǵ� Ÿ���� ������ ��� �ش� Flag �� True �� ��, ����Ʈ ���� �ٽ� �ѷ��ش�.
	LUA_GET_VALUE( luaManager_, "REPLAY_EFFECT_SET_ACCUMULATION_LIMIT1", m_bIsReplayEffectSetAccumulationType1, false );
	++iIndex;
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	
	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� ����Ʈ���� Play��Ű�� �Լ�
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		const UINT uiAccumulationLimit = pBuffTemplet_->GetAccumulationLimit();
		if ( uiAccumulationLimit > 1 && m_vecWstrEffectSetName.size() > 1 )	/// ��ø ������ ���̰� ����Ʈ�� ��ø ���� �ִٸ�
		{
			/// ������ ����Ʈ ���� (���۷����� �����ؾ���)
			BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
			{
				if ( INVALID_EFFECTSET_HANDLE != hHandle )
					g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
			}

			const UINT uiIndexPlayEffectSet = pBuffTemplet_->GetAccumulationCountNow() - 1;
			if ( uiIndexPlayEffectSet < m_vecWstrEffectSetName.size() )
			{
				m_vecHandleEffectSet[uiIndexPlayEffectSet] =
					g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecWstrEffectSetName[uiIndexPlayEffectSet], pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );
			}

		}
		else
		{
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// ��ü �Ǵ� Ÿ���� ������ ��� �ش� Flag �� True �� ��, ����Ʈ ���� �ٽ� �ѷ��ش�.
			if ( ( !DidStart() || m_bIsReplayEffectSetAccumulationType1 == true ) && !m_vecWstrEffectSetName.empty() )
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			if ( !DidStart() && !m_vecWstrEffectSetName.empty() )
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			{
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
				if ( true == m_bIsReplayEffectSetAccumulationType1 )
				{
					if ( INVALID_EFFECTSET_HANDLE != m_vecHandleEffectSet[0] )
						g_pX2Game->GetEffectSet()->StopEffectSet( m_vecHandleEffectSet[0] );
				}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
				m_vecHandleEffectSet[0] =
					g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecWstrEffectSetName[0], pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );
			}
		}
	}
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ������ ���� ��� ���� EffectSet�� �����ϴ� �Լ�
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
		{
			if ( INVALID_EFFECTSET_HANDLE != hHandle )
				g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� ���ֿ� ���� Ʈ���̽��� ���۽�Ű�� �Լ�
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffUnitSlashTraceDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )	/// ��ø �� ���̶�� �������� �ʴ´�.
	{
		//pGameUnit_->AddUnitSlashData( m_eSlashTraceCondition );
		pGameUnit_->SetUnitSlashTraceConditionToDo( static_cast<CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION>( m_eSlashTraceCondition ) );
	}
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffUnitSlashTraceDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffUnitSlashTraceDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ������ ���� ��� ���� Trace�� �����ϴ� �Լ�
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffUnitSlashTraceDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
	{
		pGameUnit_->SetUnitSlashTraceConditionToDo( CX2UnitSlashTraceManager::STC_NONE );
	}
}

/** @function : ParsingDisplayer
	@brief : ����� ��������� ���� �Ľ�
	@param : ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffUnitSlashTraceDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_ENUM( luaManager_, "SLASH_TRACE_CONDITION", m_eSlashTraceCondition, CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION, CX2UnitSlashTraceManager::STC_NONE );
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� Ÿ������ ��µ� ����Ʈ���� ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetImpactPointDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )
		StartDisplayerByType( pGameUnit_, pBuffTemplet_ );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetImpactPointDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetImpactPointDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ������ ���� ���õ� Ÿ���� ����Ʈ���� ����
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetImpactPointDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		DoFinishByType( pGameUnit_, pBuffTemplet_ );
}

/** @function : ParsingDisplayer
	@brief : ����� Ÿ���� ����Ʈ�¸� �Ľ�
	@param : ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffEffectSetImpactPointDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	if ( luaManager_.GetValue( 1, m_wstrEffectSetName ) )
	{
		luaManager_.GetValue( 2, m_wstrSoundName );		/// ����� ���� ���� ����

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-09
		luaManager_.GetValue( 3, m_fCreateGap );		///  ���� ���� ���� ���� ���� ����
#endif // SERV_NEW_DEFENCE_DUNGEON

		return true;
	}
	else
		return false;
}

/** @function : StartDisplayerByType
	@brief : Ÿ�Կ� ���� ����Ʈ���� ���� ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
void CX2BuffEffectSetImpactPointDisplayerTemplet::StartDisplayerByType( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const WCHAR* pszSoundName	= ( m_wstrSoundName.empty() ? L"" : m_wstrSoundName.c_str() );

	switch ( GetType() )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-09		���� ���� �߰�
		case BDT_EFFECT_SET_MELEE_IMPACT_POINT:		/// ���� ���� Ÿ�ݽ� ����Ʈ �� ����
			pGameUnit_->CreateAndInsertEffectSetImpactPointByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName, m_fCreateGap );
			break;

		case BDT_EFFECT_SET_HITTED:					/// �ǰݽ� ����Ʈ �� ����
			pGameUnit_->CreateAndInsertEffectSetHittedByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName, m_fCreateGap );
			break;

		case BDT_EFFECT_SET_NORMAL_ATTACK:			/// �Ϲ� ���� Ÿ�ݽ� ����Ʈ �� ����
			pGameUnit_->CreateAndInsertEffectSetNormalAttackByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName, m_fCreateGap );
			break;
#else // SERV_NEW_DEFENCE_DUNGEON
		case BDT_EFFECT_SET_MELEE_IMPACT_POINT:
			pGameUnit_->CreateAndInsertEffectSetImpactPointByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName );
			break;

		case BDT_EFFECT_SET_HITTED:
			pGameUnit_->CreateAndInsertEffectSetHittedByBuff( pBuffTemplet_->GetBuffIdentity(), m_wstrEffectSetName.c_str(), pszSoundName );
			break;
#endif // SERV_NEW_DEFENCE_DUNGEON

	default:
		break;
	}
}

/** @function : DoFinishByType
	@brief : Ÿ�Կ� ���� ����Ʈ���� ���� ����
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
void CX2BuffEffectSetImpactPointDisplayerTemplet::DoFinishByType( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	switch ( GetType() )
	{
	case BDT_EFFECT_SET_MELEE_IMPACT_POINT:	/// ���� ���� Ÿ�ݽ� ����Ʈ ���� ���� ����
		pGameUnit_->EraseEffectSetImpactPointByBuff( pBuffTemplet_->GetBuffIdentity() );
		break;

	case BDT_EFFECT_SET_HITTED:				/// �ǰݽý� ����Ʈ ���� ���� ����
		pGameUnit_->EraseEffectSetHittedByBuff( pBuffTemplet_->GetBuffIdentity() );
		break;

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-14
	case BDT_EFFECT_SET_NORMAL_ATTACK:		/// �Ϲ� ���� Ÿ�ݽ� ����Ʈ ���� ���� ����
		pGameUnit_->EraseEffectSetNormalAttackByBuff( pBuffTemplet_->GetBuffIdentity() );
		break;
#endif // SERV_NEW_DEFENCE_DUNGEON

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� Weapon ��ƼŬ ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && !DidStart() )
		StartDisplayerByBuffTempletID( pGameUnit_, pBuffTemplet_ );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffWeaponParticleDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffWeaponParticleDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ������ ���� ��� ���� Weapon ��ƼŬ�� ����
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && DidStart() )
	{
		CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
		BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequenceHandle& hHandle, m_vecHandleParticle )
		{
			pMajorParticleSystem->DestroyInstanceHandle( hHandle );
		}
	}
}

/** @function : OnFrameMove
	@brief : Displayer�� OnFrameMove
	@param : ������ �ɸ� ��������(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    switch( m_eFrameMoveType )
    {
    case FMT_SWORD_FIRE:
        OnFrameMoveSwordFire( pGameUnit_ );
        break;
    case FMT_ENCHANT:
        OnFrameMoveSwordEnchant( pGameUnit_ );
        break;
    default:
        CX2BuffDisplayerTemplet::OnFrameMove( pGameUnit_, fElapsedTime_ );
    }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateOnFrameMove( pGameUnit_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}

/** @function : StartDisplayerByBuffTempletID
	@brief : ������ ǥ����ҷ� �������ø����̵𺰷� Weapon�� ���� ��ƼŬ�� ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
void CX2BuffWeaponParticleDisplayerTemplet::StartDisplayerByBuffTempletID( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	switch ( pBuffTemplet_->GetBuffTempletID() )
	{
	case BTI_SI_SA_EMK_SWORD_FIRE:
		{
			CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
			const UINT uiSize = m_vecWstrParticleName.size();
			for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
			{
				m_vecHandleParticle[uiIndex]
					= pMajorParticleSystem->CreateSequenceHandle( pGameUnit_,  m_vecWstrParticleName[uiIndex].c_str(), 0,0,0 );
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_eFrameMoveType = FMT_SWORD_FIRE;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_delegateOnFrameMove = DelegateOnFrameMoveByType::from_method<CX2BuffWeaponParticleDisplayerTemplet, &CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordFire>( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;

	case BTI_BUFF_SWORD_ENCHANT_ICE:
	case BTI_BUFF_SWORD_ENCHANT_FIRE:
	case BTI_BUFF_SWORD_ENCHANT_POISON:
		{
			CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
			const UINT uiSize = m_vecWstrParticleName.size();
			for ( UINT uiIndex = 0; uiIndex < uiSize; ++uiIndex )
			{
				m_vecHandleParticle[uiIndex]
				= pMajorParticleSystem->CreateSequenceHandle( pGameUnit_,  m_vecWstrParticleName[uiIndex].c_str(), 0,0,0 );
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_eFrameMoveType = FMT_ENCHANT;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_delegateOnFrameMove = DelegateOnFrameMoveByType::from_method<CX2BuffWeaponParticleDisplayerTemplet, &CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordEnchant>( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;
	default:
		{
			// ������ // CX2BuffWeaponParticleDisplayerTemplet �� ������ ����ó���� ���־�� �մϴ�.
			DISPLAY_ERROR( L"Empty WeaponDisplayer OnFrameMove" );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_eFrameMoveType = FMT_DEFAULT;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_delegateOnFrameMove = DelegateOnFrameMoveByType::from_method<CX2BuffDisplayerTemplet, &CX2BuffDisplayerTemplet::OnFrameMove>( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
		break;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    OnFrameMove( pGameUnit_, 0.f );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_delegateOnFrameMove( pGameUnit_ );	/// ��ġ�� �������� �ѹ� �̸� ����
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}

/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ Weapon ��ƼŬ ������ ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffWeaponParticleDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	int iIndex = 1;

	wstring wstrParticleName;
	while( luaManager_.GetValue( iIndex, wstrParticleName ) )
	{
		m_vecWstrParticleName.push_back( wstrParticleName );
		m_vecHandleParticle.push_back( INVALID_PARTICLE_SEQUENCE_HANDLE );
		wstrParticleName.resize(0);
		++iIndex;
	}

	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : OnFrameMoveSwordFire
	@brief : ������ �ҵ����̾��� ��� ����� OnFrameMove
	@param : ������ �ɸ� ��������(pGameUnit_)
*/
void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordFire( CX2GameUnit* pGameUnit_ )
{
	const UINT NUM_OF_SWORD_FIRE_PARTICLE = 3;

	ASSERT( m_vecHandleParticle.size() == NUM_OF_SWORD_FIRE_PARTICLE );
	if ( m_vecHandleParticle.size() == NUM_OF_SWORD_FIRE_PARTICLE )	/// �ҵ����̾�� ��ƼŬ�� 3��
	{
		D3DXVECTOR3 vParticlePos[NUM_OF_SWORD_FIRE_PARTICLE] = {
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_START0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_END0", 0 ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f )
		};

		vParticlePos[2] = ( vParticlePos[0] - vParticlePos[1] ) * 0.5f + vParticlePos[1];

		CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
		for ( UINT uiIndex = 0; uiIndex < NUM_OF_SWORD_FIRE_PARTICLE; ++uiIndex )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq 
				= pMajorParticleSystem->GetInstanceSequence( m_vecHandleParticle[uiIndex] );
			if ( NULL != pSeq )
				pSeq->SetPosition( vParticlePos[uiIndex] );
		}
	}
}
/** @function : OnFrameMoveSwordEnchant
	@brief : ������ �ҵ���þƮ�� ��� ����� OnFrameMove
	@param : ������ �ɸ� ��������(pGameUnit_)
*/
void CX2BuffWeaponParticleDisplayerTemplet::OnFrameMoveSwordEnchant( CX2GameUnit* pGameUnit_ )
{
	const UINT NUM_OF_SWORD_ENCHANT_PARTICLE = 8;

	ASSERT( m_vecHandleParticle.size() == NUM_OF_SWORD_ENCHANT_PARTICLE );
	if ( m_vecHandleParticle.size() == NUM_OF_SWORD_ENCHANT_PARTICLE )	/// �ҵ���þƮ�� ��ƼŬ�� 7��
	{
		D3DXVECTOR3 vParticlePos[NUM_OF_SWORD_ENCHANT_PARTICLE] = {
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_START0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_START0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_END0", 0 ),
			pGameUnit_->GetWeaponBonePos( 0, L"TRACE_END0", 0 ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f )
		};

		vParticlePos[4] = vParticlePos[5] = vParticlePos[6] = vParticlePos[7] = ( vParticlePos[0] - vParticlePos[2] ) * 0.5f + vParticlePos[2];

		CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();
		for ( UINT uiIndex = 0; uiIndex < NUM_OF_SWORD_ENCHANT_PARTICLE; ++uiIndex )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq 
				= pMajorParticleSystem->GetInstanceSequence( m_vecHandleParticle[uiIndex] );
			if ( NULL != pSeq )
				pSeq->SetPosition( vParticlePos[uiIndex] );
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� AfterImage ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAfterImageDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertAfterImageByBuff( pBuffTemplet_->GetBuffIdentity(), m_d3dxColor );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffAfterImageDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffAfterImageDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ���������� ����� AfterImage ����
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffAfterImageDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseAfterImageByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ AfterImage ���� �Ľ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffAfterImageDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", m_d3dxColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_d3dxColor	= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	return true;
}

#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� ������ AfterImage ����
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponAfterImageDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( !DidStart() )
		pGameUnit_->CreateAndInsertWeaponAfterImageByBuff( pBuffTemplet_->GetBuffIdentity(), m_d3dxColor );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffWeaponAfterImageDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffWeaponAfterImageDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ���������� ����� ������ AfterImage ����
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffWeaponAfterImageDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( DidStart() )
		pGameUnit_->EraseWeaponAfterImageByBuff( pBuffTemplet_->GetBuffIdentity() );
}

/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ ������ AfterImage ���� �Ľ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffWeaponAfterImageDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager_, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager_, "COLOR", m_d3dxColor, D3DXCOLOR(1,1,1,1) );	
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_d3dxColor	= lua_tinker::get<D3DXCOLOR>( luaManager_.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	return true;
}
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� ���� ���
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffSoundDisplayTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	pGameUnit_->PlaySound( m_wstrSoundName.c_str() );
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffSoundDisplayTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffSoundDisplayTemplet( *this ) );
}

/** @function : ParsingDisplayer
	@brief : ������ �ɷ��� �� ����� ����� �Ľ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffSoundDisplayTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	if ( luaManager_.GetValue( 1, m_wstrSoundName ) )
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ ����Ʈ�� ��� ���� ������ ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffEffectSetCreateGapDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	int iIndex = 1;
	
	wstring wstrEffectSetName;
	while( luaManager_.GetValue( iIndex, wstrEffectSetName ) )
	{
		int iCreateGap;

		if ( true == luaManager_.GetValue( iIndex + 1, iCreateGap ) )
		{
			ASSERT( 0 != iCreateGap );
			m_vecPairEffectSetNameAndGap.push_back( std::make_pair( wstrEffectSetName, static_cast<float>( iCreateGap ) ) );
		}
		else	/// ���� ��Ÿ���� �����Ǿ���!
		{
			ASSERT( !L"BDT_EFFECT_SET_CREATE_GAP : Create EffectSet Gap is Nothing !" );
			return false;
		}

		iIndex += 2;
	}
	
	if ( 1 != iIndex )	
		return true;
	else
		return false;
}

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� ����Ʈ���� Play��Ű�� �Լ�
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		typedef std::pair< wstring, float > PairEffectSetNameAndGap;

		BOOST_FOREACH( const PairEffectSetNameAndGap& PairValue, m_vecPairEffectSetNameAndGap )
		{
			m_vecHandleEffectSet.push_back( g_pX2Game->GetEffectSet()->PlayEffectSet( PairValue.first, pGameUnit_,
				NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() ) );

			CKTDXCheckElapsedTime CreateEffectSetGap = 0.f;
			CreateEffectSetGap.SetTargetTime( PairValue.second );
			m_vecCheckElapsedTimeForEffectSet.push_back( std::make_pair( PairValue.first, CreateEffectSetGap ) );
		}
	}
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetCreateGapDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetCreateGapDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ������ ���� ��� ���� EffectSet�� �����ϴ� �Լ�
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		BOOST_FOREACH( CX2EffectSet::Handle& hHandle, m_vecHandleEffectSet )
		{
			if ( INVALID_EFFECTSET_HANDLE != hHandle )
				g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
		}
	}
}

/** @function : OnFrameMove
	@brief : Displayer�� OnFrameMove
	@param : ������ �ɸ� ��������(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffEffectSetCreateGapDisplayerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( NULL != g_pX2Game && CX2Game::GS_PLAY == g_pX2Game->GetGameState() )
	{
		typedef std::pair< wstring, CKTDXCheckElapsedTime > PairElapsedTimeForEffectSet;

		BOOST_FOREACH( PairElapsedTimeForEffectSet& PairValue, m_vecCheckElapsedTimeForEffectSet )
		{
			CKTDXCheckElapsedTime& CreateEffectSetGap = PairValue.second;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            CreateEffectSetGap.OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			CreateEffectSetGap.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

			if ( true == CreateEffectSetGap.CheckAndResetElapsedTime() )
			{
				m_vecHandleEffectSet.push_back( g_pX2Game->GetEffectSet()->PlayEffectSet( PairValue.first, pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() ) );
			}
		}
	}
}

#endif // SERV_NEW_DEFENCE_DUNGEON

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ

/** @function : ParsingDisplayer
	@brief : �������ֿ� ������ ����Ʈ�� ���� ���ø��� ��Ʒ� ���� �о���̴� �Լ�
	@param : �о���̷��� ��ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*virtual*/ bool CX2BuffEffectSetOnlyMyClassDisplayerTemplet::ParsingDisplayer( KLuaManager& luaManager_ )
{
	const int iMaxUnitNum = static_cast<int>( CX2Unit::UT_END ) - 1;		/// �ִ� ���� ��

	/// ���ּ� ��ŭ ��ȸ
	for ( int i = 0; i < iMaxUnitNum; ++i )
	{
		wstring wstrEffectSetName = L"";	/// ����Ʈ�� �̸�

		/// ���� Ÿ�� �ε����� ���� ����Ʈ�� �̸� �Ľ�
		switch ( i )
		{
			case 0: LUA_GET_VALUE( luaManager_, "ELSWORD", wstrEffectSetName, L"" );break;
			case 1: LUA_GET_VALUE( luaManager_, "AISHA",	wstrEffectSetName, L"" );break;
			case 2: LUA_GET_VALUE( luaManager_, "RENA",	wstrEffectSetName, L"" );break;
			case 3: LUA_GET_VALUE( luaManager_, "RAVEN",	wstrEffectSetName, L"" );break;
			case 4: LUA_GET_VALUE( luaManager_, "EVE",		wstrEffectSetName, L"" );break;
			case 5: LUA_GET_VALUE( luaManager_, "CHUNG",	wstrEffectSetName, L"" );break;
			case 6: LUA_GET_VALUE( luaManager_, "ARA",		wstrEffectSetName, L"" );break;
			case 7: LUA_GET_VALUE( luaManager_, "ELESIS",	wstrEffectSetName, L"" );break;
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
			case 8: LUA_GET_VALUE( luaManager_, "NEW_CHARACTER",	wstrEffectSetName, L"" );break;
#endif //SERV_9TH_NEW_CHARACTER
			default: DISPLAY_ERROR( L"DISPLAYER EFFECTSET ONLY MY CLASS - UNKNOWN CLASS" ) break;
		}

		/// ����
		m_vecEffectSetName.push_back( wstrEffectSetName );
	}

	/// ������ ���ڼ��� �ִ� ���� ���� �ƴ϶��, ������ ����
	if ( static_cast<int>( m_vecEffectSetName.size() ) != iMaxUnitNum )
	{
		DISPLAY_ERROR( L"DISPLAYER EFFECTSET ONLY MY CLASS - PARSING ERROR" );

		return false;
	}

	return true;
}

/** @function : StartDisplayer
	@brief : ������ ǥ����ҷ� ����Ʈ���� Play��Ű�� �Լ�
	@param : ǥ���� ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetOnlyMyClassDisplayerTemplet::StartDisplayer( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game && 
        NULL != pGameUnit_ &&
		 CX2GameUnit::GUT_USER == pGameUnit_->GetGameUnitType() && 
		 NULL != static_cast<CX2GUUser*>(pGameUnit_)->GetUnit() )
	{
		if ( !DidStart() )	/// ó�� �ɷ��� ��
		{
			/// ������ ����Ʈ ���� (���۷����� �����ؾ���)
			if ( INVALID_EFFECTSET_HANDLE != m_hEffectSetHandle )
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectSetHandle );

			/// ������ ����� ������ Ÿ��
			int iUnitTypeIndex = static_cast<int>( static_cast<CX2GUUser*>(pGameUnit_)->GetUnit()->GetType() ) - 1;

			/// �����̳� �� ������ �ε����� ��, ����Ʈ�� ����
			if ( iUnitTypeIndex < static_cast<int>( m_vecEffectSetName.size() ) )
			{
				m_hEffectSetHandle =
					g_pX2Game->GetEffectSet()->PlayEffectSet( m_vecEffectSetName[iUnitTypeIndex], pGameUnit_,
					NULL, ( 0 < pGameUnit_->GetRemainHyperModeTime() ), pGameUnit_->GetPowerRate() );

				/// ����Ʈ�� ���� ���� �߾��!!!
				if ( INVALID_EFFECTSET_HANDLE == m_hEffectSetHandle )
					DISPLAY_ERROR( L"DISPLAYER EFFECTSET ONLY MY CLASS - UNKNOWN EFFECTSET NAME" );
			}

		}
	}
}

/** @function : GetClonePtr
	@brief : ���ø��� Ŭ�� ����Ʈ �����͸� ��ȯ
	@return : Ŭ�� ����Ʈ������(CX2BuffDisplayerTempletPtr) 
*/
/*virtual*/ CX2BuffDisplayerTempletPtr CX2BuffEffectSetOnlyMyClassDisplayerTemplet::GetClonePtr() const
{
	return CX2BuffDisplayerTempletPtr( new CX2BuffEffectSetOnlyMyClassDisplayerTemplet( *this ) );
}

/** @function : DoFinish
	@brief : ������ ���� ��� ���� EffectSet�� �����ϴ� �Լ�
	@param : ǥ�� ���Ḧ ������ ����(pGameUnit_), ���� ����(pBuffTemplet_)
*/
/*virtual*/ void CX2BuffEffectSetOnlyMyClassDisplayerTemplet::DoFinish( CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	if ( NULL != g_pX2Game )
	{
		if ( INVALID_EFFECTSET_HANDLE != m_hEffectSetHandle )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectSetHandle );
	}
}

#endif // HAMEL_SECRET_DUNGEON