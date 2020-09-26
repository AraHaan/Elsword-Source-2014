#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI
#include ".\X2UIPersonalTrade.h"

CX2UIPersonalTrade::CX2UIPersonalTrade( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2UIDragable( pNowStage, NULL ),
m_pDLGPersonalTrade(NULL),
m_pDLGItemEnroll(NULL),
m_pDLGMsgBox(NULL),
m_bShow(false),
m_bTrading(false),
m_MovedPosition(0,0),					// D3DXVECTOR2
m_DLGPosition(0,0),						// D3DXVECTOR2
m_DLGSize(0,0),							// D3DXVECTOR2
#ifdef ADJUST_THIRD_PERSON_BUG
m_bTradeRequesting(false),
#endif //ADJUST_PERSONAL_TRADE_BUG
//m_vecTradeInvited.clear();
m_pTradeInviting(NULL),
m_iTradeUID(0),
m_iOpponentUID(0),
m_wstrOpponentNickName(L""),
m_bOpponentRegisteredTrade(false),
m_bRegisteredTrade(false),
m_bOpponentApprovedTrade(false),
m_eNowMyTradeButtonStep(TS_BEFORE_REGISTER),
m_eNowOpponentTradeButtonStep(TS_BEFORE_REGISTER),
m_pTargetDragItemSlot(NULL),
m_iItemEnrollNum(0),
m_EnrollItemUid(0),
//m_vMyItemSlot.clear();
//m_vOpponentItemSlot.clear();
m_iMyED(0),
m_iOpponentED(0)
//{{ kimhc // 2009-09-07 // ���ε� �������� �ŷ� �ϰڳİ� ���� ���̾�α�
#ifdef	SEAL_ITEM
, m_pDLGTradeSealedItem( NULL )
#endif	SEAL_ITEM
//}} kimhc // 2009-09-07 // ���ε� �������� �ŷ� �ϰڳİ� ���� ���̾�α�
#ifdef NUMBER_TO_LANGUAGE
, m_pTalkBoxMgr( NULL )
, m_iChangingEDLimit( 9 )
, m_iChangedEDLimit( 11 )
#endif NUMBER_TO_LANGUAGE
{
	//////////////////////////////////////////////////////////////////////////

	m_vecTradeInvited.clear();
	m_vMyItemSlot.clear();
	m_vOpponentItemSlot.clear();

}

CX2UIPersonalTrade::~CX2UIPersonalTrade()
{
	SAFE_DELETE_DIALOG(m_pDLGPersonalTrade);
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	
	//{{ kimhc // 2009-09-07 // ���ε� �������� �ŷ� �ϰڳİ� ���� ���̾�α�
#ifdef	SEAL_ITEM
	SAFE_DELETE_DIALOG( m_pDLGTradeSealedItem );
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-07 // ���ε� �������� �ŷ� �ϰڳİ� ���� ���̾�α�

	m_vMyItemSlot.clear();
	m_vOpponentItemSlot.clear();

#ifdef NUMBER_TO_LANGUAGE
	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxMgr );
#endif NUMBER_TO_LANGUAGE
	
}



CX2UIPersonalTrade::TradeInvited::TradeInvited( UidType iInviterUID, wstring& wstrInviterName, 
											float fTimeLeft, int YesMsgID, int NoMsgID, CKTDXStage* pNowStage )
{
	m_pDLGInvited		= NULL;

	m_iInviterUID		= iInviterUID;
	m_wstrInviterName	= wstrInviterName;
	m_fTimeLeftForReplyInvite = fTimeLeft;

	m_pDLGInvited = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_872, "Li", m_wstrInviterName, ( int )m_fTimeLeftForReplyInvite ) ), YesMsgID, (CKTDXStage*)pNowStage, NoMsgID );
}

CX2UIPersonalTrade::TradeInvited::~TradeInvited()
{
	SAFE_DELETE_DIALOG( m_pDLGInvited );
}

void CX2UIPersonalTrade::TradeInvited::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( m_fTimeLeftForReplyInvite > 0.f )
	{
		int iOldTimeLeft = (int)m_fTimeLeftForReplyInvite;
		m_fTimeLeftForReplyInvite -= fElapsedTime;

		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGInvited ) == true )
		{
			if( m_fTimeLeftForReplyInvite < 0.f )
			{
				if( NULL != m_pDLGInvited && true == m_pDLGInvited->GetShow() )
				{

					m_pDLGInvited->SetShow( false );
					m_pDLGInvited->SetEnable( false );
				}
			}
			else
			{
				if( NULL != m_pDLGInvited && 
					true == m_pDLGInvited->GetShow() &&
					iOldTimeLeft != (int)m_fTimeLeftForReplyInvite )
				{
					CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGInvited->GetControl( L"StaticMsgBoxBG" );

					if( pStatic != NULL )
					{
						pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_872, "Li", m_wstrInviterName, ( int )m_fTimeLeftForReplyInvite ) );
					}					
				}
			}
		}	
	}
}



CX2UIPersonalTrade::TradeInviting::TradeInviting( wstring& wstrInviteeName, float fTimeLeft, CKTDXStage* pNowStage )
{
	m_pDLGInviting = NULL;
	m_wstrInviteeName = wstrInviteeName;
	m_fTimeLeftForWaitingReply = fTimeLeft;

	m_pDLGInviting = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_873, "Li", m_wstrInviteeName, ( int )m_fTimeLeftForWaitingReply ) ), (CKTDXStage*)pNowStage );
}

CX2UIPersonalTrade::TradeInviting::~TradeInviting()
{
	m_pDLGInviting->SetShow(false);
	SAFE_DELETE_DIALOG( m_pDLGInviting );
}

void CX2UIPersonalTrade::TradeInviting::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( m_fTimeLeftForWaitingReply > 0.f )
	{
		int iOldTimeLeft = (int)m_fTimeLeftForWaitingReply;
		m_fTimeLeftForWaitingReply -= fElapsedTime;

		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGInviting ) == true )
		{
			if( m_fTimeLeftForWaitingReply < 0.f )
			{
				if( NULL != m_pDLGInviting && true == m_pDLGInviting->GetShow() )
				{
					m_pDLGInviting->SetShow( false );
					m_pDLGInviting->SetEnable( false );
				}
			}
			else
			{
				if( NULL != m_pDLGInviting && 
					true == m_pDLGInviting->GetShow() &&
					iOldTimeLeft != (int)m_fTimeLeftForWaitingReply )
				{
					CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGInviting->GetControl( L"StaticMsgBoxBG" );

					if( pStatic != NULL )
					{
						pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_873, "Li", m_wstrInviteeName, ( int )m_fTimeLeftForWaitingReply ) );
					}
				}
			}
		}	
	}
}


HRESULT CX2UIPersonalTrade::OnFrameMove( double fTime, float fElapsedTime )
{
#ifdef DIALOG_SHOW_TOGGLE
	if( m_bTrading == true && g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->SetShow( true );
	}
#endif

	for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
	{
		TradeInvited* pTradeInvited = m_vecTradeInvited[i];
		pTradeInvited->OnFrameMove( fTime, fElapsedTime );

		if( pTradeInvited->m_fTimeLeftForReplyInvite <= 0.f )
		{
			SAFE_DELETE( pTradeInvited );
			m_vecTradeInvited.erase( m_vecTradeInvited.begin() + i );
			i--;
		}
	}

	if( NULL != m_pTradeInviting )
	{
		m_pTradeInviting->OnFrameMove( fTime, fElapsedTime );
		if( m_pTradeInviting->m_fTimeLeftForWaitingReply <= 0.f )
		{
			SAFE_DELETE( m_pTradeInviting );
#ifdef ADJUST_THIRD_PERSON_BUG
			m_bTradeRequesting = false;
#endif //ADJUST_PERSONAL_TRADE_BUG
		}
	}

	if(true == m_bShow)
	{
		if( (m_pDLGPersonalTrade != NULL && m_pDLGPersonalTrade->GetIsMouseOver() == true) )
		{
			return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
		}
		else
		{
			CX2ItemSlotManager::InvalidSelectedItem();
			CX2ItemSlotManager::InvalidSlotDesc();
		}

		//{{ JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)
#ifdef NUMBER_TO_LANGUAGE
		if ( NULL != m_pTalkBoxMgr )
		{
			m_pTalkBoxMgr->SetShowObject( true );
		}
#endif NUMBER_TO_LANGUAGE
		//}} JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)

	}
	
	return S_OK;

}


bool CX2UIPersonalTrade::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �տ� ��� ���̾�αװ� ������ �޽����� ó������ �ʴ´�
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGPersonalTrade ) )
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
			if ( false == m_pDLGPersonalTrade->GetIsMouseOver())
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
			if ( false == m_pDLGPersonalTrade->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_PERSONAL_TRADE, true);
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

			MouseUp( mousePos );

		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			// �÷��� �������� �巡�� �� �� ���� : �ش� �޽����� ó������ �ʴ´�
			// ������ ���� �� ��Ŭ������ �ϰ�
//			if ( MouseDown( mousePos) == true )
//			return true;

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

	case WM_RBUTTONDOWN:
		{

			SetNowDragItemReturn();
		}
		break;

	case WM_RBUTTONUP:
		{
			SetNowDragItemReturn();

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			OnRClickedItem( mousePos );

		}
		break;
	}

	return bFlag;
}


bool CX2UIPersonalTrade::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UPTCM_TRADE_INVITE_ACCEPT:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();

			//m_TimerTradeAcceptButtonClicked.restart();

			if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) == true )
			{
				for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
				{
					TradeInvited* pTradeInvited = m_vecTradeInvited[i];
					if( pTradeInvited->m_pDLGInvited == pDialog )
					{
						pTradeInvited->m_fTimeLeftForReplyInvite = 0.f;
						Handler_EGS_REQUEST_TRADE_REPLY_NOT( pTradeInvited->m_iInviterUID, true ); 
						break;
					}
				}

				// ��� �ٸ� �ŷ���û ���̾�α׸� disable, �ټ��� �ŷ��� ���ÿ� �����ϴ� ���� ���� ����
				for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
				{
					TradeInvited* pTradeInvited = m_vecTradeInvited[i];
					pTradeInvited->m_fTimeLeftForReplyInvite = 0.f;
					pTradeInvited->m_pDLGInvited->SetShowEnable( false, false ); 
				}
			}
			return true;
		}
		break;
	case UPTCM_TRADE_INVITE_REFUSE:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			CKTDGUIDialogType pDialog = pButton->GetDialog();

			if( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) == true )
			{
				for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
				{
					TradeInvited* pTradeInvited = m_vecTradeInvited[i];
					if( pTradeInvited->m_pDLGInvited == pDialog )
					{
						pTradeInvited->m_fTimeLeftForReplyInvite = 0.f;
						Handler_EGS_REQUEST_TRADE_REPLY_NOT( pTradeInvited->m_iInviterUID, false ); 
						break;
					}
				}
			}

			return true;

		} break;


	case UPTCM_TRADE_ITEM_REGISTER:
		{
			// �ŷ��� ����ϱ����� ED�� �ѹ��� Ȯ��~
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGPersonalTrade->GetControl( L"IME_Edit_MyED" );

			int iED = g_pMain->GetEDFromString( pIMEEditBox->GetText() );
			if( iED < 0 )
			{
				iED = 0;
			}
			else if( iED > MAX_ED_FOR_TRADE )
			{
				iED = MAX_ED_FOR_TRADE;
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_12256 ), g_pMain->GetNowState() );
			}

			if( iED > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
			{
				iED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
			}

			wstring wstrED = g_pMain->GetEDString( iED );
			pIMEEditBox->SetText( wstrED.c_str() );

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			pIMEEditBox->SetTextColor( g_pMain->GetPriceColor( iED ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

			Handler_EGS_UPDATE_TRADE_ED_REQ( iED );

			return Handler_EGS_OFFER_TRADE_REQ();
		} break;

		//{{ kimhc // 2009-09-07 // ���ε� �������� Ʈ���̵� ��Ͽ� ���� ��� �ߴ� ���̾�α��� Ȯ��, ���
#ifdef	SEAL_ITEM
	case UPTCM_TRADE_SEALED_ITEM_ACCEPT:
		{
			if ( m_pDLGTradeSealedItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGTradeSealedItem, NULL, false );
				m_pDLGTradeSealedItem = NULL;
			}

			return Handler_EGS_APPROVE_TRADE_REQ();

		} break;

	case UPTCM_TRADE_SEALED_ITEM_CANCEL:
		{
			if ( m_pDLGTradeSealedItem != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGTradeSealedItem, NULL, false );
				m_pDLGTradeSealedItem = NULL;
			}

			return true;

		} break;

#endif	SEAL_ITEM
		//}} kimhc // 2009-09-07 // ���ε� �������� Ʈ���̵� ��Ͽ� ���� ��� �ߴ� ���̾�α��� Ȯ��, ���

	case UPTCM_TRADE_ITEM_ACCEPT:
		{
			//{{ kimhc // 2009-09-07 // ���ε� �������� Ʈ���̵� ��Ͽ� ���� ���
#ifdef	SEAL_ITEM
			if ( CheckIsSealedItemInTradeSlot() == true )
			{
				if ( m_pDLGTradeSealedItem != NULL )
				{
					SAFE_DELETE_DIALOG( m_pDLGTradeSealedItem );
					m_pDLGTradeSealedItem = NULL;
				}

				m_pDLGTradeSealedItem = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4474 ), 
					UPTCM_TRADE_SEALED_ITEM_ACCEPT, g_pMain->GetNowState(), UPTCM_TRADE_SEALED_ITEM_CANCEL  );

				return true;
			}
#endif	SEAL_ITEM
			//}} kimhc // 2009-09-07 // ���ε� �������� Ʈ���̵� ��Ͽ� ���� ���
			
			return Handler_EGS_APPROVE_TRADE_REQ();
		}break;
	case UPTCM_TRADE_ITEM_CANCEL:
		{
			if( NULL != m_pDLGMsgBox )
			{
				SAFE_DELETE_DIALOG( m_pDLGMsgBox );
				m_pDLGMsgBox = NULL;
			}

			m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_874 ), 
				UPTCM_TRADE_ITEM_CANCEL_OK, g_pMain->GetNowState()  );
			return true;
		} break;
	case UPTCM_TRADE_ITEM_CANCEL_OK:
		{
			Handler_EGS_BREAK_TRADE_REQ( NetError::NOT_LEAVE_ROOM_REASON_00 );

// 			switch( g_pMain->GetNowStateID() )
// 			{
// 			case CX2Main::XS_VILLAGE_MAP:
// 			case CX2Main::XS_SQUARE_GAME:
// 			case CX2Main::XS_PVP_ROOM:
// 			case CX2Main::XS_DUNGEON_ROOM:
// 			case CX2Main::XS_ARCADE_ROOM:
// 				//ClosePersonalTradeWindow();
// 				break;
// 			default:
// 				break;
// 			}
			return true;

		} break;
	case UPTCM_TRADE_ITEM_ED_CHANGED:
		{
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) lParam;
			int iTempED = g_pMain->GetEDFromString( pIMEEditBox->GetText() );
			if( iTempED < 0 )
			{
				iTempED = 0;
			}
			else if( iTempED > MAX_ED_FOR_TRADE )
			{
				iTempED = MAX_ED_FOR_TRADE;
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_417 ), (CKTDXStage*) g_pMain->GetNowState() );
			}

			if( iTempED > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED )
			{
				iTempED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
			}

			if( m_bRegisteredTrade == true )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_875 ), (CKTDXStage*) g_pMain->GetNowState() );
				return true;
			}

			//wstring wstrED = g_pMain->GetEDString( iTempED );
 			//pIMEEditBox->SetText( wstrED.c_str() );

			if( NULL != m_pDLGPersonalTrade )
			{
				m_pDLGPersonalTrade->ClearFocus();
			}

#ifdef NUMBER_TO_LANGUAGE
			pIMEEditBox->SetLengthLimit_LUA( m_iChangedEDLimit );
			if ( m_iMyED == iTempED )
			{
				SetMyTradeED( iTempED );
			}
#endif NUMBER_TO_LANGUAGE

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			pIMEEditBox->SetTextColor( g_pMain->GetPriceColor( iTempED ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

			return Handler_EGS_UPDATE_TRADE_ED_REQ( iTempED );

		}
		break;

	case UPTCM_REGISTER_QUANTITY_CONFIRM:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );
			m_pDLGItemEnroll = NULL;

			RegisterMyItem( m_EnrollItemUid, m_iItemEnrollNum );
			
			m_pDLGItemEnroll = NULL;
			m_pTargetDragItemSlot	= NULL; 
			
		}
		break;
	case UPTCM_REGISTER_QUANTITY_CANCLE:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );
			
			m_pDLGItemEnroll = NULL;
			m_pTargetDragItemSlot	= NULL; 
			m_iItemEnrollNum = 0;
			m_EnrollItemUid = NULL;
		}
		break;
	case UPTCM_REGISTER_QUANTITY_PLUS:
		{
			m_iItemEnrollNum++;
			UpdateRegisterQuantityDLG(false);
		}
		break;
	case UPTCM_REGISTER_QUANTITY_MINUS:
		{
			m_iItemEnrollNum--;
			UpdateRegisterQuantityDLG(false);
		}
		break;
	case UPTCM_REGISTER_QUANTITY_UPDATE:
		{
			UpdateRegisterQuantityDLG(true);
		}
		break;

		//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
	case UPTCM_TRADE_ITEM_ED_CHANGING:
		{
			CKTDGUIIMEEditBox* pIME_Myed = (CKTDGUIIMEEditBox*) m_pDLGPersonalTrade->GetControl( L"IME_Edit_MyED" );
			int iED = g_pMain->GetEDFromString( pIME_Myed->GetText() );

			pIME_Myed->SetTextColor( g_pMain->GetPriceColor( iED ) );
#ifdef NUMBER_TO_LANGUAGE
			if (pIME_Myed->GetTextLength() == 0)
			{
				iED = 0;
			}
			
			D3DXVECTOR2 vPos = m_pDLGPersonalTrade->GetPos() + D3DXVECTOR2(195, 70);
			PopPersonalTradeTalkBox( m_iTradeUID, iED, vPos, D3DXCOLOR(0, 0, 0, 1), g_pMain->GetPriceColor( iED ), g_pMain->GetPriceColor( iED ) );
#endif NUMBER_TO_LANGUAGE
		}
		break;
#endif	PRICE_COLOR
		//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

		//{{ JHKang / 2010-09-14 / IMEEDIT FocusIn �� ED ���� ����(ȭ�� ���� --> ���� ����)
#ifdef NUMBER_TO_LANGUAGE
	case UPTCM_TRADE_ITEM_ED_FOCUSIN:
		{
			CKTDGUIIMEEditBox* pIME_Myed = (CKTDGUIIMEEditBox*) m_pDLGPersonalTrade->GetControl( L"IME_Edit_MyED" );

			CX2NumbersToLanguage test;
			int iED = g_pMain->GetEDFromString( pIME_Myed->GetText() );
			string strText;
			if ( 0 == iED )
				strText.clear();
			else
				strText = test.ToString( iED );
			//pIME_Myed->SetText( g_pMain->GetEDString(iED).c_str() );
			pIME_Myed->SetText_LUA( strText.c_str() );
			pIME_Myed->SetLengthLimit_LUA( m_iChangingEDLimit );
		}
		break;
#endif NUMBER_TO_LANGUAGE
		//}} JHKang / 2010-09-14 / IMEDIT FocusIn �� ED ���� ����(ȭ�� ���� --> ���� ����)


	default:
		break;
	}

	return false;
}

bool CX2UIPersonalTrade::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case EGS_REQUEST_TRADE_ACK:
		{
			return Handler_EGS_REQUEST_TRADE_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_REQUEST_TRADE_NOT:
		{
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_SQUARE_GAME:
			case CX2Main::XS_PVP_ROOM:
			case CX2Main::XS_DUNGEON_ROOM:
			case CX2Main::XS_BATTLE_FIELD:
				{
					// CX2StateDungeonRoom* pState = (CX2StateDungeonRoom*) g_pMain->GetNowState();
					// if( true == pState->GetPersonalTrade()->GetEnable() ) // �̹� �ŷ����̸�
					if( true == m_bTrading ) // �̹� �ŷ����̸�
					{
						return true;
					}
					else
					{
						return Handler_EGS_REQUEST_TRADE_NOT( hWnd, uMsg, wParam, lParam );
					}
				} break;
			}

		} break;
	case EGS_REQUEST_TRADE_DENY_NOT:
		{
			return Handler_EGS_REQUEST_TRADE_DENY_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_CREATE_TRADE_NOT:
		{
			return Handler_EGS_CREATE_TRADE_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_BREAK_TRADE_ACK:
		{
			return Handler_EGS_BREAK_TRADE_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_BREAK_TRADE_NOT:
		{
			return Handler_EGS_BREAK_TRADE_NOT( hWnd, uMsg, wParam, lParam );

		} break;


	case EGS_TRADE_USER_INFO_NOT:
		{
			return Handler_EGS_TRADE_USER_INFO_NOT( hWnd, uMsg, wParam, lParam );

		} break;


	case EGS_JOIN_TRADE_NOT:
		{
			if( true == m_bTrading ) // �̹� �ŷ����̶�� �̰� �����ε�
			{
				return true;
			}
			else
			{
				return Handler_EGS_JOIN_TRADE_NOT( hWnd, uMsg, wParam, lParam );
			}

		} break;

	default:
		break;
	}


	// �ŷ����� �ƴϸ� �Ѿ��
	if( false == m_bTrading )
		return false;

	switch( wParam )
	{
	case EGS_UPDATE_TRADE_ITEM_ACK:
		{
			return Handler_EGS_UPDATE_TRADE_ITEM_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_UPDATE_TRADE_ITEM_NOT:
		{
			return Handler_EGS_UPDATE_TRADE_ITEM_NOT( hWnd, uMsg, wParam, lParam );

		} break;


	case EGS_UPDATE_TRADE_ED_ACK:
		{
			return Handler_EGS_UPDATE_TRADE_ED_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_UPDATE_TRADE_ED_NOT:
		{
			return Handler_EGS_UPDATE_TRADE_ED_NOT( hWnd, uMsg, wParam, lParam );

		} break;



	case EGS_OFFER_TRADE_ACK:
		{
			return Handler_EGS_OFFER_TRADE_ACK( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_OFFER_TRADE_NOT:
		{
			return Handler_EGS_OFFER_TRADE_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_APPROVE_TRADE_ACK:
		{
			return Handler_EGS_APPROVE_TRADE_ACK( hWnd, uMsg, wParam, lParam );

		} break;


	case EGS_APPROVE_TRADE_NOT:
		{
			return Handler_EGS_APPROVE_TRADE_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	case EGS_TRADE_COMPLETE_NOT:
		{
			return Handler_EGS_TRADE_COMPLETE_NOT( hWnd, uMsg, wParam, lParam );

		} break;

	}

	return false;

}

void CX2UIPersonalTrade::BreakTrade()
{
	if(m_bTrading) // â�� ���� ����(�ŷ���)�� Ȯ���ϰ�
	{
		// �ݾƹ�����~
		Handler_EGS_BREAK_TRADE_REQ( NetError::NOT_LEAVE_ROOM_REASON_00 );
	}
	else	// �ŷ����� �ƴϸ�
	{
		ClosePersonalTradeWindow();		// ��
	}
}


bool CX2UIPersonalTrade::OnRClickedItem( D3DXVECTOR2 mousePos )
{
	CX2SlotItem* pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );

	if(NULL == pItemSlot)
		return false;
	
	//���� �����̸�
	if(pItemSlot->GetSlotID() >= PTSI_OPPONENT_SLOT_0 )
		return false;

	return OnRemoveItem( pItemSlot->GetItemUID() );	
}

bool CX2UIPersonalTrade::OnRemoveItem( UidType EnrollItemUid )
{
	if( m_bRegisteredTrade == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_876 ), (CKTDXStage*) g_pMain->GetNowState() );
		return true;
	}

	// ��� �õ��ϴ� �������� ������ �����صд� (���ִ°�����)
	m_EnrollItemUid = EnrollItemUid;
	m_iItemEnrollNum = 0;


	InvalidSlotDesc();

	return Handler_EGS_UPDATE_TRADE_ITEM_REQ( m_EnrollItemUid, 0, true );
}

bool CX2UIPersonalTrade::OnDropAnyItem( D3DXVECTOR2 mousePos )
{

	//{{ 2008.11.13 ���¿� : UI ����ó��
	if(!(*m_pSlotBeforeDragging)->IsItem()) return false;
	//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
	//}}

	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pDragItem = kInventory.GetItem( *m_DraggingItemUID );
	CX2SlotItem* pDragSlot = (CX2SlotItem*) *m_pSlotBeforeDragging;

	CX2SlotItem* m_pTargetDragItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
	// ������ ���ų� �� ������ �ƴϸ�
	if ( m_pTargetDragItemSlot == NULL || m_pTargetDragItemSlot->GetSlotID() >= PTSI_OPPONENT_SLOT_0  )
		return false;
	
	if ( pDragItem == NULL || 
        NULL == pDragItem->GetItemTemplet() 
        )
		return false;

	(*m_pSlotBeforeDragging)->ResetPos();

	OnRegisterMyItem( pDragItem->GetUID() );

	return true;

}


void CX2UIPersonalTrade::SetPosition(D3DXVECTOR2 vec)
{
	m_pDLGPersonalTrade->SetPos(vec);
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

void CX2UIPersonalTrade::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	if(m_pDLGPersonalTrade != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGPersonalTrade, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGPersonalTrade, false );
	}
		
}

void CX2UIPersonalTrade::SetOpponentNickName(wstring val)
{
	m_wstrOpponentNickName = val;
	if(m_pDLGPersonalTrade != NULL)
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGPersonalTrade->GetControl(L"Static_Opponent_Name");
		if ( pStatic != NULL)
		{
			pStatic->GetString(0)->msg = m_wstrOpponentNickName;
		}
	}
	
}

void CX2UIPersonalTrade::OpenPersonalTradeWindow()
{	
	RegisterLuaBind();

	SAFE_DELETE_DIALOG(m_pDLGPersonalTrade);
	m_pDLGPersonalTrade = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_Trade.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPersonalTrade );
	
	// ���� PersonalTrade�ڵ忡�� OnStartPersonalTrade �κ�
	//////////////////////////////////////////////////////////////////////////

	// Trade â���� :
	// ED Static / IME �ʱ�ȭ
	CKTDGUIStatic* pStatic_MyED = (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_MyED" );
	pStatic_MyED->GetString(0)->msg = L"0";
	CKTDGUIIMEEditBox* pIME_Myed = (CKTDGUIIMEEditBox*) m_pDLGPersonalTrade->GetControl( L"IME_Edit_MyED" );
	pIME_Myed->SetText(L"0");
#ifdef NUMBER_TO_LANGUAGE
	// �ְ� ����(��) �̻� �Էµ��� �ʵ��� ���� ���� ����
	pIME_Myed->SetLengthLimit_LUA( m_iChangingEDLimit );
#endif NUMBER_TO_LANGUAGE
	CKTDGUIStatic* pStatic_OpponenetED = (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_OpponentED" );
	pStatic_OpponenetED->GetString(0)->msg = L"0";

	// �̸� ���Ͷ� ��
	CKTDGUIStatic* pStatic_OpponenetName = (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Opponent_Name" );
	pStatic_OpponenetName->GetString(0)->msg = m_wstrOpponentNickName;

#ifdef NUMBER_TO_LANGUAGE
	m_pTalkBoxMgr = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxMgr );
	m_pTalkBoxMgr->SetOverUI( true );
#endif NUMBER_TO_LANGUAGE

	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2SlotItem* pSlot = (CX2SlotItem*)m_SlotList[i];
		pSlot->SetShow(true);
		pSlot->SetEnable(true);
	}

	SetMyTradeButton( TS_BEFORE_REGISTER );
	SetOpponentTradeButton( TS_BEFORE_REGISTER );

	// ���� ���� �� �ְ�
	m_vMyItemSlot.clear();
	m_vOpponentItemSlot.clear();

	// ���� UI �������ְ�
	ResetMyTradeSlot();
	ResetOpponentTradeSlot();


	SetMyTradeED( 0 );
	SetOpponentTradeED( 0 );


	if( NULL != m_pDLGMsgBox )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
		m_pDLGMsgBox = NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	m_bShow = true;
	m_bTrading = true;

	D3DXVECTOR3 tmp;
	tmp = m_pDLGPersonalTrade->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGPersonalTrade->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;
		
	g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_PERSONAL_TRADE);

	if( g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true )
	{
		g_pData->GetUIManager()->CloseAllNPCDlg();
		g_pTFieldGame->ByeNpc();
	}

	// �κ��丮�� ���� ����
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
	g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
#ifdef SERV_SOCKET_NEW
	// oasis907 : ����� [2010.4.6] // ���� UI�� ���������� �ݴ´�.
	if( g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL)
	{
		g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->SetShow(false);
	}
#endif SERV_SOCKET_NEW

}

void CX2UIPersonalTrade::ClosePersonalTradeWindow()
{
	// ���� Control ��Ȱ��ȭ

	
	// OnEndPersonalTrade

	//////////////////////////////////////////////////////////////////////////
	m_bTrading = false;

	// ���� ����
	InvalidSelectedItem();
	InvalidSlotDesc();

	for(UINT i=0; i<m_SlotList.size(); i++)
	{
		CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
		if( NULL == pSlot )
			continue;

		pSlot->DestroyItemUI();
		pSlot->SetShow(false);
		pSlot->SetEnable(false);
	}

	

	m_vMyItemSlot.clear();
	m_vOpponentItemSlot.clear();

	m_iTradeUID		= 0;
	m_iOpponentUID	= 0;

	m_bOpponentRegisteredTrade = false;
	m_bRegisteredTrade = false;
	m_bOpponentApprovedTrade = false;

	if( NULL != m_pDLGMsgBox )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
		m_pDLGMsgBox = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	
	m_bShow = false;

	//m_pPersonalTrade->OnEndPersonalTrade();
	// �ϴ��� �ݱ⸸ ����

	if( NULL != m_pDLGPersonalTrade )
	{
#ifdef NUMBER_TO_LANGUAGE
		m_pTalkBoxMgr->SetShowObject( false, false );
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pTalkBoxMgr );
		m_pTalkBoxMgr->SetOverUI( false );
#endif NUMBER_TO_LANGUAGE
		m_pDLGPersonalTrade->SetShowEnable(false, false);
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPersonalTrade, NULL, false );
		m_pDLGPersonalTrade = NULL;
	}

	g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_PERSONAL_TRADE);
	g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
}

void CX2UIPersonalTrade::SetMyTradeED( int iED )
{
	m_iMyED = iED;

	// fix!! �̺κ� onstartpersonalTrade()�� �Űܾ� �ɱ�?
	if( NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		//CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
		CKTDGUIStatic* pStatic_ED = (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_MyED" );	
		if( NULL != pStatic_ED )
		{
			pStatic_ED->GetString(0)->msg = g_pMain->GetEDString( iED );

			//{{ kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����
#ifdef	PRICE_COLOR
			pStatic_ED->SetColor( g_pMain->GetPriceColor( iED ) );
#endif	PRICE_COLOR
			//}} kimhc // 2009-11-10 // ED �Է� �� �� �ٲ�� ����

		}
		CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDLGPersonalTrade->GetControl( L"IME_Edit_MyED" );	
		if( NULL != pIMEEditBox )
		{
 			wstring wstrED = g_pMain->GetEDString( iED );
 			pIMEEditBox->SetText( wstrED.c_str() );
		}

#ifdef NUMBER_TO_LANGUAGE
		if (pIMEEditBox->GetTextLength() == 0)
		{
			iED = 0;
		}

		D3DXVECTOR2 vPos = m_pDLGPersonalTrade->GetPos() + D3DXVECTOR2(195, 70);
		PopPersonalTradeTalkBox( m_iTradeUID, iED, vPos, D3DXCOLOR(0, 0, 0, 1), g_pMain->GetPriceColor( iED ), g_pMain->GetPriceColor( iED ) );
#endif NUMBER_TO_LANGUAGE
	}
}

void CX2UIPersonalTrade::SetOpponentTradeED( int iED )
{
	m_iOpponentED = iED;

	CKTDGUIStatic* pOpponentED = (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_OpponentED" );
	wstring wstrED = g_pMain->GetEDString( m_iOpponentED );
	//{{ ����� : [2009/11/5] //	���� �׽�Ʈ
#ifdef PRICE_COLOR
	pOpponentED->SetColor( g_pMain->GetPriceColor( m_iOpponentED ) );
#endif	//	PRICE_COLOR	
	//}} ����� : [2009/11/5] //	���� �׽�Ʈ
	pOpponentED->GetString(0)->msg = wstrED.c_str();

#ifdef NUMBER_TO_LANGUAGE
	if (wstrED.length() == 0)
	{
		iED = 0;
	}
	
	D3DXVECTOR2 vPos = m_pDLGPersonalTrade->GetPos() + D3DXVECTOR2(195, 315);
	PopPersonalTradeTalkBox( m_iOpponentUID, iED, vPos, D3DXCOLOR(0, 0, 0, 1), g_pMain->GetPriceColor( iED ), g_pMain->GetPriceColor( iED ) );
#endif NUMBER_TO_LANGUAGE
}

void CX2UIPersonalTrade::SetMyTradeButton( TRADE_STEP eTradeStep )
{
	m_eNowMyTradeButtonStep = eTradeStep;


	CKTDGUIButton* pButton_Register			= (CKTDGUIButton*) m_pDLGPersonalTrade->GetControl( L"Button_Trade_Register" );
	CKTDGUIButton* pButton_Accept			= (CKTDGUIButton*) m_pDLGPersonalTrade->GetControl( L"Button_Trade_Accept" );
	CKTDGUIButton* pButton_Cancel			= (CKTDGUIButton*) m_pDLGPersonalTrade->GetControl( L"Button_Trade_Cancle" );

	CKTDGUIIMEEditBox* pIMEEditBox_ED		= (CKTDGUIIMEEditBox*) m_pDLGPersonalTrade->GetControl( L"IME_Edit_MyED" );
	CKTDGUIStatic* pStatic_MyED = (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_MyED" );

	CKTDGUIStatic* pStatic_Yellow_Register	= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_My_Register" );
	CKTDGUIStatic* pStatic_Yellow_Approve	= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_My_Accept" );

	CKTDGUIStatic* pStatic_My_Arrow			= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_My_Arrow" );
	CKTDGUIStatic* pStatic_Other_Arrow		= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_Opponent_Arrow" );
	
	if( true == m_bOpponentRegisteredTrade && true == m_bRegisteredTrade )
	{
		pStatic_My_Arrow->GetPicture(0)->SetShow( false );
		pStatic_My_Arrow->GetPicture(1)->SetShow( true );

		pStatic_Other_Arrow->GetPicture(0)->SetShow( false );
		pStatic_Other_Arrow->GetPicture(1)->SetShow( true );
	}
	else
	{
		pStatic_My_Arrow->GetPicture(0)->SetShow( true );
		pStatic_My_Arrow->GetPicture(1)->SetShow( false );

		pStatic_Other_Arrow->GetPicture(0)->SetShow( true );
		pStatic_Other_Arrow->GetPicture(1)->SetShow( false );
	}

	switch( eTradeStep )
	{
	case TS_BEFORE_REGISTER:
		{
			pButton_Register->SetShowEnable( true, true );
			pStatic_Yellow_Register->SetShowEnable( false, false );

			pButton_Accept->SetShowEnable( true, false );
			pStatic_Yellow_Approve->SetShowEnable( false, false );

			pButton_Cancel->SetShowEnable( true, true );

			pIMEEditBox_ED->SetShowEnable( true, true );
			pStatic_MyED->SetShow(false);

//			pMyTradeItemSlot->SetClickable( false );

		} break;

	case TS_AFTER_REGISTER:
		{
			pButton_Register->SetShowEnable( false, false );
			pStatic_Yellow_Register->SetShowEnable( true, true );

			pButton_Accept->SetShowEnable( true, false );
			pStatic_Yellow_Approve->SetShowEnable( false, false );
			pButton_Cancel->SetShowEnable( true, true );

// 			CKTDGUIControl* pStaticMy_TradeBox_10Goods = m_pDLGPersonalTrade->GetControl( L"g_pStaticMy_TradeBox_10Goods" );
// 			if ( pStaticMy_TradeBox_10Goods != NULL )
// 			{
// 				pStaticMy_TradeBox_10Goods->SetShowEnable( false, false );
// 			}

			pIMEEditBox_ED->SetShowEnable( false, false );
			pStatic_MyED->SetShow(true);

//			pMyTradeItemSlot->SetClickable( false );


		} break;

	case TS_BEFORE_APPROVE:
		{
			pButton_Register->SetShowEnable( false, false );
			pStatic_Yellow_Register->SetShowEnable( true, true );

			pButton_Accept->SetShowEnable( true, true );
			pStatic_Yellow_Approve->SetShowEnable( false, false );
			pButton_Cancel->SetShowEnable( true, true );


// 			CKTDGUIControl* pStaticMy_TradeBox_10Goods = m_pDLGPersonalTrade->GetControl( L"g_pStaticMy_TradeBox_10Goods" );
// 			if ( pStaticMy_TradeBox_10Goods != NULL )
// 			{
// 				pStaticMy_TradeBox_10Goods->SetShowEnable( false, false );
// 			}


			pIMEEditBox_ED->SetShowEnable( false, false );
			pStatic_MyED->SetShow(true);

//			pMyTradeItemSlot->SetClickable( false );

		} break;

	case TS_AFTER_APPROVE:
		{
			pButton_Register->SetShowEnable( false, false );
			pStatic_Yellow_Register->SetShowEnable( true, true );

			pButton_Accept->SetShowEnable( false, false );
			pStatic_Yellow_Approve->SetShowEnable( true, true );

			pButton_Cancel->SetShowEnable( false, false );

// 			CKTDGUIControl* pStaticMy_TradeBox_10Goods = m_pDLGPersonalTrade->GetControl( L"g_pStaticMy_TradeBox_10Goods" );
// 			if ( pStaticMy_TradeBox_10Goods != NULL )
// 			{
// 				pStaticMy_TradeBox_10Goods->SetShowEnable( false, false );
// 			}

			pIMEEditBox_ED->SetShowEnable( false, false );
			pStatic_MyED->SetShow(true);

//			pMyTradeItemSlot->SetClickable( false );

		} break;
	}
}


void CX2UIPersonalTrade::SetOpponentTradeButton( TRADE_STEP eTradeStep )
{

	m_eNowOpponentTradeButtonStep = eTradeStep;

	CKTDGUIButton* pButton_Register			= (CKTDGUIButton*) m_pDLGPersonalTrade->GetControl( L"Button_Opponent_Trade_Register" );
	CKTDGUIButton* pButton_Accept			= (CKTDGUIButton*) m_pDLGPersonalTrade->GetControl( L"Button_Opponent_Trade_Accept" );

	CKTDGUIStatic* pStatic_Yellow_Register	= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_Opponent_Register" );
	CKTDGUIStatic* pStatic_Yellow_Approve	= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_Opponent_Accept" );

	CKTDGUIStatic* pStatic_My_Arrow			= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_My_Arrow" );
	CKTDGUIStatic* pStatic_Other_Arrow		= (CKTDGUIStatic*) m_pDLGPersonalTrade->GetControl( L"Static_Trade_Opponent_Arrow" );

	if( true == m_bOpponentRegisteredTrade && true == m_bRegisteredTrade )
	{
		pStatic_My_Arrow->GetPicture(0)->SetShow( false );
		pStatic_My_Arrow->GetPicture(1)->SetShow( true );

		pStatic_Other_Arrow->GetPicture(0)->SetShow( false );
		pStatic_Other_Arrow->GetPicture(1)->SetShow( true );
	}
	else
	{
		pStatic_My_Arrow->GetPicture(0)->SetShow( true );
		pStatic_My_Arrow->GetPicture(1)->SetShow( false );

		pStatic_Other_Arrow->GetPicture(0)->SetShow( true );
		pStatic_Other_Arrow->GetPicture(1)->SetShow( false );
	}


	switch( eTradeStep )
	{
	case TS_BEFORE_REGISTER:
		{
			pButton_Register->SetShowEnable( true, false );
			pStatic_Yellow_Register->SetShowEnable( false, false );

			pButton_Accept->SetShowEnable( true, false );
			pStatic_Yellow_Approve->SetShowEnable( false, false );

		} break;

	case TS_AFTER_REGISTER:
		{
			pButton_Register->SetShowEnable( false, false );
			pStatic_Yellow_Register->SetShowEnable( true, true );

			pButton_Accept->SetShowEnable( true, false );
			pStatic_Yellow_Approve->SetShowEnable( false, false );
		} break;

	case TS_BEFORE_APPROVE:
		{
			pButton_Register->SetShowEnable( false, false );
			pStatic_Yellow_Register->SetShowEnable( true, true );

			pButton_Accept->SetShowEnable( true, false );
			pStatic_Yellow_Approve->SetShowEnable( false, false );

		} break;

	case TS_AFTER_APPROVE:
		{
			pButton_Register->SetShowEnable( false, false );
			pStatic_Yellow_Register->SetShowEnable( true, true );

			pButton_Accept->SetShowEnable( false, false );
			pStatic_Yellow_Approve->SetShowEnable( true, true );

		} break;
	}
}

void CX2UIPersonalTrade::SetTradeButtonRegisterState( const bool bTempRegistered, const bool bTempOpponentRegistered )
{
	if( bTempRegistered != m_bRegisteredTrade )
	{
		m_bRegisteredTrade = bTempRegistered;
		if( true == m_bRegisteredTrade )
		{
			SetMyTradeButton( TS_AFTER_REGISTER );
		}
		else
		{
			SetMyTradeButton( TS_BEFORE_REGISTER );
		}
	}

	if( bTempOpponentRegistered != m_bOpponentRegisteredTrade )
	{
		m_bOpponentRegisteredTrade = bTempOpponentRegistered;
		if( true == m_bRegisteredTrade )
		{
			SetOpponentTradeButton( TS_AFTER_REGISTER );
		}
		else
		{
			SetOpponentTradeButton( TS_BEFORE_REGISTER );
		}
	}
}




bool CX2UIPersonalTrade::Handler_EGS_REQUEST_TRADE_REQ( UidType iUnitUID )
{
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_SQUARE_GAME:
		{

			CX2StateSquareGame* pStateSquareGame = (CX2StateSquareGame*)g_pMain->GetNowState();
			if ( pStateSquareGame != NULL )
			{
				CX2UIPersonalShop* pPersonalShop = g_pData->GetUIManager()->GetUIPersonalShop();
				if ( pPersonalShop != NULL )
				{
					if ( pPersonalShop->GetPersonalShopState() != CX2UIPersonalShop::XPSS_CLOSE )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_877 ), g_pMain->GetNowState() );
						return true;
					}
				}

				if ( g_pSquareGame != NULL )
				{
					CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( iUnitUID );
					if ( pSquareUnit != NULL )
					{
						if ( pSquareUnit->GetPersonalShopState() != CX2SquareUnit::PSS_NONE )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_877 ), g_pMain->GetNowState() );
							return true;
						}
					}
				}
				if( true == m_bTrading )		// �ŷ����̸�
				{
					return true;
				}
			}
		} break;

	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_ROOM:
		{
			if( true == m_bTrading )
			{
				return true;
			}

		} break;
	default:
		return true;
	}
	KEGS_REQUEST_TRADE_REQ kPacket;
	kPacket.m_iUnitUID = iUnitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_TRADE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_REQUEST_TRADE_ACK );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_REQUEST_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_TRADE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_REQUEST_TRADE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_VILLAGE_MAP:
			case CX2Main::XS_SQUARE_GAME:
				{
					wstring wstrNickName = L"";
					// �ϴ� Ŀ�´�Ƽ â���� �˻��� �õ��Ѵ�.
					if(g_pData->GetMessenger()->GetNickNameByUID(wstrNickName, kEvent.m_iUnitUID))
					{
						// �˻� ����������
						m_pTradeInviting = new TradeInviting( wstrNickName, 10.f, g_pMain->GetNowState() );
#ifdef ADJUST_THIRD_PERSON_BUG
						m_bTradeRequesting = true;
#endif //ADJUST_PERSONAL_TRADE_BUG
					}
					else
					{
						// �˻��� ����. Square Unit���� ã�´�.
						CX2SquareUnit* pSquareUnit = NULL;
						if(g_pTFieldGame != NULL)
						{
							pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( kEvent.m_iUnitUID );

						}
						else if(g_pSquareGame != NULL)
						{
							pSquareUnit = g_pSquareGame->GetSquareUnitByUID( kEvent.m_iUnitUID );
						}	
						else
						{
							// �ŷ�����ҵǾ����ϴ� ǥ��.
							g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_878 ), g_pMain->GetNowState() );
							return true;
						}
						
						if( NULL != pSquareUnit )
						{
							m_pTradeInviting = new TradeInviting( pSquareUnit->GetNickName(), 10.f, g_pMain->GetNowState() );
#ifdef ADJUST_THIRD_PERSON_BUG
							m_bTradeRequesting = true;
#endif //ADJUST_PERSONAL_TRADE_BUG
						}
						else
						{
							// �ŷ��� ������ ã������
							g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4852 ), g_pMain->GetNowState() );
						}
					}				
					// m_pTradeInviting = new TradeInviting( kEvent.m_wstrNickName, 10.f, g_pMain->GetNowState() );
				
				} break;
			case CX2Main::XS_BATTLE_FIELD:
				{
					wstring wstrNickName = L"";
					// �ϴ� Ŀ�´�Ƽ â���� �˻��� �õ��Ѵ�.
					if(g_pData->GetMessenger()->GetNickNameByUID(wstrNickName, kEvent.m_iUnitUID))
					{
						// �˻� ����������
						m_pTradeInviting = new TradeInviting( wstrNickName, 10.f, g_pMain->GetNowState() );
#ifdef ADJUST_THIRD_PERSON_BUG
						m_bTradeRequesting = true;
#endif //ADJUST_PERSONAL_TRADE_BUG

					}
					else
					{
						if( NULL != g_pX2Game )
						{
							CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID(kEvent.m_iUnitUID);
							if( NULL != pGUUser )
							{
								m_pTradeInviting = new TradeInviting( pGUUser->GetUnitName(), 10.f, g_pMain->GetNowState() );
#ifdef ADJUST_THIRD_PERSON_BUG
								m_bTradeRequesting = true;
#endif //ADJUST_PERSONAL_TRADE_BUG
								return true;
							}
						}

						// �ŷ��� ������ ã������
						g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4852 ), g_pMain->GetNowState() );
					}			
				}
				break;

			case CX2Main::XS_PVP_ROOM:
			case CX2Main::XS_DUNGEON_ROOM:
				{
					CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
					if( NULL != pSlotData && NULL != pSlotData->m_pUnit )
					{
						wstring wstrNickName = pSlotData->m_pUnit->GetNickName();
						m_pTradeInviting = new TradeInviting( wstrNickName, 10.f, g_pMain->GetNowState() );
#ifdef ADJUST_THIRD_PERSON_BUG
						m_bTradeRequesting = true;
#endif //ADJUST_PERSONAL_TRADE_BUG
					}
					else
					{
						// ���� �˾�
						g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_878 ), g_pMain->GetNowState() );
					}
				} break;
			}

			return true;

		}
//		netError�� ó��
//		else
//		{
//			// �ŷ�����
//			g_pMain->KTDGUIOKMsgBox(D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_4852 ), g_pMain->GetNowState() );
//		}
	}

	return false;
}


bool CX2UIPersonalTrade::Handler_EGS_REQUEST_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ü�� ���̵��̸� �ŷ� �ȵ�
	if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
		return true;

#ifdef ADJUST_THIRD_PERSON_BUG
	if( true == m_bTradeRequesting )
		return true;
#endif //ADJUST_PERSONAL_TRADE_BUG

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
#if 0 
			CX2SquareUnit* pSquareUnit;
			if(g_pTFieldGame != NULL)
				pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( kEvent.m_iSenderUID );
			else if(g_pSquareGame != NULL)
				pSquareUnit = g_pSquareGame->GetSquareUnitByUID( kEvent.m_iSenderUID);
			else
				return false;
			//CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( kEvent.m_iSenderUID );
#endif
			//if( NULL != pSquareUnit )
			//{
			for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
			{
				TradeInvited* pTradeInvited = m_vecTradeInvited[i];
				if( pTradeInvited->m_iInviterUID == kEvent.m_iSenderUID )
				{
					return true;
				}
			}
				
			if( g_pData->GetCashShop()->GetOpen() == false )
			{
				TradeInvited* pTradeInvited = new TradeInvited( kEvent.m_iSenderUID, kEvent.m_wstrSenderNickName, 
					10.f, UPTCM_TRADE_INVITE_ACCEPT, UPTCM_TRADE_INVITE_REFUSE, g_pMain->GetNowState() );

				m_vecTradeInvited.push_back( pTradeInvited );
			}
			
			//}

		} break;

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iSenderUID );
			if( NULL != pSlotData && NULL != pSlotData->m_pUnit )
			{
				for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
				{
					TradeInvited* pTradeInvited = m_vecTradeInvited[i];
					if( pTradeInvited->m_iInviterUID == kEvent.m_iSenderUID )
					{
						return true;
					}
				}

				if( g_pData->GetCashShop()->GetOpen() == false )
				{
					TradeInvited* pTradeInvited = new TradeInvited( kEvent.m_iSenderUID, kEvent.m_wstrSenderNickName, 
						10.f, UPTCM_TRADE_INVITE_ACCEPT, UPTCM_TRADE_INVITE_REFUSE, g_pMain->GetNowState() );

					m_vecTradeInvited.push_back( pTradeInvited );
				}				
			}
		} break;
	}

	return true;
}


bool CX2UIPersonalTrade::Handler_EGS_REQUEST_TRADE_REPLY_NOT( UidType iUnitUID, bool bAccept )
{
	KEGS_REQUEST_TRADE_REPLY_NOT kPacket;
	kPacket.m_iReceiverUID = iUnitUID;
	kPacket.m_bAccept = bAccept;

	g_pData->GetServerProtocol()->SendPacket( EGS_REQUEST_TRADE_REPLY_NOT, kPacket );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_REQUEST_TRADE_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REQUEST_TRADE_DENY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE( m_pTradeInviting );
#ifdef ADJUST_THIRD_PERSON_BUG
	m_bTradeRequesting = false;
#endif //ADJUST_PERSONAL_TRADE_BUG
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
#if 0 
			CX2SquareUnit* pSquareUnit;
			if(g_pTFieldGame != NULL)
				pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( kEvent.m_iUnitUID );
			else if(g_pSquareGame != NULL)
				pSquareUnit = g_pSquareGame->GetSquareUnitByUID( kEvent.m_iUnitUID );
			else
				return false;
				// CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( kEvent.m_iUnitUID );
#endif
			//if( NULL != pSquareUnit )
			//{

			if( kEvent.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_01 )
			{
				wstringstream wstrstm;
				//wstrstm << pSquareUnit->GetNickName().c_str() << L"�Բ���" << NetError::GetErrStr(NetError::NOT_REQUEST_TRADE_REPLY_01);
				wstrstm << NetError::GetErrStrF(NetError::NOT_REQUEST_TRADE_REPLY_01);
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), g_pMain->GetNowState() );
			}
			else if( kEvent.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_02 )
			{
				wstringstream wstrstm;
				//wstrstm << pSquareUnit->GetNickName().c_str() << L"�Բ���" << NetError::GetErrStr(NetError::NOT_REQUEST_TRADE_REPLY_02);
				wstrstm << NetError::GetErrStrF(NetError::NOT_REQUEST_TRADE_REPLY_02);
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), g_pMain->GetNowState() );
			}
#ifdef SERV_CHANGE_MSG_BLOCK_TRADE_USER
			else if( kEvent.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_05 )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_28412 ), g_pMain->GetNowState() );
			}
#endif //SERV_CHANGE_MSG_BLOCK_TRADE_USER			

			//}

		} break;

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
			if( NULL != pSlotData && NULL != pSlotData->m_pUnit )
			{
				if( kEvent.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_01 )
				{
					wstring wstrMsg;
					wstrMsg = GET_REPLACED_STRING( ( STR_ID_879, "SS", pSlotData->m_pUnit->GetNickName(), NetError::GetErrStrF( NetError::NOT_REQUEST_TRADE_REPLY_01 ) ) );
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMsg.c_str(), g_pMain->GetNowState() );
				}
				else if( kEvent.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_02 )
				{
					wstring wstrMsg;
					wstrMsg = GET_REPLACED_STRING( ( STR_ID_879, "SS", pSlotData->m_pUnit->GetNickName(), NetError::GetErrStrF( NetError::NOT_REQUEST_TRADE_REPLY_02 ) ) );
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrMsg.c_str(), g_pMain->GetNowState() );
				}
#ifdef SERV_CHANGE_MSG_BLOCK_TRADE_USER
				else if( kEvent.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_05 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_28412 ), g_pMain->GetNowState() );
				}
#endif //SERV_CHANGE_MSG_BLOCK_TRADE_USER

			}
		} break;
	}


	return true;
}


//

bool CX2UIPersonalTrade::Handler_EGS_CREATE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_iTradeUID = kEvent.m_iTradeUID;

	SAFE_DELETE(m_pTradeInviting);
#ifdef ADJUST_THIRD_PERSON_BUG
	m_bTradeRequesting = false;
#endif //ADJUST_PERSONAL_TRADE_BUG
	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_BREAK_TRADE_REQ( int iReason )
{
	KEGS_BREAK_TRADE_REQ kPacket;
	kPacket.m_iReason = iReason;

	g_pData->GetServerProtocol()->SendPacket( EGS_BREAK_TRADE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BREAK_TRADE_ACK );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_BREAK_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BREAK_TRADE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BREAK_TRADE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			ClosePersonalTradeWindow();
			return true;
		}
	}

	return false;
}

bool CX2UIPersonalTrade::Handler_EGS_BREAK_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BREAK_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	// �ŷ��� �ߴܵ� ���� msgbox
	g_pMain->IsValidPacket( kEvent.m_iReason );

	ClosePersonalTradeWindow();

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_TRADE_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TRADE_USER_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//m_iGSUID;
	for( UINT i=0; i<kEvent.m_vecTradeUserInfo.size(); i++ )
	{
		KTradeUserInfo& kTradeUserInfo = kEvent.m_vecTradeUserInfo[i];
		if( kTradeUserInfo.m_iUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
		{
			SetOpponentNickName( kTradeUserInfo.m_wstrNickName );
			m_iOpponentUID = kTradeUserInfo.m_iUnitUID;
			break;
		}
	}


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_BATTLE_FIELD:
		OpenPersonalTradeWindow();
		break;
	default:
		break;

	}

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_JOIN_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//m_iGSUID;
	SetOpponentNickName( kEvent.m_kTradeUserInfo.m_wstrNickName );
	m_iOpponentUID = kEvent.m_kTradeUserInfo.m_iUnitUID;


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_BATTLE_FIELD:
		OpenPersonalTradeWindow();
		break;
	default:
		break;
	}

	return true;
}


bool CX2UIPersonalTrade::Handler_EGS_UPDATE_TRADE_ITEM_REQ( UidType iItemUID, int iQuantity, bool bDelete )
{
	KEGS_UPDATE_TRADE_ITEM_REQ kPacket;
	if( iItemUID < 0 )
	{
		return false;
	}

	kPacket.m_iItemUID	= iItemUID;
	kPacket.m_iQuantity = iQuantity;
	kPacket.m_bDelete = bDelete;

	g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_TRADE_ITEM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_UPDATE_TRADE_ITEM_ACK );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_UPDATE_TRADE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_TRADE_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	//////////////////////////////////////////////////////////////////////////

	if( g_pMain->DeleteServerPacket( EGS_UPDATE_TRADE_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// ������ Update_TRADE_ITEM_NOT ���ƿ��ϱ�..		
			return true;
		}
	}

	return false;
}

bool CX2UIPersonalTrade::Handler_EGS_UPDATE_TRADE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_TRADE_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// ���� ������
	if( m_iOpponentUID == kEvent.m_iUnitUID )
	{
		if( false == kEvent.m_bDelete )
		{
			//////////////////////////////////////////////////////////////////////////

			// ������ �߰�!
			// �� ������ �ִ´� 
			m_vOpponentItemSlot.push_back(std::make_pair(kEvent.m_iItemUID, kEvent.m_kItemInfo));

		}
		else
		{
			// ���� �������� ����
			// �ش��ϴ� �������� �����ش�
			for(vector<std::pair<UidType, KItemInfo> >::iterator it = m_vOpponentItemSlot.begin(); it != m_vOpponentItemSlot.end(); it++)
			{
				UidType Uid = it->first;
				if(kEvent.m_iItemUID == Uid)
				{
					m_vOpponentItemSlot.erase(it);
					break;
				}
			}
		}
		ResetOpponentTradeSlot();

		const bool bTempRegistered = kEvent.m_mapOfferState[ g_pData->GetMyUser()->GetSelectUnit()->GetUID() ];
		const bool bTempOpponentRegistered = kEvent.m_mapOfferState[ m_iOpponentUID ];

		SetTradeButtonRegisterState( bTempRegistered, bTempOpponentRegistered );

		return true;
	}

	// �� ������
	if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
	{
		if( false == kEvent.m_bDelete )
		{
			// �� �������� �߰�!
			// ���� ������ ������ Vecter�� �������
			m_vMyItemSlot.push_back(std::make_pair(kEvent.m_iItemUID, kEvent.m_kItemInfo.m_iQuantity));
			
		}
		else
		{
			// �� �������� ����
			
			// ã�������
			for(vector< std::pair<UidType, int> >::iterator i = m_vMyItemSlot.begin(); i != m_vMyItemSlot.end(); i++)
			{
				if(kEvent.m_iItemUID == i->first)
				{
					m_vMyItemSlot.erase(i);
					break;
				}
			}
			
		}
		ResetMyTradeSlot();


		const bool bTempRegistered = kEvent.m_mapOfferState[ g_pData->GetMyUser()->GetSelectUnit()->GetUID() ];
		const bool bTempOpponentRegistered = kEvent.m_mapOfferState[ m_iOpponentUID ];

		SetTradeButtonRegisterState( bTempRegistered, bTempOpponentRegistered );

		return true;
	}


	return false;
}


bool CX2UIPersonalTrade::Handler_EGS_UPDATE_TRADE_ED_REQ( int iED )
{
	if ( m_iMyED == iED )
		return true;

	KEGS_UPDATE_TRADE_ED_REQ kPacket;
	kPacket.m_iED		= iED;

	g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_TRADE_ED_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_UPDATE_TRADE_ED_ACK );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_UPDATE_TRADE_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_TRADE_ED_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_UPDATE_TRADE_ED_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
		else
		{
			SetMyTradeED( m_iMyED );
			SetTradeButtonRegisterState( m_bRegisteredTrade, m_bOpponentRegisteredTrade );
		}

		return true;
	}


	return false;
}

bool CX2UIPersonalTrade::Handler_EGS_UPDATE_TRADE_ED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_TRADE_ED_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_iOpponentUID == kEvent.m_iUnitUID )
	{
		SetOpponentTradeED( kEvent.m_iED );

		const bool bTempRegistered = kEvent.m_mapOfferState[ g_pData->GetMyUser()->GetSelectUnit()->GetUID() ];
		const bool bTempOpponentRegistered = kEvent.m_mapOfferState[ m_iOpponentUID ];

		SetTradeButtonRegisterState( bTempRegistered, bTempOpponentRegistered );


		return true;
	}

	if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUnitUID )
	{
		SetMyTradeED( kEvent.m_iED );

		const bool bTempRegistered = kEvent.m_mapOfferState[ g_pData->GetMyUser()->GetSelectUnit()->GetUID() ];
		const bool bTempOpponentRegistered = kEvent.m_mapOfferState[ m_iOpponentUID ];

		SetTradeButtonRegisterState( bTempRegistered, bTempOpponentRegistered );

		return true;
	}

	return false;
}



bool CX2UIPersonalTrade::Handler_EGS_OFFER_TRADE_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_OFFER_TRADE_REQ );
	g_pMain->AddServerPacket( EGS_OFFER_TRADE_ACK );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_OFFER_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_OFFER_TRADE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_OFFER_TRADE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_bRegisteredTrade = true;
			// ��� ��ư�� �������� �߰����� ����� ���´�.

			if( true == m_bOpponentRegisteredTrade )
			{
				SetMyTradeButton( TS_BEFORE_APPROVE );
				SetOpponentTradeButton( TS_BEFORE_APPROVE );
			}
			else
			{
				SetMyTradeButton( TS_AFTER_REGISTER );
			}
			return true;
		}
	}

	return false;
}

bool CX2UIPersonalTrade::Handler_EGS_OFFER_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_OFFER_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_iOpponentUID == (UidType)kEvent )
	{
		m_bOpponentRegisteredTrade = true;

		if( true == m_bRegisteredTrade )
		{
			SetMyTradeButton( TS_BEFORE_APPROVE );
			SetOpponentTradeButton( TS_BEFORE_APPROVE );
		}
		else
		{
			SetOpponentTradeButton( TS_AFTER_REGISTER );
		}
		return true;
	}

	return false;
}

bool CX2UIPersonalTrade::Handler_EGS_APPROVE_TRADE_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_APPROVE_TRADE_REQ );
	g_pMain->AddServerPacket( EGS_APPROVE_TRADE_ACK );

	return true;
}

bool CX2UIPersonalTrade::Handler_EGS_APPROVE_TRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_APPROVE_TRADE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_APPROVE_TRADE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			SetMyTradeButton( TS_AFTER_APPROVE );

			return true;
		}
	}

	return false;
}


bool CX2UIPersonalTrade::Handler_EGS_APPROVE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_APPROVE_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_iOpponentUID == (UidType)kEvent )
	{
		m_bOpponentApprovedTrade = false;

		SetOpponentTradeButton( TS_AFTER_APPROVE );
		return true;
	}

	return false;
}


bool CX2UIPersonalTrade::Handler_EGS_TRADE_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TRADE_COMPLETE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;

	if(g_pData->GetUIManager()->GetUIInventory() != NULL)
	{		
		g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);
	}

	//g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventoryItemInfo );
	//SetInventorySort();

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_BATTLE_FIELD:
		{
			ClosePersonalTradeWindow();
		} break;
	}

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_880 ), (CKTDXStage*) g_pMain->GetNowState() );

	return true;
}

// �� �ڵ�� ������ ���� ���� �� ���̾�
void CX2UIPersonalTrade::OpenRegisgerQuantityDLG( D3DXVECTOR2 pos )
{

	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
	m_pDLGItemEnroll = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemEnroll );
// 	pos = pos - D3DXVECTOR2(75, 20);
// 	if(pos.x > 865 )
// 		pos.x = 865;
// 	if(pos.y > 633 )
// 		pos.y = 633;
// 	m_pDLGItemEnroll->SetPos( pos );
	m_pDLGItemEnroll->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(75,20)) );

	CKTDGUIStatic* pStaticPrice = (CKTDGUIStatic*) m_pDLGItemEnroll->GetControl( L"Static_Price" );
	CKTDGUIStatic* pStaticEDMark = (CKTDGUIStatic*) m_pDLGItemEnroll->GetControl( L"Static_EDMark" );
	pStaticPrice->SetShow(false);
	pStaticEDMark->SetShow(false);

	CKTDGUIButton* pButtonOK = (CKTDGUIButton*) m_pDLGItemEnroll->GetControl( L"Button_OK" );
	CKTDGUIButton* pButtonCancle = (CKTDGUIButton*) m_pDLGItemEnroll->GetControl( L"Button_Cancle" );
	CKTDGUIButton* pButtonPlus = (CKTDGUIButton*) m_pDLGItemEnroll->GetControl( L"Button_Plus" );
	CKTDGUIButton* pButtonMinus = (CKTDGUIButton*) m_pDLGItemEnroll->GetControl( L"Button_Minus" );
	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" );
	pQuantity->SetText(L"1", true);
	m_iItemEnrollNum = 1;


	pButtonOK->SetCustomMsgMouseUp(UPTCM_REGISTER_QUANTITY_CONFIRM);
	pButtonCancle->SetCustomMsgMouseUp(UPTCM_REGISTER_QUANTITY_CANCLE);
	pButtonPlus->SetCustomMsgMouseUp(UPTCM_REGISTER_QUANTITY_PLUS);
	pButtonMinus->SetCustomMsgMouseUp(UPTCM_REGISTER_QUANTITY_MINUS);
	pQuantity->SetCustomMsgChange(UPTCM_REGISTER_QUANTITY_UPDATE);
	pQuantity->SetCustomMsgEnter(UPTCM_REGISTER_QUANTITY_CONFIRM);
	m_pDLGItemEnroll->SetCloseCustomUIEventID(UPTCM_REGISTER_QUANTITY_CANCLE);

	pQuantity->RequestFocus();

}

void CX2UIPersonalTrade::UpdateRegisterQuantityDLG( bool bReadIME )
{
	ASSERT( NULL != m_pDLGItemEnroll );
	if( NULL == m_pDLGItemEnroll )
		return; 


	CKTDGUIIMEEditBox* pQuantity = (CKTDGUIIMEEditBox*) m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" );
	
	if(bReadIME)
	{
		m_iItemEnrollNum = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	
	
	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnrollItemUid );
	if ( m_iItemEnrollNum <= 0 )
	{
		if(bReadIME)				// �Է�â���� �޾ƿ� ���
			m_iItemEnrollNum = 0;
		else						// << �� ���� ��� : 0 �Ʒ��� max�� ��ȯ��Ų��.
			m_iItemEnrollNum = pItem->GetItemTemplet()->GetQuantity();	
	}
		
	if ( m_iItemEnrollNum >= pItem->GetItemData().m_Quantity )
		m_iItemEnrollNum = pItem->GetItemData().m_Quantity;

	WCHAR buff[256] = {0};
	_itow( m_iItemEnrollNum, buff, 10 );
	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );

}


wstring CX2UIPersonalTrade::GetSlotItemDesc()
{
	wstring itemDesc=  L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		int n = m_pNowOverItemSlot->GetSlotID();

		if ( n < PTSI_OPPONENT_SLOT_0 )
		{
			// �� ����
			// !m_bRegisteredTrade : ����� ���°� �ƴ� ���� ��Ŭ�� ������ ǥ���ϵ���
			itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID(), !m_bRegisteredTrade );	
			
			//itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID(), false);
			
		}
		else
		{
			// ���� ����
			// ������ UID�� �޾Ƽ�
			UidType TargetUid = m_pNowOverItemSlot->GetItemUID();
			
			for(vector< std::pair<UidType, KItemInfo> >::iterator it = m_vOpponentItemSlot.begin(); it != m_vOpponentItemSlot.end(); it++)
			{
				std::pair<UidType, KItemInfo> SlotInfo = *it;
				KItemInfo iteminfo = SlotInfo.second;
				if(TargetUid == SlotInfo.first)
				{
					CX2Item::ItemData kItemData( iteminfo );
					CX2Item* pItem = new CX2Item( kItemData, NULL );
					itemDesc = GetSlotItemDescByTID( pItem, kItemData.m_ItemID, false );
					SAFE_DELETE( pItem );
					break;
				}
			}
		}
	}

	return itemDesc;
}

std::wstring CX2UIPersonalTrade::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
	std::wstring slotItemDesc = L"";
	slotItemDesc += L"\n";
	slotItemDesc += GET_STRING( STR_ID_881 );

	return slotItemDesc;

}



bool CX2UIPersonalTrade::RegisterMyItem( UidType ItemUid, int iCount )
{
	// �������� ������ �����ϴ���-_- Ȯ�����ְ�
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();


	CX2Item* pItem = kInventory.GetItem( ItemUid );
	if(pItem == NULL) 
		return false;

	if( iCount <= 0 )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_882 ), (CKTDXStage*) g_pMain->GetNowState() );
		return false;
	}
	else if( iCount > pItem->GetItemData().m_Quantity )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_871 ), (CKTDXStage*) g_pMain->GetNowState() );
		return false;
	}

    const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
	//const CX2Item::ItemData* pItemData = &pItem->GetItemData();
	if( pItemTemplet == NULL )
		return false;

	// ��Ŷ�� ������
	return Handler_EGS_UPDATE_TRADE_ITEM_REQ( ItemUid, iCount, false );
}

void CX2UIPersonalTrade::ResetMyTradeSlot()
{
	for(UINT i=0; i<m_SlotList.size(); i++)
	{		
		CX2SlotItem* pSlotItem = static_cast<CX2SlotItem*>(m_SlotList[i]);
		if(pSlotItem->GetSlotID() < PTSI_OPPONENT_SLOT_0)		// ���� ���� 0������ ��ȣ�� �۴� : �� ����
		{
			int SlotIndex = pSlotItem->GetSlotID();
			// �ϴ� ����
			pSlotItem->DestroyItemUI();

			if( SlotIndex < (int)m_vMyItemSlot.size() )
			{
				// ���� ǥ�����ְ�...
				std::pair<UidType, int> MySlotInfo = m_vMyItemSlot[SlotIndex];
				UidType itemUID = MySlotInfo.first;
				int iCount = MySlotInfo.second;
				
				const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

				CX2Item* pItem = kInventory.GetItem( itemUID );
				if(pItem == NULL) 
					continue;

				pSlotItem->CreateItemUI( pItem, iCount );
				pSlotItem->SetItemUID(itemUID);

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
			}
		}
	}
	return;

}

void CX2UIPersonalTrade::ResetOpponentTradeSlot()
{
	for(UINT i=0; i<m_SlotList.size(); i++)
	{		
		CX2SlotItem* pSlotItem = static_cast<CX2SlotItem*>(m_SlotList[i]);
		if(pSlotItem->GetSlotID() >= PTSI_OPPONENT_SLOT_0)		// ���� ���� 0������ ��ȣ�� ũ�ų� ���� : ���� ����
		{
			int SlotIndex = pSlotItem->GetSlotID() - PTSI_OPPONENT_SLOT_0;
			// �ϴ� ����
			pSlotItem->DestroyItemUI();

			if( SlotIndex < (int)m_vOpponentItemSlot.size() )
			{
				// ���� ǥ�����ְ�...
				std::pair<UidType, KItemInfo> OpponentSlotInfo = m_vOpponentItemSlot[SlotIndex];
				UidType itemUID = OpponentSlotInfo.first;
				KItemInfo iteminfo = OpponentSlotInfo.second;
                pSlotItem->CreateItemUI( iteminfo.m_iItemID, iteminfo.m_iQuantity );
				// ���Կ� UID�� �ٿ� �д� : ���� �������� ������ ��� ���ؼ�
				pSlotItem->SetItemUID(itemUID);

				//{{ kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
#ifdef	SEAL_ITEM
				
				if ( iteminfo.IsSealedItem() == true )
				{
					pSlotItem->SetShowSealedImage( true );
				}
				else
				{
					pSlotItem->SetShowSealedImage( false );
				}
#endif	SEAL_ITEM
				//}} kimhc // 2009-09-02 // �Ǹ�â�� ���� �̹��� ���̵���
			}
		}
	}
	return;

}

bool CX2UIPersonalTrade::OnRegisterMyItem(UidType ItemUid )
{
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
	CX2Item* pItem = kInventory.GetItem( ItemUid );

	if ( pItem == NULL || 
        NULL == pItem->GetItemTemplet() 
        )
		return true;

	//{{ kimhc // 2009-09-02 // ���ε� ������ ��� �����ϵ���
#ifdef	SEAL_ITEM
	if ( pItem->GetItemTemplet()->GetVested() == true && pItem->GetItemData().m_bIsSealed == false )
#else	SEAL_ITEM
	if( true == pItem->GetItemTemplet()->GetVested() )
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-02 // ���ε� ������ ��� �����ϵ���
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_883 ), (CKTDXStage*) g_pMain->GetNowState() );
		return true;
	}

#ifdef ITEM_RECOVERY_TEST
	// ��ȭ ���� �˻� (���Ҵ��ΰ�?)
	if(true == pItem->IsDisabled())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_394 ), g_pMain->GetNowState() );
		return false; 
	}
#endif

	if( pItem->GetItemTemplet()->GetPeriodType() == CX2Item::PT_ENDURANCE && 
		pItem->GetItemData().m_Endurance != pItem->GetItemTemplet()->GetEndurance() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_884 ), (CKTDXStage*) g_pMain->GetNowState() );
		return true;
	}

	if( m_bRegisteredTrade == true )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_885 ), (CKTDXStage*) g_pMain->GetNowState() );
		return true;
	}

	// �̹� ��ϵ� ���������� Ȯ��
	for(vector< std::pair<UidType, int> >::iterator it = m_vMyItemSlot.begin(); it != m_vMyItemSlot.end(); it++)
	{
		if(pItem->GetUID() == it->first)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_886 ), (CKTDXStage*) g_pMain->GetNowState() );
			return true;
		}
	}

	// � ��ϵǾ��ֳ� ����
	
	if(m_vMyItemSlot.size() >= PERSONAL_TRADE_ITEM_SLOT_MAX)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_887 ), (CKTDXStage*) g_pMain->GetNowState() );
		return true;
	}

	// ��� �õ��ϴ� �������� ������ �����صд�
	m_EnrollItemUid = pItem->GetUID();
	m_iItemEnrollNum = 1;

	// ���������� �ƴ��� ���� �������̸�
	if( CX2Item::PT_QUANTITY == pItem->GetItemTemplet()->GetPeriodType() )
	{
		D3DXVECTOR2 Pos;
		Pos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		Pos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		OpenRegisgerQuantityDLG( Pos );
	}
	else		// �ƴϸ�
	{
		RegisterMyItem( m_EnrollItemUid, 1 );
	}
	
	return true;
}


void CX2UIPersonalTrade::DestoryAllInviteMessage()
{
	for( UINT i=0; i<m_vecTradeInvited.size(); i++ )
	{
		TradeInvited* pTradeInvited = m_vecTradeInvited[i];
		SAFE_DELETE( pTradeInvited );
	}
	m_vecTradeInvited.clear();
	SAFE_DELETE( m_pTradeInviting );
#ifdef ADJUST_THIRD_PERSON_BUG
	m_bTradeRequesting = false;
#endif //ADJUST_PERSONAL_TRADE_BUG
}

//{{ kimhc // 2009-09-07 // Ʈ���̵� �� ǰ�� ���ε� �������� �ִ��� üũ
#ifdef	SEAL_ITEM
bool CX2UIPersonalTrade::CheckIsSealedItemInTradeSlot()
{
	if ( g_pData->GetMyUser() != NULL &&
		 g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		const CX2Inventory& kInventory	= g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

		CX2Item* pItem		= NULL;
		
		for ( UINT i = 0; i < m_vMyItemSlot.size(); i++ )
		{
			pItem	= kInventory.GetItem( m_vMyItemSlot[i].first );

			if ( pItem != NULL && pItem->GetItemData().m_bIsSealed == true )
			{
				return true;				
			}

			pItem	= NULL;
		}

		for ( UINT i = 0; i < m_vOpponentItemSlot.size(); i++ )
		{
			if ( m_vOpponentItemSlot[i].second.IsSealedItem() == true )
				return true;
		}
	}

	return false;
}
#endif	SEAL_ITEM
//}} kimhc // 2009-09-07 // Ʈ���̵� �� ǰ�� ���ε� �������� �ִ��� üũ

#endif

//{{ JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)
#ifdef NUMBER_TO_LANGUAGE
void CX2UIPersonalTrade::PopPersonalTradeTalkBox( UidType iUnitUID, int iED, D3DXVECTOR2 vPos, D3DXCOLOR color /*= 0xff000000 */, D3DXCOLOR outcolor /*= 0x00000000*/, D3DXCOLOR backColor /*= 0xffffffff*/ )
{
	CX2NumbersToLanguage	NumsToLang;			/// ���� ��ȯ ��ü
	string					strOutED;			/// ED
	wstring					wStrOutED;			/// �����ڵ� ����

#if defined(NUMBER_TO_LANGUAGE_JPN)
	NumsToLang.TranslateDigits( CX2NumbersToLanguage::TL_JPN, iED, strOutED );
#elif defined( _LANGUAGE_FIX_CHINESE_ )
	NumsToLang.TranslateDigits( CX2NumbersToLanguage::TL_CHN_SIMPLIFIED, iED, wStrOutED );
#elif defined(CLIENT_COUNTRY_TWHK) || defined(CLIENT_COUNTRY_EU) || defined(CLIENT_COUNTRY_ID) || defined(CLIENT_COUNTRY_US) || defined(CLIENT_COUNTRY_TH) || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_BR)
	strOutED = NumsToLang.ToString(iED);
#else
	NumsToLang.TranslateDigits( CX2NumbersToLanguage::TL_KOR, iED, strOutED );
#endif // NUMBER_TO_LANGUAGE_JPN
	
#ifdef _LANGUAGE_FIX_CHINESE_
	wStrOutED += L" ED";
#else _LANGUAGE_FIX_CHINESE_
	strOutED += " ED";
	ConvertCharToWCHAR( wStrOutED, strOutED );
#endif _LANGUAGE_FIX_CHINESE_

	CX2TalkBoxManagerImp::TalkBox talkBox;
	talkBox.m_vPos				= vPos;
	talkBox.m_OwnerUnitUID		= iUnitUID;
	talkBox.m_wstrTalkContent	= wStrOutED;
	talkBox.m_bTraceUnit		= false;
	talkBox.m_bTrade			= true;
	talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
	talkBox.m_fRemainTime		= 9999999.f;
	talkBox.m_coTextColor		= color;
	talkBox.m_OutLineTextColor	= outcolor;
	talkBox.m_BackTexColor		= backColor;

	talkBox.m_bUseOutLine		 = true;
	if( outcolor == 0x00000000 )	//	�Է� ���� ���� �۾��� �״�� ����
	{
		talkBox.m_bUseOutLine = false;
	}

	m_pTalkBoxMgr->Push( talkBox );
}
#endif NUMBER_TO_LANGUAGE

//}} JHKang / ������ / 2010.09.06 / �ŷ� â ��ǳ�� �߰�(���� --> �ѱ�)