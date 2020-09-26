#include "StdAfx.h"



#include ".\x2TFieldGame.h"

CX2TFieldGame::CX2TFieldGame( CX2World::WORLD_ID worldID, SquareData* pSquareData )
: m_pNpcShadow( NULL )
, m_pDLGFieldName( NULL )
, m_pDLGMsgBox( NULL )
, m_pSquareUI( NULL )
, m_pWorld( NULL )
, m_pCamera( NULL )
, m_pMyUnit()
, m_pUiParticle( NULL )
, m_pUiXMeshPlayer( NULL )
, m_pFontForUnitName( NULL )
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT		
, m_pFont( NULL )
#endif	KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT		
, m_pTalkBoxManager( NULL )
, m_vecFieldNpc()
, m_pSubAniXSkinAnim( NULL )
, m_pSubAniMesh( NULL )	
, m_pSubSound( NULL )
//#ifndef MODIFY_PORTAL_GATE
//, m_pPortalGateParticle1(NULL)
//, m_pPortalGateParticle2(NULL)
//, m_pPortalGateParticle3(NULL)
//, m_pPortalGateParticle4(NULL)
//, m_pPortalGateParticle5(NULL)
//#endif  MODIFY_PORTAL_GATE
//, m_pInAreaParticle( NULL )
//, m_pOtherLineParticle( NULL )
#ifndef REFORM_NOVICE_GUIDE
, m_pNoviceGuide( NULL )
#endif //REFORM_NOVICE_GUIDE
, m_pDLGLoadingState( NULL )
, m_pCX2GameLoadingTip( NULL )
//#ifdef MODIFY_PORTAL_GATE
, m_uiPortalCount(0)
//#endif //MODIFY_PORTAL_GATE
#ifdef SERV_MOMOTI_EVENT
,m_pDLGMomotiQuizEvent( NULL )
,m_MomotiQuizEventMsg(L"")
,m_iInputDLGQuiz(0)
#endif //SERV_MOMOTI_EVENT
{
	g_pTFieldGame			= this;

	if(pSquareData != NULL)
	{
		m_SquareData.CopySquareData( pSquareData );
	}

	if( NULL != g_pInstanceData )
	{
		switch( g_pData->GetLocationManager()->GetCurrentVillageID() )
		{
		case SEnum::VMI_DUNGEON_LOUNGE_RUBEN:
		case SEnum::VMI_DUNGEON_LOUNGE_ELDER:
		case SEnum::VMI_DUNGEON_LOUNGE_BESMA:
		case SEnum::VMI_DUNGEON_LOUNGE_ALTERA:
		case SEnum::VMI_DUNGEON_LOUNGE_PEITA:
		case SEnum::VMI_DUNGEON_LOUNGE_VELDER:
			//{{ JHKang / ������ / 2010/12/28 / �ϸ� ���� �����
#ifdef ADD_HAMEL_VILLAGE
		case SEnum::VMI_DUNGEON_LOUNGE_HAMEL:
#endif ADD_HAMEL_VILLAGE
			//}} JHKang / ������ / 2010/12/28 / �ϸ� ���� �����
			{
				g_pInstanceData->SetLoadedDungeonLoungeUnits( false );
				g_pInstanceData->SetIsDungeonLounge( true );

			} break;

		default:
			{
				g_pInstanceData->SetIsDungeonLounge( false );
			} break;
		}
	}

	m_pDLGFieldName	= NULL;	
#ifdef COME_BACK_REWARD
	m_pDLGBuffName	= NULL;	
#endif
	
	m_pWorld	= NULL;
	m_pCamera	= NULL;
	//m_pMyUnit	= NULL;
	m_pMyUnit.reset();

	m_pUiParticle			= NULL;
	//m_pMajorParticle		= NULL;
	//m_pMinorParticle		= NULL;
	//m_pMajorXMeshPlayer		= NULL;
	//m_pMinorXMeshPlayer		= NULL;
	m_pFontForUnitName		= NULL;

	m_pSubAniMesh			= NULL;
	m_pSubAniXSkinAnim		= NULL;
	m_pSubSound				= NULL;
#ifdef APINK_NPC
	m_pAPinkSound			= NULL;
#endif

	if(pSquareData != NULL)
		m_MaxCount				= m_SquareData.m_MaxUser;
	else
		m_MaxCount				= 5000;
	m_fLoadUiTime			= 0.f;
	m_fFieldNameTime		= 0.f;
#ifdef COME_BACK_REWARD
	m_fBuffNameTime			= 0.f;
#endif
	m_fMarketTime			= 0.f;

	m_fStateSyncTime		= 0.f;
	m_fMarketRefresh		= 0.f;			

	m_fOtherLineTime		= 0.f;
	m_bFreeCamera			= false;

	//m_pTalkBoxMgrForPartyLeader = new CX2TalkBoxManager( CX2TalkBoxManager::TBMT_PERSONAL_SHOP );
	//m_pTalkBoxManager		= new CX2TalkBoxManager;
	m_pTalkBoxManager			= CX2TalkBoxManager::CreateTalkBoxManger( CX2TalkBoxManager::TBMT_DEFAULT );
    ASSERT( m_pTalkBoxManager != NULL );

	m_bAddMyUnit			= false;
	m_bForceSync			= true;

	SetVillageStartPosIdToBattleField( SEnum::VMI_INVALID );
	SetNearPortalToBattleField( false );
	
	m_iLastPos				= 0;
	m_bJoinNpc				= false;

	m_iJoinNpcIndex			= -1;	
	m_bJoinMarket			= false;
	m_bShowMarketList		= false;

	m_bLoadComplete			= false;
	m_bFindOtherLine		= false;

	m_pSquareUI				= NULL;
	m_pDLGMsgBox			= NULL;

	// �ʽ��� ���̵�	
	m_bUserHide = false;
#ifndef REFORM_NOVICE_GUIDE
	m_pNoviceGuide = NULL;
#endif //REFORM_NOVICE_GUIDE

	m_iJoinMarketIndex 		= -1;
	m_fCameraDistance		= 0.f;
	m_fLookAtHeight			= 0.f;
	m_fEyeHeight			= 0.f;


	CX2LocationManager::VillageTemplet* pVillage = g_pData->GetLocationManager()->GetVillageMapTemplet( g_pData->GetLocationManager()->GetCurrentVillageID() );
    ASSERT( pVillage != NULL );
    if ( pVillage != NULL )
    {
	    m_fCameraDistance		= pVillage->m_fCameraDistance;
	    m_fLookAtHeight			= pVillage->m_fLookAtHeight;
	    m_fEyeHeight			= pVillage->m_fEyeHeight;
    }//if

    /** @NOTE:

        ��׶��� �ε��� ����Ϸ��� ���带 ������ ��, CreateWorld()�� �ƴ�,
        ThreadRequest_CreateWorld()�� �����ؾ� �Ѵ�.
        ������, ������ ���ؼ��� urgent data�� ó���ϴ� �κ��� �������� �ʾұ�
        ������, ���� �����Ŀ� �ε��Ǵ� ���ҽ����� ���� ���̰� �� ���̴�.

        urgent data:
            ù ȭ�鿡�� �������� ��� �����͸� �ǹ��Ѵ�. ������ ��� ������ ó�� �������� ��
            �������� ��� �����͸� �ǹ��Ѵ�.
            
        - jintaeks on 2008-10-24, 10:31 */

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-18

    // ���忡 ���� thread �ε� �׽�Ʈ�� �ϰ� �ִ�. - jintaeks on 2008-10-18, 13:54
	m_pWorld	= g_pData->GetWorldManager()->CreateWorld( worldID, NULL, true );

//#else // BACKGROUND_LOADING_TEST // 2008-10-18
//
//    /** EnableWritingPreprocessingData()�� �ǹ�:
//        ��׶��� �ε��� �ϱ� ���ؼ��� ���� ��ü�� ��ó���� ������ �ʿ��ϴ�.
//        ���� - jintaeks on 2008-10-24, 10:34 - �� ��� �� ������, bounding sphere�� ������
//        �� �޽��� local �߽� ��ǥ�̴�. �� ������ "��ũ��Ʈ�����̸�.ppd"�� �����Ϸ���
//
//            EnableWritingPreprocessingData( true );
//
//        �� �����ϸ� �ȴ�.
//        �� �κ��� ���ӿ� �������� ����, ������ ���� �ۼ��Ǿ�� �� ���̴�.
//
//        - jintaeks on 2008-10-24, 10:34 */
//
//	//g_pData->GetWorldManager()->EnableWritingPreprocessingData( true );
//	m_pWorld = g_pData->GetWorldManager()->CreateWorld( worldID );
//    //g_pData->GetWorldManager()->EnableWritingPreprocessingData( false );
//    ASSERT( m_pWorld != NULL );
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-18

	m_pCamera		= new CX2Camera();
    ASSERT( m_pCamera != NULL );

	if( m_pWorld != NULL )
	{
#ifdef XMAS_UI
		switch( g_pData->GetLocationManager()->GetCurrentVillageID() )
		{
			//{{ oasis907 : ����� [2011.12.13]	ũ�������� ���� BGM - �����ؼ� ������ ������
		case SEnum::VMI_RUBEN:
		case SEnum::VMI_ELDER:
		case SEnum::VMI_BESMA:
		case SEnum::VMI_ALTERA:
		case SEnum::VMI_VELDER:
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_SANDER:
			{
				m_pWorld->SetBGM_LUA( "JingleBell_Square.ogg" );
			} break;
			//}}
		}
#endif
		
		m_pWorld->PlayBGM();
        if ( m_pCamera != NULL )
		    m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	}
	//m_pCamera->NomalDirectCamera( m_optrFocusUnit );



	srand((unsigned int)time(NULL));

	m_pUiParticle = g_pData->GetUIMajorParticle();
    ASSERT( m_pUiParticle != NULL );
	m_pUiXMeshPlayer = g_pData->GetUIMajorXMeshPlayer();
    ASSERT( m_pUiXMeshPlayer != NULL );

	m_pFontForUnitName = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );	
    ASSERT( m_pFontForUnitName != NULL );

	//m_bEnableKey			= true;
	m_bEnableKeyProcess		= true;
	m_bEnableCameraProcess	= true;

	g_pKTDXApp->GetDIManager()->SetEnable( true );




	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		if( NULL == g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap() )
		{
			g_pInstanceData->GetMiniMapUI()->CreateDungeonMiniMap();
		}
		
		if( NULL != g_pInstanceData->GetMiniMapUI() )
		{
			g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );
			g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->SetEyeDistance( 4000.f );
			g_pInstanceData->GetMiniMapUI()->SetVillageMapID( (int) g_pData->GetLocationManager()->GetCurrentVillageID() );
		}
	}

#ifndef DISABLE_DISAGREE_HACK_USER	// �ӱԼ� �Ϻ� ���� ���� #if 0 // �ʵ� ���Խ� ���ǽ� ������� ������ ���� �����Ƿ� ����
	if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	{
#ifndef PROCESSLIST
		g_pMain->UpdateProcessList();
#endif
		g_pMain->SendHackMail();
	}
#endif DISABLE_DISAGREE_HACK_USER
	

	// ���ϸ���Ʈ ui ����
	m_pSquareUI = new SquareUI( g_pMain->GetNowState() );
    ASSERT( m_pSquareUI != NULL );
    if ( m_pSquareUI != NULL )
	m_pSquareUI->SetShow(false);	

	// ������ ���̵� ����
//#ifndef MODIFY_PORTAL_GATE
//	CreatePortalGate();
//#endif //MODIFY_PORTAL_GATE
	m_hOtherLine			= GetUiParticle()->CreateSequenceHandle( NULL,  L"OtherRoad", 0.0f, 0.0f, 0.0f);
	CKTDGParticleSystem::CParticleEventSequence* pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hOtherLine );
    ASSERT( pSeqPortal != NULL );
	if( pSeqPortal != NULL )
	{		
        pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}
	

	// TCP Relay�� ����
	ConnectTR();

	AddFieldNpc();
	g_pData->GetLocationManager()->GetVecLinkedPos(m_vecLinkedPos);	
	g_pData->GetLocationManager()->GetVecMarketPos(m_vecMarketPos);	

	m_hMarketInArea = GetUiParticle()->CreateSequenceHandle( NULL,  L"MarketInArea", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMarket	= GetUiParticle()->GetInstanceSequence( m_hMarketInArea );
    ASSERT( pSeqMarket != NULL );
	if( pSeqMarket != NULL )
	{		
		pSeqMarket->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqMarket->SetShowObject(false);
	}	


	// ��Ƽ UI ���� ����Ÿ ����
	if( NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->OnChangeState();
	}	

	// ���� ����� �޽� �ε�	
	m_iSubAniTime = 0;
	OpenSubAni();			
		
	m_pDLGLoadingState = NULL;
	m_pCX2GameLoadingTip = NULL;
	
	OpenFieldName();
	OpenBuffName();
	LoadUI();

	m_pNpcShadow = new NpcJoinShadow();
    ASSERT( m_pNpcShadow != NULL );
	
	g_pData->GetUIManager()->OnFieldChange();


	m_fSlidePartyInfoTime = 0.f;	

	if( g_pData != NULL && g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->ResetUpdateList();

#ifdef NEW_VILLAGE_AUTO_SD_UNIT
	m_fAutoSDUnitTime = 0.f;
#endif


	g_pKTDXApp->SkipFrame();	

#ifdef OPTIMIZATION_DEV_MODE
	if( g_pMain != NULL )
		g_pMain->GetGameOption().SetFieldSD(true);
#endif OPTIMIZATION_DEV_MODE

	//{{ kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ ����
#ifdef	ADD_SERVER_GROUP
	//SaveScriptServerGroupFile();
    if ( g_pInstanceData != NULL )
        g_pInstanceData->SaveScriptServerGroupFile();
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ ����

	//{{ JHKang / ������ / 2011.6.26 / �޸� ���� ���� ���� �Ⱓ ǥ��
#ifdef COME_BACK_REWARD
	if ( NULL != g_pData->GetPremiumBuffManager() && true == g_pData->GetPremiumBuffManager()->GetViewBuffChat()
		&& true == g_pData->GetPremiumBuffManager()->IsComeBackUser() )
	{
		wstringstream wstreamDesc;
		wstreamDesc << GET_STRING( STR_ID_12577 ) << ' ' << GET_STRING( STR_ID_12581 );
		g_pChatBox->AddChatLog(  wstreamDesc.str().c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		g_pChatBox->AddChatLog( GET_STRING( STR_ID_12580 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_2699, "L", GetInt64ToDateDesc( g_pData->GetPremiumBuffManager()->GetComeBackBuffEnd(),g_pData->GetServerCurrentTime(), false ) ) ) ,	KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

		g_pData->GetPremiumBuffManager()->SetViewBuffChat( false );
	}
#endif
	//}}

	//{{ kimhc // �׳�ý��� �ູ, ��ų���� ü���� �޴� �Ⱓ�� ���� �˸� 
	if ( g_pInstanceData->GetShowCSPandSlotBDate() == true )	// ĳ���� ���� �� �Ŀ��� ������� �� ���� �� if�� �ȿ�
	{
		CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

#ifdef CLIENT_USE_XTRAP	// XTRAP - Ŭ�� �������� ȹ��
		char szServer[2], szCharType[2];	// ���� ����, ĳ�� ����
		_itoa(g_pInstanceData->GetServerGroupID(), szServer, 2);
		_itoa(pUnit->GetType(), szCharType, 2);
		CStringA strNickName, strUserName;	// wchar_t �� char �� ��ȯ�ϱ� ���ؼ� �����
		strNickName = pUnit->GetNickName();
		strUserName = g_pInstanceData->GetUserID().c_str();
		XTrap_C_SetUserInfoEx(				// ���� ������ �ִ´�.
			strUserName.GetBuffer(),
			szServer,
			strNickName.GetBuffer(),
			szCharType,
			NULL,
			NULL);
#endif	// CLIENT_USE_XTRAP

		if ( NULL != g_pChatBox &&
			NULL != pUnit )
		{
			if ( g_pData->GetUIManager() == NULL && 
				g_pData->GetUIManager()->GetUISkillTree() == NULL )
			{
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
				if( pUnit->IsTradeBlockUnit() == true )
					g_pChatBox->AddChatLog( pUnit->GetTradeBlockChatMsg().c_str(), KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR( 1,1,0,1 ), L"#CFFFF00" );
#endif SERV_NEW_UNIT_TRADE_LIMIT

				//{{ kimhc // �׳�ý��� �ູ�� ����ϴ� ������ �˾Ƴ��� ��ƾ
				bool	bUsingCSP		= false;
				CTime	cTime;
				KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, cTime );
				CTime	tCurrentTime	= g_pData->GetServerCurrentTime();

				if( tCurrentTime < cTime )
					bUsingCSP = true;
				//}} kimhc // �׳�ý��� �ູ�� ����ϴ� ������ �˾Ƴ��� ��ƾ

				if ( bUsingCSP == true )
				{
					g_pChatBox->AddChatLog( GET_STRING( STR_ID_2697 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

					g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_2699, "L", 
						GetExpirationDateDesc( pUnit->GetUnitData().m_wstrCSPointEndDate, g_pData->GetServerCurrentTime(), false ) ) ),
						KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				}

				if ( pUnit->GetUnitData().m_UserSkillTree.GetSkillSlotBExpirationState() == CX2UserSkillTree::SSBES_NOT_EXPIRED )
				{
					g_pChatBox->AddChatLog( GET_STRING( STR_ID_2698 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

					g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_2699, "L", 
						GetExpirationDateDesc( pUnit->GetUnitData().m_UserSkillTree.GetSkillSlotBEndDateString(), g_pData->GetServerCurrentTime(), false ) ) ),
						KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				}		

#ifdef SERV_PSHOP_AGENCY
				if( g_pInstanceData != NULL )
				{
					wstring wstrAgencyShopExpirationDate = g_pInstanceData->GetAgencyShopExpirationDate();

					if( wstrAgencyShopExpirationDate.empty() == false )
					{
						CTime	cAgencyTime;
						KncUtil::ConvertStringToCTime( wstrAgencyShopExpirationDate, cAgencyTime );
						if( tCurrentTime < cAgencyTime )
						{
							g_pChatBox->AddChatLog( GET_STRING( STR_ID_12146 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
							g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_2699, "L", 
								GetExpirationDateDesc( wstrAgencyShopExpirationDate, g_pData->GetServerCurrentTime(), false ) ) ),
								KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						}
					}
				}		
#endif

				g_pInstanceData->SetShowCSPandSlotBDate( false );

				//{{ ����� : [2009/11/3] //	���� �� ä�� ���� ǥ��
				CKTDGUIListBox* pListBox = g_pChatBox->GetCurrentChatbox();
				const float MAGIC_CHAT_SHOW_TIME = 50.0f;

				if( NULL != pListBox )
				{
					pListBox->SetScrollBarEndPos();
					pListBox->SetShowAllItems( true );
					pListBox->SetShowTimeAllItems( MAGIC_CHAT_SHOW_TIME );
				}
			}
		}
	}
	//}} kimhc // �׳�ý��� �ູ, ��ų���� ü���� �޴� �Ⱓ�� ���� �˸� 

	//{{ kimhc // 2010.3.10 //	���� ���⵵
#ifdef	IGA_TEST
	CX2User*			pUser		= g_pData->GetMyUser();

	CX2IGA::GetInstance()->SetUser( pUser->GetUserData().userID, pUser->GetUserAge(), pUser->IsMan() );
	
	CX2IGA::GetInstance()->CreateIgaInfoInVillage( g_pData->GetLocationManager()->GetCurrentVillageID() );
	CX2IGA::GetInstance()->Start();
#endif	IGA_TEST
	//}} kimhc // 2010.3.10 //	���� ���⵵

#ifdef SIGN_OVERRAP_BUG_FIX
	m_fDisSignToSign = 0;
	m_bSignOverRapCheck = false;
#endif SIGN_OVERRAP_BUG_FIX	

#ifdef ADD_MUSIC_SUB_VOLUME
	g_pMain->GetGameOption().SetMusicSubVolume(0.f);
#endif

#ifdef QUEST_GUIDE
	m_pNPCIndicator = new CX2NPCIndicator();
#endif //QUEST_GUIDE

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Start( true, true );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	//�����κ��� ��üũ Ȱ��ȭ ���θ� �˾Ƴ���.
	CX2StateField* pkStateField = static_cast<CX2StateField*>( g_pMain->GetNowState() );
	if( pkStateField )
		pkStateField->Handler_EGS_GET_ACTIVE_LAGCHECK_REQ();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
}

CX2TFieldGame::~CX2TFieldGame(void)
{			
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	//{{ kimhc // 2010.3.17 // ���� ���⵵ 
#ifdef	IGA_TEST
	CX2IGA::GetInstance()->End();
#endif	IGA_TEST
	//}} kimhc // 2010.3.17 // ���� ���⵵

	
g_pData->GetUIManager()->DestoryUITempInventory();
	m_pFontForUnitName = NULL;

#ifndef REFORM_NOVICE_GUIDE
	// �ʽ��� ���̵�
	SAFE_DELETE( m_pNoviceGuide );
#endif //REFORM_NOVICE_GUIDE

	GetUiParticle()->DestroyInstanceHandle( m_hOtherLine );	

//#ifdef MODIFY_PORTAL_GATE
	for( UINT i = 0; i < m_vecPortalGateParticle.size(); ++i )
	{
		GetUiParticle()->DestroyInstanceHandle( m_vecPortalGateParticle[i].m_hPortalGate1 );
		GetUiParticle()->DestroyInstanceHandle( m_vecPortalGateParticle[i].m_hPortalGate2 );
		GetUiParticle()->DestroyInstanceHandle( m_vecPortalGateParticle[i].m_hPortalGate3 );
		GetUiParticle()->DestroyInstanceHandle( m_vecPortalGateParticle[i].m_hPortalGate4 );
		GetUiParticle()->DestroyInstanceHandle( m_vecPortalGateParticle[i].m_hPortalGate5 );
	}
//#else
//	GetUiParticle()->DestroyInstanceHandle( m_hPortalGate1 );
//	GetUiParticle()->DestroyInstanceHandle( m_hPortalGate2 );
//	GetUiParticle()->DestroyInstanceHandle( m_hPortalGate3 );
//	GetUiParticle()->DestroyInstanceHandle( m_hPortalGate4 );
//	GetUiParticle()->DestroyInstanceHandle( m_hPortalGate5 );
//#endif //MODIFY_PORTAL_GATE
	
	GetUiParticle()->DestroyInstanceHandle( m_hMarketInArea );

	g_pData->GetUIEffectSet()->StopEffectSetAll();

	//SAFE_DELETE( m_pTalkBoxManager );
	//SAFE_DELETE( m_pTalkBoxMgrForPartyLeader );	

	//for(int i=0; i< m_pTalkBoxMgrForPartyLeader->GetTalkBoxNum(); ++i)
	//	m_pTalkBoxMgrForPartyLeader->DeleteByIndex(i);

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );
	
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];

		
		//SAFE_DELETE( pCX2SquareUnit );
		//CX2SquareUnit::DeleteKTDGObject( pCX2SquareUnit );
	}
	m_UserUnitList.clear();
//{{ robobeg : 2011-03-18
    m_listAddUserInUnitLoader.clear();
//}} robobeg : 2011-03-18

	for( int i=0; i<(int)m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
		//SAFE_DELETE( pNpcNode );
		//CX2TFieldNpc::DeleteKTDGObject( pNpcNode );
	}
	m_vecFieldNpc.clear();

	SAFE_DELETE( m_pWorld );
	SAFE_DELETE( m_pCamera );
	m_pMyUnit.reset();

	//SAFE_DELETE( m_pSubAniXSkinAnim );
	CKTDGXSkinAnim::DeleteKTDGObject( m_pSubAniXSkinAnim );
    m_pSubAniXSkinAnim = NULL;

	SAFE_CLOSE( m_pSubSound );
	SAFE_CLOSE( m_pSubAniMesh );

	SAFE_DELETE( m_pSquareUI );
	SAFE_DELETE( m_pNpcShadow );

#ifdef APINK_NPC
	if( m_pAPinkSound != NULL )
		m_pAPinkSound->Stop();
	SAFE_CLOSE( m_pAPinkSound );
#endif

	g_pKTDXApp->GetDIManager()->SetEnable( true );

	//g_pSquareGame = NULL;

	if( NULL != g_pData->GetUIUnitManager() )
	{
		g_pData->GetUIUnitManager()->ClearAllUIUnit();
	}

	if( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->SetHideWindow(false);

	m_vecLinkedPos.clear();
	m_vecMarketPos.clear();

	//SAFE_DELETE( m_pUiParticle );			// delete �ϸ� �ȵ�
	//SAFE_DELETE( m_pUiXMeshPlayer );		// delete �ϸ� �ȵ�
	//SAFE_DELETE( m_pMajorParticle );
	//SAFE_DELETE( m_pMinorParticle );
	//SAFE_DELETE( m_pMajorXMeshPlayer );
	//SAFE_DELETE( m_pMinorXMeshPlayer );

	SAFE_DELETE_DIALOG( m_pDLGFieldName );
#ifdef COME_BACK_REWARD
	SAFE_DELETE_DIALOG( m_pDLGBuffName );
#endif
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
	SAFE_DELETE( m_pCX2GameLoadingTip );

#ifdef ADD_MUSIC_SUB_VOLUME
	g_pMain->GetGameOption().SetMusicSubVolume(0.f);
#endif

#ifdef QUEST_GUIDE
	SAFE_DELETE( m_pNPCIndicator );
#endif //QUEST_GUIDE


#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{
		g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect(CX2RelationshipEffectManager::REST_HIDE_ALL);
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_MOMOTI_EVENT
	SAFE_DELETE_DIALOG( m_pDLGMomotiQuizEvent );
#endif //SERV_MOMOTI_EVENT

	g_pTFieldGame = NULL;	
}

void CX2TFieldGame::SetShowField(bool val)
{
	if(val == true)
		return;

	if(m_pSubAniXSkinAnim != NULL)
		m_pSubAniXSkinAnim->SetShowObject(false);
	if(m_pSquareUI != NULL)
		m_pSquareUI->SetShow(false);
	ShowJoinMarket(false, D3DXVECTOR3(0.f, 0.f, 0.f));
//#ifndef MODIFY_PORTAL_GATE
//	SetShowPortalGate(false);
//#endif //MODIFY_PORTAL_GATE
	

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];		
        if ( pCX2SquareUnit != NULL )
		    pCX2SquareUnit->SetShowObject( false );
	}

	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
        if ( pNpcNode != NULL )
		    pNpcNode->SetShowField(false);
	}
}

void CX2TFieldGame::OpenSubAni()
{
	SEnum::VILLAGE_MAP_ID iVillageId = g_pData->GetLocationManager()->GetCurrentVillageID();

	switch(iVillageId)
	{	
	case SEnum::VMI_RUBEN:
		//{{ 2010.12.13	�ڱ���		�纥 �������� ������ �ʴ� �ڵ带 ���� ������ ���� �ּ�ó��.
		m_pSubAniMesh = NULL;
		//m_iSubAniTime = 60;
		//m_pSubAniMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( "Ruben_village_training_subani.X" );
        //ASSERT( m_pSubAniMesh != NULL );
		//}}
		break;
	case SEnum::VMI_ELDER:
	case SEnum::VMI_BESMA:
		break;
	case SEnum::VMI_ALTERA:
		// 2013.11.07 ��â�� // ���׶� ���� ���� ���� �۾����� ���ܵǾ����Ƿ� �ּ� ó��.
		/*m_iSubAniTime = 60;
		m_pSubAniMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Altera_Village_SUB_npc.X" );
        ASSERT( m_pSubAniMesh != NULL );*/
		break;		
	case SEnum::VMI_PEITA:
		m_pSubAniMesh = NULL;	
		//m_iSubAniTime = 10;
		//m_pSubAniMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Besma_Peita_Dungeon_Gate_L.X" );
		break;
	}

	if( m_pSubAniMesh != NULL )
	{
		// ani name: Normal
        ASSERT( m_pSubAniXSkinAnim == NULL );
		m_pSubAniXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
        ASSERT( m_pSubAniXSkinAnim != NULL );
        if ( m_pSubAniXSkinAnim != NULL )
        {
		    m_pSubAniXSkinAnim->SetApplyMotionOffset( true );
		    m_pSubAniXSkinAnim->SetAnimXSkinMesh( m_pSubAniMesh );
		    m_pSubAniXSkinAnim->AddModelXSkinMesh( m_pSubAniMesh );
		    m_pSubAniXSkinAnim->SetShowObject(false);	
        }//if
	}	

	OpenSubAniSoundPlay();
}

void CX2TFieldGame::OpenSubAniSoundPlay()
{
	SEnum::VILLAGE_MAP_ID iVillageId = g_pData->GetLocationManager()->GetCurrentVillageID();
	
	switch(iVillageId)
	{	
	case SEnum::VMI_RUBEN:
#if 0 
		m_pSubSound = g_pKTDXApp->GetDeviceManager()->OpenSound( L"Amb_Ruben_TrainingCenter_Accident.ogg" );
		if( m_pSubSound != NULL )
		{
			m_pSubSound->Set3DPosition( D3DXVECTOR3(3838.95f, 1189.1f, -4210.15f) );
			m_pSubSound->SetMax3DDistance(2000.f);
		}		
#endif
		break;
	case SEnum::VMI_ELDER:
	case SEnum::VMI_BESMA:
		break;
	case SEnum::VMI_ALTERA:		
		break;		
	case SEnum::VMI_PEITA:		
		break;
	}
}

void CX2TFieldGame::SetStage(CKTDXStage *pStage)
{
	//SAFE_DELETE( m_pSquareUI );	

    ASSERT( pStage != NULL );

	if(m_pSquareUI == NULL)
	{
		m_pSquareUI = new SquareUI( pStage );
        ASSERT( m_pSquareUI != NULL );
        if ( m_pSquareUI != NULL )
		    m_pSquareUI->SetShow(false);
	}
    if ( m_pSquareUI != NULL )
	    m_pSquareUI->m_pStage = pStage;

	for(UINT i=0; i<m_vecFieldNpc.size(); ++i)
	{
        if ( m_vecFieldNpc[ i ] != NULL )
        {
		    if(m_vecFieldNpc[i]->GetNpcShop() != NULL)
		    {
			    m_vecFieldNpc[i]->GetNpcShop()->SetStage(pStage);
		    }
        }//if
	}
}

bool CX2TFieldGame::GetEnableKey()
{
#ifdef ATTRACTION_ITEM_TEST
	if( g_pData != NULL && g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayAttractionItem() == true )
		return false;
#endif
#ifdef SERV_TREASURE_BOX_ITEM
	if( g_pData != NULL && g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayTreasureBox() == true )
		return false;
#endif SERV_TREASURE_BOX_ITEM

	//{{ ����� : [2009/10/13] //	��� ���� UI ��½� Ű �Է� �ȸ������� ����
#ifdef GUILD_MANAGEMENT
	if( g_pData->GetGuildManager()->GetUIGuild()->GetShowGuildInfo() == true )
	{
		return false;
	}
#endif	//	GUILD_MANAGEMENT
	//}} ����� : [2009/10/13] //	��� ���� UI ��½� Ű �Է� �ȸ������� ����

#ifdef FREE_WARP
	if( g_pData != NULL && g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIInventory() != NULL && 
		g_pData->GetUIManager()->GetUIInventory()->GetShowWarpDest() == true )
	{
		return false;
	}
#endif

#ifdef SERV_ADD_WARP_BUTTON
	CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
	if( pStateMenu != NULL && pStateMenu->GetShowWarpDest() == true )
	{
		return false;
	}
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	if( g_pData != NULL && 
		g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIInventory() != NULL && 
		g_pData->GetUIManager()->GetUIInventory()->GetShowSecondJobDest() == true )
	{
		return false;
	}
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

	if( GetJoinNpc() == true || 
		m_bShowMarketList == true || 
		m_bEnableKeyProcess == false )
		return false;

#ifdef SERV_EPIC_QUEST
	if(g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIQuestNew() != NULL&&
		g_pData->GetUIManager()->GetUIQuestNew()->GetShowClearDLG() == true)
	{
		return false;
	}
#endif SERV_EPIC_QUEST
	
#ifdef SERV_SECOND_SECURITY
	if(g_pMain->GetSecurityPad()->GetShow() == true)
	{
		return false;
	}
#endif SERV_SECOND_SECURITY

	return true;
}

void CX2TFieldGame::JoinFieldUnit( KFieldUserInfo* pKFieldUserInfo )
{
    ASSERT( pKFieldUserInfo != NULL );
    if ( pKFieldUserInfo == NULL )
        return;

	CX2SquareUnit* pCX2FieldUnit = NULL;

//#if 0 // �ߺ�üũ�� �������� ���ͳ� ĳ���ߺ��� ������ ó���Ѵٴ� �����Ͽ� ó������ �ʴ´�.
#if 1 // �ߺ�üũ�� �������� ���ͳ� ĳ���ߺ��� ������ ��Ŷ�� �������ϳ� �ߺ��ǹǷ� 
	  // Ŭ���̾�Ʈ���� �ߺ�üũ�Ͽ� �ߺ����� �������� �ʵ��� �Ѵ�.
	CX2Unit* pCX2ExistingUnit = GetSquareUnitUnitByUID( pKFieldUserInfo->m_iUnitUID );
	if( pCX2ExistingUnit != NULL )
		return;
#endif

	if( g_pData->GetMyUser()->GetSelectUnit() != NULL
		&& pKFieldUserInfo->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
		if(m_bAddMyUnit == false)
		{		

// #ifdef EQUIP_BACKGROUND_LOADING_TEST 
// 			g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );
			pCX2FieldUnit = AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
// 			g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );
// #else 
// 			pCX2FieldUnit = AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
// #endif EQUIP_BACKGROUND_LOADING_TEST			
			if ( pCX2FieldUnit != NULL )
				pCX2FieldUnit->SetPartyUid(pKFieldUserInfo->m_iPartyUID);
			m_bAddMyUnit = true;			
		}
	}
	else
	{			
		CX2Unit* pCX2Unit = new CX2Unit( *pKFieldUserInfo );
        ASSERT( pCX2Unit != NULL );
		//���� ���� ������Ʈ
		pCX2Unit->ResetEqip();


//#ifdef EQUIP_BACKGROUND_LOADING_TEST 
		//g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );
		//pCX2FieldUnit = AddUnit( pCX2Unit, false, true, true );
		//g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );



		AddUnitToDeviceList( pCX2Unit
            , D3DXVECTOR3( pKFieldUserInfo->m_vPos.x, pKFieldUserInfo->m_vPos.y, pKFieldUserInfo->m_vPos.z )
            , pKFieldUserInfo->m_iPartyUID 
#ifdef ADDED_RELATIONSHIP_SYSTEM		
			, pKFieldUserInfo->m_iLoverUnitUID			
#endif // ADDED_RELATIONSHIP_SYSTEM
		);
//#else 
//		pCX2FieldUnit = AddUnit( pCX2Unit, false, true );
//#endif EQUIP_BACKGROUND_LOADING_TEST


		D3DXVECTOR3 vPos = D3DXVECTOR3(0,0,0);
		if ( pCX2FieldUnit != NULL )
			vPos = D3DXVECTOR3(pKFieldUserInfo->m_vPos.x, pKFieldUserInfo->m_vPos.y, pKFieldUserInfo->m_vPos.z);

		if ( pCX2FieldUnit != NULL )
		{

			pCX2FieldUnit->SetPos( vPos );
			pCX2FieldUnit->SetSyncOUStartPos(vPos);
			pCX2FieldUnit->SetFirstGetPacket(true);
			pCX2FieldUnit->SetFirstSync(true);		

			pCX2FieldUnit->SetPartyUid(pKFieldUserInfo->m_iPartyUID);

		}//if

		//m_bForceSync = true;

#ifdef RIDING_SYSTEM
		SetOrClearRidingPetInfo( pCX2Unit, *pKFieldUserInfo );
#endif // RIDING_SYSTEM

	}	
}

#ifdef RIDING_SYSTEM
void CX2TFieldGame::SetOrClearRidingPetInfo( CX2Unit* pUnit_, const KFieldUserInfo& kFieldUserInfo_ )
{
	/// ���� ���̵� ���� Ÿ�� ������
	if ( NULL != pUnit_ )
	{
		if ( kFieldUserInfo_.m_iRidingPetUID > 0 )
		{
			KRidingPetInfo kRidingPetInfo;
			kRidingPetInfo.m_iRidingPetUID = kFieldUserInfo_.m_iRidingPetUID;
			kRidingPetInfo.m_usRindingPetID = kFieldUserInfo_.m_usRidingPetID;
			pUnit_->SetFullRidingPetInfo( kRidingPetInfo );			
		}
		else
			pUnit_->ClearRidingPetInfo();
	}
}
#endif // RIDING_SYSTEM

bool    CX2TFieldGame::AddUnitToDeviceList( CX2Unit* pUnit
                                           , const D3DXVECTOR3& vPos, UidType uidParty
#ifdef ADDED_RELATIONSHIP_SYSTEM										   
										   , UidType uidRelation
#endif // 					ADDED_RELATIONSHIP_SYSTEM					   
										   )
{
    m_listAddUserInUnitLoader.resize( m_listAddUserInUnitLoader.size() + 1 );
    AddUserInUnitLoader& addUser = m_listAddUserInUnitLoader.back();
    addUser.m_pUnit = pUnit;
    addUser.m_vPos = vPos;
    addUser.m_iPartyUID = uidParty;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	addUser.m_iRelationshipPartnerUID = uidRelation;
#endif // ADDED_RELATIONSHIP_SYSTEM

	m_UnitLoader.AddUnitLoadDataForSquareOrField( pUnit, true );
    return true;
}



CX2SquareUnit* CX2TFieldGame::AddUnit( CX2Unit* pUnit, bool bMyUnit, bool bInit )
{
    ASSERT( pUnit != NULL );
    if ( pUnit == NULL )
        return NULL;

#ifdef RIDING_SYSTEM
	CX2SquareUnitPtr pCX2FieldUnit = CX2SquareUnit::CreateSquareUnit( bMyUnit, pUnit,
		CX2SquareUnit::GetSquareUnitScriptFileName( pUnit->GetClass() ), true );
#else //RIDING_SYSTEM
	CX2SquareUnitPtr pCX2FieldUnit = CX2SquareUnit::CreateSquareUnit( bMyUnit, pUnit, CX2SquareUnit::GetSquareUnitScriptFileName( pUnit->GetClass() ).c_str(), true );
#endif //RIDING_SYSTEM
    ASSERT( pCX2FieldUnit != NULL );
    if ( pCX2FieldUnit == NULL )
        return NULL;



	if( bInit == true )
		pCX2FieldUnit->Init();

//#ifdef TITLE_SYSTEM
    if( bInit == false )
        pCX2FieldUnit->UpdateEquippedEmblem();
//#endif



	pCX2FieldUnit->StateChange( pCX2FieldUnit->GetStateID().m_Wait );


	m_UserUnitList.push_back( pCX2FieldUnit );
	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2FieldUnit.get() );

	if( bMyUnit == true )
	{
		m_pMyUnit = pCX2FieldUnit;//.get();

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-23
        //{{ seojt // 2008-10-22, 16:48
        /** �� unit�� ��ġ�� �����ǰ� ����, �� unit�� ����Ű��
            view matrix�� �����ϰ�, viewing frustum�� �����Ѵ�.
            �� unit �Ӹ� �ƴ϶�, �ٸ� ��� ���� ��ü�� ������ ������ ���Ŀ�
            urgent data�� ���� culling�� �����ؾ� ���� ��Ȯ�ϰ� �ϱ� ���Ͽ�
            �� �Լ��� ���ο��� urgent data�� �ε����� �ʴ´�.
            - jintaeks on 2008-10-22, 16:49 */

        m_pMyUnit->OnFrameMove( g_NowTime, 0.f );
        // ī�޶� ��Ʈ������ �����Ѵ�. m_pMyUnit�� �Կ��ϴ� ī�޶� frustum�� ����
		// culling�ϱ� ���� ���̴�.
		if ( m_pCamera != NULL )
            m_pCamera->TFieldCameraUpdate( m_pMyUnit.get(), GetVillageCameraDistance(), GetVillageCameraLookAtHeight(), GetVillageCameraEyeHeight(), 0.0f, 0.0f, 0.0f );		

        /** @NOTE   �̰����� �Ʒ�ó�� LoadUrgentData()�� ȣ���ؼ��� �ȵȴ�.
                    ���� ��� ���� ������Ʈ�� Parsing�� ���� ���� Ȯ���� ���� �ʾҴ�.
                    LoadUrgentData()�� CX2StateSquareGame�� �����ڿ��� ȣ���Ѵ�.

                    g_pKTDXApp->GetDeviceManager()->LoadUrgentData( fTimeStamp );

                    - jintaeks on 2008-10-23, 15:57 */

        //}} seojt // 2008-10-22, 16:48
//#endif // BACKGROUND_LOADING_TEST // 2008-10-23

#ifndef REFORM_NOVICE_GUIDE
		// �ʺ��� ���̵�
		// �켱�� �ʺ��� ���̵带 ������ �����ϰ� �Ѵ�.
		// �ʺ��� ���̵� ����
		SAFE_DELETE(m_pNoviceGuide);
		m_pNoviceGuide = new CX2NoviceGuide();
		ASSERT( m_pNoviceGuide != NULL );
#endif //REFORM_NOVICE_GUIDE

		CX2GageManager::GetInstance()->RestoreGageData();
		CX2GageManager::GetInstance()->InitMyGageWhenGameIsOver();
		CX2GageManager::GetInstance()->ClearTempBuffFactor();

	}//if	

	//PopPartyLeaderTalkBox(pCX2FieldUnit, 0xffff0000);

		
#ifdef ADDED_RELATIONSHIP_SYSTEM	
		if ( NULL != g_pData->GetRelationshipManager() &&
			 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
			 NULL != g_pData->GetRelationshipEffectManager()  )
		{
			CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();

			if ( TRUE == bMyUnit )
			{
				if ( pRelationshipManager->GetMyRelationshipInfo()->m_eRelationStateType > SEnum::RT_SOLO )
				{
					CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_;
					RelationEffectInfo_ = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 1 ) ) );				
					RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pCX2FieldUnit->GetUID() );						
				}
			}

			if ( pCX2FieldUnit->GetUID() == pRelationshipManager->GetMyRelationshipInfo()->m_iRelationTargetUserUid )
			{
				pRelationshipManager->SetRelationPartnerInVillage( pCX2FieldUnit );
			}
		}
	
#endif // ADDED_RELATIONSHIP_SYSTEM

	return pCX2FieldUnit.get();
}

void CX2TFieldGame::RemoveUnit( UidType unitUID )
{
#ifdef ADDED_RELATIONSHIP_SYSTEM

	if ( NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();

		if ( unitUID == pRelationshipManager->GetMyRelationshipInfo()->m_iRelationTargetUserUid )
		{
			pRelationshipManager->ResetRelationPartnerInVillage();
		}
	}

	g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID ( unitUID );

#endif // ADDED_RELATIONSHIP_SYSTEM

	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2FieldUnit = m_UserUnitList[i];
		if( pCX2FieldUnit != NULL && pCX2FieldUnit->GetUID() == unitUID )
		{
            //CX2SquareUnit::DeleteKTDGObject( pCX2FieldUnit );
			m_UserUnitList.erase( m_UserUnitList.begin() + i );	
			break;
		}
	}

    for( AddUserInUnitLoaderList::iterator iter = m_listAddUserInUnitLoader.begin()
        ; iter != m_listAddUserInUnitLoader.end(); iter++ )
    {
        AddUserInUnitLoader& addUser = *iter;
        if ( addUser.m_pUnit != NULL
            && addUser.m_pUnit->GetUID() == unitUID )
        {
            m_listAddUserInUnitLoader.erase( iter );
            break;
        }//if
    }//for

    m_UnitLoader.DeleteUnitLoadData( unitUID );
}

void CX2TFieldGame::ShowJoinMarket(bool val, D3DXVECTOR3 vPos)
{		
	KTDXPROFILE();

	bool bShow = val;

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		bShow = false;
#endif

	vPos.y += 270.f;
	D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( vPos );

	CKTDGParticleSystem::CParticleEventSequence* pSeq	= GetUiParticle()->GetInstanceSequence( m_hMarketInArea );		

	if( pSeq != NULL )
	{
		if(m_fMarketTime <= 0.5f)
		{
			pSeq->ChangeTexForce(L"Get_In_Up.dds");
		}
		else if(m_fMarketTime <= 1.f)
		{
			pSeq->ChangeTexForce(L"Get_In_Down.dds");
		}
		else
		{
			m_fMarketTime = 0.f;
			pSeq->ChangeTexForce(L"Get_In_Up.dds");
		}

#ifdef SIGN_OVERRAP_BUG_FIX
		if( m_bSignOverRapCheck )
		{
			float adjustY = -45; // ���� �ػ� 1024.f �� �� ������

			if( m_fDisSignToSign < 0 )
				adjustY *= -1;


			float resRate = 0.f;
			if (true == g_pMain->GetGameOption().GetOptionList().m_bFullScreen)
			{
				int	iScreenY = GetSystemMetrics( SM_CYSCREEN );

				if( iScreenY <= 0)
					resRate = 0;
				else
					resRate = 1024.f / (float)iScreenY;
			}
			else
			{
				D3DXVECTOR2 vOriginalResolution = g_pMain->GetGameOption().GetOptionList().m_vResolution;
				if( vOriginalResolution.y <= 0)
					resRate = 0;
				else
					resRate = 1024.f / vOriginalResolution.y;
			}

			// ����ó��
			if(resRate <= 0) 
				adjustY = 0.f;
			else
				adjustY *= resRate;

			finalPos.y += adjustY;
			m_bSignOverRapCheck = false;
		}
#endif SIGN_OVERRAP_BUG_FIX

		pSeq->SetPosition( D3DXVECTOR3( finalPos.x, finalPos.y, 0 ) );						
		pSeq->SetShowObject(bShow);	
	}
			
	
	m_bJoinMarket = bShow;
}

HRESULT	CX2TFieldGame::OnFrameMove( double fTime, float fElapsedTime )
{		
	KTDXPROFILE();	

    m_UnitLoader.OnFrameMove( fElapsedTime );

    if ( m_listAddUserInUnitLoader.empty() == false )
    {
        AddUserInUnitLoader& addUser = m_listAddUserInUnitLoader.front();
        if ( addUser.m_pUnit == NULL )
        {
            m_listAddUserInUnitLoader.pop_front();
        }
        else if ( 
#ifdef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
			g_pKTDXApp->IsFirstFrameOfSimulationLoop() == true &&
#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP			
			
			m_UnitLoader.IsUnitLoadComplete( addUser.m_pUnit->GetUID() ) == true )
        {
            UidType unitUID = addUser.m_pUnit->GetUID();
            CX2SquareUnit* pCX2SquareUnit = AddUnit( addUser.m_pUnit, false, true );
            if ( pCX2SquareUnit != NULL )
            {
                addUser.m_pUnit = NULL;
		        pCX2SquareUnit->SetPos( addUser.m_vPos );
		        pCX2SquareUnit->SetSyncOUStartPos( addUser.m_vPos );
		        pCX2SquareUnit->SetFirstGetPacket(true);
		        pCX2SquareUnit->SetFirstSync(true);		
		        pCX2SquareUnit->SetPartyUid( addUser.m_iPartyUID );
				

#ifdef ADDED_RELATIONSHIP_SYSTEM
				if ( addUser.m_iRelationshipPartnerUID > 0 )
				{
					if ( NULL != g_pData->GetRelationshipEffectManager() )
					{
						CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_;
						RelationEffectInfo_ = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 1 ) ) );				
						RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pCX2SquareUnit->GetUID() );						
						pCX2SquareUnit->GetUnit()->SetLoverUnitUID( addUser.m_iRelationshipPartnerUID );
					}
				}
#endif // ADDED_RELATIONSHIP_SYSTEM


            }
            else
            {
                SAFE_DELETE( addUser.m_pUnit );
            }//if.. else..
            m_listAddUserInUnitLoader.pop_front();
            m_UnitLoader.DeleteUnitLoadData( unitUID );
        }//if.. else..
    }//if


	//ConnectTR();	

#ifdef NEW_VILLAGE_AUTO_SD_UNIT	
	m_fAutoSDUnitTime += fElapsedTime;
	// �ʵ� ������ 5�ʵڿ� �������� 10�����ϰ�� ���� SDĳ���� ���� ����
	if( m_fAutoSDUnitTime >= 5.f && m_bLoadComplete == true && g_pKTDXApp->GetFrameMoveFPS() <= 10.f )
	{
		g_pMain->GetGameOption().SetFieldSD(true);
		//g_pData->SetShowSDUnit(true);
		m_fAutoSDUnitTime = 5.f;
	}	
#endif

	//{{ kimhc // 2010.3.17 // ���� ���⵵ 
#ifdef	IGA_TEST
    if ( m_pCamera != NULL )
		CX2IGA::GetInstance()->ProcessTracking( m_pCamera, fElapsedTime );
#endif	IGA_TEST
	//}} kimhc // 2010.3.17 // ���� ���⵵

// 	D3DXMATRIX mt( g_pKTDXApp->GetViewProjectionTransform() );
// 	D3DXVECTOR3	vResult;
 	D3DXVECTOR3 v3( 15511.713f, 1425.825f, 197.458f );
// 
// 	D3DXVec3TransformCoord( &vResult, &v3, &mt );
// 
 	bool bIntersect;
 
 	if ( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( v3, 0.0f ) == false )
 		bIntersect = false;
 	else
 		bIntersect = true;
		

	// ���� �ε�ȭ�� (���� �̵��ÿ��� ����Ѵ�)
	m_fLoadUiTime += fElapsedTime;
	if(m_fLoadUiTime >= 3.f)
	{
		// �ε�ȭ�� ����
		if(m_pDLGLoadingState != NULL)
		{
			//m_pDLGLoadingState->SetShowEnable(false, false);
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
            SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
			SAFE_DELETE( m_pDLGLoadingState );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
			SAFE_DELETE( m_pCX2GameLoadingTip );
		}	

		m_fLoadUiTime = 20.f;		
	}

	if( m_pDLGLoadingState != NULL )
	{
		m_pDLGLoadingState->OnFrameMove( fTime, fElapsedTime );

		if ( m_pCX2GameLoadingTip != NULL )
			m_pCX2GameLoadingTip->OnFrameMove( fTime, fElapsedTime );		
	}


	// �ʵ��̸�ǥ��
	if ( m_pDLGFieldName != NULL )
	{
		m_fFieldNameTime += fElapsedTime;
		if(m_fFieldNameTime >= 4.f && m_fFieldNameTime <= 6.f)
		{
			CloseFieldName(m_fFieldNameTime);
		}
		else if(m_fFieldNameTime > 6.f)
		{
			SAFE_DELETE_DIALOG( m_pDLGFieldName );
			m_fFieldNameTime = 0.f;
		}
	}

#ifdef COME_BACK_REWARD
	if ( m_pDLGBuffName != NULL )
	{
		m_fBuffNameTime += fElapsedTime;
		if( m_fBuffNameTime >= 7.f && m_fBuffNameTime < 8.f )
		{
			if ( NULL != m_pDLGBuffName )
				m_pDLGBuffName->SetShowEnable( true, true );
		}
		else if(m_fBuffNameTime >= 8.f && m_fBuffNameTime <= 9.f)
		{
			CloseBuffName( m_fBuffNameTime );
		}
		else if(m_fBuffNameTime > 9.f)
		{
			SAFE_DELETE_DIALOG( m_pDLGBuffName );
			m_fBuffNameTime = 0.f;
		}
	}
#endif
	
	

	m_bJoinNpc = GetJoinNpc();	

	// ������ ó��
#ifdef OTHER_ROAD	
	m_fOtherLineTime += fElapsedTime;

#ifdef SIGN_OVERRAP_BUG_FIX
		D3DXVECTOR3 otherLinePos = D3DXVECTOR3(0.f, 0.f, 0.f);
#endif // SIGN_OVERRAP_BUG_FIX

    ASSERT( m_pMyUnit != NULL );
    if ( m_pMyUnit != NULL )
    {

		const CKTDGLineMap::LineData* pLineData;	
		pLineData = GetWorld()->GetLineMap()->GetLineData( m_pMyUnit->GetLastTouchLineIndex() );

		D3DXVECTOR3 landPos = GetWorld()->GetLineMap()->GetLandPosition_LUA( m_pMyUnit->GetPos() );
		landPos.y += 0.5f;
		bool bOnOtherLine = false;
		if(pLineData->lineType == CKTDGLineMap::LT_OTHER_ROAD)
			bOnOtherLine = true;

		int iOtherLine;
		bool bShowOtherLine = false;
		bool bShowUp = false;
		float fOtherLineDist = 0.f;
#ifndef SIGN_OVERRAP_BUG_FIX
		D3DXVECTOR3 otherLinePos = D3DXVECTOR3(0.f, 0.f, 0.f);
#endif // SIGN_OVERRAP_BUG_FIX
		m_bFindOtherLine = GetWorld()->GetLineMap()->GetNearOtherLine(landPos, otherLinePos, m_pMyUnit->GetLastTouchLineIndex(), iOtherLine, 200.f);

		const CKTDGLineMap::LineData* pOtherLineData = GetWorld()->GetLineMap()->GetLineData( iOtherLine );

		if(m_bFindOtherLine == true && pLineData->lineType == CKTDGLineMap::LT_OTHER_ROAD && pOtherLineData->lineType == CKTDGLineMap::LT_NORMAL)
		{
			if(pLineData->m_bOtherRoadUp == false)
				bShowUp = true;
			bShowOtherLine = true;
		}
		else if(m_bFindOtherLine == true && pLineData->lineType == CKTDGLineMap::LT_NORMAL && pOtherLineData->lineType == CKTDGLineMap::LT_OTHER_ROAD)
		{
			if(pOtherLineData->m_bOtherRoadUp == true)
				bShowUp = true;
			bShowOtherLine = true;
		}

		if(bShowOtherLine == true)
		{		
			CKTDGParticleSystem::CParticleEventSequence* pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hOtherLine );
			D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( otherLinePos );
            if ( pSeqPortal != NULL )
		        pSeqPortal->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - 200.f, 0.f));

			if(bShowUp == true)
			{
				// ����Ű
				if( pSeqPortal != NULL )
				{				
					if(m_fOtherLineTime <= 0.5f)
						pSeqPortal->ChangeTexForce(L"Top_Go_Up.dds");
					else if(m_fOtherLineTime <= 1.f)
						pSeqPortal->ChangeTexForce(L"Top_Go_Down.dds");
					else
					{
						m_fOtherLineTime = 0.f;
						pSeqPortal->ChangeTexForce(L"Top_Go_Up.dds");
					}

					pSeqPortal->SetShowObject(true);
				}
			}
			else
			{
				// �Ʒ��� Ű
				if( pSeqPortal != NULL )
				{				
					if(m_fOtherLineTime <= 0.5f)
						pSeqPortal->ChangeTexForce(L"Bottom_Go_Up.dds");
					else if(m_fOtherLineTime <= 1.f)
						pSeqPortal->ChangeTexForce(L"Bottom_Go_Down.dds");
					else
					{
						m_fOtherLineTime = 0.f;
						pSeqPortal->ChangeTexForce(L"Bottom_Go_Up.dds");
					}

					pSeqPortal->SetShowObject(true);
				}
			}
		
		
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hOtherLine );
			if( pSeqPortal != NULL )
			{				
				pSeqPortal->SetShowObject(false);
			}
		}

		fOtherLineDist = GetDistance(landPos, otherLinePos);
		if( m_bFindOtherLine == true )
		{
			// ������ ����
			if( (bShowUp == true && GET_KEY_STATE( GA_UP ) == TRUE || (bShowUp == false && GET_KEY_STATE( GA_DOWN ) == TRUE ) ) )
			{
#ifdef RIDING_SYSTEM
				if ( true == m_pMyUnit->GetRidingOn() )
				{
					m_pMyUnit->StateChange( "RIDING_WAIT" );
				}
				else
				{
					m_pMyUnit->StateChange( "WAIT" );
				}
#else //RIDING_SYSTEM
				m_pMyUnit->StateChange(1);
#endif //RIDING_SYSTEM
				m_pMyUnit->SetPos( otherLinePos );
			}		
		}
	}
#endif

	// ����
	//if(g_pMain->GetGameOption().GetOptionList().m_MapDetail == CX2GameOption::OL_HIGH)
	{
		if(m_pSubAniXSkinAnim != NULL)
		{
			__time64_t t0 = g_pData->GetServerCurrentTime();
#ifdef	CONVERSION_VS
            struct tm   ktm;
            struct tm* ptm = &ktm;
            bool bOK = _localtime64_s( ptm, &t0 ) == 0;
#else   CONVERSION_VS
			struct tm* ptm = _localtime64( &t0 );
			bool bOK = ( ptm != NULL );
#endif  CONVERSION_VS
			if ( bOK == true )
			{
				float fNowSecond = (float)ptm->tm_hour * 3600.f + (float)ptm->tm_min * 60.f + (float) ptm->tm_sec;

				// 1�п� �ѹ��� SubAnim ����
				if((int)fNowSecond % m_iSubAniTime == 1)			
				{
					m_pSubAniXSkinAnim->ChangeAnim( L"Normal", false );
					m_pSubAniXSkinAnim->SetPlaySpeed(0.6f);
					m_pSubAniXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE );
					m_pSubAniXSkinAnim->SetShowObject(true);

					// ���� ȿ���� ���
					//if(g_pMain->GetGameOption().GetOptionList().m_bSound == true)
					//{
					//	if(m_pSubSound != NULL)
					//		m_pSubSound->Play(false, true);
					//}				
				}

				m_pSubAniXSkinAnim->OnFrameMove(fTime, fElapsedTime);
				if(m_pSubAniXSkinAnim->IsAnimationEnd() == true)
				{
					m_pSubAniXSkinAnim->SetShowObject(false);		
				}
			}
		}		
	}
	//else
	//{
	//	if(m_pSubAniXSkinAnim != NULL)
	//		m_pSubAniXSkinAnim->SetShowObject(false);
	//}

#ifdef RIDING_SYSTEM
	CX2RidingPetManager* pRidingPetManager = CX2RidingPetManager::GetInstance();
	if ( NULL != pRidingPetManager )
		pRidingPetManager->OnFrameMove( fTime, fElapsedTime );
#endif //RIDING_SYSTEM
	
	// �����Ա� ó��
	m_fMarketTime += fElapsedTime;
	if( IsNearPortalToBattleField() == false )
	{
		float fDistance = 0.f;
		int iLinkPos = 0;
		D3DXVECTOR3 vLinkPos;

		ShowJoinMarket(false, D3DXVECTOR3(0.f, 0.f, 0.f));
		m_iJoinMarketIndex = -1;

		if(m_bShowMarketList == false)
		{
			for(unsigned int iLinkedPos=0; iLinkedPos<m_vecMarketPos.size(); ++iLinkedPos)
			{
				iLinkPos = m_vecMarketPos[iLinkedPos];
				vLinkPos = g_pData->GetLocationManager()->GetStartPosLoc(iLinkPos);

#ifdef SIGN_OVERRAP_BUG_FIX
				m_fDisSignToSign = vLinkPos.z - otherLinePos.z;
				
				if ( (-2.0f <= m_fDisSignToSign) && (m_fDisSignToSign <= 2.0f ))
				{
						m_bSignOverRapCheck = true;
				}
#endif SIGN_OVERRAP_BUG_FIX

				fDistance = GetDistance(m_pMyUnit->GetPos(), vLinkPos);
				if(fDistance <= 150.f)
				{						
					ShowJoinMarket(true, vLinkPos);
					m_iJoinMarketIndex = iLinkedPos;					
				}
			}
		}		
	}
	

	// ��Żó��	
	bool bPotalCamera = false;

	SetNearPortalToBattleField( false );

	if( IsNearPortalToBattleField() == false )
	{
		float fDistance = 0.f;
		int iVillageStartPosIdToBattleField = 0;
		D3DXVECTOR3 vLinkPos;

//#ifdef MODIFY_PORTAL_GATE
		UINT m_uiPortalIndex = 0;
//#endif //MODIFY_PORTAL_GATE
		for(unsigned int iLinkedPos=0; iLinkedPos<m_vecLinkedPos.size(); ++iLinkedPos)
		{
			iVillageStartPosIdToBattleField = m_vecLinkedPos[iLinkedPos];
			vLinkPos = g_pData->GetLocationManager()->GetStartPosLoc(iVillageStartPosIdToBattleField);

			fDistance = GetDistance(m_pMyUnit->GetPos(), vLinkPos);			


			if(fDistance <= 2000.0f)
			{
//#ifdef MODIFY_PORTAL_GATE
				if(m_bJoinNpc == false)
					SetShowPortalGate(true, m_uiPortalIndex++, vLinkPos);
				else
					SetShowPortalGate(false, m_uiPortalIndex++, vLinkPos);
//#else
//				if(m_bJoinNpc == false)
//					SetShowPortalGate(true, vLinkPos);
//				else
//					SetShowPortalGate(false, vLinkPos);
//#endif //MODIFY_PORTAL_GATE
			}


			if(m_bJoinNpc == false)
			{
				float fCDistance = GetVillageCameraDistance();

				if(fDistance <= 3000.0f)
				{
					float fAng = (5000.f / fDistance) + 10;
					bPotalCamera = true;							

					if(fAng >= 30.f)
						fAng = 30.f;

					if(fDistance <= 1000.f)
					{
						fCDistance = 1000.f;
					}
					
                    if ( m_pCamera != NULL )
					    m_pCamera->FieldCamera( m_pMyUnit.get(), fCDistance, GetVillageCameraLookAtHeight(), GetVillageCameraEyeHeight(), fAng);
					//m_pCamera->SquareCamera( m_pMyUnit, 1000.f, 300.f, fAng );							
				}

				if(fDistance <= 100.0f)
				{
					const UINT uiBattleFieldIdToMove = g_pData->GetLocationManager()->GetBattleFieldId( iVillageStartPosIdToBattleField );
					
					if ( SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= uiBattleFieldIdToMove &&
						SEnum::VMI_BATTLE_FIELD_END > uiBattleFieldIdToMove )
					{						
						SetVillageStartPosIdToBattleField( iVillageStartPosIdToBattleField );
						SetNearPortalToBattleField( true );
						break;
					}
				}
			}			
		}
	}

	// 2�ʸ��� ĳ���� ��ũ�� �����.
	m_fStateSyncTime += fElapsedTime;
	if(m_fStateSyncTime >= 2.0f)
	{
		SyncUnitFrame();
		m_fStateSyncTime = 0.f;
	}	

	// 10�ʸ��� �ŷ����� ����Ʈ�� �����Ѵ�.
	m_fMarketRefresh += fElapsedTime;
	if(m_fMarketRefresh >= 10.f && m_bShowMarketList == true)
	{
        ASSERT( m_pSquareUI != NULL );
        if ( m_pSquareUI != NULL )
		    Handler_EGS_SQUARE_LIST_REQ( m_pSquareUI->m_iNowPage );
		m_fMarketRefresh = 0.f;
	}	

	//m_pUiParticle->OnFrameMove( fTime, fElapsedTime );
	//m_pUiXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	//m_pMajorParticle->OnFrameMove( fTime, fElapsedTime );
	//m_pMinorParticle->OnFrameMove( fTime, fElapsedTime );
	//m_pMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
	//m_pMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );	

	if( GetEnableKey() == true)
	{
		if( false == m_bFreeCamera )
		{
			KeyProcess();
		}
	}
	else
	{
		InitKey();
	}
	
    if ( m_pWorld != NULL )
		m_pWorld->OnFrameMove( fTime, fElapsedTime );	

	int count = 0;
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];		
        if ( pCX2SquareUnit == NULL )
            continue;

		if( m_bJoinNpc == true || m_bUserHide == true 
#ifdef EVENT_SCENE
			|| g_pData->GetEventScene()->GetIsPlayingEventScene() == true
#endif EVENT_SCENE
			) // && m_pMyUnit != pCX2SquareUnit)
		{
			// npc join�ÿ� �ٸ� �������� ������ �ʵ��� �Ѵ�. 
			pCX2SquareUnit->SetShowObject( false );
            if ( m_pTalkBoxManager != NULL )
			    m_pTalkBoxManager->SetShowTalk(false);
            
			continue;
		}

        if ( m_pTalkBoxManager != NULL )
		    m_pTalkBoxManager->SetShowTalk(true);
        
		if( pCX2SquareUnit->GetInit() == true )
		{
#ifdef SERV_INVISIBLE_GM
			if( m_pMyUnit != pCX2SquareUnit && IsInvisible( pCX2SquareUnit->GetUID() ) == true )
			{
				pCX2SquareUnit->SetShowObject(false);
				pCX2SquareUnit->SetPlanRender( false );
				continue;
			}
			else
#endif SERV_INVISIBLE_GM
			if( m_pMyUnit != NULL && m_pMyUnit != pCX2SquareUnit && g_pMain->GetGameOption().GetOptionList().m_bParty == true )
			{
				//{{ kimhc // 2009-10-09 // ������ ���̵��� �߰�
#ifdef	GUILD_MANAGEMENT
				if ( CanNotShowThisUnit( pCX2SquareUnit->GetUID() ) == true )
#else	GUILD_MANAGEMENT
				if( g_pData != NULL &&
					(g_pData->GetPartyManager()->IsMyPartyMember(pCX2SquareUnit->GetUID()) == false) &&
					(g_pData->GetMessenger()->IsMyFriend(pCX2SquareUnit->GetUID()) == false) )
#endif	GUILD_MANAGEMENT
					// }}kimhc // 2009-10-09 // ������ ���̵��� �߰�

				{
					
					pCX2SquareUnit->SetShowObject(false);
					pCX2SquareUnit->SetPlanRender( false );
					continue;
				}
			}

			if( GetDistance( m_pMyUnit->GetPos(), pCX2SquareUnit->GetPos() ) > 1500 )
			{
				//pCX2SquareUnit->SetShowObject( false );
				pCX2SquareUnit->SetPlanRender( true );
				pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );
			}
			else
			{
				if( count > m_MaxCount )
				{
					//pCX2SquareUnit->SetShowObject( false );
					pCX2SquareUnit->SetPlanRender( true );
					pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );
				}
				else
				{					
					pCX2SquareUnit->SetShowObject( true );
					if(g_pMain->GetGameOption().GetFieldSD())
					{
						// SD ���
						pCX2SquareUnit->SetPlanRender( true );
					}
					else
					{
						// �Ϲ� ���						
						pCX2SquareUnit->SetPlanRender( false );
					}				
				
					pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );

					count++;
				}			
			}	
		}		
	}	
	
	// ���� npc
	if(m_bShowMarketList == false)
	{
		bool bFindJoinNpc = false;
		for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
		{
			CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
			if( pNpcNode != NULL )
			{
				pNpcNode->SetInArea(false);		
                float fNpcDistance = 0.f;
				if( m_pMyUnit != NULL && ( fNpcDistance = GetDistance( m_pMyUnit->GetPos(), pNpcNode->m_vPos ) ) <= 2500 )
				{
					pNpcNode->SetShowObject(true);
					pNpcNode->GetQuestInfo();

#ifdef DIALOG_SHOW_TOGGLE
					if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#endif
					{
						if(fNpcDistance <= pNpcNode->m_NPCJoinArea)
						{
							pNpcNode->SetInArea(true);								
						}
						else
						{
							if(pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
							{
								pNpcNode->SetInArea(false);
								//pNpcNode->SetNpcBye(true);
							}				
						}	
					}
#ifdef DIALOG_SHOW_TOGGLE
					else
					{
						pNpcNode->SetInArea(false);
					}
#endif
								
				}
				else
				{
					pNpcNode->SetShowObject(false);				
				}

				if(pNpcNode->GetInArea() == false)
				{
					pNpcNode->ResetNpc();
				}
				else
					bFindJoinNpc = true;

				pNpcNode->OnFrameMove( fTime, fElapsedTime );

#if defined(ADD_MUSIC_SUB_VOLUME) && defined(APINK_NPC)
				if( g_pMain->GetGameOption().GetMusic() == true && pNpcNode->GetNpcId() == CX2UnitManager::NUI_EVENT_CRAYONPOP )
				{	
					float fSubVolume = 3000.f - fNpcDistance;
					if( fSubVolume < 0.f )
						fSubVolume = 0.f;
					else if( fSubVolume > 3000.f )
						fSubVolume = 3000.f;

					g_pMain->GetGameOption().SetMusicSubVolume( -fSubVolume );

					if( m_pAPinkSound != NULL )
					{
						float fAPinkMusicVolume = g_pMain->GetGameOption().GetMusicVoluem();
						float fAPinkSubVolume = g_pMain->GetGameOption().GetMusicSubVolume();

						if( fAPinkMusicVolume > 0.f )
							fAPinkMusicVolume = 0.f;
						else if( fAPinkMusicVolume < -3000.f )
							fAPinkMusicVolume = -3000.f;
						fAPinkMusicVolume += 3000.f; //abs(fAPinkSoundVolume);
						fAPinkMusicVolume /= 3000.f;

						if( fAPinkSubVolume > 0.f )
							fAPinkSubVolume = 0.f;
						else if( fAPinkSubVolume < -3000.f )
							fAPinkSubVolume = -3000.f;
						fAPinkSubVolume = abs(fAPinkSubVolume);
						fAPinkSubVolume /= 3000.f;
						fAPinkSubVolume *= 0.8f;
						if( fAPinkSubVolume <= 0.2f )
							fAPinkSubVolume = 0.f;

						m_pAPinkSound->SetVolume( fAPinkMusicVolume * fAPinkSubVolume );
					}
				}
#endif
			}			
		}	

		if(bFindJoinNpc == false)
		{
            if ( m_pNpcShadow != NULL )
			    m_pNpcShadow->CloseShadow();

			g_pChatBox->SetChatBoxLayerUp(false);
			g_pChatBox->CloseChatWindowButton();

            
			if ( m_pTalkBoxManager != NULL )
			    m_pTalkBoxManager->SetShowTalk(true);
		}
	}

	//{{ kimhc // �ǽð� ���ҵ� �� ���������� �������� ����Ʈ ȿ��
#ifdef	REAL_TIME_ELSWORD
	if ( GetTalkNpc() == false )
	{
		if ( g_pData->GetMyUser()->GetSelectUnit()->GetIsLevelUp() == true )
		{
			DisplayLevelUpEffect( m_pMyUnit->GetUID() );

			g_pData->GetMyUser()->GetSelectUnit()->SetIsLevelUp( false );
		}
		
		if ( g_pData->GetPartyManager()->DoIHaveParty() == true && g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
		{
			int iMemberCnt = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount();

			for ( int i = 0; i < iMemberCnt; i++ )
			{
				CX2PartyManager::PartyMemberData* pPartyMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( i );
				
				if ( pPartyMemberData != NULL )
					DisplayLevelUpEffect( pPartyMemberData->m_iUnitUID );
			}
		}
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� ���������� �������� ����Ʈ ȿ��

	//{{ kimhc // 2011-04-25 // ���潺 ����, Time UI
#ifdef SERV_INSERT_GLOBAL_SERVER
	//	����̼� ���� â �ѱ�
	if( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
	{
		if( g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() == true )
		{
			bool bShow = !GetTalkNpc();
			g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( bShow );
		}
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} kimhc // 2011-04-25 // ���潺 ����, Time UI
	
#ifdef SERV_GLOBAL_MISSION_MANAGER
	if( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
	{
		if( g_pData->GetGlobalMissionManager()->IsActive() == true )
		{
			bool bShow = !GetTalkNpc();
			
			if( bShow == true && g_pData->GetGlobalMissionManager()->CheckShowGlobalMissionUI() == true )
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

	if( NULL != m_pTalkBoxManager && m_bJoinNpc == false)
	{
		m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
	}

	m_pNpcShadow->OnFrameMove(fTime, fElapsedTime);

	if( m_bEnableCameraProcess == true )
	{
		if( false == m_bFreeCamera && NULL != m_pWorld->GetLineMap())
		{
#ifdef NEW_VILLAGE_RENDERING_TEST		
			const CKTDGLineMap::CameraData* pCameraData = m_pWorld->GetLineMap()->GetCameraData( m_pMyUnit->GetLastTouchLineIndex() );
#ifdef VILLAGE_SANDER
			if( NULL != pCameraData && false == GetJoinNpc() )	/// NPC��ȭ�� ���� ó��
#else  VILLAGE_SANDER
			if( NULL != pCameraData )
#endif VILLAGE_SANDER
			{
	#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
				switch( pCameraData->m_eType )
				{
				case CKTDGLineMap::LCT_RELATIE_POSITION:
					{
						if ( m_pCamera != NULL )
						{
							m_pCamera->GetLineScriptedCameraData().m_vFinalRelativeEyePosition = pCameraData->m_vRelativeEye;
							m_pCamera->GetLineScriptedCameraData().m_fSpeed = pCameraData->m_fCameraRepositionSpeed;
							m_pCamera->NomalTrackingCamera( GetMyUnit(), g_pMain->GetGameOption().GetCameraDistance(), 200, 0.f, 0.f , 0.f, 0.3f );					
						}
					}
					break;
				default:					
					if ( m_pCamera != NULL )
						m_pCamera->SquareLineCamera( m_pMyUnit.get(), pCameraData->m_vEye, pCameraData->m_vLookAt, pCameraData->m_bFocusUnit, 0.4f );			
					break;
				}
	#else
				if ( m_pCamera != NULL )
					m_pCamera->SquareLineCamera( m_pMyUnit.get(), pCameraData->m_vEye, pCameraData->m_vLookAt, pCameraData->m_bFocusUnit, 0.4f );			
	#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
			}
			else
			{
				if(bPotalCamera == false && m_bJoinNpc == false)
				{


#ifdef HEAD_ATTACHED_CAMERA_TEST

					if( NULL != GetMyUnit() &&
						true == GetMyUnit()->GetOnRollerCoaster() )
					{
						GetMyUnit()->BoneAttachedCamera( L"Bip01_Head", D3DXVECTOR3( 0, 20, -700 ) );

					}
					else 
					{
                        if ( m_pCamera != NULL )
						    m_pCamera->FieldCamera( m_pMyUnit.get(), GetVillageCameraDistance(), GetVillageCameraLookAtHeight(), GetVillageCameraEyeHeight());
					}

#else HEAD_ATTACHED_CAMERA_TEST
                    if ( m_pCamera != NULL )
					    m_pCamera->FieldCamera( m_pMyUnit.get(), GetVillageCameraDistance(), GetVillageCameraLookAtHeight(), GetVillageCameraEyeHeight());
#endif HEAD_ATTACHED_CAMERA_TEST
					


				}
				else if(bPotalCamera == false && m_bJoinNpc == true)
				{
                    if ( m_iJoinNpcIndex >= 0 && m_iJoinNpcIndex < static_cast<int>( m_vecFieldNpc.size() )
                        && m_vecFieldNpc[m_iJoinNpcIndex] != NULL )
                    {
					    D3DXVECTOR3 vPos = m_vecFieldNpc[m_iJoinNpcIndex]->m_vPos;
					    vPos.y -= 100.f;
                        if ( m_pCamera != NULL )
					        m_pCamera->SquareCamera( m_pMyUnit.get(), vPos, 500.f, 300.f);					
                    }//if
				}
			}

#else NEW_VILLAGE_RENDERING_TEST
            if ( m_pCamera != NULL )
			    m_pCamera->SquareCamera( m_pMyUnit );
#endif NEW_VILLAGE_RENDERING_TEST
			
            if ( m_pCamera != NULL )
				m_pCamera->OnFrameMove( fTime, fElapsedTime );
		}
		else
		{
			m_FPSCamera.SetEnablePositionMovement( true );

			m_FPSCamera.FrameMove( fElapsedTime * 300.f );	
            if ( m_pCamera != NULL )
            {
			    m_pCamera->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			    D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * 500.f + *m_FPSCamera.GetEyePt();
			    m_pCamera->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
			    m_pCamera->GetCamera().UpdateCamera( fElapsedTime );
            }//if
		}
	}		


#ifndef REFORM_NOVICE_GUIDE
	// �ʺ��� ���̵�
	if(m_pNoviceGuide != NULL)
	{
		m_pNoviceGuide->OnFrameMove(fTime, fElapsedTime);
	}
#endif //REFORM_NOVICE_GUIDE

#ifdef QUEST_GUIDE
	if( NULL != m_pNPCIndicator )
	{
		m_pNPCIndicator->OnFrameMove();
	}
#endif //QUEST_GUIDE



	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() )
	{
        if ( m_pCamera != NULL )
        {
		    D3DXVECTOR3 vLookVec = m_pCamera->GetCamera().GetLookVec();
		    D3DXVec3Normalize( &vLookVec, &vLookVec );
		    D3DXVECTOR3 vUpVec = m_pCamera->GetCamera().GetUpVec();
		    D3DXVec3Normalize( &vUpVec, &vUpVec );

		    g_pKTDXApp->GetDSManager()->SetListenerData( m_pCamera->GetCamera().GetEye(), vLookVec, vUpVec );
        }//if
	}


	if ( g_pInstanceData != NULL && g_pInstanceData->IsRegisteredRank() == true &&
		 g_pInstanceData->GetIsDungeonLounge() == true )
	{
		g_pInstanceData->SetRegisteredRank( false );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3833 ), g_pMain->GetNowState() );
	}
	
	return S_OK;
}

HRESULT	CX2TFieldGame::OnFrameRender()
{	

	KTDXPROFILE();
		

	if( GetShowLoadUi() == false )
	{
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		g_pKTDXApp->GetDGManager()->ObjectChainSort();
		//{{ robobeg : 2008-10-18
		
		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();

		// ���� ���ⷻ��	
		if( m_pSubAniXSkinAnim != NULL && m_pSubAniXSkinAnim->GetShowObject() == true)
		{		
			CKTDGXRenderer::RenderParam* pRenderParam = m_pSubAniXSkinAnim->GetRenderParam();			
			pRenderParam->bZEnable			= true;
			//pRenderParam->bZWriteEnable		= true;		
			pRenderParam->renderType		= CKTDGXRenderer::RT_REAL_COLOR;
			//pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
            pRenderParam->lightPos			= ( m_pWorld != NULL ) ? m_pWorld->GetLightPos() : D3DXVECTOR3(0,0,0);
			//pRenderParam->fOutLineWide		= 1.5f;
			//pRenderParam->color				= 0xffffffff;
			pRenderParam->bAlphaBlend		= true;			

			m_pSubAniXSkinAnim->OnFrameRender();
		}	

		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
		//}} robobeg : 2008-10-18	
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		
		for( UINT i = 0; i < m_UserUnitList.size(); i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
			if( pCX2SquareUnit != NULL && m_bJoinNpc == false )
			{			
				if(pCX2SquareUnit->GetShowObject() == true || pCX2SquareUnit->GetPlanRender() == true)
					pCX2SquareUnit->RenderName();
			}
		}			


#ifdef SERV_PET_SYSTEM
		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			g_pData->GetPetManager()->RenderName();
			g_pData->GetPetManager()->OnFrameRender();
		}
#endif

#ifdef RIDING_SYSTEM
		if ( NULL != CX2RidingPetManager::GetInstance() )
			CX2RidingPetManager::GetInstance()->OnFrameRender();
#endif //RIDING_SYSTEM
		
		if( NULL != m_pTalkBoxManager )
		{
			m_pTalkBoxManager->OnFrameRender();
		}
	}	
	else
	{
		// ���� �̵��� �ε�ȭ�� ���
        if ( m_pDLGLoadingState != NULL )
		    m_pDLGLoadingState->OnFrameRender();

		if ( m_pCX2GameLoadingTip != NULL )
			m_pCX2GameLoadingTip->OnFrameRender();
	}

	return S_OK;
}

bool CX2TFieldGame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_pSquareUI != NULL && m_pSquareUI->GetShow() == true )
	{
		m_pSquareUI->MsgProc(hWnd, uMsg, wParam, lParam);
	}

	if( true == m_bFreeCamera )
	{
		m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);		
	}
	
	return false;
}

HRESULT	CX2TFieldGame::OnResetDevice()
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
        if ( pCX2SquareUnit == NULL )
            continue;
		pCX2SquareUnit->OnResetDevice();
	}

	if( m_pWorld != NULL )
		m_pWorld->OnResetDevice();

	if( m_pUiParticle != NULL )
		m_pUiParticle->OnResetDevice();
	
	//if( m_pMajorParticle != NULL )
	//	m_pMajorParticle->OnResetDevice();
	//if( m_pMajorParticle != NULL )
	//	m_pMajorParticle->OnResetDevice();

	if ( m_pDLGLoadingState != NULL)
	{
		m_pDLGLoadingState->OnResetDevice();
	}

	

	return S_OK;
}

HRESULT	CX2TFieldGame::OnLostDevice()
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
        if ( pCX2SquareUnit == NULL )
            continue;
		pCX2SquareUnit->OnLostDevice();
	}

	if( m_pWorld != NULL )
		m_pWorld->OnLostDevice();

	if( m_pUiParticle != NULL )
		m_pUiParticle->OnLostDevice();
	//if( m_pMajorParticle != NULL )
	//	m_pMajorParticle->OnLostDevice();
	//if( m_pMinorParticle != NULL )
	//	m_pMinorParticle->OnLostDevice();

	if ( m_pDLGLoadingState != NULL)
		m_pDLGLoadingState->OnLostDevice();

	return S_OK;
}

void CX2TFieldGame::InitKey()
{
	m_InputData.Init();
    if ( m_pMyUnit != NULL )
	    m_pMyUnit->SetInputData( &m_InputData );
}


void CX2TFieldGame::KeyProcess()
{
	KTDXPROFILE();

	m_InputData.Init();

	if ( GET_DOUBLEKEYPURE_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.pureDoubleLeft = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.oneDoubleLeft = true;
	}
	if ( GET_PUREKEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.pureLeft = true;
	}
	if ( GET_KEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.oneLeft = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.pureDoubleRight = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.oneDoubleRight = true;
	}
	if ( GET_PUREKEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.pureRight = true;
	}
	if ( GET_KEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.oneRight = true;
	}

	if( m_bFindOtherLine == false )
	{
#ifdef FIX_ARA_SKILL_BUG
		if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE )
		{
			if ( m_pMyUnit->GetNowState() == m_pMyUnit->GetStateID().m_JumpUp || m_pMyUnit->GetNowState() == m_pMyUnit->GetStateID().m_JumpDown )
				return;

			if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
				return;

	#ifdef RIDING_SYSTEM
			if ( m_pMyUnit->GetNowState() != m_pMyUnit->GetStateID().m_Wait && m_pMyUnit->GetNowState() != m_pMyUnit->GetStateID().m_iRidingWait )
				return;

			if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITREADY )
				return;

			g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2501 ) );

	#else //RIDING_SYSTEM
			m_pMyUnit->PlayEmotion( CX2Unit::ET_SITREADY );
	#endif //RIDING_SYSTEM
		}
		else if ( m_pMyUnit->GetNowEmotionId() != CX2Unit::ET_SITREADY && GET_PUREKEY_STATE( GA_SIT ) == FALSE )
		{
			if ( GET_DOUBLEKEYPURE_STATE( GA_UP ) == TRUE )
			{
				m_InputData.pureDoubleUp = true;
			}
			if ( GET_DOUBLEKEY_STATE( GA_UP ) == TRUE )
			{
				m_InputData.oneDoubleUp = true;
			}
			if ( GET_PUREKEY_STATE( GA_UP ) == TRUE )
			{
				m_InputData.pureUp = true;
			}
			if ( GET_KEY_STATE( GA_UP ) == TRUE )
			{
				m_InputData.oneUp = true;
			}

			if ( GET_DOUBLEKEYPURE_STATE( GA_DOWN ) == TRUE )
			{
				m_InputData.pureDoubleDown = true;
			}
			if ( GET_DOUBLEKEY_STATE( GA_DOWN ) == TRUE )
			{
				m_InputData.oneDoubleDown = true;
			}
			if ( GET_PUREKEY_STATE( GA_DOWN ) == TRUE )
			{
				m_InputData.pureDown = true;
			}
			if ( GET_KEY_STATE( GA_DOWN ) == TRUE )
			{
				m_InputData.oneDown = true;
			}
		}
#else //FIX_ARA_SKILL_BUG
		if ( GET_DOUBLEKEYPURE_STATE( GA_UP ) == TRUE )
		{
			m_InputData.pureDoubleUp = true;
		}
		if ( GET_DOUBLEKEY_STATE( GA_UP ) == TRUE )
		{
			m_InputData.oneDoubleUp = true;
		}
		if ( GET_PUREKEY_STATE( GA_UP ) == TRUE )
		{
			m_InputData.pureUp = true;
		}
		if ( GET_KEY_STATE( GA_UP ) == TRUE )
		{
			m_InputData.oneUp = true;
		}

		if ( GET_DOUBLEKEYPURE_STATE( GA_DOWN ) == TRUE )
		{
			m_InputData.pureDoubleDown = true;
		}
		if ( GET_DOUBLEKEY_STATE( GA_DOWN ) == TRUE )
		{
			m_InputData.oneDoubleDown = true;
		}
		if ( GET_PUREKEY_STATE( GA_DOWN ) == TRUE )
		{
			m_InputData.pureDown = true;
		}
		if ( GET_KEY_STATE( GA_DOWN ) == TRUE )
		{
			m_InputData.oneDown = true;
		}

		if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE ) //DIK_V
		{
			if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
				return;

	#ifdef RIDING_SYSTEM
			if ( m_pMyUnit->GetNowState() != m_pMyUnit->GetStateID().m_Wait && m_pMyUnit->GetNowState() != m_pMyUnit->GetStateID().m_iRidingWait )
				return;

			if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITREADY )
				return;

			g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2501 ) );
	#else //RIDING_SYSTEM
			m_pMyUnit->PlayEmotion( CX2Unit::ET_SITREADY );
	#endif //RIDING_SYSTEM

		}
#endif //FIX_ARA_SKILL_BUG
	}

#ifdef RIDING_SYSTEM
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

	if ( false == bFocusEditBox && NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_RIDING ) == TRUE &&/// ���� ���̵� ���� ��� �������� ó���ϴ� ������ �ʿ��ϴ�. �Ŵ������� �޾ƿ���
		( NULL != g_pMain->GetNowState() && static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() ))	
	{
		m_InputData.oneF = true;

		CX2RidingPetManager* pRidingPetManager = CX2RidingPetManager::GetInstance();
		if( NULL != pRidingPetManager && true == pRidingPetManager->GetHasRidingPet() )
		{
			if( true == pRidingPetManager->GetRidingOnState() )
			{
				if ( true == pRidingPetManager->CanPushSummonButton() )
				{
					pRidingPetManager->RefreshSummonButtonElpasedTime();

					if( false == m_pMyUnit->GetRidingOn() )
						pRidingPetManager->Handler_EGS_SUMMON_RIDING_PET_REQ( pRidingPetManager->GetFirstRidingPetUID() );
					else
						pRidingPetManager->Handler_EGS_UNSUMMON_RIDING_PET_REQ();
				}
			}
		}
	}
#endif //RIDING_SYSTEM

	m_pMyUnit->SetInputData( &m_InputData );
}

void CX2TFieldGame::Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( KEGS_SQUARE_UNIT_SYNC_DATA_NOT& kEGS_SQUARE_UNIT_SYNC_DATA_NOT )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( m_pMyUnit != NULL && m_pMyUnit->GetUID() == kEGS_SQUARE_UNIT_SYNC_DATA_NOT.m_UnitUID )
			break;
		if( pCX2SquareUnit != NULL && kEGS_SQUARE_UNIT_SYNC_DATA_NOT.m_UnitUID == pCX2SquareUnit->GetUID() )
		{
			pCX2SquareUnit->RecvPacket( kEGS_SQUARE_UNIT_SYNC_DATA_NOT );
			break;
		}		
	}	
}

bool CX2TFieldGame::Handler_EGS_LEAVE_SQUARE_NOT( KEGS_LEAVE_SQUARE_NOT& kEGS_LEAVE_SQUARE_NOT )
{
	RemoveUnit(	kEGS_LEAVE_SQUARE_NOT.m_iUnitUID );
	
	return true;
}

bool CX2TFieldGame::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT( KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT& kEGS_CHANGE_EQUIPPED_ITEM_NOT )
{
    CX2Unit*    pCX2Unit = GetSquareUnitUnitByUID( kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iUnitUID );

	if( NULL == pCX2Unit )
		return false;

	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iUnitUID );

	if( NULL == pCX2SquareUnit || 
		NULL == pCX2SquareUnit->GetUnit() )
		return false;



    if( true == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.empty() )
    {
#ifdef SERV_TITLE_DATA_SIZE
		pCX2Unit->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iEquippedTitleID);
#else
        pCX2Unit->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_sEquippedTitleID);
#endif
		if ( pCX2SquareUnit != NULL )
            pCX2SquareUnit->UpdateEquippedEmblem();

		return true;
    }


	CX2UnitViewerUI* pCX2UnitViewerUI = NULL;
    if ( pCX2SquareUnit != NULL )
    {
        pCX2UnitViewerUI = pCX2SquareUnit->GetUnitViewer();
        ASSERT( pCX2UnitViewerUI != NULL );
    }//if
	CX2Inventory& kInventory = pCX2Unit->AccessInventory();

	// �� ������ ��쿡
	if ( g_pData->GetMyUser()->GetSelectUnit() != NULL
		&& g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iUnitUID )
	{
#ifdef FIX_WRONG_EQUIP
		// EGS_CHANGE_INVENTORY_SLOT_ACK ��Ŷ�� �����ϱ� ���� �� ��Ŷ�� �����ϸ�
		// ���ŵ��� ���� �� �κ��丮�� �̿��Ͽ� �����ϱ� ������ ������Ʈ�� �� ��
		// �׷��Ƿ� ���⼭ ó������ �ʰ� EGS_CHANGE_INVENTORY_SLOT_ACK ���� ó���ϵ��� ����
#else FIX_WRONG_EQUIP
        if ( pCX2UnitViewerUI != NULL )
		    pCX2UnitViewerUI->UpdateEqip( true );
#endif FIX_WRONG_EQUIP
		return true;
	}


	// �ٸ� ����� ������ ��쿡
	CX2UnitViewerUI* pUnitViewerUIWatch = NULL; 
	if( true ==  g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_WATCH) &&
		g_pData->GetUIManager()->GetUIWatch()->GetTargetUnitUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iUnitUID )
	{
		pUnitViewerUIWatch = g_pData->GetUIManager()->GetUIWatch()->GetUnitViewerUI();
	}


	// ������� ����, ������ ����
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInventorySlotInfo, kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo )
	{
		if( CX2Inventory::ST_E_EQUIP == kInventorySlotInfo.m_cSlotCategory )
		{
			CX2Item* pItem = kInventory.GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if( pItem != NULL )
			{
				if( true == pCX2Unit->RemoveEqip( pItem->GetUID() ) )
				{
                    if ( pCX2UnitViewerUI != NULL )
					    pCX2UnitViewerUI->RemoveEqip( pItem->GetUID() );

					if( NULL != pUnitViewerUIWatch )
					{
						pUnitViewerUIWatch->RemoveEqip( pItem->GetUID() );
					}
				}
			}
		}

		kInventory.RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
	}


	// ������ �߰�, ������� �߰�
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInventorySlotInfo, kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo )
	{
		if( kInventorySlotInfo.m_iItemUID <= 0 )
			continue;

        {
		    CX2Item::ItemData kItemData( kInventorySlotInfo );
		    kInventory.AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, kItemData );
        }

		if( CX2Inventory::ST_E_EQUIP == kInventorySlotInfo.m_cSlotCategory )
		{
			CX2Item* pItem = kInventory.GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if( pItem != NULL )
			{
				if( true == pCX2Unit->AddEqip( pItem->GetUID() ) )
				{
                    if ( pCX2UnitViewerUI != NULL )
					    pCX2UnitViewerUI->AddEqip( pItem->GetUID() );

					if( NULL != pUnitViewerUIWatch )
					{
						pUnitViewerUIWatch->AddEqip( pItem->GetUID() );
					}
				}
			}
		}
	}



	if( pUnitViewerUIWatch != NULL )
	{
		g_pData->GetUIManager()->GetUIWatch()->ResetEquipUI();

		// note!! ���ĳ����������â���� Īȣ�� �ٲ���� �� �������ִ� �κ� �۾��ؾ���

		
	}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ��ȭ ���� ����Ʈ ����
	pCX2UnitViewerUI->DeleteUpgradeWeaponParticle();	/// �����Ǿ� �������� �𸣴�, ����

	pCX2UnitViewerUI->InitUpgradeWeaponParticle();		/// ��ȭ ���� ����Ʈ �ʱ�ȭ
#endif ADD_UPGRADE_WEAPON_PARTICLE
		
	return true;
}


void CX2TFieldGame::Handler_EGS_SQUARE_LIST_REQ()
{
	KEGS_SQUARE_LIST_REQ kPacket;
	kPacket.m_nViewPage		= 1;
	kPacket.m_cSquareType	= (char)CX2SquareGame::ST_TRADE;

	g_pData->GetServerProtocol()->SendPacket( EGS_SQUARE_LIST_REQ, kPacket);
	//g_pMain->AddServerPacket( EGS_SQUARE_LIST_ACK, 180.f );	
}

bool CX2TFieldGame::Handler_EGS_SQUARE_LIST_REQ(int iPage)
{
	KEGS_SQUARE_LIST_REQ kPacket;
	kPacket.m_nViewPage		= iPage;
	kPacket.m_cSquareType	= (char)CX2SquareGame::ST_TRADE;

	g_pData->GetServerProtocol()->SendPacket( EGS_SQUARE_LIST_REQ, kPacket);
	
	return true;
}

bool CX2TFieldGame::Handler_EGS_SQUARE_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SQUARE_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	//if( g_pMain->DeleteServerPacket( EGS_SQUARE_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
            if ( m_pSquareUI != NULL )
			    m_pSquareUI->OpenSquareListDLG();

			m_LastSquareListAck = kEvent;

			if( NULL != m_pSquareUI )
			{				
				m_pSquareUI->UpdateSquareSlotList( kEvent );				
			}

			//m_pSquareUI->SetPos(D3DXVECTOR2(800.f, 200.f));
            if ( m_pSquareUI != NULL )
			    m_pSquareUI->SetShow( true );			
			m_bShowMarketList = true;
			SetEnableKeyProcess(false);
			//m_bEnableKey = false;
		}
		return true;
	}

	return false;
}

vector<CX2SquareUnit*> CX2TFieldGame::GetUnitList( int& iPage, int iUnitNumPerPage /*= 10 */ )
{
	if( iUnitNumPerPage <= 0 )
		iUnitNumPerPage = 1;

	vector<CX2SquareUnit*> vecUnitList;

	int nNumUnit = (int)m_UserUnitList.size();
	int iMaxPage = (nNumUnit + iUnitNumPerPage - 1) / iUnitNumPerPage; 
	if( iMaxPage < 1 )
		return vecUnitList;

	if( iPage > iMaxPage )
		iPage = iMaxPage;

	if( iPage < 1 )
		iPage = 1;

	int iStartIndex = (iPage-1) * iUnitNumPerPage; 
	int iEndIndex;

	if( iStartIndex < (int)m_UserUnitList.size() )
	{
		iEndIndex = min( (int)m_UserUnitList.size(), iStartIndex + iUnitNumPerPage );
		for( int i=iStartIndex; i<iEndIndex; i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
            if ( pCX2SquareUnit != NULL )
			    vecUnitList.push_back( pCX2SquareUnit.get() );
		}
	}

	return vecUnitList;
}




void CX2TFieldGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( iUnitUID_ );
	if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetInit() == true && pCX2SquareUnit->GetShowObject() == true &&
		pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_NONE )
	{
#ifdef UNIT_EMOTION
		bool bCommandEmotion = g_pChatBox->IsEmotionID(pWstrMsg_);
		if( g_pChatBox != NULL && bCommandEmotion== false )

#endif
		{
            if( pCX2SquareUnit->GetBoundingRadius() > 0 )
            {
				D3DXVECTOR3 center;
				pCX2SquareUnit->GetTransformCenter( &center );
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                float   fScaledBoundingRadius =pCX2SquareUnit->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
				//�ø�
				float fScale;
				if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetYScale() )
				{
					if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
					{
						//X�� ���� ŭ
						fScale = pCX2SquareUnit->GetMatrix().GetXScale();
					}
					else
					{
						//Z�� ���� ŭ
						fScale = pCX2SquareUnit->GetMatrix().GetZScale();
					}
				}
				else
				{
					if( pCX2SquareUnit->GetMatrix().GetYScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
					{
						//Y�� ���� ŭ
						fScale = pCX2SquareUnit->GetMatrix().GetYScale();
					}
					else
					{
						//Z�� ���� ŭ
						fScale = pCX2SquareUnit->GetMatrix().GetZScale();
					}
				}
                float   fScaledBoundingRadius = pCX2SquareUnit->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

				if( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
					return;
            }

			CX2TalkBoxManagerImp::TalkBox talkBox;
			talkBox.m_OwnerUnitUID		= iUnitUID_;
			talkBox.m_wstrTalkContent	= pWstrMsg_;
			talkBox.m_bTraceUnit		= true;
			talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
			talkBox.m_coTextColor		= coTextColor_;
			talkBox.m_OutLineTextColor	= coOutColor_;
			talkBox.m_BackTexColor		= coBackColor_;
			
			talkBox.m_bUseOutLine		 = true;
			if( coOutColor_ == 0x00000000 )	//	�Է� ���� ���� �۾��� �״�� ����
			{
				talkBox.m_bUseOutLine = false;
			}

            if ( m_pTalkBoxManager != NULL )
			    m_pTalkBoxManager->Push( talkBox );
		}

#ifdef UNIT_EMOTION
		// �̸�� ��������ش�.
		if(g_pChatBox != NULL)
		{
			// ���� ���¿����� ä�����͸� ������ �ʵ��� �Ѵ�.(��ɾ�θ� �̸�ǵ�������)
			if( bCommandEmotion == false && pCX2SquareUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
				return;

			pCX2SquareUnit->PlayEmotion( g_pChatBox->GetEmotionID(pWstrMsg_) );			
		}
#endif
	}
}

CX2SquareUnit* CX2TFieldGame::GetSquareUnit( int index )
{
	if( index >= (int)m_UserUnitList.size() )
		return NULL;
	return m_UserUnitList[index].get();
}


CX2SquareUnit* CX2TFieldGame::GetSquareUnitByUID( UidType UID )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( pCX2SquareUnit->GetUID() == UID )
			{
				return pCX2SquareUnit.get();
			}
		}
	}

	return NULL;
}


CX2Unit* CX2TFieldGame::GetSquareUnitUnitByUID( UidType UID )
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetUnit() != NULL )
		{
			if( pCX2SquareUnit->GetUnit()->GetUID() == UID )
			{
				return pCX2SquareUnit->GetUnit();
			}
		}
	}

    BOOST_TEST_FOREACH( const AddUserInUnitLoader&, addUser, m_listAddUserInUnitLoader )
    {
        if ( addUser.m_pUnit != NULL && addUser.m_pUnit->GetUID() == UID )
        {
            return addUser.m_pUnit;
        }//if
    }//BOOST_FOREACH()

	return NULL;
}


CX2SquareUnit* CX2TFieldGame::GetSquareUnitByNickName( const WCHAR* wszNickName )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( 0 == pCX2SquareUnit->GetNickName().compare( wszNickName ) )
			{
				return pCX2SquareUnit.get();
			}
		}
	}

	return NULL;
}

CX2Unit*    CX2TFieldGame::GetSquareUnitUnitByNickName( const WCHAR* wszNickName )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetUnit() != NULL )
		{
			if( 0 == StrCmp( pCX2SquareUnit->GetUnit()->GetNickName(), wszNickName ) )
			{
				return pCX2SquareUnit->GetUnit();
			}
		}
	}

    BOOST_TEST_FOREACH( const AddUserInUnitLoader&, addUser, m_listAddUserInUnitLoader )
    {
        if ( addUser.m_pUnit != NULL && 0 == StrCmp( addUser.m_pUnit->GetNickName(), wszNickName ) )
        {
            return addUser.m_pUnit;
        }//if
    }//BOOST_FOREACH()

	return NULL;
}

#ifdef SERV_INVISIBLE_GM
bool CX2TFieldGame::IsInvisible( UidType UID ) const
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( pCX2SquareUnit->GetUID() == UID )
			{
				return pCX2SquareUnit->GetUnit()->AccessUnitData().IsInvisible();
			}
		}
	}

	return false;
}
#endif SERV_INVISIBLE_GM

void CX2TFieldGame::SetShowUserUnit( bool bShow )
{
	for( UINT i=0; i<m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			pCX2SquareUnit->SetShowObject( bShow );
		}
	}
}

void CX2TFieldGame::SetFreeCamera( bool bFreeCamera )
{
	m_bFreeCamera = bFreeCamera;

	if( true == m_bFreeCamera )
	{
        if ( m_pCamera != NULL )
        {
		    D3DXVECTOR3 vEye	= m_pCamera->GetCamera().GetEye();
		    D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();

		    m_FPSCamera.SetViewParams( &vEye, &vLookAt );
        }//if
#ifdef MODIFY_GAME_EDIT_CAMERA
		m_FPSCamera.ClearKeys();
#endif //MODIFY_GAME_EDIT_CAMERA
	}
}

void CX2TFieldGame::SyncUnitFrame()
{
    if ( m_pMyUnit == NULL )
        return;

	std::vector<CX2SquareUnit::SyncUnitState> syncData;
	D3DXVECTOR3 unitPos(0,0,0);

    unitPos = m_pMyUnit->GetHistorySummary(syncData);

	//dbg::clog << L"Sync Pos : " << unitPos.x << ", "<< unitPos.y << ", " << unitPos.z << dbg::endl;
	//if(m_pMyUnit->GetFootOnLine_LUA(m_pMyUnit->GetIsRight()) == false)
	//	return;
	//if(m_UserUnitList.size() <= 1)
	// 	return;
	if(m_bLoadComplete == false)
		return;

	bool bAllWait = true;	
	for(unsigned int i=0; i<syncData.size(); ++i)
	{
		if(syncData[i].m_eventID != CX2SquareUnit::SUS_W)
		{
			bAllWait = false;	
			break;
		}
	}
	
	if(bAllWait == true && m_bForceSync == false)
		return;

	if(bAllWait == false)
		m_bForceSync = true;
	else	
		m_bForceSync = false;

	KEGS_FIELD_UNIT_SYNC_DATA_NOT kPacket;

	kPacket.m_vPos.x = unitPos.x;
	kPacket.m_vPos.y = unitPos.y;
	kPacket.m_vPos.z = unitPos.z;
    kPacket.m_bIsRight = m_pMyUnit->GetIsRight();
	
	for(unsigned int i = 0; i<syncData.size(); ++i)
	{
		KFieldFrameInfo node;
		node.m_StateID = syncData[i].m_eventID;
		node.m_FrameCount = syncData[i].m_elapsedFrame;
		kPacket.m_vecFieldFrame.push_back(node);
	}


#ifdef REMEMBER_LOGOUT_POSITION_TEST
	
	kPacket.m_ucLastTouchLineIndex = m_pMyUnit->GetLastTouchLineIndex();
	const CKTDGLineMap::LineData* pLineData = GetWorld()->GetLineMap()->GetLineData( m_pMyUnit->GetLastTouchLineIndex() );

	float fLastPosValue = 0.5f;

	if( NULL != pLineData )
	{
		D3DXVECTOR3 vLineVec = pLineData->endPos - pLineData->startPos;
		D3DXVECTOR3 vPosVec = unitPos - pLineData->startPos;
		
		float fLineLength = D3DXVec3Length( &vLineVec );
		float fPosLength = D3DXVec3Length( &vPosVec );

		if( 0.f != fLineLength )
		{
			fLastPosValue = fPosLength / fLineLength;
		}
	}

	kPacket.m_usLastPosValue = floatToHalf( fLastPosValue );


#endif REMEMBER_LOGOUT_POSITION_TEST

	g_pData->GetServerProtocol()->SendPacket( EGS_FIELD_UNIT_SYNC_DATA_NOT, kPacket );
}

bool CX2TFieldGame::ReceiveSyncBroadNot(KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT& broadNot)
{
	KTDXPROFILE();

	UidType syncUid = broadNot.m_UnitUID;

	CX2SquareUnitPtr pUnit;// = NULL;
	for(unsigned int i=0; i<m_UserUnitList.size(); ++i)
	{
		pUnit = m_UserUnitList[i];
		if( pUnit != NULL && pUnit->GetUID() == syncUid)
		{
			// ���� ��Ŷ���� Ÿ���� ��ũ�� �����.
			pUnit->RecvTRPacket(broadNot);
			return true;
		}
	}
    pUnit.reset();
    
	// ��������Ʈ�� �������� �ʴ´ٸ� �� ��Ƽ������ �˻��Ͽ� ��Ƽ��������� ��ġ������ �����Ѵ�.
	if(g_pData != NULL && g_pData->GetPartyManager()->DoIHaveParty() == true)
	{
		if(g_pData->GetPartyManager()->IsMyPartyMember(syncUid) == true)
		{
			D3DXVECTOR3 vPos = D3DXVECTOR3( broadNot.m_vPos.x, broadNot.m_vPos.y, broadNot.m_vPos.z );
			g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData(syncUid)->m_vPosition = vPos;		
		}
	}

	return true;
}

bool CX2TFieldGame::ConnectTR()
{
	if(g_pData != NULL && g_pData->GetServerProtocol() != NULL && g_pData->GetServerProtocol()->IsTRConnected() == false)
	{
		UidType unitUID;
		if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_750 ), g_pMain->GetNowState() );
			return false;
		}//if
		unitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

		if( g_pData->GetServerProtocol()->ConnectedToTCPRelay( g_pMain->GetTRServerIp(), unitUID ) == true )
		{
			g_pData->GetServerProtocol()->SendTRPacket(ETR_REG_UID, unitUID);
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_750 ), g_pMain->GetNowState() );
			return false;
		}		
	}		

	return true;
}

void CX2TFieldGame::AddFieldNpc()
{	
	if(g_pData == NULL)
		return;

	CX2LocationManager::VillageTemplet	*pVillageTemplate;
	CX2LocationManager::HouseTemplet	*pHouseTemplate;
	SEnum::VILLAGE_MAP_ID	villageId;
	D3DXVECTOR3 vPos;	

	g_pData->GetLocationManager()->GetVillage(villageId, vPos);
	pVillageTemplate = g_pData->GetLocationManager()->GetVillageMapTemplet(villageId);

	if(pVillageTemplate != NULL)
	{
//#ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
//		g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );
//#endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST

		for(UINT i=0; i<pVillageTemplate->m_HouseList.size(); ++i)
		{
			pHouseTemplate = g_pData->GetLocationManager()->GetHouseTemplet(pVillageTemplate->m_HouseList[i]);
			if(pHouseTemplate != NULL)
			{
				bool bFindNpcPos = true;
				if( NULL != GetWorld() && NULL != GetWorld()->GetWorldData() )
				{
					// Village�� ��ϵ� npc�� commonPos�� ���������� �ش���尡 �������� ������ npc�������� �ʴ´�.				
					CX2World::WORLD_ID eWorldID = GetWorld()->GetWorldData()->worldID;
					if(pHouseTemplate->m_vecCommonPos.empty() == false)
					{				
						bFindNpcPos = false;
						for(UINT i=0; i<pHouseTemplate->m_vecCommonPos.size(); ++i)
						{
							if( eWorldID == pHouseTemplate->m_vecCommonPos[i].m_eWorldID )
							{
								bFindNpcPos = true;
								break;
							}
						}					
					}	
				}			

#ifdef SERV_IN_VILLAGE_EVENT_NPC_BY_CODE_EVENT
				IF_EVENT_ENABLED( CEI_EVENT_IN_VILLAGE_EVENT_NPC )
				{
					if( pHouseTemplate->m_ID == CX2LocationManager::HI_EVENT_BENDERS)
					{
						continue;
					}
				}
				ELSE
				{
				}
#endif //SERV_IN_VILLAGE_EVENT_NPC_BY_CODE_EVENT

#ifdef SERV_2013_SILVER_WEEK_TITLE
				IF_EVENT_ENABLED( CEI_2013_JUNGCHU_TITLE )
				{
				}
				ELSE
				{
					if( pHouseTemplate->m_ID == CX2LocationManager::HI_APINK_ARCHANGEL)
					{
						continue;
					}
				}
#endif //SERV_2013_SILVER_WEEK_TITLE

				// note: ��� ������ �ﷻ ��ġ�� ���������� �����Ǿ��ִ��� �׽�Ʈ ���ִ� unit test �ϳ� ������ָ� ���ڴ�~ ^^
				ASSERT( true == bFindNpcPos && "dlg_map_house.lua���� �ش� npc�� �ش� world������ ��ġ�� �����Ǿ��־�� �մϴ�." );
				if(bFindNpcPos == true)
				{
//#ifdef  NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
					CX2TFieldNpcPtr npcNode = CX2TFieldNpc::CreateTFieldNPC( true );
//#else   NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
//					CX2TFieldNpcPtr npcNode = CX2TFieldNpc::CreateTFieldNPC( false );
//#endif  NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST

					if(npcNode != NULL)
					{
						npcNode->CreateFieldNpc( GetWorld(), pHouseTemplate );
						m_vecFieldNpc.push_back( npcNode );

						npcNode->SetLayer( XL_SKY_WORLD_OBJECT_2 );						
						g_pKTDXApp->GetDGManager()->AddObjectChain( npcNode.get() );
						npcNode->SetShowObject( false );

#ifdef APINK_NPC
						if( pHouseTemplate->m_NPCList.size() > 0 && pHouseTemplate->m_NPCList[0] == CX2UnitManager::NUI_EVENT_CRAYONPOP )
						{
							m_pAPinkSound = g_pKTDXApp->GetDeviceManager()->OpenSound( L"Music_NPC.ogg", 10, false );
							if( m_pAPinkSound != NULL )
							{						
								m_pAPinkSound->Play( true, false );								
								m_pAPinkSound->SetVolume(0.f);
							}
						}
#endif
					}//if
				}
			}//if
		}//for

//#ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
//		g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );
//#endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
	}

}
//#ifdef MODIFY_PORTAL_GATE
void CX2TFieldGame::CreatePortalGate()
{	
	PortalGate PortalGate_;
	//PortalGate_.m_pPortalGateParticle1	= NULL;
	//PortalGate_.m_pPortalGateParticle2	= NULL;
	//PortalGate_.m_pPortalGateParticle3	= NULL;
	//PortalGate_.m_pPortalGateParticle4	= NULL;
	//PortalGate_.m_pPortalGateParticle5	= NULL;
	PortalGate_.m_hPortalGate1			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate1", 0.0f, 0.0f, 0.0f);
	PortalGate_.m_hPortalGate2			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate2", 0.0f, 0.0f, 0.0f);
	PortalGate_.m_hPortalGate3			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate3", 0.0f, 0.0f, 0.0f);
	PortalGate_.m_hPortalGate4			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate4", 0.0f, 0.0f, 0.0f);
	PortalGate_.m_hPortalGate5			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate5", 0.0f, 0.0f, 0.0f);

	CKTDGParticleSystem::CParticleEventSequence* pSeqPortal	= GetUiParticle()->GetInstanceSequence( PortalGate_.m_hPortalGate1 );
	ASSERT( pSeqPortal != NULL );
	if( pSeqPortal != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        PortalGate_.m_hPortalGateParticle1 = pSeqPortal->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		PortalGate_.m_pPortalGateParticle1 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeqPortal->SetShowObject(false);
	}
	pSeqPortal	= GetUiParticle()->GetInstanceSequence( PortalGate_.m_hPortalGate2 );
	ASSERT( pSeqPortal != NULL );
	if( pSeqPortal != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        PortalGate_.m_hPortalGateParticle2 = pSeqPortal->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		PortalGate_.m_pPortalGateParticle2 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeqPortal->SetShowObject(false);
	}
	pSeqPortal	= GetUiParticle()->GetInstanceSequence( PortalGate_.m_hPortalGate3 );
	ASSERT( pSeqPortal != NULL );
	if( pSeqPortal != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        PortalGate_.m_hPortalGateParticle3 = pSeqPortal->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		PortalGate_.m_pPortalGateParticle3 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeqPortal->SetShowObject(false);
	}
	pSeqPortal	= GetUiParticle()->GetInstanceSequence( PortalGate_.m_hPortalGate4 );
	ASSERT( pSeqPortal != NULL );
	if( pSeqPortal != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        PortalGate_.m_hPortalGateParticle4 = pSeqPortal->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		PortalGate_.m_pPortalGateParticle4 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeqPortal->SetShowObject(false);
	}
	pSeqPortal	= GetUiParticle()->GetInstanceSequence( PortalGate_.m_hPortalGate5 );
	ASSERT( pSeqPortal != NULL );
	if( pSeqPortal != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        PortalGate_.m_hPortalGateParticle5 = pSeqPortal->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		PortalGate_.m_pPortalGateParticle5 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeqPortal->SetShowObject(false);
	}

	m_vecPortalGateParticle.push_back(PortalGate_);
	++m_uiPortalCount;
}
void CX2TFieldGame::SetShowPortalGate(bool bShow,  UINT uiPortalIndex, D3DXVECTOR3 vPos)
{		
	if( uiPortalIndex >= m_uiPortalCount )
	{
		CreatePortalGate();
	}

	if( uiPortalIndex >= m_uiPortalCount )
	{
		return;
	}

	D3DXVECTOR3 vLinkPosTemp = vPos;
	vLinkPosTemp.y += 2.f;					

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
    PortalGate& portalGate = m_vecPortalGateParticle[uiPortalIndex];

	pSeq = GetUiParticle()->GetInstanceSequence( portalGate.m_hPortalGate1 );
	if( NULL != pSeq )
	{				
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = pSeq->ValidateParticleHandle( portalGate.m_hPortalGateParticle1 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeq->ValidateParticlePointer( portalGate.m_pPortalGateParticle1 );
        CKTDGParticleSystem::CParticle* pParticle = portalGate.m_pPortalGateParticle1;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( pParticle != NULL )
			pParticle->SetPos( vLinkPosTemp );	
		pSeq->SetPosition(vLinkPosTemp);
		pSeq->SetShowObject( bShow );
	}				
	pSeq = GetUiParticle()->GetInstanceSequence( portalGate.m_hPortalGate2 );
	if( NULL != pSeq )
	{					
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = pSeq->ValidateParticleHandle( portalGate.m_hPortalGateParticle2 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeq->ValidateParticlePointer( portalGate.m_pPortalGateParticle2 );
        CKTDGParticleSystem::CParticle* pParticle = portalGate.m_pPortalGateParticle1;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( pParticle != NULL )
			pParticle->SetPos( vPos );
		pSeq->SetPosition(vPos);
		pSeq->SetShowObject( bShow );
	}
	pSeq = GetUiParticle()->GetInstanceSequence( portalGate.m_hPortalGate3 );
	if( NULL != pSeq )
	{							
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = pSeq->ValidateParticleHandle( portalGate.m_hPortalGateParticle3 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeq->ValidateParticlePointer( portalGate.m_pPortalGateParticle3 );
        CKTDGParticleSystem::CParticle* pParticle = portalGate.m_pPortalGateParticle1;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( pParticle != NULL )
			pParticle->SetPos( vPos );
		pSeq->SetPosition(vPos);
		pSeq->SetShowObject( bShow );
	}
	pSeq = GetUiParticle()->GetInstanceSequence( portalGate.m_hPortalGate4 );
	if( NULL != pSeq )
	{							
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = pSeq->ValidateParticleHandle( portalGate.m_hPortalGateParticle4 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeq->ValidateParticlePointer( portalGate.m_pPortalGateParticle4 );
        CKTDGParticleSystem::CParticle* pParticle = portalGate.m_pPortalGateParticle1;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( pParticle != NULL )
			pParticle->SetPos( vPos );
		pSeq->SetPosition(vPos);
		pSeq->SetShowObject( bShow );
	}
	pSeq = GetUiParticle()->GetInstanceSequence( portalGate.m_hPortalGate5 );
	if( NULL != pSeq )
	{							
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = pSeq->ValidateParticleHandle( portalGate.m_hPortalGateParticle5 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pSeq->ValidateParticlePointer( portalGate.m_pPortalGateParticle5 );
        CKTDGParticleSystem::CParticle* pParticle = portalGate.m_pPortalGateParticle1;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if (pParticle != NULL )
			pParticle->SetPos( vPos );
		pSeq->SetPosition(vPos);
		pSeq->SetShowObject( bShow );
	}
}
//#else
//void CX2TFieldGame::CreatePortalGate()
//{
//	m_pPortalGateParticle1	= NULL;
//	m_pPortalGateParticle2	= NULL;
//	m_pPortalGateParticle3	= NULL;
//	m_pPortalGateParticle4	= NULL;
//	m_pPortalGateParticle5	= NULL;
//	m_hPortalGate1			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate1", 0.0f, 0.0f, 0.0f);
//	m_hPortalGate2			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate2", 0.0f, 0.0f, 0.0f);
//	m_hPortalGate3			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate3", 0.0f, 0.0f, 0.0f);
//	m_hPortalGate4			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate4", 0.0f, 0.0f, 0.0f);
//	m_hPortalGate5			= GetUiParticle()->CreateSequenceHandle( NULL,  L"PotalGate5", 0.0f, 0.0f, 0.0f);
//
//	CKTDGParticleSystem::CParticleEventSequence* pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hPortalGate1 );
//    ASSERT( pSeqPortal != NULL );
//	if( pSeqPortal != NULL )
//	{		
//		m_pPortalGateParticle1 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
//		pSeqPortal->SetShowObject(false);
//	}
//	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hPortalGate2 );
//    ASSERT( pSeqPortal != NULL );
//	if( pSeqPortal != NULL )
//	{		
//		{
//			m_pPortalGateParticle2 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
//		}
//		pSeqPortal->SetShowObject(false);
//	}
//	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hPortalGate3 );
//    ASSERT( pSeqPortal != NULL );
//	if( pSeqPortal != NULL )
//	{		
//		m_pPortalGateParticle3 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
//		pSeqPortal->SetShowObject(false);
//	}
//	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hPortalGate4 );
//    ASSERT( pSeqPortal != NULL );
//	if( pSeqPortal != NULL )
//	{		
//		m_pPortalGateParticle4 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
//		pSeqPortal->SetShowObject(false);
//	}
//	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hPortalGate5 );
//    ASSERT( pSeqPortal != NULL );
//	if( pSeqPortal != NULL )
//	{		
//		m_pPortalGateParticle5 = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
//		pSeqPortal->SetShowObject(false);
//	}
//
//}
//void CX2TFieldGame::SetShowPortalGate(bool bShow, D3DXVECTOR3 vPos)
//{	
//	D3DXVECTOR3 vLinkPosTemp = vPos;
//	vLinkPosTemp.y += 2.f;					
//	
//	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
//
//	pSeq = GetUiParticle()->GetInstanceSequence( m_hPortalGate1 );
//	if( NULL != pSeq )
//	{								
//        pSeq->ValidateParticlePointer( m_pPortalGateParticle1 );
//        if ( m_pPortalGateParticle1 != NULL )
//            m_pPortalGateParticle1->SetPos( vLinkPosTemp );	
//		pSeq->SetPosition(vLinkPosTemp);
//		pSeq->SetShowObject( bShow );
//	}				
//	pSeq = GetUiParticle()->GetInstanceSequence( m_hPortalGate2 );
//	if( NULL != pSeq )
//	{					
//        pSeq->ValidateParticlePointer( m_pPortalGateParticle2 );
//        if ( m_pPortalGateParticle2 != NULL )
//            m_pPortalGateParticle2->SetPos( vPos );
//		pSeq->SetPosition(vPos);
//		pSeq->SetShowObject( bShow );
//	}
//	pSeq = GetUiParticle()->GetInstanceSequence( m_hPortalGate3 );
//	if( NULL != pSeq )
//	{							
//        pSeq->ValidateParticlePointer( m_pPortalGateParticle3 );
//        if ( m_pPortalGateParticle3 != NULL )
//            m_pPortalGateParticle3->SetPos( vPos );
//		pSeq->SetPosition(vPos);
//		pSeq->SetShowObject( bShow );
//	}
//	pSeq = GetUiParticle()->GetInstanceSequence( m_hPortalGate4 );
//	if( NULL != pSeq )
//	{							
//        pSeq->ValidateParticlePointer( m_pPortalGateParticle4 );
//        if ( m_pPortalGateParticle4 != NULL )
//            m_pPortalGateParticle4->SetPos( vPos );
//		pSeq->SetPosition(vPos);
//		pSeq->SetShowObject( bShow );
//	}
//	pSeq = GetUiParticle()->GetInstanceSequence( m_hPortalGate5 );
//	if( NULL != pSeq )
//	{							
//        pSeq->ValidateParticlePointer( m_pPortalGateParticle5 );
//        if ( m_pPortalGateParticle5 != NULL )
//            m_pPortalGateParticle5->SetPos( vPos );
//		pSeq->SetPosition(vPos);
//		pSeq->SetShowObject( bShow );
//	}
//}
//#endif //MODIFY_PORTAL_GATE

void CX2TFieldGame::ChangeNpcType(CX2TFieldNpcShop::NPC_SHOP_BUTTON_TYPE eType)
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{			
			pNpcNode->ChangeType(eType);
			return;
		}
	}
}

void CX2TFieldGame::ByeNpc()
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{			
			pNpcNode->SetNpcBye(true);
			return;
		}
	}
}

bool CX2TFieldGame::TalkNpc()
{
    if ( m_pMyUnit == NULL )
        return false;

	// �������Ա����� Ȯ��
	if(m_bJoinMarket == true && m_iJoinMarketIndex >= 0)
	{
		// ��������
		//D3DXVECTOR3 vPos = g_pData->GetLocationManager()->GetStartPosLoc(m_vecMarketPos[m_iJoinMarketIndex]);
		g_pData->GetLocationManager()->SetLastPos( m_pMyUnit->GetLandPosition() );
		g_pData->GetLocationManager()->SetLastPosLineIndex( m_pMyUnit->GetLastTouchLineIndex() );

	    if ( m_pSquareUI != NULL )			
		    m_pSquareUI->SetFocus(0);
		Handler_EGS_SQUARE_LIST_REQ();
		//SetEnableKeyProcess(false);		
	}
	

	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == false)
		{
			if(pNpcNode->GetCanTalkNpc() == false)
			{
				// ���ɼ� ���� npc
				return false;
			}
			m_iJoinNpcIndex = i;
			pNpcNode->SetNpcTalk(true);
			m_bJoinNpc = true;
			g_pData->GetLocationManager()->SetLastPos( m_pMyUnit->GetLandPosition() );
			g_pData->GetLocationManager()->SetLastPosLineIndex( m_pMyUnit->GetLastTouchLineIndex() );

			
			g_pData->GetUIManager()->CloseAll();			

            if ( m_pNpcShadow != NULL )
			    m_pNpcShadow->OpenShadow();
			
			g_pChatBox->SetChatBoxLayerUp(true);
			g_pChatBox->HideChatWindowFrame();
			g_pChatBox->HideChatWindow();
			g_pChatBox->HideChatEditBox();			
			g_pChatBox->CloseChatWindow();
			//g_pChatBox->CloseChatWindowButton();
			g_pChatBox->OpenChatWindowButton();
			//GetMinimap()->SetShowObject(false);
#ifndef REFORM_NOVICE_GUIDE
            if ( m_pNoviceGuide != NULL )
				m_pNoviceGuide->SetHide(true);
#endif //REFORM_NOVICE_GUIDE


			if( g_pData->GetMessenger() != NULL )
				g_pData->GetMessenger()->SetHideWindow(true);

			if( pNpcNode->m_NpcHouseID == CX2LocationManager::HI_POSTBOX )
			{
				if(g_pMain->GetPostBox() != NULL)
					g_pMain->GetPostBox()->SetDialogShow(false);
			}

			//pNpcNode->GetQuestInfo();

			GetTalkBoxManager()->SetShowTalk(false);
			//GetTalkBoxManagerPartyLeader()->SetShowTalk(false);

//			g_pMain->GetPartyUI()->OpenPartyMemberDLG( false );
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( false );

			//{{ ����� : [2011/3/24/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
			//	����̼� ���� â ����
			//g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowEXPBonusDlg( false );
			g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( false );
#endif SERV_INSERT_GLOBAL_SERVER
			//}} ����� : [2011/3/24/] //	���� �̼�
#ifndef REFORM_NOVICE_GUIDE
            if ( GetNoviceGuide() != NULL )
			    GetNoviceGuide()->SetMouseClickIndex(0);
#endif //REFORM_NOVICE_GUIDE
#ifdef ADDED_RELATIONSHIP_SYSTEM
			if ( NULL != g_pData->GetRelationshipEffectManager() )
				g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect(CX2RelationshipEffectManager::REST_HIDE_ALL);
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
			SendTimeControlItemListTalk();
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
#ifdef SERV_GLOBAL_MISSION_MANAGER
			g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( false );
#endif SERV_GLOBAL_MISSION_MANAGER
			return true;
		}
	}

	return false;
}

bool CX2TFieldGame::TalkNpc(int iNpc)
{	
	
	CX2TFieldNpc *pNpcNode = GetFieldNPC(iNpc);

	if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == false && pNpcNode->GetPickNpc())
	{
		if(pNpcNode->GetCanTalkNpc() == false)
		{

			//{{ kimhc // 2009-12-04 // ũ�������� Ʈ�� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
			switch ( pNpcNode->m_NpcHouseID )
			{
			case CX2LocationManager::HI_CHRISTMAS_TREE:
				{
					return Handler_EGS_CHECK_TIME_EVENT_COMPLETE_REQ();				
				}
				break;

			default:
				return false;
			}
#else	CHRISTMAS_TREE
			// ���ɼ� ���� npc
			return false;
#endif	CHRISTMAS_TREE
			//}} kimhc // 2009-12-04 // ũ�������� Ʈ�� �̺�Ʈ

		}
		m_iJoinNpcIndex = iNpc;
		pNpcNode->SetNpcTalk(true);
		m_bJoinNpc = true;
		g_pData->GetLocationManager()->SetLastPos( m_pMyUnit->GetLandPosition() );
		g_pData->GetLocationManager()->SetLastPosLineIndex( m_pMyUnit->GetLastTouchLineIndex() );


		g_pData->GetUIManager()->CloseAll();			

        if ( m_pNpcShadow != NULL )
		    m_pNpcShadow->OpenShadow();

		g_pChatBox->SetChatBoxLayerUp(true);
		g_pChatBox->HideChatWindowFrame();
		g_pChatBox->HideChatWindow();
		g_pChatBox->HideChatEditBox();			
		g_pChatBox->CloseChatWindow();
		g_pChatBox->OpenChatWindowButton();
		//g_pChatBox->CloseChatWindowButton();
		//GetMinimap()->SetShowObject(false);
#ifndef REFORM_NOVICE_GUIDE
        if ( m_pNoviceGuide != NULL )
			m_pNoviceGuide->SetHide(true);
#endif //REFORM_NOVICE_GUIDE


		if( g_pData->GetMessenger() != NULL )
			g_pData->GetMessenger()->SetHideWindow(true);

		if( pNpcNode->m_NpcHouseID == CX2LocationManager::HI_POSTBOX )
		{
			if(g_pMain->GetPostBox() != NULL)
				g_pMain->GetPostBox()->SetDialogShow(false);
		}

		//pNpcNode->GetQuestInfo();

        if ( m_pTalkBoxManager != NULL )
		    m_pTalkBoxManager->SetShowTalk(false);
		//GetTalkBoxManagerPartyLeader()->SetShowTalk(false);

//		g_pMain->GetPartyUI()->OpenPartyMemberDLG( false );
		g_pMain->GetPartyUI()->OpenPartyFeverDLG( false );

		//{{ ����� : [2011/3/24/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
		//	����̼� ���� â ����
		//g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowEXPBonusDlg( false );
		g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( false );
#endif SERV_INSERT_GLOBAL_SERVER

#ifndef REFORM_NOVICE_GUIDE
        if ( GetNoviceGuide() != NULL )
			GetNoviceGuide()->SetMouseClickIndex(0);
#endif //REFORM_NOVICE_GUIDE
#ifdef ADDED_RELATIONSHIP_SYSTEM
		if ( NULL != g_pData->GetRelationshipEffectManager() )
			g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect(CX2RelationshipEffectManager::REST_HIDE_ALL);
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_GLOBAL_MISSION_MANAGER
		g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( false );
#endif SERV_GLOBAL_MISSION_MANAGER

		return true;
	}

	return false;
}

bool CX2TFieldGame::GetJoinNpc()
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{
			return true;
		}
	}

	return false;
}

int CX2TFieldGame::GetJoinNpcId()
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{
			return pNpcNode->GetNpcId(); 
		}
	}

	return 0;
}


int CX2TFieldGame::GetJoinNpcIndex()
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{
			return i; 
		}
	}

	return 0;
}

void CX2TFieldGame::SetShowNpcMessage(bool val)
{
	if( val == false && g_pData->GetUIManager()->GetShowNpcShop() == true )
	{		
		return;
	}

	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{
			pNpcNode->SetNpcBye(!val);
            if ( pNpcNode->GetNpcShop() != NULL )
			    pNpcNode->GetNpcShop()->SetShow(val);	

			// �ʽ��� ���̵�
			//if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true)
			//{
			//	if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP7)
			//	{
			//		g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
			//	}	
			//	else if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP2 )
			//	{				
			//		g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_NEXT);
			//	}
			//}
		}
	}
}

void CX2TFieldGame::SetQuestDesc(int npcId, const wstring &title, const wstring &message)
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
		if( pNpcNode != NULL && npcId == pNpcNode->GetNpcId())
		{
			if(pNpcNode->GetNpcShop() != NULL)
			{
				pNpcNode->GetNpcShop()->SetQuestDesc(title, message);
				return;
			}
		}
	}
}

void CX2TFieldGame::SetNpcMessage(int npcId, const wstring &npcMessage)
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
		if( pNpcNode != NULL && npcId == pNpcNode->GetNpcId())
		{
			if(pNpcNode->GetNpcShop() != NULL)
			{
				pNpcNode->GetNpcShop()->SetNpcMessage(npcMessage);
				return;
			}
		}
	}
}

void CX2TFieldGame::CloseMarketList()
{
	m_bShowMarketList = false;
    if ( m_pSquareUI != NULL )
	    m_pSquareUI->SetShow( false );
}

bool CX2TFieldGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifndef REFORM_NOVICE_GUIDE
	if(m_pNoviceGuide != NULL && GetJoinNpc() == false)
	{	
		if(m_pNoviceGuide->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif //REFORM_NOVICE_GUIDE

	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{
			if(pNpcNode->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
				return true;
		}
	}

	// �����
#ifdef CHARACTER_MOUSE_RBUTTON
	if ( g_pData->GetUserListPopUp() != NULL && g_pData->GetUserListPopUp()->GetShowMenu() == true )
		 g_pData->GetUserListPopUp()->UICustomEventProc( hWnd, uMsg, wParam , lParam );
#endif 
	// �����

	// square list ui
	switch( wParam )
	{
	case MLUI_CLOSE:
		{
			SetEnableKeyProcess(true);
			//m_bEnableKey = true;
			CloseMarketList();			
		}
		return true;

	case MLUI_PREV:
		{
            if ( m_pSquareUI != NULL )
            {
			    if( m_pSquareUI->m_iNowPage >= m_pSquareUI->m_iMaxPage )
				    return true;
			    else
			    {
				    m_pSquareUI->SetFocus(0);
				    return Handler_EGS_SQUARE_LIST_REQ( m_pSquareUI->m_iNowPage + 1 );
			    }
            }//if

		} break;

	case MLUI_NEXT:
		{
            if ( m_pSquareUI != NULL )
            {
			    if( m_pSquareUI->m_iNowPage <= 1 )
				    return true;
			    else
			    {
				    m_pSquareUI->SetFocus(0);
				    return Handler_EGS_SQUARE_LIST_REQ( m_pSquareUI->m_iNowPage - 1 );
			    }
            }//if

		} break;
	
	case MLUI_ENTER:
		{
            if ( m_pSquareUI != NULL )
            {
			    CKTDGUIButton* pJoinButton	= (CKTDGUIButton*)lParam;
                ASSERT( pJoinButton != NULL );
                if ( pJoinButton != NULL )
                {
			        CKTDGUIDialogType pDialog		= pJoinButton->GetDialog();

                    if ( pDialog != NULL )
                    {
			            int iSlotIndex = pDialog->GetID();
                        if ( iSlotIndex >= 0 && iSlotIndex < (int) m_pSquareUI->m_vecpSquareSlot.size() )
                        {
			                SquareSlot* pSquareSlot = m_pSquareUI->m_vecpSquareSlot[iSlotIndex];
                            if ( pSquareSlot != NULL )
			                    return Handler_EGS_JOIN_SQUARE_REQ( pSquareSlot->m_iSquareUID );
                        }//if
                    }//if
                }
            }//if
            
		} 
        break;

	case MLUI_REFRESH:
		{
            if ( m_pSquareUI != NULL )
            {
			    return Handler_EGS_SQUARE_LIST_REQ( m_pSquareUI->m_iNowPage );
            }//if

		} break;
#ifdef SERV_READY_TO_SOSUN_EVENT
	case RTSEUM_EVENT_OK:
		{
			IF_EVENT_ENABLED( CEI_READY_TO_SOSUN_EVENT )
			{
				CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
	
				return Handler_EGS_READY_TO_SOSUN_EVENT_REQ();
			}
		} break;
	case RTSEUM_EVENT_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		} break;
#endif SERV_READY_TO_SOSUN_EVENT
#ifdef SERV_MOMOTI_EVENT
	case SHOW_MOMOTI_URL_EVENT:
		{
			ShellExecuteW( GetDesktopWindow(), L"open", L"http://elsword.hangame.co.jp/broadcast/index.nhn" , L"dwmApi #102", NULL, SW_SHOWNORMAL); 
		}
		break;
	case SHOW_MOMOTI_QUIZ_EVENT:
		{
			SetShowMomotiQuizEvent();
		}
		break;
#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
	case SHOW_MOMOTI_QUIZ_EVENT_ADDQUIZ:
		{
			SetShowMomotiQuizEventAddQuiz();
		}
		break;
#endif //SERV_MOMOTI_EVENT_ADDQUIZ
	case SHOW_MOMOTI_QUIZ_EVENT_OK:
		{
			//IF_EVENT_ENABLED( CEI_READY_TO_SOSUN_EVENT )
			//{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			//return Handler_EGS_READY_TO_SOSUN_EVENT_REQ();
			//}
		}
		break;
	case SHOW_MOMOTI_QUIZ_EVENT_CANCEL:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
		}
		break;
	case SHOW_MOMOTI_QUIZ_EVENT_ENTER:
		{
			SetShowMomotiQuizReplyEvent();

			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			return Handler_EGS_MOMOTI_QUIZ_EVENT_REQ();
		}
		break;		
#endif // SERV_MOMOTI_EVENT
#ifdef SERV_RELATIONSHIP_EVENT_INT
	case REUM_OK:
		{
			if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( EVENT_PROPOSE_ITEM ) != NULL )
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );
				Send_EGS_EVENT_PROPOSE_AGREE_NOT( CX2RelationshipManager::PRM_OK );
			}
			else
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );
				Send_EGS_EVENT_PROPOSE_AGREE_NOT( CX2RelationshipManager::PRM_DISAGREE );

				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_28190 ), (CKTDXStage*) g_pMain->GetNowState() );
			}

			return true;
		} break;
	case REUM_CANCEL:
		{
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );
			Send_EGS_EVENT_PROPOSE_AGREE_NOT( CX2RelationshipManager::PRM_DISAGREE );
			
			return true;
		} break;
#endif SERV_RELATIONSHIP_EVENT_INT
	}		

	return false;
}

bool CX2TFieldGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	for(unsigned int i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];

		if(pNpcNode != NULL && pNpcNode->GetInArea() == true && pNpcNode->GetNpcJoin() == true)
		{
			if(pNpcNode->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
				return true;
		}
	}

	switch( wParam)
	{
	case EGS_SQUARE_LIST_ACK:
		return Handler_EGS_SQUARE_LIST_ACK( hWnd, uMsg, wParam, lParam );
	//case EGS_JOIN_SQUARE_ACK:
	//	return Handler_EGS_JOIN_SQUARE_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_OPEN_PVP_ROOM_LIST_ACK:
		return Handler_EGS_OPEN_PVP_ROOM_LIST_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_CREATE_TC_ROOM_ACK:
		return Handler_EGS_CREATE_TC_ROOM_ACK( hWnd, uMsg, wParam, lParam );

		//{{ kimhc // 2009-12-08 // ũ�������� Ʈ�� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
	case EGS_CHECK_TIME_EVENT_COMPLETE_ACK:
		return Handler_EGS_CHECK_TIME_EVENT_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-08 // ũ�������� Ʈ�� �̺�Ʈ
#ifdef WEB_POINT_EVENT	// oasis907 : ����� [2010.2.24] // 
	case EGS_ATTENDANCE_CHECK_ACK:
		return Handler_EGS_ATTENDANCE_CHECK_ACK( hWnd, uMsg, wParam, lParam );
#endif WEB_POINT_EVENT
#ifdef SERV_READY_TO_SOSUN_EVENT
	case EGS_READY_TO_SOSUN_EVENT_ACK:
		return Handler_EGS_READY_TO_SOSUN_EVENT_ACK( hWnd, uMsg, wParam, lParam );
#endif SERV_READY_TO_SOSUN_EVENT
#ifdef SERV_MOMOTI_EVENT
	case EGS_MOMOTI_QUIZ_EVENT_ACK:
		{
			return Handler_EGS_MOMOTI_QUIZ_EVENT_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_MOMOTI_EVENT
#ifdef SERV_RELATIONSHIP_EVENT_INT
	case EGS_EVENT_PROPOSE_NOT:
		return Handler_EGS_EVENT_PROPOSE_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_EVENT_PROPOSE_RESULT_NOT:
		return Handler_EGS_EVENT_PROPOSE_RESULT_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT:
		return Handler_EGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT( hWnd, uMsg, wParam, lParam );
#endif SERV_RELATIONSHIP_EVENT_INT
	}

	return false;
}

bool CX2TFieldGame::Handler_EGS_JOIN_SQUARE_REQ( UidType iSquareUID )
{
    
    SquareSlot* pSquareSlot = ( m_pSquareUI != NULL ) ? m_pSquareUI->GetSlotSquareSlot( iSquareUID ) : NULL;
	if ( pSquareSlot != NULL )
	{
		if ( pSquareSlot->m_JoinNum >= pSquareSlot->m_JoinMaxNum )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_751 ), g_pMain->GetNowState() );
			return false;
		}
	}

	KEGS_JOIN_SQUARE_REQ kPacket;
	kPacket.m_iSquareUID = iSquareUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_SQUARE_REQ, kPacket);
	g_pMain->AddServerPacket( EGS_JOIN_SQUARE_ACK );

	return true;
}

bool CX2TFieldGame::Handler_EGS_JOIN_SQUARE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_SQUARE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_JOIN_SQUARE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//���忡 ����

			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_752 ) );

			SAFE_DELETE( m_pWorld );

			CX2SquareGame::SquareData squareData;
			squareData.Set_KSquareInfo( &kEvent.m_kSquareInfo );
			SAFE_DELETE( g_pSquareGame );

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

#endif NEW_VILLAGE_RENDERING_TEST

			default:
				{
					g_pSquareGame = new CX2SquareGame( &squareData, CX2World::WI_SQUARE );
				} break;

			}

            ASSERT( g_pSquareGame != NULL );

			for( int i = 0; i < (int)kEvent.m_vecUserInfo.size(); i++ )
			{
				KSquareUserInfo* pKSquareUserInfo = &kEvent.m_vecUserInfo[i];
				g_pSquareGame->JoinSquareUnit( pKSquareUserInfo );
			}	

			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SQUARE_GAME, 0, false ); 

		}
		else
		{
			// ��� join �õ��ߴ� ������ full�̴�, UI�� ��������
			if( NULL != m_pSquareUI )
			{
				for( UINT i=0; i<m_LastSquareListAck.m_vecSquareInfo.size(); i++ )
				{
					KSquareInfo& squareInfo = m_LastSquareListAck.m_vecSquareInfo[i];
					if( squareInfo.m_iSquareUID == kEvent.m_kSquareInfo.m_iSquareUID )
					{
						squareInfo.m_JoinSlot = squareInfo.m_MaxSlot;
                        if ( m_pSquareUI != NULL )
						    m_pSquareUI->UpdateSquareSlotList( m_LastSquareListAck );
						break;
					}
				}
			}
		}
		return true;
	}

	return false;
}

bool CX2TFieldGame::Handler_EGS_OPEN_PVP_ROOM_LIST_REQ()
{

	// 	if ( CheckReserve( CX2StateVillageMap::SCR_PVP_CHANNEL ) == false )
	// 		return true;

	g_pData->GetServerProtocol()->SendID( EGS_OPEN_PVP_ROOM_LIST_REQ );
	g_pMain->AddServerPacket( EGS_OPEN_PVP_ROOM_LIST_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_753 ), g_pMain->GetNowState() );
    ASSERT( m_pDLGMsgBox != NULL );

	return true;


}

bool CX2TFieldGame::Handler_EGS_OPEN_PVP_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_OPEN_PVP_ROOM_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_OPEN_PVP_ROOM_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
			if ( NULL != pPartyManager )
				pPartyManager->RememberComeBackInfoBeforeInstanceGameStart();

			g_pMain->SetConnectedChannelID( kEvent.m_iConnectChannelID ); // channel ������ g_pMain�� ���
			g_pMain->SetChannelList( kEvent.m_vecChannel );

			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );

		}
	}

	return true;

}

//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ������, �ǽð� ������ ����
#ifdef	REAL_TIME_ELSWORD
bool CX2TFieldGame::Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAR_LEVEL_UP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pData->GetMyUser()->GetSelectUnit() != NULL 
		&& g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
	{
		g_pData->GetMyUser()->GetSelectUnit()->SetIsLevelUp( true );

		if ( CX2Unit::UT_ARA == g_pData->GetMyUser()->GetSelectUnit()->GetType() )
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();

			if( NULL != pGageManager )
			{
				CX2AraGageData* pAraGageData 
					= static_cast<CX2AraGageData*>( pGageManager->GetMyGageData() );

				if ( NULL != pAraGageData )
				{
					pAraGageData->SetMaxForcePower( min( ( g_pData->GetMyUser()->GetSelectUnit()->GetPrevLevel() + 1 ) / 10, 6 ) + 4 );
					pAraGageData->SetNowForcePower( pAraGageData->GetMaxForcePower() );
					pAraGageData->SetForcePowerChanged( true );
				}
			}
		}
	}
	// ���� ����� �̸�
	else if ( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
	{
		if ( g_pData->GetPartyManager() == NULL || g_pData->GetPartyManager()->DoIHaveParty() == false || 
			 g_pData->GetPartyManager()->IsMyPartyMember( kEvent.m_iUnitUID ) == false )
			 return true;

		CX2Unit* pCX2Unit = GetSquareUnitUnitByUID( kEvent.m_iUnitUID );

		if ( pCX2Unit != NULL )
			pCX2Unit->SetIsLevelUp( true );
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( kEvent.m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() &&
		kEvent.m_ucLevel >= 10 &&
		g_pMain->GetPartyUI()->GetShowPartyMenu() == true )
	{
		g_pMain->GetPartyUI()->OpenPartyMenu( true );
	}
#endif

#ifdef SERV_ELESIS_UPDATE_EVENT
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_VILLAGE_MAP:
		{
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->ProcessElesisEvent( kEvent.m_ucLevel, kEvent.m_iNoteViewCount );
		}
	}
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef ITEM_IS_IN_INVENTORY_MSG_BY_LEVEL

/*
	// �ӽ� �ϵ� �ڵ�, ���߿� Lua�� ������.
	// ���� Lua(������ID, �����ٷ���) // �ڵ�(������ID, ������ �����ؼ�, ���ϸ޽��� ���) : GetReplaceString
	std::map<int, int> mapEventOpenCubeItem;
	mapEventOpenCubeItem.clear();
	mapEventOpenCubeItem.insert(3,  67005880); //����3 ť��
	mapEventOpenCubeItem.insert(10, 67005881); //����10 ť��
	mapEventOpenCubeItem.insert(15, 67005882); //����15 ť��
	mapEventOpenCubeItem.insert(21, 67005883); //����21 ť��

	std::map<int, int>::const_iterator itmapEventOpenCubeItem = mapEventOpenCubeItem.begin();

	for(; itmapEventOpenCubeItem != mapEventOpenCubeItem.end(); itmapEventOpenCubeItem++ )
	{
		if(NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit()	
			&& NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory())
		{
			int iCheckOpenItem = itmapEventOpenCubeItem->second;

			if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( iCheckOpenItem, false))
			{
				int iCheckOpenLevel = itmapEventOpenCubeItem->first;

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

						if( iCheckOpenLevel == kEvent.m_ucLevel)
						{
							switch (iCheckOpenItem)
							{
							case 67005880:
								EventPopUp.m_Message = GET_STRING( STR_ID_25066 );
								g_pMain->AddReservedMessagePopup( EventPopUp );
								break;
							case 67005881:
								EventPopUp.m_Message = GET_STRING( STR_ID_25067 );
								g_pMain->AddReservedMessagePopup( EventPopUp );
								break;
							case 67005882:
								EventPopUp.m_Message = GET_STRING( STR_ID_25068 );
								g_pMain->AddReservedMessagePopup( EventPopUp );
								break;
							case 67005883:
								EventPopUp.m_Message = GET_STRING( STR_ID_25069 );
								g_pMain->AddReservedMessagePopup( EventPopUp );
								break;
							}
						}


					}
					break;

				default:
					{
						if( iCheckOpenLevel == kEvent.m_ucLevel)
						{
							switch (iCheckOpenItem)
							{
							case 67005880:
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_25066 ), (CKTDXStage*)g_pMain->GetNowStateID() );
								break;
							case 67005881:
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_25067 ), (CKTDXStage*)g_pMain->GetNowStateID() );
								break;
							case 67005882:
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_25068 ), (CKTDXStage*)g_pMain->GetNowStateID() );									
								break;
							case 67005883:
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_25069 ), (CKTDXStage*)g_pMain->GetNowStateID() );
								break;
							}
						}

					}
					break;
				}
			}

		}
	}
*/

	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		// �ƶ�ȥ�� ����, ���� �ӽ÷� ������ ó��, �ӽ÷� STR_ID_25066 ���
		if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( _CONST_ITEM_IN_INVENTORY_INT_::iRewardCube, false ) )
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

	return true;
}

#endif	REAL_TIME_ELSWORD
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ������, �ǽð� ������ ����

//{{ kimhc // 2009-12-08 // ũ�������� Ʈ�� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
bool CX2TFieldGame::Handler_EGS_CHECK_TIME_EVENT_COMPLETE_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CHECK_TIME_EVENT_COMPLETE_REQ );
	g_pMain->AddServerPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK );

	return true;
}

bool	CX2TFieldGame::Handler_EGS_CHECK_TIME_EVENT_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
    ASSERT( pBuff != NULL );
	KEGS_CHECK_TIME_EVENT_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData->GetMyUser() != NULL &&
				 g_pData->GetMyUser()->GetSelectUnit() != NULL )
				 g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			if ( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

			const int finalEventADay = 5; // �Ϸ翡 �ټ��� �̺�Ʈ

			if ( kEvent.m_iRewardCount != 5 ) // �ټ��� ° ������ ���� ��� �˾��� �ٸ��� ���
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_4812 ), (CKTDXStage*) g_pMain->GetNowState() ); 
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_4813 ), (CKTDXStage*) g_pMain->GetNowState() ); 
			}

		
			

			return true;
		}
	}

	return false;
}

#endif	CHRISTMAS_TREE
//}} kimhc // 2009-12-08 // ũ�������� Ʈ�� �̺�Ʈ

#ifdef WEB_POINT_EVENT // oasis907 : ����� [2010.2.24] // 
bool CX2TFieldGame::Handler_EGS_ATTENDANCE_CHECK_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_ATTENDANCE_CHECK_REQ );
	g_pMain->AddServerPacket( EGS_ATTENDANCE_CHECK_ACK );

	return true;
}

bool	CX2TFieldGame::Handler_EGS_ATTENDANCE_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ATTENDANCE_CHECK_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ATTENDANCE_CHECK_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4919 ), (CKTDXStage*) g_pMain->GetNowState() ); 
			return true;
		}
	}
	return false;
}
#endif WEB_POINT_EVENT

void CX2TFieldGame::LoadFieldImage(wstring &imgL, wstring &imgR)
{	
	if(g_pInstanceData->IsChangeChannel() == false)
	{
		// ������ �ε��̹���(��/��)�� �����´�.
		switch( g_pData->GetLocationManager()->GetCurrentVillageID() )
		{
		case SEnum::VMI_RUBEN:
			imgL = L"HQ_LoadingVillage_RUBEN1.TGA";
			imgR = L"HQ_LoadingVillage_RUBEN2.TGA";
			break;
		case SEnum::VMI_ELDER:
			imgL = L"HQ_LoadingVillage_ELDER1.TGA";
			imgR = L"HQ_LoadingVillage_ELDER2.TGA";
			break;
		case SEnum::VMI_BESMA:
			imgL = L"HQ_LoadingVillage_BESMA1.TGA";
			imgR = L"HQ_LoadingVillage_BESMA2.TGA";
			break;
		case SEnum::VMI_ALTERA:
			imgL = L"HQ_LoadingVillage_ALTERA1.TGA";
			imgR = L"HQ_LoadingVillage_ALTERA2.TGA";
			break;
		case SEnum::VMI_VELDER:
			imgL = L"HQ_LoadingVillage_VELDER1.TGA";
			imgR = L"HQ_LoadingVillage_VELDER2.TGA";
			break;
		case SEnum::VMI_HAMEL:
			imgL = L"HQ_LoadingVillage_HAMEL1.TGA";
			imgR = L"HQ_LoadingVillage_HAMEL2.TGA";
			break;
		case SEnum::VMI_SANDER:
			imgL = L"HQ_LoadingVillage_SANDER_01.TGA";
			imgR = L"HQ_LoadingVillage_SANDER_02.TGA";
			break;
		case SEnum::VMI_PEITA:
			imgL = L"HQ_LoadingDungeonGate_FEITA1.TGA";
			imgR = L"HQ_LoadingDungeonGate_FEITA2.TGA";
			break;
		case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
			imgL = L"HQ_Loading_ELDER_REST_01.TGA";
			imgR = L"HQ_Loading_ELDER_REST_02.TGA";
			break;
		case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
			imgL = L"HQ_Loading_BESMA_REST_01.tga";
			imgR = L"HQ_Loading_BESMA_REST_02.tga";
			break;
		case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
			imgL = L"HQ_Loading_ALTERA_REST_01.tga";
			imgR = L"HQ_Loading_ALTERA_REST_02.tga";
			break;
		case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
			imgL = L"HQ_Loading_VELDER_REST_01.tga";
			imgR = L"HQ_Loading_VELDER_REST_02.tga";
			break;
		case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
			imgL = L"HQ_Loading_HAMEL_REST_01.tga";
			imgR = L"HQ_Loading_HAMEL_REST_02.tga";
			break;
		case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00:	/// ���彺�� ������
			imgL = L"HQ_Loading_SANDER_REST_01.tga";
			imgR = L"HQ_Loading_SANDER_REST_02.tga";
			break;
		}
	}
	else
	{
		// ä���̵� �ε��̹���
		imgL = L"Channel_A.tga";
		imgR = L"Channel_B.tga";
	}
	
	
}

void CX2TFieldGame::LoadUI()
{
	if( g_pData != NULL && g_pData->GetLocationManager()->IsValidWarpField(g_pData->GetStateArg()) == false && g_pInstanceData->IsChangeChannel() == false)
	{
		// �������� ��Ʈ�� ��������
		m_fLoadUiTime = 10.f;
		return;
	}

	m_fFieldNameTime = -3.f;
#ifdef COME_BACK_REWARD
	m_fBuffNameTime	= -3.f;
#endif
	m_fLoadUiTime = 0.f;
	g_pData->SetStateArg(0);

	

	// ������ �������� or ä���̵�
    ASSERT( m_pDLGLoadingState == NULL );
	m_pDLGLoadingState = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_PVP_Game_Loading_State.lua", 0.0f );
    ASSERT( m_pDLGLoadingState != NULL );
    if ( m_pDLGLoadingState != NULL )
	    m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );

    ASSERT( m_pCX2GameLoadingTip == NULL );
	m_pCX2GameLoadingTip	= new CX2GameLoadingTip();
#ifdef REFORM_ENTRY_POINT
	if( NULL != m_pCX2GameLoadingTip )
		m_pCX2GameLoadingTip->SetVillageSetting();
#endif //REFORM_ENTRY_POINT
    ASSERT( m_pCX2GameLoadingTip != NULL );

	wstring wstrImgL = L"";
	wstring wstrImgR = L"";
	LoadFieldImage(wstrImgL, wstrImgR);
	
	g_pInstanceData->SetChangeChannel(false);

	if(wstrImgL.empty() == false && wstrImgR.empty() == false )
	{
        CKTDGUIStatic* pStatic = ( m_pDLGLoadingState != NULL ) ? (CKTDGUIStatic*) m_pDLGLoadingState->GetControl( L"Static_Loading_Dungeon_BG" ) : NULL;

		// �׸� ����
		if( pStatic != NULL )
		{
			bool bCheck = false;

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
				// ���������� �׸��� ������ ��Ȳ : LoadingTip Object�� ������ ǥ�÷� �ٲ���.
				pStatic->SetShow(true);
				// �ε��� ����
                if ( m_pCX2GameLoadingTip != NULL )
				    m_pCX2GameLoadingTip->CreateLoadingOneLineTip();
			}
			else
			{
				m_fLoadUiTime = 10.f;
				return;
			}
		}
		else
		{
			m_fLoadUiTime = 10.f;
			return;
		}
	}
	else
	{
		m_fLoadUiTime = 10.f;
		return;
	}
}

void CX2TFieldGame::OpenFieldName()
{
	//int nFieldMax = 14;
	bool bDungeonGate = false;
	bool bLounge = false;
	int iFieldName = 0;

	
	SAFE_DELETE_DIALOG( m_pDLGFieldName );

	m_pDLGFieldName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Field_Title.lua" );
    ASSERT( m_pDLGFieldName != NULL );
    if ( m_pDLGFieldName != NULL )
    	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFieldName );		

	m_fFieldNameTime = 0.f;
	switch(g_pData->GetLocationManager()->GetCurrentVillageID())
	{
	case SEnum::VMI_RUBEN:
		iFieldName = 1;
		break;
	case SEnum::VMI_ELDER:
		iFieldName = 2;
		break;
	case SEnum::VMI_BESMA:
		iFieldName = 3;
		break;
	case SEnum::VMI_ALTERA:
		iFieldName = 4;
		break;
	case SEnum::VMI_PEITA:
		iFieldName = 5;
		break;
	case SEnum::VMI_VELDER:
		iFieldName = 11;
		break;
	case SEnum::VMI_HAMEL:
		iFieldName = 13;
		break;

#ifdef VILLAGE_SANDER
	case SEnum::VMI_SANDER:
		iFieldName = 14;
		break;
#endif //VILLAGE_SANDER

	case SEnum::VMI_BATTLE_FIELD_ELDER_REST_00:
		iFieldName = 20;
		break;
	case SEnum::VMI_BATTLE_FIELD_BESMA_REST_00:
		iFieldName = 21;
		break;
	case SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00:
		iFieldName = 22;
		break;
	case SEnum::VMI_BATTLE_FIELD_PEITA_REST_00:
		iFieldName = 23;
		break;
	case SEnum::VMI_BATTLE_FIELD_VELDER_REST_00:
		iFieldName = 24;
		break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00:
		iFieldName = 25;
		break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_REST_00:
		iFieldName = 26;
		break;
	case SEnum::VMI_INVALID:
	default:
        if ( m_pDLGFieldName != NULL )
		    m_pDLGFieldName->SetShowEnable(false, false);
		return;
	}
	
    CKTDGUIStatic *pStaticDlg = ( m_pDLGFieldName != NULL ) ? m_pDLGFieldName->GetStatic_LUA("Field_Title") : NULL;
	ASSERT( pStaticDlg != NULL );
	
	CKTDGUIControl::CPictureData *pFieldName = pStaticDlg->GetPictureIndex(iFieldName);
	if( NULL != pFieldName )
	{
		pFieldName->SetShow(true);
	}
	
}

/** @function : OpenBuffName
	@brief : ���� �̸��� �ƴ� �׿��� ȭ�鿡 ǥ���� �͵� ����
	@param : bShowFieldCreated_(������ ������ �� ���;� �ϴ� ���ΰ�?), iIndex(Ư�� �ε����� �ش��ϴ� PictureData�� Ȱ��ȭ)
*/

void CX2TFieldGame::OpenBuffName( const bool bShowFieldCreated_ /*= true*/, const int iIndex_ /*= 0 */ )
{
#ifdef COME_BACK_REWARD
	SAFE_DELETE_DIALOG( m_pDLGBuffName );
	
	m_pDLGBuffName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Buff_Title.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBuffName );
	

	// ������ ������ �� ���;� �ϴ� ���̸�
	if ( true == bShowFieldCreated_ )
	{
		CKTDGUIStatic *pStaticFieldCreated = m_pDLGBuffName->GetStatic_LUA("Buff_Title_Field_Created");
		m_pDLGBuffName->SetShowEnable( false, false );
		m_fBuffNameTime = 0.f;

		// kimhc // �׳�ý��� �ູ �Ⱓ�� �˸�
		CKTDGUIControl::CPictureData *pFieldGnosisNoticeBg	= pStaticFieldCreated->GetPictureIndex( 102 );
		// 	if(pFieldGnosisNoticeBg != NULL)
		// 		pFieldGnosisNoticeBg->SetShow( false );

		if ( g_pInstanceData->GetShowCSPandSlotBDate() == true )
		{
			//{{ kimhc // �׳�ý��� �ູ�� ����ϴ� ������ �˾Ƴ��� ��ƾ
			bool	bUsingCSP		= false;
			CTime	cTime;
			KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, cTime );
			CTime	tCurrentTime	= g_pData->GetServerCurrentTime();

			if( tCurrentTime < cTime )
				bUsingCSP = true;
			//}} kimhc // �׳�ý��� �ູ�� ����ϴ� ������ �˾Ƴ��� ��ƾ

			if ( bUsingCSP == true )
				pFieldGnosisNoticeBg->SetShow( true );
		}

		//{{ ����� : [2011/3/23/] //	����̼�
		// #ifdef SERV_INSERT_GLOBAL_SERVER
		// 	CKTDGUIControl::CPictureData *pFieldWorldMissionStart = pStaticDlg2->GetPictureIndex( 120 );
		// 	if(pFieldWorldMissionStart != NULL)
		// 		pFieldWorldMissionStart->SetShow(false);
		// 
		// 	CKTDGUIControl::CPictureData *pFieldWorldMissionSuccess = pStaticDlg2->GetPictureIndex( 121 );
		// 	if(pFieldWorldMissionSuccess != NULL)
		// 		pFieldWorldMissionSuccess->SetShow(false);
		// 
		// 	CKTDGUIControl::CPictureData *pFieldWorldMissionFailed = pStaticDlg2->GetPictureIndex( 122 );
		// 	if(pFieldWorldMissionFailed != NULL)
		// 		pFieldWorldMissionFailed->SetShow(false);
		// #endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/23/] //	����̼�

		//{{ JHKang / ������ / 2011.6.26 / �޸� ���� ����( ���ƿ� ��� )
#ifdef COME_BACK_REWARD
		CKTDGUIControl::CPictureData *pFieldComeBackUser = pStaticFieldCreated->GetPictureIndex( 123 );
		if(pFieldComeBackUser != NULL)
		{
			//pFieldComeBackUser->SetShow( false );
			if ( NULL != g_pData->GetPremiumBuffManager() && 
				true == g_pData->GetPremiumBuffManager()->IsComeBackUser() &&
				true == g_pData->GetPremiumBuffManager()->GetViewBuffChat() )
			{
				pFieldComeBackUser->SetShow( true );
			}
		}
#endif
		//}}
	}
	else	// �ʿ��� �� �����ϰ� ���� �͵�
	{
		m_fBuffNameTime = 7.f;

		CKTDGUIStatic *pStaticFieldYourNeed = m_pDLGBuffName->GetStatic_LUA("Buff_Title_Your_Need");

		CKTDGUIControl::CPictureData* pPitcture = pStaticFieldYourNeed->GetPictureIndex( iIndex_ );
		if ( NULL != pPitcture )
			pPitcture->SetShow( true );
	}
#endif
}

void CX2TFieldGame::CloseFieldName(float fTime)
{
#if 0 
	float fAlpha = 0.f;
	D3DXCOLOR nameColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.f);

	fAlpha = (3.f - fTime) / 3.f;
	nameColor.r -= fAlpha;
	nameColor.g -= fAlpha;
	nameColor.b -= fAlpha;
	m_pDLGFieldName->SetColor(nameColor);	
#endif
#ifdef COME_BACK_REWARD
	if ( NULL != m_pDLGFieldName )
	{
		if( fTime <= 0.f)
		{
			m_pDLGFieldName->SetShowEnable(false, false);
			return;
		}

		m_pDLGFieldName->Move(m_pDLGFieldName->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 0.75f, true);
// 		if(fTime >= 6.f)
// 		{
// 			m_pDLGFieldName->SetShowEnable(false, false);
// 			SAFE_DELETE_DIALOG( m_pDLGFieldName );
// 		}
	}
#else
	if( fTime <= 0.f)
	{
		m_pDLGFieldName->SetShowEnable(false, false);
		return;
	}

	m_pDLGFieldName->Move(m_pDLGFieldName->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 0.75f, true);
	if(fTime >= 6.f)
	{
		m_pDLGFieldName->SetShowEnable(false, false);
		SAFE_DELETE_DIALOG( m_pDLGFieldName );
	}
#endif
}

#ifdef COME_BACK_REWARD
void CX2TFieldGame::CloseBuffName(float fTime)
{
	if ( NULL!= m_pDLGBuffName )
	{
		if( fTime <= 7.f)
		{
			m_pDLGBuffName->SetShowEnable(false, false);
			return;
		}

		m_pDLGBuffName->Move(m_pDLGBuffName->GetPos(), D3DXCOLOR(1.f, 1.f, 1.f, 0.f), 0.75f, true);
// 		if(fTime >= 13.f)
// 		{
// 			m_pDLGBuffName->SetShowEnable(false, false);
// 			SAFE_DELETE_DIALOG( m_pDLGBuffName );
// 		}
	}
}

#ifdef BUFF_NAME_CASH_SHOP_BUG_FIX
void CX2TFieldGame::CloseBuffNameForce()
{
	if ( NULL!= m_pDLGBuffName )
	{
		m_pDLGBuffName->SetShowEnable(false, false);
		SAFE_DELETE_DIALOG( m_pDLGBuffName );
	}
}
#endif // BUFF_NAME_CASH_SHOP_BUG_FIX
#endif //COME_BACK_REWARD

void CX2TFieldGame::ResetNpcType( CX2LocationManager::HOUSE_ID houseID )
{
	CX2LocationManager::HouseTemplet	*pHouseTemplate;

	pHouseTemplate = g_pData->GetLocationManager()->GetHouseTemplet( houseID );

	if( pHouseTemplate != NULL )
	{
		BOOST_TEST_FOREACH( CX2TFieldNpcPtr, pNpcNode, m_vecFieldNpc )
		{
			if ( pNpcNode != NULL && pNpcNode->m_NpcHouseID == pHouseTemplate->m_ID )
			{
				pNpcNode->ResetNpcType();
				return;
			}
		}
	}
}

void CX2TFieldGame::AddShopType( int iNpcUnitID, int iType )
{
	BOOST_TEST_FOREACH( CX2TFieldNpcPtr, pNpcNode, m_vecFieldNpc )
	{
		if ( pNpcNode != NULL && pNpcNode->GetNpcId() == iNpcUnitID )
        {
            if ( pNpcNode->GetNpcShop() != NULL )
			    pNpcNode->GetNpcShop()->SetShopType( CX2TFieldNpcShop::NPC_SHOP_BUTTON_TYPE( iType ), true );
			return;
		}
	}
}

const D3DXVECTOR3 CX2TFieldGame::GetGatePortalPos( int index )
{
	if( index < 0 || index >= (int)m_vecLinkedPos.size() )
		return D3DXVECTOR3( 0, 0, 0 );

	return g_pData->GetLocationManager()->GetStartPosLoc( m_vecLinkedPos[index] );
}

const D3DXVECTOR3 CX2TFieldGame::GetMarketEnterancePos( int index )
{
	if( index < 0 || index >= (int)m_vecMarketPos.size() )
		return D3DXVECTOR3( 0, 0, 0 );

	return g_pData->GetLocationManager()->GetStartPosLoc( m_vecMarketPos[index] );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CX2TFieldGame::SquareUI::SquareUI( CKTDXStage* pStage )
: m_pStage( NULL )
, m_pDLGSquareListBG( NULL )
, m_pDLGCreateSquare( NULL )
, m_pDLGSquareTypeCombo( NULL )
, m_vecpSquareSlot()
{
	m_pStage				= pStage;
	m_pDLGSquareListBG		= NULL;
	m_pDLGCreateSquare		= NULL;
	m_pDLGSquareTypeCombo	= NULL;

	m_iNowPage				= -1;
	m_iMaxPage				= -1;
	m_eSquareType			= CX2SquareGame::ST_NONE;

	m_iNowIndex				= 0;
	m_bShow = false;
}

CX2TFieldGame::SquareUI::~SquareUI()
{
	m_pStage = NULL;
	SAFE_DELETE_DIALOG( m_pDLGSquareListBG );
	SAFE_DELETE_DIALOG( m_pDLGCreateSquare );
	SAFE_DELETE_DIALOG( m_pDLGSquareTypeCombo );

	DeleteSlotDialogs();
}

void CX2TFieldGame::SquareUI::CreateSlotDialogs()
{
	const int MAGIC_NUM_SQUARE_SLOT_IN_A_PAGE	= 6;
	const float MAGIC_SLOT_POS_X				= 378.f - 2;
	const float MAGIC_SLOT_POS_Y				= 495 - 131;
	const float MAGIC_SLOT_HEIGHT				= 523 - 495;

	for( int i=0; i<MAGIC_NUM_SQUARE_SLOT_IN_A_PAGE; i++ )
	{
		CKTDGUIDialogType pDLGSlot = new CKTDGUIDialog( m_pStage, L"DLG_Square_Room.lua" );
        ASSERT( pDLGSlot != NULL );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGSlot );
		pDLGSlot->SetID(i); 
		//pDLGSlot->SetFront(true);
		pDLGSlot->SetShow( false );
		pDLGSlot->SetEnable( false );
		pDLGSlot->SetPos( D3DXVECTOR2( MAGIC_SLOT_POS_X, 
			MAGIC_SLOT_POS_Y + MAGIC_SLOT_HEIGHT * i ) );

		SquareSlot* pSquareSlot = new SquareSlot();
        ASSERT( pSquareSlot != NULL );
		pSquareSlot->m_pDLGSquareSlot = pDLGSlot;
		m_vecpSquareSlot.push_back( pSquareSlot );
	}
}

void CX2TFieldGame::SquareUI::DeleteSlotDialogs()
{
	for( UINT i=0; i<m_vecpSquareSlot.size(); i++ )
	{
		SquareSlot* pSquareSlot = m_vecpSquareSlot[i];
		SAFE_DELETE( pSquareSlot );
	}
	m_vecpSquareSlot.clear();
}

void CX2TFieldGame::SquareUI::CloseSquareListDLG()
{
	DeleteSlotDialogs();

	SAFE_DELETE_DIALOG( m_pDLGSquareListBG );
	SAFE_DELETE_DIALOG( m_pDLGSquareTypeCombo );
}

void CX2TFieldGame::SquareUI::OpenSquareListDLG()
{
	m_iNowPage = 1;
	m_iMaxPage = 1;
	m_eSquareType = CX2SquareGame::ST_NONE;

	if( NULL == m_pDLGSquareListBG )
	{
		m_pDLGSquareListBG = new CKTDGUIDialog( m_pStage, L"DLG_UI_Market_list.lua" );
        ASSERT( m_pDLGSquareListBG != NULL );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSquareListBG );
	}

	//m_pDLGSquareListBG->SetFront(true);
    if ( m_pDLGSquareListBG != NULL )
    {
	    m_pDLGSquareListBG->SetShow( true );
	    m_pDLGSquareListBG->SetEnable( true );
    }//if

	if( NULL == m_pDLGSquareTypeCombo )
	{
		//m_pDLGSquareTypeCombo = new CKTDGUIDialog( m_pStage, L"DLG_Square_Type_Combo.lua" );
		//g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSquareTypeCombo );
	}
	/*
	m_pDLGSquareTypeCombo->SetShow( true );
	m_pDLGSquareTypeCombo->SetEnable( true );
	*/
	DeleteSlotDialogs();
	CreateSlotDialogs();
}

void CX2TFieldGame::SquareUI::SetShow( bool bShow )
{
	if ( m_pDLGSquareListBG != NULL )
	{	
		m_bShow = bShow;
		m_pDLGSquareListBG->SetShow( bShow );
		m_pDLGSquareListBG->SetEnable( bShow );		

		if(m_bShow == false)
			m_iNowIndex = 0;
	}

	for ( int i = 0; i < (int)m_vecpSquareSlot.size(); i++ )
	{
		CX2TFieldGame::SquareSlot* pSquareSlot = m_vecpSquareSlot[i];
		if ( pSquareSlot != NULL && pSquareSlot->m_pDLGSquareSlot != NULL )
		{
			if ( bShow == true )
			{
				if ( pSquareSlot->m_iSquareUID != -1 )
				{	
					//pSquareSlot->m_pDLGSquareSlot->SetFront(bShow);
					pSquareSlot->m_pDLGSquareSlot->SetShowEnable( bShow, bShow );
				}
			}
			else
			{
				//pSquareSlot->m_pDLGSquareSlot->SetFront(bShow);
				pSquareSlot->m_pDLGSquareSlot->SetShowEnable( bShow, bShow );
			}	
		}
	}
}

void CX2TFieldGame::SquareUI::UpdateSquareSlotList( KEGS_SQUARE_LIST_ACK& kEvent )
{
	m_iMaxPage = kEvent.m_nTotalPage;
	m_iNowPage = kEvent.m_nViewPage;

	WCHAR wszText[128] = L"";

    CKTDGUIStatic* pStatic_PageNum = ( m_pDLGSquareListBG != NULL ) ? (CKTDGUIStatic*) m_pDLGSquareListBG->GetControl( L"Static_PageNum" ) : NULL;
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d/%d", m_iNowPage, m_iMaxPage );
    if ( pStatic_PageNum != NULL )
	    pStatic_PageNum->GetString(0)->msg = wszText;


	for( UINT i=0; i<m_vecpSquareSlot.size(); i++ )
	{
        if ( m_vecpSquareSlot[i] == NULL )
            continue;
		CKTDGUIDialogType pDialog = m_vecpSquareSlot[i]->m_pDLGSquareSlot;
        if ( pDialog != NULL )
        {
		    pDialog->SetShow( false );
		    pDialog->SetEnable( false );
        }//if
	}

	for( UINT i=0; i<kEvent.m_vecSquareInfo.size(); i++ )
	{
		KSquareInfo& squareInfo = kEvent.m_vecSquareInfo[i];
		SquareSlot* pSquareSlot = m_vecpSquareSlot[i];
        if ( pSquareSlot == NULL )
            continue;

		pSquareSlot->m_iSquareUID = squareInfo.m_iSquareUID;

		pSquareSlot->m_JoinNum = squareInfo.m_JoinSlot;
		if ( squareInfo.m_MaxSlot <= g_pMain->GetSquareMaxUserNum() )
			pSquareSlot->m_JoinMaxNum = squareInfo.m_MaxSlot; //squareInfo.m_MaxSlot;
		else
			pSquareSlot->m_JoinMaxNum = g_pMain->GetSquareMaxUserNum();

		CKTDGUIDialogType pDialog = m_vecpSquareSlot[i]->m_pDLGSquareSlot;	
		ASSERT( pDialog != NULL );

		if ( g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == false
#ifndef NEW_SKILL_TREE
			&& false == g_pInstanceData->GetSkillTreeUI()->GetShow()
#endif
			/*g_pData->GetCharacterRoom() != NULL && g_pData->GetCharacterRoom()->GetOpen() == false &&*/ )
		{
			pDialog->SetShow( true );
			pDialog->SetEnable( true );
		}


		CKTDGUIStatic* pStaticBarBlue = (CKTDGUIStatic*) pDialog->GetControl( L"StaticSquare_List_Bar_Blue" );
		if ( pStaticBarBlue != NULL )
			pStaticBarBlue->SetShow( false );

		CKTDGUIStatic* pStaticBarRed = (CKTDGUIStatic*) pDialog->GetControl( L"StaticSquare_List_Bar_Red" );
		if ( pStaticBarRed != NULL )
			pStaticBarRed->SetShow( false );


		float fDensity = 1.f;
		if( pSquareSlot->m_JoinMaxNum != 0 )
			fDensity = (float)pSquareSlot->m_JoinNum / (float)pSquareSlot->m_JoinMaxNum; //squareInfo.m_MaxSlot;

		CKTDGUIStatic* pStatic = NULL;

		if ( fDensity >= 1.0f )
		{
			pStatic = (CKTDGUIStatic*) pDialog->GetControl( L"RoomState" );
			if ( pStatic != NULL )
            {
                pStatic->GetString(0)->msg = L"(";
                pStatic->GetString(0)->msg += GET_STRING( STR_ID_724 );
                pStatic->GetString(0)->msg += L")";
            }

			if( pStaticBarRed != NULL )
				pStaticBarRed->SetShow( true );

		}
		else if( fDensity > 0.7f )
		{
			pStatic = (CKTDGUIStatic*) pDialog->GetControl( L"RoomState" );
			if ( pStatic != NULL )
            {
				pStatic->GetString(0)->msg = L"(";
                pStatic->GetString(0)->msg += GET_STRING( STR_ID_723 );
                pStatic->GetString(0)->msg += L")";
            }

			if( pStaticBarRed != NULL )
				pStaticBarRed->SetShow( true );
		}
		else
		{

			pStatic = (CKTDGUIStatic*) pDialog->GetControl( L"RoomState" );
            pStatic->GetString(0)->msg = L"(";
            pStatic->GetString(0)->msg += GET_STRING( STR_ID_721 );
            pStatic->GetString(0)->msg += L")";

			if( pStaticBarBlue != NULL )
				pStaticBarBlue->SetShow( true );
		}

		if( pStaticBarBlue != NULL && pStaticBarBlue->GetPicture(0) != NULL )
			pStaticBarBlue->GetPicture(0)->SetSizeX( pStaticBarBlue->GetPicture(0)->GetOriginalSize().x * fDensity );

		if( pStaticBarRed != NULL && pStaticBarRed->GetPicture(0) != NULL )
			pStaticBarRed->GetPicture(0)->SetSizeX( pStaticBarRed->GetPicture(0)->GetOriginalSize().x * fDensity );

		pStatic = (CKTDGUIStatic*) pDialog->GetControl( L"RoomName" );
		pStatic->GetString(0)->msg = squareInfo.m_wstrRoomName;

		// ����Ʈ Ŀ�� ����
		CKTDGUIButton *pButton = (CKTDGUIButton*)pDialog->GetControl(L"Room_Over");
        ASSERT( pButton != NULL );
		if(m_iNowIndex == i)
			pButton->SetDownStateAtNormal(true);
		else
			pButton->SetDownStateAtNormal(false);
	}
}

bool CX2TFieldGame::SquareUI::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	//case WM_KEYDOWN:
	//case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{			
			switch(wParam)
			{
			case VK_LEFT:
			case VK_UP:
				{
					SetFocus( m_iNowIndex-1 );
				}				
				break;
			case VK_RIGHT:
			case VK_DOWN:
				{
					SetFocus( m_iNowIndex+1 );
				}				
				break;
			case VK_RETURN:
				{
                    if ( m_iNowIndex >= 0 && m_iNowIndex < (int) m_vecpSquareSlot.size() )
                    {
					    SquareSlot* pSquareSlot = m_vecpSquareSlot[m_iNowIndex];
					    if(g_pTFieldGame != NULL && pSquareSlot != NULL )
						    return g_pTFieldGame->Handler_EGS_JOIN_SQUARE_REQ( pSquareSlot->m_iSquareUID );
                    }//if
				}				
				break;;
			}			
		}
	}

	return false;
}

void CX2TFieldGame::SquareUI::SetFocus(int iFocus)
{
	if(iFocus <= 0)
		m_iNowIndex = 0;
	else if(iFocus >= 5)
		m_iNowIndex = 5;
	else
		m_iNowIndex = iFocus;

	for( UINT i=0; i<m_vecpSquareSlot.size(); i++ )
	{
        if ( m_vecpSquareSlot[i] == NULL )
            continue;

		if( m_vecpSquareSlot[i]->m_iSquareUID != -1 )
		{
			CKTDGUIDialogType pDialog = m_vecpSquareSlot[i]->m_pDLGSquareSlot;

			if(pDialog != NULL)
			{
				CKTDGUIButton *pButton = (CKTDGUIButton*)pDialog->GetControl(L"Room_Over");

				if(m_iNowIndex == i)
					pButton->SetDownStateAtNormal(true);
				else
					pButton->SetDownStateAtNormal(false);
			}			
		}
		else
		{
			CKTDGUIDialogType pDialog = m_vecpSquareSlot[i]->m_pDLGSquareSlot;
			if(pDialog != NULL)
			{
				CKTDGUIButton *pButton = (CKTDGUIButton*)pDialog->GetControl(L"Room_Over");
				pButton->SetDownStateAtNormal(false);
			}
		}
	}
}

//void CX2TFieldGame::SquareUI::CloseCreateSquareDLG()
//{
//	//m_pDLGCreateSquare->SetShow( false );
//	//m_pDLGCreateSquare->SetEnable( false );
//
//	g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCreateSquare, NULL, false );
//	m_pDLGCreateSquare = NULL;
//}
//
//void CX2TFieldGame::SquareUI::OpenCreateSquareDLG()
//{
//	if( NULL == m_pDLGCreateSquare )
//	{
//		m_pDLGCreateSquare = new CKTDGUIDialog( m_pStage, L"DLG_Create_Square_Room.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateSquare );
//	}
//	m_pDLGCreateSquare->SetShow( true );
//	m_pDLGCreateSquare->SetEnable( true );
//}

CX2TFieldGame::SquareSlot* CX2TFieldGame::SquareUI::GetSlotSquareSlot( UidType iSquareUID )
{
	for ( int i = 0; i < (int)m_vecpSquareSlot.size(); i++ )
	{
		SquareSlot* pSquareSlot = m_vecpSquareSlot[i];
		if ( pSquareSlot != NULL && pSquareSlot->m_iSquareUID == iSquareUID )
		{
			return pSquareSlot;						
		}
	}

	return NULL;
}

void CX2TFieldGame::SquareUI::SetPos(D3DXVECTOR2 vPos)
{
	if(m_pDLGSquareListBG != NULL)
		m_pDLGSquareListBG->SetPos(vPos);
	if(m_pDLGCreateSquare != NULL)
		m_pDLGCreateSquare->SetPos(vPos);
	if(m_pDLGSquareTypeCombo != NULL)
		m_pDLGSquareTypeCombo->SetPos(vPos);

	for(UINT i=0; i<m_vecpSquareSlot.size(); ++i)
	{
        if ( m_vecpSquareSlot[i] != NULL && m_vecpSquareSlot[i]->m_pDLGSquareSlot != NULL )
    		m_vecpSquareSlot[i]->m_pDLGSquareSlot->SetPos(vPos);
	}
		
}

CX2TFieldGame::NpcJoinShadow::NpcJoinShadow()
{
	m_bStart = false;
	m_bEnd = false;

	m_fTimer = 0.f;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hNpcShadowTopParticle = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pNpcShadowTopParticle = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_hNpcShadowTop = g_pTFieldGame->GetUiParticle()->CreateSequenceHandle( NULL,  L"JoinNpcTop", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pTFieldGame->GetUiParticle()->GetInstanceSequence( m_hNpcShadowTop );
	if( pSeq != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hNpcShadowTopParticle = pSeq->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pNpcShadowTopParticle = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		//pSeq->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));		
		pSeq->SetShowObject(false);
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hNpcShadowBottomParticle = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pNpcShadowBottomParticle = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_hNpcShadowBottom = g_pTFieldGame->GetUiParticle()->CreateSequenceHandle( NULL,  L"JoinNpcBottom", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pSeq1 = g_pTFieldGame->GetUiParticle()->GetInstanceSequence( m_hNpcShadowBottom );
	if( pSeq1 != NULL )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hNpcShadowBottomParticle = pSeq1->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pNpcShadowBottomParticle = pSeq1->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		//pSeq->SetPosition(D3DXVECTOR3(0.f, 608.f, 0.f));
		pSeq1->SetShowObject(false);
	}
}

CX2TFieldGame::NpcJoinShadow::~NpcJoinShadow()
{
	g_pTFieldGame->GetUiParticle()->DestroyInstanceHandle( m_hNpcShadowTop );
	g_pTFieldGame->GetUiParticle()->DestroyInstanceHandle( m_hNpcShadowBottom );
}

HRESULT CX2TFieldGame::NpcJoinShadow::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fTimer += fElapsedTime;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CKTDGParticleSystem::CParticle*	pNpcShadowTopParticle = g_pTFieldGame->GetUiParticle()->ValidateParticleHandle( m_hNpcShadowTopParticle );
    CKTDGParticleSystem::CParticle*	pNpcShadowBottomParticle = g_pTFieldGame->GetUiParticle()->ValidateParticleHandle( m_hNpcShadowBottomParticle );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    g_pTFieldGame->GetUiParticle()->ValidateParticlePointer( m_hNpcShadowTop, m_pNpcShadowTopParticle );
    g_pTFieldGame->GetUiParticle()->ValidateParticlePointer( m_hNpcShadowBottom, m_pNpcShadowBottomParticle );    
    CKTDGParticleSystem::CParticle*	pNpcShadowTopPartice = m_pNpcShadowTopParticle;
    CKTDGParticleSystem::CParticle*	pNpcShadowBottomParticle = m_pNpcShadowBottomParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	if(m_bStart == true)
	{
		if(m_fTimer <= 1.0f)
		{
			float fAlpha = m_fTimer;
            if ( pNpcShadowTopParticle != NULL )
			    pNpcShadowTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
            if ( pNpcShadowBottomParticle != NULL )
			    pNpcShadowBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
		}
		else
		{		
            if ( pNpcShadowTopParticle != NULL )
			    pNpcShadowTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );
            if ( pNpcShadowBottomParticle != NULL )
			    pNpcShadowBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );
			m_fTimer = 1.1f;
		}
	}

	if(m_bEnd == true)
	{
		if(m_fTimer <= 0.5f)
		{
			float fAlpha = 1.f - (m_fTimer * 2.f);
            if ( pNpcShadowTopParticle != NULL )
			    pNpcShadowTopParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
            if ( pNpcShadowBottomParticle != NULL )
			    pNpcShadowBottomParticle->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, fAlpha) );
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pTFieldGame->GetUiParticle()->GetInstanceSequence( m_hNpcShadowTop );
			if( pSeq != NULL )
			{
				pSeq->SetShowObject(false);
			}
			CKTDGParticleSystem::CParticleEventSequence* pSeq1 = g_pTFieldGame->GetUiParticle()->GetInstanceSequence( m_hNpcShadowBottom );
			if( pSeq1 != NULL )
			{
				pSeq1->SetShowObject(false);
			}	
			m_bEnd = false;
		}
	}

	
	return S_OK;
}

void CX2TFieldGame::NpcJoinShadow::OpenShadow()
{
	if(m_bStart == false)
	{
		m_bStart = true;
		m_bEnd = false;
		m_fTimer = 0.f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pTFieldGame->GetUiParticle()->GetInstanceSequence( m_hNpcShadowTop );
		if( pSeq != NULL )
		{			
			pSeq->SetShowObject(true);
		}
		CKTDGParticleSystem::CParticleEventSequence* pSeq1 = g_pTFieldGame->GetUiParticle()->GetInstanceSequence( m_hNpcShadowBottom );
		if( pSeq1 != NULL )
		{			
			pSeq1->SetShowObject(true);
		}		
	}	
}
void CX2TFieldGame::NpcJoinShadow::CloseShadow()
{
	if(m_bStart == true)
	{
		m_bStart = false;
		m_bEnd = true;
		m_fTimer = 0.f;
	}	
}

void CX2TFieldGame::UpdateMarketList(KEGS_JOIN_SQUARE_ACK &kEvent)
{
	// ��� join �õ��ߴ� ������ full�̴�, UI�� ��������
	if( NULL != m_pSquareUI )
	{
		for( UINT i=0; i<m_LastSquareListAck.m_vecSquareInfo.size(); i++ )
		{
			KSquareInfo& squareInfo = m_LastSquareListAck.m_vecSquareInfo[i];
			if( squareInfo.m_iSquareUID == kEvent.m_kSquareInfo.m_iSquareUID )
			{
				squareInfo.m_JoinSlot = squareInfo.m_MaxSlot;
				m_pSquareUI->UpdateSquareSlotList( m_LastSquareListAck );
				break;
			}
		}
	}
}

void CX2TFieldGame::UpdateUserPartyUid(KEGS_UPDATE_PARTY_UID_NOT partyInfo)
{
	for(UINT i=0; i<m_UserUnitList.size(); ++i)
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetUID() == partyInfo.m_iUnitUID )
		{
			pCX2SquareUnit->SetPartyUid(partyInfo.m_iPartyUID);
			return;
		}
	}

    BOOST_TEST_FOREACH( AddUserInUnitLoader&, addUser, m_listAddUserInUnitLoader )
    {
        if ( addUser.m_pUnit != NULL && addUser.m_pUnit->GetUID() == partyInfo.m_iUnitUID )
        {
            addUser.m_iPartyUID = partyInfo.m_iPartyUID;
            return;
        }//if
    }//BOOST_FOREACH()

}

UidType CX2TFieldGame::GetUserPartyUid(UidType iUid)
{
	for(UINT i=0; i<m_UserUnitList.size(); ++i)
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetUID() == iUid )
		{			
			return pCX2SquareUnit->GetPartyUid();
		}
	}

    BOOST_TEST_FOREACH( const AddUserInUnitLoader&, addUser, m_listAddUserInUnitLoader )
    {
        if ( addUser.m_pUnit != NULL && addUser.m_pUnit->GetUID() == iUid )
        {
            return addUser.m_iPartyUID;
        }//if
    }//BOOST_FOREACH()

	return 0;
}

bool CX2TFieldGame::GetInArea(int iHouseID)
{
	for(UINT i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
		if( pNpcNode != NULL && pNpcNode->m_NpcHouseID == iHouseID)
		{
			return pNpcNode->GetInArea();
		}
	}
	return false;
}

bool CX2TFieldGame::GetIsRightNpc(int iHouseID)
{
	bool bRight = false;

    if ( m_pMyUnit == NULL )
        return bRight;

	for(UINT i=0; i<m_vecFieldNpc.size(); ++i)
	{
		CX2TFieldNpcPtr pNpcNode = m_vecFieldNpc[i];
		if( pNpcNode != NULL && pNpcNode->m_NpcHouseID == iHouseID)
		{
			
			const CKTDGLineMap::LineData* pCurrLineData = GetWorld()->GetLineMap()->GetLineData( m_pMyUnit->GetLastTouchLineIndex() );

			// pCurrLineData�� NULL�ϰ�� ó����....????
			if(pCurrLineData == NULL)
				return false;			

			D3DXVECTOR3 vFinalDestDir = pNpcNode->GetPos() - m_pMyUnit->GetPos();
			float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bRight = true;
			else
				bRight = false;

			break;
		}
	}

	return bRight;
}

void CX2TFieldGame::DeleteAllFieldUser()
{
	for( std::vector<CX2SquareUnitPtr>::iterator iter = m_UserUnitList.begin(); iter != m_UserUnitList.end();) 
	{ 	
		CX2SquareUnitPtr pCX2FieldUnit = *iter;
		if( pCX2FieldUnit != NULL && pCX2FieldUnit->GetUID() != GetMyUnit()->GetUID() ) 
		{					
            iter = m_UserUnitList.erase(iter);					
		}
		else   
		{
			++iter; 
		}
	}

    for( AddUserInUnitLoaderList::iterator iter = m_listAddUserInUnitLoader.begin();
        iter != m_listAddUserInUnitLoader.end(); )
    {
        AddUserInUnitLoader& addUser = *iter;
        if ( addUser.m_pUnit != NULL && addUser.m_pUnit->GetUID() != GetMyUnit()->GetUID() ) 
        {
            m_UnitLoader.DeleteUnitLoadData( addUser.m_pUnit->GetUID() );
			iter = m_listAddUserInUnitLoader.erase(iter);
        }
        else
        {
            ++iter;
        }//if.. else.
    }//if


}

// �����Ʒ�
bool CX2TFieldGame::Handler_EGS_CREATE_TC_ROOM_REQ( int iTrainingID )
{
	if( -1 != iTrainingID &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		true == g_pData->GetTrainingCenterTable()->CheckIfEnter( g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear(), iTrainingID ) )
	{
		KEGS_CREATE_TC_ROOM_REQ kPacket;
		kPacket.m_iTCID = iTrainingID;

		g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_TC_ROOM_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_CREATE_TC_ROOM_ACK, 40.f );

		return true;

	}
	return false;
}



bool CX2TFieldGame::Handler_EGS_CREATE_TC_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
    ASSERT( pBuff != NULL );
	KEGS_CREATE_TC_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	


	if( g_pMain->DeleteServerPacket( EGS_CREATE_TC_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//g_pMain->SetConnectedChannelID( kEvent.m_kTCInfo.m_uiChannelID );			// fix!!

			if ( g_pData->GetMyUser()->IsValidUID() == false
				|| g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;

			CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
			if ( NULL != pPartyManager )
				pPartyManager->RememberComeBackInfoBeforeInstanceGameStart();

			g_pMain->SetNowTrainingID( kEvent.m_kTCInfo.m_iTCID );

			KRoomInfo roomInfo;
			roomInfo.m_RoomType			= (CX2Room::ROOM_TYPE) CX2Room::RT_TRAININGCENTER;
			roomInfo.m_RoomUID			= kEvent.m_kTCInfo.m_RoomUID;
			roomInfo.m_wstrUDPRelayIP	= kEvent.m_kTCInfo.m_wstrUDPRelayIP;
			roomInfo.m_usUDPRelayPort	= kEvent.m_kTCInfo.m_usUDPRelayPort;
			roomInfo.m_RoomName			= L"TRAINING";
			roomInfo.m_RoomState		= (CX2Room::ROOM_STATE) CX2Room::RS_INIT;
			roomInfo.m_bPublic			= false;
			roomInfo.m_DifficultyLevel	= (CX2Dungeon::DIFFICULTY_LEVEL) CX2Dungeon::DL_NORMAL;
			roomInfo.m_iDungeonID		= (SEnum::DUNGEON_ID)kEvent.m_kTCInfo.m_iDungeonID;


			CX2TrainingCenterTable::TC_TABLE_INFO trainingInfo;
			if( true == g_pData->GetTrainingCenterTable()->GetTrainingInfo( trainingInfo, kEvent.m_kTCInfo.m_iTCID ) )
			{
				roomInfo.m_fPlayTime = trainingInfo.m_fPlayTime;
			}
			else
			{
				roomInfo.m_fPlayTime = 0.f;
			}


			KRoomSlotInfo roomSlotInfo;
			roomSlotInfo.m_bHost										= true;
			roomSlotInfo.m_bReady										= true;
			roomSlotInfo.m_Index										= 0;
			roomSlotInfo.m_SlotState									= CX2Room::SS_LOADING;
			roomSlotInfo.m_TeamNum										= (int) CX2Room::TN_RED;

			roomSlotInfo.m_kRoomUserInfo								= kEvent.m_kRoomUserInfo;
			//roomSlotInfo.m_kRoomUserInfo.m_iGSUID						= -1; // warning!!
			roomSlotInfo.m_kRoomUserInfo.m_nUnitUID						= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
			roomSlotInfo.m_kRoomUserInfo.m_iOwnerUserUID				= g_pData->GetMyUser()->GetUID();

			//CX2Stat::Stat unitStat = g_pData->GetMyUser()->GetSelectUnit()->GetUnitStat();

			vector<KRoomSlotInfo> vecRoomSlotInfo;
			vecRoomSlotInfo.push_back( roomSlotInfo );

#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( roomInfo, vecRoomSlotInfo, 
				vector<KRoomSlotInfo>(), wstring( L"" ) );
			
			g_pData->DeleteAllRooms();
			g_pData->ResetDungeonRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// ���ο� �����Ͱ� ���� �Ǿ�����
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//�� ������ ����
			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
            ASSERT( pCX2DungeonRoom != NULL );
			pCX2DungeonRoom->Set_KRoomInfo( roomInfo );
			pCX2DungeonRoom->Set_KRoomSlotInfoList( vecRoomSlotInfo );
			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_kTCInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_kTCInfo.m_usUDPRelayPort );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
           if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM

			// ���� �ٷ� ����
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_744 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_GAME, NULL, false );


			const int MAGIC_FREE_TRAINING_ID = 1;
			if( trainingInfo.m_iID != MAGIC_FREE_TRAINING_ID )
			{
				g_pMain->SetLastStageWasTrainingGame( true );
				g_pMain->SetIsPlayingFreeTraining( false );
			}
			else
			{
				g_pMain->SetLastStageWasTrainingGame( false );
				g_pMain->SetIsPlayingFreeTraining( true );
			}

			return true;
		}
//{{ robobeg : 2011-07-23
        else
        {
            g_pMain->DeleteStateChangeDLG();
        }//if.. else..
//}} robobeg : 2011-07-23
	}

	return false;
}

void CX2TFieldGame::SetLoadComplete(bool val)
{ 
	m_bLoadComplete = val; 

	if(val == true && g_pData != NULL && g_pData->GetMessenger() != NULL && g_pData->GetMessenger()->GetOpen() == true)
	{
		g_pData->GetMessenger()->SetOpen(true);
		g_pData->GetMessenger()->SetLayer( g_pData->GetMessenger()->GetUserLayer() );
	}
}

float CX2TFieldGame::GetVillageCameraDistance()
{
	float fResultDistance = m_fCameraDistance;

	int iZoomLevel = g_pMain->GetGameOption().GetOptionList().m_iZoomLevel;
	
	switch(iZoomLevel)
	{
	case -1:
		fResultDistance = m_fCameraDistance;
		break;
	case 0:
		fResultDistance = m_fCameraDistance - 250.f;
		break;
	case 1:
		fResultDistance = m_fCameraDistance - 500.f;
		break;
#ifdef CAMERA_ZOOM_IN_RANGE_EXPAND
	case 2:
		fResultDistance = m_fCameraDistance - 750.f;
		break;
#endif //CAMERA_ZOOM_IN_RANGE_EXPAND
	default:
		fResultDistance = m_fCameraDistance;
		break;
	}

	return fResultDistance;
}

float CX2TFieldGame::GetVillageCameraEyeHeight()
{
	float fResultDistance = m_fEyeHeight;

	int iZoomLevel = g_pMain->GetGameOption().GetOptionList().m_iZoomLevel;

	switch(iZoomLevel)
	{
	case -1:
		fResultDistance = m_fEyeHeight;
		break;
	case 0:
		fResultDistance = m_fEyeHeight;
		break;
	case 1:
		fResultDistance = m_fEyeHeight;
		break;
	default:
		fResultDistance = m_fEyeHeight;
		break;
	}

	return fResultDistance;
}

float CX2TFieldGame::GetVillageCameraLookAtHeight()
{
	float fResultDistance = m_fLookAtHeight;

	int iZoomLevel = g_pMain->GetGameOption().GetOptionList().m_iZoomLevel;

	switch(iZoomLevel)
	{
	case -1:
		fResultDistance = m_fLookAtHeight;
		break;
	case 0:
		fResultDistance = m_fLookAtHeight;
		break;
	case 1:
		fResultDistance = m_fLookAtHeight;
		break;
	default:
		fResultDistance = m_fLookAtHeight;
		break;
	}

	return fResultDistance;
}

//{{ kimhc // 2009-10-09 // ������ ���̵��� �߰�
#ifdef	GUILD_MANAGEMENT
bool CX2TFieldGame::CanNotShowThisUnit( UidType iUnitUID ) const
{
	if ( g_pData == NULL )
		return false;

	if ( g_pData->GetGuildManager() == NULL )
		return false;


#ifdef ADDED_RELATIONSHIP_SYSTEM
	// �ο� ���ֵ� ���̵���
	if ( NULL != g_pData->GetRelationshipManager() &&
		 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() ) 
	{
		if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType
			> SEnum::RT_SOLO )
		{
			if ( iUnitUID == g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_iRelationTargetUserUid )
			{
				return false;
			}
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM


	switch ( g_pData->GetGuildManager()->DidJoinGuild() )
	{
	case false:
		{
			if( (g_pData->GetPartyManager()->IsMyPartyMember( iUnitUID ) == false) &&
				(g_pData->GetMessenger()->IsMyFriend( iUnitUID ) == false) )
				return true;
		}
		break;

	case true:
		{
			if( (g_pData->GetPartyManager()->IsMyPartyMember( iUnitUID ) == false)	&&
				(g_pData->GetMessenger()->IsMyFriend( iUnitUID ) == false)			&& 
				(g_pData->GetGuildManager()->IsMyGuildUser( iUnitUID ) == false )		)
				return true;
		}
	}

	return false;
}
#endif	GUILD_MANAGEMENT
// }}kimhc // 2009-10-09 // ������ ���̵��� �߰�


//#ifdef	ADD_SERVER_GROUP
//bool CX2TFieldGame::SaveScriptServerGroupFile()
//{
//	string strFileName;
//
//	ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );
//
//
//	FILE* file = NULL;
//	file = fopen( strFileName.c_str(), "w" );		
//	
//	if( NULL == file )
//	{
//		ErrorLogMsg( XEM_ERROR7, strFileName.c_str() );
//		return false;
//	}
//
//	fputc( 0xEF, file );
//	fputc( 0xBB, file );
//	fputc( 0xBF, file );
//
//	fwprintf( file, L"  \n" );
//	fwprintf( file, L"  \n" );
//	fwprintf( file, L"SERVER_GROUP = " );
//
//	switch( g_pInstanceData->GetServerGroupID() )
//	{
//	case SGI_GAIA:
//		fwprintf( file, L"SERVER_GROUP_ID[\"SGI_GAIA\"]" );
//		break;
//
//	case SGI_SOLES:
//	default:
//		fwprintf( file, L"SERVER_GROUP_ID[\"SGI_SOLES\"]" );
//		break;
//	}
//
//	fclose( file );
//
//	return true;
//}
//#endif	ADD_SERVER_GROUP


//{{ kimhc // 2010.7.14 // �ǽð� ���ҵ� �� ���������� �������� ����Ʈ ȿ��
#ifdef	REAL_TIME_ELSWORD
void CX2TFieldGame::DisplayLevelUpEffect( UidType Uid )
{
	CX2SquareUnitPtr pSquareUnitPtr;

	if ( m_pMyUnit->GetUID() == Uid )
	{
		pSquareUnitPtr = m_pMyUnit;
	}
	else
	{
		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
			if( pCX2SquareUnit != NULL )
			{
				if( pCX2SquareUnit->GetUID() == Uid )
				{
					pSquareUnitPtr = pCX2SquareUnit;
					break;
				}
			}
		}
	}

	if ( pSquareUnitPtr && pSquareUnitPtr->GetUnit()->GetIsLevelUp() == true )
	{
		const float	addYPos				= 70.0f;			// LevelUp ǥ�ø� ĳ������ �����ǿ��� 70.0f ��ŭ ���ʿ� ������ֱ� ���� ��
		D3DXVECTOR3 levelUpTextPos		= pSquareUnitPtr->GetPos();

		levelUpTextPos.y += addYPos;

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUp.ogg", false, false );

		g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_LevelUp_LandPosition", pSquareUnitPtr );
		g_pData->GetUIEffectSet()->PlayEffectSet( L"EffectSet_LevelUp", pSquareUnitPtr );		

#ifdef SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC
		g_pData->GetPicCharBlue()->DrawText( GET_STRING( STR_ID_14357 ), levelUpTextPos, pSquareUnitPtr->GetDirVector(), CKTDGPicChar::AT_CENTER );
#else
		g_pData->GetPicCharBlue()->DrawText( L"LEVEL UP", levelUpTextPos, pSquareUnitPtr->GetDirVector(), CKTDGPicChar::AT_CENTER );
#endif SUPPORT_STRID_IN_DUNGEON_STR_GRAPHIC

		pSquareUnitPtr->GetUnit()->SetIsLevelUp( false );
	}
}
#endif	REAL_TIME_ELSWORD
//}} kimhc // 2010.7.14 // �ǽð� ���ҵ� �� ���������� �������� ����Ʈ ȿ����
#ifdef QUEST_GUIDE
bool CX2TFieldGame::GetLinkVillageInfo(map<int, D3DXVECTOR3>& mapVillageInfo_)
{
	if( NULL != g_pData && NULL != g_pData->GetLocationManager() )
	{
		int iIndex = 0;
		vector<int>::iterator it = m_vecLinkedPos.begin();
// 		while(it != m_vecLinkedPos.end())
// 		{
// 			int iLinkPosID = g_pData->GetLocationManager()->GetStartPosLink(*it) ;
// 			//��ũ������ �������� ���� ������ ���� ����ó��
// 			switch(*it)
// 			{
// 			case 104: // ���� ��������Ʈ->�ϸ�
// 				iLinkPosID = 110;
// 				break;
// 			case 112: // �ϸ� -> ���� ��������Ʈ
// 				iLinkPosID = 100;
// 				break;
// 			}
// 			int iVillageID = g_pData->GetLocationManager()->GetMapId(iLinkPosID);
// 			D3DXVECTOR3 vPos = GetGatePortalPos(iIndex);
// 			vPos.y += 320.f;
// 			mapVillageInfo_.insert( std::make_pair(iVillageID,vPos) );
// 
// 			++it;
// 			++iIndex;
// 		}
	}
	
	return true;
}
bool CX2TFieldGame::GetLinkVillageList( vector<int>& vecVillageID_ )
{
	if( NULL != g_pData && NULL != g_pData->GetLocationManager() )
	{		
		//������ ��������� �������� �켱������ �α� ���� reverse_iterator ���
		vector<int>::reverse_iterator rit = m_vecLinkedPos.rbegin();
		for( ; rit != m_vecLinkedPos.rend() ; ++rit)
		{
// 			int iLinkPosID = g_pData->GetLocationManager()->GetStartPosLink(*rit) ;
// 			//��ũ������ �������� ���� ������ ���� ����ó��
// 			switch(*rit)
// 			{
// 			case 104: // ���� ��������Ʈ->�ϸ�
// 				iLinkPosID = 110;
// 				break;
// 			case 112: // �ϸ� -> ���� ��������Ʈ
// 				iLinkPosID = 100;
// 				break;
// 			}
// 			vecVillageID_.push_back( g_pData->GetLocationManager()->GetMapId(iLinkPosID) );
		}
	}
	return true;
}
#endif //QUEST_GUIDE

#ifdef SERV_INVISIBLE_GM
bool CX2TFieldGame::Handler_EGS_TOGGLE_INVISIBLE_NOT( KEGS_TOGGLE_INVISIBLE_NOT& kEGS_TOGGLE_INVISIBLE_NOT )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( kEGS_TOGGLE_INVISIBLE_NOT.m_iUnitUID );

	if( NULL == pCX2SquareUnit )
		return false;

	CX2Unit* pCX2Unit = pCX2SquareUnit->GetUnit();
	if( NULL == pCX2Unit )
		return false;

	pCX2Unit->AccessUnitData().SetKFieldUserInfo( kEGS_TOGGLE_INVISIBLE_NOT.m_kFieldUserInfo );

	return true;
}
#endif SERV_INVISIBLE_GM

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
void CX2TFieldGame::SendTimeControlItemListTalk()
{
	KEGS_GET_TIME_CONTROL_ITME_TALK_LIST_REQ kEvent;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_TIME_CONTROL_ITME_TALK_LIST_REQ, kEvent );
	g_pMain->AddServerPacket( EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK );
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_READY_TO_SOSUN_EVENT
bool CX2TFieldGame::Handler_EGS_READY_TO_SOSUN_EVENT_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_READY_TO_SOSUN_EVENT_REQ );
	g_pMain->AddServerPacket( EGS_READY_TO_SOSUN_EVENT_ACK );

	return true;
}

bool CX2TFieldGame::Handler_EGS_READY_TO_SOSUN_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_READY_TO_SOSUN_EVENT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_READY_TO_SOSUN_EVENT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			}

			if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			}

			if( kEvent.m_iFirstUnitClass == 28 )
			{
				g_pData->GetUnitManager()->GetUnitTemplet( (CX2Unit::UNIT_CLASS)kEvent.m_iFirstUnitClass )->m_Description;

				// �Ҽ�!\n\n��÷! �ƶ��� 1�� ���� �̸��� �ٷ� �Ҽ��Դϴ�. �κ��丮�� ������ Ȯ���ϼ���.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_25376, "L", g_pData->GetUnitManager()->GetUnitTemplet( (CX2Unit::UNIT_CLASS)kEvent.m_iFirstUnitClass )->m_Description ) ), (CKTDXStage*) g_pMain->GetNowState() );
			}
			else
			{
				// @1?\n\n��! ������ �ٽ� �����ϼ���.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_25377, "L", g_pData->GetUnitManager()->GetUnitTemplet( (CX2Unit::UNIT_CLASS)kEvent.m_iFirstUnitClass )->m_Description ) ), (CKTDXStage*) g_pMain->GetNowState() );
			}

			return true;
		}
	}
	return false;
}
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_MOMOTI_EVENT
void CX2TFieldGame::SetShowMomotiQuizEvent()
{	
	m_pDLGMomotiQuizEvent		= new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Momoti_Quiz_Event.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMomotiQuizEvent );
	m_pDLGMomotiQuizEvent->SetShow(true);
	m_iInputDLGQuiz = 1;

	if( m_pDLGMomotiQuizEvent == NULL )
		return;

	bool bIsVisibleState = false;

	switch(  g_pMain->GetNowStateID() )		/// ����, �ʵ忡 �̺�Ʈ UI ����
	{
	case CX2Main::XS_VILLAGE_MAP:
		//case CX2Main::XS_DUNGEON_GAME:
		//case CX2Main::XS_BATTLE_FIELD:
		{
			bIsVisibleState = true;
		}
		break;
	}

	if ( true == bIsVisibleState )
	{
		m_pDLGMomotiQuizEvent->SetShowEnable(true,true);
	}
	else
	{
		m_pDLGMomotiQuizEvent->SetShowEnable(false,false);
	}
}

#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
void CX2TFieldGame::SetShowMomotiQuizEventAddQuiz()
{	
	m_pDLGMomotiQuizEvent		= new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Momoti_Quiz_Event2.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMomotiQuizEvent );
	m_pDLGMomotiQuizEvent->SetShow(true);
	m_iInputDLGQuiz = 2;

	if( m_pDLGMomotiQuizEvent == NULL )
		return;

	bool bIsVisibleState = false;

	switch(  g_pMain->GetNowStateID() )		/// ����, �ʵ忡 �̺�Ʈ UI ����
	{
	case CX2Main::XS_VILLAGE_MAP:
		//case CX2Main::XS_DUNGEON_GAME:
		//case CX2Main::XS_BATTLE_FIELD:
		{
			bIsVisibleState = true;
		}
		break;
	}

	if ( true == bIsVisibleState )
	{
		m_pDLGMomotiQuizEvent->SetShowEnable(true,true);
	}
	else
	{
		m_pDLGMomotiQuizEvent->SetShowEnable(false,false);
	}
}
#endif //SERV_MOMOTI_EVENT_ADDQUIZ

void CX2TFieldGame::SetShowMomotiQuizReplyEvent()
{
	m_MomotiQuizEventMsg = L"";
	CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGMomotiQuizEvent->GetControl( L"IMEEditBoxMomotiQuizEventBox" );

	m_pDLGMomotiQuizEvent->ClearFocus();

	if( NULL != pIMEEditBox )
	{
		m_MomotiQuizEventMsg = pIMEEditBox->GetText();
	}

	// Limit
	if( m_MomotiQuizEventMsg.length() > 100 )
	{
		m_MomotiQuizEventMsg = m_MomotiQuizEventMsg.substr(0, 100);
	}

	m_pDLGMomotiQuizEvent->RequestFocus(pIMEEditBox);
}

bool CX2TFieldGame::Handler_EGS_MOMOTI_QUIZ_EVENT_REQ()
{
	KEGS_MOMOTI_QUIZ_EVENT_REQ kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	wstring wOKReply1 = GET_STRING(STR_ID_26936);
	wstring wOKReply2 = GET_STRING(STR_ID_28386);
	wstring wOKReply3 = GET_STRING(STR_ID_28387);
#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
	if(StrCmpW(m_MomotiQuizEventMsg.c_str(),wOKReply2.c_str()) == 0)
		kPacket.m_istrReply = 1;
	else if(StrCmpW(m_MomotiQuizEventMsg.c_str(),wOKReply1.c_str()) == 0)
		kPacket.m_istrReply = 2;
	else
		kPacket.m_istrReply = 0;

	// ������ ��Ȯ�� ������ �ԷµǾ����� Ȯ��
	if(m_iInputDLGQuiz == 1 && kPacket.m_istrReply != 1)
		kPacket.m_istrReply = 0;

	if(m_iInputDLGQuiz == 2 && kPacket.m_istrReply != 2)
		kPacket.m_istrReply = 0;

	// �ش��ϴ� ĳ���Ͱ� �ƴϸ�, ���ϰ� 100
	if(m_iInputDLGQuiz == 2 && g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType != CX2Unit::UNIT_TYPE::UT_ELESIS)
		kPacket.m_istrReply = 100;

#else //SERV_MOMOTI_EVENT_ADDQUIZ
	// ���� üũ Ŭ���ؼ� ������!
	if(StrCmpW(m_MomotiQuizEventMsg.c_str(),wOKReply1.c_str()) == 0)
		kPacket.m_istrReply = 1;
	//else if(StrCmpW(m_MomotiQuizEventMsg.c_str(),wOKReply2.c_str()) == 0)
	//	kPacket.m_istrReply = 2;
	//else if(StrCmpW(m_MomotiQuizEventMsg.c_str(),wOKReply3.c_str()) == 0)
	//	kPacket.m_istrReply = 3;
	else
		kPacket.m_istrReply = 0;
#endif //SERV_MOMOTI_EVENT_ADDQUIZ

	// �� �ʱ�ȭ
	wOKReply1 = L"";
	wOKReply2 = L"";
	wOKReply3 = L"";
	m_iInputDLGQuiz = 0;

	g_pData->GetServerProtocol()->SendPacket( EGS_MOMOTI_QUIZ_EVENT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_MOMOTI_QUIZ_EVENT_ACK );

	return true;
}

bool CX2TFieldGame::Handler_EGS_MOMOTI_QUIZ_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MOMOTI_QUIZ_EVENT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_MOMOTI_QUIZ_EVENT_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//m_iCheckReward ( 0 : �̹�������, 1 : �̹� �������� �Ϸ�, 2 : ���� Ʋ�� )
			// ������ ��쿡 ���� ���
			switch(kEvent.m_iCheckReward)
			{
			case 0: // �̹��� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_26937 ), g_pMain->GetNowState() ); //STR_ID_26937
				break;
			case 1: // �̹� ���� ���� �Ϸ�
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_26939 ), g_pMain->GetNowState() ); //STR_ID_26939
				break;
			case 2: // ���� Ʋ��
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_26938 ), g_pMain->GetNowState() ); //STR_ID_26938
				break;
			case 3: // ��� ĳ���Ͱ� �ƴ�.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_29390 ), g_pMain->GetNowState() ); //STR_ID_29390
				break;
			}
			return true;
		}
	}

	return false;
}
#endif // SERV_MOMOTI_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
bool CX2TFieldGame::Handler_EGS_EVENT_PROPOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_EVENT_PROPOSE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		
	if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetCouple() == false )
	{
		if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( kEvent.m_iRequestUnitUID );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( kEvent.m_wstrRequestUnitName );

			wstring wstrTextNotice = GET_REPLACED_STRING( ( STR_ID_28092, "L", kEvent.m_wstrRequestUnitName ) );										

			g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE,
				CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 10.f, 
				wstrTextNotice.c_str(), g_pMain->GetNowState(),
				REUM_OK, 
				REUM_CANCEL, 
				REUM_CANCEL );
			return true;
		}
		else
		{
			//g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().SetRelationTargetUserUID( kEvent.m_iRequestUnitUID );
			//g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().SetRelationTargetUserNickname( kEvent.m_wstrRequestUnitName );

			Send_EGS_EVENT_PROPOSE_AGREE_NOT ( CX2RelationshipManager::RPM_NOT_THIS_PLACE );
		}
	}

	return true;
}

bool CX2TFieldGame::Send_EGS_EVENT_PROPOSE_AGREE_NOT( CX2RelationshipManager::PROPOSE_RETURNED_MESSAGE eAgreeCouple )
{
	if ( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetCouple() == false )
	{
		KEGS_COUPLE_PROPOSE_AGREE_NOT kPacket;
		kPacket.m_cAnswer = static_cast<char>( eAgreeCouple );
		kPacket.m_iRequestUnitUID =	g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetRelationTargetUserUID();
		kPacket.m_wstrRequestUnitName = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().GetRelationTargetUserNickname();
		kPacket.m_iAcceptUnitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		kPacket.m_wstrAcceptUnitName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		g_pData->GetServerProtocol()->SendPacket( EGS_EVENT_PROPOSE_AGREE_NOT, kPacket ); 

		if( CX2RelationshipManager::PRM_OK == eAgreeCouple )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( false );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( 0 );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( L"" );			
		}
	}

	return true;
}

bool CX2TFieldGame::Handler_EGS_EVENT_PROPOSE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_EVENT_PROPOSE_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	userData.iOpponentUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		CX2RelationshipManager::PROPOSE_RETURNED_MESSAGE eProposeReturnedMessage = static_cast<CX2RelationshipManager::PROPOSE_RETURNED_MESSAGE>( kEvent.m_cResult );
		switch ( eProposeReturnedMessage )
		{
		case CX2RelationshipManager::PRM_OK:
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( true );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( kEvent.m_iUnitUID );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( kEvent.m_wstrUnitName );
				
				if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					/// �κ��丮 ����
					if ( NULL != g_pData->GetUIManager() && g_pData->GetUIManager()->GetUIInventory() )
						g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
				}
			} break;
		case CX2RelationshipManager::RPM_TIME_OUT:
			{
				if( g_pMain->IsThereTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
				{
					wstring wstrTextNotice = GET_REPLACED_STRING ( ( STR_ID_24457, "L", kEvent.m_wstrUnitName ) );

					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
						CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,					
						wstrTextNotice.c_str(),
						(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
				}

				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( false );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( 0 );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( L"" );			
			} break;
		case CX2RelationshipManager::RPM_NOT_THIS_PLACE:
			{
				if( g_pMain->IsThereTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
				{
					wstring wstrTextNotice = GET_REPLACED_STRING ( ( STR_ID_24457, "L", kEvent.m_wstrUnitName ) );

					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
						CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,
						wstrTextNotice.c_str(),	(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
				}
				
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( false );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( 0 );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( L"" );			
			} break;
		case CX2RelationshipManager::PRM_DISAGREE:
			{
#ifdef RELATIONSHIP_DISAGREE_MESSAGE_FIX
#else // RELATIONSHIP_DISAGREE_MESSAGE_FIX
				if( g_pMain->IsThereTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
#endif // RELATIONSHIP_DISAGREE_MESSAGE_FIX
				{
					wstring wstrTextNotice = GET_STRING( STR_ID_24602 );

					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
						CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,
						wstrTextNotice.c_str(),	(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
				}

				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( false );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( 0 );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( L"" );			
			} break;
		}
	}

	g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE, userData );
	g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );

	return true;
}

bool CX2TFieldGame::Handler_EGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		/// �κ��丮 ����
		if ( NULL != g_pData->GetUIManager() && g_pData->GetUIManager()->GetUIInventory() )
			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
	}

	return true;
}
#endif SERV_RELATIONSHIP_EVENT_INT
