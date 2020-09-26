#include "stdafx.h"
#include ".\x2game.h"




#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GUElsword_SwordMan.h"
#include "./X2GUArme_VioletMage.h"
#include "./X2GULire_ElvenRanger.h"
#include "./X2GURaven.h"
#include "./X2GuEve.h"

	//{{ kimhc // 2010.11.25 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#include "./X2GUChung.h"
#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.11.25 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
#include "./X2GUAra.h"
#endif

#ifdef NEW_CHARACTER_EL
#include "./X2GUEl.h"
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
#include "./X2GUAdd.h"
#endif //SERV_9TH_NEW_CHARACTER

#endif REDUCED_PRECOMPILED_HEADER_TEST

 
#define X2GAME_NPC_ROBUST_PACKET_SEND_INTERVAL  (3*70)


bool CX2Game::m_sbRenderNPCName = true;





#ifdef FPS_SHOOTING_TEST

	static bool s_bFPSShootingMouseDown = false;
	static CKTDXTimer s_TimerFPSShooting;

#endif FPS_SHOOTING_TEST


#ifdef KHELP_SYSTEM_TEST
IMPLEMENT_HELP_SYSTEM( CX2Game )
{
	WCHAR   wbuffer[1024];
	int     ix = 16;
	int     iy = 16;
	StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer), L"%S", __FUNCTION__ );

	if ( g_pMain && g_pX2Game )
	{
		g_pMain->GetFontForDebuf()->OutTextXY( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
		g_pX2Game->Debug_RenderHelp( ix, iy );
	}//if
}
#endif KHELP_SYSTEM_TEST



void CX2Game::Debug_RenderHelp( int ix_, int iy_ )
{
	int     ix = ix_ + 16;
	int     iy = iy_ + 16;

	wstringstream wstrm;


#ifdef CAMERA_SCRIPTING_TEST
	if( NULL != GetFocusUnit() )
	{

		D3DXVECTOR3 vOriginalCameraPosition = GetX2Camera()->CalcNormalCameraPosition( GetFocusUnit(), g_pMain->GetGameOption().GetCameraDistance(), 0.f, 0.f, 200.f, 0.f );
		D3DXVECTOR3 vCurrentCameraPosition = GetX2Camera()->GetCamera().GetEye();
		D3DXVECTOR3 vCameraPositionDesplace = vCurrentCameraPosition - vOriginalCameraPosition;

		D3DXVECTOR3 vDirVector = GetFocusUnit()->GetDirVector();
		D3DXVECTOR3 vZVector = GetFocusUnit()->GetZVector();
		D3DXVec3Normalize( &vDirVector, &vDirVector );
		D3DXVec3Normalize( &vZVector, &vZVector );

		D3DXVECTOR3 vRelativeCameraPosition;
		vRelativeCameraPosition.x = D3DXVec3Dot( &vDirVector, &vCameraPositionDesplace );
		vRelativeCameraPosition.y = vCameraPositionDesplace.y;
		vRelativeCameraPosition.z = D3DXVec3Dot( &vZVector, &vCameraPositionDesplace );


		D3DXVECTOR3 vDistance = vCurrentCameraPosition - GetFocusUnit()->GetPos();
		float fCurrentCameraDistance = D3DXVec3Length( &vDistance );


		wstrm << L"CAMERA_DISTANCE: " << (int)fCurrentCameraDistance << std::endl;
		wstrm << L"CAMERA_DISPLACE: ( " << (int)vRelativeCameraPosition.x << ", " 
			<< (int)vRelativeCameraPosition.y << ", " 
			<< (int)vRelativeCameraPosition.z << " ), " << std::endl;

		wstrm << L"CAMERA_POSITION: ( " << (int)vCurrentCameraPosition.x << ", " 
			<< (int) vCurrentCameraPosition.y << ", " 
			<< (int) vCurrentCameraPosition.z << " ), " << std::endl;

		wstrm << std::endl;

	}
#endif CAMERA_SCRIPTING_TEST




	for( UINT i=0; i<m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pUser = m_UserUnitList[i];
		if( NULL == pUser )
			continue;

		wstrm << L"idx: " << i << ", "
			<< L"uid: " << pUser->GetUnitUID() << ", "
			<< L"pos: (" 
			<< (int)pUser->GetPos().x << ", " 
			<< (int)pUser->GetPos().y << ", " 
			<< (int)pUser->GetPos().z << "), "
			<< L"HP: " << pUser->GetNowHp() << ", "
			<< L"����:" << (int) pUser->GetGameUnitState() << ", "
			<< std::endl;
	}


	for( UINT i=0; i<m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = m_NPCUnitList[i];
		if( NULL == pNPC )
			continue;

        std::wstring    wstrTemp;

		wstrm << L"idx: " << i << ", "
			<< pNPC->GetNPCTemplet().m_Name << L", " 
			<< L"id: " << pNPC->GetNPCTemplet().m_nNPCUnitID << L", " 
			<< L"uid: " << pNPC->GetUID() << L", "
			<< L"pos: (" 
			<< (int)pNPC->GetPos().x << ", " 
			<< (int)pNPC->GetPos().y << ", " 
			<< (int)pNPC->GetPos().z << "), "
			<< L"HP: " << pNPC->GetNowHp() << ", " 
			<< L"����:" << ConvertUtf8ToWCHAR( wstrTemp, pNPC->GetStateNameByStateID( static_cast<int>(pNPC->GetGameUnitState() ) ) ) << ", " 
			<< std::endl;
	}

	g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wstrm.str().c_str(), D3DXCOLOR( 1, 1, 0, 1), 
		CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
	iy += 16 * 3;
}




CX2Game::CX2Game() :
//{{ kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾�
m_bCanUseESC( false ),
//}} kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾�
m_GameType( GT_NONE ),
m_GameState( GS_INIT ),
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
m_uidMySlotUID( 0 ),
m_uidHostSlotUID( 0 ),
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//m_bHost( false ),
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
m_pRoom( NULL ),
m_fTime( 0.f ),
m_fElapsedTime( 0.f ),
m_fGameTime( 0.f ),
m_bGameWait( false ),
m_optrMyUnit(),
m_optrFocusUnit(),
m_fFocusTime( 0.f ),
m_pWorld( NULL ),
m_pCamera( NULL ),
m_pDamageEffect( NULL ),
//m_pMajorParticle( NULL ),
//m_pMinorParticle( NULL ),
//m_pMajorXMeshPlayer( NULL ),
//m_pMinorXMeshPlayer( NULL ),
m_bBGMOn( true ), 
m_bLastKillCheck( false ),
m_fLastKillWaitTime( 0.05f ),
m_fLastKillWaitTimeAfterRebirth( 1.f ), 
m_bEnableCommandKeyProcess( true ),
m_bEnableAllKeyProcess( true ),		
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
m_iPacketSendFrameMoveCount( 0 ),
m_iNPCRobustPacketSendIndex( 0 ), 
m_dwNPCRobustPacketSendBaseFrameMoveCount( 0 ),
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//m_NPCPacketSendInterval( 0.f ),
//m_NPCPacketSendIntervalNow( 0.f ),
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
m_LoadingPercent( 0 ),
m_pDropItemManager( NULL ),
m_pFontForUnitName( NULL ),
m_bStopUnitChain( false ),
m_fLastkillShotTime( 0.f ),	
m_pRenderTargetLastkillShot( NULL ),
m_bOpenChatBox( false ),
m_pTalkBoxMgr( NULL ),
m_fElapsedTimeAfterLastKeyboardInput( 0.f ),
m_RedTeamKill( 0 ),
m_BlueTeamKill( 0 ),
m_bRenderUserName( true ),
m_fSkipTime( 0.f ),
m_bShowF1Help( false ),
m_pTextureF1Help( NULL ),
m_pFontF1Help( NULL ),
m_bEnableNPCTalkBox( true ),
m_fElapsedTimeAfterDisableNPCTalkBox( 0.f ),
m_GameScore( -1 ),
m_GameScoreBack( -1 ),
m_fTechPointViewTime( 0.f ),
m_TechPoint( 0 ),
m_TechPointFull( 0 ),
m_hSeqTech( INVALID_PARTICLE_SEQUENCE_HANDLE ),
m_pTextGuideHelp( NULL ),
m_fTimePauseNPCAI( 0.f ),
m_bFreeCamera( false ),
m_pDLGMyScore( NULL ),
m_pDLGSkillSlot( NULL ),		
m_TeamScoreAdvRed( 0 ),
m_TeamScoreAdvBlue( 0 ),
m_TeamVPMaxAdvRed( 0 ),
m_TeamVPMaxAdvBlue( 0 ),
m_AllowFrameCount( 0 ),
m_SpeedHackCheck( 0 ),
m_GetEXPInGame( 0 ),		
m_bGameEnd( false ),
m_bShowAllUnitGageBar( true ),
m_StateString( L"" ),
m_bSkipKeyPressed( false ),
m_pNPCFunc( NULL ),
m_pDLGMissionOpening( NULL ),
m_pDLGMissionProgress( NULL ),
m_bFixedCamera( false ),
m_bEnableMonsterFlyTest( false )
#ifdef NEW_SKILL_TREE
,
m_bSkillSlotJustChanged(true)
#endif
, m_bShowTeleportEffectOnStageStart( false )

#ifdef FPS_SHOOTING_TEST
, m_iFPSShootingBulletType( 1 )
#endif FPS_SHOOTING_TEST
#ifdef DAMAGE_HISTORY
, m_bSendLeaveRoom(false)
#endif
#ifdef KEYFRAME_CAMERA
, m_bWorldCameraEdit(false)
#endif KEYFRAME_CAMERA
//{{ kimhc // 2011-07-28 // �ɼǼ�ġȭ
, m_bIsDamageFreeGame( false ) // 1���� ���� �����ΰ�?
//}} kimhc // 2011-07-28 // �ɼǼ�ġȭ
#ifdef	BATTLE_FIELD_TEST
, m_optrHostGameUnit()
#endif	BATTLE_FIELD_TEST
, m_fResurrectionRemainTime( 0.f )
, m_bIsSendResurrectionReq( false )
, m_bResurrectionCountStart( false )
, m_pDLGResurrect( NULL )
, m_UnitUIDToResurrect( -1 )
, m_pDLGContinue( NULL )
, m_pSoundContinue( NULL )
, m_pDLGCheckResurrectByUseCash( NULL )
, m_fTimeWaitStopContinue( 0.f )
, m_bStopRebirthCount( false )
, m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( 0.f )
, m_bEnableSendKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( true )
, m_fAutoResurrectionTime (0.f)	/// �ʵ忡�� DIE���·� 5���̻� ����� �ڵ����� ������ �̵��ϱ� ���� ī��Ʈ
#ifdef MODIFY_DUNGEON_STAGING
, m_pCinematicUI ( NULL )
#endif //MODIFY_DUNGEON_STAGING
, m_bCanNotInputAndPauseNPCAI( false )
#ifdef FIX_OBSERVER_MODE
, m_uiPositionIndexRed( 0 )
, m_uiPositionIndexBlue( 0 )
#endif FIX_OBSERVER_MODE
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / ���� �÷��� ��, �ҷ� ���� ���� ���� �ý���
, m_iSubStageTechPoint ( 0 )				// SubStage �� ��ũ�� ����Ʈ�� �ʱ�ȭ�Ѵ�.

#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef NOT_RENDER_NPC_GAME_EDIT
, m_bShowNpcByGameEdit( true )
#endif // NOT_RENDER_NPC_GAME_EDIT
#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
, m_bIsAlreadySendingFrame ( false )
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
{	
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
    CX2GUNPC::ResetReactionSimulationCounter();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

// #ifndef _SERVICE_
// 	g_pData->GetUnitManager()->OpenNPCStatScriptFile( L"NPCStat.lua" );
// #endif	

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_kFrameUDPPack.ResetFrameUDPPack( false );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef	ELOG_STATISTICS_NEW_DATA
	m_kGameStatistics.Init();
#else	ELOG_STATISTICS_NEW_DATA
	m_AverageRenderFrameRate = 0;
#endif	ELOG_STATISTICS_NEW_DATA

	// variable init
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < ARRAY_SIZE( m_hEvents ); i++ )
	{
		m_hEvents[i] = NULL;
	}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for( int i=0; i< ARRAY_SIZE( m_arraySkillCoolTime ); i++ )
	{
		m_arraySkillCoolTime[i] = 0.f;
	}

	m_CreateNPCDataList.reserve( 32 );
	m_vecF1HelpString.reserve( 16 );


	m_LineSyncPacketSendInterval	= 3.f;
	m_LineSyncPacketSendIntervalNow	= 0.0f;





	// variable assign
	g_pX2Game					= this;
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
    m_iPacketSendFrameMoveCount = 0;
    m_iNPCRobustPacketSendIndex = 0;
    m_dwNPCRobustPacketSendBaseFrameMoveCount = 0;
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//	m_NPCPacketSendInterval		= (1.0f/8.0f);
//	m_NPCPacketSendIntervalNow	= 0.0f;
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	m_fLastKillWaitTime				= 0.05f;	//FieldFix: �� �ڵ�� �ʿ� ����
	m_fLastKillWaitTimeAfterRebirth	= 1.f;


	m_UnitList.reserve( 64 );
	m_UserUnitList.resize(MAX_GAME_USER_UNIT_NUM);
	m_NPCUnitList.resize(MAX_GAME_NPC_UNIT_NUM);

	fill( m_UserUnitList.begin(), m_UserUnitList.end(), (CX2GUUser*)NULL );
	fill( m_NPCUnitList.begin(), m_NPCUnitList.end(), (CX2GUNPC*)NULL );


	m_pFontForUnitName			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );
	m_pRenderTargetLastkillShot = g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"LastKillShot", 1024, 768 );

	for( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
	{
		CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData (i );
		if( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
		{
			pSlotData->m_pUnitViewer->SetShowObject( false );
		}
	}	

	m_pTalkBoxMgr = CX2TalkBoxManager::CreateTalkBoxManger();
	m_pTextureF1Help		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"FadeInOut.dds" );	
	m_pFontF1Help			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );


	if ( g_pX2Room != NULL )
	{
		if ( g_pX2Room->GetMySlot() != NULL )
		{
			if ( g_pX2Room->GetMySlot()->m_bObserver == false )
			{
				m_pDLGMyScore			= new CKTDGUIDialog( this, L"DLG_Game_My_Score_NEW.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyScore );
#ifdef REFORM_UI_SKILLSLOT
				//���������� ��ųâ�� ���� �ֱ� ���� UI�Ŵ������� �����ϵ��� ����
#if defined(WORLD_TOOL) || defined( X2TOOL)
				if( g_pData->GetUIManager() != NULL)
				{
					g_pData->GetUIManager()->CreateUISkillTree();
					m_pDLGSkillSlot = g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot();
				}
#else
				m_pDLGSkillSlot			= g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot();
#endif // WORLD_TOOL
#else
				m_pDLGSkillSlot			= new CKTDGUIDialog( this, L"DLG_Game_Skill_Slot.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillSlot );
#endif
			}
		}
	}
	ResetReBirthStoneNumUI();

	m_pNPCFunc = new CX2GUNPCFunc();


	m_AllowFrameCount		= (int)(85 * 5.3);


#ifdef BEE_FLY
	m_bEnableMonsterFlyTest = true;
#endif //BEE_FLY

#ifdef DUNGEON_ITEM
	m_vDropItemPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_optrItemDamageEffectGameUnit.Reset();
	m_fItemDamageEffectTime = 0.f;
	m_iItemDamageEffectIndex = 0;
#endif

	InitF1HelpString();

	g_pKTDXApp->GetDIManager()->SetEnable( true );

	if ( g_pData->GetMessenger() != NULL )
	{
		g_pData->GetMessenger()->SetOpen( false );
	}

	if ( g_pMain->GetInformerManager() != NULL )
		g_pMain->GetInformerManager()->ClearAllInfo();

	//g_pMain->SendHackMail();
	//if( g_pData != NULL && g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	//	g_pMain->SendHackMail_DamageHistory("Game Start");

	// variable init
	ZeroMemory( m_StopContinueAlarmText, sizeof(WCHAR) * 256 );



#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::InitializeCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

#ifdef SERV_PROTECT_AUTO_HACK
	if( g_pInstanceData != NULL )
		g_pInstanceData->SetSendHackMail_AutoHack(false);
#endif // SERV_PROTECT_AUTO_HACK

#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->GetTalkBoxManager() != NULL)
	{
		g_pData->GetPetManager()->GetTalkBoxManager()->Clear();
	}
#endif

	m_iNpcDiePacket = 0;

	m_fShowAlarmTime = 0.f;
	m_pShowAlarm = NULL;
	m_hDangerAlarm = INVALID_PARTICLE_SEQUENCE_HANDLE;

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	m_vecRemoveDamageEffect.clear();
#endif

#ifdef ADD_KPROTECTEDTYPE_VALUE
	if( g_pInstanceData != NULL )
		g_pInstanceData->SetVerifyNpcStateIdCnt(0);
#endif

#ifndef ADD_COLLECT_CLIENT_INFO
	if( g_pMain != NULL )
	{
		for(int i=0; i<2; ++i)
			g_pMain->SendHackMailGameStart( i, true );
	}
#endif

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	g_pInstanceData->m_mapCheckUDPTimer.clear();
	g_pInstanceData->m_mapCheckUDPTDelayCount.clear();

#ifdef UDP_DOWNLOAD_BLOCK_CHECK
	g_pInstanceData->m_setAbuseUnitUid.clear();
	g_pData->GetGameUDP()->InitConnectTestToCheck();
#endif //UDP_DOWNLOAD_BLOCK_CHECK
	
#endif//UDP_CAN_NOT_SEND_USER_KICK

#ifdef LOG_UDP_CHECK
	g_pInstanceData->m_mapCheckUDPTimerTemp.clear();
	g_pInstanceData->m_mapCheckUDPTDelayCountTemp.clear();
#endif//LOG_UDP_CHECK

#ifdef MODIFY_DUNGEON_STAGING
#ifdef WORLD_TOOL
	m_pCinematicUI = NULL;
#else
	m_pCinematicUI = new CinematicUI();
#endif //WORLD_TOOL
#endif //MODIFY_DUNGEON_STAGING

#ifdef SHOW_ATTACK_BOX_DUMMY
	if( NULL != GetMajorXMeshPlayer() )
		GetMajorXMeshPlayer()->SetShowAttackBox( false );
#endif //SHOW_ATTACK_BOX_DUMMY

	if( NULL != g_pData && 
		NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
	{
		g_pData->GetUIManager()->GetUIQuickSlot()->SetWaiting_EGS_USE_QUICK_SLOT_ACK( false );
	}

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
	/// �طο� ȿ����� ���ǵ� �������� ����� ����Ʈ ����
	SetOfficiallyDebuffList();
#endif // HAMEL_SECRET_DUNGEON
}


CX2Game::~CX2Game()
{	
	/*
	Release() ���� ����
	*/
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
    CX2GUNPC::ResetReactionSimulationCounter();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
}

/** @function : Release
	@brief : X2Game�� �Ҹ��ڿ� ������ �͵��� �̰����� ������
	(�̷��� ���� ����� �� ������ X2Game �Ҹ��ڰ� X2GameUnit ���� �Ҹ��� ��
	X2GameUnit ���� �Ҹ��� ���� �Ǵ� ȣ��Ǵ� �Լ� �鿡 ���ο��� �ٽ� X2Game�� �����ϴ� ���
	�� ���Ƽ� �� ã�Ƴ��� ����⿡ �̷��� Release�� ��������ϴ�.
	X2Game�� �Ҹ�Ǵ� ���� ���� Pvp, Dungeon, BattleField�� ������Ʈ�� �Ҹ�� �� �̹Ƿ�
	�ش� ������Ʈ�� �Ҹ��ڿ��� Release�� ȣ�� �ϵ��� �Ͽ����ϴ�.
	�� ���� ����� ������ ���� �˷��ּ��� - 2012-11-11 (����ö)
*/
/*virtual*/ void CX2Game::Release()
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD	
	::EnterCriticalSection( &m_csGameIntruder );
	for ( int i = 0; i < (int)m_vecIntrudeData.size(); i++ )
	{
		IntrudeData* pIntrudeData = m_vecIntrudeData[i];
		SAFE_DELETE( pIntrudeData );
	}
	m_vecIntrudeData.clear();
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    FlushSendFrameAverage();
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	SendFrameAverage();
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	if (m_pRoom != NULL)
		m_pRoom->SetCheckUDPGameCountStart(false);

	g_pInstanceData->m_mapCheckUDPTimer.clear();
	g_pInstanceData->m_mapCheckUDPTDelayCount.clear();

#ifdef UDP_DOWNLOAD_BLOCK_CHECK
	g_pInstanceData->m_setAbuseUnitUid.clear();
#endif //UDP_DOWNLOAD_BLOCK_CHECK

#endif//UDP_CAN_NOT_SEND_USER_KICK

	//m_NPCUnitList.clear();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	//m_UserUnitList.clear();

#ifdef BUFF_ICON_UI
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();
	// 	if( NULL != pGageManager )
	// 	{
	// 		pGageManager->ClearBuffList();
	// 	}	
#endif //BUFF_ICON_UI

	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if ( NULL != pCX2GameUnit )
			pCX2GameUnit->FinishAndClearAllBuff();
#ifdef  X2OPTIMIZE_X2GAME_RELEASE_CRASH_BUG_FIX
		if ( pCX2GameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			CX2GUUser* pCX2GUUser = static_cast<CX2GUUser*>( pCX2GameUnit );
			if ( pCX2GUUser->GetUnitIndex() >= 0 && pCX2GUUser->GetUnitIndex() < (int) m_UserUnitList.size()
				&& m_UserUnitList[ pCX2GUUser->GetUnitIndex() ] == pCX2GUUser )
			{
				m_UserUnitList[ pCX2GUUser->GetUnitIndex() ] = NULL;
			}
		}
		else
		{
			CX2GUNPC* pCX2GUNPC = static_cast<CX2GUNPC*>( pCX2GameUnit );
			if ( pCX2GUNPC->GetUnitIndex() >= 0 && pCX2GUNPC->GetUnitIndex() < (int) m_NPCUnitList.size()
				&& m_NPCUnitList[ pCX2GUNPC->GetUnitIndex() ] == pCX2GUNPC )
			{
				m_NPCUnitList[ pCX2GUNPC->GetUnitIndex() ] = NULL;
			}
		}
		m_UnitList[i] = NULL;	// note!! �̺�� GUEve��ü�� ������ �� �ڽ��� ��ȯ�� ���͸� �����ؼ� �����ϵ��� �ϴµ�, �̶� m_UnitList���� �˻��ϱ� ������ �̹� ������ unit�� NULL�� �������ش�.
		SAFE_DELETE_KTDGOBJECT( pCX2GameUnit );
#else   X2OPTIMIZE_X2GAME_RELEASE_CRASH_BUG_FIX
		SAFE_DELETE_KTDGOBJECT( pCX2GameUnit );
		m_UnitList[i] = NULL;	// note!! �̺�� GUEve��ü�� ������ �� �ڽ��� ��ȯ�� ���͸� �����ؼ� �����ϵ��� �ϴµ�, �̶� m_UnitList���� �˻��ϱ� ������ �̹� ������ unit�� NULL�� �������ش�.
#endif  X2OPTIMIZE_X2GAME_RELEASE_CRASH_BUG_FIX
	}
	m_UnitList.clear();
	m_UserUnitList.clear();
	m_NPCUnitList.clear();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for ( UINT i = 0; i < m_IntruderList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_IntruderList[i];

		SAFE_DELETE_KTDGOBJECT( pCX2GUUser );

		m_IntruderList[i] = NULL;
	}
	m_IntruderList.clear();

	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for ( UINT i = 0; i < m_CreateNPCDataList.size(); i++ )
	{
		CreateNPCData* pCreateNPCData = m_CreateNPCDataList[i];
		SAFE_DELETE(pCreateNPCData);
	}
	m_CreateNPCDataList.clear();



	//if( NULL != m_pMajorParticle )
	//{
	//	m_pMajorParticle->DestroyInstanceHandle( m_hSeqTech ); 
	//}

	SAFE_DELETE_DIALOG( m_pDLGResurrect );
	SAFE_DELETE_DIALOG( m_pDLGCheckResurrectByUseCash );
	ZeroMemory( &m_ReservedPacket, sizeof(KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ) );
	ClearDangerAlarm();
	if( NULL != GetMajorParticle() )
	{
		GetMajorParticle()->DestroyInstanceHandle( m_hSeqTech ); 
		GetMajorParticle()->DestroyInstanceHandle( m_hDangerAlarm ); 
	}

	SAFE_DELETE( m_pWorld );
	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pDamageEffect );

	GetEffectSet()->StopEffectSetAll();


	SAFE_DELETE( m_pDropItemManager );

	//SAFE_DELETE( m_pMajorParticle );
	//SAFE_DELETE( m_pMinorParticle );
	//SAFE_DELETE( m_pMajorXMeshPlayer );
	//SAFE_DELETE( m_pMinorXMeshPlayer );


	if( NULL != g_pData->GetPicChar() ) 
		g_pData->GetPicChar()->Clear();
	if( NULL != g_pData->GetPicCharRed() )
		g_pData->GetPicCharRed()->Clear();
	if( NULL != g_pData->GetPicCharBlue() )
		g_pData->GetPicCharBlue()->Clear();
	if( NULL != g_pData->GetPicCharGreen() )
		g_pData->GetPicCharGreen()->Clear();
	if( NULL != g_pData->GetPicCharYellow() )
		g_pData->GetPicCharYellow()->Clear();
	if( NULL != g_pData->GetPicCharGameScore() )
		g_pData->GetPicCharGameScore()->Clear();
	if( NULL != g_pData->GetPicCharTechPoint() )
		g_pData->GetPicCharTechPoint()->Clear();






	SAFE_CLOSE( m_pRenderTargetLastkillShot );

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxMgr );

	SAFE_CLOSE( m_pTextureF1Help );

	SAFE_DELETE_DIALOG( m_pDLGMyScore );
	SAFE_DELETE( m_pNPCFunc );

	SAFE_DELETE_DIALOG( m_pDLGMissionOpening );
	SAFE_DELETE_DIALOG( m_pDLGMissionProgress );

	for( int i = 0; i < (int)m_ScoreTimeList.size(); i++ )
	{
		ScoreTime* pScoreTime = m_ScoreTimeList[i];
		SAFE_DELETE( pScoreTime );
	}
	m_ScoreTimeList.clear();

	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForUnitName );
	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontF1Help );

	g_pData->GetSlideShot()->ScenEnd();

	g_pX2Game	= NULL;

	//RemoveAllGuideText();

	g_pKTDXApp->GetDIManager()->SetEnable( true );
	g_pKTDXApp->GetDGManager()->StartFadeIn( 0.1f );

	if( NULL != g_pData->GetMessenger() )
	{
		g_pData->GetMessenger()->SetOpen( false );
	}
	if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree()
		&& NULL != g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetEnable(true);
	}
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	m_vecRemoveDamageEffect.clear();
#endif

	bool bLoop      = true;
	DWORD ret       = NULL;


	//g_pMain->SendHackMail();
	//if( g_pData != NULL && g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	//	g_pMain->SendHackMail_DamageHistory("Game End");
#ifdef CHECK_FROM_DLLMANAGER
	if( g_pMain != NULL )
	{
		g_pMain->DllManagerReport();
	}
#endif

	if( g_pData->GetPartyManager() != NULL )
	{
		g_pData->GetPartyManager()->SetGameStartFlag(false);
	}

#ifdef MODIFY_DUNGEON_STAGING
	SAFE_DELETE(m_pCinematicUI);	
#endif //MODIFY_DUNGEON_STAGING

	CX2GageManager::GetInstance()->ClearPvpMemberUI();

	GetMajorParticle()->DestroyAllInstances();
	GetMinorParticle()->DestroyAllInstances();
	GetMajorXMeshPlayer()->DestroyAllInstances();
	GetMinorXMeshPlayer()->DestroyAllInstances();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::DeleteCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{
		g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect(CX2RelationshipEffectManager::REST_HIDE_ALL);
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

//{{ robobeg : 2013-06-12
	g_pData->GetUnitManager()->UnloadAllNPCInitData();
//}} robobeg : 2013-06-12

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
    CX2GUNPC::ResetReactionSimulationCounter();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

WORD    CX2Game::Handler_P2P_UnitUserSyncPack( const BYTE* pbyData, BYTE byNumPacks, WORD wMaxSize, UidType unitUID, DWORD dwFrameMoveCount )
{
	CX2GUUser* pUserUnit = GetUserUnitByUID( unitUID );

	if( pUserUnit != NULL )
	{
		//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		// PVP �̰�, ������ ����UID�� 0�� �ƴϰ�, UDP üũ�� ������ �Ŀ�
		if( GetGameType() == GT_PVP && m_pRoom != NULL && unitUID > 0 )
		{
			// �ð� ���ź�
			std::map< UidType, boost::timer >::iterator mitFirst = m_mapCheckUDPTimer.find( unitUID );
			if( mitFirst == m_mapCheckUDPTimer.end() )
			{
				// 2012.06.17 lygan_������ // ó�� ��ũ�� �ְ� �޾Ҵ� �������
				boost::timer timerFirst;
				timerFirst.restart();
				m_mapCheckUDPTimer.insert( std::make_pair( unitUID, timerFirst ) );
			}
			else
			{
				mitFirst->second.restart();
			}

			// �˻��
			std::map< UidType, boost::timer >::iterator mit;
			for ( mit = m_mapCheckUDPTimer.begin(); mit != m_mapCheckUDPTimer.end(); mit++ )
			{
				// 2012.06.17 lygan_������ // �ι�° ��ũ �����ð��� 5�� �̻��̴� �̻��ϴ�
				// 2012.06.17 lygan_������ // ���� ���� �Ȱ� �ƴ��� üũ
				if( mit->second.elapsed() > 1.0f &&
					NULL != GetMyUnit() &&
					mit->first != GetMyUnit()->GetUnitUID() )
				{
					// �ش� ������ ���� ���ӳ��� �����ϰ�,������ ���°� USI_LOSE ���� �� ���� ū ����(p2p ��Ŷ�� ���������� ������ �ϴ� ���µ�) �� ���� �ǽ������� ����� �޶�� ��û��
					CX2GUUser* pGUUserToBeChecked = GetUserUnitByUID( mit->first );
					if ( NULL != pGUUserToBeChecked && pGUUserToBeChecked->GetNowStateID() > static_cast<int>( CX2GUUser::USI_LOSE ) )
					{
						// �������� �����ϹǷ� �ٷ� ��ϴ�. ����� ����� �����̷� ������ �޶�� ��û
						KEGS_FORCE_RELAY_NOT kPacket;
						kPacket.m_iTargetUnitUID		= mit->first;

						if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
							g_pData->GetServerProtocol()->SendPacket( EGS_FORCE_RELAY_NOT, kPacket );
					}							
					// �ѹ� �Ű� �������� �ð� �ʱ�ȭ ���ش�.
					mit->second.restart();
				}
			}
		}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		AbuserUserCheck( unitUID ); //������ �ǿ� ���� üũ
#endif //UDP_CAN_NOT_SEND_USER_KICK

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
		IncreaseUserPacketCountToReceive( unitUID );
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

		// �˻� �� ���� ó��

        const BYTE* pbyBegin = pbyData;
        const BYTE* pbyEnd = pbyData + wMaxSize;
        KDYNAMIC_UNIT_USER_SYNC kUserSync;
        kUserSync.Reset();
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        kUserSync.m_usDataFieldFlag = KDYNAMIC_UNIT_USER_SYNC::eUnitUserSync_DataField_All;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        for( unsigned u = 0; u != byNumPacks; ++u )
        {
            if ( kUserSync.Deserialize( pbyData, pbyEnd ) == false )
                return 0;
            pUserUnit->ReceiveFrameData( ( dwFrameMoveCount + u + 1 >= byNumPacks ) ? ( dwFrameMoveCount + u + 1 - byNumPacks ) : 0, kUserSync );
        }//for
        return (WORD) ( pbyData - pbyBegin );
	}
    else
    {
        const BYTE* pbyBegin = pbyData;
        const BYTE* pbyEnd = pbyData + wMaxSize;
        KDYNAMIC_UNIT_USER_SYNC kUserSync;
        kUserSync.Reset();
        for( unsigned u = 0; u != byNumPacks; ++u )
        {
            if ( kUserSync.Deserialize( pbyData, pbyEnd ) == false )
                return 0;
        }//for
        return (WORD) ( pbyData - pbyBegin );
    }//if.. else..

    return 0;
}

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

WORD    CX2Game::Handler_P2P_UnitNpcMiniSyncPack( const BYTE* pbyData, BYTE byNumNPCs, WORD wMaxSize, UidType pidUnitUID, DWORD dwFrameMoveCount )
{
    if ( pbyData == NULL || byNumNPCs == 0 || wMaxSize == 0 )
        return 0;

    const BYTE* pbyBegin = pbyData;
    const BYTE* pbyEnd = pbyData + wMaxSize;
    KDYNAMIC_UNIT_NPC_MINISYNC  kNPCMIniSync;
    for( unsigned u = 0; u != byNumNPCs; u++ )
    {
        if ( kNPCMIniSync.Deserialize( pbyData, pbyEnd ) == false )
            return 0;
        if ( pidUnitUID != 0 )
        {
            CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( kNPCMIniSync.m_iUnitUID );
            if ( pCX2GUNPC != NULL )
            {
                pCX2GUNPC->ReceiveMiniSyncData( pidUnitUID, dwFrameMoveCount, kNPCMIniSync );
            }
        }
    }//for

    return  (WORD) ( pbyData - pbyBegin );
}

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

WORD    CX2Game::Handler_P2P_UnitNpcSyncPack( const BYTE* pbyData, BYTE byNumNPCs, WORD wMaxSize, UidType pidUnitUID, DWORD dwFrameMoveCount )
{
    if ( pbyData == NULL || byNumNPCs == 0 || wMaxSize == 0 )
        return 0;

    const BYTE* pbyBegin = pbyData;
    const BYTE* pbyEnd = pbyData + wMaxSize;
    KDYNAMIC_UNIT_NPC_SYNC  kNPCSync;
    for( unsigned u = 0; u != byNumNPCs; u++ )
    {
        if ( kNPCSync.Deserialize( pbyData, pbyEnd ) == false )
            return 0;
        if ( pidUnitUID != 0 )
        {
            CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( kNPCSync.m_iUnitUID );
            if ( pCX2GUNPC != NULL )
            {
                pCX2GUNPC->ReceiveFrameData( pidUnitUID, dwFrameMoveCount, kNPCSync );
//			    if ( bFirst_ )
//			    {
//#ifdef FIX_NPC_SHOW_OBJECT
//				    if( true == pCX2GUNPC->GetShowObjectCurrentState() )
//					    pCX2GUNPC->SetShowObject( true );
//#endif //FIX_NPC_SHOW_OBJECT
//			    }
            }
        }
    }//for

    return  (WORD) ( pbyData - pbyBegin );
}

WORD    CX2Game::Handler_P2P_UnitPetSyncPack( const BYTE* pbyData, BYTE byNumNPCs, WORD wMaxSize )
{
    if ( pbyData == NULL || byNumNPCs == 0 || wMaxSize < sizeof(KXPT_UNIT_PET_SYNC) * byNumNPCs )
        return 0;

    KXPT_UNIT_PET_SYNC  kPetSync;
	for( unsigned u = 0; u != byNumNPCs; u++, pbyData += sizeof(KXPT_UNIT_PET_SYNC) )
	{
        const KXPT_UNIT_PET_SYNC* pkSync = (const KXPT_UNIT_PET_SYNC*) pbyData;
		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetPet( pkSync->unitUID );
			if( pPet != NULL )
			{
				pPet->ReceiveFrameData( *pkSync );
			}
		}
	}

    return  (WORD) ( sizeof(KXPT_UNIT_PET_SYNC) * byNumNPCs );
}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
///*virtual*/ void CX2Game::Handler_P2P_UnitNpcSyncPack( CKTDNUDP::RecvData* pRecvData, bool bFirst_ /*= false*/ )
//{
//	KSerBuffer ksBuff;
//	ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//	KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;
//	DeSerialize( &ksBuff, &kXPT_UNIT_NPC_SYNC_PACK );
//
//	for( UINT i = 0; i < kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size(); ++i )
//	{
//		KXPT_UNIT_NPC_SYNC* pXPT_UNIT_NPC_SYNC = &kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList[i];
//		CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( pXPT_UNIT_NPC_SYNC->unitUID );
//		if( pCX2GUNPC != NULL )
//		{
//			pCX2GUNPC->ReceiveFrameData( pXPT_UNIT_NPC_SYNC );						
//
//			if ( bFirst_ )
//			{
//#ifdef FIX_NPC_SHOW_OBJECT
//				if( true == pCX2GUNPC->GetShowObjectCurrentState() )
//					pCX2GUNPC->SetShowObject( true );
//#endif //FIX_NPC_SHOW_OBJECT
//			}
//		}
//// 		else
//// 		{
//// 			if ( bFirst_ )
//// 				DISPLAY_ERROR( L"NPC doesn't exist" );
//// 		}
//	}
//}
//
///*virtual*/ void CX2Game::Handler_P2P_UnitPetSyncPack( CKTDNUDP::RecvData* pRecvData )
//{
//	KSerBuffer ksBuff;
//	ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//	KXPT_UNIT_PET_SYNC_PACK kXPT_UNIT_PET_SYNC_PACK;
//	DeSerialize( &ksBuff, &kXPT_UNIT_PET_SYNC_PACK );
//
//	for( UINT i = 0; i < kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList.size(); i++ )
//	{
//		KXPT_UNIT_PET_SYNC* pXPT_UNIT_PET_SYNC = &kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList[i];
//
//		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
//		{
//			CX2PET *pPet = g_pData->GetPetManager()->GetPet( pXPT_UNIT_PET_SYNC->unitUID );
//			if( pPet != NULL )
//			{
//				pPet->ReceiveFrameData( pXPT_UNIT_PET_SYNC );
//			}
//		}
//	}
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK




void CX2Game::P2PPacketHandler()
{
	KTDXPROFILE();

	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
		P2PPacketHandler( pRecvData );
		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();		
	}

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    if ( g_pX2Room != NULL )
        g_pX2Room->SetSkipPingSendCheck( false );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
}

bool CX2Game::P2PPacketHandler( CKTDNUDP::RecvData* pRecvData )
{
	KTDXPROFILE();

	if( NULL == pRecvData )
		return false;

	if( GS_PLAY != m_GameState )
		return false;

#ifdef UDP_DOWNLOAD_BLOCK_CHECK
	AbuserUserReport(); //������ �ǿ� ���� �Ű�
#endif //UDP_DOWNLOAD_BLOCK_CHECK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	switch( pRecvData->m_ID )
    {
    case XPT_PORT_CHECK_REQ:
        return true;
    case XPT_PORT_CHECK_ACK:
        return true;
	case XPT_PING_TEST_REQ:
		{
            if ( pRecvData->m_Size != sizeof(KXPT_PING_TEST_REQ) )
                return false;

            const KXPT_PING_TEST_REQ* pkReq = (const KXPT_PING_TEST_REQ*) pRecvData->m_pRecvBuffer;
			KXPT_PING_TEST_ACK  kKXPT_PING_TEST_ACK;
			if( GetMyUnit() != NULL )
				kKXPT_PING_TEST_ACK.m_UnitUID	=  GetMyUnit()->GetUnitUID();
			else
				kKXPT_PING_TEST_ACK.m_UnitUID	=  g_pData->GetMyUser()->GetSelectUnit()->GetUID(); 
			kKXPT_PING_TEST_ACK.m_SendTime	= pkReq->m_SendTime;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			kKXPT_PING_TEST_ACK.m_bRelay = pkReq->m_bRelay;
			kKXPT_PING_TEST_ACK.m_uiSendCounter	= pkReq->m_uiSendCounter;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            kKXPT_PING_TEST_ACK.m_bIgnoreAck = g_pX2Room != NULL && g_pX2Room->GetSkipPingSendCheck();
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            g_pData->GetGameUDP()->SendToPeer( pkReq->m_UnitUID, XPT_PING_TEST_ACK, (char*) &kKXPT_PING_TEST_ACK, sizeof(KXPT_PING_TEST_ACK) );
		} 
        return true;
    case XPT_PING_TEST_ACK:
        {
            if ( pRecvData->m_Size != sizeof(KXPT_PING_TEST_ACK) )
                return false;
            const KXPT_PING_TEST_ACK* pkAck = (const KXPT_PING_TEST_ACK*) pRecvData->m_pRecvBuffer;
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pkAck->m_UnitUID );
			if( pSlotData != NULL )
			{
                DWORD   dwCurrTime = timeGetTime();
                LONG    lTimeDiff = (LONG) ( dwCurrTime - pkAck->m_SendTime );
                DWORD   dwPingTime = ( lTimeDiff >= 0 ) ? lTimeDiff : 0;
                if ( dwPingTime > 5000 )
                    dwPingTime = 5000;
                bool    bValidPingTime = false;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                switch( pSlotData->GetSlotState() )
                {
                case CX2Room::SS_CLOSE:
                case CX2Room::SS_EMPTY:
                    break;
                default:
                    if ( pSlotData->m_uPingSendCount > 0 )
                    {
                        unsigned u = 0;
                        unsigned uMax = pSlotData->m_uPingSendCount;
                        unsigned uBase = 0;
                        if ( uMax > ARRAY_SIZE( pSlotData->m_adwPingSendTime ) )
                        {
                            uBase = uMax - ARRAY_SIZE( pSlotData->m_adwPingSendTime );
                            uMax = ARRAY_SIZE( pSlotData->m_adwPingSendTime );
                        }
                        for( ; u < uMax; u++ )
                        {
                            if ( pSlotData->m_adwPingSendTime[u] == pkAck->m_SendTime )
                                break;
                        }
                        if ( u < uMax )
                        {
                            for( unsigned v = u + 1; v < uMax; v++ )
                            {
                                pSlotData->m_adwPingSendTime[ v - 1 ] = pSlotData->m_adwPingSendTime[v];
                            }
                            if ( ( g_pX2Room == NULL || g_pX2Room->GetSkipPingSendCheck() == false ) && pkAck->m_bIgnoreAck == false )
                            {
                                lTimeDiff = (LONG) ( dwCurrTime - pkAck->m_SendTime );
                                pSlotData->m_PingTime = (lTimeDiff > 0 ) ? lTimeDiff * ( uBase + 1 ) : 0;
                                if ( pSlotData->m_PingTime > 5000 )
                                    pSlotData->m_PingTime = 5000;
                                bValidPingTime = true;
                            }
                            pSlotData->m_uPingSendCount -= ( uBase + 1 );
                        }
                    }
                    break;
                }//switch
#else   SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                bValidPingTime = true;
				pSlotData->m_PingTime = dwPingTime;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE			
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                if ( bValidPingTime == true )
                {
                    float   fInvWeight = 1.f/(float) ( pSlotData->m_dwPingCount + 1 );
                    float   fCutPingTime = __min(pSlotData->m_PingTime,2000) * 0.001f;
                    pSlotData->m_fAvgPingTime = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTime + fCutPingTime ) * fInvWeight;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                    pSlotData->m_fAvgPingTimeSqr = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTimeSqr + fCutPingTime * fCutPingTime ) * fInvWeight;
                    float fPingStd = pSlotData->m_fAvgPingTimeSqr - pSlotData->m_fAvgPingTime * pSlotData->m_fAvgPingTime;
                    pSlotData->m_fAvgPingStd = ( fPingStd <= 0.f ) ? 0.f : sqrtf( fPingStd );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                    if ( pSlotData->m_dwPingCount < 12 )
                        pSlotData->m_dwPingCount++;
                }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
				if( !pkAck->m_bRelay )
					KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->ReceivePingCheckPacket( dwPingTime, pkAck->m_uiSendCounter );
				else
					KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->ReceivePingCheckPacket( dwPingTime, pkAck->m_uiSendCounter );

				KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->ReceivePingCheckPacket( dwPingTime, pkAck->m_uiSendCounter );

				if( pkAck->m_UnitUID == m_uidHostSlotUID )
				{
					//��ȣ��Ʈ  NPC���׼� ������ PingTime�� �պ� �����̴�.
					KOGGamePerformanceCheck::GetInstance()->GetNonHostNpcReactionCheckMgr()->UpdateHostUdpPingTime( dwPingTime );
				}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			}
        }
        return true;
    case XPT_UDP_PACKET_PACK:
        {
            if ( pRecvData->m_Size < sizeof(XPT_UDP_PACKET_PACK_PREFIX) + sizeof(WORD) )
                return false;
        }
        break;

    default:
        return false;
    }//switch

    WORD    wPackSize = pRecvData->m_Size;
    const XPT_UDP_PACKET_PACK_PREFIX* pkPrefix = ((const XPT_UDP_PACKET_PACK_PREFIX*) &pRecvData->m_pRecvBuffer[0] );
    UidType pidUnitUID = pkPrefix->m_iUnitUID;
    DWORD   dwFrameMoveCount = pkPrefix->m_dwFrameMoveCount;
    BYTE    pbyUncompressBuffer[CKTDNUDP_MTU_MAX];
    WORD    wSize = 0;
    const BYTE* pbyData = NULL;
    WORD    wDataSize = 0;
    for( WORD wOffset = sizeof(XPT_UDP_PACKET_PACK_PREFIX); wOffset + sizeof(WORD) < wPackSize; wOffset += sizeof(WORD) + wSize)
    {
        wSize = *((const WORD*) &pRecvData->m_pRecvBuffer[wOffset] );
        bool    bCompressed = ( wSize & 0x8000 ) != 0;
        wSize &= 0x7fff;
        if ( wSize == 0 )
        {
            continue;
        }//if
        if ( bCompressed == true )
        {
            uLongf  uUncompressedSize = CKTDNUDP_MTU_MAX;
            if ( uncompress( &pbyUncompressBuffer[0], &uUncompressedSize, (const BYTE*) &pRecvData->m_pRecvBuffer[wOffset+sizeof(WORD)], wSize ) != Z_OK )
            {
                continue;
            }
            pbyData = pbyUncompressBuffer;
            wDataSize = (WORD) uUncompressedSize;
        }
        else
        {
            pbyData = (const BYTE*) &pRecvData->m_pRecvBuffer[wOffset + sizeof(WORD)];
            wDataSize = wSize;
        }
        for( WORD wDataOffset = 0; wDataOffset < wDataSize; )
        {
            BYTE byID = pbyData[ wDataOffset ];
            if ( byID <= XPT_UDP_PACKET_PACK )
                break;
            wDataOffset++;
            switch( byID )
            {
            case XPT_UNIT_NPC_MINISYNC_PACK:
                {
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                    ASSERT( wDataOffset + 1 <= wDataSize );
                    if ( wDataOffset + 1 > wDataSize )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    BYTE byNumPack = pbyData[ wDataOffset ];
                    wDataOffset++;
        			WORD wProcessed = Handler_P2P_UnitNpcMiniSyncPack( &pbyData[ wDataOffset ], byNumPack
                        , wDataSize - wDataOffset, ( GetHostSlotUID() == pidUnitUID ) ? pidUnitUID : 0, dwFrameMoveCount );
                    if ( wProcessed == 0 )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    wDataOffset += wProcessed;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
					wDataOffset = wDataSize;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
					break;
                }
                break;
            case XPT_UNIT_NPC_SYNC_PACK:
                {
                    ASSERT( wDataOffset + 1 <= wDataSize );
                    if ( wDataOffset + 1 > wDataSize )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    BYTE byNumPack = pbyData[ wDataOffset ];
                    wDataOffset++;
        			WORD wProcessed = Handler_P2P_UnitNpcSyncPack( &pbyData[ wDataOffset ], byNumPack
                        , wDataSize - wDataOffset,  ( GetHostSlotUID() == pidUnitUID ) ? pidUnitUID : 0, dwFrameMoveCount );
                    if ( wProcessed == 0 )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    wDataOffset += wProcessed;
		        }
                break;
#ifdef  SERV_PET_SYSTEM
            case XPT_UNIT_PET_SYNC_PACK:
                {
                    ASSERT( wDataOffset + 1 <= wDataSize );
                    if ( wDataOffset + 1 > wDataSize )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    BYTE byNumPack = pbyData[ wDataOffset ];
                    wDataOffset++;
                    WORD wProcessed = Handler_P2P_UnitPetSyncPack( &pbyData[ wDataOffset ], byNumPack
                        , wDataSize - wDataOffset );
                    if ( wProcessed == 0 )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    wDataOffset += wProcessed;
                }
                break;
#endif  SERV_PET_SYSTEM
            case XPT_UNIT_USER_SYNC_PACK:
                {
                    ASSERT( wDataOffset + 1 <= wDataSize );
                    if ( wDataOffset + 1 > wDataSize )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    BYTE byNumPack = pbyData[ wDataOffset ];
                    wDataOffset++;
        			WORD wProcessed = Handler_P2P_UnitUserSyncPack( &pbyData[ wDataOffset ], byNumPack
                        , wDataSize - wDataOffset, pidUnitUID, dwFrameMoveCount );
                    if ( wProcessed == 0 )
                    {
                        wDataOffset = wDataSize;
                        break;
                    }
                    wDataOffset += wProcessed;
                }
                break;
            default:
                wDataOffset = wDataSize;

                break;
            }
        }

    }//for
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	switch( pRecvData->m_ID )
//	{
//	case XPT_UNIT_USER_SYNC_PACK:
//		{
//			KSerBuffer ksBuff;
//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//			KXPT_UNIT_USER_SYNC_PACK kXPT_UNIT_USER_SYNC_PACK;
//			DeSerialize( &ksBuff, &kXPT_UNIT_USER_SYNC_PACK );
//			CX2GUUser* pUserUnit = GetUserUnitByUID( kXPT_UNIT_USER_SYNC_PACK.m_iUnitUID );
//
//			if( pUserUnit != NULL )
//			{
//				//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
//				if ( GetGameType() == GT_PVP )
//					AbuserUserCheck( pUserUnit );
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//				//}}
//
//#ifdef UDP_CAN_NOT_SEND_USER_KICK
//				AbuserUserCheck( kXPT_UNIT_USER_SYNC_PACK.m_iUnitUID ); //������ �ǿ� ���� üũ
//#endif //UDP_CAN_NOT_SEND_USER_KICK
//
//#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
//				IncreaseUserPacketCountToReceive( kXPT_UNIT_USER_SYNC_PACK.m_iUnitUID );
//#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM
//
//				// �˻� �� ���� ó��
//				pUserUnit->ReceiveFrameData( &kXPT_UNIT_USER_SYNC_PACK );
//			}
//		} break;
//
//#ifdef UNIT_SYNC_PACKET_DUNGEON_FOR_TEST
//		//case XPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON:
//		//	{
//		//		KSerBuffer ksBuff;
//		//		ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//		//		KXPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON kXPT_UNIT_USER_SYNC_PACK;
//		//		DeSerialize( &ksBuff, &kXPT_UNIT_USER_SYNC_PACK );
//		//		CX2GUUser* pUserUnit = GetUserUnitByUID( kXPT_UNIT_USER_SYNC_PACK.unitUID );
//		//		if( pUserUnit != NULL )
//		//			pUserUnit->ReceiveFrameData( &kXPT_UNIT_USER_SYNC_PACK );
//		//	} break;
//#endif UNIT_SYNC_PACKET_DUNGEON_FOR_TEST
//
//	case XPT_UNIT_NPC_FIRST_SYNC_PACK_BY_BATTLE_FIELD:
//		{
//			if ( false == IsHost() )
//			{
//				Handler_P2P_UnitNpcSyncPack( pRecvData, true );
//
//				g_pData->GetServerProtocol()->SendID( EGS_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT );
//			}
//		} break;
//
//	case XPT_UNIT_NPC_SYNC_PACK:
//		{
//			Handler_P2P_UnitNpcSyncPack( pRecvData );
//		} break;
//
//
//	case XPT_UNIT_NPC_SYNC:
//		{
//			KSerBuffer ksBuff;
//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//			KXPT_UNIT_NPC_SYNC kXPT_UNIT_NPC_SYNC;
//			DeSerialize( &ksBuff, &kXPT_UNIT_NPC_SYNC );
//
//			CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( kXPT_UNIT_NPC_SYNC.unitUID );
//			if( pCX2GUNPC != NULL )
//				pCX2GUNPC->ReceiveFrameData( &kXPT_UNIT_NPC_SYNC );						
//		} break;
//
//	case XPT_PING_TEST_REQ:
//		{
//			KSerBuffer ksBuff;
//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//			KXPT_PING_TEST_REQ kKXPT_PING_TEST_REQ;
//			DeSerialize( &ksBuff, &kKXPT_PING_TEST_REQ );
//
//			KXPT_PING_TEST_ACK kKXPT_PING_TEST_ACK;
//			if( GetMyUnit() != NULL )
//				kKXPT_PING_TEST_ACK.m_UnitUID	=  GetMyUnit()->GetUnitUID();
//			else
//			{
//				kKXPT_PING_TEST_ACK.m_UnitUID	=  g_pData->GetMyUser()->GetSelectUnit()->GetUID(); 
//			}
//			kKXPT_PING_TEST_ACK.m_SendTime	=  kKXPT_PING_TEST_REQ.m_SendTime;
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//			kKXPT_PING_TEST_ACK.m_bRelay = kKXPT_PING_TEST_REQ.m_bRelay;
//			kKXPT_PING_TEST_ACK.m_uiSendCounter	=  kKXPT_PING_TEST_REQ.m_uiSendCounter;
//#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//           kKXPT_PING_TEST_ACK.m_bIgnoreAck = g_pX2Room != NULL && g_pX2Room->GetSkipPingSendCheck();
//#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//
//			KSerBuffer buff;
//			Serialize( &buff, &kKXPT_PING_TEST_ACK );
//			g_pData->GetGameUDP()->Send( kKXPT_PING_TEST_REQ.m_UnitUID, XPT_PING_TEST_ACK, (char*)buff.GetData(), buff.GetLength() );
//		} break;
//
//	case XPT_PING_TEST_ACK:
//		{
//			KSerBuffer ksBuff;
//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//			KXPT_PING_TEST_ACK kKXPT_PING_TEST_ACK;
//			DeSerialize( &ksBuff, &kKXPT_PING_TEST_ACK );
//
//			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kKXPT_PING_TEST_ACK.m_UnitUID );
//			if( pSlotData != NULL )
//			{
//              LONG    lTimeDiff = (LONG) ( timeGetTime() - kKXPT_PING_TEST_ACK.m_SendTime );
//               DWORD   dwPingTime = ( lTimeDiff >= 0 ) ? lTimeDiff : 0;
//                if ( dwPingTime > 5000 )
//                    dwPingTime = 5000;
//				pSlotData->m_PingTime = dwPingTime;
//#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//                float   fInvWeight = 1.f/(float) ( pSlotData->m_dwPingCount + 1 );
//               float   fCutPingTime = __min(pSlotData->m_PingTime,2000) * 0.001f;
//                pSlotData->m_fAvgPingTime = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTime + fCutPingTime ) * fInvWeight;
//#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//                pSlotData->m_fAvgPingTimeSqr = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTimeSqr + fCutPingTime * fCutPingTime ) * fInvWeight;
//                float fPingStd = pSlotData->m_fAvgPingTimeSqr - pSlotData->m_fAvgPingTime * pSlotData->m_fAvgPingTime;
//                pSlotData->m_fAvgPingStd = ( fPingStd <= 0.f ) ? 0.f : sqrtf( fPingStd );
//#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//                if ( pSlotData->m_dwPingCount < 12 )
//                    pSlotData->m_dwPingCount++;
//#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//				if( !kKXPT_PING_TEST_ACK.m_bRelay )
//					KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->ReceivePingCheckPacket( dwPingTime, kKXPT_PING_TEST_ACK.m_uiSendCounter );
//				else
//					KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->ReceivePingCheckPacket( dwPingTime, kKXPT_PING_TEST_ACK.m_uiSendCounter );
//
//				KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->ReceivePingCheckPacket( dwPingTime, kKXPT_PING_TEST_ACK->m_uiSendCounter );
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//			}
//		} break;
//
//
//#ifdef SERV_PET_SYSTEM
//	case XPT_UNIT_PET_SYNC:
//		{
//			KSerBuffer ksBuff;
//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//			KXPT_UNIT_PET_SYNC kXPT_UNIT_PET_SYNC;
//			DeSerialize( &ksBuff, &kXPT_UNIT_PET_SYNC );
//
//			if( g_pData != NULL && g_pData->GetPetManager() != NULL )
//			{
//				CX2PET *pPet = g_pData->GetPetManager()->GetPet( kXPT_UNIT_PET_SYNC.unitUID );
//				if( pPet != NULL )
//				{
//					pPet->ReceiveFrameData( &kXPT_UNIT_PET_SYNC );
//				}
//			}				
//		} break;
//
//	case XPT_UNIT_PET_SYNC_PACK:
//		{
//			if ( false == IsHost() )
//				Handler_P2P_UnitPetSyncPack( pRecvData );			
//		} break;
//
//	case XPT_UNIT_PET_FIRST_SYNC_PACK_BY_BATTLE_FIELD:
//		{
//			Handler_P2P_UnitPetSyncPack( pRecvData );
//		} break;
//#endif
//	default:
//		break;
//
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	return true;
}



void CX2Game::GameLoading( CX2Room* pRoom )
{
	m_pRoom = pRoom;


	HRESULT hr;
	if ( FAILED ( hr = g_pKTDXApp->GetDevice()->EvictManagedResources() ) )
	{
		switch( hr )
		{
		case D3DERR_OUTOFVIDEOMEMORY:
			ErrorLogMsg( XEM_ERROR3, "D3DERR_OUTOFVIDEOMEMORY" );
			break;

		default:
			ErrorLogMsg( XEM_ERROR3, "D3DERR_UNKNOWN" );
			break;
		}
	}

	SystemLoading();
	WorldLoading();
	UnitLoading();
	EtcLoading();	



	if( true == IsHost() )
	{
		StateLog( L"i'm game HOST" );
	}

	wstringstream wstrmText;
	if( NULL != m_pRoom )
		wstrmText << L"Room User: " << m_pRoom->GetUserNum() << std::endl;
	wstrmText << L"Game User: " << GetUserUnitNum() << std::endl;
	StateLog( wstrmText.str().c_str() );
}

void CX2Game::SystemLoading()
{
	m_GameState = GS_LOADING;

	srand((unsigned int)time(NULL));

 	/*m_pMajorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
 	m_pMinorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
 	m_pMajorXMeshPlayer	= new CKTDGXMeshPlayer();
 	m_pMinorXMeshPlayer	= new CKTDGXMeshPlayer();
 
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle2.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle3.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle4.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle5.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle6.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle7.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle8.txt" );
 	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle9.txt" );
 
 	m_pMinorParticle->OpenScriptFile( L"GameMinorParticle.txt" );
 
	m_pMajorXMeshPlayer->OpenScriptFile( L"GameMajorXMeshPlayer.txt" );
	m_pMinorXMeshPlayer->OpenScriptFile( L"GameMinorXMeshPlayer.txt" );*/

#ifdef REALTIME_SCRIPT_PATCH	
	switch( g_pMain->GetNowStateID() ) 
	{
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_DUNGEON_GAME:
		{
			//���� ��ƼŬ �ý��� ����
			g_pData->ResetGameMajorParticle( L"GameMajorParticle.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle2.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle3.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle4.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle5.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle6.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle7.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle8.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle9.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle10.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle11.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle12.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle13.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle14.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle15.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle16.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle17.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle18.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle19.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle20.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle21.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle22.txt" );
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle23.txt" );

			g_pData->ResetGameMinorParticle( L"GameMinorParticle.txt" );
#ifdef INT_RESOURCE_DEVIDE
			g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle_INT.txt" );
			g_pData->GetGameMinorParticle()->OpenScriptFile( L"GameMinorParticle_INT.txt" );
#endif INT_RESOURCE_DEVIDE
		} break;
	default:
		break;
	}
#endif //REALTIME_SCRIPT_PATCH

	m_pDamageEffect		= new CX2DamageEffect();
	m_pDamageEffect->OpenScriptFile( L"DamageEffect.lua" );
#ifdef INT_RESOURCE_DEVIDE
	m_pDamageEffect->OpenScriptFile( L"DamageEffect_INT.lua" );
#endif INT_RESOURCE_DEVIDE
// 

// 
// 	m_EffectSet.OpenScriptFile( L"EffectSet.lua" );


	if( NULL != g_pData->GetPicChar() ) 
		g_pData->GetPicChar()->Clear();
	if( NULL != g_pData->GetPicCharRed() )
		g_pData->GetPicCharRed()->Clear();
	if( NULL != g_pData->GetPicCharBlue() )
		g_pData->GetPicCharBlue()->Clear();
	if( NULL != g_pData->GetPicCharGreen() )
		g_pData->GetPicCharGreen()->Clear();
	if( NULL != g_pData->GetPicCharYellow() )
		g_pData->GetPicCharYellow()->Clear();
	if( NULL != g_pData->GetPicCharGameScore() )
		g_pData->GetPicCharGameScore()->Clear();
	if( NULL != g_pData->GetPicCharTechPoint() )
		g_pData->GetPicCharTechPoint()->Clear();


	m_hDangerAlarm = GetMajorParticle()->CreateSequenceHandle( NULL, L"danger_alarm_01", 0.0f, 0.0f, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pSeq = GetMajorParticle()->GetInstanceSequence( m_hDangerAlarm );
	if( pSeq != NULL && pSeq->GetShowObject() == true )
	{
		pSeq->SetShowObject(false);
	}

	//P2PSetPeer();
	//P2PConnectTest();
}

void CX2Game::UnitLoading()
{
	//{{ kimhc // 2011-07-28 // �ɼǼ�ġȭ
	// ���⿡�� �ʿ� ���µ�...
	if ( true == CX2Dungeon::IsDamageFreeGame() ) 
		SetIsDamageFreeGame( true ); // 1���� ���� �����ΰ�?
	//}} kimhc // 2011-07-28 // �ɼǼ�ġȭ

	AddUserUnit();
	UpdateUnitPointer();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2UserUnit = m_UserUnitList[i];

		if( pCX2UserUnit != NULL )
		{
#ifdef ADDED_RELATIONSHIP_SYSTEM
			// ���� ��
			if ( NULL != g_pData->GetRelationshipManager() &&
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
				 NULL != g_pData->GetRelationshipEffectManager() )
			{
				CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
				CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = pRelationshipManager->GetMyRelationshipInfo();

				if ( pCX2UserUnit->GetRelationshipUnitUID() > 0 )
				{				
					CX2RelationshipEffectManager::RelationEffectInfo relationEffectInfo;
					relationEffectInfo = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 1 ) ) );				
					relationEffectInfo.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pCX2UserUnit->GetUnit()->GetUID() );						
				}

				if ( pRelationshipInfo->m_iRelationTargetUserUid == pCX2UserUnit->GetUnit()->GetUID() )
				{
					pRelationshipManager->SetRelationPartnerInGame ( CX2GameUnitoPtr( pCX2UserUnit ) );
				}
			}
#endif // ADDED_RELATIONSHIP_SYSTEM
			pCX2UserUnit->PreRenderForce();
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if ( NULL != GetMyUnit() )
		m_optrFocusUnit	= GetMyUnit();
}

void CX2Game::EtcLoading()
{
	SAFE_DELETE( m_pCamera );
	m_pCamera		= new CX2Camera();
	if( m_pWorld != NULL )
		m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );

	// FieldFix: �̺κ� �������̵� ������ ���� ����
	switch ( GetGameType() )
	{
	case CX2Game::GT_BATTLE_FIELD:
		m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), 1800.0f );
		break;

	default:
		m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), g_pMain->GetGameOption().GetCameraDistance() );
		break;
	}

	SAFE_DELETE(m_pDropItemManager);
	m_pDropItemManager	= new CX2DropItemManager();
	//m_pDropItemManager->OpenScriptFile( L"DropItemManager.lua" );

	TextureReady( L"HQ_clear_c.dds" );
	TextureReady( L"HQ_clear_l.dds" );
	TextureReady( L"HQ_clear_e.dds" );
	TextureReady( L"HQ_clear_a.dds" );
	TextureReady( L"HQ_clear_r.dds" );

	TextureReady( L"MagicSquare_2.dds" );
	TextureReady( L"Blocking_Net2.dds" );
	

	if( NULL == GetMyUnit() )
		return;

	CX2GUUser* pMyGUUser = GetMyUnit();
	if( NULL != pMyGUUser )
	{
		pMyGUUser->UpdateSocketDataAndEnchantData();
		pMyGUUser->UpdatePassiveAndActiveSkillState();

		UpdateSkillSlotUI();
	}
	
// #ifdef NEW_SKILL_TREE
// 	if( NULL != GetMyUnit() )
// 	{
// 		GetMyUnit()->GetGageManager()->UpdateMyManaGuageUI();
// 	}
// #endif

	D3DXVECTOR3 vEye = m_pCamera->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();
	m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
	m_FPSCamera.SetRotateButtons( false, false, true, false );

#ifdef DUNGEON_ITEM
	SoundReady( L"Nasod_Healer_Heal.ogg" );
	SoundReady( L"Item_Take.ogg" );
	SoundReady( L"Mark_Fail.ogg" );
	SoundReady( L"Mark_Success.ogg" );
	SoundReady( L"Medicine_Fail.ogg" );
	SoundReady( L"Medicine_Sprinter.ogg" );
	SoundReady( L"Summon_Dryad_Hit.wav" );
	SoundReady( L"Summon_Fail.ogg" );
	SoundReady( L"Summon_Oread_Hit.wav" );

	TextureReady( L"Summon_Dryad01.tga" );
	TextureReady( L"Summon_Elifea01.tga" );
	TextureReady( L"Summon_Oread01.tga" );
	TextureReady( L"Summon_Raul01.tga" );
	TextureReady( L"Moon_MagicSquare01.dds" );
	TextureReady( L"Marker_Sprinter_Posion01.dds" );
	TextureReady( L"Marker_Antimagic_Symbol01.dds" );
	TextureReady( L"Marker_Brave_Symbol01.dds" );
	TextureReady( L"Marker_Giant_Potion01.dds" );
	TextureReady( L"Marker_Hyper_Potion01.dds" );
	TextureReady( L"Marker_Inviolable_Symbol01.dds" );
	TextureReady( L"Marker_Magic_Symbol01.dds" );
	TextureReady( L"Marker_Pigmy_Potion01.dds" );
	TextureReady( L"Marker_Protect_Symbol01.dds" );
	TextureReady( L"Marker_Side_Effect_Potion01.dds" );

	XMeshReady( L"Blur_Light_Mesh01.Y" );
	XMeshReady( L"Explosion_Light_Mesh01.Y" );

	XSkinMeshReady( L"Motion_WilliamPPoru.X" );
	XSkinMeshReady( L"Summon_Dryad_Effect_Vine.X" );
	XSkinMeshReady( L"Summon_Dryad_Effect_Vine_Bottom.X" );		
	//XSkinMeshReady( L"Summon_Oread_Effect_Bottom.X" );
	XSkinMeshReady( L"Summon_Oread_Effect_Rock.X" );
	XSkinMeshReady( L"Antimagic_Symbol_Motion01.X" );
	XSkinMeshReady( L"Brave_Symbol_Motion01.X" );
	XSkinMeshReady( L"Magic_Symbol_Motion01.X" );
	XSkinMeshReady( L"Protect_Symbol_Motion01.X" );
	XSkinMeshReady( L"Wrong_Symbol_Motion01.X" );
#endif

#ifdef EVENT_DUNGEON_ITEM
	XSkinMeshReady( L"DEVELOPER_A.x" );
	XSkinMeshReady( L"DEVELOPER_B.X" );
	XSkinMeshReady( L"DEVELOPER_C.X" );
	XSkinMeshReady( L"DEVELOPER_D.X" );
	XSkinMeshReady( L"DEVELOPER_E.X" );
#endif
}

void CX2Game::AddUserUnit()
{
	for( int i = 0; i <  m_pRoom->GetSlotNum(); i++ )
	{
		CX2Room::SlotData* pSlotData = m_pRoom->GetSlotData( i );

		if( pSlotData == NULL 
			|| pSlotData->m_SlotState == CX2Room::SS_CLOSE
			|| pSlotData->m_SlotState == CX2Room::SS_EMPTY
			|| pSlotData->m_SlotState == CX2Room::SS_WAIT )
			continue;

#ifdef SERV_PVP_NEW_SYSTEM
		if( pSlotData->m_bNpc == false )
		{
			AddUserUnit( pSlotData, i );
		}
#else
		AddUserUnit( pSlotData, i );
#endif
	}

	if ( GT_PVP == GetGameType() )
	{
		for( int i = 0; i <  m_pRoom->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = m_pRoom->GetSlotData( i );

			if( pSlotData == NULL 
				|| pSlotData->m_SlotState == CX2Room::SS_CLOSE
				|| pSlotData->m_SlotState == CX2Room::SS_EMPTY
				|| pSlotData->m_SlotState == CX2Room::SS_WAIT )
				continue;

			if ( false == pSlotData->IsMyUnit() )
			{
				CX2GageManager* pGageManager = CX2GageManager::GetInstance();

				if ( NULL != pGageManager )
				{
					CX2GUUser* pGameUnitMember = GetUserUnitByUID( pSlotData->m_UnitUID );

					pGageManager->InsertPvpMemberUI( *pSlotData, pGameUnitMember );
					pGageManager->UpdatePvpMemberGageData( pSlotData->m_UnitUID, pGameUnitMember );
				}
			}
		}
	}
}

/*virtual*/ void CX2Game::AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex )
{
	// note!! ĳ���� �߰��Ǹ� ����
	CX2GUUser* pCX2UserUnit = CreateGUUser( pSlotData, slotIndex );
	
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	DeleteUserUnit( slotIndex );
	m_UnitList.push_back( (CX2GameUnit*)pCX2UserUnit );
	m_UserUnitList[slotIndex] = pCX2UserUnit;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( pSlotData->m_RelationshipTargetUID > 0 )
	{
		pCX2UserUnit->SetRelationshipUnitUID( pSlotData->m_RelationshipTargetUID );						
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

	//UpdateUnitPointer();
	if ( pSlotData->IsMyUnit() )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        UpdateMyGameUnit( pSlotData, pCX2UserUnit );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_optrMyUnit = pCX2UserUnit;
//		CheckAmIHost();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

	CX2GageManager::GetInstance()->SetPartyMemberGameUnit( pCX2UserUnit );

	pCX2UserUnit->Init();	
	if ( pCX2UserUnit->IsMyUnit() )
		pCX2UserUnit->SetKBuffFactorFromGageManagerToGameUnit();

	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2UserUnit );
	

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME &&
		true == pCX2UserUnit->IsMyUnit() ) // ������ // 2013-08-22 // ��Ÿ���� �ڱ� ĳ������ ���� �����ϵ��� ����
	{
		pCX2UserUnit->ResetLeftSkillCoolTimeBySkillType( CX2SkillTree::ST_HYPER_ACTIVE_SKILL, 180.f, true, BCT_FIX_VALUE );
	}
#endif //FINALITY_SKILL_SYSTEM

}

CX2GUUser* CX2Game::AddUserUnitIntruder( CX2Room::SlotData* pSlotData, int slotIndex )
{
	// note!! ���� �߰��Ǹ� ����, Ȥ�� unit_type����
	CX2GUUser* pCX2UserUnit = NULL;

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const CX2UnitManager::UnitTypeLuaTemplet& kUnitTypeLuaTemplet = 
		g_pData->GetUnitManager()->GetUnitTypeLuaTemplet( pSlotData->m_pUnit->GetType() );
	lua_State*	script_info = ( kUnitTypeLuaTemplet.m_pLuaManager != NULL ) ? kUnitTypeLuaTemplet.m_pLuaManager->GetLuaState() : NULL;
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const CX2UnitManager::UnitTypeTemplet* pUnitTypeTemplet = g_pData->GetUnitManager()->GetUnitTypeTemplet( pSlotData->m_pUnit->GetType() );
	const WCHAR* script_info = ( pUnitTypeTemplet != NULL ) ? pUnitTypeTemplet->m_wstrLuaScriptFile.c_str() : L"";
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	switch( pSlotData->m_pUnit->GetType() )
	{
	case CX2Unit::UT_ELSWORD:
		{
			pCX2UserUnit = CX2GUElsword_SwordMan::CreateGUElsword( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData->m_pUnit );
		}
		break;

	case CX2Unit::UT_ARME:
		{
			pCX2UserUnit = CX2GUArme_VioletMage::CreateGUArme( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData->m_pUnit );
		}
		break;

	case CX2Unit::UT_LIRE:
		{
			pCX2UserUnit = CX2GULire_ElvenRanger::CreateGULire( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData->m_pUnit );
		}
		break;

	case CX2Unit::UT_RAVEN:
		{
			pCX2UserUnit = CX2GURaven::CreateGURaven( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData->m_pUnit );
		}
		break;


	case CX2Unit::UT_EVE:
		{
			pCX2UserUnit = CX2GUEve::CreateGUEve( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData->m_pUnit );
		}
		break;

		//{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		{
			pCX2UserUnit = CX2GUChung::CreateGUChung( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData->m_pUnit );
		}
		break;
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		{
			pCX2UserUnit = CX2GUAra::CreateGUAra( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(),
												  pSlotData->m_pUnit );
		}
		break;
#endif
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS:
		{
			pCX2UserUnit = CX2GUEL::CreateGUEL( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(),
				pSlotData->m_pUnit );
		}
		break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CX2Unit::UT_ADD:
		{
			pCX2UserUnit = CX2GUAdd::CreateGUAdd( slotIndex, pSlotData->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(),
				pSlotData->m_pUnit );
		}
		break;
#endif //SERV_9TH_NEW_CHARACTER
	}


#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
	m_IntruderList.push_back( pCX2UserUnit );
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pCX2UserUnit->Init();
	if ( pCX2UserUnit->IsMyUnit() )
		pCX2UserUnit->SetKBuffFactorFromGageManagerToGameUnit();
	//{{ robobeg : 2008-10-21
	//pCX2UserUnit->SetShow( false );
	pCX2UserUnit->SetShowObject( false );
	//}} robobeg : 2008-10-21


#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( pSlotData->m_RelationshipTargetUID > 0 )
	{
		pCX2UserUnit->SetRelationshipUnitUID ( pSlotData->m_RelationshipTargetUID );
	}

	if ( NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
		NULL != g_pData->GetRelationshipEffectManager() )
	{
		CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
		CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = pRelationshipManager->GetMyRelationshipInfo();

		if ( pRelationshipInfo->m_eRelationStateType >	SEnum::RT_SOLO )
		{
			if ( pRelationshipInfo->m_iRelationTargetUserUid == pCX2UserUnit->GetUnit()->GetUID() )
			{
				pRelationshipManager->SetRelationPartnerInGame ( CX2GameUnitoPtr( pCX2UserUnit ) );
			}
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

	return pCX2UserUnit;
}

void CX2Game::AddUserUnitIntruderByLoader( CX2Unit* pUnit )
{

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	bool	bLoadPet = true;
	if( g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY
		|| g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM
		|| g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME
		|| g_pMain->GetNowStateID() == CX2Main::XS_PVP_RESULT
		|| g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME
		|| g_pMain->GetNowStateID() == CX2Main::XS_WEDDING_GAME
		 )
	{
		bLoadPet = false;
	}

	m_UnitLoader.AddUnitLoadDataForGame( pUnit, bLoadPet );

#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	m_UnitLoader.AddUnitLoadDataForGame( pUnit );

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

void	CX2Game::IntruderStart( CX2GUUser* pIntruder, int startPosIndex )
{
	if ( pIntruder == NULL )
		return;

	DeleteUserUnit( pIntruder->GetUnitIndex() );

	m_UnitList.push_back( (CX2GameUnit*)pIntruder );
	m_UserUnitList[pIntruder->GetUnitIndex()] = pIntruder;
	//{{ robobeg : 2008-10-21
	//pIntruder->SetShow( true );
	pIntruder->SetShowObject( true );
	//}} robobeg : 2008-10-21

	//{{ dmlee 2008.11.8 SetShowObject�ϸ鼭 ��� ���ŵǸ鼭, ���ùڽ�(��, ��)�� �ִ� �͵� ������. ���ߵ�. 
	pIntruder->SetEnableAttackBox( L"Rfoot", false );
	pIntruder->SetEnableAttackBox( L"Lfoot", false );
	pIntruder->SetEnableAttackBox( L"Lhand2", false );
	//}} dmlee 2008.11.8 SetShowObject�ϸ鼭 ��� ���ŵǸ鼭, ���ùڽ�(��, ��)�� �ִ� �͵� ������. ���ߵ�. 


	UpdateUnitPointer();
	if( startPosIndex != -1 )
    {
		pIntruder->InitPosition( false, startPosIndex );
    }

	pIntruder->StateChange( pIntruder->GetStartIntrudeStateID(), true );
	g_pKTDXApp->GetDGManager()->AddObjectChain( pIntruder );
	pIntruder->SetForceInvincible( 5.0f );
	pIntruder->SetShowInvincible( 5.0f );

	m_UnitLoader.DeleteUnitLoadData( pIntruder->GetUnitUID() );



#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		for(UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i)
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
			if( pPet != NULL )
			{
				pPet->InitGamePet();
			}
		}

	}
#endif
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

#ifdef ADDED_RELATIONSHIP_SYSTEM
	
	if ( pIntruder->GetRelationshipUnitUID() > 0 )
	{
		if ( NULL != g_pData->GetRelationshipEffectManager() )
		{
			CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_;
			RelationEffectInfo_ = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 1 ) ) );				
			RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pIntruder->GetUnit()->GetUID() );
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM
}
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
void CX2Game::IntruderStart( UidType unitUID, int startPosIndex )
{

	CSLock locker( m_csGameIntruder );

	for( int i = 0; i < (int)m_IntruderList.size(); i++ )
	{
		CX2GUUser* pIntruder = m_IntruderList[i];
		if( pIntruder->GetUnitUID() == unitUID )
		{
			m_IntruderList.erase( m_IntruderList.begin() + i );

			DeleteUserUnit( pIntruder->GetUnitIndex() );

			m_UnitList.push_back( (CX2GameUnit*)pIntruder );
			m_UserUnitList[pIntruder->GetUnitIndex()] = pIntruder;
			//{{ robobeg : 2008-10-21
			//pIntruder->SetShow( true );
			pIntruder->SetShowObject( true );
			//}} robobeg : 2008-10-21


			//{{ dmlee 2008.11.8 SetShowObject�ϸ鼭 ��� ���ŵǸ鼭, ���ùڽ�(��, ��)�� �ִ� �͵� ������. ���ߵ�. 
			pIntruder->SetEnableAttackBox( L"Rfoot", false );
			pIntruder->SetEnableAttackBox( L"Lfoot", false );
			pIntruder->SetEnableAttackBox( L"Lhand2", false );
			//}} dmlee 2008.11.8 SetShowObject�ϸ鼭 ��� ���ŵǸ鼭, ���ùڽ�(��, ��)�� �ִ� �͵� ������. ���ߵ�. 


			UpdateUnitPointer();
			if( startPosIndex != -1 )
            {
				pIntruder->InitPosition( false, startPosIndex );
            }

			pIntruder->StateChange( pIntruder->GetStartIntrudeStateID(), true );
			g_pKTDXApp->GetDGManager()->AddObjectChain( pIntruder );
			pIntruder->SetForceInvincible( 5.0f );
			pIntruder->SetShowInvincible( 5.0f );

			m_UnitLoader.DeleteUnitLoadData( pIntruder->GetUnitUID() );
			break;
		}
	}

#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		for(UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i)
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
			if( pPet != NULL )
			{
				pPet->InitGamePet();
			}
		}

	}
#endif
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void    CX2Game::UpdateMyGameUnit( const CX2Room::SlotData* pSlotData_, CX2GUUser* pUser )
{
    if ( pSlotData_ == NULL || pSlotData_->IsMyUnit() == false || pUser == NULL )
        return;
    m_uidMySlotUID = pSlotData_->m_UnitUID;
    m_optrMyUnit = pUser;
    if ( pSlotData_->m_bHost == true )
    {
        m_uidHostSlotUID = pSlotData_->m_UnitUID;
        SetHostGameUnit( pUser );
    }
    else
    {
        UpdateHostGameUnit();
    }
}

void    CX2Game::UpdateHostGameUnit()
{
	if( m_pRoom == NULL )
    {
        m_uidHostSlotUID = 0;
        ResetHostGameUnit();
    }
    else
    {
        CX2Room::SlotData* pHostSlot = m_pRoom->GetHostSlot();
        if ( pHostSlot != NULL )
        {
            m_uidHostSlotUID = pHostSlot->m_UnitUID;
            CX2GUUser* pHostGUUser = GetUserUnitByUID( m_uidHostSlotUID );
            if ( pHostGUUser != NULL )
                SetHostGameUnit( pHostGUUser );
            else
                ResetHostGameUnit();
        }
        else
        {
            m_uidHostSlotUID = 0;
            ResetHostGameUnit();
        }
    }
}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//void CX2Game::CheckAmIHost()
//{
//	if( m_pRoom == NULL )
//		return;
//
//
//	if( m_pRoom->GetMySlot() == NULL )
//		return;
//
//	// Unit�� UID�� �ƴ� User ������ UID �̴�.
//	
//	m_bHost = m_pRoom->GetMySlot()->GetIsHost();
//
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

/*virtual*/ void CX2Game::UpdateUnitPointer()
{
	KTDXPROFILE();

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    UpdateHostGameUnit();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	CheckAmIHost();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

// 	::EnterCriticalSection( &m_csGameIntruder );
// 	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
// 	{
// 		CX2GUUser* pCX2UserUnit = m_UserUnitList[i];
// 
// 		if ( pCX2UserUnit == NULL )
// 		{
// 			continue;
// 		}
// 
// 		if( pCX2UserUnit->GetUnitUID() == m_pRoom->GetMySlot()->m_pUnit->GetUID() )
// 			m_pMyUnit = pCX2UserUnit;
// 
// 	}
// 	::LeaveCriticalSection( &m_csGameIntruder );

	if( m_fFocusTime > 0.0f )
	{
		m_fFocusTime -= m_fElapsedTime;
		if( m_fFocusTime <= 0.0f )
			m_fFocusTime = 0.0f;
		
		if ( NULL != GetMyUnit() )
			m_optrFocusUnit = GetMyUnit();
	}

	ResetFocusUnit();
}

void CX2Game::GameStart()
{
	if( m_bBGMOn == true )
	{
		ASSERT( NULL != m_pWorld );
		if( NULL != m_pWorld )
		{
			m_pWorld->PlayBGM();
		}
#ifdef SERVICE_HEAVY_LOG
		else
		{
			StateLog( L"gamestart null world" );
		}
#endif SERVICE_HEAVY_LOG
	}

	g_pData->GetGameUDP()->ClearRecvBuffer();
	m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), g_pMain->GetGameOption().GetCameraDistance() );

	m_GameState = GS_PLAY;

	if( g_pChatBox != NULL )
	{
		wstring wstrColor = L"#CFFFF00";				
		D3DXCOLOR coTextColor(1.f, 1.f, 0.f, 1.f);	
		g_pChatBox->AddChatLog( GET_STRING( STR_ID_188 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
		g_pChatBox->AddChatLog( GET_STRING( STR_ID_22137 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
		g_pChatBox->AddChatLog( GET_STRING( STR_ID_22138 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
	}

	m_fElapsedTimeAfterLastKeyboardInput = 0.f;

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	m_pRoom->SetCheckUDPGameCountStart(true);
#endif//UDP_CAN_NOT_SEND_USER_KICK
}

void CX2Game::GameEnd()
{
	m_GameState = GS_END;

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	m_pRoom->SetCheckUDPGameCountStart(false);
#endif//UDP_CAN_NOT_SEND_USER_KICK
}


#ifdef RESET_INVALID_UNIT_POSITION_TEST

	bool CX2Game::CheckMisplacedUnit()
	{
		KTDXPROFILE();

		m_setMisplacedUser.clear();
		m_setMisplacedNPC.clear();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		if ( NULL == GetMyUnit() )
			return false;

		CX2GUUser* pMyGUUser = GetMyUnit();
		if( NULL != pMyGUUser )
		{
			if ( pMyGUUser->IsMisplaced() )
			{
				m_setMisplacedUser.insert( pMyGUUser->GetUnitUID() );
			}
		}

		if( true == IsHost() )
		{
			BOOST_TEST_FOREACH( CX2GUNPC*, pNPC, m_NPCUnitList )
			{
				if( NULL == pNPC )
					continue;

				if( true == pNPC->IsMisplaced() )
				{
					m_setMisplacedNPC.insert( pNPC->GetUnitUID() );
				}
			}
		}

		if( true == m_setMisplacedUser.empty() &&
			true == m_setMisplacedNPC.empty() )
		{
			return false;
		}
		else
		{
			return true;
		}
	}


	void CX2Game::ReplaceMisplacedUnit()
	{
		KTDXPROFILE();

		const CKTDGLineMap::LineData* pLineData = GetLineMap()->GetAnyEnabledNormalLine();
		ASSERT( NULL != pLineData );
		if( NULL == pLineData )
			return; 


#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		
		BOOST_TEST_FOREACH( UidType, userUID, m_setMisplacedUser )
		{
			CX2GUUser* pUser = GetUserUnitByUID( userUID );
			if( NULL == pUser )
				continue;

			pUser->SetPositionOnLine( pLineData->startPos, pLineData->lineIndex );
		}
		m_setMisplacedUser.clear();
		
		BOOST_TEST_FOREACH( UidType, npcUID, m_setMisplacedNPC )
		{
			CX2GUNPC* pNPC = GetNPCUnitByUID( (int)npcUID );
			if( NULL == pNPC )
				continue;

			pNPC->SetPositionOnLine( pLineData->endPos, pLineData->lineIndex );
		}
		m_setMisplacedNPC.clear();
	}

#endif RESET_INVALID_UNIT_POSITION_TEST

HRESULT	CX2Game::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	m_fTime			= (float)fTime;
	m_fElapsedTime	= fElapsedTime;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_kFrameUDPPack.IncreaseFrameMoveCount();



//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	Verify();

	if ( m_bGameEnd == true )
	{
		if ( m_pDLGResurrect != NULL )
		{
			SAFE_DELETE_DIALOG( m_pDLGResurrect );
			m_UnitUIDToResurrect = -1;
		}
	}

	if ( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->OnFrameMove( fTime, fElapsedTime );

#ifdef REVERSE_GRAVITY_TEST

	ReverseGravityFrameMove( fTime, fElapsedTime );

#endif REVERSE_GRAVITY_TEST



#ifndef _SERVICE_
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RCONTROL) == TRUE && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( m_bGameWait == true )
			m_bGameWait = false;
		else
			m_bGameWait = true;
	}

	/*
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_NUMPADPERIOD) == TRUE && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
	{
	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );
	}
	*/
#endif _SERVICE_



//#ifdef SHOOTING_TEST
//	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_PERIOD) == TRUE )
//	{
//		g_pMain->SetShootingLevel( g_pMain->GetShootingLevel() + 1 );
//		if( g_pMain->GetShootingLevel() > 6 )
//		{
//			g_pMain->SetShootingLevel( 0 );
//		}
//	}
//#endif SHOOTING_TEST



#ifdef FPS_SHOOTING_TEST
	//if( NULL != GetMyUnit() && true == s_bFPSShootingMouseDown )
	//{
	//	if( s_TimerFPSShooting.elapsed() > 0.05f )
	//	{
	//		s_TimerFPSShooting.restart();


	//		//POINTS ptMouse = MAKEPOINTS(lParam);

	//		D3DXVECTOR2 ptMouse;
	//		ptMouse.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	//		ptMouse.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	//		//ptMouse = g_pKTDXApp->MouseConvertByResolution( ptMouse );


	//		D3DXVECTOR3 vRayStart, vRayEnd;
	//		g_pKTDXApp->Make2DPosToRay( (int)ptMouse.x, (int)ptMouse.y, 5000.f, vRayStart, vRayEnd );
	//		vRayStart = GetX2Camera()->GetCamera().GetEye();
	//		vRayStart.y -= 100.f;
	//		vRayStart.z += 20.f;
	//		vRayStart += D3DXVECTOR3( (float) (rand() % 200 - 100), (float) (rand() % 200 - 100), (float) (rand() % 200 - 100) );
	//		//vRayEnd += D3DXVECTOR3( (float) (rand() % 100 - 50), (float) (rand() % 100 - 50), (float) (rand() % 100 - 50) );


	//		D3DXVECTOR3 vDir = vRayEnd - vRayStart;
	//		D3DXVECTOR3 vRotateDegree = GetDirVecToDegree( vDir );

	//		++m_iFPSShootingBulletType;
	//		if( m_iFPSShootingBulletType > 3 )
	//			m_iFPSShootingBulletType = 0;

	//		if( m_iFPSShootingBulletType == 0 )
	//		{
	//			GetDamageEffect()->CreateInstance( NULL,  GetMyUnit(), L"FPS_SHOOTING_TEST_BULLET_UP", 1.f, vRayStart, vRotateDegree, vRotateDegree );
	//		}
	//		else
	//		{
	//			GetDamageEffect()->CreateInstance( NULL,  GetMyUnit(), L"FPS_SHOOTING_TEST_BULLET", 1.f, vRayStart, vRotateDegree, vRotateDegree );
	//		}

	//		GetMyUnit()->PlaySound( L"Airship_Gunner_GunShot.ogg" );
	//	}
	//}

#endif FPS_SHOOTING_TEST


	if( m_bGameWait == true )
	{
#ifndef _SERVICE_
		if( null != m_optrFocusUnit )
			m_optrFocusUnit->OnCameraMove();

		if( false == m_bFreeCamera )
		{
			m_pCamera->OnFrameMove( fTime, fElapsedTime );

#ifdef KEYFRAME_CAMERA
			if(m_pCamera->GetEnableWorldCameraManualPlay() == true)
			{
				m_pCamera->WorldCameraKeyProcess();
			}
#endif KEYFRAME_CAMERA


		}
		else
		{
			m_FPSCamera.SetEnablePositionMovement( true );

			m_FPSCamera.FrameMove( fElapsedTime * 300.f );			
			m_pCamera->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * 500.f + *m_FPSCamera.GetEyePt();
			m_pCamera->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
			m_pCamera->GetCamera().UpdateCamera( fElapsedTime );

#ifdef KEYFRAME_CAMERA
			m_pCamera->WorldCameraKeyProcess();
#endif KEYFRAME_CAMERA
		}
#endif
		return S_OK;
	}

#ifdef DUNGEON_ITEM
	if( m_iItemDamageEffectIndex != 0 && null != m_optrItemDamageEffectGameUnit && m_fItemDamageEffectTime > 0.f )
	{
		m_fItemDamageEffectTime -= m_fElapsedTime;

		if( m_fItemDamageEffectTime < 0.f )
		{
			// ���� : ���� ��� ������ - �ٶ� -
			// ���� ��ȯ�̶� ���� ����̴�(�߹ؿ� ����Ʈ �����) ����Ʈ ������ ���� ����.
#ifdef SPECIAL_USE_ITEM
			if(m_iItemDamageEffectIndex == CX2Item::SST_WIND_SPHERE)	// �ٶ� ����
			{
				SpecialItemEffect(m_iItemDamageEffectIndex);
			}
			else
			{
				SummonSpiritEffect(m_iItemDamageEffectIndex);

			}
#else
			SummonSpiritEffect(m_iItemDamageEffectIndex);
#endif SPECIAL_USE_ITEM
			m_optrItemDamageEffectGameUnit.Reset();
			m_fItemDamageEffectTime = 0.f;
			m_iItemDamageEffectIndex = 0;
		}
	}
	else
	{		
		m_optrItemDamageEffectGameUnit.Reset();
		m_fItemDamageEffectTime = 0.f;
		m_iItemDamageEffectIndex = 0;
	}
#endif

	m_fSkipTime += m_fElapsedTime; 
	if( m_fSkipTime > 300.0f )
	{
		g_pKTDXApp->SkipFrame();
		HRESULT hr;
		if( FAILED( hr = g_pKTDXApp->GetDevice()->EvictManagedResources() ) )
		{
			switch( hr )
			{
			case D3DERR_OUTOFVIDEOMEMORY:
				ErrorLogMsg( XEM_ERROR4, "D3DERR_OUTOFVIDEOMEMORY" );
				break;

			default:
				ErrorLogMsg( XEM_ERROR4, "D3DERR_UNKNOWN" );
				break;
			}
		}

		m_fSkipTime = 0.0f;
	}

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	{
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		KLagCheck( eKnown_LagCheckType_PacketProcess );
		P2PPacketHandler();
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	if( m_GameState == GS_PLAY )
	{		
		m_fGameTime += m_fElapsedTime;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

        if ( m_pRoom != NULL )
        {
            m_kFrameUDPPack.SetUnitUID( GetMySlotUID() );
        }
        m_kFrameUDPPack.ResetFrameUDPPack( true );

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
        m_iPacketSendFrameMoveCount++;
        if ( m_iPacketSendFrameMoveCount >= g_pMain->GetFrameBufferNum() )
            m_iPacketSendFrameMoveCount = 0;
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND


#if 0
#ifdef SERV_PET_SYSTEM
		if( m_fGameTime > 7.f && g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			for(UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i)
			{
				CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
				if( pPet != NULL )
				{
					pPet->InitGamePet();
				}
			}

		}
#endif
#endif

		UpdateSkillCoolTime( fElapsedTime );

		if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
			true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() )
		{
			D3DXVECTOR3 vLookVec = m_pCamera->GetCamera().GetLookVec();
			D3DXVec3Normalize( &vLookVec, &vLookVec );
			D3DXVECTOR3 vUpVec = m_pCamera->GetCamera().GetUpVec();
			D3DXVec3Normalize( &vUpVec, &vUpVec );

			g_pKTDXApp->GetDSManager()->SetListenerData( m_pCamera->GetCamera().GetEye(), vLookVec, vUpVec );
		}

		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_BATTLE_FIELD:
			break;

		default:
			{
				m_AllowFrameCount--;
				if( m_AllowFrameCount < 0 )
				{
					m_AllowFrameCount = 0;
				}
			} break;
		}

		UpdateUnitPointer();



#ifdef KEYFRAME_CAMERA
		if( false == m_bFreeCamera && false == m_bWorldCameraEdit )
#else
		if( false == m_bFreeCamera )
#endif KEYFRAME_CAMERA
		{
			KLagCheck( eKnown_LagCheckType_LocalInputProcess );

			KeyProcess();
			if( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver )
			{
				// observer�� ��쿡 ī�޶�
				if( m_InputData.oneLCtr == true )
				{
					ChangeFocusUnit();
				}

				if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_1 ) == TRUE )
				{
					if( NULL != GetUserUnit(0) )
					{
						SetTimerFocusUnit( GetUserUnit(0) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_2 ) == TRUE )
				{
					if( NULL != GetUserUnit(1) )
					{
						SetTimerFocusUnit( GetUserUnit(1) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_3 ) == TRUE )
				{
					if( NULL != GetUserUnit(2) )
					{
						SetTimerFocusUnit( GetUserUnit(2) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_4 ) == TRUE )
				{
					if( NULL != GetUserUnit(3) )
					{
						SetTimerFocusUnit( GetUserUnit(3) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_5 ) == TRUE )
				{
					if( NULL != GetUserUnit(4) )
					{
						SetTimerFocusUnit( GetUserUnit(4) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_6 ) == TRUE )
				{
					if( NULL != GetUserUnit(5) )
					{
						SetTimerFocusUnit( GetUserUnit(5) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_7 ) == TRUE )
				{
					if( NULL != GetUserUnit(6) )
					{
						SetTimerFocusUnit( GetUserUnit(6) );
					}
				}
				else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_8 ) == TRUE )
				{
					if( NULL != GetUserUnit(7) )
					{
						SetTimerFocusUnit( GetUserUnit(7) );
					}
				}

			}
			else if ( NULL != GetMyUnit() )
			{
				CX2GUUser* pMyGUUser = GetMyUnit();
				// �׾��� �� ī�޶�
				if ( NULL != pMyGUUser && pMyGUUser->GetNowHp() <= 0.0f && pMyGUUser->GetShowObject() == false )
				{
					switch( GetGameType() )
					{
					case GT_DUNGEON:
						{
							if( m_InputData.oneX == true )
							{
								ChangeFocusUnit();
							}
						} break;

					case GT_PVP:
						{
							if( m_InputData.oneZ == true ||
								m_InputData.oneX == true )
							{
								ChangeFocusUnit();
							}
						} break;
					
					default:
						break;
					}
				}
			}
		}


		m_pTalkBoxMgr->OnFrameMove( fTime, fElapsedTime );
		//GuideTextFrameMove();


		m_UnitLoader.OnFrameMove( fElapsedTime );

		for( int i = 0; i < (int)m_NewGameUserList.size(); i++ )
		{
			NewGameUser* pPVPGameNewUser = &m_NewGameUserList[i];
			if( pPVPGameNewUser->m_bStart == true )
			{
				pPVPGameNewUser->m_fNewUserTime += fElapsedTime;

				if ( pPVPGameNewUser->m_fNewUserTime > 5.0f )
				{
					m_NewGameUserList.erase( m_NewGameUserList.begin() + i );
					i--;
				}
			}
			else
			{
				//��׶��� �ε��� ��÷ε��� ��� �������� �ϴ�!
				if( 
#ifdef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP					
					g_pKTDXApp->IsFirstFrameOfSimulationLoop() &&
#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP					
					m_UnitLoader.IsUnitLoadComplete( pPVPGameNewUser->m_UnitUID ) == true )
				{
					pPVPGameNewUser->m_bStart = true;
					CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID(pPVPGameNewUser->m_UnitUID);
					if( pSlotData != NULL )
					{
						
						CX2GUUser* pAddGameUser = AddUserUnitIntruder( pSlotData, pSlotData->m_Index );
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
						IntruderStart( pAddGameUser, pPVPGameNewUser->m_StartPosIndex );
#ifndef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
						g_pKTDXApp->SkipFrame();
#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
						IntruderStart( pPVPGameNewUser->m_UnitUID, pPVPGameNewUser->m_StartPosIndex );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

						switch ( GetGameType() )
						{
						case GT_PVP:
							{
								CX2GageManager* pGageManager = CX2GageManager::GetInstance();
								if ( NULL != pGageManager )
								{
									pGageManager->InsertPvpMemberUI( *pSlotData, pAddGameUser );
									pGageManager->UpdatePvpMemberGageData( pSlotData->m_UnitUID, pAddGameUser );
								}

								GetMinorParticle()->CreateSequence( NULL,  L"INTRUDE_NOT", 0,0,0, 1000,1000, -1, 1 );
								GetMinorParticle()->CreateSequence( NULL,  L"INTRUDE_NOT_ADD", 0,0,0, 1000,1000, -1, 1 );
							}
							break;

						case GT_BATTLE_FIELD:
							{
								CX2GageManager::GetInstance()->SetPartyMemberGameUnit( pAddGameUser );

								KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT kPacket;
								kPacket.m_iLoadedUnitUID = pPVPGameNewUser->m_UnitUID;
								g_pData->GetServerProtocol()->SendPacket( EGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, kPacket );
							} break;

						default:
							break;
						}			
					}					
				}
			}
		}






		// user, npc sync packet�� ������ User uid ����Ʈ ����
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_vecUserUIDforSyncPacket.resize(0);
//#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//		::EnterCriticalSection( &m_csGameIntruder );
//#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
//		{
//			CX2GUUser* pCX2GUUser = m_UserUnitList[i];
//			if( pCX2GUUser != NULL )
//			{
//				m_vecUserUIDforSyncPacket.push_back( pCX2GUUser->GetUnitUID() );
//			}
//		}
//#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//		::LeaveCriticalSection( &m_csGameIntruder );
//#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//
//		if( g_pX2Room != NULL )
//		{
//			for( int i = 0; i < (int)g_pX2Room->GetObserverSlotNum(); i++ )
//			{
//				CX2Room::SlotData* pSlotData = g_pX2Room->GetObserverSlotData( i );
//				if( pSlotData != NULL 
//					&& pSlotData->m_SlotState != CX2Room::SS_CLOSE
//					&& pSlotData->m_SlotState != CX2Room::SS_EMPTY )
//				{
//					m_vecUserUIDforSyncPacket.push_back( pSlotData->m_UnitUID );
//				}
//			}
//		}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


		if( m_bStopUnitChain == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// GUSI_DIE ������ NPC ��ü ����, ���Ͱ� �׾��ٴ� sync packet ����
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.resize(0);
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
			{
				CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
				if( NULL == pCX2GUNPC )
					continue;

				if( CX2GameUnit::GUSI_DIE != pCX2GUNPC->GetGameUnitState() )
					continue;

#ifndef X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
				if( IsHost() == true )
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					pCX2GUNPC->SendPacketImmediateForce( m_kFrameUDPPack );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					pCX2GUNPC->SendPacketImmediateForce( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
//
//					if( sizeof(KXPT_UNIT_NPC_SYNC) * m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > MTU_SIZE )
//					{
//						BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//					}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}
#endif  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE

#ifdef SERV_PVP_NEW_SYSTEM
				if( pCX2GUNPC->IsPvpBot() == false )
					DeleteNPCUnitByUID( pCX2GUNPC->GetUID() );
#else
				DeleteNPCUnitByUID( pCX2GUNPC->GetUID() );
#endif				
			}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			if( IsHost() == true )
//			{
//				if( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
//				{
//					BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//				}
//			}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef PVP_ZOOM_CAMERA
			bool bZoomOut = false;
			g_pMain->GetGameOption().DoSubZoomCameraTimer( fElapsedTime );
#endif

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			{
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

				KLagCheck( eKnown_LagCheckType_GameUnit_FrameMove );

                for( int iUnitType = 0; iUnitType <= 1; iUnitType++ )
                {

				    for( UINT i = 0; i < m_UnitList.size(); i++ )
				    {
					    CX2GameUnit* pCX2GameUnit = m_UnitList[i];

					    if ( pCX2GameUnit != NULL )
					    {
                            if ( iUnitType == 0 && pCX2GameUnit->GetGameUnitType() != CX2GameUnit::GUT_USER
                                || iUnitType == 1 && pCX2GameUnit->GetGameUnitType() != CX2GameUnit::GUT_NPC )
                                continue;

						    // user�� ��� unit�̳� unitdata�� ��ȿ���� ���� ����Ÿ�� ���ִ� ��찡 �ִ�.
						    // �̷���� ũ���������� ���� framemove ������ �ʵ����Ѵ�.
						    // �ٺ������� unit/unitdata�� �� ��ȿ���� ������ �����ľ� �� �ٺ����� �ذ��� �־���Ѵ�.
						    bool bCanDoFrameMove = true;
						    if( pCX2GameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
						    {
							    CX2GUUser *pUser = GetUserUnitByUID( pCX2GameUnit->GetUnitUID() );
							    if( pUser == NULL || pUser->GetUnit() == NULL )
							    {
								    bCanDoFrameMove = false;
							    }
						    }

						    if( bCanDoFrameMove == true )
						    {
							    pCX2GameUnit->OnFrameMove( fTime, fElapsedTime );

	    // 						if ( pCX2GameUnit != GetMyUnit()  )
	    // 						{
	    // 							pCX2GameUnit->GetGageManager()->OnFrameMove(fTime, fElapsedTime);
	    // 						}
#ifdef PVP_ZOOM_CAMERA
							    if( null != m_optrFocusUnit &&
								    g_pMain->GetGameOption().GetAutoCamera() == true && 
								    g_pMain->GetGameOption().GetSubZoomCameraTimer() <= 0.f)
							    {
								    if( IsValidUnit(pCX2GameUnit) == false )
									    continue;
								    if( pCX2GameUnit->GetNowHp() <= 0.f )
									    continue;
								    if( m_optrFocusUnit->GetTeam() == pCX2GameUnit->GetTeam() )
									    continue;						
								    if( pCX2GameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
								    {
									    CX2GUNPC *pNpc = (CX2GUNPC *)pCX2GameUnit;
									    if( pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
										    continue;
								    }

								    D3DXVECTOR3 vDist = m_optrFocusUnit->GetPos() - pCX2GameUnit->GetPos();

								    float fDist = D3DXVec3LengthSq(&vDist);
								    if( fDist > 800.f * 800.f )
								    {
									    bZoomOut = true;						
								    }
							    }					
#endif //PVP_ZOOM_CAMERA
						    }
					    }
				    }
//#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#ifndef X2OPTIMIZE_PUSH_PASS_BUG_FIX
//
//				    for( UINT i = 0; i < m_UnitList.size(); i++ )
//				    {
//					    CX2GameUnit* pCX2GameUnit = m_UnitList[i];
//					    if ( pCX2GameUnit != NULL )
//                            pCX2GameUnit->BackupPosition();
//                    }
//
//#endif  X2OPTIMIZE_PUSH_PASS_BUG_FIX
//#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                }

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#ifdef  X2OPTIMIZE_PUSH_PASS_BUG_FIX

				for( UINT i = 0; i < m_UnitList.size(); i++ )
				{
					CX2GameUnit* pCX2GameUnit = m_UnitList[i];
					if ( pCX2GameUnit != NULL )
                    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                        pCX2GameUnit->OnFrameMove_PostProcess();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                        pCX2GameUnit->BackupPosition();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    }
                }

//#endif  X2OPTIMIZE_PUSH_PASS_BUG_FIX
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef RIDING_SYSTEM
			CX2RidingPetManager* pRidingPetManager = CX2RidingPetManager::GetInstance();
			if ( NULL != pRidingPetManager )
				pRidingPetManager->OnFrameMove( fTime, fElapsedTime );
#endif //RIDING_SYSTEM

#ifdef SERV_PET_SYSTEM
			if( g_pData != NULL && g_pData->GetPetManager() != NULL )
			{
				g_pData->GetPetManager()->OnFrameMove( fTime, fElapsedTime );
			}
#endif

#ifdef PVP_ZOOM_CAMERA
			if( g_pMain->GetGameOption().GetAutoCamera() == true && g_pMain->GetGameOption().GetSubZoomCameraTimer() <= 0.f )
			{
				if( bZoomOut == true)
				{
					g_pMain->GetGameOption().SetSubZoomCameraLv(1);
				}	
				else
				{
					g_pMain->GetGameOption().SetSubZoomCameraLv(0);
				}
			}			
#endif
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		}



		ASSERT( NULL != m_pWorld );
		if( NULL != m_pWorld )
		{
			m_pWorld->OnFrameMove( fTime, fElapsedTime );
		}

		CX2GUUser* pMyGUUser = GetMyUnit();
		if ( NULL != pMyGUUser )
		{
			if( NULL != pMyGUUser && pMyGUUser->GetNowHp() > 0.0f )
			{
				UidType itemUID = m_pDropItemManager->TryGetDropItem( pMyGUUser->GetPos() );
				if( itemUID > 0 )
					Handler_EGS_GET_ITEM_REQ( itemUID );
			}
		}

 		//m_pMajorParticle->OnFrameMove( fTime, fElapsedTime );
 		//m_pMinorParticle->OnFrameMove( fTime, fElapsedTime );	
 
 		//m_pMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
 		//m_pMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

		m_pDamageEffect->OnFrameMove( fTime, fElapsedTime );

/*		m_EffectSet.OnFrameMove( fTime, fElapsedTime );*/
		

		m_pDropItemManager->OnFrameMove( fTime, fElapsedTime );


		if ( NULL == g_pData->GetCashShop() ||	!g_pData->GetCashShop()->GetOpen() )
		{
			if( null != m_optrFocusUnit )
				m_optrFocusUnit->OnCameraMove();
		}


		


		if( false == m_bFreeCamera )
		{
			m_pCamera->OnFrameMove( fTime, fElapsedTime );

#ifdef KEYFRAME_CAMERA
			if(m_pCamera->GetEnableWorldCameraManualPlay() == true)
			{
				m_pCamera->WorldCameraKeyProcess();
			}
#endif KEYFRAME_CAMERA

		}
		else
		{
			m_FPSCamera.SetEnablePositionMovement( true );

			m_FPSCamera.FrameMove( fElapsedTime * 300.f );			
			m_pCamera->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * 500.f + *m_FPSCamera.GetEyePt();
			m_pCamera->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
#ifdef KEYFRAME_CAMERA
			m_pCamera->GetCamera().UpVec( m_FPSCamera.GetWorldUp()->x, m_FPSCamera.GetWorldUp()->y, m_FPSCamera.GetWorldUp()->z);
			if(true == GetWorldCameraEdit())
				m_pCamera->GetCamera().SetTrackingTime(0.f);
#endif KEYFRAME_CAMERA
			m_pCamera->GetCamera().UpdateCamera( fElapsedTime );

#ifdef KEYFRAME_CAMERA
			m_pCamera->WorldCameraKeyProcess();
#endif KEYFRAME_CAMERA
		}

		//g_pKTDXApp->GetDSManager()->SetListenerData( m_pCamera->GetCamera().GetEye(), m_pCamera->GetCamera().GetLookVec(), m_pCamera->GetCamera().GetUpVec() );

		if( g_pData->GetPicChar() != NULL )
			g_pData->GetPicChar()->OnFrameMove( fElapsedTime );
		if( g_pData->GetPicCharRed() != NULL )
			g_pData->GetPicCharRed()->OnFrameMove( fElapsedTime );
		if( g_pData->GetPicCharBlue() != NULL )
			g_pData->GetPicCharBlue()->OnFrameMove( fElapsedTime );
		if( g_pData->GetPicCharGreen() != NULL )
			g_pData->GetPicCharGreen()->OnFrameMove( fElapsedTime );
		if( g_pData->GetPicCharYellow() != NULL )
			g_pData->GetPicCharYellow()->OnFrameMove( fElapsedTime );
		if( g_pData->GetPicCharTechPoint() != NULL )
			g_pData->GetPicCharTechPoint()->OnFrameMove( fElapsedTime );

		for( int i = 0; i < (int)m_ScoreTimeList.size(); i++ )
		{
			ScoreTime* pScoreTime = m_ScoreTimeList[i];
			pScoreTime->m_fTime -= m_fElapsedTime;
			if( pScoreTime->m_fTime < 0.0f )
			{
				AddGameScore( pScoreTime->m_Score );
				SAFE_DELETE( pScoreTime );
				m_ScoreTimeList.erase( m_ScoreTimeList.begin() + i );
				i--;
			}
		}

		if( m_fTechPointViewTime > 0.0f )
		{
			m_fTechPointViewTime -= m_fElapsedTime;
			if( m_fTechPointViewTime <= 0.0f )
			{

				AddGameScore( m_TechPoint, 0.4f );
				m_TechPoint = 0;
				m_fTechPointViewTime = 0.0f;
			}
		}


		DetermineLastKill();


		if( IsHost() == true )
		{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			if( g_pData->GetGameUDP()->GetNonRelayUIDs().empty() == false
				|| g_pData->GetGameUDP()->GetRelayUIDs().empty() == false )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			{
				KLagCheck( eKnown_LagCheckType_UDPProcess_Host );

				bool bRegularSend = false;

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
				bRegularSend = IsPacketSendFrame();
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//				m_NPCPacketSendIntervalNow += m_fElapsedTime;
//				if( m_NPCPacketSendIntervalNow >= m_NPCPacketSendInterval )	// 1.0/8.0 == ( 0.125 )
//				{
//					m_NPCPacketSendIntervalNow = 0.0f;
//					bRegularSend = true;
//				}//if
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

				if ( bRegularSend == true )
				{
					if ( m_iNPCRobustPacketSendIndex >= (int) m_NPCUnitList.size()
						&& m_kFrameUDPPack.GetFrameMoveCount() >= m_dwNPCRobustPacketSendBaseFrameMoveCount + X2GAME_NPC_ROBUST_PACKET_SEND_INTERVAL )
					{
						m_iNPCRobustPacketSendIndex = 0;
						m_dwNPCRobustPacketSendBaseFrameMoveCount = m_kFrameUDPPack.GetFrameMoveCount();
					}//if
				}//if

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
				bool    bRegularNPCSent = false;
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

				for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
				{
					CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
					if( NULL == pCX2GUNPC
						|| pCX2GUNPC->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
					{
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
						if ( bRegularSend == true && bRegularNPCSent == false && m_iNPCRobustPacketSendIndex == i )
						{
							m_iNPCRobustPacketSendIndex++;
						}
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
						continue;
					}
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
					if ( bRegularSend == true && bRegularNPCSent == false && m_iNPCRobustPacketSendIndex == i )
					{
						if( g_pMain->IsEnableUdpPacketOverlap() )
							pCX2GUNPC->SendReserve( 1 );
						m_iNPCRobustPacketSendIndex++;
						bRegularNPCSent = true;
					}
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
					pCX2GUNPC->SendPacket( m_kFrameUDPPack, ( bRegularSend == true ) ? false : true );
				}

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                if ( bRegularSend == true )
                {
				    for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
				    {
					    CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
					    if( NULL == pCX2GUNPC
						    || pCX2GUNPC->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
						    continue;
					    pCX2GUNPC->SendMiniSyncPacket( m_kFrameUDPPack );
				    }
                }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//				// �����ð� �������� sync ��Ŷ ����
//				m_NPCPacketSendIntervalNow += m_fElapsedTime;
//				if( m_NPCPacketSendIntervalNow >= m_NPCPacketSendInterval )	// 1.0/8.0 == ( 0.125 )
//				{
//					m_NPCPacketSendIntervalNow = 0.0f;
//
//					m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.resize(0);
//					for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
//					{
//						CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
//						if( NULL == pCX2GUNPC )
//							continue;
//
//						pCX2GUNPC->SendPacket( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
//
//						if( sizeof(KXPT_UNIT_NPC_SYNC) * m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > MTU_SIZE )
//						{
//							BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//						}
//					}
//
//					if( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
//					{
//						BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//					}
//				}
//
//
//
//				// ��� ������ �ϴ� �������� ��� ��� sync ��Ŷ ����
//				m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.resize(0);
//				for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
//				{
//					CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
//					if( NULL == pCX2GUNPC )
//						continue;
//
//					pCX2GUNPC->SendPacketImmediate( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
//
//					if( sizeof(KXPT_UNIT_NPC_SYNC) * m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > MTU_SIZE )
//					{
//						BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//					}
//				}
//
//				if( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
//				{
//					BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//				}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef SERV_PET_SYSTEM			
				for( UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i )
				{
					CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
					if( pPet != NULL )
					{						
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
						pPet->SendPacketImmediateForce( m_kFrameUDPPack );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//						pPet->SendPacketImmediateForce( m_kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					}

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					if( m_kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList.size() > 0 )
//					{
//						KSerBuffer buff;
//						Serialize( &buff, &m_kXPT_UNIT_PET_SYNC_PACK );
//						g_pData->GetGameUDP()->BroadCast( m_vecUserUIDforSyncPacket, XPT_UNIT_PET_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
//
//						m_kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList.resize(0);
//					}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}			
#endif//SERV_PET_SYSTEM
			}
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			else
			{
#ifdef SERV_PET_SYSTEM	
				for( UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i )
				{
					CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
					if( pPet != NULL )
					{						
						pPet->UpdateRandomTableIndex();
					}
				}			
#endif//SERV_PET_SYSTEM	
			}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		}




//#ifdef TRANSFORMER_TEST
//
//		if( NULL != GetMyUnit() &&
//			true == GetMyUnit()->GetTransformed() &&
//			NULL != GetMyUnit()->GetTransformer() &&
//			true == GetMyUnit()->GetTransformer()->IsLocalUnit() )
//		{
//			KLagCheck( eKnown_LagCheckType_UDPProcess_Transformer );
//
//			CX2GUNPC* pNPC = GetMyUnit()->GetTransformer();
//
////#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			pNPC->SendPacket( m_kFrameUDPPack );
//			pNPC->SendPacketImmediate( m_kFrameUDPPack );
////#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////			pNPC->SendPacket( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
////
////			if( sizeof(KXPT_UNIT_NPC_SYNC) * m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > MTU_SIZE )
////			{
////				BroadCast_XPT_UNIT_NPC_SYNC_PACK();
////			}
////
////			pNPC->SendPacketImmediate( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
////
////			if( sizeof(KXPT_UNIT_NPC_SYNC) * m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > MTU_SIZE )
////			{
////				BroadCast_XPT_UNIT_NPC_SYNC_PACK();
////			}
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		}
//
//#endif TRANSFORMER_TEST
//


		if( m_bLastKillCheck == false )
		{
			for( int i = 0; i < (int)m_CreateNPCDataList.size(); i++ )
			{
				CreateNPCData* pCreateNPCData = m_CreateNPCDataList[i];
				pCreateNPCData->m_DelayTime -= m_fElapsedTime;
				if( pCreateNPCData->m_DelayTime <= 0.0f )
				{
#ifdef SUMMON_MONSTER_CARD_SYSTEM
					
	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
					if( CX2GUNPC::NCT_MONSTER_CARD == pCreateNPCData->m_eCreateMonsterType )
					{
						CreateNPC( pCreateNPCData->m_NPCID, pCreateNPCData->m_Level, pCreateNPCData->m_bActive, pCreateNPCData->m_UID, 
							pCreateNPCData->m_UnitPos, pCreateNPCData->m_bRight, pCreateNPCData->m_DelayTime, pCreateNPCData->m_iKeyCode,
							pCreateNPCData->m_eTeamNum, pCreateNPCData->m_eAIType, pCreateNPCData->m_AllyUID, 0, CX2Room::TN_RED, pCreateNPCData->m_eCreateMonsterType );
					}
	#else // SERV_NEW_DEFENCE_DUNGEON
					if( true == pCreateNPCData->m_bIsSummonMonster )
					{
						CreateNPC( pCreateNPCData->m_NPCID, pCreateNPCData->m_Level, pCreateNPCData->m_bActive, pCreateNPCData->m_UID, 
							pCreateNPCData->m_UnitPos, pCreateNPCData->m_bRight, pCreateNPCData->m_DelayTime, pCreateNPCData->m_iKeyCode,
							pCreateNPCData->m_eTeamNum, pCreateNPCData->m_eAIType, pCreateNPCData->m_AllyUID, 0, CX2Room::TN_RED, pCreateNPCData->m_bIsSummonMonster );
					}
	#endif // SERV_NEW_DEFENCE_DUNGEON
					else
#endif //SUMMON_MONSTER_CARD_SYSTEM
					{
						CreateNPC( pCreateNPCData->m_NPCID, pCreateNPCData->m_Level, pCreateNPCData->m_bActive, pCreateNPCData->m_UID, 
							pCreateNPCData->m_UnitPos, pCreateNPCData->m_bRight, pCreateNPCData->m_DelayTime, pCreateNPCData->m_iKeyCode,
							pCreateNPCData->m_eTeamNum, pCreateNPCData->m_eAIType, pCreateNPCData->m_AllyUID);
					}

					SAFE_DELETE( pCreateNPCData );
					m_CreateNPCDataList.erase( m_CreateNPCDataList.begin() + i );
				}
			}
		}

		if( m_fLastkillShotTime != 0.0f )
		{
			m_fLastkillShotTime -= m_fElapsedTime;
			
#ifdef SET_LAST_KILL_SHOT_HIDE_UI // ����ȯ
			/// m_fLastkillShotTime�� �Ϸ� �Ǿ��� ��, UI�� ���ŵ� ��ũ������ �ﵵ�� ����
			if( m_fLastkillShotTime < 0.0f )
			{
				m_fLastkillShotTime = 0.0f;
				if( NULL != m_pRenderTargetLastkillShot && 
					true == m_pRenderTargetLastkillShot->IsOk() )
				{
					g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x32323232, 1.0f, 0);
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
                    CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
					g_pKTDXApp->GetDGManager()->ObjectChainSort();
					g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender(); 
					g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
                    CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
					m_pRenderTargetLastkillShot->CopyBackBufferToSurface();
				}
			}
#else // SET_LAST_KILL_SHOT_HIDE_UI
			if( m_fLastkillShotTime < 0.0f )
				m_fLastkillShotTime = 0.0f;

			if( NULL != m_pRenderTargetLastkillShot && 
				true == m_pRenderTargetLastkillShot->IsOk() )
			{
				m_pRenderTargetLastkillShot->CopyBackBufferToSurface();
			}
#endif // SET_LAST_KILL_SHOT_HIDE_UI
		}

		m_fElapsedTimeAfterLastKeyboardInput += fElapsedTime;


		if( m_fTimePauseNPCAI > 0.f )
		{
			m_fTimePauseNPCAI -= fElapsedTime;
			if( m_fTimePauseNPCAI <= 0.f )
			{
				m_fTimePauseNPCAI = 0.f;

				for( UINT i=0; i<m_NPCUnitList.size(); i++ )
				{
					CX2GUNPC* pGUNPC = m_NPCUnitList[i];
					if( NULL == pGUNPC )
						continue;

					pGUNPC->SetAIEnable( true );
				}
			}
		}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        m_kFrameUDPPack.FlushFrameUDPPack();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	}
	
	if( m_pShowAlarm != NULL )
	{
		m_fShowAlarmTime += fElapsedTime;
		if( m_fShowAlarmTime > m_pShowAlarm->m_fShowTime )
		{
			m_pShowAlarm = NULL;
			m_fShowAlarmTime = 0.f;
			
			CKTDGParticleSystem::CParticleEventSequence* pSeq = GetMajorParticle()->GetInstanceSequence( m_hDangerAlarm );
			if( pSeq != NULL && pSeq->GetShowObject() == true )
			{
				pSeq->SetShowObject(false);
			}
		}
	}

#ifdef MODIFY_DUNGEON_STAGING
	m_pCinematicUI->OnFrameMove(fTime, fElapsedTime);
#endif //MODIFY_DUNGEON_STAGING

	return S_OK;
}

HRESULT	CX2Game::OnFrameRender()
{

	if( m_GameState == GS_PLAY )
	{
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		if( m_pWorld != NULL )
		{
			m_pWorld->OnFrameRender();
		}

		g_pKTDXApp->GetDGManager()->ObjectChainSort();

		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE		
		

		switch( GetGameType() )
		{
		case GT_PVP:
			{
				if( NULL == g_pData->GetPVPRoom() )
					break;

				switch( g_pData->GetPVPRoom()->GetPVPGameType() )
				{
				case CX2PVPRoom::PGT_SURVIVAL:
					{
						CX2OldGageManager::ResetTeamIndex(0,0);
					} break;
				default:
					{
						if ( g_pX2Room == NULL )
							break;

						if ( g_pX2Room->GetMySlot() == NULL )
							break;

						if ( g_pX2Room->GetMySlot()->m_bObserver == true )
						{
							CX2OldGageManager::ResetTeamIndex( 0, 4 );
						}
						else
						{
							CX2OldGageManager::ResetTeamIndex();
						}
					} break;
				}
			} break;

		case GT_DUNGEON:
			{
				if ( g_pX2Room == NULL )
					break;

				if ( g_pX2Room->GetMySlot() == NULL )
					break;

				if ( g_pX2Room->GetMySlot()->m_bObserver == true )
				{
					CX2OldGageManager::ResetTeamIndex(4,0);
				}
				else
				{
					CX2OldGageManager::ResetTeamIndex(3,0);
				}
			} break;
		}



		m_pDropItemManager->RenderName();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		for( UINT i = 0; i < m_UnitList.size(); i++ )
		{
			CX2GameUnit* pGameUnit = m_UnitList[i];			

			if ( pGameUnit == NULL || 
				( GetGameType() == CX2Game::GT_BATTLE_FIELD && false == pGameUnit->DidReceiveFirstSyncPacket() ) )
			{
				continue;
			}

			//pGameUnit->GetGageManager()->UIFrameMove( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );

			if( pGameUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
				continue;

#ifdef NOT_RENDER_NPC_GAME_EDIT
			if ( CX2GameUnit::GUT_USER == pGameUnit->GetGameUnitType() 
				 || true == GetShowNpcByGameEdit() )
#endif // NOT_RENDER_NPC_GAME_EDIT
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
			{
				pGameUnit->RenderGageUI();
			}
#else
			pGameUnit->RenderGageUI();
#endif ALWAYS_SCREEN_SHOT_TEST

			//pGameUnit->GetGageManager()->OnFrameRender();

			if( pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				if( true == m_bRenderUserName )
				{
					pGameUnit->RenderName();
				}
			}
			else if( pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				// �����ʻ��� �հŸ��� �ִ� ���� �̸��� �������� �ʴ´�.
				if( true == m_sbRenderNPCName && 
					(pGameUnit->GetDistanceToCamera() < 4800 
#ifdef FIELD_BOSS_RAID // ���� �Ÿ� üũ
						|| true == pGameUnit->GetIsBosRaidNPC() // ���̵� ���ʹ� ����ó��
#endif // FIELD_BOSS_RAID
					))
					pGameUnit->RenderName();

				//#ifdef MAKE_GGOBONG_TEST
				//				((CX2GUNPC*)pGameUnit)->RenderCrazyMode();
				//#endif //MAKE_GGOBONG_TEST
			}
			else
			{
				// �����ʻ��� �հŸ��� �ִ� ���� �̸��� �������� �ʴ´�.
				if( pGameUnit->GetDistanceToCamera() < 4800 )
					pGameUnit->RenderName();
			}
		}

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		if( NULL != m_pTalkBoxMgr )
			m_pTalkBoxMgr->OnFrameRender();

#ifdef SERV_PET_SYSTEM
		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			g_pData->GetPetManager()->OnFrameRender();
		}
#endif

#ifdef RIDING_SYSTEM
		if ( NULL != CX2RidingPetManager::GetInstance() )
		{
			CX2RidingPetManager::GetInstance()->OnFrameRender();
		}
#endif //RIDING_SYSTEM

		if( NULL != g_pChatBox )
			g_pChatBox->OnFrameRender();

		//g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Flush();

		g_pKTDXApp->GetDGManager()->FrontUIRender();		

		RenderF1Help();

		if( m_AllowFrameCount < 0 )
		{
			//g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Draw2DText( L"�ý��� �ð��� �ʹ� �����ϴ�.", 300, 300, 0xffff0000, 0xff000000, DT_CENTER, CKTDGFontManager::FS_SHELL );
		}

#ifdef DIALOG_SHOW_TOGGLE	
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#endif
		{
			if( m_StateString.size() > 1 )
			{	//��ų ��� �� ��µǴ� ��Ʈ��
			// �ؿ��� ����
				g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD )->OutTextXY( 8, 689-20, m_StateString.c_str(), 0xffffffff,  CKTDGFontManager::FS_SHADOW, 0x00000000 );
			}
		}		

#ifdef DIALOG_SHOW_TOGGLE	
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false && 
			g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
#endif
		{
			if( m_pShowAlarm != NULL )
			{
				D3DXCOLOR cColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				int iAlarmX = 0;
				int iAlarmY = 160;

				if( GT_DUNGEON == GetGameType() )
				{
#ifdef SERV_EVENT_VALENTINE_DUNGEON
					CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
					if(	NULL != pDungeonGame &&
						NULL != pDungeonGame->GetDungeon() &&
						NULL != pDungeonGame->GetDungeon()->GetDungeonData() )
					{

						if( SEnum::DI_EVENT_VALENTINE_DAY == pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID )
						{
							iAlarmY = 230;
						}
					}	
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
					CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
					if(	NULL != pDungeonGame &&
						NULL != pDungeonGame->GetDungeon() &&
						NULL != pDungeonGame->GetDungeon()->GetDungeonData() )
					{

						if( SEnum::DI_EVENT_HALLOWEEN_DAY == pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID )
						{
							iAlarmY = 60;
						}
					}	
#endif //SERV_HALLOWEEN_EVENT_2013
					if( m_pShowAlarm->m_bDanger == true )
					{
						iAlarmY += 20; 
					}
					switch( m_pShowAlarm->m_eColor )
					{
					case ACT_BLACK:
						cColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
						break;
					case ACT_RED:
						cColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
						break;
					case ACT_BLUE:
						cColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
						break;
					case ACT_YELLOW:
						cColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
						break;
					case ACT_ORANGE:
						cColor = D3DXCOLOR(1.f, 0.38f, 0.1059f, 1.f);
						break;
					case ACT_MAGENTA:
						cColor = D3DXCOLOR(0.972f, 0.157f, 0.933f, 1.f);
						break;
					default:
						break;
					}


					RECT rt;
					rt.left = 0;
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
					// OutTextMultiline�ȿ��� GetResolutionScaleX �����ֹǷ� ���⼭ ���ؼ� �ѱ� �ʿ� ����.
					rt.right =  (LONG)1024;
//#else
//					rt.right =  (LONG)(1024 * g_pKTDXApp->GetResolutionScaleX());
//#endif

#ifdef USE_DT_VCENTER
					rt.top = iAlarmY;
					rt.bottom = (LONG)(iAlarmY + 40);
#endif // USE_DT_VCENTER


#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
					g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pShowAlarm->m_eFontType )->OutTextMultiline( iAlarmX, iAlarmY, 
						m_pShowAlarm->m_wstrMessage.c_str(), cColor, CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ),
						1.f, &rt, DT_CENTER|DT_TOP);
#else //SERV_HALLOWEEN_EVENT_2013
					g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_20_BOLD )->OutTextMultiline( iAlarmX, iAlarmY, 
						m_pShowAlarm->m_wstrMessage.c_str(), cColor, CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ),
						1.f, &rt, DT_CENTER|DT_TOP);
#endif //SERV_HALLOWEEN_EVENT_2013
				}
#ifdef FIELD_BOSS_RAID
				else if( GT_BATTLE_FIELD == GetGameType() )
				{
					if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
					{
						if( m_pShowAlarm->m_bDanger == true )
						{
							iAlarmY += 20; 
						}
						switch( m_pShowAlarm->m_eColor )
						{
						case ACT_BLACK:
							cColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
							break;
						case ACT_RED:
							cColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
							break;
						case ACT_BLUE:
							cColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
							break;
						case ACT_YELLOW:
							cColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
							break;
						case ACT_ORANGE:
							cColor = D3DXCOLOR(1.f, 0.38f, 0.1059f, 1.f);
							break;
						case ACT_MAGENTA:
							cColor = D3DXCOLOR(0.972f, 0.157f, 0.933f, 1.f);
							break;
						default:
							break;
						}


						RECT rt;
						rt.left = 0;
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
						// OutTextMultiline�ȿ��� GetResolutionScaleX �����ֹǷ� ���⼭ ���ؼ� �ѱ� �ʿ� ����.
						rt.right =  (LONG)1024;
//#else
//						rt.right =  (LONG)(1024 * g_pKTDXApp->GetResolutionScaleX());
//#endif

#ifdef USE_DT_VCENTER
						rt.top = iAlarmY;
						rt.bottom = (LONG)(iAlarmY + 40);
#endif // USE_DT_VCENTER

						g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_20_BOLD )->OutTextMultiline( iAlarmX, iAlarmY, 
							m_pShowAlarm->m_wstrMessage.c_str(), cColor, CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ),
							1.f, &rt, DT_CENTER|DT_TOP);
					}
				}
#endif // FIELD_BOSS_RAID
			}
		}

		//WCHAR adBuf[200] = {0,};
		//swprintf( adBuf, L"���׹̳� %d", (int)GetMyUnit()->GetGageManager()->GetAttackDelayGage()->fNow );
		//m_pFontForTime->Draw2DText( timeBuf, 512, 100, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, CKTDGFontManager::FS_SHELL );
		//g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_BOLD )->OutTextXY( 200, 400, adBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		//swprintf( adBuf, L"�����ٿ� %d", (int)GetMyUnit()->GetGageManager()->GetForceDownGage()->fNow );
		//g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_BOLD )->OutTextXY( 200, 430, adBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );

	}

#ifdef REFORM_UI_SKILLSLOT
	if( true == g_pData->GetUIManager()->GetUISkillTree()->GetShowEquipSkillGuide() )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL )->OutTextXY(  
			512, 300, GET_STRING(STR_ID_17274), D3DXCOLOR(1,0.6f,0,1), 	CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
	}
#endif //REFORM_UI_SKILLSLOT
	return S_OK;
}


bool CX2Game::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

#ifdef FPS_SHOOTING_TEST

	switch( uMsg )
	{
	case WM_RBUTTONDOWN:
		{
			s_bFPSShootingMouseDown = true;
		} break;

	case WM_RBUTTONUP:
		{
			s_bFPSShootingMouseDown = false;
		} break;


	case WM_LBUTTONDOWN:
		{
			s_TimerFPSShooting.restart();


			//POINTS ptMouse = MAKEPOINTS(lParam);

			D3DXVECTOR2 ptMouse;
			ptMouse.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
			ptMouse.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
			//ptMouse = g_pKTDXApp->MouseConvertByResolution( ptMouse );


			D3DXVECTOR3 vRayStart, vRayEnd;
			g_pKTDXApp->Make2DPosToRay( (int)ptMouse.x, (int)ptMouse.y, 5000.f, vRayStart, vRayEnd );
			vRayStart = GetX2Camera()->GetCamera().GetEye();
			vRayStart.y -= 100.f;
			vRayStart.z += 20.f;
			vRayStart += D3DXVECTOR3( (float) (rand() % 200 - 100), (float) (rand() % 200 - 100), (float) (rand() % 200 - 100) );
			//vRayEnd += D3DXVECTOR3( (float) (rand() % 100 - 50), (float) (rand() % 100 - 50), (float) (rand() % 100 - 50) );


			D3DXVECTOR3 vDir = vRayEnd - vRayStart;
			D3DXVECTOR3 vRotateDegree = GetDirVecToDegree( vDir );

			++m_iFPSShootingBulletType;
			if( m_iFPSShootingBulletType > 3 )
				m_iFPSShootingBulletType = 0;

			if ( NULL != GetMyUnit() )
			{
				if( m_iFPSShootingBulletType == 0 )
				{
					GetDamageEffect()->CreateInstance( GetMyUnit(), L"FPS_SHOOTING_TEST_BULLET_UP", 1.f, vRayStart, vRotateDegree, vRotateDegree );
				}
				else
				{
					GetDamageEffect()->CreateInstance( GetMyUnit(), L"FPS_SHOOTING_TEST_BULLET", 1.f, vRayStart, vRotateDegree, vRotateDegree );
				}

				GetMyUnit()->PlaySound( L"Airship_Gunner_GunShot.ogg" );
			}

		} break;

	}

#endif FPS_SHOOTING_TEST

//���콺 ���� ���� �� ���θ޴� ����ϰ� ���̵��� ����
	if ( uMsg == WM_MOUSEMOVE )
	{
#ifdef CHECK_INHERIT_STATE_MENU
		if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
		{
			CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
			if( NULL != pStateMenu )
			{
				pStateMenu->ShowMainMenuButton();
			}
		}
	}
	if( true == m_bFreeCamera )
	{
		m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);		
	}

#ifdef _SERVICE_
	if( uMsg == WM_NCLBUTTONDOWN || uMsg == WM_NCRBUTTONDOWN || uMsg == WM_NCMBUTTONDOWN )
		return true;
#endif

	return false;
}

HRESULT	CX2Game::OnResetDevice()
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if ( pCX2GameUnit != NULL )
			pCX2GameUnit->OnResetDevice();
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if( m_pWorld != NULL )
		m_pWorld->OnResetDevice();

 	//if( m_pMajorParticle != NULL )
 	//	m_pMajorParticle->OnResetDevice();
 	//if( m_pMinorParticle != NULL )
 	//	m_pMinorParticle->OnResetDevice();

	return S_OK;
}

HRESULT	CX2Game::OnLostDevice()
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if ( pCX2GameUnit != NULL )
			pCX2GameUnit->OnLostDevice();
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if( m_pWorld != NULL )
		m_pWorld->OnLostDevice();

 	//if( m_pMajorParticle != NULL )
 	//	m_pMajorParticle->OnLostDevice();
 	//if( m_pMinorParticle != NULL )
 	//	m_pMinorParticle->OnLostDevice();

	return S_OK;
}

void CX2Game::StopOtherUnit( CX2GameUnit* pNonStopUnit, float fStopTime, D3DXVECTOR3* pPos, float fDistance, bool bOtherTeam )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pUnit = m_UnitList[i];
		if( pUnit == pNonStopUnit )
			continue;

		if( bOtherTeam == true || m_GameType == GT_DUNGEON || m_GameType == GT_BATTLE_FIELD )
		{
			if( pUnit->GetTeam() == pNonStopUnit->GetTeam() )
				continue;
		}

#ifdef SPECIAL_ATTACK_DELAY_TEST

		if( GT_DUNGEON == m_GameType )
		{
			if( CX2GameUnit::GUT_USER == pUnit->GetGameUnitType() )
			{
				continue;
			}
		}

#endif // SPECIAL_ATTACK_DELAY_TEST


		if( pPos == NULL || fDistance <= 0 )
			pUnit->SetStopTime( fStopTime );
		else
		{
			if( GetDistance( *pPos, pUnit->GetPos() ) < fDistance )
				pUnit->SetStopTime( fStopTime );
		}		
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}

void CX2Game::StopAllUnit( float fStopTime, D3DXVECTOR3* pPos, float fDistance, CX2GameUnit* pMyUnit, float fMyStopTime )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pUnit = m_UnitList[i];
		if( pUnit == NULL )
			continue;

		if( pUnit == pMyUnit && fMyStopTime > 0.0f )
		{
			pUnit->SetStopTime( fMyStopTime );
			continue;
		}

#ifdef SPECIAL_ATTACK_DELAY_TEST

		if( GT_DUNGEON == m_GameType )
		{
			if( CX2GameUnit::GUT_USER == pUnit->GetGameUnitType() )
			{
				continue;
			}
		}

#endif // SPECIAL_ATTACK_DELAY_TEST

		if( m_GameType == GT_DUNGEON && pUnit->GetTeam() != pMyUnit->GetTeam() )
		{
			// ��ġ�� ���ų�, �Ÿ��� - �Ǵ� 0�̸� ��� Stop
			if( pPos == NULL || fDistance <= 0 )
				pUnit->SetStopTime( fStopTime );
			else
			{
				// ��ġ�� �Ÿ��� ������ �Ÿ����� �ִ� ������ Ÿ�� ��ž
				if( GetDistance( *pPos, pUnit->GetPos() ) < fDistance )
					pUnit->SetStopTime( fStopTime );
			}
		}
		else
		{
			if( pPos == NULL || fDistance <= 0 )
				pUnit->SetStopTime( fStopTime );
			else
			{
				if( GetDistance( *pPos, pUnit->GetPos() ) < fDistance )
					pUnit->SetStopTime( fStopTime );
			}
		}				
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}




void CX2Game::Stop2OtherUnit( CX2GameUnit* pNonStopUnit, float fStopTime, D3DXVECTOR3* pPos, float fDistance, bool bOtherTeam )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pUnit = m_UnitList[i];
		if( pUnit == pNonStopUnit )
			continue;

		if( bOtherTeam == true || m_GameType == GT_DUNGEON || m_GameType == GT_BATTLE_FIELD )
		{
			if( pUnit->GetTeam() == pNonStopUnit->GetTeam() )
				continue;
		}

#ifdef SPECIAL_ATTACK_DELAY_TEST

		if( GT_DUNGEON == m_GameType )
		{
			if( CX2GameUnit::GUT_USER == pUnit->GetGameUnitType() )
			{
				continue;
			}
		}

#endif // SPECIAL_ATTACK_DELAY_TEST

		if( pPos == NULL || fDistance <= 0 )
			pUnit->SetStop2Time( fStopTime );
		else
		{
			if( GetDistance( *pPos, pUnit->GetPos() ) < fDistance )
				pUnit->SetStop2Time( fStopTime );
		}		
	}

}

void CX2Game::Stop2AllUnit( float fStopTime, D3DXVECTOR3* pPos, float fDistance, CX2GameUnit* pMyUnit, float fMyStopTime )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		CX2GameUnit* pUnit = m_UnitList[i];
		if( pUnit == NULL )
			continue;

		if( pUnit == pMyUnit && fMyStopTime > 0.0f )
		{
			pUnit->SetStop2Time( fMyStopTime );
			continue;
		}


#ifdef SPECIAL_ATTACK_DELAY_TEST

		if( GT_DUNGEON == m_GameType )
		{
			if( CX2GameUnit::GUT_USER == pUnit->GetGameUnitType() )
			{
				continue;
			}
		}

#endif // SPECIAL_ATTACK_DELAY_TEST

		if( m_GameType == GT_DUNGEON && pUnit->GetTeam() != pMyUnit->GetTeam() )
		{
			if( pPos == NULL || fDistance <= 0 )
				pUnit->SetStop2Time( fStopTime );
			else
			{
				if( GetDistance( *pPos, pUnit->GetPos() ) < fDistance )
					pUnit->SetStop2Time( fStopTime );
			}
		}
		else
		{
			if( pPos == NULL || fDistance <= 0 )
				pUnit->SetStop2Time( fStopTime );
			else
			{
				if( GetDistance( *pPos, pUnit->GetPos() ) < fDistance )
					pUnit->SetStop2Time( fStopTime );
			}
		}		
	}
}

void CX2Game::ChangeFocusUnit(CX2GameUnit* pGameUnit /* = NULL */)
{
	bool bNext = false;
	bool bFind = false;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UserUnitList[i];

		if ( pCX2GameUnit == NULL )
		{
			continue;
		}

		if( bNext == true && pCX2GameUnit->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			m_optrFocusUnit = pCX2GameUnit;
			bFind = true;
			break;
		}
		if( m_optrFocusUnit.GetObservable() == pCX2GameUnit )
		{
			bNext = true;
		}		
	}

	if( bFind == false )
	{

		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			CX2GameUnit* pCX2GameUnit = m_UserUnitList[i];

			if ( pCX2GameUnit == NULL )
			{
				continue;
			}

			if( pCX2GameUnit->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
			{
				if ( pCX2GameUnit != pGameUnit )
				{
					m_optrFocusUnit = pCX2GameUnit;
					return;
				}
			}
		}

		m_optrFocusUnit = GetFoucusCameraNPC();
	}	
}

void CX2Game::ResetFocusUnit()
{
	if( null == m_optrFocusUnit )
	{
		ChangeFocusUnit();
		return;
	}

#ifdef FIX_OBSERVER_MODE_CRASH
	if( NULL != GetMyUnit() && m_optrFocusUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
#else  FIX_OBSERVER_MODE_CRASH
	if( m_optrFocusUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
#endif FIX_OBSERVER_MODE_CRASH
	{
		if( m_optrFocusUnit == GetMyUnit() && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
		{
			ChangeFocusUnit();
		}
		else
		{
			m_optrFocusUnit = GetMyUnit();
		}
	}
}
#ifdef INIT_FOCUS_UNIT
void CX2Game::InitFocusUnit()
{
	if ( NULL != GetMyUnit() )
		m_optrFocusUnit = GetMyUnit();
}
#endif // INIT_FOCUS_UN

void CX2Game::SetTimerFocusUnit( CX2GameUnit* pFocusUnit, float fFocusTime /*= 0.f*/, float fDistance /*= -1.f*/  )
{
	if ( NULL == pFocusUnit )
		return;

	m_optrFocusUnit = pFocusUnit;
	m_fFocusTime = fFocusTime;

	if( -1.f != fDistance )
	{
		m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), fDistance );
	}
	else
	{
		m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), g_pMain->GetGameOption().GetCameraDistance() );
	}

	m_pCamera->OnFrameMove( 0.0f, 0.0f );
}

D3DXVECTOR3 CX2Game::GetFocusUnitPos_LUA()
{ 
	if ( null != m_optrFocusUnit )
		return m_optrFocusUnit->GetPos();
	else
		return D3DXVECTOR3( -9999, -9999, -9999 );
}


CX2GUUser* CX2Game::GetUserUnitByUID( UidType unitUID )
{
	KTDXPROFILE();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GameUnit = m_UserUnitList[i];
		if( pCX2GameUnit != NULL && pCX2GameUnit->GetUnitUID() == unitUID )
		{

			return pCX2GameUnit;
		}
	}

	return NULL;
}

CX2GUNPC* CX2Game::GetNPCUnitByType( int iNPCID )
{
	KTDXPROFILE();

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL && 
			(int)pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == iNPCID )
		{
			return pCX2GUNPC;
		}
	}
	return NULL;
}

CX2GUNPC* CX2Game::GetNPCUnitByUID( int npcUID )
{
	KTDXPROFILE();

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL && pCX2GUNPC->GetUID() == npcUID )
			return pCX2GUNPC;
	}
	return NULL;
}

CX2GUNPC* CX2Game::GetNPCUnitByKeyCode( int iKeyCode )
{
	KTDXPROFILE();

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL && pCX2GUNPC->GetKeyCode() == iKeyCode )
			return pCX2GUNPC;
	}
	return NULL;
}


CX2GameUnit* CX2Game::GetUnit( int index )		
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	if ( index < 0 || index >= (int)m_UnitList.size() )
	{
		return NULL;
	}


	return m_UnitList[index]; 
}

CX2GUUser* CX2Game::GetUserUnit( int index )	
{ 
	if ( index < 0 || index >= GetUserUnitListSize() )
	{
		return NULL;
	}

	return m_UserUnitList[index]; 
}
CX2GUNPC* CX2Game::GetNPCUnit( int index )		
{ 
	if ( index < 0 || index >= MAX_GAME_NPC_UNIT_NUM )
	{
		return NULL;
	}
    ASSERT( index < (int) m_NPCUnitList.size() );

	return m_NPCUnitList[index]; 
}


void CX2Game::KillUnit( bool bNPCKiller, UidType killerUID, UidType maxDamageKillerUID, bool bNPCKilled, UidType killedUID )
{
	if( bNPCKiller == false )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		CX2GameUnit* pKiller = NULL;
		if( killerUID < 0 )
			pKiller = GetNPCUnitByUID( (int)killerUID );
		else
			pKiller = GetUserUnitByUID( killerUID );
#else
		CX2GUUser* pKiller = GetUserUnitByUID( killerUID );
#endif
		if( pKiller != NULL )
		{
			if( bNPCKilled == false )
			{
				pKiller->m_KillUserUnitList.push_back( killedUID );
			}
			else
			{
				pKiller->m_KillNPCList.push_back( (int)killedUID );
			}
		}
#ifdef SERV_PVP_NEW_SYSTEM
		if( maxDamageKillerUID < 0 )
			pKiller = GetNPCUnitByUID( (int)maxDamageKillerUID );
		else
			pKiller = GetUserUnitByUID( maxDamageKillerUID );
#else
		pKiller = GetUserUnitByUID( maxDamageKillerUID );
#endif
		if( pKiller != NULL )
		{
			if( bNPCKilled == false )
			{
				pKiller->m_MaxDamageKillUserUnitList.push_back( killedUID );
			}
			else
			{
				pKiller->m_MaxDamageKillNPCList.push_back( (int)killedUID );
			}
		}

	}
	else
	{
		CX2GUNPC* pKiller = GetNPCUnitByUID( (int)killerUID );
		if( pKiller != NULL )
		{
			if( bNPCKilled == false )
			{
				pKiller->m_KillUserUnitList.push_back( killedUID );
			}
			else
			{
				pKiller->m_KillNPCList.push_back( (int)killedUID );
			}
		}
	}
}


int CX2Game::GetKillAssistCount( UidType userUnitUID )
{
#ifdef SERV_PVP_NEW_SYSTEM
	CX2GameUnit *pUnit = NULL;
	if( userUnitUID < 0 )
	{
		pUnit = GetNPCUnitByUID( (int)userUnitUID );
	}
	else
	{
		pUnit = GetUserUnitByUID( userUnitUID );
	}
	if( pUnit == NULL )
		return -1;

	return (int)pUnit->m_MaxDamageKillUserUnitList.size();
#else
	CX2GUUser* pCX2GUUser = GetUserUnitByUID( userUnitUID );
	if( NULL == pCX2GUUser )
		return -1;

	return (int) pCX2GUUser->m_MaxDamageKillUserUnitList.size();
#endif
}

int CX2Game::GetKillCount( UidType userUnitUID )
{
#ifdef SERV_PVP_NEW_SYSTEM
	CX2GameUnit *pUnit = NULL;
	if( userUnitUID < 0 )
	{
		pUnit = GetNPCUnitByUID( (int)userUnitUID );
	}
	else
	{
		pUnit = GetUserUnitByUID( userUnitUID );
	}
	if( pUnit == NULL )
		return -1;
	return (int) pUnit->m_KillUserUnitList.size();
#else
	CX2GUUser* pCX2GUUser = GetUserUnitByUID( userUnitUID );
	if( NULL == pCX2GUUser )
		return -1;
	return (int) pCX2GUUser->m_KillUserUnitList.size();
#endif
}

void CX2Game::SetKillScore( UidType userUnitUID, int killNum, int killedNum, int mdKillNum )
{
#ifdef SERV_PVP_NEW_SYSTEM
	CX2GameUnit* pCX2GUUser = NULL;
	if( userUnitUID < 0 )
		pCX2GUUser = GetNPCUnitByUID( (int)userUnitUID );
	else
		pCX2GUUser = GetUserUnitByUID( userUnitUID );
#else
	CX2GUUser* pCX2GUUser = GetUserUnitByUID( userUnitUID );
#endif
	if( pCX2GUUser != NULL )
	{
		//��ų
		if( ((int)pCX2GUUser->m_KillUserUnitList.size()) > killNum )
		{
			int removeKill = pCX2GUUser->m_KillUserUnitList.size() - killNum;
			for( int i = 0; i < removeKill; i++ )
				pCX2GUUser->m_KillUserUnitList.erase( pCX2GUUser->m_KillUserUnitList.begin() );
		}
		else if( ((int)pCX2GUUser->m_KillUserUnitList.size()) < killNum )
		{
			int addKill = killNum - pCX2GUUser->m_KillUserUnitList.size();
			for( int i = 0; i < addKill; i++ )
				pCX2GUUser->m_KillUserUnitList.push_back( -1 );
		}

		//MD ų
		if( ((int)pCX2GUUser->m_MaxDamageKillUserUnitList.size()) > mdKillNum )
		{
			int removeKill = pCX2GUUser->m_MaxDamageKillUserUnitList.size() - mdKillNum;
			for( int i = 0; i < removeKill; i++ )
				pCX2GUUser->m_MaxDamageKillUserUnitList.erase( pCX2GUUser->m_MaxDamageKillUserUnitList.begin() );
		}
		else if( ((int)pCX2GUUser->m_MaxDamageKillUserUnitList.size()) < mdKillNum )
		{
			int addKill = killNum - pCX2GUUser->m_MaxDamageKillUserUnitList.size();
			for( int i = 0; i < addKill; i++ )
				pCX2GUUser->m_MaxDamageKillUserUnitList.push_back( -1 );
		}
	}
}


bool CX2Game::DeleteUserUnitByData( CX2GameUnit* pGameUnit )
{
	//gamelog << __FUNCTION__ << fileout;

	if ( pGameUnit == NULL )
		return false;

	if( m_optrFocusUnit.GetObservable() == pGameUnit )
		ChangeFocusUnit(pGameUnit);

	vector< CX2GameUnit* >::iterator i;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for ( i = m_UnitList.begin(); i != m_UnitList.end(); i++ )
	{
		CX2GameUnit* tempUnit = (*i);
		if ( pGameUnit ==  tempUnit )
		{
			m_UnitList.erase( i );
			break;
		}
	}

	for ( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GameUnit* tempUnit = m_UserUnitList[i];
		if ( pGameUnit == tempUnit )
		{
			m_UserUnitList[i] = NULL;
			break;
		}
	}

	pGameUnit->FinishAndClearAllBuff();
	m_pDamageEffect->UnitDeleteProcess( pGameUnit );
	CX2GameUnit::DeleteKTDGObject( pGameUnit );

	return true;
}

bool CX2Game::DeleteUserUnit( int index )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if ( index < 0 || index >= GetUserUnitListSize() )
		return false;


	CX2GUUser* pCX2GUUser = m_UserUnitList[index];

	if( pCX2GUUser == NULL )
		return false;

	return DeleteUserUnitByUID( pCX2GUUser->GetUnitUID() );
}

bool CX2Game::DeleteUserUnitByUID( UidType unitUID )
{
	// �ε��߿� ƨ��ų� ������ ����� ����� ���� ������ uid�� 0������ ���� ���� �� �ִ� �� ����. �ڼ��Ѱ��� Ȯ���غ��� ��.
	if( unitUID <= 0 )
		return false;


#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipManager() && 
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
		NULL != g_pData->GetRelationshipEffectManager() )
	{
		CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
		CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = pRelationshipManager->GetMyRelationshipInfo();
		if ( pRelationshipInfo->m_eRelationStateType !=	SEnum::RT_SOLO )
		{
			if ( pRelationshipInfo->m_iRelationTargetUserUid == unitUID )
			{
				pRelationshipManager->ResetRelationPartnerInGame();
				g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID ( unitUID );
			}
		}
		else
		{
			g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID ( unitUID );
		}
	}

#endif // ADDED_RELATIONSHIP_SYSTEM

// 	CX2GUUser* pCX2GUUser = GetUserUnitByUID( unitUID );
// 	if( pCX2GUUser == NULL )
// 	{
// 		// �������� ������ ���ų� �ϴ� ��� ���ӻ��¿��� �ٷ� leave_room_not�� ���� �� �ֱ� ������ leave_room_not�� �޾��� ���� 
// 		// ���� ������ �õ��Ѵ�. �׷��� ���������� ������ �������ͼ� ���� ������ ��� uid�� user �˻��� �����ϰ� �Ǳ� ������ �׻� ������ �ƴ�.
// 		wstringstream wstrstm;
// 		wstrstm << unitUID;
// 		ErrorLogMsg( XEM_ERROR94, wstrstm.str().c_str() );
// 		return false;
// 	}
// 

// 	if( m_optrFocusUnit.GetObservable() == pCX2GUUser )
// 		ChangeFocusUnit( pCX2GUUser );

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	CX2GameUnit* pUserGameUnit = NULL;
	for( UINT i = 0; i < m_UnitList.size(); i++ )
	{
		// ������ // 2013-04-08 // �ؿ� �� �������� ����
		// UserUID�� ������ ���� NPCUID�� ���� ��� ũ���� �߻��ϴ� ������ ���� ����ó��
		if( NULL != m_UnitList[i] && CX2GameUnit::GUT_USER == m_UnitList[i]->GetGameUnitType() )
		{
			if ( m_UnitList[i]->GetUnitUID() == unitUID )
			{
				pUserGameUnit = m_UnitList[i];
				m_UnitList.erase( m_UnitList.begin() + i );
				break;
			}
		}
	}

	if ( NULL != pUserGameUnit )
	{
		for ( UINT i = 0; i < m_UserUnitList.size(); i++ )
		{
			CX2GUUser* pCX2GUUserTemp = m_UserUnitList[i];

			if ( pCX2GUUserTemp == NULL )
			{
				continue;
			}

			if ( pCX2GUUserTemp->GetUnitUID() == unitUID )
			{
				m_UserUnitList[i] = NULL;
				break;
			}
		}

		if( m_optrFocusUnit.GetObservable() == pUserGameUnit )
			ChangeFocusUnit( pUserGameUnit );

		pUserGameUnit->FinishAndClearAllBuff();
		
		// note!! �ϴ� npc�� ��� damageeffect���� owner unit�� �����ϴ� ��찡 �� �����Ƿ� ���� ���� ó���� ������ �ʴ°ɷ� �صΰ�, ���߿� ��������
		if ( m_pDamageEffect != NULL )
			m_pDamageEffect->UnitDeleteProcess( pUserGameUnit );

		//SAFE_DELETE( pCX2GUUser );
		CX2GUUser::DeleteKTDGObject( pUserGameUnit );
	}



	// note!! �ϴ� npc�� ��� damageeffect���� owner unit�� �����ϴ� ��찡 �� �����Ƿ� ���� ���� ó���� ������ �ʴ°ɷ� �صΰ�, ���߿� ��������
	//if ( m_pDamageEffect != NULL )
	//	m_pDamageEffect->UnitDeleteProcess( pCX2GUUser );

	//SAFE_DELETE( pCX2GUUser );
	//CX2GUUser::DeleteKTDGObject( pCX2GUUser );

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	m_UnitLoader.DeleteUnitLoadData( unitUID );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	return true;
}

bool CX2Game::DeleteNPCUnit( int index )
{
	if ( index < 0 || index >= GetNPCUnitListSize() )
		return false;

	CX2GameUnit* temp = GetNPCUnit(index);

	bool bCheckUnitDelete = false;

	vector< CX2GameUnit* >::iterator i;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for ( i = m_UnitList.begin(); i != m_UnitList.end(); i++ )
	{
		CX2GameUnit* tempUnit = (*i);
		if ( temp == tempUnit )
		{
			m_UnitList.erase( i );
			bCheckUnitDelete = true;
			break;
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	bool bCheckNPCDelete = false;

	for ( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GameUnit* tempUnit = m_NPCUnitList[i];
		if ( tempUnit == NULL )
		{
			continue;
		}

		if ( temp == tempUnit )
		{
			m_NPCUnitList[i] = NULL;
			bCheckNPCDelete = true;
			break;
		}
	}

	if ( bCheckUnitDelete == false )
	{
		ErrorLog( XEM_ERROR116 );
	}

	if ( bCheckNPCDelete == false )
	{
		ErrorLog( XEM_ERROR116 );
	}


	CX2GUNPC* pCX2GUNPC = (CX2GUNPC*) temp;

	if( m_optrFocusUnit.GetObservable() == pCX2GUNPC )
		ChangeFocusUnit( pCX2GUNPC );


	if ( NULL != pCX2GUNPC )
	{
		pCX2GUNPC->FinishAndClearAllBuff();
		pCX2GUNPC->SetUserGrapReset();
	}


	//#ifdef HEAVY_LOG_TEST
	//	if ( pCX2GUNPC != NULL )
	//	{
	//		wstringstream tempLogStm;
	//		tempLogStm << L"DeleteNPCUnit, NPC ID : " << pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID;  
	//		StateLog( tempLogStm.str().c_str() );
	//	}
	//#endif HEAVY_LOG_TEST



	//if( m_pDamageEffect != NULL )
	//	m_pDamageEffect->UnitDeleteProcess( (CX2GameUnit*) pCX2GUNPC );

	//SAFE_DELETE( pCX2GUNPC );
	CX2GUNPC::DeleteKTDGObject( pCX2GUNPC );


	return true;
}

bool CX2Game::DeleteNPCUnitByUID( const UINT uiNpcUid_ )
{
	CX2GUNPC* pCX2NPCUnit = GetNPCUnitByUID( uiNpcUid_ );
	if( pCX2NPCUnit == NULL )
		return false;

	bool bCheckUnitDelete = false;

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if( pCX2GameUnit == pCX2NPCUnit )
		{
			m_UnitList.erase( m_UnitList.begin() + i );
			bCheckUnitDelete = true;
			break;
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	bool bCheckNPCDelete = false;

	for ( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2NPCUnitTemp = m_NPCUnitList[i];
		if ( pCX2NPCUnitTemp == NULL )
		{
			continue;
		}

		if ( pCX2NPCUnit == pCX2NPCUnitTemp )
		{
			m_NPCUnitList[i] = NULL;
			bCheckNPCDelete = true;
			break;
		}
	}

	if ( bCheckUnitDelete == false )
	{
		ErrorLog( XEM_ERROR115 );
	}

	if ( bCheckNPCDelete == false )
	{
		ErrorLog( XEM_ERROR115 );
	}

	if( m_optrFocusUnit.GetObservable() == pCX2NPCUnit )
		ChangeFocusUnit( pCX2NPCUnit );

	if ( NULL != pCX2NPCUnit )
	{
		pCX2NPCUnit->FinishAndClearAllBuff();
		pCX2NPCUnit->SetUserGrapReset();
	}

	//#ifdef HEAVY_LOG_TEST
	//	if ( pCX2NPCUnit != NULL )
	//	{
	//		wstringstream tempLogStm;
	//		tempLogStm << L"DeleteNPCUnitByUID, NPC ID : " << pCX2NPCUnit->GetNPCTemplet().m_nNPCUnitID;  
	//		StateLog( tempLogStm.str().c_str() );
	//	}
	//#endif HEAVY_LOG_TEST


	//if( m_pDamageEffect != NULL )
	//	m_pDamageEffect->UnitDeleteProcess( (CX2GameUnit*) pCX2NPCUnit );

	//SAFE_DELETE( pCX2NPCUnit );
	CX2GUNPC::DeleteKTDGObject( pCX2NPCUnit );

	return true;
}

bool CX2Game::DeleteAllNPCUnit()
{
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( NULL == pCX2GUNPC )
			continue;

		//{{ mauntain : ����ȯ [2012.06.14] ���� ī�� ��ȯ ��� - ��ȯ ���Ϳ� ���� ���� Pass
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
		if( CX2GUNPC::NCT_MONSTER_CARD == pCX2GUNPC->GetNPCCreateType() 
#ifdef FINALITY_SKILL_SYSTEM //��â��
			|| CX2GUNPC::NCT_CHANGESTAGE_REMAIN == pCX2GUNPC->GetNPCCreateType() 
#endif //FINALITY_SKILL_SYSTEM)
			)
	#else // SERV_NEW_DEFENCE_DUNGEON
		if( true == pCX2GUNPC->GetIsMonsterCard() )
	#endif // SERV_NEW_DEFENCE_DUNGEON
		{
			continue;	//���� Pass
		}
#endif SUMMON_MONSTER_CARD_SYSTEM
		//}}

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		if( true == pCX2GUNPC->GetIsAllyEventMonster() )
		{
			CX2GUUser* pUser = g_pX2Game->GetUserUnitByUID( pCX2GUNPC->GetOwnerGameUnitUID() );

			if( NULL != pUser )		//USer���� ��ȯ�� NPC UID ����
			{
				pUser->SetAllyEventMonsterUID( -1 );
			}
		}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		::EnterCriticalSection( &m_csGameIntruder );		
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		pCX2GUNPC->SetUserGrapReset();
		
		vector<CX2GameUnit*>::iterator vItrGameUnit = m_UnitList.begin();
		while ( m_UnitList.end() != vItrGameUnit )
		{
			CX2GameUnit* pGameUnit = *vItrGameUnit;
			if ( pCX2GUNPC == pGameUnit )
			{
				pGameUnit->FinishAndClearAllBuff();
				CX2GUNPC::DeleteKTDGObject( pGameUnit );
				vItrGameUnit = m_UnitList.erase( vItrGameUnit );
				break;
			}
			else
				++vItrGameUnit;
		}
		
		m_NPCUnitList[i] = NULL;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		if ( null == m_optrFocusUnit )
			ChangeFocusUnit();

// 	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
// 	{
// 		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
// 		if( NULL == pCX2GUNPC )
// 			continue;
// 
// 		::EnterCriticalSection( &m_csGameIntruder );
// 		for( int j = 0; j < (int)m_UnitList.size(); j++ )
// 		{
// 			CX2GameUnit* pCX2GameUnit = m_UnitList[j];
// 			if( pCX2GameUnit == pCX2GUNPC )
// 			{
// 				m_UnitList.erase( m_UnitList.begin() + j );
// 				break;
// 			}
// 		}
// 		::LeaveCriticalSection( &m_csGameIntruder );
// 		m_NPCUnitList[i] = NULL;
// 		if( m_optrFocusUnit.GetObservable() == pCX2GUNPC )
// 			ChangeFocusUnit( pCX2GUNPC );
// 
// 		CX2GUNPC::DeleteKTDGObject( pCX2GUNPC );
// 
 	}

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
    CX2GUNPC::ResetReactionSimulationCounter();
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD

#ifdef  X2OPTIMIZE_ACCELERATE_SHARED_NPC_LUA_BY_LUAJIT
    if ( g_pKTDXApp->GetLuaBinder() != NULL )
        g_pKTDXApp->GetLuaBinder()->FlushJitCompileMode();
#endif  X2OPTIMIZE_ACCELERATE_SHARED_NPC_LUA_BY_LUAJIT

	return true;
}

void    CX2Game::AddNPCResourcesToDeviceList( CX2UnitManager::NPC_UNIT_ID npcID )
{
	m_UnitLoader.AddNPCUnitIDResource( npcID );
}//CX2Game::AddNPCResourcesToDeviceList()

void    CX2Game::RemoveAllNPCResourcesFromUnitLoader()
{
	m_UnitLoader.RemoveAllNPCResources();
}//CX2Game::RemoveAllNPCResourcesFromUnitLoader()

int CX2Game::GetUserUnitNum()
{
	int num = 0;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		if( m_UserUnitList[i] != NULL )
		{
			num++;
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	return num;
}


int CX2Game::GetNPCUnitNum()
{
	int num = 0;
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		if( m_NPCUnitList[i] != NULL )
		{
			num++;
		}
	}

	return num;
}

int CX2Game::GetEmptyNPCIndex()
{
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		if( m_NPCUnitList[i] == NULL )
		{
			return i;
		}
	}

	return -1;
}








void CX2Game::KeyProcess()
{
	KTDXPROFILE();

	/// ĳ�ü��� �����ִ� ��쿡�� Ű�Է� ���� ����
	if ( NULL != g_pData->GetCashShop() && 
		 g_pData->GetCashShop()->GetOpen() )
		 return;

	CX2GUUser* pMyGUUser = static_cast<CX2GUUser*>( GetMyUnit() );
	if ( NULL == pMyGUUser )
		return;

	m_InputData.Init();

	// ������ ���� ��Ŀ���� editbox�� �ִٸ� �����̰ų� ���ེų ���� ���� �ʵ��� �Ѵ�.
	bool bFocusEditBox = false;
	CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
	if ( pControl != NULL )
	{
		if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
			pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
		{
			bFocusEditBox = true;
		}
	}

	bool bEnableKeyProcessMisc = true;

//#ifdef PVP_BOSS_COMBAT_TEST
//	if( GetMyUnit()->GetNowStateID() == GetMyUnit()->GetFrozenState() ||
//		GetMyUnit()->GetTeam() != GetMyUnit()->GetTeamNumOriginal() )
//	{
//		bEnableKeyProcessMisc = false;
//	}
//#endif PVP_BOSS_COMBAT_TEST
	
#ifdef NO_P2P_NO_GAME
	if( g_pX2Room->GetNumOfPlayers() > 1 ) 
	{
		if( NULL != g_pData->GetGameUDP() )
		{
			const UINT MAGIC_FRAME_COUNT_WITHOUT_P2P_PACKET_RECEIVE = 1000;
			if( g_pData->GetGameUDP()->GetElapsedFrameAfterPacketReceive() > MAGIC_FRAME_COUNT_WITHOUT_P2P_PACKET_RECEIVE )
			{
				bEnableKeyProcessMisc = false;
			}
		}
		else
		{
			bEnableKeyProcessMisc = false;
		}
	}
	if ( GetEnableAllKeyProcess() && m_AllowFrameCount > 0 /*&& true == bEnableKeyProcessMisc*/ )
#else NO_P2P_NO_GAME
	//{{AFX
	if ( GetEnableAllKeyProcess() && m_AllowFrameCount > 0 )
	//}}AFX
#endif NO_P2P_NO_GAME
	{
		InputData( GA_LEFT, m_InputData.pureDoubleLeft, m_InputData.oneDoubleLeft, m_InputData.pureLeft, m_InputData.oneLeft );
		InputData( GA_RIGHT, m_InputData.pureDoubleRight, m_InputData.oneDoubleRight, m_InputData.pureRight, m_InputData.oneRight );
		InputData( GA_UP, m_InputData.pureDoubleUp, m_InputData.oneDoubleUp, m_InputData.pureUp, m_InputData.oneUp );
		InputData( GA_DOWN, m_InputData.pureDoubleDown, m_InputData.oneDoubleDown, m_InputData.pureDown, m_InputData.oneDown );
		if ( CheckEnableKeyProcess() == true )
		{
			if( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
			{
				if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F5) == TRUE )
				{
					pMyGUUser->SetNowHp( pMyGUUser->GetMaxHp() );
				}

				if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F6) == TRUE )
				{
					pMyGUUser->ChargeMpAndEtcInTrainingGame();		

#ifdef SHOW_ATTACK_BOX_DUMMY
					if( NULL != GetMajorXMeshPlayer() )
					{
						GetMajorXMeshPlayer()->ToggleShowAttackBox();
						if( true == GetMajorXMeshPlayer()->GetShowAttackBox() )
						{
							g_pChatBox->AddChatLog( L"���ùڽ� ����", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR( 1, 0, 0, 1));
						}
						else
						{
							g_pChatBox->AddChatLog( L"���ùڽ� �����", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR( 1, 0, 0, 1));
						}
					}
#endif // SHOW_ATTACK_BOX_DUMMY
				}
			}

#ifndef _SERVICE_
			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F6) == TRUE )
			{
				if ( g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )	
				{
					pMyGUUser->SetNowHp( pMyGUUser->GetMaxHp() );
				}
				
#ifdef SHOW_ATTACK_BOX_DUMMY
				if( NULL != GetMajorXMeshPlayer() )
				{
					GetMajorXMeshPlayer()->ToggleShowAttackBox();
					if( true == GetMajorXMeshPlayer()->GetShowAttackBox() )
					{
						g_pChatBox->AddChatLog( L"���ùڽ� ����", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR( 1, 0, 0, 1));
					}
					else
					{
						g_pChatBox->AddChatLog( L"���ùڽ� �����", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR( 1, 0, 0, 1));
					}
				}
#endif // SHOW_ATTACK_BOX_DUMMY
			}

			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F7) == TRUE )
			{
				pMyGUUser->SetNowMp( pMyGUUser->GetMaxMp() );

			//{{ kimhc // 2010.12.15 // 2010-12-23 New Character CHUNG
		#ifdef	NEW_CHARACTER_CHUNG
				if ( CX2Unit::UT_CHUNG == pMyGUUser->GetUnit()->GetType() )
				{
					CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( pMyGUUser->GetGageData() );
					if ( NULL != pChungGageData )
						pChungGageData->SetNowCannonBallCount( pChungGageData->GetMaxCannonBallCount() );
#ifdef SERV_CHUNG_TACTICAL_TROOPER
						if( CX2Unit::UC_CHUNG_TACTICAL_TROOPER == pMyGUUser->GetUnitClass() )
						{
//							pChungGageData->SetExtraCannonBallCount( pChungGageData->GetMaxCannonBallCount() );
						}
#endif SERV_CHUNG_TACTICAL_TROOPER
				}				
		#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.15 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
				if ( CX2Unit::UT_ARA == pMyGUUser->GetUnit()->GetType() )
				{
					CX2AraGageData*	pAraGageData = static_cast<CX2AraGageData*>( pMyGUUser->GetGageData() );
					if ( NULL != pAraGageData )
						pAraGageData->SetNowForcePower( pAraGageData->GetMaxForcePower() );
				}
#endif

#ifdef ADD_RENA_SYSTEM //��â��
				if( CX2Unit::UT_LIRE == pMyGUUser->GetUnit()->GetType() )
				{
					CX2GULire_ElvenRanger* pRena = static_cast<CX2GULire_ElvenRanger*>( pMyGUUser );
					CX2RenaGageData* pRenaGageData = static_cast<CX2RenaGageData*>( pMyGUUser->GetGageData() );
					if( NULL != pRenaGageData && NULL != pRena )
					{
						pRena->UpNaturalForce(pRenaGageData->GetMaxNaturalForce());
					}
				}
#endif //ADD_RENA_SYSTEM

			}

			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )				
			{
				if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE )
				{
					if ( pMyGUUser->GetHyperModeCount() == pMyGUUser->GetFrameData().syncData.m_HyperModeCount )
					{
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
						/// �ֵ��� ����, DP �������� Ǯ�� ä������.
						if ( CX2Unit::UT_ADD == pMyGUUser->GetUnit()->GetType() )
						{
							static_cast<CX2GUAdd*>( pMyGUUser )->SetDPValue( MAX_DP_GAGE_VALUE );
						}
						else
#endif // SERV_9TH_NEW_CHARACTER
						{
							int iHyperModeCount = pMyGUUser->GetHyperModeCount();
							if ( static_cast<int>( CX2GageUI::PGUWO_ORB3 ) == ++iHyperModeCount )
								iHyperModeCount = static_cast<int>( CX2GageUI::PGUWO_ORB3 );

							pMyGUUser->SetHyperModeCount( iHyperModeCount );
						}
					}
				}
	#ifdef REALTIME_SCRIPT_PATCH
				switch( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_DUNGEON_GAME:
				case CX2Main::XS_TRAINING_GAME:
				case CX2Main::XS_BATTLE_FIELD:
					{
						if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F2) == TRUE )
						{
							//UI �޽��÷��̾�
							g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
							g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

							//���� �޽��÷��̾�
							g_pData->ResetGameMajorXMeshPlayer( L"GameMajorXMeshPlayer.txt" );
							g_pData->ResetGameMinorXMeshPlayer( L"GameMinorXMeshPlayer.txt" );

							//EffectSet
							g_pData->ResetUIEffectSet( L"UIEffectSet.lua" );
							g_pData->ResetGameEffectSet( L"EffectSet.lua" );

							//DamageEffect
							SAFE_DELETE( m_pDamageEffect );
							m_pDamageEffect	= new CX2DamageEffect();
							m_pDamageEffect->OpenScriptFile( L"DamageEffect.lua" );

#ifdef INT_RESOURCE_DEVIDE
							g_pData->GetUIMajorXMeshPlayer()->OpenScriptFile( L"UIMajorXMeshPlayer_INT.txt" );
							//g_pData->GetUIMinorXMeshPlayer()->OpenScriptFile( L"UIMinorXMeshPlayer_INT.txt" ); ���� ���� ����
							g_pData->GetGameMajorXMeshPlayer()->OpenScriptFile( L"GameMajorXMeshPlayer_INT.txt" );
							//g_pData->GetGameMinorXMeshPlayer()->OpenScriptFile( L"GameMinorXMeshPlayer_INT.txt" ); ���� ���� ����
							g_pData->GetUIEffectSet()->OpenScriptFile( L"UIEffectSet_INT.lua" );
							g_pData->GetGameEffectSet()->OpenScriptFile( L"EffectSet_INT.lua" );
							m_pDamageEffect->OpenScriptFile( L"DamageEffect_INT.lua" );
#endif INT_RESOURCE_DEVIDE
					
							//��ųƮ�� ����Ÿ �ε�
					#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							g_pData->ResetSkillTree( L"NewSkillTempletVer2.lua", L"SkillData.lua" ); // 10
					#else // UPGRADE_SKILL_SYSTEM_2013
							g_pData->ResetSkillTree( L"NewSkillTemplet.lua", L"SkillData.lua" ); // 10
					#endif // UPGRADE_SKILL_SYSTEM_2013

							g_pChatBox->AddChatLog( L"GameXMeshPlayer,EffectSet, DamageEffect\nNewSkillTemplet", 
								KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

							g_pChatBox->AddChatLog( L"��ũ��Ʈ �ǽð� ��ġ �Ϸ� ", 
								KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

							g_pChatBox->AddChatLog( L"GameParticle, ���� ��ũ��Ʈ ������ �����Ʒü� �������ؾ� ���ŵ˴ϴ�.", 
								KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

#ifdef RIDING_SYSTEM
							if( true == GetMyUnit()->GetRidingOn() )
							{
								CX2RidingPetManager::GetInstance()->Handler_EGS_UNSUMMON_RIDING_PET_REQ(true);
							}
							CX2RidingPetManager::GetInstance()->RefreshRidingPetScript();
#endif // RIDING_SYSTEM

#ifdef EXPAND_DEVELOPER_SCRIPT	  // ������, ������ ��ũ��Ʈ Ȯ�� ��� �߰�
							if ( g_pMain->ResetDeveloperScriptSet( L"DevScriptTable.lua" ) == true )
							{
								g_pChatBox->AddChatLog( L"DevScriptTable ���� �Ϸ�", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
							}
#endif // EXPAND_DEVELOPER_SCRIPT  // ������, ������ ��ũ��Ʈ Ȯ�� ��� �߰�							
						}
					} break;
				default:
					break;
				}
	#endif //REALTIME_SCRIPT_PATCH
			}
#endif

#ifdef PVP_ZOOM_CAMERA
			if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F2) == TRUE )
			{
				g_pMain->GetGameOption().ResetAutoCamera();
				g_pMain->GetGameOption().SetAutoCamera( !g_pMain->GetGameOption().GetAutoCamera() );
			}
#endif

			if( ( true == m_bEnableCommandKeyProcess ) &&
				( g_pX2Room != NULL && g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_bObserver == false ) &&
				( bFocusEditBox == false ) 
				)
			{
				InputData( GA_ATTACK_FAST, m_InputData.pureDoubleZ, m_InputData.oneDoubleZ, m_InputData.pureZ, m_InputData.oneZ ); //DIK_Z
				InputData( GA_ATTACK_STRONG, m_InputData.pureDoubleX, m_InputData.oneDoubleX, m_InputData.pureX, m_InputData.oneX ); //DIK_X

#ifdef RIDING_SYSTEM
				if( NULL != CX2RidingPetManager::GetInstance() && true == CX2RidingPetManager::GetInstance()->GetRidingOn() )
				{
					InputData( GA_SKILL4, m_InputData.pureDoubleC, m_InputData.oneDoubleC, m_InputData.pureC, m_InputData.oneC );
				}
				else
				{
#endif //RIDING_SYSTEM

				if( true == g_pData->GetUIManager()->GetUISkillTree()->GetSkillSlotSwap() )
				{
					InputData( GA_SKILL_EX1, m_InputData.pureDoubleA, m_InputData.oneDoubleA, m_InputData.pureA, m_InputData.oneA ); //DIK_A
					InputData( GA_SKILL_EX2, m_InputData.pureDoubleS, m_InputData.oneDoubleS, m_InputData.pureS, m_InputData.oneS ); //DIK_S
					InputData( GA_SKILL_EX3, m_InputData.pureDoubleD, m_InputData.oneDoubleD, m_InputData.pureD, m_InputData.oneD ); //DIK_D
					InputData( GA_SKILL_EX4, m_InputData.pureDoubleC, m_InputData.oneDoubleC, m_InputData.pureC, m_InputData.oneC ); //DIK_C

					InputData( GA_SKILL1, m_InputData.pureDoubleQ, m_InputData.oneDoubleQ, m_InputData.pureQ, m_InputData.oneQ ); //DIK_Q
					InputData( GA_SKILL2, m_InputData.pureDoubleW, m_InputData.oneDoubleW, m_InputData.pureW, m_InputData.oneW ); //DIK_W
					InputData( GA_SKILL3, m_InputData.pureDoubleE, m_InputData.oneDoubleE, m_InputData.pureE, m_InputData.oneE ); //DIK_E
					InputData( GA_SKILL4, m_InputData.pureDoubleR, m_InputData.oneDoubleR, m_InputData.pureR, m_InputData.oneR ); //DIK_R
				}
				else
				{
					InputData( GA_SKILL1, m_InputData.pureDoubleA, m_InputData.oneDoubleA, m_InputData.pureA, m_InputData.oneA );
					InputData( GA_SKILL2, m_InputData.pureDoubleS, m_InputData.oneDoubleS, m_InputData.pureS, m_InputData.oneS );
					InputData( GA_SKILL3, m_InputData.pureDoubleD, m_InputData.oneDoubleD, m_InputData.pureD, m_InputData.oneD );
					InputData( GA_SKILL4, m_InputData.pureDoubleC, m_InputData.oneDoubleC, m_InputData.pureC, m_InputData.oneC );

					InputData( GA_SKILL_EX1, m_InputData.pureDoubleQ, m_InputData.oneDoubleQ, m_InputData.pureQ, m_InputData.oneQ );
					InputData( GA_SKILL_EX2, m_InputData.pureDoubleW, m_InputData.oneDoubleW, m_InputData.pureW, m_InputData.oneW );
					InputData( GA_SKILL_EX3, m_InputData.pureDoubleE, m_InputData.oneDoubleE, m_InputData.pureE, m_InputData.oneE );
					InputData( GA_SKILL_EX4, m_InputData.pureDoubleR, m_InputData.oneDoubleR, m_InputData.pureR, m_InputData.oneR );
				}

#ifdef RIDING_SYSTEM
				}


				if( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_RIDING ) == TRUE &&
					( NULL != g_pMain->GetNowState() && static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() ))
				{
					CX2RidingPetManager* pRidingPetManager = CX2RidingPetManager::GetInstance();
					if( NULL != pRidingPetManager && true == pRidingPetManager->GetHasRidingPet() )
					{
						if( true == pRidingPetManager->GetRidingOnState() )
						{
							if ( true == pRidingPetManager->CanPushSummonButton() )
							{
								pRidingPetManager->RefreshSummonButtonElpasedTime();

								if( false == pMyGUUser->GetRidingOn() )
									pRidingPetManager->Handler_EGS_SUMMON_RIDING_PET_REQ( pRidingPetManager->GetFirstRidingPetUID() );
								else
									pRidingPetManager->Handler_EGS_UNSUMMON_RIDING_PET_REQ();
							}
						}
						else
						{   //ž�� �� �� �������� ���� �ȳ� ����
							int iStringID = ( false == pMyGUUser->GetRidingOn() ) ? STR_ID_24526 : STR_ID_24527;
							GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( iStringID ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
						}
					}
				}
#endif //RIDING_SYSTEM

			}
			if( bFocusEditBox == false )
			{
				InputData( GA_SLOT_CHANGE, m_InputData.pureDoubleSpace, m_InputData.oneDoubleSpace, m_InputData.pureSpace, m_InputData.oneSpace );	 //DIK_SPACE		
			}
			

			InputData( GA_AWAKE, m_InputData.pureDoubleLCtr, m_InputData.oneDoubleLCtr, m_InputData.pureLCtr, m_InputData.oneLCtr ); //DIK_LCONTROL
		}
	}
#ifdef BANDICAM_RECORDING
	if( g_pMain != NULL && g_pData != NULL )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_SCROLL ) == TRUE )
		{
			if( g_pMain->IsCapturing() == false )
			{
				g_pMain->Bandi_StartVideoCapture();
			}
			else
			{
				g_pMain->Bandi_EndVidioCapture();
			}
		}
	}
#endif BANDICAM_RECORDING

	KTDXPROFILE_BEGIN( "KEYPRESS_CHECK" );
	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		if( true == m_InputData.IsKeyPressed() ) 
		{
			m_fElapsedTimeAfterLastKeyboardInput = 0.f;
		}
	}
	KTDXPROFILE_END();

	if( g_pData->GetSlideShot()->IsPresentNow() == true )
	{
		m_InputData.Init();
	}
	if ( pMyGUUser != NULL && pMyGUUser->GetNowHp() <= 0.0f )
		return;

	if ( pMyGUUser != NULL )
		pMyGUUser->SetInputData( &m_InputData );
}

int CX2Game::LiveUserUnitNum()			
{ 
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	int iCount = 0;

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = (CX2GUUser*)m_UserUnitList[i];
		if( pCX2GUUser == NULL )
			continue;

		if( pCX2GUUser->GetGameUnitState() != CX2GameUnit::GUSI_DIE && 
			pCX2GUUser->GetNowHp() > 0.f )
		{
			iCount++;
		}
	}

	return iCount;
}

int CX2Game::LiveActiveNPCNum()			
{ 
	int iCount = 0;
	for( unsigned i=0; i<m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*) m_NPCUnitList[i];
		if( NULL == pNPC )
			continue;

		if( pNPC->GetTeam() != CX2Room::TN_MONSTER )
			continue;
		
		if( pNPC->IsActiveMonster() == true )
		{
			iCount++;
		}
	}
	return iCount;
}

int	CX2Game::LiveNPCNumType_LUA( CX2UnitManager::NPC_UNIT_ID unitID ) 
{
	int iCount = 0;
	for( unsigned i=0; i<m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( NULL == pNPC )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == unitID )
		{
			iCount++;
		}
	}
	return iCount;
}

bool CX2Game::IsKillNPCKeyCode_LUA( int keyCode )
{
	map<int, int>::iterator it;
	it = m_mapKilledNPCKeyCode.find( keyCode );
	if( it != m_mapKilledNPCKeyCode.end() )
	{
		if( it->second > 0 )
			return true;
	}

	return false;
}


int CX2Game::GetKilledKeyCodeNPCCount( int keyCode )
{
	map<int, int>::iterator it;
	it = m_mapKilledNPCKeyCode.find( keyCode );
	if( it != m_mapKilledNPCKeyCode.end() )
	{
		return it->second;
	}

	return 0;
}

int CX2Game::LiveKeyCodeNPCNum( int keyCode )
{
	int iCount = 0;
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetTeam() != CX2Room::TN_MONSTER )
			continue;

		if( pNPC->GetKeyCode() == keyCode )
		{
			iCount++;
		}
	}
	return iCount;
}

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-02

/** @function	: LiveKeyCodeNPCNumFreeTeam
@brief			: ���� ������� �ش� Ű�ڵ带 NPC ���� ��ü�� ��ȯ
@param			: ������ Ű�ڵ�
@return			: �ش� Ű�ڵ带 ���� NPC ��ü��
*/
int CX2Game::LiveKeyCodeNPCNumFreeTeam( int keyCode )
{
	int iCount = 0;
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetKeyCode() == keyCode )
		{
			iCount++;
		}
	}
	return iCount;
}

#endif //SERV_NEW_DEFENCE_DUNGEON

int CX2Game::GetAliveActiveNPCCount()
{
	int iCount = 0;
	for( unsigned i=0; i<m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*) m_NPCUnitList[i];
		if( NULL == pNPC )
			continue;

		if( pNPC->GetGameUnitState() == CX2GameUnit::GUSI_DIE || pNPC->GetNowHp() <= 0.f )
			continue;

		if( pNPC->GetTeam() != CX2Room::TN_MONSTER )
			continue;

		if( pNPC->IsActiveMonster() == true )
		{
			iCount++;
		}
	}
	return iCount;
}


#ifdef SERV_INSERT_GLOBAL_SERVER
int CX2Game::GetEliteNPCUnitNum()
{
	int iCount = 0;
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		if( m_NPCUnitList[i] != NULL )
		{
			CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
			if(true == m_NPCUnitList[i]->GetIsEliteMonster())
				iCount++;
		}
	}
	
	return iCount;
}

int CX2Game::GetEliteKeyCodeNPCUnitNum( int keyCode )
{
	int iCount = 0;
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		if( m_NPCUnitList[i] != NULL )
		{
			CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];

			if( pNPC->GetTeam() != CX2Room::TN_MONSTER )
				continue;
			
			if( pNPC->GetKeyCode() != keyCode )
				continue;

			if(true == m_NPCUnitList[i]->GetIsEliteMonster())
				iCount++;
		}
	}

	return iCount;
}
#endif SERV_INSERT_GLOBAL_SERVER



CX2GUNPC* CX2Game::GetKeyCodeNPC( int keyCode )
{
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetKeyCode() == keyCode )
		{
			return pNPC;
		}
	}
	return NULL;
}

CX2GUNPC* CX2Game::GetFoucusCameraNPC()
{
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetFocusCamera() == true )
		{
			return pNPC;
		}
	}
	return NULL;
}

bool CX2Game::IsValidUnit( CX2GameUnit* pUnit )
{
	if ( NULL == pUnit )
		return false;

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if( pCX2GameUnit == pUnit )
		{

			return true;
		}
	}

	return false;
}


bool CX2Game::IsAllUserHPPercentMoreThan( float fHPPercent )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for ( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser == NULL )
			continue;

		if( pCX2GUUser->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
			return false;

		if( 0.f != pCX2GUUser->GetMaxHp() )
		{
			if( pCX2GUUser->GetNowHp() / pCX2GUUser->GetMaxHp() < fHPPercent * 0.01f )
			{
				return false;
			}
		}

	}

	return true;
}


bool CX2Game::IsAllUserDead()
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for ( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser == NULL )
			continue;

		if( pCX2GUUser->GetNowHp() > 0.f )
		{
			return false;
		}
	}


	return true;
}

// ��� ������ ������Ű��, MP�� full��
void CX2Game::PartyFeverAllUser( float fAddHyperModeTime )
{
	{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		BOOST_TEST_FOREACH( CX2GUUser*, pUser, m_UserUnitList )
		{
			if( NULL == pUser )
				continue;

			if( CX2GameUnit::GUSI_DIE == pUser->GetGameUnitState() )
				continue;

			if( pUser->GetNowHp() <= 0.f )
				continue;

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			/// �ֵ��� ��, DP 150 ȸ��
			if ( CX2Unit::UT_ADD == pUser->GetUnitType() )
				pUser->UpNowDPValue( 3000.f );
			else
#endif //SERV_9TH_NEW_CHARACTER
				pUser->ForceChangeHyperModeWithoutMotion( fAddHyperModeTime );

			pUser->SetNowMp( pUser->GetMaxMp() );

		}
	}



	if( NULL != GetFocusUnit() )
		GetFocusUnit()->PlaySound( L"Alarm_Fever.ogg" );
	else if( NULL != GetMyUnit() )
		GetMyUnit()->PlaySound( L"Alarm_Fever.ogg" );



	GetMajorParticle()->CreateSequence( NULL,  L"Fever_Mode", 0,0,0 );
	GetMajorParticle()->CreateSequence( NULL,  L"Fever_Mode01", 0,0,0 );
	GetMajorParticle()->CreateSequence( NULL,  L"Fever_Mode02", 0,0,0 );
	GetMajorParticle()->CreateSequence( NULL,  L"Flare_Fever_Mode03", 512,384,0 );
	
}



bool CX2Game::RebirthUserUnit( UidType unitUID, int startPosIndex )
{
#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pX2Room->IsNpcSlot( unitUID ) == true )
	{
		CX2GUNPC* pNpc = GetNPCUnitByUID( (int)unitUID );
		if( pNpc == NULL )
			return false;

		//pNpc->ReInit(false, startPosIndex );
		pNpc->CX2GameUnit::ReInit(false, startPosIndex);
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        pNpc->ReInitExtraWork();
//#endif SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		pNpc->InitPosition( false, startPosIndex );
		pNpc->SetNowHp( pNpc->GetMaxHp() );
		pNpc->SetShowObject( true );
		
		
		pNpc->SetForceInvincible( 5.f, 5.f );
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        pNpc->StateChangeForce( pNpc->GetStartState() );
//#else  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		pNpc->StateChange( pNpc->GetStartState() );
//#endif SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		pNpc->SetAIEnable(true);

		pNpc->PlaySound( L"Revival.ogg" );
		return true;
	}
#endif

	CX2GUUser* pCX2GUUser = GetUserUnitByUID( unitUID );
	if( pCX2GUUser == NULL )
		return false;

	pCX2GUUser->ReInit( false, startPosIndex );

	pCX2GUUser->SetForceInvincible( 5.0f );
	pCX2GUUser->SetShowInvincible( 5.0f );
	pCX2GUUser->StateChange( pCX2GUUser->GetStartIntrudeStateID(), true );

	UpdateUnitPointer();

	if( GetMyUnit() == pCX2GUUser )
	{
		m_optrFocusUnit = GetMyUnit();
	}

	if( NULL != pCX2GUUser )
	{
		pCX2GUUser->PlaySound( L"Revival.ogg" );
	}

	return true;
}

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
CX2GUNPC*
#else SERV_CREATE_ELITE_FOR_ADMIN
void
#endif SERV_CREATE_ELITE_FOR_ADMIN

#ifdef SERV_INSERT_GLOBAL_SERVER
CX2Game::CreateNPC( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, int uid, 
						const D3DXVECTOR3& vPos, bool bRight, float fDelayTime, int iKeyCode, 
						CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_MONSTER*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
						UidType iAllyUID /*= -1*/ , KAttribEnchantNpcInfo* _kAttribEnchantNpcInfo /* = NULL */
	#ifdef	SERV_TRAPPING_RANGER_TEST
						, CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */
	#endif	SERV_TRAPPING_RANGER_TEST
	#ifdef SUMMON_MONSTER_CARD_SYSTEM

		#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
						, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType /*= NCT_NONE */		/// ���� ���� Ÿ��
		#else // SERV_NEW_DEFENCE_DUNGEON
						, bool bIsSummonMonster	/*= false */		//���� ī��� ��ȯ�Ǿ������� ���� ����
		#endif // SERV_NEW_DEFENCE_DUNGEON

	#endif SUMMON_MONSTER_CARD_SYSTEM
						)
#else //SERV_INSERT_GLOBAL_SERVER
CX2Game::CreateNPC( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, int uid, 
						D3DXVECTOR3 vPos, bool bRight, float fDelayTime, int iKeyCode, 
						CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_MONSTER*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
						UidType iAllyUID /*= -1*/
	#ifdef	SERV_TRAPPING_RANGER_TEST
						, CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */
	#endif	SERV_TRAPPING_RANGER_TEST
						)
#endif SERV_INSERT_GLOBAL_SERVER
{
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
	CX2GUNPC* pNPC = NULL;
#endif SERV_CREATE_ELITE_FOR_ADMIN

	if( m_bLastKillCheck == true )
	{
#ifdef SUMMON_MONSTER_CARD_SYSTEM		//��� �� NPC�� ����ص�, ���� ī��� ����� �� �ֵ��� ����

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
		if(  CX2GUNPC::NCT_NONE == eCreateMonsterType 
#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
			&& unitID != CX2UnitManager::NUI_APINK_ANGEL_03_RENA
			&& unitID != CX2UnitManager::NUI_APINK_ANGEL_04_RAVEN
			&& unitID != CX2UnitManager::NUI_APINK_ANGEL_05_EVE
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT			
			)
	#else // SERV_NEW_DEFENCE_DUNGEON
		if(  true != bIsSummonMonster )
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
			return NULL;
#else SERV_CREATE_ELITE_FOR_ADMIN
		return;
#endif SERV_CREATE_ELITE_FOR_ADMIN
	}

	if( fDelayTime <= 0.0f )
	{
		int index;
		for( index = 0; index < (int)m_NPCUnitList.size(); index++ )		
		{
			if( m_NPCUnitList[index] == NULL )
				break;
		}
		ASSERT( index < (int)m_NPCUnitList.size() );		// ���࿡ ���ڸ��� ������???

#ifdef NO_MORE_ERROR_TEST
		if( index >= (int) m_NPCUnitList.size() )
		{
			InHouseLog( L"There are too many monsters" );
		}
#endif NO_MORE_ERROR_TEST



		CX2GUUser* pOwnerGUUser = NULL;
		if( CX2NPCAI::NAT_ALLY == eAIType && -1 != iAllyUID )
			pOwnerGUUser = GetUserUnitByUID( iAllyUID );
			
		// ���� ���� ����
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		const CX2UnitManager::NPCUnitInfo*	pNPCUnitInfo = g_pData->GetUnitManager()->GetNPCUnitInfo( unitID );
		ASSERT( pNPCUnitInfo != NULL );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//		const CX2UnitManager::NPCUnitTemplet* ptemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( unitID );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
		if ( pNPCUnitInfo != NULL )
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//		if ( ptemplet != NULL )
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
		{ 
//#ifndef SERV_CREATE_ELITE_FOR_ADMIN
//			CX2GUNPC*
//#endif SERV_CREATE_ELITE_FOR_ADMIN
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			CX2GUNPC* pNPC = CX2GUNPC::CreateGUNPC( index, eTeam, uid, pNPCUnitInfo, eAIType, pOwnerGUUser );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//			CX2GUNPC* pNPC = CX2GUNPC::CreateGUNPC( index, eTeam, ptemplet->m_LuaFileName.c_str(), uid, ptemplet, eAIType, pOwnerGUUser );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
			if( NULL == pNPC )
			{
				ASSERT( "CreateNPC out of memory" );
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
				return NULL;
#else SERV_CREATE_ELITE_FOR_ADMIN
				return;
#endif SERV_CREATE_ELITE_FOR_ADMIN
			}

#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
				pNPC->SetNPCCreateType( eCreateMonsterType );
#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
				if( unitID == CX2UnitManager::NUI_APINK_ANGEL_03_RENA || unitID == CX2UnitManager::NUI_APINK_ANGEL_04_RAVEN || unitID == CX2UnitManager::NUI_APINK_ANGEL_05_EVE )
				{
					pNPC->SetIsAllyEventMonster( true );
				}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	#else // SERV_NEW_DEFENCE_DUNGEON
			if( true == bIsSummonMonster )
				pNPC->SetIsMonsterCard( true );		//���� ī��� ��ȯ�� ���� ����
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			if( NULL != pOwnerGUUser )
			{
	#ifdef SUMMON_MONSTER_CARD_SYSTEM

		#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
				if(  CX2GUNPC::NCT_NONE == eCreateMonsterType )
		#else // SERV_NEW_DEFENCE_DUNGEON
				if( false == bIsSummonMonster )
		#endif // SERV_NEW_DEFENCE_DUNGEON

	#endif SUMMON_MONSTER_CARD_SYSTEM
				{
					pNPC->SetScaleByUnit( pOwnerGUUser->GetScaleByUnit() );
				}

				//{{ JHKang / ������ / 2011/02/21 / NPC�� ��ȯ�� Unit UID ����
#ifdef DUNGEON_RANK_NEW
				pNPC->SetOwnerGameUnit( pOwnerGUUser );
#endif
				//}}
			}

#ifdef SERV_TRAPPING_RANGER_TEST
			if( eAllyTeam != CX2Room::TN_NONE )
			{
				pNPC->SetOwnerGameUnit( pOwnerGUUser );
				pNPC->SetAllyTeam( eAllyTeam );
			}
#endif SERV_TRAPPING_RANGER_TEST

#endif GIANT_UNIT_GIANT_EFFECT_TEST

			pNPC->Init();
			if( false == pNPC->SetPosition( vPos, bRight ) )
			{
				// ���� ��ġ�� ����� ������ �� ���ٸ� ����������.
				ASSERT( !"CreateNPC SetPosition error" );
				SAFE_DELETE_KTDGOBJECT( pNPC ); 
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
				return NULL;
#else SERV_CREATE_ELITE_FOR_ADMIN
				return;
#endif SERV_CREATE_ELITE_FOR_ADMIN
			}


			if ( GetGameType() == CX2Game::GT_DUNGEON )
			{
				CX2DungeonGame* pDungeonGame = (CX2DungeonGame*)this;
				CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

				ASSERT( NULL != pDungeon );
				ASSERT( NULL != pDungeon->GetNowStage() );
				ASSERT( NULL != pDungeon->GetNowStage()->GetNowSubStage() );
				ASSERT( NULL != pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageData() );

				if( NULL != pDungeon )
				{
					map< int, string >::iterator mit;
					if( NULL != pDungeon->GetNowStage() &&
						NULL != pDungeon->GetNowStage()->GetNowSubStage() &&
						NULL != pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageData() )
					{
						mit = pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageData()->m_mapKeyCodeNStartState.find( iKeyCode );
						if ( mit != pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageData()->m_mapKeyCodeNStartState.end() )
						{
							pNPC->StateChange( mit->second.c_str() );
						}
					}
				}
			}
			else if ( GetGameType() == CX2Game::GT_PVP )
			{
				CX2Room::RoomNpcSlot* pSlotData = g_pX2Room->GetNpcSlotData(pNPC->GetUID());
				if( NULL != pSlotData )
				{
					CX2GageManager* pGageManager = CX2GageManager::GetInstance();
					if ( NULL != pGageManager )
					{
						CX2GameUnit* pGameUnitMember 
							= GetNPCUnitByUID( static_cast<int>( pSlotData->m_iNpcUid ) );
						pGageManager->InsertPvpMemberUI( *pSlotData, pNPC );
						pGageManager->UpdatePvpMemberGageData( pSlotData->m_iNpcUid, pNPC );
					}
				}
			}
			
			SetNpcHardLevel( pNPC, level );

			pNPC->InitFullName();

#ifdef FIX_OBSERVER_MODE_CRASH
			if( NULL != GetMyUnit() )
#endif FIX_OBSERVER_MODE_CRASH
				pNPC->InitNameColor( GetMyUnit()->GetUnitLevel() );

			pNPC->SetActiveMonster( bActive );
			pNPC->SetFocusCamera( false );
			pNPC->SetKeyCode( iKeyCode );






#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
			if( CX2GUNPC::NCT_NONE == pNPC->GetNPCCreateType() 
#ifdef FINALITY_SKILL_SYSTEM //��â��
				|| CX2GUNPC::NCT_CHANGESTAGE_REMAIN == pNPC->GetNPCCreateType() 
#endif //FINALITY_SKILL_SYSTEM
				)
	#else // SERV_NEW_DEFENCE_DUNGEON
			if( true != pNPC->GetIsMonsterCard() )
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM
				// ������ ��ȯ�� ������ stat�� ���������� ó���Ѵ�
				SetUserSummonedNPCInfo(eAIType, iAllyUID, pNPC, level, unitID);





//#ifdef TRANSFORMER_TEST
//
//			if( CX2NPCAI::NAT_NO_BRAIN == eAIType )
//			{
//				CX2GUUser* pUser = GetUserUnitByUID( iAllyUID );
//				if( NULL != pUser )
//				{
//					pUser->SetTransformed( true );
//					pUser->SetTransformer( pNPC );
//					pUser->StateChange( pUser->GetCommonState().m_Transformed );
//					pUser->SetShowObject( false );
//
//					pNPC->SetTransformed( true );
//					pNPC->SetTransformerOwnerUID( iAllyUID );
//					pNPC->SetNowHp( pUser->GetNowHp() );
//
//				}
//				else
//				{
//					pNPC->SetNowHp( 0.f );
//				}
//			}
//#endif TRANSFORMER_TEST


#ifdef SERV_INSERT_GLOBAL_SERVER
			if(_kAttribEnchantNpcInfo != NULL)
			{
				pNPC->ApplyEnchantMonster( *_kAttribEnchantNpcInfo );
			}
#endif SERV_INSERT_GLOBAL_SERVER


			//{{ mauntain : ����ȯ [2012.06.14] ���� ī�� ��ȯ ��� - ��ȯ�� NPC�� UID ����
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
			if( NULL != pNPC && CX2GUNPC::NCT_MONSTER_CARD == pNPC->GetNPCCreateType() )
	#else // SERV_NEW_DEFENCE_DUNGEON
			if( NULL != pNPC && true == pNPC->GetIsMonsterCard() )
	#endif // SERV_NEW_DEFENCE_DUNGEON
			{
#ifdef  X2OPTIMIZE_UNITTYPE_BUG_FIX
                CX2GUUser* pUser = pNPC->GetOwnerGameUnit();
#else   X2OPTIMIZE_UNITTYPE_BUG_FIX
				CX2GUUser* pUser = GetUserUnitByUID( pNPC->GetOwnerGameUnitUID() );
#endif  X2OPTIMIZE_UNITTYPE_BUG_FIX

#ifdef NO_COPY_CARD_MONSTER_DATA_SHASHA // ī�� ���ͷ� ���۵� �ϸ� ��� ������ ���� ���� ó��, 13-09-06 kimjh, ���߿� NPC Type �� �߰��ϰų� ���ڸ� �߰��Ͽ� ������ ��!
				if ( pNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_SHASHA_STAGE0 &&
					 pNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_SHASHA_STAGE1 &&
					 pNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_SHASHA_STAGE2 )
#endif // NO_COPY_CARD_MONSTER_DATA_SHASHA // ī�� ���ͷ� ���۵� �ϸ� ��� ������ ���� ���� ó��, 13-09-06 kimjh, ���߿� NPC Type �� �߰��ϰų� ���ڸ� �߰��Ͽ� ������ ��!
				{
					if( NULL != pUser )		//USer���� ��ȯ�� NPC UID ����
					{
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
					// 4������ ���ÿ� ���� �����ֱ����� ���Ϳ� ���� ��´�
					if( static_cast<CX2UnitManager::NPC_UNIT_ID>( EVENT_MONSTER_CARD_ALLY_ID ) == unitID )
					{
						pUser->AccessSummonMonsterCardData().SetSummonMonsterUIDInVec( pNPC->GetUID() );
					}				
#endif //EVENT_MONSTER_CARD_SUMMON_ENEMY
						pUser->AccessSummonMonsterCardData().SetSummonMonsterUID( pNPC->GetUID() );

						pUser->AccessSummonMonsterCardData().SetSummonMonsterTime( pNPC->GetSummonTime() );
						if( NULL != g_pData && NULL != g_pData->GetUIEffectSet() )
						{
							CX2EffectSet::Handle hEffect =  g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_Summon_Pet", (CX2GameUnit*)pNPC );

							if( hEffect != INVALID_EFFECTSET_HANDLE )
						    {
                                if ( CX2EffectSet::EffectSetInstance* pEffectSetInst = g_pData->GetUIEffectSet()->GetEffectSetInstance( hEffect ) )
                                    pEffectSetInst->m_vPosition = pNPC->GetLandPos();
                            }
						}
					}
					else	//��ȯ ��û ��, NOT�� ���� �� User�� ��Ż�ߴٸ� �ش� NPC ����
					{
						pNPC->SetNowHp( 0.f );
					}
				}
			}
#endif SUMMON_MONSTER_CARD_SYSTEM
			//}}

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
			if( unitID == CX2UnitManager::NUI_APINK_ANGEL_03_RENA || unitID == CX2UnitManager::NUI_APINK_ANGEL_04_RAVEN || unitID == CX2UnitManager::NUI_APINK_ANGEL_05_EVE )
			{
				CX2GUUser* pUser = g_pX2Game->GetUserUnitByUID( pNPC->GetOwnerGameUnitUID() );

				if( NULL != pUser )		//USer���� ��ȯ�� NPC UID ����
				{
					pUser->SetAllyEventMonsterUID( pNPC->GetUID() );
				}
				else	//��ȯ ��û ��, NOT�� ���� �� User�� ��Ż�ߴٸ� �ش� NPC ����
				{
					pNPC->SetNowHp( 0.f );
				}
			}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
			AddUnit( (CX2GameUnit*)pNPC );
			SetNPCUnit( index, pNPC );
			g_pKTDXApp->GetDGManager()->AddObjectChain( pNPC );
		}
	}
	else
	{
		CreateNPCData* pCreateNPCData	= new CreateNPCData;
		pCreateNPCData->m_NPCID			= unitID;
		pCreateNPCData->m_Level			= level;
		pCreateNPCData->m_bActive		= bActive;
		pCreateNPCData->m_UID			= uid;
		pCreateNPCData->m_UnitPos		= vPos;
		pCreateNPCData->m_bRight		= bRight;
		pCreateNPCData->m_DelayTime		= fDelayTime;
		pCreateNPCData->m_iKeyCode		= iKeyCode;
		pCreateNPCData->m_eTeamNum		= eTeam;
		pCreateNPCData->m_eAIType		= eAIType;
		pCreateNPCData->m_AllyUID		= iAllyUID;
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON
		pCreateNPCData->m_eCreateMonsterType = eCreateMonsterType;
	#else // SERV_NEW_DEFENCE_DUNGEON
		pCreateNPCData->m_bIsSummonMonster = bIsSummonMonster;
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif //SUMMON_MONSTER_CARD_SYSTEM


		m_CreateNPCDataList.push_back( pCreateNPCData );
	}

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
	return pNPC;
#endif SERV_CREATE_ELITE_FOR_ADMIN
}

#ifdef CHRISTMAS_EVENT_2013
void CX2Game::Handler_EGS_NPC_UNIT_CREATE_REQ( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, 
	D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode,
	CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_NONE*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
	UidType iAllyUID /*= -1*/, bool bElite /* = false */
	, CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */ 
	, CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType /*= false */			//���� ī��� ��ȯ�Ǿ������� ���� ����
	)
{
	if ( m_bGameEnd == true )
		return;

	KNPCUnitReq kNPCUnitReq;
	kNPCUnitReq.Init();

	kNPCUnitReq.m_NPCID			= unitID;
	kNPCUnitReq.m_Level			= level;
	kNPCUnitReq.m_bActive		= bActive;
	kNPCUnitReq.m_vPos.x		= vPos.x;
	kNPCUnitReq.m_vPos.y		= vPos.y;
	kNPCUnitReq.m_vPos.z		= vPos.z;
	kNPCUnitReq.m_bIsRight		= bRight;
	kNPCUnitReq.m_bNoDrop		= bNoDrop;
	kNPCUnitReq.m_KeyCode		= (char)iKeyCode;
	kNPCUnitReq.m_cTeamNum		= (char) eTeam;
	kNPCUnitReq.m_cAIType		= (char) eAIType;
	kNPCUnitReq.m_iAllyUID		= iAllyUID;
	kNPCUnitReq.m_cAllyTeam		= static_cast<char>( eAllyTeam );
	kNPCUnitReq.m_cCreateMonsterType |= eCreateMonsterType;		//���� ī��� ��ȯ�Ǿ������� ���� ����

	if( eTeam != CX2Room::TN_MONSTER )
		kNPCUnitReq.m_bAllyNpc = true;

	kNPCUnitReq.m_bAttributeNpc	= bElite;
	kNPCUnitReq.m_fDelayTime	= fDelayTime;

	KEGS_NPC_UNIT_CREATE_REQ kPacket;
	kPacket.m_vecNPCUnitReq.push_back( kNPCUnitReq );

	g_pData->GetServerProtocol()->SendPacket( EGS_NPC_UNIT_CREATE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_NPC_UNIT_CREATE_ACK, 60.f );
}
#endif CHRISTMAS_EVENT_2013

#ifdef SERV_INSERT_GLOBAL_SERVER
void CX2Game::CreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, 
						   D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode,
						   CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_NONE*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
						   UidType iAllyUID /*= -1*/, bool bElite /* = false */
#ifdef	SERV_TRAPPING_RANGER_TEST
						   , CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */ 
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON
						   , CX2GUNPC::NPC_CREATE_TYPE eCreateMonsterType /*= false */			//���� ī��� ��ȯ�Ǿ������� ���� ����
	#else // SERV_NEW_DEFENCE_DUNGEON
						   , bool bIsSummonMonster /*= false */			//���� ī��� ��ȯ�Ǿ������� ���� ����
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM
						   )
#else
void CX2Game::CreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, 
						   D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode,
						   CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_NONE*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
						   UidType iAllyUID /*= -1*/
#ifdef	SERV_TRAPPING_RANGER_TEST
						   , CX2Room::TEAM_NUM eAllyTeam /*= CX2Room::TN_NONE */ 
#endif	SERV_TRAPPING_RANGER_TEST
						   )
#endif SERV_INSERT_GLOBAL_SERVER
{
	if( IsHost() == false )
		return;

	if ( m_bGameEnd == true )
		return;

	KNPCUnitReq kNPCUnitReq;
	kNPCUnitReq.Init();

	kNPCUnitReq.m_NPCID			= unitID;
	kNPCUnitReq.m_Level			= level;
	kNPCUnitReq.m_bActive		= bActive;
	kNPCUnitReq.m_vPos.x		= vPos.x;
	kNPCUnitReq.m_vPos.y		= vPos.y;
	kNPCUnitReq.m_vPos.z		= vPos.z;
	kNPCUnitReq.m_bIsRight		= bRight;
	kNPCUnitReq.m_bNoDrop		= bNoDrop;
	kNPCUnitReq.m_KeyCode		= (char)iKeyCode;
	kNPCUnitReq.m_cTeamNum		= (char) eTeam;
	kNPCUnitReq.m_cAIType		= (char) eAIType;
	kNPCUnitReq.m_iAllyUID		= iAllyUID;
#ifdef	SERV_TRAPPING_RANGER_TEST
	kNPCUnitReq.m_cAllyTeam		= static_cast<char>( eAllyTeam );
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_NEW_DEFENCE_DUNGEON
	kNPCUnitReq.m_cCreateMonsterType |= eCreateMonsterType;		//���� ī��� ��ȯ�Ǿ������� ���� ����
#else // SERV_NEW_DEFENCE_DUNGEON
	kNPCUnitReq.m_bIsMonsterCard = bIsSummonMonster;		//���� ī��� ��ȯ�Ǿ������� ���� ����
#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_ALLY_NPC
	if( eTeam != CX2Room::TN_MONSTER )
		kNPCUnitReq.m_bAllyNpc = true;
#endif

#ifdef SERV_INSERT_GLOBAL_SERVER
	kNPCUnitReq.m_bAttributeNpc	= bElite;
#endif SERV_INSERT_GLOBAL_SERVER

#ifdef FIX_CREATE_NPC_DELAY_TIME
	kNPCUnitReq.m_fDelayTime	= fDelayTime;
#endif //FIX_CREATE_NPC_DELAY_TIME

#ifndef X2TOOL
	KEGS_NPC_UNIT_CREATE_REQ kPacket;
	kPacket.m_vecNPCUnitReq.push_back( kNPCUnitReq );

	g_pData->GetServerProtocol()->SendPacket( EGS_NPC_UNIT_CREATE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_NPC_UNIT_CREATE_ACK, 60.f );
#else	
	CreateNPC( (CX2UnitManager::NPC_UNIT_ID)kNPCUnitReq.m_NPCID, 
		kNPCUnitReq.m_Level, kNPCUnitReq.m_bActive, kNPCUnitReq.m_UID, 
		D3DXVECTOR3( kNPCUnitReq.m_vPos.x, kNPCUnitReq.m_vPos.y, kNPCUnitReq.m_vPos.z ), 
		kNPCUnitReq.m_bIsRight, kNPCUnitReq.m_fDelayTime, (int) kNPCUnitReq.m_KeyCode, 
		(CX2Room::TEAM_NUM) kNPCUnitReq.m_cTeamNum,	(CX2NPCAI::NPC_AI_TYPE) kNPCUnitReq.m_cAIType,
		kNPCUnitReq.m_iAllyUID
#ifdef	SERV_INSERT_GLOBAL_SERVER
		, false
#endif	SERV_INSERT_GLOBAL_SERVER
#ifdef	SERV_TRAPPING_RANGER_TEST
		, static_cast<CX2Room::TEAM_NUM>(kNPCUnitReq.m_cAllyTeam)
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SUMMON_MONSTER_CARD_SYSTEM
#ifdef SERV_NEW_DEFENCE_DUNGEON
		, static_cast<CX2GUNPC::NPC_CREATE_TYPE>( kNPCUnitReq.m_cCreateMonsterType )		//���� ī��� ��ȯ�Ǿ������� ���� ����
#else // SERV_NEW_DEFENCE_DUNGEON
		, kNPCUnitReq.m_cCreateMonsterType			//���� ī��� ��ȯ�Ǿ������� ���� ����
#endif // SERV_NEW_DEFENCE_DUNGEON
#endif SUMMON_MONSTER_CARD_SYSTEM
		);
#endif // X2TOOL
}

void CX2Game::CreateNPCReq_LUA( int unitID, int level, bool bActive, 
							   D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop )
{
	if( IsHost() == false )
		return;

#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return;
	}
#endif LIGHT_OPERATOR_ACCOUNT

	CreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, fDelayTime, bNoDrop );
}

#ifdef SERV_INSERT_GLOBAL_SERVER
void CX2Game::CreateNPCReqElite( int unitID, int level, bool bActive, 
								D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, bool bElite /* = false */ )
{
	if( IsHost() == false )
		return;
	CreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, vfDelayTimeNKeyCode.x, bNoDrop, (int)vfDelayTimeNKeyCode.y, CX2Room::TN_MONSTER, CX2NPCAI::NAT_NORMAL, -1, bElite );
}
#endif SERV_INSERT_GLOBAL_SERVER

#ifdef SEASON3_MONSTER_2011_7
void CX2Game::CreateNPCReqAlly( int unitID, int level, bool bActive, 
	D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, CX2Room::TEAM_NUM eTeam/* = CX2Room::TN_RED*/ )
{
	if( IsHost() == false )
		return;
	CreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, vfDelayTimeNKeyCode.x, bNoDrop, (int)vfDelayTimeNKeyCode.y, eTeam, CX2NPCAI::NAT_ALLY, -1, false );
}
#endif SEASON3_MONSTER_2011_7


void CX2Game::CreateNPCReq_LUA2( int unitID, int level, bool bActive, 
								D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop )
{
	if( IsHost() == false )
		return;

	CreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, vfDelayTimeNKeyCode.x, bNoDrop, (int)vfDelayTimeNKeyCode.y );
}


void CX2Game::PushCreateNPCReq( CX2UnitManager::NPC_UNIT_ID unitID, int level, bool bActive, 
							   D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode /*= 0*/,
							   CX2Room::TEAM_NUM eTeam /*= CX2Room::TN_MONSTER*/, CX2NPCAI::NPC_AI_TYPE eAIType /*= CX2NPCAI::NAT_NORMAL*/,
							   UidType iAllyUID /*= -1*/ )
{
	if( IsHost() == false )
		return;

	KNPCUnitReq kNPCUnitReq;
	kNPCUnitReq.Init();

	kNPCUnitReq.m_NPCID			= unitID;
	kNPCUnitReq.m_Level			= level;
	kNPCUnitReq.m_bActive		= bActive;
	kNPCUnitReq.m_vPos.x		= vPos.x;
	kNPCUnitReq.m_vPos.y		= vPos.y;
	kNPCUnitReq.m_vPos.z		= vPos.z;
	kNPCUnitReq.m_bIsRight		= bRight;
	kNPCUnitReq.m_fDelayTime	= fDelayTime;
	kNPCUnitReq.m_bNoDrop		= bNoDrop;
	kNPCUnitReq.m_KeyCode		= (char) iKeyCode;
	kNPCUnitReq.m_cTeamNum		= (char) eTeam;
	kNPCUnitReq.m_cAIType		= (char) eAIType;
	kNPCUnitReq.m_iAllyUID		= iAllyUID;

#ifdef SERV_ALLY_NPC
	if( eTeam != CX2Room::TN_MONSTER )
		kNPCUnitReq.m_bAllyNpc = true;
#endif


	m_PushCreateNPCReqList.m_vecNPCUnitReq.push_back( kNPCUnitReq );
}

void CX2Game::PushCreateNPCReq_Lua( int unitID, int level, bool bActive, 
								   D3DXVECTOR3 vPos, bool bRight, float fDelayTime, bool bNoDrop, int iKeyCode )
{
	if( IsHost() == false )
		return;

	PushCreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, fDelayTime, bNoDrop, iKeyCode );
}

void CX2Game::FlushCreateNPCReq()
{
	if( IsHost() == false )
		return;

#ifndef X2TOOL
	g_pData->GetServerProtocol()->SendPacket( EGS_NPC_UNIT_CREATE_REQ, m_PushCreateNPCReqList ); 
	g_pMain->AddServerPacket( EGS_NPC_UNIT_CREATE_ACK, 60.f ); 
#else
	for( int i = 0; i < (int)m_PushCreateNPCReqList.m_vecNPCUnitReq.size(); i++ )
	{
		KNPCUnitReq* pKNPCUnitNot = &m_PushCreateNPCReqList.m_vecNPCUnitReq[i];
		CreateNPC( (CX2UnitManager::NPC_UNIT_ID)pKNPCUnitNot->m_NPCID, pKNPCUnitNot->m_Level,
			pKNPCUnitNot->m_bActive, pKNPCUnitNot->m_UID, 
			D3DXVECTOR3( pKNPCUnitNot->m_vPos.x, pKNPCUnitNot->m_vPos.y, pKNPCUnitNot->m_vPos.z ), 
			pKNPCUnitNot->m_bIsRight, pKNPCUnitNot->m_fDelayTime, (int) pKNPCUnitNot->m_KeyCode, 
			(CX2Room::TEAM_NUM) pKNPCUnitNot->m_cTeamNum, (CX2NPCAI::NPC_AI_TYPE) pKNPCUnitNot->m_cAIType,
			pKNPCUnitNot->m_iAllyUID
#ifdef	SERV_INSERT_GLOBAL_SERVER
			, false
#endif	SERV_INSERT_GLOBAL_SERVER
#ifdef	SERV_TRAPPING_RANGER_TEST
			, static_cast<CX2Room::TEAM_NUM>(pKNPCUnitNot->m_cAllyTeam)
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SUMMON_MONSTER_CARD_SYSTEM
#ifdef SERV_NEW_DEFENCE_DUNGEON
			, static_cast<CX2GUNPC::NPC_CREATE_TYPE>( pKNPCUnitNot->m_cCreateMonsterType )		//���� ī��� ��ȯ�Ǿ������� ���� ����
#else // SERV_NEW_DEFENCE_DUNGEON
			, pKNPCUnitNot->m_cCreateMonsterType			//���� ī��� ��ȯ�Ǿ������� ���� ����
#endif // SERV_NEW_DEFENCE_DUNGEON
#endif SUMMON_MONSTER_CARD_SYSTEM
			);

	}

	//{{ kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
#ifdef SERV_SECRET_HELL
	// �������� ���͸� ���Ƿ� �����ؼ� �������� ��
	// �ش� ���Ͱ� substage clear ���ǿ� �ִ� ���
	// Ŭ���� ������ �ش� ���� original count�� update ������
	if ( GetGameType() == GT_DUNGEON )
	{
		CX2Dungeon* pDungeon = static_cast< CX2DungeonGame* >( this )->GetDungeon();

		if ( pDungeon != NULL &&
			pDungeon->GetNowStage() != NULL &&
			pDungeon->GetNowStage()->GetNowSubStage() != NULL )
		{
			pDungeon->GetNowStage()->GetNowSubStage()->SetMonsterCountForSubStageClearCheck();
		}
	}
#endif SERV_SECRET_HELL
#endif //X2TOOL

	m_PushCreateNPCReqList.m_vecNPCUnitReq.resize(0);
}

#ifdef CREATE_NPC_REQ_FULL_ARGUMENTS
void CX2Game::CreateNPCReq_LUA3( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, int iTeam, int iAIType, UidType iAllyUID )
{
	if( IsHost() == false )
		return;

	CreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, vfDelayTimeNKeyCode.x, bNoDrop, (int)vfDelayTimeNKeyCode.y, (CX2Room::TEAM_NUM)iTeam, (CX2NPCAI::NPC_AI_TYPE)iAIType, iAllyUID );
}

void CX2Game::PushCreateNPCReq_LUA3( int unitID, int level, bool bActive, D3DXVECTOR3 vPos, bool bRight, D3DXVECTOR3 vfDelayTimeNKeyCode, bool bNoDrop, int iTeam, int iAIType, UidType iAllyUID )
{
	if( IsHost() == false )
		return;

	PushCreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)unitID, level, bActive, vPos, bRight, vfDelayTimeNKeyCode.x, bNoDrop, (int)vfDelayTimeNKeyCode.y, (CX2Room::TEAM_NUM)iTeam, (CX2NPCAI::NPC_AI_TYPE)iAIType, iAllyUID );
}
#endif CREATE_NPC_REQ_FULL_ARGUMENTS



#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS


bool CX2Game::Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �߰� ���� ���忡 ���� ACK
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_NPC_UNIT_CREATE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;	
}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

bool CX2Game::Handler_EGS_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_NPC_UNIT_CREATE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;	
}

bool CX2Game::Handler_EGS_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NPC_UNIT_CREATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );


	for( int i = 0; i < (int)kEvent.m_vecNPCUnitAck.size(); i++ )
	{
		KNPCUnitNot* pKNPCUnitNot = &kEvent.m_vecNPCUnitAck[i];
		CreateNPC( (CX2UnitManager::NPC_UNIT_ID)pKNPCUnitNot->m_kNPCUnitReq.m_NPCID, 
			pKNPCUnitNot->m_kNPCUnitReq.m_Level,
			pKNPCUnitNot->m_kNPCUnitReq.m_bActive,
			pKNPCUnitNot->m_kNPCUnitReq.m_UID, 
			D3DXVECTOR3( pKNPCUnitNot->m_kNPCUnitReq.m_vPos.x, pKNPCUnitNot->m_kNPCUnitReq.m_vPos.y, pKNPCUnitNot->m_kNPCUnitReq.m_vPos.z ), 
			pKNPCUnitNot->m_kNPCUnitReq.m_bIsRight,
			pKNPCUnitNot->m_kNPCUnitReq.m_fDelayTime,
			(int) pKNPCUnitNot->m_kNPCUnitReq.m_KeyCode, 
			(CX2Room::TEAM_NUM) pKNPCUnitNot->m_kNPCUnitReq.m_cTeamNum,
			(CX2NPCAI::NPC_AI_TYPE) pKNPCUnitNot->m_kNPCUnitReq.m_cAIType,
			pKNPCUnitNot->m_kNPCUnitReq.m_iAllyUID
#ifdef	SERV_INSERT_GLOBAL_SERVER
			, false
#endif	SERV_INSERT_GLOBAL_SERVER
#ifdef	SERV_TRAPPING_RANGER_TEST
			, static_cast<CX2Room::TEAM_NUM>(pKNPCUnitNot->m_kNPCUnitReq.m_cAllyTeam)
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON
			, static_cast<CX2GUNPC::NPC_CREATE_TYPE>( pKNPCUnitNot->m_kNPCUnitReq.m_cCreateMonsterType )		//���� ī��� ��ȯ�Ǿ������� ���� ����
	#else // SERV_NEW_DEFENCE_DUNGEON
			, pKNPCUnitNot->m_kNPCUnitReq.m_bIsMonsterCard			//���� ī��� ��ȯ�Ǿ������� ���� ����
	#endif // SERV_NEW_DEFENCE_DUNGEON
			
#endif SUMMON_MONSTER_CARD_SYSTEM
			);

	}

//{{ kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
#ifdef SERV_SECRET_HELL
	// �������� ���͸� ���Ƿ� �����ؼ� �������� ��
	// �ش� ���Ͱ� substage clear ���ǿ� �ִ� ���
	// Ŭ���� ������ �ش� ���� original count�� update ������
	if ( GetGameType() == GT_DUNGEON )
	{
		CX2Dungeon* pDungeon = static_cast< CX2DungeonGame* >( this )->GetDungeon();

		if ( pDungeon != NULL &&
			 pDungeon->GetNowStage() != NULL &&
			 pDungeon->GetNowStage()->GetNowSubStage() != NULL )
		{
			pDungeon->GetNowStage()->GetNowSubStage()->SetMonsterCountForSubStageClearCheck();
		}
	}
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
	return true;
}

#ifdef FINALITY_SKILL_SYSTEM //JHKang
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
bool CX2Game::Handler_EGS_USE_FINALITY_SKILL_REQ()
{
	KEGS_USE_FINALITY_SKILL_REQ kPacket;

	kPacket.m_iItemUID = 0;
	kPacket.m_bNoConsume = true;

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_FINALITY_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_USE_FINALITY_SKILL_ACK );

	return true;
}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

bool CX2Game::Handler_EGS_USE_FINALITY_SKILL_REQ( UidType itemUID_ )
{
	KEGS_USE_FINALITY_SKILL_REQ kPacket;

	kPacket.m_iItemUID = itemUID_;
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
	kPacket.m_bNoConsume = false;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
	
	g_pData->GetServerProtocol()->SendPacket( EGS_USE_FINALITY_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_USE_FINALITY_SKILL_ACK );

	return true;
}

bool CX2Game::Handler_EGS_USE_FINALITY_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_FINALITY_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_USE_FINALITY_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
			if( true == kEvent.m_bNoConsume )
				return true;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

			const int iItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

			switch ( iItemNum )
			{
			case 100:
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_26389 ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
				break;

			case 50:
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_26390 ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
				break;

			case 10:
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_26391 ), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0,1,0,1), L"#C00FF00", false );
				break;

			default:
				break;
			}
			return true;
		}
	}

	return false;	
}
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_INSERT_GLOBAL_SERVER // oasis907 : ����� [2011.4.20]
bool CX2Game::Handler_EGS_CREATE_ATTRIB_NPC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_ATTRIB_NPC_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );


	for( int i = 0; i < (int)kEvent.m_vecAttribNPCUnit.size(); i++ )
	{
		KAttribNpcUnitInfo * pKAttribNpcUnitNot = &kEvent.m_vecAttribNPCUnit[i];
		CreateNPC( (CX2UnitManager::NPC_UNIT_ID)pKAttribNpcUnitNot->m_sNPCUnitReq.m_NPCID, 
			pKAttribNpcUnitNot->m_sNPCUnitReq.m_Level,
			pKAttribNpcUnitNot->m_sNPCUnitReq.m_bActive,
			pKAttribNpcUnitNot->m_sNPCUnitReq.m_UID, 
			D3DXVECTOR3( pKAttribNpcUnitNot->m_sNPCUnitReq.m_vPos.x, pKAttribNpcUnitNot->m_sNPCUnitReq.m_vPos.y, pKAttribNpcUnitNot->m_sNPCUnitReq.m_vPos.z ), 
			pKAttribNpcUnitNot->m_sNPCUnitReq.m_bIsRight,
			pKAttribNpcUnitNot->m_sNPCUnitReq.m_fDelayTime,
			(int) pKAttribNpcUnitNot->m_sNPCUnitReq.m_KeyCode, 
			(CX2Room::TEAM_NUM) pKAttribNpcUnitNot->m_sNPCUnitReq.m_cTeamNum,
			(CX2NPCAI::NPC_AI_TYPE) pKAttribNpcUnitNot->m_sNPCUnitReq.m_cAIType,
			pKAttribNpcUnitNot->m_sNPCUnitReq.m_iAllyUID, &pKAttribNpcUnitNot->m_sAttribEnchantNpcInfo );

	}

	//{{ kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
#ifdef SERV_SECRET_HELL
	// �������� ���͸� ���Ƿ� �����ؼ� �������� ��
	// �ش� ���Ͱ� substage clear ���ǿ� �ִ� ���
	// Ŭ���� ������ �ش� ���� original count�� update ������
	if ( GetGameType() == GT_DUNGEON )
	{
		CX2Dungeon* pDungeon = static_cast< CX2DungeonGame* >( this )->GetDungeon();

		if ( pDungeon != NULL &&
			pDungeon->GetNowStage() != NULL &&
			pDungeon->GetNowStage()->GetNowSubStage() != NULL )
		{
			pDungeon->GetNowStage()->GetNowSubStage()->SetMonsterCountForSubStageClearCheck();
		}
	}
#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
	return true;
}
#endif SERV_INSERT_GLOBAL_SERVER



bool CX2Game::Handler_EGS_NPC_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

ELSWORD_VIRTUALIZER_START
	//if( g_pMain->DeleteServerPacket( EGS_NPC_UNIT_DIE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	--m_iNpcDiePacket;

ELSWORD_VIRTUALIZER_END

	return false;	
}

/*virtual*/ bool CX2Game::Handler_EGS_NPC_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NPC_UNIT_DIE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( kEvent.m_nDieNPCUID );
	if( pCX2GUNPC != NULL )
	{
#ifdef  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
        pCX2GUNPC->MarkNPCUnitDieNotReceived();
#else   X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
        pCX2GUNPC->SetNowHp( 0.f );
#endif  X2OPTIMIZE_ROBUST_NPC_FINAL_STATE

		map<int, int>::iterator it;
		it = m_mapKilledNPCKeyCode.find( pCX2GUNPC->GetKeyCode() );
		if( it != m_mapKilledNPCKeyCode.end() )
		{
			m_mapKilledNPCKeyCode[ pCX2GUNPC->GetKeyCode() ] += 1;
		}
		else
		{
			m_mapKilledNPCKeyCode[ pCX2GUNPC->GetKeyCode() ] = 1;
		}

#ifdef SERV_PVP_NEW_SYSTEM
		if( GetMyUnit() != NULL && pCX2GUNPC->IsPvpBot() == false )
#else
		if( GetMyUnit() != NULL )
#endif
		{
			ProcessExpListByNpcUnitDie( kEvent );

		}

#ifdef ARA_CHARACTER_BASE
		ProcessIKillThisNpc( kEvent );
#endif
			
#ifdef	SERV_TRAPPING_RANGER_TEST
		if( pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_EVOKE_TRAP_LIRE 
#ifdef ADDITIONAL_MEMO
			|| pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO 
#endif
			)
		{
			CX2GUUser* pUser = static_cast<CX2GUUser*>( pCX2GUNPC->GetOwnerGameUnit() );
			if( pUser != NULL && pUser->GetUnit()->GetType() == CX2Unit::UT_LIRE )
			{
				CX2GULire_ElvenRanger* pLire = static_cast<CX2GULire_ElvenRanger*>( pUser );
				pLire->DeleteEvokeNPCUID( (UidType) pCX2GUNPC->GetUID() );
			}
		}
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		if( pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_RENA_ANGER_OF_ELF_DELAY 
			||  pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_RENA_ANGER_OF_ELF )
		{
			CX2GUUser* pUser = static_cast<CX2GUUser*>( pCX2GUNPC->GetOwnerGameUnit() );
			if( pUser != NULL && pUser->GetUnit()->GetType() == CX2Unit::UT_LIRE )
			{
				CX2GULire_ElvenRanger* pLire = static_cast<CX2GULire_ElvenRanger*>( pUser );
				if( NULL != pLire )
					pLire->DeleteAngerOfElfTrapUID( static_cast<UidType>( pCX2GUNPC->GetUID() ) );
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef	SERV_CHUNG_TACTICAL_TROOPER
		if( pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_REMOTE_MORTAR )
		{
			CX2GUUser* pUser = static_cast<CX2GUUser*>( pCX2GUNPC->GetOwnerGameUnit() );
			if( pUser != NULL && pUser->GetUnit()->GetType() == CX2Unit::UT_CHUNG )
			{
				CX2GUChung* pChung = static_cast<CX2GUChung*>(pUser);

				if( NULL != pChung )
				{
					if( pChung->GetRemoteMortarNPCUID() == pCX2GUNPC->GetUnitUID() )	/// ��Ŷ ���� ���� ���̵� ��ϵǾ��ٸ�, ���̵� ������ ���� ����
					{
						pChung->DeleteRemoteMortarNPCUID();	
					}
				}
			}
		}
#endif	SERV_CHUNG_TACTICAL_TROOPER

#ifdef FINALITY_SKILL_SYSTEM //��â��
	//	ProcessSummonNPCDieEvent( kEvent );
#endif //FINALITY_SKILL_SYSTEM

#ifdef ADD_RENA_SYSTEM //��â��
		//������ ��ȯ�� npc�� �׾��� ��� ������ �������� ������ ��츦 �����
		//��ų ���� �����͸� üũ�ؼ� �����ش�
		CX2GUUser* pUser = static_cast<CX2GUUser*>( pCX2GUNPC->GetOwnerGameUnit() );
		if( NULL != pUser)
		{
			if( true == pUser->CheckDamageRelateSkillData( pCX2GUNPC->GetRelateSkillData() ) )
				pUser->DeleteDamageRelateSkillData( pCX2GUNPC->GetRelateSkillData() );
		}
#endif //ADD_RENA_SYSTEM
	}

	return true;
}


bool CX2Game::CheckEnableKeyProcess()
{
	if( GetOpenChatBox() == true )
		return false;
	
	// edit box�� ��Ŀ���� �ִ��� �˻��Ѵ�.
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFocusInEditBox() == true )
		return false;

	//if ( g_pData->GetMessenger() != NULL && g_pData->GetMessenger()->GetOpen() == true )
	//	return false;

#if !defined(WORLD_TOOL) && !defined(X2TOOL)
	if( g_pData->GetMessenger()->GetOpenChatBox() == true)
	{
		CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
		if ( pControl != NULL )
		{
			if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
				pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
			{
				return false;
			}
		}
	}	
#endif WORLD_TOOL

	return true;
}


bool CX2Game::NPCUnitDieReq( const KEGS_NPC_UNIT_DIE_REQ& kPacket_ )
{
	// checked
	ELSWORD_VIRTUALIZER_START

	if( IsHost() == false )
		return false;	

	THEMIDA_ENCODE_START
		
	g_pData->GetServerProtocol()->SendPacket( EGS_NPC_UNIT_DIE_REQ, kPacket_ );
	
	THEMIDA_ENCODE_END

	ELSWORD_VIRTUALIZER_END
}

#ifdef BONUS_DROP
bool CX2Game::NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, bool bIsAllyNPC, bool bBounsDrop /*= false*/ )
{

	// checked
	ELSWORD_VIRTUALIZER_START

#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
		AHNHS_PROTECT_FUNCTION
#endif

	if( bIsAllyNPC == true )
	{
		return NPCUnitDieReq( cAttUnitType, iNPCUID, uiAttUnit, uiMDAttUnit, diePos, KEGS_NPC_UNIT_DIE_REQ::NDS_ALLY );
	}
	else if ( bBounsDrop == true )
	{
		return NPCUnitDieReq( cAttUnitType, iNPCUID, uiAttUnit, uiMDAttUnit, diePos, KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP );
	}
	else
	{
		return NPCUnitDieReq( cAttUnitType, iNPCUID, uiAttUnit, uiMDAttUnit, diePos, KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER );
	}

	++m_iNpcDiePacket;

	ELSWORD_VIRTUALIZER_END
}

bool CX2Game::NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, KEGS_NPC_UNIT_DIE_REQ::NPC_DIE_STATE eDieState )
{

	// checked
	ELSWORD_VIRTUALIZER_START



	if( IsHost() == false )
		return false;	

	THEMIDA_ENCODE_START

	KEGS_NPC_UNIT_DIE_REQ kPacket;
	kPacket.m_cAttUnitType	= cAttUnitType;
	kPacket.m_nDieNPCUID	= iNPCUID;
	kPacket.m_uiAttUnit		= uiAttUnit;
	kPacket.m_uiMDAttUnit	= uiMDAttUnit;
	kPacket.m_DiePos.x		= diePos.x;
	kPacket.m_DiePos.y		= diePos.y;
	kPacket.m_DiePos.z		= diePos.z;
	kPacket.m_cNpcDieState	= eDieState;

#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
	//AHNHS_PROTECT_FUNCTION
#endif
	g_pData->GetServerProtocol()->SendPacket( EGS_NPC_UNIT_DIE_REQ, kPacket );
	//g_pMain->AddServerPacket( EGS_NPC_UNIT_DIE_ACK, 60.f );

	++m_iNpcDiePacket;

	THEMIDA_ENCODE_END

	ELSWORD_VIRTUALIZER_END


	return true;

}

#else // BONUS_DROP

bool CX2Game::NPCUnitDieReq( char cAttUnitType, int iNPCUID, UidType uiAttUnit, UidType uiMDAttUnit, D3DXVECTOR3 diePos, bool bIsAllyNPC )
{

	// checked 
	ELSWORD_VIRTUALIZER_START

	if( IsHost() == false )
		return false;

	THEMIDA_ENCODE_START


	KEGS_NPC_UNIT_DIE_REQ kPacket;
	kPacket.m_cAttUnitType	= cAttUnitType;
	kPacket.m_nDieNPCUID	= iNPCUID;
	kPacket.m_uiAttUnit		= uiAttUnit;
	kPacket.m_uiMDAttUnit	= uiMDAttUnit;
	kPacket.m_DiePos.x		= diePos.x;
	kPacket.m_DiePos.y		= diePos.y;
	kPacket.m_DiePos.z		= diePos.z;
	kPacket.m_cNpcDieState	= bIsAllyNPC;

	g_pData->GetServerProtocol()->SendPacket( EGS_NPC_UNIT_DIE_REQ, kPacket );
	//g_pMain->AddServerPacket( EGS_NPC_UNIT_DIE_ACK, 60.f );

	THEMIDA_ENCODE_END

	ELSWORD_VIRTUALIZER_END


	return true;



}

#endif // BONUS_DROP

/*virtual*/ void CX2Game::UserUnitDieNot( KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT )
{
	return;	
}

bool CX2Game::Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( KEGS_USER_UNIT_DIE_COMPLETE_NOT& kEGS_USER_UNIT_DIE_COMPLETE_NOT )
{
	CX2GUUser* pCX2GUUser = GetUserUnitByUID( kEGS_USER_UNIT_DIE_COMPLETE_NOT.m_UnitUID );
	if( pCX2GUUser != NULL )
	{
		//{{ robobeg : 2008-10-21
		//pCX2GUUser->SetShow( false );
		/// kimhc // 2012-11-27 // ������ ��Ȱ�� ���Ŀ� �� ��Ŷ�� �޴� ��찡 ���� ������ ���� �Ʒ��� ���� ó����
		if ( pCX2GUUser->GetNowStateID() == CX2GameUnit::GUSI_DIE && !(pCX2GUUser->GetNowHp() > 0.0f) )
			pCX2GUUser->SetShowObject( false );
 		//}} robobeg : 2008-10-21		

#ifdef UDP_CAN_NOT_SEND_USER_KICK // 2012.06.19 lygan_������ // ���� ������ �ϴ� üũ���� ����
		if(g_pInstanceData->GetUDPKickOff() == false )
		{
			if(GetGameType() == GT_PVP )
			{
#ifdef LOG_UDP_CHECK
				if( g_pInstanceData->m_mapCheckUDPTimerTemp.empty() == false )
				{
					std::map< UidType, boost::timer >::iterator mit;
					mit = g_pInstanceData->m_mapCheckUDPTimerTemp.find(kEGS_USER_UNIT_DIE_COMPLETE_NOT.m_UnitUID);
					if( mit != g_pInstanceData->m_mapCheckUDPTimerTemp.end() )
					{
						g_pInstanceData->m_mapCheckUDPTimerTemp.erase(kEGS_USER_UNIT_DIE_COMPLETE_NOT.m_UnitUID);

					}
				}
#endif //LOG_UDP_CHECK
				if( g_pInstanceData->m_mapCheckUDPTimer.empty() == false )
				{
					std::map< UidType, boost::timer >::iterator mit;
					mit = g_pInstanceData->m_mapCheckUDPTimer.find(kEGS_USER_UNIT_DIE_COMPLETE_NOT.m_UnitUID);
					if( mit != g_pInstanceData->m_mapCheckUDPTimer.end() )
					{
						g_pInstanceData->m_mapCheckUDPTimer.erase(kEGS_USER_UNIT_DIE_COMPLETE_NOT.m_UnitUID);

					}
				}
			}
		}
#endif //UDP_CAN_NOT_SEND_USER_KICK
	}
	return true;
}

bool CX2Game::Handler_EGS_UPDATE_UNIT_INFO_NOT( KEGS_UPDATE_UNIT_INFO_NOT& kEGS_UPDATE_UNIT_INFO_NOT  )
{
	if ( g_pX2Room != NULL )
	{
		for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			if ( pSlotData->m_pUnit->GetUID() == kEGS_UPDATE_UNIT_INFO_NOT.m_kUnitInfo.m_nUnitUID )
			{
				pSlotData->m_pUnit->Reset( kEGS_UPDATE_UNIT_INFO_NOT.m_kUnitInfo );
				pSlotData->m_pUnit->AccessInventory().UpdateInventorySlotList( kEGS_UPDATE_UNIT_INFO_NOT.m_vecKInventorySlotInfo );


				// �������� �����鼭 ����ġ ������Ʈ�Ǿ��� ������ ������ �ǽð� ����ġ �ӽ÷� �����ϴ� �����͸� �ʱ�ȭ�Ѵ�.
				m_GetEXPInGame = 0;


				return true;
			}
		}
	}

	return false;
}

void CX2Game::ProcessExpListByNpcUnitDie( const KEGS_NPC_UNIT_DIE_NOT& kEvent )
{
	if ( NULL != GetMyUnit() )
	{
		std::map<UidType,KEXPData>::const_iterator iter = kEvent.m_EXPList.find( GetMyUnit()->GetUnitUID() );
		if( iter != kEvent.m_EXPList.end() )
		{
			KEXPData kEXPData = iter->second;

			if( 0 == kEXPData.m_iEXP )
			{
#ifdef HARDCODING_STRING_TO_INDEX
				g_pChatBox->AddChatLogObtain(GET_STRING(STR_ID_24594), CX2ChatBox::COT_EXP);
#else
				g_pChatBox->AddChatLogObtain(L"����ġ�� ȹ�� �� �� �����ϴ�.", CX2ChatBox::COT_EXP);
#endif HARDCODING_STRING_TO_INDEX
				return;
			}
			//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ����ġ ȹ��
#ifdef REAL_TIME_ELSWORD
			CX2Unit::UnitData* pMyUnitData = NULL;
			wstringstream strstm;

#ifdef SERV_NEW_EVENT_TYPES
			int iMaxEXP = g_pData->GetEXPTable()->GetEXPData( _CONST_X2GAME_::g_iMaxLevel ).m_nTotalExp;
			if( g_pInstanceData != NULL )
				iMaxEXP = g_pData->GetEXPTable()->GetEXPData( g_pInstanceData->GetMaxLevel() ).m_nTotalExp;
#else SERV_NEW_EVENT_TYPES
			const int iMaxEXP = g_pData->GetEXPTable()->GetEXPData( _CONST_X2GAME_::g_iMaxLevel ).m_nTotalExp;
#endif SERV_NEW_EVENT_TYPES
			pMyUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			if ( pMyUnitData->m_EXP >= iMaxEXP )
			{
				strstm << L" " << GET_STRING( STR_ID_2688 );	// fix! STR_ID ���� ������ ��
			}
			else
			{
				int iExp			= 0;
				int iPartyExp		= 0;
				int iSocketOptExp	= 0;

				iExp			= kEXPData.m_iEXP;
				iPartyExp		= kEXPData.m_iPartyEXP;
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
				iSocketOptExp	= kEXPData.m_iSocketOptEXP + kEXPData.m_iEventBonusEXP;		/// ���� ����ġ + �̺�Ʈ ���ʽ� ����ġ
#else  SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
				iSocketOptExp	= kEXPData.m_iSocketOptEXP;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD

				pMyUnitData->m_EXP += iExp;

				if ( pMyUnitData->m_EXP >= iMaxEXP )
				{
					iPartyExp			= 0;
					iSocketOptExp		= 0;
					iExp				= iExp - ( pMyUnitData->m_EXP - iMaxEXP );
					pMyUnitData->m_EXP	= iMaxEXP;
				}
				else
				{
					pMyUnitData->m_EXP += iPartyExp;

					if ( pMyUnitData->m_EXP >= iMaxEXP )			/// �Ϲ� ����ġ + ��Ƽ ����ġ
					{
						iSocketOptExp		= 0;
						iPartyExp			= iPartyExp - ( pMyUnitData->m_EXP - iMaxEXP );
						pMyUnitData->m_EXP	= iMaxEXP;
					}
					else
					{
						pMyUnitData->m_EXP += iSocketOptExp;

						if ( pMyUnitData->m_EXP >= iMaxEXP )		/// �Ϲ� ����ġ + ��Ƽ ����ġ + ���ʽ� ����ġ
						{
							iSocketOptExp		= iSocketOptExp - ( pMyUnitData->m_EXP - iMaxEXP );
							pMyUnitData->m_EXP	= iMaxEXP;
						}
					}
				}
#ifdef _LANGUAGE_FIX_TW_HK
				strstm << GET_STRING( STR_ID_4402 ) << L" ";
#endif _LANGUAGE_FIX_TW_HK

				const CX2UnitManager::NPCUnitTemplet* pNpcTemplet 
					= g_pData->GetUnitManager()->GetNPCUnitTemplet( static_cast<CX2UnitManager::NPC_UNIT_ID>( kEvent.m_iNPCID ) );

				if ( NULL != pNpcTemplet )
					strstm << pNpcTemplet->m_Name.c_str();

				if( kEXPData.m_iEXP >= 0 )
				{
					strstm << L" " << GET_STRING( STR_ID_189 ) << L" " << iExp + iPartyExp + iSocketOptExp;
				}

				if( kEXPData.m_iPartyEXP > 0 )
				{
					strstm << L" " << GET_REPLACED_STRING( ( STR_ID_190, "i", iPartyExp ) );
				}

#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
				if ( ( kEXPData.m_iSocketOptEXP + kEXPData.m_iEventBonusEXP ) > 0)
#else  SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
				if ( kEXPData.m_iSocketOptEXP > 0)
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
				{
					strstm << L" " << GET_REPLACED_STRING( ( STR_ID_2625, "i", iSocketOptExp ) );
				}
			}

#else REAL_TIME_ELSWORD
			m_GetEXPInGame += kEXPData.m_iEXP;
			m_GetEXPInGame += kEXPData.m_iPartyEXP;

			wstringstream strstm;

#ifdef _LANGUAGE_FIX_TW_HK
			strstm << GET_STRING( STR_ID_4402 ) << L" ";
#endif _LANGUAGE_FIX_TW_HK

			strstm << pCX2GUNPC->GetNPCTemplet().m_Name.c_str();


			if( kEXPData.m_iEXP >= 0 )
			{

				strstm << L" " << GET_STRING( STR_ID_189 ) << L" " << kEXPData.m_iEXP;
			}
			if( kEXPData.m_iPartyEXP > 0 )
			{
				strstm << L" " << GET_REPLACED_STRING( ( STR_ID_190, "i", kEXPData.m_iPartyEXP ) );
			}
#endif REAL_TIME_ELSWORD
			//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ����ġ ȹ��

			if( NULL != g_pChatBox )
			{
				if( false == strstm.str().empty() )
				{
					g_pChatBox->AddChatLogObtain(strstm.str().c_str(), CX2ChatBox::COT_EXP);
				}
			}
		}

#ifdef TODAY_RECORD_TEST
		if( NULL != GetMyUnit() && 
			NULL != GetMyUnit()->GetUnit() )
		{
			if( -1 != kEvent.m_uiAttUnit && 				
				GetMyUnit()->GetUnitUID() == kEvent.m_uiAttUnit )
			{
				GetMyUnit()->GetUnit()->AccessUnitData().m_DailyAchievement.m_iKillCount += 1;
			}
		}
#endif TODAY_RECORD_TEST
	}

}

#ifdef FINALITY_SKILL_SYSTEM //��â��
//void CX2Game::ProcessSummonNPCDieEvent( const KEGS_NPC_UNIT_DIE_NOT& kEvent )
//{
//	CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( kEvent.m_nDieNPCUID );
//	if( pCX2GUNPC != NULL )
//	{
//		if( pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_SI_HA_FERDINAND )
//		{
//			CX2GUUser* pUser = static_cast<CX2GUUser*>( pCX2GUNPC->GetOwnerGameUnit() );
//			if( pUser != NULL && pUser->GetUnit()->GetType() == CX2Unit::UT_EVE )
//			{
//				CX2GUEve* pEve = static_cast<CX2GUEve*>( pUser );
//				if( NULL != pEve )
//				{
//					pEve->ResetLinkOverChargeIllusion();
//				}
//			}
//		}
//	}
//}
#endif //FINALITY_SKILL_SYSTEM

//bool CX2Game::Handler_EGS_NOTIFY_MSG_NOT( KEGS_NOTIFY_MSG_NOT& kEGS_NOTIFY_MSG_NOT )
//{
//	g_pMain->GetNoticeManager()->AddNotice( kEGS_NOTIFY_MSG_NOT.m_wstrMSG.c_str(), (int)kEGS_NOTIFY_MSG_NOT.m_Count );
//
//	return true;
//}

bool CX2Game::CheckNewUser( UidType unitUID )
{
	for ( int i = 0; i < (int)m_NewGameUserList.size(); i++ )
	{
		NewGameUser* pNewGameUser = &m_NewGameUserList[i];
		if( pNewGameUser->m_UnitUID == unitUID && pNewGameUser->m_bStart == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2Game::Handler_EGS_JOIN_ROOM_NOT( KEGS_JOIN_ROOM_NOT& kEGS_JOIN_ROOM_NOT )
{
	if( kEGS_JOIN_ROOM_NOT.m_JoinSlot.m_kRoomUserInfo.m_nUnitUID == g_pX2Room->GetMySlot()->m_UnitUID )
		return true;
	
	wstringstream wstrstream;
	wstrstream << L"Handler_EGS_JOIN_ROOM_NOT-";
	StateLog( wstrstream.str().c_str() );

	g_pX2Room->Set_KRoomSlotInfo( kEGS_JOIN_ROOM_NOT.m_JoinSlot );

	if ( GetGameType() == GT_BATTLE_FIELD )
	{
		CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEGS_JOIN_ROOM_NOT.m_JoinSlot.m_kRoomUserInfo.m_nUnitUID );
		if( pSlotData != NULL )
		{
#ifdef SETTING_BATTLE_FIELD_ENEMY_TEAM_FOR_TEST_PVE_SYSTEM
			if( NULL != pSlotData->m_pUnit )
			{
				switch( pSlotData->m_pUnit->GetType() )
				{
				case CX2Unit::UT_ARA:
				case CX2Unit::UT_ARME:
				case CX2Unit::UT_LIRE:
				case CX2Unit::UT_EVE:
					pSlotData->m_TeamNum = 1;
				default:
					break;
				}
			}
#endif // SETTING_BATTLE_FIELD_ENEMY_TEAM_FOR_TEST_PVE_SYSTEM

			CX2Unit* pCX2Unit = pSlotData->m_pUnit;
			if( pCX2Unit != NULL )
			{
				//P2PSetPeer( pCX2Unit );
				//P2PConnectTest();

				//��׶��� �ε��� ��÷ε��� ��� �������� �ϴ�!
				//AddUserUnitIntruder( pSlotData, pSlotData->m_Index );
				AddUserUnitIntruderByLoader( pSlotData->m_pUnit );

				if( g_pX2Room->GetMySlot()->m_UnitUID != pSlotData->m_UnitUID )
				{
					//�����ڰ� ���� �ƴϸ�
					NewGameUser pvpGameNewUser;
					pvpGameNewUser.m_UnitUID			= pSlotData->m_UnitUID;
					pvpGameNewUser.m_StartPosIndex		= kEGS_JOIN_ROOM_NOT.m_StartPosIndex;
					m_NewGameUserList.push_back( pvpGameNewUser );
				}
				else
				{
					g_pX2Room->SetIntrudeGame( false );
					if( GetMyUnit() != NULL )
					{
						GetMyUnit()->SetForceInvincible( 5.0f );
						GetMyUnit()->SetShowInvincible( 5.0f );
						GetMyUnit()->StateChange( GetMyUnit()->GetStartIntrudeStateID(), true );
					}		
					GameStart();
					g_pKTDXApp->SkipFrame();

					// ���Խ� esc �ȸ����� ���� ����
					SetCanUseEscFlag( true );
				}

#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
				InsertGUUserToCheckUDPTimer( pSlotData->m_UnitUID );
#endif // SERV_FIX_SYNC_PACKET_USING_RELAY
			}
		}
	}		



	return true;
}

bool CX2Game::Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason /* = NetError::NOT_LEAVE_ROOM_REASON_00 */ )
{
#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_LEAVE_ROOM_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST

	if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
	{
		//��踦 ���ؼ� ��������������S�������
		Handler_EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT();
	}

	KEGS_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = leaveRoomReason;

#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	CX2GUUser* pCX2GUUser = GetMyUnit();
	if( NULL != pCX2GUUser )
	{
		kPacket.m_kResultInfo.m_fHP				= pCX2GUUser->GetNowHp();
		kPacket.m_kResultInfo.m_nDamageCount	= pCX2GUUser->GetDamageCount();
		kPacket.m_kResultInfo.m_TotalScore		= m_GameScore;
		//kPacket.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();

		if( NULL != pCX2GUUser->GetComboManager() )
		{
			kPacket.m_kResultInfo.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();
		}
		else
		{
			kPacket.m_kResultInfo.m_ComboScore	= 0;
		}
		kPacket.m_kResultInfo.m_TechScore		= m_TechPointFull;

		kPacket.m_kResultInfo.m_iTotalAttackedDamage	= pCX2GUUser->GetTotalAttackedDamage();
		kPacket.m_kResultInfo.m_iTotalRecoveryHP		= pCX2GUUser->GetTotalRecoveryHP();
		kPacket.m_kResultInfo.m_iUsingKindOfSkill		= pCX2GUUser->GetUsingSkillCount();
		kPacket.m_kResultInfo.m_iTotalGivenDamamge		= pCX2GUUser->GetTotalGivenDamamge();
#ifdef DUNGEON_DASH_LOG
		kPacket.m_kResultInfo.m_iDashCount				= pCX2GUUser->GetUsingDashCount();
#endif //DUNGEON_DASH_LOG
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG 
			kPacket.m_kResultInfo.m_bSkillSlotType = g_pMain->GetGameOption().GetIsSkillUITypeA();
#endif //SERV_USE_SKILL_SLOT_TYPE_LOG 

	}
	kPacket.m_kResultInfo.m_iFrame = static_cast<int>(m_kGameStatistics.m_kAverageFps.m_fAverageFps);
#endif //SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	// �ʿ� ������ �ִ� GamePlayNetworkInfo �� vector�� ��´�.
	if ( NULL != g_pX2Room )
		g_pX2Room->MoveGamePlayNetworkInfoFromMapToVector( kPacket.m_kGamePlayNetworkInfo.m_vecGamePlayNetworkInfo );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

// #ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
// 	AHNHS_PROTECT_FUNCTION
// #endif

	g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_ROOM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LEAVE_ROOM_ACK );
	return true;
}

bool CX2Game::Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK )
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//�����߿� ������ ��������...
	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK );	

#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	// �� ��Ż�� �������� ���
	const bool bRet = g_pMain->IsValidPacket( kEGS_LEAVE_ROOM_ACK.m_iOK );
	if( bRet == true )
	{
		if ( kEGS_LEAVE_ROOM_ACK.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_22 )
		{
			std::wstringstream wstrStream;
            wstrStream << L"'" << g_pData->GetItemManager()->GetItemTemplet(kEGS_LEAVE_ROOM_ACK.m_iItemID)->GetFullName_()
                << NetError::GetErrStrF(NetError::NOT_LEAVE_ROOM_REASON_22);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrStream.str().c_str(), g_pMain->GetNowState() );
		}
		
#ifdef COME_BACK_REWARD
		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME &&
			NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() && 
			false == g_pData->GetPremiumBuffManager()->IsComeBackUser() )
		{
			g_pData->GetPremiumBuffManager()->SetComeBackUserInParty( false );
		}
#endif

#ifndef HEAP_BROKEN_BY_ROOM
		CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
		if ( NULL != pNowState )
		{
			CX2LocationManager* pLocationManager = g_pData->GetLocationManager();
			switch ( pLocationManager->GetX2StateBeforeDungeonStart() )
			{
			case CX2Main::XS_BATTLE_FIELD:
				{
					CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
					pNowState->Handler_EGS_STATE_CHANGE_FIELD_REQ( battleFieldManager.GetReturnVillageId( battleFieldManager.GetBattleFieldIdWhereIam() ) );				
					return false;
				} break;

			case CX2Main::XS_SERVER_SELECT: //Ʃ�丮���� ��� XS_SERVER_SELECT���� ����		
				{
					CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
					if ( !pLocationManager->IsValidVillage( battleFieldManager.GetBattleFieldIdWhereIam() ) )
					{
						pNowState->Handler_EGS_STATE_CHANGE_FIELD_REQ( battleFieldManager.GetReturnVillageId( battleFieldManager.GetBattleFieldIdWhereIam() ) );				
					}
					else
						pNowState->Handler_EGS_STATE_CHANGE_FIELD_REQ();				
					return false;
				} break;

			case CX2Main::XS_VILLAGE_MAP:
				{
					pNowState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
					return false;
				} break;

			default:
				{
					pNowState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
					ASSERT( !L"GetX2StateBeforeDungeonStart is wrong!" );
				} break;
			}
		}
#endif // HEAP_BROKEN_BY_ROOM
	}
	return bRet;
#else
	return g_pMain->IsValidPacket( kEGS_LEAVE_ROOM_ACK.m_iOK );
#endif
}

bool CX2Game::Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT )
{
	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_1" );
	//P2PDeletePeer( kEGS_LEAVE_ROOM_NOT.m_iUnitUID );

	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_2" );
	//{{ 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	m_mapCheckUDPTimer.erase( kEGS_LEAVE_ROOM_NOT.m_iUnitUID );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}
	DeleteUserUnitByUID( kEGS_LEAVE_ROOM_NOT.m_iUnitUID );

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_3" );
	m_UnitLoader.DeleteUnitLoadData( kEGS_LEAVE_ROOM_NOT.m_iUnitUID );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_4" );
	g_pX2Room->SetRoomState( (CX2Room::ROOM_STATE)kEGS_LEAVE_ROOM_NOT.m_cRoomState );

	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_5" );
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	g_pX2Room->Set_KRoomSlotInfoList( kEGS_LEAVE_ROOM_NOT.m_vecSlot );
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_6" );
	UpdateUnitPointer();

	DialogLog( "Handler_EGS_LEAVE_ROOM_NOT_7" );
	return true;
}

bool CX2Game::Handler_EGS_LEAVE_GAME_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_LEAVE_GAME_REQ );
	g_pMain->AddServerPacket( EGS_LEAVE_GAME_ACK );
	return true;
}

bool CX2Game::Handler_EGS_LEAVE_GAME_ACK( KEGS_LEAVE_GAME_ACK& kEGS_LEAVE_GAME_ACK )
{
#ifdef FIX_EGS_LEAVE_GAME_ACK
	if( g_pMain->DeleteServerPacket( EGS_LEAVE_GAME_ACK ) == true )
#else FIX_EGS_LEAVE_GAME_ACK
	if( g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK ) == true )
#endif FIX_EGS_LEAVE_GAME_ACK
	{
		return g_pMain->IsValidPacket( kEGS_LEAVE_GAME_ACK.m_iOK );
	}
	return false;
}

bool CX2Game::Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT )
{
	//P2PDeletePeer( kEGS_LEAVE_GAME_NOT.m_iUnitUID );
#ifdef UDP_CAN_NOT_SEND_USER_KICK	
	if(GetGameType() == GT_PVP )
	{
		if(g_pInstanceData->GetUDPKickOff() == false)
		{
			if( g_pInstanceData->m_mapCheckUDPTimer.empty() == false )
			{
				std::map< UidType, boost::timer >::iterator mit;
				mit = g_pInstanceData->m_mapCheckUDPTimer.find(kEGS_LEAVE_GAME_NOT.m_iUnitUID);
				if( mit != g_pInstanceData->m_mapCheckUDPTimer.end() )
				{
					g_pInstanceData->m_mapCheckUDPTimer.erase(kEGS_LEAVE_GAME_NOT.m_iUnitUID);
				}
			}

			if( g_pInstanceData->m_mapCheckUDPTDelayCount.empty() == false )
			{
				std::map< UidType, __int64 >::iterator mit2;
				mit2 = g_pInstanceData->m_mapCheckUDPTDelayCount.find(kEGS_LEAVE_GAME_NOT.m_iUnitUID);
				if( mit2 != g_pInstanceData->m_mapCheckUDPTDelayCount.end() )
				{
					g_pInstanceData->m_mapCheckUDPTDelayCount.erase(kEGS_LEAVE_GAME_NOT.m_iUnitUID);
				}
			}
			 
#ifdef LOG_UDP_CHECK
			if( g_pInstanceData->m_mapCheckUDPTDelayCountTemp.empty() == false )
			{
				std::map< UidType, __int64 >::iterator mit2 = g_pInstanceData->m_mapCheckUDPTDelayCountTemp.begin();
				if( NULL != GetMyUnit() )
				{
					for( ; mit2 != g_pInstanceData->m_mapCheckUDPTDelayCountTemp.end(); ++mit2 )
					{
						KEGS_UDP_CHECK_KICK_USER_NOT kPacket;
						kPacket.m_iMyUnitUID = GetMyUnit()->GetUnitUID();
						kPacket.m_iAccusationUnitUID = mit2->first;
						kPacket.m_bNowKickOut = false;

#ifdef	UDP_CAN_NOT_SEND_USER_KICK_LOG
						kPacket.m_byteNum = static_cast<byte>(mit2->second);
#endif	UDP_CAN_NOT_SEND_USER_KICK_LOG

						if( kPacket.m_iMyUnitUID != kPacket.m_iAccusationUnitUID )
						{
							if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
							{
								g_pData->GetServerProtocol()->SendPacket( EGS_UDP_CHECK_KICK_USER_NOT, kPacket );
							}
						}
					}

					mit2 = g_pInstanceData->m_mapCheckUDPTDelayCountTemp.find(kEGS_LEAVE_GAME_NOT.m_iUnitUID);
					if( mit2 != g_pInstanceData->m_mapCheckUDPTDelayCountTemp.end() )
					{
						g_pInstanceData->m_mapCheckUDPTDelayCountTemp.erase(kEGS_LEAVE_GAME_NOT.m_iUnitUID);
					}
				}
			}

#endif //LOG_UDP_CHECK
		}
	}

#endif //UDP_CAN_NOT_SEND_USER_KICK
	
	//{{ dmlee 2010/01/07 �����߿� ESC ������ ���� �� �� ��Ŷ �ް��� EGS_NPC_UNIT_DIE_NOT ������ ���� ���� ������ ����
	if( NULL != GetMyUnit() )
	{
		if( GetMyUnit()->GetUnitUID() == kEGS_LEAVE_GAME_NOT.m_iUnitUID )
		{
			m_optrMyUnit.Reset();
		}
	}
	//}} dmlee 2010/01/07 �����߿� ESC ������ ���� �� �� ��Ŷ �ް��� EGS_NPC_UNIT_DIE_NOT ������ ���� ���� ������ ����

	//{{ 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	m_mapCheckUDPTimer.erase( kEGS_LEAVE_GAME_NOT.m_iUnitUID );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

	DeleteUserUnitByUID( kEGS_LEAVE_GAME_NOT.m_iUnitUID );

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	g_pX2Room->SetRoomState( (CX2Room::ROOM_STATE)kEGS_LEAVE_GAME_NOT.m_cRoomState );
	g_pX2Room->Set_KRoomSlotInfoList( kEGS_LEAVE_GAME_NOT.m_vecSlot );
	g_pX2Room->Set_KRoomObserverSlotInfoList( kEGS_LEAVE_GAME_NOT.m_vecObserverSlot );


#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	UpdateUnitPointer();
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
////#ifdef	BATTLE_FIELD_TEST
//	//FieldTest
//	BOOST_FOREACH( const KRoomSlotInfo& kRoomSlotInfo, kEGS_LEAVE_GAME_NOT.m_vecSlot )
//	{
//		if ( true == kRoomSlotInfo.m_bHost )
//		{
//			CX2GUUser* pHostGUUser = GetUserUnitByUID( kRoomSlotInfo.m_kRoomUserInfo.m_nUnitUID );
//			SetHostGameUnit( pHostGUUser );
//		}
//	}		
////#endif	BATTLE_FIELD_TEST
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	return true;
}

bool CX2Game::Handler_EGS_STATE_CHANGE_RESULT_REQ()
{
	KEGS_STATE_CHANGE_RESULT_REQ kEGS_STATE_CHANGE_RESULT_REQ;

	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_RESULT_REQ, kEGS_STATE_CHANGE_RESULT_REQ );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_RESULT_ACK );

	return true;
}

bool CX2Game::Handler_EGS_STATE_CHANGE_RESULT_ACK( KEGS_STATE_CHANGE_RESULT_ACK& kEGS_STATE_CHANGE_RESULT_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_RESULT_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_STATE_CHANGE_RESULT_ACK.m_iOK );
	}
	return false;
}

bool CX2Game::Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT )
{
	g_pX2Room->SetRoomState( (CX2Room::ROOM_STATE)kEGS_STATE_CHANGE_RESULT_NOT.m_cRoomState );
	g_pX2Room->Set_KRoomSlotInfoList( kEGS_STATE_CHANGE_RESULT_NOT.m_vecSlot );

	return true;
}



bool CX2Game::Handler_EGS_PLAY_START_NOT( KEGS_PLAY_START_NOT& kEGS_PLAY_START_NOT )
{	
	g_pX2Room->SetRoomState( (CX2Room::ROOM_STATE)kEGS_PLAY_START_NOT.m_RoomState );
	g_pX2Room->Set_KRoomSlotInfoList( kEGS_PLAY_START_NOT.m_vecSlot );

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
	{
		g_pX2Room->DeleteNpcSlot();
	}
	else	// ���Ĵ����� �ƴ� ��쿡��
	{
		//{{ kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08
		SetCanUseEscFlag( true );
		//}} kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08
	}
#endif

	GameStart();


	


#ifdef SERVER_PVP_BASE_DEFENCE_TEST

	if( true == IsHost() )
	{
		switch( g_pX2Room->GetRoomType() )
		{
		case CX2Room::RT_PVP:
			{
				CX2PVPRoom* pPVPRoom = (CX2PVPRoom*) g_pX2Room;
				if( true == pPVPRoom->GetPVPGameOption().m_bBaseDefence ) // ���࿡ pvp base defence �̸� base monster ���� ��Ŷ�� ������
				{

					D3DXVECTOR3 vRedTeamPosition = GetWorld()->GetLineMap()->GetRandomPosition( NULL, 0.f, false );
					D3DXVECTOR3 vBlueTeamPosition = GetWorld()->GetLineMap()->GetRandomPosition( NULL, 0.f, false );

					PushCreateNPCReq( CX2UnitManager::NUI_STATUE_OF_RED, 10, true, vRedTeamPosition, true, 0.f, true, -1, CX2Room::TN_RED, CX2NPCAI::NAT_NORMAL, -1 );
					PushCreateNPCReq( CX2UnitManager::NUI_STATUE_OF_BLUE, 10, true, vBlueTeamPosition, true, 0.f, true, -1, CX2Room::TN_BLUE, CX2NPCAI::NAT_NORMAL, -1 );
					FlushCreateNPCReq();

				}

			} break;
		}

	}

#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef SERV_PVP_NEW_SYSTEM
	CKTDGLineMap* pLineMap = NULL;
	if( m_pWorld != NULL )
		pLineMap = m_pWorld->GetLineMap();
		
	if( pLineMap != NULL )
	{
		int numNpc = 0;
		std::vector< CX2Room::RoomNpcSlot > &vecNpcSlot = g_pX2Room->GetNpcSlot();
		for(int i=0; i<(int)vecNpcSlot.size(); ++i)
		{
			CX2Room::RoomNpcSlot &npcSlot = vecNpcSlot[i];
			
			int index = i - 4;
			if( index < 0 )
				index = 0;

			D3DXVECTOR3	startPos	= pLineMap->GetBlueTeamStartPosition( index );
			bool bRight				= pLineMap->GetBlueTeamStartRight( index );
			int lineIndex			= pLineMap->GetBlueTeamStartLineIndex( index );

			PushCreateNPCReq( (CX2UnitManager::NPC_UNIT_ID)npcSlot.m_iNpcId, npcSlot.m_iLevel, true, 
				startPos, bRight, 0.f, true, PVP_NPC_KEY_CODE, (CX2Room::TEAM_NUM)CX2Room::TN_BLUE, CX2NPCAI::NAT_NORMAL, -1 );
			++numNpc;
			
		}

		if( numNpc > 0 )
			FlushCreateNPCReq();
	}	
#endif


#ifdef MARIO_LIKE_BLOCK_TEST

	if( NULL != m_pWorld )
	{
		std::vector< CX2World::WorldMonsterData >& vecWorldMonsterData = m_pWorld->GetVecWorldMonsterData();
		BOOST_TEST_FOREACH( CX2World::WorldMonsterData&, worldMonsterData, vecWorldMonsterData )
		{
			PushCreateNPCReq( (CX2UnitManager::NPC_UNIT_ID) worldMonsterData.m_iNPCID, worldMonsterData.m_iLevel, true, worldMonsterData.m_vPosition, true, 0.f, true, -1, 
				(CX2Room::TEAM_NUM) worldMonsterData.m_iTeamNumber, CX2NPCAI::NAT_NORMAL, -1 );
		}

		FlushCreateNPCReq();
		vecWorldMonsterData.clear();
	}
	

#endif MARIO_LIKE_BLOCK_TEST



	g_pKTDXApp->SkipFrame();

	return true;
}

bool CX2Game::Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( KEGS_STATE_CHANGE_GAME_INTRUDE_NOT& kEvent )
{
	g_pX2Room->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_RoomState );
	g_pX2Room->Set_KRoomSlotInfo( kEvent.m_IntruderSlot );

	
	if( g_pX2Room->GetMySlot()->m_UnitUID != kEvent.m_IntruderSlot.m_kRoomUserInfo.m_nUnitUID )
	{
		//�����ڰ� ���� �ƴϸ�
		CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_IntruderSlot.m_kRoomUserInfo.m_nUnitUID );
		if( pSlotData != NULL )
		{
			CX2Unit* pCX2Unit = pSlotData->m_pUnit;
			if( pCX2Unit != NULL )
			{
				//P2PSetPeer( pCX2Unit );
				//P2PConnectTest();

				//��׶��� �ε��� ��÷ε��� ��� �������� �ϴ�!
				//AddUserUnitIntruder( pSlotData, pSlotData->m_Index );
				AddUserUnitIntruderByLoader( pSlotData->m_pUnit );
			}
		}
	}
	return true;
}

bool CX2Game::Handler_EGS_INTRUDE_START_REQ()
{
	KEGS_INTRUDE_START_REQ kEGS_INTRUDE_START_REQ;
	if( NULL != GetWorld() &&
		NULL != GetWorld()->GetLineMap() )
	{
		kEGS_INTRUDE_START_REQ.m_StartPosIndex = GetWorld()->GetLineMap()->GetRandomStartPosIndex();
	}
	else
	{
		kEGS_INTRUDE_START_REQ.m_StartPosIndex = 0;
	}

	if( kEGS_INTRUDE_START_REQ.m_StartPosIndex < 0 )
		kEGS_INTRUDE_START_REQ.m_StartPosIndex = 0;

	if( GetMyUnit() != NULL )
		GetMyUnit()->InitPosition( false, kEGS_INTRUDE_START_REQ.m_StartPosIndex );

	g_pData->GetServerProtocol()->SendPacket( EGS_INTRUDE_START_REQ, kEGS_INTRUDE_START_REQ );
	g_pMain->AddServerPacket( EGS_INTRUDE_START_ACK );

	return true;
}

bool CX2Game::Handler_EGS_INTRUDE_START_ACK( KEGS_INTRUDE_START_ACK& kEGS_INTRUDE_START_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_INTRUDE_START_ACK ) == true )
	{
		bool bResult = g_pMain->IsValidPacket( kEGS_INTRUDE_START_ACK.m_iOK );
#ifdef SERVICE_HEAVY_LOG
		if( false == bResult )
		{
			StateLog( L"Can not Intrude ack" );
		}
#endif SERVICE_HEAVY_LOG
		return bResult;
	}
	return false;
}

bool CX2Game::Handler_EGS_INTRUDE_START_NOT( KEGS_INTRUDE_START_NOT& kEGS_INTRUDE_START_NOT )
{
	
#ifdef SERVICE_HEAVY_LOG
	ASSERT( NULL != g_pX2Room );
	if( NULL == g_pX2Room )
	{
		StateLog( L"intrude room null" );
		return false;
	}
#endif SERVICE_HEAVY_LOG


	g_pX2Room->SetRoomState( (CX2Room::ROOM_STATE)kEGS_INTRUDE_START_NOT.m_RoomState );
	g_pX2Room->Set_KRoomSlotInfo( kEGS_INTRUDE_START_NOT.m_IntruderSlot );

#ifdef SERVICE_HEAVY_LOG
	ASSERT( NULL != g_pX2Room->GetMySlot() );
	if( NULL == g_pX2Room->GetMySlot() )
	{
		StateLog( L"intrude my slot null" );
		return false;
	}
#endif SERVICE_HEAVY_LOG

	if( g_pX2Room->GetMySlot()->m_UnitUID != kEGS_INTRUDE_START_NOT.m_IntruderSlot.m_kRoomUserInfo.m_nUnitUID )
	{
		//�����ڰ� ���� �ƴϸ�
		NewGameUser pvpGameNewUser;
		pvpGameNewUser.m_UnitUID			= kEGS_INTRUDE_START_NOT.m_IntruderSlot.m_kRoomUserInfo.m_nUnitUID;
		pvpGameNewUser.m_StartPosIndex		= kEGS_INTRUDE_START_NOT.m_StartPosIndex;
		m_NewGameUserList.push_back( pvpGameNewUser );

		//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
		if( NULL != g_pChatBox  &&  kEGS_INTRUDE_START_NOT.m_bObserver == false )
#else
		if( NULL != g_pChatBox )
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
		//}}
		{
			wstring wstrColor = L"#CFF3F4D";								// 255, 63, 77
			D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77

			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_196, "L", kEGS_INTRUDE_START_NOT.m_IntruderSlot.m_kRoomUserInfo.m_wstrNickName ) ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );

		}

		//��׶��� �ε��� ��÷ε��� ��� �������� �ϴ�!
		//IntruderStart( kEGS_INTRUDE_START_NOT.m_IntruderSlot.m_kRoomUserInfo.m_kUnitInfo.m_nUnitUID, kEGS_INTRUDE_START_NOT.m_StartPosIndex );
	}
	else
	{
		g_pX2Room->SetIntrudeGame( false );
		if( GetMyUnit() != NULL )
		{
			GetMyUnit()->SetForceInvincible( 5.0f );
			GetMyUnit()->SetShowInvincible( 5.0f );
			GetMyUnit()->StateChange( GetMyUnit()->GetStartIntrudeStateID(), true );
		}		
		GameStart();
		g_pKTDXApp->SkipFrame();

		// ���Խ� esc �ȸ����� ���� ����
		SetCanUseEscFlag( true );
	}

	return true;
}


void CX2Game::Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( KEGS_CURRENT_TEAM_KILL_SCORE_NOT& kEGS_CURRENT_TEAM_KILL_SCORE_NOT )
{
	m_RedTeamKill	= kEGS_CURRENT_TEAM_KILL_SCORE_NOT.m_RedTeamKill;
	m_BlueTeamKill	= kEGS_CURRENT_TEAM_KILL_SCORE_NOT.m_BlueTeamKill;
}


/*virtual*/ bool CX2Game::Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT )
{
	m_bGameEnd = true;

	SAFE_DELETE_DIALOG( m_pDLGCheckResurrectByUseCash );

// 	if( g_pData != NULL && 
// 		g_pData->GetUIManager() != NULL && 
// 		g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
// 	{
// 		g_pData->GetUIManager()->GetUIQuickSlot()->ResetCoolTime();
// 	}

	return true;
}


bool CX2Game::Handler_EGS_DROP_ITEM_NOT( KEGS_DROP_ITEM_NOT& kEGS_DROP_ITEM_NOT )
{
	for( int i = 0; i < (int)kEGS_DROP_ITEM_NOT.m_DropItemDataList.size(); i++ )
	{
		// themida_checked
		//THEMIDA_VM_START

		const KDropItemData& dropData = kEGS_DROP_ITEM_NOT.m_DropItemDataList[i];
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT ///����Ǵ� ��� �����ۿ� ��ȭ��ġ �߰�

#ifdef FIELD_BOSS_RAID // ��� ������ ���� �ð� ����
		if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
		{
			m_pDropItemManager->AddDropItem( dropData.m_iItemID, dropData.m_iDropItemUID, 
				D3DXVECTOR3( kEGS_DROP_ITEM_NOT.m_CreatePos.x, kEGS_DROP_ITEM_NOT.m_CreatePos.y, kEGS_DROP_ITEM_NOT.m_CreatePos.z ), 
				dropData.m_bLeft, dropData.m_fSpeed, 60.f, dropData.m_cEnchantLevel );
		}
		else
#endif // FIELD_BOSS_RAID
		{
			m_pDropItemManager->AddDropItem( dropData.m_iItemID, dropData.m_iDropItemUID, D3DXVECTOR3( kEGS_DROP_ITEM_NOT.m_CreatePos.x, kEGS_DROP_ITEM_NOT.m_CreatePos.y, kEGS_DROP_ITEM_NOT.m_CreatePos.z ), dropData.m_bLeft, dropData.m_fSpeed, 20.f, dropData.m_cEnchantLevel );
		}
#else  SERV_ENCHANT_ITEM_DROP_EVENT
		m_pDropItemManager->AddDropItem( dropData.m_iItemID, dropData.m_iDropItemUID, D3DXVECTOR3( kEGS_DROP_ITEM_NOT.m_CreatePos.x, kEGS_DROP_ITEM_NOT.m_CreatePos.y, kEGS_DROP_ITEM_NOT.m_CreatePos.z ), dropData.m_bLeft, dropData.m_fSpeed );
#endif SERV_ENCHANT_ITEM_DROP_EVENT

		//THEMIDA_VM_END

	}
	return true;
}





// TODO: �� �Լ��� ������ ����ȭ�� �ȵǰ� �߰� themida �� ����ȭ�߱� ������ �ټ��� �������� ���ÿ� �Ծ��� ��쿡 framerate�� ������ �� �ִ� ������ �Ǹ� THEMIDA_ENCODE�� �ٲ���
//#ifdef THEMIDA_BY_TOOL_TEAM
//#	pragma optimize("",off)    
//#endif THEMIDA_BY_TOOL_TEAM
bool CX2Game::Handler_EGS_GET_ITEM_REQ( UidType itemUID )
{
	


	// checked
	ELSWORD_VIRTUALIZER_START


	
	THEMIDA_ENCODE_START

	int iItemTID = m_pDropItemManager->GetDropItemID( itemUID );

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID = iItemTID;
	kItemInfo.m_iQuantity = 1;
	kItemInfo.m_cUsageType = 0;

	KEGS_GET_ITEM_REQ kKEGS_GET_ITEM_REQ;

	//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ������ ȹ��
#ifdef	REAL_TIME_ELSWORD
	kKEGS_GET_ITEM_REQ.m_iDropItemUID = static_cast<int>( itemUID );
#else	REAL_TIME_ELSWORD
	kKEGS_GET_ITEM_REQ.m_iItemUID = static_cast<int>(itemUID);
#endif	REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ������ ȹ��
	
	kKEGS_GET_ITEM_REQ.m_kItemInfo = kItemInfo;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_ITEM_REQ, kKEGS_GET_ITEM_REQ );
	//g_pMain->AddServerPacket( EGS_GET_ITEM_ACK );

#ifdef PET_DROP_ITEM_PICKUP
	//�� ���� ���� ��� ���� ���� ���� ���� üũ �ϵ��� ����
	if( false == m_pDropItemManager->GetIsSummonItemPickupPet() )
#endif //PET_DROP_ITEM_PICKUP
	{
#ifdef RECHECK_GETIETMREQ_OVERDIST
		D3DXVECTOR3 vItemPos( 0.f, 0.f, 0.f );
		if( IsHost() == true && 
			GetUserUnitNum() == 1 &&
			m_pDropItemManager->GetDropItemPositionByUid( (int)itemUID, vItemPos ) == true )
		{
			D3DXVECTOR3 vDist = GetMyUnit()->GetPos() - vItemPos;
			if( D3DXVec3LengthSq( &vDist ) >= 200.f * 200.f )
			{
				if( g_pData != NULL && g_pData->GetServerProtocol() != NULL &&
					g_pKTDXApp->GetFindHacking() == false )
				{
					g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );

					if( g_pData != NULL && g_pData->GetMyUser() != NULL )
						g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
				}		

#ifdef ADD_COLLECT_CLIENT_INFO
				g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_04, "", true, true );
#else
				if( g_pKTDXApp->GetFindHacking() == false )
					g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_AntiHacking_Get_Item_Req_OverDist );
				g_pKTDXApp->SetFindHacking( true );
#endif
			}
		}
#endif
	}


	
	THEMIDA_ENCODE_END

	ELSWORD_VIRTUALIZER_END

	return true;

}
//#ifdef THEMIDA_BY_TOOL_TEAM
//#	pragma optimize("",on)    
//#endif THEMIDA_BY_TOOL_TEAM




bool CX2Game::Handler_EGS_GET_ITEM_ACK( KEGS_GET_ITEM_ACK& kEGS_GET_ITEM_ACK )
{
	return true;
}

//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ��, �ǽð� ������ ȹ��

bool CX2Game::Handler_EGS_GET_ITEM_NOT( KEGS_GET_ITEM_NOT& kEGS_GET_ITEM_NOT )
{
//#ifndef NOT_USE_DICE_ROLL
//	bool bDiceRolling = false;
//#endif //NOT_USE_DICE_ROLL
	
	int itemID = -1;
	const CX2Item::ItemTemplet* pItemTemplet = NULL;
	if( KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM == kEGS_GET_ITEM_NOT.m_cGetItemType )
	{
		itemID = kEGS_GET_ITEM_NOT.m_iItemID;
		pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	}
	else
	{
		m_pDropItemManager->GetDropItem( kEGS_GET_ITEM_NOT.m_iDropItemUID, kEGS_GET_ITEM_NOT.m_GetUnitUID,
										itemID, pItemTemplet);// OUT���ڷ� ItemID�� pItemTemplet���

//#ifndef NOT_USE_DICE_ROLL
//
//		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
//		if( NULL == pItemTemplet )
//			return false;
//
//		if( NULL != g_pX2Room && 
//			false == CX2ItemManager::IsEDItem( itemID ) && 
//			//NULL != pItemTemplet &&
//			CX2Item::IT_OUTLAY != pItemTemplet->GetItemType() &&
//			LiveUserUnitNum() > 1 ) // user�� 2�� �̻��̸�
//		{
//			CX2DungeonRoom* pDungeonRoom = (CX2DungeonRoom*) g_pX2Room;
//			switch( pDungeonRoom->GetDungeonGetItemType() )
//			{
//			case CX2DungeonRoom::DGIT_RANDOM:
//				{
//#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//					CSLock locker( m_csGameIntruder );
//#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//					for( UINT i=0; i<m_UserUnitList.size(); i++ )
//					{
//						CX2GUUser* pGUUser = m_UserUnitList[i];
//						if( NULL != pGUUser && CX2GameUnit::GUSI_DIE != pGUUser->GetGameUnitState() )
//						{
//							if( NULL != pGUUser->GetDiceRoll() )
//							{
//								bDiceRolling = true;
//								if( pGUUser->GetUnitUID() == kEGS_GET_ITEM_NOT.m_GetUnitUID )
//								{
//									pGUUser->GetDiceRoll()->PushDiceRoll( true );
//								}
//								else
//								{
//									pGUUser->GetDiceRoll()->PushDiceRoll( false );
//								}
//							}
//						}
//					}
//
//				} break;
//			}
//		}
//#endif //NOT_USE_DICE_ROLL
	}
	if( -1 == itemID )
	{
		itemID = kEGS_GET_ITEM_NOT.m_iItemID;
		pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	}

	if( NULL == pItemTemplet )
		return false;

	CX2GUUser* pCX2GUUser = GetUserUnitByUID( kEGS_GET_ITEM_NOT.m_GetUnitUID );
	if( pCX2GUUser != NULL )
	{
		bool bIsEDItem = CX2ItemManager::IsEDItem(kEGS_GET_ITEM_NOT.m_iItemID);
		AddChatLogByGetItem( pItemTemplet, pCX2GUUser, bIsEDItem );

		if( pCX2GUUser->GetNowHp() <= 0.0f || pCX2GUUser->GetNowStateID() == CX2GameUnit::GUSI_DIE )
		{
			return true;
		}

		ProcessGetOutlayItem(pItemTemplet, pCX2GUUser, 
							kEGS_GET_ITEM_NOT.m_iDropItemUID, kEGS_GET_ITEM_NOT.m_bIsItemSuccess);
		ProcessGetEquipItem(pItemTemplet);
		if( NULL != GetMyUnit() )
		{
			ProcessGetEDItem(pCX2GUUser, bIsEDItem,
							kEGS_GET_ITEM_NOT.m_mapGetED[GetMyUnit()->GetUnitUID()],
							kEGS_GET_ITEM_NOT.m_GetUnitUID);
		}
	}

	return true;
}
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ��, �ǽð� ������ ȹ��

bool CX2Game::Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT )
{
	if( m_AllowFrameCount < 0 )
	{
		m_SpeedHackCheck++;
		if( m_SpeedHackCheck > 10 )
		{
			g_pKTDXApp->SetFindHacking( true );
		}
	}
	SetAllowFrameCount( (int)(85 * 5.3) );

	return true;
}

bool CX2Game::Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ()
{
	KEGS_MY_USER_UNIT_INFO_TO_SERVER_REQ kPacket;

	CX2GUUser* pCX2GUUser = GetMyUnit();
	if( NULL == pCX2GUUser )
		return false;

	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	kPacket.m_kMyPlayResult.m_fHP					= pCX2GUUser->GetNowHp();
	kPacket.m_kMyPlayResult.m_nDamageCount			= pCX2GUUser->GetDamageCount();
	kPacket.m_kMyPlayResult.m_TotalScore	= m_GameScore;
	//kPacket.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();

	if( NULL != pCX2GUUser->GetComboManager() )
	{
		kPacket.m_kMyPlayResult.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();
	}
	else
	{
		kPacket.m_kMyPlayResult.m_ComboScore	= 0;
	}
	kPacket.m_kMyPlayResult.m_TechScore		= m_TechPointFull;

	kPacket.m_kMyPlayResult.m_iTotalAttackedDamage	= pCX2GUUser->GetTotalAttackedDamage();
	kPacket.m_kMyPlayResult.m_iTotalRecoveryHP		= pCX2GUUser->GetTotalRecoveryHP();
	kPacket.m_kMyPlayResult.m_iUsingKindOfSkill		= pCX2GUUser->GetUsingSkillCount();
	kPacket.m_kMyPlayResult.m_iTotalGivenDamamge	= pCX2GUUser->GetTotalGivenDamamge();
#ifdef DUNGEON_DASH_LOG
	kPacket.m_kMyPlayResult.m_iDashCount = pCX2GUUser->GetUsingDashCount();
#endif DUNGEON_DASH_LOG
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG 
		kPacket.m_kMyPlayResult.m_bSkillSlotType = g_pMain->GetGameOption().GetIsSkillUITypeA();
#endif //SERV_USE_SKILL_SLOT_TYPE_LOG 
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	kPacket.m_kMyPlayResult.m_iFrame = static_cast<int>(m_kGameStatistics.m_kAverageFps.m_fAverageFps);
#endif //SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
#else
	kPacket.m_fHP			= pCX2GUUser->GetNowHp();
	kPacket.m_nDamageCount	= pCX2GUUser->GetDamageCount();
	kPacket.m_TotalScore	= m_GameScore;
	//kPacket.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();

	if( NULL != pCX2GUUser->GetComboManager() )
	{
		kPacket.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();
	}
	else
	{
		kPacket.m_ComboScore	= 0;
	}
	kPacket.m_TechScore		= m_TechPointFull;

	//kPacket.m_sSkillUseLv1 = pCX2GUUser->GetGageManager()->GetSkillCountForStatistics(1);
	//kPacket.m_sSkillUseLv2 = pCX2GUUser->GetGageManager()->GetSkillCountForStatistics(2);
	//kPacket.m_sSkillUseLv3 = pCX2GUUser->GetGageManager()->GetSkillCountForStatistics(3);
#endif SERV_DUNGEON_RANK_NEW
	//}}	

	g_pData->GetServerProtocol()->SendPacket( EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK, 60.0f );

	return true;
}

bool CX2Game::Handler_EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT()
{
	KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT kPacket;

	CX2GUUser* pCX2GUUser = GetMyUnit();
	if( NULL == pCX2GUUser )
		return false;
	
	kPacket.m_nDamageCount	= pCX2GUUser->GetDamageCount();

	//kPacket.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();

	if( NULL != pCX2GUUser->GetComboManager() )
	{
		kPacket.m_ComboScore	= pCX2GUUser->GetComboManager()->GetComboScoreFull();
	}
	else
	{
		kPacket.m_ComboScore	= 0;
	}
	kPacket.m_TechScore		= m_TechPointFull;

	//kPacket.m_sSkillUseLv1 = pCX2GUUser->GetGageManager()->GetSkillCountForStatistics(1);
	//kPacket.m_sSkillUseLv2 = pCX2GUUser->GetGageManager()->GetSkillCountForStatistics(2);
	//kPacket.m_sSkillUseLv3 = pCX2GUUser->GetGageManager()->GetSkillCountForStatistics(3);

	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT, kPacket );


	return true;
}

bool CX2Game::Handler_EGS_ADD_ON_STAT_ACK( KEGS_ADD_ON_STAT_ACK& kEGS_ADD_ON_STAT_ACK )
{
	return true;
}

bool CX2Game::Handler_EGS_ADD_ON_STAT_NOT( KEGS_ADD_ON_STAT_NOT& kEGS_ADD_ON_STAT_NOT )
{
	if( kEGS_ADD_ON_STAT_NOT.m_bIsUnit == true )
	{
		CX2GUUser* pCX2GUUser = GetUserUnitByUID( kEGS_ADD_ON_STAT_NOT.m_UID );
		if( NULL != pCX2GUUser )
		{
			pCX2GUUser->SetAddOnStat( (KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT)kEGS_ADD_ON_STAT_NOT.m_cStatType, kEGS_ADD_ON_STAT_NOT.m_fStatVal, kEGS_ADD_ON_STAT_NOT.m_fTime );
		}
	}
	else
	{
		CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( (int)kEGS_ADD_ON_STAT_NOT.m_UID );

		//{{ JHKang / ������ / 2010/12/07 // Ư�� NPC Ŭ���� Ÿ�� ���� ���� ���ϱ�
#ifdef NEW_SKILL_2010_11
		if ( NULL != pCX2GUNPC )
		{
			if ( pCX2GUNPC->GetNPCTemplet().m_ClassType == CX2UnitManager::NCT_THING_NOBUFF )
				return true;
		}
#endif NEW_SKILL_2010_11
		//}} JHKang / ������ / 2010/12/07 // Ư�� NPC Ŭ���� Ÿ�� ���� ���� ���ϱ�

		if( NULL != pCX2GUNPC )
		{
			pCX2GUNPC->SetAddOnStat( (KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT)kEGS_ADD_ON_STAT_NOT.m_cStatType, kEGS_ADD_ON_STAT_NOT.m_fStatVal, kEGS_ADD_ON_STAT_NOT.m_fTime );
		}
	}
	return true;
}

void CX2Game::InitF1HelpString()
{
	m_vecF1HelpString.resize(0);
	wstring wstrMessage;

	wstrMessage = GET_STRING( STR_ID_202 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_203 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_204 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_205 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_206 );
	m_vecF1HelpString.push_back( wstrMessage );	


	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_207 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_208 );
	m_vecF1HelpString.push_back( wstrMessage );	

    wstrMessage = L"    ";
	wstrMessage += GET_STRING( STR_ID_209 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_210 );
	m_vecF1HelpString.push_back( wstrMessage );	

    wstrMessage = L"    ";
	wstrMessage += GET_STRING( STR_ID_211 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_212 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_213 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"                ";
    wstrMessage += GET_STRING( STR_ID_214 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"                ";
    wstrMessage = GET_STRING( STR_ID_215 );
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = L"";
	m_vecF1HelpString.push_back( wstrMessage );	

	wstrMessage = GET_STRING( STR_ID_216 );
	m_vecF1HelpString.push_back( wstrMessage );	
}



void CX2Game::RenderF1Help()
{
	if( true == m_bShowF1Help )
	{
		const int MAGIC_OFFSET_X = 1024 / 8;
		const int MAGIC_OFFSET_Y = 768 / 8;

		const int MAGIC_WIDTH_X = 1024 / 8 * 6;
		const int MAGIC_WIDTH_Y = 768 / 8 * 6;


		const int MAGIC_TEXT_OFFSET_X = 20;
		const int MAGIC_TEXT_OFFSET_Y = 5;


		m_pTextureF1Help->Draw( MAGIC_OFFSET_X, MAGIC_OFFSET_Y, MAGIC_WIDTH_X, MAGIC_WIDTH_Y, D3DCOLOR_RGBA( 0, 0, 0, 200 ) );

		for( UINT i=0; i<m_vecF1HelpString.size(); i++ )
		{
			int iPosX = MAGIC_OFFSET_X + MAGIC_TEXT_OFFSET_X;
			int iPosY = (int)( MAGIC_OFFSET_X + MAGIC_TEXT_OFFSET_Y + i * 19 );

			m_pFontF1Help->OutTextXY( iPosX, iPosY, m_vecF1HelpString[i].c_str(), D3DCOLOR_RGBA(255,255,255,255), 
				CKTDGFontManager::FS_SHELL, D3DCOLOR_RGBA(0,0,0,255) );
		}
	}
}

void CX2Game::Verify()
{
	if( g_pKTDXApp->GetIsNowVeryfy() == true )
	{
		bool bVerify = true;

		if( m_GameScore.Verify()		== false
			|| m_GameScoreBack.Verify()	== false
			|| m_TechPoint.Verify()		== false
			|| m_TechPointFull.Verify()	== false 
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

			|| m_iSubStageTechPoint.Verify() == false 
#endif 			
			)
		{
			bVerify = false;

			m_GameScore = -1;
			m_GameScoreBack = -1;
			m_TechPoint = 0;
			m_TechPointFull = 0;
		}

		for( int i = 0; i < (int)m_ScoreTimeList.size(); i++ )
		{
			ScoreTime* pScoreTime = m_ScoreTimeList[i];
			if( pScoreTime->m_Score.Verify() == false )
			{
				bVerify = false;

				pScoreTime->m_Score = 0;
			}
		}

		if( bVerify == false )
		{
			if( g_pData != NULL && g_pData->GetServerProtocol() != NULL &&
				g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
			}

#ifdef ADD_COLLECT_CLIENT_INFO
			g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_05, "", false, false );
#else
			g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_GAME_VERIFY );			
#endif
			//g_pKTDXApp->SetFindHacking( true );
		}		

		if( m_iNpcDiePacket.Verify() == false || m_iNpcDiePacket < 0 )
		{
#ifdef ADD_COLLECT_CLIENT_INFO
			g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_06, "", false, true );
#else
			static bool bSend = false;

			if( bSend == false )
			{
				g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_NPC_DIE_PACKET_VERIFY );
				bSend = true;
			}
#endif
		}
	}
}

void CX2Game::UseItemSpecialAbility( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser )		// ���������� �ٷ� ���Ǵ� Ư���� ������ ���� �Ծ�����
{
	float	fIncreaseRateFromMasterOfAlchemistSkill							= 1.0f;
	//{{ kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)
#ifdef	GUILD_SKILL
	bool	bDoIHaveMasterOfAlchemistSkill			= false;
	int		iSkillLvMasterOfAlchemist				= 0;
	
	if ( pCX2GUUser->GetUnit() != NULL )
	{
		BYTE byMemberShipGrade = pCX2GUUser->GetUnit()->GetUnitData().m_byMemberShipGrade;
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pCX2GUUser->GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_GP_COMMON_MASTER_OF_ALCHEMIST, byMemberShipGrade);

		if ( pSkillTemplet != NULL )
		{
			bDoIHaveMasterOfAlchemistSkill  = true;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			const CX2UserSkillTree& userSkillTree = pCX2GUUser->GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// ��ų ����

			fIncreaseRateFromMasterOfAlchemistSkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			fIncreaseRateFromMasterOfAlchemistSkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

	}
#endif	GUILD_SKILL
	//}} kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)

#ifdef DUNGEON_ITEM
	
#ifndef ADD_VELDER_ACADEMY_EVENT_MONSTER		/// UseItemSpecialAbility���� �� ( ���Ŀ� SAT ID �߱��ؼ� �������� )
	ProcSpecialItem(pItemTemplet, pCX2GUUser);
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER

#endif

    unsigned int    uNumSA = pItemTemplet->GetNumSpecialAbility();
	for( int i = 0; i < (int)uNumSA ; i++ )
	{
		const CX2Item::SpecialAbility* pSa = &pItemTemplet->GetSpecialAbility(i);
		switch( pSa->GetType() )
		{
		case CX2Item::SAT_HP_UP:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					float fIncreateRateHpUp = fIncreaseRateFromMasterOfAlchemistSkill;
					float fValue = static_cast< float >( pSa->m_Value1 );

//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
					// ǻ������� ������� ����
					if ( CX2Unit::UC_CHUNG_FURY_GUARDIAN == pCX2GUUser->GetUnitClass() || CX2Unit::UC_CHUNG_IRON_PALADIN == pCX2GUUser->GetUnitClass() )
					{
						const CX2UserSkillTree& userSkillTree = pCX2GUUser->GetUnit()->GetUnitData().m_UserSkillTree;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
						int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );
	#endif // UPGRADE_SKILL_SYSTEM_2013
						
						if ( 0 < iSkillLv )
						{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							const CX2SkillTree::SkillTemplet* pSkillTemplet = 
								g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );

							if ( NULL != pSkillTemplet )
							{
								float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL, iSkillLv );

								fIncreaseRateMetabolism = CalculateIncreasingRate( fIncreaseRateMetabolism );
								fIncreateRateHpUp += fIncreaseRateMetabolism;
							}
	#else // UPGRADE_SKILL_SYSTEM_2013
							const CX2SkillTree::SkillTemplet* pSkillTemplet = 
								g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, iSkillLv );

							float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL );

							fIncreaseRateMetabolism = CalculateIncreasingRate( fIncreaseRateMetabolism );
							fIncreateRateHpUp += fIncreaseRateMetabolism;
	#endif // UPGRADE_SKILL_SYSTEM_2013
						} // if
					}
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // û 1�� ����

					//{{ kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)
#ifdef	GUILD_SKILL
					fValue *= fIncreateRateHpUp;
#endif	GUILD_SKILL
					//}} kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)

					if( pCX2GUUser->IsMyUnit() == true )
					{

			//{{ kimhc // 2011-04-26 // 1�� ���� �������� ���� ��ġ ������ ȸ������ �ʹ� ���Ƽ� ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
						if ( GT_DUNGEON == GetGameType() )
						{
							CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
							if( NULL != pDungeonGame &&
								NULL != pDungeonGame->GetDungeon() &&
								NULL != pDungeonGame->GetDungeon()->GetDungeonData())
							{
								const SEnum::DUNGEON_ID eDungeonId = pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID;
								if( CX2Dungeon::IsEventDungeon( eDungeonId ) )
								{
									const int iBasicHP = 10000;
									const int iHpByLevel = 1500;

									fValue = iBasicHP * fValue / ( iBasicHP + ( pCX2GUUser->GetUnitLevel() * iHpByLevel ) );
								}
							}
						}
#endif	SERV_INSERT_GLOBAL_SERVER
			//}} kimhc // 2011-04-26 // 1�� ���� �������� ���� ��ġ ������ ȸ������ �ʹ� ���Ƽ� ����

						pCX2GUUser->UpNowHp( fValue );

						//{{ JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
#ifdef DUNGEON_RANK_NEW
						pCX2GUUser->AddRecoveryHP( static_cast<int>(fValue) );
#endif //DUNGEON_RANK_NEW
						//}} JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
					}
					std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					strStream << GET_STRING( STR_ID_14353 ) << static_cast< int >( fValue );
#else
					strStream << L"HP+ " << static_cast< int >( fValue );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					D3DXVECTOR3 pos = pCX2GUUser->GetPos();
					pos.y += 50.0f + (i * 30.0f);
					if( NULL != g_pData->GetPicCharGreen() )
						g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );


				}				
			}
			break;
		case CX2Item::SAT_MP_UP:
			{
				float fIncreaseRateMpUp = fIncreaseRateFromMasterOfAlchemistSkill;
				float fValue = static_cast< float >( pSa->m_Value1 );
				//{{ kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)
#ifdef	GUILD_SKILL
				fValue *= fIncreaseRateMpUp;
#endif	GUILD_SKILL
				//}} kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)

				if( pCX2GUUser->IsMyUnit() == true )
				{
					pCX2GUUser->UpNowMp( fValue );
				}
				std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				strStream << GET_STRING( STR_ID_14354 ) << static_cast< int >( fValue );
#else
				strStream << L"MP+ " << static_cast< int >( fValue );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
					g_pData->GetPicCharBlue()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );

			}
			break;
		case CX2Item::SAT_REMOVE_CURSE:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
#ifdef CHUNG_SECOND_CLASS_CHANGE
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_CURSE);

#ifdef ADJUST_DELETE_BUFF_ITEM
					pCX2GUUser->EraseBuffTempletFromGameUnit( BTI_DEBUFF_CURSE );
#endif //ADJUST_DELETE_BUFF_ITEM

#else
					pCX2GUUser->AccessExtraDamagePack().m_Curse.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
				}
				//std::wstringstream strStream;
				//strStream << L"MP+ " << pSa->m_Value1;
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
				{
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					g_pData->GetPicCharBlue()->DrawText( GET_STRING( STR_ID_14358 ), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#else
					g_pData->GetPicCharBlue()->DrawText( L"REMOVE CURSE", pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				}
			}
			break;
		case CX2Item::SAT_REMOVE_SLOW:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
#ifdef CHUNG_SECOND_CLASS_CHANGE
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_SLOW);
#else
					pCX2GUUser->AccessExtraDamagePack().m_Slow.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
				}
				//std::wstringstream strStream;
				//strStream << L"MP+ " << pSa->m_Value1;
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
				{
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					g_pData->GetPicCharBlue()->DrawText( GET_STRING( STR_ID_14359 ), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#else
					g_pData->GetPicCharBlue()->DrawText( L"REMOVE SLOW", pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				}
			}
			break;
		case CX2Item::SAT_REMOVE_FIRE:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
#ifdef CHUNG_SECOND_CLASS_CHANGE
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_FIRE);
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_ENCHANT_BLAZE);

#ifdef ADJUST_DELETE_BUFF_ITEM
					pCX2GUUser->EraseBuffTempletFromGameUnit( BTI_DEBUFF_FIRE );
#endif //ADJUST_DELETE_BUFF_ITEM

#else
					pCX2GUUser->AccessExtraDamagePack().m_Fire.Init();
					pCX2GUUser->AccessExtraDamagePack().m_EnchantBlaze.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
				}
				//std::wstringstream strStream;
				//strStream << L"MP+ " << pSa->m_Value1;
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
				{
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					g_pData->GetPicCharBlue()->DrawText( GET_STRING( STR_ID_14360 ), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#else
					g_pData->GetPicCharBlue()->DrawText( L"REMOVE FIRE", pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				}
			}
			break;
		case CX2Item::SAT_REMOVE_POISON:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
#ifdef CHUNG_SECOND_CLASS_CHANGE
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_POISON);
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_ENCHANT_POISON);

#ifdef ADJUST_DELETE_BUFF_ITEM
					pCX2GUUser->EraseBuffTempletFromGameUnit( BTI_DEBUFF_POISON );
#endif //ADJUST_DELETE_BUFF_ITEM

#else
					pCX2GUUser->AccessExtraDamagePack().m_Poison.Init();
					pCX2GUUser->AccessExtraDamagePack().m_EnchantPoison.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
				}
				//std::wstringstream strStream;
				//strStream << L"MP+ " << pSa->m_Value1;
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
				{
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					g_pData->GetPicCharBlue()->DrawText( GET_STRING( STR_ID_14361 ), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#else
					g_pData->GetPicCharBlue()->DrawText( L"REMOVE POISON", pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				}
			}
			break;

		case CX2Item::SAT_HYPER_MODE:
			{				
				if ( pCX2GUUser->IsMyUnit() == true &&
					m_GameType == GT_PVP ) // ������ // 2012-10-15 �ʵ己Ī ���Ŀ��� �������� ȿ���� ���������� �������� ����
				{
					pCX2GUUser->SetHasHyperModeItem(true);
				}
			}
			break;

		case CX2Item::SAT_SOUL_GAGE_UP:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
	#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
					/// �ֵ��� ��, DP �������� �÷� �ּ���
					if ( NULL != pCX2GUUser->GetUnit() && CX2Unit::UT_ADD == pCX2GUUser->GetUnit()->GetType() )
						pCX2GUUser->UpNowDPValue( 2000.f );
					else
	#endif // SERV_9TH_NEW_CHARACTER
	#ifdef HYPER_MODE_MAX_POTION
					{
						int iNowSoul = static_cast<int>(pCX2GUUser->GetNowSoul()) + pSa->m_Value1;
						int iNowHyper = pCX2GUUser->GetHyperModeCount();
						int iPrevHyper = iNowHyper;
						while( iNowSoul >= 100 )
						{
							iNowSoul -= 100;
							++iNowHyper;
						}

						if( iNowHyper > 3 )
						{
							iNowHyper = 3;
						}

						if( iPrevHyper < iNowHyper )
						{
							iNowSoul = 0;
						}

						pCX2GUUser->SetHyperModeCount( iNowHyper );
						pCX2GUUser->SetNowSoul( iNowSoul );
					}
	#else
						pCX2GUUser->UpNowSoul( static_cast<float>( pSa->m_Value1 ) );
	#endif HYPER_MODE_MAX_POTION
				}
				
				std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				strStream << GET_STRING( STR_ID_14355 ) << pSa->m_Value1;
#else
				strStream << L"SOUL+ " << pSa->m_Value1;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharRed() )
					g_pData->GetPicCharRed()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
			}
			break;

#ifdef SPECIAL_ABILITY_WAY_OF_SWORD
		case CX2Item::SAT_WAY_OF_SWORD_GAUGE_UP:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
					pCX2GUUser->SetWayOfSwordPoint( pCX2GUUser->GetWayOfSwordPoint() + pSa->m_Value1 );
				}
			}
			break;
#endif SPECIAL_ABILITY_WAY_OF_SWORD

		case CX2Item::SAT_SHOW_OPPONENT_MP:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
					pCX2GUUser->AccessCashItemAbility().m_bShowOppnentMP = true;
				}
			}
			break;

		case CX2Item::SAT_UP_MP_AT_ATTACK_OR_DAMAGE:
			{
				pCX2GUUser->AccessCashItemAbility().m_fUpMPAtAttackOrDamage = (float) pSa->m_Value1;	// ������, // fix!! ��ø�� �ȵǴ� ������ ����. 
			} 
			break;

		case CX2Item::SAT_HP_PERCENT_UP:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					float fIncreateRateHpPercentUp = fIncreaseRateFromMasterOfAlchemistSkill;
					float fValue = pSa->m_Value1 / 100.0f;

					//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
					// ǻ������� ������� ����
					if ( CX2Unit::UC_CHUNG_FURY_GUARDIAN == pCX2GUUser->GetUnitClass() || CX2Unit::UC_CHUNG_IRON_PALADIN == pCX2GUUser->GetUnitClass() )
					{
						const CX2UserSkillTree& userSkillTree = pCX2GUUser->GetUnit()->GetUnitData().m_UserSkillTree;

		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, true );
		#else // UPGRADE_SKILL_SYSTEM_2013
						int iSkillLv = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );
		#endif // UPGRADE_SKILL_SYSTEM_2013
						
						if ( 0 < iSkillLv )
						{
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							const CX2SkillTree::SkillTemplet* pSkillTemplet = 
								g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST );

							if ( NULL != pSkillTemplet )
							{
								float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL, iSkillLv );

								fIncreaseRateMetabolism = CalculateIncreasingRate( fIncreaseRateMetabolism );
								fIncreateRateHpPercentUp += fIncreaseRateMetabolism;
							}
		#else // UPGRADE_SKILL_SYSTEM_2013
							const CX2SkillTree::SkillTemplet* pSkillTemplet = 
								g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CFG_METABOLISM_BOOST, iSkillLv );

							float fIncreaseRateMetabolism = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ITEM_SPECIAL_ABILITY_REL );

							fIncreaseRateMetabolism = CalculateIncreasingRate( fIncreaseRateMetabolism );
							fIncreateRateHpPercentUp += fIncreaseRateMetabolism;
		#endif // UPGRADE_SKILL_SYSTEM_2013
						} // if
					}
#endif	CHUNG_FIRST_CLASS_CHANGE
					//}} kimhc // 2011.1.14 // û 1�� ����

					//{{ kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)
#ifdef	GUILD_SKILL
					if ( bDoIHaveMasterOfAlchemistSkill == true &&
                        EGG_ITEM_ID == pItemTemplet->GetItemID()
                         )
					{
						fValue += ( iSkillLvMasterOfAlchemist / 100.0f );
					}
#endif	GUILD_SKILL
					//}} kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)
					fValue *= fIncreateRateHpPercentUp;

					if( pCX2GUUser->IsMyUnit() == true )
					{
#ifdef HP_PERCENT_DOWN_NO_DIE
						if( pItemTemplet->GetItemID() == 152000194 )
							pCX2GUUser->UpNowHp( pCX2GUUser->GetMaxHp() * fValue , 1.0f );
						else
#endif // HP_PERCENT_DOWN_NO_DIE
						pCX2GUUser->UpNowHp( pCX2GUUser->GetMaxHp() * fValue );
					}
						//{{ JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����
#ifdef DUNGEON_RANK_NEW
						// ���ڿ��� ���� HP ȸ�� ���� �ƴ� ��� ����
						if ( pItemTemplet->GetEqipPosition() != CX2Unit::EP_NONE )
							pCX2GUUser->AddRecoveryHP( static_cast<int>(fValue) );
#endif //DUNGEON_RANK_NEW
						//}} JHKang / ������ / 2011/02/14 / ���� ��ũ ���� ����

					std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					strStream << GET_STRING( STR_ID_14353 ) << static_cast< int >( fValue * 100 ) << GET_STRING( STR_ID_14356 );
#else
					strStream << L"HP+ " << static_cast< int >( fValue * 100 ) << L" PERCENT";
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC

					D3DXVECTOR3 pos = pCX2GUUser->GetPos();
					pos.y += 50.0f + (i * 30.0f);
					if( NULL != g_pData->GetPicCharGreen() )
						g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );

				}				
			}
			break;
//{{ 09.06.08 �¿� : MP �ۼ�Ʈ�� ä��� ��ų
		case CX2Item::SAT_MP_PERCENT_UP:
			{
				float fValue = pSa->m_Value1 / 100.0f;
				//{{ kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)
#ifdef	GUILD_SKILL
				if ( bDoIHaveMasterOfAlchemistSkill == true )
					fValue *= fIncreaseRateFromMasterOfAlchemistSkill;
#endif	GUILD_SKILL
				//}} kimhc // 2009-11-17 //��� ��ų �� ���ݼ��� ����(����, ����, ���� ȸ�� �������� ȿ���� X%����)

				if( pCX2GUUser->IsMyUnit() == true )
					pCX2GUUser->UpNowMp( pCX2GUUser->GetMaxMp() * fValue );

				std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				strStream << GET_STRING( STR_ID_14354 ) << static_cast< int >( pSa->m_Value1 * fIncreaseRateFromMasterOfAlchemistSkill ) << GET_STRING( STR_ID_14356 );
#else
				strStream << L"MP+ " << static_cast< int >( pSa->m_Value1 * fIncreaseRateFromMasterOfAlchemistSkill ) << L" PERCENT";
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
					g_pData->GetPicCharBlue()->DrawText( strStream.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
			}
			break;
//}}

		case CX2Item::SAT_TEAM_HP_UP:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					for( UINT i=0; i<m_UserUnitList.size(); i++ )
					{
						CX2GUUser* pGUUser = m_UserUnitList[i];
						if( NULL != pGUUser &&
							pGUUser->GetNowHp() > 0.f &&
							pGUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
						{
							if( pGUUser->GetTeam() == pCX2GUUser->GetTeam() )
							{
								pGUUser->UpNowHp( static_cast<float>( pSa->m_Value1 ) );

								std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
								strStream << GET_STRING( STR_ID_14353 ) << pSa->m_Value1 << GET_STRING( STR_ID_14356 );
#else
								strStream << L"HP+ " << pSa->m_Value1 << L" PERCENT";
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
								D3DXVECTOR3 pos = pGUUser->GetPos();
								pos.y += 50.0f + (i * 30.0f);
								if( NULL != g_pData->GetPicCharGreen() )
								{
#ifdef ALWAYS_SCREEN_SHOT_TEST
									if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false )
									{
										g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pGUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
									}
#else
										g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pGUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif ALWAYS_SCREEN_SHOT_TEST
								}
							}
						}
					}
				}				
			} break;


		case CX2Item::SAT_TEAM_MP_UP:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					for( UINT i=0; i<m_UserUnitList.size(); i++ )
					{
						CX2GUUser* pGUUser = m_UserUnitList[i];
						if( NULL != pGUUser &&
							pGUUser->GetNowHp() > 0.f &&
							pGUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
						{
							if( pGUUser->GetTeam() == pCX2GUUser->GetTeam() )
							{
								pGUUser->UpNowMp( static_cast<float>( pSa->m_Value1 ) );

								std::wstringstream strStream;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
								strStream << GET_STRING( STR_ID_14354 ) << pSa->m_Value1 << GET_STRING( STR_ID_14356 );
#else
								strStream << L"MP+ " << pSa->m_Value1 << L" PERCENT";
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
								D3DXVECTOR3 pos = pGUUser->GetPos();
								pos.y += 50.0f + (i * 30.0f);
								if( NULL != g_pData->GetPicCharGreen() )
								{
#ifdef ALWAYS_SCREEN_SHOT_TEST
									if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false )
									{
										g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pGUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
									}
#else
									g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pGUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif ALWAYS_SCREEN_SHOT_TEST
								}
							}
						}
					}
				}				
			} break;
			
		case CX2Item::SAT_PEPPER_RUN:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					pCX2GUUser->StartPepperRun( (float)pSa->m_Value1 );	
				}
			} break;

#ifdef DUNGEON_ITEM
		case CX2Item::SAT_MP_REGENERATION:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					pCX2GUUser->SetChangeRateByItem( static_cast<float>( pSa->m_Value1 ), static_cast<float>( pSa->m_Value2 ) );
				}
			}
			break;
		case CX2Item::SAT_USE_HYPER:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
		#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
					/// �ֵ��� ��, DP �������� �÷� �ּ���
					if ( NULL != pCX2GUUser->GetUnit() && CX2Unit::UT_ADD == pCX2GUUser->GetUnit()->GetType() )
						pCX2GUUser->UpNowDPValue( 2000.f );
					else
		#endif //SERV_9TH_NEW_CHARACTER
						pCX2GUUser->ForceChangeHyperModeWithoutMotion( (float)pSa->m_Value1 );

					if( GetFocusUnit() != NULL )
						GetFocusUnit()->PlaySound( L"Alarm_Fever.ogg" );
					else if( GetMyUnit() != NULL )
						GetMyUnit()->PlaySound( L"Alarm_Fever.ogg" );
				}
			}
			break;			
		case CX2Item::SAT_SUPERARMOR:
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					pCX2GUUser->SetSuperArmor(true, (float)pSa->m_Value1 );
				}
			}
			break;		
		case CX2Item::SAT_SUMMON_SPIRIT:	// ���ɼ�ȯ
			{
				if( pCX2GUUser != NULL 
					&& pCX2GUUser->GetNowHp() > 0.0f 
					&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
				{
					// ���ɼ�ȯ�� Ÿ�ӽ�ž
					//StopAllUnit(1.f, &m_vDropItemPos, 3000.f, pCX2GUUser, 1.f);					

					wstring wstrSpiritName = SummonSpiritEffectName( pSa->m_Value1 );
					CX2EffectSet::Handle hSpirit = GetEffectSet()->PlayEffectSet( wstrSpiritName.c_str(), (CX2GameUnit*) pCX2GUUser, NULL, false, -1.f, -1.f, D3DXVECTOR3(1.f, 1.f, 1.f), true, m_vDropItemPos ); 
					m_vechEffectSetSpirit.push_back(hSpirit);

					switch( pSa->m_Value1 )
					{
					case 1:	// ���������� �г�			
						SetWorldColor_LUA( D3DXCOLOR(1.f,0.2f,0.2f,1.f) );
						pCX2GUUser->PlaySound(L"Summon_Oread.ogg");
						break;
					case 2:	// ����̾���� �г�
						SetWorldColor_LUA( D3DXCOLOR(0.2f,1.f,0.2f,1.f) );
						pCX2GUUser->PlaySound(L"Summon_Dryad.ogg");
						break;
					case 3:	// ����� �г�			
						SetWorldColor_LUA( D3DXCOLOR(0.2f,0.2f,1.f,1.f) );
						pCX2GUUser->PlaySound(L"Summon_Raul.ogg");
						break;
					case 4:	// �ϸ��Ǿ��� �ձ�
						SetWorldColor_LUA( D3DXCOLOR(0.5f,0.5f,0.2f,1.f) );
						pCX2GUUser->PlaySound(L"Summon_Elifea1.ogg");
						break;
					default:
						break;
					}

					if( pSa->m_Value1 == 1 || pSa->m_Value1 == 3 )
					{
						// ����� �г� ������ ����Ʈ ����
						m_vecEffectPos.clear();
						m_optrItemDamageEffectGameUnit = pCX2GUUser;
						m_fItemDamageEffectTime = 0.85f;
						m_iItemDamageEffectIndex = pSa->m_Value1;
					}
				}
			}
			break;			
#endif // DUNGEON_ITEM
		case CX2Item::SAT_REMOVE_FROZEN:
			{
				if( pCX2GUUser->IsMyUnit() == true )
				{
#ifdef CHUNG_SECOND_CLASS_CHANGE
					pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_COLD);
					//pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_FROZEN);
					//pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_WATER_HOLD);
					//pCX2GUUser->AccessExtraDamagePack().Init(CX2DamageManager::EDT_ENCHANT_FROZEN);

#ifdef ELSWORD_WAY_OF_SWORD
					pCX2GUUser->CureExtraDamage( CX2DamageManager::EDT_FROZEN );
					pCX2GUUser->CureExtraDamage( CX2DamageManager::EDT_WATER_HOLD );
					pCX2GUUser->CureExtraDamage( CX2DamageManager::EDT_ENCHANT_FROZEN );
#endif

#else
					pCX2GUUser->AccessExtraDamagePack().m_Cold.Init();
					pCX2GUUser->AccessExtraDamagePack().m_Frozen.Init();
					pCX2GUUser->AccessExtraDamagePack().m_EnchantFrozen.Init();
					pCX2GUUser->AccessExtraDamagePack().m_WaterHold.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
				}
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 50.0f + (i * 30.0f);
				if( NULL != g_pData->GetPicCharBlue() )
					g_pData->GetPicCharBlue()->DrawText( L"REMOVE FROZEN", pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
			}
			break;
#ifdef SPECIAL_USE_ITEM 
		case CX2Item::SAT_SPECIAL_SKILL:
			{
				g_pData->GetUIManager()->GetUIQuickSlot()->SetWaiting_EGS_USE_QUICK_SLOT_ACK( false );

				if ( pSa->m_Value2 == CX2Item::SSMT_THROW )
				{
					pCX2GUUser->SetThrowItemValue( pSa->m_Value1 );
					pCX2GUUser->StateChange( CX2GUUser::USI_THROW_ITEM, true );
					//pCX2GUUser->StateChange( CX2GUUser::USI_THROW_ITEM, pSa->m_Value1, true );
					//pCX2GUUser->StateChange( CX2GUElsword_SwordMan::ESSI_THROW_WOODEN_PIECE, pSa->m_Value1 , true );
				}
				else if ( pSa->m_Value2 == CX2Item::SSMT_INSTALL )
				{
					pCX2GUUser->StateChange( CX2GUUser::USI_INSTALL_ITEM, true );
					//pCX2GUUser->StateChange( CX2GUElsword_SwordMan::ESSI_SUMMON_MAGIC_SPEAR, true );
				}
				else if ( pSa->m_Value2 == CX2Item::SSMT_DIRECT_USE )//���� : Ư�� ������ ��� - �ٶ�����
				{
					if( pSa->m_Value1 == CX2Item::SST_FIRE_SPHERE )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"SPECIAL_ITEM_EFFECT_FIRE", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
					else if( pSa->m_Value1 == CX2Item::SST_WATER_SPHERE )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"SPECIAL_ITEM_EFFECT_WATER", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
					else if( pSa->m_Value1 == CX2Item::SST_NATURE_SPHERE )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"SPECIAL_ITEM_EFFECT_NATURE", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
					else if( pSa->m_Value1 == CX2Item::SST_WIND_SPHERE )
					{
						// ���� : �ٶ��� ���� ��ü �� �ؿ� ����Ʈ�� �ѹ� �ѷ��ְ�(1), ������ �ɾ� ��� �Ѵ�(2)
						// ���⼭�� ����Ʈ ������ ���ô�.
						
						// ����� �г� ������ ����Ʈ ����
						m_vecEffectPos.clear();
						m_optrItemDamageEffectGameUnit = pCX2GUUser;
						m_fItemDamageEffectTime = 0.1f;
						m_iItemDamageEffectIndex = pSa->m_Value1;
						//GetDamageEffect()->CreateInstance( pCX2GUUser, L"SPECIAL_ITEM_EFFECT_WIND", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
					else if( pSa->m_Value1 == CX2Item::SST_LIGHT_SPHERE )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"SPECIAL_ITEM_EFFECT_LIGHT", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
					else if( pSa->m_Value1 == CX2Item::SST_DARK_SPHERE )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"SPECIAL_ITEM_EFFECT_DARK", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
#ifdef SERV_RELATIONSHIP_EVENT_INT
					else if( pSa->m_Value1 == CX2Item::SST_LOVE_LV1 )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"RELATIONSHIP_SKILL_THROW_LV1", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
					else if( pSa->m_Value1 == CX2Item::SST_LOVE_LV2 )
					{
						GetDamageEffect()->CreateInstance( pCX2GUUser, L"RELATIONSHIP_SKILL_THROW_LV2", 1, pCX2GUUser->GetPos(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetRotateDegree(), pCX2GUUser->GetPos().y );
					}
#endif SERV_RELATIONSHIP_EVENT_INT
				}

				SetEnableAllKeyProcess( true );
			}
			break;
#endif SPECIAL_USE_ITEM

//{{ mauntain : ����ȯ [2012.06.14] ���� ī�� ��ȯ ��� - ī�� ���� �ش� ���� ��ȯ
#ifdef SUMMON_MONSTER_CARD_SYSTEM
		case CX2Item::SAT_SUMMON_MONSTER:
			{
				switch ( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_DUNGEON_GAME:
#ifndef FIX_FIELD_SUMMON_MONSTER// �ʵ忡�� ���� ī�� ��ȯ �� �� ������ �߰�
				case CX2Main::XS_BATTLE_FIELD:
#endif // FIX_FIELD_SUMMON_MONSTER
                    CreateSummonMonsterCardNPC( pSa, pCX2GUUser );
					break;

				default:
					break;
				}
			} 
			break;
#endif SUMMON_MONSTER_CARD_SYSTEM
			//}}
#ifdef ARA_FORCE_POWER_ITEM
		case CX2Item::SAT_ARA_FORCE_POWER_PERCENT_UP:
			{
				if( pCX2GUUser != NULL &&
					pCX2GUUser->GetNowHp() > 0.0f &&
					pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE &&
					pCX2GUUser->GetUnit() != NULL && 
					pCX2GUUser->GetUnit()->GetType() == CX2Unit::UT_ARA &&
					pCX2GUUser->IsMyUnit() == true )					
				{
					CX2AraGageData*	pAraGageData = static_cast<CX2AraGageData*>( pCX2GUUser->GetGageData() );
					
					float fValue = static_cast<float>(pSa->m_Value1) / 100.0f;
					int iNowForcePower = pAraGageData->GetNowForcePower();
					int iMaxForcePower = pAraGageData->GetMaxForcePower();
					iNowForcePower += static_cast<int>(iMaxForcePower * fValue);
					
					if( iNowForcePower > iMaxForcePower )
					{
						iNowForcePower = iMaxForcePower;
					}

					pAraGageData->SetForcePowerChanged( true );
					pAraGageData->SetNowForcePower( iNowForcePower );
				}
			}
			break;
#endif ARA_FORCE_POWER_ITEM
#ifdef RIDINGPET_STAMINA_ITEM
		case CX2Item::SAT_RIDINGPET_STAMINA_PERCENT_UP:
			{
				if( pCX2GUUser != NULL &&
					pCX2GUUser->GetNowHp() > 0.0f &&
					pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE &&
					pCX2GUUser->GetUnit() != NULL && 
					pCX2GUUser->IsMyUnit() == true )					
				{
					if(!CX2RidingPetManager::GetInstance()->SetRidingPetStaminaPercentUP(pSa->m_Value1))
					{
						// �����޽��� ��¿���
					}
				}
			}
			break;
#endif RIDINGPET_STAMINA_ITEM
		}
	}
}

#ifdef PET_ITEM_GM
void CX2Game::UseItemPetAbility( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser )
{
	if( pCX2GUUser != NULL 
		&& pCX2GUUser->GetNowHp() > 0.0f 
		&& pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
	{
		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetMyPet();
			if( pPet != NULL && pPet->GetXSkinAnim() != NULL )
			{
				if( pItemTemplet->GetItemID() == PET_ATTACK_SKILL_GM_ITEM_ID )
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Common_Item_PetWhistle_Attack.ogg", false, false );
					pPet->StateChange( pPet->m_AttackState, true, false );
				}
				else if( pItemTemplet->GetItemID() == PET_CHEER_SKILL_GM_ITEM_ID )
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Common_Item_PetWhistle_CheerUp.ogg", false, false );
					pPet->StateChange( pPet->m_CheerState, true, false );				
				}
			}
		}
	}
}
#endif //PET_ITEM_GM

//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ��� �� �ƹ�Ÿ ��ü
#ifdef REAL_TIME_ELSWORD

bool CX2Game::RemoveSpecialAbilityInEquip( const CX2Item::ItemTemplet* const pItemTemplet, CX2GUUser* const pCXGUUser )
{

	if ( pItemTemplet == NULL || pCXGUUser == NULL )
		return false;

	//if ( pCXGUUser->GetCashItemAbility() == NULL )
	//	return false;

	const CX2Item::SpecialAbility* pSa = NULL;
    unsigned    uNumSA = pItemTemplet->GetNumSpecialAbility();
	for( size_t i = 0; i < uNumSA; i++ )
	{
        pSa = &pItemTemplet->GetSpecialAbility( i );

		switch( pSa->GetType() )
		{
		case CX2Item::SAT_SHOW_OPPONENT_MP:
			{
				pCXGUUser->AccessCashItemAbility().m_bShowOppnentMP = false;
			}
			break;

		case CX2Item::SAT_UP_MP_AT_ATTACK_OR_DAMAGE:
			{
				pCXGUUser->AccessCashItemAbility().m_fUpMPAtAttackOrDamage = 0.0f;	// ������
			} 
			break;
		}
	}

	return true;
}

bool CX2Game::SetSpecialAbilityInEquip( const CX2Item::ItemTemplet* const pItemTemplet, CX2GUUser* const pCXGUUser )
{
	if ( pItemTemplet == NULL || pCXGUUser == NULL )
		return false;

	//if ( pCXGUUser->GetCashItemAbility() == NULL )
	//	return false;

	const CX2Item::SpecialAbility* pSa = NULL;
    unsigned    uNumSA = pItemTemplet->GetNumSpecialAbility();
	for( size_t i = 0; i < uNumSA; i++ )
	{
        pSa = &pItemTemplet->GetSpecialAbility( i );

		switch( pSa->GetType() )
		{
		case CX2Item::SAT_SHOW_OPPONENT_MP:
			{
				pCXGUUser->AccessCashItemAbility().m_bShowOppnentMP = true;
			}
			break;

		case CX2Item::SAT_UP_MP_AT_ATTACK_OR_DAMAGE:
			{
				pCXGUUser->AccessCashItemAbility().m_fUpMPAtAttackOrDamage = static_cast< float >( pSa->m_Value1 );	// ������
			} 
			break;
		}
	}

	return true;
}

#endif REAL_TIME_ELSWORD
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ��� �� �ƹ�Ÿ ��ü

#ifdef DIALOG_SHOW_TOGGLE
void CX2Game::SetGameScore()
{
	if ( g_pX2Room != NULL && g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_bObserver == false &&
		g_pX2Room->GetRoomType() != CX2Room::RT_TRAININGCENTER
		)
	{
		WCHAR texBuf[30] = {0,};
		StringCchPrintf( texBuf, 30, L"%09d", m_GameScore );
		//wsprintf( texBuf, L"%09d", m_GameScore );
		if( NULL != g_pData->GetPicCharGameScore() )
		{
			g_pData->GetPicCharGameScore()->Clear();
			g_pData->GetPicCharGameScore()->DrawText( texBuf, D3DXVECTOR3(290,-40,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT, 0.03f );
		}
	}	
}
#endif

void CX2Game::SetGameScore( int gameScore )
{ 

	m_GameScoreBack	= m_GameScore;
	m_GameScore		= gameScore; 

#ifdef DIALOG_SHOW_TOGGLE	
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
		if( g_pData->GetPicCharGameScore() != NULL )		
			g_pData->GetPicCharGameScore()->Clear();
		
		return;
	}
#endif

	if( m_GameScoreBack != m_GameScore )
	{
		if ( g_pX2Room != NULL && g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_bObserver == false &&
			g_pX2Room->GetRoomType() != CX2Room::RT_TRAININGCENTER
			)
		{
			WCHAR texBuf[30] = {0,};
			StringCchPrintf( texBuf, 30, L"%09d", m_GameScore );
			//wsprintf( texBuf, L"%09d", m_GameScore );
			if( NULL != g_pData->GetPicCharGameScore() )
			{
				g_pData->GetPicCharGameScore()->Clear();
				g_pData->GetPicCharGameScore()->DrawText( texBuf, D3DXVECTOR3(290,-40,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT, 0.03f );

			}
		}		
	}
}
void CX2Game::AddGameScore( int gameScore )
{
	m_GameScoreBack = m_GameScore;
	m_GameScore		+= gameScore; 

	return;

#ifdef DIALOG_SHOW_TOGGLE	
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
		if( g_pData->GetPicCharGameScore() != NULL )		
			g_pData->GetPicCharGameScore()->Clear();

		return;
	}
#endif


	if( m_GameScoreBack != m_GameScore )
	{
		if ( g_pX2Room != NULL && g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_bObserver == false &&
			g_pX2Room->GetRoomType() != CX2Room::RT_TRAININGCENTER
			)
		{
			WCHAR texBuf[30] = {0,};
			StringCchPrintf( texBuf, 30, L"%09d", m_GameScore );
			//wsprintf( texBuf, L"%09d", m_GameScore );
			if( NULL != g_pData->GetPicCharGameScore() )
			{
				//g_pData->GetPicCharGameScore()->Clear();
				//g_pData->GetPicCharGameScore()->DrawText( texBuf, D3DXVECTOR3(290,-40,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT, 0.03f );
			}
		}

	}
}

void CX2Game::AddGameScore( int gameScore, float fTime )
{

	ScoreTime* pScoreTime = new ScoreTime();
	pScoreTime->m_Score = gameScore;
	pScoreTime->m_fTime = fTime;

	m_ScoreTimeList.push_back( pScoreTime );
}





#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / ���� �÷��� ��, �ҷ� ���� ���� ���� �ý���

int CX2Game::GetSubStageTechPoint () const
{
	return m_TechPointFull - m_iSubStageTechPoint;		// ������ TechPoint �� ���� SubStage �� ������ TechPoint �� ���� ���� �ִ´�.

}

void CX2Game::ResetSubStageTechPoint()
{
	m_iSubStageTechPoint = m_TechPointFull;	
}

#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM



void CX2Game::SetTechPoint( int techPoint )
{

	m_TechPoint			= techPoint;
	m_TechPointFull		= m_TechPoint;
	if( NULL != g_pData->GetPicCharTechPoint() )
	{
		g_pData->GetPicCharTechPoint()->Clear();
		g_pData->GetPicCharTechPoint()->DrawText( m_TechPoint, D3DXVECTOR3(185,310,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT );
	}

	if( m_fTechPointViewTime <= 0.0f )
	{
	}

	m_fTechPointViewTime = 1.5f;
}

void CX2Game::AddTechPoint( int techPoint )
{

	m_TechPoint			+= techPoint;
	m_TechPointFull		+= techPoint;

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
		m_fTechPointViewTime = 1.5f;
		return;
	}
#endif


	m_fTechPointViewTime = 1.5f;
}


void CX2Game::PauseNPCAI( float fTime )
{
	m_fTimePauseNPCAI = fTime;
	for( UINT i=0; i<m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pGUNPC = m_NPCUnitList[i];
		if( NULL == pGUNPC )
			continue;

		pGUNPC->SetAIEnable( false );
	}
}


// ��� �̻� ġƮ�����θ� ���
void CX2Game::KillAllNPC_LUA( bool bActiveOnly /* = true */ )
{
#ifndef _SERVICE_
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( NULL != GetMyUnit() )
		{
			KillAllNPC( bActiveOnly, GetMyUnit(), true );
		}
		else
		{
			KillAllNPC( bActiveOnly, NULL, true );
		}
	}
#endif
}


void CX2Game::KillAllNPC( bool bActiveOnly /* = true */, CX2GameUnit* pAttackerGameUnit_ /*= NULL*/, bool bExceptGate /*= false*/ )
{
#ifndef _SERVICE_
	if( IsHost() == true )
	{
		for( int i=0; i<GetNPCUnitListSize(); i++ )
		{
			CX2GUNPC* pCX2GUNPC = GetNPCUnit(i);
			if( pCX2GUNPC != NULL )
			{				
#if 1
				if( pCX2GUNPC->GetNPCTemplet().m_ClassType == CX2UnitManager::NCT_THING_CHECKER )
					continue;

				if( true == bExceptGate )
				{
					//if( NULL != pCX2GUNPC->GetNPCTemplet() )
					{
						if( CX2UnitManager::NCT_THING_GATE == pCX2GUNPC->GetNPCTemplet().m_ClassType )
						{
							continue;
						}
					}
				}

#if 1
				if( bActiveOnly == true &&
					pCX2GUNPC->IsActiveMonster() == false )
					continue;

		
				pCX2GUNPC->SetGameUnitWhoAttackedMe( pAttackerGameUnit_ );
				pCX2GUNPC->SetSelfResurrection( false );

#ifdef REFACTORING_BY_TOOL_TEAM
				pCX2GUNPC->UpNowHp( -pCX2GUNPC->GetMaxHp() );
#else REFACTORING_BY_TOOL_TEAM
				pCX2GUNPC->SetNowHp(0.f);
#endif REFACTORING_BY_TOOL_TEAM

				
#else
				pCX2GUNPC->TestDyingStart();
#endif
#endif				
			}
		}
	}
#endif
}

void CX2Game::KillNPC( const CX2UnitManager::NPC_UNIT_ID eNPCID, int nCount )
{


	if( false == IsHost() )
		return;

	if( nCount == 0 )
		return;


	THEMIDA_ENCODE_START

	int iCount = 0;
	for( int i=0; i<GetNPCUnitListSize(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = GetNPCUnit(i);
		if( NULL == pCX2GUNPC )
			continue;

		//if( NULL == pCX2GUNPC->GetNPCTemplet() )
		//	continue;

		if( eNPCID != pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID )
			continue;


		pCX2GUNPC->ResetGameUnitWhoAttackedMe();
		pCX2GUNPC->SetNowHp( 0.f );
		++iCount;

		if( iCount >= nCount )
			break;
	}


	THEMIDA_ENCODE_END
}

void CX2Game::EnableAllNPCAI( bool bEnable )
{
	if( IsHost() == true )
	{
#ifdef LIGHT_OPERATOR_ACCOUNT
		if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
		{
			return ;
		}
#endif LIGHT_OPERATOR_ACCOUNT

		for( int i=0; i<GetNPCUnitListSize(); i++ )
		{
			CX2GUNPC* pNPC = (CX2GUNPC*) GetNPCUnit(i);
			if( NULL == pNPC )
				continue;

			pNPC->SetAIEnable( bEnable );
		}
	}
}


void CX2Game::EnableAllNPCEventProcess( bool bEnable )
{
	//if( IsHost() == true )
	{
		for( int i=0; i<GetNPCUnitListSize(); i++ )
		{
			CX2GUNPC* pNPC = (CX2GUNPC*) GetNPCUnit(i);
			if( NULL == pNPC )
				continue;

			pNPC->SetEnableEventProcess( bEnable );
		}
	}
}


void CX2Game::SetAllNPCMPFull()
{
	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
		return;

#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return;
	}
#endif LIGHT_OPERATOR_ACCOUNT

	if( IsHost() == true )
	{
		for( int i=0; i<GetNPCUnitListSize(); i++ )
		{
			CX2GUNPC* pNPC = (CX2GUNPC*) GetNPCUnit(i);
			if( NULL == pNPC )
				continue;

			pNPC->SetNowMp( pNPC->GetMaxMp() );
		}
	}
}

void CX2Game::SetEnableAllNPCConditionTable( bool bEnable )
{
	for( UINT i=0; i< m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pGUNPC = m_NPCUnitList[i];
		if( pGUNPC == NULL )
			continue;

		pGUNPC->SetEnableCondtionTableEventProcess( bEnable );
	}

}
void CX2Game::SetFreeCamera( bool bFreeCamera )
{
	m_bFreeCamera = bFreeCamera;
#ifdef KEYFRAME_CAMERA
	if(true == bFreeCamera)
	{
		GetX2Camera()->GetCamera().SetFixedUpVec(false);
	}
	else
	{
		GetX2Camera()->GetCamera().SetFixedUpVec(true);
	}

#endif KEYFRAME_CAMERA


	if( true == m_bFreeCamera )
	{
		D3DXVECTOR3 vEye	= m_pCamera->GetCamera().GetEye();
		D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();
#ifdef KEYFRAME_CAMERA
		D3DXVECTOR3 vUpVec	= m_pCamera->GetCamera().GetUpVec();
		m_FPSCamera.SetViewParams( &vEye, &vLookAt, &vUpVec );
#else
		m_FPSCamera.SetViewParams( &vEye, &vLookAt );
#endif KEYFRAME_CAMERA

#ifdef MODIFY_GAME_EDIT_CAMERA
		m_FPSCamera.ClearKeys();
#endif //MODIFY_GAME_EDIT_CAMERA
	}
#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
	else
	{
		GetX2Camera()->CalcManualCameraPosition( GetFocusUnit(), *m_FPSCamera.GetEyePt() );
	}
#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST
}

#ifdef KEYFRAME_CAMERA
void CX2Game::SetWorldCameraEdit( bool bEnable)
{
	m_bWorldCameraEdit = bEnable;
	GetX2Camera()->SetWorldCameraEdit(bEnable);
	return;
}
#endif KEYFRAME_CAMERA
// oasis907 : ����� [2010.12.13] // ���� �Ľ� ���� ������ ���� ����
void CX2Game::PlayWorldCamera( int iWorldCameraID )
{
#ifdef KEYFRAME_CAMERA
	GetX2Camera()->StartWorldCamera(iWorldCameraID);
#endif KEYFRAME_CAMERA
	return;
}

bool CX2Game::GoStage( int iStageIndex, int iSubStageIndex /*= 0*/  )
{
#ifdef NEXON_QA_CHEAT_REQ
	if( GetGameType() != GT_DUNGEON )
		return false;

#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return false;
	}
#endif LIGHT_OPERATOR_ACCOUNT
	
	CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
	if( NULL != pDungeonGame )
	{
		CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

		if( NULL != pDungeon &&
			NULL != pDungeon->GetNowStage() &&
			NULL != pDungeon->GetNowStage()->GetNowSubStage() )
		{
			if( true == pDungeon->IsStageExist( iStageIndex, iSubStageIndex ) )
			{
				CX2DungeonSubStage* pSubStage = pDungeon->GetNowStage()->GetNowSubStage();
				pSubStage->ClearSubStage( CX2DungeonSubStage::CT_STAGE, iStageIndex, 0 );

				return true;
			}
			else
				return false;
		}
	}
#endif // NEXON_QA_CHEAT_REQ
	return false;
}

#ifdef SECRET_VELDER
int CX2Game::GetNowStageIndex()
{
	if( GetGameType() != GT_DUNGEON )
		return false;

	CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
	if( NULL != pDungeonGame )
	{
		CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

		if( NULL != pDungeon && NULL != pDungeon->GetNowStage() )
			return pDungeon->GetNowStageIndex();
	}
	
	return -1;
}
#endif


void CX2Game::UpdateSkillSlotUI(bool bUpdateEquipSkill /*= false*/)
{   

	if( NULL == GetMyUnit() )
	{
	#ifdef SERVICE_HEAVY_LOG
		StateLog( L"upd skill my unit null" );
	#endif //SERVICE_HEAVY_LOG
		return;
	}

	if( NULL == GetMyUnit()->GetUnit() )
	{
	#ifdef SERVICE_HEAVY_LOG
		StateLog( L"upd skill unit null" );
	#endif //SERVICE_HEAVY_LOG
		return;
	}

	#ifdef REFORM_UI_SKILLSLOT
	if( true == bUpdateEquipSkill )
	{
		//�����ۿ� ���� ������ų ���� ������ ���� �߰�bool bUpdateEquipSkill = false
		if( NULL != g_pData && NULL != g_pData->GetUIManager() &&
			NULL != g_pData->GetUIManager()->GetUISkillTree() )
			g_pData->GetUIManager()->GetUISkillTree()->UpdateSkillEquipedSlotUI();
	}
	#endif //REFORM_UI_SKILLSLOT

#ifdef ELSWORD_WAY_OF_SWORD
	//���Ǳ� ��ų������ ����Ʈ
	UpdateElswordSkillSlotEffect();
#endif //ELSWORD_WAY_OF_SWORD

#ifdef EVE_ELECTRA
	//�ڵ��Ϸ�Ʈ�� ��ų������ ����
	UpdateEveElectraSkillSlotIcon();
#endif //EVE_ELECTRA

#ifdef SERV_RENA_NIGHT_WATCHER
	UpdateNightWatcherSkillSlotIcon();
#endif //SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	UpdateVeteranCommanderSkillSlotIcon();
#endif //SERV_RAVEN_VETERAN_COMMANDER

#ifdef FINALITY_SKILL_SYSTEM //��â��
	UpdateCodeEmpressSkillSlotIcon();
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_9TH_NEW_CHARACTER
	UpdateAddNasodRulerSkillSlotIcon();
#endif // SERV_9TH_NEW_CHARACTER
}

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
void CX2Game::InvalidEventThread( HANDLE hValue )
{
	CSLock locker( m_csGameIntruder );
	for ( int i = 0; i < X2_INTRUDE_EVENT_MAX_VALUE; i++ )
	{
		if ( m_hEvents[i] == hValue )
		{
			m_hEvents[i] = NULL;
			return;
		}
	}

}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2Game::ResetReBirthStoneNumUI()
{
	if ( m_pDLGMyScore != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMyScore->GetControl( L"Static_My_Stone" );

		if( pStatic != NULL && g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		{
			pStatic->SetShow(false);
			return;
		}

		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
		{
			if( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				wstringstream wstrstm;
				wstrstm << g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum();
				//{{ ����� : [2009/10/15] //	�ڵ����� ��Ȱ�� ó��
#ifdef AUTO_PAYMENT
				if( g_pData->GetMyUser()->GetSelectUnit()->GetAutoResStoneNum() > 0 )
				{
					wstrstm << L"(+" << g_pData->GetMyUser()->GetSelectUnit()->GetAutoResStoneNum() << L")";
				}
#endif	//	AUTO_PAYMENT
				//}} ����� : [2009/10/15] //	�ڵ����� ��Ȱ�� ó���ڵ� �߰�
				pStatic->GetString(0)->msg = wstrstm.str().c_str();
			}
		}
	}
}
#ifdef ELSWORD_WAY_OF_SWORD
void CX2Game::UpdateElswordSkillSlotEffect()
{
	if( NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		NULL != GetMyUnit() &&
#ifdef FIX_OBSERVER_MODE_CRASH
		NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && g_pX2Room->GetMySlot()->m_bObserver == false &&
#endif //FIX_OBSERVER_MODE_CRASH
		( CX2Unit::UT_ELSWORD == g_pData->GetMyUser()->GetSelectUnit()->GetType() 
#ifdef NEW_CHARACTER_EL // ���� ��
		|| CX2Unit::UT_ELESIS == g_pData->GetMyUser()->GetSelectUnit()->GetType() 
#endif // NEW_CHARACTER_EL
		) )
	{
		const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
		if ( m_pDLGSkillSlot != NULL )
		{
			CKTDGUIStatic* pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_B_B" );
			CKTDGUIStatic* pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_R_B" );

			if( NULL == pStatic_ElSkillVigor || NULL == pStatic_ElSkillDest )
				return;

			pStatic_ElSkillVigor->SetShow(true);
			pStatic_ElSkillDest->SetShow(true);

			pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_B" );
			pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_R" );

			if( NULL == pStatic_ElSkillVigor || NULL == pStatic_ElSkillDest )
				return;

			pStatic_ElSkillVigor->SetShow(true);
			pStatic_ElSkillDest->SetShow(true);

			for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT*2; i++ )
			{
				bool bSlotB = (i > 3) ? true : false;
				int iSlotIndex = (i > 3) ? i-4 : i;
				const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB);

				if( true == bSlotB )
				{
					pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_R_B" );
					if( pStatic_ElSkillDest == NULL )
						break;

					pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_B_B" );
					if( pStatic_ElSkillVigor == NULL )
						break;
				}

				pStatic_ElSkillVigor->GetPicture(iSlotIndex)->SetShow(false);
				pStatic_ElSkillDest->GetPicture(iSlotIndex)->SetShow(false);

				//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
				if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
					&& bSlotB == true)
				{
					pStatic_ElSkillDest->SetShow(false);
					pStatic_ElSkillVigor->SetShow(false);
					break;
				}

				if(pSkillData != NULL && pSkillData->m_eID != CX2SkillTree::SI_NONE )
				{
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
					const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID );

					if( pSkillTemplet != NULL )
					{
						int iWayOfSwordType = pSkillTemplet->m_iWOSType;
		#else // UPGRADE_SKILL_SYSTEM_2013
					int iLevel = refUserSkillTree.GetSkillLevel( pSkillData->m_eID );

					const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID, iLevel );

					if( pSkillTemplet != NULL )
					{
						int iWayOfSwordType = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_WAY_OF_SWORD_TYPE );
		#endif // UPGRADE_SKILL_SYSTEM_2013

						if( iWayOfSwordType == 2 )
						{
							if( GetMyUnit()->GetWayOfSwordState() == 2)  // CX2GUElsword_SwordMan::WSS_VIGOR
							{
								pStatic_ElSkillVigor->GetPicture(iSlotIndex)->SetShow(true);
								pStatic_ElSkillVigor->GetPicture(iSlotIndex)->SetFlicker( 0.4f, 1.0f, 0.1f );
								pStatic_ElSkillDest->GetPicture(iSlotIndex)->SetShow(false);
							}
						}
						else if (iWayOfSwordType == 1)
						{
							if( GetMyUnit()->GetWayOfSwordState() == -2)  // CX2GUElsword_SwordMan::WSS_DESTRUCTION
							{
								pStatic_ElSkillDest->GetPicture(iSlotIndex)->SetShow(true);
								pStatic_ElSkillDest->GetPicture(iSlotIndex)->SetFlicker( 0.4f, 1.0f, 0.1f );
								pStatic_ElSkillVigor->GetPicture(iSlotIndex)->SetShow(false);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		CKTDGUIStatic* pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_B_B" );
		if( NULL != pStatic_ElSkillVigor )
			pStatic_ElSkillVigor->SetShow(false);
		CKTDGUIStatic* pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_R_B" );
		if( NULL != pStatic_ElSkillDest )		
			pStatic_ElSkillDest->SetShow(false);

		pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_B" );
		if( NULL != pStatic_ElSkillVigor )
			pStatic_ElSkillVigor->SetShow(false);
		pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_EL_SKILL_R" );
		if( NULL != pStatic_ElSkillDest )
			pStatic_ElSkillDest->SetShow(false);
	}
	return;
}
#endif ELSWORD_WAY_OF_SWORD
void CX2Game::UpdateEveElectraSkillSlotIcon()
{
	const CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

	if (
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		( eUnitClass == CX2Unit::UC_EVE_ELECTRA ||
#ifdef SERV_EVE_BATTLE_SERAPH
		 eUnitClass == CX2Unit::UC_EVE_BATTLE_SERAPH ) &&
#endif //SERV_EVE_BATTLE_SERAPH
#endif //UPGRADE_SKILL_SYSTEM_2013
		GetMyUnit() != NULL &&
		GetMyUnit()->GetUnit() != NULL )
	{
		const CX2UserSkillTree& refUserSkillTree = GetMyUnit()->GetUnit()->GetUnitData().m_UserSkillTree;
		//B���Ե� ���� �ϱ� ���ؼ� EQUIPPED_SKILL_SLOT_COUNT*2
		for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT * 2; ++i)
		{
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;

			//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
			if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
				&& bSlotB == true)
			{
				break;
			}

			const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB );
			if( pSkillData != NULL && pSkillData->m_eID != CX2SkillTree::SI_NONE )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID );
#else // UPGRADE_SKILL_SYSTEM_2013
				int iLevel = refUserSkillTree.GetSkillLevel( pSkillData->m_eID );

				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID, iLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
				if(pSkillTemplet != NULL)
				{
					wstring wstrTextureName			= pSkillTemplet->m_wstrIconName;
					wstring wstrTexturePieceName	= pSkillTemplet->m_wstrIconPieceName;
					if( pSkillData->m_eID == CX2SkillTree::SI_A_EEL_SPECTRO_EL_CRYSTAL )
					{
						CX2GUEve* pEve = static_cast< CX2GUEve* >( GetMyUnit() );
						if( pEve->GetEnableElectraSystem() == true && pEve->GetElectraFormationType() == CX2GUEve::PFT_AMPLIFIER  )
						{
							wstrTextureName = L"DLG_SKILL_EveActive_03.tga";
							wstrTexturePieceName = L"SI_A_EEL_SPECTRO_EL_CRYSTAL_01";
						}
						else if( pEve->GetEnableElectraSystem() == true && pEve->GetElectraFormationType() == CX2GUEve::PFT_SPECTRUM  )
						{
							wstrTextureName = L"DLG_SKILL_EveActive_03.tga";
							wstrTexturePieceName = L"SI_A_EEL_SPECTRO_EL_CRYSTAL_02";
						}
						else if ( pEve->GetEnableElectraSystem() == true && pEve->GetElectraFormationType() == CX2GUEve::PFT_INDUCTION )
						{
							wstrTextureName = L"DLG_SKILL_EveActive_04.tga";
							wstrTexturePieceName = L"SI_A_EBS_SPECTRO_EL_CRYSTAL_INDUCTION";
						}
						else if ( pEve->GetEnableElectraSystem() == true && pEve->GetElectraFormationType() == CX2GUEve::PFT_FUSION )
						{
							wstrTextureName = L"DLG_SKILL_EveActive_04.tga";
							wstrTexturePieceName = L"SI_A_EBS_SPECTRO_EL_CRYSTAL_FUSION";
						}

						CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
						pSlot->SetItemTex(wstrTextureName.c_str(), wstrTexturePieceName.c_str() );
					}
					else if( pSkillData->m_eID == CX2SkillTree::SI_SA_EEL_THOUSANDS_OF_STARS )
					{
						CX2GUEve* pEve = static_cast< CX2GUEve* >( GetMyUnit() );
						if( pEve->GetThousandStarCount() > 0 &&  pEve->GetThousandStarEntireFired() == false  )
						{
							wstrTextureName = L"DLG_SKILL_EveActive_03.tga";
							wstrTexturePieceName = L"SI_SA_EEL_THOUSANDS_OF_STARS_FIRE";
						}
						
						CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
						pSlot->SetItemTex(wstrTextureName.c_str(), wstrTexturePieceName.c_str() );
					}
				}	// if
			}	//if
		}//for
	}
}


#ifdef SERV_RENA_NIGHT_WATCHER
void CX2Game::UpdateNightWatcherSkillSlotIcon()
{
	if(	g_pData->GetMyUser()->GetSelectUnit()->GetClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER && 
		GetMyUnit() != NULL )
	{
		const CX2UserSkillTree& refUserSkillTree = GetMyUnit()->GetUnit()->GetUnitData().m_UserSkillTree;
		//B���Ե� ���� �ϱ� ���ؼ� EQUIPPED_SKILL_SLOT_COUNT*2
		for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT * 2; ++i)
		{
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;

			//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
			if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
				&& bSlotB == true)
			{
				break;
			}

			const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB );
			if( pSkillData != NULL && pSkillData->m_eID != CX2SkillTree::SI_NONE )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID );
#else // UPGRADE_SKILL_SYSTEM_2013
				int iLevel = refUserSkillTree.GetSkillLevel( pSkillData->m_eID );

				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID, iLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
				if(pSkillTemplet != NULL)
				{
					wstring wstrTextureName			= pSkillTemplet->m_wstrIconName;
					wstring wstrTexturePieceName	= pSkillTemplet->m_wstrIconPieceName;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					if( pSkillData->m_eID == CX2SkillTree::SI_A_LNW_DELAYED_FIRING )
#else //UPGRADE_SKILL_SYSTEM_2013
					if( pSkillData->m_eID == CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING )
#endif //UPGRADE_SKILL_SYSTEM_2013
					{
						CX2GULire_ElvenRanger* pRena = static_cast< CX2GULire_ElvenRanger* >( GetMyUnit() );

						if( pRena->GetActiveStartOfDelayedFiring() == true )			/// ��ü ���� ��� ������
						{
							wstrTextureName = L"DLG_SKILL_LenaActive_03.tga";
							wstrTexturePieceName = L"SI_A_LNW_DELAYED_FIRE_ACTIVE";
						}
						else if( pRena->GetActiveStartOfDelayedFiring() == false )		/// ������ ��ȣź ��� ������
						{
							wstrTextureName = L"DLG_SKILL_LenaActive_03.tga";
							wstrTexturePieceName = L"SI_A_LNW_DELAYED_FIRE";
						}

						CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
						pSlot->SetItemTex(wstrTextureName.c_str(), wstrTexturePieceName.c_str() );
					}
				}	// if
			}	//if
		}//for
	}
}
#endif //SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
void CX2Game::UpdateVeteranCommanderSkillSlotIcon()
{
	if(	g_pData->GetMyUser()->GetSelectUnit()->GetClass() == CX2Unit::UC_RAVEN_VETERAN_COMMANDER && 
		GetMyUnit() != NULL )
	{
		const CX2UserSkillTree& refUserSkillTree = GetMyUnit()->GetUnit()->GetUnitData().m_UserSkillTree;
		//B���Ե� ���� �ϱ� ���ؼ� EQUIPPED_SKILL_SLOT_COUNT*2
		for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT * 2; ++i)
		{
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;

			//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
			if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
				&& bSlotB == true)
			{
				break;
			}

			const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB );
			if( pSkillData != NULL && pSkillData->m_eID != CX2SkillTree::SI_NONE )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID );
#else // UPGRADE_SKILL_SYSTEM_2013
				int iLevel = refUserSkillTree.GetSkillLevel( pSkillData->m_eID );

				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID, iLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
				if(pSkillTemplet != NULL)
				{
					wstring wstrTextureName			= pSkillTemplet->m_wstrIconName;
					wstring wstrTexturePieceName	= pSkillTemplet->m_wstrIconPieceName;
					if( pSkillData->m_eID == CX2SkillTree::SI_A_RF_SHADOW_STEP )
					{
						CX2GURaven* pRena = static_cast< CX2GURaven* >( GetMyUnit() );

						if( pRena->GetIsShadowStepIcon() == true )				/// ������ ���� ���������� ����
						{
							wstrTextureName = L"DLG_SKILL_RavenActive_01.tga";
							wstrTexturePieceName = L"SI_A_RF_SHADOW_STEP";
						}
						else if( pRena->GetIsShadowStepIcon() == false )		/// ������ �齽���̵� ���������� ����
						{
							wstrTextureName = L"DLG_SKILL_Passive_04.tga";
							wstrTexturePieceName = L"SI_P_RVC_SHADOW_BACK_SLIDE";
						}

						CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
						pSlot->SetItemTex(wstrTextureName.c_str(), wstrTexturePieceName.c_str() );
					}
				}	// if
			}	//if
		}//for
	}
}
#endif //SERV_RAVEN_VETERAN_COMMANDER

#ifdef FINALITY_SKILL_SYSTEM //��â��
void CX2Game::UpdateCodeEmpressSkillSlotIcon()
{
	if(	g_pData->GetMyUser()->GetSelectUnit()->GetClass() == CX2Unit::UC_EVE_CODE_EMPRESS && 
		GetMyUnit() != NULL )
	{
		const CX2UserSkillTree& refUserSkillTree = GetMyUnit()->GetUnit()->GetUnitData().m_UserSkillTree;
		//B���Ե� ���� �ϱ� ���ؼ� EQUIPPED_SKILL_SLOT_COUNT*2
		for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT * 2; ++i)
		{
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;

			//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
			if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
				&& bSlotB == true)
			{
				break;
			}

			const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB );
			if( pSkillData != NULL && pSkillData->m_eID != CX2SkillTree::SI_NONE )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID );
#else // UPGRADE_SKILL_SYSTEM_2013
				int iLevel = refUserSkillTree.GetSkillLevel( pSkillData->m_eID );

				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID, iLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
				if(pSkillTemplet != NULL)
				{
					wstring wstrTextureName			= pSkillTemplet->m_wstrIconName;
					wstring wstrTexturePieceName	= pSkillTemplet->m_wstrIconPieceName;
					if( pSkillData->m_eID == CX2SkillTree::SI_HA_EEP_LINK_OVERCHARGE_ILLUSION )
					{
						CX2GUEve* pEve = static_cast< CX2GUEve* >( GetMyUnit() );

						if( pEve->GetActiveLinkOverChargeIllusion() == true )				/// �������� �ñر� - ��ũ �������� �����Ʈ Ȱ��ȭ
						{
							wstrTextureName = L"DLG_SKILL_Hyper_Active_02.tga";
							wstrTexturePieceName = L"SI_HA_EEP_LINK_OVERCHARGE_ILLUSION_01";
						}
						else if( pEve->GetActiveLinkOverChargeIllusion() == false )			/// �������� �ñر� - ��ũ �������� �����Ʈ ��Ȱ��ȭ
						{
							wstrTextureName = L"DLG_SKILL_Hyper_Active_01.tga";
							wstrTexturePieceName = L"SI_HA_EEP_LINK_OVERCHARGE_ILLUSION";
						}

						CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
						pSlot->SetItemTex(wstrTextureName.c_str(), wstrTexturePieceName.c_str() );
					}
				}	// if
			}	//if
		}//for
	}
}
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
void CX2Game::UpdateAddNasodRulerSkillSlotIcon()
{
	if(	NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		CX2Unit::UT_ADD == g_pData->GetMyUser()->GetSelectUnit()->GetType() &&
		NULL != g_pData->GetSkillTree() )
	{
		CX2GUAdd* pAdd = static_cast< CX2GUAdd* >( GetMyUnit() );

		if ( NULL == pAdd )
			return;

		const CX2UserSkillTree& refUserSkillTree = GetMyUnit()->GetUnit()->GetUnitData().m_UserSkillTree;

		//B���Ե� ���� �ϱ� ���ؼ� EQUIPPED_SKILL_SLOT_COUNT*2
		for( int i = 0; i<EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;

			//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
			if( false == refUserSkillTree.GetEnabledSkillSlotB() && bSlotB == true )
				break;

			const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB );

			if( pSkillData == NULL || pSkillData->m_eID == CX2SkillTree::SI_NONE )
				continue;
			
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSkillData->m_eID );

			if( pSkillTemplet == NULL)
				continue;
			
			const wchar_t* pwstrTextureName			= pSkillTemplet->m_wstrIconName.c_str();
			const wchar_t* pwstrTexturePieceName	= pSkillTemplet->m_wstrIconPieceName.c_str();
			
			switch( pSkillData->m_eID )
			{
			case CX2SkillTree::SI_SA_APT_STASIS_FIELD:
				{
					if( true == pAdd->GetIsActiveStasisfield() )		/// �����̽ý� �ʵ� Ȱ��ȭ
					{
						pwstrTextureName = L"DLG_SKILL_ADD_Active_01.tga";
						pwstrTexturePieceName = L"SI_SA_APT_STASIS_FIELD_01";
					}

					CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
					if( NULL != pSlot )
						pSlot->SetItemTex(pwstrTextureName, pwstrTexturePieceName );
				} break;
			case CX2SkillTree::SI_A_AN_PHASE_SHIFT:
				{
					if( true == pAdd->GetIsActivePhaseShift() )
					{
						pwstrTextureName = L"DLG_SKILL_ADD_Active_01.tga";
						pwstrTexturePieceName = L"SI_A_AN_PHASE_SHIFT_01";
					}
					else
					{
		#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
						/// ȯ�� ������ Ȱ�� ����
						if ( true == pAdd->GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PHASE_SHIFT ) )
						{
							pwstrTextureName = L"DLG_SKILL_Passive_07.tga";
							pwstrTexturePieceName = L"SI_P_ALP_FANTASY_TRACER";
						}
		#endif //SERV_ADD_LUNATIC_PSYKER
					}

					CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
					if( NULL != pSlot )
						pSlot->SetItemTex(pwstrTextureName, pwstrTexturePieceName );
				} break;
		#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
			case CX2SkillTree::SI_A_AN_PULSE_BULLET:
				{
					if ( true == pAdd->GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PULSE_BULLET ) )
					{
						pwstrTextureName = L"DLG_SKILL_Passive_07.tga";
						pwstrTexturePieceName = L"SI_P_ALP_FANTASY_TRACER";
					}

					CKTDGUISlot* pSlot = g_pData->GetUIManager()->GetUISkillTree()->GetEquippedSlot(i);
					if( NULL != pSlot )
						pSlot->SetItemTex(pwstrTextureName, pwstrTexturePieceName );
				} break;
		#endif //SERV_ADD_LUNATIC_PSYKER
			default:
				break;
			}
		}
	}
}
#endif //SERV_9TH_NEW_CHARACTER

void CX2Game::UpdateSkillCoolTime( float fElapsedTime )
{
	if( g_pData != NULL &&
		g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		GetMyUnit() != NULL )
	{
		const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;	
		if ( m_pDLGSkillSlot != NULL )
		{
			CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*)m_pDLGSkillSlot->GetControl( L"Static_Skill_Cool_Time" );
			CKTDGUIStatic* pStatic_Skill_Cool_Time_Fade = (CKTDGUIStatic*)m_pDLGSkillSlot->GetControl( L"Static_Skill_Cool_Time_Fade" );
			CKTDGUIStatic* pStaticSlot_A = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_SKILL_SLOT_A" );
			CKTDGUIStatic* pStaticSlot_B = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_SKILL_SLOT_B" );
			for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; i++ )
			{
				pStaticSlot_A->GetPicture(i)->SetColor( D3DXCOLOR(1.f,1.f,1.f,0.3f));
				pStaticSlot_B->GetPicture(i)->SetColor( D3DXCOLOR(1.f,1.f,1.f,0.3f));
			}

			//���� B Ȱ��ȭ ����
			bool bEnableSlotB = false;
			if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() )
					bEnableSlotB = true;
			}

	#ifdef SKILL_SLOT_UI_TYPE_B
			//��ų���� UI Ÿ�� üũ
			bool bIsSlotUITypeA = false;
			bool bIsSkillSlotSwap = false;
			if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
				
			{
				bIsSlotUITypeA = g_pData->GetUIManager()->GetUISkillTree()->GetIsSkillUiTypeA();
				bIsSkillSlotSwap = g_pData->GetUIManager()->GetUISkillTree()->GetSkillSlotSwap();
			}
			 

	#endif //SKILL_SLOT_UI_TYPE_B

			if ( pStaticCoolTime != NULL && pStatic_Skill_Cool_Time_Fade != NULL )
			{
				for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT*2; i++ )
				{
					bool bSlotB = (i > 3) ? true : false;
					int iSlotIndex = (i > 3) ? i-4 : i;

					if( true == bSlotB )
					{
						//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
						if( false == bEnableSlotB )
						{
							break;
						}

						pStatic_Skill_Cool_Time_Fade = (CKTDGUIStatic*)m_pDLGSkillSlot->GetControl( L"Static_Skill_Cool_Time_Fade_B" );
						if( NULL == pStatic_Skill_Cool_Time_Fade )
							break;

						pStaticCoolTime = (CKTDGUIStatic*)m_pDLGSkillSlot->GetControl( L"Static_Skill_Cool_Time_B" );
						if( NULL == pStaticCoolTime )
							break;						
					}
					const CX2UserSkillTree::SkillSlotData* pSlotData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB);
					if(pSlotData != NULL)
					{	
						if( NULL != pStaticSlot_A && NULL != pStaticSlot_B )
						{
							if( CX2SkillTree::SI_NONE != pSlotData->m_eID )
							{// 0~3 : B����, 4~8 : A����				
								if( true == bSlotB )
								{
									pStaticSlot_B->GetPicture(iSlotIndex)->SetColor( D3DXCOLOR(1.f,1.f,1.f,1.f));
								}
								else
								{
									pStaticSlot_A->GetPicture(iSlotIndex)->SetColor( D3DXCOLOR(1.f,1.f,1.f,1.f));
								}
							}
						}
						if(pSlotData->m_eID == CX2SkillTree::SI_NONE )
						{
							//������ ��ų�� ���ٸ� Ȱ�����·� ����
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;

							pStaticCoolTime->SetString( iSlotIndex, L"" );

							continue;
						}
						CX2GageData* pMyGageData = GetMyUnit()->GetGageData();

#ifdef SERV_9TH_NEW_CHARACTER
						// ����ȭ MP���� ���� �����Ǿ��ٸ�
						// ���� ������ ����ȭ �����̸�,  ����ȭ ��ġ �̵��� ������ ����
						bool bShowCoolTime = true;
						float fRequiredMpForPhaseShift = 0.f;
						if( pSlotData->m_eID == CX2SkillTree::SI_A_AN_PHASE_SHIFT )
						{
							CX2GUAdd* pAdd = static_cast< CX2GUAdd* >( GetMyUnit() );
							if( NULL != pAdd )
							{
								if( pAdd->GetIsActivePhaseShift() )
								{
									fRequiredMpForPhaseShift = pAdd->GetPhaseShiftMPConsume();
									bShowCoolTime = false;
								}
							}
						}
#endif // SERV_9TH_NEW_CHARACTER

#ifdef ELSWORD_WAY_OF_SWORD
						// ������ // MP �Ҹ𷮿� ���� ����ó���� ����  GetActualMPConsume�� MP �Ҹ� ������ 
						float fRequiredMPConsumption = GetMyUnit()->GetActualMPConsume( pSlotData->m_eID, pSlotData->m_iSkillLevel + pSlotData->m_iSkillLevelPlus );
						if( CX2Unit::UT_ELSWORD == GetMyUnit()->GetUnit()->GetType()
							)
						{
							int iWayOfSwordState = GetMyUnit()->GetWayOfSwordState();

							if( iWayOfSwordState == 2 ) // CX2GUElsword_SwordMan::WSS_VIGOR
							{
								const CX2SkillTree::SkillTemplet* pSkillTempletUsing = GetMyUnit()->GetEquippedActiveSkillTemplet( pSlotData->m_eID );
								if( pSkillTempletUsing != NULL )
								{
				#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
									const int iWayOfSwordType = pSkillTempletUsing->m_iWOSType;
				#else // UPGRADE_SKILL_SYSTEM_2013
									int iWayOfSwordType = (int)pSkillTempletUsing->GetSkillAbilityValue( CX2SkillTree::SA_WAY_OF_SWORD_TYPE );
				#endif // UPGRADE_SKILL_SYSTEM_2013

									if( iWayOfSwordType == 2 )
									{
										fRequiredMPConsumption *= 0.7f;
									}
								}
							}
						}
#endif ELSWORD_WAY_OF_SWORD

#ifdef FINALITY_SKILL_SYSTEM //��â��
						if( pSlotData->m_eID == CX2SkillTree::SI_HA_EEP_LINK_OVERCHARGE_ILLUSION )		/// ��ũ �������� �Ϸ��� npc �丣�𳭵� �ǴϽ� ����
						{
							CX2GUEve* pEve = static_cast< CX2GUEve* >( GetMyUnit() );

							if( true == pEve->GetActiveLinkOverChargeIllusion() )
							{
								fRequiredMPConsumption = 0.f;
							}
						}
#endif //FINALITY_SKILL_SYSTEM

#ifdef EVE_ELECTRA
						if( pSlotData->m_eID == CX2SkillTree::SI_A_EEL_SPECTRO_EL_CRYSTAL )
						{
							CX2GUEve* pEve = static_cast< CX2GUEve* >( GetMyUnit() );
#ifdef ADDITIONAL_MEMO
							if( pEve->GetEqippedSkillMemo( CX2SkillTree::SMI_EVE_MEMO13 ) == true )
							{
								fRequiredMPConsumption -= 10.f;
								if( fRequiredMPConsumption < 0.f )
									fRequiredMPConsumption = 0.f;
							}
#endif							
							if( pEve->GetEnableElectraSystem() == true )
							{
								fRequiredMPConsumption = 0.f;
							}
						}
						else if( pSlotData->m_eID == CX2SkillTree::SI_SA_EEL_THOUSANDS_OF_STARS )
						{
							CX2GUEve* pEve = static_cast< CX2GUEve* >( GetMyUnit() );
							if( pEve->GetThousandStarCount() > 0 &&  pEve->GetThousandStarEntireFired() == false  )
							{
								fRequiredMPConsumption = 0.f;
							}

						}
#endif EVE_ELECTRA

#ifdef SERV_RENA_NIGHT_WATCHER
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
						if( pSlotData->m_eID == CX2SkillTree::SI_A_LNW_DELAYED_FIRING )		/// ������ ��ȣź ��ü ����
#else //UPGRADE_SKILL_SYSTEM_2013
						if( pSlotData->m_eID == CX2SkillTree::SI_A_RNW_START_OF_DELAYED_FIRING )		/// ������ ��ȣź ��ü ����
#endif //UPGRADE_SKILL_SYSTEM_2013
						{
							CX2GULire_ElvenRanger* pRena = static_cast< CX2GULire_ElvenRanger* >( GetMyUnit() );

							if( true == pRena->GetActiveStartOfDelayedFiring() )
							{
								fRequiredMPConsumption = 0.f;
							}
						}
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
						if( pSlotData->m_eID == CX2SkillTree::SI_A_RF_SHADOW_STEP )						/// ������ ���� �޺�
						{
							CX2GURaven* pRaven = static_cast< CX2GURaven* >( GetMyUnit() );

							if( NULL != pRaven && true == pRaven->GetActiveShadowStep() )
							{
								fRequiredMPConsumption = 10.f;
							}
						}
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
						/// �����̽ý� �ʵ� ���� ��
						else if( pSlotData->m_eID == CX2SkillTree::SI_SA_APT_STASIS_FIELD )
						{
							CX2GUAdd* pAdd = static_cast< CX2GUAdd* >( GetMyUnit() );

							if( NULL != pAdd && true == pAdd->GetIsActiveStasisfield() )
							{
								fRequiredMPConsumption = 0.f;
								bShowCoolTime = false;
							}
						}

						/// ���� ��ȭ ���� ��
						else if( 0.f < fRequiredMpForPhaseShift )
						{
							fRequiredMPConsumption = fRequiredMpForPhaseShift;
						}

						/// ���Ϸ� ���� ��ȯ ������ �����̶��, ��Ÿ�� ���� ����
						else if ( CX2Unit::UT_ADD == GetMyUnit()->GetUnit()->GetType() &&
								  pSlotData->m_eID == CX2SkillTree::SI_A_APT_PYLON_DYNAMO &&
								  true == static_cast< CX2GUAdd* >( GetMyUnit() )->GetPossibleNonstopSummonPylon() )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;

							bShowCoolTime = false;

							pStaticCoolTime->SetString( iSlotIndex, L"" );	
						}
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
						/// ȯ�� ������ ���������� ����ÿ�, ��Ȱ�� ó�� ����
						else if ( CX2Unit::UT_ADD == GetMyUnit()->GetUnit()->GetType() &&
								  true == static_cast< CX2GUAdd* >( GetMyUnit() )->GetIsFantasyTracerTargetSkillID( pSlotData->m_eID ) )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;

							bShowCoolTime = false;

							pStaticCoolTime->SetString( iSlotIndex, L"" );	
						}

						/// ���ǹ� �׻� �����߿�, ��� Ȱ�� ó��
						else if ( CX2Unit::UT_ADD == GetMyUnit()->GetUnit()->GetType() &&
								  pSlotData->m_eID == CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL &&
								  CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == static_cast< CX2GUAdd* >( GetMyUnit() )->GetFutureStateSkillID() )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;

							bShowCoolTime = false;

							pStaticCoolTime->SetString( iSlotIndex, L"" );	
						}
#endif //SERV_ADD_LUNATIC_PSYKER

#ifdef FINALITY_SKILL_SYSTEM //JHKang
						const int iElEssence = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );
						const CX2SkillTree::SkillTemplet* pSkillTemplet = refUserSkillTree.GetUserSkillTemplet( pSlotData->m_eID );
#endif //FINALITY_SKILL_SYSTEM

						if( true == GetMyUnit()->GetEntangled() ||
							GetMyUnit()->GetNoSkillTime() > 0.f )	// ��ų�� ����� �� ���� silence ������ ��
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}
						else if(pSlotData->m_fCoolTimeLeft > 0.0f
#ifdef SERV_9TH_NEW_CHARACTER
								&& true == bShowCoolTime 
#endif // SERV_9TH_NEW_CHARACTER
								)
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							wstringstream wstrstm;
							wstrstm << (int)( ceil(pSlotData->m_fCoolTimeLeft) );
							pStaticCoolTime->SetString( iSlotIndex, wstrstm.str().c_str() );
						}
#ifdef ELSWORD_WAY_OF_SWORD
						else if ( NULL != GetMyUnit() && fRequiredMPConsumption > GetMyUnit()->GetNowMp() )
#else
						else if ( NULL != GetMyUnit() && pSlotData->m_fMPConsumption > GetMyUnit()->GetNowMp() )
#endif ELSWORD_WAY_OF_SWORD
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;
												
							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}
				//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
			#ifdef	NEW_CHARACTER_CHUNG
						// ĳ���Ͱ� û�ε�, ����ȭ��尡 �ƴ� ���¿��� ĳ�� �������� ���� �ϸ�
						else if ( CX2Unit::UT_CHUNG == GetMyUnit()->GetUnit()->GetType()
								  && ( false == GetMyUnit()->IsFullHyperMode() 
								        && static_cast<CX2GUChung*>(GetMyUnit())->GetCannonBallCount() < pSlotData->m_usCBConsumption )
                                 )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							// ĳ���� �����ϵ��� ǥ������� ���� ������?
							pStaticCoolTime->SetString( iSlotIndex, L"" );
						} // if
			#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE

		#ifdef ADD_ARA_MEMO	// ���볯¥: 2013-03-28
						/// �ƶ��� ��, ��� �Һ� �˻�
						else if ( CX2Unit::UT_ARA == GetMyUnit()->GetUnit()->GetType() &&
							static_cast< CX2GUAra* >( GetMyUnit() )->GetForcePower() < 
							static_cast< CX2GUAra* >( GetMyUnit() )->GetActualFPConsume( pSlotData->m_eID, pSlotData->m_usFPConsumption ) )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							pStaticCoolTime->SetString( iSlotIndex, L"" );	
						} 
		#else // ADD_ARA_MEMO
						else if ( CX2Unit::UT_ARA == GetMyUnit()->GetUnit()->GetType() && 
							static_cast<CX2GUAra*>(GetMyUnit())->GetForcePower() < pSlotData->m_usFPConsumption )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}
		#endif // ADD_ARA_MEMO
#endif // ARA_CHARACTER_BASE
#ifdef FINALITY_SKILL_SYSTEM //JHKang
						else if ( pSkillTemplet != NULL && pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL &&
							iElEssence <= 0 
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
							&& false == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence()
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
							)
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}
#endif //FINALITY_SKILL_SYSTEM
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
						/// DP �˻�
						else if ( CX2Unit::UT_ADD == GetMyUnit()->GetUnit()->GetType() &&
								  static_cast< CX2GUAdd* >( GetMyUnit() )->GetDPValue() < 
								  static_cast< CX2GUAdd* >( GetMyUnit() )->GetActualDPConsume( pSlotData )  )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							pStaticCoolTime->SetString( iSlotIndex, L"" );	
						}
						/// ���� ����Ʈ �˻�
						else if ( CX2Unit::UT_ADD == GetMyUnit()->GetUnit()->GetType() &&
								  static_cast< CX2GUAdd* >( GetMyUnit() )->GetMutationCount() < 
								  static_cast< CX2GUAdd* >( GetMyUnit() )->GetMutationPointConsume( pSlotData ) )
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							pStaticCoolTime->SetString( iSlotIndex, L"" );	
						}
#endif //SERV_9TH_NEW_CHARACTER
						else
						{
							if ( pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
							{
								if(true == pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->GetShow() && m_bSkillSlotJustChanged == false	)
								{
									bool bIsShowEffect = true;

						#ifdef DIALOG_SHOW_TOGGLE	
									if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
										bIsShowEffect = false;
						#endif
									

						#ifdef SKILL_SLOT_UI_TYPE_B
									if( false == bIsSlotUITypeA ) //UI Ÿ�� B�϶��� �ش� ������ ���õǾ��� ���� ���� ����Ʈ ����
									{
										if( true == bIsSkillSlotSwap && false == bSlotB ) //���� B������ ���� �Ǿ� �ְ�, B���Կ� ���� üũ�� �ƴ϶��
										{
											bIsShowEffect = false;
										}
										if( false == bIsSkillSlotSwap && true == bSlotB ) //���� A������ ���� �Ǿ� �ְ�, A���Կ� ���� üũ�� �ƴ϶��
										{
											bIsShowEffect = false;
										}
									}
						#endif //SKILL_SLOT_UI_TYPE_B

									if( true == bIsShowEffect )
									{
										if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() &&
											NULL != g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot() &&
											true == g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->GetShow() )
										{
											// ��ų�� �� �� �ְ� �Ǵ� ����. ����Ʈ�� ǥ���� �ش�.
											D3DXVECTOR2 EffectPos = pStatic_Skill_Cool_Time_Fade->GetDialog()->GetPos() + pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->GetPos() + pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->GetSize()/2;

						#ifdef SKILL_SLOT_UI_TYPE_B
											if( false == bIsSlotUITypeA && true == bSlotB )
												EffectPos.y+=51.f;
						#endif //SKILL_SLOT_UI_TYPE_B

											CKTDGParticleSystem::CParticleEventSequence* pParticle = 
												g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"OutBox_Skill_Recharge01", EffectPos.x, EffectPos.y, 0 );
											if( pParticle != NULL )
												pParticle->SetOverUI( true );

											CKTDGParticleSystem::CParticleEventSequence* pParticle2 = 
												g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"OutBox_Skill_Recharge02", EffectPos.x, EffectPos.y, 0 );
											if( pParticle2 != NULL )
											{
												pParticle2->SetOverUI( true );
												pParticle2->SetLatency( 0.1f );
											}	
										}									
									}
									g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Refresh.ogg", false, false );
									
								}
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;
							}

							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}
					}			
				}
			}
		}
	}
	m_bSkillSlotJustChanged = false;
	

}
void CX2Game::InitExpandSkillCoolTime()
{
	if ( m_pDLGSkillSlot != NULL )
	{
		CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*)m_pDLGSkillSlot->GetControl( L"Static_Skill_Cool_Time" );
		CKTDGUIStatic* pStaticCoolTime_B = (CKTDGUIStatic*)m_pDLGSkillSlot->GetControl( L"Static_Skill_Cool_Time_B" );

		for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; i++ )
		{
			if( NULL != pStaticCoolTime )
			{
				pStaticCoolTime->SetString( i , L"");
			}
			if( NULL != pStaticCoolTime_B )
			{
				pStaticCoolTime->SetString( i , L"");
			}
		}
	}
}
void CX2Game::ResurrectionFrameMove( double fTime, float fElapsedTime )
{
	CKTDGUIStatic* pStaticRestore_Continue_End = static_cast<CKTDGUIStatic*>( m_pDLGContinue->GetControl( L"Restore_Continue_End" ) );
	if ( pStaticRestore_Continue_End != NULL )
		pStaticRestore_Continue_End->SetShow( false );

	// �ڽ��� �׾��� ��Ȱ ī��Ʈ�� ���۵��� �ʾ�����
	if ( m_bResurrectionCountStart == false && GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE
		)
	{
		//��Ʈ�� ����
		if( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
		{
			//��Ÿ ����(Z) �������� �ؽ�Ʈ ����  --��Ȱ�Ϸ��� #CFFFF00'@1'#CXŰ�� �����ּ���.
			pStaticRestore_Continue_End->GetString(0)->msg = GET_REPLACED_STRING( (STR_ID_17325, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_ATTACK_FAST ) ) );

			//��Ȱ�� ���� ���ο� ���� �ؽ�Ʈ ����
			if ( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() > 0 
#ifdef AUTO_PAYMENT	// ���� ������ �ؿ��� �߰�
				|| GetMyUnit()->GetUnit()->GetAutoResStoneNum() > 0
#endif	//	AUTO_PAYMENT
				)				
				pStaticRestore_Continue_End->GetString(1)->msg = GET_STRING(STR_ID_17326); //-- (��Ȱ�� 1���� �Ҹ�˴ϴ�)
			else
#if defined CLIENT_COUNTRY_TW
				pStaticRestore_Continue_End->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_17327, "i", 5 ) ); //-- (150ĳ�ð� �Ҹ�˴ϴ�)
#elif defined CLIENT_COUNTRY_HK
				pStaticRestore_Continue_End->GetString(1)->msg = GET_REPLACED_STRING( ( STR_ID_17327, "i", 1 ) ); //-- (150ĳ�ð� �Ҹ�˴ϴ�)
#else
				pStaticRestore_Continue_End->GetString(1)->msg = GET_STRING(STR_ID_17327); //-- (150ĳ�ð� �Ҹ�˴ϴ�)
#endif

			if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() )
				pStaticRestore_Continue_End->GetString(2)->msg = GET_REPLACED_STRING( (STR_ID_17329, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_ATTACK_STRONG ) )); //--#CFFFF00'@1'Ű�� �������� �����
		}
		//== ī��Ʈ ����

		if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME  ) //�ʵ忡���� ī���� ���� �ʵ��� ���� 
		{
			NoticeContinue();	// m_fResurrectionRemainTime�� 10���� ���� �ϰ� SetResurrectionTimeOut�� false�� �����.
		}
		else if ( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD  ) 
		{
			m_fAutoResurrectionTime = 300.f;
		}
		m_bResurrectionCountStart = true;	// ����
	}
	if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree()
		&& NULL != g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot() 
#ifdef RIDING_SYSTEM
		&& NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetSkillSlot()
#endif //RIDING_SYSTEM
		)
	{
		if( GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
		{
#ifdef RIDING_SYSTEM
			CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot(false);
#endif //RIDING_SYSTEM
			g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetEnable(false);
			g_pData->GetUIManager()->GetUISkillTree()->ShowSkillDesc(false);
		}
		else
		{
#ifdef RIDING_SYSTEM
			if( true == g_pData->GetUIManager()->GetShowRidingPetSkillslot() )
			{
				CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot(true);
				g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetEnable(false);
			}
			else
			{
				CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot(false);
				g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetEnable(true);
			}
#else
			g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetEnable(true);
#endif //RIDING_SYSTEM
		}
	}

	// ī��Ʈ �ð��� �����ֵ��� ī��Ʈ ó��.
	if ( m_fResurrectionRemainTime > 0.0f )
	{
		if ( (int)m_fResurrectionRemainTime > (int)(m_fResurrectionRemainTime - fElapsedTime) )
		{
			if ( NULL != m_pSoundContinue )
				m_pSoundContinue->Play( false, false );
		}
		
		// ��Ȱ ī��Ʈ�� ��ž���� �ʾ����� ��Ȱ�ϴµ� ���� �ð��� 
		if ( false == m_bStopRebirthCount )
		{
			m_fResurrectionRemainTime -= fElapsedTime;
			
			if ( m_fResurrectionRemainTime <= 0.0f )
				m_fResurrectionRemainTime = 0.0f;
		}
	}
	
	//FieldFix: �̰� ��� ���� �ǰڱ�...
	m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ -= fElapsedTime;
	if ( m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ <= 0.0f )
	{
		m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ = 0.0f;
		m_bEnableSendKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ = true;
	}

	if ( m_pDLGCheckResurrectByUseCash != NULL )
	{
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGCheckResurrectByUseCash ) == true )
		{
			swprintf( m_StopContinueAlarmText, 256, 
#if defined CLIENT_COUNTRY_TW
				GET_REPLACED_STRING( ( STR_ID_139, "ii", 5, (int)m_fTimeWaitStopContinue ) ),
#elif defined CLIENT_COUNTRY_HK
				GET_REPLACED_STRING( ( STR_ID_139, "ii", 1, (int)m_fTimeWaitStopContinue ) ),
#else
				GET_REPLACED_STRING( ( STR_ID_139, "i", static_cast<int>( m_fTimeWaitStopContinue ) ) ), 
#endif
				static_cast<int>( m_fTimeWaitStopContinue ) );
				
			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDLGCheckResurrectByUseCash->GetControl( L"StaticMsgBoxBG" ) );
			if( pStatic != NULL )
				pStatic->GetString(0)->msg = m_StopContinueAlarmText;
		}	

		//FieldFix: �̰��� �ǵ���...?
		m_fTimeWaitStopContinue -= fElapsedTime;

		if ( m_fTimeWaitStopContinue <= 0.0f )
		{
			m_fTimeWaitStopContinue = 0.0f;

			SAFE_DELETE_DIALOG( m_pDLGCheckResurrectByUseCash );
			Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( false );
		}
	}


	if ( GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE &&
		m_fResurrectionRemainTime <= 0.0f && GetMyUnit()->GetResurrectionTimeOut() == false )
	{
		GetMyUnit()->SetResurrectionTimeOut(true);
	}

	ResurrectionKeyProcess();	

	CKTDGUIStatic* pStaticRestore_Continue = static_cast<CKTDGUIStatic*>( m_pDLGContinue->GetControl( L"Restore_Continue" ) );
	if ( pStaticRestore_Continue != NULL )
		pStaticRestore_Continue->SetShow( false );

	CKTDGUIStatic* pStaticRestore_Continue_For_Cash = static_cast<CKTDGUIStatic*>( m_pDLGContinue->GetControl( L"Restore_Continue_Cash" ) );
	if ( pStaticRestore_Continue_For_Cash != NULL )
		pStaticRestore_Continue_For_Cash->SetShow( false );

	CKTDGUIStatic* pStaticCount = static_cast<CKTDGUIStatic*>( m_pDLGContinue->GetControl( L"Restore_Continue_NUM" ) );
	if ( pStaticCount != NULL )
		pStaticCount->SetShow( false );

	if ( m_fResurrectionRemainTime > 0.0f && GetMyUnit() != NULL )
	{
		WCHAR timeBuf[200] = {0,};
		int coundDown = static_cast<int>( m_fResurrectionRemainTime ) - 1;
		if ( coundDown < 0 )
			coundDown = 0;
		else if( coundDown > 8) //�ִ� 8�ʷ� ����
			coundDown = 8;
		m_pDLGContinue->SetShowEnable( true, true);

		if ( pStaticCount != NULL )
		{
			pStaticCount->SetShow( true );

			for ( int i = 0; i < 9; i++ )
			{
				if ( pStaticCount->GetPicture( i ) != NULL )
				{
					if ( i == coundDown )
						pStaticCount->GetPicture( i )->SetShow( true );
					else
						pStaticCount->GetPicture( i )->SetShow( false );	
				}
			}
		}

		//{{ ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� üũ �߰�
#ifdef AUTO_PAYMENT
//��Ȱ���� ���� ��
		if ( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() > 0 || GetMyUnit()->GetUnit()->GetAutoResStoneNum() > 0 )
#else
		if ( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() > 0 )
#endif	//	AUTO_PAYMENT
		//}} ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� üũ �߰�
		{
			if ( pStaticRestore_Continue != NULL )
			{
				pStaticRestore_Continue->SetShow( true );
				if ( pStaticRestore_Continue->GetString(0) != NULL )
				{
					wstringstream wstrstm;
					
					wstrstm << L"x" << GetMyUnit()->GetUnit()->GetResurrectionStoneNum();

					//{{ ����� : [2009/10/15] //	�ڵ����� ��Ȱ�� ó��
#ifdef AUTO_PAYMENT
					if( GetMyUnit()->GetUnit()->GetAutoResStoneNum() > 0 )
					{
						wstrstm << L"(+" << GetMyUnit()->GetUnit()->GetAutoResStoneNum() << L")";
					}
#endif	//	AUTO_PAYMENT
					//}} ����� : [2009/10/15] //	�ڵ����� ��Ȱ�� ó���ڵ� �߰�

					pStaticRestore_Continue->GetString(0)->msg = wstrstm.str().c_str();
				}
			}
		}
		else
		{
//��Ȱ���� ���� ��
			if ( pStaticRestore_Continue_For_Cash != NULL )
			{
				pStaticRestore_Continue_For_Cash->SetShow( true );
				wstringstream wstrstm;
				if ( pStaticRestore_Continue_For_Cash->GetString(0) != NULL )
				{
					
					wstrstm << L"x" << GetMyUnit()->GetUnit()->GetResurrectionStoneNum();
					//{{ ����� : [2009/10/15] //	�ڵ����� ��Ȱ�� ó��
#ifdef AUTO_PAYMENT
					if( GetMyUnit()->GetUnit()->GetAutoResStoneNum() > 0 )
					{
						wstrstm << L"(+" << GetMyUnit()->GetUnit()->GetAutoResStoneNum() << L")";
					}
#endif	//	AUTO_PAYMENT
					//}} ����� : [2009/10/15] //	�ڵ����� ��Ȱ�� ó���ڵ� �߰�
					pStaticRestore_Continue_For_Cash->GetString(0)->msg = wstrstm.str().c_str();
				}

				if ( pStaticRestore_Continue_For_Cash->GetString(1) != NULL )
				{
					wstrstm.str( L"" );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
					wstrstm << g_pData->GetMyUser()->GetGlobalCashForRescurrection();
#else //SERV_SUPPORT_SEVERAL_CASH_TYPES
					wstrstm << g_pData->GetMyUser()->GetCash();
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
					pStaticRestore_Continue_For_Cash->GetString(1)->msg = wstrstm.str().c_str();
				}
			}
		}
		
	}//ī��Ʈ ���ӽð��� ����Ǿ��� ��
	else if ( GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE && 
		m_bGameEnd == false )
	{
		m_pDLGContinue->SetShow( true );
		m_pDLGContinue->SetEnable( true );
		if(  NULL != pStaticRestore_Continue_End )
		{
			if(CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
			{
				if( 0.f != m_fAutoResurrectionTime )
				{
					m_fAutoResurrectionTime -= fElapsedTime;
					int iMin = (int)m_fAutoResurrectionTime / 60;
					int iSecond = (int)m_fAutoResurrectionTime % 60;
					static int iOldSecond;

					if( 0.f >= m_fAutoResurrectionTime )
					{
						g_pMain->GetPartyUI()->CancelToWaitForDungeonPartyMatching();
						//Handler_EGS_SKIP_CONTINUE_DUNGEON_NOT();
						if ( NULL != g_pData->GetBattleFieldRoom() )
						{
							UINT uiBattleFieldId = g_pData->GetBattleFieldRoom()->GetBattleFieldId();
							UINT uiVillageId = g_pData->GetBattleFieldManager().GetReturnVillageId( uiBattleFieldId );
							CX2State* pState = static_cast<CX2State*>( g_pMain->GetNowState() );
							if ( NULL != pState )
								pState->Handler_EGS_STATE_CHANGE_FIELD_REQ( uiVillageId );
						}
					}
					else
					{
						if( iOldSecond != iSecond )
						{
							iOldSecond = iSecond;
							pStaticRestore_Continue_End->GetString(2)->msg = GET_REPLACED_STRING( (STR_ID_17328, "iiL", (int)iMin ,(int)iSecond, std::wstring( L"X") )); // -- #CFFFF00@1:@2�� �Ǵ� @3Ű�� �������� �����
						}
					}
				}

			}
			else
			{
				pStaticRestore_Continue_End->GetString(2)->msg = L"";
			}

			pStaticRestore_Continue_End->SetShow(true);
		}
	}
//========================================================================
	ResurrectOtherUser();
}

void CX2Game::ResurrectOtherUserOK()
{
	Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( m_UnitUIDToResurrect );
	if ( m_pDLGResurrect != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResurrect, NULL, false );

	m_pDLGResurrect = NULL;
}

void CX2Game::ResurrectOtherUserCancel()
{
	if ( m_pDLGResurrect != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGResurrect, NULL, false );

	m_pDLGResurrect = NULL;
}

bool CX2Game::Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( bool bStop )
{
	if ( bStop == true )
	{
		if ( m_bEnableSendKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ == false )
			return true;

		if ( m_pDLGCheckResurrectByUseCash != NULL )
			return true;
	}

	KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ kPacket;
	kPacket.m_bIsStop = bStop;

	g_pData->GetServerProtocol()->SendPacket( EGS_STOP_DUNGEON_CONTINUE_TIME_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_STOP_DUNGEON_CONTINUE_TIME_ACK );

	return true;
}

bool CX2Game::Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK& kEGS_STOP_DUNGEON_CONTINUE_TIME_ACK )
{
	if ( kEGS_STOP_DUNGEON_CONTINUE_TIME_ACK.m_bIsStop == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGCheckResurrectByUseCash );
		m_pDLGCheckResurrectByUseCash = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
#if defined CLIENT_COUNTRY_TW
			GET_REPLACED_STRING( ( STR_ID_139, "SS", L"5", L"9" ) ),		// �븸 ��Ȱ�� ����, ���� �ð�
#elif defined CLIENT_COUNTRY_HK
			GET_REPLACED_STRING( ( STR_ID_139, "SS", L"1", L"9" ) ),		// ȫ�� ��Ȱ�� ����, ���� �ð�
#else
			GET_REPLACED_STRING( ( STR_ID_139, "S", L"9" ) ), 
#endif
			CX2State::SUCM_RESURRECT_USE_CASH_OK, g_pMain->GetNowState(), 
			CX2State::SUCM_RESURRECT_USE_CASH_CANCEL );

		m_bStopRebirthCount = true;
		m_fTimeWaitStopContinue = 9.0f;
	}
	else
	{
		// ĳ�� ��Ȱ�� ��� â�� ����
		SAFE_DELETE_DIALOG( m_pDLGCheckResurrectByUseCash );
		m_bStopRebirthCount = false;
		m_bEnableSendKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ = false;
		m_fTimeEnableKEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ = 2.0f;

	}

	return true;
}

bool CX2Game::Handler_EGS_SKIP_CONTINUE_DUNGEON_NOT()
{
	g_pData->GetServerProtocol()->SendID( EGS_SKIP_CONTINUE_DUNGEON_NOT );
	return true;
}

bool CX2Game::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( UidType unitUID, bool bAllowUseCash /* = false */ )
{
	if ( NULL == GetMyUnit() )
		return false;

	// ĳ�� ��Ȱ�� ����� ������
	if ( bAllowUseCash == true )
	{
		unitUID = m_ReservedPacket.m_iUnitUID;

		Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( false );
	}
	else
	{
		m_ReservedPacket.m_iUnitUID = unitUID;

		// ��Ȱ�� ����� ���� �޴µ� ��Ȱ���� ���� ���
		//{{ ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� üũ �߰�
#ifdef AUTO_PAYMENT
		if( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() <= 0 && GetMyUnit()->GetUnit()->GetAutoResStoneNum() <= 0 )
#else
		if( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() <= 0 )
#endif	//	AUTO_PAYMENT
			//}} ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� üũ �߰�
		{
			Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( true );

			return true;
		}
	}

	m_bIsSendResurrectionReq = true;

	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ kPacket;
	kPacket.m_iUnitUID = unitUID;

	//{{ 2008. 6. 10  ������  [����] �ǽð� ��Ȱ���� ����Ϸ��� �Ʒ� bool���� true�� �ָ��

	//{{ ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� üũ �߰�
#ifdef AUTO_PAYMENT
	if ( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() > 0 || GetMyUnit()->GetUnit()->GetAutoResStoneNum() > 0 )
#else
	if ( GetMyUnit()->GetUnit()->GetResurrectionStoneNum() > 0 )
#endif	AUTO_PAYMENT
		//}} ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� üũ �߰�
	{
		kPacket.m_bUseCash = false;
	}
	else
	{
		kPacket.m_bUseCash = true;
	}

	m_bStopRebirthCount = false;

	//SAFE_DELETE_DIALOG( m_pDLGCheckResurrectByUseCash );
	if ( m_pDLGCheckResurrectByUseCash != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCheckResurrectByUseCash, NULL, false );

	m_pDLGCheckResurrectByUseCash = NULL;

	g_pData->GetServerProtocol()->SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );


	return true;
}

bool CX2Game::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK.m_iOK ) == true )
		{
			if ( GetMyUnit() != NULL && GetMyUnit()->GetUnit() != NULL )
			{
				GetMyUnit()->GetUnit()->SetResurrectionStoneNum( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK.m_iNumResurrectionStone );
				//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
				GetMyUnit()->GetUnit()->SetAutoResStoneNum( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK.m_iNumAutoPaymentResStone );
#endif	//	AUTO_PAYMENT
				//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
				ResetReBirthStoneNumUI();

				if ( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK.m_bUseCash == true )
				{
					((CX2State*)g_pMain->GetNowState())->Handler_EGS_CHECK_BALANCE_REQ();
				}
			}
		}
		m_bIsSendResurrectionReq = false;
	}

	return true;
}

/*virtual*/ bool CX2Game::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT )
{
	RebirthUserUnit( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT.m_iUnitUID, kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT.m_iStartPosIndex );

	CX2GUUser* pGUUser = GetUserUnitByUID( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT.m_iUnitUID );

	if ( pGUUser != NULL && 
		pGUUser->GetUnit() != NULL )
	{
		if ( GetGameType() == GT_DUNGEON && GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
			m_optrFocusUnit = pGUUser;

		pGUUser->SetDamageCount( 0 );

		//{{ JHKang / ������ / 2011/02/21 / ��Ȱ�� ������ ���� ������ ���� �� �ʱ�ȭ
#ifdef DUNGEON_RANK_NEW
		pGUUser->SetTotalAttackedDamage( 0 );
#endif DUNGEON_RANK_NEW
		//}} JHKang / ������ / 2011/02/21 / ��Ȱ�� ������ ���� ������ ���� �� �ʱ�ȭ

		wstring wstrText;

		if( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT.m_iUnitUID == kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT.m_iUsedByUnitUID )
		{
			wstrText = GET_REPLACED_STRING( ( STR_ID_136, "S", pGUUser->GetUnit()->GetNickName() ) );
		}
		else
		{
			CX2GUUser* pGUUser_Healer = GetUserUnitByUID( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT.m_iUsedByUnitUID );

			if( NULL != pGUUser_Healer && NULL != pGUUser_Healer->GetUnit() )
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_137, "SS", pGUUser_Healer->GetUnit()->GetNickName(), pGUUser->GetUnit()->GetNickName() ) );
			}
			else
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_138, "S", pGUUser->GetUnit()->GetNickName() ) );
			}
		}

		if( NULL != g_pChatBox )
		{
			wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
			D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77

			g_pChatBox->AddChatLog( wstrText.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
		}
	}

	if ( GetMyUnit() != NULL && GetMyUnit() == pGUUser )
	{
		m_bResurrectionCountStart = false;
		m_fResurrectionRemainTime = 0.0f;
		GetMyUnit()->SetResurrectionTimeOut( false );
		m_bStopRebirthCount = false;

#ifdef EVENT_NEW_CHARACTER_EL
		if( true == CX2EmblemManager::GetInstance()->HasEmblem(CX2EmblemManager::EI_EVENT_ELESIS) )
		{		
			GetMyUnit()->SetBuffFactorToGameUnitByBuffFactorID( BFI_EMPTY_EXP_BUFF );
		}
#endif // EVENT_NEW_CHARACTER_EL
#ifdef EVENT_NEW_HENIR
// 		if( true == CX2EmblemManager::GetInstance()->IsBurningEventTime() && 
// 			true == CX2EmblemManager::GetInstance()->HasEmblem(CX2EmblemManager::EI_WEEKEND_BURNING_EVENT) )
// 		{		
// 			GetMyUnit()->SetBuffFactorToGameUnitByBuffFactorID( BFI_EMPTY_EXP_BUFF );
// 		}
#endif // EVENT_NEW_HENIR


	}

	m_bLastKillCheck				= false;
	m_fLastKillWaitTime				= 0.05f;	//FieldFix: �� �ڵ�� �ʿ� ����
	m_fLastKillWaitTimeAfterRebirth	= 1.f;

	//���� ����Ʈ����
	if( IsHost() == true )
	{	
		for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
		{
			CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
			if( pCX2GUNPC != NULL )
			{
				if( pCX2GUNPC->GetIsFallDownMonster() == true )
				{
					if( true == pCX2GUNPC->GetIsFallDownState() )
					{
						pCX2GUNPC->StateChange( pCX2GUNPC->GetDownDamageLandFront(), true );
					}
				}
				//{{ oasis907 : ����� [2011.5.23] ���� ��Ȱ�ÿ� SPRIGGAN_LIGHT_BALL ����
				if( CX2UnitManager::NUI_SPRIGGAN_LIGHT_BALL == pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID )
				{
					pCX2GUNPC->ResetGameUnitWhoAttackedMe();
					pCX2GUNPC->SetNowHp( 0.f );
				}
				//}} oasis907 : ����� [2011.5.23]
			}			
		}
	}


	return true;
}

//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
/** @function : GetLockOnNearstTarget
	@brief : �ڽ��� ������ �������� �յ� ���о��� ���� ����� ������ Ÿ������ ����
	@param : CX2Room::TEAM_NUM eTeamNum_(�ڽ��� ��Number), OUT bool& bUserUnit_ (��������, NPC������ ����)
	@return : UidType (ã����UID, ��ã������ -1)
*/
UidType CX2Game::GetLockOnNearstTarget( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT bool& bUserUnit_ )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	const CX2GameUnit* pNearstUnit = NULL;
	float fNearstDistance = 30000.0f;	// 30000���̶�� ���� �ִ� ��ġ

	for( UINT uIndex = 0; uIndex < m_UnitList.size(); ++uIndex )
	{
		const CX2GameUnit* pCX2GameUnit = m_UnitList[uIndex];
		
		if( pCX2GameUnit == NULL )
			continue;

		if( pCX2GameUnit->GetTeam() == eTeamNum_ )
			continue;

		if( true == pCX2GameUnit->GetInvincible() )
			continue;

		if( 0.f >= pCX2GameUnit->GetNowHp() )
			continue;

		if( CX2GameUnit::GUT_NPC == pCX2GameUnit->GetGameUnitType() )
		{
#ifdef FIELD_BOSS_RAID // ���� / Ÿ�� ������ ���� ����
			if( true == pCX2GameUnit->GetIsBosRaidNPC() )
				continue;
#endif // FIELD_BOSS_RAID

			const CX2GUNPC* pNPC = static_cast<const CX2GUNPC*>( pCX2GameUnit );
			switch( pNPC->GetNPCTemplet().m_ClassType )
			{
			case CX2UnitManager::NCT_THING_TRAP:
			case CX2UnitManager::NCT_THING_CHECKER:
				{
					continue;
				} break;
			} // switch
		} // if

		float fDistance = GetDistance( vMyPos_, pCX2GameUnit->GetPos() );

		// �� ����� �ָ� ã����
		if ( fDistance < fNearstDistance )
		{
			pNearstUnit		= pCX2GameUnit;
			fNearstDistance = fDistance;
		} // if
	} // for

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	// Ÿ���� ������
	if( NULL == pNearstUnit )
	{
		return -1;
	} // if
	// Ÿ���� ������
	else
	{
		if( pNearstUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			bUserUnit_ = true;
			const CX2GUUser* pCX2GUUser = static_cast<const CX2GUUser*>( pNearstUnit );
			return pCX2GUUser->GetUnitUID();
		} // if
		else
		{
			bUserUnit_ = false;
			const CX2GUNPC* pCX2GUNPC = static_cast<const CX2GUNPC*>( pNearstUnit );
			return pCX2GUNPC->GetUnitUID();
		} // else
	} // else
}

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG

#ifdef CHUNG_SECOND_CLASS_CHANGE
UidType CX2Game::GetLockOnNearstTargetInRange( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, IN D3DXVECTOR2& vRange_, OUT bool& bUserUnit_ 
	#ifdef BALANCE_DEADLY_CHASER_20130214
												, bool bIsNotTargetPreLockOnTarget/* = false*/
	#endif //BALANCE_DEADLY_CHASER_20130214
											)
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	const CX2GameUnit* pNearstUnit = NULL;
	float fNearstDistance = vRange_.y;

	for( UINT uIndex = 0; uIndex < m_UnitList.size(); ++uIndex )
	{
		const CX2GameUnit* pCX2GameUnit = m_UnitList[uIndex];

		if( pCX2GameUnit == NULL )
			continue;

		if( pCX2GameUnit->GetTeam() == eTeamNum_ )
			continue;

		if( true == pCX2GameUnit->GetInvincible() )
			continue;

		if( 0.f >= pCX2GameUnit->GetNowHp() )
			continue;

		if( CX2GameUnit::GUT_NPC == pCX2GameUnit->GetGameUnitType() )
		{
#ifdef FIELD_BOSS_RAID // ���� / Ÿ�� ������ ���� ����
			if( true == pCX2GameUnit->GetIsBosRaidNPC() )
				continue;
#endif // FIELD_BOSS_RAID

			const CX2GUNPC* pNPC = static_cast<const CX2GUNPC*>( pCX2GameUnit );
			switch( pNPC->GetNPCTemplet().m_ClassType )
			{
			case CX2UnitManager::NCT_THING_TRAP:
			case CX2UnitManager::NCT_THING_CHECKER:
				{
					continue;
				} break;
			} // switch
		} // if

#ifdef BALANCE_DEADLY_CHASER_20130214
		if( true == bIsNotTargetPreLockOnTarget &&
			false == m_setPreLockOnTargetList.empty() &&
			m_setPreLockOnTargetList.end() != m_setPreLockOnTargetList.find( pCX2GameUnit->GetUnitUID() ) )
			continue;
#endif //BALANCE_DEADLY_CHASER_20130214

		float fDistance = GetDistance( vMyPos_, pCX2GameUnit->GetPos() );

		// �� ����� �ָ� ã����
		if ( fDistance < fNearstDistance && fDistance > vRange_.x )
		{
			pNearstUnit		= pCX2GameUnit;
			fNearstDistance = fDistance;
		} // if
	} // for
	
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	// Ÿ���� ������
	if( NULL == pNearstUnit )
	{
#ifdef BALANCE_DEADLY_CHASER_20130214
		if( true == bIsNotTargetPreLockOnTarget )
			m_setPreLockOnTargetList.clear();
#endif //BALANCE_DEADLY_CHASER_20130214
		return -1;
	} // if
	// Ÿ���� ������
	else
	{		
#ifdef BALANCE_DEADLY_CHASER_20130214
		if( true == bIsNotTargetPreLockOnTarget )
			m_setPreLockOnTargetList.insert( pNearstUnit->GetUnitUID() );
#endif //BALANCE_DEADLY_CHASER_20130214
		if( pNearstUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			bUserUnit_ = true;
			const CX2GUUser* pCX2GUUser = static_cast<const CX2GUUser*>( pNearstUnit );
			return pCX2GUUser->GetUnitUID();
		} // if
		else
		{
			bUserUnit_ = false;
			const CX2GUNPC* pCX2GUNPC = static_cast<const CX2GUNPC*>( pNearstUnit );
			return pCX2GUNPC->GetUnitUID();
		} // else
	} // else	
}
#endif


#ifdef FIX_PET_ATTACK
UidType CX2Game::GetLockOnNearstTargetOnlyBasic( CX2Room::TEAM_NUM eTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT bool& bUserUnit_ )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	const CX2GameUnit* pNearstUnit = NULL;
	float fNearstDistance = 30000.0f;	// 30000���̶�� ���� �ִ� ��ġ

	for( UINT uIndex = 0; uIndex < m_UnitList.size(); ++uIndex )
	{
		const CX2GameUnit* pCX2GameUnit = m_UnitList[uIndex];

		if( pCX2GameUnit == NULL )
			continue;

		if( pCX2GameUnit->GetTeam() == eTeamNum_ )
			continue;

		if( true == pCX2GameUnit->GetInvincible() )
			continue;

		if( 0.f >= pCX2GameUnit->GetNowHp() )
			continue;

		if( CX2GameUnit::GUT_NPC == pCX2GameUnit->GetGameUnitType() )
		{
#ifdef FIELD_BOSS_RAID // ���� / Ÿ�� ������ ���� ����
			if( true == pCX2GameUnit->GetIsBosRaidNPC() )
				continue;
#endif // FIELD_BOSS_RAID

			const CX2GUNPC* pNPC = static_cast<const CX2GUNPC*>( pCX2GameUnit );
			if( pNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
				continue;
		} // if

		float fDistance = GetDistance( vMyPos_, pCX2GameUnit->GetPos() );

		// �� ����� �ָ� ã����
		if ( fDistance < fNearstDistance )
		{
			pNearstUnit		= pCX2GameUnit;
			fNearstDistance = fDistance;
		} // if
	} // for

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	// Ÿ���� ������
	if( NULL == pNearstUnit )
	{
		return -1;
	} // if
	// Ÿ���� ������
	else
	{
		if( pNearstUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			bUserUnit_ = true;
			const CX2GUUser* pCX2GUUser = static_cast<const CX2GUUser*>( pNearstUnit );
			return pCX2GUUser->GetUnitUID();
		} // if
		else
		{
			bUserUnit_ = false;
			const CX2GUNPC* pCX2GUNPC = static_cast<const CX2GUNPC*>( pNearstUnit );
			return pCX2GUNPC->GetUnitUID();
		} // else
	} // else
}
#endif

UidType	CX2Game::GetLockOnTarget( CX2Room::TEAM_NUM teamNum, int seedNum, bool& bUserUnit, int iModulusFactor /*= 30*/ )
{
	int keyCount = (seedNum % iModulusFactor) + 1;

	bool bTargetIn = false;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if( pCX2GameUnit == NULL )
			continue;

		if( teamNum == pCX2GameUnit->GetTeam() )
			continue;

		if( pCX2GameUnit->GetInvincible() == true )
			continue;

		if( pCX2GameUnit->GetNowHp() <= 0.f )
			continue;

		if( pCX2GameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
		{
#ifdef FIELD_BOSS_RAID // ���� / Ÿ�� ������ ���� ����
			if( true == pCX2GameUnit->GetIsBosRaidNPC() )
				continue;
#endif // FIELD_BOSS_RAID

			CX2GUNPC* pNPC = (CX2GUNPC*) pCX2GameUnit;
			switch( pNPC->GetNPCTemplet().m_ClassType )
			{
			case CX2UnitManager::NCT_THING_TRAP:
			case CX2UnitManager::NCT_THING_CHECKER:
				{
					continue;
				} break;
			}
		}

		bTargetIn = true;
		break;
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	if( bTargetIn == false )
	{
		return -1;
	}

	CX2GameUnit* pRetUnit = NULL;

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	while( keyCount > 0 )
	{
		for( int i = 0; i < (int)m_UnitList.size(); i++ )
		{
			CX2GameUnit* pCX2GameUnit = m_UnitList[i];
			if( pCX2GameUnit == NULL )
				continue;

			if( teamNum == pCX2GameUnit->GetTeam() )
				continue;

			if( pCX2GameUnit->GetInvincible() == true )
				continue;

			if( pCX2GameUnit->GetNowHp() <= 0.f )
				continue;

			if( pCX2GameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{

#ifdef FIELD_BOSS_RAID // ���� / Ÿ�� ������ ���� ����
				if( true == pCX2GameUnit->GetIsBosRaidNPC() )
					continue;
#endif // FIELD_BOSS_RAID

				CX2GUNPC* pNPC = (CX2GUNPC*) pCX2GameUnit;
				switch( pNPC->GetNPCTemplet().m_ClassType )
				{
				case CX2UnitManager::NCT_THING_TRAP:
				case CX2UnitManager::NCT_THING_CHECKER:
					{
						continue;
					} break;
				}

				keyCount--;
				pRetUnit = pCX2GameUnit;
			}
			else
			{
				keyCount--;
				pRetUnit = pCX2GameUnit;
			}

			if( keyCount <= 0 )
				break;
		}

	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	if( pRetUnit != NULL )
	{
		if( pRetUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			bUserUnit = true;
			CX2GUUser* pCX2GUUser = (CX2GUUser*)pRetUnit;
			return pCX2GUUser->GetUnitUID();
		}
		else
		{
			bUserUnit = false;
			CX2GUNPC* pCX2GUNPC = (CX2GUNPC*)pRetUnit;
			return pCX2GUNPC->GetUnitUID();
		}
	}

	return -1;
}

CX2SlideShot* CX2Game::GetSlideShot()
{ 
	return g_pData->GetSlideShot(); 
}




void CX2Game::SetShowAllUnitGageBar( bool bShow )
{
	m_bShowAllUnitGageBar = bShow;
	if ( m_pDLGSkillSlot != NULL )
#ifdef RIDING_SYSTEM
		if( NULL != g_pData && NULL != g_pData->GetUIManager() )
		{
			if( true == g_pData->GetUIManager()->GetShowRidingPetSkillslot() )
			{
				CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot(bShow);
				m_pDLGSkillSlot->SetShow( false );
			}
			else
			{
				CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot(false);
				m_pDLGSkillSlot->SetShow(bShow);
			}
		}
#else
		m_pDLGSkillSlot->SetShow( bShow );
#endif //RIDING_SYSTEM

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for ( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pGameUnit = m_UnitList[i];
		if ( pGameUnit != NULL && false == pGameUnit->IsMyUnit() && pGameUnit->GetGageData() != NULL )
			pGameUnit->SetShowGage( m_bShowAllUnitGageBar );
	}

#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		CX2PET *pPet = g_pData->GetPetManager()->GetMyPet();
		if( pPet != NULL )
		{
			pPet->SetShowGageBar( m_bShowAllUnitGageBar );
		}
	}
#endif
}



bool CX2Game::AnyUserUnitInRange( D3DXVECTOR3 pos, float fRange )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser != NULL )
		{
			if( pCX2GUUser->GetNowHp() <= 0.f )
				continue;

			if( GetDistance( pos, pCX2GUUser->GetPos() ) < fRange )
			{

				return true;
			}
		}
	}


	return false;
}


bool CX2Game::IsThereAnyUserUnitOnLine( int iLineIndex )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser == NULL )
			continue;
		
		if( CX2GameUnit::GUSI_DIE == pCX2GUUser->GetGameUnitState() )
			continue;

		if( pCX2GUUser->GetNowHp() <= 0.f )
			continue;

		if( iLineIndex == pCX2GUUser->GetSyncData().lastTouchLineIndex &&
			true == pCX2GUUser->GetUnitCondition().bFootOnLine )
		{
			return true;
		}

	}


	return false;
}




bool CX2Game::IsThereAnyUserUnitUsingSkill( CX2SkillTree::SKILL_ID eSkillID )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	BOOST_TEST_FOREACH( CX2GUUser*, pUser, m_UserUnitList )
	{
		if( NULL == pUser )
			continue;

		if( CX2GameUnit::GUSI_DIE == pUser->GetGameUnitState() )
			continue;

		if( pUser->GetNowHp() <= 0.f )
			continue;

		if( pUser->GetNowStateSkillID() == eSkillID )
			return true;
	}
	
	return false;
}





// @bEnterPortal : �ڷ���Ʈ�� ���°��� ������ ����; ĳ���͸� ���̰ų� �Ⱥ��̰ų�
void CX2Game::CreateStageLoadingTeleportEffectForAllUser( bool bEnterPortal )
{
	bool bShowUser = true;
	if( true == bEnterPortal )
	{
		m_bShowTeleportEffectOnStageStart = true;	// ���� �������� ������ �� �ڷ���Ʈ ȿ�� ���̵��� �ϱ�
		bShowUser = false;
	}

	
	
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser == NULL )
			continue;

		if( CX2GameUnit::GUSI_DIE == pCX2GUUser->GetGameUnitState() )
			continue;

		if( pCX2GUUser->GetNowHp() <= 0.f )
			continue;

		GetEffectSet()->PlayEffectSet( L"EffectSet_StageLoadingTeleport", (CX2GameUnit*)pCX2GUUser );

		pCX2GUUser->SetShowObject( bShowUser );
		pCX2GUUser->PlaySound( L"Nasod_Element_Teleport.ogg" );

		if( true == bEnterPortal )
		{
			pCX2GUUser->SetCanNotInputTime_LUA( 5.f );
			pCX2GUUser->AccessPhysicParam().nowSpeed		= D3DXVECTOR2( 0, 0 );
			pCX2GUUser->AccessPhysicParam().passiveSpeed	= D3DXVECTOR2( 0, 0 );
		}
		else
		{
			pCX2GUUser->SetCanNotInputTime_LUA( 0.f );
		}
	}
}


//{{oasis:�����////2009-10-7////
bool CX2Game::IsNearNPCUnitUID_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange )
{
	bool answer = false;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == unitID && pNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_OFF)
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance )
			{
				answer = true;
				break;
			}
		}
	}
	return answer;
}

CX2GUNPC* CX2Game::GetNearNoneAINPCUnitUID( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange )
{
	CX2GUNPC* retUnit = NULL;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == unitID && 
			pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() &&
			pNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_OFF) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance )
			{
				retUnit = pNPC;
				fDistance = fTempDist;
			}
		}
	}

	return retUnit;
}


D3DXVECTOR3	CX2Game::GetFixedDistancePosition_LUA( D3DXVECTOR3 pvPos, float fDist, bool bIsRight )
{
	return GetWorld()->GetLineMap()->GetFixedDistancePosition(&pvPos, fDist, bIsRight);
}



float CX2Game::GetDist_LUA( const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2 )
{
	D3DXVECTOR3 temp = pos2 - pos1;
	return D3DXVec3Length( &temp );
}

CX2GUUser* CX2Game::GetNearestUserUnit( const D3DXVECTOR3& vPos_ )
{
	CX2GUUser* pNearestGUUser = NULL;
	float fNearestDistance3Sq = 0;

	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pGUUser = m_UserUnitList[i];
		if( NULL == pGUUser )
			continue;

		if ( pGUUser->GetNowHp() <= 0.f || CX2GameUnit::GUSI_DIE == pGUUser->GetGameUnitState() )
			continue;

		const float fDistance3Sq = GetDistance3Sq( vPos_, pGUUser->GetPos() );
		if ( NULL == pNearestGUUser || fDistance3Sq < fNearestDistance3Sq )
		{
			pNearestGUUser		= pGUUser;
			fNearestDistance3Sq = fDistance3Sq;
		}
	}

	return pNearestGUUser;
}

UidType CX2Game::GetNearestUserUnitUID_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange )
{
	UidType retUID = 0;

	wstringstream wstream;
	wstream << L"GetNearestUserUnitUID_LUA ��� ����";
	ASSERT( !"GetTargetUnitUID_LUA ��� ����" );
	StateLog( wstream.str().c_str() );
// 	float fDistance = fMaxRange;
// 
// 	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
// 	{
// 		CX2GUUser* pGUUser = m_UserUnitList[i];
// 		if( NULL == pGUUser )
// 			continue;
// 
// 		if( pGUUser->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pGUUser->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
// 		{
// 			float fTempDist = GetDistance( pos, pGUUser->GetPos() );
// 			if( fTempDist < fDistance && fTempDist > fMinRange )
// 			{
// 				retUID = pGUUser->GetUnitUID();
// 				fDistance = fTempDist;
// 			}
// 		}
// 	}

	return retUID;
}

//{{oasis:�����////2009-10-8////
D3DXVECTOR3 CX2Game::GetNearestUserUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange )
{
	D3DXVECTOR3 retPos(0, 0, 0);
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pNPC = (CX2GUUser*)m_UserUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance && fTempDist > fMinRange )
			{
				retPos = pNPC->GetPos();
				fDistance = fTempDist;
			}
		}
	}
	return retPos;
}
//}}oasis:�����////2009-10-8////

D3DXVECTOR3 CX2Game::GetNearestNPCUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange )
{
	D3DXVECTOR3 retPos(0, 0, 0);
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;
		
		if( true == pNPC->GetInvincible() )
			continue;

		if( 0.f >= pNPC->GetNowHp() )
			continue;

		if( pNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
			continue;
	
		float fTempDist = GetDistance( pos, pNPC->GetPos() );
		if( fTempDist < fDistance && fTempDist > fMinRange )
		{
			retPos = pNPC->GetPos();
			fDistance = fTempDist;
		}
	}
	return retPos;
}

//}}oasis:�����////2009-10-7////

#ifdef SEASON3_MONSTER_2011_7
UidType CX2Game::GetFarthestUserUnitUID_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange )
{
	UidType retUID = 0;
	float fDistance = fMinRange;

	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pGUUser = m_UserUnitList[i];
		if( NULL == pGUUser )
			continue;

		if( pGUUser->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pGUUser->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pGUUser->GetPos() );
			if( fTempDist > fDistance && fTempDist < fMaxRange )
			{
				retUID = pGUUser->GetUnitUID();
				fDistance = fTempDist;
			}
		}
	}

	return retUID;
}
#endif SEASON3_MONSTER_2011_7


#ifdef VELDER_BOSS_5
//{{ oasis907 : ����� [2010.7.30] // 
D3DXVECTOR3 CX2Game::GetFarthestUserUnitPos_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange )
{
	D3DXVECTOR3 retPos(0, 0, 0);
	float fDistance = fMinRange;

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pNPC = (CX2GUUser*)m_UserUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pNPC->GetLandPos() );
			if( fTempDist > fDistance && fTempDist < fMaxRange )
			{
				retPos = pNPC->GetLandPos();
				fDistance = fTempDist;
			}
		}
	}
	return retPos;
}
//}}
#endif VELDER_BOSS_5

int CX2Game::GetNearNPCUnitUIDInStart_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange )
{
	int retUID = 0;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == unitID && pNPC->GetNPCSyncData().nowState == pNPC->GetStartState() )
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance )
			{
				retUID = (int)pNPC->GetUnitUID();
				fDistance = fTempDist;
			}
		}
	}

	return retUID;
}

CX2GUNPC*	CX2Game::GetNearNPCUnitInStart( const D3DXVECTOR3 vPos_, const CX2UnitManager::NPC_UNIT_ID eUnitID_, const float fMaxRange /*= 9999*/ )
{
	CX2GUNPC* pNearNpc = NULL;
	float fDistance3Sq = fMaxRange * fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == eUnitID_ && pNPC->GetNPCSyncData().nowState == pNPC->GetStartState() )
		{
			float fTempDistance3Sq = GetDistance3Sq( vPos_, pNPC->GetPos() );
			if( fTempDistance3Sq < fDistance3Sq )
			{
				pNearNpc		= pNPC;
				fDistance3Sq	= fTempDistance3Sq;
			}
		}
	}

	return pNearNpc;
}


int CX2Game::GetNearNPCUnitUID_LUA( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, float fMaxRange )
{
	int retUID = 0;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == unitID && 
			pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance )
			{
				retUID = (int)pNPC->GetUnitUID();
				fDistance = fTempDist;
			}
		}
	}

	return retUID;
}

CX2GUNPC*	CX2Game::GetNearestNpcInSpecificRangeByNpcId( const D3DXVECTOR3& vPos_, const CX2UnitManager::NPC_UNIT_ID eUnitID_, const float fMaxRange_ /*= 9999*/ )
{
	CX2GUNPC* pNearestNpc = NULL;
	float fDistance3Sq = fMaxRange_ * fMaxRange_;

	for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = m_NPCUnitList[i];
		if( NULL == pNPC )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == eUnitID_ && 
			pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDistance3Sq = GetDistance3Sq( vPos_, pNPC->GetPos() );
			if( fTempDistance3Sq < fDistance3Sq )
			{
				pNearestNpc		= pNPC;
				fDistance3Sq	= fTempDistance3Sq;
			}
		}
	}

	return pNearestNpc;
}

int CX2Game::GetNearNPCUnitUIDEx( D3DXVECTOR3 pos, CX2UnitManager::NPC_UNIT_ID unitID, int npcUIDToExcept, float fMaxRange )
{
	int retUID = 0;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if ( pNPC->GetUID() == npcUIDToExcept )
			continue;

		if( pNPC->GetNPCTemplet().m_nNPCUnitID == unitID && 
			pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance )
			{
				retUID = (int)pNPC->GetUnitUID();
				fDistance = fTempDist;
			}
		}
	}

	return retUID;
}

int CX2Game::GetNearNPCUnitUIDEx( D3DXVECTOR3 pos, const vector< CX2UnitManager::NPC_UNIT_ID >& vecUnitID, int npcUIDToExcept, float fMaxRange )
{
	int retUID = 0;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if ( pNPC->GetUID() == npcUIDToExcept )
			continue;

		if ( std::find( vecUnitID.begin(), vecUnitID.end(), pNPC->GetNPCTemplet().m_nNPCUnitID ) != vecUnitID.end() )
		{
			if( pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
			{
				float fTempDist = GetDistance( pos, pNPC->GetPos() );
				if( fTempDist < fDistance )
				{
					retUID = (int)pNPC->GetUnitUID();
					fDistance = fTempDist;
				}
			}
		}
	}

	return retUID;
}


int CX2Game::GetNearestNPCUnitUID( D3DXVECTOR3 pos, int npcUIDToExcept ,float fMaxRange /*= 9999*/ )
{
	int retUID = 0;
	float fDistance = fMaxRange;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if ( (int)pNPC->GetUnitUID() == npcUIDToExcept )
			continue;

		if( pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDist = GetDistance( pos, pNPC->GetPos() );
			if( fTempDist < fDistance )
			{
				retUID = (int)pNPC->GetUnitUID();
				fDistance = fTempDist;
			}
		}
	}

	return retUID;
}

CX2GUUser* CX2Game::GetNearestUserUnitInSpecificRange( const D3DXVECTOR3& vPos_, const float fMinRange_, const float fMaxRange_ /*= 9999 */ )
{
	CX2GUUser* pNearestGUUser = NULL;
	float fDistance3Sq = fMaxRange_ * fMaxRange_;

	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pGUUser = m_UserUnitList[i];
		if( NULL == pGUUser )
			continue;

		if ( pGUUser->GetNowHp() <= 0.f || CX2GameUnit::GUSI_DIE == pGUUser->GetGameUnitState() )
			continue;

		float fTempDistance3Sq = GetDistance3Sq( vPos_, pGUUser->GetPos() );
		if( fTempDistance3Sq < fDistance3Sq && fTempDistance3Sq > fMinRange_ )
		{
			pNearestGUUser = pGUUser;
			fDistance3Sq = fTempDistance3Sq;
		}
	}

	return pNearestGUUser;
}

// @bOnFront : ��� ������ �� ĳ���� ���ʿ� �ִ���
CX2GameUnit* CX2Game::GetFrontNearestEnemyUnit( CX2GameUnit* pMyUnit, float& fDistance )
{
	KTDXPROFILE();


	CX2GameUnit* pNearestGameUnit = NULL;
	fDistance = -1.f;
	D3DXVECTOR3 pos = pMyUnit->GetPos();

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pGameUnit = m_UnitList[i];
		if( NULL == pGameUnit )
			continue;

		if( pMyUnit->GetTeam() != pGameUnit->GetTeam() &&
			pGameUnit->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pGameUnit->GetGameUnitState() ) 
		{
			D3DXVECTOR3 vDir = pGameUnit->GetPos() - pos;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            float fY = vDir.y;
            vDir.y = 0.f;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
			if( D3DXVec3Dot( &vDir, &pGameUnit->GetDirVector() ) > 0.f )
			{
				if( false == pMyUnit->GetIsRight() )
					continue;
			}
			else
			{
				if( true == pMyUnit->GetIsRight() )
					continue;
			}
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            vDir.y = fY;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

			float fTempDistSquare = D3DXVec3LengthSq( &vDir );
			if( fDistance == -1.f || fTempDistSquare < fDistance )
			{
				fDistance = fTempDistSquare;
				pNearestGameUnit = pGameUnit;
			}
		}
	}

	if( NULL != pNearestGameUnit )
	{
		fDistance = sqrt( fDistance );
	}
	
	return pNearestGameUnit;
}






bool CX2Game::ChangeRebirthStateNPCUnit_LUA( int NPCUID )
{
	CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( NPCUID );
	if( pCX2GUNPC != NULL && pCX2GUNPC->GetNowHp() > 0.0f )
	{
		pCX2GUNPC->StateChange( pCX2GUNPC->GetRebirthState() );
		return true;
	}
	return false;
}

int CX2Game::GetNPCCountAt( D3DXVECTOR3 vPosition, float fRadius /*= 30.f*/, CX2UnitManager::NPC_UNIT_ID eNPCID /*= CX2UnitManager::NUI_NONE*/ )
{
	int iNPCCount = 0;

	for( UINT i=0; i<m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pGUNPC = m_NPCUnitList[i];
		if( NULL == pGUNPC )
			continue;

		if( eNPCID != CX2UnitManager::NUI_NONE && eNPCID != pGUNPC->GetNPCTemplet().m_nNPCUnitID )
			continue;

		if( GetDistance( vPosition, pGUNPC->GetPos() ) < fRadius )
			iNPCCount += 1;
	}

	return iNPCCount;

}


D3DXVECTOR3 CX2Game::GetEmptyStartPos()
{
	if( m_pWorld == NULL )
		return D3DXVECTOR3(0,0,0);

	if( m_pWorld->GetLineMap() == NULL )
		return D3DXVECTOR3(0, 0, 0 );

	CKTDGLineMap* pLineMap = m_pWorld->GetLineMap();

#ifdef	X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
    const CKTDGLineMap::StartInfoMap& mapStartInfo = pLineMap->GetStartInfoMap();
	CKTDGLineMap::StartInfoMap::const_iterator it;
	for( it = mapStartInfo.begin(); it != mapStartInfo.end(); it++ )
	{
		const D3DXVECTOR3& vPos = it->second.m_vStartPos;
		int nNpcCount = GetNPCCountAt( vPos, 500.f );
		if( nNpcCount == 0 )
		{
			return vPos;
		}
	}

	if( mapStartInfo.empty() == false )
	{
        return  mapStartInfo.begin()->second.m_vStartPos;
	}
	else
	{
		return pLineMap->GetLineData(0)->startPos;
	}
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	map<int, D3DXVECTOR3>& mapStartPos = pLineMap->GetStartPosMap();
	map<int, D3DXVECTOR3>::iterator it;
	for( it = mapStartPos.begin(); it != mapStartPos.end(); it++ )
	{
		D3DXVECTOR3& vPos = it->second;
		int nNpcCount = GetNPCCountAt( vPos, 500.f );
		if( nNpcCount == 0 )
		{
			return vPos;
		}
	}

	if( mapStartPos.size() != 0 )
	{
		it = mapStartPos.begin();
		return it->second;
	}
	else
	{
		return pLineMap->GetLineData(0)->startPos;
	}
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
}





// ���� ��ȯ �ʻ��� ���������� ���� �Ŀ� ����� �� ���ٴ� ��� �޼����� �����ֱ� ���ؼ� IsSummonSpecialAttack �� �ϵ��ڵ� �ؾ� �մϴ�.
void CX2Game::SetUserSummonedNPCInfo( CX2NPCAI::NPC_AI_TYPE eAIType, UidType iAllyUID, CX2GUNPC* pNPC, int iHyperMode, CX2UnitManager::NPC_UNIT_ID unitID )
{
	// �̺��� ��ȯ �ʻ�⿡ ���� ��ȯ�� ������ ��� npc stat�� ���� ���� ���ش�
	// warning!! �ϴ��� allynpc�� ��ȯ�Ǵ� NPC�� �̺� �ۿ� �����Ƿ� �̷��� �ϵ��ڵ�
	// level�� �ϴ� �̺��� �ʻ�� ����, ������ ���� ���� ��ġ�� ��� ������ ���. ���߿��� ����... 

#ifdef	SERV_TRAPPING_RANGER_TEST
	if( (CX2NPCAI::NAT_ALLY == eAIType && -1 != iAllyUID) || CX2Room::TN_NONE != pNPC->GetAllyTeam() )
#else	SERV_TRAPPING_RANGER_TEST
	if( CX2NPCAI::NAT_ALLY == eAIType && -1 != iAllyUID )
#endif	SERV_TRAPPING_RANGER_TEST 
	{
		CX2GUUser* pUser = GetUserUnitByUID( iAllyUID );
		if( NULL != pUser )
		{
			// ��ȯ ���� ������ ��ȯ���� ������ ����
			pNPC->SetHardLevel( pUser->GetUnit()->GetUnitData().m_Level, false );
			
			float fStatAtkRate = 1.f;
#ifdef NEW_SKILL_TREE
			fStatAtkRate = pUser->GetStatAtkRateOfSummoned();
#endif NEW_SKILL_TREE

			//{{ megagame / �ڱ��� / 2010.04.30 / NPC ���� �߰�
#ifdef SUMMONED_NPC_ADDITIONAL_STAT
			// ��ȯ ���� ũ��Ȯ��, ���߷�, �ӵ�, �̵��ӵ�, �����ӵ��� ��ȯ���� �Ͱ� ����
			CX2UnitManager::NPCUnitStat NPCStat;
			pNPC->GetNPCStat(NPCStat);

			//{{ kimhc // 2011-07-28 // �ɼǼ�ġȭ
			NPCStat.m_fPercentCritical = pUser->GetCriticalRate();
			NPCStat.m_fAccuracy = pUser->GetAccuracyPercent();			
				//}} kimhc // 2011-07-28 // �ɼǼ�ġȭ

			

			pNPC->SetNPCStat(NPCStat);
#endif SUMMONED_NPC_ADDITIONAL_STAT
			//}}

			// ������ �������� npc ������ �����Ѵ�.
			CX2Stat::Stat npcStat = pUser->GetStat().GetStat();
			npcStat.m_fAtkPhysic	= pUser->GetPhysicAttackValue();
			npcStat.m_fAtkMagic		= pUser->GetMagicAttackValue();
			npcStat.m_fDefPhysic	= pUser->GetPhysicDefenceValue();
			npcStat.m_fDefMagic		= pUser->GetMagicDefenceValue();
#ifdef FIX_SUMMON_NPC_SKILL_DAMAGE		/// ��ȯ�� ���ݿ� ����, ���� ���� ���� �ʵǴ� ���׿� ���� ����
			{
				npcStat.m_fAtkPhysic += npcStat.m_fAtkPhysic * pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkPhysicRate;
				npcStat.m_fAtkMagic += npcStat.m_fAtkMagic * pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkMagicRate;
							
#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER
				pNPC->SetAdditionalAttack ( pUser->GetAdditionalAttack() );			
#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER


			}
#endif FIX_SUMMON_NPC_SKILL_DAMAGE

// 			if( pUser->GetStat().GetAddOnTime().m_fAtkPhysic > 0.f )
// 				npcStat.m_fAtkPhysic += pUser->GetStat().GetAddOnStat().m_fAtkPhysic;
// 			if( pUser->GetStat().GetAddOnTime().m_fAtkMagic > 0.f )
// 				npcStat.m_fAtkMagic += pUser->GetStat().GetAddOnStat().m_fAtkMagic;
// 			if( pUser->GetStat().GetAddOnTime().m_fDefPhysic > 0.f )
// 				npcStat.m_fDefPhysic += pUser->GetStat().GetAddOnStat().m_fDefPhysic;
// 			if( pUser->GetStat().GetAddOnTime().m_fDefMagic > 0.f )
// 				npcStat.m_fDefMagic += pUser->GetStat().GetAddOnStat().m_fDefMagic;




			float fStatScale = 1.f;
			float fHyperModeStatScale = 1.f;
			if( iHyperMode > 0 ) // �����ϰ� �ʻ�⸦ ����� ���
			{
				fHyperModeStatScale = 1.2f;
			}

#ifdef ADD_RENA_SYSTEM //��â��
			//npc�� ��ȯ�ɶ� �������Լ� ��ų ���õ����͸� �޾ƿ� ����
			pNPC->SetRelateSkillData( pUser->GetNowDamageRelateSkillData() );
#endif //ADD_RENA_SYSTEM

			switch( unitID )
			{

			default:
				{
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );
				} break;


			case CX2UnitManager::NUI_NASOD_WATCH_EVE:
#ifdef SERV_SKILL_NOTE
			case CX2UnitManager::NUI_NASOD_WATCH_EVE_MEMO:
#endif
				{
#ifdef NEW_SKILL_TREE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EN_DIMENSION_LINK );
#else //UPGRADE_SKILL_SYSTEM_2013
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EN_DIMENSION_LINK );
#endif //UPGRADE_SKILL_SYSTEM_2013
					if( NULL != pSkillTemplet )
					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
							return;
	
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_EN_DIMENSION_LINK ) );	/// ��ų ����
	
						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						fStatScale = pSkillTemplet->m_fPowerRate;
	#endif // UPGRADE_SKILL_SYSTEM_2013
					}

					fStatScale *= fHyperModeStatScale;
#ifdef FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
					if( NULL != pUser )
					{
						fStatScale *= (1.f + pUser->GetSocketData().m_fSkillDamageUpRate);
					}
#endif FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
#endif NEW_SKILL_TREE


					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 0.15f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f * fStatAtkRate,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f * fStatAtkRate,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 0.8f );
				} break;

#ifdef FINALITY_SKILL_SYSTEM //��â��
			case CX2UnitManager::NUI_SI_HA_FERDINAND:
				{
					if( NULL != pUser->GetUnit() && CX2Unit::UT_EVE == pUser->GetUnit()->GetType() )
					{
						float fPowerRate = 1.f;
						const int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_HA_EEP_LINK_OVERCHARGE_ILLUSION );	//�������� �ñر� - ��ũ �������� �����Ʈ
						if( iSkillLevel > 0 )
						{	
							const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_HA_EEP_LINK_OVERCHARGE_ILLUSION ); 

							if( NULL != pSkillTemplet )
							{
								if ( NULL == pUser->GetUnit() )
									return;

								const int iSkillTempletLevel = max( 1, iSkillLevel );	/// ��ų ����

								fPowerRate *= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
							}
						}
#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
						/// AT_SPECIAL ���� ���� ���� ���� ����
						fPowerRate *= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

						fStatScale *= fHyperModeStatScale;

						pNPC->SetNPCStat( 
							npcStat.m_fBaseHP		* fStatScale	* 1.f,
							npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
							npcStat.m_fAtkMagic		* fStatScale	* 1.f,
							npcStat.m_fDefPhysic	* fStatScale	* 1.f,
							npcStat.m_fDefMagic		* fStatScale	* 1.f );

						pNPC->SetDefaultPowerRate( fPowerRate );
						pNPC->SetNearestGUUser(pUser);
						const int iNpcUid = pNPC->GetUID();
						CX2GUEve* pEve = static_cast<CX2GUEve*>( pUser );
						if( NULL != pEve )
							pEve->SaveFerdinandNPCUID(iNpcUid);
					}
				} break;
#endif //FINALITY_SKILL_SYSTEM

			case CX2UnitManager::NUI_WALLY_9TH_EVE:
				{
#ifdef NEW_SKILL_TREE
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EN_DIMENSION_LINK_BLADER );
					if( NULL != pSkillTemplet )
					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
							return;
	
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						fStatScale = pSkillTemplet->m_fPowerRate;
	#endif // UPGRADE_SKILL_SYSTEM_2013
					}

					fStatScale *= fHyperModeStatScale;
#ifdef FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
					if( NULL != pUser )
					{
						fStatScale *= (1.f + pUser->GetSocketData().m_fSkillDamageUpRate);
					}
#endif FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
#endif NEW_SKILL_TREE

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.2f * fStatAtkRate,
						npcStat.m_fAtkMagic		* fStatScale	* 0.8f * fStatAtkRate,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
					/// AT_SPECIAL ���� ���� ���� ���� ����
					float fSummonMonsterPowerRate	= 1.f;
					fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
					pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

				} break;


			case CX2UnitManager::NUI_NASOD_ELEMENT_FIRE_EVE:
				{
#ifdef NEW_SKILL_TREE
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EN_DIMENSION_LINK_GUARDIAN );
					if( NULL != pSkillTemplet )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
							return;

						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_EN_DIMENSION_LINK_GUARDIAN ) );	/// ��ų ����

						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
						fStatScale = pSkillTemplet->m_fPowerRate;
#endif // UPGRADE_SKILL_SYSTEM_2013
					}


					fStatScale *= fHyperModeStatScale;
#ifdef FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
					if( NULL != pUser )
					{
						fStatScale *= (1.f + pUser->GetSocketData().m_fSkillDamageUpRate);
					}
#endif FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
#endif NEW_SKILL_TREE

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 0.f  * fStatAtkRate, 
						npcStat.m_fAtkMagic		* fStatScale	* 1.2f * fStatAtkRate,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
					/// AT_SPECIAL ���� ���� ���� ���� ����
					float fSummonMonsterPowerRate	= 1.f;
					fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
					pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

				} break;

#ifdef EVE_FIRST_CHANGE_JOB 
				//case CX2UnitManager::NUI_GUARDIAN_EVE:
			case CX2UnitManager::NUI_GUARDIAN_SP1_EVE:
			case CX2UnitManager::NUI_GUARDIAN_SP1_EVE_MEMO:
				{
#ifdef NEW_SKILL_TREE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_EAT_SURFACE_CUTING );
#else //UPGRADE_SKILL_SYSTEM_2013
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EAT_SURFACE_CUTING );
#endif //UPGRADE_SKILL_SYSTEM_2013
					if( NULL != pSkillTemplet )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
							return;

						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����

						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
						fStatScale = pSkillTemplet->m_fPowerRate;
#endif // UPGRADE_SKILL_SYSTEM_2013
					}

					fStatScale *= fHyperModeStatScale;
#ifdef FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
					if( NULL != pUser )
					{
						fStatScale *= (1.f + pUser->GetSocketData().m_fSkillDamageUpRate);
					}
#endif FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
#endif NEW_SKILL_TREE


					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.2f * fStatAtkRate,
						npcStat.m_fAtkMagic		* fStatScale	* 0.8f * fStatAtkRate,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
					/// AT_SPECIAL ���� ���� ���� ���� ����
					float fSummonMonsterPowerRate	= 1.f;
					fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
					pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

				} break;

			case CX2UnitManager::NUI_GUARDIAN_SP2_EVE:
#ifdef SERV_SKILL_NOTE
			case CX2UnitManager::NUI_GUARDIAN_SP2_EVE_MEMO:
#endif
				{
#ifdef NEW_SKILL_TREE

					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EAT_SONIC_WAVE );
					if( NULL != pSkillTemplet )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
							return;

						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_EAT_SONIC_WAVE ) );	/// ��ų ����

						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
						fStatScale = pSkillTemplet->m_fPowerRate;
#endif // UPGRADE_SKILL_SYSTEM_2013
					}

					fStatScale *= fHyperModeStatScale;
#ifdef FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
					if( NULL != pUser )
					{
						fStatScale *= (1.f + pUser->GetSocketData().m_fSkillDamageUpRate);
					}
#endif FIX_SUMMON_NPC_SKILL_DAMAGE_BY_USE_PROTECTION_OF_LAHELL
#endif NEW_SKILL_TREE

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.2f * fStatAtkRate,
						npcStat.m_fAtkMagic		* fStatScale	* 0.8f * fStatAtkRate,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
					/// AT_SPECIAL ���� ���� ���� ���� ����
					float fSummonMonsterPowerRate	= 1.f;
					fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
					pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

				} break;

#endif EVE_FIRST_CHANGE_JOB
				//{{ JHKang / ������ / 2010/11/22 / �̺� ����� ����
#ifdef NEW_SKILL_2010_11
 			case CX2UnitManager::NUI_ATOMIC_SHIELD_EVE:
 				{
 					float fMaxHpRel = 1.f;
 					float fShieldSize = 1.f;
 					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ENS_ATOMIC_SHIELD );
 					if( NULL != pSkillTemplet )
 					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
						return;
	
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// ��ų ����
	
						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
						/// ���� ��ü HP ũ��
						fMaxHpRel = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_HP_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_UI
						fStatScale = pSkillTemplet->m_fPowerRate;
						/// ���� ��ü HP ũ��
						fMaxHpRel = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_HP_REL );
	#endif // UPGRADE_SKILL_UI

#ifdef SKILL_BALANCE_20110728
						switch( g_pMain->GetNowStateID() )
						{
						case CX2Main::XS_DUNGEON_GAME:
						case CX2Main::XS_BATTLE_FIELD:
							{
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
								// ���� HP ���� 1.5�� �������� 2.0�� ������ ����
								fMaxHpRel *= 2.0f;
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
								fMaxHpRel *= 1.5f;
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
							} break;
						default:
							break;
						}
#endif

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						/// ���� ũ��
						fShieldSize = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						/// ���� ũ��
						fShieldSize = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
 					}
 
 						fStatScale *= fHyperModeStatScale;
 
 						pNPC->SetNPCStat( 
 							pUser->GetMaxHp() * fMaxHpRel,
 							npcStat.m_fAtkPhysic	* fStatScale	* 1.f * fStatAtkRate,
 							npcStat.m_fAtkMagic		* fStatScale	* 1.f * fStatAtkRate,
 							npcStat.m_fDefPhysic	* fStatScale	* 1.f,
 							npcStat.m_fDefMagic		* fStatScale	* 0.8f );

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
						/// AT_SPECIAL ���� ���� ���� ���� ����
						float fSummonMonsterPowerRate	= 1.f;
						fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
						pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
 
 						const D3DXVECTOR3 vScale( pUser->GetScaleByUnit().x * fShieldSize / 450.f, pUser->GetScaleByUnit().x * fShieldSize / 450.f,
 							pUser->GetScaleByUnit().x * fShieldSize / 450.f);
 						pNPC->SetScaleByUnit( vScale );
						pNPC->SetDamageOutScreen( false );	// ������ ���� ǥ�� ����
	#ifdef BALANCE_CODE_NEMESIS_20121213
						// ��ų������ ���� �ٸ� �������͸� �����ϱ� ���� ������ �߰�
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ENS_ATOMIC_SHIELD );
		#else // UPGRADE_SKILL_SYSTEM_2013
						int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ENS_ATOMIC_SHIELD );
		#endif // UPGRADE_SKILL_SYSTEM_2013
						pNPC->SetRealatedSkillLevel(iSkillLevel);
	#endif //BALANCE_CODE_NEMESIS_20121213
 				} break;
#endif NEW_SKILL_2010_11
				//}} JHKang / ������ / 2010/11/22 / �̺� ����� ����





				// ���� ���ʱ� Ʈ��
			case CX2UnitManager::NUI_ENTANGLE_TRAP_RENA:
				{
					if( NULL != pUser->GetUnit() && 
						CX2Unit::UT_LIRE == pUser->GetUnit()->GetType() )
					{
						CX2GULire_ElvenRanger* pRena = (CX2GULire_ElvenRanger*) pUser;
						pNPC->SetEntangleTrapAttackDuration( pRena->GetEntangleTrapAttackDuration() );
						pRena->AddEntangleTrap( pNPC );
					}
				} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
				// ���� ������ ���� ��ȯ NPC
			case CX2UnitManager::NUI_RENA_ANGER_OF_ELF:
			case CX2UnitManager::NUI_RENA_ANGER_OF_ELF_DELAY:
				{
					float fNpcHp = 1000000.0f;
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						fNpcHp,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_LIRE == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_LIRE == pUser->GetUnit()->GetType() )
					{
						float fPowerRate = 1.f;
						int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY, true );	//	��ɲ��� ���
						if( iSkillLevel > 0 )
						{
							const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

							if( NULL != pSkillTemplet )
							{
								fPowerRate *= ( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel ) );
							}
						}


						iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_RNW_ANGER_OF_ELF );	// ��������� �г�
						if( iSkillLevel > 0 )
						{
							const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RNW_ANGER_OF_ELF ); 

							if( NULL != pSkillTemplet )
							{
								if ( NULL == pUser->GetUnit() )
									return;

								const int iSkillTempletLevel = max( 1, iSkillLevel );	/// ��ų ����

								fPowerRate *= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
							}
						}
#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
						/// AT_SPECIAL ���� ���� ���� ���� ����
						fPowerRate *= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

						pNPC->SetDefaultPowerRate( fPowerRate );
						CX2GULire_ElvenRanger* pRena = static_cast<CX2GULire_ElvenRanger*>( pUser );
						pRena->AddAngerOfElfTrap( pNPC );
					}

				} break;
#endif //UPGRADE_SKILL_SYSTEM_2013




#ifdef AISHA_SECOND_CLASS_CHANGE


			case CX2UnitManager::NUI_UFO_AISHA:
				{
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADM_HELL_DROP );
#else //UPGRADE_SKILL_SYSTEM_2013
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AVP_HELL_DROP );
#endif //UPGRADE_SKILL_SYSTEM_2013
						if( NULL != pSkillTemplet )
						{
#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							if ( NULL == pUser->GetUnit() )
								return;

							const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

							const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ADM_HELL_DROP ) );	/// ��ų ����

							float fSummonMonsterPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );		/// ��ų ������ ���� ���� ����
	#else // UPGRADE_SKILL_SYSTEM_2013
							float fSummonMonsterPowerRate	= pSkillTemplet->m_fPowerRate;		/// ��ų ������ ���� ���� ����
	#endif // UPGRADE_SKILL_SYSTEM_2013

							fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );	/// AT_SPECIAL ���� ���� ���� ���� ����
							pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#else ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
							pNPC->SetDefaultPowerRate( pSkillTemplet->m_fPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

							CX2GUArme_VioletMage* pAisha = (CX2GUArme_VioletMage*) pUser;
							pAisha->SetUFO( pNPC );	
							CX2EffectSet::Handle hUfoEffect = GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Hell_Drop_Ring_And_Light", pNPC );
							pNPC->MakeEffectSetBeDeletedWhenDie( hUfoEffect );

		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							pNPC->ScaleMoveSpeed( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REMOTE_SPEED_ABS, iSkillTempletLevel ) );
		#else // UPGRADE_SKILL_SYSTEM_2013
							pNPC->ScaleMoveSpeed( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REMOTE_SPEED_ABS) );
		#endif // UPGRADE_SKILL_SYSTEM_2013
						}
					}
					else
					{
						pNPC->SetNowHp( 0.f );
					}
				} break;

#ifdef ADD_MEMO_1ST_CLASS //��â��
			case CX2UnitManager::NUI_UFO_AISHA_DROPPER_MEMO:
				{
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADM_HELL_DROP );
#else //UPGRADE_SKILL_SYSTEM_2013
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AVP_HELL_DROP );
#endif //UPGRADE_SKILL_SYSTEM_2013
						if( NULL != pSkillTemplet )
						{
#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							if ( NULL == pUser->GetUnit() )
								return;

							const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

							const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ADM_HELL_DROP ) );	/// ��ų ����

							float fSummonMonsterPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );		/// ��ų ������ ���� ���� ����
#else // UPGRADE_SKILL_SYSTEM_2013
							float fSummonMonsterPowerRate	= pSkillTemplet->m_fPowerRate;		/// ��ų ������ ���� ���� ����
#endif // UPGRADE_SKILL_SYSTEM_2013

							fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );	/// AT_SPECIAL ���� ���� ���� ���� ����
							pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#else ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
							pNPC->SetDefaultPowerRate( pSkillTemplet->m_fPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

							CX2GUArme_VioletMage* pAisha = (CX2GUArme_VioletMage*) pUser;
							pAisha->SetUFO( pNPC );	
							CX2EffectSet::Handle hUfoEffect = GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Hell_Drop_Ring_And_Light", pNPC );
							pNPC->MakeEffectSetBeDeletedWhenDie( hUfoEffect );
						}
					}
					else
					{
						pNPC->SetNowHp( 0.f );
					}
				} break;
#endif //ADD_MEMO_1ST_CLASS

			//case CX2UnitManager::NUI_PIG_BAT_AISHA:
			//	{
			//		ASSERT( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() );
			//		if( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() )
			//		{
			//			const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AVP_SUMMON_BAT );
			//			if( NULL != pSkillTemplet )
			//			{
			//				pNPC->SetDefaultPowerRate( pSkillTemplet->m_fPowerRate * fHyperModeStatScale );

			//				CX2GUArme_VioletMage* pAisha = (CX2GUArme_VioletMage*) pUser;
			//				pAisha->AddPigBat( pNPC->GetUID(), pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME ) );
			//			}
			//		}
			//		else
			//		{
			//			pNPC->SetNowHP( 0.f );
			//		}
			//	} break;

			case CX2UnitManager::NUI_STONE_WALL_AISHA:
				{
					float fHPScale = 1.f;
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AEM_STONE_WALL );
					if( NULL != pSkillTemplet )
					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
						if ( NULL == pUser->GetUnit() )
						return;
	
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AEM_STONE_WALL ) );	/// ��ų ����
	
						fHPScale = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_HP_ABS, iSkillTempletLevel );
						fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						fHPScale = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_HP_ABS );
						fStatScale = pSkillTemplet->m_fPowerRate;
	#endif // UPGRADE_SKILL_SYSTEM_2013

						fHPScale *= fHyperModeStatScale;
						fStatScale *= fHyperModeStatScale;

						float fNpcHp = pUser->GetUnitLevel()	* fHPScale;
						if( GetGameType() == GT_PVP )
						{
							fNpcHp = pNPC->GetStat().GetStat().m_fBaseHP;
						}	

						pNPC->SetNPCStat( 
							fNpcHp,
							npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
							npcStat.m_fAtkMagic		* fStatScale	* 1.f,
							pNPC->GetStat().GetStat().m_fDefPhysic	* 1.f,
							pNPC->GetStat().GetStat().m_fDefMagic		* 1.f );

						ASSERT( CX2Unit::UT_ARME == pUser->GetUnit()->GetUnitTemplet()->m_UnitType );
						if( CX2Unit::UT_ARME == pUser->GetUnit()->GetUnitTemplet()->m_UnitType )
						{
							CX2GUArme_VioletMage* pAisha = (CX2GUArme_VioletMage*) pUser;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							pAisha->AddStoneWall( pNPC->GetUID(), pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
							pAisha->AddStoneWall( pNPC->GetUID(), pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013
						}
						else
						{
							pNPC->SetNowHp( 0.f );
						}
					}
				} break;

#endif AISHA_SECOND_CLASS_CHANGE
	
#ifdef CHUNG_SECOND_CLASS_CHANGE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			case CX2UnitManager::NUI_TARGETING_CHUNG_NOT_SHOW_OTHER_PLAYER :
				{
					// ��ƿ���� ��Ʈ����ũ ���� �߸���Ÿ �� NPC
					// �ش� NPC �� Targeting_Chung.lua ������ ��� �մϴ�.
					// Flag 1 �� False �� ���� �� �� ���İ� ���濡�� ���̰�
					// True �� ���� �Ⱥ��δ�.

					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_CSG_ARTILLERY_STRIKE_QUANTUM_BALLISTA );

						if( NULL != pSkillTemplet )
						{
							if ( NULL == pUser->GetUnit() )
								return;

							const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
							const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_CSG_ARTILLERY_STRIKE_QUANTUM_BALLISTA ) );
							float fSummonMonsterPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
							fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
							pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
							CX2GUChung* pChung = static_cast<CX2GUChung*> ( pUser );
							pChung->SetShot( pNPC->GetUID() );
							pNPC->SetFlag_LUA( 1, true );
						}
					}
				} break;
#endif //UPGRADE_SKILL_SYSTEM_2013 //JHKang

				case CX2UnitManager::NUI_TARGETING_CHUNG:
				{
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
							const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_CTT_ARTILLERY_STRIKE );
#else //UPGRADE_SKILL_SYSTEM_2013
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_CDC_ARTILLERY_STRIKE );
#endif //UPGRADE_SKILL_SYSTEM_2013
						if( NULL != pSkillTemplet )
						{
#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							if ( NULL == pUser->GetUnit() )
								return;

							const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

							const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_CTT_ARTILLERY_STRIKE ) );	/// ��ų ����

							float fSummonMonsterPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );		/// ��ų ������ ���� ���� ����
	#else // UPGRADE_SKILL_SYSTEM_2013
							float fSummonMonsterPowerRate	= pSkillTemplet->m_fPowerRate;		/// ��ų ������ ���� ���� ����
	#endif // UPGRADE_SKILL_SYSTEM_2013

							fSummonMonsterPowerRate			*= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );	/// AT_SPECIAL ���� ���� ���� ���� ����
							pNPC->SetDefaultPowerRate( fSummonMonsterPowerRate );
#else ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
							pNPC->SetDefaultPowerRate( pSkillTemplet->m_fPowerRate );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

							CX2GUChung* pChung = (CX2GUChung*) pUser;
							pChung->SetShot( pNPC->GetUID() );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // û ��ų ����, ������
							// �ش� NPC �� Targeting_Chung.lua ������ ��� �մϴ�.
							// Flag 1 �� False �� ���� �� �� ���İ� ���濡�� ���̰�
							// True �� ���� �Ⱥ��δ�.
							pNPC->SetFlag_LUA( 1, false );
#endif // UPGRADE_SKILL_SYSTEM_2013 // û ��ų ����, ������
						}
					}
					else
					{
						pNPC->SetNowHp( 0.f );
					}
				} break;
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
			case CX2UnitManager::NUI_EVOKE_TRAP_LIRE:
#ifdef ADDITIONAL_MEMO
			case CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO:
#endif
				{
					float fNpcHp = 1000000.0f;
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						fNpcHp,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_LIRE == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_LIRE == pUser->GetUnit()->GetType() )
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
						int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY, true );	//	��ɲ��� ���

						float fPowerRate = 1.f;

						if( iSkillLevel > 0 )
						{
							const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_HUNTERS_ABILITY );

							if( NULL != pSkillTemplet )
							{
								fPowerRate *= ( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel ) );
							}
						}

						iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LTR_EVOKE );	//	�̺�ũ

						if( iSkillLevel > 0 )
						{
		#ifdef FIX_EVOKE_SKILLING
							const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_LTR_EVOKE ); 
		#else
							const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LTR_EVOKE, iSkillLevel );
		#endif //FIX_EVOKE_SKILLING
							if( NULL != pSkillTemplet )
							{
								if ( NULL == pUser->GetUnit() )
									return;

								const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

								const int iSkillLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_LTR_EVOKE ) );	/// ��ų ����

								fPowerRate *= pSkillTemplet->GetSkillPowerRateValue( iSkillLevel );
							}
						}
#else //UPGRADE_SKILL_SYSTEM_2013
						int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY );	//	��ɲ��� ���

						float fPowerRate = 1.f;

						if( iSkillLevel > 0 )
						{
							const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_HUNTERS_ABILITY, iSkillLevel );

							if( NULL != pSkillTemplet )
							{
								fPowerRate *= ( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL ) );
							}
						}

						iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_RTR_EVOKE );	//	�̺�ũ

						if( iSkillLevel > 0 )
						{
		#ifdef FIX_EVOKE_SKILLING
							const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RTR_EVOKE ); 
		#else
							const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_LTR_EVOKE, iSkillLevel );
		#endif //FIX_EVOKE_SKILLING

							if( NULL != pSkillTemplet )
							{
								fPowerRate *= pSkillTemplet->m_fPowerRate;
							}
						}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
						/// AT_SPECIAL ���� ���� ���� ���� ����
						fPowerRate *= pUser->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

						pNPC->SetDefaultPowerRate( fPowerRate );
						CX2GULire_ElvenRanger* pLire = static_cast<CX2GULire_ElvenRanger*>( pUser );
						pLire->InsertEvokeNpc( pNPC );
					}
					else
					{
						pNPC->SetNowHp( 0.f );
					}
				} break;
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef SERV_ARME_DIMENSION_WITCH
			case CX2UnitManager::NUI_ADW_DISTORTION:
				{
					fStatScale *= fHyperModeStatScale;

					pNPC->SetNPCStat( 
						npcStat.m_fBaseHP		* fStatScale	* 1.f,
						npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
						npcStat.m_fAtkMagic		* fStatScale	* 1.f,
						npcStat.m_fDefPhysic	* fStatScale	* 1.f,
						npcStat.m_fDefMagic		* fStatScale	* 1.f );


					ASSERT( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_ARME == pUser->GetUnit()->GetType() )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADW_DISTORTION );
						if( NULL != pSkillTemplet )
						{
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
							if ( NULL == pUser->GetUnit() )
								return;
	
							const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
							const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ADW_DISTORTION ) );	/// ��ų ����
	
							pNPC->SetDefaultPowerRate( pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel ) );

#ifdef MODIFY_DISTORTION_DESTRUCTION
							// ���� ��ȯ ���� �ð�
							float fRemainTime = static_cast<float>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel ));
							pNPC->StartSelfDestruction(fRemainTime);
#endif // MODIFY_DISTORTION_DESTRUCTION

		#else // UPGRADE_SKILL_SYSTEM_2013
							pNPC->SetDefaultPowerRate( pSkillTemplet->m_fPowerRate );

#ifdef MODIFY_DISTORTION_DESTRUCTION
							float fRemainTime = static_cast<float>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME ) );
							pNPC->StartSelfDestruction(fRemainTime);
#endif // MODIFY_DISTORTION_DESTRUCTION

		#endif // UPGRADE_SKILL_SYSTEM_2013

						}
					}
					else
					{
						pNPC->SetNowHp( 0.f );
					}
				} break;
#endif

#ifdef	SERV_CHUNG_TACTICAL_TROOPER
			case CX2UnitManager::NUI_REMOTE_MORTAR:		/// û 2�� ���� ��Ƽ�� Ʈ���� - �ڰ���
				{
					fStatScale *= fHyperModeStatScale;

					/// ���ݷ¸� ������ �ɷ�ġ�� �޾ƿ�
					pNPC->SetNPCStat( 
						pNPC->GetStat().GetStat().m_fBaseHP,
						npcStat.m_fAtkPhysic * fStatScale	* 1.f,
						npcStat.m_fAtkMagic * fStatScale	* 1.f,
						pNPC->GetStat().GetStat().m_fDefPhysic,
						pNPC->GetStat().GetStat().m_fDefMagic);

					ASSERT( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() );
					if( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() )
					{
						float fPowerRate = 1.f;
						int   iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_CTT_AUTOMATIC_MORTAR );

						if( iSkillLevel > 0 )
						{
							const CX2SkillTree::SkillTemplet* pSkillTemplet = pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_CTT_AUTOMATIC_MORTAR ); 

							if( NULL != pSkillTemplet )
							{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
								if ( NULL == pUser->GetUnit() )
									return;
	
								const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
								const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_CTT_AUTOMATIC_MORTAR ) );	/// ��ų ����
	
								fPowerRate *= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
								fPowerRate *= pSkillTemplet->m_fPowerRate;
	#endif // UPGRADE_SKILL_SYSTEM_2013

								fPowerRate -= 1.f;
							}
							
						}

						pNPC->SetDefaultPowerRate( fPowerRate );

						pNPC->SetNowHp( pNPC->GetMaxHp() );

						CX2GUChung* pChung = static_cast<CX2GUChung*>( pUser );

						if( NULL != pChung )
							pChung->InsertRemoteMortarNPCUID( static_cast<UidType>( pNPC->GetUID() ) );
					}
					else
					{
						pNPC->SetNowHp( 0.f );
					}
				} break;
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
			case CX2UnitManager::NUI_ELESIS_FIRE_BLOSSOM:		/// ����¡ ��Ʈ - �Ҳ� ��ȭ.
				{
					if( NULL != pUser->GetUnit() && pUser->GetUnitClass() == CX2Unit::UC_ELESIS_BLAZING_HEART )
					{
						const int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_SBH_FIRE_BLOSSOMS, true );
						pNPC->SetRealatedSkillLevel(iSkillLevel);
					}
				} break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
#ifdef SERV_9TH_NEW_CHARACTER
			case CX2UnitManager::NUI_PARTICLE_PRISM_START: // ��ƼŬ ������
			case CX2UnitManager::NUI_PARTICLE_PRISM_END:
				{
					float fHPScale = 1.f;
					const CX2SkillTree::SkillTemplet* pSkillTemplet = 
						pUser->GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AN_PARTICLE_PRISM );
					if( NULL != pSkillTemplet )
					{
						if ( NULL == pUser->GetUnit() )
							return;
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_AN_PARTICLE_PRISM ) );

						// ���� ����
						{
							/*fHPScale = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_HP_ABS, iSkillTempletLevel );
							if( 0 <= fHPScale )
							fHPScale = 1.f;

							fStatScale = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
							fHPScale *= fHyperModeStatScale;
							fStatScale *= fHyperModeStatScale;
							float fNpcHp = pUser->GetUnitLevel() * fHPScale;
							if( GetGameType() == GT_PVP )
							{
								fNpcHp = pNPC->GetStat().GetStat().m_fBaseHP;
							}	*/

							const float fHPRate = 0.25f;	/// ������ ü���� 25%

							pNPC->SetNPCStat( 
								pUser->GetMaxHp() * fHPRate,	
								npcStat.m_fAtkPhysic	* fStatScale	* 1.f,
								npcStat.m_fAtkMagic		* fStatScale	* 1.f,
								pNPC->GetStat().GetStat().m_fDefPhysic	* 1.f,
								pNPC->GetStat().GetStat().m_fDefMagic		* 1.f );
						}


						// ���� ���� �ð� ����
						{
							const float fNpcDurationTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
							pNPC->StartSelfDestruction( fNpcDurationTime );


							if( NULL != pNPC->GetOwnerGameUnit() &&
								CX2Unit::UT_ADD == pNPC->GetOwnerGameUnit()->GetUnitType() )
							{
								CX2GUAdd* pGUAdd = static_cast<CX2GUAdd*>(pNPC->GetOwnerGameUnit());
								if( NULL != pGUAdd )
								{
									// ����Ʈ �����ð� ����
									pNPC->PushEffectSetToDeleteOnDie(pGUAdd->GetParticlePrismHandle());
								}
							}
						}

					}
				} break;
#endif // SERV_9TH_NEW_CHARACTER
			}
		}
	}
}


//void CX2Game::OpenMissionOpening( bool bOpen )
//{
//	if( true == bOpen )
//	{
//		if( NULL == m_pDLGMissionOpening )
//		{
//			m_pDLGMissionOpening		= new CKTDGUIDialog( this, L"DLG_Mission_Opening_Text.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMissionOpening );
//		}
//
//		m_pDLGMissionOpening->SetShowEnable( true, true );
//
//		m_pDLGMissionOpening->Move( D3DXVECTOR2( -1024, 0 ), D3DXCOLOR(1,1,1,1), 0.01f, true );
//		m_pDLGMissionOpening->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR(1,1,1,1), 1.f, false );
//		m_pDLGMissionOpening->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR(1,1,1,1), 5.f, false );
//		m_pDLGMissionOpening->Move( D3DXVECTOR2( 1024, 0 ), D3DXCOLOR(1,1,1,1), 1.f, false );
//	}
//	else
//	{
//		if( NULL != m_pDLGMissionOpening )
//		{
//			m_pDLGMissionOpening->SetShowEnable( false, false );
//		}
//	}
//}
//
//void CX2Game::UpdateMissionOpening( const WCHAR* wszTitle, const WCHAR* wszReward )
//{
//	if( NULL == m_pDLGMissionOpening )
//		return;
//
//	CKTDGUIStatic* pStatic_Title = (CKTDGUIStatic*) m_pDLGMissionOpening->GetControl( L"StaticMission_Title" );
//	CKTDGUIStatic* pStatic_Reward = (CKTDGUIStatic*) m_pDLGMissionOpening->GetControl( L"StaticMission_Compensation" );
//
//
//	if( NULL != pStatic_Title )
//	{
//		if( NULL != wszTitle )
//		{
//			pStatic_Title->GetString(0)->msg = wszTitle;
//		}
//		else
//		{
//			pStatic_Title->GetString(0)->msg = L"";
//		}
//	}
//
//
//	if( NULL != pStatic_Reward )
//	{
//		if( NULL != wszReward )
//		{
//			pStatic_Reward->GetString(0)->msg = wszReward;
//		}
//		else
//		{
//			pStatic_Reward->GetString(0)->msg = L"";
//		}
//	}
//	
//}
//
//
//
//void CX2Game::OpenMissionProgress( bool bOpen )
//{
//	if( true == bOpen )
//	{
//		if( NULL == m_pDLGMissionProgress )
//		{
//			m_pDLGMissionProgress		= new CKTDGUIDialog( this, L"DLG_Mission_Opening_Text.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMissionProgress );
//		}
//
//		m_pDLGMissionProgress->SetShowEnable( true, true );
//
//		if( NULL != m_pMiniMap )
//		{
//			if( true == m_pMiniMap->GetMinimized() )
//			{
//				m_pDLGMissionProgress->SetPos( D3DXVECTOR2(-110, 0) );
//			}
//			else
//			{
//				m_pDLGMissionProgress->SetPos( D3DXVECTOR2(0, 0) );
//			}
//		}
//		else
//		{
//			m_pDLGMissionProgress->SetPos( D3DXVECTOR2( 0, 0 ) ); // ���� ������ 
//		}
//	}
//	else
//	{
//		if( NULL != m_pDLGMissionProgress )
//		{
//			m_pDLGMissionProgress->SetShowEnable( false, false );
//		}
//	}
//}
//
//
//void CX2Game::UpdateMissionProgress( bool bComplete, const WCHAR* wszTitle, const WCHAR* wszReward )
//{
//	if( NULL == m_pDLGMissionProgress )
//		return;
//
//	CKTDGUIStatic* pStatic_Title_OneLine = (CKTDGUIStatic*) m_pDLGMissionProgress->GetControl( L"StaticMission_Title_OneLine" );
//	CKTDGUIStatic* pStatic_Title_TwoLine = (CKTDGUIStatic*) m_pDLGMissionProgress->GetControl( L"StaticMission_Title_TwoLine" );
//	CKTDGUIStatic* pStatic_Reward = (CKTDGUIStatic*) m_pDLGMissionProgress->GetControl( L"StaticCompensation" );
//
//	CKTDGUIStatic* pStatic_Success = (CKTDGUIStatic*) m_pDLGMissionProgress->GetControl( L"StaticMission_Clear" );
//	CKTDGUIStatic* pStatic_Fail = (CKTDGUIStatic*) m_pDLGMissionProgress->GetControl( L"StaticMission_Failure" );
//
//
//
//	// fix!! �ϴ� �Ⱥ��̰� 
//	if( NULL != pStatic_Title_TwoLine )
//	{
//		pStatic_Title_TwoLine->SetShowEnable( false, false );
//	}
//
//	if( NULL != pStatic_Title_OneLine )
//	{
//		pStatic_Title_OneLine->SetShowEnable( true, true );
//		if( NULL != wszTitle )
//		{
//			pStatic_Title_OneLine->GetString(0)->msg = wszTitle;
//		}
//	}
//
//	if( NULL != pStatic_Reward )
//	{
//		if( NULL != wszReward )
//		{
//			pStatic_Reward->GetString(0)->msg = wszReward;
//		}
//	}
//	
//	if( true == bComplete )
//	{
//		if( NULL != pStatic_Success )
//		{
//			pStatic_Success->SetShowEnable( true, true );
//		}
//
//		if( NULL != pStatic_Fail )
//		{
//			pStatic_Fail->SetShowEnable( false, false );
//		}
//	}
//	else
//	{
//		if( NULL != pStatic_Success )
//		{
//			pStatic_Success->SetShowEnable( false, false );
//		}
//
//		if( NULL != pStatic_Fail )
//		{
//			pStatic_Fail->SetShowEnable( true, true );
//		}
//	}
//}


// ������ ���Ϳ��� ����ȴ�. 
void CX2Game::BuffAllNPC( int statType, float fStatVal, float fStatTime )
{
	if( false == IsHost() )
		return;



	for( int i=0; i<GetNPCUnitListSize(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = GetNPCUnit(i);
		if( NULL == pCX2GUNPC )
			continue;

		if( pCX2GUNPC->GetNowHp() <= 0.f ||
			pCX2GUNPC->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
		{
			continue;
		}

		// �̺��� ��ȯ���Ϳ� ���� �츮�� NPC���Դ� ������� �ʴ´�. 
		if( NULL != pCX2GUNPC->GetNPCAI() &&
			CX2NPCAI::NAT_ALLY == pCX2GUNPC->GetNPCAI()->GetAIType() )
		{
			continue;
		}


		//if( NULL != pCX2GUNPC->GetNPCTemplet() )
		{
			switch( pCX2GUNPC->GetNPCTemplet().m_ClassType )
			{
			case CX2UnitManager::NCT_THING_GATE:
			case CX2UnitManager::NCT_THING_BOX:
			case CX2UnitManager::NCT_THING_TRAP:
			case CX2UnitManager::NCT_THING_DEVICE:
			case CX2UnitManager::NCT_THING_HOUSE:
			case CX2UnitManager::NCT_THING_WORLD_BLOCK:
			case CX2UnitManager::NCT_THING_CHECKER:
			//{{ JHKang / ������ / 2010/12/07 / ���� �� ����� ���� �ʴ� NPC TYPE
#ifdef NEW_SKILL_2010_11
			case CX2UnitManager::NCT_THING_NOBUFF:		/// ����/������� ������ ���� ����()
#endif NEW_SKILL_2010_11
			//}} JHKang / ������ / 2010/12/07 / ���� �� ����� ���� �ʴ� NPC TYPE
			{
					continue;
				} break;

			default:
				{
					// nothing
				} break;
			}
		}





		pCX2GUNPC->AddOnStatReq( (KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT) statType, fStatVal, fStatTime );


		if( fStatVal > 0.f )
		{
			switch( (KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT) statType )
			{
			case KEGS_ADD_ON_STAT_REQ::AOS_ATK_PHYSIC:
				{
					D3DXVECTOR3 mypos = pCX2GUNPC->GetPos();
					mypos.y += 100.0f;
					mypos.z -= 20.f;
					CKTDGParticleSystem::CParticleEventSequence* pBuffPart = GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pCX2GUNPC ),  L"BuffAttackCore", mypos );
					pBuffPart->SetBlackHolePosition( mypos );

				} break;
			case KEGS_ADD_ON_STAT_REQ::AOS_ATK_MAGIC:
				{
					D3DXVECTOR3 mypos = pCX2GUNPC->GetPos();
					mypos.y += 100.0f;
					mypos.z -= 20.f;
					CKTDGParticleSystem::CParticleEventSequence* pBuffPart = GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pCX2GUNPC ),  L"BuffMagickCore", mypos );
					pBuffPart->SetBlackHolePosition( mypos );

				} break;

			case KEGS_ADD_ON_STAT_REQ::AOS_DEF_PHYSIC:
			case KEGS_ADD_ON_STAT_REQ::AOS_DEF_MAGIC:
				{
					D3DXVECTOR3 mypos = pCX2GUNPC->GetPos();
					mypos.y += 100.0f;
					mypos.z -= 20.f;
					CKTDGParticleSystem::CParticleEventSequence* pBuffPart = GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pCX2GUNPC ),  L"ShieldBuffCore", mypos );
					pBuffPart->SetBlackHolePosition( mypos );

				} break;

			default:
				{
					// nothing
				} break;
			}
		}
	}
}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//void CX2Game::BroadCast_XPT_UNIT_NPC_SYNC_PACK()
//{
//	KSerBuffer buff;
//	Serialize( &buff, &m_kXPT_UNIT_NPC_SYNC_PACK );
//	g_pData->GetGameUDP()->BroadCast( m_vecUserUIDforSyncPacket, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
//
//	m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.resize(0);
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
	void CX2Game::EnableManualCameraPosition( bool bEnabled )
	{
		if( NULL == GetX2Camera() )
			return; 


		GetX2Camera()->SetEnabledManualCameraPosition( bEnabled );
		if( true == bEnabled )
		{
			if( true == m_bFreeCamera )
			{
				GetX2Camera()->CalcManualCameraPosition( GetFocusUnit(), *m_FPSCamera.GetEyePt() );
			}
			else
			{
				GetX2Camera()->CalcManualCameraPosition( GetFocusUnit(), GetX2Camera()->GetCamera().GetEye() );
			}
		}
	}

#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST





#ifdef REVERSE_GRAVITY_TEST



	void CX2Game::ReverseGravityFrameMove( double fTime, float fElapsedTime )
	{
		if( true == m_vecReverseGravityData.empty() )
			return;

		for( int i = 0; i<(int)m_vecReverseGravityData.size(); i++ )
		{
			ReverseGravityData& reverseGravity = m_vecReverseGravityData[i];
			reverseGravity.OnFrameMove( fTime, fElapsedTime );
			if( reverseGravity.m_fTimeLeft <= 0.f )
			{
				m_vecReverseGravityData.erase( m_vecReverseGravityData.begin() + i );
				--i;
				continue;
			}
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    bool CX2Game::IsInReverseGravityRegion( float fElapsedTime, const D3DXVECTOR3& vPos, float& fSpeedYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	bool CX2Game::IsInReverseGravityRegion( const D3DXVECTOR3& vPos, float& fSpeedYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		fSpeedYDelta = 0.f;

		BOOST_TEST_FOREACH( ReverseGravityData&, reverseGravity, m_vecReverseGravityData )
		{
			if( reverseGravity.m_fTimeLeft <= 0.f )
				continue;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            reverseGravity.IsInRange( fElapsedTime, vPos, fSpeedYDelta, vSpeedRotateDelta, iTeamNumber );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			reverseGravity.IsInRange( vPos, fSpeedYDelta, vSpeedRotateDelta, iTeamNumber );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}

		if( fSpeedYDelta > 0.f )
			return true;
		else 
			return false;
	}

	void CX2Game::AddReverseGravityRegion( const D3DXVECTOR3& vPos, const float fRange, const float fTime, const float fGravity, int iTeamNumber )
	{
		ASSERT( fGravity >= 0.f );

		ReverseGravityData reverseGravity;
		reverseGravity.m_vCenterPos = vPos;
		reverseGravity.m_fRange = fRange;
		reverseGravity.m_fTimeLeft = fTime;
		reverseGravity.m_fGravity = fGravity;
		reverseGravity.m_iTeamNumber = iTeamNumber;

		m_vecReverseGravityData.push_back( reverseGravity );
	}



	bool CX2Game::ReverseGravityData::IsInRange( 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        float fElapsedTime,
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const D3DXVECTOR3& vPos, float& fSpeeYDelta, D3DXVECTOR3& vSpeedRotateDelta, int iTeamNumber )
	{
		if( m_fTimeLeft <= 0.f )
			return false;

		if( iTeamNumber == m_iTeamNumber )
			return false;

		if( g_pKTDXApp->GetCollision()->LinePointDistance( vPos, m_vCenterPos, D3DXVECTOR3(0, 1, 0 ) ) > m_fRange )
		//if( GetDistance( vPos, m_vCenterPos ) > m_fRange )
			return false;

		// oasis907 : ����� [2011.6.29] 
		vSpeedRotateDelta.y = vSpeedRotateDelta.y * 0.995f; 

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        fSpeeYDelta += m_fGravity * fElapsedTime;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		fSpeeYDelta += m_fGravity * g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		return true;
	}

#endif REVERSE_GRAVITY_TEST




//////////////////////////////////////////////////////////////////////////
CX2Game::InfoTextManager::InfoTextManager()
{
	m_iMaxLine = 4;
	m_vPos = D3DXVECTOR2( 512, 270 );
}

CX2Game::InfoTextManager::~InfoTextManager()
{
	ClearAllText();
}

void CX2Game::InfoTextManager::ClearAllText()
{
	for( UINT i=0; i<m_vecpText.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecpText[i];
		g_pMain->GetTextManager()->DeleteText( pText );
	}
	m_vecpText.clear();
}

void CX2Game::InfoTextManager::RemoveText( CX2TextManager::CX2Text* pText )
{
	for( UINT i=0; i<m_vecpText.size(); i++ )
	{
		CX2TextManager::CX2Text* pTempText = m_vecpText[i];
		if( pText == pTempText )
		{
			m_vecpText.erase( m_vecpText.begin() + i );
			g_pMain->GetTextManager()->DeleteText( pText );
			break;
		}
	}
}

void CX2Game::InfoTextManager::MakeEmptyLine( bool bScrollUpside, int nCount )
{
	for( UINT i=0; i<m_vecpText.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecpText[i];
		if( false == g_pMain->GetTextManager()->CheckText( pText ) )
		{
			m_vecpText.erase( m_vecpText.begin() + i );
			i--;
		}
	}

	//if( (int)m_vecpText.size() < m_iMaxLine )
	//	return;

	int iCountToRemove = nCount - ( m_iMaxLine - (int)m_vecpText.size() );
	while( m_vecpText.size() > 0 && iCountToRemove > 0)
	{
		CX2TextManager::CX2Text* pText = m_vecpText[0];
		g_pMain->GetTextManager()->DeleteText( pText );
		m_vecpText.erase( m_vecpText.begin() );
		iCountToRemove -= 1;
	}


	// �����ִ� �ؽ�Ʈ�� �������� ��ũ��
	D3DXVECTOR2 vPos = m_vPos;
	for( UINT i=0; i<m_vecpText.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecpText[i];
		float MAGIC_REMAIN_TIME = 3.f + (float) i / 4.f;
		//float fRemainTime = pText->GetChangeTime() - pText->GetAnimateTime();
		//if( fRemainTime < 0.f )
		//{
		//	fRemainTime = 0.1f;
		//}

		if( true == bScrollUpside )
		{
			vPos.y += pText->GetFont()->GetHeight() * 1.2f * pText->GetLineCount();
		}
		else
		{
			vPos.y -= pText->GetFont()->GetHeight() * 1.2f * pText->GetLineCount();
		}

		pText->Move( vPos, pText->GetTextMsg().c_str(), pText->GetGoalColor(), pText->GetGoalOutlineColor(), 0.01f, true, false );
		pText->Move( vPos, pText->GetTextMsg().c_str(), pText->GetGoalColor(), pText->GetGoalOutlineColor(), MAGIC_REMAIN_TIME, false, true );
	}
}

CX2TextManager::CX2Text* CX2Game::InfoTextManager::PushText( int fontIndex, const WCHAR* pTextMsg, D3DXCOLOR color, D3DXCOLOR outlineColor, 
															DWORD dtType /*= DT_LEFT*/, float fLineSpace /*= 1.0f*/, float fLifeTime /*= 1.0f*/ )
{
	bool bScrollUpside = true;
	MakeEmptyLine( bScrollUpside, 1 );

	D3DXVECTOR2 vPos = m_vPos;
	if( m_vecpText.size() > 0 )
	{
		CX2TextManager::CX2Text* pText = m_vecpText[ m_vecpText.size()-1 ];
		vPos = pText->GetGoalPos();

		if( true == bScrollUpside )
		{
			vPos.y += pText->GetFont()->GetHeight() * 1.2f * pText->GetLineCount();
		}
		//else
		//{
		//	vPos.y -= pText->GetFont()->GetHeight() * 1.2f;
		//}
	}

	CX2TextManager::CX2Text* pText = g_pMain->GetTextManager()->AddText( fontIndex, vPos, pTextMsg, color, outlineColor, dtType, fLineSpace );
	if( false == bScrollUpside )
	{
		vPos.y -= pText->GetFont()->GetHeight() * 1.2f * pText->GetLineCount();
		pText->SetPos( vPos );
	}
	pText->Move( vPos, pTextMsg, color, outlineColor, fLifeTime, false, true );
	m_vecpText.push_back( pText );

	return pText;
}
#ifdef DUNGEON_ITEM
#ifdef 	ADD_VELDER_ACADEMY_EVENT_MONSTER		/// ���� �� ��ȯ���� ����
bool
#else	ADD_VELDER_ACADEMY_EVENT_MONSTER
void
#endif	ADD_VELDER_ACADEMY_EVENT_MONSTER


CX2Game::ProcSpecialItem(
    const CX2Item::ItemTemplet* pItemTemplet, 
    CX2GUUser* pCX2GUUser)
{	
	if( pItemTemplet == NULL )
	{
#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
		return false;
#else  ADD_VELDER_ACADEMY_EVENT_MONSTER
		return;
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER		
	}


    switch( pItemTemplet->GetItemID() )
	{	
	case DUNGEON_ITEM4_1:	// ���� ��ȯ( �ĸ��� �̺�Ʈ )
		{
			if( IsHost() == true )
			{
				for( int i=0; i<GetNPCUnitListSize(); i++ )
				{
					CX2GUNPC* pCX2GUNPC = GetNPCUnit(i);
					if( pCX2GUNPC != NULL && 
						( pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_TREE_KNIGHT_EVENT ||
						pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_ENT_SMALL_EVENT ||
						pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_ENT_EVENT ) )
					{					
						pCX2GUNPC->SetNowHp(0.f);

					}
				}
			}				

			if( pCX2GUUser != NULL && GetWorld() != NULL && GetWorld()->GetLineMap() != NULL )
			{
				const D3DXVECTOR3& vPos = pCX2GUUser->GetPos();
				D3DXVECTOR3 vPos1 = GetWorld()->GetLineMap()->GetRandomPosition( &vPos, 500.f, true );
				D3DXVECTOR3 vPos2 = GetWorld()->GetLineMap()->GetRandomPosition( &vPos, 500.f, true );
				D3DXVECTOR3 vPos3 = GetWorld()->GetLineMap()->GetRandomPosition( &vPos, 500.f, true );
				D3DXVECTOR3 vPos4 = GetWorld()->GetLineMap()->GetRandomPosition( &vPos, 300.f, true );
				D3DXVECTOR3 vPos5 = GetWorld()->GetLineMap()->GetRandomPosition( &vPos, 200.f, true );

				SummonMonster(pCX2GUUser, CX2UnitManager::NUI_TREE_KNIGHT_EVENT, vPos1 );
				SummonMonster(pCX2GUUser, CX2UnitManager::NUI_TREE_KNIGHT_EVENT, vPos2 );
				SummonMonster(pCX2GUUser, CX2UnitManager::NUI_TREE_KNIGHT_EVENT, vPos3 );				
				SummonMonster(pCX2GUUser, CX2UnitManager::NUI_ENT_SMALL_EVENT, vPos4 );
				SummonMonster(pCX2GUUser, CX2UnitManager::NUI_ENT_EVENT, vPos5 );				
			}
#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
			return true;
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
		}
		break;
#ifdef EVENT_DUNGEON_ITEM
	case DUNGEON_ITEM4_2:	// ���� ������
		if( GetGameType() == GT_DUNGEON )
		{
			CreateNPCReq( CX2UnitManager::NUI_DEVELOPER_A, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER );

		#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
			return true;
		#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
		}
		break;
	case DUNGEON_ITEM4_3:	// ��ȹ ������
		if( GetGameType() == GT_DUNGEON )
		{
			CreateNPCReq( CX2UnitManager::NUI_DEVELOPER_B, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER );

		#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
			return true;
		#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
		}
		break;
	case DUNGEON_ITEM4_4:	// ���α׷� ������
		if( GetGameType() == GT_DUNGEON )
		{
			CreateNPCReq( CX2UnitManager::NUI_DEVELOPER_C, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER );

		#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
			return true;
		#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
		}
		break;
	case DUNGEON_ITEM4_5:	// ������ ������
		if( GetGameType() == GT_DUNGEON )
		{
			CreateNPCReq( CX2UnitManager::NUI_DEVELOPER_D, 1, false, m_vDropItemPos, 
				true, 0.f, false, -1, CX2Room::TN_MONSTER );

		#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
			return true;
		#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
		}
		break;
	case DUNGEON_ITEM4_6:	// ��Ƽ��Ʈ ������
		if( GetGameType() == GT_DUNGEON )
		{
			CreateNPCReq( CX2UnitManager::NUI_DEVELOPER_E, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER );

		#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
			return true;
		#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
		}
		break;
#endif

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER		/// ���� ��ī���� ���б� �̺�Ʈ
	case DUNGEON_ITEM5_1:	/// ���� ������ �ϰŽ�
		if( GetGameType() == GT_DUNGEON || GetGameType() == GT_BATTLE_FIELD )
		{
			CreateNPCReq( CX2UnitManager::NUI_EVENT_TEACHER_HAGERS, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER, CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID() );

			return true;
		}
		break;
	case DUNGEON_ITEM5_2:	/// �л����� ������ �̸ֹ�Ʈ
		if( GetGameType() == GT_DUNGEON || GetGameType() == GT_BATTLE_FIELD )
		{
			CreateNPCReq( CX2UnitManager::NUI_EVENT_TEACHER_EMIRATE, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER, CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID() );

			return true;
		}
		break;
	case DUNGEON_ITEM5_3:	/// �ױ۸��� Ƽó ��ġ��
		if( GetGameType() == GT_DUNGEON || GetGameType() == GT_BATTLE_FIELD )
		{
			CreateNPCReq( CX2UnitManager::NUI_EVENT_TEACHER_RICHANG, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER, CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID() );

			return true;
		}
		break;
	case DUNGEON_ITEM5_4:	/// ���� ������ ����
		if( GetGameType() == GT_DUNGEON || GetGameType() == GT_BATTLE_FIELD )
		{
			CreateNPCReq( CX2UnitManager::NUI_EVENT_TEACHER_ECHO, 1, false, m_vDropItemPos, 
				true, 0.f, false, -1, CX2Room::TN_MONSTER, CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID() );

			return true;
		}
		break;
	case DUNGEON_ITEM5_5:	/// ���� ������ ȣ����
		if( GetGameType() == GT_DUNGEON || GetGameType() == GT_BATTLE_FIELD )
		{
			CreateNPCReq( CX2UnitManager::NUI_EVENT_TEACHER_HOFMANN, 1, false, m_vDropItemPos, 
				true, 0.f, true, -1, CX2Room::TN_MONSTER, CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID() );

			return true;
		}
		break;
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER

	default:
		break;
	}

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
	return false;
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER
}

CX2Item::SPECIAL_ITEM_TYPE CX2Game::GetSpecialItemGroupType(const CX2Item::ItemTemplet* pItemTemplet)
{
	if( pItemTemplet == NULL )
		return CX2Item::SIT_NONE;
    DWORD   dwItemID = pItemTemplet->GetItemID();

	bool bNostrum = false;
	bool bSpirit = false;
	bool bCrest = false;

	// ���� ��ȯ
	for(int i=0; i<ARRAY_SIZE(DUNGEON_ITEM_LIST4); ++i)
	{
		if( dwItemID == DUNGEON_ITEM_LIST4[i] )
		{		
			return CX2Item::SIT_SUMMON_MONSTER;
		}
	}

	// ���
	for(int i=0; i<ARRAY_SIZE(DUNGEON_ITEM_LIST1); ++i)
	{
		if( dwItemID == DUNGEON_ITEM_LIST1[i] )
		{
			bNostrum = true;			
			return CX2Item::SIT_NOSTRUM;
		}
	}

	// ���ɼ�ȯ
	if(bNostrum == false)
	{
		for(int i=0; i<ARRAY_SIZE(DUNGEON_ITEM_LIST2); ++i)
		{
			if( dwItemID == DUNGEON_ITEM_LIST2[i] )
			{
				bSpirit = true;
				return CX2Item::SIT_GENIUS;
			}
		}
	}

	// ����
	if(bNostrum == false && bSpirit == false)
	{
		for(int i=0; i<ARRAY_SIZE(DUNGEON_ITEM_LIST3); ++i)
		{
			if( dwItemID == DUNGEON_ITEM_LIST3[i] )
			{
				bCrest = true;
				return CX2Item::SIT_CREST;
			}
		}
	}	

	return CX2Item::SIT_NONE;
}

wstring CX2Game::SummonSpiritEffectName(int iSpirit)
{
	wstring wstrSpiritEffectSet = L"";

	switch(iSpirit)
	{
	case 1:
		wstrSpiritEffectSet = L"EffectSet_Spirit_1";	// ���������� �г�
		break;
	case 2:
		wstrSpiritEffectSet = L"EffectSet_Spirit_2"; // ����̾���� �г�
		break;
	case 3:
		wstrSpiritEffectSet = L"EffectSet_Spirit_3";		// ����� �г�
		break;
	case 4:
		wstrSpiritEffectSet = L"EffectSet_Spirit_4";	// �ϸ��Ǿ��� �ձ�
		break;
	default:
		wstrSpiritEffectSet = L"";
		break;
	}

	return wstrSpiritEffectSet;
}

void CX2Game::ClearSpirit()
{
	for(UINT i=0; i<m_vechEffectSetSpirit.size(); ++i)
	{
		CX2EffectSet::Handle hHandle = m_vechEffectSetSpirit[i];
		if( hHandle != INVALID_EFFECTSET_HANDLE )
			GetEffectSet()->StopEffectSet( hHandle );
	}

	m_vechEffectSetSpirit.clear();

	m_optrItemDamageEffectGameUnit.Reset();
	m_fItemDamageEffectTime = 0.f;
	m_iItemDamageEffectIndex = 0;
	m_vecEffectPos.clear();
}

void CX2Game::SideEffectGenius(CX2GUUser* pCX2GUUser)
{
	if( pCX2GUUser == NULL )
		return;

	GetEffectSet()->PlayEffectSet( L"Effect_Marker_SideEffect_Posion01", (CX2GameUnit*) pCX2GUUser );

	// �Ƿ� ��ȯ
	int iTeam = pCX2GUUser->GetTeam();

	int iNpcLv = pCX2GUUser->GetUnit()->GetUnitData().m_Level;
	
	if( GetGameType() == GT_DUNGEON )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
		if( NULL != pDungeonGame &&
			NULL != pDungeonGame->GetDungeon() &&
			NULL != pDungeonGame->GetDungeon()->GetDungeonData() )
		{
			const SEnum::DUNGEON_ID eDungeonId = pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID;

			//{{ oasis907 : ����� [2010.10.21] // �̺�Ʈ ���������� ���̵� ����Ʈ ��ȯ �� ���� 1��
			if ( CX2Dungeon::IsEventDungeon( eDungeonId ) )
				iNpcLv = 1;
			//}} oasis907 : ����� [2010.10.21] // 	�̺�Ʈ ���������� ���̵� ����Ʈ ��ȯ �� ���� 1��

			CreateNPCReq( CX2UnitManager::NUI_PPORU_WILLIAM_GENIUS, iNpcLv, false, pCX2GUUser->GetPos(), 
				true, 0.f, true, -1, CX2Room::TN_MONSTER );
		}
	}
	else
	{
		if( g_pData->GetPVPRoom() != NULL && g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
		{
			//�����̹� ��忡���� ���� ������ ����
			CreateNPCReq( CX2UnitManager::NUI_PPORU_WILLIAM_GENIUS, iNpcLv, false, pCX2GUUser->GetPos(), 
				true, 0.f, true, -1, CX2Room::TN_MONSTER );
		}
		else
		{
			if(iTeam == 0)
				iTeam = 1; // ��� ��
			else if(iTeam == 1)
				iTeam = 0; // ���� ��

			CreateNPCReq( CX2UnitManager::NUI_PPORU_WILLIAM_GENIUS, iNpcLv, false, pCX2GUUser->GetPos(), 
				true, 0.f, true, -1, (CX2Room::TEAM_NUM)iTeam, CX2NPCAI::NAT_ALLY );
		}		
	}	

	pCX2GUUser->PlaySound( L"Summon_Fail.ogg" );
	
}

void CX2Game::SummonSpiritEffect(int iEffectIndex)
{	
	if( null == m_optrItemDamageEffectGameUnit )
		return;

	if( m_optrItemDamageEffectGameUnit->GetNowHp() > 0.f)
	{
		D3DXVECTOR3 vPosEffectUser = m_optrItemDamageEffectGameUnit->GetPos();

		for( UINT i=0; i<m_UserUnitList.size(); i++ )
		{
			CX2GUUser* pUser = m_UserUnitList[i];
			if( pUser == NULL )
				continue;
			
			if( pUser->GetInvincible() == true || pUser->GetTeam() == m_optrItemDamageEffectGameUnit->GetTeam() )
				continue;			

			D3DXVECTOR3 vPos = pUser->GetPos();

			if( GetDistance( vPos, vPosEffectUser ) > 1500.f )
				continue;

			m_vecEffectPos.push_back(vPos);
		}

		for( UINT i=0; i<m_NPCUnitList.size(); i++ )
		{
			CX2GUNPC* pNPC = m_NPCUnitList[i];
			if( pNPC == NULL )
				continue;

			//if( pNPC->GetNPCTemplet() == NULL )
			//	continue;

			if( pNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
				continue;
			if( pNPC->GetInvincible() == true || pNPC->GetTeam() == m_optrItemDamageEffectGameUnit->GetTeam() )
				continue;

			D3DXVECTOR3 vPos = pNPC->GetPos();

			if( GetDistance( vPos, vPosEffectUser ) > 1500.f )
				continue;

			m_vecEffectPos.push_back(vPos);
		}
	}

	float fPowerRate = m_optrItemDamageEffectGameUnit->GetPowerRate();
	D3DXVECTOR3 vRotate = m_optrItemDamageEffectGameUnit->GetRotateDegree();

	wstring wstrEffectName = L"";	
	if(iEffectIndex == 1)
	{
		wstrEffectName = L"SPIRIT_1_ATTACK";
	}
	else if(iEffectIndex == 3)
	{
		wstrEffectName = L"SPIRIT_3_ATTACK";
	}

	for(UINT i=0; i<m_vecEffectPos.size(); ++i)
	{		
		D3DXVECTOR3 vPos = m_vecEffectPos[i];		
		GetDamageEffect()->CreateInstance( static_cast<CX2GameUnit*>( m_optrItemDamageEffectGameUnit.GetObservable() ), wstrEffectName.c_str(), fPowerRate, vPos, vRotate, vRotate );

	}
	
}

#ifdef SPECIAL_USE_ITEM
void CX2Game::SpecialItemEffect(int iEffectIndex)
{	
	if( null == m_optrItemDamageEffectGameUnit )
		return;

	if( m_optrItemDamageEffectGameUnit->GetNowHp() > 0.f )
	{
		D3DXVECTOR3 vPosEffectUser = m_optrItemDamageEffectGameUnit->GetPos();

		for( UINT i=0; i<m_UserUnitList.size(); i++ )
		{
			CX2GUUser* pUser = m_UserUnitList[i];
			if( pUser == NULL )
				continue;

			if( /*pUser->GetInvincible() == true ||*/ pUser->GetTeam() != m_optrItemDamageEffectGameUnit->GetTeam() )// �Ʊ���
				continue;			

			D3DXVECTOR3 vPos = pUser->GetPos();

			// �Ÿ����� ����
			//if( GetDistance( vPos, vPosEffectUser ) > 1500.f )
			//	continue;

			m_vecEffectPos.push_back(vPos);
			wstring wstrEffectName = L"";	
			if(iEffectIndex == CX2Item::SST_WIND_SPHERE)	// �ٶ� ����
			{
				wstrEffectName = L"EffectSet_SI_WindSphere";
			}
			GetEffectSet()->PlayEffectSet( wstrEffectName.c_str(), (CX2GameUnit*) pUser, NULL, false, -1.f, -1.f, D3DXVECTOR3(1.f, 1.f, 1.f) ); 

			// ������ �ɾ��ش�.
			pUser->StartSpecialItemBuffWind();
		}
	}
}
#endif

void CX2Game::SummonMonster(CX2GUUser* pCX2GUUser, CX2UnitManager::NPC_UNIT_ID npcId, D3DXVECTOR3 vPos)
{
	if( pCX2GUUser == NULL )
		return;	

	// ���� ��ȯ
	int iTeam = pCX2GUUser->GetTeam();

	int iNpcLv = 1; //pCX2GUUser->GetUnit()->GetUnitData().m_Level;
	if( GetGameType() == GT_DUNGEON )
	{
		//PushCreateNPCReq( npcId, iNpcLv, false, vPos, true, 1.f, true, -1, (CX2Room::TEAM_NUM)iTeam, CX2NPCAI::NAT_ALLY );
		CreateNPCReq( npcId, iNpcLv, false, vPos, 
			true, 0.1f, true, -1, (CX2Room::TEAM_NUM)iTeam, CX2NPCAI::NAT_ALLY );

		g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_LevelUp_LandPosition", (CX2GameUnit*)pCX2GUUser, NULL, false, -1.f, -1.f, D3DXVECTOR3(1.f, 1.f, 1.f), true, vPos );		
		g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_LevelUp",  (CX2GameUnit*)pCX2GUUser, NULL, false, -1.f, -1.f, D3DXVECTOR3(1.f, 1.f, 1.f), true, vPos );		
	}
}

#endif


#ifdef SERV_WORLD_TRIGGER_RELOCATION
bool CX2Game::Handler_EGS_WORLD_TRIGGER_RELOCATION_REQ( int iTriggerId )
{
	if( GetWorld() != NULL && GetWorld()->CanActiveTrigger(iTriggerId) == false )
		return true;

#if defined(WORLD_TOOL) || defined(X2TOOL)
	GetWorld()->ActiveTrigger( iTriggerId );	
#else
	KEGS_WORLD_TRIGGER_RELOCATION_REQ kEGS_WORLD_TRIGGER_RELOCATION_REQ;
	kEGS_WORLD_TRIGGER_RELOCATION_REQ.m_iWorldTrigger = iTriggerId;

	g_pData->GetServerProtocol()->SendPacket( EGS_WORLD_TRIGGER_RELOCATION_REQ, kEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif
	return true;
}
bool CX2Game::Handler_EGS_WORLD_TRIGGER_RELOCATION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_WORLD_TRIGGER_RELOCATION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		return true;
	}

	return false;
}
bool CX2Game::Handler_EGS_WORLD_TRIGGER_RELOCATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_WORLD_TRIGGER_RELOCATION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( kEvent.m_iWorldTrigger >= 0 && GetWorld() != NULL )
	{		
		GetWorld()->ActiveTrigger(kEvent.m_iWorldTrigger);
	}

	return true;
}
#endif

//{{ ����� : [2011/3/4/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
float CX2Game::GetDungeonProgress()
{
	CX2DungeonGame* pDungeonGame = (CX2DungeonGame*)this;

	if( pDungeonGame == NULL )
	{
		return 0.0f;
	}

	float fTimeLimit = pDungeonGame->GetPlayTimeLimit();
	float fTimeRemain = pDungeonGame->GetRemainPlayTime();

	if( fTimeLimit <= 0 )	return 0.0f;

	float fProgress = 1 - ( fTimeRemain / fTimeLimit );

	if( fProgress > 1.0f ) fProgress = 1.0f;
	else if( fProgress < 0 ) fProgress = 0.0f;

	return fProgress;
}

int	CX2Game::GetNPCUnitNumByNPCID( int iNPCID )
{
	int num = 0;
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		if( m_NPCUnitList[i] != NULL )
		{
			CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
			if( pCX2GUNPC != NULL && pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID == iNPCID )
			{
				num++;
			}
		}
	}

	return num;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} ����� : [2011/3/4/] //	���� �̼�

#ifdef NEW_HENIR_TEST
int CX2Game::GetStartSecretStageEnteringEvent()
{
	if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() )
	{
		CX2StateDungeonGame* pStateDungeonGame = (CX2StateDungeonGame*) g_pMain->GetNowState();
		return pStateDungeonGame->GetStartSecretStageEnteringEvent();
	}
	return 0;
}

bool CX2Game::SecretDungeonStageLoadReq(int iVal)
{
	if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() )
	{
		CX2StateDungeonGame* pStateDungeonGame = (CX2StateDungeonGame*) g_pMain->GetNowState();
		return pStateDungeonGame->SecretDungeonStageLoadReq(iVal);
	}
	return false;
}
#endif NEW_HENIR_TEST

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
bool CX2Game::IsFront( CX2GameUnit *pMyUnit, CX2GameUnit *pTargetUnit )
{
	if( pMyUnit == NULL || pTargetUnit == NULL )
		return false;

	D3DXVECTOR3 vDir = pTargetUnit->GetPos() - pMyUnit->GetPos();
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    vDir.y = 0.f;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
	if( D3DXVec3Dot( &vDir, &pTargetUnit->GetDirVector() ) > 0.f )
	{
		if( false == pMyUnit->GetIsRight() )
			return false;
	}
	else
	{
		if( true == pMyUnit->GetIsRight() )
			return false;
	}

	return true;
}
#endif

void CX2Game::SendNpcUnitFirstSyncPacketImmediateForce( vector<UidType>& vecNonNpcSyncUserList_, OUT KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT& kPacket_ )
{
	// ȣ��Ʈ�� ���
	if( IsHost() == true )
	{
		// ��ü NPC�� ��Ŷ�� ������... (pLastUnit�� ������ �Ǵ°� �ƴѰ�??)
		//KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

		for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
		{
			CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
			if( pCX2GUNPC != NULL )
			{
				pCX2GUNPC->GetBuffInfo( kPacket_.m_vecNpcUnitBuff );
			}
		}

//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//
////#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        CX2FrameUDPPack   kFrameUDPPack;
//        if ( GetMyUnit() != NULL )
//            kFrameUDPPack.SetUnitUID( GetMyUnit()->GetUnitUID() );
//        kFrameUDPPack.SetFrameMoveCount( m_kFrameUDPPack.GetFrameMoveCount() );
//		kFrameUDPPack.ResetFrameUDPPack( true );
//        kFrameUDPPack.UpdateSyncUserListAndUDPMaxSize( vecNonNpcSyncUserList_ );
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//		for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
//		{
//			CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
//			if( pCX2GUNPC != NULL )
//			{
////#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				pCX2GUNPC->SendPacketImmediateForce( kFrameUDPPack );
////#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////				pCX2GUNPC->SendPacketImmediateForce( m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				pCX2GUNPC->GetBuffInfo( kPacket_.m_vecNpcUnitBuff );
//			}
//
////#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////			if ( sizeof(KXPT_UNIT_NPC_SYNC) * (m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() + 1) > 500 )
////			{
////				KSerBuffer buff;
////				Serialize( &buff, &m_kXPT_UNIT_NPC_SYNC_PACK );
////				g_pData->GetGameUDP()->BroadCast( vecNonNpcSyncUserList_, XPT_UNIT_NPC_FIRST_SYNC_PACK_BY_BATTLE_FIELD, (char*)buff.GetData(), buff.GetLength() );
////
////				m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.resize(0);
////			}
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		}
//
////#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        kFrameUDPPack.FlushFrameUDPPack();
////#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////		KSerBuffer buff;
////		Serialize( &buff, &m_kXPT_UNIT_NPC_SYNC_PACK );
////		g_pData->GetGameUDP()->BroadCast( vecNonNpcSyncUserList_, XPT_UNIT_NPC_FIRST_SYNC_PACK_BY_BATTLE_FIELD, (char*)buff.GetData(), buff.GetLength() );
////		m_kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.resize(0);
////#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
	}
}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void CX2Game::SetPetFirstSyncPacketImmediateForce( vector<UidType>& vecNonNpcSyncUserList_ )
//{
//	// ȣ��Ʈ�� ���
//	if( IsHost() == true )
//	{
//#ifdef SERV_PET_SYSTEM			
//		for( UINT i=0; i<g_pData->GetPetManager()->GetPetNum(); ++i )
//		{
//			CX2PET *pPet = g_pData->GetPetManager()->GetPetInx(i);
//			if( pPet != NULL )
//			{						
//				pPet->SetSendReserveStateChange( true );
//				pPet->SendPacketImmediateForce( m_kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList );
//			}
//
//			if( m_kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList.size() > 0 )
//			{
//				KSerBuffer buff;
//				Serialize( &buff, &m_kXPT_UNIT_PET_SYNC_PACK );
//				g_pData->GetGameUDP()->BroadCast( vecNonNpcSyncUserList_, XPT_UNIT_PET_FIRST_SYNC_PACK_BY_BATTLE_FIELD, (char*)buff.GetData(), buff.GetLength() );
//
//				m_kXPT_UNIT_PET_SYNC_PACK.unitPetSyncList.resize(0);
//			}
//		}			
//#endif
//
//	}
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef ADDITIONAL_MEMO
CX2GameUnit* CX2Game::GetNearestUnit( CX2GameUnit* pGameUnit, int iFlag )
{
	// iFlag 0 : ������
	// iFlag 1 : ���͸�
	// iFlag 2 : ��ü���ִ��
	float fDist = 10000.f * 10000.f;
	CX2GameUnit *pNearestUnit = NULL;

	CX2GameUnit *pMyUnit = pGameUnit;
	
	if( pGameUnit == NULL )
	{	
		if ( NULL != GetMyUnit() )
			pMyUnit = GetMyUnit();
		else
			return pNearestUnit;
	}

	int iMyTeam = pMyUnit->GetTeam();
#ifdef SERV_TRAPPING_RANGER_TEST
	if( iMyTeam < 0 )
		iMyTeam = pMyUnit->GetAllyTeam();
#endif
	
	for(UINT i=0; i<m_UnitList.size(); ++i)
	{
		CX2GameUnit *pUnit = m_UnitList[i];

		if( pUnit->GetTeam() == iMyTeam )
			continue;
		if( pUnit->GetNowHp() <= 0.f )
			continue;
		if( pUnit->GetTeam() < 0 )
			continue;
		if( pUnit->GetUnitUID() == pMyUnit->GetUnitUID() )
			continue;

		if( pUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
		{
			if( iFlag == 0 )
				continue;

			CX2GUNPC *pNpc = (CX2GUNPC*)pUnit;
			if( pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC && 
				pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_THING_GATE )
				continue;

			if( pNpc->GetInvincible() == true )
				continue;
		}
		else if( iFlag == 1 )
			continue;

		D3DXVECTOR3 vDist = pMyUnit->GetPos() - pUnit->GetPos();
		float fLen = D3DXVec3LengthSq( &vDist );
		if( fLen < fDist )
		{
			pNearestUnit = pUnit;
			fDist = fLen;
		}
	}

	return pNearestUnit;
}

#endif

void CX2Game::ShowDangerAlarm_LUA( int iAlarmId, float fShowTime, ALARM_COLOR_TYPE eColor, bool bDanger, int iStringIndex )
{
	if( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		return;

	int iAlarmIndex = GetDangerAlarm( -1, iAlarmId );
	if( iAlarmIndex < 0 )
	{
		DangerAlarm *pAlarm = new DangerAlarm();
		pAlarm->m_uid = -1;
		pAlarm->m_iStateId = iAlarmId;
		pAlarm->m_fDelayTime = 0.f;
		pAlarm->m_fShowTime = fShowTime;
		pAlarm->m_eColor = eColor;
		pAlarm->m_bDanger = bDanger;
		pAlarm->m_bRepeat = true;

		wstring wstrComment = GET_STRING( iStringIndex );
		pAlarm->m_wstrMessage = wstrComment;

		m_vecDangerAlarm.push_back( pAlarm );
		iAlarmIndex = m_vecDangerAlarm.size() - 1;
	}
	
	ShowDangerAlarm( iAlarmIndex );
}

void CX2Game::ShowDangerAlarm( int iIndex )
{
	if( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		return;

	if( iIndex < 0 )
		return;	
	DangerAlarm *pAlarm = m_vecDangerAlarm[iIndex];
	if( pAlarm == NULL )
		return;
		
	CKTDGParticleSystem::CParticleEventSequence* pSeq = GetMajorParticle()->GetInstanceSequence( m_hDangerAlarm );
	if( pSeq != NULL )
	{
		pSeq->SetShowObject(false);
	}

	m_fShowAlarmTime = 0.f;
	m_pShowAlarm = pAlarm;

#ifdef DIALOG_SHOW_TOGGLE	
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#endif
	{
		if( m_pShowAlarm->m_bDanger == true )
		{
			if( pSeq != NULL )
			{
				pSeq->SetShowObject(true);
			}
			else
			{
				m_hDangerAlarm = GetMajorParticle()->CreateSequenceHandle( NULL, L"danger_alarm_01", 0.0f, 0.0f, 0.0f );
			}
		}
	}
	
}

int CX2Game::LoadDangerAlarm( KLuaManager &luaManager, UidType iUid, int iStateId )
{
	int iRet = -1;
	DangerAlarm *pDangerAlarm = NULL;
	if( luaManager.BeginTable( "ALARM" ) == true )
	{
		pDangerAlarm = new DangerAlarm();

		int iStringIndex;

		LUA_GET_VALUE( luaManager, "DANGER",		pDangerAlarm->m_bDanger,	false );		
		LUA_GET_VALUE( luaManager, "ALARM_MESSAGE",	iStringIndex,				STR_ID_EMPTY );		
		LUA_GET_VALUE( luaManager, "DELAY",			pDangerAlarm->m_fDelayTime,	0.f );
		LUA_GET_VALUE( luaManager, "DISAPPEAR_TIME",pDangerAlarm->m_fShowTime,	3.f );
		LUA_GET_VALUE( luaManager, "REPEAT",		pDangerAlarm->m_bRepeat,	true );
		
		LUA_GET_VALUE_ENUM( luaManager, "ALARM_COLOR_TYPE", pDangerAlarm->m_eColor, ALARM_COLOR_TYPE, ACT_BLACK );
				
		pDangerAlarm->m_wstrMessage = GET_STRING( iStringIndex );		
		pDangerAlarm->m_uid = iUid;
		pDangerAlarm->m_iStateId = iStateId;
		
		luaManager.EndTable(); 
	}

	if( pDangerAlarm != NULL )
	{
		m_vecDangerAlarm.push_back( pDangerAlarm );
		iRet = (int)m_vecDangerAlarm.size() - 1;
	}

	return iRet;
}

int CX2Game::GetDangerAlarm( UidType iUid, int iStateId )
{
	for(UINT i=0; i<m_vecDangerAlarm.size(); ++i)
	{
		DangerAlarm *pAlarm = m_vecDangerAlarm[i];
		if( pAlarm != NULL &&
			pAlarm->m_uid == iUid && pAlarm->m_iStateId == iStateId )
		{
			return i;
		}
	}

	return -1;
}

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
void CX2Game::ShowTimerAlarm_LUA( int iAlarmId, float fShowTime, ALARM_COLOR_TYPE eColor, bool bDanger, float fValue_ )
{
	if( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		return;

	int iAlarmIndex = GetDangerAlarm( -1, iAlarmId );
	if( iAlarmIndex < 0 )
	{
		DangerAlarm *pAlarm = new DangerAlarm();
		pAlarm->m_uid = -1;
		pAlarm->m_iStateId = iAlarmId;
		pAlarm->m_fDelayTime = 0.f;
		pAlarm->m_fShowTime = fShowTime;
		pAlarm->m_eColor = eColor;
		pAlarm->m_bDanger = bDanger;
		pAlarm->m_bRepeat = true;
		pAlarm->m_eFontType = XUF_HEADLINE_30_NORMAL;

		wstring wstrComment = g_pMain->GetEDString( static_cast<int>( fValue_ ) );
		pAlarm->m_wstrMessage = wstrComment;

		m_vecDangerAlarm.push_back( pAlarm );
		iAlarmIndex = m_vecDangerAlarm.size() - 1;
	}

	ShowTimerAlarm( iAlarmIndex, fValue_ );
}

void CX2Game::ShowTimerAlarm( int iIndex, float fValue_ )
{
	if( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		return;

	if( iIndex < 0 )
		return;	
	DangerAlarm *pAlarm = m_vecDangerAlarm[iIndex];
	if( pAlarm == NULL )
		return;

	wstring wstrComment = g_pMain->GetEDString( static_cast<int>( fValue_ ) );
	pAlarm->m_wstrMessage = wstrComment;

	CKTDGParticleSystem::CParticleEventSequence* pSeq = GetMajorParticle()->GetInstanceSequence( m_hDangerAlarm );
	if( pSeq != NULL )
	{
		pSeq->SetShowObject(false);
	}

	m_fShowAlarmTime = 0.f;
	m_pShowAlarm = pAlarm;

#ifdef DIALOG_SHOW_TOGGLE	
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#endif
	{
		if( m_pShowAlarm->m_bDanger == true )
		{
			if( pSeq != NULL )
			{
				pSeq->SetShowObject(true);
			}
			else
			{
				m_hDangerAlarm = GetMajorParticle()->CreateSequenceHandle( NULL, L"danger_alarm_01", 0.0f, 0.0f, 0.0f );
			}
		}
	}
}
#endif //SERV_HALLOWEEN_EVENT_2013

#ifdef	ELOG_STATISTICS_NEW_DATA

void    CX2Game::KFPS_STATISTICS::AddNewFps( float fFps )
{
	if ( fFps <= 0.f )
		return;

	if ( m_uNumOfFps == 0 )
	{
		m_fAverageFps = fFps;
		m_uNumOfFps = 1;
	}
	else
	{
		float   fOneWeight = 1.f / (float) ( m_uNumOfFps + 1 );
		float   fSumWeight = (float) m_uNumOfFps * fOneWeight;

		m_fAverageFps = fSumWeight * m_fAverageFps + fOneWeight * fFps;
		m_uNumOfFps++;
	}

}//CX2Game::KFPS_STATISTICS::AddNewFps()

void        CX2Game::KGAME_STATISTICS::GatherPeriodicStatistics()
{
	m_kAverageFps.AddNewFps( DXUTGetFPS() );

}//CX2Game::KGAME_STATISTICS::GatherPeriodicStatistics()


void    CX2Game::KGAME_STATISTICS::Send_EGS_FRAME_AVERAGE_REQ( int iGameType  )
{
	if ( m_kAverageFps.m_uNumOfFps == 0 )
        return;

	KEGS_FRAME_AVERAGE_REQ kPacket;

    kPacket.m_cGameType = iGameType;
	kPacket.m_iFrameAverage = static_cast<int>( m_kAverageFps.m_fAverageFps );

	if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
		g_pData->GetServerProtocol()->SendPacket( EGS_FRAME_AVERAGE_REQ, kPacket );

}//CX2Game::KGAME_STATISTICS::Send_EGS_FRAME_AVERAGE_REQ()


#endif	ELOG_STATISTICS_NEW_DATA

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
// cheat ��ɾ�� npc �����ϴ� �̺�Ʈ�� �ùķ��̼� ������ �����ϴ�
bool CX2Game::Send_EGS_ADMIN_NPC_UNIT_CREATE_REQ_LUA( int unitID, int level, D3DXVECTOR3 vPos, bool bRight, int iAttrib1, int iAttrib2, int iSA1, int iSA2 )
{
	if ( m_bGameEnd == true )
		return false;

	//if( NULL == GetMyUser() )
	//	return false;

	if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		return false;

	int iKeyCode = -1;
	CX2Room::TEAM_NUM eTeam = CX2Room::TN_MONSTER;
	CX2NPCAI::NPC_AI_TYPE eAIType = CX2NPCAI::NAT_NORMAL;
	UidType iAllyUID = -1;
	bool bActive = true;
	bool bNoDrop = false;

	KEGS_ADMIN_NPC_UNIT_CREATE_REQ kPacket;
	kPacket.m_kNPCUnitReq.Init();

	kPacket.m_kNPCUnitReq.m_NPCID		= unitID;
	kPacket.m_kNPCUnitReq.m_Level		= level;
	kPacket.m_kNPCUnitReq.m_bActive		= bActive;
	kPacket.m_kNPCUnitReq.m_vPos.x		= vPos.x;
	kPacket.m_kNPCUnitReq.m_vPos.y		= vPos.y;
	kPacket.m_kNPCUnitReq.m_vPos.z		= vPos.z;
	kPacket.m_kNPCUnitReq.m_bIsRight	= bRight;
	kPacket.m_kNPCUnitReq.m_bNoDrop		= bNoDrop;
	kPacket.m_kNPCUnitReq.m_KeyCode		= (char)iKeyCode;
	kPacket.m_kNPCUnitReq.m_cTeamNum	= (char) eTeam;
	kPacket.m_kNPCUnitReq.m_cAIType		= (char) eAIType;
	kPacket.m_kNPCUnitReq.m_iAllyUID	= iAllyUID;

	kPacket.m_cAttrib1 = static_cast<char>(iAttrib1);
	kPacket.m_cAttrib2 = static_cast<char>(iAttrib2);
	kPacket.m_cSA1 = static_cast<char>(iSA1);
	kPacket.m_cSA2 = static_cast<char>(iSA2);

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_NPC_UNIT_CREATE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_ADMIN_NPC_UNIT_CREATE_ACK, 60.f ); 

	return true;
}

bool CX2Game::Handler_EGS_ADMIN_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_NPC_UNIT_CREATE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;	
}


bool CX2Game::Handler_EGS_ADMIN_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_NPC_UNIT_CREATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	CX2Dungeon* pDungeon = static_cast< CX2DungeonGame* >( this )->GetDungeon();
	if( pDungeon )
	{
		KNPCUnitReq& kNPCUnitReq = kEvent.m_kNPCUnitNot;

		CX2GUNPC* pNPC = CreateNPC( (CX2UnitManager::NPC_UNIT_ID)kNPCUnitReq.m_NPCID, 
			kNPCUnitReq.m_Level,
			kNPCUnitReq.m_bActive,
			kNPCUnitReq.m_UID, 
			D3DXVECTOR3( kNPCUnitReq.m_vPos.x, kNPCUnitReq.m_vPos.y, kNPCUnitReq.m_vPos.z ), 
			kNPCUnitReq.m_bIsRight,
			kNPCUnitReq.m_fDelayTime,
			(int) kNPCUnitReq.m_KeyCode, 
			(CX2Room::TEAM_NUM) kNPCUnitReq.m_cTeamNum,
			(CX2NPCAI::NPC_AI_TYPE) kNPCUnitReq.m_cAIType,
			kNPCUnitReq.m_iAllyUID );

		std::map< int, KAttribEnchantNpcInfo >& mapAttribEnchantNpcInfo = pDungeon->GetNowStage()->GetStageData()->m_mapAttribEnchantNpcInfo;
		KAttribEnchantNpcInfo& kAttribEnchantNpcInfo = mapAttribEnchantNpcInfo[kNPCUnitReq.m_UID];
		kAttribEnchantNpcInfo.m_cAttribEnchant1 = kEvent.m_cAttrib1;
		kAttribEnchantNpcInfo.m_cAttribEnchant2 = kEvent.m_cAttrib2;
		kAttribEnchantNpcInfo.m_cExtraSpecialAbility1 = kEvent.m_cSA1;
		kAttribEnchantNpcInfo.m_cExtraSpecialAbility2 = kEvent.m_cSA2;

		if ( pNPC )
		{
			pNPC->ApplyEnchantMonster( kAttribEnchantNpcInfo );
		}

	}

	//{{ kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
#ifdef SERV_SECRET_HELL
	// �������� ���͸� ���Ƿ� �����ؼ� �������� ��
	// �ش� ���Ͱ� substage clear ���ǿ� �ִ� ���
	// Ŭ���� ������ �ش� ���� original count�� update ������
	if ( GetGameType() == GT_DUNGEON )
	{
		CX2Dungeon* pDungeon = static_cast< CX2DungeonGame* >( this )->GetDungeon();

		if ( pDungeon != NULL &&
			pDungeon->GetNowStage() != NULL &&
			pDungeon->GetNowStage()->GetNowSubStage() != NULL )
		{
			pDungeon->GetNowStage()->GetNowSubStage()->SetMonsterCountForSubStageClearCheck();
		}

	}
#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.23 // ��д��� �۾�(Extra ���)
	return true;
}
#endif SERV_CREATE_ELITE_FOR_ADMIN

#ifdef SUMMON_MONSTER_CARD_SYSTEM
int CX2Game::GetSummonMonsterCardNPCLevel( CX2GUNPC* pNPC, int iLevel )
{
	if( NULL == g_pData->GetPartyManager() && NULL == g_pData->GetPartyManager()->GetMyPartyData() )
		return iLevel;
	
	if( NULL != g_pData->GetDungeonRoom() &&  true == g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID) )
	{
		return 1;	//����� ���� 1���� ����
	}
	
#ifndef NEW_HENIR_DUNGEON // 1���� ���� -> ���� ���� �������� ����
	if( CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
	{
		return 1;	//��ϸ� ���� ���� 1���� ����
	}
#endif // NEW_HENIR_DUNGEON

	if( CX2Dungeon::DM_SECRET_HELL == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode 
#ifdef NEW_HENIR_DUNGEON // 1���� ���� -> ���� ���� �������� ����
		|| CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode		
#endif // NEW_HENIR_DUNGEON
		)
	{
#ifdef  X2OPTIMIZE_UNITTYPE_BUG_FIX
        CX2GUUser* pUser = pNPC->GetOwnerGameUnit();
#else   X2OPTIMIZE_UNITTYPE_BUG_FIX
		CX2GUUser* pUser = GetUserUnitByUID( pNPC->GetOwnerGameUnitUID() );
#endif  X2OPTIMIZE_UNITTYPE_BUG_FIX
		if( NULL != pUser )
			return pUser->GetUnitLevel();	//��� ���� �� ���� User�� ������ ���� ����
	}

#ifdef SET_SUMMON_CARD_MONSTER_EVENT_DUNGEON_LEVEL
	if( true == CX2Dungeon::IsEventDungeon( static_cast<SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) )
	{
		return 1;	/// �̺�Ʈ ������ ���� ���� ����
	}
#endif SET_SUMMON_CARD_MONSTER_EVENT_DUNGEON_LEVEL

	return iLevel;
}

void CX2Game::CreateSummonMonsterCardNPC( const CX2Item::SpecialAbility* pSa, CX2GUUser* pCX2GUUser )
{
#ifdef FIX_FIELD_SUMMON_MONSTER
	if( GT_DUNGEON != GetGameType() )
		return;
#endif // FIX_FIELD_SUMMON_MONSTER

	if( -1 != pCX2GUUser->GetSummonMonsterCardData().GetSummonMonsterUID() )	//���� ��ȯ���� NPCID�� ������ ���� �� ��ȯ �Ұ�
		return;

	wstring wstrGroupMonsterInfo = pSa->GetStringValue1();		

	wstring wstrSummonMonsterID		= L"";	//��ȯ�� ���� ID
	wstring wstrSummonMonsterRate	= L"";	//�ش� ���Ͱ� ��ȯ�� Ȯ��

	int iMonsterID = 0;

	int iStartIndex = 0;						

	while( wstrGroupMonsterInfo.find( L" " ) )	//ID�� Ȯ���� �޾ƿͼ� ��ȯ Ȯ���� ���� �Ǹ� ��ȯ, ��ȯ Ȯ���� ���� ���� ������ ���� ��ȯ ������ �޾ƿ´�.
	{
		iStartIndex = wstrGroupMonsterInfo.find( L" ", iStartIndex + 1 );

		if( wstrGroupMonsterInfo.compare( L"Group 0" ) != 0 )					
		{
			int	iMiddleIndex = wstrGroupMonsterInfo.find( L" ", iStartIndex + 1 );

			if( iMiddleIndex != -1 )						//�ش� ���� ID�� ��ȯ Ȯ���� ���ԵǾ� ���� ��, Ȯ�� ���
			{
				int	iEndIndex			= wstrGroupMonsterInfo.find( L" ", iMiddleIndex + 1 );
				wstrSummonMonsterRate	= wstrGroupMonsterInfo.substr( iMiddleIndex + 1, iEndIndex - iMiddleIndex - 1 );	//NPC ��ȯ Ȯ��
				int iSummonMonsterRate	= _wtoi( wstrSummonMonsterRate.c_str() );										//������ ���� int�� ��ȯ

				if( iSummonMonsterRate > RandomInt() )	//��ȯ Ȯ���� ���� ��Ų�ٸ�, ��ȯ ID ����
				{
					wstrSummonMonsterID = wstrGroupMonsterInfo.substr( iStartIndex + 1, iMiddleIndex - iStartIndex - 1 );//NPC ID

					break;
				}
				else
				{
					iStartIndex = iMiddleIndex;			//�ƴ϶��, ���� ���� �˻�
				}
			}
			else										//ID�� ��ȯ Ȯ���� ���ԵǾ� ���� �ʴٸ�, ������ ��ȯ
			{
				wstrSummonMonsterID = wstrGroupMonsterInfo.substr( iStartIndex + 1 );	//Default ���� ID

				break;
			}
		}
		else
		{
			ASSERT( "Summon Value Error!!!" );

			break;
		}
	}

	iMonsterID = _wtoi( wstrSummonMonsterID.c_str() );	//��ȯ ID ����

	if( 0 != iMonsterID )
	{
		int iMonsterLevel = pSa->m_Value1;				//��ȯ ���� ����
		if( 0 == iMonsterLevel )
		{
			iMonsterLevel = pCX2GUUser->GetUnitLevel();	//Default���� ��, ��ȯ Unit�� ������ ����
		}

		int iMonsterTeam  = pSa->m_Value2;				//��ȯ ���� ��

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-11

		D3DXVECTOR3					vSummonPos		= pCX2GUUser->GetPos();			/// ���� ��ġ
		bool						bIsRight		= pCX2GUUser->GetIsRight();		/// ���� ����
		CX2GUNPC::NPC_CREATE_TYPE	eCreateNPCType	= CX2GUNPC::NCT_MONSTER_CARD;	/// ���� Ÿ��

		if ( NULL != g_pData && 
			 NULL != g_pData->GetUIManager() && 
			 NULL != g_pData->GetUIManager()->GetUIQuickSlot() &&
			 true == g_pData->GetUIManager()->GetUIQuickSlot()->GetIsSummonCardSlot() )
		{
			/// ����� �� �̶��, �Ʊ� ��ȯ�� ��ġ�� �����Ѵ�.
			CX2GUNPC* pSummonGateAlly = GetNPCUnitByType(  CX2UnitManager::NUI_GOD_GATE_ALLY );

			if ( NULL != pSummonGateAlly )
			{
				vSummonPos		= pSummonGateAlly->GetPos();
				bIsRight		= true;
				eCreateNPCType	= CX2GUNPC::NCT_DEFENCE_DUNGEON;
				iMonsterLevel	= 1;
			}
		}
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
		if( iMonsterID == EVENT_MONSTER_CARD_ALLY_ID )
		{
			dbg::cout << L"����α�-�׽�Ʈ : �ǹ��� ī�� ��� �� ���� ��ȯ üũ �����Ѵ�!!" << dbg::endl;
			vSummonPos.x -= 200;
			vSummonPos.z -= 200; 
			CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
			D3DXVECTOR3 outPos(0, 0, 0);
			//��ȯ�� ���� ��
			if(  pCX2GUUser->GetRandomInt() < EVENT_MONSTAR_CARD_RATE ) //50���� ������ ���۳� ��ȯ
			{
				for( int i = 0; i < EVENT_MONSTER_CARD_MAX_ENENY; ++i)
				{
					vSummonPos.x += 100;
					vSummonPos.z += 100;
					if(pLineMap != NULL)
					{
						//����� ���θʿ� ��ȯ�Ѵ�
						if( -1 != g_pX2Game->GetWorld()->GetLineMap()->GetNearestLine( vSummonPos, &outPos ) )
						{
							vSummonPos = outPos;
							g_pX2Game->CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( EVENT_MONSTER_CARD_ENEMY_ID ), iMonsterLevel, true, 
								vSummonPos, bIsRight, 0.f, false, -1, CX2Room::TN_MONSTER, 
								CX2NPCAI::NAT_NORMAL, -1,false, CX2Room::TN_NONE, CX2GUNPC::NCT_NONE );
						}
						else //���θ��� ���� ���� �ʴٸ� ������ġ�� ��ȯ�Ѵ�. 
						{
							g_pX2Game->CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( EVENT_MONSTER_CARD_ENEMY_ID ), iMonsterLevel, true, 
								pCX2GUUser->GetPos(), bIsRight, 0.f, false, -1, CX2Room::TN_MONSTER, 
								CX2NPCAI::NAT_NORMAL, -1,false, CX2Room::TN_NONE, CX2GUNPC::NCT_NONE );
						}
					}
					
				}
				return;
			}
			else //50���� ������ ������ ��ȯ
			{
				for( int i = 0; i < EVENT_MONSTER_CARD_MAX_ENENY; ++i)
				{
					vSummonPos.x += 100;
					vSummonPos.z += 100;
					if(pLineMap != NULL)
					{
						//����� ���θʿ� ��ȯ�Ѵ�
						if( -1 != g_pX2Game->GetWorld()->GetLineMap()->GetNearestLine( vSummonPos, &outPos ) )
						{
							vSummonPos = outPos;
							g_pX2Game->CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( iMonsterID ), iMonsterLevel, true, 
								vSummonPos, bIsRight, 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>( iMonsterTeam ), 
								CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID(),false, CX2Room::TN_NONE, CX2GUNPC::NCT_MONSTER_CARD );
						}
						else   //���θ��� ���� ���� �ʴٸ� ������ġ�� ��ȯ�Ѵ�.
						{
							g_pX2Game->CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( iMonsterID ), iMonsterLevel, true, 
								pCX2GUUser->GetPos(), bIsRight, 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>( iMonsterTeam ), 
								CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID(),false, CX2Room::TN_NONE, CX2GUNPC::NCT_MONSTER_CARD );
						}
					}
					
				}
				return;
			}
		}
#endif//EVENT_MONSTER_CARD_SUMMON_ENEMY

		CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( iMonsterID ), iMonsterLevel, true, 
		vSummonPos, bIsRight, 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>( iMonsterTeam ), 
		CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID(),false, CX2Room::TN_NONE, eCreateNPCType );

	#else // SERV_NEW_DEFENCE_DUNGEON

		CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( iMonsterID ), iMonsterLevel, true, 
			pCX2GUUser->GetPos(), pCX2GUUser->GetIsRight(), 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>( iMonsterTeam ), 
			CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID(),false, CX2Room::TN_NONE, true );

	#endif // SERV_NEW_DEFENCE_DUNGEON

	}
}
#endif SUMMON_MONSTER_CARD_SYSTEM

#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
bool CX2Game::CanCheckAbuser( const CX2GUUser* pCheckUser_, std::map< UidType, boost::timer >::iterator mit_ )
{ 
 	return ( pCheckUser_->GetNowStateID() > static_cast<int>( CX2GUUser::USI_LOSE ) 
 		&& mit_->second.elapsed() > 1.0f 
		&& NULL != GetMyUnit()
		&& mit_->first != GetMyUnit()->GetUnitUID() );
}

void CX2Game::AbuserUserCheck( const CX2GUUser* pCheckUser_ )
{
	if( m_pRoom != NULL )
	{
		const UidType uidCheckUser = pCheckUser_->GetUnitUID();

		// �ð� ���ź�
		std::map< UidType, boost::timer >::iterator mitFirst = m_mapCheckUDPTimer.find( uidCheckUser );
		if( mitFirst == m_mapCheckUDPTimer.end() )
		{
			// 2012.06.17 lygan_������ // ó�� ��ũ�� �ְ� �޾Ҵ� �������
			InsertGUUserToCheckUDPTimer( uidCheckUser );
		}
		else
		{
			mitFirst->second.restart();
		}

		// �˻��
		std::map< UidType, boost::timer >::iterator mit;
		for ( mit = m_mapCheckUDPTimer.begin(); mit != m_mapCheckUDPTimer.end(); mit++ )
		{
			// 2012.06.17 lygan_������ // �ι�° ��ũ �����ð��� 5�� �̻��̴� �̻��ϴ�
			// 2012.06.17 lygan_������ // ���� ���� �Ȱ� �ƴ��� üũ
			if( CanCheckAbuser( pCheckUser_, mit ) )
			{
				// �������� �����ϹǷ� �ٷ� ��ϴ�. ����� ����� �����̷� ������ �޶�� ��û
				KEGS_FORCE_RELAY_NOT kPacket;
				kPacket.m_iTargetUnitUID = mit->first;

				if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
					g_pData->GetServerProtocol()->SendPacket( EGS_FORCE_RELAY_NOT, kPacket );
				// �ѹ� �Ű� �������� �ð� �ʱ�ȭ ���ش�.
				mit->second.restart();
			}
		}
	}
}
#else

#ifdef UDP_CAN_NOT_SEND_USER_KICK
void CX2Game::AbuserUserCheck( UidType iUnitUID_)
{
	#ifdef NOT_USE_UDP_CHECK_INHOUSE
	return;
	#endif //NOT_USE_UDP_CHECK_INHOUSE

	if( NULL != g_pData && NULL != g_pData->GetGameUDP() &&
		false == g_pData->GetGameUDP()->ConnectCheckResult() )
	{//UDP ������ üũ ��� ���ж�� �������� �ʵ��� ����
		return;
	}

	if( false == g_pInstanceData->GetUDPKickOff() )
	{
#ifdef LOG_UDP_CHECK
		{
			std::map< UidType, boost::timer >::iterator mitFirst;
			if( GetGameType() == GT_PVP && m_pRoom != NULL && iUnitUID_ > 0 && m_pRoom->GetCheckUDPGameCountStart() == true && NULL != GetMyUnit() )
			{
				mitFirst = g_pInstanceData->m_mapCheckUDPTimerTemp.find(iUnitUID_);
				if(mitFirst == g_pInstanceData->m_mapCheckUDPTimerTemp.end() )
				{
					boost::timer timerFirst;
					timerFirst.restart();
					g_pInstanceData->m_mapCheckUDPTimerTemp.insert(std::make_pair(iUnitUID_, timerFirst )); // 2012.06.17 lygan_������ // ó�� ��ũ�� �ְ� �޾Ҵ� �������
				}
				else
				{
					std::map< UidType, boost::timer >::iterator mit;

					for ( mit = g_pInstanceData->m_mapCheckUDPTimerTemp.begin(); mit != g_pInstanceData->m_mapCheckUDPTimerTemp.end(); mit++ )
					{
						if ( mit->second.elapsed() > 1.0f)	// 2012.06.17 lygan_������ // �ι�° ��ũ �����ð��� 1�� �̻��̴� �̻��ϴ�
						{
							if(mit->first != GetMyUnit()->GetUnitUID()) // 2012.06.17 lygan_������ // ���� ���� �Ȱ� �ƴ��� üũ
							{
								std::map< UidType, boost::timer >::iterator mit2;
								mit2 = g_pInstanceData->m_mapCheckUDPTimerTemp.find(GetMyUnit()->GetUnitUID()); // 2012.06.17 lygan_������ // ���� �ƴ϶�� ���� �������� üũ
								if( mit2 != g_pInstanceData->m_mapCheckUDPTimerTemp.end() )
								{
									if( mit2->second.elapsed() <= 1.0f ) // 2012.06.17 lygan_������ // �� �������� 1�� �̻��̶�� ���� �̻��ѰŴ� ���� ���� ������� ���Ѵ�.
									{
										std::map< UidType, __int64 >::iterator mitDelayCount;
										mitDelayCount = g_pInstanceData->m_mapCheckUDPTDelayCountTemp.find(mit->first); // 2012.06.17 lygan_������ // �ٸ� ������ �����Ǿ��� ����ī��Ʈ �ø����� ã��
										if(mitDelayCount == g_pInstanceData->m_mapCheckUDPTDelayCountTemp.end() )
										{
											__int64 delayCount = 1;
											g_pInstanceData->m_mapCheckUDPTDelayCountTemp.insert(std::make_pair(mit->first, delayCount)); // 2012.06.17 lygan_������ // ó�� ������ ������ ���� ī��Ʈ ����ϰ� �÷�����
											mit->second.restart();
										}
										else
										{
											mitDelayCount->second += 1;
										}
									}
								}
							}
						}
						else  // 2012.06.17 lygan_������ // �ι�° ��ũ �����ð��� 5�� ���ϴ� �ʱ�ȭ ����
						{
							if (iUnitUID_ == mit->first)
								mit->second.restart();
						}
					}
				}
			}
		}
#endif //LOG_UDP_CHECK
		{
			std::map< UidType, boost::timer >::iterator mitFirst;
			if( GetGameType() == GT_PVP && m_pRoom != NULL && iUnitUID_ > 0 && m_pRoom->GetCheckUDPGameCountStart() == true && NULL != GetMyUnit() )
			{				
				
				mitFirst = g_pInstanceData->m_mapCheckUDPTimer.find(iUnitUID_);
				if(mitFirst == g_pInstanceData->m_mapCheckUDPTimer.end() )
				{
					boost::timer timerFirst;
					timerFirst.restart();
					if( NULL != GetUserUnitByUID(iUnitUID_) )
					{
						g_pInstanceData->m_mapCheckUDPTimer.insert(std::make_pair(iUnitUID_, timerFirst )); // 2012.06.17 lygan_������ // ó�� ��ũ�� �ְ� �޾Ҵ� �������
					}
				}
				else
				{
					std::map< UidType, boost::timer >::iterator mit;

					for ( mit = g_pInstanceData->m_mapCheckUDPTimer.begin(); mit != g_pInstanceData->m_mapCheckUDPTimer.end(); mit++ )
					{
						if ( mit->second.elapsed() > 5.0f)	// 2012.06.17 lygan_������ // �ι�° ��ũ �����ð��� 5�� �̻��̴� �̻��ϴ�
						{
							if(mit->first != GetMyUnit()->GetUnitUID() && // 2012.06.17 lygan_������ // ���� ���� �Ȱ� �ƴ��� üũ
								NULL != GetUserUnitByUID(iUnitUID_) )  // ���� ���ӿ� �����ϴ� �������� üũ
							{
								std::map< UidType, boost::timer >::iterator mit2;
								mit2 = g_pInstanceData->m_mapCheckUDPTimer.find(GetMyUnit()->GetUnitUID()); // 2012.06.17 lygan_������ // ���� �ƴ϶�� ���� �������� üũ
								if( mit2 != g_pInstanceData->m_mapCheckUDPTimer.end() )
								{
									if( mit2->second.elapsed() <= 5.0f ) // 2012.06.17 lygan_������ // �� �������� 5�� �̻��̶�� ���� �̻��ѰŴ� ���� ���� ������� ���Ѵ�.
									{
										std::map< UidType, __int64 >::iterator mitDelayCount;
										mitDelayCount = g_pInstanceData->m_mapCheckUDPTDelayCount.find(mit->first); // 2012.06.17 lygan_������ // �ٸ� ������ �����Ǿ��� ����ī��Ʈ �ø����� ã��
										if(mitDelayCount == g_pInstanceData->m_mapCheckUDPTDelayCount.end() )
										{
											__int64 delayCount = 1;
											g_pInstanceData->m_mapCheckUDPTDelayCount.insert(std::make_pair(mit->first, delayCount)); // 2012.06.17 lygan_������ // ó�� ������ ������ ���� ī��Ʈ ����ϰ� �÷�����
											mit->second.restart();
										}
										else
										{
											mitDelayCount->second += 1;											
											if( mitDelayCount->second >= 3 ) // 2012.06.17 lygan_������ // ���� 3ȸ �̻� �����Ǿ���. ���Ӽ����� ����ؼ� ¥����
											{
												mit->second.restart();  // �ѹ� �Ű� �������� �ð� �ʱ�ȭ ���ش�.
									#ifdef UDP_DOWNLOAD_BLOCK_CHECK
												g_pData->GetGameUDP()->ConnectTestToCheck();// ������ �Ű� �ϱ� �� UDP ������ üũ
												g_pInstanceData->m_setAbuseUnitUid.insert( mitDelayCount->first );
									#else
												KEGS_UDP_CHECK_KICK_USER_NOT kPacket;
												kPacket.m_iMyUnitUID = GetMyUnit()->GetUnitUID();
												kPacket.m_iAccusationUnitUID = mitDelayCount->first;
												kPacket.m_bNowKickOut = true;

												if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
												{
													g_pData->GetServerProtocol()->SendPacket( EGS_UDP_CHECK_KICK_USER_NOT, kPacket );
												}
									#endif //UDP_DOWNLOAD_BLOCK_CHECK
											}
											else
											{
												mit->second.restart();  // ���� ������ 3ȸ �̸��̹Ƿ� �ð��� �ʱ�ȭ ���ش�
											}
										}
									}
								}
							}
						}
						else  // 2012.06.17 lygan_������ // �ι�° ��ũ �����ð��� 5�� ���ϴ� �ʱ�ȭ ����
						{
							if (iUnitUID_ == mit->first)
								mit->second.restart();
						}
					}
				}
			}
		}
	}
	else
	{
		g_pInstanceData->m_mapCheckUDPTimer.clear(); // 2012.06.20 lygan_������ // �������� �ش� ��� �ǽð����� ���� �ð� �ʱ�ȭ �����ֱ�
#ifdef LOG_UDP_CHECK
		g_pInstanceData->m_mapCheckUDPTimerTemp.clear();
#endif //LOG_UDP_CHECK
	}
}

	#ifdef UDP_DOWNLOAD_BLOCK_CHECK 
/** @function : AbuserUserReport
	@brief	: ������ �ǿ� ������ ��ϵ� UID�� ���� ���,			
*/
void CX2Game::AbuserUserReport()
{
	if( NULL == g_pData || NULL == g_pData->GetGameUDP() ||
		NULL == g_pInstanceData )
		return;

	//������ �ǿ� ���� �Ű�
	if( false == g_pInstanceData->m_setAbuseUnitUid.empty() )
	{
		bool bConnectCheckResult = g_pData->GetGameUDP()->ConnectCheckResult();
		bool bRecieveCheckReuslt = g_pData->GetGameUDP()->ReceiveCheckResult(); // UDP������ üũ �õ� ��, üũ ��� ���ſ���

		for( set<UidType>::iterator it = g_pInstanceData->m_setAbuseUnitUid.begin();
			it != g_pInstanceData->m_setAbuseUnitUid.end(); /*++it*/ )
		{
			if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
			{
				KEGS_UDP_CHECK_KICK_USER_NOT kPacket;
				kPacket.m_bNowKickOut = true;
				if( NULL != GetMyUnit() )
					kPacket.m_iMyUnitUID = GetMyUnit()->GetUnitUID();
			
				if( false == bConnectCheckResult ) 
				{//UDP������ üũ ���� �� �ù� ������ �Ǵ�
					if( true == bRecieveCheckReuslt )
					{
						if( NULL != GetUserUnitByUID(*it) )
						{
							kPacket.m_iAccusationUnitUID = kPacket.m_iMyUnitUID;
							g_pData->GetServerProtocol()->SendPacket( EGS_UDP_CHECK_KICK_USER_NOT, kPacket );
							g_pData->GetGameUDP()->InitConnectTestToCheck();
						}
						else
						{
							g_pInstanceData->m_mapCheckUDPTimer.clear();
							g_pInstanceData->m_mapCheckUDPTDelayCount.clear();
						}
						g_pInstanceData->m_setAbuseUnitUid.clear();
					}
					return;
				}
				else	/// �Ϲ� ������ �Ű�
				{
					if( NULL != GetUserUnitByUID(*it) )
					{
						kPacket.m_iAccusationUnitUID = *it;
						g_pData->GetServerProtocol()->SendPacket( EGS_UDP_CHECK_KICK_USER_NOT, kPacket );
					}
					else
					{		
						g_pInstanceData->m_mapCheckUDPTimer.clear();
						g_pInstanceData->m_mapCheckUDPTDelayCount.clear();
					}
					g_pInstanceData->m_setAbuseUnitUid.erase(it++);
				}
			}
		}
	}
}
	#endif //UDP_DOWNLOAD_BLOCK_CHECK

void CX2Game::EraseAbuserUserCheckByUID( UidType iUnitUID_ )
{
	if(false == g_pInstanceData->GetUDPKickOff() )
	{
		if( false == g_pInstanceData->m_mapCheckUDPTimer.empty() )
		{
			std::map< UidType, boost::timer >::iterator mit;
			mit = g_pInstanceData->m_mapCheckUDPTimer.find( iUnitUID_ );
			if( mit != g_pInstanceData->m_mapCheckUDPTimer.end() )
			{
				g_pInstanceData->m_mapCheckUDPTimer.erase( iUnitUID_ );
			}
		}

		if( false == g_pInstanceData->m_mapCheckUDPTDelayCount.empty() )
		{
			std::map< UidType, __int64 >::iterator mit2;
			mit2 = g_pInstanceData->m_mapCheckUDPTDelayCount.find( iUnitUID_ );
			if( mit2 != g_pInstanceData->m_mapCheckUDPTDelayCount.end() )
			{
				g_pInstanceData->m_mapCheckUDPTDelayCount.erase( iUnitUID_ );
			}
		}

		if( false == g_pInstanceData->m_setAbuseUnitUid.empty() )
		{
			g_pInstanceData->m_setAbuseUnitUid.erase( iUnitUID_ );
		}
	}
}
#endif //UDP_CAN_NOT_SEND_USER_KICK

#endif // SERV_FIX_SYNC_PACKET_USING_RELAY

/*FORCEINLINE*/ CKTDGParticleSystem* CX2Game::GetMajorParticle()
{
	return g_pData->GetGameMajorParticle();
}

/*FORCEINLINE*/ CKTDGParticleSystem* CX2Game::GetMinorParticle()
{
	return g_pData->GetGameMinorParticle();
}

/*FORCEINLINE*/ CKTDGXMeshPlayer* CX2Game::GetMajorXMeshPlayer()
{
	return g_pData->GetGameMajorXMeshPlayer();
}

/*FORCEINLINE*/ CKTDGXMeshPlayer* CX2Game::GetMinorXMeshPlayer()
{
	return g_pData->GetGameMinorXMeshPlayer();
}

/*FORCEINLINE*/ CX2EffectSet* CX2Game::GetEffectSet()
{
	return g_pData->GetGameEffectSet();
}


bool CX2Game::CheckAndWarningBusyStateNow( const bool bCheckDead_ /*= true*/ )
{
	if ( NULL != GetMyUnit() && true == GetMyUnit()->IsBusyStateNow() )
	{
		//WCHAR* pwszWarning = NULL;
		wstring wstrWarning;

		if ( GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE || !( GetMyUnit()->GetNowHp() > 0 ) )
		{
			if ( true == bCheckDead_ )
#ifdef HARDCODING_STRING_TO_INDEX
				wstrWarning = GET_STRING( STR_ID_19230 );
#else
				wstrWarning = L"���� ���¿����� �õ� �� �� �����ϴ�.\n��Ȱ �� �õ��ϼ���.";
#endif HARDCODING_STRING_TO_INDEX
		}
		else
		{
			const float fTimeGap = GetMyUnit()->GetRemainElapsedTimeNotBusyState();

#ifdef HARDCODING_STRING_TO_INDEX
			wstrWarning += GET_REPLACED_STRING( (STR_ID_19231, "i", static_cast<int>( fTimeGap ) + 1) );
#else
			wstrWarning = L"������ �� �� �����ϴ�\n"; //.\n�� ���Ŀ� �ٽ� �õ��ϼ���." );
			wstrWarning += GET_REPLACED_STRING( (STR_ID_2632, "i", static_cast<int>( fTimeGap ) + 1 ) );
			wstrWarning += L"�� ���Ŀ� �ٽ� �õ��ϼ���.";
#endif HARDCODING_STRING_TO_INDEX
		}

		if ( wstrWarning.empty() )
			return false;
		else
		{
#ifdef CLIENT_COUNTRY_EU
			GetInfoTextManager().PushText( XUF_DODUM_15_BOLD, 
				wstrWarning.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#else //CLIENT_COUNTRY_EU
			GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, 
				wstrWarning.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif //CLIENT_COUNTRY_EU
			if ( g_pChatBox != NULL )
				g_pChatBox->AddChatLog( wstrWarning.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

			return true;
		}
	}
	else
		return false;
}

void CX2Game::SetShowSmallGageAndNameForAllGameUnit( const bool bShow_ )
{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	BOOST_FOREACH( CX2GameUnit* pGameUnit, m_UnitList )
	{
		pGameUnit->SetShowSmallGageAndName( bShow_ );
	}
}


void CX2Game::ChangeUserUnitIndex( const UINT uiFirstIndex_, const UINT uiSecondIndex_ )
{
	ASSERT( uiFirstIndex_ < m_UserUnitList.size() && uiSecondIndex_ < m_UserUnitList.size() );
	if ( uiFirstIndex_ < m_UserUnitList.size() && uiSecondIndex_ < m_UserUnitList.size() )
	{
		CX2GUUser* pFirstUser = m_UserUnitList[uiFirstIndex_];
		CX2GUUser* pSecondUser = m_UserUnitList[uiSecondIndex_];

		// ���ҽ� �̰� �� �������� ���� �� ���Ƽ� �̷��� ��
		m_UserUnitList[uiFirstIndex_] = pSecondUser;
		m_UserUnitList[uiSecondIndex_] = pFirstUser;

		if ( NULL != pFirstUser )
			pFirstUser->SetUnitIndex( uiSecondIndex_ );

		if ( NULL != pSecondUser )
			pSecondUser->SetUnitIndex( uiFirstIndex_ );
	}
}


CX2GUUser* CX2Game::CreateGUUser( const CX2Room::SlotData* pSlotData_, int iSlotIndex_ )
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const CX2UnitManager::UnitTypeLuaTemplet& kUnitTypeLuaTemplet = 
		g_pData->GetUnitManager()->GetUnitTypeLuaTemplet( pSlotData_->m_pUnit->GetType() );
	lua_State*	script_info = ( kUnitTypeLuaTemplet.m_pLuaManager != NULL ) ? kUnitTypeLuaTemplet.m_pLuaManager->GetLuaState() : NULL;
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const CX2UnitManager::UnitTypeTemplet* pUnitTypeTemplet = g_pData->GetUnitManager()->GetUnitTypeTemplet( pSlotData_->m_pUnit->GetType() );
	const WCHAR* script_info = ( pUnitTypeTemplet != NULL ) ? pUnitTypeTemplet->m_wstrLuaScriptFile.c_str() : L"";
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


	switch( pSlotData_->m_pUnit->GetType() )
	{
	case CX2Unit::UT_ELSWORD:
		{
			return CX2GUElsword_SwordMan::CreateGUElsword( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData_->m_pUnit );
		}
		break;

	case CX2Unit::UT_ARME:
		{
			return CX2GUArme_VioletMage::CreateGUArme( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData_->m_pUnit );
		}
		break;

	case CX2Unit::UT_LIRE:
		{
			return CX2GULire_ElvenRanger::CreateGULire( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData_->m_pUnit );
		}
		break;

	case CX2Unit::UT_RAVEN:
		{
			return CX2GURaven::CreateGURaven( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData_->m_pUnit );
		}
		break;

	case CX2Unit::UT_EVE:
		{
			return CX2GUEve::CreateGUEve( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData_->m_pUnit );
		}
		break;

		//{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		{
			return CX2GUChung::CreateGUChung( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(), pSlotData_->m_pUnit );
		}
		break;
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		{
			return CX2GUAra::CreateGUAra( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(),
										  pSlotData_->m_pUnit );
		}
		break;
#endif
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS:
		{
			return CX2GUEL::CreateGUEL( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(),
				pSlotData_->m_pUnit );
		} break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CX2Unit::UT_ADD:
		{
			return CX2GUAdd::CreateGUAdd( iSlotIndex_, pSlotData_->m_TeamNum, script_info, g_pMain->GetFrameBufferNum(),
				pSlotData_->m_pUnit );
		} break;
#endif //SERV_9TH_NEW_CHARACTER

	default:
		{
			ASSERT( !"Create User Object is Fail!! ( Unknown User Type )" );
			return NULL;
		}
		break;
	}
}


/*virtual*/ void CX2Game::ResurrectionKeyProcess()
{
	if ( GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE && 
		m_bIsSendResurrectionReq == false )
	{
		if ( IsAllUserDead() == false )
		{
			if ( m_bResurrectionCountStart == true )
			{

				if ( m_InputData.oneZ == true
					&& GetOpenChatBox() == false )
				{

					Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( GetMyUnit()->GetUnit()->GetUID() );
				}

				if ( m_fResurrectionRemainTime > 0.0f 
					&& g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_X ) == TRUE
					&& GetOpenChatBox() == false )
				{
					m_fResurrectionRemainTime = 0.0f;
				}
			}
		}
		else
		{
			if ( m_bGameEnd == false )
			{
				if ( m_InputData.oneZ == true
					&& GetOpenChatBox() == false )
				{
					Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( GetMyUnit()->GetUnit()->GetUID() );
				}
			}

			if ( m_fResurrectionRemainTime > 0.0f 
				&& true == m_InputData.oneX 
				&& GetOpenChatBox() == false )
			{
				Handler_EGS_SKIP_CONTINUE_DUNGEON_NOT();
				m_fResurrectionRemainTime = 0.0f;
			}
		}
	}
}

void CX2Game::UpdateNpcNameColor( const int iMyLevel_ )
{
	BOOST_FOREACH( CX2GUNPC* pGUNpc, m_NPCUnitList )
	{
		if ( NULL != pGUNpc )
			pGUNpc->InitNameColor( iMyLevel_ );
	}
}

#ifdef BATTLE_FIELD_TEST

void CX2Game::WriteBroadCastUser()
{
	WCHAR wLog[512];
	ZeroMemory( wLog, sizeof(WCHAR) * 512 );
	wsprintf( wLog, L"UIDForSyncPacket: " );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	BOOST_FOREACH( UidType uid, g_pData->GetGameUDP()->GetNonRelayUIDs() )
	{
		WCHAR wBuffer[50];
		ZeroMemory( wBuffer, sizeof(WCHAR) * 50 );
		wsprintf( wBuffer, L"%lld ", uid );
		StringCchCat( wLog, sizeof(WCHAR) * 512, wBuffer );
	}
	BOOST_FOREACH( UidType uid, g_pData->GetGameUDP()->GetRelayUIDs() )
	{
		WCHAR wBuffer[50];
		ZeroMemory( wBuffer, sizeof(WCHAR) * 50 );
		wsprintf( wBuffer, L"%lld ", uid );
		StringCchCat( wLog, sizeof(WCHAR) * 512, wBuffer );
	}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	BOOST_FOREACH( UidType uid, m_vecUserUIDforSyncPacket )
//	{
//		WCHAR wBuffer[50];
//		ZeroMemory( wBuffer, sizeof(WCHAR) * 50 );
//		wsprintf( wBuffer, L"%lld ", uid );
//		StringCchCat( wLog, sizeof(WCHAR) * 512, wBuffer );
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	StateLog( wLog );
}
#endif // BATTLE_FIELD_TEST

/** @function : SetBuffInfoPacketToNpc
	@brief : �����κ��� ���� ���������� NPC���� ����
	@param : �����κ��� ���� NPC ���� ���� ��Ŷ
*/
void CX2Game::SetBuffInfoPacketToNpc( const KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT& kEvent_ )
{
	BOOST_FOREACH( const KNpcUnitBuffInfo& kNpcUnitBuffInfo, kEvent_.m_vecNpcUnitBuff )
	{
		CX2GUNPC* pGUNpcUnit = GetNPCUnitByUID( kNpcUnitBuffInfo.m_iNpcUID );
		if ( NULL != pGUNpcUnit )
			pGUNpcUnit->SetKBuffFactorPacketToGameUnit( kNpcUnitBuffInfo.m_vecBuffFactor );
	}
}

/** @function : SetBuffInfoPacketToGUUser
	@brief : �����κ��� ���� ���������� �������� ����
	@param : �����κ��� ���� ���� ���� ���� ��Ŷ
*/
void CX2Game::SetBuffInfoPacketToGUUser( const KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT& kEvent_ )
{
	/// ���� ���� ��Ŷ�� ó�� ����
	if ( NULL != GetMyUnit() && GetMyUnit()->GetUnitUID() == kEvent_.m_iUnitUID )
		return;
	
	CX2GUUser* pGUUser = GetUserUnitByUID( kEvent_.m_iUnitUID );
	if ( NULL != pGUUser )
	{
		pGUUser->SetKBuffFactorPacketToGameUnit( kEvent_.m_vecBuffFactor );
	}
	else
	{
		StateLog( L"Buff User is NULL" );
		ASSERT( !L"Buff User is NULL" );
	}
}
#ifdef MODIFY_DUNGEON_STAGING

CX2Game::CinematicUI::CinematicUI():
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hCinematicTopParticle( INVALID_PARTICLE_HANDLE )
	,m_hCinematicBottomParticle( INVALID_PARTICLE_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_pCinematicTopParticle( NULL )
	,m_pCinematicBottomParticle( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	,m_hCinematicTop(INVALID_PARTICLE_SEQUENCE_HANDLE)
	,m_hCinematicBottom(INVALID_PARTICLE_SEQUENCE_HANDLE)
	,m_bStart (false )
	,m_bEnd ( false )
	,m_fTimer ( 0.f )
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	,m_fDeleteTime ( -1.f )
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
{
	if( NULL != g_pData->GetUIMajorParticle() )
	{
		m_hCinematicTop = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"JoinNpcTop", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicTop)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicTop );
			if( pSeq != NULL )
			{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hCinematicTopParticle = pSeq->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_pCinematicTopParticle = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pSeq->SetShowObject(false);
			}
		}

		m_hCinematicBottom = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"JoinNpcBottom", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicBottom )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicBottom );
			if( pSeq != NULL )
			{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hCinematicBottomParticle = pSeq->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_pCinematicBottomParticle = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pSeq->SetShowObject(false);
			}
		}
	}
}

CX2Game::CinematicUI::~CinematicUI()
{
	if( NULL != g_pData->GetUIMajorParticle() )		
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCinematicTop );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCinematicBottom );
	}
}

HRESULT CX2Game::CinematicUI::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fTimer += fElapsedTime;

	if( true == m_bStart )
	{
        CKTDGParticleSystem::CParticle* pCinematicTopParticle = NULL;
        CKTDGParticleSystem::CParticle* pCinematicBottomParticle = NULL;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != g_pData->GetUIMajorParticle() )
        {
            if( INVALID_PARTICLE_HANDLE != m_hCinematicTopParticle )
                pCinematicTopParticle = g_pData->GetUIMajorParticle()->ValidateParticleHandle( m_hCinematicTopParticle );
            if( INVALID_PARTICLE_HANDLE != m_hCinematicBottomParticle )
                pCinematicBottomParticle = g_pData->GetUIMajorParticle()->ValidateParticleHandle( m_hCinematicBottomParticle );
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != g_pData->GetUIMajorParticle() )
		{
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicTop && NULL != m_pCinematicTopParticle )
			{
				g_pData->GetUIMajorParticle()->ValidateParticlePointer( m_hCinematicTop, m_pCinematicTopParticle );
			}
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicBottom && NULL != m_pCinematicBottomParticle )
			{
				g_pData->GetUIMajorParticle()->ValidateParticlePointer( m_hCinematicBottom, m_pCinematicBottomParticle );    
			}
		}
        pCinematicTopParticle = m_pCinematicTopParticle;
        pCinematicBottomarticle = m_pCinematicBottomParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		
		if(m_fTimer <= 1.0f)
		{
			float fAlpha = m_fTimer;
			if ( NULL != pCinematicTopParticle )
			{
				pCinematicTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
			}
			if ( NULL != pCinematicBottomParticle )
			{
				pCinematicBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
			}
		}
		else
		{		
			if ( NULL != pCinematicTopParticle )
			{
				pCinematicTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );
			}
			if ( NULL != pCinematicBottomParticle )
			{
				pCinematicBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );
			}

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
// CinematicUI ��� ��, m_fDeleteTime �� �����Ͽ� �����Ѵٸ�
// ������ �ð� ���� Fade-In, ������ ������ �ð� ���� Fade-Out �� �ǽ�!

			if ( -1.f != m_fDeleteTime )
			{
				if ( m_fTimer >= m_fDeleteTime / 2 )
				{
					m_bStart = false;
					m_bEnd = true;
				}
			}
			else 
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
			{
				m_fTimer = 1.1f;
			}
		}
	}
	

	if( true == m_bEnd )
	{
        CKTDGParticleSystem::CParticle* pCinematicTopParticle = NULL;
        CKTDGParticleSystem::CParticle* pCinematicBottomParticle = NULL;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != g_pData->GetUIMajorParticle() )
        {
            if( INVALID_PARTICLE_HANDLE != m_hCinematicTopParticle )
                pCinematicTopParticle = g_pData->GetUIMajorParticle()->ValidateParticleHandle( m_hCinematicTopParticle );
            if( INVALID_PARTICLE_HANDLE != m_hCinematicBottomParticle )
                pCinematicBottomParticle = g_pData->GetUIMajorParticle()->ValidateParticleHandle( m_hCinematicBottomParticle );
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != g_pData->GetUIMajorParticle() )
		{
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicTop && NULL != m_pCinematicTopParticle )
			{
				g_pData->GetUIMajorParticle()->ValidateParticlePointer( m_hCinematicTop, m_pCinematicTopParticle );
			}
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicBottom && NULL != m_pCinematicBottomParticle )
			{
				g_pData->GetUIMajorParticle()->ValidateParticlePointer( m_hCinematicBottom, m_pCinematicBottomParticle );    
			}
		}
        pCinematicTopParticle = m_pCinematicTopParticle;
        pCinematicBottomarticle = m_pCinematicBottomParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS 
		if ( -1.f != m_fDeleteTime  )							// DeleteTime �� �����Ѵٸ�
		{
			if ( m_fTimer >= ( m_fDeleteTime - 1.f ) )		// Fade-Out �ð��� 1�ʷ�..
			{
				float fAlpha = m_fDeleteTime - m_fTimer;
				if ( NULL != pCinematicTopParticle )
				{
					pCinematicTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
				}
				if ( NULL != pCinematicBottomParticle )
				{
					pCinematicBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
				}

				if ( m_fTimer >= m_fDeleteTime )			// Fade-Out �ð��� ������
				{
					Init();									// �ʱ�ȭ
				}
			}
		}
		else
#endif  // SERV_BATTLEFIELD_MIDDLE_BOSS
		{
			if(m_fTimer <= 0.5f)
			{
				float fAlpha = 1.f - (m_fTimer * 2.f);
				if ( NULL != pCinematicTopParticle )
				{
					pCinematicTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
				}
				if ( NULL != pCinematicBottomParticle )
				{
					pCinematicBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
				}
			}
			else
			{
				if( NULL != g_pData->GetUIManager() )
				{
					if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicTop )
					{
						CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicTop );
						if( pSeq != NULL )
						{
							pSeq->SetShowObject(false);
						}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                        else
                        {
                            m_hCinematicTop = INVALID_PARTICLE_SEQUENCE_HANDLE;
                            m_hCinematicTopParticle = INVALID_PARTICLE_HANDLE;
                        }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					}

					if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicBottom)
					{
						CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicBottom );
						if( pSeq != NULL )
						{
							pSeq->SetShowObject(false);
						}	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                        else
                        {
                            m_hCinematicBottom = INVALID_PARTICLE_SEQUENCE_HANDLE;
                            m_hCinematicBottomParticle = INVALID_PARTICLE_HANDLE;
                        }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					}
				}
				m_bEnd = false;
			}

		}
	}

	return S_OK;
}
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void CX2Game::CinematicUI::SetCinematicUI( bool bVal, float DeleteTime_ )
#else // SERV_BATTLEFIELD_MIDDLE_BOSS
void CX2Game::CinematicUI::SetCinematicUI(bool bVal)
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
{
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	if ( -1.f != DeleteTime_ )					// DeleteTime �� �����Ѵٸ�
	{
		Init ();							// �ʱ�ȭ
		m_fDeleteTime = DeleteTime_;
		m_bStart = true;	
		m_fTimer = 0.f;

		if( NULL != g_pData->GetUIMajorParticle() )
		{
			// ���� �߰� �������� ������ Danger ����
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicTop )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicTop );
				if( pSeq != NULL )
				{			
					pSeq->SetShowObject(bVal);
				}
			}
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicBottom )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicBottom );
				if( pSeq != NULL )
				{			
					pSeq->SetShowObject(bVal);
				}
			}
		}
	}
	else 
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
	{
		if( true == bVal )
		{
			if( false == m_bStart )
			{
				m_bStart = bVal;
				m_bEnd = !bVal;
				m_fTimer = 0.f;

				if( NULL != g_pData->GetUIMajorParticle() )
				{
					if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicTop )
					{
						CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicTop );
						if( pSeq != NULL )
						{			
							pSeq->SetShowObject(bVal);
						}
					}
					if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCinematicBottom )
					{
						CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hCinematicBottom );
						if( pSeq != NULL )
						{			
							pSeq->SetShowObject(bVal);
						}
					}
				}
			}	
		}
		else
		{
			if( true == m_bStart )
			{
				m_bStart = bVal;
				m_bEnd = !bVal;
				m_fTimer = 0.f;
			}	
		}
	}
}

void CX2Game::CanNotInputAndDisalbeNPCAI(bool bVal)
{
	if( bVal == m_bCanNotInputAndPauseNPCAI )
		return;

	m_bCanNotInputAndPauseNPCAI= bVal;

	// Ű�Է� ����
	if( NULL != GetMyUnit() )
	{
		GetMyUnit()->SetCanNotInput( m_bCanNotInputAndPauseNPCAI );
	}

	// NPC AI ��Ȱ��ȭ
	for( UINT i = 0; i < m_NPCUnitList.size(); ++i )
	{
		CX2GUNPC* pGUNPC = m_NPCUnitList[i];
		if( NULL != pGUNPC )
		{
			pGUNPC->SetAIEnable( !bVal );
		}
	}
}
#endif //MODIFY_DUNGEON_STAGING


void CX2Game::ApplyBuffFactorToGUUser( const CX2Item::ItemTemplet* pItemTemplet, CX2GUUser* pCX2GUUser )
{
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
    if (unsigned uNumPtr = pItemTemplet->GetNumBuffFactorPtr() )
    {
        for( unsigned u = 0; u != uNumPtr; ++u )
	    {
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			CX2BuffFactorPtr ptrBuffFactor = pItemTemplet->GetBuffFactorPtr( u )->GetClonePtr( 0 );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2BuffFactorPtr ptrBuffFactor = pItemTemplet->GetBuffFactorPtr( u );
#endif //UPGRADE_SKILL_SYSTEM_2013
		    if ( NULL != ptrBuffFactor )
				pCX2GUUser->SetBuffFactorToGameUnit( *ptrBuffFactor );
	    }
    }
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	BOOST_FOREACH( CX2BuffFactorPtr ptrBuffFactor, pItemTemplet->m_vecBuffFactorPtr )
	{
		if ( NULL != ptrBuffFactor )
			pCX2GUUser->SetBuffFactorToGameUnit( *ptrBuffFactor );
	}
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
}


#ifdef GUIDE_QUEST_MONSTER
/** @function : SetQuestMonster
	@brief	: �ش� NPCID�� ������ ���ʹ� ����Ʈ ���ͷ� ����
	@param	: ����Ʈ ���ͷ� ������ NPC ID(eNPCUnitID), ���� ����(bShow)
*/
void CX2Game::SetQuestMonster( CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ , bool bShow /*= true*/)
{
	BOOST_TEST_FOREACH( CX2GUNPC*, pNPC, m_NPCUnitList )
	{
		if( NULL != pNPC )
		{
			if( eNPCUnitID_ == pNPC->GetNPCTemplet().m_nNPCUnitID )
			{
				pNPC->SetQuestMonster( bShow );
			}
		}
	}
}
#endif //GUIDE_QUEST_MONSTER

#ifdef	BALANCE_GRAND_ARCHER_20121213
/** @function : GetNearUnitUidList
	@brief : ���� ������ Uid�� Ư�� ��ġ�� ����� ������ ���ĵ� ������ ���ϱ�
	@param : CX2Room::TEAM_NUM eMyTeamNum_(�ڽ��� ��Number), IN const D3DXVECTOR3& vMyPos_(���� ��ġ)
			OUT std::map<float, UidType> mapNearUnitUid_(���ĵ� UID ��)
	@return : bool ���� true, ���� false
*/
bool CX2Game::GetNearUnitUidList( IN CX2Room::TEAM_NUM eMyTeamNum_, IN const D3DXVECTOR3& vMyPos_, OUT std::map<float, UidType>& mapNearUnitUid_, bool bIgnoreDistanceLimit /*= false*/)
{
	mapNearUnitUid_.clear();
	const float fLimitNearDistance = 16000000.f;	// �Ÿ� ���� �� 4000 * 4000 

	for( UINT uIndex = 0; uIndex < m_UnitList.size(); ++uIndex )
	{
		const CX2GameUnit* pCX2GameUnit = m_UnitList[uIndex];
		
		if( pCX2GameUnit == NULL )
			continue;

		if( pCX2GameUnit->GetTeam() == eMyTeamNum_ )
			continue;

		if( true == pCX2GameUnit->GetInvincible() )
			continue;

		if( 0.f >= pCX2GameUnit->GetNowHp() )
			continue;

		if( CX2GameUnit::GUT_NPC == pCX2GameUnit->GetGameUnitType() )
		{
			const CX2GUNPC* pNPC = static_cast<const CX2GUNPC*>( pCX2GameUnit );
			switch( pNPC->GetNPCTemplet().m_ClassType )
			{
			case CX2UnitManager::NCT_THING_TRAP:
			case CX2UnitManager::NCT_THING_CHECKER:
				{
					continue;
				} break;
			} // switch
		} // if

		float fDistance = GetDistance3Sq( vMyPos_, pCX2GameUnit->GetPos() );

		if( fLimitNearDistance < fDistance &&
			false == bIgnoreDistanceLimit)
			continue;

		mapNearUnitUid_.insert( std::make_pair( fDistance, pCX2GameUnit->GetUnitUID()) );
	} // for

	if( true == mapNearUnitUid_.empty() )
		return false;

	return true;
}

#endif //BALANCE_GRAND_ARCHER_20121213

/** @function : SetNpcHardLevel
	@brief : NPC�� HardLevel ���� �κ� ����
	@param : HardLevel�� ������ NPC ������, NPC Level
*/
void CX2Game::SetNpcHardLevel( CX2GUNPC* pNpcUnit_, const int iLevel_ )
{
	if ( NULL != g_pX2Room )
	{
		switch ( g_pX2Room->GetRoomType() )
		{
		case CX2Room::RT_DUNGEON:
			{
				// note!! ���׶� �ھ������ ��Ƽ�� ���� ���� ���� HP ����
				CX2DungeonRoom* pDungeonRoom = (CX2DungeonRoom*) g_pX2Room;

#ifdef SUMMON_MONSTER_CARD_SYSTEM		//���� ī�� ��ȯ NPC�� �ɷ�ġ ���� ����

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
				if( NULL != pDungeonRoom && CX2GUNPC::NCT_NONE == pNpcUnit_->GetNPCCreateType()
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
					&& pNpcUnit_->GetNPCTemplet().m_nNPCUnitID != EVENT_MONSTER_CARD_ENEMY_ID
#endif EVENT_MONSTER_CARD_SUMMON_ENEMY
					)
	#else // SERV_NEW_DEFENCE_DUNGEON
				if( NULL != pDungeonRoom && true != pNpcUnit_->GetIsMonsterCard() )
	#endif // SERV_NEW_DEFENCE_DUNGEON

#else  SUMMON_MONSTER_CARD_SYSTEM
				if( NULL != pDungeonRoom )
#endif SUMMON_MONSTER_CARD_SYSTEM
				{
					switch( pDungeonRoom->GetDungeonID() )
					{
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
						CASE_DEFENCE_DUNGEON
						{
							pNpcUnit_->SetHardLevel( iLevel_, true, 1.f );	
						} break;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
					default:
						{
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
							if ( GetGameType() == CX2Game::GT_DUNGEON )
							{
								CX2DungeonGame* pDungeonGame = (CX2DungeonGame*)this;
								CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

								// oasis907 : ����� [2011.5.16] ���� ���̵��� ���� ü��
								switch ( g_pData->GetDungeonManager()->GetDungeonType( pDungeon->GetDungeonData()->m_DungeonID ) )
								{
								case CX2Dungeon::DT_HENIR:
									{
										pNpcUnit_->SetHardLevel( iLevel_, true, 1.f );
									} break;
								case CX2Dungeon::DT_SECRET:
									{
										pNpcUnit_->SetHardLevel( iLevel_, true, 1.f );
									} break;
								default:
									{
										switch(pDungeon->GetDungeonData()->m_eDifficulty)
										{
										case CX2Dungeon::DL_NORMAL:
											{
												pNpcUnit_->SetHardLevel( iLevel_, true, 1.0f );
											} break;
										case CX2Dungeon::DL_HARD:
											{
												pNpcUnit_->SetHardLevel( iLevel_, true, 1.2f );
											} break;
										case CX2Dungeon::DL_EXPERT:
											{
												pNpcUnit_->SetHardLevel( iLevel_, true, 1.5f );
											} break;
										default:
											{
												pNpcUnit_->SetHardLevel( iLevel_, true );
											} break;
										}
									} break;
								}
							}
#else
							pNpcUnit_->SetHardLevel( iLevel_, true );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
						} break;
					}

					// oasis907 : ����� [2011.5.16] ���� �÷��̾� �� ���� ���� �ٿ� ��ġ
					switch( pDungeonRoom->GetNumOfPlayers() )
					{
					default:
					case 1:
						{
							pNpcUnit_->SetMaxForceDown( 130.f );
						} break;
					case 2:
						{
							pNpcUnit_->SetMaxForceDown( 195.f );
						} break;
					case 3:
						{
							pNpcUnit_->SetMaxForceDown( 234.f );
						} break;
					case 4:
						{
							pNpcUnit_->SetMaxForceDown( 260.f );
						} break;
					}
				}
				else
				{
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
					const int iLevelMonsterCard 
						= ( CX2GUNPC::NCT_MONSTER_CARD == pNpcUnit_->GetNPCCreateType() 
#ifdef EVENT_MONSTER_CARD_SUMMON_ENEMY
						|| pNpcUnit_->GetNPCTemplet().m_nNPCUnitID == EVENT_MONSTER_CARD_ENEMY_ID
#endif EVENT_MONSTER_CARD_SUMMON_ENEMY
						) ? GetSummonMonsterCardNPCLevel( pNpcUnit_, iLevel_ ) : iLevel_;
	#else // SERV_NEW_DEFENCE_DUNGEON
					const int iLevelMonsterCard 
						= ( pNpcUnit_->GetIsMonsterCard() ) ? GetSummonMonsterCardNPCLevel( pNpcUnit_, iLevel_ ) : iLevel_;
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM
					pNpcUnit_->SetHardLevel( iLevelMonsterCard, true );
				}

			} break;

		default:
			{
#ifdef SUMMON_MONSTER_CARD_SYSTEM

	#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-12
				const int iLevelMonsterCard 
					= ( CX2GUNPC::NCT_MONSTER_CARD == pNpcUnit_->GetNPCCreateType() ) ? GetSummonMonsterCardNPCLevel( pNpcUnit_, iLevel_ ) : iLevel_;
	#else // SERV_NEW_DEFENCE_DUNGEON
				const int iLevelMonsterCard 
					= ( pNpcUnit_->GetIsMonsterCard() ) ? GetSummonMonsterCardNPCLevel( pNpcUnit_, iLevel_ ) : iLevel_;
	#endif // SERV_NEW_DEFENCE_DUNGEON

#endif SUMMON_MONSTER_CARD_SYSTEM
				pNpcUnit_->SetHardLevel( iLevelMonsterCard, true );
			} break;
		}
	}
}

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
/** @function : IncreaseUserPacketCountToReceive
	@brief : ������ ���� ��ũ ��Ŷ�� �޾��� �� ���� Ƚ���� ����ϴ� ī��Ʈ�� ���� ��Ű�� �Լ�
	@param : ��ũ ��Ŷ�� ���� ������ UID
*/
void CX2Game::IncreaseUserPacketCountToReceive( const UidType uidGameUnit_ )
{
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if ( NULL != g_pX2Room )
		{
			CX2Room::SlotData* pNormalSlotData = 
				g_pX2Room->GetSlotDataByUnitUID( uidGameUnit_ );
			if ( NULL != pNormalSlotData )
				pNormalSlotData->m_uiUserPacketCountToReceive++;
			else
			{
				CX2Room::SlotData* pObserverSlotData = 
					g_pX2Room->GetObserverSlotDataByUnitUID( uidGameUnit_ );
				if ( NULL != pObserverSlotData )
					pObserverSlotData->m_uiUserPacketCountToReceive++;
			}
		}
	}
}
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

#ifdef ARA_CHARACTER_BASE
void CX2Game::ProcessIKillThisNpc( const KEGS_NPC_UNIT_DIE_NOT& kEvent )
{
	if ( NULL != GetMyUnit() )
	{
		if( NULL != GetMyUnit() && 
			NULL != GetMyUnit()->GetUnit() )
		{
			if( -1 != kEvent.m_uiAttUnit && 				
				GetMyUnit()->GetUnitUID() == kEvent.m_uiAttUnit )
			{
				GetMyUnit()->DoProcessWhenKillEnemy( NULL );
			}
		}
	}
}
#endif

#ifdef FIX_ARA_AWAKE_ABILITY_BUG
void CX2Game::ProcessIKillThisUser( const KEGS_USER_UNIT_DIE_NOT& kEvent )
{	
	if ( NULL != GetMyUnit() && 
		 NULL != GetMyUnit()->GetUnit() )
	{
		if( GetMyUnit()->GetUnitUID() == kEvent.m_KillerUserUnitUID || 
			GetMyUnit()->GetUnitUID() == kEvent.m_MaxDamageKillerUserUnitUID )
		{
			GetMyUnit()->DoProcessWhenKillEnemy( NULL );
		}
	}
}
#endif // FIX_ARA_AWAKE_ABILITY_BUG

void CX2Game::ProcessGetOutlayItem( 
    const CX2Item::ItemTemplet* pItemTemplet, 
    CX2GUUser* pCX2GUUser,
									int DropItemUID, bool bIsItemSuccess )
{
#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER		/// Ư�� �������� �Ծ��� �� �̷ο� ȿ���� �ְ� ������� ���� �Ͻ��� ��ȯ ( ���Ŀ� SAT ID �߱��ؼ� �������� )
	if( NULL != pCX2GUUser )
	{
		m_vDropItemPos = pCX2GUUser->GetPos();	/// ��ġ�� ���Ե� �� �ʱ�ȭ�� �ʵǰ� �־ ��� ���� ��ġ�� �����ϴ� ���� �߻� -> ������ ��ġ�� �ʱ�ȭ

		m_pDropItemManager->GetDropItemPositionByUid(DropItemUID, m_vDropItemPos);
	}

	if ( true == ProcSpecialItem(pItemTemplet, pCX2GUUser) )
		return;
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER

	if( pItemTemplet->GetNumSpecialAbility() == 0 &&
		pItemTemplet->GetNumBuffFactorPtr() == 0 )
		return;

	if( CX2Item::IT_QICK_SLOT == pItemTemplet->GetItemType() )
		return;

#ifdef DUNGEON_ITEM

#ifndef ADD_VELDER_ACADEMY_EVENT_MONSTER		/// ProcSpecialItem �Լ��� UseItemSpecialAbility �Լ��� �ű涧 ���� ��ܿ� ��������
	m_pDropItemManager->GetDropItemPositionByUid(DropItemUID, m_vDropItemPos);
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER

	// true�̸� �Ϲݾ������̰ų� �����������̸鼭 ���� ������ ����̴�.
	bool bNormalItem = true; 
	CX2Item::SPECIAL_ITEM_TYPE eType = GetSpecialItemGroupType(pItemTemplet);

	// ����&���� ���������� �˻�
	if( eType == CX2Item::SIT_NOSTRUM || eType == CX2Item::SIT_GENIUS || eType == CX2Item::SIT_CREST )
	{
		// ���� ����Ʈ
		GetEffectSet()->PlayEffectSet( L"TakeSpecialItem1", (CX2GameUnit*) pCX2GUUser );
		GetEffectSet()->PlayEffectSet( L"TakeSpecialItem2", (CX2GameUnit*) pCX2GUUser, NULL, false, -1.f, -1.f, D3DXVECTOR3(1.f, 1.f, 1.f), true, m_vDropItemPos ); 

		// �������θ� �˻�
		bNormalItem = bIsItemSuccess;
	}

	// ����/���� ���ο� ���� ���ۿ� �ߵ�
	if( bNormalItem )
	{
        if ( pItemTemplet->GetNumBuffFactorPtr() == 0 )
			UseItemSpecialAbility( pItemTemplet, pCX2GUUser );
		else
			ApplyBuffFactorToGUUser( pItemTemplet, pCX2GUUser );
	}
	else
	{				
		switch( eType )
		{
		case CX2Item::SIT_NOSTRUM:
			{
				if( pCX2GUUser != NULL )
					pCX2GUUser->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_SIDE_EFFECT_MEDICINE ) );
			} break;

		case CX2Item::SIT_GENIUS:
			{
				if( pCX2GUUser != NULL )
					SideEffectGenius(pCX2GUUser);
			} break;

		case CX2Item::SIT_CREST:
			{
				if( pCX2GUUser != NULL )
					pCX2GUUser->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_SIDE_EFFECT_CREST ) );
			} break;

		default:
			{
		        if ( pItemTemplet->GetNumBuffFactorPtr() == 0 )
			        UseItemSpecialAbility( pItemTemplet, pCX2GUUser );
		        else
			        ApplyBuffFactorToGUUser( pItemTemplet, pCX2GUUser );
			} break;		
		}

	}			
#else
	UseItemSpecialAbility( pItemTemplet, pCX2GUUser );
#endif
}

void CX2Game::ProcessGetEquipItem( const CX2Item::ItemTemplet* pItemTemplet)
{
	if ( pItemTemplet->GetItemType() == CX2Item::IT_WEAPON ||
		pItemTemplet->GetItemType() == CX2Item::IT_DEFENCE)
	{
		if( NULL != g_pData && g_pData->GetPlayGuide() )
		{	//��� �������� ȹ�� ���� �ÿ� �κ��丮 Ȯ�� ���̵� ���
			g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_OBTAIN_EQUIP_ITEM, true, g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_INVENTORY ));
		}
	}
}
void CX2Game::ProcessGetEDItem( CX2GUUser* pCX2GUUser, bool bIsEDItem,
							KDungeonRewardED DungeonRewardED, UidType uiGetUnitUID)
{
	if( true == bIsEDItem &&
		true == pCX2GUUser->IsMyUnit() )
	{
		if( g_pChatBox != NULL && GetMyUnit() != NULL && GetMyUnit()->GetNowHp() > 0 )
		{
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
			std::wstringstream strStream;

			if ( pUnitData->m_ED >= _CONST_X2GAME_::g_iMaxED )
			{
				strStream << GET_STRING( STR_ID_2683 );
			}
			else
			{
				int iSumED		= 0;
				iSumED			= DungeonRewardED.SumED();

				pUnitData->m_ED += iSumED;

				if ( pUnitData->m_ED > _CONST_X2GAME_::g_iMaxED )
				{
					iSumED -= ( pUnitData->m_ED - _CONST_X2GAME_::g_iMaxED );
					pUnitData->m_ED = _CONST_X2GAME_::g_iMaxED;

				}

				strStream << GET_REPLACED_STRING( ( STR_ID_201, "i", iSumED ) );

				if ( DungeonRewardED.m_iBonusED > 0 )
				{
					if ( iSumED - DungeonRewardED.m_iED > 0 )
						strStream << GET_REPLACED_STRING( ( STR_ID_2625, "i", iSumED - DungeonRewardED.m_iED ) );
					else
						strStream << GET_REPLACED_STRING( ( STR_ID_2625, "i", 0 ) );
				}

				if ( iSumED > 0 && GetMyUnit()->GetUnitUID() == uiGetUnitUID )
				{
					std::wstringstream strStreamED;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					strStreamED << GET_STRING( STR_ID_14352 ) << iSumED;
#else SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
					strStreamED << L"ED +" << iSumED;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC

					D3DXVECTOR3 pos = pCX2GUUser->GetPos();
					pos.y += 50.0f;
#ifdef ALWAYS_SCREEN_SHOT_TEST
					if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() ==  false )
					{
						if( NULL != g_pData->GetPicCharYellow() )
							g_pData->GetPicCharYellow()->DrawText( strStreamED.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
					}
#else
					if( NULL != g_pData->GetPicCharYellow() )
						g_pData->GetPicCharYellow()->DrawText( strStreamED.str().c_str(), pos, pCX2GUUser->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif ALWAYS_SCREEN_SHOT_TEST
				}
				if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					g_pData->GetUIManager()->GetUIInventory()->SetEDString( pUnitData->m_ED );
				}

				if( iSumED == 0 )
					return;
			}
			if( NULL != g_pChatBox )
			{
				g_pChatBox->AddChatLogObtain(strStream.str().c_str(), CX2ChatBox::COT_ED);
			}
		}
	}
}



void CX2Game::AddChatLogByGetItem(
    const CX2Item::ItemTemplet* pItemTemplet, 
    CX2GUUser* pCX2GUUser,
								bool bIsEDItem)
{
	if( false == bIsEDItem )
	{
        const wchar_t* pwszColorCode = pItemTemplet->GetNameColor_();

		wstring wstrMsg;			
		if ( pCX2GUUser->IsMyUnit() == false )
		{
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
			// �̱��� ��,���� ���� �ʿ� ����.
			wstrMsg = GET_REPLACED_STRING( ( STR_ID_197, "SSSS",
				pCX2GUUser->GetUnit()->GetNickName(),
				pwszColorCode,
				pItemTemplet->GetFullName_(),
				L"" ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			wstrMsg = GET_REPLACED_STRING( ( STR_ID_197, "SSSI",
				pCX2GUUser->GetUnit()->GetNickName(),
				pwszColorCode,
				pItemTemplet->GetFullName_(),
				KHanSungPostWordUnicodeWrapper( (WCHAR*) pItemTemplet->GetFullName_(), STR_ID_198, STR_ID_199 ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
		}
		else
		{
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
			wstrMsg = GET_REPLACED_STRING( ( STR_ID_200, "SSS", 
				pwszColorCode,
				pItemTemplet->GetFullName_(),
				L"" ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			wstrMsg = GET_REPLACED_STRING( ( STR_ID_200, "SSI", 
				pwszColorCode,
				pItemTemplet->GetFullName_(), 
				KHanSungPostWordUnicodeWrapper( (WCHAR*) pItemTemplet->GetFullName_(), STR_ID_198, STR_ID_199 ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
		}

		if( NULL != g_pChatBox )
		{
			wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
			D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

//#ifndef NOT_USE_DICE_ROLL
//			if( true == bDiceRolling )
//			{
//				const float MAGIC_DICE_ROLLING_TIME = 1.6f;
//				g_pChatBox->AddChatLog( wstrMsg.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor, true, false, MAGIC_DICE_ROLLING_TIME );
//			}
//			else
//#endif //NOT_USE_DICE_ROLL
			{
				g_pChatBox->AddChatLog( wstrMsg.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
			}
		}
	}
}

#ifdef SERV_EVENT_VALENTINE_DUNGEON
void CX2Game::NotfiyCreateValentimeCupCake()
{
	if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() )
	{
		CX2StateDungeonGame* pState = static_cast<CX2StateDungeonGame*>(g_pMain->GetNowState());
		if( NULL != pState )
		{
			pState->NotfiyCreateValentineCupCake();
		}
	}
}
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_SHARING_BANK_TEST
wstring CX2Game::GetUserName(int inx)
{
	CX2Unit *pUnit = g_pData->GetMyUser()->GetUnitByIndex(inx);
	if(pUnit != NULL)
	{
		return pUnit->GetUnitData().m_NickName;;
	}

	return L"";
}
#endif SERV_SHARING_BANK_TEST

#ifdef NEXON_QA_CHEAT_REQ
// /wakeup �Է½� ���� ������ �ϳ��� ä����
void CX2Game::AddHyperModeCount()
{
	CX2GUUser* pMyGUUser = static_cast<CX2GUUser*>( GetMyUnit() );
	if ( NULL == pMyGUUser )
		return;

	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )				
	{
		int iGetHyperModeCount =  pMyGUUser->GetHyperModeCount();
		if ( iGetHyperModeCount == pMyGUUser->GetFrameData().syncData.m_HyperModeCount )
		{
			int iHyperModeCount = iGetHyperModeCount;
			if ( static_cast<int>( CX2GageUI::PGUWO_ORB3 ) == ++iHyperModeCount )
				iHyperModeCount = static_cast<int>( CX2GageUI::PGUWO_ORB3 );

			pMyGUUser->SetHyperModeCount( iHyperModeCount );
		}
	}
}

// /energy �Է½� �ƶ� ��� �������� �ϳ��� ä����
void CX2Game::AddAraGage()
{
	CX2GUUser* pMyGUUser = static_cast<CX2GUUser*>( GetMyUnit() );
	if ( NULL == pMyGUUser )
		return;

	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )				
	{
		if ( NULL != pMyGUUser->GetUnit() && CX2Unit::UT_ARA == pMyGUUser->GetUnit()->GetType() )
		{
			CX2AraGageData*	pAraGageData = static_cast<CX2AraGageData*>( pMyGUUser->GetGageData() );
			if ( NULL != pAraGageData )
			{
				if(pAraGageData->IsFullForcePower())
					return;
				else
				{
					int iGagePower = pAraGageData->GetNowForcePower();
					pAraGageData->SetNowForcePower( ++iGagePower );
				}
			}
		}
	}
}

// /canon �Է½� û ��ź �������� ä����
void CX2Game::AddChungCannonBall()
{
	CX2GUUser* pMyGUUser = static_cast<CX2GUUser*>( GetMyUnit() );
	if ( NULL == pMyGUUser )
		return;

	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )				
	{
		if ( NULL != pMyGUUser->GetUnit() && CX2Unit::UT_CHUNG == pMyGUUser->GetUnit()->GetType() )
		{
			CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( pMyGUUser->GetGageData() );
			if ( NULL != pChungGageData )
			{
				pChungGageData->SetNowCannonBallCount( pChungGageData->GetMaxCannonBallCount() );
			}
		}
	}
}

// /Scanon �Է½� û Ư�� ��ź �������� ä����
void CX2Game::AddChungExtraCannonBall()
{
	CX2GUUser* pMyGUUser = static_cast<CX2GUUser*>( GetMyUnit() );
	if ( NULL == pMyGUUser )
		return;

	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )				
	{
		if ( NULL != pMyGUUser->GetUnit() && CX2Unit::UT_CHUNG == pMyGUUser->GetUnit()->GetType() )
		{
			if( CX2Unit::UC_CHUNG_TACTICAL_TROOPER == pMyGUUser->GetUnitClass() )
			{
				CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( pMyGUUser->GetGageData() );
				pChungGageData->SetNowCannonBallCountEx( pChungGageData->GetMaxCannonBallCountEx() );
			}
		}
	}
}
#endif //NEXON_QA_CHEAT_REQ



#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
CX2Game::CinematicUI* CX2Game::GetCinematicUI( )
{
	return ( NULL != m_pCinematicUI ) ? m_pCinematicUI : NULL;
}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
void CX2Game::StartTcpPingCheck()
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_TCPPINGTIME_CHECK
	UINT uiSequence = KOGGamePerformanceCheck::GetInstance()->GetTcpPingCheckMgr()->SendPingCheckPacket();

	//
	KEGS_TCP_PING& kTcpPing = m_mapTcpPing[ uiSequence ];

	kTcpPing.m_uiSequence = uiSequence;
	kTcpPing.m_dwStartTime = timeGetTime();

	g_pData->GetServerProtocol()->SendPacket( EGS_TCP_PING, kTcpPing ); 
#endif//ACTIVE_KOG_GAME_PERFORMANCE_TCPPINGTIME_CHECK	
}

void CX2Game::Handler_EGS_TCP_PING(  KEGS_TCP_PING& kEvent )
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_TCPPINGTIME_CHECK
	KTcpPingMap::iterator it = m_mapTcpPing.find( kEvent.m_uiSequence );
	if( it != m_mapTcpPing.end() )
	{
		KOGGamePerformanceCheck::GetInstance()->GetTcpPingCheckMgr()->ReceivePingCheckPacket( timeGetTime() - it->second.m_dwStartTime, kEvent.m_uiSequence );
		m_mapTcpPing.erase( it );
	}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_TCPPINGTIME_CHECK	
}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ADDED_GET_SUBSTAGE_INDEX_IN_SCRIPT
int CX2Game::GetNowSubStageIndex()
{
	if( GetGameType() != GT_DUNGEON )
		return false;

	CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(this);
	if( NULL != pDungeonGame )
	{
		CX2Dungeon* pDungeon = pDungeonGame->GetDungeon();

		if( NULL != pDungeon && NULL != pDungeon->GetNowStage() &&
		    NULL != pDungeon->GetNowStage()->GetNowSubStage() )
		{
			return pDungeon->GetNowStage()->GetNowSubStageIndex();
		}
	}

	return -1;
}
#endif // ADDED_GET_SUBSTAGE_NUMBER_IN_SCRIPT


#ifdef ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR

void CX2Game::DeleteRemainingNpcWhenSubStageClear ()
{
	if( GetGameType() != GT_DUNGEON )
		return ;

	for( int i = 0; i < static_cast<int>(m_NPCUnitList.size()); ++i )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( NULL == pCX2GUNPC )
			continue;

		if ( true == pCX2GUNPC->GetIsRemainingNpcWhenStageCleard() )
		{
			pCX2GUNPC->SetUserGrapReset();

			vector<CX2GameUnit*>::iterator vItrGameUnit = m_UnitList.begin();
			while ( m_UnitList.end() != vItrGameUnit )
			{
				CX2GameUnit* pGameUnit = *vItrGameUnit;
				if ( pCX2GUNPC == pGameUnit )
				{
					pCX2GUNPC->ResetGameUnitWhoAttackedMe();
					DeleteNPCUnitByUID( static_cast<const UINT> ( pCX2GUNPC->GetUnitUID() ) );
					break;
				}
				else
					++vItrGameUnit;
			}

			m_NPCUnitList[i] = NULL;

			if ( null == m_optrFocusUnit )
				ChangeFocusUnit();

		}
	}
}
#endif // ADDED_NPC_REMAINING_WHEN_SUBSTAGE_CLEAR

#ifdef NEW_CHARACTER_EL
/** @function : GetIsExsitancePartyMemberHPLessThanPer
	@brief : ü�� @1 % ������ ��Ƽ���� ���� ���� üũ
	@param : fHPRate_ (üũ�� HP ����), pGUUserWhoCallFunction_ (�� �Լ��� ȣ���� CX2GUUser �����ͷ� üũ ��󿡼� ���� �� ���� ��Ƽ���� �˾Ƴ� �뵵)
*/
/*virtual*/ 
bool CX2Game::GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const
{
	if ( NULL != g_pData->GetPartyManager() )
	{
		BOOST_FOREACH( CX2GUUser* pGUUser, m_UserUnitList )
		{
			if ( NULL != pGUUser &&	
				pGUUser->GetUnitUID() != pGUUserWhoCallFunction_->GetUnitUID() )
			{
				const float fHpPercent = 
					pGUUser->GetNowHp() / pGUUser->GetMaxHp();

				if( fHpPercent > 0.f &&
					fHpPercent <= fHPRate_ )
					return true;
			}
		}
	}
	return false;
}
#endif // NEW_CHARACTER_EL


#ifdef EFFECT_TOOL
void CX2Game::RefreshDamageEffectScript()
{
	//DamageEffect
	SAFE_DELETE( m_pDamageEffect );
	m_pDamageEffect	= new CX2DamageEffect();
	m_pDamageEffect->OpenScriptFile( L"DamageEffect.lua" );
}
#endif //EFFECT_TOOL

// ������ ���� ���� UnitUID �� ������ ���� ����� NPC �� ������
// ������ -1 �� �����ϸ� ���� üũ�� ���� �ʰ� ��� NPC �� �˻���
// MyTeam �� �����ϸ� �ش� Team �� ������ �˻����� ����
// Check Invincible �� �����ϸ� ���� ���¸� �˻����� ����
// ����, ����ε� ī�� NPC ���� �Լ�, kimjh
CX2GUNPC*	CX2Game::GetNearestNpcInSpecificRangeAndExceptUnitUID( const D3DXVECTOR3& vPos_, const UidType UnitUid_, const float fMaxRange_ = 1000, const int iMyTeam_ = -1, const bool bCheckInvincible_ = false )
{
	CX2GUNPC* pNearestNpc = NULL;
	float fDistance3Sq = fMaxRange_ * fMaxRange_;

	for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = m_NPCUnitList[i];
		if( NULL == pNPC )
			continue;
		
		if ( true == bCheckInvincible_ && true == pNPC->GetInvincible() )
			continue;

		if ( 0 <= iMyTeam_ && pNPC->GetTeam() == iMyTeam_ )
			continue;

		if( pNPC->GetUnitUID() != static_cast<UidType> ( UnitUid_) && 
			pNPC->GetNowHp() > 0.f && CX2GameUnit::GUSI_DIE != pNPC->GetGameUnitState() ) /*!pNPC->GetDyingStart())*/
		{
			float fTempDistance3Sq = GetDistance3Sq( vPos_, pNPC->GetPos() );
			if( fTempDistance3Sq < fDistance3Sq )
			{
				pNearestNpc		= pNPC;
				fDistance3Sq	= fTempDistance3Sq;
			}
		}
	}

	return pNearestNpc;
}

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
/** @function	: SetOfficiallyDebuffList
	@brief		: �طο� ȿ����� ���ǵ� �������� ����� ����Ʈ ( ��ȹ ��Ʈ���� ���� )
*/
void CX2Game::SetOfficiallyDebuffList()
{
	m_vecPossibleOfficiallyDebuffList.clear();

	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_FIRE );					/// ȭ��
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_POISON );				/// �ߵ�
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_SHOCK );				/// ���� ( ��ũ )
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_STUN );					/// ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_COLD );					/// ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_FROZEN );				/// ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_WOUND );				/// ��ó (���ٴ�)
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_BLEEDING );				/// ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_MOTION_SLOWDOWN );		/// ���� ��ȭ (�ο�ű ����)
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_MOVE_JUMP_SLOWDOWN );	/// �̵� ��ȭ
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_BLIND );				/// �þ� ���� (���߷� ����, ���� ���� ����)
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_FLASH_BANG );			/// ����ź (�Ϸ�Ʈ�� ���� �÷���)
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_PRESS );				/// ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_BLIND_SMOKE );			/// ���� (ȸ�� ����)
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_BIND );					/// ���� ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_CURSE );				/// ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_CONFUSION );			/// ȥ��
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_PANIC );				/// ��Ȳ (�ǵ���Ͽ�)
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_ARMOR_BREAK );			/// ���� �ı�
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_MAGIC_DEFFENCE_DROP );	/// ������ �罽
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_WEAPON_BREAK );			/// �� �ı�
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_ENTANGLE );				/// ���ʱ�
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_LEG_SPASM );			/// �ٸ� �λ�
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_DEATH_SENTENCE );		/// ������ ����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_AGING );				/// ��ȭ
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_STIGMA );				/// ������ ���
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_MARK_OF_COMMANDER );	/// ���ְ��� ǥ��
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_WATER_HOLD );			/// ���� ���
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_SIDE_EFFECT_MEDICINE );	/// ��� ( ���� ���ۿ� )
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_SIDE_EFFECT_CREST );	/// ����� ���� ( ���� ���ۿ� )
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_ANGER_OF_DRYAD );		/// ����̾���� �г�
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_SUPPRESSION );			/// ����
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_STIGMA_OF_FIRE );		/// �����ý�(����¡ ��Ʈ) - �Ҳ��� ���� �����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_CHANGE_PHYSIC_DEFENCE_AND_PHYSIC_DAMAGE );		/// �����ý�(�׷��� ������) - ���� �����, ���� ���ݷ� ���� / ���� ���� ���� 
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_UNEXTINGUISHABLE_FIRE );/// �����ý�(����¡ ��Ʈ) - ������ �ʴ� �Ҳ� �����
	m_vecPossibleOfficiallyDebuffList.push_back( BTI_DEBUFF_HIGH_FEVER );			/// �ҼӼ� ������ ���ҽ�Ű�� �����.
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
}

/** @function	: DisplayAddHPInformation
	@brief		: HP ȸ���� ǥ��
	@param		: HP ȸ����, ���� ��ġ, ���� ���� ����, �ۼ�Ʈ ǥ�� ����
*/
void CX2Game::DisplayAddHPInformation( IN const float fAddHPValue_, IN const D3DXVECTOR3 vPos_, IN const D3DXVECTOR3 vDirVector_, IN bool bDisplayPercent/*= true*/ )
{
	/// HP ȸ�� ǥ��� �� �� ���� ǥ���Ѵ�.
	D3DXVECTOR3 vPos = vPos_;
	vPos.y += 50.f;

	/// ȸ���� ǥ��
	WCHAR wszText[64] = L"";

	if ( true == bDisplayPercent )	/// �ۼ�Ʈ ǥ��
		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"HP+ %d PERCENT", static_cast<int>( fAddHPValue_ ) );
	else							/// ȸ���� ǥ��
		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"HP+ %d", static_cast<int>( fAddHPValue_ ) );

	if( NULL != g_pData && NULL != g_pData->GetPicCharBlue() )
		g_pData->GetPicCharBlue()->DrawText( wszText, vPos, vDirVector_, CKTDGPicChar::AT_CENTER );
} 

/** @function	: DisplayAddHPInformation
	@brief		: MP ȸ���� ǥ��
	@param		: MP ȸ����, ���� ��ġ, ���� ���� ����
*/
void CX2Game::DisplayAddMPInformation( IN const float fAddMPValue_, IN const D3DXVECTOR3 vPos_, IN const D3DXVECTOR3 vDirVector_ )
{
	/// ȸ���� ǥ��
	WCHAR wszText[64] = L"";
	StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", static_cast<int>( fAddMPValue_ ) );

	if( NULL != g_pData && NULL != g_pData->GetPicCharBlue() )
		g_pData->GetPicCharBlue()->DrawText( wszText, vPos_, vDirVector_, CKTDGPicChar::AT_CENTER );
}
#endif // HAMEL_SECRET_DUNGEON

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
/** @function	: DisplayDamageInformation
	@brief		: ������ ǥ��
	@param		: ��������, ���� ��ġ, ���� ���� ����
*/
void CX2Game::DisplayDamageInformation( IN const float fDamageValue_, IN const D3DXVECTOR3 vHeadBonePos_, IN const D3DXVECTOR3 vDirVector_ )
{
	/// ������ ��� ��ġ ����
	D3DXVECTOR3 vPos = vHeadBonePos_;
	//vPos.y += 70.0f;

	/// ȸ���� ǥ��
	if( NULL != g_pData && NULL != g_pData->GetPicChar() )
		g_pData->GetPicChar()->DrawText( static_cast<int>( fDamageValue_ ), vPos, vDirVector_, CKTDGPicChar::AT_CENTER );
}
#endif //SERV_9TH_NEW_CHARACTER

#ifdef MODFIY_LOG_IN_NPC_SCRIPT_FUNCTION
void CX2Game::ChatBoxLog( const CHAR* pLog, float fLog)
{
#ifdef _IN_HOUSE_
	if( NULL != g_pChatBox )
	{
		wstring wstrStateName = L"";
		ConvertUtf8ToWCHAR( wstrStateName, pLog );

		g_pChatBox->AddChatLog( wstrStateName.c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0.5882f, 1, 0, 1), L"#C96ff00", false );

		if( false == IsSamef(0.f,fLog))
		{
			WCHAR wBuf1[10];
			StringCchPrintfW( wBuf1, 10, L"%f", fLog );
			g_pChatBox->AddChatLog( wBuf1, KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(0.5882f, 1, 0, 1), L"#C96ff00", false );
		}
	}
#else
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�系���� ���� �Լ��� �����Ǿ��ֽ��ϴ�.\nNPC��ũ��Ʈ�� ChatBoxLog()�� �������ּ���", g_pMain->GetNowState() );
#endif _IN_HOUSE_
}
#endif // MODFIY_LOG_IN_NPC_SCRIPT_FUNCTION

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

void CX2Game::FlushSendFrameAverage()
{
#ifdef	ELOG_STATISTICS_NEW_DATA

    int iGameType = GetGameType();
#ifdef FIELD_BOSS_RAID // ���̵庸���� ������ ��� �������� �����
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		iGameType = GT_DUNGEON;
	}
#endif  // FIELD_BOSS_RAID

	m_kGameStatistics.Send_EGS_FRAME_AVERAGE_REQ( iGameType );
    m_kGameStatistics.Init();
#else	ELOG_STATISTICS_NEW_DATA
	KEGS_FRAME_AVERAGE_REQ kPacket;
	kPacket.m_cGameType		= iGameType;
	kPacket.m_iFrameAverage = m_AverageRenderFrameRate;

	g_pData->GetServerProtocol()->SendPacket( EGS_FRAME_AVERAGE_REQ, kPacket );
#endif	ELOG_STATISTICS_NEW_DATA
}

#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

void CX2Game::SendFrameAverage()
{
#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	// �̹� ������ ���� �ߴٸ�, ���̻� ������ �ʱ�
	if( true == GetAlreadySendingFrame() )
		return;
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

    int iGameType = GetGameType();
#ifdef FIELD_BOSS_RAID // ���̵庸���� ������ ��� �������� �����
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		iGameType = GT_DUNGEON;
	}
#endif  // FIELD_BOSS_RAID

#ifdef	ELOG_STATISTICS_NEW_DATA
	m_kGameStatistics.Send_EGS_FRAME_AVERAGE_REQ( iGameType );
#else	ELOG_STATISTICS_NEW_DATA
	KEGS_FRAME_AVERAGE_REQ kPacket;
	kPacket.m_cGameType		= iGameType;
	kPacket.m_iFrameAverage = m_AverageRenderFrameRate;

	g_pData->GetServerProtocol()->SendPacket( EGS_FRAME_AVERAGE_REQ, kPacket );
#endif	ELOG_STATISTICS_NEW_DATA

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	// EGS_FRAME_AVERAGE_ACK�� ���� ó���� ���� ������ REQ���� �� true�� ����
	SetAlreadySendingFrame(true);
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
}

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef SERV_CODE_EVENT
bool CX2Game::IsEnableCodeEvent_LUA( int iScriptID )
{
	IF_EVENT_ENABLED( iScriptID )
	{
		return true;
	}

	return false;
}
#endif SERV_CODE_EVENT

#ifdef NEAREST_NPC_ON_SAME_DIRECTION
D3DXVECTOR3 CX2Game::GetNearestNPCUnitPosOnSameDirection_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange, D3DXVECTOR3 vDir )
{
	D3DXVECTOR3 retPos(0, 0, 0);
	float fDistance = fMaxRange;

	// �¿� ���⸸ ���� ���̹Ƿ� y ����
	vDir.y = 0;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( true == pNPC->GetInvincible() )
			continue;

		if( 0.f >= pNPC->GetNowHp() )
			continue;

		if( pNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
			continue;

		// ������ �� ������ ��Ƽ��
		D3DXVECTOR3 vDirB = pNPC->GetPos() - pos;
		vDirB.y = 0;

		if( D3DXVec3Dot( &vDir, &vDirB ) < 0.f )
		{
			continue;
		}

		float fTempDist = GetDistance( pos, pNPC->GetPos() );
		if( fTempDist < fDistance && fTempDist > fMinRange )
		{
			retPos = pNPC->GetPos();
			fDistance = fTempDist;
		}
	}
	return retPos;
}
CX2GameUnit* CX2Game::GetNearestNPCUnitOnSameDirection_LUA( D3DXVECTOR3 pos, float fMinRange, float fMaxRange, D3DXVECTOR3 vDir )
{
	CX2GameUnit* retUnit = NULL;
	float fDistance = fMaxRange;

	// �¿� ���⸸ ���� ���̹Ƿ� y ����
	vDir.y = 0;

	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pNPC = (CX2GUNPC*)m_NPCUnitList[i];
		if( pNPC == NULL )
			continue;

		if( true == pNPC->GetInvincible() )
			continue;

		if( 0.f >= pNPC->GetNowHp() )
			continue;

		if( pNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
			continue;

		// ���������� ��NPC ���� Ÿ���ϱ� ���� ����̹Ƿ� �߰��մϴ�. by ������ 2014.01.27
		if( pNPC->GetTeam() != CX2Room::TN_MONSTER )
			continue;

		// ������ �� ������ ��Ƽ��
		D3DXVECTOR3 vDirB = pNPC->GetPos() - pos;
		vDirB.y = 0;

		if( D3DXVec3Dot( &vDir, &vDirB ) < 0.f )
		{
			continue;
		}

		float fTempDist = GetDistance( pos, pNPC->GetPos() );
		if( fTempDist < fDistance && fTempDist > fMinRange )
		{
			retUnit = pNPC;
			fDistance = fTempDist;
		}
	}
	return retUnit;
}
#endif //NEAREST_NPC_ON_SAME_DIRECTION

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
void CX2Game::SetValentineEventTrigger_LUA( D3DXVECTOR3 vPos )
{
	//�־��� ��ǥ�κ��� ���� ����� ���� �ε����� ã�� �ش� ���ο� �̵� �Ӽ��� �ο��ϴ� �Լ�

	CKTDGLineMap* pLineMap = m_pWorld->GetLineMap();

	if( pLineMap != NULL )
	{
		int iLineIndex = pLineMap->GetNearestLine( vPos );
	
		if( iLineIndex >= 0 )
		{
			CKTDGLineMap::LineData* pLineData = pLineMap->AccessLineData( iLineIndex );	// ������ �̷��� ���� �ȵǴµ� �Ф� �� �ڵ� ���� �������� ������.

			ASSERT( NULL != pLineData );
			if( NULL == pLineData )
				return; 

			pLineData->m_fSpeed = 470;
		}
	}
}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT