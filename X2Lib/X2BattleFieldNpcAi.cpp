#include "StdAfx.h"
#include ".\X2BattleFieldNpcAi.h"


const float		CX2BattleFieldNpcAi::BASIC_INCREASE_HP_PERCENT_BY_SECOND				= 0.002f;
const float		CX2BattleFieldNpcAi::OUT_OF_PATROL_LINE_INCREASE_HP_PERCENT_BY_SECOND	= 0.005f;
const float		CX2BattleFieldNpcAi::OUT_OF_PLAY_LINE_INCREASE_HP_PERCENT_BY_SECOND		= 0.02f;

CX2BattleFieldNpcAi::CX2BattleFieldNpcAi( CX2GUNPC* pMasterNpc_, const CNpcCreatedInfoPtr NpcCreatedInfoPtr_ )
	: CX2GUNPCAI( pMasterNpc_ ), m_bAggressive( NpcCreatedInfoPtr_->GetAggressive() ), m_bCanTargeting( NpcCreatedInfoPtr_->GetAggressive() ), 
	m_bCanPatrol( true ), m_bBackState01( false ), m_bBackState02( false ), m_bArriveAtDest( false ), 
 	m_vecPatrolLineMapIndex( NpcCreatedInfoPtr_->GetPetrolLineIndexList() ), 
	m_ElapsedTimeCheckOnLineMap( 5.0f ), m_ElapsedTimeAfterSuccessToPatrol( 0.0f ), //m_ElapsedTimeUpNowHp( 1.0f ),
 	m_ElapsedTimeComeBackState( 5.0f ), m_vecPlayLineMapIndex( NpcCreatedInfoPtr_->GetPlayLineIndexList() ), 
	m_fIncreaseHpPercentBySecond( BASIC_INCREASE_HP_PERCENT_BY_SECOND ), m_iCheckLineMapAtFirst( -1 )

#ifdef	BATTLE_FIELD_TEST
	, m_uiGroupId( NpcCreatedInfoPtr_->GetGroupId() )
#endif	BATTLE_FIELD_TEST

{
	SetAIType( NAT_FIELD );
}

CX2BattleFieldNpcAi::~CX2BattleFieldNpcAi()
{
}

/*virtual*/ void CX2BattleFieldNpcAi::OnFrameMove( double fTime, float fElapsedTime )
{
	//*
	if ( true == m_bNoBrain )
		return;


#ifdef NEXON_QA_CHEAT_REQ
	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )
	if( true == m_bCanPatrol )
		m_bCanPatrol = ( m_vecPatrolLineMapIndex.size() == 0) ? false : true;
#endif //NEXON_QA_CHEAT_REQ


	if ( false == m_bEnabled )
		return;

	SetMasterUnitData();

	CheckArrivedDestination();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    UpNowHpBySecondProcess( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	UpNowHpBySecondProcess();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// �齺����Ʈ�� Moving�� ����
	if ( GetBackState01() )
		return Moving( fElapsedTime );
	
	if ( GetBackState02() ) 
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_ElapsedTimeComeBackState.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_ElapsedTimeComeBackState.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		if ( m_ElapsedTimeComeBackState.CheckAndResetElapsedTime() )
		{
			// ��ġ �̵� �� �ٷ� P2P ��Ŷ ������ ���� �ϵ��� Direction�� �ٲ��ش�.
			m_pMasterNPC->SetPosition( GetFinalDest(), !IsRight() );
		}
		
		return Moving( fElapsedTime );
	}
	
	// ��Ʈ�� ���θʰ� �÷��� ���θ��� üũ �ð� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_ElapsedTimeCheckOnLineMap.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_ElapsedTimeCheckOnLineMap.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// ��Ʈ�� ���� �ƴϸ� ����ð� ����
	if ( false == GetNowPatroling() )
    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_ElapsedTimeAfterSuccessToPatrol.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_ElapsedTimeAfterSuccessToPatrol.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    }

	if ( m_ElapsedTimeCheckOnLineMap.CheckAndResetElapsedTime() )	// ���θ� üũ�ð� �˻� �� ����
	{
		// ��Ʈ�� ���θ� ���� �ִ��� üũ
		if ( false == CheckOnPatrolLineMap() )	// ��Ʈ�� ���θ� ���� ���ٸ�
		{
			// �÷��� ���θ� ���� �ִ��� �˻�
			if ( false == CheckOnPlayLineMap() )	// �÷��� ���θ� ������ ���ٸ�
			{

#ifdef NEXON_QA_CHEAT_REQ
				CX2User* pUser = g_pData->GetMyUser();
				if( (NULL != pUser && pUser->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == m_bCanPatrol) || (pUser->GetAuthLevel() < CX2User::XUAL_SPECIAL_USER) )
#endif //NEXON_QA_CHEAT_REQ

				 // Do BackState 02
				if ( Patroling() )
				{
					if ( -1 == m_iCheckLineMapAtFirst)
						m_iCheckLineMapAtFirst = m_pMasterNPC->GetLastTouchLineIndex();

					SetNowPatroling( false );
					SetBackState02( true );
					m_ElapsedTimeComeBackState.ResetSumOfElapsedTime();
					m_pMasterNPC->SetIsComeBackState( true );
					SetIncreaseHpPercentBySecond( OUT_OF_PLAY_LINE_INCREASE_HP_PERCENT_BY_SECOND );
					ResetTarget();
					SetLostTarget( false );	// Ÿ���� �Ҿ����� ���θ� �ʱ�ȭ
					return Moving( fElapsedTime );
				}
			}
			else	// �÷��� ���θ� ������ ������
			{
				if ( null == m_optrTargetGameUnit && GetLostTarget() && false == GetNowPatroling() )	// Ÿ���� ���� ���
				{

#ifdef NEXON_QA_CHEAT_REQ
					CX2User* pUser = g_pData->GetMyUser();
					if( (NULL != pUser && pUser->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == m_bCanPatrol) || (pUser->GetAuthLevel() < CX2User::XUAL_SPECIAL_USER) )
#endif //NEXON_QA_CHEAT_REQ

					// Do BackState 01
					if ( Patroling() )
					{
						SetNowPatroling( false );
						SetBackState01( true );
						SetIncreaseHpPercentBySecond( OUT_OF_PATROL_LINE_INCREASE_HP_PERCENT_BY_SECOND );
						ResetTarget();
						SetLostTarget( false );	// Ÿ���� �Ҿ����� ���θ� �ʱ�ȭ
						return Moving( fElapsedTime );
					}
				}
			}
		}
	}
	
	// ���� ������ ������ �ִ� ��� Ÿ����
	SetLostTarget( false );	// Ÿ���� �Ҿ����� ���θ� �ʱ�ȭ
	if ( TargetingGameUnitWhichAttackMe() )
	{
		return Moving( fElapsedTime );	// ��������..
	}

	// Ÿ������ ������ ����, ��Ʈ�� ������ �����̰� (�̰��� ���� �� �� Ÿ�� ���� ���� ���ٸ� Patrol�� �� ����)
	if ( null == m_optrTargetGameUnit && GetCanPatrol() )
	{

#ifdef NEXON_QA_CHEAT_REQ
		CX2User* pUser = g_pData->GetMyUser();
		if( (NULL != pUser && pUser->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == m_bCanPatrol) || (pUser->GetAuthLevel() < CX2User::XUAL_SPECIAL_USER) )
#endif //NEXON_QA_CHEAT_REQ


		// ���� ��Ʈ���� ������ �ʾ����� �ٽ� ��Ʈ�� �������� �������� �ʴ´�.
		if ( false == GetNowPatroling() && 
            m_ElapsedTimeAfterSuccessToPatrol.CheckElapsedTime() && 
			 Patroling() &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			 CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->patrolMoveData.patrolBeginRate ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
             RandomInt() < GetAIData()->patrolMoveData.patrolBeginRate 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )
		{
			m_ElapsedTimeAfterSuccessToPatrol.ResetSumOfElapsedTime();
			return Moving( fElapsedTime );
		}
	}

	// ��� Function�� ���� Ÿ������ �ϴ� ���̸�
	if( true == GetEnableLuaTargetingFunc() && 
		false == GetLuaTargetingFunc().empty() )
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;
		if( m_fElapsedTimeAfterLastTargeting > GetAIData()->targetData.targetInterval )
		{
			m_fElapsedTimeAfterLastTargeting = 0.f;
			//string func = "";
			//ConvertWCHARToChar( func, GetLuaTargetingFunc().c_str() );
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			lua_tinker::call<void>( m_pMasterNPC->GetFunctionLuaState(),  GetLuaTargetingFunc().c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//			lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  func.c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	else
	{
		Targeting( fElapsedTime );
	}
	Moving( fElapsedTime );
	//*/
}


/*virtual*/ void CX2BattleFieldNpcAi::Targeting( float fElapsedTime )
{
	// Ÿ������ �� �� ���� ��쿡�� ���� (�񼱰��ε� ���� ���� �������� ���ٰų� ���)
	if ( false == GetCanTargeting()  )
		return;

	ASSERT( NULL != g_pX2Game->GetWorld()->GetLineMap() );

	// ���� Ÿ������ ����� �ִ� ��� Ÿ���� �Ҿ������ �ʴ��� �˻� �� �Ҿ������ ���ǿ� �ɸ� ���(true ����) Ÿ���� Reset ��
	if ( CheckAndResetTargetLost() )
	{
		ResetTarget();
		SetLostTarget( true );
		SetFinalDestinationPosAndLineMapIndex( m_pMasterNPC->GetPos(), m_pMasterNPC->GetLastTouchLineIndex() );
		SetCanTargeting( IsAggressive() );	// Ÿ���� �Ҿ���� ��� �񼱰� ���ʹ� Targeting�� ����
		m_fElapsedTimeAfterLastTargeting = GetAIData()->targetData.targetInterval;
	}

	m_fElapsedTimeAfterLastTargeting += fElapsedTime;
	// TargetingInterval�� üũ�Ѵ�.
	if ( m_fElapsedTimeAfterLastTargeting < GetAIData()->targetData.targetInterval )
		return;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fElapsedTimeAfterLastTargeting = 0.0f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// ���� Ÿ���� �ִ� ��� Ÿ���� ���� �� ������ ó��
	if ( CanPreserveLastTargetUnit() )
		return;		// ����

	/////////////////////////// ���⼭ ���ʹ� ���ο� ������ Ÿ���� ////////////////////////////////
	
	// ���� �����̿� �ִ� ���ֵ��� vector�� �̾Ƴ�
	GetTargetListInNearRangeOrRange();

	// ������ Ÿ�� �� �� �ִ� �Ÿ��� ������ ���ų�(m_vecTargetGameUnitListInNearRange), 
	// GetAIData()->targetData.targetSuccessRate Ȯ�����ǿ� �����ϸ� �Ϲ����� Range ���� ���ֵ��� Ÿ�� ������� ��´�
	if ( !m_vecTargetGameUnitListInRange.empty() )
	{
		if ( m_vecTargetGameUnitListInNearRange.empty() 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            || CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->targetData.targetSuccessRate ) == true 
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            || RandomInt() < GetAIData()->targetData.targetSuccessRate 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )
			SearchingTargetUnit( m_vecTargetGameUnitListInRange );
		else	// �ƴϸ� ������ Target�� ������ ���ֵ��� ������� Ÿ�� ������ ã�´�.
			SearchingTargetUnit( m_vecTargetGameUnitListInNearRange );

		m_vecTargetGameUnitListInRange.resize( 0 );
		m_vecTargetGameUnitListInNearRange.resize( 0 );
	}
	else
		m_fElapsedTimeAfterLastTargeting = 0.0f;

	// Ÿ�� ���� ���� �Դµ��� Ÿ���� ������... ��Ʈ���� �ؾ��ϴµ�..
}

/*virtual*/ void CX2BattleFieldNpcAi::LoadAIDataFromLUA( KLuaManager& luaManager )
{
	CX2GUNPCAI::LoadAIDataFromLUA( luaManager );

	// ��Ʈ�� ��Ÿ�� ����
	m_ElapsedTimeAfterSuccessToPatrol.SetTargetTime( GetAIData()->patrolMoveData.patrolCoolingTime );
}

bool CX2BattleFieldNpcAi::Patroling()
{
	const UINT uiSizeofPatrolLineMap = m_vecPatrolLineMapIndex.size();
	ASSERT( 0 != uiSizeofPatrolLineMap );
	if ( 0 == uiSizeofPatrolLineMap )
		return false;

	const int iDestinationLineMapIndexForPatrol = m_vecPatrolLineMapIndex[RandomInt() % uiSizeofPatrolLineMap];

	// ���� �ڽ��� �� �ִ� ��ġ�� ������
	if ( GetLastTouchLineIndex() == iDestinationLineMapIndexForPatrol )
		return false;

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	ASSERT( NULL != pLineMap );

	const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iDestinationLineMapIndexForPatrol );
	ASSERT( NULL != pLineData );
	if ( NULL == pLineData )
		return false;
	
	const float fDistance3SqFromLineStartPos = GetDistance3Sq( pLineData->startPos, GetNowPosition() );
	const float fDistance3SqFromLineEndPos = GetDistance3Sq( pLineData->endPos, GetNowPosition() );
	const float fMoveGap3Sq = pow( static_cast<float>( GetAIData()->chaseMoveData.moveGap ), 2 );
	
	if ( fDistance3SqFromLineStartPos < fMoveGap3Sq && fDistance3SqFromLineEndPos > fMoveGap3Sq )
		SetFinalDestinationPosAndLineMapIndex( pLineData->endPos,	iDestinationLineMapIndexForPatrol );
	else if ( fDistance3SqFromLineEndPos < fMoveGap3Sq && fDistance3SqFromLineStartPos > fMoveGap3Sq )
		SetFinalDestinationPosAndLineMapIndex( pLineData->startPos,	iDestinationLineMapIndexForPatrol );
	else
		SetFinalDestinationPosAndLineMapIndex( D3DXVECTOR3( pLineData->startPos + (pLineData->endPos - pLineData->startPos) * RandomFloat( 0.1f, 0.9f ) ),
		iDestinationLineMapIndexForPatrol );

	SetNowPatroling( true );

	return true;
}

bool CX2BattleFieldNpcAi::TargetingGameUnitWhichAttackMe()
{
	// �ڽ��� ������ ������ �ִ��� üũ
	if ( null != m_optrAttackerGameUnit )	// �ڽ��� ������ ������ ������
	{
		if ( null == m_optrTargetGameUnit	// ������ Ÿ�� ���� ������ ���ų�
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            || m_pMasterNPC->EstimateFrameAccumPercent( GetAIData()->targetData.attackTargetRate ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			|| RandomInt() < GetAIData()->targetData.attackTargetRate 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )	// ������ ������ Ÿ���� Ȯ�� üũ�� �����ϸ�
		{
			m_optrTargetGameUnit = m_optrAttackerGameUnit;	// Ÿ���� ��

			ResetAttackerGameUnit();
			
			if ( false == IsAggressive() )	// �񼱰� ������ ��� ������ ��������
				SetCanTargeting( true );	// �� ���� ���ʹ� Ÿ������ ���� �ϵ��� ��

			return true;
		}
	}

	return false;	
}

bool CX2BattleFieldNpcAi::CanPreserveLastTargetUnit()
{
	// Ÿ������ ������ �ִµ� ���� Ÿ���� �����ϴ� ���� Ȯ���� ���� ���� �ϸ�
	if ( null == m_optrTargetGameUnit )
		return false;	// Ÿ���� ���� ���

	// ������ Ÿ���� ������ Ȯ��
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->targetData.preserveLastTargetRate ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( RandomInt() < GetAIData()->targetData.preserveLastTargetRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		return true;	// ������ Ÿ���� ���� �ϹǷ� �ٽ� Ÿ���� �� �ʿ� ����
		
	}

	// �ٽ� Ÿ������ ��, �׷��� ResetTarget�� ���� ���� (�ٽ� Ÿ���� ����� ã�� ���� ��)
	return false;
}

void CX2BattleFieldNpcAi::GetTargetListInNearRangeOrRange()
{
	const UINT uiListSize = g_pX2Game->GetUserUnitListSize();
	const D3DXVECTOR3& vMyPos = m_pMasterNPC->GetPos();

	for ( UINT i = 0; i < uiListSize; ++i )
	{
		CX2GameUnit* pGameUnit = g_pX2Game->GetUserUnit( i );

		if ( NULL == pGameUnit )
			continue;

		// ������ �׾����� �н�
		if ( CX2GameUnit::GUSI_DIE == pGameUnit->GetGameUnitState() || 0.0f >= pGameUnit->GetNowHp() )
			continue;

		const TargetData& targetData = GetAIData()->targetData;
		const D3DXVECTOR3& vOtherGameUnitPos = pGameUnit->GetPos();
		// ū�Ϳ��� �������� ���� ���� ���� ����
		const float fDistanceHeight = ( vMyPos.y > vOtherGameUnitPos.y ? vMyPos.y - vOtherGameUnitPos.y : vOtherGameUnitPos.y - vMyPos.y );
		if ( fDistanceHeight > targetData.targetHeightLimit )
			continue;

		// ���� ��� ���ְ��� �Ÿ��� ������ ����
		const float fDistance3Sq = GetDistance3Sq( vMyPos, vOtherGameUnitPos );
		
		// Ÿ�� ���� �Ÿ����� �ָ� �н�
		if ( fDistance3Sq > targetData.targetRange * targetData.targetRange )
			continue;

		// Ÿ���� ������ �Ÿ��� �ִµ� ������ Ŭ��ŷ ���̰�, ���� ��ĵ�� �� �� ������
		if ( pGameUnit->GetInvisibility() && false == GetCanScanCloaking( fDistance3Sq
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            , false
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            ) )
			continue;

		// Ÿ���� ���� �ϴٸ�
		m_vecTargetGameUnitListInRange.push_back( pGameUnit );

		// ������ Ÿ���� ������ �Ÿ����ٵ� �����ٸ�
		if ( fDistance3Sq < targetData.targetNearRange * targetData.targetNearRange )
			m_vecTargetGameUnitListInNearRange.push_back( pGameUnit );		
	}

}

bool CX2BattleFieldNpcAi::CheckAndResetTargetLost()
{
	// Ÿ������ ������ ���� ��쿡�� false�� ����
	if ( null == m_optrTargetGameUnit )
		return false;

	// �׿��� ��쿡�� retrun �ÿ��� Targeting�� �õ� �� �� �ֵ���, m_TargetingInterval�� ������

	// Ÿ���� ���� ���¸�
	if ( CX2GameUnit::GUSI_DIE == m_optrTargetGameUnit->GetGameUnitState() &&
		 0.0f >= m_optrTargetGameUnit->GetNowHp() )
		return true;

	if ( m_vecPlayLineMapIndex.empty() )
		return false;

	// Ÿ�� ���� ������ Ÿ���� �Ҿ������ ���� ������ �������� üũ
	const float fDistance3Sq = GetDistance3Sq( m_pMasterNPC->GetPos(), m_optrTargetGameUnit->GetPos() );
	
	const int& iTargetLostRange = GetAIData()->targetData.targetLostRange;
	if ( fDistance3Sq > iTargetLostRange * iTargetLostRange )
		return true;

	// Ÿ������ ������ Ŭ��ŷ �ε� ���� ��ĵ �� �� ������
	if ( m_optrTargetGameUnit->GetInvisibility() && 
		 GetCanScanCloaking( fDistance3Sq
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
         , true
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
         ) )
		return true;

	return false;
}

void CX2BattleFieldNpcAi::SearchingTargetUnit( IN vector<CX2GameUnit*>& vecTargetCandidateUnitList_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( vecTargetCandidateUnitList_.empty() == true )
        return;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	switch ( GetAIData()->targetData.targetPriority )
	{
	case TP_HIGH_LEVEL_FIRST: 
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// ������ Targeting ���� ������ ���ų�, ������ ���� ���� ���ο� ������ Level�� ������
				if ( null == m_optrTargetGameUnit ||
					 IsHigherNewTargetGameUnitLevel( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_LOW_LEVEL_FIRST:
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// ������ Targeting ���� ������ ���ų�, ������ ���� ���� ���ο� ������ Level�� ������
				if ( null == m_optrTargetGameUnit ||
					 false == IsHigherNewTargetGameUnitLevel( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_HIGH_HP_FIRST:
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// ������ Targeting ���� ������ ���ų�, ������ ���� ���� ���ο� ������ HP�� ������
				if ( null == m_optrTargetGameUnit ||
					 IsHigherNewTargetGameUnitHp( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_LOW_HP_FIRST:
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// ������ Targeting ���� ������ ���ų�, ������ ���� ���� ���ο� ������ HP�� ������
				if ( null == m_optrTargetGameUnit ||
					false == IsHigherNewTargetGameUnitHp( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_NEAR_FIRST:
		{
			float fDistanceSqFromTargetUnit = 0.0f;	// ���� Ÿ���� �� ���ְ��� �Ÿ�
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// ������ Targeting ���� ������ ���ų�, ���ο� Ÿ���� �� ������ ������
				if( null == m_optrTargetGameUnit ||
					IsNearNewTargetGameUnit( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_, fDistanceSqFromTargetUnit ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;				
			}
		} break;

	case TP_FAR_FIRST:
		{
			float fDistanceSqFromTargetUnit = 0.0f;	// ���� Ÿ���� �� ���ְ��� �Ÿ�
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// ������ Targeting ���� ������ ���ų�, ���ο� Ÿ���� �� �ָ� ������
				if( null == m_optrTargetGameUnit ||
					IsFarNewTargetGameUnit( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_, fDistanceSqFromTargetUnit ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;				
			}
		} break;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    default:
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	case TP_RANDOM:
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            float fRate = 0.f;
            BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
            {
                fRate = fRate * 0.5f + 0.5f;
            }
            if ( CX2GUNPC::EstimateFrameOneshotPercent( fRate * 100.f ) == true )
            {
                if ( vecTargetCandidateUnitList_.size() == 1 )
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_.front();
                else
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_[ rand() % vecTargetCandidateUnitList_.size() ];
            }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				if ( RandomInt() < 50 )
				{
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
					break;
				}
			}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;

	case TP_ONLY_ONE:
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if ( m_optrTargetGameUnit == null )
            {
                if ( vecTargetCandidateUnitList_.size() == 1 )
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_.front();
                else
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_[ rand() % vecTargetCandidateUnitList_.size() ];
            }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				if ( null == m_optrTargetGameUnit )
				{
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
					break;
				}
			}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;
	}
}

bool CX2BattleFieldNpcAi::IsHigherNewTargetGameUnitLevel( const CX2GameUnit* pOldTargetGameUnit_, const CX2GameUnit* pNewTargetGameUnit_ )
{
	if( pOldTargetGameUnit_->GetUnitLevel() < pNewTargetGameUnit_->GetUnitLevel() )
		return true;
	else
		return false;
}

bool CX2BattleFieldNpcAi::IsHigherNewTargetGameUnitHp( const CX2GameUnit* pOldTargetGameUnit_, const CX2GameUnit* pNewTargetGameUnit_ )
{
	if( pOldTargetGameUnit_->GetNowHp() < pNewTargetGameUnit_->GetNowHp() )
		return true;
	else
		return false;
}

bool CX2BattleFieldNpcAi::IsNearNewTargetGameUnit( IN const CX2GameUnit* pOldTargetGameUnit_, IN const CX2GameUnit* pNewGameUnit_, IN OUT float& fDistanceSqFromOldTargetUnit_ )
{
	// ���ο� Ÿ�� ������ �Ÿ��� ���� ��
	const float fDistanceSqFromNewTargetGameUnit = GetDistance3Sq( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );
	const float fDistance = GetDistance( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );

	// ������ Ÿ���� ���ų�, ���ο� Ÿ�� �ĺ��� �� ������
	if( fDistanceSqFromNewTargetGameUnit < fDistanceSqFromOldTargetUnit_ )
	{
		fDistanceSqFromOldTargetUnit_ = fDistanceSqFromNewTargetGameUnit;
		return true;
	}
	else
		return false;
}

bool CX2BattleFieldNpcAi::IsFarNewTargetGameUnit( IN const CX2GameUnit* pOldTargetGameUnit_, IN const CX2GameUnit* pNewGameUnit_, IN OUT float& fDistanceSqFromOldTargetUnit_ )
{
	// ���ο� Ÿ�� ������ �Ÿ��� ���� ��
	const float fDistanceSqFromNewTargetGameUnit = GetDistance3Sq( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );
	const float fDistance = GetDistance( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );

	// ������ Ÿ���� ���ų�, ���ο� Ÿ�� �ĺ��� �� �ָ�
	if( fDistanceSqFromNewTargetGameUnit > fDistanceSqFromOldTargetUnit_ )
	{
		fDistanceSqFromOldTargetUnit_ = fDistanceSqFromNewTargetGameUnit;
		return true;
	}
	else
		return false;
}

bool CX2BattleFieldNpcAi::CheckOnPatrolLineMap()
{
	const int iLastTouchedLineMapIndex = m_pMasterNPC->GetLastTouchLineIndex();

	BOOST_FOREACH( const int& iPatrolLineMapIndex, m_vecPatrolLineMapIndex )
	{
		// Patrol LineMap ���� ������
		if ( iLastTouchedLineMapIndex == iPatrolLineMapIndex )
			return true;
	}

	// Patrol LineMap ���� ������
	return false;
}

bool CX2BattleFieldNpcAi::CheckOnPlayLineMap()
{
	const int iLastTouchedLineMapIndex = m_pMasterNPC->GetLastTouchLineIndex();

	BOOST_FOREACH( const int& iPlayLineMapIndex, m_vecPlayLineMapIndex )
	{
		// Play LineMap ���� ������
		if ( iLastTouchedLineMapIndex == iPlayLineMapIndex )
			return true;
	}

	// Play LineMap ���� ������
	return false;
}

/*virtual*/ void CX2BattleFieldNpcAi::Moving( float fElapsedTime )
{
	CX2GUNPC::STATE_CHANGE_TYPE eSctMessage = CX2GUNPC::SCT_NO_CHANGE;

	if ( null != m_optrTargetGameUnit )
		SetFinalDestinationPosAndLineMapIndex( m_optrTargetGameUnit->GetPos(), m_optrTargetGameUnit->GetLastTouchLineIndex() );

	bool bAiIsRight = false;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	    GetAIMessageAndUnitDirection( fElapsedTime, OUT eSctMessage, OUT bAiIsRight );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		GetAIMessageAndUnitDirection( OUT eSctMessage, OUT bAiIsRight );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// ���� AI �޽����� ���� AI �޽����� �ٸ��� ���ٸ� SCT_NO_CHANGE
	if( m_PrevAIMessage == eSctMessage || CX2GUNPC::SCT_NO_CHANGE == eSctMessage )
	{
		eSctMessage = CX2GUNPC::SCT_NO_CHANGE;
	}
	else	// ���� AI�� �ٸ��ٸ� (������ȯ�� ��� SCT_AI_DIR_CHANGE �� ���� �ȴ�.)
	{
		m_PrevAIMessage = eSctMessage;
		m_pMasterNPC->PushAIMessage( eSctMessage, bAiIsRight );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::GetAIMessageAndUnitDirection( float fElapsedTime, OUT CX2GUNPC::STATE_CHANGE_TYPE& eSctMessage_, OUT bool& bAilsRight_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::GetAIMessageAndUnitDirection( OUT CX2GUNPC::STATE_CHANGE_TYPE& eSctMessage_, OUT bool& bAilsRight_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	ASSERT( NULL != pLineMap );

	// �ϴ� �ʵ� ���ʹ� LineGroup ����
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	const float& fElapsedTime = g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

// 	m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
// 	if ( m_fElapsedTimeAfterLastRandomJump > GetAIData()->chaseMoveData.jumpInterval )
// 	{
// 		m_fElapsedTimeAfterLastRandomJump = 0.0f;
// 		const float fJumpChance = RandomFloat( 0.0f, 100.0f );
// 
// 		if ( GetFinalDest().y > GetNowPosition().y )
// 		{
// 			if ( fJumpChance < GetAIData()->chaseMoveData.upJumpRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_JUMP;
// 			else if ( 100 - fJumpChance < GetAIData()->chaseMoveData.upDownRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_DOWN;
// 		}
// 		else
// 		{
// 			if( fJumpChance < GetAIData()->chaseMoveData.downJumpRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_JUMP;
// 			else if( 100 - fJumpChance < GetAIData()->chaseMoveData.downDownRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_DOWN;
// 		}
// 
// 		return;
// 	}

	bool bIsTargetOnRight = false;

	// ������ġ(m_vPosition)�� Ÿ���� �����ʿ� �ִ���(bIsTargetOnRight)�� �˾ƿ�
// 	m_PathFinder.FollowTargetPath( GetNowPosition(), GetLastTouchLineIndex(), GetFinalDest(), GetFinalDestLineIndex(),
// 		GetMaxJumpUp(), GetMaxJumpRight(), pLineMap, static_cast<float>( GetAIData()->chaseMoveData.destGap ), 
// 		static_cast<float>( GetAIData()->chaseMoveData.moveGap ), GetFootOnLine(), false, OUT bIsTargetOnRight, 
// 		GetAIData()->chaseMoveData.m_fLineEndDetectRange );
	m_PathFinder.FollowTargetPathInBattleField( this, pLineMap, bIsTargetOnRight );

	if( IsRight() != bIsTargetOnRight )
		m_bIsTargetFront = false;
	else
		m_bIsTargetFront = true;
	
	bAilsRight_ = IsRight();
	switch ( m_PathFinder.GetMoveState() )
	{
	case CX2SimplePathFinder::MS_WAIT:	
		{
			eSctMessage_ = CX2GUNPC::SCT_AI_WAIT;
		} break;

		/// �������� ����� �Ҷ�
	case CX2SimplePathFinder::MS_FORCE_LEFT:	
	case CX2SimplePathFinder::MS_LEFT:
		{
			bAilsRight_ = false;

			if( true == IsRight() )	// ���� �������� ���� �ְ�
			{
				// ���� AI �޽����� wait, walk, dash �� �ϳ����ٸ�
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{	
					// CHASE_MOVE�� DIR_CHANGE_INTERVAL üũ
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > GetAIData()->chaseMoveData.dirChangeInterval ) 
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						eSctMessage_ = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			// ������ ���� �־��ٰų�, �������� ���� �־����� wait, walk, dash�� �ƴϾ��ٸ� walk �϶�� AI�� ����
			eSctMessage_ = CX2GUNPC::SCT_AI_WALK;
		} break;

	case CX2SimplePathFinder::MS_FORCE_RIGHT:
	case CX2SimplePathFinder::MS_RIGHT: 
		{
			bAilsRight_ = true;

			if( false == IsRight() )
			{
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > GetAIData()->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						eSctMessage_ = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			eSctMessage_ = CX2GUNPC::SCT_AI_WALK;			
		} break;

	case CX2SimplePathFinder::MS_JUMP:
		{
			eSctMessage_ = CX2GUNPC::SCT_AI_JUMP;
		} break;

	case CX2SimplePathFinder::MS_DOWN:
		{
			eSctMessage_	= CX2GUNPC::SCT_AI_DOWN;
		} break;

	case CX2SimplePathFinder::MS_JUMP_LEFT:
		{
			bAilsRight_ = false;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_JUMP_RIGHT:
		{
			bAilsRight_ = true;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_LEFT:
		{
			bAilsRight_ = false;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_RIGHT:
		{
			bAilsRight_ = true;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	default:
		ASSERT( !L"Assert ó���� �ؾ��ϳ�..?" );
		break;
	}	// switch

	// walk, dash �߿� walk, dash���� ���º�ȭ�� CHASE_MOVE�� WALK_INTERVAL �������� ����
	if( ( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage || CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
		CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage ) &&
		( CX2GUNPC::SCT_AI_WALK == eSctMessage_ || CX2GUNPC::SCT_AI_DASH == eSctMessage_ ) )
	{
		m_fElapsedTimeAfterLastWalkRunUpdate += fElapsedTime;

		/// walkInterval ��ŭ ��� ������
		if( m_fElapsedTimeAfterLastWalkRunUpdate > GetAIData()->chaseMoveData.walkInterval )
		{	
			m_fElapsedTimeAfterLastWalkRunUpdate = 0.f;

			// ������ġ�� ������ ��ġ ������ �Ÿ��� MOVE_SPLIT_RANGE ���� ������
			const float fFinalDestDistance3Sq = GetDistance3Sq( GetNowPosition(), GetFinalDest() );
			const float fDistance = GetDistance( GetNowPosition(), GetFinalDest() );

			if( fFinalDestDistance3Sq < GetAIData()->chaseMoveData.moveSplitRange * GetAIData()->chaseMoveData.moveSplitRange )
			{
				// NEAR_WALK_RATE Ȯ���� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->chaseMoveData.nearWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < GetAIData()->chaseMoveData.nearWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_WALK;	// �ȴ���..
				}
				else
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_DASH;	// �ٴ���..
				}
			}
			else	// MOVE_SPLIT_RANGE ���� ���� ������
			{
				// FAR_WALK_RATE Ȯ���� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->chaseMoveData.farWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < GetAIData()->chaseMoveData.farWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_WALK;	// �ȴ���..
				}
				else
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_DASH;	// �ٴ���..
				}
			}		
		}
		else
		{
			eSctMessage_ = m_PrevAIMessage;	// ������ AI�� �״�� ���
		}
	}
}

void CX2BattleFieldNpcAi::CheckArrivedDestination()
{
	const float fDistance3Sq = GetDistance3Sq( GetNowPosition(), GetFinalDest() );
	
	const int iDestinationGap = GetAIData()->chaseMoveData.destGap;

	if ( fDistance3Sq < iDestinationGap * iDestinationGap )
	{
		// �������� ����
		if ( true == GetBackState01() )
		{
			SetBackState01( false );
			SetIncreaseHpPercentBySecond( BASIC_INCREASE_HP_PERCENT_BY_SECOND );
		}

		if ( true == GetBackState02() )
		{
			SetBackState02( false );
			m_pMasterNPC->SetIsComeBackState( false );
			SetIncreaseHpPercentBySecond( BASIC_INCREASE_HP_PERCENT_BY_SECOND );
// 			SetIncreaseMoveSpeedPercentAtComeBackState( 0.0f );
// 			SetIncreaseAnimSpeedPercentAtComeBackState( 0.0f );
		}

		if ( GetNowPatroling() )
			SetNowPatroling( false );
	}
}

/*virtual*/ void CX2BattleFieldNpcAi::RenderStateParam( WCHAR** wDest_ )
{
// 	int iTarget = 0;
// 	if ( null != m_optrTargetGameUnit )
// 	{
// 		StateLog( L"Target in RenderStateParam" );
// 		iTarget = 1;
// 	}
// 
// 	int iAttacker = 0;
// 	if ( null != m_optrAttackerGameUnit )
// 	{
// 		StateLog( L"Attacker in RenderStateParam" );
// 		iAttacker = 1;
// 	}
// 
// 	int iBackState01 = 0;
// 	if ( true == GetBackState01() )
// 		iBackState01 = 1;
// 
// 	int iBackState02 = 0;
// 	if ( true == GetBackState02() )
// 		iBackState02 = 1;

 	swprintf( *wDest_, L"DestLineIndex: %d",// Target: %d, Attacker: %d, BackState01: %d, BackState02: %d, CheckNum: %d",
 		GetFinalDestLineIndex() );//, iTarget, iAttacker, iBackState01, iBackState02,	m_iCheckLineMapAtFirst );

//	swprintf( *wDest_, L"GroupID: %d", m_uiGroupId );
}

/*virtual*/ void CX2BattleFieldNpcAi::RenderPatrolLineIndex( WCHAR** wDest_ )
{
	swprintf( *wDest_, L"Patrol: " );
	BOOST_FOREACH( int index, m_vecPatrolLineMapIndex)
	{
		WCHAR wSource[10] = { 0, };
		swprintf( wSource, L"%d,", index );
		StrCatW( *wDest_, wSource );
	}
}

/*virtual*/ void CX2BattleFieldNpcAi::RenderPlayLineIndex( WCHAR** wDest_ )
{
	swprintf( *wDest_, L"Play: " );
	BOOST_FOREACH( int index, m_vecPlayLineMapIndex )
	{
		WCHAR wSource[10] = { 0, };
		swprintf( wSource, L"%d,", index );
		StrCatW( *wDest_, wSource );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::UpNowHpBySecondProcess( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::UpNowHpBySecondProcess()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	//m_ElapsedTimeUpNowHp.OnFrameMove();
	//if ( m_ElapsedTimeUpNowHp.CheckAndResetElapsedTime() )
#ifdef FIELD_BOSS_RAID // ���� NPC�� ü�� ȸ�� ����
	if( false == m_pMasterNPC->GetIsBosRaidNPC() )
#endif // FIELD_BOSS_RAID
	{
		const float fMaxHp = m_pMasterNPC->GetMaxHp();
		const float fNowHp = m_pMasterNPC->GetNowHp();
		if ( fNowHp < fMaxHp && 0.0f < fNowHp )
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_pMasterNPC->UpNowHp( fMaxHp * GetIncreaseHpPercentBySecond() * fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_pMasterNPC->UpNowHp( fMaxHp * GetIncreaseHpPercentBySecond() * g_pKTDXApp->GetElapsedTime() );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
	}
}
