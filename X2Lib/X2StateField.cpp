#include "StdAfx.h"

#include ".\x2statefield.h"

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
	KRenderState( D3DRS_ALPHATESTENABLE,	TRUE ),
	KRenderState( D3DRS_ALPHAFUNC,			D3DCMP_NOTEQUAL ),
	KRenderState( D3DRS_ALPHAREF,			0 ),

	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),

	KRenderState( D3DRS_ZENABLE,			FALSE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//s_akStates[]
//}} robobeg : 2008-10-13


// g_pTFieldGame�� ����� �� �� null check ���ּ���. null�� �Ǵ� ���� �־��~ Ư�� ��Ŷ �ڵ鷯���� ����� �� �������ּ���~

CX2StateField::CX2StateField(void):
m_TimerWaitingPortal(3.f),
m_RenderStateID(s_akStates),
m_TexDataMovingGageBG( L"DLG_LOADING_BAR.tga", L"LOADING_BG" ),
m_TexDataMovingGage( L"DLG_LOADING_BAR.tga", L"LOADING_BAR" )
#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
, m_SumDelta( 0 )
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL
{
	SetNowMovingToBattleField( false );
	m_fLoadCompleteTimer = 0;
	m_bSendLoadCompleteReq = false;
	m_bLoadCompleteReq = false;

	m_pDLGJoinParty = NULL;
	m_pDLGExitMsgBox	= NULL;
#ifdef SERV_EVENT_GUIDELINE_POPUP
	m_pDLGDungeonReformGuide = NULL;
#endif //SERV_EVENT_GUIDELINE_POPUP

	m_TalkBoxunitUIDToFind = 0;
	m_TalkBoxunitUID = 0;
	
	m_iTalkNpcIndex = -1;

	g_pKTDXApp->GetDIManager()->SetEnable(true);
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(),	true );
	
	//{{ dmlee 2009.3.25 �ŷ������ ������ �Դٰ��� �� �� ũ���� ���� ���������� �ϴ� comment out
	//	//{{ seojt // 2008-10-22, 17:22
	//#ifdef BACKGROUND_LOADING_TEST // 2008-10-23
	//
	//	/** urgent data�� �ε� ó��:
	//	���� ī�޶� �������� �������� �����͸� urgent data�� �����Ͽ� ��� �ε��Ѵ�.
	//	ObjectChainSort()�� ȣ���ϸ� ���� ī�޶� ������ ���ؼ� culling �۾��� �Բ�,
	//	������ ���� �ð� time stamp�� g_NowTime���� �����Ѵ�.
	//	������ ī�޶� ������ X2SquareGame.cpp���� m_pCamera->SquareCameraUpdate()��
	//	ȣ���ؼ� ������ �ξ���.
	//	LoadUrgentData()�� resource loading thread�� ��û ť�� ��� �ִ� ��带 �˻��Ͽ�
	//	������ ���� �ð��� fTimeStamp���� ũ�ų� ���� ��� ��忡 ���ؼ� �ε带 �����Ѵ�.
	//	LoadUrgentData()�� urgent resource�� ��� �ε�� ������ block���°� �ȴ�.
	//
	//	@note   �Ʒ� ����� �ּ�ó���ϸ�, urgent data�� �ε���� �ٷ� �������� �����ϹǷ�
	//	���� ��ü���� �ǽð� �ε�Ǵ� ���� ������ Ȯ���� �� �ִ�.
	//
	//	- jintaeks on 2008-10-24, 9:36 */
	//	{
	//		double  fTimeStamp = g_NowTime;
	//		// frustum�� ���� ���� ��ü���� time stamp�� g_NowTime���� �����Ѵ�.
	//		// urgent data�� �ε带 ���� ��ó�� �۾��̴�.
	//		g_pKTDXApp->GetDGManager()->ObjectChainSort();
	//		// last accesstime���� fTimeStamp�� ���� ũ�ų� ���� ��� ���ҽ��� �ε��϶�� �䱸�Ѵ�.
	//		g_pKTDXApp->GetDeviceManager()->LoadUrgentData( fTimeStamp );
	//	}//block
	//
	//#endif // BACKGROUND_LOADING_TEST // 2008-10-22
	//	//}} seojt // 2008-10-22, 17:22
	//}} dmlee 2009.3.25 �ŷ������ ������ �Դٰ��� �� �� ũ���� ���� ���������� �ϴ� comment out

	//g_pKTDXApp->GetDGManager()->GetCamera().Point( 6705, 1164, 4121, 6701, 1013, 4907 ); 
	//g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( 1.0f );


	// ���� ���� ���̾�α� 
	m_pDLGSquareTitle = NULL;
	//m_pDLGSquareTitle = new CKTDGUIDialog( this, L"DLG_SquareUI_Title.lua" );
	//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSquareTitle );	
	SetVillageName();

	g_pData->GetUserMenu()->SetStage(this);

#ifdef CHARACTER_MOUSE_RBUTTON
	g_pData->GetUserListPopUp()->SetStage(this);
#endif CHARACTER_MOUSE_RBUTTON

	SetOpenCommonBG( false );

	ResourcePreLoad();

	if(g_pTFieldGame != NULL)
	{
		g_pTFieldGame->SetStage(this);
	}

	if( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->SetHideWindow(false);


#ifdef HEAP_BROKEN_BY_ROOM
	g_pData->DeleteDungeonResultInfo();
	g_pData->DeletePVPResultInfo();
#else	HEAP_BROKEN_BY_ROOM
	g_pData->DeleteDungeonRoom();
	g_pData->DeleteDungeonResultInfo();
	g_pData->DeletePVPRoom();
	g_pData->DeletePVPResultInfo();
	g_pData->DeleteBattleFieldRoom();
#endif // HEAP_BROKEN_BY_ROOM



	// �ʵ�ɼ��� ������ ������.
	Handler_EGS_OPTION_UPDATE_REQ();
	
	// x2state�����ڿ��� g_pChatBox�� ���������� �ʴ´ٸ� 
	// ���⼭ �ٽ� chatbox type�� �������־�� �Ѵ�.
	// ó�� ���� ���Խ� g_pChatbox�� �����Ǿ� ���� ���� ���¿��� 
	// x2state���� state type�� �°� chatbox type�� �����Ѵ�.
	if(g_pChatBox != NULL)
	{
		g_pChatBox->SetChatBoxLayerUp(false);
		g_pChatBox->CloseChatWindowButton();
		g_pChatBox->OpenChatWindow();
		g_pChatBox->HideChatWindowFrame();
		g_pChatBox->HideChatEditBox();
	}

#ifdef DLL_MANAGER
	if( g_pMain != NULL )
		g_pMain->DllManagerReport();
#endif

#ifdef TUTORIAL_TRAINGING_KEYPROCESS_FIX
	g_pMain->SetIsPlayingFreeTraining(false);
#endif TUTORIAL_TRAINGING_KEYPROCESS_FIX

#ifdef SERV_DLL_MANAGER
	SendLoadedModuleList();
#endif
	
#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE	/// �ش� ��� CX2State�� �̵� ( ���� ���� �� �����ϴٰ� �Ǵܽ� ���� ���� ���� ���� )
#ifdef NEW_DEFENCE_DUNGEON
	m_pFontDefenceDungeonNotice	= NULL;	/// ����� �� ���̵� ����� ȭ�鿡 ��µ� �ؽ�Ʈ
	m_fDefenceDungeonNoticeTime = 0.f;	/// ����� �� ���� ����� �����Ǵ� �ȳ� �ؽ�Ʈ ǥ�� �ð�
	m_iDefenceDungeonNoticeType	= 0;	/// �ȳ� �ؽ�Ʈ ����
#endif NEW_DEFENCE_DUNGEON
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	m_pFontTogetherFestivalEarthQuakeNotice = NULL;		/// ������ ���� ���� �̺�Ʈ ����
	m_fTogetherFestivalEarthQuakeTime = 0.f;			/// ������ ���� ���� �̺�Ʈ ���� ��� ǥ�� �ð�
#endif TOGETHER_FESTIVAL_2012_AUTUMN
	//EGS_GET_USER_LIST_IN_SECTOR_NOT();
	CreateMovingSmallBar();
}//CX2StateField::CX2StateField()


CX2StateField::~CX2StateField(void)
{
#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	/// ���� �ȳ� ���� ����
	m_fTogetherFestivalEarthQuakeTime = 0.f;

	m_pFontTogetherFestivalEarthQuakeNotice = NULL;

	/// ī�޶� ũ���� ����
	if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetCamera() )
	{
		g_pTFieldGame->GetCamera()->GetCamera().InitUpDownCrashCameraSmooth();
	}
#endif TOGETHER_FESTIVAL_2012_AUTUMN
	SAFE_DELETE_DIALOG( m_pDLGJoinParty );
	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGSquareTitle );	

#ifdef SERV_EVENT_GUIDELINE_POPUP
	SAFE_DELETE_DIALOG( m_pDLGDungeonReformGuide );
#endif //SERV_EVENT_GUIDELINE_POPUP
	SAFE_DELETE( g_pTFieldGame );

	g_pKTDXApp->GetDIManager()->SetEnable(true);	

#ifdef BUFF_ICON_UI
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	if( NULL != pGageManager )
	{
		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			CX2Unit* pCX2Unit = g_pData->GetMyUser()->GetSelectUnit();

			if( NULL != pCX2Unit )
				pGageManager->ClearBuffList( pCX2Unit->GetUID() );
		}
	}
#endif //BUFF_ICON_UI

	if( NULL != g_pData->GetPlayGuide() )
		g_pData->GetPlayGuide()->StopGoEffect();
}

void CX2StateField::ResourcePreLoad()
{
	SoundReady( L"X2_Button_Mouse_Up.ogg" );

	XSkinMeshReady( L"Mesh_Arme_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Elsword_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Eve_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Lire_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Raven_Base_SD.x" );

//{{ kimhc // 2010.12.9 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	XSkinMeshReady( L"Mesh_CHUNG_Base_SD.x" );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.9 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	XSkinMeshReady( L"Mesh_Ara_Base_SD.x" );
#endif
#ifdef NEW_CHARACTER_EL
	XSkinMeshReady( L"Mesh_EL_Base_SD.x" );
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	XSkinMeshReady( L"Mesh_Add_Base_SD.x" );
#endif //SERV_9TH_NEW_CHARACTER
	//TextureReady( L"Motion_Cubic2.tga" );
}

void CX2StateField::SetVillageName()
{
	//CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSquareTitle->GetControl( L"Static_SquareTitle" );
	//pStatic->GetString(0)->msg = g_pData->GetLocationManager()-> GetVillageMapTemplet(g_pData->GetLocationManager()->GetCurrentVillageID())->m_Name;

	m_bLoadCompleteReq = false;
	if( NULL != g_pData && NULL != g_pData->GetLocationManager() && g_pData->GetPlayGuide() )
	{
		//�����̵� ���̵� ��� ���¿��� �ȳ����� ������ �����ϸ� ���̵� ����
		if( g_pData->GetLocationManager()->GetCurrentVillageID() == g_pData->GetPlayGuide()->GetVillageID() )
		{
			g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_COMPLETE_EPIC_QUEST, false);
		}
	}
}

HRESULT CX2StateField::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if(g_pTFieldGame == NULL)
		return S_OK;

	if( true == GetNowMovingToBattleField() )
		return S_OK;

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );	

	
	m_fLoadCompleteTimer += fElapsedTime;

	if( g_pTFieldGame != NULL && 
#ifndef NEW_SKILL_TREE
		false == g_pInstanceData->GetSkillTreeUI()->GetShow() &&
#endif
		/*m_pCharacterRoom->GetOpen() == false && */
        m_pCashShop != NULL &&
		m_pCashShop->GetOpen() == false )
	{
		CX2WorldMapUI *pWorldMapUI = g_pInstanceData->GetMiniMapUI()->GetWorldMapUI();
		bool bShowWorld = false;
		if( pWorldMapUI != NULL && pWorldMapUI->GetShow() == true )
			bShowWorld = true;

		bool bShowLocalMap = false;
		if( g_pMain->GetPartyUI() != NULL)
		{
			bShowLocalMap = g_pMain->GetPartyUI()->GetShowLocalMap();
		}

#ifndef REFORM_NOVICE_GUIDE
		bool bShowNoviceGuide = false;
		if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetShowNoviceNpcGuide() == true)
		{
			bShowNoviceGuide = true;
		}
#endif //REFORM_NOVICE_GUIDE
		bool bShowUserMenu = false;
		if(g_pData != NULL && g_pData->GetUserMenu() != NULL && g_pData->GetUserMenu()->GetShowMenu() == true)
		{
			bShowUserMenu = true;
		}

		//{{ ����� : [2009/10/13] //
#ifdef GUILD_MANAGEMENT
		bool bShowGuildInfo = false;

		if(g_pData->GetGuildManager() != NULL )
		{
			bShowGuildInfo = g_pData->GetGuildManager()->GetUIGuild()->GetShowGuildInfo();
		}

#endif	//	GUILD_MANAGEMENT
		//}} ����� : [2009/10/13] //

#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
		bool bWriteMemo = false;
		if( g_pData->GetUIManager()->GetUISkillNote() == NULL ||
			(	g_pData->GetUIManager()->GetUISkillNote() != NULL &&
				g_pData->GetUIManager()->GetUISkillNote()->GetWriteMemoTime() <= 0.f ) )
			bWriteMemo = false;
		else
			bWriteMemo = true;
#endif // REFORM_SKILL_NOTE_UI

		// �̺�Ʈ(Ű/���콺) ó��	
		if(
#ifndef REFORM_NOVICE_GUIDE
			bShowNoviceGuide == false &&
#endif //REFORM_NOVICE_GUIDE
			bShowWorld == false && 
			bShowLocalMap == false &&
			m_Book.GetShow() == false &&
			GetShowSystemMenu() == false &&
			GetShowMsgBox() == false &&
			GetOptionWindowOpen() == false &&
#ifdef GUILD_MANAGEMENT
			bShowGuildInfo  == false &&
#endif	//	GUILD_MANAGEMENT
#ifdef ATTRACTION_ITEM_TEST
			g_pData != NULL && g_pData->GetUIManager() != NULL && 
			g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayAttractionItem() == false &&
#endif
#ifdef SERV_TREASURE_BOX_ITEM
			g_pData != NULL && g_pData->GetUIManager() != NULL && 
			g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayTreasureBox() == false &&
#endif SERV_TREASURE_BOX_ITEM
#ifdef FREE_WARP
			g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetShowWarpDest() == false &&
#endif
#ifdef SERV_ADD_WARP_BUTTON
			GetShowWarpDest() == false &&
#endif // SERV_ADD_WARP_BUTTON
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetShowSecondJobDest() == false &&
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB
#ifdef SERV_EPIC_QUEST
			g_pData->GetUIManager()->GetUIQuestNew() != NULL &&
			g_pData->GetUIManager()->GetUIQuestNew()->GetShowClearDLG() == false &&
#endif SERV_EPIC_QUEST			
#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
			bWriteMemo == false && 		
#endif // REFORM_SKILL_NOTE_UI
#ifdef SERV_KEY_MAPPING_INT
			GET_KEY_STATE( GAMEACTION_ATTACK_FAST ) == TRUE )
#else // SERV_KEY_MAPPING_INT
			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_Z) == TRUE )
#endif // SERV_KEY_MAPPING_INT
		{
			bool bCommunityChat = false;
			CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
			if ( pControl != NULL )
			{
				if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
					pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
				{
					bCommunityChat = true;
				}
			}

			//if(g_pChatBox->GetChatEditBox() == false)
			if(g_pChatBox->GetFocus() == false && bCommunityChat == false)
			{
				if(bShowUserMenu == true)
				{
					bShowUserMenu = false;
					g_pData->GetUserMenu()->ClosePopupMenu();
				}


//{{ ����� - ĳ���� ��Ŭ�� �˾� �޴� UI ���� 
#ifdef CHARACTER_MOUSE_RBUTTON
				if( g_pData != NULL && g_pData->GetUserListPopUp()!= NULL && g_pData->GetUserListPopUp()->GetShowMenu() == true )
					g_pData->GetUserListPopUp()->ClosePopupMenu();

#endif CHARACTER_MOUSE_RBUTTON
//}}  ����� - ĳ���� ��Ŭ�� �˾� �޴� UI ���� 

#ifdef DIALOG_SHOW_TOGGLE
				if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
					g_pTFieldGame->TalkNpc();
#else
				g_pTFieldGame->TalkNpc();
#endif
				
			}
		}

		if( g_pMain->GetPostBox()->GetEnable() == false && 
			g_pData->GetUIManager()->IsOpenedWindow() == false && 
			( NULL == g_pData->GetMessenger() || g_pData->GetMessenger()->GetOverDialog() == false ) &&
			GetShowSystemMenu() == false &&
			GetShowMsgBox() == false &&
			m_Book.GetShow() == false &&
#ifdef ATTRACTION_ITEM_TEST
			g_pData != NULL && g_pData->GetUIManager() != NULL && 
			g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayAttractionItem() == false &&
#endif
#ifdef SERV_TREASURE_BOX_ITEM
			g_pData != NULL && g_pData->GetUIManager() != NULL && 
			g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayTreasureBox() == false &&
#endif SERV_TREASURE_BOX_ITEM
			bShowWorld == false &&
			bShowLocalMap == false )
		{
			
			UserAndPetListPopUpMenuProcess();

			if( GetOptionWindowOpen() == false &&
				bShowUserMenu == false 
				//{{ kimhc // 2009-12-08 // NPC�� ��ȭ���� ���°� �ƴѰ�� Ŭ�� ����
#ifdef	CHRISTMAS_TREE
				&& g_pTFieldGame->GetJoinNpc() == false
#endif	CHRISTMAS_TREE
				//}} kimhc // 2009-12-08 // NPC�� ��ȭ���� ���°� �ƴѰ�� Ŭ�� ����

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
				&& g_pKTDXApp->GetDIManager()->IsActivated() == true
#endif

				)
			{
				m_iTalkNpcIndex = -1;

				for(int i=0; i<g_pTFieldGame->GetFieldNPCCount(); ++i)
				{
					CX2TFieldNpc* pNpc = g_pTFieldGame->GetFieldNPC(i);

					pNpc->SetPickNpc(false);
					if(pNpc != NULL && pNpc->GetInArea() == true)
					{
						D3DXVECTOR3 pos = pNpc->GetPos();
#ifdef PRIORITY_PARTY_TALK_BOX
						D3DXVECTOR2 mousePos;
#endif PRIORITY_PARTY_TALK_BOX
						
						pos.y += 120.f;

						
						D3DXVECTOR3 startRay, endRay;
						g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
							g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

						// ����� 2010.09.30   NPC ���� ��Ƽ ������ �ִ� ��� ��Ƽ ���� Ŭ���� �켱�� �ϵ���
#ifdef PRIORITY_PARTY_TALK_BOX
						mousePos.x = (FLOAT)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
						mousePos.y = (FLOAT)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
#endif PRIORITY_PARTY_TALK_BOX
						// ����� 2010.09.30   NPC ���� ��Ƽ ������ �ִ� ��� ��Ƽ ���� Ŭ���� �켱�� �ϵ���
												
						
						if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos, pNpc->GetBoundingRadius()/2.0f ) == true )
						{
							
							//{{ ������ : ��� UI �Ʒ� NPC�� ���� �� UI�� �켱�ϵ��� ����
#ifdef PRIORITY_EVERY_UI
							if( g_pKTDXApp->GetDGManager()->GetDialogManager()->ExistDialogAtMousePos() == true )
							{
								//nothing
							}
							else
#else PRIORITY_EVERY_UI
							//}} ������ : ��� UI �Ʒ� NPC�� ���� �� UI�� �켱�ϵ��� ����

							//{{ ����� 2010.09.30  NPC ���� ��Ƽ ������ ���� ��� ��Ƽ ���� Ŭ���� �켱��
	#ifdef PRIORITY_PARTY_TALK_BOX
							if( g_pTFieldGame != NULL &&
								g_pTFieldGame->GetTalkBoxManagerPartyLeader() != NULL && 
								g_pTFieldGame->GetTalkBoxManagerPartyLeader()->CheckMousePointInTalkbox( mousePos, m_TalkBoxunitUIDToFind ) == true )   //
							{
								//nothing
							}
							else
	#endif PRIORITY_PARTY_TALK_BOX
							//}} �����  2010.09.30 NPC ���� ��Ƽ ������ ���� ��� ��Ƽ ���� Ŭ���� �켱��

#endif PRIORITY_EVERY_UI
							{
								pNpc->SetPickNpc(true);

#ifdef PRIORITY_EVERY_UI
								m_iTalkNpcIndex = i;
#else PRIORITY_EVERY_UI
								if( g_pData->GetUserListPopUp()->GetShowMenu() == false && g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_LBUTTON ) == TRUE )
								{
#ifdef DIALOG_SHOW_TOGGLE
									if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
										g_pTFieldGame->TalkNpc(i);
#else
									g_pTFieldGame->TalkNpc(i);
#endif //DIALOG_SHOW_TOGGLE
									//m_iTalkNpcIndex = i;
								}
#endif PRIORITY_EVERY_UI
							}
						}
					}
				}
			}						
		}
#ifdef PRIORITY_EVERY_UI
		else if( m_iTalkNpcIndex != -1 )
		{
			for( int i = 0; i < g_pTFieldGame->GetFieldNPCCount(); ++i )
			{
				CX2TFieldNpc* pNpc = g_pTFieldGame->GetFieldNPC( i );
				if( pNpc )
					pNpc->SetPickNpc( false );
			}
			m_iTalkNpcIndex = -1;
		}
#endif PRIORITY_EVERY_UI


		if( g_pTFieldGame != NULL && m_bSendLoadCompleteReq == false)
		{
			g_pTFieldGame->OnFrameMove( fTime, fElapsedTime );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if( true == MoveToOtherPlace( fElapsedTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( true == MoveToOtherPlace() )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			{
				return S_OK;
			}

			g_pTFieldGame->SetEnableKeyProcess(true);
			
			// �ý��� â�� ���� ������� ĳ���� �������� ���ϵ���...
			// �˾� Ȯ��â�� ���� ������� ĳ���� �������� ���ϵ���...
			if( GetShowSystemMenu() == true ||
				GetShowMsgBox() == true ||
				bShowLocalMap == true ||

#ifndef REFORM_NOVICE_GUIDE
				g_pTFieldGame->GetNoviceGuide()->GetShowGuideDlg() == true ||
#endif //REFORM_NOVICE_GUIDE
#ifdef FREE_WARP
				(g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetUIInventory() != NULL && 
				g_pData->GetUIManager()->GetUIInventory()->GetShowWarpDest() == true) 
#endif
#ifdef SERV_ADD_WARP_BUTTON
				|| (CX2StateMenu::GetShowWarpDest() == true )
#endif // SERV_ADD_WARP_BUTTON
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
				|| (g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetUIInventory() != NULL && 
				g_pData->GetUIManager()->GetUIInventory()->GetShowSecondJobDest() == true) 
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB
				)
			{
				g_pTFieldGame->SetEnableKeyProcess(false);
			}
		}
	}
	else
	{
		if(g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetDialogShow() == true)
		{
			g_pMain->GetPostBox()->SetDialogShow(false);
		}

		if(g_pTFieldGame->GetEnableKey() == true)
			g_pTFieldGame->SetEnableKeyProcess(false);
		g_pTFieldGame->SetShowField(false);

		g_pData->GetUIManager()->CloseAll();
	}

#ifdef FRAME_MOVE_CULL

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_Y) == TRUE )
	{
		CKTDGXSkinAnim::SetTestUpdate( !CKTDGXSkinAnim::GetTestUpdate() );
	}

#endif //FRAME_MOVE_CULL

#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE	/// �ش� ��� CX2State�� �̵� ( ���� ���� �� �����ϴٰ� �Ǵܽ� ���� ���� ���� ���� )
#ifdef NEW_DEFENCE_DUNGEON
	//����� �� ���̵� ����� �����Ǵ� �ȳ� ���� ǥ�� �ð�
	if( m_fDefenceDungeonNoticeTime > 0.f )
		m_fDefenceDungeonNoticeTime -= fElapsedTime;
	else
		m_fDefenceDungeonNoticeTime = 0.f;
#endif NEW_DEFENCE_DUNGEON
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN

	/// ĳ�ü��� �����ų� NPC�� ��ȭ �� ��, ���� ��� ����
	if( ( NULL != g_pData && NULL != g_pData->GetCashShop() && true == g_pData->GetCashShop()->GetOpen() ) ||
		( NULL != g_pTFieldGame && true == g_pTFieldGame->GetJoinNpc() ) )
	{
		/// ���� �ȳ� ���� ����
		m_fTogetherFestivalEarthQuakeTime = 0.f;

		m_pFontTogetherFestivalEarthQuakeNotice = NULL;

		/// ī�޶� ũ���� ����
		if( NULL != g_pTFieldGame->GetCamera() )
		{
			g_pTFieldGame->GetCamera()->GetCamera().InitUpDownCrashCameraSmooth();
		}
	}

	/// ���� �ȳ� ���� ǥ�� �ð�
	if( m_fTogetherFestivalEarthQuakeTime > 0.f )
		m_fTogetherFestivalEarthQuakeTime -= fElapsedTime;
	else
		m_fTogetherFestivalEarthQuakeTime = 0.f;

#endif TOGETHER_FESTIVAL_2012_AUTUMN

	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB) == TRUE &&
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LALT) != TRUE )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ToggleUserShowQuickQuestDLG();
	}

#ifdef ADD_SHORT_CUT_KEY_IN_HOUSE
	if( NULL != g_pData->GetMyUser() &&
		CX2User::XUAL_DEV <= g_pData->GetMyUser()->GetAuthLevel() )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F2) == TRUE )
		{
			g_pData->SetStateArg(2);
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_755 ) );
			if(g_pTFieldGame != NULL)
				g_pTFieldGame->Handler_EGS_CREATE_TC_ROOM_REQ( 1 );
		}
		else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F3) == TRUE )
		{
			if(g_pTFieldGame != NULL)
				g_pTFieldGame->Handler_EGS_OPEN_PVP_ROOM_LIST_REQ();
		}
	}
#endif // ADD_SHORT_CUT_KEY_IN_HOUSE
#ifdef VILLAGE_MAP_FAST_RELOADING
	if( NULL != g_pData->GetMyUser() &&
		CX2User::XUAL_DEV <= g_pData->GetMyUser()->GetAuthLevel() )
	{
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F4) == TRUE )
		{
			if( NULL != g_pInstanceData->GetMiniMapUI() &&
				NULL != g_pInstanceData->GetMiniMapUI()->GetWorldMapUI() )
			{
				CX2WorldMapUI *pWorldMapUI = g_pInstanceData->GetMiniMapUI()->GetWorldMapUI();
				if( NULL != pWorldMapUI )
				{
					pWorldMapUI->ReLoadingVillageMap();
				}
			}
		}	
	}
#endif //VILLAGE_MAP_FAST_RELOADING
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
	if (g_pData != NULL &&	g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().GetItemByTIDCheckAll( TEAR_OF_ELWOMAN_ITEM_ID, true ) != NULL)
		{
			int ItemByTIDCheckAllCount = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().GetItemByTIDCheckAll( TEAR_OF_ELWOMAN_ITEM_ID, true );
			g_pInstanceData->GetMiniMapUI()->UpdateTearOfELWomanCount(ItemByTIDCheckAllCount);
			g_pData->GetMyUser()->SetUserTearOfELWoman(ItemByTIDCheckAllCount);
		}
		else
		{
			g_pInstanceData->GetMiniMapUI()->UpdateTearOfELWomanCount(0);
			g_pData->GetMyUser()->SetUserTearOfELWoman(0);
		}
	}
#endif SERV_EVENT_TEAR_OF_ELWOMAN

	return S_OK;
}

HRESULT CX2StateField::OnFrameRender()
{
	KTDXPROFILE();

	
	if( g_pInstanceData->GetIsDungeonLounge() == true )
	{
		if( false == g_pInstanceData->GetLoadedDungeonLoungeUnits() )
		{
			g_pInstanceData->SetLoadedDungeonLoungeUnits( true );

			const std::vector< KFieldUserInfo >& vecFieldUserInfo = g_pInstanceData->GetDungeonLoungeUnitInfo();
			for( UINT i=0; i<vecFieldUserInfo.size(); i++ )
			{
				const KFieldUserInfo& fieldUserInfo = vecFieldUserInfo[i];
				JoinSector( (KFieldUserInfo*) &fieldUserInfo );
			}
		}


		if(g_pTFieldGame != NULL && m_bLoadCompleteReq == false)
		{
			m_bLoadCompleteReq = true;
			g_pTFieldGame->SetLoadComplete(true);
		}
	}
	else
	{
		if( m_bLoadCompleteReq == false )
		{
			Handler_EGS_FIELD_LOADING_COMPLETE_REQ();
		}
	}

	

	if( g_pTFieldGame != NULL && 
#ifndef NEW_SKILL_TREE
		false == g_pInstanceData->GetSkillTreeUI()->GetShow() &&
#endif
		/*m_pCharacterRoom->GetOpen() == false &&*/ 
        m_pCashShop != NULL &&
		m_pCashShop->GetOpen() == false)
	{
		g_pTFieldGame->OnFrameRender();
	}
	else
	{
//{{ robobeg : 2008-10-18
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		g_pKTDXApp->GetDGManager()->ObjectChainSort();

		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
        g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
//}} robobeg : 2008-10-18
	}


	if(g_pTFieldGame != NULL && g_pTFieldGame->GetShowLoadUi() == false)
	{
		g_pKTDXApp->GetDGManager()->FrontUIRender();

		if ( g_pMain->GetTextManager() != NULL )
			g_pMain->GetTextManager()->OnFrameRender();

		if ( g_pMain->GetGameEdit() != NULL )
			g_pMain->GetGameEdit()->OnFrameRender();


		//RenderMarketingEventTimer();


		if( m_pCursor != NULL )
			m_pCursor->OnFrameRender();

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

	}
		
#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE	/// �ش� ��� CX2State�� �̵� ( ���� ���� �� �����ϴٰ� �Ǵܽ� ���� ���� ���� ���� )
#ifdef NEW_DEFENCE_DUNGEON
	//����� �� ���̵� �����, �ȳ� ���� ǥ�� �ð��� ������ �� ��� ǥ��
	if( m_fDefenceDungeonNoticeTime > 0 )
		SetDefenceDungeonChangeLevelNotice();
#endif NEW_DEFENCE_DUNGEON
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
	ProcessFadeInOutNotice();		/// ���̵� �� ~ ���̵� �ƿ� �Ǵ� ���� ó��
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	/// ������ ���� ���� �̺�Ʈ �߻��� �����Ǵ� ����
	if( m_fTogetherFestivalEarthQuakeTime > 0 )
		SetTogetherFestivalEarthQuakeNotice();
#endif TOGETHER_FESTIVAL_2012_AUTUMN

#ifdef SERV_GLOBAL_MISSION_MANAGER
	if( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
	{
		if( g_pData->GetGlobalMissionManager()->IsActive() == true )
		{
			if( g_pData->GetGlobalMissionManager()->CheckShowGlobalMissionUI() == true )
			{
				g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( true );
			}
			else
			{
				g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( false );
			}
		}
	}
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef REFORM_UI_SKILLSLOT
	if( true == g_pData->GetUIManager()->GetUISkillTree()->GetShowEquipSkillGuide() )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL )->OutTextXY(  
			512, 300, GET_STRING(STR_ID_17274), D3DXCOLOR(1,0.6f,0,1), 	CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
	}
#endif //REFORM_UI_SKILLSLOT

	DrawMovingSmallBar();

	return S_OK;
}

HRESULT CX2StateField::OnResetDevice()
{
	if( g_pTFieldGame != NULL )
		g_pTFieldGame->OnResetDevice();

	return S_OK;
}

HRESULT CX2StateField::OnLostDevice()
{
	if( g_pTFieldGame != NULL )
		g_pTFieldGame->OnLostDevice();

	return S_OK;
}

bool CX2StateField::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	if( g_pTFieldGame != NULL )
	{
		if( g_pTFieldGame->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	if( CX2StateMenu::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	switch ( uMsg ) 
	{
	case WM_LBUTTONDOWN:
		{
	#ifdef PRIORITY_EVERY_UI
			if( 0 <= m_iTalkNpcIndex )
			{
		#ifdef DIALOG_SHOW_TOGGLE
				if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
				{
					g_pTFieldGame->TalkNpc(m_iTalkNpcIndex);
					m_iTalkNpcIndex = -1;
				}
		#else //DIALOG_SHOW_TOGGLE
				g_pTFieldGame->TalkNpc(m_iTalkNpcIndex);
				m_iTalkNpcIndex = -1;
		#endif //DIALOG_SHOW_TOGGLE
			}
			return true;
	#endif //PRIORITY_EVERY_UI
		} break;
#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		} break;
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL
	}

	return false;
}

void CX2StateField::SetShowStateDLG(bool bShow)
{
	if ( bShow == true )
	{
		//if ( m_pDLGSquareTitle != NULL )
		//	m_pDLGSquareTitle->SetShowEnable( true, true );

		g_pKTDXApp->GetDGManager()->GetCamera().Move( m_vOldEyePt.x, m_vOldEyePt.y, m_vOldEyePt.z );
		g_pKTDXApp->GetDGManager()->GetCamera().LookAt( m_vOldLookAtPt.x, m_vOldLookAtPt.y, m_vOldLookAtPt.z );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->UnHideChatWindow();
			//g_pPartyMsgerUI->UnHideUserListWindow();
		}

		if( NULL != g_pTFieldGame )
		{
			g_pTFieldGame->GetWorld()->SetShowObject( true ); 
			g_pTFieldGame->GetWorld()->SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );

			g_pTFieldGame->SetShowUserUnit( true );

			g_pTFieldGame->SetEnableCameraProcess( true );
		}

		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );


		if( NULL != g_pTFieldGame )
		{
			if ( g_pTFieldGame->GetTalkBoxManager() != NULL )
			{
				g_pTFieldGame->GetTalkBoxManager()->SetShowObject( true );
			}
		}
	}
	else
	{
		//if ( m_pDLGSquareTitle != NULL )
		//	m_pDLGSquareTitle->SetShowEnable( false, false );

		m_vOldEyePt		= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
		m_vOldLookAtPt	= g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();

		g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,-5000,-1300, 0,-5000,0 );
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->HideChatWindow();			
		}

		if( NULL != g_pTFieldGame )
		{


			g_pTFieldGame->GetWorld()->SetShowObject( false ); 
			g_pTFieldGame->SetShowUserUnit( false );
			g_pTFieldGame->SetEnableCameraProcess( false );
		}

		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, false );

		if( NULL != g_pTFieldGame )
		{
			if ( g_pTFieldGame->GetTalkBoxManager() != NULL )
			{
				g_pTFieldGame->GetTalkBoxManager()->SetShowObject( false );
			}
		}
	}

	CX2GageManager::GetInstance()->SetShow( bShow );
}

void CX2StateField::SetShowCommonBG()
{
	if ( m_pCashShop != NULL && 
		m_pCashShop->GetOpen() == true )
	{
		SetOpenCommonBG( true );
	}
	else
	{
		SetOpenCommonBG( false );
	}

}

bool CX2StateField::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}

	if(	g_pTFieldGame != NULL)
	{
		if(g_pTFieldGame->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true)
			return true;
	}

	switch(wParam)
	{
	case FQ_DUNGEON_ACCEPT:
		{
			ASSERT( L"��� �� �� ���̶�� �����ߴµ�..." );
			StateLog( L"��� �� �� ���̶�� �����ߴµ�..." );
// 			D3DXVECTOR3 vLastPos = g_pData->GetLocationManager()->GetLastLinkPos(g_pTFieldGame->GetLastPos());
// 			g_pData->GetLocationManager()->SetLastPos(vLastPos);
// 			//g_pData->GetLocationManager()->SetLastPosLineIndex();
// 
// 
// 			if(m_pDLGQuery != NULL)
// 				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
// 			m_pDLGQuery = NULL;
// 
// 			Handler_EGS_CREATE_TUTORIAL_ROOM_REQ(m_iMoveLinkPos);
// 
// 			m_iMoveLinkPos = 0;
// 			if( NULL != g_pTFieldGame )
// 			{
// 				g_pTFieldGame->ResetLinkMap();
// 			}
		}
		return true;

	case FQ_DUNGEON_CANCEL:
		{
			ASSERT( L"��� �� �� ���̶�� �����ߴµ�..." );
			StateLog( L"��� �� �� ���̶�� �����ߴµ�..." );
// 			D3DXVECTOR3 vLastPos = g_pData->GetLocationManager()->GetLastLinkPos(g_pTFieldGame->GetLastPos());
// 			g_pData->GetLocationManager()->SetLastPos(vLastPos);
// 
// 			if(m_pDLGQuery != NULL)
// 				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
// 			m_pDLGQuery = NULL;
// 			
// 			if( NULL != g_pTFieldGame )
// 			{
// 				g_pTFieldGame->GetMyUnit()->SetPos(vLastPos);
// 
// 				m_iMoveLinkPos = 0;
// 				g_pTFieldGame->ResetLinkMap();
// 				g_pTFieldGame->SetEnableKeyProcess(true);			
// 			}
		}
		return true;

	case FQ_PARTY_JOIN_REQ:
		{
			if(m_pDLGJoinParty != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinParty, NULL, false );
			m_pDLGJoinParty = NULL;

			ASSERT( !L"Party join Req" );
		}
		return true;
	case FQ_PARTY_JOIN_CANCEL:
		{
			if(m_pDLGJoinParty != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinParty, NULL, false );
			m_pDLGJoinParty = NULL;
			m_TalkBoxunitUID = 0;
		} 
		return true;
#ifndef REFORM_NOVICE_GUIDE
	case GUI_EPIC_EXIT:
	case GUI_ITEM_EXIT:
		{
			if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL )
			{
				g_pTFieldGame->GetNoviceGuide()->HideGuideDlg();
			}
		}
		return true;
#endif //REFORM_NOVICE_GUIDE

#ifdef SERV_EVENT_GUIDELINE_POPUP
	case GUI_GUIDELINE_CLOSE:
		{
			m_pDLGDungeonReformGuide->SetShowEnable(false, false);
		}
		return true;
#endif //SERV_EVENT_GUIDELINE_POPUP
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	case ICCM_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			ShellExecute(NULL, L"open", GET_STRING( STR_ID_11306 ), L"", L"", SW_SHOW ); 
		}
		return true;
	case ICCM_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		}
		return true;
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	}
	
	return false;
}

bool CX2StateField::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	if( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if(	g_pTFieldGame != NULL)
	{
		if(g_pTFieldGame->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true)
			return true;
	}

	switch( wParam)
	{
	case EGS_JOIN_SQUARE_ACK:
		return Handler_EGS_JOIN_SQUARE_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_FIELD_ACK:
		return Handler_EGS_STATE_CHANGE_FIELD_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_FIELD_LOADING_COMPLETE_ACK:
		return Handler_EGS_FIELD_LOADING_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
		break;

	//case EGS_LEAVE_SECTOR_NOT:
	//	return Handler_EGS_LEAVE_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
	//case EGS_JOIN_SECTOR_NOT:
	//	return Handler_EGS_JOIN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
	//case EGS_DEL_USER_LIST_IN_SECTOR_NOT:
	//	return Handler_EGS_DEL_USER_LIST_IN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
	//case EGS_NEW_USER_LIST_IN_SECTOR_NOT:
	//	return Handler_EGS_NEW_USER_LIST_IN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CHANGE_USER_LIST_IN_SECTOR_NOT:
		return Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT:
		return Handler_EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;
		
	case EGS_REPAIR_ITEM_NOT:
	case EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT:
		{			
			return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_CREATE_TUTORIAL_ROOM_ACK:
		return Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_ROOM_ACK:
		return Handler_EGS_LEAVE_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_ROOM_NOT:
		return Handler_EGS_LEAVE_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		break;
	
	case EGS_UPDATE_PARTY_UID_NOT:
		return Handler_EGS_UPDATE_PARTY_UID_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHAR_LEVEL_UP_NOT:
		return Handler_EGS_CHAR_LEVEL_UP_NOT( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	case EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT:
		return Handler_EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

#ifdef SERV_EPAY_SYSTEM
	case EGS_EPAY_INFO_NOT:
		return Handler_EGS_EPAY_INFO_NOT( hWnd, uMsg, wParam, lParam );
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_INVISIBLE_GM
	case EGS_TOGGLE_INVISIBLE_NOT:
		return Handler_EGS_TOGGLE_INVISIBLE_NOT( hWnd, uMsg, wParam, lParam );
#endif SERV_INVISIBLE_GM
	}

	return false;
}



void CX2StateField::JoinSector( KFieldUserInfo *pKFieldUserInfo )
{
	// �� ���ͳ��� ������ �������� ��� ����
	if(NULL != g_pTFieldGame)
	{
		g_pTFieldGame->JoinFieldUnit( pKFieldUserInfo );
	}
}

void CX2StateField::LeaveSector( UidType iUnitUID )
{
	// �� ���ͳ��� ������ ������ ��� ����	
	if(NULL != g_pTFieldGame)
	{
		g_pTFieldGame->RemoveUnit( iUnitUID );
	}	
}



bool CX2StateField::Handler_EGS_STATE_CHANGE_FIELD_REQ()
{
	ASSERT( L"�ʵ忡���� �ϴ� ����!" );
// 	KEGS_STATE_CHANGE_FIELD_REQ kPacket;
// 
// 	int iMapId = 0;
// 	
// 	if(g_pData != NULL)
// 	{
// 		iMapId = g_pData->GetLocationManager()->GetMapId( );
// 	}	
// 	
// 	if(iMapId <= 0)
// 		iMapId = SEnum::VMI_RUBEN;
// 
// 	D3DXVECTOR3 vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(m_bNowMovingToBattleField);
// 
// 	kPacket.m_iMapID =  (int)iMapId;
// 	//kPacket.m_vPos.x = vStartPos.x;
// 	//kPacket.m_vPos.y = vStartPos.y;
// 	//kPacket.m_vPos.z = vStartPos.z;
// 
// 	g_pData->GetLocationManager()->SetVillage( (SEnum::VILLAGE_MAP_ID)kPacket.m_iMapID, vStartPos );
// 
// 	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
// 	g_pMain->AddServerPacket( EGS_STATE_CHANGE_FIELD_ACK );
// 
// 	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
// 	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_545 ), this );	

	return true;
}

bool CX2StateField::Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_FIELD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	g_pMain->DeleteStateChangeDLG();
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_FIELD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//g_pMain->CreateStateChangeDLG( L"���������� �̵����Դϴ�." );			

			bool bCanCreateWorld = true;
			CX2World::WORLD_ID	m_iWorldId =  CX2World::WI_NONE;
			SEnum::VILLAGE_MAP_ID m_villageId = SEnum::VMI_INVALID;
			D3DXVECTOR3 m_vPos( 0.0f, 0.0f, 0.0f );	

			// ���� ������ village�� �����Ѵ�.	
			if(g_pData != NULL && g_pData->GetLocationManager() != NULL)
			{
				g_pData->GetLocationManager()->GetVillage(m_villageId, m_vPos);
				m_iWorldId = (CX2World::WORLD_ID)g_pData->GetLocationManager()->GetWorldID(m_villageId);		
			}
			else
			{
				// ������ ������ �� ����.
				bCanCreateWorld = false;
			}

			if(m_iWorldId <= CX2World::WI_NONE || m_iWorldId >= CX2World::WI_END)
			{
				// ������ ������ �� ����
				bCanCreateWorld = false;
			}

			// ���� ����		
			if(bCanCreateWorld == true)
			{
				
				SAFE_DELETE(g_pTFieldGame);				

				g_pTFieldGame = new CX2TFieldGame(m_iWorldId);


				// stateautochanger�� �������ϵ��� �ϱ� ���� statefield���� fieldgame�� ���� �ε��Ǵ� ��쿡 ���º�ȭ �õ��� �ٽ� �ϵ��� �������ش�. ���� ��� ������������� ��������Ʈ�� ���������� ��쿡
				m_bOneTimeProcAfterContructor = false;




				if(g_pTFieldGame != NULL)
				{
					g_pTFieldGame->AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
					g_pTFieldGame->SetAddedMyUnit(true);										
				}

				//m_bNowMovingToBattleField = 0;
				
				//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, (LPARAM)m_villageId, false );
			}
#ifdef SERV_EPIC_QUEST
			//{{ 09.03.17 ���¿� : ���� �ʵ���� �ٲ� ��쿡 ����Ʈ �˸��̸� ������Ʈ���ش�.
			if(NULL != g_pData->GetUIManager())
			{
				if( g_pData->GetUIManager()->GetUIQuestNew() == NULL )
				{
					ASSERT( !"QuestNew is NULL" );		// ���� ������ �� ������ ���� ���� class.
					g_pData->GetUIManager()->CreateUIQuestNew();
				}
				g_pData->GetUIManager()->GetUIQuestNew()->UpdateNewQuestNotice();
			}
#else
			//{{ 09.03.17 ���¿� : ���� �ʵ���� �ٲ� ��쿡 ����Ʈ �˸��̸� ������Ʈ���ش�.
			if(NULL != g_pData->GetUIManager())
			{
				if( g_pData->GetUIManager()->GetUIQuestReceive() == NULL )
				{
					ASSERT( !"QuestReceive is NULL" );		// ���� ������ �� ������ ���� ���� class.
					g_pData->GetUIManager()->CreateUIQuestReceive();
				}
				g_pData->GetUIManager()->GetUIQuestReceive()->UpdateNewQuestNotice();
			}
#endif SERV_EPIC_QUEST
			SetVillageName();
		}
		else
		{
			switch ( kEvent.m_iOK )
			{
			case NetError::ERR_FIELD_05:
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING((STR_ID_18892, "i",kEvent.m_iLimitLevel)) , g_pMain->GetNowState());

				} break;
			case NetError::ERR_FIELD_06:
				{
					if( NULL != g_pData && NULL != g_pData->GetDungeonManager() )
					{
						wstring DungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( static_cast<SEnum::DUNGEON_ID>(kEvent.m_iRequireDungeonID));
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 	GET_REPLACED_STRING((STR_ID_18893, "L", DungeonName )),	g_pMain->GetNowState());
					}
				} break;
			}

			// ������ġ�� ����
			g_pData->GetLocationManager()->RestoreVillage();
			if(g_pTFieldGame != NULL)
			{
				g_pTFieldGame->SetNearPortalToBattleField( false );
			}
			//m_bNowMovingToBattleField = 0;

			if(g_pTFieldGame != NULL)
			{
				g_pTFieldGame->GetMyUnit()->SetPos(g_pData->GetLocationManager()->GetNearLoginPos(g_pTFieldGame->GetMyUnit()->GetPos()));
				g_pTFieldGame->SetEnableKeyProcess(true);
			}					
		}
	}
	

	return true;
}

bool CX2StateField::Handler_EGS_FIELD_LOADING_COMPLETE_REQ()
{
	KEGS_FIELD_LOADING_COMPLETE_REQ kPacket;
#if 0 
	int iMapId = 0;

	if(g_pData != NULL)
	{
		D3DXVECTOR3 tempPos;
		g_pData->GetLocationManager()->GetVillage(iMapId, tempPos);
	}	

	if(iMapId <= 0)
		iMapId = SEnum::VMI_RUBEN;
#endif

	D3DXVECTOR3 vStartPos = g_pTFieldGame->GetMyUnit()->GetPos();

	//kPacket.m_iMapID =  (int)iMapId;
	kPacket.m_vPos.x = vStartPos.x;
	kPacket.m_vPos.y = vStartPos.y;
	kPacket.m_vPos.z = vStartPos.z;




	


	
#ifdef REMEMBER_LOGOUT_POSITION_TEST
	if( NULL != g_pTFieldGame )
	{
		//int iLineIndex = 0;
		//g_pTFieldGame->GetWorld()->GetLineMap()->GetLandPosition( vStartPos, LINE_RADIUS, &iLineIndex );
		int iLineIndex = g_pTFieldGame->GetMyUnit()->GetLastTouchLineIndex();
		D3DXVECTOR3 vCurrPos = g_pTFieldGame->GetMyUnit()->GetPos();

		kPacket.m_ucLastTouchLineIndex = (UCHAR) iLineIndex;
		const CKTDGLineMap::LineData* pLineData = g_pTFieldGame->GetWorld()->GetLineMap()->GetLineData( iLineIndex );

		float fLastPosValue = 0.5f;
		if( NULL != pLineData )
		{
			D3DXVECTOR3 vLineVec = pLineData->endPos - pLineData->startPos;
			D3DXVECTOR3 vPosVec = vCurrPos - pLineData->startPos;

			float fLineLength = D3DXVec3Length( &vLineVec );
			float fPosLength = D3DXVec3Length( &vPosVec );

			if( 0.f != fLineLength )
			{
				fLastPosValue = fPosLength / fLineLength;
			}
		}

		kPacket.m_usLastPosValue = floatToHalf( fLastPosValue );
	}
#endif REMEMBER_LOGOUT_POSITION_TEST

	


	g_pData->GetServerProtocol()->SendPacket( EGS_FIELD_LOADING_COMPLETE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_FIELD_LOADING_COMPLETE_ACK );

	m_bSendLoadCompleteReq = true;
	m_bLoadCompleteReq = true;
	
	if(g_pTFieldGame != NULL)
		g_pTFieldGame->SetLoadComplete(true);
	
#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	CX2State * pNowState = static_cast<CX2State *> ( g_pMain->GetNowState() );
	if ( NULL != pNowState )
	{
		if ( true == g_pMain->GetIsJumpingCharacter() )
		{
			// ���� ���� �˸�â�� ����.
			pNowState->SetShowJumpingCharacterClassNoticeChangeDlg( true );
		}
	}
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

 	return true;
}


bool CX2StateField::Handler_EGS_FIELD_LOADING_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FIELD_LOADING_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_FIELD_LOADING_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_EVENT_RETURN_USER_MARK
			if( true == kEvent.m_bEventReturnUserMark )
			{
				//�Ĺ� �˾��� ��ٸ� �ߺ����� �ʵ��� ����ó��
				if( false == g_pData->GetPremiumBuffManager()->IsFirstComeBackBuffDesc() )
				{
					//�������� ���̵�� �̺�Ʈ ������ ����
					CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
					pStateMenu->OpenElliosGuide(110);
				}
			}
#endif //SERV_EVENT_RETURN_USER_MARK
			//{{ 2012. 05. 16 �ڼ��� ù ���� �� ���̵� ���� ����ֱ� 
#ifdef SERV_EVENT_GUIDELINE_POPUP 
			if( true == kEvent.m_bPopupTheGuideLine )
			{
				if( m_pDLGDungeonReformGuide == NULL )
				{
					m_pDLGDungeonReformGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_DUNGEON_FLOW_POPUP.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonReformGuide );
					m_pDLGDungeonReformGuide->SetDisableUnderWindow(true);					
				}		
				m_pDLGDungeonReformGuide->SetShow(true);
			}
#endif SERV_EVENT_GUIDELINE_POPUP 
			//}}
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
			{
				g_pData->GetQuestManager()->CompleteQuestToVisitVillage();
			}
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef QUEST_GUIDE
			if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetNpcIndicator() )
			{
				g_pTFieldGame->GetNpcIndicator()->ResetGuideTargetInfo();
			}
#endif //QUEST_GUIDE
#ifdef REFORM_NOVICE_GUIDE
			if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
			{
				g_pData->GetPlayGuide()->ResetNoviceGuide();
			}
#endif //REFORM_NOVICE_GUIDE
			m_bSendLoadCompleteReq = false;
			return true;
		}
	}

	m_bSendLoadCompleteReq = false;
	return true;
}

//bool CX2StateField::Handler_EGS_LEAVE_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//#if 0 
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_LEAVE_SECTOR_NOT kEvent;
//
//	DeSerialize( pBuff, &kEvent );		
//
//	LeaveSector(kEvent.m_iUnitUID);
//#endif
//	return true;
//}
//bool CX2StateField::Handler_EGS_JOIN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//#if 0 
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_JOIN_SECTOR_NOT kEvent;
//
//	DeSerialize( pBuff, &kEvent );		
//
//	JoinSector(&kEvent.m_kNewJoinUser);
//#endif
//	return true;
//}

//bool CX2StateField::Handler_EGS_DEL_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_DEL_USER_LIST_IN_SECTOR_NOT kEvent;
//
//	DeSerialize( pBuff, &kEvent );		
//
//	for(int i=0; i<(int)kEvent.m_vecUserList.size(); ++i)
//	{
//		LeaveSector(kEvent.m_vecUserList[i]);
//	}	
//
//	return true;
//}
//bool CX2StateField::Handler_EGS_NEW_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_NEW_USER_LIST_IN_SECTOR_NOT kEvent;
//
//	DeSerialize( pBuff, &kEvent );		
//
//	for(int i=0; i<(int)kEvent.m_vecUserList.size(); ++i)
//	{			
//		JoinSector(&kEvent.m_vecUserList[i]);
//	}	
//
//	return true;
//}

bool CX2StateField::Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_USER_LIST_IN_SECTOR_NOT kEvent;

	DeSerialize( pBuff, &kEvent );		

	for(int i=0; i<(int)kEvent.m_vecDelUserList.size(); ++i)
	{
		LeaveSector(kEvent.m_vecDelUserList[i]);
	}

	for(int i=0; i<(int)kEvent.m_vecNewUserList.size(); ++i)
	{			
		JoinSector(&kEvent.m_vecNewUserList[i]);
	}

	return true;
}

bool CX2StateField::Handler_EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT kEvent;

	DeSerialize( pBuff, &kEvent );		

	if(g_pTFieldGame != NULL)
	{
		g_pTFieldGame->ReceiveSyncBroadNot(kEvent);
	}

	return true;
}

bool CX2StateField::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pTFieldGame )
	{
		g_pTFieldGame->Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT( kEvent );
	}

	return true;
}

void CX2StateField::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ(int iDungeonId)
{
	KEGS_CREATE_TUTORIAL_ROOM_REQ kEGS_CREATE_ROOM_REQ;

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomType			= CX2Room::RT_DUNGEON;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName			= GET_STRING( STR_ID_644 );
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic			= false;	
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword		= L"a*h97lk2";
	
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot			= 1;

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_DifficultyLevel	= CX2Dungeon::DL_NORMAL;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime			= 19999.0f;

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= (SEnum::DUNGEON_ID) iDungeonId;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_cGetItemType = CX2DungeonRoom::DGIT_PERSON;

	g_pMain->SetExitFieldDungeon(false);
	g_pMain->SetPlayFieldDungeon(true);	

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_TUTORIAL_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
	g_pMain->AddServerPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, 40.f );	
}

bool CX2StateField::Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_TUTORIAL_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
	//CX2PacketLog::PrintLog( &kEvent );

	//SAFE_DELETE_DIALOG( m_pDLGCreateDungeonRoom );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CREATE_TUTORIAL_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
				vector<KRoomSlotInfo>(), kEvent.m_wstrCNIP );

			g_pData->DeleteAllRooms();
			g_pData->ResetDungeonRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// ���ο� �����Ͱ� ���� �Ǿ�����
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//�� ������ ����
			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
			pCX2DungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
			pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
			pCX2DungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
           if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM


			//������ ������Ʈ �̵�
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_645 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false );

			return true;
		}
	}

	if( true == g_pMain->GetPlayFieldDungeon() )
		g_pMain->SetPlayFieldDungeon( false );

	return false;
}

bool CX2StateField::Handler_EGS_JOIN_SQUARE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_SQUARE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_JOIN_SQUARE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//���忡 ����
			CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
			if ( NULL != pPartyManager )
				pPartyManager->RememberComeBackInfoBeforeInstanceGameStart();

			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_646 ) );

			if(g_pTFieldGame != NULL)
				g_pTFieldGame->DeleteWorld();			

			CX2SquareGame::SquareData squareData;
			squareData.Set_KSquareInfo( &kEvent.m_kSquareInfo );
			SAFE_DELETE( g_pSquareGame );
			SAFE_DELETE( g_pTFieldGame );

			switch(squareData.m_SquareType)
			{
			case CX2SquareGame::ST_TRADE:  // �ŷ�����
				{ 
					g_pSquareGame = new CX2SquareGame( &squareData, CX2World::WI_SQUARE_MARKET );
				} break;

#ifdef NEW_VILLAGE_RENDERING_TEST

			case CX2SquareGame::ST_AGE:
				{
					g_pSquareGame = new CX2SquareGame( &squareData, CX2World::WI_DUNGEON_GATE_RUBEN );
				} break;

			case CX2SquareGame::ST_FREE:
				{
					g_pSquareGame = new CX2SquareGame( &squareData, CX2World::WI_DUNGEON_GATE_ELDER );
				} break;

			case CX2SquareGame::ST_AREA:
				{
					g_pSquareGame = new CX2SquareGame( &squareData, CX2World::WI_VILLAGE_BESMA );
				} break;

#endif // NEW_VILLAGE_RENDERING_TEST

			default:
				{
					g_pSquareGame = new CX2SquareGame( &squareData, CX2World::WI_SQUARE );
				} break;

			}
			if( NULL != g_pSquareGame )
			{
				for( int i = 0; i < (int)kEvent.m_vecUserInfo.size(); i++ )
				{
					KSquareUserInfo* pKSquareUserInfo = &kEvent.m_vecUserInfo[i];
					g_pSquareGame->JoinSquareUnit( pKSquareUserInfo );
				}	
			}
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SQUARE_GAME, 0, false ); 

		}
		else
		{
			if(g_pTFieldGame != NULL)
				g_pTFieldGame->UpdateMarketList(kEvent);
		}
		return true;
	}

	return false;
}



// bool CX2StateField::Handler_EGS_PARTY_TALKBOX_INFO_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	KSerBuffer* pBuff = (KSerBuffer*)lParam;
// 	KEGS_PARTY_TALKBOX_INFO_NOT kEvent;
// 	DeSerialize( pBuff, &kEvent );
// 
// 	if( NULL != g_pTFieldGame )
// 	{
// 		g_pTFieldGame->UpdatePartyTalkBox( kEvent );
// 	}
// 	return true;
// }

bool CX2StateField::Handler_EGS_LEAVE_ROOM_REQ()
{
	//ASSERT( NULL != g_pX2Room );
	//if( NULL != g_pX2Room )
	//{
	//	g_pX2Room->Handler_EGS_LEAVE_ROOM_REQ();
	//}

//#ifdef DISABLE_REDUDANT_PACKET_TEST
//	if( true == g_pMain->IsWaitingServerPacket( EGS_LEAVE_ROOM_ACK ) )
//	{
//		return false;
//	}
//#endif DISABLE_REDUDANT_PACKET_TEST

	KEGS_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	// �ʿ� ������ �ִ� GamePlayNetworkInfo �� vector�� ��´�.
	if ( NULL != g_pX2Room )
		g_pX2Room->MoveGamePlayNetworkInfoFromMapToVector( kPacket.m_kGamePlayNetworkInfo.m_vecGamePlayNetworkInfo );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

	g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_ROOM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LEAVE_ROOM_ACK );

	return true;
}

// ��Ƽ���� ������������� ��������Ʈ�� ȥ�� �������� �� �� ��Ŷ�� �޴´�.
bool CX2StateField::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	//bool bRetVal = false;


	//if( NetError::ERR_ROOM_34 == kPacket.m_iOK )
	//{
	//	bRetVal = Handler_EGS_STATE_CHANGE_FIELD_REQ();
	//}
	//else
	//{
		if( g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK ) == true )
		{
			if( g_pMain->IsValidPacket(kPacket.m_iOK) == true )
			{

//				g_pData->GetPartyManager()->RememberComeBackInfoBefroDungeonStart();
// 				SEnum::VILLAGE_MAP_ID eDungeonLoungeID = g_pData->GetLocationManager()->GetCurrentVillageID();
// 				CX2LocationManager::LOCAL_MAP_ID eLocalID = g_pData->GetLocationManager()->GetLocalMapID( eDungeonLoungeID );
// 				SEnum::VILLAGE_MAP_ID eDungeonGateID = g_pData->GetLocationManager()->GetDungeonGateID( eLocalID );
// 
// 				ASSERT( SEnum::VMI_INVALID != eDungeonGateID );
// 				D3DXVECTOR3 vStartPos = g_pInstanceData->GetLastDungeonGatePos();
// 				g_pData->GetLocationManager()->SetVillage( eDungeonGateID, vStartPos, g_pInstanceData->GetLastDungeonGatePosLineIndex() );
// 				g_pData->GetLocationManager()->SetLastPos( vStartPos );
// 				g_pData->GetLocationManager()->SetLastPosLineIndex( g_pInstanceData->GetLastDungeonGatePosLineIndex() );

				ReturnToPlaceWhereBeforeDungeonStart();
				//CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ();
			}
		}
	return true;
}

bool CX2StateField::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	// �� ���ͳ��� ������ ������ ��� ����	
	if ( NULL != g_pTFieldGame )
	{
		g_pTFieldGame->RemoveUnit( kEvent.m_iUnitUID );
	}	
	return true;
}

bool CX2StateField::Handler_EGS_UPDATE_PARTY_UID_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_PARTY_UID_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pTFieldGame != NULL)
		g_pTFieldGame->UpdateUserPartyUid( kEvent );

	return true;
}

/*FORCEINLINE*/ bool CX2StateField::MoveFromVillageToBattleField( const int iVillageStartPosIdToBattleField_ ) const
{
	const CX2LocationManager::VillageStartPos* pVillageStartPos 
		= g_pData->GetLocationManager()->GetVillageStartPos( iVillageStartPosIdToBattleField_ );

	if ( NULL == pVillageStartPos )
	{
		ASSERT( L"DLG_Map_Village.lua is wrong!" );
		StateLog( L"DLG_Map_Village.lua isepi wrong!" );
		return false;
	}

	Handler_EGS_JOIN_BATTLE_FIELD_REQ( pVillageStartPos->m_uiBattleFieldId, pVillageStartPos->m_usBattleFieldStartLineIndex );

	return true;
}

/*virtual*/ void CX2StateField::UserAndPetListPopUpMenuProcess()
{
	D3DXVECTOR3 startRay, endRay;
	g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
		g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

	vector<pair<UidType,bool> > vTempUserList;

	BOOL bRbutton = g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_RBUTTON );// BOOL

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		bRbutton = FALSE;
#endif

#ifdef SERV_PET_SYSTEM
	if( g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->GetMyPetPick(false) == true )  // ���콺�� ���� �ִ� ���!?
	{
		if ( bRbutton == TRUE )
			vTempUserList.push_back( make_pair( g_pData->GetPetManager()->GetMyPet()->GetPetInfo().m_PetUid, true ) );
	}
#endif

	const UINT uiUnitListSize = g_pTFieldGame->GetSquareUnitNum();
	for( UINT i = 0; i < uiUnitListSize; ++i )
	{
		CX2SquareUnit* pCX2FieldUnit = g_pTFieldGame->GetSquareUnit(i);	

		if( pCX2FieldUnit != NULL && 
			//pCX2FieldUnit->GetUID() != g_pTFieldGame->GetMyUnit()->GetUID() && 
			pCX2FieldUnit->GetShowObject() == true )
		{
			D3DXVECTOR3 pos = pCX2FieldUnit->GetPos();
			pos.y += 100.0f;

			// pick.
			pCX2FieldUnit->GetUnitViewer()->SetPickUnit(false);
			if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos,	pCX2FieldUnit->GetBoundingRadius()/2.0f ) == true )
			{
				pCX2FieldUnit->GetUnitViewer()->SetPickUnit(true);

				if( bRbutton == TRUE )
				{
					if(g_pData->GetUserListPopUp() != NULL )
					{
						vTempUserList.push_back( make_pair( pCX2FieldUnit->GetUnit()->GetUID() , false  ) );
					}
				}
			}				
		}			
	}

	if ( false == vTempUserList.empty() )
	{
#ifdef SERV_PET_SYSTEM
		g_pData->GetPetManager()->ClosePetPopupMenu();
#endif
		g_pData->GetUserListPopUp()->ClosePopupMenu();
		for (UINT i=0;i<vTempUserList.size();++i)
			g_pData->GetUserListPopUp()->AddList(  vTempUserList[i].first, vTempUserList[i].second );
		g_pData->GetUserListPopUp()->OpenPopupMenu( false );
	}
}
#ifdef PLAY_EMOTION_BY_USER_SELECT
/*virtual*/void CX2StateField::PlayEmotionByUserSelect()
{
	//���콺 üũ
	if( g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_LBUTTON ) == TRUE &&
		NULL != g_pChatBox && NULL != g_pTFieldGame )
	{
		D3DXVECTOR3 startRay, endRay;
		g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
			g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

		for( int i = 0; i < g_pTFieldGame->GetSquareUnitNum(); ++i )
		{
			CX2SquareUnit* pCX2FieldUnit = g_pTFieldGame->GetSquareUnit(i);	
			if( NULL != pCX2FieldUnit && 
				true == pCX2FieldUnit->IsMyUnit())
			{
				D3DXVECTOR3 pos = pCX2FieldUnit->GetPos();
				pos.y += 100.0f;

				if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos,	pCX2FieldUnit->GetBoundingRadius()/2.0f ) == true )
				{
					int iRandVAlue = RandomInt(0,4);
					switch( iRandVAlue )
					{
					case 0 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2508 ) ); break;
					case 1 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2509 ) ); break;
					case 2 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2513 ) ); break;
					case 3 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2515 ) ); break;
					default: g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2503 ) ); break;
					}
					break;
				}
			}			
		}
	}
}
#endif // PLAY_EMOTION_BY_USER_SELECT

/*virtual*/ bool CX2StateField::Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( NULL != g_pTFieldGame )
		return g_pTFieldGame->Handler_EGS_CHAR_LEVEL_UP_NOT( hWnd, uMsg, wParam, lParam );
	else
		return false;
}

#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE	/// �ش� ��� CX2State�� �̵� ( ���� ���� �� �����ϴٰ� �Ǵܽ� ���� ���� ���� ���� )
#ifdef NEW_DEFENCE_DUNGEON
void CX2StateField::SetDefenceDungeonChangeLevelNotice()
{
	float		fTextAlpha				= 1.f;																//ǥ�õ� ���� ���� ��
	const float fShowTime				= 4.f;																//ǥ�� �� �ð�
	const int	iNowDefenceDungeonLevel =  g_pData->GetWorldMissionManager()->GetNowDefenceDungeonLevel();	//���� ����� �� ����

	if( m_fDefenceDungeonNoticeTime > fShowTime )							//5��~4�ʰ��� ���İ��� ���� Ŀ����.
	{
		fTextAlpha = 1.f - ( m_fDefenceDungeonNoticeTime - fShowTime );	//0.f ~ 1.f
	}
	else if( m_fDefenceDungeonNoticeTime < 1.f )						//1��~0�ʰ��� ���İ��� ���� �۾�����.
	{
		fTextAlpha = m_fDefenceDungeonNoticeTime;					//1.f ~ 0.f
	}

	m_pFontDefenceDungeonNotice = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL );

	switch( m_iDefenceDungeonNoticeType )
	{
	case 0:
		{
			m_pFontDefenceDungeonNotice->OutTextXY( 512, 250, GET_STRING( STR_ID_17204 ), D3DXCOLOR(1,1,0,fTextAlpha), 
													CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,fTextAlpha), NULL, DT_CENTER );
		}break;
	case 1:
		{
			m_pFontDefenceDungeonNotice->OutTextXY( 512, 250, GET_STRING( STR_ID_17207 ), D3DXCOLOR(1,1,0,fTextAlpha), 
													CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,fTextAlpha), NULL, DT_CENTER );
		}break;
	}
}
#endif NEW_DEFENCE_DUNGEON
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
void CX2StateField::SetTogetherFestivalEarthQuakeNotice()		/// ������ ���� ���� �̺�Ʈ �߻��� �����Ǵ� ����
{
	float fTextAlpha = 1.f;		/// ǥ�õ� ���� ���� ��
	float fShowTime  = 4.f;		/// ���İ� 1.f �Ǵ� �ð�

	if( m_fTogetherFestivalEarthQuakeTime > fShowTime )							/// ���İ��� ���� Ŀ����.
	{
		fTextAlpha = 1.f - ( m_fTogetherFestivalEarthQuakeTime - fShowTime );	/// 0.f ~ 1.f
	}
	else if( m_fTogetherFestivalEarthQuakeTime < 1.f )		/// ���İ��� ���� �۾�����.
	{
		fTextAlpha = m_fTogetherFestivalEarthQuakeTime;		/// 1.f ~ 0.f
	}

	m_pFontTogetherFestivalEarthQuakeNotice = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL );

	if( NULL != m_pFontTogetherFestivalEarthQuakeNotice )
		m_pFontTogetherFestivalEarthQuakeNotice->OutTextXY( 512, 250, GET_STRING( STR_ID_19109 ), D3DXCOLOR(1,1,0,fTextAlpha), 
															CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,fTextAlpha), NULL, DT_CENTER );
}
#endif TOGETHER_FESTIVAL_2012_AUTUMN

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
bool CX2StateField::MoveToOtherPlace( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
bool CX2StateField::MoveToOtherPlace()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if( true == g_pTFieldGame->IsNearPortalToBattleField() )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_TimerWaitingPortal.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_TimerWaitingPortal.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_TimerWaitingPortal.CheckAndResetElapsedTime() )
		{
			if ( MoveFromVillageToBattleField( g_pTFieldGame->GetVillageStartPosIdToBattleField() ) )
				SetNowMovingToBattleField( true );

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Portal.ogg", false, false ); 

			return true;
		}
	}
	else
	{
		m_TimerWaitingPortal.ResetSumOfElapsedTime();
	}

	return false;
}

void CX2StateField::DrawMovingSmallBar()
{	
	if ( m_TimerWaitingPortal.GetSumOfElapsedTime() > 0.0f )
	{
		D3DXVECTOR3 vPosMovingGage = g_pData->GetLocationManager()->GetStartPosLoc(g_pTFieldGame->GetVillageStartPosIdToBattleField());
		vPosMovingGage.y += 300;
		
		D3DXVECTOR3 vOut( 0.0f, 0.0f, 0.0f );
		D3DXVec3Project( &vOut, &vPosMovingGage, &g_pKTDXApp->GetViewport(), 
			&g_pKTDXApp->GetProjectionTransform(), &g_pKTDXApp->GetViewTransform(), &g_pKTDXApp->GetWorldTransform() );

		vOut.x -= ( 25 * g_pKTDXApp->GetResolutionScaleX() );
		vOut.y += ( -90 * g_pKTDXApp->GetResolutionScaleY() );

		m_TexDataMovingGageBG.pTexture->SetDeviceTexture();
		DrawFace( vOut.x+1.0f, vOut.y+1.0f, m_TexDataMovingGageBG,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		vOut.x += ( 13 * g_pKTDXApp->GetResolutionScaleX() );
		vOut.y += ( 25 * g_pKTDXApp->GetResolutionScaleY() );
		const float fNowPercent = m_TimerWaitingPortal.GetSumOfElapsedTime() / m_TimerWaitingPortal.GetTargetTime();
		m_TexDataMovingGage.pTexture->SetDeviceTexture();
		DrawFace( vOut.x, vOut.y, m_TexDataMovingGage,
			D3DCOLOR_ARGB( 255, 255, 255, 255 ), fNowPercent );
	}
}

void CX2StateField::DrawFace( const float fX_, const float fY_, const CKTDGUIControl::UITextureData& texData_, 
	D3DCOLOR color_ /* = 0xffffffff */, const float fWidthPercent_ /*= 1.0f*/  )
{
	const float fWidth_		= texData_.texSize.x * g_pKTDXApp->GetResolutionScaleX();
	const float fHeight_	= texData_.texSize.y * g_pKTDXApp->GetResolutionScaleY();


	float X1 = fX_ - 0.5f;
	float Y1 = fY_ - 0.5f;
	float X2 = fX_ + fWidth_ * fWidthPercent_;
	float Y2 = fY_ + fHeight_;

	float UMax = 1.0f;
	float VMax = 1.0f;

	DRAWFACE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(DRAWFACE_RHW_VERTEX) * 4 );

	const float& fTextureLeftTopX		= texData_.uvTexture[CKTDGUIControl::VP_LEFT_TOP].x;
	const float& fTextureLeftBottomX	= texData_.uvTexture[CKTDGUIControl::VP_LEFT_BOTTOM].x;
	const float& fTextureRightTopX		= texData_.uvTexture[CKTDGUIControl::VP_RIGHT_TOP].x;
	const float& fTextureRightBottomX	= texData_.uvTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x;
	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
	vertex[0].u = fTextureLeftTopX;   vertex[0].v = texData_.uvTexture[CKTDGUIControl::VP_LEFT_TOP].y;

	vertex[1].x = X1;     vertex[1].y = Y2;
	vertex[1].u = fTextureLeftBottomX;   vertex[1].v = texData_.uvTexture[CKTDGUIControl::VP_LEFT_BOTTOM].y;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

	vertex[2].x = X2;     vertex[2].y = Y1;
	vertex[2].u = fTextureLeftTopX + ( fTextureRightTopX - fTextureLeftTopX ) * fWidthPercent_;   vertex[2].v = texData_.uvTexture[CKTDGUIControl::VP_RIGHT_TOP].y;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].u = fTextureLeftBottomX + ( fTextureRightBottomX - fTextureLeftBottomX )* fWidthPercent_;   vertex[3].v = texData_.uvTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].y;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = color_;

	KD3DPUSH( m_RenderStateID )

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		BOOST_STATIC_ASSERT( D3DFVF_DRAWFACE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//		g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAWFACE_RHW_VERTEX) );
//#endif

	KD3DEND()
}

void CX2StateField::CreateMovingSmallBar()
{
	/// �ε������� ��׶���
	m_TexDataMovingGageBG.pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( m_TexDataMovingGageBG.texName );
	const CKTDXDeviceTexture::TEXTURE_UV* pTexUvBG = m_TexDataMovingGageBG.pTexture->GetTexUV( m_TexDataMovingGageBG.keyName );

	if ( NULL != pTexUvBG )
	{
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]	= pTexUvBG->leftTop;
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]	= pTexUvBG->rightTop;
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUvBG->leftBottom;
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUvBG->rightBottom;
		m_TexDataMovingGageBG.texSize = pTexUvBG->rectSize;

		m_TexDataMovingGageBG.SetTextureUV();
	}

	/// �ε�������
	m_TexDataMovingGage.pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( m_TexDataMovingGage.texName );
	const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = m_TexDataMovingGage.pTexture->GetTexUV( m_TexDataMovingGage.keyName );

	if ( NULL != pTexUV )
	{
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]	= pTexUV->leftTop;
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]	= pTexUV->rightTop;
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;
		m_TexDataMovingGage.texSize = pTexUV->rectSize;

		m_TexDataMovingGage.SetTextureUV();
	}
}
#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
bool CX2StateField::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL != g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFirstFrontModalDlg() )
		return true;

#ifdef CHANGE_INVENTORY_TAB_BY_WHEEL
	// �κ��丮�� ���� ���� ���� �κ��丮�� �� ó���� �ϱ�
	if( NULL != g_pData &&
		NULL != g_pData->GetUIManager() && 
		NULL != g_pData->GetUIManager()->GetUIInventory() &&
		true == g_pData->GetUIManager()->GetUIInventory()->GetIsMouseOver() )
	{
		return true;
	}
#endif // CHANGE_INVENTORY_TAB_BY_WHEEL

	short zDelta = static_cast<short>(HIWORD(wParam));
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			g_pMain->GetGameOption().CameraZoomIn( 1 );
 			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{
			g_pMain->GetGameOption().CameraZoomIn( -1 );
			m_SumDelta += WHEEL_DELTA;
		}	
	}
	return true;
}
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
bool CX2StateField::Handler_EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if (kEvent.m_bCheckAdult == false && m_bIdentityConfirmCheck == false)
	{
		g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_14496 ), ICCM_OK, 
			g_pMain->GetNowState(), ICCM_CANCEL );
	}
	else
	{
		m_bIdentityConfirmCheck = true;
	}

	return true;
}
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

#ifdef SERV_EPAY_SYSTEM
bool CX2StateField::Handler_EGS_EPAY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EPAY_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	g_pInstanceData->SetGiantZone(kEvent.m_usZone);
	g_pInstanceData->SetGiantGame(kEvent.m_usGame);

	return true;
}
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_INVISIBLE_GM
bool CX2StateField::Handler_EGS_TOGGLE_INVISIBLE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TOGGLE_INVISIBLE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pTFieldGame )
	{
		g_pTFieldGame->Handler_EGS_TOGGLE_INVISIBLE_NOT( kEvent );
	}

	return true;
}
#endif SERV_INVISIBLE_GM


#ifdef ADDED_RELATIONSHIP_SYSTEM

/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���

/** @function	: Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT
	@brief		: ��ȯ ����ڸ� ��ȯ ��û�ڰ� �ִ� ������ �̵� ��Ű�� ��Ŷ�� ó��
	@param		: �̺�Ʈ �޽���
	@return		: ��Ŷ ó�� ����
*/
bool CX2StateField::Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		bool						bCanCreateWorld		= true;
		const int					iLastTouchLineIndex	= kEvent.m_LastTouchLineIndex;
		const SEnum::VILLAGE_MAP_ID	eVillageId			= static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iMapID );
		const D3DXVECTOR3			vPos( kEvent.m_vPos.x, kEvent.m_vPos.y, kEvent.m_vPos.z );

		if ( g_pData == NULL || 
			 g_pData->GetLocationManager() == NULL||
			 eVillageId <= SEnum::VMI_INVALID ||
			 eVillageId >= SEnum::VMI_VILLAGE_MAP_END )
		{
			 bCanCreateWorld = false;		/// ������ ������ �� ����.
		}

		// ���� ����		
		if ( bCanCreateWorld == true )
		{
			SAFE_DELETE(g_pTFieldGame);				

			g_pData->GetLocationManager()->SetVillage( eVillageId, vPos, iLastTouchLineIndex );

			const CX2World::WORLD_ID eWorldID = static_cast<CX2World::WORLD_ID>( g_pData->GetLocationManager()->GetWorldID( eVillageId ) );

			g_pTFieldGame = new CX2TFieldGame( eWorldID );

			// stateautochanger�� �������ϵ��� �ϱ� ���� statefield���� fieldgame�� ���� �ε��Ǵ� ��쿡 ���º�ȭ �õ��� �ٽ� �ϵ��� �������ش�. ���� ��� ������������� ��������Ʈ�� ���������� ��쿡
			m_bOneTimeProcAfterContructor = false;

			if(g_pTFieldGame != NULL)
			{
				g_pTFieldGame->AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
				g_pTFieldGame->SetAddedMyUnit(true);										
			}
		}
		//{{ 09.03.17 ���¿� : ���� �ʵ���� �ٲ� ��쿡 ����Ʈ �˸��̸� ������Ʈ���ش�.
		if(NULL != g_pData->GetUIManager())
		{
			if( g_pData->GetUIManager()->GetUIQuestNew() == NULL )
			{
				ASSERT( !"QuestNew is NULL" );		// ���� ������ �� ������ ���� ���� class.
				g_pData->GetUIManager()->CreateUIQuestNew();
			}
			g_pData->GetUIManager()->GetUIQuestNew()->UpdateNewQuestNotice();
		}
		SetVillageName();
	}
	else
	{
		// ������ġ�� ����
		g_pData->GetLocationManager()->RestoreVillage();
		if(g_pTFieldGame != NULL)
		{
			g_pTFieldGame->SetNearPortalToBattleField( false );
		}

		if(g_pTFieldGame != NULL)
		{
			g_pTFieldGame->GetMyUnit()->SetPos(g_pData->GetLocationManager()->GetNearLoginPos(g_pTFieldGame->GetMyUnit()->GetPos()));
			g_pTFieldGame->SetEnableKeyProcess(true);
		}					
	}

	CX2UIManager* pUIManager = g_pData->GetUIManager();
	pUIManager->SetShowUI(true, true);		
	pUIManager->CloseAllNPCDlg();

	return true;
}

#endif // ADDED_RELATIONSHIP_SYSTEM 


/*virtual*/
void CX2StateField::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if ( NULL != g_pTFieldGame )
		g_pTFieldGame->PopTalkBox( iUnitUID_, pWstrMsg_, coTextColor_, coOutColor_, coBackColor_ );
}
