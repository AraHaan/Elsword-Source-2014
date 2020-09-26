#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI
#include ".\x2uipersonalshop.h"

CX2UIPersonalShop::CX2UIPersonalShop( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
m_bShow(false),
m_NowShopState(CX2UIPersonalShop::XPSS_CLOSE),
m_MovedPosition(0,0),				// D3DXVECTOR2
m_DLGPosition(0,0),					// D3DXVECTOR2
m_DLGSize(0,0),						// D3DXVECTOR2
m_pDLGUIPersonalShop(NULL),
m_pDLGHelp(NULL),
m_pDLGItemEnroll(NULL),
m_pDLGCloseShopCheck(NULL),
m_pDLGBuyConfirm(NULL),
m_pDLGBuyQuantity(NULL),
//m_vecSlotInfo;
m_ShopName(L""),
m_ShopKeeperName(L""),
m_PickedShopItemIndex(-1),
m_RegShopItemSellEDPerOne(0),
m_RegShopItemNum(0),
m_RegShopItemUID(0),
m_RegShopItemFee(0),
m_RegShopItemID(0),
//m_vecPersonalShopSlotItem.clear();
m_ItemUIDToBuy(0),
m_ItemNumToBuy(0),
m_iNowPageIndex(1),
m_iMaxPageIndex(1),
m_Layer(XDL_NORMAL_1),
m_bIsMyShopPremium(false)
#ifdef DEF_TRADE_BOARD
, m_bOpenFromBoard(false)
#endif DEF_TRADE_BOARD
//{{ kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�
#ifdef	SEAL_ITEM
, m_pDLGBuySealedItemConfirm( NULL )
#endif	SEAL_ITEM
//}} kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
, m_bIsPShopAgency( false )
#endif
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
, m_uidSelectedItemUID( -1 )
#endif //SERV_UPGRADE_TRADE_SYSTEM
{	

	// �ʱ�ȭ ////////////////////////////////////////////////////////////////////////
	m_vecSlotInfo.clear();
	m_vecPersonalShopSlotItem.clear();
	//////////////////////////////////////////////////////////////////////////
	
	// ���� : Dialog�� ���� ������ �ε����� �ʴ´�
	RegisterLuaBind();

	SetLayer(m_Layer);

	ResetDLGItemEnroll();

	// ��񿡼� ����ڰ� ĳ�� �������� ����� ��� ������ 3���Ӹ� �ƴ϶� 8���� �����ϰԲ� �ؾ� �Ѵ� �� �κп�
	// �� ������ ���� �ƴ� ����ʿ��� ���� �ִ� ���� ������ ǥ�����־���Ѵ�.
	for ( int i = 0; i < PERSONAL_SHOP_ITEM_SLOT_MAX; i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = new CX2UIPersonalShop::SlotInfo();
		m_vecSlotInfo.push_back( pSlotInfo ); 
	}

	CloseShop();
}


CX2UIPersonalShop::~CX2UIPersonalShop(void)
{
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		SAFE_DELETE(pSlotInfo);
	}
	m_vecSlotInfo.clear();
	SAFE_DELETE_DIALOG(m_pDLGUIPersonalShop);
	SAFE_DELETE_DIALOG(m_pDLGHelp);
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
	SAFE_DELETE_DIALOG(m_pDLGCloseShopCheck);
	SAFE_DELETE_DIALOG(m_pDLGBuyConfirm);
	SAFE_DELETE_DIALOG(m_pDLGBuyQuantity);

	//{{ kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�
#ifdef	SEAL_ITEM
	SAFE_DELETE_DIALOG( m_pDLGBuySealedItemConfirm );
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-07 // ���ε� ������ ���Ž� �ߴ� ���̾�α�

}


// State�� ���� �׿� �´� ������� ���� ���� �õ�.
void CX2UIPersonalShop::BreakShop()
{
	switch(m_NowShopState)						
	{
	case XPSS_WAIT:
	case XPSS_SELL:
		{
			if ( m_pDLGCloseShopCheck != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCloseShopCheck, NULL, false );
			m_pDLGCloseShopCheck = NULL;
			Handler_BREAK_PERSONAL_SHOP_REQ();
		}
		break;
	case XPSS_BUY:
		Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
		break;
	case XPSS_CLOSE:
		break;
	}
}

void CX2UIPersonalShop::SetPosition(D3DXVECTOR2 vec)
{
	m_pDLGUIPersonalShop->SetPos(vec);
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


void CX2UIPersonalShop::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	m_Layer = layer;
	if(m_pDLGUIPersonalShop != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIPersonalShop, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIPersonalShop, false );
	}
	if(m_pDLGHelp != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGHelp, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGHelp, false );
	}
	
}
 
bool CX2UIPersonalShop::OnDropAnyItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	if ( pItemSlot == NULL || pItemSlot->GetEnable() == false )
		return false;
	if ( pItemSlot == *m_pSlotBeforeDragging )
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Item_Pick_And_Drop.ogg", false, false );
		SetNowDragItemReturn();
		return true;
	}
	//{{ 2008.11.13 ���¿� : UI ����ó��
	if(!(*m_pSlotBeforeDragging)->IsItem()) return false;
	//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
	//}}


	int slotIndex = pItemSlot->GetSlotID();

	switch((*m_pSlotBeforeDragging)->GetSlotType())
	{
	case CX2Slot::ST_INVENTORY:
		// �κ��丮���� ������ ���
		{
            switch( slotIndex )
			{
			case 0:
			case 1:
			case 2:
				{
                    m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;

					CX2SlotItem* pDraggedItem = (CX2SlotItem*)*m_pSlotBeforeDragging;
					EnrollItemByUid( pDraggedItem->GetItemUID(), m_PickedShopItemIndex );
				}
				break;
			default:
				break;
			}

			SetNowDragItemReturn();
			return true;
		}
		break;

 	case CX2Slot::ST_EQUIPPED:
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_843 ), g_pMain->GetNowState() );
			SetNowDragItemReturn();
			return true;
		}
 		break;
	default:
		break;
	}

	// ó���� �� �� ���
	return false;
 }

bool CX2UIPersonalShop::CheckDoubleShopItem( UidType itemUID, int SlotIdToSetting )
{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// �̻��� UID���, �˻� ���� ��Ű��
	if ( 0 >= itemUID )
		return false;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( i == SlotIdToSetting )
			{
				continue;
			}

			if ( pSlotInfo->m_ItemUID == itemUID )
			{
				return true;
			}
		}
	}

	return false;

}
bool CX2UIPersonalShop::CheckShopItemListToReg()
{
	//�ϴ� ����ҷ��� �������� �Ѱ� �̻��� �� ��� üũ�Ѵ�.
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( pSlotInfo->m_ItemUID != 0 && pSlotInfo->m_Quantity != 0 )
				return true;
		}
	}

	return false;

}


bool CX2UIPersonalShop::Handler_EGS_CREATE_PERSONAL_SHOP_REQ()
{
	if ( g_pMain->GetNowStateID() != CX2Main::XS_SQUARE_GAME || g_pSquareGame == NULL )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_844 ), g_pMain->GetNowState() );
		return true;
	}
	if ( m_NowShopState != CX2UIPersonalShop::XPSS_CLOSE )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_845 ), g_pMain->GetNowState() );
		return true;
	}
	if(true == g_pSquareGame->InExitArea())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_846 ), g_pMain->GetNowState() );
		return true;
	}
	if( g_pSquareGame != NULL && g_pSquareGame->GetMyUnit()->GetNowState() != g_pSquareGame->GetMyUnit()->GetStateID().m_Wait )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_847 ), g_pMain->GetNowState() );
		return true;
	}

	//g_pKTDXApp->GetDIManager()->Getkeyboard()->Clear();
	g_pKTDXApp->GetDIManager()->SetEnable( false );

	g_pData->GetServerProtocol()->SendID( EGS_CREATE_PERSONAL_SHOP_REQ );
	g_pMain->AddServerPacket( EGS_CREATE_PERSONAL_SHOP_ACK );

	return true;
}



bool CX2UIPersonalShop::Handler_EGS_CREATE_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	g_pKTDXApp->GetDIManager()->SetEnable( true );

	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_PERSONAL_SHOP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_CREATE_PERSONAL_SHOP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			CreateShop();		//���� ����

			// ���ȴٰ� ��ȣ
			m_bShow = true;
			D3DXVECTOR3 tmp;
			tmp = m_pDLGUIPersonalShop->GetDummyPos(0);
			m_DLGPosition.x = tmp.x;
			m_DLGPosition.y = tmp.y;
			tmp = m_pDLGUIPersonalShop->GetDummyPos(1);
			m_DLGSize.x = tmp.x;
			m_DLGSize.y = tmp.y;
			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_PERSONAL_SHOP);
			
#ifdef SERV_SOCKET_NEW
			// oasis907 : ����� [2010.4.6] // 
			if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
			}
			// oasis907 : ����� [2010.4.6] // ���� UI�� ���������� �ݴ´�.
			if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->SetShow(false);
			}
#endif SERV_SOCKET_NEW

			return true;
		}
#ifdef SERV_PSHOP_AGENCY
		else if( kEvent.m_iOK == NetError::ERR_PERSONAL_SHOP_37 )
		{			
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12163 ), m_pNowState );
			return true;
		}
#endif
	}

	return false;
}


bool CX2UIPersonalShop::Handler_EGS_JOIN_PERSONAL_SHOP_REQ( UidType unitUID )
{
#ifdef DEF_TRADE_BOARD
	m_bOpenFromBoard = false;
#endif DEF_TRADE_BOARD
	if ( m_NowShopState != CX2UIPersonalShop::XPSS_CLOSE )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_848 ), g_pMain->GetNowState() );
		return true;
	}

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_JOIN_PERSONAL_SHOP_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST

	KEGS_JOIN_PERSONAL_SHOP_REQ kPacket;
	kPacket.m_iUnitUID = unitUID;

#ifdef DEF_TRADE_BOARD //{{ oasis907 : ����� [2010.3.25] // 
	kPacket.m_cStageKey = CX2UIPersonalShop::SK_SQUARE;
#endif DEF_TRADE_BOARD

	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_PERSONAL_SHOP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_PERSONAL_SHOP_ACK );

	return true;
}

#ifdef DEF_TRADE_BOARD
//{{ oasis907 : ����� [2010.3.25] // 
bool CX2UIPersonalShop::Handler_EGS_JOIN_PERSONAL_SHOP_FROM_BOARD_REQ()
{
#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// UI�� ��ġ�� �ʱ� ������, ����
	m_bOpenFromBoard = true;
	if ( m_NowShopState != CX2UIPersonalShop::XPSS_CLOSE )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_848 ), g_pMain->GetNowState() );
		return true;
	}
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_JOIN_PERSONAL_SHOP_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST

	KEGS_JOIN_PERSONAL_SHOP_REQ kPacket;
	// Ȯ��
	kPacket.m_iUnitUID = g_pData->GetUIManager()->GetUIPersonalShopBoard()->GetSelectedPersonalShopUid();
	kPacket.m_cStageKey = CX2UIPersonalShop::SK_BOARD;
	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_PERSONAL_SHOP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_PERSONAL_SHOP_ACK );

	return true;
}
//}} oasis907 : ����� [2010.3.25] // 
#endif DEF_TRADE_BOARD

bool CX2UIPersonalShop::Handler_EGS_JOIN_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_PERSONAL_SHOP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_JOIN_PERSONAL_SHOP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			std::wstring	m_wstrPersonalShopName;
			std::wstring	m_wstrNickName;
			std::vector<KSellPersonalShopItemInfo> m_vecSellItemInfo;

#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
			m_bIsPShopAgency = kEvent.m_bIsPShopAgency;
#endif
			m_ShopName = kEvent.m_wstrPersonalShopName;
			m_ShopKeeperName = kEvent.m_wstrNickName;

			m_vecPersonalShopSlotItem.clear();
			m_vecPersonalShopSlotItem = kEvent.m_vecSellItemInfo;

#ifdef LIMIT_PERSONAL_SHOP_PAGE

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// ���� �븮 ���� ��޿� ���� ������ �� ����
			if ( m_vecPersonalShopSlotItem.size() > static_cast<UINT>( PERSONAL_SHOP_ITEM_SLOT_MAX ) )
			{
				m_vecPersonalShopSlotItem.resize( PERSONAL_SHOP_ITEM_SLOT_MAX );
			}
	#else // SERV_UPGRADE_TRADE_SYSTEM
			if ( m_vecPersonalShopSlotItem.size() > 9 )
			{
				m_vecPersonalShopSlotItem.resize(9);
			}
	#endif // SERV_UPGRADE_TRADE_SYSTEM

			m_iMaxPageIndex =  (int) ( ((int)m_vecPersonalShopSlotItem.size()+2) / 3);
			m_iNowPageIndex = 1;

			ResetSlotList( m_vecPersonalShopSlotItem );
#else
			m_iMaxPageIndex =  (int) ( ((int)kEvent.m_vecSellItemInfo.size()+2) / 3);
			m_iNowPageIndex = 1;
			ResetSlotList( kEvent.m_vecSellItemInfo );
#endif //LIMIT_PERSONAL_SHOP_PAGE

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			m_NowShopState			= CX2UIPersonalShop::XPSS_BUY;		/// ���� ����

			SlotInfo* pSlotInfo		= NULL;		/// ���� ��� ������ ��ü
			m_PickedShopItemIndex	= 0;		/// ���� ��� ������ �ε���

			/// ���� ���� �� ��� ������ ���� ��ȸ
			vector<SlotInfo*>::const_iterator vIt = m_vecSlotInfo.begin();
			for ( ; vIt != m_vecSlotInfo.end(); ++vIt )
			{
				/// ���� ���� ��� ������ UID�� ���� �������� �ִٸ�, �ش� ���� ����
				if ( (*vIt)->m_ItemUID == m_uidSelectedItemUID )
				{
					pSlotInfo = (*vIt);
					break;
				}

				/// ���� ��� ������ �ε��� ����
				++m_PickedShopItemIndex;
			}

			/// ������ ������ UID �ʱ�ȭ
			m_uidSelectedItemUID = -1;

			/// ���� ��� �������� ã�� �� ���ٸ�, �ȳ� �˾� ( �̹� �ȸ� ������ �� �� �ִ�. )
			if ( NULL == pSlotInfo )	
			{
				/// �Ǹ� ����� ��ǰ�Դϴ�.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28250 ), g_pMain->GetNowState() );

				/// ������ ����Ʈ ����
				if ( NULL != g_pData &&
					 NULL != g_pData->GetUIManager() && 
					 NULL != g_pData->GetUIManager()->GetUIPersonalShopBoard() )
					g_pData->GetUIManager()->GetUIPersonalShopBoard()->Handler_EGS_SEARCH_TRADE_BOARD_REQ();

				/// ���� ó��
				Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();

				return true;
			}

			/// ������ ������ UID ����
			m_ItemUIDToBuy = pSlotInfo->m_ItemUID;

			/// ���� ���� �˾� ����----------------------------------------------------------------------------------------------------
			// ���������� �ƴ��� ���� �������̸�
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pSlotInfo->m_ItemID );

			if( CX2Item::PT_QUANTITY == pItemTemplet->GetPeriodType() )
			{
				D3DXVECTOR2 pos;
				pos.x = 512;
				pos.y = 384;
				OpenBuyQuantityDLG( pos );
			}
			else	// �ƴϸ�
			{
				wstringstream	wstrstm;
				wstring			itemFullName = GetPersonalShopSlotItemFullName( m_ItemUIDToBuy );

				// �������� �ƴϸ� 1���� ���
				m_ItemNumToBuy = 1;
	#ifdef DEF_TRADE_BOARD // oasis907 : ����� [2010.3.30] // 

		#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
				if(m_bOpenFromBoard == true && m_bIsPShopAgency == false )
		#else //SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
				if(m_bOpenFromBoard == true)
		#endif //SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
				{
					int iTotalED = pSlotInfo->m_PricePerOne; 
					iTotalED	+= static_cast<int>(pSlotInfo->m_PricePerOne * _CONST_UIPERSONALSHOPBOARD_INFO_::g_dPersonalShopBoardFee);

					wstrstm << GET_REPLACED_STRING( ( STR_ID_4977, "LILL",
							itemFullName,
							KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ),
							g_pMain->GetEDString( iTotalED ),
							g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
				}
				else
	#endif //DEF_TRADE_BOARD
				{
					wstrstm << GET_REPLACED_STRING( ( STR_ID_861, "LILL",
							itemFullName,
							KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ),
							g_pMain->GetEDString( pSlotInfo->m_PricePerOne ),
							g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
				}

				m_pDLGBuyConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
																	 wstrstm.str().c_str(), 
																	 UPSCM_BUY_CONFIRM, 
																	 g_pMain->GetNowState(), 
																	 UPSCM_BUY_CANCLE );
			 }

#else //SERV_UPGRADE_TRADE_SYSTEM

			JoinShop();
			
			// ���ȴٰ� ��ȣ
			m_bShow = true;
			D3DXVECTOR3 tmp;
			tmp = m_pDLGUIPersonalShop->GetDummyPos(0);
			m_DLGPosition.x = tmp.x;
			m_DLGPosition.y = tmp.y;
			tmp = m_pDLGUIPersonalShop->GetDummyPos(1);
			m_DLGSize.x = tmp.x;
			m_DLGSize.y = tmp.y;
			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_PERSONAL_SHOP);

		#ifdef SERV_SOCKET_NEW
			// oasis907 : ����� [2010.4.6] // 
			if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
			}
			// oasis907 : ����� [2010.4.6] // ���� UI�� ���������� �ݴ´�.
			if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->SetShow(false);
			}
		#endif SERV_SOCKET_NEW

#endif //SERV_UPGRADE_TRADE_SYSTEM

			return true;
		}

	}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// ���� ���� ���� ���� ������, ������ ������ UID �ʱ�ȭ
	m_uidSelectedItemUID = -1;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	return false;
}


bool CX2UIPersonalShop::Handler_BREAK_PERSONAL_SHOP_REQ()
{
	KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;

	// �ϴ� ���̾�α׸� �� ���̰� ..

	g_pData->GetServerProtocol()->SendPacket( EGS_BREAK_PERSONAL_SHOP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BREAK_PERSONAL_SHOP_ACK );

	return true;
}

bool CX2UIPersonalShop::Handler_BREAK_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BREAK_PERSONAL_SHOP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_BREAK_PERSONAL_SHOP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CloseShop();

			// ���� ���� �ϴ� �ּ�ó��
// 			if ( g_pPartyMsgerUI != NULL && g_pPartyMsgerUI->GetOpenUserListWindow() == false )
// 				g_pPartyMsgerUI->Handler_CLICKED_PARTY_BUTTON();

			return true;
       	}
	}
	return false;
}

bool CX2UIPersonalShop::Handler_BREAK_PERSONAL_SHOP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BREAK_PERSONAL_SHOP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// ��ǰ ����� �� ���Ž�, ������ �ݵ��� ����
	if ( XPSS_AGENCY_WAIT == m_NowShopState )
	{
		CloseShop();

		/// ���� ������ ������� ������ ǥ�� ������ ���� ������ ����
		if( g_pData->GetUIManager()->GetUIInventory() != NULL )
		{		
			CX2UIInventory* pUIInventory = g_pData->GetUIManager()->GetUIInventory();

			if ( NULL != pUIInventory )
				pUIInventory->UpdateInventorySlot();
		}

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF( kEvent.m_iReason ), g_pMain->GetNowState() );
	}

	m_uidSelectedItemUID	= -1;
	m_NowShopState			= XPSS_WAIT;
#else //SERV_UPGRADE_TRADE_SYSTEM
	CloseShop();

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), NetError::GetErrStrF( kEvent.m_iReason ), g_pMain->GetNowState() );
#endif //SERV_UPGRADE_TRADE_SYSTEM

	return true;
}


bool CX2UIPersonalShop::Handler_EGS_REG_PERSONAL_SHOP_ITEM_REQ()
{
	KEGS_REG_PERSONAL_SHOP_ITEM_REQ kPacket;

	kPacket.m_wstrPersonalShopName = m_ShopName;

	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( pSlotInfo->m_ItemUID != 0 && pSlotInfo->m_Quantity != 0 )
			{
				KRegPersonalShopItemInfo kRegPersonalShopItemInfo;
				kRegPersonalShopItemInfo.m_iItemUID		= pSlotInfo->m_ItemUID;
				kRegPersonalShopItemInfo.m_iPricePerOne = pSlotInfo->m_PricePerOne;
				kRegPersonalShopItemInfo.m_iQuantity	= pSlotInfo->m_Quantity;

				kPacket.m_vecRegItem.push_back( kRegPersonalShopItemInfo );
			}
		}
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_REG_PERSONAL_SHOP_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK );

	return true;
}

bool CX2UIPersonalShop::Handler_EGS_REG_PERSONAL_SHOP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REG_PERSONAL_SHOP_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_ShopName = kEvent.m_wstrPersonalShopName;
			m_ShopKeeperName = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName;
			int shopED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED - kEvent.m_iED; 
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

			/*
			if ( g_pSquareGame != NULL )
			{
			CX2SquareUnit* pMySquareUnit = g_pSquareGame->GetSquareUnitByUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
			if ( pMySquareUnit != NULL )
			{
			pMySquareUnit->SetPersonalShopState( CX2SquareUnit::PSS_SHOP );
			}
			}
			*/

			//����ٰ� std::vector<KSellPersonalShopItemInfo> m_vecSellItemInfo; ��Ͻ�����߰���?

			wstringstream wstrstm;
			wstrstm << GET_REPLACED_STRING( ( STR_ID_849, "i", shopED ) );
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), g_pMain->GetNowState() );

			m_vecPersonalShopSlotItem.clear();
			m_vecPersonalShopSlotItem = kEvent.m_vecSellItemInfo;
			ResetSlotList( kEvent.m_vecSellItemInfo );

#ifdef CHECK_PSHOP
			bool bCheckPShop = false;
			for(int iItem = 0; iItem < (int)kEvent.m_vecSellItemInfo.size(); ++iItem)
			{
				KSellPersonalShopItemInfo &sellPShopItemInfo = kEvent.m_vecSellItemInfo[iItem];
				if( g_pData != NULL && g_pData->GetItemManager() != NULL )
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( sellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );
					if( pItemTemplet != NULL )
					{
						float sellED = (float)pItemTemplet->GetPrice() * 0.2f * 1.f;

						if( sellED < 0.f )
							continue;

						if( sellPShopItemInfo.m_iPricePerOne < sellED )
						{
							bCheckPShop = true;
							break;
						}	
					}						
				}						
			}	
			if( bCheckPShop == true )
			{
				// �ǸŰ� ���� �� �ΰ� �Ȱ�� �ǽ��غ���....
#ifdef ADD_COLLECT_CLIENT_INFO			
				g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_22, "", false, false );
#else
				g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_ID_PSHOP );
#endif
			}			
#endif

			RegiserShop();

			return true;
		}
	}

	return false;
}



bool CX2UIPersonalShop::Handler_EGS_BUY_PERSONAL_SHOP_ITEM_REQ( UidType itemUID, int itemNum )
{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	if ( XPSS_CLOSE == m_NowShopState )
	{
		/// �̹� �Ǹ� ����� ��ǰ �Դϴ�.
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28250 ), g_pMain->GetNowState() );

		return false;
	}
#endif //SERV_UPGRADE_TRADE_SYSTEM

	CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( itemUID );
	if ( pSlotInfo != NULL )
	{
		if ( pSlotInfo->m_ItemUID == 0 || pSlotInfo->m_ItemID == 0 )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_833 ), g_pMain->GetNowState() );
			return false;
		}

		if ( itemNum <= 0 )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_850 ), g_pMain->GetNowState() );
			return false;
		}

		//���˻� ����.
		//if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED < itemNum * pSlotInfoUI->GetNowUiTotalEDNum() )
		if ( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED < itemNum * pSlotInfo->m_PricePerOne )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_851 ), g_pMain->GetNowState() );
			return false;
		}

		KEGS_BUY_PERSONAL_SHOP_ITEM_REQ kPacket;
		kPacket.m_iItemUID = pSlotInfo->m_ItemUID;
		kPacket.m_iItemID = pSlotInfo->m_ItemID;
		kPacket.m_iQuantity = itemNum;

		g_pData->GetServerProtocol()->SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK );

		return true;
	}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ

	if ( NULL != g_pMain && 
		 NULL != g_pMain->GetNowState() &&
		 NULL != g_pData &&
		 NULL != g_pData->GetUIManager() &&
		 NULL != g_pData->GetUIManager()->GetUIPersonalShopBoard() )
	{
		/// ��˻� ( ���� )
		g_pData->GetUIManager()->GetUIPersonalShopBoard()->Handler_EGS_SEARCH_TRADE_BOARD_REQ();

		/// �̹� �Ǹ� ����� ��ǰ �Դϴ�.
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28250 ), g_pMain->GetNowState() );

	}
#endif //SERV_UPGRADE_TRADE_SYSTEM

	return false;
}

bool CX2UIPersonalShop::Handler_EGS_BUY_PERSONAL_SHOP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		else
		{
			/// ���� ���� ������, ���� ���� ����
			if ( XPSS_CLOSE != m_NowShopState )
				Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();

			/// ����
			if ( NULL != g_pData &&
				 NULL != g_pData->GetUIManager() &&
				 NULL != g_pData->GetUIManager()->GetUIPersonalShopBoard() )
				g_pData->GetUIManager()->GetUIPersonalShopBoard()->Handler_EGS_SEARCH_TRADE_BOARD_REQ();
		}
#endif //SERV_UPGRADE_TRADE_SYSTEM
	}

	return false;
}

bool CX2UIPersonalShop::Handler_EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	

#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ		/// ���� Ȯ�� �˾� ������ ����, ����
	if ( m_NowShopState == CX2UIPersonalShop::XPSS_BUY )
#endif //SERV_UPGRADE_TRADE_SYSTEM
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_852 ), g_pMain->GetNowState() );
	}

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

	// �κ��丮 ������Ʈ
	if(g_pData->GetUIManager()->GetUIInventory() != NULL)
	{		
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);
	}
	
#ifdef DEF_TRADE_BOARD // oasis907 : ����� [2010.3.30] // 
	if(g_pData->GetUIManager() != NULL)
	{
		if(g_pData->GetUIManager()->GetUIPersonalShopBoard() != NULL)
		{
			g_pData->GetUIManager()->GetUIPersonalShopBoard()->Handler_EGS_SEARCH_TRADE_BOARD_REQ();
		}
	}
#endif DEF_TRADE_BOARD

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// �ʱ�ȭ
	m_uidSelectedItemUID	= -1;
#endif //SERV_UPGRADE_TRADE_SYSTEM
	return true;
}


// ���� ���� �ִ� ����(�� �����̴� ���� �����̴�)�� ������Ʈ �Ǿ��� �� ���ƿ��� �޽���
bool CX2UIPersonalShop::Handler_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	wstring tempItemFullName = GetPersonalShopSlotItemFullName( kEvent.m_iItemUID );

	for ( int i = 0; i < (int)m_vecPersonalShopSlotItem.size(); i++ )
	{
		KSellPersonalShopItemInfo& kInfo = m_vecPersonalShopSlotItem[i];
		if ( kInfo.m_kInventoryItemInfo.m_iItemUID == kEvent.m_iItemUID )
		{
			kInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity = kEvent.m_iQuantity;
			if ( kInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
			{
				m_vecPersonalShopSlotItem.erase( m_vecPersonalShopSlotItem.begin() + i );
			}
			break;
		}
	}

	//Ư���� ó�� �������
	CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( kEvent.m_iItemUID );
	if ( pSlotInfo != NULL )
	{
		//���� ���� ���¸� üũ�ؼ� �˾�â �������
#ifdef SERV_PSHOP_AGENCY
		if ( m_NowShopState == CX2UIPersonalShop::XPSS_SELL || m_NowShopState == CX2UIPersonalShop::XPSS_AGENCY_SELL )
#else
		if ( m_NowShopState == CX2UIPersonalShop::XPSS_SELL )
#endif
		{
			int sellItemNum = pSlotInfo->m_Quantity - kEvent.m_iQuantity; //�ȸ� ������ ����
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pSlotInfo->m_ItemID );
			if ( pItemTemplet != NULL )
			{
				if ( tempItemFullName.empty() == true )
                {
                    tempItemFullName = pItemTemplet->GetFullName_();
                }

				int sellTotalED = sellItemNum * pSlotInfo->m_PricePerOne;
				
				wstringstream wstrstm;
				wstrstm << GET_REPLACED_STRING( ( STR_ID_853, "LiL", tempItemFullName, sellItemNum, g_pMain->GetEDString( sellTotalED ) ) );

		#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
				/// ĳ�� �븮���� ����� ��
				if ( NULL != g_pInstanceData && SEnum::AST_PREMIUM == g_pInstanceData->GetAgencyShopType() )
		#else // SERV_UPGRADE_TRADE_SYSTEM
				if( m_bIsMyShopPremium )
		#endif // SERV_UPGRADE_TRADE_SYSTEM
				{
					wstrstm << L"(" << GET_STRING( STR_ID_854 ) << L")";
				}
				else
				{
					int iCommission = sellTotalED / 10; // ������ 10%
					wstrstm << L"(" << GET_REPLACED_STRING( ( STR_ID_855, "i", iCommission ) ) << L")";
				}

				
				
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), g_pMain->GetNowState() );
			}
		}

		pSlotInfo->m_Quantity = kEvent.m_iQuantity;

		// ���⼭ ������ ����Ʈ�� �ٲٰ� UI�� ������ �ش�
		ResetSlotList( m_vecPersonalShopSlotItem );

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		/// ����
		if ( NULL == g_pData &&
			 NULL == g_pData->GetUIManager() &&
			 NULL == g_pData->GetUIManager()->GetUIPersonalShopBoard() )
			 g_pData->GetUIManager()->GetUIPersonalShopBoard()->Handler_EGS_SEARCH_TRADE_BOARD_REQ();
		
#else //SERV_UPGRADE_TRADE_SYSTEM
		if ( m_NowShopState == CX2UIPersonalShop::XPSS_BUY )
		{
			ResetBuySlotUIList();
		}
		else if ( m_NowShopState == CX2UIPersonalShop::XPSS_SELL )
		{
			ResetSellSlotUIList();
		}
	#ifdef SERV_PSHOP_AGENCY
		else if( m_NowShopState == CX2UIPersonalShop::XPSS_AGENCY_SELL )
		{
			ResetBuySlotUIList();
		}
	#endif
#endif //SERV_UPGRADE_TRADE_SYSTEM
	}

	return true;
}


bool CX2UIPersonalShop::Handler_EGS_LEAVE_PERSONAL_SHOP_REQ()
{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	if ( XPSS_CLOSE == m_NowShopState )
	{
		/// �̹� �Ǹ� ����� ��ǰ �Դϴ�.
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28250 ), g_pMain->GetNowState() );

		return false;
	}
#endif //SERV_UPGRADE_TRADE_SYSTEM

	g_pData->GetServerProtocol()->SendID( EGS_LEAVE_PERSONAL_SHOP_REQ );
	g_pMain->AddServerPacket( EGS_LEAVE_PERSONAL_SHOP_ACK );

	return true;
}

bool CX2UIPersonalShop::Handler_EGS_LEAVE_PERSONAL_SHOP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_PERSONAL_SHOP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_LEAVE_PERSONAL_SHOP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ

			if ( -1 < m_uidSelectedItemUID )
				Handler_EGS_JOIN_PERSONAL_SHOP_FROM_BOARD_REQ();
			else
			{
				m_NowShopState = XPSS_WAIT;	/// ����
				m_uidSelectedItemUID = -1;
			}
#else //SERV_UPGRADE_TRADE_SYSTEM
			CloseShop();
#endif //SERV_UPGRADE_TRADE_SYSTEM

			return true;
		}
	}

	return false;
}

HRESULT CX2UIPersonalShop::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( (m_pDLGUIPersonalShop != NULL && m_pDLGUIPersonalShop->GetIsMouseOver() == true) )
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


bool CX2UIPersonalShop::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
// 	if ( GetCheckOperationCondition() == false )
// 		return false;

	if ( GetPersonalShopState() == CX2UIPersonalShop::XPSS_CLOSE )
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
	//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDLGUIPersonalShop->GetIsMouseOver())
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
			if ( false == m_pDLGUIPersonalShop->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_PERSONAL_SHOP, true);
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

			MouseUp( mousePos );

		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;
	}
	return bFlag;
}



bool CX2UIPersonalShop::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_CREATE_PERSONAL_SHOP_ACK:
		{
			return Handler_EGS_CREATE_PERSONAL_SHOP_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;

	case EGS_REG_PERSONAL_SHOP_ITEM_ACK:
		{
			return Handler_EGS_REG_PERSONAL_SHOP_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} 
 		break;

	case EGS_JOIN_PERSONAL_SHOP_ACK:
		{
			return Handler_EGS_JOIN_PERSONAL_SHOP_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_BUY_PERSONAL_SHOP_ITEM_ACK:
		{
			return Handler_EGS_BUY_PERSONAL_SHOP_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		} 
		break;

	case EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT:
		{
			return Handler_EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT:
		{
			return Handler_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} 
		break;

 	case EGS_BREAK_PERSONAL_SHOP_ACK:
 		{
			return Handler_BREAK_PERSONAL_SHOP_ACK( hWnd, uMsg, wParam, lParam );
 		} 
 		break;
 
 	case EGS_BREAK_PERSONAL_SHOP_NOT:
 		{
			return Handler_BREAK_PERSONAL_SHOP_NOT( hWnd, uMsg, wParam, lParam );
 		} 
 		break;
 
	case EGS_LEAVE_PERSONAL_SHOP_ACK:
		{
			return Handler_EGS_LEAVE_PERSONAL_SHOP_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

#ifdef SERV_PSHOP_AGENCY
	case EGS_CREATE_PSHOP_AGENCY_ACK:
		{
			return Handler_EGS_CREATE_PSHOP_AGENCY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_LEAVE_PSHOP_AGENCY_ACK:
		{
			return Handler_EGS_LEAVE_PSHOP_AGENCY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_REG_PSHOP_AGENCY_ITEM_ACK:
		{
			return Handler_EGS_REG_PSHOP_AGENCY_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_JOIN_MY_PSHOP_AGENCY_ACK:
		{
			return Handler_EGS_JOIN_MY_PSHOP_AGENCY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_STOP_SALE_PSHOP_AGENCY_ACK:
		{
			return Handler_EGS_STOP_SALE_PSHOP_AGENCY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK:
		{
			return Handler_EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif
	}

	return false;
}


bool CX2UIPersonalShop::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UPSCM_SELL_EXIT:
		{
#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState == XPSS_AGENCY_SELL )
				m_pDLGCloseShopCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_12144 ), UPSCM_SELL_EXIT_CONFIRM, g_pMain->GetNowState() );
			else
				m_pDLGCloseShopCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_856 ), UPSCM_SELL_EXIT_CONFIRM, g_pMain->GetNowState() );
#else
			m_pDLGCloseShopCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_856 ), UPSCM_SELL_EXIT_CONFIRM, g_pMain->GetNowState() );
#endif
			return true;
		}
		break;
	case UPSCM_SELL_EXIT_CONFIRM:
		{
			if ( m_pDLGCloseShopCheck != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCloseShopCheck, NULL, false );

			m_pDLGCloseShopCheck = NULL;

#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState == XPSS_AGENCY_WAIT || m_NowShopState == XPSS_AGENCY_SELL )
			{
				return Handler_EGS_STOP_SALE_PSHOP_AGENCY_REQ();
			}
#endif
			return Handler_BREAK_PERSONAL_SHOP_REQ();
		}
		break;		
	case UPSCM_SELL_HELPTOGGLE:
		{
			if(m_pDLGHelp == NULL )
			{
				m_pDLGHelp = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_Helper.lua");
				m_pDLGHelp->SetLayer(m_Layer);
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGHelp );

#ifdef SERV_PSHOP_AGENCY
				if( m_NowShopState == XPSS_AGENCY_WAIT || m_NowShopState == XPSS_AGENCY_SELL )
				{
					CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"shop");
					if( pStatic != NULL )
						pStatic->SetString( 0, GET_STRING( STR_ID_12164 ) );

					pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"detail1");

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
					if( pStatic != NULL && NULL != g_pInstanceData )
					{	
						/// �븮 ���� Ÿ�Կ� ���� �ȳ� ���� ����
						SEnum::AGENCY_SHOP_TYPE eAgencyShopType = g_pInstanceData->GetAgencyShopType();

						switch( eAgencyShopType )
						{
						case SEnum::AST_PREMIUM:
							{
								pStatic->SetString( 0, GET_STRING( STR_ID_28418 ) );
							} break;
						case SEnum::AST_NORMAL:
							{
								pStatic->SetString( 0, GET_STRING( STR_ID_28417 ) );
							} break;
						default:
							{
								pStatic->SetString( 0, GET_STRING( STR_ID_28416 ) );
							} break;
						}
					}
#else // SERV_UPGRADE_TRADE_SYSTEM
					if( pStatic != NULL )
						pStatic->SetString( 0, GET_STRING( STR_ID_12136 ) );
#endif // SERV_UPGRADE_TRADE_SYSTEM

					pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"detail2");
					if( pStatic != NULL )
						pStatic->SetString( 0, L"" );

					pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"detail3");
					if( pStatic != NULL )
						pStatic->SetString( 0, L"" );
				}
#endif
			}
			else if(false == m_pDLGHelp->GetShow() )
			{
				m_pDLGHelp->SetShow(true);
			}
			else
			{
				m_pDLGHelp->SetShow(false);
			}
			return true;
		}		
		break;
	case UPSCM_SELL_MODIFY:
		{
#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState != CX2UIPersonalShop::XPSS_WAIT && m_NowShopState != CX2UIPersonalShop::XPSS_AGENCY_WAIT )
#else
			if ( m_NowShopState != CX2UIPersonalShop::XPSS_WAIT )
#endif
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_857 ), g_pMain->GetNowState() );
				return true;
			}

			CKTDGUIButton* pModifyButton = (CKTDGUIButton*)lParam;
			if( pModifyButton != NULL )
			{
				int slotIndex = pModifyButton->GetDummyInt(0);
				m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;
				SlotInfo* pSlotInfo = m_vecSlotInfo[m_PickedShopItemIndex];
				if(pSlotInfo->m_ItemUID != NULL)
					OpenDLGItemEnroll( pSlotInfo->m_ItemUID );
			}
			return true;
		}
		break;
	case UPSCM_SELL_PREV_PAGE:
		{
			if( m_iNowPageIndex > 1 )
			{
				m_iNowPageIndex -= 1;
			}

			ResetSellSlotUIList( m_iNowPageIndex );
			return true;

		}
		break;
	case UPSCM_SELL_NEXT_PAGE:
		{
			if( m_iNowPageIndex < m_iMaxPageIndex )
			{
				m_iNowPageIndex += 1;
			}
			ResetSellSlotUIList( m_iNowPageIndex );
			return true;
		}
		break;
	case UPSCM_SELL_BEGIN:
		{
			if( true == CheckEdLimit() )
			{
				if(m_NowShopState == XPSS_WAIT)
					m_pDLGCloseShopCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_858 ),
					UPSCM_SELL_BEGIN_CONFIRM, g_pMain->GetNowState() );
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2566 ), g_pMain->GetNowState() );
			}
			// 
			return true;
		}
		break;
	case UPSCM_SELL_BEGIN_CONFIRM:
		{
			if ( m_pDLGCloseShopCheck != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCloseShopCheck, NULL, false );

			m_pDLGCloseShopCheck = NULL;

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// �Ѱ��� ��� ���߾��
			if ( CheckShopItemListToReg() == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_860 ), g_pMain->GetNowState() );
				return true;
			}

			/// �����Ḧ �� ���� �����
			if ( CheckCanPaymentRegistTex() == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24612 ), g_pMain->GetNowState() );
				return true;
			}
	#else //SERV_UPGRADE_TRADE_SYSTEM
			/// ���� �̸� �Է�â ����
			if ( m_pDLGUIPersonalShop != NULL )
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGUIPersonalShop->GetControl( L"IME_Editname" );
				m_ShopName = pIMEEditBox->GetText();
				if( m_ShopName.length() < 2 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_863 ), g_pMain->GetNowState() );
					return true;
				}
				m_ShopName = m_ShopName.substr( 0, 20 );				
			}

			if ( m_ShopName.empty() == true )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_859 ), g_pMain->GetNowState() );
				return true;
			}

			if ( CheckShopItemListToReg() == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_860 ), g_pMain->GetNowState() );
				return true;
			}
	#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState == CX2UIPersonalShop::XPSS_AGENCY_WAIT )
				return Handler_EGS_REG_PSHOP_AGENCY_ITEM_REQ();
#endif
			return Handler_EGS_REG_PERSONAL_SHOP_ITEM_REQ();
		}
		break;

//////////////////////////////////////////////////////////////////////////
	case UPSCM_BUY_EXIT:
		return Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
	case UPSCM_BUY_BUYBUTTON:
		{
			CKTDGUIControl* pBuyButton = (CKTDGUIControl*)lParam;
			if ( pBuyButton != NULL )
			{

				int slotIndex = pBuyButton->GetDummyInt(0);
				m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;
				SlotInfo* pSlotInfo = m_vecSlotInfo[m_PickedShopItemIndex];
				m_ItemUIDToBuy = pSlotInfo->m_ItemUID;

				// ���������� �ƴ��� ���� �������̸�
                const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pSlotInfo->m_ItemID );
				if( CX2Item::PT_QUANTITY == pItemTemplet->GetPeriodType() )

				{
					D3DXVECTOR2 pos;
					pos.x = pBuyButton->GetDummyPos(0).x;
					pos.y = pBuyButton->GetDummyPos(0).y;
					OpenBuyQuantityDLG( m_pDLGUIPersonalShop->GetPos() + pos );
				}
				else	// �ƴϸ�
				{
					wstringstream wstrstm;
					wstring itemFullName = GetPersonalShopSlotItemFullName( m_ItemUIDToBuy );
					
					// �������� �ƴϸ� 1���� ���
					m_ItemNumToBuy = 1;
#ifdef DEF_TRADE_BOARD // oasis907 : ����� [2010.3.30] // 
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
					if(m_bOpenFromBoard == true && m_bIsPShopAgency == false )
#else
					if(m_bOpenFromBoard == true)
#endif
					{
						int iTotalED = pSlotInfo->m_PricePerOne; 
						iTotalED += static_cast<int>(pSlotInfo->m_PricePerOne * _CONST_UIPERSONALSHOPBOARD_INFO_::g_dPersonalShopBoardFee);
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
						wstrstm << GET_REPLACED_STRING( ( STR_ID_4977, "LSLL",
							itemFullName,
							L"",
							g_pMain->GetEDString( iTotalED ),
							g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
						wstrstm << GET_REPLACED_STRING( ( STR_ID_4977, "LILL",
							itemFullName,
							KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ),
							g_pMain->GetEDString( iTotalED ),
							g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
					}
					else
#endif DEF_TRADE_BOARD
					{
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
						wstrstm << GET_REPLACED_STRING( ( STR_ID_861, "LSLL",
							itemFullName,
							L"",
							g_pMain->GetEDString( pSlotInfo->m_PricePerOne ),
							g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
						wstrstm << GET_REPLACED_STRING( ( STR_ID_861, "LILL",
							itemFullName,
							KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ),
							g_pMain->GetEDString( pSlotInfo->m_PricePerOne ),
							g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
					}
					m_pDLGBuyConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), UPSCM_BUY_CONFIRM, g_pMain->GetNowState() );
					
				}
				
				return true;
			}
		}
		break;
	case UPSCM_BUY_PREV_PAGE:
		{
			if( m_iNowPageIndex > 1 )
			{
				m_iNowPageIndex -= 1;
			}

#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState == XPSS_AGENCY_WAIT )
			{
				ResetSellSlotUIList( m_iNowPageIndex );
			}
			else
			{
				ResetBuySlotUIList( m_iNowPageIndex );
			}
#else			
			ResetBuySlotUIList( m_iNowPageIndex );
#endif
			return true;
		}
		break;
	case UPSCM_BUY_NEXT_PAGE:
		{
			if( m_iNowPageIndex < m_iMaxPageIndex )
			{
				m_iNowPageIndex += 1;
			}

#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState == XPSS_AGENCY_WAIT )
			{
				ResetSellSlotUIList( m_iNowPageIndex );
			}
			else
			{
				ResetBuySlotUIList( m_iNowPageIndex );
			}
#else	
			ResetBuySlotUIList( m_iNowPageIndex );
#endif
			return true;
		}
		break;
	case UPSCM_BUY_CONFIRM:
		{
			if ( m_pDLGBuyConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyConfirm, NULL, false );

			m_pDLGBuyConfirm = NULL;

			//{{ kimhc // 2009-09-07 // ������ �������� ���ε� ���������� Ȯ��
#ifdef	SEAL_ITEM
			if ( CheckIsSealedItem( m_ItemUIDToBuy ) == true )
			{
				if ( m_pDLGBuySealedItemConfirm != NULL )
				{ 
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuySealedItemConfirm, NULL, false );
					m_pDLGBuySealedItemConfirm = NULL;
				}

				m_pDLGBuySealedItemConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
					GET_REPLACED_STRING( ( STR_ID_4475, "L", GetPersonalShopSlotItemFullName( m_ItemUIDToBuy ) ) ),
					UPSCM_BUY_SEALED_ITEM_CONFIRM, g_pMain->GetNowState(), UPSCM_BUY_SEALED_ITEM_CANCEL );

				return true;
			}
#endif	SEAL_ITEM
			//}} kimhc // 2009-09-07 // ������ �������� ���ε� ���������� Ȯ��

			return Handler_EGS_BUY_PERSONAL_SHOP_ITEM_REQ( m_ItemUIDToBuy, m_ItemNumToBuy );
		}
		break;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	case UPSCM_BUY_CANCLE:
		{
			if ( m_pDLGBuyConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyConfirm, NULL, false );

			m_pDLGBuyConfirm = NULL;

			/// ���� ���� ���� ������
			SetSelectedItemID( -1 );

			Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
		} 
		break;
#endif //SERV_UPGRADE_TRADE_SYSTEM

		//{{ kimhc // 2009-09-07 // ���ε� ������ �ŷ� Ȯ��, ���
#ifdef	SEAL_ITEM
	case UPSCM_BUY_SEALED_ITEM_CONFIRM:
		{
			if ( m_pDLGBuySealedItemConfirm != NULL )
			{ 
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuySealedItemConfirm, NULL, false );
				m_pDLGBuySealedItemConfirm = NULL;
			}

			return Handler_EGS_BUY_PERSONAL_SHOP_ITEM_REQ( m_ItemUIDToBuy, m_ItemNumToBuy );
		}
		break;

	case UPSCM_BUY_SEALED_ITEM_CANCEL:
		{
			if ( m_pDLGBuySealedItemConfirm != NULL )
			{ 
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuySealedItemConfirm, NULL, false );
				m_pDLGBuySealedItemConfirm = NULL;
			}

			return true;
		}
		break;
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-07 // ���ε� ������ �ŷ� Ȯ��, ���

//////////////////////////////////////////////////////////////////////////

	case UPSCM_ENROLL_LEFTARROW:
		{
			m_RegShopItemNum--; 

			CKTDGUIIMEEditBox* pDialogNum = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl( L"IME_Enroll_Item_Num" );
			WCHAR buff[256] = {0};
			_itow( m_RegShopItemNum, buff, 10 );
			wstring wstrNum = buff;
			pDialogNum->SetText( wstrNum.c_str() );

			UpdateDLGItemEnroll();
		}

		break;
	case UPSCM_ENROLL_RIGHTARROW:
		{
			m_RegShopItemNum++;
			CKTDGUIIMEEditBox* pDialogNum = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl( L"IME_Enroll_Item_Num" );
			WCHAR buff[256] = {0};
			_itow( m_RegShopItemNum, buff, 10 );
			wstring wstrNum = buff;
			pDialogNum->SetText( wstrNum.c_str() );
			UpdateDLGItemEnroll();
		}
		break;
	case UPSCM_ENROLL_REGISTER:
		RegisterShopItem();
		break;
	case UPSCM_ENROLL_CANCLE:
		CloseDLGItemEnroll();
		break;
	case UPSCM_ENROLL_DLG_CHANGE:
		UpdateDLGItemEnroll();
		break;
		
//////////////////////////////////////////////////////////////////////////
	case UPSCM_BUY_QUANTITY_CONFIRM:
		{
			SlotInfo* pSlotInfo = m_vecSlotInfo[m_PickedShopItemIndex];
			wstringstream wstrstm;
			wstring itemFullName = GetPersonalShopSlotItemFullName( m_ItemUIDToBuy );

#ifdef DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
			if( m_bOpenFromBoard == true && m_bIsPShopAgency == false )
#else
			if(m_bOpenFromBoard == true)
#endif
			{
				int iTotalED = pSlotInfo->m_PricePerOne * m_ItemNumToBuy;
				iTotalED += static_cast<int>(iTotalED * _CONST_UIPERSONALSHOPBOARD_INFO_::g_dPersonalShopBoardFee);
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrstm << GET_REPLACED_STRING( ( STR_ID_4978, "LSiLL",
					itemFullName,
					L"",
					m_ItemNumToBuy,
					g_pMain->GetEDString( static_cast<int>(iTotalED) ),
					g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrstm << GET_REPLACED_STRING( ( STR_ID_4978, "LIiLL",
					itemFullName,
					KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ),
					m_ItemNumToBuy,
					g_pMain->GetEDString( static_cast<int>(iTotalED) ),
					g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			}
			else
#endif DEF_TRADE_BOARD
			{
#ifdef REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrstm << GET_REPLACED_STRING( ( STR_ID_862, "LSiLL",
					itemFullName,
					L"",
					m_ItemNumToBuy,
					g_pMain->GetEDString( pSlotInfo->m_PricePerOne * m_ItemNumToBuy ),
					g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#else //REMOVE_POSTPOSITION_IN_REPLACE_STRING
				wstrstm << GET_REPLACED_STRING( ( STR_ID_862, "LIiLL",
					itemFullName,
					KHanSungPostWordUnicodeWrapper( itemFullName.c_str(), STR_ID_198, STR_ID_199 ),
					m_ItemNumToBuy,
					g_pMain->GetEDString( pSlotInfo->m_PricePerOne * m_ItemNumToBuy ),
					g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED ) ) );
#endif //REMOVE_POSTPOSITION_IN_REPLACE_STRING
			}
            m_pDLGBuyConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), UPSCM_BUY_CONFIRM, g_pMain->GetNowState() );

			if ( m_pDLGBuyQuantity != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyQuantity, NULL, false );
			m_pDLGBuyQuantity = NULL;

		}
		break;
	case UPSCM_BUY_QUANTITY_CANCLE:
		{
			if ( m_pDLGBuyQuantity != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyQuantity, NULL, false );
			m_pDLGBuyQuantity = NULL;

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// ���� ���� ���� ������
			SetSelectedItemID( -1 );

			Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
	#endif //SERV_UPGRADE_TRADE_SYSTEM
		}
		break;
	case UPSCM_BUY_QUANTITY_PLUS:
		{

//			SlotInfo* pSlotInfo = m_vecSlotInfo[m_PickedShopItemIndex];
			m_ItemNumToBuy++;
// 			if(m_ItemNumToBuy > pSlotInfo->m_Quantity) m_ItemNumToBuy = pSlotInfo->m_Quantity;
// 
// 			CKTDGUIIMEEditBox* pDialogNum = (CKTDGUIIMEEditBox*)m_pDLGBuyQuantity->GetControl( L"IME_Edit_Quantity" );
// 			WCHAR buff[256] = {0};
// 			_itow( m_ItemNumToBuy, buff, 10 );
// 			wstring wstrNum = buff;
// 			pDialogNum->SetText( wstrNum.c_str() );

			UpdateBuyQuantityDLG( false );

		}
		break;
	case UPSCM_BUY_QUANTITY_MINUS:
		{

			m_ItemNumToBuy--;
// 			if(m_ItemNumToBuy < 0) m_ItemNumToBuy = 0;
// 			
// 			CKTDGUIIMEEditBox* pDialogNum = (CKTDGUIIMEEditBox*)m_pDLGBuyQuantity->GetControl( L"IME_Edit_Quantity" );
// 			WCHAR buff[256] = {0};
// 			_itow( m_ItemNumToBuy, buff, 10 );
// 			wstring wstrNum = buff;
// 			pDialogNum->SetText( wstrNum.c_str() );

			UpdateBuyQuantityDLG( false );
		}
		break;
	case UPSCM_BUY_QUANTITY_UPDATE:
		UpdateBuyQuantityDLG();
		break;

	case UPSCM_USE_MEGAPHONE1:
		{
			CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
			if( NULL != pMyUnit )
			{
				{
					if( NULL != pMyUnit->GetInventory().GetItemByTID( NASOD_SCOPE_ITEM_ID ) )
					{
						g_pChatBox->SetLastUsedMegaphone(NASOD_SCOPE_ITEM_ID);
						g_pChatBox->ShowNasodMessageDlg( true );
						return true;

					}
#ifdef SERV_VIP_SYSTEM
					else if(NULL != pMyUnit->GetInventory().GetItemByTID( EVENT_NASOD_SCOPE_ITEM_ID ))
					{
						g_pChatBox->SetLastUsedMegaphone(EVENT_NASOD_SCOPE_ITEM_ID);
						g_pChatBox->ShowNasodMessageDlg( true );
						return true;
					}
#endif //SERV_VIP_SYSTEM
					else
					{
						//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"���ҵ� �ް��� �������� �������� �ʽ��ϴ�.", g_pMain->GetNowState() );
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING(STR_ID_9620), g_pMain->GetNowState() );
					}
				}
			}		
		}
		break;
	case UPSCM_USE_MEGAPHONE2:
		{
			CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
			if( NULL != pMyUnit )
			{
				{
					if( NULL != pMyUnit->GetInventory().GetItemByTID( NASOD_SCOPE_HIGH_ITEM_ID ) )
					{
						g_pChatBox->SetLastUsedMegaphone(NASOD_SCOPE_HIGH_ITEM_ID);
						g_pChatBox->ShowNasodMessageDlg( true );
						return true;
					}
#ifdef SERV_VIP_SYSTEM
					else if(NULL != pMyUnit->GetInventory().GetItemByTID( EVENT_NASOD_SCOPE_HIGH_ITEM_ID ))
					{
						g_pChatBox->SetLastUsedMegaphone(EVENT_NASOD_SCOPE_HIGH_ITEM_ID);
						g_pChatBox->ShowNasodMessageDlg( true );
						return true;
					}
#endif //SERV_VIP_SYSTEM
					else
					{
						//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"���� ���ҵ� �ް��� �������� �������� �ʽ��ϴ�.", g_pMain->GetNowState() );
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING(STR_ID_9621), g_pMain->GetNowState() );
					}

				}

			}		
		}
		break;
#ifdef SERV_PSHOP_AGENCY
	case UPSCM_DIALOG_EXIT:
		{
			Handler_EGS_LEAVE_PSHOP_AGENCY_REQ();
		}
		break;
	case UPSCM_AGENCY_SELL_BEGIN:
		{
			if(m_NowShopState == XPSS_AGENCY_WAIT)
				m_pDLGCloseShopCheck = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_12147 ),
				UPSCM_SELL_BEGIN_CONFIRM, g_pMain->GetNowState() );
			return true;
		}
		break;
	case UPSCM_AGENCY_TAKE_ITEM:
		{
			if(m_NowShopState == XPSS_AGENCY_TAKE)
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				if( pButton != NULL )
				{
					int slotIndex = pButton->GetDummyInt(0);
					if( slotIndex >= 0 )
						m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;
					else 
						m_PickedShopItemIndex = -1;
					Handler_EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ( m_PickedShopItemIndex );
					return true;
				}				
			}			
		}
		break;
#endif

	}
	return false;
}

//{{ oasis907 : ����� [2010.3.29] // 
void CX2UIPersonalShop::UpdateMegaphoneButton()
{
	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
	if( NULL != pMyUnit && m_pDLGUIPersonalShop != NULL)
	{
		WCHAR buff[256] = {0,};
		CX2Item* pItem = pMyUnit->GetInventory().GetItemByTID( NASOD_SCOPE_ITEM_ID );
#ifdef SERV_VIP_SYSTEM
		CX2Item* pItem2 = pMyUnit->GetInventory().GetItemByTID( EVENT_NASOD_SCOPE_ITEM_ID );
#endif //SERV_VIP_SYSTEM
		if(pItem != NULL)
		{
			CKTDGUIStatic* pStaticButtonM1 = static_cast <CKTDGUIStatic*> (m_pDLGUIPersonalShop->GetControl( L"Static_Button_M01" ));
			int iItemQuantity = pItem->GetItemData().m_Quantity;
			//wsprintf( buff, L"x%d", iItemQuantity );
			StringCchPrintf(buff, 256, L"x%d", iItemQuantity );

			pStaticButtonM1->SetString(0, buff);
		}
#ifdef SERV_VIP_SYSTEM
		else if(pItem2 != NULL)
		{
			CKTDGUIStatic* pStaticButtonM1 = static_cast <CKTDGUIStatic*> (m_pDLGUIPersonalShop->GetControl( L"Static_Button_M01" ));
			int iItemQuantity = pItem2->GetItemData().m_Quantity;
			StringCchPrintf(buff, 256, L"x%d", iItemQuantity );

			pStaticButtonM1->SetString(0, buff);
		}
#endif //SERV_VIP_SYSTEM
		else
		{
			CKTDGUIStatic* pStaticButtonM1 = static_cast <CKTDGUIStatic*> (m_pDLGUIPersonalShop->GetControl( L"Static_Button_M01" ));
			//wsprintf( buff, L"x0");
			StringCchPrintf( buff, 256, L"x0");
			pStaticButtonM1->SetString(0, buff);
		}


		pItem = pMyUnit->GetInventory().GetItemByTID( NASOD_SCOPE_HIGH_ITEM_ID );
#ifdef SERV_VIP_SYSTEM
		pItem2 = pMyUnit->GetInventory().GetItemByTID( EVENT_NASOD_SCOPE_HIGH_ITEM_ID );
#endif //SERV_VIP_SYSTEM
		if(pItem != NULL)
		{
			CKTDGUIStatic* pStaticButtonM2 = static_cast <CKTDGUIStatic*> (m_pDLGUIPersonalShop->GetControl( L"Static_Button_M02" ) );

			int iItemQuantity = pItem->GetItemData().m_Quantity;
			//wsprintf( buff, L"x%d", iItemQuantity );
			StringCchPrintf( buff, 256, L"x%d", iItemQuantity );
			pStaticButtonM2->SetString(0, buff);
		}
#ifdef SERV_VIP_SYSTEM
		else if(pItem2 != NULL)
		{
			CKTDGUIStatic* pStaticButtonM2 = static_cast <CKTDGUIStatic*> (m_pDLGUIPersonalShop->GetControl( L"Static_Button_M02" ) );

			int iItemQuantity = pItem2->GetItemData().m_Quantity;
			StringCchPrintf( buff, 256, L"x%d", iItemQuantity );
			pStaticButtonM2->SetString(0, buff);
		}
#endif //SERV_VIP_SYSTEM
		else
		{
			CKTDGUIStatic* pStaticButtonM2 = static_cast <CKTDGUIStatic*> (m_pDLGUIPersonalShop->GetControl( L"Static_Button_M02" ));
			//wsprintf( buff, L"x0");
			StringCchPrintf( buff, 256, L"x0");
			pStaticButtonM2->SetString(0, buff);
		}
	}
	return;
}
//}} oasis907 : ����� [2010.3.29] // 


void CX2UIPersonalShop::ResetSlotList()
{
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
			pSlotInfo->Init();
	}
}

void CX2UIPersonalShop::ResetSlotList( vector<KSellPersonalShopItemInfo>& vecSellItemInfo )
{
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
			pSlotInfo->Init();
	}

	for ( int i = 0; i < (int)vecSellItemInfo.size(); i++ )
	{
		KSellPersonalShopItemInfo& kSellPersonalShopItemInfo = vecSellItemInfo[i];
		CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( i );
		if ( pSlotInfo != NULL )
		{
			pSlotInfo->SetData( kSellPersonalShopItemInfo );
		}
	}
}

void CX2UIPersonalShop::ResetSlot( UidType itemUID )
{
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( pSlotInfo->m_ItemUID == itemUID )
			{
				pSlotInfo->Init();
				break;
			}
		}
	}
}

void CX2UIPersonalShop::CreateShop()		//���� ����
{
	m_NowShopState = CX2UIPersonalShop::XPSS_WAIT;
	m_ShopName = L"";
	m_vecPersonalShopSlotItem.resize(0);

	ResetSlotList();
 	SetDialog( m_NowShopState );
	
	if ( g_pSquareGame != NULL )
		g_pSquareGame->SetEnableKeyProcess( false );

	// oasis907 : ����� [2010.3.29] // 
	UpdateMegaphoneButton();
}


void CX2UIPersonalShop::JoinShop()		//�����ڰ� ���� ����
{
	m_NowShopState = CX2UIPersonalShop::XPSS_BUY;
	SetDialog( m_NowShopState );

	if ( g_pSquareGame != NULL )
		g_pSquareGame->SetEnableKeyProcess( false );

#ifdef DEF_TRADE_BOARD
	//{{ oasis907 : ����� [2010.3.30] // 
	if ( g_pTFieldGame != NULL )
	{
		CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
		if ( pJoinNpc != NULL )
		{
			if ( pJoinNpc->GetNpcShop() != NULL )
				pJoinNpc->GetNpcShop()->SetEnablePersonalShopBoardButton( false );
		}
	}
	//}}
#endif DEF_TRADE_BOARD


}

void CX2UIPersonalShop::CloseShop()		//�Ǹ��ڿ��� �Ǹ� ����, �����ڿ��� ���� ����. â�� ���� �� ȣ��
{
	m_iNowPageIndex = 1;
	m_iMaxPageIndex = 1;

	CloseDLGItemEnroll();

	m_NowShopState = CX2UIPersonalShop::XPSS_CLOSE;
	ResetSlotList();
	SetDialog( m_NowShopState );

	if ( g_pSquareGame != NULL )
		g_pSquareGame->SetEnableKeyProcess( true );

	InvalidSelectedItem();
	InvalidSlotDesc();

	// �����ٰ� ǥ���ϰ� UIManager�� ��ȣ
	m_bShow = false;
	g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_PERSONAL_SHOP);

	if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
	}



#ifdef DEF_TRADE_BOARD
	//{{ oasis907 : ����� [2010.3.30] // 
	if ( g_pTFieldGame != NULL )
	{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		/// ��ȭâ�� �ݱ� ��ư�� ���� ��, �ٸ� NPC�� ������ ������! ������ �Խ����� �����ϵ��� ����.
		CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetHouseFieldNPC( static_cast<int>( CX2LocationManager::HI_BILLBOARD ) );
	#else SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
	#endif SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		if ( pJoinNpc != NULL )
		{
			if ( pJoinNpc->GetNpcShop() != NULL )
				pJoinNpc->GetNpcShop()->SetEnablePersonalShopBoardButton( true );
		}
	}
	//}}
#endif DEF_TRADE_BOARD
}


void CX2UIPersonalShop::RegiserShop()		//�Ǹ� ���� 
{
	m_NowShopState = CX2UIPersonalShop::XPSS_SELL;
	SetDialog( m_NowShopState );

	if ( g_pSquareGame != NULL )
		g_pSquareGame->SetEnableKeyProcess( false );

	if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
	}
	
}

void CX2UIPersonalShop::SetDialog( CX2UIPersonalShop::X2_PERSONAL_SHOP_STATE personalShopState )
{
	// ���̾�α� ���⼭ ������ �� ���������
	RegisterLuaBind();
	switch(personalShopState)
	{
	case XPSS_CLOSE:	//���� ����

		// ������ �� ��������
		InvalidSlotDesc();

		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2Slot* pSlot = m_SlotList[i];
			SAFE_DELETE( pSlot );
		}
		m_SlotList.clear();
		// ���̾�α� ����������
		SAFE_DELETE_DIALOG(m_pDLGUIPersonalShop);
		SAFE_DELETE_DIALOG(m_pDLGHelp);
		SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
		SAFE_DELETE_DIALOG(m_pDLGCloseShopCheck);
		SAFE_DELETE_DIALOG(m_pDLGBuyQuantity);

		break;
	case XPSS_WAIT:		//���� �� �Ǹ� �غ��� : CreateShop() �� �� ȣ��
#ifdef SERV_PSHOP_AGENCY
	case XPSS_AGENCY_WAIT:
#endif
		{
			wstring wstrDlgName = L"DLG_UI_Seller.lua";
#ifdef SERV_PSHOP_AGENCY
			if( personalShopState == XPSS_AGENCY_WAIT )
			{
				wstrDlgName = L"DLG_UI_Agency_Seller.lua"; 
			}
#endif
			SAFE_DELETE_DIALOG(m_pDLGUIPersonalShop);

			m_pDLGUIPersonalShop = new CKTDGUIDialog(g_pMain->GetNowState(),wstrDlgName.c_str());
			m_pDLGUIPersonalShop->SetLayer(m_Layer);
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIPersonalShop );

			ResetSlotList();

			// ����â�� ����
			SAFE_DELETE_DIALOG(m_pDLGHelp);
			m_pDLGHelp = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_Helper.lua");
			m_pDLGHelp->SetLayer(m_Layer);
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGHelp );

#ifdef SERV_PSHOP_AGENCY
			if( personalShopState == XPSS_AGENCY_WAIT )
			{
				CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"shop");
				if( pStatic != NULL )
					pStatic->SetString( 0, GET_STRING( STR_ID_12164 ) );

				pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"detail1");
				
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
				if( pStatic != NULL && NULL != g_pInstanceData )
				{	
					/// �븮 ���� Ÿ�Կ� ���� �ȳ� ���� ����
					SEnum::AGENCY_SHOP_TYPE eAgencyShopType = g_pInstanceData->GetAgencyShopType();

					switch( eAgencyShopType )
					{
						case SEnum::AST_PREMIUM:
							{
								pStatic->SetString( 0, GET_STRING( STR_ID_28418 ) );
							} break;
						case SEnum::AST_NORMAL:
							{
								pStatic->SetString( 0, GET_STRING( STR_ID_28417 ) );
							} break;
						default:
							{
								pStatic->SetString( 0, GET_STRING( STR_ID_28416 ) );
							} break;
					}
				}
		#else // SERV_UPGRADE_TRADE_SYSTEM
				if( pStatic != NULL )
					pStatic->SetString( 0, GET_STRING( STR_ID_12136 ) );
		#endif // SERV_UPGRADE_TRADE_SYSTEM

				pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"detail2");
				if( pStatic != NULL )
					pStatic->SetString( 0, L"" );

				pStatic = (CKTDGUIStatic*)m_pDLGHelp->GetControl( L"detail3");
				if( pStatic != NULL )
					pStatic->SetString( 0, L"" );
			}
#endif

			// ������ ����
			m_iNowPageIndex = 1;
			CKTDGUIButton* pButtonLArrow;
			CKTDGUIButton* pButtonRArrow;
			pButtonLArrow = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_ArrowL" );
			pButtonRArrow = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_ArrowR" );

#ifdef SERV_PSHOP_AGENCY
			if( personalShopState == XPSS_AGENCY_WAIT && g_pInstanceData->IsActiveAgencyShop() == true )
			{				
				m_bIsMyShopPremium = true;

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
				/// ���� �븮 ���� Ÿ�Կ� ���� ��� ������ ����
				m_iMaxPageIndex = GetMaxItemPageIndex();
	#else // SERV_UPGRADE_TRADE_SYSTEM
				m_iMaxPageIndex = 3;
	#endif // SERV_UPGRADE_TRADE_SYSTEM
				
				pButtonLArrow->SetShowEnable(true, true);
				pButtonRArrow->SetShowEnable(true, true);
			}
			else
			{
				const int MAGIC_PREMIUM_SHOP_ITEM_ID = 200740;
#ifdef SERV_VIP_SYSTEM
				const int EVENT_MAGIC_PREMIUM_SHOP_ITEM_ID = 84000173;
#endif SERV_VIP_SYSTEM
				if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( MAGIC_PREMIUM_SHOP_ITEM_ID ) > 0 )
				{
					m_bIsMyShopPremium = true;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
					m_iMaxPageIndex = GetMaxItemPageIndex();
#else // SERV_UPGRADE_TRADE_SYSTEM
					m_iMaxPageIndex = 3;
#endif // SERV_UPGRADE_TRADE_SYSTEM

					pButtonLArrow->SetShowEnable(true, true);
					pButtonRArrow->SetShowEnable(true, true);
				}
#ifdef SERV_VIP_SYSTEM
				else if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( EVENT_MAGIC_PREMIUM_SHOP_ITEM_ID ) > 0 )
				{
					m_bIsMyShopPremium = true;
					m_iMaxPageIndex = 3;
					pButtonLArrow->SetShowEnable(true, true);
					pButtonRArrow->SetShowEnable(true, true);
				}
#endif SERV_VIP_SYSTEM
				else 
				{
					m_bIsMyShopPremium = false;
					m_iMaxPageIndex = 2;
					pButtonLArrow->SetShowEnable(false, false);
					pButtonRArrow->SetShowEnable(false, false);
				}
			}
#else
			const int MAGIC_PREMIUM_SHOP_ITEM_ID = 200740;
			if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( MAGIC_PREMIUM_SHOP_ITEM_ID ) > 0 )
			{
				m_bIsMyShopPremium = true;
				m_iMaxPageIndex = 3;
				pButtonLArrow->SetShowEnable(true, true);
				pButtonRArrow->SetShowEnable(true, true);
			}
			else 
			{
				m_bIsMyShopPremium = false;
				m_iMaxPageIndex = 1;
				pButtonLArrow->SetShowEnable(false, false);
				pButtonRArrow->SetShowEnable(false, false);
			}
#endif

			WCHAR buff[256] = {0,};
			//wsprintf( buff, L"%d/%d", m_iNowPageIndex, m_iMaxPageIndex );
			StringCchPrintf( buff, 256, L"%d/%d", m_iNowPageIndex, m_iMaxPageIndex );

			CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Page_Number" );
			pStaticPageNum->GetString(0)->msg = buff;

			// ������ â���� ����
			ResetSellSlotUIList();

#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ			/// ���� �̸� �Է�â ����
			CKTDGUIStatic* pStatic;
			// �̸� �Է����ּ��� ��Ʈ��
			pStatic = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Up" );
			pStatic->GetString(0)->msg = GET_STRING( STR_ID_863 );
			pStatic->SetShow(true);

			pStatic = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Down" );
			pStatic->GetString(0)->msg = L"";

			// �����̸� �Է��ϴ� �κ� ����� (��Ŀ���� ���� ����)
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGUIPersonalShop->GetControl( L"IME_Editname" );
			pIMEEditBox->SetText( GET_REPLACED_STRING( ( STR_ID_864, "L", g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName ) ), true );
			pIMEEditBox->SetShowEnable(true, true);
#endif // SERV_UPGRADE_TRADE_SYSTEM

			// �ǸŽ��� ��ư�� ���̰�
			CKTDGUIButton* pButtonSellBegin = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Sell_Begin" );
			CKTDGUIButton* pButtonSellEnd = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Sell_End" );
			pButtonSellBegin->SetShowEnable(true, true);
			pButtonSellEnd->SetShowEnable(false, false);

			//{{ oasis907 : ����� [2010.3.30] // 
			CKTDGUIStatic* pStaticSellBegin = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Btn_Sell" );
			CKTDGUIStatic* pStaticSellEnd = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Btn_End" );
			pStaticSellBegin->SetShow(true);
			pStaticSellEnd->SetShow(false);
			//}}

		}
		break;
	case XPSS_SELL:		//�Ǹ���		
		{
			if(m_pDLGUIPersonalShop == NULL)
			{
				wstring wstrDlgName = L"DLG_UI_Seller.lua";

				m_pDLGUIPersonalShop = new CKTDGUIDialog(g_pMain->GetNowState(), wstrDlgName.c_str());
				m_pDLGUIPersonalShop->SetLayer(m_Layer);
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIPersonalShop );
			}

			m_pDLGUIPersonalShop->SetShowEnable( true, true );
			
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Up" );
			pStaticName->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_864, "L", m_ShopKeeperName ) );

			pStaticName = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Down" );
			//pStaticName->GetString(0)->msg = g_pMain->GetStrByLienBreak( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );	
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrShopName = CWordLineHandler::GetStrByLineBreakInX2Main(m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex);
#else //CLIENT_GLOBAL_LINEBREAK
			wstring wstrShopName = g_pMain->GetStrByLienBreak( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
			pStaticName->GetString(0)->msg  = g_pMain->GetStringFilter()->FilteringChatString( wstrShopName.c_str(), L'��' );

#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ		/// ���� �̸� �Է�â ����
			// �̸� �Է�â�� �� ���̰�..
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGUIPersonalShop->GetControl( L"IME_Editname" );
			pIMEEditBox->SetShowEnable(false, false);
#endif // SERV_UPGRADE_TRADE_SYSTEM

			// �Ǹ����� ��ư�� ���̰�
			CKTDGUIButton* pButtonSellBegin = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Sell_Begin" );
			CKTDGUIButton* pButtonSellEnd = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Sell_End" );
			pButtonSellBegin->SetShowEnable(false, false);
			pButtonSellEnd->SetShowEnable(true, true);
			
			//{{ oasis907 : ����� [2010.3.30] // 
			CKTDGUIStatic* pStaticSellBegin = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Btn_Sell" );
			CKTDGUIStatic* pStaticSellEnd = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Btn_End" );
			pStaticSellBegin->SetShow(false);
			pStaticSellEnd->SetShow(true);
			//}}

			ResetSellSlotUIList();
		}
		break;
	case XPSS_BUY:		//�����
#ifdef SERV_PSHOP_AGENCY
	case XPSS_AGENCY_SELL:
	case XPSS_AGENCY_TAKE:
#endif
		{
			SAFE_DELETE_DIALOG(m_pDLGUIPersonalShop);

			wstring wstrDlgName = L"DLG_UI_Buyer.lua";
#ifdef SERV_PSHOP_AGENCY
			if( personalShopState == XPSS_AGENCY_SELL )
			{
				wstrDlgName = L"DLG_UI_Agency_Seller.lua"; 
			}
			else if( personalShopState == XPSS_AGENCY_TAKE )
			{
				wstrDlgName = L"DLG_UI_Get_item.lua"; 
			}
#endif

			m_pDLGUIPersonalShop = new CKTDGUIDialog(g_pMain->GetNowState(), wstrDlgName.c_str());
			m_pDLGUIPersonalShop->SetLayer(m_Layer);
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIPersonalShop );
			//ResetSlotList();

			// ������ ����
			m_iNowPageIndex = 1;

			CKTDGUIButton* pButtonLArrow;
			CKTDGUIButton* pButtonRArrow;
			pButtonLArrow = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_ArrowL" );
			pButtonRArrow = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_ArrowR" );
			if( m_iMaxPageIndex <= 1 )
			{
				pButtonLArrow->SetShowEnable(false, false);
				pButtonRArrow->SetShowEnable(false, false);
			}
			else 
			{
				pButtonLArrow->SetShowEnable(true, true);
				pButtonRArrow->SetShowEnable(true, true);
			}

			WCHAR buff[256] = {0,};
			//wsprintf( buff, L"%d/%d", m_iNowPageIndex, m_iMaxPageIndex );
			StringCchPrintf( buff, 256, L"%d/%d", m_iNowPageIndex, m_iMaxPageIndex );

			CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Page_Number" );
			pStaticPageNum->GetString(0)->msg = buff;


#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ			/// ������� �ʴ� �ڵ� ����
			// �ӽ��ڵ�
	#ifdef SERV_PSHOP_AGENCY
			CKTDGUIStatic* pStaticName = NULL;
			if( personalShopState != XPSS_AGENCY_TAKE )
			{
				pStaticName = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Up" );
				pStaticName->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_864, "L", m_ShopKeeperName ) );

				pStaticName = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Down" );
				//pStaticName->GetString(0)->msg = g_pMain->GetStrByLienBreak( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );	
#ifdef CLIENT_GLOBAL_LINEBREAK
				wstring wstrShopName = CWordLineHandler::GetStrByLineBreakInX2Main( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
				wstring wstrShopName = g_pMain->GetStrByLienBreak( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
				pStaticName->GetString(0)->msg  = g_pMain->GetStringFilter()->FilteringChatString( wstrShopName.c_str(), L'��' );
			}
	#else  //SERV_UPGRADE_TRADE_SYSTEM
			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Up" );
			pStaticName->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_864, "L", m_ShopKeeperName ) );

			pStaticName = (CKTDGUIStatic*)m_pDLGUIPersonalShop->GetControl( L"Static_Personal_Shop_Name_Down" );
			//pStaticName->GetString(0)->msg = g_pMain->GetStrByLienBreak( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );	
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrShopName = CWordLineHandler::GetStrByLineBreakInX2Main( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
			wstring wstrShopName = g_pMain->GetStrByLienBreak( m_ShopName.c_str(), 291, pStaticName->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
			pStaticName->GetString(0)->msg  = g_pMain->GetStringFilter()->FilteringChatString( wstrShopName.c_str(), L'��' );
	#endif  //SERV_UPGRADE_TRADE_SYSTEM
#endif //SERV_UPGRADE_TRADE_SYSTEM


#ifdef SERV_PSHOP_AGENCY
			if( personalShopState == XPSS_AGENCY_SELL )
			{
				// �Ǹ����� ��ư�� ���̰�
				CKTDGUIButton* pButtonSellBegin = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Sell_Begin" );
				CKTDGUIButton* pButtonSellEnd = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Sell_End" );
				pButtonSellBegin->SetShowEnable(false, false);
				pButtonSellEnd->SetShowEnable(true, true);

				//{{ oasis907 : ����� [2010.3.30] // 
				CKTDGUIStatic* pStaticSellBegin = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Btn_Sell" );
				CKTDGUIStatic* pStaticSellEnd = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Btn_End" );
				pStaticSellBegin->SetShow(false);
				pStaticSellEnd->SetShow(true);
				//}}

				ResetSellSlotUIList();
			}
#endif

			ResetBuySlotUIList( m_iNowPageIndex );
		}
		break;
	}
}

void CX2UIPersonalShop::ResetSellSlotUIList( int iPageIndex )
{
	
	// ������ �����ϰ�
	if( iPageIndex < 1 )
		iPageIndex = 1;

	if( iPageIndex > m_iMaxPageIndex )
		iPageIndex = m_iMaxPageIndex;

	m_iNowPageIndex = iPageIndex;

	CKTDGUIStatic* pStatic_PageNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Page_Number" );
	if( NULL != pStatic_PageNum )
	{
		WCHAR wszText[32] = L"";
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d/%d", m_iNowPageIndex, m_iMaxPageIndex );
		pStatic_PageNum->GetString(0)->msg = wszText;
	}

	// �ϴ� �۽����� ����
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem != NULL )
		{
			if ( pSlotItem->GetSlotType() == CX2Slot::ST_SHOP_PREVIEW )
			{
				pSlotItem->LostItemUI();
			}
		}
	}

	// �������� �ش��ϴ� ���Ե鿡 ���ؼ�
	for( int i= (m_iNowPageIndex-1)*3; i < m_iNowPageIndex*3; i++ )
	{
		int iSlotIndex = i%3;
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[iSlotIndex];
		CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( i );

		// ��Ʈ�� ���ͼ�
		CKTDGUIStatic* pStaticItemName;
		CKTDGUIStatic* pStaticPricePerOne;
		CKTDGUIStatic* pStaticItemNum;
		CKTDGUIStatic* pStaticTotalED;
		CKTDGUIButton* pButtonModify;
				
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlotInfo->m_ItemUID );

		switch(iSlotIndex)
		{
		default:
		case 0:
			{
				pStaticItemName = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Name1" );
				pStaticPricePerOne = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Price1" );
				pStaticItemNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Num1" );
				pStaticTotalED = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_TotalED1" );
				pButtonModify = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Modify1" );
			}
			break;
		case 1:
			{
				pStaticItemName = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Name2" );
				pStaticPricePerOne = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Price2" );
				pStaticItemNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Num2" );
				pStaticTotalED = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_TotalED2" );
				pButtonModify = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Modify2" );
			}
			break;
		case 2:
			{
				pStaticItemName = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Name3" );
				pStaticPricePerOne = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Price3" );
				pStaticItemNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Num3" );
				pStaticTotalED = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_TotalED3" );
				pButtonModify = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Modify3" );
			}
			break;	
		}
		// ���� ������ְ�
		if ( pItemSlot != NULL )
		{
			if ( pItemSlot->GetSlotType() == CX2Slot::ST_SHOP_PREVIEW && pItemSlot->GetSlotID() == iSlotIndex )
			{
				if ( pItemSlot->IsCreatedItemUI() == true )
					pItemSlot->ResetItemUI( pSlotInfo->m_ItemID, pSlotInfo->m_Quantity );
				else
					pItemSlot->CreateItemUI( pSlotInfo->m_ItemID, pSlotInfo->m_Quantity );

				if(pItem != NULL) 
					pItemSlot->SetItemUID( pSlotInfo->m_ItemUID );

	//{{ kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
#ifdef	SEAL_ITEM
				if ( pItem != NULL && pSlotInfo != NULL )
				{
					if ( pItem->GetItemData().m_bIsSealed == true )
					{
						pItemSlot->SetShowSealedImage( true );
					}
					//else
					//{
					//	pItemSlot->SetShowSealedImage( false );
					//}
				}
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
			}
		}
		if(pItem != NULL)
		{	
			// ���̸� ���ְ�
#ifdef CLIENT_GLOBAL_LINEBREAK
#ifdef UNIQUENESS_EU_ONLY
			bool bEllipse = false;
			wstring tpString = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(pItem->GetFullName().c_str(), 220, XUF_DODUM_15_BOLD, 1, bEllipse);
#else //UNIQUENESS_EU_ONLY
			wstring tpString = CWordLineHandler::GetStrByLineBreakInX2Main(pItem->GetFullName().c_str(), 220, XUF_DODUM_15_BOLD );
#endif 	//UNIQUENESS_EU_ONLY
			pStaticItemName->GetString(0)->msg = tpString;
#else //CLIENT_GLOBAL_LINEBREAK
			pStaticItemName->GetString(0)->msg = pItem->GetFullName();
#endif //CLIENT_GLOBAL_LINEBREAK
			// ���� ���ְ�
			pStaticPricePerOne->GetString(0)->msg = g_pMain->GetEDString(pSlotInfo->m_PricePerOne);

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			pStaticPricePerOne->SetColor( g_pMain->GetPriceColor( pSlotInfo->m_PricePerOne ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

			// ���� ���ְ�
#ifdef CLIENT_COUNTRY_EU
			pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "i", pSlotInfo->m_Quantity ) );
#else
			WCHAR buff[256] = {0};
			_itow( pSlotInfo->m_Quantity, buff, 10 );
			wstring wstrNum = buff;
			wstrNum += GET_STRING( STR_ID_24 );
			pStaticItemNum->GetString(0)->msg = wstrNum.c_str();
#endif CLIENT_COUNTRY_EU
			// ��ED ���ְ�
			pStaticTotalED->GetString(0)->msg = g_pMain->GetEDString( pSlotInfo->m_PricePerOne * pSlotInfo->m_Quantity );

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			pStaticTotalED->SetColor( g_pMain->GetPriceColor( pSlotInfo->m_PricePerOne * pSlotInfo->m_Quantity ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

			// ��ư ���ְ�
			pButtonModify->SetShow(true);
		}
		else
		{
			// �� �� ���������
			pStaticItemName->GetString(0)->msg = L"";
			pStaticPricePerOne->GetString(0)->msg = L"0";
#ifdef CLIENT_COUNTRY_EU
			// 2010/11/11 ��ȿ�� ��� ��ġ ��Ű�� ���� ���� 	
			pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "i", 0 ) );
#else
			pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "S", L"0" ) );
#endif CLIENT_COUNTRY_EU
			pStaticTotalED->GetString(0)->msg = L"0";
			pButtonModify->SetShow(false);
		}
	}

}


void CX2UIPersonalShop::ResetBuySlotUIList( int iPageIndex /*= 1*/ )
{
	if( iPageIndex < 1 )
		iPageIndex = 1;

	if( iPageIndex > m_iMaxPageIndex )
		iPageIndex = m_iMaxPageIndex;

	m_iNowPageIndex = iPageIndex;


	CKTDGUIStatic* pStatic_PageNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Page_Number" );
	if( NULL != pStatic_PageNum && NULL != pStatic_PageNum->GetString(0) )
	{
		WCHAR wszText[32] = L"";
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d/%d", m_iNowPageIndex, m_iMaxPageIndex );
		pStatic_PageNum->GetString(0)->msg = wszText;
	}

	// �ϴ� ������ �����ϰ�
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
		if ( pSlotItem != NULL )
		{
			if ( pSlotItem->GetSlotType() == CX2Slot::ST_SHOP_PREVIEW )
			{
				pSlotItem->LostItemUI();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	// �������� �ش��ϴ� ���Կ� ���ؼ�
	for( int i= (m_iNowPageIndex-1)*3; i < m_iNowPageIndex*3; i++ )
	{
		int iSlotIndex = i%3;
		
		if ( iSlotIndex < 0 || iSlotIndex >= static_cast<int>( m_SlotList.size() ) )
			continue;

		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[iSlotIndex];
#ifdef LIMIT_PERSONAL_SHOP_PAGE

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		int iSlotInfo = i % ( m_iMaxPageIndex * 3 );
	#else // SERV_UPGRADE_TRADE_SYSTEM
		int iSlotInfo = i%9;
	#endif // SERV_UPGRADE_TRADE_SYSTEM
		
		CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( iSlotInfo );
#else
		CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( i );
#endif //LIMIT_PERSONAL_SHOP_PAGE

		// ��Ʈ�� ���ͼ�
		CKTDGUIStatic* pStaticItemName;
		CKTDGUIStatic* pStaticPricePerOne;
		CKTDGUIStatic* pStaticItemNum;
		CKTDGUIStatic* pStaticTotalED;
		CKTDGUIButton* pButtonBuy;

        const CX2Item::ItemTemplet* pItemTemplet = NULL;
#ifdef LIMIT_PERSONAL_SHOP_PAGE
		if( pSlotInfo != NULL)
#endif //LIMIT_PERSONAL_SHOP_PAGE
		{
			pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pSlotInfo->m_ItemID );
		}
				
		switch(iSlotIndex)
		{
		default:
		case 0:
			{
				pStaticItemName = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Name1" );
				pStaticPricePerOne = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Price1" );
				pStaticItemNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Num1" );
				pStaticTotalED = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_TotalED1" );
#ifdef SERV_PSHOP_AGENCY
				if( m_NowShopState == XPSS_AGENCY_WAIT || m_NowShopState == XPSS_AGENCY_SELL )
					pButtonBuy = NULL;
				else if( m_NowShopState == XPSS_AGENCY_TAKE )
					pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Get_itemS1" );
				else
					pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Buy1" );
#else
				pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Buy1" );
#endif
				
			}
			break;
		case 1:
			{
				pStaticItemName = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Name2" );
				pStaticPricePerOne = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Price2" );
				pStaticItemNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Num2" );
				pStaticTotalED = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_TotalED2" );
#ifdef SERV_PSHOP_AGENCY
				if( m_NowShopState == XPSS_AGENCY_WAIT || m_NowShopState == XPSS_AGENCY_SELL )
					pButtonBuy = NULL;
				else if( m_NowShopState == XPSS_AGENCY_TAKE )
					pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Get_itemS2" );
				else
					pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Buy2" );
#else
				pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Buy2" );
#endif
			}
			break;
		case 2:
			{
				pStaticItemName = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Name3" );
				pStaticPricePerOne = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Price3" );
				pStaticItemNum = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_Num3" );
				pStaticTotalED = (CKTDGUIStatic*) m_pDLGUIPersonalShop->GetControl( L"Static_Item_TotalED3" );
#ifdef SERV_PSHOP_AGENCY
				if( m_NowShopState == XPSS_AGENCY_WAIT || m_NowShopState == XPSS_AGENCY_SELL )
					pButtonBuy = NULL;
				else if( m_NowShopState == XPSS_AGENCY_TAKE )
					pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Get_itemS3" );
				else
					pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Buy3" );
#else
				pButtonBuy = (CKTDGUIButton*) m_pDLGUIPersonalShop->GetControl( L"Button_Buy3" );
#endif
			}
			break;	
		}
		// ���� ������ְ�
		if ( pItemSlot != NULL )
		{
			if ( pItemSlot->GetSlotType() == CX2Slot::ST_SHOP_PREVIEW && pItemSlot->GetSlotID() == iSlotIndex )
			{
				if ( pItemSlot->IsCreatedItemUI() == true )
					pItemSlot->ResetItemUI( pSlotInfo->m_ItemID, pSlotInfo->m_Quantity );
				else
					pItemSlot->CreateItemUI( pSlotInfo->m_ItemID, pSlotInfo->m_Quantity );

				if(pItemTemplet != NULL) 
					pItemSlot->SetItemUID( pSlotInfo->m_ItemUID );

	//{{ kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
#ifdef	SEAL_ITEM
				if ( pSlotInfo->m_ItemUID > 0 &&
					 m_vecPersonalShopSlotItem[i].m_kInventoryItemInfo.m_kItemInfo.IsSealedItem() == true )
				{	
					pItemSlot->SetShowSealedImage( true );
				}

#endif	SEAL_ITEM
	//}} kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
			}
		}
		if(pItemTemplet != NULL)
		{	
			// ���̸� ���ְ�
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring tpString = CWordLineHandler::GetStrByLineBreakInX2Main(GetPersonalShopSlotItemFullName( pSlotInfo->m_ItemUID ).c_str(), 220, XUF_DODUM_15_BOLD );
			if ( NULL != pStaticItemName && NULL != pStaticItemName->GetString(0) )
				pStaticItemName->GetString(0)->msg = tpString;
#else //CLIENT_GLOBAL_LINEBREAK
			if ( NULL != pStaticItemName && NULL != pStaticItemName->GetString(0) )
				pStaticItemName->GetString(0)->msg = GetPersonalShopSlotItemFullName( pSlotInfo->m_ItemUID );
#endif //CLIENT_GLOBAL_LINEBREAK			
			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			if ( NULL != pStaticPricePerOne )
				pStaticPricePerOne->SetColor( g_pMain->GetPriceColor( pSlotInfo->m_PricePerOne ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

			// ���� ���ְ�
			if ( NULL != pStaticPricePerOne && NULL != pStaticPricePerOne->GetString(0) )
				pStaticPricePerOne->GetString(0)->msg = g_pMain->GetEDString(pSlotInfo->m_PricePerOne);
			

#ifdef SERV_PSHOP_AGENCY
			if( m_NowShopState == XPSS_AGENCY_TAKE )
			{
				// ���� ���ְ�
				if ( NULL != pStaticItemNum && NULL != pStaticItemNum->GetString(0) )
					pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "i", pSlotInfo->m_iTotalSoldItemQuantity ) );
				// ��ED ���ְ�
				if ( NULL != pStaticTotalED && NULL != pStaticTotalED->GetString(0) )
					pStaticTotalED->GetString(0)->msg = g_pMain->GetEDString( pSlotInfo->m_iTotalSellEDIn );
			}
			else
			{
				// ���� ���ְ�
				if ( NULL != pStaticItemNum && NULL != pStaticItemNum->GetString(0) )
					pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "i", pSlotInfo->m_Quantity ) );
				// ��ED ���ְ�
				if ( NULL != pStaticTotalED && NULL != pStaticTotalED->GetString(0) )
					pStaticTotalED->GetString(0)->msg = g_pMain->GetEDString( pSlotInfo->m_PricePerOne * pSlotInfo->m_Quantity );
			}
#else
			// ���� ���ְ�
			pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "i", pSlotInfo->m_Quantity ) );
			// ��ED ���ְ�
			pStaticTotalED->GetString(0)->msg = g_pMain->GetEDString( pSlotInfo->m_PricePerOne * pSlotInfo->m_Quantity );
#endif

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			if ( NULL != pStaticTotalED )
				pStaticTotalED->SetColor( g_pMain->GetPriceColor( pSlotInfo->m_PricePerOne * pSlotInfo->m_Quantity ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����


			// ��ư ���ְ�
#ifdef SERV_PSHOP_AGENCY
			if( pButtonBuy != NULL )
#endif
				pButtonBuy->SetShow(true);
		}
		else
		{
			// �� �� ���������
#ifdef GET_STRING_NULL_CHECK //GetString���� �޾ƿ� �� �����ϱ� ���� NULL üũ
			if( NULL != pStaticItemName &&  NULL != pStaticItemName->GetString(0) )
				pStaticItemName->GetString(0)->msg = L"";
			if( NULL != pStaticPricePerOne &&  NULL != pStaticPricePerOne->GetString(0) )
				pStaticPricePerOne->GetString(0)->msg = L"0";
			if( NULL != pStaticItemNum && NULL != pStaticItemNum->GetString(0) )
				pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "S", L"0" ) );
			if( NULL != pStaticTotalED &&  NULL != pStaticTotalED->GetString(0) )
				pStaticTotalED->GetString(0)->msg = L"0";
#else
			pStaticItemName->GetString(0)->msg = L"";
			pStaticPricePerOne->GetString(0)->msg = L"0";
            pStaticItemNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_865, "S", L"0" ) );
			pStaticTotalED->GetString(0)->msg = L"0";
#endif //GET_STRING_NULL_CHECK
#ifdef SERV_PSHOP_AGENCY
			if( pButtonBuy != NULL )
#endif
				pButtonBuy->SetShow(false);
		}

	}
}


void CX2UIPersonalShop::OpenDLGItemEnroll( UidType itemUID )
{
	CloseDLGItemEnroll();
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);

	// �� ������ ���� ����
	m_pDLGItemEnroll = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Item_Enroll.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemEnroll );

	//����� ���� ���� ���� �������ְ� �Žñ� �ؾ߰ڱ�..
	//���� �����ϰ� �̰� ���� �� ���� ������� �Ѵ���~

// 	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
// 	{
// 		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
// 		if ( pSlotItem != NULL )
// 		{
// 			if ( pSlotItem->GetSlotType() == CX2SlotItem::ST_SHOP_PREVIEW && pSlotItem->GetSlotID() == m_DecideSellNumDLGSlotID ) 
// 			{
// 				// �ش� �������� ����
// 				// ���� ���̾� �ٲ��ְ�
// 				if ( pSlotItem->GetDialog() != NULL )
// 					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( pSlotItem->GetDialog(), XDL_POP_UP );
// 
// 				if ( pSlotItem->GetDLGSlotFixBack() != NULL )
// 					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( pSlotItem->GetDLGSlotFixBack(), XDL_POP_UP );
// 				// ���� ���� ����
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUID );
				if ( pItem != NULL )
				{
//					pSlotItem->CreateItemUI( pItem );

					// ������ �̸� ������Ʈ
					CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGItemEnroll->GetControl( L"Static_Enroll_Item_Name" );
					if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
					{
#ifdef CLIENT_GLOBAL_LINEBREAK
						wstring tpString = CWordLineHandler::GetStrByLineBreakInX2Main(pItem->GetFullName().c_str(), 220, pStaticItemName->GetString(0)->fontIndex );
						pStaticItemName->GetString(0)->msg = tpString;
#else //CLIENT_GLOBAL_LINEBREAK
						pStaticItemName->GetString(0)->msg = pItem->GetFullName();
#endif //CLIENT_GLOBAL_LINEBREAK
					}

					// ������ ������Ʈ
					/*
					CKTDGUIStatic* pStaticCharge = (CKTDGUIStatic*)m_pDLGDecideItemED->GetControl( L"Shop_Trade_Set_Commission_Num" );
					if ( pStaticCharge != NULL && pStaticCharge->GetString(0) != NULL )
					{
					if ( pItem->GetItemTemplet()->GetItemGrade() == CX2Item::IG_ELITE || 
					pItem->GetItemTemplet()->GetItemGrade() == CX2Item::IG_UNIQUE )
					{
					m_RegShopItemFee = pItem->GetEDToSell(); 
					}
					else
					{
					m_RegShopItemFee = 0;
					}

					pStaticCharge->GetString(0)->msg = g_pMain->GetEDString( m_RegShopItemFee );
					}
					*/

					// �׸� �־�����
					CKTDGUIStatic* pStaticSlot = (CKTDGUIStatic*)m_pDLGItemEnroll->GetControl( L"Static_Slot" );
					if(pStaticSlot != NULL && pStaticSlot->GetPicture(0) != NULL)
					{
						pStaticSlot->GetPicture(0)->SetTex( 
                            pItem->GetItemTemplet()->GetShopImage()
                            );
					}
	//{{ kimhc // 2009-09-02 // ������ ��� â�� ���� �̹��� ���̵���
#ifdef	SEAL_ITEM
					if ( pItem->GetItemData().m_bIsSealed == true )
					{
						if( pStaticSlot != NULL && pStaticSlot->GetPicture( 1 ) != NULL )
						{
							pStaticSlot->GetPicture( 1 )->SetShow( true );
						}
					}
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-02 // ������ ��� â�� ���� �̹��� ���̵���
					m_RegShopItemUID = pItem->GetUID();
                    m_RegShopItemID = pItem->GetItemTemplet()->GetItemID();

//					break;
				}
// 			}
// 		}
//	}

// 	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
// 	{
// 		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
// 		if ( pSlotItem != NULL )
// 		{
// 			if ( pSlotItem->GetSlotType() != CX2SlotItem::ST_SHOP_PREVIEW && pSlotItem->GetSlotID() != m_DecideSellNumDLGSlotID ) 
// 			{
//				pSlotItem->SetEnable( false );
//			}
// 		}
// 	}

	// �Է�â �ʱ�ȭ
	if(m_pDLGItemEnroll != NULL)
	{
		CKTDGUIIMEEditBox* pDialogNum = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl( L"IME_Enroll_Item_Num" );
		if(pDialogNum != NULL) pDialogNum->SetText( L"1" );
	}
	CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl(L"IME_Enroll_Item_Price");       
	pDialogEd->RequestFocus();

	UpdateDLGItemEnroll();
    
}


void CX2UIPersonalShop::EnrollItemByUid( UidType itemUid, int SlotIndex )
{
	bool bFlag = false;
	
	if(SlotIndex == -1)
	{
		// �����̾� �����̸� ���� 9�� �ƴϸ� 3��..
		const int MAGIC_PREMIUM_SHOP_ITEM_ID = 200740;
#ifdef SERV_VIP_SYSTEM
		const int EVENT_MAGIC_PREMIUM_SHOP_ITEM_ID = 84000173;
#endif SERV_VIP_SYSTEM
		int MaxSlot = 3;
#ifdef SERV_PSHOP_AGENCY
		if( m_NowShopState == XPSS_AGENCY_WAIT && g_pInstanceData->IsActiveAgencyShop() == true )
		{				
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// ���� �븮 ���� Ÿ�Կ� ���� ��ϼ� ����
			const SEnum::AGENCY_SHOP_TYPE eAgencyShopType = g_pInstanceData->GetAgencyShopType();

			switch( eAgencyShopType )
			{
			case SEnum::AST_PREMIUM:	MaxSlot = CASH_PERSONAL_SHOP_ITEM_SLOT_MAX;	break;		/// ĳ�� ���� ������
			case SEnum::AST_NORMAL:		MaxSlot = ED_PERSONAL_SHOP_ITEM_SLOT_MAX;	break;		/// ED ���� ������
			default:					MaxSlot = FREE_PERSONAL_SHOP_ITEM_SLOT_MAX;	break;		/// ���� ������
			}
	#else //SERV_UPGRADE_TRADE_SYSTEM
			MaxSlot = PERSONAL_SHOP_ITEM_SLOT_MAX;
	#endif //SERV_UPGRADE_TRADE_SYSTEM
		}
		else if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( MAGIC_PREMIUM_SHOP_ITEM_ID ) > 0 )
		{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// ���� �븮 ���� Ÿ�Կ� ���� ��ϼ� ����
			const SEnum::AGENCY_SHOP_TYPE eAgencyShopType = g_pInstanceData->GetAgencyShopType();

			switch( eAgencyShopType )
			{
			case SEnum::AST_PREMIUM:	MaxSlot = CASH_PERSONAL_SHOP_ITEM_SLOT_MAX;	break;		/// ĳ�� ���� ������
			case SEnum::AST_NORMAL:		MaxSlot = ED_PERSONAL_SHOP_ITEM_SLOT_MAX;	break;		/// ED ���� ������
			default:					MaxSlot = FREE_PERSONAL_SHOP_ITEM_SLOT_MAX;	break;		/// ���� ������
			}
	#else //SERV_UPGRADE_TRADE_SYSTEM
			MaxSlot = PERSONAL_SHOP_ITEM_SLOT_MAX;
	#endif //SERV_UPGRADE_TRADE_SYSTEM
		}
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		/*else if( m_NowShopState == XPSS_AGENCY_WAIT && g_pInstanceData->IsActiveAgencyShop() == true )
		{				
			MaxSlot = ED_PERSONAL_SHOP_ITEM_SLOT_MAX;
		}
		else
		{				
			MaxSlot = FREE_PERSONAL_SHOP_ITEM_SLOT_MAX;
		}*/
	#endif //SERV_UPGRADE_TRADE_SYSTEM
#ifdef SERV_VIP_SYSTEM
		else if(g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( EVENT_MAGIC_PREMIUM_SHOP_ITEM_ID ) > 0 )
		{
			MaxSlot = PERSONAL_SHOP_ITEM_SLOT_MAX;			
		}
#endif SERV_VIP_SYSTEM
#else
		if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( MAGIC_PREMIUM_SHOP_ITEM_ID ) > 0 )
		{
			MaxSlot = PERSONAL_SHOP_ITEM_SLOT_MAX;			
		}
#endif
		
		// �켱 �� ������ �˻�����
#ifdef CONVERSION_VS
		int iPickedShopItemIndex = 0;
		for( iPickedShopItemIndex = 0; iPickedShopItemIndex < MaxSlot; iPickedShopItemIndex++ )
#else CONVERSION_VS
		for ( int i = 0; i < MaxSlot; i++ )
#endif CONVERSION_VS
		{
#ifdef CONVERSION_VS
			CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[iPickedShopItemIndex];
#else CONVERSION_VS
			CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
#endif CONVERSION_VS
			if ( pSlotInfo != NULL )
			{
				if(pSlotInfo->m_ItemUID == 0)
				{
					bFlag = true;
					break;
				}
			}
		}
		if(false == bFlag)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_866 ), g_pMain->GetNowState() );
			return;
		}
#ifdef CONVERSION_VS
		m_PickedShopItemIndex = iPickedShopItemIndex;
#else CONVERSION_VS
		m_PickedShopItemIndex = i;
#endif CONVERSION_VS
	}
	else
	{
		m_PickedShopItemIndex = SlotIndex;

	}

	// �������� �Ű� ����
	m_iNowPageIndex = m_PickedShopItemIndex/3 + 1;
	int slotIndex = m_PickedShopItemIndex%3;

	m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;

#ifdef SERV_PSHOP_AGENCY
	if( m_NowShopState != XPSS_WAIT && m_NowShopState != XPSS_AGENCY_WAIT )
		return;
#else
	if(m_NowShopState != XPSS_WAIT)
		return;
#endif
	if ( CheckDoubleShopItem( itemUid, m_PickedShopItemIndex ) == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999,-999 ), GET_STRING( STR_ID_396 ), g_pMain->GetNowState() );
	}
	else
	{
		bool bCheck = true;
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( itemUid );
		if( NULL == pItem || pItem->GetItemTemplet() == NULL )
			return;
#ifdef ITEM_RECOVERY_TEST
		// ��ȭ ���� �˻� (���Ҵ��ΰ�?)
		if( true == pItem->IsDisabled())
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_394 ), g_pMain->GetNowState() );
			bCheck = false;
		}
#endif //ITEM_RECOVERY_TEST

		if ( pItem->GetItemTemplet()->GetPeriodType() == CX2Item::PT_ENDURANCE )
		{
			if ( pItem->GetItemData().m_Endurance < pItem->GetItemTemplet()->GetEndurance() )
			{
				bCheck = false;
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_867 ),g_pMain->GetNowState() );
			}
		}

		//{{ kimhc // 2009-09-02 // ���ε� ������ ���λ����� ��� ���� �ϵ���
#ifdef	SEAL_ITEM
		if ( bCheck == true && pItem->GetItemTemplet()->GetVested() == true 
			&& pItem->GetItemData().m_bIsSealed == false )
#else	SEAL_ITEM
		if ( bCheck == true && pItem->GetItemTemplet()->GetVested() == true )
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-02 // ���ε� ������ ���λ����� ��� ���� �ϵ���
		{
			bCheck = false;
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_868 ),g_pMain->GetNowState() );
		}

		if ( bCheck == true )
			OpenDLGItemEnroll( pItem->GetUID() );
	}
}


void CX2UIPersonalShop::CloseDLGItemEnroll()
{
	if(m_pDLGItemEnroll != NULL) m_pDLGItemEnroll->SetShow(false);

	// ���� ��������
// 	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
// 	{
// 		CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
// 		if ( pSlotItem != NULL )
// 		{
// 			if ( pSlotItem->GetSlotType() == CX2SlotItem::ST_SHOP_PREVIEW && pSlotItem->GetSlotID() == m_DecideSellNumDLGSlotID ) 
// 			{
// 				SAFE_DELETE( pSlotItem );
// 				m_SlotList.erase( m_SlotList.begin() + i );
// 				i--;
// 				break;
// 			}
// 		}
// 	}

	//SetEnable( true );

	ResetDLGItemEnroll();
}

void CX2UIPersonalShop::ResetDLGItemEnroll()
{
	m_RegShopItemSellEDPerOne	= 0;
	m_RegShopItemNum			= 1;
	m_RegShopItemUID			= 0;
	m_RegShopItemFee			= 0;
		
	m_RegShopItemID				= 0;
}


void CX2UIPersonalShop::UpdateDLGItemEnroll()
{
	if ( m_pDLGItemEnroll != NULL )
	{
		// ���� ���� ������Ʈ
		CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl(L"IME_Enroll_Item_Price");       
		m_RegShopItemSellEDPerOne = g_pMain->GetEDFromString( pDialogEd->GetText() );
		if( m_RegShopItemSellEDPerOne < 0 )
		{
			m_RegShopItemSellEDPerOne = 0;
		}
		else if( m_RegShopItemSellEDPerOne  > MAX_ED_FOR_TRADE )
		{
			m_RegShopItemSellEDPerOne = MAX_ED_FOR_TRADE;     
		}
		
        wstring wstrED = g_pMain->GetEDString( m_RegShopItemSellEDPerOne );
		pDialogEd->SetText( wstrED.c_str() );

		//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
		pDialogEd->SetTextColor( g_pMain->GetPriceColor( m_RegShopItemSellEDPerOne ) );
#endif	PRICE_COLOR
		//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

		

		// ���� üũ �� ������Ʈ
		CKTDGUIIMEEditBox* pDialogNum = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl( L"IME_Enroll_Item_Num" );
		m_RegShopItemNum = g_pMain->GetEDFromString( pDialogNum->GetText() );

		if ( m_RegShopItemNum <= 0 ) 
			m_RegShopItemNum = 0;

		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_RegShopItemUID );

		if ( NULL != pItem )
		{
			if ( m_RegShopItemNum >= pItem->GetItemData().m_Quantity )
				m_RegShopItemNum = pItem->GetItemData().m_Quantity;

			if(pItem->GetItemTemplet()->GetPeriodType() == CX2Item::PT_QUANTITY)
			{
				if ( m_RegShopItemNum >= pItem->GetItemTemplet()->GetQuantity() )
					m_RegShopItemNum = pItem->GetItemTemplet()->GetQuantity();
			}
		}
		
		// Overflow Check
		if (m_RegShopItemSellEDPerOne != 0 && m_RegShopItemNum > MAX_ED_FOR_PLAYER/m_RegShopItemSellEDPerOne)
		{
			m_RegShopItemNum = MAX_ED_FOR_PLAYER/m_RegShopItemSellEDPerOne;
		}
        
		WCHAR buff[256] = {0};
		_itow( m_RegShopItemNum, buff, 10 );
		wstring wstrNum = buff;
		pDialogNum->SetText( wstrNum.c_str() );


		// �� ���� ������Ʈ
		CKTDGUIStatic* pStaticTotalED = (CKTDGUIStatic*)m_pDLGItemEnroll->GetControl( L"Static_Enroll_Item_Total" );
		if ( pStaticTotalED != NULL && pStaticTotalED->GetString(0) != NULL )
		{
			pStaticTotalED->GetString(0)->msg = g_pMain->GetEDString(m_RegShopItemNum * m_RegShopItemSellEDPerOne);

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			pStaticTotalED->SetColor( g_pMain->GetPriceColor( m_RegShopItemNum * m_RegShopItemSellEDPerOne ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
		}
	}
}

void CX2UIPersonalShop::RegisterShopItem()
{
	// ��� ��ư�� ������ ��
	// ���� ������ ���� ������ �������
	CKTDGUIIMEEditBox* pIMEEditBox;
	pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl( L"IME_Enroll_Item_Price" );
	m_RegShopItemSellEDPerOne = g_pMain->GetEDFromString( pIMEEditBox->GetText() );
	pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGItemEnroll->GetControl( L"IME_Enroll_Item_Num" );
	m_RegShopItemNum = g_pMain->GetEDFromString( pIMEEditBox->GetText() );

	if ( m_RegShopItemSellEDPerOne <= 0 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_869 ), g_pMain->GetNowState() );
		return;
	}

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_RegShopItemUID );
	if (m_RegShopItemNum <= 0 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_870 ), g_pMain->GetNowState() );
		return;
	}
	else if( m_RegShopItemNum > pItem->GetItemData().m_Quantity )
	{
		// Ȥ�� �𸣴� ó���س���
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_871 ), g_pMain->GetNowState() );
		return;
	}

	CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( m_PickedShopItemIndex /* * ( NowPageNum - 1 * MAX_SLOT_NUM_BY_PAGE ) */ );
	if ( pSlotInfo != NULL )
	{
		pSlotInfo->m_ItemID = m_RegShopItemID;
		pSlotInfo->m_ItemUID = m_RegShopItemUID;
		pSlotInfo->m_Quantity = m_RegShopItemNum;
		pSlotInfo->m_PricePerOne = m_RegShopItemSellEDPerOne;
		pSlotInfo->m_ItemFee = m_RegShopItemFee;

		ResetSellSlotUIList(m_iNowPageIndex);
	}

	int iPickedSlotIndex = m_PickedShopItemIndex % 3;
	CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( iPickedSlotIndex );
	if ( pSlotItem != NULL )
	{
		pSlotItem->DestroyItemUI();
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlotInfo->m_ItemUID );
		if ( pItem != NULL && pSlotInfo != NULL )
		{
			pSlotItem->CreateItemUI( pItem->GetItemTemplet(), pSlotInfo->m_Quantity );
			pSlotItem->SetItemUID( pSlotInfo->m_ItemUID );

			//{{ kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
#ifdef	SEAL_ITEM
			if ( pItem->GetItemData().m_bIsSealed == true )
			{
				pSlotItem->SetShowSealedImage( true );
			}
			else
			{
				pSlotItem->SetShowSealedImage( false );
			}
#endif	SEAL_ITEM
			//}} kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// ���� ������ ������� ������ ǥ�ø� ���� ������ ����
			if ( NULL != g_pData &&
				 NULL != g_pData->GetUIManager() &&
				 NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				CX2SlotItem* pSlotInvenItem = g_pData->GetUIManager()->GetUIInventory()->GetSlotByItemUID( pSlotItem->GetItemUID() );

				if ( pSlotInvenItem != NULL )
					pSlotInvenItem->ResetItemUI( pItem );
			}
#endif //SERV_UPGRADE_TRADE_SYSTEM
		}
	}

	CloseDLGItemEnroll();
}


CX2UIPersonalShop::SlotInfo* CX2UIPersonalShop::GetShopSlot( int slotIndex )
{
	if ( slotIndex < 0 || slotIndex >= (int)m_vecSlotInfo.size() )
		return NULL;

	CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[slotIndex];
	return pSlotInfo;
}

CX2UIPersonalShop::SlotInfo* CX2UIPersonalShop::GetShopSlot( UidType itemUID )
{
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( pSlotInfo->m_ItemUID == itemUID )
			{
				return pSlotInfo;
			}
		}
	}

	return NULL;
}

wstring CX2UIPersonalShop::GetPersonalShopSlotItemFullName( UidType itemUID )
{
	wstring itemFullName = L"";

	for ( int i = 0; i < (int)m_vecPersonalShopSlotItem.size(); i++ )
	{
		KSellPersonalShopItemInfo& kInfo = m_vecPersonalShopSlotItem[i];
		if ( kInfo.m_kInventoryItemInfo.m_iItemUID == itemUID )
		{
			CX2Item::ItemData kItemData( kInfo.m_kInventoryItemInfo );
			CX2Item* pItem = new CX2Item( kItemData, NULL );
			itemFullName = pItem->GetFullName();
			SAFE_DELETE( pItem );
			break;
		}
	}

	return itemFullName;
}


wstring CX2UIPersonalShop::GetSlotItemDesc()
{

	wstring itemDesc=  L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
			m_pNowOverItemSlot->GetItemUID() );
		if ( pkItem != NULL )
			itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID(), false );
		else
		{
			CX2UIPersonalShop::SlotInfo* pSlotInfo = GetShopSlot( m_pNowOverItemSlot->GetItemUID() );
			if ( pSlotInfo != NULL )
			{
				itemDesc = GetPersonalShopSlotItemDesc( m_pNowOverItemSlot->GetItemUID() );
			}
			else
				itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID(), false );
		}
	}

	return itemDesc;

}


wstring CX2UIPersonalShop::GetPersonalShopSlotItemDesc( UidType itemUID )
{
	wstring itemDesc = L"";

	for ( int i = 0; i < (int)m_vecPersonalShopSlotItem.size(); i++ )
	{
		KSellPersonalShopItemInfo& kInfo = m_vecPersonalShopSlotItem[i];
		if ( kInfo.m_kInventoryItemInfo.m_iItemUID == itemUID )
		{
			CX2Item::ItemData kItemData( kInfo.m_kInventoryItemInfo );
			CX2Item* pItem = new CX2Item( kItemData, NULL );
			itemDesc = GetSlotItemDescByTID( pItem, pItem->GetItemData().m_ItemID, false );
			SAFE_DELETE( pItem );
			break;
		}
	}

	return itemDesc;

}

// �� �ڵ�� ������ ���� ���� �� ���̾�
void CX2UIPersonalShop::OpenBuyQuantityDLG( D3DXVECTOR2 pos )
{
	SAFE_DELETE_DIALOG(m_pDLGBuyQuantity);
	m_pDLGBuyQuantity = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );

	if ( NULL != m_pDLGBuyQuantity )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBuyQuantity );
		pos = pos - D3DXVECTOR2(75, 20);
		if(pos.x > 865 ) 
			pos.x = 865;
		if(pos.y > 633 )
			pos.y = 633;

		m_pDLGBuyQuantity->SetPos( pos );

		m_ItemNumToBuy = 0;
		m_pDLGBuyQuantity->SetCloseCustomUIEventID(UPSCM_BUY_QUANTITY_CANCLE);

		CKTDGUIStatic* pStaticPrice = static_cast<CKTDGUIStatic*>( m_pDLGBuyQuantity->GetControl( L"Static_Price" ) );
		if ( NULL != pStaticPrice )
			pStaticPrice->GetString(0)->msg = L"0";

		CKTDGUIButton* pButtonOK = static_cast<CKTDGUIButton*>( m_pDLGBuyQuantity->GetControl( L"Button_OK" ) );
		if ( NULL != pButtonOK )
			pButtonOK->SetCustomMsgMouseUp(UPSCM_BUY_QUANTITY_CONFIRM);

		CKTDGUIButton* pButtonCancle = static_cast<CKTDGUIButton*>( m_pDLGBuyQuantity->GetControl( L"Button_Cancle" ) );
		if ( NULL != pButtonCancle )
			pButtonCancle->SetCustomMsgMouseUp(UPSCM_BUY_QUANTITY_CANCLE);

		CKTDGUIButton* pButtonPlus = static_cast<CKTDGUIButton*>( m_pDLGBuyQuantity->GetControl( L"Button_Plus" ) );
		if ( NULL != pButtonPlus )
			pButtonPlus->SetCustomMsgMouseUp(UPSCM_BUY_QUANTITY_PLUS);

		CKTDGUIButton* pButtonMinus = static_cast<CKTDGUIButton*>( m_pDLGBuyQuantity->GetControl( L"Button_Minus" ) );
		if ( NULL != pButtonMinus )
			pButtonMinus->SetCustomMsgMouseUp(UPSCM_BUY_QUANTITY_MINUS);

		CKTDGUIIMEEditBox* pQuantity = static_cast<CKTDGUIIMEEditBox*>( m_pDLGBuyQuantity->GetControl( L"IME_Edit_Quantity" ) );
		if ( NULL != pQuantity )
		{
			pQuantity->SetText(L"0", true);	
			pQuantity->SetCustomMsgChange(UPSCM_BUY_QUANTITY_UPDATE);
			pQuantity->SetCustomMsgEnter(UPSCM_BUY_QUANTITY_CONFIRM);
			pQuantity->RequestFocus();
		}
	}
}

void CX2UIPersonalShop::UpdateBuyQuantityDLG( bool bReadIME )
{
	SlotInfo* pSlotInfo = m_vecSlotInfo[m_PickedShopItemIndex];
	if ( NULL != pSlotInfo && NULL != m_pDLGBuyQuantity )
	{
		CKTDGUIIMEEditBox* pIMEQuantity 
			= (CKTDGUIIMEEditBox*) m_pDLGBuyQuantity->GetControl( L"IME_Edit_Quantity" );

		if ( bReadIME && NULL != pIMEQuantity )
			m_ItemNumToBuy = g_pMain->GetEDFromString( pIMEQuantity->GetText() );

		if ( m_ItemNumToBuy <= 0 )
		{
			if(bReadIME)				// �Է�â���� �޾ƿ� ���
				m_ItemNumToBuy = 0;
			else						// << �� ���� ��� : 0 �Ʒ��� max�� ��ȯ��Ų��.
				m_ItemNumToBuy = pSlotInfo->m_Quantity;	
		}

		if(m_ItemNumToBuy > pSlotInfo->m_Quantity) 
			m_ItemNumToBuy = pSlotInfo->m_Quantity;

		WCHAR buff[256] = {0};
		_itow( m_ItemNumToBuy, buff, 10 );
		wstring wstrNum = buff;
		pIMEQuantity->SetText( wstrNum.c_str() );


		CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*) m_pDLGBuyQuantity->GetControl( L"Static_Price" );
		if ( NULL != pStaticPrice )
			pStaticPrice->GetString(0)->msg = g_pMain->GetEDString(m_ItemNumToBuy * pSlotInfo->m_PricePerOne);
	}
}

// ���� �ʹ� ���Ƽ� ���λ����� ������ �� ���ٸ� false�� return�Ѵ�.
bool CX2UIPersonalShop::CheckEdLimit()
{
	INT64 EDTotalAfterSell = 0;
	EDTotalAfterSell += g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;

	for(int i=0; i<(int)m_vecSlotInfo.size(); ++i)
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if(pSlotInfo == NULL)
			continue;
        
		EDTotalAfterSell += pSlotInfo->m_PricePerOne * pSlotInfo->m_Quantity;
	}
	
	if(EDTotalAfterSell < 0 || EDTotalAfterSell > MAX_ED_FOR_PLAYER)
	{
		return false;
	}

	return true;

}

//{{ kimhc // 2009-09-07 // ������ �������� ���ε� ���������� Ȯ��
#ifdef	SEAL_ITEM
bool CX2UIPersonalShop::CheckIsSealedItem( UidType itemUID )
{
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, personalShopSlotItem, m_vecPersonalShopSlotItem )
	{
		if ( personalShopSlotItem.m_kInventoryItemInfo.m_iItemUID == itemUID && 
			 personalShopSlotItem.m_kInventoryItemInfo.m_kItemInfo.IsSealedItem() )
		{
			return true;
		}

	}
	
	return false;
}

#endif	SEAL_ITEM
//}} kimhc // 2009-09-07 // ������ �������� ���ε� ���������� Ȯ��

#ifdef SERV_PSHOP_AGENCY
bool CX2UIPersonalShop::CreateAgencyShop()
{
	m_NowShopState = CX2UIPersonalShop::XPSS_AGENCY_WAIT;
	m_ShopName = L"";
	m_vecPersonalShopSlotItem.resize(0);

	ResetSlotList();
	SetDialog( m_NowShopState );

	if ( g_pSquareGame != NULL )
		g_pSquareGame->SetEnableKeyProcess( false );

	// oasis907 : ����� [2010.3.29] // 
	UpdateMegaphoneButton();

	// ���ȴٰ� ��ȣ
	m_bShow = true;
	D3DXVECTOR3 tmp;
	tmp = m_pDLGUIPersonalShop->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUIPersonalShop->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;
	g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_PERSONAL_SHOP);

	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);

#ifdef SERV_SOCKET_NEW
	// oasis907 : ����� [2010.4.6] // 
	if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
	}
	// oasis907 : ����� [2010.4.6] // ���� UI�� ���������� �ݴ´�.
	if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL)
	{
		g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->SetShow(false);
	}
#endif SERV_SOCKET_NEW

	if ( g_pTFieldGame != NULL )
	{
		CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
		if ( pJoinNpc != NULL )
		{
			if ( pJoinNpc->GetNpcShop() != NULL )
				pJoinNpc->GetNpcShop()->SetEnablePersonalShopBoardButton( false );
		}
	}

	return true;
}
bool CX2UIPersonalShop::RegisterAgencyShopItem()
{
	m_NowShopState = CX2UIPersonalShop::XPSS_AGENCY_SELL;
	
	if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
	}
	return true;
}
bool CX2UIPersonalShop::JoinMyAgencyShop()
{
	m_NowShopState = CX2UIPersonalShop::XPSS_AGENCY_SELL;
	SetDialog( m_NowShopState );

	// oasis907 : ����� [2010.3.29] // 
	UpdateMegaphoneButton();

#ifdef DEF_TRADE_BOARD
	//{{ oasis907 : ����� [2010.3.30] // 
	if ( g_pTFieldGame != NULL )
	{
		CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
		if ( pJoinNpc != NULL )
		{
			if ( pJoinNpc->GetNpcShop() != NULL )
				pJoinNpc->GetNpcShop()->SetEnablePersonalShopBoardButton( false );
		}
	}
	//}}
#endif DEF_TRADE_BOARD

	return true;
}
bool CX2UIPersonalShop::TakeMyAgencyShop()
{
	m_NowShopState = CX2UIPersonalShop::XPSS_AGENCY_TAKE;
	SetDialog( m_NowShopState );

#ifdef DEF_TRADE_BOARD
	//{{ oasis907 : ����� [2010.3.30] // 
	if ( g_pTFieldGame != NULL )
	{
		CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
		if ( pJoinNpc != NULL )
		{
			if ( pJoinNpc->GetNpcShop() != NULL )
				pJoinNpc->GetNpcShop()->SetEnablePersonalShopBoardButton( false );
		}
	}
	//}}
#endif DEF_TRADE_BOARD

	return true;
}
bool CX2UIPersonalShop::CloseAgencyShop()
{
	CloseShop();
	return true;
}

bool CX2UIPersonalShop::Handler_EGS_CREATE_PSHOP_AGENCY_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_CREATE_PSHOP_AGENCY_REQ );
	g_pMain->AddServerPacket( EGS_CREATE_PSHOP_AGENCY_ACK );

	return true;
}
bool CX2UIPersonalShop::Handler_EGS_CREATE_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_PSHOP_AGENCY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if( g_pMain->DeleteServerPacket( EGS_CREATE_PSHOP_AGENCY_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// �븮���� ������ ���� ��ǰ���
			CreateAgencyShop();
			return true;
		}
		else if( kEvent.m_iOK == NetError::ERR_PERSONAL_SHOP_37 )
		{
			// �̹� �븮���� ������ ���� - ���� �Ǹ����� ��ǰ����
			Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ(); //JoinMyAgencyShop();
			return true;
		}
		else
		{
			// �븮���� ���� �Ұ�	
			return false;
		}
	}
	
	return false;
}

bool CX2UIPersonalShop::Handler_EGS_REG_PSHOP_AGENCY_ITEM_REQ()
{
	KEGS_REG_PERSONAL_SHOP_ITEM_REQ kPacket;

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	if ( NULL != g_pData &&
		 NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		m_ShopKeeperName = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName;

		/// @���� ���λ���
		kPacket.m_wstrPersonalShopName = GET_REPLACED_STRING( ( STR_ID_864, "L", m_ShopKeeperName ) );
	}
#else // SERV_UPGRADE_TRADE_SYSTEM
	kPacket.m_wstrPersonalShopName = m_ShopName;
#endif // SERV_UPGRADE_TRADE_SYSTEM

	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( pSlotInfo->m_ItemUID != 0 && pSlotInfo->m_Quantity != 0 )
			{
				KRegPersonalShopItemInfo kRegPersonalShopItemInfo;
				kRegPersonalShopItemInfo.m_iItemUID		= pSlotInfo->m_ItemUID;
				kRegPersonalShopItemInfo.m_iPricePerOne = pSlotInfo->m_PricePerOne;
				kRegPersonalShopItemInfo.m_iQuantity	= pSlotInfo->m_Quantity;

				kPacket.m_vecRegItem.push_back( kRegPersonalShopItemInfo );
			}
		}
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_REG_PSHOP_AGENCY_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK );

	return true;
}
bool CX2UIPersonalShop::Handler_EGS_REG_PSHOP_AGENCY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REG_PSHOP_AGENCY_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ

			RegisterAgencyShopItem();
			CloseAgencyShop();

			// �κ��丮 ������Ʈ
			if( g_pData->GetUIManager()->GetUIInventory() != NULL )
			{		
				CX2UIInventory* pUIInventory = g_pData->GetUIManager()->GetUIInventory();

				if ( NULL != pUIInventory )
				{
					pUIInventory->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);

					/// ���� ������ ������� ������ ǥ�ø� ���� ������ �ʱ�ȭ
					const CX2Inventory::SORT_TYPE	eNowInventorySortType	= pUIInventory->GetInventorySortType();
					const int						iNowInventoryPageNum	= pUIInventory->GetNowInventoryPageNum( eNowInventorySortType );
					pUIInventory->SetInventorySort( eNowInventorySortType, iNowInventoryPageNum );
				}
			}

			/// �����Ḧ ���� ���� ED ����
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

			Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ();

#else //SERV_UPGRADE_TRADE_SYSTEM

			// �κ��丮 ������Ʈ
			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{		
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);
			}

			RegisterAgencyShopItem();
			CloseAgencyShop();
			Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ();

#endif //SERV_UPGRADE_TRADE_SYSTEM
			return true;
		}
	}

	return false;
}

bool CX2UIPersonalShop::Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ(bool bOnlyView)
{
	KEGS_JOIN_MY_PSHOP_AGENCY_REQ kPacket;

	kPacket.m_cJoinType = KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST;
	if( bOnlyView == false )
		kPacket.m_cJoinType = KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_FOR_PICK_UP;

	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK );

	return true;
}
bool CX2UIPersonalShop::Handler_EGS_JOIN_MY_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_MY_PSHOP_AGENCY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			std::wstring	m_wstrPersonalShopName;
			std::wstring	m_wstrNickName;
			std::vector<KSellPersonalShopItemInfo> m_vecSellItemInfo;

			m_ShopName = kEvent.m_wstrPersonalShopName;
			m_ShopKeeperName = kEvent.m_wstrNickName;

			m_vecPersonalShopSlotItem.clear();
			m_vecPersonalShopSlotItem = kEvent.m_vecSellItemInfo;

#ifdef LIMIT_PERSONAL_SHOP_PAGE

		#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// �� Ÿ�Կ� ���� �ִ� ���� ����
			const int iLimitItemNum = GetMaxItemPageIndex() * 3;

			if ( static_cast<int>( m_vecPersonalShopSlotItem.size() ) > iLimitItemNum )
			{
				m_vecPersonalShopSlotItem.resize(iLimitItemNum);
			}

		#else // SERV_UPGRADE_TRADE_SYSTEM

			if ( m_vecPersonalShopSlotItem.size() > 9 )
			{
				m_vecPersonalShopSlotItem.resize(9);
			}

		#endif // SERV_UPGRADE_TRADE_SYSTEM

			m_iMaxPageIndex =  (int) ( ((int)m_vecPersonalShopSlotItem.size()+2) / 3);
			m_iNowPageIndex = 1;

			ResetSlotList( m_vecPersonalShopSlotItem );
#else
			m_iMaxPageIndex =  (int) ( ((int)kEvent.m_vecSellItemInfo.size()+2) / 3);
			m_iNowPageIndex = 1;
			ResetSlotList( kEvent.m_vecSellItemInfo );
#endif //LIMIT_PERSONAL_SHOP_PAGE

			if( kEvent.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST )
				JoinMyAgencyShop();
			else
				TakeMyAgencyShop();

			// ���ȴٰ� ��ȣ
			m_bShow = true;
			D3DXVECTOR3 tmp;
			tmp = m_pDLGUIPersonalShop->GetDummyPos(0);
			m_DLGPosition.x = tmp.x;
			m_DLGPosition.y = tmp.y;
			tmp = m_pDLGUIPersonalShop->GetDummyPos(1);
			m_DLGSize.x = tmp.x;
			m_DLGSize.y = tmp.y;
			g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_PERSONAL_SHOP);

#ifdef SERV_SOCKET_NEW
			// oasis907 : ����� [2010.4.6] // 
			if ( g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
			}
			// oasis907 : ����� [2010.4.6] // ���� UI�� ���������� �ݴ´�.
			if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->SetShow(false);
			}
#endif SERV_SOCKET_NEW

			return true;
		}

	}

	return false;
}

bool CX2UIPersonalShop::Handler_EGS_STOP_SALE_PSHOP_AGENCY_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_STOP_SALE_PSHOP_AGENCY_REQ );
	g_pMain->AddServerPacket( EGS_STOP_SALE_PSHOP_AGENCY_ACK );

	return true;
}
bool CX2UIPersonalShop::Handler_EGS_STOP_SALE_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STOP_SALE_PSHOP_AGENCY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_STOP_SALE_PSHOP_AGENCY_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			Handler_EGS_LEAVE_PSHOP_AGENCY_REQ();
			//CloseAgencyShop();

			return true;
		}
	}

	return false;
}

bool CX2UIPersonalShop::Handler_EGS_LEAVE_PSHOP_AGENCY_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_LEAVE_PSHOP_AGENCY_REQ );
	g_pMain->AddServerPacket( EGS_LEAVE_PSHOP_AGENCY_ACK );

	return true;
}
bool CX2UIPersonalShop::Handler_EGS_LEAVE_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_PSHOP_AGENCY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_LEAVE_PSHOP_AGENCY_ACK ) == true )
	{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		/// ������ ��Ŷ�̶�, â�� �ݾ�����
		CloseAgencyShop();
	#endif //SERV_UPGRADE_TRADE_SYSTEM

		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
	#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// ���� ������ ������� ������ ǥ�� ������ ���� ������ ����
			if( g_pData->GetUIManager()->GetUIInventory() != NULL )
			{		
				CX2UIInventory* pUIInventory = g_pData->GetUIManager()->GetUIInventory();

				if ( NULL != pUIInventory )
					pUIInventory->UpdateInventorySlot();
			}
	#else //SERV_UPGRADE_TRADE_SYSTEM
			CloseAgencyShop();
	#endif //SERV_UPGRADE_TRADE_SYSTEM

			return true;
		}
	}

	return false;
}
bool CX2UIPersonalShop::Handler_EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ(int iSlotInx)
{
	KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacket;
	vector<UidType> vecTakeItemList;

	if( iSlotInx >= 0 && iSlotInx < (int)m_vecSlotInfo.size() )
	{
		SlotInfo* pSlotInfo = m_vecSlotInfo[iSlotInx];
		if( pSlotInfo != NULL )
		{
			vecTakeItemList.push_back(pSlotInfo->m_ItemUID);
		}
	}
	else
	{
		for(int i=0; i<(int)m_vecSlotInfo.size(); ++i)
		{
			SlotInfo* pSlotInfo = m_vecSlotInfo[i];
			if( pSlotInfo != NULL && pSlotInfo->m_ItemUID > 0 )
			{				
				vecTakeItemList.push_back(pSlotInfo->m_ItemUID);
			}
		}
	}
	kPacket.m_vecPickUpItemList = vecTakeItemList;

	g_pData->GetServerProtocol()->SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	return true;
}
bool CX2UIPersonalShop::Handler_EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	if ( g_pMain->DeleteServerPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;
			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{		
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);
			}

			m_vecPersonalShopSlotItem.clear();
			m_vecPersonalShopSlotItem = kEvent.m_vecSellItemInfo;
			ResetSlotList( kEvent.m_vecSellItemInfo );
			
			ResetBuySlotUIList();

			if( kEvent.m_vecSellItemInfo.size() == 0 )
			{
				CloseAgencyShop();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
				/// ������ ���� �ʾҴٸ�, ���� ó�� ���ּ���!
				if ( true == kEvent.m_bRemainSellItem )
					Handler_EGS_LEAVE_PSHOP_AGENCY_REQ();
#endif // SERV_UPGRADE_TRADE_SYSTEM 
			}

			return true;
		}
	}

	return false;


	
}

#endif


#endif

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
/** @function	: GetMaxItemPageIndex
	@brief		: ���� �� Ÿ�Կ� ���� �ִ� ��� ������ �� ��ȯ
	@return		: �ִ� ��� ������ ��
*/
const int CX2UIPersonalShop::GetMaxItemPageIndex()
{
	int iMaxPageIndex = 1;

	if ( NULL != g_pInstanceData )
	{
		const SEnum::AGENCY_SHOP_TYPE eAgencyShopType = g_pInstanceData->GetAgencyShopType();

		switch( eAgencyShopType )
		{
		case SEnum::AST_PREMIUM:	iMaxPageIndex = 5;	break;		/// ĳ�� ���� ������
		case SEnum::AST_NORMAL:		iMaxPageIndex = 3;	break;		/// ED ���� ������
		default:					iMaxPageIndex = 1;	break;		/// ���� ������
		}
	}

	return iMaxPageIndex;
}

/** @function	: CheckCanPaymentRegistTex
	@brief		: ��ǰ ��Ͽ� �ʿ��� �����Ḧ ������ �� �ִ°�
	@return		: ���� ����
*/
const bool CX2UIPersonalShop::CheckCanPaymentRegistTex()
{
	/// ĳ�� ������ ������ �� �ʿ䰡 ����.
	if ( NULL != g_pInstanceData && SEnum::AST_PREMIUM == g_pInstanceData->GetAgencyShopType() )
		return true;

	if ( NULL == g_pData ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() )
		 return false;

	int iItemNum = 0;	/// ����� ������ ��

	//�ϴ� ����ҷ��� �������� �Ѱ� �̻��� �� ��� üũ�Ѵ�.
	for ( int i = 0; i < (int)m_vecSlotInfo.size(); i++ )
	{
		CX2UIPersonalShop::SlotInfo* pSlotInfo = m_vecSlotInfo[i];
		if ( pSlotInfo != NULL )
		{
			if ( pSlotInfo->m_ItemUID != 0 && pSlotInfo->m_Quantity != 0 )
				++iItemNum;
		}
	}

	const int iTex = iItemNum * 1000;		/// ������ ��� ( ���� 1000ED )

	/// ���� ������ ������ �� �� ���ٸ�, False ó��
	if ( iTex > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
		return false;

	return true;
}
#endif //SERV_UPGRADE_TRADE_SYSTEM