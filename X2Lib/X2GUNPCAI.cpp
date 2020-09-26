#include "StdAfx.h"
#include ".\x2gunpcai.h"

CX2GUNPCAI::CX2GUNPCAI( CX2GUNPC* pMasterNPC ) :
CX2NPCAI( pMasterNPC, CX2NPCAI::NAT_NORMAL )
, m_bNowPatroling( false )
{
	m_PrevAIMessage = CX2GUNPC::SCT_NO_CHANGE;

	m_iFinalDestLineIndex = -1;

	m_bLostTarget = false;
	SetNowPatroling( false );

	m_bFleeing = false;

	m_bEnableLuaTargetingFunc	= false;
	m_strLuaTargetingFunc		= "";

	m_fScanCloakingNearRange	= 0.f;
	m_fScanCloaking				= 0.f;
	m_bScanCloaking				= false;
	//{{ oasis : ����� // 2009-10-7 ////////////////
	m_pMainAIData = new AIData();	//FieldFix: new�� ������ �ʿ� ����
	m_pSubAIData = new AIData();
	SetAItoMainAIData(); // m_pAIData = m_pMainAIData;
	//}} oasis : ����� // 2009-10-7 ////////////////
#ifdef RIDING_MONSTER
	m_bRideToUnit = false;
#endif RIDING_MONSTER

#ifdef SEASON3_MONSTER_2010_12
	m_bTargetedOnlyOne = false;
#endif
	
	m_bFootOnLine	= false;
	m_bIsRight		= false;
	m_fMaxHP		= 0.f;
	m_fMaxJumpRight	= 0.f;
	m_fMaxJumpUp	= 0.f;
	m_fNowHP		= 0.f;
	m_iLastTouchLineIndex	= -1;
	m_iPivotPointLineIndex	= -1;
}

/*virtual*/ CX2GUNPCAI::~CX2GUNPCAI(void)
{
	//{{ oasis : ����� // 2009-10-7 ////////////////
	SAFE_DELETE(m_pMainAIData);		//FieldFix: new�� ������ �ʿ� ������
	SAFE_DELETE(m_pSubAIData);
	//}} oasis : ����� // 2009-10-7 ////////////////
}

void CX2GUNPCAI::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( true == m_bNoBrain )
		return;

	if( false == m_bEnabled )
		return;
	
	CX2GUNPC::STATE_CHANGE_TYPE message = CX2GUNPC::SCT_NO_CHANGE; // FieldFix: ������
	SetMasterUnitData();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();	//FieldFix: �̰͵� ������ �־ �ɵ�


	// ��� Function�� ���� Ÿ������ �ϴ� ���̸�
	if( true == m_bEnableLuaTargetingFunc && 
		false == m_strLuaTargetingFunc.empty() )
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;
		if( m_fElapsedTimeAfterLastTargeting > m_pAIData->targetData.targetInterval )
		{
			m_fElapsedTimeAfterLastTargeting = 0.f;
			//string func = "";
			//ConvertWCHARToChar( func, m_wstrLuaTargetingFunc.c_str() );
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			lua_tinker::call<void>( m_pMasterNPC->GetFunctionLuaState(),  m_strLuaTargetingFunc.c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
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
	// �׷�AI�� ���� ������
	//{{oasis:�����////2009-10-7////
	else if(m_pMasterNPC->GetGroupAICommand_LUA() != CX2GUNPC::GAI_CMD_OFF ) // GROUP AI enable
	{		
		switch ( m_pMasterNPC->GetGroupAICommand_LUA() )
		{
		case CX2GUNPC::GAI_CMD_HOLD:
			{
				CX2GameUnit* pGroupAiTargetGameUnit = g_pX2Game->GetNearestUserUnitInSpecificRange( m_pMasterNPC->GetPos(), 0.0f );
				m_pMasterNPC->SetTargetUnit( pGroupAiTargetGameUnit );
			} break;

		default:
			{
				CX2GameUnit* pGroupAiTargetGameUnit = m_pMasterNPC->GetGroupAITargetGameUnit();
				m_pMasterNPC->SetTargetUnit( pGroupAiTargetGameUnit );
			} break;
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	//}}oasis:�����////2009-10-7////
	else	// �׷� AI�� ��� �ϴ� ���� �ƴϸ�
	{
		// ���� Ÿ���� NPC �̸�
		if( true == m_pAIData->targetData.targetNPC )
		{
			TargetingNPC( fElapsedTime );
		}
		else
		{
			Targeting( fElapsedTime );
		}
	}

	Moving( fElapsedTime );
}

void CX2GUNPCAI::LoadAIDataFromLUA( KLuaManager& luaManager )
{

	//FieldFix: BegingTable ������ ���� ���� ������...
	CX2NPCAI::LoadAIDataFromLUA( luaManager );
	if( true == m_bNoBrain )
		return;


	if( luaManager.BeginTable( "INIT_AI" ) == true )
	{

		if( luaManager.BeginTable( "TARGET" ) == true )
		{
			LUA_GET_VALUE_UTF8( luaManager, "MANUAL_TARGETING_FUNC",	m_pMainAIData->targetData.strLuaTargetingFunc,		"" );

			if( false == m_pMainAIData->targetData.strLuaTargetingFunc.empty() )
			{
				SetLuaTargetingFunc( m_pMainAIData->targetData.strLuaTargetingFunc );
				SetEnableLuaTargetingFunc( true );
			}			

			LUA_GET_VALUE( luaManager, "TARGET_NPC",				m_pMainAIData->targetData.targetNPC,					false );
			LUA_GET_VALUE_ENUM( luaManager, "TARGET_PRIORITY",		m_pMainAIData->targetData.targetPriority,				TARGET_PRIORITY, TP_RANDOM );
			LUA_GET_VALUE( luaManager, "TARGET_INTERVAL",			m_pMainAIData->targetData.targetInterval,				0 );
			LUA_GET_VALUE( luaManager, "TARGET_HEIGHT_LIMIT",		m_pMainAIData->targetData.targetHeightLimit,			100 );
			LUA_GET_VALUE( luaManager, "TARGET_NEAR_RANGE",			m_pMainAIData->targetData.targetNearRange,			0 );
			m_pMainAIData->targetData.targetNearRange = min( m_pMainAIData->targetData.targetNearRange, 8000);

			LUA_GET_VALUE( luaManager, "TARGET_RANGE",				m_pMainAIData->targetData.targetRange,				0 );
			m_pMainAIData->targetData.targetRange	 = min( m_pMainAIData->targetData.targetRange, 9000);

			LUA_GET_VALUE( luaManager, "TARGET_LOST_RANGE",			m_pMainAIData->targetData.targetLostRange,			0 );
			m_pMainAIData->targetData.targetLostRange = min( m_pMainAIData->targetData.targetLostRange, 10000);

			LUA_GET_VALUE( luaManager, "TARGET_SUCCESS_RATE",		m_pMainAIData->targetData.targetSuccessRate,			0.0f );
			LUA_GET_VALUE( luaManager, "ATTACK_TARGET_RATE",		m_pMainAIData->targetData.attackTargetRate,			0.0f );
			LUA_GET_VALUE( luaManager, "PRESERVE_LAST_TARGET_RATE", m_pMainAIData->targetData.preserveLastTargetRate,		0.0f );			

			luaManager.EndTable(); // TARGET
		}

		if( luaManager.BeginTable( "CHASE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pMainAIData->chaseMoveData.moveSplitRange,			0 );
			LUA_GET_VALUE( luaManager, "DEST_GAP",			m_pMainAIData->chaseMoveData.destGap,					0 );
			LUA_GET_VALUE( luaManager, "MOVE_GAP",			m_pMainAIData->chaseMoveData.moveGap,					0 );

			LUA_GET_VALUE( luaManager, "DIR_CHANGE_INTERVAL", m_pMainAIData->chaseMoveData.dirChangeInterval,		0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pMainAIData->chaseMoveData.walkInterval,			0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pMainAIData->chaseMoveData.nearWalkRate,			0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pMainAIData->chaseMoveData.farWalkRate,				0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pMainAIData->chaseMoveData.jumpInterval,			0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pMainAIData->chaseMoveData.upJumpRate,				0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pMainAIData->chaseMoveData.upDownRate,				0 );
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pMainAIData->chaseMoveData.downJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pMainAIData->chaseMoveData.downDownRate,			0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pMainAIData->chaseMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pMainAIData->chaseMoveData.bOnlyThisLineGroup,	false );


			luaManager.EndTable(); // CHASE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pMainAIData->escapeMoveData.moveSplitRange,		0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_GAP",		m_pMainAIData->escapeMoveData.escapeGap,			0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pMainAIData->escapeMoveData.walkInterval,		0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pMainAIData->escapeMoveData.nearWalkRate,		0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pMainAIData->escapeMoveData.farWalkRate,		0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pMainAIData->escapeMoveData.jumpInterval,		0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pMainAIData->escapeMoveData.upJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pMainAIData->escapeMoveData.upDownRate,			0 );			
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pMainAIData->escapeMoveData.downJumpRate,		0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pMainAIData->escapeMoveData.downDownRate,		0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pMainAIData->escapeMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			luaManager.EndTable(); // ESCAPE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_CONDITION" ) == true )
		{
			LUA_GET_VALUE( luaManager, "RATE",			m_pMainAIData->escapeCondition.escapeRate,		0 );
			LUA_GET_VALUE( luaManager, "MY_HP",			m_pMainAIData->escapeCondition.myHP,				0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_RANGE",	m_pMainAIData->escapeCondition.escapeRange,		0.f );
			// 09.04.16 �¿� : Escape Condition�� Flage Check �߰�
			LUA_GET_VALUE( luaManager, "FLAG_TRUE",		m_pMainAIData->escapeCondition.m_iFlagTrue,		-1 );

			if( m_pMainAIData->escapeCondition.escapeRate  != 0 ||
				m_pMainAIData->escapeCondition.myHP		 != 0 ||
				m_pMainAIData->escapeCondition.escapeRange != 0.f )
			{
				if( m_pMainAIData->escapeCondition.escapeRate == 0 )
					m_pMainAIData->escapeCondition.escapeRate = 100;

				if( m_pMainAIData->escapeCondition.myHP		== 0 )
					m_pMainAIData->escapeCondition.myHP		= 100;

				if( m_pMainAIData->escapeCondition.escapeRange == 0.f )
					m_pMainAIData->escapeCondition.escapeRange = 10000.f;
			}

			luaManager.EndTable(); // ESCAPE_CONDITION
		}

		if( luaManager.BeginTable( "PATROL_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "PATROL_BEGIN_RATE",		m_pMainAIData->patrolMoveData.patrolBeginRate,		0 );
			LUA_GET_VALUE( luaManager, "PATROL_RANGE",			m_pMainAIData->patrolMoveData.patrolRange,			0 );
			LUA_GET_VALUE( luaManager, "PATROL_COOL_TIME",		m_pMainAIData->patrolMoveData.patrolCoolingTime,		0 );
			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pMainAIData->patrolMoveData.bOnlyThisLineGroup,		false );

			luaManager.EndTable(); // PATROL_MOVE
		}

#ifdef AI_FLY
		if( luaManager.BeginTable( "FLY_CHASE_MOVE" ) == true )
		{
			SetAIFly(true);

			LUA_GET_VALUE( luaManager, "DEST_HEIGHT_GAP",		m_pMainAIData->flymovedata.fDestHeight,			0 );
			LUA_GET_VALUE( luaManager, "DEST_LAND_GAP",			m_pMainAIData->flymovedata.fDestGap,				0 );
			LUA_GET_VALUE( luaManager, "DEST_AREA",				m_pMainAIData->flymovedata.fDestArea,				0 );
			LUA_GET_VALUE( luaManager, "FLY_MOVE_INTERVAL",		m_pMainAIData->flymovedata.fFlyMoveInterval,		0 );			

			LUA_GET_VALUE( luaManager, "UNDERWATER_MODE",		m_pMainAIData->flymovedata.bUnderWaterMode,		false );			
			luaManager.EndTable();
		}
#endif	

		luaManager.EndTable(); // INIT_AI
	}
	//{{ oasis : ����� // 2009-10-7 ////////////////
	if( luaManager.BeginTable( "INIT_SUB_AI" ) == true )
	{

		if( luaManager.BeginTable( "TARGET" ) == true )
		{
			LUA_GET_VALUE_UTF8( luaManager, "MANUAL_TARGETING_FUNC",	m_pSubAIData->targetData.strLuaTargetingFunc,		"" );

			if( false == m_pSubAIData->targetData.strLuaTargetingFunc.empty() )
			{
				SetLuaTargetingFunc( m_pSubAIData->targetData.strLuaTargetingFunc );
				SetEnableLuaTargetingFunc( true );
			}			

			LUA_GET_VALUE( luaManager, "TARGET_NPC",				m_pSubAIData->targetData.targetNPC,					false );
			LUA_GET_VALUE_ENUM( luaManager, "TARGET_PRIORITY",		m_pSubAIData->targetData.targetPriority,				TARGET_PRIORITY, TP_RANDOM );
			LUA_GET_VALUE( luaManager, "TARGET_INTERVAL",			m_pSubAIData->targetData.targetInterval,				0 );
			LUA_GET_VALUE( luaManager, "TARGET_HEIGHT_LIMIT",		m_pMainAIData->targetData.targetHeightLimit,			100 );
			LUA_GET_VALUE( luaManager, "TARGET_NEAR_RANGE",			m_pSubAIData->targetData.targetNearRange,			0 );
			m_pSubAIData->targetData.targetNearRange = min( m_pSubAIData->targetData.targetNearRange, 8000);

			LUA_GET_VALUE( luaManager, "TARGET_RANGE",				m_pSubAIData->targetData.targetRange,				0 );
			m_pSubAIData->targetData.targetRange	 = min( m_pSubAIData->targetData.targetRange, 9000);

			LUA_GET_VALUE( luaManager, "TARGET_LOST_RANGE",			m_pSubAIData->targetData.targetLostRange,			0 );
			m_pSubAIData->targetData.targetLostRange = min( m_pSubAIData->targetData.targetLostRange, 10000);

			LUA_GET_VALUE( luaManager, "TARGET_SUCCESS_RATE",		m_pSubAIData->targetData.targetSuccessRate,			0 );
			LUA_GET_VALUE( luaManager, "ATTACK_TARGET_RATE",		m_pSubAIData->targetData.attackTargetRate,			0 );
			LUA_GET_VALUE( luaManager, "PRESERVE_LAST_TARGET_RATE", m_pSubAIData->targetData.preserveLastTargetRate,		0 );			

			luaManager.EndTable(); // TARGET
		}

		if( luaManager.BeginTable( "CHASE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pSubAIData->chaseMoveData.moveSplitRange,			0 );
			LUA_GET_VALUE( luaManager, "DEST_GAP",			m_pSubAIData->chaseMoveData.destGap,					0 );
			LUA_GET_VALUE( luaManager, "MOVE_GAP",			m_pSubAIData->chaseMoveData.moveGap,					0 );

			LUA_GET_VALUE( luaManager, "DIR_CHANGE_INTERVAL", m_pSubAIData->chaseMoveData.dirChangeInterval,		0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pSubAIData->chaseMoveData.walkInterval,			0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pSubAIData->chaseMoveData.nearWalkRate,			0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pSubAIData->chaseMoveData.farWalkRate,				0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pSubAIData->chaseMoveData.jumpInterval,			0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pSubAIData->chaseMoveData.upJumpRate,				0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pSubAIData->chaseMoveData.upDownRate,				0 );
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pSubAIData->chaseMoveData.downJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pSubAIData->chaseMoveData.downDownRate,			0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pSubAIData->chaseMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pSubAIData->chaseMoveData.bOnlyThisLineGroup,	false );


			luaManager.EndTable(); // CHASE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pSubAIData->escapeMoveData.moveSplitRange,		0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_GAP",		m_pSubAIData->escapeMoveData.escapeGap,			0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pSubAIData->escapeMoveData.walkInterval,		0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pSubAIData->escapeMoveData.nearWalkRate,		0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pSubAIData->escapeMoveData.farWalkRate,		0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pSubAIData->escapeMoveData.jumpInterval,		0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pSubAIData->escapeMoveData.upJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pSubAIData->escapeMoveData.upDownRate,			0 );			
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pSubAIData->escapeMoveData.downJumpRate,		0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pSubAIData->escapeMoveData.downDownRate,		0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pSubAIData->escapeMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			luaManager.EndTable(); // ESCAPE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_CONDITION" ) == true )
		{
			LUA_GET_VALUE( luaManager, "RATE",			m_pSubAIData->escapeCondition.escapeRate,		0 );
			LUA_GET_VALUE( luaManager, "MY_HP",			m_pSubAIData->escapeCondition.myHP,				0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_RANGE",	m_pSubAIData->escapeCondition.escapeRange,		0.f );
			// 09.04.16 �¿� : Escape Condition�� Flage Check �߰�
			LUA_GET_VALUE( luaManager, "FLAG_TRUE",		m_pSubAIData->escapeCondition.m_iFlagTrue,		-1 );

			if( m_pSubAIData->escapeCondition.escapeRate  != 0 ||
				m_pSubAIData->escapeCondition.myHP		 != 0 ||
				m_pSubAIData->escapeCondition.escapeRange != 0.f )
			{
				if( m_pSubAIData->escapeCondition.escapeRate == 0 )
					m_pSubAIData->escapeCondition.escapeRate = 100;

				if( m_pSubAIData->escapeCondition.myHP		== 0 )
					m_pSubAIData->escapeCondition.myHP		= 100;

				if( m_pSubAIData->escapeCondition.escapeRange == 0.f )
					m_pSubAIData->escapeCondition.escapeRange = 10000.f;
			}

			luaManager.EndTable(); // ESCAPE_CONDITION
		}

		if( luaManager.BeginTable( "PATROL_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "PATROL_BEGIN_RATE",		m_pSubAIData->patrolMoveData.patrolBeginRate,		0 );
			LUA_GET_VALUE( luaManager, "PATROL_RANGE",			m_pSubAIData->patrolMoveData.patrolRange,			0 );
			LUA_GET_VALUE( luaManager, "PATROL_COOL_TIME",		m_pSubAIData->patrolMoveData.patrolCoolingTime,		0 );
			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pSubAIData->patrolMoveData.bOnlyThisLineGroup,		false );

			luaManager.EndTable(); // PATROL_MOVE
		}

#ifdef AI_FLY
		if( luaManager.BeginTable( "FLY_CHASE_MOVE" ) == true )
		{
			SetAIFly(true);

			LUA_GET_VALUE( luaManager, "DEST_HEIGHT_GAP",		m_pSubAIData->flymovedata.fDestHeight,			0 );
			LUA_GET_VALUE( luaManager, "DEST_LAND_GAP",			m_pSubAIData->flymovedata.fDestGap,				0 );
			LUA_GET_VALUE( luaManager, "DEST_AREA",				m_pSubAIData->flymovedata.fDestArea,				0 );
			LUA_GET_VALUE( luaManager, "FLY_MOVE_INTERVAL",		m_pSubAIData->flymovedata.fFlyMoveInterval,		0 );			

			LUA_GET_VALUE( luaManager, "UNDERWATER_MODE",		m_pSubAIData->flymovedata.bUnderWaterMode,		false );			

			luaManager.EndTable();
		}
#endif	

		luaManager.EndTable(); // INIT_SUB_AI
	}
	//}} oasis : ����� // 2009-10-7 ////////////////
}

void CX2GUNPCAI::SetMasterUnitData()
{
	KTDXPROFILE();

	const CX2GUNPC::NPCFrameData& frameData = m_pMasterNPC->GetNPCFrameData();
	//const CX2GUNPC::NPCSyncData& syncData = m_pMasterNPC->GetNPCSyncData();

#ifdef RIDING_MONSTER
	if(m_pMasterNPC->GetRideState() == CX2GUNPC::RS_ON_RIDING && 
		m_pMasterNPC->GetRideType() ==  CX2GUNPC::RT_RIDER )
	{
		const CX2GUNPC::NPCFrameData& ridingFrameData = m_pMasterNPC->GetRideUnit()->GetNPCFrameData();
		m_vPosition = ridingFrameData.syncData.position;	//FieldFix: ������ ������ �ش� �κ� �����ص� �ɵ�..
		m_bIsRight = ridingFrameData.syncData.bIsRight;
		m_iLastTouchLineIndex = ridingFrameData.syncData.lastTouchLineIndex;
		m_bFootOnLine = ridingFrameData.unitCondition.bFootOnLine;
	}
	else
	{
		m_vPosition = frameData.syncData.position;
		m_bIsRight = frameData.syncData.bIsRight;
		m_iLastTouchLineIndex = frameData.syncData.lastTouchLineIndex;
		m_bFootOnLine = frameData.unitCondition.bFootOnLine;
	}


#else
	m_vPosition = frameData.syncData.position;
	m_bIsRight = frameData.syncData.bIsRight;
	m_iLastTouchLineIndex = frameData.syncData.lastTouchLineIndex;
	m_bFootOnLine = frameData.unitCondition.bFootOnLine;
#endif RIDING_MONSTER

	m_fNowHP = m_pMasterNPC->GetNowHp();
	m_fMaxHP = m_pMasterNPC->GetMaxHp();

	// fix!! �̸� �ѹ��� ����ؼ� ������ �־��, �ƴϸ� �뷫 ����ϰų�
	// t = v0/g
	// l = v0*t - 1/2*g*t^2 = 1/2*v0^2/g
	const CX2GameUnit::PhysicParam& physicParam = m_pMasterNPC->GetPhysicParam();	

	float fHalfTimeOnAir;
	if( physicParam.fGAccel != 0.f )
		fHalfTimeOnAir = physicParam.GetJumpSpeed() / physicParam.fGAccel;	
	else
		fHalfTimeOnAir = 0.f;

	m_fMaxJumpUp = 0.5f * physicParam.GetJumpSpeed() * fHalfTimeOnAir;
	m_fMaxJumpRight = 2.f * physicParam.GetDashSpeed() * fHalfTimeOnAir - physicParam.fReloadAccel * fHalfTimeOnAir * fHalfTimeOnAir;

	if( 0.f != physicParam.GetDashSpeed() && m_fMaxJumpRight < LINE_RADIUS )
	{
		m_fMaxJumpRight = LINE_RADIUS;
		//ASSERT( !L"RUN_SPEED is too small" );
	}
}

/*virtual*/ void CX2GUNPCAI::Targeting( float fElapsedTime )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;


	//FieldFix: �ε��Ҽ����� == ���� �ǵ��� ������� ����
	// �׸��� ���� Cloaking �κа� �����ؼ��� ���� ��ȿ�����ε�
	if( m_fScanCloaking == 0.f && m_fScanCloakingNearRange == 0.f )
	{
		m_bScanCloaking = false;
	}

	/// Ÿ���� Interval ��ŭ�� �ð��� �������� �ӽ÷� üũ �غ���
	float fTime = m_fElapsedTimeAfterLastTargeting + fElapsedTime;

	// ��������
	if( fTime >= m_pAIData->targetData.targetInterval )
	{
		m_bScanCloaking = false;
		// Ŭ��ŷ�� �� �� �ִ� Ȯ���� Ȯ��
		if( m_fScanCloaking > 0.f && 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            CX2GUNPC::EstimateFrameOneshotPercent( m_fScanCloaking ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            (float)RandomInt() <= m_fScanCloaking 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )
		{
			m_bScanCloaking = true;
		}
	}

	// �ڽ��� ������ ������ ������ Ÿ����
	if( m_optrAttackerGameUnit != null )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if ( m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->targetData.attackTargetRate ) == true )
		{
			SetTargetGameUnit( m_optrAttackerGameUnit.GetObservable() );
        }
		// ���ݴ����� �� ���� Ȯ���� ��������~
		const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;

		if ( NULL != m_pMasterNPC && m_pMasterNPC->GetSiegeMode() )
		{
			ResetAttackerGameUnit();

			if ( null != m_optrTargetGameUnit )
			{
				if( fNowHPRate < m_pAIData->escapeCondition.myHP &&
					m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->escapeCondition.escapeRate ) == true &&
					true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue) )	// 09.04.16 �¿�
				{
					m_bFleeing = true;
				}

				return TargetUpdate( true );
			}
		}	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if ( RandomFloat( 0.f, 100.f ) < m_pAIData->targetData.attackTargetRate )
		{
			SetTargetGameUnit( m_optrAttackerGameUnit.GetObservable() );

			// ���ݴ����� �� ���� Ȯ���� ��������~
			const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;

			if ( NULL != m_pMasterNPC && m_pMasterNPC->GetSiegeMode() )
			{
				ResetAttackerGameUnit();

				if ( null != m_optrTargetGameUnit )
				{
					if( fNowHPRate < m_pAIData->escapeCondition.myHP &&
						RandomFloat( 0.f, 100.f ) < m_pAIData->escapeCondition.escapeRate &&
						true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue) )	// 09.04.16 �¿�
					{
						m_bFleeing = true;
					}

					return TargetUpdate();
				}
			}	
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	// �������� ���ε�
	if( true == m_bFleeing )
	{
		m_bFleeing = false;

		// Ÿ���� ���ų�, Ÿ���� �ִµ� ���� �Ÿ���ŭ �־����ٸ�
		if ( null == m_optrTargetGameUnit )
		{ 
			ResetTarget();
		}
 		else if( GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_vPosition ) > static_cast<float>( m_pAIData->escapeMoveData.escapeGap * m_pAIData->escapeMoveData.escapeGap ) )
 		{
			m_bFleeing = false;
 		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	
	//FieldFix: ���� ����ȭ ó���� �̷��� ������ �Ǿ� �ֳ�...
	// ���� Ÿ���� ����ȭ �Ǿ��ٸ� Ÿ���� �Ҵ´�, �̺��� ����ȭ ��ų�� ���� ó��
	if( null != m_optrTargetGameUnit )
	{
		if ( m_optrTargetGameUnit->GetInvisibility() && false == m_bScanCloaking )
		{
			ResetTarget();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
		
#ifdef EVE_ELECTRA
		if( m_pMasterNPC->GetShowSubBossName() == false && m_pMasterNPC->GetShowBossName() == false && 
			m_pMasterNPC->GetExtraDamagePack().m_FlashBang.m_fTime > 0.0f )
		{
			ResetTarget();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
#endif EVE_ELECTRA

	}

	// Ÿ���� �� ���� �Ǿ����� �˾ƺ���~
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;

		// targetInterval �ð��� ������ �ʾ����� �� �߰��� �ؿ� �κ��� ������ �� ����
		if( m_fElapsedTimeAfterLastTargeting < m_pAIData->targetData.targetInterval )
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
		m_fElapsedTimeAfterLastTargeting = 0.f;	// FieldFix: �̰� ��ȣ ������ �ִ��� �ؾ��� ��

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.targetSuccessRate ) == false )
			return TargetUpdate( false );	// �̺κ��� ���� TargetUpdate�� Success�� �ǹ����� �ʴµ�
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( RandomFloat( 0.f, 100.f ) >= m_pAIData->targetData.targetSuccessRate )
			return TargetUpdate();	// �̺κ��� ���� TargetUpdate�� Success�� �ǹ����� �ʴµ�
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	// ���� Ÿ���� �����ұ��?
	if( false == m_bLostTarget && null != m_optrTargetGameUnit )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if ( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.preserveLastTargetRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if ( RandomFloat( 0.f, 100.f ) < m_pAIData->targetData.preserveLastTargetRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			const float fDistance3Sq = GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_vPosition );
			if( fDistance3Sq < m_pAIData->targetData.targetLostRange * m_pAIData->targetData.targetLostRange 
				&& CX2GameUnit::GUSI_DIE == m_optrTargetGameUnit->GetGameUnitState() )
            {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            }
			else
				ResetTarget();
		}
	}

	// ��~ ���������� ���ο� Ÿ���� ã�� ���ô�~
	
	// Escape ���� ����
	CX2GameUnit*	pFleePickedGameUnit				= NULL;
	float			fFleePickedDistance3Sq			= 0.f;	

	// Targeting ���� ����
	CX2GameUnit*	pTargetPickedGameUnit			= NULL;
	float			fTargetPickedDistance3Sq		= 0.f;
	
	bool			bFoundNearTargetGameUnit		= false;	// ������ Ÿ�ٵǴ� �Ÿ����� ������ ã�Ҵ���

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_vecpTempGameUnit.resize( 0 );
    float                       fRate = 0.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i = 0; i<g_pX2Game->GetUserUnitListSize(); i++ )
	{
		CX2GameUnit* pGameUnit =  g_pX2Game->GetUserUnit(i);
		if( NULL == pGameUnit || CX2GameUnit::GUSI_DIE == pGameUnit->GetGameUnitState() )
			continue;
		
		if( true == pGameUnit->GetInvisibility() && m_bScanCloaking == false)
			continue;

		if( pGameUnit->GetNowHp() <= 0.f )
			continue;

		const float fDistance3Sq = GetDistance3Sq( pGameUnit->GetPos(), m_vPosition );

		// ���� ���� ���°� �ƴ� ���
		if( false == m_bFleeing )
		{
			// ���������� ��츦 ����� 
			// escapeRange �ȿ� �ִ� ���� �� ���� ����� ������ �ϴ� ���� ������� ������ ���´�
			if( fDistance3Sq < m_pAIData->escapeCondition.escapeRange * m_pAIData->escapeCondition.escapeRange )
			{
				if( NULL == pFleePickedGameUnit || fDistance3Sq < fFleePickedDistance3Sq )
				{
					pFleePickedGameUnit		= pGameUnit;
					fFleePickedDistance3Sq	= fDistance3Sq;					
				}
			}
		}

		// targetRange ���� �ָ� Ÿ�� ���� ����
		if( fDistance3Sq > m_pAIData->targetData.targetRange * m_pAIData->targetData.targetRange )
			continue;
		
		// ������ Ÿ���� �Ǵ� �Ÿ��ȿ� �ִ� �����̰�
		if( fDistance3Sq < m_pAIData->targetData.targetNearRange * m_pAIData->targetData.targetNearRange )
		{
			// ������ Ÿ���� ���ų�, ���� Ÿ�� ���� ������
			if ( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
			{
				pTargetPickedGameUnit		= pGameUnit;
				fTargetPickedDistance3Sq	= fDistance3Sq;
				bFoundNearTargetGameUnit	= true;	// NearRange ������ ������ ã�Ҵ�
			}
		}
		

		if ( false == bFoundNearTargetGameUnit )
		{
			switch( m_pAIData->targetData.targetPriority )
			{
			case TP_HIGH_LEVEL_FIRST: 
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetUnitLevel() > pTargetPickedGameUnit->GetUnitLevel() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_LOW_LEVEL_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetUnitLevel() < pTargetPickedGameUnit->GetUnitLevel() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_HIGH_HP_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_LOW_HP_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_NEAR_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
					{
						fTargetPickedDistance3Sq	= fDistance3Sq;
						pTargetPickedGameUnit		= pGameUnit;
					}
				} break;

			case TP_FAR_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
					{
						fTargetPickedDistance3Sq	= fDistance3Sq;
						pTargetPickedGameUnit		= pGameUnit;
					}
				} break;

			default:
			case TP_RANDOM:
				{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    m_vecpTempGameUnit.push_back( pGameUnit );
                    fRate = fRate * 0.5f + 0.5f;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
					if( RandomFloat( 0.f, 100.f ) < 50.f || NULL == pTargetPickedGameUnit )
					{
						pTargetPickedGameUnit = pGameUnit;
					}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				} break;

#ifdef SEASON3_MONSTER_2010_12
			case TP_ONLY_ONE:
				{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    m_vecpTempGameUnit.push_back( pGameUnit );
                    fRate = 1.f;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
					if( NULL == pTargetPickedGameUnit )
						pTargetPickedGameUnit = pGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				} break;
#endif
			}
		}		
	} // end of for(i)
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( pTargetPickedGameUnit == NULL && m_vecpTempGameUnit.empty() == false
        && CX2GUNPC::EstimateFrameOneshotPercent( fRate * 100.f ) == true )
    {
        if ( m_vecpTempGameUnit.size() == 1 )
            pTargetPickedGameUnit = m_vecpTempGameUnit.front();
        else
        {
            int i = rand()% m_vecpTempGameUnit.size();
            pTargetPickedGameUnit = m_vecpTempGameUnit[ i ];
        }
    }
    m_vecpTempGameUnit.resize( 0 );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// escape range �ȿ� �� ������ Ÿ������ �������� ���� �����Ѵ�

	// NPC�� �����尡 �ƴϰ� ���������ϴ� ������ ���� �ϴ� ���
	// iPickedFleeTargetIndex�� �����س��� ����� Ÿ������ �Ͽ� ���� ����
	const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;
	
	if( false == m_pMasterNPC->GetSiegeMode() && 
		NULL != pTargetPickedGameUnit && 
		fNowHPRate <= m_pAIData->escapeCondition.myHP &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->escapeCondition.escapeRate ) == true &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		RandomFloat( 0.f, 100.f ) <= m_pAIData->escapeCondition.escapeRate &&
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue ) )		// 09.04.16 �¿�
	{
		m_optrTargetGameUnit = pFleePickedGameUnit;
	}
	else // �׳� chase Ÿ���� �i�ư���
	{
		m_optrTargetGameUnit = pTargetPickedGameUnit;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    return TargetUpdate( false );	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	return TargetUpdate();	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}


void CX2GUNPCAI::TargetingNPC( float fElapsedTime )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    // �� �Ʒ� �ڵ�� �� �������� �ƴ϶� adaptive �ϰ� ����� �� �ִ�. ���� �ùٸ� ���� Ȯ�� ����� ���ؼ���
    // m_pMasterNPC->EstimateFrameAccumPercent �� ����Ѵ�.
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// ��ó�� ���� ������ ������ �� ������ Ÿ������ flee
	// ���� ������ ������ ��ó�� �ִ� ���ҵ� �߿��� ����, �߰�����, �Ϲ� ������ �ǰ� ���� �ָ� Ÿ������ �̵��Ѵ�
	// Ư���Ÿ����� ��������� ���� ����~

	// �ڽ��� ������ ������ Ÿ������ flee
	if( m_optrAttackerGameUnit != null )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if ( m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->targetData.attackTargetRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( RandomFloat( 0.f, 100.f ) < m_pAIData->targetData.attackTargetRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_bFleeing = true;
			ResetAttackerGameUnit();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
	}
	
	// �������� ���̸� �� �����ƴ��� Ȯ���Ѵ�
	if( true == m_bFleeing && null != m_optrTargetGameUnit )
	{
		if( GetDistance( m_optrTargetGameUnit->GetPos(), m_vPosition ) > m_pAIData->escapeMoveData.escapeGap * m_pAIData->escapeMoveData.escapeGap )
		{
			ResetTarget();
			m_bFleeing		= false;

		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	// Ÿ���� �� ���� �Ǿ����� �˾ƺ���~
	m_fElapsedTimeAfterLastTargeting += fElapsedTime;
	if( m_fElapsedTimeAfterLastTargeting < m_pAIData->targetData.targetInterval )
    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    }
	m_fElapsedTimeAfterLastTargeting = 0.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    // �� �Ʒ� �ڵ�� m_fElapsedTimeAfterLastTargeting �� m_pAIData->targetData.targetInterval �� ���� ������
    // 1���� trigger �Ǿ� ����ȴ�. ���� random ����� ���� CX2GUNPC::EstimateFrameOneshotPercent �� �̿��ϵ��� �Ѵ�
    // <- ������ random ������ ���� 

    if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.targetSuccessRate ) == false )
        return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( RandomFloat( 0.f, 100.f ) >= m_pAIData->targetData.targetSuccessRate )
		return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


	// ���� Ÿ���� �����ұ��?
	if( false == m_bLostTarget && null != m_optrTargetGameUnit )
	{
		if ( CX2GameUnit::GUSI_DIE != m_optrTargetGameUnit->GetGameUnitState() )
		{
			// LostRange �Ÿ� ���̸� Ÿ���ϴ� ���� ���ش�
			const float fDistance3Sq = GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_vPosition );
			if( fDistance3Sq <= m_pAIData->targetData.targetLostRange * m_pAIData->targetData.targetLostRange )
            {
			// �Ÿ� ���� ������ ���� Ÿ���� Ȯ���� ���� ���� �����ϸ�
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.preserveLastTargetRate ) == true )
                    return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( RandomFloat( 0.f, 100.f ) <= m_pAIData->targetData.preserveLastTargetRate )
                    return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

            }
		}
		ResetTarget();
	}

	// Escape ���� ����
	CX2GameUnit*	pFleePickedGameUnit				= NULL;
	float			fFleePickedDistance3Sq			= 0.f;	

	// Targeting ���� ����
	CX2GameUnit*	pTargetPickedGameUnit			= NULL;
	float			fTargetPicketDistance3Sq		= 0.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_vecpTempGameUnit.resize( 0 );
    float                       fRate = 0.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i = 0; i < g_pX2Game->GetNPCUnitListSize(); i++ )
	{
		CX2GameUnit* pGameUnit =  g_pX2Game->GetNPCUnit( i );
		if( NULL == pGameUnit || CX2GameUnit::GUSI_DIE == pGameUnit->GetGameUnitState() )
			continue;

		if( true == pGameUnit->GetInvisibility() && m_bScanCloaking == false)
			continue;

		if( pGameUnit->GetNowHp() <= 0.f )
			continue;
		
		const float fDistance3Sq = GetDistance3Sq( pGameUnit->GetPos(), m_vPosition );

		// Ư�� �Ÿ����� ������ ������ Ÿ����
		if ( fDistance3Sq < m_pAIData->targetData.targetNearRange )
		{
			m_optrTargetGameUnit = pGameUnit;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_vecpTempGameUnit.resize( 0 );
            return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}

		// ���� ���� ���°� �ƴ� ���
		if( false == m_bFleeing )
		{
			// ���������� ��츦 ����� 
			// escapeRange �ȿ� �ִ� ���� �� ���� ����� ������ �ϴ� ���� ������� ������ ���´�
			if( fDistance3Sq < m_pAIData->escapeCondition.escapeRange * m_pAIData->escapeCondition.escapeRange )
			{
				if( NULL == pFleePickedGameUnit || fDistance3Sq < fFleePickedDistance3Sq )
				{
					pFleePickedGameUnit		= pGameUnit;
					fFleePickedDistance3Sq	= fDistance3Sq;					
				}
			}
		}

		// targetRange ���� �ָ� Ÿ�� ���� ����
		if( fDistance3Sq > m_pAIData->targetData.targetRange * m_pAIData->targetData.targetRange )
			continue;

		switch( m_pAIData->targetData.targetPriority )
		{
		case TP_HIGH_LEVEL_FIRST: 
			{
				if( NULL == pTargetPickedGameUnit || pGameUnit->GetUnitLevel() > pTargetPickedGameUnit->GetUnitLevel() )
					pTargetPickedGameUnit = pGameUnit;
			} break;

		case TP_LOW_LEVEL_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || pGameUnit->GetUnitLevel() < pTargetPickedGameUnit->GetUnitLevel() )
					pTargetPickedGameUnit = pGameUnit;
			} break;

		case TP_HIGH_HP_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
					pTargetPickedGameUnit = pGameUnit;
			} break;

		case TP_LOW_HP_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
					pTargetPickedGameUnit = pGameUnit;
			} break;

		case TP_NEAR_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPicketDistance3Sq )
				{
					fTargetPicketDistance3Sq	= fDistance3Sq;
					pTargetPickedGameUnit		= pGameUnit;
				}
			} break;

		case TP_FAR_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPicketDistance3Sq )
				{
					fTargetPicketDistance3Sq	= fDistance3Sq;
					pTargetPickedGameUnit		= pGameUnit;
				}
			} break;

		case TP_RANDOM:
			{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                m_vecpTempGameUnit.push_back( pGameUnit );
                fRate = fRate * 0.5f + 0.5f;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < 50.f || NULL == pTargetPickedGameUnit )
				{
					pTargetPickedGameUnit = pGameUnit;
				}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			} break;

#ifdef SEASON3_MONSTER_2010_12
		case TP_ONLY_ONE:
			{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                m_vecpTempGameUnit.push_back( pGameUnit );
                fRate = 1.f;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( NULL == pTargetPickedGameUnit )
					pTargetPickedGameUnit = pGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			} break;
#endif
		default:
			ASSERT( !L"Wrong Path!" );
			break;
		}
	} // end of for(i)
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( pTargetPickedGameUnit == NULL && m_vecpTempGameUnit.empty() == false
        && CX2GUNPC::EstimateFrameOneshotPercent( fRate * 100.f ) == true )
    {
        if ( m_vecpTempGameUnit.size() == 1 )
            pTargetPickedGameUnit = m_vecpTempGameUnit.front();
        else
        {
            int i = rand()% m_vecpTempGameUnit.size();
            pTargetPickedGameUnit = m_vecpTempGameUnit[ i ];
        }
    }
    m_vecpTempGameUnit.resize( 0 );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


	// NPC�� �����尡 �ƴϰ� ���������ϴ� ������ ���� �ϴ� ���
	// iPickedFleeTargetIndex�� �����س��� ����� Ÿ������ �Ͽ� ���� ����
	const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;

	if( false == m_pMasterNPC->GetSiegeMode() && 
		NULL != pTargetPickedGameUnit && 
		fNowHPRate <= m_pAIData->escapeCondition.myHP &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->escapeCondition.escapeRate ) == true &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		RandomFloat( 0.f, 100.f ) <= m_pAIData->escapeCondition.escapeRate &&
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue ) )		// 09.04.16 �¿�
	{
		m_optrTargetGameUnit = pFleePickedGameUnit;
	}
	else // �׳� chase Ÿ���� �i�ư���
	{
		m_optrTargetGameUnit = pTargetPickedGameUnit;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    return TargetUpdate( false );	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	return TargetUpdate();	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GUNPCAI::TargetUpdate( bool bAccumulate_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GUNPCAI::TargetUpdate()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	KTDXPROFILE();	

#ifdef RIDING_MONSTER
	// �ٸ� NPC�� Ÿ���ϴ� ���Ͷ��
	// FieldFix: �̷����� ó������ �׳� ������.. �� Targeting�� ���� �߳�...
	if(m_bRideToUnit)
	{
		// Ÿ���� ����
		ResetTarget();
	}

#endif RIDING_MONSTER

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;

	//{{oasis:�����////2009-10-7////
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_FORCED_MOVE &&
		m_pMasterNPC->IsGroupAIGroundTarget() ==  true ) // ���� Ÿ��
	{
		m_vFinalDest = m_pMasterNPC->GetGroupAITargetPos();
		pLineMap->GetLandPosition(m_vFinalDest, 30.0f, &m_iFinalDestLineIndex);
		return;
	}
	//}}oasis:�����////2009-10-7////

	// Ÿ���� ���ٸ�
	if( null == m_optrTargetGameUnit )
	{
		m_bFleeing = false; // target�� ������ �������� �͵� ����� �Ѵ�

		// Ư�� �������� �����Ѵٸ�
		if( null != m_optrRallyGameUnit )
		{
			D3DXVECTOR3 vRallyUnitPos( m_optrRallyGameUnit->GetPos() );
			int iRallyLineIndex = m_optrRallyGameUnit->GetLastTouchLineIndex();

			// �Ÿ����� ���
			float fDistance3Sq = GetDistance3Sq( vRallyUnitPos, m_vPosition );

			// �Ÿ� ���̰� CHASE����ü�� ���� DEST_GAP ���� �϶�
			if( fDistance3Sq < m_pAIData->chaseMoveData.destGap * m_pAIData->chaseMoveData.destGap )
			{
#ifdef RIDING_MONSTER
				// ���� �ڽ��� �����ΰ��� Ż �� �ִ� �����̶��
				if ( m_bRideToUnit )
				{
					//Ż������ Rally ����
					if ( null == m_optrRallyGameUnit )
						StopRideToUnit();
				}
				else	// Ride �� �� �ִ� ������ �ƴϸ� �������� ���� �����Ƿ�
				{
#endif RIDING_MONSTER
					m_optrRallyGameUnit.Reset();

					// ���� frame�� �ٷ� �ٽ� targeting �� �� �ֵ��� �Ѵ�
					m_fElapsedTimeAfterLastTargeting = m_pAIData->targetData.targetInterval + 1.f;
#ifdef RIDING_MONSTER
				}
#endif RIDING_MONSTER
			}			
			else	// �Ÿ����̰� DEST_GAP �̻��̸� ���� ���� ��ġ�� ������Ʈ��
			{
				m_iFinalDestLineIndex	= iRallyLineIndex;
				m_vFinalDest			= vRallyUnitPos;					
				return;
			}
		}


		// FieldFix: m_iTargetUnitUID�̱� ������ m_bLostTarget�� ���⼭ true �����ִ±�...
		// ���� lostTarget �Լ��� ���� �� �ȿ��� m_bLostTarget�� m_iTargetUnitUID ó���� �Ѵ� ���ִ°� ������
		if( false == m_bLostTarget )	// ���� ���� �Դٸ� Ÿ���� ������?
		{		
			m_bLostTarget = true;
			m_vPivotPoint = m_vPosition;	// ���� �ڸ��� �������� ��Ʈ��?
			m_iPivotPointLineIndex = m_iLastTouchLineIndex;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if( bAccumulate_ == true && m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->patrolMoveData.patrolBeginRate ) == true
                || bAccumulate_ == false && CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->patrolMoveData.patrolBeginRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( RandomFloat( 0.f, 100.f ) < m_pAIData->patrolMoveData.patrolBeginRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				SetNowPatroling( true );
			else
				SetNowPatroling( false );
		}
	}
	else	// Ÿ���� ������
	{
		// Ÿ���� �ִµ� ã�ư����� ������ �ִٸ�
		if( null != m_optrRallyGameUnit )
		{
			//dbg::clog << L"���� �� - �� ���� Ÿ��" << dbg::endl;

			// ã�ư��� �ʵ��� ��
			m_optrRallyGameUnit.Reset();
		}

		// FieldFix: Ÿ���� �ִµ� Ÿ���� �Ҿ�...�̷� ���� �ڵ�� ����̰�, ����� �Ѵ�.
		if( true == m_bLostTarget )
		{
			m_bLostTarget = false;
			SetNowPatroling( false );
		}

		// Ÿ���� �ִ� ��� ���� �������� Ÿ������ ����.. (�������� �ʴ� ���Ϳ��Դ� �ʿ� ���ڱ�..)
		m_iFinalDestLineIndex = m_optrTargetGameUnit->GetLastTouchLineIndex();
		m_vFinalDest = m_optrTargetGameUnit->GetPos();
	}
}

void CX2GUNPCAI::Moving( float fElapsedTime )
{
	KTDXPROFILE();	

	CX2GUNPC::STATE_CHANGE_TYPE message = CX2GUNPC::SCT_NO_CHANGE;
	CKTDGLineMap* pLineMap	= g_pX2Game->GetWorld()->GetLineMap();

	// ������ m_iFinalDestLineIndex�� m_vFinalDest�� �״�� ������ ���� �� �ְڱ�
	int iFinalDestLineIndex = m_iFinalDestLineIndex;
	D3DXVECTOR3 vFinalDest	= m_vFinalDest;
	bool bAIIsRight = false;


	bool bStayOnCurrLineGroup = false;
	if( GetNowPatroling() )	// ��Ʈ���� �Ѵٸ�
	{
		bStayOnCurrLineGroup = m_pAIData->patrolMoveData.bOnlyThisLineGroup;
	}
	else	// ��Ʈ���� ���Ѵٸ�
	{
		// �������� �͵� �ƴϰ�, Ư�� ������ ��ġ�� ���� �͵� �ƴϸ鼭, Chase�� ���� ���α׷츸 ���� �ѵ� (bOneyThisLineGroup�� �⺻���� false)
		if( false == m_bFleeing && null == m_optrRallyGameUnit && 
			true == m_pAIData->chaseMoveData.bOnlyThisLineGroup )
		{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
			CKTDGLineMap::LineGroupID pLineGroupCurr = pLineMap->GetLineGroupIncludesLineData( m_iLastTouchLineIndex );
			CKTDGLineMap::LineGroupID pLineGroupDest = pLineMap->GetLineGroupIncludesLineData( m_iFinalDestLineIndex );
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			CKTDGLineMap::LineGroup* pLineGroupCurr = pLineMap->GetLineGroupIncludesLineData( pLineMap->GetLineData(m_iLastTouchLineIndex ) );
			CKTDGLineMap::LineGroup* pLineGroupDest = pLineMap->GetLineGroupIncludesLineData( pLineMap->GetLineData(m_iFinalDestLineIndex ) );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			// ���� ��ġ�� �������� ��ġ�� ���� �ٸ� ���� �׷��̸�
			if( pLineGroupCurr != pLineGroupDest )
			{
				// Ÿ���� �ҵ��� �ϰ� ���� ��ġ�� �������� ��Ʈ�� ��Ų��.
				ResetTarget();
				m_bLostTarget = true;
				m_vPivotPoint = m_vPosition;
				m_iPivotPointLineIndex = m_iLastTouchLineIndex;
				SetNowPatroling( true );

				// �׸���� AI_WAIT�� ����
				message = CX2GUNPC::SCT_AI_WAIT;
				goto SEND_AI_MESSAGE;
			}
		}
	}

	// Ư�� ���ֿ��� �����ϴ� �͵� �ƴѵ� Ÿ���� �Ҿ�����
	if( null == m_optrRallyGameUnit && true == m_bLostTarget )
	{	
		// �׷��� ��Ʈ�� �ϴ� �͵� �ƴ϶��
		if( false == GetNowPatroling() )
		{
			// ������ iFinalDestLineIndex �� vFinalDest �� ������ �� ���� ���� ��ġ�� ����
			iFinalDestLineIndex = m_iLastTouchLineIndex;
			vFinalDest = m_vPosition;
		}
		else	// ��Ʈ���� �Ѵٸ�
		{			
			bool bOnEdge = false;

			// patrol�߿� ���� linegroup���� �־�� �Ѵٸ� ���θ��� ���� �ִ��� üũ
			if( true == bStayOnCurrLineGroup )
			{
				const CKTDGLineMap::LineData* pLineDataTemp = pLineMap->GetLineData( m_iLastTouchLineIndex );
				if( NULL != pLineDataTemp )
				{
					// �������� �ٶ󺸰� ������ ������ ���� üũ
					if( true == m_bIsRight &&
						( -1 == pLineDataTemp->nextLine ||
						-2 == pLineDataTemp->nextLine ) )
					{
						float fDist = GetDistance( m_vPosition, pLineDataTemp->endPos );
						if( fDist < LINE_RADIUS )
						{
							bOnEdge = true;
						}
					}
					// ������ �ٶ󺸰� ������ ������ ������ üũ
					else if( false == m_bIsRight &&
						( -1 == pLineDataTemp->beforeLine ||
						-2 == pLineDataTemp->beforeLine ) )
					{
						float fDist = GetDistance( m_vPosition, pLineDataTemp->startPos );
						if( fDist < LINE_RADIUS )
						{
							bOnEdge = true;
						}
					}
				}
			}

			// ��Ʈ�� ��, ���� ��ġ�� ������ ��ġ�� ��Ʈ�� ���� �Ÿ� �̻����� ������ �ְų�, ���� ���α׷쿡�� �־�� �ϴµ� ���� ��ġ�� ���� �׷��� �����ڸ� �̸�
			if( GetDistance(m_vPivotPoint, m_vPosition) > m_pAIData->patrolMoveData.patrolRange ||
				true == bOnEdge )
			{
				m_fElapsedPatrolCoolingTime += fElapsedTime;
				if( m_fElapsedPatrolCoolingTime > m_pAIData->patrolMoveData.patrolCoolingTime )	/// ��Ÿ���� ��������
				{		
					// �� ���⼭�� m_fElapsedPatrolCoolingTime�� �ʱ�ȭ ��Ű�� ����?
					if( true == m_bIsRight )	// �������� ���� ������ ��������~
					{
						iFinalDestLineIndex = -1;
						vFinalDest = m_vPivotPoint + D3DXVECTOR3(-10000,0,0);
					}
					else	// ������ ���� ������ ����������~
					{
						iFinalDestLineIndex = -1;
						vFinalDest = m_vPivotPoint + D3DXVECTOR3(10000,0,0);
					}
				}
				else	// cool Ÿ���� ������ �ʾ�����
				{
					// ���� ��ġ�� �������� ��
					iFinalDestLineIndex = m_iLastTouchLineIndex;
					vFinalDest = m_vPosition;
				}				
			}
			else
			{
				m_fElapsedPatrolCoolingTime = 0.f;
				if( true == m_bIsRight )
				{
					iFinalDestLineIndex = -1;
					vFinalDest = m_vPivotPoint + D3DXVECTOR3(10000,0,0);
				}
				else
				{
					iFinalDestLineIndex = -1;
					vFinalDest = m_vPivotPoint + D3DXVECTOR3(-10000,0,0);
				}
			}
		}
	}

	//{{oasis:�����////2009-10-7////
	// �׷�AI�� ������� �ʰ�
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_OFF)
	{
		//}}oasis:�����////2009-10-7////
		if( false == bStayOnCurrLineGroup )	// ���� ���α׷츸 ��Ʈ���ϴ°� �ƴ϶��
		{
			if( true == m_bFleeing ) // �������� ���̸�
			{
				// ESCAPE_MOVE�� JUMP_INTERVAL üũ
				m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
				if( m_fElapsedTimeAfterLastRandomJump > m_pAIData->escapeMoveData.jumpInterval )
				{
					m_fElapsedTimeAfterLastRandomJump = 0.f;
					const float fJumpChance = RandomFloat( 0.f, 100.f );
					
					// �������� ��ġ�� ���� ��ġ���� ���� ������
					if( vFinalDest.y > m_vPosition.y )
					{
						// ���������� UP_JUMP_RATE üũ
						if( fJumpChance < m_pAIData->escapeMoveData.upJumpRate )
						{	
							message = CX2GUNPC::SCT_AI_JUMP;
							goto SEND_AI_MESSAGE;
						}
						else if( 100.f-fJumpChance < m_pAIData->escapeMoveData.upDownRate ) // ���� ������ UP_DOWN_RATE üũ
						{
							message = CX2GUNPC::SCT_AI_DOWN;
							goto SEND_AI_MESSAGE;
						}
					}
					else	// �������� ��ġ�� ���� ��ġ���� �Ʒ��� ������
					{
						// ���� ������ DOWN_JUMP_RATE üũ
						if( fJumpChance < m_pAIData->escapeMoveData.downJumpRate )
						{
							message = CX2GUNPC::SCT_AI_JUMP;
							goto SEND_AI_MESSAGE;
						}
						else if( 100.f-fJumpChance < m_pAIData->escapeMoveData.downDownRate )	 // ���� ������ DOWN_DOWN_RATE üũ
						{
							message = CX2GUNPC::SCT_AI_DOWN;
							goto SEND_AI_MESSAGE;
						}
					}
				}
			}
			else // chase ���̸�
			{
#ifdef AI_FLY
				if(GetAIFlyNowState() == false)	// Fly ���°� �ƴ� ��쿡�� ����
#endif
				{
					// FieldFix: ��m_fElapsedTimeAfterLastRandomJump �� if(!m_bRideToUnit)�� ���ԵǾ�� �� ��
					m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
#ifdef RIDING_MONSTER
					if( !m_bRideToUnit)		/// �ٸ� ������ Ride�ϴ� ������ �ƴѰ�� ���� 
					{
#endif RIDING_MONSTER
						// FieldFix: ���� Flee ó���� ���� �ϹǷ�, �̺κ��� ���ø� �Ǵ� �Լ������� ����°� ������...
						// CHASE_MOVE�� JUMP_INTERVAL üũ
						if( m_fElapsedTimeAfterLastRandomJump > m_pAIData->chaseMoveData.jumpInterval )
						{
							m_fElapsedTimeAfterLastRandomJump = 0.f;
							const float fJumpChance = RandomFloat( 0.f, 100.f );

							if( vFinalDest.y > m_vPosition.y )
							{
								if( fJumpChance < m_pAIData->chaseMoveData.upJumpRate )
								{	
									message = CX2GUNPC::SCT_AI_JUMP;
									goto SEND_AI_MESSAGE;
								}
								else if( 100-fJumpChance < m_pAIData->chaseMoveData.upDownRate )
								{
									message = CX2GUNPC::SCT_AI_DOWN;
									goto SEND_AI_MESSAGE;
								}
							}
							else
							{
								if( fJumpChance < m_pAIData->chaseMoveData.downJumpRate )
								{
									message = CX2GUNPC::SCT_AI_JUMP;
									goto SEND_AI_MESSAGE;
								}
								else if( 100-fJumpChance < m_pAIData->chaseMoveData.downDownRate )
								{
									message = CX2GUNPC::SCT_AI_DOWN;
									goto SEND_AI_MESSAGE;
								}
							}
						}
#ifdef RIDING_MONSTER
					}
#endif RIDING_MONSTER
				}			
			}
		}
		//{{oasis:�����////2009-10-7////
	}
	//}}oasis:�����////2009-10-7////

	// �� Ÿ���õ� �������� �̵��� �غ���~
	float fDestGap = 0.f;
	//{{oasis:�����////2009-10-7////
	float fMoveGap = 0.f;

	// �׷�AI�� GAI_CMD_FORCED_MOVE�� ���� ���̸�
	if(m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_FORCED_MOVE)
	{
		fMoveGap = 15.f;
		fDestGap = 15.f;
	}
#ifdef RIDING_MONSTER
	else if(m_bRideToUnit == true)	// �ٸ� ������ Ride�ϴ� �����̸�
	{
		fMoveGap = 15.f;
		fDestGap = 15.f;
	}
#endif RIDING_MONSTER
	//}}oasis:�����////2009-10-7////
	else	/// �Ϲ� �����̸�
	{
		fDestGap = (float)m_pAIData->chaseMoveData.destGap;	// ��ũ��Ʈ�� DEST_GAP
		//{{oasis:�����////2009-10-7////
		fMoveGap = (float)m_pAIData->chaseMoveData.moveGap;	// ��ũ��Ʈ�� MOVE_GAP
		//}}oasis:�����////2009-10-7////
	}
	bool bIsTargetOnRight;
	//{{oasis:�����////2009-10-7////
	// �������� �ƴϰų�, �׷�AI�� ���� ���� ���
	if( false == m_bFleeing || m_pMasterNPC->GetGroupAICommand_LUA() != CX2GUNPC::GAI_CMD_OFF)
	{
		// ����: if( false == m_bFleeing )
		//}}oasis:�����////2009-10-7////
#ifdef AI_FLY
		if( GetAIFlyNowState() == true )	// Fly ���¸�
		{
			D3DXVECTOR3 vDistPos = vFinalDest;

			if( m_bLostTarget == true )	// Ÿ���� ���� ���
			{
				// vFinalDest ���� DEST_HEIGHT_GAP ��ŭ ���� �ֵ��� �Ѵ�.
				vDistPos.y -= m_pAIData->flymovedata.fDestHeight;	 
			}
			
			m_PathFinder.FollowTargetPathFly( /*IN fElapsedTime,*/ m_vPosition, IN m_iLastTouchLineIndex, IN vDistPos, IN iFinalDestLineIndex,
				IN m_fMaxJumpUp * 2.f, IN m_fMaxJumpRight,
				IN (const float)m_pAIData->flymovedata.fDestHeight, IN (const float)m_pAIData->flymovedata.fDestArea,
				pLineMap, IN (const float)m_pAIData->flymovedata.fDestGap, /*IN (const float)m_pAIData->chaseMoveData.moveGap,*/
				/*IN m_bFootOnLine,*/ /*IN bStayOnCurrLineGroup,*/ OUT bIsTargetOnRight, IN m_pAIData->chaseMoveData.m_fLineEndDetectRange );
		}
		else	// FLY ���°� �ƴϸ�
#endif
		{
			m_PathFinder.FollowTargetPath( /*fElapsedTime,*/ m_vPosition, m_iLastTouchLineIndex, vFinalDest, iFinalDestLineIndex,
				m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, (const float)fDestGap, (const float)fMoveGap,
				m_bFootOnLine, bStayOnCurrLineGroup, OUT bIsTargetOnRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
		}		
	}
	else	// �׷� AI�� �������� �����鼭 ���� ���̸�
	{
		// FLY�� ó�� ����?
		// oasis  fDestGap -> 150.f  ����: (const float)fDestGap,
		m_PathFinder.EscapeTargetPath( /*fElapsedTime,*/ m_vPosition, m_iLastTouchLineIndex, vFinalDest, iFinalDestLineIndex,
			m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, (const float)150.f, /*(const float)m_pAIData->chaseMoveData.moveGap,*/
			m_bFootOnLine, /*bStayOnCurrLineGroup,*/ OUT bIsTargetOnRight, m_bIsRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
	}

	// Ÿ���� ���� �ִ��� �ƴ��� �Ǵ���.. ������ ���罺����Ʈ�� VIEW_TARGET�� true�� ��쿡�� ���ϸ� �ɵ��ѵ�...
	if( m_bIsRight != bIsTargetOnRight )
	{
		m_bIsTargetFront = false;
	}
	else
	{
		m_bIsTargetFront = true;
	}


	bAIIsRight = m_bIsRight;
	const float fFinalDestDist = GetDistance( m_vPosition, vFinalDest );

	switch( m_PathFinder.GetMoveState() )
	{
	case CX2SimplePathFinder::MS_WAIT:	
		{
			message = CX2GUNPC::SCT_AI_WAIT;
		} break;

		/// �������� ����� �Ҷ�
	case CX2SimplePathFinder::MS_FORCE_LEFT:	
	case CX2SimplePathFinder::MS_LEFT:
		{
			bAIIsRight = false;

			if( true == m_bIsRight )	// ���� �������� ���� �ְ�
			{
				// ���� AI �޽����� wait, walk, dash �� �ϳ����ٸ�
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{	
					// CHASE_MOVE�� DIR_CHANGE_INTERVAL üũ
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval ) 
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			// ������ ���� �־��ٰų�, �������� ���� �־����� wait, walk, dash�� �ƴϾ��ٸ� walk �϶�� AI�� ����
			message = CX2GUNPC::SCT_AI_WALK;
		} break;

	case CX2SimplePathFinder::MS_FORCE_RIGHT:
	case CX2SimplePathFinder::MS_RIGHT: 
		{
			bAIIsRight = true;

			if( false == m_bIsRight )
			{
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			message = CX2GUNPC::SCT_AI_WALK;			
		} break;

	case CX2SimplePathFinder::MS_JUMP:
		{
			message = CX2GUNPC::SCT_AI_JUMP;
		} break;

	case CX2SimplePathFinder::MS_DOWN:
		{
			message	= CX2GUNPC::SCT_AI_DOWN;
		} break;

	case CX2SimplePathFinder::MS_JUMP_LEFT:
		{
			bAIIsRight = false;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_JUMP_RIGHT:
		{
			bAIIsRight = true;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_LEFT:
		{
			bAIIsRight = false;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_RIGHT:
		{
			bAIIsRight = true;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

#ifdef AI_FLY
	case CX2SimplePathFinder::MS_FLY_LEFT_FRONT:
	case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_UP:
	case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_DOWN:
	case CX2SimplePathFinder::MS_FLY_LEFT_BACK:
	case CX2SimplePathFinder::MS_FLY_LEFT_BACK_UP:
	case CX2SimplePathFinder::MS_FLY_LEFT_BACK_DOWN:
		{
			bAIIsRight = false;

			if( m_bIsRight == true )
			{
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_DOWN	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_DOWN	== m_PrevAIMessage )
				{		
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;		// ���� ��ȯ			
						break;
					}
				}
			}

			switch( m_PathFinder.GetMoveState() )
			{
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT:
				message = CX2GUNPC::SCT_AI_FLY_FRONT;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_UP:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_DOWN;
				break;

			case CX2SimplePathFinder::MS_FLY_LEFT_BACK:
				message = CX2GUNPC::SCT_AI_FLY_BACK;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_BACK_UP:
				message = CX2GUNPC::SCT_AI_FLY_BACK_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_BACK_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_BACK_DOWN;
				break;
			default:
				message = CX2GUNPC::SCT_NO_CHANGE;
			}
		}
		break;

	case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT:
	case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_UP:
	case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_DOWN:
	case CX2SimplePathFinder::MS_FLY_RIGHT_BACK:
	case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_UP:
	case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_DOWN:
		{
			bAIIsRight = true;

			if( m_bIsRight == false )
			{
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_DOWN	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_DOWN	== m_PrevAIMessage )
				{		
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;		// ������ȯ			
						break;
					}
				}
			}

			switch( m_PathFinder.GetMoveState() )
			{
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT:
				message = CX2GUNPC::SCT_AI_FLY_FRONT;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_UP:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_DOWN;
				break;

			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK:
				message = CX2GUNPC::SCT_AI_FLY_BACK;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_UP:
				message = CX2GUNPC::SCT_AI_FLY_BACK_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_BACK_DOWN;
				break;
			default:
				message = CX2GUNPC::SCT_NO_CHANGE;
			}
		}
		break;
	case CX2SimplePathFinder::MS_FLY_UP:
		{
			message = CX2GUNPC::SCT_AI_FLY_UP;
		}
		break;
	case CX2SimplePathFinder::MS_FLY_DOWN:
		{
			message = CX2GUNPC::SCT_AI_FLY_DOWN;
		}
		break;
#endif
	} // end of switch

	// ������ AI�� �̵��̰ų� WAIT ������
#ifdef AI_FLY	
	if( ( m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_FRONT || m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_BACK ||
		m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_FRONT_UP || m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_BACK_UP || 
		m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_FRONT_DOWN || m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_BACK_DOWN || m_PrevAIMessage == CX2GUNPC::SCT_AI_WAIT ) &&
		( message == CX2GUNPC::SCT_AI_FLY_FRONT || message == CX2GUNPC::SCT_AI_FLY_BACK ||
		message == CX2GUNPC::SCT_AI_FLY_FRONT_UP || message == CX2GUNPC::SCT_AI_FLY_BACK_UP ||
		message == CX2GUNPC::SCT_AI_FLY_FRONT_DOWN || message == CX2GUNPC::SCT_AI_FLY_BACK_DOWN ) )
	{
		m_fElapsedTimeAfterLastFlyMoveUpdate += fElapsedTime;
		if( m_fElapsedTimeAfterLastFlyMoveUpdate > (float)m_pAIData->flymovedata.fFlyMoveInterval )
		{
			m_fElapsedTimeAfterLastFlyMoveUpdate = 0.f;
		}
		else
		{
			message = m_PrevAIMessage;
		}
	}	
#endif


	// �׷�AI�� HOLD �� ���
	//{{oasis:�����////2009-10-7////
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_HOLD) // COMMAND == 1. HOLD
	{
		// ���� ��ȯ�� ó��
		if(message == CX2GUNPC::SCT_AI_DIR_CHANGE)
		{
			;
		}
		else	// �� �ܿ��� ���
		{
			message = CX2GUNPC::SCT_AI_WAIT;
		}
		goto SEND_AI_MESSAGE;
	}

	// �׷�AI�� FORCED_MOVE �� ��� 
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_FORCED_MOVE) // COMMAND == 3. FORCED MOVE
	{	
		if(true == m_pMasterNPC->GetGroupAISelfCommandTransit_LUA())
		{
			message = CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT;
			m_pMasterNPC->PushUnUseAIMessage(CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT);
			m_pMasterNPC->SetGroupAISelfCommandTransit_LUA(false);
		}
		else if( message == CX2GUNPC::SCT_AI_WAIT)
		{
			m_pMasterNPC->SetGroupAISelfCommandTransit_LUA(true);
		}
		else if( message == CX2GUNPC::SCT_AI_WALK)	// AI�� WALK�� ��� DASH��..
		{
			message = CX2GUNPC::SCT_AI_DASH;
		}

		// FieldFix: else ó���� ����..?

		goto SEND_AI_MESSAGE;
	}

	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_SKILL_A) // COMMAND == 4. SKILL A
	{	
		if(true == m_pMasterNPC->GetGroupAISelfCommandTransit_LUA())
		{
			message = CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT;
			m_pMasterNPC->PushUnUseAIMessage(CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT);
			m_pMasterNPC->SetGroupAISelfCommandTransit_LUA(false);
		}
		goto SEND_AI_MESSAGE;
	}
	//}}oasis:�����////2009-10-7////


	// FieldFix: �׷�AI�� ��� ������� ������ �ʰڱ�
	// walk, dash �߿� walk, dash���� ���º�ȭ�� CHASE_MOVE�� WALK_INTERVAL �������� ����
	if( ( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
		CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
		CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage ) &&
		( CX2GUNPC::SCT_AI_WALK == message || 
		CX2GUNPC::SCT_AI_DASH == message ) )
	{
		m_fElapsedTimeAfterLastWalkRunUpdate += fElapsedTime;

		/// walkInterval ��ŭ ��� ������
		if( m_fElapsedTimeAfterLastWalkRunUpdate > (float)m_pAIData->chaseMoveData.walkInterval )
		{	
			m_fElapsedTimeAfterLastWalkRunUpdate = 0.f;

			// ������ġ�� ������ ��ġ ������ �Ÿ��� MOVE_SPLIT_RANGE ���� ������
			if( fFinalDestDist < m_pAIData->chaseMoveData.moveSplitRange )
			{
				// NEAR_WALK_RATE Ȯ���� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->chaseMoveData.nearWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < m_pAIData->chaseMoveData.nearWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					message = CX2GUNPC::SCT_AI_WALK;	// �ȴ���..
				}
				else
				{
					message = CX2GUNPC::SCT_AI_DASH;	// �ٴ���..
				}
			}
			else	// MOVE_SPLIT_RANGE ���� ���� ������
			{
				// FAR_WALK_RATE Ȯ���� ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->chaseMoveData.farWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < m_pAIData->chaseMoveData.farWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					message = CX2GUNPC::SCT_AI_WALK;	// �ȴ���..
				}
				else
				{
					message = CX2GUNPC::SCT_AI_DASH;	// �ٴ���..
				}
			}		
		}
		else
		{
			message = m_PrevAIMessage;	// ������ AI�� �״�� ���
		}
	}

SEND_AI_MESSAGE:

#ifdef AI_FLY	
	// ȥ�� �ȵ�θ޴ٷ� ���� �� ������ ���� �������Ѵ�.
	if( GetAIFlyNowState() == true )	// �����ִ� ���
	{
		float fHeightHigh = -9999.f;		// ����ִ� ��������� Y������ ������ ���� ū ��
		float fHeightLow = 9999.f;		// ����ִ� ��������� Y������ ������ ���� ������
		for(int i=0; i<g_pX2Game->GetUserUnitListSize(); ++i)
		{			
			CX2GUUser *pUser = g_pX2Game->GetUserUnit(i);
			if(pUser != NULL && pUser->GetNowHp() > 0.f )
			{
				float fHeightDist = m_vPosition.y - pUser->GetPos().y;

				if(fHeightDist > fHeightHigh)
					fHeightHigh = fHeightDist;
				if(fHeightDist < fHeightLow)
					fHeightLow = fHeightDist;
			}
		}

		/// �� ����
		if(fHeightHigh >= 1500.f)
			message = CX2GUNPC::SCT_AI_FLY_DOWN;
		else if(fHeightLow <= -1500.f)
			message = CX2GUNPC::SCT_AI_FLY_UP;

		// �����θʿ��� �����̴� ������� ��쵵 fly�� ó���ϴµ�...
		// FLY_CHASE_MOVE �� UNDERWATER_MODE �� TRUE ��
		if( m_pAIData != NULL && m_pAIData->flymovedata.bUnderWaterMode == true && m_pMasterNPC != NULL )
		{
			int iLastLine = m_pMasterNPC->GetLastTouchLineIndex();
			CKTDGLineMap* pLineMap	= g_pX2Game->GetWorld()->GetLineMap();
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(iLastLine);
			if( pLineData != NULL && pLineData->m_bUnderWater == true )
			{
				// �����θ��� ���̸� ���� �ʵ��� ����
				if( message == CX2GUNPC::SCT_AI_FLY_UP || message == CX2GUNPC::SCT_AI_FLY_FRONT_UP || message == CX2GUNPC::SCT_AI_FLY_BACK_UP )
				{
					D3DXVECTOR3 vNpcPos = m_pMasterNPC->GetPos();
					if( vNpcPos.y >= pLineData->m_fWaterHeight - m_pMasterNPC->GetUnitHeight(true) )
						message = CX2GUNPC::SCT_AI_WAIT;
				}				
			}
			else
			{
				message = CX2GUNPC::SCT_AI_WAIT;
			}
		}
	}	
#endif

	// ���� AI �޽����� ���� AI �޽����� �ٸ��� ���ٸ� SCT_NO_CHANGE
	if( m_PrevAIMessage == message || CX2GUNPC::SCT_NO_CHANGE == message )
	{
		message = CX2GUNPC::SCT_NO_CHANGE;
	}
	else	// ���� AI�� �ٸ��ٸ� (������ȯ�� ��� SCT_AI_DIR_CHANGE �� ���� �ȴ�.)
	{
		m_PrevAIMessage = message;
		m_pMasterNPC->PushAIMessage( message, bAIIsRight );
	}
}

#ifdef RIDING_MONSTER
void CX2GUNPCAI::StopRideToUnit()
{
	m_bRideToUnit = false;
	m_optrRallyGameUnit.Reset();

	// ���� frame�� �ٷ� �ٽ� targeting �� �� �ֵ��� �Ѵ�
	m_fElapsedTimeAfterLastTargeting = m_pAIData->targetData.targetInterval + 1.f;

	ResetTarget();
	m_bFleeing = false;
	SetNowPatroling( false );
	m_bLostTarget = false;
}
void CX2GUNPCAI::SetRideToUnit( CX2GameUnit* pRallyGameUnit_ )
{
	m_bRideToUnit = true;		// �ٸ� ������ ride
	SetRallyToUnit( pRallyGameUnit_ );
}
#endif RIDING_MONSTER


void CX2GUNPCAI::SetRallyToUnit( CX2GameUnit* pRallyGameUnit_ )
{
	m_optrRallyGameUnit = pRallyGameUnit_;

	ResetTarget();
	m_bFleeing = false;
	SetNowPatroling( false );
	m_bLostTarget = false;
}


//{{ 09.04.16 �¿�
bool CX2GUNPCAI::EscapeFlagCheck( int iFlagIndex )
{
	// true�� �������� Flag ������ �����Ѵ�.
	// flag ������ �������� ���� ��쿡�� ������ true��� �Ѵ� (���� �ڵ�� �浹�� ������)

	// 0~9 ���̰� �ƴϸ� ������ �������� �ʰų� �������� index�� �ƴϹǷ� ���� �����ϰ� ����
	if( iFlagIndex < 0 || 9 < iFlagIndex )
		return true;
	else
	{
		if( true == m_pMasterNPC->GetFlag_LUA( iFlagIndex ) )
			return true;
	}

	return false;

}
//}}

//{{ oasis907 : ����� [2009.12.9] //
void CX2GUNPCAI::SetAItoMainAIData()
{ 
	m_pAIData = m_pMainAIData; 
	m_bFleeing = false;
}
void CX2GUNPCAI::SetAItoSubAIData() 
{ 
	m_pAIData = m_pSubAIData; 
	//m_bFleeing = false;
}
//}} oasis907 : ����� [2009.12.9] //