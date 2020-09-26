#pragma once

typedef srutil::delegate0<void> GUStateCBFunc;

typedef srutil::delegate1<void, CKTDGLineMap*> DelegatePhysicProcessPortalByGameType;

//{{ kimhc // 2010.11.3 // ���� - ��ȭ�� �г�
#ifdef	NEW_SKILL_2010_11

// ������ �� �ִ��� (�簢�� �� �� �ִ��� ���� �߰�)
#define ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( hyperModeState ) \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) ) \
	{ StateChange( hyperModeState ); }

#else

// ������ �� �ִ���
#define ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( hyperModeState ) \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && GetRemainHyperModeTime() <= 0.0f ) \
	{ StateChange( hyperModeState ); }

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.3 // ���� - ��ȭ�� �г�


// kimhc // 2010-11-30 // �޺� �� ����Ű�� ����Ͽ� ĵ�� ���� ���θ� �Ǵ� (�ݴ� �������θ� ĵ�� ����)
#define CAN_WALK_CANCEL \
	( ( true == m_InputData.pureRight &&  false == m_FrameDataFuture.syncData.bIsRight ) || \
		( true == m_InputData.pureLeft && true == m_FrameDataFuture.syncData.bIsRight ) )

// kimhc // 2010-11-27 // �޺� �� ��ø� ����Ͽ� ĵ�� ���� ���θ� �Ǵ�
#define CAN_DASH_CANCEL \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )

//{{������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����
// Ư�� �ִϸ��̼� �ð��� ���� �Ŀ� ����Ű, ZXASDCQWER�� ������ �� (QWER�߰�)
#define ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureZ == true || m_InputData.pureX == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true) )



// Ư�� �ִϸ��̼� �ð��� ���� �Ŀ� ����Ű, ASDCQWER�� ������ �� (QWER�߰�)
#define ELSE_IF_ARROW_ASDC_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true) )





// Ư�� �ִϸ��̼� �ð��� ���� �Ŀ� ASDCQWER�� ������ �� (QWER�߰�)
#define ELSE_IF_ASDC_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true) )



// Ư�� �ִϸ��̼� �ð��� ���� �Ŀ� ZX�� ������ ��
#define ELSE_IF_ZX_PRESSED_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	(m_InputData.pureZ == true || m_InputData.pureX == true) )

//}}������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����

#ifdef ELSWORD_SHEATH_KNIGHT
#define BWALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
	( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) ) \
	{ StateChange( USI_WAIT ); }

#define WALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_WAIT ); }

#define WALK_CANCEL_AFTER_CANNOT_ATTACKTIME( time, cannotattacktime ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_WAIT ); m_fCanNotAttackTime = cannotattacktime; }

#define DASH_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }
	
#ifdef SKILL_CANCEL_BY_HYPER_MODE // ����ȯ

	/// ��ų ĵ�� ��ũ�ο� ���� ĵ���� �߰�
	#define SKILL_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ } \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) && \
	(time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) ) \
	{ StateChange( GetHypetMpdeStateID() ); }

	/// if�� �����ϴ� ��ų ĵ�� ��ũ�ΰ� �ʿ��Ͽ� �߰� ( ���� ��ų ĵ�� ��ũ�ο� ���� ���� )
	#define IF_SKILL_CANCEL_AFTER( time ) \
	if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ } \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	m_InputData.oneLCtr == true && m_FrameDataFuture.syncData.m_HyperModeCount > 0 && \
	( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) && \
	(time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) ) \
	{ StateChange( GetHypetMpdeStateID() ); }

#else // SKILL_CANCEL_BY_HYPER_MODE

	#define SKILL_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ }
#endif //SKILL_CANCEL_BY_HYPER_MODE

// Ư�� �ִϸ��̼� �ð��� ���� �Ŀ� ZX�� ������ ���� ��
#define ZX_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) )\
	{ if ( m_InputData.pureZ == true ) StateChange( GetComboZStateID() ); \
      else if ( m_InputData.pureX == true ) StateChange( GetComboXStateID() ); }

// Ư�� �ִϸ��̼� �ð��� ���� �Ŀ� ZX�� ������ ��( OneX, OneZ )
#define ZX_CANCEL_AFTER_EX( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) )\
	{ if ( m_InputData.oneZ == true ) StateChange( GetComboZStateID() ); \
	  else if ( m_InputData.oneX == true ) StateChange( GetComboXStateID() ); }
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_RAVEN_VETERAN_COMMANDER		/// ���߿��� ����� �� �ִ� ��ų�� ���ؼ��� ĵ���� �� �ִ�.

#define SKILL_CANCEL_AFTER_FLY( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) == true ) \

#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef RIDING_SYSTEM
#define RIDING_BWALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
	( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) ) \
	{ StateChange( USI_RIDING_WAIT ); }

#define RIDING_WALK_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_RIDING_WAIT ); }

#define RIDING_WALK_CANCEL_AFTER_CANNOT_ATTACKTIME( time, cannotattacktime ) \
	else if( (time) >= 0.f NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureRight || true == m_InputData.pureLeft ) ) \
	{ StateChange( USI_RIDING_WAIT ); m_fCanNotAttackTime = cannotattacktime; }

#define RIDING_DASH_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft ) ) \
	{ StateChangeDashIfPossible(); }

#define RIDING_SKILL_CANCEL_AFTER( time ) \
	else if( (time) >= 0.f && NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && \
	m_ptrRidingPet->GetXSkinAnimFuturePtr()->GetNowAnimationTime() > (time) && \
	RidingPetSpecialAttackEventProcess() == true ) \
	{ }
#endif //RIDING_SYSTEM

////////// �� �������� ��ŵ Define�� ���� ���̹Ƿ� �ǵ��� ���� ���� ������ּ���!! //////////////



//{{������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����
// ��ŵ ����Ʈ�� �ʻ�� Ű�� ���������� (QWER�߰�)
#define ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C \
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true )



// ��ŵ ����Ʈ�� ZX �ʻ�� (QWER�߰�)
#define ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC \
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureZ == true || m_InputData.pureX == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true )



// ��ŵ ����Ʈ�� ZX �ʻ��, ����, ��, �ƹ�Ű�� �� (QWER�߰�)
#define ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS \
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true || m_InputData.pureRight == true || m_InputData.pureLeft == true \
	|| m_InputData.pureZ == true || m_InputData.pureX == true \
	|| m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true \
	|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true \
	|| m_InputData.pureLCtr == true )
//}}������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����


// ������ �ٲ�� ���Էµ� �޺��� ��ҽ�Ų��
#define CANCEL_COMBO_ON_DIRECTION_CHANGE( eventFlagCount ) \
	if( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || \
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) ) \
	{ for( int i=0; i< (int)(eventFlagCount); i++ ) m_FrameDataFuture.stateParam.bEventFlagList[i] = false; } 



// 0~������ �ð����̿� Ư�� ������ �����ϸ� flag = true�� �����, ������ �ð��� �Ǿ��� �� flag == true�̸� ������ ���·� �ٲ��
#define ELSE_IF_STATE_CHANGE_ON_( iEventFlagIndex, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }

#ifdef ELSWORD_SHEATH_KNIGHT
// 0~������ �ð����̿� Ư�� ������ �����ϸ� flag = true�� �����, ������ �ð��� �Ǿ��� �� flag == true�̸� ������ ���·� �ٲ��
#define IF_STATE_CHANGE_ON_EX_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#define ELSE_IF_STATE_CHANGE_ON_EX_( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif ELSWORD_SHEATH_KNIGHT

#ifdef NEW_CHARACTER_EL
#define IF_STATE_CHANGE_ON_EX_STRUCT( iEventFlagIndex, iEventTImeIndex, bCondition, iStateID ) \
	if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(m_fEventTime[iEventTImeIndex].stateChange) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(m_fEventTime[iEventTImeIndex].keyInputEnd) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(m_fEventTime[iEventTImeIndex].keyInputStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#define ELSE_IF_STATE_CHANGE_ON_EX_STRUCT( iEventFlagIndex, iEventTImeIndex, bCondition, iStateID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(m_fEventTime[iEventTImeIndex].stateChange) ) \
	{ StateChange( (iStateID) ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(m_fEventTime[iEventTImeIndex].keyInputEnd) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(m_fEventTime[iEventTImeIndex].keyInputStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
// 0~������ �ð����̿� Ư�� ������ �����ϸ� flag = true�� �����
#define ELSE_IF_SET_EVENT_FLAG( iEventFlagIndex, fInputTimingStart, fInputTiming, bCondition ) \
	else if( true == (bCondition) && m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) ) \
	{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }
#endif // SERV_ARA_CHANGE_CLASS_SECOND

// ���߿� �� �ִ� �ð��� �����ð� �̻�Ǹ� wait ���·� �ٲ۴�
//#ifdef LINEMAP_FAST_WIND_TEST
//	#define IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( time, targetState ) \
//		if( false == m_bWaitInTheAir ) { m_bWaitInTheAir = true; m_fTimeInTheAir = (time); } \
//		else if( 0.f == m_fTimeInTheAir ) StateChange( (targetState) ); 
//#endif LINEMAP_FAST_WIND_TEST

#define MAGIC_OXYGEN_GAGE	200.f


#ifdef X2OPTIMIZE_NPC_NONHOST_SIMULATION
class CX2GUNPC;
#endif // X2OPTIMIZE_NPC_NONHOST_SIMULATION



#ifdef SUPERPOSITION_HYPER_MODE_BUG
#define IF_HYPER_MODE_STATE( stateID_ ) \
	if( USI_HYPER_MODE == stateID_ || USI_RIDING_HYPER_MODE == stateID_ )
#endif // SUPERPOSITION_HYPER_MODE_BUG


class CX2GUUser : public CX2GameUnit
{
public:
	//{{ Public Variable Start
#ifdef KHELP_SYSTEM_TEST
	DECLARE_HELP_SYSTEM( CX2GUUser );
#endif KHELP_SYSTEM_TEST

	void Debug_RenderHelp( int ix_, int iy_ );

	enum USER_STATE_ID
	{
		USI_BASE = GUSI_END + 1,

		USI_DIE_FRONT,						/// ������ �������� �״� ������Ʈ
		USI_DIE_BACK,						/// �ڷ� �������� �״� ������Ʈ
		
		USI_START,							/// ����, ���� ������ ���� ������Ʈ
		USI_START_INTRUDE,					/// ���� ��� �߰��� ���� ���� ���� ������Ʈ
		
		USI_WIN,							/// �¸� ���� ���� ������Ʈ
		USI_LOSE,							/// �й� ���� ���� ������Ʈ

		USI_WAIT,							/// ������ �� �ִ� ������Ʈ
		USI_WALK,							/// �ȴ� ������Ʈ
		USI_JUMP_READY,						/// ���� �� ������Ʈ
		USI_JUMP_UP,						/// ���� �� ������Ʈ
		USI_JUMP_DOWN,						/// ���� �� ��, ���� �� �ϰ��ϴ� ������Ʈ
		USI_JUMP_LANDING,					/// ���� �� ������Ʈ
		USI_DASH,							/// ����ϴ� ������Ʈ
		USI_DASH_END,						/// ����ϴٰ� �����ϴ� ������Ʈ
		USI_DASH_JUMP,						/// ��� ���� �ϴ� ������Ʈ
		USI_DASH_JUMP_LANDING,				/// ��� ���� �� ���� �ϴ� ������Ʈ
				
		USI_HYPER_MODE,						/// ���� �ϴ� ������Ʈ

		USI_DAMAGE_GROGGY,					/// HP�� ���� �Ҹ�Ǿ� ������ϴ� ������Ʈ
		USI_DAMAGE_SMALL_FRONT,				/// ���濡�� SMALL_TYPE�� ������ �¾��� ���� ������Ʈ
		USI_DAMAGE_SMALL_BACK,				/// �Ĺ濡�� SMALL_TYPE�� ������ �¾��� ���� ������Ʈ
		USI_DAMAGE_BIG_FRONT,				/// ���濡�� BIG_TYPE�� ������ �¾��� ���� ������Ʈ
		USI_DAMAGE_BIG_BACK,				/// �Ĺ濡�� BIG_TYPE�� ������ �¾��� ���� ������Ʈ
		USI_DAMAGE_DOWN_FRONT,				/// ���濡�� ������ �޾� DOWN �Ǿ��� ���� ������Ʈ
		USI_DAMAGE_DOWN_BACK,				/// �Ĺ濡�� ������ �޾� DOWN �Ǿ��� ���� ������Ʈ
		USI_DAMAGE_STANDUP_FRONT,			/// ���濡�� ������ �޾Ƽ� DOWN�� �Ŀ� �Ͼ�� ������Ʈ
		USI_DAMAGE_STANDUP_BACK,			/// �Ĺ濡�� ������ �޾Ƽ� DOWN�� �Ŀ� �Ͼ�� ������Ʈ
		USI_DAMAGE_AIR_SMALL,				/// ���߿� �� �ִ� ���¿��� SMALL_TYPE�� ������ �޾��� ���� ������Ʈ
		USI_DAMAGE_AIR_DOWN,				/// ���߿� �� �ִ� ���¿��� DOWN_TYPE�� ������ �޾��� ���� ������Ʈ
		USI_DAMAGE_AIR_DOWN_INVINCIBLE,		/// �������� ���� �̻� �޾Ƽ� DOWN �Ǵ� ������Ʈ (DOWN ���ݿ� ���ؼ��� �ƴ�)
		USI_DAMAGE_AIR_DOWN_LANDING,		/// ���߿��� DOWN�Ǿ� �������� ������Ʈ
		USI_DAMAGE_AIR_FALL,				/// UP_TYPE�� ���ݿ� ���� �������� �ö󰬴ٰ� �������� ������Ʈ
		USI_DAMAGE_AIR_UP,					/// UP_TYPE�� ���ݵ ���Ͽ� �������� �ö󰡴� ������Ʈ
		USI_DAMAGE_AIR_FLY_FRONT,			/// ���濡�� FLY_TYPE ������ �޾��� ���� ������Ʈ
		USI_DAMAGE_AIR_FLY_BACK,			/// ���濡�� FLY_TYPE ������ �޾��� ���� ������Ʈ
		USI_DAMAGE_REVENGE,					/// �����ߴٰ� ���ҵ��� �ݰ��� �޾��� �� ����Ǵ� ������Ʈ
		USI_DAMAGE_GRAPPLED_FRONT,			/// �系���� ���� ��
		USI_RIDING_HYPER_MODE,				/// Ż �� ž�� �� ����

		USI_PEPPER_RUN_READY,				/// û����� ������ ��� ������Ʈ
		USI_PEPPER_RUN,						/// 
		USI_PEPPER_RUN_END,					/// 
		USI_PEPPER_RUN_JUMP_UP,				/// 
		USI_PEPPER_RUN_JUMP_DOWN,			/// 

		USI_SPECIAL_ATTACK_1,				/// A������ ù��° ��ų����(�����AŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_HYPER_1,			/// A������ ù��° ��ų����(�����AŰ)�� �ش��ϴ� ���� ������Ʈ
		USI_SPECIAL_ATTACK_2,				/// A������ �ι�° ��ų����(�����SŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_HYPER_2,			/// A������ �ι�° ��ų����(�����SŰ)�� �ش��ϴ� ���� ������Ʈ
		USI_SPECIAL_ATTACK_3,				/// A������ ����° ��ų����(�����DŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_HYPER_3,			/// A������ ����° ��ų����(�����DŰ)�� �ش��ϴ� ���� ������Ʈ
		USI_SPECIAL_ATTACK_4,				/// A������ �׹�° ��ų����(�����CŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_HYPER_4,			/// A������ �׹�° ��ų����(�����CŰ)�� �ش��ϴ� ���� ������Ʈ

		USI_SPECIAL_ATTACK_SLOTB_1,			/// B������ ù��° ��ų����(�����AŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_HYPER_1,	/// B������ ù��° ��ų����(�����AŰ)�� �ش��ϴ� ���� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_2,			/// B������ �ι�° ��ų����(�����SŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_HYPER_2,	/// B������ �ι�° ��ų����(�����SŰ)�� �ش��ϴ� ���� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_3,			/// B������ ����° ��ų����(�����DŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_HYPER_3,	/// B������ ����° ��ų����(�����DŰ)�� �ش��ϴ� ���� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_4,			/// B������ �׹�° ��ų����(�����CŰ)�� �ش��ϴ� ������Ʈ
		USI_SPECIAL_ATTACK_SLOTB_HYPER_4,	/// B������ �׹�° ��ų����(�����CŰ)�� �ش��ϴ� ���� ������Ʈ

		//#ifdef SPECIAL_USE_ITEM
		USI_THROW_ITEM,						/// ������� ������, ������
		USI_INSTALL_ITEM,					/// ������� ������, ��ġ
		//#endif SPECIAL_USE_ITEM

#ifdef RIDING_SYSTEM			// ���볯¥: 2013-04-02
		USI_RIDING_START,				/// Ÿ�� �ִ� ���¿��� ���� ����
		USI_RIDING_ON,					/// Ÿ��
		USI_RIDING_OFF,					/// ������
		USI_RIDING_WAIT_HABIT,			/// ����
		USI_RIDING_WAIT,				/// Ÿ�� ����
		USI_RIDING_WALK,				/// �ȱ�
		USI_RIDING_JUMP_UP,				/// ���� ��
		USI_RIDING_JUMP_DOWN,			/// ���� �ٿ�
		USI_RIDING_JUMP_LANDING,		/// ���� ����
		USI_RIDING_DASH,				/// ���
		USI_RIDING_DASH_END,			/// ��� ����
		USI_RIDING_DASH_JUMP,			/// ��� ����
		USI_RIDING_DASH_JUMP_LANDING,	/// �뽬 ���� ����
		USI_RIDING_DAMAGE_FRONT,		/// �ǰ� ��
		USI_RIDING_DAMAGE_BACK,			/// �ǰ� ��
		USI_RIDING_DIE,					/// Ÿ�� �״� ����

		USI_RIDING_ATTACK_Z,			/// Z ����
		USI_RIDING_JUMP_ATTACK_Z,		/// ���� Z ����
		USI_RIDING_ATTACK_X,			/// X ����
		USI_RIDING_SPECIAL_ATTACK,		/// Ư�� ����
		USI_RIDING_SPECIAL_MOVE,		/// Ư�� �̵�

//#ifdef MODIFY_RIDING_PET_AWAKE
		//������Ʈ 255�� �ʰ� ���� ������ �ӽ� ����
		//USI_RIDING_HYPER_MODE,			/// ����
//#endif // MODIFY_RIDING_PET_AWAKE

#endif //RIDING_SYSTEM

		USI_END,
	};

#ifdef NEW_SKILL_TREE
		enum SPECIAL_ATTACK_KEY_PRESSED
		{
			SAKP_NONE,
			SAKP_A,
			SAKP_S,
			SAKP_D,
			SAKP_C,
			SAKP_Q,
			SAKP_W,
			SAKP_E,
			SAKP_R,
		};
#endif NEW_SKILL_TREE

		// ���, ���º�ȭ ���� �ٲ�� �� sync 
		enum USER_ACTION_ID
		{
			UAI_NONE = 0,
			UAI_ENDURANCE,
#ifdef SKILL_30_TEST
			UAI_MANA_SHIELD,
			UAI_AHM_MEDITATION_START,
			UAI_AHM_MEDITATION_STOP,
#endif SKILL_30_TEST
			UAI_SIEGE_FIRE_ANGLE_UP,
			UAI_SIEGE_FIRE_ANGLE_DOWN,
			UAI_MEGA_ELECTRONBALL,
			UAI_MEGA_ELECTRONBALL_CHARGE_FAIL,

			UAI_ELEMENTAL_FRIENDSHIP_UP,

			//{{ kimhc // 2010.11.1 // �Ž�ų
#ifdef	NEW_SKILL_2010_11
			UAI_REDUCE_PAIN,				/// �ε峪��Ʈ - ���� ����
			UAI_ENDURANCE_AND_REDUCE_PAIN,	/// �ε峪��Ʈ - �ر� �� ��������� ���� �ߵ� �Ǵ� ��� (�ӽ�)
			UAI_CHARGING_BOOSTER,	// ���� - ���� ������
#endif	NEW_SKILL_2010_11
			//}} kimhc // 2010.11.1 // �Ž�ų
#ifdef EVE_ELECTRA
			UAI_THOUSANDS_OF_STARS,
			UAI_ELECTRA_PLANE_ANGLE_UP,
			UAI_ELECTRA_PLANE_ANGLE_DOWN,
#endif EVE_ELECTRA

#ifdef SERV_RENA_NIGHT_WATCHER
			UAI_START_OF_DELAYED_FIRING,	/// ������ ��ȣź ��ü ����
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_EVE_BATTLE_SERAPH
			UAI_TASER_PILUM,
			UAI_TASER_PILUM_CHARGE_FAIL,
			UAI_CHANGE_SPECTRO_WITHOUT_MOTION,
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			UAI_AHM_SHINING_BODY,			/// ���̴� �ٵ�
			UAI_AVP_SHADOW_BODY,			/// ������ �ٵ�
			UAI_ADW_ENERGETIC_BODY,			/// ������ƽ �ٵ�

			UAI_KUGELBLITZ,					/// ��ۺ���
			UAI_KUGELBLITZ_CHARGE_FAIL,		/// ��ۺ��� ���� ����
#endif //UPGRADE_SKILL_SYSTEM_2013

			UAI_ENDURANCE_POWER,			/// �ر� - ��(˭)

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			UAI_REFLECTION,					/// ��ź��
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef FINALITY_SKILL_SYSTEM //��â��
			UAI_EEP_LINK_OVERCHARGE_ILLUSION,	// �������� �ñر� - ��ũ �������� �����Ʈ
#endif //FINALITY_SKILL_SYSTEM
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			UAI_RELEASE_FORMATION_MODE,		/// �ֵ� ���� ��� ����
			UAI_ACTIVE_STASIS_FIELD,		/// �ֵ� �����̽ý� �ʵ� �ߵ�
#endif //SERV_9TH_NEW_CHARACTER

		};

		// kimhc // 2010.11.3 ��� ������ �������� ���� ������ ��ƼŬ
		enum COMMON_MAJOR_PARTICLE_INSTANCE_ID
		{
			COMMON_MAJOR_PII_REVENGE_IMPACT_SLASH_CW,		/// �ݰݱ⿡ ���ϱ� ������ ����� ��ƼŬ(�ð����)
			COMMON_MAJOR_PII_REVENGE_IMPACT_SLASH_CCW,		/// �ݰݱ⿡ ���ϱ� ������ ����� ��ƼŬ(�ݽð����)
			COMMON_MAJOR_PII_REVENGE_IMPACT,				/// 	
			COMMON_MAJOR_PII_REVENGE_IMPACT_TICK,			/// 
			COMMON_MAJOR_PII_MAGIC_FAIL,					/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT,					/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE,			/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH,			/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,				/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED,		/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED,		/// 
			COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED,		/// 

			COMMON_MAJOR_PII_END,
		};

		// kimhc // 2010.11.3 ��� ������ �������� ���� ���̳� ��ƼŬ
		enum COMMON_MINOR_PARTICLE_INSTANCE_ID
		{
			COMMON_MINOR_PII_STEP_SMOKE,
			COMMON_MINOR_PII_STEP_WATER,
			COMMON_MINOR_PII_HYPER_MODE_CHANGE_LINE,
			COMMON_MINOR_PII_HYPER_MODE_CHANGE_CENTER,
			COMMON_MINOR_PII_GROUND_SHOCK_WAVE,
			COMMON_MINOR_PII_WHITE_SHOCK_WAVE,
			COMMON_MINOR_PII_IMPACT_TICK,
			COMMON_MINOR_PII_STR_TOKANG,
			COMMON_MINOR_PII_AIR_DOWN_TICK,
			COMMON_MINOR_PII_DOWN_SMOKE,
	
			COMMON_MINOR_PII_END,
		};

#ifdef ADD_STATE_EXTRA_ATTACK
		enum STATE_EXTRA_ATTACK
		{
			SEA_NONE					= 0,
			SEA_BIGBANG_STREAM_BOMB,
			SEA_END,
		};
#endif
#ifdef ELSWORD_WAY_OF_SWORD
		enum WAY_OF_SWORD_STATE
		{
			WSS_DESTRUCTION				= -2,
			WSS_TOWARD_DESTRUCTION		= -1,
			WSS_CENTER					= 0,
			WSS_TOWARD_VIGOR			= 1,
			WSS_VIGOR					= 2,
		};
#endif ELSWORD_WAY_OF_SWORD
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
		enum UNIT_CLASS_LINE //���� Ŭ���� ���° ��������.
		{
			UCL_FIRST					= 0, // ù��° ���� EX) ���ҵ��� ��� �ҵ� ����Ʈ - �ε� ����Ʈ ����
			UCL_SECOND,						 // �ι�° ���� EX) ���ҵ��� ��� ���� ����Ʈ - �� �����̾� ����
			UCL_THIRD,						 // ����° ���� EX) ���ҵ��� ��� �ý� ����Ʈ - ���Ǵ�Ƽ �ҵ� ����
		};
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		struct  InitData
		{
			InitDeviceData          m_device;
			void                AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ ) const;
		};//struct  InitData

		//{{ kimhc // 2010.11.2 // ��Ÿ��, �����ð� ���� �ʿ���ϴ� ��ų� ����ϱ� ���� Ŭ����
#ifdef	NEW_SKILL_2010_11
				
		/** @class : CSkillDataBase
			@brief : ���� �� �� ĳ���� Ŭ������ ���� ���Ǵ� ��ų �����͸� �����Ͽ� ���� �ϱ� ���� Ŭ����
					 2010/11/09�� ����� �⺻���� ��Ÿ���� ��ɸ� ������ ������, ����Ʈ, ����Ÿ � ������ �� �ֵ���
					 Ȯ�� or ������ ����
			@date  : 2010/11/09
		*/
        class   CSkillDataBase;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<CSkillDataBase> CSkillDataBasePtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<CSkillDataBase> CSkillDataBasePtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		class CSkillDataBase
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{
			// kimhc // 2010-11-10 // �����غ��� ��Ÿ���� �ʿ� ������.. ��ĳ ���� ���� ���!
		private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			CX2SkillTree::SKILL_ID m_eSkill_ID;		/// ������ ��ų ID
			float		m_fCoolTime;				/// ��Ÿ���� �ִ� ��ų�� ��� �ٽ� ����ϱ� ������ ���ѽð�
			float		m_fTimerBasedOnCoolTime;	/// ��Ÿ�ӿ��� ���� 0�� ������ ��ȭ�Ǵ�, ���� �������� Ÿ�̸�
			float		m_fEffectiveTime;			/// ��ų ���ӽð�

		protected:
			CSkillDataBase() : m_eSkill_ID( CX2SkillTree::SI_NONE ), m_fCoolTime( 0.f ), 
				m_fTimerBasedOnCoolTime( 0.f ), m_fEffectiveTime( 0.f ) 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            {}
			CSkillDataBase( CX2SkillTree::SKILL_ID eSkill_ID_, float fCoolTime_ ) : m_eSkill_ID( eSkill_ID_ ), 
				m_fCoolTime( fCoolTime_ ), m_fTimerBasedOnCoolTime( 0.f ), m_fEffectiveTime( 0.f )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            {}

		public:
			
			static CSkillDataBasePtr CSkillCreateSkillDataBasePtr() { return CSkillDataBasePtr( new CSkillDataBase() ); }
			static CSkillDataBasePtr CSkillCreateSkillDataBasePtr(  CX2SkillTree::SKILL_ID eSkill_ID_, float fCoolTime_ ) { return CSkillDataBasePtr( new CSkillDataBase( eSkill_ID_, fCoolTime_ ) ); }

			CX2SkillTree::SKILL_ID GetSkill_ID() const { return m_eSkill_ID; }
			void SetSkill_ID( CX2SkillTree::SKILL_ID eSkill_ID_ ) { m_eSkill_ID = eSkill_ID_; }

			float GetCoolTime() const { return m_fCoolTime; }
			void SetCoolTime( float fCoolTime_ ) {	m_fCoolTime = fCoolTime_; }

			float GetTimerBasedOnCoolTime() const { return m_fTimerBasedOnCoolTime; };
			void SetTimerBasedOnCoolTime( float fTimerBasedOnCoolTime_ ) { m_fTimerBasedOnCoolTime = fTimerBasedOnCoolTime_; }

			float GetEffectiveTime() const { return m_fEffectiveTime; }
			void SetEffectiveTime(float fEffectiveTime_ ) { m_fEffectiveTime = fEffectiveTime_; }

			/** @function : IsEndCoolTime() const
			@brief : ��Ÿ���� ���������� Ȯ���ϴ� �Լ�. (m_fTimerBaseOnCoolTime�� 0.f�� ������� Ȯ��)
			@param : void
			@return : ��Ÿ���� �������� true, ���� ��Ÿ�� ���̸� false
			*/
			bool IsEndCoolTime() const { return ( 0.f >= m_fTimerBasedOnCoolTime ? true : false ); }
			bool IsEndEffectiveTime() const { return ( 0.f >= m_fEffectiveTime ? true : false ); }

			void OnFrameMove( float fElapsedTime_ )
			{
				if ( false == IsEndCoolTime() )
				{
					SetTimerBasedOnCoolTime( GetTimerBasedOnCoolTime() - fElapsedTime_ );
				}

				if ( false == IsEndEffectiveTime() )
				{
					SetEffectiveTime( GetEffectiveTime() - fElapsedTime_ );
				}
			}

			void ResetTimerBasedOnCoolTime() { SetTimerBasedOnCoolTime( GetCoolTime() ); }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		};

#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.2 // ��Ÿ��, �����ð� ���� �ʿ���ϴ� ��ų� ����ϱ� ���� Ŭ����

//{{ kimhc // 2010.4.7 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
		struct Bone_Shield	// ��Ʈ������ ȿ�� �� �����带 ���� ����ü
		{
			float	m_fIntervalTime;				/// ��Ÿ��
			float	m_fReducePercent;				/// ���� ���Ҿ�
			float	m_fTimeLeft;					/// ���� �ð�
			BYTE	m_byCountLeft;					/// ���� Ƚ��
			CX2EffectSet::Handle m_hEffectSet;		/// ����Ʈ ��
			wstring m_wstrEffectName;				/// Ÿ�� ������ �� ������ ����Ʈ �� ��, ���� ����Ʈ �°� �ٸ� ����Ʈ

			Bone_Shield() 
			{
				Init();
			}

			~Bone_Shield()
			{
				DestroyParticle();
			}

			void Init()
			{
				m_fIntervalTime		= 0.0f;
				m_fReducePercent	= 0.0f;
				m_fTimeLeft			= 0.0f;
				m_byCountLeft		= 0;
				m_hEffectSet		= INVALID_EFFECTSET_HANDLE;
				m_wstrEffectName.clear();
			}

			void DestroyParticle();
		};
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.7 // ��д��� �۾�(��Ʈ������ȿ��)

		//{{ kimhc // 2009-11-18 // ��彺ų ������ ��ȸ���� ���� ����ü
		// Ƚ���� �ð� �����
#ifdef	GUILD_SKILL_PART_2
		struct GuildSkillInfiniteStrength
		{
			float		m_byCountLeft;				// ���� Ƚ��
			float		m_fRecoveryHpOnce;			// �ѹ��� ȸ���� ��
			double		m_fElapsedTime;				// ���� ����Ʈ �� ����� �ð�
			const float	m_fTimeGap;					// ���� ȿ������ �ð� ��
			const float m_fRecoveryPerOneDamage;	// �������� ȸ���� % 
			CKTDGParticleSystem::CParticleEventSequenceHandle m_hSeqEffect;
			
			GuildSkillInfiniteStrength() : m_fTimeGap( 0.8f ), m_fRecoveryPerOneDamage( 0.4f )
			{
				Init();
			}

			void Init()
			{
				m_byCountLeft		= 0;
				m_fRecoveryHpOnce	= 0.0f;
				m_fElapsedTime		= 0.0f;
				m_hSeqEffect		= INVALID_PARTICLE_SEQUENCE_HANDLE;
			}

			bool IsElaspedTimeOverGap( float fElapsedTime )
			{
				m_fElapsedTime += fElapsedTime;

				if ( m_fElapsedTime > m_fTimeGap )
				{
					m_byCountLeft--;
					m_fElapsedTime = 0.0f;
					return true;
				}

				return false;
			}
		};

		struct GuildSkillData 
		{
			bool							m_bLearnChanceToReverse;
			GuildSkillInfiniteStrength		m_InfiniteStrength;

			GuildSkillData()
			{
				Init();
			}

			void Init()
			{
				m_bLearnChanceToReverse = false;
				m_InfiniteStrength.Init();
			}
		};

#endif	GUILD_SKILL_PART_2
		//}} kimhc // 2009-11-18 // ��彺ų ������ ��ȸ���� ���� ����ü
		
		struct SkillCutIn
		{
			std::wstring fileName;
			D3DXVECTOR2 vSize;
			D3DXVECTOR3 vPosition;

			SkillCutIn( const std::wstring& fileName_, const D3DXVECTOR2& vSize_, const D3DXVECTOR3& vPosition_ )
			: fileName( fileName_ )
			, vSize( vSize_ )
			, vPosition( vPosition_ )
			{
			}


			SkillCutIn( const std::wstring& fileName_)
				: fileName( fileName_ )
				, vSize( D3DXVECTOR2(512.f,512.f) )
				, vPosition( D3DXVECTOR2(256.f,512.f) )
			{
			}
		};

		struct SkillCutInSet
		{
			SkillCutIn cutIn[3];
		};


		struct SyncData
		{
			DWORD					dwFrameMoveCount;
			KProtectedType<UCHAR>	nowState;
			KProtectedType<char>	nowSubState;
			KProtectedType<UCHAR>	nowAction;

			bool					bDirectChange;
			char					stateChangeNum;

			D3DXVECTOR3				position;
			unsigned char 			lastTouchLineIndex;

			bool					bIsRight;
			bool					bFrameStop;

			KProtectedType<float>	fNowHP;
			KProtectedType<float>	fNowMP;

			KProtectedType<int>		m_HyperModeCount;

//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			KProtectedType<int>		m_CannonBallCount;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG

#ifdef NEW_RANDOM_TABLE_TEST
			USHORT m_usRandomTableIndex;
#else NEW_RANDOM_TABLE_TEST
//{{AFX
			char					m_RandSeed;
			char					m_RandSeed2;
//}}AFX
#endif NEW_RANDOM_TABLE_TEST

			UCHAR				ucHitCount;			/// Ÿ��Ƚ��
			UCHAR				ucHittedCount;		/// �ǰ�Ƚ��
			UCHAR				ucNumOfDeBuff;		/// ���������
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
            DWORD                   m_dwRelativePos;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

			SyncData()
			{
				Init();
			}
			void Init()
			{
				dwFrameMoveCount		= 0;
				nowState				= 0;
				nowSubState				= -1;
				nowAction				= UAI_NONE;
				bDirectChange			= false;
				stateChangeNum			= 0;

				INIT_VECTOR3( position,	0,0,0 );
				lastTouchLineIndex		= 0;

				bIsRight				= true;

				fNowHP					= 0.0f;
				fNowMP					= 0.0f;

				bFrameStop				= false;

//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
				m_CannonBallCount		= 0;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
				m_HyperModeCount		= 0;
				
#ifdef NEW_RANDOM_TABLE_TEST
				m_usRandomTableIndex	= (USHORT) ( rand() % CKTDXRandomNumbers::RANDOM_NUMBER_COUNT );
#else NEW_RANDOM_TABLE_TEST
//{{AFX
				m_RandSeed				= (char) (rand() % 100);
				m_RandSeed2				= (char) (rand() % 100);
//}}AFX
#endif NEW_RANDOM_TABLE_TEST		

				ucHitCount				= 0;			/// Ÿ��Ƚ��
				ucHittedCount			= 0;		/// �ǰ�Ƚ��
				ucNumOfDeBuff			= 0;		/// ���������
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
                m_dwRelativePos         = 0;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
			}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			void SetData( const KDYNAMIC_UNIT_USER_SYNC& SyncPacket, DWORD dwRealFrameMoveCount, float fMaxHP, float fMaxMP );

//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
            void _DecodeUserSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosZ, unsigned char ucLastTouchLineIndex );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//			void _DecodeUserSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosY, USHORT usPosZ, unsigned char ucLastTouchLineIndex );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef OPTIMIZED_P2P
//            void SetData( KXPT_UNIT_USER_SYNC& SyncPacket, USHORT usHP, USHORT usMP );
//#else
//            void SetData( KXPT_UNIT_USER_SYNC* pSyncPacket );
//#endif
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			void Verify();
#ifdef ELSWORD_WAY_OF_SWORD
			int		IsActionActive( UINT uiActionIndex_ );
#endif ELSWORD_WAY_OF_SWORD

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK        
            bool    operator < ( const SyncData& rhs_ ) const   { return dwFrameMoveCount < rhs_.dwFrameMoveCount; }
            bool    operator < ( DWORD dwrhs_ ) const           { return dwFrameMoveCount < dwrhs_; }
            friend bool    operator < ( DWORD dwlhs_, const SyncData& rhs_ ) { return dwlhs_ < rhs_.dwFrameMoveCount; }
//#endif     SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK      
                 };          
            
            
        
		struct FrameData
		{
			StateParam		stateParam;
			UnitCondition	unitCondition;
			SyncData		syncData;

			FrameData& operator=( const FrameData& data )
			{
				stateParam		= data.stateParam;
				unitCondition	= data.unitCondition;
				syncData		= data.syncData;

				return *this;
			}
			void Init()
			{
				stateParam.Init();
				unitCondition.Init();
				syncData.Init();
			}
		};

		
		struct UserUnitStateData
		{
//{{ robobeg : 2008-10-28
			UCHAR				stateID;
			//wstring				stateTable;
//}} robobeg : 2008-10-28
			char				m_SPLevel;
#ifdef NEW_SKILL_TREE
			float				m_fPowerRate;
#endif NEW_SKILL_TREE
			CX2SkillTree::SKILL_ID	m_eSkillID;			// �ʻ�� ������ ��쿡 ���� ����� �ʻ�Ⱑ � ������

			bool    m_bHyperState;	// �������� ��������

			GUStateCBFunc		StateInit;
			GUStateCBFunc		StateStartFuture;
			GUStateCBFunc		StateStart;
			GUStateCBFunc		OnFrameMoveFuture;
			GUStateCBFunc		OnFrameMove;
			GUStateCBFunc		OnCameraMove;
			GUStateCBFunc		OnEventProcess;
//{{ robobeg : 2008-10-28
			//GUStateCBFunc		OnCustomRender;
//}} robobeg : 2008-10-28
			GUStateCBFunc		StateEndFuture;
			GUStateCBFunc		StateEnd;

			CX2ComboAndSkillChainInfoPtr	m_ptrComboAndSkillChainInfo;

			UserUnitStateData()
			{
				Init();
			}

			virtual void Init()
			{
				stateID				= GUSI_NONE;
//{{ robobeg : 2008-10-28
				//stateTable			= L"";
//}} robobeg : 2008-10-28
				m_SPLevel			= 0;					// �ʻ���� SP level�� 1���� �����̰�, �׿� ����޺��� SP level�� ��� 0�̴�
#ifdef NEW_SKILL_TREE
				m_fPowerRate		= 1.f;
#endif NEW_SKILL_TREE
				m_eSkillID			= CX2SkillTree::SI_NONE;

				StateInit			= GUStateCBFunc();
				StateStartFuture	= GUStateCBFunc();
				StateStart			= GUStateCBFunc();
				OnFrameMove			= GUStateCBFunc();
				OnFrameMoveFuture	= GUStateCBFunc();
				OnCameraMove		= GUStateCBFunc();
				OnEventProcess		= GUStateCBFunc();
//{{ robobeg : 2008-10-28
				//OnCustomRender		= GUStateCBFunc();
//}} robobeg : 2008-10-28
				StateEndFuture		= GUStateCBFunc();
				StateEnd			= GUStateCBFunc();

				m_bHyperState = false;
				m_ptrComboAndSkillChainInfo.reset();
			}


			UserUnitStateData( const UserUnitStateData& t ) { *this = t; }    // copy constructor
			UserUnitStateData& operator=( const UserUnitStateData& rhs )
			{
				stateID				= rhs.stateID;
//{{ robobeg : 2008-10-28
				//stateTable			= data.stateTable;
//}} robobeg : 2008-10-28
				m_SPLevel			= rhs.m_SPLevel;
#ifdef NEW_SKILL_TREE
				m_fPowerRate		= rhs.m_fPowerRate;
#endif NEW_SKILL_TREE
				m_eSkillID			= rhs.m_eSkillID;

				StateInit			= rhs.StateInit;
				StateStartFuture	= rhs.StateStartFuture;
				StateStart			= rhs.StateStart;
				OnFrameMove			= rhs.OnFrameMove;
				OnFrameMoveFuture	= rhs.OnFrameMoveFuture;
				OnCameraMove		= rhs.OnCameraMove;
				OnEventProcess		= rhs.OnEventProcess;
//{{ robobeg : 2008-10-28
				//OnCustomRender		= data.OnCustomRender;
//}} robobeg : 2008-10-28
				StateEndFuture		= rhs.StateEndFuture;
				StateEnd			= rhs.StateEnd;

				m_bHyperState		= rhs.m_bHyperState;
				m_ptrComboAndSkillChainInfo = rhs.m_ptrComboAndSkillChainInfo;

				return *this;
			}
		};
		typedef map<UCHAR, UserUnitStateData> UserUnitStateDataMap;


//#ifndef NOT_USE_DICE_ROLL
//		struct DiceRoll
//		{
//			DiceRoll( CX2GUUser* pGUUser );
//			~DiceRoll();
//			void OnFrameMove( double fTime, float fElapsedTime );
//
//			void PushDiceRoll( bool bWinningDice )
//			{
//				m_vecDiceRoll.push_back( bWinningDice );
//			}
//
//			D3DXVECTOR3 GetParticlePos();
//			D3DXVECTOR3	GetZVector( D3DXVECTOR3& vDirVector );
//			
//		public:
//			CX2GUUser*		m_pGUUser;
//			vector<bool>	m_vecDiceRoll;	// true�̸� 6�� ������ �ֻ��� ����, false �̸� 6�� ������ ������ ���ڰ� ������ �ֻ��� ����
//
//			int		m_iNowRollingDiceCount;
//			int		m_iNowDiceSeqIndex;
//
//			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqRollingDice[6];			// ������ �ֻ��� ��ƼŬ
//			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDice[6];					// ���� �ֻ��� ��ƼŬ
//
//			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDiceBalloon;
//			CKTDGParticleSystem::CParticle*					m_pParticle;
//		};
//#endif //NOT_USE_DICE_ROLL

		//{{ dmlee 2008.04.11 - ĳ�þ����� ����
		struct CashItemAbility
		{
			bool		m_bShowOppnentMP;
			float		m_fUpMPAtAttackOrDamage;

			CashItemAbility()
			{
				m_bShowOppnentMP		= false;
				m_fUpMPAtAttackOrDamage = 0.f;
			};
		};
		//}} dmlee 2008.04.11 - ĳ�þ����� ����
		



		struct CommonRandomState
		{
			std::string	    m_StateName;
			int				m_iPercent;
		};


#ifdef SERV_PET_SYSTEM
		struct PetCheer
		{
			float m_fTime;

			float m_fChangeHp;			

			float m_fSpeed;
			float m_fMoveSpeed;
			float m_fCriticalRate;
			float m_fChargeMp;
			float m_fChargeHp;

			PetCheer()
			{
				Init();
			}

			void Init()
			{
				m_fTime = 0.f;
				m_fChangeHp = 0.f;
				m_fSpeed = 0.f;
				m_fMoveSpeed = 0.f;
				m_fCriticalRate = 0.f;
				m_fChargeMp = 0.f;
				m_fChargeHp = 0.f;
			}
		}; 
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
		struct EventProcessTime
		{
			float keyInputStart;
			float keyInputEnd;
			float stateChange;
			EventProcessTime()
			{
				keyInputStart	= 0.f;
				keyInputEnd		= 9999.f;
				stateChange		= 9999.f;
			}
		};
#endif ELSWORD_SHEATH_KNIGHT

#ifdef ADD_STATE_EXTRA_ATTACK
		struct StateExtraAttack
		{
			STATE_EXTRA_ATTACK	eId;
			float				m_fStartTime;
			StateExtraAttack()
			{
				eId = SEA_NONE;
				m_fStartTime = 9999.f;
			}
		};
#endif

		struct SummonNpcSocket
		{
			CX2UnitManager::NPC_UNIT_ID m_iNpcId;
			float						m_fSummonRate;
			float						m_fSummonNpcCoolTime;
		};

#ifdef SUMMON_MONSTER_CARD_SYSTEM		// mauntain : ����ȯ [2012.06.26] ���� ī�� ��ȯ ��� - �ش� ������ ���� ��ȯ ����
		struct SummonMonsterCardData
		{
			int	  m_iSummonMonsterUID;		//��ȯ�� ���� UID
			float m_fSummonMonsterTime;		//��ȯ�� ������ ���� ���� �ð�
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
			std::vector<int> m_vecSummonMonsterUID;  //4���� ��ȯ�� ���� UID�� ���� ����
#endif //EVENT_MONSTER_CARD_SUMMON_ENEMY
			SummonMonsterCardData()
			{
				m_iSummonMonsterUID  = -1;
				m_fSummonMonsterTime = 0.f;
			}

			void init()
			{
				m_iSummonMonsterUID  = -1;
				m_fSummonMonsterTime = 0.f;
			}

			int GetSummonMonsterUID() const						
			{ 
				return m_iSummonMonsterUID;
			}
			void SetSummonMonsterUID( int iSummonMonsterUID ) 
			{ 
				m_iSummonMonsterUID = iSummonMonsterUID;
			}
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
			void SetSummonMonsterUIDInVec( int iSummonMonsterUID ) 
			{ 
				m_vecSummonMonsterUID.push_back( iSummonMonsterUID );
			}
			std::vector<int> GetSummonMonsterUIDVec()							
			{ 
				return m_vecSummonMonsterUID;
			}
			void ClearSummonMonsterUIDInVec() 
			{ 
				m_vecSummonMonsterUID.clear();
			}
#endif //EVENT_MONSTER_CARD_SUMMON_ENEMY
			float GetSummonMonsterTime() const						
			{ 
				return m_fSummonMonsterTime;
			}
			void SetSummonMonsterTime( float fSummonMonsterTime ) 
			{ 
				m_fSummonMonsterTime = fSummonMonsterTime;
			}
		};
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		struct GameCameraOffset
		{
			bool	bEnable; 
			float	fCameraDistance;
			float	fHeight;
			float	fAngleDegree;
			float	fEyeDistance;
			float	fLookatDistance;
			float	fTrackingTime;
			float	fTrackingAtTime;
			GameCameraOffset()
			{
				bEnable	= false;
				fCameraDistance		= 0.f;
				fHeight				= 0.f;
				fAngleDegree		= 0.f;
				fEyeDistance		= 0.f;
				fLookatDistance		= 0.f;
				fTrackingTime		= 0.f;
				fTrackingAtTime		= 0.f;
			}
		};
#endif SERV_CHUNG_TACTICAL_TROOPER

		//{{ Public Variable End

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ

	/// �����̰� ������ ��ų Ű �Է� ���� ��� ����
	/** @struct : CurrentSkillKeyManager
	@brief : ���� ��ųŰ�� ������ �Է��Ͽ� �ߵ��ϴ� ��ų�� ���� �Է� Ű ���� ����ü
	
	��� �� �ʼ������� �����ؾ� �ϴ� ����
	1. StartFuture���� InitCurrentSkillKeyManager() ȣ��
	2. EventProcess���� CheckSkillSlotSwap() ȣ��
	3. Ű�Է� ���� üũ�� IsPressCurrentSkillKey()�� üũ
*/
	struct CurrentSkillKeyManager
	{
		int iSkillSlotIndex;
		bool m_bIsSkillSLotSwap;

		CurrentSkillKeyManager():
		iSkillSlotIndex(-1),
			m_bIsSkillSLotSwap(false)
		{}

		void CheckSkillSlotSwap();
		void InitCurrentSkillKeyManager( int iSkillSlotIndex_ );
		bool IsPressedCurrentSkillKey( const CX2GUUser::InputData& InputData_ );
		bool IsUnpressedCurrentSkillKey( const CX2GUUser::InputData& InputData_ );
		void Init();
	};

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_RENA_SYSTEM //��â��
	struct AllDamageRelateSkillData	
	{
		Byte m_byteSkillIndex;	//��ų id�� ���� ���� �����Ƿ� ������ ���� index��
		CX2DamageManager::DamageRelateSkillData m_NowData;	// ���� �������� ���� data ��
		std::vector<CX2DamageManager::DamageRelateSkillData> m_vecSaveData;	// ������ ������ �� damagedata�� ����Ǿ� �ִ� ���� �񱳸� ���� ���尪.

		AllDamageRelateSkillData():m_byteSkillIndex(0){}

		void Init()
		{
			m_byteSkillIndex = 0;
			m_NowData.Init();
			m_vecSaveData.clear();
		}
	};

	//���� m_NowData�� �������� �Լ�
	const CX2DamageManager::DamageRelateSkillData& GetNowDamageRelateSkillData(){ return m_AllDamageRelateSkillData.m_NowData; }

	//���� ���� ������ m_NowData�� �������� �Լ�
	CX2DamageManager::DamageRelateSkillData& GetAccessNowDamageRelateSkillData(){ return m_AllDamageRelateSkillData.m_NowData; }

	//���� ���� ������ m_AllDamageRelateSkillData�� �������� �Լ�
	AllDamageRelateSkillData& GetAccessAllDamageRelateSkillData(){ return m_AllDamageRelateSkillData; }

	//���� ���� �����͸� �����ϴ� �Լ�. �����ϸ� m_vecSaveData ���嵵 ���� ��.
	void SetNowDamageRelateSkillData( Byte byteRelateData_ );

	//���ڷ� ���� ���� m_vecSaveData�� ����� �� �߿� ��ġ�ϴ� ���� �ִ��� üũ�ϴ� �Լ� 
	virtual bool CheckDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ ){ return false; }

	//m_vecSaveData �� �߿� ���ڷ� ���� ���� ��ġ�ϴ� ���� �����ϴ� �Լ�
	virtual void DeleteDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ ){};

	//DamageReact�� �����ϱ� ���� �ش� DamageData�� ������ �� �ִ� �Լ�
	virtual void AdjustDamageDataBeforeDamageReact( CX2DamageManager::DamageData* pDamageData ){};
#endif //ADD_RENA_SYSTEM

#ifdef NEXON_QA_CHEAT_REQ
	bool	m_bIsInvincibleAndNoMpConsume;
#endif //NEXON_QA_CHEAT_REQ


	protected:
		//{{ Protected Function Start
		CX2GUUser( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit );
		virtual ~CX2GUUser(void);
		//{{ Protected End Start

	public:
		//{{ Public Function Start
		static void                 InitMotion( InitData& OutInit_, KLuaManager& luaManager_ );
		static void					InitInit( InitData& OutInit_, KLuaManager& luaManager_);

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					InitComponent( InitData& OutInit_, KLuaManager& luaManager_);
		static void					AppendMotionToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );
		static void					AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );
		static void					AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );
		static void					AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pX2Unit_ );

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ���� ��ȭ ����Ʈ ���ε� �Լ�
		static void					AppendUpgradeWeaponParticleToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit::UNIT_TYPE eUnitType_, int iUpgradeLevel_ );
#endif ADD_UPGRADE_WEAPON_PARTICLE

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		virtual void				Init( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				ReInit( bool bUseTeamPos = true, int startPosIndex = -1 );

		void						InitByGameType();

		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnCameraMove();

//{{ robobeg : 2008-10-28
        virtual RENDER_HINT			OnFrameRender_Prepare();
		virtual void                OnFrameRender_Draw();
//}} robobeg : 2008-10-28


		virtual void				PreRenderForce();

		virtual HRESULT				OnResetDevice();
		virtual HRESULT				OnLostDevice();

		//virtual void				StateChange( int state, bool bDirectChange = false );
		//virtual void				StateChange( int state, int subState, bool bDirectChange = false );
		virtual bool				StateChange( int state, bool bDirectChange = false );
		virtual bool				StateChange( int state, int subState, bool bDirectChange = false );
        int							GetStartStateID(){ return USI_START; }
		int							GetStartIntrudeStateID(){ return USI_START_INTRUDE; }
		int							GetWalkStateID(){ return USI_WALK; }
		int							GetDashStateID(){ return USI_DASH; }
		int							GetDashEndStateID(){ return USI_DASH_END; }
		int							GetChargeMpStateID() { return m_ChargeMpState; }
		int 						GetDamageGroggyStateID() const { return USI_DAMAGE_GROGGY; }
		int 						GetDamageAirDownStateID() const { return USI_DAMAGE_AIR_DOWN; }

		virtual int					GetComboZStateID() const { return USI_WAIT; }
		virtual int					GetComboXStateID() const { return USI_WAIT; }

#ifdef RIDING_SYSTEM
		// ������ // �ڵ� ������ ���� ������Ʈ ��� �Լ� �߰�
		int			 GetWaitStateID() { if (true == GetRidingOn()) return USI_RIDING_WAIT; return m_CommonState.m_Wait; }
		USER_STATE_ID GetJumpDownStateID() { if( true == GetRidingOn()) return USI_RIDING_JUMP_DOWN; return USI_JUMP_DOWN; }
		USER_STATE_ID GetWinStateID() { if (true == GetRidingOn()) return USI_RIDING_WAIT_HABIT; return USI_WIN; }
		USER_STATE_ID GetLoseStateID() { if (true == GetRidingOn()) return USI_RIDING_WAIT; return USI_LOSE; }
#else
		int			  GetWaitStateID(){ return m_CommonState.m_Wait; }
		USER_STATE_ID GetJumpDownStateID() { return USI_JUMP_DOWN; }
		USER_STATE_ID GetWinStateID() { return USI_WIN; }
		USER_STATE_ID GetLoseStateID() { return USI_LOSE; }
#endif //RIDING_SYSTEM

//{{ kimhc // 2010.4.28 // ��д��� �۾�(���� ���� ȿ��)
// #ifdef SERV_SECRET_HELL
// 		CKTDGXSkinAfterImage* GetAfterImage() const { return m_pAfterImage; }
// #endif SERV_SECRET_HELL
//}} kimhc // 2010.4.28 // ��д��� �۾�(���� ���� ȿ��)

#ifdef _CLASS_CUTIN_
		wstring						GetCutInFileName(bool IsRight);
#endif _CLASS_CUTIN_

		virtual float				GetPowerRate(); // moved by wonpok. 20091215.

#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	#ifdef ADD_MEMO_1ST_CLASS
		void                        LoadEffectSetFromScript( IN KLuaManager& luaManager, IN const bool IsEqippedMemo_ = false );
	#else //ADD_MEMO_1ST_CLASS
		void                        LoadEffectSetFromScript( IN KLuaManager& luaManager );
	#endif //ADD_MEMO_1ST_CLASS
        void                        CommonFrameMove_EffectSet( float fNowAnimationTime );

		//{{ kimhc // 2010.11.3 // �Ž�ų �۾�
#ifdef	NEW_SKILL_2010_11
		// �� ������Ʈ���� Immune�Ǿ���� ExtraDamage�� ��ũ��Ʈ���� �о��
		void LoadImmunityAtThisStateFromScript( IN KLuaManager& luaManager_ );
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.3 // �Ž�ų �۾�


        const D3DXVECTOR2&          GetNowAfterImageTime() const { return m_v2NowAfterImageTime; }
        bool                        FindNowSlashTraceTimeWithWeaponIndex( int iWeaponIndex, OUT D3DXVECTOR3& vSlashTraceTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


//#ifdef PVP_BOSS_COMBAT_TEST
//		int							GetFrozenState() const { return m_FrozenState; }
//#endif PVP_BOSS_COMBAT_TEST


		virtual void				DamageReact( CX2DamageManager::DamageData* pDamageData );

		bool						IsSocketSuperArmor() const { return m_bSocketSuperArmor; }
		float						GetTimeSocketSuperArmor() const { return m_fSocketSuperArmor; }
		
		void DamageReact_ExtraDamage( CX2DamageManager::DamageData* pDamageData, bool bApplyExtraDamageOfAttacker );
		void Process_ExtraDamage( const CX2DamageManager::DamageData* pDamageData, const CX2DamageManager::ExtraDamageData &extraDamageData );
//{{ oasis907 : ����� [2010.3.22] // 
		void LevelCorrectionExtraDamage( const CX2DamageManager::DamageData* pDamageData, CX2DamageManager::ExtraDamageData* pExtraDamageData);
//}}

		//{{ kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		void SetShowTooniLandEventEffectSet( bool bShow_ );
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ

#ifdef ARA_CHANGE_CLASS_FIRST
		virtual void				DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack = -1, int iAvoidance = -1 );
#else
		virtual void				DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack = -1 );
#endif //ARA_CHANGE_CLASS_FIRST
		virtual void				DetonationStateChange( CX2DamageManager::DamageData* pDamageData );

		int							GetDamageCount(){ return m_nDamageCount; }
		void						SetDamageCount( int nCount ) { m_nDamageCount = nCount; }
		virtual void				AttackResult();
		virtual void				AttackResultStateChange( int damageRevenge );
		
		//{{ kimhc // 2010-11-22 // �� ĳ������ AttackResult�� ������
		virtual void				AttackResultPlayEffect();// �� ĳ������ AttackResult�� ����� �������� �ű�

#ifdef UPGRADE_RAVEN
		virtual void				AttackResultByType( CX2DamageManager::DamageData &pDamageData );
#endif

		void						PlayRevengeReactTypeParticle( const D3DXVECTOR3& vImpactPoint_ );
		void						PlaySmallDamageReactTypeParticle( const D3DXVECTOR3& vImpactPoint_ );
		void						PlayEtcReactTypeParticle( const D3DXVECTOR3& vImpactPoint_ );
		//}} kimhc // 2010-11-22 // �� ĳ������ AttackResult�� ������

		//{{ kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����
		float						CalculateHpUpPercentBySocketOption( const vector<D3DXVECTOR2>& vecHpUpPercentBySocketOption );
		float						CalculateMpUpBySocketOption( const vector<D3DXVECTOR2>& vecMpUpBySocketOption );

#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
		float						CalculateMpUpAttackedBySocketOption( vector<D3DXVECTOR4>& vecMpUpAttackedBySocketOption );
#endif //ADJUST_SECRET_ITEM_OPTION

		//}} kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����

		virtual void				ProcessAttackResult();
#ifdef SKILL_CASH_10_TEST
		void						ProcessDrainParticle( D3DXVECTOR3 vPos, bool bCreate = true );
#endif
#ifdef ATTACK_DELAY_GAGE
		//{{AFX
		virtual void				AddAttackDelay( float fDelay );
		//}}AFX
#endif ATTACK_DELAY_GAGE

		virtual void				AddForceDownDelay( float fDelay );
		virtual void				Win();
		virtual void				Lose();
		virtual void				Draw(); // ����
#ifdef DUNGEON_NEW_RESULT
		virtual void				Wait();
#endif
		

		virtual D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName ) const;
		virtual D3DXVECTOR3			GetHeadBonePos() const;	//optimization	

#ifdef MODIFY_GET_BONE_POS
		// ������ // 2012-12-17 // ���� ���� ��쿡 ���� üũ�ϱ� ���� OUT ���ڸ� ����ϴ� �Լ� �߰�
		virtual bool				GetBonePos( const WCHAR* pBoneName, OUT D3DXVECTOR3& pos_ ) const;
#endif //MODIFY_GET_BONE_POS

		//{{ kimhc // 2011.1.17 // ������ Bone�� ��Ʈ���� ���� Trace �� (chung �ڵ� ����)
		virtual const D3DXMATRIX*	GetCombineMatrixFromBoneName( const wstring& wstrBoneName ) const;
		//}} kimhc // 2011.1.17 // ������ Bone�� ��Ʈ���� ���� Trace �� (chung �ڵ� ����)

//{{ robobeg : 2008-10-21
		//virtual void				SetShow( bool bShow );
        virtual void                NotifyShowObjectChanged();
//}} robobeg : 2008-10-21

// X2GUUser �� KTDGManager �� ���, ������ �� ���� ��ǰ�鵵 ���� ���, �����ϱ� ���� ����
        virtual void                NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17

		virtual void				SetHyperModeCount( int i )			
		{ 
			if ( NULL != GetGageData() )
				GetGageData()->SetHyperModeCount( i );
			m_FrameDataFuture.syncData.m_HyperModeCount = i; 
		}
		
		virtual int					GetHyperModeCount()	const			{ return m_FrameDataFuture.syncData.m_HyperModeCount; }
		virtual float				GetHyperModeTime() const;

		virtual int					GetLastStateHyperModeCount() const { return m_iLastStateHyperModeCount; }
		
		virtual void				CreateAndInsertRenderParamByBuffUnitType( const KBuffIdentity& BuffIdentity_, const map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>& mapStBuffRenderParamPtr_ );

		virtual float				GetStopTime();

		virtual void				SetStopTime( float fStopTime );
		virtual void				SetStop2Time( float fStopTime );

		virtual const UnitCondition& GetUnitCondition( bool bFuture = false ) const	{ return GetFrameData(bFuture).unitCondition; }
		virtual const StateParam&	GetStateparam( bool bFuture = false ) const	{ return GetFrameData(bFuture).stateParam; }

#ifdef USER_HOLD
		void						SetHold(bool val);
		bool						GetHold()								{ return m_bHold; }
		virtual void				SetPos( D3DXVECTOR3 vPos );
#endif

#ifdef TEST_GROUP_GRAP
		void						SetGrap( bool val );			/// ���� ���� �ִ� �����ΰ�?
		bool						GetGrap() { return m_bGrap; }	/// ���� ���� �ִ� �����ΰ�?
		void						SetGrapShakeCount( int val ){ m_iGrapShakeCount = val; }	/// Ű���带 �¿�� �������� �ϴ� Ƚ�� (�� Ƚ�� ��ŭ ���� ��⿡�� Ǯ��)
		int							GetGrapShakeCount(){ return m_iGrapShakeCount; }			/// Ű���带 �¿�� �������� �ϴ� Ƚ�� (�� Ƚ�� ��ŭ ���� ��⿡�� Ǯ��)

		int							GetGrapStateID(){ return m_iGrapStateID; }			/// �������� �� ������ ����� ������Ʈ (���� ���ȿ��� �� ������Ʈ�� ���� ��)
		void						SetGrapStateID(int val) { m_iGrapStateID = val; }	/// �������� �� ������ ����� ������Ʈ (���� ���ȿ��� �� ������Ʈ�� ���� ��)
		
		void						SetGrappedPosition( D3DXVECTOR3 vPos );		/// ������ ���¿����� ���� ��ġ	//optimization
		D3DXVECTOR3					GetGrappedPosition(){ return m_vGrap; }		/// ������ ���¿����� ���� ��ġ	//optimization
		void						SetBeforeGrapPosition( D3DXVECTOR3 vPos );	/// �������� ������ ���� ��ġ		//optimization
		void						ResetToBeforeGrapPos();		/// �������� ������ ���� ��ġ�� �ǵ���
#endif TEST_GROUP_GRAP


#ifdef NEW_GRAPPLE_ATTACK
		void						SetGrappled( bool val );
		bool						GetGrappled(){ return m_bGrappled; }
		
		void						SetGrappledStatdID( int val ){ m_iGrappledStateID = val; }
		int							GetGrappledStateID() { return m_iGrappledStateID; }
#endif NEW_GRAPPLE_ATTACK


		// �Էµ� ��ġ���� ������ ������ ������ Y�࿡ ������ ���� �����ϴ� �������� �Űܼ� ĳ���� ��ġ�� �����Ѵ�
		virtual void SetPositionOnLine( const D3DXVECTOR3& vPosition, const int iLineIndex );


		virtual const D3DXVECTOR3&	GetPos( bool bFuture = false )	const	{ return GetSyncData(bFuture).position; }	//optimization
		virtual float				GetXPos( bool bFuture = false )	const		{ return GetSyncData(bFuture).position.x; }
		virtual float				GetYPos( bool bFuture = false )	const		{ return GetSyncData(bFuture).position.y; }
		virtual float				GetZPos( bool bFuture = false )	const		{ return GetSyncData(bFuture).position.z; }
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        virtual void                BackupPosition()  
        {
            const SyncData& kSyncData = GetSyncData(true);
            const UnitCondition& kUnitCondition = GetUnitCondition(true);
            m_vPositionBackup = kSyncData.position;
            m_iPositionBackup_LineIndex = kSyncData.lastTouchLineIndex;
            m_bPositionBackup = true;
            m_bPositionBackup_FootOnLine = kUnitCondition.bFootOnLine;
        }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual void    OnFrameMove_PostProcess()   { BackupPosition(); }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual const D3DXVECTOR3&	GetRotateDegree() const					{ return m_FrameDataNow.unitCondition.dirDegree; }
		virtual float				GetXRotateDegree() const					{ return m_FrameDataNow.unitCondition.dirDegree.x; }
		virtual float				GetYRotateDegree() const					{ return m_FrameDataNow.unitCondition.dirDegree.y; }
		virtual float				GetZRotateDegree() const				{ return m_FrameDataNow.unitCondition.dirDegree.z; }

		virtual float				GetPlaySpeed() const					{ return m_pXSkinAnim->GetPlaySpeed(); }

		virtual const D3DXVECTOR3&	GetDirVector() const							{ return m_FrameDataNow.unitCondition.dirVector; }
		virtual bool				GetIsRight() const							{ return m_FrameDataNow.syncData.bIsRight; }
		virtual bool				GetIsRightBefore() const					{ return m_FrameDataNowBefore.syncData.bIsRight; }

		virtual GAME_UNIT_STATE_ID	GetGameUnitState() const					{ return (GAME_UNIT_STATE_ID)( (UCHAR) m_FrameDataNow.syncData.nowState); }
		virtual bool				GetInvincible()	const					{ return m_FrameDataNow.stateParam.bInvincible; }
		virtual bool				GetIsFallDownState()					
        { 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_bNowFallDown;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return m_FrameDataNow.stateParam.bFallDown; 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        float                       GetNowSlashTraceTipWide() const         { return m_fNowSlashTraceTipWide; }
        bool                        FindNowAddSlashTraceWithWeaponIndex( int iWeaponIndex, OUT D3DXVECTOR3& vSlaceTraceTime ) const;
        virtual UINT                GetSizeOfAttackTimeList() const             { return m_vecAttackTime.size(); }
        virtual const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ ) const
        {
            if ( uiIndex_ < m_vecAttackTime.size() )
                return  &m_vecAttackTime[ uiIndex_ ].m_v2AttackTime;
            else
                return  NULL;
        }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef FIX_MEDITATION_RUN
		virtual GAME_UNIT_STATE_ID	GetGameUnitStateFuture() const			{ return (GAME_UNIT_STATE_ID)( (UCHAR) m_FrameDataFuture.syncData.nowState); }
#endif FIX_MEDITATION_RUN

		virtual float				GetUnitWidth( bool bFuture ) const		{ return GetFrameData( bFuture ).unitCondition.fUnitWidth;	}
		virtual float				GetUnitHeight( bool bFuture ) const		{ return GetFrameData( bFuture ).unitCondition.fUnitHeight; }

		bool						GetAttackSuccess( bool bFuture )		{ return GetFrameData( bFuture ).unitCondition.bHit; }


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void				        ReceiveFrameData( DWORD dwFrameMoveCount, const KDYNAMIC_UNIT_USER_SYNC& kUserSync );
        void                        _PostProcess_ReceiveFrameData();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual void				ReceiveFrameData( KXPT_UNIT_USER_SYNC_PACK* pKXPT_UNIT_USER_SYNC_PACK );
//#ifdef UNIT_SYNC_PACKET_DUNGEON_FOR_TEST
//		//virtual void				ReceiveFrameData( KXPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON* pKXPT_UNIT_USER_SYNC_PACK );
//#endif UNIT_SYNC_PACKET_DUNGEON_FOR_TEST
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		void						SendFrameData();

		virtual bool				IsMyUnit() const;
		virtual bool				IsLocalUnit() const;
		virtual bool				IsFocusUnit() const;

		//////////////////////////////////////////////////////////
		UidType						GetOwnerUnitUID()	{ return m_OwnerUserUID; }
		UidType						GetUnitUID() const	{ return m_UnitUID; }
		//virtual CX2Unit*			GetUnit() const		{ return m_pUnit; }
        CX2Unit*			        GetUnit() const		{ return m_pUnit; }
		const SyncData&				GetSyncData( bool bFuture = false ) const;
		const FrameData&			GetFrameData( bool bFuture = false ) const;
        const FrameData&			GetFrameDataDead() const { 	return m_FrameDataDead; }

		const InputData&			GetInputData() { return (const InputData&)m_InputData; }
		void						SetInputData( InputData* pInputData );
#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
		void						SetUnit( CX2Unit* pUnit){ m_pUnit = pUnit; }
#endif GUUSER_UNIT_POINTER_CRASH_FIX
	
		void						ClearRecieveSyncData();

		CX2ComboManager*			GetComboManager() { return m_pComboManager; }
		virtual void				InitPosition( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				InitPosition( CX2GUUser* pCX2GUUser );
		virtual void				InitPosByDeadPos( CX2GUUser* pCX2GUUser );
		void						InitPosByLineData( int iLineIndex );
		void						InitPosByBattleFieldPosition();

//{{ kimhc // 2010.8.10 // Ư�� ������ ��ġ ���� ��Ȱ ��ġ�� ���
		bool						InitPosByMonsterPos();
//}} kimhc // 2010.8.10 // Ư�� ������ ��ġ ���� ��Ȱ ��ġ�� ���

		const FrameData&			GetNowFrameData() const { return m_FrameDataNow; }
		FrameData&			        AccessNowFrameData() { return m_FrameDataNow; }
//{{ robobeg : 2013-11-04
        // m_FrameDataFuture�� My unit �� ��쿡�� �ǹ�����. �ٸ� ���� ĳ���� ���ؼ��� GetFrameData( true ) �� ����Ͻÿ�.
		const FrameData&			GetFutureFrameData() const { return m_FrameDataFuture; }
        FrameData&			        AccessFutureFrameData() { return m_FrameDataFuture; }
//}} robobeg : 2013-11-04
		void						SetRebirthTime( float fRebirthTime ){ m_fRebirthTime = fRebirthTime; }
		float						GetRebirthTime(){ return m_fRebirthTime; }

		virtual int					GetNowStateID() const { return m_NowStateData.stateID; }
		int							GetFutureStateID() const { return m_FutureStateData.stateID; }
		CX2SkillTree::SKILL_ID		GetNowStateSkillID() const { return m_NowStateData.m_eSkillID; }
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		CX2SkillTree::SKILL_ID		GetFutureStateSkillID() const { return m_FutureStateData.m_eSkillID; }
#endif //SERV_ADD_LUNATIC_PSYKER

		bool IsHyperState() { return m_NowStateData.m_bHyperState; }

		virtual void				RenderName();

		virtual void				InitEffect();

		virtual void				InitializeGUUserMajorParticleArray();		// ������ ��ƼŬ �迭 �ʱ�ȭ	// kimhc // 2010.11.5
		virtual void				CreateMajorParticleArray();			// ������ ��ƼŬ �迭�� ������	// kimhc // 2010.11.5
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetCommonMajorParticleByEnum( COMMON_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// ���������� ���� ������ ��ƼŬ �� ENUM ���� �ش��ϴ� ��ƼŬ�� ������ // kimhc // 2010.11.5 

		ParticleEventSequenceHandle	GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const	// ���������� ���� ������ ��ƼŬ �� ENUM ���� �ش��ϴ� ��ƼŬ �ڵ� �ϳ��� ����// kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MAJOR_PII_END > eVal_ && COMMON_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahCommonMajorParticleInstance[eVal_];
		}
		
		void						SetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// ���������� ���� ������ ��ƼŬ �ڵ� �� ENUM ���� �ش��ϴ� �ڵ��� ������ // kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MAJOR_PII_END > eVal_ && COMMON_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahCommonMajorParticleInstance[eVal_] = hHandle_;
		}
		void						DeleteGUUserMajorParticle();
		

		virtual void				InitializeGUUserMinorParticleArray();	// ���̳� ��ƼŬ �迭 �ʱ�ȭ	// kimhc // 2010.11.6 
		virtual void				CreateMinorParticleArray();		// ���̳� ��ƼŬ �迭�� ������	// kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetCommonMinorParticleByEnum( COMMON_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrPatricleName_, int iDrawCount_ = -1 ); // ���������� ���� ���̳� ��ƼŬ �� ENUM ���� �ش��ϴ� ��ƼŬ�� ������ // kimhc // 2010.11.5 
		ParticleEventSequenceHandle	GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const // ���������� ���� ���̳� ��ƼŬ �� ENUM ���� �ش��ϴ� ��ƼŬ �ڵ� �ϳ��� ���� // kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MINOR_PII_END > eVal_ && COMMON_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahCommonMinorParticleInstance[eVal_];
		}
		void						SetHandleCommonMinorParticleByEnum( COMMON_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // ���������� ���� ���̳� ��ƼŬ �ڵ� �� ENUM ���� �ش��ϴ� �ڵ��� ������ // kimhc // 2010.11.5 
		{
			ASSERT( COMMON_MINOR_PII_END > eVal_ && COMMON_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahCommonMinorParticleInstance[eVal_] = hHandle_;
		}
		void						DeleteGUUserMinorParticle();

		virtual void				DeleteMinorParticle();

		// �������� ����ϴ� weapon enchant particle ���� �Լ�
		void CreateEnchantedWeaponParticleAtHand( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );
		void ProcessEnchantedWeaponEffectAtHand( CX2Unit::UNIT_TYPE unitType );
		void InitEnchantedWeaponEffectAtHand();
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void	AppendEnchantedWeaponEffectAtHandToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		void SetShowEnchantedWeaponEffectAtHand( bool bShow );


				
		// unit class ���� ������ ����ϴ� weapon enchant particle ���� �Լ�, CX2GameUnit::Weapon Ŭ������ ���ԵǾ� �ִ� ��ƼŬ�̴�
		virtual void				InitEnchantWeaponEffectUnitSpecific() = NULL;



		//////////////////////////////////////////////////////////////////////////
		bool						GetHyperModeUsed() { return m_bHyperModeUsed; }		// ������ ����ߴ��� ���ߴ���
		const UserUnitStateDataMap&		GetStateList() const { return m_StateList; }
		void						ToggleSkillSlotAB();

		bool						GetResurrectionTimeOut() { return m_bResurrectionTimeOut; }
		void						SetResurrectionTimeOut( bool bCheck ) { m_bResurrectionTimeOut = bCheck; }
		
//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		// ���� Ÿ���� �߰���
		void						SetLockOn( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0, CX2DamageEffect::LOCK_ON_TYPE eLockOnType_ = CX2DamageEffect::LOT_NONE, int iModulusFactor = 30 );
#else	NEW_CHARACTER_CHUNG
		void						SetLockOn( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0 );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG

		void						SetDevAddDamage(float val = 0.f) { m_fDevAddDamage = val; }
		void						ForceChangeHyperMode( int iAddBall_ );
		void						ForceChangeHyperModeWithoutMotion( float fAddTime_ );


		void						SetNowSpecialAttack(int val) { m_iNowSpecialAttack = val; }
		int							GetNowSpecialAttack() const { return m_iNowSpecialAttack; }
		bool						GetHoldSpecialAttack() const { return m_bHoldSpecialAttack; }
		bool						GetHoldingSpecialAttack() const { return m_bHoldingSpecialAttack; }
//#ifndef NOT_USE_DICE_ROLL
//		DiceRoll*					GetDiceRoll() { return m_pDiceRoll; }
//#endif //NOT_USE_DICE_ROLL

		const CX2GUUser::CashItemAbility& GetCashItemAbility() const { return m_CashItemAbility; }
        CX2GUUser::CashItemAbility& AccessCashItemAbility() { return m_CashItemAbility; }


		int 				GetRandomIntFuture( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomInt( m_FrameDataFuture.syncData.m_usRandomTableIndex + iOffset ); }
		float				GetRandomFloatFuture( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomFloat( m_FrameDataFuture.syncData.m_usRandomTableIndex + iOffset ); }


		virtual int 				GetRandomInt( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomInt( m_FrameDataNow.syncData.m_usRandomTableIndex + iOffset ); }
		virtual float				GetRandomFloat( int iOffset = 0 ) { return CKTDXRandomNumbers::GetRandomFloat( m_FrameDataNow.syncData.m_usRandomTableIndex + iOffset ); }


		virtual const D3DXVECTOR3&	GetLandPosition() const { return m_FrameDataNow.unitCondition.landPosition; }


		virtual int					GetLastTouchLineIndex( bool bFuture = false ) const
		{
			if( true == bFuture )
				return m_FrameDataFuture.syncData.lastTouchLineIndex;
			else 
				return m_FrameDataNow.syncData.lastTouchLineIndex;
		}

		virtual const D3DXVECTOR3&		GetLandPos( bool bFuture = false ) const
		{
			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.landPosition; 
			else
				return m_FrameDataNow.unitCondition.landPosition; 
		}



		const wstring GetRandomWinSpeech() //optimization
		{
			if( m_vecDungeonWinSpeech.size() == 0 )
			{
				return L"VICTORY";
			}

			int iRand = rand() % (int) m_vecDungeonWinSpeech.size(); 
			return m_vecDungeonWinSpeech[ iRand ];
		} 


		//virtual float				GetEvadePercent();
		//virtual float				GetAntiEvadePercent();

		//{{ kimhc // 2011-07-19 // �ɼǵ���Ÿ ��ġȭ �۾�
		// �ɼ� ��ġȭ�� ���� �߰� ������
#ifdef ELSWORD_SHEATH_KNIGHT
		/*virtual*/ float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData );
#else
		/*virtual*/ float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData ) const;
#endif ELSWORD_SHEATH_KNIGHT
		//}} kimhc // 2011-07-19 // �ɼǵ���Ÿ ��ġȭ �۾�

		int							GetSkillLevelUpNum( CX2SkillTree::SKILL_ID skillID ) const;

#ifdef VERIFY_STAT_BY_BUFF
		virtual	void				TransformScale( const PROTECT_VECTOR3& vScale_ );
#else	// VERIFY_STAT_BY_BUFF
		virtual	void				TransformScale( const D3DXVECTOR3& vScale_ );
#endif // VERIFY_STAT_BY_BUFF
		
		virtual float				GetCriticalPercent( BYTE byDamageType, float fExtraCritical = 0.f );
		
		void						ChangeModelDetail( int detailPercent );

		bool						GetIsRidingMachine() { return m_bRidingMachine; }


		bool						IsImmuneToEnchant( CX2EnchantItem::ENCHANT_TYPE enchantType );
		void						AddEnchantResist( const CX2SocketItem::SocketData& kSocketData );
#ifdef PET_AURA_SKILL
		void						AddEnchantAttack( const CX2SocketItem::SocketData& kSocketData);
		bool						GetApplyPetAura() { return m_bApplyAura; }
		void						SetApplyPetAura( bool bVal ) { m_bApplyAura = bVal; }
#endif
		//void AddEnchantResist( CX2EnchantItem::ENCHANT_TYPE eEnchantType, float fResistValue );


// ����� 2010.11.1
#ifdef NEW_SKILL_2010_11
	#ifndef FIX_LIMITED_MANA_MANAGEMENT
		void						SetLimitManaManagement( bool bBool ){ m_bLimitManaManagement = bBool; }
	#endif //FIX_LIMITED_MANA_MANAGEMENT
#endif 
// ����� 2010.11.1

		virtual void				ShowSnatchCureEffect();

		bool						IsSummonSpecialAttack( CX2SkillTree::SKILL_ID iSkiiID );
		bool						CheckSummonSpecialAttack( CX2SkillTree::SKILL_ID iSkillID_ );

		float						GetDamageUpPercentBySMA( int npcID ); // SpecificMonsterAttack
		float						GetEvadeUpPerBySMA( int npcID );

		const CKTDGXSkinAnim* GetXSkinAnim() const { return m_pXSkinAnim.get(); }
		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ������ ����Ʈ ȿ��
		CKTDGXSkinAnimPtr GetXSkinAnimPtr() const { return m_pXSkinAnim; }
		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ������ ����Ʈ ȿ��
		const CKTDXDeviceXSkinMesh* GetMotion() const { return m_pMotion; }


		int GetDamageDownLandFront() const { return USI_DAMAGE_DOWN_FRONT; }
		int GetDamageDownLandBack() const { return USI_DAMAGE_DOWN_BACK; }
		int GetDamageAirDownLanding() const { return USI_DAMAGE_AIR_DOWN_LANDING; }



		void SetPepperRunTime(float val) { m_fPepperRunTime = val; }
		void StartPepperRun( float fTime );



		void GenericGroundComboEndEventProcess();
		void GenericSpecialActiveSkillEventProcess();
		void GenericActiveSkillEventProcess();



		bool IsOnSomethingFuture()
		{
//#ifdef STEP_ON_MONSTER_TEST
//			return m_FrameDataFuture.unitCondition.bFootOnLine || 
//					m_FrameDataFuture.unitCondition.bFootOnUnit;
//#else STEP_ON_MONSTER_TEST
			return m_FrameDataFuture.unitCondition.bFootOnLine;
//#endif STEP_ON_MONSTER_TEST
		}

#ifdef CLIFF_CLIMBING_TEST
		
		bool IsOnCliffFuture();
		void ClimbWaitEventProcess();
		void ClimbFrontFrameMoveFuture();
		void ClimbFrontEventProcess();
		void ClimbUpEventProcess();
		void ClimbDownEventProcess();
		void ClimbUpFrontFrameMoveFuture();
		void ClimbUpFrontEventProcess();
		void ClimbDownFrontFrameMoveFuture();
		void ClimbDownFrontEventProcess();


#endif CLIFF_CLIMBING_TEST


		void ResetOnSomethingFuture()
		{
			m_FrameDataFuture.unitCondition.bFootOnLine = false;
//#ifdef STEP_ON_MONSTER_TEST
//			m_FrameDataFuture.unitCondition.bFootOnUnit = false;
//#endif STEP_ON_MONSTER_TEST
		}

	//{{ kimhc // 2010.11.3 // �Ž�ų �۾�
#ifdef	NEW_SKILL_2010_11
		// ���� - ��ȭ�� �г�
		bool GetCanChangeHyperModeInHyperModeState() const { return m_bCanChangeHyperModeInHyperModeState; };
		void SetCanChangeHyperModeInHyperModeState( bool bVal_ ) { m_bCanChangeHyperModeInHyperModeState = bVal_; }
#endif	NEW_SKILL_2010_11
	//}} kimhc // 2010.11.3 // �Ž�ų �۾�


		//////////////////////////////////////////////////////////////////////////
		
		virtual void PEPPER_RUN_READY_EventProcess();
		virtual void PEPPER_RUN_FrameMoveFuture();
		virtual void PEPPER_RUN_EventProcess();
		virtual void PEPPER_RUN_END_FrameMove();
		virtual void PEPPER_RUN_END_EventProcess();

		virtual void PEPPER_RUN_JUMP_UP_StateStartFuture();
		virtual void PEPPER_RUN_JUMP_UP_FrameMoveFuture();
		virtual void PEPPER_RUN_JUMP_UP_EventProcess();

		virtual void PEPPER_RUN_JUMP_DOWN_StateStartFuture();
		virtual void PEPPER_RUN_JUMP_DOWN_FrameMoveFuture();
		virtual void PEPPER_RUN_JUMP_DOWN_EventProcess();


		virtual void DAMAGE_GROGGY_EventProcess();

#ifdef SPECIAL_USE_ITEM
		void THROW_ITEM_StateStart();
		void THROW_ITEM_FrameMoveFuture();
		void THROW_ITEM_FrameMove();
		void THROW_ITEM_CameraMove();
		void THROW_ITEM_EventProcess();
		void THROW_ITEM_StateEnd();
#endif SPECIAL_USE_ITEM

		void THROW_WOODEN_PIECE_FrameMoveFuture();
		virtual void THROW_WOODEN_PIECE_FrameMove() {}
		void THROW_WOODEN_PIECE_CameraMove();
		void THROW_WOODEN_PIECE_EventProcess();

		

		void MACHINE_GUN_RIDE_StartFuture();
		void MACHINE_GUN_RIDE_Start();
		void MACHINE_GUN_RIDE_FrameMoveFuture();
		void MACHINE_GUN_RIDE_FrameMove();
		void MACHINE_GUN_RIDE_CameraMove();
		void MACHINE_GUN_RIDE_EventProcess();

		void MACHINE_GUN_LEAVE_StartFuture();
		void MACHINE_GUN_LEAVE_Start();
		void MACHINE_GUN_LEAVE_FrameMoveFuture();
		void MACHINE_GUN_LEAVE_FrameMove();
		void MACHINE_GUN_LEAVE_CameraMove();
		void MACHINE_GUN_LEAVE_EventProcess();

		void MACHINE_GUN_WAIT_StartFuture();
		void MACHINE_GUN_WAIT_Start();
		void MACHINE_GUN_WAIT_FrameMoveFuture();
		void MACHINE_GUN_WAIT_EventProcess();
		
		void MACHINE_GUN_WALK_FrameMoveFuture();
		void MACHINE_GUN_WALK_EventProcess();
		
		void MACHINE_GUN_ATTACK_FrameMoveFuture();
		void MACHINE_GUN_ATTACK_FrameMove();
		void MACHINE_GUN_ATTACK_EventProcess();

		void MACHINE_GUN_JUMP_UP_FrameMoveFuture();
		void MACHINE_GUN_JUMP_UP_EventProcess();

		void MACHINE_GUN_JUMP_DOWN_FrameMoveFuture();
		void MACHINE_GUN_JUMP_DOWN_EventProcess();

		void SUMMON_MAGIC_SPEAR_FrameMove();
		void SUMMON_MAGIC_SPEAR_EventProcess();

		virtual void COMMON_BUFF_FrameMove() = NULL;
		void COMMON_BUFF_EventProcess();

#ifdef ADDED_RELATIONSHIP_SYSTEM
		virtual void COMMON_RELATIONSHIP_SKILL_FrameMove();
		virtual void COMMON_RELATIONSHIP_SKILL_EventProcess();
#endif // ADDED_RELATIONSHIP_SYSTEM

		// Start ������Ʈ
		void StartEventProcess();
		void StartEnd();
		// StartIntrude ������Ʈ
		void StartIntrudeStart();
		void StartIntrudeFrameMove();
		void StartIntrudeEventProcess();
		// Die ������Ʈ
		void DieFrameMove( float fTimeCreateStepDust_, float fTimeSacrificeOfHero_ );	// Front, Back �������� ���
		// WaitStart ���¿��� ���ǿ� ���� �ִϸ��̼��� ���� ��Ŵ
		void PlayAnimationWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ );	
		bool PlaySitReadyWaitStart( CKTDGXSkinAnim* pXSkinAnim_ );
		void WalkFrameMoveFuture();

		// JumpUp, JumpDown ���¿��� �̵��� �����Ͽ� �������� ó���Ǵ� �κ�
		void JumpFrameMoveFuture();
		void JumpLandingStart();

		// Dash Jump Landing
		void DashJumpLandingStartFuture();

		// Dash
		virtual void DashStartFuture();
		void DashStart();
		void DashFrameMoveFuture();

		// StandUp( Back, Front )
		void DamageStandUpEventProcess();
		void DamageStandUpEnd();

		// DamageAirSmall
		void DamageAirSmallStartFuture();
		void DamageAirSmallEventProcess();

		// DamageAirDown
		void DamageAirDownEventProcess();

		// DamageAirDown_INVINCIBLE
		void DamageAirDownInvincibleEventProcess();

		// DamageAirDownLanding
		void DamageAirDownLandingStart();
		void DamageAirDownLandingFrameMove();

		// DamageAirFall
		void DamageAirFallEventProcess();
		// DamageAirUp
		void DamageAirUpEventProcess();
		// DamageAirFly
		void DamageAirFlyEventProcess();
		// DamageRevenge
		void DamageRevengeStart();
		void DamageRevengeCameraMove();
		void DamageRevengeEventProcess();
		// Wall Jump	
#ifdef WALL_JUMP_TEST
		void WallLandingEventProcess();
		void WallLandingEndFuture();
#endif WALL_JUMP_TEST

		// ����
		void HyperModeStart();
		virtual void HyperModeFrameMove() = NULL;
		void HyperModeEventProcess();
#ifdef HYPER_MODE_FIX
		void HyperModeStartFuture();
		void HyperModeEnd();
#endif HYPER_MODE_FIX

#ifdef MODIFY_RIDING_PET_AWAKE
		// ����
		void			RidingHyperModeStart();
		void			RidingHyperModeEventProcess();
#ifdef HYPER_MODE_FIX
		void			RidingHyperModeStartFuture();
		void			RidingHyperModeEnd();
#endif HYPER_MODE_FIX

#endif // MODIFY_RIDING_PET_AWAKE

#ifdef RIDING_SYSTEM
		void RidingStartStart();			// ���� ����
		void RidingStartStartFuture();
		void RidingStartEventProcess();

		void RidingOnStart();			// ž��
		void RidingOnStartFuture();
		void RidingOnEventProcess();
		void RidingOnEnd();
				
		void RidingOffEventProcess();		// ����
		void RidingOffEnd();

		void RidingWaitHabitStart();		// ����
		void RidingWaitHabitStartFuture();
		void RidingWaitHabitEventProcess();

		void RidingWaitStart();				// ���
		void RidingWaitStartFuture();
		virtual void RidingWaitEventProcess();

		void RidingWalkStart();				// �ȱ�
		void RidingWalkStartFuture();
		virtual void RidingWalkEventProcess();

		void RidingJumpUpStart();			// ����
		void RidingJumpUpStartFuture();
		void RidingJumpUpEventProcess();

		void RidingJumpDownStart();			// ���� ������
		void RidingJumpDownStartFuture();
		void RidingJumpDownEventProcess();

		void RidingJumpLandingStart();		// ����
		void RidingJumpLandingStartFuture();
		virtual void RidingJumpLandingEventProcess();

		void RidingDashStart();				// ���
		virtual void RidingDashEventProcess();

		void RidingDashEndStart();			// ��� ��
		void RidingDashEndStartFuture();
		virtual void RidingDashEndEventProcess();
		void RidingDashEndFrameMove();

		void RidingDashJumpStart();			// ��� ����
		void RidingDashJumpStartFuture();
		void RidingDashJumpEventProcess();
		void RidingDashJumpFrameMoveFuture();
		void RidingDashJumpEndFuture();

		void RidingDashJumpLandingStart();	// ��� ���� ����
		void RidingDashJumpLandingEventProcess();

		void RidingDamageFrontStart();		// �ǰ� ��
		void RidingDamageFrontStartFuture();
		void RidingDamageFrontEventProcess();

		void RidingDamageBackStart();		// �ǰ� ��
		void RidingDamageBackStartFuture();
		void RidingDamageBackEventProcess();
				
		void RidingAttackZStart();
		void RidingAttackZStartFuture();
		void RidingAttackZEventProcess();

		void RidingJumpAttackZStart();
		void RidingJumpAttackZStartFuture();
		void RidingJumpAttackZFrameMoveFuture();
		void RidingJumpAttackZEventProcess();
		
		void RidingAttackXStart();
		void RidingAttackXStartFuture();
		void RidingAttackXEventProcess();

		void RidingSpecialAttackStart();
		void RidingSpecialAttackStartFuture();
		void RidingSpecialAttackFrameMove();
		void RidingSpecialAttackEventProcess();
				
		void RidingSpecialMoveStartFuture();		// AirialDash, DoubleJump
		void RidingSpecialMoveStart();				// AirialDash, BoostDash
		void RidingSpecialMoveEventProcess();		// AirialDash, BoostDash, DoubleJump
		void RidingSpecialMoveFrameMoveFuture();	// AirialDash, BoostDash
		void RidingSpecialMoveFrameMove();			// AirialDash, BoostDash
		void RidingSpecialMoveEndFutrue();			// AirialDash
		void RidingSpecialMoveEnd();				// BoostDash

		int	GetRidingStartStateID() const { return USI_RIDING_START; }
		int	GetRidingWaitStateID() const { return USI_RIDING_WAIT; }
		int	GetRidingWaitHabitStateID() const { return USI_RIDING_WAIT_HABIT; }

		const D3DXVECTOR3&	GetSaddlePos() const { return m_vSaddlePos; }
		void		SetSaddlePos( IN D3DXVECTOR3 vPos_ ) { m_vSaddlePos = vPos_; }
		const D3DXVECTOR3&	GetSaddleDegree() const { return m_vDirDegree; }
		void		SetSaddleDegree( IN D3DXVECTOR3 vDegree_ ) { m_vDirDegree = vDegree_; }
		const wstring& GetRidingMotionName() const { return m_wstrRidingMotionName; }
		void		SetRidingMotionName( IN const wstring& wstrName_ ) { m_wstrRidingMotionName = wstrName_; }

		virtual bool GetRidingOn() const { return m_bRidingOn; }
		void SetRidingOn( IN bool bVal_ ) { m_bRidingOn = bVal_; }
		void ProcessRidingOn( IN bool bVal_, IN const KRidingPetInfo& _Info  = KRidingPetInfo() );

		const std::vector<TIME_SPEED>& GetSpeedFatorVector() const { return m_vecSpeedFactor; }
        std::vector<TIME_SPEED>& AccessSpeedFatorVector() { return m_vecSpeedFactor; }

		void SetDeleteEffectSetOnStateEnd( IN bool bVal_ ) { m_bDeleteEffectSetOnStateEnd = bVal_; }
		void SetDeleteEffectSetOnDamageReact( IN bool bVal_ ) { m_bDeleteEffectSetOnDamageReact = bVal_; }
		void SetDeleteEffectSetOnDie( IN bool bVal_ ) { m_bDeleteEffectSetOnDie = bVal_; }

		bool RidingPetSpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );

		const vector<SoundPlayData>& GetSoundPlayData() const { return m_vecSoundPlayData; }
        vector<SoundPlayData>& AccessSoundPlayData() { return m_vecSoundPlayData; }

		void SetStateSuperArmor( IN bool bVal_ ) { m_bStateSuperArmor = bVal_; }
		
		void SetSkillCancelAfter( IN float fVal_ ) { m_fSkillCancelAfter = fVal_; }
		void SetBWalkCancelAfter( IN float fVal_ ) { m_fBWalkCancelAfter = fVal_; }
		void SetWalkCancelAfter( IN float fVal_ ) { m_fWalkCancelAfter = fVal_; }
		void SetDashCancelAfter( IN float fVal_ ) { m_fDashCancelAfter = fVal_; }
		virtual bool CanRidingState();
#ifdef CHECK_CAMERA_INFO
		void SetIsLineMapCamera( IN bool bVal_ ) { m_bIsLineMapCamera = bVal_;}
#endif // CHECK_CAMERA_INFO
		float   GetCameraAngleDegree() const { return m_fAngleDegree; }
		const GameCameraOffset& GetGameCameraOffset() const { return m_GameCameraOffset; }
#endif //RIDING_SYSTEM
		
		// ���� �� �� ������� �ϴ� ����Ʈ
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		void ShowMinorParticleHyperModeChange( const wstring& wstrBoneName = L"Dummy1_Rhand" );
		void ShowMinorParticleHyperModeShock( const wstring& wstrBoneName = L"Dummy1_Rhand" );
#else // SERV_9TH_NEW_CHARACTER
		void ShowMinorParticleHyperModeChange();
		void ShowMinorParticleHyperModeShock();
#endif // SERV_9TH_NEW_CHARACTER
		void ShowMinorParticleHyperModeTrace();
#ifdef HYPER_MODE_EFFECT_ADD_BY_ITEM
		void ShowSpecialEffectHyperMode();
#endif // HYPER_MODE_EFFECT_ADD_BY_ITEM

//#ifdef PVP_BOSS_COMBAT_TEST
//
//		void Frozen_StateStart();
//		void Frozen_StateEnd();
//		void Frozen_EventProcess();
//		
//#endif PVP_BOSS_COMBAT_TEST

#ifdef UNIT_EMOTION
		bool PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionId, wstring &wstrEmotionType);
		CX2Unit::EMOTION_TYPE GetNowEmotionId() { return m_ePlayedEmotion; }
#endif

#ifdef EQUIP_STRIP_TEST
		bool IsEquipStripped( CX2Unit::EQIP_POSITION eEquipPosition );
		void SetEquipStrippedState( CX2Unit::EQIP_POSITION eEquipPosition, bool bStripped );
		CX2EqipPtr GetBasicEquipAtStrippedPosition( CX2Unit::EQIP_POSITION eEquipPosition );
		void SetBasicEquipAtStrippedPosition( CX2Unit::EQIP_POSITION eEquipPosition, CX2EqipPtr pEquip );
#endif EQUIP_STRIP_TEST

		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� Īȣ, ��� ��ü ���� �κ� �߰�
#ifdef REAL_TIME_ELSWORD
		void			UpdateSocketDataAndEnchantData();
		virtual void	UpdatePassiveAndActiveSkillState();
		void			UpdateEquippedEmblem();
		
//{{ kimhc // 2010.12.10 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	
	#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		#ifdef FIX_ARA_ONE_PIECE_FOR_BERSERK
		virtual void UpdateNotRenderingOtherEquipList();
		#else //FIX_ARA_ONE_PIECE_FOR_BERSERK
		void UpdateNotRenderingOtherEquipList();
		#endif //FIX_ARA_ONE_PIECE_FOR_BERSERK
	#endif	NOT_RENDERING_OTHER_EQUIP_POS
		void UpdateEquipCollisionData( IN vector<CX2EqipPtr>& m_vecEquipList );
		CX2EqipPtr	GetEquipPtrFromEquipViewListByEquipPosition( const CX2Unit::EQIP_POSITION eEquipPosition );			
		virtual void SetShowViewList( bool bShow_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.10 //  2010-12-23 New Character CHUNG
		
		virtual void		UpdateWeaponEnchantEffectAndExtraDamageType();
		virtual void		UpdateEquipCollisionData();
		virtual bool		UpdateEquipViewListInfo( CX2Item* pItemToAddInViewList, CX2Item* const pItemToRemoveInViewList );
		void		AddEquipToViewList( CX2Item* const pItem );
		void		RemoveEquipFromViewList( CX2EqipPtr pEquipPtrToRemove, int index	);	//optimization
		//void		RemoveEquipFromViewListByEquipPostion( CX2Unit::EQIP_POSITION equipPosition );
		void		DeleteEnchantedWeaponEffectAtHand();

		void UpdateSlashTraceTextureName();
#ifdef FIX_SLASH_TRACE
		void UpdateSlashTrace()
		{
			if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
			{
				for( UINT i=0; i<m_vecpWeapon.size(); ++i )
				{
					Weapon* pWeapon = m_vecpWeapon[i];
					pWeapon->InitSlashTrace();
					pWeapon->LoadDataForUser( m_LuaManager );
				}

				m_LuaManager.EndTable(); // INIT_COMPONENT
			}	
		}
#endif
#endif REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� Īȣ, ��� ��ü ���� �κ� �߰�




		float GetStatAtkRateOfSummoned() const { return m_SkillRelatedData.m_fStatAtkRateOfSummoned; }
		const CX2SkillTree::SkillTemplet* GetEquippedActiveSkillTemplet( CX2SkillTree::SKILL_ID eSkillID ) const;

#ifdef SERV_SKILL_NOTE
		bool GetEqippedSkillMemo(CX2SkillTree::SKILL_MEMO_ID eMemoId) const;
#endif


		void ResetMaxHP();
		void ResetMaxMP();
		void ResetMPChangeRate( float fMPChangeRate );

		
		void CreateTeleportEffect( const D3DXVECTOR3& vPos_, const D3DXVECTOR3& vRotDegree_ );	//optimization

#ifdef NO_DETONATION
		bool GetNoDetonation() { return m_bNoDetonation; }
		void SetNoDetonation(bool val) { m_bNoDetonation = val; }
#endif

		virtual void SetEnableDash( bool bEnableDash, ENABLE_DASH_STATE eState = EDS_END );
		void StateChangeDashIfPossible();

//{{ robobeg : 2012-11-16
		static CX2DamageManager::EXTRA_DAMAGE_TYPE	CalcWeaponEnchantExtraDamageType( CX2Unit* pX2Unit_ );
//}} robobeg : 2012-11-16
		void CalcWeaponEnchantExtraDamageType();

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ���� ������ ��ȭ ���� ��ȯ
		static int CalcWeaponEnchantLevel( CX2Unit* pX2Unit_ );
#endif ADD_UPGRADE_WEAPON_PARTICLE



#ifdef HEAD_ATTACHED_CAMERA_TEST
		bool BoneAttachedCamera( const wstring& boneName, const D3DXVECTOR3& vEyeOffset = D3DXVECTOR3(0, 0, 0) );
		bool BoneAttachedCamera( const CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vEyeOffset = D3DXVECTOR3(0, 0, 0) );
#endif HEAD_ATTACHED_CAMERA_TEST

		//{{ kimhc // 2009-11-20 // ��� ��ų 3, 4����
#ifdef	GUILD_SKILL_PART_2
		const GuildSkillData& GetGuildSkillData() { return m_GuildSkillData; }	//optimization

		virtual void GuildSkillProcess( const CX2SkillTree::SKILL_ID eSkillID );
		void ProcessSacrificeOfHero();		// ������ ��� ��彺ų
#endif	GUILD_SKILL_PART_2
		//}} kimhc // 2009-11-20 // ��� ��ų 3, 4����

		bool DoScaleHeadBone();

#ifdef DUNGEON_ITEM		
		void SetSuperArmor(bool bVal, float fVal)
		{
			m_bSuperArmorByItem = bVal;
			m_fSuperArmorByItem = fVal;
		}

		void ScaleHead();
		void RestoreHead();
#endif

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-04-01
		void ScaleHand( D3DXVECTOR3 vChangeScale );
		void RestoreHand();
#endif // SERV_NEW_DEFENCE_DUNGEON

		//{{ kimhc // 2010.2.10 //	���ҵ� ���� ��ȭ �ڵ鷯
#ifdef	EVE_SECOND_CLASS_CHANGE
		void				SetHitNasodWeapon( bool bValue ) { m_bHitNasodWeapon = bValue; }
		bool				DidHitNasodWeapon() const { return m_bHitNasodWeapon; }
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.2.10 //	���ҵ� ���� ��ȭ �ڵ鷯

//#ifdef TRANSFORMER_TEST
//		void TransformIntoUser();
//		void TransformIntoMonster( CX2UnitManager::NPC_UNIT_ID eNPCID );
//
//		CX2GUNPC* GetTransformer() { return m_pTransformer; }
//		void SetTransformer( CX2GUNPC* val) { m_pTransformer = val; }
//
//
//#endif TRANSFORMER_TEST
		
		//{{ �ӱԼ� �ӱԼ� ���� ��ŸƮ ǥ�� ������ �� , ���� ���� ���� �Ŀ� ������ ��� ���ϵ��� ����
#ifdef FIX_QUICK_SLOT_USE_DUNGEON_PLAY
		bool GetStarted(){ return m_bStarted; }
#endif 
		//}}

		int GetUnitLevel() const
		{
			if(m_pUnit != NULL)
				return GetUnit()->GetUnitData().m_Level; 
			return 0;
		}

		const CX2Unit::UNIT_CLASS GetUnitClass()
		{
			if( m_pUnit != NULL )
			{
				return GetUnit()->GetUnitData().m_UnitClass;
			}

			return CX2Unit::UC_NONE;
		}

		const CX2Unit::UNIT_TYPE GetUnitType()
		{
			if( m_pUnit != NULL )
				return GetUnit()->GetType();

			return CX2Unit::UT_NONE;
		}

#ifdef SERV_PET_SYSTEM
		int GetDieCount() { return m_iDieCount; }
		const PetCheer& GetPetCheer() const { return m_petCheer; }
        PetCheer& AccessPetCheer() { return m_petCheer; }
#endif

		//{{ JHKang / ������ / 2010/12/16 / ���� �ǰ� �����ΰ�?
#ifdef SEASON3_MONSTER_2010_12
		bool GetNowHit() { return m_bNowHit; }
		void SetNowHit( IN bool bHit_ ) { m_bNowHit = bHit_; }
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / ������ / 2010/12/16 / ���� �ǰ� �����ΰ�?

		//{{ JHKang / ������ / 2011/01/17 / ���� ������ ���� ������ ��� �Լ�
#ifdef SEASON3_MONSTER_2010_12
		float GetRealDamage() { return m_fRealDamage; }
		void SetAnimSpeed_LUA( IN float fAnimSpeedRate_ )
		{
			m_fAnimSpeedRateByNpc = fAnimSpeedRate_;
		}
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / ������ / 2011/01/17 / ���� ������ ���� ������ ��� �Լ�

		float GetOxygenGage() const { return m_fOxygenGage; }
		//{{ JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
#ifdef DUNGEON_RANK_NEW
		/// HP ȸ����( �Һ� ������(����, ����) / ����, �ϸ��Ǿ�, �� ��Ÿ ȸ���� ���� )
		void	AddRecoveryHP( IN int iHP_ ) { m_iTotalRecoveryHP += iHP_; }
		int		GetTotalAttackedDamage( void ) { return m_iTotalAttackedDamage; }
		void	SetTotalAttackedDamage( IN int iAttackedDamage_ ) { m_iTotalAttackedDamage = iAttackedDamage_; }
		int		GetTotalRecoveryHP( void ) { return m_iTotalRecoveryHP; }
		int		GetUsingSkillCount( void );
		void	CountUsedSkill( int iSlotNumber_ );
		int		GetTotalGivenDamamge( void ) { return m_iTotalGivenDamamge; }
		void	AddGivenDamamge( IN int iGivenDamage_ ) { m_iTotalGivenDamamge += iGivenDamage_; }
#endif DUNGEON_RANK_NEW
		//}} JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����

		void	SetCurrentDamage( IN float fFinalDamage_ ) { m_fCurrentFinalDamage = fFinalDamage_; }
		float	GetCurrentDamage() { return m_fCurrentFinalDamage; }

#ifdef DUNGEON_DASH_LOG
		int		GetUsingDashCount( void ){ return m_iDashCount; }
#endif DUNGEON_DASH_LOG

#ifdef SERV_PVP_NEW_SYSTEM
		virtual wstring GetUnitName() 
		{
			return GetUnit()->GetNickName();
		}
#endif

#ifdef NEW_HENIR_TEST
		void StartHenirBuffFirst();
#ifndef ADD_HENIR_BUFF		/// ������ ����Ǹ鼭 ������ �ʰ� �� ����
		void StartHenirBuffFire();
		void StartHenirBuffWater();
		void StartHenirBuffNature();
		void StartHenirBuffWind();
		void StartHenirBuffLight();
#endif ADD_HENIR_BUFF
		void StartHenirBuffDark();
		void EndHenirBuff();

		float GetHpUpRateBySkill();
		float GetMpUpRateBySkill();
#endif NEW_HENIR_TEST

#ifdef SPECIAL_USE_ITEM
		void StartSpecialItemBuffWind();
		void EndSpecialItemBuff();
		void StopSpecialItemBuffWind();
		void SetThrowItemValue( int iValue_ ){ m_iThrowItemValue = iValue_; }
#endif

#ifdef FACE_OFF_MONSTER_HEAD_TEST
		void SetHideNoRenderable( bool bVal ){ m_bHideNoRenderable = bVal; }
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef ELSWORD_WAY_OF_SWORD
		virtual int			GetWayOfSwordState() const;
		virtual float		GetWayOfSwordPoint() const;
		virtual void		SetWayOfSwordPoint( float fWayOfSwordPoint_ );
		bool				IsActionActive( USER_ACTION_ID eUserActionID_ );
#endif ELSWORD_WAY_OF_SWORD
#ifdef ELSWORD_SHEATH_KNIGHT
//		void				AddComboToBrutalSlayer();
// 		float				GetBrutalSlayerDamageRel();
// 		float				GetBrutalSlayerManaBurnRate();
// 		float				GetBrutalSlayerManaBurnABS();
#endif ELSWORD_SHEATH_KNIGHT


#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		bool IsTransformed() { return m_bIsTransformed; }
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
		void				ProcessVPP();
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef EVE_ELECTRA
		bool				GetWhiteOut(){ return m_bWhiteOut; }
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	float GetCannonBallDamageUp() { return m_fCannonBallDamageUp; }
	float GetForceDownRelDamageRateBase() { return m_fForceDownRelDamageRateBase; }
#endif

#ifdef BUFF_TEMPLET_SYSTEM
	void UpdateBuffInfo( const KStat& kStat_, const vector<KBuffInfo>& vecSetBuffInfo_, const vector<int>& vecEraseBuffInfo_ );
	//���� ����Ʈ ���� �Լ�
	void UpdateBuffEffect( const vector<KBuffInfo>& vecSetBuffInfo_, const vector<int>& vecEraseBuffInfo_ );
	
	//10�ʴ� ��ü ü���� % ȸ�� ����
	void SetHPChangeRatePerSecond( float fElapsedTime );
	//���� ���� ȸ�� ����
	void SetMPChangeRateValue( float fElapsedTime );
#endif BUFF_TEMPLET_SYSTEM

#ifdef PVP_ZOOM_CAMERA
	void SetSubZoomCameraTimer();
#endif

#ifdef SERV_ELSWORD_INFINITY_SWORD
	bool IsDashState()
	{
		switch( GetNowStateID() )
		{
		case USI_DASH:
		case USI_DASH_END:
		case USI_DASH_JUMP:
		case USI_DASH_JUMP_LANDING:
			return true;
		default:
			break;
		}

		return false;
	}	
#endif

#ifdef SUMMON_MONSTER_CARD_SYSTEM
	const SummonMonsterCardData& GetSummonMonsterCardData() const { return m_SummonMonsterCardData; }			///��ȯ ���Ϳ� ī�� ���� ��ȯ
    SummonMonsterCardData& AccessSummonMonsterCardData() { return m_SummonMonsterCardData; }			///��ȯ ���Ϳ� ī�� ���� ��ȯ
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef FINALITY_SKILL_SYSTEM
	void ResetLinkOverChargeIllusion();
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_RENA_NIGHT_WATCHER
	void UpdateStartOfDelayedFiring( bool bInsertUnit = true );			/// �����ڰ� �߻��� ������ ��ȣź ���� ����
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	float GetEDTBleedingTime();
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	int GetHyperModeCountUsed(){	return m_iHyperModeCountUsed;	}
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
	bool GetUseChangeWeapon() { return m_bUseChangeWeapon; }										/// ���̻��� ������ ����ũ ���̳� û�� 3������ ���� ���� ���� ���� ��ȯ
	void SetUseChangeWeapon( bool bUseChangeWeapon_ ) { m_bUseChangeWeapon = bUseChangeWeapon_; }	/// ���̻��� ������ ����ũ ���̳� û�� 3������ ���� ���� ���� ���� ����
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int GetAllyEventMonsterUID() { return m_iAllyEventMonsterUID; }
	void SetAllyEventMonsterUID( int iAllyEventMonsterUID ) { m_iAllyEventMonsterUID = iAllyEventMonsterUID; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

		virtual void		InitEquippedActiveSkillState( bool bOnlySkillLevel = false );

		void				ChangeEquippedSkillState( const int iSkillSlotId_, const int iSkillId_ );
		void				ChangeEquippedSkillState( CX2Unit::UnitData& kUnitData_, const CX2SkillTree::SKILL_ID eSkillId_, const int iSlotIndex_, const bool bSlotB_ );

		// �ٻ� ���¿����� �ʵ忡�� ĳ�ü� ����, ���ü, ����/���� ���� ����� �Ұ�( true�� �����ϸ� ��� �Ұ�)
		bool IsBusyStateNow() { return !( m_ElapsedTimeNotBusyState.CheckElapsedTime() ); }
		float GetRemainElapsedTimeNotBusyState() const {	return m_ElapsedTimeNotBusyState.GetTargetTime() - m_ElapsedTimeNotBusyState.GetSumOfElapsedTime();	}

		void				UpdateBuffInfo( const KStat& kStat_, const vector<KBuffInfo>& vecBuffInfo_ );

		virtual void ChargeMpAndEtcInTrainingGame();

		virtual void				InitAquiredActiveSkillState( const int iSkillID_ ) {}
		virtual void				InitPassiveSkillState();
		void						SendBuffInfoNot( const UidType uidGameUnitToSendNot_ );
		virtual bool				DidReceiveFirstSyncPacket() const { return m_bFirstDataReceive; }		/// �ش� ���������� ù ��ũ��Ŷ�� �޾Ҵ��� ���θ� �˾ƺ��� �Լ�


#ifdef UPGRADE_SKILL_SYSTEM_2013
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE, const float fCoolTimeRate = 1.f, const bool bIsRelativeAllSkill = false );
#else // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE );
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
#else // UPGRADE_SKILL_SYSTEM_2013
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_ );
#endif // UPGRADE_SKILL_SYSTEM_2013

		virtual CKTDGXSkinAnimPtr	GetSkinAnimPtr() const { return m_pXSkinAnim; }
		void DoGuildSkillChanceToReverse();

		/// �� ���ָ��� Ư�� ������ ���� �Ǵ� ���� �� �� �����ؾ� �ϴ� �Լ� ����(ex: �����ø���ũ���� �������̶�� �÷��� ��...)
		virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void				AnimStop();
		virtual void				AnimPlay();
		
		virtual void SetStateToNowAnimation( const CKTDGXSkinAnim::XSKIN_ANIM_STATE eAnimState_ )
		{
			m_pXSkinAnim->SetState( eAnimState_ );
		}
		
		virtual float GetNowAnimationTime() const
		{
			return ( m_pXSkinAnim != NULL ? m_pXSkinAnim->GetNowAnimationTime() : -1.0f );
		}

		virtual void SetNowAnimationTime( const float fAnimCurrentTime_ )
		{
			if ( NULL != m_pXSkinAnim )
				m_pXSkinAnim->SetAnimationTime( fAnimCurrentTime_ );
		}

		inline	void UpdateUserAllStatus()
		{
			UpdateSocketDataAndEnchantData();
			UpdatePassiveAndActiveSkillState();
		}
		void SetEnterCashShop(bool bVal);

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		bool GetHasHyperModeItem() const { return m_bHasHyperModeItem; }
#endif //SERV_9TH_NEW_CHARACTER
		void SetHasHyperModeItem(bool bVal){m_bHasHyperModeItem = bVal;}

#ifdef MODIFY_SET_DAMAGE_DATA
		virtual	void				SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_, float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT		/// ������ ���� ���� �׷α� ������Ʈ ��ȯ ����
		void SetChangeGroggyState( bool bChangeGroggyState_ ) { m_bChangeGroggyState = bChangeGroggyState_; }
#endif NOT_CANCEL_BBT_STUN_BY_HIT

		float GetNowTimeAnimFuture() const
		{
			if ( NULL != m_pXSkinAnimFuture )
				return m_pXSkinAnimFuture->GetNowAnimationTime();
			else
				return 0.0f;
		}

		bool IsAnimFutureEnd() const
		{
			if ( NULL != m_pXSkinAnimFuture )
				return m_pXSkinAnimFuture->IsAnimationEnd();
			else
				return false;
		}
		

		// ������ // X2Game���� UI�� ��� �ϱ� ���� ��� �� �� �ֵ��� public���� �̵�.
		virtual float				GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;
		virtual float				GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const;

#ifdef NEXON_QA_CHEAT_REQ
		void SetInvincibleAndNoMpConsume_Cheat( bool bValue_ ) { m_bIsInvincibleAndNoMpConsume = bValue_; }
		bool GetInvincibleAndNoMpConsume_Cheat() const { return m_bIsInvincibleAndNoMpConsume; }
#endif //NEXON_QA_CHEAT_REQ


#ifdef EFFECT_TOOL
		void SetXSkinAnim_EffectTool( CKTDGXSkinAnim* pAnim_ ) { m_pXSkinAnim_EffectTool = pAnim_; }
#endif //EFFECT_TOOL

#ifdef FIX_SUMMONED_MONSTER_POSITION
		virtual void	CallSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
#endif //FIX_SUMMONED_MONSTER_POSITION


#ifdef CHECK_CAMERA_INFO		
		bool GetIsLineMapCamera() const { return m_bIsLineMapCamera; }
#endif //CHECK_CAMERA_INFO

#ifdef  ADDED_RELATIONSHIP_SYSTEM
		UidType GetRelationshipUnitUID() const { return m_RelationshipTargetUID; }
		void SetRelationshipUnitUID(UidType val) { m_RelationshipTargetUID = val; }
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef NEW_CHARACTER_EL
		// (���߷� ����)
		float GetAddAccuaracyPercent() const { return min(m_fAddAccuaracyPercent, 1.f); }
		void SetAddAccuaracyPercent(float val) { m_fAddAccuaracyPercent = val; }

		// (���� ���� ũ��Ƽ�� �ߵ� Ȯ�� )
		float GetAddMagicAttackCriticalRate() const { return min(m_fAddMagicAttackCriticalRate, 1.f); }
		void SetAddMagicAttackCriticalRate(float val) { m_fAddMagicAttackCriticalRate = val; }

		// (���� ���� ũ��Ƽ�� ������ ����)
		float GetAddMagicAttackCriticalDamageRate() const { return min(m_fAddMagicAttackCriticalDamageRate, 1.f); }
		void SetAddMagicAttackCriticalDamageRate(float val) { m_fAddMagicAttackCriticalDamageRate = val; }
#endif // NEW_CHARACTER_EL
		const vector< int >& GetSetItemOptions(){ return m_vecSetItemOptions; }
		int GetMyUnitSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_ );
#ifdef FIX_FORCE_DOWN_AND_DETONATION_BUG
		void ForceDownAndDetonationProcess();
#endif //FIX_FORCE_DOWN_AND_DETONATION_BUG

#ifdef MODIFY_AFTER_IMAGE
		void UpdateAfterImageEquip() { if( NULL != m_pAfterImage ) m_pAfterImage->UpdateEquipView( m_pXSkinAnim.get() ); }
#endif // MODIFY_AFTER_IMAGE

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		/// ��ų ���� ������Ʈ ���̵� �ش��ϴ� ������Ʈ ���̵� ��ȯ
		virtual void GetStateIDBySecretSkillTriggerStateID( IN OUT int& iStateID_ ) const {};
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
		void ApplyBuffByUseHyperMode();								/// ������ ������ ������ �����ϴ� �Լ�
		void EraseDebuffAndAddMP( IN const float fMPValue_ );		///  (define ADJUST_SECRET_ITEM_OPTION) ����� ����, ���� ������ ������� mp ȸ������ ���� <- �ɷ��ִ� ������� �����, ���� ������ŭ MP ȸ��

#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
		const float GetSocketOptionHyperCoolTime(){ return m_fSocketOptionHyperCoolTime; }
#endif //ADJUST_SECRET_ITEM_OPTION

#endif // HAMEL_SECRET_DUNGEON

		virtual void		ApplyWorldLightColor( OUT CKTDGXRenderer::RenderParam* pRenderParam_ );

#ifdef SKILL_CANCEL_BY_HYPER_MODE // ����ȯ
		virtual const int GetHypetMpdeStateID() { return static_cast<int>( USI_HYPER_MODE ); }
#endif //SKILL_CANCEL_BY_HYPER_MODE

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		virtual void		UpNowDPValueByDamage( IN const float fDamage_ ) {}				/// �ֵ��� ��, �ǰݽ� DP ���� ó��
		virtual const float	GetDPValue( IN const float fDPValue_ ) { return 0.f; }			/// �ֵ��� ��, DP ����
		virtual void		SetDPValue( IN const float fDPValue_ ) {}						/// �ֵ��� ��, DP ����
		virtual const bool	GetIsFormationMode() const { return false; }					/// ���� ��� ���� ���� ��ȯ �Լ�
		virtual void		SetIsFormationMode( IN const bool bIsFormationMode_ ) {}		/// ���� ��� ���� ���� ���� �Լ�

		virtual void		SetStasisfieldHitValue( IN const float fHitValue_ ) {}			/// �����̽ý� �ʵ� Ÿ��ġ ���� �Լ�
		virtual void		SetStasisfieldDamageValue( IN const float fDamageValue_ ) {}	/// �����̽ý� �ʵ� �ǰ�ġ ���� �Լ�

		virtual const bool	GetUpDPByAttackThisFrame() { return false; }					/// Ÿ�ݽ� �� �����ӿ� DP ȹ��� ���� Flag ��ȯ
		virtual void		SetUpDPByAttackThisFrame( IN const bool bval_ ) {}				/// Ÿ�ݽ� �� �����ӿ� DP ȹ��� ���� Flag ����
		virtual const bool	GetUpDPByDamageThisFrame() { return false; }					/// Ÿ�ݽ� �� �����ӿ� DP ȹ��� ���� Flag ��ȯ
		virtual void		SetUpDPByDamageThisFrame( IN const bool bval_ ) {}				/// Ÿ�ݽ� �� �����ӿ� DP ȹ��� ���� Flag ����

		/// Ư�� ������ ��, �ִϸ��̼��� �ٲٴ� �Լ�
		virtual void		SetCustomAnimName( IN const UserUnitStateData& UserStateData, IN OUT wstring& wstrAnimName ) {}

		/// ��ų ���̵� �ش��ϴ� SA���� �޾ƿ��� �Լ�
		const float			GetSpecialAbilityValue( IN const CX2SkillTree::SKILL_ABILITY_TYPE eSAValue, IN const CX2SkillTree::SKILL_ID eSkillID ) const;

		/// MP ���� �� ������ ���� ���� ( ���� �� ĳ���� ���� ���� �Ǿ� �־�����, �� ���� �Լ��� ���� �־ X2GUUser�� �̵� )
		void				CreateNotEnoughMPEffect( IN const D3DXVECTOR3 vPos, IN const float fDegreeX, IN const float fDegreeY, IN const float fDegreeZ );

		/// ���ݿ� �ʿ��� MP�� ����ϸ� true, ������� ������ false�� �Բ� ���� ����Ʈ �߻�
		bool				IsEnoughMPForAttack( IN const float fConsumeMP, IN const D3DXVECTOR3 vPos, IN const float fOffsetPos = 0.f, 
			IN const D3DXVECTOR3 vRotDegree = D3DXVECTOR3( 0.f, 0.f, 0.f ) );

		const bool			CheckElapsedTimeRidingWaitHabit() { return m_ElapsedTimeRidingWaitHabit.CheckElapsedTime(); }
		const int			GetAirDashCount(){ return m_iAirDashCount; }

#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		virtual void		SetMutationCount( IN const int iMutationCount_ ) {}						/// �ֵ��� ��, ���� ����Ʈ ����
		virtual void		SetReverseReactorTransitionDamage( IN const float fFinalDamage_ ) {}	/// ���� ������ �� ��ȯ �� ���� �Լ�
		const int			GetSkillLevelValue( IN const CX2SkillTree::SKILL_ID eSkillID_ );		/// ��ų ���̵� �ش��ϴ� ��ų ������ �޾ƿ��� �Լ�
		void				SetEffectSoundParticle( IN const D3DXVECTOR3& vPos_, IN const wstring& wstrEffectName_ );		/// ȿ���� ����Ʈ ��� �Լ�
#endif //SERV_ADD_LUNATIC_PSYKER

		//{{ Public Function End

#ifdef  SUPER_ARMOR_TIME
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const std::vector<D3DXVECTOR2>&   GetVecNowSuperArmorTime() const { return m_vecNowSuperArmorTime; }
        std::vector<D3DXVECTOR2>&   AccessVecNowSuperArmorTime() { return m_vecNowSuperArmorTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif  SUPER_ARMOR_TIME

#ifdef CHEAT_SELF_DAMAGE // ����ȯ
		void SetSelfDamage( IN int iValue_, IN int iFrontDamage_ );					/// �ڽſ��� �������� �ִ� ���
#endif //CHEAT_SELF_DAMAGE

#ifdef FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE // ����ȯ
		bool ProcessSkillAtDie( IN const float fFinalDamage_, IN CX2DamageManager::DamageData* pDamageData_ = NULL );	/// ��� ������ �ߵ��Ǵ� ��ų �˻�
#endif //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE

#ifdef FIX_SKILL_SLOT_CHANGE_BUG
		bool IsValideSlotChange() const { return ( m_fSlotChangeLeftCoolTime > 0.f ) ? false : true; } 
		void SetSlotChangeCoolTime(float val) { m_fSlotChangeLeftCoolTime = val; }
#endif // FIX_SKILL_SLOT_CHANGE_BUG

#ifdef FIX_SHOOTING_MEGABALL_AT_JUMP_LINE_MAP
		virtual void InitInpuDataProcess();	
#endif // FIX_SHOOTING_MEGABALL_AT_JUMP_LINE_MAP

#ifdef SERV_9TH_NEW_CHARACTER
		void InitPhaseShift();
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_EVE_SYSTEM_2014		// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�
		virtual		void		AddManeuverGauge(float fVal) {}				// �̺� �⵿ ������ ���� ���� �Լ�
		virtual		bool		IsWaitManeuverCore () { return false; }	// �̺� �⵿ �ھ ���� ���� �����ΰ�?	
		virtual		void		SetAttackManeuverCore ( CX2DamageManager::DamageData & pDamageData ) {}		//�̺� �⵿ �ھ ���� ���·� ��ȯ�Ѵ�.
		virtual		bool		IsSkillSummonedMonster ( CX2UnitManager::NPC_UNIT_ID eUnitID ) { return false; }	// ��ų�� ��ȯ�� �����ΰ�? �� ĳ���� ���� ����
#endif // ADD_EVE_SYSTEM_2014	// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�


	protected:
		//{{ Protected Function Start
		virtual void				Verify();
		virtual void				InitDevice();
		virtual void				InitSystem();
		virtual void				InitStat();
		virtual void				InitComponent();

		virtual void ParseCommonRandomState() = NULL;
		void ParseCommonRandomState( const char* tableName );


		
		///////////////////////////////////////////////////////////////
		virtual void				InitMotion();
		virtual	void				InitSocketData();

		bool						CanApplySocketDataByGameType( const CX2SocketItem::SocketData& kSocketData_ );
		//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
		void						SumSocketOption( const vector<int>& vecSocketOption, const float fAtkMagic, const int iSocketLevel_ );
		//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

		virtual void				InitSetItemOptions();
		virtual void				InitEnchantData();
		virtual void				InitEqip();
		virtual void				InitState();


		void InitStateCommonRandom( OUT std::string &tableNameStartUTF8, OUT std::string &tableNameWinUTF8, OUT std::string &tableNameLoseUTF8 );
		
		void						ProcessAcceleraterBuff( CX2SkillTree::SKILL_ID eSkillID, bool bHyperMode );	/// ���������ͷ� ���� ����
		virtual	float				GetAdditionalAccelBuffTime( const CX2SkillTree::SKILL_ID eSkillID_ ) const { return 0.0f; }	/// ���������ͷ� ���� �߰��ð�
		void						ApplyAcceleratorBuffToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const float fAddtionalAccelBuffTime_, const D3DXVECTOR3& vMyPos_ );
		virtual void				ApplyAuraAcceleratorBuffToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const float fAddtionalAccelBuffTime_, const D3DXVECTOR3& vMyPos_ );
		virtual bool				CanApplyBuffToGameUnit() const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual const std::vector<BUFF_TEMPLET_ID>& GetVecImmunityAtThisState() const   { return m_vecImmunityAtThisState; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		virtual void				SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ ) = NULL;
		void						SetEquippedSkillFunc( const CX2Unit::UnitData& kUnitData_, const bool bSlotB_ );
		virtual void				SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ ) = NULL;

		virtual void				SetEquippedSkillLevelCommon( const CX2Unit::UnitData& kUnitData_, bool bSlotB_ );
		const CX2GUUser::UserUnitStateData&			GetPressedSkillState( const int iPressedIndex_ );

		void						CalcEquippedSkillLevelPlusAndMPCostBySlotID( CX2UserSkillTree& cUserSkillTree_, const int iSlotIndex_, const bool bSlotB_ );
		void						CalcEquippedSkillLevelPlusAndMPCost( CX2Unit::UnitData& kUnitData );

		void						DoStateEndStartFuture();
		void						DoStateEndStart();
		void						DoFrameMoveFuture();
		void						DoFrameMove();

		virtual	void				DyingStart();
		virtual void				CommonStateStartFuture();
		virtual void				CommonStateStart();
		virtual void				CommonFrameMoveFuture();		

		void CommonFrameMoveFuture_ExtraDamage();
		void CommonFrameMoveFuture_TimedEvent();

		virtual void				CommonFrameMove();


//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//		void HeadIKFrameMove( double fTime, float fElapsedTime );
//#endif HEAD_INVERSE_KINEMATICS_TEST


		void CommonFrameMove_Invincible();
		void CommonFrameMove_Particle();
		void CommonFrameMove_TimedEvent();

#ifdef CHUNG_SECOND_CLASS_CHANGE
		//void CommonFrameMove_ChainLockOn();
		//void SetLockOnCount( WCHAR* pwstrName_, float fPowerRate_, D3DXVECTOR3 vPos_, int iCount_ = 0 );
#endif

		//void PlayLuaSubCamera( int iCameraTableIndex );
		virtual void				CommonCameraMove();
		virtual void				CommonEventProcess();
		virtual void				CommonStateEndFuture();
		virtual void				CommonStateEnd();
		virtual void				DetonationEventProcess();

//{{ robobeg : 2008-10-28
		//virtual void				CommonRender();
        virtual RENDER_HINT			CommonRender_Prepare();	//optimization
//}} robobeg : 2008-10-28
		virtual void				CreateStepDust();	// kimhc // 2010-11-19 // ���θ��� eDustType�� ���� ������ ��ƼŬ�� �÷�����(�� ĳ���Ϳ� �ִ� �ߺ� �ڵ带 �Űܿ�)

#ifdef GRAPPLING_TEST
		virtual void				GrappledEventProcess();
		virtual void				GrappledPhysicProcess( bool bFuture );
#endif GRAPPLING_TEST

		virtual void				PhysicProcess();


		//void PhysicProcess_FastUpwardWind( CKTDGLineMap* pLineMap );
		void PhysicProcess_Jumping( CKTDGLineMap* pLineMap );
		void PhysicProcess_Bungee( CKTDGLineMap* pLineMap );
		void PhysicProcess_Portal( CKTDGLineMap* pLineMap );
		void PhysicProcess_PortalInBattleField( CKTDGLineMap* pLineMap );

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        bool    ProcessCanPushUnit( CX2GameUnit* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vMyPos_, int iDirection );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//        bool    ProcessCanPushUserUnit( CX2GUUser* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, const D3DXVECTOR2& vMyUnitMinMaxYCur_, OUT D3DXVECTOR3& vMyPos_ );
//		bool    ProcessCanPushNPCUnit( CX2GUNPC* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vMyPos_, int iDirection );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
		bool    CheckPassUnit( const CX2GameUnit* pGameUnit_, OUT D3DXVECTOR3& vPos_ );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		FORCEINLINE void	ProcessCanPushUnit( const CX2GameUnit* pGameUnit_, const D3DXVECTOR2& vMyUnitMinMaxY_, OUT D3DXVECTOR3& vPos_ );
		FORCEINLINE void	ProcessCanPassUnit( const CX2GameUnit* pGameUnit_, OUT D3DXVECTOR3& vPos_ );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef LINKED_LINEMAP_JUMP_USER_TEST
		void LinkJumpEventProcess();
		void LinkJumpPhysicProcess( double fTime, float fElapsedTime );
		bool CanLinkJumpNow( float fElapsedTimeLastLinkJump = 0.f );
		void LinkJumpStart();
		void LinkJumpEnd();
#endif LINKED_LINEMAP_JUMP_USER_TEST


		bool CommonSpecialAttackEventProcess( IN const CX2UserSkillTree& cUserSkillTree_, OUT const CX2UserSkillTree::SkillSlotData* & pSkillSlotDataPressed_, OUT int& iSkillSlotIndexPressed_ );

		virtual bool				SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND ) = NULL;

		virtual void				NoStateChangeActionFrameMove() = NULL;		
		virtual bool				SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet ) = NULL;

#ifdef ACTUAL_CB_CONSUME
		virtual	USHORT				GetActualCBConsume( CX2SkillTree::SKILL_ID eSkill_ID_, int iSkillLevel_ );
#endif //ACTUAL_CB_CONSUME

#ifdef ADD_MEMO_1ST_CLASS //��â��
		bool CheckSkillUseCondition( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition, CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseConditionSkillTemplet );
#else //ADD_MEMO_1ST_CLASS
		bool CheckSkillUseCondition( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition, const CX2SkillTree::SkillTemplet* pSkillTemplet );
#endif //ADD_MEMO_1ST_CLASS

		bool CheckSkillUsePVPOfficial( CX2SkillTree::SKILL_ID eSkill_ID_ );

#ifdef FIX_NO_STATE_SKILL_BUG
		virtual bool IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ ){ return true;}
#endif // FIX_NO_STATE_SKILL_BUG

		virtual	bool				IsSuperArmor() const;

		bool						EventTimer( float fTime, bool bFuture );
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool						EventCheck( float fTime, bool bFuture );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        void						PushFrameData( bool bSendForce );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool                        PopFrameData( bool bPopOnce, OUT bool& bFrameBufferPass );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool                        PopFrameData( bool bPopOnce );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//		void						PushFrameData();
//		void						PopFrameData();
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	
		//virtual float				GetPowerRate(); // // moved by wonpok. 20091215. to public.

		//optimization
		void						ActiveSkillShow( const WCHAR* pBoneName, const SkillCutInSet *s_SkillCutInSet, int iCutInSet, int iCutIn, bool bHyper, bool bOnlyLight = false);
		void						ActiveSkillShow( const WCHAR* pBoneName, const WCHAR* pBackFaceTexName, D3DXVECTOR2 size, D3DXVECTOR3 pos, bool bHyper, bool bOnlyLight = false );
		void						ActiveSkillShow( D3DXVECTOR3 bonePos, const WCHAR* pBackFaceTexName, D3DXVECTOR2 size, D3DXVECTOR3 pos, bool bHyper, bool bOnlyLight = false );

		bool						CheckDashAngleChangeTime();
		float						GetDashCameraAngleDegree();

		virtual float				GetBasicReducedDamagePercent();


		virtual float				GetReducedDamagePercentByMonsterAttack( int monsterID );
		
		virtual void				HyperModeBuffEffectStart();
		virtual void				HyperModeBuffEffectEnd();
		
		virtual float 				GetDamageUpByAMADS();


		//��æƮ Ư�� �������� ���� ����� ���⼭ ���� ����!!
		virtual void				GetDamageReduceByResistExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE damageType, float& fFinalDamage );
	

		



		virtual bool				IsImmuneBySocketOption( CX2DamageManager::EXTRA_DAMAGE_TYPE exDamageType );

		void ResetAnimSpeed( float fAnimSpeed, bool bSpeedUpByItem, bool bFuture, float fExtraSpeedRate = 1.f );

#ifdef UNIT_EMOTION
		void						EmotionFrameMove();
#endif

		void DamageDoubleAttack();



		virtual void CommonFrameMoveStateAbnormalityFrozen();

#ifdef EDT_WATER_HOLD_TEST
		virtual void CommonFrameMoveStateAbnormalityWaterHold();
#endif EDT_WATER_HOLD_TEST

#ifdef EVE_ELECTRA
		virtual void CommonFrameMoveStateAbnormalityFlashBang();
#endif EVE_ELECTRA

#ifdef GRAPPLING_TEST
		virtual void CalcGrapplingPosition( bool bFuture )
		{
			if( true == bFuture )
				m_FrameDataFuture.unitCondition.m_GrapplingState.CalcGrapplingPosition();
			else
				m_FrameDataNow.unitCondition.m_GrapplingState.CalcGrapplingPosition();
		}

		virtual void ResetGrapplingState( bool bOnlyMe = false )
		{
			if( false == bOnlyMe )
			{
				if( true == IsGrappled( false ) )
				{
					SetPositionOnLine( GetPos(), GetLastTouchLineIndex() );
				}
			}

			m_FrameDataFuture.unitCondition.m_GrapplingState.ResetGrapplingState( bOnlyMe );
			m_FrameDataNow.unitCondition.m_GrapplingState.ResetGrapplingState( bOnlyMe );
		}

		virtual void SetUnitGrapplingMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
		{
			if( true == bFuture )
				m_FrameDataFuture.unitCondition.m_GrapplingState.SetUnitGrapplingMe( eGameUnitType, unitUID );
			else
				m_FrameDataNow.unitCondition.m_GrapplingState.SetUnitGrapplingMe( eGameUnitType, unitUID );
		}

		virtual void SetUnitGrappledByMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
		{
			if( true == bFuture )
				m_FrameDataFuture.unitCondition.m_GrapplingState.SetUnitGrappledByMe( eGameUnitType, unitUID );
			else
				m_FrameDataNow.unitCondition.m_GrapplingState.SetUnitGrappledByMe( eGameUnitType, unitUID );
		}


		virtual CX2GameUnit* GetUnitGrapplingMe( bool bFuture )
		{
			KTDXPROFILE();

			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.m_GrapplingState.GetUnitGrapplingMe();
			else
				return m_FrameDataNow.unitCondition.m_GrapplingState.GetUnitGrapplingMe();
		}

		virtual CX2GameUnit* GetUnitGrappledByMe( bool bFuture )
		{
			KTDXPROFILE();

			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.m_GrapplingState.GetUnitGrappledByMe();
			else
				return m_FrameDataNow.unitCondition.m_GrapplingState.GetUnitGrappledByMe();
		}

		virtual bool IsGrappled( bool bFuture )
		{
			KTDXPROFILE();

			if( true == bFuture )
				return m_FrameDataFuture.unitCondition.m_GrapplingState.m_UnitUIDGrapplingMe > 0; 
			else
				return m_FrameDataNow.unitCondition.m_GrapplingState.m_UnitUIDGrapplingMe > 0; 
		}


#endif GRAPPLING_TEST



#ifdef RENA_SIEGE_MODE_ANGLE_TEST
		void LookAtIKFrameMove( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vRotationAxis, const float fRotationAngle );
#endif RENA_SIEGE_MODE_ANGLE_TEST

		float DoAimingTarget(float fDist, const D3DXVECTOR3& vCoefficient, bool &bIsTarget, D3DXVECTOR3 &vTargetPos, D3DXVECTOR3 &vLandPos, bool bGap = true );
		void RotateBone( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, const D3DXVECTOR3& vRotationAxis, const float fRotationAngle );

#ifdef ELSWORD_SECOND_CLASS_CHANGE
		void SetUnitReForm();
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
		void ResetPassiveCriticalRate() { m_fPassiveCriticalRate = 0.f; }
		void SetPassiveCriticalRate(float fVal) { m_fPassiveCriticalRate = fVal; }
		float GetPassiveCriticalRate() { return m_fPassiveCriticalRate; }
#endif

//{{ kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
		//{{ kimhc // 2010.6.14 //	�巡��м�Ʈ ���� �������� �߰�
		float GetOriginalMPChangeRate();
		void SetOriginalMPChangeRate(float val) { m_fOriginalMPChangeRate = val; }
		//}} kimhc // 2010.6.14 //	�巡��м�Ʈ ���� �������� �߰�

		float	GetNowMPChange();			// ���� �ڽ��� MP �������� ���� // ��! ����� ��� �������� ���� ���� �������� (����ΰ�� ���� MP�������� * 7.0�� ���ִ� ó���� ����)
		bool	InitSocketEffectEx( const CX2SocketItem::SocketData& kSocketData );		// SOCKET_ITEM_EFFECT_EX ���� ������ ó��
		CX2SocketItem::SOCKET_ITEM_EFFECT_EX ProcessSocketEffectEx( const CX2SocketItem::SOCKET_ITEM_EFFECT_EX eEx_, CX2GameUnit* pGameUnit_ );		// �ش� ��Ʈȿ���� �߻����� �ȵ����� ������, ���ϰ��� �߻����θ� �Ǵ���, �߻��� �ȵǸ� NONE�� ����

		float	GetManaRecoveryVal() const { return m_fManaRecoveryVal; }
		float	GetManaRecoveryTime() const { return m_fManaRecoveryDurationTime; }
		void	CheckNChangeManaRecoveryValueNTime( float fElpasedTime );		// �ڿ�����ȸ������ �÷��ִ� �ð��� üũ�� �Ŀ� time�� 0�̵Ǹ� valu�� 0.0f�� ���� �����ְ� �ڿ� ����ȸ���� ���� �����Ƿ� ������

		void	SetManaRecoveryVal( float fVal ) { m_fManaRecoveryVal = fVal; }
		void	SetManaRecoveryTime( float fTime ) { m_fManaRecoveryDurationTime = fTime; }

		void	SetIntervalTimeForCureDebuff( float fVal ) { m_fIntervalTimeForCureDebuff = fVal; }
		float	GetIntervalTimeForCureDebuff() const { return m_fIntervalTimeForCureDebuff; }
		void	CheckNChangeIntervalTimeForCureDebuff( float fElpasedTime );

		
		void	CreateBoneShield( const CX2SocketItem::SocketData& kSocketData );
		void	DeleteBoneShield() { SAFE_DELETE( m_pBoneShieldData ); }
		void	CheckAndDeleteBoneShield( float fElpasedTime );

		// kimhc // 2010-11-01 GameUnit���� �ű�
		/*static	bool GreaterExtraDamageTime( CX2DamageManager::ExtraDamageData* pFirst, CX2DamageManager::ExtraDamageData *pSecond );*/

#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
		const float GetSocketOptionCoolTime(){ return m_fSocketOptionCoolTime; }
		void SetSocketOptionCoolTime( const float fVal_ ){ m_fSocketOptionCoolTime = fVal_; }
		void CheckSocketOptionCoolTime( const float fElpasedTime_ );

		void SetSocketOptionHyperCoolTime( const float fVal_ ){ m_fSocketOptionHyperCoolTime = fVal_; }
		void CheckSocketOptionHyperCoolTime( const float fElpasedTime_ );

		void CheckSocketMpUpAttackedCoolTime( const float fElpasedTime_ );

		const float GetSocketReducedDamagePercentInCase( const CX2SocketItem::SocketData& kSocketData_ );
#endif //ADJUST_SECRET_ITEM_OPTION

#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)

		void				RenderRank(int iFinalLeft, int iFinalTop);
#ifdef SERV_INTEGRATION
		//{{ oasis907 : ����� [2010.5.17] // 
		void				RenderServer(int iFinalLeft, int iFinalTop);
		//}}
#endif SERV_INTEGRATION

		//{{ kimhc // 2010.6.16 
		// 1. GameUnit::Weapon::SetEnchantParticleShow() �� GUUser::SetShowEnchantedWeaponEffectAtHand()��
		// ���� ���� ��찡 �����Ƿ� �ѵ� ����
		// 2. NotifyShowObjectChanged()������ ���� ���� ȣ���ϵ��� �Ǿ��ִ°��� �Ѱ������� ȣ���ϵ��� ����
		// 3. virtual�� ���� GameUnit������ GameUnit::Weapon::SetEnchantParticleShow()�� ȣ���ϰ�
		// GUUser������ SetShowEnchantedWeaponEffectAtHand()�� ȣ���ϵ��� ����
#ifdef	ENCHANT_BUG_TEST
		virtual void SetShowEnchantWeaponParticle( bool bShow );
#endif	ENCHANT_BUG_TEST
		//}} kimhc // 2010.6.16 

#ifdef SERV_PET_SYSTEM
		void CommonStartStateEnd();	
		void WinStateStart();
		void LoseStateStart();
#endif

		//{{ kimhc // 2010.7.12 // �������� ������ HOLD �� �� �ֵ��� ��
		// HOLD �����϶� ����� ������Ʈ ���� �� ������( �⺻�� m_DamageAirFall )��
		int GetHoldStateID() const { return m_iHoldStateID; }
		void SetHoldStateID( int val ) { m_iHoldStateID = val; }
		void SetHoldStateByReactType( CX2DamageManager::REACT_TYPE eReactType = CX2DamageManager::RT_DOWN );
		//}} kimhc // 2010.7.12 // �������� ������ HOLD �� �� �ֵ��� ��

		void PlayCommonBuffMinorParticle();	// CommonBuffFrame���� ���̴� ��ƼŬ ���
		bool GetSkillSlotIndexUsed( OUT int& iSkillSlotIndex_, OUT bool& bSlotB_ );	// ���� ������ �ε��� �� B���� ��뿩�θ� ����

//{{ kimhc // 2010.12.6 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#ifdef ADD_MEMO_1ST_CLASS
		bool SetDamageData( const char* pszTableNameUTF8_, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		void SetDamageData( const char* pszTableNameUTF8_ );
#endif //ADD_MEMO_1ST_CLASS

		void CreateWeapon( 
            IN const CX2Item::ItemTemplet* pItemTemplet_, 
            CX2EqipPtr pEquipWeaponPtr_ );

	#ifdef	REAL_TIME_ELSWORD
		bool GetAddItemAndRemoveItemToViewList( OUT CX2EqipPtr& pEquipPtrToRemove_, IN OUT CX2Item** ppItemToAddInViewList_, IN CX2Item* const pItemToRemoveInViewList_, OUT int& iIndexInViewListForRemove_ );
	#endif	REAL_TIME_ELSWORD

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.6 //  2010-12-23 New Character CHUNG

		void AddDefenceEnchantData( CX2EnchantItem::ENCHANT_TYPE eEnchantType, int iEnchantValue );
		void AddExtraDamageImmuneData( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType, float fImmuneValue );
//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		void AddEnchantStatFromPassiveSkillToDefenceEnchantData();
		void AddExtraDamageImmuneRateFromPassiveSkillToImmuneData();
		const float GetEnchantStatFromPassiveSkillByEnchantType( int iEnchantType );	// ��þƮ Ÿ�Ժ� ��ġ �� ����

		float GetAddAnimSpeedFactor() const { return m_fAddAnimSpeedFactor; }	// û 1�� ����, ���� ������� ������ ��� ��ų�� ���� �ִϸ��̼� �ӵ� ����
		void SetAddAnimSpeedFactor(float fAddAnimSpeedFactor_) { m_fAddAnimSpeedFactor = fAddAnimSpeedFactor_; }
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // û 1�� ����

		//{{ kimhc // 2011.4.24 // ������� 2�ܰ�, �巡���� ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
		void AddEnchantStatFromDragonBreathToDefenceEnchantData();	
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011.4.24 // ������� 2�ܰ�, �巡���� ����

#ifdef SUPER_SOCKET_IN_TITLE
		void SetSuperArmorSocket();
#endif
		

		//{{ mauntain : ����ȯ [2012.07.26] Ŭ��ŷ �޸� ������ ��ٸ� ���� ���� ( �۾��� : ���ҵ� �븸 ������ )
#ifdef EVE_MEMO5_BUG_FIX
		void SetAdditionalMPChangeRateByPassive(float val) { m_fAdditionalMPChangeRateByPassive = val; }
#endif EVE_MEMO5_BUG_FIX

		bool GetShowCutInAndChangeWorldColor();
		void ChangeWorldColorByHyperMode();
		bool GetShowActiveSkillShow() const;
		
		void ResetNotBusyTimerOnSpecificState();	// �ٻ� ���� Ÿ�̸Ӹ� ����
		virtual bool ShouldResetNotBusyTimer();	// �ٻ� ���� Ÿ�̸Ӱ� ���� �Ǿ� �ϴ� ��Ȳ ���� �Ǵ�
		
		virtual void SetCanPassUnit( const bool bFlag_ )
		{
			m_FrameDataNow.stateParam.bCanPassUnit = bFlag_;
			m_FrameDataFuture.stateParam.bCanPassUnit = bFlag_;
		}

#ifdef REALTIME_SCRIPT_PATCH
		void SetScriptFileName();
#endif //REALTIME_SCRIPT_PATCH

#ifdef ARA_CHANGE_CLASS_FIRST
		/// ������ // 2013-01-10 // ȸ�� ���� ���� ( ����� �Ҽ��� ��ȭ�������θ� ȸ�ǰ� �����մϴ�. )
		void SetCanAvoidance(bool bCanAvoidance_ ) { m_bCanAvoidance = bCanAvoidance_; }
		bool GetCanAvoidance() const { return m_bCanAvoidance; }
#endif //ARA_CHANGE_CLASS_FIRST

		void StopTime_StateStart();

#ifdef SERV_SKILL_USE_SUBQUEST
		void Send_SKILL_USE_REQ( CX2SkillTree::SKILL_ID eSkillID );
#endif SERV_SKILL_USE_SUBQUEST

		virtual void DamageDataChangeProcess();

#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#ifdef ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_ );
#endif // ADD_MEMO_1ST_CLASS

			void CommonFrameMove_InsertBuffFactor();
#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		
		virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ ){}
		//}} Protected Function End

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
		virtual void ChangeDamageData () {}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

#ifdef BALANCE_PATCH_20131107
		//�� ���ָ��� Enchant�� ����� �� �����ؾ� �ϴ� �Լ� ����. NPC���� �ʿ���� �����Ͽ� GUUser�� �ۼ�.
		virtual void				SetSpecificValueByEnchant(){}
#endif //BALANCE_PATCH_20131107

#ifdef MODIFY_NO_STATE_SKILL_DURING_DAMAGE_REACT
		bool IsAvaliableActionDuringDamageReact( const UCHAR ucNowAction_ ) const; // �ǰ� �� ��밡���� Action
		void GetActionSyncData( OUT map< int, UCHAR >& mapActionSyncData_, IN const vector<SyncData>& ReceiveSyncDataList_ ) const;
		void SetActionSyncData( OUT vector<SyncData>& ReceiveSyncDataList_, IN const map< int, UCHAR >& mapActionSyncData_ ) const;
#endif // MODIFY_NO_STATE_SKILL_DURING_DAMAGE_REACT



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void    ClearAnimEventTimerOneshotNow();
        void    ClearAnimEventTimerOneshotFuture();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
		bool	IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId );
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
	protected:
		//{{ Protected Variable Start
		UserUnitStateData								m_FutureStateData;	/// Future�� StateData
		UserUnitStateData								m_NowStateData;	/// Now�� StateData
		UserUnitStateDataMap							m_StateList;	///Key: StateID, Value: UserUnitStateData
		
		int												m_MachineGunWaitState;	/// �̰� �Ⱦ��� �׳� ���� �ϴ°�... ���ҵ常 �ְ� �ٸ��� ����...
		int												m_ChargeMpState;	/// MP�� �����ϴ� ������Ʈ... ������ ���̻�, û�� ���� ������Ʈ��.. �������� �׳� Wait�� �������

		CKTDXTimer	m_TimerPepperRun;		/// û�����..? �ֱٿ� �ٽ� ���...optimization
		float		m_fPepperRunTime;		/// û����� ���� �ð�



#ifdef SERV_APRIL_FOOLS_DAY		//������ �̺�Ʈ 

		bool m_bIsFoolsDay; //�ش� ��¥���� ��ų �ƽ� �ٲٱ� ���� ��¥ üũ ���� �߰�

#endif //APRIL_FOOLS_DAY


//#ifdef PVP_BOSS_COMBAT_TEST
//		
//		int			m_FrozenState;
//		
//#endif PVP_BOSS_COMBAT_TEST



//{{ robobeg : 2008-10-28		
		//wstring											m_StateTableName;
		//wstring											m_StateTableNameFuture;
//}} robobeg : 2008-10-28		

		//optimization
		//FrameData										m_FrameDataFutureBefore;	/// ������
		FrameData										m_FrameDataFuture;	
		FrameData										m_FrameDataNowBefore;	/// syncData�� IsRight�� ���
		FrameData										m_FrameDataNow;
		FrameData										m_FrameDataDead;		/// ����,landPostion, dirDegree, lastTouchLineIndex�� �ʿ��� �Ŷ�� �װ͸� ������ ����.. ĳ���Ͱ� �׾ ������� �ٷ� ������ ������ ����Ÿ
		int	m_DeadStageIndex;		/// ������, ĳ���Ͱ� �׾ ������� �ٷ� ������ �������� ��ȣ
		int m_DeadSubStageIndex;	/// ������, ĳ���Ͱ� �׾ ������� �ٷ� ������ �������� ��ȣ	

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef  SUPER_ARMOR_TIME
        std::vector<D3DXVECTOR2>                        m_vecNowSuperArmorTime;
#endif  SUPER_ARMOR_TIME
#ifdef SKILL_BALANCE_PATCH
		bool	m_bFutureCanPassUnit;		// �ٸ� ������ �н� �� �� �ִ��� �ΰ� ������.. ������ CommonStateStartFuture�� ��� �Ľ� �κ��� ������ �� �κ��� �����ص� �� ��..
#endif
        bool    m_bFutureApplyAnimMove;
        bool    m_bNowFallDown;
		bool											m_bDisableGravity;				/// ����ü �̰� ��... ���� �ִ���... User�� ��� ��...�ڵ峻���� �߷� ���� �����ϵ��� �� �� ����մϴ�
		bool											m_bDisableGravityInScript;		/// ĳ����, ���� ��ũ��Ʈ���� �߷� ���� �����ϵ��� �� �� ����մϴ� ��� ������ �̰͵� User�� ��� �ϴ±�...
		D3DXVECTOR2										m_vDisableGravityInScriptTime;	/// �߷� ���� �ð� (X�� ���۽ð�, Y�� ����ð�)
#ifdef  AISHA_SECOND_CLASS_CHANGE
		D3DXVECTOR2										m_vFutureIgnoreLineTime;				/// �� �ִϸ��̼� �ð� ������ �������� �߿��� ���� üũ�� �����Ѵ�. ��, ������ ��� ���� �ʴ´�. (�����ϰ�, ��ƿ������ ����ϴ� ��) NPC�� ��� ���̳�..
#endif  AISHA_SECOND_CLASS_CHANGE
        float                                           m_fNowSlashTraceTipWide;
        std::map< int, D3DXVECTOR3 >                    m_mapNowAddSlashTrace;
        float                                           m_fCommonDamageChangeTime;
//{{ kimhc // 2011.1.21 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		CX2DamageManager::HITTED_TYPE					m_eHittedTypeAtState;	// ������Ʈ �� HittedType ����, �̰͵� �ϴ��� û�� ��� �ϴ� �ɷ� �˰� �ֱ� �ѵ�...
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // û 1�� ����

        D3DXVECTOR2                                     m_v2NowAfterImageTime;
        std::vector<KAttackTimeProjSeqName>             m_vecAttackTime;
		vector<D3DXVECTOR3>								m_StopAllList;
		vector<D3DXVECTOR2>								m_StopOtherList;
		vector<D3DXVECTOR2>								m_StopMyList;
		vector<D3DXVECTOR3>								m_Stop2AllList;
		vector<D3DXVECTOR2>								m_Stop2OtherList;
		vector<D3DXVECTOR2>								m_Stop2MyList;
        std::vector<SoundPlayData>                      m_vecSoundPlayData;
		std::vector<TIME_SPEED> m_vecSpeedFactor;	/// ������ �����, ��ũ��Ʈ �󿡼� Ư�� �ð��뿡 ���ǵ带 �ٶ� ���

		std::vector< EffectSetToPlay >                  m_vecEffectSetToPlay;	/// �÷��� �Ǿ� �ϴ� ����Ʈ ��

		bool m_bDeleteEffectSetOnStateEnd;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToDeleteOnStateEnd�� �־�� �ϴ��� ����
		bool m_bDeleteEffectSetOnDamageReact;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToDeleteOnDamageReact�� �־�� �ϴ��� ����
		bool m_bDeleteEffectSetOnDie;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToDeleteOnDie�� �־�� �ϴ��� ����
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		bool m_bDeleteEffectSetOnCustomState;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToPlay�� �־�� �ϴ��� ����
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE
		bool m_bHyperEffectSet;	/// ����Ʈ �¿� HYPER_NAME�� �����ϰ�, �̰��� ���� �ϸ� HYPER_NAME���� ������ ����Ʈ�� ����
		float m_fEffectSetLifeTime;	/// ����Ʈ ���� �÷��� �ð�...
#ifdef ADDITIONAL_MEMO
		int	m_iMemoId;
#endif
#ifdef CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT
		bool m_bIsCustomStateDeleteEffectOnDamageReact;
#endif // CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT

		// kimhc // 2010-12-08	
		std::vector<BUFF_TEMPLET_ID>	                m_vecImmunityAtThisState;	/// ������ ������Ʈ������ Immune �Ǿ���� ExtraDamage ����.. �ٵ� ���⿡ ���� �������׸� ���� �ǵ��� �س���... ����!!�ؾ���

#ifdef  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		map< UINT, float >                              m_mapStateInsertBuffFactor;	// Ư�� ������Ʈ�� ���� �ð��� �Ǹ� �ڱ� �ڽſ��� �Ŵ� �������� ����Ʈ
#endif  INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		UidType											m_OwnerUserUID;		/// UserUID ���� UID
		int												m_FrameBufferNum;	/// �Ϲ����� ��Ȳ���� ��Ŷ�� �� �����Ӿ� ��� �����°�
		CX2Unit*										m_pUnit;	/// X2Unit Ŭ����
		
		bool											m_bFirstDataReceive;	/// P2P ��Ŷ�� �ѹ��̶� �޾Ҵ��� �ȹ޾Ҵ���.. �ȹ޾����� OnFrameRender_Prepare�� �������� ����
		bool											m_bPopAgain;	/// POP�� �ѹ� ���� �� �Ŀ��� m_ReceiveSyncDataList.size() > m_FrameBufferNum(7) + m_AddFrameBuffer(1) �̸� Pop�� �ѹ� �� ���� �Ѵ�. 

		InputData										m_BeforeInputData;	/// �̰͵� pureRight�� pureLeft�� ������ ������ ��
		InputData										m_InputData;		/// �������� �������δ� �̰͵� ���� �ʿ䰡 ���� �� ������...
		InputData										m_OrgInputData;		/// ������ ���ؼ� ����ϴ� �� �ѵ�, pureX�� pureZ�� ������ ������ ��

		//optimization
		vector<SyncData>								m_SendSyncDataList;		/// ������ SyncData�� ����
		vector<SyncData>								m_ReceiveSyncDataList;	/// ���� SyncData�� ����
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        vector<SyncData>                                m_LastSendSyncDataList;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		map<float,bool>									m_EventTimeStampFuture;	/// CommonStateStartFuture���� clear, Key: �ִϸ��̼� Ÿ��, Value: false �� �̹� ������ �ִϸ��̼�
		map<float,bool>									m_EventTimeStampNow;	/// CommonStateStart �� û�� Ư�� AnimationEnd �ÿ� clear, Key: �ִϸ��̼� Ÿ��, Value: false �� �̹� ������ �ִϸ��̼�
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		//���� ������Ʈ
		CKTDXDeviceXSkinMesh*							m_pMotionFuture;	/// �ڱ� ĳ�������׸� �ʿ�
		CKTDXDeviceXSkinMesh*							m_pMotion;		
		CKTDGXSkinAnim*									m_pXSkinAnimFuture;	/// �̰͵� �ڱ� ĳ�������׸� �ʿ�

		//{{ dmlee 2009.3.12 ��׶��� �ε� ���� ũ���� ������ shared_ptr�� ����
		CKTDGXSkinAnimPtr								m_pXSkinAnim;		
		//}} dmlee 2009.3.12 ��׶��� �ε� ���� ũ���� ������ shared_ptr�� ����

		CKTDXDeviceXET*									m_pAniXET;	/// ��� ����

		CKTDXDeviceXSkinMesh*							m_pOrgMotionFuture;	/// ��� ����
		CKTDXDeviceXSkinMesh*							m_pOrgMotion;		/// ��� ���..
		CKTDGXSkinAnim*									m_pOrgXSkinAnimFuture;	/// ��� ����
		CKTDGXSkinAnimPtr								m_pOrgXSkinAnim;	/// ��� ����




        /// ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
        /// - jintaeks on 2009-01-12, 17:39
#ifdef FIX_MEMLEAK01
		CKTDXDeviceXSkinMesh*							m_pAfterImageMotion;
#endif
		//CKTDGXSkinAfterImage*							m_pAfterImage;	/// �̰�.. �Ⱦ��� ������ ������ ����...
		

		CKTDGSlashTrace*								m_pSlashTrace;	/// ��� ����
		CKTDGSlashTrace*								m_pSlashTraceTip;	/// ��� ����


		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHeadMarker;	/// �Ӹ����� ǥ��
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEmblem;		/// Īȣ
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticleHandle				m_hHeadMarkerParticle;
		CKTDGParticleSystem::CParticleHandle				m_hPart_Emblem_200;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticle*						m_pHeadMarkerParticle;
		CKTDGParticleSystem::CParticle*						m_pPart_Emblem_200;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHoldEffCenter;	/// ������.. ������ ���� Hold�� ��� �ϴ����� ���̱�...������ �̻��ѵ�?? Create�� Major���� �ϰ�, Get�� �� Minor���� �ϳ�?
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHoldEffCircle;	/// ������ �̻��ѵ�?? Create�� Major���� �ϰ�, Get�� �� Minor���� �ϳ�?

		/// �̰͵鵵 �ٵ� ��æƮ ���� ��������??? �ʿ� ���ݾ�...
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeaponCommon1;	/// ��æƮ ��ƼŬ
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeaponCommon2;	/// ��æƮ ��ƼŬ
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeaponCommon3;	/// ��æƮ ��ƼŬ

		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchantedWeaponCommon4;	/// Ʈ���� �Ӽ� ������ ��쿡 �տ� �ٴ� ��ƼŬ

		

#ifdef SKILL_CASH_10_TEST
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDrainParticle;	/// ���� �׼��� �¾��� �� ����� ����Ʈ..
#endif
		//{{ seojt // 2009-1-13, 16:09
		vector<CX2EqipPtr>								m_ViewEqipList;		/// ���� ���̴� ���
		//}} seojt // 2009-1-13, 16:09

#ifdef EQUIP_STRIP_TEST
		CX2ItemManager::EquipPositionMap				m_mapBasicEquip;		/// �⺻ ���
		std::map< CX2Unit::EQIP_POSITION, bool >		m_mapEquipStripped;		/// ��� Ż�ǵ� ��������.

		float											m_fTimeLeftToClearOutSuccessiveHit;
		int												m_iSuccessiveHitCount;
#endif EQUIP_STRIP_TEST

		int												m_AddFrameBuffer;	/// 1 ������ �����Ǿ� �ִµ�.. �� �ʿ��� �ɱ�??
		float											m_fFrameWaitTime;
		int												m_FrameWaitNum;
		float											m_fFrameSkipTime;
		int												m_FrameSkipNum;

		float											m_fRebirthTime;		/// ��Ȱ �ð�

		int												m_nDamageCount;		/// �ǰ�ȹ��

		bool											m_StopAdvanceTimeFuture;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		int												m_AdvanceTimeCount;		/// ������ 1�̿��� ���� �����ϴ� ���� ����
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_Spine;
		

		bool						m_bHyperModeUsed;	/// Ʃ�丮�󿡼� ������ ����ߴ��� üũ�ϱ� ���� �뵵
		float						m_fElaspedTimeLastGuideMessage;


		bool											m_bConnect;
		float											m_fConnectTimeOut;
		bool											m_bConnectSend;
		bool											m_bSwapSkillSlot;
#ifdef ATTACK_DELAY_GAGE
//{{AFX
		bool											m_bAddAttackDelayThisState;
		float											m_fLastAttackPassTime;
//}}AFX
#endif ATTACK_DELAY_GAGE

		bool											m_bAddForceDownGageThisState;
		float											m_fLastDamagePassTime;		/// �ֱ� Ÿ�� ���� �� ���� �ð�... �ٵ� �̰� ����ü �� ���� ���� �Ǿ� �ִ°�..?

		bool											m_bResurrectionTimeOut;


		float											m_fDevAddDamage;


		int												m_iNowSpecialAttack;		/// 1, 2, 3 special attack, 0: nothing, �Ʒüҿ��� �ʻ�� �Ʒö� ���		
		bool											m_bHoldSpecialAttack;		/// Ȧ���ߴٰ� �ʻ�⸦ ����Ѱ���~
		bool											m_bHoldingSpecialAttack;	/// �ʻ�⸦ hold�ϰ� �ִ���

//#ifndef NOT_USE_DICE_ROLL
//		DiceRoll*										m_pDiceRoll;	/// ���������� �ʿ�
//#endif //NOT_USE_DICE_ROLL

		//optimization
		CashItemAbility									m_CashItemAbility;


		// ���θ� ���� ���� ����
		D3DXVECTOR3										m_vAddJumpSpeed;
		bool											m_bSteppedJumpLinemap;


		float											m_fAnimSpeedRate;			/// �Ӽ� ������� ���� �ִϸ��̼� �ӵ��� �������� ���, m_fAnimSpeed�� ��������.
		float											m_fAnimSpeed;				/// ��ũ��Ʈ�� ������ �� ���º� �ִϸ��̼� �ӵ�, default = 1.f
#ifdef ADD_ANIM_SPEED_FUTURE
		float											m_fAnimSpeedFuture;			/// ��ũ��Ʈ�� ������ �� ���º� �ִϸ��̼� �ӵ�, default = 1.f, used in future state
#endif // ADD_ANIM_SPEED_FUTURE
		bool											m_bColdSlowApplied;
		bool											m_bFrozenSlowApplied;




		//optimization
		std::vector< wstring >							m_vecDungeonWinSpeech;

		float											m_fTimeToDashCameraChange;
		float											m_fAngleDegree;
		float											m_fTimeToReturnCameraAngle; //��� ������ ī�޶� �ޱ� �ǵ����� ������ ī��Ʈ 


		vector< int >									m_vecSetItemOptions;
        vector< int >                                 m_vecTitleOptions;

#ifdef PORTAL_LINEMAP_TEST
		bool											m_bEnterPortal;		
		bool											m_bLeavePortal;		
		CKTDXTimer										m_timerInPortal;
#endif PORTAL_LINEMAP_TEST
		
		float											m_fOriginalMPChangeRate; // ���̻�, ����ó�� charge���°� ���� �ִ� ��쿡 ������� �����ϱ� ����

		//{{ mauntain : ����ȯ [2012.07.26] Ŭ��ŷ �޸� ������ ��ٸ� ���� ���� ( �۾��� : ���ҵ� �븸 ������ )
#ifdef EVE_MEMO5_BUG_FIX
		float											m_fAdditionalMPChangeRateByPassive;	// ��ú꿡 ���� �߰��� mp ȸ����
#endif EVE_MEMO5_BUG_FIX
		//}}

		bool											m_bRidingMachine;
		bool											m_bLeaping;

		map< int, float >									m_mapNpcIdNDamageUpPercent; //Ư�� ���� ������ �� ������ �� ����.
		map< int, float >									m_mapNpcIDNDEvadeUpPercent;	//Ư�� ���Ͱ� �ϴ� ���ݿ� ȸ���� ����
		map< CX2DamageManager::EXTRA_DAMAGE_TYPE, float >	m_mapExtraDamageNImmunePer;


#ifdef SHOOTING_TEST
		CKTDXTimer										m_TimerGunFireCooling;
		CKTDXTimer										m_TimerGrenadeCooling;
#endif SHOOTING_TEST

		bool											m_bReserveForceDownForShock;


		CKTDXTimer										m_TimerRandomSeedSync;
		static const double								RANDOM_SEED_RESET_TIME;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		//optimization
//		KXPT_UNIT_USER_SYNC_PACK                        m_kXPT_UNIT_USER_SYNC_PACK;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		
#ifdef UNIT_EMOTION
		CX2Unit::EMOTION_TYPE							m_ePlayedEmotion;		
		bool											m_bReserveSitReadyEmotion;
#endif

		bool											m_bSocketSuperArmor;
		float											m_fSocketSuperArmor;
		bool											m_bStateSuperArmor;
#ifdef DUNGEON_ITEM
		//optimization
		bool											m_bSuperArmorByItem;
		float											m_fSuperArmorByItem;		
#endif

		//{{ kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		CX2EffectSet::Handle m_hEffectTooniLandEvent;
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ

#ifdef BUFF_TEMPLET_SYSTEM
		//���� ���� ���� ����Ʈ
		CX2EffectSet::Handle m_hEffectCrystalOfElBuff;
#endif BUFF_TEMPLET_SYSTEM

//#ifdef PVP_BOSS_COMBAT_TEST
//		CX2EffectSet::Handle m_hEffectSetBossMark;		
//		int			m_iFriendlyHittedCountAtFrozenState;
//#endif PVP_BOSS_COMBAT_TEST

#ifdef USER_HOLD
		bool		m_bHold;
		D3DXVECTOR3 m_vHold;
#endif

#ifdef SKILL_CASH_10_TEST
		CX2GameUnitoPtr	m_optrLastAttackUnit;
#endif

#ifdef NO_DETONATION
		bool				m_bNoDetonation;		// ���� ���ɿ��� 
		bool				m_bNowStateDamageReact; // ���� ������Ʈ�� �ǰ����� ���� ����� ������... 
#endif

		CX2ComboGuide*		m_pComboGuide;



#ifdef RENA_SIEGE_MODE_ANGLE_TEST
		bool m_bEnableLookAtIK_Spine;
		float m_fLookAtIKAngle_Spine;
#endif RENA_SIEGE_MODE_ANGLE_TEST
		bool m_bRotateBone;
		float m_fRotateBoneDegree;

		
		//optimization
		std::vector< CommonRandomState > m_vecRandomStartState;
		std::vector< CommonRandomState > m_vecRandomWinState;
		std::vector< CommonRandomState > m_vecRandomLoseState;


		SPECIAL_ATTACK_KEY_PRESSED m_eSpecialAttackKeyPressed;
		bool m_bSpecialAttackEventProcessedAtThisFrame; // �̹� �����ӿ� �ʻ�� Ű �̺�Ʈ ó�� �ߴ��� ���ߴ���


		int m_iSkillCutInSetIndex;
		int m_iSkillCutInSetSubIndex;

		//{{ kimhc // 2009-11-20 // ��� ��ų 3, 4����
#ifdef	GUILD_SKILL_PART_2
		//optimization
		GuildSkillData			m_GuildSkillData;			//  ��� ��ų�� ���õ� ������ ��� ����ü
		bool					m_bHaveSacrificeOfHero;		// ������ ����� ������ �ִ°�?
#endif	GUILD_SKILL_PART_2
		//}} kimhc // 2009-11-20 // ��� ��ų 3, 4����

#ifdef RAVEN_SECOND_CLASS_CHANGE
		float m_fPassiveCriticalRate;		// �߰� ũ��Ƽ�� Ȯ����ġ(�н��� ����)
		//optimization
		std::vector<TIME_PUSHPASS> m_vecCanPushUnit;
		std::vector<TIME_PUSHPASS> m_vecCanPassUnit;
//#ifdef  X2OPTIMIZE_USER_PASS_PUSH_UNIT_TIME_BUG_FIX
		std::vector<TIME_PUSHPASS> m_vecCanPushUnitNow;
		std::vector<TIME_PUSHPASS> m_vecCanPassUnitNow;
//#endif  X2OPTIMIZE_USER_PASS_PUSH_UNIT_TIME_BUG_FIX
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		float					m_fShadowOfDisasterRate;	/// ����� �׸��ڷ� �����Ǵ� ũ��Ƽ�� ����
		CurrentSkillKeyManager	m_sCurrentSkillKeyManager;	/// Ű �߰� �Է� ��ų ���� ����ü
		bool					m_bIsLimitCrusherCharge;	/// ���̺� ��ũ���� �ǽ�Ʈ - ����Ʈ ũ���� ���� ���� ����
#endif //UPGRADE_SKILL_SYSTEM_2013

//#ifdef TRANSFORMER_TEST
//		CX2GUNPC* m_pTransformer;
//#endif TRANSFORMER_TEST
		
		//{{ �ӱԼ� �ӱԼ� ���� ��ŸƮ ǥ�� ������ �� , ���� ���� ���� �Ŀ� ������ ��� ���ϵ��� ����
#ifdef FIX_QUICK_SLOT_USE_DUNGEON_PLAY
		bool	  m_bStarted;
#endif 
		//}}

		//{{ kimhc // 2010.2.10  // �̺� 2������ 
#ifdef	EVE_SECOND_CLASS_CHANGE
		bool					m_bHitNasodWeapon;	//	�̺� ���ҵ� ������ �¾Ҵ°�?
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.2.10 //	�̺� 2������

//{{ kimhc // 2010.4.2 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
		// �߰��� ���Ͼ����� ȿ���� Ű������ �ɼ�id���� �ִ´�.

		//optimization
		typedef map< CX2SocketItem::SOCKET_ITEM_EFFECT_EX, vector<int> > SOCKET_EFFECT_EX_MAP;
		SOCKET_EFFECT_EX_MAP m_mapSocketItemEffectEx; // value�� ���Ͽɼ�id

		float				m_fManaRecoveryDurationTime;	// ��Ʈ������ ���� ���� �ڿ�ȸ���翡 + �Ǵ� ȿ���� ���� �ð�
		float				m_fManaRecoveryVal;				// ���� �ڿ�ȸ�� �翡 + �Ǵ� ��

		float				m_fIntervalTimeForCureDebuff;	// ��Ʈ ȿ���� CureDebuff�� ���Ͽ� �̽ð��� 0�� ���� ������ �ٽ� �߻����� ����
		Bone_Shield*		m_pBoneShieldData;				// ��Ʈ ȿ���� ������

#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
		float				m_fSocketOptionCoolTime;		// ���� 5�� �ɼ� ��Ÿ��
		float				m_fSocketOptionHyperCoolTime;	// ������ ����Ǵ� �ɼ� ��Ÿ��
#endif //ADJUST_SECRET_ITEM_OPTION

#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // ��д��� �۾�(��Ʈ������ȿ��)
		CKTDXDeviceTexture*				m_pTextureRank;		/// ���� ��ũ ǥ�� �ؽ���
#ifdef SERV_INTEGRATION
		//{{ oasis907 : ����� [2010.5.17] // ���� ���� ���� ���� ������
		CKTDXDeviceTexture*				m_pTextureServer;	
		//}}
#endif SERV_INTEGRATION

#ifdef SERV_PET_SYSTEM
		int			m_iDieCount;
		PetCheer	m_petCheer;
#endif

//{{ kimhc // 2010.7.12 // �������� ������ HOLD �� �� �ֵ��� ��
		// HOLD �����϶� ����� ������Ʈ( �⺻�� m_DamageAirFall )��
		int					m_iHoldStateID;	
//}} kimhc // 2010.7.12 // �������� ������ HOLD �� �� �ֵ��� ��
		
#ifdef PET_AURA_SKILL
		bool		m_bApplyAura;	// �� ��ų ���뿩��
#endif

//{{ ����� 2010.10.29
#ifdef NEW_SKILL_2010_11
	#ifndef FIX_LIMITED_MANA_MANAGEMENT
		bool		m_bLimitManaManagement;//Ȯ�������� �нú� ��ų�� �ߵ��ߴ���.
	#endif //FIX_LIMITED_MANA_MANAGEMENT
#endif 
//}} ����� 2010.10.29
		
		bool	m_bDownForce;	/// ���� ���� ���¿��� Ű���忡�� �Ʒ��� ���� ȭ��ǥ�� ���������� ���� ���θ� ���� (�� ĳ���Ϳ��� �ִ� ���� �Űܿ�)

		//{{ kimhc // 2010.12.9 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		vector<CX2EqipPtr>	m_vecCloneEquipList;		// �ٸ� ���� ���� �켱���� ������ �ϴ� ���(Ŭ��)�� ����Ʈ		
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.9 //  2010-12-23 New Character CHUNG


#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		bool	m_bIsTransformed;
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
		EventProcessTime		m_fEventTime[10];
		float					m_fAttackCancelAfterEx; // OneZ, OneX
		float					m_fAttackCancelAfter;	// PureZ, PuerX
		float					m_fSkillCancelAfter;
		float					m_fBWalkCancelAfter;
		float					m_fWalkCancelAfter;
		float					m_fDashCancelAfter;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		float					m_fSkillCancelAfterFly;		/// ���߿� ��ų ĵ��
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef HYPER_MODE_FIX
		int						m_iLastStateHyperModeCount;
#endif HYPER_MODE_FIX
#ifdef SERV_CHUNG_TACTICAL_TROOPER
		int						m_iHyperModeCountUsed;
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef SPECIAL_USE_ITEM
		CX2EffectSet::Handle m_hSpecialItemBuff_Wind;
		bool			m_bThrowSuccess;
		int				m_iThrowItemValue;
		float			m_fThrowStartTime;
		D3DXVECTOR3		m_vThrowStartPosOffset;
		D3DXVECTOR3		m_vThrowStartVelocity;
#endif

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	float		m_fCannonBallDamageUp;
	float		m_fForceDownRelDamageRateBase;
#endif

	float m_fSummonNpcCoolTime;
	vector<SummonNpcSocket> m_vecSummonNpcSocket;

#ifdef SERV_ELSWORD_INFINITY_SWORD
	float	m_bLearnLightningStep;
#endif

#ifdef SERV_ARME_DIMENSION_WITCH
	int		m_iAdvancedTeleportationLv;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	GameCameraOffset m_GameCameraOffset;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle   m_hCEffectTacticalField;	/// ��Ƽ�� �ʵ� ������ ����Ʈ
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect* m_pCEffectTacticalField;	/// ��Ƽ�� �ʵ� ������ ����Ʈ
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif SERV_CHUNG_TACTICAL_TROOPER


#ifdef SERV_TRAPPING_RANGER_TEST
	typedef	srutil::delegate0<void>	DelegateVitalPointPiercing;
	DelegateVitalPointPiercing		m_delegateVitalPointPiercing;
#endif SERV_TRAPPING_RANGER_TEST

	CX2ComboAndSkillChainSystem	m_ComboAndSkillChainSystem;

#ifdef NEW_CHARACTER_EL
	float m_fAddAccuaracyPercent;	/// ���߷� ��� �нú�, ���߷� �ۼ�Ʈ ���� �� ( �⺻�� : 0 / �ִ밪 1)
	float m_fAddMagicAttackCriticalRate;  /// ���߷� ��� �нú�, ���� ���� ũ��Ƽ�� �ߵ� Ȯ�� ���� ��( �⺻�� : 0 / �ִ밪 1 )
	float m_fAddMagicAttackCriticalDamageRate;  /// ���߷� ��� �нú�, ���� ���� ũ��Ƽ�� ������ ���� ���� ���� ��( �⺻�� : 0 / �ִ밪 1 )
#endif //NEW_CHARACTER_EL

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	bool		m_bSetCustomRenderParam;			/// ������ m_RenderParam�� ���� �״�� ������ �� �ְ��ϱ� ���� ����.
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

private:

	//{{ kimhc // 2010.11.3 // ��ų�߰� ���
#ifdef	NEW_SKILL_2010_11
		// Ȥ�� �ٸ� ĳ���Ϳ��� �簢���� ���� ��츦 �����ؼ� �߰���
		bool		m_bCanChangeHyperModeInHyperModeState;	/// �������¿��� ������ ����� �� �ִ°�? (���� - ��ȭ�� �г�)
#endif	NEW_SKILL_2010_11
	//}} kimhc // 2010.11.3 // ��ų�߰� ���

		//{{ JHKang / ������ / 2010/12/16 / ���� �ǰ� �����ΰ�?
#ifdef SEASON3_MONSTER_2010_12
		bool	m_bNowHit;				/// ���ظ� �޾Ҵ���?
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / ������ / 2010/12/16 / ���� �ǰ� �����ΰ�?

		//{{ JHKang / ������ / 2011/01/17 / ���� ������ ���� ������ ��� ����
#ifdef SEASON3_MONSTER_2010_12
		float	m_fRealDamage;			/// ���� User�� ���� ������ ��ġ
		float   m_fAnimSpeedRateByNpc;	/// User Animation �ӵ�
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / ������ / 2011/01/17 / ���� ������ ���� ������ ��� ����

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahCommonMajorParticleInstance[COMMON_MAJOR_PII_END];
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahCommonMinorParticleInstance[COMMON_MINOR_PII_END];

		float	m_fOxygenGage;

		//{{ JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
#ifdef DUNGEON_RANK_NEW
		KProtectedType<int> m_iTotalAttackedDamage;		/// ���� ������� ���� �� ������ ������
		KProtectedType<int> m_iTotalRecoveryHP;			/// HP ȸ����( �Һ� ������(����, ����) / ����, �ϸ��Ǿ�, �� ��Ÿ ȸ���� ���� )
		KProtectedType<int> m_iUsingSkillCount;			/// ��� ��ų ī��Ʈ(8���� ��ų �� 2ȸ �̻� ���� ī��Ʈ)
		KProtectedType<int> m_iTotalGivenDamamge;		/// ������ ���Ϳ��� �� ���ط�
		int m_iSkillSlotACount[4], m_iSkillSlotBCount[4];
#endif DUNGEON_RANK_NEW
		//}} JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
#ifdef DUNGEON_DASH_LOG
		int m_iDashCount;			/// ������ �������� �뽬 ��� �� Ƚ�� ī��Ʈ
#endif DUNGEON_DASH_LOG

		KProtectedType<float>	m_fCurrentFinalDamage;		/// ���� ������ ��뿡�� �� �������� ������


#ifdef TEST_GROUP_GRAP
		bool						m_bGrap;				/// ���� ���� �ִ� �����ΰ�?		
		int							m_iGrapShakeCount;		/// Ű���带 �¿�� �������� �ϴ� Ƚ�� (�� Ƚ�� ��ŭ ���� ��⿡�� Ǯ��)
		int							m_iGrapStateID;			/// �������� �� ������ ����� ������Ʈ (���� ���ȿ��� �� ������Ʈ�� ���� ��)
		D3DXVECTOR3					m_vGrap;				/// ������ ���¿����� ���� ��ġ
		D3DXVECTOR3					m_vBeforeGrapPos;		/// �������� ������ ���� ��ġ
		
		bool						m_bGrapEscapeKeyLeftCheck;	/// ������ Left ����Ű�� ���������� ���� ������ ��� ����
		bool						m_bGrapEscapeKeyRightCheck;	/// ������ Right ����Ű�� ���������� ���� ������ ��� ����

#endif TEST_GROUP_GRAP


#ifdef NEW_GRAPPLE_ATTACK
		
		int							m_iGrappledStateID;
		int							m_iShaking;
		bool						m_bGrappled;			

#endif NEW_GRAPPLE_ATTACK
#ifdef WORLD_TRIGGER
		float						m_fTriggerTime;
#endif

#ifdef CHUNG_FIRST_CLASS_CHANGE
		float						m_fAddAnimSpeedFactor;			/// û 1�� ����, ���� ������� ������ ��� ��ų�� ���� �ִϸ��̼� �ӵ� ����
#endif

		//}} Protected Variable End
		bool m_bMixedEmotion[AVATAR_EMOTION_NUM];
		CKTDXDeviceSound*				m_pAvatarEmotionSound;

#ifdef NEW_HENIR_TEST
		CX2EffectSet::Handle m_hHenirBuff_Nature;
		CX2EffectSet::Handle m_hHenirBuff_Wind;
#endif NEW_HENIR_TEST

#ifdef FACE_OFF_MONSTER_HEAD_TEST
		bool			m_bHideNoRenderable;
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef SERV_ADD_TITLE_CONDITION
		unsigned char m_ucDieReason;
#endif

#ifdef UPGRADE_RAVEN
		bool m_bParryingState;
#endif

#ifdef ARA_CHANGE_CLASS_FIRST
		bool	m_bQuickenGuardBlocking;		/// 1-2�� �����̴� ���� �нú� ����
		bool	m_bQuickenGuardTurning;			/// 1-2�� �����̴� ���� �нú� �ڵ��Ƹ±�
#endif

#ifdef HYPER_MODE_FIX
		bool m_bReserveHyperModeZero;
#endif HYPER_MODE_FIX

#ifdef EVE_ELECTRA
		bool m_bWhiteOut;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWhiteOut;
#endif EVE_ELECTRA
		
#ifdef BUFF_TEMPLET_SYSTEM
		//10�ʴ� ��ü ü���� % ȸ�� ���� - ȸ�� ī��Ʈ �ð�
		float m_fSocketHPIncreasePerSecondTime;
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_ELSWORD_INFINITY_SWORD
		UCHAR			m_OldStateId;	// ���� ������Ʈ�� ������ (������ ��������� ���� ������?)
#endif


		CKTDXCheckElapsedTime m_ElapsedTimeNotBusyState;	// ���� �ٻ� �������� �ƴ��� üũ �뵵

		DelegatePhysicProcessPortalByGameType		m_delegatePhysicProcessPortalByGameType;


#ifdef SUMMON_MONSTER_CARD_SYSTEM
		SummonMonsterCardData m_SummonMonsterCardData;		/// ��ȯ ���� ī�� ���� ���� ��ü
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		bool			m_bUseChangeWeapon;					/// ���̻��� ������ ����ũ ���̳� û�� 3������ ���� ���� ���� ����
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hEnterCashShop;

		bool m_bHasHyperModeItem;

		bool m_bEnterCashShop;								/// ĳ�û� ���� ����

#ifdef ARA_CHANGE_CLASS_FIRST
		/// ������ // 2013-01-10 // ȸ�� ���� ���� ( ����� �Ҽ��� ��ȭ�������θ� ȸ�ǰ� �����մϴ�. )
		bool m_bCanAvoidance;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef NOT_CANCEL_BBT_STUN_BY_HIT
		bool m_bChangeGroggyState;		/// ������ ���� ���� �׷α� ������Ʈ ��ȯ ����
#endif NOT_CANCEL_BBT_STUN_BY_HIT

#ifdef EFFECT_TOOL
		CKTDGXSkinAnim*				m_pXSkinAnim_EffectTool;
#endif //EFFECT_TOOL
#ifdef RIDING_SYSTEM
		bool			m_bRidingOn;	/// ž��?
		D3DXVECTOR3		m_vSaddlePos;	/// ���� ��ġ
		D3DXVECTOR3		m_vDirDegree;	/// ���� ȸ�� ��

		wstring		m_wstrRidingMotionName;					/// ���̵� ���
		CKTDXCheckElapsedTime m_ElapsedTimeRidingWaitHabit;		/// ���� ���� ��� �ð� ����
		UINT		m_iRidingBoost;		/// Ż ���� Boost ������ ���� ��� ����
		float		m_fAirDashTime;		/// ���� �޸���
		UINT		m_iAirDashCount;	/// ���� �޸��� Ƚ�� ����
		UINT		m_iDoubleJumpCount;	/// �̴� ���� Ƚ��
#endif //RIDING_SYSTEM

#ifdef CHECK_CAMERA_INFO
		bool			m_bIsLineMapCamera;
#endif //CHECK_CAMERA_INFO


#ifdef  ADDED_RELATIONSHIP_SYSTEM
		UidType		m_RelationshipTargetUID;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef	CHUNG_FIRST_CLASS_CHANGE
		// ������ // X2Unit�� �ִ� ���� �״�� �ܾ��.
		// X2Unit���� ó�� �ϸ� �ڱ� �ڽ� �� �����ϱ� ������, 
		// �ٸ� �������Ե� ���� ���� �� �� �ֵ��� Guuser�� �ű�
		// X2Unit�� ó���ϰ� �ִ� ���� �����ϴ� ������ UI ����!!
		CX2Stat::EnchantStat		m_EnchantStatFromPassiveSkill;	// �нú� ��ų � ���� �����Ǵ� EnchantStat
#endif	CHUNG_FIRST_CLASS_CHANGE

#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		// ���� ������Ʈ ��ų ���� 
		// ������ �ð��� �ڱ� �ڽſ��� �����ϴ� ������ ���� ���
		UINT m_uiNowStateSkillLevel;
#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE

	// ������ // 2013-07-02
	// ��ų���� ������ ��ũ��Ʈ���� �� �� �ֵ��� �߰�
	float m_fShowSkillCutInTime;
	bool m_bChangeWorldColor;

#ifdef CHEAT_SELF_DAMAGE // ����ȯ
	int m_iFrontDamage;		/// �ڽſ��� �������� �ִ� ġƮ�� �ǰ� ���� ����
#endif //CHEAT_SELF_DAMAGE

#ifdef ADD_MEMO_1ST_CLASS //��â��
	bool m_bApplyMemo;		/// �ɸ��� ��ũ��Ʈ�� ���ǵ� �޸� ID�� ���� �����Ǿ� �ִ��� üũ.
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //��â��
	AllDamageRelateSkillData	m_AllDamageRelateSkillData;
#endif //ADD_RENA_SYSTEM

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	float						m_fGetFinalDamageValue;
#endif //SERV_ADD_LUNATIC_PSYKER

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef ADD_EVE_SYSTEM_2014		// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�
	bool	m_bManeuverParryingState;
#endif // ADD_EVE_SYSTEM_2014	// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�

private:
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    void _EncodeUserSyncPos( USHORT& usPosX, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//	void _EncodeUserSyncPos( USHORT& usPosX, USHORT& usPosY, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

private:
	DWORD m_adwInitFrameMoveCount[2];

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
private:
	DWORD m_dwLastRecvFrameMoveCount;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef MODIFY_AFTER_IMAGE // ������ // 2013-07-19	
	CKTDXDeviceXSkinMesh* m_pHairXSkinMesh; // ���� ���� �� ������ �̹����� ���� �������� Hair�� �߰� �ϱ� ���� ����
#endif //MODIFY_AFTER_IMAGE

#ifdef CHECK_SOUND_LOADING_TIME
	DWORD dwStartTime;
	DWORD dwTickCount;
#endif // CHECK_SOUND_LOADING_TIME

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	bool m_bRightAtThisState;			/// �ش� ������Ʈ������ ������ ����, ������Ʈ�� ������ ���� �������� ���ƺ�
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_BATTLE_FIELD_DYNAMIC_CAMERA
	float fPreWorldNear;
#endif // FIX_BATTLE_FIELD_DYNAMIC_CAMERA

//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    void    ResetLocalAndRelativePosAndTimers();
    void    IncrementLocalRelativePosTimers( float fElapsedTime );
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX
    int     GetRelativePos( int iUnitIndex ) const          { return  ( GetSyncData( true ).m_dwRelativePos >> ( iUnitIndex * 2 ) ) & 0x3; }
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX
//    DWORD   GetRelativePosFlags() const                     { return m_FrameDataNow.syncData.m_dwRelativePos; }
//    int     GetRelativePos( int iUnitIndex ) const          { return  ( m_FrameDataNow.syncData.m_dwRelativePos >> ( iUnitIndex * 2 ) ) & 0x3; }
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX
    int     GetLocalRelativePos( int iUnitIndex ) const 
    { 
        return ( iUnitIndex >= 0 && iUnitIndex < MAX_GAME_USER_UNIT_NUM ) ? m_acLocalRelativePos[ iUnitIndex ] : 0;
    }
    void    PhysicProcess_RetrieveRelativePosInfo( CX2GUUser* pGameUnit, int iUnitIndex, OUT int& iDirection );
    void    PhysicProcess_UpdateRelativePos();
    char    m_acLocalRelativePos[MAX_GAME_USER_UNIT_NUM];
    float   m_afLocalRelativePosTimers[MAX_GAME_USER_UNIT_NUM];
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

#ifdef FIX_SKILL_SLOT_CHANGE_BUG
	float	m_fSlotChangeLeftCoolTime;	// ���� ���� ��Ÿ�� ���� �ð�
#endif // FIX_SKILL_SLOT_CHANGE_BUG


#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		bool											m_bDoubleAttack;		/// ������� ��������
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

#ifdef ADD_PLAY_SOUND //��â��
		void PlaySoundRevive();	//��Ȱ������ ���� ���.
#endif //ADD_PLAY_SOUND


#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
	bool	m_bIsPlayAvatarEmotionSoundWithoutEmotion;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int	  m_iAllyEventMonsterUID;		//��ȯ�� ���� UID
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
};

namespace _CONST_GUUSER_
{
	const int aNormalSkillStateID[EQUIPPED_SKILL_SLOT_COUNT]		= { CX2GUUser::USI_SPECIAL_ATTACK_1, CX2GUUser::USI_SPECIAL_ATTACK_2, CX2GUUser::USI_SPECIAL_ATTACK_3, CX2GUUser::USI_SPECIAL_ATTACK_4, };
	const int aHyperSkillStateID[EQUIPPED_SKILL_SLOT_COUNT]			= { CX2GUUser::USI_SPECIAL_ATTACK_HYPER_1, CX2GUUser::USI_SPECIAL_ATTACK_HYPER_2, CX2GUUser::USI_SPECIAL_ATTACK_HYPER_3, CX2GUUser::USI_SPECIAL_ATTACK_HYPER_4, };
	const int aNormalSkillStateIDSlotB[EQUIPPED_SKILL_SLOT_COUNT]	= { CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_1, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_2, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_3, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_4, };
	const int aHyperSkillStateIDSlotB[EQUIPPED_SKILL_SLOT_COUNT]	= { CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_1, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_2, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_3, CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_4, };
}

struct  CX2UnitManager_UnitTypeTemplet
{
	std::wstring         m_wstrLuaScriptFile;
	CX2GUUser::InitData m_init;

	CX2UnitManager_UnitTypeTemplet()
		: m_wstrLuaScriptFile()
		, m_init()
	{
	}//CX2UnitManager_UnitTypeTemplet()
};//struct  CX2UnitManager_UnitTypeTemplet


typedef KObserverPtr<CX2GUUser> CX2GUUseroPtr;

IMPLEMENT_INTRUSIVE_PTR( CX2GUUser::CSkillDataBase );