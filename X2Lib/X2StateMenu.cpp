#include "StdAfx.h"
#include ".\x2statemenu.h"


CX2StateMenu::CX2StateMenu(void) :
// 2009.01.09 ���¿� : �ʱ�ȭ �ڵ�����
m_pDLGMenu(NULL),
m_pDLGMenuWindow(NULL),
m_pDLGFindUser(NULL),
m_pDLGUserInfo(NULL),
m_pDLGF1Help(NULL),
m_bShowWindowMenu(false),
m_bShowSystemMenu(false),
m_pCashShop(NULL),
m_wstrFindUserName(L""),
m_FindUserUid(0),
m_pDLGInviteNot(NULL),
m_InviterName(L""),
m_eSearchUnitConnectionState(CX2Unit::CUS_NONE),
m_iSearchUnitChannelID(0),
m_iSearchUnitDungeonID(0),
m_iSearchUnitRoomUID(0),
//m_mapHideMouseClickUI;
m_fTimeRefreshMenuInfo(0.0f),
m_CashItemPage(1),
m_pDLGCashItemUpdate(NULL),
m_pDLGPresentArrival(NULL),
m_pDLGSystem(NULL),
m_bShowMenu(true),
m_pDLGLeavePartyOrLounge( NULL ),
m_pDLGGuideBookButton( NULL ),
m_pPicCharMenuPlusInfo(NULL),
m_pPicCharMenuPlusInfoRed(NULL),
m_TotalExp(0),
m_TotalED(0),
#ifdef SERV_PVP_NEW_SYSTEM
m_TotalAP(0),
#else
m_TotalVP(0),
#endif
m_TotalLevel(0)
, m_bShowMainMenuButton(true)
, m_fMenuMaxRemainTime( 6.f )
#ifdef MODIFY_DUNGEON_STAGING
, m_bShowUI ( true ) 
#endif //MODIFY_DUNGEON_STAGING
#ifdef SERV_RECOMMEND_LIST_EVENT
, m_pDLGFriendRecommendPopUp( NULL )
#endif //SERV_RECOMMEND_LIST_EVENT
#ifdef SERV_ADD_WARP_BUTTON
, m_pDLGWarpDestination( NULL )
, m_iSelectedWarpDest(0)
, m_SumDelta(0)
,m_iCurrentVillageWarpIndex(-1)
,m_iSelectedWarpDestIndex(-1)
,m_bIsEdConsumptionWarp(false)
,m_fTimeCheckWarpVip(0.f)
#endif // SERV_ADD_WARP_BUTTON
//}}
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
,m_pDLGCoboEventUI(NULL)
,m_UsedCashShop(false)
,m_iElapsedTimeStage(0)
,m_pDLGCoboEventCountUI(NULL)
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_NEW_YEAR_EVENT_2014
,m_pDLG2014Confirm( NULL )
#endif SERV_NEW_YEAR_EVENT_2014
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
,m_pDLGChungItemUI(NULL)
,m_UsedCashShop(false)
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
,m_pDLGAdamsEventShopUI(NULL)
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
#ifdef SERV_4TH_ANNIVERSARY_EVENT
, m_pDLG4thConfirm( NULL )
, m_iSelectedButtonIndex( -1 )
#endif //SERV_4TH_ANNIVERSARY_EVENT
{
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

	InitUI();
	InitETC();

	// fix!! �ϴ��� �޴��� �ִ� ������ ��ƼUI�� �׻� �ִٰ� ���� ������ ���� ����, ���� �� �� ��Ƽ UI�����ϴ� ������ ó���س���
	if( NULL == g_pMain->GetPartyUI() )
	{
		g_pMain->ResetPartyUI();
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MenuInfoPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetOverUI( true );
	m_pPicCharMenuPlusInfo = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
	m_pPicCharMenuPlusInfo->SetWidth( 10 );

	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MenuInfoPicCharRed", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetOverUI( true );
	m_pPicCharMenuPlusInfoRed = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
	m_pPicCharMenuPlusInfoRed->SetWidth( 10 );

	m_TotalExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
	m_TotalED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;	
	m_TotalLevel = g_pData->GetSelectUnitLevel();

#ifdef SERV_PVP_NEW_SYSTEM
	m_TotalAP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint;
#else
	m_TotalVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
#endif
#ifdef MODIFY_ACCEPT_QUEST // ����Ʈ �޴� Newǥ�� ����
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( NULL != g_pData && 
				NULL != g_pData->GetQuestManager() )
			{
				vector<int> vecAvailQuestID;
				g_pData->GetQuestManager()->GetAvailableQuest( CX2UnitManager::NUI_BILLBOARD, vecAvailQuestID );
				SetShowNewQuest( !vecAvailQuestID.empty() );
			}
		} break;
	default:
		break;
	}
#endif // MODIFY_ACCEPT_QUEST
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	m_tRemaindTimeDisCount = g_pData->GetServerCurrentTime64();
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
}

CX2StateMenu::~CX2StateMenu(void)
{
	//SAFE_DELETE_DIALOG( m_pDLGMenu );
	if ( m_pDLGMenu != NULL )
	{
		m_pDLGMenu->SetShow( false );
		m_pDLGMenu->SetEnable( false );
	}
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	if(m_pDLGChungItemUI != NULL)
	{
		m_pDLGChungItemUI->SetShow(false);
		m_pDLGChungItemUI = NULL;
	}
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	if(m_pDLGCoboEventUI != NULL)
	{
		m_pDLGCoboEventUI->SetShow(false);
		m_pDLGCoboEventUI = NULL;
	}
	if(m_pDLGCoboEventCountUI != NULL)
	{
		m_pDLGCoboEventCountUI->SetShow(false);
		m_pDLGCoboEventCountUI = NULL;
	}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	if(m_pDLGAdamsEventShopUI != NULL)
	{
		m_pDLGAdamsEventShopUI->SetShow(false);
		m_pDLGAdamsEventShopUI = NULL;
	}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	// 2009.01.29 ���¿� : ������ SetShow Ÿ�̹� �ٲ�
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, false);

	SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
	SAFE_DELETE_DIALOG( m_pDLGFindUser );
	SAFE_DELETE_DIALOG( m_pDLGUserInfo );

	SAFE_DELETE_DIALOG( m_pDLGF1Help );
	SAFE_DELETE_DIALOG( m_pDLGCashItemUpdate );

	SAFE_DELETE_DIALOG( m_pDLGPresentArrival );

	SAFE_DELETE_DIALOG( m_pDLGSystem );
	SAFE_DELETE_DIALOG( m_pDLGLeavePartyOrLounge );
#ifdef SERV_NEW_YEAR_EVENT_2014
	SAFE_DELETE_DIALOG( m_pDLG2014Confirm );
#endif SERV_NEW_YEAR_EVENT_2014


	CX2Community* pMessenger = g_pData->GetMessenger();
	if ( pMessenger != NULL )
		pMessenger->SetOpen( false );

#ifndef COUPON_SYSTEM // ���� UI ����
	if( NULL != g_pMain->GetCouponBox() )
	{
		g_pMain->GetCouponBox()->OpenCouponBoxDLG( false );
		g_pMain->GetCouponBox()->OpenCouponNumberInputDLG( false );
	}
#endif // COUPON_SYSTEM

	g_pMain->GetInformerManager()->CheckInfo( true );


	//{{ 09.03.18 ���¿�
	SAFE_DELETE_DIALOG(m_pDLGGuideBookButton);
	//}}
	SAFE_DELETE( m_pPicCharMenuPlusInfo );
	SAFE_DELETE( m_pPicCharMenuPlusInfoRed );
	m_vecpButtonMainMenu.clear();

#ifdef SERV_RECOMMEND_LIST_EVENT
	SAFE_DELETE_DIALOG( m_pDLGFriendRecommendPopUp );
#endif //SERV_RECOMMEND_LIST_EVENT
#ifdef SERV_ADD_WARP_BUTTON
	SAFE_DELETE_DIALOG( m_pDLGWarpDestination );
#endif // SERV_ADD_WARP_BUTTON
}

HRESULT CX2StateMenu::OnFrameMove( double fTime, float fElapsedTime )
{
	KLagCheck( eKnown_LagCheckType_StateMenu_FrameMove );
	KTDXPROFILE();
	CX2StateCommonBG::OnFrameMove( fTime, fElapsedTime );

#ifdef SERV_EVENT_CHECK_POWER
	g_pMain->GetMemoryHolder()->UpdateCheckPowerEventTimer();
#endif SERV_EVENT_CHECK_POWER

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    CheckAndSendingPlayStatus( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CheckAndSendingPlayStatus();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	ShowChungGiveItem_UI();
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
	{
		ShowCoboEventUI(fElapsedTime);
	}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	SetShowAdamsUI();
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	//{{ 09.03.18 �¿�
	// �� FrameMove�� �� ���� ������ ���µ�..
	// �׷��� �ɼǿ��� ������ �� ����� ���� �͵� �� �ָ��ϱ� �ϴ�-_-;
	if( ( (g_pTFieldGame != NULL && false == g_pTFieldGame->GetJoinNpc() ) ||
		  (g_pSquareGame != NULL ) )							// NPC�� ��ȭ���� �ƴϰ�
#ifndef NEW_SKILL_TREE_UI
		&& ( g_pInstanceData == NULL || false == g_pInstanceData->GetSkillTreeUI()->GetShow() )		// ��ųƮ���� ����������
#endif
		)
	{
		CKTDGUIStatic* pPartyOnly = (CKTDGUIStatic*)m_pDLGMenu->GetControl( L"Static_MyPartyOnly" );
		if(pPartyOnly != NULL && g_pSquareGame == NULL)
		{
			pPartyOnly->SetShow(true);
			bool bCurrentOption = g_pMain->GetGameOption().GetFieldParty();
			pPartyOnly->GetPicture(0)->SetShow(!bCurrentOption);	// off picture
			pPartyOnly->GetPicture(1)->SetShow(bCurrentOption);		// on picture			
		}
		else if(pPartyOnly != NULL && g_pSquareGame != NULL)
		{
			pPartyOnly->SetShow(false);
		}

		CKTDGUIStatic* pSDMode = (CKTDGUIStatic*)m_pDLGMenu->GetControl( L"Static_SDMode" );
		if(pSDMode != NULL)
		{
			pSDMode->SetShow(true);
			bool bCurrentOption = g_pMain->GetGameOption().GetFieldSD();
			pSDMode->GetPicture(0)->SetShow(!bCurrentOption);	// off picture
			pSDMode->GetPicture(1)->SetShow(bCurrentOption);		// on picture
		}
	}
	else
	{
		CKTDGUIControl* pPartyOnly = m_pDLGMenu->GetControl( L"Static_MyPartyOnly" );
		if(pPartyOnly != NULL)
			pPartyOnly->SetShow(false);
		CKTDGUIControl* pSDMode = m_pDLGMenu->GetControl( L"Static_SDMode" );
		if(pSDMode != NULL)
			pSDMode->SetShow(false);

#ifdef MODIFY_DUNGEON_STAGING
		if( NULL != g_pX2Game )
		{
			if( NULL != g_pData && NULL != g_pData->GetSlideShot() && true == g_pData->GetSlideShot()->IsPresentNow() )
			{
				g_pData->GetUIManager()->SetShowUI( false );
#ifdef SERV_CHINA_SPIRIT_EVENT
				g_pMain->GetMemoryHolder()->SetShowUseSpiritEvent( false );
#endif SERV_CHINA_SPIRIT_EVENT
			}
			else
			{
				g_pData->GetUIManager()->SetShowUI( m_bShowUI );//, false );
#ifdef SERV_CHINA_SPIRIT_EVENT
				g_pMain->GetMemoryHolder()->SetShowUseSpiritEvent( m_bShowUI );
#endif SERV_CHINA_SPIRIT_EVENT
			}
		}
#endif //MODIFY_DUNGEON_STAGING

	}
	//}}
	if( NULL != m_pDLGMenu && NULL != g_pMain)
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGMenu->GetControl(L"Static_ShowHelp");
		if( NULL != pStatic )
		{
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_BATTLE_FIELD:
			case CX2Main::XS_DUNGEON_GAME:
				{
					pStatic->GetPicture(0)->SetShow( true );
						pStatic->GetPicture(1)->SetShow( g_pMain->GetGameOption().GetOptionList().m_bPlayGuide );
				}break;
			default:
				{
					pStatic->GetPicture(0)->SetShow( false );
					pStatic->GetPicture(1)->SetShow( false );
				}break;
			}

		}
	/*
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGMenu->GetControl(L"Static_ShowHelp");
			if( NULL != g_pTFieldGame && true == g_pTFieldGame->GetJoinNpc() )
			{
				pStatic->GetPicture(0)->SetShow( false );
				pStatic->GetPicture(1)->SetShow( false );
			}
			else
			{
				if( NULL != pStatic && NULL != g_pMain )
				{
					pStatic->GetPicture(0)->SetShow( true );
					pStatic->GetPicture(1)->SetShow( g_pMain->GetGameOption().GetOptionList().m_bPlayGuide );
				}
			}*/
	
	}
	if ( m_pCashShop != NULL && m_pCashShop->GetOpen() == false )
	{
		SetShowDlgMenu(true);
	}
	else
	{
		SetShowDlgMenu(false);
	}

#ifdef SERV_NEW_YEAR_EVENT_2014
	if ( m_pCashShop != NULL && m_pCashShop->GetOpen() == false && m_bShowUI == true )
		g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
	else
		g_pMain->GetMemoryHolder()->GetNewYear2014EventDLG( this )->SetShow( false );
#endif SERV_NEW_YEAR_EVENT_2014


	//ChatProcess();
	//m_pCharacterRoom->OnFrameMove( fTime, fElapsedTime );
    if( m_pCashShop != NULL )
    {
	    m_pCashShop->OnFrameMove( fTime, fElapsedTime );
    }//if

	if ( g_pMain->GetLVUpEventMgr() != NULL && g_pTFieldGame != NULL)
		g_pMain->GetLVUpEventMgr()->OnFrameMove( fTime, fElapsedTime );

	//{{ kimhc // 2009-11-02 // ��� �����Ӵ� ���� �÷��� �߿��� ��µǾ� ������ ����
	// OnFrameMove ������ ���� ������ ���� ��
#ifndef	GUILD_MANAGEMENT
	if(g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME)
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-11-02 // ��� �����Ӵ� ���� �÷��� �߿��� ��µǾ� ������ ����
		g_pMain->GetInformerManager()->OnFrameMove( fTime, fElapsedTime );

// #ifdef NEW_VILLAGE_UI
// 	if(g_pData->GetUIManager() != NULL )
// 		g_pData->GetUIManager()->OnFrameMove( fTime, fElapsedTime );
// #endif
// X2State�� �̵�!

#ifdef POSTBOX
	if( g_pMain->GetPostBox() != NULL)
	{
		g_pMain->GetPostBox()->OnFrameMove( fTime, fElapsedTime );
	}	
#endif


	// ������ �޴���ư new ������ ǥ��
	if( NULL != m_pDLGMenu )
	{
		if(g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME)
		{			
			CKTDGUIStatic* pStaticInven = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Inventory_New" );
			if( NULL != pStaticInven )
			{
				if( g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL &&
					g_pData->GetUIManager()->GetUIInventory()->GetNewItem() == true)
				{
					pStaticInven->SetShowEnable( true, false );
				}
				else
				{
					pStaticInven->SetShowEnable( false, false );
				}
			}	

#ifdef NEW_SKILL_TREE
			
			CKTDGUIStatic* pStaticSkill = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Skill_New" );
			if( NULL != pStaticSkill )
			{
				if( g_pData->GetMyUser() != NULL &&
					g_pData->GetMyUser()->GetSelectUnit() != NULL )
				{
					if ( 0 < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint + g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint )
					{
						pStaticSkill->SetShowEnable( true, false );
					}
					else
					{
						pStaticSkill->SetShowEnable( false, false );
					}
				}
			}
#else NEW_SKILL_TREE
//{{AFX
			// fix!! ���߿� framemove���� �ٸ� ������ �ű���
			CKTDGUIStatic* pStaticSkill = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Skill_New" );
			if( NULL != pStaticSkill )
			{
				if( NULL != g_pData->GetSkillTree() &&
					g_pData->GetSkillTree()->GetPossibleLearnSkillNum() > 0 )
				{
					pStaticSkill->SetShowEnable( true, false );
				}
				else
				{
					pStaticSkill->SetShowEnable( false, false );
				}
			}
//}}AFX
#endif NEW_SKILL_TREE

		}
		


#ifdef TEMP_SHOW_NEW_ON_CASH_SHOP_BUTTON
		// ĳ�ü� ��ư���� New ǥ��
		CKTDGUIStatic *pStaticShop = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Cash_New" );
		if( NULL != pStaticShop )
		{
			pStaticShop->SetShowEnable( true, false );
		}
#endif TEMP_SHOW_NEW_ON_CASH_SHOP_BUTTON

#ifdef SERV_ADD_WARP_BUTTON
		m_fTimeCheckWarpVip -= fElapsedTime;
		if( m_fTimeCheckWarpVip <= 0.f )
		{
			CKTDGUIStatic *pStaticWarpVip = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Warp_VIP" );
			//vip �� ��� �Ͽ����� ���. ��ư�� ��ũ���ǿ� �ð��� ǥ���Ѵ�. 
			if( pStaticWarpVip != NULL )
			{
				// 1�еڿ� �ٽ� üũ
				m_fTimeCheckWarpVip = 60.f;

				if( g_pData->GetMyUser() != NULL &&
					g_pData->GetMyUser()->GetSelectUnit() != NULL )
				{
					CX2Unit::UnitData& kUnitData = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
					__int64 trWarpVipEndDate = kUnitData.m_trWarpVipEndDate;

					CKTDGUIButton *pButtonWarp = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl( L"Warp" ));
					if( trWarpVipEndDate > g_pData->GetServerCurrentTime64() )
					{
						// VIP �Ⱓ�̴� ������ ǥ��������
						pStaticWarpVip->SetShowEnable( true, false );
						// VIP �Ⱓ�� ���� ���õ� ������
						kUnitData.m_bWarpVip = true;

						// �Ⱓ�� ǥ��������						
						if( NULL != pButtonWarp	)
						{
							wstring wstrRwstrRemainTime = GetExpirationDateDesc( trWarpVipEndDate );
							wstrRwstrRemainTime += GET_REPLACED_STRING( ( STR_ID_24618, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_WARP ) ) );
							SetGuideString( pButtonWarp, wstrRwstrRemainTime.c_str() );
						}						
					}
					else
					{
						pStaticWarpVip->SetShowEnable( false, false );
						kUnitData.m_bWarpVip = false;

						// �Ⱓ�� ��������
						if( NULL != pButtonWarp )
						{
							wstring wstrRwstrRemainTime =GET_REPLACED_STRING( ( STR_ID_24618, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_WARP ) ) );
							SetGuideString( pButtonWarp, wstrRwstrRemainTime.c_str() );
						}						
					}
				}
			}
		}
#endif //SERV_ADD_WARP_BUTTON

#ifdef SHOW_DISCOUNT_TAG
		// ���ϴ� ���⼭ ���� üũ ����
		// 2013.12.17 darkstarbt_������ // ĳ�� �̺�Ʈ ������ 1�д����� ������ üũ �ϴ������� �ǽð� üũ�� ����. �̺�Ʈ�� ����ǰ� 1�еڿ� ���� �±װ� ������ �ִ�.
		CKTDGUIStatic *pStaticDiscountTag = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"DiscountTag" );
		if( NULL != pStaticDiscountTag )
		{
			IF_EVENT_ENABLED( CEI_CHRISTMAS_DISCOUNT_EVENT_2013 )
			{
				switch(g_pMain->GetNowStateID())
				{
				case CX2Main::XS_PVP_LOBBY:
				case CX2Main::XS_PVP_ROOM:
				case CX2Main::XS_PVP_GAME:
					pStaticDiscountTag->SetShowEnable(false, false);
					break;
				default:
					pStaticDiscountTag->SetShowEnable(true, true);
					break;
				}
			}
			ELSE
			{
				pStaticDiscountTag->SetShowEnable(false, false);
			}
		}
#endif //SHOW_DISCOUNT_TAG

#ifdef SERV_PET_SYSTEM
		bool bMark = false;

		// �� ��ȭ���� �˸� ��ư
		CKTDGUIStatic *pStaticPet = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Pat_Inventory_Evolution" );
		CKTDGUIStatic *pStaticPetNew = (CKTDGUIStatic*) m_pDLGMenu->GetControl( L"Pat_Inventory_New" );
		if( pStaticPetNew != NULL )
			pStaticPetNew->SetShowEnable( false, false );

		if( NULL != pStaticPet )
		{
			if( g_pData != NULL && g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->CanMyPetEvolution() == true )
			{
				pStaticPet->SetShowEnable( true, false );
				bMark = true;
			}
			else
				pStaticPet->SetShowEnable( false, false );
		}

		if( bMark == false )
		{			
			if( NULL != pStaticPetNew )
			{
				if( g_pData != NULL && g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->GetNerverSummonPet() == true )
				{
					pStaticPetNew->SetShowEnable( true, false );			
				}
				else
					pStaticPetNew->SetShowEnable( false, false );
			}
		}		
#endif
#ifdef SERV_ELESIS_UPDATE_EVENT
		if( g_pData->GetMyUser() != NULL &&
			g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			CKTDGUIButton* pButtonElesisEvent = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl( L"ElesisEventButton" ));
			if( NULL != pButtonElesisEvent )
			{
				if( ( g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD || g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME || g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ) &&
					g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().GetNoteViewCount() > 0 &&
					g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level > 2 && 
					g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level < 20 &&
					CX2Unit::UT_ELESIS == g_pData->GetMyUser()->GetSelectUnit()->GetType() &&
					//g_pData->GetPartyManager()->GetProcessDungeonMatch() &&
					GetWaitGameStart() == false )
				{
					pButtonElesisEvent->SetShowEnable( true, true );
				}
				else
				{
					pButtonElesisEvent->SetShowEnable( false, false );
				}
			}
		}
#endif SERV_ELESIS_UPDATE_EVENT
	}

#ifndef COUPON_SYSTEM // ���� UI ����
	if( g_pMain->GetCouponBox() != NULL )
		g_pMain->GetCouponBox()->OnFrameMove( fTime, fElapsedTime );
#endif // COUPON_SYSTEM

	// ������ // �޽��� �����ӹ��� 2�� ���� �ϴ� �Ϳ� ���� ����ó���� NULL == g_pX2Game ���� �߰�
	// StateMenu�� ��ӹ��� �ʴ� Game���� �����ϴ°��� ����Ͽ� ���� ��ü�� ���� ����ó�� ��.
	if ( g_pData->GetMessenger() != NULL && NULL == g_pX2Game)
		g_pData->GetMessenger()->OnFrameMove( fTime, fElapsedTime );
	//��Ȳ�� ���� �޴� ��ư ���� ����
	UpdateMenuPosistion();

	m_fTimeRefreshMenuInfo += fElapsedTime;
	if ( m_fTimeRefreshMenuInfo > 0.1f )
	{
		m_fTimeRefreshMenuInfo = 0.0f;
		RefreshMenuInfo();
	}

	if ( m_pCashShop != NULL && m_pCashShop->GetOpen() == false && m_pCashShop->GetAutoOpen() == true )
	{
		m_pCashShop->SetAutoOpen( false );
		ToggleCashShop();
	}

	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_TRAINING_SCHOOL:
	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_DUNGEON_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			CheckMyInfoChange();
		} break;
	default:
		{

		} break;		
	}	
		
	CheckPresentArrival();
	
	//���콺 ���� ���� ���� ���θ޴� �����ֵ��� ����
	m_fMenuMaxRemainTime -= fElapsedTime;
	if( true == m_bShowMainMenuButton )
	{
		if( m_fMenuMaxRemainTime < 0.3f)
		{
			m_bShowMainMenuButton = false;
		}
		vector<CKTDGUIButton*>::iterator it = m_vecpButtonMainMenu.begin();
		for( ; it!= m_vecpButtonMainMenu.end(); it++)
		{
			if( NULL != (*it))
			{
				(*it)->SetColor( D3DXCOLOR(1,1,1,m_fMenuMaxRemainTime ));
			}
		}
	}
	//x2game�� �����Ǿ� ���� ���� x2game���� ��Ÿ�� UI ����
	if ( NULL == g_pX2Game )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateSkillCoolTime(fElapsedTime);
		CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;
		refUserSkillTree.OnFrameMove(fTime, fElapsedTime);
	}
	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
	{
		g_pData->GetPlayGuide()->OnFrameMove(fTime, fElapsedTime);
	}

#ifdef COME_BACK_REWARD
	if ( NULL != g_pData->GetPremiumBuffManager() )
	{
		if ( true == g_pData->GetPremiumBuffManager()->IsFirstComeBackBuffDesc() )
		{
			g_pData->GetPremiumBuffManager()->SetShowFirstComBackUI( true );
		}
	}
#endif

	return S_OK;
}

HRESULT CX2StateMenu::OnFrameRender()
{
	CX2State::OnFrameRender();

	return S_OK;
}



bool CX2StateMenu::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{


#ifdef POSTBOX
	//if( g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->MsgProc( hWnd, uMsg, wParam, lParam ) == true)
	//{
	//	return true;
	//}
#endif

#ifdef NEW_VILLAGE_UI
	// ����! Itemslot �巡���� �ʿ��� UI��� UIManager�� MsgProc�� ���� ���� ���� MsgProc�� ���� ��! (ex : postbox )
	// UIManager�� MsgProc���� ItemSlot�� �巡�� ���� ó���� ��
	// MouseUp Ÿ�ֿ̹� 
	//if ( g_pData->GetUIManager() != NULL )
	//{
	//	g_pData->GetUIManager()->MsgProc( hWnd, uMsg, wParam, lParam);
	//}
	// X2State�� �̵�!
#endif

#ifdef SERV_ADD_WARP_BUTTON
	// ���� �� ó���� �ٸ� �͵麸�� ���� ó�� �մϴ�.
	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{ 
			if( m_pDLGWarpDestination != NULL && m_pDLGWarpDestination->GetShow() && m_pDLGWarpDestination->GetIsMouseOver() ) 
			{
				CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGWarpDestination->GetControl( L"WarpDestControlList" );
				if( pControlList == NULL )
					return false;
				CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
				if(pScroolBarY == NULL)
					return false;

				short zDelta = (short)HIWORD(wParam);
				m_SumDelta += zDelta;

				while (abs(m_SumDelta) >= WHEEL_DELTA)
				{
					if(m_SumDelta>0)
					{
						pScroolBarY->Scroll(-1);
						m_SumDelta -= WHEEL_DELTA;
					}
					else
					{	
						pScroolBarY->Scroll(1);
						m_SumDelta += WHEEL_DELTA;
					}
				}
				return true;
			}
		}break;
	}
#endif //SERV_ADD_WARP_BUTTON

	bool bRetVal = false;

	bRetVal = CX2State::MsgProc( hWnd, uMsg, wParam, lParam );

	if ( bRetVal == true )
	{
		PopUpMenuClose( hWnd, uMsg, wParam, lParam );
		return true;
	}	

	if ( uMsg == WM_LBUTTONDOWN )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMenuWindow, NULL, false );
		m_pDLGMenuWindow = NULL;
		m_bShowWindowMenu = false;
	}	
	if ( uMsg == WM_MOUSEMOVE )
		ShowMainMenuButton();

	if ( m_pCashShop != NULL && 
		m_pCashShop->GetOpen() == true &&
		m_pCashShop->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}

#ifndef COUPON_SYSTEM // ���� UI ����
	if( NULL != g_pMain->GetCouponBox() &&
		true == g_pMain->GetCouponBox()->MsgProc( hWnd, uMsg, wParam, lParam ) )
	{
		return true;
	}
#endif // COUPON_SYSTEM

	if ( g_pMain->GetInformerManager() != NULL && g_pMain->GetInformerManager()->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if( true == m_Book.GetShow() &&
		true == m_Book.MsgProc( hWnd, uMsg, wParam, lParam ) )
	{
		return true;
	}

	return false;
}

void CX2StateMenu::OnResetStateAndEquip()
{
	if ( m_pCashShop != NULL )
    {
		m_pCashShop->ResetUnitViewerEquip();
    }//if
}


bool CX2StateMenu::Handler_EGS_SEARCH_UNIT_REQ( const WCHAR* wszNickName )
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
}


bool CX2StateMenu::Handler_EGS_SEARCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
			g_pData->GetMessenger()->OpenSearchFriendResultWindow( kEvent );

			/*
			// user ������ ã�Ҵ� 
			m_wstrFindUserName = kEvent.m_kCUnitInfo.m_wstrCharName;
			m_FindUserUid = kEvent.m_kCUnitInfo.m_iUnitUID;

			//m_iSearchUnitChannelID = (int) kEvent.m_kCUnitInfo.m_uiChannelID;			// fix!!

			m_iSearchUnitDungeonID = (int) kEvent.m_kCUnitInfo.m_iDungeonID;
			m_iSearchUnitRoomUID = kEvent.m_kCUnitInfo.m_iRoomUID;


			// fix!!
			//m_eSearchUnitConnectionState = (CX2Unit::CONNECT_UNIT_STATE) kEvent.m_kCUnitInfo.m_cConnectionState;


			SAFE_DELETE_DIALOG( m_pDLGFindUser );
			SAFE_DELETE_DIALOG( m_pDLGUserInfo );
			m_pDLGUserInfo = new CKTDGUIDialog( this, L"DLG_User_Info.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUserInfo );

			UpdateUserInfoUI(kEvent);			
			*/
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


bool CX2StateMenu::Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( const WCHAR* wszNickName )
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


bool CX2StateMenu::Handler_EGS_GET_CONNECTION_UNIT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_CONNECTION_UNIT_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// user ������ ã�Ҵ� 
			m_wstrFindUserName = kEvent.m_kCUnitInfo.m_wstrCharName;
			m_FindUserUid = kEvent.m_kCUnitInfo.m_iUnitUID;

			//m_iSearchUnitChannelID = (int) kEvent.m_kCUnitInfo.m_uiChannelID;			// fix!!

			m_iSearchUnitDungeonID = (int) kEvent.m_kCUnitInfo.m_iDungeonID;
			m_iSearchUnitRoomUID = kEvent.m_kCUnitInfo.m_iRoomUID;


			// fix!!
			//m_eSearchUnitConnectionState = (CX2Unit::CONNECT_UNIT_STATE) kEvent.m_kCUnitInfo.m_cConnectionState;


			SAFE_DELETE_DIALOG( m_pDLGFindUser );
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


bool CX2StateMenu::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateOption::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef BUFF_TEMPLET_SYSTEM	
#ifdef COME_BACK_REWARD
	if ( NULL != g_pData->GetPremiumBuffManager() )
	{
		if ( true == g_pData->GetPremiumBuffManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif COME_BACK_REWARD
#endif BUFF_TEMPLET_SYSTEM

	if ( NULL != g_pMain->GetKeyPad() )
	{
		if ( true == g_pMain->GetKeyPad()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}

	if(m_Book.UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
	{
		return true;
	}

#ifdef NEW_VILLAGE_UI
	if(g_pData != NULL && g_pData->GetUserMenu() != NULL)
	{
		if( g_pData->GetUserMenu()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}

// X2State�� �̵�
// 	if(g_pData != NULL && g_pData->GetUIManager() != NULL)
// 	{
// 		if( g_pData->GetUIManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
// 			return true;
// 	}
#endif


#ifndef COUPON_SYSTEM // ���� UI ����
	if( NULL != g_pMain->GetCouponBox() )
	{
		if( true == g_pMain->GetCouponBox()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif // COUPON_SYSTEM


#ifdef POSTBOX
    if( NULL != g_pMain->GetPostBox() )
    {
        if( true == g_pMain->GetPostBox()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
            return true;
    }
#endif

#ifdef SERV_SECOND_SECURITY
	if( NULL != g_pMain->GetSecurityPad() )
	{
		if( true == g_pMain->GetSecurityPad()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif SERV_SECOND_SECURITY



	//if ( m_pCharacterRoom != NULL && m_pCharacterRoom->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
	//	return true;
	if ( m_pCashShop != NULL && m_pCashShop->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;



	switch(wParam)
	{
	case SMUCM_BACK:
		{
		}
		break;

	case SMUCM_STATE_CHANGE_UNIT_SELECT:
		{
			return ProcessStateChangeUnitSelectWithOptionWindow();
		}
		break;


	case SMUCM_STATE_CHANGE_PVP_LOBBY:
		{
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_PVP_LOBBY:
			case CX2Main::XS_PVP_ROOM:
			case CX2Main::XS_DUNGEON_ROOM:
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_SQUARE_GAME:
			case CX2Main::XS_TRAINING_SCHOOL:
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
				{
					if( NULL != g_pData->GetMyUser() &&
						g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV &&
						g_pData->GetSelectUnitLevel() < 3 )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_657 ), this );
						return true;
					}

					CX2StateAutoChanger::TARGET_DETAIL targetDetail;

					g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_PVP_LOBBY, targetDetail );	

				} break;
			}

			return true;
		}
		break;

	case SMUCM_STATE_CHANGE_VILLAGE:
		{
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_PVP_LOBBY:
			case CX2Main::XS_PVP_ROOM:
			case CX2Main::XS_DUNGEON_ROOM:
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_SQUARE_GAME:
			case CX2Main::XS_TRAINING_SCHOOL:
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
				{
					CX2StateAutoChanger::TARGET_DETAIL targetDetail;
					g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_VILLAGE_MAP, targetDetail );	

				} break;
			}

			return true;
		} 
		break;

	case SMUCM_MENU:
		{
			//return ToggleSystemMenu();
			//return ToggleLobbyMenuWindow();
		}
		break;

	case SMUCM_MY_INFO:
		{
			if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
				return true;

#ifdef NEW_VILLAGE_UI
			g_pMain->GetInformerManager()->GetInvenInformer()->InvalidInfo();
            if(g_pData->GetUIManager() != NULL)
                return g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_CHARINFO);
#else
			g_pMain->GetInformerManager()->GetInvenInformer()->InvalidInfo();
			//return ToggleMyInfo( CX2CharacterRoom::CRS_INVENTORY );
#endif
		}
		break;
		// 2008.11.17 ���¿�
#ifdef NEW_VILLAGE_UI
	case SMUCM_COMMUNITY:
		{
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
			if(g_pInstanceData->GetAdamsEventShopUIShow() == true )
			{
				return false;
			}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
			// Ŀ�´�Ƽ â
			if(g_pData->GetUIManager() != NULL)
				return g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_COMMUNITY);
		}
		break;

	case SMUCM_INVENTORY:
		{
			if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
				return true;

 			if(g_pData->GetUIManager() != NULL)
 				return g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN);
		}
		break;

	case SMUCM_SYSTEM:
		{
			// ���� : ���� ���������� �ý��� �Ŵ� �� ������
#ifdef SERV_SHARING_BANK_TEST
			if ( g_pData->GetUIManager() != NULL &&													
				g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PRIVATE_BANK ) == true )
			{
			}
			else
			{
				return ToggleSystemMenu(true);
			}
#else // SERV_SHARING_BANK_TEST
			return ToggleSystemMenu(true);
#endif // SERV_SHARING_BANK_TEST
		}

#endif
        
	case SMUCM_VIEW_STUDENT_CANDIDATE_LIST:
		{
			// ü�� ���̵� ���� 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			if( g_pData->GetSelectUnitLevel() >= 20 )
			{
				if( NULL != g_pMain->GetTutorSystem() )
				{
					wstring wstrNickName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
					g_pMain->GetTutorSystem()->OpenStudentCandidateList( true );
					g_pMain->GetTutorSystem()->UpdateStudentCandidateListPage();
					g_pMain->GetTutorSystem()->SetLastViewDateToday( wstrNickName );
				}
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_658 ), this );
			}
			return true;
		}
		break;

	case SMUCM_CASH_SHOP:
		{

#ifdef SERV_PVP_NEW_SYSTEM
			if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_13599 ), this );
				return true;
			}
#endif

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
			if(g_pTFieldGame != NULL)
			{
				g_pTFieldGame->CloseFieldName(0.f);
#ifdef BUFF_NAME_CASH_SHOP_BUG_FIX
				g_pTFieldGame->CloseBuffNameForce();
#endif // BUFF_NAME_CASH_SHOP_BUG_FIX
			}
#ifndef NEW_SKILL_TREE_UI
			if( NULL != g_pInstanceData->GetSkillTreeUI() &&
				true == g_pInstanceData->GetSkillTreeUI()->GetShow() )
			{
				g_pInstanceData->GetSkillTreeUI()->SetShow( false );
			}
#endif
			ToggleCashShop();
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP
			return true;
		} break;

	case SMUCM_OPTION:
		{
			//ToggleSystemMenu();
			ToggleSystemMenu(false);
			if(GetOptionWindowOpen() == true)
				return CloseOptionWindow();
			else
				return OpenOptionWindow();
		} 
		break;
#ifdef SERV_SECOND_SECURITY
	case SMUCM_SECURITY:
		{			
			ToggleSystemMenu(false);
			if(GetOptionWindowOpen() == true)
				return CloseOptionWindow();
			else
				return OpenSecurityOptionWindow();
		}
		break;
#endif SERV_SECOND_SECURITY

	case SMUCM_TUTORIAL:
		{
			ASSERT( !"invalid code block" );
			// fix!! �� �ڵ� ��� �ȵǴ� �� ��




			// ���� �ε�ȭ�� ����
			CKTDGUIDialogType pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua", 0.0f );
			const int nLoadingBG = pDLGLoadingState->GetDummyInt(0);
			int iRandBG = rand()%nLoadingBG;
			g_pMain->SetTutorialRandomLoadingBG( iRandBG );
			SAFE_DELETE_DIALOG( pDLGLoadingState );


			g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
			switch( g_pMain->GetNowStateID() ) 
			{
				// fix!! ���࿡ �� �ڵ� ���ȴٸ� ���º��� tutorial �����ϴ� �ڵ� ������ ��
			case CX2Main::XS_PVP_LOBBY:
				{
					CX2StatePvPLobby* pStatePVPLobby = (CX2StatePvPLobby*)g_pMain->GetNowState();
					return pStatePVPLobby->EnterTutorial();
				} break;

			case CX2Main::XS_PVP_ROOM:
				{
					CX2StatePVPRoom* pStatePVPRoom = (CX2StatePVPRoom*)g_pMain->GetNowState();
					return pStatePVPRoom->EnterTutorial();
				} break;
			}
		}
		break;

	case SMUCM_MY_INFO_MY_STATE:		// fix!! 
		{
			//return m_pCharacterRoom->SetCharacterRoomState( CX2CharacterRoom::CRS_MY_STATE );
		}
		break;

	case SMUCM_MY_INFO_INVENTORY:		// fix!! 
		{
			//return m_pCharacterRoom->SetCharacterRoomState( CX2CharacterRoom::CRS_INVENTORY );
		}
		break;

	case SMUCM_MY_INFO_QUEST:
		{
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
			if(g_pInstanceData->GetAdamsEventShopUIShow() == true )
			{
				return false;
			}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
#ifdef SERV_EPIC_QUEST
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUEST_NEW);
			return true;
#else
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUEST);
			return true;
#endif SERV_EPIC_QUEST
		}
		break;

	case SMUCM_MY_INFO_SKILL_TREE:
		{
			if( false == static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() )
				return true;

			OnClickedMenuSkillButton();
			return true;
		} break;
		
	case SMUCM_STATE_CHANGE_SERVER_SELECT:
		{
			return ProcessStateChangeServerSelectWithOptionWindow();
		}
		break;

	case SMUCM_PARTY:
		{
			// 08.12.14 UIManager ����
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PARTY);
// 			if( NULL != g_pMain->GetPartyUI() )
// 			{
// 				g_pMain->GetPartyUI()->OpenPartyDLG( true );
// 				g_pMain->GetPartyUI()->UpdatePartyDLG();
// 			}

			return true;

			//if ( g_pPartyMsgerUI->GetOpenUserListWindow() == false )
			//	return g_pPartyMsgerUI->Handler_CLICKED_PARTY_BUTTON();
			//else
			//{
			//	g_pPartyMsgerUI->CloseUserListWindow();
			//	//g_pPartyMsgerUI->CloseChatWindow();
			//	return true;
			//}
		}
		break;

	case SMUCM_FIND_USER:
		{
			OpenFindUserDLG();

			return true;
		}
		break;

	case SMUCM_FIND_USER_OK:
		{
			if( NULL != m_pDLGFindUser )
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGFindUser->GetControl( L"IMEEditBox_NickName" );
				if( NULL != pIMEEditBox )
				{
					m_wstrFindUserName = pIMEEditBox->GetText();
					if( m_wstrFindUserName.length() > 0 )
					{
						const int MAGIC_MAX_NICKNAME_LENGTH = 20;
						m_wstrFindUserName = m_wstrFindUserName.substr( 0, MAGIC_MAX_NICKNAME_LENGTH );

						Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( m_wstrFindUserName.c_str() );
						return true;
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_660 ), this );
					}
				}
			}
			return true;
		} 
		break;

	case SMUCM_FIND_USER_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGFindUser, NULL, false );
			m_pDLGFindUser = NULL;

			//{{ 2007. 11. 13  ������  �Ⱦ��� �������
			//g_pData->GetServerProtocol()->SendID( EGS_CLOSE_WND_FINDUSER_NOT );
			//}}
			return true;
		} 
		break;

	case SMUCM_USER_INFO_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUserInfo, NULL, false );
			m_pDLGUserInfo = NULL;

			return true;
		}
		break;

	case SMUCM_USER_INFO_PLAY_TOGETHER:
		{
			// �ش� user�� �ִ� ������ �̵��ؾ� �Ѵ�
			CX2StateAutoChanger::TARGET_DETAIL targetDetail;
			targetDetail.m_iChannelID = m_iSearchUnitChannelID;
			targetDetail.m_iDungeonID = m_iSearchUnitDungeonID;
			targetDetail.m_iRoomUID = m_iSearchUnitRoomUID;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUserInfo, NULL, false );
			m_pDLGUserInfo = NULL;

			return true;
		}
		break;

	case SMUCM_USER_INFO_REFRESH:
		{
			Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( m_wstrFindUserName.c_str() );

		} break;


#ifndef NEW_MESSENGER
	case SMUCM_INVITE_NOT_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGInviteNot, NULL, false );
			m_pDLGInviteNot = NULL;
			return g_pData->GetMessenger()->Handler_EGS_KNM_TOGETHER_REQ( m_InviterName );
		}
		break;
#endif
	case SMUCM_INVITE_NOT_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGInviteNot, NULL, false );
			m_pDLGInviteNot = NULL;
		}
		break;

	case SMUCM_FIND_USER_SELECT:
		{
			if ( m_pDLGFindUser != NULL )
			{
				CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGFindUser->GetControl( L"ComboBoxUser_Search_Window" );
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGFindUser->GetControl(L"IMEEditBox_NickName");
				if ( pComboBox->GetSelectedItem() != NULL )
				{
					pIMEEditBox->SetText( pComboBox->GetSelectedItem()->strText );
				}
			}
		}
		break;

	//case SMUCM_SELL_ITEM_NUM_CHANGE:
	//	{
	//		CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
	//		m_pCharacterRoom->SellItemNumChange( pControl->GetDummyInt( 0 ) );
	//	}
	//	break;

	//case SMUCM_SELL_ITEM_NUM_OK:
	//	{
	//		m_pCharacterRoom->DecideSellItemNum( true );
	//		return true;
	//	}
	//	break;

	//case SMUCM_SELL_ITEM_NUM_CANCEL:
	//	{
	//		m_pCharacterRoom->DecideSellItemNum( false );
	//		return true;
	//	}
	//	break;

	case SMUCM_CHANGE_SELL_MOUSE_CURSOR:
		{
#ifdef NEW_VILLAGE_UI
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
#endif
			m_pCursor->ChangeCursorState( CX2Cursor::XCS_SELL );
		}
		break;

	case SMUCM_CHANGE_REPAIR_MOUSE_CURSOR:
		{
#ifdef NEW_VILLAGE_UI
			//g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
			//g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_CHARINFO, true);

#endif
			m_pCursor->ChangeCursorState( CX2Cursor::XCS_REPAIR );
		}
		break;

	case SMUCM_COUPON_BOX:
		{
#ifndef COUPON_SYSTEM // ���� UI ����
			if( NULL != g_pMain->GetCouponBox() )
			{
				g_pMain->GetCouponBox()->OpenCouponBoxDLG( true );
			}

			return true;
#endif // COUPON_SYSTEM
		}
		break;

#ifndef COUPON_SYSTEM // ���� UI ����
	case SMUCM_COUPON_INPUT_BOX:
		{
			// ü�� ���̵� ���� 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			if( NULL != g_pMain->GetCouponBox() )
			{
				g_pMain->GetCouponBox()->OpenCouponNumberInputDLG( true );
			}

			return true;
		}
		break;
#endif // COUPON_SYSTEM

	case SMUCM_CHANGE_DECOMPOSITION_MOUSE_CURSOR:
		{
			m_pCursor->ChangeCursorState( CX2Cursor::XCS_DECOMPOSITION );
		}
		break;

	case SMUCM_CHANGE_ENCHANT_MOUSE_CURSOR:
		{
#ifdef NEW_VILLAGE_UI
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
			g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );
#endif
			m_pCursor->ChangeCursorState( CX2Cursor::XCS_ENCHANT );
		}
		break;

	case SMUCM_CHANGE_SOCKET_MOUSE_CURSOR:
		{ 
			if( g_pData->GetUIManager()->GetUIInventory()->GetSortType() != CX2Inventory::ST_AVARTA )
				g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );
			m_pCursor->ChangeCursorState( CX2Cursor::XCS_SOCKET );
			//{{ �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
#ifdef PRINT_INGAMEINFO_TO_EXCEL
			if(g_pMain->IsInGameInfoToExcel())
			{
				g_pData->GetSocketItem()->PrintOptionInfo_ToExcel( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level );
			}
#endif PRINT_INGAMEINFO_TO_EXCEL
			//}} �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
		}
		break;

	case SMUCM_CHANGE_ATTRIB_ENCHANT_MOUSE_CURSOR:
		{
#ifdef NEW_VILLAGE_UI
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
			g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );				
#endif
			m_pCursor->ChangeCursorState( CX2Cursor::XCS_ATTRIB_ENCHANT );
		}
		break;

	case SMUCM_CHANGE_EXCHANGE_ITEM_MOUSE_CURSOR:
		{
			//{{ dmlee 2008.12.2 �������� ��ȯ �̺�Ʈ�Ⱓ ����� ��ɻ���
			//m_pCursor->ChangeCursorState( CX2Cursor::XCS_ITEM_EXCHANGE );
			return true;
			//}} dmlee 2008.12.2 �������� ��ȯ �̺�Ʈ�Ⱓ ����� ��ɻ���
		} break;

	case SMUCM_USER_INFO_FRIEND_REQ:
		{
			// ü�� ���̵� ���� 
			if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
				return true;
			}

			if ( g_pData->GetMessenger() != NULL )
			{
#ifdef NEW_MESSENGER
				wstring addFriendMessage = GET_STRING( STR_ID_74 );
				return g_pData->GetMessenger()->Handler_EGS_REQUEST_FRIEND_REQ( m_wstrFindUserName.c_str(), addFriendMessage );
#else NEW_MESSENGER
				return g_pData->GetMessenger()->Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( m_wstrFindUserName.c_str() );
#endif NEW_MESSENGER
			}
		}
		break;

	//case SMUCM_USER_INFO_PARTY_INVITE:
	//	{
	//		if ( g_pPartyMsgerUI != NULL )
	//			return g_pPartyMsgerUI->Handler_EGS_INVITE_PARTY_REQ( m_FindUserUid, m_wstrFindUserName.c_str(), false );
	//	}
	//	break;

	case SMUCM_PRESENT_ARRIVAL_CLOSE:
		{
			if ( m_pDLGPresentArrival != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPresentArrival, NULL, false );

			m_pDLGPresentArrival = NULL;
		}
		break;


	//case SMUCM_STATE_CHANGE_ARCADE_LOBBY:
	//	{
	//		switch( g_pMain->GetNowStateID() )
	//		{
	//		case CX2Main::XS_VILLAGE_MAP:
	//			{
	//				Handler_EGS_OPEN_ARCADE_ROOM_LIST_REQ();


	//			} break;

	//		default:
	//			{
	//				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
	//				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_ARCADE_LOBBY, targetDetail );	

	//			} break;
	//		}
	//		return true;
	//	} break;

#ifdef POSTBOX
    case SMUCM_POSTBOX:
        {
            // �������� ���� �ݴ´�.                        
            //if(!g_pMain->GetPostBox()->GetDialogShow() == true)
            //{                
            //    if( true == GetCharacterRoom()->GetOpen() )
            //    {
            //        g_pMain->GetPostBox()->SetSwitchState(true);
            //        ToggleMyInfo( GetCharacterRoom()->GetCharRoomState() );                    
            //    }
            //}
            g_pMain->GetPostBox()->SetDialogShow( !g_pMain->GetPostBox()->GetDialogShow() );            
        }
        break;
#endif


	case SMUCM_EXIT_SYSTEM:
		{
			return ToggleSystemMenu(false);
		} 
		break;

	case SMUCM_LEAVE_DUNGEON_LOUNGE_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeavePartyOrLounge, NULL, false );
			m_pDLGLeavePartyOrLounge = NULL;
			g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, CX2PartyUI::PUCM_PARTY_LEAVE_DUNGEON_LOUNGE, NULL, false );

			return true;
		} break;

	case SMUCM_LEAVE_DUNGEON_LOUNGE_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeavePartyOrLounge, NULL, false );
			m_pDLGLeavePartyOrLounge = NULL;

			return true;
		} break;


	case SMUCM_LEAVE_PARTY_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeavePartyOrLounge, NULL, false );
			m_pDLGLeavePartyOrLounge = NULL;

			g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, CX2PartyUI::PUCM_PARTY_LEAVE_PARTY, NULL, false );

			return true;
		} break;

	case SMUCM_LEAVE_PARTY_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLeavePartyOrLounge, NULL, false );
			m_pDLGLeavePartyOrLounge = NULL;

			return true;
		} break;
		//{{ 09.03.18 ���¿�
	case SMUCM_OPEN_ELLIOS_GUIDE:
		{
			if(false == m_Book.GetShow())
			{
				m_Book.SetBookTable( "BT_ELLIOS_GUIDE");
				m_Book.SetShow(true);
			}
// 			else
// 			{
// 				m_Book.SetShow(false);
// 			}

		} break;
		//}}

	//  [11/9/2009 �����]
	case SMUCM_OPEN_ELLIOS_GUIDE_GO_TO_PAGE:
		{
			if(false == m_Book.GetShow())
			{
				m_Book.SetBookTable( "BT_ELLIOS_GUIDE");
				m_Book.SetShow(true);
			}
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iTargetPage = pButton->GetDummyInt(0);
			m_Book.GotoPage(iTargetPage);

#ifdef PRINT_INGAMEINFO_TO_EXCEL
			if(g_pMain->IsInGameInfoToExcel())
			{
				if(g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true)
					g_pData->GetQuestManager()->PrintQuestInfo_ToExcel();
				else if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL)==true)
					g_pData->GetSkillTree()->PrintSkillInfo_ToExcel();
				else if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PET_LIST) == true)
					g_pData->GetPetManager()->PrintPetInfo_ToExcel();
				
				g_pData->GetUnitManager()->PrintNpcInfo_ToExcel();
			}
#endif PRINT_INGAMEINFO_TO_EXCEL

		} break;

	//  [11/9/2009 �����]

#ifdef SERV_PET_SYSTEM
	case SMUCM_PET_LIST:
		{
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
			if(g_pInstanceData->GetAdamsEventShopUIShow() == true )
			{
				return false;
			}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PET_LIST);
			return true;
		}
		break;
#endif

	case SMUCM_DUNGEON_MATCH_GAME_ACCEPT:
		{
			if ( NULL != g_pX2Game )
			{
				CX2GUUser* pMyUser = g_pX2Game->GetMyUnit();

				// ���� �߿� ���� ��쿡��
				if ( NULL != pMyUser && ( pMyUser->GetGameUnitState() == CX2GameUnit::GUSI_DIE || !( pMyUser->GetNowHp() > 0 ) ) )
					g_pX2Game->CheckAndWarningBusyStateNow();
				else // �����߿� ����ִ� ���
					g_pData->GetPartyManager()->Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT( true );
			}
			else
				g_pData->GetPartyManager()->Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT( true );

			//��Ī Ȯ�� ���� ����Ű ��� ���ϵ��� ����
			CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
			if ( NULL != pNowState )
				pNowState->SetEnableShortCutKey(false);

			return true;
		} break;

	case SMUCM_DUNGEON_MATCH_GAME_DENY:
		{
			g_pData->GetPartyManager()->Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT( false );
			return true;
		} break;
	case SMUCM_DISABLE_QUICK_SLOT_MOUSE_UP:
		{			
			//�����ߴٸ� �н�
			if( true == g_pData->GetMyUser()->GetSelectUnit()->IsExpandQuickSlot() )
			{
				return true;
			}

			//������ �ƴϸ� ä�� �α� �߰�
			if( g_pX2Game != NULL && g_pMain->GetNowStateID() != CX2Main::XS_BATTLE_FIELD )
			{
				g_pChatBox->AddChatLog(  GET_STRING( STR_ID_16478 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				return false;
			}
#ifdef DIRECT_OPEN_BUY_DIALOG
			g_pData->GetCashShop()->SetMenuTypeCallingCashShop( CX2UIManager::UI_MENU_QUICK_SLOT );
#endif //DIRECT_OPEN_BUY_DIALOG
			ToggleCashShop();
			return true;
		} break;
#ifdef REFORM_UI_SKILLSLOT
	case SMUCM_DISABLE_SKILL_SLOT_MOUSE_UP:
		{
			//�����ߴٸ� �н�
			if( true == g_pData->GetUIManager()->GetUISkillTree()->IsExpandSkillSlot() )
			{
				return true;
			}

			//����/���� �����߿��� ĳ���� ���� �ʱ�.
			if( g_pX2Game != NULL && g_pMain->GetNowStateID() != CX2Main::XS_BATTLE_FIELD )
			{
				g_pChatBox->AddChatLog(  GET_STRING( STR_ID_16478 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				return true;
			}
#ifdef DIRECT_OPEN_BUY_DIALOG
			g_pData->GetCashShop()->SetMenuTypeCallingCashShop( CX2UIManager::UI_SKILL_SLOT );
#endif //DIRECT_OPEN_BUY_DIALOG
			ToggleCashShop();
			return true;
		}
		break;
#endif //REFORM_UI_SKILLSLOT
	case SMUCM_GUILD:
		{	// ��� â
			if(g_pData->GetUIManager() != NULL)
				return g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_GUILD);
		}
		break;	
	case SMUCM_NOTICE_CLOSE:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( NULL != pButton && NULL != g_pMain && NULL != g_pMain->GetNoticeManager())
				g_pMain->GetNoticeManager()->CloseItemNotice( pButton->GetDialog()->GetDummyInt(0) );
			return true;
		}
		break;
	case SMUCM_PLAYGUIDE_CLOSE:
		{
			if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
			{
				g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_NONE, false);
			}
		}
		break;

#ifdef SERV_RECOMMEND_LIST_EVENT
	case SMUCM_FRIEND_RECOMMEND_OPEN:
		{
			OpenRecommendMsgBox( true );

			return true;
		}
		break;

	case SMUCM_FRIEND_RECOMMEND_OK:
		{
			CKTDGUIIMEEditBox* pIMEEditBox = static_cast<CKTDGUIIMEEditBox* >( m_pDLGFriendRecommendPopUp->GetControl( L"g_pIMEEditBoxName" ) );
			if( NULL != pIMEEditBox )
			{
				return Handler_EGS_RECOMMEND_USER_REQ( pIMEEditBox->GetText() );
			}
		}
		break;

	case SMUCM_FRIEND_RECOMMEND_NEXT:
		{
			OpenRecommendMsgBox( false );

			return true;
		}
		break;

	case SMUCM_FRIEND_RECOMMEND_CLOSE:
		{
			OpenRecommendMsgBox( false );

			return Handler_EGS_RECOMMEND_USER_REQ( L"", true );
		}
		break;

#endif //SERV_RECOMMEND_LIST_EVENT

	case SMUCM_OPEN_KEYPAD:
		{
			ToggleSystemMenu(false);
			
			if ( NULL != g_pMain->GetKeyPad() )
				g_pMain->GetKeyPad()->ShowKeyPad( true );
		} 
		break;
#ifdef FIELD_BOSS_RAID // ���̵� �ʵ� �� ESC ó��
	case SMUCM_RAID_FIELD_EXIT_OK:
		{
			if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
				static_cast<CX2StateBattleField*>(g_pMain->GetNowState())->MoveToBeforePlaceAtRaidField();

			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		} break;
	case SMUCM_RAID_FIELD_EXIT_CANCEL:
		{
			CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>(lParam);
			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		} break;
#endif // FIELD_BOSS_RAID
#ifdef SHOW_WEB_ADVERTISEMENT
	case SMUCM_SHOW_ADVERTISEMENT:
		{
#ifdef SHOW_WEB_ADVERTISEMENT_USE_DEFAULT_BROWSER
			ShellExecuteW( GetDesktopWindow(), L"open", L"http://elsword.hangame.co.jp/campaign/index.nhn" , L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#else //SHOW_WEB_ADVERTISEMENT_USE_DEFAULT_BROWSER
			//���� �̺�Ʈ�� ���ư��� �͸� ��������� ��� �Ʒ� �Լ� ���� ��.
			//Handler_EGS_GET_ADVERTISEMENT_EVENT_REQ();
			if( false == g_pMain->GetBrowserWrapper()->IsClosed() )
			{
				g_pMain->GetBrowserWrapper()->CloseBrowser();
				g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			}

			if( true == g_pMain->GetBrowserWrapper()->IsClosed() )
			{
				g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE );

				RECT rcWebPage;
				GetWindowRect( g_pKTDXApp->GetHWND(), &rcWebPage );

				POINT ptCenter;
				ptCenter.x = rcWebPage.left + ( rcWebPage.right - rcWebPage.left ) / 2;
				ptCenter.y = rcWebPage.top + ( rcWebPage.bottom - rcWebPage.top ) / 2;

				rcWebPage.left = ptCenter.x - 415;
				rcWebPage.right = ptCenter.x + 415;
				rcWebPage.top = ptCenter.y - 235;
				rcWebPage.bottom = ptCenter.y + 235;

				// �ؿ��� ������ ���� ������
	#if defined( CLIENT_COUNTRY_TW )
				std::wstring wstrURL = L"http://tw.beanfun.com/ELSWORD/patch/patch.html";
	#elif defined( CLIENT_COUNTRY_HK )
				std::wstring wstrURL = L"http://www.elsonline.com.hk/launcher/launcher.aspx";
	#elif defined( CLIENT_COUNTRY_JP )
				std::wstring wstrURL = L"http://alpha-pubgame.hangame.co.jp/game.nhn?gameId=J_ES";
	#elif defined( CLIENT_COUNTRY_EU )
				std::wstring wstrURL = L"http://www.elswordonline.com/ingameinfo/";
	#elif defined( CLIENT_COUNTRY_US )
				std::wstring wstrURL = L"http://en.elswordonline.com/ingameinfo/";// �Ϲ� ���� �ּ�
#ifdef SERV_SEPARATE_SERVERSET_US_USER
				if( 1 == g_pInstanceData->GetServerGroupID() )// ���̸� �ּ� �ٲ۴�
					wstrURL = L"http://es.elswordonline.com/ingameinfo/";
#endif //SERV_SEPARATE_SERVERSET_US_USER
	#elif defined( CLIENT_COUNTRY_CN )
				std::wstring wstrURL = L"DLG_Advertisement.tga";
	#elif defined( CLIENT_COUNTRY_TH )
				std::wstring wstrURL = L"http://www.elswordonline.com/ingameinfo/";
	#elif defined( CLIENT_COUNTRY_ID )
				std::wstring wstrURL = L"http://elsword.netmarble.co.id/ingame/popup_start.html";
	#elif defined( CLIENT_COUNTRY_BR )
				std::wstring wstrURL = L"http://launcher.levelupgames.com.br/Launchers/Elsword/popup-ad-webpage.html";
	#elif defined( CLIENT_COUNTRY_PH )
			#if defined (_SERVICE_) && !defined (_OPEN_TEST_)
					std::wstring wstrURL = L"http://elsword.garena.ph/ingame";
			#else
				std::wstring wstrURL = L"http://elsword.garena.ph/ingame_test/";
			#endif			
	#elif defined( CLIENT_COUNTRY_IN )	
				std::wstring wstrURL = L"";
	#endif
				OpenAdvertisementDialog( wstrURL );
			}
#endif //SHOW_WEB_ADVERTISEMENT_USE_DEFAULT_BROWSER
		}
		break;
#endif SHOW_WEB_ADVERTISEMENT
#ifdef SERV_ADD_WARP_BUTTON
	case SMUCM_WARP_BUTTON:
		{
			// ĳ�� �Ҹ� ���� �ϰ� ������, ���⼭ ���� �˻縦 ����
			if( NULL != g_pData->GetMyUser() 
				&& NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_bWarpVip )
					ClickWarpButton( false );
				else
					ClickWarpButton( true );
			}
		}
		break;
	case SMUCM_WARP_DEST_CLOSE:
		{
			if( m_pDLGWarpDestination != NULL )
			{
				m_pDLGWarpDestination->SetShowEnable(false, false);
				m_iSelectedWarpDest = 0;
			}
		}
		break;
	case SMUCM_WARP_DEST_SELECT:
		{
			CKTDGUIButton *pButton = (CKTDGUIButton*)lParam;
			if( pButton == NULL )
				return true;

			m_iSelectedWarpDest = 0;
			m_iSelectedWarpDest = pButton->GetDummyInt(0);

			for(UINT i=0; i<m_vecWarpListButton.size(); ++i)
			{
				CKTDGUIButton *pDestButton = m_vecWarpListButton[i];
				if(pDestButton != NULL)
				{
					pDestButton->SetDownStateAtNormal(false);
					if( pDestButton->GetDummyInt(0) == m_iSelectedWarpDest )
					{
						m_iSelectedWarpDestIndex = i;
					}
				}
			}

			pButton->SetDownStateAtNormal(true);
		}			
		break;
	case SMUCM_WARP_DEST:
		{
			if( m_iCurrentVillageWarpIndex == -1 )
				return true;

			if( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() &&
				m_iSelectedWarpDest== g_pData->GetLocationManager()->GetCurrentVillageID() )
				return true;

			UseWarpPopup(m_bIsEdConsumptionWarp);
			return true;
		}
		break;
	case SMUCM_WARP_DEST_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			// ���� ���� ó�� �ڵ鷯 ���� 
			Handler_EGS_WARP_BY_BUTTON_REQ( m_iSelectedWarpDest );

			m_iSelectedWarpDest = 0;

			if( m_pDLGWarpDestination != NULL )
			{
				m_pDLGWarpDestination->SetShowEnable(false, false);
			}

			return true;
		}
		break;
#endif // SERV_ADD_WARP_BUTTON
#ifdef SERV_CHINA_SPIRIT_EVENT
	case SMUCM_USE_SPIRIT_REWARD_RAGE:
		{
			Handler_EGS_USE_SPIRIT_REWARD_REQ();
		}
		break;
#endif SERV_CHINA_SPIRIT_EVENT
#ifdef CLIENT_COUNTRY_US
case SMUCM_FACEBOOK_MOVE:
		{
			wstring wstrURL = L"http://www.facebook.com/ElswordOnlineNA";
#ifdef USE_DEFAULT_BROWSER
			ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#else USE_DEFAULT_BROWSER
			ShellExecuteW(NULL, _T("open"), _T("iexplore.exe"), wstrURL.c_str(), NULL, SW_SHOW); 
#endif USE_DEFAULT_BROWSER
			return true;
		}
		break;
	case SMUCM_STEAM_COMMUNITY_MOVE:
		{
			wstring wstrURL = L"https://www.facebook.com/Elsword/app_112813808737465";
#ifdef USE_DEFAULT_BROWSER
			ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#else USE_DEFAULT_BROWSER
			ShellExecuteW(NULL, _T("open"), _T("iexplore.exe"), wstrURL.c_str(), NULL, SW_SHOW); 
#endif USE_DEFAULT_BROWSER
			return true;
		}
		break;
#endif //CLIENT_COUNTRY_US
#ifdef SERV_ELESIS_UPDATE_EVENT
	case SMUCM_VIEW_NOTE:
		{
			CX2State* pState = (CX2State*)g_pMain->GetNowState();

			// ��Ʈ UI�� ��� �غ�
			pState->ReadyToShowEventNoteUI();
			pState->SetShowNoteUI( true );
			return true;
		}
		break;
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_NEW_YEAR_EVENT_2014
	case SMUCM_COMPLETE_SPECIAL_MISSION:
		{
			Handler_EGS_2013_EVENT_MISSION_COMPLETE_REQ( false );
			return true;
		} break;
	case SMUCM_COMPLETE_SUPPORT_MISSION:
		{
			Handler_EGS_2013_EVENT_MISSION_COMPLETE_REQ( true );
			return true;
		} break;
	case SMUCM_COMPLETE_NEW_YEAR_MISSION:
		{
			if( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
			{
				return false;
			}

			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
			if( pUnit->GetNewYearMissionStepID() == 0 )
			{
				m_pDLG2014Confirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_29054 ), SMUCM_COMPLETE_NEW_YEAR_MISSION_OK , g_pMain->GetNowState(), SMUCM_COMPLETE_NEW_YEAR_MISSION_CANCEL );
			}
			else if( pUnit->GetNewYearMissionStepID() > 0 )
			{
				Handler_EGS_2014_EVENT_MISSION_COMPLETE_REQ();
			}
			return true;
		} break;
	case SMUCM_COMPLETE_NEW_YEAR_MISSION_OK:
		{
			if ( m_pDLG2014Confirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLG2014Confirm, NULL, false );

			Handler_EGS_2014_EVENT_MISSION_COMPLETE_REQ();

			m_pDLG2014Confirm		= NULL;
			return true;
		} break;
	case SMUCM_COMPLETE_NEW_YEAR_MISSION_CANCEL:
		{
			if ( m_pDLG2014Confirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLG2014Confirm, NULL, false );

			m_pDLG2014Confirm		= NULL;
			return true;
		} break;
	case SMUCM_NEW_YEAR_2014_UI_TOGGLE:
		{
			g_pMain->GetMemoryHolder()->SetShowNewYear2014EventBoard( !g_pMain->GetMemoryHolder()->GetShowNewYear2014EventBoard() );
			return true;
		} break;
	case SMUCM_NEW_YEAR_2014_UI_CLOSE:
		{
			g_pMain->GetMemoryHolder()->SetShowNewYear2014EventBoard( false );
			return true;
		} break;
#endif SERV_NEW_YEAR_EVENT_2014
#ifdef SERV_CHANNELING_AERIA
	case SMUCM_SHOW_AERIA_USER_SUPPORT:
		{
#if defined (_SERVICE_) && !defined (_OPEN_TEST_) // ����
			std::wstring wstrURL = L"https://api.elswordonline.com/aeria/auth/gamesupport?SID=";
#else // �׼�
			std::wstring wstrURL = L"http://apitest.elswordonline.com/aeria/auth/gamesupport?SID=";
#endif // defined (_SERVICE_) && !defined (_OPEN_TEST_)
			wstrURL += g_pInstanceData->GetUserSessionID();
#ifdef USE_DEFAULT_BROWSER
			ShellExecuteW( GetDesktopWindow(), L"open", wstrURL.c_str(), L"dwmApi #102", NULL, SW_SHOWNORMAL); 
#else USE_DEFAULT_BROWSER
			ShellExecuteW(NULL, _T("open"), _T("iexplore.exe"), wstrURL.c_str(), NULL, SW_SHOW); 
#endif USE_DEFAULT_BROWSER
			return true;
		} break;
#endif //SERV_CHANNELING_AERIA

#ifdef SERV_EVENT_CHECK_POWER
	case SMUCM_TOGGLE_CHECK_POWER_GUIDE_PAGE:
		{
			if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

				if( pUnit->IsShowCheckPowerPopUp() == true )
				{
					pUnit->SetShowCheckPowerPopUp( false );
					Handler_EGS_START_CHECK_POWER_REQ( false );
				}
			}

			g_pMain->GetMemoryHolder()->SetShowCheckPowerEventGuidePage( !g_pMain->GetMemoryHolder()->GetShowCheckPowerEventGuidePage() );
			g_pMain->GetMemoryHolder()->UpdateCheckPowerEvent();
		} return true;
	case SMUCM_START_CHECK_POWER:
		{
			if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
				pUnit->SetCheckPowerTime( g_pData->GetServerCurrentTime() );
			}

			g_pMain->GetMemoryHolder()->SetShowCheckPowerEventGuidePage( false );
			Handler_EGS_START_CHECK_POWER_REQ( true );
		} return true;
#endif SERV_EVENT_CHECK_POWER
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		case SMUCM_USE_CHUNG_GIVE_ITEM_ONE:
			{
				Handler_EGS_EVENT_CHUNG_GIVE_ITEM_REQ( 1 ); //����
			}break;
		case SMUCM_USE_CHUNG_GIVE_ITEM_TWO:
			{
				Handler_EGS_EVENT_CHUNG_GIVE_ITEM_REQ( 2 ); //1��
			}break;
		case SMUCM_USE_CHUNG_GIVE_ITEM_TREE:
			{
				Handler_EGS_EVENT_CHUNG_GIVE_ITEM_REQ( 3 ); //2��
			}break;
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	case SMUCM_USE_COBO_EVET_BUTTON:
		{
			return Handler_EGS_EVENT_COBO_DUNGEON_FIELD_REQ(true); //��ư�� ������.
		}break;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	case SMUCM_USE_ADAMS_EVET_BUTTON:
		{
			if( NULL != g_pData &&
				NULL != g_pData->GetUIManager() )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_SHOP, true, CX2LocationManager::HI_EVENT_ADAMS_UI_SHOP);	//�̺�Ʈ ���� UI������ 
			}
			if( g_pInstanceData != NULL )
			{
				g_pInstanceData->SetAdamsEventShopUIShow(true); //���� UI�� ������!!!
				g_pData->GetUIManager()->SetShowPartyMenu(false);
				g_pData->GetUIManager()->SetShowQucikQuest(false);
				//���� �ο����� ���� �� ����Ű ��� ���ϵ��� ����
				CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( NULL != pNowState )
					pNowState->SetEnableShortCutKey(false);
			}
		} break;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
#ifdef SERV_4TH_ANNIVERSARY_EVENT
	case SMUCM_TOGGLE_EVENT:
		{
			g_pMain->GetMemoryHolder()->ToggleShow4thEvent();
			return true;
		} break;
	case SMUCM_4TH_EVENT_BUTTON:
		{
			CKTDGUIButton *pButton = (CKTDGUIButton*)lParam;
			if( pButton != NULL )
			{
				int iSelectedButtonIndex = pButton->GetDummyInt( 0 );
				if( iSelectedButtonIndex >= 0 && iSelectedButtonIndex <= 11 )
				{
					m_iSelectedButtonIndex = iSelectedButtonIndex;
					m_pDLG4thConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_30426 ), SMUCM_4TH_EVENT_OK, g_pMain->GetNowState(), SMUCM_4TH_EVENT_CANCEL );
				}
			}
			return true;
		} break;
	case SMUCM_4TH_EVENT_OK:
		{
			Handler_EGS_4TH_ANNIV_EVENT_REWARD_REQ( m_iSelectedButtonIndex );

			if ( m_pDLG4thConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLG4thConfirm, NULL, false );

			m_pDLG4thConfirm		= NULL;
			return true;
		} break;
	case SMUCM_4TH_EVENT_CANCEL:
		{
			if ( m_pDLG4thConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLG4thConfirm, NULL, false );

			m_pDLG4thConfirm		= NULL;
			return true;
		} break;

#endif //SERV_4TH_ANNIVERSARY_EVENT
	}
	
	return false; 
}

bool CX2StateMenu::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateCommonBG::UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
		return true;

#ifdef COME_BACK_REWARD
	if ( NULL !=g_pData->GetPremiumBuffManager() )
	{
		if ( true == g_pData->GetPremiumBuffManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif

#ifndef COUPON_SYSTEM // ���� UI ����
	if( NULL != g_pMain->GetCouponBox() )
	{
		if( true == g_pMain->GetCouponBox()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif // COUPON_SYSTEM

	if ( m_pCashShop != NULL )
	{
		if ( m_pCashShop->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

//#ifndef NEW_VILLAGE_UI
	//if ( NULL != m_pCharacterRoom ) 
	//{
	//	if( m_pCharacterRoom->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
	//		return true;
	//}
//#endif
#ifndef NEW_SKILL_TREE
	if( NULL != g_pInstanceData->GetSkillTreeUI() )
	{
		if( g_pInstanceData->GetSkillTreeUI()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif

	if( g_pMain->GetKeyPad() != NULL )
	{
		if( true == g_pMain->GetKeyPad()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}

#ifdef POSTBOX
    if( g_pMain->GetPostBox() != NULL )
    {
        if( true == g_pMain->GetPostBox()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
            return true;
    }
#endif

#ifdef SERV_SECOND_SECURITY
	if( g_pMain->GetSecurityPad() != NULL )
	{
		if( true == g_pMain->GetSecurityPad()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif SERV_SECOND_SECURITY


	switch ( wParam )
	{
#ifdef SERV_RECOMMEND_LIST_EVENT
	case EGS_RECOMMEND_USER_ACK:
		{
			return Handler_EGS_RECOMMEND_USER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_RECOMMEND_LIST_EVENT

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
#ifndef NEW_MESSENGER
		case EGS_KNM_TOGETHER_ACK:
			{
				return Handler_EGS_KNM_TOGETHER_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_KNM_INVITE_ACK:
			{
				return Handler_EGS_KNM_INVITE_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_KNM_INVITE_NOT:
			{
				return Handler_EGS_KNM_INVITE_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
#endif NEW_MESSENGER
#ifdef SERV_GLOBAL_BILLING
		case EGS_BILL_PRODUCT_INFO_ACK:
			{
				return Handler_EGS_BILL_PRODUCT_INFO_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;
#else // SERV_GLOBAL_BILLING
		case EGS_CASH_PRODUCT_INFO_ACK:
			{
				return Handler_EGS_CASH_PRODUCT_INFO_ACK( hWnd, uMsg, wParam, lParam ); 
			}
			break;
#endif // SERV_GLOBAL_BILLING
//{{ kimhc	// �ǽð� ���ҵ� �� �ǽð� ������ ȹ�� ���� �ӽ� �κ��丮
#ifdef	REAL_TIME_ELSWORD
		case EGS_CLEAR_TEMP_INVENTORY_NOT:
			{
				return Handler_EGS_CLEAR_TEMP_INVENTORY_NOT( hWnd, uMsg, wParam, lParam );
			} break;
#endif	REAL_TIME_ELSWORD
//}} kimhc	// �ǽð� ���ҵ� �� �ǽð� ������ ȹ�� ���� �ӽ� �κ��丮

#ifdef SERV_NAVER_CHANNELING
		case EGS_GET_NAVER_ACCESS_TOKEN_ACK:
			{
				return Handler_EGS_GET_NAVER_ACCESS_TOKEN_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif // SERV_NAVER_CHANNELING
		//case EGS_OPEN_ARCADE_ROOM_LIST_ACK:
		//	return Handler_EGS_OPEN_ARCADE_ROOM_LIST_ACK( hWnd, uMsg, wParam, lParam );
		//	break;
#ifdef SERV_ADVERTISEMENT_EVENT
		case EGS_GET_ADVERTISEMENT_EVENT_ACK:
			{
				return Handler_EGS_GET_ADVERTISEMENT_EVENT_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif SERV_ADVERTISEMENT_EVENT
#ifdef SERV_ADD_WARP_BUTTON
		case EGS_WARP_BY_BUTTON_ACK:
			{
				return Handler_EGS_WARP_BY_BUTTON_ACK( hWnd, uMsg, wParam, lParam );
			} break;
		case EGS_UPDATE_WARP_VIP_NOT:
			{
				return Handler_EGS_UPDATE_WARP_VIP_NOT( hWnd, uMsg, wParam, lParam );
			} break;
#endif //SERV_ADVERTISEMENT_EVENT
#ifdef SERV_CHINA_SPIRIT_EVENT
		case EGS_USE_SPIRIT_REWARD_ACK:
			{
				return Handler_EGS_USE_SPIRIT_REWARD_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif SERV_CHINA_SPIRIT_EVENT
#ifdef SERV_NEW_YEAR_EVENT_2014
		case EGS_2013_EVENT_MISSION_COMPLETE_ACK:
			{
				return Handler_EGS_2013_EVENT_MISSION_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
			} break;
		case EGS_2014_EVENT_MISSION_COMPLETE_ACK:
			{
				return Handler_EGS_2014_EVENT_MISSION_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif SERV_NEW_YEAR_EVENT_2014
#ifdef SERV_CONTENT_MANAGER_INT
		case EGS_CASH_SHOP_OPEN_NOT:
			{
				return Handler_EGS_CASH_SHOP_OPEN_NOT( hWnd, uMsg, wParam, lParam );
			} break;
#endif //SERV_CONTENT_MANAGER_INT
#ifdef SERV_EVENT_CHECK_POWER
		case EGS_START_CHECK_POWER_ACK:
			{
				return Handler_EGS_START_CHECK_POWER_ACK( hWnd, uMsg, wParam, lParam );
			} break;
		case EGS_UPDATE_CHECK_POWER_NOT:
			{
				return Handler_EGS_UPDATE_CHECK_POWER_NOT( hWnd, uMsg, wParam, lParam );
			} break;
		case EGS_CHECK_POWER_RESULT_NOT:
			{
				return Handler_EGS_CHECK_POWER_RESULT_NOT( hWnd, uMsg, wParam, lParam );
			} break;
#endif SERV_EVENT_CHECK_POWER
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		case EGS_EVENT_CHUNG_GIVE_ITEM_ACK:
			{
				return Handler_EGS_EVENT_CHUNG_GIVE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		case EGS_EVENT_COBO_DUNGEON_FIELD_ACK:
			{
				return Handler_EGS_EVENT_COBO_DUNGEON_FIELD_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		case EGS_4TH_ANNIV_EVENT_REWARD_ACK:
			{
				return Handler_EGS_4TH_ANNIV_EVENT_REWARD_ACK( hWnd, uMsg, wParam, lParam );
			} break;
#endif //SERV_4TH_ANNIVERSARY_EVENT
	}

	return false;
}






#ifndef NEW_MESSENGER
bool CX2StateMenu::Handler_EGS_KNM_TOGETHER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KNM_TOGETHER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_KNM_TOGETHER_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData->GetMessenger() != NULL )
				return g_pData->GetMessenger()->Handler_EGS_KNM_TOGETHER_ACK( kEvent );
		}

	}


	return false;
}

bool CX2StateMenu::Handler_EGS_KNM_INVITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KNM_INVITE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_KNM_INVITE_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_661 ), this );
			return true;
		}
	}

	return false;
}

bool CX2StateMenu::Handler_EGS_KNM_INVITE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KNM_INVITE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if ( m_pDLGInviteNot == NULL )
	{
		//SAFE_DELETE_DIALOG( m_pDLGInviteNot );
		m_InviterName = kEvent.m_wstrNickName;
        m_pDLGInviteNot = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_REPLACED_STRING( ( STR_ID_662, "L", m_InviterName ) ), CX2StateMenu::SMUCM_INVITE_NOT_OK, this, CX2StateMenu::SMUCM_INVITE_NOT_CANCEL );
	}
	return true;
}
#endif NEW_MESSENGER

#ifdef SERV_GLOBAL_BILLING
bool CX2StateMenu::Handler_EGS_BILL_PRODUCT_INFO_REQ()
{
	KEGS_BILL_PRODUCT_INFO_REQ kPacket;
	kPacket.m_iPage = m_CashItemPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_BILL_PRODUCT_INFO_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BILL_PRODUCT_INFO_ACK );

	SAFE_DELETE_DIALOG( m_pDLGCashItemUpdate );
	m_pDLGCashItemUpdate = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_663 ), g_pMain->GetNowState() );

	return true;
}

bool CX2StateMenu::Handler_EGS_BILL_PRODUCT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_BILL_PRODUCT_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_BILL_PRODUCT_INFO_ACK ) == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGCashItemUpdate );
#ifdef SERV_WISH_LIST_NO_ITEM
		g_pData->GetItemManager()->AddCashItem( kEvent.m_vecProductInfo, kEvent.m_setWishListNoItemList );
#else	//SERV_WISH_LIST_NO_ITEM
		g_pData->GetItemManager()->AddCashItem( kEvent.m_vecProductInfo );
#endif //SERV_WISH_LIST_NO_ITEM
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		g_pData->GetItemManager()->SetKeepShowItemList( kEvent.m_mapKeepShowItem );
		g_pData->GetItemManager()->SetDisCountList( kEvent.m_DisCountInfoMap );
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

		if ( kEvent.m_bFinal == true )
		{
			m_CashItemPage = 1;
			g_pData->GetItemManager()->AddCashItemEx();
			if( m_pCashShop != NULL )
			{
				m_pCashShop->Handler_EGS_GET_WISH_LIST_REQ();
				m_pCashShop->SetOpen( true );
			}//if
			ChangeByToggleCashShop();
			//���� �ѹ� ������.. �������.. 
		}
		else
		{
			m_CashItemPage++;
			Handler_EGS_BILL_PRODUCT_INFO_REQ();
		}

		return true;
	}
	return false;
}
#else // SERV_GLOBAL_BILLING
bool CX2StateMenu::Handler_EGS_CASH_PRODUCT_INFO_REQ()
{
	KEGS_CASH_PRODUCT_INFO_REQ kPacket;
	kPacket.m_iPage = m_CashItemPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_CASH_PRODUCT_INFO_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CASH_PRODUCT_INFO_ACK );

	SAFE_DELETE_DIALOG( m_pDLGCashItemUpdate );
	m_pDLGCashItemUpdate = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_663 ), g_pMain->GetNowState() );

	return true;
}

bool CX2StateMenu::Handler_EGS_CASH_PRODUCT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_CASH_PRODUCT_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CASH_PRODUCT_INFO_ACK ) == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGCashItemUpdate );

		g_pData->GetItemManager()->AddCashItem( kEvent.m_vecProductInfo );

		if ( kEvent.m_bFinal == true )
		{
			m_CashItemPage = 1;
			g_pData->GetItemManager()->AddCashItemEx();
            if( m_pCashShop != NULL )
            {
			    m_pCashShop->Handler_EGS_GET_WISH_LIST_REQ();
			    m_pCashShop->SetOpen( true );
            }//if
			ChangeByToggleCashShop();
			//���� �ѹ� ������.. �������.. 
		}
		else
		{
			m_CashItemPage++;
			Handler_EGS_CASH_PRODUCT_INFO_REQ();
		}

		return true;
	}
	return false;
}
#endif // SERV_GLOBAL_BILLING

bool CX2StateMenu::Handler_EGS_CASH_PRODUCT_INFO_CHANGED_NOT()
{
	if ( m_pCashShop != NULL )
	{
		if ( m_pCashShop->GetOpen() == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_664 ), this);
			ToggleCashShop();
		}
	}

	CX2State::Handler_EGS_CASH_PRODUCT_INFO_CHANGED_NOT();

	return true;
}

//{{ kimhc	// �ǽð� ���ҵ� �� �ǽð� ������ ȹ�� ���� �ӽ� �κ��丮
#ifdef	REAL_TIME_ELSWORD
bool CX2StateMenu::Handler_EGS_CLEAR_TEMP_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	g_pData->GetUIManager()->DestoryUITempInventory();

	return true;
}
#endif	REAL_TIME_ELSWORD
//}} kimhc	// �ǽð� ���ҵ� �� �ǽð� ������ ȹ�� ���� �ӽ� �κ��丮


bool CX2StateMenu::HandleMsgByESCImp()
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

	if ( m_pCashShop != NULL &&
		m_pCashShop->GetOpen() == true )
	{
		ToggleCashShop();
		return true;
	}
#ifdef POSTBOX
    else if( g_pMain->GetPostBox() != NULL && g_pMain->GetPostBox()->GetDialogShow() == true)
    {
        g_pMain->GetPostBox()->SetDialogShow(false);
        return true;
    }
#endif

	// �ʵ峻 escŰ ���Ǻ� ó��
	if(g_pTFieldGame != NULL )
	{
		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PARTY) == true)
		{
			if( g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) == false 
#ifdef SERV_PVP_NEW_SYSTEM
				&& g_pData->GetPartyManager()->GetProcessPvpMatch() == false 
#endif
				)
			{
				g_pMain->GetPartyUI()->OpenPartyDLG(false);
				return true;
			}
		}

		if(g_pTFieldGame->GetShowMarketList() == true)
		{
			g_pTFieldGame->CloseMarketList();
			return true;
		}

		if(g_pTFieldGame->GetJoinNpc() == true)
		{
			g_pTFieldGame->SetShowNpcMessage(false);
			return true;
		}

		if(g_pData->GetUIManager()->GetUIQuestNew() != NULL)
		{
			if(g_pData->GetUIManager()->GetUIQuestNew()->GetShowClearDLG() == true)
				return true;

		}

	}

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( true == g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) )
			{
				SAFE_DELETE_DIALOG( m_pDLGLeavePartyOrLounge );
				m_pDLGLeavePartyOrLounge = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_665 ), SMUCM_LEAVE_DUNGEON_LOUNGE_OK, g_pMain->GetNowState(), SMUCM_LEAVE_DUNGEON_LOUNGE_CANCEL );
			}
			else
			{
				if ( NULL != g_pMain->GetKeyPad() && false == g_pMain->GetKeyPad()->GetShowKeyPad() )
					ToggleSystemMenu(); // �ý���UI ����.
			}
		} break;

	case CX2Main::XS_BATTLE_FIELD:
		{
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PARTY) == true)
			{
				g_pMain->GetPartyUI()->OpenPartyDLG(false);
				return true;
			}

#ifdef FIX_ESC_KEY_BY_FIELD		/// �ʵ忡�� ����Ʈ �Ϸ�â ������ ESCŰ ������ ����( ũ���� )
			if(g_pData->GetUIManager()->GetUIQuestNew() != NULL)
			{
				if(g_pData->GetUIManager()->GetUIQuestNew()->GetShowClearDLG() == true)
					return true;
			}
#endif FIX_ESC_KEY_BY_FIELD

#ifdef FIELD_BOSS_RAID // ���̵� �ʵ� �� ESC ó��
			if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
			{
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_29048 ), 
					SMUCM_RAID_FIELD_EXIT_OK, g_pMain->GetNowState(), SMUCM_RAID_FIELD_EXIT_CANCEL );
				return true;
			}
#endif // HandleMsgByESCImp
			if ( NULL != g_pMain->GetKeyPad() && false == g_pMain->GetKeyPad()->GetShowKeyPad() )
				ToggleSystemMenu(); // �ý���UI ����.
		} break;

	default:
		LastMsgByESC();
		break;
	}

	if ( NULL != g_pMain->GetKeyPad() )
		g_pMain->GetKeyPad()->ShowKeyPad( false );

	return false;
}

bool CX2StateMenu::ToggleLobbyMenuWindow()
{
	m_bShowWindowMenu = !m_bShowWindowMenu;

	if ( m_bShowWindowMenu == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
		m_pDLGMenuWindow = new CKTDGUIDialog( this, L"DLG_Menu_Window.lua", 0.1f);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMenuWindow );
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
		//g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMenuWindow, NULL, false );
		//m_pDLGMenuWindow = NULL;
	}

	return true;
}


CX2Inventory::SORT_TYPE CX2StateMenu::GetInventorySortType( const WCHAR* pRadioButtonName )
{
	CX2Inventory::SORT_TYPE sortType = CX2Inventory::ST_NONE;

	if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_Equip" ) == 0 )
	{
		sortType = CX2Inventory::ST_EQUIP;
	}
	else if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_Accessory" ) == 0 )
	{
		sortType = CX2Inventory::ST_ACCESSORY;
	}
	else if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_UseItem" ) == 0 )
	{
		sortType = CX2Inventory::ST_QUICK_SLOT;
	}
	else if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_Material" ) == 0 )
	{
		sortType = CX2Inventory::ST_MATERIAL;
	}
	else if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_Card" ) == 0 )
	{
		sortType = CX2Inventory::ST_QUEST;
	}
	else if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_Special" ) == 0 )
	{
		sortType = CX2Inventory::ST_SPECIAL;
	}
	else if ( wcscmp( pRadioButtonName, L"RadioButtonUnit_Inventory_Avatar" ) == 0 )
	{
		sortType = CX2Inventory::ST_AVARTA;
	}
	

	return sortType;
}



void CX2StateMenu::UpdateUserInfoUI( KEGS_SEARCH_UNIT_ACK &kEvent )
{

	if( m_pDLGUserInfo == NULL )
		return;

	
	CKTDGUIStatic* pStatic_UnitFace = (CKTDGUIStatic*) m_pDLGUserInfo->GetControl( L"Static_UnitFace" );
	CKTDGUIStatic* pStatic_UnitInfo = (CKTDGUIStatic*) m_pDLGUserInfo->GetControl( L"Static_User_Info" );

	// ĳ���� Ŭ���� �׸� 
	wstring fileName;
	wstring pieceName;
	if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS) kEvent.m_kCUnitInfo.m_cUnitClass, CX2Data::CIT_Party ) )
	{
		pStatic_UnitFace->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
	}
	else
	{
		ASSERT( !"NO" );
	}




	// ĳ���� ���� �̸�
	pStatic_UnitInfo->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_667, "i", ( int )kEvent.m_kCUnitInfo.m_ucLevel ) );
	pStatic_UnitInfo->GetString(1)->msg = kEvent.m_kCUnitInfo.m_wstrCharName.c_str();

#ifdef SERV_PVP_NEW_SYSTEM
	// ���� 
	pStatic_UnitInfo->GetString(3)->msg = GET_REPLACED_STRING( ( STR_ID_669, "i", kEvent.m_kCUnitInfo.m_iWin ) );
#else
	// ���� ����
	pStatic_UnitInfo->GetString(2)->msg = GET_REPLACED_STRING( ( STR_ID_668, "i", ( int )kEvent.m_kCUnitInfo.m_uiPVPRanking ) );

	// ���� 
	pStatic_UnitInfo->GetString(3)->msg = GET_REPLACED_STRING( ( STR_ID_669, "i", kEvent.m_kCUnitInfo.m_iWin ) );
	pStatic_UnitInfo->GetString(4)->msg = GET_REPLACED_STRING( ( STR_ID_667, "i", kEvent.m_kCUnitInfo.m_iLose ) );

	// �·� 
	if ( ( kEvent.m_kCUnitInfo.m_iLose + kEvent.m_kCUnitInfo.m_iWin ) == 0 )
	{
		pStatic_UnitInfo->GetString(5)->msg = L"";
	}
	else
	{
		pStatic_UnitInfo->GetString(5)->msg = GET_REPLACED_STRING( ( STR_ID_667, "f", ( float )( kEvent.m_kCUnitInfo.m_iWin ) / (float)( kEvent.m_kCUnitInfo.m_iLose + kEvent.m_kCUnitInfo.m_iWin ) * 100.0f ) );
	}
#endif

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

	//���� ǥ�� ������ϴµ� �����ʿ��� ���� �۾��� �� �Ǿ���.
	CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)m_pDLGUserInfo->GetControl( L"UnitEmblem" );
	if ( pStaticEmblem != NULL && pStaticEmblem->GetPicture(0) != NULL )
	{
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
		char cRank = kEvent.m_kCUnitInfo.m_cRank;
		CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>(cRank) );

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

	// �����ϱ� ��ư 
	CKTDGUIButton* pButton_PlayTogether = (CKTDGUIButton*) m_pDLGUserInfo->GetControl( L"Button_Play_Together" );


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



bool CX2StateMenu::InitUI()
{
	m_pDLGMenu = g_pMain->GetMemoryHolder()->GetMenuDLG( this );
	m_vecpButtonMainMenu.clear();
	UpdateMenuPosistion();
	UpdateMenuString();

#ifndef NEW_VILLAGE_UI
	if ( m_pDLGMenu->GetControl( L"ButtonBack" ) != NULL )
	{
		m_pDLGMenu->GetControl( L"ButtonBack" )->SetEnable( true );
		m_pDLGMenu->GetControl( L"ButtonBack" )->SetShow( true );
	}
#endif

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		m_pDLGChungItemUI = g_pMain->GetMemoryHolder()->GetChungGiveItemDLG(this);
		if( m_pDLGChungItemUI != NULL )
			m_pDLGChungItemUI->SetShow(true);
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
	{
		m_pDLGCoboEventUI = g_pMain->GetMemoryHolder()->GetUseCoboEventDLG(this);
		if(m_pDLGCoboEventUI != NULL)
		{
			m_pDLGCoboEventUI->SetShow(true);
		}
		m_pDLGCoboEventCountUI = g_pMain->GetMemoryHolder()->GetCoboEventCountDLG(this);
		if(m_pDLGCoboEventCountUI != NULL)
		{
			m_pDLGCoboEventCountUI->SetShow(true);
			m_pDLGCoboEventCountUI->SetPos(D3DXVECTOR2( 0, 17 ));
		}
	}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	m_pDLGAdamsEventShopUI = g_pMain->GetMemoryHolder()->GetUseAdamsEventShop(this);
	if(m_pDLGAdamsEventShopUI != NULL)
	{
		m_pDLGAdamsEventShopUI->SetShow(true);
	}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
#ifdef SERV_RECOMMEND_LIST_EVENT
	if( NULL != m_pDLGMenu->GetControl( L"FriendRecommed" ) )
	{
		bool is_show = g_pData->Get_ShowRecommendUI();
		//���������� �����ϵ���
		if( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
			m_pDLGMenu->GetControl( L"FriendRecommed" )->SetShowEnable( is_show, is_show );
		else
			m_pDLGMenu->GetControl( L"FriendRecommed" )->SetShowEnable( is_show, false );
	}
#endif //SERV_RECOMMEND_LIST_EVENT

#ifndef COUPON_SYSTEM // ���� UI ����
	if( NULL != g_pMain->GetCouponBox() )
	{
		g_pMain->GetCouponBox()->SetNowState( (CX2State*) this );
	}
#endif // COUPON_SYSTEM

#ifdef POSTBOX
    if( NULL != g_pMain->GetPostBox() )
    {
        g_pMain->GetPostBox()->SetNowState( (CX2State*) this );
    }
#endif

#ifdef DISABLE_STATEMENU_IN_DUNGEON
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			for( int i=0; i<m_pDLGMenu->GetControlSize(); i++ )
			{
				CKTDGUIControl* pControl = m_pDLGMenu->GetControl( i );
				if( NULL == pControl )
					continue;

				if( CKTDGUIControl::UCT_BUTTON != pControl->GetType() )
					continue;

				CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;
				switch(	pButton->GetCustomMsgMouseUp() )
				{
					// ����Ʈ�� Ŀ�´�Ƽ�� Ȱ��ȭ
				case CX2StateMenu::SMUCM_MY_INFO_QUEST:
				case CX2StateMenu::SMUCM_COMMUNITY:
		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� �κ��丮 ���� �� ����
#ifdef	REAL_TIME_ELSWORD 
				case CX2StateMenu::SMUCM_MY_INFO:
				case CX2StateMenu::SMUCM_INVENTORY:
#endif	REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� �κ��丮 ���� �� ����
				case CX2StateMenu::SMUCM_MY_INFO_SKILL_TREE:
#ifdef SERV_PET_SYSTEM
				case CX2StateMenu::SMUCM_PET_LIST:
#endif

					{
						pButton->SetEnable(true);
					} break;
				default:
					{
						pButton->SetEnable(false);

					} break;
				}
			}
		} break;

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			for( int i=0; i<m_pDLGMenu->GetControlSize(); i++ )
			{
				CKTDGUIControl* pControl = m_pDLGMenu->GetControl( i );
				if( NULL == pControl )
					continue;

				if( CKTDGUIControl::UCT_BUTTON != pControl->GetType() )
					continue;

				CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;
				switch(	pButton->GetCustomMsgMouseUp() )
				{
					// ��Ƽ�� ��Ȱ��ȭ
				case CX2StateMenu::SMUCM_PARTY:
#ifdef SERV_PET_SYSTEM // oasis907 : ����� [2010.9.15] // �굵 ��Ȱ��ȭ
				case CX2StateMenu::SMUCM_PET_LIST:
#endif SERV_PET_SYSTEM
#ifdef DISABLE_CASHSHOP_BUTTON
				case CX2StateMenu::SMUCM_CASH_SHOP:
#endif
#ifdef RESTRICTION_SYSTEM_BUTTON_BY_SQUARE	/// �ŷ� ���忡���� �ý��� �޴� ��Ȱ��ȭ ��Ŵ
				case CX2StateMenu::SMUCM_SYSTEM:
#endif RESTRICTION_SYSTEM_BUTTON_BY_SQUARE
					{
						pButton->SetEnable(false);

					} break;
				default:
					{
						pButton->SetEnable(true);

					} break;
				}
			}

		} break;
		
	default:
		{
			for( int i=0; i<m_pDLGMenu->GetControlSize(); i++ )
			{
				CKTDGUIControl* pControl = m_pDLGMenu->GetControl( i );
				if( NULL == pControl )
					continue;

				if( CKTDGUIControl::UCT_BUTTON != pControl->GetType() )
					continue;

				CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;

#ifdef SERV_RECOMMEND_LIST_EVENT
				if( 0 == StrCmp( pButton->GetName(), L"FriendRecommed") )
					continue;				
#endif //SERV_RECOMMEND_LIST_EVENT

#ifdef DISABLE_CASHSHOP_BUTTON
				if ( pButton->GetCustomMsgMouseUp() != CX2StateMenu::SMUCM_CASH_SHOP )
#endif
					pControl->SetEnable(true);
			}

		} break;
	}
#endif
	if( NULL != g_pData && NULL != g_pData->GetWorldMissionManager() && 
		NULL != g_pData->GetWorldMissionManager()->GetUIWorldMission() )
		g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetForceUpdateCrystalUI();

	RefreshMenuInfo();
	return true;
}

bool CX2StateMenu::InitETC()
{
#ifndef NEW_SKILL_TREE
	if( NULL == g_pInstanceData->GetSkillTreeUI() )
	{
		g_pInstanceData->ResetSkillTreeUI();
	}
#endif

	//if ( g_pData->GetCharacterRoom() == NULL )
	//{
	//	g_pData->ResetCharacterRoom( this, L"DLG_Unit_State_SlotList.lua" );
	//}

	//m_pCharacterRoom = g_pData->GetCharacterRoom();
	//m_pCharacterRoom->SetNowStage( this );
	//m_pCharacterRoom->SetOpen( false );


	if ( g_pData->GetCashShop() == NULL )
	{
		g_pData->ResetCashShop( this, L"DLG_Cash_Shop_Slot_List.lua" );
	}


	m_pCashShop = g_pData->GetCashShop();
    ASSERT( m_pCashShop != NULL );
    if( m_pCashShop != NULL && m_pCashShop->GetOpen() )
    {
	    m_pCashShop->SetOpen( false );
    }//if

	CX2Community* pMessenger = g_pData->GetMessenger();
	if ( pMessenger != NULL )
	{
		pMessenger->SetStage( this );
		pMessenger->SetOpenFriendReqMsgBox( true );
	}

	g_pMain->GetInformerManager()->CheckInfo( true );

	
	return true;
}


void CX2StateMenu::PopUpMenuClose( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if  ( ( uMsg == CKTDXApp::KM_UI_CONTROL_EVENT || 
		uMsg == CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT ||
		uMsg == CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC ) && ( (STATE_MENU_UI_CUSTOM_MSG)wParam != SMUCM_MENU ) )
	{
		CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
		if( NULL != pControl )
		{
			CKTDGUIDialogType pDialog = pControl->GetDialog();
			if ( pDialog != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMenuWindow, NULL, false );
				m_pDLGMenuWindow = NULL;

				m_bShowWindowMenu = false;
			}
		}
	}





#if 0 
	// ���� user list���� ������ ��ư���� ��� �˾� �޴� ���߱�
	if( (uMsg == CKTDXApp::KM_UI_CONTROL_EVENT || 
		uMsg == CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT ||
		uMsg == CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC) && wParam != 0 )
	{
		CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
		if( NULL != pControl && g_pData->GetUserMenu() != NULL && g_pData->GetUserMenu()->GetDialog() != NULL)
		{
			CKTDGUIDialogType pDialog = pControl->GetDialog();
			if( pDialog != NULL && pDialog != g_pData->GetUserMenu()->GetDialog() )
			{			
				g_pData->GetUserMenu()->ClosePopupMenu();		
			}
		}
	}
#endif
}

bool CX2StateMenu::GetShowMsgBox()
{	
	CKTDGUIDialogType pDialog = NULL;

	if(g_pKTDXApp->GetDGManager()->GetDialogManager() != NULL)
	{
		pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"DLG_OKMsgBox");
		if(pDialog != NULL)
		{
			return pDialog->GetShow();
		}

		pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"DLG_MsgBox");
		if(pDialog != NULL)
		{
			return pDialog->GetShow();
		}

		pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"OkAndCancelMsgBox");
		if(pDialog != NULL)
		{
			return pDialog->GetShow();
		}
	}

	return false;
}

void CX2StateMenu::OpenFindUserDLG()
{
	SAFE_DELETE_DIALOG( m_pDLGFindUser );
	m_pDLGFindUser = new CKTDGUIDialog( this, L"DLG_Find_User.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFindUser );

	
	CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)m_pDLGFindUser->GetControl( L"ComboBoxUser_Search_Window" );

	if ( g_pData->GetMessenger() != NULL && pComboBox != NULL )
	{
		vector< wstring > vecFriendNameList;
		g_pData->GetMessenger()->GetFriendNameList( vecFriendNameList );
		for ( int i = 0; i < (int)vecFriendNameList.size(); i++ )
		{
			wstring friendName = vecFriendNameList[i];
			pComboBox->AddItem( friendName.c_str(), NULL );
		}
	}
	
	//{{ 2007. 11. 13  ������  �Ⱦ��� �������
	//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_FINDUSER_NOT );
	//}}
}

void CX2StateMenu::SetShowDlgMenu(bool val)
{	
#ifdef REFORM_UI_SKILLSLOT
	if( val )
	{
		if ( m_pDLGMenu != NULL )
			m_pDLGMenu->SetShowEnable(m_bShowMenu,m_bShowMenu);

		if( NULL != g_pData && NULL != g_pData->GetUIManager() )
		{
#ifdef RIDING_SYSTEM
			if( NULL != CX2RidingPetManager::GetInstance()->GetMyRidingPet() )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, false);
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_RIDING_SKILL_SLOT, g_pData->GetUIManager()->GetShowRidingPetSkillslot());
			}
			else
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, g_pData->GetUIManager()->GetShowSkillslot());
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_RIDING_SKILL_SLOT, false);
			}
#else
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, g_pData->GetUIManager()->GetShowSkillslot() );
#endif //RIDING_SYSTEM
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, g_pData->GetUIManager()->GetShowQuickSlot() );
		}
	}
	else
	{
		if ( m_pDLGMenu != NULL )
			m_pDLGMenu->SetShowEnable(false,false);

		if( NULL != g_pData && NULL != g_pData->GetUIManager() )
		{
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, false);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, false);
#ifdef RIDING_SYSTEM
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_RIDING_SKILL_SLOT, false);
#endif //RIDING_SYSTEM
		}
	}
#else
	if ( m_pDLGMenu != NULL )
	{
		m_pDLGMenu->SetShow( val );
		m_pDLGMenu->SetEnable( val );
	}
	// 2009.01.29 ���¿� : ������ SetShow Ÿ�̹� �ٲ�
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, val);
#endif //REFORM_UI_SKILLSLOT
} 

void CX2StateMenu::SetShowMenu()
{	
	if ( m_pDLGMenu != NULL )
	{
		m_pDLGMenu->SetShow( true );
		m_pDLGMenu->SetEnable( true );
	}
	// 2009.01.29 ���¿� : ������ SetShow Ÿ�̹� �ٲ�
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, true);
}



void CX2StateMenu::OnClickedMenuSkillButton()
{
	// ��ų Ʈ���� �޺� Ʈ�� �Ѵ� ���ÿ� �����ִ� ���� ����� �Ѵ�
	ASSERT( false == ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) && g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_COMBO_TREE ) ) );
	if( false == g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) &&
		false == g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_COMBO_TREE ) )
	{
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
	}
	else 
	{
		if( true == g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) )
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
		}

		if( true == g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_COMBO_TREE ) )
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMBO_TREE );
		}
	}
}

bool CX2StateMenu::ShortCutKeyProcess()
{


	if ( CX2State::ShortCutKeyProcess() == true )
		return true;

#ifdef ALLOW_CAMERA_ZOOM_BY_TRAINING		/// ���� �Ʒÿ��� ī�޶� �� ����� �� �ֵ��� ����
#ifndef _IN_HOUSE_	/// �系������ ����
	if( NULL != g_pMain && true == g_pMain->GetIsPlayingFreeTraining() )
	{
		CameraZoomInAndOutKeyProcess();
		return true;		/// ī�޶� �� ��� �˻� ��, ���ο� ������� �ٷ� Ż��
	}
#endif _IN_HOUSE_
#endif ALLOW_CAMERA_ZOOM_BY_TRAINING

	//��ų

	bool bHideDialog = false;
#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		bHideDialog = true;
#endif


#ifdef DUNGEON_SELECT_SKILLTREE_LOCK
	if(NULL != g_pMain && NULL != g_pMain->GetPartyUI() && false == g_pMain->GetPartyUI()->GetShowLocalMap())
#endif DUNGEON_SELECT_SKILLTREE_LOCK
	if( bHideDialog == false && GET_KEY_STATE( GA_SKILL_TREE ) == TRUE )
	{	
		OnClickedMenuSkillButton();
		return true;
	}



	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F5 ) == TRUE )
	{
		if( g_pSquareGame == NULL )
		{
			CX2GameOption* pGameOption = &g_pMain->GetGameOption();
			pGameOption->SetFieldParty( !pGameOption->GetFieldParty() );
		}		
	}

	// temp!!! ������ NPC ���̱�, �Ⱥ��̱�
	//if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
	//{
	//	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F11 ) == TRUE )
	//	{
	//		if( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() &&
	//			NULL != g_pTFieldGame )
	//		{
	//			g_pTFieldGame->ToggleEnableNPC();
	//		}
	//	}
	//}


	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F12) == TRUE 
#ifdef SIMPLE_BUG_FIX
		&& false == g_pMain->GetIsPlayingTutorial()
#endif //SIMPLE_BUG_FIX
		)
	{
		if(false == m_Book.GetShow())
		{
			m_Book.SetBookTable( "BT_ELLIOS_GUIDE");
			m_Book.SetShow(true);
		}
	}

	// ���Ӱ��̵� on/off
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F1) == TRUE )
	{
		wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
		D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

		g_pMain->GetGameOption().SetPlayGuide( !g_pMain->GetGameOption().GetOptionList().m_bPlayGuide );
		g_pMain->GetGameOption().SaveScriptFile();
		if( NULL !=g_pData && NULL != g_pData->GetPlayGuide() && NULL != g_pData->GetPlayGuide()->GetDlgPlayGuide())
		{
			bool bShow = g_pMain->GetGameOption().GetOptionList().m_bPlayGuide;
			g_pData->GetPlayGuide()->GetDlgPlayGuide()->SetShowEnable(bShow, bShow);
		}
		if( m_bIsOptionWindowOpen == true )
		{
			InitOtherOption();
		}

		if( true == g_pMain->GetGameOption().GetOptionList().m_bPlayGuide )
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_567 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}
		}
		else
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_568 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}
		}	

		return true;
	}

	// ��ĳ�� SDĳ���� ǥ��
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
		{
			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F6 ) == TRUE )
			{
				CX2GameOption* pGameOption = &g_pMain->GetGameOption();
				if( NULL != pGameOption )
					pGameOption->SetFieldSD( !pGameOption->GetFieldSD() );
			}

			if ( CameraZoomInAndOutKeyProcess() )
				return true;
		} break;
	}
	// ����Űó��
	return g_pMain->KeyProcess();
}

void CX2StateMenu::ChatProcess()
{
}

void CX2StateMenu::RefreshMenuInfo()
{
	if ( m_pDLGMenu != NULL )
	{
		UpdateExpGageBar();		
		CKTDGUIStatic* pStatic;
		pStatic = (CKTDGUIStatic*)m_pDLGMenu->GetControl( L"StaticMenu_Button_Tired_Gage" );
		float gauagePercentage = (g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax );

		if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
		{
			float _width = (g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax ) * pStatic->GetPicture(0)->GetOriginalSize().x;
			pStatic->GetPicture(0)->SetSizeX( _width );
		}

		//���콺 ���� �� �ټ��� �ۼ�Ʈ
		CKTDGUIButton* pSpiritInvisibleButton = static_cast< CKTDGUIButton* >(m_pDLGMenu->GetControl( L"Button_Invisible_SpiritGuage" ));
		if( NULL != pSpiritInvisibleButton )
		{
			WCHAR buff[256] = L"";
#ifdef RESTORE_SPIRIT_VALUE_BUG_FIX
			float fNowSpirit = static_cast<float>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit );
			fNowSpirit = fNowSpirit / static_cast<float>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax ) * 100.0f;
			fNowSpirit = floor(fNowSpirit * 10.0f + 0.5f) / 10.0f;
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%.1f%%", fNowSpirit );
#else
			int iSpirit = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit;
			float fMaxSpirit = (float)(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax);
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%.1f%%", iSpirit / fMaxSpirit  * 100.f );
#endif RESTORE_SPIRIT_VALUE_BUG_FIX
			pSpiritInvisibleButton->SetGuideDesc(buff);
			pSpiritInvisibleButton->SetShowEnable(true,true);
		}
		//}}
		if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
		{
			g_pData->GetUIManager()->GetUIInventory()->SetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED );
			g_pData->GetUIManager()->GetUIInventory()->SetAPString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint );
		}
		
	}
}


void CX2StateMenu::UpdateExpGageBar()
{
	if( NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return; 
	
//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ����ġ ȹ��
#ifdef REAL_TIME_ELSWORD	

	float	nowExp		= static_cast< float >( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP );
	int		iOrgLevel	= g_pData->GetSelectUnitLevel();			// ĳ���� ������ ����� level (Level up ��)	


//{{ ������ // 2012-04-16 // UI����_�ϴܸ޴�

	float fTotalExpRate		= 0.0f;
	float nowBaseExp		= 0;

	nowBaseExp		= static_cast< float >( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NowBaseLevelEXP );
	fTotalExpRate	= ( nowExp - nowBaseExp ) / g_pData->GetEXPTable()->GetEXPData( iOrgLevel + 1 ).m_nNeedExp;

	if( fTotalExpRate > 1.f )
	{
		fTotalExpRate = 1.f;
	}
	else if( fTotalExpRate < 0.f )
	{
		fTotalExpRate = 0.f;
	}

	CKTDGUIStatic* pStatic;
	//EXP������
	pStatic = static_cast< CKTDGUIStatic* >( m_pDLGMenu->GetControl( L"StaticMenu_Button_EXP_Gauge" ) );

	if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
	{
		pStatic->GetPicture(0)->SetSizeX( fTotalExpRate * pStatic->GetPicture(0)->GetOriginalSize().x );
	}
	
	//���콺 ���� �� ����ġ �ۼ�Ʈ
	CKTDGUIButton* pExpInvisibleButton = static_cast< CKTDGUIButton* >(m_pDLGMenu->GetControl( L"Button_Invisible_ExpGuage" ));
	if( NULL != pExpInvisibleButton )
	{
		WCHAR buff[256] = L"";
		StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%.1f%%", fTotalExpRate * 100.f );
		pExpInvisibleButton->SetGuideDesc(buff);
		pExpInvisibleButton->SetShowEnable(true, true);
	}
	//}}
//}} ������ // 2012-04-16 // UI����_�ϴܸ޴�
#else	REAL_TIME_ELSWORD	
	float nowExp		= (float) g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
	float fGainedEXP	= 0.f;
	if( NULL != g_pX2Game )		// ���� �÷��� ���϶��� ����
	{
		fGainedEXP = (float) g_pX2Game->GetGetEXPInGame();
	}

	int iOrgLevel = g_pData->GetSelectUnitLevel();			// ĳ���� ������ ����� level (Level up ��)
	int iNowLevel = g_pData->GetEXPTable()->GetLevel( (int)(nowExp + fGainedEXP) );			// Level up �ÿ� iOrgLevel �� �޶���


	//����
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMenu->GetControl( L"StaticMenu_Button_LV_Num" );
	if ( pStatic != NULL && pStatic->GetString(0) != NULL )
	{
		pStatic->GetString(0)->msg = g_pMain->GetEDString( iNowLevel );
	}



	float fOrgExpRate		= -1;
	float fGainedExpRate	= -1;
	float fTotalExpRate		= -1;



	if( iNowLevel != iOrgLevel )				// ���� ���� ���� ���
	{
		CX2EXPTable::EXPData nowExpData = g_pData->GetEXPTable()->GetEXPData( iNowLevel );
		CX2EXPTable::EXPData nextExpData = g_pData->GetEXPTable()->GetEXPData( iNowLevel+1 );
		if( nextExpData.m_Level == 0 )
		{
			// fix!! ������ ����???
			nextExpData.m_nNeedExp = nowExpData.m_nNeedExp;
		}

		fGainedEXP		= fGainedEXP + nowExp - nowExpData.m_nTotalExp;

		fOrgExpRate		= 0.f;
		fGainedExpRate	= fGainedEXP / nextExpData.m_nNeedExp;
		fTotalExpRate	= fGainedExpRate;
	}
	else										// ����ġ�� �ö��� ��(���� ���â or quest complete)
	{

		float nowBaseExp	= (float) g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NowBaseLevelEXP;
		float nextBaseExp	= (float) g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NextBaseLevelEXP;

		fOrgExpRate	= ( nowExp - nowBaseExp ) / ( nextBaseExp - nowBaseExp );
		if( fOrgExpRate > 1.f )
		{
			fOrgExpRate = 1.f;
		}
		else if( fOrgExpRate < 0.f )
		{
			fOrgExpRate = 0.f;
		}


		fGainedExpRate = (float) fGainedEXP / ( nextBaseExp - nowBaseExp );
		fTotalExpRate	= (float) ( nowExp - nowBaseExp + fGainedEXP ) / ( nextBaseExp - nowBaseExp );
		if( fTotalExpRate > 1.f )
		{
			fTotalExpRate = 1.f;
			fTotalExpRate = fTotalExpRate - fOrgExpRate;
		}
		else if( fTotalExpRate < 0.f )
		{
			fTotalExpRate = 0.f;
			fTotalExpRate = 0.f;
		}
	}

	//EXP������
	pStatic = (CKTDGUIStatic*)m_pDLGMenu->GetControl( L"StaticMenu_Button_EXP_Gauge" );
	if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
	{
		pStatic->GetPicture(0)->SetSizeX( fTotalExpRate * pStatic->GetPicture(0)->GetOriginalSize().x );
	}

	//{{09.06.09 �¿� : ���콺 �����ϸ� ����ġ �����ֱ�
	CKTDGUIButton* pExpInvisibleButton = static_cast< CKTDGUIButton* >( m_pDLGMenu->GetControl( L"Button_Invisible_ExpGuage" ));
	if( NULL != pExpInvisibleButton )
	{
		WCHAR buf[256] = L"";
		int iNowEXP = static_cast<int>(nowExp) - static_cast<int>(nowBaseExp) + static_cast<int>(fGainedEXP);
		StringCchPrintf( buf, 256, L" %d / %d ", iNowEXP), g_pData->GetEXPTable()->GetEXPData( iNowLevel+1 ).m_nNeedExp );
		//wsprintf( buf, L" %d / %d ", (int)(nowExp - nowBaseExp + fGainedEXP), g_pData->GetEXPTable()->GetEXPData( iNowLevel+1 ).m_nNeedExp );
		pExpInvisibleButton->SetGuideDesc(buf);
	}
	//}}


	//����ġ �ۼ�Ʈ
	pStatic = (CKTDGUIStatic*)m_pDLGMenu->GetControl( L"StaticMenu_Button_EXP" );
	if ( pStatic != NULL && pStatic->GetString(0) != NULL )
	{
		WCHAR buff[256] = L"";
		StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%.1f", fTotalExpRate * 100.f );
		pStatic->GetString(0)->msg = buff;
	}
#endif REAL_TIME_ELSWORD
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ����ġ ȹ��
}


void CX2StateMenu::ToggleCashShop()
{
#ifdef FIELD_BOSS_RAID // ĳ�ü� ���� ����
	CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
	if( true == battleFieldManager.GetIsBossRaidCurrentField() )
	{
		g_pChatBox->AddChatLog(  GET_STRING( STR_ID_28379 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		return;
	}
#endif // FIELD_BOSS_RAID

#ifdef FIX_CASH_SHOP_ENTER_BUG
	if( NULL != g_pData->GetPartyManager() &&
		true == g_pData->GetPartyManager()->GetProcessPvpMatch() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_13599 ), this );
		return;
	}
#endif // FIX_CASH_SHOP_ENTER_BUG

#ifdef CASH_ITEM_REFUND
	CX2State* pState = static_cast< CX2State* >(g_pMain->GetNowState());
	if(pState->GetCursor() != NULL)
		pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
#endif CASH_ITEM_REFUND



	if( NULL != g_pData->GetUIManager() &&
		(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_TRADE) ||
		true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP)) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_659 ), g_pMain->GetNowState() );
		return;
	}

	// ü�� ���̵� ���� 
	if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
		return;
	}

    ASSERT( m_pCashShop != NULL );
    if( m_pCashShop == NULL )
    {
        return;
    }//if

	if ( m_pCashShop->GetOpen() == false )
	{
		//Open!!

		// ���ӳ����� �ٻ� ������ ��� ĳ�ü��� �� �� ����
		if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
			return ;

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
		IF_EVENT_ENABLED( CEI_CASH_SHOP_CATEGORY_EVENT_2_SHOW )
		{
			m_pCashShop->ShowEventCategoryInCashShop();
		}
		ELSE
		{
			m_pCashShop->OffEventCategoryInCashShop();
		}
#endif ADD_CASH_SHOP_CATEGORY_EVENT_2

		//{{ kimhc // 2009-12-19 // �ٸ� �������� �����ؼ� ĳ�ü� ��ư Ŭ���� �ٸ� ����Ʈ �޾ƿ�����
#ifdef	ADD_SERVER_GROUP
		if ( g_pInstanceData->GetServerGroupIDCashShop() != g_pInstanceData->GetServerGroupID() )
		{
			g_pData->GetItemManager()->ClearCashItemList();
			g_pInstanceData->SetServerGroupIDCashShop( g_pInstanceData->GetServerGroupID() ); 
		}
#endif	ADD_SERVER_GROUP
		//}} kimhc // 2009-12-19 // �ٸ� �������� �����ؼ� ĳ�ü� ��ư Ŭ���� �ٸ� ����Ʈ �޾ƿ�����

#if	defined(SERV_EVENT_CHUNG_GIVE_ITEM) || defined( ALWAYS_EVENT_ADAMS_UI_SHOP )
		SetUsedCashShop(true);
#endif SERV_EVENT_CHUNG_GIVE_ITEM

		if ( g_pData->GetItemManager()->GetIsUpdateCashItem() == false )
		{
#ifdef SERV_NAVER_CHANNELING
			switch ( g_pData->GetMyUser()->GetUserData().m_uChannelCode )
			{
			case KNexonAccountInfo::CE_NAVER_ACCOUNT:
				{
					if ( NULL != g_pInstanceData && 
						g_pInstanceData->GetNaverAccessToken().empty() )
						g_pData->GetServerProtocol()->SendID( EGS_GET_NAVER_ACCESS_TOKEN_REQ );
				} break;

			default:
				break;
			}
#endif // SERV_NAVER_CHANNELING

#ifdef SERV_GLOBAL_BILLING
			Handler_EGS_BILL_PRODUCT_INFO_REQ();
#else
			Handler_EGS_CASH_PRODUCT_INFO_REQ();
#endif SERV_GLOBAL_BILLING
		}
		else
		{
#ifdef SERV_GLOBAL_BILLING
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_CHECK_BALANCE_REQ();
#endif SERV_GLOBAL_BILLING
			m_pCashShop->SetOpen( true );
			ChangeByToggleCashShop();
		}
#ifdef NEW_VILLAGE_UI
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, false);
#ifdef REFORM_UI_SKILLSLOT
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, false);
#endif //REFORM_UI_SKILLSLOT
#endif
#ifdef RIDING_SYSTEM
		if( NULL != g_pData->GetUIManager() )
			g_pData->GetUIManager()->SetShowRidingPetSkillSlot(false);
#endif //RIDING_SYSTEM
	}
	else
	{
#ifdef CJ_ID_WEB_BILLING
		g_pMain->GetNMBrowserWrapper()->DestroySSOBrowser();
#endif //CJ_ID_WEB_BILLING
		//close!!
		m_pCashShop->SetOpen( false );
		ChangeByToggleCashShop();
#ifdef NEW_VILLAGE_UI
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUICK_SLOT, true);
#ifdef REFORM_UI_SKILLSLOT
#ifdef RIDING_SYSTEM
		if( NULL != g_pData->GetUIManager() && NULL != CX2RidingPetManager::GetInstance() && true == CX2RidingPetManager::GetInstance()->GetRidingOn() )
		{
			g_pData->GetUIManager()->SetShowRidingPetSkillSlot(true);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, false );
		}
		else
		{
			g_pData->GetUIManager()->SetShowRidingPetSkillSlot(false);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, true );
		}
#else
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, true );
#endif //RIDING_SYSTEM
#endif //REFORM_UI_SKILLSLOT
#endif
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		SetUsedCashShop(false);
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	if( g_pInstanceData != NULL )
		{
			g_pInstanceData->SetAdamsEventShopUIShow(false);
		}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	}
}

void CX2StateMenu::ChangeByToggleCashShop()
{
    ASSERT( m_pCashShop != NULL );
    if( m_pCashShop == NULL )
    {
        return;
    }

	SetShowStateDLG( !m_pCashShop->GetOpen() );

	if ( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->SetOpen( false );



	g_pData->GetUIManager()->CloseAll();

}

void CX2StateMenu::CheckPresentArrival()
{
	PresentArrival presentArrival;
	if ( m_pDLGPresentArrival == NULL && g_pMain->GetPresentArrival( presentArrival ) == true )
	{
		m_pDLGPresentArrival = new CKTDGUIDialog( this, L"DLG_Present_Arrival.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPresentArrival );

#ifdef SERV_GLOBAL_BILLING
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( presentArrival.m_kBillProductInfo.m_iProductID );
#else // SERV_GLOBAL_BILLING
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( _wtoi( presentArrival.m_KNXBTProductInfo.m_wstrProductID.c_str() ) );
#endif // SERV_GLOBAL_BILLING
		if ( pItemTemplet != NULL )
		{
			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGPresentArrival->GetControl( L"g_pStaticCashShop_PresentWindow_Accept_Slot" );
			if ( pStatic != NULL && pStatic->GetPicture( 0 ) != NULL )
			{
				pStatic->GetPicture( 0 )->SetTex( 
                    pItemTemplet->GetShopImage()
                    );
			}

			pStatic = (CKTDGUIStatic*)m_pDLGPresentArrival->GetControl( L"g_pStatic_Item_ID_1Line" );
			if ( pStatic != NULL && pStatic->GetString(0) != NULL )
			{
                wstring tempName = pItemTemplet->GetName();
				wstringstream wstrstm;

#ifdef SERV_GLOBAL_BILLING
				if ( presentArrival.m_kBillProductInfo.m_cPeriod != 0 )
				{
					wstrstm << L" (" << static_cast<int>(presentArrival.m_kBillProductInfo.m_cPeriod) << GET_STRING( STR_ID_14 ) << L")"; 
				}
				else
				{
					wstrstm << L" (" << static_cast<int>(presentArrival.m_kBillProductInfo.m_cQuantity) << GET_STRING( STR_ID_24 ) << L")";
				}
#else // SERV_GLOBAL_BILLING
				if ( presentArrival.m_KNXBTProductInfo.GetProductExpire() != 0 )
				{
					wstrstm << L" (" << presentArrival.m_KNXBTProductInfo.GetProductExpire() << GET_STRING( STR_ID_14 ) << L")"; 
				}
				else
				{

					wstrstm << L" (" << presentArrival.m_KNXBTProductInfo.m_usProductPieces << GET_STRING( STR_ID_24 ) << L")";
				}
#endif // SERV_GLOBAL_BILLING
				tempName += wstrstm.str().c_str();				
#ifdef CLIENT_GLOBAL_LINEBREAK
				tempName = CWordLineHandler::GetStrByLineBreakInX2Main( tempName.c_str(), 214, pStatic->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
				tempName = g_pMain->GetStrByLienBreak( tempName.c_str(), 214, pStatic->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
				pStatic->SetString( 0, tempName.c_str() );
			}
		}


		CKTDGUIStatic* pStaticMsg = (CKTDGUIStatic*)m_pDLGPresentArrival->GetControl( L"Present_Message" );
		if ( pStaticMsg != NULL )
		{
			pStaticMsg->SetString( 0, presentArrival.m_PresentMsg.c_str() );
		}

		pStaticMsg = (CKTDGUIStatic*)m_pDLGPresentArrival->GetControl( L"User_Name" );
		if ( pStaticMsg != NULL  )
		{
			pStaticMsg->SetString( 0, GET_REPLACED_STRING( ( STR_ID_672, "L", presentArrival.m_SenderNickName ) ) );
		}
	}
}


bool CX2StateMenu::ToggleSystemMenu(bool bShow)
{
	// bShow�� true�� �ڵ� ���
	// bShow�� false�� ������ ����.

	if(bShow == true)
	{	
		m_bShowSystemMenu = !m_bShowSystemMenu;

		if ( m_bShowSystemMenu == true )
		{
			if(m_pDLGSystem == NULL)
			{			
				//SAFE_DELETE_DIALOG( m_pDLGSystem );
#if defined( SERV_SECOND_SECURITY ) && !defined( SERV_DISABLE_SECOND_SECURITY )
	#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		#ifdef SHOW_WEB_ADVERTISEMENT_USE_DEFAULT_BROWSER
				m_pDLGSystem = new CKTDGUIDialog( this, L"DLG_UI_System_URLEvent.lua", 0.1f);
		#else
			#ifdef SERV_CHANNELING_AERIA
				if( true == g_pInstanceData->IsAeriaChanneling() )
					m_pDLGSystem = new CKTDGUIDialog( this, L"DLG_UI_System_New_For_Aeria.lua", 0.1f);
				else
			#endif //SERV_CHANNELING_AERIA
				m_pDLGSystem = new CKTDGUIDialog( this, L"DLG_UI_Entry_Point_System_New.lua", 0.1f);
		#endif SHOW_WEB_ADVERTISEMENT_USE_DEFAULT_BROWSER
	#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
				m_pDLGSystem = new CKTDGUIDialog( this, L"DLG_UI_System_New.lua", 0.1f);
	#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh			
#else
				m_pDLGSystem = new CKTDGUIDialog( this, L"DLG_UI_System.lua", 0.1f);
#endif //defined( SERV_SECOND_SECURITY ) && !defined( SERV_DISABLE_SECOND_SECURITY )
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSystem );	
				m_pDLGSystem->SetDisableUnderWindow(true);
				
				//m_pDLGSystem->SetLayer(XDL_POP_UP);
				//m_pDLGSystem->SetFront(true);
				//m_pDLGSystem->SetPos(D3DXVECTOR2(434, 256));
			}
			else
			{
				m_pDLGSystem->SetShowEnable(true, true);				
				m_pDLGSystem->RequestFocus( m_pDLGSystem->GetControl(L"SystemMenuOption") );
				m_pDLGSystem->GetControl(L"SystemMenuOption")->OnFocusIn();
			}
		}		
		else
		{
			m_pDLGSystem->SetShowEnable(false, false);
			//SAFE_DELETE_DIALOG( m_pDLGSystem );
		}
	}
	else
	{
		if(m_bShowSystemMenu == false)
			return true;

		m_bShowSystemMenu = false;
		
		if(m_pDLGSystem != NULL)
		{
			m_pDLGSystem->SetShowEnable(false, false);
			//SAFE_DELETE_DIALOG( m_pDLGSystem );		
		}		
	}	
	
	// 09.03.06 ���¿� : �ý���â ������ ��Ƽâ disable
	if(g_pMain->GetPartyUI() != NULL)
	{
		g_pMain->GetPartyUI()->EnableDialog( !m_bShowSystemMenu );
	}
	
	return true;
}



//{{ 09.03.24 ���¿� : �� CharacterRoom���� �̻�� �ڵ�
void CX2StateMenu::CheckMyInfoChange()
{
	if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() &&
		true == g_pData->GetUIManager()->GetUIInventory()->GetShow() )
	{
		if ( m_TotalED < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
		{
			int changeED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED - m_TotalED;
			wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
			wstrstm << GET_STRING( STR_ID_14363 ) << changeED;
#else
			wstrstm << L"ED +" << changeED;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
			m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 789,687,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
		}

		if ( m_TotalED > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
		{
			int changeED = m_TotalED - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
			wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
			wstrstm << GET_STRING( STR_ID_14364 ) << changeED;
#else
			wstrstm << L"ED -" << changeED;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
			m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 789,687,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
		}

		m_TotalED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;

		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����
	#ifdef	REAL_TIME_ELSWORD
		// ���� ��Ż�� ED�� �г�Ƽ�� ����� �޴� ȿ���� �����ֱ����� �뵵
		if ( g_pInstanceData != NULL &&
			g_pInstanceData->GetInstanceED() != 0 )
		{
			if( g_pInstanceData->GetInstanceED() > m_TotalED )	
			{
				int changeED = g_pInstanceData->GetInstanceED() - m_TotalED;
				wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		wstrstm << GET_STRING( STR_ID_14364 ) << changeED;
#else
				wstrstm << L"ED -" << changeED;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
				m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 789,687,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );

				g_pInstanceData->SetInstanceED( 0 );
			}
			else
			{
				// oasis907 : ����� [2011.12.28] �� ���� ���. ED���Ƽ ���� ����( �����Ʒ� ) 
				g_pInstanceData->SetInstanceED( 0 );
			}
		}
	#endif	REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����
	}
	else
	{
		if( NULL != m_pPicCharMenuPlusInfo )
			m_pPicCharMenuPlusInfo->Clear();
		if( NULL != m_pPicCharMenuPlusInfoRed)
			m_pPicCharMenuPlusInfoRed->Clear();
	}
#ifdef SERV_PVP_NEW_SYSTEM
	if ( m_TotalAP < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint )
	{
		int changeAP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint - m_TotalAP;
		wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		wstrstm << GET_STRING( STR_ID_14367 ) << changeAP;
#else
		wstrstm << L"AP +" << changeAP;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 940,687,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
	}

	if ( m_TotalAP > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint )
	{
		int changeAP = m_TotalAP - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint;
		wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		wstrstm << GET_STRING( STR_ID_14368 ) << changeAP;
#else
		wstrstm << L"AP -" << changeAP;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 940,687,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
	}
	m_TotalAP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAPoint;
#else
	if ( m_TotalVP < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
	{
		int changeVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint - m_TotalVP;
		wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		wstrstm << GET_STRING( STR_ID_14367 ) << changeVP;
#else
		wstrstm << L"VP +" << changeVP;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 969,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
	}

	if ( m_TotalVP > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
	{
		int changeVP = m_TotalVP - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
		wstringstream wstrstm;
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		wstrstm << GET_STRING( STR_ID_14368 ) << changeVP;
#else
		wstrstm << L"VP -" << changeVP;
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 969,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
	}
	m_TotalVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
#endif

	if ( m_TotalLevel < g_pData->GetSelectUnitLevel() )
	{

		D3DXVECTOR2 vPos(698, 730);

		for ( int i = 0; i < 3; i++ )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
			pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"HyperBallTick", 
				vPos.x, vPos.y,
				0.0f, 1000,1000, -1, 10 );
			if( pSeq != NULL )
			{
				pSeq->SetOverUI( true );
				pSeq->SetBlackHolePosition( pSeq->GetPosition() );
				pSeq->UseLookPoint( true );
				pSeq->SetLookPoint( pSeq->GetPosition() );
			}
			pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"HyperBallRing", 
				vPos.x, vPos.y,
				0.0f, 1000,1000, -1, 1 );
			if( pSeq != NULL )
				pSeq->SetOverUI( true );
		}
#ifdef _NEXON_KR_
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
		{
			CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass << 24 ) | (UINT32)g_pData->GetSelectUnitLevel(), kUserFlag_GeneralLevelUp );
		}
#endif
	}

	m_TotalLevel = g_pData->GetSelectUnitLevel();


}
//}}

#ifdef	SERV_INSERT_GLOBAL_SERVER
void CX2StateMenu::AddStringRemainTime( OUT wstringstream& wstreamDesc_, IN const int iRemainTimeWorldBuff_ )
{
	wstreamDesc_ << '\n' << '\n';
	wstreamDesc_ << GET_STRING( STR_ID_11537 );

	// 60�� ���� ũ�� ������ ǥ��
	if ( 60 < iRemainTimeWorldBuff_ )
	{
		wstreamDesc_ << iRemainTimeWorldBuff_ / 60 << GET_STRING( STR_ID_311  );
	}
	// 60�� ���� ������ �ʷ� ǥ��
	else
	{
		wstreamDesc_ << iRemainTimeWorldBuff_ << GET_STRING( STR_ID_180  );
	}

	wstreamDesc_ << '\n';
}
#endif	SERV_INSERT_GLOBAL_SERVER
//}} kimhc // 2011-04-22 // ���潺���� ���� ���� UI

//{{ JHKang / ������ / 2011.6.26 / �����̾� ����
#ifdef COME_BACK_REWARD
/** @function 	: UpdateGuideDescPremiumBuffUI
	@brief 		: �����̾� ����(���콺 �� �̺�Ʈ �� ����)
	@param		: iRemainTimeBuff_(���� �ð�)
*/
void CX2StateMenu::UpdateGuideDescPremiumBuffUI()
{
	if ( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )
		return;

#ifdef BUFF_TEMPLET_SYSTEM
	if ( g_pData->GetPremiumBuffManager()->GetPremiumBuffList().empty() == false )
	{
		BOOST_FOREACH( CX2PremiumBuffIconPtr pPremiumBuffPtr , g_pData->GetPremiumBuffManager()->GetPremiumBuffList() )
		{
			if( pPremiumBuffPtr->GetPremiumBuffID() != 16 )
				UpdateGuideDescBuffTemplet( pPremiumBuffPtr );
		}
	}
#endif BUFF_TEMPLET_SYSTEM
}
#endif
//}}

//{{ kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
void CX2StateMenu::SetShowTooniLandEvent( const bool bShow_ )
{
	CKTDGUIButton* pButtonTooniLandEvent	= static_cast<CKTDGUIButton*>( m_pDLGMenu->GetControl( L"Button_TooniLand" ) );
	if ( NULL != pButtonTooniLandEvent )
		pButtonTooniLandEvent->SetShow( bShow_ );
}

void CX2StateMenu::UpdateGuideDescTooniLandEvent()
{
	CKTDGUIButton* pButtonTooniLandEvent = static_cast<CKTDGUIButton*>( m_pDLGMenu->GetControl( L"Button_TooniLand" ) );
	if ( NULL != pButtonTooniLandEvent )
	{
		if ( false == pButtonTooniLandEvent->GetEnable() )
			pButtonTooniLandEvent->SetEnable( true );

		pButtonTooniLandEvent->SetGuideDesc_LUA( STR_ID_13710 );
	}
}
#endif	SERV_TOONILAND_CHANNELING_EVENT
//}} kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ

bool CX2StateMenu::Handler_EGS_ADD_ON_STAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADD_ON_STAT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Game )
		return g_pX2Game->Handler_EGS_ADD_ON_STAT_ACK( kEvent );
	else
		return false;
}

bool CX2StateMenu::Handler_EGS_ADD_ON_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADD_ON_STAT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Game )
		return g_pX2Game->Handler_EGS_ADD_ON_STAT_NOT( kEvent );
	else
		return false;
}

//{{ 2010. 12. 23 ����ö	��� ���� ��Ŷ Ÿ��
#ifdef	SERV_CHANGE_EQUIPPED_PACKET_REFAC

bool CX2StateMenu::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2GUUser*	pUser = ( NULL != g_pX2Game ? g_pX2Game->GetUserUnitByUID( kEvent.m_UnitUID ) : NULL );
	if ( pUser == NULL )
		return false;

	CX2Unit* pUnit = pUser->GetUnit();
	if ( pUser->GetUnit() == NULL )
		return false;

	CX2Unit::UnitData*	pUnitData	= &pUnit->AccessUnitData();

	pUser->SetGameStat( kEvent.m_kGameStat );
	pUnitData->m_GameStat.SetKStat( kEvent.m_kGameStat );

	pUser->SetMaxHp( static_cast< const float >( kEvent.m_kGameStat.m_iBaseHP ) );
	if ( pUser->GetNowHp() > pUser->GetMaxHp() )
	{
		pUser->SetNowHp( pUser->GetMaxHp() );
	}

	switch ( kEvent.m_cChangeEquippedPacketType )
	{
		// Ÿ��Ʋ�� ���� �Ǿ��� ���
	case KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT::CEPT_CHANGE_TITLE:
		{
#ifdef SERV_TITLE_DATA_SIZE
			pUnit->SetTitleId( kEvent.m_iEquippedTitleID );
#else
			pUnit->SetTitleId( kEvent.m_sEquippedTitleID );
#endif
			pUser->UpdateEquippedEmblem();
			pUser->NotifyShowObjectChanged();
		} break;

		// ��� ���� �Ǿ��� ���
	case KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT::CEPT_CHANGE_EQUIP:
		{
			CX2Inventory& kInventory = pUnit->AccessInventory();

			const UidType myUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

			// ��Ƽ���̸�
			if ( myUID != kEvent.m_UnitUID )
			{
				BOOST_TEST_FOREACH( KInventoryItemInfo&, kInventoryItemInfo, kEvent.m_vecInventorySlotInfo )
				{
					CX2Item* pItemToRemove = kInventory.GetItem( static_cast< CX2Inventory::SORT_TYPE >( kInventoryItemInfo.m_cSlotCategory ), 
						kInventoryItemInfo.m_sSlotID );

					CX2Item* pItemToAdd = NULL;

					if ( kInventoryItemInfo.m_iItemUID > 0 )
					{
						CX2Item::ItemData kItemDataToAdd( kInventoryItemInfo );
						pItemToAdd = new CX2Item( kItemDataToAdd, pUnit ); 

						if ( pItemToAdd == NULL )
						{
							ASSERT( false );
							return false;
						} // if
					} // if

					// iventoryItemInfo.m_iItemUID �� 0�϶� pItemToAdd�� NULL�� �Ǵµ� �� ��쿣 ������ ��� ���� �����
					if ( pUser->UpdateEquipViewListInfo( pItemToAdd, pItemToRemove ) == false )
					{
						ASSERT( false );
						return false;
					} // if

					// ��� ����
					if ( pItemToRemove != NULL )
					{
						// ��Ƽ���� ��� �̰����� ó��(���� �ڽ��� ��쿡�� CXGUUser�� UpdateEquip���� ó��)
						g_pX2Game->RemoveSpecialAbilityInEquip( pItemToRemove->GetItemTemplet(), pUser );

						pUnit->RemoveEqip( pItemToRemove->GetUID() );						
						kInventory.RemoveItem( pItemToRemove->GetUID() );				
					} // if

					if ( pItemToAdd != NULL )
					{
						pUnit->AddEqip( pItemToAdd );
						kInventory.AddItem( static_cast< CX2Inventory::SORT_TYPE >( kInventoryItemInfo.m_cSlotCategory ),
							kInventoryItemInfo.m_sSlotID, pItemToAdd );

						g_pX2Game->SetSpecialAbilityInEquip( pItemToAdd->GetItemTemplet(), pUser );
					} // if
				} // BOOST

				//{{ kimhc // 2010.12.11 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
				pUser->SetShowViewList( true );
#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
				pUser->UpdateNotRenderingOtherEquipList();
#endif	NOT_RENDERING_OTHER_EQUIP_POS
				pUser->UpdateEquipCollisionData();

				pUser->SetEnableAttackBox( L"Rfoot", false );
				pUser->SetEnableAttackBox( L"Lfoot", false );
				pUser->SetEnableAttackBox( L"Lhand2", false );		
#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.12.11 //  2010-12-23 New Character CHUNG

				BOOST_TEST_FOREACH( KInventoryItemInfo&, kInventoryItemInfo, kEvent.m_vecInventorySlotInfo )
				{
					if ( kInventoryItemInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP &&
						( kInventoryItemInfo.m_sSlotID == _CONST_UIINVENTORY_::g_iEquippedNormalWeaponSlotID 
						|| kInventoryItemInfo.m_sSlotID == _CONST_UIINVENTORY_::g_iEquippedFashionWeaponslotID ) )
					{
						pUser->UpdateWeaponEnchantEffectAndExtraDamageType();
						break;
					} // if
				} // BOOST

			} // if

#ifdef MODIFY_AFTER_IMAGE
			pUser->UpdateAfterImageEquip();
#endif // MODIFY_AFTER_IMAGE
		} break;

	default:
		{
			bool bTest = false;
			bTest = true;
		}
		break;

	} // switch


	pUser->UpdateSocketDataAndEnchantData();
	pUser->UpdatePassiveAndActiveSkillState();

	pUser->ResetMaxMP();
	if( pUser->GetNowMp() > pUser->GetMaxMp() )
	{
		pUser->SetNowMp( pUser->GetMaxMp() );
	}

	return true;
}


#else	SERV_CHANGE_EQUIPPED_PACKET_REFAC
//{{AFX
bool CX2StateMenu::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2GUUser* pUser = m_pDungeonGame->GetUserUnitByUID( kEvent.m_UnitUID );
	if ( pUser == NULL )
		return false;

	CX2Unit* pUnit = pUser->GetUnit();
	if ( pUser->GetUnit() == NULL )
		return false;

	CX2Unit::UnitData*	pUnitData	= &pUnit->AccessUnitData();

	pUser->SetGameStat( kEvent.m_kGameStat );
	pUnitData->m_GameStat.SetKStat( kEvent.m_kGameStat );

	pUser->SetMaxHP( static_cast< const float >( kEvent.m_kGameStat.m_iBaseHP ) );
	if ( pUser->GetNowHp() > pUser->GetMaxHp() )
	{
		pUser->SetNowHp( pUser->GetMaxHp() );
	}

	if ( kEvent.m_vecInventorySlotInfo.empty() == true ) // Ÿ��Ʋ�� ���� �Ǿ��� ���
	{
#ifdef SERV_TITLE_DATA_SIZE
		pUnit->SetTitleId( kEvent.m_iEquippedTitleID );
#else
		pUnit->SetTitleId( kEvent.m_sEquippedTitleID );
#endif
		pUser->UpdateEquippedEmblem();
		pUser->NotifyShowObjectChanged();
	}
	else // ��� ���� �Ǿ��� ���
	{
		CX2Inventory& kInventory = pUnit->GetInventory();

		const UidType myUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

		// ��Ƽ���̸�
		if ( myUID != kEvent.m_UnitUID )
		{
			BOOST_TEST_FOREACH( KInventoryItemInfo&, kInventoryItemInfo, kEvent.m_vecInventorySlotInfo )
			{
				CX2Item* pItemToRemove = kInventory.GetItem( static_cast< CX2Inventory::SORT_TYPE >( kInventoryItemInfo.m_cSlotCategory ), 
					kInventoryItemInfo.m_sSlotID );

				CX2Item* pItemToAdd = NULL;

				if ( kInventoryItemInfo.m_iItemUID > 0 )
				{
					CX2Item::ItemData kItemDataToAdd( kInventoryItemInfo );
					pItemToAdd = new CX2Item( kItemDataToAdd, pUnit ); 

					if ( pItemToAdd == NULL )
					{
						ASSERT( false );
						return false;
					}
				}

				// iventoryItemInfo.m_iItemUID �� 0�϶� pItemToAdd�� NULL�� �Ǵµ� �� ��쿣 ������ ��� ���� �����
				//if ( pUser->UpdateEquip( &inventoryItemInfo, pItemToAdd ) == false )
				if ( pUser->UpdateEquipViewListInfo( pItemToAdd, pItemToRemove ) == false )
				{
					ASSERT( false );
					return false;
				}

				// ��� ����
				if ( pItemToRemove != NULL )
				{
					// ��Ƽ���� ��� �̰����� ó��(���� �ڽ��� ��쿡�� CXGUUser�� UpdateEquip���� ó��)
					g_pX2Game->RemoveSpecialAbilityInEquip( pItemToRemove->GetItemTemplet(), pUser );

					pUnit->RemoveEqip( pItemToRemove->GetUID() );						
					kInventory.RemoveItem( pItemToRemove->GetUID() );				
				}

				if ( pItemToAdd != NULL )
				{
					pUnit->AddEqip( pItemToAdd );
					kInventory.AddItem( static_cast< CX2Inventory::SORT_TYPE >( kInventoryItemInfo.m_cSlotCategory ),
						kInventoryItemInfo.m_sSlotID, pItemToAdd );

					g_pX2Game->SetSpecialAbilityInEquip( pItemToAdd->GetItemTemplet(), pUser );
				}
			}

			//{{ kimhc // 2010.12.11 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			pUser->SetShowViewList( true );
#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
			pUser->UpdateNotRenderingOtherEquipList();
#endif	NOT_RENDERING_OTHER_EQUIP_POS
			pUser->UpdateEquipCollisionData();

			pUser->SetEnableAttackBox( L"Rfoot", false );
			pUser->SetEnableAttackBox( L"Lfoot", false );
			pUser->SetEnableAttackBox( L"Lhand2", false );		
#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.11 //  2010-12-23 New Character CHUNG

			BOOST_TEST_FOREACH( KInventoryItemInfo&, kInventoryItemInfo, kEvent.m_vecInventorySlotInfo )
			{
				if ( kInventoryItemInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP &&
					( kInventoryItemInfo.m_sSlotID == _CONST_UIINVENTORY_::g_iEquippedNormalWeaponSlotID 
					|| kInventoryItemInfo.m_sSlotID == _CONST_UIINVENTORY_::g_iEquippedFashionWeaponslotID ) )
				{
					pUser->UpdateWeaponEnchantEffectAndExtraDamageType();
					break;
				}
			}
		}
	}

	pUser->UpdateSocketDataAndEnchantData();
	pUser->UpdatePassiveAndActiveSkillState();

	pUser->ResetMaxMP();
	if( pUser->GetNowMP() > pUser->GetMaxMp() )
	{
		pUser->SetNowMp( pUser->GetMaxMp() );
	}

	return true;
}
//}}AFX
#endif	SERV_CHANGE_EQUIPPED_PACKET_REFAC

bool CX2StateMenu::Hander_EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL == g_pX2Game )
	{
		ASSERT( g_pX2Game );
		return false;
	}

	CX2GUUser* pUser = g_pX2Game->GetUserUnitByUID( kEvent.m_UnitUID );

	if ( pUser == NULL ||
		pUser->GetUnit() == NULL )
		return false;

	CX2Unit* pUnit = pUser->GetUnit();
	CX2Unit::UnitData* pUnitData = &pUnit->AccessUnitData();


	pUser->SetGameStat( kEvent.m_kGameStat );
	pUnitData->m_GameStat.SetKStat( kEvent.m_kGameStat );

	for (size_t i = 0; i < kEvent.m_vecInventorySlotInfo.size(); i++ )
	{
		CX2Item* pItemToRemoveInView = pUser->GetUnit()->GetInventory().GetItem( kEvent.m_vecInventorySlotInfo[i].m_iItemUID );

		if ( pItemToRemoveInView == NULL ||
			pItemToRemoveInView->GetItemTemplet() == NULL )
		{
			ASSERT( pItemToRemoveInView );
			return false;
		}

		pItemToRemoveInView->AccessItemData().m_Endurance		= 0;

		g_pX2Game->RemoveSpecialAbilityInEquip( pItemToRemoveInView->GetItemTemplet(), pUser );
		//kEvent.m_vecInventorySlotInfo[i].m_iItemUID = 0;

		//if ( pUser->UpdateEquip( &kEvent.m_vecInventorySlotInfo[i] ) == false )
		if ( pUser->UpdateEquipViewListInfo( NULL, pItemToRemoveInView ) == false )
		{
			ASSERT( false );
			return false;
		}


		//{{ kimhc // 2010.12.11 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		pUser->SetShowViewList( true );
#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		pUser->UpdateNotRenderingOtherEquipList();
#endif	NOT_RENDERING_OTHER_EQUIP_POS
		pUser->UpdateEquipCollisionData();

		pUser->SetEnableAttackBox( L"Rfoot", false );
		pUser->SetEnableAttackBox( L"Lfoot", false );
		pUser->SetEnableAttackBox( L"Lhand2", false );		
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.11 //  2010-12-23 New Character CHUNG

		// kimhc // SetShowViewList�� �����ϱ� ���� ����
		if ( pItemToRemoveInView->GetItemTemplet()->GetEqipPosition() == CX2Unit::EP_WEAPON_HAND )
		{
			pUser->UpdateWeaponEnchantEffectAndExtraDamageType();
		}

		if( g_pChatBox != NULL)
		{
			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_UnitUID )		// ���� �ڽ��� ��
			{


				std::wstringstream strStream;
				strStream <<  GET_REPLACED_STRING( ( STR_ID_2639, "SS", pItemToRemoveInView->GetItemTemplet()->GetNameColor_(),
					pItemToRemoveInView->GetItemTemplet()->GetFullName_() ) );

				wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
				D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

				g_pChatBox->AddChatLog( strStream.str().c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
			}			
		}

		pItemToRemoveInView = NULL;	
	}

	pUser->UpdateSocketDataAndEnchantData();
	pUser->UpdatePassiveAndActiveSkillState();

	return true;
}

/*virtual*/ bool CX2StateMenu::Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAR_LEVEL_UP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL == g_pX2Game )
	{
		ASSERT( g_pX2Game );
		return false;
	}

	CX2GUUser*	pUser = g_pX2Game->GetUserUnitByUID( kEvent.m_iUnitUID );

	if ( pUser == NULL || 
		pUser->GetUnit() == NULL )
		return false;

	CX2Unit::UnitData* pUnitData = &pUser->GetUnit()->AccessUnitData();

#ifdef _NEXON_KR_
	if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID
		&& false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
	{
		CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass << 24 ) | (UINT32)kEvent.m_ucLevel, kUserFlag_GeneralLevelUp );
	}
#endif

	if ( pUser->GetNowHp() > 0 )
	{
		// ���� ����Ÿ ����
		pUnitData->m_GameStat.SetKStat( kEvent.m_kGameStat );
		pUnitData->m_Stat.SetKStat( kEvent.m_kBaseStat );

		pUnitData->m_Level					= kEvent.m_ucLevel;
		pUnitData->m_NowBaseLevelEXP		= g_pData->GetEXPTable()->GetEXPData( kEvent.m_ucLevel ).m_nTotalExp;
		pUnitData->m_NextBaseLevelEXP		= g_pData->GetEXPTable()->GetEXPData( kEvent.m_ucLevel + 1 ).m_nTotalExp;

		// ���� ����
		pUser->SetGameStat( kEvent.m_kGameStat );

		pUser->SetMaxHp( (float)kEvent.m_kGameStat.m_iBaseHP );



		pUser->SetNowHp( pUser->GetMaxHp() );
		pUser->SetNowMp( pUser->GetMaxMp() );
		pUser->GetUnit()->SetIsLevelUp( true );

		DisplayLevelUpEffect( pUser );

		//{{ kimhc // 2010.12.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		if ( CX2Unit::UT_CHUNG == pUser->GetUnit()->GetType() )
		{
			CX2GageData* pGageData = pUser->GetGageData();
			// û�� ��� ������ �ÿ� ĳ���� ���� �� �ش�.
			if ( NULL != pGageData )
			{
				CX2ChungGageData* pChungGageData = static_cast<CX2ChungGageData*>( pGageData );
				pChungGageData->SetNowCannonBallCount( pChungGageData->GetNowCannonBallCount() );
			}
		}
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		if ( CX2Unit::UT_ARA == pUser->GetUnit()->GetType() )
		{
			CX2GageData* pGageData = pUser->GetGageData();
			
			// �ƶ��� ��� ������ �ÿ� ����� ����
			if ( NULL != pGageData )
			{
				CX2AraGageData* pAraGageData = reinterpret_cast<CX2AraGageData*>( pGageData );
				pAraGageData->SetMaxForcePower( min( pUser->GetUnitLevel() / 10, 6 ) + 4 );
				pAraGageData->SetNowForcePower( pAraGageData->GetMaxForcePower() );
			}
		}
#endif

#ifdef SERV_ELESIS_UPDATE_EVENT
		ProcessElesisEvent( kEvent.m_ucLevel, kEvent.m_iNoteViewCount );
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef ITEM_IS_IN_INVENTORY_MSG_BY_LEVEL
		if( NULL != g_pData &&
			NULL != g_pData->GetMyUser() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			// �ƶ�ȥ�� ����, ���� �ӽ÷� ������ ó��, �ӽ÷� STR_ID_25066 ���
			if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( _CONST_ITEM_IN_INVENTORY_INT_::iRewardCube, false))
			{
				switch ( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_SQUARE_GAME:
				case CX2Main::XS_PVP_GAME:
				case CX2Main::XS_DUNGEON_GAME:
					{
						CX2Main::ReservedMessagePopUp EventPopUp;

						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_PVP_LOBBY );
						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_PVP_ROOM );
						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_DUNGEON_ROOM );
						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_SQUARE_GAME );
						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_TRAINING_SCHOOL );
						EventPopUp.m_vecTargetState.push_back( CX2Main::XS_BATTLE_FIELD );

						if( _CONST_ITEM_IN_INVENTORY_INT_::iGoalLevel <= kEvent.m_ucLevel)
						{
							EventPopUp.m_Message = GET_STRING( STR_ID_29335 );
							g_pMain->AddReservedMessagePopup( EventPopUp );
						}
					} break;
				default:
					{
						if( _CONST_ITEM_IN_INVENTORY_INT_::iGoalLevel <= kEvent.m_ucLevel)
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_29335 ), (CKTDXStage*)g_pMain->GetNowStateID() );
						}
					} break;
				}
			}
		}
#endif //ITEM_IS_IN_INVENTORY_MSG_BY_LEVEL

		if( g_pChatBox != NULL)
		{
			std::wstringstream strStream;
			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )		// ���� �ڽ��� ��
			{
				strStream << GET_REPLACED_STRING( ( STR_ID_2637, "i", kEvent.m_ucLevel ) );

				g_pX2Game->UpdateNpcNameColor( kEvent.m_ucLevel );

				if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
				{	//������ ���̵� ���
					g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_LEVEL_UP, true, pUnitData->m_Level);
				}
			}
			else
			{
				strStream << GET_REPLACED_STRING( ( STR_ID_2638, "Si", pUser->GetUnit()->GetNickName(), kEvent.m_ucLevel ) );
			}

			wstring wstrColor = L"#C4EB2D2";								// 78, 178, 210
			D3DXCOLOR coTextColor(0.30588f, 0.69803f, 0.823529f, 1.f);		// 78, 178, 210	

			g_pChatBox->AddChatLog( strStream.str().c_str(), KEGS_CHAT_REQ::CPT_TOTAL, coTextColor, wstrColor );		
		}
	}

	return true;
}

/*virtual*/ bool CX2StateMenu::Handler_EGS_DECREASE_ENDURANCE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DECREASE_ENDURANCE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL == g_pX2Game )
	{
		ASSERT( g_pX2Game );
		return false;
	}

	CX2GUUser*	pUser = g_pX2Game->GetMyUnit();

	if ( pUser == NULL ||
		pUser->GetUnit() == NULL )
		return false;

	const CX2Inventory& kInventory = pUser->GetUnit()->GetInventory();

	BOOST_TEST_FOREACH( const KDecreaseEnduranceInfo&, enduranceInfo, kEvent.m_vecEnduranceUpdate )
	{
		CX2Item* pItem = kInventory.GetItem( enduranceInfo.m_iItemUID );
		if ( NULL == pItem )
			continue;

		CX2Item::ItemData& kItemData = pItem->AccessItemData();

		kItemData.m_Endurance = enduranceInfo.m_iEndurance;

		if(g_pInstanceData != NULL && g_pInstanceData->GetMiniMapUI() != NULL)
		{
			g_pInstanceData->GetMiniMapUI()->UpdateEnduranceNotice();
		}
	}

	return true;
}

void CX2StateMenu::DisplayLevelUpEffect( CX2GUUser* pGUUser_ )
{
	if ( NULL != pGUUser_ )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUp.ogg", false, false );
		g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_LevelUp_LandPosition", pGUUser_ );		
		g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_LevelUp", pGUUser_ );
#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		g_pData->GetPicCharBlue()->DrawText( GET_STRING( STR_ID_14357 ), pGUUser_->GetHeadBonePos(), pGUUser_->GetDirVector(), CKTDGPicChar::AT_CENTER );
#else
		g_pData->GetPicCharBlue()->DrawText( L"LEVEL UP", pGUUser_->GetHeadBonePos(), pGUUser_->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		pGUUser_->GetUnit()->SetIsLevelUp( false );
	}
}

bool CX2StateMenu::CameraZoomInAndOutKeyProcess()
{
	if( GET_KEY_STATE( GA_ZOOMIN ) == TRUE
#ifndef REMOVE_DEFAULT_ZOOM
		|| g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ADD ) == TRUE
#endif REMOVE_DEFAULT_ZOOM
		)
	{
		g_pMain->GetGameOption().CameraZoomIn( 1 );
		return true;
	}
	else if( GET_KEY_STATE( GA_ZOOMOUT ) == TRUE
#ifndef REMOVE_DEFAULT_ZOOM
			 || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_SUBTRACT ) == TRUE
#endif REMOVE_DEFAULT_ZOOM
			 )
	{
		g_pMain->GetGameOption().CameraZoomIn( -1 );
		return true;
	}
	else
		return false;
}

void CX2StateMenu::ShowMainMenuButton()
{
	//���콺 ���� ���� ���� ���θ޴� �����ֵ��� ����
	if( false == m_bShowMainMenuButton )
	{
		m_bShowMainMenuButton = true;
		m_fMenuMaxRemainTime = 6.0f;
	}
}
void CX2StateMenu::UpdateMenuString()
{
	if( NULL == m_pDLGMenu )
		return;

	CKTDGUIStatic* pStaticMenu = static_cast<CKTDGUIStatic*>(m_pDLGMenu->GetControl(L"Static"));
	pStaticMenu->GetString(0)->msg = GET_STRING(STR_ID_19974);
	pStaticMenu->GetString(1)->msg = GET_STRING(STR_ID_19975);
		
	//�޴� ��ư
	wstring strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19976, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_INFO ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Charac")), strButtonDesc.c_str() );
	
	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19977, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_INVENTORY ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Inven")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19978, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_TREE ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Skill")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19979, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUEST ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Quest")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19980, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_PET ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Pet")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19981, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_GUILD ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Guild")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19982, "LL", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_COMMUNITY ), g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_FRIEND ) ));
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Comm")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_19983, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_OPTION ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"System")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_STRING(STR_ID_19984);
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Cash")), strButtonDesc.c_str() );

	strButtonDesc.clear();
	strButtonDesc = GET_STRING(STR_ID_19985);
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Guide")), strButtonDesc.c_str() );

#ifdef SERV_ADD_WARP_BUTTON
	strButtonDesc.clear();
	strButtonDesc = GET_REPLACED_STRING( ( STR_ID_24618, "L", g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_WARP ) ) );
	SetGuideString( static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Warp")), strButtonDesc.c_str() );
#endif // SERV_ADD_WARP_BUTTON
}
void CX2StateMenu::UpdateMenuPosistion()
{
	if( NULL == m_pDLGMenu )
		return;
	//���콺 ���� ���� �� ���θ޴� ��������� �ϱ� ���� ��ü ����	

	//�޴� ��ư
	CKTDGUIButton* pButtonChar = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Charac"));	//ĳ����
	CKTDGUIButton* pButtonInven = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Inven"));	//�κ��丮
	CKTDGUIButton* pButtonSkill = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Skill"));	//��ų
	CKTDGUIButton* pButtonQuest = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Quest"));	//����Ʈ
	CKTDGUIButton* pButtonPet = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Pet"));		//��
	CKTDGUIButton* pButtonGuild = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Guild"));	//���
	CKTDGUIButton* pButtonComm = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Comm"));		//Ŀ�´�Ƽ
	CKTDGUIButton* pButtonSystem = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"System"));	//�ý���
	CKTDGUIButton* pButtonCash = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Cash"));		//ĳ����
	CKTDGUIButton* pButtonGuide = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Guide"));		//���̵�
#ifdef SERV_ADD_WARP_BUTTON
	CKTDGUIButton* pButtonWarp = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Warp"));		//�̵���ư
#endif //SERV_ADD_WARP_BUTTON
#ifdef SERV_RECOMMEND_LIST_EVENT
	CKTDGUIButton* pButtonFriendRecommend = static_cast<CKTDGUIButton* >( m_pDLGMenu->GetControl( L"FriendRecommed" ) ); //��õ�ϱ�
#endif //SERV_RECOMMEND_LIST_EVENT
#ifdef SERV_4TH_ANNIVERSARY_EVENT
	CKTDGUIButton* pButtonEvent = static_cast<CKTDGUIButton*>(m_pDLGMenu->GetControl(L"Event"));	//�̺�Ʈ��ư
#endif SERV_4TH_ANNIVERSARY_EVENT


	if( true == m_vecpButtonMainMenu.empty() )
	{
		m_vecpButtonMainMenu.push_back(pButtonChar);
		m_vecpButtonMainMenu.push_back(pButtonInven);
		m_vecpButtonMainMenu.push_back(pButtonSkill);
		m_vecpButtonMainMenu.push_back(pButtonQuest);
		m_vecpButtonMainMenu.push_back(pButtonPet);
		m_vecpButtonMainMenu.push_back(pButtonGuild);
		m_vecpButtonMainMenu.push_back(pButtonComm);
		m_vecpButtonMainMenu.push_back(pButtonSystem);
		m_vecpButtonMainMenu.push_back(pButtonCash);
		m_vecpButtonMainMenu.push_back(pButtonGuide);
#ifdef SERV_ADD_WARP_BUTTON
		m_vecpButtonMainMenu.push_back(pButtonWarp);
#endif //SERV_ADD_WARP_BUTTON
#ifdef SERV_RECOMMEND_LIST_EVENT
		m_vecpButtonMainMenu.push_back(pButtonFriendRecommend);
#endif //SERV_RECOMMEND_LIST_EVENT
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		m_vecpButtonMainMenu.push_back(pButtonEvent);
#endif SERV_4TH_ANNIVERSARY_EVENT
	}

	//�޴� ��ư new
	CKTDGUIStatic* pButtonPetEvolNew = (CKTDGUIStatic*)m_pDLGMenu->GetControl(L"Pat_Inventory_Evolution"); //����ȭ
	CKTDGUIStatic* pButtonPetNew = (CKTDGUIStatic*)m_pDLGMenu->GetControl(L"Pat_Inventory_New"); //���ȯ
	CKTDGUIStatic* pButtonInvenNew = (CKTDGUIStatic*)m_pDLGMenu->GetControl(L"Inventory_New"); //�κ�
	CKTDGUIStatic* pButtonSkillNew = (CKTDGUIStatic*)m_pDLGMenu->GetControl(L"Skill_New"); //��ų
#ifdef MODIFY_ACCEPT_QUEST
	CKTDGUIStatic* pButtonQuestNew = (CKTDGUIStatic*)m_pDLGMenu->GetControl(L"Quest_New"); //����Ʈ
#endif // MODIFY_ACCEPT_QUEST
#ifdef SERV_ADD_WARP_BUTTON
	CKTDGUIStatic* pButtonWarpVIP = (CKTDGUIStatic*)m_pDLGMenu->GetControl(L"Warp_VIP"); //��ų
#endif //SERV_ADD_WARP_BUTTON
	//��ų, ��, ��� ��ư�� ��� �� �� ���� ���� ��� �� �ֱ�.
	if( NULL != g_pData )
	{
		//�ʱ�ȭ
		D3DXVECTOR2 offset(0,0);
		//���θ޴�
		pButtonGuild->SetShowEnable( true, true );
		pButtonPet->SetShowEnable( true, true );
		pButtonSkill->SetShowEnable( true, true );
#ifdef SERV_ADD_WARP_BUTTON
		pButtonWarp->SetShowEnable( true, true );
#endif //SERV_ADD_WARP_BUTTON
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		pButtonEvent->SetShowEnable( true, true );
		pButtonEvent->SetOffsetPos( offset );
#endif //SERV_4TH_ANNIVERSARY_EVENT
		pButtonChar->SetOffsetPos( offset );
		pButtonInven->SetOffsetPos( offset );
		pButtonSkill->SetOffsetPos( offset );
		pButtonQuest->SetOffsetPos( offset );
		pButtonPet->SetOffsetPos( offset );
		pButtonGuide->SetOffsetPos(offset);
#ifdef SERV_ADD_WARP_BUTTON
		pButtonWarp->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
		//���θ޴� new
		pButtonPetEvolNew->SetOffsetPos( offset );
		pButtonPetNew->SetOffsetPos( offset );
		pButtonInvenNew->SetOffsetPos( offset );
		pButtonSkillNew->SetOffsetPos( offset );
#ifdef MODIFY_ACCEPT_QUEST
		pButtonQuestNew->SetOffsetPos( offset );	
#endif // MODIFY_ACCEPT_QUEST
#ifdef SERV_ADD_WARP_BUTTON
		pButtonWarpVIP->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
	//�˸���ǳ��
		g_pMain->GetInformerManager()->GetInvenInformer()->SetTalkBoxOffsetPos(offset);
		g_pMain->GetInformerManager()->GetGuildInformer()->SetTalkBoxOffsetPos(offset);
		g_pMain->GetInformerManager()->GetSkillInformer()->SetTalkBoxOffsetPos(offset);
		g_pMain->GetInformerManager()->GetQuestInformer()->SetTalkBoxOffsetPos(offset);



		//��尡 ���ٸ�
		if( NULL != g_pData->GetGuildManager() && false == g_pData->GetGuildManager()->DidJoinGuild() )
		{
#ifdef INT_WIDE_BAR
			offset.x += 30;
#else // INT_WIDE_BAR
			offset.x += 38;
#endif // INT_WIDE_BAR
			
			pButtonGuild->SetShowEnable(false, false);

			//���θ޴�
			pButtonGuide->SetOffsetPos(offset);
			pButtonChar->SetOffsetPos( offset );
			pButtonInven->SetOffsetPos( offset );
			pButtonPet->SetOffsetPos( offset );
			pButtonSkill->SetOffsetPos( offset );
			pButtonQuest->SetOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
			pButtonWarp->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
			//���̵� ������
			pButtonGuide->SetGuideDescOffsetPos(offset);
			pButtonChar->SetGuideDescOffsetPos( offset );
			pButtonInven->SetGuideDescOffsetPos( offset );
			pButtonPet->SetGuideDescOffsetPos( offset );
			pButtonSkill->SetGuideDescOffsetPos( offset );
			pButtonQuest->SetGuideDescOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
			pButtonWarp->SetGuideDescOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
			//���θ޴� new
			pButtonPetEvolNew->SetOffsetPos( offset );
			pButtonPetNew->SetOffsetPos( offset );
			pButtonInvenNew->SetOffsetPos( offset );
			pButtonSkillNew->SetOffsetPos( offset );
#ifdef MODIFY_ACCEPT_QUEST
			pButtonQuestNew->SetOffsetPos( offset );
#endif // MODIFY_ACCEPT_QUEST
#ifdef SERV_ADD_WARP_BUTTON
			pButtonWarpVIP->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
			//�˸���ǳ��
			g_pMain->GetInformerManager()->GetInvenInformer()->SetTalkBoxOffsetPos(offset);
			g_pMain->GetInformerManager()->GetGuildInformer()->SetTalkBoxOffsetPos(offset);
			g_pMain->GetInformerManager()->GetSkillInformer()->SetTalkBoxOffsetPos(offset);
		}

		//�� ����Ʈ�� ���� �ϳ��� ���ų�, ��ȯ���� ���� ������
		if( (NULL != g_pData->GetUIManager() &&	NULL != g_pData->GetUIManager()->GetUIPetList() &&	0 == g_pData->GetUIManager()->GetUIPetList()->GetPetListSize() )
			&& ( NULL != g_pData->GetPetManager() && NULL == g_pData->GetPetManager()->GetMyPet() )
#ifdef RIDING_SYSTEM
			&& ( NULL != CX2RidingPetManager::GetInstance() && false == CX2RidingPetManager::GetInstance()->GetHasRidingPet() )
#endif //RIDING_SYSTEM
			)
		{
#ifdef INT_WIDE_BAR
			offset.x += 30;
#else // INT_WIDE_BAR
			offset.x += 38;
#endif // INT_WIDE_BAR
			pButtonPet->SetShowEnable(false, false);

			//���θ޴�
			pButtonGuide->SetOffsetPos(offset);
			pButtonChar->SetOffsetPos( offset );
			pButtonInven->SetOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
			pButtonWarp->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
			//���̵� ������
			pButtonGuide->SetGuideDescOffsetPos(offset);
			pButtonChar->SetGuideDescOffsetPos( offset );
			pButtonInven->SetGuideDescOffsetPos( offset );
			pButtonPet->SetGuideDescOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
			pButtonWarp->SetGuideDescOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
			//���θ޴� new
			pButtonInvenNew->SetOffsetPos( offset );
			pButtonSkillNew->SetOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
			pButtonWarpVIP->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
			//�˸���ǳ��
			g_pMain->GetInformerManager()->GetInvenInformer()->SetTalkBoxOffsetPos(offset);
			g_pMain->GetInformerManager()->GetGuildInformer()->SetTalkBoxOffsetPos(offset);
			g_pMain->GetInformerManager()->GetSkillInformer()->SetTalkBoxOffsetPos(offset);
			g_pMain->GetInformerManager()->GetQuestInformer()->SetTalkBoxOffsetPos(offset);
		}

		//��ų ����Ʈ�� ���ٸ�
		if( NULL != g_pData->GetMyUser() && 
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{

			int iUsedCP, iUsedSP;
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.CalcUsedSPointAndCSPoint( iUsedSP, iUsedCP );
			int iTotalUsedSP = iUsedCP + iUsedSP;

			int iNewSP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint + g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint;
			if( iNewSP == 0 && iTotalUsedSP == 2 )
			{
#ifdef INT_WIDE_BAR
				offset.x += 30;
#else // INT_WIDE_BAR
				offset.x += 38;
#endif // INT_WIDE_BAR
				pButtonSkill->SetShowEnable(false, false);

				pButtonChar->SetOffsetPos( offset );
				pButtonGuide->SetOffsetPos(offset);
				pButtonInven->SetOffsetPos( offset );
				pButtonPet->SetOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
				pButtonWarp->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
				//���̵� ������
				pButtonGuide->SetGuideDescOffsetPos(offset);
				pButtonChar->SetGuideDescOffsetPos( offset );
				pButtonInven->SetGuideDescOffsetPos( offset );
				pButtonPet->SetGuideDescOffsetPos( offset );
#ifdef SERV_ADD_WARP_BUTTON
				pButtonWarp->SetGuideDescOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
				//���θ޴� new
				pButtonPetEvolNew->SetOffsetPos( offset );
				pButtonPetNew->SetOffsetPos( offset );
				pButtonInvenNew->SetOffsetPos( offset );
#ifdef MODIFY_ACCEPT_QUEST
				pButtonQuestNew->SetOffsetPos( offset );
#endif // MODIFY_ACCEPT_QUEST
#ifdef SERV_ADD_WARP_BUTTON
				pButtonWarpVIP->SetOffsetPos( offset );
#endif //SERV_ADD_WARP_BUTTON
				//�˸���ǳ��
				g_pMain->GetInformerManager()->GetInvenInformer()->SetTalkBoxOffsetPos(offset);
			}
		}
	}

}

/*virtual*/ bool CX2StateMenu::ProcessStateChangeUnitSelectWithOptionWindow()
{

#ifdef RIDING_SYSTEM
	if( NULL != CX2RidingPetManager::GetInstance() )
		CX2RidingPetManager::GetInstance()->Handler_EGS_UNSUMMON_RIDING_PET_REQ();
#endif //RIDING_SYSTEM

	ToggleSystemMenu(false);
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_TRAINING_SCHOOL:
	case CX2Main::XS_BATTLE_FIELD:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
#ifdef NEW_VILLAGE_UI
			g_pData->GetUIManager()->CloseAll();
#endif

			//{{ kimhc // 2010.9.29 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
#ifdef	AVATAR_IMAGE_TEST
			g_pMain->ScreenCaptureAndSendToWeb( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );
#endif	AVATAR_IMAGE_TEST
			//}} kimhc // 2010.9.29 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ

			//{{ kimhc // 2009-10-12 // ���� ���ý� ��� ���� ����
#ifdef	GUILD_MANAGEMENT
			g_pData->GetGuildManager()->SafeDeleteGuildInfo();
#endif	GUILD_MANAGEMENT
			//}} kimhc // 2009-10-12 // ���� ���ý� ��� ���� ����

#ifdef SERV_GLOBAL_MISSION_MANAGER
			g_pData->ResetGlobalMissionManager();
#endif SERV_GLOBAL_MISSION_MANAGER

			CX2StateAutoChanger::TARGET_DETAIL targetDetail;
#ifdef ELSWORD_NEW_BEGINNING
			g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_BEGINNING, targetDetail );	
#else
#ifdef DEPARTURE_PREVENTION_SYSTEM
			if ( NULL != g_pData && NULL != g_pKTDXApp && NULL != g_pData->GetDeparturePrevention() )
			{
				g_pData->GetDeparturePrevention()->SetPrevState( CX2DeparturePrevention::SDPPS_CHARACTER_SELECT );
				return g_pData->GetDeparturePrevention()->Handler_EGS_OUT_USER_RETAINING_REQ();
			}
			else
			{
				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_SERVER_SELECT, targetDetail );
			}
#else
			g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_SERVER_SELECT, targetDetail );	

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#endif
#endif ELSWORD_NEW_BEGINNING
		} break;
	}

	return true;
}

/*virtual*/ bool CX2StateMenu::ProcessStateChangeServerSelectWithOptionWindow()
{
#ifdef RIDING_SYSTEM
	if( NULL != CX2RidingPetManager::GetInstance() )
		CX2RidingPetManager::GetInstance()->Handler_EGS_UNSUMMON_RIDING_PET_REQ();
#endif //RIDING_SYSTEM

	ToggleSystemMenu(false);

	//{{ kimhc // 2010.9.29 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
#ifdef	AVATAR_IMAGE_TEST
	g_pMain->ScreenCaptureAndSendToWeb( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );
#endif	AVATAR_IMAGE_TEST
	//}} kimhc // 2010.9.29 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ

#ifdef NEW_VILLAGE_UI
	g_pData->GetUIManager()->CloseAll();
#endif

	//{{ kimhc // 2009-10-12 // ���� ���ý� ��� ���� ����
#ifdef	GUILD_MANAGEMENT
	g_pData->GetGuildManager()->SafeDeleteGuildInfo();
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-12 // ���� ���ý� ��� ���� ����

#ifdef ELSWORD_NEW_BEGINNING
	CX2StateBeginning::m_sbSelectedServerSet = false;
#else
	CX2StateServerSelect::m_sbSelectedServerSet = false;
#endif ELSWORD_NEW_BEGINNING
	CX2StateAutoChanger::TARGET_DETAIL targetDetail;
#ifdef ELSWORD_NEW_BEGINNING
	g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)CX2Main::XS_BEGINNING, targetDetail );
#else
#ifdef DEPARTURE_PREVENTION_SYSTEM
	if ( NULL != g_pData && NULL != g_pKTDXApp && NULL != g_pData->GetDeparturePrevention()	&&
		CX2Main::XS_SERVER_SELECT != g_pMain->GetNowStateID() )
	{
		g_pData->GetDeparturePrevention()->SetPrevState( CX2DeparturePrevention::SDPPS_SERVER_SELECT );
		return g_pData->GetDeparturePrevention()->Handler_EGS_OUT_USER_RETAINING_REQ();
	}
	else
	{
		g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)
			CX2Main::XS_SERVER_SELECT, targetDetail );
	}
#else
	g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), (int)
		CX2Main::XS_SERVER_SELECT, targetDetail );

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#endif
#endif ELSWORD_NEW_BEGINNING

	return true;
	//return Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
}

#ifdef SERV_EVENT_RETURN_USER_MARK
void CX2StateMenu::OpenElliosGuide( int iTargetPage )
{
	if(false == m_Book.GetShow())
	{
		m_Book.SetBookTable( L"BT_ELLIOS_GUIDE");
		m_Book.SetShow(true);
		m_Book.GotoPage(iTargetPage);
	}
}
#endif SERV_EVENT_RETURN_USER_MARK

#ifdef BUFF_TEMPLET_SYSTEM
void CX2StateMenu::UpdateGuideDescBuffTemplet( CX2PremiumBuffIconPtr pPremiumBuffPtr )
{
	CKTDGUIDialogType BuffIconDialogType	= pPremiumBuffPtr->GetBuffIconDialogType();

	CKTDGUIButton*	  pButton				= static_cast<CKTDGUIButton*>( BuffIconDialogType->GetControl( L"Button_Buff_Icon" ) );	//�ӽ� ����

	if( pButton->IsOnMouseOver() == true )
	{
		wstringstream						wstreamDesc;																					
		wstring								strBufftempletDesc	= L"";																		
		wstring								wstrEndTime			= pPremiumBuffPtr->GetBuffEndTime();											//���� ���� �ð�
		CX2PremiumBuffTemplet::BUFF_ID		ePremiumBuffID		= pPremiumBuffPtr->GetPremiumBuffID();											//���� ID

		CX2PremiumBuffTempletPtr			ptrTemplet			= g_pData->GetPremiumBuffManager()->GetPremiumBuffTempletPtr( ePremiumBuffID );	//���� ���ø�
		UINT								uiBuffName			= ptrTemplet->GetStringID();													//���� �̸� String ID
		int									BuffPeriodSecond	= static_cast<int>( ptrTemplet->GetBuffPeriodSecond() / 60 );					//���� ���� �ð�
		int									iBuffTimeAllSec		= ConvertTimeForBuffTemplet( wstrEndTime );										//���� ���� ���� �ð� ( ���� �� )
		int									iBuffTimeMin		= ConvertTimeForBuffTemplet( wstrEndTime ) / 60;								//���� ���� ���� �ð� ( �� )
		int									iBuffTimeSec		= ConvertTimeForBuffTemplet( wstrEndTime ) % 60;								//���� ���� ���� �ð� ( �� )
		bool								bShowEndTime		= true;																			//���� ���� �ð� ǥ�� ����

#ifdef SERV_2012_PVP_SEASON2_EVENT
		bool								bSetYellowColorBuffName	= false;		/// ���� �̸� ����� ó��

		switch( ePremiumBuffID )		/// ���� �̸� ��������� ó�� �� ������ �˻�
		{
		case CX2PremiumBuffTemplet::BI_TIME_OF_GLORY:			/// ������ �ð�
			{
				bSetYellowColorBuffName = true;
			} break;
		}
		
		if( true == bSetYellowColorBuffName )
			wstreamDesc << "#CFFFF00" << GET_STRING( uiBuffName ) << "#CX" << '\n';		/// ����� ���� �̸�
		else
			wstreamDesc << GET_STRING( uiBuffName ) << '\n';	/// ���� �̸�
#else SERV_2012_PVP_SEASON2_EVENT
		wstreamDesc << GET_STRING( uiBuffName ) << '\n';		/// ���� �̸�
#endif SERV_2012_PVP_SEASON2_EVENT

		switch( ePremiumBuffID )
		{
		case CX2PremiumBuffTemplet::BI_PROTECTION_OF_NUT:			// ��Ʈ�� ��ȣ   ( ��� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_16564 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16601, "i", 100 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17099 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_PROTECTION_OF_PUTA:			// ��Ÿ�� ��ȣ   ( ��� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17130 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16602, "i",25 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17099 );

				strBufftempletDesc += wstreamDesc.str().c_str();

			}break;

		case CX2PremiumBuffTemplet::BI_PROTECTION_OF_LAHELL:		// ������ ��ȣ   ( ��� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17131 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16603, "i", 20 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17099 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_PROTECTION_OF_JAKIELL:		// ��Ű���� ��ȣ ( ��� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17132 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16604, "f", 1.5 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17099 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_BELSSING_OF_SERAPHIM:		// �������� �ູ	 ( ���� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_16565 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16605, "ii", 100 , 2 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17100 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_BELSSING_OF_GEV:				// �Ժ��� �ູ	 ( ���� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17133 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16606, "i", 5 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17100 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_BELSSING_OF_AMON:			// �Ƹ��� �ູ	 ( ���� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17134 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16607, "i", 5 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17100 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_BELSSING_OF_CRONOS:			// ũ�γ뽺�� �ູ ( ���� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17135 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_16608, "ii",50, 2 ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_17100 );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;

		case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_1:	// ����� �� ���� ���� ���� Level 1
		case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_2:	// ����� �� ���� ���� ���� Level 2
		case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_3:	// ����� �� ���� ���� ���� Level 3
		case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_4:	// ����� �� ���� ���� ���� Level 4
			{
				//������ ó���� ���ڵ� ����
				strBufftempletDesc += wstreamDesc.str().c_str();

				//���� ���� ���� ���� ǥ��
				wstringstream wstreamBuffLevelDesc;

				int iBuffLevel = 0;

				switch( ePremiumBuffID )
				{
				case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_1:	// ����� �� ���� ���� ���� Level 1
					{
						iBuffLevel = 1;
					}break;
				case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_2:	// ����� �� ���� ���� ���� Level 2
					{
						iBuffLevel = 2;
					}break;
				case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_3:	// ����� �� ���� ���� ���� Level 3
					{
						iBuffLevel = 3;
					}break;
				case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_4:	// ����� �� ���� ���� ���� Level 4
					{
						iBuffLevel = 4;
					}break;
				default:
					{
						ASSERT( "Save Crystal Buff is Nothing!" );
					}break;
				}
				wstreamBuffLevelDesc << GET_REPLACED_STRING( ( STR_ID_16598, "i", iBuffLevel ) ) << '\n' << '\n'
				<< GET_STRING( STR_ID_16599 ) << '\n' << '\n';

				strBufftempletDesc += wstreamBuffLevelDesc.str().c_str();

				//���� ������ �����ͼ� ���
				vector<int> vSocketOptionList = ptrTemplet->GetSocketOptionsList();
				CX2Unit*	pkUnit			  = g_pData->GetMyUser()->GetSelectUnit();

				for ( int i = 0; i < (int)vSocketOptionList.size(); i++ )
				{
					const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( vSocketOptionList[i] );

					if( pSocketData != NULL )
					{
						strBufftempletDesc += L"#CFFFF00";

						//���� ���� ���
						strBufftempletDesc += pSocketData->GetSocketDesc( pkUnit->GetUnitData().m_Level );

						strBufftempletDesc += L"#CX";

						strBufftempletDesc += L"\n";
					}
					
				}

				//���� ���� ���� �ȳ� ǥ��
				wstringstream wstreamSaveBuffDesc;

				wstreamSaveBuffDesc << '\n' << GET_STRING( STR_ID_16600 );

				strBufftempletDesc += wstreamSaveBuffDesc.str().c_str();

				//����� �� ���� ���� ������ ���� ���� �ð��� ǥ������ ����
				//bShowEndTime = false;
			}break;

		case CX2PremiumBuffTemplet::BI_STRENGTHENING_BODY:	// ��ȭ�� ��ü ( ��� ��� ���� ����� )
			{
				wstreamDesc << '\n' << GET_STRING( STR_ID_17097 ) << '\n' << '\n'
				<< GET_REPLACED_STRING( ( STR_ID_17098, "i", 5 ) );

				strBufftempletDesc += wstreamDesc.str().c_str();
			}break;
#ifdef SERV_QUEST_POSSESSION_BUFF
		case CX2PremiumBuffTemplet::BI_APINK_QUEST_BUFF:	// ������ũ ����Ʈ ����	
			{
				wstreamDesc << '\n' << GET_REPLACED_STRING( ( STR_ID_17791, "i", 15 ) ) << '\n'
					<< GET_REPLACED_STRING( ( STR_ID_17792, "i", 15 ) );

				strBufftempletDesc += wstreamDesc.str().c_str();
			} break;
#endif //SERV_QUEST_POSSESSION_BUFF

#ifdef SERV_2012_PVP_SEASON2_EVENT 
		case CX2PremiumBuffTemplet::BI_TIME_OF_GLORY:		// ���� ���� ���� 2 ���� �̺�Ʈ - ������ �ð�
			{
				wstreamDesc << '\n' << GET_REPLACED_STRING( ( STR_ID_18688, "iii", 10, 1, 1 ) );

				strBufftempletDesc += wstreamDesc.str().c_str();

				bShowEndTime = false;		/// ���� �ð� ��ǥ��
			} break;
#endif SERV_2012_PVP_SEASON2_EVENT
		default:
			{
			}break;
		}

		//���� �ð� �� ���� �ð� ǥ��
		if( bShowEndTime == true )
		{
			/*
			wstringstream wstreamTimeDesc;

			wstreamTimeDesc << '\n' << GET_REPLACED_STRING( ( STR_ID_16566, "i", iBuffTimeMin ) ) << L":";

			//���ڸ� ������ ��, �տ� 0�� ���� ( �� :7 -> 07 )
			if( iBuffTimeSec < 10 )
				wstreamTimeDesc << L"0" << iBuffTimeSec << '\n';
			else
				wstreamTimeDesc << iBuffTimeSec << '\n';

			//������ ó���� ���ڵ� ����
			strBufftempletDesc += wstreamTimeDesc.str().c_str();
			*/
			wstringstream wstreamTimeDesc;

			AddStringRemainTime( wstreamTimeDesc, iBuffTimeAllSec );

			strBufftempletDesc += wstreamTimeDesc.str().c_str();
		}

		pButton->SetGuideDescAbsolutePos( D3DXVECTOR2( 15.f, 685.f) );
		pButton->SetGuideDesc( strBufftempletDesc.c_str() );

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( BuffIconDialogType );
	}
}

int CX2StateMenu::ConvertTimeForBuffTemplet( wstring wstrEndTime )
{
	CTime cEndTime;

	//std::wstring tempString = kEvent.m_wstrElixirUseTime.substr(0, 19);
	KncUtil::ConvertStringToCTime( wstrEndTime, cEndTime );
	if( wstrEndTime.length() > 0 )
	{
		CTime cCurrentTime = g_pData->GetServerCurrentTime();	//���� �ð�

		CTimeSpan cBuffTime = cEndTime - cCurrentTime;


		if( cBuffTime.GetTotalSeconds() > 0 )					//ȿ�� �ð��� �������� ��
			return static_cast<int>(cBuffTime.GetTotalSeconds());
	}
	return 0;
}

//���� ǥ�� ����
void CX2StateMenu::SetShowBuffTempletIcon( bool  bVal)
{
	if( g_pData->GetPremiumBuffManager()->GetPremiumBuffList().empty() != true )
	{
		BOOST_FOREACH( CX2PremiumBuffIconPtr pPremiumBuffPtr, g_pData->GetPremiumBuffManager()->GetPremiumBuffList() )
		{
			CX2PremiumBuffTemplet::BUFF_ID		ePremiumBuffID		= pPremiumBuffPtr->GetPremiumBuffID();													//���� ID

			//���� �������� �ִ� ������ ��,
			if( pPremiumBuffPtr->GetUsingIcon() == true )
			{
				CX2PremiumBuffTempletPtr		ptrTemplet			= g_pData->GetPremiumBuffManager()->GetPremiumBuffTempletPtr( ePremiumBuffID );			//���� ���ø�	

				CKTDGUIDialogType				BuffIconDialogType	= pPremiumBuffPtr->GetBuffIconDialogType();												

				CKTDGUIButton*					pButton				= static_cast<CKTDGUIButton*>( BuffIconDialogType->GetControl( L"Button_Buff_Icon" ) );

				pButton->SetShow( bVal );
			}
		}
	}
	g_pData->GetPremiumBuffManager()->SetShowBuffTempletUI( bVal );
}
#endif BUFF_TEMPLET_SYSTEM



#ifdef SERV_RECOMMEND_LIST_EVENT
bool CX2StateMenu::Handler_EGS_RECOMMEND_USER_REQ( const WCHAR* wszNickName, bool bCancel/* = false*/ )
{
	KEGS_RECOMMEND_USER_REQ kPacket;
	kPacket.m_wstrNickName = wszNickName;
	kPacket.m_wstrNickName = kPacket.m_wstrNickName.substr( 0, 20 );
	kPacket.m_bCancel = bCancel;

	if( false == kPacket.m_bCancel )
	if( NULL != g_pMain->GetStringFilter() && false == g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, kPacket.m_wstrNickName.c_str() ) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), this );
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_RECOMMEND_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RECOMMEND_USER_ACK );

	return true;
}

bool CX2StateMenu::Handler_EGS_RECOMMEND_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer* >( lParam );
	KEGS_RECOMMEND_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_RECOMMEND_USER_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			OpenRecommendMsgBox( false );
			if ( kEvent.m_iRewardResurrectionStone > 0 )
			{
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_718, "i", kEvent.m_iRewardResurrectionStone ) );
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
				
			}

			if( NULL != m_pDLGMenu->GetControl( L"FriendRecommed" ) )
			{
				m_pDLGMenu->GetControl( L"FriendRecommed" )->SetShowEnable( false, false );
			}

			return true;
		}
	}

	return false;
}

void CX2StateMenu::OpenRecommendMsgBox( bool _bOpen )
{
	if ( _bOpen == true )
	{
		if ( m_pDLGFriendRecommendPopUp != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGFriendRecommendPopUp, NULL, false );

		m_pDLGFriendRecommendPopUp = NULL;

#ifdef SERV_RECOMMEND_LIST_EVENT
		m_pDLGFriendRecommendPopUp = new CKTDGUIDialog( this, L"DLG_Recommend2.lua" );
#else
		m_pDLGFriendRecommendPopUp = new CKTDGUIDialog( this, L"DLG_Recommend.lua" );
#endif //SERV_RECOMMEND_LIST_EVENT

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFriendRecommendPopUp );

		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGFriendRecommendPopUp->GetControl( L"g_pIMEEditBoxName" );
		if ( pIMEEditBox != NULL )
		{
			pIMEEditBox->RequestFocus();
		}

		CKTDGUIButton* pButton = static_cast<CKTDGUIButton* >( m_pDLGFriendRecommendPopUp->GetControl( L"Button_Nominator_Input" ) );
		if( NULL != pButton )
		{
			pButton->SetCustomMsgMouseUp( SMUCM_FRIEND_RECOMMEND_OK );
		}

		pButton = static_cast<CKTDGUIButton* >( m_pDLGFriendRecommendPopUp->GetControl( L"X" ) );
		if( NULL != pButton )
		{
			pButton->SetCustomMsgMouseUp( SMUCM_FRIEND_RECOMMEND_NEXT );
		}

		pButton = static_cast<CKTDGUIButton* >( m_pDLGFriendRecommendPopUp->GetControl( L"Button_NextTime" ) );
		if( NULL != pButton )
		{
			pButton->SetCustomMsgMouseUp( SMUCM_FRIEND_RECOMMEND_NEXT );
		}

		pButton = static_cast<CKTDGUIButton* >( m_pDLGFriendRecommendPopUp->GetControl( L"Button_DontSee" ) );
		if( NULL != pButton )
		{
			pButton->SetCustomMsgMouseUp( SMUCM_FRIEND_RECOMMEND_CLOSE );
		}
	}
	else
	{
		if ( m_pDLGFriendRecommendPopUp != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGFriendRecommendPopUp, NULL, false );

		m_pDLGFriendRecommendPopUp = NULL;
	}
}
#endif //SERV_RECOMMEND_LIST_EVENT

#ifdef MODIFY_ACCEPT_QUEST
void CX2StateMenu::SetShowNewQuest(bool bVal)
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGMenu->GetControl( L"Quest_New" ));
	if( NULL != pStatic )
	{
		pStatic->SetShow(bVal);
	}
}
#endif // MODIFY_ACCEPT_QUEST


#ifdef SERV_NAVER_CHANNELING

bool CX2StateMenu::Handler_EGS_GET_NAVER_ACCESS_TOKEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_NAVER_ACCESS_TOKEN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) 
		&& NULL != g_pInstanceData )
	{
		g_pInstanceData->SetNaverAccessToken( kEvent.m_strAccessToken );
	}		

	return true;
}

#endif // SERV_NAVER_CHANNELING

#ifdef SERV_ADVERTISEMENT_EVENT
bool CX2StateMenu::Handler_EGS_GET_ADVERTISEMENT_EVENT_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_GET_ADVERTISEMENT_EVENT_REQ );
	g_pMain->AddServerPacket( EGS_GET_ADVERTISEMENT_EVENT_ACK );

	return true;
}

bool CX2StateMenu::Handler_EGS_GET_ADVERTISEMENT_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_ADVERTISEMENT_EVENT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_ADVERTISEMENT_EVENT_ACK ) == true )
	{
		if( kEvent.m_vecAdvertisementURL.size() > 0 )
		{
			if( false == g_pMain->GetBrowserWrapper()->IsClosed() )
			{
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

			g_pMain->GetBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );

			std::string strURL;
			ConvertWCHARToChar( strURL, kEvent.m_vecAdvertisementURL[0] );
			g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), NULL );
		}
	}

	return false;
}
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_ADD_WARP_BUTTON
void CX2StateMenu::ClickWarpButton( bool bIsEdConsumption_ )
{	
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
		{
			m_bIsEdConsumptionWarp = bIsEdConsumption_;
			CreateWarpDest();
		} break;

	default:
		{
			// ����/�ŷ�����/��Ʋ�ʵ� �̿ܿ����� ���ȵ�...
			g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4495 ),g_pMain->GetNowState() );
		} break;
	}	/// switch
}

void CX2StateMenu::UseWarpPopup(bool bIsEdConsumption_)
{
	if( NULL != g_pData && NULL != g_pData->GetLocationManager() &&
		NULL != g_pData->GetLocationManager()->GetVillageMapTemplet(static_cast<SEnum::VILLAGE_MAP_ID>(m_iSelectedWarpDest)) )
	{
		wstring strVillageName = g_pData->GetLocationManager()->GetVillageMapTemplet(static_cast<SEnum::VILLAGE_MAP_ID>(m_iSelectedWarpDest))->m_Name;

		if( true == strVillageName.empty() )
			return;

		if( true == bIsEdConsumption_ )
		{
			int iWarpDistance = abs(m_iSelectedWarpDestIndex - m_iCurrentVillageWarpIndex);
			int iBasePrice = 6000;
			float fDecreaseFactor = 0.6f;
			if( NULL !=  g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{				
				float fLevel = static_cast<float>(g_pData->GetSelectUnitLevel());
				if( fLevel > 0 )
				{
					int iWarpCommission = static_cast<int>((fLevel/60) * (iBasePrice + static_cast<int>(iBasePrice * fDecreaseFactor) * (iWarpDistance-1) )); //�̵� ������	

					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
						GET_REPLACED_STRING((STR_ID_20266, "iL", iWarpCommission, strVillageName)), 
						SMUCM_WARP_DEST_OK, g_pMain->GetNowState() );	
				}
			}
		}
		else
		{
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
				GET_REPLACED_STRING((STR_ID_20297, "L", strVillageName)), 
				SMUCM_WARP_DEST_OK, g_pMain->GetNowState() );	
		}
	}
}

void CX2StateMenu::AddWarpList(CKTDGUIContolList *pControlList, const int row, const int iVillageId)
{
	if( pControlList == NULL )
		return;

	int iCurrentVillageID = -1;
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:		
	case CX2Main::XS_SQUARE_GAME:
		{
			iCurrentVillageID = static_cast<int>(g_pData->GetLocationManager()->GetCurrentVillageID());
		} break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			iCurrentVillageID = g_pData->GetBattleFieldManager().GetReturnVillageId();
		} break;
	default:
		break;
	}

	if( iVillageId == iCurrentVillageID )
	{
		m_iCurrentVillageWarpIndex = row;
	}

	WCHAR wVillageId[256] = {0,};
	//wsprintf(wVillageId, L"Village_%d", iVillageId);
	StringCchPrintf(wVillageId, 256, L"Village_%d", iVillageId);
	// ��������Ʈ �߰�
	CKTDGUIStatic *pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(wVillageId);
	pControl->SetShow(true);
	pControlList->InsertItem( pControl, row, 0, false );

	if( m_pDLGWarpDestination != NULL )
	{
		m_pDLGWarpDestination->OpenScriptFile(L"DLG_Destination_Control_byButton.lua");

		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGWarpDestination->GetControl( L"ListControl_Button" );
		//wsprintf(wVillageId, L"Button_%d", iVillageId);
		StringCchPrintf(wVillageId, 256, L"Button_%d", iVillageId);
		pButton->SetName( wVillageId );
		pButton->AddDummyInt(iVillageId);
		pControlList->InsertItem( pButton, row, 1 );
		pButton->SetShowEnable(true, true);

		pButton->SetDownStateAtNormal(false);

		m_vecWarpListButton.push_back( pButton );
	}
}

void CX2StateMenu::InitWarpList()
{
	CKTDGUIStatic *pControl = NULL;
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20000");	//VMI_RUBEN
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20001");	//VMI_ELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1000");	//VMI_BATTLE_FIELD_ELDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20002");	//VMI_BESMA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1001");	//VMI_BATTLE_FIELD_BESMA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20003");	//VMI_ALTERA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1002");	//VMI_BATTLE_FIELD_ALTERA_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20005");	//VMI_PEITA
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20004");	//VMI_VELDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1004");	//VMI_BATTLE_FIELD_VELDER_REST_00
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20006");	//VMI_HAMEL
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1005");	//VMI_BATTLE_FIELD_HAMEL_REST_00
	pControl->SetShow(false);
#ifdef VILLAGE_SANDER
#ifndef NO_SANDER_VILLIAGE
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20007");	//VMI_SANDER
	pControl->SetShow(false);
	pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1006");	//VMI_BATTLE_FIELD_SANDER_REST_00
	pControl->SetShow(false);
#endif NO_SANDER_VILLIAGE
#endif //VILLAGE_SANDER
}

void CX2StateMenu::CreateWarpDest()
{	
	m_vecWarpListButton.clear();
	SAFE_DELETE_DIALOG( m_pDLGWarpDestination );

	if( m_pDLGWarpDestination == NULL )
	{
		m_pDLGWarpDestination = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Destination_byButton.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarpDestination );
	}

	if( m_pDLGWarpDestination != NULL )
	{		
		m_pDLGWarpDestination->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(300, 400), D3DXVECTOR2(75,20)) );
		m_pDLGWarpDestination->SetShowEnable(true, true);

		CKTDGUIContolList *pControlList = (CKTDGUIContolList*)m_pDLGWarpDestination->GetControl(L"WarpDestControlList");
		if( pControlList != NULL )
		{				
			CKTDGUIStatic *pControl = NULL;						
			CX2LocationManager::LocalMapTemplet* pLocalMapTemplet = NULL;

			int row = 0;
			InitWarpList();
			// �纥����
			{
				// �纥����
				pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20000");	//VMI_RUBEN
				pControl->SetShow(false);
				AddWarpList( pControlList, row, SEnum::VMI_RUBEN );
				++row;
			}			

			// ��������
			const int iMyUnitLevel = g_pData->GetSelectUnitLevel();
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_ELDER );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ��������
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20001");	//VMI_ELDER
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_ELDER );
					++row;

					// ���� �޽�ó(������ ����)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1000");	//VMI_BATTLE_FIELD_ELDER_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_ELDER_REST_00 );
					++row;
				}
			}

			// ����������
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_BESMA );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ����������
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20002");	//VMI_BESMA
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BESMA );
					++row;

					// ������ �޽�ó(�ƽ��ƽ� �ϴñ�)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1001");	//VMI_BATTLE_FIELD_BESMA_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_BESMA_REST_00 );
					++row;
				}
			}

#ifdef SERV_ALTERA_AUTO_OPEN_HARD_CODE
			IF_EVENT_ENABLED( CEI_ALTERA_AUTO_OPEN_HARD_CODE )
			{
				// ���׶�����
				pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_ALTERA_ISLAND );
				if( pLocalMapTemplet != NULL )
				{
					if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
					{
						// ���׶���
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20003");	//VMI_ALTERA
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_ALTERA );
						++row;

						// ���׶� �޽�ó(ǳ���� ����ó)
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1002");	//VMI_BATTLE_FIELD_ALTERA_REST_00
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00 );
						++row;
					}
				}
			}
			ELSE
			{
				// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
			}
#else
			// ���׶�����
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_ALTERA_ISLAND );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ���׶���
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20003");	//VMI_ALTERA
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_ALTERA );
					++row;

					// ���׶� �޽�ó(ǳ���� ����ó)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1002");	//VMI_BATTLE_FIELD_ALTERA_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00 );
					++row;
				}
			}

#endif //SERV_ALTERA_AUTO_OPEN_HARD_CODE
			
#ifdef SERV_PEITA_AUTO_OPEN_HARD_CODE
			IF_EVENT_ENABLED( CEI_PEITA_AUTO_OPEN_HARD_CODE )
			{
				// ����Ÿ����
				pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_PEITA );
				if( pLocalMapTemplet != NULL )
				{
					if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
					{
						// ����Ÿ(�İ� ��Ʈ)
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20005");	//VMI_PEITA
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_PEITA );
						++row;
					}
				}
			}
			ELSE
			{
				// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
			}
#else
			// ����Ÿ����
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_PEITA );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ����Ÿ(�İ� ��Ʈ)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20005");	//VMI_PEITA
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_PEITA );
					++row;
				}
			}
#endif //SERV_PEITA_AUTO_OPEN_HARD_CODE

#ifdef SERV_VELDER_AUTO_OPEN_HARD_CODE
			IF_EVENT_ENABLED( CEI_VELDER_AUTO_OPEN_HARD_CODE )
			{
				// ��������
				pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_VELDER );
				if( pLocalMapTemplet != NULL )
				{
					if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
					{
						// ��������
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20004");	//VMI_VELDER
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_VELDER );
						++row;

						// ���� �޽�ó (�ð�ž ����)
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1004");	//VMI_BATTLE_FIELD_VELDER_REST_00
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_VELDER_REST_00 );
						++row;
					}
				}
			}
			ELSE
			{
				// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
			}
#else

#ifdef SERV_NO_VELDER_VILLIAGE
#else //SERV_NO_VELDER_VILLIAGE
			// ��������
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_VELDER );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ��������
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20004");	//VMI_VELDER
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_VELDER );
					++row;

					// ���� �޽�ó (�ð�ž ����)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1004");	//VMI_BATTLE_FIELD_VELDER_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_VELDER_REST_00 );
					++row;
				}
			}
#endif //SERV_NO_VELDER_VILLIAGE
#endif //SERV_VELDER_AUTO_OPEN_HARD_CODE

#ifdef SERV_HAMEL_AUTO_OPEN_HARD_CODE
			IF_EVENT_ENABLED( CEI_HAMEL_AUTO_OPEN_HARD_CODE )
			{
				//{{ JHKang / ������ / 2011/01/11 / �ϸ� �߰�
#ifdef ADD_HAMEL_VILLAGE
				// �ϸ�����
				pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_HAMEL );
				if( pLocalMapTemplet != NULL )
				{
					if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
					{
						// ���� �ϸ�
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20006");	//VMI_HAMEL
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_HAMEL );
						++row;

						// �ϸ� �޽�ó(ġ���� ����)
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1005");	//VMI_BATTLE_FIELD_HAMEL_REST_00
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00 );
						++row;
					}
				}
#endif ADD_HAMEL_VILLAGE
				//}} JHKang / ������ / 2011/01/11 / �ϸ� �߰�
			}
			ELSE
			{
				// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
			}
#else
			//{{ JHKang / ������ / 2011/01/11 / �ϸ� �߰�
#ifdef ADD_HAMEL_VILLAGE
#ifdef SERV_NO_HAMEL_VILLIAGE
#else //SERV_NO_HAMEL_VILLIAGE
			// �ϸ�����
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_HAMEL );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ���� �ϸ�
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20006");	//VMI_HAMEL
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_HAMEL );
					++row;

					// �ϸ� �޽�ó(ġ���� ����)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1005");	//VMI_BATTLE_FIELD_HAMEL_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00 );
					++row;
				}
			}
#endif //SERV_NO_HAMEL_VILLIAGE
#endif ADD_HAMEL_VILLAGE
			//}} JHKang / ������ / 2011/01/11 / �ϸ� �߰�

#endif //SERV_HAMEL_AUTO_OPEN_HARD_CODE

#ifdef SERV_SANDER_AUTO_OPEN_HARD_CODE
			IF_EVENT_ENABLED( CEI_SANDER_AUTO_OPEN_HARD_CODE )
			{
#ifdef VILLAGE_SANDER 
				// ��������
				pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_SANDER );
				if( pLocalMapTemplet != NULL )
				{
					if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
					{
						// ���� ����
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20007");	//VMI_HAMEL
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_SANDER );
						++row;

						// ���� �޽�ó(���彺�� ������)
						pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1006");	//VMI_BATTLE_FIELD_HAMEL_REST_00
						pControl->SetShow(false);
						AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_SANDER_REST_00 );
						++row;
					}
				}
#endif //VILLAGE_SANDER
			}
			ELSE
			{
				// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
			}
#else
#ifdef VILLAGE_SANDER 
#ifndef NO_SANDER_VILLIAGE
			// ��������
			pLocalMapTemplet = g_pData->GetLocationManager()->GetLocalMapTemplet( CX2LocationManager::LMI_SANDER );
			if( pLocalMapTemplet != NULL )
			{
				if( iMyUnitLevel >= pLocalMapTemplet->m_RequireUnitLevel )
				{
					// ���� ����
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_20007");	//VMI_HAMEL
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_SANDER );
					++row;

					// ���� �޽�ó(���彺�� ������)
					pControl = (CKTDGUIStatic*)m_pDLGWarpDestination->GetControl(L"Village_1006");	//VMI_BATTLE_FIELD_HAMEL_REST_00
					pControl->SetShow(false);
					AddWarpList( pControlList, row, SEnum::VMI_BATTLE_FIELD_SANDER_REST_00 );
					++row;
				}
			}
#endif NO_SANDER_VILLIAGE
#endif //VILLAGE_SANDER
#endif //SERV_SANDER_AUTO_OPEN_HARD_CODE
			pControlList->SetIndex(0, 0);				
		}
	}	
}

void CX2StateMenu::SetShowWarpDest(bool val)
{
	if( m_pDLGWarpDestination != NULL )
		m_pDLGWarpDestination->SetShowEnable(val, val);
}

bool CX2StateMenu::GetShowWarpDest()
{
	if( m_pDLGWarpDestination != NULL )
		return m_pDLGWarpDestination->GetShow();

	return false;
}

bool CX2StateMenu::Handler_EGS_WARP_BY_BUTTON_REQ( int iWarpDest_ )
{
	KEGS_WARP_BY_BUTTON_REQ kPacket;
	kPacket.m_iCurrentVillageWarpIndex = iWarpDest_;
	kPacket.m_bFreeWarp = !(m_bIsEdConsumptionWarp);// ! �� ����

	g_pData->GetServerProtocol()->SendPacket( EGS_WARP_BY_BUTTON_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_WARP_BY_BUTTON_ACK );

	return true;
}

bool CX2StateMenu::Handler_EGS_WARP_BY_BUTTON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_WARP_BY_BUTTON_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_WARP_BY_BUTTON_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
			}

			// ������ ���� �̵� �˻�
			if( kEvent.m_iWarpPointMapID > 0 && g_pData->GetLocationManager()->IsValidWarpField(kEvent.m_iWarpPointMapID) )
			{
				m_iCurrentVillageWarpIndex = -1;

				if(g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME)
				{
					// �ŷ����忡�� ����					
					g_pData->SetStateArg(kEvent.m_iWarpPointMapID); 
					CX2StateSquareGame* pState = (CX2StateSquareGame*) g_pMain->GetNowState();
					pState->LeaveSquareGame();
				}
				else
				{
					// �̵�Ƽ���� �̿��Ͽ� ���� �̵�
					g_pData->SetStateArg(kEvent.m_iWarpPointMapID);
					CX2State* pState = (CX2State*) g_pMain->GetNowState();
					pState->Handler_EGS_STATE_CHANGE_FIELD_REQ(kEvent.m_iWarpPointMapID);
				}				
			}

			return true;
		}
	}

	return false;
}


bool CX2StateMenu::Handler_EGS_UPDATE_WARP_VIP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_WARP_VIP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if( g_pData->GetMyUser() != NULL &&
			g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_trWarpVipEndDate = kEvent.m_trWarpVipEndDate;
			m_fTimeCheckWarpVip = 0.f;
			return true;
		}	
	}

	return false;
}


wstring CX2StateMenu::GetExpirationDateDesc( __int64 trTime_ )		// ����!! KTDXCommonFunc.h�� ���ǵǾ� �ִ°Ͱ� ���� �̸��̳� ������ �ٸ�
{
	wstring tempDesc = L"";

	tempDesc = GET_STRING( STR_ID_309 );

	CTime cTime = trTime_;
			
	wstringstream wstrstm;

	wstrstm.str( L"" );

	__time64_t t0 = g_pData->GetServerCurrentTime();

	double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );

	int remainDay =  (int)(fDiffTime / ( 60 * 60 * 24 ) );
	if ( remainDay > 0 )
		wstrstm << remainDay << GET_STRING( STR_ID_14 ) << L" ";

	int remainHour = ((time_t)fDiffTime % ( 60 * 60 * 24 )) / (60 * 60);
	if ( remainHour < 0 )
		remainHour = 0;

	wstrstm << remainHour << GET_STRING( STR_ID_310 ) << L" ";

	int remainMinute = ((time_t)fDiffTime % ( 60 * 60 )) / (60 );

	if ( remainDay <= 0 )
	{
		if ( remainMinute <= 0 )
			remainMinute = 0;
		wstrstm << remainMinute << GET_STRING( STR_ID_311 ) << L" ";
	}

	tempDesc += wstrstm.str().c_str();

	tempDesc += L"\n";

	return tempDesc;
}

#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_CHINA_SPIRIT_EVENT
bool CX2StateMenu::Handler_EGS_USE_SPIRIT_REWARD_REQ()
{
	if( NULL == g_pData || NULL == g_pData->GetMyUser() || NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;

	int iLocationIndex = g_pData->GetMyUser()->GetSelectUnit()->GetChinaSpiritIndex();
	if( iLocationIndex > 5 || iLocationIndex < 0 )
		return false;

	KEGS_USE_SPIRIT_REWARD_REQ kPacket;
	kPacket.m_iLocationIndex		= iLocationIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_SPIRIT_REWARD_REQ , kPacket );
	g_pMain->AddServerPacket( EGS_USE_SPIRIT_REWARD_ACK );

	return true;
}


bool CX2StateMenu::Handler_EGS_USE_SPIRIT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_SPIRIT_REWARD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_USE_SPIRIT_REWARD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[0]	= kEvent.m_arrChinaSpirit[0];
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[1]	= kEvent.m_arrChinaSpirit[1];
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[2]	= kEvent.m_arrChinaSpirit[2];
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[3]	= kEvent.m_arrChinaSpirit[3];
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[4]	= kEvent.m_arrChinaSpirit[4];
			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[5]	= kEvent.m_arrChinaSpirit[5];

			g_pMain->GetMemoryHolder()->UpdateUseSpiritEvent();
			return true;
		}
	}

	return false;
}
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
bool CX2StateMenu::Handler_EGS_EVENT_COBO_DUNGEON_FIELD_REQ( bool bTemp )
{
	//��ư�� ������ ��Ŷ ������
	KEGS_EVENT_COBO_DUNGEON_FIELD_REQ kPacket;
	kPacket.m_iOK = 0;
	kPacket.m_EventStart = bTemp;
	//�ϴ� ���� ���� ��Ȱ�� ó�� �������
	if(g_pInstanceData != NULL)
	{
		g_pInstanceData->SetStartUI(false);
	}
	g_pData->GetServerProtocol()->SendPacket( EGS_EVENT_COBO_DUNGEON_FIELD_REQ , kPacket );
	g_pMain->AddServerPacket( EGS_EVENT_COBO_DUNGEON_FIELD_ACK );
	return true;
}
bool CX2StateMenu::Handler_EGS_EVENT_COBO_DUNGEON_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_COBO_DUNGEON_FIELD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
	if( g_pMain->DeleteServerPacket( EGS_EVENT_COBO_DUNGEON_FIELD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//���� ������ ������ ����.
			g_pInstanceData->SetStartUI(kEvent.m_bStartUI);
			g_pInstanceData->SetDungeonCountUI(kEvent.m_DungeonCountUI);
			g_pInstanceData->SetFieldCountUI(kEvent.m_FieldCountUI);
			g_pInstanceData->SetDungeonCount(kEvent.m_DungeonCount);
			g_pInstanceData->SetFieldCount(kEvent.m_FieldMonsterKillCount);
			g_pInstanceData->SetRemaindTime(kEvent.m_iRemaindTime);
			g_pInstanceData->SetButtonPushTime(kEvent.m_tPushTime);
			g_pInstanceData->SetSecondTime(0);
		}
		else
		{
			//�ϴ� ���� ���� ��Ȱ�� ó�� �������
			if(g_pInstanceData != NULL)
			{
				g_pInstanceData->SetStartUI(false);
				g_pInstanceData->SetRemaindTime(-1); //���� �ϸ� ���� ���� �ʴ´� �ٽ� ������ �Ѵ�.
			}
		}
	}
	return true;
}
void CX2StateMenu::ShowCoboEventUI(float TempTime)
{
	//�̺�Ʈ UIȰ��ȭ �����Ǵ�
	if( g_pInstanceData == NULL || m_pDLGCoboEventUI == NULL || m_pDLGCoboEventCountUI == NULL)
	{
		return;
	}
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGCoboEventCountUI->GetControl(L"StaticCoboCount"));
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL && g_pData->GetMyUser()->GetSelectUnit()->GetUnitData() != NULL)
	{
		if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData()->m_Level < 10 )
		{
			if( pStatic != NULL )
			{
				//�ϴ� �Ⱥ����ֵ��� �ʱ�ȭ.
				for(int i=0; i<11; ++i) // 4~5 : ������ ų��,  6~7: ����� ų��
				{
					if( NULL == pStatic->GetPicture(i) )
						return;

					pStatic->GetPicture(i)->SetShow(false);
				}
			}
			return;
		}
	}
	CKTDGUIButton* pCoboEventStartButton = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_B")  );
	CKTDGUIButton* pCoboEventStartButtonGray = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_C")  );
	CKTDGUIButton* pCoboTimer = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_D")  );
	CKTDGUIButton* pCoboDungeonBG = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_E")  );
	CKTDGUIButton* pCoboDungeonBGGray = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_F")  );
	CKTDGUIButton* pCoboFieldCountBG = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_G")  );
	CKTDGUIButton* pCoboFieldCountBGGray = static_cast<CKTDGUIButton*>(m_pDLGCoboEventUI->GetControl(L"Static_RamadanCenter_H")  );
	IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
	{
		if( (g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true) || GetUsedCashShop() == true)
		{
			if(pCoboEventStartButton != NULL )
			{
				pCoboEventStartButton->SetShow(false); //�̺�Ʈ Ȱ��ȭ ��ư ����
			}
			if(pCoboEventStartButtonGray != NULL )
			{
				pCoboEventStartButtonGray->SetShow(false); //��Ȱ�� ��ư�� Ų��
			}
			if( pCoboTimer != NULL)
			{
				pCoboTimer->SetShow(false);
			}
			if(pCoboDungeonBG != NULL)
			{
				pCoboDungeonBG->SetShow(false);
			}
			if( pCoboDungeonBGGray != NULL )
			{
				pCoboDungeonBGGray->SetShow(false);
			}
			if( pCoboFieldCountBG != NULL)
			{
				pCoboFieldCountBG->SetShow(false);
			}
			if(pCoboFieldCountBGGray != NULL)
			{
				pCoboFieldCountBGGray->SetShow(false);
			}
			if( pStatic != NULL )
			{
				//�ϴ� �Ⱥ����ֵ��� �ʱ�ȭ.
				for(int i=0; i<11; ++i) // 4~5 : ������ ų��,  6~7: ����� ų��
				{
					if( NULL == pStatic->GetPicture(i) )
						return;

					pStatic->GetPicture(i)->SetShow(false);
				}
			}
			return;
		}
		///���⼭ ��Ȱ��ȭ �ð� üũ ����
		///���⼭ �ð��� 23��30�п��� 00�� ���̶�� ��ư ��Ȱ��ȭ ó�� �Ѵ�.
		CTime tLimitTime(g_pData->GetServerCurrentTime64());
		if(tLimitTime.GetHour() == 23 && tLimitTime.GetMinute() >= 30 && tLimitTime.GetMinute() <= 59)
		{
			if(pCoboEventStartButton != NULL )
			{
				pCoboEventStartButton->SetShow(false); //�̺�Ʈ Ȱ��ȭ ��ư ����
			}
			if(pCoboEventStartButtonGray != NULL )
			{
				pCoboEventStartButtonGray->SetShow(true); //��Ȱ�� ��ư�� Ų��
			}
			if( pCoboTimer != NULL)
			{
				pCoboTimer->SetShow(false);
			}
			if(pCoboDungeonBG != NULL)
			{
				pCoboDungeonBG->SetShow(false);
			}
			if( pCoboDungeonBGGray != NULL )
			{
				pCoboDungeonBGGray->SetShow(true);
			}
			if( pCoboFieldCountBG != NULL)
			{
				pCoboFieldCountBG->SetShow(false);
			}
			if(pCoboFieldCountBGGray != NULL)
			{
				pCoboFieldCountBGGray->SetShow(true);
			}
			if( pStatic != NULL )
			{
				//�ϴ� �Ⱥ����ֵ��� �ʱ�ȭ.
				for(int i=0; i<11; ++i) // 4~5 : ������ ų��,  6~7: ����� ų��
				{
					if( NULL == pStatic->GetPicture(i) )
						return;

					pStatic->GetPicture(i)->SetShow(false);
				}
			}
			return;
		}
		else
		{
			if(pCoboEventStartButtonGray != NULL )
			{
				pCoboEventStartButtonGray->SetShow(false); //��Ȱ�� ��ư�� ����
			}
			if( pCoboDungeonBGGray != NULL )
			{
				pCoboDungeonBGGray->SetShow(false);
			}
			if(pCoboFieldCountBGGray != NULL)
			{
				pCoboFieldCountBGGray->SetShow(false);
			}
			if( pCoboTimer != NULL)
			{
				pCoboTimer->SetShow(false);
			}
		}

		if( g_pInstanceData->GetStartUI() == true ) //�̺�Ʈ ��ŸƮ ��ư Ȱ��ȭ
		{
			//��ŸƮ ��ư�� Ȱ��ȭ ��Ų��.
			if( pCoboEventStartButton != NULL )
			{			
				pCoboEventStartButton->SetShow(true);
			}
		}
		else
		{
			if( pCoboEventStartButton != NULL )
			{
				pCoboEventStartButton->SetShow(false);
			}
			if(pCoboEventStartButtonGray != NULL )
			{
				if( g_pInstanceData->GetRemaindTime() <= 0 )
					pCoboEventStartButtonGray->SetShow(true); //��Ȱ�� ��ư�� ����
			}

		}
		if( pStatic != NULL )
		{
			//�ϴ� �Ⱥ����ֵ��� �ʱ�ȭ.
			for(int i=0; i<11; ++i) // 4~5 : ������ ų��,  6~7: ����� ų��
			{
				if( NULL == pStatic->GetPicture(i) )
					return;

				pStatic->GetPicture(i)->SetShow(false);
			}
			const wstring MAGIC_TEXTURE_KEY[] = {	L"_NUM_0",
				L"_NUM_1",
				L"_NUM_2",
				L"_NUM_3",
				L"_NUM_4",
				L"_NUM_5",
				L"_NUM_6",
				L"_NUM_7",
				L"_NUM_8",
				L"_NUM_9" };

			if( g_pInstanceData->GetDungeonCountUI() == true ) //���� ī��Ʈ UI�� Ȱ��ȭ ��Ų��.
			{
				//���� ī��Ʈ ��׶��� Ȱ��ȭ ��Ű��
				if(pCoboDungeonBG != NULL)
				{
					pCoboDungeonBG->SetShow(true);
				}
				///�ϴ� ��Ʈ�� ��� ����
				///���� ī��Ʈ �� ����
				int iDungeonCountBag = (g_pInstanceData->GetDungeonCount() / 100 ) % 10;
				int iDungeonCountTen = (g_pInstanceData->GetDungeonCount() / 10 ) % 10;
				int iDungeonCount = g_pInstanceData->GetDungeonCount() % 10;
				wstring KeyName;
				if( iDungeonCountBag > 0 )
				{
					KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iDungeonCountBag];
					pStatic->GetPicture(3)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(3)->SetShow(true);
					KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iDungeonCountTen];
					pStatic->GetPicture(4)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(4)->SetShow(true);
				}
				if( iDungeonCountTen > 0 )
				{
					KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iDungeonCountTen];
					pStatic->GetPicture(4)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(4)->SetShow(true);
				}
				KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iDungeonCount] ;
				pStatic->GetPicture(5)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(5)->SetShow(true);

			}
			else
			{
				//���� ī��Ʈ ��׶��� ��Ȱ��ȭ ��Ű��
				if(pCoboDungeonBG != NULL)
				{
					pCoboDungeonBG->SetShow(false);
				}
				if( pCoboDungeonBGGray != NULL )
				{
					pCoboDungeonBGGray->SetShow(true);
				}
			}
			if( g_pInstanceData->GetFieldCountUI() ==  true ) //�ʵ� ī��Ʈ UI�� Ȱ��ȭ ��Ų��.
			{
				if( pCoboFieldCountBG != NULL)
				{
					pCoboFieldCountBG->SetShow(true);
				}
				int iFieldCountbag = (g_pInstanceData->GetFieldCount() / 100 ) % 10;
				int iFieldCountTen = (g_pInstanceData->GetFieldCount() / 10 ) % 10;
				int iFieldCount = g_pInstanceData->GetFieldCount() % 10;
				wstring KeyName;
				if( iFieldCountbag > 0 )
				{
					KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iFieldCountbag];
					pStatic->GetPicture(6)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(6)->SetShow(true);
					KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iFieldCountTen];
					pStatic->GetPicture(7)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(7)->SetShow(true);
				}
				if( iFieldCountTen > 0 )
				{
					KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iFieldCountTen];
					pStatic->GetPicture(7)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(7)->SetShow(true);
				}
				KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iFieldCount] ;
				pStatic->GetPicture(8)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(8)->SetShow(true);

			}
			else
			{
				if( pCoboFieldCountBG != NULL)
				{
					pCoboFieldCountBG->SetShow(false);
				}
				if(pCoboFieldCountBGGray != NULL)
				{
					pCoboFieldCountBGGray->SetShow(true);
				}
			}
			///�ð� ǥ�ø� ����� �ϴµ� �̰� ���� �ð��� �ְ� ��ŸƮ ��ư�� ��Ȱ���̶�� �����ش�
			__time64_t tNowTime(g_pData->GetServerCurrentTime64());//������ �ð��� �޾ƿ´�.
			CTimeSpan tItemGiveTime = tNowTime - g_pInstanceData->GetButtonPushTime(); //�帥�ð� 
			if( g_pInstanceData->GetStartUI() == false && g_pInstanceData->GetRemaindTime() >= tItemGiveTime.GetTotalMinutes() )
			{
				if( g_pInstanceData->GetRemaindTime() < 0 )
				{
					wstring KeyName;
					KeyName = L"RED" + MAGIC_TEXTURE_KEY[0];
					pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(2)->SetShow(true);
					// : �߰�
					KeyName = L"EVENT_TIME";
					pStatic->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(0)->SetShow(true);
					KeyName = L"RED" + MAGIC_TEXTURE_KEY[0];
					pStatic->GetPicture(9)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(9)->SetShow(true);			
					KeyName = L"RED" + MAGIC_TEXTURE_KEY[0] ;
					pStatic->GetPicture(10)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(10)->SetShow(true);
				}
				else
				{
					if( pCoboTimer != NULL)
					{
						pCoboTimer->SetShow(true);
					}

					__time64_t tNowTime(g_pData->GetServerCurrentTime64());//������ �ð��� �޾ƿ´�.
					CTimeSpan tItemGiveTime = tNowTime - g_pInstanceData->GetButtonPushTime(); //�帥�ð� 
					int TempRemainTime = g_pInstanceData->GetRemaindTime() - 1; //9�к��� ǥ�ð� �Ǿ� �ϱ� ������ �ʶ�����
					int iMinutes = TempRemainTime - tItemGiveTime.GetMinutes();
					if( iMinutes <= 0)
					{
						iMinutes = 0;
					}
					int iSecond =  60 - tItemGiveTime.GetSeconds();
					if( iSecond >= 60 )
					{
						iSecond = 59;
					}

					int iRemaindTimeTen = (iMinutes / 10 ) % 10;
					int iRemaindTime = iMinutes % 10;
					wstring KeyName;
					if( iRemaindTimeTen > 0 )
					{
						KeyName = L"RED" + MAGIC_TEXTURE_KEY[iRemaindTimeTen];
						pStatic->GetPicture(1)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
						pStatic->GetPicture(1)->SetShow(true);

					}
					KeyName = L"RED" + MAGIC_TEXTURE_KEY[iRemaindTime] ;
					pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(2)->SetShow(true);
					// : �߰�
					KeyName = L"EVENT_TIME";
					pStatic->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(0)->SetShow(true);

					int iSecondTimeTen = (iSecond / 10 ) % 10;
					int iSecondTime = iSecond % 10;

					KeyName = L"RED" + MAGIC_TEXTURE_KEY[iSecondTimeTen];
					pStatic->GetPicture(9)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(9)->SetShow(true);			
					KeyName = L"RED" + MAGIC_TEXTURE_KEY[iSecondTime] ;
					pStatic->GetPicture(10)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
					pStatic->GetPicture(10)->SetShow(true);


				}

			}
		}
	}
	ELSE
	{
		if(pCoboEventStartButton != NULL )
		{
			pCoboEventStartButton->SetShow(false); //�̺�Ʈ Ȱ��ȭ ��ư ����
		}
		if(pCoboEventStartButtonGray != NULL )
		{
			pCoboEventStartButtonGray->SetShow(false); //��Ȱ�� ��ư�� Ų��
		}
		if( pCoboTimer != NULL)
		{
			pCoboTimer->SetShow(false);
		}
		if(pCoboDungeonBG != NULL)
		{
			pCoboDungeonBG->SetShow(false);
		}
		if( pCoboDungeonBGGray != NULL )
		{
			pCoboDungeonBGGray->SetShow(false);
		}
		if( pCoboFieldCountBG != NULL)
		{
			pCoboFieldCountBG->SetShow(false);
		}
		if(pCoboFieldCountBGGray != NULL)
		{
			pCoboFieldCountBGGray->SetShow(false);
		}
		if( pStatic != NULL )
		{
			//�ϴ� �Ⱥ����ֵ��� �ʱ�ȭ.
			for(int i=0; i<11; ++i) // 4~5 : ������ ų��,  6~7: ����� ų��
			{
				if( NULL == pStatic->GetPicture(i) )
					return;

				pStatic->GetPicture(i)->SetShow(false);
			}
		}
	}
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD


#ifdef SERV_NEW_YEAR_EVENT_2014
bool CX2StateMenu::Handler_EGS_2013_EVENT_MISSION_COMPLETE_REQ( IN bool bLevelUpEvent )
{
	KEGS_2013_EVENT_MISSION_COMPLETE_REQ kPacket;
	kPacket.m_bLevelUpEvent = bLevelUpEvent;

	g_pData->GetServerProtocol()->SendPacket( EGS_2013_EVENT_MISSION_COMPLETE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_2013_EVENT_MISSION_COMPLETE_ACK );

	return true;
}
bool CX2StateMenu::Handler_EGS_2013_EVENT_MISSION_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_2013_EVENT_MISSION_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_2013_EVENT_MISSION_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// ������� ���� ����
			if( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				if( g_pData->GetMyUser()->GetSelectUnit()->GetOldYearMissionRewardedLevel() < kEvent.m_iRewardedLevel )
				{
					g_pData->GetMyUser()->GetSelectUnit()->SetOldYearMissionRewardedLevel( kEvent.m_iRewardedLevel );
					g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
				}
			}

			// ������ �κ��丮 ����
			if( kEvent.m_vecInventorySlotInfo.empty() == false )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				if( NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
			}

			return true;
		}
	}

	return false;
}

bool CX2StateMenu::Handler_EGS_2014_EVENT_MISSION_COMPLETE_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_2014_EVENT_MISSION_COMPLETE_REQ );
	g_pMain->AddServerPacket( EGS_2014_EVENT_MISSION_COMPLETE_ACK );

	return true;
}
bool CX2StateMenu::Handler_EGS_2014_EVENT_MISSION_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_2014_EVENT_MISSION_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_2014_EVENT_MISSION_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// �̼� �ܰ� ����
			if( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				int iStepID = g_pData->GetMyUser()->GetSelectUnit()->GetNewYearMissionStepID();
				g_pData->GetMyUser()->GetSelectUnit()->SetNewYearMissionStepID( iStepID + 1 );
			}

			g_pMain->GetMemoryHolder()->UpdateNewYear2014Event();
			return true;
		}
	}

	return false;
}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_CONTENT_MANAGER_INT
bool CX2StateMenu::Handler_EGS_CASH_SHOP_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CASH_SHOP_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if (m_pDLGMenu != NULL)
	{
		for( int i=0; i<m_pDLGMenu->GetControlSize(); i++ )
		{
			CKTDGUIControl* pControl = m_pDLGMenu->GetControl( i );
			if( NULL == pControl )
				continue;

			if( CKTDGUIControl::UCT_BUTTON != pControl->GetType() )
				continue;

			CKTDGUIButton* pButton = (CKTDGUIButton*) pControl;
			switch(	pButton->GetCustomMsgMouseUp() )
			{
			case CX2StateMenu::SMUCM_CASH_SHOP:
				{
					pButton->SetEnable(kEvent.m_bCashShopOpen);
				} break;
			}
		}
	}
		

	return false;
}
#endif //SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_CHECK_POWER
bool CX2StateMenu::Handler_EGS_START_CHECK_POWER_REQ( bool bStart )
{
	KEGS_START_CHECK_POWER_REQ kPacket;
	kPacket.m_bStart = bStart;

	g_pData->GetServerProtocol()->SendPacket( EGS_START_CHECK_POWER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_START_CHECK_POWER_ACK );

	return true;
}
bool CX2StateMenu::Handler_EGS_START_CHECK_POWER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_START_CHECK_POWER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_START_CHECK_POWER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// �˾� ���� ����
			if( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->SetShowCheckPowerPopUp( false );
			}

			g_pMain->GetMemoryHolder()->UpdateCheckPowerEvent();
			return true;
		}
	}

	return false;
}
bool CX2StateMenu::Handler_EGS_UPDATE_CHECK_POWER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_CHECK_POWER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// ����� �� ����
	if( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		g_pData->GetMyUser()->GetSelectUnit()->SetCheckPowerCount( kEvent.m_ucCheckPowerCount );
		g_pData->GetMyUser()->GetSelectUnit()->SetCheckPowerScore( kEvent.m_ucCheckPowerScore );
		g_pData->GetMyUser()->GetSelectUnit()->SetCheckPowerTime( kEvent.m_iCheckPowerTime );

		g_pMain->GetMemoryHolder()->UpdateCheckPowerEvent();
		return true;
	}

	return false;
}

bool CX2StateMenu::Handler_EGS_CHECK_POWER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_CHECK_POWER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// ����� �� ����
	if( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_29863, "i", static_cast<int>( kEvent.m_ucCheckPowerScore ) ) ), g_pMain->GetNowState() );

		return true;
	}

	return false;
}

#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
bool CX2StateMenu::Handler_EGS_EVENT_CHUNG_GIVE_ITEM_REQ( int iChoice )
{
	KEGS_EVENT_CHUNG_GIVE_ITEM_REQ kPacket;
	kPacket.iChoice = iChoice;
	kPacket.bTwoGiveItem = g_pInstanceData->GetNextGiveItem();

	///�ѹ� �޾����ϱ� ��Ȱ��ó��
	if( g_pInstanceData != NULL )
	{
		g_pInstanceData->SetChungUIShow(false);
	}
	g_pData->GetServerProtocol()->SendPacket( EGS_EVENT_CHUNG_GIVE_ITEM_REQ , kPacket );
	g_pMain->AddServerPacket( EGS_EVENT_CHUNG_GIVE_ITEM_ACK );
	return true;
}
bool CX2StateMenu::Handler_EGS_EVENT_CHUNG_GIVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EVENT_CHUNG_GIVE_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	
	if( g_pMain->DeleteServerPacket( EGS_EVENT_CHUNG_GIVE_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// ��Ȱ�� ó���� �����ϱ� ������ ǥ������ �ð��� üũ �ؾ��Ѵ�.
			if( g_pInstanceData != NULL )
				g_pInstanceData->SetToolTipTime(kEvent.m_wstrGetItemTime);
			return true;
		}
		else
		{
			if( g_pInstanceData != NULL )
				g_pInstanceData->SetChungUIShow(true);
			return false;
		}
	}
	return false;
}
void CX2StateMenu::ShowChungGiveItem_UI(void)
{
	if(m_pDLGChungItemUI == NULL)
	{
		return;
	}
	wstringstream wstrstm;
	std::wstring TempString;
	CTime GetItemTime;
	CTime GetServerTime(g_pData->GetServerCurrentTime64());
	int iNextTime = 0;
	CKTDGUIButton* pChungGiveItem_One_A = static_cast<CKTDGUIButton*>(m_pDLGChungItemUI->GetControl(L"Static_RamadanCenter_A")  );
	CKTDGUIButton* pChungGiveItem_One_B = static_cast<CKTDGUIButton*>(m_pDLGChungItemUI->GetControl(L"Static_RamadanCenter_B")  );
	//1��
	CKTDGUIButton* pChungGiveItem_Two_A = static_cast<CKTDGUIButton*>(m_pDLGChungItemUI->GetControl(L"Static_RamadanLeft_A")  );
	CKTDGUIButton* pChungGiveItem_Two_B = static_cast<CKTDGUIButton*>(m_pDLGChungItemUI->GetControl(L"Static_RamadanLeft_B")  );
	//2��
	CKTDGUIButton* pChungGiveItem_Tree_A = static_cast<CKTDGUIButton*>(m_pDLGChungItemUI->GetControl(L"Static_RamadanRight_A")  );
	CKTDGUIButton* pChungGiveItem_Tree_B = static_cast<CKTDGUIButton*>(m_pDLGChungItemUI->GetControl(L"Static_RamadanRight_B")  );
	if(KncUtil::ConvertStringToCTime( g_pInstanceData->GetToolTipTime(), GetItemTime ) ==  false )
	{
		iNextTime = 0;
	}
	else
	{
		CTimeSpan cNextGetItemTime = GetServerTime - GetItemTime; //���� �ð����� ������ ���� �ð��� ����
		if( cNextGetItemTime.GetDays() < 7 )
		{
			iNextTime = 7 - static_cast<int>(cNextGetItemTime.GetDays());
		}
		else 
		{
			iNextTime = 0;
		}
	}
	
	if( (g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true) || GetUsedCashShop() == true)
	{
		if( pChungGiveItem_One_A != NULL)
			pChungGiveItem_One_A->SetShow(false);
		if( pChungGiveItem_One_B != NULL )
			pChungGiveItem_One_B->SetShow(false);
		//1��
		if( pChungGiveItem_Two_A != NULL )
			pChungGiveItem_Two_A->SetShow(false);
		if( pChungGiveItem_Two_B != NULL )
			pChungGiveItem_Two_B->SetShow(false);
		//2��
		if( pChungGiveItem_Tree_A != NULL )
			pChungGiveItem_Tree_A->SetShow(false);
		if( pChungGiveItem_Tree_B != NULL )
			pChungGiveItem_Tree_B->SetShow(false);

		return;
	}
	if( g_pInstanceData != NULL && g_pInstanceData->GetChungUIShow() == true )
	{
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_DUNGEON_GAME:
			{
				if( CX2Unit::UC_CHUNG_IRON_CANNON == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass()) //û ������
				{
					//������ Ȱ�� ��Ű�� ������ ��Ȱ��
					if( pChungGiveItem_One_A != NULL)
					pChungGiveItem_One_A->SetShow(false);
	
					if( pChungGiveItem_One_B != NULL )
					pChungGiveItem_One_B->SetShow(true);

					//1��
					if( pChungGiveItem_Two_A != NULL )
					pChungGiveItem_Two_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", 0 ) );
					pChungGiveItem_Two_A->SetGuideDesc(TempString.c_str());

					//���⼭ ���÷����ؼ� ���� ǥ�� 
					if( pChungGiveItem_Two_B != NULL )
					pChungGiveItem_Two_B->SetShow(false);

					//2��
					if( pChungGiveItem_Tree_A != NULL )
					pChungGiveItem_Tree_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", 0 ) );
					pChungGiveItem_Tree_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_Tree_B != NULL )
					pChungGiveItem_Tree_B->SetShow(false);

				}
				else if( CX2Unit::UC_CHUNG_FURY_GUARDIAN == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass() || CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass() )
				{
					//������ Ȱ�� ��Ű�� ������ ��Ȱ��
					if( pChungGiveItem_One_A != NULL)
					pChungGiveItem_One_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", 0 ) );
					pChungGiveItem_One_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_One_B != NULL )
					pChungGiveItem_One_B->SetShow(false);
					//1��
					
					if( pChungGiveItem_Two_A != NULL )
					pChungGiveItem_Two_A->SetShow(false);
					
					if( pChungGiveItem_Two_B != NULL )
					pChungGiveItem_Two_B->SetShow(true);
					//2��
					
					if( pChungGiveItem_Tree_A != NULL )
						pChungGiveItem_Tree_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", 0 ) );
					pChungGiveItem_Tree_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_Tree_B != NULL )
					pChungGiveItem_Tree_B->SetShow(false);
				}
				else if ( CX2Unit::UC_CHUNG_IRON_PALADIN == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass() || CX2Unit::UC_CHUNG_DEADLY_CHASER == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass() )
				{
					//û 2�� ���� Ȱ�� ��Ű�� ������ ��Ȱ��
					//������ Ȱ�� ��Ű�� ������ ��Ȱ��
				
					if( pChungGiveItem_One_A != NULL)
					pChungGiveItem_One_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", 0 ) );
					pChungGiveItem_One_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_One_B != NULL )
					pChungGiveItem_One_B->SetShow(false);
					//1��
					
					if( pChungGiveItem_Two_A != NULL )
					pChungGiveItem_Two_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", 0 ) );
					pChungGiveItem_Two_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_Two_B != NULL )
					pChungGiveItem_Two_B->SetShow(false);
					//2��
					
					if( pChungGiveItem_Tree_A != NULL )
					pChungGiveItem_Tree_A->SetShow(false);
					
					if( pChungGiveItem_Tree_B != NULL )
					pChungGiveItem_Tree_B->SetShow(true);
				}
				else
				{
					//���⼭�� �ٲ���� UI��ü�� ������ �ȵ�
					//������ Ȱ�� ��Ű�� ������ ��Ȱ��
					
					if( pChungGiveItem_One_A != NULL)
						pChungGiveItem_One_A->SetShow(false);
					
					if( pChungGiveItem_One_B != NULL )
						pChungGiveItem_One_B->SetShow(false);
					//1��
					
					if( pChungGiveItem_Two_A != NULL )
						pChungGiveItem_Two_A->SetShow(false);
					
					if( pChungGiveItem_Two_B != NULL )
						pChungGiveItem_Two_B->SetShow(false);
					//2��
					
					if( pChungGiveItem_Tree_A != NULL )
						pChungGiveItem_Tree_A->SetShow(false);
					
					if( pChungGiveItem_Tree_B != NULL )
						pChungGiveItem_Tree_B->SetShow(false);
				}
			}break;
		default:
			{
				//���⼭�� �ٲ���� UI��ü�� ������ �ȵ�
				//������ Ȱ�� ��Ű�� ������ ��Ȱ��
				
				if( pChungGiveItem_One_A != NULL)
				pChungGiveItem_One_A->SetShow(false);
				
				if( pChungGiveItem_One_B != NULL )
				pChungGiveItem_One_B->SetShow(false);
				//1��
				
				if( pChungGiveItem_Two_A != NULL )
				pChungGiveItem_Two_A->SetShow(false);
				
				if( pChungGiveItem_Two_B != NULL )
				pChungGiveItem_Two_B->SetShow(false);
				//2��
				
				if( pChungGiveItem_Tree_A != NULL )
				pChungGiveItem_Tree_A->SetShow(false);
				
				if( pChungGiveItem_Tree_B != NULL )
				pChungGiveItem_Tree_B->SetShow(false);
			}break;
		}
	}
	else
	{
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_DUNGEON_GAME:
			{
				//û�̸� ��Ȱ��
				if( CX2Unit::UC_CHUNG_IRON_CANNON == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass()
					|| CX2Unit::UC_CHUNG_FURY_GUARDIAN == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass() 
					|| CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass()
					||CX2Unit::UC_CHUNG_IRON_PALADIN == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass()
					|| CX2Unit::UC_CHUNG_DEADLY_CHASER == (CX2Unit::UNIT_CLASS)g_pInstanceData->GetChungUIClass() )
				{
					//���� ��Ȱ�� ó���ϸ� ��
					//������ Ȱ�� ��Ű�� ������ ��Ȱ��
					
					if( pChungGiveItem_One_A != NULL)
					pChungGiveItem_One_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", iNextTime ) );
					pChungGiveItem_One_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_One_B != NULL )
					pChungGiveItem_One_B->SetShow(false);
					//1��
					
					if( pChungGiveItem_Two_A != NULL )
					pChungGiveItem_Two_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", iNextTime ) );
					pChungGiveItem_Two_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_Two_B != NULL )
					pChungGiveItem_Two_B->SetShow(false);
					//2��
					
					if( pChungGiveItem_Tree_A != NULL )
					pChungGiveItem_Tree_A->SetShow(true);
					TempString = GET_REPLACED_STRING( ( STR_ID_28252, "i", iNextTime ) );
					pChungGiveItem_Tree_A->SetGuideDesc(TempString.c_str());
					
					if( pChungGiveItem_Tree_B != NULL )
					pChungGiveItem_Tree_B->SetShow(false);
				}
				else
				{
					//���⼭�� �ٲ���� UI��ü�� ������ �ȵ�
					//������ Ȱ�� ��Ű�� ������ ��Ȱ��
					
					if( pChungGiveItem_One_A != NULL)
						pChungGiveItem_One_A->SetShow(false);
					
					if( pChungGiveItem_One_B != NULL )
						pChungGiveItem_One_B->SetShow(false);
					//1��
					
					if( pChungGiveItem_Two_A != NULL )
						pChungGiveItem_Two_A->SetShow(false);
					
					if( pChungGiveItem_Two_B != NULL )
						pChungGiveItem_Two_B->SetShow(false);
					//2��
					
					if( pChungGiveItem_Tree_A != NULL )
						pChungGiveItem_Tree_A->SetShow(false);
					
					if( pChungGiveItem_Tree_B != NULL )
						pChungGiveItem_Tree_B->SetShow(false);
				}
			}break;
		default:
			{
				//���⼭�� �ٲ���� UI��ü�� ������ �ȵ�
				//������ Ȱ�� ��Ű�� ������ ��Ȱ��
				
				if( pChungGiveItem_One_A != NULL)
				pChungGiveItem_One_A->SetShow(false);
				
				if( pChungGiveItem_One_B != NULL )
				pChungGiveItem_One_B->SetShow(false);
				//1��
				
				if( pChungGiveItem_Two_A != NULL )
				pChungGiveItem_Two_A->SetShow(false);
				
				if( pChungGiveItem_Two_B != NULL )
				pChungGiveItem_Two_B->SetShow(false);
				//2��
				
				if( pChungGiveItem_Tree_A != NULL )
				pChungGiveItem_Tree_A->SetShow(false);
				
				if( pChungGiveItem_Tree_B != NULL )
				pChungGiveItem_Tree_B->SetShow(false);
			}break;
		}
	}
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
void CX2StateMenu::SetShowAdamsUI(void)
{
	if( m_pDLGAdamsEventShopUI == NULL )
	{
		return;
	}
	CKTDGUIButton* pCoboEventShopOpen = static_cast<CKTDGUIButton*>(m_pDLGAdamsEventShopUI->GetControl(L"Static_AdamsShop")  );
	IF_EVENT_ENABLED( CEI_EVENT_ADAMS_SHOP )
	{
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == false )
				{
					if( g_pData->GetUIManager()->GetbShowAdamsShop() != true ) //���� �� NPC�� ���� ���� �ʴٸ�
					{
						g_pInstanceData->SetAdamsEventShopUIShow(false);
					}
				}
				if( pCoboEventShopOpen != NULL )
				{
					if(g_pInstanceData->GetAdamsEventShopUIShow() == true || (g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true) || GetUsedCashShop() == true)
					{
						pCoboEventShopOpen->SetShow(false);
					}
					else
					{
						pCoboEventShopOpen->SetShow(true);
					}
				}
			}break;
		default:
			{
				if( pCoboEventShopOpen != NULL)
				{
					pCoboEventShopOpen->SetShow(false);
				}
				if( g_pInstanceData != NULL )
				{
					g_pInstanceData->SetAdamsEventShopUIShow(false);
				}
			}break;
		}
	}
	ELSE
	{
		if( pCoboEventShopOpen != NULL)
		{
			pCoboEventShopOpen->SetShow(false);
		}
	}
}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef SERV_4TH_ANNIVERSARY_EVENT
bool CX2StateMenu::Handler_EGS_4TH_ANNIV_EVENT_REWARD_REQ( IN int iSelectedIndex )
{
	KEGS_4TH_ANNIV_EVENT_REWARD_REQ kPacket;
	kPacket.m_iSeletedIndex = iSelectedIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_4TH_ANNIV_EVENT_REWARD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK );

	return true;
}
bool CX2StateMenu::Handler_EGS_4TH_ANNIV_EVENT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_4TH_ANNIV_EVENT_REWARD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pInstanceData->Set4thRewarded( kEvent.m_iSeletedIndex );
			g_pMain->GetMemoryHolder()->Update4thEvent();

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_30427 ), this );
		}
	}

	return true;
}
#endif //SERV_4TH_ANNIVERSARY_EVENT
