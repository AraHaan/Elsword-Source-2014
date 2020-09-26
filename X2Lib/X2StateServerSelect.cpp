#include "StdAfx.h"
#ifndef ELSWORD_NEW_BEGINNING
#include ".\x2stateserverselect.h"

bool CX2StateServerSelect::m_sbSelectedServerSet = false;
#ifdef FIX_REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
bool CX2StateServerSelect::m_bCanNotConenctToBusyServer = false;		// �ش� ������ ������ �� �� ���ٴ� �޽��� �ڽ��� ��� ���ΰ�?
#endif // FIX_REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
bool CX2StateServerSelect::m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection = false;
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#ifdef FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
const int TODAY_CREATE_UNIT_MAX_NUMBER = 500;	// �Ϸ翡 ���� ������ ���� �ִ� ���� ( ���̾�, �ַ��� ���� ��� ���� )
#else  // FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
const int TODAY_CREATE_UNIT_MAX_NUMBER = 8;	// �Ϸ翡 ���� ������ ���� �ִ� ���� ( ���̾�, �ַ��� ���� ��� ���� )
#endif // FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED

#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
const float MAX_WAITING_TIME_RECEIVE_CHARACTER_LIST = 5.f;
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����

#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef CLIENT_PURPLE_MODULE	// �ӱԼ� �Ϻ� �߰�
#include "OnlyGlobal/JP/Auth/PurpleForClient.h"
#endif // CLIENT_PURPLE_MODULE

#ifdef SERVER_GROUP_UI_ADVANCED
const int MAX_SERVERGROUP_SIZE = 5;
#endif SERVER_GROUP_UI_ADVANCED

#ifdef CLIENT_PORT_CHANGE_REQUEST
#define UDP_REQUEST_MAX_COUNT 1 // 2013.05.10 lygan_������ // X2State.cpp ���� �Ȱ����� ������ ������ ���� ��������� �Ѵ�.
#endif //CLIENT_PORT_CHANGE_REQUEST

// dmlee 2009.03.11
// ������ �α��� ������ �α��� -> ���Ӽ��� ���� -> �������� -> �ε� 100% -> ���� ����
// ���� �ٲ� �α��� ������ �α��� -> �ε� 100% -> ä�μ��� ���� -> ���Ӽ������� -> ���Ӽ������� -> �������� -> ����

// ���Ӽ��� ���� ���� ��Ŷ ����
// connect_req( nexon passport�� ���Ź޴´�) -> verify_account -> port check -> state change server select req -> current time req -> Handler_EGS_SELECT_SERVER_SET_REQ -> Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ


// ���Ӽ����� ������ �Ǹ� ä�μ��� ������ ���� ���� REQ�� ������. ä�μ����� �����ϱ����� ���Ӽ��� ������ ���� ���� REQ�� ������

#ifdef FIX_REFORM_ENTRY_POINT_6TH		// kimjh, ä�� ��ư ���� �ҽ� ����, ���� ������ �� ������ ���� �� �� �ֵ��� ����
float CHANNEL_CONGESTION_SCOPE_TEMP[] = {0.1f, 0.2f, 0.3f, 0.9f, };		
// �� ��ư�� ����, ���� ���� �� * CHANNEL_CONGESTION_SCOPE_TEMP[0] = ����
// �� ��ư�� ����, ���� ���� �� * CHANNEL_CONGESTION_SCOPE_TEMP[1] = ����
// �� ��ư�� ����, ���� ���� �� * CHANNEL_CONGESTION_SCOPE_TEMP[2] = ȥ��
// �� ��ư�� ����, ���� ���� �� * CHANNEL_CONGESTION_SCOPE_TEMP[3] = FULL
#endif // FIX_REFORM_ENTRY_POINT_6TH	// kimjh, ä�� ��ư ���� �ҽ� ����, ���� ������ �� ������ ���� �� �� �ֵ��� ����

CX2StateServerSelect::CX2StateServerSelect(void) :
m_pDLGServerSelectBack(NULL),
m_pDLGCheckHack(NULL),
//m_vecUnitSlot;
m_pDLGUnitSelectBack(NULL),
m_pDLGUnitSelectFront(NULL),
m_pDLGDeleteUnitCheck(NULL),
m_pDLGLoadingState(NULL),
m_pDLGRenameMsgBox(NULL),
m_pDLGRenameConfirmMsgBox( NULL ),
m_pDLGRenameSuccessMsgBox( NULL ),
m_pDLGTutorialMsgBox(NULL),
m_pDLGGoToFirstDungeon(NULL),
m_pDLGRecommend(NULL),
m_pDLGRecommendSuccess(NULL),
//m_UnitButtonList;
//m_UnitViewerUIList;
m_pSelectUnit(NULL),
m_pSelectButton(NULL),
m_bUnitClicked(false),
m_fMouseSensitivity(0.01f),
m_fRemainRotY(0.0f),
m_bRotateReturn(true),
m_fRotSpeed(false),
m_pOveredButton(NULL),
m_pOveredName(NULL),
m_bOneTimeInit(true),
m_bStateChangingToUnitCreate(false),
m_bPlayDownAnim(false),
m_fDownAnimTime(0.0f),
m_MaxUnitNum(0),
m_bReserveEnterTutorial(false),
m_bCheckRecommend(false),
m_bCheckRecommendSuccess(false),
m_NowPage(1),
m_MaxPage(1),		// �Ʒ��ʿ��� �ٽ� ����
m_bPortCheckWait( false ),
m_fPortCheckWaitTime( 0.f ),
m_PortCheckRetryTime( 0 ),
m_bENX_USER_LOGIN_NOT( false ),
m_bEGS_VERIFY_ACCOUNT_ACK( false ),
m_bIsGameBang( false ),
m_bPortCheckReq( false ),
m_bEGS_VERIFY_ACCOUNT_REQ( false ),
m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime( 0.f ),
#ifdef SERVER_GROUP_UI_ADVANCED
m_bReceivedServerGroupList( false ),
m_pDLGServerGroupSelectBack(NULL),
#endif SERVER_GROUP_UI_ADVANCED
//{{ //2012.02.28 lygan_������ // ��ư�� unitUID�� �����Ͽ� ĳ���� ���� �Ҷ� ����� �뵵
#ifdef SERV_UNIT_WAIT_DELETE
m_iSelectUnitUID( 0 ),
m_bMaintainNowUnitButton( false ),
#endif SERV_UNIT_WAIT_DELETE
//}}
m_bReceivedChannelList( false ),
m_bSentEGS_CONNECT_REQ( false ),
m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( false ),
m_bReservedConnectionToChannelServer( false ), 
m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( false ),
m_iTryConnectChannelID( -1 ), 
m_bJustChangedNickName( false ), 
m_NickNameChangeUnitUID( -1 ),
m_NickNameChangeUnitNickName( L"" ),
m_bRemoveUnitAfterReceiveNMSerialNum( false )
//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
, m_pDLGChannelSelectBack( NULL )
, m_iChannelNum( 0 )
//{{ ����� : [2011/6/20] //        ä�� ���� ������� ����
, m_bSelectChannel( false )
//}} ����� : [2011/6/20] //        ä�� ���� ������� ����
#endif	ADD_SERVER_GROUP
//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef SERV_SECOND_SECURITY
, m_bEnableUnitSelectBySecondSecurity( false )
#endif SERV_SECOND_SECURITY
, m_bRecivedKeyMap( false )
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
, m_pDLGATLLimitCheckMsgBox (NULL)
#endif //SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
, m_pDLGEventGuide (NULL)
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28
, m_hMeshAddAdvertisement_Back( INVALID_MESH_INSTANCE_HANDLE )
, m_hMeshAddAdvertisement_Front( INVALID_MESH_INSTANCE_HANDLE )
#endif // MODIFY_ADVERTISEMENT
#ifdef CLIENT_PORT_CHANGE_REQUEST
, m_iUDPRequestCount( 0 )
#endif //CLIENT_PORT_CHANGE_REQUEST
#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
, m_pDLGInfoCreateJumpingChar ( NULL )		// ���� ĳ���� DLG
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
, m_pUnitViewerUI ( NULL )
, m_iChannelNowPage ( 1 )
, m_iChannelMaxPage ( 1 )
, m_hStandEffect( INVALID_EFFECTSET_HANDLE )
, m_iCreateUnitTodayCount ( 0 )
#ifndef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
, m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection ( false )
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����

, m_fFadeTitleControlValue ( 0.f )
, m_iReservedEntryPointServerChannelIndex ( -1 )
, m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection ( false )
, m_pSelectChannelButton ( NULL )
, m_bIsAbledStateCreateUnitButton ( true )
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
#ifdef ADD_PLAY_SOUND //��â��
, m_pSound( NULL )
#endif //ADD_PLAY_SOUND
#ifdef PLAY_PROMOTION_MOVIE //JHKang
, m_bOrgSound( false )
, m_bOrgMusic( false )
#endif //PLAY_PROMOTION_MOVIE

#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2
, m_pDLGReConnectChannelServer ( NULL )
, m_bReservedSecurityPad ( false )
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
, m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp ( false )
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����

#ifdef FIX_REFORM_ENTRY_POINT_5TH				// ������, ���� ���� ���� 5�� ����
, m_bReservedHackingUserAgreeDlg ( false )
#endif // #ifdef FIX_REFORM_ENTRY_POINT_5TH		// ������, ���� ���� ���� 5�� ����

#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, ���� ���� ����, 8�� ���� ���� 
, m_uidCreatedUnit ( -1 )
#endif // FIX_REFORM_ENTRY_POINT_8TH

#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ���� ���� ����, 10�� ���� ���� 
, m_fElapsedTime_WaitingCharacterList ( 0.f )
#endif // FIX_REFORM_ENTRY_POINT_10TH

{
	m_bReConnectChannelServer = true;
#ifndef NEW_SKILL_TREE
	g_pInstanceData->DeleteSkillTreeUI();
#endif

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	g_pMain->SoundReady( L"UI_MouseClick_01.ogg" );
	g_pMain->SoundReady( L"UI_MouseClick_Keypad.ogg" );
	g_pMain->SoundReady( L"UI_MouseOver_01.ogg" );
	g_pMain->SoundReady( L"UI_PopUp_Negative_01.ogg" );
	g_pMain->SoundReady( L"UI_PopUp_Positive_01.ogg" );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifndef NEW_VILLAGE_UI
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back.lua" );
#else
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#if defined CLIENT_COUNTRY_TW
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_Unit_Select_Back_New_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_Unit_Select_Back_New_HK.lua" );
#else
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_Unit_Select_Back_New.lua" );
#endif
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	m_pDLGUnitSelectBack = new CKTDGUIDialog( this, L"DLG_Unit_Select_Back.lua" );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUnitSelectBack );	

#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28	
	AddMeshAdvertisement_Back();
#endif // MODIFY_ADVERTISEMENT
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#if defined CLIENT_COUNTRY_TW
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection_New_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection_New_HK.lua" );
#else
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection_New.lua" ); 
#endif
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#ifdef NEW_VILLAGE_UI
#ifdef EXPAND_CHANNEL_24
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection_Expansion.lua" ); 
#else
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Server_Chanal_Selection.lua" ); 
#endif //EXPAND_CHANNEL_24
#else
	m_pDLGServerSelectBack = new CKTDGUIDialog( this, L"DLG_Server_Select_Back.lua" );
#endif
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh



	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGServerSelectBack );

	
	wstring wstrClientVersion = g_pMain->GetClientVersion();
#ifdef VERSION_DOT
	wstrClientVersion += L".";
#else VERSION_DOT
	wstrClientVersion += L"_";
#endif VERSION_DOT
	wstrClientVersion += g_pMain->GetSubClientVersion_LUA();
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGServerSelectBack->GetControl( L"Static_Client_Version" );
	if( pStatic != NULL )
	{
		pStatic->GetString(0)->msg = wstrClientVersion;
	}

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	//g_pKTDXApp->GetDGManager()->GetCamera().Point( 0.f, 232.83f, -848.49f, 0, 100.f, 0 );
	
	//g_pMain->SetProjectionFov( 4.f );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

	//GetServerSetDataReq();

#ifdef PLAY_PROMOTION_MOVIE //JHKang
	m_bOrgSound = g_pMain->GetGameOption().GetOptionList().m_bSound;
	m_bOrgMusic = g_pMain->GetGameOption().GetOptionList().m_bMusic;

	g_pMain->GetGameOption().SetSound( false );
	g_pMain->GetGameOption().SetMusic( false );
#endif //PLAY_PROMOTION_MOVIE

#ifdef CHANGE_SERVER_SELECT_BGM
	g_pData->PlayLobbyBGM( L"CharacterSelect.ogg", true );
#else
	g_pData->PlayLobbyBGM( L"Lobby.ogg", true );
#endif //CHANGE_SERVER_SELECT_BGM

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
	//���� ����â

	// fix!! �� �κ� �����ؾ� �մϴ�. �������� GetMyuser�� NULL�Ǵ� ��찡 ��������, �Ÿ��� Ŭ���̾�Ʈ������ ���� ���ӽ� getmyuser�� NULL�� �˴ϴ�.
	if( NULL != g_pData->GetMyUser() &&
		false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
	{
		if( g_pMain->GetNexonLoginMessenger() == true && g_pMain->GetNexonVirtualLogin() == true )
		{
			if ( CNMCOClientObject::GetInstance().LogoutVirtual() == FALSE )
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_699 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_699 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

				

			g_pMain->SetNexonVirtualLogin( false );
		}
	}
#endif // #ifndef NEW_MESSENGER
	if ( g_pData->GetMessenger() != NULL )
	{		
		//g_pData->GetMessenger()->ClearChatSessionList();
		g_pData->GetMessenger()->Reset();
	}

#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP


#ifndef	HEAP_BROKEN_BY_ROOM
	g_pData->DeletePVPRoom();
	g_pData->DeleteDungeonRoom();
	g_pData->DeleteBattleFieldRoom();
#endif // HEAP_BROKEN_BY_ROOM


	if ( g_pMain->GetInformerManager() )
		g_pMain->GetInformerManager()->InvalidInfo();

	g_pMain->GetMemoryHolder()->ReleaseLocalMapDLG();

	//g_pData->DeleteCharacterRoom();
	g_pMain->GetMemoryHolder()->RelaseCommonDLG();
	
#ifdef NEW_VILLAGE_UI
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
#if defined CLIENT_COUNTRY_TW
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back_New_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back_New_HK.lua" );
#else
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back_New.lua" );	
#endif
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Back.lua" );	
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
#else
	m_pDLGUnitSelectFront = new CKTDGUIDialog( this, L"DLG_Unit_Select_Front.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUnitSelectFront );	
	
#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28
	AddMeshAdvertisement_Front();
#endif //MODIFY_ADVERTISEMENT

#ifdef ADD_CREATE_CHARACTER_BUTTON
	//ĳ���� ���� ������ ������, ĳ������ ���ϰ� �ϴ� ���. ���߻系���� �־�� ��.
	if( NULL != m_pDLGUnitSelectFront )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectFront->GetControl(L"createUnit"));
		if( NULL != pStatic )
		{
			pStatic->SetShowEnable(true,true);
		}
	}
#endif //ADD_CREATE_CHARACTER_BUTTON

#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-700, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), false );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	



	if( NULL != g_pData->GetMyUser() )
	{
		m_MaxUnitNum	= g_pData->GetMyUser()->GetUserData().maxUnitCount;

		m_NowPage		= 1;
		const int SLOT_COUNT_FOR_CREATE_UNIT = 1;

		if( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
		{
			m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
		}
		else
		{
			m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
		}
	}

#ifdef FESTIVAL_UI
	if( NULL != g_pMain && NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
	{
		CKTDGParticleSystem::CParticleEventSequenceHandle SeqFestivalUI = g_pMain->GetFestivalSeq();
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != SeqFestivalUI)
		{		
			CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( SeqFestivalUI );
			if( pParticle != NULL )
			{
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( SeqFestivalUI );
				g_pMain->SetFestivalSeq( SeqFestivalUI );
			}
			else
			{
				SeqFestivalUI = INVALID_PARTICLE_SEQUENCE_HANDLE;
				g_pMain->SetFestivalSeq( SeqFestivalUI );
			}
		}
	}
#endif //FESTIVAL_UI

	//CreateUnitButton();

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
	if( 0 != g_pInstanceData->GetPreEventElesisUID() )
	{
		Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ();
	}
	else
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT
	if ( m_sbSelectedServerSet == true )
	{
		ClearBeforeUnit( m_pSelectUnit );
		
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh				
		Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
		Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ ();
		Handler_EGS_CHARACTER_LIST_REQ ();
#else  // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
     	ClearUnitButton();
		CreateUnitButton();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

	
		m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK = true;

#ifdef SERVER_GROUP_UI_ADVANCED
		if(m_pDLGServerGroupSelectBack != NULL)
			m_pDLGServerGroupSelectBack->SetShowEnable(false, false);
#endif SERVER_GROUP_UI_ADVANCED
		m_pDLGServerSelectBack->SetShowEnable(false, false);		
		m_pDLGUnitSelectFront->SetShowEnable(true, true);
#ifdef SERV_SECOND_SECURITY
		// oasis907 : ����� [2011.6.28] ���� ������ �Ǿ� �ִ� ��Ȳ������ 2�� ���� üũ���� �����Ƿ�. 
		m_bEnableUnitSelectBySecondSecurity = true;
#endif SERV_SECOND_SECURITY

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE

	}
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#if defined CLIENT_COUNTRY_TW
	m_pDLGShowBeforeConnect = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Before_Connect_TW.lua" );
#elif defined CLIENT_COUNTRY_HK
	m_pDLGShowBeforeConnect = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Before_Connect_HK.lua" );
#else
	m_pDLGShowBeforeConnect = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Before_Connect.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGShowBeforeConnect );	
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh


#ifdef FIX_REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	if ( true == m_bCanNotConenctToBusyServer )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING ( STR_ID_29885 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
		m_bCanNotConenctToBusyServer = false;
	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	if( NULL != g_pMain->GetTutorSystem() )
	{
		g_pMain->GetTutorSystem()->SetBeenUnitSelect( true );
	}

	if( true == g_pMain->GetIsExitingTutorial() )
	{
		switch( g_pMain->GetStateIDBeforeTutorial() )
		{
		case CX2Main::XS_SERVER_SELECT:
			{
				g_pMain->SetIsExitingTutorial( false );
			} break;
		}
	}

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );
		// ���� �ε� ȭ�� 
		wstringstream wstmStaticName;
		wstmStaticName << L"Static_Loading_BG";
		wstmStaticName << g_pMain->GetTutorialRandomLoadingBG();
		m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );

		m_pSelectUnit = g_pData->GetMyUser()->GetUnitByIndex( g_pData->GetMyUser()->GetUnitNum() - 1 );
		g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
		m_bCheckRecommend = true;
		EnterTutorial();
	}



	g_pData->ResetPartyManager(); // ĳ������â���� ĳ���͸� �ٲٴ� ��쿡 ��Ƽ�Ŵ��� ����

	if( NULL != g_pData->GetMyUser() )
	{
		g_pData->GetMyUser()->SelectUnit( NULL );
	}

	//{{ kimhc // 2009-12-15 // ������ �÷����ߴ� ä�� ������
#ifdef	ADD_SERVER_GROUP
#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
	if ( g_pInstanceData->GetServerGroupID() == SGI_INVALID )
    {
		//OpenScriptServerGroupFile();
        g_pInstanceData->OpenScriptServerGroupFile();
    }
#endif EXTEND_SERVER_GROUP_MASK

#ifdef SERVER_GROUP_UI_ADVANCED
	UpdateServerGroupListUI();
#endif SERVER_GROUP_UI_ADVANCED
	UpdateServerSelectButton( g_pInstanceData->GetServerGroupID() );
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // ������ �÷����ߴ� ä�� ������
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	UpdateChannelListUI();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh


//#ifdef _SERVICE_
//	OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck5" );
//#endif

	//if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_DISAGREE_HACK_USER )
	//{
	//	m_pDLGCheckHack = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
	//	L"ȸ�簡 �������� �ƴ��� ���α׷� ���� ����ϴ� ���, ȸ��� �ش� ���α׷� ���� ������ ������ �� ������ ȸ���� ��� �� ���å�� ���� ���� �̿��� ���ѵ� �� �ֽ��ϴ�. ���� �Ͻðڽ��ϱ�?", 
	//	SSSUCM_HACK_USER_PROCESS_CHECK_OK, this, SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL );
	//}
	//else if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	//{
	//	g_pMain->UpdateProcessList();
	//	g_pMain->SendHackMail();
	//}



	g_pData->GetServerProtocol()->SendTRID( ETR_UNIT_DISCONNECT );



	// ���Ӽ����� �����ߴٰ� �ý��� �޴��� "��������" ��ư�� ������ ���ƿ� ��� ���� ���Ӽ������� ������ �������� �ʾҴ�.
	if( false == m_sbSelectedServerSet && 		
		g_pData->GetServerProtocol()->IsConnected() == true )
	{
		m_bENX_USER_LOGIN_NOT		= false;
		m_bEGS_VERIFY_ACCOUNT_REQ	= false;
		m_bEGS_VERIFY_ACCOUNT_ACK	= false;
		m_bPortCheckReq				= false;

		m_bSentEGS_CONNECT_REQ 		= false;
		m_bReceivedChannelList 		= false;

		// ���Ӽ������� ������ ���´�
		Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
	}


#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	if( false == m_sbSelectedServerSet )
	{
		ConnectToChannelServer();
		SetShowServerStateButtonSet ( false, false );
	}
	else
	{
		SetShowServerStateButtonSet ( true, false );
	}
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	if( false == m_sbSelectedServerSet )
	{
		ConnectToChannelServer();
	}
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	


// ������ title ���� �˻�
// #ifdef _SERVICE_
// 	WCHAR wTextWindow[_MAX_PATH] ;
// 	GetWindowText(g_pKTDXApp->GetHWND(),wTextWindow,_MAX_PATH);
// 	if( StrCmpW( wTextWindow, GET_STRING( STR_ID_1 ) ) != 0 )
// 	{
// 		// ������ Ÿ��Ʋ �����
// 		g_pMain->SendHackMail_DamageHistory(ANTI_HACK_STRING_MODIFY_TITLE  );
// 		//m_pKTDXApp->SetFindHacking( true );
// 	}
// #endif //_SERVIC

// robobeg : CheckProcess �� ���� thread ���� ���� ������ �ʳ�!!!, 2013-12-31
//#if 0 // ��� �׽�Ʈ�� �� �ʿ��� �켱 �����Ѵ�.
//#ifdef CHECKFILTER_IN_MAIN
//	if( g_pInstanceData->GetChangeHackList() == false )
//		g_pMain->GetCheckWindowInfo()->CheckProcess();
//#endif
//#endif


#ifdef BANDICAM_RECORDING
	if( g_pMain->IsCapturing() == true )
	{
		g_pMain->Bandi_EndVidioCapture();
	}
#endif

	g_pKTDXApp->SkipFrame();
	
#ifdef GUIDE_FIRST_EPIC_QUEST
	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
		g_pData->GetPlayGuide()->SetShowFirstEpicQuestGuide(false);
#endif //GUIDE_FIRST_EPIC_QUEST

#ifdef SHOW_ONLY_ADVERTISEMENT
	m_bHideServerSelectUI = false;
	m_pDLGShowOnlyAdvertise = new CKTDGUIDialog( this, L"DLG_ShowOnlyAdvertisement.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGShowOnlyAdvertise );	
#endif // SHOW_ONLY_ADVERTISEMENT


#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	// ���� ���� ���� �˸� â
	m_pDLGInfoCreateJumpingChar = new CKTDGUIDialog( this, L"DLG_UI_JUMPING_EVENT_01.lua" );	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInfoCreateJumpingChar );
	m_pDLGInfoCreateJumpingChar->SetShowEnable( false, false );
	
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
}

CX2StateServerSelect::~CX2StateServerSelect(void)
{
	//{{ kimhc // 2009-12-15 // ������ �÷����ߴ� ä�� ������
#ifdef	ADD_SERVER_GROUP
	SAFE_DELETE_DIALOG( m_pDLGChannelSelectBack );
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // ������ �÷����ߴ� ä�� ������

#ifdef SERVER_GROUP_UI_ADVANCED
	SAFE_DELETE_DIALOG( m_pDLGServerGroupSelectBack );
#endif SERVER_GROUP_UI_ADVANCED
	SAFE_DELETE_DIALOG( m_pDLGServerSelectBack );

	SAFE_DELETE_DIALOG( m_pDLGCheckHack );

	ClearUnitButton();

	SAFE_DELETE_DIALOG( m_pDLGUnitSelectBack );
	SAFE_DELETE_DIALOG( m_pDLGUnitSelectFront );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: ��� ���̾�α� �Ŵ������� �����Ǵ� �ְ� �ƴ϶�~ �䷸��
#endif // DIALOG_HANDLE_TEST // 2009-8-19


	SAFE_DELETE_DIALOG( m_pDLGRenameMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGRenameConfirmMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGRenameSuccessMsgBox );

	SAFE_DELETE_DIALOG( m_pDLGTutorialMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGGoToFirstDungeon );
	SAFE_DELETE_DIALOG( m_pDLGRecommend );
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	SAFE_DELETE_DIALOG( m_pDLGEventGuide );
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE

#ifdef SHOW_ONLY_ADVERTISEMENT
	SAFE_DELETE_DIALOG(m_pDLGShowOnlyAdvertise);
#endif // SHOW_ONLY_ADVERTISEMENT

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	// ���� ���� ���� �˸�â
	SAFE_DELETE_DIALOG ( m_pDLGInfoCreateJumpingChar );
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����

#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
	SAFE_DELETE_DIALOG ( m_pDLGReConnectChannelServer );		
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28
	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshAddAdvertisement_Back )	
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshAddAdvertisement_Back );

	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshAddAdvertisement_Front )
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshAddAdvertisement_Front );
#endif // MODIFY_ADVERTISEMENT

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	SAFE_DELETE_DIALOG ( m_pDLGShowBeforeConnect );

	if ( NULL != g_pData->GetUIMajorXMeshPlayer() )
	{
		BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshInstanceHandle hStandMeshInst, m_vecStandMeshInst )
		{	
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( hStandMeshInst );
		}
		m_vecStandMeshInst.clear();
	}
	if( NULL != g_pData->GetUIEffectSet() )
	{
		g_pData->GetUIEffectSet()->StopEffectSet( m_hStandEffect );
	}
	CX2UnitViewerUI::DeleteKTDGObject( m_pUnitViewerUI );
	m_pUnitViewerUI = NULL;
	
	ClearChannelButton ();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef ADD_PLAY_SOUND //��â��
	if( NULL != m_pSound )
	{
		m_pSound->Stop();
		SAFE_CLOSE( m_pSound );
	}
#endif //ADD_PLAY_SOUND
}

HRESULT CX2StateServerSelect::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2State::OnFrameMove( fTime, fElapsedTime );

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

	OnFrameMove_GameServerConnect( fTime, fElapsedTime );

	if( true == m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK &&
		true == m_bOneTimeInit )
	{
		m_bOneTimeInit = false;

		if( NULL != g_pData->GetMyUser() && 0 == g_pData->GetMyUser()->GetUnitNum() )
		{
			//{{ kimhc // 2009-12-23 // �������� ������ ���� ����� �̵� ����
#ifdef   ADD_SERVER_GROUP
			//{{ ����� : [2011/6/20] //        ä�� ���� ������� ����
			if( false == m_bSelectChannel )
			{
				UnitSelectExit();
				m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK = false;
				m_bOneTimeInit = true;
			}
			else
#endif   ADD_SERVER_GROUP
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ), L"DLG_UI_Selection_MessageBox_No_Button.lua", L"UI_PopUp_Positive_01.ogg"  );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ) );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
				m_bStateChangingToUnitCreate = true;
			}
			//}} ����� : [2011/6/20] //        ä�� ���� ������� ����
			//}} kimhc // 2009-12-23 // �������� ������ ���� ����� �̵� ����

		}
	}
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	UnitHandling( fTime, fElapsedTime );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

	if ( m_pDLGUnitSelectFront != NULL )
	{
		if ( m_sbSelectedServerSet == true )
		{
#ifdef SERVER_GROUP_UI_ADVANCED
			if(m_pDLGServerGroupSelectBack != NULL)
				m_pDLGServerGroupSelectBack->SetShowEnable(false, false);
#endif SERVER_GROUP_UI_ADVANCED
			m_pDLGServerSelectBack->SetShowEnable(false, false);
			m_pDLGUnitSelectFront->SetShowEnable( true, true );
			//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
			m_pDLGChannelSelectBack->SetShowEnable( false, false );
#endif	ADD_SERVER_GROUP
			//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			// ä�ι�ȣ ǥ��
			if( g_pInstanceData != NULL && g_pInstanceData->GetConnectChannelServerID() > 0 )
			{
				CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Channel_Name" );
				if( NULL != pStatic &&
					NULL != pStatic->GetString(0) )
				{
					pStatic->GetString(0)->msg = g_pInstanceData->GetConnectChannelServerName();
				}
			}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef SHOW_ONLY_ADVERTISEMENT
			if( NULL != m_pDLGShowOnlyAdvertise )
				m_pDLGShowOnlyAdvertise->SetShow(false);
#endif //SHOW_ONLY_ADVERTISEMENT
		}
		else
		{
#ifdef SERVER_GROUP_UI_ADVANCED
			if(m_pDLGServerGroupSelectBack != NULL)
				m_pDLGServerGroupSelectBack->SetShowEnable(true, true);
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SHOW_ONLY_ADVERTISEMENT
			if( NULL != m_pDLGShowOnlyAdvertise )
				m_pDLGShowOnlyAdvertise->SetShow(true);

			if( false == m_bHideServerSelectUI )
#endif //SHOW_ONLY_ADVERTISEMENT
			{
				m_pDLGServerSelectBack->SetShowEnable(true, true);
				m_pDLGUnitSelectFront->SetShowEnable( false, false );
				//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
				m_pDLGChannelSelectBack->SetShowEnable( true, true );
#endif	//ADD_SERVER_GROUP

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
				SetShowServerStateButtonSet( false, false );
				SetBackgroundImageBySelectUnitType ( CX2Unit::UT_NONE ); 
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
				//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�

			}
		}

#ifdef PLAY_PROMOTION_MOVIE //JHKang
		CKTDGUIStatic * pMovieStatic = reinterpret_cast<CKTDGUIStatic*>( m_pDLGUnitSelectBack->GetControl( L"PromotionMovie" ) );

		if ( NULL != pMovieStatic )
		{

			CKTDGUIControl::CMovieData* pIntroMovieData = pMovieStatic->GetMovie( 0 );

			if ( NULL != pIntroMovieData )
			{
				if ( g_pMain->m_bPlayIntroMovie == true )
				{
					BVL_STATUS status;
					pIntroMovieData->m_Bvl.GetStatus( status );
					pIntroMovieData->SetLoop( false );

					if ( status == BVL_STATUS_READY )
					{
						pIntroMovieData->SetShow( false );
						pMovieStatic->SetShow( false );
						g_pMain->GetGameOption().SetSound( m_bOrgSound );
						g_pMain->GetGameOption().SetMusic( m_bOrgMusic );
						g_pMain->m_bPlayIntroMovie = false;

#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
	#ifdef FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
						ProcessIntroMovieEnd();		// ��Ʈ�� �������� ������ �� ó������� �� ��
	#else  // FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
						// ���� �е带 ������ ��� ���Ŀ� ��µǵ��� �Ѵ�.
						if ( true == m_bReservedSecurityPad )
						{
							m_bReservedSecurityPad = false;
							g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_GAME_START);
							g_pMain->GetSecurityPad()->SetUseSecondPW(true);
						}
	#endif // FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

					}
#ifdef STOP_INTRO_MOVIE_ON_ERROR
					else if( status == BVL_STATUS_ERROR )
					{
						pIntroMovieData->m_Bvl.Stop();
						pMovieStatic->SetShow( false );
						g_pMain->GetGameOption().SetSound( m_bOrgSound );
						g_pMain->GetGameOption().SetMusic( m_bOrgMusic );
						g_pMain->m_bPlayIntroMovie = false;
					}
#endif //STOP_INTRO_MOVIE_ON_ERROR
				}
				else
				{
					pIntroMovieData->m_Bvl.Stop();
					pMovieStatic->SetShow( false );
					g_pMain->GetGameOption().SetSound( m_bOrgSound );
					g_pMain->GetGameOption().SetMusic( m_bOrgMusic );
				}
			}
		}
#endif //PLAY_PROMOTION_MOVIE
	}

	static bool bAutoStart = true;

	if ( bAutoStart == true && g_pMain->m_bAutoStart == true && NULL != g_pData->GetMyUser() )
	{
		bAutoStart = false;

		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( g_pMain->m_AutoLoginUnitIndex );

		if( pUnit == NULL )
			return true;

		UnitButtonUp( pUnit );


		Handler_EGS_SELECT_UNIT_REQ( pUnit->GetUID(), pUnit->GetUnitData().m_Level );
	}

	//	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );



#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	UnitHandling( fTime, fElapsedTime );
	DownAnimFrameMove( fTime, fElapsedTime );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	

	if ( m_pOveredButton != NULL && m_pOveredName != NULL )
	{
		if ( m_pOveredButton->GetMouseOver() == false )
		{
			if ( m_pOveredButton->GetButtonControlState() != CKTDGUIButton::BCS_MOUSEDOWN )
			{
				//SetShowCharacterName( false );
				m_pOveredName->Move( m_pOveredName->GetOffsetPos(), D3DXCOLOR(0,0,0,0), 0.2f, true, false );
				m_pOveredButton = NULL;
				m_pOveredName = NULL;
			}
		}
	}

	//SetShowCharacterNameForSelectedUnit();


#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP

	if ( g_pData->GetMessenger() != NULL )
		g_pData->GetMessenger()->CheckFriendReq();

#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP



	//if ( m_bCheckPostItem == true )
	//{
	//	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGRecommendReward ) == false &&
	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGTutorReward ) == false &&
	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGGuestUserReward ) == false )
	//	{
	//		m_bCheckPostItem = false;
	//		Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
	//	}
	//}

	if ( m_bCheckRecommendSuccess == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGRecommendSuccess ) == false )
		{
			m_bCheckRecommendSuccess = false;
			//Handler_EGS_GET_POST_LETTER_LIST_REQ();
			Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
		}
	}

	if ( NULL != g_pData->GetMyUser() )
	{
		// ä�� ����â������ ����Ű �������� �ʵ��� ����
		if( false == IsChannelSelectMode() )
		{
#ifdef SERV_KEY_MAPPING_INT
			if( GET_KEY_STATE( GA_LEFT ) == TRUE )
#else // SERV_KEY_MAPPING_INT
			if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_LEFT ) == TRUE )
#endif // SERV_KEY_MAPPING_INT
			{
				if ( m_NowPage > 1 )
				{

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					ClearUnitButton( false );
					m_NowPage--;
					ChangeUnitButtonInfo();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					ClearBeforeUnit( m_pSelectUnit );
					ClearUnitButton();
					m_NowPage--;
					CreateUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����			
				}
			}
#ifdef SERV_KEY_MAPPING_INT
			else if( GET_KEY_STATE( GA_RIGHT ) == TRUE )
#else // SERV_KEY_MAPPING_INT
			else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RIGHT ) == TRUE )
#endif // SERV_KEY_MAPPING_INT
			{
				if ( m_NowPage < m_MaxPage )
				{
					if( (m_NowPage * UNIT_SELECT_UI_MAX_UNIT_NUM) <= g_pData->GetMyUser()->GetUnitNum()+1 )
					{	

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
						ClearUnitButton( false );
						m_NowPage++;
						ChangeUnitButtonInfo();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
						ClearBeforeUnit( m_pSelectUnit );
						ClearUnitButton();
						m_NowPage++;
						CreateUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����					
					}
				}
			}
		}
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	}
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	if ( NULL != m_pDLGUnitSelectBack )
	{
		CKTDGUIStatic * pFadeStatic = NULL;
		if ( NULL != m_pDLGUnitSelectBack->GetControl( L"FadeNotice1" ) && true == m_pDLGUnitSelectBack->GetControl( L"FadeNotice1" )->GetShow() )
		{
			pFadeStatic = static_cast<CKTDGUIStatic *> ( m_pDLGUnitSelectBack->GetControl( L"FadeNotice1" ) );
		}
		if ( NULL != m_pDLGUnitSelectBack->GetControl( L"FadeNotice2" ) && true == m_pDLGUnitSelectBack->GetControl( L"FadeNotice2" )->GetShow() )
		{
			pFadeStatic = static_cast<CKTDGUIStatic *> ( m_pDLGUnitSelectBack->GetControl( L"FadeNotice2" ) );
		}

		if ( NULL != pFadeStatic )
		{
			m_fFadeTitleControlValue += fElapsedTime / 3;

			if ( m_fFadeTitleControlValue > 2.f )
			{
				m_fFadeTitleControlValue = 0.f;
			}
			float m_fFadeTitlePercentage = m_fFadeTitleControlValue;
			if ( m_fFadeTitleControlValue > 1.f ) 
			{
				m_fFadeTitlePercentage = 2.f - m_fFadeTitleControlValue;
			}
			pFadeStatic->SetColor( D3DXCOLOR ( 1.f, 1.f, 1.f, m_fFadeTitlePercentage ) );
		}
		else
		{
			m_fFadeTitleControlValue = 0.f;
		}
	}

	if ( false == m_bIsAbledStateCreateUnitButton )
	{
		if ( true == m_pDLGUnitSelectFront->CheckControl( L"createUnit" ) )
		{
			CKTDGUIButton * pCreateUnitButton = static_cast<CKTDGUIButton *> ( m_pDLGUnitSelectFront->GetControl ( L"createUnit" ) );
			if ( NULL != pCreateUnitButton )
			{
				if ( pCreateUnitButton->GetShow () == true )
					pCreateUnitButton->ChangeState( CKTDGUIButton::BCS_DISABLE, true );
			}
		}
	}


#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
	
	if ( NULL != m_pDLGWaitingCharacterList && true == m_pDLGWaitingCharacterList->GetShow() )
	{
		m_fElapsedTime_WaitingCharacterList += fElapsedTime;
		if ( m_fElapsedTime_WaitingCharacterList >= MAX_WAITING_TIME_RECEIVE_CHARACTER_LIST )
		{		
			m_pDLGReConnectChannelServer = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ),  GET_STRING ( STR_ID_29884 ), 
				SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK, this, SUCM_EXIT, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
			SAFE_DELETE_DIALOG ( m_pDLGWaitingCharacterList );
		}	 
	}
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
	
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

	return S_OK;
}



void CX2StateServerSelect::OnFrameMove_GameServerConnect( double fTime, float fElapsedTime )
{
	if ( m_bPortCheckReq == false && 
		g_pData->GetMyUser() != NULL &&
		m_bENX_USER_LOGIN_NOT == true &&
		m_bEGS_VERIFY_ACCOUNT_ACK == true )
	{
		m_bPortCheckReq = true;


		g_pData->GetMyUser()->SetPCRoom( m_bIsGameBang );

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


		Handler_KXPT_PORT_CHECK_REQ();
	}


	if ( m_bEGS_VERIFY_ACCOUNT_REQ == true && m_bEGS_VERIFY_ACCOUNT_ACK == false )
	{
		m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime += fElapsedTime;
		if ( m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime > 180.f )
		{
			//{{ kimhc // 2010-05-07 // ũ���ð� �ƴ϶� ���� ���� �ϵ��� ����
			// 	 int* x = NULL;	
			// 	 *x = 1;
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
			//}} kimhc // 2010-05-07 // ũ���ð� �ƴ϶� ���� ���� �ϵ��� ����
		}
	}



	if( m_bPortCheckWait == true )
	{
		m_fPortCheckWaitTime -= fElapsedTime;
		if( m_fPortCheckWaitTime < 0.0f )
		{
			m_PortCheckRetryTime++;
			if( m_PortCheckRetryTime < 10 )
			{
				Handler_KXPT_PORT_CHECK_REQ();
			}
			else
			{
				m_bPortCheckWait			= false;
				m_fPortCheckWaitTime		= 0.0f;
				m_PortCheckRetryTime		= 0;

				if( 
#ifdef CLIENT_PORT_CHANGE_REQUEST
					m_iUDPRequestCount < UDP_REQUEST_MAX_COUNT &&
#endif //CLIENT_PORT_CHANGE_REQUEST
					g_pData->GetGameUDP()->IncreaseMyIPIndex() == true )
				{
					Handler_KXPT_PORT_CHECK_REQ();
#ifdef CLIENT_PORT_CHANGE_REQUEST
					m_iUDPRequestCount++;
					if (g_pInstanceData != NULL)
					{
						g_pInstanceData->SetUDPPortSuccessType(1);
					}
#endif //CLIENT_PORT_CHANGE_REQUEST
				}
				else
				{
					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"��Ʈ üũ ������ �����ϴ�", this );
					ErrorLogMsg( XEM_ERROR89, L"PortCheckFail" );

#ifdef CLIENT_PORT_CHANGE_REQUEST
					m_iUDPRequestCount = 0;
					if (g_pInstanceData != NULL)
					{
						g_pInstanceData->SetUDPPortSuccessType(2);
					}
#endif //CLIENT_PORT_CHANGE_REQUEST

					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    kXPT_PORT_CHECK_ACK.m_IPAddress	= g_pData->GetGameUDP()->GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					kXPT_PORT_CHECK_ACK.m_IP	= g_pData->GetGameUDP()->GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kXPT_PORT_CHECK_ACK.m_Port	= g_pData->GetGameUDP()->GetMyPort();

#ifdef CLIENT_PORT_CHANGE_REQUEST
					if ( g_pInstanceData != NULL )
					{
						g_pInstanceData->SetStartPortChangeRequest(true);
					}
#endif //CLIENT_PORT_CHANGE_REQUEST

					Handler_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
				}				
			}
		}		
	}

	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
		switch( pRecvData->m_ID )
		{
		//case XPT_CONNECT_SERVER_ACK:
		//	{
		//		//if( m_bLoadBalancerWait == true )
		//		//{
		//		//	KSerBuffer ksBuff;
		//		//	ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
		//		//	KXPT_CONNECT_SERVER_ACK kXPT_CONNECT_SERVER_ACK;
		//		//	DeSerialize( &ksBuff, &kXPT_CONNECT_SERVER_ACK );
		//		//	Handler_KXPT_CONNECT_SERVER_ACK( &kXPT_CONNECT_SERVER_ACK );
		//		//	
		//		//}
		//	}
		//	break;

		case XPT_PORT_CHECK_ACK:
			{
				if( m_bPortCheckWait == true )
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    if ( pRecvData->m_Size == sizeof(KXPT_PORT_CHECK_ACK) )
                    {
                        Handler_KXPT_PORT_CHECK_ACK( *( (const KXPT_PORT_CHECK_ACK*) pRecvData->m_pRecvBuffer ) );
                    }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					KSerBuffer ksBuff;
//					ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//					DeSerialize( &ksBuff, &kXPT_PORT_CHECK_ACK );
//					Handler_KXPT_PORT_CHECK_ACK( kXPT_PORT_CHECK_ACK );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}
			}
			break;
		}

		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();
	}


}

HRESULT CX2StateServerSelect::OnFrameRender()
{
	if( true == m_bStateChangingToUnitCreate )
	{
		return S_OK;
	}
	
	CX2State::OnFrameRender();

	return S_OK;
}


bool CX2StateServerSelect::EnterTutorial()
{
	if( NULL != m_pSelectUnit )
	{
		m_bReserveEnterTutorial = true;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh

		m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection = true;
		m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection = true;
		m_bENX_USER_LOGIN_NOT		= false;
		m_bEGS_VERIFY_ACCOUNT_REQ	= false;
		m_bEGS_VERIFY_ACCOUNT_ACK	= false;
		m_bPortCheckReq				= false;

		m_bSentEGS_CONNECT_REQ 		= false;
		m_bReceivedChannelList 		= false;
		m_iReservedEntryPointServerChannelIndex = -2;
	
		Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );	
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

		return true;
	}

	return false;
}


bool CX2StateServerSelect::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef SERV_SECOND_SECURITY
	if( NULL != g_pMain->GetSecurityPad() )
	{
		if( true == g_pMain->GetSecurityPad()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif SERV_SECOND_SECURITY

#ifdef COME_BACK_REWARD
	if ( NULL != g_pData->GetPremiumBuffManager() )
	{
		if ( true == g_pData->GetPremiumBuffManager()->UICustomEventProc(hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif

#ifdef PLAY_PROMOTION_MOVIE //JHKang
	if ( g_pMain != NULL && g_pMain->m_bPlayIntroMovie == true )
		return true;
#endif //PLAY_PROMOTION_MOVIE

	switch(wParam)
	{
		case SUSUCM_UNIT_BUTTON_OVER:
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				return false;
				//return AnimateCharacterName( pButton );				
			}
			break;

		case SUSUCM_UNIT_BUTTON_UP:
			{

#ifdef SERV_SECOND_SECURITY
				if(m_bEnableUnitSelectBySecondSecurity == false)
				{
					return true;
				}
#endif SERV_SECOND_SECURITY



				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;

				if ( NULL == pButton )
					break;

				
#ifdef FIX_BUTTON_CRASH //JHKang
				for ( UINT i = 0; i < m_vecUnitSlot.size(); ++i )
				{
					CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];

					if ( pUnitSlot == NULL )
						break;

					CKTDGUIButton* pCharButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 0 ) );

					if ( NULL != pCharButton )
						pCharButton->SetDownStateAtNormal( false );
				}

				pButton->SetDownStateAtNormal( true );
#else //FIX_BUTTON_CRASH
				pButton->SetDownStateAtNormal( true );

				if ( m_pSelectButton != NULL )
					m_pSelectButton->SetDownStateAtNormal( false );

				if ( m_pSelectButton != pButton )
				{
					m_pSelectButton = pButton;
					m_pSelectButton->SetDownStateAtNormal( true );
				}
#endif //FIX_BUTTON_CRASH

				wstring tempButtonName = pButton->GetName();
				tempButtonName = tempButtonName.substr( 12 );
				UidType unitUID = _wtoi64( tempButtonName.c_str() ); 
				CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( unitUID );


#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				
				// if ( )
				if( pUnit == NULL )
					return true;

				UnitButtonUp( pUnit );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				if( pUnit == NULL )
					return true;

				UnitButtonUp( pUnit );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

				//SetShowCharacterName( false );

				//SetShowCharacterNameForSelectedUnit();

			}
			break;

		case SUSUCM_BACK_TO_LOGIN:
			{
				
			}
			break;

		case SUSUCM_CREATE_UNIT:

#ifdef SERV_SECOND_SECURITY
			if(m_bEnableUnitSelectBySecondSecurity == false)
			{
				return true;
			}
#endif SERV_SECOND_SECURITY

			if ( m_sbSelectedServerSet == false )
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_700 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_700 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				return true;
			}
#ifdef FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
			if ( 1 )
#else  // FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
			if ( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
#endif // FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ), L"DLG_UI_Selection_MessageBox_No_Button.lua", L"UI_PopUp_Positive_01.ogg"  );
				//Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ) );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����


				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
			}
			else
			{

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_702 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_702 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����					
				//}
			}
			break;	

		case SUSUCM_SELECT_UNIT:
			{

#ifdef SERV_SECOND_SECURITY
				if(m_bEnableUnitSelectBySecondSecurity == false)
				{
					return true;
				}
#endif SERV_SECOND_SECURITY

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����

				if ( NULL != m_pSelectChannelButton )
				{
#ifdef FIX_REFORM_ENTRY_POINT
					vector< KChannelInfo > * vecChannelInfo = NULL;
					if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
					{
						vecChannelInfo = &m_vecGaiaChannelInfo;
					}
					else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
					{
						vecChannelInfo = &m_vecSolesChannelInfo;
					}
					else 
					{
						ASSERT (!L"Can't Find Server Group ID !! ");
					}
			
					int iCheckEntryPointServerChannelIndex = m_pSelectChannelButton->GetDummyInt( 0 );
					
					if ( (*vecChannelInfo)[ iCheckEntryPointServerChannelIndex ].m_iCurrentUser >= (*vecChannelInfo)[ iCheckEntryPointServerChannelIndex ].m_iMaxUser )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING ( STR_ID_29885 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
					}

					else
#endif // FIX_REFORM_ENTRY_POINT
					{
						m_iReservedEntryPointServerChannelIndex = m_pSelectChannelButton->GetDummyInt( 0 );
						m_bReserveEnterTutorial = false;
						m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection = true;
						m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection = true;
						m_bENX_USER_LOGIN_NOT		= false;
						m_bEGS_VERIFY_ACCOUNT_REQ	= false;
						m_bEGS_VERIFY_ACCOUNT_ACK	= false;
						m_bPortCheckReq				= false;

						m_bSentEGS_CONNECT_REQ 		= false;
						m_bReceivedChannelList 		= false;
						
						// ���Ӽ������� ������ ���´�					
						Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
					}

				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING(STR_ID_29403), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
				}
				
#else  // REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				if( NULL != m_pSelectUnit )
				{
					m_bReserveEnterTutorial = false;
					Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );
				}
				return true;
#endif // REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			}

		
			break;
		case SUSUCM_DELETE_UNIT:
			{	
				if ( m_pSelectUnit != NULL )
				{
					//DeleteUnitReq();

#ifdef SERV_UNIT_WAIT_DELETE
					wstring tempNickName = m_pSelectUnit->GetNickName();
					if(tempNickName.length() != 0 )
					{
						if ( tempNickName.find( L"_") != -1 )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 275), GET_STRING( STR_ID_713 ), this, -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2 ( 0, -130 ),  L"UI_PopUp_Negative_01.ogg" );
							return false;
						}
						else if( m_pSelectUnit->GetUnitData().m_bDeleted == true )		// ���� ��� ����
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 275), GET_STRING( STR_ID_30402 ), this, -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2 ( 0, -130 ),  L"UI_PopUp_Negative_01.ogg" );
							return false;
						}
						else
						{
							m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), L"", SUSUCM_DELETE_UNIT_CHECK, this, -1, L"DLG_UI_Character_Selection_Delete_Unit_New.lua" );
						}
					}
#else SERV_UNIT_WAIT_DELETE
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), L"", SUSUCM_DELETE_UNIT_CHECK, this, -1, L"DLG_UI_Character_Selection_Delete_Unit_New.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					wstring tempEx = GET_REPLACED_STRING( ( STR_ID_703, "SI", m_pSelectUnit->GetNickName(), KHanSungPostWordUnicodeWrapper( (WCHAR*) m_pSelectUnit->GetNickName(), STR_ID_198, STR_ID_199 ) ) );
					m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), tempEx.c_str(), SUSUCM_DELETE_UNIT_CHECK ,this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
#endif SERV_UNIT_WAIT_DELETE
				}
				else
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_704 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_704 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				}

			}

			break;

		case SUSUCM_DELETE_UNIT_CHECK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGDeleteUnitCheck, NULL, false );
				m_pDLGDeleteUnitCheck = NULL;
				
#ifdef REMOVE_USER_ONLY_IF_REMOVED_NM_VIRTUAL_USER
				if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
				{
					return Handler_EGS_DELETE_UNIT_REQ();
				}
				else
				{
					m_bRemoveUnitAfterReceiveNMSerialNum = true;
					return Handler_EGS_GET_MY_MESSENGER_SN_REQ( m_pSelectUnit->GetUID() );
				}
#else REMOVE_USER_ONLY_IF_REMOVED_NM_VIRTUAL_USER
				//{{AFX
				return Handler_EGS_DELETE_UNIT_REQ();
				//}}AFX
#endif REMOVE_USER_ONLY_IF_REMOVED_NM_VIRTUAL_USER
				
			}
			break;

			//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
		case SUSUCM_FINAL_DELETE_UNIT:
			{
				if( m_pSelectUnit == NULL )
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_704 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_704 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					return true;
				}
				
				wstring tempEx = GET_REPLACED_STRING( ( STR_ID_16107, "SI", m_pSelectUnit->GetNickName(), KHanSungPostWordUnicodeWrapper( (WCHAR*) m_pSelectUnit->GetNickName(), STR_ID_198, STR_ID_199 ) ) );
#ifdef REFORM_ENTRY_POINT
				m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ), tempEx.c_str(), SUSUCM_FINAL_DELETE_UNIT_CHECK ,this, -1, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
#else
				m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), tempEx.c_str(), SUSUCM_FINAL_DELETE_UNIT_CHECK ,this );
#endif REFORM_ENTRY_POINT
			} 
			break;
		case SUSUCM_FINAL_DELETE_UNIT_CHECK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGDeleteUnitCheck, NULL, false );
				m_pDLGDeleteUnitCheck = NULL;

				return Handler_EGS_FINAL_DELETE_UNIT_REQ();
			}
			break;
		case SUSUCM_RESTORE_UNIT:
			{	
				if( m_pSelectUnit == NULL )
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_705 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_705 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					return true;
				}
								
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_16106 ), SUSUCM_RESTORE_UNIT_CHECK, this, -1, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				wstring tempEx = GET_REPLACED_STRING( ( STR_ID_703, "SI", m_pSelectUnit->GetNickName(), KHanSungPostWordUnicodeWrapper( (WCHAR*) m_pSelectUnit->GetNickName(), STR_ID_198, STR_ID_199 ) ) );
				m_pDLGDeleteUnitCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), tempEx.c_str(), SUSUCM_RESTORE_UNIT_CHECK ,this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			}
			break;
		case SUSUCM_RESTORE_UNIT_CHECK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGDeleteUnitCheck, NULL, false );
				m_pDLGDeleteUnitCheck = NULL;

				return Handler_EGS_RESTORE_UNIT_REQ();
			}
			break;
#endif SERV_UNIT_WAIT_DELETE
			//}}

		case SUSUCM_UNIT_ANIMATION_DOWN:
			m_bUnitClicked = true;
			break;

		case SUSUCM_UNIT_ANIMATION_UP:
			{
				m_bUnitClicked = false;
			}
			break;

		case SUSUCM_UNIT_ANIMATION_OVER:
			break;

		case SUSUCM_TUTORIAL:
			{
				if( NULL != m_pSelectUnit )
				{
					g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
					EnterTutorial();
				}
				else
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_705 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_705 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					
				}
			}
			break;

		case SUSUCM_RENAME_OK:
			{
				if( m_pSelectUnit != NULL && NULL != m_pDLGRenameMsgBox )
				{
					CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pDLGRenameMsgBox->GetControl( L"IME_NewNick" );
					if( NULL != pEditBox )
					{
						m_NickNameChangeUnitUID = m_pSelectUnit->GetUID();
						m_NickNameChangeUnitNickName = pEditBox->GetText();

						wstring wstrErrorMessage	= L"";
						if( true == CX2Main::IsInvalidNickName( m_NickNameChangeUnitNickName, wstrErrorMessage ) )
						{
							SetRenameMsgBoxError( wstrErrorMessage );
							return true;
						}
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
						Handler_EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ ( m_NickNameChangeUnitUID, m_NickNameChangeUnitNickName.c_str(), true, m_pSelectUnit->GetServerGroupID() );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
						Handler_EGS_CHANGE_NICK_NAME_REQ( m_NickNameChangeUnitUID, m_NickNameChangeUnitNickName.c_str(), true );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					}
				}
				return true;

			} break;

		case SUSUCM_RENAME_CANCEL:
			{
				OpenRenameMsgBox( false );
				return true;
			} break;

		case SUSUCM_RENAME_CONFIRM_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRenameConfirmMsgBox, NULL, false );
				m_pDLGRenameConfirmMsgBox = NULL;

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				Handler_EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ ( m_NickNameChangeUnitUID, m_NickNameChangeUnitNickName.c_str(), false, m_pSelectUnit->GetServerGroupID() );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				Handler_EGS_CHANGE_NICK_NAME_REQ( m_NickNameChangeUnitUID, m_NickNameChangeUnitNickName.c_str(), false );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				return true;
			} break;


		case SUSUCM_RENAME_CONFIRM_CANCEL:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRenameConfirmMsgBox, NULL, false );
				m_pDLGRenameConfirmMsgBox = NULL;
				return true;
			} break;


		case SUSUCM_RENAME_SUCCESS_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRenameSuccessMsgBox, NULL, false );
				m_pDLGRenameSuccessMsgBox = NULL;

				
				m_bJustChangedNickName = true;

				const int MAGIC_UNIT_LEVEL = -1;

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
				Handler_EGS_CHARACTER_LIST_REQ ();
#else  // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
				Handler_EGS_SELECT_UNIT_REQ( m_NickNameChangeUnitUID, MAGIC_UNIT_LEVEL );

#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
				return true;
			} break;

		case SUSUCM_START_TUTORIAL_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGTutorialMsgBox, NULL, false );
				m_pDLGTutorialMsgBox = NULL;


				g_pMain->SetStateIDBeforeTutorial( g_pMain->GetNowStateID() );
				EnterTutorial();
				return true;
			} break;

		case SUSUCM_START_TUTORIAL_CANCEL:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGTutorialMsgBox, NULL, false );
				m_pDLGTutorialMsgBox = NULL;

				g_pMain->SetIsPlayingTutorial( false );

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				m_pDLGGoToFirstDungeon = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_706 ), 
					SUSUCM_START_FIRST_DUNGEON_OK, this, SUSUCM_START_FIRST_DUNGEON_CANCEL, L"DLG_UI_Selection_MessageBox_Ok_Exit_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				m_pDLGGoToFirstDungeon = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_706 ), 
					SUSUCM_START_FIRST_DUNGEON_OK, this, SUSUCM_START_FIRST_DUNGEON_CANCEL );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����


				return true;
			} break;


		case SUSUCM_START_FIRST_DUNGEON_OK:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGoToFirstDungeon, NULL, false );
				m_pDLGGoToFirstDungeon = NULL;

				if( NULL == m_pSelectUnit )
					return true;

				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				targetDetail.m_iChannelID = (int) 104; // fix!!! ä�ι�ȣ �ϴ� �ϵ��ڵ�
				targetDetail.m_iDungeonID = (int) SEnum::DI_EL_FOREST_GATE_NORMAL;
				targetDetail.m_iRoomUID = -1;

				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_DUNGEON_GAME, targetDetail, m_pSelectUnit->GetUID() );

				return true;
			} break;

		case SUSUCM_START_FIRST_DUNGEON_CANCEL:
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGoToFirstDungeon, NULL, false );
				m_pDLGGoToFirstDungeon = NULL;


				if( NULL == m_pSelectUnit )
					return true;

				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_VILLAGE_MAP, targetDetail, m_pSelectUnit->GetUID() );

				return true;

			} break;

		case SUSUCM_RECOMMEND_OK:
			{
				//REQ��������
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGRecommend->GetControl( L"g_pIMEEditBoxName" );
				if ( pIMEEditBox != NULL )
				{
					return Handler_EGS_RECOMMEND_USER_REQ( pIMEEditBox->GetText() );
				}
			}
			break;

		case SUSUCM_RECOMMEND_CANCEL:
			{
				//�׳� �н� ����������.
				OpenRecommendMsgBox( false );
				//Handler_EGS_GET_POST_LETTER_LIST_REQ();

				return Handler_EGS_RECOMMEND_USER_REQ( L"", true );				
			}
			break;

#ifdef SERV_RECOMMEND_LIST_EVENT
		case SUSUCM_RECOMMEND_NEXT:
			{
				OpenRecommendMsgBox( false );
				return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
			}
			break;
#endif //SERV_RECOMMEND_LIST_EVENT


		case SUSUCM_PREV:
			{
				if ( m_NowPage > 1 )
				{
					
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					ClearUnitButton( false );
					--m_NowPage;
					ChangeUnitButtonInfo();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					ClearBeforeUnit( m_pSelectUnit );
					ClearUnitButton();
					--m_NowPage;
					CreateUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����



					
				}
				//else
				//{
				//	ClearBeforeUnit( m_pSelectUnit );
				//	ClearUnitButton();

				//	m_NowPage = m_MaxPage;

				//	CreateUnitButton();
				//}
			}
			break;

		case SUSUCM_NEXT:
			{
				if ( m_NowPage < m_MaxPage )
				{
					if( (m_NowPage * UNIT_SELECT_UI_MAX_UNIT_NUM) <= g_pData->GetMyUser()->GetUnitNum() + 1)
					{	
						
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
						ClearUnitButton( false );
						++m_NowPage;
						ChangeUnitButtonInfo();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
						ClearBeforeUnit( m_pSelectUnit );
						ClearUnitButton();
						++m_NowPage;
						CreateUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����



						
					}
				}
				//else
				//{
				//	ClearBeforeUnit( m_pSelectUnit );
				//	ClearUnitButton();

				//	m_NowPage = 1;

				//	CreateUnitButton();
				//}
			}
			break;

		case SSSUCM_SERVER_CONNECT0:	// ���� ���� ����
			{	
				// �ٸ� ���Ӽ������� ������ �������� ��ٸ��� ���̶�� ���Ӽ����� �������� �ʰ� ��ٸ����� �Ѵ�.
				if( true == m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK )		
					return true;
				
				
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				int iChannelIndex = pButton->GetDummyInt(0)-1;

#ifdef CHANGE_CHANNEL
				const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[ iChannelIndex ];
#else
				const KChannelInfo& channelInfo = m_vecChannelInfo[ iChannelIndex ];
#endif
				m_iTryConnectChannelID = channelInfo.m_iChannelID;
				
				//{{ ����� : [2011/6/20] //        ä�� ���� ������� ����
#ifdef ADD_SERVER_GROUP
				m_bSelectChannel = true;
#endif ADD_SERVER_GROUP
				//}} ����� : [2011/6/20] //        ä�� ���� ������� ����

				return Handler_EGS_CONNECT_REQ( channelInfo.m_wstrIP, channelInfo.m_usMasterPort );
				
			} break;

		case SSSUCM_SELECT_SERVER_GROUP:  // ä�� ���� ���� �� ����
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;							// fix!! ���� ��ư���� ���� �ؾ��� 
				int iNewChannelServerIndex = pButton->GetDummyInt(0);

				//{{ kimhc // 2009-12-15 // ���� �׷� �߰�
#ifdef	ADD_SERVER_GROUP
				// ������ �������� �ٲ���ٸ� ����
#ifdef EXTEND_SERVER_GROUP_MASK
				if ( g_pInstanceData->GetServerGroupID() != iNewChannelServerIndex )
#else
				if ( g_pInstanceData->GetServerGroupID() != SERVER_GROUP_ID( iNewChannelServerIndex ) )
#endif EXTEND_SERVER_GROUP_MASK
				{
#ifdef EXTEND_SERVER_GROUP_MASK
					g_pInstanceData->SetServerGroupID( iNewChannelServerIndex );
#else
					g_pInstanceData->SetServerGroupID( SERVER_GROUP_ID( iNewChannelServerIndex ) );
#endif EXTEND_SERVER_GROUP_MASK
					UpdateServerSelectButton( g_pInstanceData->GetServerGroupID() );	

					if( 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol() != NULL && g_pData->GetServerProtocol()->IsChUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        )
					{
						// ������ ����� ä�� ���� ������ ����
						// ���� ���õ� ä�� ������ �����Ѵ�. 
						Handler_ECH_DISCONNECT_REQ();
						m_bReservedConnectionToChannelServer = true;
					}
					else
					{
						ConnectToChannelServer();
					}
				}
#else	ADD_SERVER_GROUP
				if( g_pMain->GetPickedChannelServerIPIndex() == iNewChannelServerIndex )
				{
					return true;
				}
				else
				{
					if( true == m_sbSelectedServerSet && 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol() != NULL && g_pData->GetServerProtocol()->IsChUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
                        )
					{
						// ������ ����� ä�� ���� ������ ����
						// ���� ���õ� ä�� ������ �����Ѵ�. 
						Handler_ECH_DISCONNECT_REQ();
					}
					else
					{
						ConnectToChannelServer();
					}
				}
#endif	ADD_SERVER_GROUP
				//}} kimhc // 2009-12-15 // ���� �׷� �߰�
								
				return true;

			} break;

			//������� �ʰ� ����.
		//case SSSUCM_SERVER_CONNECT1:
		//	{
		//		return Handler_EGS_SELECT_SERVER_SET_REQ();
		//	}
		//	break;

		//case SSSUCM_SERVER_CONNECT2:
		//	{
		//		return Handler_EGS_SELECT_SERVER_SET_REQ();
		//	}
		//	break;

#ifndef DISABLE_DISAGREE_HACK_USER
		case SSSUCM_HACK_USER_PROCESS_CHECK_OK:
			{
				Handler_EGS_AGREE_HACK_USER_REQ();
			}
			break;

		case SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL:
			{
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
			}
			break;
#endif DISABLE_DISAGREE_HACK_USER
		case SSSUCM_SERVER_SELECT_EXIT:
			{
				CX2State::QuitGame();
			}
			break;

		case SSSUCM_UNIT_SELECT_EXIT:
			{
				UnitSelectExit();		
			}
			break;
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		case SSSUCM_ATL_LIMIT_CHECK_OK:
			{
				ShellExecute(NULL, L"open", L"https://member.nexon.com/verify/realname.aspx", L"",L"", SW_SHOWNORMAL );
				if( NULL != m_pDLGATLLimitCheckMsgBox )
				{
					g_pKTDXApp->SendGameDlgMessage(XGM_DELETE_DIALOG, m_pDLGATLLimitCheckMsgBox, NULL, false);
				}
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
			}
			break;
#endif //SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		case SSSUCM_EVENT_GUIDE_OK:
			{
				if( NULL != m_pDLGEventGuide )
				{
					m_pDLGEventGuide->SetShowEnable(false,false);
				}
			}
			break;
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE

#ifdef SHOW_ONLY_ADVERTISEMENT
		case SSSUCM_HIDE_UI_MOUSE_OVER:
			{
				SetShowServerSelectUI( false );
			} break;
		case SSSUCM_HIDE_UI_MOUSE_OUT:
			{
				SetShowServerSelectUI( true );
			} break;
#endif //SHOW_ONLY_ADVERTISEMENT


#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
		// ���� ���� ���� �˸�â ����
		case SSSUCM_JUMPING_CHARACTER_OK:
			{
				if( NULL != m_pDLGInfoCreateJumpingChar )
					m_pDLGInfoCreateJumpingChar->SetShowEnable(false, false);
			} break;
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����			

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		case SSSUCM_UNIT_VIEWER_UP :
			{
				CX2UnitViewerUI::SetUnitClicked( false );
			}
			break;

		case SSSUCM_UNIT_VIEWER_DOWN :
			{
				CX2UnitViewerUI::SetUnitClicked( true );
			}
			break;
		case SSSUCM_UNIT_VIEWER_OVER: 
			{
			}
			break;
		case SUSUCM_SELECT_CHANNEL_LIST_STEP: 
			{
#ifdef FIX_REFORM_ENTRY_POINT_3RD		// ������, ���� ���� ���� 3�� ( ũ���� ) ����
				if ( NULL != m_pSelectUnit )
#endif // FIX_REFORM_ENTRY_POINT_3RD	// ������, ���� ���� ���� 3�� ( ũ���� ) ����
				{
					wstring tempNickName = m_pSelectUnit->GetNickName();
					if ( tempNickName.find( GET_STRING(STR_ID_29404) ) != -1 )
					{
						//�г����� ���� ĳ���ʹ� �г����� ���� �Է��϶�� �˾�â�� ����.
						OpenRenameMsgBox( true );
						SetRenameMsgBoxError( GET_STRING(STR_ID_29411) );
					}
					else
					{
	#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT //��â��
						if( NULL != m_pSelectUnit && m_pSelectUnit->GetType() == CX2Unit::UT_ADD )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 275), GET_STRING( STR_ID_29409 ), this, -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2 ( 0, -130 ),  L"UI_PopUp_Negative_01.ogg" );

							break;
						}
	#endif //SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_UNIT_WAIT_DELETE
						if( NULL != m_pSelectUnit && m_pSelectUnit->GetUnitData().m_bDeleted == true )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 275), GET_STRING( STR_ID_30401 ), this, -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2 ( 0, -130 ),  L"UI_PopUp_Negative_01.ogg" );
							break;
						}
#endif SERV_UNIT_WAIT_DELETE

						Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
						m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY ); 
						m_bPlayDownAnim = false;
					
	// 					SetShowServerStateButtonSet ( false, true );
	// 					ClearUnitButton ( false );		
	// 					CreateChannelButton ( );

	#ifdef ADD_PLAY_SOUND //��â��
						if( m_pSelectUnit != NULL )
							PlaySoundSelect( m_pSelectUnit->GetType(), SPT_SERVER_SELECT );
	#endif //ADD_PLAY_SOUND
	//					SAFE_DELETE_DIALOG ( m_pDLGMsgBox );
	//					m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), L"ä�� ����� �޾ƿ��� �� �Դϴ�.", this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );		
	//					m_bWaiting_EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK = true;
	//					Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
#ifndef FIX_REFORM_ENTRY_POINT_3RD		// ������, ���� ���� ���� 3�� ( ũ���� ) ����
						m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY ); 
						m_bPlayDownAnim = false;
#endif // FIX_REFORM_ENTRY_POINT_3RD	// ������, ���� ���� ���� 3�� ( ũ���� ) ����
					}
				}
#ifdef FIX_REFORM_ENTRY_POINT_3RD		// ������, ���� ���� ���� 3�� ( ũ���� ) ����
				return true;
#endif // FIX_REFORM_ENTRY_POINT_3RD	// ������, ���� ���� ���� 3�� ( ũ���� ) ����
			}
			break;

		case SUSUCM_SECURITY_PAD:
			{
				if( NULL != g_pMain && NULL != g_pMain->GetSecurityPad() )
					g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPC_SELECT_STATE);
			}
			break;

		case SUSUCM_SELECT_UNIT_LIST_STEP: 
			{
				MoveToCharacterSelectUI();
			}
			break;


		case SUSUCM_PREV_CHANNEL_LIST :
			{
				if ( m_iChannelNowPage > 1 )
				{
					ClearChannelButton();
					--m_iChannelNowPage;
					CreateChannelButton();
				}
			}
			break;

		case SUSUCM_NEXT_CHANNEL_LIST :
			{	
				if ( m_iChannelNowPage < m_iChannelMaxPage )
				{
					ClearChannelButton();
					++m_iChannelNowPage;
					CreateChannelButton();
				}
			}
			break;

		case SUSUCM_CHANNEL_BUTTON_UP :
			{
				CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*> ( lParam );
				if ( NULL != pButton )
				{
					return ChannelButtonUp( pButton );
				}
			}
			break;

		case SUSUCM_SECURITY_PAD_CREATE :
			{
				if( NULL != g_pMain && NULL != g_pMain->GetSecurityPad() )
					g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_CREATE_PASSWORD );
			}
			break;
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��

		case SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK :
			{
#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
				if ( g_pMain->GetIsPlayingTutorial() == true )
				{
					g_pMain->SetIsPlayingTutorial( false );		
				}
				m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection = false; 
				m_sbSelectedServerSet = false;
				if ( true == g_pData->GetServerProtocol()->IsConnected() )
					g_pData->GetServerProtocol()->DisconnectFromGameServer();
				if ( true == g_pData->GetServerProtocol()->IsChConnected() )
					g_pData->GetServerProtocol()->DisconnectFromChannelServer();
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false );
#else  // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����
				g_pData->GetServerProtocol()->DisconnectFromChannelServer();
				ConnectToChannelServer ( true );
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGReConnectChannelServer, NULL, false );
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����
			} break;
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef NEW_OPTION_SECURITYPAD
		case SSSUCM_SECURITYPAD:
			{
				if( NULL != g_pMain && NULL != g_pMain->GetSecurityPad() )
					g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_CREATE_PASSWORD);
			}
			break;
#endif NEW_OPTION_SECURITYPAD
#ifdef SIMPLE_BUG_FIX
		case SSSUCM_DOUBLE_CONNECT:
			{
				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_DOUBLE_CONNECT );
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
			}break;
#endif SIMPLE_BUG_FIX
	}

	return false;
}

bool CX2StateServerSelect::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef POSTBOX
    if( g_pMain->GetPostBox() != NULL)
    {
        if( g_pMain->GetPostBox()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
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
	
	switch( wParam )
	{
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION		/// ��Ŷ�� ũ�Ⱑ �ִ� ��� ũ�⸦ �Ѱܹ�����, ����
	case EGS_SELECT_UNIT_1_NOT:		/// ������ ����, �κ��丮, ��ų
		return Handler_EGS_SELECT_UNIT_1_NOT( hWnd, uMsg, wParam, lParam );
		break;
	case EGS_SELECT_UNIT_2_NOT:		/// ����Ʈ
		return Handler_EGS_SELECT_UNIT_2_NOT( hWnd, uMsg, wParam, lParam );
		break;
	case EGS_SELECT_UNIT_3_NOT:		/// Ÿ��Ʋ, ��
		return Handler_EGS_SELECT_UNIT_3_NOT( hWnd, uMsg, wParam, lParam );
		break;
	case EGS_SELECT_UNIT_4_NOT:		/// ���� �̺�Ʈ, ����, ����, ��ŷ
		return Handler_EGS_SELECT_UNIT_4_NOT( hWnd, uMsg, wParam, lParam );
		break;
	case EGS_SELECT_UNIT_5_NOT:		/// ETC, 1ȸ�� �̺�Ʈ
		return Handler_EGS_SELECT_UNIT_5_NOT( hWnd, uMsg, wParam, lParam );
		break;
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	case EGS_SELECT_UNIT_ACK:		/// m_iOK ��
		return Handler_EGS_SELECT_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		break;
#if 0 
	case EGS_STATE_CHANGE_VILLAGE_MAP_ACK:
		return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif

	case EGS_DELETE_UNIT_ACK:
		return Handler_EGS_DELETE_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		break;


	case EGS_CHANGE_NICK_NAME_ACK:
		{
			return Handler_EGS_CHANGE_NICK_NAME_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_RECOMMEND_USER_ACK:
		{
			return Handler_EGS_RECOMMEND_USER_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	//case EGS_GET_POST_LETTER_LIST_ACK:
	//	{
	//		return Handler_EGS_GET_POST_LETTER_LIST_ACK( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	// remove!!		
	//case EGS_ITEM_EXPIRATION_NOT:
	//	{
	//		return Handler_EGS_ITEM_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;



	case EGS_SELECT_SERVER_SET_ACK:
		return Handler_EGS_SELECT_SERVER_SET_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK:
		return Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_AGREE_HACK_USER_ACK:
		return Handler_EGS_AGREE_HACK_USER_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_FIELD_ACK:
		return Handler_EGS_STATE_CHANGE_FIELD_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_USER_LIST_IN_SECTOR_NOT:
		return Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CREATE_TUTORIAL_ROOM_ACK:
		return Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		break;

//#ifdef SERV_KOG_OTP_VERIFY -- 2009-08-31  ������ : �ּ�ó����
	case ECH_VERIFY_ACCOUNT_ACK:
		return Handler_ECH_VERIFY_ACCOUNT_ACK(hWnd, uMsg, wParam, lParam);
		break;
//#endif


	case EGS_CONNECT_ACK:
		{
			return Handler_EGS_CONNECT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_VERIFY_ACCOUNT_ACK:
		{
			return Handler_EGS_VERIFY_ACCOUNT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case ENX_USER_LOGIN_NOT:
		{
			return Handler_ENX_USER_LOGIN_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CURRENT_TIME_ACK:
		{
			return Handler_EGS_CURRENT_TIME_ACK( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef SERVER_GROUP_UI_ADVANCED
	case ECH_GET_SERVERGROUP_LIST_ACK:
		{
			return Handler_ECH_GET_SERVERGROUP_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;
		
	case ECH_GET_SERVERGROUP_LIST_NOT:
		{
			return Handler_ECH_GET_SERVERGROUP_LIST_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERVER_GROUP_UI_ADVANCED

	case ECH_GET_CHANNEL_LIST_ACK:
		{
			return Handler_ECH_GET_CHANNEL_LIST_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case ECH_GET_CHANNEL_LIST_NOT:
		{
#ifdef FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
			if ( true == Handler_ECH_GET_CHANNEL_LIST_NOT( hWnd, uMsg, wParam, lParam ) )
				return true;

			// m_bCanNotConenctToBusyServer = true;
			if ( true == g_pData->GetServerProtocol()->IsConnected() )
				g_pData->GetServerProtocol()->DisconnectFromGameServer();
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false ); 
			return true;
#else //  FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
			return Handler_ECH_GET_CHANNEL_LIST_NOT( hWnd, uMsg, wParam, lParam );
#endif // FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
		} break;


	case ECH_DISCONNECT_ACK:
		{
			return Handler_ECH_DISCONNECT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_DISCONNECT_FOR_SERVER_SELECT_ACK:
		{
#ifdef FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
			if ( true == Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( hWnd, uMsg, wParam, lParam ) )
				return true;

			m_bCanNotConenctToBusyServer = true;

			if ( true == g_pData->GetServerProtocol()->IsConnected() )
				g_pData->GetServerProtocol()->DisconnectFromGameServer();
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false ); 
			return true;
#else //  FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
			return Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( hWnd, uMsg, wParam, lParam );
#endif // FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����

		} break;

	case EGS_GET_MY_MESSENGER_SN_ACK:
		{
			return Handler_EGS_GET_MY_MESSENGER_SN_ACK( hWnd, uMsg, wParam, lParam );
		} break;

		//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
	case EGS_SELECT_UNIT_INVENTORY_INFO_NOT:
		{
			return Handler_EGS_SELECT_UNIT_INVENTORY_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_GET_MY_INVENTORY_ACK:
		{
			return Handler_EGS_GET_MY_INVENTORY_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

#ifdef SERV_SECOND_SECURITY
	case EGS_SECOND_SECURITY_INFO_NOT:
		{
			return Handler_EGS_SECOND_SECURITY_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_SECOND_SECURITY

	case EGS_KEYBOARD_MAPPING_INFO_NOT:
		{
			return Handler_EGS_KEYBOARD_MAPPING_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_CHAT_OPTION_INFO_NOT:
		{
			return Handler_EGS_CHAT_OPTION_INFO_NOT( hWnd, uMsg, wParam, lParam ); 
		} break;

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	case EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK:
		{
			return Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE

#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	case EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT:
		{
			return Handler_EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT( hWnd, uMsg, wParam, lParam );
		}break;
#endif //SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	case EGS_JUMPING_CHARACTER_GUIDE_NOT :
		{
			// ���� ���� ���� �˸�â�� ��� ���ΰ�?
			return Handler_EGS_JUMPING_CHARACTER_GUIDE_NOT ( hWnd, uMsg, wParam, lParam);	
		} break;
#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	case EGS_CHARACTER_LIST_ACK :
		{
			return Handler_EGS_CHARACTER_LIST_ACK ( hWnd, uMsg, wParam, lParam);
		} break;

	case EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK :
		{
			Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK ( hWnd, uMsg, wParam, lParam);
		} break;

	case EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK :
		{
			Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK ( hWnd, uMsg, wParam, lParam);
		} break;

#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
		//{{ //2012.03.05 lygan_������ // ��ȿ���� �۾� �����Ⱥκ� �߰� �۾�
#ifdef SERV_UNIT_WAIT_DELETE
	case EGS_FINAL_DELETE_UNIT_ACK:
		{
			return Handler_EGS_FINAL_DELETE_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_RESTORE_UNIT_ACK:
		{
			return Handler_EGS_RESTORE_UNIT_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SERV_UNIT_WAIT_DELETE
		//}}
#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
	case EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK:
		{
			return Handler_EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK( hWnd, uMsg, wParam, lParam );
		}break;
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
	}
	return false;
}



#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
void CX2StateServerSelect::CreateChannelButton( bool bSelectTopChannelButton /* = false */ )
{
	CKTDGUIDialogType pChannelSlot = NULL;
	CKTDGUIButton* pButton = NULL;

	D3DXVECTOR3 dummyPos;

	ClearChannelButton();



	vector< KChannelInfo > * vecChannelInfo = NULL;
	if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
	{

		m_iChannelMaxPage = static_cast <int> ( m_vecGaiaChannelInfo.size() ) / CHANNEL_SELECT_UI_MAX_CHANNEL_NUM + 1;

		if ( static_cast <int> ( m_vecGaiaChannelInfo.size() ) > 0 )
		{
			if ( ( static_cast <int> ( m_vecGaiaChannelInfo.size() ) % CHANNEL_SELECT_UI_MAX_CHANNEL_NUM )  == 0 )
			{
				if ( m_iChannelMaxPage > 1 )
					m_iChannelMaxPage -= 1;
			}
		}
		vecChannelInfo = &m_vecGaiaChannelInfo;
	}
	else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
	{
		m_iChannelMaxPage = static_cast <int> ( m_vecSolesChannelInfo.size() ) / CHANNEL_SELECT_UI_MAX_CHANNEL_NUM + 1;

		if ( static_cast <int> ( m_vecSolesChannelInfo.size() ) > 0 )
		{
			if ( ( static_cast <int> ( m_vecSolesChannelInfo.size() ) % CHANNEL_SELECT_UI_MAX_CHANNEL_NUM ) == 0 )
			{
				if ( m_iChannelMaxPage > 1 )
					m_iChannelMaxPage -= 1;
			}
		}
		vecChannelInfo = &m_vecSolesChannelInfo;
	}
	else
	{
#ifdef FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
		m_iChannelMaxPage = static_cast <int> ( m_vecSolesChannelInfo.size() ) / CHANNEL_SELECT_UI_MAX_CHANNEL_NUM + 1;

		if ( static_cast <int> ( m_vecGaiaChannelInfo.size() ) > 0 )
		{
			if ( ( static_cast <int> ( m_vecGaiaChannelInfo.size() ) % CHANNEL_SELECT_UI_MAX_CHANNEL_NUM ) == 0 )
			{
				if ( m_iChannelMaxPage > 1 )
					m_iChannelMaxPage -= 1;
			}
		}
		vecChannelInfo = &m_vecSolesChannelInfo;
#else
		ASSERT (! "Worng Server Group ID !! ");	
#endif // FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
		return ;
	}

	CKTDGUIStatic* pStaticUnitPage = ( CKTDGUIStatic* ) m_pDLGUnitSelectFront->GetControl( L"number" );
	WCHAR buff[256] = {0,};
	StringCchPrintf( buff, 256, L"%d / %d", m_iChannelNowPage, m_iChannelMaxPage );	
	pStaticUnitPage->GetString(0)->msg = buff;


	int i = (m_iChannelNowPage - 1) * CHANNEL_SELECT_UI_MAX_CHANNEL_NUM;
	int j = 0;

	for( ; i < static_cast<int> ( vecChannelInfo->size() ) && j < CHANNEL_SELECT_UI_MAX_CHANNEL_NUM; i++, j++ )
	{
		const KChannelInfo& channelInfo = (*vecChannelInfo)[i];
		pChannelSlot = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection_Slot_New.lua" );

		dummyPos = m_pDLGUnitSelectFront->GetDummyPos( 8 + j );		
	
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pChannelSlot );
		pChannelSlot->SetShowEnable(true, true);
		pChannelSlot->SetPos ( D3DXVECTOR2 ( dummyPos.x, dummyPos.y ) );
		
		wstring wstrChannelButtonName = g_pInstanceData->GetChannelButtonNameByChannelName ( channelInfo.m_wstrChannelName );

		CKTDGUIButton * pSelectChannelButton = static_cast<CKTDGUIButton *> ( pChannelSlot->GetControl( wstrChannelButtonName.c_str() ) );		
		if ( NULL != pSelectChannelButton )
		{
			pSelectChannelButton->SetShowEnable( true, true );
			pSelectChannelButton->AddDummyInt(i);
		}

		float fChannelCongestionRate = static_cast< float >( channelInfo.m_iCurrentUser ) / static_cast< float >( channelInfo.m_iMaxUser );

		wstring wstrChannelCongestion = L"";
		if(fChannelCongestionRate >= CHANNEL_CONGESTION_SCOPE_TEMP[3])
		{
			wstrChannelCongestion += L"#Cff383f"; // FULL
			wstrChannelCongestion += GET_STRING(STR_ID_724);
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[2])
		{
			wstrChannelCongestion += L"#Cf7941d"; // ȥ��
			wstrChannelCongestion += GET_STRING(STR_ID_723);
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[1])
		{
			wstrChannelCongestion += L"#Cfff200"; // ����
			wstrChannelCongestion += GET_STRING(STR_ID_722);
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[0])
		{
			wstrChannelCongestion += L"#C93fe20"; // ��Ȱ
			wstrChannelCongestion += GET_STRING(STR_ID_721);
		}
		else
		{
			wstrChannelCongestion += L"#C93fe20"; // ��Ȱ
			wstrChannelCongestion += GET_STRING(STR_ID_721);
		}
		std::map<int, KChannelBonusInfo>::const_iterator mit;

		if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
		{
			mit = m_mapGaiaChannelBonusInfo.find(channelInfo.m_iChannelID);
			if ( mit == m_mapGaiaChannelBonusInfo.end() )
			{
				ASSERT ( !L"Can't Find Server Info !!");
				return ;
			}
		}
		else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
		{
			mit = m_mapSolesChannelBonusInfo.find(channelInfo.m_iChannelID);
			if ( mit == m_mapSolesChannelBonusInfo.end() )
			{
				ASSERT ( !L"Can't Find Server Info !!");
				return ;
			}
		}
		
		else
		{
			ASSERT ( !L"Wrong Server Group ID ! ");
			return ;
		}
		wstringstream wstrChannelButtonLevelInfo;
		wstrChannelButtonLevelInfo << L" (Lv." << mit->second.m_iBeginLv << L"~" << mit->second.m_iEndLv << L")";			

		CKTDGUIStatic * pChannelInfoStatic = static_cast<CKTDGUIStatic *> ( pChannelSlot->GetControl( L"ChannelButtonInfo" ) );		
		if ( NULL != pChannelInfoStatic )
		{
			pChannelInfoStatic->SetString ( 0, channelInfo.m_wstrChannelName.c_str() );
			pChannelInfoStatic->SetString ( 1, wstrChannelButtonLevelInfo.str().c_str() );
			pChannelInfoStatic->SetString ( 2, wstrChannelCongestion.c_str() );
		}
			
		m_vecChannelSlot.push_back(pChannelSlot);		
		pChannelSlot->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );

		if ( true == bSelectTopChannelButton )
		{
			if ( j == 0 )
			{
				ChannelButtonUp ( pSelectChannelButton );
			}
		}
	}
}

#endif // REFORM_ENTRY_POINT 13-11-11, ���� ���� ����, kimjh

void CX2StateServerSelect::CreateUnitButton()
{
	//���� ������ Ȯ���Ѵ�.
	//�� ���� ������ ���� UnitButton�� �����Ѵ�.
	//��ư �̸��� ���� �г������� �ٲ۴�.
	//�ε����� ���� ��ġ�� �ٲ۴�.
#if 1 
	CKTDGUIDialogType pUnitSlot = NULL;
	CKTDGUIButton* pButton = NULL;

	D3DXVECTOR3 dummyPos;
	bool bAllSlotOccupiedInAPage = false;	// �ѹ��� �� �� �ִ� ĳ���� ���Լ� ��ŭ ĳ���Ͱ� ��� �ִ���, ���ٸ� ������ ����� ĳ���� ���������� ����� ���ؼ�


	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		SAFE_DELETE_DIALOG(pUnitSlot);
	}
	m_vecUnitSlot.resize(0);
	
	//if(g_pData->GetMyUser()->GetUnitNum() > 0)
	//{
	//	m_MaxPage = ((g_pData->GetMyUser()->GetUnitNum()-1) / UNIT_SELECT_UI_MAX_UNIT_NUM) + 1;
	//}
	//else
	//{
	//	m_MaxPage = 1;
	//}

	CKTDGUIStatic* pStaticUnitPage = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"number" );
	WCHAR buff[256] = {0,};
	//wsprintf( buff, L"%d / %d", m_NowPage, m_MaxPage );	
	StringCchPrintf( buff, 256, L"%d / %d", m_NowPage, m_MaxPage );	
	pStaticUnitPage->GetString(0)->msg = buff;

	
#ifdef CONVERSION_VS
	int i = (m_NowPage - 1)*UNIT_SELECT_UI_MAX_UNIT_NUM;
	int j = 0;
	for( ; i < g_pData->GetMyUser()->GetUnitNum() && j < UNIT_SELECT_UI_MAX_UNIT_NUM; i++, j++ )
#else CONVERSION_VS
	for( int i = (m_NowPage - 1)*UNIT_SELECT_UI_MAX_UNIT_NUM, j = 0; i < g_pData->GetMyUser()->GetUnitNum() && j < UNIT_SELECT_UI_MAX_UNIT_NUM; i++, j++ )
#endif CONVERSION_VS
	{
		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( i );
		if( pUnit == NULL )
			continue;	
		
		dummyPos = m_pDLGUnitSelectFront->GetDummyPos( j );		

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Slot_New.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Slot.lua" );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pUnitSlot );
		pUnitSlot->SetShowEnable(true, true);
		m_vecUnitSlot.push_back(pUnitSlot);		

		CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)pUnitSlot->GetControl( L"UnitEmblem" );
		pStaticEmblem->GetPicture(0)->SetShow( true );

		if ( NULL != g_pMain->GetPVPEmblem() )
		{
#ifdef PVP_SEASON2
		CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnit->GetPvpRank() );
#else
		CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnit->GetPVPEmblem() );
#endif
			if ( pPVPEmblemData != NULL )
			{
				pStaticEmblem->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
			}
#ifdef NO_RANK_ICON_OF_NEW_CHARACTER
			else
			{
				pStaticEmblem->GetPicture(0)->SetShow( false );
			}
#endif NO_RANK_ICON_OF_NEW_CHARACTER
		}
		
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		CX2UnitViewerUI* pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
		//pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
		//pUnitViewerUI->SetLightPos( 300, 300, -500 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
		pUnitViewerUI->SetLightPos( -250, 100, -600 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
		g_pKTDXApp->GetDGManager()->AddObjectChain( pUnitViewerUI );

		D3DXVECTOR3 vUnitPos = dummyPos;
		vUnitPos.x -= 400.f; // 222
		vUnitPos.y -= 110.f; // 302
		
#ifdef RAVEN_SECOND_CLASS_CHANGE
		// ���̺� �ⷰ���� �ʹ� �� �̸��� ������ ����� ���̺츸 �ణ ���� �÷��ش�. 
		if( pUnit != NULL && (pUnit->GetType() == CX2Unit::UT_RAVEN || pUnit->GetType() == CX2Unit::UT_EVE) )
			vUnitPos.y += 5.f;
#endif

		//{{ kimhc // 2010-01-06 // PC�� �����̾� ����
#ifdef	PC_BANG_WORK
		if ( pUnit != NULL )
			pUnit->AccessInventory().RemovePCBangEquips();
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-06 // PC�� �����̾� ����

#ifdef FIX_CLASS_CHANGE_MESH
		pUnitViewerUI->SetClassChange( false );
#endif

#ifdef SEPARATION_MOTION
		pUnitViewerUI->SetUnit( pUnit, CX2UnitViewerUI::UVS_SELECTUNIT ); 
#else
		pUnitViewerUI->SetUnit( pUnit );
#endif
		//pUnitViewerUI->SetOverUI(true);
		pUnitViewerUI->GetMatrix().Move( vUnitPos );
		pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) * pUnitViewerUI->GetScale() );
		pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		//pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );

		pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );

		pUnitViewerUI->SetShowObject( false );

		m_UnitViewerUIList.push_back( pUnitViewerUI );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
		pButton = (CKTDGUIButton*)pUnitSlot->GetControl( L"character1" );

		switch( pUnit->GetType() )
		{
		case CX2Unit::UT_ELSWORD:
		case CX2Unit::UT_LIRE:
		case CX2Unit::UT_ARME:
		case CX2Unit::UT_RAVEN:
		case CX2Unit::UT_EVE:
	//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		case CX2Unit::UT_CHUNG:
#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA:
#endif
		case CX2Unit::UT_ELESIS:

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		case CX2Unit::UT_ADD:
#endif //SERV_9TH_NEW_CHARACTER
			break;


		default:
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_707 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_707 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����				
			}break;
		}
		XSkinMeshReady( pUnit->GetUnitTemplet()->m_MotionFile.c_str() );


		wstringstream buttonName;
		buttonName << L"SLOT_BUTTON_" << pUnit->GetUID();
		if( NULL != pButton )
		{
			pButton->SetName( buttonName.str().c_str() );
		}
		
		WCHAR buff[256] = {0,};
		//wsprintf( buff, L"%d", (int)pUnit->GetUnitData().m_Level );
		StringCchPrintf( buff, 256, L"%d", (int)pUnit->GetUnitData().m_Level );
		CKTDGUIStatic* pStaticLVNum = (CKTDGUIStatic*)pUnitSlot->GetControl( L"StaticUnitSelectStringLVNum" );
		pStaticLVNum->GetString(0)->msg = buff;





		CKTDGUIStatic* pStaticID = (CKTDGUIStatic*)pUnitSlot->GetControl( L"StaticUnitSelectStringID" );
		CKTDGUIStatic* pStaticChangeNickEmblem = (CKTDGUIStatic*) pUnitSlot->GetControl( L"ChangeNick" );
		CKTDGUIStatic* pStaticChangeNickMessage = (CKTDGUIStatic*) pUnitSlot->GetControl( L"standby" );

		wstring tempNickName = pUnit->GetNickName();
		if ( tempNickName.find( L"__DELETED__") != -1 )
		{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			pStaticID->GetString(0)->msg = GET_STRING(STR_ID_29404);
#else  // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
			pStaticID->GetString(0)->msg = L"     ?";
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

			pStaticChangeNickEmblem->SetShowEnable( true, true );
			pStaticChangeNickMessage->SetShowEnable( true, true );

		}
		else
		{
			pStaticID->GetString(0)->msg = pUnit->GetNickName();

			pStaticChangeNickEmblem->SetShowEnable( false, false );
			pStaticChangeNickMessage->SetShowEnable( false, false );
		}

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		CKTDGUIStatic* pStaticLastPlace = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"StaticUnitSelectStringID_LastPlace" ) );
		wstring wstringLastPlace = L"�ӽý�Ʈ����������繰�������������";
		


		wstringLastPlace = wstringLastPlace.substr(0, 11);
		wstringLastPlace += L"..";

		pStaticLastPlace->GetString(0)->msg = wstringLastPlace.c_str();

		CKTDGUIStatic* pStaticUnitFace = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"UnitFace" ) );

		wstring fileName, pieceName;		
		if ( true == CX2Data::GetCharacterImageName( fileName, pieceName, pUnit->GetClass(), CX2Data::CIT_UnitSlot ) )
		{
			CKTDGUIControl::CPictureData* pPictureData = pUnitSlot->CreatePicture();
			pStaticUnitFace->AddPicture ( pPictureData );
			pPictureData->SetTex ( fileName.c_str(), pieceName.c_str() );
			CKTDGUIControl::UIPointData* pPointData = new CKTDGUIControl::UIPointData();
			pPointData->leftTopPoint = D3DXVECTOR2 ( 32, 8 );
			pPointData->color = D3DXCOLOR ( 1.0f, 1.0f, 1.0f, 1.0f );
			pPictureData->SetPoint ( pPointData );
			pPictureData->SetSize( D3DXVECTOR2 ( 38, 38 ) );
		}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

		
		D3DXVECTOR2 buttonPos = D3DXVECTOR2( dummyPos.x, dummyPos.y );

		/*
		D3DXVECTOR2 buttonPos = D3DXVECTOR2( 0, 0 );
		if ( i == 0 )
		{
		buttonPos = D3DXVECTOR2( 378, 123 );
		}
		else if ( i == 1 )
		{
		buttonPos = D3DXVECTOR2( 586, 123 );
		}
		else if ( i == 2 )
		{
		buttonPos = D3DXVECTOR2( 795, 123 );
		}
		else if ( i == 3 )
		{
		buttonPos = D3DXVECTOR2( 378, 404 );
		}
		else if ( i == 4 )
		{
		buttonPos = D3DXVECTOR2( 586, 404 );
		}
		else if ( i == 5 )
		{
		buttonPos = D3DXVECTOR2( 795, 404 );
		}
		*/
		/*
		m_pDLGUnitSelectFront->OpenScriptFile( L"DLG_Unit_Select_Unit_Info.lua" );
		wstring tempName;
		tempName = pUnit->GetNickName();
		tempName += L"UnitSelectUnitInfo";

		CKTDGUIStatic* pStaticUnitInfo = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"UnitSelectUnitInfo" );
		pStaticUnitInfo->SetName( tempName.c_str() );
		pStaticUnitInfo->GetString(0)->msg = pUnit->GetNickName();
		pStaticUnitInfo->GetString(1)->msg = L"�޺������մ�";

		WCHAR arLV[8] = {0,};
		//wsprintf(arLV, 7, L"%d", pUnit->GetUnitData().m_Level );
		StringCchPrintf( buff, 8, L"%d", pUnit->GetUnitData().m_Level );
		pStaticUnitInfo->GetString(2)->msg = arLV;
		*/
		//		buttonPos.x -= 12;
		//		buttonPos.y -= 13;

		//if( NULL != pButton )
		//	pButton->SetOffsetPos( buttonPos );
		//D3DXVECTOR2 staticPos( buttonPos.x + 5, buttonPos.y );

		if( pUnitSlot != NULL)
			pUnitSlot->SetPos(buttonPos);
		if ( j == UNIT_SELECT_UI_MAX_UNIT_NUM - 1 )
			bAllSlotOccupiedInAPage = true;
		//		pStaticUnitInfo->SetOffsetPos( staticPos );
// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
// 		if ( pUnit == m_pSelectUnit )
// 		{
// 			CKTDGUIButton* pCharButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 0 ) );
// 			pCharButton->SetDownStateAtNormal( true );
// 		}
// 
// #endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	}



	dummyPos = m_pDLGUnitSelectFront->GetDummyPos( j );
	D3DXVECTOR2 buttonPos = D3DXVECTOR2( dummyPos.x, dummyPos.y );
	/*
	D3DXVECTOR2 buttonPos = D3DXVECTOR2( 0, 0 );
	if ( i == 0 )
	{
	buttonPos = D3DXVECTOR2( 378, 123 );
	}
	else if ( i == 1 )
	{
	buttonPos = D3DXVECTOR2( 586, 123 );
	}
	else if ( i == 2 )
	{
	buttonPos = D3DXVECTOR2( 795, 123 );
	}
	else if ( i == 3 )
	{
	buttonPos = D3DXVECTOR2( 378, 404 );
	}
	else if ( i == 4 )
	{
	buttonPos = D3DXVECTOR2( 586, 404 );
	}
	else if ( i == 5 )
	{
	buttonPos = D3DXVECTOR2( 795, 404 );
	}
	*/

	if( false == bAllSlotOccupiedInAPage &&
		g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
	{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		if ( TODAY_CREATE_UNIT_MAX_NUMBER > m_iCreateUnitTodayCount )
			pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Make_New.lua" );
		else
			pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Dont_Make_New.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Make.lua" );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pUnitSlot );
		pUnitSlot->SetShowEnable(true, true);
		m_vecUnitSlot.push_back(pUnitSlot);	

		//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_UI_Character_Selection_Slot.lua" );
		pButton = (CKTDGUIButton*)pUnitSlot->GetControl( L"character2" );
		pButton->SetName( L"EmptySlot" );
		pUnitSlot->SetPos(buttonPos);

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		for ( j = j + 1; j < UNIT_SELECT_UI_MAX_UNIT_NUM; j++ )
		{
			pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Empty_Slot_New.lua" );
			dummyPos = m_pDLGUnitSelectFront->GetDummyPos( j );
			D3DXVECTOR2 buttonPos = D3DXVECTOR2( dummyPos.x, dummyPos.y );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pUnitSlot );
			pUnitSlot->SetShowEnable(true, true);
			m_vecUnitSlot.push_back(pUnitSlot);	
			pUnitSlot->SetPos(buttonPos);
		}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	}

	PlayDownAnim();
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	g_pKTDXApp->SkipFrame();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	
#endif
}
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
bool CX2StateServerSelect::ClearUnitButton( bool bIsClearUnitViewer_ /* = true */ )
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
bool CX2StateServerSelect::ClearUnitButton()
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		CKTDGUIButton* pSelectUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 0 ) );
		CKTDGUIButton* pCreateUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 1 ) );
		CKTDGUIButton* pNoCreateUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 2 ) );
		CKTDGUIButton* pEmptyUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 3 ) );
		CKTDGUIButton* pFullUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 4 ) );
		if ( NULL != pSelectUnitButton && NULL != pCreateUnitButton && NULL != pNoCreateUnitButton && NULL != pEmptyUnitButton && NULL != pFullUnitButton )
		{	
			pSelectUnitButton->SetShowEnable( false, false );
			pSelectUnitButton->SetName ( L"ButtonSelectUnit");
			pCreateUnitButton->SetShowEnable( false, false );
			pNoCreateUnitButton->SetShowEnable( false, false );
			pEmptyUnitButton->SetShowEnable( false, false );
			pFullUnitButton->SetShowEnable( false, false );
		}

		CKTDGUIStatic* pStaticEmblem = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"UnitEmblem" ) );
		if ( NULL != pStaticEmblem )
			pStaticEmblem->SetShow ( false );

		CKTDGUIStatic* pStaticLV = (CKTDGUIStatic*)pUnitSlot->GetControl( L"lv" );
		if ( NULL != pStaticLV )
			pStaticLV->SetShow ( false );

		CKTDGUIStatic* pStaticLVNum = (CKTDGUIStatic*)pUnitSlot->GetControl( L"StaticUnitSelectStringLVNum" );
		if ( NULL != pStaticLVNum )
			pStaticLVNum->SetShow ( false );

		CKTDGUIStatic* pStaticID = (CKTDGUIStatic*)pUnitSlot->GetControl( L"StaticUnitSelectStringID" );
		if ( NULL != pStaticID )
			pStaticID->SetShow ( false );

		CKTDGUIStatic* pStaticLastPlace = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"StaticUnitSelectStringID_LastPlace" ) );
		if ( NULL != pStaticLastPlace )
			pStaticLastPlace->SetShow ( false );

		CKTDGUIStatic* pStaticUnitFace = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"UnitFace" ) );
		if ( NULL != pStaticUnitFace )
			pStaticUnitFace->SetShow ( false );

		CKTDGUIStatic* pStaticChangeNickEmblem = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"ChangeNick" ) );
		if ( NULL != pStaticChangeNickEmblem )
		{
			pStaticChangeNickEmblem->SetShowEnable ( false, false );
		}

		CKTDGUIStatic* pStaticChangeNickMessage = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"standby" ) );
		if ( NULL != pStaticChangeNickMessage )
		{
			pStaticChangeNickMessage->SetShowEnable ( false, false );
		}

	}


	if ( true == bIsClearUnitViewer_ )
	{
		ClearUnitViewerList();
	}

	m_iChannelNowPage = 1;
	m_iChannelMaxPage = 1;

#else // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		SAFE_DELETE_DIALOG(pUnitSlot);
	}
	m_vecUnitSlot.clear();
	ClearUnitViewerList();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh	

	return true;
}

void CX2StateServerSelect::ClearBeforeUnit( CX2Unit* pUnit )
{
	if( pUnit == NULL )
		return;
	
	m_pSelectUnit = NULL;
}

void CX2StateServerSelect::UnitButtonUp( CX2Unit* pUnit )
{
	if ( m_pSelectUnit == pUnit )
	{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		wstring tempNickName = m_pSelectUnit->GetNickName();
		if ( tempNickName.find( GET_STRING(STR_ID_29404) ) != -1 || 
			 tempNickName.find( L"__DELETED__") != -1 )
		{
			//�г����� ���� ĳ���ʹ� �г����� ���� �Է��϶�� �˾�â�� ����.
			OpenRenameMsgBox( true );
		}
		else
		{
#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT //��â��
			if( NULL != m_pSelectUnit && m_pSelectUnit->GetType() == CX2Unit::UT_ADD )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 275), GET_STRING( STR_ID_29409 ), this, -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2 ( 0, -130 ),  L"UI_PopUp_Negative_01.ogg" );

				return;
			}
#endif //SERV_ELISIS_PREVIOUS_SIS_EVENT
#ifdef SERV_UNIT_WAIT_DELETE
			CX2Unit::UnitData* pUnitData = &(pUnit->AccessUnitData());
			if( pUnitData->m_bDeleted != true)
			{
#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
				m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2558 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
				Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
				m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY ); 
				m_bPlayDownAnim = false;

#ifdef ADD_PLAY_SOUND //��â��
				if( m_pSelectUnit != NULL )
					PlaySoundSelect( m_pSelectUnit->GetType(), SPT_SERVER_SELECT );
#endif //ADD_PLAY_SOUND
			}
			else
			{
				m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 275), GET_STRING( STR_ID_30401 ), this, -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2 (0, -130), L"UI_PopUp_Negative_01.ogg" );
			}
#else
#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
			m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2558 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
			Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY ); 
			m_bPlayDownAnim = false;

#ifdef ADD_PLAY_SOUND //��â��
			if( m_pSelectUnit != NULL )
				PlaySoundSelect( m_pSelectUnit->GetType(), SPT_SERVER_SELECT );
#endif //ADD_PLAY_SOUND
#endif SERV_UNIT_WAIT_DELETE
		}
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		
		Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

		return;
	}
#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	if ( m_pSelectUnit != NULL )
	{
		CX2UnitViewerUI* pUnitViewer = GetUnitViewer( m_pSelectUnit );
		if ( pUnitViewer != NULL )
		{
			pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}
	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	ClearBeforeUnit( m_pSelectUnit );



	m_pSelectUnit = pUnit;

/*
	CKTDGUIStatic* pStaticCharBG = (CKTDGUIStatic*)m_pDLGUnitSelectBack->GetControl(L"CharBG" );
	if ( pStaticCharBG != NULL )
	{
		for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
		{
			if ( pStaticCharBG->GetPicture(i) != NULL )
				pStaticCharBG->GetPicture(i)->SetShow( false );
		}

		// note!! ���� �߰��Ǹ� ����, Ȥ�� unit_type����
		switch( pUnit->GetClass() )
		{
		case CX2Unit::UC_ELSWORD_SWORDMAN:
		case CX2Unit::UC_ELSWORD_KNIGHT:
		case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			{
				if ( pStaticCharBG->GetPicture(0) != NULL )
					pStaticCharBG->GetPicture(0)->SetShow( true );
			}
			break;

		case CX2Unit::UC_LIRE_ELVEN_RANGER:
		case CX2Unit::UC_LIRE_COMBAT_RANGER:
		case CX2Unit::UC_LIRE_SNIPING_RANGER:
			{
				if ( pStaticCharBG->GetPicture(2) != NULL )
					pStaticCharBG->GetPicture(2)->SetShow( true );
			}
			break;

		case CX2Unit::UC_ARME_VIOLET_MAGE:
		case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		case CX2Unit::UC_ARME_DARK_MAGICIAN:
			{
				if ( pStaticCharBG->GetPicture(1) != NULL )
					pStaticCharBG->GetPicture(1)->SetShow( true );
			}
			break;

		case CX2Unit::UC_RAVEN_FIGHTER:
			{
				if ( pStaticCharBG->GetPicture(3) != NULL )
					pStaticCharBG->GetPicture(3)->SetShow( true );
			}
			break;

		}
	}
*/

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	CreateServerSelectUnitViewerUI ( pUnit );
	SetRightSideFlagColor ( GetUnitKeyColorByUnitType ( pUnit->GetType() ) );
	SetShowSelectUnitInfo ( pUnit );	
	ChangeSelectUnitStandMeshInstance ( pUnit->GetType() );
	SetBackgroundImageBySelectUnitType ( pUnit->GetType() );
#ifdef ADD_PLAY_SOUND //��â��
	PlaySoundSelect( pUnit->GetType(), SPT_UNIT_SELECT );
#endif //ADD_PLAY_SOUND

#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	CX2UnitViewerUI* pUnitViewer = GetUnitViewer( pUnit );
	if ( pUnitViewer != NULL  )
	{
		pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
#ifdef SERV_UNIT_WAIT_DELETE //2012.03.07 lygan_������ // ĳ���ͺ� ���� ���ÿ� �ִϸ��̼� ����
		if(m_pBeforeSelectUnit != NULL)
		{
			m_pBeforeSelectUnit->AccessUnitData().m_bDeletedMotionPlay = false;
		}
		m_pBeforeSelectUnit = pUnit;
#endif //SERV_UNIT_WAIT_DELETE
	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	/*
	D3DXVECTOR3 unitSize = D3DXVECTOR3( 1.6f, 1.6f, 1.6f );


	//	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUnitSelectBack->GetControl( L"StaticUnit_Select_UnitInfo" );
	//	pStatic->SetOffsetPos( D3DXVECTOR2( -200, 200 ) );
	//	pStatic->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR(1,1,1,1), 0.1f, true );

	switch( pUnit->GetClass() )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
	//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_ElSword_BG.lua" );

	//pStatic->GetPicture(0)->SetShow( true );

	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

	m_pUnitViewerUI->SetUnit( pUnit );
	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -70, 200 ) );
	m_pUnitViewerUI->GetMatrix().Scale( unitSize );
	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
	m_pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );

	break;

	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Lire_BG.lua" );
	//pStatic->GetPicture(2)->SetShow( true );

	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

	m_pUnitViewerUI->SetUnit( pUnit );
	m_pUnitViewerUI->GetMatrix().Move( D3DXVECTOR3( -250, -70, 200 ) );
	m_pUnitViewerUI->GetMatrix().Scale( unitSize );
	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
	m_pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );
	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );

	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
	break;

	case CX2Unit::UC_ARME_VIOLET_MAGE:
	//m_pDLGUnitSelectBack->OpenScriptFile( L"DLG_Unit_Select_Arme_BG.lua" );
	//pStatic->GetPicture(1)->SetShow( true );
	m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
	pUnitViewerUI->SetLightPos( 300, 300, -500 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

	m_pUnitViewerUI->SetUnit( pUnit );
	m_pUnitViewerUI->GetMatrix().Move( D3DXVECTOR3( -250, -70, 200 ) );
	m_pUnitViewerUI->GetMatrix().Scale( unitSize );
	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
	m_pUnitViewerUI->PlayAnim( L"LobbyFullDown", CKTDGXSkinAnim::XAP_ONE_WAIT );

	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );

	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
	break;

	}




	g_pKTDXApp->SkipFrame();
	*/
}


#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_REQ( UidType unitUID /* = -1 */, int iUnitLevel /* = -1 */ )
#else // FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_REQ( UidType unitUID, int iUnitLevel )
#endif // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )

{
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
	if ( unitUID < 0 )
	{
		unitUID = m_pSelectUnit->GetUID();
	}
	if ( iUnitLevel < 0 )
	{
		iUnitLevel = m_pSelectUnit->GetUnitData().m_Level;
	}
#endif // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
	if( false == g_pMain->GetIsValideCheckKomScript() )
	{
		g_pMain->SetQuitType( NetError::ERR_CLIENT_QUIT_03 );
		g_pMain->SendQuitMsgToServer();
	}
#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT // ĳ���� ���� �� �� ������ ����
	if( NULL != m_pSelectUnit )
	{
		if( CX2Unit::UC_ELESIS_KNIGHT == static_cast<CX2Unit::UNIT_CLASS>(m_pSelectUnit->GetUnitData().m_UnitClass) )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_25374 ), g_pMain->GetNowState() );
			return true;
		}
	}
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_SECOND_SECURITY
	if(m_bEnableUnitSelectBySecondSecurity == false)
	{
		return true;
	}
#endif SERV_SECOND_SECURITY

	// ü�� ���̵� ���� 
	if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
		iUnitLevel >= 20 )
	{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_40 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		return true;
	}

	KEGS_SELECT_UNIT_REQ	kPacket;
	kPacket.m_iUnitUID		= unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_SELECT_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SELECT_UNIT_ACK, 30.0f );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_709 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_709 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

#ifdef NEW_EMBLEM_MANAGER
	CX2EmblemManager::GetInstance()->InitEmbelmPlayInfo(true);
#endif // NEW_EMBLEM_MANAGER

#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
	// ���� ���� ��, �����ߴ� ä���� ���̵� �ʱ�ȭ �Ѵ�.
	g_pInstanceData->ResetConnectedChannelID();
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
	return true;
}

#ifdef SERV_SELECT_UNIT_PACKET_DIVISION		/// ĳ���� ���� ��Ŷ ����

/// ������ ����, �κ��丮, ��ų
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_1_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_1_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if( NULL != g_pMain && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
		battleFieldManager.SetBattleFieldIdWhereIam( 0 );

		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_kUnitInfo.m_nUnitUID );

			if( NULL != pUnit )
			{
#ifdef LIST_SORT_AT_CHARACTER_SELECT
				g_pData->GetMyUser()->SortFirstByUID(pUnit->GetUID());
#endif LIST_SORT_AT_CHARACTER_SELECT

				g_pData->GetMyUser()->SelectUnit( pUnit );
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				g_pData->GetItemStatCalculator().SetUnitData( pUnit->GetType(), pUnit->GetClass() );
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef REFORM_SKILL_NOTE_UI
				if( pUnit->GetClass() != CX2SkillNoteManager::GetInstance()->GetUnitClass() )
				{
					CX2SkillNoteManager::GetInstance()->SetUnitClass( pUnit->GetClass() );
					CX2SkillNoteManager::GetInstance()->ResetMemoList();
				}
#endif // REFORM_SKILL_NOTE_UI

				//{{ ������ // 2012-04-06 // �̺�Ʈ �޾ƿ��� �� Ÿ�̸� �ʱ�ȭ ��ų �� �ֵ��� ����
#ifdef FIX_EVENT_TIMER	
				pUnit->m_mapEventData.clear();
				pUnit->SetReceivedGuestUserReward( true );		
#endif //FIX_EVENT_TIMER
				//}} ������ // 2012-04-06 // �̺�Ʈ �޾ƿ��� �� Ÿ�̸� �ʱ�ȭ ��ų �� �ֵ��� ����

				pUnit->Reset( kEvent.m_kUnitInfo, true );

				//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
				pUnit->ResetInventorySize( kEvent.m_mapInventorySlotSize );
				pUnit->ResetInventoryItems( kEvent.m_mapItem ); // �������� ��� ���� Reset
#else	PC_BANG_WORK
				//kimhc // 2008-08-04 // m_mapInventorySlotSize���� Ű���� ST_BANK�� 
				// value�� ���ཽ���� ������ �̰� �� ������� ȸ�� ����� �����Ѵ�.
				pUnit->ResetInventory( kEvent.m_mapInventorySlotSize, kEvent.m_mapItem );
#endif	PC_BANG_WORK
				//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

				pUnit->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );

				//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
				pUnit->SetAutoResStoneNum( kEvent.m_iNumAutoPaymentResStone );
#endif	//	AUTO_PAYMENT
				//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��


#ifdef ADDED_RELATIONSHIP_SYSTEM
				// �� ������ ��ų ���� ������ ���� �־�� �մϴ�!!
				if ( NULL != g_pData->GetMessenger() )
				{
					g_pData->GetMessenger()->ResetRelationDummyUnit ();
				}

				CX2RelationshipManager * pRelationshipManager = g_pData->ResetRelationshipManager();
				CX2RelationshipEffectManager * pRelationshipEffectManager = g_pData->ResetRelationshipEffectManager();

				if ( NULL != pRelationshipManager )
				{
					CX2RelationshipManager::RelationshipInfo * pMyRelationshipInfo = pRelationshipManager->GetMyRelationshipInfo();
					if ( NULL != pMyRelationshipInfo )
					{
						pMyRelationshipInfo->ClearRelationInfo();
						pMyRelationshipInfo->m_eRelationStateType				= (SEnum::RELATIONSHIP_TYPE) kEvent.m_kRelationshipInfo.m_cRelationshipType;
						pMyRelationshipInfo->m_iRelationTargetUserUid			= kEvent.m_kRelationshipInfo.m_iOtherUnitUID;

						CTime ctCurrentTime( g_pData->GetServerCurrentTime() );		
						CTime ctCoupleStartTime = static_cast<CTime> ( kEvent.m_kRelationshipInfo.m_tDate );
						CTimeSpan ctsGapTime = ctCurrentTime - ctCoupleStartTime;


						pMyRelationshipInfo->m_ctsDurationDay					= ctsGapTime;
						pMyRelationshipInfo->m_wstrNotifyLoveMessage			= kEvent.m_kRelationshipInfo.m_wstrLoveWord;
						pMyRelationshipInfo->m_wstrRelationTargetUserNickname	= kEvent.m_kRelationshipInfo.m_wstrOtherNickName;
					}
				}

				// �ӽ� �ڵ�!!
				pRelationshipEffectManager->SetMyRelationEffect ( 1 );
				

#endif // ADDED_RELATIONSHIP_SYSTEM

			
				// skill list
				{
			#ifdef SKILL_PAGE_SYSTEM //JHKang
					pUnit->AccessUnitData().m_UserSkillTree.SetUsingPage( 
						static_cast<USHORT>( kEvent.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber ) );
					pUnit->AccessUnitData().m_UserSkillTree.SetOpenedPage( 
						static_cast<USHORT>( kEvent.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable ) );
					pUnit->AccessUnitData().m_UserSkillTree.SetSelectPage( 
						static_cast<USHORT>( kEvent.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber ) );

					pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredSkill( kEvent.m_vecUserSkillPageData );
					pUnit->AccessUnitData().m_UserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );
					pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill,
						kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );

				#ifdef ADDED_RELATIONSHIP_SYSTEM
					if ( NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
					{
						int iSkillLevel = static_cast<int>(g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType);
						pUnit->AccessUnitData().m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_ETC_WS_COMMON_LOVE,
							iSkillLevel, 0, pUnit->AccessUnitData().m_UserSkillTree.GetUsingPage() );
					}
				#endif // ADDED_RELATIONSHIP_SYSTEM
			#else //SKILL_PAGE_SYSTEM
					//pUnit->AccessUnitData().m_UserSkillTree.SetUnitClass( (int) kEvent.m_kUnitInfo.m_cUnitClass );
					pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredSkill( kEvent.m_vecSkillAcquired );
					pUnit->AccessUnitData().m_UserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );
					pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill, kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );
					// 		pUnit->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( (CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_kUnitInfo.m_UnitSkillData.m_cSkillSlotBExpirationState );
					// 		pUnit->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate );

				#ifdef ADDED_RELATIONSHIP_SYSTEM
					if ( NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
					{
						int iSkillLevel = static_cast<int>(g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType);
						pUnit->AccessUnitData().m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_ETC_WS_COMMON_LOVE, iSkillLevel, 0 );
					}
				#endif // ADDED_RELATIONSHIP_SYSTEM
			#endif //SKILL_PAGE_SYSTEM
#ifdef SERV_SKILL_NOTE		
					pUnit->SetSkillNote( kEvent.m_mapSkillNote, kEvent.m_cSkillNoteMaxPageNum );
					pUnit->AccessUnitData().m_UserSkillTree.SetEqipSkillMemo( kEvent.m_kUnitInfo.m_UnitSkillData.m_vecSkillNote );
#endif

//#ifdef TITLE_SYSTEM
#ifdef SERV_TITLE_DATA_SIZE
					pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_iTitleID;
#else
					pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_sTitleID;
#endif
//#endif TITLE_SYSTEM
				}

				if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
				{
					g_pData->GetUIManager()->CreateUISkillTree();
					g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
				}

				#ifdef GUILD_SKILL
				//{{  [11/11/2009 : oasis907 ]

				// �ӽ�: �������Լ� GuildUserSkillTree ������ �޾Ҵ� ����
				/*
				// skill list
				pUnit->AccessUnitData().m_UserSkillTree.SetGuildClass(0);
				pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredGuildSkill( kEvent.m_vecSkillAcquired ); // �Լ� �ӽ� ����
				*/		
		
				//pUnit->AccessUnitData().m_GuildUserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );


				// �Լ� �ӽ� ����
				//pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill, kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );

				//pUnit->AccessUnitData().m_GuildUserSkillTree.SetSkillSlotBExpirationState( (CX2GuildUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_kUnitInfo.m_UnitSkillData.m_cSkillSlotBExpirationState );
				//pUnit->AccessUnitData().m_GuildUserSkillTree.SetSkillSlotBEndDateString( kEvent.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate );

				/*
				if( g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() != NULL)
				{
					g_pData->GetGuildManager()->GetUIGuild()->CreateUIGuildSkillTree();
					g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->InitSkillTreeUI();
				}
				//}}  [11/11/2009 : oasis907 ]
				*/
#endif GUILD_SKILL

#ifdef SERV_TITLE_DATA_SIZE
				pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_iTitleID;
#else
				pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_sTitleID;
#endif

				//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
				pUnit->ResetInventorySize( kEvent.m_mapInventorySlotSize );
				pUnit->ResetInventoryItems( kEvent.m_mapItem ); // �������� ��� ���� Reset
#else	PC_BANG_WORK
				//kimhc // 2008-08-04 // m_mapInventorySlotSize���� Ű���� ST_BANK�� 
				// value�� ���ཽ���� ������ �̰� �� ������� ȸ�� ����� �����Ѵ�.
				pUnit->ResetInventory( kEvent.m_mapInventorySlotSize, kEvent.m_mapItem );
#endif	PC_BANG_WORK
				//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

				pUnit->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );

				//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
				pUnit->SetAutoResStoneNum( kEvent.m_iNumAutoPaymentResStone );
#endif	//	AUTO_PAYMENT
				//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
				pUnit->SetNewUnitTradeBlockDay( kEvent.m_iNewUnitTradeBlockDay );
				pUnit->SetNewUnitTradeBlockUnitClass( kEvent.m_iNewUnitTradeBlockUnitClass );
				pUnit->SetUnitCreateDate( kEvent.m_wstrUnitCreateDate );

				if( true == kEvent.m_bIsNewUnitTradeBlock )
				{
					CTime tReleaseTradeBlockTime;
					KncUtil::ConvertStringToCTime( kEvent.m_wstrUnitCreateDate, tReleaseTradeBlockTime );
					tReleaseTradeBlockTime += CTimeSpan( kEvent.m_iNewUnitTradeBlockDay, 0, 0, 0 );
					CTime tCurTime = g_pData->GetServerCurrentTime();            

					if( kEvent.m_iNewUnitTradeBlockDay != 0 && tCurTime < tReleaseTradeBlockTime )
					{
						wstring wstrMonth = static_cast< const wchar_t * >( tReleaseTradeBlockTime.Format( _T("%m") ) );
						wstring wstrDay = static_cast< const wchar_t * >( tReleaseTradeBlockTime.Format( _T("%d") ) );
						wstring wstrHour = static_cast< const wchar_t * >( tReleaseTradeBlockTime.Format( _T("%H") ) );
						wstring wstrMinute = static_cast< const wchar_t * >( tReleaseTradeBlockTime.Format( _T("%M") ) );
						wstring wstrNotice = GET_REPLACED_STRING( ( STR_ID_14586 , "LLLL", wstrMonth, wstrDay , wstrHour , wstrMinute ) );

						pUnit->SetTradeBlockUnit( true );
						pUnit->SetTradeBlockChatMsg( wstrNotice );
					}
					else if( pUnit->GetNewUnitTradeBlockUnitClass() != 0 
						&& pUnit->GetClass() < pUnit->GetNewUnitTradeBlockUnitClass() )
					{
						// ���� �Ŀ� �ŷ������մϴ�.
						wstring wstrNotice =  GET_STRING( STR_ID_24523 );

						pUnit->SetTradeBlockUnit( true );
						pUnit->SetTradeBlockChatMsg( wstrNotice );
					}
				}
#endif SERV_NEW_UNIT_TRADE_LIMIT
			}
		}

		if( g_pData->GetUIManager() != NULL )
		{
			g_pData->GetUIManager()->CreateUISkillTree();
			g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
		}

		
		//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
		g_pData->GetServerProtocol()->SendID( EGS_GET_MY_INVENTORY_REQ );
		g_pMain->AddServerPacket( EGS_GET_MY_INVENTORY_ACK  );
#else	PC_BANG_WORK
		// ü�� ���̵� ���� 
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
			kEvent.m_bIsRecommend == false && m_bCheckRecommend == true )
		{
			OpenRecommendMsgBox( true );
		}
		else
		{

			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_PartyTalkBoxInfo.m_iPartyUID = 0;

			if(g_pMain->GetIsPlayingTutorial() == true)
			{
				// Ʃ�丮�� ���Կ�û
				Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
			}
			else
			{
				// �������� ���Կ�û
				Handler_EGS_STATE_CHANGE_FIELD_REQ();
			}
		}
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		if ( NULL != g_pData &&
			 NULL != g_pData->GetMyUser() &&
			 NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			/// ��ų �߰� ���� ȿ�� ����
			g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelBySocket();
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013

		return true;
		}

	return false;
}

/// ����Ʈ
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_2_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_2_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pMain && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		//{{ kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_SECRET_HELL
		if( NULL != g_pInstanceData )
			g_pInstanceData->SetPreCheckTimeForDailyQuest( g_pData->GetServerCurrentTime() );
#endif SERV_SECRET_HELL
		//}} kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)

		if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
			g_pData->GetQuestManager()->SetUnitQuest( kEvent.m_vecQuest, kEvent.m_vecCompletQuest );

		//{{ kimhc // GiveUpForbiddenQuest()�� EGS_GET_MY_INVENTORY_ACK�� �̵���
#ifndef	SERV_EPIC_QUEST
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
#endif	SERV_EPIC_QUEST
		//}} kimhc // GiveUpForbiddenQuest()�� EGS_GET_MY_INVENTORY_ACK�� �̵���

#ifdef	SERV_EPIC_QUEST
		// oasis907 : ����� [2010.7.21] // ������Ʈ â ���� �ʱ�ȭ
		if(	g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIQuestNew() != NULL)
			g_pData->GetUIManager()->GetUIQuestNew()->InitUserShowQuickQuestDLG(true);
#endif	SERV_EPIC_QUEST

		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
			if( NULL != pUnit && NULL != g_pMain && NULL != g_pMain->GetInformerManager() && NULL != g_pMain->GetLVUpEventMgr() )
			{
				g_pMain->GetInformerManager()->Reset();
				g_pMain->GetLVUpEventMgr()->Reset( pUnit->GetUnitData().m_Level );
			}

			if ( g_pData->GetCashShop() != NULL )
				g_pData->GetCashShop()->ResetUnitViewer( pUnit );

			g_pMain->ResetCrashReport( L"" );
		}

#ifdef SERV_RANDOM_DAY_QUEST
		if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
			g_pData->GetQuestManager()->SetRandomQuestInfo( kEvent.m_mapRandomQuestList );
#endif SERV_RANDOM_DAY_QUEST

#ifdef REFORM_QUEST
		if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
		{
			g_pData->GetUIManager()->GetUIQuestNew()->SetOpenEpicCategory();
			g_pData->GetUIManager()->GetUIQuestNew()->InitNewQuestList();
		}
#endif //REFORM_QUEST

		return true;
	}

	return false;
}

/// Ÿ��Ʋ, ��
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_3_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_3_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{

		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

			if( NULL != pUnit )
			{
//#ifdef TITLE_SYSTEM
				if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
				{
					g_pData->GetTitleManager()->InitRecord();
					g_pData->GetTitleManager()->AttachTitle(pUnit->GetUnitData().m_iTitleId);

					g_pData->GetTitleManager()->TakeTitle(kEvent.m_vecTitle);
					g_pData->GetTitleManager()->UpdateMission(kEvent.m_vecMission);
				}                
//#endif

#ifdef SERV_PET_SYSTEM

				pUnit->SetSummonPetUid( kEvent.m_iSummonedPetUID );
				//if( g_pData != NULL && g_pData->GetPetManager() != NULL )
				//	g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( kEvent.m_iSummonedPetUID );
				if( kEvent.m_vecPetList.size() > 0 )
					pUnit->SetFullPetInfo(kEvent.m_vecPetList[0]);

				if( g_pData->GetPetManager() != NULL )
				{
					g_pData->GetPetManager()->AddMyPetList( kEvent.m_vecCanEvolutionPetList );
					g_pData->GetPetManager()->AddNeverSummonPetList( kEvent.m_vecNeverSummonPetList, kEvent.m_iSummonedPetUID );

					// oasis907 : ����� [2010.9.7] // 
					if( kEvent.m_vecPetList.size() > 0 )
					{
						KPetInfo kPetInfo = kEvent.m_vecPetList[0];
						int EvolutionStep = kPetInfo.m_cEvolutionStep;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
						int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_iPetID, EvolutionStep);
#else //SERV_PETID_DATA_TYPE_CHANGE
						int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_cPetID, EvolutionStep);
#endif //SERV_PETID_DATA_TYPE_CHANGE

						pUnit->AccessInventory().SetItemMaxNum(CX2Inventory::ST_PET, iInvenSlotSize);

						// oasis907 : ����� [2010.9.8] // �ڵ� ���� �ֱ� ���
						if( g_pData->GetUIManager()->GetUIInventory() != NULL)
						{
							g_pData->GetUIManager()->GetUIInventory()->SetAutoFeed(kPetInfo.m_bAutoFeed);
						}
						//}}
					}
					//}}
#ifdef SERV_PERIOD_PET
					else
					{
						g_pData->GetMyUser()->GetSelectUnit()->ClearPetInfo();
					}
#endif SERV_PERIOD_PET
				}

#endif SERV_PET_SYSTEM
			}
		}

		return true;
	}

	return false;
}

/// ���� �̺�Ʈ, ����, ����, ��ŷ
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_4_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_4_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pMain && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		/// Ʃ�丮��
		if( NULL != g_pMain->GetTutorSystem() )
		{
			g_pMain->GetTutorSystem()->ResetTutorStudent( kEvent.m_vecTutorialDBUnitInfo );
		}

		if ( m_bReserveEnterTutorial == true )
		{
			g_pMain->SetIsPlayingTutorial( true );
		}

		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

			if( NULL != pUnit )
			{
				/// ���� ����Ʈ				
				pUnit->SetBlackList( kEvent.m_vecChatBlackList );

				CX2GageManager* pGageManager = CX2GageManager::GetInstance();

				if( NULL != pGageManager )
				{
					pGageManager->CreateMyGageUI( pUnit->GetClass(), pUnit->GetUID() );
					pGageManager->InitMySkillCoolTimeList( pUnit->GetUnitData().m_UserSkillTree );
					//pGageManager->InitMyQuickSlotCoolTimeList( pUnit->GetInventory().GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ) );


					ASSERT( !kEvent.m_kGamePlayStatus.m_vecGamePlayStatus.empty() );
					if ( !kEvent.m_kGamePlayStatus.m_vecGamePlayStatus.empty() )
					{
						pGageManager->SetMyPlayStatusFromPacket( kEvent.m_kGamePlayStatus.m_vecGamePlayStatus[0] );
						pGageManager->UpdateNowHpPercent();
						pGageManager->UpdateNowMpPercent();
					}

					pUnit->AccessUnitData().m_UserSkillTree.InitSkillCoolTimeFromGageManager( pGageManager->GetSkillCoolTimeList() );

					if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
						g_pData->GetUIManager()->GetUIQuickSlot()->InitMyQuickSlotCoolTimeFromGageManager( pGageManager->GetQuickSlotCoolTimeList() );
					//		const float fResultMaxMp = pUnit->ResetMaxMp( ONE_CHARGE * 3.0f );
					//		pGageManager->SetMaxMp( pUnit->GetUID(), fResultMaxMp );
					// GameStat���� ���� �ؾ� ��, �׸��� �ѹ��� ���� �� �ִ� ����...
					// 		pGageManager->SetMaxHp( pUnit->GetUID(), static_cast<const float>( kEvent.m_kUnitInfo.m_kStat.m_iBaseHP ) );
					// 		pGageManager->SetNowHp( pUnit->GetUID(), static_cast<const float>( kEvent.m_kUnitInfo.m_kStat.m_iBaseHP ) );	
					// 		pGageManager->SetMaxSoul( pUnit->GetUID(), 100.0f );

					// �޸��������� Ȯ���� �˾��� ����. -- ���� ������ �ʴ� �̺�Ʈ
					//switch(kEvent.m_iSelectUnitEvent)
					//{
					//case 0:	// ����
					//	break;
					//case 1:	// �Ĺ�����
					//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2634 ), g_pMain->GetNowState() );
					//	break;
					//case 2:	// �Ĺ��ε� �������� ����
					//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2635 ), g_pMain->GetNowState() );
					//	break;
					//case 3:	// �������� ����
					//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2636 ), g_pMain->GetNowState() );
					//	break;
					//case 4: // �ű� ������
					//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2640 ), g_pMain->GetNowState() );
					//	break;
					//default:
					//	break;
					//}
				}
			}
		}

		{
			g_pMain->GetGameOption().SetDenyInviteGuild( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyInviteGuild );
			g_pMain->GetGameOption().SetDenyPersonalTrade( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade );
			g_pMain->GetGameOption().SetDenyFriendship( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip );
			g_pMain->GetGameOption().SetDenyParty( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty );

			g_pMain->GetGameOption().SetRefuseParty( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty );
			g_pMain->GetGameOption().SetRefuseFriend( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip );
			g_pMain->GetGameOption().SetRefusePersonalTrade( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade );
			//{{ kimhc // 2009-10-12 // ��� �ʴ� �ź� �߰�
#ifdef	GUILD_MANAGEMENT
			g_pMain->GetGameOption().SetRefuseGuildInvitation( static_cast< CX2GameOption::BlackListDenyState >( kEvent.m_kDenyOptions.m_cDenyInviteGuild ) );
#endif	GUILD_MANAGEMENT
			//}} kimhc // 2009-10-12 // ��� �ʴ� �ź� �߰�

#ifdef ADDED_RELATIONSHIP_SYSTEM	/// Ŀ�ý�û �ź� �߰�
			g_pMain->GetGameOption().SetRefuseRequestCouple( static_cast< CX2GameOption::BlackListDenyState >( kEvent.m_kDenyOptions.m_cDenyRequestCouple ) );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // ����ȯ	/// ���� �ʴ� �ź� �߰�
			g_pMain->GetGameOption().SetRefuseInvitePVP( static_cast< CX2GameOption::BlackListDenyState >( kEvent.m_kDenyOptions.m_cDenyInvitePracticePVP ) );
#endif // FIX_INVITE_PVP_PLAYER

		}

		// ģ��ä��â �����丮 �ʱ�ȭ �� ģ������Ʈ �ʱ� ����
		if(g_pData != NULL && g_pData->GetMessenger() != NULL)
		{
			g_pData->GetMessenger()->ClearChatSessionList();
			g_pData->GetMessenger()->Reset();		
			g_pData->GetMessenger()->ResetHistory();
#ifdef NEW_MESSENGER
			g_pData->GetMessenger()->SetFriendList( kEvent.m_kMessengerInfo );
#else		
			g_pData->GetMessenger()->SyncFriendList();
#endif NEW_MESSENGER
		}

#ifdef ADDED_RELATIONSHIP_SYSTEM
		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetUIRelationship() )
		{
			g_pData->GetRelationshipManager()->GetUIRelationship()->GetFriendListForInvitingWeddingUser();
		}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		g_pInstanceData->SetRecruit( !kEvent.m_vecRecruiterUnitInfo.empty() );
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

		return true;
	}

	return false;
}

/// ETC, 1ȸ�� �̺�Ʈ
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_5_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_5_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pMain && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		if( g_pInstanceData != NULL )
		{
			g_pInstanceData->Set4ThAnnivEventRewardInfo( kEvent.m_vec4ThAnnivEventRewardInfo );
		}		
#endif //SERV_4TH_ANNIVERSARY_EVENT

#ifdef SERV_PSHOP_AGENCY
		if( g_pInstanceData != NULL )
		{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			g_pInstanceData->SetPShopAgencyInfo( kEvent.m_kPShopAgencyInfo.m_bIsPShopOpen, 
												 kEvent.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate,
												 static_cast<SEnum::AGENCY_SHOP_TYPE>( kEvent.m_kPShopAgencyInfo.m_cShopType ) );
	#else // SERV_UPGRADE_TRADE_SYSTEM
			g_pInstanceData->SetPShopAgencyInfo( kEvent.m_kPShopAgencyInfo.m_bIsPShopOpen, kEvent.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate );
	#endif // SERV_UPGRADE_TRADE_SYSTEM
		}
#endif

#ifdef NEW_HENIR_TEST
		if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
			g_pData->GetMyUser()->GetSelectUnit()->SetHenirRewardCountInfo( kEvent.m_PacketHenirRewardCount );
#endif NEW_HENIR_TEST

#ifdef SERV_EVENT_MONEY
		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			g_pData->GetMyUser()->SetUserAngelsFeather(kEvent.m_iEventMoney);
		}
		if( NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI() )
		{
			g_pInstanceData->GetMiniMapUI()->UpdateAngelsFeatherCount(kEvent.m_iEventMoney);
		}
#endif //SERV_EVENT_MONEY

		//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifndef	CHRISTMAS_TREE
		// ������ �̺�Ʈ �ð� ǥ��
		g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().restart();
		g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( true );
		g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventTime( -1.f );

		std::map< int, float >::iterator it;
		for( it = kEvent.m_mapEventTime.begin() ; it != kEvent.m_mapEventTime.end(); it++ )
		{
			g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( false );
			if( it->second > g_pData->GetMyUser()->GetSelectUnit()->GetMarketingEventTime() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventID( it->first );
				g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventTime( it->second );
			}
		}

#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ

#ifdef SERV_LOCAL_RANKING_SYSTEM
		g_pData->GetProfileManager()->SetIsProfileSave(kEvent.m_bLocalRankingUser);
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		/// ĳ������ ù �����̸�
		if ( 1 == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level &&
			0 == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP )
		{
			m_bReserveEnterTutorial = true;
			g_pMain->SetIsPlayingTutorial( true );	/// Ʃ�丮�� ����
		}
#endif // SERV_ARA_FIRST_SELECT_TUTORIAL

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		if ( 1 == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level &&
			0 == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP 
			&& 2 == kEvent.m_iConnectExperienceAck  // �ű� �����ε�, 7���� �������� ���.
			)
		{
			// � ������ 7�� �Ŀ� ���� �� �ִٴ� �޽��� ���
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_25378 ), g_pMain->GetNowState() ); //STR_ID_25378
		}
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetGateOfDarknessSupportEventTime( kEvent.m_iGateOfDarknessSupportEventTime );
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetCouple( kEvent.m_bCouple );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserNickname( kEvent.m_wstrRelationTargetUserNickname );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetRelationTargetUserUID( kEvent.m_iRelationTargetUserUid );
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_ELESIS_UPDATE_EVENT
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().SetNoteViewCount( kEvent.m_iNoteViewCount );
#endif SERV_ELESIS_UPDATE_EVENT

		return true;
	}
	
	return false;
}

#endif SERV_SELECT_UNIT_PACKET_DIVISION

/// �ɸ��� ���ý� ��Ŷ�� ������� �ʴ� ���� ���� �� m_iOK ó��
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );


	if( NULL != g_pMain && false == g_pMain->DeleteServerPacket( EGS_SELECT_UNIT_ACK ) )
	{
		StateLog( L"invalid sel char ack" );
		return false;
	}


	if( NULL != g_pMain && g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION

#ifdef ADD_SERVER_GROUP
		if( NULL != g_pData && NULL != g_pInstanceData && g_pInstanceData->GetServerGroupID() != g_pInstanceData->GetServerGroupIDScript() )
		{
			g_pData->LoadItemManager();
			g_pInstanceData->SetServerGroupIDScript( g_pInstanceData->GetServerGroupID() );
		}		
#endif

#ifdef POSTBOX
		if( NULL != g_pMain->GetPostBox() )
			g_pMain->GetPostBox()->InitPostBox();
#endif

		if( NULL != g_pMain->GetPartyUI() && NULL != g_pMain->GetPartyUI()->GetLocalMapUI() )
		{
			g_pMain->GetPartyUI()->GetLocalMapUI()->ResetLocalMapID();
		}
		// Ŭ������ �����߿��� ���� ���� ������ ������ �ڵ� ����
		if( NULL != g_pData && NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData() &&
			NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() &&
			NULL != g_pData->GetDungeonManager())
		{
			g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID = g_pData->GetMyUser()->GetSelectUnit()->GetLastClearDungeonID();

			// Ŭ������ ������ �ϳ��� ���� ��쿡 ���� ���� ���� normal�� �������ش�
			switch( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
			{
			case -1:
			case SEnum::DI_NONE:
			case SEnum::DI_EL_FOREST_GATE_NORMAL:
				//{{ kimhc // 2010-07-09 // �纥���� �������� �߰�
			case SEnum::DI_EL_FOREST_NORTH_NORMAL:
			case SEnum::DI_EL_FOREST_WEST_NORMAL:
			case SEnum::DI_EL_FOREST_HELL_NORMAL:
			case SEnum::DI_RUBEN_SECRET_COMMON:
				//}} kimc	// 2010-07-09 // �纥���� �������� �߰�
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;
				} break;
			}

			//{{ kimhc // 2010.5.2 // ��д��� �۾�(��Ƽ����Ʈ)
#ifdef SERV_SECRET_HELL
			switch ( ( g_pData->GetDungeonManager()->GetDungeonType( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) )
			{
			case CX2Dungeon::DT_SECRET:
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
				} break;

			default:
				break;
			}
#endif SERV_SECRET_HELL
			//}} kimhc // 2010.5.2 // ��д��� �۾�(��Ƽ����Ʈ)
		}

		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

			if( NULL != pUnit )
			{
				wstringstream wstreamNickNameAndUid;
				wstreamNickNameAndUid << L"UserUID: " << pUnit->GetOwnerUserUID(); 
				wstreamNickNameAndUid << L" NickName: " << pUnit->GetNickName();
				wstreamNickNameAndUid << L" UnitUID: " << pUnit->GetUID();
				StateLog( wstreamNickNameAndUid.str().c_str() );

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
				if ( g_pMain->GetNexonLoginMessenger() == true )
				{
					wstring tempNickName = CX2Community::ConvertFullName( pUnit->GetNickName() );
					switch( g_pMain->GetPublisher() )
					{
#ifdef _NEXON_KR_
					case CX2Main::XP_NEXON_KOREA:
						{
#ifdef _USE_NEXON_MSG_INHOUSE // ������ ĳ������ serialnum�� �ϵ��ڵ��������� �ؽ��α����Ѵ�.
							pUnit->AccessUnitData().m_iNMKSerialNum = 52890;	// "������" ĳ����
							//pUnit->AccessUnitData().m_iNMKSerialNum = 3712941;	// "�Ƕ˽��̺�" ĳ����
							//pUnit->AccessUnitData().m_iNMKSerialNum = 1928754;	// "��" ĳ����
#endif

							if( CNMCOClientObject::GetInstance().LoginVirtual( tempNickName.c_str(), (_UInt32_)pUnit->GetUnitData().m_iNMKSerialNum, GET_STRING( STR_ID_0 ) ) == FALSE )						
							{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING( STR_ID_710 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_710 ), g_pMain->GetNowState() );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����								
							}
							else
							{
								g_pMain->SetNexonVirtualLogin( true );							
							}

							CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)pUnit->GetUnitData().m_UnitClass << 24 ) | (UINT32)pUnit->GetUnitData().m_Level );



							// �г��� �ٲ� ���Ķ�� �г����� �ٲ���ٰ� �ؽ��޽����� �̺�Ʈ ����
							if( true == m_bJustChangedNickName )
							{
								m_bJustChangedNickName = false;

								NMVirtualKey nmVirtualKey;
								nmVirtualKey.uGameCode		= NEXON_KOREA_ELSWORD_GAMECODE;
								nmVirtualKey.uVirtualIDCode = (UINT32) pUnit->GetUnitData().m_iNMKSerialNum;

								if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
								{
									if ( CNMCOClientObject::GetInstance().ChangeNickname( nmVirtualKey, tempNickName.c_str() ) == FALSE )
									{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_711 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_711 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
									}
								}
							}
#ifdef SEND_URL_TO_NEXON
							// game start
							static bool bSended = false;
							if( bSended == false )
							{
#ifdef SEND_NEXON_WISE_LOG_BY_URL // ����ȯ
								const CX2User::UserData& kUserData = g_pData->GetMyUser()->GetUserData();

								if ( NULL != g_pMain->GetBrowserWrapper() )
								{
									if( false == g_pMain->GetBrowserWrapper()->IsClosed() )
										g_pMain->GetBrowserWrapper()->CloseBrowser();

									RECT		rcWebPage;
									char		buf[255]	= {0,};
									std::string strURL		= "http://elsword.nexon.com/wiselog/gamestart.aspx";				/// �α� URL
									::WideCharToMultiByte( CP_ACP, 0, kUserData.userID.c_str(), -1, buf, 255, NULL, NULL );	/// wstring�� string ���� ��ȯ

									std::string strNXID		= buf;					/// �ؽ� ���̵�
									UidType		uidNXUID	= kUserData.userUID;	/// �ؽ� UID

									g_pMain->GetBrowserWrapper()->Create( rcWebPage, g_pKTDXApp->GetHWND() );

									if ( true == g_pData->GetMyUser()->GetFirstCreateCharacter() )		/// �ű� ���� ( ������ �ɸ��͸� �������� ���� ���� )
										strURL += "?GRU=true";	/// �ű� ������ ��� �߰��Ǵ� ����

									StringCchPrintfA( buf, 255, "&NXID=%s&OID=%lld", strNXID.c_str(), uidNXUID );	/// �ؽ� ���̵�� UID�� ����

									strURL += buf;				/// ��Ʈ�� ����

									g_pMain->GetBrowserWrapper()->Navigate( strURL.c_str(), NULL );					/// �������� URL�� ����
								}
#endif // SEND_NEXON_WISE_LOG_BY_URL

								NMCOHelpers::WriteToWiselog( L"http://elsword.nexon.com/wiselog/gamestart.aspx" );
								bSended = true;
							}						
#endif
						}
						break;
#endif _NEXON_KR_
					}
				}
#endif NEW_MESSENGER
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP
			}
		}

		Handler_EGS_CHECK_BALANCE_REQ();

		if ( g_pInstanceData != NULL )
		{
			g_pInstanceData->SetShowCSPandSlotBDate( true );
#ifdef SERV_HACKING_TOOL_LIST
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
            g_pInstanceData->ReSetHackList_MainThread();
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
			g_pInstanceData->SetChangeHackList(true);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
#endif
		}

		//{{ ����� : [2009/11/3] //	ä�� �ڽ� �ʱ�ȭ
		if( g_pChatBox != NULL )
		{
			g_pChatBox->ClearAllChatLog();
			g_pChatBox->ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
		}

#ifdef SKILL_SLOT_UI_TYPE_B
		{
			g_pMain->GetGameOption().SetIsSkillUITypeA( g_pMain->GetGameOption().GetIsSkillUITypeA(), false );				
		}
#endif //SKILL_SLOT_UI_TYPE_B

#else  SERV_SELECT_UNIT_PACKET_DIVISION
		CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
		battleFieldManager.SetBattleFieldIdWhereIam( 0 );

		// �޸��������� Ȯ���� �˾��� ����. -- ���� ������ �ʴ� �̺�Ʈ
		//switch(kEvent.m_iSelectUnitEvent)
		//{
		//case 0:	// ����
		//	break;
		//case 1:	// �Ĺ�����
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2634 ), g_pMain->GetNowState() );
		//	break;
		//case 2:	// �Ĺ��ε� �������� ����
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2635 ), g_pMain->GetNowState() );
		//	break;
		//case 3:	// �������� ����
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2636 ), g_pMain->GetNowState() );
		//	break;
		//case 4: // �ű� ������
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2640 ), g_pMain->GetNowState() );
		//	break;
		//default:
		//	break;
		//}

		if( NULL != g_pMain->GetTutorSystem() )
		{
			g_pMain->GetTutorSystem()->ResetTutorStudent( kEvent.m_vecTutorialDBUnitInfo );
		}

		if ( m_bReserveEnterTutorial == true )
		{
			g_pMain->SetIsPlayingTutorial( true );
		}

		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_kUnitInfo.m_nUnitUID );
		g_pData->GetMyUser()->SelectUnit( pUnit );
		pUnit->SetBlackList( kEvent.m_vecChatBlackList ); 
//{{ ������ // 2012-04-06 // �̺�Ʈ �޾ƿ��� �� Ÿ�̸� �ʱ�ȭ ��ų �� �ֵ��� ����
#ifdef FIX_EVENT_TIMER	
		pUnit->m_mapEventData.clear();
		pUnit->SetReceivedGuestUserReward( true );		
#endif //FIX_EVENT_TIMER
//}} ������ // 2012-04-06 // �̺�Ʈ �޾ƿ��� �� Ÿ�̸� �ʱ�ȭ ��ų �� �ֵ��� ����


#ifdef ADD_SERVER_GROUP
		if( g_pData != NULL && g_pInstanceData->GetServerGroupID() != g_pInstanceData->GetServerGroupIDScript() )
		{
			g_pData->LoadItemManager();
			g_pInstanceData->SetServerGroupIDScript( g_pInstanceData->GetServerGroupID() );
		}		
#endif


#ifdef POSTBOX
		g_pMain->GetPostBox()->InitPostBox();
#endif


		
		pUnit->Reset( kEvent.m_kUnitInfo, true );

		//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
		pUnit->ResetInventorySize( kEvent.m_mapInventorySlotSize );
		pUnit->ResetInventoryItems( kEvent.m_mapItem ); // �������� ��� ���� Reset
#else	PC_BANG_WORK
		//kimhc // 2008-08-04 // m_mapInventorySlotSize���� Ű���� ST_BANK�� 
		// value�� ���ཽ���� ������ �̰� �� ������� ȸ�� ����� �����Ѵ�.
		pUnit->ResetInventory( kEvent.m_mapInventorySlotSize, kEvent.m_mapItem );
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

		pUnit->SetResurrectionStoneNum( kEvent.m_iNumResurrectionStone );

		//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
		pUnit->SetAutoResStoneNum( kEvent.m_iNumAutoPaymentResStone );
#endif	//	AUTO_PAYMENT
		//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��

		// skill list
		//pUnit->AccessUnitData().m_UserSkillTree.SetUnitClass( (int) kEvent.m_kUnitInfo.m_cUnitClass );
		pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredSkill( kEvent.m_vecSkillAcquired );
		pUnit->AccessUnitData().m_UserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );
 		pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill, kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );
// 		pUnit->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( (CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_kUnitInfo.m_UnitSkillData.m_cSkillSlotBExpirationState );
// 		pUnit->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate );


		if( g_pData->GetUIManager() != NULL)
		{
			g_pData->GetUIManager()->CreateUISkillTree();
			g_pData->GetUIManager()->GetUISkillTree()->InitSkillTreeUI();
		}
#ifdef GUILD_SKILL
		//{{  [11/11/2009 : oasis907 ]

		// �ӽ�: �������Լ� GuildUserSkillTree ������ �޾Ҵ� ����
/*
		// skill list
		pUnit->AccessUnitData().m_UserSkillTree.SetGuildClass(0);
		pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredGuildSkill( kEvent.m_vecSkillAcquired ); // �Լ� �ӽ� ����
*/		
		
		//pUnit->AccessUnitData().m_GuildUserSkillTree.SetUnsealedSkill( kEvent.m_vecSkillUnsealed );


		// �Լ� �ӽ� ����
		//pUnit->AccessUnitData().m_UserSkillTree.SetEquippedSkill( kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill, kEvent.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB );

		//pUnit->AccessUnitData().m_GuildUserSkillTree.SetSkillSlotBExpirationState( (CX2GuildUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_kUnitInfo.m_UnitSkillData.m_cSkillSlotBExpirationState );
		//pUnit->AccessUnitData().m_GuildUserSkillTree.SetSkillSlotBEndDateString( kEvent.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate );

/*
		if( g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() != NULL)
		{
			g_pData->GetGuildManager()->GetUIGuild()->CreateUIGuildSkillTree();
			g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->InitSkillTreeUI();
		}
		//}}  [11/11/2009 : oasis907 ]
*/
#endif GUILD_SKILL

#ifdef SERV_SKILL_NOTE		
		pUnit->SetSkillNote( kEvent.m_mapSkillNote, kEvent.m_cSkillNoteMaxPageNum );
		pUnit->AccessUnitData().m_UserSkillTree.SetEqipSkillMemo( kEvent.m_kUnitInfo.m_UnitSkillData.m_vecSkillNote );
#endif

//#ifdef TITLE_SYSTEM
#ifdef SERV_TITLE_DATA_SIZE
		pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_iTitleID;
#else
		pUnit->AccessUnitData().m_iTitleId = kEvent.m_kUnitInfo.m_sTitleID;
#endif

		if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
		{
			g_pData->GetTitleManager()->InitRecord();
			g_pData->GetTitleManager()->AttachTitle(pUnit->GetUnitData().m_iTitleId);

			g_pData->GetTitleManager()->TakeTitle(kEvent.m_vecTitle);     
			g_pData->GetTitleManager()->UpdateMission(kEvent.m_vecMission);
		}                
//#endif

#ifdef SERV_PET_SYSTEM

		pUnit->SetSummonPetUid( kEvent.m_iSummonedPetUID );
		//if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		//	g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( kEvent.m_iSummonedPetUID );
		if( kEvent.m_vecPetList.size() > 0 )
			pUnit->SetFullPetInfo(kEvent.m_vecPetList[0]);
		
		if( g_pData != NULL && g_pData->GetPetManager() != NULL )
		{
			g_pData->GetPetManager()->AddMyPetList( kEvent.m_vecCanEvolutionPetList );
			g_pData->GetPetManager()->AddNeverSummonPetList( kEvent.m_vecNeverSummonPetList, kEvent.m_iSummonedPetUID );

		}
		
		// oasis907 : ����� [2010.9.7] // 
		if( kEvent.m_vecPetList.size() > 0 )
		{
			KPetInfo kPetInfo = kEvent.m_vecPetList[0];
			int EvolutionStep = kPetInfo.m_cEvolutionStep;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_iPetID, EvolutionStep);
#else //SERV_PETID_DATA_TYPE_CHANGE
			int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_cPetID, EvolutionStep);
#endif //SERV_PETID_DATA_TYPE_CHANGE

			pUnit->AccessInventory().SetItemMaxNum(CX2Inventory::ST_PET, iInvenSlotSize);
		
		

			// oasis907 : ����� [2010.9.8] // �ڵ� ���� �ֱ� ���
			if( g_pData->GetUIManager()->GetUIInventory() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->SetAutoFeed(kPetInfo.m_bAutoFeed);
			}
			//}}
		}
		//}}
#endif

		g_pMain->GetGameOption().SetDenyInviteGuild( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyInviteGuild );				
		g_pMain->GetGameOption().SetDenyPersonalTrade( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade );
		g_pMain->GetGameOption().SetDenyFriendship( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip );
		g_pMain->GetGameOption().SetDenyParty( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty );

		g_pMain->GetGameOption().SetRefuseParty( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyParty );
		g_pMain->GetGameOption().SetRefuseFriend( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyFriendShip );
		g_pMain->GetGameOption().SetRefusePersonalTrade( (CX2GameOption::BlackListDenyState) kEvent.m_kDenyOptions.m_cDenyPersonalTrade );
		//{{ kimhc // 2009-10-12 // ��� �ʴ� �ź� �߰�
#ifdef	GUILD_MANAGEMENT
		g_pMain->GetGameOption().SetRefuseGuildInvitation( static_cast< CX2GameOption::BlackListDenyState >( kEvent.m_kDenyOptions.m_cDenyInviteGuild ) );
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-10-12 // ��� �ʴ� �ź� �߰�

		if( NULL != g_pMain && NULL != g_pMain->GetPartyUI() && NULL != g_pMain->GetPartyUI()->GetLocalMapUI() )
		{
			g_pMain->GetPartyUI()->GetLocalMapUI()->ResetLocalMapID();
		}
		// Ŭ������ �����߿��� ���� ���� ������ ������ �ڵ� ����
		if( NULL != g_pData->GetPartyManager() )
		{
			g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID = g_pData->GetMyUser()->GetSelectUnit()->GetLastClearDungeonID();

			// Ŭ������ ������ �ϳ��� ���� ��쿡 ���� ���� ���� normal�� �������ش�
			switch( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID )
			{
			case -1:
			case SEnum::DI_NONE:
			case SEnum::DI_EL_FOREST_GATE_NORMAL:
				//{{ kimhc // 2010-07-09 // �纥���� �������� �߰�
			case SEnum::DI_EL_FOREST_NORTH_NORMAL:
			case SEnum::DI_EL_FOREST_WEST_NORMAL:
			case SEnum::DI_EL_FOREST_HELL_NORMAL:
			case SEnum::DI_RUBEN_SECRET_COMMON:
				//}} kimc	// 2010-07-09 // �纥���� �������� �߰�
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID = SEnum::DI_RUBEN_EL_TREE_NORMAL;
				} break;
			}

		//{{ kimhc // 2010.5.2 // ��д��� �۾�(��Ƽ����Ʈ)
		#ifdef SERV_SECRET_HELL
			switch ( ( g_pData->GetDungeonManager()->GetDungeonType( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) )
			{
			case CX2Dungeon::DT_SECRET:
				{
					g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode = CX2Dungeon::DM_SECRET_NORMAL;
				} break;

			default:
				break;
			}
		#endif SERV_SECRET_HELL
		//}} kimhc // 2010.5.2 // ��д��� �۾�(��Ƽ����Ʈ)
		}

	//{{ kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_SECRET_HELL
		g_pInstanceData->SetPreCheckTimeForDailyQuest( g_pData->GetServerCurrentTime() );
#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.12 // ��д��� �۾�(��������Ʈ �ý���)

		g_pData->GetQuestManager()->SetUnitQuest( kEvent.m_vecQuest, kEvent.m_vecCompletQuest );
		
		//{{ kimhc // GiveUpForbiddenQuest()�� EGS_GET_MY_INVENTORY_ACK�� �̵���
	#ifndef	SERV_EPIC_QUEST
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	#endif	SERV_EPIC_QUEST
		//}} kimhc // GiveUpForbiddenQuest()�� EGS_GET_MY_INVENTORY_ACK�� �̵���


#ifdef	SERV_EPIC_QUEST
		// oasis907 : ����� [2010.7.21] // ������Ʈ â ���� �ʱ�ȭ
		if(g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetUIQuestNew() != NULL)
		{
			g_pData->GetUIManager()->GetUIQuestNew()->InitUserShowQuickQuestDLG(true);
		}
#endif	SERV_EPIC_QUEST

		g_pMain->GetInformerManager()->Reset();
		g_pMain->GetLVUpEventMgr()->Reset( pUnit->GetUnitData().m_Level );

		if ( g_pData->GetCashShop() != NULL )
			g_pData->GetCashShop()->ResetUnitViewer( pUnit );

		g_pMain->ResetCrashReport( L"" );


#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
		if ( g_pMain->GetNexonLoginMessenger() == true )
		{
			wstring tempNickName = CX2Community::ConvertFullName( pUnit->GetNickName() );
			switch( g_pMain->GetPublisher() )
			{
#ifdef _NEXON_KR_
				case CX2Main::XP_NEXON_KOREA:
					{
#ifdef _USE_NEXON_MSG_INHOUSE // ������ ĳ������ serialnum�� �ϵ��ڵ��������� �ؽ��α����Ѵ�.
						pUnit->AccessUnitData().m_iNMKSerialNum = 52890;	// "������" ĳ����
						//pUnit->AccessUnitData().m_iNMKSerialNum = 3712941;	// "�Ƕ˽��̺�" ĳ����
						//pUnit->AccessUnitData().m_iNMKSerialNum = 1928754;	// "��" ĳ����
#endif

						if( CNMCOClientObject::GetInstance().LoginVirtual( tempNickName.c_str(), (_UInt32_)pUnit->GetUnitData().m_iNMKSerialNum, GET_STRING( STR_ID_0 ) ) == FALSE )						
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_710 ), g_pMain->GetNowState() );
						}
						else
						{
							g_pMain->SetNexonVirtualLogin( true );							
						}

						CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)pUnit->GetUnitData().m_UnitClass << 24 ) | (UINT32)pUnit->GetUnitData().m_Level );



						// �г��� �ٲ� ���Ķ�� �г����� �ٲ���ٰ� �ؽ��޽����� �̺�Ʈ ����
						if( true == m_bJustChangedNickName )
						{
							m_bJustChangedNickName = false;

							NMVirtualKey nmVirtualKey;
							nmVirtualKey.uGameCode		= NEXON_KOREA_ELSWORD_GAMECODE;
							nmVirtualKey.uVirtualIDCode = (UINT32) pUnit->GetUnitData().m_iNMKSerialNum;

							if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
							{
								if ( CNMCOClientObject::GetInstance().ChangeNickname( nmVirtualKey, tempNickName.c_str() ) == FALSE )
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_711 ), this );
								}
							}
						}

#ifdef SEND_URL_TO_NEXON
						// game start
						static bool bSended = false;
						if( bSended == false )
						{
							NMCOHelpers::WriteToWiselog( L"http://elsword.nexon.com/wiselog/gamestart.aspx" );
							bSended = true;
						}						
#endif
					}
					break;
#endif _NEXON_KR_
			}
		}
#endif NEW_MESSENGER
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP


		Handler_EGS_CHECK_BALANCE_REQ();


		//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
		g_pData->GetServerProtocol()->SendID( EGS_GET_MY_INVENTORY_REQ );
		g_pMain->AddServerPacket( EGS_GET_MY_INVENTORY_ACK  );
#else	PC_BANG_WORK
		
		// ü�� ���̵� ���� 
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
			kEvent.m_bIsRecommend == false && m_bCheckRecommend == true )
		{
#ifdef SERV_NO_GAMEINVITE
			Handler_EGS_RECOMMEND_USER_REQ( L"", true );
#else SERV_NO_GAMEINVITE
			OpenRecommendMsgBox( true );
#endif SERV_NO_GAMEINVITE
		}
		else
		{

			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_PartyTalkBoxInfo.m_iPartyUID = 0;

			if(g_pMain->GetIsPlayingTutorial() == true)
			{
				// Ʃ�丮�� ���Կ�û
				Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
			}
			else
			{
				// �������� ���Կ�û
				Handler_EGS_STATE_CHANGE_FIELD_REQ();
			}
		}

#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

#ifdef SERV_PSHOP_AGENCY
		if( g_pInstanceData != NULL )
		{
			g_pInstanceData->SetPShopAgencyInfo( kEvent.m_kPShopAgencyInfo.m_bIsPShopOpen, kEvent.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate );
		}
#endif


		//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifndef	CHRISTMAS_TREE
		// ������ �̺�Ʈ �ð� ǥ��
		g_pData->GetMyUser()->GetSelectUnit()->GetTimerGuestUser().restart();
		g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( true );
		g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventTime( -1.f );

		std::map< int, float >::iterator it;
		for( it = kEvent.m_mapEventTime.begin() ; it != kEvent.m_mapEventTime.end(); it++ )
		{
			g_pData->GetMyUser()->GetSelectUnit()->SetReceivedGuestUserReward( false );
			if( it->second > g_pData->GetMyUser()->GetSelectUnit()->GetMarketingEventTime() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventID( it->first );
				g_pData->GetMyUser()->GetSelectUnit()->SetMarketingEventTime( it->second );
			}
		}

#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ


		// ģ��ä��â �����丮 �ʱ�ȭ �� ģ������Ʈ �ʱ� ����
		if(g_pData != NULL && g_pData->GetMessenger() != NULL)
		{
			g_pData->GetMessenger()->ClearChatSessionList();
			g_pData->GetMessenger()->Reset();		
			g_pData->GetMessenger()->ResetHistory();
#ifdef NEW_MESSENGER
			g_pData->GetMessenger()->SetFriendList( kEvent.m_kMessengerInfo );
#else		
			g_pData->GetMessenger()->SyncFriendList();
#endif NEW_MESSENGER
		}




		if ( g_pInstanceData != NULL )
		{
			g_pInstanceData->SetShowCSPandSlotBDate( true );
#ifdef SERV_HACKING_TOOL_LIST
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
            g_pInstanceData->ReSetHackList_MainThread();
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
			g_pInstanceData->SetChangeHackList(true);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
#endif
		}


		//{{ ����� : [2009/11/3] //	ä�� �ڽ� �ʱ�ȭ
		if( g_pChatBox != NULL )
		{
			g_pChatBox->ClearAllChatLog();
			g_pChatBox->ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
		}
#ifdef NEW_HENIR_TEST
		g_pData->GetMyUser()->GetSelectUnit()->SetHenirRewardCountInfo( kEvent.m_PacketHenirRewardCount );
#endif NEW_HENIR_TEST

#ifdef SERV_RANDOM_DAY_QUEST
		g_pData->GetQuestManager()->SetRandomQuestInfo( kEvent.m_mapRandomQuestList );
#endif SERV_RANDOM_DAY_QUEST

		CX2GageManager* pGageManager = CX2GageManager::GetInstance();
		pGageManager->CreateMyGageUI( pUnit->GetClass(), pUnit->GetUID() );
		pGageManager->InitMySkillCoolTimeList( pUnit->GetUnitData().m_UserSkillTree );
		//pGageManager->InitMyQuickSlotCoolTimeList( pUnit->GetInventory().GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ) );

		
		ASSERT( !kEvent.m_kGamePlayStatus.m_vecGamePlayStatus.empty() );
		if ( !kEvent.m_kGamePlayStatus.m_vecGamePlayStatus.empty() )
		{
			pGageManager->SetMyPlayStatusFromPacket( kEvent.m_kGamePlayStatus.m_vecGamePlayStatus[0] );
			pGageManager->UpdateNowHpPercent();
			pGageManager->UpdateNowMpPercent();
		}

		pUnit->AccessUnitData().m_UserSkillTree.InitSkillCoolTimeFromGageManager( pGageManager->GetSkillCoolTimeList() );
		g_pData->GetUIManager()->GetUIQuickSlot()->InitMyQuickSlotCoolTimeFromGageManager( pGageManager->GetQuickSlotCoolTimeList() );
//		const float fResultMaxMp = pUnit->ResetMaxMp( ONE_CHARGE * 3.0f );
//		pGageManager->SetMaxMp( pUnit->GetUID(), fResultMaxMp );
		// GameStat���� ���� �ؾ� ��, �׸��� �ѹ��� ���� �� �ִ� ����...
// 		pGageManager->SetMaxHp( pUnit->GetUID(), static_cast<const float>( kEvent.m_kUnitInfo.m_kStat.m_iBaseHP ) );
// 		pGageManager->SetNowHp( pUnit->GetUID(), static_cast<const float>( kEvent.m_kUnitInfo.m_kStat.m_iBaseHP ) );	
// 		pGageManager->SetMaxSoul( pUnit->GetUID(), 100.0f );

#ifdef REFORM_QUEST
		if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
		{
			g_pData->GetUIManager()->GetUIQuestNew()->SetOpenEpicCategory();
			g_pData->GetUIManager()->GetUIQuestNew()->InitNewQuestList();
		}
#endif //REFORM_QUEST

#ifdef SERV_EVENT_MONEY
		if( NULL != g_pData && NULL != g_pData->GetMyUser() )
		{
			g_pData->GetMyUser()->SetUserAngelsFeather(kEvent.m_iAngelFeather);
		}
		if( NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI() )
		{
			g_pInstanceData->GetMiniMapUI()->UpdateAngelsFeatherCount(kEvent.m_iAngelFeather);
		}
#endif //SERV_EVENT_MONEY

#ifdef SKILL_SLOT_UI_TYPE_B
		if( NULL != g_pMain )
		{
			g_pMain->GetGameOption().SetIsSkillUITypeA( g_pMain->GetGameOption().GetIsSkillUITypeA(), false );				
		}
#endif //SKILL_SLOT_UI_TYPE_B

#endif SERV_SELECT_UNIT_PACKET_DIVISION
	}
	else if( kEvent.m_iOK == NetError::ERR_SELECT_UNIT_06 )
	{
		//�г����� ���� ĳ���ʹ� �г����� ���� �Է��϶�� �˾�â�� ����.
		OpenRenameMsgBox( true );
	}
	//{{ 2009. 4. 29  ������	�߸��� ���¿��� SELECT_UNIT_REQ�� ������ ���ƿ��� ACK ���� �ڵ忡 ���� ó��
	else if( kEvent.m_iOK == NetError::ERR_WRONG_STATE_00 )
	{
		// ����ó��?
		return true;
	}
	//}} 2009. 4. 29  ������
	else
	{
		if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
		{
			g_pMain->GetStateAutoChanger().EndStateChange();
		}
	}


	m_bJustChangedNickName = false;


	// kimhc // 2010-02-03 // PC���� ��� PC�� ���� �����尡 �ǵ��� ����
	if ( g_pInstanceData->m_bFirstSelect == false &&
		 g_pData->GetMyUser() != NULL &&
		 g_pData->GetMyUser()->GetIsPCRoom() == true
#ifdef SERV_PC_BANG_TYPE
		 && g_pData->GetPremiumBuffManager() != NULL
		 && g_pData->GetPremiumBuffManager()->IsPcBangAvatar() == true
#endif SERV_PC_BANG_TYPE
		 )
	{
		if ( g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetUIInventory() != NULL )
			g_pData->GetUIManager()->GetUIInventory()->SetSortType( CX2Inventory::ST_PC_BANG );
	}

	g_pInstanceData->m_bFirstSelect = true;


	//{{ kimhc // 2009-01-12 // ���̾� �������� ��õ�� ����Ʈ ���̴� ���
#ifdef	VIEW_REFEREE_LIST
	if ( g_pData != NULL &&
		 g_pData->GetMessenger() != NULL )
	{
		switch ( g_pInstanceData->GetServerGroupID() )
		{
		case SGI_GAIA:
			{
				g_pData->GetMessenger()->SetRefereeUI( true );
			}
			break;

		default:
			{
				g_pData->GetMessenger()->SetRefereeUI( false );
			}
			break;
		}
	}
#endif	VIEW_REFEREE_LIST
	//}} kimhc // 2009-01-12 // ���̾� �������� ��õ�� ����Ʈ ���̴� ���

#ifdef SERV_RECRUIT_EVENT_BASE
	if( g_pData != NULL &&
		g_pData->GetMessenger() != NULL )
	{
		g_pData->GetMessenger()->SetRecruitUI( true );
	}
#endif SERV_RECRUIT_EVENT_BASE

	//�긮��Ʈ UI�� ���� �ʰ� �� ���� ���θ� �ľ� �ϱ� ���� �߰�
	if( NULL != g_pData && NULL != g_pData->GetUIManager() )
		g_pData->GetUIManager()->CreatePetList();

#ifdef RIDING_SYSTEM
	if( NULL != g_pData )
	{
		if( NULL != g_pData->GetUIManager() &&
			NULL != g_pData->GetUIManager()->GetUIPetList() )
			g_pData->GetUIManager()->GetUIPetList()->Handler_EGS_GET_RIDING_PET_LIST_REQ(0);
	}
#endif //RIDING_SYSTEM

#ifdef SERV_2012_PVP_SEASON2_EVENT
	if( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )		/// �ɸ��� Ŭ���� ���� �������� ������ �ð� ����������, �ش� ���� ����
		g_pData->GetPremiumBuffManager()->SetLocalTimeOfGloryBuff();
#endif SERV_2012_PVP_SEASON2_EVENT

#ifdef SERV_LOCAL_RANKING_SYSTEM
	g_pData->LoadAreaData();
#endif //SERV_LOCAL_RANKING_SYSTEM


#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData &&
		 NULL != g_pData->GetMessenger() &&
		 NULL != g_pData->GetRelationshipManager() &&
		 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		SEnum::RELATIONSHIP_TYPE eType = g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType;
		g_pData->GetMessenger()->SetRelationshipUI( eType );
	}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FESTIVAL_UI
#ifndef NEW_EMBLEM_MANAGER
	if( NULL != g_pMain && NULL != g_pData )
	{
		CKTDGParticleSystem::CParticleEventSequenceHandle SeqFestivalUI = 
			g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"UI_Festival_StartEffect", 512.f+20.f, 284.f+60.f+20.f, 0.f, 9999.f, 9999.f, -1, 1, -1.0f, true, 2.5f, false );

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != SeqFestivalUI )
		{
			CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( SeqFestivalUI );
			g_pMain->SetFestivalSeq( SeqFestivalUI );
			if( pParticle != NULL )
			{
				pParticle->SetOverUI( true );
			};
		}
	}
#endif //NEW_EMBLEM_MANAGER
#endif //FESTIVAL_UI


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID> ( m_pSelectUnit->GetServerGroupID() ) );
	g_pData->GetMyUser()->AccessUserData().maxUnitCount = g_pInstanceData->GetMaxUserUnitManyInServer( m_pSelectUnit->GetServerGroupID() );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	return true;
}

//bool CX2StateUnitSelect::SelectUnitTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"������ ������ �����ϴ�\n������ ������ �� �����ϴ�", this );
//return true;
//}

bool CX2StateServerSelect::Handler_EGS_DELETE_UNIT_REQ()
{
	if( m_pSelectUnit == NULL )
		return true;

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	KEGS_ENTRY_POINT_DELETE_UNIT_REQ kPacket;
	kPacket.m_iUnitUID		= m_pSelectUnit->GetUID();
	kPacket.m_iServerGroup	= m_pSelectUnit->GetServerGroupID();	
	g_pData->GetServerProtocol()->SendPacket( EGS_ENTRY_POINT_DELETE_UNIT_REQ, kPacket );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	KEGS_DELETE_UNIT_REQ kPacket;
	kPacket.m_iUnitUID		= m_pSelectUnit->GetUID();
	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_UNIT_REQ, kPacket );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

	g_pMain->AddServerPacket( EGS_DELETE_UNIT_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_712 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_712 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

	return true;
}

bool CX2StateServerSelect::Handler_EGS_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_DELETE_UNIT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_iUnitUID );
			if ( pUnit == NULL )
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_713 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_713 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				
				return true;
			}

#ifdef _NEXON_KR_
			CNMCOClientObject::GetInstance().RemoveMyVirtualUser( g_pMain->GetNMVirtualKey() );
#endif // _NEXON_KR_

#ifdef SERV_UNIT_WAIT_DELETE
			pUnit->AccessUnitData().m_bDeleted = true;
			pUnit->AccessUnitData().m_trDelAbleDate = kEvent.m_tDelAbleDate;

			CTime tDeleAbleDate(kEvent.m_tDelAbleDate);
			std::wstring wstrYear = (CStringW)( tDeleAbleDate.Format(_T( "%Y" )) );
			std::wstring wstrMonth = (CStringW)( tDeleAbleDate.Format(_T( "%m" )) );
			std::wstring wstrDay = (CStringW)( tDeleAbleDate.Format(_T( "%d" )) );
			std::wstring wstrHour = (CStringW)( tDeleAbleDate.Format(_T( "%H" )) );
			std::wstring wstrMinute = (CStringW)( tDeleAbleDate.Format(_T( "%M" )) );
			std::wstring wstrMessage = GET_REPLACED_STRING( ( STR_ID_16103, "SSSSS", wstrDay.c_str(), wstrMonth.c_str(), wstrYear.c_str(), wstrHour.c_str(), wstrMinute.c_str()));			
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMessage.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
			// ChangeUnitButtonInfo();
#else SERV_UNIT_WAIT_DELETE
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305,375), GET_STRING( STR_ID_714 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
			// ChangeUnitButtonInfo();
#endif SERV_UNIT_WAIT_DELETE
			
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����						int iTopCharacterIndex = (m_NowPage - 1) * UNIT_SELECT_UI_MAX_UNIT_NUM;
			if ( g_pData->GetMyUser()->GetUnitNum() <= 0 )
			{
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ), L"DLG_UI_Selection_MessageBox_No_Button.lua", L"UI_PopUp_Positive_01.ogg"  );
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
			}
			else
			{
#ifdef SERV_UNIT_WAIT_DELETE
				m_bMaintainNowUnitButton = true;
				m_iSelectUnitUID = pUnit->GetUID();		// ĳ���� ����Ʈ�� ���� ���� ���� m_pSelectUnit�� �ʱ�ȭ �ǹǷ� �� �����͸� ������ ����
#endif SERV_UNIT_WAIT_DELETE

				Handler_EGS_CHARACTER_LIST_REQ();
			}
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		}
#ifdef SERV_UNIT_WAIT_DELETE
		else if( kEvent.m_iOK == NetError::ERR_DELETE_UNIT_02)
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_iUnitUID );
			if( pUnit == NULL )
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_713 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_713 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

				return true;
			}

			pUnit->AccessUnitData().m_trDelAbleDate = kEvent.m_tReDelAbleDate;

			CTime tReDeleAbleDate(kEvent.m_tReDelAbleDate);
			std::wstring wstrYear = (CStringW)( tReDeleAbleDate.Format(_T( "%Y" )) );
			std::wstring wstrMonth = (CStringW)( tReDeleAbleDate.Format(_T( "%m" )) );
			std::wstring wstrDay = (CStringW)( tReDeleAbleDate.Format(_T( "%d" )) );
			std::wstring wstrHour = (CStringW)( tReDeleAbleDate.Format(_T( "%H" )) );
			std::wstring wstrMinute = (CStringW)( tReDeleAbleDate.Format(_T( "%M" )) );
			std::wstring wstrMessage = GET_REPLACED_STRING( ( STR_ID_16109, "SSSSS", wstrDay.c_str(), wstrMonth.c_str(), wstrYear.c_str(), wstrHour.c_str(), wstrMinute.c_str()));			
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), wstrMessage.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMessage.c_str(), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		}
#endif SERV_UNIT_WAIT_DELETE
	}
	return true;
}

//bool CX2StateUnitSelect::DeleteUnitTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"������ ������ �����ϴ�\n������ ���� �� �����ϴ�.", this );
//
//return true;
//}


bool CX2StateServerSelect::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ()
{
	if ( g_pMain->GetIsPlayingTutorial() == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_716 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_716 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		
		Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_545 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_545 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

		Handler_EGS_STATE_CHANGE_FIELD_REQ();
	}

	return true;
}

bool CX2StateServerSelect::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//#if 0 
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KStateChangeAck kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pMain->CreateStateChangeDLG( L"���������� �̵����Դϴ�." );
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, kEvent.m_nMapID, false );
//		}
//	}
//#endif


	return true;
}






bool CX2StateServerSelect::Handler_EGS_CHANGE_NICK_NAME_REQ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly )
{
	if( NULL == wszNickName )
		return false;

	KEGS_CHANGE_NICK_NAME_REQ kPacket;
	kPacket.m_iUnitUID		= uid;
	kPacket.m_wstrNickName	= wszNickName;
	kPacket.m_bCheckOnly	= bCheckOnly;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_NICK_NAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_NICK_NAME_ACK );

	return true;
}


bool CX2StateServerSelect::Handler_EGS_CHANGE_NICK_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_CHANGE_NICK_NAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_NICK_NAME_ACK ) == true )
	{
		m_bJustChangedNickName = false;



		if( true == kEvent.m_bCheckOnly )
		{
			switch( kEvent.m_iOK )
			{
			case NetError::ERR_RESTORE_NICK_NAME_00:			
			case NetError::ERR_RESTORE_NICK_NAME_01:			
			case NetError::ERR_RESTORE_NICK_NAME_02:			
			case NetError::ERR_RESTORE_NICK_NAME_03:			
			case NetError::ERR_RESTORE_NICK_NAME_04:			
			case NetError::ERR_RESTORE_NICK_NAME_05:			
			case NetError::ERR_RESTORE_NICK_NAME_06:			
				{
					SetRenameMsgBoxError( NetError::GetErrStrF( kEvent.m_iOK ) );
					return true;
				} break;
			}


			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				// �г��� ���� ���� Ȯ�� �Ϸ�. ��¥ �ٲ��� Ȯ�� �˾�â ����
				SAFE_DELETE_DIALOG( m_pDLGRenameConfirmMsgBox );
				
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				m_pDLGRenameConfirmMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), GET_REPLACED_STRING( ( STR_ID_2556, "S", m_NickNameChangeUnitNickName.c_str() ) ), SUSUCM_RENAME_CONFIRM_OK, this, SUSUCM_RENAME_CONFIRM_CANCEL, L"DLG_UI_Selection_MessageBox_Ok_Exit_Button_New.lua", D3DXVECTOR2 ( 0, 0 ),  L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				m_pDLGRenameConfirmMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_2556, "S", m_NickNameChangeUnitNickName.c_str() ) ), SUSUCM_RENAME_CONFIRM_OK, this, SUSUCM_RENAME_CONFIRM_CANCEL );
				SetRenameMsgBoxError( L"" );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			}
		}
		else
		{
			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				// �г��� ���� �Ϸ�. ĳ���� �����ؼ� ��������.
				OpenRenameMsgBox( false );

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				m_pDLGRenameSuccessMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_REPLACED_STRING( ( STR_ID_2557, "S", kEvent.m_wstrNickName.c_str() ) ), this, SUSUCM_RENAME_SUCCESS_OK, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				m_pDLGRenameSuccessMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_2557, "S", kEvent.m_wstrNickName.c_str() ) ), this, SUSUCM_RENAME_SUCCESS_OK );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

			}
		}
	}

	return true;
}

//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
bool CX2StateServerSelect::Handler_EGS_SELECT_UNIT_INVENTORY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_UNIT_INVENTORY_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	m_bIdentityConfirmCheck = false; // 2011.11.09 lygan_������ // ĳ���� �������� �� ��ħ�� �������� ���� ������ �ʱ�ȭ ��Ű��
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

	if ( g_pData == NULL || g_pData->GetMyUser() == NULL )
	{
		ASSERT( !L"g_pData or g_pData->GetMyUser In EGS_SELECT_UNIT_INVENTORY_INFO_NOT is WRONG" );
		return false;
	}

	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	if ( pMyUnit != NULL )
	{
		pMyUnit->AccessInventory().ResetItems( kEvent.m_mapItem );
		return true;
	}
	if( g_pData != NULL )
	{
		g_pData->ResetPlayGuide();
	}
	ASSERT( !L"pMyUnit or GetInventory In EGS_SELECT_UNIT_INVENTORY_INFO_NOT is WRONG" );
	return false;	
}

bool CX2StateServerSelect::Handler_EGS_GET_MY_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_MY_INVENTORY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( false == g_pMain->DeleteServerPacket( EGS_GET_MY_INVENTORY_ACK ) )
	{
		StateLog( L"Invalid EGS_GET_MY_INVENTORY_ACK" );
		return false;
	}

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
#ifdef SERV_RECOMMEND_LIST_EVENT
		g_pData->Set_ShowRecommendUI( !kEvent.m_bIsRecommend );
#endif //SERV_RECOMMEND_LIST_EVENT		
		// ü�� ���̵� ���� 
		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser &&
			kEvent.m_bIsRecommend == false && m_bCheckRecommend == true )
		{
#ifdef SERV_NO_GAMEINVITE
			Handler_EGS_RECOMMEND_USER_REQ( L"", true );
#else SERV_NO_GAMEINVITE
			OpenRecommendMsgBox( true );
#endif SERV_NO_GAMEINVITE
		}
		else
		{
			if ( g_pMain->GetIsPlayingTutorial() == true )
			{
				// Ʃ�丮�� ���Կ�û
				Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
			}
			else
			{
				const CX2Unit::UnitData* pUnitData = 	&g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

				if ( SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 <= pUnitData->m_nMapID && 
					 SEnum::VMI_BATTLE_FIELD_END > pUnitData->m_nMapID )
				{
					CX2GageManager* pGageManager = CX2GageManager::GetInstance();
					if ( pGageManager->GetMyGageData()->GetNowHp() > 0.0f )
						Handler_EGS_JOIN_BATTLE_FIELD_REQ( pUnitData->m_nMapID, pUnitData->m_ucLastTouchLineIndex, pUnitData->m_usLastPosValue  );
					else
					{
						CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
						CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ( battleFieldManager.GetReturnVillageId( pUnitData->m_nMapID ) );	// �������� ���Կ�û
					}
				}
				else
					Handler_EGS_STATE_CHANGE_FIELD_REQ();	// �������� ���Կ�û
			}
		}

		//{{ kimhc // GiveUpForbiddenQuest()�� EGS_SELECT_UNIT_ACK���� �̵���
	#ifdef	SERV_EPIC_QUEST
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	#endif	SERV_EPIC_QUEST
		//}} kimhc // GiveUpForbiddenQuest()�� EGS_SELECT_UNIT_ACK���� �̵���

		return true;
	}

	return false;

}
#endif	PC_BANG_WORK
//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����


void CX2StateServerSelect::SetRenameMsgBoxError( const wstring& wstrErrorMessage )
{

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), wstrErrorMessage.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	if( NULL == m_pDLGRenameMsgBox )
		return;

	CKTDGUIStatic* pStaticErrorMessage = (CKTDGUIStatic*) m_pDLGRenameMsgBox->GetControl( L"NickNameErrorMessage" );
	if( NULL == pStaticErrorMessage )
		return;
	
	if( NULL == pStaticErrorMessage->GetString(0) )
		return;

	pStaticErrorMessage->GetString(0)->msg = wstrErrorMessage;
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
}


void CX2StateServerSelect::OpenRenameMsgBox( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGRenameMsgBox )
		{

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			m_pDLGRenameMsgBox = new CKTDGUIDialog( this, L"DLG_Change_Nickname_New.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			m_pDLGRenameMsgBox = new CKTDGUIDialog( this, L"DLG_Change_Nickname.lua" );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRenameMsgBox );
		}

		m_pDLGRenameMsgBox->SetShowEnable( true, true );
	}
	else
	{
		if( NULL != m_pDLGRenameMsgBox )
		{
			m_pDLGRenameMsgBox->SetShowEnable( false, false );
		}
	}
}


bool CX2StateServerSelect::Handler_EGS_RECOMMEND_USER_REQ( const WCHAR* wszNickName, bool bCancel )
{
	KEGS_RECOMMEND_USER_REQ kPacket;
	kPacket.m_wstrNickName = wszNickName;
	kPacket.m_wstrNickName = kPacket.m_wstrNickName.substr( 0, 20 );
	kPacket.m_bCancel = bCancel;

	if ( kPacket.m_bCancel == false && g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, kPacket.m_wstrNickName.c_str() ) == false )
	{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_717 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		return true;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_RECOMMEND_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RECOMMEND_USER_ACK );

	return true;
}

bool CX2StateServerSelect::Handler_EGS_RECOMMEND_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_RECOMMEND_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_RECOMMEND_USER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_RECOMMEND_LIST_EVENT
			g_pData->Set_ShowRecommendUI( false );
#endif //SERV_RECOMMEND_LIST_EVENT	
			OpenRecommendMsgBox( false );
			g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() + kEvent.m_iRewardResurrectionStone );
			if ( kEvent.m_iRewardResurrectionStone > 0 )
			{
				m_bCheckRecommendSuccess = true;
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_718, "i", kEvent.m_iRewardResurrectionStone ) );
				
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				m_pDLGRecommendSuccess = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), wstrstm.str().c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				m_pDLGRecommendSuccess = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			}
			else
			{
				Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
				//Handler_EGS_GET_POST_LETTER_LIST_REQ();
			}

			return true;
		}
	}

	return false;
}

void CX2StateServerSelect::OpenRecommendMsgBox( bool bOpen )
{
	if ( bOpen == true )
	{
		if ( m_pDLGRecommend != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRecommend, NULL, false );

		m_pDLGRecommend = NULL;

#ifdef SERV_RECOMMEND_LIST_EVENT
		m_pDLGRecommend = new CKTDGUIDialog( this, L"DLG_Recommend2.lua" );
#else
		m_pDLGRecommend = new CKTDGUIDialog( this, L"DLG_Recommend.lua" );
#endif //SERV_RECOMMEND_LIST_EVENT

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRecommend );

		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGRecommend->GetControl( L"g_pIMEEditBoxName" );
		if ( pIMEEditBox != NULL )
		{
			pIMEEditBox->RequestFocus();
		}
	}
	else
	{
		if ( m_pDLGRecommend != NULL )
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGRecommend, NULL, false );

		m_pDLGRecommend = NULL;
	}
}





bool CX2StateServerSelect::Handler_EGS_SELECT_SERVER_SET_REQ()
{
	if ( m_sbSelectedServerSet == true )
		return true;

	KEGS_SELECT_SERVER_SET_REQ kPacket;
	//kPacket.m_ServerSetID = iServerSetIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_SELECT_SERVER_SET_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_SELECT_SERVER_SET_ACK );

	return true;	
}

bool CX2StateServerSelect::Handler_EGS_SELECT_SERVER_SET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_SELECT_SERVER_SET_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	
	if( g_pMain->DeleteServerPacket( EGS_SELECT_SERVER_SET_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )	
		{
			m_sbSelectedServerSet = true;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			Handler_EGS_CHARACTER_LIST_REQ();
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
			
		}
		else
		{
			SAFE_DELETE_DIALOG( m_pDLGMsgBox );

			switch( g_pData->GetMyUser()->GetAuthLevel() )
			{
			case -2:
			case -3:
				//{{ 2010. 06. 30  ������	�������߰�
			case -5:
			case -6:
				//}}
				//{{ ���� ���� �ϼ� ���� �߰� - ��μ�
//#ifdef SERV_ADD_BLOCK_DAY
			case -7:
			case -8:
			case -9:
//#endif SERV_ADD_BLOCK_DAY
				//}}
				{
                    wstring wstrMsg = GET_REPLACED_STRING( ( STR_ID_719, "L", kEvent.m_kAccountBlockInfo.m_wstrEndTime ) );
					wstrMsg += kEvent.m_kAccountBlockInfo.m_wstrBlockReason;
					
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), wstrMsg.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), wstrMsg.c_str(), (CKTDXStage*)this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				} break;

			case -4:
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), kEvent.m_kAccountBlockInfo.m_wstrBlockReason.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), kEvent.m_kAccountBlockInfo.m_wstrBlockReason.c_str(), (CKTDXStage*)this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

					
				} break;
			default:
#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-04	// �ڼ���
				if( ( SEnum::HUCT_NONE < kEvent.m_kAccountBlockInfo.m_byteBlockType ) &&
					( kEvent.m_kAccountBlockInfo.m_byteBlockType < SEnum::HUCT_MAX )
					)
				{
					if( ( SEnum::HUCT_NONE < kEvent.m_kAccountBlockInfo.m_byteBlockType ) &&
						( kEvent.m_kAccountBlockInfo.m_byteBlockType < SEnum::HUCT_MAX )
						)
					{
						wstring wstrMsg;
						if( kEvent.m_kAccountBlockInfo.m_wstrBlockEndDate.empty() == false )
						{
							wstrMsg = GET_REPLACED_STRING( ( STR_ID_719, "L", kEvent.m_kAccountBlockInfo.m_wstrBlockEndDate ) );

							if( kEvent.m_kAccountBlockInfo.m_wstrBlockReason2.empty() == false )
							{
								wstrMsg += L"\n";
								wstrMsg += kEvent.m_kAccountBlockInfo.m_wstrBlockReason2;
							}

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), wstrMsg.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), wstrMsg.c_str(), (CKTDXStage*)this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
							
						}
						else if( kEvent.m_kAccountBlockInfo.m_wstrBlockReason2.empty() == false )
						{
							wstrMsg = kEvent.m_kAccountBlockInfo.m_wstrBlockReason2;
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), wstrMsg.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����							
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,150), wstrMsg.c_str(), (CKTDXStage*)this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

						}
					}
				}
#endif // SERV_HACKING_USER_CHECK_COUNT
				break;
			}
#ifdef CLIENT_COUNTRY_TWHK
			// �ùٸ� ���� ������ �ƴϾ��ٴ� NetError���� ���⼭ �˻��ؼ� Ŭ�� ���������Ų��
#endif CLIENT_COUNTRY_TWHK
		}
	}

	// �������� �� ä�� ���ý� ����/ä�μ���â�� ������� ���ּ���â�� ���´�.
#ifdef SERVER_GROUP_UI_ADVANCED
	if(m_pDLGServerGroupSelectBack != NULL)
		m_pDLGServerGroupSelectBack->SetShowEnable(false, false);
#endif SERVER_GROUP_UI_ADVANCED
	m_pDLGServerSelectBack->SetShowEnable(false, false);
	m_pDLGUnitSelectFront->SetShowEnable(true, true);

	//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
	m_pDLGChannelSelectBack->SetShowEnable( false, false );
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�

#ifndef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	// ä�ι�ȣ ǥ��
	if( g_pInstanceData->GetConnectChannelServerID() > 0 )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGUnitSelectFront->GetControl( L"Channel_Name" );

		pStatic->GetString(0)->msg = g_pInstanceData->GetConnectChannelServerName();
	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	

	return true;
}

bool CX2StateServerSelect::Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ()
{
	/*
	KEVENT_X2_SERVER_UNIT_INFO_LIST_REQ	kEVENT_X2_SERVER_UNIT_INFO_LIST_REQ;
	kEVENT_X2_SERVER_UNIT_INFO_LIST_REQ.userUID = g_pData->GetMyUser()->GetUID();
*/
#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
	if ( g_pMain != NULL )
	{
		bool bCheckPcBang = false;
		bCheckPcBang = g_pMain->IsStartPcBang();
	}
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

	g_pData->GetServerProtocol()->SendID( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ );
	g_pMain->AddServerPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_720 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_720 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
	IF_EVENT_ENABLED( CEI_ELISIS_PREVIOUS_SIS_EVENT )
	{
#ifdef CLIENT_COUNTRY_CN
		int iMaxCreateCount = 1000;
		if( 1 == g_pInstanceData->GetServerGroupID() ) // ȭ��1
			iMaxCreateCount = 3000;
		else if( 4 == g_pInstanceData->GetServerGroupID() ) // ȭ��
			iMaxCreateCount = 2000;
		else if( 6 == g_pInstanceData->GetServerGroupID() ) // ȭ��2
			iMaxCreateCount = 3000;
		else if( 101 == g_pInstanceData->GetServerGroupID() ) //ȭ��
			iMaxCreateCount = 1000;
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_REPLACED_STRING(( STR_ID_27072, "i", iMaxCreateCount )), g_pMain->GetNowState() );
#else // CLIENT_COUNTRY_CN
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_27072 ), g_pMain->GetNowState() );
#endif // CLIENT_COUNTRY_CN
	}
#endif //SERV_ELISIS_PREVIOUS_SIS_EVENT

	return true;
}

bool CX2StateServerSelect::Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
#ifdef SERV_SHARING_BANK_QUEST_CASH
	// ���� : ���� ���� �����ٰ� Ŭ�� ��������.
	g_pData->GetMyUser()->SetSharingBankOpen(kEvent.m_bSharingBank);
	// ���� ����Ʈ�� �������̴� ĳ���Ͷ�� ���� ��Ű��
	// 			if( NULL != g_pData->GetQuestManager() && g_pData->GetMyUser()->IsSharingBankOpen() == true )
	// 			{
	// 				g_pData->GetQuestManager()->GiveUpForbiddenQuest(); 
	// 			}
#endif // SERV_SHARING_BANK_QUEST_CASH

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	g_pInstanceData->SetK4ThAnnivEventInfo( kEvent.m_4ThAnnivEventInfo );
#endif //SERV_4TH_ANNIVERSARY_EVENT
	
	if( g_pMain->DeleteServerPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			ClearBeforeUnit( m_pSelectUnit );
			ClearUnitButton();
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			SetShowSelectUnitInfo( NULL );
			if ( NULL != g_pData->GetUIMajorXMeshPlayer() )
			{
				BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshInstanceHandle hStandMeshInst, m_vecStandMeshInst )
				{	
					g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( hStandMeshInst );
				}
				m_vecStandMeshInst.clear();
			}
			if( NULL != g_pData->GetUIEffectSet() )
			{
				g_pData->GetUIEffectSet()->StopEffectSet( m_hStandEffect );
			}
#endif // 13-11-11, ���� ���� ����, kimjh
			g_pData->GetMyUser()->DeleteAllUnit();

			g_pData->GetMyUser()->AccessUserData().maxUnitCount = kEvent.m_nUnitSlot; //�������� ���� ����?

			m_MaxUnitNum	= kEvent.m_nUnitSlot;

            for( int i =0; i < (int)kEvent.m_vecUnitInfo.size(); i++ )
			{	 

				KUnitInfo kUnitInfo = kEvent.m_vecUnitInfo[i];
				CX2Unit* pUnit = new CX2Unit( kUnitInfo );

				pUnit->SetOwnerUserUID( g_pData->GetMyUser()->GetUID() );
				wstring nickNameToChange = L"__DELETED__";			
				
				if ( nickNameToChange == pUnit->GetUnitData().m_NickName )
				{
					wstringstream tempNickName;
					tempNickName << nickNameToChange.c_str() << i;
					pUnit->AccessUnitData().m_NickName = tempNickName.str().c_str();
				}
				g_pData->GetMyUser()->AddUnit( pUnit );
			}

			m_NowPage		= 1;
			const int SLOT_COUNT_FOR_CREATE_UNIT = 1;
			if( g_pData->GetMyUser()->GetUnitNum() < m_MaxUnitNum )
			{
				m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
			}
			else
			{
				m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
			}



#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			CreateUnitButtonNew();
			ClearUnitButton();
			ChangeUnitButtonInfo();
			int iTopCharacterIndex = (m_NowPage - 1) * UNIT_SELECT_UI_MAX_UNIT_NUM;
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( iTopCharacterIndex );
			if ( NULL != pUnit )
			{
				CreateServerSelectUnitViewerUI ( pUnit );
				SetFocusUnitButtonByIndex ( 0 );
				m_pSelectUnit = pUnit;
				SetRightSideFlagColor ( GetUnitKeyColorByUnitType ( m_pSelectUnit->GetType() ) );
				SetShowSelectUnitInfo ( pUnit );
				ChangeSelectUnitStandMeshInstance ( pUnit->GetType () );
				SetBackgroundImageBySelectUnitType ( pUnit->GetType() );
			}
			SetShowServerStateButtonSet ( true, false );			
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			CreateUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

			m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK = true;

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
			if( NULL != g_pMain && NULL != g_pMain->GetSecurityPad() 
				&& false == g_pMain->GetSecurityPad()->GetUseSecondPW() )
			{
				Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
			}
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
			if( 0 != g_pInstanceData->GetPreEventElesisUID() )
			{
//				g_pData->GetMyUser()->SortFirstByUID( g_pInstanceData->GetPreEventElesisUID());
				g_pInstanceData->SetPreEventElesisUID(0);
			}
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT
		}
		else
		{
			g_pData->DeleteServerProtocol();
			g_pData->DeleteMyUser();
		}
	}

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	return true;
}




bool CX2StateServerSelect::Handler_EGS_AGREE_HACK_USER_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_AGREE_HACK_USER_REQ );
	g_pMain->AddServerPacket( EGS_AGREE_HACK_USER_ACK, 500.0f );

	return true;
}

bool CX2StateServerSelect::Handler_EGS_AGREE_HACK_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_AGREE_HACK_USER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if( g_pMain->DeleteServerPacket( EGS_AGREE_HACK_USER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifndef PROCESSLIST
			g_pMain->UpdateProcessList();
#endif
			g_pData->GetMyUser()->AccessUserData().hackingUserType = (CX2User::HACKING_USER_TYPE)kEvent.m_cHackingUserType;
			SAFE_DELETE_DIALOG( m_pDLGCheckHack );
		}
		else
		{
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, 0, 0, false );
		}
	}
	return true;
}
//{{ kimhc // 2009-12-15 // ������ �÷����ߴ� ä�� ������
#ifdef	ADD_SERVER_GROUP

void CX2StateServerSelect::UpdateChannelListUI()
{
#ifdef SHOW_ONLY_ADVERTISEMENT
	if( true == m_bHideServerSelectUI )
		return;
#endif //SHOW_ONLY_ADVERTISEMENT

	SAFE_DELETE_DIALOG( m_pDLGChannelSelectBack );
		
#ifdef SERV_CHANNEL_LIST_RENEWAL
	int ServerGroupNum = g_pInstanceData->GetServerGroupID();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection_New.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection.lua" );

	if(m_pDLGChannelSelectBack == NULL)
		m_pDLGChannelSelectBack =  new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection_Auto.lua" );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

#else //SERV_CHANNEL_LIST_RENEWAL

	switch ( g_pInstanceData->GetServerGroupID() )
	{
	case SGI_GAIA:
		{
			m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection02.lua" );
		}
		break;

	case SGI_SOLES:
	default:
		{
			m_pDLGChannelSelectBack = new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection01.lua" ); //new CKTDGUIDialog( this, L"DLG_UI_Channel_Selection01.lua" );
		}
		break;
	}
#endif //SERV_CHANNEL_LIST_RENEWAL

	if ( m_pDLGChannelSelectBack == NULL )
		return;
	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChannelSelectBack );

	// ���⼭ ���� ä�� ����Ʈ ����
#if defined(CLIENT_COUNTRY_TH)
	const int MAX_CHANNEL_COUNT = 16;	// ä�� ����
#else
	const int MAX_CHANNEL_COUNT = m_iChannelNum;	// ä�� ����
#endif 

	// ä�� ����Ʈ ��ư �ʱ�ȭ
	WCHAR wszControlName[32] = L"";

	// ä�� ȥ�⵵ ������
	CKTDGUIStatic* pStaticCHCongestionGage	= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_line" ) );		
	// ä�� ȸ�� �ڽ�
	CKTDGUIStatic* pStaticCHBox				= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_state" ) );		
	// ä���̸� ex)�纥1
	CKTDGUIStatic* pStaticCHName			= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_num" ) );		
	
#ifdef SERV_CHANNEL_LIST_RENEWAL
	CKTDGUIStatic* pStaticCHBar			= static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( L"chanal_bar" ) );	
#endif
	
	// ��ü ä�� �� ������ �ʵ��� �ʱ�ȭ
	for ( int i = 0; i < MAX_CHANNEL_COUNT; ++i )
	{
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i + 1 );
		CKTDGUIButton *pChannelButton = static_cast< CKTDGUIButton* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );
		
		if( pChannelButton != NULL )
			pChannelButton->SetShowEnable(false, false);

		// ä�� ȥ�⵵ ��Ʈ�� ex) ����, ��Ȱ
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i + 1);
		CKTDGUIStatic* pStaticCHCongestionStr = static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );
		
		if(pStaticCHCongestionStr != NULL)
			pStaticCHCongestionStr->SetShowEnable(false, false);

#ifdef MODIFY_CHANNEL_MODE_RATE_INT
		const int	iCntChannelGageMode = 4;
		for( int j=0; j< iCntChannelGageMode; ++j )
		{
            if ( pStaticCHCongestionGage != NULL &&
                pStaticCHCongestionGage->GetPicture( (i * iCntChannelGageMode) + j ) != NULL )
			    pStaticCHCongestionGage->GetPicture( (i * iCntChannelGageMode) + j )->SetShow(false);
		}
#else MODIFY_CHANNEL_MODE_RATE_INT
        if ( pStaticCHCongestionGage->GetPicture( i * 3 ) != NULL )
		    pStaticCHCongestionGage->GetPicture( i * 3 )->SetShow(false);
        if ( pStaticCHCongestionGage->GetPicture( i * 3 + 1 ) != NULL )
		    pStaticCHCongestionGage->GetPicture( i * 3 + 1 )->SetShow(false);
        if ( pStaticCHCongestionGage->GetPicture( i * 3 + 2 ) != NULL )
		    pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetShow(false);
#endif MODIFY_CHANNEL_MODE_RATE_INT

        if ( pStaticCHBox->GetPicture( i * 3 ) != NULL )
		    pStaticCHBox->GetPicture( i * 3 )->SetShow(false);
        if ( pStaticCHBox->GetPicture( i * 3 + 1 ) != NULL )
		    pStaticCHBox->GetPicture( i * 3 + 1 )->SetShow(false);
        if ( pStaticCHBox->GetPicture( i * 3 + 2 ) != NULL )
		    pStaticCHBox->GetPicture( i * 3 + 2 )->SetShow(false);

#ifndef SERV_CHANNEL_LIST_RENEWAL
        if ( pStaticCHName->GetPicture( i ) != NULL )
		    pStaticCHName->GetPicture( i )->SetShow(false);
#else
        if ( pStaticCHBar->GetPicture( i + 1 ) != NULL )
		    pStaticCHBar->GetPicture( i + 1 )->SetShow(false);
#endif
	}				

	if( false == m_bReceivedChannelList )	// ä�θ���Ʈ�� ���� �ʾ����� ����
		return;

#ifdef CHANGE_CHANNEL
	if( g_pInstanceData == NULL || g_pInstanceData->m_vecChannelInfo.empty() == true)
		return;
#else
	if( true == m_vecChannelInfo.empty() )
		return;
#endif

#ifdef MODIFY_CHANNEL_MODE_RATE_INT
	float			CHANNEL_CONGESTION_SCOPE_TEMP[] = { 0.1f,	0.2f,	0.5f,	0.99f, };
#ifndef CHANNEL_CONGESTION_SCOPE_NO_CHEAT
	const float		CHANNEL_CONGESTION_SCOPE[]		= { 0.1f,	0.2f,	0.3f,	0.9f, };//{ 0.2f,	0.4f,	0.6f,	0.999f, };
#endif CHANNEL_CONGESTION_SCOPE_NO_CHEAT
	const float		CHANNEL_CONGESTION_DISPLAY[]	= { 0.25f,	0.5f,	0.5f,	0.75f,	};
#else // MODIFY_CHANNEL_MODE_RATE_INT
	// ä�� ������ ǥ��� ����
	float			CHANNEL_CONGESTION_SCOPE_TEMP[] = { 0.1f,	0.2f,	0.3f,	0.9f, };
	const float		CHANNEL_CONGESTION_SCOPE[]		= { 0.1f,	0.2f,	0.3f,	0.9f, };//{ 0.2f,	0.4f,	0.6f,	0.999f, };
	const float		CHANNEL_CONGESTION_DISPLAY[]	= { 0.25f,	0.5f,	0.75f,	0.9f, };
#endif // MODIFY_CHANNEL_MODE_RATE_INT	

	// ä�� ����Ʈ ���� ����
#ifdef CHANGE_CHANNEL
	for( int i = 0; i < static_cast< int >( g_pInstanceData->m_vecChannelInfo.size() )&& i < MAX_CHANNEL_COUNT; i++ )
#else
	for( int i = 0; i < static_cast< int >( m_vecChannelInfo.size() ) && i < MAX_CHANNEL_COUNT; i++ )
#endif
	{	
#ifdef CHANNEL_DOWN_EMERGENCY
		// oasis907 : ����� [2010.10.28] // Ư��ä�� �ӽ÷� ����
		if(i == 10 && g_pInstanceData->GetServerGroupID() == SGI_GAIA)
		{
			continue;
		}
#endif CHANNEL_DOWN_EMERGENCY


		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i + 1 );
		CKTDGUIButton *pChannelButton = static_cast< CKTDGUIButton* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );

		// ä�� ȥ�⵵ ��Ʈ�� ex) ����, ��Ȱ
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i + 1);
		CKTDGUIStatic* pStaticCHCongestionStr = static_cast< CKTDGUIStatic* >( m_pDLGChannelSelectBack->GetControl( wszControlName ) );
#ifndef CHANNEL_CONGESTION_SCOPE_NO_CHEAT
		g_pInstanceData->SetChannelDistributionByServerGroup( CHANNEL_CONGESTION_SCOPE_TEMP, CHANNEL_CONGESTION_SCOPE, i );
#endif CHANNEL_CONGESTION_SCOPE_NO_CHEAT
		// Show ����
		pChannelButton->SetShowEnable(true, true);
		pStaticCHCongestionStr->SetShowEnable(true, true);
		pStaticCHBox->GetPicture( i * 3 )->SetShow(true);
		pStaticCHBox->GetPicture( i * 3 + 1 )->SetShow(true);
		pStaticCHBox->GetPicture( i * 3 + 2 )->SetShow(true);

#ifndef SERV_CHANNEL_LIST_RENEWAL
		pStaticCHName->GetPicture( i )->SetShow(true);
#else
		pStaticCHBar->GetPicture( i + 1 )->SetShow(true);
#endif

#ifdef CHANGE_CHANNEL
		const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[i];
#else
		const KChannelInfo& channelInfo = m_vecChannelInfo[i];
#endif
		float fChannelCongestionRate = static_cast< float >( channelInfo.m_iCurrentUser ) / static_cast< float >( channelInfo.m_iMaxUser );
		float fChannelCongestionRateDisplay = 0.f;

#ifdef MODIFY_CHANNEL_MODE_RATE_INT
		if(fChannelCongestionRate >= CHANNEL_CONGESTION_SCOPE_TEMP[3])	// 99%~100%
		{
			// FULL ������ , ������ ���� 100%
			pStaticCHCongestionGage->GetPicture( i * 4 + 3 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*4+3)->GetOriginalSize().x );
			pStaticCHCongestionGage->GetPicture( i * 4 + 3 )->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_724 );

			pChannelButton->SetShowEnable(true, false);
		}
		else if(fChannelCongestionRate >= CHANNEL_CONGESTION_SCOPE_TEMP[2]) // 50%~98%
		{
			// ȥ�� ��Ȳ�� , ������ ���� 75%
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3];
			pStaticCHCongestionGage->GetPicture( i * 4 + 2 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*4+2)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 4 + 2 )->SetShow( true );	
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_723 );
		}
		else if(fChannelCongestionRate >= CHANNEL_CONGESTION_SCOPE_TEMP[1]) // 20%~49%
		{
			// ���� ��� , ������ ���� 50%
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2];
			pStaticCHCongestionGage->GetPicture( i * 4 + 1 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*4+1)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 4 + 1)->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_722 );
		}
		else	//	0%~19%
		{
			// ��Ȱ �Ķ��� , ������ ���� 25% ( ���� 10%���� ) , 50%
			if( fChannelCongestionRate >= CHANNEL_CONGESTION_SCOPE_TEMP[0] )	// 10%~19%
				fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1];
			else																// 0%~9%
				fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0];
			pStaticCHCongestionGage->GetPicture( i * 4 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*4)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 4 )->SetShow( true );				
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_721 );
		}
#else // MODIFY_CHANNEL_MODE_RATE_INT
		if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[3] )
		{
			fChannelCongestionRateDisplay = 1.f;
		}
		else if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[2] )
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3]; // ȥ��
		}
		else if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[1] )
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2]; // ����
		}
		else if( fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[0] )
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1]; // ��Ȱ
		}
		else
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0]; // ��Ȱ
		}

		if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[1])
		{
			pStaticCHCongestionGage->GetPicture( i * 3 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 3 )->SetShow( true );				
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_721 );
		}
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[2])
		{
			pStaticCHCongestionGage->GetPicture( i * 3 + 1 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3+1)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 3 + 1)->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_722 );
		}
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[3])
		{
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3+2)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_723 );
		}
		else
		{
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetSizeX(pStaticCHCongestionGage->GetPicture(i*3+2)->GetOriginalSize().x );
			pStaticCHCongestionGage->GetPicture( i * 3 + 2 )->SetShow( true );
			pStaticCHCongestionStr->GetString( 0 )->msg = GET_STRING( STR_ID_724 );

			pChannelButton->SetShowEnable(true, false);
		}
#endif // MODIFY_CHANNEL_MODE_RATE_INT
	}


#ifdef SERV_CHANNEL_LIST_RENEWAL
	for(int j = 0; j < static_cast< int >( g_pInstanceData->m_vecChannelInfo.size() ) && j < MAX_CHANNEL_COUNT; ++j)
	{
		wstring wstrtempChannelName = L"";
		wstring wstrtempFieldName = L"";
		wstring wstrtempFieldNameNum = L"";

		int iFieldNum = 0;

		wstrtempChannelName = g_pInstanceData->GetChannelServerName(g_pInstanceData->m_vecChannelInfo[j].m_iChannelID);

		int iCharNum = 0;
		ASSERT(wstrtempChannelName.empty() == false);

		if(wstrtempChannelName.empty() == false)
		{
			for(int k = 0; k < (int)wstrtempChannelName.size(); k++ )
			{
				WCHAR tempChar = wstrtempChannelName[k];
				if ( tempChar < '0' || tempChar > '9' )
					iCharNum++;
			}
			wstrtempFieldName = wstrtempChannelName.substr(0,iCharNum);

			iFieldNum = _wtoi(wstrtempChannelName.substr(iCharNum, (wstrtempChannelName.size()-iCharNum)).c_str());

			if(iFieldNum < 10)
			{
				wstrtempFieldNameNum = L"0";
			}
			wstrtempFieldNameNum += wstrtempChannelName.substr(iCharNum, (wstrtempChannelName.size()-iCharNum));

			CKTDGUIControl::CPictureData* pPicture = NULL;
			pPicture = new CKTDGUIControl::CPictureData();
			CKTDGUIControl::UIPointData* pPointData = new CKTDGUIControl::UIPointData();

			if(pStaticCHName == NULL && pPicture == NULL && pPointData == NULL)
				return;

			int iChannelNameOffset = 0; // �� �������� ä�� UI ����� Ʋ���� ������ ���� �ʿ��ϴ�
			
#ifdef CHANNEL_UI_JAPAN
			if(wstrtempFieldName == GET_STRING( STR_ID_15886 ))
			{
				wstring wstrtempField = L"FIRSTSTEP";
				wstrtempField += wstrtempFieldNameNum;
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());
#ifdef CHANGE_TGA_JAPAN
				//iChannelNameOffset = 7;
#else // CHANGE_TGA_JAPAN
				iChannelNameOffset = 8;
#endif // CHANGE_TGA_JAPAN
			}
			else
#endif // CHANNEL_UI_JAPAN
			if(wstrtempFieldName == GET_STRING( STR_ID_17731 ))
			{
				wstring wstrtempField = L"RUBEN"; //2011.11.22 lygan_������ //DLG_UI_Title02_A.tet �� �ִ� �̸��� ����Ѵ�
				wstrtempField += wstrtempFieldNameNum;	
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) || defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());
#endif //CHANNEL_UI_EU
#ifdef CHANGE_TGA_JAPAN
				//iChannelNameOffset = 7;
#else // CHANGE_TGA_JAPAN
				iChannelNameOffset = 8;
#endif // CHANGE_TGA_JAPAN
			}
			else if(wstrtempFieldName == GET_STRING( STR_ID_17732 ))
			{

				wstring wstrtempField = L"ELDER";
				wstrtempField += wstrtempFieldNameNum;
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());	
#endif //CHANNEL_UI_EU
#ifdef CHANGE_TGA_JAPAN
				//iChannelNameOffset = 8;
#else // CHANGE_TGA_JAPAN
				iChannelNameOffset = 8;
#endif // CHANGE_TGA_JAPAN
			}
			else if(wstrtempFieldName == GET_STRING( STR_ID_17733 ))
			{
				wstring wstrtempField = L"BESMA";
				wstrtempField += wstrtempFieldNameNum;
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());	
#endif //CHANNEL_UI_EU

			}
			else if(wstrtempFieldName == GET_STRING( STR_ID_17734 ))
			{
				wstring wstrtempField = L"ALTERA";
				wstrtempField += wstrtempFieldNameNum;
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());	
#endif //CHANNEL_UI_EU	
			}
			else if(wstrtempFieldName == GET_STRING( STR_ID_23098 ))
			{
				wstring wstrtempField = L"PEITA";
				wstrtempField += wstrtempFieldNameNum;
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());	
#endif //CHANNEL_UI_EU	

#ifdef CHANGE_TGA_JAPAN
				//iChannelNameOffset = 8;
#else // CHANGE_TGA_JAPAN
				iChannelNameOffset = 8;	
#endif // CHANGE_TGA_JAPAN
			}
			else if(wstrtempFieldName == GET_STRING( STR_ID_17735 ))
			{

				wstring wstrtempField = L"VELDER";
				wstrtempField += wstrtempFieldNameNum;
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());	
#endif //CHANNEL_UI_EU	
#ifdef CHANGE_TGA_JAPAN
				//iChannelNameOffset = 7;
#else // CHANGE_TGA_JAPAN
				iChannelNameOffset = 7;
#endif // CHANGE_TGA_JAPAN

			}
			else if(wstrtempFieldName == GET_STRING( STR_ID_17736 ))
			{
				wstring wstrtempField = L"HAMEL";
				wstrtempField += wstrtempFieldNameNum;
#if defined ( CHANNEL_UI_EU ) || defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
				pPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrtempField.c_str());
#else //CHANNEL_UI_EU
				pPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrtempField.c_str());	
#endif //CHANNEL_UI_EU	
#ifdef CHANGE_TGA_JAPAN
				//iChannelNameOffset = 7;
#else // CHANGE_TGA_JAPAN
				iChannelNameOffset = 7;
#endif // CHANGE_TGA_JAPAN
			}

			if (iFieldNum >= 10)
				iChannelNameOffset -= 5;

#ifdef CLIENT_COUNTRY_US
			// �̱��� ��ġ ���� �ʿ� �����ϴ�.
			iChannelNameOffset = 0;
#endif //CLIENT_COUNTRY_US

			if((j+1)%2 == 0)
			{
#ifdef CHANNEL_UI_EU
				pPointData->leftTopPoint = D3DXVECTOR2(678.f+iChannelNameOffset,126 + static_cast< float >( (j/2) * 48));
#else
#ifdef CHANNEL_UI_TH
				pPointData->leftTopPoint = D3DXVECTOR2(678.f+iChannelNameOffset,132 + static_cast< float >( (j/2) * 48));
#else 
				pPointData->leftTopPoint = D3DXVECTOR2(678.f+iChannelNameOffset,126 + static_cast< float >( (j/2) * 48));
#endif //CHANNEL_UI_TH
#endif //CHANNEL_UI_EU
			}
			else
			{
#ifdef CHANNEL_UI_EU
				pPointData->leftTopPoint = D3DXVECTOR2(355.f+iChannelNameOffset,126 + static_cast< float >( (j/2) * 48));
#else //CHANNEL_UI_EU
#ifdef CHANNEL_UI_TH
				pPointData->leftTopPoint = D3DXVECTOR2(355.f+iChannelNameOffset,126 + static_cast< float >( (j/2) * 48));
#else //CHANNEL_UI_TH
				pPointData->leftTopPoint = D3DXVECTOR2(355.f+iChannelNameOffset,132 + static_cast< float >( (j/2) * 48));
#endif CHANNEL_UI_TH
#endif //CHANNEL_UI_EU
			}

			pPicture->SetPoint(pPointData);
			pPicture->SetShow(true);
			pStaticCHName->AddPicture(pPicture);
			pStaticCHName->SetShow(true);
		}

		std::map<int, KChannelBonusInfo>::const_iterator mit2 = g_pInstanceData->m_mapChannelBonusInfo.find(g_pInstanceData->m_vecChannelInfo[j].m_iChannelID);

		//ASSERT(mit2 != g_pInstanceData->m_mapChannelBonusInfo.end());

		if(mit2 != g_pInstanceData->m_mapChannelBonusInfo.end() )
		{
			wstring wstrChannelLv = KncUtil::toWideString( boost::str(boost::format("(LV%1%~%2%)") % mit2->second.m_iBeginLv % mit2->second.m_iEndLv) );
			float fRightChannelStrPos = 678.f;
			float fLeftChannelStrPos = 355.f;
			wstring wstrLvWordCheck = L"";
			bool bStrLvWordCheck = false;

			for ( int istrNumCheck = 0; istrNumCheck< wstrChannelLv.length(); ++istrNumCheck )
			{
				CKTDGUIControl::CPictureData* pLvPicture = new CKTDGUIControl::CPictureData();
				CKTDGUIControl::UIPointData* pLvPointData = new CKTDGUIControl::UIPointData();
				
				wstring wstrsubChannelLv = L"";

				if(pStaticCHName == NULL && pLvPicture == NULL && pLvPointData == NULL)
					continue;

				wstrsubChannelLv = wstrChannelLv[istrNumCheck];

				if ( wstrsubChannelLv == L"L" || wstrsubChannelLv == L"V" )
				{
					wstrLvWordCheck += wstrChannelLv[istrNumCheck];
					if ( wstrLvWordCheck == L"LV")
					{
						bStrLvWordCheck = true;
					}
					else
					{
						continue;	
					}
				}

				if((j+1)%2 == 0)
				{
					pLvPointData->leftTopPoint = D3DXVECTOR2(fRightChannelStrPos,126 +20 + static_cast< float >( (j/2) * 48));
					if ( bStrLvWordCheck == true)
					{
						fRightChannelStrPos += 19.f;
					}
					else
					{
						fRightChannelStrPos += 7.f;
					}
					
				}
				else
				{
					pLvPointData->leftTopPoint = D3DXVECTOR2(fLeftChannelStrPos,126 +20 + static_cast< float >( (j/2) * 48));
					if ( bStrLvWordCheck == true)
					{
						fLeftChannelStrPos += 19.f;
					}
					else
					{
						fLeftChannelStrPos += 7.f;
					}
					
				}

				if ( bStrLvWordCheck == false )
				{
#if defined ( CHANNEL_UI_BR ) || defined ( CHANNEL_UI_TH )
					pLvPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrsubChannelLv.c_str() );
#else // CHANNEL_UI_BR
					pLvPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrsubChannelLv.c_str() );
#endif // CHANNEL_UI_BR
				}
				else
				{
#if defined ( CHANNEL_UI_BR ) ||  defined ( CHANNEL_UI_TH )
					pLvPicture->SetTex(L"DLG_UI_Title02_B.TGA", wstrLvWordCheck.c_str() );
#else // CHANNEL_UI_BR
					pLvPicture->SetTex(L"DLG_UI_Title02_A.TGA", wstrLvWordCheck.c_str() );
#endif // CHANNEL_UI_BR
					bStrLvWordCheck = false;
				}

				pLvPicture->SetPoint(pLvPointData);
				pLvPicture->SetShow(true);
				pStaticCHName->AddPicture(pLvPicture);
			}

			pStaticCHName->SetShow(true);
		}
	}
#endif //SERV_CHANNEL_LIST_RENEWAL
}

#else	ADD_SERVER_GROUP

void CX2StateServerSelect::UpdateChannelListUI()
{
#ifdef SHOW_ONLY_ADVERTISEMENT
	if( true == m_bHideServerSelectUI )
		return false;
#endif // SHOW_ONLY_ADVERTISEMENT

	if( NULL == m_pDLGServerSelectBack )
		return;

	
	// ä���߰�
	const int MAX_CHANNEL_COUNT = 16;


	// ä�� ����Ʈ ��ư �ʱ�ȭ
	WCHAR wszControlName[32] = L"";

	CKTDGUIStatic* pStatic1 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"chanal_line" );		// ���¹�(3)
	CKTDGUIStatic* pStatic3 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"chanal_state" );	// 3
	CKTDGUIStatic* pStatic4 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"chanal_num" );		// 2
	CKTDGUIStatic* pStaticChannelNum = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"channel_num" );

	for(int i=0; i<MAX_CHANNEL_COUNT; ++i)
	{
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i+1);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGServerSelectBack->GetControl( wszControlName );
		if(pButton != NULL)
			pButton->SetShowEnable(false, false);		

		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i+1);
		CKTDGUIStatic* pStatic2 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( wszControlName );		// ���¸�
		if(pStatic2 != NULL)
			pStatic2->SetShowEnable(false, false);			

		pStatic1->GetPicture(i*3)->SetShow(false);
		pStatic1->GetPicture(i*3+1)->SetShow(false);
		pStatic1->GetPicture(i*3+2)->SetShow(false);
		pStatic3->GetPicture(i*3)->SetShow(false);
		pStatic3->GetPicture(i*3+1)->SetShow(false);
		pStatic3->GetPicture(i*3+2)->SetShow(false);
		pStatic4->GetPicture(i)->SetShow(false);
		//pStatic4->GetPicture(i*2+1)->SetShow(false);
	}				





	if( false == m_bReceivedChannelList )
		return;

#ifdef CHANGE_CHANNEL
	if( g_pInstanceData == NULL || g_pInstanceData->m_vecChannelInfo.empty() == true)
		return;
#else
	if( true == m_vecChannelInfo.empty() )
		return;
#endif

	// ä��/���� ������ ǥ��� ����
	float CHANNEL_CONGESTION_SCOPE_TEMP[] = {0.1f, 0.2f, 0.3f, 0.999f, };
	const float CHANNEL_CONGESTION_SCOPE[] = {0.2f, 0.4f, 0.6f, 0.999f, };
	const float CHANNEL_CONGESTION_DISPLAY[] = {0.25f, 0.5f, 0.75f, 0.99f, };

	const float CHANNEL_CONGESTION_CRITERION[] = { 0.3f, 0.6f, 0.999f, };			// ���� ȥ�� ���� ��谪 30%, 60%, 100%
	int numChannel = 0;
	//int serverLevel = 0; 

	int nMaxNum = 0;
	int nCurrentNum = 0;

	// ä�� ����Ʈ ���� ����
#ifdef CHANGE_CHANNEL
	for( int i=0; i<(int)g_pInstanceData->m_vecChannelInfo.size() && i<MAX_CHANNEL_COUNT; i++ )
#else
	for( int i=0; i<(int)m_vecChannelInfo.size() && i<MAX_CHANNEL_COUNT; i++ )
#endif
	{			
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"chanal%d", i+1);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGServerSelectBack->GetControl( wszControlName );
		StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"statechanal%d", i+1);
		CKTDGUIStatic* pStatic2 = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( wszControlName ); // ���¸�

		if(i == 0 || i == 1 || i == 6 || i == 7)
		{
			// ä�α׷��� ùä���� ������ �ٸ��� ǥ��
			CHANNEL_CONGESTION_SCOPE_TEMP[0] = 0.1f;
			CHANNEL_CONGESTION_SCOPE_TEMP[1] = 0.2f;
			CHANNEL_CONGESTION_SCOPE_TEMP[2] = 0.3f;
			CHANNEL_CONGESTION_SCOPE_TEMP[3] = 0.999f;
		}
		else
		{
			CHANNEL_CONGESTION_SCOPE_TEMP[0] = CHANNEL_CONGESTION_SCOPE[0];
			CHANNEL_CONGESTION_SCOPE_TEMP[1] = CHANNEL_CONGESTION_SCOPE[1];
			CHANNEL_CONGESTION_SCOPE_TEMP[2] = CHANNEL_CONGESTION_SCOPE[2];
			CHANNEL_CONGESTION_SCOPE_TEMP[3] = CHANNEL_CONGESTION_SCOPE[3];
		}
		

		// Show ����
		pButton->SetShowEnable(true, true);
		pStatic2->SetShowEnable(true, true);
		pStatic3->GetPicture(i*3)->SetShow(true);
		pStatic3->GetPicture(i*3+1)->SetShow(true);
		pStatic3->GetPicture(i*3+2)->SetShow(true);
		pStatic4->GetPicture(i)->SetShow(true);
		//pStatic4->GetPicture(i*2+1)->SetShow(true);

		numChannel++;

#ifdef CHANGE_CHANNEL
		const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[i];
#else
		const KChannelInfo& channelInfo = m_vecChannelInfo[i];
#endif
		float fChannelCongestionRate = (float) channelInfo.m_iCurrentUser / (float) channelInfo.m_iMaxUser;
		float fChannelCongestionRateDisplay = 0.f;
		if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[3])
		{
			fChannelCongestionRateDisplay = 1.f;
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[2])
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3]; // ȥ��
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[1])
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2]; // ����
		}
		else if(fChannelCongestionRate > CHANNEL_CONGESTION_SCOPE_TEMP[0])
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1]; // ��Ȱ
		}
		else
		{
			fChannelCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0]; // ��Ȱ
		}


		nMaxNum += channelInfo.m_iMaxUser;
		nCurrentNum += channelInfo.m_iCurrentUser;

		//if( fChannelCongestionRate < CHANNEL_CONGESTION_CRITERION[0] )
		if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[1])
		{
			//serverLevel += 1;
			pStatic1->GetPicture(i*3)->SetSizeX(pStatic1->GetPicture(i*3)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStatic1->GetPicture(i*3)->SetShow( true );				
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_721 );
		}
		//else if( fChannelCongestionRate < CHANNEL_CONGESTION_CRITERION[1] )
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[2])
		{
			//serverLevel += 2;
			pStatic1->GetPicture(i*3+1)->SetSizeX(pStatic1->GetPicture(i*3+1)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStatic1->GetPicture(i*3+1)->SetShow( true );
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_722 );
		}
		//else if( fChannelCongestionRate < CHANNEL_CONGESTION_CRITERION[2] )
		else if(fChannelCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[3])
		{
			//serverLevel += 3;
			pStatic1->GetPicture(i*3+2)->SetSizeX(pStatic1->GetPicture(i*3+2)->GetOriginalSize().x * fChannelCongestionRateDisplay );
			pStatic1->GetPicture(i*3+2)->SetShow( true );
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_723 );
		}
		else
		{
			//serverLevel += 4;
			pStatic1->GetPicture(i*3+2)->SetSizeX(pStatic1->GetPicture(i*3+2)->GetOriginalSize().x );
			pStatic1->GetPicture(i*3+2)->SetShow( true );
			pStatic2->GetString(0)->msg = GET_STRING( STR_ID_724 );

			pButton->SetShowEnable(true, false);
		}

#if 0
		// �����ڸ�忡���� ä�λ��� ���̵��� �Ѵ�.
		if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			WCHAR staticBuff[64] = {0,};
			//wsprintf(staticBuff, L"%d/%d", channelInfo.m_iCurrentUser, channelInfo.m_iMaxUser);
			StringCchPrintf( buff, 64, L"%d/%d", channelInfo.m_iCurrentUser, channelInfo.m_iMaxUser);
			pStaticChannelNum->GetString(i)->msg = staticBuff;
		}
#endif
	}





	// ������ UI ����
	const WCHAR SOLRES_GAGE_CONTROL_NAME[] = L"Sever_Soless_Server_Gauge";	// �ַ��� ������ static control �̸� 


	//if( numChannel != 0 )
	//{
	//	serverLevel = serverLevel / numChannel;
	//}

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( SOLRES_GAGE_CONTROL_NAME );
	if ( pStatic != NULL )
	{
		for(int i=0; i<6; ++i)
		{
			pStatic->GetPicture(i)->SetShow(false);
		}

		float  fServerCongestionRate = (float)nCurrentNum / (float)nMaxNum;
		float fServerCongestionRateDisplay = 0.f;
		if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[3])
		{
			fServerCongestionRateDisplay = 1.f;
		}
		else if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[2])
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[3];
		}
		else if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[1])
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[2];
		}
		else if(fServerCongestionRate > CHANNEL_CONGESTION_SCOPE[0])
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[1];
		}
		else
		{
			fServerCongestionRateDisplay = CHANNEL_CONGESTION_DISPLAY[0];
		}
		 
		//if( fServerCongestionRate < CHANNEL_CONGESTION_CRITERION[0] )
		if(fServerCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[1])
		{
			pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(4)->SetShow( true );
			pStatic->GetPicture(5)->SetShow( true );
		}
		//else if( fServerCongestionRate < CHANNEL_CONGESTION_CRITERION[1] )
		else if(fServerCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[2])
		{
			pStatic->GetPicture(2)->SetSizeX(pStatic->GetPicture(2)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(2)->SetShow( true );
			pStatic->GetPicture(3)->SetShow( true );
		}
		//else if( fServerCongestionRate < CHANNEL_CONGESTION_CRITERION[2] )
		else if(fServerCongestionRateDisplay <= CHANNEL_CONGESTION_DISPLAY[3])
		{
			pStatic->GetPicture(0)->SetSizeX(pStatic->GetPicture(0)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(0)->SetShow( true );
			pStatic->GetPicture(1)->SetShow( true );
		}
		else
		{
			pStatic->GetPicture(0)->SetSizeX(pStatic->GetPicture(0)->GetOriginalSize().x * fServerCongestionRateDisplay);
			pStatic->GetPicture(0)->SetShow( true );
			pStatic->GetPicture(1)->SetShow( true );
		}

#if 0 
		// �����ڸ�忡���� ä�λ��� ���̵��� �Ѵ�.
		if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			CKTDGUIStatic* pStaticChannelNum = (CKTDGUIStatic*)m_pDLGServerSelectBack->GetControl( L"server_user_num" );

			WCHAR staticBuff[64] = {0,};
			//wsprintf(staticBuff, L"%d/%d", nCurrentNum, nMaxNum);
			StringCchPrintf( buff, 64, L"%d/%d", nCurrentNum, nMaxNum);
			pStaticChannelNum->GetString(0)->msg = staticBuff;
		}
#endif
#if 0 
		switch( serverLevel )
		{
		case KServerSetData::UCL_FREE:
			{
				pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * 0.3f);
				pStatic->GetPicture(4)->SetShow( true );
				pStatic->GetPicture(5)->SetShow( true );
			} break;
		case KServerSetData::UCL_NORMAL:
			{
				pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * 0.6f);
				pStatic->GetPicture(2)->SetShow( true );
				pStatic->GetPicture(3)->SetShow( true );
			}
		case KServerSetData::UCL_BUSY:
			{
				pStatic->GetPicture(4)->SetSizeX(pStatic->GetPicture(4)->GetOriginalSize().x * 0.9f);
				pStatic->GetPicture(0)->SetShow( true );
				pStatic->GetPicture(1)->SetShow( true );
			}
		case KServerSetData::UCL_FULL:
			{
				pStatic->GetPicture(0)->SetShow( true );
				pStatic->GetPicture(1)->SetShow( true );
			}
		}
#endif
	}


#ifndef SERVER_GROUP_UI_ADVANCED
	CKTDGUIButton *pServerButton = (CKTDGUIButton*)m_pDLGServerSelectBack->GetControl( L"sever_select" );
	pServerButton->SetDownStateAtNormal(true);
#endif SERVER_GROUP_UI_ADVANCED
}
#endif	ADD_SERVER_GROUP
// kimhc // 2009-12-15 // ������ �÷����ߴ� ä�� ������


void CX2StateServerSelect::UnitHandling( double fTime, float fElapsedTime )
{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	if ( NULL != m_pUnitViewerUI )
	{
		if ( CX2UnitViewerUI::GetUnitClicked() == true )
		{
			D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
			m_pUnitViewerUI->GetMatrix().RotateRel(RotateY);
			m_bRotateReturn = false;

			m_fRotSpeed = 13.0f;
		}
		else
		{
			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
		#ifdef REFORM_ENTRY_POINT //JHKang
			if ( g_pData != NULL && m_pUnitViewerUI->GetUnit() != NULL && NULL != m_pUnitViewerUI->GetUnit()->GetUnitTemplet()  )
			{
				float fRotY = g_pData->RotateYSelectCharacterType( m_pUnitViewerUI->GetUnit()->GetClass() );
				m_pUnitViewerUI->GetMatrix().Rotate( D3DXToRadian(0),D3DXToRadian(fRotY),D3DXToRadian(0) );
			}
		#else //REFORM_ENTRY_POINT
			m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
		#endif //REFORM_ENTRY_POINT			
			int addSpeed = 20;

			if ( m_bRotateReturn == false )
			{
				m_fRotSpeed -= (fElapsedTime * addSpeed);

				if ( m_fRotSpeed <= 0 )
				{
					m_fRotSpeed = fElapsedTime;
				}

				if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
				{

					if ( m_fRemainRotY <= 0.0f )
					{
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						m_fRemainRotY = 0.0f;
						m_bRotateReturn = true;
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*m_fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= 0.0f )
						{
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							m_fRemainRotY = 0.0f;
							m_bRotateReturn = true;
						}
					}
				}
				else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
				{
					if ( m_fRemainRotY >= 6.18f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*m_fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 6.18f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
				{
					if ( m_fRemainRotY >= 0.0f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*m_fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 0.0f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
				{
					if ( m_fRemainRotY <= -6.28f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*m_fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= -6.28f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
			}
		}

#ifdef SERV_UNIT_WAIT_DELETE
		CX2Unit::UnitData* pUnitData = &m_pUnitViewerUI->GetUnit()->AccessUnitData();
		
		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );
		if( true == pUnitData->m_bDeleted && m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
		{
			if( false == pUnitData->m_bDeletedMotionPlay)
			{
				m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT_DELETE );
				pUnitData->m_bDeletedMotionPlay = true;
			}
		}
		else if( m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			if( true == m_bPlayDownAnim )
			{
				if( false == pUnitData->m_bDeleted )
				{
					m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
				}
				
				m_bPlayDownAnim = false;
			}
		}
#else
		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );
		if ( m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			if ( true == m_bPlayDownAnim )
			{
				m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
				m_bPlayDownAnim = false;
			}
		}
#endif SERV_UNIT_WAIT_DELETE
	}
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];

		if ( pUnitViewerUI == NULL )
			continue;

		pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );

		if ( pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			if ( m_pSelectUnit != NULL )
			{
				if ( pUnitViewerUI == GetUnitViewer( m_pSelectUnit ) )
					continue;
			}
			pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}


	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
}




void CX2StateServerSelect::ClearUnitViewerList()
{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
#ifdef SERV_UNIT_WAIT_DELETE
	if( m_pUnitViewerUI != NULL )
	{
		m_pUnitViewerUI->GetUnit()->AccessUnitData().m_bDeletedMotionPlay = false;
	}	
#endif SERV_UNIT_WAIT_DELETE

	CX2UnitViewerUI::DeleteKTDGObject( m_pUnitViewerUI );
	m_pUnitViewerUI = NULL;
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
		CX2UnitViewerUI::DeleteKTDGObject( pUnitViewerUI );
	}
	m_UnitViewerUIList.resize(0);
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
}

CX2UnitViewerUI* CX2StateServerSelect::GetUnitViewer( CX2Unit* pUnit )
{
	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
		if ( pUnitViewerUI->GetUnit() == pUnit )
			return pUnitViewerUI;
	}

	return NULL;
}




void CX2StateServerSelect::DownAnimFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bPlayDownAnim == true )
	{
		m_fDownAnimTime += fElapsedTime;

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh	
		if ( m_pUnitViewerUI != NULL && m_pUnitViewerUI->GetShowObject() == false )
		{
			if ( m_fDownAnimTime > 0.f )
			{
				m_pUnitViewerUI->SetShowObject( true );
				m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );
				m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
				m_bPlayDownAnim = false;
			}
		}
#else  // REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh

		for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
		{
			CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
			if ( pUnitViewerUI != NULL && pUnitViewerUI->GetShowObject() == false )
			{
				if ( m_fDownAnimTime > (i * 0.4f) )
				{
					pUnitViewerUI->SetShowObject( true );
					pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );
					pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
				}
			}
		}

		if ( m_fDownAnimTime >= ( (float)g_pData->GetMyUser()->GetUnitNum() * 0.5f + 1.0f ) )
		{
			m_bPlayDownAnim = false;
		}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	}
}

void CX2StateServerSelect::PlayDownAnim()
{
	m_bPlayDownAnim = true;
	m_fDownAnimTime = 0.0f;
}




bool CX2StateServerSelect::Handler_EGS_STATE_CHANGE_FIELD_REQ(bool bForceMove)
{
	KEGS_STATE_CHANGE_FIELD_REQ kPacket;
	D3DXVECTOR3 vStartPos(0, 0, 0);
	int startPos = 0;

	const CX2Unit::UnitData* pUnitData = NULL;
	if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
	{
		pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
	}

	if( NULL != pUnitData )
	{
		int iMapId = pUnitData->m_nMapID;

		if( g_pData->GetLocationManager()->GetVillageMapTemplet((SEnum::VILLAGE_MAP_ID)iMapId) == NULL || 
			bForceMove == true )
		{
			// ���� �纥�̵�
			iMapId = SEnum::VMI_RUBEN;
			//vStartPos = D3DXVECTOR3(-16.f, 1445.f, -2873.f);
			startPos = 1;
		}	
		else
		{
			if( pUnitData->m_EXP <= 0 )
			{
				// �ʱ��������� ����.

				// ���� �纥�̵�
				iMapId = SEnum::VMI_RUBEN;
				startPos = 1;
			}
			else
			{
				startPos = g_pData->GetLocationManager()->GetLoginPos( iMapId );

				// �α׾ƿ��ߴ� ��ġ�� ����Ѵ�.
#ifdef REMEMBER_LOGOUT_POSITION_TEST
#ifdef FIELD_BOSS_RAID
				if( false == pUnitData->m_bIgnoreLastTouch  )				
#endif // FIELD_BOSS_RAID
				{
					const CX2World::WorldData* pWorldData = g_pData->GetWorldManager()->GetWorldData( (CX2World::WORLD_ID) g_pData->GetLocationManager()->GetWorldID( (SEnum::VILLAGE_MAP_ID) iMapId ) );
					if( NULL != pWorldData )
					{
						CX2World* pWorld = g_pData->GetWorldManager()->CreateWorld( pWorldData->worldID ); // fix!! ���� �������� �ʰ� ���θ� ���� ������ �� �ְ� �����ؾ���
						if( NULL != pWorld )
						{
							pWorld->SetShowObject( false );
							CKTDGLineMap* pLineMap = pWorld->GetLineMap();
							if( NULL != pLineMap )
							{
								const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( pUnitData->m_ucLastTouchLineIndex );
								if( NULL != pLineData )
								{
									float fLastPosValue = halfToFloat( pUnitData->m_usLastPosValue );
									if( fLastPosValue < 0.f )
										fLastPosValue = 0.f;
									if( fLastPosValue > 1.f )
										fLastPosValue = 1.f;

									vStartPos = pLineData->startPos * (1.f - fLastPosValue) + pLineData->endPos * fLastPosValue;
									vStartPos = pLineMap->GetLandPosition( vStartPos, LINE_RADIUS, (int*) NULL );

									startPos = -1;
								}
							}
							SAFE_DELETE( pWorld );
						}
					}
				}
#endif REMEMBER_LOGOUT_POSITION_TEST
			}		
		}	


		if( startPos >= 0 )
		{
			vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(startPos);	
		}

		g_pData->GetLocationManager()->SetLastPos(vStartPos);
		//g_pData->GetLocationManager()->SetLastPosLineIndex();


		kPacket.m_iMapID =  (int)iMapId;	


		g_pData->GetLocationManager()->SetVillage( (SEnum::VILLAGE_MAP_ID)kPacket.m_iMapID, vStartPos );

		g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_STATE_CHANGE_FIELD_ACK );

	}

	return true;
}

bool CX2StateServerSelect::Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_FIELD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_FIELD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef REFORM_ENTRY_POINT
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ), L"DLG_UI_Selection_MessageBox_No_Button.lua" );
#else	// REFORM_ENTRY_POINT
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );			
#endif	// REFORM_ENTRY_POINT

			bool bCanCreateWorld = true;
			CX2World::WORLD_ID	m_iWorldId = CX2World::WI_NONE;
			SEnum::VILLAGE_MAP_ID m_villageId = SEnum::VMI_INVALID;;
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
//#if 0
//				g_pTFieldGame = new CX2TFieldGame(m_iWorldId);
//				
//				if(g_pTFieldGame != NULL)
//				{
//					g_pTFieldGame->AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
//					g_pTFieldGame->SetAddedMyUnit(true);					
//				}
//#endif

				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, NULL, false );
			}
		}
		else
		{		
			switch ( kEvent.m_iOK )
			{
			case NetError::ERR_FIELD_05:
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_REPLACED_STRING((STR_ID_18892, "i",kEvent.m_iLimitLevel)), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING((STR_ID_18892, "i",kEvent.m_iLimitLevel)) , g_pMain->GetNowState());
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����


				} break;
			case NetError::ERR_FIELD_06:
				{
					if( NULL != g_pData && NULL != g_pData->GetDungeonManager() )
					{
						wstring DungeonName = g_pData->GetDungeonManager()->MakeDungeonNameString( static_cast<SEnum::DUNGEON_ID>(kEvent.m_iRequireDungeonID));
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_REPLACED_STRING((STR_ID_18893, "L", DungeonName )), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 	GET_REPLACED_STRING((STR_ID_18893, "L", DungeonName )),	g_pMain->GetNowState());
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����		
					
					}
				} break;
			}

			// ���� �纥�̵�
			Handler_EGS_STATE_CHANGE_FIELD_REQ(true);

			// ������ġ�� ����
			//g_pData->GetLocationManager()->RestoreVillage();
		}

	}
	return true;
}

bool CX2StateServerSelect::Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_USER_LIST_IN_SECTOR_NOT kEvent;

	DeSerialize( pBuff, &kEvent );		

	if(g_pTFieldGame != NULL)
	{
		for(int i=0; i<(int)kEvent.m_vecNewUserList.size(); ++i)
		{			
			g_pTFieldGame->JoinFieldUnit( &kEvent.m_vecNewUserList[i] );
		}			
	}

	return true;
}







void CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_REQ()
{
	KEGS_CREATE_TUTORIAL_ROOM_REQ kEGS_CREATE_ROOM_REQ;

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomType			= CX2Room::RT_DUNGEON;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName			= L"tutorial";
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic			= false;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword		= L"a*h97lk2";
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot			= 1;
	
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_DifficultyLevel	= CX2Dungeon::DL_NORMAL;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime			= 19999.0f;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_cGetItemType		= CX2DungeonRoom::DGIT_RANDOM;

	switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
	{
	default:
	case CX2Unit::UT_ELSWORD:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_ELSWORD;
		} break;

	case CX2Unit::UT_ARME:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_AISHA;
		} break;

	case CX2Unit::UT_LIRE:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_LENA;
		} break;

	case CX2Unit::UT_RAVEN:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_RAVEN;
		} break;

	case CX2Unit::UT_EVE:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_EVE;		
		} break;

		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_CHUNG;
		} break;
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_ARA;
		} break;
#endif
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS:
		{
#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
			return;
#endif SERV_ELISIS_PREVIOUS_SIS_EVENT
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_EL;
		} break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CX2Unit::UT_ADD:		/// ������ �ʿ� �ϴ�.
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= SEnum::DI_BATTLE_FIELD_TUTORIAL_ADD;
		} break;
#endif //SERV_9TH_NEW_CHARACTER
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_TUTORIAL_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
	g_pMain->AddServerPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, 40.f );	
}

bool CX2StateServerSelect::Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
/*
			int startPos = 0;
			int iMapId;			
			if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
				iMapId = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_nMapID;

			if(g_pData->GetLocationManager()->GetVillageMapTemplet((SEnum::VILLAGE_MAP_ID)iMapId) == NULL)
			{
				// ���� �纥�̵�
				iMapId = SEnum::VMI_RUBEN;
				//vStartPos = D3DXVECTOR3(-16.f, 1445.f, -2873.f);
				startPos = 1;
			}	
			else
			{
				startPos = g_pData->GetLocationManager()->GetLoginPos( iMapId );		
			}	
	
			D3DXVECTOR3 vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(startPos);
			g_pData->GetLocationManager()->SetLastPos(vStartPos);
			//g_pData->GetLocationManager()->SetLastPosLineIndex();

			g_pData->GetLocationManager()->SetVillage((SEnum::VILLAGE_MAP_ID)iMapId, vStartPos);
*/

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
			if(g_pInstanceData != NULL)
				g_pInstanceData->SetFirstJoinVillage(true);
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_645 ), L"DLG_UI_Selection_MessageBox_No_Button.lua", L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_645 ) );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false );	
			
			//���� ������ġ ����
			if( NULL != g_pData && NULL != g_pData->GetPartyManager() )
				g_pData->GetPartyManager()->RememberComeBackInfoBeforeInstanceGameStart();

			if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
			{	//Ʃ�丮�� ������� ���̵�
				g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_01, true );
			}
			return true;
		}
	}

	if( true == g_pMain->GetPlayFieldDungeon() )
		g_pMain->SetPlayFieldDungeon( false );

	return false;
}








//////////////////////////////////////////////////////////////////////////
// statelogin�� �ִ� �ڵ� �Űܿ� ��
bool CX2StateServerSelect::Handler_EGS_CONNECT_REQ( const wstring& wstrGameServerIP, const int iGameServerPort )
{

	if( true == wstrGameServerIP.empty() )
		return false;

	if( true == m_bSentEGS_CONNECT_REQ )
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
			#ifdef	SERV_OTP_AUTH
				// ������ �ϵ���� ID�� ȹ��
				WCHAR lpNxHWID[NXHWID_SIZE] = { 0, };
				CNMCOClientObject::GetInstance().GetNxHWID( lpNxHWID );
				g_pInstanceData->SetNxHWID( lpNxHWID );
			#endif	SERV_OTP_AUTH
				//}} kimhc // 2010-06-24 // OTP �۾�

			} break;
#endif _NEXON_KR_
#ifdef CLIENT_COUNTRY_TWHK
		case CX2Main::XP_GAMANIA_TW:
		case CX2Main::XP_GAMANIA_HK:
			{
				//ID/Pass�� �̹� �ν��Ͻ� �����Ϳ� ��Ʈ�Ǿ� �ִ�.
				//g_pInstanceData->SetUserID( g_pMain->GetArgvID() );
				//g_pInstanceData->SetUserPassword( g_pMain->GetArgvPass() );

			} break;
#endif CLIENT_COUNTRY_TWHK
		}
	}



	if( LoginErrorCheck() == true )
	{
		//���ӽõ�
		//g_pData->ResetServerProtocol();
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
		if ( g_pData->GetServerProtocol()->IsConnected () )
		{
			g_pData->GetServerProtocol()->DisconnectFromGameServer();
		}
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

		if( true == g_pData->GetServerProtocol()->ConnectedToGameServer( wstrGameServerIP.c_str(), iGameServerPort ) )
		{
			g_pMain->SetConnectedGameServerIP( wstrGameServerIP );
			g_pMain->SetConnectedGameServerPort( iGameServerPort );

			KEGS_CONNECT_REQ kPacket;
			kPacket.m_wstrVersion = g_pMain->GetClientVersion();

			g_pData->GetServerProtocol()->SendPacket( EGS_CONNECT_REQ, kPacket );
			g_pMain->AddServerPacket( EGS_CONNECT_ACK, 60.0f );

			m_bSentEGS_CONNECT_REQ = true;

			SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_725 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_725 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����


			g_pInstanceData->SetConnectChannelServerID( m_iTryConnectChannelID );
			m_iTryConnectChannelID = -1;

	
			// ���� ���������� game_server ip�� �����Ѵ�. (�������Խ� TRServer ���ӿ�)
			if(g_pMain != NULL)
			{
				g_pMain->SetTRServerIp( wstrGameServerIP );
			}

			// ä�� �������� ������ ���´�.
			Handler_ECH_DISCONNECT_REQ();
		}
		else
		{
#ifdef _SERVICE_
	#ifndef FIX_REFORM_ENTRY_POINT_9TH 
			m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_726 ), this );
	#endif // FIX_REFORM_ENTRY_POINT_9TH 
#else

#endif _SERVICE_

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
			if ( NULL == m_pDLGReConnectChannelServer )
			{

		#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
				if ( true == m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp )
				{	
					m_bCanNotConenctToBusyServer = true;
					m_sbSelectedServerSet = false;
					// ConnectToChannelServer(true);
					g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_SERVER_SELECT, NULL, false );						

					if ( g_pData->GetServerProtocol()->IsConnected() == true )
					{
						g_pData->GetServerProtocol()->DisconnectFromGameServer();
					}
				}
				else
		#endif // FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
				{
					m_pDLGReConnectChannelServer = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ),  GET_STRING ( STR_ID_29884 ), 
						SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK, this, SUCM_EXIT, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
				}
		#ifdef PLAY_PROMOTION_MOVIE //kimjh
				if ( true == g_pMain->m_bPlayIntroMovie )
					m_pDLGReConnectChannelServer->SetShow ( false );
		#endif // PLAY_PROMOTION_MOVIE //kimjh		
			}
		#ifdef FIX_REFORM_ENTRY_POINT_9TH 
			else
			{
				m_pDLGReConnectChannelServer = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ),  GET_STRING ( STR_ID_29884 ), 
					SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK, this, SUCM_EXIT, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
			}		
		#endif // FIX_REFORM_ENTRY_POINT_9TH 

	#else  // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
			m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_726 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��					
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			m_pMsgOkFailToConnectServer = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_726 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		}
	}
	else
	{
		//g_pData->DeleteServerProtocol();
		//g_pData->DeleteMyUser();		
		g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_INVALID_ID_OR_PASSWORD );
		g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
	}

#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp = false;
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	return true;
}

bool CX2StateServerSelect::Handler_EGS_CONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CONNECT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	
	if( g_pMain->DeleteServerPacket( EGS_CONNECT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
//#ifdef _SERVICE_
//			OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck3" );
//#endif
			g_pMain->SetServerUDPPort( kEvent.m_UDPPort );

#ifdef UDP_CAN_NOT_SEND_USER_KICK
			g_pInstanceData->SetUDPKickOff(kEvent.m_bUDPKickOff);
#endif //UDP_CAN_NOT_SEND_USER_KICK
			Handler_EGS_VERIFY_ACCOUNT_REQ();
		}
		else
		{
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
					
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh

	#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
					m_bReservedConnectionToChannelServer = true;
					if ( NULL == m_pDLGReConnectChannelServer )
					{
						SAFE_DELETE_DIALOG ( m_pDLGMsgBox );
						
						m_bCanNotConenctToBusyServer = true;
						m_sbSelectedServerSet = false;
						// ConnectToChannelServer(true);
						g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_SERVER_SELECT, NULL, false );						
						
						if ( g_pData->GetServerProtocol()->IsConnected() == true )
						{
							g_pData->GetServerProtocol()->DisconnectFromGameServer();
						}

						if ( true == g_pMain->GetIsPlayingTutorial() ) 
						{
							g_pMain->SetIsPlayingTutorial( false );
						}

	#ifndef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����						
						else
						{
							m_pDLGReConnectChannelServer = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ),  GET_STRING ( STR_ID_29884 ), 
								SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK, this, SUCM_EXIT, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
						}
	#endif // FIX_REFORM_ENTRY_POINT_4TH			// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	
	#ifdef PLAY_PROMOTION_MOVIE //kimjh
						if ( true == g_pMain->m_bPlayIntroMovie )
							m_pDLGReConnectChannelServer->SetShow ( false );
	#endif // PLAY_PROMOTION_MOVIE //kimjh

					}
	#else // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
					m_bReservedConnectionToChannelServer = true;
					ConnectToChannelServer(true);
//					Handler_ECH_DISCONNECT_REQ ();
	#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��


#else  // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh					

					m_bSentEGS_CONNECT_REQ = false;
					m_bConnectFullChannel = true;
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh					

					//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�����ο��� ���Ƽ� ���� �� �� �����ϴ�.", this );
				} break;
			}
		}
	}
	return true;
}

bool CX2StateServerSelect::Handler_EGS_VERIFY_ACCOUNT_REQ()
{
	KEGS_VERIFY_ACCOUNT_REQ kPacket;

#ifdef _SERVICE_ 
	ELSWORD_VIRTUALIZER_START
#endif

#ifndef CLIENT_PURPLE_MODULE
	kPacket.m_wstrUserID = g_pInstanceData->GetUserID();
	kPacket.m_wstrPasswd = g_pInstanceData->GetUserPassword();
#else // CLIENT_PURPLE_MODULE
	bool bUsePurpleModule = true;
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	if(g_pInstanceData->GetInHouseAccount())
		bUsePurpleModule = false;
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

	if( bUsePurpleModule )
	{
		ConvertCharToWCHAR( kPacket.m_wstrUserID, PurpleGetUserID() );
#ifdef SERV_KOG_OTP_VERIFY
		if (g_pInstanceData->IsIDVerified())
			kPacket.m_wstrPasswd = g_pInstanceData->GetUserPassword();
		else
			ConvertCharToWCHAR( kPacket.m_wstrPasswd, PurpleGetAuthString() );
#else //SERV_KOG_OTP_VERIFY
		ConvertCharToWCHAR( kPacket.m_wstrPasswd, PurpleGetAuthString() );
#endif //SERV_KOG_OTP_VERIFY
	}
	else
	{
		kPacket.m_wstrUserID = g_pInstanceData->GetUserID();
		kPacket.m_wstrPasswd = g_pInstanceData->GetUserPassword();
	}
#endif // CLIENT_PURPLE_MODULE
	
#ifdef SERV_COUNTRY_TH
	kPacket.m_wstrSocketID = g_pInstanceData->GetSocketID();
#endif // SERV_COUNTRY_TH

#ifdef SERV_JAPAN_CHANNELING 
	char szBuffer[256] = {0};

#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	if( g_pInstanceData->GetInHouseAccount() == false )
	{
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

		PurpleGetValue("site", szBuffer, 255);

#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	}
	else
	{
		int index = kPacket.m_wstrUserID.find( L'@' );
		if( index != -1 )
		{
			wstring wstrChannelCode = kPacket.m_wstrUserID.substr( index+1 , kPacket.m_wstrUserID.length()-1 );
			string strChannelCode;
			ConvertWCHARToChar( strChannelCode, wstrChannelCode.c_str() );
			sprintf(szBuffer, "%s", strChannelCode.c_str() );
		}
	}
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

	if( strcmp(szBuffer, "MS") == 0 )
		kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_MSN;
	else if( strcmp(szBuffer, "NICO") == 0 )
		kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_NICO;
	else if( strcmp(szBuffer, "NX") == 0 )
		kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_NEXON;
	else
		kPacket.m_iChannelingCode = KNexonAccountInfo::JCE_NHN;
#endif // SERV_JAPAN_CHANNELING

#ifdef SERV_STEAM
	// ChannelingCode �� ����
	// 0 : �Ϲ� 1 : ���� 2 : ���� �Ϲ� 3: ���� ����
	kPacket.m_iChannelingCode = g_pInstanceData->GetChannelingCode();
#endif SERV_STEAM

	//{{ kimhc // 2010-06-24 // OTP �۾�
#ifdef	SERV_OTP_AUTH
	kPacket.m_wstrHWID = g_pInstanceData->GetNxHWID();
#endif	SERV_OTP_AUTH
	//}} kimhc // 2010-06-24 // OTP �۾�

#ifdef ARGUMENT_LOGIN

#ifdef _SERVICE_MANUAL_LOGIN_
	kPacket.m_bDebugAuth = g_pMain->GetManualLogin();		
#else _SERVICE_MANUAL_LOGIN_
#if defined( _IN_HOUSE_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ )
	kPacket.m_bDebugAuth = true;						// �系�� true
#else 
	kPacket.m_bDebugAuth = false;						// �ڵ�/���� �α����� ��� ������ ����ϹǷ� ���� ������ ��� ������ false
#endif // _IN_HOUSE_ || _IN_HOUSE_SERVICE_READY_QA_
	
#endif _SERVICE_MANUAL_LOGIN_

#else ARGUMENT_LOGIN
	kPacket.m_bDebugAuth = g_pMain->GetManualLogin();
#endif ARGUMENT_LOGIN
	
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK 
#ifdef MACHINE_ID
	kPacket.m_strMachineID = g_pMain->GetMachineId();

	if( g_pInstanceData != NULL )
		g_pInstanceData->SetMachineId( kPacket.m_strMachineID );
#endif
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	if( g_pInstanceData != NULL && g_pInstanceData->GetSN() != NULL )
		memcpy(kPacket.m_charServerSN, g_pInstanceData->GetSN(), sizeof(unsigned char) * SERVER_SN);
#endif

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	if( g_pInstanceData != NULL )
		kPacket.m_kChannelRandomKey = g_pInstanceData->GetChannelKey();
#endif

#ifdef SERV_CHECK_MACHINE_LOCALTIME
	CTime tCurTime = CTime::GetCurrentTime();
	kPacket.m_wstrClientTime = (const wchar_t*)tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif

	m_bEGS_VERIFY_ACCOUNT_REQ = true;

#ifdef SERV_STEAM
	kPacket.m_bSteamClient = CX2Steam::IsSteamUser();
#endif //SERV_STEAM

#ifdef SERV_CHANNELING_AERIA
	kPacket.m_bAeriaClient = g_pInstanceData->IsAeriaChanneling();
#endif //SERV_CHANNELING_AERIA

	g_pData->GetServerProtocol()->SendPacket( EGS_VERIFY_ACCOUNT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_VERIFY_ACCOUNT_ACK, 60 );

#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	if( kPacket.m_strMachineID.empty() == true )
	{
#ifdef ADD_COLLECT_CLIENT_INFO
		if( g_pMain != NULL )
			g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_33, "", false, false );
#else
		g_pMain->SendHackMail_VERIFYVALUE( ANTI_HACK_STRING_AntiHacking_MACHINE_ID_EMPTY );
#endif
	}
#endif

#ifdef _SERVICE_ 
	ELSWORD_VIRTUALIZER_END
#endif

	return true;	
}

bool CX2StateServerSelect::Handler_EGS_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef _SERVICE_ 
	ELSWORD_VIRTUALIZER_START
#endif

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_VERIFY_ACCOUNT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_VERIFY_ACCOUNT_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_EVENT_MONEY
			//���� ����â�� ������ �˾� ��� ���� üũ 2013-07-02 ��â�� 
			if( kEvent.m_iEventMoney < 1 )
				//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(50,300), GET_STRING( STR_ID_25386 ), g_pMain->GetNowState(), -1, -1.f, L"", D3DXVECTOR2(0,250) );
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 270), GET_STRING( STR_ID_29369 ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2(0, -100));
#endif //SERV_EVENT_MONEY

			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(260, 270), GET_STRING( STR_ID_29369 ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_OKMsgBoxPlusNew.lua", D3DXVECTOR2(0, -100));

#ifdef EVENT_NEW_HENIR
			//���� ����â�� ������ �˾� ��� 
// 			{
// 				static bool bIsAlreadyShow = false;
// 				if( false == bIsAlreadyShow )
// 				{
// 					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_25386 ), g_pMain->GetNowState(), -1, -1.f, L"DLG_UI_OKMsgBoxPlus.lua" );
// 					bIsAlreadyShow = true;
// 				}
// 			}
#endif // EVENT_NEW_HENIR

#if defined(SERV_MACHINE_ID_BLOCK) && defined(MACHINE_ID)
			ELSWORD_VIRTUALIZER_START
			Handler_EGS_CHECK_MACHINE_ID_REQ();
			ELSWORD_VIRTUALIZER_END
#endif

//#ifdef _SERVICE_
//			OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck4" );
//#endif

#ifdef SERV_KOG_OTP_VERIFY
			// �������� �߱޹��� OTP�� Ŭ���̾�Ʈ�� �����Ѵ�.
			if( false == kEvent.m_kAccountInfo.m_wstrOTP.empty() )
			{
				g_pInstanceData->SetIDVerified(true);
				g_pInstanceData->SetUserPassword( kEvent.m_kAccountInfo.m_wstrOTP );	
			}
#endif // SERV_KOG_OTP_VERIFY			

#ifdef _SERVICE_
#ifdef _NEXON_KR_
			//OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck5" );
			OpenGlobalURL( L"http://elsword.nexon.com/wiselog/gameinstall.aspx?section=check5" );
#endif
#endif
			//////////////////////////////////////////////////////////////////////////
			//{{ 2009. 7. 25  ������	üũ�޽���
#ifdef HENIR_TEST
			if( kEvent.m_kAccountInfo.m_iAuthLevel == CX2User::XUAL_DEV )
			{
				//MessageBox( g_pKTDXApp->GetHWND(), L"��ϸ� �ð� �׽�Ʈ ����Դϴ�! ���� ������ ������ġ�ϸ� �ȵǴ� �׽�Ʈ �����Դϴ�!", L"�˸�!", MB_OK );
			}
#endif HENIR_TEST
			//}}
			//////////////////////////////////////////////////////////////////////////

			wstringstream wstrstm;
			wstrstm << GET_REPLACED_STRING( ( STR_ID_728, "L", kEvent.m_kAccountInfo.m_wstrID ) );
			g_pMain->SetMailNameToFindHack( wstrstm.str().c_str() );
#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
            {
                std::string strID;
                ConvertWCHARToChar( strID, kEvent.m_kAccountInfo.m_wstrID );
                g_pMain->SetUserIdToFindHack_ThreadSafe( strID );
            }
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
			g_pMain->SetUserIdToFindHack( kEvent.m_kAccountInfo.m_wstrID.c_str() );
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
#ifdef SERV_CLIENT_PUBLIC_IP
			g_pMain->SetPublicIp( kEvent.m_wstrClientIP );
#endif
			if(kEvent.m_cHackingUserType == CX2User::HUT_AGREE_HACK_USER)
				g_pMain->SetHackingUser(true);

			m_bEGS_VERIFY_ACCOUNT_ACK = true;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			if ( false == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
            {
			    CX2User::UserData kUserData;
			    kUserData = kEvent;
			    g_pData->ResetMyUser( kUserData );
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
				Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ ();
				//Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
				//Handler_EGS_CHARACTER_LIST_REQ();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

            }

			//m_MaxUnitNum	= g_pData->GetMyUser()->GetUserData().maxUnitCount;
			//const int SLOT_COUNT_FOR_CREATE_UNIT = 1;
			//m_MaxPage		= (g_pData->GetMyUser()->GetUnitNum()+SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;



#ifndef DISABLE_DISAGREE_HACK_USER
			if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_DISAGREE_HACK_USER )
			{


#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	#ifdef FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
				if ( g_pMain->m_bPlayIntroMovie == true )
				{
					m_bReservedHackingUserAgreeDlg = true;
				}
				else
	#endif // FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
				{
					m_pDLGCheckHack = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), 
						GET_STRING( STR_ID_729 ), 
						SSSUCM_HACK_USER_PROCESS_CHECK_OK, this, SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL, L"DLG_UI_Selection_MessageBox_Ok_Exit_Button_New.lua", D3DXVECTOR2( 0, 10 ), L"UI_PopUp_Negative_01.ogg" );
					// MsgBox �� ESC �� ������ Ư���� ������ �ִ�, ������ �� â�� ��� �����Ǿ�� �ǴܵǾ�
					// �ش� ���� ���� false �� �����Ѵ�.	kimjh
					if ( NULL != m_pDLGCheckHack )
						m_pDLGCheckHack->SetOKAndCancelMsgBox( false );
	#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					m_pDLGCheckHack = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
						GET_STRING( STR_ID_729 ), 
						SSSUCM_HACK_USER_PROCESS_CHECK_OK, this, SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL );
	#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����				
				}
			}
#if 0 // ���ӽ� ���ǽ� ���ö�� ������ ������ �����Ƿ� ����
			else if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
			{
#ifndef PROCESSLIST
				g_pMain->UpdateProcessList();
#endif
				g_pMain->SendHackMail();
			}
#endif
#endif DISABLE_DISAGREE_HACK_USER
		
			/// 2014-01-17 // itexpertkim // ����ö // ���̹� ä�θ� �̽��� �ٽ� ���
#ifdef SERV_NAVER_CHANNELING
			g_pData->GetMyUser()->SetNexonSN( kEvent.m_uiNexonSN );
#endif // SERV_NAVER_CHANNELING




			g_pMain->GetGameOption().SetPlayGuide( kEvent.m_kAccountInfo.m_kAccountOption.m_bPlayGuide, false );

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
			if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
			{
				if( false == g_pMain->GetNexonLoginMessenger() )
				{


					if ( g_pMain->GetNexonLogin() == false )				// �系�����̸�, ��, ID, password ���� �Է��ϰ� �α����� ���
					{
						if( false == g_pMain->GetNexonLoginMessenger() )
						{
							NMLoginAuthReplyCode resultAuth = CNMCOClientObject::GetInstance().LoginAuth( g_pInstanceData->GetUserID().c_str(), g_pInstanceData->GetUserPassword().c_str() );
							
							switch( resultAuth )
							{
							case kLoginAuth_OK:
								{
									UINT32 supportFlag = 0;

									supportFlag |= kSupportFlag_ChatSession;
									supportFlag |= kSupportFlag_AddFriendConfirm;
									supportFlag |= kSupportFlag_RequestSession;
									supportFlag |= kSupportFlag_ServerMessage;
									if ( CNMCOClientObject::GetInstance().LoginMessenger( kSupportFlag_All  ) == FALSE )
									{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
										if ( false == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
											g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_730 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg"  );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_730 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	
									
									}
									else
									{
										g_pMain->SetNexonLoginMessenger( true );
									}
								}
								break;


							case kLoginAuth_WrongID:
								{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
									if ( false == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_731 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_731 ), g_pMain->GetNowState() );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
									
								}
								break;

							case kLoginAuth_WrongPassword:
								{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
									if ( false == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_732 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_732 ), g_pMain->GetNowState() );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
								}
								break;

							case kLoginAuth_ServiceShutdown:
								{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
									if ( false == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_733 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_733 ), g_pMain->GetNowState() );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
								}
								break;

							default:
								{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
									if ( false == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_734 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_734 ), g_pMain->GetNowState() );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
								}
								break;
							}
						}
					}
					else // ����, �����׼� ���� �����̸� 
					{
						UINT32 supportFlag = 0;

						supportFlag |= kSupportFlag_ChatSession;
						supportFlag |= kSupportFlag_AddFriendConfirm;
						supportFlag |= kSupportFlag_RequestSession;
						supportFlag |= kSupportFlag_ServerMessage;
						if ( CNMCOClientObject::GetInstance().LoginMessenger( kSupportFlag_All  ) == FALSE )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_730 ), this );
						}
						else
						{
							g_pMain->SetNexonLoginMessenger( true );
						}
					}
				}
			}
#endif // #ifndef NEW_MESSENGER
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP



#ifdef SERV_COUNTRY_PH
#else //SERV_COUNTRY_PH
#ifndef NO_GAMEGUARD
			if ( CheckNPHackUser() == false )
			{
				CloseNPGameMon();
			}

#ifdef ALWAYS_INTERNAL_NPGE_PACKING
			if( KNP_Init( g_pKTDXApp->GetHInstance(), g_pMain->GetClinetArg() ) == false )
#else //ALWAYS_INTERNAL_NPGE_PACKING
			if( KNP_Init( g_pKTDXApp->GetHInstance() ) == false )
#endif ALWAYS_INTERNAL_NPGE_PACKING
			{
				g_pKTDXApp->SetFindHacking( true );
			}

			KNP_SetHwnd( g_pKTDXApp->GetHWND() );

			//���Ӽ����� �ϸ� nProtect�� ���̵� ��Ͻ�Ų��.
			KNP_Send( g_pInstanceData->GetUserID().c_str() );
			//���Ӱ��� �Ϸ�
#endif // NO_GAMEGUARD
#endif //SERV_COUNTRY_PH

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			//���� pc ��� ���� ����
			_SendSystemInfo();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK


		}
		else
		{
			StateLog( L"���" );

			//g_pData->DeleteServerProtocol();
			//g_pData->DeleteMyUser();
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL 
			switch ( kEvent.m_iOK )
			{
			case NetError::ERR_VERIFY_00:
			case NetError::ERR_VERIFY_01:
				{
#ifdef SIMPLE_BUG_FIX
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), NetError::GetErrStrF( NetError::ERR_USER_REGISTRATION_00 ), g_pMain->GetNowState(),SSSUCM_DOUBLE_CONNECT);
#else
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_DOUBLE_CONNECT );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
#endif SIMPLE_BUG_FIX
				} break;
			case NetError::ERR_VERIFY_24:
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					m_pDLGATLLimitCheckMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_17745 ), this, SSSUCM_ATL_LIMIT_CHECK_OK, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					m_pDLGATLLimitCheckMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_17745 ), g_pMain->GetNowState(), SSSUCM_ATL_LIMIT_CHECK_OK );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����	
					
				
				
				} break;
			default:
				{
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_VERIFY_ACCOUNT );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;
			}
#else
			if ( kEvent.m_iOK == NetError::ERR_VERIFY_00 ||
				kEvent.m_iOK == NetError::ERR_VERIFY_01 )
			{
				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_DOUBLE_CONNECT );
			}
			else
				g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_VERIFY_ACCOUNT );

#ifdef LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
			m_bSentEGS_CONNECT_REQ = false;
			m_bEGS_VERIFY_ACCOUNT_REQ = false;

			switch( kEvent.m_iOK )
			{
			case NetError::ERR_CONNECT_04:
				break;
			default:
				g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				break;
			}
#else
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
#endif LOGIN_STATE_NOEXIT_WHEN_DISCONNECTED
#endif //SERV_CHANGE_NEXON_AUTH_ATL_LEVEL 

		}
	}


#ifdef _SERVICE_ 
	ELSWORD_VIRTUALIZER_END
#endif

	return true;
}

bool CX2StateServerSelect::Handler_ENX_USER_LOGIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KENX_USER_LOGIN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bENX_USER_LOGIN_NOT = true;
	m_bIsGameBang = kEvent.m_bIsGameBang;

#ifdef SERV_PC_BANG_TYPE 	// �������� ���� pc�� ������ Ŭ�� ������ �Ӵϴ�.
	if ( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )
		g_pData->GetPremiumBuffManager()->SetPcBangReward( kEvent.m_kPcBangReward );
#endif SERV_PC_BANG_TYPE

#ifdef COME_BACK_REWARD
	if ( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )
		g_pData->GetPremiumBuffManager()->SetPremiumPC( m_bIsGameBang );
#endif

	return true;
}

bool CX2StateServerSelect::Handler_KXPT_PORT_CHECK_REQ()
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
    return g_pData->GetGameUDP()->SendToIP( CKTDNUDP::ConvertIPToAddress( g_pMain->GetConnectedGameServerIP() ), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, 
        &kXPT_PORT_CHECK_REQ, sizeof(kXPT_PORT_CHECK_REQ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	KSerBuffer buff;
//	Serialize( &buff, &kXPT_PORT_CHECK_REQ );
//	return g_pData->GetGameUDP()->Send( g_pMain->GetConnectedGameServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//	//return g_pData->GetGameUDP()->Send( g_pMain->GetServerIP(), g_pMain->GetServerUDPPort(), XPT_PORT_CHECK_REQ, (char*)buff.GetData(), buff.GetLength() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

bool CX2StateServerSelect::Handler_KXPT_PORT_CHECK_ACK( const KXPT_PORT_CHECK_ACK& kXPT_PORT_CHECK_ACK )
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

	g_pData->GetGameUDP()->SetMyExtPort( kXPT_PORT_CHECK_ACK.m_Port );

	Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
	return true;
}



/*virtual*/ bool CX2StateServerSelect::Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ()
{
	return CX2State::Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
}

/*virtual*/ bool CX2StateServerSelect::Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_STATE_CHANGE_SERVER_SELECT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			Handler_EGS_CURRENT_TIME_REQ();
			return true;
		}
	}
	return true;
}



bool CX2StateServerSelect::Handler_EGS_CURRENT_TIME_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CURRENT_TIME_REQ );
	g_pMain->AddServerPacket( EGS_CURRENT_TIME_ACK );

	return true;
}

bool CX2StateServerSelect::Handler_EGS_CURRENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CURRENT_TIME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CURRENT_TIME_ACK ) == true )
	{
		g_pData->SetServerCurrentTime( kEvent.m_wstrCurrentTime );
#ifdef SERVER_CURRENT_TIME_INT64
		g_pData->SetServerCurrentTimeInt64(kEvent.m_tCurrentTime);
#endif //SERVER_CURRENT_TIME_INT64
		//StateChangeLoading();
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh

		if ( true == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection &&
			 true == m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection )
		{
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
			Handler_EGS_CHARACTER_LIST_REQ();
#else // FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
			Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );	
#endif //FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )

		}
		else
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
		{
			Handler_EGS_SELECT_SERVER_SET_REQ();
		}


		return true;
	}

	return false;
}


bool CX2StateServerSelect::LoginErrorCheck()
{

	//if( m_UserID.size() < 3 )
	//{
	//	//ID�� 4�� �̸����� �Է��� ���
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"���̵� 4�� �̻� ��������.", this );
	//	return false;
	//}
	//if( m_UserPassword.size() < 4 )
	//{
	//	//Password�� 4�� �̸����� �Է��� ���
	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�н����带 4�� �̻� ��������.", this );
	//	return false;
	//}


	return true;
}



bool CX2StateServerSelect::Handler_ECH_GET_CHANNEL_LIST_REQ()
{
	g_pData->GetServerProtocol()->SendChID( ECH_GET_CHANNEL_LIST_REQ );
	g_pMain->AddServerPacket( ECH_GET_CHANNEL_LIST_ACK, 100.0f );

	return true;
}


bool CX2StateServerSelect::Handler_ECH_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_GET_CHANNEL_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( ECH_GET_CHANNEL_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing;
		}
	}
	return true;
}

bool CX2StateServerSelect::Handler_ECH_GET_CHANNEL_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_GET_CHANNEL_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bReceivedChannelList = true;

#ifdef EXTEND_SERVER_GROUP_MASK
	g_pInstanceData->SetServerGroupID( kEvent.m_iServerGroupID );
#else
	g_pInstanceData->SetServerGroupID( (SERVER_GROUP_ID)kEvent.m_iServerGroupID );
#endif EXTEND_SERVER_GROUP_MASK

#ifdef SERVER_GROUP_UI_ADVANCED
	UpdateServerSelectButton( g_pInstanceData->GetServerGroupID() );
#endif SERVER_GROUP_UI_ADVANCED

#ifdef CHANGE_CHANNEL
	g_pInstanceData->m_vecChannelInfo.resize(0);
#else
	m_vecChannelInfo.resize( 0 );
#endif

	std::map< int, KChannelInfo >::const_iterator cit;
	for( cit = kEvent.m_mapChannelList.begin() ; cit != kEvent.m_mapChannelList.end(); cit++ )
	{
		const KChannelInfo& channelInfo = cit->second;

#ifdef CHANGE_CHANNEL
		if(g_pInstanceData != NULL)
			g_pInstanceData->m_vecChannelInfo.push_back( channelInfo );
#else
		m_vecChannelInfo.push_back( channelInfo );
#endif
	}

#ifdef SERV_CHANNEL_LIST_RENEWAL

	g_pInstanceData->m_mapChannelBonusInfo.clear();

	std::map< int, KChannelBonusInfo >::const_iterator cmit;
	for( cmit = kEvent.m_mapChannelBonusList.begin() ; cmit != kEvent.m_mapChannelBonusList.end(); ++cmit )
	{
		//const KChannelBonusInfo& ChannelBonusInfo = cmit->second;
		if(g_pInstanceData != NULL)
			g_pInstanceData->m_mapChannelBonusInfo.insert(std::make_pair(cmit->first, cmit->second));
	}
#endif //SERV_CHANNEL_LIST_RENEWAL

	//std::sort( m_vecChannelInfo.begin(), m_vecChannelInfo.end(), LessChannelIndex() );

	//{{ kimhc // 2009-12-15 // ������ �߰� �۾�
#ifdef	ADD_SERVER_GROUP
	g_pInstanceData->GetMiniMapUI()->SetChannelComboBox();
	m_bReservedConnectionToChannelServer = false;
#endif	ADD_SERVER_GROUP

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����

	// �ٸ� ���Ӽ������� ������ �������� ��ٸ��� ���̶�� ���Ӽ����� �������� �ʰ� ��ٸ����� �Ѵ�.
	if( true == m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK )		
		return true;
	
	int iLowerUserChannelIndex = -1;
	int iLowerUserChannelCount = INT_MIN;

	for ( int i = 0; i < static_cast<int> ( g_pInstanceData->m_vecChannelInfo.size() ); ++i )
	{
		KChannelInfo channelInfo = g_pInstanceData->m_vecChannelInfo[ i ];
		if ( iLowerUserChannelCount < channelInfo.m_iMaxUser - channelInfo.m_iCurrentUser )
		{
#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
			if ( false == g_pInstanceData->IsConnectedChannel( channelInfo.m_iServerUID ) )
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
			{
				iLowerUserChannelCount = channelInfo.m_iMaxUser - channelInfo.m_iCurrentUser;
				iLowerUserChannelIndex = i;
#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
				g_pInstanceData->SetConnectedChannelID( channelInfo.m_iServerUID );
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
			}
		}
	}
	if ( iLowerUserChannelIndex == -1 || iLowerUserChannelCount == INT_MIN )
	{
		ASSERT( !L"Can't Find Auto Connecting Channel Index" );
		iLowerUserChannelIndex = 0;
#ifdef FIX_REFORM_ENTRY_POINT_10TH			// kimjh, ĳ���� ����Ʈ �������� ������ ����
		return false;
#endif // FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
	}
		
	const KChannelInfo& channelInfo = g_pInstanceData->m_vecChannelInfo[ iLowerUserChannelIndex ];

	m_iTryConnectChannelID = channelInfo.m_iChannelID;

	m_bSelectChannel = true;
	Handler_EGS_CONNECT_REQ( channelInfo.m_wstrIP, channelInfo.m_usMasterPort );
	
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	UpdateChannelListUI();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	
	return true;
}


bool CX2StateServerSelect::Handler_ECH_DISCONNECT_REQ()
{

	if( 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            g_pData->GetServerProtocol() != NULL
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
             g_pData->GetServerProtocol()->GetChUserProxy() != NULL 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->ChUserProxy_SetIntendedDisconnect( true );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetChUserProxy()->SetIntendedDisconnect( true );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}


	g_pData->GetServerProtocol()->SendChID( ECH_DISCONNECT_REQ );
	g_pMain->AddServerPacket( ECH_DISCONNECT_ACK, 100.0f );


	return true;
}


bool CX2StateServerSelect::Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pMain->DeleteServerPacket( ECH_DISCONNECT_ACK ) == true )
	{
		g_pData->GetServerProtocol()->DisconnectFromChannelServer();
	}

	if( true == m_bReservedConnectionToChannelServer )
	{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		ConnectToChannelServer(true);
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		ConnectToChannelServer();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

	}

	return true;
}


bool CX2StateServerSelect::Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ()
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

	m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK = true;
	
	g_pData->GetServerProtocol()->SendID( EGS_DISCONNECT_FOR_SERVER_SELECT_REQ );
	g_pMain->AddServerPacket( EGS_DISCONNECT_FOR_SERVER_SELECT_ACK, 100.0f );


	return true;
}


bool CX2StateServerSelect::Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pMain->DeleteServerPacket( EGS_DISCONNECT_FOR_SERVER_SELECT_ACK ) == true )
	{
		m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK = false;

		g_pData->GetServerProtocol()->DisconnectFromGameServer();


		g_pMain->SetConnectedGameServerIP( L"" );
		g_pMain->SetConnectedGameServerPort( 0 );
	}

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	if ( true == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection )
	{
		if ( -1 == m_iReservedEntryPointServerChannelIndex )
		{
			int iLowerUserChannelIndex = -1;
			int iLowerUserChannelCount = INT_MIN;
			vector< KChannelInfo > * vecChannelInfo = NULL;
			if ( m_pSelectUnit != NULL )
			{
				if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
					vecChannelInfo = &m_vecGaiaChannelInfo;
				else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
					vecChannelInfo = &m_vecSolesChannelInfo;
			}

			if ( vecChannelInfo == NULL )
			{
				ASSERT (!L"Can't Find Server Group ID !" );
#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
				return false;
#else // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
				vecChannelInfo = &m_vecGaiaChannelInfo;
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
				
			}

			for ( int i = 0; i < static_cast<int> ( vecChannelInfo->size() ); ++i )
			{
				KChannelInfo channelInfo = (*vecChannelInfo)[ i ];
#ifdef FIX_REFORM_ENTRY_POINT_10TH			// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
				if ( false == g_pInstanceData->IsConnectedChannel( channelInfo.m_iServerUID ) )
#endif // FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
				{
					if ( iLowerUserChannelCount < channelInfo.m_iMaxUser - channelInfo.m_iCurrentUser )
					{
						iLowerUserChannelCount = channelInfo.m_iMaxUser - channelInfo.m_iCurrentUser;
						iLowerUserChannelIndex = i;
#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
						g_pInstanceData->SetConnectedChannelID( channelInfo.m_iServerUID  );
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
					}						
				}
			}
			if ( iLowerUserChannelIndex == -1 || iLowerUserChannelCount == INT_MIN )
			{
				ASSERT( !L"Can't Find Auto Connecting Channel Index" );
#ifdef FIX_REFORM_ENTRY_POINT_10TH			// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
				return false;
#else // FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
				iLowerUserChannelIndex = 0;
#endif // FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
			}

			const KChannelInfo& channelInfo = (*vecChannelInfo)[ iLowerUserChannelIndex ];
			m_iTryConnectChannelID = channelInfo.m_iChannelID;
			m_bSelectChannel = true;
			Handler_EGS_CONNECT_REQ ( channelInfo.m_wstrIP.c_str(), channelInfo.m_usMasterPort );
		}

#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����
		else if ( -2 == m_iReservedEntryPointServerChannelIndex )
		{
			
			if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
			{
				m_iTryConnectChannelID = FindAndConnectMatchingLevelGameServer( 1, m_mapGaiaChannelBonusInfo, m_vecGaiaChannelInfo);
				BOOST_FOREACH ( const KChannelInfo & kChannelInfo , m_vecGaiaChannelInfo )
				{
					if ( m_iTryConnectChannelID == kChannelInfo.m_iChannelID )
					{
						Handler_EGS_CONNECT_REQ ( kChannelInfo.m_wstrIP.c_str(), kChannelInfo.m_usMasterPort );
					}
				}
			}
			else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
			{
				m_iTryConnectChannelID = FindAndConnectMatchingLevelGameServer( 1, m_mapSolesChannelBonusInfo, m_vecSolesChannelInfo);
				BOOST_FOREACH ( const KChannelInfo & kChannelInfo , m_vecSolesChannelInfo )
				{
					if ( m_iTryConnectChannelID == kChannelInfo.m_iChannelID )
					{
						Handler_EGS_CONNECT_REQ ( kChannelInfo.m_wstrIP.c_str(), kChannelInfo.m_usMasterPort );
					}
				}
			}
			else
			{
				ASSERT( !L"Can't Find Server Group ID !" );
				return false;
			}			
			m_bSelectChannel = true;			
		}

#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����
		else
		{
			vector< KChannelInfo > * vecChannelInfo = NULL;
			if ( m_pSelectUnit != NULL )
			{
				if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
					vecChannelInfo = &m_vecGaiaChannelInfo;
				else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
					vecChannelInfo = &m_vecSolesChannelInfo;
			}

			if ( vecChannelInfo == NULL )
			{
				ASSERT( ! L"Can't Find Server Group ID ! ");
				vecChannelInfo = &m_vecGaiaChannelInfo;
			}

			const KChannelInfo& channelInfo = (*vecChannelInfo)[ m_iReservedEntryPointServerChannelIndex ];
			m_iTryConnectChannelID = channelInfo.m_iChannelID;
			m_bSelectChannel = true;
			Handler_EGS_CONNECT_REQ ( channelInfo.m_wstrIP.c_str(), channelInfo.m_usMasterPort );
		}
	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh


	return true;
}







bool CX2StateServerSelect::Handler_EGS_GET_MY_MESSENGER_SN_REQ( UidType unitUID )
{
	KEGS_GET_MY_MESSENGER_SN_REQ kPacket;
	kPacket = unitUID;

#ifdef _NEXON_KR_
	g_pMain->SetNMVirtualKey( 0, 0 );
#endif // _NEXON_KR_
	

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_MY_MESSENGER_SN_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_MY_MESSENGER_SN_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_2558 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2558 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

	return true;
}

bool CX2StateServerSelect::Handler_EGS_GET_MY_MESSENGER_SN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KEGS_GET_MY_MESSENGER_SN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_GET_MY_MESSENGER_SN_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( true == m_bRemoveUnitAfterReceiveNMSerialNum )
			{
				m_bRemoveUnitAfterReceiveNMSerialNum = false;

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifndef NEW_MESSENGER
				
#ifdef _NEXON_KR_
				g_pMain->SetNMVirtualKey( NEXON_KOREA_ELSWORD_GAMECODE, kEvent.m_uiKNMSerialNum );
#endif // _NEXON_KR_

				
				if( true == g_pMain->GetNexonLoginMessenger() )
				{
					if( 0 == kEvent.m_uiKNMSerialNum )	// nmserial number�� ���� ��쿡�� �ٷ� �����~
					{
						// 2009.05.13.hoons. �ٷλ��� ��ġ�� ����Ǿ� �˾����� �޼����� ����
						//Handler_EGS_DELETE_UNIT_REQ();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_2559 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2559 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					
						// hoons
					}
					else
					{
						Handler_EGS_DELETE_UNIT_REQ();
// 						if( CNMCOClientObject::GetInstance().RemoveMyVirtualUser( nmVirtualKey ) == TRUE )
// 						{
// 							Handler_EGS_DELETE_UNIT_REQ();
// 						}
// 						else
// 						{
// 							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2560 ), this );
// 						}
					}
				}
				else
				{
					Handler_EGS_DELETE_UNIT_REQ();
				}
#endif // #ifndef NEW_MESSENGER
#else OPEN_TEST_1_NO_MESSENGER_CASHSHOP
#ifdef NEW_MESSENGER
				Handler_EGS_DELETE_UNIT_REQ();
#endif
#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

			}
		}
		else
		{
			if( true == m_bRemoveUnitAfterReceiveNMSerialNum )
			{
				m_bRemoveUnitAfterReceiveNMSerialNum = false;
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_2561 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_2561 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			}
		}
		return true;
	}

	return false;
}


#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
bool CX2StateServerSelect::ConnectToChannelServer( bool bIsReversingConnect /* = false */ )
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
bool CX2StateServerSelect::ConnectToChannelServer()
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
{
	ASSERT( NULL != g_pData->GetServerProtocol() );

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
#ifdef SERVER_GROUP_UI_ADVANCED
	if( g_pData->GetServerProtocol()->IsChConnected() == true )
	{
		g_pData->GetServerProtocol()->DisconnectFromChannelServer();
	}

	m_bSentEGS_CONNECT_REQ = false;
	g_pData->GetServerProtocol()->DisconnectFromChannelServer();
#else
	wstring wstrPickedChannelServerIP = L"";
	int iChannelServerPort = g_pMain->GetChannelServerPort();


	if ( true == bIsReversingConnect )
	{
		if ( g_pInstanceData->GetServerGroupID() == SEnum::SGI_GAIA )
		{
			g_pMain->SetPickedChannelServerIPIndex( static_cast< int >( SEnum::SGI_SOLES ) );
			g_pInstanceData->SetServerGroupID ( SGI_SOLES );
		}
		else if ( g_pInstanceData->GetServerGroupID() == SEnum::SGI_SOLES )
		{
			g_pMain->SetPickedChannelServerIPIndex( static_cast< int >( SEnum::SGI_GAIA ) );
			g_pInstanceData->SetServerGroupID ( SGI_GAIA );
		}
		else
		{
			ASSERT( !L"Can't Find Server Group ID !" );
		}
		wstrPickedChannelServerIP = g_pMain->GetPickedChannelServerIP().c_str();
		m_bSentEGS_CONNECT_REQ = false;
		g_pData->GetServerProtocol()->DisconnectFromChannelServer();
	}
	else
	{
		wstrPickedChannelServerIP = g_pMain->GetPickedChannelServerIP().c_str();
	}
#endif SERVER_GROUP_UI_ADVANCED
#endif // REFORM_ENTRY_POINT

#ifdef FIX_REFORM_ENTRY_POINT_2ND		// ������, ���� ���� ���� ���� 2��
#ifdef SERVER_GROUP_UI_ADVANCED
	if( g_pData->GetServerProtocol()->ConnectedToChannelServer( g_pMain->GetPickedChannelServer().m_kServerIP.c_str(), g_pMain->GetPickedChannelServer().m_usMasterPort ) == true )
#else
	if( g_pData->GetServerProtocol()->ConnectedToChannelServer( wstrPickedChannelServerIP.c_str(), iChannelServerPort ) == true )
#endif SERVER_GROUP_UI_ADVANCED
#else	// FIX_REFORM_ENTRY_POINT_2TH	// ������, ���� ���� ���� ���� 2��
	if( g_pData->GetServerProtocol()->ConnectedToChannelServer( g_pMain->GetPickedChannelServerIP().c_str(), g_pMain->GetChannelServerPort() ) == true )
#endif	// FIX_REFORM_ENTRY_POINT_2TH	// ������, ���� ���� ���� ���� 2��
	{
		//{{ 09.08. �¿� : ����-Ŭ�� ���ӽ� ��Ŷ ����.
//#ifdef SERV_KOG_OTP_VERIFY
		return Handler_ECH_VERIFY_ACCOUNT_REQ();
// #else
// 		return Handler_ECH_GET_CHANNEL_LIST_REQ();
// #endif
		//}}
	}
	else
	{

#ifndef FIX_REFORM_ENTRY_POINT
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		if ( true == m_bReConnectChannelServer )
		{
			m_bReConnectChannelServer = false;
		}
		else
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
#endif // FIX_REFORM_ENTRY_POINT
		{			
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
			if ( NULL == m_pDLGReConnectChannelServer )
			{
				m_pDLGReConnectChannelServer = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 305, 375 ),  GET_STRING ( STR_ID_29884 ), 
					SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK, this, SUCM_EXIT, L"DLG_UI_Selection_MessageBox_Ok_Cancle_Button_New.lua" );
#ifdef PLAY_PROMOTION_MOVIE //kimjh
				if ( true == g_pMain->m_bPlayIntroMovie )
					m_pDLGReConnectChannelServer->SetShow ( false );
#endif // PLAY_PROMOTION_MOVIE //kimjh

			}
	#else  // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_735 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
			g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );									// fix!! id ����
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
	#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#else  // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_735 ), this );	
			g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );									// fix!! id ����
			g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh			
		}
	}

	return false;
}

bool CX2StateServerSelect::Handler_ECH_VERIFY_ACCOUNT_REQ()
{
	KECH_VERIFY_ACCOUNT_REQ kPacket;

#ifdef CLIENT_PURPLE_MODULE	
	bool bUsePurpleModule = true;
	
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	if( g_pInstanceData->GetInHouseAccount() )
		bUsePurpleModule = false;
#endif CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

	if( bUsePurpleModule )
	{
		ConvertCharToWCHAR( kPacket.m_wstrID, PurpleGetUserID() );
#ifdef SERV_KOG_OTP_VERIFY
		if( g_pInstanceData->IsIDVerified() )
			kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
		else
			ConvertCharToWCHAR( kPacket.m_wstrPassword, PurpleGetAuthString() );
#else //SERV_KOG_OTP_VERIFY
		ConvertCharToWCHAR( kPacket.m_wstrPassword, PurpleGetAuthString() );
#endif //SERV_KOG_OTP_VERIFY
	}
	else
	{
		kPacket.m_wstrID = g_pInstanceData->GetUserID();
		kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
	}
#else
	kPacket.m_wstrID = g_pInstanceData->GetUserID();
	kPacket.m_wstrPassword = g_pInstanceData->GetUserPassword();
#endif CLIENT_PURPLE_MODULE

#ifdef SERV_KOG_OTP_VERIFY
	kPacket.m_bLogin = !(g_pInstanceData->IsIDVerified());
	kPacket.m_bIDVerified = g_pInstanceData->IsIDVerified();
#else SERV_KOG_OTP_VERIFY
    kPacket.m_bLogin = true;
#endif SERV_KOG_OTP_VERIFY
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
#ifdef MACHINE_ID
	if( g_pMain != NULL )
		kPacket.m_strMachineID = g_pMain->GetMachineId();
#endif
#endif
#ifdef SERV_COUNTRY_PH
#ifdef _SERVICE_
	if (g_pInstanceData != NULL)
	{
		kPacket.m_strUserToken = g_pInstanceData->GetAuthToken();
	}
#endif //_SERVICE_
#endif //SERV_COUNTRY_PH

#ifdef SERV_STEAM
	kPacket.m_iChannelingCode = g_pInstanceData->GetChannelingCode();
#endif //SERV_STEAM

	g_pData->GetServerProtocol()->SendChPacket( ECH_VERIFY_ACCOUNT_REQ, kPacket );
	g_pMain->AddServerPacket( ECH_VERIFY_ACCOUNT_ACK );
	
	return true;
}

bool CX2StateServerSelect::Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_VERIFY_ACCOUNT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( ECH_VERIFY_ACCOUNT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
#ifdef SERV_KOG_OTP_VERIFY
			// ���� Gash ���� �ķδ� �������� �߱޹��� Pass�� ��� �̿��ϰ� �ǹǷ�(���Ӽ��� ���ӵ�), �ƿ� ������ ������.
			g_pInstanceData->SetIDVerified(true);
			g_pInstanceData->SetUserPassword( kEvent.m_wstrPassport );	
#endif SERV_KOG_OTP_VERIFY

	//{{����ȯ �����ð� �޾ƿ���
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->SetServerCurrentTime( kEvent.m_wstrCurrentTime );
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->MassFileMapping();
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

#ifdef SERVER_GROUP_UI_ADVANCED
			Handler_ECH_GET_SERVERGROUP_LIST_REQ();
			Handler_ECH_GET_CHANNEL_LIST_REQ();

			return true;
#else
			return Handler_ECH_GET_CHANNEL_LIST_REQ();
#endif SERVER_GROUP_UI_ADVANCED
		}
		else
		{			
#ifdef SERV_KOG_OTP_VERIFY
			switch( kEvent.m_iOK )
			{
			case NetError::ERR_VERIFY_20:
				{
					g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_CONNECT_FAILED );
					g_pKTDXApp->SendGameMessage( XGM_QUIT_GAME, NULL, NULL, false );
				} break;
			default:
				{

				} break;
			}
#endif SERV_KOG_OTP_VERIFY
		}
	}

	return false;
}



#ifdef SERV_SECOND_SECURITY
bool CX2StateServerSelect::Handler_EGS_SECOND_SECURITY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SECOND_SECURITY_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( kEvent.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT )
	{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_12352 ), this, CX2State::SUCM_EXIT, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12352 ), g_pMain->GetNowState(), CX2State::SUCM_EXIT );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		
	
	}
	else if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{	
#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
		if ( g_pMain->GetSecurityPad()->GetIsAutoAuthoritySecondSecurityBegin() == true  )
		{
			g_pMain->GetSecurityPad()->AutoAuthoritySecondSecurity();
		}
		else
#else // FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
	#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
			if ( m_bEnableUnitSelectBySecondSecurity == true 
		#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
				&& g_pMain->GetSecurityPad()->GetIsAutoAuthoritySecondSecurityBegin() == true 
		#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��						
				)
			{
				g_pMain->GetSecurityPad()->AutoAuthoritySecondSecurity();
			}

			else
	#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
#endif // FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ���� 
		{
			if(kEvent.m_bUseSecondPW == true)
			{
#if defined( FIX_REFORM_ENTRY_POINT_2ND ) && defined( PLAY_PROMOTION_MOVIE )
				if ( g_pMain->m_bPlayIntroMovie == true )
				{
					m_bReservedSecurityPad = true;
				}
				else
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
				{
					g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_GAME_START);
					g_pMain->GetSecurityPad()->SetUseSecondPW(true);
				}
			}
			else
			{
				m_bEnableUnitSelectBySecondSecurity = true;
			}
		}
	}
	return true;
}
#endif SERV_SECOND_SECURITY

#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
bool CX2StateServerSelect::Handler_EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( NULL == g_pMain)
		return false;
	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	switch( kEvent)
	{
	case KFirstSelectUnitReward::FSUR_SYSTEM_ERROR:
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_22430 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_22430 ), NULL);
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		
		
		return true;
	case KFirstSelectUnitReward::FSUR_WEB_EVENT:
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_22429 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_22429 ), NULL);
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

		return true;
	default:
		return true;
	}

	return true;
}
#endif //SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
bool CX2StateServerSelect::Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ );
	g_pMain->AddServerPacket( EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK );

	return true;
}

bool CX2StateServerSelect::Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KDBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	
	if( g_pMain->DeleteServerPacket( EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( false == kEvent.m_bReceiveTheLetter )
			{
				if( NULL == m_pDLGEventGuide )
				{
					m_pDLGEventGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Event_Guide.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEventGuide );
					m_pDLGEventGuide->SetShowEnable(true,true);
				}
				else
				{
					m_pDLGEventGuide->SetShowEnable(true,true);
				}
			}
		}
	}
	return true;
}
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE


/*virtual*/ bool CX2StateServerSelect::LastMsgByESC()
{
#ifdef SERV_SECOND_SECURITY
	// oasis907 : ����� [2011.6.8] �����е� Ȱ���� ���� ����â���� ESCŰ ����
	if(g_pMain->GetSecurityPad() != NULL &&
		g_pMain->GetSecurityPad()->GetShow() == true)
	{
		return true;
	}
#endif SERV_SECOND_SECURITY

#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
	#ifdef FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
		if ( NULL != m_pDLGCheckHack && m_pDLGCheckHack->GetShow() == true )
			return true;
		ProcessIntroMovieEnd();		// ��Ʈ�� �������� ������ �� ó������� �� ��
	#else  // FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
		// ���� �е带 ������ ��� ���Ŀ� ��µǵ��� �Ѵ�.
		if ( true == m_bReservedSecurityPad )
		{
			m_bReservedSecurityPad = false;
			g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_GAME_START);
			g_pMain->GetSecurityPad()->SetUseSecondPW(true);
		}
	#endif // FIX_REFORM_ENTRY_POINT_5TH // ������, ���� ���� ���� ���� 5��
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��

#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	// ���ӿ� ���� ���̶��, ESC �� ������ �ʵ���
	if ( true == m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp )
	{
		return true;
	}

	if ( NULL != m_pDLGMsgBox )
	{
		return true;
	}
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����

	if( true == CX2StateServerSelect::m_sbSelectedServerSet )
	{
#ifdef REFORM_ENTRY_POINT		
		// ä�� ����â���� ESC �Է� �� ĳ���� ����â���� �̵�
#ifdef PLAY_PROMOTION_MOVIE //JHKang
		if( true == IsChannelSelectMode() )
		{
			MoveToCharacterSelectUI();
		}
		else if ( g_pMain->m_bPlayIntroMovie == true )
		{
			g_pMain->m_bPlayIntroMovie = false;
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
			// �������ϴ� ��Ȳ�̸� State �� �ٽ� �ҷ��´�.
			if ( NULL != m_pDLGReConnectChannelServer && false == m_pDLGReConnectChannelServer->GetShow() )
			{
				m_sbSelectedServerSet = false;
				m_bCanNotConenctToBusyServer = false;
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_SERVER_SELECT, NULL, false );		
			}
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
		}
#else //PLAY_PROMOTION_MOVIE
		if( true == IsChannelSelectMode() )
		{		 
			MoveToCharacterSelectUI();
		}
#endif //PLAY_PROMOTION_MOVIE
		else
		{
			QuitGame();
		}
#else
		OpenLastMsgPopUp( GET_STRING( STR_ID_736 ) );
#endif	// REFORM_ENTRY_POINT
	}
	else
	{
		return QuitGame();
	}

	return true;
}

/*virtual*/ void CX2StateServerSelect::LastMsgPopOk()
{
	if ( m_pDLGLastMsgPopUp != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGLastMsgPopUp, NULL, false );
	m_pDLGLastMsgPopUp = NULL;

	g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, SSSUCM_UNIT_SELECT_EXIT, NULL, false );
}

void CX2StateServerSelect::UnitSelectExit()
{
	ClearBeforeUnit( m_pSelectUnit );
	ClearUnitButton();
#ifdef SERVER_GROUP_UI_ADVANCED
	if(m_pDLGServerGroupSelectBack != NULL)
		m_pDLGServerGroupSelectBack->SetShowEnable(true, true);
#endif SERVER_GROUP_UI_ADVANCED
	m_pDLGServerSelectBack->SetShowEnable(true, true);
	m_pDLGUnitSelectFront->SetShowEnable(false, false);

	//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
	m_pDLGChannelSelectBack->SetShowEnable( true, true );
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�

	m_pSelectUnit = NULL;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	SetRightSideFlagColor ( D3DXCOLOR ( 0.f, 0.f, 0.f, 0.f ) );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
#ifdef SERV_UNIT_WAIT_DELETE
	m_iSelectUnitUID = 0;
#endif //SERV_UNIT_WAIT_DELETE	
	m_sbSelectedServerSet = false;

	m_bENX_USER_LOGIN_NOT		= false;
	m_bEGS_VERIFY_ACCOUNT_REQ	= false;
	m_bEGS_VERIFY_ACCOUNT_ACK	= false;
	m_bPortCheckReq				= false;

	m_bSentEGS_CONNECT_REQ 		= false;
	m_bReceivedChannelList 		= false;
	if( NULL != g_pData->GetServerProtocol() )
	{
		g_pData->GetServerProtocol()->SendTRID( ETR_UNIT_DISCONNECT );
	}
	// ���Ӽ������� ������ ���´�
	Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();

	// ä�μ����� �����Ѵ�.
	ConnectToChannelServer();  

#ifdef SERV_SECOND_SECURITY
	if(g_pMain->GetSecurityPad() != NULL &&
		g_pMain->GetSecurityPad()->GetShow() == true)
	{
		g_pMain->GetSecurityPad()->SetShow(false);
	}
#endif SERV_SECOND_SECURITY

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	ClearChannelButton();
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
	g_pKTDXApp->SkipFrame();
}

//{{ kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ �б�
#ifdef	ADD_SERVER_GROUP
//bool CX2StateServerSelect::OpenScriptServerGroupFile()
//{
//	string			strFileName;
//	SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
//	bool			bParsingOK		= false;
//
//	ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );
//
//	ConvertFileAnsiToUTF8( strFileName, strFileName );
//
//	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//
//	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
//	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile_LocalFile( g_pData->GetSavedServerGroupFileName() );
//	if( Info != NULL )
//	{
//		if( true == g_pKTDXApp->LoadLuaTinker_LocalFile( g_pData->GetSavedServerGroupFileName().c_str() ) )
//		{
//			if( true == g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, g_pData->GetSavedServerGroupFileName().c_str() ) )
//			{
//				LUA_GET_VALUE_ENUM( luaManager, L"SERVER_GROUP", 			eServerGroupID,			SERVER_GROUP_ID,		SGI_INVALID	);
//			}
//		}
//	}
//
//	switch ( eServerGroupID )
//	{
//	case SGI_SOLES:
//	case SGI_GAIA:
//		{
//			g_pInstanceData->SetServerGroupID( eServerGroupID );
//			bParsingOK = true;
//		}
//		break;
//
//	default:
//		{
//#ifdef RANDOM_SERVER
//			if( g_pMain->GetDefaultChannelServerIPIndex() == SGI_INVALID )
//			{
//				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( (rand() % 2) ) );
//			}
//			else
//			{
//				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
//			}
//#else
//			g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
//#endif
//		}
//		break;
//
//	}
//
//	return bParsingOK;
//
//}

//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
bool CX2StateServerSelect::Handler_EGS_FINAL_DELETE_UNIT_REQ()
{
	KEGS_FINAL_DELETE_UNIT_REQ kPacket;
	kPacket.m_iUnitUID = m_pSelectUnit->GetUID();

	g_pData->GetServerProtocol()->SendPacket( EGS_FINAL_DELETE_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_FINAL_DELETE_UNIT_ACK  );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_16104 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Positive_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16104 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
	
	return true;
}

bool CX2StateServerSelect::Handler_EGS_FINAL_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FINAL_DELETE_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_FINAL_DELETE_UNIT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_iUnitUID );
			if ( pUnit == NULL )
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_713 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua",D3DXVECTOR2 (0, 0),  L"UI_PopUp_Negative_01.ogg" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_713 ), this );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����

				return true;
			}

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����						int iTopCharacterIndex = (m_NowPage - 1) * UNIT_SELECT_UI_MAX_UNIT_NUM;
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305,375), GET_STRING( STR_ID_714 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
			// ChangeUnitButtonInfo();

			if ( g_pData->GetMyUser()->GetUnitNum() <= 0 )
			{
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_701 ), L"DLG_UI_Selection_MessageBox_No_Button.lua", L"UI_PopUp_Positive_01.ogg"  );
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_CREATE_UNIT, NULL, false );
			}
			else
			{
				Handler_EGS_CHARACTER_LIST_REQ();
			}
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
/*
			ClearBeforeUnit( pUnit );
			ClearUnitButton();
			g_pData->GetMyUser()->DeleteUnitByUID(kEvent.m_iUnitUID);
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16108 ), this );
			CreateUnitButtonNew();
			ChangeUnitButtonInfo();
*/
		}
	}
	return true;
}

bool CX2StateServerSelect::Handler_EGS_RESTORE_UNIT_REQ()
{
	KEGS_RESTORE_UNIT_REQ kPacket;
	kPacket.m_iUnitUID = m_pSelectUnit->GetUID();

	g_pData->GetServerProtocol()->SendPacket( EGS_RESTORE_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESTORE_UNIT_ACK  );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
#ifdef REFORM_ENTRY_POINT
	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305,375), GET_STRING( STR_ID_16105 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#else
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16105 ), this );
#endif REFORM_ENTRY_POINT

	return true;
}

bool CX2StateServerSelect::Handler_EGS_RESTORE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESTORE_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_RESTORE_UNIT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{			
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_iUnitUID );
			if( pUnit == NULL )
			{
#ifdef REFORM_ENTRY_POINT
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305,375), GET_STRING( STR_ID_713 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_713 ), this );
#endif REFORM_ENTRY_POINT
				return true;
			}

#ifdef REFORM_ENTRY_POINT
#ifdef SERV_UNIT_WAIT_DELETE
			m_bMaintainNowUnitButton = true;
#endif SERV_UNIT_WAIT_DELETE
			Handler_EGS_CHARACTER_LIST_REQ();			
#else
			ClearBeforeUnit( pUnit );
			ClearUnitButton();
			pUnit->AccessUnitData().m_bDeleted = false;
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_16111 ), this );
			CreateUnitButtonNew();
			ChangeUnitButtonInfo();
#endif REFORM_ENTRY_POINT
		}
		else if(kEvent.m_iOK == NetError::ERR_RESTORE_UNIT_02)
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByUID( kEvent.m_iUnitUID );
			if ( pUnit == NULL )
			{
#ifdef REFORM_ENTRY_POINT
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305,375), GET_STRING( STR_ID_713 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_713 ), this );
#endif REFORM_ENTRY_POINT
				return true;
			}

			pUnit->AccessUnitData().m_trRestoreAbleDate = kEvent.m_tRestoreAbleDate;

			CTime tRestoreAbleDate(kEvent.m_tRestoreAbleDate);
			std::wstring wstrYear = (CStringW)( tRestoreAbleDate.Format(_T( "%Y" )) );
			std::wstring wstrMonth = (CStringW)( tRestoreAbleDate.Format(_T( "%m" )) );
			std::wstring wstrDay = (CStringW)( tRestoreAbleDate.Format(_T( "%d" )) );
			std::wstring wstrHour = (CStringW)( tRestoreAbleDate.Format(_T( "%H" )) );
			std::wstring wstrMinute = (CStringW)( tRestoreAbleDate.Format(_T( "%M" )) );
			std::wstring wstrMessage = GET_REPLACED_STRING( ( STR_ID_16113, "SSSSS", wstrDay.c_str(), wstrMonth.c_str(), wstrYear.c_str(), wstrHour.c_str(), wstrMinute.c_str()));
#ifdef REFORM_ENTRY_POINT
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305,375), wstrMessage.c_str(), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Negative_01.ogg" );
#else
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMessage.c_str(), this );
#endif REFORM_ENTRY_POINT
		}
	}
	return true;
}
#endif SERV_UNIT_WAIT_DELETE
//}}

#ifdef SERVER_GROUP_UI_ADVANCED
bool CX2StateServerSelect::Handler_ECH_GET_SERVERGROUP_LIST_REQ()
{
	g_pData->GetServerProtocol()->SendChID( ECH_GET_SERVERGROUP_LIST_REQ );
	g_pMain->AddServerPacket( ECH_GET_SERVERGROUP_LIST_ACK, 100.0f );

	return true;
}

bool CX2StateServerSelect::Handler_ECH_GET_SERVERGROUP_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
	KECH_GET_CHANNEL_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( ECH_GET_SERVERGROUP_LIST_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// nothing;
		}
	}
	return true;
}

bool CX2StateServerSelect::Handler_ECH_GET_SERVERGROUP_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KECH_GET_SERVERGROUP_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bReceivedServerGroupList = true;

#ifdef CRASH_PROBE_LOG
	CrashLog( "{E46CACFD-654A-4c77-9FD7-FABCBA0E703C}" );
#endif CRASH_PROBE_LOG

	g_pInstanceData->m_mapServerGroupInfo.clear();
	g_pInstanceData->m_mapServerGroupInfo = kEvent.m_mapServerGroupList;

	// ä�� ����Ʈ UI�� Update�Ѵ�
	UpdateServerGroupListUI();

#ifdef CRASH_PROBE_LOG
	CrashLog( "{A45C0907-23CF-41d1-89BF-0D9F3594DA74}" );
#endif CRASH_PROBE_LOG

	return true;
}

bool CompareServerGroup(const KServerGroupInfo& a, const KServerGroupInfo& b)
{
	return a.m_iOrder < b.m_iOrder;
}

void CX2StateServerSelect::UpdateServerGroupListUI()
{
	const D3DXVECTOR2 posDLG(20, 60);
	const D3DXVECTOR2 posButton(18, 18);

	// ����
	SAFE_DELETE_DIALOG( m_pDLGServerGroupSelectBack );

    m_pDLGServerGroupSelectBack =  new CKTDGUIDialog( this, L"DLG_UI_ServerGroup_Selection.lua" );

	if ( m_pDLGServerGroupSelectBack == NULL )
		return;

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGServerGroupSelectBack );
	m_pDLGServerGroupSelectBack->SetPos( posDLG );

	// ������Ʈ
	int iServerGroupSize = 0;
	vector<KServerGroupInfo> vecSortedServerGroup;
	map<int, KServerGroupInfo>::iterator iterEnable = g_pInstanceData->m_mapServerGroupInfo.begin();
	while(iterEnable != g_pInstanceData->m_mapServerGroupInfo.end())
	{
		//if(iterEnable->second.m_bEnable == true)
		{
			++iServerGroupSize;
			vecSortedServerGroup.push_back(iterEnable->second);
		}

		++iterEnable;
	}
	std::sort( vecSortedServerGroup.begin(), vecSortedServerGroup.end(), CompareServerGroup);

	int iMaxServergroup = min(iServerGroupSize, MAX_SERVERGROUP_SIZE);
	int i = 0;

	// ���
	const float button_height = 61.f-1.f;
	const float ADD_Y = button_height*iMaxServergroup - 13;

	CKTDGUIStatic* pStaticBG = (CKTDGUIStatic*)m_pDLGServerGroupSelectBack->GetControl(L"BG");
	if(pStaticBG)
	{
		CKTDGUIStatic::CPictureData* pPictureMiddle = pStaticBG->GetPicture(1);
		CKTDGUIStatic::CPictureData* pPictureBottom = pStaticBG->GetPicture(2);

		if(pPictureMiddle != NULL && pPictureMiddle->pPoint != NULL)
		{
			pPictureMiddle->pPoint->addSize.y = ADD_Y;
			pPictureMiddle->pPoint->SetAutoPointByTextureSize();
		}
		if(pPictureBottom != NULL && pPictureBottom->pPoint != NULL)
		{
			D3DXVECTOR2 pos = pPictureBottom->GetPos();
			pos.y += ADD_Y;
			pPictureBottom->SetPos( pos );
		}
	}

	// ��ư
	int button_index = 0;
	vector<KServerGroupInfo>::iterator iter = vecSortedServerGroup.begin();
	for(; iter != vecSortedServerGroup.end(); ++iter)
	{
		ASSERT(button_index < MAX_SERVERGROUP_SIZE);

		if(i >= iMaxServergroup)
			break;

		//if(false == iter->second.m_bEnable)
		//	continue;

		boost::wformat fmtButtonName(L"server_button_%1%");
		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGServerGroupSelectBack->GetControl( boost::str(fmtButtonName % button_index).c_str() );
		if(pButton)
		{
			pButton->AddDummyInt(iter->m_iServerGroupUID);
			pButton->SetShow(true);
		}

		boost::wformat fmtButtonCaption(L"server_name_%1%");
		CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGServerGroupSelectBack->GetControl( boost::str(fmtButtonCaption % button_index).c_str() );
		if(pStaticName)
		{
			pStaticName->SetShow(true);

			CKTDGUIControl::UIStringData* pStringName = pStaticName->GetString(0);
			if(pStringName != NULL)
				pStringName->msg = iter->m_wstrServerGroupName;
			CKTDGUIControl::UIStringData* pStringShadow = pStaticName->GetString(1);
			if(pStringShadow != NULL)
				pStringShadow->msg = iter->m_wstrServerGroupName;
			CKTDGUIControl::UIStringData* pStringPostfix = pStaticName->GetString(2);
			if(pStringPostfix != NULL)
			{
				CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStringName->fontIndex );
				if(pFont)
				{
					int width = pFont->GetWidth( pStringName->msg.c_str() );
					pStringPostfix->pos.x += width + 10;
				}
			}
		}

		++button_index;
	}

	UpdateServerSelectButton( g_pMain->GetPickedChannelServerIPIndex() );
}
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERVER_GROUP_UI_ADVANCED
void CX2StateServerSelect::UpdateServerSelectButton( int iID )
{
	if(m_pDLGServerGroupSelectBack != NULL)
	{
		boost::wformat fmtButtonName(L"server_button_%1%");
		for(int i=0; i<MAX_SERVERGROUP_SIZE; ++i)
		{
			CKTDGUIButton* pServerButton = (CKTDGUIButton*)m_pDLGServerGroupSelectBack->GetControl( boost::str(fmtButtonName % i).c_str() );
			if(pServerButton)
			{
				int iServerGroup = pServerButton->GetDummyInt(0);
				if(iServerGroup == iID)
				{
					pServerButton->SetDownStateAtNormal(true);
					//m_iChannelNum = pServerButton->GetDummyInt( 1 );		// ä�� ����
				}
				else
					pServerButton->SetDownStateAtNormal(false);
			}
		}
#ifdef SERVER_SELECT_INT		
		m_iChannelNum = 24;
#endif // SERVER_SELECT_INT		
		g_pMain->SetPickedChannelServerIPIndex( iID );
	}
}
#else SERVER_GROUP_UI_ADVANCED
void CX2StateServerSelect::UpdateServerSelectButton( SERVER_GROUP_ID eID )
{
	CKTDGUIButton *pServerButtonSoles		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_01" ) ); // �ַ���
	CKTDGUIButton *pServerButtonGaia		= static_cast< CKTDGUIButton* >( m_pDLGServerSelectBack->GetControl( L"sever_select_02" ) );	// �ż���
	

	if ( pServerButtonSoles		== NULL ||
		 pServerButtonGaia	== NULL )
		 return;

	// ��� �� false�� �ʱ�ȭ
	pServerButtonSoles->SetDownStateAtNormal( false );		// �ַ���
	pServerButtonGaia->SetDownStateAtNormal( false );		// �ż���

	switch ( eID )
	{
	case SGI_GAIA:
		{
			pServerButtonGaia->SetDownStateAtNormal(true);
			m_iChannelNum	= pServerButtonGaia->GetDummyInt( 1 );		// ä�� ����
			//g_pMain->m_iPickedChannelServerIPIndex = 1; // �ӽ�
		}
		break;

	case SGI_SOLES:
	default:
		{
			pServerButtonSoles->SetDownStateAtNormal(true);
			m_iChannelNum	= pServerButtonSoles->GetDummyInt( 1 );		// ä�� ����
			//g_pMain->m_iPickedChannelServerIPIndex = 0; // �ӽ�
		}
		break;		
	}

	g_pMain->SetPickedChannelServerIPIndex( static_cast< int >( eID ) );
	
}
#endif SERVER_GROUP_UI_ADVANCED
#endif	ADD_SERVER_GROUP
#endif ELSWORD_NEW_BEGINNING
//}}  kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ �б�


bool CX2StateServerSelect::Handler_EGS_KEYBOARD_MAPPING_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_KEYBOARD_MAPPING_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bRecivedKeyMap = true;

	if ( NULL != g_pKTDXApp->GetDIManager() && NULL != g_pKTDXApp->GetDIManager()->Getkeyboard()
		 && true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
	{
		if ( kEvent.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.empty() )
		{
			g_pKTDXApp->GetDIManager()->SetDefaultMap();

			if ( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
			{
				g_pMain->GetKeyPad()->LoadSlotData();
				g_pMain->GetKeyPad()->SaveSlotData( true );

				if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
				{
					g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
				}
			}
		}
#ifdef SERV_KEY_MAPPING_INT
		else if ( GAMEACTION_RETURN == kEvent.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.size() )
#else //SERV_KEY_MAPPING_INT
		else if ( GAMEACTION_END == kEvent.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.size() )
#endif //SERV_KEY_MAPPING_INT
		{
			SettingUserKeyboardMappingInfo( kEvent.m_kKeyboardMappingInfo );
		}
		// ����� Ʋ���� ��Ī�� �ȵ� ����Ʈ Ű�� ���õ�
		else 
		{
			if ( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
				g_pMain->GetKeyPad()->InitKeySlotData();

			SettingUserKeyboardMappingInfo( kEvent.m_kKeyboardMappingInfo );
		}
			
	}

#ifdef SERV_KEY_MAPPING_INT
	if ( NULL != g_pKTDXApp->GetDIManager() && NULL != g_pKTDXApp->GetDIManager()->GetJoystic()
		&& true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
	{
		if ( kEvent.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.empty() )
		{
			g_pKTDXApp->GetDIManager()->SetGamePadDefaultMap();
		}
		else
		{
			SettingUserGamePadMappingInfo( kEvent.m_kKeyboardMappingInfo );
		}
	}
#endif //SERV_KEY_MAPPING_INT
	
	return true;
}

void CX2StateServerSelect::SettingUserKeyboardMappingInfo( const KKeyboardMappingInfo& kKeyboardMappingInfo_ )
{
	std::map< short, short >::const_iterator constIter;

	for ( constIter = kKeyboardMappingInfo_.m_mapKeyboardMappingInfo.begin() ;
		constIter != kKeyboardMappingInfo_.m_mapKeyboardMappingInfo.end(); ++constIter )
	{
		// DB�� ����� Key Setting ���� �°� Ű���� ���� ������
		if ( constIter->second < CX2KeyPad::USE_KEY_NUMBER )
			g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAME_ACTION( constIter->first ), CX2KeyPad::USE_DIK_LIST[constIter->second] );
		else
			g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAME_ACTION( constIter->first ), 0x00 );
	}

	if ( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
	{
		g_pMain->GetKeyPad()->LoadSlotData();

		if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
		{
			g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
		}
	}
}

#ifdef SERV_KEY_MAPPING_INT
void CX2StateServerSelect::SettingUserGamePadMappingInfo( const KKeyboardMappingInfo& kKeyboardMappingInfo_ )
{
	std::map< short, short >::const_iterator constIter;

	for ( constIter = kKeyboardMappingInfo_.m_mapGamePadMappingInfo.begin() ;
		constIter != kKeyboardMappingInfo_.m_mapGamePadMappingInfo.end(); ++constIter )
	{
		g_pKTDXApp->GetDIManager()->GetJoystic()->SetAction( GAME_ACTION( constIter->first ), constIter->second );
	}
}
#endif //SERV_KEY_MAPPING_INT

bool CX2StateServerSelect::Handler_EGS_CHAT_OPTION_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAT_OPTION_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) && NULL != g_pChatBox )
	{
		g_pChatBox->SetOptionData(kEvent.m_mapChatOptionSetInfo);			
	}
	return true;
}
#ifdef MODIFY_ADVERTISEMENT // ���볯¥: 2013-03-28
void CX2StateServerSelect::AddMeshAdvertisement_Back()
{
	if( NULL == m_pDLGUnitSelectBack )
		return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = 
		g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"MeshAdvertisement_Back",  0.f, 0.f, 0.f, 0,0,0, 0,0,0 );
	
	if( NULL != pMeshInst )
	{
		pMeshInst->SetOverUI(true);
		g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst);
		m_pDLGUnitSelectBack->SetHasUnit( pMeshInst );
		m_hMeshAddAdvertisement_Back = pMeshInst->GetHandle();
	}
}
void CX2StateServerSelect::AddMeshAdvertisement_Front()
{
	if( NULL == m_pDLGUnitSelectFront )
		return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = 
		g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"MeshAdvertisement_Front", 0.f, 0.f, 0.f, 0,0,0, 0,0,0 );

	if( NULL != pMeshInst )
	{
		pMeshInst->SetOverUI(true);
		g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst);

		m_pDLGUnitSelectFront->SetHasUnit( pMeshInst );
		m_hMeshAddAdvertisement_Back = pMeshInst->GetHandle();
	}
}
#endif // MODIFY_ADVERTISEMENT

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
void CX2StateServerSelect::_SendSystemInfo()
{
	//������ ����
	KEGS_SYSTEM_INFO kPacket;
	kPacket.m_iAccountUID = g_pData->GetMyUser()->GetUserData().userUID;
	kPacket.m_iSystemKey = g_pMain->GetSystemInfo()->GetMacAddress();
	kPacket.m_wstrCPU = g_pMain->GetSystemInfo()->GetCpuString();
	kPacket.m_wstrMemory = g_pMain->GetSystemInfo()->GetMemString();
	kPacket.m_wstrGPU = g_pMain->GetSystemInfo()->GetDisplayString(DXUTGetD3DObject());
	kPacket.m_wstrOS = g_pMain->GetSystemInfo()->GetOsString();

	//
	g_pData->GetServerProtocol()->SendPacket( EGS_SYSTEM_INFO, kPacket );
}
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
bool CX2StateServerSelect::Handler_EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bIsGameBang = kEvent.m_bCheckClinetDirectPcBang;

#ifdef SERV_PC_BANG_TYPE
	// �������� ���� pc�� ������ Ŭ�� ������ �Ӵϴ�.
	if ( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )
	{
		g_pData->GetPremiumBuffManager()->SetPremiumPC(kEvent.m_bCheckClinetDirectPcBang);
		g_pData->GetPremiumBuffManager()->SetPcBangReward( kEvent.m_kPcBangReward );
	}
#endif SERV_PC_BANG_TYPE
	
	return true;
}
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef SHOW_ONLY_ADVERTISEMENT
/** @function : SetShowServerSelectUI
	@brief : ���� ����â UI�� Show ���¸� �����ϴ� �Լ�
	@param : True = ����, false = �����
*/
void CX2StateServerSelect::SetShowServerSelectUI( bool bVal_ )
{
	m_bHideServerSelectUI = !bVal_;

	if( NULL != m_pDLGUnitSelectBack )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"Background"));
		if( NULL != pStatic )
		{
			pStatic->SetShow( bVal_ );
		}
	}

	if( NULL != m_pDLGChannelSelectBack )
		m_pDLGChannelSelectBack->SetShow( bVal_ );

	if( NULL != m_pDLGServerSelectBack )
		m_pDLGServerSelectBack->SetShow( bVal_ );

}
#endif // SHOW_ONLY_ADVERTISEMENT

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����
	// ���� ĳ���� �˾�â ����� �������� �ִ� not
bool CX2StateServerSelect::Handler_EGS_JUMPING_CHARACTER_GUIDE_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	// ���� ���� ���� �˸�â, ���ٸ� �ٽ� �ҷ����� �ִٸ� SetShow True �� ����
	if ( NULL != m_pDLGInfoCreateJumpingChar )
	{
		m_pDLGInfoCreateJumpingChar->SetShowEnable(true, true);
		m_pDLGInfoCreateJumpingChar->SetFront( true );
	}
	else	
	{
		m_pDLGInfoCreateJumpingChar = new CKTDGUIDialog( this, L"DLG_UI_JUMPING_EVENT_01.lua" );	
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInfoCreateJumpingChar );
		m_pDLGInfoCreateJumpingChar->SetShowEnable(true, true);
	}	

	return true;
}

#endif // ADDED_EVENT_JUMPING_CHARACTER	// ������, �������� �̺�Ʈ ���� ĳ����


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
void CX2StateServerSelect::HideAllUnitViewerUI ()
{
	for ( int i = 0; i < (int)m_UnitViewerUIList.size(); i++ )
	{
		CX2UnitViewerUI* pUnitViewerUI = m_UnitViewerUIList[i];
		pUnitViewerUI->SetShowObject( false );
		pUnitViewerUI->SetShowUnitViewerUI( false );
	}
}

void CX2StateServerSelect::CreateServerSelectUnitViewerUI ( CX2Unit * pUnit_ )
{
	if ( NULL != m_pUnitViewerUI )
	{
		CX2UnitViewerUI::DeleteKTDGObject ( m_pUnitViewerUI );
		m_pUnitViewerUI = NULL;
	}		
	
	if ( NULL != pUnit_ )
	{
		m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
		m_pUnitViewerUI->SetLightPos( -250, 100, -600 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

		if ( NULL != m_pDLGUnitSelectFront )
		{
			D3DXVECTOR3 vUnitPos	= m_pDLGUnitSelectFront->GetDummyPos( 18 );
			D3DXVECTOR3 vEye		= m_pDLGUnitSelectFront->GetDummyPos( 19 );
			D3DXVECTOR3 vLookAt		= m_pDLGUnitSelectFront->GetDummyPos( 20 );
			D3DXVECTOR3 vFov		= m_pDLGUnitSelectFront->GetDummyPos( 21 );
			
			pUnit_->AccessInventory().RemovePCBangEquips();

			m_pUnitViewerUI->SetClassChange( false );
			m_pUnitViewerUI->SetUnit( pUnit_, CX2UnitViewerUI::UVS_SELECTUNIT );

			g_pKTDXApp->GetDGManager()->GetCamera().Point( vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z );
			g_pMain->SetProjectionFov( vFov.x );
		
			m_pUnitViewerUI->GetMatrix().Move( D3DXVECTOR3 ( vUnitPos.x, vUnitPos.y, vUnitPos.z ) );
			m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) * m_pUnitViewerUI->GetScale() );
			m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );
			m_bPlayDownAnim = true;

#ifdef SERV_UNIT_WAIT_DELETE
			CX2Unit::UnitData* pUnitData = &(pUnit_->AccessUnitData());
			/* ���� �κ��� ��� ���
			CKTDGUIStatic* pStatic_UnitDeleteInfo = (CKTDGUIStatic*)(m_pDLGUnitSelectFront->GetControl( L"StaticUnitSelectDeleteInfo" ));
			if( NULL != pStatic_UnitDeleteInfo )
			{
				if( pUnitData->m_bDeleted )
				{
					CTime tDeleAbleDate( pUnitData->m_trDelAbleDate );

					// ���� ���� ���� �˸���
					std::wstring wstrYear = (CStringW)( tDeleAbleDate.Format(_T("%Y")) );
					std::wstring wstrMonth = (CStringW)( tDeleAbleDate.Format(_T("%m")) );
					std::wstring wstrDay = (CStringW)( tDeleAbleDate.Format(_T("%d")) );
					std::wstring wstrHour = (CStringW)( tDeleAbleDate.Format(_T("%H")) );
					std::wstring wstrMinute = (CStringW)( tDeleAbleDate.Format(_T("%M")) );

#if defined(CLIENT_GLOBAL_LINEBREAK)
					pStatic_UnitDeleteInfo->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2Main(GET_REPLACED_STRING( ( STR_ID_16109, "SSSSS", wstrDay.c_str(), wstrMonth.c_str(), wstrYear.c_str(), wstrHour.c_str(), wstrMinute.c_str() ) ), 200, XUF_DODUM_15_BOLD );
#elif defined(CLIENT_GLOBAL_LINEBREAK_FOR_CN)
					pStatic_UnitDeleteInfo->GetString(0)->msg = g_pMain->GetStrByLienBreak(GET_REPLACED_STRING( ( STR_ID_16109, "SSSSS", wstrDay.c_str(), wstrMonth.c_str(), wstrYear.c_str(), wstrHour.c_str(), wstrMinute.c_str() ) ), 180, pStatic_UnitDeleteInfo->GetString(0)->fontIndex );
#else
					pStatic_UnitDeleteInfo->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_16109, "SSSSS", wstrDay.c_str(), wstrMonth.c_str(), wstrYear.c_str(), wstrHour.c_str(), wstrMinute.c_str() ) );
#endif //defined(CLIENT_GLOBAL_LINEBREAK)
					pStatic_UnitDeleteInfo->SetShow( true );
				}
				else
				{
					pStatic_UnitDeleteInfo->SetShow( false );
				}
			}
			*/
			// ĳ���� ���� ���� ��ư
			CKTDGUIButton* pBtn_FinalDelete = (CKTDGUIButton*)(m_pDLGUnitSelectFront)->GetControl( L"final_delete_unit" );
			if( NULL != pBtn_FinalDelete )
			{
				if( pUnitData->m_bDeleted )
				{					
					//CTime cClientTime = CTime::GetCurrentTime();
					//__int64 tint64TimeGap = g_pData->GetServerCurrentTime() - cClientTime.GetTime();
					//if(pUnitData->m_trDelAbleDate <  ( g_pData->GetServerCurrentTime() + tint64TimeGap) ) // 2013.01.30 lygan_������ // ������ �ٸ� �������� ������ �Ҷ��� ��ư�� Ȱ��ȭ �ǰ� ó���ϱ�
					if( pUnitData->m_trDelAbleDate <  g_pData->GetServerCurrentTime64() )
					{
						pBtn_FinalDelete->SetShowEnable( true, true );
					}
					else
					{
						pBtn_FinalDelete->SetShowEnable( true, false );
					}				
				}
				else
				{
					pBtn_FinalDelete->SetShowEnable( false, false );
				}
			}

			// ĳ���� ���� ��ư
			CKTDGUIButton* pBtn_Restore = (CKTDGUIButton*)(m_pDLGUnitSelectFront)->GetControl( L"restore_unit" );
			if( NULL != pBtn_Restore )
			{
				if( pUnitData->m_bDeleted )
				{
					if( pUnitData->m_trRestoreAbleDate < g_pData->GetServerCurrentTime64() )
					{
						pBtn_Restore->SetShowEnable( true, true );
					}
					else
					{
						pBtn_Restore->SetShowEnable( true, false );
					}
				}
				else
				{
					pBtn_Restore->SetShowEnable( false, false );
				}
			}
#endif SERV_UNIT_WAIT_DELETE
		}
	}
}

// ��Ȳ�� �°� �� ������Ѵ�.
void CX2StateServerSelect::SetFocusUnitButtonByIndex ( const int iIndex_ )
{
	for ( UINT i = 0; i < m_vecUnitSlot.size(); ++i )
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];

		if ( pUnitSlot == NULL )
			continue;

		CKTDGUIButton* pCharButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 0 ) );
		if ( i == iIndex_ )
			pCharButton->SetDownStateAtNormal( true );
		else
			pCharButton->SetDownStateAtNormal( false );
	}	
}

void CX2StateServerSelect::SetRightSideFlagColor ( D3DXCOLOR colorFlag )
{
	if ( NULL != m_pDLGUnitSelectFront )
	{
		CKTDGUIStatic* pStaticSideFlag = m_pDLGUnitSelectFront->GetStatic_LUA("SideFlag");
		// CKTDGUIControl::CPictureData* pPictureBattleFieldName = pStaticBattleFieldName->GetPicture( 0 );		
		if ( NULL != pStaticSideFlag )
		{
			CKTDGUIControl::CPictureData* pSideFlagPictureData = pStaticSideFlag->GetPicture ( 0 );
			if ( NULL != pSideFlagPictureData )
			{
				pSideFlagPictureData->SetColor ( colorFlag );
			}
		}	
	}
}

void CX2StateServerSelect::SetShowServerStateButtonSet ( bool bIsShowUnitListState, bool bIsShowChannelListState )
{
	if( NULL != m_pDLGUnitSelectFront )
	{
		CKTDGUIButton* pEndGameButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"new_finish_game") );

		CKTDGUIStatic* pSlotNumberStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectFront->GetControl(L"number"));
		if ( false == bIsShowUnitListState && false == bIsShowChannelListState )
		{						
			if( NULL != pEndGameButton )
			{
				pEndGameButton->SetShowEnable(false,false);
			}

			if( NULL != pSlotNumberStatic )
			{
				pSlotNumberStatic->SetShowEnable( false, false );
			}
			SetShowSelectUnitInfo ( NULL );
		}
		else
		{
			if( NULL != pEndGameButton )
			{
				pEndGameButton->SetShowEnable(true, true);
			}

			if( NULL != pSlotNumberStatic )
			{
				pSlotNumberStatic->SetShowEnable( true, true );
			}
		}
		
		// Ÿ��Ʋ, ĳ���� ���� ��ư, ä�� ����, ���� �е�, ���� ����, ĳ���� ����, �¿� ȭ��ǥ ( ���� ���� ), "���� ���� ���� ������ / �ִ� ���� ���� ������" �� ���� �׷�				
		CKTDGUIStatic* pSelectUnitTitleStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"charac") );
		if ( NULL != pSelectUnitTitleStatic )
		{
			pSelectUnitTitleStatic->SetShow ( bIsShowUnitListState );
		}

		for ( UINT i = 0; i < m_vecUnitSlot.size(); ++i )
		{
			CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];

			if ( pUnitSlot == NULL )
				break;
			pUnitSlot->SetShowEnable( bIsShowUnitListState, bIsShowUnitListState );
		}

		CKTDGUIButton* pCreateUnitButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"createUnit"));
		if( NULL != pCreateUnitButton )
		{
			pCreateUnitButton->SetShowEnable(bIsShowUnitListState,bIsShowUnitListState);
		}

		CKTDGUIStatic* pSecurityPadStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectFront->GetControl(L"new_Securitypad"));
		CKTDGUIStatic* pSecurityPadCreateStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectFront->GetControl(L"new_Securitypad_Create"));

		if( NULL != pSecurityPadStatic && NULL != pSecurityPadCreateStatic )
		{
			if ( NULL != g_pMain && NULL != g_pMain->GetSecurityPad() )
			{
				if ( true == g_pMain->GetSecurityPad()->GetUseSecondPW() )
				{
					pSecurityPadStatic->SetShowEnable(bIsShowUnitListState,bIsShowUnitListState);
					pSecurityPadCreateStatic->SetShowEnable( false,false );
				}
				else
				{
					pSecurityPadStatic->SetShowEnable(false , false );
					pSecurityPadCreateStatic->SetShowEnable( bIsShowUnitListState,bIsShowUnitListState);
				}
			}
#ifdef SERV_DISABLE_SECOND_SECURITY
			pSecurityPadCreateStatic->SetShowEnable( false,false );
#endif //SERV_DISABLE_SECOND_SECURITY
		}
		CKTDGUIStatic* pFadeNotice1 = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"FadeNotice1"));
		if ( NULL != pFadeNotice1 )
		{
			pFadeNotice1->SetShow ( bIsShowUnitListState );
		}
		

		

		CKTDGUIStatic* pGoToChannelListStateStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectFront->GetControl(L"new_channel_select_bt"));
		if( NULL != pGoToChannelListStateStatic )
		{
			pGoToChannelListStateStatic->SetShowEnable(bIsShowUnitListState,bIsShowUnitListState);
		}

		CKTDGUIStatic* pDeleteUnitStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectFront->GetControl(L"new_ChaDelete"));
		if( NULL != pDeleteUnitStatic )
		{
			pDeleteUnitStatic->SetShowEnable(bIsShowUnitListState,bIsShowUnitListState);
		}

		CKTDGUIButton* pUnitSlotListLeftArrowButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"ButtonUnitListLeftArrow"));
		if( NULL != pUnitSlotListLeftArrowButton )
		{
			pUnitSlotListLeftArrowButton->SetShowEnable(bIsShowUnitListState,bIsShowUnitListState);
		}

		CKTDGUIButton* pUnitSlotListRightArrowButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"ButtonUnitListRightArrow"));
		if( NULL != pUnitSlotListRightArrowButton )
		{
			pUnitSlotListRightArrowButton->SetShowEnable(bIsShowUnitListState,bIsShowUnitListState);
		}
		
		// Ÿ��Ʋ, ĳ���� ��������, ���� ����, Ʃ�丮��, ��������, �¿� ȭ��ǥ ( ä�� ���� ) �� ���� �׷�
		CKTDGUIStatic* pChannelListStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"ChannelListStatic") );
		if ( NULL != pChannelListStatic )
		{
			pChannelListStatic->SetShow ( bIsShowChannelListState );
		}



		CKTDGUIButton* pTutorialButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"new_Tutorial"));
		if( NULL != pTutorialButton )
		{
			pTutorialButton->SetShowEnable(bIsShowChannelListState,bIsShowChannelListState);
		}

		CKTDGUIButton* pBackToUnitSelectButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"new_BackToUnitSelect"));
		if( NULL != pBackToUnitSelectButton )
		{
			pBackToUnitSelectButton->SetShowEnable(bIsShowChannelListState,bIsShowChannelListState);
		}


		CKTDGUIButton* pStartGameButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"new_channel_select_start"));
		if( NULL != pStartGameButton )
		{
			pStartGameButton->SetShowEnable(bIsShowChannelListState,bIsShowChannelListState);
		}

		CKTDGUIButton* pChannelSlotListLeftArrowButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"ButtonChannelListLeftArrow"));
		if( NULL != pChannelSlotListLeftArrowButton )
		{
			pChannelSlotListLeftArrowButton->SetShowEnable(bIsShowChannelListState,bIsShowChannelListState);
		}

		CKTDGUIButton* pChannelSlotListRightArrowButton = static_cast<CKTDGUIButton*>(m_pDLGUnitSelectFront->GetControl(L"ButtonChannelListRightArrow"));
		if( NULL != pChannelSlotListRightArrowButton )
		{
			pChannelSlotListRightArrowButton->SetShowEnable(bIsShowChannelListState,bIsShowChannelListState);
		}

		CKTDGUIStatic* pFadeNotice2 = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"FadeNotice2"));
		if ( NULL != pFadeNotice2 )
		{
			pFadeNotice2->SetShow ( bIsShowChannelListState );
		}
	}
}

/* virtual */ bool CX2StateServerSelect::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{
			if ( CX2UnitViewerUI::GetUnitClicked() == true && m_pUnitViewerUI != NULL )
			{
				D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
				int degree = (int)D3DXToDegree( rot.y );
				m_fRemainRotY = D3DXToRadian( degree % 360 );
			}
			CX2UnitViewerUI::SetUnitClicked( false );
		}
		break;
	}

	return CX2State::MsgProc( hWnd, uMsg, wParam, lParam );
}

void CX2StateServerSelect::SetShowSelectUnitInfo( CX2Unit * pUnit )
{
	if ( NULL != m_pDLGUnitSelectBack )
	{
		if ( NULL != static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"SelectUnitInfo") ) )
		{
			CKTDGUIStatic* pGameStartStatic = static_cast<CKTDGUIStatic*>(m_pDLGUnitSelectBack->GetControl(L"SelectUnitInfo") );

			if ( NULL != pUnit )
			{
				wstringstream wstringstreamLevelInfo;
				wstringstreamLevelInfo << L"Lv. " << pUnit->GetUnitData().m_Level;		
				pGameStartStatic->SetShow( true );
				pGameStartStatic->SetString( 0, wstringstreamLevelInfo.str().c_str() );
				wstring wstrNickname = pUnit->GetNickName();
				if ( -1 != wstrNickname.find ( L"__DELETED__") )
				{
					wstrNickname = GET_STRING(STR_ID_29404);//�г��� ����
				}
				pGameStartStatic->SetString( 1, wstrNickname.c_str() );
			}
			else
			{
				pGameStartStatic->SetShow( false );
			}
		}
	}
}

void CX2StateServerSelect::ClearChannelButton()
{
	for(UINT i=0; i<m_vecChannelSlot.size(); ++i)
	{
		CKTDGUIDialogType pChannelSlot =  m_vecChannelSlot[i];
		SAFE_DELETE_DIALOG(pChannelSlot);
	}
	m_vecChannelSlot.resize(0);
	m_pSelectChannelButton = NULL;
}

void CX2StateServerSelect::ChangeSelectUnitStandMeshInstance ( CX2Unit::UNIT_TYPE eUnitType_ )
{

	if ( NULL != g_pData->GetUIMajorXMeshPlayer() && NULL != g_pData->GetUIEffectSet() )
	{
		wstring wstrName = L"";
		std::vector<wstring> vecWstrName;

		if ( NULL != g_pData->GetUIMajorXMeshPlayer() )
		{
			BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshInstanceHandle hStandMeshInst, m_vecStandMeshInst )
			{	
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( hStandMeshInst );
			}
			m_vecStandMeshInst.clear();
		}
		g_pData->GetUIEffectSet()->StopEffectSet( m_hStandEffect );

		switch ( eUnitType_ )	
		{
		case CX2Unit::UT_ELSWORD : 
			vecWstrName.push_back(L"Dummy_Village_Board1");
			wstrName = L"EffectSet_Elsword_Stand_Aura";
			break;
		case CX2Unit::UT_ARME : 
			vecWstrName.push_back(L"Dummy_Village_Board2");
			wstrName = L"EffectSet_Aisha_Stand_Aura";
			break;
		case CX2Unit::UT_LIRE : 
			vecWstrName.push_back(L"Dummy_Village_Board3");
			vecWstrName.push_back(L"Dummy_Village_Board10");
			wstrName = L"EffectSet_Rena_Stand_Aura";
			break;
		case CX2Unit::UT_RAVEN : 
			vecWstrName.push_back(L"Dummy_Village_Board4");
			wstrName = L"EffectSet_Revan_Stand_Aura";
			break;
		case CX2Unit::UT_EVE : 
			vecWstrName.push_back(L"Dummy_Village_Board5");
			wstrName = L"EffectSet_Eve_Stand_Aura";
			break;
#ifdef NEW_CHARACTER_CHUNG
		case CX2Unit::UT_CHUNG : 
			vecWstrName.push_back(L"Dummy_Village_Board6");
			wstrName = L"EffectSet_Chung_Stand_Aura";
			break;
#endif // NEW_CHARACTER_CHUNG

#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA : 
			vecWstrName.push_back(L"Dummy_Village_Board7");
			wstrName = L"EffectSet_Ara_Stand_Aura";
			break;
#endif // ARA_CHARACTER_BASE

#ifdef NEW_CHARACTER_EL
		case CX2Unit::UT_ELESIS : 
			vecWstrName.push_back(L"Dummy_Village_Board8");
			wstrName = L"EffectSet_Elsword_Stand_Aura";
			break;
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER
		case CX2Unit::UT_ADD : 
			vecWstrName.push_back(L"Dummy_Village_Board9");
			wstrName = L"EffectSet_Add_Stand_Aura";
			break;
#endif // SERV_9TH_NEW_CHARACTER
		default : 
			ASSERT ( !L"Wrong Unit Type");
			return ;
		}
		BOOST_FOREACH( wstring wstrName, vecWstrName )
		{	
			CKTDGXMeshPlayer::CXMeshInstance* pMeshStandInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( m_pUnitViewerUI, 
				wstrName.c_str(), m_pUnitViewerUI->GetMatrix().GetPos(), 
				D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, 0 ) );

			if( NULL != pMeshStandInst )
			{
				D3DXVECTOR3 vRot = pMeshStandInst->GetRotateLocalDegree();
				//vRot.y -= 7.45f;
				//vRot.z -= 2.75f;
				//pMeshStandInst->SetRotateLocalDegree( vRot );
				m_vecStandMeshInst.push_back(pMeshStandInst->GetHandle());
			}
		}
		if( m_vecStandMeshInst.empty() == false )
			m_hStandEffect = g_pData->GetUIEffectSet()->PlayEffectSetByMeshPlayer(wstrName.c_str(), m_vecStandMeshInst[0], CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR );
	}
}
/** @function : ChangeToCharacterSelectUI
	@brief : ä�� ����â���� 'ĳ���� ����â'���� �̵�
*/
void CX2StateServerSelect::MoveToCharacterSelectUI()
{
	SetShowServerStateButtonSet ( true, false );
	ClearUnitButton( false );
	ChangeUnitButtonInfo();
	ClearChannelButton ();
	if( NULL != m_pUnitViewerUI )
		m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT ); 
	m_bPlayDownAnim = false;
}
bool CX2StateServerSelect::Handler_EGS_CHARACTER_LIST_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CHARACTER_LIST_REQ );
	g_pMain->AddServerPacket( EGS_CHARACTER_LIST_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	m_pDLGMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_720 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_No_Button.lua", D3DXVECTOR2 (0, 0), L"UI_PopUp_Positive_01.ogg" );
#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, ĳ���� ����Ʈ �������� ������ ����
	m_pDLGWaitingCharacterList = m_pDLGMsgBox;
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, ĳ���� ����Ʈ �������� ������ ����
	return true;
}


bool CX2StateServerSelect::Handler_EGS_CHARACTER_LIST_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHARACTER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_CHARACTER_LIST_ACK ) )
	{
	
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� )
			if ( true == m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection &&
				true == m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection &&
				NULL != m_pSelectUnit )
			{
				if ( NULL != g_pMain->GetSecurityPad() )
				{
					if ( true == g_pMain->GetSecurityPad()->GetUseSecondPW () && 
						( m_bEnableUnitSelectBySecondSecurity == true || 
							g_pMain->GetSecurityPad()->GetIsAutoAuthoritySecondSecurityBegin() == true ) )
					{
						g_pMain->GetSecurityPad()->Handler_EGS_GET_SECOND_SECURITY_INFO_REQ();
					}
				
					else
					{
						Handler_EGS_SELECT_UNIT_REQ( m_pSelectUnit->GetUID(), m_pSelectUnit->GetUnitData().m_Level );
					}				
				}				
			}
			else
#endif // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, ���� ���� ����, 7�� ���� ���� ( ���� ť�� �� ���� ���� ���� ���� �� �ֵ��� ���� ) 
			{
				ClearBeforeUnit( m_pSelectUnit );
				ClearUnitButton();

				SetShowSelectUnitInfo( NULL );
				if ( NULL != g_pData->GetUIMajorXMeshPlayer() )
				{
					BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshInstanceHandle hStandMeshInst, m_vecStandMeshInst )
					{	
						g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( hStandMeshInst );
					}
					m_vecStandMeshInst.clear();
				}
				if( NULL != g_pData->GetUIEffectSet() )
				{
					g_pData->GetUIEffectSet()->StopEffectSet(m_hStandEffect);
				}

				g_pData->GetMyUser()->DeleteAllUnit();
				g_pInstanceData->ClearUserUnitDataInServer ();

				typedef std::map< int, int > ServerGroupUnitMaxMany;
				map < SEnum::SERVER_GROUP_ID, int > mapServerMaxUnitNumber;
				int iMaxUnitNumberAllServerGroup = 0;
				BOOST_TEST_FOREACH( const ServerGroupUnitMaxMany::value_type&, UnitSlotData, kEvent.m_mapServerGroupUnitSlot )
				{
					switch ( UnitSlotData.first )
					{
					case SEnum::SGI_SOLES :
						mapServerMaxUnitNumber.insert( make_pair( SEnum::SGI_SOLES , UnitSlotData.second ) );
						break;
					case SEnum::SGI_GAIA :
						mapServerMaxUnitNumber.insert( make_pair( SEnum::SGI_GAIA , UnitSlotData.second ) );
						break;
					}
					iMaxUnitNumberAllServerGroup += UnitSlotData.second;
				}


				typedef std::map< int, std::vector<KUnitInfo>  > ServerGroupUnitNowMany;
				map < SEnum::SERVER_GROUP_ID, int > mapServerNowUnitNumber;
				int iNowUnitNumberAllServerGroup = 0;
				BOOST_TEST_FOREACH( const ServerGroupUnitNowMany::value_type&, UnitInfo, kEvent.m_mapServerGroupUnitInfo )
				{
					int iGroupInfoSize = static_cast<int> ( UnitInfo.second.size() );
					switch ( UnitInfo.first )
					{
					case SEnum::SGI_SOLES :					
						mapServerNowUnitNumber.insert( make_pair( SEnum::SGI_SOLES , iGroupInfoSize ) );
						break;
					case SEnum::SGI_GAIA :
						mapServerNowUnitNumber.insert( make_pair( SEnum::SGI_GAIA , iGroupInfoSize ) );
						break;
					}
					iNowUnitNumberAllServerGroup += iGroupInfoSize;
				}


				typedef std::map< int, vector<KUnitInfo> >				ServerGroupUnitInfoBeforeSort;
				typedef std::multimap< UnitInfoAddData, KUnitInfo, greater<UnitInfoAddData> >		ServerGroupUnitInfoAfterSort;

				ServerGroupUnitInfoAfterSort mapServerGroupUnitInfo;

				BOOST_TEST_FOREACH( const ServerGroupUnitInfoBeforeSort::value_type&, UnitinfoData, kEvent.m_mapServerGroupUnitInfo )
				{
					for( int i =0; i < (int)UnitinfoData.second.size(); i++ )
					{	 
						CTime ctLastLoginTime;
#ifdef SERV_UNIT_WAIT_DELETE
						KncUtil::ConvertStringToCTime( UnitinfoData.second[i].m_wstrLastDate, ctLastLoginTime );
#else
						KncUtil::ConvertStringToCTime( UnitinfoData.second[i].m_wstrLastLoginTime, ctLastLoginTime );
#endif SERV_UNIT_WAIT_DELETE
					
						double dwLastLoginTime = KncUtil::TimeToInt( ctLastLoginTime );
						SEnum::SERVER_GROUP_ID eServerGroupID = static_cast<SEnum::SERVER_GROUP_ID> ( UnitinfoData.first );
					
#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, ���� ���� ����, 8�� ���� ���� 
						// ���� ���� �� �����ϴ� Key ���� ������ �α��� �ð��� �� ������ ���ŵʿ� ���� ����� ���� ����
						UnitInfoAddData sUnitInfoAddData ( dwLastLoginTime, eServerGroupID, ( GetUidCreatedUnit() == UnitinfoData.second[i].m_nUnitUID ) );

#else // FIX_REFORM_ENTRY_POINT_8TH		// kimjh, ���� ���� ����, 8�� ���� ���� 
						UnitInfoAddData sUnitInfoAddData ( dwLastLoginTime, eServerGroupID );
#endif // FIX_REFORM_ENTRY_POINT_8TH	// kimjh, ���� ���� ����, 8�� ���� ���� 
						mapServerGroupUnitInfo.insert ( std::make_pair( sUnitInfoAddData, UnitinfoData.second[i] ) ); 						
					}
				}

				BOOST_TEST_FOREACH( const ServerGroupUnitInfoAfterSort::value_type &, ServerGroupUnitInfo, mapServerGroupUnitInfo )
				{
					KUnitInfo kUnitInfo = ServerGroupUnitInfo.second;
					CX2Unit* pUnit = new CX2Unit( kUnitInfo );
					pUnit->SetServerGroupID ( ServerGroupUnitInfo.first.m_eServerGroupID );
					pUnit->SetOwnerUserUID( g_pData->GetMyUser()->GetUID() );
					wstring nickNameToChange = L"__DELETED__";
					if ( nickNameToChange == pUnit->GetUnitData().m_NickName )
					{
						wstringstream tempNickName;
						tempNickName << nickNameToChange.c_str(); // << i;
						pUnit->AccessUnitData().m_NickName = GET_STRING(STR_ID_29404); //�г��� ����
					}
					g_pData->GetMyUser()->AddUnit( pUnit );
				}


				m_MaxUnitNum	= iMaxUnitNumberAllServerGroup;
#ifdef SERV_UNIT_WAIT_DELETE
				if( m_bMaintainNowUnitButton == false )
				{
					m_NowPage = 1;
				}
#else
				m_NowPage		= 1;
#endif SERV_UNIT_WAIT_DELETE
				const int SLOT_COUNT_FOR_CREATE_UNIT = 1;

				if( iNowUnitNumberAllServerGroup < m_MaxUnitNum )
				{
					m_MaxPage		= (iNowUnitNumberAllServerGroup + SLOT_COUNT_FOR_CREATE_UNIT+UNIT_SELECT_UI_MAX_UNIT_NUM-1) / UNIT_SELECT_UI_MAX_UNIT_NUM;
				}
				else
				{
					m_MaxPage		= (iNowUnitNumberAllServerGroup + UNIT_SELECT_UI_MAX_UNIT_NUM - 1 ) / UNIT_SELECT_UI_MAX_UNIT_NUM;
				}

				typedef std::map< SEnum::SERVER_GROUP_ID, int > ServerGroupUnitMany;
				
			

				BOOST_TEST_FOREACH( const ServerGroupUnitMany::value_type &, UnitManyMaxData, mapServerMaxUnitNumber )
				{
					ServerGroupUnitMany::const_iterator cit = mapServerNowUnitNumber.find ( UnitManyMaxData.first );
					if ( cit != mapServerMaxUnitNumber.end() )
					{
						g_pInstanceData->AddUserUnitDataInServer ( UnitManyMaxData.first, cit->second, UnitManyMaxData.second );
					}
					else
					{
						ASSERT ( !L"Can't Find Server Group ID !");
					}
				}
			
				CreateUnitButtonNew();
				ClearUnitButton();
				ChangeUnitButtonInfo();
				int iTopCharacterIndex = (m_NowPage - 1) * UNIT_SELECT_UI_MAX_UNIT_NUM;
#ifdef SERV_UNIT_WAIT_DELETE
				int iSelectedCharacterIndex = 0;
				CX2Unit* pUnit = NULL;
				
				if( m_bMaintainNowUnitButton == false )
				{
					pUnit = g_pData->GetMyUser()->GetUnitByIndex( iTopCharacterIndex );
				}
				else
				{
					iSelectedCharacterIndex = g_pData->GetMyUser()->GetIndexByUID( m_iSelectUnitUID );

					if( iSelectedCharacterIndex < 0 )
					{
						iSelectedCharacterIndex = iTopCharacterIndex;
					}

					pUnit = g_pData->GetMyUser()->GetUnitByIndex( iSelectedCharacterIndex );
				}
#else
				CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( iTopCharacterIndex );
#endif SERV_UNIT_WAIT_DELETE
				if ( NULL != pUnit )
				{
					CreateServerSelectUnitViewerUI ( pUnit );
#ifdef SERV_UNIT_WAIT_DELETE
					if( m_bMaintainNowUnitButton == false )
					{
						SetFocusUnitButtonByIndex( 0 );
					}
					else
					{
						SetFocusUnitButtonByIndex( iSelectedCharacterIndex );
					}
#else
					SetFocusUnitButtonByIndex ( 0 );
#endif SERV_UNIT_WAIT_DELETE
					m_pSelectUnit = pUnit;
					SetRightSideFlagColor ( GetUnitKeyColorByUnitType ( m_pSelectUnit->GetType() ) );
					SetShowSelectUnitInfo ( pUnit );
					ChangeSelectUnitStandMeshInstance ( pUnit->GetType () );
					SetBackgroundImageBySelectUnitType ( pUnit->GetType() );
					SAFE_DELETE_DIALOG ( m_pDLGShowBeforeConnect );
				}
				SetShowServerStateButtonSet ( true, false );			
				m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK = true;

		#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
				if( NULL != g_pMain && NULL != g_pMain->GetSecurityPad() 
					&& false == g_pMain->GetSecurityPad()->GetUseSecondPW() )
				{
					Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
				}
		#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
			}
#ifdef SERV_UNIT_WAIT_DELETE
			m_bMaintainNowUnitButton = false;
			m_iSelectUnitUID = 0;
#endif SERV_UNIT_WAIT_DELETE
#ifdef FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
			g_pInstanceData->ResetConnectedChannelID();
#endif // FIX_REFORM_ENTRY_POINT_10TH // kimjh, ĳ���� ����Ʈ �������� ������ ����
		}
		else
		{
			g_pData->DeleteServerProtocol();
			g_pData->DeleteMyUser();
		}
	}
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	return true;
}

void CX2StateServerSelect::SetBackgroundImageBySelectUnitType ( CX2Unit::UNIT_TYPE eUnitType_ )
{
	if ( NULL != m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" ) &&
		 NULL != m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" )->GetPicture(0) )
	{
		m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" )->GetPicture(0)->SetTex ( L"DLG_Unit_Background.tga", L"Unit_Background" );
	}
	m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" )->SetEnableLeftTop( D3DXVECTOR2 ( 0.f, 0.f ) );
	wstring wstrFileName = L"";
	switch ( eUnitType_ )
	{
		case CX2Unit::UT_NONE :
			return ;
		case CX2Unit::UT_ELSWORD :
			wstrFileName = L"DLG_Unit_Background_elsword.tga";
			break;
		case CX2Unit::UT_LIRE :
			wstrFileName = L"DLG_Unit_Background_rena.tga";
			break;
		case CX2Unit::UT_ARME :
			wstrFileName = L"DLG_Unit_Background_aisha.tga";
			break;
		case CX2Unit::UT_RAVEN :
			wstrFileName = L"DLG_Unit_Background_raven.tga";
			break;
		case CX2Unit::UT_EVE :
			wstrFileName = L"DLG_Unit_Background_eve.tga";
			break;
#ifdef	NEW_CHARACTER_CHUNG
		case CX2Unit::UT_CHUNG :
			wstrFileName = L"DLG_Unit_Background_chung.tga";
			break;
#endif // NEW_CHARACTER_CHUNG

#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA :
			wstrFileName = L"DLG_Unit_Background_ara.tga";
			break;
#endif // ARA_CHARACTER_BASE

#ifdef NEW_CHARACTER_EL
		case CX2Unit::UT_ELESIS :
			wstrFileName = L"DLG_Unit_Background_elesis.tga";
			break;
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER
		case CX2Unit::UT_ADD :
			wstrFileName = L"DLG_Unit_Background_add.tga";
			break;
#endif // SERV_9TH_NEW_CHARACTER 
	}
	
	if ( NULL != m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" ) && 
		 NULL != m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" )->GetPicture(0) )
	{
		m_pDLGUnitSelectFront->GetStatic_LUA( "Unit_Select_background" )->GetPicture(0)->SetTex ( wstrFileName.c_str() );
	}
	
}


void CX2StateServerSelect::CreateUnitButtonNew ()
{
	
	CKTDGUIDialogType pUnitSlot = NULL;
	CKTDGUIButton* pButton = NULL;
	
	for(UINT i=0; i<m_vecUnitSlot.size(); ++i)
	{
		CKTDGUIDialogType pUnitSlot =  m_vecUnitSlot[i];
		SAFE_DELETE_DIALOG(pUnitSlot);
	}
	m_vecUnitSlot.resize(0);
	
	
	for( int j = 0; j < UNIT_SELECT_UI_MAX_UNIT_NUM; j++ )
	{
		pUnitSlot = new CKTDGUIDialog( this, L"DLG_UI_Character_Selection_Slot_New.lua" );

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pUnitSlot );
		pUnitSlot->SetShowEnable(true, true);
		m_vecUnitSlot.push_back(pUnitSlot);		
	}
}
void CX2StateServerSelect::ChangeUnitButtonInfo ()
{
	if ( NULL != m_pDLGUnitSelectFront && true != m_vecUnitSlot.empty() )
	{
		CKTDGUIStatic* pStaticUnitPage = static_cast<CKTDGUIStatic*> ( m_pDLGUnitSelectFront->GetControl( L"number" ) );
		if ( NULL != pStaticUnitPage )
		{
			WCHAR buff[256] = {0,};
			StringCchPrintf( buff, 256, L"%d / %d", m_NowPage, m_MaxPage );	
			pStaticUnitPage->GetString(0)->msg = buff;
		}


		m_bIsAbledStateCreateUnitButton = true;
#ifdef REFORM_ENTRY_POINT_FOR_ONE_SERVER_GROUP
		if ( g_pInstanceData->GetMaxUserUnitManyInServer( SEnum::SGI_SOLES ) <= g_pInstanceData->GetNowUserUnitManyInServer( SEnum::SGI_SOLES ) )
#else
		if ( g_pInstanceData->GetMaxUserUnitManyInServer( SEnum::SGI_GAIA ) <= g_pInstanceData->GetNowUserUnitManyInServer( SEnum::SGI_GAIA ) 
			&& g_pInstanceData->GetMaxUserUnitManyInServer( SEnum::SGI_SOLES ) <= g_pInstanceData->GetNowUserUnitManyInServer( SEnum::SGI_SOLES ) )
#endif REFORM_ENTRY_POINT_FOR_ONE_SERVER_GROUP
		{
			m_bIsAbledStateCreateUnitButton = false;
		}
		else if ( m_iCreateUnitTodayCount >= TODAY_CREATE_UNIT_MAX_NUMBER )
		{
			m_bIsAbledStateCreateUnitButton = false;
		}






		bool bIsNoCreateUnitButtonDrow = false;
		int i = (m_NowPage - 1)*UNIT_SELECT_UI_MAX_UNIT_NUM;
		int j = 0;
		for( ; j < UNIT_SELECT_UI_MAX_UNIT_NUM; i++, j++ )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( i );
			
			CKTDGUIDialogType pUnitSlot = m_vecUnitSlot[j];

			D3DXVECTOR3 dummyPos = m_pDLGUnitSelectFront->GetDummyPos( j );
			D3DXVECTOR2 buttonPos = D3DXVECTOR2( dummyPos.x, dummyPos.y );
			pUnitSlot->SetPos(buttonPos);

			CKTDGUIButton* pSelectUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 0 ) );
			CKTDGUIButton* pCreateUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 1 ) );
			CKTDGUIButton* pNoCreateUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 2 ) );
			CKTDGUIButton* pEmptyUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 3 ) );
			CKTDGUIButton* pFullUnitButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 4 ) );

			if ( NULL != pSelectUnitButton && NULL != pCreateUnitButton && NULL != pNoCreateUnitButton && NULL != pEmptyUnitButton && NULL != pFullUnitButton )
			{							
				pSelectUnitButton->SetDownStateAtNormal( false );
			
				if( NULL == pUnit )
				{			
					if ( false == bIsNoCreateUnitButtonDrow )
					{
						if ( g_pInstanceData->GetMaxUserUnitManyInServer( SEnum::SGI_GAIA ) <= g_pInstanceData->GetNowUserUnitManyInServer( SEnum::SGI_GAIA ) 
							&& g_pInstanceData->GetMaxUserUnitManyInServer( SEnum::SGI_SOLES ) <= g_pInstanceData->GetNowUserUnitManyInServer( SEnum::SGI_SOLES ) )
						{
							pFullUnitButton->SetShowEnable ( true, true );
						}
						else if ( m_iCreateUnitTodayCount >= TODAY_CREATE_UNIT_MAX_NUMBER )
						{
							pNoCreateUnitButton->SetShowEnable( true, true );
						}
						else
						{
							pCreateUnitButton->SetShowEnable ( true, true );
						}
						bIsNoCreateUnitButtonDrow = true;
					}
					else
					{
						pEmptyUnitButton->SetShowEnable( true, true );
					}

					continue;
				}
				else
				{
					pSelectUnitButton->SetShowEnable( true, true );
				}
			}
			
#ifndef REFORM_ENTRY_POINT
			CKTDGUIStatic* pStaticEmblem = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"UnitEmblem" ) );
			if ( NULL != pStaticEmblem )		
			{
				pStaticEmblem->SetShow ( true );
				pStaticEmblem->GetPicture(0)->SetShow( true );

				if ( NULL != g_pMain->GetPVPEmblem() )
				{
					CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnit->GetPvpRank() );
					if ( pPVPEmblemData != NULL )
					{
						pStaticEmblem->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
					}
				}
			}
#endif //REFORM_ENTRY_POINT

			switch( pUnit->GetType() )
			{
			case CX2Unit::UT_ELSWORD:
			case CX2Unit::UT_LIRE:
			case CX2Unit::UT_ARME:
			case CX2Unit::UT_RAVEN:
			case CX2Unit::UT_EVE:

	#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
	#endif	// NEW_CHARACTER_CHUNG

	#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
	#endif // ARA_CHARACTER_BASE

	#ifdef NEW_CHARACTER_EL
			case CX2Unit::UT_ELESIS:
	#endif // NEW_CHARACTER_EL

	#ifdef SERV_9TH_NEW_CHARACTER 
			case CX2Unit::UT_ADD:
	#endif //SERV_9TH_NEW_CHARACTER
				break;


			default:
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375), GET_STRING( STR_ID_707 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 (0, 0),L"UI_PopUp_Negative_01.ogg" );
				}break;
			}
			XSkinMeshReady( pUnit->GetUnitTemplet()->m_MotionFile.c_str() );


			wstringstream buttonName;
			buttonName << L"SLOT_BUTTON_" << pUnit->GetUID();
			if( NULL != pSelectUnitButton )
			{
				pSelectUnitButton->SetName( buttonName.str().c_str() );
			}
		
			CKTDGUIStatic* pStaticLV = (CKTDGUIStatic*)pUnitSlot->GetControl( L"lv" );
			if ( NULL != pStaticLV)
			{
				pStaticLV->SetShow ( true );
			}
			
			WCHAR buff[256] = {0,};
			StringCchPrintf( buff, 256, L"%d", static_cast<int> ( pUnit->GetUnitData().m_Level ) );
			CKTDGUIStatic* pStaticLVNum = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"StaticUnitSelectStringLVNum" ) );
			
			if ( NULL != pStaticLVNum )
			{
				pStaticLVNum->SetShow ( true );
				pStaticLVNum->GetString(0)->msg = buff;
			}

			CKTDGUIStatic* pStaticID = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"StaticUnitSelectStringID" ) );
			if ( NULL != pStaticID )
			{
				pStaticID->SetShow ( true );
			}

			CKTDGUIStatic* pStaticChangeNickEmblem = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"ChangeNick" ) );
			

			CKTDGUIStatic* pStaticChangeNickMessage = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"standby" ) );
			
			if ( pStaticChangeNickEmblem != NULL && pStaticChangeNickMessage != NULL )
			{
				wstring tempNickName = pUnit->GetNickName();

				if ( tempNickName.find( L"__DELETED__") != -1 
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
					|| tempNickName.find( GET_STRING(STR_ID_29404) ) != -1
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
					)
				{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
					if( NULL != pStaticID )
					{
						pStaticID->GetString(0)->msg = GET_STRING(STR_ID_29412);
						pStaticID->SetOffsetPos(D3DXVECTOR2(-22,-3));
					}
					
					pStaticChangeNickEmblem->SetShowEnable( true, true );
					if( NULL != pStaticLVNum && NULL != pStaticLV )
					{
						pStaticLVNum->SetShow ( false );
						pStaticLV->SetShow ( false );
					}
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
					pStaticID->GetString(0)->msg = L"     ?";
					pStaticChangeNickEmblem->SetShowEnable( true, true );
					pStaticChangeNickMessage->SetShowEnable( true, true );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				}
				else
				{
					pStaticID->GetString(0)->msg = pUnit->GetNickName();
					pStaticID->SetOffsetPos(D3DXVECTOR2(0,0));
					pStaticChangeNickEmblem->SetShowEnable( false, false );
					pStaticChangeNickMessage->SetShowEnable( false, false );
				}
			}
			CKTDGUIStatic* pStaticLastPlace = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"StaticUnitSelectStringID_LastPlace" ) );
			if ( NULL != pStaticLastPlace )
			{
				pStaticLastPlace->SetShow ( true );
				
				wstring wstringLastPlace = L"";
				if ( pUnit->GetServerGroupID() == SEnum::SGI_GAIA )
				{
					wstringLastPlace = L"#C007000";
					wstringLastPlace += GET_STRING(STR_ID_4835);
					wstringLastPlace += L"#CX";
					wstringLastPlace += L" : ";
				}
				else
				{
					wstringLastPlace = L"#CFF0000";
					wstringLastPlace += GET_STRING(STR_ID_4836);
					wstringLastPlace += L"#CX";
					wstringLastPlace += L" : ";
				}

				if ( NULL != g_pData->GetLocationManager() )
				{
					int iMapID = pUnit->GetUnitData().m_nMapID;
					
					wstring wstrLastPlace = L""; 

					if ( iMapID >= SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01 && SEnum::VMI_BATTLE_FIELD_END > iMapID )
					{
						CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
						wstrLastPlace = battleFieldManager.GetBattleFieldNameByBattleFieldId( iMapID );
					}

					else if ( iMapID == SEnum::VMI_TUTORIAL )
					{
						wstrLastPlace = GET_STRING(STR_ID_2761);
					}
					else
					{
						CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( (SEnum::VILLAGE_MAP_ID) iMapID );
						if ( NULL != pVillageTemplet )
						{
							wstrLastPlace = pVillageTemplet->m_Name;
						}

					}
					
					wstringLastPlace += wstrLastPlace.c_str();
#ifdef SERV_GLOBAL_COMMON
					if ( wstringLastPlace.length() >= 40 )
					{
						wstringLastPlace = wstringLastPlace.substr(0, 38);
						wstringLastPlace += L"..";
					}
#else //SERV_GLOBAL_COMMON
					if ( wstringLastPlace.length() >= 24 )
					{
						wstringLastPlace = wstringLastPlace.substr(0, 22);
						wstringLastPlace += L"..";
					}
#endif //SERV_GLOBAL_COMMON
					pStaticLastPlace->GetString(0)->msg = wstringLastPlace.c_str();
				}
				

			
			}

			CKTDGUIStatic* pStaticUnitFace = static_cast<CKTDGUIStatic*> ( pUnitSlot->GetControl( L"UnitFace" ) );
			if ( NULL != pStaticUnitFace )
			{
				pStaticUnitFace->SetShow ( true );
				wstring fileName, pieceName;		
				if ( true == CX2Data::GetCharacterImageName( fileName, pieceName, pUnit->GetClass(), CX2Data::CIT_UnitSlot ) )
				{				
					CKTDGUIControl::CPictureData* pPictureData = pUnitSlot->CreatePicture();
					pStaticUnitFace->AddPicture ( pPictureData );
					pPictureData->SetTex ( fileName.c_str(), pieceName.c_str() );
					CKTDGUIControl::UIPointData* pPointData = new CKTDGUIControl::UIPointData();
					pPointData->leftTopPoint = D3DXVECTOR2 ( 23, 11 );
#ifdef SERV_UNIT_WAIT_DELETE
					if( pUnit->GetUnitData().m_bDeleted == true )
					{
						pPointData->color = D3DXCOLOR ( 0.5f, 0.5f, 0.5f, 0.5f );
					}
					else
					{
						pPointData->color = D3DXCOLOR ( 1.0f, 1.0f, 1.0f, 1.0f );
					}
#else
					pPointData->color = D3DXCOLOR ( 1.0f, 1.0f, 1.0f, 1.0f );
#endif SERV_UNIT_WAIT_DELETE
					pPictureData->SetPoint ( pPointData );
					pPictureData->SetSize( D3DXVECTOR2 ( 38, 38 ) );
				}
			}

			if ( pUnit == m_pSelectUnit )
			{
				CKTDGUIButton* pCharButton = reinterpret_cast<CKTDGUIButton*>( pUnitSlot->GetControl( 0 ) );
				pCharButton->SetDownStateAtNormal( true );
			}
		}
		PlayDownAnim();
	}
}

bool CX2StateServerSelect::Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK ) == true )
	{
		typedef std::map< int, int > TodayCreateUnitCount;
		
		BOOST_TEST_FOREACH( const TodayCreateUnitCount::value_type&, Count, kEvent.m_mapCreateCharCountToday )
		{
			switch ( Count.first )
			{
				case SEnum::SGI_SOLES :
					m_iCreateUnitTodayCount += Count.second;
					break;

				case SEnum::SGI_GAIA :
					m_iCreateUnitTodayCount += Count.second;
					break;

				default :
					break;
			}
		}		
	}
	return true;
}

bool CX2StateServerSelect::Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ ()
{
	g_pData->GetServerProtocol()->SendID( EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ );
	g_pMain->AddServerPacket( EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK );
	return true;
}


void CX2StateServerSelect::Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ()
{
	g_pData->GetServerProtocol()->SendID( EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ );
	g_pMain->AddServerPacket( EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK );
}
bool CX2StateServerSelect::Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	SAFE_DELETE_DIALOG ( m_pDLGMsgBox );
	ClearUnitButton ( false );
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����

#ifdef FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG
#pragma region Auto Keyword
	//////////////////////////////////////////////////////////////////////////
	// 1. ���ʽ� ä�� �����ϰ�,
	// 1. ä�� ���ʽ� ������ ��ȸ�Ͽ�, ��õ ������ �ش��ϴ� ä���� ID���
	auto SetChannelBonusInfo = [](  IN const int iSelectedUnitLevel_, 
									IN const std::map< int, KChannelBonusInfo >& kPacketChannelBonus_, 
									OUT vector<int>& vecRecommandedLevelChannelID_,
									OUT map< int, KChannelBonusInfo >& mapChannelBonusInfo_ ) 
	{
		std::map< int, KChannelBonusInfo >::const_iterator mitChannelBonusInfo = // key : ä�� ID, second : ���ʽ� ����
			kPacketChannelBonus_.begin();
				
		for( ; mitChannelBonusInfo != kPacketChannelBonus_.end(); ++mitChannelBonusInfo )
		{
			const int& iChannelID = mitChannelBonusInfo->first;

			if ( mitChannelBonusInfo->second.m_iBeginLv <= iSelectedUnitLevel_ && iSelectedUnitLevel_ <= mitChannelBonusInfo->second.m_iEndLv )
				vecRecommandedLevelChannelID_.push_back( iChannelID );
				
			/*# TODO : �ڷḦ �ߺ��ؼ� �����ϰ� ����.
			���� ��� 1 ) ��ü ������ ���ʽ� ������ ��� �����̳� ����.
						2 ) ��ü ������ ���ʽ� ���� ��� ���� , ���� ���� �߰��ϱ�.*/

			g_pInstanceData->m_mapChannelBonusInfo.insert( std::make_pair(iChannelID, mitChannelBonusInfo->second) );
			mapChannelBonusInfo_.insert( std::make_pair ( iChannelID, mitChannelBonusInfo->second ) ); 		
		}

	};

	//////////////////////////////////////////////////////////////////////////
	// ä�� ����Ʈ ����(��õ ������ �ش��ϴ� ä���� ������ ��ġ)
	auto SetChannelList = []( IN const std::map< int, KChannelInfo >& kPacketChannelList_, 
							IN const vector<int>& vecRecommandedLevelChannelID_,
							OUT vector< KChannelInfo >& vecChannelInfo ) 
	{

		// #1 ��õ ������ �ش��ϴ� ä�� ���� �߰��ϱ�
		std::map< int, KChannelInfo >::const_iterator mitChannelInfo;
		BOOST_FOREACH( const int& iChannelID, vecRecommandedLevelChannelID_ )
		{
			mitChannelInfo = kPacketChannelList_.find( iChannelID );
			if( mitChannelInfo != kPacketChannelList_.end() )
				vecChannelInfo.push_back( mitChannelInfo->second );
		}

		// #2 ��õ ���� �̿� ä�� �߰��ϱ�

		mitChannelInfo = kPacketChannelList_.begin();
		for( ; mitChannelInfo != kPacketChannelList_.end(); ++ mitChannelInfo )
		{
			bool bAredyInsert = false;
			BOOST_FOREACH( const int& iChannelID, vecRecommandedLevelChannelID_ )
			{
				if( iChannelID == mitChannelInfo->first )
					bAredyInsert = true;
			}

			if( false == bAredyInsert )
				vecChannelInfo.push_back( mitChannelInfo->second );

			// ���ĵǸ� �� �Ǳ� ������ g_pInstanceData->m_vecChannelInfo�� ���� ó��
			g_pInstanceData->m_vecChannelInfo.push_back( mitChannelInfo->second );
		}
	};
#pragma endregion Auto Keyword

	SEnum::SERVER_GROUP_ID eSelectedCharacterServerGroupID = SEnum::SGI_INVALID;
	if ( NULL != m_pSelectUnit )
	{
		eSelectedCharacterServerGroupID = m_pSelectUnit->GetServerGroupID();
#ifdef FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
		// �ڱ� �ڸ��� ���� �÷��� �� �߻��ϴ� ������ ���� �ӽ� ���� ó��
		if( SEnum::SGI_END <= eSelectedCharacterServerGroupID)
			eSelectedCharacterServerGroupID = SEnum::SGI_SOLES;
#endif // FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
	}
	else
	{
		// ĳ���Ͱ� ���õ��� �ʾҴٸ� �� �̻� ���� �ϸ� �� ��.
		return true;
	}
	ASSERT( SEnum::SGI_INVALID != eSelectedCharacterServerGroupID );

	InitChannelInfo();

	vector<int> vecRecommandedLevelChannelID;
	switch( eSelectedCharacterServerGroupID )
	{
	case SEnum::SGI_SOLES:
		{
			// ä�� ���ʽ� ������ ��ȸ�Ͽ�, ��õ ������ �ش��ϴ� ä���� ID���
			// m_pSelectUnit ���� ��ó���� ������ ó��
			SetChannelBonusInfo( m_pSelectUnit->GetUnitData().m_Level, kEvent.m_mapSolesChannelBonusList, 
								vecRecommandedLevelChannelID, m_mapSolesChannelBonusInfo );

			// ��õ ���� ä���� ������ ��ġ �Ͽ� ä�� ��� �߰�
			m_vecSolesChannelInfo.reserve( kEvent.m_mapSolesChannelList.size() );
			SetChannelList( kEvent.m_mapSolesChannelList, vecRecommandedLevelChannelID, m_vecSolesChannelInfo );
		} break;
	case SEnum::SGI_GAIA:
		{
			// ä�� ���ʽ� ������ ��ȸ�Ͽ�, ��õ ������ �ش��ϴ� ä���� ID���
			SetChannelBonusInfo( m_pSelectUnit->GetUnitData().m_Level, kEvent.m_mapGaiaChannelBonusList, 
								vecRecommandedLevelChannelID, m_mapGaiaChannelBonusInfo );

			// ��õ ���� ä���� ������ ��ġ �Ͽ� ä�� ��� �߰�
			m_vecGaiaChannelInfo.reserve( kEvent.m_mapGaiaChannelList.size() );
			SetChannelList( kEvent.m_mapGaiaChannelList, vecRecommandedLevelChannelID, m_vecGaiaChannelInfo );
		} break;
	default:
		break;
	}

	m_bReserveEnterTutorial = false;
	m_bReservedConnectionToChannelServer = false;

	if( NULL != g_pInstanceData->GetMiniMapUI() )
		g_pInstanceData->GetMiniMapUI()->SetChannelComboBox();

	if( g_pMain->GetIsPlayingTutorial() == false )
		CreateChannelButton ( true );	

	SetShowServerStateButtonSet ( false, true );
#ifndef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	ClearUnitButton ( false );
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
	
#else
	std::map< int, KChannelBonusInfo >::const_iterator cmit;
	SEnum::SERVER_GROUP_ID eServerGroupID = SEnum::SGI_INVALID;
	if ( NULL != m_pSelectUnit )
	{
		eServerGroupID = m_pSelectUnit->GetServerGroupID();
#ifdef FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
		if( SEnum::SGI_END <= eServerGroupID)
		{
			eServerGroupID = SEnum::SGI_SOLES;
		}
#endif // FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
	}
	vector<int> vecRecommandedLevelChannelID;
	m_mapGaiaChannelBonusInfo.clear();
	for( cmit = kEvent.m_mapGaiaChannelBonusList.begin() ; cmit != kEvent.m_mapGaiaChannelBonusList.end(); ++cmit )
	{
		if ( eServerGroupID == SEnum::SGI_GAIA )
		{
			if ( cmit->second.m_iBeginLv <= m_pSelectUnit->GetUnitData().m_Level && m_pSelectUnit->GetUnitData().m_Level <= cmit->second.m_iEndLv )
				vecRecommandedLevelChannelID.push_back( cmit->first );
			g_pInstanceData->m_mapChannelBonusInfo.insert( std::make_pair(cmit->first, cmit->second) );
		}

		m_mapGaiaChannelBonusInfo.insert( std::make_pair ( cmit->first, cmit->second ) ); 		
	}

	m_mapSolesChannelBonusInfo.clear();
	for( cmit = kEvent.m_mapSolesChannelBonusList.begin() ; cmit != kEvent.m_mapSolesChannelBonusList.end(); ++cmit )
	{
		if ( eServerGroupID == SEnum::SGI_SOLES )
		{
			if ( cmit->second.m_iBeginLv <= m_pSelectUnit->GetUnitData().m_Level && m_pSelectUnit->GetUnitData().m_Level <= cmit->second.m_iEndLv )
				vecRecommandedLevelChannelID.push_back( cmit->first );
			g_pInstanceData->m_mapChannelBonusInfo.insert( std::make_pair(cmit->first, cmit->second) );
		}
		m_mapSolesChannelBonusInfo.insert( std::make_pair ( cmit->first, cmit->second ) ); 		
	}


	std::map< int, KChannelInfo >::const_iterator cit;
	g_pInstanceData->m_vecChannelInfo.clear();
	g_pInstanceData->m_mapChannelBonusInfo.clear();
	m_vecGaiaChannelInfo.clear();
	for( cit = kEvent.m_mapGaiaChannelList.begin(); cit != kEvent.m_mapGaiaChannelList.end(); cit++ )
	{
		KChannelInfo channelInfo = cit->second;

		bool bInsertToFront = false;
		if ( eServerGroupID == SEnum::SGI_GAIA )
		{		
			for ( vector<int>::iterator it = vecRecommandedLevelChannelID.begin(); it != vecRecommandedLevelChannelID.end(); ++it )
			{
				if ( (*it) == cit->first )
				{
					m_vecGaiaChannelInfo.insert ( m_vecGaiaChannelInfo.begin(), channelInfo );
					bInsertToFront = true;
				}
			}
			g_pInstanceData->m_vecChannelInfo.push_back( channelInfo );
		}
		if ( false == bInsertToFront )
			m_vecGaiaChannelInfo.push_back( channelInfo );
	}

	m_vecSolesChannelInfo.clear();
	for( cit = kEvent.m_mapSolesChannelList.begin(); cit != kEvent.m_mapSolesChannelList.end(); cit++ )
	{
		KChannelInfo channelInfo = cit->second;

		bool bInsertToFront = false;
		if ( eServerGroupID == SEnum::SGI_SOLES )
		{
			for ( vector<int>::iterator it = vecRecommandedLevelChannelID.begin(); it != vecRecommandedLevelChannelID.end(); ++it )
			{
				if ( (*it) == cit->first )
				{
					m_vecSolesChannelInfo.insert ( m_vecSolesChannelInfo.begin(), channelInfo );
					bInsertToFront = true;
				}
			
			}
			g_pInstanceData->m_vecChannelInfo.push_back( channelInfo );
		}
		if ( false == bInsertToFront )
			m_vecSolesChannelInfo.push_back( channelInfo );
	}


	if ( m_pSelectUnit != NULL )
	{
		g_pInstanceData->GetMiniMapUI()->SetChannelComboBox();
		m_bReservedConnectionToChannelServer = false;

		m_bReserveEnterTutorial = false;
		SetShowServerStateButtonSet ( false, true );
		ClearUnitButton ( false );		
#ifdef REFORM_ENTRY_POINT
		if( g_pMain->GetIsPlayingTutorial() == false )
#endif //REFORM_ENTRY_POINT
		{
			CreateChannelButton ( true );	
		}
	}
#endif // FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG

	return true;
}
#ifdef FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG
void CX2StateServerSelect::InitChannelInfo()
{
	// ä���� ���ʽ� ���� �ʱ�ȭ
	m_mapGaiaChannelBonusInfo.clear();
	m_mapSolesChannelBonusInfo.clear();
	g_pInstanceData->m_mapChannelBonusInfo.clear();
	// ä�� ���� �ʱ�ȭ
	m_vecGaiaChannelInfo.clear();
	m_vecSolesChannelInfo.clear();
	g_pInstanceData->m_vecChannelInfo.clear();
}
#endif // FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG

bool CX2StateServerSelect::Handler_EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly, SEnum::SERVER_GROUP_ID eGroupID )
{
	KEGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ kPacket;

	kPacket.m_iUnitUID		= uid;
	kPacket.m_wstrNickName	= wszNickName;
	kPacket.m_bCheckOnly	= bCheckOnly;
	kPacket.m_iServerGroup	= eGroupID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_NICK_NAME_ACK );
	return true;
}

bool CX2StateServerSelect::ChannelButtonUp( CKTDGUIButton * pButton )
{

#ifdef SERV_SECOND_SECURITY
	if(m_bEnableUnitSelectBySecondSecurity == false)
	{
		return true;
	}
#endif SERV_SECOND_SECURITY

#ifdef FIX_REFORM_ENTRY_POINT_3RD		// ������, ���� ���� ���� 3�� ( ũ���� ) ����
	if ( NULL == m_pSelectUnit )
		return true;
#endif // FIX_REFORM_ENTRY_POINT_3RD	// ������, ���� ���� ���� 3�� ( ũ���� ) ����

	for ( int iChannel = 0;  iChannel < static_cast<int> ( m_vecChannelSlot.size() ); ++iChannel )
	{
		CKTDGUIDialogType pChannelSlot =  m_vecChannelSlot[iChannel];

		if ( NULL != pChannelSlot )
		{	
			pChannelSlot->SetDownStateToAllButtonType ();
		}
	}

	if ( NULL != pButton )
		pButton->SetDownStateAtNormal( true );

	if ( m_pSelectChannelButton == pButton )
	{
#ifdef FIX_REFORM_ENTRY_POINT
		
#ifndef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
		int iCheckEntryPointServerChannelIndex = m_pSelectChannelButton->GetDummyInt( 0 );
		bool bIsConnectable = IsConnectableToGameServer ( m_pSelectUnit->GetServerGroupID(), iCheckEntryPointServerChannelIndex );
		if ( true == bIsConnectable )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING ( STR_ID_29885 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
		}
#else // FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
		vector< KChannelInfo > * vecChannelInfo = NULL;
		if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_GAIA )
		{
			vecChannelInfo = &m_vecGaiaChannelInfo;
		}
		else if ( m_pSelectUnit->GetServerGroupID() == SEnum::SGI_SOLES )
		{
			vecChannelInfo = &m_vecSolesChannelInfo;
		}
		else 
		{
			ASSERT (!L"Can't Find Server Group ID !! ");
		}

		int iCheckEntryPointServerChannelIndex = m_pSelectChannelButton->GetDummyInt( 0 );

	#ifdef FIX_REFORM_ENTRY_POINT_6TH		// kimjh, ä�� ��ư ���� �ҽ� ����, ���� ������ �� ������ ���� �� �� �ֵ��� ����
		float fChannelCongestionRate = 
			static_cast< float >(  (*vecChannelInfo)[ iCheckEntryPointServerChannelIndex ].m_iCurrentUser ) / static_cast< float >(  (*vecChannelInfo)[ iCheckEntryPointServerChannelIndex ].m_iMaxUser );
		
		if ( fChannelCongestionRate >= CHANNEL_CONGESTION_SCOPE_TEMP[3] )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING ( STR_ID_29885 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
		}
	#else // FIX_REFORM_ENTRY_POINT_6TH		// kimjh, ä�� ��ư ���� �ҽ� ����, ���� ������ �� ������ ���� �� �� �ֵ��� ����
		if ( (*vecChannelInfo)[ iCheckEntryPointServerChannelIndex ].m_iCurrentUser * 0.9 >= (*vecChannelInfo)[ iCheckEntryPointServerChannelIndex ].m_iMaxUser )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(305, 375 ), GET_STRING ( STR_ID_29885 ), this, -1, -1.f, L"DLG_UI_Selection_MessageBox_Ok_Button_New.lua", D3DXVECTOR2 ( 0, 0 ), L"UI_PopUp_Negative_01.ogg" );
		}
	#endif // FIX_REFORM_ENTRY_POINT_6TH		// kimjh, ä�� ��ư ���� �ҽ� ����, ���� ������ �� ������ ���� �� �� �ֵ��� ����

#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
		else
#endif // FIX_REFORM_ENTRY_POINT
		{
			m_bReserveEnterTutorial = false;
			m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection = true;
			m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection = true;
			m_bENX_USER_LOGIN_NOT		= false;
			m_bEGS_VERIFY_ACCOUNT_REQ	= false;
			m_bEGS_VERIFY_ACCOUNT_ACK	= false;
			m_bPortCheckReq				= false;

			m_bSentEGS_CONNECT_REQ 		= false;
			m_bReceivedChannelList 		= false;
#ifdef FIX_REFORM_ENTRY_POINT_4TH				// ������, ���� ���� ���� 4�� ( ���� ��� ) ����
			m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp = true;
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// ������, ���� ���� ���� 4�� ( ���� ��� ) ����

			if ( NULL != pButton )
			{
				int iChIndex = pButton->GetDummyInt ( 0 );// - 1 ) + ( m_iChannelNowPage - 1 ) * CHANNEL_SELECT_UI_MAX_CHANNEL_NUM;
				m_iReservedEntryPointServerChannelIndex = iChIndex;
			}


			// ���Ӽ������� ������ ���´�				
		
			Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
		}
	}
	else
	{
		m_pSelectChannelButton = pButton;
	}
	return true;
}
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
void CX2StateServerSelect::ConnectToLowerUserCountGameServer ( bool bIsConnectAnotherServerGroup /* = false */  )
#else  // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
void CX2StateServerSelect::ConnectToLowerUserCountGameServer ()
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
{
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ������, ���� ���� ���� ���� 2��
	SEnum::SERVER_GROUP_ID eConnectingServerGroupID = SEnum::SGI_INVALID;
	if ( NULL != m_pSelectUnit )
	{
		eConnectingServerGroupID = m_pSelectUnit->GetServerGroupID();
	}
	else
	{
		eConnectingServerGroupID = static_cast<SEnum::SERVER_GROUP_ID> ( g_pInstanceData->GetServerGroupID() ); 
	}

	if ( true == bIsConnectAnotherServerGroup )
	{
		// �������� 3�� �̻� �� ��, �Ʒ� �ҽ��� �����ؾ� �մϴ�.
		if ( eConnectingServerGroupID == SEnum::SGI_GAIA)
			eConnectingServerGroupID = SEnum::SGI_SOLES;
		else if ( eConnectingServerGroupID == SEnum::SGI_SOLES)
			eConnectingServerGroupID = SEnum::SGI_GAIA;
		else
			ASSERT ( !L"Can not Find Server Group ID !");
	}
	ASSERT ( eConnectingServerGroupID != SEnum::SGI_INVALID );
	
	if ( SEnum::SGI_GAIA == eConnectingServerGroupID )
	{
		FindAndConnectLowerUserCountGameServer ( m_vecGaiaChannelInfo );
	}

	else if ( SEnum::SGI_SOLES == eConnectingServerGroupID )
	{
		FindAndConnectLowerUserCountGameServer ( m_vecSolesChannelInfo );
	}
	else
	{
		ASSERT( !L"Can't Find Server Group ID !");
	}
#else  // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
	if ( NULL != m_pSelectUnit )
	{
		if ( SEnum::SGI_GAIA == m_pSelectUnit->GetServerGroupID() )
		{
			FindAndConnectLowerUserCountGameServer ( m_vecGaiaChannelInfo );
		}

		else if ( SEnum::SGI_SOLES == m_pSelectUnit->GetServerGroupID() )
		{
			FindAndConnectLowerUserCountGameServer ( m_vecSolesChannelInfo );
		}
		else
		{
			ASSERT( !L"Can't Find Server Group ID !");
			m_bReConnectChannelServer = true;
		}
	}

	else
	{
		m_bCanNotConenctToBusyServer = false;
		if ( SEnum::SGI_GAIA == g_pInstanceData->GetServerGroupID() )
		{
			FindAndConnectLowerUserCountGameServer ( m_vecGaiaChannelInfo );
		}
		else if ( SEnum::SGI_SOLES == g_pInstanceData->GetServerGroupID() )
		{
			FindAndConnectLowerUserCountGameServer ( m_vecSolesChannelInfo );
		}
		else
		{
			ASSERT( !L"Can't Find Server Group ID !");
			m_bReConnectChannelServer = true;
		}
	}
#endif // FIX_REFORM_ENTRY_POINT_2TH // ������, ���� ���� ���� ���� 2��
	
	
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef ADD_PLAY_SOUND //��â��
void CX2StateServerSelect::PlaySoundSelect( CX2Unit::UNIT_TYPE eUnitType_, SOUND_PLAY_TYPE eSoundPlayType_ )
{
	if( g_pKTDXApp->GetDeviceManager() == NULL )
		return;

	wstring wstrSoundName = L"";

	switch( eUnitType_ )
	{
	case CX2Unit::UT_ELSWORD:
		{
			wstrSoundName = L"Elsword";
		} break;
	case CX2Unit::UT_ARME:
		{
			wstrSoundName = L"Aisha";
		} break;
	case CX2Unit::UT_LIRE:
		{
			wstrSoundName = L"Rena";
		} break;
	case CX2Unit::UT_RAVEN:
		{
			wstrSoundName = L"Raven";
		} break;
	case CX2Unit::UT_EVE:
		{
			wstrSoundName = L"Eve";
		} break;
	case CX2Unit::UT_CHUNG:
		{
			wstrSoundName = L"Chung";
		} break;
	case CX2Unit::UT_ARA:
		{
			wstrSoundName = L"Ara";
		} break;
	case CX2Unit::UT_ELESIS:
		{
			wstrSoundName = L"Elesis";
		} break;
	case CX2Unit::UT_ADD:
		{
			wstrSoundName = L"Add";
		} break;

	default:
		break;
	}

	switch( eSoundPlayType_)
	{
	case SPT_UNIT_SELECT:
		{
			wstrSoundName += L"Voice_Character_Select.ogg";
		} break;
	case SPT_SERVER_SELECT:
		{
			wstrSoundName += L"Voice_GameStart.ogg";
		} break;

	default:
		break;
	}

	if( StrCmp( wstrSoundName.c_str(), L"" ) != 0 )
	{
		if( NULL != m_pSound )
		{
			m_pSound->Stop();
			SAFE_CLOSE( m_pSound );
		}
		m_pSound = g_pKTDXApp->GetDeviceManager()->PlaySound(wstrSoundName.c_str(), false, false );
	}
}
#endif //ADD_PLAY_SOUND

/** @function : IsCharacterSelectUIMode
	@brief : ���� ä�μ��� â���� Ȯ��.
			 Ư���� ���� �߰����� �ʱ� ���� ä�θ���Ʈ�� �˻� ��
*/
bool CX2StateServerSelect::IsChannelSelectMode()
{
	// m_vecChannelSlot�� ������� �ʴ� ���´� ä�� ����â���� ���� ��
	if( false == m_vecChannelSlot.empty() )
		return true;

	return false;
}

#ifdef FIX_REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
void CX2StateServerSelect::FindAndConnectLowerUserCountGameServer ( const vector< KChannelInfo > & vecChannelInfo )
{
	int iLowerUserChannelCount = INT_MIN;
	int iLowerUserChannelIndex = -1;
	int iLowerUserChannelID = -1;
	if ( false == vecChannelInfo.empty () )
	{
		int iIndex = 0;
		BOOST_FOREACH ( const KChannelInfo & kChannelInfo, vecChannelInfo )
		{
			if ( iLowerUserChannelCount < kChannelInfo.m_iMaxUser - kChannelInfo.m_iCurrentUser )
			{
#ifdef FIX_REFORM_ENTRY_POINT_10TH		//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
				if ( false == g_pInstanceData->IsConnectedChannel( kChannelInfo.m_iServerUID ) )
#endif // FIX_REFORM_ENTRY_POINT_10TH	//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
				{
					iLowerUserChannelCount = kChannelInfo.m_iMaxUser - kChannelInfo.m_iCurrentUser;
					iLowerUserChannelIndex = iIndex;
					m_iTryConnectChannelID = kChannelInfo.m_iChannelID;
					g_pMain->SetConnectedGameServerIP( kChannelInfo.m_wstrIP );
					g_pMain->SetConnectedGameServerPort( kChannelInfo.m_usMasterPort );

#ifdef FIX_REFORM_ENTRY_POINT_10TH		//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
					g_pInstanceData->SetConnectedChannelID( kChannelInfo.m_iServerUID  );
#endif // FIX_REFORM_ENTRY_POINT_10TH	//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
				}	
			}
			++iIndex;
		}
		if ( iLowerUserChannelIndex < 0 || iLowerUserChannelCount < INT_MIN ) 
		{
			ASSERT ( L"Can't Find Channel Index !");
			m_bReConnectChannelServer = true;
			return ;			
		}

		m_bSelectChannel = true;
		m_bSentEGS_CONNECT_REQ = false;
		m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection = false;
		m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection = false;
		m_sbSelectedServerSet = false;


		g_pData->GetServerProtocol()->DisconnectFromGameServer();
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_SERVER_SELECT, NULL, false ); 
	}
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// ������, ���Ա��� ����, ������ ���� �˰��� ����

int CX2StateServerSelect::FindAndConnectMatchingLevelGameServer ( int iMatchingUnitLevel, const map<int, KChannelBonusInfo > & mapChannelBonusInfo, const vector< KChannelInfo > & vecChannelInfo )
{
	if ( false == mapChannelBonusInfo.empty () )
	{
		vector <int> vecRecommandedLevelChannelID;

		std::map< int, KChannelBonusInfo >::const_iterator cmit;
		for( cmit = mapChannelBonusInfo.begin() ; cmit != mapChannelBonusInfo.end(); ++cmit )
		{
			if ( cmit->second.m_iBeginLv <= iMatchingUnitLevel && iMatchingUnitLevel <= cmit->second.m_iEndLv )
				vecRecommandedLevelChannelID.push_back ( cmit->first );
		}
		if ( true == vecRecommandedLevelChannelID.empty() )
			return -1;
		
		int iLowerUserChannelCount = INT_MIN;
		int iPickedChannelID = -1;

		BOOST_FOREACH ( const KChannelInfo & kChannelInfo, vecChannelInfo )
		{
			BOOST_FOREACH ( const int & iRecommandedLevelChannelID, vecRecommandedLevelChannelID )
			{
				if ( iRecommandedLevelChannelID == kChannelInfo.m_iChannelID )
				{
					if ( iLowerUserChannelCount < kChannelInfo.m_iMaxUser - kChannelInfo.m_iCurrentUser )
					{
						iLowerUserChannelCount = kChannelInfo.m_iMaxUser - kChannelInfo.m_iCurrentUser;
						iPickedChannelID = iRecommandedLevelChannelID;
					}	
				}
			}
		}
		return iPickedChannelID;		
	}
	return -1;
}


#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// ������, ���Ա��� ����, ������ ���� �˰��� ����

#ifdef FIX_REFORM_ENTRY_POINT_5TH				// ������, ���� ���� ���� 5�� ����
void CX2StateServerSelect::ProcessIntroMovieEnd ()		// ��Ʈ�� ���� ( �ֵ� �߰� ������ �� ) �� ������ ���� ó��
{
	// ���� �е带 ������ ��� ���Ŀ� ��µǵ��� �Ѵ�.
	if ( true == m_bReservedSecurityPad )
	{
		m_bReservedSecurityPad = false;
		g_pMain->GetSecurityPad()->SetShow(true, CX2SecurityPad::SPS_GAME_START);
		g_pMain->GetSecurityPad()->SetUseSecondPW(true);
	}

	// ��ŷ ���� ���� ����â�� ������ ��� ���Ŀ� ��µǵ��� �Ѵ�.
	if ( true == m_bReservedHackingUserAgreeDlg )
	{
		m_bReservedHackingUserAgreeDlg = false;
		m_pDLGCheckHack = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(305, 375), 
			GET_STRING( STR_ID_729 ), 
			SSSUCM_HACK_USER_PROCESS_CHECK_OK, this, SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL, L"DLG_UI_Selection_MessageBox_Ok_Exit_Button_New.lua", D3DXVECTOR2( 0, 10 ), L"UI_PopUp_Negative_01.ogg" );

		// MsgBox �� ESC �� ������ Ư���� ������ �ִ�, ������ �� â�� ��� �����Ǿ�� �ǴܵǾ�
		// �ش� ���� ���� false �� �����Ѵ�.	kimjh
		if ( NULL != m_pDLGCheckHack )
			m_pDLGCheckHack->SetOKAndCancelMsgBox( false );
		
	}
}
#endif // FIX_REFORM_ENTRY_POINT_5TH				// ������, ���� ���� ���� 5�� ����