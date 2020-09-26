/** @file : X2BuffFinalizerTemplet.cpp
    @breif : ������ ���� ������ �����ϴ� Templet ����
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffFinalizerTemplet.h"

#define CASE_FINALIZER_TEMPLET( class_, type_ ) \
case type_: \
	{ \
	CX2BuffFinalizerTempletPtr ptrFinalizerTemplet = class_::CreateBuffFinalizerTempletPtr(); \
	if ( NULL != ptrFinalizerTemplet && ptrFinalizerTemplet->ParsingFinalizerTemplateMethod( luaManager_, STRINGIZEA2(type_) ) ) \
		{ \
		ptrFinalizerTemplet->SetType( type_ ); \
		vecBuffFinalizerTempletPtr_.push_back( ptrFinalizerTemplet ); \
		} \
		else \
		return DISPLAY_ERROR( STRINGIZE2(type_) ); \
	} break; \

/** @function : ParsingCombinationFinalizer
	@brief : ������ ���������� ������� �����Ǿ� �ִ��� �˾Ƴ���, ������ Ÿ�Ժ��� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� FinalizerTempletPtr�� ���� vector(vecBuffFinalizerTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
/*static*/ bool CX2BuffFinalizerTemplet::ParsingCombinationFinalizer( KLuaManager& luaManager_, OUT vector<CX2BuffFinalizerTempletPtr>& vecBuffFinalizerTempletPtr_ )
{
	vector<UINT> vecFinalizerType;

#ifdef EXCEPTION_BUFF_FACTOR
	bool bExceptionCase = true;
#endif // EXCEPTION_BUFF_FACTOR
	
	/// Finalizer�� � �����ҵ��� �������� �̷�� ������ ����
	if ( CX2BuffTempletManager::GetCombination( luaManager_, vecFinalizerType ) )
	{
		vector<UINT>::iterator vItr = vecFinalizerType.begin();
		while ( vecFinalizerType.end() != vItr )	/// ���տ� ���Ե� �����ҵ��� �Ľ�
		{
#ifdef EXCEPTION_BUFF_FACTOR
			if(static_cast<BUFF_FINALIZER_TYPE>( *vItr ) != BFT_RIDING_ON_OR_NOT)
				bExceptionCase = false;
#endif // EXCEPTION_BUFF_FACTOR

			switch ( static_cast<BUFF_FINALIZER_TYPE>( *vItr ) )
			{
			CASE_FINALIZER_TEMPLET( CX2BuffTimeFinalizerTemplet, BFT_TIME )
			CASE_FINALIZER_TEMPLET( CX2BuffHitCountFinalizerTemplet, BFT_HIT_COUNT )
			CASE_FINALIZER_TEMPLET( CX2BuffHittedCountFinalizerTemplet, BFT_HITTED_COUNT )
			CASE_FINALIZER_TEMPLET( CX2BuffStateChangeFinalizerTemplet, BFT_STATE_CHANGE )
			CASE_FINALIZER_TEMPLET( CX2BuffDamageValueFinalizerTemplet, BFT_DAMAGE_VALUE )
			CASE_FINALIZER_TEMPLET( CX2BuffPassHpFinalizerTemplet, BFT_PASS_HP )
			CASE_FINALIZER_TEMPLET( CX2BuffPassMpFinalizerTemplet, BFT_PASS_MP )
			CASE_FINALIZER_TEMPLET( CX2BuffImmediateFinalizerTemplet, BFT_IMMEDIATE )
			CASE_FINALIZER_TEMPLET( CX2BuffDungeonStageChangeFinalizerTemplet, BFT_DUNGEON_STAGE_CHANGE )
#ifdef ADD_LIVE_CREATOR_FINALIZER
			CASE_FINALIZER_TEMPLET( CX2BuffLiveCreatorFinalizerTemplet, BFT_LIVE_CREATOR )
#endif // ADD_LIVE_CREATOR_FINALIZER
#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
			CASE_FINALIZER_TEMPLET( CX2BuffFinishHyperFinalizerTemplet, BFT_FINISH_HYPER )
#endif // ADD_BUFF_FINALIZER_FINISH_HYPER
#ifdef RIDING_SYSTEM
			CASE_FINALIZER_TEMPLET( CX2BuffFinishRidingPetOnOrNotFinalizerTemplet, BFT_RIDING_ON_OR_NOT )
#endif	// RIDING_SYSTEM
			CASE_FINALIZER_TEMPLET( CX2BuffX2StateChangeFinalizerTemplet, BFT_X2STATE_CHANGE )

#ifdef EXCEPTION_BUFF_FACTOR
			case BFT_RIDING_ON_OR_NOT:
				++vItr;
				continue;
#endif // EXCEPTION_BUFF_FACTOR

			default:
				return DISPLAY_ERROR( L"Unknown Finalizer Type" );	/// false ����
				break;
			}

			++vItr;
		}
	}

	if ( vecBuffFinalizerTempletPtr_.empty()
#ifdef EXCEPTION_BUFF_FACTOR
		&& !bExceptionCase 
#endif // EXCEPTION_BUFF_FACTOR	
		)
	{
		return DISPLAY_ERROR( L"FINALIZER Templet is empty" );
	}
	else
		return true;
}

/** @function : ParsingFinalizerTemplateMethod
	@brief : �� �Ļ�Ŭ�������� ������ ParsingFinalizer�� �Ȱ��� ���� ó�� ���̿��� ȣ���ϱ� ���� ���ø��޼ҵ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� FinalizerTempletPtr�� ���� vector(vecBuffFinalizerTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
bool CX2BuffFinalizerTemplet::ParsingFinalizerTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ )
{
	if ( luaManager_.BeginTable( pszTableName_ ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END

		return ParsingFinalizer( luaManager_ );
	}
	else
		return DISPLAY_ERROR( pszTableName_ );	
}

/** @function : SetFactorFromPacketTemplateMothod
	@brief : ��Ŷ���� ���� ���� ���� ������ �� ���������� SetFactorFromPacket�� ���� ��Ű�� ���ø� �޼ҵ�
	@param : ������ ���� ���� ��������(kBuffFactor_), ������ ��������(pGameUnit_)
	@return : ����(true), ����(false)
*/
bool CX2BuffFinalizerTemplet::SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ )
{
	BOOST_FOREACH( const KBuffFinalizerFactor& kFactor_, kBuffFactor_.m_vecBuffFinalizerFactors )
	{
		if ( GetType() == kFactor_.m_uiType )
		{
			SetFactorFromPacket( kFactor_, pGameUnit_ );
			return true;
		}		
	}

	return DISPLAY_ERROR( L"SetFactorFromPacket Error" );
}

/** @function : ParsingFinalizer
	@brief : �ð��� ���� ����Ǵ� ���ø� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : ������ true
*/
/*virtual*/ bool CX2BuffTimeFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_DURATION_TIME_TYPE", m_eDurationTimeType,
		BUFF_DURATION_TIME_TYPE, BDTT_NONE, return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE" ) );

	return true;
}

/** @function : OnFrameMove;
	@brief : �� ������ ���� DurationTime�� ���� ��Ű�� 0 �̸��� �Ǹ� �����
	@param : ������ �ɸ� ������ ������(pGameUnit_)
	@return : 
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffTimeFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffTimeFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fDurationTime -= fElapsedTime_;	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fDurationTime -= g_pKTDXApp->GetElapsedTime();	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef BUFF_ICON_UI //���� ���� ���� �ð� ���	
	if( false == m_bIsAlreadyNotifyDurationTime // �����ð��� ���� �˸��� ������ �ʾ��� ��
		&& 5.f >= m_fDurationTime )
	{
		CX2GageManager::GetInstance()->NotifyDurationTime5sec( pGameUnit_->GetUnitUID(), m_eBuffTempleteID );
		m_bIsAlreadyNotifyDurationTime = true;
	}
#ifdef DISPLAY_BUFF_DURATION_TIME
	int iDurationSec = (false == IsSamef(m_fDurationTime,0.f)) ? static_cast<int>(m_fDurationTime) + 1 : 0;
	if( iDurationSec != m_iDurationSec )
	{
		m_iDurationSec = iDurationSec;
		CX2GageManager::GetInstance()->SetDurationTime( pGameUnit_->GetUnitUID(), m_eBuffTempleteID, m_iDurationSec );
	}
#endif // DISPLAY_BUFF_DURATION_TIME

#endif //BUFF_ICON_UI
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffTimeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{

	const KBuffFinalizerFactor* pFactor = NULL;
	
	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		BUFF_DURATION_TIME_TYPE eDurationTimeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_DURATION_TIME_TYPE, (*pFactor)[FO_DURATION_BUFF_TIME_TYPE] );
		
		/// ������ �����ϴ� �͵� �ƴϰ� ������ ���� ������ �ִ� ��� ���װ��� ����

// 		const float fRegist = ( BuffFactor_.GetIgnoreRegistForRate() ? 
// 			0.0f : pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );

		const float fRegist = pGameUnit_->GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( pBuffTemplet_->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE;

		if ( m_eDurationTimeType == eDurationTimeType )
		{
			switch ( m_eDurationTimeType )
			{
			case BDTT_NORMAL_TIME:
				{
					const float fNormalTime = (*pFactor)[FO_NORMAL_TIME] * (1.0f - fRegist);
					// ���ӽð��� ���ؼ� �ð��� ������� ��ü
					m_fDurationTime = ( m_fDurationTime > fNormalTime ? m_fDurationTime : fNormalTime );
				} break;

			case BDTT_RELATION_HYPER_MODE_COUNT:
				{
					const float fNormalTime = (*pFactor)[FO_NORMAL_TIME] * (1.0f - fRegist);
					// ���ӽð��� ���ؼ� �ð��� ������� ��ü
					m_fDurationTime += fNormalTime;
				} break;

			case BDTT_RANDOM_TIME:
				{
					// ���� �ð��� fMinTime ~ fMaxTime ����
					const float fMinTime = (*pFactor)[FO_MIN_TIME];
					const float fMaxTime = (*pFactor)[FO_MAX_TIME];
					const UINT uiTimeGap = static_cast<UINT>( fMaxTime - fMinTime );

					/// �ٸ� �����̱� ������ ���� �������� 0���� ���� �ص� ���� ����
					const float fNewTime = ( fMinTime + ( pGameUnit_->GetRandomInt( 0 ) % (uiTimeGap > 0 ? uiTimeGap : 1) ) ) * (1.0f - fRegist);

					/// ���ο� �ð��� ������ �ð����� Ŭ�� ����
					m_fDurationTime = ( m_fDurationTime > fNewTime ? m_fDurationTime : fNewTime );
				} break;

			default:
				return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE Invalid" );
				break;
			}

			//���̺� �뺴�� ó�ż� �нú�(SI_P_RWT_BEHAVIOR_OF_MERCENARY) ȿ���� �����Ű�� ���� �߰�
			m_fDurationTime *= pGameUnit_->GetReduceDebuffDurationRate( pBuffTemplet_->GetBuffTempletID() );
#ifdef BUFF_ICON_UI
			m_eBuffTempleteID = BuffFactor_.GetBuffTempletID();
			m_bIsAlreadyNotifyDurationTime = false;
#endif //BUFF_ICON_UI

			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_DURATION_TIME_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffTimeFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffTimeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffTimeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffTimeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( !DidReserveToFinish() && 0 < m_fDurationTime )
		return false;
	else
		return true;
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffTimeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	/// ���� �ð�
	FinalizerFactor.m_vecValues.push_back( m_fDurationTime );

	/// ���۽��� FrameMoveCount;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    DWORD dwCurFrameMoveCount = ( g_pX2Game != NULL ) ? g_pX2Game->GetFrameUDPPack().GetFrameMoveCount() : 0;
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( dwCurFrameMoveCount ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	FinalizerFactor.m_vecValues.push_back( static_cast<float>( pGameUnit_->GetFrameMoveCountNow() ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ð��� ���� ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffTimeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// ���� ���� ���� �ð��� ����
	m_fDurationTime = kFactor_[0];

	const DWORD dwFrameMoveCount = static_cast<DWORD>( kFactor_[1] );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    DWORD dwCurFrameMoveCount = ( g_pX2Game != NULL ) ? g_pX2Game->GetFrameUDPPack().GetFrameMoveCount() : 0;
	if ( dwCurFrameMoveCount > dwFrameMoveCount )
	{
		const float fElapsedTime = static_cast<float>( ( dwCurFrameMoveCount - dwFrameMoveCount ) / g_pKTDXApp->GetFrameMoveFPS() );
		m_fDurationTime -= fElapsedTime;
	}
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	if ( pGameUnit_->GetFrameMoveCountNow() > dwFrameMoveCount )
//	{
//		const float fElapsedTime = static_cast<float>( ( pGameUnit_->GetFrameMoveCountNow() - dwFrameMoveCount ) / g_pKTDXApp->GetFrameMoveFPS() );
//		m_fDurationTime -= fElapsedTime;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : Ÿ�� ī��Ʈ�� ���� ����Ǵ� ���ø� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : ������ true
*/
/*virtual*/ bool CX2BuffHitCountFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_USE_COUNT_TYPE", m_eUseCountType,
		BUFF_USE_COUNT_TYPE, BUCT_NOT_USE, return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" ) );
	return true;
}

/** @function : OnFrameMove
	@brief : Ÿ��ī��Ʈ�� ��ȿ������ ����� m_iCountToBeFinished�� �缳�� �Ѵ�.
	@param : ������ �ɸ� ����(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffHitCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffHitCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	/// �̹� �����ӿ� Ÿ���� ����ŭ�� ī��Ʈ���� ���Ѵ�.
	if ( m_bStart && MAX_COUNT <= m_iCountToBeFinished )
	{
		const UCHAR ucCountNow = pGameUnit_->GetHitCount();
		/// ������ ī��Ʈ�� �������� ���� ī��Ʈ���� �۴ٸ�
		/// ������ ��ȿ����(UCHAR:0~255)�� �ѹ��� �����̱� ������ �缳����
		if ( ucCountNow < m_ucCountWhenStart )
			m_iCountToBeFinished -= MAX_COUNT;	
	}
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffHitCountFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		/// Ÿ���� �˾Ƴ���
		const BUFF_USE_COUNT_TYPE eUseCountType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, (*pFactor)[FO_USE_BUFF_COUNT_TYPE] );

		if ( eUseCountType == m_eUseCountType )
		{
			const UCHAR ucCountNow = pGameUnit_->GetHitCount();
			
			if ( false == m_bStart )
			{
				m_ucCountWhenStart = ucCountNow;
				m_bStart = true;
			}

			const UINT uiCountNew = GetNewCount( pFactor, pGameUnit_ );
			/// ������ �ͺ��� ���ο� ��(uiCountNew)�� ���� ī��Ʈ�� ��� ���ο� ������ ��ü
			const int iCountOld = m_iCountToBeFinished - static_cast<int>( ucCountNow );

			m_iCountToBeFinished 
				= ( iCountOld > static_cast<int>( uiCountNew ) ? m_iCountToBeFinished : static_cast<int>( ucCountNow + uiCountNew ) );
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffHitCountFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffHitCountFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffHitCountFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffHitCountFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( m_bStart && ( m_iCountToBeFinished <= static_cast<int>( pGameUnit_->GetHitCount() ) || DidReserveToFinish() ) )
		return true;	/// ����
	else
		return false;	/// ���
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffHitCountFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_iCountToBeFinished ) );	/// ������ ������ ���� ī��Ʈ
	FinalizerFactor.m_vecValues.push_back( m_ucCountWhenStart );	/// ������ �������� ���� ī��Ʈ
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ Ÿ�� ī��Ʈ�� ���� ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffHitCountFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_iCountToBeFinished = static_cast<int>( kFactor_[0] );
	m_ucCountWhenStart = static_cast<UCHAR>( kFactor_[1] );

	m_bStart = true;
	const UCHAR ucCountNow = pGameUnit_->GetHitCount();
	/// ������ ī��Ʈ�� �������� ���� ī��Ʈ���� �۴ٸ�
	/// ������ ��ȿ����(UCHAR:0~255)�� �ѹ��� �����̱� ������ �缳����
	if ( ucCountNow < m_ucCountWhenStart )
		m_iCountToBeFinished -= MAX_COUNT;	
}

/** @function : GetNewCount
	@brief : ������ ī��Ʈ ���� ��
	@param : ����������(pFactor_), ������ �ɸ� ��������(pGameUnit_)
	@return : ī��Ʈ ��
*/
UINT CX2BuffHitCountFinalizerTemplet::GetNewCount( const KBuffFinalizerFactor* pFactor_, CX2GameUnit* pGameUnit_ ) const
{
	if ( 0.0f < (*pFactor_)[FO_RESULT_COUNT] )		/// ������ ī��Ʈ�� �ִٸ� �ٷ� ����(�̹� �������� ���� ������ ���� �޴� ��� �Ǵ� �������� ����ī��Ʈ ���� ��)
		return static_cast<UINT>( (*pFactor_)[FO_RESULT_COUNT] );
	else	/// ���ٸ�
	{
		switch ( m_eUseCountType )
		{
		case BUCT_ENEMY_BUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( false ) );
			} break;

		case BUCT_ENEMY_DEBUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( true ) );
			} break;

		default:
			{
				DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" );	/// �ٸ� Ÿ�Կ� �̰��� �����Ѵٸ� ERROR
				return 0;
			} break;
		}
	}		
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : �ǰ� ī��Ʈ�� ���� ����Ǵ� ���ø� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffHittedCountFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_USE_COUNT_TYPE", m_eUseCountType,
		BUFF_USE_COUNT_TYPE, BUCT_NOT_USE, return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" ) );
	return true;
}

/** @function : OnFrameMove
	@brief : �ǰ�ī��Ʈ�� ��ȿ������ ����� m_iCountToBeFinished�� �缳�� �Ѵ�.
	@param : ������ �ɸ� ����(pGameUnit_)
*/

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BuffHittedCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffHittedCountFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	/// �̹� �����ӿ� Ÿ���� ����ŭ�� ī��Ʈ���� ���Ѵ�.
	if ( m_bStart && MAX_COUNT <= m_iCountToBeFinished )
	{
		const UCHAR ucCountNow = pGameUnit_->GetHittedCount();
		/// ������ ī��Ʈ�� �������� ���� ī��Ʈ���� �۴ٸ�
		/// ������ ��ȿ����(UCHAR:0~255)�� �ѹ��� �����̱� ������ �缳����
		if ( ucCountNow < m_ucCountWhenStart )
			m_iCountToBeFinished -= MAX_COUNT;	
	}
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffHittedCountFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		/// Ÿ���� �˾Ƴ���
		const BUFF_USE_COUNT_TYPE eUseCountType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_USE_COUNT_TYPE, (*pFactor)[FO_USE_BUFF_COUNT_TYPE] );

		if ( eUseCountType == m_eUseCountType )
		{
			const UCHAR ucCountNow = pGameUnit_->GetHittedCount();

			if ( false == m_bStart )
			{
				m_ucCountWhenStart = ucCountNow;
				m_bStart = true;
			}

			const UINT uiCountNew = GetNewCount( pFactor, pGameUnit_ );
			/// ������ �ͺ��� ���ο� ��(uiCountNew)�� ���� ī��Ʈ�� ��� ���ο� ������ ��ü
			const int iCountOld = m_iCountToBeFinished - static_cast<int>( ucCountNow );

			m_iCountToBeFinished 
				= ( iCountOld > static_cast<int>( uiCountNew ) ? m_iCountToBeFinished : static_cast<int>( ucCountNow + uiCountNew ) );
			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffHitCountFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffHittedCountFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffHittedCountFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffHittedCountFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( m_bStart && ( m_iCountToBeFinished <= static_cast<int>( pGameUnit_->GetHittedCount() ) || DidReserveToFinish() ) )
		return true;	/// ����
	else
		return false;	/// ���
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffHittedCountFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_iCountToBeFinished ) );	/// ������ ������ ���� ī��Ʈ
	FinalizerFactor.m_vecValues.push_back( m_ucCountWhenStart );	/// ������ �������� ���� ī��Ʈ
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ �ǰ� ī��Ʈ������ ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffHittedCountFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_iCountToBeFinished = static_cast<int>( kFactor_[0] );
	m_ucCountWhenStart = static_cast<UCHAR>( kFactor_[1] );

	//if ( pGameUnit_->DidReceiveFirstSyncPacket() )
	{
		m_bStart = true;
		const UCHAR ucCountNow = pGameUnit_->GetHittedCount();
		/// ������ ī��Ʈ�� �������� ���� ī��Ʈ���� �۴ٸ�
		/// ������ ��ȿ����(UCHAR:0~255)�� �ѹ��� �����̱� ������ �缳����
		if ( ucCountNow < m_ucCountWhenStart )
			m_iCountToBeFinished -= MAX_COUNT;	
	}
// 	else
// 	{
// 		DISPLAY_ERROR( L"GameUnit didn't receive first sync packet" );
// 	}
}

/** @function : GetNewCount
	@brief : ������ ī��Ʈ ���� ��
	@param : ����������(pFactor_), ������ �ɸ� ��������(pGameUnit_)
	@return : ī��Ʈ ��
*/
UINT CX2BuffHittedCountFinalizerTemplet::GetNewCount( const KBuffFinalizerFactor* pFactor_, CX2GameUnit* pGameUnit_ ) const
{
	if ( 0.0f < (*pFactor_)[FO_RESULT_COUNT] )		/// ������ ī��Ʈ�� �ִٸ� �ٷ� ����(�̹� �������� ���� ������ ���� �޴� ��� �Ǵ� �������� ����ī��Ʈ ���� ��)
		return static_cast<UINT>( (*pFactor_)[FO_RESULT_COUNT] );
	else	/// ���ٸ�
	{
		switch ( m_eUseCountType )
		{
		case BUCT_ENEMY_BUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( false ) );
			} break;

		case BUCT_ENEMY_DEBUFF_COUNT:
			{
				return static_cast<UINT>( (*pFactor_)[FO_COUNT] * pGameUnit_->GetAppliedBuffCount( true ) );
			} break;

		default:
			{
				DISPLAY_ERROR( L"BUFF_USE_COUNT_TYPE" );	/// �ٸ� Ÿ�Կ� �̰��� �����Ѵٸ� ERROR
				return 0;
			} break;
		}
	}		
}
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffStateChangeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if ( 0 == m_uiStateID )
			m_uiStateID = pGameUnit_->GetNowStateID();

		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffHitCountFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffStateChangeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffStateChangeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffStateChangeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return ( DidReserveToFinish() || m_uiStateID != static_cast<UINT>( pGameUnit_->GetNowStateID() ) );		/// �ٸ��� ����
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffStateChangeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_uiStateID ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ������ ������Ʈ�� ����Ǹ� ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffStateChangeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_uiStateID = static_cast<UINT>( kFactor_[0] );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : ���� �������� ������ ����Ǵ� ���� �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffDamageValueFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );

	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_RELATION_TYPE", m_eRelationType, 
		BUFF_RELATION_TYPE, BRT_END, return DISPLAY_ERROR( L"BUFF_RELATION_TYPE" ) );

	return true;
}

/** @function : OnFrameMove
	@brief : ����� HP�� ���� �������� HP���� ������ ������ ������ �� �ִ� �������� ���� ��Ŵ
	@param : ������ �ɸ� ����(pGameUnit_)
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( m_bStart )
	{
		ModifyDamageValue( pGameUnit_ );

		const float fHpNowFrame = pGameUnit_->GetNowHp();

#ifndef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// �������� Damage Value ���� ��Ű�� ���� ����
		/// kimhc // ���� �������� Hp �� �� ũ�� ���ݹ��� ���̶�� ����(�������ӿ� Ÿ���� �ް� ȸ���Ѵٸ�?)
		if ( fHpNowFrame < m_fNowHpOldFrame )
			m_fDamageValue -= (m_fNowHpOldFrame - fHpNowFrame);
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

		m_fNowHpOldFrame = fHpNowFrame;	/// �������� HP�� ������������ HP�� ��

	}	
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffDamageValueFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		const BUFF_CHANGE_TYPE eChangeType 
			= STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pFactor)[FO_CHANGE_TYPE] );
		
		if ( eChangeType == m_eChangeType )
		{
			if ( BCT_RELATION_VALUE == m_eChangeType )
			{
				const BUFF_RELATION_TYPE eRelationType 
					= STATIC_CAST_FLOAT_TO_ENUM( BUFF_RELATION_TYPE, (*pFactor)[FO_RELATION_TYPE] );

				if ( eRelationType != m_eRelationType )
					return DISPLAY_ERROR( L"BUFF_RELATION_TYPE MISS MATCH" );
			}

			const float fValue = GetValue( pFactor, pGameUnit_ );

			if ( false == m_bStart )
			{
				m_fDamageValue = fValue;
				m_fNowHpOldFrame = pGameUnit_->GetNowHp();
				m_bStart = true;
			}
			else
				m_fDamageValue = ( fValue > m_fDamageValue ? fValue : m_fDamageValue );

			return true;
		}
		else
			return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffDamageValueFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffDamageValueFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffDamageValueFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffDamageValueFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return ( DidReserveToFinish() || !(m_fDamageValue > 0) );	/// 0���� ũ�� ������ ������
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();
	
	FinalizerFactor.m_vecValues.push_back( m_fNowHpOldFrame );	/// ������ ������ OldFrame Hp (������ ���� �� Ŭ������ OnFrameMove�� �����ϰ� �� m_fDamageValue�� �����Ǿ��� ���̴�.
	FinalizerFactor.m_vecValues.push_back( m_fDamageValue );	/// ���� DamageValue
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� �������� ������ ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffDamageValueFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fNowHpOldFrame = kFactor_[0];
	m_fDamageValue = kFactor_[1];		

	m_bStart = true;
	ModifyDamageValue( pGameUnit_ );
}

/** @function : GetValue
	@brief : Ÿ�Կ� ���� ���� ����
	@param : ������(pFinalizerFactor_), ��������(pGameUnit_)
	@return : Ÿ�Կ� ���� ��� ��
*/
float CX2BuffDamageValueFinalizerTemplet::GetValue( const KBuffFinalizerFactor* pFinalizerFactor_, CX2GameUnit* pGameUnit_ ) const
{
	const float fValue = (*pFinalizerFactor_)[FO_VALUE];

	switch ( m_eChangeType )
	{
	case BCT_FIX_VALUE:
		{
			return fValue;
		} break;

	case BCT_RELATION_VALUE:
		{
			return fValue * (*pFinalizerFactor_)[FO_MULTIPLIER];
		} break;

	default:
		return 0.0f;
		break;
	}
}

/** @function : ModifyDamageValue
	@brief : ���� �����Ӱ� ���� HP�� ���Ͽ� ����� ���������� ������
	@param : ������ ����� ��������
*/
void CX2BuffDamageValueFinalizerTemplet::ModifyDamageValue( const CX2GameUnit* pGameUnit_ )
{
	const float fNowHp = pGameUnit_->GetNowHp();
	if ( fNowHp > m_fNowHpOldFrame )
		m_fDamageValue += fNowHp - m_fNowHpOldFrame;	/// ������ Hp�� ��Ŷ�� ���������� Hp���� ũ�� �� ��ŭ �����ְ�
	else
		m_fDamageValue -= m_fNowHpOldFrame - fNowHp;	/// ������ Hp�� ��Ŷ�� ���������� Hp���� ������ �� ��ŭ ���ش�
	/// ��Ŷ ���۵ɶ����� Hp�� ä����̶�� ��� �ϳ�...
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : ���� HP�� ���� �Ǵ� ���� ���� �ϸ� ����Ǵ� Ŭ������ �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffPassHpFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "PASS_UP", m_bPassUp, false, return DISPLAY_ERROR( L"PASS_UP" ) );
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eBuffChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
	return true;
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffPassHpFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		const bool bPassUp = ( (*pFactor)[FO_PASS_UP] > 0.0f ? true : false );	/// ���� ���� �ΰ� ���⵹���ΰ�?
		if ( bPassUp == m_bPassUp )
		{
			const BUFF_CHANGE_TYPE eBuffChangeType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pFactor)[FO_CHANGE_TYPE] );		/// % �ΰ� �� �ΰ�?
			if ( eBuffChangeType == m_eBuffChangeType )
			{
				SelectDelegate();

				const float fCriterion = (*pFactor)[FO_CRITERION];	/// ���� ��ġ
				m_fCriterion = ( m_fCriterion > fCriterion ? m_fCriterion : fCriterion );
				return true;
			}
			else
				return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
		}
		else
			return DISPLAY_ERROR( L"PASS_UP MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffPassHpFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffPassHpFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffPassHpFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffPassHpFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return DidReserveToFinish() || m_delegateCheckHpByPassTypeFunc( pGameUnit_ );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffPassHpFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( m_fCriterion );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� HP�� ���� �Ǵ� ���� �����ϸ� ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffPassHpFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fCriterion = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : ����Ÿ�Կ� ���� �ٸ��� ������ �Լ� ����
*/
void CX2BuffPassHpFinalizerTemplet::SelectDelegate()
{
	if ( m_bPassUp )
		m_delegateCheckHpByPassTypeFunc = DelegateCheckHpByPassType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::CheckHpByUp>( this );
	else
		m_delegateCheckHpByPassTypeFunc = DelegateCheckHpByPassType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::CheckHpByDown>( this );		

	switch ( m_eBuffChangeType )
	{
	case BCT_FIX_VALUE:	
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::GetCriterionByValue>( this );
		break;

	case BCT_PERCENT:
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::GetCriterionByPercent>( this );
		break;

	default:
		{
			DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" );
			m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassHpFinalizerTemplet, &CX2BuffPassHpFinalizerTemplet::GetCriterionByValue>( this );
		} break;
	}
}

/** @function : CheckHpByUp
	@brief : ���⵹�� ���θ� üũ
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���⵹�Ľ� true �ƴϸ� false
*/
bool CX2BuffPassHpFinalizerTemplet::CheckHpByUp( CX2GameUnit* pGameUnit_ ) const
{
	/// ���� HP�� ���� ����
	return pGameUnit_->GetNowHp() > m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : CheckHpByDown
	@brief : ���⵹�� ���θ� üũ
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���⵹�Ľ� true �ƴϸ� false
*/
bool CX2BuffPassHpFinalizerTemplet::CheckHpByDown( CX2GameUnit* pGameUnit_ ) const
{
	/// ���� HP�� ���� ����
	return pGameUnit_->GetNowHp() < m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : GetCriterionByValue
	@brief : ������ ��ġ�� ������ �־� ��ġ�� ����
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���� ��ġ
*/
float CX2BuffPassHpFinalizerTemplet::GetCriterionByValue( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion;	/// ��ġ
}

/** @function : GetCriterionByPercent
	@brief : ������ %�� ������ �־� MaxHp ��� Percent�� ���� ��ġ�� ����
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���� ��ġ(%)
*/
float CX2BuffPassHpFinalizerTemplet::GetCriterionByPercent( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion * pGameUnit_->GetMaxHp();	/// �ִ�HP ��� ��ġ
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : ParsingFinalizer
	@brief : ���� Mp�� ���� �Ǵ� ���� ���� �ϸ� ����Ǵ� Ŭ������ �Ľ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_)
	@return : ������ true, ���н� false
*/
/*virtual*/ bool CX2BuffPassMpFinalizerTemplet::ParsingFinalizer( KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "PASS_UP", m_bPassUp, false, return DISPLAY_ERROR( L"PASS_UP" ) );
	LUA_GET_VALUE_RETURN_ENUM( luaManager_, "BUFF_CHANGE_TYPE", m_eBuffChangeType, 
		BUFF_CHANGE_TYPE, BCT_SWAP_VALUE, return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" ) );
	return true;
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffPassMpFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		const bool bPassUp = ( (*pFactor)[FO_PASS_UP] > 0.0f ? true : false );	/// ���� ���� �ΰ� ���⵹���ΰ�?
		if ( bPassUp == m_bPassUp )
		{
			const BUFF_CHANGE_TYPE eBuffChangeType = STATIC_CAST_FLOAT_TO_ENUM( BUFF_CHANGE_TYPE, (*pFactor)[FO_CHANGE_TYPE] );		/// % �ΰ� �� �ΰ�?
			if ( eBuffChangeType == m_eBuffChangeType )
			{
				SelectDelegate();
				const float fCriterion = (*pFactor)[FO_CRITERION];	/// ���� ��ġ
				m_fCriterion = ( m_fCriterion > fCriterion ? m_fCriterion : fCriterion );
				return true;
			}
			else
				return DISPLAY_ERROR( L"BUFF_CHANGE_TYPE MISS MATCH" );
		}
		else
			return DISPLAY_ERROR( L"PASS_UP MISS MATCH" );
	}
	else
		return DISPLAY_ERROR( L"CX2BuffPassMpFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffPassMpFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffPassMpFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffPassMpFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return DidReserveToFinish() || m_delegateCheckMpByPassTypeFunc( pGameUnit_ );
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffPassMpFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( m_fCriterion );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� MP�� ���� �Ǵ� ���� �����ϸ� ����Ǵ� ���ø��� ����
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffPassMpFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	m_fCriterion = kFactor_[0];
	SelectDelegate();
}

/** @function : SelectDelegate
	@brief : ����Ÿ�Կ� ���� �ٸ��� ������ �Լ� ����
*/
void CX2BuffPassMpFinalizerTemplet::SelectDelegate()
{
	if ( m_bPassUp )
		m_delegateCheckMpByPassTypeFunc = DelegateCheckMpByPassType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::CheckMpByUp>( this );
	else
		m_delegateCheckMpByPassTypeFunc = DelegateCheckMpByPassType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::CheckMpByDown>( this );		

	switch ( m_eBuffChangeType )
	{
	case BCT_FIX_VALUE:	
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::GetCriterionByValue>( this );
		break;

	case BCT_PERCENT:
		m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::GetCriterionByPercent>( this );
		break;

	default:
		{
			DISPLAY_ERROR( L"BUFF_CHANGE_TYPE" );
			m_delegateGetCriterionByChangeTypeFunc = DelegateGetCriterionByChangeType::from_const_method<CX2BuffPassMpFinalizerTemplet, &CX2BuffPassMpFinalizerTemplet::GetCriterionByValue>( this );
		} break;
	}
}

/** @function : CheckMpByUp
	@brief : ���⵹�� ���θ� üũ
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���⵹�Ľ� true �ƴϸ� false
*/
bool CX2BuffPassMpFinalizerTemplet::CheckMpByUp( CX2GameUnit* pGameUnit_ ) const
{
	/// ���� Mp�� ���� ����
	return pGameUnit_->GetNowMp() > m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : CheckMpByDown
	@brief : ���⵹�� ���θ� üũ
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���⵹�Ľ� true �ƴϸ� false
*/
bool CX2BuffPassMpFinalizerTemplet::CheckMpByDown( CX2GameUnit* pGameUnit_ ) const
{
	/// ���� Mp�� ���� ����
	return pGameUnit_->GetNowMp() < m_delegateGetCriterionByChangeTypeFunc( pGameUnit_ );
}

/** @function : GetCriterionByValue
	@brief : ������ ��ġ�� ������ �־� ��ġ�� ����
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���� ��ġ
*/
float CX2BuffPassMpFinalizerTemplet::GetCriterionByValue( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion;	/// ��ġ
}

/** @function : GetCriterionByPercent
	@brief : ������ %�� ������ �־� MaxMp ��� Percent�� ���� ��ġ�� ����
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : ���� ��ġ(%)
*/
float CX2BuffPassMpFinalizerTemplet::GetCriterionByPercent( CX2GameUnit* pGameUnit_ ) const
{
	return m_fCriterion * pGameUnit_->GetMaxMp();	/// �ִ�Mp ��� ��ġ
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffImmediateFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffImmediateFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffImmediateFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffImmediateFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffImmediateFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return true;
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffImmediateFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffDungeonStageChangeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if ( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
		{
			CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
			m_uiIndex = pDungeonGame->GetNowStageIndex();
			return true;
		}
		else
			return false;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffDungeonStageChangeFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffDungeonStageChangeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffDungeonStageChangeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffDungeonStageChangeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if ( DidReserveToFinish() || pDungeonGame->GetNowStageIndex() != m_uiIndex )	/// ������ ���� �Ǿ��� ���ϰ� ���������� �ٸ���
			return true;	/// ����
		else
			return false;
	}
	else
		return true;
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffDungeonStageChangeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_uiIndex ) );
}


/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� ��Ҹ� ���� �Ͽ� �۵� ��Ŵ
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffDungeonStageChangeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// ������ ���۵Ǿ��� ���� ��������
	m_uiIndex = static_cast<UINT>( kFactor_[0] );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ADD_LIVE_CREATOR_FINALIZER
/** @function : OnFrameMove;
	@brief : �� ������ ���� ���� ���� ������ ����ְų� �����ϴ��� Ȯ��
	@param : ������ �ɸ� ������ ������(pGameUnit_)
	@return : 
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if( null == m_optrCreatorUnit || 0.f >= m_optrCreatorUnit->GetNowHp() )		/// �����ڰ� �װų� ������ �˻�
		m_bIsLiveCreator = false;
}

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffLiveCreatorFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		m_uidCreatorUID = BuffFactor_.GetUidGameUnit();

		if( true == BuffFactor_.GetIsUserUID() )	/// ������ ���
		{
			m_bIsUserUnit = true;
			CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( g_pX2Game->GetUserUnitByUID( m_uidCreatorUID ) );

			if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
			{
				m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// ���� ������ ���� ��ü ����
				m_bIsLiveCreator  = true;		/// ���� ������ ���� Ȥ�� �������

				return true;
			}
		}
		else	/// NPC�� ���
		{
			CX2GUNPC* pCreatroUnit = static_cast<CX2GUNPC*>( g_pX2Game->GetNPCUnitByUID( static_cast<int>( m_uidCreatorUID ) ) );

			if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
			{
				m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// ���� ������ ���� ��ü ����
				m_bIsLiveCreator  = true;		/// ���� ������ ���� Ȥ�� �������

				return true;
			}
		}

		m_bIsLiveCreator  = false;		/// ���� ������ ��� Ȥ�� ����
	}
	else
		return DISPLAY_ERROR( L"CX2BuffPassMpFinalizerTemplet GetFinalizerFactor didn't work" );

	return true;
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffLiveCreatorFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffLiveCreatorFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffLiveCreatorFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( NULL != g_pX2Game )
	{
		if( true == m_bIsLiveCreator )		/// ���� �����ڰ� ���������, ���� ��� ����
			return false;
	}
	
	return true;							/// ���� �����ڰ� ����ϰų� �������� ������, ����
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	/// int64�� 16��Ʈ�� float 4���� �ɰ��� ����...
	float fUnitUIDBackDown	=  static_cast<float>( m_uidCreatorUID & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDBackDown ) );	/// ���� UID ���� �Ʒ� ��Ʈ

	float fUnitUIDBackUp	=  static_cast<float>( ( m_uidCreatorUID >> 16 ) & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDBackUp ) );		/// ���� UID ���� �� ��Ʈ

	float fUnitUIDFrontDown =  static_cast<float>( ( m_uidCreatorUID >> 32 ) & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDFrontDown ) );	/// ���� UID ���� �Ʒ� ��Ʈ

	float fUnitUIDFrontUp	=  static_cast<float>( ( m_uidCreatorUID >> 48 ) & 0xFFFF );
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( fUnitUIDFrontUp ) );		/// ���� UID ���� �� ��Ʈ

	/// �������� NPC���� ����
	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_bIsLiveCreator ) );
}


/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� ��Ҹ� ���� �Ͽ� �۵� ��Ŵ
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffLiveCreatorFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// 16��Ʈ�� float 4���� �ɰ��� UID�� int64�� ����

	UidType uidCreatorUID1 = static_cast<UidType>( kFactor_[FO_UNIT_UID_BACK_DOWN] );	/// ���� UID ���� �Ʒ� ��Ʈ
	UidType uidCreatorUID2 = static_cast<UidType>( kFactor_[FO_UNIT_UID_BACK_UP] );		/// ���� UID ���� �� ��Ʈ
	UidType uidCreatorUID3 = static_cast<UidType>( kFactor_[FO_UNIT_UID_FRONT_DOWN] );	/// ���� UID ���� �Ʒ� ��Ʈ
	UidType uidCreatorUID4 = static_cast<UidType>( kFactor_[FO_UNIT_UID_FRONT_UP] );	/// ���� UID ���� �� ��Ʈ

	/// �� ��Ʈ ��ġ ����
	uidCreatorUID2 = uidCreatorUID2 << 16;
	uidCreatorUID3 = uidCreatorUID3 << 32;
	uidCreatorUID4 = uidCreatorUID4 << 48;

	m_uidCreatorUID = uidCreatorUID1 | uidCreatorUID2 | uidCreatorUID3 | uidCreatorUID4;	/// ��Ʈ ��ü
	
	if( false == IsSamef( kFactor_[FO_IS_USER_UNIT], 0.f ) )	/// ������ ���
	{
		m_bIsUserUnit = true;
		CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( g_pX2Game->GetUserUnitByUID( m_uidCreatorUID ) );

		if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
		{
			m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// ���� ������ ���� ��ü ����
			m_bIsLiveCreator  = true;		/// ���� ������ ���� Ȥ�� �������

			return;
		}
	}
	else		/// NPC�� ���
	{
		CX2GUNPC* pCreatroUnit = static_cast<CX2GUNPC*>( g_pX2Game->GetNPCUnitByUID( static_cast<int>( m_uidCreatorUID ) ) );

		if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetNowHp() )
		{
			m_optrCreatorUnit = CX2GameUnitoPtr( pCreatroUnit );	/// ���� ������ ���� ��ü ����
			m_bIsLiveCreator  = true;		/// ���� ������ ���� Ȥ�� �������

			return;
		}
	}

	m_bIsLiveCreator  = false;		/// ���� ������ ��� Ȥ�� ����
}
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffFinishHyperFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if ( NULL != pGameUnit_ )
		{
			CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( pGameUnit_ );

			if( NULL != pCreatroUnit && 0.f < pCreatroUnit->GetHyperModeTime() )
			{
				m_bHyper = true;
				m_eBuffTempleteID = BuffFactor_.GetBuffTempletID();
				m_bIsAlreadyNotifyDurationTime = false;

				return true;
			}
			else
			{
				m_bHyper = false;

				return false;
			}
		}

		m_bHyper = false;
		return false;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffFinishHyperFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffFinishHyperFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffFinishHyperFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffFinishHyperFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( DidReserveToFinish() || false == m_bHyper )
		return true;
	else
		return false;
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( ( true == m_bHyper ? 1.0f : 0.0f ) );
}

/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� ��Ҹ� ���� �Ͽ� �۵� ��Ŵ
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// ������ ���۵Ǿ��� ���� ��������
	m_bHyper = ( kFactor_[0] == 1.f ? true : false );
}

/** @function : OnFrameMove;
	@brief : �� ������ Ȯ��
	@return : 
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_, float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*virtual*/ void CX2BuffFinishHyperFinalizerTemplet::OnFrameMove( CX2GameUnit* pGameUnit_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( NULL != pGameUnit_ )
	{
		CX2GUUser* pCreatroUnit = static_cast<CX2GUUser*>( pGameUnit_ );

		if( NULL != pCreatroUnit && 0.f >= pCreatroUnit->GetRemainHyperModeTime() )
			m_bHyper = false;
		else
			m_bHyper = true;

		m_fDurationTime = pCreatroUnit->GetRemainHyperModeTime();

		if( false == m_bIsAlreadyNotifyDurationTime
			&& 5.f >= m_fDurationTime )
		{
			CX2GageManager::GetInstance()->NotifyDurationTime5sec( pGameUnit_->GetUnitUID(), m_eBuffTempleteID );
			m_bIsAlreadyNotifyDurationTime = true;
		}
		else if ( true == m_bIsAlreadyNotifyDurationTime && 5.f < m_fDurationTime )
		{
			CX2GageManager::GetInstance()->InitBuffIconFlicker( pGameUnit_->GetUnitUID(), m_eBuffTempleteID );
			m_bIsAlreadyNotifyDurationTime = false;
		}
	}
}
#endif //ADD_BUFF_FINALIZER_FINISH_HYPER

#ifdef RIDING_SYSTEM
/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffFinishRidingPetOnOffFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffFinishRidingPetOnOrNotFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	if ( NULL != pGameUnit_ && 
		 ( pGameUnit_->GetRidingOn() != m_bFinishRidingOn ) )	/// ���� ���ǰ� �ݴ��
		 return false;	/// �������� �ʰ�
	else	/// pGameUnit�� ���ų�, �������ǰ� ��ġ�ϸ�
		return true;	/// ���� ����
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
}

/** @function : ParsingFinalizerTemplateMethod
	@brief : �� �Ļ�Ŭ�������� ������ ParsingFinalizer�� �Ȱ��� ���� ó�� ���̿��� ȣ���ϱ� ���� ���ø��޼ҵ�
	@param : �о���̰� �ִ� ��ƽ�ũ��Ʈ�� ��ƸŴ���(luaManager_), �Ľ̼����� FinalizerTempletPtr�� ���� vector(vecBuffFinalizerTempletPtr_)
	@return : �Ľ� ������ true, ���н� false ����
*/
bool CX2BuffFinishRidingPetOnOrNotFinalizerTemplet::ParsingFinalizerTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ )
{
	LUA_GET_VALUE_RETURN( luaManager_, "FINISH_WHEN_RIDING_ON", m_bFinishRidingOn, false, 
		return DISPLAY_ERROR( L"FINISH_WHEN_RIDING_ON" ) );

	return true;
}
#endif //RIDING_SYSTEM


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/** @function : SetFactor
	@brief : ���� ��Ҹ� ���ø��� �����Ͽ� �۵� ��Ŵ
	@param : ���� ���(factor_)
*/
/*virtual*/ bool CX2BuffX2StateChangeFinalizerTemplet::SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ )
{
	const KBuffFinalizerFactor* pFactor = NULL;

	if ( BuffFactor_.GetFinalizerFactor( GetType(), &pFactor ) )
	{
		if( 0 == m_uiIndex )
			m_uiIndex = static_cast<UINT>(g_pMain->GetNowStateID());
		
		return true;
	}
	else
		return DISPLAY_ERROR( L"CX2BuffGameStateChangeFinalizerTemplet GetFinalizerFactor didn't work" );
}

/** @function : GetClonePtr
	@brief : �ڽ��� ������ ��ü�� ����Ʈ�����͸� �����ϴ� �Լ�
	@return : CX2BuffFinalizerTempletPtr
*/
/*virtual*/ CX2BuffFinalizerTempletPtr CX2BuffX2StateChangeFinalizerTemplet::GetClonePtr() const
{
	return CX2BuffFinalizerTempletPtr( new CX2BuffX2StateChangeFinalizerTemplet( *this ) );
}

/** @function : DidFinish
	@brief : �ش� ���������� ���� ������ ���������� �˻��ϴ� �Լ�
	@param : ������ �ɸ� ��������(pGameUnit_)
	@return : true �� ����, false �� ����
*/
/*virtual*/ bool CX2BuffX2StateChangeFinalizerTemplet::DidFinish( CX2GameUnit* pGameUnit_ ) const
{
	return ( DidReserveToFinish() || m_uiIndex != static_cast<UINT>( g_pMain->GetNowStateID() ) );		/// �ٸ��� ����
}

/** @function : GetFactor
	@brief : ���ø� ���ڵ� �� �ʿ� �� �͵��� factor�� ��ȯ
	@param : ������� �ν��Ͻ�(factor_)
*/
/*virtual*/ void CX2BuffX2StateChangeFinalizerTemplet::GetFactor( OUT vector<KBuffFinalizerFactor>& vecFactors_, const CX2GameUnit* pGameUnit_ ) const
{
	vecFactors_.push_back( KBuffFinalizerFactor( GetType() ) );
	KBuffFinalizerFactor& FinalizerFactor = vecFactors_.back();

	FinalizerFactor.m_vecValues.push_back( static_cast<float>( m_uiIndex ) );
}


/** @function : SetFactorFromPacket
	@brief : ��Ŷ���� ���� ���� ���� ������ ���� ��Ҹ� ���� �Ͽ� �۵� ��Ŵ
	@param : ������ ���� ���� ������(kFactor_), ������ ��������(pGameUnit_)
*/
/*virtual*/ void CX2BuffX2StateChangeFinalizerTemplet::SetFactorFromPacket( const KBuffFinalizerFactor& kFactor_, CX2GameUnit* pGameUnit_ )
{
	/// ������ ���۵Ǿ��� ���� ������Ʈ
	m_uiIndex = static_cast<UINT>( kFactor_[0] );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////