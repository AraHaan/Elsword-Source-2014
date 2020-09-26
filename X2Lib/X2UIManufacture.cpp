#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI

#include ".\X2UIManufacture.h"

// item slot���� �����ϱ� ���ؼ� ������� slot�� CX2Slot::ST_MATERIAL�� �Ǿ��ְ�, ������� ������ slot�� CX2Slot::ST_CASH_SHOP_MAIN_NEW�� �Ǿ��ִ�.
// GetSlotperPage�� �̿��ؼ� ���� ������ ���ϰ� �ִ� : X2Define�� �ִ� define ���� ���� �ʰ� ����.

CX2UIManufacture::CX2UIManufacture( CKTDXStage* pNowStage, const WCHAR* pFileName )
// : CX2UIDragable( pNowStage, NULL )
: CX2ItemSlotManager( pNowStage, NULL ),
m_bShow(false),
m_MovedPosition(0,0),					// D3DXVECTOR2
m_DLGPosition(0,0),						// D3DXVECTOR2
m_DLGSize(0,0),							// D3DXVECTOR2
m_pDLGManufacture(NULL),
m_pDLGManufactureResult(NULL),
m_pDLGMakeQuery(NULL),
//m_vecRecipeSlot.clear();
m_nNowPage(1),
m_nMaxPage(1),
m_bShowAllCharItem(false),
m_SelectedRecipeID(0),
//m_vecManufactureIDList[MT_END]
m_NowItemCategory(MT_NONE)
//{{ kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
, m_iNowOpenHouseID(0)	// ���� ���� ���� NPC�� HOUSE_ID
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
//}} kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef SERV_MANUFACTURE_FIX
, m_pDLGItemEnroll(NULL)
, m_EnrollItemUid( 0 )
, m_nQuantity( 1 )
#endif
, m_bWaitForManufactureResult(false)
{

	m_vecRecipeSlot.clear();

	for(int i=0; i<MT_END; ++i)
	{
		m_vecManufactureIDList[i].clear();
	}
}


CX2UIManufacture::~CX2UIManufacture(void)
{
	SAFE_DELETE_DIALOG(m_pDLGManufacture);
	SAFE_DELETE_DIALOG(m_pDLGManufactureResult);
	SAFE_DELETE_DIALOG(m_pDLGMakeQuery);	

	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2Slot* pSlot = m_SlotList[i];
		SAFE_DELETE(pSlot);
	}
	m_SlotList.clear();
	for(UINT j=0; j<m_vecRecipeSlot.size(); j++)
	{
		ManufactureSlot* pMSlot = m_vecRecipeSlot[j];
		SAFE_DELETE(pMSlot);
	}
	m_vecRecipeSlot.clear();

#ifdef SERV_MANUFACTURE_FIX
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
#endif
}



HRESULT CX2UIManufacture::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();

	if(m_pDLGManufacture != NULL && m_pDLGManufacture->GetIsMouseOver() == true)
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

bool CX2UIManufacture::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
			if ( false == m_pDLGManufacture->GetIsMouseOver())
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
			if ( false == m_pDLGManufacture->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_MANUFACTURE, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}


	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{			
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlotInMousePos( mousePos );

// 			if ( pSlotItem != NULL && pSlotItem->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW )
// 			{
// 
// 				if ( pSlotItem->GetSlotID() >= 0 && pSlotItem->GetSlotID() < (int)m_vecRecipeSlot.size() )
// 				{
// 					ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[ pSlotItem->GetSlotID() ];
// 					if ( pRecipeSlot != NULL )
// 					{
// 						g_pKTDXApp->GetDeviceManager()->PlaySound( L"X2_Button_Mouse_Up.ogg" );
// 						SelectRecipe( pSlotItem->GetSlotID() );
// 					}
// 				}
// 			}
			return true;

		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseMove( mousePos );
			return true;
		}
		break;
	}

	return bFlag;
}
// 
// bool CX2UIManufacture::MouseRButtonUp( D3DXVECTOR2 mousePos )
// {
// 	CX2SlotItem* pSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
// 	if(pSlot == NULL) return false;
// 
// 	// ���� ���콺���� �̹����� �����ְ�
// 	m_pDLGSelectedItem->SetShow( false );
// 	InvalidSlotDesc();
// 
// 	return true;
// 
// }

bool CX2UIManufacture::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UMCM_EXIT:
		{
			if(m_bWaitForManufactureResult == true)
				return true;

			SetShow(false);

			if(g_pTFieldGame != NULL)
			{
				CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
				if(pJoinNpc != NULL)
				{
					pJoinNpc->GetNpcShop()->SetKeyEvent();
				}
			}
		}
		return true;
	case UMCM_CATEGORY_WEAPON:
		{
			m_NowItemCategory = MT_WEAPON;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_COAT:
		{
			m_NowItemCategory = MT_DEFENCE_BODY;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_PANTS:
		{
			m_NowItemCategory = MT_DEFENCE_LEG;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_GLOVE:
		{
			m_NowItemCategory = MT_DEFENCE_HAND;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_SHOES:
		{
			m_NowItemCategory = MT_DEFENCE_FOOT;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_QUICKSLOT:
		{
			m_NowItemCategory = MT_QUICK_SLOT;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_CATEGORY_ETC:
		{
			m_NowItemCategory = MT_ETC;
			ChangeManufactureTab( 1 );
		}
		return true;
	case UMCM_SHOW_OTHER_CHARACTER_EQUIP_ON:
		{
			if(m_bShowAllCharItem == false)
			{
				m_bShowAllCharItem = true;
				GetManufactureList();
				ChangeManufactureTab( 1 );
			}
		}
		return true;
	case UMCM_SHOW_OTHER_CHARACTER_EQUIP_OFF:
		{
			if(m_bShowAllCharItem == true)
			{
				m_bShowAllCharItem = false;
				GetManufactureList();
				ChangeManufactureTab( 1 );
			}
		}
		return true;
	case UMCM_PREV_PAGE:
		{
			if( m_nNowPage > 1 ) m_nNowPage--;
			ChangeManufactureTab( m_nNowPage );
		}
		return true;
	case UMCM_NEXT_PAGE:
		{
			if( m_nNowPage < m_nMaxPage ) m_nNowPage++;
			ChangeManufactureTab( m_nNowPage );
		}
		return true;
	case UMCM_SELECT_ITEM:
		{
			// �̷� �� �� ��-_-
			// ������ ����Ʈ�� ���� MsgProc : MouseUp�� �ֽ��ϴ�.

			// �� �� ��ȹ �ٲ�µ�-_- �̰� �и��� ���� �����ش޶�� �߾��ݾ� ��
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			if( pButton->GetDummyInt(0) >= 0 && pButton->GetDummyInt(0) < (int)m_vecRecipeSlot.size())
			{
				ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[ pButton->GetDummyInt(0) ];
				if ( pRecipeSlot != NULL )
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"X2_Button_Mouse_Up.ogg" );
					SelectRecipe( pButton->GetDummyInt(0) );
				}

			}
// 			if ( pSlotItem != NULL && pSlotItem->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW )
// 			{
// 
// 				if ( pSlotItem->GetSlotID() >= 0 && pSlotItem->GetSlotID() < (int)m_vecRecipeSlot.size() )
// 				{
// 					ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[ pSlotItem->GetSlotID() ];
// 					if ( pRecipeSlot != NULL )
// 					{
// 						g_pKTDXApp->GetDeviceManager()->PlaySound( L"X2_Button_Mouse_Up.ogg" );
// 						SelectRecipe( pSlotItem->GetSlotID() );
// 					}
// 				}
// 			}

		}
		return true;
	case UMCM_MAKE:
		{
			if( CheckManufacture() == true )
			{
				if( m_pDLGMakeQuery == NULL )
				{
					m_pDLGMakeQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5018), UMCM_MAKE_OK, g_pMain->GetNowState() );
					if( m_pDLGMakeQuery != NULL )
					{
						CKTDGUIButton* pCancelButton = (CKTDGUIButton*)m_pDLGMakeQuery->GetControl( L"MsgBoxOkAndCancelCancelButton" );		
						pCancelButton->RequestFocus();
					}
				}	
			}
			else
			{
#ifdef SERV_MANUFACTURE_FIX
				if(CheckCanMake(m_SelectedRecipeID, m_nQuantity))
				{
					// ���� : �������̸� ���� ���� �Ŀ� ���� ��Ŷ�� �������� ����
					if(CheckIsQuantity(m_SelectedRecipeID))
					{
						OpenRegisterQuantityDLG();
					}
					else
						return Handler_EGS_ITEM_MANUFACTURE_REQ();
				}
				else
					return false;
#else
				return Handler_EGS_ITEM_MANUFACTURE_REQ();
#endif //SERV_MANUFACTURE_FIX
			}			
		}
		return true;
	case UMCM_RESULT_CLOSE:
		{
			if ( m_pDLGManufactureResult != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGManufactureResult, NULL, false );

			m_pDLGManufactureResult = NULL;            
		}
		return true;
	case UMCM_MAKE_OK:
		{
			if( m_pDLGMakeQuery != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMakeQuery, NULL, false );
				m_pDLGMakeQuery = NULL;
			}			
#ifdef SERV_MANUFACTURE_FIX
			if(CheckCanMake(m_SelectedRecipeID, m_nQuantity))
			{
				// ���� : �������̸� ���� ���� �Ŀ� ���� ��Ŷ�� �������� ����
				if(CheckIsQuantity(m_SelectedRecipeID))
				{
					OpenRegisterQuantityDLG();
				}
				else
					return Handler_EGS_ITEM_MANUFACTURE_REQ();
			}
			else
				return false;
#else
			return Handler_EGS_ITEM_MANUFACTURE_REQ();
#endif //SERV_MANUFACTURE_FIX
		}
		break;	
#ifdef SERV_MANUFACTURE_FIX
	case UMCM_REGISTER_QUANTITY_PLUS:
		{
			if(CheckCanMake(m_SelectedRecipeID, m_nQuantity + 1))
			{
				m_nQuantity++;
				UpdateRegisterQuantityDLG(false);
				ResetRecipeMaterial();
			}
			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_MINUS:
		{
			if(m_nQuantity > 0)
			{
				if(CheckCanMake(m_SelectedRecipeID, m_nQuantity - 1))
				{
					m_nQuantity--;
					UpdateRegisterQuantityDLG(false);
					ResetRecipeMaterial();
				}
			}
			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_UPDATE:
		{
			if(m_nQuantity > 0)
			{
				if(CheckCanMake(m_SelectedRecipeID, m_nQuantity))
				{
					UpdateRegisterQuantityDLG(true);
					ResetRecipeMaterial();
				}
			}
			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_CONFIRM:
		{
			// Ȯ�� �ϸ� ������Ʈ�� �ѹ� �� ���ش�.

			UpdateRegisterQuantityDLG(true);
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

			m_pDLGItemEnroll = NULL;
			m_EnrollItemUid = 0;

			Handler_EGS_ITEM_MANUFACTURE_REQ();

			return true;
		}break;
	case UMCM_REGISTER_QUANTITY_CANCEL:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

			m_pDLGItemEnroll = NULL;
			m_nQuantity = 1;		// ������ �������� ������ Dialog Text 1�� ����
			m_EnrollItemUid = 0;
			//m_bRegisteredTrade = false;

			ResetRecipeMaterial();

			//UnRegisterItem();

			return true;
		}break;
#endif //SERV_MANUFACTURE_FIX
	default:
		break;
	}

	return false;
}

bool CX2UIManufacture:: UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_ITEM_MANUFACTURE_ACK:
		{
			return Handler_EGS_ITEM_MANUFACTURE_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	}
	return false;
}


void CX2UIManufacture::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);
	if(m_pDLGManufacture != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGManufacture, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGManufacture, false );
	}
}

void CX2UIManufacture::SetPosition(D3DXVECTOR2 vec)
{
	// ���԰� ���̾�α� ��ġ�� ������ �ش�
	
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

void CX2UIManufacture::SetShow(bool val)
{
	SAFE_DELETE_DIALOG(m_pDLGManufactureResult);
	m_pDLGManufactureResult = NULL;
    
	if(val)	// ���� �� ó���ؾ� �� �κ�
	{
		RegisterLuaBind();

		if(m_pDLGManufacture == NULL)
		{
			m_pDLGManufacture = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_Alchemy_List.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGManufacture );
		}
		m_pDLGManufacture->SetShowEnable(true, true);
        
		// üũ�ڽ��� ����
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGManufacture->GetControl( L"CheckBox_ShowAll" );
		if ( pCheckBox != NULL )
			pCheckBox->SetCheckedPure( m_bShowAllCharItem );

		//{{ kimhc // 2009-12-10 // ���� ��ư ��� false�� �ʱ�ȭ
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		for ( int i = 0; i < static_cast<int>( MT_END ); i++ )
		{
			CKTDGUIRadioButton* pRadioButton = GetRadioButtonByType( MANUFACTURE_TYPE(i) );

			if ( pRadioButton != NULL )
				pRadioButton->SetShow( false );
		}
#else	ADD_HOUSE_ID_TO_MANUFACTURE
		// ������ư ��������		
		m_NowItemCategory = MT_WEAPON;
		CKTDGUIRadioButton* pRButtionWeapon = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Weapon" );
		pRButtionWeapon->SetChecked(true);
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		// kimhc // 2009-12-10 // ���� ��ư ��� false�� �ʱ�ȭ

		// ������ ���� ���� �ʱ�ȭ
		const int SlotperPage = GetSlotperPage();
		for(UINT i = 0; i<m_vecRecipeSlot.size(); i++)
		{
			ManufactureSlot* pBuySlot = m_vecRecipeSlot[i];
			SAFE_DELETE(pBuySlot);
		}
		m_vecRecipeSlot.clear();

		// ���� ����� ����
		for(int i = 0; i<SlotperPage; i++)
		{
			WCHAR buff[256] = {0,};
			ManufactureSlot* pRecipeSlot = new ManufactureSlot();
			//wsprintf( buff, L"Static_Recipe%d", i);
			StringCchPrintf(buff, 256, L"Static_Recipe%d", i);
			pRecipeSlot->m_pStatic = (CKTDGUIStatic*) m_pDLGManufacture->GetControl( buff );
			//wsprintf( buff, L"Button_Recipe%d", i);
			StringCchPrintf(buff, 256, L"Button_Recipe%d", i);
			pRecipeSlot->m_pButton = (CKTDGUIButton*) m_pDLGManufacture->GetControl( buff );


			m_vecRecipeSlot.push_back(pRecipeSlot);
		}

		// ����Ʈ �޾ƿ��� �����ϰ�.. ����� ó������
		GetManufactureList();

		//{{ kimhc // 2009-12-08 // �Ǹ� ��ǰ�� ���� �� Ȱ��ȭ�ϴ� ��� �߰�
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		// ����
		int MAGIC_CONTROL_WIDTH = 0;
		bool bFlag = false;
		for ( int i = 0; i < static_cast<int>( MT_END ); i++ )
		{
			if ( m_vecManufactureIDList[i].empty() == false )
			{
				CKTDGUIRadioButton* pRadioButton = GetRadioButtonByType( MANUFACTURE_TYPE(i) );

				if ( pRadioButton != NULL )
				{
					if ( false == bFlag )
					{
						m_NowItemCategory = MANUFACTURE_TYPE(i);						 
						pRadioButton->SetChecked( true );
						bFlag = true;
					}
					pRadioButton->SetOffsetPos( D3DXVECTOR2( (float)MAGIC_CONTROL_WIDTH, 0 ) );
					pRadioButton->SetShow( true );
					pRadioButton->SetEnable( true );
					MAGIC_CONTROL_WIDTH += pRadioButton->GetDummyInt(0);		// Width�� LUA�� AddDummyInt�� ����� �־� �� ��!
				}
			}
		}

		ChangeManufactureTab( 1 );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-12-08 // �Ǹ� ��ǰ�� ���� �� Ȱ��ȭ�ϴ� ��� �߰�

		m_bShow = true;
		D3DXVECTOR3 tmp;
		tmp = m_pDLGManufacture->GetDummyPos(0);
		m_DLGPosition.x = tmp.x;
		m_DLGPosition.y = tmp.y;
		tmp = m_pDLGManufacture->GetDummyPos(1);
		m_DLGSize.x = tmp.x;
		m_DLGSize.y = tmp.y;
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_MANUFACTURE);

	}
	else	// ���� �� ó���ؾ� �� �κ�
	{

		for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
		{
			CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
			if ( pkSlotItem != NULL )
			{
				pkSlotItem->DestroyItemUI();
				pkSlotItem->SetShow(false);
				pkSlotItem->SetEnable(false);
			}
		}

		// ���� ���콺���� �̹����� �����ְ�
		m_pDLGSelectedItem->SetShow( false );
		InvalidSlotDesc();

		m_bShow = false;
		m_pDLGManufacture->SetShowEnable(false, false);
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_MANUFACTURE);
	}

	//g_pKTDXApp->SkipFrame();
}

void CX2UIManufacture::GetManufactureList()
{
	m_SelectedRecipeID = -1;
	for ( int i = 0; i < MT_END; i++ )
	{
		m_vecManufactureIDList[i].resize(0);
	}

	// ����Ʈ�� ������ �ް� ������ ���ô�
	vector<int> vecItemList;
	//{{ kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
	g_pData->GetItemManager()->GetManufactureVillageData( m_iNowOpenHouseID, vecItemList );
#else	ADD_HOUSE_ID_TO_MANUFACTURE
	g_pData->GetItemManager()->GetManufactureVillageData( g_pData->GetLocationManager()->GetCurrentVillageID(), vecItemList );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
	
	for( UINT i=0; i<vecItemList.size(); i++ )
	{
		int iManufactureID = vecItemList[i];
		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( iManufactureID );
		
		if ( pManufactureData != NULL )
		{
			// ������ �׽�Ʈ������ �������� ī���� �� ��� ����־� �ݽô�
			// �ϴ� ��밡���� �������� Ȯ��
#ifdef MANUFACTURE_FROM_RESULTGROUP_ID
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pManufactureData->m_ResultGroupID );
#else
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iManufactureID );
#endif MANUFACTURE_FROM_RESULTGROUP_ID
			if ( pItemTemplet != NULL )
			{
				
				if ( m_bShowAllCharItem == false )
				{
					// ���� �� �� �ִ� ������ �ƴϸ� �Ѿ��
					if ( false == IsPossibleUsedByMyCharacter(pItemTemplet) )
					{
						//{{ ����� : [2009/7/29] //	����Ҵ� ����� ��� ȭ�鿡 ǥ�����ش�.(��ϸ��� �ð� ���� ����)
						if( true == pItemTemplet->GetNoEquip() != true )
						{
							continue;
						}						
						//}} ����� : [2009/7/29] //	����Ҵ� ����� ��� ȭ�鿡 ǥ�����ش�.(��ϸ��� �ð� ���� ����)
					}
				}
			}	// pItemTemplet != NULL

			// ī������ ����
#ifdef MANUFACTURE_FROM_RESULTGROUP_ID
			switch ( GetManufactureTypeByItemID( pManufactureData->m_ResultGroupID ) )
#else
			switch ( GetManufactureTypeByItemID( iManufactureID ) )
#endif MANUFACTURE_FROM_RESULTGROUP_ID
			{
			case MT_NONE:
				m_vecManufactureIDList[MT_NONE].push_back(iManufactureID);
				break;
			case MT_WEAPON:
				m_vecManufactureIDList[MT_WEAPON].push_back(iManufactureID);
				break;
			case MT_DEFENCE_BODY:
				m_vecManufactureIDList[MT_DEFENCE_BODY].push_back(iManufactureID);
				break;
			case MT_DEFENCE_LEG:
				m_vecManufactureIDList[MT_DEFENCE_LEG].push_back(iManufactureID);
				break;
			case MT_DEFENCE_HAND:
				m_vecManufactureIDList[MT_DEFENCE_HAND].push_back(iManufactureID);
				break;
			case MT_DEFENCE_FOOT:
				m_vecManufactureIDList[MT_DEFENCE_FOOT].push_back(iManufactureID);
				break;
			case MT_QUICK_SLOT:
				m_vecManufactureIDList[MT_QUICK_SLOT].push_back(iManufactureID);
				break;
			case MT_ETC:
				m_vecManufactureIDList[MT_ETC].push_back(iManufactureID);
				break;
			default:
				break;
			}
		}

	}// for

	//{{ kimhc // 2009-12-08 // �Ǹ� ��ǰ�� ���� �� Ȱ��ȭ�ϴ� ��� �߰�
#ifndef	ADD_HOUSE_ID_TO_MANUFACTURE
	ChangeManufactureTab( 1 );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-12-08 // �Ǹ� ��ǰ�� ���� �� Ȱ��ȭ�ϴ� ��� �߰�
	
}

void CX2UIManufacture::ChangeManufactureTab( int nPage )
{
	// ���� ������ ���� max page ����
	const int MAGIC_MAX_NUM_ITEM_SLOT = GetSlotperPage();
	m_nNowPage = nPage;
	m_nMaxPage = (int)(m_vecManufactureIDList[m_NowItemCategory].size()+MAGIC_MAX_NUM_ITEM_SLOT-1)/MAGIC_MAX_NUM_ITEM_SLOT;
	m_nMaxPage = max(1, m_nMaxPage);
	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*) m_pDLGManufacture->GetControl( L"Static_PageNumber" );
	wstringstream wstrmPageNum;
	wstrmPageNum << m_nNowPage << L"/" << m_nMaxPage;
	pStaticPageNum->GetString(0)->msg = wstrmPageNum.str();

	// ��ü ���� �ε���
	int iStartIndex = (m_nNowPage-1) * MAGIC_MAX_NUM_ITEM_SLOT;
	int iEndIndex = min( m_nNowPage * MAGIC_MAX_NUM_ITEM_SLOT, (int)m_vecManufactureIDList[m_NowItemCategory].size() );

	// ���� ������ ���� �ʱ�ȭ
	for( UINT iS = 0; iS < m_SlotList.size(); iS++ )
	{
		CX2SlotItem* pkSlotItem = (CX2SlotItem*)m_SlotList[iS];
		if ( pkSlotItem != NULL )
		{
			pkSlotItem->DestroyItemUI();
			pkSlotItem->SetShow(false);
			pkSlotItem->SetEnable(false);
		}
	}

	// ����
	for( unsigned int iS = 0; iS < m_vecRecipeSlot.size(); ++iS )
	{
		ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[iS];		
		if ( pRecipeSlot->m_pStatic != NULL )
		{
			pRecipeSlot->m_pStatic->SetShow(false);
			pRecipeSlot->m_pButton->SetShow(false);
		}
	}

	// Slot Index
	int iSlotIndex = 0;
	// ���� ������ ����
	for( int iS = iStartIndex; iS < iEndIndex; iS++ )
	{
		int iManufactureID = m_vecManufactureIDList[m_NowItemCategory][iS];
		
//		CX2SlotItem* pkSlotItem = (CX2SlotItem*) m_SlotList[iSlotIndex];
		ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[iSlotIndex];		
		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( iManufactureID );

		//////////////////////////////////////////////////////////////////////////
		
		if ( pRecipeSlot != NULL && pManufactureData != NULL )
		{
			pRecipeSlot->m_iManufactureID = iManufactureID;
#ifdef MANUFACTURE_FROM_RESULTGROUP_ID
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pManufactureData->m_ResultGroupID );
#else
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iManufactureID );
#endif MANUFACTURE_FROM_RESULTGROUP_ID
			if ( pItemTemplet != NULL )
			{
				wstring itemName = L"";

				bool bCheck = false;
				if ( CheckIsThereAllMaterial( iManufactureID ) == false )
					bCheck = true;

				if ( CheckIsEnoughED( iManufactureID ) == false )
					bCheck = true;

				if ( bCheck == true )
					itemName += L"#CFF0000";

                itemName += pItemTemplet->GetFullName_();

#ifdef CLIENT_GLOBAL_LINEBREAK //�� ������ �°� MAGIC_TEXT_WIDTH ���� 
#ifdef COUNTRY_WORDWRAP_WIDTH_ID 
				const int MAGIC_TEXT_WIDTH = 135;	
				bool bEllipse = false;
				itemName = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse( itemName.c_str(),(int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), pRecipeSlot->m_pStatic->GetString(0)->fontIndex, 1, bEllipse );
#else //COUNTRY_WORDWRAP_WIDTH_ID
#ifdef COUNTRY_WORDWRAP_WIDTH_US
				const int MAGIC_TEXT_WIDTH = 110;
#else //COUNTRY_WORDWRAP_WIDTH_US
				const int MAGIC_TEXT_WIDTH = 160;
#endif //COUNTRY_WORDWRAP_WIDTH_US
				const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pRecipeSlot->m_pStatic->GetString(0)->fontIndex );	
				CWordLineHandler::LineBreakInX2Main( itemName, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#endif //COUNTRY_WORDWRAP_WIDTH_ID
				
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
				const int MAGIC_TEXT_WIDTH = 110;
				const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pRecipeSlot->m_pStatic->GetString(0)->fontIndex );
				CX2Main::LineBreak( itemName, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#endif //CLIENT_GLOBAL_LINEBREAK

				pRecipeSlot->m_pStatic->GetString(0)->msg = itemName;
				pRecipeSlot->m_pStatic->SetShow(true);
				pRecipeSlot->m_pButton->SetShow(true);

#ifdef SERV_MANUFACTURE_PERIOD_FIX
				std::map< int, int >::iterator mit1;
				mit1 = m_mapPeriodGroup.find( iManufactureID );
				if( mit1 != m_mapPeriodGroup.end() )
				{
					wstringstream wstreamDesc;
					//����������� ���� �����϶� '�Ⱓ�� ����' �̶�� �����.
					if( mit1->second != -1)
					{
						wchar_t wszNumber[32];
						::_itow( mit1->second, wszNumber, 10 );
						wstreamDesc << " ( " << wszNumber << GET_STRING( STR_ID_14 ) << " ) ";
						if( pRecipeSlot->m_pStatic->GetString(1)  != NULL)
							pRecipeSlot->m_pStatic->GetString(1)->msg = wstreamDesc.str().c_str();
					}
					else
					{
						wstreamDesc << GET_STRING( STR_ID_24532 );
						if( pRecipeSlot->m_pStatic->GetString(1)  != NULL)
							pRecipeSlot->m_pStatic->GetString(1)->msg = wstreamDesc.str().c_str();
					}

				}
#endif SERV_MANUFACTURE_PERIOD_FIX

				for ( int slotListIter = 0; slotListIter < (int)m_SlotList.size(); slotListIter++ )
				{
					CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[slotListIter];
					if ( pSlotItem != NULL )
					{
						if ( pSlotItem->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW && pSlotItem->GetSlotID() == iSlotIndex )
						{
							pSlotItem->CreateItemUI( pItemTemplet );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable(true);
							AttachSelectedPicture(pSlotItem);
						}
					}
				}
			}
			else
			{
				pRecipeSlot->m_pStatic->GetString(0)->msg = L"";
			}
			
			// ���� ���� ��ַ� ������ (������ ���)
			pRecipeSlot->m_pButton->SetDownStateAtNormal( false );

		}

		// ������ 0���� �����
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGManufacture->GetControl( L"Static_Price" );
		if ( pStatic != NULL && pStatic->GetString(0) != NULL )
		{
			pStatic->GetString(0)->msg = L"0";
		}

		// ��ư�� ���ְ�
		CKTDGUIControl* pControl = m_pDLGManufacture->GetControl( L"Button_Make" );
		if ( pControl != NULL )
			pControl->SetShowEnable( false, false );

		//////////////////////////////////////////////////////////////////////////

		iSlotIndex++;
	}
}



void CX2UIManufacture::SelectRecipe( int iSlotNum )
{

	// ���� ���� ����� �� ���Ƿ� ���� ����
// 	CX2UIManufacture::ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[iSlotNum];
// 	if ( pRecipeSlot != NULL )
// 	{
// 		if ( pRecipeSlot->m_iManufactureID != -1 )
// 			m_SelectedRecipeID = pRecipeSlot->m_iManufactureID;
// 	}
// 
// 	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
// 	{
// 		CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
// 		if ( pSlot != NULL && pSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW )
// 		{
// 			if ( pSlot->GetSlotID() == iSlotNum )
// 			{
// 				//** ���� üũ�� ������
// 				SetShowSelectedPicture(pSlot, true);
// 				
// 			}
// 			else
// 			{
// 				//** üũ ǥ�ø� ��������
// 				SetShowSelectedPicture(pSlot, false);
// 				
// 			}
// 		}
// 	}
	// ��ư ���� ������� ����
	for( int i = 0; i < (int)m_vecRecipeSlot.size(); i++)
	{
		CX2UIManufacture::ManufactureSlot* pRecipeSlot = m_vecRecipeSlot[i];
		
		if( pRecipeSlot->m_pButton != NULL)
		{
			if( i == iSlotNum )
			{
				if ( pRecipeSlot->m_iManufactureID != -1 )
				{
					m_SelectedRecipeID = pRecipeSlot->m_iManufactureID;
					pRecipeSlot->m_pButton->SetDownStateAtNormal(true);
				}

			}
			else
			{
				pRecipeSlot->m_pButton->SetDownStateAtNormal(false);

			}
		}
		

	}

	if ( m_pDLGManufacture != NULL )
	{
		if ( CheckIsThereAllMaterial( m_SelectedRecipeID ) == false || 
			CheckIsEnoughED( m_SelectedRecipeID ) == false )
		{
			// ��ư�� ��������
			CKTDGUIControl* pControl = m_pDLGManufacture->GetControl( L"Button_Make" );
			if ( pControl != NULL )
				pControl->SetShowEnable( false, false );
		}
		else
		{	
			// ��ư�� �� ����
			CKTDGUIControl* pControl = m_pDLGManufacture->GetControl( L"Button_Make" );
			if ( pControl != NULL )
				pControl->SetShowEnable( true, true );
		}
	}

	ResetRecipeMaterial();
}

// ������ �������� ���� ������, �������...
void CX2UIManufacture::ResetRecipeMaterial()
{
	//���õ� �������� ������, �̸�, ���� ���� ���� UI ����
	int recipeFee = 0;
	wstring recipeName = L"";
	wstring recipeDesc = L"";

	//���� ��ȿ�� �����ǰ� ���õȰ��ΰ�?
	if ( m_SelectedRecipeID != -1 )
	{
		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
		if ( pManufactureData != NULL )
		{
			recipeFee = pManufactureData->m_Cost;
		}
	}

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGManufacture->GetControl( L"Static_Price" );
	if ( pStatic != NULL && pStatic->GetString(0) != NULL )
	{
		pStatic->GetString(0)->msg = g_pMain->GetEDString( recipeFee ).c_str();
	}

	// ��Ḧ ������ ����
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem != NULL )
		{
			if ( pSlotItem->GetSlotType() == CX2Slot::ST_MATERIAL )
			{
				pSlotItem->DestroyItemUI();
				pSlotItem->SetShow(false);
				pSlotItem->SetEnable(false);
			}
		}
	}

	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
	if ( pManufactureData != NULL )
	{
		for ( int i = 0; i < (int)pManufactureData->m_vecMaterials.size(); i++ )
		{
			CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[i];
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( materialData.m_MaterialItemID );
			if ( pItemTemplet != NULL )
			{
				int itemQuantity = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );

				for ( int j = 0; j < (int)m_SlotList.size(); j++ )
				{
					CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[j];
					if ( pSlotItem != NULL && pSlotItem->GetSlotType() == CX2Slot::ST_MATERIAL &&
						pSlotItem->GetSlotID() == i )
					{
#ifdef SERV_MANUFACTURE_FIX
						pSlotItem->CreateMaterialItemUI( pItemTemplet, itemQuantity, materialData.m_MaterialCount * m_nQuantity);
#else
						pSlotItem->CreateMaterialItemUI( pItemTemplet, itemQuantity, materialData.m_MaterialCount);
#endif
						pSlotItem->SetShow(true);
						pSlotItem->SetEnable(true);
						break;
					}
				}
			}
		}
	}

}


bool CX2UIManufacture::Handler_EGS_ITEM_MANUFACTURE_REQ()
{
#ifdef SERV_MANUFACTURE_FIX
	if ( CheckIsThereAllMaterial( m_SelectedRecipeID, m_nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_836 ), g_pMain->GetNowState() );
		return false;
	}

	// �����ϰ� ��ᰡ ������(�ƿ� ����) ���� ������ �̸� üũ�Ѵ�. ���⼭�� ������������ üũ.
	// ���� �޽����� �ٸ��� �ϱ� ������ �̷��� ó��.
	if ( CheckIsThereAllMaterialNoEquipped( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2565 ), g_pMain->GetNowState() );
		return false;
	}

	if ( CheckIsEnoughED( m_SelectedRecipeID, m_nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_837 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_ITEM_MANUFACTURE_REQ kPacket;
	kPacket.m_iManufactureID = m_SelectedRecipeID;
	kPacket.m_iQuantity = m_nQuantity;
	m_nQuantity = 1;
#else
	if ( CheckIsThereAllMaterial( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_836 ), g_pMain->GetNowState() );
		return false;
	}

	// �����ϰ� ��ᰡ ������(�ƿ� ����) ���� ������ �̸� üũ�Ѵ�. ���⼭�� ������������ üũ.
	// ���� �޽����� �ٸ��� �ϱ� ������ �̷��� ó��.
	if ( CheckIsThereAllMaterialNoEquipped( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2565 ), g_pMain->GetNowState() );
		return false;
	}

	if ( CheckIsEnoughED( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_837 ), g_pMain->GetNowState() );
		return false;
	}

	KEGS_ITEM_MANUFACTURE_REQ kPacket;
	kPacket.m_iManufactureID = m_SelectedRecipeID;
#endif //SERV_MANUFACTURE_FIX

	g_pData->GetServerProtocol()->SendPacket( EGS_ITEM_MANUFACTURE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ITEM_MANUFACTURE_ACK );

	m_bWaitForManufactureResult = true;

	return true;
}


bool CX2UIManufacture::Handler_EGS_ITEM_MANUFACTURE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ITEM_MANUFACTURE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bWaitForManufactureResult = false;

	if( g_pMain->DeleteServerPacket( EGS_ITEM_MANUFACTURE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			wstring itemName = L"";

			if ( kEvent.m_mapInsertedItem.empty() == false )
			{

				std::map< int, int >::iterator i = kEvent.m_mapInsertedItem.begin();
				int resultItemID = i->first;

				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( resultItemID );
				if ( pItemTemplet != NULL )
				{
                    itemName = pItemTemplet->GetFullName_();

					SAFE_DELETE_DIALOG( m_pDLGManufactureResult );
					m_pDLGManufactureResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_ManufactureUI_Result_Success.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGManufactureResult );

					CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGManufactureResult->GetControl( L"StaticAlchemy_Success_Window_Item_Slot" );
					if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
					{
                        const wchar_t* pwszShopImage = pItemTemplet->GetShopImage();
						if ( pwszShopImage[0] != NULL )
							pStatic->GetPicture(0)->SetTex( pwszShopImage );
						else
							pStatic->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
					}

#ifdef SERV_MANUFACTURE_FIX
					int iTextLenOffset = 0;// ���� ǥ�� ����
					CKTDGUIStatic* pStaticQuantity = (CKTDGUIStatic*)m_pDLGManufactureResult->GetControl( L"Success_Item_Quantity" );
					if( NULL != pStaticQuantity )
					{
						if( i->second >= 2 ) 
						{
							WCHAR wszQuantity[64];
							StringCchPrintfW( wszQuantity, ARRAY_SIZE(wszQuantity) , L"#CFF0000X%d", i->second );
							pStaticQuantity->GetString(0)->msg = wszQuantity;
							iTextLenOffset = -25; // ���� ǥ�� ����
						}
						else
						{
							pStaticQuantity->GetString(0)->msg = L"";
						}
					}
#endif //SERV_MANUFACTURE_FIX

					pStatic = (CKTDGUIStatic*)m_pDLGManufactureResult->GetControl( L"StaticAlchemy_Success_Item_Name" );

#ifdef SERV_MANUFACTURE_FIX
					const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * (250 + iTextLenOffset));
#else
					const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * 250);
#endif //SERV_MANUFACTURE_FIX					
					CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic->GetString(0)->fontIndex );
#ifdef CLIENT_GLOBAL_LINEBREAK
					int lineNum = CWordLineHandler::LineBreakInX2MainMsgBox( itemName, pFont, constTextMaxLen );
#else //CLIENT_GLOBAL_LINEBREAK
					int nowTextLen = 0;
					int lineNum = 1;

					for ( int i = 0; i < (int)itemName.size(); i++ )
					{
						WCHAR tempWchar = itemName[i];
						if ( nowTextLen < constTextMaxLen && tempWchar == '\n' )
						{
							if ( i != (int)itemName.size() - 1 )
							{
								lineNum++;
							}

							nowTextLen = 0;
							continue;
						}
						int iUniCharSize = 0;
						iUniCharSize = pFont->GetWidth( tempWchar );
						nowTextLen += iUniCharSize;

						if ( nowTextLen >= constTextMaxLen )
						{
							if ( i != (int)itemName.size() - 1 )
							{
								lineNum++;
							}

							nowTextLen = 0;
							wstring enterChar = L"\n";
							itemName.insert( itemName.begin() + i, enterChar.begin(), enterChar.end() );
							i++;
						}

						if ( i >= (int)itemName.size() )
						{
							break;
						}
					}
#endif //CLIENT_GLOBAL_LINEBREAK
					if ( lineNum == 2 )
					{
						pStatic->GetString(0)->pos.y -= 9.f;
					}
					
					if ( pStatic != NULL && pStatic->GetString(0) != NULL )
					{
						pStatic->GetString(0)->msg = itemName;
					}

// 					CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGManufactureResult->GetControl( L"ButtonCube_Window_Complete" );
// 					pButton->SetCustomMsgMouseUp(UMCM_RESULT_CLOSE);

				}
			}
			else
			{
				//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_838 ), m_p );

				SAFE_DELETE_DIALOG( m_pDLGManufactureResult );
				m_pDLGManufactureResult = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_ManufactureUI_Result_Fail.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGManufactureResult );
// 				CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGManufactureResult->GetControl( L"ButtonCube_Window_Complete" );
// 				pButton->SetCustomMsgMouseUp(UMCM_RESULT_CLOSE);

			}


			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			
			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{		
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
			}

			//			Reset();

			ResetRecipeMaterial();

			return true;
		}		
	}
	return false;
}



CX2UIManufacture::MANUFACTURE_TYPE CX2UIManufacture::GetManufactureTypeByItemID( int itemID )
{
	CX2UIManufacture::MANUFACTURE_TYPE manuType = CX2UIManufacture::MT_NONE;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
	if ( pItemTemplet != NULL )
	{
		switch( pItemTemplet->GetItemType() ) 
		{
		case CX2Item::IT_WEAPON:
			{
				manuType = CX2UIManufacture::MT_WEAPON;
			}
			break;

		case CX2Item::IT_DEFENCE:
			{
				switch( pItemTemplet->GetEqipPosition() ) 
				{
				case CX2Unit::EP_DEFENCE_BODY:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_BODY;
					}
					break;
				case CX2Unit::EP_DEFENCE_LEG:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_LEG;
					}
					break;
				case CX2Unit::EP_DEFENCE_HAND:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_HAND;
					}
					break;
				case CX2Unit::EP_DEFENCE_FOOT:
					{
						manuType = CX2UIManufacture::MT_DEFENCE_FOOT;
					}
					break;
				default:
					{
						manuType = CX2UIManufacture::MT_ETC;
					}
				}
			}
			break;

		case CX2Item::IT_QICK_SLOT:
			{
				manuType = CX2UIManufacture::MT_QUICK_SLOT;
			}
			break;

		default:
			{
				manuType = CX2UIManufacture::MT_ETC;
			}
		}
	}

	return manuType;
}


bool CX2UIManufacture::IsPossibleUsedByMyCharacter( 
    const CX2Item::ItemTemplet* pItemTemplet 
    )
{
	if ( pItemTemplet == NULL )
		return false;

	return CX2Unit::CanUse( 
        pItemTemplet->GetItemID(), 
        g_pData->GetMyUser()->GetSelectUnit() );
}

int CX2UIManufacture::GetSlotperPage()
{
	int num = 0;
	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2Slot* pSlot = m_SlotList[i];
		if(pSlot->GetSlotType() == CX2Slot::ST_CASH_SHOP_MAIN_NEW)
			num++;
	}

	return num;

}

#ifdef SERV_MANUFACTURE_FIX
bool CX2UIManufacture::CheckIsThereAllMaterial( int manufactureID, int nQuantity )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
		int nowCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		if ( nowCount < materialData.m_MaterialCount * nQuantity )
		{

			return false;
		}
	}

	return true;
}
#else
bool CX2UIManufacture::CheckIsThereAllMaterial( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
		int nowCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		if ( nowCount < materialData.m_MaterialCount )
		{

			return false;
		}
	}

	return true;

}
#endif //SERV_MANUFACTURE_FIX

// �׳� ������ ���� ������ ���� CheckIsThereAllMaterial���� üũ.
// ���⼭�� ������� �������� �����ؼ� ���� üũ�Ѵ�. CheckIsThereAllMaterial�� ������ �� CheckMaterialIsEquipped�� ��������μ�
// ���� ���ڶ� ��츦 �˻��� �� ������̶� �� �Ǵ� ��츦 üũ�� �� �ִ�.
// �ش� �������� ������̶� ���� �������� �κ��丮�� �� ����ִ� ��쿡�� true�� �����Ѵ� : �κ��� �ִ� �������� ������ ���ȴ�.
bool CX2UIManufacture::CheckIsThereAllMaterialNoEquipped( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
//		int TotalCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		int NoEquipCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID, true );
		if ( NoEquipCount < materialData.m_MaterialCount )
		{
			return false;
		}
	}

	return true;


}

#ifdef SERV_MANUFACTURE_FIX
bool CX2UIManufacture::CheckIsEnoughED( int manufactureID, int nQuantity )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	if ( pManufactureData != NULL )
	{
		int recipeFee = pManufactureData->m_Cost * nQuantity;
		if ( recipeFee > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED)
		{
			return false;
		}
	}

	return true;

}
#else
bool CX2UIManufacture::CheckIsEnoughED( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	if ( pManufactureData != NULL )
	{
		int recipeFee = pManufactureData->m_Cost;
		if ( recipeFee > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
		{
			return false;
		}
	}

	return true;

}
#endif //SERV_MANUFACTURE_FIX

wstring CX2UIManufacture::GetSlotItemDesc()
{
	if( NULL == m_pNowOverItemSlot )
		return L"";


	WCHAR buff[256] = L"";
	wstring desc = L"";

	if ( m_pNowOverItemSlot->IsResetItemUI() == false )
	{
		desc = m_pNowOverItemSlot->GetSlotDesc();
	}
	else
	{	
		switch( m_pNowOverItemSlot->GetSlotType() )
		{
		case CX2Slot::ST_MATERIAL:
			{
				if( m_pNowOverItemSlot->GetItemUID() != -1 )
				{
					desc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
				}
				else
				{
					desc = GetSlotItemDescByTID( NULL, m_pNowOverItemSlot->GetItemTID(), false );
				}
			} break;

		default:
		case CX2Slot::ST_CASH_SHOP_MAIN_NEW:
			{
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
				if( NULL != pItemTemplet )
				{
                    desc = pItemTemplet->GetFullName_();
					desc += L"\n\n";

					switch( pItemTemplet->GetUseCondition() )
					{
					default:
					case CX2Item::UC_ANYONE:
						{
							ZeroMemory( buff, sizeof(buff) );
						} break;

					case CX2Item::UC_ONE_UNIT:
						{
							switch( pItemTemplet->GetUnitType() )
							{
							case CX2Unit::UT_ELSWORD:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_614 ) ) );
								} break;

							case CX2Unit::UT_ARME:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_615 ) ) );
								} break;


							case CX2Unit::UT_LIRE:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_616 ) ) );
								} break;


							case CX2Unit::UT_RAVEN:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_617 ) ) );
								} break;


							case CX2Unit::UT_EVE:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_618 ) ) );
								} break;

						//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
					#ifdef	NEW_CHARACTER_CHUNG
							case CX2Unit::UT_CHUNG:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_9739 ) ) );
								} break;
					#endif	NEW_CHARACTER_CHUNG
						//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

					#ifdef ARA_CHARACTER_BASE
							case CX2Unit::UT_ARA:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_21181 ) ) );
								} break;
					#endif
					#ifdef NEW_CHARACTER_EL
							case CX2Unit::UT_ELESIS:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_25873 ) ) );
								} break;
					#endif // NEW_CHARACTER_EL
					#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
							case CX2Unit::UT_ADD:
								{
									StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "I", STR_ID_29422 ) ) );
								} break;
					#endif //SERV_9TH_NEW_CHARACTER

							}
						} break;

					case CX2Item::UC_ONE_CLASS:
						{
							const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetUnitManager()->GetUnitTemplet( pItemTemplet->GetUnitClass() );
							if( NULL != pUnitTemplet )
							{
								StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_839, "S", pUnitTemplet->m_Description.c_str() ) ) );
							}
						} break;
					}

					if( false == CX2Unit::CanUse( m_pNowOverItemSlot->GetItemTID(), g_pData->GetMyUser()->GetSelectUnit() ) )
					{
						desc += L"#CFF0000";
						desc += buff;
						desc += L"#CX";
					}
					else
					{
						desc += buff;
					}

					StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_840, "i", pItemTemplet->GetUseLevel() ) ) );
					desc += buff;


					desc += L"[";
                    desc += GET_STRING( STR_ID_841 );
                    desc += L"]\n\n";
				}

				int iSlotID = m_pNowOverItemSlot->GetSlotID() + GetSlotperPage() * (m_nNowPage-1);
				if( iSlotID >= 0 && iSlotID < (int)m_vecManufactureIDList[m_NowItemCategory].size() )
				{
					CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_vecManufactureIDList[m_NowItemCategory][iSlotID] );
					if( NULL != pManufactureData )
					{
						const CX2ItemManager::ManufactureResultGroupData* pResultGroupData = 
							g_pData->GetItemManager()->GetManufactureResultGroupData( pManufactureData->m_ResultGroupID );

						if ( NULL != pResultGroupData )
						{

							// ���� ��� �׷쿡 �������� 1�� �ۿ� ������ ������ ������ 
							if( pResultGroupData->m_vecResultGroupItemID.size() == 1 )
							{
								if( m_pNowOverItemSlot->GetItemUID() != -1 )
								{
									desc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
								}
								else
								{
									desc = GetSlotItemDescByTID( NULL, m_pNowOverItemSlot->GetItemTID(), false );
								}
							}
							else // ������ �ִٸ� ������ ����Ʈ�� �����ش�
							{
								wstring wstrItems;
								for( UINT i=0; i<pResultGroupData->m_vecResultGroupItemID.size(); i++ )
								{
									const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pResultGroupData->m_vecResultGroupItemID[i] );
									if( NULL != pItemTemplet )
									{

										wstrItems += pItemTemplet->GetNameColor_();
										wstrItems += pItemTemplet->GetName();
										wstrItems += L"#CX\n";
									}
								}

								desc += GET_REPLACED_STRING( ( STR_ID_842, "L", wstrItems ) );
							}
						}
					} // if( NULL != pManufactureData )
				}

			} break;
		}
	}

	return desc;
}

void CX2UIManufacture::AttachSelectedPicture( CX2SlotItem* pItemSlot )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
	if ( pkStatic == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureEquiped = new CKTDGUIControl::CPictureData();
	pPictureEquiped->SetTex( L"HQ_ShopEqipedBorder.tga" );
	pPictureEquiped->SetPoint( new CKTDGUIControl::UIPointData );
	pPictureEquiped->SetPos( D3DXVECTOR2( 0, 0 ) );
	pPictureEquiped->SetSize( pItemSlot->GetSize() );
	pPictureEquiped->SetShow( false );
	pPictureEquiped->SetColor( D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f ));

	pkStatic->AddPictureAt( 2, pPictureEquiped );
}

void CX2UIManufacture::SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow )
{
	CKTDGUIDialogType pkDialog = pItemSlot->GetDialog();
	if ( pkDialog == NULL )
		return;

	CKTDGUIStatic* pkStatic = pkDialog->GetStatic_LUA( "StaticRoot_ItemSlot" );
	if ( pkStatic == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureEquiped = pkStatic->GetPicture(2);
	if ( pPictureEquiped == NULL )
		return;

	CKTDGUIControl::CPictureData* pPictureImpossible = pkStatic->GetPicture(1);
	if ( pPictureImpossible == NULL )
		return;

	pPictureEquiped->SetShow( bShow );
	// ������ ó�� : ���Ұ��� ������ ��ũ�� ó������..
	if(false == bShow)
	{
		if ( g_pData->GetMyUser()->GetSelectUnit()->EqipAbility( pItemSlot->GetItemTID() ) == true )
		{
			pPictureImpossible->SetShow( false );	
		}
		else
		{
			pPictureImpossible->SetShow( true );
		}
	}
	else
	{
		pPictureImpossible->SetShow( false );
	}
}

//{{ kimhc // 2009-11-23 // ���� UI ����
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
CKTDGUIRadioButton* CX2UIManufacture::GetRadioButtonByType( MANUFACTURE_TYPE eType ) const		// ���� Ÿ�Ժ� ��ư ������ ������
{
	if( m_pDLGManufacture == NULL )
		return NULL;

	CKTDGUIRadioButton* pRButton = NULL;
	
	switch( eType )
	{
	case MT_WEAPON:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Weapon" );
		break;
	case MT_DEFENCE_BODY:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Coat" );
		break;
	case MT_DEFENCE_LEG:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Pants" );
		break;
	case MT_DEFENCE_HAND:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Glove" );
		break;
	case MT_DEFENCE_FOOT:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Shoes" );
		break;
	case MT_QUICK_SLOT:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_QuickSlot" );
		break;
	case MT_ETC:
		pRButton = (CKTDGUIRadioButton*) m_pDLGManufacture->GetControl( L"RadioButton_Etc" );
		break;
	default:
		break;
	}

	return pRButton;
}
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
//}} kimhc // 2009-11-23 // ���� UI ����

bool CX2UIManufacture::CheckManufacture()
{	 
	if( m_SelectedRecipeID == 132094 ||
		m_SelectedRecipeID == 132095 ||
		m_SelectedRecipeID == 132096 ||
		m_SelectedRecipeID == 132097 ||
		m_SelectedRecipeID == 132098 ||
		m_SelectedRecipeID == 132099 ||
		m_SelectedRecipeID == 132100 ||
		m_SelectedRecipeID == 132101 ||
		m_SelectedRecipeID == 132102 ||
		m_SelectedRecipeID == 132103 ||
		m_SelectedRecipeID == 132104 ||
		m_SelectedRecipeID == 132105 ||
		m_SelectedRecipeID == 132106 ||
		m_SelectedRecipeID == 132107 )
	{
		return true;
	}

	return false;
}

#ifdef SERV_MANUFACTURE_FIX

bool CX2UIManufacture::CheckIsQuantity( int manufactureID )
{
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}
	const CX2ItemManager::ManufactureResultGroupData* pManuResult = g_pData->GetItemManager()->GetManufactureResultGroupData(pManufactureData->m_ResultGroupID);
	if ( pManuResult == NULL )
	{
		return false;
	}

	// ���� ����� ������. �� ���� ������ ������ �������� ���� ��Ű��.
	if(pManuResult->m_vecResultGroupItemID.size() > 1)
		return false;

	if(g_pData->GetItemManager()->GetItemTemplet(pManuResult->m_vecResultGroupItemID[0])->GetPeriodType() == CX2Item::PT_QUANTITY)
		return true;

	return false;
}

bool CX2UIManufacture::CheckCanMake( int manufactureID, int nQuantity)
{
	if ( CheckIsThereAllMaterial( m_SelectedRecipeID, nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_836 ), g_pMain->GetNowState() );
		return false;
	}

	// �����ϰ� ��ᰡ ������(�ƿ� ����) ���� ������ �̸� üũ�Ѵ�. ���⼭�� ������������ üũ.
	// ���� �޽����� �ٸ��� �ϱ� ������ �̷��� ó��.
	if ( CheckIsThereAllMaterialNoEquipped( m_SelectedRecipeID ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2565 ), g_pMain->GetNowState() );
		return false;
	}

	if ( CheckIsEnoughED( m_SelectedRecipeID, nQuantity ) == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_837 ), g_pMain->GetNowState() );
		return false;
	}

	//}}

	// �κ��� ������� �ִ��� ���⼭�� üũ ����.
	// ���� �κ� ������� �� �� ���� ������ ���� �Ұ��� ����.
	/*
	int iSortType = CX2Inventory::ST_EQUIP;
	while( iSortType != CX2Inventory::ST_END )
	{
		if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().IsPossibleAddItem((CX2Inventory::SORT_TYPE)iSortType) == false )
			return false;
		iSortType++;
	}
	*/

	return true;
}

void CX2UIManufacture::OpenRegisterQuantityDLG()
{

	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);

	m_pDLGItemEnroll = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemEnroll );
	m_pDLGItemEnroll->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(53,62)) );

	CKTDGUIStatic* pStaticPrice			= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_Price" ) );
	CKTDGUIStatic* pStaticEDMark		= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_EDMark" ) );

	// ���� ����
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
	pStaticPrice->GetString(0)->msg = g_pMain->GetEDString( m_nQuantity * pManufactureData->m_Cost );

	CKTDGUIButton*		pButtonOK		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_OK" ) );
	CKTDGUIButton*		pButtonCancel	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Cancle" ) );
	CKTDGUIButton*		pButtonPlus		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Plus" ) );
	CKTDGUIButton*		pButtonMinus	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Minus" ) );
	CKTDGUIIMEEditBox*	pQuantity		= static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );

	pQuantity->SetText( GET_REPLACED_STRING( ( STR_ID_2632, "i", m_nQuantity ) ), true);
	pButtonOK->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_CONFIRM );
	pButtonCancel->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_CANCEL );
	pButtonPlus->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_PLUS );
	pButtonMinus->SetCustomMsgMouseUp( UMCM_REGISTER_QUANTITY_MINUS );
	pQuantity->SetCustomMsgChange( UMCM_REGISTER_QUANTITY_UPDATE );
	pQuantity->SetCustomMsgEnter( UMCM_REGISTER_QUANTITY_CONFIRM );
	m_pDLGItemEnroll->SetCloseCustomUIEventID( UMCM_REGISTER_QUANTITY_CANCEL );
	pQuantity->RequestFocus();

}

void CX2UIManufacture::UpdateRegisterQuantityDLG( bool bReadIME )
{
	ASSERT( NULL != m_pDLGItemEnroll );
	if( NULL == m_pDLGItemEnroll )
		return; 

	CKTDGUIIMEEditBox* pQuantity = static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );
	CKTDGUIStatic* pStaticPrice = static_cast<CKTDGUIStatic*>(m_pDLGItemEnroll->GetControl( L"Static_Price" ));
	

	if(bReadIME)
	{
		m_nQuantity =(int) wcstol( pQuantity->GetText(), NULL, 10 );
	}	

	int nMaxNum = GetMaxMakeNum(m_SelectedRecipeID);

	if ( m_nQuantity <= 0 )
	{
		if(bReadIME)				// �Է�â���� �޾ƿ� ���
			m_nQuantity = 1;
		else						// << �� ���� ��� : 1 �Ʒ��� max�� ��ȯ��Ų��.
			m_nQuantity = nMaxNum;	
	}

	if ( m_nQuantity >= nMaxNum )
		m_nQuantity = nMaxNum;

	WCHAR buff[256] = {0};
	_itow( m_nQuantity, buff, 10 );

	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );

	// ���� ����
	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( m_SelectedRecipeID );
	pStaticPrice->GetString(0)->msg = g_pMain->GetEDString( m_nQuantity * pManufactureData->m_Cost );
}

// ���� : �ִ� ������ ���ϱ�
int CX2UIManufacture::GetMaxMakeNum( int manufactureID)
{
	int nMaxNum = 9999;

	CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( manufactureID );
	if ( pManufactureData == NULL )
	{
		return false;
	}

	for ( int j = 0; j < (int)pManufactureData->m_vecMaterials.size(); j++ )
	{
		CX2ItemManager::MaterialData& materialData = pManufactureData->m_vecMaterials[j];
		int nowCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( materialData.m_MaterialItemID );
		if( nMaxNum > (nowCount / materialData.m_MaterialCount) )
			nMaxNum = (nowCount / materialData.m_MaterialCount);
	}

#ifdef MANUFACTURE_MINUS_CHECK_ELD //nMaxNum(�������� �ִ밹��)�� ������ ED�� ���� �ʵ��� �Ѵ�.
	bool CheckingEld = false;
	CheckingEld = CheckIsEnoughED(manufactureID, nMaxNum); //nMaxNum��ŭ ED�� ������� üũ�Ѵ�.
	while(!CheckingEld)									   //CheckingEld�� false���(ED�� ������� �ʴٸ�) while���� ���� nMaxNum�� 1�� ���δ�.
	{
		nMaxNum--;
		CheckingEld = CheckIsEnoughED(manufactureID, nMaxNum);
	}
#endif MANUFACTURE_MINUS_CHECK_ELD

	return nMaxNum;
}

#endif //SERV_MANUFACTURE_FIX

#endif
