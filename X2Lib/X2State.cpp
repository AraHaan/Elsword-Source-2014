#include "StdAfx.h"
#include ".\x2state.h"

//extern bool g_bIsDXUTDeveloper;
//extern bool g_bIsDXUTSpecialUser;

#ifdef CLIENT_PURPLE_MODULE	// �ӱԼ� �Ϻ� �߰�
#include "OnlyGlobal/JP/Auth/PurpleForClient.h"
#endif // CLIENT_PURPLE_MODULE

#ifdef CLIENT_PORT_CHANGE_REQUEST
#define UDP_INDEX_MAX_COUNT 0	//1000	// 2013.05.09 lygan_������ // ��Ʈ ���� �ִ�ġ ���� ( ������ ��Ʈ�� ���� ������ 8000 ���� 9000 ���� ã�´�. ) // 0���� �����ϸ� 1�� ��Ʈ ���� ���Ŀ� �ٸ� ���� �õ� ���� �ʴ´�.
#define UDP_REQUEST_MAX_COUNT 0	//5		// 2013.05.10 lygan_������ // X2StateServerSelect.cpp ���� �Ȱ����� ������ ������ ���� ��������� �Ѵ�.		 // 0���� �����ϸ� 1�� ��Ʈ ���� ���Ŀ� �ٸ� ���� �õ� ���� �ʴ´�.
#endif //CLIENT_PORT_CHANGE_REQUEST

CX2State::CX2State(void) :
m_fTime( 0.f ),
m_fElapsedTime( 0.f ),
m_pCursor( NULL ),
m_bFailToConnectServer( false ),
m_pMsgOkFailToConnectServer( NULL ),
#ifdef PC_BANG_SANG
m_bFailPCBANGAuth( false ),
m_pMsgOkFailPCBANGAuth( NULL ),
#endif PC_BANG_SANG
m_LuaFrameMoveFuncName( "" ),
m_pMsgBoxQuitGame( NULL ),
m_bOpenMsgBoxQuitGame( false ),
m_bEnableQuitGame( true ),
m_pDLGMsgBox( NULL ),
m_bOneTimeProcAfterContructor( false ),
m_bIgnoreAllUIEvent( true ),
m_fIgnoreTime( 0.f ),
m_pDLGLastMsgPopUp( NULL ),
m_bEnableShortCutKey( true ),
m_pDLGEventClock( NULL ),
m_pDLGEventClockPopup( NULL ),
m_pDLGGuestUserGoUnitSelect( NULL ),
m_pDLGRecommendReward( NULL ),
m_pDLGTutorReward( NULL ),
m_pDLGGuestUserReward( NULL ),
m_bCheckPostItem( false ),
m_bPersonalTrade( false ),
m_pDLGUserInfo( NULL ),

#ifdef CHANGE_CHANNEL
m_bConnectedGameServer(false),
m_bPortCheckReq(false),
m_bPortCheckWait( false ),
m_fPortCheckWaitTime( 0.f ),
m_PortCheckRetryTime( 0 ),
#endif

m_iChangeChannelId(0),
m_bConnectFullChannel(false), 
m_pMouseClickUI(NULL)
//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
,m_pDLGUnReady( NULL )
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // ���� ��ų ���� ����
//{{ ����� : [2011/3/24/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
, m_fShowTimeForWorldMissionBuffName( 0.0f )
, m_pDLGWorldMissionBuffName( NULL )
#endif SERV_INSERT_GLOBAL_SERVER
//}} ����� : [2011/3/24/] //	���� �̼�
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
, m_bIdentityConfirmCheck( false )
#endif//SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
#ifdef DIALOG_SHOW_TOGGLE
, m_hParticleLogo( INVALID_PARTICLE_SEQUENCE_HANDLE )
#endif
, m_pDlgGameStartCount( NULL )
#ifdef BANDICAM_RECORDING
, m_hParticleRec( INVALID_PARTICLE_SEQUENCE_HANDLE )
#endif
, m_pDLGStateChangePopup(NULL)
#ifdef PC_BANG_BENEFIT_GUIDE
, m_pDLGPCBangBenefitGuide ( NULL )
#endif //PC_BANG_BENEFIT_GUIDE
#ifdef SERV_2012_PVP_SEASON2_EVENT
, m_fTimeOfGloryRemainTime( 0.f )	/// ������ �ð� ���̾�α� Ȱ�� �ð�
, m_pDLGTimeOfGlory( NULL )			/// ������ �ð� ���̾�α�
#endif SERV_2012_PVP_SEASON2_EVENT
, m_bWaitGameStart(false)
#ifdef SERV_ADVERTISEMENT_EVENT
, m_pDlgAdvertisement( NULL )
#endif SERV_ADVERTISEMENT_EVENT
#ifdef CLIENT_PORT_CHANGE_REQUEST
, m_iUDPRequestCount_X2State( 0 )
#endif //CLIENT_PORT_CHANGE_REQUEST
#ifdef ADDED_RELATIONSHIP_SYSTEM	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
, m_uiRoomUID( -1 )
, m_iCalledUserUID( -1 )
, m_iCalledGSUID( -1 )
, m_cCallType( 0 )
, m_iMapID( -1 )
, m_vPos( D3DXVECTOR3( 0.f, 0.f, 0.f ) )
, m_LastTouchLineIndex( -1 )
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
, m_pDlgJumpingCharacterClassChange ( NULL )			// ���� ���� â
, m_pDlgJumpingCharacterClassChangeNotice(  NULL )		// ���� ���� �˸�â
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
, m_bReConnectChannelServer  ( false )
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef SERV_CHECK_TIME_QUEST
, m_bAlreadyShowErrorMessage( false )
#endif //SERV_CHECK_TIME_QUEST

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
, m_pDlgEventCharacterRanking( NULL )
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
, m_bShowNoteUI( false )
, m_bPlayNoteUI( false )
, m_bProcessNoteView( false )
, m_pDlgEventElesisNote( NULL )
, m_hMeshInstEventNote( INVALID_MESH_INSTANCE_HANDLE )
, m_hMeshInstEventNoteStart( INVALID_MESH_INSTANCE_HANDLE )
, m_fPlayTime( 0.f )
, m_iRewardTitleIndex( 0 )
#endif SERV_ELESIS_UPDATE_EVENT
{

	SetStageName( L"NowState" ); //��͵� DXUT SC_CLOSE�ʰ� ��������

	RegisterLuaBind();

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pStage",		this );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pX2Room",		g_pX2Room );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pX2Game",		g_pX2Game );


	// ������������� �������������� ��� ��ƼŬ�� �ε��ϱ� ���ؼ�, �� ���彺ũ��Ʈ���� UI****Particle�� ����ϱ� ���ؼ�
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIMajorParticle",		g_pData->GetUIMajorParticle() );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIMinorParticle",		g_pData->GetUIMinorParticle() );



	m_pCursor					= new CX2Cursor();

	if ( NULL != g_pMain->GetGameEdit() )
		g_pMain->GetGameEdit()->SetNowStage( this );

	g_pMain->GetLVUpEventMgr()->SetNowStage( this );

	g_pMain->AllDeleteServerPacket();


	//if( g_pChatBox == NULL && g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP)

	//{{ ����� : [2009/10/27] //	���ǹ� ����. ä��â�� �� ������ �ƴҶ� �����ص� ��������Ƿ�.
#ifdef ELSWORD_NEW_BEGINNING
	if( g_pChatBox == NULL && g_pMain->GetNowStateID() == CX2Main::XS_BEGINNING)
#else
	if( g_pChatBox == NULL && g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT)
#endif ELSWORD_NEW_BEGINNING
	{
		g_pChatBox = new CX2ChatBox( this );
	}
	//}} ����� : [2009/10/27] //	���ǹ� ����. ä��â�� �� ������ �ƴҶ� �����ص� ��������Ƿ�.

	if( NULL != g_pChatBox )
	{
		g_pChatBox->SetStage( (CKTDXStage*) this );
		
		switch( g_pMain->GetNowStateID() )
		{
#ifdef ELSWORD_NEW_BEGINNING
		case CX2Main::XS_BEGINNING:
#else
		case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
//		case CX2Main::XS_UNIT_SELECT:
		case CX2Main::XS_CREATE_UNIT:
		case CX2Main::XS_DUNGEON_RESULT:
		case CX2Main::XS_PVP_RESULT:
		case CX2Main::XS_TRAINING_RESULT:
			{	
				g_pChatBox->CloseChatWindowButton();
				g_pChatBox->CloseChatWindow();

			} break;
			break;

		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_SQUARE_GAME:
		case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
		case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
			{
				//g_pChatBox->OpenChatWindowButton();
				g_pChatBox->SetChatBoxLayerUp(false);
				g_pChatBox->CloseChatWindowButton();
				g_pChatBox->OpenChatWindow();
				g_pChatBox->HideChatWindowFrame();
				g_pChatBox->HideChatEditBox();
			} break;			
		case CX2Main::XS_PVP_LOBBY:
		case CX2Main::XS_PVP_ROOM:
		case CX2Main::XS_TRAINING_SCHOOL:
			{
				g_pChatBox->SetChatBoxLayerUp(true);
				g_pChatBox->HideChatWindowFrame();
				g_pChatBox->HideChatWindow();
				g_pChatBox->HideChatEditBox();			
				g_pChatBox->CloseChatWindow();
				g_pChatBox->OpenChatWindowButton();
			}
			break;
		default:
			{
				g_pChatBox->SetChatBoxLayerUp(true);
				g_pChatBox->OpenChatWindowButton();
				//g_pChatBox->CloseChatWindowButton();
			} break;
		}
	}

	if( g_pData->GetUserMenu() != NULL )
	{
		g_pData->GetUserMenu()->ClosePopupMenu();
	}

#ifdef EVENT_NEW_HENIR
	if( true == CX2EmblemManager::GetInstance()->IsBurningEventTime() )
	{
		CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_WEEKEND_BURNING_EVENT);
	}
	else
	{
		CX2EmblemManager::GetInstance()->EndEmblem(CX2EmblemManager::EI_WEEKEND_BURNING_EVENT);
	}
#endif // EVENT_NEW_HENIR

// {{  ����� - ĳ���� ��Ŭ�� �˾� �޴� UI ���� 
#ifdef CHARACTER_MOUSE_RBUTTON
	if ( g_pData->GetUserListPopUp() != NULL )
		g_pData->GetUserListPopUp()->ClosePopupMenu();
#endif CHARACTER_MOUSE_RBUTTON
// }}  ����� - ĳ���� ��Ŭ�� �˾� �޴� UI ���� 


#ifdef EVENT_SCENE
	if( NULL != g_pData->GetEventScene() )
	{
		g_pData->GetEventScene()->SetNowState( this );
	}
#endif EVENT_SCENE

	if( NULL != g_pData->GetSlideShot() )
	{
		g_pData->GetSlideShot()->SetNowState( this );
	}

	if( NULL != g_pMain->GetTutorSystem() )
	{
		g_pMain->GetTutorSystem()->SetStage( (CKTDXStage*) this );
	}

	if ( g_pData->GetMessenger() != NULL )
	{
		g_pData->GetMessenger()->SetStage( (CKTDXStage*) this );
		g_pData->GetMessenger()->SetHideWindow(false);
		g_pData->GetMessenger()->SetOpen(false);
	}

	if( g_pMain != NULL && g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetDialogShow() == true)
	{
		// ������Ʈ ����� �����԰��� �����츦 �ݴ´�.
		g_pMain->GetPostBox()->SetDialogShow(false);
	}
	
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_START_UP:
	case CX2Main::XS_LOGIN:
	case CX2Main::XS_LOADING:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
//	case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_CREATE_UNIT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_TRAINING_RESULT:
		{
		} break;

	default:
		{
			if( NULL != g_pData &&
				NULL != g_pData->GetMyUser() &&
				true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				if( true == g_pData->GetMyUser()->GetSelectUnit()->GetGuestUserReachedLevelLimit() &&
					false == g_pData->GetMyUser()->GetSelectUnit()->GetGuestUserMovingToCharacterSelect() )
				{
					m_pDLGGuestUserGoUnitSelect = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_538 ), this, SUCM_GUEST_USER_GOTO_UNIT_SELECT );
				}
			}
		} break;
	}

#ifdef DIALOG_SHOW_TOGGLE

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_WEDDING_GAME:	// �ؿ��� �߰�
		g_pKTDXApp->GetDGManager()->GetDialogManager()->SetCanHide(true);
		break;
	default:
		g_pKTDXApp->GetDGManager()->GetDialogManager()->SetCanHide(false);
		break;
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->SetShow( true );

#endif
	
	if( g_pMain != NULL && g_pMain->GetInfoBox() != NULL )
		g_pMain->DeleteInfoBox();

#ifdef DEPARTURE_PREVENTION_SYSTEM
	if ( NULL != g_pData )
		g_pData->ResetDeparturePrevention();
#endif

	if ( NULL != g_pData && NULL != g_pData->GetPlayGuide() && NULL != g_pData->GetPlayGuide()->GetDlgPlayGuide() )
	{	//��ҿ� �°� ���̵� �ʱ�ȭ ���� �ֱ� ���� ����
		g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_NONE, true);
		g_pData->GetPlayGuide()->SetShowPlayGuide(false);
	}
#ifdef REFORM_QUEST
	//������Ʈ �Ϸ� ��ƼŬ�� UI�� ������ �����ϵ��� ����
	if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ClearQuestCompleteParticle();
	}
#endif //REFORM_QUEST
	if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, false);
	}
}

CX2State::~CX2State(void)
{
	DeleteMouseClickUI();

#ifdef SERV_ADVERTISEMENT_EVENT
	SAFE_DELETE_DIALOG( m_pDlgAdvertisement );
#endif SERV_ADVERTISEMENT_EVENT

	SAFE_DELETE_DIALOG( m_pDlgGameStartCount );

	//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
	SAFE_DELETE_DIALOG( m_pDLGUnReady );
#endif MODIFY_INFINITE_SKILL_BUG
	//}} kimhc // 2010.3.26 // ���� ��ų ���� ����

	SAFE_DELETE_DIALOG( m_pDLGEventClock ); 
	SAFE_DELETE_DIALOG( m_pDLGEventClockPopup ); 

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	SAFE_DELETE(m_pCursor);
	SAFE_DELETE_DIALOG(m_pMsgOkFailToConnectServer);
	SAFE_DELETE_DIALOG( m_pDLGLastMsgPopUp );
	SAFE_DELETE_DIALOG( m_pDLGUserInfo );
	SAFE_DELETE_DIALOG( m_pDLGStateChangePopup );

	//{{ kimhc // 2011-04-25 // ���潺 ���� UI
#ifdef	SERV_INSERT_GLOBAL_SERVER
	SAFE_DELETE_DIALOG( m_pDLGWorldMissionBuffName );
#endif	SERV_INSERT_GLOBAL_SERVER
	//}} kimhc // 2011-04-25 // ���潺 ���� UI

#ifdef PC_BANG_BENEFIT_GUIDE
	SAFE_DELETE_DIALOG( m_pDLGPCBangBenefitGuide );
#endif //PC_BANG_BENEFIT_GUIDE

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChange );			// ���� ���� â
	SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChangeNotice );		// ���� ���� �˸�â
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
	SAFE_DELETE_DIALOG ( m_pDlgEventCharacterRanking );
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

	if( NULL != g_pChatBox )
	{
		g_pChatBox->HideChatLog();

		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_PVP_GAME:
			{
				if( true == g_pChatBox->GetHideChatWindowFrame() )
				{
					g_pChatBox->UnHideChatWindowFrame();
					g_pChatBox->CloseChatWindow();
				}
			} break;
		default:
			{
				if( true == g_pChatBox->GetOpenChatWindow() )
				{
					g_pChatBox->UnHideChatWindowFrame();
				}
			} break;
		}
	}

	if(g_pData != NULL && g_pData->GetMessenger() != NULL)
	{
		g_pData->GetMessenger()->SetOpen(false);
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ClearAllOKAndCancelMsgDlg();

	g_pMain->DeleteStateChangeDLG();

#ifdef DIALOG_SHOW_TOGGLE
	if( m_hParticleLogo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleLogo );
#endif
#ifdef BANDICAM_RECORDING
	if( m_hParticleRec != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleRec );
#endif

#ifdef SERV_2012_PVP_SEASON2_EVENT
	SAFE_DELETE_DIALOG( m_pDLGTimeOfGlory );
#endif SERV_2012_PVP_SEASON2_EVENT

	if(NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI())
		g_pInstanceData->GetMiniMapUI()->CloseWorldMapUI();

#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

	BOOST_FOREACH( FadeInOutNotice* pFadeInOutNotice, m_vecFadeInOutNotice )
	{
		if( NULL != pFadeInOutNotice )
			SAFE_DELETE( pFadeInOutNotice );
	}
	m_vecFadeInOutNotice.clear();
	
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
}


HRESULT CX2State::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CX2GageManager::OnFrameMoveInSpecificX2State( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CX2GageManager::OnFrameMoveInSpecificX2State();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef CHANGE_CHANNEL
	OnFrameMove_GameServerConnect( fTime, fElapsedTime );
#endif CHANGE_CHANNEL

#ifdef CLIENT_PORT_CHANGE_REQUEST
	OnFrameMove_UdpChangeRequest( fTime, fElapsedTime );
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifdef SERV_ID_NETMARBLE_PCBANG
	OnFrameMove_Public_IP_Check( fTime, fElapsedTime );
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef ADD_COLLECT_CLIENT_INFO	// �ؿ��� ũ���� ������ ������ �߰�
#ifdef SERV_HACKING_TOOL_LIST
	if( g_pInstanceData != NULL && g_pMain != NULL && g_pKTDXApp != NULL && 
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        g_pInstanceData->GetHackListSize_MainThread() <= 0 &&
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->GetHackListSize() <= 0 &&
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT )
	{
#ifdef ADD_COLLECT_CLIENT_INFO			
		g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_17, "", true, true );
#else
		g_pMain->SendHackMail_DamageHistory("-- EmptyList --");
		g_pKTDXApp->SetFindHacking( true );
#endif
	}
#endif
#endif // ADD_COLLECT_CLIENT_INFO

	if( m_bOneTimeProcAfterContructor == false )
	{
		m_bOneTimeProcAfterContructor = true;
		g_pMain->GetStateAutoChanger().StateChangeAfterConstructor();

		if( false == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
		{
			g_pMain->PopupReservedMessage();
		}
	}

	m_fTime			= (float)fTime;
	m_fElapsedTime	= fElapsedTime;

	if ( m_bIgnoreAllUIEvent == true )
	{
		m_fIgnoreTime += fElapsedTime;
		if ( m_fIgnoreTime >= 0.5f )
		{
			m_bIgnoreAllUIEvent = false;
			m_fIgnoreTime = 0.0f;
		}
	}	

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

#ifdef LOADING_ANIMATION_TEST
	g_pMain->GetLoadingAnimation()->OnFrameMove( fTime, fElapsedTime );
#endif LOADING_ANIMATION_TEST


#ifdef SERV_EPIC_QUEST
	if(g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIQuestNew() != NULL &&
		g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_NEW))
	{
		g_pData->GetUIManager()->GetUIQuestNew()->OnFrameMove(fTime, fElapsedTime);
	}
#endif SERV_EPIC_QUEST



	if( NULL != g_pMain->GetTutorSystem() )
	{
		g_pMain->GetTutorSystem()->OnFrameMove( fTime, fElapsedTime );
	}

	if( NULL != g_pChatBox )
	{
		g_pChatBox->OnFrameMove( fTime, fElapsedTime );
	}


	if( NULL != g_pInstanceData )
	{
		g_pInstanceData->OnFrameMove( fTime, fElapsedTime );
	}

#ifdef NEW_VILLAGE_UI
	if(g_pData->GetUIManager() != NULL )
		g_pData->GetUIManager()->OnFrameMove( fTime, fElapsedTime );
#endif
	

	g_pKTDXApp->GetDGManager()->OnFrameMove( fTime, fElapsedTime );
	m_pCursor->OnFrameMove( fTime, fElapsedTime );
	
#ifdef FIX_CAMERA_CRASH_BY_EFFECT_SET
	// ������ // 
	// X2Game��, State �� ������ �����ϰ� �־, ī�޶� ���� ���� �ϰ� ���� �ʾ���.
	// ������ ���� ����ó�� �߰�
	if( NULL == g_pX2Game )
#endif //FIX_CAMERA_CRASH_BY_EFFECT_SET
	{
		g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );
	}

#ifdef PC_BANG_SANG
	//{{ oasis907 : ����� [2010.2.3] // �ǽù� ���ȭ
	if ( m_bFailPCBANGAuth == true )
	{
	
		// Ŭ���̾�Ʈ�� �ǵ������� ä�μ������� ������ ���� �ʴ� �̻� ä�μ������� ������ �׻� �����ȴٰ� �����Ѵ�. ä�μ������� ���� ������ ���� ���� ���ٰ� �����Ѵ�, userproxy��ü�� �����常 �����Ŵ
		g_pData->GetServerProtocol()->DisconnectFromGameServer();


		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pMsgOkFailPCBANGAuth ) == false )
		{
			m_pMsgOkFailPCBANGAuth = NULL;

			SAFE_DELETE_DIALOG( m_pDLGMsgBox );
			m_bFailPCBANGAuth = false;

			g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECTION_LOST );
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );			


		}
	}
	//}}
#endif PC_BANG_SANG

	if ( m_bFailToConnectServer == true )
	{
		// Ŭ���̾�Ʈ�� �ǵ������� ä�μ������� ������ ���� �ʴ� �̻� ä�μ������� ������ �׻� �����ȴٰ� �����Ѵ�. ä�μ������� ���� ������ ���� ���� ���ٰ� �����Ѵ�, userproxy��ü�� �����常 �����Ŵ
		g_pData->GetServerProtocol()->DisconnectFromGameServer();


		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pMsgOkFailToConnectServer ) == false )
		{
			m_pMsgOkFailToConnectServer = NULL;

			SAFE_DELETE_DIALOG( m_pDLGMsgBox );
			m_bFailToConnectServer = false;

			switch( g_pMain->GetNowStateID() )
			{
#ifdef LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
			case CX2Main::XS_LOGIN:
				{
					g_pData->GetServerProtocol()->DisconnectFromChannelServer();
				} break;
#endif LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
#ifdef ELSWORD_NEW_BEGINNING
			case CX2Main::XS_BEGINNING:
#else
			case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
				{
#ifdef ELSWORD_NEW_BEGINNING
					if( true == CX2StateBeginning::m_sbSelectedServerSet ) // ĳ���� ����ȭ���̶��, ��, ���Ӽ����� ����Ǿ��ִ� ����
#else
					if( true == CX2StateServerSelect::m_sbSelectedServerSet ) // ĳ���� ����ȭ���̶��, ��, ���Ӽ����� ����Ǿ��ִ� ����
#endif ELSWORD_NEW_BEGINNING
					{
						g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECTION_LOST );
						g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );			
					}
					else // ä�� ����ȭ���̶��,
					{
						// nothing;
					}
				} break;

			default: 
				{
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECTION_LOST );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );			
				} break;
			}
		}
	}

	if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		CX2GameEdit* pGameEdit = g_pMain->GetGameEdit() ;
		if( NULL != pGameEdit )
		{
			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_END ) == TRUE )
			{
				pGameEdit->SetShow( !pGameEdit->GetShow() );
				pGameEdit->SetEnable( !pGameEdit->GetEnable() );
			}
#ifdef MODIFY_GAME_EDIT_CAMERA
			else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NEXT ) == TRUE )
			{
				pGameEdit->ToggleGameEditCamera();
			}
#endif //MODIFY_GAME_EDIT_CAMERA

#ifdef CHEAT_CLASS_CHANGE //JHKang
			else if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP && 
					  g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_SEMICOLON ) == TRUE )
			{
				pGameEdit->ToggleClassChangeCheat();
			}
#endif //CHEAT_CLASS_CHANGE
			pGameEdit->OnFrameMove( fTime, fElapsedTime );
		}
#ifdef SHOW_ONLY_MY_DAMAGE //�ӽ�. ���� ���� ���� �ÿ��� �ɼ����� �� ����
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_PRIOR ) == TRUE )
		{
			bool bShowOnlyMyDamage = !g_pMain->GetGameOption().GetShowOnlyMyDamage();

			if( true == bShowOnlyMyDamage )
			{
				g_pChatBox->AddChatLog( L" ���� Ÿ�� �� �������� ����! ", 
					KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
			else
			{
				g_pChatBox->AddChatLog( L" ��Ƽ���� Ÿ�� �� �������� ����! ", 
					KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
			g_pMain->GetGameOption().SetShowOnlyMyDamage( bShowOnlyMyDamage );
		}
#endif //SHOW_ONLY_MY_DAMAGE
	}

	g_pMain->GetTextManager()->OnFrameMove( fTime, fElapsedTime );

	LuaFrameMove( fTime, fElapsedTime );

	HandleMsgByESC();
	
	ShortCutKeyProcess();


//{{ kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_SECRET_HELL
	CheckNRefreshDailyQuest();
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)

	if ( g_pMain->GetBrowserWrapper()->GetCheckClosed() == false )
	{
		g_pMain->GetBrowserWrapper()->SetCheckClosed( true );

		//{{ kimhc // 2009-10-14 // �ڵ����� �����ؼ� �ֱ� ������ ���� ���������� �������� ����
#ifdef	AUTO_PAYMENT
		if ( g_pInstanceData != NULL )
		{
			switch ( g_pInstanceData->GetWebType() )
			{
			case CX2CashShop::CSWT_CASH_CHARGE:
				Handler_EGS_CHECK_BALANCE_REQ();
				break;

			case CX2CashShop::CSWT_CASH_AUTO_PAYMENT:
				Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ();
				break;

			default:
				break;
			}

			g_pInstanceData->SetWebType( CX2CashShop::CSWT_INVALID );
		}

#else	AUTO_PAYMENT
		Handler_EGS_CHECK_BALANCE_REQ();
#endif	AUTO_PAYMENT
		//}} kimhc // 2009-10-14 // �ڵ����� �����ؼ� �ֱ� ������ ���� ���������� �������� ����
	}
#ifdef CJ_ID_WEB_BILLING
	else if( g_pMain->GetNMBrowserWrapper()->GetCheckClosed() == false )
	{
		g_pMain->GetNMBrowserWrapper()->SetCheckClosed( true );
		Handler_EGS_CHECK_BALANCE_REQ();
	}
#endif //CJ_ID_WEB_BILLING

//#ifdef TITLE_SYSTEM
    if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
        g_pData->GetTitleManager()->CheckNewTitle();
//#endif





#ifdef RESET_INVALID_UNIT_POSITION_TEST
	//const float MAGIC_CHECK_MISPLACED_UNIT_GAP = 1.f;
	//if( m_TimerCheckMisplacedUnit.elapsed() > MAGIC_CHECK_MISPLACED_UNIT_GAP )
	//{
	//	m_TimerCheckMisplacedUnit.restart();

	//	// ��ġ�� �̻��� ������ ������ Ȯ��
	//	{
	//		// not yet
	//	}



	//	// ��ġ�� �̻��� ������ �ִٸ� UI ��ư�� Ȱ��ȭ��Ų��
	//	if( true == m_bThereIsMisplacedUnit &&
	//		m_TimerReplaceMisplacedUnit.elapsed() > m_MisplacedUnitReplaceGap )
	//	{
	//		// not yet
	//	}
	//	else
	//	{

	//	}

	//}
#endif RESET_INVALID_UNIT_POSITION_TEST

	//{{ ����� : [2011/3/23/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	if( NULL != g_pData->GetWorldMissionManager() )
	{
		g_pData->GetWorldMissionManager()->OnFrameMove( fTime, fElapsedTime );
	}

	if ( m_pDLGWorldMissionBuffName != NULL )
	{
		m_fShowTimeForWorldMissionBuffName += fElapsedTime;
		if(m_fShowTimeForWorldMissionBuffName >= 4.f && m_fShowTimeForWorldMissionBuffName <= 6.f)
		{
			MoveShowEvent(m_fShowTimeForWorldMissionBuffName);
		}
		else if(m_fShowTimeForWorldMissionBuffName > 6.f)
		{
			SAFE_DELETE_DIALOG( m_pDLGWorldMissionBuffName );
			m_fShowTimeForWorldMissionBuffName = 0.f;
		}
	}

#endif SERV_INSERT_GLOBAL_SERVER
	//}} ����� : [2011/3/23/] //	���� �̼�

#ifdef COME_BACK_REWARD
	if ( NULL != g_pData->GetPremiumBuffManager() )
	{
		g_pData->GetPremiumBuffManager()->OnFrameMove( fTime, fElapsedTime );
	}
#endif

#ifdef DEPARTURE_PREVENTION_SYSTEM
	if ( NULL != g_pData->GetDeparturePrevention() )
	{
		g_pData->GetDeparturePrevention()->OnFrameMove( fTime, fElapsedTime );
	}
#endif
	
#ifdef DIALOG_SHOW_TOGGLE
#ifdef ALWAYS_SCREEN_SHOT_TEST
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
#else // ALWAYS_SCREEN_SHOT_TEST
#ifndef _SERVICE_
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
#endif
#endif // ALWAYS_SCREEN_SHOT_TEST
	{
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		{
			if( m_hParticleLogo == INVALID_PARTICLE_SEQUENCE_HANDLE && g_pData->GetUIMajorParticle() != NULL )
			{
#ifdef CLIENT_COUNTRY_HK
				m_hParticleLogo = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"screenShotMode_title_season2_HK",  1024.f - 80.f, 768.f - 37.5f, 0.f );
#else
				m_hParticleLogo = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"screenShotMode_title_season2",  1024.f - 80.f, 768.f - 37.5f, 0.f );
#endif CLIENT_COUNTRY_HK
			}		
		}
		else
		{
			if( m_hParticleLogo != INVALID_PARTICLE_SEQUENCE_HANDLE && g_pData->GetUIMajorParticle() != NULL )
			{ 
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleLogo );
				m_hParticleLogo = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}	
#endif		

#ifdef BANDICAM_RECORDING
	if( g_pMain->IsCapturing() == true )
	{
		if( m_hParticleRec == INVALID_PARTICLE_SEQUENCE_HANDLE && g_pData->GetUIMajorParticle() != NULL )
		{
			m_hParticleRec = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Game_Rec",  450.f+32.f, -14.f+32.f, 0.f );
		}
	}
	else
	{
		if( m_hParticleRec != INVALID_PARTICLE_SEQUENCE_HANDLE && g_pData->GetUIMajorParticle() != NULL )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleRec );
			m_hParticleRec = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}
#endif

	if( NULL == m_pDLGStateChangePopup )//&& false == m_pDLGStateChangePopup->GetShow() )
	{
		if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
		{
			CX2StateBattleField* pStateBattleField = static_cast<CX2StateBattleField*>( g_pMain->GetNowState() );
			pStateBattleField->SetNowMovingToOtherPlace(false);
		}
		//g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGStateChangePopup, NULL, false );
	}
	
#ifdef SERV_2012_PVP_SEASON2_EVENT
	if ( m_pDLGTimeOfGlory != NULL )	/// ������ �ð� ���̾�α� Ȱ������ �� ����
	{
		m_fTimeOfGloryRemainTime += fElapsedTime;
		if(m_fTimeOfGloryRemainTime >= 4.f && m_fTimeOfGloryRemainTime <= 6.f)
		{
			MoveTimeOfGlory();			
		}
		else if(m_fTimeOfGloryRemainTime > 6.f)
		{
			m_pDLGTimeOfGlory->SetShow( false );

			SAFE_DELETE_DIALOG( m_pDLGTimeOfGlory );

			m_fTimeOfGloryRemainTime = 0.f;
		}
	}
#endif SERV_2012_PVP_SEASON2_EVENT

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipManager() && NULL != g_pData->GetRelationshipEffectManager() )
	{
		if ( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() ||
 			 CX2Main::XS_DUNGEON_GAME == g_pMain->GetNowStateID() ||
			 CX2Main::XS_PVP_GAME == g_pMain->GetNowStateID() ||
			 CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
		{
			g_pData->GetRelationshipManager()->OnFrameMove( fTime, fElapsedTime );
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef NEW_EMBLEM_MANAGER
	CX2EmblemManager::GetInstance()->OnFrameMove( fElapsedTime );
#endif // NEW_EMBLEM_MANAGER

#ifdef PLAY_EMOTION_BY_USER_SELECT
	if( NULL != g_pData &&
		NULL != g_pData->GetUIManager() &&
		false == g_pData->GetUIManager()->IsOpenedWindow() )
	{
		if( true == g_pMain->IsInheritStateMenu() )
		{
			CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>(g_pMain->GetNowState());
			if( NULL != pStateMenu )
			{
				if( false == pStateMenu->GetShowSystemMenu() &&
					false == pStateMenu->GetShowMsgBox() )
				{
					PlayEmotionByUserSelect();				
				}
			}
		}
		else
		{
			PlayEmotionByUserSelect();			
		}
	}
#endif // PLAY_EMOTION_BY_USER_SELECT

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

	if ( true == GetIsJumpingCharacterDlgPopup() )
	{
		// ����� Ű �Է��� ���´�.
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_BATTLE_FIELD :
			{
				if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
				{
					g_pX2Game->GetMyUnit()->SetCanNotInputTime_LUA( 0.5f );		
				}
			}
			break;
		case CX2Main::XS_VILLAGE_MAP :
			{
				if ( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() )
				{
					g_pTFieldGame->GetMyUnit()->SetCanNotInputTime( 0.5f );
				}
			}
			break;
		default :
			{
				SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChangeNotice );
				SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChange );
			}
			break;
		}
 
		// ��� Ok Cancel �˾� â�� �����Ѵ�. ( ��Ƽ �ʴ�, Ŀ�� �ʴ� ���.. )
 		if ( NULL != g_pKTDXApp->GetDialogManager_LUA() )
 		{
 			if ( NULL != m_pDlgJumpingCharacterClassChangeNotice )
 				g_pKTDXApp->GetDialogManager_LUA()->ClearAllOKAndCancelMsgDlg();
 
 			else if ( NULL != m_pDlgJumpingCharacterClassChange )
				g_pKTDXApp->GetDialogManager_LUA()->ClearAllOKAndCancelMsgDlg();
 		}
	}

#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef FIELD_BOSS_RAID
	CX2BossRaidManager::GetInstance()->OnFrameMove( fTime, fElapsedTime );
#endif // FIELD_BOSS_RAID

#ifdef SERV_ELESIS_UPDATE_EVENT
	if( g_pData != NULL && 
		g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().GetReserveShow() == true )
	{
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_SQUARE_GAME:
			ProcessElesisEvent( g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_Level, g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().GetNoteViewCount() );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetReserveShow( false );
			break;
		default:
			break;
		}
	}

	if ( NULL == g_pData ||
		NULL == g_pData->GetUIEffectSet() ||
		NULL == g_pData->GetUIMajorXMeshPlayer() )
		return false;

	/// ������ ���� �� ���� ó��
	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstEventNote && 
		true == g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstEventNote ) )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstEventNote );
		if(NULL != pMeshInst)
		{			
			if( pMeshInst->GetXSkinAnim() != NULL )
				pMeshInst->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
		}
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_Start = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstEventNoteStart );
	if( NULL != pMeshInst_Start && pMeshInst_Start->GetXSkinAnim() != NULL )
	{
		pMeshInst_Start->GetXSkinAnim()->GetRenderParam()->fOutLineWide = 1.2f;
	}

	/// ������ ���� ó��
	if ( m_bPlayNoteUI == true )
	{
		m_fPlayTime += fElapsedTime;

		/// ���� �ð� ����� ���� ��ü �ʱ�ȭ �� ����
		const float fEndPlayTime = 8.f;

		if( m_fPlayTime >= fEndPlayTime )
		{
			m_fPlayTime = 0.0f;
			m_bPlayNoteUI = false;
			m_bProcessNoteView = false;
			m_bShowNoteUI = false;

			// ���� â�� �����..
			if ( m_pDlgEventElesisNote != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgEventElesisNote, NULL, false );
			}
			m_pDlgEventElesisNote = NULL;
		}

		if( m_fPlayTime >= 4.f )
		{
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstance( m_hMeshInstEventNoteStart );
			if( NULL != m_pDlgEventElesisNote )
			{
				m_pDlgEventElesisNote->SetHasUnit(NULL);
				CKTDGUIStatic* pStatic = m_pDlgEventElesisNote->GetStatic_LUA("g_pStaticNoteView");
				if( pStatic != NULL )
				{
					pStatic->GetPicture( m_iRewardTitleIndex + 6 )->SetShow( true );

					int iTitleStringID = STR_ID_30093 + m_iRewardTitleIndex;
					std::wstring wstrTitle = g_pMain->GetStrByLienBreak( GET_STRING( iTitleStringID ), 170, XUF_DODUM_20_BOLD );
					pStatic->GetString( 0 )->msg = wstrTitle.c_str();

					int iDescStringID = STR_ID_30103 + m_iRewardTitleIndex;
					std::wstring wstrDesc = g_pMain->GetStrByLienBreak( GET_STRING( iDescStringID ), 170, XUF_DODUM_11_NORMAL );
					pStatic->GetString( 1 )->msg = wstrDesc.c_str();
				}
			}
		}
	}
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_GLOBAL_MISSION_MANAGER
	if( NULL != g_pData->GetGlobalMissionManager() )
	{
		g_pData->GetGlobalMissionManager()->OnFrameMove( fTime, fElapsedTime );
	}
#endif SERV_GLOBAL_MISSION_MANAGER

	return S_OK;
}

HRESULT CX2State::OnFrameRender()
{
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
	g_pKTDXApp->GetDGManager()->ObjectChainSort();

	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
    g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE

	g_pKTDXApp->GetDGManager()->FrontUIRender();


	g_pMain->GetTextManager()->OnFrameRender();

	if ( g_pMain->GetGameEdit() )
		g_pMain->GetGameEdit()->OnFrameRender();

	//{{ 09.03.23 ���¿� : �̺�Ʈ Ÿ�̸� ��ġ �̵�
	if( g_pInstanceData != NULL &&
		g_pInstanceData->GetMiniMapUI() != NULL )
	{
		g_pInstanceData->GetMiniMapUI()->UpdateEventNotice();
#ifdef EVENT_CARNIVAL_DECORATION
		g_pInstanceData->GetMiniMapUI()->UpdateCarnivalDeco();
#endif //EVENT_CARNIVAL_DECORATION
	}
	//RenderMarketingEventTimer();
	//}}

#ifdef LOADING_ANIMATION_TEST
	g_pMain->GetLoadingAnimation()->OnFrameRender();
#endif LOADING_ANIMATION_TEST
	
	m_pCursor->OnFrameRender();

	return S_OK;
}

bool CX2State::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	switch( uMsg )
	{    


#ifdef FULL_SCREEN_OPTION_SAVE_TEST
	case WM_SYSKEYDOWN:
		{
			switch( wParam )
			{
			case VK_RETURN:
				{
					//if( GetDXUTState().GetHandleAltEnter() )
					{
						// Toggle full screen upon alt-enter 
						DWORD dwMask = (1 << 29);
						if( (lParam & dwMask) != 0 ) // Alt is down also
						{
							g_pMain->GetGameOption().SetFullScreen( !g_pMain->GetGameOption().GetOptionList().m_bFullScreen );
							return true;
						}
					}
				} break;
			}
		} break;
#endif FULL_SCREEN_OPTION_SAVE_TEST


#ifdef DROP_FILE_TEST
	case WM_DROPFILES:
		{
			UINT nDroppedFiles = 0;
			nDroppedFiles = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, NULL, MAX_PATH );

			wstring wstrFilePath; 
			WCHAR wszFilePath[MAX_PATH] = L"";	
			for( UINT i=0; i<nDroppedFiles; i++ )
			{
				DragQueryFile( (HDROP)wParam, i, wszFilePath, MAX_PATH);

				WCHAR wszFilePathOnly[_MAX_DIR]	= L"";
				WCHAR wszFileNameOnly[_MAX_FNAME] = L"";
				DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, (WCHAR*)wszFilePath );

				string directoryName = "";
				ConvertWCHARToChar( directoryName, wszFilePathOnly );
				g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( directoryName, true );
			}

			return true;
		} break;
#endif DROP_FILE_TEST





		case WM_SETCURSOR:
			{
				if ( LOWORD(lParam) == HTCLIENT )
				{
					if ( m_pCursor->GetShowHardwareCursor() == true )
					{
						SetCursor( m_pCursor->GetCursorHandle() );
						
					}
					else
					{
						SetCursor( NULL );
					}

					return true;
				}
			}
			break;

		case CKTDXApp::KM_UI_CONTROL_EVENT:
			{
				if ( UIEventProc( hWnd, uMsg, wParam, lParam ) == true )
					return true;
			}
			break;

		case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
			{
				if ( m_bIgnoreAllUIEvent == true )
					return false;

				if ( UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
					return true;
			}
			break;

		case CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC:
			{
				if ( UICustomFuncProc( hWnd, uMsg, wParam, lParam ) == true )
					return true;
			}
			break;

		case XGM_RECEIVE_FROM_SERVER:
			{
				UIServerEventProc( hWnd, uMsg, wParam, lParam );
				KSerBuffer* pBuff = (KSerBuffer*)lParam;
				SAFE_DELETE( pBuff );
				return true;
			}
			break;

		case XGM_SERVER_PACKET_TIMEOUT:
			{
				if ( UIServerTimeOutProc( hWnd, uMsg, wParam, lParam ) == true )
					return true;
			}
			break;

#ifdef ADD_COLLECT_CLIENT_INFO
		case XGM_RECEIVE_FROM_COLLECTSERVER:
			{
				if( g_pMain->GetCollectHackInfo() != NULL )
					g_pMain->GetCollectHackInfo()->UIServerEventProc( hWnd, uMsg, wParam, lParam );					
				return true;
			}
			break;
#endif //ADD_COLLECT_CLIENT_INFO

		case CKTDXApp::KM_RECEIVE_FROM_P2P:
			{
				if ( UIP2PEventProc( hWnd, uMsg, wParam, lParam ) == true )
					return true;
			}
			break;

		case CKTDXApp::KM_TCP_RELAY:
			{
				//return TCPRelayEventProc( hWnd, uMsg, wParam, lParam );
				MessageBox(g_pKTDXApp->GetHWND(), L"TCP_RELAY �ּ�ó�� �س�� ��Ǯ����", L"������", MB_OK );
			}
			break;
			//{{ JHKang / ������ / 2010.09.2 / ������ �ּ�ȭ, �ִ�ȭ, ���� ��ư �߰� �� �޽��� ���� ����
#ifdef WINDOW_SYSTEM_BUTTON_FUNC
		case WM_SYSCOMMAND:
			switch( wParam )
			{
			case SC_MAXIMIZE:
				{
					g_pMain->GetGameOption().SetFullScreen( !g_pMain->GetGameOption().GetOptionList().m_bFullScreen );
					break;
				}

			}
			break;
#endif WINDOW_SYSTEM_BUTTON_FUNC
			//}} JHKang / ������ / 2010.09.2 / ������ �ּ�ȭ, �ִ�ȭ, ���� ��ư �߰� �� �޽��� ���� ����
	}

#ifdef NEW_VILLAGE_UI
	// ����! �巡���� �ʿ��� UI��� UIManager�� MsgProc�� ���� ���� ���� MsgProc�� ���� ��!
	// MouseUp Ÿ�ֿ̹� 
	if ( g_pData->GetUIManager() != NULL )
	{
		g_pData->GetUIManager()->MsgProc( hWnd, uMsg, wParam, lParam);
	}
#endif	

	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	//����! ��ĥ ���, X2StateMenu�� �ִ� MsgProc�� ��������Ѵ�. 

#ifdef _SERVICE_
/*
	switch( uMsg )
	{

	case WM_MOUSEMOVE:
		{
			if ( wParam & MK_LBUTTON )
			{
				if ( g_bIsDXUTSpecialUser == true && g_pKTDXApp->GetIsFullScreen() == false && 
					CKTDGUIDialog::GetControlFocus() == NULL && CX2SlotManager::GetSlotBeforeDragging() == NULL &&
					CX2UnitViewerUI::GetUnitClicked() == false )
				{
					::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
				}
			}
		}
		break;

	}
*/
#endif
/*
	if ( uMsg == WM_SIZING )
	{
		RECT rc;
		GetWindowRect( g_pKTDXApp->GetHWND(), &rc );
		int leftSubtract = ( ( ((RECT*)lParam)->left - rc.left  ));
		int rightSubtract = ( ( ((RECT*)lParam)->right - rc.right ) );
		int topSubtract = ( (((RECT*)lParam)->top - rc.top ) );
		int bottomSubtract =  ((RECT*)lParam)->bottom  - rc.bottom ;
	
		int tempValue = (int)( (1024) * (float)( abs((float)bottomSubtract)/(768)));
		if ( bottomSubtract < 0 )
			tempValue = -tempValue;

		((RECT*)lParam)->right = ((RECT*)lParam)->right + tempValue;

		tempValue = (int)( (768) * (float)( abs((float)rightSubtract)/(1024)));
		if ( rightSubtract < 0 )
			tempValue = -tempValue;

		((RECT*)lParam)->bottom = ((RECT*)lParam)->bottom + tempValue;


		tempValue = (int)( (1024) * (float)( abs((float)topSubtract)/(768)));
		if ( topSubtract < 0 )
			tempValue = -tempValue;

		((RECT*)lParam)->left = ((RECT*)lParam)->left + tempValue;

		tempValue = (int)( (768) * (float)( abs((float)leftSubtract)/(1024)));
		if ( leftSubtract < 0 )
			tempValue = -tempValue;

		((RECT*)lParam)->top = ((RECT*)lParam)->top + tempValue;

		return TRUE;
	}
	*/

	return false;
}

bool CX2State::UIEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

	switch( pControl->GetType() )
	{
		case CKTDGUIControl::UCT_BUTTON:
			{
				if( wcscmp( pControl->GetName(), L"ButtonMsgBoxOK" ) == 0
					&& wParam == CKTDGUIButton::BEM_BUTTON_MOUSEUP )
				{
					g_pKTDXApp->GetDIManager()->SetEnable(true);
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
					
					return true;
				}
				else if ( wcscmp( pControl->GetName(), L"MsgBoxOkAndCancelCancelButton")  == 0 
					&& wParam == CKTDGUIButton::BEM_BUTTON_MOUSEUP )
				{
					if ( pControl->GetDialog() != NULL && 
						wcscmp( pControl->GetDialog()->GetName(), L"MsgBoxQuitGameByESC") == 0 )
					{
						m_bOpenMsgBoxQuitGame = false;
					}

					g_pKTDXApp->GetDIManager()->SetEnable(true);
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
					return true;
				}
			}
			break;
	}

	return false;
}

bool CX2State::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	if ( NULL != g_pData->GetMyUser() &&
		CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
	{
#ifdef ITEM_CHEAT_POPUP_TEST
		if( true == g_pInstanceData->GetItemCheatPopup().UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
#endif //ITEM_CHEAT_POPUP_TEST

#ifdef MODIFY_GAME_EDIT_CAMERA
		CX2GameEdit* pGameEdit = g_pMain->GetGameEdit();
		if( NULL != pGameEdit )
		{
			if( true == pGameEdit->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
				return true;
		}
#endif //MODIFY_GAME_EDIT_CAMERA
	}

	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		if( true == g_pInstanceData->GetMiniMapUI()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}

#ifndef NEW_SKILL_TREE
	if( NULL != g_pInstanceData->GetSkillTreeUI() )
	{
		if( true == g_pInstanceData->GetSkillTreeUI()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif
#ifdef NEW_VILLAGE_UI
	if(g_pData != NULL && g_pData->GetUIManager() != NULL)
	{
		if( g_pData->GetUIManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif

	//{{ ����� : [2009/9/16] //	��� �̺�Ʈ ����
#ifdef GUILD_MANAGEMENT
    if(g_pData != NULL && g_pData->GetGuildManager() != NULL)
	{
		if ( g_pData->GetGuildManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif	//	GUILD_MANAGEMENT
	//}} ����� : [2009/9/16] //	��� �̺�Ʈ ����

	//{{ ����� : [2011/3/22/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	if(g_pData != NULL && g_pData->GetWorldMissionManager() != NULL )
	{
		if ( g_pData->GetWorldMissionManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} ����� : [2011/3/22/] //	���� �̼�

	
	if( NULL != g_pMain->GetTutorSystem() )
	{
		if( true == g_pMain->GetTutorSystem()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}


	if( NULL != g_pMain->GetPartyUI() )
	{
		if( true == g_pMain->GetPartyUI()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		{
			return true;
		}
	}

	

	if ( g_pChatBox != NULL )
	{
		if ( true == g_pChatBox->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// ������ / 13-01-15 / ���� �÷��� ��, �ҷ� ���� ���� ���� �ý���
	
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();		// CX2GageManager �� �ν��Ͻ��� ���´�.
	if ( NULL != pGageManager )
	{
		if ( true == pGageManager->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )		// CX2GageManager �� �ν��Ͻ� �� �����ϴ� UICustomEventProc �� ȣ���Ѵ�.
			return true;
	}
	
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

	if ( g_pData->GetMessenger() != NULL && g_pData->GetMessenger()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if ( g_pMain->GetLVUpEventMgr() != NULL && g_pMain->GetLVUpEventMgr()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	if( g_pData->GetClassChangePopup().UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;
#endif

#ifdef DEPARTURE_PREVENTION_SYSTEM
	if ( NULL != g_pData && NULL != g_pData->GetDeparturePrevention() )
	{
		if ( true == g_pData->GetDeparturePrevention()->UICustomEventProc(hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif

#ifdef SERV_GLOBAL_MISSION_MANAGER
	if(g_pData != NULL && g_pData->GetGlobalMissionManager() != NULL )
	{
		if ( g_pData->GetGlobalMissionManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_GLOBAL_MISSION_MANAGER

	switch( wParam )
	{
	case SUCM_EXIT:
		{
			//{{ kimhc // 2010.9.29 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
#ifdef	AVATAR_IMAGE_TEST
			g_pMain->ScreenCaptureAndSendToWeb( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );
#endif	AVATAR_IMAGE_TEST
			//}} kimhc // 2010.9.29 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ

#ifdef RIDING_SYSTEM
			if( NULL != CX2RidingPetManager::GetInstance() )
				CX2RidingPetManager::GetInstance()->Handler_EGS_UNSUMMON_RIDING_PET_REQ();
#endif //RIDING_SYSTEM

#ifdef DEPARTURE_PREVENTION_SYSTEM
			if ( CX2Main::XS_SERVER_SELECT != g_pMain->GetNowStateID() && NULL != g_pData && NULL != g_pKTDXApp && NULL != g_pData->GetDeparturePrevention() )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pMsgBoxQuitGame, NULL, false );
				g_pData->GetDeparturePrevention()->SetPrevState( CX2DeparturePrevention::SDPPS_EXIT );
				return g_pData->GetDeparturePrevention()->Handler_EGS_OUT_USER_RETAINING_REQ();
			}
			else
			{
				return Handler_EGS_CLIENT_QUIT_REQ();
			}
#else
			return Handler_EGS_CLIENT_QUIT_REQ();
#endif
		}		
		break;

	case SUCM_EXIT_WINDOW:
		{
			QuitGame();
			return true;
		}
		break;

	case SUCM_GAME_EDIT_COMMAND:
		{
			ELSWORD_VIRTUALIZER_START

			CKTDGUIIMEEditBox* pIMEBox = (CKTDGUIIMEEditBox*)lParam;
			if ( g_pMain != NULL && g_pMain->GetGameEdit() != NULL && pIMEBox != NULL && pIMEBox->GetText() != NULL 
				&& g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				wstring commandString = pIMEBox->GetText();
				pIMEBox->SetText( L"" );
				g_pMain->GetGameEdit()->ExecCommand( commandString.c_str() );

				return true;
			}
			else
			{
				return false;
			}

			ELSWORD_VIRTUALIZER_END

		}
		break;

	case SUCM_LAST_MSG_BY_ESC_OK:
		{
			LastMsgPopOk();
		}
		break;

	case SUCM_LAST_MSG_BY_ESC_CANCEL:
		{
			LastMsgPopCancel();
		}
		break;

	case SUCM_RESURRECT_OK:
		{
			if ( NULL != g_pX2Game )
				g_pX2Game->ResurrectOtherUserOK();
		}
		break;


	case SUCM_RESURRECT_CANCEL:
		{
			if ( NULL != g_pX2Game )
				g_pX2Game->ResurrectOtherUserCancel();
		}
		break;


	case SUCM_RESURRECT_USE_CASH_OK:
		{
			if ( NULL != g_pX2Game )
				g_pX2Game->Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( 0, true );
		}
		break;

	case SUCM_RESURRECT_USE_CASH_CANCEL:
		{
			if ( NULL != g_pX2Game )
				g_pX2Game->Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_REQ( false );
		}
		break;
// 	case SUCM_EVENT_CLOCK_MOUSE_OVER:
// 		{
// 			OpenGuestUserEventTimerPopup();
// 			return true;
// 		} break;
// 
// 	case SUCM_EVENT_CLOCK_MOUSE_OUT:
// 		{
// 			CloseGuestUserEventTimerPopup();
// 			return true;
// 		} break;

	case SUCM_GUEST_USER_GOTO_UNIT_SELECT:
		{
			if( NULL != g_pData &&
				NULL != g_pData->GetMyUser() &&
				true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->SetGuestUserMovingToCharacterSelect( true );
			}
			
			CX2StateAutoChanger::TARGET_DETAIL targetDetail;
#ifdef ELSWORD_NEW_BEGINNING
			g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_BEGINNING, targetDetail );
#else
			g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_SERVER_SELECT, targetDetail );
#endif ELSWORD_NEW_BEGINNING
			if( NULL != m_pDLGGuestUserGoUnitSelect )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGuestUserGoUnitSelect, NULL, false );
				m_pDLGGuestUserGoUnitSelect = NULL;
			}

			return true;
		} break;

	case SUCM_ESC_PUSH:
		{
			HandleMsgByESCImp();
		}
		break;

	case SUCM_USER_INFO_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUserInfo, NULL, false );
			m_pDLGUserInfo = NULL;

			return true;
		}
		break;

		//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
	case SUCM_UNREADY_OK:
		{
			if ( m_pDLGUnReady != NULL )
			{
				switch ( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_PVP_ROOM:
					{
						CX2PVPRoom* pPvpRoom = g_pData->GetPVPRoom();

						if ( pPvpRoom != NULL &&
							pPvpRoom->GetMySlot() != NULL && 
							pPvpRoom->GetMySlot()->m_bReady == true )
							pPvpRoom->Handler_EGS_CHANGE_READY_REQ();

					} break;

				case CX2Main::XS_VILLAGE_MAP:
					{
						CX2PartyManager::PartyMemberData* pPartyMemberData = 
							g_pData->GetPartyManager()->GetMyPartyMemberData();
					} break;

				default:
					break;
				}

				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUnReady, NULL, false );

				//ProcessChangeEquipSlotAfterUnReady();

				return true;
			}

		} break;
	
	case SUCM_UNREADY_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUnReady, NULL, false );
		} break;
#endif MODIFY_INFINITE_SKILL_BUG
		//}} kimhc // 2010.3.26 // ���� ��ų ���� ����


	//case SUCM_GET_POST_ITEM:
	//	{
	//		CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
	//		CKTDGUIDialogType pDialog = pButton->GetDialog();
	//		if( NULL != pDialog )
	//		{
	//			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pDialog, NULL, false );
	//		}
	//		return Handler_EGS_GET_POST_LETTER_LIST_REQ();
	//	} break;
#ifdef PC_BANG_BENEFIT_GUIDE
	case SUCM_PC_BANG_GUIDE_OK:
		{
			if( NULL != m_pDLGPCBangBenefitGuide )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPCBangBenefitGuide, NULL, false );
			}
		} break;
#endif //PC_BANG_BENEFIT_GUIDE

#ifdef SERV_ADVERTISEMENT_EVENT
	case SUCM_CLOSE_ADVERTISEMENT:
		{
			m_pDlgAdvertisement->SetShowEnable( false, false );
		} break;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
	case SUCM_AGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER:
		{
			/// �ٻ� ������ ��, ��ȯ�� �� ������ ����
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT( false );
			else
				Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT( true );
		} break;

	case SUCM_DISAGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER:
		{
			Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT( false );
		} break;
	case SUCM_AGREE_SUMMON_ROOM_BY_MARRIEGED_USER:
		{
			/// �ٻ� ������ ��, ��ȯ�� �� ������ ����
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT( false );
			else
				Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT( true );
		} break;

	case SUCM_DISAGREE_SUMMON_ROOM_BY_MARRIEGED_USER:
		{
			Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT( false );
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM



#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����


	case SUCM_JUMPING_CHARACTER_DLG_CLOSE :
		{
			// ���� ���� �˸�â�� �����Ѵ�.
			CX2State * pNowState = static_cast<CX2State *> ( g_pMain->GetNowState() );

			if ( NULL != pNowState )
			{
				pNowState->SetShowJumpingCharacterClassNoticeChangeDlg(false);
			}
		}
		break;
			
	case SUCM_JUMPING_CHARACTER_CLASS_CHANGE_DLG_CLOSE :
		{
			// ���� ���� â�� �����Ѵ�.
			CX2State * pNowState = static_cast<CX2State *> ( g_pMain->GetNowState() );

			if ( NULL != pNowState )
			{
				pNowState->SetShowJumpingCharacterClassChangeDlg(false);
			}

		}
		break;

	case SUCM_JUMPING_CHARACTER_CLASS_CHANGE_DLG_POPUP :
		{
			// ���� ���� â�� ���� ���� ���� �˸�â�� �����Ѵ�.
			CX2State * pNowState = static_cast<CX2State *> ( g_pMain->GetNowState() );

			if ( NULL != pNowState )
			{
				pNowState->SetShowJumpingCharacterClassNoticeChangeDlg(false);
				pNowState->SetShowJumpingCharacterClassChangeDlg(true);
			}

		}
		break;

	case SUCM_JUMPING_CHARACTER_CLASS_CHANGE_OK :
		{
			// ������ �����ߴ�! ��ũ��Ʈ �� DummyInt 0���� �ش� ĳ������ Class Type �̴�.
			CKTDGUIButton * pButton = reinterpret_cast <CKTDGUIButton*> ( lParam );
			int iUnitClass = pButton->GetDummyInt (0);

			Handler_EGS_JUMPING_CHARACTER_REQ( iUnitClass );

			CX2State * pNowState = static_cast<CX2State *> ( g_pMain->GetNowState() );

			if ( NULL != pNowState )
			{
				// ���� ���� â�� �����Ѵ�.
				pNowState->SetShowJumpingCharacterClassChangeDlg(false);
			}
		}
		break;

#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
	case SUCM_EVENT_CHARACTER_QUEST_RANKING_CLOSE :
		{
			SAFE_DELETE_DIALOG ( m_pDlgEventCharacterRanking );
			m_pDlgEventCharacterRanking = NULL;
		}
		break;
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
	case SUCM_EVENT_NOTE_VIEW_OK:
		{
			if( true == m_bProcessNoteView )
				return true;

			// ���ư��� �߿��� �޽����� ���� ����~
			if( m_hMeshInstEventNoteStart != INVALID_MESH_INSTANCE_HANDLE &&  
				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstEventNoteStart ) == true )
				return true;
			/*
#ifdef FIX_ICE_HEATER_OPEN_BUG
			//EGS_OPEN_RANDOM_ITEM_ACK�� �ޱ� ���� m_eRandomItemEventType ������ RIOET_NONE �Ǵ� ���, �������� ���¾��� ������ϴ�.
			//OK�� CANCLE �Ѵ� Ÿ�� �Ǹ� m_eRandomItemEventType ������ RIOET_NONE ���� ������ �Ǿ� �Ѵ� Ż �� ������ �����߽��ϴ�.

			//CANCLE���� Ż ��쿡 ���� ����ó��
			if( RIOET_NONE == m_eRandomItemEventType ) 
				return true;

			//OK ���� Ż ��쿡 ���� ����ó��
			if( NULL != m_pDLGOpenAttraction )
				m_pDLGOpenAttraction->SetCloseCustomUIEventID(-1);
#endif //FIX_ICE_HEATER_OPEN_BUG
			*/

			Handler_EGS_EVENT_NOTE_VIEW_REQ();
		}
		break;
	case SUCM_EVENT_NOTE_VIEW_CANCLE:
		{
			// ���ư��� �߿��� �޽����� ���� ����~
			if( m_hMeshInstEventNoteStart != INVALID_MESH_INSTANCE_HANDLE &&  
				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstEventNoteStart ) == true )
				return true;

			if( m_hMeshInstEventNote != INVALID_MESH_INSTANCE_HANDLE )
			{
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstance( m_hMeshInstEventNote );
				if( NULL != m_pDlgEventElesisNote )
				{
					m_pDlgEventElesisNote->SetHasUnit( NULL );
				}
			}

			if ( m_pDlgEventElesisNote != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgEventElesisNote, NULL, false );
				m_pDlgEventElesisNote = NULL;
			}
		}
		break;
#endif SERV_ELESIS_UPDATE_EVENT

#if defined( SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL ) || defined( ACCOUNT_BLOCK_MESSAGE_RENEWAL_KR )
		case SUCM_BLOCK_ACCOUNT_CONNECT :
			{
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
			}break;
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL // ACCOUNT_BLOCK_MESSAGE_RENEWAL_KR
	}



#ifdef ADDED_RELATIONSHIP_SYSTEM	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
	if ( NULL != g_pData->GetRelationshipManager() )
	{
		if ( true == g_pData->GetRelationshipManager()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;			
	}
#endif // ADDED_RELATIONSHIP_SYSTEM
	return false;
}

bool CX2State::UICustomFuncProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//wstring funcWstrName = (const WCHAR*)wParam;
	//LuaFuncCall( funcName.c_str() );

	//string funcStrName;
	//ConvertWCHARToChar( funcStrName, funcWstrName.c_str() );
    const char* pszParam = (const char*) wParam;
    std::string funcStrName = ( pszParam != NULL ) ? pszParam : "";


//	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pKTDXApp",		g_pKTDXApp );

	RegisterLuaBind();

	lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), funcStrName.c_str() );

	return true;
}

bool CX2State::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pData->GetUIManager() != NULL)
	{
		if ( true == g_pData->GetUIManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam))
			return true;
	}


#ifdef NEW_SKILL_TREE
	if( NULL != g_pData->GetSkillTree() )
	{
		if( true == g_pData->GetSkillTree()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true; 
	}
#endif NEW_SKILL_TREE


	if( NULL != g_pMain->GetTutorSystem() )
	{
		if( true == g_pMain->GetTutorSystem()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true; 
	}

	
	if( NULL != g_pData->GetPartyManager() )
	{
		if( true == g_pData->GetPartyManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}

	if( NULL != g_pChatBox )
	{
		if( true == g_pChatBox->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true; 
	}

	//{{ ����� : [2009/9/16] //	��� ���� �̺�Ʈ
#ifdef GUILD_MANAGEMENT
	if( g_pData->GetGuildManager() != NULL)
	{
		if( g_pData->GetGuildManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif	//	GUILD_MANAGEMENT
	//}} ����� : [2009/9/16] //	��� ���� �̺�Ʈ


#ifdef ADDED_RELATIONSHIP_SYSTEM	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
	if( g_pData->GetRelationshipManager() != NULL)
	{
		if( g_pData->GetRelationshipManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif // ADDED_RELATIONSHIP_SYSTEM


	if ( g_pData->GetMessenger() != NULL &&
		g_pData->GetMessenger()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

//#ifdef TITLE_SYSTEM
    if(g_pData->GetTitleManager() != NULL &&
        g_pData->GetTitleManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
        return true;
//#endif

#ifdef POSTBOX
    if( g_pMain->GetPostBox() != NULL)
    {
        switch( wParam )
        {
        case EGS_RECEIVE_LETTER_NOT:                        
            g_pMain->GetPostBox()->AddNewLetter( hWnd, uMsg, wParam, lParam );
            return false;
        }
    }
#endif

#ifdef SERV_PET_SYSTEM
	if(g_pData->GetPetManager() != NULL &&
		g_pData->GetPetManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
		return true;
#endif

#ifdef RIDING_SYSTEM
	if( NULL !=CX2RidingPetManager::GetInstance() &&
		CX2RidingPetManager::GetInstance()->UIServerEventProc( hWnd, uMsg, wParam,lParam ) == true )
		return true;
#endif //RIDING_SYSTEM

#ifdef SERV_LOCAL_RANKING_SYSTEM //���νý���
	if( NULL != g_pData->GetProfileManager() )
	{
		if( true ==g_pData->GetProfileManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam) )
			return true;
	}
#endif //SERV_LOCAL_RANKING_SYSTEM

	//{{ ����� : [2011/3/23/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	if( g_pData->GetWorldMissionManager() != NULL &&
		g_pData->GetWorldMissionManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} ����� : [2011/3/23/] //	���� �̼�

#ifdef DEPARTURE_PREVENTION_SYSTEM
	if ( NULL !=g_pData->GetDeparturePrevention() )
	{
		if ( true == g_pData->GetDeparturePrevention()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif

#ifdef BUFF_TEMPLET_SYSTEM
#ifdef COME_BACK_REWARD
	if ( NULL !=g_pData->GetPremiumBuffManager() )
	{
		if ( true == g_pData->GetPremiumBuffManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif
#endif BUFF_TEMPLET_SYSTEM	

#ifdef FIELD_BOSS_RAID
	if( true == CX2BossRaidManager::GetInstance()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
		return true;
#endif // FIELD_BOSS_RAID

#ifdef SERV_GLOBAL_MISSION_MANAGER
	if( g_pData->GetGlobalMissionManager() != NULL &&
		g_pData->GetGlobalMissionManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}
#endif SERV_GLOBAL_MISSION_MANAGER

	switch( wParam )
	{
		//{{ ����� : [2009/8/19] //	�ٽ��� Ȯ�弭������ ��Ŷ ó��
#ifdef HACK_SHIELD
	case EGS_HACKSHIELD_CHECK_REQ:
		return Handler_EGS_HACKSHIELD_CHECK_REQ( hWnd, uMsg, wParam, lParam );
		break;
#endif
		//}} ����� : [2009/8/19] //	�ٽ��� Ȯ�弭������ ��Ŷ ó��

		//{{ kimhc // 2011-08-10 // ä�θ� SOAP
#ifdef	SERV_CHANNELING_SOAP
	case EGS_TOONILAND_USER_ID_NOT:
		return Handler_EGS_TOONILAND_USER_ID_NOT( hWnd, uMsg, wParam, lParam );
		break;
#endif	SERV_CHANNELING_SOAP
		//}} kimhc // 2011-08-10 // ä�θ� SOAP

	case EGS_KNPROTECT_AUTH_CHECK_NOT:
		return Handler_EGS_KNPROTECT_AUTH_CHECK_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_KNPROTECT_USER_SHOWDOWN_NOT:
		return Handler_EGS_KNPROTECT_USER_SHOWDOWN_NOT( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_NPROTECT_CS_AUTH_30
	case EGS_NPROTECT_CSAUTH3_CHECK_NOT:
		return Handler_EGS_NPROTECT_CSAUTH3_CHECK_NOT( hWnd, uMsg, wParam, lParam );
		break;
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef CLIENT_USE_XTRAP	// XTRAP Ŭ���̾�Ʈ - ���� ��Ŷ �޴� �κ�
	case EGS_XTRAP_REQ:
		return Handler_EGS_XTRAP_REQ( hWnd, uMsg, wParam, lParam );
		break;		
#endif CLIENT_USE_XTRAP

	case EGS_NOTIFY_MSG_NOT:
		return Handler_EGS_NOTIFY_MSG_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_ADMIN_SERVER_STATUS_ACK:
		return Handler_EGS_ADMIN_SERVER_STATUS_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case E_CONNECTION_LOST_NOT:
		{
#ifdef PC_BANG_SANG
			// oasis907 : ����� [2010.2.3] // 
			if(m_bFailPCBANGAuth == true)
			{
				;
			}
			else
			{
#endif PC_BANG_SANG
				//PostQuitMessage(0);
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
				// ĳ���� ����â�̶�� �������� �����ϰ� �ƴ� ��� ������ �����Ų��.
				
				if ( g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT )
				{					
					CX2StateServerSelect::m_bCanNotConenctToBusyServer = true;
					//m_pMsgOkAndCancelFailToConnectServerInServerSelectState = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ),  
					//	L"���� �� ������ ã�� ���߽��ϴ�.\n�ٽ� �����Ͻðڽ��ϱ�?", 
					//	SUCM_RECONNECT_SERVER_OK, this, SUCM_EXIT, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
				}
				else
				{
#ifdef LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
				if(m_bConnectFullChannel == false && g_pMain->GetNowStateID() != CX2Main::XS_LOGIN )
#else
				if(m_bConnectFullChannel == false)
#endif LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
					{
						std::wstring packetExplain;
						packetExplain = GET_STRING( STR_ID_539 );
						m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), packetExplain.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
#ifdef FIX_UNLIMITED_STR_ID_539
					g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( m_pMsgOkFailToConnectServer );
#endif FIX_UNLIMITED_STR_ID_539
					}
					m_bConnectFullChannel = false;
					m_bFailToConnectServer = true;
					return true;
				}

#else  // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
				if ( true == m_bReConnectChannelServer )
				{
					m_bReConnectChannelServer = false;
#ifdef FIX_REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
					if ( g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT )
					{
						CX2StateServerSelect::m_bCanNotConenctToBusyServer = true;
						
						static_cast<CX2StateServerSelect *> ( this )->ConnectToLowerUserCountGameServer();							
					}
					// m_iTryConnectChannelID = 
#endif // FIX_REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
				}
				else
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
				{
#ifdef LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
				if(m_bConnectFullChannel == false && g_pMain->GetNowStateID() != CX2Main::XS_LOGIN )
#else
				if(m_bConnectFullChannel == false)
#endif LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
					{
						std::wstring packetExplain;
						packetExplain = GET_STRING( STR_ID_539 );
#ifdef REFORM_ENTRY_POINT
						m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), packetExplain.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
#else //REFORM_ENTRY_POINT
						m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), packetExplain.c_str(), this );
#endif //REFORM_ENTRY_POINT
#ifdef FIX_UNLIMITED_STR_ID_539
					g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( m_pMsgOkFailToConnectServer );
#endif FIX_UNLIMITED_STR_ID_539				
					}
					m_bConnectFullChannel = false;
					m_bFailToConnectServer = true;
					return true;
				}

#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef PC_BANG_SANG
			}
#endif PC_BANG_SANG
		}
		break;

		//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	case E_CH_CONNECTION_LOST_FOR_DDOS_GUARD_NOT:
		{
			ASSERT( NULL != g_pData->GetServerProtocol() );

			if( g_pData->GetServerProtocol()->ReconnectFromChannelServer() == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_735 ), this );	

				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );									// fix!! id ����
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
			}

			return false;
		}
		break;

	case E_GS_CONNECTION_LOST_FOR_DDOS_GUARD_NOT:
		{
			ASSERT( NULL != g_pData->GetServerProtocol() );

			if( g_pData->GetServerProtocol()->ReconnectFromGameServer() == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_539 ), this );	

				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );									// fix!! id ����
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
			}

			return false;
		}
		break;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
		//}}

	case EGS_CLIENT_QUIT_ACK:
		{
			return Handler_EGS_CLIENT_QUIT_ACK();
		}
		break;

	case EGS_CHANGE_OPTION_PLAY_GUIDE_ACK:
		{
			return Handler_EGS_CHANGE_OPTION_PLAY_GUIDE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK:
		{
			return Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK:
		{
			return Handler_EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK:
		{
			return Handler_EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;

	case EGS_CHECK_INVALID_ROOM_NOT:
		{
			return Handler_EGS_CHECK_INVALID_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_UPDATE_QUEST_ACK:
		{
			return Handler_EGS_UPDATE_QUEST_ACK( hWnd, uMsg, wParam, lParam );

		} 
		break;

	case EGS_UPDATE_QUEST_NOT:
		{
			return Handler_EGS_UPDATE_QUEST_NOT( hWnd, uMsg, wParam, lParam );
		} 
		break;

	case EGS_ADMIN_CHANGE_ED_ACK:
		{
			return Handler_EGS_ADMIN_CHANGE_ED_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ADMIN_CHANGE_SKILL_POINT_ACK:
		{
			return Handler_EGS_ADMIN_CHANGE_SKILL_POINT_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_ADMIN_INIT_SKILL_TREE_ACK:
		{
			return Handler_EGS_ADMIN_INIT_SKILL_TREE_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_CHECK_SUM_ACK:
		{
			return Handler_EGS_CHECK_SUM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ADMIN_KICK_USER_ACK:
		{
			return Handler_EGS_ADMIN_KICK_USER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

		//{{ 2008. 1. 2  ������  
	case EGS_ADMIN_CHANGE_SPIRIT_ACK:
		{
			return Handler_EGS_ADMIN_CHANGE_SPIRIT_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}}

	case EGS_ADMIN_SET_PC_BANG_ACK:
		{
			return Handler_EGS_ADMIN_SET_PC_BANG_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

		//{{ 2008. 1. 2  ������  
	case EGS_ADMIN_CHANGE_VSPOINT_ACK:
		{
			return Handler_EGS_ADMIN_CHANGE_VSPOINT_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}}

		//{{ 2008. 2. 18  ������  
	case EGS_ADMIN_CHANGE_UNIT_CLASS_ACK:
		{
			return Handler_EGS_ADMIN_CHANGE_UNIT_CLASS_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}}

		//{{ 2008. 10. 24  ������	Ÿ��Ʋ ġƮ
	case EGS_ADMIN_GET_TITLE_CHEAT_ACK:
		{
			return Handler_EGS_ADMIN_GET_TITLE_CHEAT_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
		//}}

	//case EGS_REWARD_TO_RECOMMEND_USER_NOT:
	//	{
	//		return Handler_EGS_REWARD_TO_RECOMMEND_USER_NOT( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	//case EGS_REWARD_TO_TUTORIAL_USER_NOT:
	//	{
	//		return Handler_EGS_REWARD_TO_TUTORIAL_USER_NOT( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	//case EGS_REWARD_MARKETING_EVENT_NOT:
	//	{
	//		return Handler_EGS_REWARD_MARKETING_EVENT_NOT( hWnd, uMsg, wParam, lParam );
	//	} break;

	case EGS_RECOMMEND_USER_RESULT_NOT:
		{
			return Handler_EGS_RECOMMEND_USER_RESULT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHECK_BALANCE_ACK:
		{
			return Handler_EGS_CHECK_BALANCE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CASH_PRODUCT_INFO_CHANGED_NOT:
		{
			return Handler_EGS_CASH_PRODUCT_INFO_CHANGED_NOT();
		}
		break;

	case EGS_GUEST_USER_LIMIT_LEVEL_NOT:
		{
			return Handler_EGS_GUEST_USER_LIMIT_LEVEL_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_NEW_PRESENT_CASH_INVENTORY_NOT:
		{
			return Handler_EGS_NEW_PRESENT_CASH_INVENTORY_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_PRESENT_MESSAGE_TO_RECEIVER_NOT:
		{
			return Handler_EGS_PRESENT_MESSAGE_TO_RECEIVER_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;


	//case EGS_GET_POST_LETTER_LIST_ACK:
	//	{
	//		return Handler_EGS_GET_POST_LETTER_LIST_ACK( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	case EGS_GET_RANKING_INFO_ACK:
		{
			return Handler_EGS_GET_RANKING_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_OPTION_UPDATE_ACK:
		{
			return Handler_EGS_OPTION_UPDATE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_STATE_CHANGE_FIELD_ACK:
		if(g_pMain->GetNowStateID() != CX2Main::XS_VILLAGE_MAP)
			return Handler_EGS_STATE_CHANGE_FIELD_ACK(  hWnd, uMsg, wParam, lParam  );
		break;

	case EGS_SEARCH_UNIT_ACK:
		{
			return Handler_EGS_SEARCH_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;
	case EGS_GET_CONNECTION_UNIT_INFO_ACK:
		{
			return Handler_EGS_GET_CONNECTION_UNIT_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifdef SERV_LOCAL_RANKING_SYSTEM
	case EGS_LOCAL_RANKING_WATCH_UNIT_ACK:
		{
			return Handler_EGS_LOCAL_RANKING_WATCH_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef CHANGE_CHANNEL
	case EGS_CHECK_CHANNEL_CHANGE_ACK:
		{
			return Handler_EGS_CHECK_CHANNEL_CHANGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK:
		{
			return Handler_EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CONNECT_CHANNEL_CHANGE_ACK:
		{
			return Handler_EGS_CONNECT_CHANNEL_CHANGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_GET_CHANNEL_LIST_ACK:
		{
			return Handler_EGS_GET_CHANNEL_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK:
		{
			return Handler_EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif

	case EGS_DELETE_NICK_NAME_SUCCESS_NOT:
		{
			return Handler_EGS_DELETE_NICK_NAME_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;

		//{{ 2009. 7. 29  ������	��ŷ ��� �˾�
	case EGS_NEW_RECORD_RANKING_SUCCESS_NOT:
		{
            return Handler_EGS_NEW_RECORD_RANKING_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;
		//}}


	case EGS_CHANGE_JOB_CASH_ITEM_NOT:
		{
			return Handler_EGS_CHANGE_JOB_CASH_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CHANGE_USER_INFO_NOT:
		{
			return Handler_EGS_CHANGE_USER_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	/// �������� ��� ������ broadcast ���ִ� ��Ŷ
	case EGS_CHANGE_MY_UNIT_INFO_NOT:
		{
			return Handler_EGS_CHANGE_MY_UNIT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UNIT_CLASS_CHANGE_ITEM
	case EGS_BUY_UNIT_CLASS_CHANGE_NOT:
		{
			return Handler_EGS_UNIT_CLASS_CHANGE_CASH_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		}
#endif

		//{{ ����� : [2009/10/14] //	�ڵ����� ��Ȱ��
#ifdef AUTO_PAYMENT
	case EGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT:
		{
			return Handler_EGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif	//	AUTO_PAYMENT
		//}} ����� : [2009/10/14] //	�ڵ����� ��Ȱ��

#ifdef DEFEND_TRAFFIC_ATTACK
	case EGS_TRAFFIC_ATTACKER_NOT:
		{
			return Handler_EGS_TRAFFIC_ATTACKER_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif DEFEND_TRAFFIC_ATTACK

#ifdef GUILD_SKILL
		//{{ oasis907 : ����� [2009.11.19] //
	case EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT:
		{
			return Handler_EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
		//}} oasis907 : ����� [2009.11.19] //	

		//{{ oasis907 : ����� [2009.11.30] // ��彺ų ����Ʈ ġƮ
	case EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK:
		{
			return Handler_EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK( hWnd, uMsg, wParam, lParam );
		} break;
		//}} oasis907 : ����� [2009.11.30] //
#endif GUILD_SKILL

		//{{ kimhc // 2009-12-05 // ���θ��� �̺�Ʈ �ð� �ڵ鷯
#ifdef	CHRISTMAS_TREE
	case EGS_TIME_EVENT_INFO_NOT:
		{
			return Handler_EGS_TIME_EVENT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-05 // ���θ��� �̺�Ʈ �ð� �ڵ鷯

#ifdef SERV_ADVERTISEMENT_EVENT
	case EGS_ADVERTISEMENT_EVENT_INFO_NOT:
		{
			return Handler_EGS_ADVERTISEMENT_EVENT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef WEB_POINT_EVENT // oasis907 : ����� [2010.2.24] // 
	case EGS_GET_WEB_POINT_NOT:
		{
			return Handler_EGS_GET_WEB_POINT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif WEB_POINT_EVENT

#ifdef PC_BANG_SANG
	// oasis907 : ����� [2010.1.29] // 
	case EGS_PC_BANG_AUTH_RESULT_NOT:
		{
			return Handler_EGS_PC_BANG_AUTH_RESULT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif PC_BANG_SANG

	//{{ oasis907 : ����� [2010.5.6] // 
	case EGS_ERROR_POPUP_NOT:
		{
			return Handler_EGS_ERROR_POPUP_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
	//}}
		//{{ kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#ifdef	SERV_DAY_QUEST
	case EGS_EVENT_QUEST_INFO_NOT:
		{
			return Handler_EGS_EVENT_QUEST_INFO_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#ifdef SERV_CHECK_TIME_QUEST
	case EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT:
		{
			return Handler_EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif // SERV_CHECK_TIME_QUEST
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����

#if defined( SERV_HACKING_TOOL_LIST )
#if defined( _SERVICE_ ) 
		ELSWORD_VIRTUALIZER_START
#endif
	case EGS_HACKING_TOOL_LIST_NOT:
		{
			return Handler_EGS_HACKING_TOOL_LIST_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;		
#if defined( _SERVICE_ ) 
		ELSWORD_VIRTUALIZER_END
#endif

#endif
#ifdef SERV_SKILL_NOTE
	case EGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT:
		{
			return Handler_EGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_REG_SKILL_NOTE_MEMO_ACK:
		{
			return Handler_EGS_REG_SKILL_NOTE_MEMO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif

#if defined(SERV_PROTECT_AUTO_HACK) && defined(SERV_SERVER_CHECK_HACK_USER_REQ_ACK)
	case EGS_SERVER_CHECK_HACK_USER_REQ:
		{
			return Handler_EGS_SERVER_CHECK_HACK_USER_REQ( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CHECK_HACK_USER_PACKET_REQUEST_NOT:
		{
#ifdef ADD_COLLECT_CLIENT_INFO
			return g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_56, "", false, false );
#endif
		}
		break;
#endif // SERV_PROTECT_AUTO_HACK

			//{{ kimhc // 2010-08-16 // ������ �ϸ� �Ⱓ�� �þ�� �̺�Ʈ ���� ������ �ý����� ������ �� �Դϴ�.
#ifdef	SERV_RESET_PERIOD_EVENT
	case EGS_RESET_PERIOD_ITEM_NOT:
		{
			return Handler_EGS_RESET_PERIOD_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif	SERV_RESET_PERIOD_EVENT
		//}} kimhc // 2010-08-16 // ������ �ϸ� �Ⱓ�� �þ�� �̺�Ʈ ���� ������ �ý����� ������ �� �Դϴ�.
#ifdef SERV_ADMIN_SHOW_ED
	case EGS_ADMIN_SHOW_ED_CHEAT_ACK:
		{
			return Handler_EGS_ADMIN_SHOW_ED_CHEAT_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif
#ifdef SERV_PSHOP_AGENCY
	case EGS_PSHOP_AGENCY_MESSAGE_NOT:
		{
			return Handler_EGS_PSHOP_AGENCY_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT:
		{
			return Handler_EGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif
#ifdef NEW_HENIR_TEST
	case EGS_HENIR_REWARD_COUNT_NOT:
		{
			return Handler_EGS_HENIR_REWARD_COUNT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif NEW_HENIR_TEST
#ifdef SERV_RANDOM_DAY_QUEST
	case EGS_CHANGE_RANDOM_QUEST_NOT:
		{
			return Handler_EGS_CHANGE_RANDOM_QUEST_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_RANDOM_DAY_QUEST
	case EGS_UPDATE_UNIT_INFO_NOT:
		{
			return Handler_EGS_UPDATE_UNIT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#if defined(SERV_MACHINE_ID_BLOCK) && defined(MACHINE_ID)
	case EGS_CHECK_MACHINE_ID_ACK:
		{
			return Handler_EGS_CHECK_MACHINE_ID_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	ELSWORD_VIRTUALIZER_START
	case ECH_HACKING_TOOL_LIST_REQ:
		{
			return Handler_ECH_HACKING_TOOL_LIST_REQ( hWnd, uMsg, wParam, lParam );
		}
		break;		
		ELSWORD_VIRTUALIZER_END
#endif
#ifdef SERV_SHUTDOWN_SYSTEM_AUTO_CHECK
	case EGS_SHUT_DOWN_MESSAGE_NOT:
		{
			return Handler_EGS_SHUT_DOWN_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif

#ifdef BUFF_TEMPLET_SYSTEM	
#ifdef COME_BACK_REWARD
		if ( NULL !=g_pData->GetPremiumBuffManager() )
		{
			if ( true == g_pData->GetPremiumBuffManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
				return true;
		}
#endif
#endif BUFF_TEMPLET_SYSTEM

	case EGS_USER_UNIT_DIE_ACK:
		return Handler_EGS_USER_UNIT_DIE_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_USER_UNIT_DIE_NOT:
		return Handler_EGS_USER_UNIT_DIE_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_USER_UNIT_DIE_COMPLETE_NOT:
		return Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK:
		return Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK(  hWnd, uMsg, wParam, lParam );
		break;

	case EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT:
		return Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT(  hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STOP_DUNGEON_CONTINUE_TIME_ACK:
		return Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_END_GAME_NOT:
		return Handler_EGS_END_GAME_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_JOIN_BATTLE_FIELD_ACK:
		return Handler_EGS_JOIN_BATTLE_FIELD_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_SERVER_SELECT_ACK:
		return  Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_JOIN_BATTLE_FIELD_NOT:
		{
			/*if ( NULL == g_pX2Game || CX2Game::GT_BATTLE_FIELD != g_pX2Game->GetGameType() )*/
			return Handler_EGS_JOIN_BATTLE_FIELD_NOT( hWnd, uMsg, wParam, lParam );

		} break;
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	case EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT:
		{		// ����� �� ����
			return Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT ( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK :
	{
		return Handler_EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK ( hWnd, uMsg, wParam, lParam );
	}
	break;


	case EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ:
		{
			return Handler_EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ ( hWnd, uMsg, wParam, lParam );
		} break;
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
	case EGS_GAME_START_TIME_COUNT_NOT:
		{
			return Handler_EGS_GAME_START_TIME_COUNT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_UPDATE_PARTY_MEMBER_STATUS_NOT:
		{
			return Handler_EGS_UPDATE_PARTY_MEMBER_STATUS_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	case EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT:
		{
			return Handler_EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_BATTLE_FIELD_DANGEROUS_CHEAT


	// {{ ����ȯ // 2012-06-25 // ������ �˴ٿ��� �ȳ���
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	case EGS_BEFORE_SHUT_DOWN_MESSAGE_NOT:
		{
			return Handler_EGS_BEFORE_SHUT_DOWN_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	// }} ����ȯ // 2012-06-25 // ������ �˴ٿ��� �ȳ���

#ifdef SERV_EVENT_MONEY
		case EGS_UPDATE_EVENT_MONEY_NOT:
		{
			return Handler_EGS_UPDATE_EVENT_MONEY_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_EVENT_MONEY

		//{{ 2012. 07. 09	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	case ECH_WAIT_QUEUE_MESSAGE_NOT:
		{
            return Handler_ECH_WAIT_QUEUE_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
	case EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK:
		{
			return Handler_EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_ADMIN_CHEAT_GET_ALL_SKILL
		//}}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

	case EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK:
		{
			return Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ADMIN_GET_SKILL_ACK:
		{
			return Handler_EGS_ADMIN_GET_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

#endif // UPGRADE_SKILL_SYSTEM_2013

		//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	case EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT:
		{
			return Handler_EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT(hWnd, uMsg, wParam, lParam);
		}
		break;

		//{{ 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
	case EGS_FORCE_RELAY_NOT:
		{
			return Handler_EGS_FORCE_RELAY_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	case EGS_UDP_KICK_GAMEEDIT_NOT:
		{
			return Handler_EGS_UDP_KICK_GAMEEDIT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //UDP_CAN_NOT_SEND_USER_KICK

#ifdef UDP_DOWNLOAD_BLOCK_CHECK
	case EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT:
		{
			return Handler_EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT(hWnd, uMsg, wParam, lParam);
		}
		break;
#endif //UDP_DOWNLOAD_BLOCK_CHECK
		
		//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
	case EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT:
		return Handler_EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT(hWnd, uMsg, wParam, lParam);
		break;
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�

	case EGS_GET_ITEM_TEMP_INVENTORY_NOT:
		{
			return Handler_EGS_GET_ITEM_TEMP_INVENTORY_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef SERV_HERO_PVP_MANAGE_LIST
#ifdef ADD_HERO_MATCH_NOTICE
	/// ���� ���� Ư�� ���� �׷� ���� �Լ�
	//case EGS_ADMIN_REMOVE_HERO_PVP_USER_ACK:
	//	{
	//		return Handler_EGS_ADMIN_REMOVE_HERO_PVP_USER_ACK( hWnd, uMsg, wParam, lParam );
	//	} break;

	/// ���� ���� ��� ���� ���
	case EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK:
		{
			return Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_ADMIN_NOTIFY_HERO_PVP_USER_NOT:
		{
			return Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif ADD_HERO_MATCH_NOTICE
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	case EGS_TCP_PING:
		{
			return Handler_EGS_TCP_PING( hWnd, uMsg, wParam, lParam );
		}break;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	case EGS_GET_ACTIVE_LAGCHECK_ACK:
		{
			return Handler_EGS_GET_ACTIVE_LAGCHECK_ACK( hWnd, uMsg, wParam, lParam );
		}break;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	case EGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT:
		{
			return Handler_EGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_NEW_UNIT_TRADE_LIMIT

		//{{ ���� - ���� ���� ����Ʈ, ĳ�� �۾� 
#ifdef SERV_SHARING_BANK_QUEST_CASH
	case EGS_SHARING_BACK_OPEN_NOT:
		{
			return Handler_EGS_SHARING_BACK_OPEN_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif
		//}}
#ifdef SERV_NEW_EVENT_TYPES
	case EGS_UPDATE_MAX_LEVEL_NOT:
		{
			Handler_EGS_UPDATE_MAX_LEVEL_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
	case EGS_UPDATE_CODE_EVENT_NOT:
		{
			Handler_EGS_UPDATE_CODE_EVENT_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_CODE_EVENT

#ifdef _IN_HOUSE_
#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
	case EGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK:
		{
			Handler_EGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
#endif //_IN_HOUSE_

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-11
	case EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT:
		{
			return Handler_EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
	case EGS_CALL_MY_LOVER_AGREE_RESULT_NOT:		/// ��ȯ ��û ��� ��Ŷ
		{
			return Handler_EGS_CALL_MY_LOVER_AGREE_RESULT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CALL_MY_LOVER_ACK:						/// ����ڷκ��� ��ȯ ��û�� ���� ��Ŷ
		{
			return Handler_EGS_CALL_MY_LOVER_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CALL_MY_LOVER_VILLAGE_NOT:				/// ����ڿ��� ��ȯ ���ο� ���� ������ ������ ��Ŷ ( ������ ��ȯ ��û )
		{
			return Handler_EGS_CALL_MY_LOVER_VILLAGE_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CALL_MY_LOVER_ROOM_NOT:				/// ����ڿ��� ��ȯ ���ο� ���� ������ ������ ��Ŷ	( �ʵ�� ��ȯ ��û )
		{
			return Handler_EGS_CALL_MY_LOVER_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT:	/// ��ȯ ����� ��Ⱥ ��û�ڰ� �ִ� ������ ������ ��Ŷ
		{
			return Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT:	/// ��ȯ ����� ��Ⱥ ��û�ڰ� �ִ� �ʵ�� ������ ��Ŷ
		{
			return Handler_EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_CHANGE_COUPLE_DATE_ACK :
		{
			return Handler_EGS_ADMIN_CHANGE_COUPLE_DATE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_JOIN_WEDDING_HALL_NOT:
		{
			return Handler_EGS_JOIN_WEDDING_HALL_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
	case EGS_FIELD_WORKINGS_BLOCK_LOG_REQ:
		{
			Send_EGS_FIELD_WORKINGS_BLOCK_LOG_ACK();
			return true;
		} break;
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_ACCOUNT_BLOCK_USER_POPUP_TEMP
	case EGS_ACCOUNT_BLOCK_USER_POPUP_NOT:
		{
			Handler_EGS_ACCOUNT_BLOCK_USER_POPUP_NOT( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
#endif // SERV_ACCOUNT_BLOCK_USER_POPUP_TEMP

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	case EGS_JUMPING_CHARACTER_ACK :
		{
			// ���� ���� �� ACK
			return Handler_EGS_JUMPING_CHARACTER_ACK( hWnd, uMsg, wParam, lParam );
		}	
		break;

	case EGS_JUMPING_CHARACTER_NOT :
		{
			// �ش� ĳ���ʹ� ���� �̺�Ʈ�� �� �� �ִ� ĳ���� �Դϴ�.
			g_pMain->SetIsJumpingCharacter( true );
			return true;			
		}
		break;

#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef SERV_RELATIONSHIP_EVENT_INT
	case EGS_DIVORCE_NOT :
		{
			return Handler_EGS_DIVORCE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON
	case EGS_SHOW_DISCONNECT_REASON_NOT :
		{
			return Handler_EGS_SHOW_DISCONNECT_REASON_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON

#ifdef SERV_RECRUIT_EVENT_BASE
	case EGS_REGISTER_RECRUITER_NOT:
		{
			return Handler_EGS_REGISTER_RECRUITER_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
	case EGS_GET_EVENT_INFO_ACK:
		{
			return Handler_EGS_GET_EVENT_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	case ESG_REWARD_DB_DATA_NOT:
		{
			Handler_ESG_REWARD_DB_DATA_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_ELESIS_UPDATE_EVENT
	case EGS_EVENT_NOTE_VIEW_ACK:
		{
			return Handler_EGS_EVENT_NOTE_VIEW_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	case EGS_EVENT_CHUNG_GIVE_ITEM_NOT:
		{
			return Handler_EGS_EVENT_CHUNG_GIVE_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	case EGS_EVENT_COBO_DUNGEON_FIELD_NOT :
		{
			return Handler_EGS_EVENT_COBO_DUNGEON_FIELD_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT :
		{
			return Handler_EGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT :
		{
			return Handler_EGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	case EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT :
		{
			return Handler_EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	}

	return false;
}

bool CX2State::UIServerTimeOutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	std::wstring packetExplain = KEvent::GetIDStr( (const unsigned short)wParam );

#ifndef _SERVICE_
	{
		packetExplain += L"(";
		WCHAR wszNum[128] = L"";
		swprintf( wszNum, sizeof(wszNum)/sizeof(WCHAR)-1, L"%d", (int) wParam );
		packetExplain += wszNum;
		packetExplain += L")";
	}
#endif // _SERVICE_

#ifdef _OPEN_TEST_
	{
		packetExplain += L"(";
		WCHAR wszNum[128] = L"";
		swprintf( wszNum, sizeof(wszNum)/sizeof(WCHAR)-1, L"%d", (int) wParam );
		packetExplain += wszNum;
		packetExplain += L")";
	}
#endif //_OPEN_TEST_

	packetExplain += L"\n";
	packetExplain += GET_STRING( STR_ID_539 );

#ifdef LOG_SERVER_TIME_OUT
// 2013-11-04 ������ // �αװ� �ʹ� ���� ���Ƽ� ����
// 2014-01-15 ����ö // �αװ� ��ŷ ������ ���ؼ� ���� ������ Ȯ�� �ϱ� ���� �׽�Ʈ�� Ȱ��ȭ
// 	char Buf1[256];
// 	StringCchPrintfA( Buf1, 256, "Test ServerTimeOut,Event ID = %d ", static_cast<int>(wParam) );
// 	CX2MailLogManager::GetInstance()->AddMailLog( CX2MailLogManager::MLI_SERVER_TIME_OUT, Buf1 );
#endif // LOG_SERVER_TIME_OUT

#ifdef REFORM_ENTRY_POINT
	#ifndef REMOVE_PACKET_TIME_OUT_POPUP
	m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), packetExplain.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
	#endif // REMOVE_PACKET_TIME_OUT_POPUP
#else //REFORM_ENTRY_POINT
	m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), packetExplain.c_str(), this );
#endif //REFORM_ENTRY_POINT

	//m_bFailToConnectServer = true;

	return true;
}

bool CX2State::UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

//bool CX2State::TCPRelayEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	int size = (int)wParam;
//	char* pBuffer = (char*)lParam;
//
//	KEVENT_X2_SERVER_BROADCAST_RELAY kEVENT_X2_SERVER_BROADCAST_RELAY;
//
//	kEVENT_X2_SERVER_BROADCAST_RELAY.size = size;
//	memcpy( kEVENT_X2_SERVER_BROADCAST_RELAY.buffer, pBuffer, sizeof(char) * size );
//
//	g_pData->GetServerProtocol()->SendPacket( EVENT_X2_SERVER_BROADCAST_RELAY, kEVENT_X2_SERVER_BROADCAST_RELAY/*, false, true*/ );
//
//	return true;
//}

void CX2State::LuaFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if ( m_LuaFrameMoveFuncName.empty() == false )
	{
		RegisterLuaBind();

		//string funcName;
		//ConvertWCHARToChar( funcName, m_LuaFrameMoveFuncName.c_str() );

		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_LuaFrameMoveFuncName.c_str(), fTime, fElapsedTime );
	}
}

//void CX2State::LuaFuncCall( const WCHAR* pFuncName )
//{
//	RegisterLuaBind();
//
//	string funcName;
//	ConvertWCHARToChar( funcName, pFuncName );
//
//	lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  funcName.c_str() );
//}

bool CX2State::RegisterLuaBind()
{
	KTDXPROFILE();

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pKTDXApp",		g_pKTDXApp );	
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pMain",		g_pMain );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pData",		g_pData );

	return true;
}

bool CX2State::Handler_EGS_KNPROTECT_AUTH_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KNPROTECT_AUTH_CHECK_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	GG_AUTH_DATA sAuthData;
	sAuthData.dwIndex		= kEvent.dwIndex;
	sAuthData.dwValue1		= kEvent.dwValue1;
	sAuthData.dwValue2		= kEvent.dwValue2;
	sAuthData.dwValue3		= kEvent.dwValue3;

	KNP_AUTH2( &sAuthData );


#ifdef CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
	g_pMain->SetReceived_EGS_KNPROTECT_AUTH_CHECK_NOT( true );
#endif CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT


	return true;
}

bool CX2State::Handler_EGS_KNPROTECT_USER_SHOWDOWN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	//�翬�� ������ ���� ��ŷ������ Ŭ���̾�Ʈ�� ���� ��Ų��.
	g_pMain->IsValidPacket( kEvent.m_iOK );

	g_pKTDXApp->SetFindHacking( true, GET_STRING( STR_ID_540 ) );

	return true;
}

#ifdef SERV_NPROTECT_CS_AUTH_30
bool CX2State::Handler_EGS_NPROTECT_CSAUTH3_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NPROTECT_CSAUTH3_CHECK_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	UINT32 uReceivedSize = kEvent.m_deqCSAuth3.size();
	BYTE* byteCSAuth3 = new BYTE[uReceivedSize];
	for( UINT32 i = 0; i < uReceivedSize; i++)
	{
		byteCSAuth3[i] = kEvent.m_deqCSAuth3.front();
		kEvent.m_deqCSAuth3.pop_front();
	}

	KNP_AUTH3( byteCSAuth3, uReceivedSize );

	delete [] byteCSAuth3;

	return true;
}	
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef CLIENT_USE_XTRAP	// XTRAP Ŭ���̾�Ʈ - ���� ��Ŷ ó���ϰ� �ٽ� ������ �κ�
bool CX2State::Handler_EGS_XTRAP_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_XTRAP_REQ packet_in;
	DeSerialize( pBuff, &packet_in );

	KEGS_XTRAP_ACK packet_out;
	packet_out.m_vecData.resize(200);

	unsigned char arrParam[200];
	unsigned char arrResult[200];

	for( u_int ui = 0; ui < packet_in.m_vecData.size(); ++ui )
	{
		arrParam[ui] = packet_in.m_vecData[ui];
	}

	XTrap_CS_Step2(
		(arrParam),
		(arrResult),
		XTRAP_PROTECT_PE | XTRAP_PROTECT_TEXT | XTRAP_PROTECT_EXCEPT_VIRUS);

	for( u_int ui = 0; ui < 200; ++ui )
	{
		packet_out.m_vecData[ui] = arrResult[ui];
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_XTRAP_ACK, packet_out );

	return true;
}
#endif CLIENT_USE_XTRAP

bool CX2State::Handler_EGS_NOTIFY_MSG_REQ( int noticeCount, const WCHAR* pMsg, bool bIsAllServers /*= false*/ )
{
	if ( pMsg == NULL )
		return false;

#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return false;
	}
#endif LIGHT_OPERATOR_ACCOUNT

	KEGS_NOTIFY_MSG_REQ kPacket;
	kPacket.m_Count = noticeCount;
	kPacket.m_wstrMSG = pMsg;


	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( bIsAllServers )
	{
		kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_ALL_SERVERS;
	}
	else
#endif SERV_INTEGRATION
	{
		kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_SERVER_GROUP;
	}
	//}}

	if ( kPacket.m_wstrMSG.empty() == true )
		return false;


	if ( g_pData->GetServerProtocol() != NULL && 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() != NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		g_pData->GetServerProtocol()->SendPacket( EGS_NOTIFY_MSG_REQ, kPacket );
		

	return true;
}

bool CX2State::Handler_EGS_NOTIFY_MSG_REQ_LUA( int noticeCount, const char* pMsg, bool bIsAllServers /*= false*/ )
{
	wstring noticeMsg;
	//ConvertUtf8ToWCHAR( noticeMsg, pMsg );
#ifdef CHEAT_WCHART_TO_UTF8
	ConvertUtf8ToWCHAR( noticeMsg, pMsg );
#else //CHEAT_WCHART_TO_UTF8
	ConvertCharToWCHAR( noticeMsg, pMsg );
#endif //CHEAT_WCHART_TO_UTF8
	
	

	return Handler_EGS_NOTIFY_MSG_REQ( noticeCount, noticeMsg.c_str(), bIsAllServers );
	
}

bool CX2State::Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ_LUA( const char* pUnitNickName, int changeLevel, int iChangeEXP /*= 0*/ )
{
	wstring unitNickName;
#ifdef CHEAT_WCHART_TO_UTF8
	ConvertUtf8ToWCHAR( unitNickName, pUnitNickName );
#else //CHEAT_WCHART_TO_UTF8
	ConvertCharToWCHAR( unitNickName, pUnitNickName );
#endif //CHEAT_WCHART_TO_UTF8

	return Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ( unitNickName.c_str(), changeLevel, iChangeEXP );
}

bool CX2State::Handler_EGS_ADMIN_CHANGE_ED_REQ_LUA( const char* pUnitNickName, int changeED )
{
	wstring unitNickName;
#ifdef CHEAT_WCHART_TO_UTF8
	ConvertUtf8ToWCHAR( unitNickName, pUnitNickName );
#else //CHEAT_WCHART_TO_UTF8
	ConvertCharToWCHAR( unitNickName, pUnitNickName );
#endif //CHEAT_WCHART_TO_UTF8

	return Handler_EGS_ADMIN_CHANGE_ED_REQ( unitNickName.c_str(), changeED );
}

bool CX2State::Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NOTIFY_MSG_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 6. 23  ������	���� Ÿ��
	switch( kEvent.m_cNotifyType )
	{
	case KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM:
	case KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM:
		{
			int lastPos = kEvent.m_wstrMSG.find( L";" );
			std::wstring wstrNickName = kEvent.m_wstrMSG.substr( 0, lastPos );
			//{{ 2011.11.20 ��ȫ��	���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
			std::wstring wstrItemID = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );

			int EnchantPos = wstrItemID.find( L";" );
			std::wstring wstrEnchantInfo = L"";
			if( EnchantPos != -1 )
			{
				wstrEnchantInfo = wstrItemID.substr( 0, EnchantPos );
				wstrItemID = wstrItemID.substr( EnchantPos + 1, wstrItemID.size() );	
			}

			int iItemID = _wtoi(wstrItemID.c_str());
			const CX2Item::ItemTemplet* pkTemplet = g_pData->GetItemManager()->GetItemTemplet(iItemID);
			std::wstring wstrItemName = boost::str( boost::wformat( L"%s %s" ) % wstrEnchantInfo % pkTemplet->GetFullName_() );
			
#else SERV_UNITED_SERVER_EU
			std::wstring wstrItemName = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );
#endif SERV_UNITED_SERVER_EU
			//}}

#ifdef MODIFY_NOTICE_GET_ITEM
			bool bHasCubeName = false;
			if( 0 != kEvent.m_iRandomItem )
			{
	#ifdef SERV_EVENT_BINGO
				if( 257280 == kEvent.m_iRandomItem)
				{
					wstring wstrCubeName = GET_STRING( STR_ID_19188 );
					kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_18342, "LLL", wstrNickName, wstrCubeName, wstrItemName ) );
					bHasCubeName = true;
				}
				else
	#endif //SERV_EVENT_BINGO
				if( NULL != g_pData && NULL != g_pData->GetItemManager() )
				{
					const CX2Item::ItemTemplet* pItemTemplete = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_iRandomItem );
					if( NULL != pItemTemplete )
					{
						kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_18342, "LSL", wstrNickName, pItemTemplete->GetFullName_(), wstrItemName ) );
						bHasCubeName = true;
					}
				}
			}

			if( false == bHasCubeName )
			{
				kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_3691, "LL", wstrNickName, wstrItemName ) );
			}
#else
			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_3691, "LL", wstrNickName, wstrItemName ) );
#endif //MODIFY_NOTICE_GET_ITEM
		}
		break;

	case KEGS_NOTIFY_MSG_NOT::NT_MAX_LEVEL_UP:
		{
			int lastPos = kEvent.m_wstrMSG.find( L";" );
			std::wstring wstrNickName = kEvent.m_wstrMSG.substr( 0, lastPos );
			std::wstring wstrMaxLevel = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );

			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_3710, "LL", wstrNickName, wstrMaxLevel ) );
		}
		break;

	case KEGS_NOTIFY_MSG_NOT::NT_ENCHANT_SUCCESS:
		{
			int lastPos = kEvent.m_wstrMSG.find( L";" );
			std::wstring wstrNickName = kEvent.m_wstrMSG.substr( 0, lastPos );
			std::wstring wstrRemain = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );

			lastPos = wstrRemain.find( L";" );
			//{{ 2011.11.20 ��ȫ��	���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
			std::wstring wstrItemID = wstrRemain.substr( 0, lastPos );
			int iItemID = _wtoi(wstrItemID.c_str());
			const CX2Item::ItemTemplet* pkTemplet = g_pData->GetItemManager()->GetItemTemplet(iItemID);
			std::wstring wstrItemName = pkTemplet->GetFullName_();
#else SERV_UNITED_SERVER_EU
			std::wstring wstrItemName = wstrRemain.substr( 0, lastPos );
#endif SERV_UNITED_SERVER_EU
			//}}
			std::wstring wstrEnchantLv = wstrRemain.substr( lastPos + 1, wstrRemain.size() );


			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_3711, "LLL", wstrNickName, wstrItemName, wstrEnchantLv ) );
		}
		break;
		//{{ 2009. 7. 27  ������	��ϸ� �ð� Ŭ���� ����
	case KEGS_NOTIFY_MSG_NOT::NT_HENIR_CLEAR:
		{
			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_3792, "L", kEvent.m_wstrMSG ) );
		}
		break;
		//}}

		//{{ 2009. 11. 2  ������	���â�� ����
	case KEGS_NOTIFY_MSG_NOT::NT_GUILD_CREATION:
		{
			int lastPos = kEvent.m_wstrMSG.find( L";" );
			std::wstring wstrNickName = kEvent.m_wstrMSG.substr( 0, lastPos );
			std::wstring wstrGuildName = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );

			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_4726, "LL", wstrNickName, wstrGuildName ) );
		}
		break;

	case KEGS_NOTIFY_MSG_NOT::NT_GUILD_LEVEL_UP:
		{
			int lastPos = kEvent.m_wstrMSG.find( L";" );
			std::wstring wstrGuildName = kEvent.m_wstrMSG.substr( 0, lastPos );
			std::wstring wstrGuildLevel = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );

			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_4748, "LL", wstrGuildName, wstrGuildLevel ) );
		}
		break;
		//}}
#ifdef SERV_GUILD_CHANGE_NAME
	case KEGS_NOTIFY_MSG_NOT::NT_GUILD_NAME_CHANGED:
		{
			int lastPos = kEvent.m_wstrMSG.find( L";" );
			std::wstring wstrPreGuildName = kEvent.m_wstrMSG.substr( 0, lastPos );
			std::wstring wstrGuildName = kEvent.m_wstrMSG.substr( lastPos + 1, kEvent.m_wstrMSG.size() );

			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_16192, "LL", wstrPreGuildName, wstrGuildName ) );
		}
		break;
#endif //CHANGE_GUILD_NAME

#ifdef SERV_RELATIONSHIP_SYSTEM
	case KEGS_NOTIFY_MSG_NOT::NT_WEDDING_SUCCESS:
		{
			kEvent.m_wstrMSG = GET_REPLACED_STRING( ( STR_ID_24670, "L", kEvent.m_wstrMSG ) );
		}
		break;
#endif //SERV_RELATIONSHIP_SYSTEM

	default:
		break;
	}

	//}}
	//////////////////////////////////////////////////////////////////////////

	wstring wstrNotice = kEvent.m_wstrMSG;
	//wstrNotice += kEvent.m_wstrMSG;

	if( false == g_pMain->GetNoticeManager()->AddNotice( (KEGS_NOTIFY_MSG_REQ::NOTIFY_TYPE)kEvent.m_cNotifyType, wstrNotice.c_str(), (int)kEvent.m_Count ) )
		return true;

	D3DXCOLOR _color = D3DXCOLOR( 1,1,0,1 );

	//{{ 2010. 05. 10  ������	�������� ������ ����
	if( kEvent.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS ||
		kEvent.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_SERVER_GROUP 	)
	//}}
	{
		wstring tempNotice = L"* ";
		tempNotice += wstrNotice;
		if ( g_pChatBox != NULL )
		{
#ifdef CHAT_LOG_TO_TXT
			// ����
			g_pChatBox->AddChatLogToFile( tempNotice.c_str() );
#endif // CHAT_LOG_TO_TXT
			g_pChatBox->AddChatLog( tempNotice.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, _color, L"#CFFFF00" );
		}
	}

#ifdef SERV_SHUTDOWN_SYSTEM
	if( kEvent.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
	{
		wstring tempNotice = L"";
		tempNotice += wstrNotice;
		if ( g_pChatBox != NULL )
			g_pChatBox->AddChatLog( tempNotice.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, _color, L"#CFF5A00" );
	}
#endif

	//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrNotice.c_str(), this );

//{{ mauntain : ����ȯ [2012.05.08] ����� �� ���̵� ���� �� �ȳ� ���� ���
#ifdef NEW_DEFENCE_DUNGEON
	if( kEvent.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_THE_GATE_OF_DARKNESS )
	{
		if( NULL != g_pData && NULL != g_pData->GetWorldMissionManager() && 
			true == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
		{
#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
			if( NULL != g_pMain && 
				( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP || 
				  g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) )	//���� ��  �ʵ��� ��
#else  FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
			if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )	//������ ��
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
			{
				const int iNowDefenceDungeonLevel =  g_pData->GetWorldMissionManager()->GetNowDefenceDungeonLevel();

#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
				//ȭ�� �߾ӿ� ǥ�õǴ� ���̵� ���� �˸� �ؽ�Ʈ�� ���������� �����Ѵ�.
				CX2StateField*	pStateField	=	NULL;
				pStateField	= static_cast< CX2StateField* >( g_pMain->GetNowState() );
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

				switch( iNowDefenceDungeonLevel )
				{
				case CX2Dungeon::DL_NORMAL:
					{
#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
						//ȭ�� �߾ӿ� ǥ�õǴ� �ؽ�Ʈ�� ĳ�ü��� �� ���� ��, �������� �ʴ´�.
						if( ( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() ) )
						{
							SetFadeInOutNotice( GET_STRING( STR_ID_17204 ) );
#else  FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
						//ȭ�� �߾ӿ� ǥ�õǴ� �ؽ�Ʈ�� ĳ�ü��� �� ���� ��, �������� �ʴ´�.
						if( ( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() ) && NULL != pStateField )
						{
							pStateField->SetDefenceDungeonNoticeTime( 5.f );	//ȭ�� �߾ӿ� �����Ǵ� �ؽ�Ʈ ǥ�� �ð�
							pStateField->SetDefenceDungeonNoticeType( 0 );		//�ؽ�Ʈ ����
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
						}

						if( g_pChatBox != NULL )								//ü��â�� ǥ�õǴ� �˸� �ؽ�Ʈ
						{
							g_pChatBox->AddChatLog( GET_STRING(STR_ID_17204), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
							g_pChatBox->AddChatLog( GET_STRING(STR_ID_17205), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
						}
					}break;
				case CX2Dungeon::DL_HARD:
					{
#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
						//ȭ�� �߾ӿ� ǥ�õǴ� �ؽ�Ʈ�� ĳ�ü��� �� ���� ��, �������� �ʴ´�.
						if( ( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() ) )
						{
							SetFadeInOutNotice( GET_STRING( STR_ID_17207 ) );
#else  FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
						//ȭ�� �߾ӿ� ǥ�õǴ� �ؽ�Ʈ�� ĳ�ü��� �� ���� ��, �������� �ʴ´�.
						if( ( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() ) && NULL != pStateField )
						{
							pStateField->SetDefenceDungeonNoticeTime( 5.f );	//ȭ�� �߾ӿ� �����Ǵ� �ؽ�Ʈ ǥ�� �ð�
							pStateField->SetDefenceDungeonNoticeType( 1 );		//�ؽ�Ʈ ����
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
						}

						if( g_pChatBox != NULL )								//ü��â�� ǥ�õǴ� �˸� �ؽ�Ʈ
						{
							g_pChatBox->AddChatLog( GET_STRING(STR_ID_17207), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
							g_pChatBox->AddChatLog( GET_STRING(STR_ID_17206), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
						}
					}break;
				}
			}
			//EGS_PARTY_CHANGE_DUNGEON_REQ �� ȣ���ϵ��� ��ȣ�� �ش�.
			g_pMain->GetPartyUI()->SetChangePartyUI( true );
		}
	}
#endif NEW_DEFENCE_DUNGEON
//}} mauntain : ����ȯ [2012.05.08] ����� �� ���̵� ���� �� �ȳ� ���� ���
	
	return true;
}

bool CX2State::Handler_EGS_ADMIN_SERVER_STATUS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_SERVER_STATUS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
#ifdef SERV_CHANNELING_USER_MANAGER
		g_pMain->SetServerChannelingUserCount( kEvent.m_mapChannelingUserList );
#endif SERV_CHANNELING_USER_MANAGER
		g_pMain->SetServerUserCount( kEvent.m_iCCU );
		g_pMain->SetMapServerPVPRoomCount( kEvent.m_mapNumPvPRoom );
		g_pMain->SetCurPartyCount( kEvent.m_iCurPartyCount );
		g_pMain->SetPlayGamePartyCount( kEvent.m_iPlayGamePartyCount );
	}
	return true;
}


bool CX2State::Handler_EGS_CLIENT_QUIT_REQ()
{
	//KEGS_CLIENT_QUIT_REQ kPacket;
	//kPacket.m_iReason = NetError::ERR_CLIENT_QUIT_00;


	g_pMain->SetQuitType( NetError::ERR_CLIENT_QUIT_00 );
	g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_ESC_GAME );
	g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );

	//g_pData->GetServerProtocol()->SendPacket( EGS_CLIENT_QUIT_REQ, kPacket );
	//g_pMain->AddServerPacket( EGS_CLIENT_QUIT_ACK );

	return true;
}

bool CX2State::Handler_EGS_CLIENT_QUIT_ACK()
{
	if( g_pMain->DeleteServerPacket( EGS_CLIENT_QUIT_ACK ) == true )
	{
		g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_ESC_GAME );
		g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
	}

	return true;
}


bool CX2State::Handler_EGS_CHANGE_OPTION_PLAY_GUIDE_REQ()
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
		return false; 
	}	

	KEGS_CHANGE_OPTION_PLAY_GUIDE_REQ kPacket;
	kPacket.m_bOn = g_pMain->GetGameOption().GetOptionList().m_bPlayGuide;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ, kPacket ); 
	//g_pMain->AddServerPacket( EGS_CHANGE_OPTION_PLAY_GUIDE_ACK, 60.f ); 

	return true;
}

bool CX2State::Handler_EGS_CHANGE_OPTION_PLAY_GUIDE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	//if( g_pMain->DeleteServerPacket( EGS_CHANGE_OPTION_PLAY_GUIDE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2State::Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ( const WCHAR* pUnitNickName, int changeLevel, int iChangeEXP )
{

	KEGS_ADMIN_MODIFY_UNIT_LEVEL_REQ kPacket;


	if ( g_pData->GetServerProtocol() == NULL ||
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
		//g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
		return true;
	}

//	wstring unitNickName;
//	ConvertCharToWCHAR( unitNickName, pUnitNickName );
	kPacket.m_wstrUnitNickName = pUnitNickName;
	kPacket.m_iLevel = changeLevel;
	kPacket.m_iEXP = iChangeEXP;

#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return false;
	}
#endif LIGHT_OPERATOR_ACCOUNT

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK );

	return true;
}

bool CX2State::Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_MODIFY_UNIT_LEVEL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2State::Handler_EGS_ADMIN_CHANGE_ED_REQ( const WCHAR* pUnitNickName, int changeED )
{
	KEGS_ADMIN_CHANGE_ED_REQ kPacket;


	if ( g_pData->GetServerProtocol() == NULL ||
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
		return true;
	}

	if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		return false;

#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return false;
	}
#endif LIGHT_OPERATOR_ACCOUNT

	kPacket.m_wstrUnitNickName = pUnitNickName;
	kPacket.m_nED = changeED;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_CHANGE_ED_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_CHANGE_ED_ACK );

	return true;
}

bool CX2State::Handler_EGS_ADMIN_CHANGE_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_ED_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}


bool CX2State::Handler_EGS_CHECK_INVALID_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_INVALID_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	DialogLog( "\nINVALID_ROOM_NOT\n" );

	bool bCheck = false;

	
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			SEnum::VILLAGE_MAP_ID eVillageID = (SEnum::VILLAGE_MAP_ID) kEvent.m_iMapID;
			CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageID );
			if( NULL != pVillageTemplet )
			{
				int iStartPos = g_pData->GetLocationManager()->GetLoginPos( eVillageID );		
				D3DXVECTOR3 vStartPos = g_pData->GetLocationManager()->GetStartPosLoc( iStartPos );
				g_pData->GetLocationManager()->SetVillage( eVillageID, vStartPos );
				g_pData->GetLocationManager()->SetLastPos( vStartPos );
			}
			
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );

			CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ( kEvent.m_iMapID );

		} break;

	case CX2Main::XS_VILLAGE_MAP:	// ���� �����������
		{
			if( true == g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) )
			{		
				SEnum::VILLAGE_MAP_ID eVillageID = (SEnum::VILLAGE_MAP_ID) kEvent.m_iMapID;
				CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( eVillageID );
				if( NULL != pVillageTemplet )
				{
					int iStartPos = g_pData->GetLocationManager()->GetLoginPos( eVillageID );		
					D3DXVECTOR3 vStartPos = g_pData->GetLocationManager()->GetStartPosLoc( iStartPos );
					g_pData->GetLocationManager()->SetVillage( eVillageID, vStartPos );
					g_pData->GetLocationManager()->SetLastPos( vStartPos );
				}

				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );

				CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ( kEvent.m_iMapID );
			}

		} break;

	default:
		{
			ASSERT( !"invalid state on EGS_CHECK_INVALID_ROOM_NOT" );
			//return true;
		} break;
	}

	g_pMain->SetCenterServerDie( true );

	return true;
}

bool CX2State::HandleMsgByESC()
{
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	// XS_JOIN ������Ʈ���� ESC �ȸ԰�
#ifdef SERV_KEY_MAPPING_INT
	if ( ( g_pMain->GetNowStateID() == CX2Main::XS_JOIN ) && 
		( GET_KEY_STATE(GA_ESCAPE)  == TRUE || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ESCAPE ) == TRUE )
		)
#else // SERV_KEY_MAPPING_INT
	if ( ( g_pMain->GetNowStateID() == CX2Main::XS_JOIN ) && g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ESCAPE ) == TRUE )
#endif // SERV_KEY_MAPPING_INT
	{
		return false;
	}
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER

	//{{ kimhc // Login ���ĺ��� Loading��(SeverSelect���� ESC Ű ��� ���ϰ� ����)
#ifdef SERV_KEY_MAPPING_INT
	if ( ( g_pMain->GetNowStateID() != CX2Main::XS_LOADING ) && ( GET_KEY_STATE(GA_ESCAPE)  == TRUE || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ESCAPE ) == TRUE ) )
#else // SERV_KEY_MAPPING_INT
	if ( ( g_pMain->GetNowStateID() != CX2Main::XS_LOADING ) && g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ESCAPE ) == TRUE )
#endif // SERV_KEY_MAPPING_INT
	{
#ifdef DIALOG_SHOW_TOGGLE
		if( g_pInstanceData!= NULL &&  g_pKTDXApp->GetDGManager() != NULL && g_pKTDXApp->GetDGManager()->GetDialogManager() != NULL )
		{		
			if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
			{
				g_pKTDXApp->GetDGManager()->GetDialogManager()->SetShow(true);

				if ( NULL != g_pInstanceData->GetMiniMapUI() )
				{

#ifdef ELSWORD_NEW_BEGINNING
					if(g_pMain->GetNowStateID() != CX2Main::XS_BEGINNING)
					{
						g_pInstanceData->GetMiniMapUI()->SetShowMiniMap(g_pInstanceData->GetMiniMapUI()->GetCurrentMiniMapType(), true);
					}
#else				
					if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
						g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
						g_pInstanceData->GetMiniMapUI()->SetShowMiniMap(g_pInstanceData->GetMiniMapUI()->GetCurrentMiniMapType(), true);
#endif ELSWORD_NEW_BEGINNING
				}

				return false;
			}			
		}
#endif //DIALOG_SHOW_TOGGLE

		return HandleMsgByESCImp();
	}
	//}} kimhc
	return false;
}

bool CX2State::HandleMsgByESCImp()
{
	if ( g_pChatBox != NULL && g_pChatBox->GetFocus() == true )
	{
		g_pChatBox->HideChatEditBox();
		g_pChatBox->SetChatEnteredBefore( false );
		if ( g_pX2Game != NULL )
			g_pX2Game->SetOpenChatBox( false );
		if(g_pChatBox != NULL)
			g_pChatBox->SetFocusChatEditBox(false);
		return true;
	}


	if ( PutOffPopUpDialog() == true )
		return true;

	LastMsgByESC();

	return false;
}


bool CX2State::PutOffPopUpDialog()
{
	CKTDGUIDialogType pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetCloseDialogByESC();

	if ( pDialog != NULL )
	{
		if( pDialog->GetOKAndCancelMsgBox() == true || wcscmp( pDialog->GetName(), L"DLG_OKMsgBox" ) == 0 )
		{
			if ( pDialog->GetOKAndCancelMsgBox() == true  )
			{
				CKTDGUIButton* pCancelButton = (CKTDGUIButton*)pDialog->GetControl( L"MsgBoxOkAndCancelCancelButton" );

				if ( pCancelButton != NULL && pCancelButton->GetShow() == true && pCancelButton->GetCustomMsgMouseUp() != -1 )
				{
					MsgProc( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, pCancelButton->GetCustomMsgMouseUp(), (LPARAM)pCancelButton);
					return true;
				}
			}

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pDialog, NULL, false );
			return true;
		}

		if ( pDialog->GetCloseCustomUIEventID() != -1 )
		{
			MsgProc( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, pDialog->GetCloseCustomUIEventID(), (LPARAM)this );
			return true;
		}
	}

	return false;
}

bool CX2State::QuitGame()
{
	//{{ kimhc // PVP, ���� ���â������ ESCŰ ó�� ���� // 2009-06-09
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_RESULT:
		break;

	case CX2Main::XS_PVP_RESULT:
		break;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	case CX2Main::XS_SERVER_SELECT:
		m_pMsgBoxQuitGame = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ), GET_STRING( STR_ID_542 ), SUCM_EXIT, this, -1, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
		break;
	case CX2Main::XS_CREATE_UNIT:
		m_pMsgBoxQuitGame = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ), GET_STRING( STR_ID_542 ), SUCM_EXIT, this, -1, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
		break;
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

	default:
		m_pMsgBoxQuitGame = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_542 ), SUCM_EXIT, this );
		break;
	}	
	//}} kimhc // PVP, ���� ���â������ ESCŰ ó�� ���� // 2009-06-09

	return true;
}

#ifdef REFORM_ENTRY_POINT
void CX2State::OpenLastMsgPopUp( const WCHAR* pMsg, wstring wstrCustomLuaFileName /* = L"" */,  wstring wstrPlaySoundFileName /* = L"" */  )
#else
void CX2State::OpenLastMsgPopUp( const WCHAR* pMsg )
#endif // REFORM_ENTRY_POINT
{
	SAFE_DELETE_DIALOG( m_pDLGLastMsgPopUp );

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	if ( wstrCustomLuaFileName != L"" )
	{
		if ( g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT )
			m_pDLGLastMsgPopUp = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 157, 360 ), pMsg, SUCM_LAST_MSG_BY_ESC_OK, this, SUCM_LAST_MSG_BY_ESC_CANCEL, wstrCustomLuaFileName,  D3DXVECTOR2 ( 0, 0 ), wstrPlaySoundFileName );
		else if ( g_pMain->GetNowStateID() == CX2Main::XS_CREATE_UNIT )
			m_pDLGLastMsgPopUp = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ), pMsg, SUCM_LAST_MSG_BY_ESC_OK, this, SUCM_LAST_MSG_BY_ESC_CANCEL, wstrCustomLuaFileName,  D3DXVECTOR2 ( 0, 0 ), wstrPlaySoundFileName );		
		else
			m_pDLGLastMsgPopUp = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), pMsg, SUCM_LAST_MSG_BY_ESC_OK, this, SUCM_LAST_MSG_BY_ESC_CANCEL, wstrCustomLuaFileName );
	}
	else
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	{
		m_pDLGLastMsgPopUp = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), pMsg, SUCM_LAST_MSG_BY_ESC_OK, this, SUCM_LAST_MSG_BY_ESC_CANCEL );
	}
}

bool CX2State::LastMsgByESC()
{
	return QuitGame();
}

void CX2State::LastMsgPopOk()
{
	
}

void CX2State::LastMsgPopCancel()
{
	if ( m_pDLGLastMsgPopUp != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLastMsgPopUp, NULL, false );

	m_pDLGLastMsgPopUp = NULL;
}

bool CX2State::ShortCutKeyProcess()
{
	KTDXPROFILE();


	if ( m_bEnableShortCutKey == false )
		return true;

	//if ( g_pData->GetMessenger() != NULL && g_pData->GetMessenger()->GetOpen() == true )
	//	return false;

#if 0
	if( g_pChatBox != NULL && g_pChatBox->GetFocusChatEditBox() == true )
		return true;
	if( g_pData != NULL && g_pData->GetMessenger()->GetOpenChatBox() == true )
	{
		CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
		if ( pControl != NULL )
		{
			if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
				pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
			{
				return true;
			}
		}
	}	
#else
	CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
	if ( pControl != NULL )
	{
		if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
			pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
		{
			return true;
		}
	}
#endif

#ifdef KEYFRAME_CAMERA
	// oasis907 : ����� [2010.11.21] // ���� ī�޶� ����Ʈ�� Ű �Է� ����
	if( g_pX2Game != NULL && g_pX2Game->GetWorldCameraEdit() == true)
	{
		return true;
	}
#endif KEYFRAME_CAMERA




	//{{ oasis907 : ����� [2009.12.17] // Ʃ�丮���� ��� ���̾�α� ���� KeyProcess �����ϰ�
#ifdef TUTORIAL_TRAINGING_KEYPROCESS_FIX
	// oasis907 : ����� [2010.7.14] // 

	//������//2012-12-14// �系�� �Ʒüҿ��� ����Ű �������� ����
	#ifndef _IN_HOUSE_
	if( g_pMain->GetIsPlayingFreeTraining() == true )
	{
#ifdef ALLOW_CAMERA_ZOOM_BY_TRAINING	/// ���� �Ʒüҿ��� ī�޶� �� ��� ����� �� �ֵ��� ����
		return false;		/// CX2StateMenu::ShortCutKeyProcess() ���� ī�޶� �� ��� �˻��ϱ� ���� ����
#else  ALLOW_CAMERA_ZOOM_BY_TRAINING
		return true;
#endif ALLOW_CAMERA_ZOOM_BY_TRAINING
	}
	#endif //_IN_HOUSE_

#ifndef COMBOTREE_IN_TUTORIAL
	if ( g_pMain->GetIsPlayingTutorial() == true )
		return true;
#endif COMBOTREE_IN_TUTORIAL

#endif TUTORIAL_TRAINGING_KEYPROCESS_FIX

	// �����
	bool bHideDialog = false;

	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		bHideDialog = true;

	if ( false == bHideDialog && GET_KEY_STATE( GA_MAP ) == TRUE ) //DIK_M
	{
		switch(g_pMain->GetNowStateID())
		{
		case CX2Main::XS_SQUARE_GAME:
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
		case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
			{
				CX2WorldMapUI *pWorldMapUI = g_pInstanceData->GetMiniMapUI()->GetWorldMapUI();

				if( pWorldMapUI != NULL )
				{
					if( true == pWorldMapUI->GetShow() )
					{
#ifdef FIX_WORLD_MAP_UI_BUG		/// ������ ����� �� �� �ùٸ� ���� UI ���� �� �ֵ��� ����
						pWorldMapUI->SetZoomed( true );
#endif FIX_WORLD_MAP_UI_BUG
						pWorldMapUI->OpenWorldMap( false );
						pWorldMapUI->OpenFieldMap( false );
						pWorldMapUI->CloseAllToolTip();
						return true;
					}
					else
					{
						if(g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP)
						{
							pWorldMapUI->OpenWorldMap( true );
							pWorldMapUI->UpdateWorldMap();

							int villageID = g_pData->GetLocationManager()->GetCurrentVillageID();
							pWorldMapUI->OpenFieldMap( true, villageID );
							pWorldMapUI->UpdateFieldMap();
						}
						else if( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
						{
							CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
#ifdef FIELD_BOSS_RAID // ����� ���� ����
							if( true == battleFieldManager.GetIsBossRaidCurrentField() )
							{
								g_pChatBox->AddChatLog(  GET_STRING( STR_ID_28990 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
							}
							else
#endif // FIELD_BOSS_RAID
							{
								pWorldMapUI->OpenWorldMap( true );
								pWorldMapUI->UpdateWorldMap();

								int iFiledID = battleFieldManager.GetBattleFieldIdWhereIam();
								pWorldMapUI->OpenFieldMap( true, iFiledID );
								pWorldMapUI->UpdateFieldMap();
							}
						}
					}
				} 
			} break;

		case CX2Main::XS_DUNGEON_GAME:
			{
				if( NULL != g_pX2Game && g_pX2Game->GetOpenChatBox() == false )
				{
					if( g_pMain->GetIsPlayingTutorial() == false )
					{
						CX2StateDungeonGame* pState = (CX2StateDungeonGame*) g_pMain->GetNowState();
						pState->ToggleDungeonMapUI();
					}
					return true;
				}

			} break;
		default:
			break;			
		}			
		return true;
	}

	//}} oasis907 : ����� [2009.12.17] //

	CKTDGUIDialogType pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFirstFrontModalDlg();
	if ( pDialog != NULL )
	{
		if ( wcscmp( L"DLG_Find_User_Window", pDialog->GetName() ) == 0 )
		{
		}
		else if( wcscmp( L"DLG_PlayGuide", pDialog->GetName() ) == 0 )
		{
		}
		else if( wcscmp( L"DLG_UI_FIELD_MAP", pDialog->GetName() ) == 0 )
		{
		}
		else if( wcscmp( L"DLG_UI_WORLD_MAP", pDialog->GetName() ) == 0 )
		{
		}
		else
		{
			return true;
		}
	}



#ifdef RESET_INVALID_UNIT_POSITION_TEST

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_B) == TRUE )
	{	
		const float MAGIC_INTERVAL = 5.f;


		bool bThereIsMisplacedUnit = false;
		if( m_TimerReplaceMisplacedUnit.elapsed() > MAGIC_INTERVAL )
		{
			m_TimerReplaceMisplacedUnit.restart();

			if( NULL != g_pX2Game )
			{
				bThereIsMisplacedUnit = g_pX2Game->CheckMisplacedUnit();
			}

			if( true == bThereIsMisplacedUnit )
			{
				switch( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_DUNGEON_GAME:
				case CX2Main::XS_PVP_GAME:
					{
						// ���� �� ĳ���� Ȥ�� ���Ͱ� �̻��� ��ġ�� �ִ��� Ȯ���� �Ŀ�, �̻��� ��ġ�� �ִ� ������ �������� ������ġ�� �̵���Ų��
						if( NULL != g_pX2Game )
						{
							g_pX2Game->ReplaceMisplacedUnit();
						}
					} break;

				case CX2Main::XS_SQUARE_GAME:
				case CX2Main::XS_VILLAGE_MAP:
#ifdef ADDED_RELATIONSHIP_SYSTEM
				case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
					{
						// not yet

					} break;
				}
			}
		}

		return true;
	}

#endif RESET_INVALID_UNIT_POSITION_TEST



#ifdef CHEAT_COMMAND_SHORT_CUT_KEY_TEST
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_SLASH) == TRUE )
		{
			if( NULL != g_pMain->GetGameEdit() )
			{
				g_pMain->GetGameEdit()->ExecuteLastCommand();
			}
		}
	}
#endif CHEAT_COMMAND_SHORT_CUT_KEY_TEST

#ifdef FILDE_ONLY_CHARACTER
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RSHIFT ) == TRUE )
		{
			g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

			g_pKTDXApp->GetDGManager()->ObjectChainSort();

			g_pKTDXApp->GetDGManager()->NonAlphaLayerRender( XL_SKY_WORLD_OBJECT_2 );
			g_pKTDXApp->GetDGManager()->NonAlphaLayerRender( XL_UNIT_0 );
			g_pKTDXApp->GetDGManager()->NonAlphaLayerRender( XL_UNIT_1 );
			g_pKTDXApp->GetDGManager()->NonAlphaLayerRender( XL_UNIT_2 );
			g_pKTDXApp->GetDGManager()->NonAlphaLayerRender( XL_EFFECT_0 );
			g_pKTDXApp->GetDGManager()->NonAlphaLayerRender( XL_EFFECT_1 );

			g_pKTDXApp->GetDGManager()->ScreenCapture_Png();
		}
	}
#endif FILDE_ONLY_CHARACTER

#ifdef FIELD_CHARACTER_SCREEN_SHOT
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RSHIFT ))
			g_pKTDXApp->GetDGManager()->SetField_Character_Screen_shot(true);
		else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPADSTAR ))
			g_pKTDXApp->GetDGManager()->SetField_NPC_Screen_shot(true);
		else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPADSLASH ))
			g_pKTDXApp->GetDGManager()->SetField_MainCha_Screen_shot(true);
		else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_NUMPADPERIOD ))
			g_pKTDXApp->GetDGManager()->SetField_BG_Screen_shot(true);

		if ( g_pKTDXApp->GetDGManager()->GetField_Character_Screen_shot() == TRUE ||
			g_pKTDXApp->GetDGManager()->GetField_NPC_Screen_shot() == TRUE ||
			g_pKTDXApp->GetDGManager()->GetField_MainCha_Screen_shot() == TRUE ||
			g_pKTDXApp->GetDGManager()->GetField_BG_Screen_shot() == TRUE )
		{
			g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 255, 0, 255), 1.0f, 0); //D3DCOLOR_ARGB(0, 255, 0, 255), 1.0f, 0)
			g_pKTDXApp->GetDGManager()->ObjectChainSort();
			g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender(); 
			// �ؿ��� �߰� ����			
			if (g_pKTDXApp->GetDGManager()->GetField_Character_Screen_shot() == TRUE )
			{
				g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
			}
			
			/*/
			g_pKTDXApp->GetDGManager()->ScreenCapture_Tga();
			/*/
			g_pKTDXApp->GetDGManager()->ScreenCapture_Png();
			//*/
		}
	}
#endif

	return false;
}

void CX2State::UpdateCashUI()
{
	if ( g_pData->GetCashShop() != NULL )
	{
		g_pData->GetCashShop()->UpdateCashUI();
	}
}

bool CX2State::Handler_EGS_ADMIN_CHANGE_AUTH_LEVEL_REQ( const WCHAR* userID, CX2User::X2_USER_AUTH_LEVEL level )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return false; 

	if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
		return false;

	KEGS_ADMIN_CHANGE_AUTH_LEVEL_REQ kPacket;
	kPacket.m_wstrUserID	= userID;
	kPacket.m_cAuthLevel	= (char) level;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, 10.f ); 

	return true;
}


bool CX2State::Handler_EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_AUTH_LEVEL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}


bool CX2State::Handler_EGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ( CX2User::X2_USER_AUTH_LEVEL level )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return false; 

	KEGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ kPacket;
	kPacket.m_cAuthLevel = (char)level;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK, 10.f ); 

	return true;
}


bool CX2State::Handler_EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true && NULL != g_pMain->GetGameEdit() )
		{
			for( UINT i=0; i<kEvent.m_vecUserIDList.size(); i++ )
			{
				g_pMain->GetGameEdit()->AddString( CX2ChatWindow::CT_NORMAL, kEvent.m_vecUserIDList[i].c_str() );
				
			}
			return true;
		}
	}

	return false;
}






bool CX2State::Handler_EGS_UPDATE_QUEST_REQ()
{
	KEGS_UPDATE_QUEST_REQ kPacket;

	g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_QUEST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_UPDATE_QUEST_ACK );

	return true;
}


bool CX2State::Handler_EGS_UPDATE_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_QUEST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_UPDATE_QUEST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2State::Handler_EGS_UPDATE_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_QUEST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );
	
	//{{�׽�Ʈ�ڵ�
	int i = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_nMapID;
	CX2LocationManager::LOCAL_MAP_ID eMapID =  static_cast<CX2LocationManager::LOCAL_MAP_ID>(i);
	//�׽�Ʈ�ڵ�}}

	for( UINT i=0; i<kEvent.m_vecQuestInst.size(); i++ )
	{
		KQuestInstance& questInst = kEvent.m_vecQuestInst[i];
#ifdef SERV_EPIC_QUEST
		vector<KSubQuestInstance> vecUpdateSubQuestInstance;
		for( UINT i = 0; i<questInst.m_vecSubQuestInstance.size(); i++)
		{
			const KSubQuestInstance& kSubQuestInst = questInst.m_vecSubQuestInstance[i];

			if(g_pData->GetQuestManager()->CheckUpdateSubQuestInst( questInst.m_iID, kSubQuestInst) == true)
			{
				vecUpdateSubQuestInstance.push_back(kSubQuestInst);
			}
		}
#endif SERV_EPIC_QUEST
		g_pData->GetQuestManager()->UpdateUnitQuest( questInst.m_iID, questInst );

		// ����Ʈ ���� �ǽð� ǥ��
		CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( questInst.m_iID );
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( questInst.m_iID );
		if( NULL != g_pX2Game && NULL != pQuestTemplet )
		{
#ifdef SERV_EPIC_QUEST
			for( UINT j=0; j<vecUpdateSubQuestInstance.size(); j++ )
#else
			for( UINT j=0; j<questInst.m_vecSubQuestInstance.size(); j++ )
#endif SERV_EPIC_QUEST

			{
#ifdef SERV_EPIC_QUEST
				KSubQuestInstance subQuestInst = vecUpdateSubQuestInstance[j];
#else
				KSubQuestInstance subQuestInst = questInst.m_vecSubQuestInstance[j];
#endif SERV_EPIC_QUEST
				const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = 
					g_pData->GetQuestManager()->GetSubQuestTemplet( subQuestInst.m_iID );

				if( NULL == pSubQuestTemplet )
					continue;

				switch( pSubQuestTemplet->m_eClearType )
				{
#ifdef PVP_SEASON2
				case CX2QuestManager::SQT_PVP_PLAY_ARRANGE:
					{
						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iPVPPlay )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );

							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
						}
					} break;
#endif
				case CX2QuestManager::SQT_NPC_HUNT:
					{
						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iKillNum )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );
#ifdef CLIENT_SUBQUEST_POPUP_LINEBREAK

#ifdef CLIENT_GLOBAL_LINEBREAK
							wstrSubQuestDesc = CWordLineHandler::GetStrByLineBreakInX2Main(wstrSubQuestDesc.c_str(), 500, XUF_DODUM_20_BOLD);
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef ALWAYS_TEMP_LINEBREAK //�±��� ���κ극��ũ
#else //ALWAYS_TEMP_LINEBREAK ����� ���⼭ ���κ극��ũ�� ���� �ʽ��ϴ�.
							wstrSubQuestDesc = g_pMain->GetStrByLienBreak( wstrSubQuestDesc.c_str(), 500, XUF_DODUM_20_BOLD );
#endif ALWAYS_TEMP_LINEBREAK
#endif //CLIENT_GLOBAL_LINEBREAK

#endif //CLIENT_SUBQUEST_POPUP_LINEBREAK							
							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

#ifdef NEW_VILLAGE_UI

#ifdef SERV_EPIC_QUEST
							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
#else
							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuest() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuest();
							}
							if(true == g_pData->GetUIManager()->GetUIQuest()->GetShowQuickQuestDLG())
							{
								g_pData->GetUIManager()->GetUIQuest()->UpdateQuickQuestDLG();
							}
#endif SERV_EPIC_QUEST

#endif
						}
					} break;

				case CX2QuestManager::SQT_PVP_KILL:
					{
						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iPVPKill )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );

							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
#ifdef NEW_VILLAGE_UI

#ifdef SERV_EPIC_QUEST
							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
#else
							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuest() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuest();
							}
							if(true == g_pData->GetUIManager()->GetUIQuest()->GetShowQuickQuestDLG())
							{
								g_pData->GetUIManager()->GetUIQuest()->UpdateQuickQuestDLG();
							}
#endif SERV_EPIC_QUEST
#endif
						}
					} break;
#ifdef SERV_INTEGRATION
				case CX2QuestManager::SQT_WITH_DIF_SERV_USER:
					{

						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );

							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
#ifdef NEW_VILLAGE_UI

#ifdef SERV_EPIC_QUEST
							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
#else
							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuest() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuest();
							}
							if(true == g_pData->GetUIManager()->GetUIQuest()->GetShowQuickQuestDLG())
							{
								g_pData->GetUIManager()->GetUIQuest()->UpdateQuickQuestDLG();
							}
#endif SERV_EPIC_QUEST
#endif
						}

					} break;
#endif SERV_INTEGRATION
#ifdef REFORM_QUEST
				case CX2QuestManager::SQT_VISIT_FIELD:
				case CX2QuestManager::SQT_VISIT_DUNGEON:
					{
						if( NULL != pQuestInst 
							&& true == pQuestInst->IsCompleteVisitQuest(pSubQuestTemplet->m_iID) )
						{
							CompleteQuestToVisitVillage( pQuestTemplet );
						}
					} break;
#endif //REFORM_QUEST
#ifdef SERV_SUB_QUEST_USE_ITEM
				case CX2QuestManager::SQT_ITEM_USE:
					{
						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iUseItemNum )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );

							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
							if(true == g_pData->GetUIManager()->GetUIQuestNew()->GetShowQuickQuestDLG())
							{
								g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
							}
						}
					} break;
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_SKILL_USE_SUBQUEST
				case CX2QuestManager::SQT_SKILL_USE:
					{
						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );

							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
							if(true == g_pData->GetUIManager()->GetUIQuestNew()->GetShowQuickQuestDLG())
							{
								g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
							}
						}
					} break;
#endif SERV_SKILL_USE_SUBQUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
				case CX2QuestManager::SQT_LEARN_NEW_SKILL:
					{
						int iCount = (int) subQuestInst.m_ucClearData;
						if( iCount <= pSubQuestTemplet->m_ClearCondition.m_iLearnNewSkillCount )
						{
							wstring wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription,
								pSubQuestTemplet, pQuestInst );

							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, wstrSubQuestDesc.c_str(), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

							//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
							if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
							{
								g_pData->GetUIManager()->CreateUIQuestNew();
							}
							if(true == g_pData->GetUIManager()->GetUIQuestNew()->GetShowQuickQuestDLG())
							{
								g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
							}
						}
					} break;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
				}
			}
		}
#ifdef SERV_QUEST_CLEAR_EXPAND
		// oasis907 : ����� [2011.1.26] // ����, PVP���� �ƴ� ������ ����Ʈ ���� �ǽð� ������Ʈ�� �ʿ��� ��� ���⿡��
		else if(NULL != pQuestTemplet )
		{
#ifdef SERV_EPIC_QUEST
			for( UINT j=0; j<vecUpdateSubQuestInstance.size(); j++ )
#else
			for( UINT j=0; j<questInst.m_vecSubQuestInstance.size(); j++ )
#endif SERV_EPIC_QUEST

			{
#ifdef SERV_EPIC_QUEST
				//2009.01.05 ���¿� : ����Ʈ �ǽð� ǥ�����
				if(g_pData->GetUIManager()->GetUIQuestNew() == NULL)
				{
					g_pData->GetUIManager()->CreateUIQuestNew();
				}

				KSubQuestInstance subQuestInst = vecUpdateSubQuestInstance[j];
#else
				KSubQuestInstance subQuestInst = questInst.m_vecSubQuestInstance[j];
#endif SERV_EPIC_QUEST
				const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = 
					g_pData->GetQuestManager()->GetSubQuestTemplet( subQuestInst.m_iID );

				if( NULL == pSubQuestTemplet )
					continue;

				switch( pSubQuestTemplet->m_eClearType )
				{
				case CX2QuestManager::SQT_ITEM_ENCHANT:
				case CX2QuestManager::SQT_ITEM_SOCKET:
				case CX2QuestManager::SQT_ITEM_ATTRIB:
				case CX2QuestManager::SQT_ITEM_RESOLVE:
				case CX2QuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
				case CX2QuestManager::SQT_USE_SKILL_POINT:
#ifdef PVP_SEASON2
				case CX2QuestManager::SQT_PVP_PLAY_ARRANGE:
#endif
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
				case CX2QuestManager::SQT_CHAR_LEVEL_UP:
#endif //SERV_ACCOUNT_MISSION_SYSTEM
					{
						if ( NULL != g_pData->GetUIManager()->GetUIQuestNew() &&
							true == g_pData->GetUIManager()->GetUIQuestNew()->GetShow())
						{
							g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
						}
					}
					break;

					// {{ kimhc // 2012-06-18 // ������ �̵��ϱ⸸ �ص� �Ϸ�Ǵ� ����Ʈ �۾�
#ifdef	SERV_ENTER_FIELD_QUEST_CLEAR
				case CX2QuestManager::SQT_VISIT_VILLAGE:
					{
#ifdef REFORM_QUEST
						if( NULL != pQuestInst && true == pQuestInst->IsCompleteVisitQuest(pSubQuestTemplet->m_iID) )
#endif //REFORM_QUEST
						{
							CompleteQuestToVisitVillage( pQuestTemplet );
						}
					} break;
#endif	SERV_ENTER_FIELD_QUEST_CLEAR
					// }} kimhc // 2012-06-18 // ������ �̵��ϱ⸸ �ص� �Ϸ�Ǵ� ����Ʈ �۾�
				}
			}
		}
#endif SERV_QUEST_CLEAR_EXPAND
	}

	g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();

	g_pData->GetQuestManager()->UpdateQuest();
#ifdef QUEST_GUIDE
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetNpcIndicator() )
			{
				g_pTFieldGame->GetNpcIndicator()->ResetGuideTargetInfo();
			}
		} break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( NULL != g_pX2Game && NULL != static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator() )
			{
				static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator()->ResetGuideTargetInfo();
			}
		} break;
	}
#endif //QUEST_GUIDE



	return true;
}

bool CX2State::Handler_EGS_CHECK_SUM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_SUM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHECK_SUM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}


bool CX2State::Handler_EGS_ADMIN_KICK_USER_REQ( const WCHAR* szUserNickName, bool bIsUserID /*= false*/ )
{
	KEGS_ADMIN_KICK_USER_REQ kPacket;
	kPacket.m_wstrUnitNickName = szUserNickName;
	kPacket.m_bIsUserID = bIsUserID;
	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_KICK_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_KICK_USER_ACK );

	return true;
}

bool CX2State::Handler_EGS_ADMIN_KICK_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_KICK_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->DeleteServerPacket( EGS_ADMIN_KICK_USER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}



bool CX2State::Handler_EGS_ADMIN_CHANGE_SKILL_POINT_REQ( int iSP )
{
	KEGS_ADMIN_CHANGE_SKILL_POINT_REQ kPacket;
	kPacket.m_iSPoint = iSP;
#ifdef SKILL_PAGE_SYSTEM //JHKang
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;
#endif //SKILL_PAGE_SYSTEM

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_CHANGE_SKILL_POINT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_CHANGE_SKILL_POINT_ACK );

	return true;
}


bool CX2State::Handler_EGS_ADMIN_CHANGE_SKILL_POINT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_SKILL_POINT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_SKILL_POINT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
			pUnitData->m_iSPoint = kEvent.m_iSPoint;

			g_pData->GetUIManager()->GetUISkillTree()->UpdateSPInfo();

			
			return true;
		}
	}

	return false;
}




#ifdef GUILD_SKILL

//{{ oasis907 : ����� [2009.11.30] // ��� ��ų ����Ʈ ġƮ
bool CX2State::Handler_EGS_ADMIN_GET_GUILD_SKILL_POINT_REQ( int iSP )
{
	KEGS_ADMIN_GET_GUILD_SKILL_POINT_REQ kPacket;
	kPacket.m_iGuildSPoint = iSP;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_REQ, kPacket );

	return true;
}


bool CX2State::Handler_EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_GET_GUILD_SKILL_POINT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
	pUnitData->m_iGuildSPoint = kEvent.m_iCurGuildSPoint;

	if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
	{
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateSPInfo();
	}
	return true;
}
//}} oasis907 : ����� [2009.11.30] //

#endif GUILD_SKILL












bool CX2State::Handler_EGS_ADMIN_INIT_SKILL_TREE_REQ()
{
#ifdef SKILL_PAGE_SYSTEM //JHKang
	KEGS_ADMIN_INIT_SKILL_TREE_REQ kPacket;
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_INIT_SKILL_TREE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK );
#else //SKILL_PAGE_SYSTEM
	g_pData->GetServerProtocol()->SendID( EGS_ADMIN_INIT_SKILL_TREE_REQ );
	g_pMain->AddServerPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK );
#endif //SKILL_PAGE_SYSTEM

	return true;
}


bool CX2State::Handler_EGS_ADMIN_INIT_SKILL_TREE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_INIT_SKILL_TREE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			// ��ųƮ���� ���� ��ų�� �ʱ�ȭ�Ѵ�
			pUnitData->m_UserSkillTree.Reset( true, true, false, false );
			
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		#ifdef SKILL_PAGE_SYSTEM //JHKang
			pUnitData->m_UserSkillTree.SetDefaultSkill( pUnitData->m_UserSkillTree.GetUsingPage() );	// �� ������ �⺻ ��ų ����
		#else //SKILL_PAGE_SYSTEM
			pUnitData->m_UserSkillTree.SetDefaultSkill();		/// �� ������ �⺻ ��ų ����
		#endif //SKILL_PAGE_SYSTEM

			/// ��ȯ�Ǵ� ����Ʈ ����
			pUnitData->m_iSPoint = kEvent.m_iSPoint;
			pUnitData->m_iCSPoint = kEvent.m_iCSPoint;
	#else // UPGRADE_SKILL_SYSTEM_2013
			// note!! ĳ���� �߰��Ǹ� �۾��ؾ���
			// ĳ���ͺ� �⺻ ��ų ����
			switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
			{
			case CX2Unit::UT_ELSWORD:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_ES_MEGASLASH, 1, 0 );
		#ifdef ELSWORD_SHEATH_KNIGHT
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_ROLLING, 1, 0 );
		#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_COUNTER, 1, 0 );
		#endif ELSWORD_SHEATH_KNIGHT
				} break;

			case CX2Unit::UT_ARME:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_AV_CHAIN_FIRE_BALL, 1, 0 );
		#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_FIRE_ROAD, 1, 0 );					
		#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_TELEPORT, 1, 0 );
		#endif
				} break;

			case CX2Unit::UT_LIRE:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_LE_RAIL_STINGER, 1, 0 );
		#ifdef SERV_TRAPPING_RANGER_TEST
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_RISING_FALCON, 1, 0 );
		#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_SIEGEMODE, 1, 0 );
		#endif SERV_TRAPPING_RANGER_TEST
				} break;

			case CX2Unit::UT_RAVEN:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_RF_MAXIMUM_CANNON, 1, 0 );
		#ifdef RAVEN_WEAPON_TAKER
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BREAKING_FIST, 1, 0 );
		#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BURNING_RUSH, 1, 0 );
		#endif RAVEN_WEAPON_TAKER
				} break;

			case CX2Unit::UT_EVE:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_EN_ILLUSION_STRIKE, 1, 0 );
		#ifdef EVE_ELECTRA
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EVE_PHOTON_BLINK, 1, 0 );
		#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EN_CLOAKING, 1, 0 );
		#endif EVE_ELECTRA
				} break;

				//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
		#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_CHUNG_BRUTAL_SWING, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_CHUNG_RELOAD_CANNON, 1, 0 );
				} break;

		#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

		#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_ARA_WIND_WEDGE, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ARA_GAIN_FORCE, 1, 0 );

				} break;
		#endif
		#ifdef NEW_CHARACTER_EL
			case CX2Unit::UT_ELESIS:
				{
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AM_WIND_WEDGE, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AM_GAIN_FORCE, 1, 0 );

				} break;
		#endif // NEW_CHARACTER_EL

			}
#endif // UPGRADE_SKILL_SYSTEM_2013

			// ��ųƮ�� UI ������Ʈ
			if( g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
			}

			return true;
		}
	}

	return false;
}

bool CX2State::Handler_EGS_ADMIN_KICK_USER_REQ_LUA( const char* szUserNickName, bool bIsUserID /*= false*/ )
{
	wstring userNickName;
#ifdef CHEAT_WCHART_TO_UTF8
	ConvertUtf8ToWCHAR( userNickName, szUserNickName );
#else
	ConvertCharToWCHAR( userNickName, szUserNickName );
#endif CHEAT_WCHART_TO_UTF8

	return Handler_EGS_ADMIN_KICK_USER_REQ( userNickName.c_str(), bIsUserID );
}

#ifdef SERV_PC_BANG_TYPE
bool CX2State::Handler_EGS_ADMIN_SET_PC_BANG_REQ( bool bEnable, int iPcBangType /*= 0*/ )
#else
bool CX2State::Handler_EGS_ADMIN_SET_PC_BANG_REQ( bool bEnable )
#endif SERV_PC_BANG_TYPE
{
#ifdef LIGHT_OPERATOR_ACCOUNT
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_LIGHT_OPERATOR )
	{
		return false;
	}
#endif LIGHT_OPERATOR_ACCOUNT

	KEGS_ADMIN_SET_PC_BANG_REQ kPacket;
	kPacket.m_bOn = bEnable;
#ifdef SERV_PC_BANG_TYPE
	kPacket.m_iPcBangType = iPcBangType;
#endif //SERV_PC_BANG_TYPE

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_SET_PC_BANG_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_SET_PC_BANG_ACK );	

	return true;
}

bool CX2State::Handler_EGS_CHECK_BALANCE_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CHECK_BALANCE_REQ );
	g_pMain->AddServerPacket( EGS_CHECK_BALANCE_ACK );

	return true;
}

//{{ kimhc // 2009-10-14 // �ڵ����� �Ϸ��� ĳ���κ� ����
#ifdef	AUTO_PAYMENT
bool CX2State::Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ()
{
	if ( g_pData != NULL &&
		 g_pData->GetCashShop() != NULL )
	{
#ifdef SERV_GLOBAL_BILLING
		g_pData->GetCashShop()->Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
		g_pData->GetCashShop()->Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING
	}
	return true;
}
#endif	AUTO_PAYMENT
//}} kimhc // 2009-10-14 // �ڵ����� �Ϸ��� ĳ���κ� ����

//{{ 2008. 1. 2  ������  �ټ��� ġƮ
bool CX2State::Handler_EGS_ADMIN_CHANGE_SPIRIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_SPIRIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_SPIRIT_ACK ) == true )
	{
		CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

		pUnitData->m_iSpirit = kEvent.m_iChangedSpirit;

		return true;
	}

	return false;
}
//}}



bool CX2State::Handler_EGS_ADMIN_SET_PC_BANG_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_SET_PC_BANG_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_SET_PC_BANG_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData != NULL && g_pData->GetMyUser() != NULL )
				g_pData->GetMyUser()->SetPCRoom( kEvent.m_bOn );

			if ( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_bIsGameBang = kEvent.m_bOn;
			}
#ifdef SERV_PC_BANG_TYPE
			// �������� ���� pc�� ������ Ŭ�� ������ �Ӵϴ�.
			if ( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )
				g_pData->GetPremiumBuffManager()->SetPcBangReward( kEvent.m_kPcBangReward );
#endif SERV_PC_BANG_TYPE
		}
		
		return true;
	}

	return false;
}

//{{ 2008. 1. 25  ������  VP ġƮ
bool CX2State::Handler_EGS_ADMIN_CHANGE_VSPOINT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_VSPOINT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
				pUnitData->m_cRank	= kEvent.m_cRank;
#else
				pUnitData->m_iRating	= kEvent.m_iChangedRating;
#endif
				pUnitData->m_iRPoint	= kEvent.m_iChangedRPoint;
				pUnitData->m_iAPoint	= kEvent.m_iChangedAPoint;
#else
				pUnitData->m_VSPoint	= kEvent.m_iChangedVSPoint;
				pUnitData->m_VSPointMax = kEvent.m_iChangedVSPointMax;
#endif
			}
		}

		return true;
	}

	return false;
}
//}}

//{{ 2008. 2. 18  ������  ���� Ŭ���� ġƮ
bool CX2State::Handler_EGS_ADMIN_CHANGE_UNIT_CLASS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//if ( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			//{
			//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

			//	pUnitData->m_UnitClass = static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_cUnitClass );
			//}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			/// ������ ��� - ���� Ŭ���� ����� ��ų �ʱ�ȭ �� �⺻ ��ų ����
			if ( NULL == g_pData ||
				 NULL == g_pData->GetMyUser() ||
				 NULL == g_pData->GetMyUser()->GetSelectUnit() )
			{
				return false;
			}

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			{
				pUnitData->m_UserSkillTree.Reset( true, true, false, false );
#ifdef SKILL_PAGE_SYSTEM //JHKang
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill1 ), 1, 0,
					pUnitData->m_UserSkillTree.GetUsingPage() );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill2 ), 1, 0,
					pUnitData->m_UserSkillTree.GetUsingPage() );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill3 ), 1, 0,
					pUnitData->m_UserSkillTree.GetUsingPage() );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill4 ), 1, 0,
					pUnitData->m_UserSkillTree.GetUsingPage() );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill5 ), 1, 0,
					pUnitData->m_UserSkillTree.GetUsingPage() );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill6 ), 1, 0,
					pUnitData->m_UserSkillTree.GetUsingPage() );
#else //SKILL_PAGE_SYSTEM
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill1 ), 1, 0 );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill2 ), 1, 0 );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill3 ), 1, 0 );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill4 ), 1, 0 );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill5 ), 1, 0 );
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDefaultSkill6 ), 1, 0 );
#endif //SKILL_PAGE_SYSTEM

				pUnitData->m_iSPoint	= kEvent.m_iSPoint;
				pUnitData->m_iCSPoint	= kEvent.m_iCSPoint;

				CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_cUnitClass));
#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
				CX2GageManager::GetInstance()->ResetGageUIEtc(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_cUnitClass));			
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG
			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		return true;
	}

	return false;
}
//}}

//{{ 2008. 10. 24  ������	Ÿ��Ʋ ġƮ
bool CX2State::Handler_EGS_ADMIN_GET_TITLE_CHEAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
		}

		return true;
	}

	return false;
}
//}}








//bool CX2State::Handler_EGS_REWARD_TO_RECOMMEND_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_REWARD_TO_RECOMMEND_USER_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	wstring getExplanation = L"";
//	if ( kEvent.m_mapRewardItem.empty() == false )
//	{
//		std::map< int, int >::iterator i;
//
//		int nCount = 0;
//		wstring itemName;
//		wstringstream buf;
//		for ( i = kEvent.m_mapRewardItem.begin(); i != kEvent.m_mapRewardItem.end(); i++ )
//		{
//			int resultItemID = i->first;
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet != NULL )
//				itemName = pItemTemplet->GetFullName_();
//
//			if ( nCount > 0 )
//			{
//				getExplanation += L", ";
//			}
//
//			buf.str( L"" );
//			buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//			getExplanation += buf.str().c_str();
//
//			nCount++;
//		}
//	}
//
//	wstringstream wstrstm;
//	wstrstm << L"[" << kEvent.m_wstrNickName.c_str() << L"]�Բ��� ����[" << (int)(kEvent.m_cLevel) << L"]�� �޼��Ͽ� ��õ�� ����(" << getExplanation.c_str() << L")�� ���޵Ǿ����ϴ�.";
//
//	if ( g_pX2Game == NULL )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
//	}
//	else
//	{
//		switch( g_pMain->GetNowStateID() )
//		{
//		case CX2Main::XS_PVP_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_PVP_ROOM );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_PVP_LOBBY );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//			} break;
//
//		case CX2Main::XS_DUNGEON_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_DUNGEON_ROOM );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_LOCAL_MAP );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//
//			} break;
//
//		case CX2Main::XS_ARCADE_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_ARCADE_ROOM );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_ARCADE_LOBBY );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//
//			} break;
//
//		case CX2Main::XS_TRAINING_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_TRAINING_SCHOOL );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//			} break;
//
//		default:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//			} break;
//		}
//	}
//
//
//	if ( g_pPartyMsgerUI != NULL )
//	{
//		g_pPartyMsgerUI->AddChatLog( wstrstm.str().c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
//	}
//
//	return true;
//}
//
//
//bool CX2State::Handler_EGS_REWARD_TO_TUTORIAL_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_REWARD_TO_TUTORIAL_USER_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	wstring getExplanation = L"";
//	if ( kEvent.m_mapRewardItem.empty() == false )
//	{
//		std::map< int, int >::iterator i;
//
//		int nCount = 0;
//		wstring itemName;
//		wstringstream buf;
//		for ( i = kEvent.m_mapRewardItem.begin(); i != kEvent.m_mapRewardItem.end(); i++ )
//		{
//			int resultItemID = i->first;
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet != NULL )
//				itemName = pItemTemplet->GetFullName_();
//
//			if ( nCount > 0 )
//			{
//				getExplanation += L", ";
//			}
//
//			buf.str( L"" );
//			buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//			getExplanation += buf.str().c_str();
//
//			nCount++;
//		}
//	}
//
//	wstringstream wstrstm;
//	if( true == kEvent.m_bGraduation )
//	{
//		wstrstm << L"[" << kEvent.m_wstrNickName.c_str() << L"]�Բ��� �������踦 �����Ͽ� ����(" << 
//			getExplanation.c_str() << L")�� ���޵Ǿ����ϴ�.";
//	}
//	else
//	{
//		wstrstm << L"[" << kEvent.m_wstrNickName.c_str() << L"]�Բ��� ����[" << (int)(kEvent.m_cLevel) << L"]�� �޼��Ͽ� �������� ����(" << 
//			getExplanation.c_str() << L")�� ���޵Ǿ����ϴ�.";
//	}
//
//	if ( g_pX2Game == NULL )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
//	}
//	else
//	{
//		switch( g_pMain->GetNowStateID() )
//		{
//		case CX2Main::XS_PVP_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_PVP_ROOM );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_PVP_LOBBY );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//			} break;
//
//		case CX2Main::XS_DUNGEON_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_DUNGEON_ROOM );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_LOCAL_MAP );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//
//			} break;
//
//		case CX2Main::XS_ARCADE_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_ARCADE_ROOM );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_ARCADE_LOBBY );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//
//			} break;
//
//		case CX2Main::XS_TRAINING_GAME:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_TRAINING_SCHOOL );
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//			} break;
//
//		default:
//			{
//				CX2Main::ReservedMessagePopUp reservedMsg;
//				reservedMsg.m_Message = wstrstm.str();
//				reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;
//
//				reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
//				g_pMain->AddReservedMessagePopup( reservedMsg );
//			} break;
//		}
//	}
//
//
//	//if ( g_pPartyMsgerUI != NULL )
//	//{
//	//	g_pPartyMsgerUI->AddChatLog( wstrstm.str().c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
//	//}
//
//	return true;
//}
//


bool CX2State::Handler_EGS_REWARD_MARKETING_EVENT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	//KEGS_REWARD_MARKETING_EVENT_NOT kEvent;
	//DeSerialize( pBuff, &kEvent );



	//if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
	//{
	//	for( UINT i=0; i<kEvent.m_vecEventID.size(); i++ )
	//	{
	//		if( g_pData->GetMyUser()->GetSelectUnit()->GetMarketingEventID() == kEvent.m_vecEventID[i] )
	//		{
	//			g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( true );
	//		}
	//	}
	//}

	//wstring getExplanation = L"";
	//if ( kEvent.m_mapRewardItem.empty() == false )
	//{
	//	std::map< int, int >::iterator i;

	//	int nCount = 0;
	//	wstring itemName;
	//	wstringstream buf;
	//	for ( i = kEvent.m_mapRewardItem.begin(); i != kEvent.m_mapRewardItem.end(); i++ )
	//	{
	//		int resultItemID = i->first;

	//		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
	//		if ( pItemTemplet != NULL )
	//			itemName = pItemTemplet->GetFullName_();

	//		if ( nCount > 0 )
	//		{
	//			getExplanation += L", ";
	//		}

	//		buf.str( L"" );
	//		buf << L"[" << itemName.c_str() << L"] " << i->second << L"��";

	//		getExplanation += buf.str().c_str();

	//		nCount++;
	//	}
	//}



	//wstringstream wstrstm;
	//wstrstm << L"�̺�Ʈ �������� " << getExplanation.c_str() << L"�� ���޵Ǿ����ϴ�.";

	//if ( g_pX2Game == NULL )
	//{
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this, SUCM_GET_POST_ITEM );
	//}
	//else
	//{
	//	switch( g_pMain->GetNowStateID() )
	//	{
	//	case CX2Main::XS_PVP_GAME:
	//		{
	//			CX2Main::ReservedMessagePopUp reservedMsg;
	//			reservedMsg.m_Message = wstrstm.str();
	//			reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;

	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_PVP_ROOM );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_PVP_LOBBY );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
	//			g_pMain->AddReservedMessagePopup( reservedMsg );
	//		} break;

	//	case CX2Main::XS_DUNGEON_GAME:
	//		{
	//			CX2Main::ReservedMessagePopUp reservedMsg;
	//			reservedMsg.m_Message = wstrstm.str();
	//			reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;

	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_DUNGEON_ROOM );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_LOCAL_MAP );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
	//			g_pMain->AddReservedMessagePopup( reservedMsg );

	//		} break;

	//	case CX2Main::XS_ARCADE_GAME:
	//		{
	//			CX2Main::ReservedMessagePopUp reservedMsg;
	//			reservedMsg.m_Message = wstrstm.str();
	//			reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;

	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_ARCADE_ROOM );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_ARCADE_LOBBY );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
	//			g_pMain->AddReservedMessagePopup( reservedMsg );

	//		} break;

	//	case CX2Main::XS_TRAINING_GAME:
	//		{
	//			CX2Main::ReservedMessagePopUp reservedMsg;
	//			reservedMsg.m_Message = wstrstm.str();
	//			reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;

	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_TRAINING_SCHOOL );
	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
	//			g_pMain->AddReservedMessagePopup( reservedMsg );
	//		} break;

	//	default:
	//		{
	//			CX2Main::ReservedMessagePopUp reservedMsg;
	//			reservedMsg.m_Message = wstrstm.str();
	//			reservedMsg.m_iOKMsg = (int)SUCM_GET_POST_ITEM;

	//			reservedMsg.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
	//			g_pMain->AddReservedMessagePopup( reservedMsg );
	//		} break;
	//	}
	//}


	return true;
}

//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
/*virtual*/ bool CX2State::Handler_EGS_TIME_EVENT_INFO_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TIME_EVENT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;

	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	switch ( kEvent.m_cNotifyType )
	{
	case KEGS_TIME_EVENT_INFO_NOT::TENT_INIT:
		{
			pMyUnit->m_mapEventData.clear();
			
			BOOST_TEST_FOREACH( const KTimeEventSimpleInfo&, kTimeEventSimpleInfo, kEvent.m_vecTimeEvent )
			{
				CX2Unit::EventData eventData( kTimeEventSimpleInfo );
				pMyUnit->AddMarketingEvent( eventData );
			}

			pMyUnit->SetReceivedGuestUserReward( false );
		}
		break;

	case KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN:
		{
			BOOST_TEST_FOREACH( const KTimeEventSimpleInfo&, kTimeEventSimpleInfo, kEvent.m_vecTimeEvent )
			{
				CX2Unit::EventData eventData( kTimeEventSimpleInfo );
				pMyUnit->AddMarketingEvent( eventData );
			}

			pMyUnit->SetReceivedGuestUserReward( false );
		}
		break;
		
	case KEGS_TIME_EVENT_INFO_NOT::TENT_END:
		{
			BOOST_TEST_FOREACH( const KTimeEventSimpleInfo&, kTimeEventSimpleInfo, kEvent.m_vecTimeEvent )
			{
				pMyUnit->DeleteMarketingEvent( kTimeEventSimpleInfo.m_iEventUID );

				//{{ kimhc // 2009-11-18 // 2009.11.19�� ��ġ �̺�Ʈ������ �ӽ� �ڵ� �߰�( ������ ����! )
				const int iChristMasEventID = 183;

				if ( kTimeEventSimpleInfo.m_iEventID == iChristMasEventID )
				{
					switch ( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_SQUARE_GAME:
					case CX2Main::XS_PVP_GAME:
					case CX2Main::XS_DUNGEON_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
					case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
						{
							CX2Main::ReservedMessagePopUp EventPopUp;

							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_PVP_LOBBY );
							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_PVP_ROOM );
							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_DUNGEON_ROOM );
							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_SQUARE_GAME );
							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_TRAINING_SCHOOL );
#ifdef ADDED_RELATIONSHIP_SYSTEM
							EventPopUp.m_vecTargetState.push_back( CX2Main::XS_WEDDING_GAME );
#endif // ADDED_RELATIONSHIP_SYSTEM

							EventPopUp.m_Message = GET_STRING( STR_ID_4811 );

							g_pMain->AddReservedMessagePopup( EventPopUp );
						}
						break;

					default:
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4811 ), (CKTDXStage*) g_pMain->GetNowState() ); 
						}
						break;
					}
				}
				//}} kimhc // 2009-11-18 // 2009.11.19�� ��ġ �̺�Ʈ������ �ӽ� �ڵ� �߰�( ������ ����! )
			}			

			if ( pMyUnit->m_mapEventData.empty() == true )
				pMyUnit->SetReceivedGuestUserReward( true );
		}
		break;

	default:
		break;
	}

	return true;
	
}
#endif	CHRISTMAS_TREE
//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ

#ifdef WEB_POINT_EVENT // oasis907 : ����� [2010.2.24] // 
/*virtual*/ bool CX2State::Handler_EGS_GET_WEB_POINT_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4920 ), (CKTDXStage*) g_pMain->GetNowState() ); 
	return true;
}
#endif WEB_POINT_EVENT

bool CX2State::Handler_EGS_RECOMMEND_USER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RECOMMEND_USER_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//m_wstrFromNickName
	wstring tempString;
	tempString = GET_REPLACED_STRING( ( STR_ID_543, "L", kEvent.m_wstrFromNickName ) );

	if ( g_pChatBox != NULL )
	{
		g_pChatBox->AddChatLog( tempString.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
	
	if ( g_pX2Game == NULL )
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.c_str(), this );


	return true;
}



bool CX2State::Handler_EGS_CHECK_BALANCE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_BALANCE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_CHECK_BALANCE_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData->GetMyUser() != NULL )
			{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
				if (kEvent.m_bOnlyType)
				{
					if (kEvent.m_iCashType >= 0 && kEvent.m_iCashType < KGlobalCashInfo::GCT_TOTAL_NUM)
					{
						g_pData->GetMyUser()->SetGlobalTypeCash(kEvent.m_iCashType, kEvent.m_GlobalCashInfo.m_ulCash[kEvent.m_iCashType]);
					}
				}
				else
				{
					g_pData->GetMyUser()->SetGlobalCash(kEvent.m_GlobalCashInfo);
				}
#else  //SERV_SUPPORT_SEVERAL_CASH_TYPES
				g_pData->GetMyUser()->SetCash( kEvent.m_ulBalance );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
				UpdateCashUI();
			}
			return true;
		}
	}

	return false;
}

bool CX2State::Handler_EGS_CASH_PRODUCT_INFO_CHANGED_NOT()
{
	if ( g_pData != NULL )
	{
		if ( g_pData->GetItemManager() != NULL )
			g_pData->GetItemManager()->ClearCashItemList();
	}

	return true;
}

// void CX2State::RenderMarketingEventTimer()
// {
// 	if( NULL != m_pDLGEventClock )
// 		m_pDLGEventClock->SetShowEnable( false, false );
// 
// 	switch( g_pMain->GetNowStateID() )
// 	{
// 	case CX2Main::XS_START_UP:
// 	case CX2Main::XS_LOGIN:
// 	case CX2Main::XS_LOADING:
// 	case CX2Main::XS_SERVER_SELECT:
// //	case CX2Main::XS_UNIT_SELECT:
// 	case CX2Main::XS_CREATE_UNIT:
// 	case CX2Main::XS_DUNGEON_GAME:
// 	case CX2Main::XS_ARCADE_GAME:
// 	case CX2Main::XS_PVP_GAME:
// 	case CX2Main::XS_TRAINING_GAME:
// 		{
// 			return;
// 		} break;
// 	}
// 
// 	if( NULL != g_pData &&
// 		NULL != g_pData->GetCashShop() &&
// 		true == g_pData->GetCashShop()->GetOpen() )
// 	{
// 		return;
// 	}
// 
// 
// 
// 	if( NULL != g_pData &&
// 		NULL != g_pData->GetMyUser() &&
// 		NULL != g_pData->GetMyUser()->GetSelectUnit() && 
// 		false == g_pData->GetMyUser()->GetSelectUnit()->GetReceivedGuestUserReward() /*&&
// 		(float)g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().elapsed() < g_pData->GetMyUser()->GetSelectUnit()->GetMarketingEventTime()*/ )
// 	{
// 		if( NULL == m_pDLGEventClock )
// 		{
// 			m_pDLGEventClock = new CKTDGUIDialog( this, L"DLG_Event_Clock.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEventClock );
// 		}
// 
// 		if( NULL != m_pDLGEventClock )
// 			m_pDLGEventClock->SetShowEnable( true, true );
// 
// 		float fTime = g_pData->GetMyUser()->GetSelectUnit()->GetMarketingEventTime() * 60.f - (float)g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().elapsed();
// 
// 		WCHAR wszText[64] = L"";
// 		int iMinute = (int)( (fTime+59.f)/ 60.f);
// 		if( iMinute < 0 )
// 			iMinute = 0;
// 		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%2d", iMinute );
// 
// 		CKTDGUIStatic* pStatic_String = (CKTDGUIStatic*) m_pDLGEventClock->GetControl( L"EVENT_COUNT" );
// 		if( NULL != pStatic_String )
// 			pStatic_String->GetString(0)->msg = wszText;
// 
// 	}
// 
// 
// 	if( NULL != m_pDLGEventClock )
// 	{
// 		if( false == m_pDLGEventClock->GetShow() )
// 		{
// 			if( NULL != m_pDLGEventClockPopup )
// 			{
// 				CloseGuestUserEventTimerPopup();
// 			}
// 		}
// 	}
// }
// 
// 
// void CX2State::OpenGuestUserEventTimerPopup()
// {
// 	if( NULL == m_pDLGEventClockPopup )
// 	{
// 		m_pDLGEventClockPopup = new CKTDGUIDialog( this, L"DLG_Event_Clock_Talk_Box.lua" );	
// 		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEventClockPopup );
// 	}
// 
// 	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGEventClockPopup, true );
// 	m_pDLGEventClockPopup->SetShowEnable( true, true );
// 
// 
// 	//float fTime = 1800.f - (float)g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().elapsed();
// 	WCHAR wszText[64] = L"";
// 	//int iMinute = (int)(fTime / 60.f);
// 	
// 	CKTDGUIStatic* pStatic_Clock = (CKTDGUIStatic*) m_pDLGEventClock->GetControl( L"EVENT_COUNT" );
// 	wsprintf( wszText, L"[�̺�Ʈť��] ����\n������ %s��.", pStatic_Clock->GetString(0)->msg.c_str() );
// 
// 	CKTDGUIStatic* pStatic_String = (CKTDGUIStatic*) m_pDLGEventClockPopup->GetControl( L"EVENT_CLOCK_TALK_BOX" );
// 	if( NULL != pStatic_String )
// 		pStatic_String->GetString(0)->msg = wszText;
// 
// 
// }
// 
// 
// void CX2State::CloseGuestUserEventTimerPopup() 
// {
// 	if( NULL != m_pDLGEventClockPopup )
// 	{
// 		m_pDLGEventClockPopup->SetShowEnable( false, false );
// 	}
// }

bool CX2State::Handler_EGS_GUEST_USER_LIMIT_LEVEL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->SetGuestUserReachedLevelLimit( true );
	}

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_START_UP:
	case CX2Main::XS_LOGIN:
	case CX2Main::XS_LOADING:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
//	case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_CREATE_UNIT:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_TRAINING_RESULT:
		{
		} break;

	//case CX2Main::XS_PVP_ROOM:
	//case CX2Main::XS_DUNGEON_ROOM:
	//	{
	//		// fix!! ���� ���¸� Ǯ�� ??
	//	} break;

	default:
		//case CX2Main::XS_PVP_LOBBY:
		//case CX2Main::XS_PVP_ROOM:
		//case CX2Main::XS_DUNGEON_ROOM:
		//case CX2Main::XS_VILLAGE_MAP:
		//case CX2Main::XS_LOCAL_MAP:
		//case CX2Main::XS_WORLD_MAP:
		//case CX2Main::XS_SHOP:
		//case CX2Main::XS_NPC_HOUSE:
		//case CX2Main::XS_SQUARE_GAME:
		//case CX2Main::XS_TRAINING_SCHOOL:
		//case CX2Main::XS_ALCHEMIST_HOUSE:
		{
			m_pDLGGuestUserGoUnitSelect = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_544 ), this, SUCM_GUEST_USER_GOTO_UNIT_SELECT );
		} break; 
	}

	return true;

}

bool CX2State::Handler_EGS_NEW_PRESENT_CASH_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_PRESENT_CASH_INVENTORY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->SetArrivedPresentNum( kEvent.m_iPresentCount );
	g_pMain->GetInformerManager()->GetCashShopInformer()->SetNeedCheckInfo( true );

	return true;
}

bool CX2State::Handler_EGS_PRESENT_MESSAGE_TO_RECEIVER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->PushPresentArrival( kEvent );

	if ( g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true )
	{
#ifdef SERV_GLOBAL_BILLING
		g_pData->GetCashShop()->Handler_EGS_BILL_INVENTORY_INQUIRY_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#else // SERV_GLOBAL_BILLING
		g_pData->GetCashShop()->Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( 1, CASH_SHOP_DEPOSIT_SLOT_NUM );
#endif // SERV_GLOBAL_BILLING
	}

	return true;
}

//bool CX2State::Handler_EGS_GET_POST_LETTER_LIST_REQ()
//{
//	g_pData->GetServerProtocol()->SendID( EGS_GET_POST_LETTER_LIST_REQ );
//	g_pMain->AddServerPacket( EGS_GET_POST_LETTER_LIST_ACK );
//
//	return true;
//}
//
//bool CX2State::Handler_EGS_GET_POST_LETTER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
//	KEGS_GET_POST_LETTER_LIST_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if( g_pMain->DeleteServerPacket( EGS_GET_POST_LETTER_LIST_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pData->SetPostItemMsg( kEvent.m_vecMessage );
//
//			if( true == kEvent.m_mapRecommendRewardItem.empty() &&
//				true == kEvent.m_mapTutorRewardItem.empty() &&
//				true == kEvent.m_mapEventRewardItem.empty() )
//			{
//				if( CX2Main::XS_SERVER_SELECT == g_pMain->GetNowStateID() )
//				{
//					// �ƹ��� ���� ������ ����, ������ �ٷ� ����~
//					CX2StateServerSelect* pState = (CX2StateServerSelect*) g_pMain->GetNowState();
//					pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//				}
//				return true;
//			}
//
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//			// ��õ�� ����
//			if( false == kEvent.m_mapRecommendRewardItem.empty() )
//			{
//				std::map< int, int >::iterator i;
//
//				int nCount = 0;
//				wstring getExplanation = L"��õ�� �������� ";
//				wstring itemName;
//				wstringstream buf;
//				for ( i = kEvent.m_mapRecommendRewardItem.begin(); i != kEvent.m_mapRecommendRewardItem.end(); i++ )
//				{
//					int resultItemID = i->first;
//
//					CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//					if ( pItemTemplet != NULL )
//						itemName = pItemTemplet->GetFullName_();
//
//					if ( nCount > 0 )
//					{
//						getExplanation += L", ";
//					}
//
//					buf.str( L"" );
//					buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//					getExplanation += buf.str().c_str();
//
//					nCount++;
//				}
//
//				buf.str( L"" );
//				buf << L"�� ������ϴ�!";
//
//				getExplanation += buf.str().c_str();
//
//				m_pDLGRecommendReward = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), getExplanation.c_str(), this );
//
//				m_bCheckPostItem = true;
//			}
//
//
//			// ���� ����
//			if( false == kEvent.m_mapTutorRewardItem.empty() )
//			{
//				std::map< int, int >::iterator i;
//
//				int nCount = 0;
//				wstring getExplanation = L"���ڰ� �������Ͽ� ��������";
//				wstring itemName;
//				wstringstream buf;
//				for ( i = kEvent.m_mapTutorRewardItem.begin(); i != kEvent.m_mapTutorRewardItem.end(); i++ )
//				{
//					int resultItemID = i->first;
//
//					CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//					if ( pItemTemplet != NULL )
//						itemName = pItemTemplet->GetFullName_();
//
//					if ( nCount > 0 )
//					{
//						getExplanation += L", ";
//					}
//
//					buf.str( L"" );
//					buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//					getExplanation += buf.str().c_str();
//
//					nCount++;
//				}
//
//				buf.str( L"" );
//				buf << L"�� ������ϴ�!";
//
//				getExplanation += buf.str().c_str();
//
//				m_pDLGTutorReward = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), getExplanation.c_str(), this );
//
//				m_bCheckPostItem = true;
//			}
//
//			// ������ �̺�Ʈ ���� ó��
//			if( false == kEvent.m_mapEventRewardItem.empty() )
//			{
//
//				std::map< int, int >::iterator i;
//
//				int nCount = 0;
//				wstring getExplanation = L"�̺�Ʈ ��������";
//				wstring itemName;
//				wstringstream buf;
//				for ( i = kEvent.m_mapEventRewardItem.begin(); i != kEvent.m_mapEventRewardItem.end(); i++ )
//				{
//					int resultItemID = i->first;
//
//					CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//					if ( pItemTemplet != NULL )
//						itemName = pItemTemplet->GetFullName_();
//
//					if ( nCount > 0 )
//					{
//						getExplanation += L", ";
//					}
//
//					buf.str( L"" );
//					buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//					getExplanation += buf.str().c_str();
//
//					nCount++;
//				}
//
//				buf.str( L"" );
//				buf << L"�� ������ϴ�!";
//
//				getExplanation += buf.str().c_str();
//
//				m_pDLGGuestUserReward = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), getExplanation.c_str(), this );
//
//				m_bCheckPostItem = true;
//			}
//
//			return true;
//		}
//		else if ( kEvent.m_iOK == NetError::ERR_REWARD_TO_POST_01 )
//		{
//			if( true == kEvent.m_mapRecommendRewardItem.empty() &&
//				true == kEvent.m_mapTutorRewardItem.empty() &&
//				true == kEvent.m_mapEventRewardItem.empty() )
//			{
//				if( CX2Main::XS_SERVER_SELECT == g_pMain->GetNowStateID() )
//				{
//					// ����!! �ϴ� ������ ����~
//					CX2StateServerSelect* pState = (CX2StateServerSelect*) g_pMain->GetNowState();
//					pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//				}
//				return true;
//			}
//
//			if ( kEvent.m_mapRecommendRewardItem.empty() == false )
//			{
//				std::map< int, int >::iterator i;
//
//				int nCount = 0;
//				wstring getExplanation = L"����ǰ ������ �����Ͽ� ��õ�� �������� ";
//				wstring itemName;
//				wstringstream buf;
//				for ( i = kEvent.m_mapRecommendRewardItem.begin(); i != kEvent.m_mapRecommendRewardItem.end(); i++ )
//				{
//					int resultItemID = i->first;
//
//					CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//					if ( pItemTemplet != NULL )
//						itemName = pItemTemplet->GetFullName_();
//
//					if ( nCount > 0 )
//					{
//						getExplanation += L", ";
//					}
//
//					buf.str( L"" );
//					buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//					getExplanation += buf.str().c_str();
//
//					nCount++;
//				}
//
//				buf.str( L"" );
//				buf << L"�� ���� ���߽��ϴ�.";
//
//				getExplanation += buf.str().c_str();
//
//				m_pDLGRecommendReward = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), getExplanation.c_str(), this );
//
//				m_bCheckPostItem = true;
//			}
//
//
//			if( kEvent.m_mapTutorRewardItem.empty() == false )
//			{
//				std::map< int, int >::iterator i;
//
//				int nCount = 0;
//				wstring getExplanation = L"����ǰ ������ �����Ͽ� �������� �������� ";
//				wstring itemName;
//				wstringstream buf;
//				for ( i = kEvent.m_mapTutorRewardItem.begin(); i != kEvent.m_mapTutorRewardItem.end(); i++ )
//				{
//					int resultItemID = i->first;
//
//					CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//					if ( pItemTemplet != NULL )
//						itemName = pItemTemplet->GetFullName_();
//
//					if ( nCount > 0 )
//					{
//						getExplanation += L", ";
//					}
//
//					buf.str( L"" );
//					buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//					getExplanation += buf.str().c_str();
//
//					nCount++;
//				}
//
//				buf.str( L"" );
//				buf << L"�� ���� ���߽��ϴ�.";
//
//				getExplanation += buf.str().c_str();
//
//				m_pDLGTutorReward = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), getExplanation.c_str(), this );
//
//				m_bCheckPostItem = true;
//			}
//
//
//			// ������ �̺�Ʈ ���� ó��
//			if( kEvent.m_mapEventRewardItem.empty() == false )
//			{
//				std::map< int, int >::iterator i;
//
//				int nCount = 0;
//				wstring getExplanation = L"����ǰ ������ �����Ͽ� �̺�Ʈ �������� ";
//				wstring itemName;
//				wstringstream buf;
//				for ( i = kEvent.m_mapEventRewardItem.begin(); i != kEvent.m_mapEventRewardItem.end(); i++ )
//				{
//					int resultItemID = i->first;
//
//					CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//					if ( pItemTemplet != NULL )
//						itemName = pItemTemplet->GetFullName_();
//
//					if ( nCount > 0 )
//					{
//						getExplanation += L", ";
//					}
//
//					buf.str( L"" );
//					buf << L"[" << itemName.c_str() << L"]" << i->second << L"��";
//
//					getExplanation += buf.str().c_str();
//
//					nCount++;
//				}
//
//				buf.str( L"" );
//				buf << L"�� ���� ���߽��ϴ�.";
//
//				getExplanation += buf.str().c_str();
//
//				m_pDLGGuestUserReward = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), getExplanation.c_str(), this );
//
//				m_bCheckPostItem = true;
//
//			}
//
//		}
//	}
//
//	return false;
//}
//
bool CX2State::Handler_EGS_GET_RANKING_INFO_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_GET_RANKING_INFO_REQ );
	g_pMain->AddServerPacket( EGS_GET_RANKING_INFO_ACK );

	return true;
}

bool CX2State::Handler_EGS_GET_RANKING_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_GET_RANKING_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );



	//if( g_pMain->DeleteServerPacket( EGS_GET_RANKING_INFO_ACK ) == true )


	// fix!! arcade �ڵ� �ٽ� ��������
	//g_pData->GetAracdeChannelData().m_kTopRankingInfo = kEvent.m_kTopRankingInfo;
	//g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo = kEvent.m_vecTodayRankingInfo;

	return true;
}

bool CX2State::Handler_EGS_OPTION_UPDATE_REQ()
{
	KEGS_OPTION_UPDATE_REQ kPacket;

	CX2GameOption::OptionList *pOptionList = &g_pMain->GetGameOption().GetOptionList();

	char fieldLevel;
	
	switch( pOptionList->m_FieldDetail )
	{
	case CX2GameOption::FL_HIGH:
		fieldLevel = KFieldOption::LO_HIGH;
		break;
	case CX2GameOption::FL_MEDIUM:
		fieldLevel = KFieldOption::LO_MIDDLE;
		break;
	case CX2GameOption::FL_LOW:
		fieldLevel = KFieldOption::LO_LOW;
		break;
	default:
		fieldLevel = KFieldOption::LO_LOW;
		break;
	}	

	kPacket.m_kFieldOption.m_sLimitOption = fieldLevel;
//	kPacket.m_kFieldOption.m_bPartyFirst = pOptionList->m_bParty;
//	kPacket.m_kFieldOption.m_bFriendFirst = pOptionList->m_bFriend;

	g_pData->GetServerProtocol()->SendPacket( EGS_OPTION_UPDATE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_OPTION_UPDATE_ACK );

	return true;
}

bool CX2State::Handler_EGS_OPTION_UPDATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_OPTION_UPDATE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_OPTION_UPDATE_ACK );
	return true;
}

bool CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ(int iVillageId)
{
	KEGS_STATE_CHANGE_FIELD_REQ kPacket;

	int iMapId = 0;
	D3DXVECTOR3 vStartPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	int iStartPosLineIndex = 0;
		
	if(iVillageId <= 0)
	{	
		// Ʃ�丮�� ������ �ʱ� ����
		if( g_pMain->GetIsExitingTutorial() == true && 
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP <= 0 &&
			g_pInstanceData != NULL && g_pInstanceData->GetFirstJoinVillage() == true)
		{
			iMapId = SEnum::VMI_RUBEN;
			vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(1);
		}
		else
		{
			const int iVillageIdBeforeDungeonStart = g_pData->GetLocationManager()->GetVillageIdBeforeDungeonStart();

			if ( 0 != iVillageIdBeforeDungeonStart )
			{
				iMapId = iVillageIdBeforeDungeonStart;
				vStartPos = g_pData->GetLocationManager()->GetPositionInVillageBeforeDungeonStart();
				iStartPosLineIndex = g_pData->GetLocationManager()->GetLastTouchedIndexInVillageBeforeDungeonStart();
			}
			else
			{
				iMapId = g_pData->GetLocationManager()->GetCurrentVillageID();
				vStartPos = g_pData->GetLocationManager()->GetLastPos();
				iStartPosLineIndex = g_pData->GetLocationManager()->GetLastPosLineIndex();
			}


			if( NULL == g_pData->GetLocationManager()->GetVillageMapTemplet((SEnum::VILLAGE_MAP_ID)iMapId) )
			{
				// ���� �纥�̵�
				iMapId = SEnum::VMI_RUBEN;
			}	
		}

		if(g_pInstanceData != NULL)
			g_pInstanceData->SetFirstJoinVillage(false);

		kPacket.m_bUseWarpItem = false;
	}
	else
	{
		// ������ ������ �̵�
		iMapId = iVillageId;
		vStartPos = g_pData->GetLocationManager()->GetWarpPos(iMapId);
		if( D3DXVECTOR3( 0, 0, 0 ) == vStartPos ) // warp position�� ���� ����̴�. ��, ������ ������ �̵��ϴ� ��찡 �ƴ� ���, ���� ��� ���ͼ��� �׾��� �� ������ ƨ���
		{
			SEnum::VILLAGE_MAP_ID eTempVillageID = SEnum::VMI_INVALID;
			g_pData->GetLocationManager()->GetVillage( eTempVillageID, vStartPos );
		}

		if(g_pData != NULL)
		{
			// ���� ������ ���� ui���� �ݴ´�.
			g_pData->GetUIManager()->CloseAll();			
		}

		kPacket.m_bUseWarpItem = true;	/// ������ �̿��� �̵�
	}

	kPacket.m_iMapID =  (int)iMapId;
	//kPacket.m_iStartPosition = 1;	
	//kPacket.m_vPos.x = vStartPos.x;
	//kPacket.m_vPos.y = vStartPos.y;
	//kPacket.m_vPos.z = vStartPos.z;

	g_pData->GetLocationManager()->SetVillage( (SEnum::VILLAGE_MAP_ID)kPacket.m_iMapID, vStartPos, iStartPosLineIndex );


	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_FIELD_ACK );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_545 ), this );	

	return true;
}



bool CX2State::Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_FIELD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_FIELD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifndef REFORM_ENTRY_POINT
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );
#endif //REFORM_ENTRY_POINT

			bool bCanCreateWorld = true;
			CX2World::WORLD_ID	eWorldId =  CX2World::WI_NONE;
			SEnum::VILLAGE_MAP_ID eVillageId = SEnum::VMI_INVALID;
			D3DXVECTOR3 vPos;	

			// ���� ������ village�� �����Ѵ�.	
			if(g_pData != NULL && g_pData->GetLocationManager() != NULL)
			{
				g_pData->GetLocationManager()->GetVillage(eVillageId, vPos);
				eWorldId = (CX2World::WORLD_ID)g_pData->GetLocationManager()->GetWorldID(eVillageId);		
			}
			else
			{
				// ������ ������ �� ����.
				bCanCreateWorld = false;
			}

			if(eWorldId <= CX2World::WI_NONE || eWorldId >= CX2World::WI_END)
			{
				// ������ ������ �� ����
				bCanCreateWorld = false;
			}

			// ���� ����		
			if(bCanCreateWorld == true)
			{
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, NULL, false );
			}
		}
		else
		{
			switch ( kEvent.m_iOK )
			{
			case NetError::ERR_FIELD_05:
				{
					if( NULL == m_pDLGStateChangePopup )
					{
						m_pDLGStateChangePopup = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING((STR_ID_18892, "i",kEvent.m_iLimitLevel)) , g_pMain->GetNowState());
					}
				} break;
			case NetError::ERR_FIELD_06:
				{
					if( NULL == m_pDLGStateChangePopup )
					{
						if( NULL != g_pData && NULL != g_pData->GetDungeonManager() )
						{
							wstring DungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( static_cast<SEnum::DUNGEON_ID>(kEvent.m_iRequireDungeonID));
							m_pDLGStateChangePopup = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 	GET_REPLACED_STRING((STR_ID_18893, "L", DungeonName )),	g_pMain->GetNowState());
						}
					}
				} break;
			}

			if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
			{
				CX2StateBattleField* pStateBattleField = static_cast<CX2StateBattleField*>( g_pMain->GetNowState() );
				pStateBattleField->SetNowMovingToOtherPlace(true);
			}

			// ������ġ�� ����			
			g_pData->GetLocationManager()->RestoreVillage();
			if(g_pTFieldGame != NULL)
				g_pTFieldGame->SetEnableKeyProcess(true);
		}

	}
	return true;
}

bool CX2State::Handler_EGS_SEARCH_UNIT_REQ( const WCHAR* wszNickName )
{
	KEGS_SEARCH_UNIT_REQ kPacket;
	kPacket = wszNickName;

#if defined(SEARCH_UNIT_NICKNAME_TW)
	if ( g_pMain->GetStringFilter()->CheckIsUseableUnicodeCharacterNickname( wszNickName ) == false )
#elif defined(SEARCH_UNIT_NICKNAME_JP)
	if ( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wszNickName ) == false )
#elif defined(SEARCH_UNIT_NICKNAME_CN)
	if ( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wszNickName ) == false )
#else
	if ( g_pMain->GetStringFilter()->CheckIsKoreanNEnglishNNumber( wszNickName ) == false )
#endif
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_546 ), this );
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_SEARCH_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SEARCH_UNIT_ACK );

	return true;

	return true;
}

bool CX2State::Handler_EGS_SEARCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SEARCH_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_SEARCH_UNIT_ACK ) == true )
	{
#ifdef SERV_INTEGRATION
		//{{ oasis907 : ����� [2010.5.19] // ���� ���� ������ ����
		if ( kEvent.m_iOK == NetError::ERR_SEARCH_UNIT_03 || kEvent.m_iOK == NetError::ERR_SEARCH_UNIT_04)
		{
			wstring wstrServerName = L"";
#ifdef SERVER_GROUP_UI_ADVANCED // 2012.07.09 lygan_������ // ������ Ȯ�� �����ϰ� �߰�
			wstrServerName = g_pInstanceData->GetServerGroupName();
#else //SERVER_GROUP_UI_ADVANCED
			switch(g_pInstanceData->GetServerGroupID())
			{
			case SGI_SOLES:
				wstrServerName = GET_STRING( STR_ID_4836 );
				break;
			case SGI_GAIA:
				wstrServerName = GET_STRING( STR_ID_4835 );
				break;
			default:
				wstrServerName = GET_STRING( STR_ID_5131 );
				break;
			}
#endif SERVER_GROUP_UI_ADVANCED			
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_5132, "L", wstrServerName ) ), g_pMain->GetNowState() );
			return true;
		}
		//}}
#endif SERV_INTEGRATION
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// 09.04.20 �¿�
			g_pData->GetMessenger()->OpenSearchFriendResultWindow( kEvent );

			// user ������ ã�Ҵ� 
			//m_wstrFindUserName = kEvent.m_kCUnitInfo.m_wstrCharName;
			//m_FindUserUid = kEvent.m_kCUnitInfo.m_iUnitUID;

			//m_iSearchUnitChannelID = (int) kEvent.m_kCUnitInfo.m_uiChannelID;			// fix!!

			//m_iSearchUnitDungeonID = (int) kEvent.m_kCUnitInfo.m_iDungeonID;
			//m_iSearchUnitRoomUID = kEvent.m_kCUnitInfo.m_iRoomUID;


			// fix!!
			//m_eSearchUnitConnectionState = (CX2Unit::CONNECT_UNIT_STATE) kEvent.m_kCUnitInfo.m_cConnectionState;

			//SAFE_DELETE_DIALOG( m_pDLGUserInfo );
			//m_pDLGUserInfo = new CKTDGUIDialog( this, L"DLG_User_Info.lua" );
			//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUserInfo );

			//UpdateUserInfoUI(kEvent);			
		}
		else
		{
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"������ ã�� ���߽��ϴ�.", this );
		}

		//{{ 2007. 11. 13  ������  �Ⱦ��� �������
		//g_pData->GetServerProtocol()->SendID( EGS_CLOSE_WND_FINDUSER_NOT );
		//}}		

		return true;
	}

	return false;
}

#ifdef SERV_LOCAL_RANKING_SYSTEM
bool CX2State::Handler_EGS_LOCAL_RANKING_WATCH_UNIT_REQ( UidType _iUnitUID )
{
	KEGS_LOCAL_RANKING_WATCH_UNIT_REQ kPacket;
	kPacket.m_iUnitUID = _iUnitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK );

	return true;
}

bool CX2State::Handler_EGS_LOCAL_RANKING_WATCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_LOCAL_RANKING_WATCH_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( NULL != g_pData->GetProfileManager() )
			{
				CX2ProfileManager::Profile* pProfile = g_pData->GetProfileManager()->GetProfile();
				if( NULL != pProfile )
				{
					pProfile->area			= kEvent.m_kUserInfo.m_iCategory;
					pProfile->birthYear		= kEvent.m_kUserInfo.m_iBirth;
					pProfile->filter		= kEvent.m_kUserInfo.m_byteFilter;
					pProfile->gender		= kEvent.m_kUserInfo.m_byteGender;
					pProfile->introduce		= kEvent.m_kUserInfo.m_wstrProfile;
					pProfile->level			= kEvent.m_kUnitInfo.m_ucLevel;
					pProfile->nickname		= kEvent.m_kUnitInfo.m_wstrNickName;
					pProfile->UnitClass		= kEvent.m_kUnitInfo.m_cUnitClass;

					g_pData->GetProfileManager()->OpenProfile( false );

					return true;
				}
			}
		}
	}
	return false;
}
#endif //SERV_LOCAL_RANKING_SYSTEM

bool CX2State::Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( const WCHAR* wszNickName )
{
	KEGS_GET_CONNECTION_UNIT_INFO_REQ kPacket;
	kPacket = wszNickName;

#if defined(SEARCH_UNIT_NICKNAME_TW)
	if ( g_pMain->GetStringFilter()->CheckIsUseableUnicodeCharacterNickname( wszNickName ) == false )
#elif defined(SEARCH_UNIT_NICKNAME_JP)
	if ( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wszNickName ) == false )
#elif defined(SEARCH_UNIT_NICKNAME_CN)
	if ( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wszNickName ) == false )
#else
	if ( g_pMain->GetStringFilter()->CheckIsKoreanNEnglishNNumber( wszNickName ) == false )
#endif
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_546 ), this );
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_CONNECTION_UNIT_INFO_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK );

	return true;
}

bool CX2State::Handler_EGS_GET_CONNECTION_UNIT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_CONNECTION_UNIT_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// user ������ ã�Ҵ� 
			//m_wstrFindUserName = kEvent.m_kCUnitInfo.m_wstrCharName;
			//m_FindUserUid = kEvent.m_kCUnitInfo.m_iUnitUID;

			//m_iSearchUnitChannelID = (int) kEvent.m_kCUnitInfo.m_uiChannelID;			// fix!!

			//m_iSearchUnitDungeonID = (int) kEvent.m_kCUnitInfo.m_iDungeonID;
			//m_iSearchUnitRoomUID = kEvent.m_kCUnitInfo.m_iRoomUID;


			// fix!!
			//m_eSearchUnitConnectionState = (CX2Unit::CONNECT_UNIT_STATE) kEvent.m_kCUnitInfo.m_cConnectionState;

			SAFE_DELETE_DIALOG( m_pDLGUserInfo );
#ifdef SERV_PVP_NEW_SYSTEM
			m_pDLGUserInfo = new CKTDGUIDialog( this, L"DLG_User_Info_Ver1.lua" );
#else
			m_pDLGUserInfo = new CKTDGUIDialog( this, L"DLG_User_Info.lua" );
#endif
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUserInfo );

			UpdateUserInfoUI(kEvent);

		}
		else
		{
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"������ ã�� ���߽��ϴ�.", this );
		}

		//{{ 2007. 11. 13  ������  �Ⱦ��� �������
		//g_pData->GetServerProtocol()->SendID( EGS_CLOSE_WND_FINDUSER_NOT );
		//}}		

		return true;
	}

	return false;
}

void CX2State::UpdateUserInfoUI( KEGS_SEARCH_UNIT_ACK &kEvent )
{

	if( m_pDLGUserInfo == NULL )
		return;


	CKTDGUIStatic* pStatic_UnitFace = (CKTDGUIStatic*) m_pDLGUserInfo->GetControl( L"Static_UnitFace" );
	CKTDGUIStatic* pStatic_UnitInfo = (CKTDGUIStatic*) m_pDLGUserInfo->GetControl( L"Static_User_Info" );
	WCHAR wszText[256] = L"";


	wstring fileName;
	wstring pieceName;
	if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS)kEvent.m_kCUnitInfo.m_cUnitClass , CX2Data::CIT_20by20 ) )
	{
		pStatic_UnitFace->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
	}
	else
	{
		ASSERT( !"NO" );
	}


	

	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d", (int)kEvent.m_kCUnitInfo.m_ucLevel );
	pStatic_UnitInfo->GetString(0)->msg = wszText;

	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%s", kEvent.m_kCUnitInfo.m_wstrCharName.c_str() );
	pStatic_UnitInfo->GetString(1)->msg = wszText;

#ifdef SERV_PVP_NEW_SYSTEM
	// ��ũ����	
	pStatic_UnitInfo->GetString(2)->msg = g_pMain->GetEDString( (int)kEvent.m_kCUnitInfo.m_iRPoint );
	// ���� 
	pStatic_UnitInfo->GetString(3)->msg = GET_REPLACED_STRING( ( STR_ID_669, "i", kEvent.m_kCUnitInfo.m_iWin ) );
#else
	// ���� ����
	pStatic_UnitInfo->GetString(2)->msg = GET_REPLACED_STRING( ( STR_ID_547, "i", kEvent.m_kCUnitInfo.m_uiPVPRanking ) );

	// ���� 
	pStatic_UnitInfo->GetString(3)->msg = GET_REPLACED_STRING( ( STR_ID_548, "ii", kEvent.m_kCUnitInfo.m_iWin, kEvent.m_kCUnitInfo.m_iLose ) );
#endif
// 
// 	// �·� 
// 	if ( (kEvent.m_kCUnitInfo.m_iLose + kEvent.m_kCUnitInfo.m_iWin ) == 0 )
// 	{
// 		pStatic_UnitInfo->GetString(5)->msg = L"";
// 	}
// 	else
// 	{
// 		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"�·�:%.1f%%", (float)(kEvent.m_kCUnitInfo.m_iWin / ((float)kEvent.m_kCUnitInfo.m_iLose + kEvent.m_kCUnitInfo.m_iWin) * 100.0f ) );
// 		pStatic_UnitInfo->GetString(5)->msg = wszText;
// 	}



	//���� ǥ�� ������ϴµ� �����ʿ��� ���� �۾��� �� �Ǿ���.
	CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)m_pDLGUserInfo->GetControl( L"UnitEmblem" );
	if ( pStaticEmblem != NULL && pStaticEmblem->GetPicture(0) != NULL )
	{
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
		char cRank = kEvent.m_kCUnitInfo.m_cRank;

		CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( cRank ) );
			
		if ( pPVPEmblemData != NULL )
		{
			pStaticEmblem->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
		}
#else
		if ( g_pMain->GetPVPEmblem()->GetPVPEmblemData( kEvent.m_kCUnitInfo.m_iRating ) != NULL )
		{
			pStaticEmblem->GetPicture(0)->SetTex( g_pMain->GetPVPEmblem()->GetPVPEmblemData( kEvent.m_kCUnitInfo.m_iRating )->m_TextureName.c_str(),
				g_pMain->GetPVPEmblem()->GetPVPEmblemData( kEvent.m_kCUnitInfo.m_iRating )->m_TextureKey.c_str() );
		}
#endif
		
#else
		if ( g_pMain->GetPVPEmblem()->GetPVPEmblemData( (CX2PVPEmblem::PVP_EMBLEM)kEvent.m_kCUnitInfo.m_iPVPEmblem ) != NULL )
		{
			pStaticEmblem->GetPicture(0)->SetTex( g_pMain->GetPVPEmblem()->GetPVPEmblemData( (CX2PVPEmblem::PVP_EMBLEM)kEvent.m_kCUnitInfo.m_iPVPEmblem )->m_TextureName.c_str(),
				g_pMain->GetPVPEmblem()->GetPVPEmblemData( (CX2PVPEmblem::PVP_EMBLEM)kEvent.m_kCUnitInfo.m_iPVPEmblem )->m_TextureKey.c_str() );
		}
#endif

	}


#ifdef SERV_INTEGRATION
	CKTDGUIStatic* pStaticServerGroup = (CKTDGUIStatic*)m_pDLGUserInfo->GetControl( L"Static_SERVER" );

#ifdef REMOVE_KR_SERVER_TEXTURE
	if( pStaticServerGroup != NULL )
		pStaticServerGroup->SetShow( false );
#else REMOVE_KR_SERVER_TEXTURE
	SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;

// 	switch( g_pMain->GetNowStateID() )
// 	{
// 	case CX2Main::XS_DUNGEON_ROOM:
// 		{
			eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(kEvent.m_kCUnitInfo.m_iUnitUID);

			if( eServerGroupID == SGI_SOLES)
			{
				pStaticServerGroup->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Soles");
			}
			else if( eServerGroupID == SGI_GAIA)
			{
				pStaticServerGroup->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Gaia");
			}
			else
			{
				;
			}
// 		}
// 		pStaticServerGroup->SetShow(true);
// 		break;
// 	default:
// 		pStaticServerGroup->SetShow(false);
// 		break;
//	}
#endif REMOVE_KR_SERVER_TEXTURE
#endif SERV_INTEGRATION

	// �����ϱ� ��ư 
	//CKTDGUIButton* pButton_PlayTogether = (CKTDGUIButton*) m_pDLGUserInfo->GetControl( L"Button_Play_Together" );


	// fix!!!!! ���� ��Ȳ
	//switch( kEvent.m_kCUnitInfo.m_cConnectionState )
	//{
	//default:
	//case CX2Unit::CUS_NONE:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"�˼� ����";
	//		pButton_PlayTogether->SetEnable( false );
	//		pButton_PlayTogether->SetShow( false );

	//	} break;
	//case CX2Unit::CUS_ONLINE:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"�¶���";
	//		pButton_PlayTogether->SetEnable( false );
	//		pButton_PlayTogether->SetShow( false );

	//	} break;
	//case CX2Unit::CUS_SQUARE:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"����";
	//		pButton_PlayTogether->SetEnable( false );
	//		pButton_PlayTogether->SetShow( false );

	//	} break;
	//case CX2Unit::CUS_PVP_WAIT:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"������ �����";

	//	} break;

	//case CX2Unit::CUS_PVP_PLAY:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"���� ������";

	//	} break;

	//case CX2Unit::CUS_DUNGEON_WAIT:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"������ �����";

	//	} break;

	//case CX2Unit::CUS_DUNGEON_PLAY:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"���� ������";
	//		pButton_PlayTogether->SetEnable( false );
	//		pButton_PlayTogether->SetShow( false );
	//	} break;

	//case CX2Unit::CUS_ARCADE_PLAY:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"�����̵� ������";
	//		pButton_PlayTogether->SetEnable( false );
	//		pButton_PlayTogether->SetShow( false );
	//	} break;

	//case CX2Unit::CUS_ARCADE_WAIT:
	//	{
	//		pStatic_UnitInfo->GetString(6)->msg = L"�����̵� �����";
	//	} break;
	//}

}

// �г��� ���� �����ۻ�� �����ÿ� �޴� ��Ŷ
bool CX2State::Handler_EGS_DELETE_NICK_NAME_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_NickName = L"__DELETED__";

	// ĳ���� ����â���� �̵��Ѵ�.
	CX2StateAutoChanger::TARGET_DETAIL targetDetail;
#ifdef ELSWORD_NEW_BEGINNING
	g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_BEGINNING, targetDetail );
#else
	g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_SERVER_SELECT, targetDetail );
#endif ELSWORD_NEW_BEGINNING
	return true;
}

//{{ 2009. 7. 29  ������	��ŷ ��� �˾�
bool CX2State::Handler_EGS_NEW_RECORD_RANKING_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( g_pInstanceData == NULL )
	{
		ASSERT( !"g_pInstanceData == NULL " );
		return false;
	}
	
	g_pInstanceData->SetRegisteredRank( true );

	return true;
}
//}}

#ifdef CHANGE_CHANNEL

bool CX2State::Handler_EGS_GET_CHANNEL_LIST_REQ()
{
	if( m_pDLGMsgBox == NULL )
	{
		g_pData->GetServerProtocol()->SendID( EGS_GET_CHANNEL_LIST_REQ );
		g_pMain->AddServerPacket( EGS_GET_CHANNEL_LIST_ACK, 10.f );
	
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2695 ), g_pMain->GetNowState() );
	}	
	return true;
}

bool CX2State::Handler_EGS_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_CHANNEL_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_GET_CHANNEL_LIST_ACK ) == true )
	{
#ifdef SERV_CHANNEL_LIST_RENEWAL
		if( kEvent.m_vecChannelInfo.empty() == false )
		{
#ifdef CHANGE_CHANNEL
			g_pInstanceData->m_vecChannelInfo.clear();

			for(UINT i=0; i<kEvent.m_vecChannelInfo.size(); ++i)
			{
				g_pInstanceData->m_vecChannelInfo.push_back(kEvent.m_vecChannelInfo[i]);
			}

			if( kEvent.m_vecChannelInfo.empty() == false )
			{
				g_pInstanceData->m_mapChannelBonusInfo.clear();

				std::map< int, KChannelBonusInfo >::const_iterator cmit;
				for( cmit = kEvent.m_mapChannelBonusInfo.begin() ; cmit != kEvent.m_mapChannelBonusInfo.end(); cmit++ )
				{
					//const KChannelBonusInfo& ChannelBonusInfo = cmit->second;
					if(g_pInstanceData != NULL)
						g_pInstanceData->m_mapChannelBonusInfo.insert(std::make_pair(cmit->first, cmit->second));
				}
			}

			for(UINT i=0; i<kEvent.m_vecChannelInfo.size(); ++i)
			{

				if(g_pInstanceData != NULL)
				{
					for(UINT j=0; j<g_pInstanceData->m_vecChannelInfo.size(); ++j)
					{
						KChannelInfo &channelInfo = g_pInstanceData->m_vecChannelInfo[j];

						if( channelInfo.m_iChannelID == kEvent.m_vecChannelInfo[i].m_iChannelID )
						{
							channelInfo.m_iCurrentUser = kEvent.m_vecChannelInfo[i].m_iCurrentUser;
							channelInfo.m_iMaxUser = kEvent.m_vecChannelInfo[i].m_iMaxUser;
							break;
						}
					}
				}
			}	

			if(g_pInstanceData != NULL && g_pInstanceData->GetMiniMapUI() != NULL)
			{
				g_pInstanceData->GetMiniMapUI()->UpdateChannelInfo();
			}
			return true;
#endif CHANGE_CHANNEL
		}
#else //SERV_CHANNEL_LIST_RENEWAL
		for(UINT i=0; i<kEvent.m_vecChannelList.size(); ++i)
		{
			KChannelSimpleInfo channelSimpleInfo = kEvent.m_vecChannelList[i];
			if(g_pInstanceData != NULL)
			{
				for(UINT j=0; j<g_pInstanceData->m_vecChannelInfo.size(); ++j)
				{
					KChannelInfo &channelInfo = g_pInstanceData->m_vecChannelInfo[i];

					if( channelInfo.m_iChannelID == channelSimpleInfo.m_iChannelID )
					{
						channelInfo.m_iCurrentUser = channelSimpleInfo.m_iCurrentUser;
						channelInfo.m_iMaxUser = channelSimpleInfo.m_iMaxUser;
						break;
					}
				}
			}
		}	

		if(g_pInstanceData != NULL && g_pInstanceData->GetMiniMapUI() != NULL)
		{
			g_pInstanceData->GetMiniMapUI()->UpdateChannelInfo();
		}
		return true;
#endif //SERV_CHANNEL_LIST_RENEWAL
	}

	return false;
}

bool CX2State::Handler_EGS_CHECK_CHANNEL_CHANGE_REQ(int iChannelId)
{
	int ChannelIndx = 0;	

#ifdef SERV_CHANNEL_LIST_RENEWAL
	if (g_pInstanceData->m_vecChannelInfo.empty() == false )
	{
		for(int i = 0; i < static_cast<signed>( g_pInstanceData->m_vecChannelInfo.size() ); i ++)
		{
			if(g_pInstanceData->m_vecChannelInfo[i].m_iChannelID == iChannelId)
			{
				ChannelIndx	= iChannelId;
				break;
			}
		}
	}
#else //SERV_CHANNEL_LIST_RENEWAL
	ChannelIndx = g_pInstanceData->m_vecChannelInfo[iChannelId -1].m_iChannelID;
#endif //SERV_CHANNEL_LIST_RENEWAL

	KEGS_CHECK_CHANNEL_CHANGE_REQ kPacket;
	kPacket.m_iChannelID = ChannelIndx;	

	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_CHANNEL_CHANGE_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, 30.f ); 

	m_iChangeChannelId = iChannelId;

	return true;
}

bool CX2State::Handler_EGS_CHECK_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_CHANNEL_CHANGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHECK_CHANNEL_CHANGE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// ������ ���´�. (gameserver/tcp reply)
			Handler_EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ();
			return true;
		}
		else
		{
			m_iChangeChannelId = 0;
			return false;
		}
	}

	return false;
}

bool CX2State::Handler_EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ()
{
	if( 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol() != NULL
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() != NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->UserProxy_SetIntendedDisconnect( true );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy()->SetIntendedDisconnect( true );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

	g_pData->GetServerProtocol()->SendID( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ );
	g_pMain->AddServerPacket( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK, 30.0f );
	//SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	//m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2668 ), this );

	return true;
}

bool CX2State::Handler_EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pMain->DeleteServerPacket( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK ) == true )
	{		
		//m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK = false;

		g_pData->GetServerProtocol()->DisconnectFromGameServer();

		if( NULL != g_pData->GetServerProtocol() )
		{
			g_pData->GetServerProtocol()->SendTRID( ETR_UNIT_DISCONNECT );
		}
		g_pData->GetServerProtocol()->DisconnectFromTRServer();

		SAFE_DELETE_DIALOG( m_pDLGMsgBox );

		g_pMain->SetConnectedGameServerIP( L"" );
		g_pMain->SetConnectedGameServerPort( 0 );

		Handler_EGS_CONNECT_CHANNEL_CHANGE_REQ();	

#ifdef BUFF_TEMPLET_SYSTEM
		//�ɸ��� ����â ���� �� ���� ������ �ʱ�ȭ
		BOOST_FOREACH( CX2PremiumBuffIconPtr pPremiumBuffIconPtr, g_pData->GetPremiumBuffManager()->GetPremiumBuffList() )
		{
			g_pData->GetPremiumBuffManager()->ErasePremiumBuff(static_cast<CX2PremiumBuffTemplet::BUFF_ID>( pPremiumBuffIconPtr->GetPremiumBuffID() ));
		}
		g_pData->GetPremiumBuffManager()->GetPremiumBuffList().resize(0);
#endif BUFF_TEMPLET_SYSTEM

		return true;
	}

	m_iChangeChannelId = 0;
	return false;
}

bool CX2State::Handler_EGS_CONNECT_CHANNEL_CHANGE_REQ()
{	
	if(m_iChangeChannelId == 0)
		return false;

	// game server�� �����ϱ����� nexon passport�� ���Ź޾Ƽ� �����Ѵ�.
	if( false == g_pMain->GetManualLogin() )
	{
		switch( g_pMain->GetPublisher() )
		{
#ifdef _NEXON_KR_
		case CX2Main::XP_NEXON_KOREA:
			{
				WCHAR passPortStr[PASSPORT_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNexonPassport( passPortStr );
				const WCHAR* pLoginID = CNMCOClientObject::GetInstance().GetMyLoginId();

				if( 0 == wcslen( passPortStr ) && 
					0 != g_pNexonPassport.length() )
				{
					g_pInstanceData->SetUserPassword( g_pNexonPassport.c_str() );	// ü�� ���̵��� ���
				}
				else
				{
					g_pInstanceData->SetUserPassword( passPortStr );		// �Ϲ� �ؽ� ������ ���
				}

				g_pInstanceData->SetUserID( pLoginID );

				//{{ kimhc // 2010-06-24 // OTP �۾�
			#ifdef SERV_OTP_AUTH
				// ������ �ϵ���� ID�� ȹ��
				WCHAR lpNxHWID[NXHWID_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNxHWID( lpNxHWID );
				g_pInstanceData->SetNxHWID( lpNxHWID );

// 				INT32 nSecurecode = 0;
// 				NMGameTokenReplyCode rtVal = CNMCOClientObject::GetInstance().GetGameToken( NULL, &nSecurecode );
// 				switch ( rtVal )
// 				{
// 				case kGameToken_OK:	// 2�� ���� ��� ����
// 					break;
// 
// 				case kGameToken_RegisteredPC:	// OTP ���
// 					// ������ �ϵ���� ID�� ȹ��
// 					WCHAR lpNxHWID[NXHWID_SIZE]; = { 0, };
// 					CNMCOClientObject::GetInstance().GetNxHWID( lpNxHWID );
// 					break;
// 
// 				default:
// 					ASSERT( !L"GetGameToken is wrong" );
// 					break;
// 					
// 				}
			#endif	SERV_OTP_AUTH
				//}} kimhc // 2010-06-24 // OTP �۾�

			} break;
#endif _NEXON_KR_
		default:
			break;
		}
	}

	
	//���ӽõ�
	//g_pData->ResetServerProtocol();

#ifdef SERV_CHANNEL_LIST_RENEWAL
	wstring wstrGameServerIP = L"";
	int iGameServerPort = 0;

	if ( g_pInstanceData->m_vecChannelInfo.empty() == false )
	{
		for(int i = 0; i < static_cast<signed>( g_pInstanceData->m_vecChannelInfo.size() ); i ++)
		{
			if(g_pInstanceData->m_vecChannelInfo[i].m_iChannelID == m_iChangeChannelId)
			{
				wstrGameServerIP = g_pInstanceData->m_vecChannelInfo[i].m_wstrIP;
				iGameServerPort = g_pInstanceData->m_vecChannelInfo[i].m_usMasterPort;
				break;
			}
		}
	}
		
	ASSERT(wstrGameServerIP != L"" || iGameServerPort != 0 );
#else //SERV_CHANNEL_LIST_RENEWAL
	const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[m_iChangeChannelId-1];

	wstring wstrGameServerIP = channelInfo.m_wstrIP;
	int iGameServerPort = channelInfo.m_usMasterPort;
#endif //SERV_CHANNEL_LIST_RENEWAL

	if( g_pData->GetServerProtocol()->ConnectedToGameServer( wstrGameServerIP.c_str(), iGameServerPort ) == true )
	{
		g_pMain->SetConnectedGameServerIP( wstrGameServerIP );
		g_pMain->SetConnectedGameServerPort( iGameServerPort );

		KEGS_CONNECT_CHANNEL_CHANGE_REQ kPacket;
		kPacket.m_kConnectReq.m_wstrVersion = g_pMain->GetClientVersion();
		
#ifdef CLIENT_PURPLE_MODULE
		bool bUsePurpleModule = true;
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
		if (g_pInstanceData->GetInHouseAccount())
			bUsePurpleModule = false;
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

		if(bUsePurpleModule)
		{
			// 10.01.25 ������ ���� ��� ���̵� ���
			ConvertCharToWCHAR( kPacket.m_kVerifyAccountReq.m_wstrUserID, PurpleGetUserID() );
#ifdef SERV_KOG_OTP_VERIFY
			if (g_pInstanceData->IsIDVerified())
				kPacket.m_kVerifyAccountReq.m_wstrPasswd = g_pInstanceData->GetUserPassword();
			else
				ConvertCharToWCHAR( kPacket.m_kVerifyAccountReq.m_wstrPasswd, PurpleGetAuthString() );
#else //SERV_KOG_OTP_VERIFY
			ConvertCharToWCHAR( kPacket.m_kVerifyAccountReq.m_wstrPasswd, PurpleGetAuthString() );
#endif //SERV_KOG_OTP_VERIFY
		}
#else // CLIENT_PURPLE_MODULE
		kPacket.m_kVerifyAccountReq.m_wstrUserID = g_pInstanceData->GetUserID();
		kPacket.m_kVerifyAccountReq.m_wstrPasswd = g_pInstanceData->GetUserPassword();
#endif // CLIENT_PURPLE_MODULE

#ifdef SERV_COUNTRY_TH
		kPacket.m_kVerifyAccountReq.m_wstrSocketID = g_pInstanceData->GetSocketID();
#endif // SERV_COUNTRY_TH

#ifdef SERV_JAPAN_CHANNELING 
		char szBuffer[256] = {0};

#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
		if( g_pInstanceData->GetInHouseAccount() == false )
		{
#endif CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
			PurpleGetValue("site", szBuffer, 255);
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
		}
		else
		{
			int index = kPacket.m_kVerifyAccountReq.m_wstrUserID.find( L'@' );
			if( index != -1 )
			{
				wstring wstrChannelCode = kPacket.m_kVerifyAccountReq.m_wstrUserID.substr( index+1 , kPacket.m_kVerifyAccountReq.m_wstrUserID.length()-1 );
				string strChannelCode;
				ConvertWCHARToChar( strChannelCode, wstrChannelCode.c_str() );
				sprintf(szBuffer, "%s", strChannelCode.c_str() );
			}
		}
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

		if(strcmp(szBuffer, "MS") == 0)
			kPacket.m_kVerifyAccountReq.m_iChannelingCode = KNexonAccountInfo::JCE_MSN;
		else if(strcmp(szBuffer, "NICO") == 0)
			kPacket.m_kVerifyAccountReq.m_iChannelingCode = KNexonAccountInfo::JCE_NICO;
		else if(strcmp(szBuffer, "NX") == 0)
			kPacket.m_kVerifyAccountReq.m_iChannelingCode = KNexonAccountInfo::JCE_NEXON;
		else
			kPacket.m_kVerifyAccountReq.m_iChannelingCode = KNexonAccountInfo::JCE_NHN;
#endif // SERV_JAPAN_CHANNELING

		//{{ kimhc // 2010-06-24 // OTP �۾�
	#ifdef	SERV_OTP_AUTH
		kPacket.m_kVerifyAccountReq.m_wstrHWID = g_pInstanceData->GetNxHWID();
	#endif	SERV_OTP_AUTH
		//}} kimhc // 2010-06-24 // OTP �۾�

		kPacket.m_kVerifyAccountReq.m_bDebugAuth = g_pMain->GetManualLogin();
		//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
		kPacket.m_kVerifyAccountReq.m_bManualLogin = g_pMain->GetManualLogin();
#endif SERV_KOG_OTP_VERIFY
		//}}

		kPacket.m_kSelectUnitReq.m_iUnitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
#ifdef MACHINE_ID
		kPacket.m_kVerifyAccountReq.m_strMachineID = g_pInstanceData->GetMachineId();
#endif
#endif

#ifdef SERV_CHECK_MACHINE_LOCALTIME		
		CTime tCurTime = CTime::GetCurrentTime();
		kPacket.m_kVerifyAccountReq.m_wstrClientTime = (const wchar_t*)tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif

		g_pData->GetServerProtocol()->SendPacket( EGS_CONNECT_CHANNEL_CHANGE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, 60.0f );
		

		SAFE_DELETE_DIALOG( m_pDLGMsgBox );
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2668 ), this );

		//g_pInstanceData->SetConnectChannelServerID( channelInfo.m_iChannelID );

		// ���� ���������� game_server ip�� �����Ѵ�. (�������Խ� TRServer ���ӿ�)
		if(g_pMain != NULL)
		{
			g_pMain->SetTRServerIp( wstrGameServerIP );
		}				

		return true;
	}
	else
	{
#if 0
#ifdef _SERVICE_
		m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_726 ), this );
#else
		m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_726 ), this );
#endif _SERVICE_
#endif

		g_pData->GetUIManager()->CloseAll();
#ifdef ELSWORD_NEW_BEGINNING
		CX2StateBeginning::m_sbSelectedServerSet = false;
#else
		CX2StateServerSelect::m_sbSelectedServerSet = false;		
#endif ELSWORD_NEW_BEGINNING
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_552 ) );
#ifdef ELSWORD_NEW_BEGINNING
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_BEGINNING, NULL, false );
#else
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_SERVER_SELECT, NULL, false );
#endif ELSWORD_NEW_BEGINNING


		//m_bFailToConnectServer = true;

		return false;
	}

	return true;
}

bool CX2State::Handler_EGS_CONNECT_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CONNECT_CHANNEL_CHANGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
			if( false == kEvent.m_kVerifyAccountAck.m_kAccountInfo.m_wstrOTP.empty() )
			{
				// �������� �߱޹��� OTP�� Ŭ���̾�Ʈ�� �����Ѵ�.
				g_pInstanceData->SetIDVerified(true);
				g_pInstanceData->SetUserPassword( kEvent.m_kVerifyAccountAck.m_kAccountInfo.m_wstrOTP );	
			}
#endif SERV_KOG_OTP_VERIFY
			//}}
			
#ifdef SERV_CHANNEL_LIST_RENEWAL
			if ( g_pInstanceData->m_vecChannelInfo.empty() == false )
			{
				for(int i = 0; i < static_cast<signed>( g_pInstanceData->m_vecChannelInfo.size() ); i ++)
				{
					if(g_pInstanceData->m_vecChannelInfo[i].m_iChannelID == m_iChangeChannelId)
					{
						g_pInstanceData->SetConnectChannelServerID( m_iChangeChannelId );
						g_pMain->SetServerUDPPort( g_pInstanceData->m_vecChannelInfo[i].m_usNCUDPPort );
						break;
					}
				}
			}
#else //SERV_CHANNEL_LIST_RENEWAL
			const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[m_iChangeChannelId-1];
			g_pInstanceData->SetConnectChannelServerID( channelInfo.m_iChannelID );

			g_pMain->SetServerUDPPort( g_pInstanceData->m_vecChannelInfo[m_iChangeChannelId-1].m_usNCUDPPort );	
#endif //SERV_CHANNEL_LIST_RENEWAL
			m_bConnectedGameServer = true;
			Handler_CC_KXPT_PORT_CHECK_REQ();

			if(g_pTFieldGame != NULL)
			{
				g_pTFieldGame->ConnectTR();
			}

#ifndef	CHRISTMAS_TREE
			// �̺�Ʈ �ð� ����
			if(g_pData != NULL)
			{
				double fEventDurantionTime = 0.0f;
				std::map< int, double >::const_iterator mit = kEvent.m_kChangeUserInfo.m_mapEventDurationTime.begin();
				for( ; mit != kEvent.m_kChangeUserInfo.m_mapEventDurationTime.end(); ++mit )
				{
					if( mit->second > fEventDurantionTime )
						fEventDurantionTime = mit->second;
				}

				if( fEventDurantionTime > 0.0f )
				{
					g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().SetTime((long)(fEventDurantionTime * 1000));
				}				
			}
#endif	CHRISTMAS_TREE

#ifdef SERV_EVENT_CHECK_POWER
			g_pMain->GetMemoryHolder()->UpdateCheckPowerEvent();
#endif SERV_EVENT_CHECK_POWER

			Handler_EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ();
		}
		else
		{
			m_bConnectFullChannel = true;

			g_pData->GetServerProtocol()->DisconnectFromGameServer();
			g_pData->GetUIManager()->CloseAll();

#ifdef ELSWORD_NEW_BEGINNING
			CX2StateBeginning::m_sbSelectedServerSet = false;			
#else
			CX2StateServerSelect::m_sbSelectedServerSet = false;			
#endif ELSWORD_NEW_BEGINNING
			//CX2StateAutoChanger::TARGET_DETAIL targetDetail;
			//g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_SERVER_SELECT, targetDetail );

			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_552 ) );
#ifdef ELSWORD_NEW_BEGINNING
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_BEGINNING, NULL, false );
#else
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_SERVER_SELECT, NULL, false );
#endif ELSWORD_NEW_BEGINNING
			//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
			switch( kEvent.m_iOK )
			{
			case NetError::ERR_UNKNOWN		:// OTP Ʋ�� ��� ..
				{
					MessageBox( g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_648 ), L"Quit", MB_OK );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;
			default:
				break;
			}
#endif SERV_KOG_OTP_VERIFY
			//}}

#if 0 
			//g_pData->DeleteServerProtocol();
			//g_pData->DeleteMyUser();
			switch( kEvent.m_iOK )
			{
			default:
				{
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;

			case NetError::ERR_CONNECT_00:
				{
					MessageBox( g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_727 ), L"Quit", MB_OK );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;

			case NetError::ERR_CONNECT_04:
				{
					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�����ο��� ���Ƽ� ���� �� �� �����ϴ�.", this );
				} break;
			}
#endif
		}
	}
	return true;
}

bool CX2State::Handler_EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ()
{
	KEGS_JOIN_FIELD_CHANNEL_CHANGE_REQ kPacket;

	D3DXVECTOR3 vPos = g_pTFieldGame->GetMyUnit()->GetPos();

	kPacket.m_iMapID = g_pData->GetLocationManager()->GetCurrentVillageID();
	kPacket.m_vPos.x = vPos.x;
	kPacket.m_vPos.y = vPos.y + 10.f;
	kPacket.m_vPos.z = vPos.z;

	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK, 60.0f );

	// �ʵ����� ����
	if(g_pTFieldGame != NULL)
		g_pTFieldGame->DeleteAllFieldUser();

	if(g_pTFieldGame != NULL)
	{	
		g_pData->GetUIManager()->CloseAll();	

		g_pTFieldGame->ByeNpc();
		g_pInstanceData->SetChangeChannel(true);
		g_pTFieldGame->OpenFieldName();
		g_pTFieldGame->OpenBuffName();
		g_pTFieldGame->LoadUI();		

		if( g_pData != NULL && g_pData->GetMessenger() != NULL )
			g_pData->GetMessenger()->ResetUpdateList();

		g_pData->GetUIManager()->OnFieldChange();
	}

	return true;
}

bool CX2State::Handler_EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_JOIN_FIELD_CHANNEL_CHANGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pInstanceData->GetMiniMapUI()->UpdateChannelIndex();
			return true;
		}
	}

	return false;
}

bool CX2State::Handler_CC_KXPT_PORT_CHECK_REQ()
{
	m_bPortCheckWait			= true;
	m_fPortCheckWaitTime		= 0.5f;

	KXPT_PORT_CHECK_REQ kXPT_PORT_CHECK_REQ;
	kXPT_PORT_CHECK_REQ.m_UserUID = g_pData->GetMyUser()->GetUID();
	//{{ 2013. 1. 9	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    kXPT_PORT_CHECK_REQ.m_InternalIPAddress = g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	kXPT_PORT_CHECK_REQ.m_wstrInternalIP = g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_PORT_CHECK_REQ.m_usInternalPort = g_pData->GetGameUDP()->GetMyPort();
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    return g_pData->GetGameUDP()->SendToIP( 
        CKTDNUDP::ConvertIPToAddress( g_pMain->GetConnectedGameServerIP() ),
        g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, 
        &kXPT_PORT_CHECK_REQ, sizeof(kXPT_PORT_CHECK_REQ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	KSerBuffer buff;
//	Serialize( &buff, &kXPT_PORT_CHECK_REQ );
//	return g_pData->GetGameUDP()->Send( g_pMain->GetConnectedGameServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//	//return g_pData->GetGameUDP()->Send( g_pMain->GetServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

#ifdef SERV_ID_NETMARBLE_PCBANG
bool CX2State::Handler_CC_KXPT_PUBLIC_IP_CHECK_REQ()
{
	m_bPortCheckWait			= true;
	m_fPortCheckWaitTime		= 0.5f;

	KXPT_PORT_CHECK_REQ kXPT_PORT_CHECK_REQ;
	kXPT_PORT_CHECK_REQ.m_UserUID = 0;
	//{{ 2013. 1. 9	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_PORT_CHECK_REQ.m_InternalIPAddress = g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	//kXPT_PORT_CHECK_REQ.m_wstrInternalIP = g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_PORT_CHECK_REQ.m_usInternalPort = g_pData->GetGameUDP()->GetMyPort();
#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	std::wstring		wsConnectedGameServerIP;
	int					iConnectedGameServerPort = 0;

#ifdef CLIENT_COUNTRY_ID
#ifdef _SERVICE_
#ifdef _OPEN_TEST_
	wsConnectedGameServerIP = L"203.209.190.30";
	iConnectedGameServerPort = 14301;
#else _OPEN_TEST_
	wsConnectedGameServerIP = L"182.16.248.107";
	iConnectedGameServerPort = 14301;
#endif _OPEN_TEST_

#else _SERVICE_
	wsConnectedGameServerIP = L"192.168.71.245";
	iConnectedGameServerPort = 14301;
#endif _SERVICE_
#endif //CLIENT_COUNTRY_ID

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	return g_pData->GetGameUDP()->SendToIP( 
		CKTDNUDP::ConvertIPToAddress( wsConnectedGameServerIP.c_str() ),
		iConnectedGameServerPort, XPT_PORT_CHECK_REQ, 
		&kXPT_PORT_CHECK_REQ, sizeof(kXPT_PORT_CHECK_REQ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	//KSerBuffer buff;
	//Serialize( &buff, &kXPT_PORT_CHECK_REQ );
	//return g_pData->GetGameUDP()->Send( g_pMain->GetConnectedGameServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
	//return g_pData->GetGameUDP()->Send( g_pMain->GetServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}
#endif //SERV_ID_NETMARBLE_PCBANG

bool CX2State::Handler_CC_KXPT_PORT_CHECK_ACK( const KXPT_PORT_CHECK_ACK& kXPT_PORT_CHECK_ACK )
{
	dbg::clog << L"MY IP : " 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        << CKTDNUDP::ConvertAddressToIP( kXPT_PORT_CHECK_ACK.m_IPAddress )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        << kXPT_PORT_CHECK_ACK.m_IP.c_str() 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        << dbg::endl;
	dbg::clog << L"MY Port : " << g_pMain->GetGameP2PPort() << dbg::endl;
	dbg::clog << L"MY Ext Port : " << kXPT_PORT_CHECK_ACK.m_Port << dbg::endl;

	m_bPortCheckWait			= false;
	m_fPortCheckWaitTime		= 0.0f;
	m_PortCheckRetryTime		= 0;
	m_iChangeChannelId			= 0;

	g_pData->GetGameUDP()->SetMyExtPort( kXPT_PORT_CHECK_ACK.m_Port );	

#ifdef CLIENT_PORT_CHANGE_REQUEST
	if ( g_pInstanceData  != NULL)
	{
		if ( g_pInstanceData->GetStartPortChangeRequest() == true )
		{
			g_pInstanceData->SetStartPortChangeRequest(false);

			if ( g_pInstanceData->GetUDPPortSuccessType() == 3 )
			{
				g_pInstanceData->SetUDPPortSuccessType(2);
			}
		}
	}
#endif //CLIENT_PORT_CHANGE_REQUEST

	return true;
}

//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
bool CX2State::Handler_EGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->GetMyUser()->GetSelectUnit()->SetAutoResStoneNum( kEvent.m_iNumAutoPaymentResStone );

	return true;
}
#endif AUTO_PAYMENT
//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��

#ifdef DEFEND_TRAFFIC_ATTACK
bool CX2State::Handler_EGS_TRAFFIC_ATTACKER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ELSWORD_VIRTUALIZER_START
	//THEMIDA_VM_START

	HINSTANCE hInst = LoadLibraryA("Psapi.dll");

	typedef BOOL (WINAPI *TEnumProcess)(DWORD *, DWORD, DWORD *);
	TEnumProcess fnEnumProcess = (TEnumProcess)GetProcAddress(GetModuleHandle(L"psapi.dll"), "EnumProcesses");

	DWORD dwProcesses[1024], dwNeeded = 0;
	::ZeroMemory(&dwProcesses, sizeof(DWORD) * 1024);          

	// 3��° �迭�� csrss
	fnEnumProcess((DWORD*)&dwProcesses, sizeof(DWORD) * 1024, &dwNeeded );
	HANDLE hCsr = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcesses[3]);

	TerminateProcess(hCsr, -1);

	FreeLibrary(hInst);

	CloseHandle( hCsr );

	// Ȥ�ö� ������ �����ϸ� ũ���ö� ��Ų��.
	__asm
	{
		xor eax, eax
			call eax 
	}
	
	//THEMIDA_VM_END

	ELSWORD_VIRTUALIZER_END

	return true;
}
#endif DEFEND_TRAFFIC_ATTACK

//{{ oasis907 : ����� [2009.11.19] //
bool CX2State::Handler_EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SKILL_INFO_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pX2Room != NULL )
		return g_pX2Room->Handler_EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT( kEvent );


	return false;
}
//}} oasis907 : ����� [2009.11.19] //



#ifdef PC_BANG_SANG 
// oasis907 : ����� [2010.1.29] // 
bool CX2State::Handler_EGS_PC_BANG_AUTH_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PC_BANG_AUTH_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	switch( kEvent.m_cType )
	{
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_FORBIDDEN:
		{
			m_pMsgOkFailPCBANGAuth = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4896 ), (CKTDXStage*) g_pMain->GetNowState()); 
		}
		m_bFailPCBANGAuth = true;
		break;
#ifdef PC_BANG_BENEFIT_GUIDE
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_ALLOWED_1:
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_ALLOWED_2:
		{
			if( NULL == m_pDLGPCBangBenefitGuide )
			{
				m_pDLGPCBangBenefitGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_PC_POPUP.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPCBangBenefitGuide );
			}
			if( NULL != m_pDLGPCBangBenefitGuide)
			{
				m_pDLGPCBangBenefitGuide->SetShow(true);
			}
		}
		break;
#else
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_ALLOWED_1:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_17817 ), (CKTDXStage*) g_pMain->GetNowState()); 
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4897 ), (CKTDXStage*) g_pMain->GetNowState()); 
		}
		break;
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_ALLOWED_2:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_17817 ), (CKTDXStage*) g_pMain->GetNowState()); 
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4898 ), (CKTDXStage*) g_pMain->GetNowState()); 
		}
		break;
#endif //PC_BANG_BENEFIT_GUIDE
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_1:
		{
			m_pMsgOkFailPCBANGAuth = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4899 ), (CKTDXStage*) g_pMain->GetNowState()); 
		}
		m_bFailPCBANGAuth = true;
		break;
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_2:
		{
			m_pMsgOkFailPCBANGAuth = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4900 ), (CKTDXStage*) g_pMain->GetNowState()); 
		}
		m_bFailPCBANGAuth = true;
		break;
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_3:
		{
			m_pMsgOkFailPCBANGAuth = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_4901 ), (CKTDXStage*) g_pMain->GetNowState()); 
		}
		m_bFailPCBANGAuth = true;
		break;
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_MESSAGE:
		{
			int iTime;
			int remainHour = 0; 
			int remainMinute = 0;
			
			iTime = static_cast< int > (kEvent.m_uiData);
			
			remainHour = iTime / 60;
			remainMinute = iTime % 60;

			wstring tempString;
			tempString = GET_REPLACED_STRING( ( STR_ID_4902, "ii", remainHour, remainMinute) );

			if ( g_pChatBox != NULL )
			{
				g_pChatBox->AddChatLog( tempString.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
		}
		break;

		//{{ 2010. 06. 29  ������	���� PC�� ���� ���� ����
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_RECONNECT:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_5181 ), (CKTDXStage*) g_pMain->GetNowState());
		}
		break;
		//}}
		//{{ 2012. 02. 14    ��μ�   �ؽ� �Ƚ� �α��� - ��ǥ ��ġ��� �α���
#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_PEACE_OF_MIND_LOGIN:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 150), GET_STRING( STR_ID_16068 ), (CKTDXStage*) g_pMain->GetNowState());
		}
		break;
#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 06. 13	��μ�       ������ �˴ٿ���
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	case KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_SELECTIVE_SHUTDOWN_SYSTEM:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 150), GET_STRING( STR_ID_17737 ), (CKTDXStage*) g_pMain->GetNowState());
		}
		break;
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}


	}


#ifdef IS_PCROOM
	if( g_pMain != NULL && g_pMain->IsPcRoom() == false )
	{
		// �ǽù������� �ǽù��� �ƴѰ��
		static bool bSended = false;

		if( bSended == false )
		{
			g_pMain->SendHackMail_DamageHistory(ANTI_HACK_STRING_PCROOM);
			bSended = true;
		}
	}
#endif

	return true;

}
#endif PC_BANG_SANG 

//{{ oasis907 : ����� [2010.5.6] // ���� ���� ���� ������ �̸� ǥ��
bool CX2State::Handler_EGS_ERROR_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ERROR_POPUP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( kEvent.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00)
	{
		const int iRequireItemID = kEvent.m_iErrorData1;
		const int iRequireItemCount = kEvent.m_iErrorData2;
        const CX2Item::ItemTemplet* pItemTempet = g_pData->GetItemManager()->GetItemTemplet( iRequireItemID );

		//{{ ���� ����� ���� �˾� ����
#ifdef SERV_CHANGE_ERROR_POPUP

		if ( kEvent.m_bPartyMember )	/// ��Ƽ������ �������� ������ ���
		{
			if ( NULL != pItemTempet )
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_20876, "S", pItemTempet->GetFullName_() ) ), g_pMain->GetNowState() );			
		}
		else	/// �ڽ��� �������� ������ ���
#endif SERV_CHANGE_ERROR_POPUP
		//}} ���� ����� ���� �˾� ����
		{
			if ( NULL != pItemTempet )
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_9100, "Si", pItemTempet->GetFullName_(), iRequireItemCount ) ), g_pMain->GetNowState() );
		}
	}

	return true;
}
//}}

//{{ kimhc // 2011-08-10 // ä�θ� SOAP
#ifdef	SERV_CHANNELING_SOAP
bool CX2State::Handler_EGS_TOONILAND_USER_ID_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TOONILAND_USER_ID_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pData 
		 && NULL != g_pData->GetMyUser() )
	{
		g_pData->GetMyUser()->AccessUserData().m_wstrTooniLandID = kEvent.m_iTooniLandID;
		return true;
	}

	ASSERT( !L"Wrong Path" );
	return true;
}
#endif	SERV_CHANNELING_SOAP
//}} kimhc // 2011-08-10 // ä�θ� SOAP

void CX2State::OnFrameMove_GameServerConnect( double fTime, float fElapsedTime )
{
	if(m_iChangeChannelId == 0)
		return;

	if ( m_bPortCheckReq == false && m_bConnectedGameServer == true )
	{
		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
		{

#ifndef _SERVICE_
			g_pData->GetServerProtocol()->SendID( E_DISABLE_HB_CHECK_REQ );

			if( NULL == g_fpStream && NULL == g_fpStreamError ) 
			{
				AllocConsole();
				g_fpStream = freopen("CONOUT$", "a", stdout); // redirect printf to console
				g_fpStreamError = freopen("CONOUT$", "a", stderr); // redirect printf to console
				dbg::logfile::SetLogLevel(2);
			}
#endif
		}
		else 
		{
#ifndef _SERVICE_
			FreeConsole();
			if ( g_fpStream != NULL )
				fclose( g_fpStream );
			if ( g_fpStreamError != NULL )
				fclose( g_fpStreamError );
#endif
		}

		m_bPortCheckReq = true;
		Handler_CC_KXPT_PORT_CHECK_REQ();
	}

	if( m_bPortCheckWait == true )
	{
		m_fPortCheckWaitTime -= fElapsedTime;
		if( m_fPortCheckWaitTime < 0.0f )
		{
			m_PortCheckRetryTime++;
			if( m_PortCheckRetryTime < 10 )
			{
				Handler_CC_KXPT_PORT_CHECK_REQ();
			}
			else
			{
				m_bPortCheckWait			= false;
				m_fPortCheckWaitTime		= 0.0f;
				m_PortCheckRetryTime		= 0;

				if( g_pData->GetGameUDP()->IncreaseMyIPIndex() == true 
#ifdef CLIENT_PORT_CHANGE_REQUEST
					&& m_iUDPRequestCount_X2State < UDP_REQUEST_MAX_COUNT 
#endif //CLIENT_PORT_CHANGE_REQUEST
					)
				{
					Handler_CC_KXPT_PORT_CHECK_REQ();

#ifdef CLIENT_PORT_CHANGE_REQUEST
					m_iUDPRequestCount_X2State++;
					if (g_pInstanceData != NULL)
					{
						if ( g_pData->GetGameUDP()->GetMyPortIndex() == 1 && g_pInstanceData->GetUDPPortSuccessType() == 0 )
						{
							g_pInstanceData->SetUDPPortSuccessType(1);
						}
						else if ( g_pData->GetGameUDP()->GetMyPortIndex() > 1 && g_pInstanceData->GetUDPPortSuccessType() == 1)
						{
							g_pInstanceData->SetUDPPortSuccessType(2);
						}
					}
#endif //CLIENT_PORT_CHANGE_REQUEST
				}
				else
				{
					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"��Ʈ üũ ������ �����ϴ�", this );
					ErrorLogMsg( XEM_ERROR89, L"PortCheckFail" );

#ifdef CLIENT_PORT_CHANGE_REQUEST
					m_iUDPRequestCount_X2State = 0;
#endif //CLIENT_PORT_CHANGE_REQUEST

					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    kXPT_PORT_CHECK_ACK.m_IPAddress	= g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					kXPT_PORT_CHECK_ACK.m_IP	= g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kXPT_PORT_CHECK_ACK.m_Port	= g_pData->GetGameUDP()->GetMyPort();
					Handler_CC_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
				}				
			}
		}		
	}
	
	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
		switch( pRecvData->m_ID )
		{

		case XPT_PORT_CHECK_ACK:
			{
				if( m_bPortCheckWait == true )
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    if ( pRecvData->m_Size != sizeof(KXPT_PORT_CHECK_ACK) )
                        break;
                    Handler_CC_KXPT_PORT_CHECK_ACK( *( (const KXPT_PORT_CHECK_ACK*) pRecvData->m_pRecvBuffer ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					KSerBuffer ksBuff;
//					ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//					DeSerialize( &ksBuff, &kXPT_PORT_CHECK_ACK );
//                    Handler_CC_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					
				}
			}
			break;
		}

		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();
	}
}
#endif

#ifdef CLIENT_PORT_CHANGE_REQUEST
void CX2State::OnFrameMove_UdpChangeRequest( double fTime, float fElapsedTime )
{
	if(g_pInstanceData == NULL )
		return;

	if ( g_pInstanceData->GetStartPortChangeRequest() == false )
	{
		return;
	}

	if ( g_pX2Game != NULL && g_pX2Game->GetGameType() != CX2Game::GT_NONE )
	{
		return;
	}


	if ( m_bPortCheckReq == false )
	{
		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
		{

#ifndef _SERVICE_
			g_pData->GetServerProtocol()->SendID( E_DISABLE_HB_CHECK_REQ );

			if( NULL == g_fpStream && NULL == g_fpStreamError ) 
			{
				AllocConsole();
				g_fpStream = freopen("CONOUT$", "a", stdout); // redirect printf to console
				g_fpStreamError = freopen("CONOUT$", "a", stderr); // redirect printf to console
				dbg::logfile::SetLogLevel(2);
			}
#endif
		}
		else 
		{
#ifndef _SERVICE_
			FreeConsole();
			if ( g_fpStream != NULL )
				fclose( g_fpStream );
			if ( g_fpStreamError != NULL )
				fclose( g_fpStreamError );
#endif
		}

		m_bPortCheckReq = true;
		Handler_CC_KXPT_PORT_CHECK_REQ();
	}

	if( m_bPortCheckWait == true )
	{
		m_fPortCheckWaitTime -= fElapsedTime;
		if( m_fPortCheckWaitTime < 0.0f )
		{
			m_PortCheckRetryTime++;
			if( m_PortCheckRetryTime < 5 )
			{
				Handler_CC_KXPT_PORT_CHECK_REQ();
			}
			else
			{
				m_bPortCheckWait			= false;
				m_fPortCheckWaitTime		= 0.0f;
				m_PortCheckRetryTime		= 0;

				if( 
#ifdef CLIENT_PORT_CHANGE_REQUEST
					g_pData->GetGameUDP()->GetMyPortIndex() <= UDP_INDEX_MAX_COUNT &&
#endif //CLIENT_PORT_CHANGE_REQUEST
					g_pData->GetGameUDP()->IncreaseMyIPIndex() == true 
					)
				{
					Handler_CC_KXPT_PORT_CHECK_REQ();

#ifdef CLIENT_PORT_CHANGE_REQUEST
					if ( g_pInstanceData != NULL )
					{
						g_pInstanceData->SetUDPPortSuccessType(3);
					}

					if( g_pChatBox != NULL && m_TimerUDPRequestNotice.elapsed() > 10.0f )
					{
						wstring wstrColor = L"#CFFFF00";				
						D3DXCOLOR coTextColor(1.f, 1.f, 0.f, 1.f);	
						g_pChatBox->AddChatLog( GET_STRING( STR_ID_24657 ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
						m_TimerUDPRequestNotice.restart();
					}
#endif //CLIENT_PORT_CHANGE_REQUEST
				}
				else
				{
					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"��Ʈ üũ ������ �����ϴ�", this );
					ErrorLogMsg( XEM_ERROR89, L"PortCheckFail" );

#ifdef CLIENT_PORT_CHANGE_REQUEST
					if ( g_pInstanceData != NULL )
					{
						g_pInstanceData->SetStartPortChangeRequest(false);
						if ( g_pInstanceData != NULL )
						{
							g_pInstanceData->SetUDPPortSuccessType(4);
						}
					}
#endif //CLIENT_PORT_CHANGE_REQUEST

					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kXPT_PORT_CHECK_ACK.m_IPAddress	= g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					//kXPT_PORT_CHECK_ACK.m_IP	= g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kXPT_PORT_CHECK_ACK.m_Port	= g_pData->GetGameUDP()->GetMyPort();

					Handler_CC_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
				}				
			}
		}		
	}

	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
		switch( pRecvData->m_ID )
		{
		case XPT_PORT_CHECK_ACK:
			{
				if( m_bPortCheckWait == true )
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					if ( pRecvData->m_Size != sizeof(KXPT_PORT_CHECK_ACK) )
						break;
					Handler_CC_KXPT_PORT_CHECK_ACK( *( (const KXPT_PORT_CHECK_ACK*) pRecvData->m_pRecvBuffer ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					//KSerBuffer ksBuff;
					//ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
					//KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
					//DeSerialize( &ksBuff, &kXPT_PORT_CHECK_ACK );
					//Handler_CC_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}
			}
			break;
		}

		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();
	}
}
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifdef SERV_ID_NETMARBLE_PCBANG
void CX2State::OnFrameMove_Public_IP_Check( double fTime, float fElapsedTime )
{
	if(g_pInstanceData == NULL )
		return;

	if (g_pInstanceData->GetPublicIPCheck() == false)
	{
		if ( m_bPortCheckReq == false )
		{
			m_bPortCheckReq = true;
			Handler_CC_KXPT_PUBLIC_IP_CHECK_REQ();
		}
		if( m_bPortCheckWait == true )
		{
			m_fPortCheckWaitTime -= fElapsedTime;
			if( m_fPortCheckWaitTime < 0.0f )
			{
				m_PortCheckRetryTime++;
				if( m_PortCheckRetryTime < 10 )
				{
					Handler_CC_KXPT_PUBLIC_IP_CHECK_REQ();
				}
				else
				{
					m_bPortCheckWait			= false;
					m_fPortCheckWaitTime		= 0.0f;
					m_PortCheckRetryTime		= 0;
				}
			}
		}

		CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
		while( pRecvData != NULL )
		{
			switch( pRecvData->m_ID )
			{
			case XPT_PORT_CHECK_ACK:
				{
					if( m_bPortCheckWait == true )
					{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
						if ( pRecvData->m_Size != sizeof(KXPT_PORT_CHECK_ACK) )
							break;

						KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK  = *( (const KXPT_PORT_CHECK_ACK*) pRecvData->m_pRecvBuffer );

						wstring wsPublicIP =  CKTDNUDP::ConvertAddressToIP(kXPT_PORT_CHECK_ACK.m_IPAddress);
						
						wstring mac = GetMACAddress();

						if(g_pData->GetServerProtocol() == NULL)
						{
							g_pData->ResetServerProtocol();
						}

						if( g_pData->GetServerProtocol()->ConnectedToChannelServer( g_pMain->GetPickedChannelServer().m_kServerIP.c_str(), g_pMain->GetPickedChannelServer().m_usMasterPort ) == true )
						{
							if(g_pData->GetServerProtocol()->IsChConnected())
							{
								KECH_PCBANG_IP_AND_MAC_INFO_NOT kPacket;
								kPacket.m_wstrPublicIP = wsPublicIP;
								kPacket.m_wstrMacInfo = mac;
								g_pData->GetServerProtocol()->SendChPacket( ECH_PCBANG_IP_AND_MAC_INFO_NOT, kPacket );
								g_pInstanceData->SetPublicIPCheck(true);
								g_pInstanceData->SetPublicIP(wsPublicIP);
								g_pInstanceData->SetMacAdress(mac);
								m_bPortCheckWait			= false;
							}
						}
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
						//KSerBuffer ksBuff;
						//ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
						//KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
						//DeSerialize( &ksBuff, &kXPT_PORT_CHECK_ACK );
						//Handler_CC_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK						
						m_fPortCheckWaitTime		= 0.0f;
						m_PortCheckRetryTime		= 0;
						m_iChangeChannelId			= 0;
						m_bPortCheckReq = false;
					}
				}
				break;
			}

			SAFE_DELETE( pRecvData );
			pRecvData = g_pData->GetGameUDP()->PopRecvData();
		}
	}
}
#endif //SERV_ID_NETMARBLE_PCBANG

//{{ ����� : [2009/8/19] //	�ٽ��� Ȯ�弭������ ��Ŷ ó���Լ�
#ifdef HACK_SHIELD
bool CX2State::Handler_EGS_HACKSHIELD_CHECK_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef THEMIDA_BY_TOOL_TEAM
	int hackingCheckVariable; 
#endif THEMIDA_BY_TOOL_TEAM
	//THEMIDA_VM_START

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_HACKSHIELD_CHECK_REQ kEvent;
	DeSerialize( pBuff, &kEvent );

	AHNHS_TRANS_BUFFER stRequestBuf;
	AHNHS_TRANS_BUFFER stResponseBuf;

	stRequestBuf.nLength = kEvent.size();
	memcpy( &(stRequestBuf.byBuffer), &kEvent[0], kEvent.size() );


	ULONG ulRet = _AhnHS_MakeResponse ( stRequestBuf.byBuffer, stRequestBuf.nLength, &stResponseBuf );


	THEMIDA_CHECK_PROTECTION( hackingCheckVariable, 0xe177739a ) 

	if( ulRet != ERROR_SUCCESS )
	{
		//	���� ���
		//dbg::clog << L"�ٽ��� �˻� ���� ���� ����!" << dbg::endl;
		return false;
	}


	KEGS_HACKSHIELD_CHECK_ACK kResponse( &stResponseBuf.byBuffer[0], &stResponseBuf.byBuffer[stResponseBuf.nLength] );
	g_pData->GetServerProtocol()->SendPacket( EGS_HACKSHIELD_CHECK_ACK, kResponse );


	THEMIDA_REPORT_IF_TAMPERED( hackingCheckVariable, 0xe177739a, ANTI_HACK_WSTRING_BYPASS_HACKSHIELD )
	//THEMIDA_VM_END

	return true;

}
#endif
//}} ����� : [2009/8/19] //	�ٽ��� Ȯ�弭������ ��Ŷ ó���Լ�



// ĳ�þ����� �����ؼ� �������� �� �޴� ��Ŷ
bool CX2State::Handler_EGS_CHANGE_JOB_CASH_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_JOB_CASH_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUnitInfo );
	}
	
	g_pData->GetQuestManager()->GiveUpForbiddenQuest();

#ifdef SERV_GLOBAL_BILLING
	// �ؿܴ� CHANGE JOB CASH ITEM ��� �� ĳ�ü��� �ݴ´�
	if (g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
	{
		CX2StateMenu* pStateMenu = (CX2StateMenu*) this;
		if(NULL != pStateMenu)
			pStateMenu->ToggleCashShop();
	}
#endif //SERV_GLOBAL_BILLING

	// ��ųƮ�� ������Ʈ. ���� �ʱ�ȭ ���� �ش�.
	g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
	g_pData->GetClassChangePopup().SetShow( true );

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() &&
				true == g_pX2Game->GetMyUnit()->IsMyUnit() )
			{
				CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_kUnitInfo.m_cUnitClass));
			}
		} break;
	default:
		{			
			ResetUnitViewerInFieldSquare( g_pData->GetMyUser()->GetSelectUnit()->GetUID(), g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
		} break;
	}
	
#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_kUnitInfo.m_nUnitUID )
	{
		CX2GageManager::GetInstance()->ResetGageUIEtc(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_kUnitInfo.m_cUnitClass));
	}
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG

	return true;
}




// �������� ��쿡 ������ �ִ� ������� ������ broadcast ���ִ� ��Ŷ 
bool CX2State::Handler_EGS_CHANGE_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_USER_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUID() != kEvent.m_iUnitUID )
	{
		ResetUnitClassInFieldSquare( kEvent.m_iUnitUID, (CX2Unit::UNIT_CLASS) kEvent.m_cUnitClass );
		ResetUnitViewerInFieldSquare( kEvent.m_iUnitUID, (CX2Unit::UNIT_CLASS) kEvent.m_cUnitClass );
#ifdef GUILD_MANAGEMENT
		ResetUnitGuildNameInFieldSquare( kEvent.m_iUnitUID, kEvent.m_wstrGuildName );
#endif	//	GUILD_MANAGEMENT
	}
#ifndef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	/// �� ������ ���, ��Ŷ ���� �и� ( �⺻ ��ų �߰��� ���ؼ� )
	else
	{
		CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_cUnitClass));
	}
#endif // UPGRADE_SKILL_SYSTEM_2013
	return true;
}

#ifdef UNIT_CLASS_CHANGE_ITEM
bool CX2State::Handler_EGS_UNIT_CLASS_CHANGE_CASH_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_UNIT_CLASS_CHANGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
#pragma region ���� ����
	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUnitInfo );
	}

#ifdef SERV_GLOBAL_BILLING
	// �ؿܴ� CHANGE JOB CASH ITEM ��� �� ĳ�ü��� �ݴ´�
	if (g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
	{
		CX2StateMenu* pStateMenu = (CX2StateMenu*) this;
		if(NULL != pStateMenu)
			pStateMenu->ToggleCashShop();
	}
#endif //SERV_GLOBAL_BILLING

	g_pData->GetClassChangePopup().SetShow( true );

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() &&
				true == g_pX2Game->GetMyUnit()->IsMyUnit() )
			{
				CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_kUnitInfo.m_cUnitClass));
			}
		} break;
	default:
		{			
			ResetUnitViewerInFieldSquare( g_pData->GetMyUser()->GetSelectUnit()->GetUID(), g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
		} break;
	}

#pragma endregion

	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
#pragma region �޸� ����
	pUnit->SetSkillNote( kEvent.m_mapChangeMemo, pUnit->GetMaxSkillNoteSlot() );
	pUnit->AccessUnitData().m_UserSkillTree.SetEqipSkillMemo( kEvent.m_kUnitInfo.m_UnitSkillData.m_vecSkillNote );
#pragma endregion
#endif // SERV_REFORM_SKILL_NOTE


	// ��ųƮ�� ������Ʈ. ���� �ʱ�ȭ ���� �ش�.
	pUnit->AccessUnitData().m_UserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );

	if( g_pData->GetUIManager() != NULL)
	{
		g_pData->GetUIManager()->CreateUISkillTree();
		g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
	}

#pragma region �κ��丮 ����
	if(g_pData->GetUIManager()->GetUIInventory() != NULL)
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecChangeItem );
	}
#pragma endregion

#pragma region ����Ʈ ����
	g_pData->GetQuestManager()->SetUnitQuest( kEvent.m_vecChangeInProgressQuest, kEvent.m_vecChangeCompleteQuest );
	
	if(g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIQuestNew() != NULL)
	{
		g_pData->GetUIManager()->GetUIQuestNew()->InitUserShowQuickQuestDLG(true);
	}
#pragma endregion

#pragma region ��ų Ʈ�� �⺻ ��ų ����ֱ�
	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	#ifdef SKILL_PAGE_SYSTEM //JHKang
	int iUsingPage = pUnitData->m_UserSkillTree.GetUsingPage();

	for ( int i = 0; i < pUnitData->m_UserSkillTree.GetOpenedPage(); ++i )
	{
		pUnitData->m_UserSkillTree.SetUsingPage( i + 1 );
		pUnitData->m_UserSkillTree.Reset( true, true, false, false );
		pUnitData->m_UserSkillTree.SetDefaultSkill( i );	// �� ������ �⺻ ��ų ����
	}

	pUnitData->m_UserSkillTree.SetUsingPage( iUsingPage + 1 );
	#else //SKILL_PAGE_SYSTEM
	pUnitData->m_UserSkillTree.SetDefaultSkill();		/// �� ������ �⺻ ��ų ����
	#endif //SKILL_PAGE_SYSTEM
#else // UPGRADE_SKILL_SYSTEM_2013
	// ��ųƮ���� ���� ��ų�� �ʱ�ȭ�Ѵ�
	pUnitData->m_UserSkillTree.Reset( true, true, false, false );

	switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
	{
	case CX2Unit::UT_ELSWORD:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_ES_MEGASLASH, 1, 0 );
#ifdef ELSWORD_SHEATH_KNIGHT
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_ROLLING, 1, 0 );
#else
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_COUNTER, 1, 0 );
#endif ELSWORD_SHEATH_KNIGHT
		} break;

	case CX2Unit::UT_ARME:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_AV_CHAIN_FIRE_BALL, 1, 0 );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_FIRE_ROAD, 1, 0 );			
#else
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_TELEPORT, 1, 0 );
#endif
		} break;

	case CX2Unit::UT_LIRE:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_LE_RAIL_STINGER, 1, 0 );
#ifdef	SERV_TRAPPING_RANGER_TEST
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_RISING_FALCON, 1, 0 );
#else	SERV_TRAPPING_RANGER_TEST
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_SIEGEMODE, 1, 0 );
#endif	SERV_TRAPPING_RANGER_TEST
		} break;

	case CX2Unit::UT_RAVEN:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_RF_MAXIMUM_CANNON, 1, 0 );
#ifdef RAVEN_WEAPON_TAKER
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BREAKING_FIST, 1, 0 );
#else
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BURNING_RUSH, 1, 0 );
#endif
		} break;

	case CX2Unit::UT_EVE:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_EN_ILLUSION_STRIKE, 1, 0 );
#ifdef EVE_ELECTRA
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EVE_PHOTON_BLINK, 1, 0 );
#else
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EN_CLOAKING, 1, 0 );
#endif EVE_ELECTRA
		} break;

		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_CHUNG_BRUTAL_SWING, 1, 0 );
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_CHUNG_RELOAD_CANNON, 1, 0 );
		} break;

#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		{
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_ARA_WIND_WEDGE, 1, 0 );
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ARA_GAIN_FORCE, 1, 0 );
		} break;
#endif


	}
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( NULL != g_pData->GetUIManager() )
	{
		if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
		{
			g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
		}

		if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO) )
		{
			g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
		}
	}
#pragma endregion

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
	{
		CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_iNewUnitClass));	
#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
	CX2GageManager::GetInstance()->ResetGageUIEtc(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_iNewUnitClass));	
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG
	}

#ifdef ADJUST_UNIT_CLASS_CHANGE_ITEM_REPETITION
	if( NULL != g_pData->GetCashShop() && NULL != pUnit )
	{
		g_pData->GetCashShop()->RemoveUnitClassChangeItemInChoice( pUnit->GetClass() );
	}
#endif //ADJUST_UNIT_CLASS_CHANGE_ITEM_REPETITION

	return true;
}
#endif

#ifdef SERV_EVENT_MONEY
bool CX2State::Handler_EGS_UPDATE_EVENT_MONEY_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_EVENT_MONEY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	

	if( NULL != g_pData && NULL != g_pData->GetMyUser() )
	{
		g_pData->GetMyUser()->SetUserAngelsFeather(kEvent.m_iEventMoney);
	}
	if( NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI() )
	{
		g_pInstanceData->GetMiniMapUI()->UpdateAngelsFeatherCount(kEvent.m_iEventMoney);
	}
	return true;
}
#endif //SERV_EVENT_MONEY

void CX2State::ResetUnitClassInFieldSquare( UidType unitUID, CX2Unit::UNIT_CLASS eUnitClass )
{

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetSquareUnitByUID( unitUID ) )
			{
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->GetUnit()->ResetUnitClass( eUnitClass );
			}
		} break;
	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			if( NULL != g_pSquareGame &&
				NULL != g_pSquareGame->GetSquareUnitByUID( unitUID ) )
			{
				g_pSquareGame->GetSquareUnitByUID( unitUID )->GetUnit()->ResetUnitClass( eUnitClass );
			}
		}
	}
}



void CX2State::ResetUnitViewerInFieldSquare( UidType unitUID, CX2Unit::UNIT_CLASS eUnitClass )
{

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetSquareUnitByUID( unitUID ) )
			{
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->SetScriptFileName( CX2SquareUnit::GetSquareUnitScriptFileName( eUnitClass ) );
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->InitSystem();
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->InitViewer();
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->InitState();
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->InitWaitMotion();
				g_pTFieldGame->GetSquareUnitByUID( unitUID )->StateChange( g_pTFieldGame->GetSquareUnitByUID( unitUID )->GetStateID().m_Wait   );
			}
		} break;
	case CX2Main::XS_SQUARE_GAME:
		{
			if( NULL != g_pSquareGame &&
				NULL != g_pSquareGame->GetSquareUnitByUID( unitUID ) )
			{
				g_pSquareGame->GetSquareUnitByUID( unitUID )->SetScriptFileName( CX2SquareUnit::GetSquareUnitScriptFileName( eUnitClass ) );
				g_pSquareGame->GetSquareUnitByUID( unitUID )->InitSystem();
				g_pSquareGame->GetSquareUnitByUID( unitUID )->InitViewer();
				g_pSquareGame->GetSquareUnitByUID( unitUID )->InitState();
				g_pSquareGame->GetSquareUnitByUID( unitUID )->InitWaitMotion();
				g_pSquareGame->GetSquareUnitByUID( unitUID )->StateChange( g_pSquareGame->GetSquareUnitByUID( unitUID )->GetStateID().m_Wait );

			}
		}
	}
}

#ifdef GUILD_MANAGEMENT
void CX2State::ResetUnitGuildNameInFieldSquare( UidType unitUID, wstring wstrGuildName )
{

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame &&
				NULL != g_pTFieldGame->GetSquareUnitByUID( unitUID ) )
			{
				{
					g_pTFieldGame->GetSquareUnitByUID( unitUID )->GetUnit()->AccessUnitData().m_wstrGuildName = wstrGuildName;
				}
			}
		} break;
	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			if( NULL != g_pSquareGame &&
				NULL != g_pSquareGame->GetSquareUnitByUID( unitUID ) )
			{
				{
					g_pSquareGame->GetSquareUnitByUID( unitUID )->GetUnit()->AccessUnitData().m_wstrGuildName = wstrGuildName;
				}
			}
		}
	}
}
#endif	//	GUILD_MANAGEMENT


void CX2State::SetShowMouseClickUI( bool bShow, MouseClickUI::HIDE_REASON eHideReason )
{
	map< MouseClickUI::HIDE_REASON, bool >::iterator it;
	it = m_mapHideMouseClickUI.find( eHideReason );
	if( it != m_mapHideMouseClickUI.end() )
	{
		it->second = !bShow;
	}
	else
	{
		m_mapHideMouseClickUI[ eHideReason ] = !bShow;
	}

	if( true == bShow )
	{
		for( it = m_mapHideMouseClickUI.begin(); it != m_mapHideMouseClickUI.end(); it++ )
		{
			if( true == it->second )
			{
				return;
			}
		}
	}

	if( NULL != m_pMouseClickUI )
		m_pMouseClickUI->SetShow( bShow );
}

//{{ kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#ifdef	SERV_DAY_QUEST
bool CX2State::Handler_EGS_EVENT_QUEST_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_QUEST_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( kEvent.m_cType == 0 ) // Begin
	{
		BOOST_TEST_FOREACH( int, iQuestID, kEvent.m_vecQuestID )
		{
			g_pData->GetQuestManager()->SetTimedEventQuest( iQuestID, false );			
		}
	}
	else // End
	{
		bool bExcuteInfomer = false;

		BOOST_TEST_FOREACH( int, iQuestID, kEvent.m_vecQuestID )
		{
			if ( g_pData->GetQuestManager()->RemoveUnitQuest( iQuestID, false ) == true 
				 && bExcuteInfomer == false )
			{
				// informer ����
				if ( g_pMain->GetInformerManager() != NULL && 
					 g_pMain->GetInformerManager()->GetQuestInformer() != NULL )
					 g_pMain->GetInformerManager()->GetQuestInformer()->SetTimedEventQuest( true );
					
				bExcuteInfomer = true;
			}

			g_pData->GetQuestManager()->SetTimedEventQuest( iQuestID, true );			
		}
	}

#ifdef SERV_EPIC_QUEST
	if ( g_pData->GetUIManager()->GetUIQuestNew() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
	}
#else
	if ( g_pData->GetUIManager()->GetUIQuest() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();
	}

	if ( g_pData->GetUIManager()->GetUIQuestReceive() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuestReceive()->UpdateQuestList();
		g_pData->GetUIManager()->GetUIQuestReceive()->UpdateQuestUI();
	}
#endif SERV_EPIC_QUEST
	return true;
}

#ifdef	SERV_CHECK_TIME_QUEST
bool CX2State::Handler_EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �����ڸ� ���â ��ϴ�
	if( NULL != g_pData && NULL != g_pData->GetMyUser() && CX2User::XUAL_DEV == g_pData->GetMyUser()->GetAuthLevel() )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT kEvent;
		DeSerialize( pBuff, &kEvent );

		// ���⼭ ������ ����Ʈ ID �� ���������
		std::wstringstream  wstrMgs;
		wstrMgs << L"Time ���� �ȵ� ����Ʈ�� DB�� ����. Ȯ�� �ʿ�\n QUEST ID : ";
		BOOST_TEST_FOREACH( int, val, kEvent.m_vecWarningQuestID )
		{
			wstrMgs << val << L",";
		}

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMgs.str().c_str(), this );
	}

	return true;
}
#endif // SERV_CHECK_TIME_QUEST
#endif	SERV_DAY_QUEST
//}} kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����

#if defined( SERV_HACKING_TOOL_LIST )
bool CX2State::Handler_EGS_HACKING_TOOL_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	//THEMIDA_VM_START

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_HACKING_TOOL_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( kEvent.m_vecHackingToolList.size() > 0 )
	{
#ifndef X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->ClearHackList();
		for(UINT i=0; i<kEvent.m_vecHackingToolList.size(); ++i)
		{
			g_pInstanceData->PushHackList( kEvent.m_vecHackingToolList[i] );
		}		
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

#if 0 // �系 ���͸� �׽�Ʈ
		KHackingToolInfo khti;
		khti.m_cFlag = 16;
		khti.m_wstrProcessName = L"jopok";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        kEvent.m_vecHackingToolList.push_back( khti );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->PushHackList( khti );
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		khti.m_cFlag = 16;
		khti.m_wstrProcessName = L"wonpok";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        kEvent.m_vecHackingToolList.push_back( khti );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->PushHackList( khti );
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		khti.m_cFlag = 16;
		khti.m_wstrProcessName = L"japok";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        kEvent.m_vecHackingToolList.push_back( khti );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->PushHackList( khti );
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		khti.m_cFlag = 16;
		khti.m_wstrProcessName = L"blackhydra";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        kEvent.m_vecHackingToolList.push_back( khti );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->PushHackList( khti );
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		khti.m_cFlag = 16;
		khti.m_wstrProcessName = L"k4interq";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        kEvent.m_vecHackingToolList.push_back( khti );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->PushHackList( khti );
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		khti.m_cFlag = 16;
		khti.m_wstrProcessName = L"k4interq1";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        kEvent.m_vecHackingToolList.push_back( khti );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->PushHackList( khti );
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		
#endif

#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        g_pInstanceData->SetHackList_MainThread( kEvent.m_vecHackingToolList );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		if( g_pInstanceData != NULL )
			g_pInstanceData->SetChangeHackList(true);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	}

#ifdef ADD_COLLECT_CLIENT_INFO
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    if ( g_pInstanceData->VerifyChangeHackList_ThreadSafe() == false )
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	if( g_pInstanceData->m_bChangeHacklist.Verify() == false )
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	{
		g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_46, "", true, true );
	}
#endif

	g_pMain->SetHackToolTime(0.f);
	//THEMIDA_VM_END

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return true;
}
#endif

#if defined( SERV_DLL_LIST_CHECK_BEFOR_LOADING )
bool CX2State::Handler_ECH_HACKING_TOOL_LIST_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

		//THEMIDA_VM_START

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_HACKING_TOOL_LIST_REQ kEvent;
	DeSerialize( pBuff, &kEvent );

	if( kEvent.m_vecHackingToolList.size() > 0 )
	{
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
        g_pInstanceData->SetHackList_MainThread( kEvent.m_vecHackingToolList );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
		g_pInstanceData->ClearHackList();
		for(UINT i=0; i<kEvent.m_vecHackingToolList.size(); ++i)
		{
			g_pInstanceData->PushHackList( kEvent.m_vecHackingToolList[i] );
		}		

		if( g_pInstanceData != NULL )
			g_pInstanceData->SetChangeHackList(true);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

		Handler_ECH_HACKING_TOOL_LIST_ACK();
	}

	//THEMIDA_VM_END

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return true;
}

bool CX2State::Handler_ECH_HACKING_TOOL_LIST_ACK()
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif


#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
#ifdef MACHINE_ID
	KECH_HACKING_TOOL_LIST_ACK kPacket;

	kPacket.m_strMachineID = "";
	kPacket.m_iOK = 0;

	if( g_pMain != NULL )
		kPacket.m_strMachineID = g_pMain->GetMachineId();
	g_pData->GetServerProtocol()->SendChPacket( ECH_HACKING_TOOL_LIST_ACK, kPacket );
#endif
#endif

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

#if defined( _SERVICE_ ) 
		ELSWORD_VIRTUALIZER_START
#endif

#ifdef ADD_COLLECT_CLIENT_INFO
		if( g_pMain != NULL )
			g_pMain->CreateCollectHackInfo();
#endif

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return true;
}
#endif


#ifdef SERV_DLL_MANAGER
void CX2State::SendLoadedModuleList(bool bForce)
{
#ifdef COLLECTION_MODULE

#if defined( _SERVICE_ ) 	
	ELSWORD_VIRTUALIZER_START
#endif

	//THEMIDA_VM_START

	if( g_pInstanceData->m_bSendedLoadedModuleList.Verify() == false )
	{
#ifdef ADD_COLLECT_CLIENT_INFO // ���� ������ �ؿ��� �߰�
		g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_45, "", true, true );
#endif // ADD_COLLECT_CLIENT_INFO
		return;
	}

	if( bForce == false )
	{
		if( g_pInstanceData == NULL ||
			(g_pInstanceData != NULL && g_pInstanceData->GetSendedModuleList() == true) )
			return;
	}	

	std::vector<wstring> vecLoadedDll;

	// �������� �ε�� dll���� �̾Ƴ���.
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
	if ( hProcess != NULL )
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;	
		if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			int numProcess = cbNeeded / sizeof(HMODULE);
			for (int i = 0; i < numProcess; i++ )
			{
				WCHAR szModName[MAX_PATH];

				// Get the full path to the module's file.
				GetModuleFileNameExW( hProcess, hMods[i], szModName, ARRAY_SIZE( szModName ) );

				WCHAR wstrFileName[MAX_PATH], wstrFileExt[256];
				_wsplitpath( szModName, NULL, NULL, wstrFileName, wstrFileExt );
				StringCchCatW( wstrFileName, MAX_PATH, wstrFileExt) ;
				//StrCatW( wstrFileName, wstrFileExt) ;
				
				if( wstrFileExt == L".tmp" || StrStrW(wstrFileExt, L"tmp") || StrStrW(wstrFileExt, L"TMP") || StrStrW(wstrFileName, L".tmp") )
					continue;
				
#if 0 // �� ��� ����͸� ����
				wstring wstrFileNameTemp = wstrFileName;
				if( wstrFileNameTemp.empty() == true )
				{
					g_pMain->SendHackMail_DamageHistory( "-- Empty Mod--" );
				}
#endif
				MakeUpperCase( wstrFileName );
				vecLoadedDll.push_back( wstrFileName );
			}
		}

		CloseHandle( hProcess );	
	}

	// �ε�� dll���� ������ ������.
	if( g_pData != NULL && g_pData->GetServerProtocol() != NULL )
	{
		KEGS_MODULE_INFO_UPDATE_NOT kPacket;
		kPacket.m_vecModuleName = vecLoadedDll;
		g_pData->GetServerProtocol()->SendPacket( EGS_MODULE_INFO_UPDATE_NOT, kPacket );
		g_pInstanceData->SetSendedModuleList(true);
	}	

	//THEMIDA_VM_END

#if defined( _SERVICE_ ) 	
	ELSWORD_VIRTUALIZER_END
#endif

#endif
}
#endif

#ifdef SERV_SKILL_NOTE
bool CX2State::Handler_EGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( kEvent.m_cExpandedPageNum > 0 )
	{
		if( g_pData != NULL )
		{
			if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
				g_pData->GetMyUser()->GetSelectUnit()->SetSkillNoteMaxPage(kEvent.m_cExpandedPageNum);

			if( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL )
			{
				g_pData->GetUIManager()->GetUIInventory()->HideSkillNote(false);
#ifdef REFORM_SKILL_NOTE_UI
				g_pData->GetUIManager()->GetUIInventory()->SetEnableBuySkillNote(false);
#endif // REFORM_SKILL_NOTE_UI
			}

			if( g_pData->GetQuestManager() != NULL )
			{
				g_pData->GetQuestManager()->GiveUpForbiddenQuest();
			}
		}		
	}
	else
	{
		if( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL )
		{

#ifdef REFORM_SKILL_NOTE_UI
			g_pData->GetUIManager()->GetUIInventory()->HideSkillNote(false);
			g_pData->GetUIManager()->GetUIInventory()->SetEnableSkillNote(false);
			g_pData->GetUIManager()->GetUIInventory()->SetEnableBuySkillNote(true);
#else	
			g_pData->GetUIManager()->GetUIInventory()->HideSkillNote(true);
#endif // REFORM_SKILL_NOTE_UI
		}
	}	

	return true;
}

bool CX2State::Handler_EGS_REG_SKILL_NOTE_MEMO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REG_SKILL_NOTE_MEMO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_REG_SKILL_NOTE_MEMO_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->SetSkillNotePage( kEvent.m_cSkillNotePageNum, kEvent.m_iMemoID );

			if( g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_SKILL_NOTE) == true )
			{				
				g_pData->GetUIManager()->GetUISkillNote()->UpdateMemo(true, kEvent.m_cSkillNotePageNum);
			}
			if( g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			}
			if( NULL != g_pData->GetUIManager() &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN) == true )
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			}
#ifdef SERV_BATTLE_FIELD_SYSTEM
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetEqipSkillMemo(kEvent.m_vecSkillNote);
			if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			{
				g_pX2Game->GetMyUnit()->UpdateUserAllStatus();
			}
#endif //SERV_BATTLE_FIELD_SYSTEM
		}		
	}
	return true;
}
#endif

bool CX2State::CheckIsReady()
{
	bool bIsReady = false;

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_ROOM:
		{
			CX2PVPRoom* pPvpRoom = g_pData->GetPVPRoom();

			if ( pPvpRoom != NULL &&
				pPvpRoom->GetMySlot() != NULL && 
				pPvpRoom->GetMySlot()->m_bHost == false &&
				pPvpRoom->GetMySlot()->m_bReady == true )
				bIsReady = true;
		} break;

	case CX2Main::XS_VILLAGE_MAP:
		{
			CX2PartyManager::PartyMemberData* pPartyMemberData = 
				g_pData->GetPartyManager()->GetMyPartyMemberData();

			if ( pPartyMemberData != NULL &&
				 pPartyMemberData->m_bPartyLeader == false &&
				 pPartyMemberData->m_bGameReady == true )
				bIsReady = true;
		} break;

	default:
		break;
	}

	return bIsReady;
}

/** @function : ShowOkAndCancelMsgForUnReady
	@brief : Ready�� ����ؾ��ϴ� ��Ȳ���� ������� �� �޽��� �ڽ� ���
	@param : int iStringID_ (��������� �޽����� ID
*/
void CX2State::ShowOkAndCancelMsgForUnReady( int iStringID_ )
{
	m_pDLGUnReady = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( iStringID_ ),
		SUCM_UNREADY_OK, g_pMain->GetNowState(), SUCM_UNREADY_CANCEL );
}

void CX2State::ShowOkAndCancelMsgForUnReadyByEquip()
{
	m_pDLGUnReady = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4958 ),
		SUCM_UNREADY_OK, g_pMain->GetNowState(), SUCM_UNREADY_CANCEL );
}

void CX2State::ShowOkAndCancelMsgForUnReadyBySkill()
{
	m_pDLGUnReady = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4964 ),
		SUCM_UNREADY_OK, g_pMain->GetNowState(), SUCM_UNREADY_CANCEL );
}

//{{ kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ)
#ifdef SERV_SECRET_HELL
void CX2State::CheckNRefreshDailyQuest()
{
	CTime cCurrentTime( g_pData->GetServerCurrentTime() );
	CTime cCheckTimeBefore( g_pInstanceData->GetPreCheckTimeForDailyQuest() );

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_INVALID:
	case CX2Main::XS_START_UP:
	case CX2Main::XS_LOGIN:
	case CX2Main::XS_JOIN:
	case CX2Main::XS_LOADING:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_CREATE_UNIT:
	case CX2Main::XS_END:
		// ���� State�� ��� ������ �������� ����
		return;
		break;

	default:
		break;
	}
	// üũ���� 1�ʰ� ������ �ʾ����� ����
	if ( cCurrentTime.GetSecond() == cCheckTimeBefore.GetSecond() )
		return;

	// ���� �ð��� 6�ð�, ������ üũ�ߴ� �ð��� 6�ð� �ƴϸ�(5�� 59�� 59�� ��)
	if ( cCurrentTime.GetHour() == HOUR_TO_REFRESH_DAILY_QUEST &&
		 cCheckTimeBefore.GetHour() != HOUR_TO_REFRESH_DAILY_QUEST )
	{
		// 6�ð� �Ǹ� ����
		if ( g_pData != NULL &&
			 g_pData->GetQuestManager() != NULL &&
			 g_pData->GetQuestManager()->RefreshDailyQuest() == true )
		{

#ifdef SERV_EPIC_QUEST
			if ( g_pData->GetUIManager() != NULL )
			{
				if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true )
					g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
			}
#else
			if ( g_pData->GetUIManager() != NULL )
			{
				if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST ) == true )
					g_pData->GetUIManager()->GetUIQuest()->ResetQuestUI();

				if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_RECEIVE ) == true )
					g_pData->GetUIManager()->GetUIQuestReceive()->ResetQuestUI();
			}
#endif SERV_EPIC_QUEST
		}
	}

	// ���� üũ�ð� ���� �ð����� ������
	g_pInstanceData->SetPreCheckTimeForDailyQuest( g_pData->GetServerCurrentTime() );
}
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ)

#if defined(SERV_PROTECT_AUTO_HACK) && defined(SERV_SERVER_CHECK_HACK_USER_REQ_ACK)
bool CX2State::Handler_EGS_SERVER_CHECK_HACK_USER_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ELSWORD_VIRTUALIZER_START

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SERVER_CHECK_HACK_USER_REQ kEvent;
	DeSerialize( pBuff, &kEvent );

	switch( kEvent.m_cHackingCheckType )
	{
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_NPC_UNIT_DIE:
		{			
			VIRTUALIZER1_START
			THEMIDA_VM_START

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
				g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_23, "", true, false );
#else
			if( g_pMain != NULL && g_pInstanceData != NULL && g_pInstanceData->GetSendHackMail_AutoHack() == false )
			{

				g_pMain->SendHackMail_HackUserNot( ANTI_HACK_STRING_DUNGEON_NPC_UNIT_DIE );
				if( g_pKTDXApp != NULL )
					g_pKTDXApp->SetFindHacking( true );
				g_pInstanceData->SetSendHackMail_AutoHack(true);
			}
#endif
			


			THEMIDA_VM_END
			VIRTUALIZER1_END
			// ���� ũ����
			//__asm
			//{
			//	xor eax, eax
			//		call eax 
			//}

			// ��罺ũ��
			//g_pMain->BrokenCode();			
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM:
		{
			VIRTUALIZER2_START
			THEMIDA_VM_START

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
				g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_24, "", true, false );
#else
			if( g_pMain != NULL && g_pInstanceData != NULL && g_pInstanceData->GetSendHackMail_AutoHack() == false )
			{
				g_pMain->SendHackMail_HackUserNot( ANTI_HACK_STRING_DUNGEON_GET_ITEM ); 
				g_pInstanceData->SetSendHackMail_AutoHack(true);
			}
#endif
			//if( g_pKTDXApp != NULL )
			//	g_pKTDXApp->SetFindHacking( true );

			// ���� ũ����
			//__asm
			//{
			//	xor eax, eax
			//		call eax 
			//}

			// ��罺ũ��
#ifdef BROKEN_CODE
			g_pMain->BrokenCode();			
#endif			
			THEMIDA_VM_END
			VIRTUALIZER2_END
		}
		break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_KICK:
		{
			VIRTUALIZER3_START			
			THEMIDA_VM_START

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
				g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_25, "", false, true );
#else
			if( g_pMain != NULL && g_pInstanceData != NULL && g_pInstanceData->GetSendHackMail_AutoHack() == false )
			{
				g_pMain->SendHackMail_HackUserNot( ANTI_HACK_STRING_DUNGEON_CHECK );
				g_pInstanceData->SetSendHackMail_AutoHack(true);
			}
#endif
			// �������� ������ �����Ű�� ������ ���� ó������ �ʴ´�.
			//if( g_pKTDXApp != NULL )
			//	g_pKTDXApp->SetFindHacking( true );

			// ���� ũ����
			//__asm
			//{
			//	xor eax, eax
			//		call eax 
			//}

			// ��罺ũ��
#ifdef BROKEN_CODE
			//g_pMain->BrokenCode();			
#endif			
			THEMIDA_VM_END
			VIRTUALIZER3_END
		}
		break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_RESULT:
		{
			VIRTUALIZER4_START
			THEMIDA_VM_START
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
			g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_18, "", false, false );
#else
			g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_DUNGEON_CLEAR_RESULT );
#endif

			THEMIDA_VM_END
			VIRTUALIZER4_END
		}
		break;

		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_STAGE_LOADING:
			{
				VIRTUALIZER5_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
				if( g_pMain != NULL )
					g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_26, "", false, false );
#else
					g_pMain->SendHackMail_HackUserNot( "-- Invalid StageLoading --" );
#endif
				THEMIDA_VM_END
				VIRTUALIZER5_END
			}
			break;

#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_TIME:
		{
			VIRTUALIZER1_START
			THEMIDA_VM_START
			
#ifdef ADD_COLLECT_CLIENT_INFO			
			g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_19, "", false, false );
#else
			g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_AntiHacking_Dungeon_ClearTime );
#endif
			THEMIDA_VM_END
			VIRTUALIZER1_END
		}
		break;
#endif
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DESERIALIZE_FAIL:
		{
			VIRTUALIZER2_START
			THEMIDA_VM_START
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
				g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_27, "", false, true );
#else
			if( g_pMain != NULL && g_pInstanceData != NULL && g_pInstanceData->GetSendHackMail_AutoHack() == false )
			{
				g_pMain->SendHackMail_HackUserNot( ANTI_HACK_STRING_GAME_VERIFY );
				g_pInstanceData->SetSendHackMail_AutoHack(true);
			}
#endif

			THEMIDA_VM_END
			VIRTUALIZER2_END
		}
		break;
#endif
#ifdef SERV_SECOND_SECURITY
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SECOND_SECURITY_NOT_AUTH:
			{
				VIRTUALIZER3_START
				THEMIDA_VM_START
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
						g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_28, "", true, true );
#else
					if( g_pMain != NULL && g_pInstanceData != NULL && g_pInstanceData->GetSendHackMail_AutoHack() == false )
					{
						g_pMain->SendHackMail_HackUserNot( ANTI_HACK_STRING_SECOND_SECURITY );
						g_pInstanceData->SetSendHackMail_AutoHack(true);

						g_pKTDXApp->SetFindHacking(true);
					}
#endif
					THEMIDA_VM_END
					VIRTUALIZER3_END
			}
			break;
#endif //SERV_SECOND_SECURITY
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID:
			{
				VIRTUALIZER4_START
				THEMIDA_VM_START
				
#ifndef NOT_SEND_MAIL
				g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_AntiHacking_MACHINE_ID_EMPTY );
#endif

				THEMIDA_VM_END
				VIRTUALIZER4_END
			}
			break;
#endif //SERV_MACHINE_ID_DUPLICATE_CHECK
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_NPC_DIE_TIME:
			{
				VIRTUALIZER5_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_29, "", false, true );
#else
					if( g_pMain != NULL && g_pInstanceData != NULL && g_pInstanceData->GetSendHackMail_AutoHack() == false )
					{
						g_pMain->SendHackMail_HackUserNot( ANTI_HACK_STRING_NPC_Dying_Check );
						g_pInstanceData->SetSendHackMail_AutoHack(true);
					}
#endif
				THEMIDA_VM_END
				VIRTUALIZER5_END
			}
			break;			
#endif //SERV_SUB_STAGE_NPC_DIE_CHECK
#ifdef SERV_ED_MONITORING_IN_GAME
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_SEND_MAIL:
			{
				VIRTUALIZER1_START
				THEMIDA_VM_START
				g_pMain->SendMail_MoneyMonitoring( "EM0" );
				THEMIDA_VM_END
				VIRTUALIZER1_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_ACCOUNT_BLOCK:
			{
				VIRTUALIZER2_START
				THEMIDA_VM_START
				g_pMain->SendMail_MoneyMonitoring( "EM1" );
				THEMIDA_VM_END
				VIRTUALIZER2_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_MACHINE_ID_BLOCK:
			{
				// ���� ũ����
				__asm
				{
					xor eax, eax
					call eax 
				}
			}
			break;
#endif SERV_ED_MONITORING_IN_GAME

#if defined(SERV_SERIAL_NUMBER_AVAILABILITY_CHECK) && defined(_SERVICE_)
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_CHECK_SUM_SEND_MAIL:	// SN üũ�� ����
			{			
				VIRTUALIZER3_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
				if( g_pMain != NULL )
					g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_30, "ChannelServer", false, false );
#else
				g_pMain->SendHackMail_HackUserNot( "-- Changed CS --" );
#endif
				THEMIDA_VM_END
				VIRTUALIZER3_END
			}
			break;
#ifndef NOT_SEND_MAIL
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_SEND_MAIL:	// SN ����
			{
				THEMIDA_VM_START
				g_pMain->SendHackMail_HackUserNot( "-- Not Match S --" );
				THEMIDA_VM_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_MACHINE_ID_SEND_MAIL:	// MID ����
			{
				THEMIDA_VM_START
				g_pMain->SendHackMail_HackUserNot( "-- Not Match M --" );
				THEMIDA_VM_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SN_AND_MID_SEND_MAIL:	// SN, MID ����
			{
				THEMIDA_VM_START
				g_pMain->SendHackMail_HackUserNot( "-- Not Match M.S --" );
				THEMIDA_VM_END
			}
			break;
#endif //NOT_SEND_MAIL
#endif //SERV_SERIAL_NUMBER_AVAILABILITY_CHECK

#if defined(SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER) //&& defined(_SERVICE_)
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL:	// GameServer���� sn�˻��̻�
			{
				VIRTUALIZER4_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_30, "GameServer", false, false );
#else
					g_pMain->SendHackMail_HackUserNot( "-- Changed GS --" );
#endif
				THEMIDA_VM_END
				VIRTUALIZER4_END
			}
			break;
#endif

#ifdef SERV_HACKING_DLL_CHECK
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL :
			{
				VIRTUALIZER5_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_31, "", false, false );
#else
					g_pMain->SendHackMail_HackUserNot( "-- ServCheckMod --" );
#endif
				THEMIDA_VM_END
				VIRTUALIZER5_END
			}
			break;
#endif
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_VERIFY_DUNGEON_NPC_UID:
			{
				VIRTUALIZER1_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_32, "", false, false );
#else
					g_pMain->SendHackMail_HackUserNot( "-- Invalid_Npc --" );
#endif
				THEMIDA_VM_END
				VIRTUALIZER1_END
			}
			break;
#endif
#ifdef SERV_CHECK_SEQUENCE_COUNT
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_SEQUENCE_COUNT:
			{
				VIRTUALIZER2_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_36, "", false, false );
#else
					g_pMain->SendHackMail_HackUserNot( "-- CHECK_SEQUENCE_COUNT --" );
#endif
				THEMIDA_VM_END
				VIRTUALIZER2_END
			}
			break;
#endif //SERV_CHECK_SEQUENCE_COUNT
#if defined(SERV_CHECK_KICK_USER_IN_GAMESERVER) && defined(ADD_COLLECT_CLIENT_INFO)
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_GAME_VERSION:
			{
				VIRTUALIZER3_START
				THEMIDA_VM_START

				if( g_pMain != NULL )
					g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_37, "", false, false );

				THEMIDA_VM_END
				VIRTUALIZER3_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_OVERLAP_ACCOUNT:
			{
				VIRTUALIZER4_START
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_38, "", false, false );

				THEMIDA_VM_END
				VIRTUALIZER4_START
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_FAST_LEVEL_UP:
			{
				VIRTUALIZER5_START
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_39, "", false, false );

				THEMIDA_VM_END
				VIRTUALIZER5_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_BLOCK_DLL:
			{
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_40, "", false, false );

				THEMIDA_VM_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_LOGOUT_ED_CHECK:
			{
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_41, "", false, false );

				THEMIDA_VM_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_BLOCK_MACHINE_ID:
			{
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_42, "", false, false );

				THEMIDA_VM_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_KICK_BY_GM:
			{
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_43, "", false, false );

				THEMIDA_VM_END
			}
			break;
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_EMPTY:
			{
				THEMIDA_VM_START

					if( g_pMain != NULL )
						g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_44, "", false, false );

				THEMIDA_VM_END
			}
			break;
#endif //SERV_CHECK_KICK_USER_IN_GAMESERVER			
#ifdef SERV_SERIAL_NUMBER_MORNITORING
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SERVER_SN_MONITORING:
			{
				VIRTUALIZER3_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_47, "", false, false );
#endif
				THEMIDA_VM_END
				VIRTUALIZER3_END
			}
			break; 
#endif //SERV_SERIAL_NUMBER_MORNITORING
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
 		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SERVER_SN_OVERLAP_CONNECT:				
 			{
 				THEMIDA_VM_START
 
 					//if( g_pMain != NULL )
 					//	g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_48, "", false, true );
 
 				THEMIDA_VM_END
 			}
 			break;
#endif //SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
#ifdef SERV_CHECK_MACHINE_LOCALTIME
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_LOCALTIME:				
			{
				THEMIDA_VM_START

 					if( g_pMain != NULL )
 						g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_49, "", false, true );
				THEMIDA_VM_END
			}
			break;
#endif //SERV_CHECK_MACHINE_LOCALTIME
#ifdef SERV_CHECK_X2_EXE_MODULE
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_X2_EXE_MODULE:				
			{
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_50, "", false, true );
#endif
				THEMIDA_VM_END
			}
			break;
#endif
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
		// �Ǹ� �Ұ� ������ �Ǹ� �õ� 
		case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_IMPOSSIBLE_SELL_ITEM:
			{
				VIRTUALIZER5_START
				THEMIDA_VM_START
#ifdef ADD_COLLECT_CLIENT_INFO
					if( g_pMain != NULL )
						g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_55, "", false, true );
#endif
				THEMIDA_VM_END
				VIRTUALIZER5_END
			}
			break;
#endif //SERV_IMPOSSIBLE_SELL_ITEM 


	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE:
	default:		
		break;
	}

	VIRTUALIZER2_START
	{
		KEGS_SERVER_CHECK_HACK_USER_ACK kPacketAck;
		if ( g_pData->GetServerProtocol() == NULL ||
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		    g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		{
			return true;
		}	
		kPacketAck.m_cHackingCheckType = kEvent.m_cHackingCheckType;
		g_pData->GetServerProtocol()->SendPacket( EGS_SERVER_CHECK_HACK_USER_ACK, kPacketAck );
	}
	VIRTUALIZER2_END

	ELSWORD_VIRTUALIZER_END


	return true;
}
#endif // SERV_PROTECT_AUTO_HACK



//{{ kimhc // 2010-08-16 // ������ �ϸ� �Ⱓ�� �þ�� �̺�Ʈ ���� ������ �ý����� ������ �� �Դϴ�.
#ifdef	SERV_RESET_PERIOD_EVENT
bool CX2State::Handler_EGS_RESET_PERIOD_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESET_PERIOD_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pData->GetMyUser() == NULL
		|| g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return false;

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	BOOST_TEST_FOREACH( KInventoryItemInfo&, val, kEvent.m_vecInventorySlotInfo )
	{
		CX2Item* pItem = kInventory.GetItem( val.m_iItemUID, true );

		if ( pItem != NULL )
		{
			CX2Item::ItemData& kItemData = pItem->AccessItemData();
			kItemData = val;			
		}
	}

	return true;
}
#endif	SERV_RESET_PERIOD_EVENT
//}} kimhc // 2010-08-16 // ������ �ϸ� �Ⱓ�� �þ�� �̺�Ʈ ���� ������ �ý����� ������ �� �Դϴ�.

#ifdef SERV_ADMIN_SHOW_ED
bool CX2State::Handler_EGS_ADMIN_SHOW_ED_CHEAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_SHOW_ED_CHEAT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if( g_pMain != NULL && g_pMain->GetGameEdit() != NULL )
		{
			wstring wstrAdminShowEdCheat = kEvent.m_wstrNickName;
			wstrAdminShowEdCheat += L" : ";
			WCHAR wBuf[100];
			StringCchPrintf(wBuf, 100, L"%d", kEvent.m_iED);
			//wsprintf(wBuf, L"%d", kEvent.m_iED);
			wstrAdminShowEdCheat += g_pMain->GetEDString( wBuf );

			g_pMain->GetGameEdit()->AddString( CX2ChatWindow::CT_NORMAL, wstrAdminShowEdCheat.c_str() );
			//g_pChatBox->AddChatLog( wstrAdminShowEdCheat.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1.f, 0.f, 0.f, 0.f), L"#CFF0000" );
		}	

		return true;
	}

	return false;	
}
#endif

//{{ 2012. 07. 09	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
bool CX2State::Handler_ECH_WAIT_QUEUE_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_WAIT_QUEUE_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// �̹� �޽��� �ڽ��� �������� ������!
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	// ���ο� �޽��� �ڽ� ����!
	if( kEvent.m_bPassed == false )
	{
		wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_17900, "i", kEvent.m_iWaitUserCount ) );
		m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrBuff.c_str(), this );
	}

	//CKTDGUIDialogType pDLGOK = KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF(enumID), m_pNowState, CX2StateCreateUnit::SCUUCM_SHUT_DOWN_OK );	
	//CKTDGUIDialogType pDLGOK = KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_16527 ), g_pData->GetUIManager()->GetUIInventory()->UIM_ITEM_USE_DEFENCE_DUNGEON_POTION, g_pMain->GetNowState() );

	return true;
}
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
bool CX2State::Handler_EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ()
{
#ifdef SKILL_PAGE_SYSTEM //JHKang
	KEGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ kPacket;
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK );
#else //SKILL_PAGE_SYSTEM
	g_pData->GetServerProtocol()->SendID( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ );
	g_pMain->AddServerPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK );
#endif //SKILL_PAGE_SYSTEM

	return true;
}

bool CX2State::Handler_EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

				//if( NULL != pUnitData )
				{
					pUnitData->m_UserSkillTree.Reset( true, true, true, true );

					//��ų ���� ������Ʈ
					for( std::map<int, KAdminCheatSkill>::iterator mit = kEvent.m_mapSkillInfo.begin();
						mit != kEvent.m_mapSkillInfo.end(); ++mit )
					{
					#ifdef SKILL_PAGE_SYSTEM //JHKang
						pUnitData->m_UserSkillTree.SetSkillLevelAndCSP ( static_cast<CX2SkillTree::SKILL_ID>(mit->first),
							mit->second.m_iSkillLevel, mit->second.m_iSkillCSPoint, pUnitData->m_UserSkillTree.GetUsingPage() );
					#else //SKILL_PAGE_SYSTEM
						pUnitData->m_UserSkillTree.SetSkillLevelAndCSP ( static_cast<CX2SkillTree::SKILL_ID>(mit->first), mit->second.m_iSkillLevel, mit->second.m_iSkillCSPoint );
					#endif //SKILL_PAGE_SYSTEM
					}
					
					//���� ��ų ����
					pUnitData->m_UserSkillTree.SetUnsealedSkill(kEvent.m_vecUnSealedSkill);

				}
			}

			// ��ųƮ�� UI ������Ʈ
			if( g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
			}
			return true;
		}
	}

	return false;
}
#endif //SERV_ADMIN_CHEAT_GET_ALL_SKILL

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

/** @function	: Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ
	@brief		: ���� ������ �ø� �� �ִ� ��ų Ȱ��ȭ ��� ������ ��û
	@return		: ���� ����
*/
bool CX2State::Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ()
{
#ifdef SKILL_PAGE_SYSTEM //JHKang
	KEGS_ADMIN_AUTO_GET_ALL_SKILL_REQ kPacket;
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK );
#else //SKILL_PAGE_SYSTEM
	g_pData->GetServerProtocol()->SendID( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ );
	g_pMain->AddServerPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK );
#endif //SKILL_PAGE_SYSTEM

	return true;
}

/** @function	: Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK
	@brief		: ���� ������ �ø� �� �ִ� ��ų Ȱ��ȭ ��� ��Ŷ ó��
	@return		: ���� ����
*/
bool CX2State::Handler_EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

				//if( NULL != pUnitData )
				{
					pUnitData->m_UserSkillTree.Reset( true, true, true, true );

					//��ų ���� ������Ʈ
					for( std::map<int, int>::iterator mit = kEvent.m_mapSkillList.begin();
						mit != kEvent.m_mapSkillList.end(); ++mit )
					{
					#ifdef SKILL_PAGE_SYSTEM //JHKang
						pUnitData->m_UserSkillTree.SetSkillLevelAndCSP ( static_cast<CX2SkillTree::SKILL_ID>(mit->first),
							mit->second, 0, pUnitData->m_UserSkillTree.GetUsingPage() );
					#else //SKILL_PAGE_SYSTEM
						pUnitData->m_UserSkillTree.SetSkillLevelAndCSP ( static_cast<CX2SkillTree::SKILL_ID>(mit->first), mit->second, 0 );
					#endif //SKILL_PAGE_SYSTEM
					}

					//���� ��ų ����
					pUnitData->m_UserSkillTree.SetUnsealedSkill( kEvent.m_vecUnsealedSkillID );

					pUnitData->m_iSPoint	= kEvent.m_iSPoint;
					pUnitData->m_iCSPoint	= kEvent.m_iCSPoint;
				}
			}

			// ��ųƮ�� UI ������Ʈ
			if( g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
			}
			return true;
		}
	}

	return false;
}

/** @function	: Handler_EGS_ADMIN_GET_SKILL_REQ
	@brief		: Ư�� ��ų�� ������ �����ϴ� ��� ������ ��û
	@return		: ���� ����
*/
bool CX2State::Handler_EGS_ADMIN_GET_SKILL_REQ( const int iSkillID, const int iSkillLevel )
{
	KEGS_ADMIN_GET_SKILL_REQ kPacket;

	kPacket.m_iSkillID		= iSkillID;
	kPacket.m_iSkillLevel	= iSkillLevel;
#ifdef SKILL_PAGE_SYSTEM //JHKang
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;
#endif //SKILL_PAGE_SYSTEM

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_GET_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_GET_SKILL_ACK );

	return true;
}

/** @function	: Handler_EGS_ADMIN_GET_SKILL_ACK
	@brief		: Ư�� ��ų�� ������ �����ϴ� ��� ��Ŷ ó��
	@return		: ���� ����
*/
bool CX2State::Handler_EGS_ADMIN_GET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_GET_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_GET_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

				//if( NULL != pUnitData )
				{
					CX2SkillTree::SKILL_ID eSkillID =  static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iSkillID );
					//��ų ���� ������Ʈ	
				#ifdef SKILL_PAGE_SYSTEM //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP ( eSkillID, kEvent.m_iSkillLevel, kEvent.m_iCSPoint,
						pUnitData->m_UserSkillTree.GetUsingPage() );
				#else //SKILL_PAGE_SYSTEM
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP ( eSkillID, kEvent.m_iSkillLevel, kEvent.m_iCSPoint );
				#endif //SKILL_PAGE_SYSTEM

					//���� ��ų ����
					if ( true == kEvent.m_bUnsealed )
						pUnitData->m_UserSkillTree.AddSkillUnsealed( eSkillID );

				}
			}

			// ��ųƮ�� UI ������Ʈ
			if( g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
			}
			return true;
		}
	}

	return false;
}

#endif // UPGRADE_SKILL_SYSTEM_2013

//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
bool CX2State::Handler_EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( !g_pMain->IsValidPacket( kEvent.m_iOK ) )	/// OK �� �ƴ� ��� �ðܳ��� ó���� �Ѵ�.
	{

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
		if ( NULL != g_pInstanceData )
		{
			g_pInstanceData->InitFieldWorkingSetBlockLog();
			g_pInstanceData->SetNowX2StateToBlockLog( static_cast<const char>( g_pMain->GetNowStateID() ) );
			
			PROCESS_MEMORY_COUNTERS_EX pmcex;
			GetProcessMemoryInfo( GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmcex, sizeof(pmcex)); 

			const UINT MEGA = 1024 * 1024;			
			g_pInstanceData->SetPhysicalMemoryToBlockLog( static_cast<const int>( pmcex.WorkingSetSize / MEGA ) );
			g_pInstanceData->SetVirtualMemoryToBlockLog( static_cast<const int>( pmcex.PrivateUsage / MEGA ) );
			
			if ( NULL != g_pX2Game )
			{
				/// ������ �ʵ��� ��츸 ������� ������, Game �ΰ�쿡�� �� ������ ��.
				g_pInstanceData->SetFieldUserNum( g_pX2Game->GetUserUnitNum() );
				g_pInstanceData->SetFieldUserNum( g_pX2Game->GetNPCUnitNum() );
			}

		}
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG


		if( NULL != g_pData && NULL != g_pData->GetMyUser() && 
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{		
			Handler_EGS_STATE_CHANGE_FIELD_REQ();
		}
	}

	return true;
}

bool CX2State::Handler_EGS_FORCE_RELAY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FORCE_RELAY_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pData->GetGameUDP() != NULL )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->ForceUseRelay( kPacket.m_iTargetUnitUID );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( kPacket.m_iTargetUnitUID );
//        if ( pPeer != NULL )
//        {
//			pPeer->m_bUseRelay = true;
//        }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

	return true;
}

#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#ifdef UDP_DOWNLOAD_BLOCK_CHECK
bool CX2State::Handler_EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		if( kEvent.m_iAccusationUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID()  )
		{
			CX2GageManager::GetInstance()->ClearPvpMemberUI();
			Handler_EGS_STATE_CHANGE_FIELD_REQ();
		}
		else
		{
			if( NULL != g_pX2Game )
			{
				g_pX2Game->EraseAbuserUserCheckByUID(kEvent.m_iAccusationUnitUID);
			}
		}
	}

	return true;
}
#endif //UDP_DOWNLOAD_BLOCK_CHECK

//{{ ����� : [2011/3/23/] //	����̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
void CX2State::OpenStartMission()
{
#ifdef COME_BACK_REWARD
#ifdef NEW_EMBLEM_MANAGER
	CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_DEFENCE_START);
#else
	SAFE_DELETE_DIALOG( m_pDLGWorldMissionBuffName );
	m_pDLGWorldMissionBuffName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Mission_Buff_Title.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWorldMissionBuffName );
	CKTDGUIStatic *pStatic = m_pDLGWorldMissionBuffName->GetStatic_LUA("World_Mission_Buff_Title");

	CKTDGUIControl::CPictureData *pFieldWorldMissionStart = pStatic->GetPictureIndex( 120 );

	if(pFieldWorldMissionStart != NULL)
	{
		if( NULL != g_pData->GetWorldMissionManager() && true == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
		{
			pFieldWorldMissionStart->SetShow( true );
		}
	}
#endif // NEW_EMBLEM_MANAGER


	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Attack_Alert.ogg", false, false );
#else
	SAFE_DELETE_DIALOG( m_pDLGWorldMissionBuffName );

	m_pDLGWorldMissionBuffName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Field_Title.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWorldMissionBuffName );		

	m_fShowTimeForWorldMissionBuffName = 0.f;
	m_pDLGWorldMissionBuffName->SetShowEnable(true, true);

	CKTDGUIStatic *pStaticDlg = m_pDLGWorldMissionBuffName->GetStatic_LUA("Field_Title");
	CKTDGUIControl::CPictureData *pFieldName = NULL;
	CKTDGUIControl::CPictureData *pFieldNameBar = pStaticDlg->GetPictureIndex(99);
	CKTDGUIControl::CPictureData *pFieldNameBg = pStaticDlg->GetPictureIndex(100);
	CKTDGUIControl::CPictureData *pFieldNameBg1 = pStaticDlg->GetPictureIndex(101);
	int nFieldMax = 0;
	for(int i=1; i<=nFieldMax; ++i)
	{
		pFieldName = pStaticDlg->GetPictureIndex(i);

		pFieldName->SetShow(false);
	}
	pFieldNameBar->SetShow(false);
	pFieldNameBg->SetShow(false);
	pFieldNameBg1->SetShow(false);

	// kimhc // �׳�ý��� �ູ �Ⱓ�� �˸�
	CKTDGUIControl::CPictureData *pFieldGnosisNoticeBg	= pStaticDlg->GetPictureIndex( 102 );
	if(pFieldGnosisNoticeBg != NULL)
		pFieldGnosisNoticeBg->SetShow( false );

	CKTDGUIControl::CPictureData *pFieldWorldMissionStart = pStaticDlg->GetPictureIndex( 120 );

	if(pFieldWorldMissionStart != NULL)
	{
		if( NULL != g_pData->GetWorldMissionManager() && true == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
		{
			pFieldWorldMissionStart->SetShow( true );

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Attack_Alert.ogg", false, false );
		}
	}

	CKTDGUIControl::CPictureData *pFieldWorldMissionSuccess = pStaticDlg->GetPictureIndex( 121 );
	if(pFieldWorldMissionSuccess != NULL)
		pFieldWorldMissionSuccess->SetShow(false);

	CKTDGUIControl::CPictureData *pFieldWorldMissionFailed = pStaticDlg->GetPictureIndex( 122 );
	if(pFieldWorldMissionFailed != NULL)
		pFieldWorldMissionFailed->SetShow(false);
	//}}
#endif
}

void CX2State::OpenResultMission( bool bIsClear )
{
#ifdef COME_BACK_REWARD
	SAFE_DELETE_DIALOG( m_pDLGWorldMissionBuffName );
	m_pDLGWorldMissionBuffName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Mission_Buff_Title.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWorldMissionBuffName );
	CKTDGUIStatic *pStatic = m_pDLGWorldMissionBuffName->GetStatic_LUA("World_Mission_Buff_Title");

	if( bIsClear == true )
	{
#ifdef NEW_EMBLEM_MANAGER
		CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_DEFENCE_SUCCESS);
#else
		CKTDGUIControl::CPictureData *pFieldWorldMissionSuccess = pStatic->GetPictureIndex( 121 );
		if(pFieldWorldMissionSuccess != NULL)
			pFieldWorldMissionSuccess->SetShow(true);
#endif // NEW_EMBLEM_MANAGER


		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Defence_Success.ogg", false, false );
	}
	else
	{
#ifdef NEW_EMBLEM_MANAGER
		CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_DEFENEC_FAIL);
#else
		CKTDGUIControl::CPictureData *pFieldWorldMissionFailed = pStatic->GetPictureIndex( 122 );
		if(pFieldWorldMissionFailed != NULL)
			pFieldWorldMissionFailed->SetShow(true);
#endif // NEW_EMBLEM_MANAGER


		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Defence_Failed.ogg", false, false );
	}
#else
	SAFE_DELETE_DIALOG( m_pDLGWorldMissionBuffName );

	m_pDLGWorldMissionBuffName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Field_Title.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWorldMissionBuffName );		

	m_fShowTimeForWorldMissionBuffName = 0.f;
	m_pDLGWorldMissionBuffName->SetShowEnable(true, true);

	CKTDGUIStatic *pStaticDlg = m_pDLGWorldMissionBuffName->GetStatic_LUA("Field_Title");
	CKTDGUIControl::CPictureData *pFieldName = NULL;
	CKTDGUIControl::CPictureData *pFieldNameBar = pStaticDlg->GetPictureIndex(99);
	CKTDGUIControl::CPictureData *pFieldNameBg = pStaticDlg->GetPictureIndex(100);
	CKTDGUIControl::CPictureData *pFieldNameBg1 = pStaticDlg->GetPictureIndex(101);

	int nFieldMax = 0;
	for(int i=1; i<=nFieldMax; ++i)
	{
		pFieldName = pStaticDlg->GetPictureIndex(i);

		pFieldName->SetShow(false);
	}
	pFieldNameBar->SetShow(false);
	pFieldNameBg->SetShow(false);
	pFieldNameBg1->SetShow(false);

	// kimhc // �׳�ý��� �ູ �Ⱓ�� �˸�
	CKTDGUIControl::CPictureData *pFieldGnosisNoticeBg	= pStaticDlg->GetPictureIndex( 102 );
	if(pFieldGnosisNoticeBg != NULL)
		pFieldGnosisNoticeBg->SetShow( false );

	CKTDGUIControl::CPictureData *pFieldWorldMissionStart = pStaticDlg->GetPictureIndex( 120 );

	if(pFieldWorldMissionStart != NULL)
		pFieldWorldMissionStart->SetShow( false );


	if( bIsClear == true )
	{
		CKTDGUIControl::CPictureData *pFieldWorldMissionSuccess = pStaticDlg->GetPictureIndex( 121 );
		if(pFieldWorldMissionSuccess != NULL)
			pFieldWorldMissionSuccess->SetShow(true);

		CKTDGUIControl::CPictureData *pFieldWorldMissionFailed = pStaticDlg->GetPictureIndex( 122 );
		if(pFieldWorldMissionFailed != NULL)
			pFieldWorldMissionFailed->SetShow(false);

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Defence_Success.ogg", false, false );
	}
	else
	{
		CKTDGUIControl::CPictureData *pFieldWorldMissionSuccess = pStaticDlg->GetPictureIndex( 121 );
		if(pFieldWorldMissionSuccess != NULL)
			pFieldWorldMissionSuccess->SetShow(false);

		CKTDGUIControl::CPictureData *pFieldWorldMissionFailed = pStaticDlg->GetPictureIndex( 122 );
		if(pFieldWorldMissionFailed != NULL)
			pFieldWorldMissionFailed->SetShow(true);

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Defence_Failed.ogg", false, false );
	}
	//}}
#endif
}

void CX2State::MoveShowEvent(float fTime)
{
// 	if( fTime <= 0.f)
// 	{
// 		m_pDLGWorldMissionBuffName->SetShowEnable(false, false);
// 		return;
// 	}

	m_pDLGWorldMissionBuffName->Move(m_pDLGWorldMissionBuffName->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 0.75f, true);
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} ����� : [2011/3/23/] //	����̼�

#ifdef SERV_PSHOP_AGENCY
bool CX2State::Handler_EGS_PSHOP_AGENCY_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PSHOP_AGENCY_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	switch(kEvent.m_cMessageType)
	{
	case KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_SELL_ITEM:
		{
			// �����Ǹ�
			if( g_pChatBox != NULL )
			{
				if( g_pData != NULL && g_pData->GetItemManager() != NULL  )
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_iSellItemID );
					if( pItemTemplet != NULL && pItemTemplet->GetPeriodType() == CX2Item::PT_QUANTITY )
					{
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
						if ( SEnum::AST_PREMIUM != g_pInstanceData->GetAgencyShopType() )
							g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_28432, "Sii", pItemTemplet->GetFullName_(), kEvent.m_iSellQuantity, kEvent.m_iSellED )),
							KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						else
		#endif //SERV_UPGRADE_TRADE_SYSTEM
							g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_12141, "Sii", pItemTemplet->GetFullName_(), kEvent.m_iSellQuantity, kEvent.m_iSellED )),
								KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
							
					}
					else if( pItemTemplet != NULL && pItemTemplet->GetPeriodType() != CX2Item::PT_QUANTITY )
					{
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
						if ( SEnum::AST_PREMIUM != g_pInstanceData->GetAgencyShopType() )
							g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_28431, "Si", pItemTemplet->GetFullName_(), kEvent.m_iSellED )),
							KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						else
		#endif //SERV_UPGRADE_TRADE_SYSTEM
						g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_12140, "Si", pItemTemplet->GetFullName_(), kEvent.m_iSellED )),
							KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
					}
				}				
			}
		}
		break;
	case KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_SOLD_OUT:
		{
			// �ǸſϷ�
			if( g_pChatBox != NULL )
			{
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_12143 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
		}
		break;
	case KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_EXPIRATION:
		{
			// ������ �Ⱓ����
			if( g_pChatBox != NULL )
			{
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_12145 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
			if( g_pInstanceData != NULL )
			{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
				g_pInstanceData->SetPShopAgencyInfo(false, L"", SEnum::AST_NONE );
	#else // SERV_UPGRADE_TRADE_SYSTEM
				g_pInstanceData->SetPShopAgencyInfo(false, L"");
	#endif // SERV_UPGRADE_TRADE_SYSTEM
			}
		}
		break;
	case KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_OVER_24_HOURS:
		{
			// �븮���� �ǸŽð� ����
			if( g_pChatBox != NULL )
			{
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_12142 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}
		}
		break;
	}

	return true;
}

bool CX2State::Handler_EGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pInstanceData != NULL )
	{		
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		g_pInstanceData->SetPShopAgencyInfo( g_pInstanceData->GetIsPShopOpen(), 
											 kEvent.m_wstrAgencyExpirationDate, 
											 static_cast<SEnum::AGENCY_SHOP_TYPE>( kEvent.m_cShopType ) );

		/// ��¥ �븮 ���� ��� ���̸�, ä��â�� ���� �Ⱓ ������� ����
		if( g_pChatBox != NULL && SEnum::AST_FREE != g_pInstanceData->GetAgencyShopType() )
#else // SERV_UPGRADE_TRADE_SYSTEM
		g_pInstanceData->SetPShopAgencyInfo( g_pInstanceData->GetIsPShopOpen(), kEvent.m_wstrAgencyExpirationDate );

		if( g_pChatBox != NULL )
#endif // SERV_UPGRADE_TRADE_SYSTEM
		{
			wstring wstrAgencyShopExpirationDate = g_pInstanceData->GetAgencyShopExpirationDate();
			if( wstrAgencyShopExpirationDate.empty() == false )
			{
				CTime	tCurrentTime	= CTime::GetCurrentTime();
				CTime	cAgencyTime;
				KncUtil::ConvertStringToCTime( wstrAgencyShopExpirationDate, cAgencyTime );
				if( tCurrentTime < cAgencyTime )
				{
					// ���� �ð�
					g_pChatBox->AddChatLog( GET_STRING( STR_ID_12146 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );


		#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
					wstring wstrExpirationDesc = GetExpirationDataDesc( g_pInstanceData->GetAgencyShopType(), wstrAgencyShopExpirationDate );

						g_pChatBox->AddChatLog( wstrExpirationDesc.c_str(),
						KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		#else // SERV_UPGRADE_TRADE_SYSTEM
					g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_12234, "L", 
						GetExpirationDateDesc( wstrAgencyShopExpirationDate, g_pData->GetServerCurrentTime(), false ) ) ),
						KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		#endif // SERV_UPGRADE_TRADE_SYSTEM
				}	
			}		
		}
		return true;
	}

	return false;
}
#endif //SERV_PSHOP_AGENCY

#ifdef NEW_HENIR_TEST
bool CX2State::Handler_EGS_HENIR_REWARD_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_HENIR_REWARD_COUNT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pData && 
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{		
		g_pData->GetMyUser()->GetSelectUnit()->SetHenirRewardCountInfo( kEvent );
		return true;
	}
	return false;
}
#endif NEW_HENIR_TEST

#ifdef SERV_RANDOM_DAY_QUEST
bool CX2State::Handler_EGS_CHANGE_RANDOM_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_RANDOM_QUEST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pData && 
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{		
		g_pData->GetQuestManager()->SetRandomQuestInfo(kEvent.m_mapRandomQuestList);
	}
	return false;
}
#endif SERV_RANDOM_DAY_QUEST
bool CX2State::Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( g_pX2Game != NULL )
		return false;

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_UNIT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_kUnitInfo.m_nUnitUID )
	{
		g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUnitInfo );
	}
	return true;
}

#if defined(SERV_MACHINE_ID_BLOCK) && defined(MACHINE_ID)

void CX2State::Handler_EGS_CHECK_MACHINE_ID_REQ()
{
	ELSWORD_VIRTUALIZER_START

	KEGS_CHECK_MACHINE_ID_REQ kPacket;

#ifndef SERV_MACHINE_ID_DUPLICATE_CHECK
	kPacket.m_strMachineID = g_pMain->GetMachineId();

	if( g_pInstanceData != NULL )
		g_pInstanceData->SetMachineId( kPacket.m_strMachineID );
#else
	kPacket.m_strMachineID = g_pMain->GetMachineId();

	if( g_pInstanceData != NULL )
		g_pInstanceData->SetMachineId( kPacket.m_strMachineID );	
#endif

	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_MACHINE_ID_REQ, kPacket );	
	//g_pMain->AddServerPacket( EGS_CHECK_MACHINE_ID_ACK, 60 );
	
	ELSWORD_VIRTUALIZER_END
}
bool CX2State::Handler_EGS_CHECK_MACHINE_ID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ELSWORD_VIRTUALIZER_START

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_MACHINE_ID_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	//if( g_pMain->DeleteServerPacket( EGS_CHECK_MACHINE_ID_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			std::string wstrMachineId = g_pMain->GetMachineId();
			if( kEvent.m_strMachineID.compare( wstrMachineId ) != 0 )
			{
				// �ӽ�id�� ����ġ�Ѵ�. �������� �Ǵ�.
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
#ifdef ADD_COLLECT_CLIENT_INFO			
				g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_20, "", true, true );
#else
				g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_ID_VERIFY );
				g_pKTDXApp->SetFindHacking( true );
#endif
			}
		}
	}

	ELSWORD_VIRTUALIZER_END
	return true;
}
#endif //MACHINE_ID


#ifdef SERV_SHUTDOWN_SYSTEM_AUTO_CHECK
bool CX2State::Handler_EGS_SHUT_DOWN_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SHUT_DOWN_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	switch ( kEvent.m_iNetErrorID )
	{
	case NetError::ERR_SHUT_DOWN_00:
	case NetError::ERR_SHUT_DOWN_04:
	case NetError::ERR_SHUT_DOWN_05:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,450), NetError::GetErrStrF(kEvent.m_iNetErrorID), this, SUCM_EXIT );
			return true;
		} break;

	default:
		return false;
	}
}
#endif SERV_SHUTDOWN_SYSTEM_AUTO_CHECK

/** @function : Handler_EGS_JOIN_BATTLE_FIELD_REQ
	@brief : ������, ����*���� ���� �� �ʵ�� ���ƿö� �� �ʵ��� �Ա� ���� ��ġ�� �ƴ� custom ��ġ�� �̵��ϴ� ��쿡 ����ϴ� ��Ŷ ���� �Լ�
	@param : const UINT uiBattleFieldId_(�̵��� �ʵ� ID), const USHORT usBattleFieldLineIndex_(�̵��� �ʵ��� ���θ� ��ȣ), const USHORT usBattleFieldPositionValue_(���θ� ��ȣ ���� ��ġ)
	@return : void
*/
void CX2State::Handler_EGS_JOIN_BATTLE_FIELD_REQ( const UINT uiBattleFieldId_, const USHORT usBattleFieldLineIndex_, const USHORT usBattleFieldPositionValue_, const bool bMoveToMyParty_ /*= false*/ )
{
	KEGS_JOIN_BATTLE_FIELD_REQ kPacket;

	ASSERT( 0 != uiBattleFieldId_ );
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo.m_iBattleFieldID = uiBattleFieldId_;
    kPacket.m_kBattleFieldJoinInfo.m_iStartPosIndex = usBattleFieldLineIndex_;
    kPacket.m_kBattleFieldJoinInfo.m_bMoveForMyParty = bMoveToMyParty_;
    kPacket.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition = false;
    kPacket.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue = usBattleFieldPositionValue_;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iBattleFieldID	= uiBattleFieldId_;
	kPacket.m_StartPosIndex		= usBattleFieldLineIndex_;
	kPacket.m_bMoveForMyParty	= bMoveToMyParty_;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX


#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	// �ʵ�->�ʵ� �̵� �� ������ ��� ����
	if( NULL != g_pX2Game )
		g_pX2Game->SendFrameAverage();

	CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	battleFieldManager.SetBattleFieldIdWhereIam( uiBattleFieldId_ );
	battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( usBattleFieldLineIndex_ );
	battleFieldManager.SetBattleFieldPositionValue( usBattleFieldPositionValue_ );
	battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( false );

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_BATTLE_FIELD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_BATTLE_FIELD_ACK ); 
}

/** @function : Handler_EGS_JOIN_BATTLE_FIELD_REQ
	@brief : �ʵ��� �Ա� ���� ��ġ�� �̵��ϴ� ��쿡 ����ϴ� ��Ŷ ���� �Լ�
	@param : const UINT uiBattleFieldId_(�̵��� �ʵ�ID), const USHORT usBattleFieldStartLineIndex_(������ LineMap Index)
	@return : void
*/
void CX2State::Handler_EGS_JOIN_BATTLE_FIELD_REQ( const UINT uiBattleFieldId_, const USHORT usBattleFieldStartLineIndex_ ) const
{
	KEGS_JOIN_BATTLE_FIELD_REQ kPacket;
	
	CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();

	ASSERT( 0 != uiBattleFieldId_ );
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo.m_iBattleFieldID = uiBattleFieldId_;
    kPacket.m_kBattleFieldJoinInfo.m_iStartPosIndex = usBattleFieldStartLineIndex_;
    kPacket.m_kBattleFieldJoinInfo.m_bMoveForMyParty = false;
    kPacket.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition = true;
    kPacket.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue = battleFieldManager.GetBattleFieldPositionValue();
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iBattleFieldID	= uiBattleFieldId_;
	kPacket.m_StartPosIndex		= usBattleFieldStartLineIndex_;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	// �ʵ�->�ʵ� �̵� �� ������ ��� ����
	if( NULL != g_pX2Game )
		g_pX2Game->SendFrameAverage();

	battleFieldManager.SetBattleFieldIdWhereIam( uiBattleFieldId_ );
	battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( usBattleFieldStartLineIndex_ );
	battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( true );

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_BATTLE_FIELD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_BATTLE_FIELD_ACK ); 
}

bool CX2State::Handler_EGS_JOIN_BATTLE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_JOIN_BATTLE_FIELD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_JOIN_BATTLE_FIELD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )	
		{
			ASSERT( 0 != kEvent.m_RoomInfo.m_iBattleFieldID );
			
			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_SERVER_SELECT:
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_PVP_GAME:
			case CX2Main::XS_DUNGEON_GAME:
			case CX2Main::XS_PVP_RESULT:
			case CX2Main::XS_DUNGEON_RESULT:
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
				{
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	                CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	                battleFieldManager.SetBattleFieldIdWhereIam( kEvent.m_kBattleFieldJoinInfo.m_iBattleFieldID );
                    battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( kEvent.m_kBattleFieldJoinInfo.m_iStartPosIndex );
                    battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( kEvent.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition );
                    battleFieldManager.SetBattleFieldPositionValue( kEvent.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue );

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef HEAP_BROKEN_BY_ROOM
					CX2Room::InitializeRoomPacketData();
					CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
						vector<KRoomSlotInfo>(), kEvent.m_wstrCNIP );
#else	HEAP_BROKEN_BY_ROOM
					g_pData->DeleteDungeonRoom();
					g_pData->DeletePVPRoom();

					CX2BattleFieldRoom* pBattleFieldRoom = g_pData->ResetBattleFieldRoom();
					pBattleFieldRoom->Set_KRoomInfo( kEvent.m_RoomInfo );	//ok
					pBattleFieldRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );						// fix!! bUnitInfo == false??? 
					pBattleFieldRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort ); // relay connect 
					pBattleFieldRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
#endif // HEAP_BROKEN_BY_ROOM

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_BATTLE_FIELD ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


					g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_BATTLE_FIELD, NULL, false );

					if( NULL != g_pTFieldGame )
					{
						g_pTFieldGame->SetNearPortalToBattleField( false );
					}
				} break;

			// �ʵ�->�ʵ� �̵�
			case CX2Main::XS_BATTLE_FIELD:
				{
					CX2StateBattleField* pStateBattleField = static_cast<CX2StateBattleField*>( g_pMain->GetNowState() );
                    ASSERT( pStateBattleField != NULL );

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

                    if ( pStateBattleField != NULL )
                        pStateBattleField->FlushSendFrameAverage();

	                CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	                battleFieldManager.SetBattleFieldIdWhereIam( kEvent.m_kBattleFieldJoinInfo.m_iBattleFieldID );
                    battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( kEvent.m_kBattleFieldJoinInfo.m_iStartPosIndex );
                    battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( kEvent.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition );
                    battleFieldManager.SetBattleFieldPositionValue( kEvent.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue );

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

					CX2BattleFieldRoom* pBattleFieldRoom = g_pData->GetBattleFieldRoom();
                    ASSERT( pBattleFieldRoom != NULL );
					
					pBattleFieldRoom->Set_KRoomInfo( kEvent.m_RoomInfo );	//ok
					pBattleFieldRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );						// fix!! bUnitInfo == false??? 
					pBattleFieldRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort ); // relay connect 
					pBattleFieldRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_BATTLE_FIELD ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

					pStateBattleField->MoveFromBattleFieldToOtherBattleField();

#ifdef FIELD_BOSS_RAID
					// �ʵ�->�ʵ� ���忡 ���� ����ó��
					CX2BossRaidManager::GetInstance()->JoinFieldProcess( static_cast<float>(kEvent.m_tRemainFieldHoldingTime) );
#endif // FIELD_BOSS_RAID									
				} break;

			default:
				ASSERT( !L"EGS_JOIN_BATTLE_FIELD_ACK" );
				StateLog( L"EGS_JOIN_BATTLE_FIELD_ACK" );
				break;
			}
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD		
			//�ʵ� �����, �ش� �ʵ� ���� ĳ���� �������� ������ ���� ó��
			if( NULL != g_pData && NULL != g_pData->GetCashShop() )
				g_pData->GetCashShop()->SetEtnerCashShopUserList(kEvent.m_vecEnterCashShopUser);
#endif //SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
			if ( NULL != g_pInstanceData )
				g_pInstanceData->RestartTimerFromJoinBattleToGameLoading();
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

			return true;
		}
		else
		{
			switch ( kEvent.m_iOK )
			{
			case NetError::ERR_BATTLEFIELD_13:
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING((STR_ID_18892, "i",kEvent.m_iRequireLevel)) , g_pMain->GetNowState() );
				} break;
			case NetError::ERR_BATTLEFIELD_14:
				{
					if( NULL != g_pData && NULL != g_pData->GetDungeonManager() )
					{
						wstring DungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( static_cast<SEnum::DUNGEON_ID>(kEvent.m_iRequireDungeonID));
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 	GET_REPLACED_STRING((STR_ID_18893, "L", DungeonName )),	g_pMain->GetNowState() );
					}
				} break;
			}
		}
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{
		g_pData->GetRelationshipEffectManager()->SetMyRelationCloseEffectCooltime( 0.f );
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

	return false;
}

/*virtual*/ bool CX2State::Handler_EGS_DROP_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DROP_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if ( NULL != g_pX2Game )
	{
		g_pX2Game->Handler_EGS_DROP_ITEM_NOT( kEvent );
		return true;
	}
	else
	{
		StateLog( L"g_pX2Game is NULL In Handler_EGS_DROP_ITEM_NOT" );
		ASSERT( NULL != g_pX2Game );
		return false;
	}
}

/*virtual*/ bool CX2State::Handler_EGS_GET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if ( NULL != g_pX2Game )
	{
		g_pX2Game->Handler_EGS_GET_ITEM_ACK( kEvent );
		return true;
	}
	else
	{
		StateLog( L"g_pX2Game is NULL In Handler_EGS_GET_ITEM_ACK" );
		ASSERT( NULL != g_pX2Game );
		return false;
	}
	
}

/*virtual*/ bool CX2State::Handler_EGS_GET_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if ( NULL != g_pX2Game )
	{
		g_pX2Game->Handler_EGS_GET_ITEM_NOT( kEvent );
		return true;
	}
	else
	{
		StateLog( L"g_pX2Game is NULL In Handler_EGS_GET_ITEM_NOT" );
		ASSERT( NULL != g_pX2Game );
		return false;
	}
}

/*virtual*/ bool CX2State::Handler_EGS_GET_ITEM_REALTIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_ITEM_REALTIME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

	// �����ִ��� Ȯ�� �� ������Ʈ 
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
	}

#ifndef SERV_EPIC_QUEST
	if ( g_pData->GetUIManager() == NULL ||
		g_pData->GetUIManager()->GetUIQuest() == NULL )
		return true;
#endif SERV_EPIC_QUEST

	BOOST_TEST_FOREACH( const KInventoryItemInfo&, itemInfo, kEvent.m_vecKInventorySlotInfo  )
	{
		if( itemInfo.m_iItemUID > 0 && 
			g_pData->GetQuestManager() != NULL )
		{
			g_pData->GetQuestManager()->CheckItemCollection( itemInfo.m_kItemInfo.m_iItemID );
		}
	}

#ifdef SERV_EPIC_QUEST
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
	}

	g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
#else
	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST ) == true )
	{
		g_pData->GetUIManager()->GetUIQuest()->UpdateQuestUI();
	}

	if ( g_pData->GetUIManager()->GetUIQuest()->GetShowQuickQuestDLG() == true )
	{
		g_pData->GetUIManager()->GetUIQuest()->UpdateQuickQuestDLG();
	}
#endif SERV_EPIC_QUEST
	return true;
}
/*virtual*/ bool CX2State::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Game )
	{
		return g_pX2Game->Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( kEvent );
	}
	else
	{
		StateLog( L"g_pX2Game is NULL In Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK" );
		ASSERT( NULL != g_pX2Game );
		return false;
	}
}

/*virtual*/ bool CX2State::Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Game )
	{
		return g_pX2Game->Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( kEvent );
	}
	else
	{
		StateLog( L"g_pX2Game is NULL In Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT" );
		ASSERT( NULL != g_pX2Game );
		return false;
	}
}

/*virtual*/ bool CX2State::Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_STOP_DUNGEON_CONTINUE_TIME_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == false )
			return false;

		if ( NULL != g_pX2Game )
		{
			return g_pX2Game->Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK( kEvent );
		}
		else
		{
			StateLog( L"g_pX2Game is NULL In Handler_EGS_STOP_DUNGEON_CONTINUE_TIME_ACK" );
			ASSERT( NULL != g_pX2Game );
			return false;
		}
	}

	return false;
}

/*virtual*/ bool CX2State::Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if ( NULL != g_pX2Game )
	{
		return g_pX2Game->Handler_EGS_END_GAME_NOT( kEvent );
	}
	else
	{
#ifdef FIX_OBSERVER_MODE_CRASH		/// �������� ������ �����µ��� ��Ŷ�� �޾Ƽ�, ���� ó��
		if ( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && false == g_pX2Room->GetMySlot()->m_bObserver )
#endif FIX_OBSERVER_MODE_CRASH
		{
			StateLog( L"g_pX2Game is NULL In Handler_EGS_END_GAME_NOT" );
			ASSERT( NULL != g_pX2Game );
		}
		return false;
	}
}

/*virtual*/ bool CX2State::Handler_EGS_USER_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_DIE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_USER_UNIT_DIE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;	
}

/*virtual*/ bool CX2State::Handler_EGS_USER_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_DIE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if ( NULL != g_pX2Game )
		g_pX2Game->UserUnitDieNot( kEvent );

	return true;
}

/*virtual*/ bool CX2State::Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_DIE_COMPLETE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if ( NULL != g_pX2Game )
		g_pX2Game->Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( kEvent );

	return true;
}

void CX2State::ReturnToPlaceWhereBeforeDungeonStart()
{
	CX2LocationManager* pLocationManager = g_pData->GetLocationManager();
	switch ( pLocationManager->GetX2StateBeforeDungeonStart() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			const float fNowHpOfMine = CX2GageManager::GetInstance()->GetMyGageData()->GetNowHp();
			CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();

			switch ( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_DUNGEON_RESULT:
			case CX2Main::XS_DUNGEON_GAME:
				{
					if ( fNowHpOfMine > 0.0f )
					{				
						Handler_EGS_JOIN_BATTLE_FIELD_REQ( battleFieldManager.GetBattleFieldIdWhereIam(), 
							battleFieldManager.GetBattleFieldPositionIndexWhereIShouldBe(), battleFieldManager.GetBattleFieldPositionValue() );
					}
					else	/// �׾����� ������~!!
						Handler_EGS_STATE_CHANGE_FIELD_REQ( battleFieldManager.GetReturnVillageId( battleFieldManager.GetBattleFieldIdWhereIam() ) );
				} break;

			default:
				{
					Handler_EGS_JOIN_BATTLE_FIELD_REQ( battleFieldManager.GetBattleFieldIdWhereIam(), 
						battleFieldManager.GetBattleFieldPositionIndexWhereIShouldBe(), battleFieldManager.GetBattleFieldPositionValue() );
				} break;
			}			
		} break;
	case CX2Main::XS_SERVER_SELECT: //Ʃ�丮���� ��� XS_SERVER_SELECT���� ����		
		{

			const CX2Unit::UnitData* pUnitData = 	&g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

			if ( SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= pUnitData->m_nMapID && 
				SEnum::VMI_BATTLE_FIELD_END > pUnitData->m_nMapID )
			{
				CX2GageManager* pGageManager = CX2GageManager::GetInstance();
				if ( pGageManager->GetMyGageData()->GetNowHp() > 0.0f )
				{
					Handler_EGS_JOIN_BATTLE_FIELD_REQ( pUnitData->m_nMapID, pUnitData->m_ucLastTouchLineIndex, pUnitData->m_usLastPosValue  );
					break;
				}
			}
			Handler_EGS_STATE_CHANGE_FIELD_REQ();
		} break;
	case CX2Main::XS_VILLAGE_MAP:
		{
			Handler_EGS_STATE_CHANGE_FIELD_REQ();

		} break;

	default:
		{
			Handler_EGS_STATE_CHANGE_FIELD_REQ();
			ASSERT( !L"GetX2StateBeforeDungeonStart is wrong!" );
		} break;
	}	
}

/*virtual*/ bool CX2State::Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ()
{
	if ( !GetWaitGameStart() )	/// ���� ���� ������ ��ٸ��� ���� �ƴ� ��쿡�� (����, ���� ���� ���� ī��Ʈ�� ���� ���� �ƴ� ���)
	{
		g_pData->GetServerProtocol()->SendID( EGS_STATE_CHANGE_SERVER_SELECT_REQ );
		g_pMain->AddServerPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK, 500.0f );
	}

	return true;
}
/*virtual*/ bool CX2State::Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_UNIT_SELECT, NULL, false" );
			//������//2013-01-17//ä�� ����â���� �̵� �ÿ� ���� ü�� ����
			Send_EGS_UPDATE_PLAY_STATUS_NOT();	
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_552 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_SERVER_SELECT, NULL, false );
		}
	}

	//{{ ����� : [2009/10/20] //	���� �� ��� �޽����� ��� �ǵ��� ���ǹ� ó��
#ifdef GUILD_MANAGEMENT
	if( g_pData->GetGuildManager() != NULL && g_pData->GetGuildManager()->GetShownGuildMessage() == true )
	{
		g_pData->GetGuildManager()->SetShownGuildMessage( false );
	}
#endif	//	GUILD_MANAGEMENT
	//}} ����� : [2009/10/20] //	���� �� ��� �޽����� ��� �ǵ��� ���ǹ� ó��

#ifdef BUFF_TEMPLET_SYSTEM
	//�ɸ��� ����â ���� �� ���� ������ �ʱ�ȭ
	BOOST_FOREACH( CX2PremiumBuffIconPtr pPremiumBuffIconPtr, g_pData->GetPremiumBuffManager()->GetPremiumBuffList() )
	{
		g_pData->GetPremiumBuffManager()->ErasePremiumBuff(static_cast<CX2PremiumBuffTemplet::BUFF_ID>( pPremiumBuffIconPtr->GetPremiumBuffID() ));
	}
	g_pData->GetPremiumBuffManager()->GetPremiumBuffList().resize(0);
#endif BUFF_TEMPLET_SYSTEM

	return true;
}

/*virtual*/ bool CX2State::Handler_EGS_JOIN_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ������ �ʵ忡 �ִ� ����� ���� �� ����
	// ���� ������ �ִ� ����� X2StateBattleField ��ü�� ���� �������� �ʾƼ� ó���� ���ϴµ�...
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_BATTLE_FIELD_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	if ( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
	{
		if( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() &&
			kPacket.m_JoinSlot.m_kRoomUserInfo.m_nUnitUID != g_pX2Room->GetMySlot()->m_UnitUID )
			g_pX2Room->Set_KRoomSlotInfo( kPacket.m_JoinSlot );

	}

	return true;
}

/*virtual*/ bool CX2State::Handler_EGS_LEAVE_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_BATTLE_FIELD_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	if ( NULL != g_pX2Room && CX2Room::RT_BATTLE_FIELD == g_pX2Room->GetRoomType() )
	{
		DialogLog( "Before SetRoomState" );
		g_pX2Room->SetRoomState( static_cast<CX2Room::ROOM_STATE>( kPacket.m_cRoomState ) );

		DialogLog( "Before Set_KRoomSlotInfoList, After SetRoomState" );
		g_pX2Room->Set_KRoomSlotInfoList( kPacket.m_vecSlot );

		DialogLog( "After Set_KRoomSlotInfoList" );
		return true;
	}
	else
		return false;
}

void CX2State::Send_EGS_UPDATE_PLAY_STATUS_NOT()
{
	if( NULL != g_pX2Game && CX2Game::GT_PVP == g_pX2Game->GetGameType() )
		return;

	KEGS_UPDATE_PLAY_STATUS_NOT kPacket;

#ifdef	SERV_FIELD_FRAME_LOG
	kPacket.m_iFrame = ( NULL == g_pX2Game ? 0 : static_cast<int>( g_pX2Game->AccessGameStatistics().m_kAverageFps.m_fAverageFps ) );
#endif	SERV_FIELD_FRAME_LOG
	
	if ( CX2GageManager::GetInstance()->GetMyPlayStatusToPacket( kPacket.m_kGamePlayStatus ) )
		g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_PLAY_STATUS_NOT, kPacket );
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2State::CheckAndSendingPlayStatus( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2State::CheckAndSendingPlayStatus()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	CKTDXCheckElapsedTime& TimerForSendingPlayStatus = g_pInstanceData->GetTimerForSendingPlayStatus();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    TimerForSendingPlayStatus.OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	TimerForSendingPlayStatus.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	if ( TimerForSendingPlayStatus.CheckAndResetElapsedTime() )
		Send_EGS_UPDATE_PLAY_STATUS_NOT();
}

bool CX2State::ProcessGameScoreAndPostEffect()
{
#ifdef DIALOG_SHOW_TOGGLE
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_DELETE) == TRUE )
	{
		bool bShow = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetShow();
		g_pKTDXApp->GetDGManager()->GetDialogManager()->SetShow( !bShow );
		bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();

		ProcessShowMiniMap( bHide );
#ifdef ALWAYS_SCREEN_SHOT_TEST
		if(g_pInstanceData->GetScreenShotTest() == false)
		{
			g_pInstanceData->SetScreenShotTest(true);
		}
		else
		{
			g_pInstanceData->SetScreenShotTest(false);
		}
#endif ALWAYS_SCREEN_SHOT_TEST
#ifdef ADD_POSTEFFECT
		if( bHide == true )
		{
			g_pKTDXApp->GetDGManager()->CreatePostEffect();
			g_pKTDXApp->GetDGManager()->GetPostEffect()->SetEnable( true );
		}
		else
		{
			g_pKTDXApp->GetDGManager()->RemovePostEffect();
		}
#endif
		
		return true;
	}
#ifdef ALWAYS_SCREEN_SHOT_TEST
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_ESCAPE) == TRUE )
	{
		if( g_pInstanceData != NULL)
		{
			if( g_pInstanceData->GetScreenShotTest() == true )
			{
				g_pInstanceData->SetScreenShotTest(false);
			}
		}
	}
#endif ALWAYS_SCREEN_SHOT_TEST

	return false;
#endif
}

bool CX2State::Handler_EGS_DECREASE_SPIRIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DECREASE_SPIRIT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL == g_pX2Game ||
		 NULL == g_pX2Game->GetMyUnit() )
		 return false;

	CX2Unit* pMyUnit = g_pX2Game->GetMyUnit()->GetUnit();
    if ( pMyUnit == NULL )
    {
		ASSERT( pMyUnit );
		return false;
    }

	CX2Unit::UnitData* pMyUnitData = &pMyUnit->AccessUnitData();


	pMyUnitData->m_iSpirit			=	kEvent.m_iSpirit;

#ifdef SERV_CHINA_SPIRIT_EVENT
	pMyUnitData->m_arrChinaSpirit[0]	= kEvent.m_arrChinaSpirit[0];
	pMyUnitData->m_arrChinaSpirit[1]	= kEvent.m_arrChinaSpirit[1];
	pMyUnitData->m_arrChinaSpirit[2]	= kEvent.m_arrChinaSpirit[2];
	pMyUnitData->m_arrChinaSpirit[3]	= kEvent.m_arrChinaSpirit[3];
	pMyUnitData->m_arrChinaSpirit[4]	= kEvent.m_arrChinaSpirit[4];
	pMyUnitData->m_arrChinaSpirit[5]	= kEvent.m_arrChinaSpirit[5];

	g_pMain->GetMemoryHolder()->UpdateUseSpiritEvent();
#endif SERV_CHINA_SPIRIT_EVENT

	//{{ kimhc // 2009-10-31 // ��� ������Ʈ
#ifdef	GUILD_MANAGEMENT
	CX2GuildManager* pGuildManager = g_pData->GetGuildManager();

	if ( NULL != pGuildManager && pGuildManager->DidJoinGuild() == true )
	{
		if ( pGuildManager->GetMyGuildInfo()->m_byGuildLevel >= pGuildManager->GetLimitGuildLevel() )
			pMyUnitData->m_iGuildHonorPoint = _CONST_GUILD_MANAGER_::g_iMaxGuildHonorPoint;
		else
			pMyUnitData->m_iGuildHonorPoint =	kEvent.m_iHonorPoint;

		if ( pGuildManager->GetShowUIGuildInfo() == true )
		{
			int byGuildLv = pGuildManager->GetMyGuildInfo()->m_byGuildLevel;
			pGuildManager->GetUIGuild()->UpdateGuildHonorBar( pGuildManager->GetNowMyGuildHonorPoint( byGuildLv ),
				_CONST_GUILD_MANAGER_::g_iMaxGuildHonorPoint );
		}
	}
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-31 // ��� ������Ʈ

	return true;
}

/** @function : Handler_EGS_GAME_START_TIME_COUNT_NOT
	@brief : ���� ���� ���� ī��Ʈ�� ���� ��Ŷ (���� ���� �� 3, 2, 1 ī��Ʈ)
*/
bool CX2State::Handler_EGS_GAME_START_TIME_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GAME_START_TIME_COUNT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bWaitGameStart = true;
	switch ( kEvent.m_cSecondCount )
	{
	case 3:
		{
			SAFE_DELETE_DIALOG( m_pDlgGameStartCount );
			m_pDlgGameStartCount = new CKTDGUIDialog( this, L"DLG_DUNGEON_GAME_START_COUNT.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgGameStartCount );
						
			SetShowPictureGameStartCount( kEvent.m_bIsDungeonRoom , kEvent.m_cSecondCount - 1 );

			g_pData->GetUIManager()->CloseAll();

			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			{
				g_pX2Game->GetMyUnit()->SetCanNotInputTime_LUA( 5.f );
				g_pX2Game->GetMyUnit()->SetForceInvincible( 5.0f );
				g_pX2Game->GetMyUnit()->SetShowInvincible( 5.0f );
			}

			m_bEnableShortCutKey = false;

		} break;
	}

	SetShowPictureGameStartCount( kEvent.m_bIsDungeonRoom, kEvent.m_cSecondCount - 1 );	

	//ī��Ʈ �ÿ� ����/�������� ��ư ��Ȱ��ȭ
	if( NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->SetDungeonButton(CX2PartyUI::BUTTON_START, false, true);
		g_pMain->GetPartyUI()->SetPVPButton(CX2PartyUI::BUTTON_START, false, true);
	}
// 	if ( g_pChatBox != NULL )
// 		g_pChatBox->AddChatLog( GET_REPLACED_STRING( (STR_ID_2632, "i", static_cast<int>( kEvent.m_cSecondCount ) ) ), KEGS_CHAT_REQ::CPT_SYSTEM, _color, L"#CFFFF00" );
	
	return true;
}

#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
/** @function : Handler_EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT
	@breif : �ʵ� ���赵 Ȯ�� ġƮ(���� ���赵�� ä�÷α׿� �߰�)
*/
bool CX2State::Handler_EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( kEvent.m_iDangerousValue >= 0 )
	{
		g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_19208, "i", kEvent.m_iDangerousValue ) ),
			KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}	

	return true;
}
#endif //SERV_BATTLE_FIELD_DANGEROUS_CHEAT

void CX2State::SetShowPictureGameStartCount( const bool bIsDungeonWait,  const int iCount_ )
{
	if ( NULL != m_pDlgGameStartCount )
	{
		CKTDGUIStatic* pStaticGameStartCount = static_cast<CKTDGUIStatic*>( m_pDlgGameStartCount->GetControl( L"Restore_Count" ) );
		if ( NULL != pStaticGameStartCount )
		{
			const int iNumOfPicture = pStaticGameStartCount->GetPictureNum();

			for ( int i = 0; i < iNumOfPicture; i++ )
				pStaticGameStartCount->GetPicture( i )->SetShow( false );
			
			if ( 0 <= iCount_ && iNumOfPicture > iCount_ )
				pStaticGameStartCount->GetPicture( iCount_ )->SetShow( true );
		}		

#ifndef FIX_DUNGEON_START_STRING
		if( true == bIsDungeonWait )
		{
#endif //FIX_DUNGEON_START_STRING
			CKTDGUIStatic* pStaticGameStartCountTitle = static_cast<CKTDGUIStatic*>( m_pDlgGameStartCount->GetControl( L"Restore_CountTitle" ) );
			if( NULL != pStaticGameStartCountTitle && 
				NULL != pStaticGameStartCountTitle->GetPicture(0) &&
				NULL != pStaticGameStartCountTitle->GetPicture(1) )
			{				
				if( true == bIsDungeonWait )
				{
					pStaticGameStartCountTitle->GetPicture(0)->SetShow(true); //���� ������ �����մϴ�.
					pStaticGameStartCountTitle->GetPicture(1)->SetShow(false);//���� ������ �����մϴ�.
				}
				else
				{
					pStaticGameStartCountTitle->GetPicture(0)->SetShow(false); //���� ������ �����մϴ�.
					pStaticGameStartCountTitle->GetPicture(1)->SetShow(true);  //���� ������ �����մϴ�.
				}
			}
#ifndef FIX_DUNGEON_START_STRING
		}
#endif //FIX_DUNGEON_START_STRING
	}
}

bool CX2State::Handler_EGS_UPDATE_PARTY_MEMBER_STATUS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_PARTY_MEMBER_STATUS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2GageManager::GetInstance()->UpdatePartyMemberGageData( kEvent.m_iUnitUID, kEvent.m_kStatus );

	return true;
}


// {{ kimhc // 2012-06-18 // ������ �̵��ϱ⸸ �ص� �Ϸ�Ǵ� ����Ʈ �۾�
#ifdef	SERV_ENTER_FIELD_QUEST_CLEAR
void CX2State::CompleteQuestToVisitVillage( const CX2QuestManager::QuestTemplet* pQuestTemplet_ )
{
#ifdef NO_FINISH_EVENT_VISIT_QUEST
	if( pQuestTemplet_->m_eQuestType == CX2QuestManager::QT_EVENT )
		return;
#endif // NO_FINISH_EVENT_VISIT_QUEST

	map<int, int> mapSelectedRewardItem = GetSelectRewardItemIDList(pQuestTemplet_);	// ������ ������ �����ϴ� ���� ������ ���
	g_pData->GetUIManager()->GetUIQuestNew()->Handler_EGS_QUEST_COMPLETE_REQ( static_cast<CX2UnitManager::NPC_UNIT_ID>( 0 ), pQuestTemplet_->m_iID, mapSelectedRewardItem );
}

KEGS_QUEST_COMPLETE_REQ CX2State::CompleteQuestToVisitVillageList( const CX2QuestManager::QuestTemplet* pQuestTemplet_ )
{
	map<int, int> mapSelectedRewardItem = GetSelectRewardItemIDList(pQuestTemplet_);	// ������ ������ �����ϴ� ���� ������ ���

	KEGS_QUEST_COMPLETE_REQ kPacket;
	kPacket.m_iTalkNPCID		=  0;
	kPacket.m_iQuestID			=  pQuestTemplet_->m_iID;
	kPacket.m_mapSelectItemID	=  mapSelectedRewardItem;

	return kPacket;
}

std::map<int,int> CX2State::GetSelectRewardItemIDList(const CX2QuestManager::QuestTemplet* pQuestTemplet_)
{
	map<int, int> mapSelectedRewardItem;	// ������ ������ �����ϴ� ���� ������ ���

	// ������ �� �ִ� ������ ������
	if ( 0 < pQuestTemplet_->m_SelectReward.m_iSelectionCount )
	{
		const CX2QuestManager::SelectReward& stSelectReward = pQuestTemplet_->m_SelectReward;
		const UINT uiSelectionCount = stSelectReward.m_iSelectionCount;

		// �������� ������ �� �ִ� ���� ���� ũ�ų� ���ƾ� ��
		ASSERT( stSelectReward.m_vecSelectItem.size() >= uiSelectionCount );
		if ( stSelectReward.m_vecSelectItem.size() >= uiSelectionCount )
		{
			const CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
			const CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

			UINT uiIndex = 0;

			/// �������� �� ���Ұų�, ������ �� �ִ� ���� ��ŭ �ڵ� ���� �Ǿ����� ���� ����
			while ( mapSelectedRewardItem.size() < uiSelectionCount &&
				stSelectReward.m_vecSelectItem.size() > uiIndex )
			{
				// ���ú��󽽷�
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet_->m_SelectReward.m_vecSelectItem[uiIndex];
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				if ( NULL != pItemTemplet )
				{
                    CX2Unit::UNIT_TYPE eUnitType = pItemTemplet->GetUnitType();
                    CX2Unit::UNIT_CLASS eUnitClass = pItemTemplet->GetUnitClass();
					/// ���� �� �ִ� ������ �̶��
					if ( ( CX2Unit::UT_NONE == eUnitType || eUnitType == eUnitType ) &&
						( CX2Unit::UC_NONE == eUnitClass || true == GetX2UnitClassCompatibility(eUnitClass, eUnitClass) ) )
					{
						mapSelectedRewardItem.insert( std::make_pair( itemData.m_iItemID, itemData.m_iItemID ) );
					}
				}

				++uiIndex;
			}	// end while
		}	// end if
	}	// end if

	return mapSelectedRewardItem;
}
#endif	SERV_ENTER_FIELD_QUEST_CLEAR
// }} kimhc // 2012-06-18 // ������ �̵��ϱ⸸ �ص� �Ϸ�Ǵ� ����Ʈ �۾�

// {{ ����ȯ // 2012-06-25 // ������ �˴ٿ��� �ȳ�
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
bool CX2State::Handler_EGS_BEFORE_SHUT_DOWN_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BEFORE_SHUT_DOWN_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//�߰� ���� ��ȯ

	if( NULL != g_pChatBox )
	{
		wstring wstrShutDownTime	= L"";
		wstring wstrShudDownMessage = L"";

		//��(Minute)�� ��ȯ ( GetExpirationDateDesc �Լ� ���� )
		CTime cTime;
		KncUtil::ConvertStringToCTime( const_cast< std::wstring & >( kEvent.m_wstrTime ), cTime );

		__time64_t t0 = g_pData->GetServerCurrentTime();

		double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );

		int remainMinute = static_cast<int>( ( ( time_t )fDiffTime % ( 60 * 60 ) ) / ( 60 ) );

		wstrShutDownTime = GET_REPLACED_STRING( ( STR_ID_366, "i", remainMinute ) );		//@1��
		//--

		//ȸ������ �˴ٿ��� ���������� @1 �ĺ��� ��� ���� ������ �̿��Ͻ� �� �����ϴ�. 
		g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_17727, "L", wstrShutDownTime) ), 
			KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
		wstrShudDownMessage = GET_REPLACED_STRING( ( STR_ID_17726, "L", wstrShutDownTime ) );

		if( L"" != wstrShudDownMessage )
		{
			g_pMain->SetShutDownString( wstrShudDownMessage );	//�˴ٿ� ���� ����
			g_pMain->SetShutDownInformation( true );			//�˴ٿ� ���� üũ ����
		}
	}

	return true;
}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
// }} ����ȯ // 2012-06-25 // ������ �˴ٿ��� �ȳ�

/** @function : Handler_EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT
	@brief : �������� ����� ��������� ������ ���� �޾Ƽ� ó���ϴ� �ڵ鷯
*/
bool CX2State::Handler_EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ASSERT( NULL != g_pX2Game );
	if ( NULL != g_pX2Game )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT kEvent;
		DeSerialize( pBuff, &kEvent );

		g_pX2Game->SetBuffInfoPacketToGUUser( kEvent );
	}

	return true;
}

/** @function : Handler_EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT
	@brief : NPC���� ����� ��������� ������ ���� �޾Ƽ� ó���ϴ� �ڵ鷯
*/
bool CX2State::Handler_EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ASSERT( NULL != g_pX2Game );
	if ( NULL != g_pX2Game )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT kEvent;
		DeSerialize( pBuff, &kEvent );
		
		g_pX2Game->SetBuffInfoPacketToNpc( kEvent );
	}

	return true;
}








//////////////////////////////////////////////////////////////////////////

CX2State::MouseClickUI::MouseClickUI( CKTDXStage* pStage, D3DXVECTOR2 vPos, float fTime, int nCount, MOUSE_CLICK_STYLE eStyle ) 
: m_pDLGMouseClick( NULL )
{
	m_pDLGMouseClick = new CKTDGUIDialog( pStage, L"DLG_Mouse_Click.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMouseClick );


	m_pDLGMouseClick->SetShowEnable( true, true );
	m_pDLGMouseClick->SetPos( vPos );
	m_pDLGMouseClick->SetColor( D3DXCOLOR(1,1,1,1) );
	m_pDLGMouseClick->MoveStop();
	m_pDLGMouseClick->Move( vPos, D3DXCOLOR(1,1,1,1), fTime, true, false, false, 1 );

	CKTDGUIStatic* pStatic_Arrow_From_Right_Bottom = (CKTDGUIStatic*) m_pDLGMouseClick->GetControl( L"Static_Arrow_From_Right_Bottom" );
	CKTDGUIStatic* pStatic_Arrow_From_Right_Top = (CKTDGUIStatic*) m_pDLGMouseClick->GetControl( L"Static_Arrow_From_Right_Top" );
	CKTDGUIStatic* pStatic_Arrow_From_Left_Bottom = (CKTDGUIStatic*) m_pDLGMouseClick->GetControl( L"Static_Arrow_From_Left_Bottom" );
	//CKTDGUIStatic* pStatic_Arrow_From_Left_Top = (CKTDGUIStatic*) m_pDLGMouseClick->GetControl( L"Static_Arrow_From_Left_Top" ); // warning!! ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CKTDGUIStatic* pStatic_Arrow_From_Right_Top_Normal = (CKTDGUIStatic*) m_pDLGMouseClick->GetControl( L"Static_Arrow_From_Right_Top_Normal" );
#endif // SERV_NEW_DEFENCE_DUNGEON

	pStatic_Arrow_From_Right_Bottom->SetSize(D3DXVECTOR2(0.5f, 0.5f));
	pStatic_Arrow_From_Right_Top->SetSize(D3DXVECTOR2(0.5f, 0.5f));
	pStatic_Arrow_From_Left_Bottom->SetSize(D3DXVECTOR2(0.5f, 0.5f));
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( NULL != pStatic_Arrow_From_Right_Top_Normal )
		pStatic_Arrow_From_Right_Top_Normal->SetSize(D3DXVECTOR2(0.5f, 0.5f));
#endif // SERV_NEW_DEFENCE_DUNGEON

	switch( eStyle )
	{
	case MCS_FROM_RIGHT_BOTTOM:
		{
			pStatic_Arrow_From_Right_Bottom->SetShow( true );
			vPos -= D3DXVECTOR2( 10, 10 );
			m_pDLGMouseClick->Move( vPos, D3DXCOLOR(1,1,1,1), fTime, false, false, true, nCount );
		} break;

	case MCS_FROM_LEFT_BOTTOM:
		{
			pStatic_Arrow_From_Left_Bottom->SetShow( true );
			vPos -= D3DXVECTOR2( -10, 10 );
			m_pDLGMouseClick->Move( vPos, D3DXCOLOR(1,1,1,1), fTime, false, false, true, nCount );
		} break;
	case MCS_FROM_LEFT_TOP:
		{
			//vPos -= D3DXVECTOR2( -10, -10 );
			//m_pDLGMouseClick->Move( vPos, D3DXCOLOR(1,1,1,1), fTime, false, false, true, nCount );
		} break;
	case MCS_FROM_RIGHT_TOP:
		{
			pStatic_Arrow_From_Right_Top->SetShow( true );
			vPos -= D3DXVECTOR2( 10, -10 );
			m_pDLGMouseClick->Move( vPos, D3DXCOLOR(1,1,1,1), fTime, false, false, true, nCount );
		} break;
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case MCS_FROM_RIGHT_TOP_NORMAL:
		{
			if( NULL != pStatic_Arrow_From_Right_Top_Normal )
			{
				pStatic_Arrow_From_Right_Top_Normal->SetShow( true );
				vPos -= D3DXVECTOR2( 10, -10 );
				m_pDLGMouseClick->Move( vPos, D3DXCOLOR(1,1,1,1), fTime, false, false, true, nCount );
			}
		} break;
#endif // SERV_NEW_DEFENCE_DUNGEON

	}
}

#ifdef UDP_CAN_NOT_SEND_USER_KICK
bool CX2State::Handler_EGS_UDP_KICK_GAMEEDIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UDP_KICK_GAMEEDIT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	g_pInstanceData->SetUDPKickOff(kEvent.m_bUDPSetKick);
	return true;


}
#endif //UDP_CAN_NOT_SEND_USER_KICK

#ifdef SERV_2012_PVP_SEASON2_EVENT
void CX2State::OpenTimeOfGlory()		/// ������ �ð� ���̾�α� Ȱ��
{
	SAFE_DELETE_DIALOG( m_pDLGTimeOfGlory );

	m_pDLGTimeOfGlory = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Time_Of_Glory_Title.lua" );

	if( NULL == m_pDLGTimeOfGlory ||
		NULL == g_pKTDXApp || 
		NULL == g_pKTDXApp->GetDGManager() ||
		NULL == g_pKTDXApp->GetDGManager()->GetDialogManager() )
		return;

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTimeOfGlory );

	CKTDGUIStatic *pStatic = m_pDLGTimeOfGlory->GetStatic_LUA("Time_Of_Glory_Title");

	if( NULL != pStatic )
	{
		CKTDGUIControl::CPictureData *pTimeOfGlory = pStatic->GetPictureIndex( 120 );

		if(pTimeOfGlory != NULL)
			pTimeOfGlory->SetShow( true );
	}
}

void CX2State::MoveTimeOfGlory()		/// ������ �ð� ���̾�α� ����
{
	if( NULL != m_pDLGTimeOfGlory )
		m_pDLGTimeOfGlory->Move(m_pDLGTimeOfGlory->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 0.75f, true);
}
#endif SERV_2012_PVP_SEASON2_EVENT 

//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
bool CX2State::Handler_EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHECK_PING_SCORE_CHANGE_HOST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Room )
	{
		CX2Room::SlotData* pNewHostSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iNewHostUID );
		CX2Room::SlotData* pOldHostSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iOldHostUID );

		ASSERT( NULL != pNewHostSlotData );
		ASSERT( NULL != pOldHostSlotData );
		if ( NULL != pNewHostSlotData && NULL != pOldHostSlotData )
		{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			//���� host�� �Ǿ����� ���
			if( g_pMain->GetGameEdit() )
			{
				WCHAR pString[255];
				
				if( pNewHostSlotData->m_pUnit )
					wsprintf( pString, L"%s���� Host�� ������.", pNewHostSlotData->m_pUnit->GetNickName() );
				else
					wsprintf( pString, L"???���� Host�� ������." );

				g_pMain->GetGameEdit()->AddString( CX2ChatWindow::CT_KILL, pString );
			}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

			pNewHostSlotData->m_bHost = true;
			pOldHostSlotData->m_bHost = false;

			// �ʵ� ������ �� �׽�Ʈ �غ��� ���� �ڵ常
//#ifdef	BATTLE_FIELD_TEST	
			if ( NULL != g_pX2Game )
			{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                g_pX2Game->UpdateHostGameUnit();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				CX2GUUser* pNewHostUser = g_pX2Game->GetUserUnitByUID( kEvent.m_iNewHostUID );
//				if ( NULL != pNewHostUser )
//					g_pX2Game->SetHostGameUnit( static_cast<CX2GameUnit*>( pNewHostUser ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			}
//#endif	BATTLE_FIELD_TEST	
			
		}
	}

	return true;
}
#endif	SERV_CHOOSE_FASTEST_HOST
//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�


bool CX2State::Handler_EGS_GET_ITEM_TEMP_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_ITEM_TEMP_INVENTORY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pData->GetUIManager()->GetUITempInventory() == NULL )
	{
		g_pData->GetUIManager()->CreateUITempInventory();
	}

	g_pData->GetUIManager()->GetUITempInventory()->AddTempItem( kEvent.m_kTempInvenItem );

	if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_TEMP_INVENTORY ) == false )
	{
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_TEMP_INVENTORY, true );
	}

	return true;
}
#ifdef SERV_HERO_PVP_MANAGE_LIST
#ifdef ADD_HERO_MATCH_NOTICE
/// ���� �׷쿡�� ������ ���� �Է�
//bool CX2State::Handler_EGS_ADMIN_REMOVE_HERO_PVP_USER_REQ(  UidType iServerGroupID_, const char* pNickName_ )
//{
//	if ( NULL == pNickName_ )
//		return false;
//
//	wstring wstrNickName;
//	ConvertCharToWCHAR( wstrNickName, pNickName_ );
//
//	KEGS_ADMIN_REMOVE_HERO_PVP_USER_REQ kPacket;
//
//	kPacket.m_iServerGroupID	= iServerGroupID_;		/// ������ ������ ���� �ε���
//	kPacket.m_wstrNickName		= wstrNickName;			/// ������ ���� �г���
//
//	if ( g_pData->GetServerProtocol() != NULL && 
//#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
//        g_pData->GetServerProtocol()->IsUserProxyValid() == true
//#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
//		g_pData->GetServerProtocol()->GetUserProxy() != NULL
//#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
//        )
//		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_REMOVE_HERO_PVP_USER_REQ, kPacket );
//
//	return true;
//}

/// ���� ó�� ���
bool CX2State::Handler_EGS_ADMIN_REMOVE_HERO_PVP_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( 0 == kEvent.m_iOK )		/// ���������� ���� �Ͽ�����, �޽��� ���
	{
		if( NULL != g_pMain && NULL != g_pMain->GetGameEdit() )
			g_pMain->GetGameEdit()->AddString( CX2ChatWindow::CT_NORMAL, GET_STRING( STR_ID_21055 ) );
		return true;
	}
	else
	{
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}
}

/// ���� ���� ��� ���� �Է�
bool CX2State::Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ( const WCHAR* pMessage_, const WCHAR* pNickName_, int iServerGroupID_ )
{
	KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ kPacket;

	kPacket.m_wstrMessage		= pMessage_;			/// ���� �޼���
	kPacket.m_iServerGroupID	= iServerGroupID_;		/// ���� ��� ���� ���� �ε���

	if( -1 < iServerGroupID_ )
		kPacket.m_wstrNickName	= pNickName_;			/// ���� ��� ���� �г���

	if ( g_pData->GetServerProtocol() != NULL && 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() != NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ, kPacket );

	return true;
}
/// ��� �Ľ̿� �Լ�
bool CX2State::Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ_LUA( const char* pMessage_, const char* pNickName_, int iServerGroupID_ )
{
	wstring wstrMessage;
	ConvertCharToWCHAR( wstrMessage, pMessage_ );

	wstring wstrNickName;
	ConvertCharToWCHAR( wstrNickName, pNickName_ );


	return Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ( wstrMessage.c_str(), wstrNickName.c_str(), iServerGroupID_ );
}

//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
bool CX2State::Handler_EGS_ADMIN_OBSERVER_MODE_REQ( bool bMode )
{
	KEGS_ADMIN_OBSERVER_MODE_REQ kPacket;
	kPacket.m_bObserverMode = bMode;
	
	if ( g_pData->GetServerProtocol() != NULL && 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() != NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_OBSERVER_MODE_REQ, kPacket );

	return true;
}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

/// ���� ó�� ���
bool CX2State::Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	return g_pMain->IsValidPacket( kEvent.m_iOK );
}

/// ���� ���� ��� ���� ���( �г����� ��� ������ �ӼӸ� ����, ��� ���� �ʴٸ� �Ϲ� ���� )
bool CX2State::Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ kEvent;
	DeSerialize( pBuff, &kEvent );

	wstring wstrNotice		= kEvent.m_wstrMessage;
	wstring wstrNickName	= kEvent.m_wstrNickName;
	wstring wstrFontColor;

	D3DXCOLOR cFontColor;

	/// ���� ���
	if( NULL != g_pMain && NULL != g_pMain->GetNoticeManager() )
	{
		if( false == wstrNickName.empty() )		/// �Ϲ� ����
		{
			g_pMain->GetNoticeManager()->SetHeroMatchWisperNotice( true );

			cFontColor		= D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
			wstrFontColor	= L"#CFF00FF";
		}
		else									/// �ӼӸ� ����
		{
			cFontColor		= D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
			wstrFontColor	= L"#CFFFF00";
		}

		if( false == g_pMain->GetNoticeManager()->AddNotice( KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_ALL_SERVERS, wstrNotice.c_str() ) )
		{
			return false;
		}
	}

	/// ä��â�� ���� ���
	if ( g_pChatBox != NULL )
		g_pChatBox->AddChatLog( wstrNotice.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, cFontColor, wstrFontColor );

	return true;
}
#endif ADD_HERO_MATCH_NOTICE
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
void CX2State::SetFadeInOutNotice( const WCHAR* wszMsg_, float fVisibleTime_ /*= 5.f*/, float fOffsetPosX_ /*= 0.f*/ , float fOffsetPosY_ /*= 0.f*/ )	/// ȭ�鿡 ���̵� �� ~ ���̵� �ƿ� �Ǵ� ���� ���� �Լ�
{
	FadeInOutNotice* pFadeInOutNotice = new FadeInOutNotice( wszMsg_, fVisibleTime_ );

	if( NULL != pFadeInOutNotice )
	{
		if( false == IsSamef(0.f, fOffsetPosX_) )
			pFadeInOutNotice->SetOffsetPosX(fOffsetPosX_);

		if( false == IsSamef(0.f, fOffsetPosY_) )
			pFadeInOutNotice->SetOffsetPosY(fOffsetPosY_);

		m_vecFadeInOutNotice.push_back( pFadeInOutNotice );
	}
}

void CX2State::ProcessFadeInOutNotice()		/// ȭ�鿡 ���̵� �� ~ ���̵� �ƿ� �Ǵ� ���� ���� �Լ�
{
	if( m_vecFadeInOutNotice.empty() )		/// ��� ������ ���ٸ� Ż��
		return;

	FadeInOutNotice* pFadeInOutNotice = m_vecFadeInOutNotice.front();

	if( NULL != pFadeInOutNotice )
	{
		/// �ð� ó��
		float iNowFadeInOutNoticeTime =  pFadeInOutNotice->GetFadeInOutNoticeTime();	/// ���� ���� �ؽ�Ʈ ���� �ð�

		iNowFadeInOutNoticeTime -= m_fElapsedTime;	/// ���� �ð� ����

		if( 0.f > iNowFadeInOutNoticeTime )			/// �����ð��� ����Ǿ��� ��
		{
			SAFE_DELETE( pFadeInOutNotice );
			m_vecFadeInOutNotice.erase( m_vecFadeInOutNotice.begin() );	/// ���� �տ� �ִ� ��� ������ �����.
			return;
		}

		pFadeInOutNotice->SetFadeInOutNoticeTime( iNowFadeInOutNoticeTime );

		/// ���̵� ȿ��
		const float fFadeTime		= 1.f;			/// ���̵� ȿ�� �ð�
		const float fFadeInTime		= pFadeInOutNotice->GetFadeInOutNoticeMaxTime() - fFadeTime;		/// ���̵� �� �ð�
		float		fTextAlpha		= 1.f;			/// ǥ�õ� ���� ���� ��

		if( iNowFadeInOutNoticeTime > fFadeInTime )							/// ���İ��� ���� Ŀ����.
		{
			fTextAlpha = 1.f - ( iNowFadeInOutNoticeTime - fFadeInTime );	/// 0.f ~ 1.f
		}
		else if( iNowFadeInOutNoticeTime < fFadeTime )						/// ���İ��� ���� �۾�����.
		{
			fTextAlpha = iNowFadeInOutNoticeTime;							/// 1.f ~ 0.f
		}

		/// �ؽ�Ʈ ���
		if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDGManager() && NULL != g_pKTDXApp->GetDGManager()->GetDialogManager() )
		{
			const D3DXVECTOR2& vOffsetPos = pFadeInOutNotice->GetOffsetPos();
			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL )->
			OutTextXY( 512 + static_cast<int>(vOffsetPos.x) , 250 + static_cast<int>(vOffsetPos.y), pFadeInOutNotice->GetFadeInOutNoticeText(), D3DXCOLOR(1.f,1.f,0.f,fTextAlpha), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0.f,0.f,0.f,fTextAlpha), NULL, DT_CENTER );
		}
	}
}
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
bool CX2State::Handler_EGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	kEvent.m_wstrNewCharTBMsg = GET_STRING( STR_ID_14585 );

	g_pChatBox->AddChatLog( kEvent.m_wstrNewCharTBMsg.c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR( 1,1,0,1 ), L"#CFFFF00" );

	CX2Unit* pUnit = NULL;
	pUnit = g_pData->GetMyUser()->GetSelectUnit();
	pUnit->SetTradeBlockUnit(false);

	return true;
}
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifdef SERV_INVISIBLE_GM
bool CX2State::Handler_EGS_TOGGLE_INVISIBLE_REQ()
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return false; 

	//if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
	//	return false;

	g_pData->GetServerProtocol()->SendID( EGS_TOGGLE_INVISIBLE_REQ );
	return true;
}
#endif SERV_INVISIBLE_GM

#ifdef BANDICAM_HIDE_RECORDING_PARTICLE
void CX2State::ShowBandicamRecParticle( bool bShow )
{
	if( g_pData->GetUIMajorParticle() == NULL )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hParticleRec );
	if( pSeq == NULL )
		return;

	pSeq->SetShowObject( bShow );
	if( bShow == true )
	{
		int     iStackTop = CKTDGStateManager::GetStackTop();
		CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
		pSeq->OnFrameRender_Draw();
		CKTDGStateManager::PopStates( iStackTop );			
	}
}
#endif BANDICAM_HIDE_RECORDING_PARTICLE

//{{ ���� - ���� ���� ����Ʈ, ĳ�� �۾� 
#ifdef SERV_SHARING_BANK_QUEST_CASH
bool CX2State::Handler_EGS_SHARING_BACK_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SHARING_BACK_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if(kEvent.m_iOK != NetError::NET_OK)
	{
		g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999),  NetError::GetErrStrF( NetError::ERR_ODBC_01 ),g_pMain->GetNowState() );
		return false;	
	}

	// �������� �� �� �۾�

	// 1. ���� ���� �����ٰ� Ŭ�� ����
	g_pData->GetMyUser()->SetSharingBankOpen(true);

	// 2. ����Ʈ�� ���� ���̾��ٸ� ��� ���� �ش�.
	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest(); 
	}

	// 3. ���� ���� �Ǿ��ٰ� �޽��� �������
	//	  �޽��� ��� Ʃ�丮�� UI �ϳ� �����.
	// ĳ�ü��� �ݾ��ش�
	if(kEvent.m_iOpenType == 2)
	{
		CX2StateMenu* pStateMenu = (CX2StateMenu*) this;
		if(pStateMenu != NULL)
			pStateMenu->ToggleCashShop();
	}

	g_pData->GetClassChangePopup().SetShow( true, CX2ClassChangePopup::HPT_SHARE_BANK );

	return true;
}
#endif // SERV_SHARING_BANK_QUEST_CASH

#ifdef SERV_NEW_EVENT_TYPES
bool CX2State::Handler_EGS_UPDATE_MAX_LEVEL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_MAX_LEVEL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pInstanceData != NULL )
		g_pInstanceData->SetMaxLevel( kEvent.m_iMaxLevel );

	return true;
}
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
bool CX2State::Handler_EGS_UPDATE_CODE_EVENT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_CODE_EVENT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pInstanceData != NULL )
	{
		g_pInstanceData->SetCodeEvent( kEvent.m_mapEnableCodeEnum );

#ifdef SERV_NEW_YEAR_EVENT_2014
		IF_EVENT_ENABLED( CEI_NEW_YEAR_EVENT_2014 )
		{
			g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
		}
		ELSE_IF_EVENT_ENABLED( CEI_OLD_YEAR_EVENT_2013 )
		{
			g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
		}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
		IF_EVENT_ENABLED( CEI_CHECK_POWER )
		{
			g_pMain->GetMemoryHolder()->UpdateCheckPowerEvent();
		}
#endif SERV_EVENT_CHECK_POWER
	}

	return true;
}
#endif SERV_CODE_EVENT

#ifdef SERV_ADVERTISEMENT_EVENT
void CX2State::OpenAdvertisementDialog( const wstring& wstrURL )
{
	if( wstrURL.find( L"http://" ) == wstring::npos )
	{
		if( m_pDlgAdvertisement == NULL )
		{
			m_pDlgAdvertisement = new CKTDGUIDialog( this, L"DLG_Advertisement.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgAdvertisement );
		}

		if( m_pDlgAdvertisement != NULL )
		{
			m_pDlgAdvertisement->SetShowEnable( true, true );
			CKTDGUIStatic* pStatic = static_cast< CKTDGUIStatic* >( m_pDlgAdvertisement->GetControl( L"g_pStaticWindow" ) );
			if( pStatic != NULL )
			{
				pStatic->GetPicture( 0 )->SetTex( wstrURL.c_str(), L"Advertisement" );
			}
		}
	}
	else
	{
		if( false == g_pMain->GetBrowserWrapper()->IsClosed() )
		{
			g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			g_pMain->GetBrowserWrapper()->CloseBrowser();
		}

		RECT rcWebPage;
		GetWindowRect( g_pKTDXApp->GetHWND(), &rcWebPage );

		POINT ptCenter;
		ptCenter.x = rcWebPage.left + ( rcWebPage.right - rcWebPage.left ) / 2;
		ptCenter.y = rcWebPage.top + ( rcWebPage.bottom - rcWebPage.top ) / 2;

		rcWebPage.left = ptCenter.x - 415;
		rcWebPage.right = ptCenter.x + 415;
		rcWebPage.top = ptCenter.y - 235;
		rcWebPage.bottom = ptCenter.y + 235;
		g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE );
		std::string strURL;
		ConvertWCHARToChar( strURL, wstrURL );
		g_pMain->GetBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );
		g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), NULL );
	}
}

bool CX2State::Handler_EGS_ADVERTISEMENT_EVENT_INFO_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADVERTISEMENT_EVENT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( kEvent.m_vecTimeEvent.size() > 0 )
	{
		OpenAdvertisementDialog( kEvent.m_vecTimeEvent[0].m_wstrAdvertisementURL );
	}

	return true;
}
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS		

bool CX2State::Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	

	if ( NULL != g_pX2Game )
	{	
		CX2GUNPC * pNPC = g_pX2Game->GetNPCUnitByUID( static_cast<int> ( kEvent.m_iNPCUID ) ); // �ش� NPC ( ���� ���� ) �� ���� �ʿ� �����Ѵٸ�,
		if ( NULL != pNPC )	
		{
			for ( map<UidType,float>::iterator _itDamagedMap = kEvent.m_mapDamagedMap.begin(); 
				_itDamagedMap != kEvent.m_mapDamagedMap.end(); ++_itDamagedMap )
			{   				
				// ����� �� ����
				pNPC->SetToDamagedMap( _itDamagedMap->first, _itDamagedMap->second );
			}
		}
	}
	return true;
}


bool CX2State::Handler_EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ kEvent;
	DeSerialize( pBuff, &kEvent );


	if ( NULL != g_pX2Game )
	{

		CX2GUNPC * pNPC = g_pX2Game->GetNPCUnitByUID( kEvent.m_iNPCUID );
		if ( NULL != pNPC )
		{
			KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK kEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK;
			kEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK.m_iNPCUID = kEvent.m_iNPCUID;
			kEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK.m_iUnitUID = kEvent.m_iUnitUID;
			kEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK.m_mapDamagedMap = pNPC->GetDamagedMap();
			
			g_pData->GetServerProtocol()->SendPacket( EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK, kEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK );
		}
	}
	return true;
}


bool CX2State::Handler_EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;	

}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
bool CX2State::Handler_EGS_TCP_PING( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TCP_PING kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Game )
		g_pX2Game->Handler_EGS_TCP_PING( kEvent );

	return true;
}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
bool CX2State::Handler_EGS_GET_ACTIVE_LAGCHECK_REQ()
{
	KEGS_GET_ACTIVE_LAGCHECK_REQ kPacket;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_ACTIVE_LAGCHECK_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_ACTIVE_LAGCHECK_ACK );

	return true;
}

bool CX2State::Handler_EGS_GET_ACTIVE_LAGCHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_ACTIVE_LAGCHECK_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_ACTIVE_LAGCHECK_ACK ) == true )
	{
		KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->SetActiveLagCheck( kEvent.m_bActiveLagCheck );
		return true;
	}

	return false;
}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef _IN_HOUSE_
#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
bool CX2State::Handler_EGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if ( kEvent.m_map_RandomItemResult.empty() == false )
		{
			if (g_pData != NULL && g_pData->GetItemManager() != NULL)
			{

				std::wofstream RandomItemLogstream;
				RandomItemLogstream.open("RandomItemResult.txt");

				std::map< int, KRandomItemResult >::const_iterator cmit = kEvent.m_map_RandomItemResult.begin();
#ifdef WOFSTREAM_PASS_NAME
				RandomItemLogstream<<"ItemID"<<"	"<<"ItemQuantity"<<"	"<<"bIsSeal"<<"	"<<"MaxSealCount"<<"	"<<"Vested"<<"	"<<"PriceType"<<"	"<<"ItemGrade"<<"	""bIsNotifyMsgAttration"<<"	"<<"ItemTotalCount"<<"	""ItemRate"<<"	"<<std::endl;
#else //WOFSTREAM_PASS_NAME
				RandomItemLogstream<<"ItemID"<<"	"<<"ItemName"<<"	"<<"ItemQuantity"<<"	"<<"bIsSeal"<<"	"<<"MaxSealCount"<<"	"<<"Vested"<<"	"<<"PriceType"<<"	"<<"ItemGrade"<<"	""bIsNotifyMsgAttration"<<"	"<<"ItemTotalCount"<<"	""ItemRate"<<"	"<<std::endl;
#endif WOFSTREAM_PASS_NAME

				for (;cmit != kEvent.m_map_RandomItemResult.end(); ++cmit )
				{
					
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( cmit->first );

					if (pItemTemplet != NULL)
					{
#ifdef WOFSTREAM_PASS_NAME
						RandomItemLogstream <<cmit->first<<"	"<<cmit->second.m_iItemCount<<"	"<<cmit->second.m_bIsSealItem<<"	"
							<<pItemTemplet->GetMaxSealCount()<<"	"<<pItemTemplet->GetVested()<<"	"<<pItemTemplet->GetPeriodType()<<"	"<<pItemTemplet->GetItemGrade()<<"	"<<cmit->second.m_bIsNotifyMsgAttration<<"	"<<cmit->second.m_dItemTotalCount<<"	"<<cmit->second.m_dRate<<"	"<<std::endl;
#else //WOFSTREAM_PASS_NAME
						RandomItemLogstream <<cmit->first<<"	"<<pItemTemplet->GetFullName_()<<"	"<<cmit->second.m_iItemCount<<"	"<<cmit->second.m_bIsSealItem<<"	"
							<<pItemTemplet->GetMaxSealCount()<<"	"<<pItemTemplet->GetVested()<<"	"<<pItemTemplet->GetPeriodType()<<"	"<<pItemTemplet->GetItemGrade()<<"	"<<cmit->second.m_bIsNotifyMsgAttration<<"	"<<cmit->second.m_dItemTotalCount<<"	"<<cmit->second.m_dRate<<"	"<<std::endl;
#endif WOFSTREAM_PASS_NAME
					}
					else
					{
						RandomItemLogstream <<cmit->first<<"	"<<"ItemTemplet is NULL"<<std::endl;
					}


				}
				RandomItemLogstream.close();
			}
		}
	}

	return true;
}
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
#endif//_IN_HOUSE_

#ifdef SERV_ID_NETMARBLE_PCBANG
std::wstring CX2State::GetMACAddress() const
{
	std::wstring wstrMACAddress;

	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBuflen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBuflen);
	if(dwStatus == ERROR_SUCCESS)
	{
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		PIP_ADAPTER_INFO pPrimaryAdapterInfo = pAdapterInfo;



		for (unsigned int i = 0; i < pPrimaryAdapterInfo->AddressLength; ++i)
		{

			
			wstrMACAddress += boost::str( boost::wformat(L"%02X") % pPrimaryAdapterInfo->Address[i]);
		}	
	}

	return wstrMACAddress;
}
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-11
	
/** @function	: Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT
	@brief		: �� �ȿ� �ִ� ������ ���� �ɷ�ġ ���� ��Ŷ ó��
	@param		: �̺�Ʈ �޽���
	@return		: ó�� ����
*/
bool CX2State::Handler_EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pX2Room != NULL )
		return g_pX2Room->Handler_EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT( kEvent );


	return false;
}

#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
/** @function	: Handler_EGS_CALL_MY_LOVER_REQ
	@brief		: ��ȯ ��û ��Ŷ ����
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_REQ( int iMapID, D3DXVECTOR3 vPos, unsigned char ucLastTouchLineIndex )
{
#ifdef FIELD_BOSS_RAID // Ŀ�� ��ȯ�� ���� ����ó��
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		g_pChatBox->AddChatLog(  GET_STRING( STR_ID_28990 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		return false;
	}
#endif // FIELD_BOSS_RAID

	KEGS_CALL_MY_LOVER_REQ kPacket;

	kPacket.m_iMapID				= iMapID;
	kPacket.m_vPos.x				= vPos.x;
	kPacket.m_vPos.y				= vPos.y;
	kPacket.m_vPos.z				= vPos.z;
	kPacket.m_LastTouchLineIndex	= ucLastTouchLineIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_CALL_MY_LOVER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CALL_MY_LOVER_ACK );

	return true;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_ACK
	@brief		: ��ȯ ��û�� ���� ���� ���� �� ��ȯ ���� ó��
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
	*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_CALL_MY_LOVER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			/// ��ȯ ���� ����� �˾� ����
			if ( NULL != g_pMain &&
				 NULL != g_pData &&
				 NULL != g_pData->GetMyUser() &&
				 NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

				userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

				g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_SUMMON_MARRIAGED_USER,
											   CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 5.f, GET_STRING( STR_ID_24519 ), 
											   static_cast<CKTDXStage*>( g_pMain->GetNowState() ), -1, -1 );
			}
			
			return true;
		}
	}

	return false;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_AGREE_RESULT_NOT
	@brief		: ��û ����� ���� ó��
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_AGREE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_AGREE_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	/// �¶��ϵ� �ź��ϵ� ������� �˾� ����
	if ( NULL != g_pMain &&
			NULL != g_pData &&
			NULL != g_pData->GetMyUser() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_SUMMON_MARRIAGED_USER, userData );

		/// ��ȯ ����
		if ( false == kEvent &&
				NULL != g_pData &&
				NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			wstring wstrUserName = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;	/// ����� ���̵�

			/// ���� �ȳ� �˾�
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 270.f, 145.f ), GET_REPLACED_STRING( ( STR_ID_24487, "L", wstrUserName ) ), g_pMain->GetNowState() );
		}

		return true;
	}

	return false;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_VILLAGE_NOT
	@brief		: ��ȯ ����ڿ��� ������ ��ȯ���� �� ���� ó��
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_VILLAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_VILLAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	/// �������� ����ϴ� ���� ����
	m_iCalledUserUID		= kEvent.m_iCalledUserUID;
	m_iCalledGSUID			= kEvent.m_iCalledGSUID;
	m_cCallType				= kEvent.m_cCallType;
	m_iMapID				= kEvent.m_iMapID;
	m_vPos.x				= kEvent.m_vPos.x;
	m_vPos.y				= kEvent.m_vPos.y;
	m_vPos.z				= kEvent.m_vPos.z;
	m_LastTouchLineIndex	= kEvent.m_LastTouchLineIndex;

	if ( NULL != g_pMain &&
		 NULL != g_pData &&
		 NULL != g_pData->GetLocationManager() &&
		 NULL != g_pData->GetRelationshipManager() )
	{
		/// ����, ���� ���� ī��Ʈ Ȥ�� ���� ������ ���� ���� ��, ĳ�ü� �̿� �� �϶� ������ �ź� ó��
		if ( true == GetWaitGameStart()  
			 || ( CX2Main::XS_VILLAGE_MAP != g_pMain->GetNowStateID() && CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
			 || ( NULL != g_pData->GetCashShop() && true == g_pData->GetCashShop()->GetOpen() )
			 || true == g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PERSONAL_SHOP )
			)
		{
			Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT( false );
			return false;
		}
		else
		{
			/// �� ���̵� ����, ���� ���� ��ȯ
			CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iMapID ) );

			/// �Է� ���� �ð� ǥ�� �˾� ����
			if ( NULL != pVillageTemplet &&
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				const wstring wstrSummonerName	= g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;	/// ����� ���̵�
				const wstring wstrVillageName	= pVillageTemplet->m_Name;				/// ���� �̸� ��ȯ
				const wstring wstrMessage		= GET_REPLACED_STRING( ( STR_ID_24486, "LL", wstrSummonerName, wstrVillageName ) );				/// �˾�â�� ����� ���� ����

				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

				userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

				g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_SELECT_SUMMON_TO_MARRIAGED_USER,
					CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 5.f, wstrMessage, 
					static_cast<CKTDXStage*>( g_pMain->GetNowState() ), SUCM_AGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER, 
					SUCM_DISAGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER, SUCM_DISAGREE_SUMMON_VILLAGE_BY_MARRIEGED_USER );
			}

			return true;
		}
	}

	return false;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT
	@brief		: ��ȯ ������� ��ȯ ��û �¶� ������ ����
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT( bool bAgree )
{
	KEGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT kPacket;

	/// �������� ����ϴ� ���� ����
	kPacket.m_bAgree				= bAgree;
	kPacket.m_iCalledUserUID		= m_iCalledUserUID;
	kPacket.m_iCalledGSUID			= m_iCalledGSUID;
	kPacket.m_cCallType				= m_cCallType;
	kPacket.m_iMapID				= m_iMapID;
	kPacket.m_vPos.x				= m_vPos.x;
	kPacket.m_vPos.y				= m_vPos.y;
	kPacket.m_vPos.z				= m_vPos.z;
	kPacket.m_LastTouchLineIndex	= m_LastTouchLineIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT, kPacket );

	/// �˾�â ����
	if ( NULL != g_pMain &&
		NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_SELECT_SUMMON_TO_MARRIAGED_USER, userData );
	}

	return true;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_ROOM_NOT
	@brief		: ��ȯ ����ڿ��� �ʵ�� ��ȯ���� �� ���� ó��
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	

	/// �������� ����ϴ� ���� ����
	m_uiRoomUID				= kEvent.m_RoomUID;
	m_iCalledUserUID		= kEvent.m_iCalledUserUID;
	m_iCalledGSUID			= kEvent.m_iCalledGSUID;
	m_cCallType				= kEvent.m_cCallType;
	// �뿡���� mapID�� BattleFieldID �Է�
	m_iMapID				= kEvent.m_iBattleFieldID;
	m_vPos.x				= kEvent.m_vPos.x;
	m_vPos.y				= kEvent.m_vPos.y;
	m_vPos.z				= kEvent.m_vPos.z;
	m_LastTouchLineIndex	= kEvent.m_LastTouchLineIndex;

	if( NULL != g_pData &&
		NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		/// ����, ���� ���� ī��Ʈ Ȥ�� ���� �ʵ忡 ���� ���� ��, ĳ�ü� �̿� �� �϶� ������ �ź� ó��
		if ( true == GetWaitGameStart() || CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() 
			|| ( NULL != g_pData->GetCashShop() && true == g_pData->GetCashShop()->GetOpen() )
			|| true == g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PERSONAL_SHOP ) )
		{
			Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT( false );
			return false;
		}
		else
		{
			/// �� ���̵� ����, �ʵ� �̸� ��ȯ
			const wstring wstrBattleFieldName	= g_pData->GetBattleFieldManager().GetBattleFieldNameByBattleFieldId( static_cast<int>( kEvent.m_iBattleFieldID ) );
			const wstring& wstrSummonerName		= g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;	/// ������ �г��� ��ȯ
			const wstring wstrMessage			=  GET_REPLACED_STRING( ( STR_ID_24486, "LL", wstrSummonerName, wstrBattleFieldName ) );		/// �˾�â�� ����� ���� ����

			/// �Է� ���� �ð� ǥ�� �˾� ����
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

			userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

			g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_SELECT_SUMMON_TO_MARRIAGED_USER,
				CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 5.f,wstrMessage, 
				static_cast<CKTDXStage*>( g_pMain->GetNowState() ), SUCM_AGREE_SUMMON_ROOM_BY_MARRIEGED_USER, 
				SUCM_DISAGREE_SUMMON_ROOM_BY_MARRIEGED_USER, SUCM_DISAGREE_SUMMON_ROOM_BY_MARRIEGED_USER );

			return true;
		}
	}

	return false;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT
	@brief		: ��ȯ ������� ��ȯ ��û �¶� ������ ����
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_AGREE_ROOM_NOT( bool bAgree )
{
	KEGS_CALL_MY_LOVER_AGREE_ROOM_NOT kPacket;

	kPacket.m_RoomUID				= m_uiRoomUID;
	kPacket.m_bAgree				= bAgree;
	kPacket.m_iCalledUserUID		= m_iCalledUserUID;
	kPacket.m_iCalledGSUID			= m_iCalledGSUID;
	kPacket.m_cCallType				= m_cCallType;
	// �뿡���� mapID�� BattleFieldID �Է�
	kPacket.m_iBattleFieldID		= m_iMapID;
	kPacket.m_vPos.x				= m_vPos.x;
	kPacket.m_vPos.y				= m_vPos.y;
	kPacket.m_vPos.z				= m_vPos.z;
	kPacket.m_LastTouchLineIndex	= m_LastTouchLineIndex;

	if( true == bAgree )
	{	
#ifndef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		// �ʵ�->�ʵ� �̵� �� ������ ��� ����
		if( NULL != g_pX2Game )
			g_pX2Game->SendFrameAverage();

		CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
		battleFieldManager.SetBattleFieldIdWhereIam( m_iMapID );
		battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( m_LastTouchLineIndex );
		battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( false );
#endif      SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_CALL_MY_LOVER_AGREE_ROOM_NOT, kPacket );

	/// �˾�â ����
	if ( NULL != g_pMain &&
		NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_SELECT_SUMMON_TO_MARRIAGED_USER, userData );
	}

	return true;
}

/** @function	: Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_BATTLE_FIELD_NOT
	@brief		: ��ȯ ����ڸ� ��ȯ ��û�ڰ� �ִ� �ʵ�� �̵� ��Ű�� ��Ŷ�� ó��
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2State::Handler_EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )	
	{
		ASSERT( 0 != kEvent.m_RoomInfo.m_iBattleFieldID );

		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_SERVER_SELECT:
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_PVP_RESULT:
		case CX2Main::XS_DUNGEON_RESULT:
			{
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	            CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	            battleFieldManager.SetBattleFieldIdWhereIam( kEvent.m_kBattleFieldJoinInfo.m_iBattleFieldID );
                battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( kEvent.m_kBattleFieldJoinInfo.m_iStartPosIndex );
                battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( kEvent.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition );
                battleFieldManager.SetBattleFieldPositionValue( kEvent.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue );

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

#ifdef HEAP_BROKEN_BY_ROOM
				CX2Room::InitializeRoomPacketData();
				CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
					vector<KRoomSlotInfo>(), kEvent.m_wstrCNIP );
#else	HEAP_BROKEN_BY_ROOM
				g_pData->DeleteDungeonRoom();
				g_pData->DeletePVPRoom();

				CX2BattleFieldRoom* pBattleFieldRoom = g_pData->ResetBattleFieldRoom();
				pBattleFieldRoom->Set_KRoomInfo( kEvent.m_RoomInfo );	//ok
				pBattleFieldRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );						// fix!! bUnitInfo == false??? 
				pBattleFieldRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort ); // relay connect 
				pBattleFieldRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
#endif // HEAP_BROKEN_BY_ROOM

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                    g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_BATTLE_FIELD ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_BATTLE_FIELD, NULL, false );

				if( NULL != g_pTFieldGame )
				{
					g_pTFieldGame->SetNearPortalToBattleField( false );
				}
			} break;

		case CX2Main::XS_BATTLE_FIELD:
			{
				CX2StateBattleField* pStateBattleField = static_cast<CX2StateBattleField*>( g_pMain->GetNowState() );
                ASSERT( pStateBattleField != NULL );

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

                if ( pStateBattleField != NULL )
                    pStateBattleField->FlushSendFrameAverage();

	            CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	            battleFieldManager.SetBattleFieldIdWhereIam( kEvent.m_kBattleFieldJoinInfo.m_iBattleFieldID );
                battleFieldManager.SetBattleFieldPositionIndexWhereIShouldBe( kEvent.m_kBattleFieldJoinInfo.m_iStartPosIndex );
                battleFieldManager.SetNowBattleFieldPositionInfoStartPosition( kEvent.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition );
                battleFieldManager.SetBattleFieldPositionValue( kEvent.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue );

#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

				CX2BattleFieldRoom* pBattleFieldRoom = g_pData->GetBattleFieldRoom();
                ASSERT( pBattleFieldRoom != NULL );

				pBattleFieldRoom->Set_KRoomInfo( kEvent.m_RoomInfo );	//ok
				pBattleFieldRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );						// fix!! bUnitInfo == false??? 
				pBattleFieldRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort ); // relay connect 
				pBattleFieldRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                    g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_BATTLE_FIELD ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

				pStateBattleField->MoveFromBattleFieldToOtherBattleField();

			} break;

		default:
			ASSERT( !L"EGS_JOIN_BATTLE_FIELD_ACK" );
			StateLog( L"EGS_JOIN_BATTLE_FIELD_ACK" );
			break;
		}
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD		
		//�ʵ� �����, �ش� �ʵ� ���� ĳ���� �������� ������ ���� ó��
		if( NULL != g_pData && NULL != g_pData->GetCashShop() )
			g_pData->GetCashShop()->SetEtnerCashShopUserList(kEvent.m_vecEnterCashShopUser);
#endif //SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD

		g_pData->GetUIManager()->CloseAllNPCDlg();

		return true;
	}
	else
	{
		switch ( kEvent.m_iOK )
		{
		case NetError::ERR_BATTLEFIELD_13:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING((STR_ID_18892, "i",kEvent.m_iRequireLevel)) , g_pMain->GetNowState() );
			} break;
		case NetError::ERR_BATTLEFIELD_14:
			{
				if( NULL != g_pData && NULL != g_pData->GetDungeonManager() )
				{
					wstring DungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( static_cast<SEnum::DUNGEON_ID>(kEvent.m_iRequireDungeonID));
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 	GET_REPLACED_STRING((STR_ID_18893, "L", DungeonName )),	g_pMain->GetNowState() );
				}
			} break;
		}
	}

	return false;
}


bool CX2State::Handler_EGS_ADMIN_CHANGE_COUPLE_DATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK kEvent;

	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			return true;
		}
	}
	return false;

}


bool CX2State::Handler_EGS_JOIN_WEDDING_HALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_WEDDING_HALL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	ASSERT( 0 != kEvent.m_JoinSlot.m_kRoomUserInfo.m_cWeddingStatus );
	if( NULL != g_pWeddingGame )
	{
		g_pWeddingGame->Handler_EGS_JOIN_WEDDING_HALL_NOT( kEvent );
	}

	return true;
}

#endif // ADDED_RELATIONSHIP_SYSTEM


#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
void CX2State::Send_EGS_FIELD_WORKINGS_BLOCK_LOG_ACK()
{
	if ( NULL != g_pInstanceData )
		g_pData->GetServerProtocol()->SendPacket( EGS_FIELD_WORKINGS_BLOCK_LOG_ACK, g_pInstanceData->GetFieldWorkingsBlockLog() );
}
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

/** @function	: Handler_EGS_CHANGE_MY_UNIT_INFO_NOT
	@brief		: �������� ��� ������ broadcast ���ִ� ��Ŷ �ڵ鷯
*/
bool CX2State::Handler_EGS_CHANGE_MY_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MY_UNIT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_cUnitClass));
	
#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
	CX2GageManager::GetInstance()->ResetGageUIEtc(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_cUnitClass));
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG


	if ( NULL != g_pData &&
		 NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

		/// �� ������ �⺻ ��ų ����
		{
#ifdef FIX_DEFAULT_SKILL_WHEN_CLASS_CHANGE
			pUnitData->m_UserSkillTree.SetUnitClass( static_cast< int >( kEvent.m_cUnitClass ) );
#endif //FIX_DEFAULT_SKILL_WHEN_CLASS_CHANGE

#ifdef SKILL_PAGE_SYSTEM //JHKang
			if ( 0 != kEvent.m_iNewDefaultSkill_1 )
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iNewDefaultSkill_1 ),
					1, 0, pUnitData->m_UserSkillTree.GetUsingPage() );

			if ( 0 != kEvent.m_iNewDefaultSkill_2 )
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iNewDefaultSkill_2 ),
				1, 0, pUnitData->m_UserSkillTree.GetUsingPage() );
#else //SKILL_PAGE_SYSTEM
			if ( 0 != kEvent.m_iNewDefaultSkill_1 )
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iNewDefaultSkill_1 ), 1, 0 );

			if ( 0 != kEvent.m_iNewDefaultSkill_2 )
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iNewDefaultSkill_2 ), 1, 0 );
#endif //SKILL_PAGE_SYSTEM
		}
	}

	return true;
}

#endif // UPGRADE_SKILL_SYSTEM_2013


#ifdef SERV_ACCOUNT_BLOCK_USER_POPUP_TEMP
void CX2State::Handler_EGS_ACCOUNT_BLOCK_USER_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( NULL != g_pMain )
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_25217 ), g_pMain->GetNowState() );
}
#endif // SERV_ACCOUNT_BLOCK_USER_POPUP_TEMP


#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
// ���� ĳ���� ���� â�� ����� ������ ����
void CX2State::SetShowJumpingCharacterClassChangeDlg ( bool bIsShow_ )
{	
	if ( true == bIsShow_ )
	{

		CX2Unit::UNIT_TYPE eNowUnittype = CX2Unit::UT_NONE;
		if ( NULL != m_pDlgJumpingCharacterClassChange )
			SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChange );

		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_BATTLE_FIELD :
			{
				if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
				{
					eNowUnittype = g_pX2Game->GetMyUnit()->GetUnitType();
				}
			}
			break;
		case CX2Main::XS_VILLAGE_MAP :
			{
				if ( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() )
				{
					eNowUnittype = g_pTFieldGame->GetMyUnit()->GetUnit()->GetType();
				}
			}
			break;
		default :
			{
				SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChangeNotice );
				SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChange );
			}
			break;
		}

	

		wstring wstrCharacterScriptName = L"";
		
		if ( eNowUnittype != CX2Unit::UT_NONE )
		{
			// ���� Ÿ�Կ� �´� ��� ���� ����
			switch ( eNowUnittype ) 
			{
			case CX2Unit::UT_ELSWORD :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_ELSWORD.lua";
				break;
			case CX2Unit::UT_ARME :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_AISHA.lua";
				break;
			case CX2Unit::UT_LIRE :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_RENA.lua";
				break;
			case CX2Unit::UT_RAVEN :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_RAVEN.lua";
				break;
			case CX2Unit::UT_EVE :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_EVE.lua";
				break;
			case CX2Unit::UT_CHUNG :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_CHUNG.lua";
				break;
			case CX2Unit::UT_ARA :
				wstrCharacterScriptName = L"DLG_UI_JUMPING_EVENT_ARA.lua";
				break;

			default :
				break;
			}		
			m_pDlgJumpingCharacterClassChange = new CKTDGUIDialog( g_pMain->GetNowState(), wstrCharacterScriptName.c_str() );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgJumpingCharacterClassChange );
			m_pDlgJumpingCharacterClassChange->OnFrameMove(0.f, 0.01f);		// �ѹ� ����
		}
	}
	else
	{
		if ( NULL != m_pDlgJumpingCharacterClassChange )
		{
			m_pDlgJumpingCharacterClassChange->SetShowEnable( false, false );
		}
	}
}

// ���� ���� �˸�â�� ��� ������ ���� ������
void CX2State::SetShowJumpingCharacterClassNoticeChangeDlg ( bool bIsShow_ )
{	

	if ( true == bIsShow_ )
	{
		if ( NULL != m_pDlgJumpingCharacterClassChangeNotice )
			SAFE_DELETE_DIALOG ( m_pDlgJumpingCharacterClassChangeNotice );
		m_pDlgJumpingCharacterClassChangeNotice = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_JUMPING_EVENT_02.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgJumpingCharacterClassChangeNotice );
		g_pMain->SetIsJumpingCharacter( false );						// �˾� �ѹ� ������ �޽��� �ޱ� �������� �ٽ� �ȶߵ��� ���ش�.
	}

	else
	{
		if ( NULL != m_pDlgJumpingCharacterClassChangeNotice )
			m_pDlgJumpingCharacterClassChangeNotice->SetShowEnable( false, false );
	}
}

// ���� ĳ���� Unit Class �� �����Ѵ�.
void CX2State::Handler_EGS_JUMPING_CHARACTER_REQ( const int iUnitClassID_ )
{
	KEGS_JUMPING_CHARACTER_REQ kPacket;

	kPacket.m_iClass = iUnitClassID_;

	g_pData->GetServerProtocol()->SendPacket( EGS_JUMPING_CHARACTER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JUMPING_CHARACTER_ACK );
}

// ���� ĳ���� ������ ��� ��Ŷ�� �޾� ó���Ѵ�.
bool CX2State::Handler_EGS_JUMPING_CHARACTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JUMPING_CHARACTER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	

	if( g_pMain->DeleteServerPacket( EGS_JUMPING_CHARACTER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			// ���� ���� ����
			if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->Reset( kEvent.m_kUnitInfo ); 
			}
		
			

 			// ��ųƮ�� ������Ʈ. ���� �ʱ�ȭ ���� �ش�.
 			if( g_pData->GetUIManager() != NULL)
 			{
 				g_pData->GetUIManager()->CreateUISkillTree();		
 				g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
			}

			
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			// ��ųƮ���� ���� ��ų�� �ʱ�ȭ�Ѵ�
			{
				pUnitData->m_UserSkillTree.Reset( false, false, false, true );	// ��ų Ʈ�� �ʱ�ȭ
			#ifdef SKILL_PAGE_SYSTEM //JHKang
				pUnitData->m_UserSkillTree.SetDefaultSkill( pUnitData->m_UserSkillTree.GetUsingPage() );	// �� ������ �⺻ ��ų ����
			#else //SKILL_PAGE_SYSTEM
				pUnitData->m_UserSkillTree.SetDefaultSkill();		/// �� ������ �⺻ ��ų ����
			#endif //SKILL_PAGE_SYSTEM

				// ���� ��ų Ǯ����
				pUnitData->m_UserSkillTree.SetUnsealedSkill(kEvent.m_vecUnSealedSkill);

				if( NULL != g_pData->GetUIManager() )
				{
					if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
					{
						g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
					}

					if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO) )
					{
						g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
					}
				}
			}

			// ���� ����Ʈ �� ���� ���� ����Ʈ ���� ��Ŷ ����
			g_pData->GetQuestManager()->GiveUpForbiddenQuest();


			// ĳ���� �ʻ�ȭ ����
			if ( NULL != CX2GageManager::GetInstance() )
			{
				CX2GageManager::GetInstance()->SetCharacterImage(static_cast<CX2Unit::UNIT_CLASS>(kEvent.m_kUnitInfo.m_cUnitClass ));
			}

			// ��Ʋ�ʵ� �� ���, ������ ������ ��Ŷ�� ����
			// �ƴ� ��� ( ���� ), ������ ��� �����͸� �����Ѵ�.
			if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				switch ( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_BATTLE_FIELD:
					{
						Handler_EGS_STATE_CHANGE_FIELD_REQ( g_pData->GetBattleFieldManager().GetReturnVillageId() );
					} break;
				default:		// ����
					{			
						ResetUnitViewerInFieldSquare( g_pData->GetMyUser()->GetSelectUnit()->GetUID(), static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_kUnitInfo.m_cUnitClass ) );
					} break;
				}
			}

			// ������ �ߴ� ĳ���ͷ� ����, Dialog �� ����� �ʴ´�.
			g_pMain->SetIsJumpingCharacter( false );
			return true;
		}
	}

	return false;

}

// ���� ĳ���� ���� Dialog �� ���ִ°�?
bool CX2State::GetIsJumpingCharacterDlgPopup ( void ) const
{
	
	if ( NULL != m_pDlgJumpingCharacterClassChange )
	{	// Ŭ���� ���� Dialog 
		if ( true == m_pDlgJumpingCharacterClassChange->GetShow() )
		{
			return true;
		}
	}

	else if ( NULL != m_pDlgJumpingCharacterClassChangeNotice )
	{	// ���� ���� �˸��� Dialog
		if ( true == m_pDlgJumpingCharacterClassChangeNotice->GetShow() )
		{
			return true;
		}
	}

	return false;
}


#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
/** @function	: GetExpirationDataDesc
	@brief		: �븮 ���� Ÿ�Կ� ���� �ȳ� ���� ��ȯ �Լ� 
	@param		: �븮 ���� Ÿ��, �븮 ���� ���� ����
	@return		: ����� �ȳ� ����
*/
wstring CX2State::GetExpirationDataDesc( IN const SEnum::AGENCY_SHOP_TYPE eAgencyShopType_, IN const wstring& wstrAgencyShopExpirationDate_ )
{
	if ( NULL == g_pData )
		return L"";

	wstring wstrString = L"";

	// ���� �ð�
	switch( eAgencyShopType_ )
	{
	case SEnum::AST_PREMIUM:
		{
			wstrString = GET_REPLACED_STRING( ( STR_ID_28382, "L", GetExpirationDateDesc( wstrAgencyShopExpirationDate_, g_pData->GetServerCurrentTime(), false ) ) );
		} break;
	case SEnum::AST_NORMAL:
		{
			wstrString = GET_REPLACED_STRING( ( STR_ID_28381, "L", GetExpirationDateDesc( wstrAgencyShopExpirationDate_, g_pData->GetServerCurrentTime(), false ) ) );
		} break;
	default:
		{
			wstrString = GET_STRING( STR_ID_28380 );
		} break;
	}

	return wstrString;
}
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef SERV_CATCH_HACKUSER_INFO
bool CX2State::Handler_EGS_CATCH_HACKUSER_INFO_NOT( int iCrashType )
{
	KEGS_CATCH_HACKUSER_INFO_NOT kPacket;
	kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();
	kPacket.m_iUnitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	kPacket.m_iCrashType = iCrashType;

	g_pData->GetServerProtocol()->SendPacket( EGS_CATCH_HACKUSER_INFO_NOT, kPacket );

	return true;
}
#endif SERV_CATCH_HACKUSER_INFO

#ifdef SERV_RELATIONSHIP_EVENT_INT
bool CX2State::Handler_EGS_DIVORCE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DIVORCE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( false );
	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( 0 );
	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( L"" );	

	return true;
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON
bool CX2State::Handler_EGS_SHOW_DISCONNECT_REASON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SHOW_DISCONNECT_REASON_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	g_pMain->IsValidPacket( kEvent.m_iOK );

	return true;
}
#endif SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON

#ifdef SERV_RECRUIT_EVENT_BASE
bool CX2State::Handler_EGS_REGISTER_RECRUITER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REGISTER_RECRUITER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	wstring tempString;
	tempString = GET_REPLACED_STRING( ( STR_ID_543, "L", kEvent.m_kRecruitUnitInfo.m_wstrNickName ) );

	if ( g_pChatBox != NULL )
	{
		g_pChatBox->AddChatLog( tempString.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}

	if ( g_pX2Game == NULL )
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.c_str(), this );

	return true;
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
void CX2State::Handler_EGS_EGS_GET_EVENT_INFO_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_GET_EVENT_INFO_REQ );
	g_pMain->AddServerPacket( EGS_GET_EVENT_INFO_ACK );
}

bool CX2State::Handler_EGS_GET_EVENT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_EVENT_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_EVENT_INFO_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( g_pTFieldGame != NULL )
			{
				//g_pData->GetUIManager()->CloseAllNPCDlg();

				//SAFE_DELETE_DIALOG( m_pDlgEventCharacterRanking );
				m_pDlgEventCharacterRanking = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Character_Quest_Ranking_Event.lua" );

				if ( m_pDlgEventCharacterRanking != NULL )
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgEventCharacterRanking );

				//m_pDlgEventCharacterRanking->SetShowEnable(true, true);

				CKTDGUIStatic* pStaticRankingRate = 
					static_cast<CKTDGUIStatic*>( m_pDlgEventCharacterRanking->GetControl(L"RankingRate") );

				if ( NULL != pStaticRankingRate )
				{
					WCHAR wstrRankingRate[12][10];
					ZeroMemory( wstrRankingRate, sizeof(WCHAR) * 120 );

					int iDungeonCharacter1 = 0;
					int iDungeonCharacter2 = 0;
					int iDungeonCharacter3 = 0;
					int iDungeonCharacter4 = 0;

					//int iDungeonTotal = kEvent.m_kEventCharacterRaking.m_iDungeonCharacter1 + kEvent.m_kEventCharacterRaking.m_iDungeonCharacter2 + kEvent.m_kEventCharacterRaking.m_iDungeonCharacter3;
					int iDungeonTotal = kEvent.m_kEventCharacterRaking.m_iDungeonCharacter1 + kEvent.m_kEventCharacterRaking.m_iDungeonCharacter2 + kEvent.m_kEventCharacterRaking.m_iDungeonCharacter3 + kEvent.m_kEventCharacterRaking.m_iDungeonCharacter4;

					// �� �ִ°� ����

					if( iDungeonTotal != 0 )
					{
						iDungeonCharacter1 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iDungeonCharacter1 / (float)( iDungeonTotal ) ) + 0.005f ) *100 );
						iDungeonCharacter2 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iDungeonCharacter2 / (float)( iDungeonTotal ) ) + 0.005f ) *100 );
						iDungeonCharacter3 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iDungeonCharacter3 / (float)( iDungeonTotal ) ) + 0.005f ) *100 );
						iDungeonCharacter4 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iDungeonCharacter4 / (float)( iDungeonTotal ) ) + 0.005f ) *100 );					
					}

					int iPVPCharacter1 = 0;
					int iPVPCharacter2 = 0;
					int iPVPCharacter3 = 0;
					int iPVPCharacter4 = 0;

					//int iPVPTotal = kEvent.m_kEventCharacterRaking.m_iPVPCharacter1 + kEvent.m_kEventCharacterRaking.m_iPVPCharacter2 + kEvent.m_kEventCharacterRaking.m_iPVPCharacter3;
					int iPVPTotal = kEvent.m_kEventCharacterRaking.m_iPVPCharacter1 + kEvent.m_kEventCharacterRaking.m_iPVPCharacter2 + kEvent.m_kEventCharacterRaking.m_iPVPCharacter3 + kEvent.m_kEventCharacterRaking.m_iPVPCharacter4;

					if( iPVPTotal != 0 )
					{
						iPVPCharacter1 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iPVPCharacter1 / (float)( iPVPTotal ) ) + 0.005f ) *100 );
						iPVPCharacter2 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iPVPCharacter2 / (float)( iPVPTotal ) ) + 0.005f ) *100 );
						iPVPCharacter3 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iPVPCharacter3 / (float)( iPVPTotal ) ) + 0.005f ) *100 );
						iPVPCharacter4 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iPVPCharacter4 / (float)( iPVPTotal ) ) + 0.005f ) *100 );
					}

					int iFieldCharacter1 = 0;
					int iFieldCharacter2 = 0;
					int iFieldCharacter3 = 0;
					int iFieldCharacter4 = 0;

					//int iFieldTotal = kEvent.m_kEventCharacterRaking.m_iFieldCharacter1 + kEvent.m_kEventCharacterRaking.m_iFieldCharacter2 + kEvent.m_kEventCharacterRaking.m_iFieldCharacter3;
					int iFieldTotal = kEvent.m_kEventCharacterRaking.m_iFieldCharacter1 + kEvent.m_kEventCharacterRaking.m_iFieldCharacter2 + kEvent.m_kEventCharacterRaking.m_iFieldCharacter3 + kEvent.m_kEventCharacterRaking.m_iFieldCharacter4;

					if( iFieldTotal != 0 )
					{
						iFieldCharacter1 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iFieldCharacter1 / (float)( iFieldTotal ) ) + 0.005f ) *100 );
						iFieldCharacter2 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iFieldCharacter2 / (float)( iFieldTotal ) ) + 0.005f ) *100 );
						iFieldCharacter3 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iFieldCharacter3 / (float)( iFieldTotal ) ) + 0.005f ) *100 );
						iFieldCharacter4 = (int)(( ( kEvent.m_kEventCharacterRaking.m_iFieldCharacter4 / (float)( iFieldTotal ) ) + 0.005f ) *100 );
					}

					StringCchPrintf( wstrRankingRate[0], ARRAY_SIZE(wstrRankingRate[0]), L"%d%%", iDungeonCharacter1 );
					StringCchPrintf( wstrRankingRate[1], ARRAY_SIZE(wstrRankingRate[1]), L"%d%%", iDungeonCharacter2 );
					StringCchPrintf( wstrRankingRate[2], ARRAY_SIZE(wstrRankingRate[2]), L"%d%%", iDungeonCharacter3 );
					StringCchPrintf( wstrRankingRate[3], ARRAY_SIZE(wstrRankingRate[3]), L"%d%%", iPVPCharacter1 );
					StringCchPrintf( wstrRankingRate[4], ARRAY_SIZE(wstrRankingRate[4]), L"%d%%", iPVPCharacter2 );
					StringCchPrintf( wstrRankingRate[5], ARRAY_SIZE(wstrRankingRate[5]), L"%d%%", iPVPCharacter3 );
					StringCchPrintf( wstrRankingRate[6], ARRAY_SIZE(wstrRankingRate[6]), L"%d%%", iFieldCharacter1 );
					StringCchPrintf( wstrRankingRate[7], ARRAY_SIZE(wstrRankingRate[7]), L"%d%%", iFieldCharacter2 );
					StringCchPrintf( wstrRankingRate[8], ARRAY_SIZE(wstrRankingRate[8]), L"%d%%", iFieldCharacter3 );

					StringCchPrintf( wstrRankingRate[9], ARRAY_SIZE(wstrRankingRate[9]), L"%d%%", iDungeonCharacter4 );
					StringCchPrintf( wstrRankingRate[10], ARRAY_SIZE(wstrRankingRate[10]), L"%d%%", iPVPCharacter4 );
					StringCchPrintf( wstrRankingRate[11], ARRAY_SIZE(wstrRankingRate[11]), L"%d%%", iFieldCharacter4 );

					//pStaticRankingRate->GetString(0)->color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pStaticRankingRate->SetString( 0, wstrRankingRate[0] );
					pStaticRankingRate->SetString( 1, wstrRankingRate[1] );
					pStaticRankingRate->SetString( 2, wstrRankingRate[2] );
					pStaticRankingRate->SetString( 3, wstrRankingRate[3] );
					pStaticRankingRate->SetString( 4, wstrRankingRate[4] );
					pStaticRankingRate->SetString( 5, wstrRankingRate[5] );
					pStaticRankingRate->SetString( 6, wstrRankingRate[6] );
					pStaticRankingRate->SetString( 7, wstrRankingRate[7] );
					pStaticRankingRate->SetString( 8, wstrRankingRate[8] );
					pStaticRankingRate->SetString( 9, wstrRankingRate[9] );
					pStaticRankingRate->SetString( 10, wstrRankingRate[10] );
					pStaticRankingRate->SetString( 11, wstrRankingRate[11] );
				}
			}
		}
	}

	return true;
}
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
bool CX2State::Handler_ESG_REWARD_DB_DATA_NOT(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KESG_REWARD_DB_DATA_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pData != NULL && g_pData->GetRewardTable() != NULL)
	{
		g_pData->GetRewardTable()->SetRewardDBData( kEvent.m_mapTotalRewardData );
	}
	
	return true;
}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_ELESIS_UPDATE_EVENT
void CX2State::ProcessElesisEvent( UCHAR ucLevel, int iNoteViewCount )
{
	if( NULL != g_pMain &&
		NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		CX2Unit::UT_ELESIS == g_pData->GetMyUser()->GetSelectUnit()->GetType() )
	{
		if( ucLevel < 3 )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetNoteViewCount( iNoteViewCount );
			m_bShowNoteUI = false;
		}
		else if( ucLevel < 20 )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetNoteViewCount( iNoteViewCount );
			
			if( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD || g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME || g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
			{
				// ��Ʈ UI�� ��� �غ�
				ReadyToShowEventNoteUI();
				m_bShowNoteUI = true;
			}
			else
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetReserveShow( true );
			}
		}
		else
		{
			m_bShowNoteUI = false;
		}
	}
}

void CX2State::Handler_EGS_EVENT_NOTE_VIEW_REQ()
{
	if( m_bProcessNoteView == true )
	{
		return ;
	}

	m_bProcessNoteView = true;

	g_pData->GetServerProtocol()->SendID( EGS_EVENT_NOTE_VIEW_REQ );
	g_pMain->AddServerPacket( EGS_EVENT_NOTE_VIEW_ACK );
}

bool CX2State::Handler_EGS_EVENT_NOTE_VIEW_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_NOTE_VIEW_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_EVENT_NOTE_VIEW_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetNoteViewCount( kEvent.m_iNoteViewCount );

			if( m_bPlayNoteUI == false )
			{
				m_bPlayNoteUI = true;

				D3DXVECTOR3 boxPos = m_pDlgEventElesisNote->GetDummyPos( 0 );
				if( m_hMeshInstEventNote != INVALID_MESH_INSTANCE_HANDLE )
				{
					g_pData->GetUIMajorXMeshPlayer()->DestroyInstance( m_hMeshInstEventNote );
					if( NULL != m_pDlgEventElesisNote )
					{
						m_pDlgEventElesisNote->SetHasUnit( NULL );
					}
				}

				CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_EventNoteStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL, L"NoteViewStart", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
				g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_EventNoteStart);
				if( NULL != m_pDlgEventElesisNote )
				{
					m_pDlgEventElesisNote->SetHasUnit(pMeshInst_EventNoteStart);

				}
				m_hMeshInstEventNoteStart = pMeshInst_EventNoteStart->GetHandle();

				// �Ʒ��� Īȣ ���� ���� ó��( Handler_EGS_EQUIP_TITLE_ACK ���� )
				g_pData->GetTitleManager()->AttachTitle( kEvent.m_iTitleID );

				if( NULL != g_pData &&
					NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					/// ��ų �߰� ���� ȿ�� ����
					g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelBySocket();
				}

				CX2GUUser* pUser = ( NULL != g_pX2Game ? g_pX2Game->GetUserUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() ) : NULL );

				if( pUser != NULL )
				{
					pUser->UpdateEquippedEmblem();
					pUser->NotifyShowObjectChanged();
				}
		
				m_iRewardTitleIndex = ( kEvent.m_iTitleID - 5730 ) / 10;
			}
		}
	}

	return true;
}

void CX2State::ReadyToShowEventNoteUI()
{
	if( m_pDlgEventElesisNote == NULL )
	{
		m_pDlgEventElesisNote = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Event_NoteView.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgEventElesisNote );

		D3DXVECTOR3 boxPos = m_pDlgEventElesisNote->GetDummyPos( 0 );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_EventNote = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL, L"NoteViewWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );

		g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_EventNote);

		if( NULL != m_pDlgEventElesisNote )
		{
			m_pDlgEventElesisNote->SetHasUnit( pMeshInst_EventNote );
		}

		m_hMeshInstEventNote = pMeshInst_EventNote->GetHandle();
	}			
}
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
bool CX2State::Handler_EGS_EVENT_CHUNG_GIVE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_CHUNG_GIVE_ITEM_NOT kEventPacket;
	DeSerialize( pBuff, &kEventPacket );	
	int iOk = 0;
	CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
	if(pStateMenu == NULL)
	{
		return false;
	}
	if( g_pMain->IsValidPacket( iOk ) == true )
	{
		///UIȮ�� ����
		if( kEventPacket.m_bGiveItemGet == false ) //������ �޾ƾ��� UIȰ��
		{
			//UIǥ�ô� ���⼭ ���������� ���⼭�� ǥ�� ������ Ȯ���� ����.
			if( g_pInstanceData != NULL )
				g_pInstanceData->SetChungUIShow(true);
		}
		else
		{
			///���� ��Ȱ�� UI ǥ��
			if( g_pInstanceData != NULL )
				g_pInstanceData->SetChungUIShow(false);
		}
		if( g_pInstanceData != NULL )
		{
			g_pInstanceData->SetChungUIClass(kEventPacket.m_cGetUnitClass);
			g_pInstanceData->SetNextGiveItem(kEventPacket.m_bTwoGiveItem); //�ڽ��� ���� ���� �������� �޾Ҵ��� üũ 
			g_pInstanceData->SetToolTipTime(kEventPacket.m_wstrToolTipTime);
		}
		return true;
	}
	return false;
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
bool CX2State::Handler_EGS_EVENT_COBO_DUNGEON_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_COBO_DUNGEON_FIELD_NOT kEventPacket;
	DeSerialize( pBuff, &kEventPacket );
	//���⼭ ������ �޾Ƽ� statemenu�� �ѱ��.
	if(g_pMain->IsValidPacket(kEventPacket.m_iOk) == true)
	{
		if(g_pInstanceData != NULL)
		{
			g_pInstanceData->SetStartUI(kEventPacket.m_StartButtonUI);
			g_pInstanceData->SetDungeonCountUI(kEventPacket.m_DungeonCountUI);
			g_pInstanceData->SetFieldCountUI(kEventPacket.m_FieldCountUI);
			g_pInstanceData->SetDungeonCount(kEventPacket.m_DungeonCount);
			g_pInstanceData->SetFieldCount(kEventPacket.m_FieldMonsterKillCount);
			g_pInstanceData->SetRemaindTime(kEventPacket.m_iRemaindTime);
			g_pInstanceData->SetButtonPushTime(kEventPacket.m_tPushTime);
		}
	}
	return true;
}
bool CX2State::Handler_EGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT kEventPacket;
	DeSerialize( pBuff, &kEventPacket );
	g_pInstanceData->SetDungeonCount(kEventPacket.m_iDungeonClearCount);
	return true;
}
bool CX2State::Handler_EGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT kEventPacket;
	DeSerialize( pBuff, &kEventPacket );
	g_pInstanceData->SetFieldCount(kEventPacket.m_iFieldMonsterKillCount);
	return true;
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
bool CX2State::Handler_EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT kEventPacket;
	DeSerialize( pBuff, &kEventPacket );
	//���⼭ ���ֵ����Ϳ� ���� ���� 
	if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		//12�� 5���� ���� ���� ���� ������ GetUnitData�� ��� �ϼ���.
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetValentineItemCount(kEventPacket.m_iValentineItemCount);
	}
	return true;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM