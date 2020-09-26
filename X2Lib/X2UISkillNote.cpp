#include "StdAfx.h"

#ifdef SERV_SKILL_NOTE

#include "X2UISkillNote.h"

CX2UISkillNote::CX2UISkillNote( CKTDXStage* pNowStage, const WCHAR* pFileName ) :
m_iRegisterPage(0),
m_iCurrentPage(0),
m_iUpdatePage(0),
m_bShow(false),
m_bRegisterMemo(false),
#ifdef REFORM_SKILL_NOTE_UI
m_SumDelta(0),
m_pDLGMemoList(NULL),
#else
m_bUpdateMemo(false), // �޸� ��� ��ϵǵ��� ����
m_fWriteMemoTime(0.f),
m_pDLGWrittingNote(NULL),
#endif // REFORM_SKILL_NOTE_UI
m_iRegisterMemo(0),
m_pDLGSkillNote(NULL),
m_pDLGQuery(NULL),
m_iViewPage(0)
{
	m_pDLGSkillNote = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillNote );
	m_pDLGSkillNote->SetDisableUnderWindow(true);
	m_pDLGSkillNote->SetFront(true);
	m_pDLGSkillNote->SetDisableUnderWindow(true);
	m_pDLGSkillNote->SetShowEnable(false, false);


	CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGSkillNote->GetControl(L"_Prev_button");
	if( pButton ) pButton->SetEnable(false);
	pButton = (CKTDGUIButton*)m_pDLGSkillNote->GetControl(L"_Next_button");
	if( pButton ) pButton->SetEnable(false);


	m_hParticle1 = INVALID_PARTICLE_SEQUENCE_HANDLE;	
	m_hParticle2 = INVALID_PARTICLE_SEQUENCE_HANDLE;	
#ifndef REFORM_SKILL_NOTE_UI// ���� ǥ�� ��� �������� ���� ��� �� ��
#ifdef COUNTRY_LV_STRING
	std::wstring wstrLevel = L"Lv.";
	switch(g_pMain->GetNationFlag())
	{
		// ����
	case CX2Main::NF_FR:
	case CX2Main::NF_BR:
		wstrLevel = L"Nv.";
		break;	
	case CX2Main::NF_PL:
		wstrLevel = L"Pz.";
		break;	
	case CX2Main::NF_ES:
		wstrLevel = L"Niv.";
		break;	
	default:
		break;
	}
	
	for( int iLevel = 20; iLevel <= 70; iLevel += 10 )
	{
		std::wstringstream wstrstmLevel;
		wstrstmLevel << wstrLevel;

		if( g_pInstanceData->GetMaxLevel() < iLevel )
			wstrstmLevel << L"??";
		else
			wstrstmLevel << iLevel;

	    m_vecSlotLv.push_back( wstrstmLevel.str() );
	}
#else //COUNTRY_LV_STRING
	// �� ���� ��Ʈ�� ����
    m_vecSlotLv.push_back(L"Lv.20");
	m_vecSlotLv.push_back(L"Lv.30");
	m_vecSlotLv.push_back(L"Lv.40");
	m_vecSlotLv.push_back(L"Lv.50");
	m_vecSlotLv.push_back(L"Lv.60");
	m_vecSlotLv.push_back(L"Lv.70");
#endif //COUNTRY_LV_STRING
#endif // REFORM_SKILL_NOTE_UI
}
CX2UISkillNote::~CX2UISkillNote(void)
{
	SAFE_DELETE_DIALOG( m_pDLGSkillNote );
#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
	SAFE_DELETE_DIALOG( m_pDLGWrittingNote );
#endif // REFORM_SKILL_NOTE_UI
}
 
HRESULT CX2UISkillNote::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();	

#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
	if( GetShow() == false )
	{
		m_bUpdateMemo = false;
		return S_OK;
	}
#endif // REFORM_SKILL_NOTE_UI

	CX2State* pState = (CX2State*)g_pMain->GetNowState();
	if( pState == NULL )
	{
#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
		if( m_bUpdateMemo == true )
		{
			SAFE_DELETE_DIALOG( m_pDLGWrittingNote );
			m_pDLGWrittingNote = NULL;
			ResetSkillNote();
		}
#endif // REFORM_SKILL_NOTE_UI
		
		SetShow(false);
		return S_OK;
	}

	if( pState->GetCursor() != NULL &&
		pState->GetCursor()->GetCurorState() == CX2Cursor::XCS_REGISTER_MEMO )
	{	
		m_pDLGInfoBox = NULL;

		if( m_pDLGQuery != NULL )
		{
			if( g_pMain->GetInfoBox() != NULL )
				g_pMain->DeleteInfoBox();
		}
		else
		{
			if( g_pMain->GetInfoBox() == NULL )
				g_pMain->KTDGUIInfoBox( D3DXVECTOR2( 250, 200 ), GET_STRING( STR_ID_5009), g_pMain->GetNowState() );
		}
	}
	else
	{
#ifdef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
		if( g_pMain->GetInfoBox() != NULL && m_pDLGInfoBox == NULL )
			g_pMain->DeleteInfoBox();
#else
		if( m_bUpdateMemo == false )
		{
			if( g_pMain->GetInfoBox() != NULL && m_pDLGInfoBox == NULL )
				g_pMain->DeleteInfoBox();
		}
		else
		{
			if( m_fWriteMemoTime > 0.f )
			{
				m_fWriteMemoTime -= fElapsedTime;
				
				float fProgressNote = 1.f;
				fProgressNote -= (m_fWriteMemoTime / 2.f);

				if( m_pDLGWrittingNote != NULL )
				{
					CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGWrittingNote->GetControl(L"g_pStaticNote_Loading");
					CKTDGUIControl::CPictureData *pPicture = pStatic->GetPictureIndex(1);
					float fProgressRate = pPicture->GetOriginalSize().x * fProgressNote;
					pPicture->SetSizeX( fProgressRate );
				}

				if( m_fWriteMemoTime <= 0.f && m_pDLGInfoBox == NULL )
				{
					SAFE_DELETE_DIALOG( m_pDLGWrittingNote );
					m_pDLGWrittingNote = NULL;

					m_iViewPage = 0;
					UpdateMemoSlot((int)m_iUpdatePage);
					UpdateMemoEffect();
					m_pDLGInfoBox = g_pMain->KTDGUIInfoBox( D3DXVECTOR2( 250, 200 ), GET_STRING( STR_ID_5010), g_pMain->GetNowState(), 3.f);
					m_fWriteMemoTime = 0.f;					
					m_bUpdateMemo = false;
					m_iUpdatePage = 0;

					g_pKTDXApp->GetDeviceManager()->PlaySound( L"TechNote_CompleteWriting.ogg", false, false );
				}
			}
		}
#endif // REFORM_SKILL_NOTE_UI
	}

	return S_OK;
}

bool CX2UISkillNote::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �տ� ��� ���̾�αװ� ������ �޽����� ó������ �ʴ´�
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGSkillNote ) )
		return false;

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
		{
			if ( false == m_pDLGSkillNote->GetIsMouseOver())
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
			if ( false == m_pDLGSkillNote->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_SKILL_NOTE, true);
			bFlag = true;			
		}
		break;
#ifdef REFORM_SKILL_NOTE_UI
	case WM_MOUSEWHEEL:
		{			
			// ����� ��Ʈ ���� ���콺�� ���� ����!!
			if( NULL != m_pDLGMemoList &&
				true == m_pDLGMemoList->GetIsMouseOver() )
			{
				CKTDGUIContolList *pControlList = static_cast<CKTDGUIContolList*>(m_pDLGMemoList->GetControl(L"MemoControlList"));
				if( NULL != pControlList)
				{
					CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
					if( NULL != pScroolBarY )
					{
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
					}
				}
			}
		} break;
#endif // REFORM_SKILL_NOTE_UI

	default:
		break;
	}
	return bFlag;
}

bool CX2UISkillNote::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case USNM_EXIT:
		{
			SetShow(false);
			return true;
		}
		break;
	case USNM_REGISTER_VIEW_SLOT:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;							// fix!! ���� ��ư���� ���� �ؾ��� 
			m_iRegisterPage = (char)pButton->GetDummyInt(0);

			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if( pState != NULL && 
				pState->GetCursor() != NULL &&
				pState->GetCursor()->GetCurorState() != CX2Cursor::XCS_REGISTER_MEMO )
			{
				m_bRegisterMemo = false;
				m_iRegisterMemo = 0;				
			}

			if( m_bRegisterMemo == true )
			{
				// �޸���	Ȯ��
				CheckRegisterMemo();
			}
			else
			{
				// �޸��
				ViewMemo( (int)(m_iRegisterPage-1) );
			}

			return true;
		}
		break;
	case USNM_SLOT_PAGE_LEFT:
		{
			if( m_iCurrentPage > 0)
				--m_iCurrentPage;
			return true;
		}
		break;
	case USNM_SLOT_PAGE_RIGHT:
		{
			++m_iCurrentPage;
			return true;
		}
		break;
	case USNM_REGISTER_OK:
		{
			//�޸���
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
			m_pDLGQuery = NULL;

			Handler_EGS_REG_SKILL_NOTE_MEMO_REQ( m_iRegisterPage );

			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if( pState != NULL && pState->GetCursor() != NULL )
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );
			m_bRegisterMemo = false;
			m_iRegisterMemo = 0;
			m_iRegisterPage = 0;
			return true;
		}
		break;
	case USNM_REGISTER_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
			m_pDLGQuery = NULL;
		}
		break;
	case USNM_PAGE_PREV:
		{
			ViewMemo( m_iViewPage - 3 );
		}
		break;
	case USNM_PAGE_NEXT:
		{
			ViewMemo( m_iViewPage + 1 );
		}
		break;
#ifdef REFORM_SKILL_NOTE_UI
	case USNM_USE_MEMO:
		{
			CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
			if( NULL != pButton )
			{
				const int iMemoItemID = pButton->GetDummyInt(0);
				if( 0 < iMemoItemID &&
					NULL != g_pData &&
					NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetMyUser()->GetSelectUnit() &&
					NULL != g_pData->GetUIManager() &&
					NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					const CX2Inventory& pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

					// �ش� �޸� ������ �������̶��
					CX2Item* pItem = pInventory.GetItemByTID(iMemoItemID);
					if( NULL != pItem )
					{
						UidType uiMemoItemUID = pItem->GetUID();
						const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
						if( NULL != pItemTemplet )
						{
							// �޸� ���
							g_pData->GetUIManager()->GetUIInventory()->RegisterMemo( pItemTemplet, uiMemoItemUID );
						}
					}
				}
			}
		} break;
	case USNM_BUY_MEMO:
		{			
			CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
			if( NULL != pButton )
			{
				const int iMemoItemID = pButton->GetDummyInt(0);
				if( 0 < iMemoItemID )
				{
					//����/���� �����߿��� ĳ���� ���� �ʱ�.
					if( g_pX2Game != NULL && g_pMain->GetNowStateID() != CX2Main::XS_BATTLE_FIELD )
					{
						g_pChatBox->AddChatLog(  GET_STRING( STR_ID_16478 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
						return true;
					}
					if( NULL != g_pData->GetCashShop() )
					{
						g_pData->GetCashShop()->SetMenuTypeCallingCashShop( CX2UIManager::UI_SKILL_NOTE );
						g_pData->GetCashShop()->SetItemIDShowBuyUIAfterEnter(iMemoItemID);
					}
					if( g_pMain->IsInheritStateMenu() )
					{
						CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
						if( NULL != pStateMenu)
							pStateMenu->ToggleCashShop();
					}
				}
			}
		} break;
	case USNM_HAS_MEMO_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
			if( NULL != pButton )
			{
				const int iMemoItemID = pButton->GetDummyInt(0);

				wstring wstrMemoDesc = L"";
				GetMemoDesc( wstrMemoDesc, iMemoItemID, true);

				pButton->SetGuideDescOffsetPos( D3DXVECTOR2(760.f,30.f));
				pButton->SetGuideDesc( wstrMemoDesc.c_str() );
			}
		} break;
	case USNM_NOT_HAS_MEMO_MOUSE_OVER:
		{
			CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
			if( NULL != pButton )
			{
				const int iMemoItemID = pButton->GetDummyInt(0);

				wstring wstrMemoDesc = L"";
				GetMemoDesc( wstrMemoDesc, iMemoItemID, false);

				pButton->SetGuideDescOffsetPos( D3DXVECTOR2(780.f,30.f));
				pButton->SetGuideDesc( wstrMemoDesc.c_str() );
			}
		} break;
#endif // REFORM_SKILL_NOTE_UI
	default:
		break;
	}

	return false;
}

bool CX2UISkillNote::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	return false;
}

void CX2UISkillNote::SetShow(bool val)
{
	m_iViewPage = 0;

	if( val )
	{
		g_pData->GetUIManager()->CloseAllLUDlg();
		g_pData->GetUIManager()->CloseAllNPCDlg();
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_SKILL_NOTE);
		
		if( m_bShow == false )
		{			
			m_iCurrentPage = 0;
			UpdateMemoSlot();
			ViewMemo(0);
		}

		m_bShow = val;
		m_pDLGSkillNote->SetShowEnable(val, val);

#ifdef REFORM_SKILL_NOTE_UI
		ResetMemoList();
#endif // REFORM_SKILL_NOTE_UI
	}
	else
	{
		m_bShow = val;
		m_pDLGSkillNote->SetShowEnable(val, val);

		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_SKILL_NOTE);
		m_iRegisterMemo = 0;
		m_bRegisterMemo = false;


		CX2State* pState = (CX2State*)g_pMain->GetNowState();
		if( pState != NULL )
			pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_NORMAL );

		ResetSkillNote();		
	}	

#ifdef REFORM_SKILL_NOTE_UI
	if( NULL != m_pDLGMemoList )
		m_pDLGMemoList->SetShowEnable( m_bShow, m_bShow );
#endif // REFORM_SKILL_NOTE_UI

}



bool CX2UISkillNote::OpenDialog()
{
	m_bShow = true;
	m_pDLGSkillNote->SetShowEnable(true, true);

	// �� ���Ⱦ��~
	g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_SKILL_NOTE);
	return true;

}

void CX2UISkillNote::SetLayer(X2_DIALOG_LAYER layer)
{	
	if( NULL != m_pDLGSkillNote )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGSkillNote, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGSkillNote, false );
	}
	
#ifdef REFORM_SKILL_NOTE_UI
	if( NULL != m_pDLGMemoList )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGMemoList, layer+1);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGMemoList, false );
	}
#endif // REFORM_SKILL_NOTE_UI
	// ���ֺ��� ���̾�α׶� ���� �׷�����.

}

void CX2UISkillNote::RegisterMemo(UidType iMemoId)
{
	m_bRegisterMemo = true;
	m_iRegisterMemo = iMemoId;
}

void CX2UISkillNote::Handler_EGS_REG_SKILL_NOTE_MEMO_REQ(char iPage)
{
	if( NULL != g_pTFieldGame &&
		true == g_pTFieldGame->IsNearPortalToBattleField() )
		return;

	if ( NULL != g_pX2Game &&
		CX2Game::GT_BATTLE_FIELD == g_pX2Game->GetGameType() &&
		true == static_cast<CX2BattleFieldGame*>(g_pX2Game)->IsNearPortalLineMap() )
		return;

	if( iPage <= 0 )
		return;

	CX2State* pState = (CX2State*)g_pMain->GetNowState();

	if( pState != NULL && 
		pState->GetCursor() != NULL &&
		pState->GetCursor()->GetCurorState() == CX2Cursor::XCS_REGISTER_MEMO &&
		m_bRegisterMemo == true && m_iRegisterMemo > 0 )
	{
		KEGS_REG_SKILL_NOTE_MEMO_REQ kEGS_REG_SKILL_NOTE_MEMO_REQ;

		kEGS_REG_SKILL_NOTE_MEMO_REQ.m_cSkillNotePageNum = iPage - 1;
		kEGS_REG_SKILL_NOTE_MEMO_REQ.m_iItemUID = m_iRegisterMemo;

		g_pData->GetServerProtocol()->SendPacket( EGS_REG_SKILL_NOTE_MEMO_REQ, kEGS_REG_SKILL_NOTE_MEMO_REQ );
		g_pMain->AddServerPacket( EGS_REG_SKILL_NOTE_MEMO_ACK );
	}
}



#ifdef REFORM_SKILL_NOTE_UI
void CX2UISkillNote::UpdateMemoSlot(int iPage)
{
	if( NULL == m_pDLGSkillNote )
		return;

	char cMaxPage = 0;
	if( NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
		cMaxPage = g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot();

//#1 ��� ���� ��� ���·� �ʱ�ȭ 
	CKTDGUIStatic *pStaticLockAndLV = 
		static_cast<CKTDGUIStatic*>(m_pDLGSkillNote->GetControl(L"MemoLock_And_LV"));
	for( int i = 0; i < 6; ++i )
	{
		WCHAR wcControlName[20] = {0,};
		StringCchPrintf( wcControlName, 20, L"_Icon_button%d", i);
		CKTDGUIButton *pButton = static_cast<CKTDGUIButton*>(m_pDLGSkillNote->GetControl(wcControlName));
		if( NULL != pButton ) 
		{
			pButton->SetEnable(false);
			pButton->SetGuideDesc(L"");
		}

		if( pStaticLockAndLV )
		{
			CKTDGUIControl::CPictureData *pPicture =  pStaticLockAndLV->GetPicture(i + 6);
			pPicture->SetTex(L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.TGA", L"Bg_Lock");
			pPicture->SetShow(true);
		}
	}

//#2 ���� ���� ���ο� ���� �ؽ��� ����
	for(char cSlotId = 0; cSlotId < 6 && cSlotId < cMaxPage; ++cSlotId)
	{
		if( cSlotId < m_iCurrentPage )
			continue;

		if( NULL != g_pData &&
			NULL != g_pData->GetMyUser() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			int iMemoId = g_pData->GetMyUser()->GetSelectUnit()->GetSkillMemoIdBySlot( cSlotId );

			// #3 �������� ���� �޸� ���Կ��� ��� �ؽ��ĸ� �����ϱ�.
			if( 0 >= iMemoId )
			{ // �� �޸� ���� 
				if( NULL != pStaticLockAndLV )
				{ 
					CKTDGUIControl::CPictureData *pPicture =  pStaticLockAndLV->GetPicture(cSlotId+6);
					pPicture->SetShow(false);
				}
			}
			// #4 ������ �޸� ������ �޸� ������ ���������� �ؽ��� �����ϱ�
			else
			{
				if( NULL != pStaticLockAndLV &&
					NULL != g_pData->GetItemManager() )
				{				
					CKTDGUIControl::CPictureData *pPicture =  pStaticLockAndLV->GetPicture(cSlotId+6);
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet(iMemoId);
					if( NULL != pItemTemplet )
					{
						pPicture->SetTex( pItemTemplet->GetShopImage() );				
						pPicture->SetShow(true);
					}
				}
			}

			// #5 ������ �޸� ���� ���� �߰�
			WCHAR wcControlName[20] = {0,};
			StringCchPrintf( wcControlName, 20, L"_Icon_button%d", static_cast<int>(cSlotId));
			CKTDGUIButton *pButton = static_cast<CKTDGUIButton*>(m_pDLGSkillNote->GetControl(wcControlName));
			if( NULL != pButton )
			{	
				pButton->SetEnable(true);
				if( NULL != g_pData->GetItemManager() ) 
				{
					const CX2Item::ItemTemplet* pkTemplet = g_pData->GetItemManager()->GetItemTemplet(iMemoId);
					if( NULL != pkTemplet )
					{
						wstring wstrMemoDesc = pkTemplet->GetName();
						wstrMemoDesc += L"\n\n";
						wstrMemoDesc += pkTemplet->GetDescription();
						pButton->SetGuideDesc( wstrMemoDesc.c_str() );
					}
				}
			}
		}
	}

	ViewMemo(iPage);
}
#else
void CX2UISkillNote::UpdateMemoSlot(int iPage)
{
	if( m_pDLGSkillNote == NULL )
		return;

	char cMaxPage = g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot();
	CKTDGUIStatic *pLvStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"Static_LV");
	CKTDGUIStatic *pPageStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"Static_Slot_Page");
	CKTDGUIStatic *pNoteStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"g_pStaticic_Note_of_Skill");

	
	for( int i = 0; i < 6; ++i )
	{
		WCHAR wcControlName[20] = {0,};

		//wsprintf(wcControlName, L"_Icon_button%d", i);
		StringCchPrintf( wcControlName, 20, L"_Icon_button%d", i);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGSkillNote->GetControl(wcControlName);
		if( pButton ) pButton->SetEnable(false);
	
		if( pLvStatic )
			pLvStatic->GetString(i)->msg = m_vecSlotLv[i + (int)m_iCurrentPage];

		if( pPageStatic )
		{
#ifdef CLIENT_COUNTRY_EU
			// 2011.08.03 ��ȫ�� �������� ��Ÿ���� ���ڿ��� ��Ʈ�� ���̺�� ����
			WCHAR wcPageNum[2];
			wstring wstrPage;
			_itow(i+(int)m_iCurrentPage+1, wcPageNum, 10);
			wstrPage = GET_STRING(STR_ID_13663);
			wstrPage += L" ";
			wstrPage += wcPageNum;
			
			pPageStatic->GetString(i)->msg = wstrPage;		
#else
#ifdef COUNTRY_STRING_CONVERT
			StringCchPrintf( wcControlName, 20, GET_STRING( STR_ID_28188 ), i+(int)m_iCurrentPage+1);
			pPageStatic->GetString(i)->msg = wcControlName;
#else
			//wsprintf( wcControlName, L"%d Page", i+(int)m_iCurrentPage+1);
			StringCchPrintf( wcControlName, 20, L"%d Page", i+(int)m_iCurrentPage+1);
			pPageStatic->GetString(i)->msg = wcControlName;
#endif
#endif //CLIENT_COUNTRY_EU
		
		}

		if( pNoteStatic )
		{
			CKTDGUIControl::CPictureData *pPicture =  pNoteStatic->GetPictureIndex(i+1);
			pPicture->SetTex(L"DLG_UI_Common_Texture29.TGA", L"Lock");
			pPicture->SetShow(true);
		}
	}
	

	// open�� �� ���� �˻��Ͽ� �޸��ϵ� �������� �� �������� �˻�	
	for(char cSlotId = 0; cSlotId < 6 && cSlotId < cMaxPage; ++cSlotId)
	{
		if( cSlotId < m_iCurrentPage )
			continue;		

		int iMemoId = g_pData->GetMyUser()->GetSelectUnit()->GetSkillMemoIdBySlot( cSlotId );

		if( iMemoId < 0 )
		{
			// �� ����
			if( pNoteStatic )
			{
				CKTDGUIControl::CPictureData *pPicture =  pNoteStatic->GetPictureIndex(cSlotId+1);
				pPicture->SetShow(false);
			}
		}
		else
		{
			// ��ϵ� ����
			if( pLvStatic )
			{
				pLvStatic->GetString((int)cSlotId)->msg = L"";
			}
			if( pNoteStatic )
			{				
				CKTDGUIControl::CPictureData *pPicture =  pNoteStatic->GetPictureIndex(cSlotId+1);
				pPicture->SetTex(
                    g_pData->GetItemManager()->GetItemTemplet(iMemoId)->GetShopImage()
                    );				
				pPicture->SetShow(true);
			}
		}

		WCHAR wcControlName[20] = {0,};
		//wsprintf(wcControlName, L"_Icon_button%d", (int)cSlotId);
		StringCchPrintf( wcControlName, 20, L"_Icon_button%d", (int)cSlotId);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGSkillNote->GetControl(wcControlName);
		if( pButton ) 
		{	
			pButton->SetEnable(true);
			if( iMemoId > 0 )
			{
                const CX2Item::ItemTemplet* pkTemplet = g_pData->GetItemManager()->GetItemTemplet(iMemoId);
                wstring wstrMemoDesc = pkTemplet->GetName();
				wstrMemoDesc += L"\n\n";
#ifdef CLIENT_GLOBAL_LINEBREAK
				wstrMemoDesc += CWordLineHandler::GetStrByLineBreakInX2Main( g_pData->GetItemManager()->GetItemTemplet(iMemoId)->GetDescription(), 330, 1);
#else //CLIENT_GLOBAL_LINEBREAK
				wstrMemoDesc += pkTemplet->GetDescription();
#endif //CLIENT_GLOBAL_LINEBREAK
				pButton->SetGuideDesc( wstrMemoDesc.c_str() );
			}
			else
				pButton->SetGuideDesc(L"");
		}
	}

	ViewMemo(iPage);
}
#endif // REFORM_SKILL_NOTE_UI

void CX2UISkillNote::UpdateMemo(bool bVal, char cPage)
{ 
	m_iUpdatePage = cPage;
	// �޸� ��� ��ϵǵ��� ����
	// ( ���� ������ UI�� �Բ� ���α׷����� ���Ծ��� )
#ifdef REFORM_SKILL_NOTE_UI 
	m_iViewPage = 0;
	UpdateMemoSlot(static_cast<int>(m_iUpdatePage));
	UpdateMemoEffect();
	m_pDLGInfoBox = g_pMain->KTDGUIInfoBox( D3DXVECTOR2( 250, 200 ), GET_STRING( STR_ID_5010), g_pMain->GetNowState(), 3.f);
	m_iUpdatePage = 0;

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"TechNote_CompleteWriting.ogg", false, false );
#else
	m_bUpdateMemo = bVal; 
	m_fWriteMemoTime = 2.f;

	if( m_bUpdateMemo == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGWrittingNote );
		m_pDLGWrittingNote = NULL;
		// ��Ʈ ���� ��
		m_pDLGWrittingNote = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Note_Loading.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWrittingNote );
		m_pDLGSkillNote->SetFront(true);		

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"TechNote_Writing.ogg", false, false );
	}
#endif // REFORM_SKILL_NOTE_UI

}

void CX2UISkillNote::ViewMemo(int iPage)
{
	char cMaxPage = g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot();

	if( g_pData == NULL )
		return;	

	if( m_pDLGSkillNote == NULL )
		return;

	if( iPage % 2 != 0 )
		iPage -= 1;

	if( m_iViewPage == iPage+1 )
		return;

	if( iPage < 0 || iPage >= cMaxPage )	
		return;		

	m_iViewPage = iPage+1;

	g_pKTDXApp->GetDeviceManager()->PlaySound( L"TechNote_FlipBook.ogg", false, false );

	WCHAR wcBuf[10] = {0,};
	int itemId1 = g_pData->GetMyUser()->GetSelectUnit()->GetSkillMemoIdBySlot(iPage);
	int itemId2 = g_pData->GetMyUser()->GetSelectUnit()->GetSkillMemoIdBySlot(iPage+1);
    const CX2Item::ItemTemplet* pkTemplet1 = g_pData->GetItemManager()->GetItemTemplet(itemId1);
    const CX2Item::ItemTemplet* pkTemplet2 = g_pData->GetItemManager()->GetItemTemplet(itemId2);

#ifdef SKILL_NOTE_SCROLLBAR
	CKTDGUIListBox* pListBox1 = (CKTDGUIListBox*)m_pDLGSkillNote->GetControl(L"MemoDesc1");
	CKTDGUIListBox* pListBox2 = (CKTDGUIListBox*)m_pDLGSkillNote->GetControl(L"MemoDesc2");
	if (pListBox1 != NULL)
		pListBox1->RemoveAllItems();
	if (pListBox2 != NULL)
		pListBox2->RemoveAllItems();
#endif //SKILL_NOTE_SCROLLBAR

#ifdef REFORM_SKILL_NOTE_UI // ��ų ������ ����
	CKTDGUIStatic *pSkillIcon = static_cast<CKTDGUIStatic*>(m_pDLGSkillNote->GetControl(L"g_pStaticic_SkillIcon"));
	if( NULL != pSkillIcon &&
		NULL != g_pData->GetSkillTree() )
	{		
		// ���� ������
		pSkillIcon->GetPicture(0)->SetTex( L"DLG_Common_New_Texture03.tga", L"invisible" );
		pSkillIcon->GetPicture(2)->SetTex( L"DLG_Common_New_Texture03.tga", L"invisible" );

		// ������ ������
		pSkillIcon->GetPicture(1)->SetTex( L"DLG_Common_New_Texture03.tga", L"invisible" );
		pSkillIcon->GetPicture(3)->SetTex( L"DLG_Common_New_Texture03.tga", L"invisible" );

		if( 0 < itemId1 )
		{
			int iSkillID_LeftPage = CX2SkillNoteManager::GetInstance()->GetSkillIDByMemoItemID( itemId1 );
			if( 0 == iSkillID_LeftPage )
			{
				iSkillID_LeftPage = CX2SkillNoteManager::GetInstance()->GetSkillIDByMemoItemID( itemId1, false );
				pSkillIcon->GetPicture(2)->SetTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Icon_Unserviceabillity" );
			}

			const CX2SkillTree::SkillTemplet* pSkillTemplet = 
				g_pData->GetSkillTree()->GetSkillTemplet(static_cast<CX2SkillTree::SKILL_ID>(iSkillID_LeftPage));

			if( NULL != pSkillTemplet )
				pSkillIcon->GetPicture(0)->SetTex( pSkillTemplet->GetIconTextureFileName(),pSkillTemplet->GetIconTextureKeyName() );
		}
		
		if( 0 < itemId2 )
		{
			int iSkillID_RightPage = CX2SkillNoteManager::GetInstance()->GetSkillIDByMemoItemID( itemId2 );
			if( 0 == iSkillID_RightPage )
			{
				iSkillID_RightPage = CX2SkillNoteManager::GetInstance()->GetSkillIDByMemoItemID( itemId2, false );
				pSkillIcon->GetPicture(3)->SetTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Icon_Unserviceabillity" );
			}

			const CX2SkillTree::SkillTemplet* pSkillTemplet = 
				g_pData->GetSkillTree()->GetSkillTemplet(static_cast<CX2SkillTree::SKILL_ID>(iSkillID_RightPage));

			if( NULL != pSkillTemplet )
				pSkillIcon->GetPicture(1)->SetTex( pSkillTemplet->GetIconTextureFileName(),pSkillTemplet->GetIconTextureKeyName() );
		}
	}
#else
	// ������
	CKTDGUIStatic *pPageStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"Static_Note_Page");
	if( pPageStatic != NULL )
	{
#ifdef CLIENT_COUNTRY_EU
		// 2011.08.03 ��ȫ�� �������� ��Ÿ���� ���ڿ��� ��Ʈ�� ���̺�� ����
		WCHAR wcPageNum[2];
		wstring wstrPage;

		_itow(iPage + 1, wcPageNum, 10);
		wstrPage = GET_STRING(STR_ID_13663);
		wstrPage += L" ";
		wstrPage += wcPageNum;
				
		//wsprintf(wcBuf, L"%d Page", iPage + 1);
		//StringCchPrintf( wcBuf, 10, L"%d Page", iPage + 1);
		pPageStatic->GetString(0)->msg = wstrPage;

		wstrPage.clear();

		_itow(iPage + 2, wcPageNum, 10);
		wstrPage = GET_STRING(STR_ID_13663);
		wstrPage += L" ";
		wstrPage += wcPageNum;
				
		//wsprintf(wcBuf, L"%d Page", iPage + 2);
		//StringCchPrintf( wcBuf, 10, L"%d Page", iPage + 2);
		pPageStatic->GetString(1)->msg = wstrPage;
#else
#ifdef COUNTRY_STRING_CONVERT
		StringCchPrintf( wcBuf, 10, GET_STRING( STR_ID_28188 ), iPage + 1);
		pPageStatic->GetString(0)->msg = wcBuf;
		StringCchPrintf( wcBuf, 10, GET_STRING( STR_ID_28188 ), iPage + 2);
		pPageStatic->GetString(1)->msg = wcBuf;
#else
		//wsprintf(wcBuf, L"%d Page", iPage + 1);
		StringCchPrintf( wcBuf, 10, L"%d Page", iPage + 1);
		pPageStatic->GetString(0)->msg = wcBuf;
		//wsprintf(wcBuf, L"%d Page", iPage + 2);
		StringCchPrintf( wcBuf, 10, L"%d Page", iPage + 2);
		pPageStatic->GetString(1)->msg = wcBuf;
#endif COUNTRY_STRING_CONVERT
#endif CLIENT_COUNTRY_EU
	}
#endif // REFORM_SKILL_NOTE_UI

	// Ÿ��Ʋ
	CKTDGUIStatic *pTitleStatic = NULL;	
	pTitleStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"Static_Title_A");
	if( pTitleStatic != NULL )
	{
		if( itemId1 > 0 && pkTemplet1 != NULL )
		{	
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrTitleA = L"";
			wstrTitleA = CWordLineHandler::GetStrByLineBreakColorInX2Main( pkTemplet1->GetName(), 190, 1 );
			pTitleStatic->GetString(0)->msg = wstrTitleA;
#else //CLIENT_GLOBAL_LINEBREAK
			pTitleStatic->GetString(0)->msg = pkTemplet1->GetName();
#endif //CLIENT_GLOBAL_LINEBREAK	
		}
		else
		{
			pTitleStatic->GetString(0)->msg = L"";
		}

	}	
	pTitleStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"g_pStatic_Title_B");
	if( pTitleStatic != NULL )
	{
		if( itemId2 > 0 && pkTemplet2 != NULL )
		{
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrTitleB = L"";
			wstrTitleB = CWordLineHandler::GetStrByLineBreakColorInX2Main( pkTemplet2->GetName(), 190, 1 );
			pTitleStatic->GetString(0)->msg = wstrTitleB;
#else //CLIENT_GLOBAL_LINEBREAK
			pTitleStatic->GetString(0)->msg = pkTemplet2->GetName();
#endif //CLIENT_GLOBAL_LINEBREAK	
		}
		else
		{
			pTitleStatic->GetString(0)->msg = L"";
		}
	}
#ifdef REFORM_SKILL_NOTE_UI
	// ���� ������ ����
	CKTDGUIStatic *pMemoStatic = NULL;
	pMemoStatic = static_cast<CKTDGUIStatic*>(m_pDLGSkillNote->GetControl(L"g_pStatic_Note_A"));
	if( NULL != pMemoStatic )
	{
		if( NULL != pkTemplet1 )
		{
			wstring wstrItemDesc = L"#C0185EB";
			wstrItemDesc += pkTemplet1->GetDescription();
			wstrItemDesc = g_pMain->GetStrByLineBreakColor( wstrItemDesc.c_str(), 250, 1 );
			wstrItemDesc += L"#CX\n\n";

			pMemoStatic->GetString(0)->msg = wstrItemDesc;
		}
		else
		{
			pMemoStatic->GetString(0)->msg = L"";
		}
	}
	pMemoStatic = static_cast<CKTDGUIStatic*>(m_pDLGSkillNote->GetControl(L"g_pStatic_Note_A2"));
	if( NULL != pMemoStatic )
	{
		if( NULL != pkTemplet1 )
		{
			wstring wstrMemoDesc = pkTemplet1->GetDescriptionInSkillNote();
			wstrMemoDesc = g_pMain->GetStrByLineBreakColor( wstrMemoDesc.c_str(), 250, 1 );

			pMemoStatic->GetString(0)->msg = wstrMemoDesc;
		}
		else
		{
			pMemoStatic->GetString(0)->msg = L"";
		}
	}

	// ������ ������ ����
	pMemoStatic = static_cast<CKTDGUIStatic*>(m_pDLGSkillNote->GetControl(L"g_pStatic_Note_b"));
	if( NULL != pMemoStatic )
	{
		if( NULL != pkTemplet2 )
		{
			wstring wstrItemDesc = L"#C0185EB";
			wstrItemDesc += pkTemplet2->GetDescription();
			wstrItemDesc = g_pMain->GetStrByLineBreakColor( wstrItemDesc.c_str(), 250, 1 );
			wstrItemDesc += L"#CX\n\n";

			pMemoStatic->GetString(0)->msg = wstrItemDesc;
		}
		else
		{
			pMemoStatic->GetString(0)->msg = L"";
		}
	}
	pMemoStatic = static_cast<CKTDGUIStatic*>(m_pDLGSkillNote->GetControl(L"g_pStatic_Note_b2"));
	if( NULL != pMemoStatic )
	{
		if( NULL != pkTemplet2 )
		{
			wstring wstrMemoDesc = pkTemplet2->GetDescriptionInSkillNote();
			wstrMemoDesc = g_pMain->GetStrByLineBreakColor( wstrMemoDesc.c_str(), 250, 1 );

			pMemoStatic->GetString(0)->msg = wstrMemoDesc;
		}
		else
		{
			pMemoStatic->GetString(0)->msg = L"";
		}
	}
#else
	// ����
	CKTDGUIStatic *pMemoStatic = NULL;
	pMemoStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"g_pStatic_Note_A");
	if( pMemoStatic != NULL )
	{
		wstring wstrItemDesc = L"";		

		if( itemId1 > 0 && pkTemplet1 != NULL )
		{			
			wstrItemDesc += L"#C0185EB";
#ifdef FIX_TOOLTIP
			wstrItemDesc += 
                pkTemplet1->GetDescription();
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrItemDesc.c_str(), 305, 1 );
#else //CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc = g_pMain->GetStrByLineBreakColor( wstrItemDesc.c_str(), 300, 1 );
#endif //CLIENT_GLOBAL_LINEBREAK
#else
			wstrItemDesc += ReplaceDesc_( 
                pkTemplet1->GetDescription(), 
                L"\n", L"\n#C0185EB" );
#endif FIX_TOOLTIP
			wstrItemDesc += L"#CX\n\n";

#ifdef FIX_TOOLTIP
			wstring wstrMemoDesc = 
                pkTemplet1->GetDescriptionInSkillNote();
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstrMemoDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrMemoDesc.c_str(), 305, 1 );
#else //CLIENT_GLOBAL_LINEBREAK
			wstrMemoDesc = g_pMain->GetStrByLineBreakColor( wstrMemoDesc.c_str(), 300, 1 );
#endif //CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc += wstrMemoDesc;
#else
			wstrItemDesc += 
                pkTemplet1->GetDescriptionInSkillNote()
#endif
		}

		pMemoStatic->GetString(0)->msg = wstrItemDesc;

#ifdef SKILL_NOTE_SCROLLBAR
		pMemoStatic->SetShow(false);

		wstring::size_type lastPosCond = wstrItemDesc.find_first_not_of(L"\n", 0);
		wstring::size_type posCond     = wstrItemDesc.find_first_of(L"\n", lastPosCond);

		while (wstring::npos != posCond || wstring::npos != lastPosCond)
		{        
			wstring tempStr = wstrItemDesc.substr(lastPosCond, posCond - lastPosCond);
			pListBox1->AddItem(tempStr.c_str(), NULL);

			lastPosCond = wstrItemDesc.find_first_not_of(L"\n", posCond);
			posCond = wstrItemDesc.find_first_of(L"\n", lastPosCond);
		}
#endif // SKILL_NOTE_SCROLLBAR
		
	}

	pMemoStatic = (CKTDGUIStatic*)m_pDLGSkillNote->GetControl(L"g_pStatic_Note_b");
	if( pMemoStatic != NULL )
	{
		wstring wstrItemDesc = L"";		

		if( itemId2 > 0 && pkTemplet2 != NULL )
		{
			wstrItemDesc += L"#C0185EB";
#ifdef FIX_TOOLTIP
			wstrItemDesc += 
                pkTemplet2->GetDescription();
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main( wstrItemDesc.c_str(), 305, 1 );
#else //CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc = g_pMain->GetStrByLineBreakColor( wstrItemDesc.c_str(), 300, 1 );
#endif //CLIENT_GLOBAL_LINEBREAK
#else
			wstrItemDesc += ReplaceDesc_( 
                pkTemplet2->GetDescription(),
                L"\n", L"\n#C0185EB" );
#endif FIX_TOOLTIP
			wstrItemDesc += L"#CX\n\n";

#ifdef FIX_TOOLTIP
			wstring wstrMemoDesc = 
                pkTemplet2->GetDescriptionInSkillNote();
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstrMemoDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main(wstrMemoDesc.c_str(), 305, 1 );
#else //CLIENT_GLOBAL_LINEBREAK
			wstrMemoDesc = g_pMain->GetStrByLineBreakColor( wstrMemoDesc.c_str(), 300, 1 );
#endif //CLIENT_GLOBAL_LINEBREAK
			wstrItemDesc += wstrMemoDesc;
#else
			wstrItemDesc += 
                pkTemplet2->GetDescriptionInSkillNote();
#endif
		}

		pMemoStatic->GetString(0)->msg = wstrItemDesc;

#ifdef SKILL_NOTE_SCROLLBAR
		pMemoStatic->SetShow(false);

		wstring::size_type lastPosCond = wstrItemDesc.find_first_not_of(L"\n", 0);
		wstring::size_type posCond     = wstrItemDesc.find_first_of(L"\n", lastPosCond);

		while (wstring::npos != posCond || wstring::npos != lastPosCond)
		{        
			wstring tempStr = wstrItemDesc.substr(lastPosCond, posCond - lastPosCond);
			pListBox2->AddItem(tempStr.c_str(), NULL);

			lastPosCond = wstrItemDesc.find_first_not_of(L"\n", posCond);
			posCond = wstrItemDesc.find_first_of(L"\n", lastPosCond);
		}
#endif // SKILL_NOTE_SCROLLBAR
	}
#endif // REFORM_SKILL_NOTE_UI

}

void CX2UISkillNote::CheckRegisterMemo()
{
	int iMemo = 0;
	int iItemId = 0;
	CX2Item *pItem = NULL;

	if( g_pData == NULL || 
		g_pData->GetMyUser() == NULL || 
		g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return;
	pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iRegisterMemo );
	if( pItem == NULL )
		return;	
    iItemId = pItem->GetItemTemplet()->GetItemID();
	

	if( m_iRegisterPage > 0 && iItemId > 0 )
	{
		char cMaxPage = g_pData->GetMyUser()->GetSelectUnit()->GetMaxSkillNoteSlot();
		for(char i=0; i<cMaxPage; ++i)
		{
			iMemo = g_pData->GetMyUser()->GetSelectUnit()->GetSkillMemoIdBySlot(i);			
			if( iMemo > 0 && iMemo == iItemId )
			{
				// ���� �޸� ��ϵǾ� ����
				if( m_pDLGQuery == NULL )
				{
					m_pDLGQuery = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4992), g_pMain->GetNowState() );
				}
				return;
			}
		}

		iMemo = g_pData->GetMyUser()->GetSelectUnit()->GetSkillMemoIdBySlot(m_iRegisterPage-1);
		if( iMemo > 0 )
		{
			// ����� �������� �ٸ� �޸� ��ϵǾ� ����
			if( m_pDLGQuery == NULL )
			{
				m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4991), USNM_REGISTER_OK, g_pMain->GetNowState(), USNM_REGISTER_CANCEL );
			}			
			return;
		}
		else
		{
			// ���ִ� ������
			if( m_pDLGQuery == NULL )
			{
				m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4990), USNM_REGISTER_OK, g_pMain->GetNowState(), USNM_REGISTER_CANCEL );
			}			
			return;
		}
	}
	else
	{
		// �޸��� �Ұ�����
		return;
	}
}

void CX2UISkillNote::UpdateMemoEffect()
{
	if( g_pData != NULL && g_pData->GetUIMajorParticle() != NULL && m_pDLGSkillNote != NULL && GetShow() == true )
	{
		WCHAR wcControlName[20] = {0,};
		//wsprintf(wcControlName, L"_Icon_button%d", m_iUpdatePage - m_iCurrentPage);
		StringCchPrintf(wcControlName, 20, L"_Icon_button%d", m_iUpdatePage - m_iCurrentPage);
		CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGSkillNote->GetControl(wcControlName);

		D3DXVECTOR2 vEffectPos = D3DXVECTOR2(0.f, 0.f);			

		vEffectPos = pButton->GetDialog()->GetPos() + pButton->GetPos();
		const float fButtonSizeX = (float)pButton->GetWidth();
		const float fButtonSizeY = (float)pButton->GetHeight();
		vEffectPos.x += fButtonSizeX/2;
		vEffectPos.y += fButtonSizeY/2;

		CKTDGParticleSystem::CParticleEventSequence* pParticle1 = 
			g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"UI_Memo_StartEffect01", vEffectPos.x, vEffectPos.y, 0  );
		if( pParticle1 != NULL )
		{
			if( m_hParticle1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle1);
			}

			pParticle1->SetOverUI( true );
			m_hParticle1 = pParticle1->GetHandle();
		}

		CKTDGParticleSystem::CParticleEventSequence* pParticle2 = 
			g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"UI_Memo_StartEffect02", vEffectPos.x, vEffectPos.y, 0  );
		if( pParticle2 != NULL )
		{
			if( m_hParticle2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle2);
			}

			pParticle2->SetOverUI( true );
			m_hParticle2 = pParticle2->GetHandle();
		}
	}	
}

void CX2UISkillNote::ResetSkillNote()
{
	m_bRegisterMemo = false;
	m_iRegisterMemo = 0;
	m_iUpdatePage = 0;
#ifndef REFORM_SKILL_NOTE_UI // �޸� ��� ��ϵǵ��� ����
	m_bUpdateMemo = false;
	m_fWriteMemoTime = 0.f;	
#endif // REFORM_SKILL_NOTE_UI

	if( g_pMain->GetInfoBox() != NULL )
		g_pMain->DeleteInfoBox();

	if( m_hParticle1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle1);	
	if( m_hParticle2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle2);	
}

wstring CX2UISkillNote::ReplaceDesc_(const wchar_t* pwstrOri, const wchar_t* pwstrPattern, const wchar_t* pwstrReplace)
{
	wstring result = pwstrOri;   
	wstring::size_type pos = 0;   
	wstring::size_type offset = 0;   

    unsigned uSizePattern = wcslen( pwstrPattern );
    unsigned uSizeReplace = wcslen( pwstrReplace );

	while((pos = result.find(pwstrPattern, offset, uSizePattern)) != wstring::npos)   
	{   
		result.replace(result.begin() + pos, result.begin() + pos + uSizePattern, pwstrReplace, uSizeReplace);   
		offset = pos + uSizeReplace;   
	}   

	return result;   
}


#endif

#ifdef REFORM_SKILL_NOTE_UI
/** @function : ResetMemoList()
	@brief : �޸� ���� ���� ���� �� ����
*/
void CX2UISkillNote::ResetMemoList()
{
	// �޸� ���� ���� ���� �� ���� ���ο� ���� ����
	CX2SkillNoteManager::GetInstance()->UpdateMemoPossession();
	CX2SkillNoteManager::GetInstance()->UpdateSortMemoList();

	// ���� ���� �°� UI ����
	ResetMemoListUI();
}

/** @function : ResetMemoListUI
	@brief : �޸� ����Ʈ ���� ��, ���� ������ �޸𸮽�Ʈ�� ������ ����
*/
void CX2UISkillNote::ResetMemoListUI()
{
	if( NULL == m_pDLGMemoList )
	{
		ResetMemoListDLG();
	}

	if( NULL != m_pDLGMemoList )
	{
		CKTDGUIContolList *pControlList = static_cast<CKTDGUIContolList*>(m_pDLGMemoList->GetControl(L"MemoControlList"));
		if( pControlList != NULL )
		{
			int iRowIndex = 0;
			BOOST_FOREACH( const CX2SkillNoteManager::MemoData& sMemoData, CX2SkillNoteManager::GetInstance()->GetUsableMemoListIfIHave() )
			{
				WCHAR wStaticName[256] = {0,};
//------------------------�޸� ���� ���� ����
				StringCchPrintf(wStaticName, ARRAY_SIZE(wStaticName), L"SkillNote_Memo_%d", iRowIndex);
				// �ش� row�� �������� ������ ��ũ��Ʈ �Ľ��ؼ� ���� �����
				bool bIsNewOpenScript = false;
				CKTDGUIStatic *pStaticMemo = NULL;
				if( false == m_pDLGMemoList->CheckControl(wStaticName) )
				{
					m_pDLGMemoList->OpenScriptFile(L"DLG_MemoList_ControlList.lua");
					pStaticMemo = static_cast<CKTDGUIStatic*>(m_pDLGMemoList->GetControl( L"SkillNote_Memo"));	
					pStaticMemo->SetName( wStaticName );
					bIsNewOpenScript = true;
				}
				else
				{
					pStaticMemo = static_cast<CKTDGUIStatic*>(m_pDLGMemoList->GetControl( wStaticName ));	
				}
				SetMemoListData_MemoData( pStaticMemo, sMemoData.iSkillID, sMemoData.iMemoItemID, sMemoData.iMemoItemNum );
				pControlList->InsertItem( pStaticMemo,iRowIndex, 0 );	

//------------------------�̺��� �޸�� ��Ȱ��ȭ ��Ű��
				CKTDGUIStatic *pStaticBlack = NULL;
				// �ش� row�� �������� ������ ��ũ��Ʈ �Ľ��ؼ� ���� �����
				StringCchPrintf(wStaticName, ARRAY_SIZE(wStaticName), L"SkillNote_Black_%d", iRowIndex);
				if( true == bIsNewOpenScript )
				{
					pStaticBlack = static_cast<CKTDGUIStatic*>(m_pDLGMemoList->GetControl( L"SkillNote_Black"));	
					if( NULL != pStaticBlack )
						pStaticBlack->SetName( wStaticName );
				}
				else
				{
					pStaticBlack = static_cast<CKTDGUIStatic*>(m_pDLGMemoList->GetControl( wStaticName ));	
				}

				if( NULL != pStaticBlack )
				{
					pStaticBlack->SetShow(!( 0 < sMemoData.iMemoItemNum ));
					pControlList->InsertItem( pStaticBlack,iRowIndex, 1 );	
				}

//------------------------����(�̺���) �޸�� �޸𾲱�(����) ��ư
// ������ ���� ���¿� ���� ��ư �ؽ��� ���� 
				CKTDGUIButton *pButton = NULL;
				// �ش� row�� �������� ������ ��ũ��Ʈ �Ľ��ؼ� ���� �����
				StringCchPrintf(wStaticName, 256, L"SkillNote_Button_%d", iRowIndex);
				if( true == bIsNewOpenScript )
				{
					pButton = static_cast<CKTDGUIButton*>(m_pDLGMemoList->GetControl( L"SkillNote_Button" ));
					if( NULL != pButton )
						pButton->SetName( wStaticName );
				}
				else
				{
					pButton = static_cast<CKTDGUIButton*>(m_pDLGMemoList->GetControl( wStaticName ));
				}

				SetMemoListData_MemoButton(pButton, sMemoData.iMemoItemID, sMemoData.iMemoItemNum );
				pControlList->InsertItem( pButton,	iRowIndex, 2 );
				++iRowIndex;
			}
			pControlList->SetIndex(0, 0);
		}
	}
}

/** @function : SetMemoListData_MemoButton
	@brief : �޸� ����Ʈ�� ��Ʈ�� ����Ʈ ���� ��ư ����
*/
void CX2UISkillNote::SetMemoListData_MemoButton( CKTDGUIButton *pButton_, int iMemoItemID_, int iMemoItemNum_)
{
	if( NULL != pButton_ )
	{
		if( 0 < iMemoItemNum_ )
		{
			pButton_->SetOverTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Write_O" );
			pButton_->SetDownTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Write_O" );
			pButton_->SetCustomMsgMouseUp(USNM_USE_MEMO);
			pButton_->SetCustomMsgMouseOver(USNM_HAS_MEMO_MOUSE_OVER);
		}
		else
		{
			pButton_->SetOverTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Buy_O" );
			pButton_->SetDownTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Buy_O" );
			pButton_->SetCustomMsgMouseUp(USNM_BUY_MEMO);
			pButton_->SetCustomMsgMouseOver(USNM_NOT_HAS_MEMO_MOUSE_OVER);
		}

		if( -1 == pButton_->GetDummyInt(0) &&
			iMemoItemID_ > 0 )
		{
			pButton_->AddDummyInt( iMemoItemID_ );
		}
		else
		{
			pButton_->SetDummyInt(0, iMemoItemID_ );
		}
	}
}
/** @function : SetMemoListData_MemoData
	@brief : �޸� ����Ʈ�� ��Ʈ�� ����Ʈ ���� ��ų������, �޸��̸� ����
*/
void CX2UISkillNote::SetMemoListData_MemoData( CKTDGUIStatic *pStaticMemo_, int iSkillID_, int iMemoItemID_, int iMemoItemNum_ )
{
	//------------------------�޸� ���� ���� ����
	if( NULL != pStaticMemo_ )
	{ 	
		// ��ų ������ �߰�
		if( NULL != pStaticMemo_->GetPicture(1) &&
			NULL != g_pData->GetSkillTree() )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = 
				g_pData->GetSkillTree()->GetSkillTemplet(static_cast<CX2SkillTree::SKILL_ID>(iSkillID_));

			if( NULL != pSkillTemplet )
				pStaticMemo_->GetPicture(1)->SetTex( pSkillTemplet->GetIconTextureFileName(),pSkillTemplet->GetIconTextureKeyName() );
		}
		// ���� ���¿� ���� ��� �ؽ��� �����ϱ�
		if( NULL != pStaticMemo_->GetPicture(0) )
		{
			if( 0 < iMemoItemNum_ )
				pStaticMemo_->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_N" );
			else
				pStaticMemo_->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Empty" );
		}

		// �޸� �����۸� ����
		if( NULL != pStaticMemo_->GetString(0) && 
			NULL != g_pData->GetItemManager() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iMemoItemID_ );
			if( NULL != pItemTemplet )
			{
				// �޸� ������ �̸�																
				pStaticMemo_->GetString(0)->msg = pItemTemplet->GetName(); 

				// �ٹٲ� ó��
				CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStaticMemo_->GetString(0)->fontIndex );
				const int MAGIC_TEXT_WIDTH = 70;
				g_pMain->LineBreak( pStaticMemo_->GetString(0)->msg , pFont, static_cast<int>(static_cast<float>(MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX())), L"", true );

			}
		}
		// �޸� ������ ���� ����
		if( NULL != pStaticMemo_->GetString(1) )
		{
			pStaticMemo_->GetString(1)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", max(iMemoItemNum_,0)));
		}
		pStaticMemo_->SetShow(true);
	}
}
/** @function : UpdateMemoList_MemoItemNum
	@brief : �޸� ������ ���� ����.
			 ���Ļ��´� ���������ʰ� ������ ���� �ϴ� ����.		
			(����� ��Ʈ�� ������ ���¿����� �ҷ� �� �� �ִ� )
*/
void CX2UISkillNote::UpdateMemoList_MemoItemNum()
{
	// ����� ��Ʈ�� ������ ���¿����� �ҷ� �� �� �ִ� 
	if( false == m_bShow )
		return;

	// �޸� ���� ���� ����
	CX2SkillNoteManager::GetInstance()->UpdateMemoPossession();

	// ������ Ȱ��/��Ȱ��ȭ ���� ����
	int iRowIndex = 0;
	BOOST_FOREACH( const CX2SkillNoteManager::MemoData sMemoData, CX2SkillNoteManager::GetInstance()->GetUsableMemoListIfIHave() )
	{
		CKTDGUIContolList *pControlList = static_cast<CKTDGUIContolList*>(m_pDLGMemoList->GetControl(L"MemoControlList"));
		if( NULL != pControlList )
		{
			WCHAR wStaticName[256] = {0,};
			StringCchPrintf(wStaticName, ARRAY_SIZE(wStaticName), L"SkillNote_Memo_%d", iRowIndex);
			CKTDGUIStatic *pStaticMemo = static_cast<CKTDGUIStatic*>(m_pDLGMemoList->GetControl( wStaticName ));	
			if( NULL != pStaticMemo )
			{ 	
				//------------------------�޸� ������ ���� ����
				if( NULL != pStaticMemo->GetString(1) )
					pStaticMemo->GetString(1)->msg = GET_REPLACED_STRING((STR_ID_2632, "i", max(sMemoData.iMemoItemNum,0)));

				// ���� ���¿� ���� ��� �ؽ��� �����ϱ�
				if( NULL != pStaticMemo->GetPicture(0) )
				{
					if( 0 < sMemoData.iMemoItemNum )
						pStaticMemo->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_N" );
					else
						pStaticMemo->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Empty" );
				}
			}


			//------------------------�̺��� �޸�� ��Ȱ��ȭ ��Ű��
			StringCchPrintf(wStaticName, ARRAY_SIZE(wStaticName), L"SkillNote_Black_%d", iRowIndex);
			CKTDGUIStatic *pStaticBlack = static_cast<CKTDGUIStatic*>(m_pDLGMemoList->GetControl( wStaticName ));	
			if( NULL != pStaticBlack )
				pStaticBlack->SetShow(!( 0 < sMemoData.iMemoItemNum ));
			


			//------------------------������ ���� ���¿� ���� ��ư �ؽ��� ���� 
			StringCchPrintf(wStaticName, 256, L"SkillNote_Button_%d", iRowIndex);
			CKTDGUIButton *pButton = static_cast<CKTDGUIButton*>(m_pDLGMemoList->GetControl( wStaticName ));
			if( NULL != pButton )
			{
				if( 0 < sMemoData.iMemoItemNum )
				{
					pButton->SetOverTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Write_O" );
					pButton->SetDownTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Write_O" );
					pButton->SetCustomMsgMouseUp(USNM_USE_MEMO);
					pButton->SetCustomMsgMouseOver(USNM_HAS_MEMO_MOUSE_OVER);
				}
				else
				{
					pButton->SetOverTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Buy_O" );
					pButton->SetDownTex( L"DLG_UI_Common_Texture_GUI_NoteOfSkill_01.tga", L"Bt_MemoList_Buy_O" );
					pButton->SetCustomMsgMouseUp(USNM_BUY_MEMO);
					pButton->SetCustomMsgMouseOver(USNM_NOT_HAS_MEMO_MOUSE_OVER);
				}
			}
		}
		++iRowIndex;
	}
}
void CX2UISkillNote::ResetMemoListDLG()
{
	SAFE_DELETE_DIALOG( m_pDLGMemoList );

	m_pDLGMemoList = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_MemoList.lua" );
	if( NULL != m_pDLGMemoList )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMemoList );
		m_pDLGMemoList->SetDisableUnderWindow(true);
		m_pDLGMemoList->SetFront(true);
		m_pDLGMemoList->SetDisableUnderWindow(true);
		m_pDLGMemoList->SetShowEnable(false, false);

		if( NULL != m_pDLGSkillNote )
			m_pDLGMemoList->SetLayer( m_pDLGSkillNote->GetLayer()+1 );
	}
}
/** @function : ShowMemoDesc
	@brief : �޸� ���� ����
*/
void CX2UISkillNote::GetMemoDesc( OUT wstring& wstrMemoDesc_, int iMemoItemID_ /*= 0*/, bool bHas_ /*= false*/)
{
	// �ʼ� ���� ����
	if( NULL == g_pData->GetSkillTree() )
		return;

	if( NULL == g_pData->GetItemManager() )
		return;

	// �޸� ������ ���ø�
	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iMemoItemID_ );
	if( NULL == pItemTemplet )
		return;

	// ��ų ������ ���ø�
	int iSkillID = CX2SkillNoteManager::GetInstance()->GetSkillIDByMemoItemID(iMemoItemID_);

	const CX2SkillTree::SkillTemplet* pSkillTemplet = 
		g_pData->GetSkillTree()->GetSkillTemplet(static_cast<CX2SkillTree::SKILL_ID>(iSkillID));
	if( NULL == pSkillTemplet )
		return;
	//#note : pItemTemplet, pSkillTemplet �� üũ �Ϸ�

	const int MAGIC_TEXT_WIDTH = 250;
	if( true == bHas_ )
	{
		// #1 �޸� ������ �̸�
		wstrMemoDesc_ += pItemTemplet->GetNameColor_();
		wstrMemoDesc_ += pItemTemplet->GetFullName_();
		wstrMemoDesc_ += L"\n";
		// #2 ���� ����
		if( NULL != g_pData->GetUnitManager() )
		{
			wstrMemoDesc_ += L"#CFF611B";
			const CX2Unit::UnitTemplet* pUnitTemplet 
				= g_pData->GetUnitManager()->GetUnitTemplet( pItemTemplet->GetUnitClass() );

			if( NULL != pUnitTemplet )
				wstrMemoDesc_ += GET_REPLACED_STRING( ( STR_ID_4987, "L", pUnitTemplet->m_Description ) );

			wstrMemoDesc_ += L"\n";
		}

		// #3 �䱸 ����
		if( g_pData->GetSelectUnitLevel() < pItemTemplet->GetUseLevel() )
			wstrMemoDesc_ += L"#CFF0000";

		wstrMemoDesc_ += GET_REPLACED_STRING( ( STR_ID_279, "i", static_cast<int>(pItemTemplet->GetUseLevel()) ) );
		wstrMemoDesc_ += L"#CX";

		// #3 �޸� ����
		CKTDGFontManager::CUKFont* pDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 2 );
		wstring wstrMemoItemDesc = pItemTemplet->GetDescription();
		g_pMain->LineBreak( wstrMemoItemDesc, pDescFont, static_cast<int>(static_cast<float>(MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX())), L"", true );

		wstrMemoDesc_ += wstrMemoItemDesc;
		wstrMemoDesc_ += L"\n\n";

		// #4 ��ȭ ��ų �̸�
		wstrMemoDesc_ += L"#CFFFF00";
		wstrMemoDesc_ += GET_STRING(STR_ID_29352);
		wstrMemoDesc_ += L"\n#CFFFF00 - ";
		wstrMemoDesc_ += pSkillTemplet->m_wstrName;
		wstrMemoDesc_ += L"\n";
	}
	else
	{

		CKTDGFontManager::CUKFont* pDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 2 );
		wstring wstrGuide = GET_STRING(STR_ID_29351);
		g_pMain->LineBreak( wstrGuide, pDescFont, static_cast<int>(static_cast<float>(MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX())), L"", true );

		wstrMemoDesc_ += wstrGuide;
		wstrMemoDesc_ += L"\n";

		// #1 �޸� ������ �̸�
		wstrMemoDesc_ += pItemTemplet->GetNameColor_();
		wstrMemoDesc_ += pItemTemplet->GetFullName_();
		wstrMemoDesc_ += L"\n";

		// #3 �޸� ����
		wstring wstrMemoItemDesc = pItemTemplet->GetDescription();
		g_pMain->LineBreak( wstrMemoItemDesc, pDescFont, static_cast<int>(static_cast<float>(MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX())), L"", true );

		wstrMemoDesc_ += wstrMemoItemDesc;
		wstrMemoDesc_ += L"\n\n";


		wstrMemoDesc_ += GET_STRING(STR_ID_29350);
		wstrMemoDesc_ += L"\n";

#ifdef _IN_HOUSE_
		wstrMemoDesc_ += L"MemoItemID : ";
		wstrMemoDesc_ += GET_REPLACED_STRING((STR_ID_2632, "i", iMemoItemID_));
#endif // m_vecSkillDataMap
	}
}
#endif // REFORM_SKILL_NOTE_UI