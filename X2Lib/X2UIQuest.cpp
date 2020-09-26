#include "StdAfx.h"


#ifndef SERV_EPIC_QUEST
#include ".\x2uiQuest.h"

CX2UIQuest::CX2UIQuest( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2ItemSlotManager( pNowStage, pFileName ),
m_iQuestPerPage(0),
m_iRewardSlotNum(0),
m_iQuestListSize(0),
m_iQuestListUnderTop(0),
m_bShow(false),
m_MovedPosition(0,0),						// D3DXVECTOR2
m_DLGPosition(0,0),							// D3DXVECTOR2
m_DLGSize(0,0),								// D3DXVECTOR2
m_pDLGUIQuest(NULL),
m_pDLGMsgBox(NULL),
//m_vQuestListSlot.clear();
//m_vQuestList.clear();
m_iTopIndex(0),
m_iPickedQuestID(-1),
m_iQuestNum(0),
m_SumDelta(0),
m_vQuestDescSize(0,0),						// D3DXVECTOR2
//m_vecQuestDesc.clear();
m_iQuestDescIndex(0),
m_iQuestDescLinePerPage(1),
//m_mapGetItem.clear();
m_pDLGQuickQuest(NULL),
m_bQuickQuestOpenedUP(true),
m_bQuickQuestOpen(true)
{

	// �ʱ�ȭ ////////////////////////////////////////////////////////////////////////

	m_vQuestListSlot.clear();
	m_vQuestList.clear();
	m_vecQuestDesc.clear();

	//////////////////////////////////////////////////////////////////////////

	RegisterLuaBind();	

	m_pDLGUIQuest = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quest.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIQuest );
	m_pDLGUIQuest->SetShowEnable(false, false);
	m_pDLGUIQuest->SetDisableUnderWindow(true);

	D3DXVECTOR3 tmp;
	tmp = m_pDLGUIQuest->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUIQuest->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;

	m_iQuestPerPage = m_pDLGUIQuest->GetDummyInt(0);
	m_iRewardSlotNum = m_pDLGUIQuest->GetDummyInt(1);
}


CX2UIQuest::~CX2UIQuest(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUIQuest );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGQuickQuest );

// 	for(UINT i=0; i<m_vQuestListSlot.size(); i++)
// 	{
// 		MyQuestListSlot* pQuestSlot = m_vQuestListSlot[i];
// 		SAFE_DELETE(pQuestSlot);
// 	}
	m_vQuestListSlot.clear();

// 	for(UINT i=0; i<m_vQuestList.size(); i++)
// 	{
// 		MyQuestList* pQuest = m_vQuestList[i];
// 		SAFE_DELETE(pQuest);
// 	}


	m_vQuestList.clear();
}

void CX2UIQuest::SetLayer( X2_DIALOG_LAYER layer )
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	if(m_pDLGUIQuest != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIQuest, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIQuest, false );
	}
}

void CX2UIQuest::SetPosition( D3DXVECTOR2 vec )
{
	// ���� ���̾�α� ���ֺ��
	if(m_pDLGUIQuest != NULL) m_pDLGUIQuest->SetPos(vec);

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		D3DXVECTOR2 pos;
		pos = pItemSlot->GetPos();

		pos -= m_MovedPosition;
		pos += vec;

		pItemSlot->SetPos(pos);
	}
	m_MovedPosition = vec;
}

HRESULT CX2UIQuest::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();



	if(m_pDLGUIQuest != NULL && m_pDLGUIQuest->GetIsMouseOver() == true)
	{
		return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
	}
	else
	{
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
	}

	return S_OK;
}

bool CX2UIQuest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bFlag = false;
	switch( uMsg )
	{
		// ���콺 �޽����鿡 ����
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDLGUIQuest->GetIsMouseOver())
			{
				return false;
			}
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGUIQuest->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_QUEST, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}


	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	}

	return bFlag;
}

bool CX2UIQuest::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case QUCM_EXIT:
		{
			SetShow(false);
		}
		return true;
	case QUCM_SELECT_QUEST:
		{
			CKTDGUIRadioButton* pQuestButton = (CKTDGUIRadioButton*)lParam;
			MyQuestListSlot& QuestSlotList = m_vQuestListSlot[pQuestButton->GetDummyInt(0)];
			int QuestIndex = QuestSlotList.m_iQuestIndex;

			if( QuestIndex < (int)m_vQuestList.size() )
			{
				MyQuestList& Quest = m_vQuestList[QuestIndex];				
				SelectQuest( Quest.m_QuestID );
			}
		}
		return true;
	case QUCM_CATEGORY_BUTTON:
		{
			CKTDGUIButton* pCategoryButton = (CKTDGUIButton*)lParam;
			MyQuestListSlot& QuestSlotList = m_vQuestListSlot[pCategoryButton->GetDummyInt(0)];
			int QuestIndex = QuestSlotList.m_iQuestIndex;

			if( QuestIndex < (int)m_vQuestList.size() )
			{
				MyQuestList& Quest = m_vQuestList[QuestIndex];				
				Quest.m_bIsOpen = !Quest.m_bIsOpen;
				SelectCategory( Quest.m_DungeonID, Quest.m_bIsOpen );
			}

		}
		return true;
	case QUCM_LIST_UP:
		{
			SetNextTopIndex(true);
			UpdateQuestUI();
		}
		return true;
	case QUCM_LIST_DOWN:
		{
			SetNextTopIndex(false);
			UpdateQuestUI();
		}
		return true;
	case QUCM_COMMENT_UP:
		{
			m_iQuestDescIndex--;
			UpdateQuestDescPage();
		}
		return true;
	case QUCM_COMMENT_DOWN:
		{
			m_iQuestDescIndex++;
			UpdateQuestDescPage();
		}
		return true;
	case QUCM_QUEST_GIVEUP:
		{
			if( NULL != m_pDLGMsgBox )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

			m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_888 ), QUCM_QUEST_GIVEUP_CONFIRM, g_pMain->GetNowState(), QUCM_QUEST_GIVEUP_CANCLE );
			CKTDGUIButton* pButton_Cancle = (CKTDGUIButton*) m_pDLGMsgBox->GetControl( L"MsgBoxOkAndCancelCancelButton" );
			pButton_Cancle->RequestFocus();
		}
		return true;
	case QUCM_QUEST_GIVEUP_CONFIRM:
		{
			if( NULL != m_pDLGMsgBox )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

			// �ʽ��� ���̵�
			if( m_iPickedQuestID == 11010 && 
				g_pTFieldGame != NULL && 
				g_pTFieldGame->GetNoviceGuide() != NULL && 
				g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )
			{			
				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_PREV);
			}

			if( g_pData->GetUIManager()->GetUIQuestReceive() == NULL )
			{
				ASSERT( !"QuestReceive is NULL" );		// ���� ������ �� ������ ���� ���� class.
				g_pData->GetUIManager()->CreateUIQuestReceive();
			}
			g_pData->GetUIManager()->GetUIQuestReceive()->Handler_EGS_GIVE_UP_QUEST_REQ( m_iPickedQuestID );

		}
		return true;
	case QUCM_QUEST_GIVEUP_CANCLE:
		{
			if( NULL != m_pDLGMsgBox )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

		}
		return true;
	case QUCM_LIST_NAVI_LCLICKED:
	case QUCM_LIST_NAVI_DRAGGING:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			if(vRelativePos.y <= 0.01)
			{
				m_iTopIndex = 0;
			}
			else
			{
				// 0.68 : Navi Size�� ���� ��. MAGIC VALUE

				m_iTopIndex = (int)((vRelativePos.y / 0.68f) * (m_iQuestListSize - m_iQuestPerPage)) + 1;

			}

			UpdateQuestUI( false );

		}
		return true;
	case QUCM_COMMENT_NAVI_LCLICKED:
	case QUCM_COMMENT_NAVI_DRAGGING:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			// 0.55 : Navi Size�� ���� ��. MAGIC VALUE
			m_iQuestDescIndex = (int)((vRelativePos.y / 0.55f) * ((int) m_vecQuestDesc.size() - m_iQuestDescLinePerPage));

			UpdateQuestDescPage( false );
		}
		return true;
	case QUCM_QUICK_QUEST_CHECKBOX:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			m_bQuickQuestOpen = pCheckBox->GetChecked();
			SetShowQuickQuestDLG( pCheckBox->GetChecked() );
		}
		return true;
	case QUCM_QUICK_QUEST_SIZE_BUTTON:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			bool bQuickQuestDLGState = !GetQuickQuestDLGOpenedUP();
			OpenUpQuickQuesstDLG( bQuickQuestDLGState );
		}
		return true;
	default:
		break;
	}
	return false;
}

bool CX2UIQuest::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	return false;
}

void CX2UIQuest::SetShow(bool val)
{
	m_bShow = val;

	if(val)	// ���� �� ó���ؾ� �� �κ�
	{
		// UI�� �� ����Ʈ�� ������ �ְ�
		ResetQuestUI();

		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_QUEST);
#ifdef DISABLE_STATEMENU_IN_DUNGEON
// 		switch(g_pMain->GetNowStateID())
// 		{
// 		case CX2Main::XS_ARCADE_GAME:
// 		case CX2Main::XS_DUNGEON_GAME:
// 		case CX2Main::XS_PVP_GAME:
// 			{
// 				//SetPosition( D3DXVECTOR2(270, 60) );
// 				//SetLayer(XDL_POP_UP);
// 			} break;
// 		default:
// 			break;
// 		}
#endif
	}
	else	// ���� �� ó���ؾ� �� �κ�
	{
		// ������ ���콺 ���� �̹����� ������
		m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();

		// ������ ����
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->LostItemUI();
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
		}

		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_QUEST);
	}

	m_pDLGUIQuest->SetShowEnable(m_bShow, m_bShow);
}

//����Ʈ UI�� ������ �����Ѵ�
void CX2UIQuest::ResetQuestUI( bool bResetTopIndex )
{
	if(m_bShow)
	{
		// ���� EXP/ED/SP String 0���� ����� �ְ�
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ED" );
		pStaticED->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_Exp" );
		pStaticExp->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_SP" );
		pStaticSP->GetString(0)->msg = L"";

		// ���� ����Ʈ ǥ�� üũ�ڽ� : ���� ����Ʈ â�� ���¸� �޾ƿͼ� �������� �������� Ȯ���� ��� ��.
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIQuest->GetControl(L"CheckBox_ShowQuickQuest"); 
		if ( pCheckBox != NULL )
		{
			pCheckBox->SetCheckedPure( m_bQuickQuestOpen );
		}

		// ���� �� �� ���̰� �����
		for(UINT i=0; i<m_SlotList.size(); i++)
		{
			CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
			pSlot->SetShow(false);
			pSlot->SetEnable( false );
		}
		// ����Ʈ ���Ը���Ʈ �� �������ش� :
		// 1. �ϴ� �� �����
		m_vQuestListSlot.clear();

		//2. ���� �����
		WCHAR buff[256] = {0,};
		for(int i=0; i<m_iQuestPerPage; i++)
		{
			MyQuestListSlot QuestListSlot;

			// ����Ʈ ��ư ����
			//wsprintf( buff, L"RadioButton_List%d", i);
			StringCchPrintf( buff, 256, L"RadioButton_List%d", i);

			QuestListSlot.m_pListButton = (CKTDGUIRadioButton*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pListButton->SetShow(false);
			// ����Ʈ ����ƽ ����
			//wsprintf( buff, L"Static_QuestList%d", i);
			StringCchPrintf( buff, 256, L"Static_QuestList%d", i);
			QuestListSlot.m_pListStatic = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pListStatic ->SetShow(false);

			// ī���� ��ư ����
			//wsprintf( buff, L"Button_Category%d", i);
			StringCchPrintf( buff, 256, L"Button_Category%d", i);
			QuestListSlot.m_pCategoryButton = (CKTDGUIButton*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pCategoryButton->SetShow(false);
			// ī���� ����ƽ ����
			//wsprintf( buff, L"Static_Category%d", i);
			StringCchPrintf( buff, 256, L"Static_Category%d", i);
			QuestListSlot.m_pCategoryStatic = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pCategoryStatic ->SetShow(false);

			m_vQuestListSlot.push_back(QuestListSlot);
		}

		if(bResetTopIndex)
		{
			m_iTopIndex = 0;
		}		
		// ���� �ٽ� ����Ʈ ����Ʈ �޾ƿ���
		UpdateQuestList();

		// UI�� ������Ʈ �� �ش�
		UpdateQuestUI();

	}
	else
	{
		if(m_bQuickQuestOpen)
		{
			UpdateQuestList();
		}
	}

}

void CX2UIQuest::UpdateQuestList()
{
	bool bLastSelectedQuestExist = false;
	// �����ִ� ī���� ID�� �����Ѵ�
	vector<int> OpenedCategoryID;
	for(vector<MyQuestList>::iterator it = m_vQuestList.begin(); it < m_vQuestList.end(); advance(it, 1))
	{
		MyQuestList& QuestList = *it;
		if(QuestList.m_bIsCategory == true && QuestList.m_bIsOpen == true)
		{
			OpenedCategoryID.push_back(QuestList.m_DungeonID);
		}
	}


	// �� �����
// 	for(UINT j=0; j<m_vQuestList.size(); j++)
// 	{
// 		MyQuestList* pQuest = m_vQuestList[j];
// 		SAFE_DELETE(pQuest);	
// 	}

	m_vQuestList.clear();

	// �� ����Ʈ �޾ƿ���
	const map<int,CX2QuestManager::QuestInst*> mapUnitQuest = g_pData->GetQuestManager()->GetUnitQuestMap();
	//{{ kimhc // 2010.1.29 //	PC���� �ƴ� ��� PC�� ����Ʈ ��Ͽ��� ����
#ifdef	PC_BANG_QUEST
	m_iQuestNum = 0;
#else	PC_BANG_QUEST
	m_iQuestNum = mapUnitQuest.size();
#endif	PC_BANG_QUEST
	//}} kimhc // 2010.1.29 //	PC���� �ƴ� ��� PC�� ����Ʈ ��Ͽ��� ����
	
	if ( mapUnitQuest.empty() == true )
		return;


	//vector<MyQuestList*> vQuestList;
	// map DungeonID, MyQuestlist.
	std::multimap<int, MyQuestList> mapQuestList;
	map<int, CX2QuestManager::QuestInst*>::const_iterator i;

	for ( i = mapUnitQuest.begin(); i != mapUnitQuest.end(); i++ )
	{
		int questID = i->first;
		if( questID == m_iPickedQuestID )
		{
			bLastSelectedQuestExist = true;
		}
		CX2QuestManager::QuestInst* pQuestInst = i->second;
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( questID );

		if ( pQuestInst == NULL || 
			pQuestInst->m_OwnorUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() ||
			pQuestTemplet == NULL )
			continue;

		//{{ kimhc // 2010.1.29 //	PC���� �ƴ� ��� PC�� ����Ʈ ��Ͽ��� ����
#ifdef	PC_BANG_QUEST
		m_iQuestNum++;
#endif	PC_BANG_QUEST
		//}} kimhc // 2010.1.29 //	PC���� �ƴ� ��� PC�� ����Ʈ ��Ͽ��� ����

		// ����Ʈ�� �ִ� �� Ȯ�������� ����� ����..
		MyQuestList QuestList;
		QuestList.m_QuestID = questID;
		QuestList.m_bIsCategory = false;
		QuestList.m_bIsOpen = false;

		// �ش� ���� ���̵� ��� ���ؿ��� : ù��° ��������Ʈ�� �������� �Ѵ�. ���� �������� �����ؾ� �ϴ� ���� ����Ǿ� ���� �ʴ�.
		// ������ or �̺�Ʈ���� Ư���� �з��Ѵ�
		if( pQuestTemplet->m_vecSubQuest.size() > 0 && 
			( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_NORMAL || pQuestTemplet->m_eQuestType == CX2QuestManager::QT_SKILL ) )
		{
			if(false == pQuestTemplet->m_vecSubQuest.empty())
			{
				int SubQuestID = pQuestTemplet->m_vecSubQuest[0];
				QuestList.m_DungeonID = GetSubQuestDungeonID( SubQuestID );				
			}
			else
			{
				ASSERT( !"Quest with No Subquest!" );
				QuestList.m_DungeonID = SEnum::DI_NONE;
			}
		}
		else
		{
			QuestList.m_DungeonID = SEnum::DI_NONE;
		}

		mapQuestList.insert(std::pair<int, MyQuestList>(QuestList.m_DungeonID, QuestList));
		//vQuestList.push_back(pQuestList);
	}

	// ������ �޾����� DungeonID ������ ���ĵǾ� �����Ƿ�
	// ī���� �� ����־
	// ���� �ϳ��� �����
	int LastDID = -1;
	bool bCurrentCategoryOpened = false;
	for(std::multimap<int, MyQuestList>::iterator i = mapQuestList.begin();	i != mapQuestList.end(); advance(i, 1))
	{
		// DID�� �ٲ� ��� : �� ī�װ� ���� �־��ش�
		if( (i->first) != LastDID )
		{
			LastDID = i->first;
			MyQuestList QuestCategory;
			QuestCategory.m_bIsCategory = true;
// 			if(i->first == SEnum::DI_END)
// 				pQuestCategory->m_bIsOpen = true;
			QuestCategory.m_DungeonID = i->first;

			// ��~�� �����ص״� �����ִ� ī���� ����Ʈ�� �̿��ؼ� �ٽ� �������� �ݾ����� ����������..
			if( find(OpenedCategoryID.begin(), OpenedCategoryID.end(), QuestCategory.m_DungeonID) != OpenedCategoryID.end() )				
			{
				// ���� �־�����
				QuestCategory.m_bIsOpen = true;
				bCurrentCategoryOpened= true;
			}
			else
			{
				// �ƴϸ�
				QuestCategory.m_bIsOpen = false;
				bCurrentCategoryOpened = false;
			}

			m_vQuestList.push_back(QuestCategory);
			m_iQuestListSize++;

		}

		MyQuestList QuestList = i->second;
		QuestList.m_bIsOpen = bCurrentCategoryOpened;
		m_vQuestList.push_back(QuestList);
		m_iQuestListSize++;
	}


	if(false == bLastSelectedQuestExist)
	{
		m_iPickedQuestID = -1;
	}	

	//m_vQuestList.insert( m_vQuestList.end(), vQuestList.begin(), vQuestList.end());
	UpdateQuickQuestDLG();

}

int CX2UIQuest::GetSubQuestDungeonID( int iSubQuestID )
{
	const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( iSubQuestID );

	if(pSubQuestTemplet != NULL )
	{
		switch( pSubQuestTemplet->m_eClearType )
		{
		case CX2QuestManager::SQT_NPC_HUNT:
		case CX2QuestManager::SQT_QUEST_ITEM_COLLECTION:
		case CX2QuestManager::SQT_DUNGEON_TIME:
		case CX2QuestManager::SQT_DUNGEON_RANK:
		case CX2QuestManager::SQT_DUNGEON_DAMAGE:
		case CX2QuestManager::SQT_DUNGEON_CLEAR_COUNT:
#ifdef SERV_INTEGRATION
		case CX2QuestManager::SQT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
#ifdef SERV_POINT_COUNT_SYSTEM
		case CX2QuestManager::SQT_POINT_COUNT:
#endif SERV_POINT_COUNT_SYSTEM
			{
				// DID�� normal-hard-expert�� �پ� �ְ�, 30000���� �����ϰ�, �� �������� 10������ �з��Ǿ� �ִ�.
				// ���� �׳� 10�ڸ��� �ڸ� ���� �־� �ش�.
				// �ش� ������ �����ϴ� �����̸� ���� ID�� �����ְ�
				if( NULL != g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)pSubQuestTemplet->m_ClearCondition.m_eDungeonID) )
				{
					return (int)(pSubQuestTemplet->m_ClearCondition.m_eDungeonID / 10) * 10;
				}
				else
				{
					// �ƴϸ� �Ϲ����� ���´�
					return SEnum::DI_END;
				}				
			}
			break;
		case CX2QuestManager::SQT_NPC_TALK:
		case CX2QuestManager::SQT_ITEM_COLLECTION:
		case CX2QuestManager::SQT_PVP_PLAY:
		case CX2QuestManager::SQT_PVP_WIN:
		case CX2QuestManager::SQT_PVP_KILL:
#ifdef SERV_NEW_PVP_QUEST
		case CX2QuestManager::SQT_PVP_NPC_HUNT:
#endif SERV_NEW_PVP_QUEST
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
		case CX2QuestManager::SQT_PVP_HERO_NPC_KILL:
		case CX2QuestManager::SQT_PVP_HERO_NPC_PLAY:
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND

		default:
			{
				// ����
				return SEnum::DI_END;
			} break;
		}
	}
	else
	{
		// �ƿ� �ش� ��������Ʈ�� ����
		return SEnum::DI_END;
	}

}

void CX2UIQuest::UpdateQuestUI( bool bUpdateNavi )
{
	if(m_iQuestListSize == 0) return;

	bool bLastCategoryWasOpen = true;
	int SlotIndex = 0;

	// ����Ʈ �� �� �޾ҳ�~
	WCHAR buff[64] = {0,};
	CKTDGUIStatic* pStaticQuestNum = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_QuestNum" );
	//wsprintf( buff, L"%d/%d", m_iQuestNum, MY_QUEST_LIST_MAX_SIZE );
	StringCchPrintf( buff, 64, L"%d/%d", m_iQuestNum, MY_QUEST_LIST_MAX_SIZE );
	pStaticQuestNum->GetString(0)->msg = buff;

	m_iQuestListSize = 0;
	m_iQuestListUnderTop = 0;
	// ����Ʈ ������ m_vQuestList�� �� �����ִٰ� �����ϰ� m_iTopIndex�������� ���ʷ� �־ ������ ����
	for(int i = 0; i<(int)m_vQuestList.size(); i++)
	{
		if(i<m_iTopIndex || SlotIndex >= m_iQuestPerPage)
		{
			MyQuestList& Quest = m_vQuestList[i];
			if(Quest.m_bIsCategory || Quest.m_bIsOpen)
			{
				m_iQuestListSize++;
				if(i>m_iTopIndex) m_iQuestListUnderTop++;
			}
			continue;
		}
		MyQuestListSlot& QuestListSlot = m_vQuestListSlot[SlotIndex];

		// �ϴ� �� ����..
		QuestListSlot.m_pListButton->SetShow(false);
		QuestListSlot.m_pListStatic->SetShow(false);
		QuestListSlot.m_pCategoryStatic->SetShow(false);
		QuestListSlot.m_pCategoryButton->SetShow(false);

		MyQuestList& Quest = m_vQuestList[i];
		QuestListSlot.m_iQuestIndex = i;

		// �� ID�� �̿��ؼ� ����Ʈ ������ ���
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( Quest.m_QuestID );

		if( Quest.m_bIsCategory )
		{
			m_iQuestListSize++;
			if(i>m_iTopIndex) m_iQuestListUnderTop++;
			// ī������ ���
			bLastCategoryWasOpen = Quest.m_bIsOpen;

			// �ϴ� ���ĵ��� �� ����..
			QuestListSlot.m_pCategoryStatic->GetPicture(0)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(1)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(2)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(3)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(4)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(5)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(6)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(7)->SetShow(false);
			// ���� ���̵� �̿��ؼ� ���� �̸��� ������ �ְ�
			// DungeonData.m_DungeonName���� �������
			if(Quest.m_DungeonID == SEnum::DI_END)
			{
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = L"";
				QuestListSlot.m_pCategoryStatic->GetPicture(5)->SetShow(true);
			}
			else if(Quest.m_DungeonID ==SEnum::DI_NONE)
			{
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = L"";
				QuestListSlot.m_pCategoryStatic->GetPicture(6)->SetShow(true);
			}
			else if(NULL != g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)Quest.m_DungeonID) )
			{
				wstring dungeonName = g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)Quest.m_DungeonID)->m_DungeonName;
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = dungeonName;
			}
			else
			{
				ASSERT( !"���� �� ã����" );
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = L"";
			}

			// ���� ���̵�->���� ���̵�->���� �������� ������ �ְ� ���ƾƾƾ�

			CX2LocationManager::LOCAL_MAP_ID LMapID = g_pData->GetLocationManager()->GetLocalMapID( (SEnum::DUNGEON_ID)Quest.m_DungeonID );
			switch(LMapID)
			{
			case CX2LocationManager::LMI_RUBEN:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(0)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_ELDER:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(1)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_BESMA:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(2)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_ALTERA_ISLAND:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(3)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_PEITA:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(4)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_VELDER:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(7)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_INVALID:
			default:
				{

				} break;
			}
// 			wstring TextureName = GetVillageIconFileName( (int)LMapID );        
// 			wstring TextureKey = GetVillageIconKey( (int)LMapID );
// 			QuestListSlot.m_pCategoryStatic->GetPicture(0)->SetTex( TextureName.c_str(), TextureKey.c_str() );

			// ��ư�� ������ ���¸� �ٲ�־� �ش�
			QuestListSlot.m_pCategoryButton->SetDownStateAtNormal(Quest.m_bIsOpen);

			QuestListSlot.m_pCategoryButton->SetShow(true);
			QuestListSlot.m_pCategoryStatic->SetShow(true);

		}
		else
		{
			// ����Ʈ ��ư�� ���
			if(false == Quest.m_bIsOpen)
				continue;

			m_iQuestListSize++;
			if(i>m_iTopIndex) m_iQuestListUnderTop++;
			//���� �ְ�
			QuestListSlot.m_pListStatic->GetString(0)->msg = pQuestTemplet->m_wstrTitle;
			// ���� �ְ�
			QuestListSlot.m_pListStatic->GetString(0)->color = g_pData->GetQuestManager()->GetQuestColor( Quest.m_QuestID, g_pData->GetSelectUnitLevel() );
			// ���� ���ϰ�
			QuestListSlot.m_pListButton->SetShow(true);
			QuestListSlot.m_pListStatic->SetShow(true);
			QuestListSlot.m_pListStatic->GetPicture(0)->SetShow(false);
			QuestListSlot.m_pListStatic->GetPicture(1)->SetShow(false);
			QuestListSlot.m_pListStatic->GetPicture(2)->SetShow(false);
			QuestListSlot.m_pListStatic->GetPicture(3)->SetShow(false);
			//{{ kimhc // 2009-08-03 // ��ų picture �߰�
			QuestListSlot.m_pListStatic->GetPicture( 5 )->SetShow( false );
			//}} kimhc // 2009-08-03 // ��ų picture �߰�
			
			//{{ kimhc // 2010.2.1 //	PC�� ����Ʈ
#ifdef	PC_BANG_QUEST
			QuestListSlot.m_pListStatic->GetPicture( 6 )->SetShow( false );
#endif	PC_BANG_QUEST
				//}} kimhc // 2010.2.1 //	PC�� ����Ʈ

//{{ kimhc // 2010.4.27 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_SECRET_HELL
			QuestListSlot.m_pListStatic->GetPicture( 7 )->SetShow( false );
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.27 // ��д��� �۾�(��������Ʈ �ý���)

			switch(pQuestTemplet->m_eQuestType)
			{
			case CX2QuestManager::QT_NORMAL:
				{
					//{{ kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef	SERV_DAILY_QUEST
					switch ( pQuestTemplet->m_eRepeatType )
					{
					case  CX2QuestManager::QRT_NORMAL:
						QuestListSlot.m_pListStatic->GetPicture(0)->SetShow(true);
						break;

					case CX2QuestManager::QRT_REPEAT:
						QuestListSlot.m_pListStatic->GetPicture(3)->SetShow(true);
						break;

					case CX2QuestManager::QRT_DAY:
						QuestListSlot.m_pListStatic->GetPicture(7)->SetShow(true);
						break;


					default:
						break;
					}
#else	SERV_DAILY_QUEST
					if(pQuestTemplet->m_bRepeat == false)	// �Ϲ���
					{
						QuestListSlot.m_pListStatic->GetPicture(0)->SetShow(true);

					}
					else	// �ݺ���
					{
						QuestListSlot.m_pListStatic->GetPicture(3)->SetShow(true);
					}
#endif	SERV_DAILY_QUEST
					//}} kimhc // 2010.3.30 // ��д��� �۾�(��������Ʈ �ý���)
				}break;
			case CX2QuestManager::QT_CHANGE_JOB:
				{
					QuestListSlot.m_pListStatic->GetPicture(1)->SetShow(true);
				}break;
			case CX2QuestManager::QT_EVENT:
				{
					QuestListSlot.m_pListStatic->GetPicture(2)->SetShow(true);
				}break;

				//{{ kimhc // 2009-08-03 // ��ų picture �߰�
			case CX2QuestManager::QT_SKILL:
				{
					QuestListSlot.m_pListStatic->GetPicture( 5 )->SetShow( true );
				} break;
				//}} kimhc // 2009-08-03 // ��ų picture �߰�

				//{{ kimhc // 2010.2.1 //	PC�� ����Ʈ
#ifdef	PC_BANG_QUEST
			case CX2QuestManager::QT_PCBANG:
				{
					QuestListSlot.m_pListStatic->GetPicture( 6 )->SetShow( true );
				} break;
#endif	PC_BANG_QUEST
				//}} kimhc // 2010.2.1 //	PC�� ����Ʈ

			default:
				break;
			}

			// ���ְ�
			if(Quest.m_QuestID == m_iPickedQuestID && m_iPickedQuestID != -1 )
			{
				QuestListSlot.m_pListButton->SetChecked(true);
			}
			else
			{
				QuestListSlot.m_pListButton->SetChecked(false);
			}

		}

		// �Ϸ�� ����Ʈ��
		QuestListSlot.m_pListStatic->GetPicture(4)->SetShow(false);
		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( Quest.m_QuestID );
		if( pQuestInst != NULL && true == pQuestInst->IsComplete() )
		{
			QuestListSlot.m_pListStatic->GetPicture(4)->SetShow(true);
		}

		SlotIndex++;

	}

	for(int i=SlotIndex; i<m_iQuestPerPage; i++)
	{
		MyQuestListSlot& QuestListSlot = m_vQuestListSlot[i];

		// �� ����
		QuestListSlot.m_pListButton->SetShow(false);
		QuestListSlot.m_pListStatic->SetShow(false);
		QuestListSlot.m_pCategoryStatic->SetShow(false);
		QuestListSlot.m_pCategoryButton->SetShow(false);
	}

	// �����̶� ����Ʈ ������
	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
	// ����� ���� ������..
	if( pQuestTemplet != NULL && pQuestInst != NULL)
	{
		// ���� �Ϸ� �Ǿ��ų� ���� ���� ���� ������ �� ���� �Ѵ�
		CKTDGUIButton* pGiveUpButton = (CKTDGUIButton*)m_pDLGUIQuest->GetControl( L"Button_Giveup" );
		if( true == pQuestInst->IsComplete() || 
			g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
			g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
		{
			pGiveUpButton->SetShow(false);
		}
		else
		{
			pGiveUpButton->SetShow(true);
		}

		// ���� ���� �־��ְ�
		WCHAR wszText[64] = {0,};
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ED" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iED );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iED );
		pStaticED->GetString(0)->msg = wszText;

		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_Exp" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iEXP );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iEXP );
		pStaticExp->GetString(0)->msg = wszText;

		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_SP" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iSP );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iSP );
		pStaticSP->GetString(0)->msg = wszText;

		// ����Ʈ ������ �����
		const wstring wstrDesc = CreateQuestDesc();
		UpdateQuestDesc( wstrDesc );
		m_iQuestDescIndex = 0;
		UpdateQuestDescPage();

		// ���� ������ ���� �־��ְ�
		SetRewardItemSlot();

	}
	else
	{
		// ����� ���� ����
		CKTDGUIButton* pGiveUpButton = (CKTDGUIButton*)m_pDLGUIQuest->GetControl( L"Button_Giveup" );
		pGiveUpButton->SetShow(false);

		// ���� ������ �����
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ED" );
		pStaticED->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_Exp" );
		pStaticExp->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_SP" );
		pStaticSP->GetString(0)->msg = L"";

		// ����Ʈ ���� ����ƽ��..
		const wstring wstrDesc = CreateQuestDesc();
		UpdateQuestDesc( wstrDesc );
		m_iQuestDescIndex = 0;
		UpdateQuestDescPage();

		// ���� ������ ������ �����
		SetRewardItemSlot();
	}

	// �׺������ ���� �� �ְ�
	if(bUpdateNavi)
		UpdateQuestListNavi();


}

void CX2UIQuest::ClearQuestSlotList()
{
	for(UINT i=0; i<m_vQuestListSlot.size(); i++)
	{
		MyQuestListSlot& QuestSlot = m_vQuestListSlot[i];
		QuestSlot.m_pListButton->SetShow(false);
		QuestSlot.m_pListStatic->SetShow(false);
	}

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->DestroyItemUI();
		pItemSlot->SetShow( false );
		pItemSlot->SetEnable( false );
	}
}

void CX2UIQuest::SelectQuest( int SelectedQuestID )
{
	m_iPickedQuestID = SelectedQuestID;
	UpdateQuestUI();
}

void CX2UIQuest::SelectCategory( int SelectedCategoryDID, bool forOpen )
{
	for(UINT i=0; i<m_vQuestList.size(); i++)
	{
		MyQuestList& Quest = m_vQuestList[i];
		if(Quest.m_DungeonID == SelectedCategoryDID)
			Quest.m_bIsOpen = forOpen;
	}

	UpdateQuestUI();
}

void CX2UIQuest::SetRewardItemSlot()
{
	m_mapItemIDAndSocketID.clear();

	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	if( pQuestTemplet != NULL )
	{
		for(int i=0; i<m_iRewardSlotNum; i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[i];

			if( i < (int)pQuestTemplet->m_Reward.m_vecItem.size() )
			{
				// ���󽽷�
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_Reward.m_vecItem[i];
                const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				if ( itemData.m_iSocketOption1 != 0 )
				{
					m_mapItemIDAndSocketID.insert( std::make_pair( itemData.m_iItemID, itemData.m_iSocketOption1 ) );
				}
				if(pItemTemplet != NULL)
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_SocketOption.push_back(itemData.m_iSocketOption1);
					kItemData.m_ItemID = itemData.m_iItemID;
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					kItemData.m_Period = itemData.m_iPeriod;
					CX2Item* pItem = new CX2Item(kItemData, NULL);
					if ( pItem != NULL )
					{
						if ( pSlotItem != NULL )
						{
							pSlotItem->CreateItemUI( pItem, itemData.m_iQuantity );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable( true );
						}
					}
					SAFE_DELETE(pItem);
				}

			}
			else
			{
				// �����
				if(pSlotItem != NULL)
				{
					pSlotItem->DestroyItemUI();
					pSlotItem->SetShow(false);
					pSlotItem->SetEnable( false );
				}
			}
		}

		// ���ú���
		int SlotIndex = 0;
		for(int i=0; SlotIndex < m_iRewardSlotNum; i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[SlotIndex+m_iRewardSlotNum];

			if( i < (int)pQuestTemplet->m_SelectReward.m_vecSelectItem.size())
			{
				// ���ú��󽽷�
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_SelectReward.m_vecSelectItem[i];
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				//{{10.01.14 ������ ������ ���� �� ���� ó��
				if( pItemTemplet == NULL )
					continue;
				//}}10.01.14 ������ ������ ���� �� ���� ó��

				// fix!! �ϴ� ���ú��󿡼��� unit_type�� üũ
				if( pItemTemplet->GetUnitType() != CX2Unit::UT_NONE )
				{
					CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
					if( pItemTemplet->GetUnitType() != eUnitType )
					{
						continue;
					}
				}
				if ( itemData.m_iSocketOption1 != 0 )
				{
					m_mapItemIDAndSocketID.insert( std::make_pair( itemData.m_iItemID, itemData.m_iSocketOption1 ) );
				}
				if(pItemTemplet != NULL)
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_SocketOption.push_back(itemData.m_iSocketOption1);
					kItemData.m_ItemID = itemData.m_iItemID;
					kItemData.m_Period = itemData.m_iPeriod;
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					CX2Item* pItem = new CX2Item(kItemData, NULL);
					if ( pItem != NULL )
					{
						if ( pSlotItem != NULL )
						{
							pSlotItem->CreateItemUI( pItem, itemData.m_iQuantity );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable( true );
						}
					}
					SAFE_DELETE(pItem);

				}
			}
			else
			{
				// �����
				if(pSlotItem != NULL)
				{
					pSlotItem->DestroyItemUI();
					pSlotItem->SetEnable( false );
					pSlotItem->SetShow(false);
				}
			}
			SlotIndex++;
		}

	}
	else
	{
		// �� ���������
		for(UINT i=0; i<m_SlotList.size(); i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[i];
			pSlotItem->DestroyItemUI();
			pSlotItem->SetShow(false);
			pSlotItem->SetEnable(false);

		}
	}
}




bool CX2UIQuest::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	D3DXVECTOR2 mousePos;

	mousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	mousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	//mousePos = g_pKTDXApp->MouseConvertByResolution( mousePos );

	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			if( MouseOnQuestListArea(mousePos) )
			{
				SetNextTopIndex(true);
				UpdateQuestUI();
			}
			else if(MouseOnQuestCommentArea(mousePos))
			{
				m_iQuestDescIndex--;
				UpdateQuestDescPage();
			}
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{
			if( MouseOnQuestListArea(mousePos) )
			{
				SetNextTopIndex(false);
				UpdateQuestUI();
			}
			else if(MouseOnQuestCommentArea(mousePos))
			{
				m_iQuestDescIndex++;
				UpdateQuestDescPage();
			}
			m_SumDelta += WHEEL_DELTA;

		}	
	}

	return true;
}
bool CX2UIQuest::MouseOnQuestListArea( D3DXVECTOR2 mousePos )
{
	// ����Ʈ ����� ��ġ�� ũ��
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(15, 75) + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(409, 172) );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;

}
bool CX2UIQuest::MouseOnQuestCommentArea( D3DXVECTOR2 mousePos )
{
	// ����Ʈ ���� �κ��� ��ġ�� ũ��
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(15, 270) + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(406, 112) );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;

}

// m_iTopIndex�� ���Ϸ� �� ĭ �����̴� �Լ�
// isUp�� true�� ���� false�� �Ʒ���
void CX2UIQuest::SetNextTopIndex(bool isUp)
{
	if(m_iQuestListSize <= m_iQuestPerPage)
	{
		m_iTopIndex = 0;
		return;
	}
	if(isUp)
	{
		if(m_iTopIndex == 0) return;
		for(int i=m_iTopIndex-1; i>=0; i--)
		{
			MyQuestList& Quest = m_vQuestList[i];
			if(Quest.m_bIsCategory || Quest.m_bIsOpen)
			{
				m_iTopIndex = i;
				return;
			}
		}
		m_iTopIndex = 0;
		return;
	}
	else
	{
		if(m_iQuestListUnderTop < m_iQuestPerPage)
		{
			// �� �� ��������
			return;
		}
		for(UINT i=m_iTopIndex+1; i<m_vQuestList.size(); i++)
		{
			MyQuestList& Quest = m_vQuestList[i];
			if(Quest.m_bIsCategory || Quest.m_bIsOpen)
			{
				m_iTopIndex = i;
				return;
			}
		}
		return;
	}
	ASSERT(!"SetNextTopIndex Failed");
	m_iTopIndex = 0;
}

void CX2UIQuest::UpdateQuestListNavi()
{
	if(m_pDLGUIQuest != NULL)
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGUIQuest->GetControl( L"Navi_QuestList" );
		if( NULL != pNavi )
		{
			if( m_iQuestListSize <= m_iQuestPerPage )
			{
				pNavi->SetShow(false);
				return;
			}

			pNavi->SetShow(true);

			// ��ġ �����ְ�~
			D3DXVECTOR2 vPos;
			vPos.x = 0.0f;
			// 0.68 : Navi Size�� ���� ��. MAGIC VALUE
			vPos.y = (float)(m_iQuestListSize - m_iQuestListUnderTop - 1) / ((float)m_iQuestListSize - m_iQuestPerPage ) * 0.68f;

			pNavi->SetRelativeWindowPos( vPos );
		}
	}
}

void CX2UIQuest::UpdateQuestDesc( const wstring& wstrDesc )
{
	// �ϴ� wstrDesc�� ������-_-
	// Ŭ���� ���� -> 
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

	m_vQuestDescSize = D3DXVECTOR2( (float) m_pDLGUIQuest->GetDummyInt( 2 ), (float) m_pDLGUIQuest->GetDummyInt( 3 ) );
	m_vQuestDescSize = g_pKTDXApp->ConvertByResolution( m_vQuestDescSize );

	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ClearCondition" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	const int MAGIC_MAX_WIDTH = (int) m_vQuestDescSize.x;

	m_vecQuestDesc.resize(0);

	if( wstrDesc.length() == 0 )
	{
		m_iQuestDescIndex = 0;
		return;
	}

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrPageBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 

	int iDescLength = (int) wstrDesc.length();
	while( iNowCharIndex < iDescLength )
	{
		WCHAR wszCharBuf = wstrDesc[iNowCharIndex];
		wstring wstrCharBuf = wstrDesc.substr( iNowCharIndex, 1 ); 

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrPageBuf += wstrLineBuf;
					wstrLineBuf = L"";
					iNowLineWidth = 0;

					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}
					if(false == bIsFirstCharOfLine)
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
					}

					bIsFirstCharOfLine = true;
					wstrPageBuf = L"";

				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else if( 0 == wstrCharBuf.compare( L"m" ) )
				{
					wstrLineBuf.erase( wstrLineBuf.end()-1 ); // '#'�� ������
					wstrPageBuf += wstrLineBuf;

					m_vecQuestDesc.push_back( wstrPageBuf );

					wstrLineBuf = L"";
					wstrPageBuf = L"";
					bIsFirstCharOfLine = true;
					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					eNowCharState = Q_NORMAL;
				}
				else
				{
					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;
				}

			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";
					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					wstrColorCode = L"#C";
					wstrColorCode += wstrDesc.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrDesc.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

			} break;
		}

		if( iNowLineWidth >= MAGIC_MAX_WIDTH && Q_NORMAL == eNowCharState )
		{
			wstrLineBuf += L"\n";
			wstrPageBuf += wstrLineBuf;
			wstrLineBuf = L"";
			iNowLineWidth = 0;
			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}

			if(false == bIsFirstCharOfLine)
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
			}
			bIsFirstCharOfLine = true;
			wstrPageBuf = L"";
		}

		iNowCharIndex++;
	}

	if( wstrLineBuf.length() > 0 )
	{
		wstrPageBuf += wstrLineBuf;
	}

	if( wstrPageBuf.length() > 0 )
	{
		m_vecQuestDesc.push_back( wstrPageBuf );
	}

	m_iQuestDescIndex = 0;

}

wstring CX2UIQuest::CreateQuestDesc()
{
	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );

	wstringstream wstrm;

	// �ϴ� ����Ʈ �Ϸ����� �ִ� �κ�
	if( pQuestTemplet != NULL && pQuestInst != NULL )
	{
		// ���õ� ���� ������~
		bool bThereWasNPCTalkSubQuest = false;

		// sub-quest
		for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
		{
			const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
			if( NULL == pSubQuestTemplet )
				continue;

			const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;
			if( CX2QuestManager::SQT_NPC_TALK == pSubQuestTemplet->m_eClearType )
			{
				bThereWasNPCTalkSubQuest = true;
			}

			wstring wstrSubQuestDesc = L"";
			wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

			if( 0 != wstrSubQuestDesc.length() )
			{
				if( CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
					NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
				{
					// �Ϸ� �� ��
					wstrm << L"#CAA0000" << wstrSubQuestDesc << L"#CX\n" ;
				}
				else
				{
					// �Ϸ� �� �� ��
					wstrm << L"#C281CC0" << wstrSubQuestDesc << L"#CX\n";
				}
			}
		}

		if( false == bThereWasNPCTalkSubQuest )
		{
			wstringstream wstrmQuestCond;

			if( NULL != pQuestInst && true == pQuestInst->IsComplete() )
			{
                wstring wstrText;
				const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( pQuestTemplet->m_eEndNPCID );
				if( NULL != pNPCTemplet )
				{
					wstrText = GET_REPLACED_STRING( ( STR_ID_889, "LLI", pNPCTemplet->m_Description, pNPCTemplet->m_Name, KHanSungPostWordUnicodeWrapper( ( WCHAR* )pNPCTemplet->m_Name.c_str(), STR_ID_453, STR_ID_454 ) ) );
				}
				wstrm << wstrText << "\n";
			}
		}
		// ���� ���� ���� �ؽ�Ʈ�� �־� ����

		wstrm << L"#CX\n" << L" \n" << pQuestTemplet->m_wstrMainText;
	}

	return wstrm.str().c_str();
}

void CX2UIQuest::UpdateQuestDescPage( bool bUpdateNavi )
{
	if(m_iQuestDescIndex > (int)m_vecQuestDesc.size() - m_iQuestDescLinePerPage) m_iQuestDescIndex = m_vecQuestDesc.size() - m_iQuestDescLinePerPage;
	if(m_iQuestDescIndex < 0) m_iQuestDescIndex = 0;

	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ClearCondition" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	m_iQuestDescLinePerPage = (int) (m_vQuestDescSize.y / (float) pFont->GetHeight() * 0.5f);


	wstringstream wstrm;

	if( m_vecQuestDesc.size() == 0 )
	{
		m_iQuestDescIndex = 0;

	}
	else
	{
		int j = 0;
		for(UINT i=m_iQuestDescIndex; i<m_vecQuestDesc.size(); i++, j++)
		{
			wstrm << m_vecQuestDesc[i];
			if(j>m_iQuestDescLinePerPage) break;
		}

	}

	pStatic_QuestDesc->GetString(0)->msg = wstrm.str().c_str();

	if(bUpdateNavi)
		UpdateQuestCommentNavi();

}

void CX2UIQuest::UpdateQuestCommentNavi()
{
	if(m_pDLGUIQuest != NULL)
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGUIQuest->GetControl( L"Navi_QuestComment" );

		if( NULL != pNavi )
		{
			if( (int)m_vecQuestDesc.size() <= m_iQuestDescLinePerPage )
			{
				pNavi->SetShow(false);
				return;
			}

			pNavi->SetShow(true);

			// ��ġ �����ְ�~
			D3DXVECTOR2 vPos;
			vPos.x = 0.0f;
			// 0.55 : Navi Size�� ���� ��. MAGIC VALUE
			vPos.y = m_iQuestDescIndex / ((float) m_vecQuestDesc.size() - m_iQuestDescLinePerPage) * 0.55f;


			pNavi->SetRelativeWindowPos( vPos );
		}

	}
}



void CX2UIQuest::SetShowQuickQuestDLG( bool forOpen )
{
	if( forOpen && m_bQuickQuestOpen)
	{
		// ����
		if(m_pDLGQuickQuest == NULL)
		{
			RegisterLuaBind();
			m_pDLGQuickQuest = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quest_Restor.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGQuickQuest );
		}

		//UpdateQuickQuestDLG();
		OpenUpQuickQuesstDLG( true );
		m_pDLGQuickQuest->SetShow(true);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGQuickQuest, false );

	}
	else
	{
		// ����
		if(m_pDLGQuickQuest != NULL)
		{
			m_pDLGQuickQuest->SetShow(false);
			SAFE_DELETE_DIALOG(m_pDLGQuickQuest);
			m_pDLGQuickQuest = NULL;
		}
	}
}
bool CX2UIQuest::GetShowQuickQuestDLG()
{
	if( m_pDLGQuickQuest == NULL ) return false;
	return (m_bQuickQuestOpen && m_pDLGQuickQuest->GetShow());
}
void CX2UIQuest::UpdateQuickQuestDLG()
{
	if( m_pDLGQuickQuest == NULL || 
		m_bQuickQuestOpenedUP == false ||
		m_bQuickQuestOpen == false ) return;
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGQuickQuest->GetControl( L"Static_QuickQuest" );
	if(pStatic == NULL || pStatic->GetString(0) == NULL)
	{
		ASSERT( !"QuickQuest Static Read Error" );
		return;
	}

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	m_pDLGQuickQuest->SetStage( g_pMain->GetNowState() );

	wstring wstr;
	// "Ư��" ī�װ� ���� �̸��� ���� ���� �ֳ�?
	// �ʱⰪ�� true�� ���, Ư�� ī������ �����ϸ� false�� �ٲ㼭 �׷��ֵ��� �Ѵ�.
	bool bSpecialQuestCategoryWritten = true;	
	// ����Ʈ ����Ʈ �� �о
	for(UINT i=0; i<m_vQuestList.size(); i++)
	{
		MyQuestList& Quest = m_vQuestList[i];
		if(Quest.m_bIsCategory)
		{
			if(Quest.m_DungeonID == SEnum::DI_END)
				continue;
			if(Quest.m_DungeonID == SEnum::DI_NONE)
			{
				// "Ư��" �� ī�װ� ����
				bSpecialQuestCategoryWritten = false;
				continue;
			}

			// ���� ���̵� "Ư��" �� �ƴϰ�, Ư���� ����Ʈ�� �ִµ� ī���� �̸��� ���� ���� ������ (�� �ش� ���� ����Ʈ�� ������)
			if(false == bSpecialQuestCategoryWritten)
			{
				// ī���� �̸� ���ְ�
				wstr += L"#C23AF1E+ ";
                wstr += GET_STRING( STR_ID_890 );
                wstr += L"#CX\n";
				bSpecialQuestCategoryWritten = true;
			}

			// ī������ ���� �̸� ����~
			if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME && 
				g_pData->GetDungeonRoom() != NULL && g_pData->GetDungeonRoom()->GetDungeonID() / 10 == Quest.m_DungeonID / 10)
			{
				wstr += L"#CFFFF00- ";
			}
			else
			{
				wstr += L"#C23AF1E+ ";
			}
			wstr += g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)Quest.m_DungeonID)->m_DungeonName;
			wstr += L"#CX\n";
		}
		else if( Quest.m_DungeonID == SEnum::DI_NONE )
		{
			// "Ư��" �� �з���, �̺�Ʈ/���� ����Ʈ
			// �ϴ� �ش� ����Ʈ�� ���� �� �������� ó���Ǵ��� Ȯ���غ���.
			if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME && g_pData->GetDungeonRoom() != NULL )
			{
				const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( Quest.m_QuestID );
				if(pQuestTemplet != NULL && false == pQuestTemplet->m_vecSubQuest.empty())
				{
					// ù��° ��������Ʈ ��������
					int SubQuestDungeonID = GetSubQuestDungeonID( pQuestTemplet->m_vecSubQuest[0] );
					int CurrentDungeonID = g_pData->GetDungeonRoom()->GetDungeonID();
					// �ش� �����̸� ( ���̵� �����Ϸ��� 10���� ������ )
					if(SubQuestDungeonID / 10 == CurrentDungeonID / 10)
					{
						// ����Ʈ ǥ�� �� ����.                        
						// ī���� �̸��� ���� ���� ������
						if(false == bSpecialQuestCategoryWritten)
						{
							// ī���� �̸� ���ְ�
							wstr += L"#CFFFF00- ";
                            wstr += GET_STRING( STR_ID_890 );
                            wstr += L"#CX\n";
							bSpecialQuestCategoryWritten = true;
						}

						//{{ ���� ����Ʈ ������ ���ش�
						const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( Quest.m_QuestID );

						// ����Ʈ �Ϸ����� �ִ� �κ�
						if( pQuestInst != NULL )
						{
							// sub-quest
							for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
							{
								const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
								if( NULL == pSubQuestTemplet )
									continue;

								const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;

								wstring wstrSubQuestDesc = L"";
								wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

								if( 0 != wstrSubQuestDesc.length() )
								{
									if( CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
										NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
									{
										wstr += L"#CFBAF5D";
										wstr += wstrSubQuestDesc;
										wstr += L" (";
                                        wstr += GET_STRING( STR_ID_759 );
                                        wstr += L")#CX";
									}
									else
									{
										wstr += wstrSubQuestDesc;
									}

									wstr += L"\n";
								}
							}
						}
						//}} ����Ʈ ���� �� ���

					}
				}
			}
		}
		else if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME && 
			g_pData->GetDungeonRoom() != NULL && g_pData->GetDungeonRoom()->GetDungeonID() / 10 == Quest.m_DungeonID / 10)
		{
			// ����Ʈ ���� ����~
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( Quest.m_QuestID );
			const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( Quest.m_QuestID );

			// �ϴ� ����Ʈ �Ϸ����� �ִ� �κ�
			if( pQuestTemplet != NULL && pQuestInst != NULL )
			{
				// sub-quest
				for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
				{
					const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
					if( NULL == pSubQuestTemplet )
						continue;

					const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;

					wstring wstrSubQuestDesc = L"";
					wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

					if( 0 != wstrSubQuestDesc.length() )
					{
						if( CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
							NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
						{
							wstr += L"#CFBAF5D";
							wstr += wstrSubQuestDesc;
							wstr += L" (";
                            wstr += GET_STRING( STR_ID_759 );
                            wstr += L")#CX";
						}
						else
						{
							wstr += wstrSubQuestDesc;
						}

						wstr += L"\n";
					}
				}
			}
		}
	}

	// ������ �� ���Ҵµ� "Ư��" �ؽ�Ʈ�� ������ ���� ��� : Ư���� ����Ʈ"��" �ִ� ��쿡 ����ؼ�
	if(false == bSpecialQuestCategoryWritten)
	{
		// ī���� �̸� ���ְ�
		wstr += L"#C23AF1E+ ";
        wstr += GET_STRING( STR_ID_890 );
        wstr += L"#CX\n";
		bSpecialQuestCategoryWritten = true;
	}

	// ���⼭ �ϴ� string�� �ϼ��ư�. ���� Line Break ������.


	int nLine = 0;
	const int MAGIC_TEXT_WIDTH = 220;
	const int MAGIC_DESC_ADD_SIZE_Y = 20;

#ifdef CLIENT_GLOBAL_LINEBREAK
	nLine = CWordLineHandler::LineBreakInX2Main( wstr, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	nLine = CX2Main::LineBreak( wstr, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#endif //CLIENT_GLOBAL_LINEBREAK

	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGQuickQuest->GetControl( L"Static_QuickQuest" );

	int DescHeight = (int)( pFont->GetHeight( wstr.c_str() ) / g_pKTDXApp->GetResolutionScaleY() )+ MAGIC_DESC_ADD_SIZE_Y;
	int DescWidth = 0;

	// Y ���� �ּҰ��� ������ �ش�. MinSize�� ������ Lua ���Ͽ� �ִ� ũ��
	const int MinSizeY = 126;
	DescHeight = max(DescHeight, MinSizeY);
	DescWidth = (int)pStatic_BG->GetPicture(1)->GetSize().x;

	// ���
	pStatic_BG->GetPicture(1)->SetSize( D3DXVECTOR2( (float)DescWidth, (float)DescHeight ) );

	// �ٴ�
	pStatic_BG->GetPicture(2)->SetPos( D3DXVECTOR2( 0, (float)DescHeight+20 ) );


	pStatic->GetString(0)->msg = wstr.c_str();

}

void CX2UIQuest::OpenUpQuickQuesstDLG( bool forOpen )
{
	if( NULL == m_pDLGQuickQuest )
		return;

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGQuickQuest->GetControl( L"Static_QuickQuest" );
	//CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGQuickQuest->GetControl( L"Button_QuickQuest" );

	m_bQuickQuestOpenedUP = forOpen;
	if(forOpen) 
		UpdateQuickQuestDLG();

	//pButton->SetDownStateAtNormal(forOpen);
	if( NULL != pStatic )
		pStatic->SetShow(forOpen);

}

bool CX2UIQuest::GetOverMouseQuest()
{
	if( GetShow() == true && m_pDLGUIQuest != NULL && m_pDLGUIQuest->GetIsMouseOver() == true )
		return true;
	return false;		
}

// wstring CX2UIQuest::GetVillageIconFileName( int LocalMapID )
// {
// 	switch(LocalMapID)
// 	{
// 	case CX2LocationManager::LMI_VELDER_NORTH:
// 	case CX2LocationManager::LMI_VELDER_EAST:
// 	case CX2LocationManager::LMI_VELDER_SOUTH:
// 	case CX2LocationManager::LMI_ALTERA_ISLAND:
// 	case CX2LocationManager::LMI_PEITA:
// 	case CX2LocationManager::LMI_INVALID:
// 		return L"DLG_UI_Common_Texture02.TGA";
// 	default:
// 		break;
// 	}
// 	return L"";
// }
// wstring CX2UIQuest::GetVillageIconKey( int LocalMapID )
// {
// 	switch(LocalMapID)
// 	{
// 	case CX2LocationManager::LMI_VELDER_NORTH:
// 		return L"ruben";
// 	case CX2LocationManager::LMI_VELDER_EAST:
// 		return L"eldu";	
// 	case CX2LocationManager::LMI_VELDER_SOUTH:
// 		return L"besma";	
// 	case CX2LocationManager::LMI_ALTERA_ISLAND:
// 		return L"altera";	
// 	case CX2LocationManager::LMI_PEITA:
// 		return L"besma";	
// 	case CX2LocationManager::LMI_INVALID:
// 		return L"feel";
// 	default:
// 		break;
// 	}
// 	return L"";
// 	
// }

wstring CX2UIQuest::GetSlotItemDesc()
{
	wstring itemDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetDialog() == NULL )
		{
			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
		}
		else
		{	
			CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
				m_pNowOverItemSlot->GetItemUID() );
			if ( pkItem != NULL )
				itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
			else
			{
				CX2Item* pItem = NULL;
				map< int, int >::iterator mit;
				mit = m_mapItemIDAndSocketID.find( m_pNowOverItemSlot->GetItemTID() );
				if ( mit != m_mapItemIDAndSocketID.end() )
				{

					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
					if ( pItemTemplet != NULL )
					{
					    CX2Item::ItemData kItemData;
						kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
						kItemData.m_Endurance = pItemTemplet->GetEndurance();
						int socketID = mit->second; 
						kItemData.m_SocketOption.push_back( (short)socketID );
						kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
#ifdef QUEST_REWARD_PERIOD
						if(m_pNowOverItemSlot->GetPeriod() != 0)
						{
							kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
						}
#endif QUEST_REWARD_PERIOD
						CX2Item* pItem = new CX2Item( kItemData, NULL );
						itemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID() );
						SAFE_DELETE( pItem );
					}

				}
#ifdef QUEST_REWARD_PERIOD	
				else if(m_pNowOverItemSlot->GetPeriod() != 0)
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
					if ( pItemTemplet != NULL )
					{
					    CX2Item::ItemData kItemData;
						kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
						kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
						kItemData.m_Endurance = pItemTemplet->GetEndurance();
						kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
						CX2Item* pItem = new CX2Item( kItemData, NULL );
						itemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID() );
						SAFE_DELETE( pItem );
					}
				}
#endif QUEST_REWARD_PERIOD
				else
				{
					itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
				}

			}
		}

	}

	return itemDesc;	
}
#endif SERV_EPIC_QUEST

