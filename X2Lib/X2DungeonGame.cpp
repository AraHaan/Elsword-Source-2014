#include "StdAfx.h"
#include ".\x2dungeongame.h"



#ifdef REDUCED_PRECOMPILED_HEADER_TEST
//#include "./X2GUElsword_SwordMan.h"
//#include "./X2GUArme_VioletMage.h"
//#include "./X2GULire_ElvenRanger.h"
//#include "./X2GURaven.h"
#include "./X2GuEve.h"
#endif REDUCED_PRECOMPILED_HEADER_TEST



CX2DungeonGame::CX2DungeonGame(void) :
m_pDungeonRoom( NULL ),
m_pDungeon( NULL ),
m_WinTeam( CX2Room::TN_NONE ),
m_bGoNextPacketSended( false ),
m_bShowClear( false ),
m_fClearEffDelayTime( 0.f ),
#ifdef DUNGEON_TIME_OUT_FRAMEMOVE // oasis907 : ����� [2010.4.26] // 
m_fDungeonClearLimitTimeOut( 0.f ),
m_bDungeonClearLimitTimeOut( false ),
m_bShowDungeonClearLimitTimeOut( false ),	// �ʱ�ȭ �� �� �ִ� ���� ���� by ������
#endif DUNGEON_TIME_OUT_FRAMEMOVE
m_bCountTimeLeftAfterSubStageOpen( false ),
m_fTimeLeftAfterSubStageOpen( 0.f ),
m_pFontForTime( NULL ),
m_bIsSendSkipContinueNot( false ),
m_bIsThereTutorialMsg( false ), 		
m_pFontForTutorialMsg( NULL ), 
m_wstrTutorialMsg( L"" ),
m_fTimeLeftForTutorialMessage( 0.f ),
m_coTutorialMsg( D3DXCOLOR(1,1,1,1) ),
m_coTutorialMsgIncrement( D3DXCOLOR(-0.04f, -0.04f, -0.04f, 0.f) ),
m_iTutorialMsgFlashCount( 0 ),
m_bShowTutorialUI( false ),
m_bEnableRemainTime( false ),
m_fRemainPlayTime( 0.f ),
m_bStartRealTraining( false ),
m_pTrainingGameUI( NULL ),
m_bShowDungeonRemainTime( false ),
m_bShowDungeonPlayTimeForQuest( false ), 
m_bHideDungeonPlayTimeAfterLoading( false ),
//m_ReservedPacket;
//m_StopContinueAlarmText[256];
//m_TimerManyPurpose1;
//m_TimerManyPurpose2;
m_pMonsterIndicator( NULL ),
#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
m_pDamageDataForDungeonTimeOut( NULL ),
#endif DUNGEON_TIME_OUT_FRAMEMOVE
m_bKilledBoss( false ),
m_hBossName(INVALID_PARTICLE_SEQUENCE_HANDLE)
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
, m_iDefenceDungeonWaveID( -1 )		// ����� �� ���꽺������( Wave )
#endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
, m_bEscape( true )
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
, m_bShowStageReaminTime( false )
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
{
	// super class member init
	m_GameType = GT_DUNGEON;

	// variable assign
	m_pFontForTime			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL );
	m_pFontForTutorialMsg	= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_20_BOLD );
	m_pDLGContinue	= new CKTDGUIDialog( this, L"DLG_Dungeon_Game_Continue_New.lua" );
	m_pDLGContinue->SetShow( false );
	m_pDLGContinue->SetEnable( false );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGContinue );

#ifdef _GAMANIA_HK_
	CKTDGUIStatic* pStaticRestore_Continue_For_Cash = (CKTDGUIStatic*)m_pDLGContinue->GetControl( L"Restore_Continue_Cash" );
	if( NULL != pStaticRestore_Continue_For_Cash )
	{
		pStaticRestore_Continue_For_Cash->GetPicture(0)->SetTex_LUA( "DLG_Common_New_Texture24_A.tga", "CONTINUE_MESSAGE_2" );
	}
#endif _GAMANIA_HK_

	m_pSoundContinue			= g_pKTDXApp->GetDeviceManager()->OpenSound( L"Continue.ogg" );

#ifndef X2TOOL
	m_bShowDungeonPlayTimeForQuest		= DetermineShowDungeonPlayTime();
#endif

	memset( m_StopContinueAlarmText, 0, sizeof(WCHAR)*256 );

	//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
	if ( g_pInstanceData != NULL )
	{
		g_pInstanceData->SetSendGameStartReqPacket( false );		// packet�� �������� ���� ���·� �ʱ�ȭ
	}
#endif MODIFY_INFINITE_SKILL_BUG
	//}} kimhc // 2010.3.26 // ���� ��ų ���� ����

#ifdef CHECK_SUB_STAGE_GO_NEXT
	m_bCheckSubStageGoNext = false;
#endif

#ifdef COME_BACK_REWARD
	m_pDLGTitle			= NULL;
	m_fDungeonNameTime	= 0.f;
#endif

}

CX2DungeonGame::~CX2DungeonGame(void)
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
/*virtual*/ void CX2DungeonGame::Release()
{
	SAFE_DELETE( m_pMonsterIndicator );
	SAFE_DELETE( m_pDungeon );

	SAFE_DELETE( m_pTrainingGameUI );


	SAFE_DELETE_DIALOG( m_pDLGContinue );

	SAFE_CLOSE( m_pSoundContinue );

#ifdef COME_BACK_REWARD
	SAFE_DELETE_DIALOG( m_pDLGTitle );
#endif

//{{ robobeg : 2013-06-12
	//g_pData->GetUnitManager()->UnloadAllNPCInitData();
//}} robobeg : 2013-06-12

	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
	{
		g_pData->GetPlayGuide()->DeleteQuestGuideParticle();
		g_pData->GetPlayGuide()->DeleteInputGuideParticle();
	}

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hBossName )
		GetMinorParticle()->DestroyInstanceHandle(m_hBossName);

	CX2Game::Release();	/// ���� Ŭ������ �Ҹ��ڰ� ���߿� ȣ��
}

HRESULT CX2DungeonGame::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2Game::OnFrameMove( fTime, fElapsedTime );

	if( m_GameState == GS_PLAY )
	{
#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
		//{{ oasis907 : ����� [2010.4.26] // 
		DungeonTimeOutFrameMove( fTime, fElapsedTime );
		//}}
#endif DUNGEON_TIME_OUT_FRAMEMOVE
		CountTimeSubStageFrameMove( fTime, fElapsedTime );

		if( m_pDungeon != NULL )
			m_pDungeon->OnFrameMove( fTime, fElapsedTime );

		if( NULL != m_pMonsterIndicator )
		{
			m_pMonsterIndicator->OnFrameMove();
		}


		if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			if( m_fRemainPlayTime > 0.0f )
				m_fRemainPlayTime -= fElapsedTime;

			if( m_fRemainPlayTime <= 0.0f )
				m_fRemainPlayTime = 0.0f;
		}

#ifdef COME_BACK_REWARD
		if ( m_pDLGTitle != NULL )
		{
			m_fDungeonNameTime += fElapsedTime;
			if(m_fDungeonNameTime >= 2.f && m_fDungeonNameTime <= 3.f)
			{
				CloseDungeonName(m_fDungeonNameTime);
			}
			else if(m_fDungeonNameTime > 3.f)
			{
				m_fDungeonNameTime = 0.f;
				SAFE_DELETE_DIALOG( m_pDLGTitle );
			}
		}
#endif

		if( m_bShowClear == true )
		{
			if( m_fClearEffDelayTime <= 0.0f )
			{
				m_bShowClear = false;
				m_fClearEffDelayTime = 0.0f;

				ShowClearEffect();
			}
			else
			{
				m_fClearEffDelayTime -= m_fElapsedTime;
			}			
		}

		if( m_bGoNextPacketSended == false && m_pDungeon->GetNowStage()->GetNowSubStage() != NULL )
		{
			CX2GUUser* pCX2GUUser = GetMyUnit();
			if( pCX2GUUser != NULL && pCX2GUUser->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
			{
				const CX2GUUser::SyncData& kSyncData = pCX2GUUser->GetSyncData( false );
				if( m_pDungeon->GetNowStage()->GetNowSubStage()->TouchStartLineMapSet( kSyncData.lastTouchLineIndex ) == true )
				{
#ifdef CHECK_SUB_STAGE_GO_NEXT
					m_bCheckSubStageGoNext = true;
#endif

					//CX2StateDungeonGame* pCX2StateDungeonGame = (CX2StateDungeonGame*)g_pMain->GetNowState();
					Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ( m_pDungeon->GetNowStage()->GetBeforeSubStageIndex(), m_pDungeon->GetNowStage()->GetNowSubStageIndex() );
					m_bGoNextPacketSended = true;

					m_pDungeon->GetNowStage()->CloseBeforeSubStage();
				}				
			}
		}

		if( false == m_bEnableNPCTalkBox )
		{
			m_fElapsedTimeAfterDisableNPCTalkBox += fElapsedTime;
			if( m_fElapsedTimeAfterDisableNPCTalkBox > 2.f )
			{
				m_bEnableNPCTalkBox = true;
			}
		}
		
		
		m_pDLGContinue->SetShow( false );
		m_pDLGContinue->SetEnable( false );

		if( GetResurrectionOperationCond() == true )
		{
			ResurrectionFrameMove( fTime, fElapsedTime );
		}


		if( CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID() )
		{
			if( NULL != m_pTrainingGameUI )
			{
				m_pTrainingGameUI->OnFrameMove( fTime, fElapsedTime );
			}

			if( true == m_bEnableRemainTime )
			{
				if( m_fRemainPlayTime > 0.0f )
					m_fRemainPlayTime -= fElapsedTime;

				if( m_fRemainPlayTime <= 0.0f )
					m_fRemainPlayTime = 0.0f;
			}
		}
	}

	return S_OK;
}

HRESULT CX2DungeonGame::OnFrameRender()
{
	CX2Game::OnFrameRender();

	if( m_GameState == GS_PLAY )
	{

		if( true == m_bCountTimeLeftAfterSubStageOpen )
		{
			// ���� substage ���� ��� ���� �ð�
			WCHAR timeBuf[200] = {0,};
			swprintf( timeBuf, L"%d", (int)m_fTimeLeftAfterSubStageOpen );
			m_pFontForTime->OutTextXY( 512, 100, timeBuf, D3DXCOLOR(1,0.6f,0,1), 
									CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}
		else
		{
			//TextOutCurrQuestState();
		}

		

		if( CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID() && true == m_bStartRealTraining &&
			false == g_pMain->GetIsPlayingFreeTraining() )
		{


			// �Ʒ� ���� ���� �ð�
			float fTime = GetRemainPlayTime();

			WCHAR wszText[64] = L"";
			int iMinute = (int)(fTime / 60.f);
			int iSecond = (int)(fTime-(float)iMinute*60.f);

			if( 0 != iMinute )
			{
				if( iSecond < 10 )
				{
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"%2d:0%d", iMinute, iSecond );
				}
				else
				{
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"%2d:%2d", iMinute, iSecond );
				}
			}
			else
			{
				if( iSecond < 10 )
				{
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"0%d", iSecond );
				}
				else
				{
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"%2d", iSecond );
				}
			}

			m_pFontForTime->OutTextXY( 512, 100, wszText, D3DXCOLOR(1,0.6f,0,1), 
							CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}

		if( CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() && false == g_pMain->GetIsPlayingTutorial() )
		{
			if( false == m_bHideDungeonPlayTimeAfterLoading && 
				false == m_bCountTimeLeftAfterSubStageOpen )
			{
				
				float fTime = -1.f;
				if( true == m_bShowDungeonRemainTime )					// ���� ���� ���� �ð� ǥ��
				{
					fTime = m_fRemainPlayTime;
				}
				else if( true == m_bShowDungeonPlayTimeForQuest )		// ���� ���� �ð� ǥ��
				{
					fTime = m_pDungeonRoom->GetPlayTimeLimit() - m_fRemainPlayTime;
				}	
#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
				//{{ oasis907 : ����� [2010.5.6] // Ư�� �ð� ������ Ư���� ����� ó�ϴ� ����
				else if( true == m_bShowDungeonClearLimitTimeOut )
				{
					fTime = m_fDungeonClearLimitTimeOut;
				}
				//}}
#endif DUNGEON_TIME_OUT_FRAMEMOVE
#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
				else if( m_bShowStageReaminTime == true )
				{
					fTime = m_pDungeon->GetNowStage()->GetNowSubStage()->GetRemainTimeInClearCondition();
				}				
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION

#ifdef DIALOG_SHOW_TOGGLE	
				if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#endif
				{
					if( fTime > 0.f )
					{
						WCHAR wszText[64] = L"";
						int iMinute = (int)(fTime / 60.f);
						int iSecond = (int)(fTime-(float)iMinute*60.f);

						if( 0 != iMinute )
						{
							if( iSecond < 10 )
							{
								StringCchPrintf(wszText, 64, L"%2d:0%d", iMinute, iSecond ); 
								//wsprintf( wszText, L"%2d:0%d", iMinute, iSecond );
							}
							else
							{
								StringCchPrintf( wszText, 64, L"%2d:%2d", iMinute, iSecond );
								//wsprintf( wszText, L"%2d:%2d", iMinute, iSecond );
							}
						}
						else
						{
							if( iSecond < 10 )
							{
								StringCchPrintf( wszText, 64 ,  L"0%d", iSecond );
								//wsprintf( wszText, L"0%d", iSecond );
							}
							else
							{
								StringCchPrintf( wszText, 64 ,  L"%2d", iSecond );
								//wsprintf( wszText, L"%2d", iSecond );
							}
						}

						m_pFontForTime->OutTextXY( 512, 100, wszText, D3DXCOLOR(1,1,0,1), 
							CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
					}
				}				
			}
		}
	}

	return S_OK;
}


bool CX2DungeonGame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2Game::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	return false;
}

void CX2DungeonGame::GameLoading( CX2Room* pRoom )
{	
	const int LEVEL_DEFFERENCE_BETWEEN_DUNGEON_AND_USER = 20;
	const int LEVEL_DEFFERENCE_BETWEEN_OTHERUSER_AND_ME = 13;

	m_pDungeonRoom	= (CX2DungeonRoom*) pRoom;
	m_pRoom			= pRoom;

	SystemLoading();
#ifdef X2TOOL
	DungeonLoading(g_pInstanceData->m_bIsNpcLoad);
#else
	DungeonLoading();
#endif
	UnitLoading();	
	EtcLoading();


	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		// 09.03.23 �¿� : �̴ϸ� UI �ڵ庯��
		//g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->SetTitle( dungeonName.c_str() );
		if( false == g_pMain->GetIsPlayingTutorial() &&
			CX2Main::XS_TRAINING_GAME != g_pMain->GetNowStateID() )
		{
			g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );
			
			wstring dungeonName = L"";
			if( NULL != m_pDungeonRoom )
			{
				dungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( m_pDungeonRoom->GetDungeonID(), m_pDungeonRoom->GetDifficulty(), 
					(CX2Dungeon::DUNGEON_MODE) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode );				
			}
			
			g_pInstanceData->GetMiniMapUI()->SetTitle( dungeonName.c_str() );



	//{{ kimhc // 2010.5.2 // ��д��� �۾�(����ġ)
	#ifdef	SERV_SECRET_HELL
			g_pInstanceData->GetMiniMapUI()->SetNoExpAtThisDungeon( false );
			g_pInstanceData->GetMiniMapUI()->SetWarningForGettingExp( false );

			switch ( g_pData->GetDungeonManager()->GetDungeonType( m_pDungeon->GetDungeonData()->m_DungeonID ) )
			{
			case CX2Dungeon::DT_NORMAL:
				{
					//{{ kimhc // 2011-04-28 // ���潺���� 
			#ifdef	SERV_INSERT_GLOBAL_SERVER
					if ( false == g_pData->GetDungeonManager()->IsDefenceDungeon( m_pDungeon->GetDungeonData()->m_DungeonID ) )					
			#endif	SERV_INSERT_GLOBAL_SERVER
					//}} kimhc // 2011-04-28 // ���潺���� 
					{
						int levelDifference = abs( m_pDungeon->GetDungeonData()->m_MinLevel - g_pData->GetSelectUnitLevel() );
						if( levelDifference >= LEVEL_DEFFERENCE_BETWEEN_DUNGEON_AND_USER )
							g_pInstanceData->GetMiniMapUI()->SetNoExpAtThisDungeon( true );
					}
				} break;

			case CX2Dungeon::DT_SECRET:
				{
					int		iLevel	= ( GetMyUnit() != NULL ) ? GetMyUnit()->GetUnitLevel() : 0;
					int		iSize	= GetUserUnitListSize();
					bool	bShow	= false;

					if ( iLevel > m_pDungeon->GetDungeonData()->m_MaxLevel
						 && iLevel - m_pDungeon->GetDungeonData()->m_MaxLevel >= LEVEL_DEFFERENCE_BETWEEN_DUNGEON_AND_USER )
							bShow = true;
					else if ( iLevel < m_pDungeon->GetDungeonData()->m_MinLevel 
							  && m_pDungeon->GetDungeonData()->m_MinLevel - iLevel >= LEVEL_DEFFERENCE_BETWEEN_OTHERUSER_AND_ME )
							bShow = true;
					else
					{
						for ( int i = 0; i < iSize; i++ )
						{
							CX2GUUser* pUser = GetUserUnit( i );

							if ( pUser != NULL )
							{
								int iOtherLevel = pUser->GetUnitLevel();

								if ( iOtherLevel > iLevel && 
									iOtherLevel - iLevel >= LEVEL_DEFFERENCE_BETWEEN_OTHERUSER_AND_ME )
								{
									bShow = true;
									break;
								}
							}
						}
					}

					g_pInstanceData->GetMiniMapUI()->SetWarningForGettingExp( bShow );

				} break;

			default:
			case CX2Dungeon::DT_HENIR:
				break;
			}
			
	#else	SERV_SECRET_HELL
			if( true == g_pData->GetDungeonManager()->IsHenirDungeon( m_pDungeon->GetDungeonData()->m_DungeonID ) )
			{
				g_pInstanceData->GetMiniMapUI()->SetNoExpAtThisDungeon( false );
			}
			else
			{
				int levelDifference = abs( m_pDungeon->GetDungeonData()->m_MinLevel - g_pData->GetSelectUnitLevel() );
				if( levelDifference >= 20 )
				{
					g_pInstanceData->GetMiniMapUI()->SetNoExpAtThisDungeon( true );
				}
				else
				{
					g_pInstanceData->GetMiniMapUI()->SetNoExpAtThisDungeon( false );
				}
			}
	#endif	SERV_SECRET_HELL
	//}} kimhc // 2010.5.2 // ��д��� �۾�(����ġ)
			
		}
		

		

	}

	m_fRemainPlayTime = m_pDungeonRoom->GetPlayTimeLimit();
	


	
	if( NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
	{
		g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
	}

	SAFE_DELETE( m_pMonsterIndicator );
	if( false == g_pMain->GetIsPlayingTutorial() )		// Ʃ�丮�󿡼��� ���� ���� ǥ�� ȭ��ǥ �ȳ�����
	{
		if( g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
			m_pMonsterIndicator = new CX2MonsterIndicator();
	}

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
	if ( g_pKTDXApp->GetDeviceManager() != NULL )
		g_pKTDXApp->GetDeviceManager()->ReleaseAllMemoryBuffers();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

}

void CX2DungeonGame::GameStart()
{
	//PVP������ �ٷ� �����ϸ� ������
	//���������� ���� ��������, ���� ���������� �ε���� �ʾ����Ƿ�
	//�������� �ε带 �����Ѵ�
	if( IsHost() == true )
	{
		//�������� �ε带 ��û�Ѵ�
	}

	//{{ JHKang / ������ / 2011.6.26 / �޸� ���� ����( ���ƿ� ��� )
#ifdef COME_BACK_REWARD
	SAFE_DELETE_DIALOG( m_pDLGTitle );

	m_pDLGTitle = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Buff_Title.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTitle );
	CKTDGUIStatic *pStaticDlg = m_pDLGTitle->GetStatic_LUA("Buff_Title_Field_Created");

	CKTDGUIControl::CPictureData *pFieldComeBackUser = pStaticDlg->GetPictureIndex( 123 );
	if(pFieldComeBackUser != NULL)
	{
		pFieldComeBackUser->SetShow( false );
		if ( NULL != g_pData->GetPremiumBuffManager() && true == g_pData->GetPremiumBuffManager()->IsComeBackUser() || true == g_pData->GetPremiumBuffManager()->IsComeBackUserInParty() )
		{
			pFieldComeBackUser->SetShow( true );
		}
	}

#endif
	//}}
#ifndef FIX_RESET_COOLTIME_AT_ENTER_DUNGEON	// ����ȯ
#ifdef FIX_COOLTIME_NOT_INIT_BUG
	if( g_pData != NULL && 
		g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuickSlot()->ResetCoolTime();
	}
#endif //FIX_COOLTIME_NOT_INIT_BUG
#endif // FIX_RESET_COOLTIME_AT_ENTER_DUNGEON
}

void CX2DungeonGame::StageLoading( int stageNum )
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Pause();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	wstringstream dungeonStageIndex;
	dungeonStageIndex << L"Stage Loading : " << stageNum;
	StateLog( dungeonStageIndex.str().c_str() );

	m_GameState			= GS_LOADING;
	m_bStopUnitChain	= true;

	//���������� �ε��϶�� NOT�� ���� �ε��� �����Ѵ�
	for( int i = 0; i < (int)m_CreateNPCDataList.size(); i++ )
	{
		CreateNPCData* pCreateNPCData = m_CreateNPCDataList[i];
		SAFE_DELETE( pCreateNPCData );
	}

#ifdef APPLY_THEMIDA
	ENCODE_START
#endif

	m_CreateNPCDataList.resize(0);

	DeleteAllNPCUnit();
	m_pDropItemManager->DeleteAllItem();

#ifdef DUNGEON_ITEM
	ClearSpirit();
#endif

#ifdef APPLY_THEMIDA
	ENCODE_END
#endif

	CX2DungeonStage* pCX2DungeonStage = m_pDungeon->CreateStage( stageNum );
	CX2DungeonStage::StageData* pStageData = pCX2DungeonStage->GetStageData();


	SAFE_DELETE( m_pWorld );
	
	//m_pWorld = pCX2DungeonStage->FlushWorld();
	if( pStageData != NULL )
    {
//#ifdef BACKGROUND_LOADING_TEST // 2008-12-12
//        m_pWorld = g_pData->GetWorldManager()->ThreadRequest_CreateWorld( pStageData->m_WorldID );
//#else // BACKGROUND_LOADING_TEST // 2008-12-12
#ifdef X2TOOL
		if( pStageData->m_WorldID == CX2World::WI_NONE )
			m_pWorld = g_pData->GetWorldManager()->CreateWorld(	pStageData->m_wstrWorldDataName );
		else
			m_pWorld = g_pData->GetWorldManager()->CreateWorld(	pStageData->m_WorldID );
#else
		m_pWorld = g_pData->GetWorldManager()->CreateWorld(	pStageData->m_WorldID );
#endif

#ifdef DEEP_WATER_DUNGEON_TEST
		BOOST_TEST_FOREACH( CX2GUUser*, pUser, m_UserUnitList )
		{
			if( NULL == pUser )
				continue;

			pUser->ResetPhysicParam();
			pUser->InitPassiveSkillState();
		}
#endif DEEP_WATER_DUNGEON_TEST

		// ù �������� �̸� �ش� ������ default ���� �������� ���� ID�� dungeon lounge world id�� �����Ѵ�
		if( 0 == stageNum )
		{
			g_pInstanceData->SetDungeonLoungeWorldID( m_pDungeon->GetDungeonData()->m_eDefaultDungeonLoungeWorldID ); 
		}

//#endif // BACKGROUND_LOADING_TEST // 2008-12-12

		
#ifdef PORTAL_LINEMAP_TEST
		// note!! dmlee 2008.12.21 �ϴ� ��Ż ���θ���ġ�� ��Ż ǥ�� ��ƼŬ�� ���⼭ �����ϴ� �ɷ� �ϵ� �ڵ�
		if( NULL != m_pWorld &&
			NULL != m_pWorld->GetLineMap() )
		{
			for( int i=0; i<m_pWorld->GetLineMap()->GetNumLineData(); i++ )
			{
				const CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( i );
				if( NULL != pLineData &&
					CKTDGLineMap::LT_POTAL == pLineData->lineType )
				{
					CX2WorldObjectParticle* pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), "Peita_Teleport_MagicSquare01" );
					if( NULL != pParticle )
					{
						pParticle->SetParticlePos( ( pLineData->startPos + pLineData->endPos ) * 0.5f + D3DXVECTOR3(0, 3, 0 ) );
						pParticle->SetLayer( XL_EFFECT_0 );
					}
					if( true == g_pMain->GetIsPlayingTutorial() )
					{
						CX2WorldObjectParticle* pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), "CompleteQuest" );
						if( NULL != pParticle )
						{
							pParticle->SetParticlePos( ( pLineData->startPos + pLineData->endPos ) * 0.5f + D3DXVECTOR3(0, 250, 0 ) );
							pParticle->SetLayer( XL_EFFECT_0 );
						}
					}
				}
			}
		}
#endif PORTAL_LINEMAP_TEST

    }//if

	// dungeon script���� ������ color�� world�� ����
	if( m_pDungeon->GetWorldColor().r != -1.f )
	{
		m_pWorld->SetOriginColor_LUA( m_pDungeon->GetWorldColor().r, m_pDungeon->GetWorldColor().g, m_pDungeon->GetWorldColor().b );
	}
	else if( pStageData->m_WorldColor.r != -1.f )
	{
		m_pWorld->SetOriginColor_LUA( pStageData->m_WorldColor.r, pStageData->m_WorldColor.g, pStageData->m_WorldColor.b );
	}

	if( m_pDungeon->GetUnitColor().r != -1.f )
	{
		m_pWorld->SetLightColor_LUA( m_pDungeon->GetUnitColor().r, m_pDungeon->GetUnitColor().g, m_pDungeon->GetUnitColor().b );
	}

	else if( pStageData->m_UnitColor.r != -1.f )
	{
		m_pWorld->SetLightColor_LUA( pStageData->m_UnitColor.r, pStageData->m_UnitColor.g, pStageData->m_UnitColor.b );
	}



	

	// �Ʒü� �����̸� linemap�� landheight�� ���� ����
	if( CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID() )
	{

		if( NULL != m_pWorld &&
			NULL != m_pWorld->GetLineMap() )
		{
			m_pWorld->GetLineMap()->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() - 300.f );
		}
	}

	

	// user unit���� ���ġ
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = (CX2GUUser*)m_UserUnitList[i];
		if( pCX2GUUser == NULL )
			continue;

		pCX2GUUser->ClearRecieveSyncData();	
		pCX2GUUser->InitPosition( true );
		if( pCX2GUUser->GetNowHp() > 0.0f || pCX2GUUser->GetNowStateID() != CX2GameUnit::GUSI_DIE )
		{
			if( pStageData->m_bStartMotion == true ) // ù��° �Ǵ� ������ stage���� start motion�� ���Ѵ�.
			{
#ifdef RIDING_SYSTEM
				if ( pCX2GUUser->GetRidingOn() )
					pCX2GUUser->StateChange( pCX2GUUser->GetRidingStartStateID(), true );
				else
					pCX2GUUser->StateChange( pCX2GUUser->GetStartStateID(), true );
#else //RIDING_SYSTEM
				pCX2GUUser->StateChange( pCX2GUUser->GetStartStateID(), true );
#endif //RIDING_SYSTEM
			}
			else
			{
#ifdef RIDING_SYSTEM
				if ( pCX2GUUser->GetRidingOn() )
					pCX2GUUser->StateChange( pCX2GUUser->GetRidingWaitStateID(), true );
				else
					pCX2GUUser->StateChange( pCX2GUUser->GetWaitStateID(), true );
#else //RIDING_SYSTEM
				pCX2GUUser->StateChange( pCX2GUUser->GetWaitStateID(), true );
#endif //RIDING_SYSTEM
			}
		}

		pCX2GUUser->SetPepperRunTime( 0.f );
		
		//GetX2Camera()->NomalDirectCamera( (CX2GameUnit*) pCX2GUUser );
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), g_pMain->GetGameOption().GetCameraDistance() );

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
	if ( g_pKTDXApp->GetDeviceManager() != NULL )
		g_pKTDXApp->GetDeviceManager()->ReleaseAllMemoryBuffers();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

}

void CX2DungeonGame::StageStart()
{
	//ȣ��Ʈ�� ���� ���������� �ε��϶�� ����� ������

	if( m_bBGMOn == true )
		m_pWorld->PlayBGM();

	if( m_pDungeon->GetNowStage()->GetStageData()->m_bStartMotion == false )
		g_pKTDXApp->GetDGManager()->StartFadeIn( 99999 );

	m_GameState = GS_PLAY;
	m_pWorld->GetLineMap()->DisableAllLineData();
	m_bKilledBoss = false;

	m_pCamera->NomalDirectCamera( m_optrFocusUnit.GetObservable(), g_pMain->GetGameOption().GetCameraDistance() );
	m_pCamera->OnFrameMove( 0, 0 );

#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser != NULL )
		{
			pCX2GUUser->InitPosition( true );
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

#ifndef _SERVICE_
	dbg::clog << L"StageStart! Index: " << m_pDungeon->GetNowStageIndex() << L" WORLD ID : " << (int)m_pDungeon->GetNowStage()->GetStageData()->m_WorldID << dbg::endl;
#endif
	wstringstream dungeonStageIndex;
	dungeonStageIndex << L"Stage Start : " << m_pDungeon->GetNowStageIndex();
	StateLog( dungeonStageIndex.str().c_str() );

#ifndef X2TOOL
	if( IsHost() == true )
	{		
		Handler_EGS_DUNGEON_SUB_STAGE_OPEN_REQ( 0 );
	}
#endif

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Resume();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    if ( g_pData->GetGameUDP() != NULL )
        g_pData->GetGameUDP()->RemoveAllPendingPingSends();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
	if( NULL != m_pDungeon && NULL != m_pDungeon->GetDungeonData() &&
		m_pDungeon->GetDungeonData()->m_bShowStageTime == true )
	{
		m_bShowStageReaminTime = true;
	}
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{
		g_pData->GetRelationshipEffectManager()->SetMyRelationCloseEffectCooltime( 0.f );
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

}


void CX2DungeonGame::SubStageOpen( int subStageNum )
{
	for( int i = 0; i < (int)m_CreateNPCDataList.size(); i++ )
	{
		CreateNPCData* pCreateNPCData = m_CreateNPCDataList[i];
		SAFE_DELETE( pCreateNPCData );
	}
	m_CreateNPCDataList.resize(0);

#ifdef CHRISTMAS_EVENT_2013
	IF_EVENT_ENABLED( CEI_CHRISTMAS_EVENT_2013 )
	{
		if( NULL != g_pData->GetDungeonRoom() && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() );

			// �������� ������ �����ϴ� ù ���꽺������������ ��ȯ
			if( m_pDungeon->GetNowStageIndex() == 0 &&
				subStageNum == 0 &&
				g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level >= pDungeonData->m_MinLevel - 2 &&
				g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level <= pDungeonData->m_MaxLevel + 2 &&
				CX2Dungeon::IsHenirDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) == false &&
				g_pData->GetDungeonRoom()->GetDungeonID() != SEnum::DI_RUBEN_EL_TREE_NORMAL &&
				g_pData->GetDungeonRoom()->GetDungeonID() != SEnum::DI_RUBEN_RUIN_OF_ELF_NORMAL &&
				g_pData->GetDungeonRoom()->GetDungeonID() != SEnum::DI_RUBEN_SWAMP_NORMAL )
			{
				CX2GUUser* pCX2GUUser = GetMyUnit();
				if( pCX2GUUser->GetUnit()->GetInventory().GetItemByTID( _CONST_SERV_2013_CHRISTMAS_EVENT_::iPresentBagItemID ) != NULL )
				{
					g_pX2Game->Handler_EGS_NPC_UNIT_CREATE_REQ( CX2UnitManager::NUI_SOCCER_PPORU, pCX2GUUser->GetUnitLevel(), true,
						pCX2GUUser->GetPos(), pCX2GUUser->GetIsRight(), 5.0f, true, -1, CX2Room::TN_RED,
						CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID(), false, CX2Room::TN_RED, CX2GUNPC::NCT_MONSTER_CARD );
				}
			}
		}
	}
#endif CHRISTMAS_EVENT_2013
	
	CX2DungeonSubStage* pCX2DungeonSubStage				= m_pDungeon->GetNowStage()->CreateSubStage( subStageNum );

	CX2DungeonSubStage::SubStageData* pSubStageData		= pCX2DungeonSubStage->GetSubStageData();
	CX2DungeonSubStage* pBeforeSubStage					= m_pDungeon->GetNowStage()->GetBeforeSubStage();

	if( pBeforeSubStage != NULL )
	{
#ifdef DYNAMIC_PORTAL_LINE_MAP
		// ������ // 2013-09-04 // 
		// HOST�� ������ SUB_STAEG_OPEN_NOT�� ����, 
		// HOST�� �ƴ� ������ Ŭ���� ������ üũ �� �� ���� ������. 
		// Ŭ���� �� �����ؾ� �ϴ� ���� �߰� ó��
		if( false == IsHost() )
		{
			pBeforeSubStage->ProcessAfterSubStageClear_NotHost();
		}
#endif // DYNAMIC_PORTAL_LINE_MAP
		pBeforeSubStage->ToggleEndLineMapSet( false );
	}
	if( pCX2DungeonSubStage != NULL )
		pCX2DungeonSubStage->ToggleStartLineMapSet( true );

	m_bGoNextPacketSended	= false;
			

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	if ( NULL != g_pX2Room && NULL != m_pDungeon )	// null check
	{
		if ( g_pX2Room->GetUserNum() > 1 )			// ��Ƽ ���� ��
		{			
			if ( subStageNum <= 0 && m_pDungeon->GetNowStageIndex() <= 0 ) // ù��° ��������, ù��° ���� �������� �� ���
			{
				m_pDungeon->SetBeforeStageIndexUsingPacket(0);				// Index �� �ʱ�ȭ �����ش�.
				m_pDungeon->SetBeforeSubStageIndexUsingPacket(0);
			}
			else
			{
				// ù��° ��������, ù��° ���� ���������� �ƴ� ���, �ش� ������ ������ ������ �����Ѵ�.
				Send_EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT (m_pDungeon->GetBeforeStageIndexUsingPacket(), 
														   m_pDungeon->GetBeforeSubStageIndexUsingPacket() );				
			}
		}
	}

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

	if( subStageNum != 0 )
	{
		if( false == g_pMain->GetIsPlayingTutorial() &&
			g_pX2Room->GetUserNum() > 1 ) 
		{
			CX2GUUser* pCX2GUUser = GetMyUnit();
			if( pCX2GUUser != NULL && CX2GameUnit::GUSI_DIE != pCX2GUUser->GetGameUnitState() )
			{			
				m_bCountTimeLeftAfterSubStageOpen = true;
				m_fTimeLeftAfterSubStageOpen = 20.0f;
			}
		}
	}
	else
	{
#ifdef CHECK_SUB_STAGE_GO_NEXT
		m_bCheckSubStageGoNext = true;
#endif
		//CX2StateDungeonGame* pCX2StateDungeonGame = (CX2StateDungeonGame*)g_pMain->GetNowState();
	
		Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ( m_pDungeon->GetNowStage()->GetBeforeSubStageIndex(), m_pDungeon->GetNowStage()->GetNowSubStageIndex() );
		m_bGoNextPacketSended = true;

		m_pDungeon->GetNowStage()->CloseBeforeSubStage();
	}





	m_bEnableNPCTalkBox = false;
}

void CX2DungeonGame::SubStageLoading( int subStageNum )
{
#ifdef X2TOOL
	return;
#endif

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Pause();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	wstringstream dungeonSubStageIndex;
	dungeonSubStageIndex << L"Sub Stage Loading : " << subStageNum;
	StateLog( dungeonSubStageIndex.str().c_str() );
	// ���� ����
	m_pDungeon->SetDungeonMapOpen( m_pDungeon->GetNowStageIndex(), subStageNum, true );
	CX2StateDungeonGame* pStateDungeonGame = (CX2StateDungeonGame*) g_pMain->GetNowState();
	if( NULL != pStateDungeonGame )
	{
		if( pStateDungeonGame->GetDungeonMapUI() != NULL )
			pStateDungeonGame->GetDungeonMapUI()->CreateDungeonMapUI();
	}

	g_pKTDXApp->SkipFrame();
}

void CX2DungeonGame::SubStageStart()
{
#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif
	//�帷�� ���� ���� ���������� �����Ѵ�
	m_bLastKillCheck		= false;
	m_fLastKillWaitTime		= 0.05f;

	m_bShowClear			= false;
	m_fClearEffDelayTime	= 0.0f;

	CX2DungeonSubStage* pBeforeSubStage = m_pDungeon->GetNowStage()->GetBeforeSubStage();
	CX2DungeonSubStage* pNowSubStage	= m_pDungeon->GetNowStage()->GetNowSubStage();
	if( pBeforeSubStage != NULL )
		pBeforeSubStage->BreakCurtain();

#ifdef FIX_DUNGEON_IS_NULL
	if( m_pDungeon == NULL || m_pDungeon->GetNowStage() == NULL || m_pDungeon->GetNowStage()->GetNowSubStage() == NULL )
	{
		if(g_pData != NULL && g_pData->GetServerProtocol() != NULL )
		{
			if( g_pData != NULL && g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
			}
		}	

#ifdef ADD_COLLECT_CLIENT_INFO
		g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_01, "", true, true );
#else
		string strMailTitle = ANTI_HACK_STRING_AntiHacking_Process_Dungeon_Is_NULL_SubStageStart;
		g_pMain->SendHackMail_DamageHistory(strMailTitle.c_str());
		g_pKTDXApp->SetFindHacking( true );
#endif
		return;
	}
#endif




	m_pDungeon->GetNowStage()->DeleteBeforeSubStage();
	m_pDungeon->GetNowStage()->GetNowSubStage()->Start();
	m_pDungeon->GetNowStage()->StopGoEffect();
	m_bCountTimeLeftAfterSubStageOpen = false;


	pNowSubStage->SwapToGameNPCUnit();

	wstringstream dungeonSubStageIndex;
	dungeonSubStageIndex << L"Sub Stage Start : " << pNowSubStage->GetSubStageIndex();
	StateLog( dungeonSubStageIndex.str().c_str() );



	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL )
		{
			pCX2GUNPC->SetAIEnable( true );

			if( NULL != pCX2GUNPC->GetNPCAI() && 
				CX2NPCAI::NAT_ALLY == pCX2GUNPC->GetNPCAI()->GetAIType() )
			{
				// nothing
			}
			else
			{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                if ( pCX2GUNPC->GetNowStateID() != CX2GameUnit::GUSI_DIE )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    				pCX2GUNPC->StateChangeForce( pCX2GUNPC->GetStartState() );
			}

			for( int i = 0; i < (int)pNowSubStage->GetSubStageData()->m_NPCDataList.size(); i++ )
			{
				CX2DungeonSubStage::NPCData* pNPCData = pNowSubStage->GetSubStageData()->m_NPCDataList[i];
				if( pNPCData != NULL && pCX2GUNPC->GetUID() == pNPCData->m_UID )
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    if ( pCX2GUNPC->GetNowStateID() != CX2GameUnit::GUSI_DIE )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    {
					    pCX2GUNPC->SetSiegeModeForce( false );
					    pCX2GUNPC->SetSiegeModeForce( pNPCData->m_bSiegeMode );
                    }
				}
			}


			//����Ʈ ���Ÿ�� �ɰ����� ������~~~~~~~~~~~?
			
			map< int, KAttribEnchantNpcInfo  >::const_iterator mit
				= GetDungeon()->GetNowStage()->GetStageData()->m_mapAttribEnchantNpcInfo.find( pCX2GUNPC->GetUID() );
			if ( mit != GetDungeon()->GetNowStage()->GetStageData()->m_mapAttribEnchantNpcInfo.end() )
			{
				const KAttribEnchantNpcInfo& kAttribEnchantNpcInfo = mit->second;
				pCX2GUNPC->ApplyEnchantMonster( kAttribEnchantNpcInfo );
			}
		}
	}


	if( NULL != pNowSubStage )
	{
		pNowSubStage->SetMonsterCountForSubStageClearCheck();
	}

#ifndef X2TOOL
	CX2StateDungeonGame* pStateDungeonGame = (CX2StateDungeonGame*) g_pMain->GetNowState();
	if( NULL != pStateDungeonGame )
	{
		int subStageNum = pNowSubStage->GetSubStageIndex();
		if( subStageNum == 0 )
		{
			// �������� �ε��߿� ZŰ�� ������ �ִٰ� �������� �������� ���� ������ �ʻ�� ������ ���� ���� ���ؼ� 
			if( NULL != GetMyUnit() )
				GetMyUnit()->SetNowChargeMpForDetonation( 0.f );					

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_DUNGEON_GAME:
				{
					if( false == g_pMain->GetIsPlayingTutorial() )
					{
						const map< std::pair<int,int>, CX2Dungeon::DungeonMapData >& mapDungeonMapData = m_pDungeon->GetMapDungeonMapData();
						map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::const_iterator it;
						it = mapDungeonMapData.find( std::make_pair( m_pDungeon->GetNowStageIndex(), subStageNum ) );
						if( it != mapDungeonMapData.end() )
						{
							const CX2Dungeon::DungeonMapData& dungeonMapData = it->second;
							if( false == dungeonMapData.m_bIsBossStage )
							{
								pStateDungeonGame->GetDungeonMapUI()->OnFirstSubStage();
							}
						}
					}
				} break;
			}
		}
	}
#endif //X2TOOL

	// �̺��� ��ȯ ���͸� ĳ���� ������ �̵����� �ش�
	for( int i=0; i< GetUnitNum(); i++ )
	{
		CX2GameUnit* pUnit = GetUnit(i);
		if( NULL == pUnit )
			continue;

		if( CX2GameUnit::GUT_USER != pUnit->GetGameUnitType() )
			continue;

		CX2GUUser* pUser = (CX2GUUser*) pUnit;
		if( NULL != pUser->GetUnit() )
		{
#ifdef FIX_SUMMONED_MONSTER_POSITION
			pUser->CallSummonedNPC();
#else //FIX_SUMMONED_MONSTER_POSITION
			if( CX2Unit::UT_EVE == pUser->GetUnit()->GetType() )
			{
				CX2GUEve* pEve = (CX2GUEve*) pUser;
				pEve->CallSummonedNPC();
			}
#endif //FIX_SUMMONED_MONSTER_POSITION

			// ����� ���� ������ ���� ���� ����
			pUser->SetShieldDamageReduce( 1.f );
		}
	}




	if( true == m_bShowTeleportEffectOnStageStart )
	{
		m_bShowTeleportEffectOnStageStart = true; // FieldFix: �̰� ��� �� ��
		CreateStageLoadingTeleportEffectForAllUser( false );
	}

	
	m_bStopUnitChain = false;

	m_fElapsedTimeAfterDisableNPCTalkBox = 0.f;

#ifdef X2TOOL
	g_pInstanceData->m_bChangeStage = true;
#endif
	CanNotInputAndDisalbeNPCAI(false);	

	g_pKTDXApp->SkipFrame();

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	if( true == IsHost() )
	{
		CreateAllyEventMonster();
	}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Resume();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    if ( g_pData->GetGameUDP() != NULL )
        g_pData->GetGameUDP()->RemoveAllPendingPingSends();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

#ifdef SERV_9TH_NEW_CHARACTER
	if( NULL != GetMyUnit() && GetMyUnit()->GetUnitType() == CX2Unit::UT_ADD )
	{
		GetMyUnit()->InitPhaseShift();

		/// �������� ��ȯ��, �������ε� ���� ������ �ɷ����� �ʾҴٸ� ���� ��������
		if ( true == GetMyUnit()->GetIsFormationMode() && false == GetMyUnit()->IsApplyBuffByBuffTempletID( BTI_FORMATION_MODE ) )
			GetMyUnit()->SetIsFormationMode( false );

		/// �������� ��ȯ��, ������ �Ǿ� ���� ������ ���� ������ �ɷ��ִٸ� ���� ���� ����
		if ( false == GetMyUnit()->GetIsFormationMode() && true == GetMyUnit()->IsApplyBuffByBuffTempletID( BTI_FORMATION_MODE ) )
			GetMyUnit()->EraseBuffTempletFromGameUnit( BTI_FORMATION_MODE );
	}
#endif //SERV_9TH_NEW_CHARACTER
}





void CX2DungeonGame::DungeonLoading(bool bIsNpcLoad)
{
	SEnum::DUNGEON_ID dungeonID = (SEnum::DUNGEON_ID)(m_pDungeonRoom->GetDungeonID() + m_pDungeonRoom->GetDifficulty());
	SAFE_DELETE( m_pDungeon );
	m_pDungeon = g_pData->GetDungeonManager()->CreateDungeon( dungeonID, bIsNpcLoad );


	if( NULL != m_pDungeon &&
		NULL != m_pDungeon->GetDungeonData() &&
		m_pDungeon->GetDungeonData()->m_fTimeLimit > 0.f )	// �ð� ������ �ִ� �����̶�� ���� �÷��� ���� �ð� ǥ��
	{
		m_bShowDungeonRemainTime = true;
	}

#ifdef EXTRACT_ALL_DUNGEON_NPC_INFO
	ExtractAllDungeonNPCInfo_DevOnly( "EXTRACT_ALL_DUNGEON_NPC_INFO.txt" );
#endif EXTRACT_ALL_DUNGEON_NPC_INFO

}

bool CX2DungeonGame::DetermineLastKill() 
{ 
	KTDXPROFILE();

	if( m_bLastKillCheck == true )
		return false;

	int remainRedTeam = 0;	// �������� �����ִ� ��� ��
	int dyingRedTeam = 0;	// ���� ��� ��
	
	int remainMonster = 0;  // ������ �����ִ� ���� ��
	int dyingMonster = 0;	// ���� ���� ���� ���� ��

#ifdef SUBBOSS_MONSTER_KILL_EFFECT
	//{{ oasis907 : ����� [2010.4.29] // 
	bool bIsNotLastKill = false;
	//}}
#endif SUBBOSS_MONSTER_KILL_EFFECT

#ifdef FIX_WRONG_CLEAR
	CX2GameUnit* pLastRedTeam = NULL;
	CX2GameUnit* pLastMonster = NULL;
#endif FIX_WRONG_CLEAR
	CX2GameUnit* pLastUnit = NULL;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];

		if ( pCX2GameUnit == NULL )
		{
			continue;
		}

		// ���� ���̰� ���� �״� ������Ʈ�� �ƴϸ�
		if( pCX2GameUnit->GetTeam() == CX2Room::TN_RED &&
			pCX2GameUnit->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			// ������ �����ִ� ��� �� ����
			remainRedTeam++;
			if( pCX2GameUnit->GetNowHp() <= 0.0f )
			{
#ifdef FIX_WRONG_CLEAR
				if( pLastRedTeam != NULL )
				{
					// �������� �̹� ������ ���� �� ���� �ð� üũ �ؼ� ���߿� ���� ������� ����
					if( pLastRedTeam->GetDieAfterTime() > pCX2GameUnit->GetDieAfterTime() )
						pLastRedTeam = pCX2GameUnit;
				}
				else
				{
					pLastRedTeam = pCX2GameUnit;
				}
#else FIX_WRONG_CLEAR
				// pLastUnit�� �ƹ��� �����Ǿ� ���� ������ ����
				if( pLastUnit != NULL )
				{
					// �������� �̹� ������ ���� �� ���� �ð� üũ �ؼ� ���߿� ���� ������� ����
					if( pLastUnit->GetDieAfterTime() > pCX2GameUnit->GetDieAfterTime() )
						pLastUnit = pCX2GameUnit;
				}
				else
				{
					pLastUnit = pCX2GameUnit;
				}
#endif FIX_WRONG_CLEAR

				// ������ ���� ��� �� ����
				dyingRedTeam++;
			}
		}
		// ���� ���̰� ���� �״� ������Ʈ�� �ƴϸ�
		else if( pCX2GameUnit->GetTeam() == CX2Room::TN_MONSTER &&
				 pCX2GameUnit->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			// ���� ��� ��Ƽ�� ���� ���� üũ
			if( pCX2GameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{
				CX2GUNPC* pCX2GUNPC = (CX2GUNPC*)pCX2GameUnit;
				if( pCX2GUNPC->IsActiveMonster() == false )
					continue;
			}

			// �����ִ� ���� ��
			remainMonster++;
			if( pCX2GameUnit->GetNowHp() <= 0.f )
			{
				// HP�� 0�̴��� NEVER_DIE_THIS_STATE�� true �� �н�
				//{{ kimhc // 2010-07-21 // ���� ������Ʈ���� HP�� 0�� �Ǿ �׾��ٰ� �Ǵ����� �ʰ� �� �� �ִ� ���
				if ( static_cast< CX2GUNPC* >( pCX2GameUnit )->GetNeverDieThisState() == true )
					continue;
					//}} kimhc // 2010-07-21 // ���� ������Ʈ���� HP�� 0�� �Ǿ �׾��ٰ� �Ǵ����� �ʰ� �� �� �ִ� ���

#ifdef FIX_WRONG_CLEAR
				if( pLastMonster != NULL )
				{
					// ���߿� ���� �������� ����
					if( pLastMonster->GetDieAfterTime() > pCX2GameUnit->GetDieAfterTime() )
						pLastMonster = pCX2GameUnit;
				}
				else
				{
					// �����Ǿ� ���� ������ ����
					pLastMonster = pCX2GameUnit;
				}
#else FIX_WRONG_CLEAR
				// pLastUnit�� �����Ǿ� ������
				if( pLastUnit != NULL )
				{
					// ���߿� ���� �������� ����
					if( pLastUnit->GetDieAfterTime() > pCX2GameUnit->GetDieAfterTime() )
						pLastUnit = pCX2GameUnit;
				}
				else
				{
					// �����Ǿ� ���� ������ ����
					pLastUnit = pCX2GameUnit;
				}
#endif FIX_WRONG_CLEAR

				// ���� ���� ���� ���� �� ����
				dyingMonster++;
			}
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	// ���� ���� ���� ���� ���� ������ �۰ų� ������ �Ǵ� ���� ������ ���� ���� ������ ������ �۰ų� ������
	// �����غ��� 
	if( remainRedTeam <= dyingRedTeam || remainMonster <= dyingMonster )
	{
		m_fLastKillWaitTime -= m_fElapsedTime;		// Ÿ���� ������
	}

#ifdef SUBBOSS_MONSTER_KILL_EFFECT
	//{{ oasis907 : ����� [2010.4.29] // ���� ���� �������� ���� �ߺ� ���� ���� ų���� ���� 
	else
	{
		// �̰� �� �� �� �ȵ�..
		// �ؿ� m_fLastKillWaitTimeAfterRebirth �� ���� 0���� ������ bIsNotLastKill �� true���� �ϴ°� �ƴѰ�?
		bIsNotLastKill = true;
	}
	//}}
#endif SUBBOSS_MONSTER_KILL_EFFECT

#ifdef FIX_WRONG_CLEAR
	if( remainMonster <= dyingMonster )
	{
		pLastUnit = pLastMonster;
	}
	else
	{
		pLastUnit = pLastRedTeam;
	}
#endif FIX_WRONG_CLEAR

	//
	// ��� ����� ���� �Ŀ� ��Ȱ�� �� ���� �Ŀ��� �������� Ŭ���� �ȵǴ� ���� �ذ��ϱ� ���� �ӽ� ����, ��Ȱ �Ŀ� 1�� ���� ��ٸ���.
	// ����: 
	// ��Ȱ ���Ŀ� ��Ȱ�� ����� HP�� 0���� ���� ���� �Ǿ �����Ŀ����� m_bLastKillCheck �� true�� �Ǿ������, 
	// ���� �������͸� ����� ���� �������� Ŭ���� ó���� �ȵǰ� ��
	if( m_fLastKillWaitTimeAfterRebirth > 0.f )
	{
		m_fLastKillWaitTimeAfterRebirth -= m_fElapsedTime;
		if( m_fLastKillWaitTimeAfterRebirth < 0.f )
		{
			m_fLastKillWaitTimeAfterRebirth = 0.f;
		}
	}


	if( false == m_bLastKillCheck && // FieldFix: �̰� �ǹ� ���� Ȯ�� (������ true������ �Ǵ��Ͽ� retur��Ŵ)
		m_fLastKillWaitTime <= 0.0f &&
		m_fLastKillWaitTimeAfterRebirth <= 0.f )
	{
		// pLastUnit�� �����Ǿ� ������
		if( pLastUnit != NULL )
		{
			bool bLastKillSlowMotion = true;

			// pLastUnit �� �ڱ� �ڽ��̸�
			if( pLastUnit == GetMyUnit() )
			{
				// ��Ŷ ������
				((CX2GUUser*)pLastUnit)->SendFrameData();

				// NPC �߿� focusCamera�� ���� NPC�� �����ϸ�
				CX2GUNPC* pCX2GUNPC = GetFoucusCameraNPC();
				if( pCX2GUNPC != NULL )
				{
					// 100�� ���� �� NPC���� focus��..
					SetTimerFocusUnit( pCX2GUNPC, 100.0f );
				}
				else
				{
					// 1�� ���� pLastUnit(�ڱ��ڽ�) ���� ��Ŀ����...
					SetTimerFocusUnit( pLastUnit, 1.0f );
				}
			}
			// pLastUnit �� NPC��
			else if( pLastUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
			{		
				// ȣ��Ʈ�� ���
				if( IsHost() == true )
				{


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    if ( g_pData->GetGameUDP()->GetNonRelayUIDs().empty() == false
                        || g_pData->GetGameUDP()->GetRelayUIDs().empty() == false )
                    {
					    for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
					    {
						    CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
						    if( pCX2GUNPC != NULL )
							    pCX2GUNPC->SendPacketImmediateForce( m_kFrameUDPPack );
					    }
                    }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//					//�Ʒ��� �ǹ̾��� �ڵ��� �� ����( by hcsung )
//					// ��ü NPC�� ��Ŷ�� ������... (pLastUnit�� ������ �Ǵ°� �ƴѰ�??)
//					KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;
//
//					for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
//					{
//						CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
//						if( pCX2GUNPC != NULL )
//							pCX2GUNPC->SendPacketImmediateForce( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
//					}
//
//
//
//					BroadCast_XPT_UNIT_NPC_SYNC_PACK();
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

					//KSerBuffer buff;
					//Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );
					//g_pData->GetGameUDP()->BroadCast( GetVecUserUIDforSyncPacket(), XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );

				}
								
				//Ŭ���� ó��

				//{{ ����� : [2011/3/22/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
				if( NULL != g_pData->GetDungeonRoom() && g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) == false )
#endif SERV_INSERT_GLOBAL_SERVER
				//}} ����� : [2011/3/22/] //	���� �̼�
				{
					// ���潺 ������ ��쿡�� Clear�� �ߴµ�.. �̰� �и��ؾ� �ҵ�...
					SetClearEffect( 1.0f );
				}

				//{{ dmlee 2008.06.03 ����, �߰� ������ ���� �� ���ο� ���
				CX2GUNPC* pLastDyingNPC = (CX2GUNPC*) pLastUnit;
				if( false == pLastDyingNPC->GetShowBossName() && 
					false == pLastDyingNPC->GetShowSubBossName() &&
					false == pLastDyingNPC->GetHasBossGage() )
				{
					// �̷� �ֵ��� ���ο��� ����
					bLastKillSlowMotion = false;
				}

				// ��ϸ��� �и��ϴ°� ������...
#ifdef HENIR_TEST
				// HENIR ������ ��쿡 ������ ���ΰɷ� üũ�Ǹ� EGS_PLAY_TIME_OUT_NOT�� ���� ������ �� ������ ������.
				if( CX2Dungeon::IsHenirDungeon( m_pDungeon->GetDungeonData()->m_DungeonID ) )
#endif HENIR_TEST
				{
					// ��ϸ� ������ �ƴ� ��� ���������� �ִ� �ְ� ������
					if( true == pLastDyingNPC->GetHasBossGage() )
					{
						m_bKilledBoss = true;
					}
				}

				if( true == bLastKillSlowMotion )
				//}} dmlee 2008.06.03 ����, �߰� ������ ���� �� ���ο� ���
				{
					// �̰͵�.. �и� �ϴ°� �����ٵ�...
					if( CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID() )
					{
						SetTimerFocusUnit( GetMyUnit(), 1.0f );
					}
					else
					{
						// �������� 1.5�� ����, ī�޶� �Ÿ� 800
						SetTimerFocusUnit( pLastUnit, 1.5f, 800.f );
					}	
				}

				// ������ ���� �Ÿ�
				if( true == m_bKilledBoss )
				{
					// �ǴϽ� ����Ʈ!!
					if( NULL != pLastUnit )
					{
						D3DXVECTOR3 vPos = pLastDyingNPC->GetPos();
						vPos.y += pLastDyingNPC->GetUnitHeight( false ) * 0.5f;

						CKTDGParticleSystem::CParticleEventSequence* pSeq1 = 
							GetMinorParticle()->CreateSequence( NULL,  L"Finish_Particle01", vPos, -1, -1 );

						CKTDGParticleSystem::CParticleEventSequence* pSeq2 = 
							GetMinorParticle()->CreateSequence( NULL,  L"Finish_Particle02", vPos, -1, -1 );

						if( NULL != pSeq1 && NULL != pSeq2 )
						{
							CX2GUNPC* pNPC = (CX2GUNPC*) pLastUnit;
							pSeq1->SetLandPosition( pNPC->GetLandPosition_LUA().y );
							pSeq2->SetLandPosition( pNPC->GetLandPosition_LUA().y );
						}

						CKTDGParticleSystem::CParticleEventSequence* pSeq3 = 
							GetMinorParticle()->CreateSequence( NULL,  L"Finish_Particle03", D3DXVECTOR3(0, 0, 0), -1, -1 );
						if( NULL != pSeq3 )
						{
							pSeq3->CreateNewParticle( D3DXVECTOR3(0, 0, 0) );
							g_pKTDXApp->GetDeviceManager()->PlaySound( L"Boss_Finish.ogg", false, false ); 
						}

						GetMinorParticle()->CreateSequence( NULL,  L"Dungeon_Finish_FadeIn", D3DXVECTOR3(0, 0, 0), -1, -1 );

#ifdef SUBBOSS_MONSTER_KILL_EFFECT
						//{{ oasis907 : ����� [2010.4.29] // 
						pLastDyingNPC->SetPlayBossKillEffect(false);
						//}}
#endif SUBBOSS_MONSTER_KILL_EFFECT

					}
				}

			}

#ifdef SET_LAST_KILL_SHOT_HIDE_UI // ����ȯ
			// ���θ���̸� �����ӵ� ��������
			if( true == bLastKillSlowMotion )
			{
				g_pKTDXApp->StopFrame( 80 ); 
				g_pKTDXApp->GetDGManager()->ClearScreen( 5 );
				g_pKTDXApp->SlowFrame( 0.2f, 1.5f ); 

				/// ���ο� ����̸�, �ε��� ĸó Ÿ�̹��� �ڷ� �̷��.
				m_fLastkillShotTime = 0.3f;
			}
			else
				m_fLastkillShotTime = 0.05f;
#else // SET_LAST_KILL_SHOT_HIDE_UI
			m_fLastkillShotTime = 0.05f;

			// ���θ���̸� �����ӵ� ��������
			if( true == bLastKillSlowMotion )
			{
				g_pKTDXApp->StopFrame( 80 ); 
				g_pKTDXApp->GetDGManager()->ClearScreen( 5 );
				g_pKTDXApp->SlowFrame( 0.2f, 1.5f ); 
			}
#endif // SET_LAST_KILL_SHOT_HIDE_UI

			m_bLastKillCheck = true;
			
			return true;	
		}					
	}

#ifdef SUBBOSS_MONSTER_KILL_EFFECT
	// oasis907 : ����� [2010.4.29] // 
	CX2GUNPC* pLastDyingNPC = NULL;
	if( pLastUnit != NULL && pLastUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
		pLastDyingNPC = (CX2GUNPC*) pLastUnit;

	if(pLastDyingNPC != NULL && pLastDyingNPC->IsPlayBossKillEffect() == true && bIsNotLastKill == true)
	{

		// ���� ������ ���� ���� �̷� ó��.. ������ lastKill�� �ƴϵ���...
		if( true == pLastDyingNPC->GetShowBossName() || 
			true == pLastDyingNPC->GetShowSubBossName() ||
			true == pLastDyingNPC->GetHasBossGage() )
		{
			SetTimerFocusUnit( pLastUnit, 1.5f, 800.f );

			D3DXVECTOR3 vPos = pLastDyingNPC->GetPos();
			vPos.y += pLastDyingNPC->GetUnitHeight( false ) * 0.5f;

			CKTDGParticleSystem::CParticleEventSequence* pSeq1 = 
				GetMinorParticle()->CreateSequence( NULL,  L"Finish_Particle01", vPos, -1, -1 );

			CKTDGParticleSystem::CParticleEventSequence* pSeq2 = 
				GetMinorParticle()->CreateSequence( NULL,  L"Finish_Particle02", vPos, -1, -1 );

			if( NULL != pSeq1 && NULL != pSeq2 )
			{
				CX2GUNPC* pNPC = (CX2GUNPC*) pLastUnit;
				pSeq1->SetLandPosition( pNPC->GetLandPosition_LUA().y );
				pSeq2->SetLandPosition( pNPC->GetLandPosition_LUA().y );
			}

			CKTDGParticleSystem::CParticleEventSequence* pSeq3 = 
				GetMinorParticle()->CreateSequence( NULL,  L"Finish_Particle03", D3DXVECTOR3(0, 0, 0), -1, -1 );
			if( NULL != pSeq3 )
			{
				pSeq3->CreateNewParticle( D3DXVECTOR3(0, 0, 0) );
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Boss_Finish.ogg", false, false ); 
			}

			GetMinorParticle()->CreateSequence( NULL,  L"Dungeon_Finish_FadeIn", D3DXVECTOR3(0, 0, 0), -1, -1 );


			g_pKTDXApp->StopFrame( 80 ); 
			g_pKTDXApp->GetDGManager()->ClearScreen( 5 );
			g_pKTDXApp->SlowFrame( 0.2f, 1.5f ); 

			pLastDyingNPC->SetPlayBossKillEffect(false);
		}
	}
#endif SUBBOSS_MONSTER_KILL_EFFECT

	return false;	
}


void CX2DungeonGame::ShowClearEffect()
{
#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#endif
	{
		GetMajorParticle()->CreateSequence( NULL,  L"Clear_C", 0,0,0, 1000, 1000, 1, 1 );
		GetMajorParticle()->CreateSequence( NULL,  L"Clear_L", 0,0,0, 1000, 1000, 1, 1 );
		GetMajorParticle()->CreateSequence( NULL,  L"Clear_E", 0,0,0, 1000, 1000, 1, 1 );
		GetMajorParticle()->CreateSequence( NULL,  L"Clear_A", 0,0,0, 1000, 1000, 1, 1 );
		GetMajorParticle()->CreateSequence( NULL,  L"Clear_R", 0,0,0, 1000, 1000, 1, 1 );
	}
	
	if( NULL != m_pDungeon &&
		NULL != m_pDungeon->GetNowStage() &&
		NULL != m_pDungeon->GetNowStage()->GetNowSubStage() )
	{
		if( true == m_pDungeon->GetNowStage()->GetNowSubStage()->GetIsBossStage() )
		{
			// ������ ������ ������ �������������� ��� (�Ϲ����� ����) �������������� ������ ���� �� �����ϰ�
			if( false == m_pDungeon->GetNowStage()->GetNowSubStage()->GetBossStageHaveManyNextStage() )
			{
				// ���� �������� Ŭ���� �̺�Ʈ ������ �����Ѵ�
				CX2StateDungeonGame* pState = (CX2StateDungeonGame*) g_pMain->GetNowState();
				pState->GetDungeonEndingEvent()->m_fElapsedTime_ShowClearEffect = 0.f;
			}
			else // ���� ������������ ������ ������ ���� ���� �ִٸ� clearsubstage(CT_GAME)�� ȣ��ɶ� �������������� ���۵ǵ��� �Ѵ�
			{
				// nothing;
			}
		}
	}
}


bool CX2DungeonGame::Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK )
{
	return CX2Game::Handler_EGS_LEAVE_ROOM_ACK( kEGS_LEAVE_ROOM_ACK );
}


bool CX2DungeonGame::Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT )
{
	CX2Game::Handler_EGS_LEAVE_ROOM_NOT( kEGS_LEAVE_ROOM_NOT );

	return true;
}


#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_REQ( int iClearConditionIndex )
{
	KEGS_DUNGEON_SUB_STAGE_CLEAR_REQ kPacket;
	kPacket.m_iClearConditionIndex = iClearConditionIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_SUB_STAGE_CLEAR_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DUNGEON_SUB_STAGE_CLEAR_ACK, 60.0f ); 

	return true;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK& kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_SUB_STAGE_CLEAR_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK.m_iOK ) == true )
		{
			// �����ʿ� ������ �Ϸ� ������ ���� ó���� ����
			GetDungeon()->GetNowStage()->GetNowSubStage()->ClearSubStage( kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK.m_iClearType, kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK.m_iStageIndex, kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK.m_iSubStageIndex );
		}
		return g_pMain->IsValidPacket( kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK.m_iOK );
	}
	return false;
}
#endif // SERV_STAGE_CLEAR_IN_SERVER


bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_OPEN_REQ( int subStageNum )
{
#ifdef X2TOOL
	SubStageOpen( subStageNum );
#else


	KEGS_DUNGEON_SUB_STAGE_OPEN_REQ kPacket;
	kPacket.m_iSubStageID = subStageNum;
	CX2PacketLog::PrintLog( &kPacket );
	
	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_SUB_STAGE_OPEN_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DUNGEON_SUB_STAGE_OPEN_ACK, 60.0f ); 
#endif

	return true;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( KEGS_DUNGEON_SUB_STAGE_OPEN_ACK& kEGS_DUNGEON_SUB_STAGE_OPEN_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_SUB_STAGE_OPEN_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_DUNGEON_SUB_STAGE_OPEN_ACK.m_iOK );
	}
	return false;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( KEGS_DUNGEON_SUB_STAGE_OPEN_NOT& kEGS_DUNGEON_SUB_STAGE_OPEN_NOT )
{
	SubStageOpen( kEGS_DUNGEON_SUB_STAGE_OPEN_NOT.m_iSubStageID );
	return true;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ( int beforeStage, int nextStage )
{
	THEMIDA_VM_START

#ifdef X2TOOL
	CX2DungeonSubStage* pCX2DungeonSubStage	= m_pDungeon->GetNowStage()->GetNowSubStage();
	if( pCX2DungeonSubStage != NULL )
		pCX2DungeonSubStage->CreateCurtain();

	m_bCountTimeLeftAfterSubStageOpen = false;
	m_fTimeLeftAfterSubStageOpen = 0.f;

	m_bCheckSubStageGoNext = false;

	SubStageLoading( nextStage );
	Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ();
#else

	KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ kPacket;
	kPacket.m_iBeforeStageID	= beforeStage;
	kPacket.m_iNextStageID		= nextStage;

	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK, 60.0f ); 

#ifdef CHECK_SUB_STAGE_GO_NEXT
#	ifdef _SERVICE_
	// checked
	ELSWORD_VIRTUALIZER_START
#	endif

	if( m_bCheckSubStageGoNext == false || m_bCheckSubStageGoNext.Verify() == false )
	{
		// ������ �ǽ�
		if(g_pData != NULL && g_pData->GetServerProtocol() != NULL )
		{
			if( g_pData != NULL && g_pData->GetMyUser() != NULL && 
				g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
			}
		}		
		
#ifdef ADD_COLLECT_CLIENT_INFO
		if( g_pMain != NULL )
			g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_34, "", false, true );
#else
		g_pMain->SendHackMailGameStart(0);
#endif
		//g_pKTDXApp->SetFindHacking( true );
	}

	m_bCheckSubStageGoNext = false;

#	ifdef _SERVICE_
	ELSWORD_VIRTUALIZER_END
#	endif
#endif // CHECK_SUB_STAGE_GO_NEXT
#endif //X2TOOL

	THEMIDA_VM_END


	return true;


}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK& kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK.m_iOK ) == true )
		{
			CX2DungeonSubStage* pCX2DungeonSubStage	= m_pDungeon->GetNowStage()->GetNowSubStage();
			if( pCX2DungeonSubStage != NULL )
				pCX2DungeonSubStage->CreateCurtain();

			m_bCountTimeLeftAfterSubStageOpen = false;
			m_fTimeLeftAfterSubStageOpen = 0.f;


		}
		return g_pMain->IsValidPacket( kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK.m_iOK );
	}
	return false;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT& kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT )
{
	SubStageLoading( kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT.m_iNextStageID );
	Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ();
	return true;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ()
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

#ifdef X2TOOL
	SubStageStart();
	if( m_pDungeon->GetNowStage()->GetStageData()->m_bStartMotion == false )
	{
			if(  m_pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageIndex() == 0 )
				g_pKTDXApp->GetDGManager()->StartFadeIn();
	}
	m_mapKilledNPCKeyCode.clear();

	if ( m_pWorld != NULL )
	{
		if ( m_pWorld->GetLineMap() != NULL )
			m_pWorld->GetLineMap()->UpdateLineVB();
	}

	g_pKTDXApp->SkipFrame();

	//{{ kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08
	if ( m_pDungeon->GetNowStage()->GetNowSubStageIndex() == 0 )
	{
		SetCanUseEscFlag( true );
	}
	//}} kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08


#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		g_pData->GetPetManager()->InitPos();
	}
#endif
#else //X2TOOL

#ifdef FIX_DUNGEON_IS_NULL
	if( m_pDungeon == NULL || m_pDungeon->GetNowStage() == NULL || m_pDungeon->GetNowStage()->GetNowSubStage() == NULL )
	{
		if(g_pData != NULL && g_pData->GetServerProtocol() != NULL )
		{
			if( g_pData != NULL && g_pData->GetMyUser() != NULL && 
				g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
			}
		}	

#ifdef ADD_COLLECT_CLIENT_INFO
		g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_02, "", true, true );
#else
		string strMailTitle = ANTI_HACK_STRING_AntiHacking_Process_Dungeon_Is_NULL_SubStageLoadComplete;
		g_pMain->SendHackMail_DamageHistory(strMailTitle.c_str());
		g_pKTDXApp->SetFindHacking( true );
#endif
		return true;
	}
#endif // FIX_DUNGEON_IS_NULL

	KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ kPacket;
	kPacket.m_iSubStageID		= m_pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageIndex();

	CX2PacketLog::PrintLog( &kPacket );

	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, 60.0f ); 

#endif //X2TOOL

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return true;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK& kEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK.m_iOK );
	}
	return false;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( KEGS_DUNGEON_SUB_STAGE_START_NOT& kEGS_DUNGEON_SUB_STAGE_START_NOT )
{
	SubStageStart();
	if( m_pDungeon->GetNowStage()->GetStageData()->m_bStartMotion == false )
	{
		if( kEGS_DUNGEON_SUB_STAGE_START_NOT.m_iSubStageID == 0 )
			g_pKTDXApp->GetDGManager()->StartFadeIn();
	}

	m_mapKilledNPCKeyCode.clear();

	if ( m_pWorld != NULL )
	{
		if ( m_pWorld->GetLineMap() != NULL )
			m_pWorld->GetLineMap()->UpdateLineVB();
	}

	g_pKTDXApp->SkipFrame();

	//{{ kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08
	if ( m_pDungeon->GetNowStage()->GetNowSubStageIndex() == 0 )
	{
		SetCanUseEscFlag( true );
	}
	//}} kimhc // ���� �ε��� ������ ���� ESC ó�� �ȵǰ� �ϴ� �۾� // 2009-06-08


#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		g_pData->GetPetManager()->InitPos();
	}
#endif





	return true;
}

bool CX2DungeonGame::Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket )
{
	g_pData->ResetDungeonResultInfo( kPacket );

// 	if( g_pData != NULL && 
// 		g_pData->GetUIManager() != NULL && 
// 		g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
// 	{
//         g_pData->GetUIManager()->GetUIQuickSlot()->ResetCoolTime();
	// 	}

#ifdef COME_BACK_REWARD
	if ( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() &&
		false == g_pData->GetPremiumBuffManager()->IsComeBackUser())
	{
		g_pData->GetPremiumBuffManager()->SetComeBackUserInParty( false );
	}
#endif
	return true;
}

bool CX2DungeonGame::Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT )
{
	CX2Game::Handler_EGS_STATE_CHANGE_RESULT_NOT( kEGS_STATE_CHANGE_RESULT_NOT );
	
	DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_RESULT, NULL, false" );
	g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_135 ) );
	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_RESULT, NULL, false );

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	m_bEscape = false;

	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	return true;
	
}

/*virtual*/ bool CX2DungeonGame::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT )
{
	CX2Game::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT );

	// note!! ��Ȱ�ߴµ� ���Ͱ� �� �װ� ������ �������� Ŭ����, �׽�Ʈ �ʿ�
	if( 0 == LiveActiveNPCNum() )
	{
		SetClearEffect( 1.f );
	}

#ifdef CHILDRENS_DAY_EVENT_DUNGEON
	// �̺�Ʈ �������� ŷ ���ҵ� ���������̰�, Ŭ���������� ���� �������� ��Ȱ�� �� ���� ������� ŷ ���ҵ� ���������� �������ֱ� ���� ����ó��
	if( NULL != m_pDungeon &&
		NULL != m_pDungeon->GetDungeonData() )
	{
		switch( m_pDungeon->GetDungeonData()->m_DungeonID )
		{
		case SEnum::DI_EVENT_KIDDAY_RUBEN:
		case SEnum::DI_EVENT_KIDDAY_ELDER:
		case SEnum::DI_EVENT_KIDDAY_BESMA:
		case SEnum::DI_EVENT_KIDDAY_ALTERA:

		case SEnum::DI_EVENT_TREE_DAY_ELDER:
		case SEnum::DI_EVENT_TREE_DAY_BESMA:
		case SEnum::DI_EVENT_TREE_DAY_ALTERA:
		case SEnum::DI_EVENT_TREE_DAY_PEITA:
		case SEnum::DI_EVENT_TREE_DAY_VELDER:
		case SEnum::DI_EVENT_TREE_DAY_HAMEL:
			{
				if( NULL != m_pDungeon->GetNowStage() && NULL != m_pDungeon->GetNowStage()->GetStageData() )
					
				{
					g_pInstanceData->SetDungeonLoungeWorldID( m_pDungeon->GetNowStage()->GetStageData()->m_WorldID );
				}
			} break;

		case SEnum::DI_ELDER_HENIR_SPACE:
// 		case SEnum::DI_BESMA_HENIR_SPACE:
// 		case SEnum::DI_ALTERA_HENIR_SPACE:
// 		case SEnum::DI_FEITA_HENIR_SPACE:
// 		// kimhc // ���� ��ϸ� // 2009-10-27
// 		case SEnum::DI_VELDER_HENIR_SPACE:
// 		case SEnum::DI_HAMEL_HENIR_SPACE:
			{
				if( NULL != m_pDungeon->GetNowStage() &&
					NULL != m_pDungeon->GetNowStage()->GetStageData() &&
					(CX2Dungeon::DUNGEON_MODE) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE )
				{
					g_pInstanceData->SetDungeonLoungeWorldID( m_pDungeon->GetNowStage()->GetStageData()->m_WorldID );
				}
			} break;
		}
	}
#endif CHILDRENS_DAY_EVENT_DUNGEON 



	return true;
}



bool CX2DungeonGame::Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT )
{
	CX2Game::Handler_EGS_REMAINING_PLAY_TIME_NOT( kEGS_REMAINING_PLAY_TIME_NOT );

	float   fRemainingPlayTime = kEGS_REMAINING_PLAY_TIME_NOT;
		
	SetRemainPlayTime( fRemainingPlayTime );

	if( m_pDungeon != NULL
		&& m_pDungeon->GetNowStage() != NULL 
		&& m_pDungeon->GetNowStage()->GetNowSubStage() != NULL 
		&& m_pDungeon->GetNowStage()->GetNowSubStage()->GetSubStageTime() > 10.0f )
	{
#ifdef  ELOG_STATISTICS_NEW_DATA
		m_kGameStatistics.GatherPeriodicStatistics();
#else   ELOG_STATISTICS_NEW_DATA
		if( m_AverageRenderFrameRate <= 0 )
		{
			m_AverageRenderFrameRate = (int)DXUTGetFPS();
		}
		else
		{
			m_AverageRenderFrameRate += (int)DXUTGetFPS();
			m_AverageRenderFrameRate /= 2;
		}
#endif  ELOG_STATISTICS_NEW_DATA

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		CX2Game::StartTcpPingCheck();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	}

	const float MAGIC_TIME = 60.f;	// 60�� ������ �� ���� ȭ���� ��ο����� ������
	if( fRemainingPlayTime <= MAGIC_TIME )
	{
		const D3DXCOLOR MAGIC_WORLD_COLOR( 0.f, 0.f, 0.f, 0.f );
		const D3DXCOLOR MAGIC_UNIT_COLOR( 0.19f, 0.19f, 0.19f, 1.f );
		
		if( NULL != GetWorld() )
		{
			GetWorld()->FadeOriginWorldColor( MAGIC_WORLD_COLOR, fRemainingPlayTime );
			GetWorld()->FadeLightColor( MAGIC_UNIT_COLOR, fRemainingPlayTime );
		}
	}

	return true;
}

void CX2DungeonGame::TextOutTutorial_LUA( int nMsg, float fTime ) 
{
	m_bIsThereTutorialMsg = true;
	m_fTimeLeftForTutorialMessage = fTime;
	m_iTutorialMsgFlashCount = 0;

	
	m_wstrTutorialMsg = GET_STRING( nMsg );
	
}

/*virtual*/ void CX2DungeonGame::UpdateUnitPointer()
{
	CX2Game::UpdateUnitPointer();
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL )
		{
#ifdef MODIFY_DUNGEON_STAGING
			//���� ��ũ��Ʈ�� ���� ����
			if( true == pCX2GUNPC->GetStopAtStartState() )
			{
				if( pCX2GUNPC->GetStartState() == static_cast<int>(pCX2GUNPC->GetGameUnitState()) )
				{
					CanNotInputAndDisalbeNPCAI(true);	
				}
				else
				{
					CanNotInputAndDisalbeNPCAI(false);	
				}
			}

			//NPC ��ũ��Ʈ�� ���� ����
			if( true == pCX2GUNPC->GetStopAtCurrentState() )
			{//Ű�Է� ���� ������ NPC�� ������Ʈ ����� ����. (C ommonStateEnd() )
				CanNotInputAndDisalbeNPCAI(true);				
			}

			if( true == pCX2GUNPC->GetFocusCamera() && pCX2GUNPC->GetStartState() == (int)pCX2GUNPC->GetGameUnitState()
				|| true == pCX2GUNPC->GetFocusCameraAtCurrentState() )
			{
				if( true == pCX2GUNPC->GetApplyCinematicUI() && true == g_pMain->IsInheritStateMenu())
				{
					CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
					if( NULL != pStateMenu )
					{	//��Ŀ�� ī�޶� ���� �ÿ� ���� UI�� ���� ���⵵�� ����
						pStateMenu->SetShowUI(false);
					}
				}
				m_optrFocusUnit = pCX2GUNPC;
				ResetFocusUnit();
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

			//{{ kimhc // 2010.8.7 // ������ NPC���� ī�޶� ������ �ϴ� ���
			if( true == pCX2GUNPC->GetFocusCameraForce() )
			{
				m_optrFocusUnit = pCX2GUNPC;
				ResetFocusUnit();
				return;
			}
			//}} kimhc // 2010.8.7 // ������ NPC���� ī�޶� ������ �ϴ� ���
#else
			//{{ kimhc // 2010.8.7 // ������ NPC���� ī�޶� ������ �ϴ� ���
			if( pCX2GUNPC->GetFocusCameraForce() == true 
				|| ( pCX2GUNPC->GetFocusCamera() == true && pCX2GUNPC->GetStartState() == (int)pCX2GUNPC->GetGameUnitState() ) )
				//}} kimhc // 2010.8.7 // ������ NPC���� ī�޶� ������ �ϴ� ���
			{
				m_optrFocusUnit = pCX2GUNPC;
				ResetFocusUnit();
				return;
			}
#endif //MODIFY_DUNGEON_STAGING
			if( m_optrFocusUnit.GetObservable() == pCX2GUNPC && pCX2GUNPC->GetStartState() != (int)pCX2GUNPC->GetGameUnitState() )
			{
				if( GetMyUnit() != NULL )
					m_optrFocusUnit = GetMyUnit();
				else
					m_optrFocusUnit.Reset();
				ResetFocusUnit();
				return;
			}
		}
	}
}
#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
//{{ oasis907 : ����� [2010.5.6] // 
bool CX2DungeonGame::Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT& kEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT )
{
	m_fDungeonClearLimitTimeOut = kEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT.m_fRemainingTime;
	m_bShowDungeonClearLimitTimeOut = true;
	m_bDungeonClearLimitTimeOut = false;
	return true;
}

bool CX2DungeonGame::Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( KEGS_DUNGEON_EFFECT_TIME_OUT_NOT& kEGS_DUNGEON_EFFECT_TIME_OUT_NOT )
{
	if( NULL != g_pData->GetDungeonRoom() )
	{
		switch(g_pData->GetDungeonRoom()->GetDungeonID())
		{
		case SEnum::DI_ALTERA_SECRET_COMMON:
		case SEnum::DI_ALTERA_SECRET_HELL:
			{
				m_bDungeonClearLimitTimeOut = true;
				m_bShowDungeonClearLimitTimeOut = false;
				CreateDamageDataForDungeonTimeOut();
			} 
			break;
		default:
			{
				;
			}
			break;
		}
	}
	return true;
}
//}} oasis907 : ����� [2010.5.6] // 


//{{ oasis907 : ����� [2010.4.26] //
void CX2DungeonGame::CreateDamageDataForDungeonTimeOut()
{
	SAFE_DELETE( m_pDamageDataForDungeonTimeOut ); 

	// ���׶� ��д������� ���̵��� �ϵ� �ڵ�
	m_pDamageDataForDungeonTimeOut = new CX2DamageManager::DamageData;
	m_pDamageDataForDungeonTimeOut->SimpleInit();
	m_pDamageDataForDungeonTimeOut->reActType		= CX2DamageManager::RT_DUMMY_DAMAGE;
	m_pDamageDataForDungeonTimeOut->reActResult		= CX2DamageManager::RT_DUMMY_DAMAGE;
	m_pDamageDataForDungeonTimeOut->fHitAddMP		= 0.0f;

	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.Init();
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fRate = 1.0f;
#ifdef EXTRADAMAGE_RATE_BUG_FIX
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fPureRate = 1.0f;
#endif EXTRADAMAGE_RATE_BUG_FIX
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_POISON;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fTime = 1.f;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_DamagePerSec = 2000.f;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_Accumulation = 0;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fElapsedTimeToEffectDamage = 0.f;

#ifdef NEW_EXTRA_DAMAGE
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_iLevel = -1;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_iAddLevel = 0;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_bIgnoreResist = true;
#endif
#ifdef ADDITIONAL_MEMO
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_bDefenderLevel = false;
#endif

#ifdef SKILL_CASH_10_TEST
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fDamageRate		= 0.f;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fRunJumpRate	= 0.f;
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fAnimSpeedRate	= 0.f;
#endif

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_pDamageDataForDungeonTimeOut->m_ExtraDamage.m_fJumpRate	= 0.f;
#endif
	return;
}



 
void CX2DungeonGame::DungeonTimeOutFrameMove( double fTime, float fElapsedTime )
{
	// oasis907 : ����� [2010.5.6] // Ÿ�̸� ����
	if( m_bShowDungeonClearLimitTimeOut == true )
	{
		m_fDungeonClearLimitTimeOut -= fElapsedTime;
		if( m_fDungeonClearLimitTimeOut <= 0.f )
		{
			m_fDungeonClearLimitTimeOut = 0.f;
		}
	}

	if( true == m_bDungeonClearLimitTimeOut && !GetKilledBoss())
	{
		for ( int i = 0; i < GetUserUnitListSize(); i++ )
		{
			CX2GUUser* pGUUser = GetUserUnit(i);
			if ( pGUUser != NULL && CX2GameUnit::GUSI_DIE != pGUUser->GetGameUnitState()  )
			{
				pGUUser->DamageReact_ExtraDamage(m_pDamageDataForDungeonTimeOut, false);
			}
		}

		const D3DXCOLOR MAGIC_WORLD_COLOR( 0.f, 0.f, 0.f, 0.f );
		const D3DXCOLOR MAGIC_UNIT_COLOR( 0.19f, 0.19f, 0.19f, 1.f );

		if( NULL != GetWorld() )
		{
			GetWorld()->FadeOriginWorldColor( MAGIC_WORLD_COLOR, 0.f );
			GetWorld()->FadeLightColor( MAGIC_UNIT_COLOR, 0.f );
		}
	}
	return;
}
//}} oasis907 : ����� [2010.4.26] // 
#endif DUNGEON_TIME_OUT_FRAMEMOVE

void CX2DungeonGame::CountTimeSubStageFrameMove( double fTime, float fElapsedTime )
{
	if( true == m_bCountTimeLeftAfterSubStageOpen )
	{
		CX2GUUser* pCX2GUUser = GetMyUnit();
		if( pCX2GUUser != NULL && CX2GameUnit::GUSI_DIE == pCX2GUUser->GetGameUnitState() )
		{
			return;
			//m_bCountTimeLeftAfterSubStageOpen = false;
		}

		m_fTimeLeftAfterSubStageOpen -= fElapsedTime;
		if( m_fTimeLeftAfterSubStageOpen <= 0.0f )
		{
			m_fTimeLeftAfterSubStageOpen = 0.0f;
			if( GetMyUnit() != NULL )
			{
				int crashPercent = RandomNumber( 0, 100 );
				if ( GetMyUnit()->GetNowHp() > 0.f && crashPercent < 2 )
				{
					CX2GageManager::GetInstance()->CrashMyStateGageUI( D3DXVECTOR2( -7, -7 ), D3DXVECTOR2( 7, 7 ), D3DXCOLOR( 1, 0, 0, 1) );
				}
				const float fHpIncrement = -GetMyUnit()->GetMaxHp() * 0.05f * fElapsedTime;
				GetMyUnit()->UpNowHp( fHpIncrement );

			}
		}
	}
}

void CX2DungeonGame::TutorialFrameMove( double fTime, float fElapsedTime )
{
	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		if( false == m_bShowTutorialUI 
			&& GetMyUnit() != NULL
			&& GetMyUnit()->GetWaitStateID() == GetMyUnit()->GetNowStateID() )
		{
			m_bShowTutorialUI = true;
		}



		if( true == m_bShowTutorialUI && true == m_bIsThereTutorialMsg )
		{ 
			if( m_fTimeLeftForTutorialMessage > 0.f )
			{
				m_fTimeLeftForTutorialMessage -= fElapsedTime;

				if( m_iTutorialMsgFlashCount < 1 )
				{
					m_coTutorialMsg += m_coTutorialMsgIncrement;
					if( m_coTutorialMsg.r > 1.f )
					{
						m_coTutorialMsgIncrement = -m_coTutorialMsgIncrement;							
						m_coTutorialMsg = D3DXCOLOR(1,1,1,1);
						m_iTutorialMsgFlashCount += 1;
					}
					else if( m_coTutorialMsg.r < 0.f )
					{
						m_coTutorialMsgIncrement = -m_coTutorialMsgIncrement;
						m_coTutorialMsg = D3DXCOLOR(0,0,0,1);
					}
				}
			}
			else
			{
				if( -1.f != m_fTimeLeftForTutorialMessage )
				{
					m_bIsThereTutorialMsg = false;
					m_wstrTutorialMsg = L"";
				}
			}
		}
	}
}



bool CX2DungeonGame::GetResurrectionOperationCond()
{
	if ( CX2Main::XS_TRAINING_GAME != g_pMain->GetNowStateID() && m_bGameEnd == false )
	{
// 09.04.27 �¿� : Ư���������� ��Ȱ ���ϰ�
#ifdef CHILDRENS_DAY_EVENT_DUNGEON
		if( NULL != g_pData->GetDungeonRoom() )
		{
			switch(g_pData->GetDungeonRoom()->GetDungeonID())
			{
			case SEnum::DI_EVENT_KIDDAY_RUBEN:
			case SEnum::DI_EVENT_KIDDAY_ELDER:
			case SEnum::DI_EVENT_KIDDAY_BESMA:
			case SEnum::DI_EVENT_KIDDAY_ALTERA:
				m_fResurrectionRemainTime = 0.0f;
				return false;
			case SEnum::DI_ELDER_HENIR_SPACE:
// 			case SEnum::DI_BESMA_HENIR_SPACE:
// 			case SEnum::DI_ALTERA_HENIR_SPACE:
// 			case SEnum::DI_FEITA_HENIR_SPACE:
// 				// kimhc // ���� ��ϸ� // 2009-10-27
// 			case SEnum::DI_VELDER_HENIR_SPACE:
// 			case SEnum::DI_HAMEL_HENIR_SPACE:
				{					
					if( (CX2Dungeon::DUNGEON_MODE) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE )
					{
						m_fResurrectionRemainTime = 0.0f;
						return false;
					}
				} break;
			default:
				break;
			}
		}
#endif
		return true;
	}
	else
		return false;
}

bool CX2DungeonGame::DetermineShowDungeonPlayTime()
{
	
//#ifdef BUBBLE_BOBBLE_TEST
//	if( CX2Room::RT_ARCADE == g_pX2Room->GetRoomType() )
//	{
//		return true;
//	}
	//#endif BUBBLE_BOBBLE_TEST
	//

	if ( g_pData->GetDungeonRoom() == NULL )
		return false;


	// ��ϸ� �ð� �����̸� ���� ���� �ð� ǥ��
	if( true == CX2Dungeon::IsHenirDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) )
	{
		return true;
	}


	// ���� �ð� ���� ����Ʈ ������ �ְ�, �� �ش� �����̸� ���� ���� �ð� ǥ��
	const map<int,CX2QuestManager::QuestInst*> mapUnitQuest = g_pData->GetQuestManager()->GetUnitQuestMap();
	map<int,CX2QuestManager::QuestInst*>::const_iterator cmit;
	for ( cmit = mapUnitQuest.begin(); cmit != mapUnitQuest.end(); cmit++ )
	{
		CX2QuestManager::QuestInst* pQuestInst = cmit->second;
		if ( pQuestInst == NULL )
			continue;
		
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( pQuestInst->m_iID );
		if( pQuestTemplet == NULL )
			continue;

		for ( int i = 0; i < (int)pQuestTemplet->m_vecSubQuest.size(); i++ )
		{
			const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
			if( NULL == pSubQuestTemplet )
				continue;

			const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( pSubQuestTemplet->m_iID );
			if( NULL == pSubQuestInst )
				continue;

			if ( pSubQuestTemplet->m_eClearType != CX2QuestManager::SQT_DUNGEON_TIME )
				continue;

			if( pSubQuestInst->IsComplete() == true )
				continue;

#ifdef REFORM_QUEST
	#ifdef SERV_RANDOM_DAY_QUEST
			if( false == pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() )
			{
				std::set<SEnum::DUNGEON_ID>::iterator sit = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.begin();
				for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setDungeonID.end(); ++sit)
				{
					if( ((*sit) - g_pData->GetDungeonManager()->GetDungeonData(*sit)->m_eDifficulty)  == g_pData->GetDungeonRoom()->GetDungeonID() )
					{
						if( CX2Dungeon::DT_SECRET == g_pData->GetDungeonManager()->GetDungeonType( (int) g_pData->GetDungeonRoom()->GetDungeonID() ) )
						{
							if( true == pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty )
							{
								if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode <= g_pData->GetDungeonRoom()->GetDungeonMode() && 
									pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CX2Dungeon::DM_INVALID )
								{
									return true;
								}
							}
							else
							{
								if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode == g_pData->GetDungeonRoom()->GetDungeonMode() && 
									pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CX2Dungeon::DM_INVALID )
								{
									return true;
								}
							}
						}
						else
						{
							if( true == pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty )
							{
								if( g_pData->GetDungeonManager()->GetDungeonData(*sit)->m_eDifficulty <= g_pData->GetDungeonRoom()->GetDifficulty() )
								{
									return true;
								}
							}
							else
							{
								if( g_pData->GetDungeonManager()->GetDungeonData(*sit)->m_eDifficulty == g_pData->GetDungeonRoom()->GetDifficulty() )
								{
									return true;
								}
							}

						}
					}
				}
			}
	#endif SERV_RANDOM_DAY_QUEST
#else
#ifdef SERV_RANDOM_DAY_QUEST
			if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID == g_pData->GetDungeonRoom()->GetDungeonID() &&
				CX2Dungeon::DT_SECRET == g_pData->GetDungeonManager()->GetDungeonType( (int) g_pData->GetDungeonRoom()->GetDungeonID() ) )
			{
				if( true == pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty )
				{
					if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode <= g_pData->GetDungeonRoom()->GetDungeonMode() && 
						pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CX2Dungeon::DM_INVALID )
					{
						return true;
					}
				}
				else
				{
					if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode == g_pData->GetDungeonRoom()->GetDungeonMode() && 
						pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CX2Dungeon::DM_INVALID )
					{
						return true;
					}
				}
			}
			else
#endif SERV_RANDOM_DAY_QUEST
			if ( pSubQuestTemplet->m_ClearCondition.m_eDungeonID == g_pData->GetDungeonRoom()->GetDungeonID() )
			{
				if( true == pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty )
				{
					if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty <= g_pData->GetDungeonRoom()->GetDifficulty() )
					{
						return true;
					}
				}
				else
				{
					if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty == g_pData->GetDungeonRoom()->GetDifficulty() )
					{
						return true;
					}
				}
			}
#endif //REFORM_QUEST
		}
	}

	return false;
}

CX2DungeonGame::DungeonResultUnitInfo::DungeonResultUnitInfo()
{
    m_UnitUID = 0;
	m_bIsDie = false;

    m_nTotalScore = 0;
    m_nComboScore = 0;
    m_nTechnicalScore = 0;
    m_nDamaged = 0;
    m_nNPCKillNum = 0;
    m_nED = 0;
    m_nEXP  = 0;

	//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ����ġ �� ED ȹ��
#ifdef	REAL_TIME_ELSWORD
	m_nOldED	= 0;
	m_nOldEXP	= 0;
#endif	REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ����ġ �� ED ȹ��
	
    m_nClearBonusEXP        = 0;
    m_nRankBonusEXP         = 0;
    m_nPartyBonusEXP        = 0;
    m_nMvpBonusEXP          = 0;
    m_nGameBangBonusEXP     = 0;
    m_nPremiumBonusEXP      = 0;
    m_nEventBonusExp        = 0;   
	m_nChannelBonusEXP		= 0;

    m_fRankBonus        = 0.0f;
    m_fPartyBonus       = 0.0f;    

    m_cComboRank        = CX2DungeonRoom::RT_NONE;
    m_cTechnicalRank    = CX2DungeonRoom::RT_NONE;
    m_cDamagedRank      = CX2DungeonRoom::RT_NONE;
    m_cTimeRank         = CX2DungeonRoom::RT_NONE;
    m_cTotalRank        = CX2DungeonRoom::RT_NONE;

#ifdef SERV_DUNGEON_MVP
	m_bIsMVPPlayer		= false;
#endif // SERV_DUNGEON_MVP

}

#ifdef SERV_DUNGEON_MVP
UidType CX2DungeonGame::DungeonResultInfo::GetMvpPlayerUID ()
{
	vector< CX2DungeonGame::DungeonResultUnitInfo* >::iterator it = m_DungeonResultUnitInfoList.begin();
	for ( ; it != m_DungeonResultUnitInfoList.end(); ++it ) 
	{
		if ( (*it)->m_bIsMVPPlayer == true )
		{
			return (*it)->m_UnitUID;
		}
	}
	return 0;
}
#endif // DDED_STATE_DUNGEON_RESULT_DRAW_MVP_PLAYER_UI

int CX2DungeonGame::DungeonResultUnitInfo::GetTotalExp()
{
    int totalBounsExp = 0;

    totalBounsExp = m_nEXP + m_nClearBonusEXP + m_nRankBonusEXP + m_nPartyBonusEXP + 
        m_nMvpBonusEXP + m_nPremiumBonusEXP + m_nGameBangBonusEXP + m_nEventBonusExp + m_nChannelBonusEXP;

    return totalBounsExp;
}

//{{ 2009. 7. 1  ������		�����̵� ���ó���� ����
CX2DungeonGame::DungeonResultInfo::DungeonResultInfo( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket )
{
	m_Sec	= kPacket.m_nSec;
	m_bWin	= kPacket.m_bIsWin;
		//{{ kimhc // 2011-04-18 // ����, ���潺 ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
	m_ucNumOfProtectedSmallCrystals = static_cast<unsigned char>( kPacket.m_iProtectedSmallCrystal );
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-18 // ����, ���潺 ����

	for ( int i = 0; i < (int)kPacket.m_vecDungeonUnitInfo.size(); i++ )
	{
		KDungeonUnitResultInfo kDungeonUnitInfo = kPacket.m_vecDungeonUnitInfo[i];

		CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo	= new CX2DungeonGame::DungeonResultUnitInfo();
		pDungeonResultUnitInfo->m_UnitUID			= kDungeonUnitInfo.m_UnitUID;
		pDungeonResultUnitInfo->m_bIsDie			= kDungeonUnitInfo.m_bIsDie;

		pDungeonResultUnitInfo->m_nTotalScore		= kDungeonUnitInfo.m_nTotalScore;
		pDungeonResultUnitInfo->m_nComboScore		= kDungeonUnitInfo.m_nComboScore;
		pDungeonResultUnitInfo->m_nTechnicalScore	= kDungeonUnitInfo.m_nTechnicalScore;
		pDungeonResultUnitInfo->m_nDamaged			= kDungeonUnitInfo.m_nDamaged;
		pDungeonResultUnitInfo->m_nNPCKillNum		= kDungeonUnitInfo.m_nKillNPCNum;
		pDungeonResultUnitInfo->m_nED				= kDungeonUnitInfo.m_nED;
		pDungeonResultUnitInfo->m_nEXP				= kDungeonUnitInfo.m_nEXP;

		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED �� ����ġ ȹ��
#ifdef	REAL_TIME_ELSWORD
		pDungeonResultUnitInfo->m_nOldED			= kDungeonUnitInfo.m_nOldED	;
		pDungeonResultUnitInfo->m_nOldEXP			= kDungeonUnitInfo.m_nOldEXP;
#endif	REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED �� ����ġ ȹ��

		pDungeonResultUnitInfo->m_nClearBonusEXP		= kDungeonUnitInfo.m_nClearBonusEXP;
		pDungeonResultUnitInfo->m_nRankBonusEXP			= kDungeonUnitInfo.m_nRankBonusEXP;
		pDungeonResultUnitInfo->m_nPartyBonusEXP		= kDungeonUnitInfo.m_nPartyBonusEXP;
		pDungeonResultUnitInfo->m_nMvpBonusEXP			= kDungeonUnitInfo.m_nMvpBonusEXP;
		pDungeonResultUnitInfo->m_nGameBangBonusEXP		= kDungeonUnitInfo.m_nGameBangBonusEXP;
		pDungeonResultUnitInfo->m_nPremiumBonusEXP		= kDungeonUnitInfo.m_nPremiumBonusEXP;
        pDungeonResultUnitInfo->m_nEventBonusExp		= kDungeonUnitInfo.m_nEventEXP;

		pDungeonResultUnitInfo->m_nChannelBonusEXP		= kDungeonUnitInfo.m_nChannelBonusEXP;
		pDungeonResultUnitInfo->m_cComboRank		= (CX2DungeonRoom::RANK_TYPE)kDungeonUnitInfo.m_cComboRank;
		pDungeonResultUnitInfo->m_cTechnicalRank	= (CX2DungeonRoom::RANK_TYPE)kDungeonUnitInfo.m_cTechnicalRank;
		pDungeonResultUnitInfo->m_cDamagedRank		= (CX2DungeonRoom::RANK_TYPE)kDungeonUnitInfo.m_cDamagedRank;
		pDungeonResultUnitInfo->m_cTimeRank			= (CX2DungeonRoom::RANK_TYPE)kDungeonUnitInfo.m_cTimeRank;
		pDungeonResultUnitInfo->m_cTotalRank		= (CX2DungeonRoom::RANK_TYPE)kDungeonUnitInfo.m_cTotalRank;
		m_DungeonResultUnitInfoList.push_back( pDungeonResultUnitInfo );
#ifdef SERV_DUNGEON_MVP
		pDungeonResultUnitInfo->m_bIsMVPPlayer		=	kDungeonUnitInfo.m_bIsMVP;
#endif // SERV_DUNGEON_MVP
	}
	
}

CX2DungeonGame::DungeonResultInfo::~DungeonResultInfo()
{
	for ( int i = 0; i < (int)m_DungeonResultUnitInfoList.size(); i++ )
	{
		CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = m_DungeonResultUnitInfoList[i];
		SAFE_DELETE( pDungeonResultUnitInfo );
	}
	m_DungeonResultUnitInfoList.clear();
}


void CX2DungeonGame::ShowSubStageOpenGuide()
{
}



void CX2DungeonGame::ClearSubStage( int clearType, int nextStageNum, int nextSubStageNum )
{
	GetDungeon()->GetNowStage()->GetNowSubStage()->ClearSubStage( clearType, nextStageNum, nextSubStageNum );
}

void CX2DungeonGame::CreateTrainingGameUI( CX2State* pNowState )
{
	SAFE_DELETE( m_pTrainingGameUI );
	m_pTrainingGameUI = new CX2TrainingGameUI( pNowState );
}

void CX2DungeonGame::SetStartRealTraining( bool val )
{
	if( CX2Main::XS_TRAINING_GAME == g_pMain->GetNowStateID() )
	{
		if( m_bStartRealTraining != val )
		{
			CX2StateTrainingGame* pState = (CX2StateTrainingGame*) g_pMain->GetNowState();
			if( NULL != pState )
			{
				pState->Handler_EGS_SET_TC_REMAINING_TIME_REQ( val );
			}
		}

		m_bStartRealTraining = val; 
	
	}
}

void CX2DungeonGame::ShowBossName( const WCHAR* wszFileName )
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
	pSeq = GetMinorParticle()->CreateSequence( NULL,  L"BOSS_NAME", D3DXVECTOR3(0,0,0) );
	if( pSeq != NULL )
	{
		pSeq->ChangeTexForce( wszFileName );
		m_hBossName = pSeq->GetHandle();
	}
}

void CX2DungeonGame::ShowBossName_LUA( char* pFileName )
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
	pSeq = GetMinorParticle()->CreateSequence( NULL,  L"BOSS_NAME", D3DXVECTOR3(0,0,0) );
	if( pSeq != NULL )
	{
		wstring texFileName;
		ConvertUtf8ToWCHAR( texFileName, pFileName );
		pSeq->ChangeTexForce( texFileName.c_str() );
	}
}

// �Ʒüҿ��� ĳ���� ��ġ�� ���� ��ġ�� ����ġ��
void CX2DungeonGame::ResetUnitPositionAtTrainingGame()
{
	if( g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
		return;

	CX2GUUser* pGUUser = GetMyUnit();
	if( NULL != pGUUser )
	{
		pGUUser->InitPosition( true, 0 );
		pGUUser->StateChange( pGUUser->GetWaitStateID(), true );
	}


	int iCount = 1;
	for( UINT i=0; i < m_NPCUnitList.size() ; i++ )
	{
		CX2GUNPC* pGUNPC = GetNPCUnit( i );
		if( NULL == pGUNPC || pGUNPC->GetNowHp() <= 0.f ||
			pGUNPC->GetGameUnitState() == CX2GameUnit::GUSI_DIE ||
			pGUNPC->GetNPCTemplet().m_nNPCUnitID == CX2UnitManager::NUI_INVISIBLE_DUMMY )
		{
			continue;
		}

		pGUNPC->InitPosition( false, iCount );
		pGUNPC->StateChange( pGUNPC->GetWaitStateID(), true );
		iCount += 1;
	}

}

#ifdef GET_DUNGEON_ID_IN_LUA
int	CX2DungeonGame::GetDungeonID()
{
	return (int)m_pDungeon->GetDungeonData()->m_DungeonID;
}
#endif GET_DUNGEON_ID_IN_LUA

int	CX2DungeonGame::GetDungeonType() 
{ 
	return (int)m_pDungeon->GetDungeonData()->m_eDungeonType; 
}


#ifdef EXTRACT_ALL_DUNGEON_NPC_INFO



	struct NPCInfoKey
	{
		CX2UnitManager::NPC_UNIT_ID		m_eNPCID;
		bool							m_bActive;
		bool							m_bNoDrop;
		int								m_iRate;


		NPCInfoKey( CX2UnitManager::NPC_UNIT_ID eNPCID, bool bActive, bool bNoDrop, int iRate )
		{
			m_eNPCID = eNPCID;
			m_bActive = bActive;
			m_bNoDrop = bNoDrop;
			m_iRate = iRate;
		}

		bool operator < (const NPCInfoKey& rhs ) const
		{
			if( (int)m_eNPCID < (int)rhs.m_eNPCID )
				return true;
			else if( (int)m_eNPCID > (int)rhs.m_eNPCID )
				return false;


			if( (int)m_bActive < (int)rhs.m_bActive )
				return true;
			else if( (int)m_bActive > (int)rhs.m_bActive )
				return false;


			if( (int)m_bNoDrop < (int)rhs.m_bNoDrop )
				return true;
			else if( (int)m_bNoDrop >= (int)rhs.m_bNoDrop )
				return false;

			if( m_iRate < rhs.m_iRate )
				return true;
			
			return false;
		}
	};



	struct NextStageInfo
	{
		int		m_iClearCondIndex;
		int		m_iNextStageIndex;
		int		m_iRate;

		NextStageInfo()
		{
			m_iClearCondIndex = -1;
			m_iNextStageIndex = -1;
			m_iRate = -1;
		}
		NextStageInfo( int iClearCondIndex, int iNextStageIndex, int iRate )
		{
			m_iClearCondIndex = iClearCondIndex;
			m_iNextStageIndex = iNextStageIndex;
			m_iRate = iRate;
		}
	};

	
	void CX2DungeonGame::ExtractAllDungeonNPCInfo_DevOnly( const char* szFileName )
	{
		if( NULL == szFileName )
			return; 

		std::ofstream ofs;
		ofs.open( szFileName );
		if( false == ofs.is_open() )
		{
			return;
		}

		map< std::wstring, SEnum::DUNGEON_ID > mapDungeonList;
		map< std::wstring, SEnum::DUNGEON_ID >::iterator it;
		g_pData->GetDungeonManager()->GetDungeonList( mapDungeonList );



		ofs << "NPC_ID,		ACTIVE,		NO_DROP,	RATE" << std::endl;
		ofs << std::endl << std::endl << std::endl;


		for( it = mapDungeonList.begin(); it != mapDungeonList.end() ; ++it )
		{
			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( it->second );
			CX2Dungeon* pDungeon = g_pData->GetDungeonManager()->CreateDungeon( it->second, true );
			vector<CX2DungeonStage::StageData*>& vecStageData = pDungeon->GetStageDataList();

			string strDungeonName = "";
			ConvertWCHARToChar( strDungeonName, pDungeonData->m_DungeonName.c_str() );

			ofs << std::endl << std::endl << std::endl;
			ofs << strDungeonName << ", " << (int) pDungeonData->m_DungeonID << std::endl;

			for( UINT i=0; i<vecStageData.size(); ++i )
			{
				CX2DungeonStage::StageData* pStageData = vecStageData[i];
				vector<CX2DungeonSubStage::SubStageData*>& vecSubStageData = pStageData->m_SubStageDataList;

				map<NPCInfoKey, int> mapStageNPCInfo;
				map<NPCInfoKey, int>::iterator it_NPCInfo;

				vector<NextStageInfo> vecNextStageInfo;

				for( UINT j=0; j<vecSubStageData.size(); j++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = vecSubStageData[j];
					vector<CX2DungeonSubStage::NPCData*> vecNPCData = pSubStageData->m_NPCDataList;

					if( vecSubStageData.size() - 1 == j )
					{
						for( UINT k=0; k<pSubStageData->m_vecClearCondData.size(); k++ )
						{
							for( UINT m=0; m<pSubStageData->m_vecClearCondData[k].m_vecNextStage.size(); m++ )
							{
								CX2DungeonSubStage::NextStageData& nextStageData = pSubStageData->m_vecClearCondData[k].m_vecNextStage[m];
								if( nextStageData.m_eClearType == CX2DungeonSubStage::CT_STAGE ||
									nextStageData.m_eClearType == CX2DungeonSubStage::CT_STAGE_FORCE ||
									nextStageData.m_eClearType == CX2DungeonSubStage::CT_STAGE_SECRET )
								{
									vecNextStageInfo.push_back( NextStageInfo( k, nextStageData.m_iStageIndex, nextStageData.m_iRate ) );
								}
							}
						}
					}


					for( UINT k=0; k<vecNPCData.size(); k++ )
					{
						CX2DungeonSubStage::NPCData* pNPCData = vecNPCData[k];
						NPCInfoKey npcKey( pNPCData->m_UnitID, pNPCData->m_bActive, pNPCData->m_bNoDrop, pNPCData->m_Rate );

						it_NPCInfo = mapStageNPCInfo.find( npcKey );
						if( it_NPCInfo != mapStageNPCInfo.end() )
						{
							it_NPCInfo->second += 1;
						}
						else
						{
							mapStageNPCInfo[ npcKey ] = 1;
						}
					}
				}

				ofs << "STAGE: " << i << std::endl;
				ofs << "\t" << "SUB_STAGE_COUNT: " << (int) vecSubStageData.size() << std::endl;

				ofs << "\tNEXT_STAGE_INDEX,	RATE" << std::endl;
				for( UINT j=0; j<vecNextStageInfo.size(); j++ )
				{
					ofs << "\t" << vecNextStageInfo[j].m_iNextStageIndex
						<< ",\t" << vecNextStageInfo[j].m_iRate
						/*<< ",\t" << vecNextStageInfo[j].m_iClearCondIndex */<< std::endl;
				}

				ofs << std::endl;
				
				for( it_NPCInfo = mapStageNPCInfo.begin(); it_NPCInfo != mapStageNPCInfo.end(); ++it_NPCInfo )
				{
					const NPCInfoKey& npcKey = it_NPCInfo->first;

					
					ofs << "\t" << (int) npcKey.m_eNPCID << "\t\t";

					if( true == npcKey.m_bActive )
					{
						ofs << "TRUE" << "\t\t";
					}
					else
					{
						ofs << "FALSE" << "\t\t";
					}


					if( true == npcKey.m_bNoDrop )
					{
						ofs << "TRUE" << "\t\t";
					}
					else
					{
						ofs << "FALSE" << "\t\t";
					}

					ofs << npcKey.m_iRate << "\t\t";

					ofs << it_NPCInfo->second << "\t\t";


					const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( npcKey.m_eNPCID );
					string strNPCName = "";
					ConvertWCHARToChar( strNPCName, pNPCTemplet->m_Name.c_str() );
					if( NULL != pNPCTemplet )
					{
						ofs << strNPCName << "\t\t";
					}

					ofs << std::endl;

				}

				ofs << std::endl;
			}
			ofs.flush();
			SAFE_DELETE( pDungeon );
		}

		ofs.close();
	}
#endif EXTRACT_ALL_DUNGEON_NPC_INFO







	//{{ ����� : [2011/3/4/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	float CX2DungeonGame::GetPlayTimeLimit()
	{
		if( m_pDungeonRoom != NULL )
		{
			return m_pDungeonRoom->GetPlayTimeLimit();
		}

		return 0.0f;
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} ����� : [2011/3/4/] //	���� �̼�

#ifdef COME_BACK_REWARD
	void CX2DungeonGame::CloseDungeonName( float fTime )
	{
		m_pDLGTitle->Move(m_pDLGTitle->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 0.75f, true);
	}
#endif

#ifdef X2TOOL
	void CX2DungeonGame::ChangeStage(int iNextStage, int iNextSubStage)
	{	
		DeleteAllNPCUnit();
		//m_mapKilledNPCKeyCode.clear();
		StageLoading( iNextStage );
		StageStart();
		SubStageOpen(iNextSubStage);

		if( g_pInstanceData != NULL )
			g_pInstanceData->m_bChangeStage = true;
	}
#endif

/** @function : IsNearPortalLineMap() const
	@brief : ��Ż ���θʿ� �� ������ ��Ҵ����� �˻�
	@param : void
	@return : bool (������� true)
*/
bool CX2DungeonGame::IsNearPortalLineMap()
{
	if( NULL != GetMyUnit() && NULL != GetWorld() && NULL != GetWorld()->GetLineMap() )
	{
		CX2GUUser* pMyGUUser = GetMyUnit();

		// ������ ���� ���¸� ��Ż ��ó���� �׾�� ó�� ����
		if ( pMyGUUser->GetGameUnitState() == CX2GameUnit::GUSI_DIE || !( pMyGUUser->GetNowHp()	> 0 ) )
			return false;

		const int iLastTouchedLineMapIndex = pMyGUUser->GetLastTouchLineIndex( true );
		const CKTDGLineMap::LineData* pLastTouchedLineData = GetLineMap()->GetLineData( iLastTouchedLineMapIndex );

		ASSERT( pLastTouchedLineData );
		if ( NULL != pLastTouchedLineData )
		{		
			// Portal LineMap �̸� true ����
			return ( CKTDGLineMap::LT_POTAL == pLastTouchedLineData->lineType );
		}
		else
		{
			StateLog( L"lastTouchedLineData is wrong!" );
			return false;
		}	
	}
	else
	{
		return false;
	}
}


void CX2DungeonGame::ResurrectOtherUser()
{
	if ( m_bGameEnd == false 
		&& GetMyUnit() != NULL &&
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
				if ( pUnit != NULL && g_pData->GetDungeonRoom()->GetMySlot()->m_Index != i )
				{
					if ( tempCount == sequenceToFind )
					{
						if ( pUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
						{
							//Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( pUnit->GetUnitUID() );
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


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

bool CX2DungeonGame::Handler_EGS_BAD_ATTITUDE_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);	// lParam �� KSerBuffer ��
	KEGS_BAD_ATTITUDE_USER_NOT kEvent;								
	DeSerialize( pBuff, &kEvent );									// KSerBuffer �� ����

	CX2GageManager* pGageManager = CX2GageManager::GetInstance();	// GageManager Instance �� ����
	if ( NULL != pGageManager )
	{
		pGageManager->ChangeButtonType ( kEvent.m_iUserUID );		// Button Type �� ����
	}			
	return true;
}

bool CX2DungeonGame::Send_EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT ( const int iStageIndex_, const int iSubStageIndex_ )
{
	if  (  NULL != m_pDungeon
		&& NULL != m_pDungeon->GetNowStage()
		&& NULL != m_pDungeon->GetNowStage()->GetNowSubStage() )
	{
		KEGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT kEvent;

		kEvent.m_iStageID	 = iStageIndex_;			// ���� ���������� Index �� ���ͼ� ��Ŷ �����Ϳ� �ִ´�.
		kEvent.m_iSubStageID = iSubStageIndex_;	// ���� ���� ���������� Index �� ���ͼ� ��Ŷ �����Ϳ� �ִ´�.
		

		if ( NULL != g_pData )
		{
			CX2DungeonRoom * pDungeonRoom = g_pData->GetDungeonRoom();	// ���� �� �����͸� �޾ƿ�
			if ( NULL != pDungeonRoom )
			{
				if ( false == pDungeonRoom->IsDefenceDungeon( m_pDungeon->GetDungeonData()->m_DungeonID ) )	// Defence ������ �ƴ� ���
				{
					m_pDungeon->SetBeforeStageIndexUsingPacket ( m_pDungeon->GetNowStageIndex() );						// ���� ���������� Index �� ���ͼ� �����Ѵ�.
					m_pDungeon->SetBeforeSubStageIndexUsingPacket( m_pDungeon->GetNowStage()->GetNowSubStageIndex() );	// ���� ���� ���������� Index �� ���ͼ� �����Ѵ�.
				}

				int iComboScore = 0;				// ���� ���� ���������� �޺� ���ھ�
				CX2GUUser * pMyUnit = GetMyUnit ();	// �޺� �Ŵ����� �ҷ����� ����
				if ( NULL != pMyUnit )
				{
					CX2ComboManager * pComboManager = pMyUnit->GetComboManager();		// �޺� �Ŵ����� ���´�.
					if ( NULL != pComboManager )
					{
						iComboScore = pComboManager->GetSubStageComboScore();			// �޺� �Ŵ����κ��� ���� ���� ���������� �޺� ������ ���´�.
						pComboManager->ResetSubStageComboScore ();
					}
				}

				const int iTechScore = GetSubStageTechPoint();	// ���� ���� ���������� ��ũ�� ������ ���´�.
				ResetSubStageTechPoint();
			

				kEvent.m_iComboScore = iComboScore;
				kEvent.m_iTechScore = iTechScore;		// ��Ŷ �����Ϳ� �ִ´�.
				g_pData->GetServerProtocol()->SendPacket( EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT, kEvent ); 
			}
		}
	}
	return true;
}
	
/*virtual */bool CX2DungeonGame::Send_EGS_DEFENCE_DUNGEON_WAVE_ID_NOT ( const int iDungeonWaveID_ )
{
#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-25
	if( 0 == iDungeonWaveID_ )
	{
#ifdef CHECK_INHERIT_STATE_MENU
		if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
		{
			CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
			if( pStateMenu != NULL )
			{
				CX2State::MouseClickUI::MOUSE_CLICK_STYLE arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP_NORMAL;
				D3DXVECTOR2 vPos = D3DXVECTOR2(107, 763);
				pStateMenu->CreateMouseClickUI( (CKTDXStage*) pStateMenu, vPos, 0.5f, 1000, arrowStyle );
			}
		}
	}
#endif //SERV_NEW_DEFENCE_DUNGEON

	if ( g_pX2Room->GetUserNum() > 1 )			// ��Ƽ ���� ��
	{			
		if( iDungeonWaveID_ != m_iDefenceDungeonWaveID )
		{			
			if ( iDungeonWaveID_ > 0 )
			{
				Send_EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT ( 0, iDungeonWaveID_ );
			}
			
			KEGS_DEFENCE_DUNGEON_WAVE_ID_NOT kPacket;
			kPacket.m_iWaveID = iDungeonWaveID_ + 1;			// 1 ������� ����
			m_iDefenceDungeonWaveID = iDungeonWaveID_;

			g_pData->GetServerProtocol()->SendPacket( EGS_DEFENCE_DUNGEON_WAVE_ID_NOT, kPacket );

			return true;
		}
	}
	return false;
}

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
void CX2DungeonGame::CreateAllyEventMonster()
{
	bool bExceptDungeon = false;

	if( NULL != m_pDungeon &&
		NULL != m_pDungeon->GetDungeonData() )
	{
		switch( m_pDungeon->GetDungeonData()->m_DungeonID )
		{
		case SEnum::DI_RUBEN_EL_TREE_NORMAL:
		case SEnum::DI_RUBEN_RUIN_OF_ELF_NORMAL:
		case SEnum::DI_RUBEN_RUIN_OF_ELF_HARD:
		case SEnum::DI_RUBEN_RUIN_OF_ELF_EXPERT:
		case SEnum::DI_RUBEN_SWAMP_NORMAL:
		case SEnum::DI_RUBEN_SWAMP_HARD:
		case SEnum::DI_RUBEN_SWAMP_EXPERT:
			{					
				bExceptDungeon = true;
			} break;
		}

		if( true == g_pData->GetDungeonManager()->IsHenirDungeon( m_pDungeon->GetDungeonData()->m_DungeonID ) ||
			CX2Dungeon::DT_SECRET == g_pData->GetDungeonManager()->GetDungeonType( (int) g_pData->GetDungeonRoom()->GetDungeonID() ) ||
			true == g_pData->GetDungeonManager()->IsDefenceDungeon( m_pDungeon->GetDungeonData()->m_DungeonID ) )
			bExceptDungeon = true;
	}

	if( bExceptDungeon == false && 
		m_pDungeon != NULL &&
		m_pDungeon->GetNowStage() != NULL &&
		m_pDungeon->GetNowStage()->GetNowSubStage()->GetIsBossStage() )
	{
		unsigned int iUnitList = 0;
		bool bIngEvent = false;
		BOOST_TEST_FOREACH( CX2GUUser*, pUser, m_UserUnitList )
		{
			if( NULL == pUser )
				continue;

			if( CX2GameUnit::GUSI_DIE == pUser->GetGameUnitState() )
				continue;

			if( pUser->GetNowHp() <= 0.f )
				continue;

			if( pUser->GetUnit()->GetUnitData().GetGateOfDarknessSupportEventTime() > 0 )
			{
				bIngEvent = true;
			}

			iUnitList++;
		}

		if( bIngEvent == false )
		{
			return ;
		}
		
		for( int i = 0; i < 4 - iUnitList; i++ )
		{
			unsigned int randomIndex = rand() % iUnitList;
			CX2GUUser* pCX2GUUser = m_UserUnitList[randomIndex];

			if( NULL == pCX2GUUser || CX2GameUnit::GUSI_DIE == pCX2GUUser->GetGameUnitState() || pCX2GUUser->GetNowHp() <= 0.f )
			{
				BOOST_TEST_FOREACH( CX2GUUser*, pUser, m_UserUnitList )
				{
					if( NULL == pUser )
						continue;

					if( CX2GameUnit::GUSI_DIE == pUser->GetGameUnitState() )
						continue;

					if( pUser->GetNowHp() <= 0.f )
						continue;

					pCX2GUUser = pUser;
					break;
				}
			}

			if( NULL != pCX2GUUser )
			{
				int iNpcID = 0;

				if( i == 0 )
				{
					iNpcID = CX2UnitManager::NUI_DOPPELGANGER_ELSWORD;
				}
				else if( i == 1 )
				{
					iNpcID = CX2UnitManager::NUI_DOPPELGANGER_ARME;
				}
				else
				{
					iNpcID = CX2UnitManager::NUI_DOPPELGANGER_LIRE;
				}

				g_pX2Game->CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( iNpcID ), pCX2GUUser->GetUnitLevel(), true, 
					pCX2GUUser->GetPos(), pCX2GUUser->GetIsRight(), 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>( 0 ), 
					CX2NPCAI::NAT_ALLY, pCX2GUUser->GetUnitUID(),false, CX2Room::TN_NONE, CX2GUNPC::NCT_NONE );
			}
		}	
	}
}

void CX2DungeonGame::ReCreateAllyEventMonster( UidType EventOwnUnitUID )
{
	if( true == IsHost() )
	{
		BOOST_TEST_FOREACH( CX2GUUser*, pUser, m_UserUnitList )
		{
			if( NULL == pUser )
				continue;

			if( CX2GameUnit::GUSI_DIE == pUser->GetGameUnitState() )
				continue;

			if( pUser->GetNowHp() <= 0.f )
				continue;

			if( EventOwnUnitUID == pUser->GetUnitUID() )
				continue;

			if( NULL != pUser )
			{
				g_pX2Game->CreateNPCReq( static_cast<CX2UnitManager::NPC_UNIT_ID>( 1094 ), pUser->GetUnitLevel(), true, 
					pUser->GetPos(), pUser->GetIsRight(), 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>( 0 ), 
					CX2NPCAI::NAT_ALLY, pUser->GetUnitUID(),false, CX2Room::TN_NONE, CX2GUNPC::NCT_NONE );
				break;
			}
		}
	}
}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT