#include "StdAfx.h"
#include ".\x2characterroom.h"




//bool CX2CharacterRoom::m_sbCheckQuestionSellNum = true;
//
//
//
//CX2CharacterRoom::CX2CharacterRoom( CKTDXStage* pNowStage, const WCHAR* pFileName )
//: CX2ItemSlotManager( pNowStage, pFileName )
//{
//	RegisterLuaBind();
//	m_pDLGSkillSlot = NULL;
//
//	
//
//	m_pDLGMyInfoFront = new CKTDGUIDialog( pNowStage, L"DLG_Unit_State_Front.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyInfoFront );
//	
//	m_pDLGMyInfoInventory = new CKTDGUIDialog( pNowStage, L"DLG_Unit_State_Inventory.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyInfoInventory );
//
//
///*
//	CKTDGUIControl* pControlattribEnchant = m_pDLGMyInfoInventory->GetControl( L"attribEnchant" );
//	if ( pControlattribEnchant != NULL )
//	{
//		pControlattribEnchant->SetShowEnable( true, true );
//	}
//*/
//
//
//	CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl(L"ResaleCheckBox");
//	if ( pCheckBox != NULL )
//		pCheckBox->SetChecked( !m_sbCheckQuestionSellNum );
//
//	if ( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
//	{
//		CKTDGUIControl* pControl = m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//			pControl->SetEnable( false );
//		}
//	}
//
//	m_pSkillTreeUI			= new CX2SkillTreeUI( pNowStage );
//
//	m_pDLGSkillSlot = new CKTDGUIDialog( pNowStage, L"DLG_Character_Room_Skill_Slot.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillSlot );
//
//
//	m_pNowState		= (CX2State*) pNowStage;
//	m_pStateMenu	= (CX2StateMenu*) pNowStage; // note!! statemenu�� ��ӹ��� state������ character room�� ���δٰ� ��������.
//
//	CX2UnitViewerUI::SetUnitClicked( false );
//	m_pUnitViewerUI = NULL;
//	m_fMouseSensitivity = 0.01f;
//	m_fRemainRotY = 0.0f;
//	m_bRotateReturn = true;
//
//	m_pUnitViewerUI = new CX2UnitViewerUI();
//	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
//	m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
//	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
//
//
//	m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit() );
//	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -60, 200 ) );
//	m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
//	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
//	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
//
//	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
//
//	m_UnitClickPos = D3DXVECTOR2( 145, 145 );
//	m_UnitClickSize = D3DXVECTOR2( 237, 297 );
//
//
//
//	m_NowCharacterRoomState = CRS_INVENTORY;
//
//
//	m_pDLGMyInfoQuickSlotNumber = new CKTDGUIDialog( pNowStage, L"DLG_Unit_State_QuickSlot_Number.lua", 0.07f, GetQuickSlotNumLayer() );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyInfoQuickSlotNumber );
//
//	m_MyInfoSlotItemReactLeftTop		= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(2).x, m_pDLGMyInfoFront->GetDummyPos(2).y );
//	m_MyInfoSlotItemReactRightBottom	= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(3).x, m_pDLGMyInfoFront->GetDummyPos(3).y );
//
//	m_NowInventorySortType = CX2Inventory::ST_EQUIP;
//
//	//�κ� �ǿ� �°Բ� ������ ����������� ������ �׳� ��~�� �������
//	m_NowInvenSortTypePageNum		= 1;
//	m_NowInvenSortTypePageMaxNum	= 1;
//
////	m_pStat				= new CX2Stat();
////	m_pStatEqip			= new CX2Stat();
//
//	m_GarbageBoxPos		= D3DXVECTOR2( 527, 614 );
//	m_GarbageBoxSize	= D3DXVECTOR2( 90, 105 );
//
//	m_RepairBoxPos		= D3DXVECTOR2( 634, 629 );
//	m_RepairBoxSize		= D3DXVECTOR2( 95, 86 );
//
//	m_vQuickSlotWindowPos	= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(0).x, m_pDLGMyInfoFront->GetDummyPos(0).y );
//	m_vQuickSlotWindowSize	= D3DXVECTOR2( m_pDLGMyInfoFront->GetDummyPos(1).x, m_pDLGMyInfoFront->GetDummyPos(1).y );
//
//	//{{ 2007. 10. 8  ������  ���� ������ �ʱ�ȭ
//	m_pDLGRandomItem = NULL;
//	m_iRandomItemUID = 0;
//	m_RandomItemID = 0;
//	//}}
//
//	m_DecompositionItemUID = 0;
//	m_EnchantItemUID = 0;
//	m_bUseEnchantAdjuvant = false;
//
//	m_SocketItemUID = 0;
//	m_SocketSlotIndex = 0;
//	m_bSocketRareMagicStone = false;
//	m_bSocketRemove = false;
//
//	m_DecompositionWindowNowPageNum = 1;
//	m_mapResolveResultInsertedItem.clear();
//
//	m_TempSortType	= -1;
//	m_TempSlotID	= -1;
//	m_TempItemUID	= -1;
//	m_QuickSellItemUID		= 0;
//
//	m_pDialogToRemoveItem = NULL;
//	m_pDLGToRepair	= NULL;
//
//	m_bPlayGetRandomItem	= false;
//	m_fPlayTime				= 0.0f;
//	m_fPlayMaxTime			= 7.0f;
//
//	m_pDLGGetRandomItemBG	= NULL;
//	m_hMeshInstRandomBox	= INVALID_MESH_INSTANCE_HANDLE;
//	m_hMeshInstRandomBoxKey	= INVALID_MESH_INSTANCE_HANDLE;
//	m_hSeqKeyTrace			= INVALID_PARTICLE_SEQUENCE_HANDLE;
//
//	m_pDLGSelectSellNumWindowPopup	= NULL;
//	m_SellItemUID					= -1;
//	m_SellItemNum					= 1;
//
//	m_ElChangerItemUID				= 0;
//	m_ElChangerItemCount			= 0;
//
//	m_pDLGAllEquippingItemRepair	= NULL;
//
//	m_pDLGResolveItem		= NULL;
//	m_pDLGEnchantItem		= NULL;
//	m_pDLGResolveItemResult = NULL;
//	m_pDLGEnchantItemResult = NULL;
//
//	m_pDLGSocketItem = NULL;
//	m_pDLGSocketItemPushOk = NULL;
//	m_pDLGSocketItemCheat = NULL;
//	m_pDLGCubeCheck	= NULL;
//
//	m_pDLGAttribEnchantItem	= NULL;
//	m_pDLGElChanger			= NULL;
//
//	m_AttribEnchantSlotID	= -1;
//	m_AttribEnchantID		= -1;
//
//	m_pDLGSocketItemGuide = new CKTDGUIDialog( m_pNowState, L"DLG_Socket_Item_Guide.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItemGuide );
//	m_pDLGSocketItemGuide->SetShowEnable( false, false );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGSocketItemGuide, false );
//
//	m_pDLGEnchantItemGuide = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Guide.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItemGuide );
//	m_pDLGEnchantItemGuide->SetShowEnable( false, false );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGEnchantItemGuide, false );
//
//	m_pDLGAttribItemGuide = new CKTDGUIDialog( m_pNowState, L"DLG_Attribute_item_guide.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribItemGuide );
//	m_pDLGAttribItemGuide->SetShowEnable( false, false );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGAttribItemGuide, false );
//
//
//	m_pDLGWarningAllReady	= new CKTDGUIDialog( m_pNowState, L"DLG_Room_Warning_All_Ready.lua", 0.0f );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarningAllReady );
//	m_pDLGWarningAllReady->SetColor( D3DXCOLOR(1,1,1,0) );
//
//	m_TotalExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//	m_TotalED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
//	m_TotalVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
//	m_TotalLevel = g_pData->GetSelectUnitLevel();
//
//	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"MenuInfoPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetOverUI( true );
//	m_pPicCharMenuPlusInfo = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharMenuPlusInfo->SetWidth( 10 );
//
//	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"MenuInfoPicCharRed", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetOverUI( true );
//	m_pPicCharMenuPlusInfoRed = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharMenuPlusInfoRed->SetWidth( 10 );
//
//	ResourcePreLoad();
//
//#ifdef ITEM_RECOVERY_TEST
//	m_RecoveryItemUid = 0;
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//	m_bOpenedAttractionItem	= false;
//	m_bPlayAttractionItem	= false;
//	m_pDLGOpenAttraction	= NULL;
//	m_pDLGGetAttractionItemBG = NULL;
//	m_pDLGAttractionResult	= NULL;
//	m_hMeshInstMeltMachine		= INVALID_MESH_INSTANCE_HANDLE;
//	m_hMeshInstMeltMachineStart = INVALID_MESH_INSTANCE_HANDLE;
//	m_hSeqSpreadLight			= INVALID_PARTICLE_SEQUENCE_HANDLE;
//	m_hSeqEndLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//		
//	m_bUseCoupon	= false;
//	m_CouponTargetUid = 0;
//	m_CouponUid		= 0;
//#endif
//
//
////{{ 2009.01.20 ���¿� : ��ڿ� ���������� �ڵ����� ���
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//	m_bUsingRandomItemAutoOpen	= false;
//	m_OpenedRandomItemCount		= 0;
//	m_pDLGMsgBox				= NULL;
//	m_MapAutoRandomItemGet.clear();
//#endif
//
//	SetOpen(false);
//
//}
//
//CX2CharacterRoom::~CX2CharacterRoom(void)
//{
//	m_pNowState		= NULL;
//	m_pStateMenu	= NULL;
//	
//
//	SAFE_DELETE_DIALOG( m_pDLGMyInfoFront );
//
//	SAFE_DELETE_DIALOG( m_pDLGMyInfoInventory );
//
//	SAFE_DELETE_DIALOG( m_pDLGMyInfoQuickSlotNumber );
//
//	SAFE_DELETE_DIALOG( m_pDLGSkillSlot );
//	SAFE_DELETE_DIALOG( m_pDLGGetRandomItemBG );
//	SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );
//	SAFE_DELETE_DIALOG( m_pDLGResolveItem );
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItem );
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );
//	SAFE_DELETE_DIALOG( m_pDLGSocketItem );
//	SAFE_DELETE_DIALOG( m_pDLGSocketItemCheat );
//
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItemGuide );
//	SAFE_DELETE_DIALOG( m_pDLGSocketItemGuide );
//	SAFE_DELETE_DIALOG( m_pDLGAttribItemGuide );
//
//	SAFE_DELETE_DIALOG( m_pDLGWarningAllReady );
//
//	SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
//
//	SAFE_DELETE_DIALOG( m_pDLGElChanger );
//	
//	if ( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstRandomBox )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstRandomBox );
//	}
//
//	if ( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstRandomBoxKey )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstRandomBoxKey );
//	}
//
//	if ( m_hSeqKeyTrace != INVALID_PARTICLE_SEQUENCE_HANDLE )
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqKeyTrace );
//	}
//
//
//#ifdef ATTRACTION_ITEM_TEST
//	SAFE_DELETE_DIALOG(m_pDLGOpenAttraction);
//	SAFE_DELETE_DIALOG(m_pDLGGetAttractionItemBG);
//	SAFE_DELETE_DIALOG(m_pDLGAttractionResult);
//
//	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachineStart )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
//	}
//
//	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstMeltMachine )
//	{
//		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//	}
//
//	if ( m_hSeqSpreadLight != INVALID_PARTICLE_SEQUENCE_HANDLE )
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
//	}
//
//	if ( m_hSeqEndLight != INVALID_PARTICLE_SEQUENCE_HANDLE )
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight );
//	}
//#endif
//	
//
//	SAFE_DELETE( m_pSkillTreeUI );
//
//	SAFE_DELETE( m_pUnitViewerUI );
//
//
////	SAFE_DELETE( m_pStat );
////	SAFE_DELETE( m_pStatEqip );
//
//	if ( m_pPicCharMenuPlusInfo != NULL )
//		m_pPicCharMenuPlusInfo->Clear();
//	SAFE_DELETE( m_pPicCharMenuPlusInfo );
//	
//	if ( m_pPicCharMenuPlusInfoRed != NULL )
//		m_pPicCharMenuPlusInfoRed->Clear();
//	SAFE_DELETE( m_pPicCharMenuPlusInfoRed );
//
//	ClearDeviceList();
//}
//
//
//
//void CX2CharacterRoom::SetNowStage( CKTDXStage* pNowStage )
//{
//	m_pNowState = (CX2State*)pNowStage;
//	m_pStateMenu = (CX2StateMenu*)pNowStage;
//
//	if ( m_pDLGMyInfoFront != NULL )
//		m_pDLGMyInfoFront->SetStage( m_pNowState );
//
//	if ( m_pDLGMyInfoInventory != NULL )
//		m_pDLGMyInfoInventory->SetStage( m_pNowState );
//
//	if ( m_pDLGItemDesc != NULL )
//		m_pDLGItemDesc->SetStage( m_pNowState );
//	if ( m_pDLGSelectedItem != NULL )
//		m_pDLGSelectedItem->SetStage( m_pNowState );
//
//	if ( m_pDLGMyInfoQuickSlotNumber != NULL )
//		m_pDLGMyInfoQuickSlotNumber->SetStage( m_pNowState );
//
//	if( m_pSkillTreeUI != NULL )
//		m_pSkillTreeUI->SetStage( m_pNowState );
//
//	if( m_pDLGSkillSlot != NULL )
//		m_pDLGSkillSlot->SetStage( m_pNowState );
//}
//
//
//HRESULT CX2CharacterRoom::OnFrameMove( double fTime, float fElapsedTime )
//{
//
//#ifdef TITLE_SYSTEM
//    if( g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetShow() == true )
//    {
//        g_pData->GetTitleManager()->OnFrameMove( fTime, fElapsedTime );
//    }
//#endif
//
//	if ( m_pPicCharMenuPlusInfo != NULL )
//		m_pPicCharMenuPlusInfo->OnFrameMove( fElapsedTime );
//
//	if ( m_pPicCharMenuPlusInfoRed != NULL )
//		m_pPicCharMenuPlusInfoRed->OnFrameMove( fElapsedTime );
//
//	CheckMyInfoChange();
//
//	if ( m_bOpen == false )
//		return S_OK;
///*
//	//�ӽ� �ڵ� �̰� �����ؾ� �ϴµ� ���� ���� ������. ��,�Ѥ�
//	if ( m_pDLGMyInfoQuickSlotNumber != NULL )
//	{
//		if ( m_NowCharacterRoomState == CX2CharacterRoom::CRS_INVENTORY )
//		{
//			m_pDLGMyInfoQuickSlotNumber->SetEnable( !(g_pMain->GetNewQuestUI()->GetOpenQuestPopUpWindow()) );
//			m_pDLGMyInfoQuickSlotNumber->SetShow( !(g_pMain->GetNewQuestUI()->GetOpenQuestPopUpWindow()) );
//		}
//		else if ( m_NowCharacterRoomState == CX2CharacterRoom::CRS_SKILL_TREE )
//		{
//			m_pDLGMyInfoQuickSlotNumber->SetEnable( false );
//			m_pDLGMyInfoQuickSlotNumber->SetShow( false );
//		}
//	}
//*/
//
//	
//	m_pSkillTreeUI->OnFrameMove( fTime, fElapsedTime );
//
//	CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
//
//	UnitHandling( fTime, fElapsedTime );
//
//
//	if ( m_bPlayGetRandomItem == true )
//	{
//		m_fPlayTime += fElapsedTime;
//
//		if( m_hSeqKeyTrace != INVALID_PARTICLE_SEQUENCE_HANDLE && m_hMeshInstRandomBoxKey != NULL )
//		{
//			if( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstRandomBoxKey ) == false )
//			{
//				m_hMeshInstRandomBoxKey = INVALID_MESH_INSTANCE_HANDLE;
//				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqKeyTrace );
//			}
//			else if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqKeyTrace ) == false )
//			{
//				m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//			}
//			else
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
//				if( NULL != pSeq )
//				{
//					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstRandomBoxKey );
//					if( NULL != pMeshInst )
//					{
//						pSeq->SetPosition( pMeshInst->GetPos() );
//					}
//				}
//				else
//				{
//					m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//				}
//			}
//		}
//
//
//		if( m_hMeshInstRandomBox != NULL && g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstRandomBox ) == true )
//		{
//			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstRandomBox );
//
//			if( NULL != pMeshInst &&
//				pMeshInst->EventTimerGlobal( 3.3f ) == true )
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"BoxJewel", 0,0,-100 );
//				pTemp->SetOverUI( true );
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Lobby_Chest01", 0,0,-100 );
//				pTemp->SetOverUI( true );
//			}
//		}
//
//
//		if ( m_fPlayTime >= m_fPlayMaxTime )
//		{
//			m_bPlayGetRandomItem = false;
//			m_fPlayTime = 0.0f;
//
//			if ( m_pDLGGetRandomItemBG != NULL )
//			{
//				m_pDLGGetRandomItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
//				m_pDLGGetRandomItemBG = NULL;
//			}
//
//			if ( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstRandomBox )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstRandomBox );
//			}
//
//			UpdateOpenRandomItemAck();
//			
//		}
//	}
//
//#ifdef ATTRACTION_ITEM_TEST
//	if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE && 
//		g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachine ) == true )
//	{
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachine );
//		if( NULL != pMeshInst &&
//			g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqSpreadLight ) == false )
//		{
//			
//			D3DXVECTOR3 pos = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( L"Object05" );
//			CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_machine01", pos );
//			pTemp->SetOverUI(true);
//			pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			m_hSeqSpreadLight = pTemp->GetHandle();
//		}
//	}
//	else
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
//		m_hSeqSpreadLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//	}
//
//
//
//	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_Start = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstMeltMachineStart );
//
//	if( NULL != pMeshInst_Start &&
//		pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" )
//	{
//		if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqEndLight ) == false )
//		{
//			D3DXVECTOR3 pos = pMeshInst_Start->GetXSkinAnim()->GetCloneFramePosition( L"Sphere01" );
//			pos.z = pos.z - 100;
//			pos.y = pos.y - 10;
//			CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_machine_End02", pos );
//			pTemp->SetOverUI(true);
//			pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			m_hSeqEndLight = pTemp->GetHandle();
//		}
//	}
//	else
//	{
//		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEndLight );
//		m_hSeqEndLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//	}
//
//
//
//	if ( m_bPlayAttractionItem == true )
//	{
//		m_fPlayTime += fElapsedTime;
//
//		if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
//		{
//			if( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == false )
//			{
//				m_hMeshInstMeltMachineStart = INVALID_MESH_INSTANCE_HANDLE;
//				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqSpreadLight );
//			}
//			else if( g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hSeqSpreadLight ) == false )
//			{
//				m_hSeqSpreadLight = INVALID_PARTICLE_SEQUENCE_HANDLE;
//			}
//		}
//
//
//		// ��ƼŬ �ϵ��ڵ�.. �ФФФФФФФФФФФФФФФФФ�
//		if( pMeshInst_Start != NULL )
//		{
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.0f ) == true )
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_Start05", 0,-40,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 3.33f ) == true )
//			{
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_Start04", 0,40,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"Start" && pMeshInst_Start->EventTimerGlobal( 5.53f ) == true )
//			{ 
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_Start03", 0,40,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_Machine_Start02", 0,80,0);
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_Machine_Start01", 0,80,0 );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			if( pMeshInst_Start->GetXSkinAnim()->GetNowAnimName() == L"End" && pMeshInst_Start->EventTimerGlobal( 0.12f ) == true )
//			{
//				D3DXVECTOR3 pos = pMeshInst_Start->GetXSkinAnim()->GetCloneFramePosition( L"Sphere01" );
//				pos.z = pos.z - 100;
//				pos.y = pos.y - 10;
//				CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"LightParticle_Fire_Machine_End01", pos );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//				m_hSeqEndLight = pTemp->GetHandle();
//				pTemp = g_pData->GetUIMajorParticle()->CreateSequence( L"Light_Fire_machine_End02", pos );
//				pTemp->SetOverUI( true );
//				pTemp->SetScaleFactor( D3DXVECTOR3(2, 2, 2) );
//			}
//			
//		}
//
//
//		
//		if ( m_fPlayTime >= m_fPlayMaxTime + 1.5f)
//		{
//			m_bPlayAttractionItem = false;
//			m_fPlayTime = 0.0f;
//
//			if ( m_pDLGGetAttractionItemBG != NULL )
//			{
//				m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
//				m_pDLGGetAttractionItemBG = NULL;
//			}
//
//			if ( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
//			}
//
//			UpdateOpenAttractionItemAck();
//
//		}
//	}
//#endif
//
//	//OnFrameMove_VP_EXP( fTime, fElapsedTime );
//
//	if ( m_pDLGEnchantItemGuide != NULL )
//	{
//		if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_ENCHANT )
//		{
//			m_pDLGEnchantItemGuide->SetShowEnable( true, true );
//		}
//		else
//		{
//			m_pDLGEnchantItemGuide->SetShowEnable( false, false );
//		}
//	}
//
//	if ( m_pDLGSocketItemGuide != NULL )
//	{
//		if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_SOCKET )
//		{
//			m_pDLGSocketItemGuide->SetShowEnable( true, true );
//		}
//		else
//		{
//			m_pDLGSocketItemGuide->SetShowEnable( false, false );
//		}
//	}
//
//	if ( m_pDLGAttribItemGuide != NULL )
//	{
//		if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_ATTRIB_ENCHANT )
//		{
//			m_pDLGAttribItemGuide->SetShowEnable( true, true );
//		}
//		else
//		{
//			m_pDLGAttribItemGuide->SetShowEnable( false, false );
//		}
//	}
//
//	return S_OK;
//}
//
//bool CX2CharacterRoom::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if ( GetCheckOperationCondition() == false )
//		return false;
//
//	switch( uMsg )
//	{
//	case WM_LBUTTONUP:
//		{
//			if ( m_pUnitViewerUI != NULL && CX2UnitViewerUI::GetUnitClicked() == true )
//			{
//				D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
//				int degree = (int)D3DXToDegree( rot.y );
//				m_fRemainRotY = D3DXToRadian( degree % 360 );
//			}
//
//			CX2UnitViewerUI::SetUnitClicked( false );
//
//#ifdef ATTRACTION_ITEM_TEST
//			if ( m_pStateMenu->GetCursor() != NULL )
//			{
//				if ( m_pStateMenu->GetCursor()->GetCurorState() != CX2Cursor::XCS_ENCHANT )
//				{
//					m_bUseCoupon = false;
//					m_CouponUid = 0;
//					m_CouponTargetUid = 0;
//				}
//			}
//#endif
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//			
//			MouseUp( mousePos );
//
//
//		}
//		break;
//
//	case WM_LBUTTONDOWN:
//		{
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//
//
//			if ( MouseDown( mousePos) == true )
//				return true;
//
//			if ( m_bOpen == true && IsInUnitClickReact( mousePos ) == true )
//			{
//				CX2UnitViewerUI::SetUnitClicked( true );
//				return true;
//			}
//		}
//		break;
//
//	case WM_MOUSEMOVE:
//		{
//
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//			m_NowMousePos = mousePos;
//
//			MouseMove( mousePos );
//		}
//		break;
//
//	case WM_RBUTTONDOWN:
//		{
//			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
//			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
//
//			SetNowDragItemReturn();
//
//			if ( m_pStateMenu->GetCursor() != NULL )
//			{
//				if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_NORMAL )
//					MouseRButtonUp( mousePos );
//				else
//				{
//					if ( m_pStateMenu->GetCursor()->GetCurorState() == CX2Cursor::XCS_REMOVE_SKILL )
//					{
//						if( NULL != m_pSkillTreeUI )
//						{
//							m_pSkillTreeUI->UpdateRemoveSkillState( false );
//							if( true == m_pSkillTreeUI->GetShow() )
//							{
//								m_pSkillTreeUI->UpdateSkillTrees();
//							}
//						}
//					}
//#ifdef ATTRACTION_ITEM_TEST
//					m_bUseCoupon = false;
//					m_CouponUid = 0;
//					m_CouponTargetUid = 0;
//#endif
//					m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//
//					return true;
//				}
//			}
//		}
//		break;
//
//	case WM_RBUTTONUP:
//		{
//			SetNowDragItemReturn();
//		}
//		break;
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if ( m_pSkillTreeUI != NULL && /*m_NowCharacterRoomState == CRS_SKILL_TREE &&*/
//		m_pSkillTreeUI->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
//		return true;   
//
//#ifdef TITLE_SYSTEM
//    if( g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetShow() == true )
//    {
//        if( g_pData->GetTitleManager()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
//            return true;
//    }
//#endif
//
//	switch(wParam)
//	{
//	case CRM_QUICK_SELL_ITEM:
//		{
//			return Handler_EGS_SELL_ITEM_REQ();
//		}
//		break;
//
//	case CRM_QUICK_SELL_ITEM_CANCEL:
//		{
//			if ( m_pDialogToRemoveItem != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDialogToRemoveItem, NULL, false );
//
//			m_pDialogToRemoveItem = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_REPAIR_ITEM:
//		{
//			return Handler_EGS_REPAIR_ITEM_REQ();
//		}
//		break;
//
//	case CRM_OPEN_RANDOM_ITEM:
//		{
//#ifdef ATTRACTION_ITEM_TEST
//			m_bOpenedAttractionItem = false;
//#endif
//			return Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//		}
//		break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_1_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//			if( 0 != pUnitData->m_EqipSkill1 )
//			{
//				//CKTDGUIDialog* pDLGSkillSlot = GetDLGSkillSlot();
//				//CKTDGUIButton* pButton = (CKTDGUIButton*) pDLGSkillSlot->GetControl( L"Skill_Slot_1" );
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill1, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_2_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//			if( 0 != pUnitData->m_EqipSkill2 )
//			{
//				//CKTDGUIDialog* pDLGSkillSlot = GetDLGSkillSlot();
//				//CKTDGUIButton* pButton = (CKTDGUIButton*) pDLGSkillSlot->GetControl( L"Skill_Slot_2" );
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill2, vPos, true );
//			}
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_3_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_EqipSkill3 )
//			{
//				//CKTDGUIDialog* pDLGSkillSlot = GetDLGSkillSlot();
//				//CKTDGUIButton* pButton = (CKTDGUIButton*) pDLGSkillSlot->GetControl( L"Skill_Slot_3" );
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill3, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_4_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_Eqip2Skill1 )
//			{
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill1, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_5_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_Eqip2Skill2 )
//			{
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill2, vPos, true );
//			}
//
//		} break;
//
//	case CRM_SKILL_SLOT_UNEQUIP_6_OVER:
//		{
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			if( 0 != pUnitData->m_Eqip2Skill3 )
//			{
//				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
//				D3DXVECTOR2 vPos = pButton->GetPos() + D3DXVECTOR2( 20, -200 );
//				GetSkillTreeUI()->OpenSkillInfoPopup( (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill3, vPos, true );
//			}
//
//		} break;
//
//
//
//	case CRM_SKILL_SLOT_UNEQUIP_OUT:
//		{
//			GetSkillTreeUI()->CloseSkillInfoPopup( true );
//			return true;
//		} break;
//
//	case CRM_ALL_EQUIPPING_ITEM_REPAIR:
//		{
//			OpenDLGAllEquippingItemRepair();
//			return true;
//		}
//		break;
//
//	case CRM_ALL_EQUIPPING_ITEM_REPAIR_OK:
//		{
//			AllEquippingItemRepairREQ();
//			return true;
//		}
//		break;
//
//	case CRM_ALL_EQUIPPING_ITEM_REPAIR_CANCEL:
//		{
//			if ( m_pDLGAllEquippingItemRepair != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAllEquippingItemRepair, NULL, false );
//				m_pDLGAllEquippingItemRepair = NULL;
//			}
//			return true;
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_OK:
//		{
//			if ( m_pDLGResolveItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItem, NULL, false );
//			}
//			m_pDLGResolveItem = NULL;
//
//			return Handler_EGS_RESOLVE_ITEM_REQ();
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_CANCEL:
//		{
//			if ( m_pDLGResolveItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItem, NULL, false );
//			}
//			m_pDLGResolveItem = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_SKILL_JOB_LEVEL_1:
//		{
//			if( NULL != m_pSkillTreeUI )
//			{
//				switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
//				{
//				case CX2Unit::UT_ELSWORD:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ELSWORD_SWORDMAN );
//					} break;
//
//				case CX2Unit::UT_ARME:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ARME_VIOLET_MAGE );
//					} break;
//
//				case CX2Unit::UT_LIRE:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_LIRE_ELVEN_RANGER );
//					} break;
//
//				case CX2Unit::UT_RAVEN:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_RAVEN_FIGHTER );
//					} break;
//				case CX2Unit::UT_EVE:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_EVE_NASOD );
//					} break;
//
//				}
//				m_pSkillTreeUI->OpenSkillTree( true, m_pSkillTreeUI->GetPickedUnitClass() );
//			}
//			return true;
//
//		} break;
//
//	case CRM_SKILL_JOB_LEVEL_2:
//		{
//			// note!! ���� �߰��Ǹ� ����
//			if( NULL != m_pSkillTreeUI )
//			{
//				switch( g_pData->GetMyUser()->GetSelectUnit()->GetClass() )
//				{
//				default:
//					break;
//
//				case CX2Unit::UC_ELSWORD_KNIGHT:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ELSWORD_KNIGHT );
//					} break;
//
//				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT );
//					} break;
//
//				case CX2Unit::UC_LIRE_COMBAT_RANGER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_LIRE_COMBAT_RANGER );
//					} break;
//
//				case CX2Unit::UC_LIRE_SNIPING_RANGER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_LIRE_SNIPING_RANGER );
//					} break;
//
//				case CX2Unit::UC_ARME_HIGH_MAGICIAN:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ARME_HIGH_MAGICIAN );
//					} break;
//
//				case CX2Unit::UC_ARME_DARK_MAGICIAN:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_ARME_DARK_MAGICIAN );
//					} break;
//
//				case CX2Unit::UC_RAVEN_SOUL_TAKER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_RAVEN_SOUL_TAKER );
//					} break;
//				case CX2Unit::UC_RAVEN_OVER_TAKER:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_RAVEN_OVER_TAKER );
//					} break;
//#ifdef EVE_FIRST_CHANGE_JOB
//				case CX2Unit::UC_EVE_EXOTIC_GEAR:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_EVE_EXOTIC_GEAR );
//					} break;
//				case CX2Unit::UC_EVE_ARCHITECTURE:
//					{
//						m_pSkillTreeUI->SetPickedUnitClass( CX2Unit::UC_EVE_ARCHITECTURE );
//					} break;
//#endif
//				}	
//				m_pSkillTreeUI->OpenSkillTree( true, m_pSkillTreeUI->GetPickedUnitClass() );
//			}
//			return true;
//
//		} break;
//
//	case CRM_USE_ENCHANT_ADJUVANT:
//		{
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
//			m_bUseEnchantAdjuvant = pCheckBox->GetChecked();
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
//			ASSERT( NULL != pItem );
//			if( NULL == pItem )
//				return true;
//
//			if( true == m_bUseEnchantAdjuvant )
//			{
//				{
//					if( pItem->GetItemData().m_EnchantLevel >= 10 )
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//							L"��ȭ �ܰ谡 +10 �̻��� �����ۿ��� �÷���� ������ ����� �� �����ϴ�.", m_pNowState );
//						m_bUseEnchantAdjuvant = false;
//						pCheckBox->SetChecked( false );
//					}
//				}
//			}
//
//			if( true == m_bUseEnchantAdjuvant )
//			{
//				ASSERT( NULL != pItem->GetItemTemplet() );
//				if( NULL != pItem->GetItemTemplet() )
//				{
//					int iAdjuvantItemID = GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->m_UseLevel );
//					int iAdjuvantCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iAdjuvantItemID );
//					if( iAdjuvantCount <= 0 )
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"�÷���� ������ �����մϴ�.", m_pNowState );
//						m_bUseEnchantAdjuvant = false;
//						pCheckBox->SetChecked( false );
//					}
//				}
//			}
//			UpdateEnchantWindow();
//
//			return true;
//		} break;
//
//	case CRM_ENCHANT_ITEM_OK:
//		{
//			if ( m_pDLGEnchantItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//			}
//			m_pDLGEnchantItem = NULL;
//
//			return Handler_EGS_ENCHANT_ITEM_REQ( false );	 
//		}
//		break;
//
//	case CRM_SPECIAL_ENCHANT_ITEM_OK:
//		{
//			if ( m_pDLGEnchantItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//			}
//			m_pDLGEnchantItem = NULL;
//
//			return Handler_EGS_ENCHANT_ITEM_REQ( true );
//		}
//		break;
//
//	case CRM_ENCHANT_ITEM_CANCEL:
//		{
//			if ( m_pDLGEnchantItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//			}
//			m_pDLGEnchantItem = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_RESULT_OK:
//		{
//			if ( m_pDLGResolveItemResult != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItemResult, NULL, false );
//			}
//			m_pDLGResolveItemResult = NULL;
//			
//			return true;
//		}
//		break;
//
//	case CRM_ENCHANT_ITEM_RESULT_OK:
//		{
//			if ( m_pDLGEnchantItemResult != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItemResult, NULL, false );
//			}
//			m_pDLGEnchantItemResult = NULL;
//
//			return true;
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_RESULT_PREV_PAGE:
//		{
//			ChangeDecompositionPage( false );
//		}
//		break;
//
//	case CRM_RESOLVE_ITEM_RESULT_NEXT_PAGE:
//		{
//			ChangeDecompositionPage( true );
//		}
//		break;
//
//	case CRM_RESALE_ITEM_NUM_CHECK:
//		{
//			m_sbCheckQuestionSellNum = false;
//		}
//		break;
//	
//	case CRM_RESALE_ITEM_NUM_UNCHECK:
//		{
//
//			m_sbCheckQuestionSellNum = true;
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_CLOSE:
//		{
//			if ( m_pDLGSocketItem != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSocketItem, NULL, false );
//
//			m_pDLGSocketItem = NULL;
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_PUSH_NORMAL_MAGIC_STONE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			wstring controlName = pControl->GetName();
//			WCHAR slotNum = controlName.at( controlName.size() - 1  );
//			controlName = slotNum;
//			int socketIndex = _wtoi( controlName.c_str() );
//			socketIndex--;
//			if ( socketIndex < 0 )
//				socketIndex = 0;
//
//			m_SocketSlotIndex = socketIndex;
//			m_bSocketRareMagicStone = false;
//			m_bSocketRemove = false;
//
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
//			if ( pCheckBox != NULL )
//			{
//				if ( pCheckBox->GetChecked() == true )
//				{
//					OpenSocketCheatWindow();
//				}
//				else
//				{
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
//					if ( pItem != NULL )
//					{
//						wstringstream wstrstm;
//						wstrstm << L"������� [" << g_pMain->GetEDString( pItem->GetEDToSocketPush() ) << L"]ED �Դϴ�.\n�������� ���� �Ͻðڽ��ϱ�?";
//						m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), CRM_SOCKET_ITEM_FEE_CHECK_OK, m_pNowState );
//					}
//				}
//			}
//
//			
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_PUSH_RARE_MAGIC_STONE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			wstring controlName = pControl->GetName();
//			WCHAR slotNum = controlName[ controlName.size() - 1 ];
//			controlName = slotNum;
//			int socketIndex = _wtoi( controlName.c_str() );
//			socketIndex--;
//			if ( socketIndex < 0 )
//				socketIndex = 0;
//
//			m_SocketSlotIndex = socketIndex;
//			m_bSocketRareMagicStone = true;
//			m_bSocketRemove = false;
//
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
//			if ( pCheckBox->GetChecked() == true )
//			{
//				OpenSocketCheatWindow();
//			}
//			else
//			{
//				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
//				if ( pItem != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"������� [" << g_pMain->GetEDString( pItem->GetEDToSocketPush() ) << L"]ED �Դϴ�.\n�������� ���� �Ͻðڽ��ϱ�?";
//					m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), CRM_SOCKET_ITEM_FEE_CHECK_OK, m_pNowState );
//				}
//			}
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_REMOVE_MAGIC_STONE:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			wstring controlName = pControl->GetName();
//			WCHAR slotNum = controlName[ controlName.size() - 1 ];
//			controlName = slotNum;
//			int socketIndex = _wtoi( controlName.c_str() );
//			socketIndex--;
//			if ( socketIndex < 0 )
//				socketIndex = 0;
//			
//
//			m_SocketSlotIndex = socketIndex;
//			m_bSocketRareMagicStone = false;
//			m_bSocketRemove = true;
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID );
//			if ( pItem != NULL )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"������� [" << g_pMain->GetEDString( pItem->GetEDToSocketRemove() ) << L"]ED �Դϴ�.\n�������� �����Ͻðڽ��ϱ�?\n#CFF0000����! ���ŵ� �������� ������ϴ�.#CX";
//				m_pDLGSocketItemPushOk = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), CRM_SOCKET_ITEM_FEE_CHECK_OK, m_pNowState );
//			}
//		}
//		break;
//
//	case CRM_SOCKET_ITEM_FEE_CHECK_OK:
//		{
//			if ( m_pDLGSocketItemPushOk != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSocketItemPushOk, NULL, false );
//
//			m_pDLGSocketItemPushOk = NULL;
//			return Handler_EGS_SOCKET_ITEM_REQ( m_SocketSlotIndex, m_bSocketRareMagicStone, m_bSocketRemove );
//		}
//		break;
//
//	case CRM_CLOSE_CUBE_RESULT:
//		{
//			if ( m_pDLGCubeCheck != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGCubeCheck, NULL, false );
//
//			m_pDLGCubeCheck = NULL;
//		}
//		break;
//
//	case CRM_INVEN_PAGE_PREV:
//		{
//			PrevInvenPage();
//		}
//		break;
//
//	case CRM_INVEN_PAGE_NEXT:
//		{
//			NextInvenPage();
//		}
//		break;
//
//	case CRM_ATTRIB_ENCHANT_ITEM_CANCEL:
//		{
//			if ( m_pDLGAttribEnchantItem != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAttribEnchantItem, NULL, false );
//
//			m_pDLGAttribEnchantItem = NULL;
//		}
//		break;
//
//	case CRM_ATTRIB_ENCHANT_ITEM_OK:
//		{
//			//Send Packet gogo
//			CKTDGUIControl* pControlItem = (CKTDGUIControl*)lParam;
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//			if ( pItem != NULL )
//			{
//				if ( pItem->GetItemData().m_EnchantOption1 == 0 )
//				{
//					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_1;
//					m_AttribEnchantID = pControlItem->GetDummyInt(0);
//
//					
//					wstringstream wstrstm;
//					bool bDual = false;
//					bool bWeapon = false;
//					int eDForEnchant = 0;
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//					if ( pItem != NULL )
//					{
//						if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//						{
//							bWeapon = true;
//						}
//						else
//						{
//							bWeapon = false;
//						}
//
//						if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//						{
//							bDual = false;
//						}
//						else
//						{
//							bDual = true;
//						}
//
//						g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bDual, bWeapon, 
//							pItem->GetItemTemplet()->m_UseLevel, pItem->GetItemTemplet()->m_ItemGrade, eDForEnchant );
//						wstrstm << L"������� [" << eDForEnchant << L"]ED �Դϴ�.\n��æƮ�� �Ͻðڽ��ϱ�?";
//						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrstm.str().c_str(), CRM_ATTRIB_ENCHANT_ITEM_OK_REP, g_pMain->GetNowState() );
//					}
//					
//
//					return true;
//				}
//				else if ( pItem->GetItemData().m_EnchantOption2 == 0 )
//				{
//					m_AttribEnchantSlotID = CX2EnchantItem::ESI_SLOT_2;
//					m_AttribEnchantID = pControlItem->GetDummyInt(0);
//					
//
//					
//					wstringstream wstrstm;
//					bool bDual = false;
//					bool bWeapon = false;
//					int eDForEnchant = 0;
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//					if ( pItem != NULL )
//					{
//						if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//						{
//							bWeapon = true;
//						}
//						else
//						{
//							bWeapon = false;
//						}
//
//						if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//						{
//							bDual = false;
//						}
//						else
//						{
//							bDual = true;
//
//						}
//
//						g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bDual, bWeapon, 
//							pItem->GetItemTemplet()->m_UseLevel, pItem->GetItemTemplet()->m_ItemGrade, eDForEnchant );
//						wstrstm << L"������� [" << eDForEnchant << L"]ED �Դϴ�.\n��æƮ�� �Ͻðڽ��ϱ�?";
//						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrstm.str().c_str(), CRM_ATTRIB_ENCHANT_ITEM_OK_REP, g_pMain->GetNowState() );
//					}
//
//					return true;
//				}	
//			}
//		}
//		break;
//
//	case CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT:
//		{
//			//remove_slot
//			CKTDGUIControl* pControlItem = (CKTDGUIControl*)lParam;
//
//			m_AttribEnchantSlotID = pControlItem->GetDummyInt(0);
//			m_AttribEnchantID = CX2EnchantItem::ET_NONE;
//			//return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( pControlItem->GetDummyInt(0), CX2EnchantItem::ET_NONE, false );
//
//			wstringstream wstrstm;
//			bool bDual = false;
//			bool bWeapon = false;
//			int eDForEnchant = 0;
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID );
//			if ( pItem != NULL )
//			{
//				if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//				{
//					bWeapon = true;
//				}
//				else
//				{
//					bWeapon = false;
//				}
//
//				if ( pItem->GetItemData().m_EnchantOption1 != 0 && pItem->GetItemData().m_EnchantOption2 != 0 )
//				{
//					bDual = true;
//				}
//				else
//				{
//					bDual = false;
//				}
//
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireED( bDual, bWeapon, 
//					pItem->GetItemTemplet()->m_UseLevel, pItem->GetItemTemplet()->m_ItemGrade, eDForEnchant );
//
//				wstrstm << L"������� [" << eDForEnchant << L"]ED �Դϴ�.\n�Ӽ��� ���� �Ͻðڽ��ϱ�?";
//
//				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), wstrstm.str().c_str(), CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT_REP, g_pMain->GetNowState() );
//			}
//
//
//			return true;
//		}
//		break;
//
//	case CRM_EL_CHANGER_OK:
//		{
//			Handler_EGS_IDENTIFY_ITEM_REQ();
//
//			//��Ŷ�� ��������
//			if ( m_pDLGElChanger != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGElChanger, NULL, false );
//
//			m_pDLGElChanger = NULL;
//		}
//		break;
//
//	case CRM_EL_CHANGER_CANCEL:
//		{
//			//�׳� ���η�
//			if ( m_pDLGElChanger != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGElChanger, NULL, false );
//
//			m_pDLGElChanger = NULL;
//		}
//		break;
//
//	case CRM_EL_CHANGER_CHANGE_ITEM_COUNT:
//		{
//			//���� ���� ���� �ɾ������?
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			AdjustElChangerItemCount( pControl->GetName() );
//
//		}
//		break;
//#ifdef TITLE_SYSTEM
//    case CRM_LIST_TITLE:
//        if(g_pData->GetTitleManager() != NULL)
//        {
//            g_pData->GetTitleManager()->OpenTitle();
//        }
//        break;
//    case CRM_CANCELLATION_TITLE:
//        if(g_pData->GetTitleManager() != NULL)
//        {
//            g_pData->GetTitleManager()->DetachTitle();
//        }
//        break;
//#endif
//
//	case CRM_ATTRIB_ENCHANT_ITEM_OK_REP:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( m_AttribEnchantSlotID, m_AttribEnchantID, false );
//		}
//		break;
//			
//	case CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT_REP:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			return Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( m_AttribEnchantSlotID, m_AttribEnchantID, false );
//		}
//		break;
//
//	case CRM_EXCHANGE_OLD_ITEM_OK:
//		{
//			//{{ dmlee 2008.12.2 �� ������ ��ȯ �̺�Ʈ�Ⱓ ����� ��� ����
//			//CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			//if ( pControl != NULL )
//			//	g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//			//
//			//return Handler_EGS_ITEM_EXCHANGE_REQ();
//			return true;
//			//}} dmlee 2008.12.2 �� ������ ��ȯ �̺�Ʈ�Ⱓ ����� ��� ����		
//		} break;
//#ifdef ITEM_RECOVERY_TEST
//	case CRM_ITEM_RECOVERY_OK:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			Handler_EGS_RESTORE_ITEM_REQ( m_RecoveryItemUid );
//			m_RecoveryItemUid = 0;
//
//			return true;
//		} break;
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//	case CRM_ATTRACTION_ITEM_DUMMY:
//		{
//			// ���� : ESC�� ����ϱ� ����. �ƹ��͵� ���� �ʴ´�.
//			return true;
//		} break;
//	case CRM_ATTRACTION_ITEM_OK:
//		{
//			// ���ư��� �߿��� �޽����� ���� ����~
//			if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE &&  
//				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == true )
//				return true;
//
//			m_bOpenedAttractionItem = true;
//
//			return Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//		}
//		break;
//	case CRM_ATTRACTION_ITEM_CANCLE:
//		{
//			// ���ư��� �߿��� �޽����� ���� ����~
//			if( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE &&  
//				g_pData->GetUIMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstMeltMachineStart ) == true )
//				return true;
//
//			if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//			}
//
//			if ( m_pDLGOpenAttraction != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGOpenAttraction, NULL, false );
//				m_pDLGOpenAttraction = NULL;
//			}
//			
//			m_bOpenedAttractionItem = false;
//
//			return true;
//		}
//		break;
//	case CRM_ATTRACTION_ITEM_RESULT_OK:
//		{
//			if ( m_pDLGAttractionResult != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAttractionResult, NULL, false );
//			m_pDLGAttractionResult = NULL;
//
//			return true;
//		}
//		break;
//	case CRM_USE_COUPON_ITEM_OK:
//		{
//			Handler_EGS_ATTACH_ITEM_REQ( m_CouponUid, m_CouponTargetUid );		
//
//			m_bUseCoupon = false;
//			m_CouponUid = 0;
//			m_CouponTargetUid = 0;
//			if(m_pStateMenu->GetCursor() != NULL)
//				m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//
//			return true;
//		}
//		break;
//	case CRM_USE_COUPON_ITEM_CANCLE:
//		{
//			if ( m_pStateMenu->GetCursor() != NULL )
//			{
//				m_bUseCoupon = false;
//				m_CouponUid = 0;
//				m_CouponTargetUid = 0;
//				if(m_pStateMenu->GetCursor() != NULL)
//					m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//			}
//			
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			if ( pControl != NULL )
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)pControl->GetDialog(), NULL, false );
//			return true;
//	
//		}
//		break;
//	case CRM_OPEN_RANDOM_ITEM_CANCLE:
//		{
//			// ����ġ�� ���� ��
//			if ( m_pDLGRandomItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGRandomItem, NULL, false );
//				m_pDLGRandomItem = NULL;
//			}
//
//		}
//		break;
//#endif
//	}
//	return false;
//}
//
//
//
//
//bool CX2CharacterRoom::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	switch ( wParam )
//	{
//
//		case EGS_GET_SKILL_ACK:
//			{
//				return Handler_EGS_GET_SKILL_ACK( hWnd, uMsg, wParam, lParam );
//			} 
//			break;
//
//		case EGS_SELL_ED_ITEM_ACK:
//			{
//				return Handler_EGS_SELL_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_CHANGE_SKILL_SLOT_ACK:
//			{
//				return Handler_EGS_CHANGE_SKILL_SLOT_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_CHANGE_SKILL_SLOT_NOT:
//			{
//				return Handler_EGS_CHANGE_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_REPAIR_ITEM_ACK:
//			{
//				return Handler_EGS_REPAIR_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//			//{{ 2007. 10. 8  ������  ���� ������ ACK
//		case EGS_OPEN_RANDOM_ITEM_ACK:
//			{
//				return Handler_EGS_OPEN_RANDOM_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_RESOLVE_ITEM_ACK:
//			{
//				return Handler_EGS_RESOLVE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//			//}}
//
//		case EGS_ENCHANT_ITEM_ACK:
//			{
//				return Handler_EGS_ENCHANT_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_SOCKET_ITEM_ACK:
//			{
//				return Handler_EGS_SOCKET_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_ATTRIB_ENCHANT_ITEM_ACK:
//			{
//				return Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//		case EGS_IDENTIFY_ITEM_ACK:
//			{
//				return Handler_EGS_IDENTIFY_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			}
//			break;
//
//        case EGS_USE_ITEM_IN_INVENTORY_ACK:
//            {
//                return Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( hWnd, uMsg, wParam, lParam );
//            }
//            break;
//
//		case EGS_ITEM_EXCHANGE_ACK:
//			{
//				return Handler_EGS_ITEM_EXCHANGE_ACK( hWnd, uMsg, wParam, lParam );
//			} break;
//#ifdef ITEM_RECOVERY_TEST
//		case EGS_RESTORE_ITEM_ACK:
//			{
//				return Handler_EGS_RESTORE_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//
//			} break;
//
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//		case EGS_ATTACH_ITEM_ACK:
//			{
//				return Handler_EGS_ATTACH_ITEM_ACK( hWnd, uMsg, wParam, lParam );
//			} break;
//#endif
//	}
//
//	if( NULL != m_pSkillTreeUI )
//	{
//		if( true == m_pSkillTreeUI->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
//		{
//			return true;
//		}
//	}
//
//
//	return false;
//}
//
//bool CX2CharacterRoom::GetCheckOperationCondition()
//{
//	if ( CX2ItemSlotManager::GetCheckOperationCondition() == false )
//		return false;
//
//	if ( m_bOpen == false )
//		return false;
//
//	return true;
//}
//
//void CX2CharacterRoom::SetOpen( bool bOpen ) 
//{ 
//	m_bOpen = bOpen;
//
//	SetShow( bOpen );
//	SetEnable( bOpen );
//
//#ifdef TITLE_SYSTEM
//    if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
//    {
//        g_pData->GetTitleManager()->CloseTitle();
//    }
//#endif
//
//	// note!! character room�� cx2statemenu�� ��ӹ��� Ŭ���������� ���ȴٰ� ����
//	CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
//	if( NULL != pStateMenu )
//	{
//		if( true == bOpen )
//		{
//			pStateMenu->SetShowMouseClickUI( false, CX2StateMenu::MouseClickUI::HR_CHARACTER_ROOM );
//		}
//		else
//		{
//			pStateMenu->SetShowMouseClickUI( true, CX2StateMenu::MouseClickUI::HR_CHARACTER_ROOM );
//		}
//	}
//	
//
//	m_pSkillTreeUI->SetShow( m_bOpen );
//	m_pDLGSkillSlot->SetShow( m_bOpen );
//	m_pDLGSkillSlot->SetEnable( m_bOpen );
//
//	if ( m_pStateMenu->GetCursor() != NULL )
//		m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//	
//	//�ϴ� ������â�� �Ѹ� �� ����â�� ���� ������ ���´ٰ� �����Ѵ�.
//	if ( m_bOpen == true )
//	{
//		ResetStat();
//		ResetNowEquipUI();
//		ResetCharRoomEDnCashUInVP();
//		UpdateLevel();
//
//		CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticID" );
//		pStaticUserInfo->GetString(0)->msg = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
//
//		ResetResurrectionStoneUI();
//
//		
//
//		SetCharacterRoomState( m_NowCharacterRoomState );
//
//		//{{ 2007. 11. 13  ������  �Ⱦ��� �������
//		// ��踦 ���� ���� ��Ŷ
//		//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_MYINFO_NOT );
//		//}}
//
//		g_pKTDXApp->SkipFrame();
//	}
//	else
//	{
//		m_pUnitViewerUI->SetShowObject( m_bOpen );
//
//		m_pDLGMyInfoInventory->SetShow( m_bOpen );
//		m_pDLGMyInfoInventory->SetEnable( m_bOpen );
//
//		m_pDLGMyInfoFront->SetShow( m_bOpen );
//		m_pDLGMyInfoFront->SetEnable( m_bOpen );
//
//
//		m_pDLGMyInfoQuickSlotNumber->SetShow( m_bOpen );
//		m_pDLGMyInfoQuickSlotNumber->SetEnable( m_bOpen );
//
//
//
//		// ������ ���콺 ���� �̹����� ������
//		m_pDLGSelectedItem->SetShow( false ); 
//		InvalidSlotDesc();
//
//		// ��ų ���� �˾� â�� ����
//		if( NULL != m_pSkillTreeUI )
//		{
//			m_pSkillTreeUI->CloseSkillInfoPopup();			
//			m_pSkillTreeUI->CloseSkillInfoPopup( true );
//		}
//
//		SetNowDragItemReturn();
//
//		//{{ 2007. 11. 13  ������  �Ⱦ��� �������
//		// ��踦 ���� ���� ��Ŷ
//		//g_pData->GetServerProtocol()->SendID( EGS_CLOSE_WND_MYINFO_NOT );
//		//}}
//
//		SAFE_DELETE_DIALOG( m_pDLGSelectSellNumWindowPopup );
//		SAFE_DELETE_DIALOG( m_pDLGResolveItem );
//		SAFE_DELETE_DIALOG( m_pDLGEnchantItem );
//		SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );
//		SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );
//		SAFE_DELETE_DIALOG( m_pDLGSocketItem );
//		SAFE_DELETE_DIALOG( m_pDLGSocketItemCheat );
//		SAFE_DELETE_DIALOG( m_pDLGCubeCheck );
//		SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
//		SAFE_DELETE_DIALOG( m_pDLGElChanger );
//
//		if ( m_pDLGEnchantItemGuide != NULL )
//			m_pDLGEnchantItemGuide->SetShowEnable( false, false );
//
//		if ( m_pDLGSocketItemGuide != NULL )
//			m_pDLGSocketItemGuide->SetShowEnable( false, false );
//
//		if ( m_pDLGAttribItemGuide != NULL )
//			m_pDLGAttribItemGuide->SetShowEnable( false, false );
//	}
//}
//
//
//bool CX2CharacterRoom::SetCharacterRoomState( CHARACTER_ROOM_STATE characterRoomState )
//{
//
//	m_pDLGMyInfoInventory->SetShow( false );
//	m_pUnitViewerUI->SetShowObject( false );
//
//	m_pDLGMyInfoInventory->SetEnable( false );
//
//	m_pDLGMyInfoQuickSlotNumber->SetEnable( false );
//	m_pDLGMyInfoQuickSlotNumber->SetShow( false );
//
//	m_pSkillTreeUI->SetShow( false );
//	m_pDLGSkillSlot->SetShowEnable( false, false );
//
//	m_pDLGMyInfoFront->SetShow( false );
//	m_pDLGMyInfoFront->SetEnable( false );
//
//
//	SetEnable( false );
//	SetShow( false );
//
//
//	if ( m_pStateMenu->GetCursor() != NULL )
//		m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
//
//
//	switch ( characterRoomState	)
//	{
//	/*
//	case CRS_MY_STATE:
//		{
//			SetCharacterRoomMyState();
//		}
//		break;
//*/
//	case CRS_INVENTORY:
//		{
//			SetCharacterRoomInventoryState();
//
//#ifdef TITLE_SYSTEM
//            SetTitleName();
//            SetTitleNew(false);
//#endif
//			//{{ 2007. 11. 13  ������  �Ⱦ��� �������
//			// ��踦 ���� ���� ��Ŷ
//			//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_INVENTORY_NOT );
//			//}}
//		}
//		break;
//
//	case CRS_QUEST:
//		{
//			SetCharacterRoomQuestState();
//
//			//{{ 2007. 11. 13  ������  �Ⱦ��� �������
//			// ��踦 ���� ���� ��Ŷ
//			//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_QUEST_NOT );
//			//}}
//		}
//		break;
//		
//	case CRS_SKILL_TREE:
//		{
//			SetCharacterRoomSkillTreeState();
//			
//			//{{ 2007. 11. 13  ������  �Ⱦ��� �������
//			// ��踦 ���� ���� ��Ŷ
//			//g_pData->GetServerProtocol()->SendID( EGS_OPEN_WND_MYINFO_SKILLTREE_NOT );
//			//}}
//		} 
//		break;
//
//	default:
//		{
//			return false;
//		}
//		break;
//	}
//
//	return true;
//}
//
//bool CX2CharacterRoom::ChangeInventoryTab( CX2Inventory::SORT_TYPE sortType )
//{
//	return SetInventorySort( sortType );
//}
//
//bool CX2CharacterRoom::SetInventorySort( CX2Inventory::SORT_TYPE sortType, int pageNum )
//{
//	if ( m_bOpen == false )
//		return false;
//
//	if ( sortType == CX2Inventory::ST_NONE )
//		return false;
//
//	m_NowInventorySortType = sortType;
//
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if ( pInventory == NULL )
//		return false;
//
//	int maxPageNum = pInventory->GetItemMaxNum( m_NowInventorySortType )/MY_INVENTORY_SHOW_MAX_SLOT_NUM;
//	if ( pInventory->GetItemMaxNum( m_NowInventorySortType )%MY_INVENTORY_SHOW_MAX_SLOT_NUM > 0 )
//		maxPageNum++;
//
//	ResetInvenPage( pageNum, maxPageNum );
//
//	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
//		if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
//		{
//			pItemSlot->LostItemUI();
//			pItemSlot->SetShow( false );
//			pItemSlot->SetEnable( false );
//			pItemSlot->SetSortType( sortType );
//		}
//
//	}
//
//	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_MAX_SLOT_NUM), slotIndex = 0; slotID < pInventory->GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; slotID++, slotIndex++ )
//	{
//		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//		{
//			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//			if ( pItemSlot == NULL )
//				continue;
//
//			if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlot->GetSlotID() == slotIndex )
//			{
//				pItemSlot->SetShow( true, false );
//				pItemSlot->SetEnable( true, false );
//				break;
//			}
//		}
//
//
//
//	}
//
//	
//
//	for ( int slotID = ((pageNum-1)*MY_INVENTORY_SHOW_MAX_SLOT_NUM), slotIndex = 0; slotID < pInventory->GetItemMaxNum( m_NowInventorySortType ) && slotIndex < MY_INVENTORY_SHOW_MAX_SLOT_NUM; slotID++, slotIndex++ )
//	{
//		CX2Item* pItem = pInventory->GetItem( m_NowInventorySortType, slotID );
//		if ( pItem == NULL )
//			continue;
//
//
//		for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//		{
//			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//			if ( pItemSlot == NULL )
//				continue;
//
//			if ( pItemSlot->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlot->GetSlotID() == slotIndex )
//			{
//				if ( pItemSlot->IsCreatedItemUI() == true )
//					pItemSlot->ResetItemUI( pItem );
//				else
//					pItemSlot->CreateItemUI( pItem );
//
//				break;
//			}
//		}
//
//
//
//	}
//
//	return true;
//}
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_INVENTORY_SLOT_ACK( KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK& kEvent )
//{
//	UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//
//	m_pStateMenu->OnResetStateAndEquip();
//
//
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ITEM_EXPIRATION_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( kEvent.m_vecItemUID.empty() == true )
//		StateLog( L"ExpiredItemEmpty" );
//
//	if ( kEvent.m_vecItemUID.empty() == false )
//	{
//		wstring tempString;
//
//		for ( int i = 0; i < (int)kEvent.m_vecItemUID.size(); i++ )
//		{
//			UidType itemUID = kEvent.m_vecItemUID[i];
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID );
//
//
//			// ��ų ���� B Ȯ�� �������� ��쿡 UI update
//			if( NULL != pItem && 
//				NULL != pItem->GetItemTemplet() )
//			{
//				const MAGIC_CHANGE_SKILL_SLOT_ITEM_ID = 200880;
//				if( MAGIC_CHANGE_SKILL_SLOT_ITEM_ID == pItem->GetItemTemplet()->m_ItemID )
//				{
//					if( NULL != m_pSkillTreeUI && true == m_pSkillTreeUI->GetShow() )
//					{
//						m_pSkillTreeUI->UpdateSkillSet( CX2SkillTreeUI::SKILL_SET_A );
//					}
//					if( true == m_bOpen )
//					{
//						ResetSkillSlotUI();
//					}
//				}
//			}
//
//
//			if ( pItem != NULL )
//			{
//				tempString += L"[";
//				tempString += pItem->GetFullName();
//				tempString += L"] ";
//			}
//
//			if ( m_pUnitViewerUI != NULL )
//				m_pUnitViewerUI->RemoveEqip( itemUID );
//
//			if ( g_pSquareGame != NULL )
//			{
//				CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
//				if ( pSquareUnit != NULL )
//				{
//					pSquareUnit->GetUnitViewer()->RemoveEqip( itemUID );
//				}
//			}
//
//			if ( g_pX2Room != NULL )
//			{
//				CX2Room::SlotData* pSlotData = g_pX2Room->GetMySlot();
//				if ( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
//				{
//					pSlotData->m_pUnitViewer->RemoveEqip( itemUID );
//				}
//			}
//
//			g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( itemUID );
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( itemUID );		
//		}
//
//		if ( tempString.empty() == false )
//		{
//			tempString += L"�Ⱓ�� ����Ǿ� ��������ϴ�.";
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.c_str(), m_pNowState );
//		}
//
//
//		switch( g_pMain->GetNowStateID() )
//		{
//		case CX2Main::XS_PVP_RESULT:
//		case CX2Main::XS_DUNGEON_RESULT:
//			{
//				// do nothing
//			} break;
//
//		default:
//			{
//				m_pStateMenu->OnResetStateAndEquip(); // fix!! �� �Լ� �� ȣ���ؾ� �ϴ���.
//			}
//			break;
//		}
//		
//
//		// ���� ������Ʈ
//		ResetNowEquipUI();
//
//		// �κ��丮 ������Ʈ
//		SetInventorySort( m_NowInventorySortType );
//
//
//		// ���� ������Ʈ
//		ResetStat();
//	}
//
//	return true;
//
//}
//
//bool CX2CharacterRoom::Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid)
//{
//    KEGS_USE_ITEM_IN_INVENTORY_REQ kPacket;
//    kPacket.m_iItemUID = iUid;
//
//    g_pData->GetServerProtocol()->SendPacket( EGS_USE_ITEM_IN_INVENTORY_REQ, kPacket );
//    g_pMain->AddServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );
//
//    return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//    // ��Ŷ ó��
//    KSerBuffer* pBuff = (KSerBuffer*)lParam;
//    KEGS_USE_ITEM_IN_INVENTORY_ACK kEvent;
//    DeSerialize( pBuff, &kEvent );	
//
//    g_pMain->DeleteServerPacket( EGS_USE_ITEM_IN_INVENTORY_ACK );
//    {
//        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//        {
//            
//            
//            g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//            UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//            ResetCharRoomEDnCashUInVP();
//
//            m_pStateMenu->OnChangedInventory();
//
//            // paul, Can you show me how to use the binding machine?
//            // sure, It's really easy, first you need to make sure the pages are collated and stacked properly like this.
//            // ok
//            // then, after punching the holes, insert the coil binder in to the holes like this.,
//
//            // what number do I press for room-service~?
//
//
//
//            return true;
//        }
//    }
//
//    return false;
//}
//
//void CX2CharacterRoom::SetUnitClicked( bool bClicked )
//{
//	if ( bClicked == false )
//	{
//		if ( CX2UnitViewerUI::GetUnitClicked() == true && m_pUnitViewerUI != NULL )
//		{
//			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
//			int degree = (int)D3DXToDegree( rot.y );
//			m_fRemainRotY = D3DXToRadian( degree % 360 );
//		}
//	}
//
//	CX2UnitViewerUI::SetUnitClicked( bClicked );
//}
//
//void CX2CharacterRoom::SetNowDragItemReturn()
//{
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//		if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( m_DraggingItemUID ) == true )
//				m_pUnitViewerUI->AddEqip( m_DraggingItemUID  );
//
//			ResetStat();
//		}
//
//		m_pSlotBeforeDragging->ResetPos();
//
//		if ( m_pSlotBeforeDragging->GetDialog() != NULL )
//			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//	}
//	
//
//
//	m_pSlotBeforeDragging = NULL;
//	m_DraggingItemUID = -1;
//}
//
//void CX2CharacterRoom::ResourcePreLoad()
//{
//	/*
//	ResourcePreLoad( CX2Inventory::ST_EQUIP );
//	ResourcePreLoad( CX2Inventory::ST_AVARTA );
//	ResourcePreLoad( CX2Inventory::ST_E_EQUIP );
//	*/
//
//	
//	vector< CKTDXDevice* > vecReadyDevice;
//
//	for ( int i = 0; i < CX2Inventory::ST_END; i++ )
//	{
//		ResourcePreLoad( (CX2Inventory::SORT_TYPE)i, vecReadyDevice );
//	}
//
//	ClearDeviceList();
//
//	m_pReadyDeviceList = vecReadyDevice;
//
//	SoundReady( L"Item_Pick_And_Drop.ogg" );
//	SoundReady( L"Throw_Garbage.ogg" );
//	SoundReady( L"Skill_Getting.ogg" );
//	SoundReady( L"Skill_Setting2.ogg" );
//
//	SoundReady( L"LevelUp.ogg" );
//	SoundReady( L"LevelUpBGM.ogg" );
//
//	SoundReady( L"TreasureBox_Open.ogg" );
//
//	SoundReady( L"ItemUpgrade_Success.ogg" );
//	SoundReady( L"ItemUpgrade_Disappear.ogg" );
//	SoundReady( L"ItemUpgrade_DownGrade.ogg" );
//	SoundReady( L"ItemUpgrade_Initial.ogg" );
//	SoundReady( L"ItemUpgrade_NoChange.ogg" );
//
//	XMeshReady( L"Cubic_Key.Y" );
//	XSkinMeshReady( L"Motion_Cubic.x" );
//	TextureReady( L"Motion_Cubic2.tga" );
//	TextureReady( L"Motion_Cubic3.tga" );
//	TextureReady( L"Motion_Cubic4.tga" );
//	TextureReady( L"Motion_Cubic5.tga" );
//	TextureReady( L"Motion_Cubic6.tga" );
//	TextureReady( L"Motion_Cubic7.tga" );
//	TextureReady( L"Motion_Cubic8.tga" );
//	TextureReady( L"Motion_Cubic9.tga" );
//	TextureReady( L"Motion_Cubic10.tga" );
//	TextureReady( L"Motion_Cubic11.tga" );
//
//	TextureReady( L"Arme_Damage_Center.dds" );
//}
//
//
//void CX2CharacterRoom::ClearDeviceList()
//{
//	for( int i = 0; i < (int)m_pReadyDeviceList.size(); i++ )
//	{
//		CKTDXDevice* pDevice = m_pReadyDeviceList[i];
//		SAFE_CLOSE( pDevice );
//	}
//	m_pReadyDeviceList.clear();
//}
//
//
//bool CX2CharacterRoom::IsInEquippedItemSlotReact( D3DXVECTOR2 mousePos )
//{
//	if ( CX2ItemSlotManager::IsInEquippedItemSlotReact( mousePos ) == true )
//		return true;
//
//
//	D3DXVECTOR2 m_ConvertLeftTop		= g_pKTDXApp->ConvertByResolution( m_vQuickSlotWindowPos );
//	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_vQuickSlotWindowSize );
//
//	if ( mousePos.x >= m_ConvertLeftTop.x && mousePos.x <= m_ConvertLeftTop.x + m_ConvertSize.x &&
//		mousePos.y >= m_ConvertLeftTop.y && mousePos.y <= m_ConvertLeftTop.y + m_ConvertSize.y )
//	{
//		return true;
//	}
//
//	return false;
//}
//
//
//
//bool CX2CharacterRoom::Handler_EGS_DELETE_ITEM_REQ()
//{
//	//KEGS_DELETE_ITEM_REQ kPacket;
//	//kPacket.m_cSortType = m_TempSortTypeToRemove;
//	//kPacket.m_sSlotID = m_TempSlotIDToRemove;
//
//	//g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_ITEM_REQ, kPacket );
//	//g_pMain->AddServerPacket( EGS_DELETE_ITEM_ACK );
//
//	return true;
//}
//
//
//
//bool CX2CharacterRoom::Handler_EGS_DELETE_ITEM_ACK( KEGS_DELETE_ITEM_ACK& kEvent )
//{
//	//g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDialogToRemoveItem, NULL, false );
//	//m_pDialogToRemoveItem = NULL;
//
//	//UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//	//	
//	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Throw_Garbage.ogg" );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SELL_ITEM_REQ()
//{
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if( NULL == pInventory )
//		return false;
//
//	CX2Item* pItem = pInventory->GetItem( m_TempItemUID );
//	if( NULL == pItem )
//		return false;
//
//	KEGS_SELL_ED_ITEM_REQ kPacket;
//	kPacket.m_iItemUID = pItem->GetItemData().m_ItemUID;
//	kPacket.m_iQuantity = m_SellItemNum;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_SELL_ED_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_SELL_ED_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SELL_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// ���̾�α� ó��
//	if ( m_pDialogToRemoveItem != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDialogToRemoveItem, NULL, false );
//
//	m_pDialogToRemoveItem = NULL;
//
//	// ��Ŷ ó��
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_SELL_ED_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	g_pMain->DeleteServerPacket( EGS_SELL_ED_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			std::vector< KInventoryItemInfo > vecInventoryItemInfo;
//			vecInventoryItemInfo.push_back( kEvent.m_kInventorySlotInfo );
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( vecInventoryItemInfo );
//			g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED		= kEvent.m_iED;		
//
//			UpdateInventorySlotList( vecInventoryItemInfo );
//
//			ResetCharRoomEDnCashUInVP();
//			
//			m_pStateMenu->OnChangedInventory();
//
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_REPAIR_ITEM_REQ()
//{
//	if ( m_pDLGToRepair != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGToRepair, NULL, false );
//	
//	m_pDLGToRepair = NULL;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)m_TempSortType, m_TempSlotID );
//	if ( pItem != NULL )
//	{
//		int edToRepair = pItem->GetEDToRepair();
//		int vpToRepair = pItem->GetVPToRepair();
//
//
//		float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//		if( fDiscountRate > 0.f )
//		{
//			edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
//		}
//
//
//
//		if ( edToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"ED�� �����Ͽ� ������ �� �����ϴ�!", m_pNowState );
//			return true;
//		}
//		if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"VP�� �����Ͽ� ������ �� �����ϴ�!", m_pNowState );
//			return true;
//		}
//
//		UidType itemUID = pItem->GetUID();
//		KEGS_REPAIR_ITEM_REQ kPacket;
//		kPacket.m_vecItemUID.push_back( itemUID );
//
//		g_pData->GetServerProtocol()->SendPacket( EGS_REPAIR_ITEM_REQ, kPacket );
//		g_pMain->AddServerPacket( EGS_REPAIR_ITEM_ACK );
//	}
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_REPAIR_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_REPAIR_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	// ���̾�α� ó��
//	if ( m_pDLGToRepair != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGToRepair, NULL, false );
//	
//	m_pDLGToRepair = NULL;
//
//	g_pMain->DeleteServerPacket( EGS_REPAIR_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
//			if ( pUnit != NULL )
//			{
//				pUnit->AccessUnitData().m_ED = kEvent.m_iED;
//				pUnit->AccessUnitData().m_VSPoint = kEvent.m_iVP;
//
//				UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//				ResetCharRoomEDnCashUInVP();
//				ResetNowEquipUI();
//				m_pStateMenu->OnResetStateAndEquip();
//
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"���� �Ǿ����ϴ�.", m_pNowState );
//			}	
//		}
//	}
//
//	return true;
//}
//
//
//
//bool CX2CharacterRoom::Handler_EGS_GET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// ��Ŷ ó��
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_GET_SKILL_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	if( NULL == m_pSkillTreeUI )
//		return false;
//
//	CX2SkillTree::SKILL_ID eSkillID = m_pSkillTreeUI->GetGetSkillID();
//
//
//
//	g_pMain->DeleteServerPacket( EGS_GET_SKILL_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			// ���� �� ������ ��� ó��
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			
//			pUnitData->m_GetSkillList.push_back( eSkillID );
//			pUnitData->m_iSPoint = kEvent.m_iSPoint;
//
//
//			// �� �ɸ����� ���, UI ������Ʈ
//			if( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData() == pUnitData )
//			{
//				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Getting.ogg", false, false );
//
//				m_pSkillTreeUI->UpdateInfo();
//				m_pSkillTreeUI->UpdateSkillTrees();
//
//				if( kEvent.m_vecUpdatedItem.size() > 0 )
//				{
//					UpdateInventorySlotList( kEvent.m_vecUpdatedItem, false );
//				}
//
//				CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
//				switch( pSkillTemplet->m_Type )
//				{
//				case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
//				case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
//				case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
//				case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
//					{
//						ResetStat();
//					} break;
//				}
//			}
//
//
//			// ��ų ������ ����ų� ������ ��ų���� ���� ������ ���� ��ų�̸� �ٷ� ����
//			if( NULL != m_pSkillTreeUI )
//			{
//				m_pSkillTreeUI->EquipSkillWhenGetSkill( eSkillID );
//			}
//			
//			return true;
//		}
//	}
//	
//	return false;
//}
////
////bool CX2CharacterRoom::Handler_EGS_GET_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
////{
////	// ��Ŷ ó��
////	KSerBuffer* pBuff = (KSerBuffer*)lParam;
////	KEGS_GET_SKILL_NOT kEvent;
////	DeSerialize( pBuff, &kEvent );	
////
////	// ���� �� ������ ��� ó��
////	CX2Unit::UnitData* pUnitData = NULL;
////	if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM )
////	{
////		CX2StatePVPRoom* pkStatePVPRoom = (CX2StatePVPRoom*) m_pStateMenu;
////		CX2PVPRoom* pkPVPRoom = pkStatePVPRoom->GetCX2PVPRoom();
////		CX2Room::SlotData* pkSlotData = pkPVPRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
////		if ( pkSlotData != NULL )
////			pUnitData = pkSlotData->m_pUnit->GetUnitData();
////	}
////	else if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM )
////	{
////		CX2StateDungeonRoom* pkStateDungeonRoom = (CX2StateDungeonRoom*) m_pStateMenu;
////		CX2DungeonRoom* pkDungeonRoom = pkStateDungeonRoom->GetCX2DungeonRoom();
////		CX2Room::SlotData* pkSlotData = pkDungeonRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
////		if ( pkSlotData != NULL )
////			pUnitData = pkSlotData->m_pUnit->GetUnitData();
////	}
////
////	if ( pUnitData == NULL )
////		return false;
////
////	if( NULL != m_pSkillTreeUI )
////	{
////		pUnitData->m_GetSkillList.push_back( kEvent.m_iSkillID );
////	}
////
////	return true;
////}
////
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_SKILL_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// ��Ŷ ó��
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_SKILL_SLOT_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	g_pMain->DeleteServerPacket( EGS_CHANGE_SKILL_SLOT_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			WCHAR wszText[256] = L"";
//
//
//			// ���� �� ������ ��� ó��
//			CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//			
//			switch( kEvent.m_iSlotID )
//			{
//			case 0:
//				{
//					pUnitData->m_EqipSkill1 = kEvent.m_iSkillID;
//				} break;
//			case 1:
//				{
//					pUnitData->m_EqipSkill2 = kEvent.m_iSkillID;
//				} break;
//			case 2:
//				{
//					pUnitData->m_EqipSkill3 = kEvent.m_iSkillID;
//				} break;
//
//			case 3:
//				{
//					pUnitData->m_Eqip2Skill1 = kEvent.m_iSkillID;
//				} break;
//			case 4:
//				{
//					pUnitData->m_Eqip2Skill2 = kEvent.m_iSkillID;
//				} break;
//			case 5:
//				{
//					pUnitData->m_Eqip2Skill3 = kEvent.m_iSkillID;
//				} break;
//			}
//
//
//			// Ÿ �ɸ����� ���, UI ������Ʈ ��ŵ
//			if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData() != pUnitData )
//				return true;
//
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Setting2.ogg", false, false );
//			ResetSkillSlotUI();
//			m_pSkillTreeUI->UpdateSkillTrees();
//
//			
//			if( 0 == kEvent.m_iSkillID )
//			{
//				if( true == m_pSkillTreeUI->GetRemoveSkillAfterUnEqip() )
//				{
//					m_pSkillTreeUI->SetRemoveSkillAfterUnEqip( false );
//					m_pSkillTreeUI->Handler_EGS_RESET_SKILL_REQ( m_pSkillTreeUI->GetRemoveSkillID() );
//				}
//			}
//					
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	// ��Ŷ ó��
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_SKILL_SLOT_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	WCHAR wszText[256] = L"";
//
//	//CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_Skill_Slot" );
//
//	// ���� �� ������ ��� ó��
//	CX2Unit::UnitData* pUnitData = NULL;
//
//	if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM )
//	{
//		CX2StatePVPRoom* pkStatePVPRoom = (CX2StatePVPRoom*) m_pStateMenu;
//		CX2PVPRoom* pkPVPRoom = pkStatePVPRoom->GetCX2PVPRoom();
//		CX2Room::SlotData* pkSlotData = pkPVPRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
//		if ( pkSlotData != NULL )
//		{
//			pUnitData = pkSlotData->m_pUnit->GetUnitData();
//		}
//	}
//	else if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM ||
//		g_pMain->GetNowStateID() == CX2Main::XS_ARCADE_ROOM )
//	{
//		CX2StateDungeonRoom* pkStateDungeonRoom = (CX2StateDungeonRoom*) m_pStateMenu;
//		CX2DungeonRoom* pkDungeonRoom = pkStateDungeonRoom->GetCX2DungeonRoom();
//		CX2Room::SlotData* pkSlotData = pkDungeonRoom->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
//		if ( pkSlotData != NULL )
//		{
//			pUnitData = pkSlotData->m_pUnit->GetUnitData();
//		}
//	}
//
//	if ( pUnitData == NULL )
//		return false;
//
//	switch( kEvent.m_iSlotID )
//	{
//	case 0:
//		{
//			pUnitData->m_EqipSkill1 = kEvent.m_iSkillID;
//		} break;
//	case 1:
//		{
//			pUnitData->m_EqipSkill2 = kEvent.m_iSkillID;
//		} break;
//	case 2:
//		{
//			pUnitData->m_EqipSkill3 = kEvent.m_iSkillID;
//		} break;
//
//	case 3:
//		{
//			pUnitData->m_Eqip2Skill1 = kEvent.m_iSkillID;
//		} break;
//	case 4:
//		{
//			pUnitData->m_Eqip2Skill2 = kEvent.m_iSkillID;
//		} break;
//	case 5:
//		{
//			pUnitData->m_Eqip2Skill3 = kEvent.m_iSkillID;
//		} break;
//	}
//
//	return true;
//}
//
//void CX2CharacterRoom::PlayGetRandomItem() 
//{
//	if ( m_bPlayGetRandomItem == false )
//	{
//		g_pKTDXApp->GetDeviceManager()->PlaySound( L"TreasureBox_Open.ogg", false, false );
//
//		m_bPlayGetRandomItem = true;
//		m_pDLGGetRandomItemBG = new CKTDGUIDialog( m_pNowState, L"DLG_Unit_State_RandomItem_BG.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetRandomItemBG );
//		m_pDLGGetRandomItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
//		m_pDLGGetRandomItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );
//
//		D3DXVECTOR3 boxPos = m_pDLGGetRandomItemBG->GetDummyPos( 0 );
//
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_RandomBoxKey = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"RewardKey", boxPos.x, 300.0f, boxPos.z - 150.0f , 0,0,0, 0,0,0 );
//		pMeshInst_RandomBoxKey->SetOverUI( true );
//		m_hMeshInstRandomBoxKey = pMeshInst_RandomBoxKey->GetHandle();
//		
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_RandomBox = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"RewardItemBox", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
//		pMeshInst_RandomBox->SetOverUI( true );
//		m_hMeshInstRandomBox = pMeshInst_RandomBox->GetHandle();
//
//
//		//���� ���ڿ� ���� �ؽ��� �ٲ�����
//		if( pMeshInst_RandomBox->GetXSkinMesh()->GetXET() != NULL )
//		{
//			switch( m_RandomItemID )
//			{
//				//����
//				case 105000:
//				case 105010:
//				case 105020:
//				case 105030:
//				case 105040:
//				case 105050:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic.tga" );
//					break;
//
//				//�ü�
//				case 105200:
//				case 105210:
//				case 105220:
//				case 105230:
//				case 105240:
//				case 105250:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic5.tga" );
//					break;
//
//				//����
//				case 105400:
//				case 105410:
//				case 105420:
//				case 105430:
//				case 105440:
//				case 105450:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic2.tga" );
//					break;
//
//				//������
//				case 105600:
//				case 105610:
//				case 105620:
//				case 105630:
//				case 105640:
//				case 105650:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic6.tga" );
//					break;
//
//				//�ü����
//				case 105800:
//				case 105810:
//				case 105820:
//				case 105830:
//				case 105840:
//				case 105850:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic10.tga" );
//					break;
//
//				//������
//				case 106000:
//				case 106010:
//				case 106020:
//				case 106030:
//				case 106040:
//				case 106050:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic7.tga" );
//					break;
//
//				//���ǻ���
//				case 129020:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic4.tga" );
//					break;
//
//				//�Ƹ����� ���� - ��ȫ ����
//				case 129030:
//				case 129040:
//				case 129050:
//				case 129220:
//				case 129260:
//				case 129270:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic3.tga" );
//					break;
//
//				//ȸ������
//				case 108000:
//				case 108010:
//				case 108020:
//				case 108030:
//				case 108040:
//				case 108050:
//				case 108060:
//				case 108070:
//				case 108080:
//				case 108090:
//				case 108100:
//				case 108200:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic11.tga" );
//					break;
//	
//				//��Ȱ�� ���� - �������
//				case 129070:
//				case 129230:
//				case 129240:
//				case 129250:
//				case 129280:
//					pMeshInst_RandomBox->GetXSkinMesh()->GetXET()->AddChangeTexture_LUA( "Motion_Cubic.tga", "Motion_Cubic12.tga" );
//					break;
//
//			}
//		}
//		
//
//		m_hSeqKeyTrace = g_pData->GetUIMajorParticle()->CreateSequenceHandle( L"KeyTrace", pMeshInst_RandomBoxKey->GetPos() );
//		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
//		if( NULL != pSeq )
//		{
//			pSeq->SetOverUI( true );
//		}
//		else
//		{
//			m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//		}
//
//		if ( m_pDLGRandomItem != NULL )
//		{
//			g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGRandomItem, NULL, false );
//			m_pDLGRandomItem = NULL;
//		}
//	}
//}
//
//
////{{ 2007. 10. 8  ������  
//bool CX2CharacterRoom::Handler_EGS_OPEN_RANDOM_ITEM_REQ()
//{
//	if ( m_iRandomItemUID == 0 )
//		return false;
///*
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	for ( int i = CX2Inventory::ST_EQUIP; i <= CX2Inventory::ST_AVARTA; i++ )
//	{
//		if ( pInventory->GetUsedSlotNum( (CX2Inventory::SORT_TYPE)i ) >= pInventory->GetItemMaxNum( (CX2Inventory::SORT_TYPE)i ) )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"ť�긦 ����Ͻ÷��� ��� ����ǰ ī�װ��� ���� ��ĭ �̻��� ������� �־�� �մϴ�.", g_pMain->GetNowState() );
//			return false;
//		}
//	}
//*/	
//
//	KEGS_OPEN_RANDOM_ITEM_REQ kEGS_OPEN_RANDOM_ITEM_REQ;
//
//	kEGS_OPEN_RANDOM_ITEM_REQ.m_iItemUID  = m_iRandomItemUID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_OPEN_RANDOM_ITEM_REQ, kEGS_OPEN_RANDOM_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_OPEN_RANDOM_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_OPEN_RANDOM_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_OPEN_RANDOM_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( m_pDLGRandomItem != NULL )
//	{
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGRandomItem, NULL, false );
//		m_pDLGRandomItem = NULL;
//	}
//
//	g_pMain->DeleteServerPacket( EGS_OPEN_RANDOM_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			m_TempOpenRandomItemAck = kEvent;
////{{ 2009.01.20 ���¿�
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//			if(g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV)
//			{
//				if(m_bUsingRandomItemAutoOpen)	// �ڵ� ������ ������̸�
//				{
//					if(m_pDLGMsgBox == NULL)
//					{
//						m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"��� ��ٷ� �ּ���", m_pNowState );
//					}
//					// ���� �������� �����ϰ� �κ��丮 �������� �� �ٷ� ������ Uid�� ���ؼ� Open�� ��û
//					UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//					
//					for(std::map<int, int>::iterator i = kEvent.m_mapInsertedItem.begin(); i != kEvent.m_mapInsertedItem.end(); i++)
//					{
//						std::map<int, int>::iterator target = m_MapAutoRandomItemGet.find(i->first);
//						if(target != m_MapAutoRandomItemGet.end())
//						{
//							// �̹� ����� ���� �ִ� �������̸� ������ ��������
//							target->second = target->second + i->second;
//						}
//						else
//						{
//							// ���� ���� �������̸� ���� ����
//							m_MapAutoRandomItemGet.insert( std::make_pair(i->first, i->second));
//						}
//					}
//
//					m_OpenedRandomItemCount++;
//					Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//					return true;
//				}
//			}
//#endif
//
//#ifdef ATTRACTION_ITEM_TEST
//			if(m_bOpenedAttractionItem)
//			{
//				// ��ڿ� ��ŵ �ڵ�
//				if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
//				{
//					if ( m_pDLGMyInfoInventory != NULL )
//					{
//						CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//						if ( pCheckBox != NULL )
//						{
//							if ( pCheckBox->GetChecked() == true )
//							{
//								if ( m_pDLGGetAttractionItemBG != NULL )
//								{
//									m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2( 0,0 ), D3DXCOLOR( 0,0,0,0 ), 0.5f, true, true );
//									m_pDLGGetAttractionItemBG = NULL;
//								}
//
//								if ( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//								{
//									g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//								}
//
//
//								if ( m_hMeshInstMeltMachineStart != INVALID_MESH_INSTANCE_HANDLE )
//								{
//									g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachineStart );
//								}
//
//								UpdateOpenAttractionItemAck();
//								return true;								
//							}
//						}
//					}
//				}
//				
//				PlayGetAttractionItem();
//				//UpdateOpenAttractionItemAck();
//			}
//			else
//			{
//				//PlayGetRandomItem();
//				UpdateOpenRandomItemAck();	
//
//			}
//#else
//			//���� ���� �׳� ����~
//
//			//PlayGetRandomItem();
//			UpdateOpenRandomItemAck();	
//#endif
//
//			return true;
//		}
//		else
//		{
////{{ 2009.01.20 ���¿�
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//			if(g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV)
//			{
//				if(m_bUsingRandomItemAutoOpen)	// �ڵ� ������ ������̸�
//				{
//					// �ڵ� ������ �������ϴ�~
//					if(m_pDLGMsgBox != NULL)
//					{
//						SAFE_DELETE_DIALOG(m_pDLGMsgBox);
//						m_pDLGMsgBox = NULL;
//					}
//
//					WCHAR buf[256] = L"";
//					wsprintf( buf, L"���������� %d���� �������ϴ�.", m_OpenedRandomItemCount );
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), buf, m_pNowState );
//
//					UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//					// ���� : OpenDecompositionResultWindow �ȿ��� �κ��丮 ������ �� �ϱ� ���� �� ���� =3=
//					std::vector<KInventoryItemInfo> vecDummyInfo;
//					vecDummyInfo.clear();
//					OpenDecompositionResultWindow(m_MapAutoRandomItemGet, vecDummyInfo, false);
//					// �� �� ������ ġ��ô�
//					m_bUsingRandomItemAutoOpen = false;
//					m_OpenedRandomItemCount = 0;
//					m_MapAutoRandomItemGet.clear();
//				}
//				
//			}
//#endif
//
//#ifdef ATTRACTION_ITEM_TEST
//			// ����-_-�� â�� ������~
//			if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//			{
//				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//			}
//
//			if ( m_pDLGOpenAttraction != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGOpenAttraction, NULL, false );
//				m_pDLGOpenAttraction = NULL;
//			}
//			m_bOpenedAttractionItem = false;
//#endif
//		}
//
//	}
//
//	return false;
//}
////}}
//
//bool CX2CharacterRoom::Handler_EGS_RESOLVE_ITEM_REQ()
//{
//	KEGS_RESOLVE_ITEM_REQ kPacket;
//
//	kPacket.m_iItemUID  = m_DecompositionItemUID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_RESOLVE_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_RESOLVE_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_RESOLVE_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( g_pMain->DeleteServerPacket( EGS_RESOLVE_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			OpenDecompositionResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo, kEvent.m_bJackpot );
//
//			return true;
//		}		
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ENCHANT_ITEM_REQ( bool bSpecialEnchant )
//{
//	KEGS_ENCHANT_ITEM_REQ kPacket;
//
//	kPacket.m_iItemUID				= m_EnchantItemUID;
//	kPacket.m_bIsRareEnchantStone	= false;				// obsolete!! �����ϴ� ����
//	kPacket.m_bIsNewEnchant			= bSpecialEnchant;
//	kPacket.m_bDebug				= false;
//	kPacket.m_bIsSupportMaterial	= m_bUseEnchantAdjuvant;		// ��ȭ ������
//
//
//	if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
//	{
//		if ( m_pDLGMyInfoInventory != NULL )
//		{
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//			if ( pCheckBox != NULL )
//			{
//				if ( pCheckBox->GetChecked() == true )
//				{
//					kPacket.m_bDebug = true;
//				}
//			}
//		}
//	}
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ENCHANT_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_ENCHANT_ITEM_ACK );
//	
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ENCHANT_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( g_pMain->DeleteServerPacket( EGS_ENCHANT_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			OpenEnchantResultWindow( kEvent );
//
//			return true;
//		}		
//	}
//	
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SOCKET_ITEM_REQ( int socketIndex, bool bIsRareMagicStone, bool bIsRemoveSocketOption )
//{
//	KEGS_SOCKET_ITEM_REQ kPacket;
//
//
//	kPacket.m_iItemUID = m_SocketItemUID;
//	kPacket.m_iSocketIndex = socketIndex;
//	kPacket.m_bIsRareMagicStone = bIsRareMagicStone;
//	kPacket.m_bIsRemoveSocketOption = bIsRemoveSocketOption;
//	kPacket.m_bDebug = false;
//	kPacket.m_sDebugSocketOption = 0;
//	
//	if ( bIsRemoveSocketOption == false )
//	{
//		if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
//		{
//			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket");
//			if ( pCheckBox != NULL )
//			{
//				if ( pCheckBox->GetChecked() == true )
//				{
//					kPacket.m_bDebug = true;
//
//					//kPacket.m_sDebugSocketOption = (short)
//					CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*)m_pDLGSocketItemCheat->GetControl(L"EditBoxPassword");
//					if ( pEditBox != NULL )
//					{
//						int socketOption = _wtoi( pEditBox->GetText() );
//						kPacket.m_sDebugSocketOption = (short)socketOption;
//					}
//					
//				}
//			}
//		}
//	}
//
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_SOCKET_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_SOCKET_ITEM_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_SOCKET_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	CloseSocketCheatWindow();
//
//	if ( g_pMain->DeleteServerPacket( EGS_SOCKET_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			vector< short > vecOrgSocketOption;
//			bool bCheckNewElementOption = false;
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iSocketItemUID );
//			if ( pItem != NULL )
//			{
//				vecOrgSocketOption = pItem->GetItemData().m_SocketOption;
//			}
//
//			//Ư�� ó�� ����
//			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//
//			pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEvent.m_iSocketItemUID );
//			if ( pItem != NULL )
//			{
//				vector< short > vecNewSocketOption = pItem->GetItemData().m_SocketOption;
//				for ( int i = 0; i < (int)vecNewSocketOption.size(); i++ )
//				{
//					int newSocketOption = vecNewSocketOption[i];
//					bool bCheck = true;
//					for ( int j = 0; j < (int)vecOrgSocketOption.size(); j++ )
//					{
//						int orgSocketOption = vecOrgSocketOption[j];
//						if ( newSocketOption == orgSocketOption )
//						{
//							bCheck = false;
//						}
//					}
//					if ( bCheck == true )
//					{
//						const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( newSocketOption );
//						if ( pSocketData != NULL )
//						{
//							if ( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType && 
//								pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
//							{
//								bCheckNewElementOption = true;
//							}	
//						}
//					}
//				}
//
//				if ( bCheckNewElementOption == true )
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�Ӽ� ������ �ߺ��Ǿ� ������� �ʽ��ϴ�.", m_pNowState );
//			}
//
//			ResetCharRoomEDnCashUInVP();
//
//			ResetSocketWindow();
//
//			return true;
//		}		
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( int slotNum, int enchantID, bool bDebug )
//{
//	KEGS_ATTRIB_ENCHANT_ITEM_REQ kPacket;
//	kPacket.m_iItemUID = m_AttribEnchantItemUID;
//	kPacket.m_cAttribEnchantSlotNo = slotNum;
//	kPacket.m_cAttribEnchantID = enchantID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ATTRIB_ENCHANT_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK );
//	
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ATTRIB_ENCHANT_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//
//
//	if ( g_pMain->DeleteServerPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			//Ư�� ó�� ����
//			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//
//			ResetCharRoomEDnCashUInVP();
//
//			ResetAtrribEnchantWindow();
//
//		}
//
//		return true;
//	}
//	
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_IDENTIFY_ITEM_REQ()
//{
//	KEGS_IDENTIFY_ITEM_REQ kPacket;
//
//
//	kPacket.m_iItemUID = m_ElChangerItemUID;
//	kPacket.m_iQuantity = m_ElChangerItemCount;
//	
//	g_pData->GetServerProtocol()->SendPacket( EGS_IDENTIFY_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_IDENTIFY_ITEM_ACK );
//	
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_IDENTIFY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_IDENTIFY_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if ( g_pMain->DeleteServerPacket( EGS_IDENTIFY_ITEM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			OpenDecompositionResultWindow( kEvent.m_mapInsertedItem, kEvent.m_vecKInventorySlotInfo, false, true );
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//
//
//
//bool CX2CharacterRoom::Handler_EGS_ITEM_EXCHANGE_REQ()
//{
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if( NULL == pInventory )
//		return false;
//
//	CX2Item* pItem = pInventory->GetItem( m_TempItemUID );
//	if( NULL == pItem )
//		return false;
//
//	KEGS_ITEM_EXCHANGE_REQ kPacket;
//	kPacket.m_iItemUID	= pItem->GetItemData().m_ItemUID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_EXCHANGE_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_ITEM_EXCHANGE_ACK );
//
//	return true;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ITEM_EXCHANGE_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );	
//
//	g_pMain->DeleteServerPacket( EGS_ITEM_EXCHANGE_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
//			if ( pUnit != NULL )
//			{
//				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//				UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//				m_pStateMenu->OnChangedInventory();
//
//				
//				CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEvent.m_kChangedItemInfo.m_iItemID );
//				if( NULL != pItemTemplet )
//				{
//					WCHAR wszMsg[512] = L"";
//					StringCchPrintfW( wszMsg, sizeof(wszMsg), L"+%d %s�� ȹ���߽��ϴ�.", kEvent.m_kChangedItemInfo.m_cEnchantLevel, 
//						pItemTemplet->GetFullName().c_str() );
//
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wszMsg, m_pNowState );
//				}
//			}	
//		}
//	}
//
//	return true;
//}
//
//
//
//bool CX2CharacterRoom::MouseDown( D3DXVECTOR2 mousePos )
//{
//	CX2SlotItem* pSlot = (CX2SlotItem*)GetSlotInMousePos( mousePos );
//
//	if ( pSlot == NULL )
//		return false;
//
//	if ( pSlot->IsResetItemUI() == false )
//	{
//		return false;
//	}
//    
//	CX2Cursor* pCursor = m_pStateMenu->GetCursor();
//	if ( pCursor != NULL )
//	{
//		bool bCheck = false;
//
//		//{{ 2009.01.19 ���¿� : �ڵ�����
//		switch(pCursor->GetCurorState())
//		{
//		case CX2Cursor::XCS_SELL:
//			{
//				bCheck = true;
//				OpenSellWindow( pSlot );
//			}break;
//		case CX2Cursor::XCS_REPAIR:
//			{
//				bCheck = true;
//				OpenRepairWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_DECOMPOSITION:
//			{
//				bCheck = true;
//				OpenDecompositionWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_ENCHANT:
//			{
//#ifdef ATTRACTION_ITEM_TEST
//				if(m_bUseCoupon)
//				{
//					// ó���ϰ�
//
//					if( pSlot->GetSortType() == CX2Inventory::ST_EQUIP )
//					{
//						m_CouponTargetUid = pSlot->GetItemUID();
//						CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_CouponUid );
//						CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_CouponTargetUid );
//
//						if(pCouponItem != NULL || pTargetItem != NULL)
//						{
//
//							wstringstream wstrstm;
//							wstrstm << L"[+" << pTargetItem->GetItemData().m_EnchantLevel << L" " << pTargetItem->GetItemTemplet()->GetFullName().c_str() << L"]��\n";
//							wstrstm << L"[" << pCouponItem->GetItemTemplet()->GetFullName().c_str() << L"] ��������\n";
//							wstrstm << L"��� �Ͻðڽ��ϱ�?";
//
//							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), CRM_USE_COUPON_ITEM_OK, m_pNowState, CRM_USE_COUPON_ITEM_CANCLE );
//
//							bCheck = true;
//						}
//					}
//					else
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �����۸� ��ȭ�� �� �ֽ��ϴ�.", m_pNowState );
//						bCheck = true;
//					}
//				}
//				else
//				{
//					bCheck = true;
//					OpenEnchantWindow( pSlot );
//				}
//#else
//				bCheck = true;
//				OpenEnchantWindow( pSlot );
//#endif				
//			} break;
//		case CX2Cursor::XCS_SOCKET:
//			{
//				bCheck = true;
//				OpenSocketWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_ATTRIB_ENCHANT:
//			{
//				bCheck = true;
//				OpenAtrribEnchantWindow( pSlot );
//			} break;
//		case CX2Cursor::XCS_ITEM_EXCHANGE:
//			{
//				bCheck = true;
//				OpenExchangeOldItemWindow( pSlot );
//			} break;
//		default:
//			{
//
//			} break;
//		}
//		//}} elseif -> switch
//
//		if ( bCheck == true )
//			return true;
//	}
//
//	m_pSlotBeforeDragging = pSlot;
//	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pSlotBeforeDragging->GetDialog(), true );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetDraggingItemLayer() );
//	m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );
//
//	m_DraggingItemUID = pSlot->GetItemUID();
//
//	if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{		
//
//		g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( m_DraggingItemUID );
//		m_pUnitViewerUI->RemoveEqip( m_DraggingItemUID );
//
//
//		ResetStat();
//		ResetRoomStateToInventory( m_DraggingItemUID );
//
//	}
//
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//
//	return true;
//}
//
//
//bool CX2CharacterRoom::MouseUp( D3DXVECTOR2 mousePos )
//{
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//		// fix!! ��ų�� dragging ���̸� 
//		CX2SlotItem* pSlotItem = (CX2SlotItem*) m_pSlotBeforeDragging;
//		if( CX2Inventory::ST_E_SKILL == pSlotItem->GetSortType() ) 
//		{
//			if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
//			{
//				g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetItemLayer() );
//				m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//
//				m_pSlotBeforeDragging->ResetPos();
//			}
//
//			m_pSlotBeforeDragging = NULL;
//			m_DraggingItemUID = -1;
//
//			return true;
//		}
//
//		bool bCheck = false;
//
//		// ���â ������ ���
//		if ( IsInEquippedItemSlotReact( mousePos ) == true  )
//		{
//			bCheck = OnDropItemAnyToEquip( mousePos );
//
//		}
//		// �κ��丮 ������ ���
//		else
//		{
//			bCheck = OnDropItemAnyToInven( mousePos );
//
//		}
//
//		// ���â��, �κ��丮 �������� ó������ �ʾ��� ���
//		if ( bCheck == false )
//		{
//			OnDropItemAnyToOther( mousePos );
//		}
//
//		if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
//		{
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetItemLayer() );
//			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//		}
//
//		m_pSlotBeforeDragging = NULL;
//		m_DraggingItemUID = -1;
//
//		return true;
//	}
//
//
//	if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
//	{
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), GetItemLayer() );
//		m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
//	}
//
//	m_pSlotBeforeDragging = NULL;
//	m_DraggingItemUID = -1;
//
//	return false;
//}
//
//void CX2CharacterRoom::MouseMove( D3DXVECTOR2 mousePos )
//{
//	bool bCheck = false;
//	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//		if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
//			continue;
//
//		if ( pItemSlot->IsInMousePoint( mousePos ) == true )
//		{
//			m_pNowOverItemSlot = pItemSlot;
//			bCheck = true;
//		}
//	}
//
//	if ( bCheck == false )
//		m_pNowOverItemSlot = NULL;
//
//	CX2SlotManager::MouseMove( mousePos );
//}
//
//
//
//bool CX2CharacterRoom::MouseRButtonUp( D3DXVECTOR2 mousePos )
//{
//	m_pSlotBeforeDragging = NULL;
//	m_DraggingItemUID = -1;
//
//	if ( IsInEquippedItemSlotReact( mousePos ) == true )
//	{
//		return OnRClickedItemInEquip( mousePos );
//	}
//	else
//	{
//		return OnRClickedItemInInven( mousePos );
//	}
//
//	return false;
//}
//
//bool CX2CharacterRoom::OnDropItemAnyToEquip( D3DXVECTOR2 mousePos )
//{
//	ResetRoomStateToInventory( m_DraggingItemUID );
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	CX2Item* pItem = pInventory->GetItem( m_DraggingItemUID );
//
//	CX2SlotItem* pItemSlot = NULL;
//	
//	if ( pInventory->GetItem( m_DraggingItemUID ) != NULL )
//	{
//		CX2Item::ITEM_TYPE itemType = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_ItemType;
//		if (  itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY ||
//			itemType == CX2Item::IT_SKILL )
//		{
//			CX2Unit::EQIP_POSITION equipPosition = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_EqipPosition;
//
//			for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//			{
//				CX2SlotItem* pTempItemSlot = (CX2SlotItem*)GetSlot(i);
//				if ( pTempItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pTempItemSlot->GetEquipPos() == equipPosition && 
//					pTempItemSlot->GetFashion() == pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_bFashion )
//				{
//					pItemSlot = pTempItemSlot;
//					break;
//				}
//			}
//		}
//	}
//	
//
//	if ( pItemSlot == NULL )
//		pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//
////	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
//		return false;
//
//	if ( pItemSlot->GetEquipPos() == pItem->GetItemTemplet()->m_EqipPosition &&
//		pItemSlot->GetFashion() == pItem->GetItemTemplet()->m_bFashion )
//	{
//		if ( pItemSlot == m_pSlotBeforeDragging )
//		{
//			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( m_DraggingItemUID ) == true )
//				m_pUnitViewerUI->AddEqip( m_DraggingItemUID );
//
//			m_pSlotBeforeDragging->ResetPos();
//
//			ResetStat();
//
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//		}	
//		else
//		{
//#ifdef ITEM_RECOVERY_TEST
//			if( true == pItem->IsDisabled() )
//			{
//				return false;
//			}
//#endif
//			if ( pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_ENDURANCE && pItem->GetItemData().m_Endurance <= 0 )
//			{
//				return false;
//			}
//			else
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ(
//					((CX2SlotItem*)m_pSlotBeforeDragging),
//					pItemSlot );
//
//				CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//				pkSlotBeforeDragging->DestroyItemUI();
//			}
//		}
//
//		return true;
//	}	
//	
//	return false;
//}
//
//bool CX2CharacterRoom::OnDropItemAnyToInven( D3DXVECTOR2 mousePos )
//{
//	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
//		return false;
//
//	if ( pItemSlot == m_pSlotBeforeDragging )
//	{
//		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
//		return false;
//	}
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	CX2Item* pItem = pInventory->GetItem( m_DraggingItemUID );
//
//	// �ش� Ÿ���� ī�װ����� üũ
//	if ( m_NowInventorySortType == pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) )
//	{
//		// ���â���� �Ѿ�� ���
//		if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			// ����� �� ������ ���
//			if ( pItemSlot->IsResetItemUI() == false )
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)m_pSlotBeforeDragging),
//					pItemSlot );
//
//				CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//				pkSlotBeforeDragging->DestroyItemUI();
//				return true;
//			}
//			// ����� �̹� �ִ� ���, ����մ� ������ �˻�
//			else
//			{
//				for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//				{
//					CX2SlotItem* pItemSlotToPush = (CX2SlotItem*)GetSlot(j);
//					if ( pItemSlotToPush->GetSlotType() == CX2Slot::ST_INVENTORY && pItemSlotToPush->GetDialog() == NULL )
//					{
//						Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)m_pSlotBeforeDragging), 
//							pItemSlotToPush );
//
//						CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//						pkSlotBeforeDragging->DestroyItemUI();
//						return true;
//					}
//				}
//			}
//		}
//		// �κ��丮���� ��ȯ�� ���
//		else
//		{
//			Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( ((CX2SlotItem*)m_pSlotBeforeDragging), 
//				pItemSlot );
//
//			CX2SlotItem* pkSlotBeforeDragging = (CX2SlotItem*) m_pSlotBeforeDragging;
//			pkSlotBeforeDragging->DestroyItemUI();
//			return true;
//		}
//	}
//	else
//	{
//		//�߸��� Ÿ���� �κ��丮�� ���°��
//		return false;
//	}
//	
//	return false;
//}
//
//void CX2CharacterRoom::OnDropItemAnyToOther( D3DXVECTOR2 mousePos )
//{
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//
//
//		if ( m_pSlotBeforeDragging->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( m_DraggingItemUID ) == true )
//				m_pUnitViewerUI->AddEqip( m_DraggingItemUID  );
//
//			ResetStat();
//		}
//
//		m_pSlotBeforeDragging->ResetPos();
//	}
//}
//
//bool CX2CharacterRoom::OnRClickedItemInEquip( D3DXVECTOR2 mousePos )
//{
//	// ��� �������� ��� ó�� : ���� ����
//	CX2SlotItem* pItemSlot = NULL;
//	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL )
//		return false;
//
//	if ( pItemSlot->GetSlotType() != CX2SlotManager::CX2Slot::ST_EQUIPPED )
//		return false;
//
//	CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
//	if ( pkItemTemplet == NULL )
//		return false;
//
//	ResetRoomStateToInventory( pItemSlot->GetItemUID() );
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	if ( pInventory != NULL )
//	{
//		for ( int i = 0; i < pInventory->GetItemMaxNum( m_NowInventorySortType ); i++ )
//		{
//			CX2Item* pItem = pInventory->GetItem( m_NowInventorySortType, i );
//			if ( pItem == NULL )
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( pItemSlot->GetSortType(), pItemSlot->GetSlotID(),
//													m_NowInventorySortType, i );
//
//				pItemSlot->DestroyItemUI();
//				return true;
//			}
//		}
//	}
//	
//
//
//	return false;
//}
//
//bool CX2CharacterRoom::OnRClickedItemInInven( D3DXVECTOR2 mousePos )
//{
//	// ���콺���� ��ġ�� �κ� ���� ���
//	CX2SlotItem* pItemSlot = NULL;
//	pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
//	if ( pItemSlot == NULL )
//		return false;
//
//	if ( pItemSlot->GetSlotType() != CX2SlotManager::CX2Slot::ST_INVENTORY || pItemSlot->IsResetItemUI() == false )
//		return false;
//
//	CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlot->GetItemTID() );
//	if ( pkItemTemplet == NULL )
//		return false;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
//
//#ifdef ITEM_RECOVERY_TEST
//	// ���Ҵ��ΰ���
//	if( pItem != NULL && true == pItem->IsDisabled() )
//	{
//		// 1. ���� �������� �ִ��� Ȯ���Ѵ�.
//		int RecoveryItemID = GetRequiredRecoveryStoneID( pkItemTemplet->m_UseLevel );
//		int iRecoveryItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RecoveryItemID );
//
//		if( iRecoveryItemCount > 0 )
//		{
//			m_RecoveryItemUid = pItem->GetUID();
//			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), L"[�ູ ���� ������ �ֹ���] �������� ����ؼ� \n�������� �����Ͻðڽ��ϱ�?", CRM_ITEM_RECOVERY_OK, m_pNowState );
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �Ұ� ������ �������Դϴ�.\n[�ູ ���� ������ �ֹ���] �������� ����ؼ�\n��� �Ұ� ���¸� �����·� ������ �� �ֽ��ϴ�.", m_pNowState );
//		}
//		return true; 
//	}
//#endif
//    
//	// �ش��ϴ� ������ �˻�, ���â���� �̵�
//	CX2SlotItem* pItemSlotEquip = NULL;
//	for ( unsigned int iS = 0; iS < m_SlotList.size(); ++iS )
//	{
//		CX2SlotItem* pItemSlotEquip = (CX2SlotItem*) m_SlotList[iS];
//		if ( pItemSlotEquip->GetSlotType() != CX2SlotManager::CX2Slot::ST_EQUIPPED )
//			continue;
//
//		
//		/*
//		else
//		{
//			CX2Item* pkEquipItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlotEquip->GetItemUID() );
//			CX2Item::ItemTemplet* pkEquipItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pItemSlotEquip->GetItemTID() );
//			if ( pkEquipItem != NULL && pkEquipItemTemplet != NULL )
//			{
//				if ( pkEquipItemTemplet->m_Quantity <= pkEquipItem->GetItemData().m_Quantity )
//					continue;
//			}
//		}
//		*/
//
//
//
//		// �������� �̸�
//		if ( pkItemTemplet->m_ItemType == CX2Item::IT_WEAPON || 
//			pkItemTemplet->m_ItemType == CX2Item::IT_DEFENCE || 
//			pkItemTemplet->m_ItemType == CX2Item::IT_ACCESSORY )
//		{
//			if( false == CX2Unit::CanEquipAsParts( pkItemTemplet->m_ItemID, g_pData->GetMyUser()->GetSelectUnit() ) ) // fix!! ���� üũ�� �� ������?
//			{
//				continue;
//			}
//
//			//��� ������ ������ �� ������� �Žñ� �غ���
//			//�巡���ؼ� �Žñ� �ϴºκе� �ذ��غ���
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
//			if ( pItem != NULL )
//			{
//				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE )
//				{
//					if ( pItem->GetItemData().m_Endurance <= 0 )
//					{
//						continue;
//					}
//				}
//			}
//			
//			if ( pItemSlotEquip->GetEquipPos() == pkItemTemplet->m_EqipPosition &&
//				pItemSlotEquip->GetFashion() == pkItemTemplet->m_bFashion )
//			{
//				Handler_EGS_CHANGE_INVENTORY_SLOT_REQ(
//					pItemSlot, 
//					pItemSlotEquip );
//
//			pItemSlot->DestroyItemUI();
//				return true;
//			}
//		}
//		else if ( pkItemTemplet->m_ItemType == CX2Item::IT_QICK_SLOT )
//		{
//			if ( pItemSlotEquip->GetEquipPos() == CX2Unit::EP_QUICK_SLOT )
//			{
//				if ( pItemSlotEquip->IsResetItemUI() == false )
//				{
//					Handler_EGS_CHANGE_INVENTORY_SLOT_REQ(
//						pItemSlot, 
//						pItemSlotEquip );
//
//					pItemSlot->DestroyItemUI();
//					return true;
//				}
//			}
//		}
//
//		
//	}
//	
//    // �κ��� ��밡�� �����ۿ� ���� ó��
//    bool bCanUse = false;
//    CX2Item *pUseItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
//    if(pUseItem != NULL)
//    {
//        bCanUse = pUseItem->GetItemTemplet()->m_bCanUseInventory;
//        if(bCanUse == true)
//        {
//            //m_iItemUID
//            if(CheckUseItem(pUseItem) == true)
//            {
//                Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(pItemSlot->GetItemUID());
//                return true;
//            }
//        }
//    }
//
//	//{{ 2007. 10. 8  ������  �κ����� �������� ������ Ŭ������ ��� [�ӽ� �׽�Ʈ]
//	if ( pItem != NULL )
//	{
//		const CX2Item::ItemData& kItemData = &pItem->GetItemData();
//		{
//			CX2ItemManager::RandomItemData* pRandomItem = g_pData->GetItemManager()->GetRandomItemData( kItemData.m_ItemID );
//			if( pRandomItem != NULL )
//			{		
//				// ���� �������� ���~
////{{ 2009.01.20 ���¿�
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//				if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
//				{
//					if ( m_pDLGMyInfoInventory != NULL )
//					{
//						CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGMyInfoInventory->GetControl( L"g_pCheckBoxForceEnchant" );
//						if ( pCheckBox != NULL )
//						{
//							if ( pCheckBox->GetChecked() == true )
//							{
//								m_bUsingRandomItemAutoOpen = true;
//							}
//						}
//					}
//				}
//#endif
////}}
//
//				m_RandomItemID = kItemData.m_ItemID;
//				m_iRandomItemUID = pItemSlot->GetItemUID();
//
//				WCHAR buf[256] = L"";
//
//				int keyItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( pRandomItem->m_KeyItemID );
//
//				if ( keyItemNum < pRandomItem->m_RequiredKeyCount )
//				{
//					wstring itemName = L"";
//					CX2Item::ItemTemplet* pItemTemplet;
//					pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pRandomItem->m_KeyItemID );
//					itemName = pItemTemplet->GetFullName();
//					wsprintf( buf, L"[%s] %d���� �ʿ��մϴ�.", itemName.c_str(), pRandomItem->m_RequiredKeyCount );
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), buf, m_pNowState );
//					return false;
//				}
//
//#ifdef ATTRACTION_ITEM_TEST
//
//				int MAGIC_ATTRACTION_ITEM_ID = 108900;
//				if( MAGIC_ATTRACTION_ITEM_ID == kItemData.m_ItemID )
//				{
//					// ���� �߰�...
//					m_pDLGOpenAttraction = new CKTDGUIDialog( m_pNowState, L"DLG_Ice_Heater_Use.lua" );
//					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOpenAttraction );
//
//					// �߰��� �� �׷�����
//					D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
//					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachine = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"FireMachineWait", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
//					pMeshInst_MeltMachine->SetOverUI(true);
//					m_hMeshInstMeltMachine = pMeshInst_MeltMachine->GetHandle();
//				}
//				else
//				{
//					m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), L"�������� ����ϰڽ��ϱ�?", CX2CharacterRoom::CRM_OPEN_RANDOM_ITEM, m_pNowState, CRM_OPEN_RANDOM_ITEM_CANCLE );
//				}
//#else
//				m_pDLGRandomItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), L"�������� ����ϰڽ��ϱ�?", CX2CharacterRoom::CRM_OPEN_RANDOM_ITEM, m_pNowState );
//#endif
//			}
//			else // ���� ť�갡 �ƴϸ�
//			{
//#ifdef ATTRACTION_ITEM_TEST
//				if( IsEnchantCoupon(kItemData.m_ItemID))
//				{
//					// ��ȭ�� ���� ��
//					if(m_pStateMenu->GetCursor() != NULL)
//						m_pStateMenu->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ENCHANT );
//					m_bUseCoupon = true;
//					m_CouponUid = pItemSlot->GetItemUID();
//                    
//
//					// �� �ٲ�����..
//					ChangeInventoryTab( CX2Inventory::ST_EQUIP );
//
//					wstring radioButtonName = GetRadioButtonNameByInvenSortType( CX2Inventory::ST_EQUIP );
//					CKTDGUIRadioButton* pRadioButtonInvenSortType = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( radioButtonName.c_str() );
//					if ( pRadioButtonInvenSortType != NULL )
//					{
//						pRadioButtonInvenSortType->GetCheckedControl()->SetChecked( false );
//						pRadioButtonInvenSortType->SetChecked( true );
//					}
//
//
//				}
//#endif
//				const int MAGIC_RESET_ALL_SKILL_ITEM_ID = 203800;	
//				//{{ 2009.01.19 ���¿� : �ڵ�����
//				switch(kItemData.m_ItemID)
//				{
//				case MAGIC_RESET_ALL_SKILL_ITEM_ID:
//					{
//						if( NULL != m_pSkillTreeUI )
//						{
//							m_pSkillTreeUI->OpenResetAllSkillPopup( true );
//						}
//					} break;
//				case CX2EnchantItem::ATI_RED:
//				case CX2EnchantItem::ATI_BLUE:
//				case CX2EnchantItem::ATI_GREEN:
//				case CX2EnchantItem::ATI_WIND:
//				case CX2EnchantItem::ATI_LIGHT:
//				case CX2EnchantItem::ATI_DARK:
//					{
//						//�Žñ� �ִ��� Ȯ���غ��� ..ȯ����
//						if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE ) > 0 )
//						{
//							OpenElChangerWindow( false, pItem );
//						}
//						else
//						{
//							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"�� �Ӽ� ȯ���Ⱑ �����ϴ�!", g_pMain->GetNowState() );
//						}
//					} break;
//				case CX2EnchantItem::ATI_UNKNOWN:
//					{
//						//�Žñ� �ִ��� Ȯ���غ���. ������
//						if ( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE ) > 0 )
//						{
//							OpenElChangerWindow( true, pItem );
//						}
//						else
//						{
//							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"�� �Ӽ� �Ǻ��Ⱑ �����ϴ�!", g_pMain->GetNowState() );
//						}
//					} break;
//				default:
//					break;
//				}
//				//}} elseif -> switch
//
//			}
//		}
//	}
//	//}}
//
//	return false;
//}
//
//void CX2CharacterRoom::DrawSlotMouseOverImage()
//{
//	if ( GetCheckOperationCondition() == false )
//	{
//		m_pDLGSelectedItem->SetShow( false );
//		return;
//	}
//
//	bool bCheck = false;
//
//	if ( m_pSlotBeforeDragging != NULL )
//	{
//		if ( IsInEquippedItemSlotReact( m_NowMousePos ) == true )
//		{
//			CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//			CX2Item::ITEM_TYPE itemType = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_ItemType;
//			if (  itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY ||
//				itemType == CX2Item::IT_SKILL )
//			{
//				CX2Unit::EQIP_POSITION equipPosition = pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_EqipPosition;
//
//				for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//				{
//					CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//					if ( pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pItemSlot->GetEquipPos() == equipPosition && 
//						pItemSlot->GetFashion() == pInventory->GetItem( m_DraggingItemUID )->GetItemTemplet()->m_bFashion )
//					{
//						m_pDLGSelectedItem->SetPos( pItemSlot->GetPos() );
//						m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( pItemSlot->GetSize() );
//
//						bCheck = true;
//					}
//				}
//			}
//			else
//			{
//				if ( m_pNowOverItemSlot != NULL )
//				{
//					m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//					m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//					bCheck = true;
//				}
//			}
//		}
//		else
//		{
//			if ( m_pNowOverItemSlot != NULL )
//			{
//				m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//				m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//				bCheck = true;
//			}
//		}
//	}
//	else
//	{
//		if ( m_pNowOverItemSlot != NULL )
//		{
//			m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
//			m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );
//
//			bCheck = true;
//		}
//	}
//
//	if ( bCheck == true )
//	{
//		m_pDLGSelectedItem->SetShow( true );
//	}
//	else
//	{
//		m_pDLGSelectedItem->SetShow( false );
//	}
//
//}
//
//wstring CX2CharacterRoom::GetSlotItemDesc()
//{
//
//	wstring itemDesc=  L"";
//
//	if ( m_pNowOverItemSlot != NULL )
//	{
//		if ( m_pNowOverItemSlot->IsResetItemUI() == false )
//		{
//			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
//		}
//		else
//		{	
//			itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
//		}
//	}
//
//	return itemDesc;
//
//
//	
//}
//
//
//void CX2CharacterRoom::UnitHandling( double fTime, float fElapsedTime )
//{
//	if( m_pUnitViewerUI != NULL )
//	{
//		float fRotSpeed = 13.0f;
//
//
//		if ( CX2UnitViewerUI::GetUnitClicked() == true )
//		{
//			D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
//			m_pUnitViewerUI->GetMatrix().RotateRel(RotateY);
//			m_bRotateReturn = false;
//
//			fRotSpeed = 13.0f;
//		}
//		else
//		{
//			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
//
//
//
//			m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//			int addSpeed = 20;
//
//			if ( m_bRotateReturn == false )
//			{
//				fRotSpeed -= (fElapsedTime * addSpeed);
//
//				if ( fRotSpeed <= 0 )
//				{
//					fRotSpeed = fElapsedTime;
//				}
//
//				if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
//				{
//
//					if ( m_fRemainRotY <= 0.0f )
//					{
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						m_fRemainRotY = 0.0f;
//						m_bRotateReturn = true;
//					}
//					else
//					{
//						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY <= 0.0f )
//						{
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//							m_fRemainRotY = 0.0f;
//							m_bRotateReturn = true;
//						}
//					}
//				}
//				else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
//				{
//					if ( m_fRemainRotY >= 6.18f )
//					{
//						m_bRotateReturn = true;
//						m_fRemainRotY = 0.0f;
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//					}
//					else
//					{
//						m_fRemainRotY += (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY >= 6.18f )
//						{
//							m_bRotateReturn = true;
//							m_fRemainRotY = 0.0f;
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						}
//					}
//				}
//				else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
//				{
//					if ( m_fRemainRotY >= 0.0f )
//					{
//						m_bRotateReturn = true;
//						m_fRemainRotY = 0.0f;
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//					}
//					else
//					{
//						m_fRemainRotY += (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY >= 0.0f )
//						{
//							m_bRotateReturn = true;
//							m_fRemainRotY = 0.0f;
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						}
//					}
//				}
//				else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
//				{
//					if ( m_fRemainRotY <= -6.28f )
//					{
//						m_bRotateReturn = true;
//						m_fRemainRotY = 0.0f;
//						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//					}
//					else
//					{
//						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
//						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));
//
//						if ( m_fRemainRotY <= -6.28f )
//						{
//							m_bRotateReturn = true;
//							m_fRemainRotY = 0.0f;
//							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
//						}
//					}
//
//				}
//			}
//		}
//
//		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );
//
//		if ( m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
//		{
//			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
//		}
//	}
//}
//
//void CX2CharacterRoom::ResetUnitViewerUI()
//{
//	SAFE_DELETE( m_pUnitViewerUI );
//
//	CX2UnitViewerUI::SetUnitClicked( false );
//	m_fMouseSensitivity = 0.01f;
//	m_fRemainRotY = 0.0f;
//	m_bRotateReturn = true;
//
//	m_pUnitViewerUI = new CX2UnitViewerUI();
//	//m_pUnitViewerUI->SetLightPos( 1000, 1000, -200 );
//	m_pUnitViewerUI->SetLightPos( 300, 300, -500 );	// ĳ���ͺ�� ����Ʈ ��ġ ����
//	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );
//
//	m_pUnitViewerUI->SetUnit( g_pData->GetMyUser()->GetSelectUnit() );
//	m_pUnitViewerUI->GetMatrix().Move(  D3DXVECTOR3( -250, -60, 200 ) );
//	m_pUnitViewerUI->GetMatrix().Scale( D3DXVECTOR3( 1.5f, 1.5f, 1.5f ) );
//	m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
//	m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
//
//	m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
//
//}
//
//void CX2CharacterRoom::PlayWarningAllReady()
//{
//	if ( m_pDLGWarningAllReady != NULL )
//	{
//		if ( m_pDLGWarningAllReady->GetIsMoving() == false )
//		{
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGWarningAllReady, true );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,1), 2.0f );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,0), 1.0f );
//			m_pDLGWarningAllReady->Move( m_pDLGWarningAllReady->GetPos(), D3DXCOLOR(1,1,1,0), 4.0f );
//		}
//	}
//}
//
//void CX2CharacterRoom::StopWarningAllReady()
//{
//	if ( m_pDLGWarningAllReady != NULL )
//	{
//		m_pDLGWarningAllReady->MoveStop();
//		m_pDLGWarningAllReady->SetColor( D3DXCOLOR( 1,1,1,0 ) );
//	}
//}
//
//
//
//
//void CX2CharacterRoom::ResourcePreLoad( CX2Inventory::SORT_TYPE sortType, vector< CKTDXDevice* >& vecReadyDevice )
//{
//	return;
//
//	// obsolete!!
//
//	//if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
//	//	return;
//
//	//CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//	//if ( pInventory == NULL )
//	//	return;
//
//	//for ( int slotID = 0; slotID < pInventory->GetItemMaxNum( sortType ); slotID++ )
//	//{
//	//	CX2Item* pItem = pInventory->GetItem( sortType, slotID );
//	//	if ( pItem == NULL )
//	//		continue;
//
//	//	wstring tempModelName = pItem->GetItemTemplet()->m_ModelName;
//	//	wstring tempTextureName = pItem->GetItemTemplet()->m_ShopImage;
//
//	//	if ( g_pData->GetMyUser()->GetSelectUnit()->EqipAbility( pItem ) == true )
//	//	{
//	//		if ( tempModelName.empty() == false )
//	//		{
//	//			//����ġ ����̸� �׳� �޽�			
//	//			if ( pItem->GetItemTemplet() != NULL )
//	//			{
//	//				if ( pItem->GetItemTemplet()->m_UseType == CX2Item::UT_ATTACH_NORMAL )
//	//				{
//	//					/*
//	//					CKTDXDeviceXMesh* pModel = g_pKTDXApp->GetDeviceManager()->OpenXMesh( tempModelName );
//	//					if ( pModel != NULL )
//	//					{
//	//						vecReadyDevice.push_back( pModel );
//	//					}
//	//					*/
//	//				}
//	//				else
//	//				{
//	//					/*
//	//					CKTDXDeviceXSkinMesh* pModel = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( tempModelName );
//	//					if ( pModel != NULL )
//	//					{
//	//						vecReadyDevice.push_back( pModel );
//	//					}
//	//					*/
//	//				}
//	//			}
//	//		}
//	//	}
//
//	//	if ( tempTextureName.empty() == false )
//	//	{
//	//		/*
//	//		CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( tempTextureName );
//	//		if ( pTexture != NULL )
//	//		{
//	//			vecReadyDevice.push_back( pTexture );
//	//		}
//	//		*/
//	//	}
//	//}
//}
//
//bool CX2CharacterRoom::IsInUnitClickReact( D3DXVECTOR2 mousePos )
//{
//	D3DXVECTOR2 m_ConvertPos		= g_pKTDXApp->ConvertByResolution( m_UnitClickPos );
//	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_UnitClickSize );
//
//	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
//		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
//	{
//		return true;
//	}
//
//	return false;
//}
//
//
//void CX2CharacterRoom::ResetNowEquipUI()
//{
//	CX2Unit::UnitData* pMyUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//	{
//		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//		if ( pItemSlot != NULL && pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//		{
//			pItemSlot->DestroyItemUI();
//		}
//
//	}
//
//	if ( pMyUnitData != NULL && pInventory != NULL )
//	{
//		for ( int i = 0; i < (int)pMyUnitData->m_NowEqipItemUIDList.size(); i++ )
//		{
//			UidType uidType = pMyUnitData->m_NowEqipItemUIDList[i];
//			CX2Item* pItem = pInventory->GetItem( uidType );
//			if ( pItem == NULL )
//				continue;
//
//			for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//			{
//				CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//				if ( pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED && pItemSlot->GetEquipPos() == pItem->GetItemTemplet()->m_EqipPosition &&
//					pItemSlot->GetFashion() == pItem->GetItemTemplet()->m_bFashion )
//				{
//					pItemSlot->CreateItemUI( pItem );
//
//					break;
//				}
//			}
//		}
//
//		for ( int i = 0; i < (int)pInventory->GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT ); i++ )
//		{
//			CX2Item* pItem = pInventory->GetItem( CX2Inventory::ST_E_QUICK_SLOT, i );
//			if ( pItem == NULL )
//				continue;
//
//			for ( int j = 0; j < (int)m_SlotList.size(); j++ )
//			{
//				CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
//				if ( pItemSlot->GetSortType() == CX2Inventory::ST_E_QUICK_SLOT && pItemSlot->GetSlotID() == i )
//				{
//					pItemSlot->CreateItemUI( pItem );
//					break;
//				}
//			}
//		}
//	}
//
//
//	ResetSkillSlotUI();
//}
//
//void CX2CharacterRoom::ResetStat()
//{
//	m_Stat.InitStat();
//	m_StatEqip.InitStat();
//
//	m_Stat.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetUnitStat() );
//	m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetEqipStat(), true );
//	m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetSkillStat(), true );
//	m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetSetItemOptionStat(), true );
//	//m_StatEqip.AddStat( g_pData->GetMyUser()->GetSelectUnit()->GetEnchantDefence(), true );
//
//    
//#ifdef TITLE_SYSTEM
//    m_StatEqip.AddStat( g_pData->GetTitleManager()->GetSocketStat(), true );
//#endif
//	
//
//	// ���� �ɼ� ���� �����ְ�
//
//
//	// ��Ʈ ������ �ɼ� ���� �����ְ�.. ���� �̳��� ������~~
//
//	WCHAR buff[256] = {0};
//
//	// 2008.10.27 �κ��丮 ���� ���� �κ� ����, ���� �ڵ�� comment out - ���¿�
//	// X2SkillTreeUI.cpp���� �ܾ���
//	//{{
//
//	CKTDGUIStatic* pStaticUnitStatContent;
//	CKTDGUIStatic* pStaticUnitStatContent_add;
//	CKTDGUIStatic* pStaticUnitStatContent_NoEquip;
//
//	// hp
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"HP_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"HP_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"HP_Point_NoEquip" );
//
//	if( m_StatEqip.GetStat().m_fBaseHP == 0 &&
//		m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate == 0.f )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fBaseHP );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//
//		pStaticUnitStatContent->SetShow(true);
//		pStaticUnitStatContent_add->SetShow(true);
//		pStaticUnitStatContent_NoEquip->SetShow(false);
//
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fBaseHP );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//
//		// �����Ǵ� HP ���
//		if( m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate > CX2SocketItem::SocketData::MAX_INCREASE_HP_RATE )
//			m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate = CX2SocketItem::SocketData::MAX_INCREASE_HP_RATE;
//
//		float fAddHP = m_StatEqip.GetStat().m_fBaseHP;
//		fAddHP += m_Stat.GetStat().m_fBaseHP * m_StatEqip.GetStat().m_ExtraStat.m_fIncreaseHPRate;
//
//
//
//		if( fAddHP > 0.f )
//			wsprintf( buff, L"+%d", (int)fAddHP );
//		else
//			wsprintf( buff, L"-%d", (int)-fAddHP );
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//	}
//
//
//
//	// phy_attk
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Attack_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Attack_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Attack_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fAtkPhysic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkPhysic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkPhysic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fAtkPhysic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fAtkPhysic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fAtkPhysic ));
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//	}
//
//	// mag_attk
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fAtkMagic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkMagic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkMagic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fAtkMagic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fAtkMagic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fAtkMagic) );
//
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//
//
//	}
//
//
//	// def
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Defense_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Defense_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Defense_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fDefPhysic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefPhysic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefPhysic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fDefPhysic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fDefPhysic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fDefPhysic) );
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//	}
//
//
//	// magicdef
//	pStaticUnitStatContent			= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Defense_Point" );
//	pStaticUnitStatContent_add		= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Defense_Point_add" );
//	pStaticUnitStatContent_NoEquip	= (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Magic_Defense_Point_NoEquip" );
//	if( m_StatEqip.GetStat().m_fDefMagic == 0 )
//	{
//		pStaticUnitStatContent -> SetShow(false);
//		pStaticUnitStatContent_add -> SetShow(false);
//		pStaticUnitStatContent_NoEquip -> SetShow(true);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefMagic );
//		pStaticUnitStatContent_NoEquip -> GetString(0)->msg = buff;
//	}
//	else
//	{
//		pStaticUnitStatContent -> SetShow(true);
//		pStaticUnitStatContent_add -> SetShow(true);
//		pStaticUnitStatContent_NoEquip -> SetShow(false);
//
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefMagic );
//		pStaticUnitStatContent->GetString(0)->msg = buff;
//
//		if( m_StatEqip.GetStat().m_fDefMagic > 0 )
//			wsprintf( buff, L"+%d", (int)m_StatEqip.GetStat().m_fDefMagic );
//		else
//			wsprintf( buff, L"-%d", abs((int)m_StatEqip.GetStat().m_fDefMagic) );
//		pStaticUnitStatContent_add ->GetString(0)->msg = buff;
//
//	}
//
//
//	const CX2Stat::EnchantStat& enchantStat = g_pData->GetMyUser()->GetSelectUnit()->GetEnchantStat( true );
//
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Fire_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefBlaze, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Ice_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefWater, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Nature_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefNature, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Wind_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefWind, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Light_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefLight, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"Dark_Resist" );
//	wsprintf( buff, L"%d/%d", (int)enchantStat.m_fDefDark, (int)CX2EnchantItem::EAR_MAX_VALUE );
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//	/*
//	CKTDGUIStatic* pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticHP_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fBaseHP == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fBaseHP );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fBaseHP > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fBaseHP, (int)m_StatEqip.GetStat().m_fBaseHP );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fBaseHP, abs( (int)m_StatEqip.GetStat().m_fBaseHP ) );
//		}
//
//		
//	}
//
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticAttack_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fAtkPhysic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkPhysic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkPhysic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkPhysic, (int)m_StatEqip.GetStat().m_fAtkPhysic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkPhysic, abs( (int)m_StatEqip.GetStat().m_fAtkPhysic ) );
//		}
//	}
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticMagic_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fAtkMagic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkMagic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkMagic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkMagic, (int)m_StatEqip.GetStat().m_fAtkMagic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkMagic, abs( (int)m_StatEqip.GetStat().m_fAtkMagic ) );
//		}
//		
//	}
//
//	
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticDefense_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fDefPhysic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefPhysic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefPhysic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefPhysic, (int)m_StatEqip.GetStat().m_fDefPhysic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefPhysic, abs( (int)m_StatEqip.GetStat().m_fDefPhysic ) );
//		}	
//
//		
//	}
//
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	pStaticUnitStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticMDefense_Point_Left" );
//	if ( m_StatEqip.GetStat().m_fDefMagic == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefMagic );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefMagic > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefMagic, (int)m_StatEqip.GetStat().m_fDefMagic );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefMagic, abs( (int)m_StatEqip.GetStat().m_fDefMagic ) );
//		}
//	}
//
//	pStaticUnitStatContent->GetString(0)->msg = buff;
//
//	
//
//
//	CKTDGUIStatic* pStaticUnitDetailStatContent = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticPVP_Room_Unit_DetailStat_Content" );
//
//	if ( m_StatEqip.GetStat().m_fAtkFire == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkFire );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkFire > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkFire, (int)m_StatEqip.GetStat().m_fAtkFire );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkFire, abs( (int)m_StatEqip.GetStat().m_fAtkFire ) );
//		}
//
//		
//	}
//	
//	pStaticUnitDetailStatContent->GetString(0)->msg = buff;
//
//
//	if ( m_StatEqip.GetStat().m_fAtkIce == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkIce );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkIce > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkIce, (int)m_StatEqip.GetStat().m_fAtkIce );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkIce, abs( (int)m_StatEqip.GetStat().m_fAtkIce ) );
//		}
//	}
//
//	pStaticUnitDetailStatContent->GetString(1)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fAtkLightning == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkLightning );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkLightning > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkLightning, (int)m_StatEqip.GetStat().m_fAtkLightning );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkLightning, abs( (int)m_StatEqip.GetStat().m_fAtkLightning ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(2)->msg = buff;
//
//
//	if ( m_StatEqip.GetStat().m_fAtkEarth == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fAtkEarth );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fAtkEarth > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fAtkEarth, (int)m_StatEqip.GetStat().m_fAtkEarth );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fAtkEarth, abs( (int)m_StatEqip.GetStat().m_fAtkEarth ) );
//		}
//	}
//
//	pStaticUnitDetailStatContent->GetString(3)->msg = buff;
//
//
//	if ( m_StatEqip.GetStat().m_fDefFire == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefFire );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefFire > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefFire, (int)m_StatEqip.GetStat().m_fDefFire );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefFire, abs( (int)m_StatEqip.GetStat().m_fDefFire ) );
//		}
//
//
//	}
//
//	pStaticUnitDetailStatContent->GetString(4)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fDefIce == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefIce );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefIce > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefIce, (int)m_StatEqip.GetStat().m_fDefIce );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefIce, abs( (int)m_StatEqip.GetStat().m_fDefIce ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(5)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fDefLightning == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefLightning );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefLightning > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefLightning, (int)m_StatEqip.GetStat().m_fDefLightning );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefLightning, abs( (int)m_StatEqip.GetStat().m_fDefLightning ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(6)->msg = buff;
//
//
//
//	if ( m_StatEqip.GetStat().m_fDefEarth == 0 )
//		wsprintf( buff, L"%d", (int)m_Stat.GetStat().m_fDefEarth );
//	else
//	{
//		if ( m_StatEqip.GetStat().m_fDefEarth > 0 )
//		{
//			wsprintf( buff, L"%d #C008000+%d", (int)m_Stat.GetStat().m_fDefEarth, (int)m_StatEqip.GetStat().m_fDefEarth );
//		}
//		else
//		{
//			wsprintf( buff, L"%d #C008000-%d", (int)m_Stat.GetStat().m_fDefEarth, abs( (int)m_StatEqip.GetStat().m_fDefEarth ) );
//		}
//
//		
//	}
//
//	pStaticUnitDetailStatContent->GetString(7)->msg = buff;
//	*/
//
//
//}
//
//void CX2CharacterRoom::ResetCharRoomEDnCashUInVP()
//{
//
//	WCHAR buff[256] = {0};
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	CKTDGUIStatic* pStaticPVP_Room_Unit_ED = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticED_Num" );
//	//wsprintf( buff, L"%d", (int)pUnitData->m_ED );
//	pStaticPVP_Room_Unit_ED->GetString(0)->msg = g_pMain->GetEDString( (int)pUnitData->m_ED );
//
//	pStaticPVP_Room_Unit_ED = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticVP_Num" );
//	wsprintf( buff, L"%d", (int)pUnitData->m_VSPoint  );
//	pStaticPVP_Room_Unit_ED->GetString(0)->msg = buff;
//
//	pStaticPVP_Room_Unit_ED = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticCash_Num" );
//	pStaticPVP_Room_Unit_ED->GetString(0)->msg = g_pMain->GetEDString( g_pData->GetMyUser()->GetCash() );
//}
//
//void CX2CharacterRoom::UpdateLevel()
//{
//
//	CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"StaticLv_Num" );
//	if( NULL == pStaticUserInfo )
//		return;
//
//	if( NULL == g_pData->GetMyUser() ||
//		NULL == g_pData->GetMyUser()->GetSelectUnit() )
//		return;
//
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	WCHAR wszText[32] = L"";
//	wsprintf( wszText, L"%d", (int)pUnitData->m_Level );
//	pStaticUserInfo->GetString(0)->msg = wszText;
//
//
//}
//
//
//void CX2CharacterRoom::RemoveNoEnduranceNowEquip()
//{
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//	if ( pInventory != NULL )
//	{
//		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
//		{
//			CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
//			if ( pItemSlot != NULL && pItemSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//			{
//				if ( pItemSlot->GetSortType() == CX2Inventory::ST_E_EQUIP )
//				{
//					UidType itemUID = pItemSlot->GetItemUID();
//					CX2Item* pItem = pInventory->GetItem( itemUID );
//					if ( pItem != NULL )
//					{
//						const CX2Item::ItemData& kItemData = &pItem->GetItemData();
//						if ( kItemData.m_PeriodType == CX2Item::PT_ENDURANCE )
//						{
//							if ( kItemData.m_Endurance == 0 )
//							{
//								m_pUnitViewerUI->RemoveEqip( kItemData.m_ItemUID );
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//
//void CX2CharacterRoom::ResetResurrectionStoneUI()
//{
//	CKTDGUIStatic* pStaticResurrectionNum = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"resurrectionNum" );
//	if ( pStaticResurrectionNum != NULL && pStaticResurrectionNum->GetString(0) != NULL )
//		pStaticResurrectionNum->GetString(0)->msg = g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum() );	
//}
//
//void CX2CharacterRoom::ResetSpirit()
//{
//	if ( m_pDLGMyInfoFront != NULL )
//	{
//		CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//		if ( pUnitData != NULL )
//		{
//			CKTDGUIStatic* pStaticSpiritGauge = (CKTDGUIStatic*)m_pDLGMyInfoFront->GetControl( L"SpiritGauge" );
//			if ( pStaticSpiritGauge != NULL && pStaticSpiritGauge->GetPicture(0) != NULL )
//			{
//				float _width = pStaticSpiritGauge->GetPicture(0)->GetOriginalSize().x * ( pUnitData->m_iSpirit / (float)pUnitData->m_iSpiritMax );
//
//				pStaticSpiritGauge->GetPicture(0)->SetSizeX( _width );
//			}
//
//			CKTDGUIButton* pButtonSpiritGauge = (CKTDGUIButton*)m_pDLGMyInfoFront->GetControl( L"SpiritButton" );
//			if ( pButtonSpiritGauge != NULL )
//			{
//				stringstream strstm;
//				strstm << "�ټ��� : " << (int)((pUnitData->m_iSpirit / (float)pUnitData->m_iSpiritMax) * 100 ) << "%";
//				pButtonSpiritGauge->SetGuideDesc_LUA( strstm.str().c_str() );
//			}
//		}
//	}
//
//}
//
//void CX2CharacterRoom::ResetRoomStateToInventory( UidType iItemUID )
//{
//	if ( GetCharRoomState() != CX2CharacterRoom::CRS_INVENTORY )
//		return;
//
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//	CX2Item* pItem = pInventory->GetItem( iItemUID );
//
//	int invenPage = 1;
//	
//
//	for ( int i = 0; i < pInventory->GetItemMaxNum( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) ); i++ )
//	{
//		CX2Item* pItemTemp = pInventory->GetItem( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), i );
//		if ( pItemTemp == NULL )
//		{
//			int showPage = i / MY_INVENTORY_SHOW_MAX_SLOT_NUM;
//			showPage++;
//
//			if ( showPage != 1 )
//				invenPage = showPage;
//
//
//			break;
//		}
//	}
//
//
//	if ( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) != m_NowInventorySortType 
//		|| invenPage != m_NowInvenSortTypePageNum )
//	{
//		if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
//		{
//			wstring radioButtonName = GetRadioButtonNameByInvenSortType( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ) );
//			CKTDGUIRadioButton* pRadioButtonInvenSortType = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( radioButtonName.c_str() );
//			if ( pRadioButtonInvenSortType != NULL )
//			{
//				pRadioButtonInvenSortType->GetCheckedControl()->SetChecked( false );
//				pRadioButtonInvenSortType->SetChecked( true );
//
//				
//
//				SetInventorySort( pInventory->GetSortTypeByItemTemplet( pItem->GetItemTemplet() ), invenPage );
//			}
//		}
//	}
//}
//
//bool CX2CharacterRoom::UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo, bool bInventoryUIUpdate /*= true*/ )
//{
//	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
//	{
//		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
//		if ( kInventorySlotInfo.m_iSlotCategory == CX2Inventory::ST_E_EQUIP )
//		{
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem(
//				(CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID );
//			if ( pItem != NULL )
//			{
//				g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( pItem->GetUID() );
//				m_pUnitViewerUI->RemoveEqip( pItem->GetUID() );
//			}
//		}
//	}
//
//	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
//	{
//		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
//		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( 
//			(CX2Inventory::SORT_TYPE) kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID );
//	}
//
//	for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
//	{
//		KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
//		if ( kInventorySlotInfo.m_iItemUID > 0 )
//		{
//          CX2Item::ItemData kItemData( kInventorySlotInfo );
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().AddItem( 
//				(CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID, kItemData );
//
//			if ( kInventorySlotInfo.m_iSlotCategory == CX2Inventory::ST_E_EQUIP )
//			{
//				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
//					(CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_iSlotCategory, kInventorySlotInfo.m_iSlotID );
//				if ( pItem != NULL )
//				{
//					if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( pItem->GetUID() ) == true )
//					{
//						m_pUnitViewerUI->AddEqip( pItem->GetUID() );
//
//					
//
//
//					}
//				}
//			}
//
//		}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//
//	//	m_pUnitViewerUI->RemoveAllEquips();
//	//	m_pUnitViewerUI->UpdateEqip();
//
//	// ���� ������Ʈ
//	ResetNowEquipUI();
//
//	if( true == bInventoryUIUpdate )
//	{
//		// �κ��丮 ������Ʈ
//		if ( SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum ) == false )
//			return false;
//	}
//
//	// ���� ������Ʈ
//	ResetStat();
//
//
//	//// ��ų ���� ������Ʈ
//	//{
//	//	CX2SlotItem* pItemSlot = NULL;
//	//	
//	//	pItemSlot = (CX2SlotItem*)GetSlotByID( CX2Unit::NSSI_SKILL_1 );
//	//	pItemSlot->DestroyItemUI();
//	//	pItemSlot = (CX2SlotItem*)GetSlotByID( CX2Unit::NSSI_SKILL_2 );
//	//	pItemSlot->DestroyItemUI();
//	//	pItemSlot = (CX2SlotItem*)GetSlotByID( CX2Unit::NSSI_SKILL_3 );
//	//	pItemSlot->DestroyItemUI();
//	//}
//
//	return true;
//}
//
//void CX2CharacterRoom::UpdateSP()
//{
//
//	CKTDGUIStatic* pStatic_SP = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_SP" );
//	if( NULL == pStatic_SP )
//		return;
//
//	if( NULL == g_pData->GetMyUser() ||
//		NULL == g_pData->GetMyUser()->GetSelectUnit() )
//		return;
//
//
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	WCHAR wszText[32] = L"";
//	wsprintf( wszText, L"%d", (int)pUnitData->m_iSPoint );
//	pStatic_SP->GetString(0)->msg = wszText;
//
//
//}
//
//void CX2CharacterRoom::UpdateExpGageBar()
//{
//	
//	CKTDGUIStatic* pStaticEXP = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticUser_Inventory_Info_EXP" );
//	if( NULL == pStaticEXP )
//		return; 
//
//	CKTDGUIControl::CPictureData* pPictureEXP = pStaticEXP->GetPicture( 0 );
//	D3DXVECTOR2 picOrgSize = pPictureEXP->GetOriginalSize();
//
//	int nowExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//	int nowBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NowBaseLevelEXP;
//	int nextBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NextBaseLevelEXP;
//
//	pPictureEXP->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );
//
//	WCHAR buff[256] = {0};
//	if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_NORMAL_USER )
//	{
//		swprintf( buff, L"%d / %d (%.1f%%)", nowExp - nowBaseExp, nextBaseExp - nowBaseExp, (float)(nowExp - nowBaseExp)/(nextBaseExp - nowBaseExp)*100.0f );
//	}
//	else
//	{
//		swprintf( buff, L"%d/%d (%.1f%%) ( %d/%d )", nowExp - nowBaseExp, nextBaseExp - nowBaseExp, (float)(nowExp - nowBaseExp)/(nextBaseExp - nowBaseExp)*100.0f , nowExp, nextBaseExp );
//	}
//
//	pStaticEXP->GetString( 0 )->msg = buff;
//	
//}
//
//
//
//void CX2CharacterRoom::ResetSkillSlotUI()
//{
//	// ��ų ���� update
//	WCHAR wszText[256] = L"";
//
//	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_Skill_Slot" );
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//
//	if( 0 == pUnitData->m_EqipSkill1 )
//	{
//		pStatic->GetPicture(0)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill1 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(0)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(0)->SetShow( true );
//			if( false == pStatic->GetPicture(0)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//	if( 0 == pUnitData->m_EqipSkill2 )
//	{
//		pStatic->GetPicture(1)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill2 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(1)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(1)->SetShow( true );
//			if( false == pStatic->GetPicture(1)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(1)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//
//	}
//
//
//	if( 0 == pUnitData->m_EqipSkill3 )
//	{
//		pStatic->GetPicture(2)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) pUnitData->m_EqipSkill3 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(2)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(2)->SetShow( true );
//			if( false == pStatic->GetPicture(2)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(2)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//
//
//
//
//	if( 0 == pUnitData->m_Eqip2Skill1 )
//	{
//		pStatic->GetPicture(3)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill1 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(3)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(3)->SetShow( true );
//			if( false == pStatic->GetPicture(3)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(3)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//
//
//
//
//	if( 0 == pUnitData->m_Eqip2Skill2 )
//	{
//		pStatic->GetPicture(4)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill2 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(4)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(4)->SetShow( true );
//			if( false == pStatic->GetPicture(4)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(4)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//
//
//	if( 0 == pUnitData->m_Eqip2Skill3 )
//	{
//		pStatic->GetPicture(5)->SetShow( false );
//	}
//	else
//	{
//		CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(  (CX2SkillTree::SKILL_ID) pUnitData->m_Eqip2Skill3 );
//
//		if( NULL == pSkillTemplet )
//		{
//			pStatic->GetPicture(5)->SetShow( false );
//		}
//		else
//		{
//			pStatic->GetPicture(5)->SetShow( true );
//			if( false == pStatic->GetPicture(5)->SetTex( pSkillTemplet->m_wstrTextureName.c_str(), pSkillTemplet->m_wstrTexturePieceName.c_str() ) )
//			{
//				pStatic->GetPicture(5)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//		}
//	}
//
//
//	if( NULL != m_pSkillTreeUI )
//	{
//		m_pSkillTreeUI->ResetEquippedSkillSlotUI();
//	}
//}
//
//
//
//
//
//bool CX2CharacterRoom::XSkinMeshReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceXSkinMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//bool CX2CharacterRoom::XMeshReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceXMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXMesh( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//bool CX2CharacterRoom::TextureReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceTexture* pDevice = g_pKTDXApp->GetDeviceManager()->OpenTexture( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//bool CX2CharacterRoom::SoundReady( const WCHAR* pFileName )
//{
//	CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSound( pFileName );
//	if( pDevice == NULL )
//		return false;
//
//	DeviceReady( pDevice );
//	return true;
//}
//
//
//
//wstring CX2CharacterRoom::GetRadioButtonNameByInvenSortType( CX2Inventory::SORT_TYPE sortType )
//{
//	wstring radioButtonName = L"";
//
//	//{{ 2009.01.19 ���¿� : �ڵ�����
//	switch(sortType)
//	{
//	case CX2Inventory::ST_EQUIP:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Equip";
//		} break;
//	case CX2Inventory::ST_ACCESSORY:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Accessory";
//		} break;
//	case CX2Inventory::ST_QUICK_SLOT:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_UseItem";
//		} break;
//	case CX2Inventory::ST_MATERIAL:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Material";
//		} break;
//	case CX2Inventory::ST_QUEST:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Card";
//		} break;
//	case CX2Inventory::ST_SPECIAL:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Special";
//		} break;
//	case CX2Inventory::ST_AVARTA:
//		{
//			radioButtonName = L"RadioButtonUnit_Inventory_Avatar";
//		} break;
//	default:
//		break;
//	}
//	//}} elseif -> switch
//	
//	return radioButtonName;
//}
//
//void CX2CharacterRoom::UpdateOpenRandomItemAck()
//{
//	WCHAR buf[256] = {0};
//
//
//	wstring itemName = L"";
//	wstring acquireComment;
//
//	bool bCheck = false;
//
//	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
//	{
//		std::map< int, int >::iterator i;
//
//		int nCount = 0;
//		wstring getExplanation = L"";
//		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
//		{
//			int resultItemID = i->first;
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet != NULL )
//				itemName = pItemTemplet->GetFullName();
//
//			if ( nCount > 0 )
//			{
//				getExplanation += L", ";
//			}
//
//			wsprintf( buf, L"%s", itemName.c_str() );
//
//			getExplanation += buf;
//
//			//{{ 2009.2.4 ���¿� : ���������� ȹ��� ȹ��� ǥ��
//			int iNum = i->second;
//			if(iNum > 1)
//			{
//				wsprintf( buf, L" %d��", iNum );
//				getExplanation += buf;
//			}
//
//			//}}
//
//			nCount++;
//		}
//
//		wsprintf( buf, L"%s ������ϴ�!",
//			KHanSungPostWordUnicode( (WCHAR*)getExplanation.c_str() ) ? L"��" : L"��");
//
//		getExplanation += buf;
//
//		acquireComment = getExplanation;
//		bCheck = true;
//	}
//	
//	if ( m_TempOpenRandomItemAck.m_iRessurectionCount != 0 )
//	{
//		wstringstream tempString;
//		int getStoneNum = m_TempOpenRandomItemAck.m_iRessurectionCount - g_pData->GetMyUser()->GetSelectUnit()->GetResurrectionStoneNum(); 
//		tempString << L"��Ȱ�� " << getStoneNum << L"���� ������ϴ�!";
//
//		g_pData->GetMyUser()->GetSelectUnit()->SetResurrectionStoneNum( m_TempOpenRandomItemAck.m_iRessurectionCount );
//		ResetResurrectionStoneUI();
//
//		acquireComment = tempString.str().c_str();
//		bCheck = true;
//	}
//
//	
//
//
//	if ( bCheck == true )
//	{
//		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.str().c_str(), m_pNowState );
//		SAFE_DELETE_DIALOG( m_pDLGCubeCheck );
//		m_pDLGCubeCheck = new CKTDGUIDialog( m_pNowState, L"DLG_Cube_Result_Window.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCubeCheck );
//		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCubeCheck->GetControl( L"StaticCube_Window_Text" );
//		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
//		{
//			wstring tpString = g_pMain->GetStrByLienBreak( acquireComment.c_str(), 366, XUF_DODUM_15_BOLD );
//			pStatic->GetString(0)->msg = tpString;
//		}
//
//		if ( m_RandomItemID == 129755 )
//		{
//			CKTDGUIStatic* pStaticCube = (CKTDGUIStatic*)m_pDLGCubeCheck->GetControl( L"g_pStaticCube_Image" );
//			if ( pStaticCube != NULL )
//			{
//				if ( pStaticCube->GetPicture(0) != NULL )
//				{
//					pStaticCube->GetPicture(0)->SetTex( L"DLG_Common_New_Texture62.tga", L"CUBE_EVENT_MILITARY" );
//				}
//			}
//		}
//	}
//	else if ( m_TempOpenRandomItemAck.m_iRestoreSpirit != 0 )
//	{
//		wstringstream tempString;
//		int getSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpirit;
//		getSpirit = (int)(getSpirit / (float)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSpiritMax * 100.0f);
//		tempString << L"�ټ��� " << getSpirit << L"%�� ȸ���Ͽ����ϴ�.";
//
//		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_iSpirit = m_TempOpenRandomItemAck.m_iRestoreSpirit;
//		ResetSpirit();
//
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), tempString.str().c_str(), m_pNowState );
//	}
//	else
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�ƹ��͵� ���� ���߽��ϴ�!!", m_pNowState );
//	}
//
//
//	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//}
//
//void CX2CharacterRoom::OpenSellWindow( CX2SlotItem* pSlot )
//{
//	int popUpSizeX = 425;
//	int popUpSizeY = 147;
//
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"�������� �������� �� �� �����ϴ�.", m_pNowState );
//	}
//	else
//	{
//		m_TempItemUID = pSlot->GetItemUID();
//
//		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );
//		if ( pItem != NULL )
//		{
//			if ( pItem->GetItemTemplet() != NULL )
//			{
//				if ( pItem->GetItemTemplet()->m_PriceType == CX2Item::SPT_NONE )
//				{
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//						L"�ش� �������� �� �� �����ϴ�!", m_pNowState );
//				}
//				else
//				{
//					if ( m_sbCheckQuestionSellNum == false )
//					{
//						if ( pItem->GetItemTemplet() != NULL && pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_QUANTITY )
//							m_SellItemNum = pItem->GetItemData().m_Quantity;
//						else
//							m_SellItemNum = 1;
//
//						WCHAR buff[256] = {0};
//						wsprintf( buff, L"�ǸŰ��� %d ED�Դϴ�.\n���� �Ľðڽ��ϱ�?", pItem->GetEDToSell() * m_SellItemNum );
//
//						m_pDialogToRemoveItem = g_pMain->KTDGUIOkAndCancelMsgBox( 
//							D3DXVECTOR2( -999, -999 ), 
//							buff, CRM_QUICK_SELL_ITEM , m_pNowState, CRM_QUICK_SELL_ITEM_CANCEL );
//
//						CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDialogToRemoveItem->GetControl( L"MsgBoxOkAndCancelOkButton" );
//						if ( pButton != NULL )
//						{
//							pButton->ChangeMouseUpSound( L"Item_Sell.ogg" );
//						}
//					}
//					else
//					{
//						if ( pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_QUANTITY )
//						{
//							OpenItemSellNumDLG( pItem->GetItemData().m_ItemUID );
//						}
//						else
//						{
//							WCHAR buff[256] = {0};
//							wsprintf( buff, L"�ǸŰ��� %d ED�Դϴ�.\n���� �Ľðڽ��ϱ�?", pItem->GetEDToSell() );
//							m_SellItemNum = 1;
//							m_pDialogToRemoveItem = g_pMain->KTDGUIOkAndCancelMsgBox( 
//								D3DXVECTOR2( -999, -999 ), 
//								buff, CRM_QUICK_SELL_ITEM , m_pNowState, CRM_QUICK_SELL_ITEM_CANCEL );
//
//							CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDialogToRemoveItem->GetControl( L"MsgBoxOkAndCancelOkButton" );
//							if ( pButton != NULL )
//							{
//								pButton->ChangeMouseUpSound( L"Item_Sell.ogg" );
//							}
//						}
//					}
//				}
//			}
//		}
//
//	}	
//}
//
//void CX2CharacterRoom::OpenItemSellNumDLG( UidType sellItemUID )
//{
//	SAFE_DELETE_DIALOG( m_pDLGSelectSellNumWindowPopup );
//
//	m_SellItemUID	= sellItemUID;
//	m_SellItemNum	= 1;
//
//	m_pDLGSelectSellNumWindowPopup = new CKTDGUIDialog( m_pNowState, L"DLG_Item_Sell_Num_Window.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSelectSellNumWindowPopup );
//	m_pDLGSelectSellNumWindowPopup->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 366, 212 ), D3DXVECTOR2( 211, 125 ) ) );
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SellItemUID );
//	if ( pItem != NULL )
//	{
//		WCHAR buff[256] = {0};
//		CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"Item_Name" );
//		if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
//		{
//			pStaticItemName->GetString(0)->msg = pItem->GetItemTemplet()->GetFullName();
//		}
//
//		CKTDGUIStatic* pStaticItemSellED = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"StaticItem_Sell_ED" );
//		if ( pStaticItemSellED != NULL && pStaticItemSellED->GetString(0) != NULL )
//		{
//			wsprintf( buff, L"%d", pItem->GetEDToSell() );
//			pStaticItemSellED->GetString(0)->msg = buff;
//		}
//	}
//}
//
//void CX2CharacterRoom::OpenRepairWindow( CX2SlotItem* pSlot )
//{
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//
//	if ( pItem != NULL )
//	{
//		if ( pItem->GetItemTemplet()->m_PeriodType == CX2Item::PT_ENDURANCE )
//		{
//			if ( pItem->GetItemData().m_Endurance >= pItem->GetItemTemplet()->m_Endurance )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//					L"�������� �̹� �� ���ֽ��ϴ�!", m_pNowState );
//			}
//			else
//			{
//				m_TempSortType = ((CX2SlotItem*)pSlot)->GetSortType();
//				m_TempSlotID = ((CX2SlotItem*)pSlot)->GetSlotID();
//
//				WCHAR buff[256] = {0};
//				int repairEDCost = pItem->GetEDToRepair();
//				int repairVPCost = pItem->GetVPToRepair(); 
//
//
//				float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//				if( fDiscountRate > 0.f )
//				{
//					repairEDCost = (int) ( repairEDCost * ( 1.f - fDiscountRate ) );
//				}
//
//
//#ifdef ITEM_RECOVERY_TEST
//				if ( repairVPCost == 0 )
//				{
//					if(true == pItem->IsDisabled())
//						wsprintf( buff, L"��������� %d ED�Դϴ�. \n�������� ȸ�������� ��� �Ұ� ���´� ȸ������ \n�ʽ��ϴ�. ���� �����Ͻðڽ��ϱ�?", repairEDCost  );
//					else
//						wsprintf( buff, L"��������� %d ED�Դϴ�. \n���� �����Ͻðڽ��ϱ�?", repairEDCost  );
//				}
//				else
//				{
//					if(true == pItem->IsDisabled())
//						wsprintf( buff, L"��������� %d ED�� #CFF0000%d VP#CX�Դϴ�.\n�������� ȸ�������� ��� �Ұ� ���´� ȸ������ \n�ʽ��ϴ�. ���� �����Ͻðڽ��ϱ�?", repairEDCost, repairVPCost  );
//					else
//					wsprintf( buff, L"��������� %d ED�� #CFF0000%d VP#CX�Դϴ�. \n���� �����Ͻðڽ��ϱ�?", repairEDCost, repairVPCost  );
//				}
//#else
//				if ( repairVPCost == 0 )
//					wsprintf( buff, L"��������� %d ED�Դϴ�. \n���� �����Ͻðڽ��ϱ�?", repairEDCost  );
//				else
//					wsprintf( buff, L"��������� %d ED�� #CFF0000%d VP#CX�Դϴ�. \n���� �����Ͻðڽ��ϱ�?", repairEDCost, repairVPCost  );
//#endif
//				m_pDLGToRepair = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
//					buff, CRM_REPAIR_ITEM , m_pNowState );
//			}
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"���������� �������� �ƴմϴ�!", m_pNowState );
//		}
//	}
//}
//
//void CX2CharacterRoom::OpenDecompositionWindow( CX2SlotItem* pSlot )
//{
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"�������� �������� ������ �� �����ϴ�.", m_pNowState );
//
//		return;
//	}
//
//	if ( pSlot->GetSlotType() == CX2Slot::ST_INVENTORY )
//	{
//		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );
//#ifdef ITEM_RECOVERY_TEST
//		if ( pItem == NULL )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"�������� ã�� �� �����ϴ�!", m_pNowState );
//
//			return;
//		}
//		if(true == pItem->IsDisabled())
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �Ұ� ������ �������Դϴ�.\n[�ູ ���� ������ �ֹ���] �������� ����ؼ�\n��� �Ұ� ���¸� �����·� ������ �� �ֽ��ϴ�.", m_pNowState );
//			return; 
//		}
//#endif // ITEM_RECOVERY_TEST
//
//#ifdef ENABLE_RESOLVE_FASHION
//
//		if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"�������� ã�� �� �����ϴ�!", m_pNowState );
//
//			return;
//		}
//
//		if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON || 
//			pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_DEFENCE )
//		{
//			if ( pItem->GetItemTemplet()->m_bFashion == true )
//			{
//				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY &&
//					pItem->GetItemData().m_Period > 0 )
//				{
//					//�Ⱓ���� �Ǹ�.
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//						L"�Ⱓ�� �������� ������ �� �����ϴ�!", m_pNowState );
//
//					return;
//				}
//			}
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"��� �����۸� ������ �� �ֽ��ϴ�.", m_pNowState );
//
//			return;
//		}
//
//
//#else //ENABLE_RESOLVE_FASHION
//
//		if ( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"��� �����۸� ������ �� �ֽ��ϴ�.", m_pNowState );
//
//			return;
//		}
//
//#endif //ENABLE_RESOLVE_FASHION
//
//		
//
//		
//
//		if ( pItem != NULL )
//		{
//			m_DecompositionItemUID = pItem->GetUID();
//			if ( m_pDLGResolveItem != NULL )
//			{
//				g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGResolveItem, NULL, false );
//			}
//			//m_pDLGResolveItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), L"���� �����Ͻðڽ��ϱ�?", CX2CharacterRoom::CRM_RESOLVE_ITEM, m_pNowState );
//			m_pDLGResolveItem = new CKTDGUIDialog( m_pNowState, L"DLG_Resolve_Item.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItem );
//			m_pDLGResolveItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 400, 244 ), D3DXVECTOR2( 197, 157 ) ) );
//
//			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItem->GetControl( L"Static_broken_item_Slot" );
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
//			{
//				if ( pItem->GetItemTemplet()->m_ShopImage.empty() == false )
//					pStaticSlot->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//				else
//					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//			CKTDGUIControl* pControlButton = m_pDLGResolveItem->GetControl( L"ok_Button" );
//			if ( pControlButton != NULL )
//				pControlButton->RequestFocus();
//		}
//	}	
//}
//
//void CX2CharacterRoom::ChangeDecompositionPage( bool bNextPage )
//{
//	//�Žñ� â�� ����� �ִ� ��쿡��..
//	if ( m_pDLGResolveItemResult != NULL )
//	{
//		if ( bNextPage == false )
//		{
//			if ( m_DecompositionWindowNowPageNum <= 1 )
//				return;
//
//			m_DecompositionWindowNowPageNum--;
//		}
//
//		
//
//		int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
//		if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
//		{
//			maxPage += 1;
//		}
//
//		if ( bNextPage == true )
//		{
//			if ( m_DecompositionWindowNowPageNum >= maxPage )
//				return;
//
//			m_DecompositionWindowNowPageNum++;
//		}
//
//		wstringstream wstrstm;
//		wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
//		CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"StaticResolve_List_Page_Num");
//		if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
//		{
//			pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
//		}
//
//		CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_broken_result_Slot" );
//		CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_comment1" );
//		CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_num1" );
//
//		std::map< int, int >::iterator i;
//		int j = 0;
//		i = m_mapResolveResultInsertedItem.begin();
//		for ( int nextNum = 0; nextNum < (m_DecompositionWindowNowPageNum - 1) * RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; nextNum++ )
//		{
//			i++;
//		}
//
//		std::map< int, int >::iterator maxiter = m_mapResolveResultInsertedItem.begin();
//		for ( int nextNum = 0; nextNum < ( RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM*m_DecompositionWindowNowPageNum ); nextNum++ )
//		{
//			maxiter++;
//		}
//
//		//�ϴ� �ʱ�ȭ ���� ��������Ҳ� ���� �Ĵ���
//		for ( int slotNum = 0; slotNum < RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM; slotNum++ )
//		{
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(slotNum) != NULL )
//			{
//				pStaticSlot->GetPicture(slotNum)->SetTex( L"DLG_Common_New_Texture02.dds", L"item_slot1" );
//			}
//
//			if ( pStaticName != NULL && pStaticName->GetString(slotNum) != NULL )
//			{
//				pStaticName->GetString(slotNum)->msg = L"";
//			}
//
//			if ( pStaticNum != NULL && pStaticNum->GetString(slotNum) != NULL )
//			{
//				pStaticNum->GetString(slotNum)->msg = L"";
//			}
//		}
//		
//		for ( ; i != m_mapResolveResultInsertedItem.end() && i != maxiter ; i++, j++ )
//		{
//			int resultItemID = i->first;
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet != NULL )
//			{
//				if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
//				{
//					if ( pItemTemplet->m_ShopImage.empty() == false )
//						pStaticSlot->GetPicture(j)->SetTex( pItemTemplet->m_ShopImage.c_str() );
//					else
//						pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//				}
//
//				if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
//				{
//					pStaticName->GetString(j)->msg = pItemTemplet->GetFullName();
//				}
//
//				if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"x " << i->second;
//					pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
//				}
//			}
//		}
//	}
//}
//
//
//
//// @bIdentifyResult: ���ذ��â�� �ƴ� �Ӽ��� ���� ���â����, ���̾�α׸� �������� �Ἥ �ϴ��� �䷸�� ����
//void CX2CharacterRoom::OpenDecompositionResultWindow( std::map< int, int >& mapInsertedItem, 
//													 std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
//													 bool bJackpot, bool bIdentifyResult /*= false*/ )
//{
//	SAFE_DELETE_DIALOG( m_pDLGResolveItemResult );
//
//	m_DecompositionWindowNowPageNum = 1;
//	m_mapResolveResultInsertedItem = mapInsertedItem;
//
//	m_pDLGResolveItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Resolve_Item_Result.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGResolveItemResult );
//
//
//	CKTDGUIStatic* pStatic_ResolveTitle		= (CKTDGUIStatic*) m_pDLGResolveItemResult->GetControl( L"g_pStatic_WindowTitle_Resolve99" );
//	CKTDGUIStatic* pStatic_IdentifyTitle	= (CKTDGUIStatic*) m_pDLGResolveItemResult->GetControl( L"g_pStatic_WindowTitle_Identify99" );
//	if( true == bIdentifyResult )		
//	{
//		if( NULL != pStatic_ResolveTitle )
//			pStatic_ResolveTitle->SetShowEnable( false, false );
//
//		if( NULL != pStatic_IdentifyTitle )
//			pStatic_IdentifyTitle->SetShowEnable( true, true );
//
//	}
//	else
//	{
//		if( NULL != pStatic_ResolveTitle )
//			pStatic_ResolveTitle->SetShowEnable( true, true );
//
//		if( NULL != pStatic_IdentifyTitle )
//			pStatic_IdentifyTitle->SetShowEnable( false, false );
//	}
//
//
//	if ( bJackpot == true && m_pDLGResolveItemResult->GetControl( L"StaticBig_Item_Acquirement" ) != NULL )
//		m_pDLGResolveItemResult->GetControl( L"StaticBig_Item_Acquirement" )->SetShow( true );
//
//	m_pDLGResolveItemResult->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 403, 350 ), D3DXVECTOR2( 270, 332 ) ) );
//
//	CKTDGUIControl* pControl = m_pDLGResolveItemResult->GetControl( L"ok_Button_Resolve_Item" );
//	if ( pControl != NULL )
//	{
//		pControl->RequestFocus();
//	}
//
//	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_broken_result_Slot" );
//	CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_comment1" );
//	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl( L"g_pStatic_num1" );
//
//	std::map< int, int >::iterator i;
//	int j = 0;
//	for ( i = mapInsertedItem.begin(); i != mapInsertedItem.end(); i++, j++ )
//	{
//		int resultItemID = i->first;
//
//		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//		if ( pItemTemplet != NULL )
//		{
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(j) != NULL )
//			{
//				if ( pItemTemplet->m_ShopImage.empty() == false )
//					pStaticSlot->GetPicture(j)->SetTex( pItemTemplet->m_ShopImage.c_str() );
//				else
//					pStaticSlot->GetPicture(j)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//			if ( pStaticName != NULL && pStaticName->GetString(j) != NULL )
//			{
//				pStaticName->GetString(j)->msg = pItemTemplet->GetFullName();
//			}
//
//			if ( pStaticNum != NULL && pStaticNum->GetString(j) != NULL )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"x " << i->second;
//				pStaticNum->GetString(j)->msg = wstrstm.str().c_str();
//			}
//		}
//	}
//
//	int maxPage = (int)m_mapResolveResultInsertedItem.size() / RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM;
//	if ( (int)m_mapResolveResultInsertedItem.size() % RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM != 0 )
//	{
//		maxPage += 1;
//	}
//
//	wstringstream wstrstm;
//	wstrstm << m_DecompositionWindowNowPageNum << L"/" << maxPage;
//	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGResolveItemResult->GetControl(L"StaticResolve_List_Page_Num");
//	if ( pStaticPageNum != NULL && pStaticPageNum->GetString(0) != NULL )
//	{
//		pStaticPageNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//
//	UpdateInventorySlotList( vecKInventorySlotInfo );
//}
//
//
//// ��ȭ�� ������ ������ ���� �ʿ��� ���ο� ��ȭ���� ������ ����ϴ� �Լ�
//int CX2CharacterRoom::GetEnchantStoneLevel( int iItemLevel )
//{
//
//	if( iItemLevel <= 20 )
//	{
//		return 0;
//	}
//	else if( iItemLevel <= 30 )
//	{
//		return 1;
//	}
//	else if( iItemLevel <= 40 )
//	{
//		return 2;
//	}
//	else if( iItemLevel <= 50 )
//	{
//		return 3;
//	}
//	else if( iItemLevel <= 60 )
//	{
//		return 4;
//	}
//	else if( iItemLevel <= 70 )
//	{
//		return 5;
//	}
//	else if( iItemLevel <= 80 )
//	{
//		return 6;
//	}
//	else if( iItemLevel <= 90 )
//	{
//		return 7;
//	}
//	else
//	{
//		return 8;
//	}
//}
//
//
//
//// ��ȭ UI ����, ��ȭ������ ��뿩�� üũ�ڽ��� üũ�� �� ���� ȣ��ȴ�.
//void CX2CharacterRoom::UpdateEnchantWindow()
//{
//	if( NULL == m_pDLGEnchantItem )
//		return;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnchantItemUID );
//	if( NULL == pItem )
//		return;
//
//
//	int normalStoneCount	= 0;
//	int newStoneCount		= 0;
//	int iNewStoneLevel		= GetEnchantStoneLevel( pItem->GetItemTemplet()->m_UseLevel );
//
//
//	int iAdjuvantItemID		= GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->m_UseLevel );
//	int iAdjuvantCount		= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iAdjuvantItemID );
//
//	wstring leftButtonDesc	= L"";
//	wstring rightButtonDesc		= L"";
//	
//
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
//			if ( pItemTemplet != NULL )
//			{
//				leftButtonDesc = pItemTemplet->GetFullName();
//			}
//
//			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//			if ( pItemTemplet != NULL )
//			{
//				rightButtonDesc = pItemTemplet->GetFullName();
//			}
//
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
//			if ( pItemTemplet != NULL )
//			{
//				leftButtonDesc = pItemTemplet->GetFullName();
//			}
//
//			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//			if ( pItemTemplet != NULL )
//			{
//				rightButtonDesc = pItemTemplet->GetFullName();
//			}
//		} break;
//
//	default:
//		{
//			return;
//		} break;
//	}
//
//	leftButtonDesc += L" 1��";
//	rightButtonDesc += L" 1��";
//
//	if( true == m_bUseEnchantAdjuvant )
//	{
//		leftButtonDesc += L"�� �÷���� ���� 1���� �Ҹ�˴ϴ�.";
//		rightButtonDesc += L"�� �÷���� ���� 1���� �Ҹ�˴ϴ�.";
//	}
//	else
//	{
//		leftButtonDesc += L"�� �Ҹ�˴ϴ�.";
//		rightButtonDesc += L"�� �Ҹ�˴ϴ�.";
//	}
//
//
//
//	WCHAR leftButtonName[128] = L"";
//	WCHAR rightButtonName[128] = L"";
//	CKTDGUIControl* pButton = NULL;
//
//
//
//
//	// ���� ���� ���� ��ȭ�� ��ư (�ູ���� ��ȭ��)
//	pButton = m_pDLGEnchantItem->GetControl( L"g_pButtonitem_normal_Button" );
//	if( NULL != pButton )
//	{
//		pButton->SetShowEnable( false, false );
//		pButton->SetGuideDesc( leftButtonDesc.c_str() );
//	}
//
//
//	// �÷���� ���� ������� �� �ູ���� ��ȭ�� ��ư
//	const int MAGIC_ENCHANT_BUTTON_COUNT = 4;
//	for( int i=0; i<MAGIC_ENCHANT_BUTTON_COUNT; i++ )
//	{
//		StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_a%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( leftButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//			pButton->SetGuideDesc( leftButtonDesc.c_str() );
//
//		}
//
//		StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_b%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( leftButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//			pButton->SetGuideDesc( leftButtonDesc.c_str() );
//		}
//	}
//	
//
//
//
//	CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Item_Num1" );
//	if( pStaticItemNum != NULL && pStaticItemNum->GetString( 0 ) != NULL )
//	{
//		wstringstream wstrstm;
//		if( true == m_bUseEnchantAdjuvant ) 
//		{
//			wstrstm << L"���� Ƚ��:" << min( normalStoneCount, iAdjuvantCount );
//		}
//		else
//		{
//			wstrstm << L"���� Ƚ��:" << normalStoneCount;
//		}
//		pStaticItemNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_a%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pButtonitem_normal_Button" );
//			}
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pitem_Bless_cash_upgrade_b%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( leftButtonName, sizeof(leftButtonName), L"g_pButtonitem_normal_Button" );
//			}
//		} break;
//	}
//
//
//
//	pButton = m_pDLGEnchantItem->GetControl( leftButtonName );
//	if( NULL != pButton )
//	{
//		if( normalStoneCount >= 1 )
//		{
//			pButton->SetShowEnable( true, true );
//		}
//		else
//		{
//			pButton->SetShowEnable( true, false );
//		}
//		pButton->SetGuideDesc( leftButtonDesc.c_str() );
//	}
//
//
//
//
//
//
//
//
//
//
//
//	// �������� �� ��ȭ�� ��ư
//	const int MAGIC_NEW_ENCHANT_BUTTON_COUNT = 4;
//	for( int i=0; i<MAGIC_NEW_ENCHANT_BUTTON_COUNT; i++ )
//	{
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_advanced%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_normal%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//
//
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_advanced%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//
//		StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_normal%d", i+1 );		
//		pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//		if( NULL != pButton )
//		{
//			pButton->SetShowEnable( false, false );
//		}
//	}
//
//
//	pStaticItemNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_Item_Num2" );
//	if ( pStaticItemNum != NULL && pStaticItemNum->GetString( 0 ) != NULL )
//	{
//		wstringstream wstrstm;
//		if( true == m_bUseEnchantAdjuvant ) 
//		{
//			wstrstm << L"���� Ƚ��:" << min( newStoneCount, iAdjuvantCount );
//		}
//		else
//		{
//			wstrstm << L"���� Ƚ��:" << newStoneCount;
//		}
//		pStaticItemNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_advanced%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_normal%d", iNewStoneLevel+1 );
//			}
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			if( true == m_bUseEnchantAdjuvant ) 
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_advanced%d", iNewStoneLevel+1 );
//			}
//			else
//			{
//				StringCchPrintfW( rightButtonName, sizeof(rightButtonName), L"g_pitem_spe_Button_def_normal%d", iNewStoneLevel+1 );
//			}
//		} break;
//	}
//
//
//
//	pButton = m_pDLGEnchantItem->GetControl( rightButtonName );
//	if( NULL != pButton )
//	{
//		if( newStoneCount >= 1 )
//		{
//			pButton->SetShowEnable( true, true );
//		}
//		else
//		{
//			pButton->SetShowEnable( true, false );
//		}
//		pButton->SetGuideDesc( rightButtonDesc.c_str() );
//	}
//
//
//}
//
//
//
//
//// ��ȭ UI �˾�
//void CX2CharacterRoom::OpenEnchantWindow( CX2SlotItem* pSlot )
//{
//	
//
//
//	if( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"�������� �������� ��ȭ�� �� �����ϴ�.", m_pNowState );
//		return;
//	}
//
//	if( m_NowInventorySortType != CX2Inventory::ST_EQUIP )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �����۸� ��ȭ�� �� �ֽ��ϴ�.", m_pNowState );
//		return;
//	}
//
//	if( pSlot->GetSlotType() != CX2Slot::ST_INVENTORY )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"�κ��丮�� �ִ� �����۸� ��ȭ�� �� �ֽ��ϴ�.", m_pNowState );
//		return;
//	}
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//	if( NULL == pItem )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"������ ������ �ùٸ��� �ʽ��ϴ�.", m_pNowState );
//		return; 
//	}
//
//
//#ifdef ITEM_RECOVERY_TEST
//	// ��ȭ ���� �˻� (���Ҵ��ΰ�?)
//	if(true == pItem->IsDisabled())
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �Ұ� ������ �������Դϴ�.\n[�ູ ���� ������ �ֹ���] �������� ����ؼ�\n��� �Ұ� ���¸� �����·� ������ �� �ֽ��ϴ�.", m_pNowState );
//		return; 
//	}
//
//#endif
//
//
//
//
//	m_bUseEnchantAdjuvant = false;
//
//
//	int normalStoneCount	= 0;
//	int newStoneCount		= 0;
//	int iNewStoneLevel		= GetEnchantStoneLevel( pItem->GetItemTemplet()->m_UseLevel );
//
//
//	int iAdjuvantItemID		= GetRequiredEnchantAdjuvantItemID( pItem->GetItemTemplet()->m_UseLevel );
//	int iAdjuvantCount		= g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( iAdjuvantItemID );
//
//	wstring normalButtonDesc	= L"";
//	wstring newButtonDesc		= L"";
//	WCHAR wszEnchantED[256]		= L"";
//
//
//
//
//	// ��� �˻�
//	int iCost = 0;
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			iCost = (int)( ( (float)pItem->GetItemTemplet()->m_Price * 0.1f ) * 0.5f );
//		}
//		break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			iCost = (int)( ( (float)pItem->GetItemTemplet()->m_Price * 0.1f ) * 0.1f );
//		}
//		break;
//	}
//
//	StringCchPrintfW( wszEnchantED, sizeof(wszEnchantED), L"%d", iCost );
//
//
//	switch( pItem->GetItemTemplet()->m_ItemType )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_WEAPON_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_WEAPON_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			if ( normalStoneCount <= 0 && newStoneCount <= 0 )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//					L"������ �������� ��ȭ�Ϸ��� ���� ��ȭ���� �ʿ��մϴ�!", m_pNowState );
//
//				return;
//			}
//
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			normalStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_DEFENCE_ENCHANT_STONE_ITEM_ID );
//			newStoneCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NEW_DEFENCE_ENCHANT_STONE_ITEM_ID[iNewStoneLevel] );
//
//			if ( normalStoneCount <= 0 && newStoneCount <= 0 )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//					L"������ �������� ��ȭ�Ϸ��� �� ��ȭ���� �ʿ��մϴ�!", m_pNowState );
//
//				return;
//			}
//
//
//		} break;
//
//	default:
//		{
//			return;
//		} break;
//	}
//	
//	m_EnchantItemUID = pItem->GetUID();
//	if ( m_pDLGEnchantItem != NULL )
//	{
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGEnchantItem, NULL, false );
//	}
//
//	m_pDLGEnchantItem = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItem );
//	m_pDLGEnchantItem->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2( 402, 325 ), D3DXVECTOR2( 176, 244 ) ) );
//
//
//	// ��ȭ ���
//	CKTDGUIStatic* pStaticEnchantED = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_comment6" );
//	if ( pStaticEnchantED != NULL && pStaticEnchantED->GetString(0) != NULL )
//	{
//		pStaticEnchantED->GetString(0)->msg = g_pMain->GetEDString( wszEnchantED );  
//	}
//
//
//	// ��ȭ ��� ������ �̸�
//	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"Static_comment4" );
//	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItem->GetItemTemplet()->GetFullName().c_str();
//		pStaticItemName->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//	// ��ȭ ��� ������ �׸�
//	CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"g_pStatic_item_upgrade_Slot" );
//	if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
//	{
//		if ( pItem->GetItemTemplet()->m_ShopImage.empty() == false )
//			pStaticSlot->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//		else
//			pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//	}
//
//	// ��ȭ ��� ������ ��ȭ ���� ǥ��
//	CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItem->GetControl( L"g_pStatic_item_upgrade_Num" );
//	if ( pStaticNum != NULL )
//	{
//		for ( int i = 0; i <= MAX_ENCHANT_LEVEL; i++ )
//		{
//			if ( pStaticNum->GetPicture( i ) != NULL )
//			{
//				pStaticNum->GetPicture( i )->SetShow( false );
//			}
//		}
//
//		if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//		{
//			pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//		}
//	}
//
//
//	// �ش��ϴ� ��ȭ ��ư�� Ȱ��ȭ ���ش�
//	UpdateEnchantWindow();
//
//}
//
//
//void CX2CharacterRoom::OpenEnchantResultWindow( KEGS_ENCHANT_ITEM_ACK& kEGS_ENCHANT_ITEM_ACK )
//{
//	//������Ʈ ���ֱ�����..
//	//������ üũ�ؼ�.. ���׷��̵� �ƴ���, �ʱ�ȭ �ƴ���, ��ȭ������, �ٿ�ƴ���, ���������..
//
//	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEGS_ENCHANT_ITEM_ACK.m_iED;
//
//	ResetCharRoomEDnCashUInVP();
//
//	UpdateInventorySlotList( kEGS_ENCHANT_ITEM_ACK.m_vecInventorySlotInfo );
//
//	SAFE_DELETE_DIALOG( m_pDLGEnchantItemResult );
//
//	//{{ 2009.01.19 ���¿� : �ڵ�����
//	switch( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult )
//	{
//	//��ȭ ����. 
//	case NetError::ERR_ENCHANT_RESULT_00:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Success.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Success.lua" );
//
//			//��ȭ ������ ��� ��ȭ �ܰ� �����ְ�..
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//			if ( pItem != NULL )
//			{
//				CKTDGUIStatic* pStaticComment = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_comment1" );
//				if ( pStaticComment != NULL && pStaticComment->GetString(0) != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"���ϵ帳�ϴ�! ������ ��ȭ�� �����Ͽ�\n      ��ȭ���� " << pItem->GetItemData().m_EnchantLevel << L"�� �Ǿ����ϴ�!";
//					pStaticComment->GetString(0)->msg = wstrstm.str().c_str();
//				}
//
//				CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_upgrade_Num_Success" );
//				if ( pStaticNum != NULL )
//				{
//					for ( int i = 0; i < MAX_ENCHANT_LEVEL+1; i++ )
//					{
//						if ( pStaticNum->GetPicture( i ) != NULL )
//						{
//							pStaticNum->GetPicture( i )->SetShow( false );
//						}
//					}
//
//					if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//					{
//						pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//					}
//				}
//			}
//
//
//		} break;
//	//��ȭ ����.
//	case NetError::ERR_ENCHANT_RESULT_01:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_NoChange.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_NoChange.lua" );
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//			if ( pItem != NULL )
//			{
//				CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_upgrade_Num_NoChange" );
//				if ( pStaticNum != NULL )
//				{
//					for ( int i = 0; i < MAX_ENCHANT_LEVEL+1; i++ )
//					{
//						if ( pStaticNum->GetPicture( i ) != NULL )
//						{
//							pStaticNum->GetPicture( i )->SetShow( false );
//						}
//					}
//
//					if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//					{
//						pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//					}
//				}
//			}
//			
//		} break;
//	//1�ܰ� �϶�.
//	case NetError::ERR_ENCHANT_RESULT_02:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_DownGrade.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_Down.lua" );
//
//			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//			if ( pItem != NULL )
//			{
//				CKTDGUIStatic* pStaticComment = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"Static_comment1" );
//				if ( pStaticComment != NULL && pStaticComment->GetString(0) != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"�Ʊ��׿�! \n��ȭ�� �����Ͽ� ��ȭ ������ ";
//					wstrstm << pItem->GetItemData().m_EnchantLevel << L"�� �Ǿ����ϴ�\n   ������ �� ū ���ذ� ��� �����̿���.";
//
//					pStaticComment->GetString(0)->msg = wstrstm.str().c_str();
//				}
//
//				CKTDGUIStatic* pStaticNum = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"g_pStatic_item_upgrade_Num_Down" );
//				if ( pStaticNum != NULL )
//				{
//					for ( int i = 0; i < MAX_ENCHANT_LEVEL+1; i++ )
//					{
//						if ( pStaticNum->GetPicture( i ) != NULL )
//						{
//							pStaticNum->GetPicture( i )->SetShow( false );
//						}
//					}
//
//					if ( pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel ) != NULL )
//					{
//						pStaticNum->GetPicture( pItem->GetItemData().m_EnchantLevel )->SetShow( true );
//					}
//				}
//			}
//		} break;
//	//��ȭ �ܰ� �ʱ�ȭ.
//	case NetError::ERR_ENCHANT_RESULT_03:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Initial.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_Initial.lua" );
//		} break;
//	//������ ����.
//	case NetError::ERR_ENCHANT_RESULT_04:
//		{
//			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ItemUpgrade_Disappear.ogg", false, false );
//
//			m_pDLGEnchantItemResult = new CKTDGUIDialog( m_pNowState, L"DLG_Enchant_Item_Result_Fail_Broken.lua" );
//		} break;
//	default:
//		{
//			// ���� �߸��ƴ�
//			ASSERT( !"Unexpected EnchantResult Neterror" );
//		} break;
//	}//}} elseif -> switch
//
//	if ( m_pDLGEnchantItemResult != NULL )
//	{
//		CKTDGUIControl* pControl = (CKTDGUIControl*)m_pDLGEnchantItemResult->GetControl( L"Enchant_OK_Button" );
//		if ( pControl != NULL )
//		{
//			pControl->RequestFocus();
//		}
//
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGEnchantItemResult );
//
//		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemID );
//		if ( pItemTemplet != NULL )
//		{
//			//���� �̹��� ǥ�� ���ֱ�.
//			CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"ItemSlot" );
//			if ( pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL )
//			{
//
//				if ( pItemTemplet->m_ShopImage.empty() == false )
//					pStaticSlot->GetPicture(0)->SetTex( pItemTemplet->m_ShopImage.c_str() );
//				else
//					pStaticSlot->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGEnchantItemResult->GetControl( L"ItemName" );
//			if ( pStaticName != NULL && pStaticName->GetString(0) != NULL )
//			{
//				if ( kEGS_ENCHANT_ITEM_ACK.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
//				{
//					pStaticName->GetString(0)->msg = pItemTemplet->GetFullName();
//				}
//				else
//				{
//					CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( kEGS_ENCHANT_ITEM_ACK.m_iEnchantedItemUID );
//					if ( pItem != NULL )
//					{
//						wstringstream wstrstm;
//						wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItemTemplet->GetFullName().c_str();
//						pStaticName->GetString(0)->msg = wstrstm.str().c_str();
//					}
//					else
//					{
//						pStaticName->GetString(0)->msg = pItemTemplet->GetFullName();
//					}	
//				}
//			}
//
//		}
//
//	}
//}
//
//void CX2CharacterRoom::OpenSocketWindow( CX2SlotItem* pSlot )
//{
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"�������� �������� ���� �߰��� �� �����ϴ�.", m_pNowState );
//
//		return;
//	}
//
//
//	if ( g_pData->GetSocketItem()->GetIsPossibleSocketItemByOnlyItemType( pSlot->GetItemTID() ) == false )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"��� �����۸� ���� �߰��� �� �ֽ��ϴ�.", m_pNowState );
//
//		return;
//	}
//
//	int normalStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_MAGIC_STONE_ITEM_ID );
//	int specialStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( SPECIAL_MAGIC_STONE_ITEM_ID );
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlot->GetItemUID() );
//
//#ifdef ITEM_RECOVERY_TEST
//	// ��ȭ ���� �˻� (���Ҵ��ΰ�?)
//	if(true == pItem->IsDisabled())
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �Ұ� ������ �������Դϴ�.\n[�ູ ���� ������ �ֹ���] �������� ����ؼ�\n��� �Ұ� ���¸� �����·� ������ �� �ֽ��ϴ�.", m_pNowState );
//		return; 
//	}
//
//#endif
//
//	bool bCheckHaveOption = false;
//	if ( pItem != NULL )
//	{
//		for ( int i = 0; i < (int)pItem->GetItemData().m_SocketOption.size(); i++ )
//		{
//			int socketOption = pItem->GetItemData().m_SocketOption[i];
//			if ( socketOption != 0 )
//				bCheckHaveOption = true;
//		}
//	}
//	
//	if ( normalStone <= 0 && specialStone <= 0 && bCheckHaveOption == false )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"�ش� �������� �����Ϸ��� �������� �ʿ��մϴ�!", m_pNowState );
//		return;
//	}
//
//	if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
//	{
//		int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();
//		if ( slotNum == 0 )
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"�������� ������ ���� ������ �����ϴ�!", m_pNowState );
//			
//			return;
//		}
//
//		m_SocketItemUID = pItem->GetUID();
//
//		m_pDLGSocketItem = new CKTDGUIDialog( m_pNowState, L"DLG_Socket_Item.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItem );
//
//		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGSocketItem->GetControl(L"g_pCheckBoxForceSocket"); 
//		if ( pCheckBox != NULL )
//		{
//			if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
//			{
//				pCheckBox->SetShow( true );
//				pCheckBox->SetEnable( true );
//			}
//			else
//			{
//				pCheckBox->SetShow( false );
//				pCheckBox->SetEnable( false );
//			}
//		}
//
//		
//
//		ResetSocketWindow();
//	}
//}
//
//void CX2CharacterRoom::ResetSocketWindow()
//{
//	if ( m_pDLGSocketItem == NULL )
//		return;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SocketItemUID ); 
//
//	if ( pItem == NULL )
//		return;
//
//	//������ �̸��ϰ� ������ ���� �̹��� ǥ��
//	CKTDGUIStatic* pStaticSlotImage = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_item_Slot_Image" );
//	if ( pStaticSlotImage != NULL && pStaticSlotImage->GetPicture(0) != NULL )
//	{
//		pStaticSlotImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//	}
//
//	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"Static_comment4" );
//	if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
//	{
//		pStaticItemName->GetString(0)->msg = pItem->GetFullName();
//	}
//	
//
//	int slotNum = pItem->GetItemTemplet()->GetSocketSlotNum();
//
//	int normalStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( NORMAL_MAGIC_STONE_ITEM_ID );
//	int specialStone = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( SPECIAL_MAGIC_STONE_ITEM_ID );
//
//	
//	CKTDGUIStatic* pStaticMagicStoneNum = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_Normal_Magic_Stone_Num" );
//	if ( pStaticMagicStoneNum != NULL && pStaticMagicStoneNum->GetString(0) != NULL )
//	{
//		wstringstream wstrstm;
//		wstrstm << normalStone;
//		pStaticMagicStoneNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//	CKTDGUIStatic* pStaticRareStoneNum = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( L"g_pStatic_Rare_Magic_Stone_Num" );
//	if ( pStaticRareStoneNum != NULL && pStaticRareStoneNum->GetString(0) != NULL )
//	{
//		wstringstream wstrstm;
//		wstrstm << specialStone;
//		pStaticRareStoneNum->GetString(0)->msg = wstrstm.str().c_str();
//	}
//
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Lower_Back_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			if ( slotNum == i + 1 )
//			{
//				pControl->SetShow( true );
//			}
//			else
//				pControl->SetShow( false );
//		}
//			
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Gray_Back_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			if ( slotNum >= i + 1 )
//			{
//				pControl->SetShow( true );
//			}
//			else
//				pControl->SetShow( false );
//		}
//	}
//
//	//������ ǥ�� ���ֱ� ���� �ϴ� ���δ� �ʱ�ȭ ���ְ�.. �ؿ� �ʿ��� �͸� ��������.
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Empty_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Jewely_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShow( false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShowEnable( false, false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShowEnable( false, false );
//		}
//	}
//
//	for ( int i = 0; i < MAX_SOCKET_SLOT_NUM; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pButton_Remove_Stone_Slot";
//		wstrstm << i + 1;
//		CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//		if ( pControl != NULL )
//		{
//			pControl->SetShowEnable( false, false );
//		}
//	}
//
//
//	CKTDGUIStatic* pStatic_LastExtraDamage = NULL;
//	
//
//
//	for ( int i = 0; i < slotNum; i++ )
//	{
//		bool bCheckEmpty = true;
//
//		if ( i < (int)pItem->GetItemData().m_SocketOption.size() )
//		{
//			//0���� ä���� ������ ����ִ°Ű� �ٸ������� ä���� �ִ°Ÿ� �ִ°Ŵ�..
//			int socketOption = pItem->GetItemData().m_SocketOption[i];
//			if ( socketOption != 0 )
//			{
//				bCheckEmpty = false;
//
//				wstringstream wstrstm;
//				wstrstm << L"g_pStatic_Jewely_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShow( true );
//				}
//
//				wstrstm.str( L"" );
//				wstrstm << L"g_pButton_Remove_Stone_Slot";
//				wstrstm << i + 1;
//				pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//				
//				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
//				if ( pSocketData != NULL )
//				{
//					//����ٰ�.. ���� �ɼ� ���� �����ͼ� ���� ¥������ ����¥������ ����.. ������..
//
//					bool bCheckTwoLine = false;
//
//					wstring socketDesc = g_pMain->GetStrByLienBreak( pSocketData->GetSocketDesc().c_str(), 175, XUF_DODUM_13_SEMIBOLD );
//					if ( socketDesc.find( L"\n") != -1 )
//						bCheckTwoLine = true;
//
//					if ( bCheckTwoLine == false )
//					{
//						wstringstream wstrstm;
//						wstrstm << L"g_pStatic_One_Line_Option_Desc_Slot";
//						wstrstm << i + 1;
//						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
//						{
//							pControlStatic->GetString(0)->msg = socketDesc;
//							pControlStatic->SetShow( true );
//							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
//
//							if( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType && 
//								pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
//							{
//								if( NULL != pStatic_LastExtraDamage )
//								{
//									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
//								}
//								pStatic_LastExtraDamage = pControlStatic;
//							}
//						}
//					}
//					else
//					{
//						wstringstream wstrstm;
//						wstrstm << L"g_pStatic_Two_Line_Option_Desc_Slot";
//						wstrstm << i + 1;
//						CKTDGUIStatic* pControlStatic = (CKTDGUIStatic*)m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//						if ( pControlStatic != NULL && pControlStatic->GetString(0) != NULL )
//						{
//							pControlStatic->GetString(0)->msg = socketDesc;
//							pControlStatic->SetShow( true );
//							pControlStatic->SetColor( D3DXCOLOR( 1,1,1,1 ) );
//
//							if( CX2DamageManager::EDT_NONE != pSocketData->m_SocketExtraDamage.m_ExtraDamageType && 
//								pSocketData->m_SocketExtraDamage.m_fRate > 0.f )
//							{
//								if( NULL != pStatic_LastExtraDamage )
//								{
//									pStatic_LastExtraDamage->SetColor( D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.f ) );
//								}
//								pStatic_LastExtraDamage = pControlStatic;
//							}
//						}
//					}			
//				}
//
//
//				wstringstream wstrstmRemove_Stone_Slot;
//				wstrstmRemove_Stone_Slot << L"g_pButton_Remove_Stone_Slot";
//				wstrstmRemove_Stone_Slot << i + 1;
//				pControl = m_pDLGSocketItem->GetControl( wstrstmRemove_Stone_Slot.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//
//				//���� ǥ��
//				//���� ���� ���������� �Ұ����� ���������� �Ұ�����?
//				//�Ϲ� ������, ��� ������ ǥ��
//			}
//			
//		}
//
//		if ( bCheckEmpty == true )
//		{
//			//����ִ°͵�..
//			wstringstream wstrstm;
//			wstrstm << L"g_pStatic_Empty_Slot";
//			wstrstm << i + 1;
//			CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//			if ( pControl != NULL )
//			{
//				pControl->SetShow( true );
//			}
//
//			if ( normalStone > 0 )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//			}
//			else
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Normal_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, false );
//				}
//			}
//
//			if ( specialStone > 0 )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, true );
//				}
//			}	
//			else
//			{
//				wstringstream wstrstm;
//				wstrstm << L"g_pButton_Rare_Magic_Stone_Slot";
//				wstrstm << i + 1;
//				CKTDGUIControl* pControl = m_pDLGSocketItem->GetControl( wstrstm.str().c_str() );
//				if ( pControl != NULL )
//				{
//					pControl->SetShowEnable( true, false );
//				}
//			}
//		}
//	}
//}
//
////
////void CX2CharacterRoom::EmptySkillSlotUI()
////{
////	if( NULL == m_pDLGSkillSlot )
////		return;
////
////	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSkillSlot->GetControl( L"Static_Skill_Slot" );
////	if( NULL == pStatic )
////		return;
////
////	pStatic->GetPicture(0)->SetShow( false );
////	pStatic->GetPicture(1)->SetShow( false );
////	pStatic->GetPicture(2)->SetShow( false );
////
////}
//
//void CX2CharacterRoom::OpenSocketCheatWindow()
//{
//	CloseSocketCheatWindow();
//	m_pDLGSocketItemCheat = new CKTDGUIDialog( m_pNowState, L"DLG_PVP_Lobby_Password_Check.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSocketItemCheat );
//	CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGSocketItemCheat->GetControl( L"Ok" );
//	if ( pButton != NULL )
//	{
//		pButton->SetCustomMsgMouseUp( (int)CX2CharacterRoom::CRM_SOCKET_ITEM_FEE_CHECK_OK );
//	}
//
//	pButton = (CKTDGUIButton*)m_pDLGSocketItemCheat->GetControl( L"Cancel" );
//	if ( pButton != NULL )
//	{
//		pButton->SetShow( false );
//		pButton->SetEnable( false );
//	}
//
//	CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*)m_pDLGSocketItemCheat->GetControl( L"EditBoxPassword" );
//	if ( pEditBox != NULL )
//	{
//		pEditBox->SetCustomMsgEnter( CRM_SOCKET_ITEM_FEE_CHECK_OK );
//	}
//	
//}
//
//void CX2CharacterRoom::CloseSocketCheatWindow()
//{
//	if ( m_pDLGSocketItemCheat != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSocketItemCheat, NULL, false );
//
//	m_pDLGSocketItemCheat = NULL;
//}
//
//
//
//void CX2CharacterRoom::OpenExchangeOldItemWindow( CX2SlotItem* pSlot )
//{
//	if( NULL == pSlot )
//		return; 
//
//
//	if( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"�������� �������� ��ȯ �� �� �����ϴ�.", m_pNowState );
//		return;
//	}
//
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//
//	if( NULL == pItem ||
//		NULL == pItem->GetItemTemplet() )
//		return;
//
//
//
//
//	// ���� ��� �۸� ��ȯ����̴�
//	if( CX2Item::IT_WEAPON != pItem->GetItemTemplet()->m_ItemType &&
//		CX2Item::IT_DEFENCE != pItem->GetItemTemplet()->m_ItemType )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"�� �������� ��ȯ ����� �ƴմϴ�.", m_pNowState );
//		return;
//	}
//
//	if( false == g_pData->GetItemManager()->IsCanItemExchange( pItem->GetItemTemplet()->m_ItemID ) )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"�� �������� ��ȯ ����� �ƴմϴ�.", m_pNowState );
//		return;
//	}
//
//	m_TempItemUID = pSlot->GetItemUID();
//	//const WCHAR msg[] = L"������ �������� ������ ����� ���ο� ���������� ��ü�ϰ� �ֽ��ϴ�.\n������ ����� ������ �� �������� �Ѱ����� ���� �˴ϴ�.\n#CFF0000����! �ѹ� ��ü�� �������� �ٽ� �ǵ��� �� �����ϴ�.#CX";
//	const WCHAR msg[] = L"������ ����� ������ �� �ϳ��� ���� �˴ϴ�.\n#CFF0000����! �ѹ� ��ü�� ��������#CX\n#CFF0000�ٽ� �ǵ��� �� �����ϴ�.#CX";
//	g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), msg, CRM_EXCHANGE_OLD_ITEM_OK, m_pNowState );
//
//}
//
//
//void CX2CharacterRoom::OpenAtrribEnchantWindow( CX2SlotItem* pSlot )
//{
//	if ( pSlot->GetSlotType() == CX2Slot::ST_EQUIPPED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"�������� �������� �Ӽ� ��ȭ�� �� �����ϴ�.", m_pNowState );
//
//		return;
//	}
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( ((CX2SlotItem*)pSlot)->GetSortType(), 
//		((CX2SlotItem*)pSlot)->GetSlotID() );
//
//	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//			L"�������� ã�� �� �����ϴ�!!", m_pNowState );
//
//		return;
//	}
//
//
//	CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
//	if ( pItemTemplet != NULL )
//	{
//		if ( ( pItemTemplet->m_ItemType == CX2Item::IT_WEAPON ||
//			pItemTemplet->m_ItemType == CX2Item::IT_DEFENCE ) && pItemTemplet->m_bFashion == false )
//		{
//
//		}
//		else
//		{
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), 
//				L"��� �����۸� �Ӽ� ��ȭ�� �� �ֽ��ϴ�.", m_pNowState );
//
//			return;
//		}
//	}
//
//
//#ifdef ITEM_RECOVERY_TEST
//	// (���Ҵ��ΰ�?)
//	if(true == pItem->IsDisabled())
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"��� �Ұ� ������ �������Դϴ�.\n[�ູ ���� ������ �ֹ���] �������� ����ؼ�\n��� �Ұ� ���¸� �����·� ������ �� �ֽ��ϴ�.", m_pNowState );
//		return; 
//	}
//#endif
//
//	if ( pItem != NULL && pItem->GetItemTemplet() != NULL )
//	{
//		m_AttribEnchantItemUID = pItem->GetUID();
//
//		SAFE_DELETE_DIALOG( m_pDLGAttribEnchantItem );
//		if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//		{
//			m_pDLGAttribEnchantItem = new CKTDGUIDialog( m_pNowState, L"DLG_Attribute_Item_Weapon.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribEnchantItem );
//		}
//		else if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_DEFENCE )
//		{
//			m_pDLGAttribEnchantItem = new CKTDGUIDialog( m_pNowState, L"DLG_Attribute_Item_Armor.lua" );
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttribEnchantItem );
//		}
//	
//		/*
//		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGAttribEnchantItem->GetControl(L"g_pCheckBoxForceSocket"); 
//		if ( pCheckBox != NULL )
//		{
//			if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
//			{
//				pCheckBox->SetShow( true );
//				pCheckBox->SetEnable( true );
//			}
//			else
//			{
//				pCheckBox->SetShow( false );
//				pCheckBox->SetEnable( false );
//			}
//		}
//		*/
//
//
//
//		ResetAtrribEnchantWindow();
//	}
//
//
//}
//
//void CX2CharacterRoom::ResetAtrribEnchantWindow()
//{
//	if ( m_pDLGAttribEnchantItem == NULL )
//		return;
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_AttribEnchantItemUID ); 
//
//	if ( pItem == NULL )
//		return;
//
//
//	if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_WEAPON )
//	{
//		//���� �̹��� �������ְ�..
//		CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_item_Slot_Image" );
//		if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
//		{
//			pStaticItemImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//		}
//
//		//�̸� �������ְ�..
//		CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Name" );
//		if ( pStaticItemName != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItem->GetItemTemplet()->GetFullName().c_str();
//
//			pStaticItemName->SetString( 0, wstrstm.str().c_str() );
//		}
//
//		//��� �Ӽ�����.. ���� ������ � �ʿ�����..
//		CKTDGUIStatic* pStaticUpgradeNotice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Upgrade_Notice" );
//		if ( pStaticUpgradeNotice != NULL )
//		{
//			pStaticUpgradeNotice->SetString(0, L"");
//			pStaticUpgradeNotice->SetString(1, L"");
//		}
//
//		//�Ӽ� �̸� ���� ���ְ�..
//		CKTDGUIStatic* pStaticAttribName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Attribute" );
//		if ( pStaticAttribName != NULL )
//		{
//			vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//			if( 0 != pItem->GetItemData().m_EnchantOption1 )
//				vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 );
//
//			if( 0 != pItem->GetItemData().m_EnchantOption2 )
//				vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption2 );
//			
//			pStaticAttribName->SetString( 0, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) ).c_str() );
//		}
//
//		//��� �Ӽ�����.. ���� ������ � �ʿ�����.. �ƴϸ� �� �̻� �Ӽ� ��æƮ�� �� �� ������.. ���..
//		CKTDGUIStatic* pStatic_First_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_First_Attribute_Notice" );
//		if ( pStatic_First_Attribute_Notice != NULL )
//			pStatic_First_Attribute_Notice->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Random_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Random_Attribute_Notice" );
//		if ( pStatic_Random_Attribute_Notice != NULL )
//			pStatic_Random_Attribute_Notice->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Enable_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Enable_Notice" );
//		if ( pStatic_Enable_Notice != NULL )
//		{
//			pStatic_Enable_Notice->SetShow( false );
//
//			for ( int i = 0; i <= 6; i++ )
//			{
//				pStatic_Enable_Notice->SetString( i, L"�Ұ���" );
//			}
//		}
//
//		CKTDGUIStatic* pStatic_Two_Line_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Two_Line_Attribute_Notice" );
//		if ( pStatic_Two_Line_Attribute_Notice != NULL )
//			pStatic_Two_Line_Attribute_Notice->SetShow( false );
//
//
//		CKTDGUIButton* pButtonSlotBig = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//		if ( pButtonSlotBig != NULL )
//		{
//			pButtonSlotBig->SetShowEnable( false, false );
//		}
//
//		CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//		if ( pStaticElPieceEx != NULL )
//			pStaticElPieceEx->SetShow( false );
//
//		wstringstream wstrstm2;
//		for ( int i = 1; i <= 6; i++ )
//		{
//			wstrstm2.str( L"" );
//			wstrstm2 << L"g_pButton_El_Slot" << i;
//
//			CKTDGUIButton* pButtonSlotElSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm2.str().c_str() );
//			if ( pButtonSlotElSlot != NULL )
//				pButtonSlotElSlot->SetShowEnable( false, false );
//
//		}
//
//		
//
//		//�ƹ��� �Ӽ��� ���� ���
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//		{
//			//��ư.. ����~~!!
//			//���� ������ 20���� �ʿ�!!
//
//			if ( pStatic_Random_Attribute_Notice != NULL )
//				pStatic_Random_Attribute_Notice->SetShow( true );
//
//			CKTDGUIButton* pButtonSlot = NULL;
//
//			//
//			pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//			if ( pButtonSlot != NULL )
//			{
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//																					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//			}
//
//
//			wstringstream wstrstm;
//			for ( int i = 1; i <= 6; i++ )
//			{
//				wstrstm.str( L"" );
//				wstrstm << L"g_pButton_El_Slot" << i;
//
//				pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );
//
//				int itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
//
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//				else
//				{
//					pButtonSlot->SetShowEnable( false, false );
//				}
//
//
//				pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetToolTip( (CX2DamageManager::EXTRA_DAMAGE_TYPE)( (int)CX2DamageManager::EDT_ENCHANT_BLAZE + i - 1 ), NULL ).c_str()  );
//			}
//
//			CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//			if ( pStaticElPieceEx != NULL )
//			{
//				pStaticElPieceEx->SetShow( true );
//
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//
//				for ( int i = 0; i <= 6; i++ )
//				{
//					wstrstm.str( L"" );
//					int itemNum = 0;
//					if ( i == 0 )
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//					}
//					else
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i ) );
//					}
//
//					wstrstm << L"���� ����";
//
//					//{{ 2009.01.19 ���¿� : �ڵ�����
//					switch(i)
//					{
//					case 0:
//						{
//							wstrstm << L"(�Ҹ�)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 1:
//						{
//							wstrstm << L"(����)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 2:
//						{
//							wstrstm << L"(���)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 3:
//						{
//							wstrstm << L"(�׸�)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 4:
//						{
//							wstrstm << L"(����)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 5:
//						{
//							wstrstm << L"(����Ʈ)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 6:
//						{
//							wstrstm << L"(��ũ)\nX " << itemNum << L" (" << needItemNum << L")";
//						}break;
//					default:
//						{
//							ASSERT(!"Invalid Attribute stone" );
//						}
//						break;
//					}//}} elseif -> switch
//					
//					pStaticElPieceEx->SetString( i, wstrstm.str().c_str() );
//				}
//			}
//
//			
//			if ( pStatic_First_Attribute_Notice != NULL )
//				pStatic_First_Attribute_Notice->SetShow( true );
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				wstringstream wstrstm;
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				wstrstm << L"���� ���� " << needItemNum << L"�� �ʿ�!";
//				pStaticUpgradeNotice->SetString(1, wstrstm.str().c_str() );
//
//			}
//		}
//		//�ΰ��� �ɼ��� ���� �پ��ִ� ���
//		else if ( pItem->GetItemData().m_EnchantOption1 != 0 && pItem->GetItemData().m_EnchantOption2 != 0 )
//		{
//			if ( pStatic_Enable_Notice != NULL )
//				pStatic_Enable_Notice->SetShow( true );
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				pStaticUpgradeNotice->SetString(0, L"��� �Ӽ�");
//			}
//		}
//		//���� ������ �Ѱ��� �پ��ִ� ���..
//		else
//		{
//			if ( pStatic_Random_Attribute_Notice != NULL )
//				pStatic_Random_Attribute_Notice->SetShow( true );
//
//			//���� ������ 60���� �ʿ�!!
//			int existingOption = 0;
//			if ( pItem->GetItemData().m_EnchantOption1 != 0 )
//				existingOption = pItem->GetItemData().m_EnchantOption1;
//
//			if ( pItem->GetItemData().m_EnchantOption2 != 0 )
//				existingOption = pItem->GetItemData().m_EnchantOption2;
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				wstringstream wstrstm;
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				wstrstm << L"���� ���� " << needItemNum << L"�� �ʿ�!";
//				pStaticUpgradeNotice->SetString(1, wstrstm.str().c_str() );
//
//			}
//
//			//���� �Ұ��� �Ѱ��ϰ�.. ���� �� �� �ִ°��ϰ� �ɵ����̰� ���̰Բ� ��������ٵ�..
//			//��� �ϸ� ������ ����������?
//
//			
//
//			//���� �ɼ� �κк��� ó���ϰ�..
//
//			CKTDGUIButton* pButtonSlot = NULL;
//
//			//
//			pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//			if ( pButtonSlot != NULL )
//			{
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//			}
//
//
//
//			//���� �������.. ó���غ���..
//
//			if ( pStatic_Two_Line_Attribute_Notice != NULL )
//			{
//				pStatic_Two_Line_Attribute_Notice->SetShow( true );
//
//				//���� ���� �Է� ������ �غ���.. �̳��� �����ϳ���.. ���Ҳ� ����
//
//				
//				if ( pStatic_Enable_Notice != NULL )
//				{
//					pStatic_Enable_Notice->SetShow( true );
//
//					if ( pStatic_Enable_Notice != NULL )
//					{
//						pStatic_Enable_Notice->SetString( 0, L"" );
//					}
//				}
//
//				CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//				if ( pStaticElPieceEx != NULL )
//				{
//					pStaticElPieceEx->SetShow( true );
//					wstringstream wstrstm;
//
//					int needItemNum = 0;
//					g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//						pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//
//					for ( int i = 0; i <= 6; i++ )
//					{
//						wstrstm.str( L"" );
//
//						int itemNum = 0;
//						if ( i == 0 )
//						{
//							itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//						}
//						else
//						{
//							itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i ) );
//						}
//
//						wstrstm << L"���� ����";
//
//						//{{ 2009.01.19 ���¿� : �ڵ�����
//						switch(i)
//						{
//						case 0:
//							{
//								wstrstm << L"(�Ҹ�)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 1:
//							{
//								wstrstm << L"(����)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 2:
//							{
//								wstrstm << L"(���)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 3:
//							{
//								wstrstm << L"(�׸�)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 4:
//							{
//								wstrstm << L"(����)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 5:
//							{
//								wstrstm << L"(����Ʈ)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						case 6:
//							{
//								wstrstm << L"(��ũ)\nX " << itemNum << L" (" << needItemNum << L")";
//							} break;
//						default:
//							{
//                                ASSERT( !"Invalid Attribute stone" );
//							} break;
//						}//}} elseif -> switch
//
//						if ( g_pData->GetEnchantItem()->IsPossibleToPush( (CX2EnchantItem::ENCHANT_TYPE)existingOption, (CX2EnchantItem::ENCHANT_TYPE)i ) == true )
//						{
//							pStaticElPieceEx->SetString( i, wstrstm.str().c_str() );
//						}
//						else
//						{
//							pStaticElPieceEx->SetString( i, L"" );
//						}
//					}
//				}
//				
//			
//				wstringstream wstrstm;
//				for ( int i = 1; i <= 6; i++ )
//				{	
//					wstrstm.str( L"" );
//					wstrstm << L"g_pButton_El_Slot" << i;
//
//					pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );
//
//
//					if ( g_pData->GetEnchantItem()->IsPossibleToPush( (CX2EnchantItem::ENCHANT_TYPE)existingOption, (CX2EnchantItem::ENCHANT_TYPE)i ) == true )
//					{
//						int itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
//
//						int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
//						if ( pButtonSlot != NULL )
//						{
//							int needItemNum = 0;
//							g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( true, true, pItem->GetItemTemplet()->m_UseLevel,
//								pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//							if ( itemNum >= needItemNum )
//							{
//								pButtonSlot->SetShowEnable( true, true );
//
//								vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//								if( 0 != existingOption )
//									vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) existingOption );
//
//								if( 0 != i )
//									vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) i );
//
//								pButtonSlot->SetGuideDesc( g_pData->GetEnchantItem()->GetToolTip( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ), NULL ).c_str() );
//							}
//							else
//							{
//								pButtonSlot->SetShowEnable( false, false );
//							}
//						}
//						
//
//						vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//						if( 0 != existingOption )
//							vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) existingOption );
//
//						if( 0 != i )
//							vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) i );
//
//						
//						pStatic_Two_Line_Attribute_Notice->SetString( i - 1, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ), true ).c_str() );
//
//						if ( pStatic_Enable_Notice != NULL )
//						{
//							pStatic_Enable_Notice->SetString( i, L"" );
//						}
//					}
//					else
//					{
//						if ( pButtonSlot != NULL )
//							pButtonSlot->SetShowEnable( false, false );
//
//						pStatic_Two_Line_Attribute_Notice->SetString( i - 1, L"" );
//
//						
//					}
//				}
//			
//			}
//			
//		}
//
//
//
//		//�Ӽ� �����ϱ� ��ư�ϰ� � �Ӽ����� �����ִ��� �����������?? 
//		CKTDGUIStatic* pStaticEmptySlot1 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Empty_Slot1" );
//		if ( pStaticEmptySlot1 != NULL )
//			pStaticEmptySlot1->SetShow( false );
//
//		CKTDGUIStatic* pStaticEmptySlot2 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Empty_Slot2" );
//		if ( pStaticEmptySlot2 != NULL )
//			pStaticEmptySlot2->SetShow( false );
//
//		CKTDGUIStatic* pStaticNoticeSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Notice_Slot" );
//		if ( pStaticNoticeSlot != NULL )
//		{
//			pStaticNoticeSlot->SetString( 0, L"" );
//			pStaticNoticeSlot->SetString( 1, L"" );
//		}
//
//		CKTDGUIButton* pButtonRemoveSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_Remove_El_Slot1" );
//		if ( pButtonRemoveSlot != NULL )
//		{
//			pButtonRemoveSlot->SetShowEnable( false, false );
//		}
//
//		CKTDGUIButton* pButtonRemoveSlot2 = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_Remove_El_Slot2" );
//		if ( pButtonRemoveSlot2 != NULL )
//		{
//			pButtonRemoveSlot2->SetShowEnable( false, false );
//		}
//
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 )
//		{
//			if ( pStaticEmptySlot1 != NULL )
//				pStaticEmptySlot1->SetShow( true );
//		}
//		else
//		{
//			if ( pStaticNoticeSlot != NULL )
//			{
//				vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//				if( 0 != pItem->GetItemData().m_EnchantOption1 )
//					vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) pItem->GetItemData().m_EnchantOption1 );
//
//				pStaticNoticeSlot->SetString( 0, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) ).c_str() );
//			}
//
//			if ( pButtonRemoveSlot != NULL )
//				pButtonRemoveSlot->SetShowEnable( true, true );
//		}
//
//		if ( pItem->GetItemData().m_EnchantOption2 == 0 )
//		{
//			if ( pStaticEmptySlot2 != NULL )
//				pStaticEmptySlot2->SetShow( true );
//		}
//		else
//		{
//			if ( pStaticNoticeSlot != NULL )
//			{
//				vector< CX2EnchantItem::ENCHANT_TYPE > vecEnchantType;
//				if( 0 != pItem->GetItemData().m_EnchantOption2 )
//					vecEnchantType.push_back( (CX2EnchantItem::ENCHANT_TYPE) pItem->GetItemData().m_EnchantOption2 );
//
//				pStaticNoticeSlot->SetString( 1, g_pData->GetDamageManager()->GetExtraDamageName( g_pData->GetEnchantItem()->GetExtraDamageType( vecEnchantType ) ).c_str() );
//			}
//
//			if ( pButtonRemoveSlot2 != NULL )	
//				pButtonRemoveSlot2->SetShowEnable( true, true );
//		}
//
//	}
//	else if ( pItem->GetItemTemplet()->m_ItemType == CX2Item::IT_DEFENCE )
//	{
//		
//
//		
//
//		//�̹��� �������ְ�..
//		CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_item_Slot_Image" );
//		if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
//		{
//			pStaticItemImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//		}
//
//		//�̸� �������ְ�..
//		CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Name" );
//		if ( pStaticItemName != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << L"+" << pItem->GetItemData().m_EnchantLevel << L" " << pItem->GetItemTemplet()->GetFullName().c_str();
//
//			pStaticItemName->SetString( 0, wstrstm.str().c_str() );
//		}
//
//		//���� ������ � �ʿ�����..
//		CKTDGUIStatic* pStaticUpgradeNotice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Upgrade_Notice" );
//		if ( pStaticUpgradeNotice != NULL )
//		{
//			pStaticUpgradeNotice->SetString(0, L"");
//		}
//
//
//
//		//�Ӽ� �̸� ���� ���ְ�..
//		CKTDGUIStatic* pStaticAttribName = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"Static_Item_Attribute" );
//		if ( pStaticAttribName != NULL )
//		{
//			
//			pStaticAttribName->SetString( 0, g_pData->GetEnchantItem()->GetEnchantResistName( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 ).c_str() );
//		}
//
//		CKTDGUIStatic* pStaticElPieceEx = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_El_Stone_Num" );
//		if ( pStaticElPieceEx != NULL )
//			pStaticElPieceEx->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Enable_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Enable_Notice" );
//		if ( pStatic_Enable_Notice != NULL )
//			pStatic_Enable_Notice->SetShow( false );
//
//		CKTDGUIStatic* pStatic_Random_Attribute_Notice = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Random_Attribute_Notice" );
//		if ( pStatic_Random_Attribute_Notice != NULL )
//			pStatic_Random_Attribute_Notice->SetShow( false );
//
//		CKTDGUIButton* pButtonRandom = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_El_Slot_Big" );
//		if ( pButtonRandom != NULL )
//			pButtonRandom->SetShowEnable( false, false );
//
//		wstringstream wstrstm2;
//		for ( int i = 1; i <= 6; i++ )
//		{
//			wstrstm2.str( L"" );
//			wstrstm2 << L"g_pButton_El_Slot" << i;
//
//			CKTDGUIButton* pButtonSlotElSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm2.str().c_str() );
//			if ( pButtonSlotElSlot != NULL )
//				pButtonSlotElSlot->SetShowEnable( false, false );
//
//		}
//		
//
//		//�ƹ��� �Ӽ��� ���� ���
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 && pItem->GetItemData().m_EnchantOption2 == 0 )
//		{
//			if ( pStatic_Random_Attribute_Notice != NULL )
//				pStatic_Random_Attribute_Notice->SetShow( true );
//
//			wstringstream wstrstm;
//
//			if ( pStaticUpgradeNotice != NULL )
//			{
//				wstrstm.str( L"" );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				wstrstm << L"���� ���� " << needItemNum << L"�� �ʿ�";
//				pStaticUpgradeNotice->SetString(0, wstrstm.str().c_str() );
//			}
//
//
//			if ( pButtonRandom != NULL )
//			{
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//				if ( itemNum >= needItemNum )
//				{
//					pButtonRandom->SetShowEnable( true, true );
//				}
//			}
//
//
//		
//			for ( int i = 1; i <= 6; i++ )
//			{
//				wstrstm.str( L"" );
//				wstrstm << L"g_pButton_El_Slot" << i;
//
//				CKTDGUIButton* pButtonSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( wstrstm.str().c_str() );
//
//				int itemID = g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i );
//
//				int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( itemID );
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//				if ( itemNum >= needItemNum )
//				{
//					pButtonSlot->SetShowEnable( true, true );
//				}
//				else
//				{
//					pButtonSlot->SetShowEnable( false, false );
//				}
//			}
//
//
//			if ( pStaticElPieceEx != NULL )
//			{
//				pStaticElPieceEx->SetShow( true );
//
//				int needItemNum = 0;
//				g_pData->GetEnchantItem()->GetAttribEnchantRequireCount( false, false, pItem->GetItemTemplet()->m_UseLevel,
//					pItem->GetItemTemplet()->m_ItemGrade, needItemNum );
//
//				for ( int i = 0; i <= 6; i++ )
//				{
//					wstrstm.str( L"" );
//					int itemNum = 0;
//					if ( i == 0 )
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN );
//					}
//					else
//					{
//						itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( g_pData->GetEnchantItem()->GetItemID( (CX2EnchantItem::ENCHANT_TYPE)i ) );
//
//					}
//
//					wstrstm << L"���� ����";
//
//					//{{ 2009.01.19 ���¿� : �ڵ�����
//					switch(i)
//					{
//					case 0:
//						{
//							wstrstm << L"(�Ҹ�)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 1:
//						{
//							wstrstm << L"(����)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 2:
//						{
//							wstrstm << L"(���)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 3:
//						{
//							wstrstm << L"(�׸�)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 4:
//						{
//							wstrstm << L"(����)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 5:
//						{
//							wstrstm << L"(����Ʈ)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					case 6:
//						{
//							wstrstm << L"(��ũ)\nX " << itemNum << L" (" << needItemNum << L")";
//						} break;
//					default:
//						{
//							ASSERT( !"Invaild Attribute stone" );
//						}
//						break;
//					}//}} elseif -> switch
//					
//
//					pStaticElPieceEx->SetString( i, wstrstm.str().c_str() );
//				}
//			}
//		}
//		else
//		{
//			if ( pStatic_Enable_Notice != NULL )
//				pStatic_Enable_Notice->SetShow( true );	
//
//			
//		}
//
//
//
//		//�ؿ� �Ӽ� ��
//
//		//�Ӽ� �����ϱ� ��ư�ϰ� � �Ӽ����� �����ִ��� �����������?? 
//		CKTDGUIStatic* pStaticEmptySlot1 = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Empty_Slot1" );
//		if ( pStaticEmptySlot1 != NULL )
//			pStaticEmptySlot1->SetShow( false );
//
//		
//		CKTDGUIStatic* pStaticNoticeSlot = (CKTDGUIStatic*)m_pDLGAttribEnchantItem->GetControl( L"g_pStatic_Attribute_Notice_Slot" );
//		if ( pStaticNoticeSlot != NULL )
//		{
//			pStaticNoticeSlot->SetString( 0, L"" );
//			
//		}
//
//		CKTDGUIButton* pButtonRemoveSlot = (CKTDGUIButton*)m_pDLGAttribEnchantItem->GetControl( L"g_pButton_Remove_El_Slot1" );
//		if ( pButtonRemoveSlot != NULL )
//		{
//			pButtonRemoveSlot->SetShowEnable( false, false );
//		}
//
//		
//
//		if ( pItem->GetItemData().m_EnchantOption1 == 0 )
//		{
//			if ( pStaticEmptySlot1 != NULL )
//				pStaticEmptySlot1->SetShow( true );
//		}
//		else
//		{
//			if ( pStaticNoticeSlot != NULL )
//			{
//				
//				pStaticNoticeSlot->SetString( 0, g_pData->GetEnchantItem()->GetEnchantResistName( (CX2EnchantItem::ENCHANT_TYPE)pItem->GetItemData().m_EnchantOption1 ).c_str() );
//			}
//
//			if ( pButtonRemoveSlot != NULL )
//				pButtonRemoveSlot->SetShowEnable( true, true );
//		}
//
//	}
//}
//
////�� ����, �ǵ�����
//void CX2CharacterRoom::OpenElChangerWindow( bool bIdentify, CX2Item* pItem )
//{
//	SAFE_DELETE_DIALOG( m_pDLGElChanger );
//
//	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//		return;
//
//	m_ElChangerItemUID		= pItem->GetItemData().m_ItemUID;
//	m_ElChangerItemCount	= 1;
//
//	m_pDLGElChanger = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_El_Changer_Window.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElChanger );
//
//	//������ �̹��� �������ְ�.
//	CKTDGUIStatic* pStaticItemImage = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"ItemImage" );
//	if ( pStaticItemImage != NULL && pStaticItemImage->GetPicture(0) != NULL )
//	{
//		pStaticItemImage->GetPicture(0)->SetTex( pItem->GetItemTemplet()->m_ShopImage.c_str() );
//	}
//
//	//������ �̸� �������ְ�.
//	CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Price" );
//	if ( pStaticItemName != NULL )
//	{
//		pStaticItemName->SetString(0, pItem->GetItemTemplet()->m_Name.c_str() );
//	}
//
//	//�������� �ִ� ���� ��������߰���.. �ϳ� ������..
//	CKTDGUIStatic* pStaticItemNum3 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num3" );
//	if ( pStaticItemNum3 != NULL )
//	{
//		wstringstream wstrstm3;
//		wstrstm3 << pItem->GetItemData().m_Quantity;
//		pStaticItemNum3->SetString(0, wstrstm3.str().c_str() );
//	}
//
//	//������ �ٲ� ���� �������ְ�.
//	CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num" );
//	if ( pStaticItemNum != NULL )
//	{
//		pStaticItemNum->SetString(0, L"1" );
//	}
//
//	//�����ϴ°��� �ǵ����°��� �������ְ�.
//	//�����ϴ°Ÿ� ������.. �ǵ����°Ÿ� ������ ���������� ����������������������������������...
//	if ( bIdentify == true )
//	{
//		CKTDGUIControl* pControl = m_pDLGElChanger->GetControl( L"Identify_Tool" );
//		if ( pControl != NULL )
//			pControl->SetShow( true );
//
//		int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE );
//
//		CKTDGUIStatic* pStaticItemNum2 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num2" );
//	
//		if ( pStaticItemNum2 != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << itemNum << L"��";
//			pStaticItemNum2->SetString( 0, wstrstm.str().c_str() );
//		}
//		
//	}
//	else
//	{
//		CKTDGUIControl* pControl = m_pDLGElChanger->GetControl( L"Identify_Tool2" );
//		if ( pControl != NULL )
//			pControl->SetShow( true );
//
//		int itemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE );
//
//		CKTDGUIStatic* pStaticItemNum2 = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num2" );
//
//		if ( pStaticItemNum2 != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << itemNum << L"��";
//			pStaticItemNum2->SetString( 0, wstrstm.str().c_str() );
//		}
//	}
//}
//
//void CX2CharacterRoom::AdjustElChangerItemCount( const WCHAR* wszAdjustCommand )
//{
//	//before, we must check the type if It is an Identifier or an returner.
//
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_ElChangerItemUID );
//	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
//		return;
//
//	int nowQuantity = pItem->GetItemData().m_Quantity;
//
//	int needStoneQuantity = 0;
//	if ( pItem->GetItemTemplet()->m_ItemID == CX2EnchantItem::ATI_UNKNOWN )
//	{
//		//identifier
//		needStoneQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_IDENTIFY_STONE );
//	}
//	else
//	{
//		//returner
//		needStoneQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_UNKNOWN_STONE );
//	}
//
//	if ( wcscmp( wszAdjustCommand, L"Up10" ) == 0 )
//	{
//		m_ElChangerItemCount += 10;
//
//		if ( m_ElChangerItemCount >= nowQuantity )
//			m_ElChangerItemCount = nowQuantity;
//
//		if ( m_ElChangerItemCount >= needStoneQuantity )
//			m_ElChangerItemCount = needStoneQuantity;
//	}
//	else if ( wcscmp( wszAdjustCommand, L"Down10" ) == 0  )
//	{
//		m_ElChangerItemCount -= 10;
//		if ( m_ElChangerItemCount <= 1 )
//			m_ElChangerItemCount = 1;
//	}
//	else if ( wcscmp( wszAdjustCommand, L"Up1" ) == 0  )
//	{
//		m_ElChangerItemCount += 1;
//
//		if ( m_ElChangerItemCount >= nowQuantity )
//			m_ElChangerItemCount = nowQuantity;
//
//		if ( m_ElChangerItemCount >= needStoneQuantity )
//			m_ElChangerItemCount = needStoneQuantity;
//	}
//	else if ( wcscmp( wszAdjustCommand, L"Down1" ) == 0  )
//	{
//		m_ElChangerItemCount -= 1;
//		if ( m_ElChangerItemCount <= 1 )
//			m_ElChangerItemCount = 1;
//	}
//
//	if ( m_pDLGElChanger != NULL )
//	{
//		//������ �ٲ� ���� �������ְ�.
//		CKTDGUIStatic* pStaticItemNum = (CKTDGUIStatic*)m_pDLGElChanger->GetControl( L"Identify_Window_Num" );
//		if ( pStaticItemNum != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << m_ElChangerItemCount;
//			pStaticItemNum->SetString(0, wstrstm.str().c_str() );
//		}
//	}
//
//	
//
//}
//
//void CX2CharacterRoom::CheckMyInfoChange()
//{
//	if ( m_TotalED < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//	{
//		int changeED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED - m_TotalED;
//		wstringstream wstrstm;
//		wstrstm << L"ED +" << changeED;
//		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 869,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	if ( m_TotalED > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//	{
//		int changeED = m_TotalED - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
//		wstringstream wstrstm;
//		wstrstm << L"ED -" << changeED;
//		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 869,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	m_TotalED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
//
//	if ( m_TotalExp < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP )
//	{
//		int changeEXP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP - m_TotalExp;
//		wstringstream wstrstm;
//		wstrstm << L"EXP +" << changeEXP;
//		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 707,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	if ( m_TotalExp > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP )
//	{
//		int changeEXP = m_TotalExp - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//		wstringstream wstrstm;
//		wstrstm << L"EXP -" << changeEXP;
//		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 707,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//	m_TotalExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
//
//	if ( m_TotalVP < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//	{
//		int changeVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint - m_TotalVP;
//		wstringstream wstrstm;
//		wstrstm << L"VP +" << changeVP;
//		m_pPicCharMenuPlusInfo->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 969,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//
//	if ( m_TotalVP > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//	{
//		int changeVP = m_TotalVP - g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
//		wstringstream wstrstm;
//		wstrstm << L"VP -" << changeVP;
//		m_pPicCharMenuPlusInfoRed->DrawText( wstrstm.str().c_str(), D3DXVECTOR3( 969,721,0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
//	}
//	m_TotalVP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint;
//
//	if ( m_TotalLevel < g_pData->GetSelectUnitLevel() )
//	{
//		for ( int i = 0; i < 3; i++ )
//		{
//			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
//			pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"HyperBallTick", 
//				698, 730, 
//				0.0f, 1000,1000, -1, 10 );
//			if( pSeq != NULL )
//			{
//				pSeq->SetOverUI( true );
//				pSeq->SetBlackHolePosition( pSeq->GetPosition() );
//				pSeq->UseLookPoint( true );
//				pSeq->SetLookPoint( pSeq->GetPosition() );
//			}
//			pSeq = g_pData->GetUIMajorParticle()->CreateSequence( L"HyperBallRing", 
//				698, 730,
//				0.0f, 1000,1000, -1, 1 );
//			if( pSeq != NULL )
//				pSeq->SetOverUI( true );
//		}
//		
//		if( false == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser ) // ü�� ���̵� ����
//		{
//			CNMCOClientObject::GetInstance().ChangeMyLevel( ( (UINT32)g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass << 24 ) | (UINT32)g_pData->GetSelectUnitLevel(), kUserFlag_GeneralLevelUp );
//		}
//	}
//
//	m_TotalLevel = g_pData->GetSelectUnitLevel();
//
//	
//}
//
//void CX2CharacterRoom::ResetInvenPage( int nowPage, int maxPage )
//{
//	m_NowInvenSortTypePageNum = nowPage;
//	m_NowInvenSortTypePageMaxNum = maxPage;
//
//	ResetInvenPageUI();
//}
//
//void CX2CharacterRoom::ResetInvenPageUI()
//{
//	//m_NowInvenSortTypePageMaxNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetInvenSlot() m_NowInventorySortType
//	//int invenMaxSize = GetInvenMaxSize( m_NowInventorySortType );
//
//	if ( m_pDLGMyInfoInventory != NULL )
//	{
//		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"Static_My_Inven_Page" );
//		if ( pStatic != NULL )
//		{
//			wstringstream wstrstm;
//			wstrstm << m_NowInvenSortTypePageNum << L"/" << m_NowInvenSortTypePageMaxNum;
//			pStatic->SetString( 0, wstrstm.str().c_str() );
//		}
//	}
//	
//}
//
//void CX2CharacterRoom::NextInvenPage()
//{
//	if ( m_NowInvenSortTypePageNum >= m_NowInvenSortTypePageMaxNum )
//		return;
//
//	m_NowInvenSortTypePageNum++;
//
//	SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
//}
//
//void CX2CharacterRoom::PrevInvenPage()
//{
//	if ( m_NowInvenSortTypePageNum <= 1 )
//		return;
//
//	m_NowInvenSortTypePageNum--;
//
//	SetInventorySort( m_NowInventorySortType, m_NowInvenSortTypePageNum );
//}
//
//
//void CX2CharacterRoom::CreatLevelupEffect()
//{
//	if( NULL == g_pData->GetUIMajorParticle() )
//		return;
//
//
//	CKTDGXMeshPlayer::CXMeshInstance* pInst = NULL;
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp01_1", 
//		m_pUnitViewerUI->GetMatrix().GetPos().x,
//		m_pUnitViewerUI->GetMatrix().GetPos().y - 25.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().z, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.3f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp01",
//		m_pUnitViewerUI->GetMatrix().GetPos().x,
//		m_pUnitViewerUI->GetMatrix().GetPos().y - 25.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().z, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.3f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp04",
//		m_pUnitViewerUI->GetMatrix().GetPos().x,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z, 
//		0,0,0, 0,0,0 );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02", 
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02", 
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.1f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02",
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.2f );
//	pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"LobbyLevelUp02",
//		m_pUnitViewerUI->GetMatrix().GetPos().x + 5.0f,
//		m_pUnitViewerUI->GetMatrix().GetPos().y,
//		m_pUnitViewerUI->GetMatrix().GetPos().z + 30.0f, 
//		0,0,0, 0,0,0 );
//	pInst->SetDelayTime( 0.3f );
//
//
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUp.ogg", false, false );
//	g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUpBGM.ogg", false, false );
//
//	//m_fPlayBGM = -1;
//
//	
//
//	g_pData->GetUIMajorParticle()->CreateSequence( L"LevelUp", 269, 361, 0, 1000, 1000, 1, 1, 1 );
//	CKTDGParticleSystem::CParticleEventSequence* pPartInst = NULL;
//	pPartInst = g_pData->GetUIMajorParticle()->CreateSequence( L"LobbyLevelUp01", 
//		m_pUnitViewerUI->GetMatrix().GetPos() );
//	pPartInst->SetLatency( 2.266f );
//}
//
//
//void CX2CharacterRoom::SellItemNumChange( int iNum )
//{
//	CX2Item* pSellItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_SellItemUID );
//	if ( pSellItem != NULL )
//	{
//		int maxItemSellNum = pSellItem->GetItemData().m_Quantity;
//		m_SellItemNum += iNum;
//
//		if ( m_SellItemNum <= 1 )
//			m_SellItemNum = 1;
//	
//		if ( m_SellItemNum >= maxItemSellNum )
//			m_SellItemNum = maxItemSellNum;
//
//		WCHAR buff[256] = {0};
//		CX2Item* pItem = pSellItem;
//
//		CKTDGUIStatic* pStaticItemSellED = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"StaticItem_Sell_ED" );
//		if ( pStaticItemSellED != NULL && pStaticItemSellED->GetString(0) != NULL )
//		{
//			int itemSellED = pItem->GetEDToSell();
//			wsprintf( buff, L"%d", itemSellED * m_SellItemNum );
//			pStaticItemSellED->GetString(0)->msg = buff;			
//		}
//
//		CKTDGUIStatic* pStaticItemSellNum = (CKTDGUIStatic*)m_pDLGSelectSellNumWindowPopup->GetControl( L"Item_Num" );
//		if ( pStaticItemSellNum != NULL && pStaticItemSellED->GetString(0) != NULL )
//		{
//			wsprintf( buff, L"%d", m_SellItemNum );
//			pStaticItemSellNum->GetString(0)->msg = buff;
//		}
//	}
//}
//
//void CX2CharacterRoom::DecideSellItemNum( bool bCheck )
//{
//	if ( bCheck == true )	//������ ���
//	{
//		Handler_EGS_SELL_ITEM_REQ();
//	}
//
//	if ( m_pDLGSelectSellNumWindowPopup != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGSelectSellNumWindowPopup, NULL, false );
//
//	m_pDLGSelectSellNumWindowPopup = NULL;
//}
//
//void CX2CharacterRoom::OpenDLGAllEquippingItemRepair()
//{
//	int edToRepair = 0;
//	int vpToRepair = 0;
//	m_vecAllEquippingItem.clear();
//	CX2Inventory* pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
//
//
//	
//	
//
//	for ( int i = 0; i < pInventory->GetItemMaxNum( CX2Inventory::ST_E_EQUIP ); i++ )
//	{
//		CX2Item* pItem = pInventory->GetItem( CX2Inventory::ST_E_EQUIP, i );
//		if ( pItem == NULL || pItem->GetItemTemplet() == NULL || pItem->GetItemTemplet()->m_PeriodType != CX2Item::PT_ENDURANCE )
//			continue;
//
//		edToRepair += pItem->GetEDToRepair();
//		vpToRepair += pItem->GetVPToRepair();
//		m_vecAllEquippingItem.push_back( pItem->GetUID() );
//	}
//
//	float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//	if( fDiscountRate > 0.f )
//	{
//		edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
//	}
//
//
//
//	WCHAR buff[256] = {0};
//	if ( vpToRepair == 0 )
//		wsprintf( buff, L"���� ���� �������� ��� �����ϴµ�\n%d ED�� �Ҹ�˴ϴ�.\n���� �Ͻðڽ��ϱ�?", edToRepair );
//	else
//		wsprintf( buff, L"���� ���� �������� ��� �����ϴµ�\n%d ED�� #CFF0000%d VP#CX�� �Ҹ�˴ϴ�.\n���� �Ͻðڽ��ϱ�?", edToRepair, vpToRepair );
//
//	m_pDLGAllEquippingItemRepair = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), buff, CRM_ALL_EQUIPPING_ITEM_REPAIR_OK, m_pNowState, CRM_ALL_EQUIPPING_ITEM_REPAIR_CANCEL );
//}
//
//void CX2CharacterRoom::AllEquippingItemRepairREQ()
//{
//	if ( m_pDLGAllEquippingItemRepair != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGAllEquippingItemRepair, NULL, false );
//
//	m_pDLGAllEquippingItemRepair = NULL;
//
//	KEGS_REPAIR_ITEM_REQ kPacket;
//	kPacket.m_vecItemUID = m_vecAllEquippingItem;
//
//	int edToRepair = 0;
//	int vpToRepair = 0;
//
//	for ( int i = 0; i < (int)m_vecAllEquippingItem.size(); i++ )
//	{
//		UidType itemUID = m_vecAllEquippingItem[i];
//		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID );
//		if ( pItem == NULL || pItem->GetItemTemplet() == NULL || pItem->GetItemTemplet()->m_PeriodType != CX2Item::PT_ENDURANCE )
//			continue;
//
//		edToRepair += pItem->GetEDToRepair();
//		vpToRepair += pItem->GetVPToRepair();
//	}
//
//
//	float fDiscountRate = g_pData->GetMyUser()->GetSelectUnit()->GetRepairDiscountRate();
//	if( fDiscountRate > 0.f )
//	{
//		edToRepair = (int) ( edToRepair * ( 1.f - fDiscountRate ) );
//	}
//
//
//
//
//	if ( edToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"ED�� �����Ͽ� ������ �� �����ϴ�!", m_pNowState );
//		return;
//	}
//	if ( vpToRepair > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_VSPoint )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"VP�� �����Ͽ� ������ �� �����ϴ�!", m_pNowState );
//		return;
//	}
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_REPAIR_ITEM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_REPAIR_ITEM_ACK );
//
//	
//
//	m_pDLGAllEquippingItemRepair = NULL;
//}
//
//
//
//void CX2CharacterRoom::SetCharacterRoomInventoryState()
//{
//	m_NowCharacterRoomState = CRS_INVENTORY;
//
//	//m_pUnitViewerUI->PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
//	m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING ); 
//	m_pUnitViewerUI->SetShowObject( true );
//
//	CX2UnitViewerUI::SetUnitClicked( false );
//	m_fMouseSensitivity = 0.01f;
//	m_fRemainRotY = 0.0f;
//	m_bRotateReturn = true;
//
//	m_pDLGMyInfoInventory->SetEnable( true );
//	m_pDLGMyInfoInventory->SetShow( true );
//	m_pUnitViewerUI->SetShowObject( true );
//
//	m_pDLGMyInfoQuickSlotNumber->SetEnable( true );
//	m_pDLGMyInfoQuickSlotNumber->SetShow( true );
//
//	m_pDLGMyInfoFront->SetShow( true );
//	m_pDLGMyInfoFront->SetEnable( true );
//
//	m_pDLGSkillSlot->SetShowEnable( true, true );
//
//	//m_pDLGSkillSlot->SetPos( D3DXVECTOR2(0,0 ) );
//
//	/*
//	CKTDGUIStatic* pkStaticEDnCash = (CKTDGUIStatic*) m_pDLGMyInfoFront->GetControl( L"StaticPVP_Room_Unit_ED" );
//	pkStaticEDnCash->SetShowEnable( true, true );
//	*/
//	SetEnable( CX2Slot::ST_EQUIPPED, true );
//	SetEnable( CX2Slot::ST_INVENTORY, true );
//	SetShow( CX2Slot::ST_EQUIPPED, true );
//	SetShow( CX2Slot::ST_INVENTORY, true );
//	/*
//	CKTDGUIRadioButton* pRadioButtonUnit_Inventory_Equip = (CKTDGUIRadioButton*)m_pDLGMyInfoInventory->GetControl( L"RadioButtonUnit_Inventory_Equip" );
//	if ( pRadioButtonUnit_Inventory_Equip->GetCheckedControl() != NULL )
//	{
//	pRadioButtonUnit_Inventory_Equip->GetCheckedControl()->SetChecked( false );
//	}
//	pRadioButtonUnit_Inventory_Equip->SetChecked( true );
//	*/
//
//	UpdateExpGageBar();
//	UpdateSP();
//
//	WCHAR buff[256] = {0};
//
//	CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
//	CKTDGUIStatic* pStaticPVP_Room_Unit_WinNum = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_PVP_Win" );
//	if( NULL != pStaticPVP_Room_Unit_WinNum &&
//		NULL != pStaticPVP_Room_Unit_WinNum->GetString(0) )
//	{	
//		wsprintf( buff, L"%d�� %d��", (int)pUnitData->m_Win, (int)pUnitData->m_Lose );
//		pStaticPVP_Room_Unit_WinNum->GetString(0)->msg = buff;
//	}
//
//	CKTDGUIStatic* pStaticPVP_Room_Unit_VP = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_VP1" );
//	wsprintf( buff, L"%d / %d", (int)pUnitData->m_VSPoint, (int)pUnitData->m_VSPointMax );
//	pStaticPVP_Room_Unit_VP->GetString(0)->msg = buff;
//
//	CKTDGUIStatic* pStaticPVP_Room_Unit_Title = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Info_PVP_Grade" );
//	if ( pStaticPVP_Room_Unit_Title != NULL && g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem() != NULL )
//	{
//		pStaticPVP_Room_Unit_Title->GetString(0)->msg = g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_EmblemName;
//		pStaticPVP_Room_Unit_Title->GetPicture(0)->SetTex( g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_TextureName.c_str(),
//			g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_TextureKey.c_str() );
//	}
//
//	ResetSpirit();
//
//	
//	RemoveNoEnduranceNowEquip();
//
//	SetInventorySort( m_NowInventorySortType );
//}
//
//
//
//void CX2CharacterRoom::SetCharacterRoomQuestState()
//{
//	m_NowCharacterRoomState = CRS_QUEST;
//
//	SetEnable( CX2Slot::ST_EQUIPPED, true );
//	SetShow( CX2Slot::ST_EQUIPPED, true );
//
//	m_pDLGMyInfoQuickSlotNumber->SetEnable( true );
//	m_pDLGMyInfoQuickSlotNumber->SetShow( true );
//	
//
//	m_pUnitViewerUI->SetShowObject( true );
//}
//
//
//void CX2CharacterRoom::SetCharacterRoomSkillTreeState()
//{
//	m_NowCharacterRoomState = CRS_SKILL_TREE;
//
//	//m_pDLGSkillSlot->SetPos( D3DXVECTOR2(0,5) );
//
//
//	if( NULL != m_pSkillTreeUI )
//	{
//		m_pSkillTreeUI->SetShow( true );
//	}
//
//	ResetSkillSlotUI();
//
//}
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem )
//{
//	if ( pFromCX2SlotItem == NULL || pToCX2SlotItem == NULL )
//		return false;
//
//	if ( pFromCX2SlotItem->GetSortType() == pToCX2SlotItem->GetSortType() && pFromCX2SlotItem->GetSlotID() == pToCX2SlotItem->GetSlotID() )
//		return false;
//
//	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;
//
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= pFromCX2SlotItem->GetSortType();
//	if ( pFromCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID() + (( m_NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
//	}
//	else
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= pFromCX2SlotItem->GetSlotID();
//	}
//	
//
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= pToCX2SlotItem->GetSortType();
//
//	if ( pToCX2SlotItem->GetSlotType() == CX2Slot::ST_INVENTORY )
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID() + (( m_NowInvenSortTypePageNum - 1 ) * MY_INVENTORY_SHOW_MAX_SLOT_NUM);
//	}
//	else
//	{
//		kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= pToCX2SlotItem->GetSlotID();
//	}
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );
//
//	return true;
//}
//
//
//bool CX2CharacterRoom::Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID )
//{
//	if ( fromSortType == toSortType && fromSlotID == toSlotID )
//		return false;
//
//	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ;
//
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cFromSlotType	= fromSortType;
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iFromSlotID	= fromSlotID;
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_cToSlotType	= toSortType;
//	kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ.m_iToSlotID		= toSlotID;
//
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ, kEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK );
//
//	return true;
//}
//
//
//
//
//// �� �κ��丮�� �ִ� ��ȭ ������ ���� 
//int CX2CharacterRoom::GetRequiredEnchantAdjuvantItemID( int iItemLevel )
//{
//	if( iItemLevel <= 20 )
//	{
//		return 206730;
//	}
//	else if( iItemLevel <= 30 )
//	{
//		return 206740;
//	}
//	else if( iItemLevel <= 40 )
//	{
//		return 206750;
//	}
//	else if( iItemLevel <= 50 )
//	{
//		return 206760;
//	}
//	else if( iItemLevel <= 60 )
//	{
//		return 206770;
//	}
//	else if( iItemLevel <= 70 )
//	{
//		return 206780;
//	}
//	else if( iItemLevel <= 80 )
//	{
//		return 206790;
//	}
//	else if( iItemLevel <= 90 )
//	{
//		return 206800;
//	}
//	else if( iItemLevel <= 100 )
//	{
//		return 206810;
//	}
//	else
//	{
//		ASSERT( !"invalid item level" );
//		return -1;
//	}
//}
//
//#ifdef ITEM_RECOVERY_TEST
//// ������ ������ ���� �ʿ��� ������ �������� ID
//int CX2CharacterRoom::GetRequiredRecoveryStoneID( int iItemLevel )
//{
//	if( iItemLevel <= 20 )
//	{
//		return 206880;	// ������ lv1
//	}
//	else if( iItemLevel <= 30 )
//	{
//		return 206890;	// ������ lv2
//	}
//	else if( iItemLevel <= 40 )
//	{
//		return 206900;	// ������ lv3
//	}
//	else if( iItemLevel <= 50 )
//	{
//		return 206910;	// ������ lv4
//	}
//	else if( iItemLevel <= 60 )
//	{
//		return 206920;	// ������ lv5
//	}
//	else if( iItemLevel <= 70 )
//	{
//		return 206930;	// ������ lv6
//	}
//	else if( iItemLevel <= 80 )
//	{
//		return 206940;	// ������ lv7
//	}
//	else if( iItemLevel <= 90 )
//	{
//		return 206950;	// ������ lv8
//	}
//	else if( iItemLevel <= 100 )
//	{
//		return 206960;	// ������ lv9
//	}
//	else
//	{
//		ASSERT( !"invalid item level" );
//		return -1;
//	}
//
//}
//bool CX2CharacterRoom::Handler_EGS_RESTORE_ITEM_REQ( UidType RecoveryItemUid )
//{
//	// �ϴ� ������ �ִ��� �ٽ� Ȯ�����ְ�
//	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( RecoveryItemUid );
//	if(pItem == NULL) 
//		return false;
//	
//	KEGS_RESTORE_ITEM_REQ kEGS_RESTORE_ITEM_REQ;
//	
//	kEGS_RESTORE_ITEM_REQ.m_iItemUID = RecoveryItemUid;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_RESTORE_ITEM_REQ, kEGS_RESTORE_ITEM_REQ );
//	g_pMain->AddServerPacket( EGS_RESTORE_ITEM_ACK );
//
//	return true;
//
//}
//
//bool CX2CharacterRoom::Handler_EGS_RESTORE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_RESTORE_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	g_pMain->DeleteServerPacket( EGS_RESTORE_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"�������� �����Ǿ����ϴ�.", m_pNowState );
//
//			return true;
//		}		
//	}
//
//	return false;
//}
//
//#endif
//
//
//#ifdef TITLE_SYSTEM
//void CX2CharacterRoom::SetTitleName()
//{    
//    if(m_pDLGMyInfoInventory != NULL && m_pDLGMyInfoInventory->GetShow() == true)
//    {
//        CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"StaticMy_Title" );
//        if(pStaticUserInfo != NULL)
//            pStaticUserInfo->GetString(0)->msg = g_pData->GetTitleManager()->GetTitleName();        
//    }    
//}
//
//void CX2CharacterRoom::SetTitleNew(bool val)
//{
//    if(m_pDLGMyInfoInventory != NULL && m_pDLGMyInfoInventory->GetShow() == true)
//    {
//        CKTDGUIStatic* pStaticNewTitle = (CKTDGUIStatic*)m_pDLGMyInfoInventory->GetControl( L"g_pStatictitle" );
//        if(pStaticNewTitle != NULL)
//            pStaticNewTitle->GetPicture(0)->SetShow(val);        
//    }
//}
//#endif
//
//
//bool CX2CharacterRoom::CheckUseItem(CX2Item *pItem)
//{
//#ifdef TITLE_SYSTEM
//    if(pItem != NULL && pItem->GetItemTemplet()->m_ItemID == 130063) // ����Ŭ�ι�
//    {
//        if(g_pData != NULL && g_pData->GetTitleManager() != NULL && g_pData->GetTitleManager()->GetTakeTitle(80) == true)
//        {
//            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"�������� ����� �� �����ϴ�.", m_pNowState );
//
//            return false;
//        }
//    }
//#endif
//
//    return true;
//}
//
//#ifdef ATTRACTION_ITEM_TEST
//
//void CX2CharacterRoom::UpdateOpenAttractionItemAck()
//{
//	// ���� â�� �����..
//	if ( m_pDLGOpenAttraction != NULL )
//		g_pKTDXApp->SendGameMessage( XGM_DELETE_DIALOG, (WPARAM)m_pDLGOpenAttraction, NULL, false );
//	m_pDLGOpenAttraction = NULL;
//	m_bOpenedAttractionItem = false;
//
//	// ���� ������
//	m_pDLGAttractionResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Charm_Item_Result.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAttractionResult );
//
//	if ( m_TempOpenRandomItemAck.m_mapInsertedItem.empty() == false )
//	{
//		// �׳� ������ + ���ʽ� ������ �ΰ��� ���´ٰ� �����ϰ�..
//		std::map< int, int >::iterator i;
//
//		int nCount = 0;
//		wstring getExplanation = L"";
//		for ( i = m_TempOpenRandomItemAck.m_mapInsertedItem.begin(); i != m_TempOpenRandomItemAck.m_mapInsertedItem.end(); i++ )
//		{
//			int resultItemID = i->first;
//			int resultNum	= i->second;
//			CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
//			if ( pItemTemplet == NULL ) continue;
//
//			int MAGIC_ATTRACTION_BONUS_ITEM_ID = 91610;
//			CKTDGUIStatic* pStatic;
//			if(resultItemID == MAGIC_ATTRACTION_BONUS_ITEM_ID)
//			{
//				WCHAR buf[256] = {0};
//				wstring itemName = L"";
//				// ���ʽ� ������
//				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Bonus" );
//
//				itemName = pItemTemplet->GetFullName();
//
//				wsprintf( buf, L"%s %d��", itemName.c_str(), resultNum );
//				pStatic->GetString(0)->msg = buf;
//			}
//			else
//			{
//				// �׳� ������
//				WCHAR buf[256] = {0};
//				wstring itemName = L"";
//				pStatic = (CKTDGUIStatic*) m_pDLGAttractionResult->GetControl( L"Static_Item" );
//				itemName = pItemTemplet->GetFullName();
//				
//				if( resultNum == 1)
//				{
//					wsprintf( buf, L"%s", itemName.c_str() );					
//				}
//				else
//				{
//					wsprintf( buf, L"%s %d��", itemName.c_str(), resultNum );
//				}
//				pStatic->GetString(0)->msg = buf;
//
//			}
//
//			if( false == pStatic->GetPicture(0)->SetTex( pItemTemplet->m_ShopImage.c_str() ) )
//			{
//				pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
//			}
//
//		}
//	}
//
//	UpdateInventorySlotList( m_TempOpenRandomItemAck.m_vecKInventorySlotInfo );
//}
//
//void CX2CharacterRoom::PlayGetAttractionItem() 
//{
//	if ( m_bPlayAttractionItem == false )
//	{
//		m_bPlayAttractionItem = true;
//		
//		m_pDLGGetAttractionItemBG = new CKTDGUIDialog( m_pNowState, L"DLG_Unit_State_RandomItem_BG.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGetAttractionItemBG );
//		m_pDLGGetAttractionItemBG->SetColor( D3DXCOLOR( 0,0,0,0) );
//		m_pDLGGetAttractionItemBG->Move( D3DXVECTOR2(0,0), D3DXCOLOR( 0,0,0,0.7f), 0.5f, true );
//
//		D3DXVECTOR3 boxPos = m_pDLGOpenAttraction->GetDummyPos( 0 );
//
//		if( m_hMeshInstMeltMachine != INVALID_MESH_INSTANCE_HANDLE )
//		{
//			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstMeltMachine );
//		}
//
//		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_MeltMachineStart = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( L"FireMachineStart", boxPos.x, boxPos.y, boxPos.z , 0,0,0, 0,0,0 );
//		pMeshInst_MeltMachineStart->SetOverUI(true);
//		m_hMeshInstMeltMachineStart = pMeshInst_MeltMachineStart->GetHandle();
//
//		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Fire_Machine_Start.ogg", false, false );
//				
//
//// 		m_hSeqKeyTrace = g_pData->GetUIMajorParticle()->CreateSequenceHandle( L"KeyTrace", m_pMeshInstRandomBoxKey->GetPos() );
//// 		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqKeyTrace );
//// 		if( NULL != pSeq )
//// 		{
//// 			pSeq->SetOverUI( true );
//// 		}
//// 		else
//// 		{
//// 			m_hSeqKeyTrace = INVALID_PARTICLE_SEQUENCE_HANDLE;
//// 		}
//		
//	}
//}
//
//bool CX2CharacterRoom::IsEnchantCoupon( int ItemID )
//{
//	switch(ItemID)
//	{
//	case 130147:
//	case 130148:
//	case 130149:
//	case 130150:
//	case 130151:
//	case 130152:
//		return true;
//	default:
//		break;
//	}
//	return false;
//}
//
//bool CX2CharacterRoom::Handler_EGS_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID )
//{
//	// �ϴ� ������ �ִ��� �ٽ� Ȯ�����ְ�
//	CX2Item* pCouponItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( CouponItemUID );
//	CX2Item* pTargetItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( TargetItemUID );
//
//	// ��밡�������� Ȯ���Ѵ�
//	if ( pCouponItem == NULL || pTargetItem == NULL ||
//		pCouponItem->GetItemTemplet() == NULL )
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"������ ������ ã�� �� �����ϴ�.", m_pNowState );
//		return false;
//	}
//	if( IsEnchantCouponUseable( pCouponItem->GetItemTemplet()->m_ItemID, pTargetItem->GetItemData().m_EnchantLevel ) )
//	{
//		KEGS_ATTACH_ITEM_REQ kEGS_ATTACH_ITEM_REQ;
//
//		kEGS_ATTACH_ITEM_REQ.m_iAttachItemUID = CouponItemUID;
//		kEGS_ATTACH_ITEM_REQ.m_iDestItemUID = TargetItemUID;
//
//		g_pData->GetServerProtocol()->SendPacket( EGS_ATTACH_ITEM_REQ, kEGS_ATTACH_ITEM_REQ );
//		g_pMain->AddServerPacket( EGS_ATTACH_ITEM_ACK );
//
//		return true;
//	}
//	else
//	{
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"��ȭ�Ǻ��� ���� �ܰ��� �������� �����ؾ� �մϴ�.", m_pNowState );
//	}
//	return false;
//}
//bool CX2CharacterRoom::Handler_EGS_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ATTACH_ITEM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	g_pMain->DeleteServerPacket( EGS_ATTACH_ITEM_ACK );
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//
//			UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), L"������ ��뿡 �����߽��ϴ�!", m_pNowState );
//
//			return true;
//		}		
//	}
//
//	return false;
//
//}
//
//bool CX2CharacterRoom::IsEnchantCouponUseable(int CouponItemID, int TargetEnchantLevel )
//{
//	int CouponLevel = 0;
//	switch(CouponItemID)
//	{
//	case 130147:	// 5
//		CouponLevel = 5;
//		break;
//	case 130148:	// 6
//		CouponLevel = 6;
//		break;
//	case 130149:	// 7
//		CouponLevel = 7;
//		break;
//	case 130150:	// 8
//		CouponLevel = 8;
//		break;
//	case 130151:	// 9
//		CouponLevel = 9;
//		break;
//	case 130152:	// 10
//		CouponLevel = 10;
//		break;
//	default:
//		return false;
//	}
//	if(CouponLevel > TargetEnchantLevel) return true;
//	return false;
//}
//#endif
