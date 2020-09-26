#include "StdAfx.h"
#include "./X2BattleFieldGame.h"

CX2BattleFieldGame::CX2BattleFieldGame()
: m_pBattleFieldRoom( NULL )
, m_pDlgBattleFieldName( NULL )
, m_TimerForSendingPositionInfo( 10.0f )
, m_iMAX_NUM_OF_NPC( 12 )
, m_uiWorldId( 0 )
, m_bIsNpcLoadComplete(false)
#ifdef QUEST_GUIDE
, m_pNPCIndicator(NULL)
#endif
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
, m_pMonsterIndicator( NULL )			// �ʵ� �ߺ� Indicator, ���� Ȯ�� ����
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef SERV_CHECK_EXIST_MONSTER_UID
, m_ElapsedTimeSinceGettingFirstNpcSyncPacket( 10.0f )
, m_bCanCheckGettingFirstNpcSyncPacket( false )
#endif // SERV_CHECK_EXIST_MONSTER_UID
#ifdef FIELD_BOSS_RAID
, m_hRaidGateParticle( INVALID_PARTICLE_SEQUENCE_HANDLE )
, m_hRaidPortalParticle( INVALID_PARTICLE_SEQUENCE_HANDLE )
, m_pDLGLoadingState( NULL )
, m_fLoadUIReaminTime(0.f)
#endif // FIELD_BOSS_RAID

{
	m_GameType = CX2Game::GT_BATTLE_FIELD;

	m_pDLGContinue	= new CKTDGUIDialog( this, L"DLG_Dungeon_Game_Continue_New.lua" );
	m_pDLGContinue->SetShow( false );
	m_pDLGContinue->SetEnable( false );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGContinue );
	m_pSoundContinue			= g_pKTDXApp->GetDeviceManager()->OpenSound( L"Continue.ogg" );

#ifdef QUEST_GUIDE
	m_pNPCIndicator = new CX2NPCIndicator();
#endif //QUEST_GUIDE

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_hBossDanger = INVALID_PARTICLE_SEQUENCE_HANDLE;			// �ʵ� �ߺ� ���� ���� ����Ʈ �ڵ�
	m_hBossDangerAdd = INVALID_PARTICLE_SEQUENCE_HANDLE;		// �ʵ� �ߺ� ���� ���� ����Ʈ �ڵ�
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	CX2BossRaidManager::GetInstance()->SetRaidFieldClearLimitTime(0.f);
	CX2BossRaidManager::GetInstance()->SetSendIntrudeRestrictionNot(false);

	// ���̵� ����Ʈ ����Ʈ ��ƼŬ �ڵ�
	BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequenceHandle hRaidGateEffectParticle, m_vecRaidGateEffectParticle )
	{
		hRaidGateEffectParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
#endif // FIELD_BOSS_RAID

//#ifdef SERV_COMMON_AUTO_OPEN_HARD_CODE
	m_bOpenPotalParticle = true;
//#endif //SERV_COMMON_AUTO_OPEN_HARD_CODE
}

/*virtual*/ CX2BattleFieldGame::~CX2BattleFieldGame()
{
	/*
	Release() ���� ����
	*/
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
/*virtual*/ void CX2BattleFieldGame::Release()
{
	SAFE_DELETE_DIALOG( m_pDlgBattleFieldName );
	SAFE_DELETE_DIALOG( m_pDLGContinue );
	SAFE_CLOSE( m_pSoundContinue );

#ifdef QUEST_GUIDE
	SAFE_DELETE(m_pNPCIndicator);
#endif

//#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
//	g_pData->GetUnitManager()->UnloadAllNPCInitData();
//#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// ���� Handle ����
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hBossDanger )
		GetMinorParticle()->DestroyInstanceHandle( m_hBossDanger );
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hBossDangerAdd )
		GetMinorParticle()->DestroyInstanceHandle( m_hBossDangerAdd );
	SAFE_DELETE( m_pMonsterIndicator );
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

	// ������ // 2013-08-22 // �ʵ� ����Ǹ� �ӽ��κ��丮 �����ϵ��� ����.
	g_pData->GetUIManager()->DestoryUITempInventory();

#ifdef FIELD_BOSS_RAID
	CX2GageManager::GetInstance()->ClearBossGageUIList();
	DestoryRaidPotalParticle();
	SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#endif // FIELD_BOSS_RAID

	CX2Game::Release();	/// ���� Ŭ������ �Ҹ��ڰ� ���߿� ȣ��
}

/*virtual*/ HRESULT CX2BattleFieldGame::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2Game::OnFrameMove(fTime, fElapsedTime );

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::deque<CNpcCreatedInfoPtr>& conNpcCreatedInfo = m_deqNpcCreatedInfo;
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::vector<CNpcCreatedInfoPtr>& conNpcCreatedInfo = m_vecNpcCreatedInfo;
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

	if ( false == conNpcCreatedInfo.empty() )
 	{
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		if ( 
	#ifdef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
			g_pKTDXApp->IsFirstFrameOfSimulationLoop() == true &&
	#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
			m_UnitLoader.IsNPCUnitUIDResourceLoadCompleted( conNpcCreatedInfo.front()->GetNpcUid() ) == true )
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		{
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
			CreateBattleFieldNpcByCreatedInfo( conNpcCreatedInfo.front() );
			conNpcCreatedInfo.pop_front();
	#ifndef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
			g_pKTDXApp->SkipFrame();
	#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
			CreateBattleFieldNpcByCreatedInfo( conNpcCreatedInfo.back() );
			conNpcCreatedInfo.pop_back();
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

			if ( conNpcCreatedInfo.empty() )
			{
#ifndef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
				ClearKAttribEnchantNpcInfoMap();
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

				CX2StateBattleField* pState = static_cast<CX2StateBattleField*>(g_pMain->GetNowState());
				if( NULL != pState )
					pState->Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ();
			}
		}
 	}

	if( m_GameState == GS_PLAY )
	{
		if( false == GetGameEnd() )
		{
			ResurrectionFrameMove( fTime, fElapsedTime );

			// 10�� ���� �ѹ��� ��Ŷ ����
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_TimerForSendingPositionInfo.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_TimerForSendingPositionInfo.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if ( m_TimerForSendingPositionInfo.CheckAndResetElapsedTime() )
			{
				m_kGameStatistics.GatherPeriodicStatistics();	/// ������ ����
				Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT();

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
				CX2Game::StartTcpPingCheck();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			}
		}

#ifdef SERV_CHECK_EXIST_MONSTER_UID
		if ( GetCanCheckGettingFirstNpcSyncPacket() )	/// ù ��ũ ��Ŷ�� �޾Ҵ����� üũ�ؾ� �ϴ��� ����
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_ElapsedTimeSinceGettingFirstNpcSyncPacket.OnFrameMove( fElapsedTime );	/// üũ Ÿ�̹��� ����ϱ� ���� ElapsedTime�� OnFrameMove
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_ElapsedTimeSinceGettingFirstNpcSyncPacket.OnFrameMove();	/// üũ Ÿ�̹��� ����ϱ� ���� ElapsedTime�� OnFrameMove
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if ( m_ElapsedTimeSinceGettingFirstNpcSyncPacket.CheckElapsedTime() )	/// üũ �� Ÿ�̹� ����
			{
				// üũ �� Send!
				CheckNpcNotReceiveFirstSyncPacketAndRequestToCheckNpcUidList();
				/// ���� ������ �ٸ� �ʵ忡 ���� �ϴ� ������ ���ؼ� 
				/// �ʵ� NPC�� ù ��ũ ��Ŷ�� �ޱ� ������ üũ �� �ʿ� ����
				SetCanCheckGettingFirstNpcSyncPacket( false );	
			}
		}
#endif // SERV_CHECK_EXIST_MONSTER_UID
	}

#ifdef QUEST_GUIDE
	if( NULL != m_pNPCIndicator )
	{
		m_pNPCIndicator->OnFrameMove();
	}
#endif



#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// �ʵ� �ߺ��� ���� Indicator �߰�
	if( NULL != m_pMonsterIndicator )
	{
		if (null != m_pMonsterIndicator->GetTargetNPC() )
			m_pMonsterIndicator->OnFrameMove();
		else
			SAFE_DELETE ( m_pMonsterIndicator );
	}

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	OnFrameMove_LoadingUI(fTime, fElapsedTime );
#endif // FIELD_BOSS_RAID

	return S_OK;
}

/*virtual*/ HRESULT CX2BattleFieldGame::OnFrameRender()
{

#ifdef FIELD_BOSS_RAID
	if( true == GetShowLoadUi() )
	{
		// ���� �̵��� �ε�ȭ�� ���
		if ( m_pDLGLoadingState != NULL )
			m_pDLGLoadingState->OnFrameRender();
	}
	else
#endif // FIELD_BOSS_RAID
	{
		CX2Game::OnFrameRender();
	}

	return S_OK;
}

/*virtual*/ void CX2BattleFieldGame::GameLoading( CX2Room* pRoom )
{
	m_pBattleFieldRoom		= static_cast<CX2BattleFieldRoom*>( pRoom );
	CX2Game::GameLoading( pRoom );
	m_GameState				= GS_LOADING;
	
	if ( NULL != GetMyUnit() )
	{
		GetMyUnit()->SetForceInvincible( 3.0f );
		GetMyUnit()->SetShowInvincible( 3.0f );
	}

	if( NULL == g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap() )
	{
		g_pInstanceData->GetMiniMapUI()->CreateDungeonMiniMap();
	}

	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );
		g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->SetEyeDistance( 2000.f );
	}

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
	if ( g_pKTDXApp->GetDeviceManager() != NULL )
		g_pKTDXApp->GetDeviceManager()->ReleaseAllMemoryBuffers();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// �ʵ� �ߺ��� ���� Indicator �߰�
	SAFE_DELETE( m_pMonsterIndicator );
	m_pMonsterIndicator = NULL;
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


}

/*virtual*/ void CX2BattleFieldGame::WorldLoading()
{
	SAFE_DELETE( m_pWorld );

	const UINT uiBattleFieldId = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

	ASSERT( SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= uiBattleFieldId && SEnum::VMI_BATTLE_FIELD_END > uiBattleFieldId );

	const UINT uiWorldId = g_pData->GetBattleFieldManager().GetWorldIdByBattleFieldId( uiBattleFieldId );

	ASSERT( CX2World::WI_NONE < uiWorldId && CX2World::WI_END > uiWorldId );
	SetWorldId( uiWorldId );

	m_pWorld = g_pData->GetWorldManager()->CreateWorld( static_cast<CX2World::WORLD_ID>( uiWorldId ), NULL, true );

	CreatePotalParticle();
	OpenBattleFieldName();
#ifdef FIELD_BOSS_RAID // ���̵� �ʵ� �Ա� ����Ʈ ����	
	CreateRaidPotal();
#endif // FIELD_BOSS_RAID

}

// void CX2BattleFieldGame::CreateNpcFromDungeonIntrudeSyncPacket(	const KEGS_DUNGEON_INTRUDE_SYNC_NOT& kPacket_ )
// {
// 	for( int i = 0; i < (int)kPacket_.m_vecNPCData.size(); i++ )
// 	{
// 		const KNPCUnitReq& kNPCUnitReq = kPacket_.m_vecNPCData[i];
// 
// 		// �� �Լ��� KNPCUnitReq ��ü�� �ѱ⵵�� �����ϴ°� ������
// 		CreateNPC( (CX2UnitManager::NPC_UNIT_ID)kNPCUnitReq.m_NPCID, 
// 			kNPCUnitReq.m_Level,
// 			kNPCUnitReq.m_bActive,
// 			kNPCUnitReq.m_UID, 
// 			D3DXVECTOR3( kNPCUnitReq.m_vPos.x, kNPCUnitReq.m_vPos.y, kNPCUnitReq.m_vPos.z ), 
// 			kNPCUnitReq.m_bIsRight,
// 			kNPCUnitReq.m_fDelayTime,
// 			(int) kNPCUnitReq.m_KeyCode, 
// 			(CX2Room::TEAM_NUM) kNPCUnitReq.m_cTeamNum,
// 			(CX2NPCAI::NPC_AI_TYPE) kNPCUnitReq.m_cAIType,
// 			kNPCUnitReq.m_iAllyUID );
// 	}
// }

void CX2BattleFieldGame::CreatePotalParticle()
{
	if( NULL != m_pWorld &&
		NULL != m_pWorld->GetLineMap() )
	{
		int iNumOfLineData = m_pWorld->GetLineMap()->GetNumLineData();

		for( int i=0; i < iNumOfLineData; i++ )
		{
			const CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( i );
			if( NULL != pLineData &&
				CKTDGLineMap::LT_POTAL == pLineData->lineType )
			{
#ifdef SERV_ALTERA_AUTO_OPEN_HARD_CODE
				IF_EVENT_ENABLED( CEI_ALTERA_AUTO_OPEN_HARD_CODE )
				{
					// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
				}
				ELSE
				{
					if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_BESMA_FIELD_02 &&
						pLineData->lineIndex == 200 )
					{
						m_bOpenPotalParticle = false;
						continue;
					}
				}
#endif //SERV_ALTERA_AUTO_OPEN_HARD_CODE
#ifdef SERV_PEITA_AUTO_OPEN_HARD_CODE
				IF_EVENT_ENABLED( CEI_PEITA_AUTO_OPEN_HARD_CODE )
				{
					// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
				}
				ELSE
				{
					if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_BESMA_FIELD_01 &&
						pLineData->lineIndex == 16 )
					{
						m_bOpenPotalParticle = false;
						continue;
					}
				}
#endif //SERV_PEITA_AUTO_OPEN_HARD_CODE
#ifdef SERV_VELDER_AUTO_OPEN_HARD_CODE
				IF_EVENT_ENABLED( CEI_VELDER_AUTO_OPEN_HARD_CODE )
				{
					// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
				}
				ELSE
				{
					if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_PEITA_FIELD_02 &&
						pLineData->lineIndex == 24 )
					{
						m_bOpenPotalParticle = false;
						continue;
					}
				}
#endif //SERV_VELDER_AUTO_OPEN_HARD_CODE
#ifdef SERV_HAMEL_AUTO_OPEN_HARD_CODE
				IF_EVENT_ENABLED( CEI_HAMEL_AUTO_OPEN_HARD_CODE )
				{
					// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
				}
				ELSE
				{
					if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_VELDER_FIELD_03 &&
						pLineData->lineIndex == 190 )
					{
						m_bOpenPotalParticle = false;
						continue;
					}
				}
#endif //SERV_HAMEL_AUTO_OPEN_HARD_CODE
#ifdef SERV_SANDER_AUTO_OPEN_HARD_CODE
				IF_EVENT_ENABLED( CEI_SANDER_AUTO_OPEN_HARD_CODE )
				{
					// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
				}
				ELSE
				{
					if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_HAMEL_FIELD_03 &&
						pLineData->lineIndex == 170 )
					{
						m_bOpenPotalParticle = false;
						continue;
					}
				}
#endif //SERV_SANDER_AUTO_OPEN_HARD_CODE
				CX2WorldObjectParticle* pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), "Peita_Teleport_MagicSquare01" );
				if( NULL != pParticle )
				{
					pParticle->SetParticlePos( ( pLineData->startPos + pLineData->endPos ) * 0.5f + D3DXVECTOR3(0, 3, 0 ) );
					pParticle->SetLayer( XL_EFFECT_0 );
				}
			}
		}
	}
}

#ifdef SERV_COMMON_AUTO_OPEN_HARD_CODE
void CX2BattleFieldGame::CreatePotalParticle( const int& iLineIdx ) const
{
	if( NULL != m_pWorld &&
		NULL != m_pWorld->GetLineMap() )
	{
		int iNumOfLineData = m_pWorld->GetLineMap()->GetNumLineData();

		const CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( iLineIdx );
		if( NULL != pLineData &&
			CKTDGLineMap::LT_POTAL == pLineData->lineType )
		{
			CX2WorldObjectParticle* pParticle = m_pWorld->CreateObjectParticle( g_pX2Game->GetMajorParticle(), "Peita_Teleport_MagicSquare01" );
			if( NULL != pParticle )
			{
				pParticle->SetParticlePos( ( pLineData->startPos + pLineData->endPos ) * 0.5f + D3DXVECTOR3(0, 3, 0 ) );
				pParticle->SetLayer( XL_EFFECT_0 );
			}
		}
	}
}
#endif // SERV_COMMON_AUTO_OPEN_HARD_CODE

void CX2BattleFieldGame::SwapWorld()
{
	const UINT uiBattleFieldId = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();
	ASSERT( SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= uiBattleFieldId && SEnum::VMI_BATTLE_FIELD_END > uiBattleFieldId );

	const UINT uiWorldId = g_pData->GetBattleFieldManager().GetWorldIdByBattleFieldId( uiBattleFieldId );
	ASSERT( CX2World::WI_NONE < uiWorldId && CX2World::WI_END > uiWorldId );

	// �̵��� �ʵ��� WorldID�� ������ �ٸ��ٸ�
	//if ( GetWorldId() != uiWorldId )
	{
		SetWorldId( uiWorldId );
		CX2World* pNewWorld = g_pData->GetWorldManager()->CreateWorld( static_cast<CX2World::WORLD_ID>( uiWorldId ), NULL, true );

		std::swap( m_pWorld, pNewWorld );

		SAFE_DELETE( pNewWorld );

		CreatePotalParticle();

#ifdef FIELD_BOSS_RAID 
		// ������ ����Ʈ �ʱ�ȭ
		if( NULL != GetDamageEffect() )
		{
			GetDamageEffect()->DestroyAllInstances();
		}

		// �ó׸�ƽ UI �ʱ�ȭ
		CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
		if( NULL != pStateMenu )
		{
			pStateMenu->SetShowUI(true);
		}

		// ���̵� �ʵ� �Ա� ����Ʈ ����
		CreateRaidPotal();

		//���������� UI ����
		CX2GageManager::GetInstance()->ClearBossGageUIList();

		// ���̵� �ʵ��� ��� �̸� �������� �ʰ� �ε� ���� �����ֱ�
		if( true == g_pData->GetBattleFieldManager().GetIsBossRaidFieldByFieldID( uiBattleFieldId ) )
		{
			CreateLoadingUI();

			/// ȭ�� ���� ����� �ʵ�� ����
			if( NULL != g_pData && NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI() )
				g_pInstanceData->GetMiniMapUI()->SetVillageMapID( g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam() );
		}
		else
#endif // FIELD_BOSS_RAID
		{
			OpenBattleFieldName();
		}
	}	

}

/** @function : IsNearPortalLineMap() const
	@brief : ��Ż ���θʿ� �� ������ ��Ҵ����� �˻�
	@param : void
	@return : bool (������� true)
*/
bool CX2BattleFieldGame::IsNearPortalLineMap()
{
	ASSERT( GetMyUnit() );
	ASSERT( GetLineMap() );

	CX2GUUser* pMyGUUser = GetMyUnit();

	if ( pMyGUUser == NULL )
		return false;

	// ������ ���� ���¸� ��Ż ��ó���� �׾�� ó�� ����
	if ( pMyGUUser->GetGameUnitState() == CX2GameUnit::GUSI_DIE || !( pMyGUUser->GetNowHp()	> 0 ) )
		return false;

	const int iLastTouchedLineMapIndex = GetMyUnit()->GetLastTouchLineIndex( true );
	const CKTDGLineMap::LineData* pLastTouchedLineData = GetLineMap()->GetLineData( iLastTouchedLineMapIndex );

	ASSERT( pLastTouchedLineData );
	if ( NULL != pLastTouchedLineData )
	{

#ifdef SERV_COMMON_AUTO_OPEN_HARD_CODE
		int iLineIdx = 0;
#endif // SERV_COMMON_AUTO_OPEN_HARD_CODE
			
#ifdef SERV_ALTERA_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_ALTERA_AUTO_OPEN_HARD_CODE )
		{
			iLineIdx = 200;
		}
		ELSE
		{
			if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_BESMA_FIELD_02 &&
				CKTDGLineMap::LT_POTAL == pLastTouchedLineData->lineType &&
				pLastTouchedLineData->lineIndex == 200 )
			{
				return false;
			}
		}
#endif //SERV_ALTERA_AUTO_OPEN_HARD_CODE
#ifdef SERV_PEITA_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_PEITA_AUTO_OPEN_HARD_CODE )
		{
			iLineIdx = 16;
		}
		ELSE
		{
			if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_BESMA_FIELD_01 &&
				CKTDGLineMap::LT_POTAL == pLastTouchedLineData->lineType &&
				pLastTouchedLineData->lineIndex == 16 )
			{
				return false;
			}
		}
#endif //SERV_PEITA_AUTO_OPEN_HARD_CODE
#ifdef SERV_VELDER_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_VELDER_AUTO_OPEN_HARD_CODE )
		{
			iLineIdx = 24;
		}
		ELSE
		{
			if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_PEITA_FIELD_02 &&
				CKTDGLineMap::LT_POTAL == pLastTouchedLineData->lineType &&
				pLastTouchedLineData->lineIndex == 24 )
			{
				return false;
			}
		}
#endif //SERV_VELDER_AUTO_OPEN_HARD_CODE
#ifdef SERV_HAMEL_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_HAMEL_AUTO_OPEN_HARD_CODE )
		{
			iLineIdx = 190;
		}
		ELSE
		{
			if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_VELDER_FIELD_03 &&
				CKTDGLineMap::LT_POTAL == pLastTouchedLineData->lineType &&
				pLastTouchedLineData->lineIndex == 190 )
			{
				return false;
			}
		}
#endif //SERV_HAMEL_AUTO_OPEN_HARD_CODE
#ifdef SERV_SANDER_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_SANDER_AUTO_OPEN_HARD_CODE )
		{
			iLineIdx = 170;
		}
		ELSE
		{
			if( m_pWorld->GetWorldData()->worldID == CX2World::WI_BATTLE_FIELD_HAMEL_FIELD_03 &&
				CKTDGLineMap::LT_POTAL == pLastTouchedLineData->lineType &&
				pLastTouchedLineData->lineIndex == 170 )
			{
				return false;
			}
		}
#endif //SERV_SANDER_AUTO_OPEN_HARD_CODE

#ifdef SERV_COMMON_AUTO_OPEN_HARD_CODE
		if( iLineIdx != 0 && m_bOpenPotalParticle == false )
		{
			CreatePotalParticle( iLineIdx );
			m_bOpenPotalParticle = true;
		}
#endif // SERV_COMMON_AUTO_OPEN_HARD_CODE

		// Portal LineMap �̸� true ����
		switch ( pLastTouchedLineData->lineType )
		{
#ifdef FIELD_BOSS_RAID
		case CKTDGLineMap::LT_NORMAL:
			{
				if( true == pLastTouchedLineData->m_bIsPortalWhenActiveRaid &&
					true == CX2BossRaidManager::GetInstance()->IsActiveRaidPortal() )
					return true;
			} break;
#endif // FIELD_BOSS_RAID
		case CKTDGLineMap::LT_POTAL:
			{
				return true;
			} break;
		default:
			break;
		}
	}
	else
	{
		StateLog( L"lastTouchedLineData is wrong!" );
		return false;
	}	

	return false;
}

void CX2BattleFieldGame::CreateBattleFieldNpcAtStart( const std::vector< KNPCUnitReq >& vecNPCDataList_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ 
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								)
{
	BOOST_FOREACH( const KNPCUnitReq& kNpcUnitData, vecNPCDataList_ )
	{
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		const KAttribEnchantNpcInfo* pkAttribInfo = NULL;
		KAttribEnchantNpcInfoMap::const_iterator iter = mapAttribNpcInfo_.find( kNpcUnitData.m_UID );
		if ( iter != mapAttribNpcInfo_.end() )
			pkAttribInfo = &iter->second;
		CreateBattleFieldNpc( kNpcUnitData, pkAttribInfo );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		CreateBattleFieldNpc( kNpcUnitData );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	}

#ifdef	BATTLE_FIELD_TEST
	BOOST_FOREACH( CX2GUNPC* pNpc, m_NPCUnitList )
	{
		if ( NULL != pNpc )
			pNpc->SetCreatedBySyncNot( true );
	}
#endif	BATTLE_FIELD_TEST
	
}
void CX2BattleFieldGame::CreateBattleFieldNpcByNotData( const std::vector< KNPCUnitNot >& vecNPCDataList_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ 
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD		
	)
{
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::vector< KNPCUnitReq > vec;
	vec.reserve( vecNPCDataList_.size() );
	BOOST_FOREACH( const KNPCUnitNot& kNpcUnitData, vecNPCDataList_ )
	{
		vec.push_back( kNpcUnitData.m_kNPCUnitReq );
	}

	CreateNpcCreatedInfo( vec, mapAttribNpcInfo_, false );

#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD


	BOOST_FOREACH( const KNPCUnitNot& kNpcUnitData, vecNPCDataList_ )
	{
		CreateBattleFieldNpc( kNpcUnitData.m_kNPCUnitReq );
	}

#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
}


UINT CX2BattleFieldGame::GetEmpyIndexNpcUnitList()
{
	for ( UINT i = 0; i < m_NPCUnitList.size(); ++i )
	{
		if ( NULL == m_NPCUnitList[i] )
			return i;
	}

	return 0;
	ASSERT( !L"There are too many monsters" );
	StateLog( L"There are too many monsters" );
}

void CX2BattleFieldGame::CreateBattleFieldNpc( const KNPCUnitReq& kNpcUnitData_ 
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfo* pkAttribNpcInfo_ )
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
{	
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
	const CX2UnitManager::NPCUnitInfo*	pNPCUnitInfo = g_pData->GetUnitManager()->GetNPCUnitInfo(
		static_cast<CX2UnitManager::NPC_UNIT_ID>( kNpcUnitData_.m_NPCID ) );
	ASSERT( pNPCUnitInfo != NULL );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//	const CX2UnitManager::NPCUnitTemplet* pTemplet = 
//		g_pData->GetUnitManager()->GetNPCUnitTemplet( static_cast<CX2UnitManager::NPC_UNIT_ID>( kNpcUnitData_.m_NPCID ) );
//	ASSERT( NULL != pTemplet );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
	if ( NULL != pNPCUnitInfo )
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//	if ( NULL != pTemplet )
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
	{
		// Ally Npc�� ���
		if ( -1 < kNpcUnitData_.m_iAllyUID )
		{
			CX2GUUser* pOwnerUser = GetUserUnitByUID( kNpcUnitData_.m_iAllyUID );

			CreateNPC( static_cast<CX2UnitManager::NPC_UNIT_ID>( kNpcUnitData_.m_NPCID ),
				kNpcUnitData_.m_Level, kNpcUnitData_.m_bActive, kNpcUnitData_.m_UID, 
				D3DXVECTOR3( kNpcUnitData_.m_vPos.x, kNpcUnitData_.m_vPos.y, kNpcUnitData_.m_vPos.z ),
				kNpcUnitData_.m_bIsRight, kNpcUnitData_.m_fDelayTime, kNpcUnitData_.m_KeyCode, 
				static_cast<CX2Room::TEAM_NUM>( kNpcUnitData_.m_cTeamNum ), 
				static_cast<CX2NPCAI::NPC_AI_TYPE>( kNpcUnitData_.m_cAIType ), kNpcUnitData_.m_iAllyUID, 
				0, static_cast<CX2Room::TEAM_NUM>(kNpcUnitData_.m_cAllyTeam));
		}
		else
		{
			CNpcCreatedInfoPtr NpcCreatedInfoPtr( CNpcCreatedInfo::CreateNpcCreatedInfoPtr( 0, kNpcUnitData_ ) );
			CX2GUNPC* pBattleFieldNpc = CX2GUNPC::CreateBattleFieldNpc( NpcCreatedInfoPtr, 
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
				pNPCUnitInfo
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//				pTemplet 
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
				);

			ASSERT( NULL != pBattleFieldNpc );
			if ( NULL != pBattleFieldNpc )
			{
				ASSERT( -1 != kNpcUnitData_.m_nStartPos );
				pBattleFieldNpc->Init( false, kNpcUnitData_.m_nStartPos );
#ifndef SERV_BATTLEFIELD_MIDDLE_BOSS
				// ���赵 ������ ���� ���� ���Ͱ� ������ ī�޶� �� Ű�Է� ó��
				if ( true == kNpcUnitData_.m_bFocusCamera )
				{
					pBattleFieldNpc->SetFocusCamera( true );
					GetMyUnit()->SetCanNotInputTime_LUA( 4.f );
					PauseNPCAI( 4.f );
				}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
				// �� �ҽ��� CreateBattleFieldNpcByCreatedInfo ���� ���� Camera ���ڸ� �����Ƿ�
				// �ʿ� ������
				int iLastTouchedIndex = pBattleFieldNpc->GetLastTouchLineIndex( false );
				pBattleFieldNpc->SetHardLevel( kNpcUnitData_.m_Level, true );
				pBattleFieldNpc->SetActiveMonster( kNpcUnitData_.m_bActive );
				
				AddUnit( pBattleFieldNpc );
				pBattleFieldNpc->SetUnitIndex( GetEmptyNPCIndex() );
				SetNPCUnit( pBattleFieldNpc->GetUnitIndex(), pBattleFieldNpc );

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
				if ( pkAttribNpcInfo_ != NULL && pkAttribNpcInfo_->IsValidInfo() == true )
					pBattleFieldNpc->ApplyEnchantMonster( *pkAttribNpcInfo_ );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
				if ( !m_mapAttribNpcInfo.empty() )
				{
					KAttribEnchantNpcInfoMap::const_iterator mitr = m_mapAttribNpcInfo.find( pBattleFieldNpc->GetUID() );

					if ( m_mapAttribNpcInfo.end() != mitr )
					{
						const KAttribEnchantNpcInfo& kAttribEnchantNpcInfo = mitr->second;
						pBattleFieldNpc->ApplyEnchantMonster( kAttribEnchantNpcInfo );
					}
				}
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

				pBattleFieldNpc->InitFullName();
				pBattleFieldNpc->InitNameColor( GetMyUnit()->GetUnitLevel() );

				g_pKTDXApp->GetDGManager()->AddObjectChain( pBattleFieldNpc );
			}
			else
				StateLog( "FieldNpc is NULL in CreateBattleFieldNpc" );
		}		
	}
	else
		StateLog( "NpcTemplet is NULL in CreateBattleFieldNpc" );
}

void CX2BattleFieldGame::CreateBattleFieldNpcByCreatedInfo( const CNpcCreatedInfoPtr NpcCreatedInfoPtr_ )
{
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
	const CX2UnitManager::NPCUnitInfo*	pNPCUnitInfo = g_pData->GetUnitManager()->GetNPCUnitInfo(
		static_cast<CX2UnitManager::NPC_UNIT_ID>( NpcCreatedInfoPtr_->GetNpcId() ) );
	ASSERT( pNPCUnitInfo != NULL );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//	const CX2UnitManager::NPCUnitTemplet* pTemplet = 
//		g_pData->GetUnitManager()->GetNPCUnitTemplet( static_cast<CX2UnitManager::NPC_UNIT_ID>( NpcCreatedInfoPtr_->GetNpcId() ) );
//	ASSERT( NULL != pTemplet );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING

//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
	if ( pNPCUnitInfo != NULL )
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//	if ( NULL != pTemplet )
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
	{
		// Ally Npc�� ���
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
 		if ( -1 < NpcCreatedInfoPtr_->GetAllyUid() )
 		{
 			//CX2GUUser* pOwnerUser = GetUserUnitByUID( NpcCreatedInfoPtr_->GetAllyUid() );
 
			CX2Room::TEAM_NUM eAllyTeam = CX2Room::TN_NONE;
			if( NULL != GetUserUnitByUID(NpcCreatedInfoPtr_->GetAllyUid()) )
				eAllyTeam = static_cast<CX2Room::TEAM_NUM>(GetUserUnitByUID(NpcCreatedInfoPtr_->GetAllyUid())->GetTeam());

 			CreateNPC( static_cast<CX2UnitManager::NPC_UNIT_ID>( NpcCreatedInfoPtr_->GetNpcId() ),
 				NpcCreatedInfoPtr_->GetLevel(), NpcCreatedInfoPtr_->GetActive(), NpcCreatedInfoPtr_->GetNpcUid(), 
				NpcCreatedInfoPtr_->GetPos(),
 				NpcCreatedInfoPtr_->GetIsRight(), NpcCreatedInfoPtr_->GetDelayTime(), NpcCreatedInfoPtr_->GetKeyCode(), 
 				static_cast<CX2Room::TEAM_NUM>( NpcCreatedInfoPtr_->GetTeamNum() ), 
 				static_cast<CX2NPCAI::NPC_AI_TYPE>( NpcCreatedInfoPtr_->GetAIType() ), NpcCreatedInfoPtr_->GetAllyUid(), 0,
				eAllyTeam
		#ifdef FIX_FIELD_SUMMON_MONSTER // �ʵ忡�� ���� ī�尡 ��ȯ���� �� ���ӽð� ���� ���� �� �ֵ��� �߰�
				, static_cast<CX2GUNPC::NPC_CREATE_TYPE>(NpcCreatedInfoPtr_->GetCreateMonsterType())
		#endif // FIX_FIELD_SUMMON_MONSTER
				) ;
 		}
 		else
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		{
			CX2GUNPC* pBattleFieldNpc = CX2GUNPC::CreateBattleFieldNpc( NpcCreatedInfoPtr_, 
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
				pNPCUnitInfo
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//				pTemplet 
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
				);


			ASSERT( NULL != pBattleFieldNpc );
			if ( NULL != pBattleFieldNpc )
			{
				ASSERT( -1 != NpcCreatedInfoPtr_->GetStartPosNum() );

#ifdef NEXON_QA_CHEAT_REQ
				if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )
				{
					if(  0 == NpcCreatedInfoPtr_->GetPetrolLineIndexList().size() 
#ifdef FIELD_BOSS_RAID
						// �������̵� ���ʹ� ��Ʈ���� �������� �ʵ��� �ǵ� ��.
						&& false == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField()					
#endif // FIELD_BOSS_RAID
						)
					{
						const D3DXVECTOR3 mPos = NpcCreatedInfoPtr_->GetPos();
						pBattleFieldNpc->InitCheat( mPos, false );
					}
					else
					{
						pBattleFieldNpc->Init( false, NpcCreatedInfoPtr_->GetStartPosNum() );
					}
				}
				else
					pBattleFieldNpc->Init( false, NpcCreatedInfoPtr_->GetStartPosNum() );
#else
				pBattleFieldNpc->Init( false, NpcCreatedInfoPtr_->GetStartPosNum() );
#endif //NEXON_QA_CHEAT_REQ

				int iLastTouchedIndex = pBattleFieldNpc->GetLastTouchLineIndex( false );

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
				UINT uiBossGroupID = NpcCreatedInfoPtr_->GetBossGroupId ();		// Boss Group ID �� ������ ����
				pBattleFieldNpc->SetBossGroupID ( uiBossGroupID );
				bool bFocusCamera = NpcCreatedInfoPtr_->GetFocusCamera();		// FocusCamera �� ������ ����
				pBattleFieldNpc->SetFocusCamera( bFocusCamera );			
				pBattleFieldNpc->SetIsMainBoss ( false );						// Main Boss �ʱ�ȭ
				if (  0 < uiBossGroupID )										// Boss Group ID �� �ִٸ�
				{	
					if ( true == bFocusCamera )									// Focus Camera �� �ִٸ�, ���α� ����
					{
						pBattleFieldNpc->SetIsMainBoss( true );					// ���� ������ �����ϰ� indicator �� ������ ���α����� 
						SetIndicatorToNpc ( pBattleFieldNpc ); 						
						UpdateCreateUnitDisplayEffect ( pBattleFieldNpc, true);	// ���� ���� ���� ����
						// �������� ������ ��, ���� ������ ���ٸ� ��Ŷ ���� ������ ó�� �˻��� ���Ϳ��� ���� ������ ������
					}			
					if ( NULL == m_pMonsterIndicator )							// ó�� ������ ���͸�
						SetIndicatorToNpc ( pBattleFieldNpc ); 					// ���� ���ͷ� 

					PauseNPCAI( 4.f );			

					if ( false == IsHost() )							// ������ �ƴϸ� DamagedMap ��û ��Ŷ�� ����
					{
						KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ kEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ;
						kEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ.m_iNPCUID = static_cast<int> ( pBattleFieldNpc->GetUnitUID () );
						g_pData->GetServerProtocol()->SendPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ, kEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ );
						g_pMain->AddServerPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK );							
					}
				}
#ifdef FIELD_BOSS_RAID
				else if( true == pBattleFieldNpc->GetIsBosRaidNPC() )
				{
					// ������ �ƴϸ� DamagedMap ��û ��Ŷ�� ����
					if ( false == IsHost() )
					{
						KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ kEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ;
						kEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ.m_iNPCUID = static_cast<int> ( pBattleFieldNpc->GetUnitUID () );
						g_pData->GetServerProtocol()->SendPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ, kEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ );
						g_pMain->AddServerPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK );							
					}
				}
#endif // FIELD_BOSS_RAID

#else // SERV_BATTLEFIELD_MIDDLE_BOSS
				if ( true == NpcCreatedInfoPtr_->GetFocusCamera() )
				{
					pBattleFieldNpc->SetFocusCamera( true );
					GetMyUnit()->SetCanNotInputTime_LUA( 4.f );
					PauseNPCAI( 4.f );
				}
				else
				{
					pBattleFieldNpc->SetFocusCamera( false );
				}

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifndef NEXON_QA_CHEAT_REQ
				if ( CX2User::XUAL_NORMAL_USER < g_pData->GetMyUser()->GetAuthLevel() )
				{
					if ( NpcCreatedInfoPtr_->GetPetrolLineIndexList().end() == std::find( NpcCreatedInfoPtr_->GetPetrolLineIndexList().begin(), NpcCreatedInfoPtr_->GetPetrolLineIndexList().end(), iLastTouchedIndex ) )
					{
						WCHAR wLog[512];
						ZeroMemory( wLog, sizeof(WCHAR) * 512 );
						wsprintf( wLog, L"NPC_ID: %d, NPC_UID: %d, nStartPos: %d, LineMapIndex: %d\n", 
							NpcCreatedInfoPtr_->GetNpcId(), NpcCreatedInfoPtr_->GetNpcUid(), NpcCreatedInfoPtr_->GetStartPosNum(), iLastTouchedIndex );
						StringCchCat( wLog, 256, L"PetrolLineIndex: " );

						BOOST_FOREACH( const int& iIndex, NpcCreatedInfoPtr_->GetPetrolLineIndexList() )
						{
							WCHAR wBuffer[10];
							ZeroMemory( wBuffer, sizeof(WCHAR) * 10 );
							wsprintf( wBuffer, L" %d", iIndex );
							StringCchCat( wLog, 512, wBuffer );
						}

						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), wLog, g_pMain->GetNowState() );

					}
				}
#endif //NEXON_QA_CHEAT_REQ
			//#endif	BATTLE_FIELD_TEST

				pBattleFieldNpc->SetHardLevel( NpcCreatedInfoPtr_->GetLevel(), true );
				pBattleFieldNpc->SetActiveMonster( NpcCreatedInfoPtr_->GetActive() );

				//pBattleFieldNpc->SetKeyCode( kNpcUnitData_.m_KeyCode );

				AddUnit( pBattleFieldNpc );
				pBattleFieldNpc->SetUnitIndex( GetEmptyNPCIndex() );
				SetNPCUnit( pBattleFieldNpc->GetUnitIndex(), pBattleFieldNpc );
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
				if ( NpcCreatedInfoPtr_->GetAttribEnchantNpcInfo().IsValidInfo() == true )
					pBattleFieldNpc->ApplyEnchantMonster( NpcCreatedInfoPtr_->GetAttribEnchantNpcInfo() );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
				if ( !m_mapAttribNpcInfo.empty() )
				{
					KAttribEnchantNpcInfoMap::const_iterator mitr = m_mapAttribNpcInfo.find( pBattleFieldNpc->GetUID() );

					if ( m_mapAttribNpcInfo.end() != mitr )
					{
						const KAttribEnchantNpcInfo& kAttribEnchantNpcInfo = mitr->second;
						pBattleFieldNpc->ApplyEnchantMonster( kAttribEnchantNpcInfo );
					}

				}
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

				pBattleFieldNpc->InitFullName();
				pBattleFieldNpc->InitNameColor( GetMyUnit()->GetUnitLevel() );

				g_pKTDXApp->GetDGManager()->AddObjectChain( pBattleFieldNpc );
			}
			else
			{
				ASSERT( !L"FieldNpc is NULL in CreateBattleFieldNpcByCreatedInfo" );
				StateLog( "FieldNpc is NULL in CreateBattleFieldNpcByCreatedInfo" );
			}
		}		
	}
	else
	{
		ASSERT( !L"NpcTemplet is NULL in CreateBattleFieldNpcByCreatedInfo" );
		StateLog( "NpcTemplet is NULL in CreateBattleFieldNpcByCreatedInfo" );
	}
}
//*/

bool CX2BattleFieldGame::Handler_EGS_JOIN_BATTLE_FIELD_NOT( KEGS_JOIN_ROOM_NOT& kPacket_ )
{
	return Handler_EGS_JOIN_ROOM_NOT( kPacket_ );
}

/*virtual*/ void CX2BattleFieldGame::UpdateUnitPointer()
{
	CX2Game::UpdateUnitPointer();
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL )
		{
#ifdef MODIFY_DUNGEON_STAGING
			if( true == pCX2GUNPC->GetStopAtCurrentState() )
			{	//���� Ű�Է� ���� NPC AI��Ȱ��ȭ
				CanNotInputAndDisalbeNPCAI(true);			
			}

			if( true == pCX2GUNPC->GetFocusCameraAtCurrentState() )
			{
				m_optrFocusUnit = pCX2GUNPC;
				if( true == pCX2GUNPC->GetApplyCinematicUI() && true == g_pMain->IsInheritStateMenu())
				{
					CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
					if( NULL != pStateMenu )
					{	//��Ŀ�� ī�޶� ���� �ÿ� ���� UI�� ���� ���⵵�� ����
						pStateMenu->SetShowUI(false);
					}
				}
				return;
			}
			else
			{
				CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
				if( NULL != pStateMenu )
				{
					pStateMenu->SetShowUI(true);
				}
			}
#endif //MODIFY_DUNGEON_STAGING

			//{{ kimhc // 2010.8.7 // ������ NPC���� ī�޶� ������ �ϴ� ���
			if( pCX2GUNPC->GetFocusCameraForce() == true 
				|| ( pCX2GUNPC->GetFocusCamera() == true && pCX2GUNPC->GetStartState() == (int)pCX2GUNPC->GetGameUnitState() ) )
				//}} kimhc // 2010.8.7 // ������ NPC���� ī�޶� ������ �ϴ� ���
			{
				m_optrFocusUnit = pCX2GUNPC;

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
				// ī�޶� ��Ŀ���� ���� �ӽ÷� ������ �κ�,
				// ���� ������ ���� State ��,  GetFocusCamera �̿��ؼ� �� ��
				// ī�޶� 2�� �ٴ� ��, SetFocusCamera ���� �̹Ƿ� �ش� �κ� ��� ���� �� ���� �� ��
				if ( true == pCX2GUNPC->GetIsMainBoss() &&					// Main ���� �̰�
 					 true == m_pCinematicUI->GetIsCinematicUIActive () )	// Cinematic UI �� Ȱ��ȭ ���°� �ƴ϶��
				{	
					// FocusCameraForce �� �����ϰ� ��Ŀ���� ������� ������.
					pCX2GUNPC->SetFocusCameraForce( false );
					m_optrFocusUnit = GetMyUnit();
				}
				else		// �ش� ��찡 �ƴϸ�, FocusCamera �� �����Ѵ�.
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
					return;
			}
		}
	}

	m_optrFocusUnit = GetMyUnit();
}

void CX2BattleFieldGame::CreateNpcCreatedInfo( const vector<KNPCUnitReq>& vecNpcData_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ 
								, bool bSetNPCLoadComplete
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								)
{
	const D3DXVECTOR3& vMyPos = GetMyUnit()->GetPos();
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::deque<CNpcCreatedInfoPtr>& conNpcCreatedInfo = m_deqNpcCreatedInfo;
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::vector<CNpcCreatedInfoPtr>& conNpcCreatedInfo = m_vecNpcCreatedInfo;
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	unsigned	uiOldSize = conNpcCreatedInfo.size();

	BOOST_FOREACH( const KNPCUnitReq& kData, vecNpcData_ )
	{
		
		const D3DXVECTOR3 vNpcPos( kData.m_vPos.x, kData.m_vPos.y, kData.m_vPos.z );

		const float fDistance3Sq = GetDistance3Sq( vMyPos, vNpcPos );
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		const KAttribEnchantNpcInfo* pkAttribInfo = NULL;
		KAttribEnchantNpcInfoMap::const_iterator iter = mapAttribNpcInfo_.find( kData.m_UID );
		if ( iter != mapAttribNpcInfo_.end() )
			pkAttribInfo = &iter->second;
		CNpcCreatedInfoPtr spNpcCreatedInfo = CNpcCreatedInfo::CreateNpcCreatedInfoPtr( fDistance3Sq, kData, pkAttribInfo );
		//������//2012-11-20//���� ��ȯ�� ��ų �������� �ʴ� ���� ������ ���� SetAllyUid �ּ�ó��
		//spNpcCreatedInfo->SetAllyUid( -1 );
				
		
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		
#else	// SERV_BATTLEFIELD_MIDDLE_BOSS
		spNpcCreatedInfo->SetFocusCamera( false );
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		conNpcCreatedInfo.push_back( spNpcCreatedInfo );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		conNpcCreatedInfo.push_back( CNpcCreatedInfo::CreateNpcCreatedInfoPtr( fDistance3Sq, kData ) );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef FIELD_BOSS_RAID
		CX2BossRaidManager::GetInstance()->CreateNpcExceptProcess( static_cast<CX2UnitManager::NPC_UNIT_ID>(kData.m_NPCID) );
#endif // FIELD_BOSS_RAID
	}

	

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	auto MoreFarDistance3SqCompFunc = []( CNpcCreatedInfoPtr lhs_, CNpcCreatedInfoPtr rhs_ ) {
		return *lhs_ < *rhs_;
	};

	std::sort( conNpcCreatedInfo.begin() + uiOldSize, conNpcCreatedInfo.end(), MoreFarDistance3SqCompFunc );
	unsigned	uiSize = conNpcCreatedInfo.size();
	for( unsigned i = uiOldSize; i < uiSize; i++ )
	{
		m_UnitLoader.AddNPCUnitUIDResource( conNpcCreatedInfo[ i ]->GetNpcUid(), (CX2UnitManager::NPC_UNIT_ID) conNpcCreatedInfo[ i ]->GetNpcId()
			, &conNpcCreatedInfo[ i ]->GetAttribEnchantNpcInfo() );
	}
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	auto MoreFarDistance3SqCompFunc = []( CNpcCreatedInfoPtr lhs_, CNpcCreatedInfoPtr rhs_ ) {
		return *lhs_ > *rhs_;
	};
	std::sort( conNpcCreatedInfo.begin(), conNpcCreatedInfo.end(), MoreFarDistance3SqCompFunc );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	if ( bSetNPCLoadComplete == true )
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		m_bIsNpcLoadComplete = true;

}

/*virtual*/ bool CX2BattleFieldGame::DeleteNPCUnitByUID( const UINT uiNpcUid_ )
{
	bool bReturn = CX2Game::DeleteNPCUnitByUID( uiNpcUid_ );

	if ( bReturn )
		DeleteNpcInCreatedInfoList( uiNpcUid_ );
	else
		bReturn = DeleteNpcInCreatedInfoList( uiNpcUid_ );

	return bReturn;
}

bool CX2BattleFieldGame::DeleteNpcInCreatedInfoList( const UINT uiNpcUid_ )
{
	bool bReturn = false;

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::deque<CNpcCreatedInfoPtr>&	conNpcCreatedInfo = m_deqNpcCreatedInfo;
	std::deque<CNpcCreatedInfoPtr>::iterator iter;
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::vector<CNpcCreatedInfoPtr>& conNpcCreatedInfo = m_vecNpcCreatedInfo;
	vector<CNpcCreatedInfoPtr>::iterator iter;
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

	if ( false == conNpcCreatedInfo.empty() )
	{
		for( iter = conNpcCreatedInfo.begin(); iter != conNpcCreatedInfo.end(); ++iter )
		{
			if ( (*iter)->GetNpcUid() == uiNpcUid_ )
			{
				conNpcCreatedInfo.erase( iter );
				bReturn = true;
				break;
			}
		}
	}
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_UnitLoader.RemoveNPCUnitUIDResource( uiNpcUid_ );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

	return bReturn;
}

/*virtual*/ bool CX2BattleFieldGame::Handler_EGS_NPC_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NPC_UNIT_DIE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	CX2GUNPC* pCX2GUNPC = GetNPCUnitByUID( kEvent.m_nDieNPCUID );
	if ( NULL != pCX2GUNPC )
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
	}

	DeleteNpcInCreatedInfoList( kEvent.m_nDieNPCUID );
	ProcessExpListByNpcUnitDie( kEvent );
#ifdef ARA_CHARACTER_BASE
	ProcessIKillThisNpc( kEvent );
#endif

	return true;
}

void CX2BattleFieldGame::Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT()
{
	if ( NULL != m_pBattleFieldRoom && NULL != GetMyUnit() )
	{
		KEGS_UPDATE_BATTLE_FIELD_USER_POS_NOT kPacket;

		kPacket.m_kLastPos.m_iMapID = m_pBattleFieldRoom->GetBattleFieldId();
		kPacket.m_kLastPos.m_ucLastTouchLineIndex = GetMyUnit()->GetLastTouchLineIndex();

		
		const CKTDGLineMap::LineData* pLineData =
			( NULL == GetLineMap() ? NULL : GetLineMap()->GetLineData( kPacket.m_kLastPos.m_ucLastTouchLineIndex ) );

		if ( NULL != pLineData )
		{
			// ���ε������� ���ۺ��� �������� ��������
			const float fDistance3SqFromStartToEnd = GetDistance3Sq( pLineData->startPos, pLineData->endPos );
			// ���ε������� ���ۺ��� ���� �ڽ��� ��ġ ������ ��������
			const float fDistance3SqFromStartToMyPos = GetDistance3Sq( pLineData->startPos, GetMyUnit()->GetLandPos() );
			
			// ���� ���ε������� ��%�� ��ġ�ϴ��� �˾Ƴ����� �˾Ƴ��� ����
			const float fLastPosValue3Sq = fDistance3SqFromStartToMyPos / fDistance3SqFromStartToEnd;

			if ( fLastPosValue3Sq > 0 )
				kPacket.m_kLastPos.m_usLastPosValue = static_cast<USHORT>( sqrtf( fLastPosValue3Sq ) * 100 );
			else
				kPacket.m_kLastPos.m_usLastPosValue = 1;
		}
		else
			kPacket.m_kLastPos.m_usLastPosValue = 1;

		CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
		battleFieldManager.SetBattleFieldIdWhereIam( m_pBattleFieldRoom->GetBattleFieldId() );
		battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( GetMyUnit()->GetLastTouchLineIndex() );
		battleFieldManager.SetBattleFieldPositionValue( kPacket.m_kLastPos.m_usLastPosValue );
		battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( false );

		g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT, kPacket );
	}
	else
	{
		ASSERT( !"Wrong Path!" );
		return;
	}
	
}

/*virtual*/ bool CX2BattleFieldGame::Handler_EGS_LEAVE_BATTLE_FIELD_NOT( const KEGS_LEAVE_BATTLE_FIELD_NOT& kPacket )
{
	if ( NULL != g_pX2Room && CX2Room::RT_BATTLE_FIELD == g_pX2Room->GetRoomType() )
	{
		DialogLog( "Before DeleteUserUnitByUID" );
		DeleteUserUnitByUID( kPacket.m_iUnitUID );

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		DialogLog( "Before DeleteUnitLoadData, After DeleteUserUnitByUID" );
		m_UnitLoader.DeleteUnitLoadData( kPacket.m_iUnitUID );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		DialogLog( "Before SetRoomState, After DeleteUnitLoadData" );
		g_pX2Room->SetRoomState( static_cast<CX2Room::ROOM_STATE>( kPacket.m_cRoomState ) );

		DialogLog( "Before Set_KRoomSlotInfoList, After SetRoomState" );
		g_pX2Room->Set_KRoomSlotInfoList( kPacket.m_vecSlot );

		DialogLog( "Before UpdateUnitPointer, After Set_KRoomSlotInfoList" );
		UpdateUnitPointer();
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
////#ifdef	BATTLE_FIELD_TEST
//		//FieldTest
//		BOOST_FOREACH( const KRoomSlotInfo& kRoomSlotInfo, kPacket.m_vecSlot )
//		{
//			if ( true == kRoomSlotInfo.m_bHost )
//			{
//				CX2GUUser* pHostGUUser = GetUserUnitByUID( kRoomSlotInfo.m_kRoomUserInfo.m_nUnitUID );
//				if ( NULL != pHostGUUser)
//					SetHostGameUnit( pHostGUUser );
//				else
//					ResetHostGameUnit();
//			}
//		}		
////#endif	BATTLE_FIELD_TEST
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		DialogLog( "After UpdateUnitPointer" );
		return true;
	}
	else
		return false;
	
}

/*virtual*/ void CX2BattleFieldGame::AddUserUnit( CX2Room::SlotData* pSlotData_, int iSlotIndex_ )
{
#ifdef SETTING_BATTLE_FIELD_ENEMY_TEAM_FOR_TEST_PVE_SYSTEM
	if( NULL != pSlotData_ && 
		NULL != pSlotData_->m_pUnit )
	{
		if( false == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
		{
			switch( pSlotData_->m_pUnit->GetType() )
			{
			case CX2Unit::UT_ARA:
			case CX2Unit::UT_ARME:
			case CX2Unit::UT_LIRE:
			case CX2Unit::UT_EVE:
			case CX2Unit::UT_ELESIS:
				pSlotData_->m_TeamNum = 1;
			default:
				break;
			}
		}
	}
#endif // SETTING_BATTLE_FIELD_ENEMY_TEAM_FOR_TEST_PVE_SYSTEM

	ASSERT( static_cast<UINT>( iSlotIndex_ ) < m_UserUnitList.size() );
	if ( static_cast<UINT>( iSlotIndex_ ) < m_UserUnitList.size() )
	{
		CX2GUUser* pCX2UserUnit = m_UserUnitList[iSlotIndex_];

		// ��������Ʈ�� ������ ������ ������
		if ( NULL == pCX2UserUnit )
		{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			pCX2UserUnit = CreateAndInsertUserUnit( pSlotData_, iSlotIndex_ );
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		}
		else	// ��������Ʈ�� ������ ������ �ְ�
		{
			// �װ��� �� ���� ������ �ٸ� �������
			if ( pSlotData_->m_UnitUID != pCX2UserUnit->GetUnitUID() )
			{
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				DeleteUserUnit( iSlotIndex_ );
				pCX2UserUnit = CreateAndInsertUserUnit( pSlotData_, iSlotIndex_ );
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			}
			else
			{
				if ( true == pSlotData_->m_bMySlot )
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    UpdateMyGameUnit( pSlotData_, pCX2UserUnit );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					m_optrMyUnit = pCX2UserUnit;
//					CheckAmIHost();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}

				if ( true == pSlotData_->m_bMySlot && false == g_pData->GetBattleFieldManager().GetNowBattleFieldPositionInfoStartPosition() )
					pCX2UserUnit->InitPosByBattleFieldPosition();
				else
					pCX2UserUnit->InitPosition( false, g_pData->GetBattleFieldManager().GetBattleFieldPositionIndexWhereIShouldBe() );
			}			

			pCX2UserUnit->ClearRecieveSyncData();
		}

		if ( NULL != pCX2UserUnit )
		{

//#ifdef	BATTLE_FIELD_TEST
			//FieldTest
			if ( pSlotData_->GetIsHost() )
				SetHostGameUnit( pCX2UserUnit );
//#endif	BATTLE_FIELD_TEST

			CX2GageManager::GetInstance()->SetPartyMemberGameUnit( pCX2UserUnit );	

#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( pSlotData_->m_RelationshipTargetUID > 0 )	// ���� ���� ������ Ŀ���̶��
			{
				// �ش� ������ UID �� �޾ƿ� GUUser ��ü�� �־�д�.
				pCX2UserUnit->SetRelationshipUnitUID ( pSlotData_->m_RelationshipTargetUID );
			}


			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
				CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = pRelationshipManager->GetMyRelationshipInfo();
				
				// �� ���°� �ַΰ� �ƴ� ��, �ش� UID �� �� ������ UID ���
				// �� �������� �����Ѵ�.
				if ( pRelationshipInfo->m_eRelationStateType >	SEnum::RT_SOLO )
				{
					if ( pRelationshipInfo->m_iRelationTargetUserUid == pCX2UserUnit->GetUnit()->GetUID() )
					{
						pRelationshipManager->SetRelationPartnerInGame ( CX2GameUnitoPtr( pCX2UserUnit ) );
					}
				}
			}

#endif // ADDED_RELATIONSHIP_SYSTEM
		}

	}
}

CX2GUUser* CX2BattleFieldGame::CreateAndInsertUserUnit( CX2Room::SlotData* pSlotData_, int iSlotIndex_ )
{
	CX2GUUser* pCX2UserUnit = CreateGUUser( pSlotData_, iSlotIndex_ );

	if ( NULL != pCX2UserUnit )
	{
		if ( true == pSlotData_->m_bMySlot )
		{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            UpdateMyGameUnit( pSlotData_, pCX2UserUnit );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			m_optrMyUnit = pCX2UserUnit;
//			CheckAmIHost();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		}

		m_UnitList.push_back( static_cast<CX2GameUnit*>( pCX2UserUnit ) );
		m_UserUnitList[iSlotIndex_] = pCX2UserUnit;

		pCX2UserUnit->Init( false, g_pData->GetBattleFieldManager().GetBattleFieldPositionIndexWhereIShouldBe() );
		if ( pCX2UserUnit->IsMyUnit() )
			pCX2UserUnit->SetKBuffFactorFromGageManagerToGameUnit();

		g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2UserUnit );
	}

	return pCX2UserUnit;
}

/*virtual*/ void CX2BattleFieldGame::ResurrectionKeyProcess()
{
	if ( GetMyUnit() != NULL && GetMyUnit()->GetGameUnitState() == CX2GameUnit::GUSI_DIE && 
		m_bIsSendResurrectionReq == false )
	{
		if ( false == GetOpenChatBox() )
		{
			if ( m_InputData.oneZ == true )
			{
				Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( GetMyUnit()->GetUnit()->GetUID() );
			}
			else if ( m_InputData.oneX )
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
				m_fResurrectionRemainTime = 0.0f;
			}
		}
	}
}

void CX2BattleFieldGame::OpenBattleFieldName()
{
	SAFE_DELETE_DIALOG( m_pDlgBattleFieldName );

	m_pDlgBattleFieldName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_BATTLE_FIELD_NAME.LUA" );
	ASSERT( NULL != m_pDlgBattleFieldName );
	if ( m_pDlgBattleFieldName != NULL )
	{
		CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();

		CKTDGUIStatic* pStaticBattleFieldName = static_cast<CKTDGUIStatic*>( m_pDlgBattleFieldName->GetControl( L"BATTLE_FIELD_NAME" ) );
		if ( NULL != pStaticBattleFieldName )
		{
			CKTDGUIControl::CPictureData* pPictureBattleFieldName = pStaticBattleFieldName->GetPicture( 0 );
			if ( NULL != pPictureBattleFieldName )
			{
				const WCHAR* pTextureName = NULL;
				const WCHAR* pTextureKey = NULL;
				if ( battleFieldManager.GetBattleFieldTextureNameAndKey( 
					battleFieldManager.GetBattleFieldIdWhereIam(), &pTextureName, &pTextureKey ) )
					pPictureBattleFieldName->SetTex( pTextureName, pTextureKey );
			}
		}

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBattleFieldName );		

		m_pDlgBattleFieldName->Move( m_pDlgBattleFieldName->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 5.0f, false, true );

		if( NULL != g_pInstanceData->GetMiniMapUI() )
		{
			g_pInstanceData->GetMiniMapUI()->SetVillageMapID( battleFieldManager.GetBattleFieldIdWhereIam() );
		}
	}
}


void CX2BattleFieldGame::ResurrectOtherUser()
{
	if ( GetMyUnit() != NULL &&
		m_bIsSendResurrectionReq == false &&
		m_pDLGResurrect == NULL &&
		m_pDLGCheckResurrectByUseCash == NULL)
	{

#ifdef FIX_RESURRECTION_INDEX // ���볯¥: 2013-04-25

		UidType uidSelectUserUID = -1;

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F2 ) == TRUE )
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();

			if ( NULL != pGageManager )
				uidSelectUserUID = pGageManager->GetPartyGageUIPositionIndexByUserList( 2 );
		}
		else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F3 ) == TRUE )
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();

			if ( NULL != pGageManager )
				uidSelectUserUID = pGageManager->GetPartyGageUIPositionIndexByUserList( 1 );
		}
		else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F4 ) == TRUE )
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();

			if ( NULL != pGageManager )
				uidSelectUserUID = pGageManager->GetPartyGageUIPositionIndexByUserList( 0 );
		}

		if ( uidSelectUserUID != - 1 )
		{
			for ( int i = 0; i < GetUserUnitListSize(); i++ )
			{
				CX2GUUser* pUserUnit = GetUserUnit(i);

				if ( NULL != pUserUnit )
				{
					UidType uidNowUserUID = pUserUnit->GetUnitUID();

					if ( uidNowUserUID == uidSelectUserUID && pUserUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
					{
						m_UnitUIDToResurrect = uidNowUserUID;
						wstring wstrResurrectText = L"";

						wstrResurrectText = GET_REPLACED_STRING( ( STR_ID_140, "S", pUserUnit->GetUnit()->GetNickName() ) );
						SAFE_DELETE_DIALOG( m_pDLGResurrect );
						m_pDLGResurrect = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), wstrResurrectText.c_str(), 
							CX2State::SUCM_RESURRECT_OK, g_pMain->GetNowState(), 
							CX2State::SUCM_RESURRECT_CANCEL );

						break;
					}
				}
			}
		}

#else // FIX_RESURRECTION_INDEX

		int sequenceToFind = -1;

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F2 ) == TRUE )
		{
			sequenceToFind = 3;	
		}
		else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F3 ) == TRUE )
		{
			sequenceToFind = 2;
		}
		else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F4 ) == TRUE )
		{
			sequenceToFind = 1;
		}

		if ( sequenceToFind != - 1 )
		{
			int tempCount = 1;
			for ( int i = 0; i < GetUserUnitListSize(); i++ )
			{
				CX2GameUnit* pUnit = GetUnit(i);
				if ( pUnit != NULL && g_pData->GetBattleFieldRoom()->GetMySlot()->m_Index != i )
				{
					if ( tempCount == sequenceToFind )
					{
						if ( pUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
						{
							m_UnitUIDToResurrect = pUnit->GetUnitUID();
							wstringstream wstrstm;
							CX2GUUser* pGUUSer = (CX2GUUser*)pUnit;
							wstrstm << GET_REPLACED_STRING( ( STR_ID_140, "S", pGUUSer->GetUnit()->GetNickName() ) );
							SAFE_DELETE_DIALOG( m_pDLGResurrect );
							m_pDLGResurrect = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), wstrstm.str().c_str(), CX2State::SUCM_RESURRECT_OK, 
								g_pMain->GetNowState(), CX2State::SUCM_RESURRECT_CANCEL );
						}
						break;
					}
					tempCount++;
				}
			}
		}

#endif // FIX_RESURRECTION_INDEX
	}
}

void	CX2BattleFieldGame::ClearNpcCreatedInfoList() 
{ 
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_deqNpcCreatedInfo.clear(); 
	m_UnitLoader.RemoveAllNPCResources();
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_vecNpcCreatedInfo.clear(); 
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_bIsNpcLoadComplete = false;
}

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS

CX2GUNPC * CX2BattleFieldGame::GetChainedBossGroup ( CX2GUNPC * pGUNPC_ )
{
	// �켱 ����, 1���� : Main �� ����, 2���� : �ֿ켱 �˻��Ǵ� ����
	if ( pGUNPC_ != NULL )
	{
		int uiGroupID = pGUNPC_->GetBossGroupID();
		int  iUnitID   = pGUNPC_->GetUID();
		CX2GUNPC * pSecondPickedNPC = NULL;		// 2���� NPC, Main Boss ���� ���� �� ��ȯ�Ǹ� 
		// ù��° �˻��� ���� Pointer �� ���� �ִ´�.
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		CX2GUNPC * pEventBossNPC = NULL;		// �̺�Ʈ ���� NPC�� �ְ� MainBoss�� ���ٸ� ��ȯ �Ѵ�.
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
		for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
		{
			CX2GUNPC * pNPC = m_NPCUnitList[i];
			if ( NULL != pNPC )
			{
				// ���� �׷��̰� �ڱ� �ڽ��� �ƴ϶��..�� ������ ���� �׷쿡 ���� �����̴�!
				if ( pNPC->GetBossGroupID() == uiGroupID && pNPC->GetUID() != iUnitID )
				{
					if ( pNPC->GetIsMainBoss() )		// Main Boss �� ã�Ҵٸ� �ٷ� ��ȯ�ϰ� ��
					{
						return pNPC;
					}
					else if ( pSecondPickedNPC == NULL )	// 2���� NPC �� ����, 
					{
						pSecondPickedNPC = pNPC;
					}
				}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
				//�̺�Ʈ ������ �ִٸ� �̺�Ʈ ������ ��ȯ�Ѵ�.
				if( pNPC->GetBossGroupID() > 100 && pNPC->GetUID() != iUnitID )
				{
					pEventBossNPC = pNPC;
				}
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
				
			}
		}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		if( pSecondPickedNPC == NULL && pEventBossNPC != NULL )
		{
			return pEventBossNPC;
		}
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
		return pSecondPickedNPC; // ������ ���� �Դٴ� �� Main boss �� ���ٴ� ��
	}
	return NULL;				 // �ش� NPC �� NULL �̶�� NULL ��ȯ
}

void CX2BattleFieldGame::UpdateCreateUnitDisplayEffect ( CX2GUNPC * pNPC_, bool bIsBossSummoned_ )
{
	// bIsBossSummoned_ �� ������ ������������ true, �׾��� ���� false

	// SetFadeInOutNotice �Լ��� ���, �׷� �̸��� ��Ÿ����.
	CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	m_wstrBossGroupName = battleFieldManager.GetBattleFieldBossDataGroupName(battleFieldManager.GetBattleFieldIdWhereIam(), pNPC_->GetBossGroupID() );
	if ( m_wstrBossGroupName != L"NoData" )
	{
		wstring tempEx = L" ";
		if ( true == bIsBossSummoned_ )
		{
			// �λ縦 ���, @1 �����Ͽ����ϴ�. ( ��/ �� ) 
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
			tempEx = GET_REPLACED_STRING ( ( STR_ID_23588, "L", std::wstring(L"") ) );
#else
			// �λ縦 ���, @1 �����Ͽ����ϴ�. ( ��/ �� ) 
			tempEx = GET_REPLACED_STRING ( ( STR_ID_23588, "I", KHanSungPostWordUnicodeWrapper( ( WCHAR* ) m_wstrBossGroupName.c_str(), STR_ID_806, STR_ID_807 ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			
		}
		else
		{
			// �λ縦 ���, @1 �����Ͽ����ϴ�. ( ��/ �� ) 
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
			tempEx = GET_REPLACED_STRING ( ( STR_ID_23589, "L", std::wstring(L"") ) );
#else
			// �λ縦 ���, @1 óġ�Ͽ����ϴ�.. ( ��/ �� ) 
			tempEx = GET_REPLACED_STRING ( ( STR_ID_23589, "I", KHanSungPostWordUnicodeWrapper( ( WCHAR* ) m_wstrBossGroupName.c_str(), STR_ID_198, STR_ID_199 ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			
		}

		m_wstrBossGroupName += tempEx;
		static_cast <CX2State *> ( g_pMain->GetNowState() )->SetFadeInOutNotice( m_wstrBossGroupName.c_str(), 5.f);
	}
	
	// ���Ͱ� �������� ��, ī�޶� �� �Է� ����, ����Ʈ�� ����Ѵ�.
	if ( true == bIsBossSummoned_ )
	{	
		m_pCinematicUI->SetCinematicUI ( true, 4.f );		// 4�ʰ� Cinematic UI 
		pNPC_->SetFocusCameraForce( true );					// ī�޶� ���� ����
		GetMyUnit()->SetCanNotInputTime_LUA( 4.f );			// Input �� 4�� ���� ���ް�
	}
}


void CX2BattleFieldGame::SetIndicatorToNpc ( CX2GUNPC * pNPC_ )
{
	if ( NULL == m_pMonsterIndicator )
	{
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		if( pNPC_->GetBossGroupID() > 100 )
			m_pMonsterIndicator = new CX2MonsterIndicator(CX2MonsterIndicator::MIT_FIELD_EVENT_BOSS);
		else
			m_pMonsterIndicator = new CX2MonsterIndicator(CX2MonsterIndicator::MIT_FIELD_MIDDLE_BOSS);
#else //SERV_BATTLEFIELD_EVENT_BOSS_INT
		m_pMonsterIndicator = new CX2MonsterIndicator(CX2MonsterIndicator::MIT_FIELD_MIDDLE_BOSS);
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	else
	{
		SAFE_DELETE( m_pMonsterIndicator );
		if( pNPC_->GetBossGroupID() < 100 )
		{
			m_pMonsterIndicator = new CX2MonsterIndicator(CX2MonsterIndicator::MIT_FIELD_MIDDLE_BOSS);
		}
		else if( pNPC_->GetBossGroupID() > 100 )
		{
			m_pMonsterIndicator = new CX2MonsterIndicator(CX2MonsterIndicator::MIT_FIELD_EVENT_BOSS);
		}
	}
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	m_pMonsterIndicator->SetTargetNPC( pNPC_ );
}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef SERV_CHECK_EXIST_MONSTER_UID
/** @function : StartElapsedTimeSinceGettingFirstNpcSyncPacket
	@brief : �� �Լ��� ȣ�� �� �� 10�� ���� �Ŀ� NPC�� ù ��ũ ��Ŷ�� �޾Ҵ����� �˻��Ѵ�.
*/
void CX2BattleFieldGame::StartElapsedTimeSinceGettingFirstNpcSyncPacket()
{
	SetCanCheckGettingFirstNpcSyncPacket( true );
	m_ElapsedTimeSinceGettingFirstNpcSyncPacket.ResetSumOfElapsedTime();
}

/** @function : CheckNpcNotReceiveFirstSyncPacketAndRequestToCheckNpcUidList
	@brief : ù ��ũ ��Ŷ�� ������ NPC�� ������ �Ŀ� ������ üũ�� ��û�ϴ� �Լ�
*/
void CX2BattleFieldGame::CheckNpcNotReceiveFirstSyncPacketAndRequestToCheckNpcUidList()
{
	KEGS_CHECK_EXIST_MONSTER_UID_REQ kPacket;
	CheckNpcNotReceiveFirstSyncPacketAndCollectNpcUid( kPacket.m_vecNpcUID );

	if ( false == kPacket.m_vecNpcUID.empty() )
		Handler_EGS_CHECK_EXIST_MONSTER_UID_REQ( kPacket );
}

/** @function : CheckNpcNotReceiveFirstSyncPacketAndCollectNpcUid
	@brief : ��ũ��Ŷ�� �ѹ��� ������ NPC���� üũ�ϰ�, �ش� UID�� �����ϴ� �Լ�
	@param : ��ũ��Ŷ�� �ѹ��� ������ NPC�� UID�� ��Ƴ��� ����
*/
void CX2BattleFieldGame::CheckNpcNotReceiveFirstSyncPacketAndCollectNpcUid( vector<int>& vecNpcUidToCheck_ ) const
{
	BOOST_FOREACH( CX2GUNPC* pGUNpc, m_NPCUnitList )
	{
		/// ���� ù ��ũ ��Ŷ�� �ѹ��� ���� ���� NPC���
		if ( NULL != pGUNpc && false == pGUNpc->DidReceiveFirstSyncPacket() )
			vecNpcUidToCheck_.push_back( pGUNpc->GetUID() );	/// �ش� NPC�� UID�� üũ ��Ͽ� �ִ´�.
	}	
}

/** @function : Handler_EGS_CHECK_EXIST_MONSTER_UID_REQ
	@brief : ��ũ��Ŷ�� ������ NPC���� UID �� ������ ���� �ϴ� �Լ�
	@param : ������ ������ EGS_CHECK_EXIST_MONSTER_UID_REQ ��Ŷ���� �׾ȿ��� ��ũ��Ŷ�� ������ NPC���� UID ����Ʈ�� �ִ�.
*/
void CX2BattleFieldGame::Handler_EGS_CHECK_EXIST_MONSTER_UID_REQ( const KEGS_CHECK_EXIST_MONSTER_UID_REQ& kPacket_ )
{
 	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_EXIST_MONSTER_UID_REQ, kPacket_ );
 	g_pMain->AddServerPacket( EGS_CHECK_EXIST_MONSTER_UID_ACK );
}

/** @function : Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK
	@brief : ���� �ʵ忡 �������� �ʴ� ������ UID�� ��� ��Ŷ ó��, �ش� NPC���� ������ �ش�.
	@return : ��Ŷ�� ���������� ó�� ������ true, �ƴϸ� false
*/
bool CX2BattleFieldGame::Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_EXIST_MONSTER_UID_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_CHECK_EXIST_MONSTER_UID_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
			if ( false == kPacket.m_vecNotExistNpcUID.empty() )
			{
				StateLog( L"m_vecNotExistNpcUID has elements" );
				BOOST_FOREACH( const int iNpcUid, kPacket.m_vecNotExistNpcUID )
				{
					DeleteNPCUnitByUID( iNpcUid );
				}
			}
			return true;
		}
	}

	return false;
}

#endif // SERV_CHECK_EXIST_MONSTER_UID


#ifdef NEW_CHARACTER_EL
/** @function : GetIsExsitancePartyMemberHPLessThanPer
	@brief : ��Ʋ�ʵ忡���� ������ false
*/
/*virtual*/ 
bool CX2BattleFieldGame::GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const
{
	return false;
}
#endif // NEW_CHARACTER_EL

#ifdef FIELD_BOSS_RAID
/** @function : CreateRaidPotalParticle
	@brief : ���̵� ��Ż ����.
			 ������ 1���� ���� �Ǵ� ������ ����.
*/
void CX2BattleFieldGame::CreateRaidPotal()
{
	DestoryRaidPotalParticle();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hRaidGateParticle && 
		true == CX2BossRaidManager::GetInstance()->IsActiveRaidPortal() )
	{
		if( NULL != m_pWorld && NULL != m_pWorld->GetLineMap() )
		{
			const int iNumOfLineData = m_pWorld->GetLineMap()->GetNumLineData();
			for( int i=0; i < iNumOfLineData; ++i )
			{
				const CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( i );
				if( NULL != pLineData )
				{
					if( CKTDGLineMap::LT_NORMAL == pLineData->lineType &&
						true == pLineData->m_bIsPortalWhenActiveRaid )
					{
						// �� �� ����Ʈ
						CX2WorldObjectParticle* pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), "RaidFieldGate" );
						if( NULL != pParticle )
						{	
							CKTDGParticleSystem::CParticleEventSequence* pSeq = pParticle->GetParticleEventSequence();
							if( NULL != pSeq )
							{
								/// ���θ� ���Ⱚ�� ���� ��ġ ��������.
								D3DXVECTOR3 vPos = ( pLineData->startPos + pLineData->endPos ) * 0.5f;
								vPos.z += 200.f * cos( pLineData->dirRadian.y );
								vPos.x += 200.f * sin( pLineData->dirRadian.y );

								pSeq->SetPosition( vPos );						/// ��ġ ����
								pSeq->SetAddRotate( pLineData->dirDegree );		/// ȸ�� ����
							}

							pParticle->SetLayer( XL_EFFECT_0 );
							m_hRaidGateParticle = pParticle->GetHandle();
						}

						// �� ����Ʈ
						m_vecRaidGateEffectParticle.resize(0);

						for ( int i = 0; i < 7; ++i )
						{
							char buf[256] = {0,};

							StringCchPrintfA( buf, 255, "BossRaid_Gate_P0%d", i + 1 );

							pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), buf );
							if( NULL != pParticle )
							{	
								CKTDGParticleSystem::CParticleEventSequence* pSeq = pParticle->GetParticleEventSequence();
								if( NULL != pSeq )
								{
									/// ���θ� ���Ⱚ�� ���� ��ġ ��������.
									D3DXVECTOR3 vPos = ( pLineData->startPos + pLineData->endPos ) * 0.5f;
									vPos.z += 200.f * cos( pLineData->dirRadian.y );
									vPos.x += 200.f * sin( pLineData->dirRadian.y );

									pSeq->SetPosition( vPos );						/// ��ġ ����
									pSeq->SetAddRotate( pLineData->dirDegree );		/// ȸ�� ����
								}

								pParticle->SetLayer( XL_EFFECT_0 );
								m_vecRaidGateEffectParticle.push_back( pParticle->GetHandle() );
							}
						}

						// �ٴ� ����Ʈ
						pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), "Peita_Teleport_MagicSquare01" );
						if( NULL != pParticle )
						{
							pParticle->SetParticlePos( ( pLineData->startPos + pLineData->endPos ) * 0.5f + D3DXVECTOR3(0, 3, 0 ) );
							pParticle->SetLayer( XL_EFFECT_0 );
							m_hRaidPortalParticle = pParticle->GetHandle();
						}

						// ī�޶�� ����Ʈ�� ���� �� �����ֱ�
						if( NULL != GetWorld() )
						{
							SEnum::VILLAGE_MAP_ID eCurrentVillageID = static_cast<SEnum::VILLAGE_MAP_ID>(g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam());
							const int iCameraIndex = CX2BossRaidManager::GetInstance()->GetCameraIndex( eCurrentVillageID );
							GetWorld()->SetTriggerCamera( iCameraIndex, 2.f );
						}

						g_pKTDXApp->GetDeviceManager()->PlaySound( L"ES_FieldRaidBoss_Gate_Open.ogg", false, false );;
						break;
					}
				}
			}
		}
	}
}
void CX2BattleFieldGame::DestoryRaidPotalParticle()
{
	if( NULL != GetMajorParticle() )
	{
		GetMajorParticle()->DestroyInstanceHandle( m_hRaidGateParticle );
		m_hRaidGateParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;

		GetMajorParticle()->DestroyInstanceHandle( m_hRaidPortalParticle );
		m_hRaidPortalParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;

		BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequenceHandle hRaidGateEffectParticle, m_vecRaidGateEffectParticle )
		{
			GetMajorParticle()->DestroyInstanceHandle( hRaidGateEffectParticle );
			hRaidGateEffectParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}
}
/** @function : CreateLoadingUI
	@brief : �ʵ� ���� �� ����� ����UI
			 ���� �ڵ�� FieldGame�ڵ� ���ܿ�
*/
void CX2BattleFieldGame::CreateLoadingUI()
{	
	// ������ �������� or ä���̵�
	if( NULL == m_pDLGLoadingState )
		m_pDLGLoadingState = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_PVP_Game_Loading_State.lua", 0.0f );
	ASSERT( m_pDLGLoadingState != NULL );

	if ( m_pDLGLoadingState != NULL )
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );

	wstring wstrImgL = L"";
	wstring wstrImgR = L"";
	CX2BossRaidManager::GetInstance()->GetLoadingUIFileName(wstrImgL, wstrImgR);
	bool bCheck = false;
	if(wstrImgL.empty() == false && wstrImgR.empty() == false )
	{
		CKTDGUIStatic* pStatic = ( m_pDLGLoadingState != NULL ) ? static_cast<CKTDGUIStatic*>(m_pDLGLoadingState->GetControl( L"Static_Loading_Dungeon_BG" )) : NULL;

		// �׸� ����
		if( pStatic != NULL )
		{
			if(pStatic->GetPicture(0) != NULL)
			{
				pStatic->GetPicture(0)->SetShow(false);
			}

			bool bCheck1 = false;
			bool bCheck2 = false;
			if(pStatic->GetPicture(1) != NULL)
			{
				bCheck1 = pStatic->GetPicture(1)->SetTex( wstrImgL.c_str() );
			}
			if(pStatic->GetPicture(2) != NULL)
			{
				bCheck2 = pStatic->GetPicture(2)->SetTex( wstrImgR.c_str() );
			}
			bCheck = bCheck1 && bCheck2;


			if(bCheck == true)
			{
				pStatic->SetShow(true);
				m_fLoadUIReaminTime = 0.f;
			}
		}
	}

	if( false == bCheck )
	{
		m_fLoadUIReaminTime = -1.f;
	}
}
void CX2BattleFieldGame::OnFrameMove_LoadingUI(double fTime, float fElapsedTime)
{
	if( m_fLoadUIReaminTime < 0.f )
		return;

	m_fLoadUIReaminTime += fElapsedTime;
	if(m_fLoadUIReaminTime >= 3.f)
	{
		// �ε�ȭ�� ����
		if(m_pDLGLoadingState != NULL)
			SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );

		m_fLoadUIReaminTime = -1.f;
	}
	else
	{
		if( m_pDLGLoadingState != NULL )
			m_pDLGLoadingState->OnFrameMove( fTime, fElapsedTime );
	}
}
#endif // FIELD_BOSS_RAID