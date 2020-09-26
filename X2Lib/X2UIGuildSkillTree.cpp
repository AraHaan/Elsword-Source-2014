#include "StdAfx.h"

#ifdef NEW_SKILL_TREE_UI
#ifdef GUILD_SKILL
#include ".\x2uiguildskilltree.h"

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const int	MAX_GUILD_SKILL_LEVEL					= 5;		/// ��� ��ų �ִ� ����
	const int	MAGIC_SKILL_DESC_WIDTH					= 370;		/// ��ų ���� ��Ʈ�� ���ٴ� �ִ� ���� ����
	const int	MAX_ONE_CLASS_SKILL_ROW_NUM				= 5;		/// �ϳ��� ������ ��ų Ʈ�� �ִ� �� ����
	const int	SKILL_USE_IMAGE_WIDTH_SIZE				= 70;		/// ��ų ���� ���� �̹��� ���� ũ��
	const float SKILL_MAIN_DESC_OFFSET_Y				= 65.f;		/// ���� ������ ������ ���� ��ġ
#else // UPGRADE_SKILL_SYSTEM_2013
	const int MAGIC_SKILL_DESC_WIDTH = 260;
#endif // UPGRADE_SKILL_SYSTEM_2013

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// CX2SkillTreeSlot : ��ų Ʈ���� ������ ����
//////////////////////////////////////////////////////////////////////////
CX2GuildSkillTreeSlotData::CX2GuildSkillTreeSlotData()
{
	m_eSlotDataType = SDT_SKILL_TREE;
	m_eSkillID = CX2SkillTree::SI_NONE;
}
/*virtual*/ CX2GuildSkillTreeSlotData::~CX2GuildSkillTreeSlotData()
{


}
/*virtual*/ void CX2GuildSkillTreeSlotData::UpdateUI( CKTDGUISlot* pSlot )
{    
	// 1. ���� ���� �� �ִ� ��ų ���� : ��ư�� Ȱ��ȭ/��Ȱ��ȭ. bool ���� ����
	// 2. 1�����̶� ���� ��ų ���� : ��ų ������ �巡�� Ȱ��ȭ / ��Ȱ��ȭ(�нú�� �巡�� �� �ǰ�), �ؽ��� ����. ��Ʈ�� ���� (����/�ִ뷹��)

	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();

	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree; // ���� B ����

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	int iSkillLevel = refUserSkillTree.GetSkillLevel( m_eSkillID, true );

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = refUserSkillTree.GetSkillLevel( m_eSkillID );

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID, (iSkillLevel != 0 ? iSkillLevel : 1) );
#endif // UPGRADE_SKILL_SYSTEM_2013

	if(pSkillTree == NULL || pSkillTemplet == NULL)
		return;

	const map<CX2SkillTree::SKILL_ID, CX2UIGuildSkillTree::SkillSlotUI>& refmapSkillSlotUISet = g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->GetMapSkillSlotUISet();
	//const map<CX2SkillTree::SKILL_ID, CX2UISkillTree::SkillSlotUI>& refmapSkillSlotUISet = g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->GetMapSkillSlotUISet();

	//map<CX2SkillTree::SKILL_ID, CX2UIGuildSkillTree::SkillSlotUI>::const_iterator it = refmapSkillSlotUISet.find(m_eSkillID);
	map<CX2SkillTree::SKILL_ID, CX2UIGuildSkillTree::SkillSlotUI>::const_iterator it = refmapSkillSlotUISet.find(m_eSkillID);

	if(it == refmapSkillSlotUISet.end())
	{
		return;
	}
	//const CX2UIGuildSkillTree::SkillSlotUI& refSkillSlotUI = it->second; // oasis
	const CX2UIGuildSkillTree::SkillSlotUI& refSkillSlotUI = it->second; 

	bool bRemoveMode = g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->GetRemoveMode();
	//bool bRemoveMode = g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->GetRemoveMode(); // oasis

	if( pSkillTemplet->m_bBornSealed == false || refUserSkillTree.IsSkillUnsealed( m_eSkillID ) )
	{
		//ShowSlotPicture( false, pSlot, STSAPT_SEALED );

		// 1. ���� ������Ʈ
		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );

		if(true == refUserSkillTree.DoIHaveThisSkill(m_eSkillID))
		{
			switch(pSkillTemplet->m_eType)
			{
			case CX2SkillTree::ST_ACTIVE:
			case CX2SkillTree::ST_BUFF:
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					pSlot->SetDragable(true);
				} break;
			default:
				{
					pSlot->SetDragable(false);
				} break;
			}
			if(pSkillTemplet->m_bShowSkillLevel)
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

				ShowSlotPicture( true, pSlot, STSAPT_LEVEL, refUserSkillTree.GetSkillLevel(m_eSkillID, true) );
#else // UPGRADE_SKILL_SYSTEM_2013
				ShowSlotPicture( true, pSlot, STSAPT_LEVEL, refUserSkillTree.GetSkillLevel(m_eSkillID) );
#endif // UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			ShowSlotPicture( false, pSlot, STSAPT_LEVEL );
			pSlot->SetDragable(false);
		}

		// 1-2. ���� ���� ������Ʈ
		if( refUserSkillTree.CanILearnThisSkill(m_eSkillID, true) && false == refUserSkillTree.DoIHaveThisSkill(m_eSkillID) )
		{
			// ��� �� �ְ�, �� ������ �ִ� ��ų
			ShowSlotPicture( false, pSlot, STSAPT_DISABLE );
			ShowSlotPicture( true, pSlot, STSAPT_CANLEARN );
		}
		else if(true == refUserSkillTree.IsMaxSkillLevel(m_eSkillID) || refUserSkillTree.CanILearnThisSkill(m_eSkillID, false ) )
		{
			// �̹� ������ �ִ� ��ų
			ShowSlotPicture( false, pSlot, STSAPT_DISABLE );
			ShowSlotPicture( false, pSlot, STSAPT_CANLEARN );
		}
		else
		{				
			// ���� ��ų
			ShowSlotPicture( true, pSlot, STSAPT_DISABLE );
			ShowSlotPicture( false, pSlot, STSAPT_CANLEARN );
		}

		// 2. ��ư ������Ʈ : refUserSkillTree�� ������ �������� �Ѵ�.
		if(false == bRemoveMode)
		{		
			refSkillSlotUI.m_pButtonPlus->SetShow(true);
			refSkillSlotUI.m_pButtonMinus->SetShow(false);
			refSkillSlotUI.m_pButtonMaster->SetShow(false);

			if(true == refUserSkillTree.IsMaxSkillLevel(m_eSkillID))
			{
				refSkillSlotUI.m_pButtonMaster->SetShow(true);
				refSkillSlotUI.m_pButtonPlus->SetShow(false);
			}
			else if(true == refUserSkillTree.CanILearnThisSkill(m_eSkillID))
			{
				refSkillSlotUI.m_pButtonPlus->SetEnable(true);
			}
			else
			{				
				refSkillSlotUI.m_pButtonPlus->SetEnable(false);
			}
		}
		else
		{
			refSkillSlotUI.m_pButtonPlus->SetShow(false);
			refSkillSlotUI.m_pButtonMinus->SetShow(true);
			refSkillSlotUI.m_pButtonMaster->SetShow(false);

			if(true == refUserSkillTree.CanIResetThisSkill(m_eSkillID))
			{
				refSkillSlotUI.m_pButtonMinus->SetEnable(true);
			}
			else
			{
				refSkillSlotUI.m_pButtonMinus->SetEnable(false);
			}
		}

		// 3. ��ų ���� ������Ʈ
		CKTDGUIControl::UIStringData* pString = refSkillSlotUI.m_pStaticLevelString->GetString(0);
		if(pString != NULL)
		{
			int level = refUserSkillTree.GetSkillLevel(m_eSkillID);
			int maxlevel = pSkillTree->GetMaxGuildSkillLevel( 0, m_eSkillID );

			WCHAR buf[32] = {0,};
			int csp = 0;
			refUserSkillTree.GetSkillLevelAndCSP(m_eSkillID, level, csp);

			if(refUserSkillTree.IsMaxSkillLevel(m_eSkillID) == true)
			{
				if(csp > 0)
				{
					//wsprintf( buf, L"#CEC008C%d#CX#C94f412/%d#CX", level, maxlevel );
					StringCchPrintf( buf, 32, L"#CEC008C%d#CX#C94f412/%d#CX", level, maxlevel );

				}
				else
				{
					//wsprintf( buf, L"#C94f412%d/%d#CX", level, maxlevel );
					StringCchPrintf( buf, 32, L"#C94f412%d/%d#CX", level, maxlevel );
				}

			}
			else
			{
				if(csp > 0)
				{
					//wsprintf( buf, L"#CEC008C%d#CX/%d", level, maxlevel );
					StringCchPrintf( buf, 32, L"#CEC008C%d#CX/%d", level, maxlevel );
				}
				else
				{
					//wsprintf( buf, L"%d/%d", level, maxlevel );
					StringCchPrintf( buf, 32, L"%d/%d", level, maxlevel );
				}				
			}

			pString->msg = buf;

		}

		// 4. ȭ��ǥ ������Ʈ
		if(true == refUserSkillTree.IsMaxSkillLevel(m_eSkillID))
		{
			g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->SetArrowColor(m_eSkillID, D3DXCOLOR(1,1,1,1));
		}
		else
		{
			g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->SetArrowColor(m_eSkillID, D3DXCOLOR(0.5f,0.5f,0.5f,1));
		}


		// 5. ����ǥ�� �ڹ���
		refSkillSlotUI.m_pStaticLock->SetShowEnable( false, false );


	}
	else
	{
		// ���ε� ��ų
		// 1. ���� ������Ʈ
		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );

		pSlot->SetDragable(false);
		ShowSlotPicture( true, pSlot, STSAPT_DISABLE );
		//ShowSlotPicture( false, pSlot, STSAPT_SEALED );

		// 2. ��ư ������Ʈ
		refSkillSlotUI.m_pButtonPlus->SetShowEnable(true, false);
		refSkillSlotUI.m_pButtonMinus->SetShow(false);
		refSkillSlotUI.m_pButtonMaster->SetShow(false);

		// 3. ��ų ���� ������Ʈ
		CKTDGUIControl::UIStringData* pString = refSkillSlotUI.m_pStaticLevelString->GetString(0);
		if(pString != NULL)
		{
			int maxlevel = pSkillTree->GetMaxGuildSkillLevel( 0, m_eSkillID );

			WCHAR buf[32] = {0,};
			//wsprintf( buf, L"0/%d", maxlevel );
			StringCchPrintf( buf, 32, L"0/%d", maxlevel );
			pString->msg = buf;
		}
		// 4. ȭ��ǥ ������Ʈ
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->SetArrowColor(m_eSkillID, D3DXCOLOR(0.5f,0.5f,0.5f,1));

		// 4. ��ų ���� ���ĸ� �־�����!
		ShowSlotPicture( false, pSlot, STSAPT_LEVEL );

		// 5. ����ǥ�� �ڹ���
		refSkillSlotUI.m_pStaticLock->SetShowEnable( true, true );

	}

}


void CX2GuildSkillTreeSlotData::ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_TREE_SLOT_ADD_PICTURE_TYPE eType, int val /*= -1*/ )
{
	if(pSlot == NULL)
		return;

	CKTDGUIControl::CPictureData* pPicture = pSlot->GetPicture((int)eType);
	if(pPicture == NULL )
	{
		if(bShow)
		{
			pPicture = new CKTDGUIControl::CPictureData();
			CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData(pSlot->GetNowPoint());
			pPicture->SetPoint( pPoint );

			switch(eType)
			{
			case STSAPT_DISABLE:
				{
					pPicture->SetTex( L"HQ_ImpossibleEqip.tga" );			
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,0.6f );
				} break;
			case STSAPT_SEALED:
				{
					pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"lock" );
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );
				} break;
			case STSAPT_LEVEL:
				{
					// ���� �ؽ���. ���ҽ� ������...
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

					/// ���� �ؽ�ó ��ġ ����
					D3DXVECTOR2 vecPos					= pPicture->GetPos();
					pPicture->SetPos( D3DXVECTOR2( vecPos.x, vecPos.y + 33.f ) );

					D3DXVECTOR2 vecPicturePos			= pPicture->pPoint->leftTopPoint;
					pPicture->pPoint->leftBottomPoint	= D3DXVECTOR2( vecPicturePos.x, vecPicturePos.y + 14.f );
					pPicture->pPoint->rightTopPoint		= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y );
					pPicture->pPoint->rightBottomPoint	= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y + 14.f );
				} break;
			case STSAPT_EQUIPPED:
				{
					pPicture->SetTex( L"DLG_Common_New_Texture26.tga", L"Skill_Mount" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

				} break;
			case STSAPT_CANLEARN:
				{
					pPicture->SetTex( L"DLG_Common_New_Texture26.tga", L"acquire_Skill" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

				} break;
			default:
				{

				} break;
			}

			pSlot->AddPicture( (int)eType, pPicture );
			pPicture->SetShow(bShow);
		}		
	}
	else
	{
		if(bShow)
		{
			switch(eType)
			{
			case STSAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

				} break;
			default:
				break;
			}

		}

		pPicture->SetShow(bShow);
	}

}

//////////////////////////////////////////////////////////////////////////


CX2UIGuildSkillTree::CX2UIGuildSkillTree( CKTDXStage* pNowStage ) :
m_bShow(false),
m_bSkillRemoveMode(false),
m_bClassInfoVaild(false),
//m_MovedPosition(0,0),
//m_DLGPosition(0,0),
m_DLGSize(0,0),
m_pDLGUISkillTree(NULL),
m_pDLGSkillToolTip(NULL),
m_pDLGSkillUnsealInform(NULL),
m_pDLGSkillUnsealed( NULL ),
m_SumDelta(0),
m_iPrevSP(-1),
m_pTalkBoxManager(NULL),
m_bUsingCSP( false )		// �̰͵� �����ϴµ�...
{
	m_pUserSkillTree = NULL;

	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );
}


CX2UIGuildSkillTree::~CX2UIGuildSkillTree(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUISkillTree );	
	SAFE_DELETE_DIALOG( m_pDLGSkillToolTip );
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );

	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealed );

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );

	m_mapSkillSlotUISet.clear();
	m_mapArrowUISet.clear();

}

void CX2UIGuildSkillTree::SetLayer( X2_DIALOG_LAYER layer )
{
	if(m_pDLGUISkillTree != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUISkillTree, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUISkillTree, false );
	}
}

void CX2UIGuildSkillTree::SetPosition( D3DXVECTOR2 vec )
{
	// ���� ���̾�α�
	m_pDLGUISkillTree->SetPos(vec);

	//m_MovedPosition = vec;
	
}

HRESULT CX2UIGuildSkillTree::OnFrameMove( double fTime, float fElapsedTime )
{

	return S_OK;
}

bool CX2UIGuildSkillTree::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �տ� ��� ���̾�αװ� ������ �޽����� ó������ �ʴ´�
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGUISkillTree ) )
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
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			//bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			//g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_SKILL, true);
			//bFlag = true;			
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{ 
			if( m_pDLGUISkillTree->GetIsMouseOver() ) 
				OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	}

	return bFlag;
}

bool CX2UIGuildSkillTree::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	if( pControlList == NULL )
		return false;
	CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
	if(pScroolBarY == NULL)
		return false;

	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;

#if 0
	if(abs(m_SumDelta) < WHEEL_DELTA)
		return true;

	pScroolBarY->Scroll(-m_SumDelta / WHEEL_DELTA);
	m_SumDelta %= WHEEL_DELTA;
#else
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
#endif
	return true;
}

bool CX2UIGuildSkillTree::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case GSTUCM_UNSEAL_SKILL_CANCLE:
		{
			if( m_pDLGSkillUnsealInform != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSkillUnsealInform, NULL, false );
				m_pDLGSkillUnsealInform = NULL;
			}			
		} break;
	case GSTUCM_UNSEAL_SKILL_BUY:
		{
			//** ��ų ���� ��ư�� ������ �� ���⿡ �۾��� �ּ���.

		} break;
	case GSTUCM_CSP_GUIDE_MOUSEIN:
		{	
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			pos.y += pControl->GetHeight();
						
			if(g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
				int iSPoint = 0;
				int iCSPoint = 0;

				// oasis907 : ����� [2009.12.3] // ��� �Ⱓ�� ����Ʈ ���
				int iMaxCSPoint = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iMaxGuildCSPoint; 
				WCHAR wszText[32] = L"";
				iMaxCSPoint = 5; // �ӽ�
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"(%dGSP)\n", iMaxCSPoint );
				
				wstring wstr = L"";
				wstr += GET_STRING(STR_ID_4815);
				wstr += wszText;
				// oasis907 : ����� [2009.12.3] //
				wstr += GetExpirationDateDesc( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrGuildCSPointEndDate, g_pData->GetServerCurrentTime() );

				ShowUIDesc(true, wstr, pos, CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0.97f, 0.23f, 0.06f, 1) );			
			}
			
			return true;
		} break;
	case GSTUCM_CSP_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;
	case GSTUCM_SKILLTREESLOTB_GUIDE_MOUSEIN: // oasis907
		{
/*
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			
			if(g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
				wstring wstr = L"";
				wstr += GET_STRING(STR_ID_2690);
				wstr += GetExpirationDateDesc( refUserSkillTree.GetSkillSlotBEndDateString(), g_pData->GetServerCurrentTime() );
				ShowUIDesc(true, wstr, pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0.97f, 0.23f, 0.06f, 1) );			
			}
						 
			return true;
*/
		} break;
	case GSTUCM_SKILLTREESLOTB_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;

/*
	case GSTUCM_INGAME_ICON_MOUSE_OVER:
		{
			if( g_pX2Game != NULL &&
				g_pX2Game->GetMyUnit() != NULL &&
				g_pX2Game->GetMyUnit()->GetUnit() != NULL )				
			{
				CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
				int iIndex = pControl->GetDummyInt(0);

				const CX2UserSkillTree& refUserSkillTree = g_pX2Game->GetMyUnit()->GetUnit()->GetUnitData().m_UserSkillTree;
				bool bSlotB = !g_pX2Game->GetMyUnit()->GetSelectSkillSlot1();
				CX2UserSkillTree::SkillSlotData* pSlotData = refUserSkillTree.GetSkillSlot( iIndex, bSlotB );
				
				D3DXVECTOR2 pos = pControl->GetPos();
				D3DXVECTOR2 size = pControl->GetNormalPoint()->rightBottomPoint - pControl->GetNormalPoint()->leftTopPoint;

				ShowSkillDesc(true, pSlotData->m_eID, pos, size, true );

			}
			return true;
		} break;
	case GSTUCM_INGAME_ICON_MOUSE_OUT:
		{
			ShowSkillDesc(false);

			return true;
		} break;
*/
	case GSTUCM_SKILL_RESET_MODE_CHANGE_MOUSEOVER:
		{
			// oasis907 : ����� [2009.12.7] //
			if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade == CX2GuildManager::GUG_MASTER)
			{
				CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
				pos.x += pControl->GetWidth() / 2.f;
				ShowUIDesc( true, GET_STRING(STR_ID_2667), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT);
			}
			return true;
		} break;
	case GSTUCM_SKILL_RESET_MODE_CHANGE_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;
	case GSTUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			
			// oasis907 : ����� [2009.11.25] // ���⼭ ó��, ��� ������ �ƴ� ����� ���� �ٸ���
			if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
			{
				ShowUIDesc( true, GET_STRING(STR_ID_4804), pos );
			}
			else
			{
				ShowUIDesc( true, GET_STRING(STR_ID_4803), pos );
			}
			return true;
		} break;
	case GSTUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case GSTUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2664), pos );

			// oasis907 : ����� [2009.11.25] // ���⼭ ó��, ��� ������ �ƴ� ����� ���� �ٸ���
			if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
			{
				ShowUIDesc( true, GET_STRING(STR_ID_4806), pos );
			}
			else
			{
				ShowUIDesc( true, GET_STRING(STR_ID_4805), pos );
			}
		} break;
	case GSTUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case GSTUCM_BUTTON_SKILL_MASTER_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2665), pos );

				return true;
		} break;
	case GSTUCM_BUTTON_SKILL_MASTER_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case GSTUCM_NAVI_LEFT_CLICKED:
	case GSTUCM_NAVI_DRAGGING:
		{
/*
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			if( NULL != pNavi )
			{
				if( m_fNaviMoveRange != 0 )
				{
					vRelativePos = vRelativePos / m_fNaviMoveRange;
				}
								
				if( vRelativePos.y < 0.1f )
				{
					pControlList->SetIndex(0, 0);
				}
				else
				{
					// ��ġ �����ְ�~
					D3DXVECTOR2 vPos = vRelativePos;
					vPos.x = 0.0f;
					vPos.y = vPos.y * ((float)pControlList->GetMaxIndexY() - (float)pControlList->GetViewSizeY() + 1.f );
					pControlList->SetIndex(0, (int)vPos.y, true, false);
				}

			}
			UpdateBlind();
*/
			return true;		

		} break;
	case GSTUCM_CONTROLLIST_SCROLL:
		{
			UpdateBlind();
		
			return true;
		} break;
	case GSTUCM_SKILLTREESLOT_MOUSEIN:
		{
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2GuildSkillTreeSlotData* pSlotData = (CX2GuildSkillTreeSlotData*) pSlot->GetSlotData();
			if(pSlotData == NULL || pSlotData->m_eSkillID == CX2SkillTree::SI_NONE )
			{
				ShowSkillDesc(false);
				return true;
			}

			D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetOffsetPos() + pSlot->GetDialog()->GetPos();
			D3DXVECTOR2 size = pSlot->GetSize();

			ShowSkillDesc(true, pSlotData->m_eSkillID, pos, size, false );		
			return true;

		} break;
	case GSTUCM_EQUIPSLOT_MOUSEIN:
		{
/*
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2GuildSkillTreeSlotData* pSlotData = (CX2GuildSkillTreeSlotData*) pSlot->GetSlotData();
			if(pSlotData == NULL || pSlotData->m_eSkillID == CX2SkillTree::SI_NONE )
			{
				D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetDialog()->GetPos();
				pos.x = pos.x + pSlot->GetSize().x * 0.5f;
				ShowUIDesc( true, GET_STRING(STR_ID_2666), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT );

				ShowSkillDesc(false);
				return true;
			}

			D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetOffsetPos() + pSlot->GetDialog()->GetPos();
			D3DXVECTOR2 size = pSlot->GetSize();
			ShowSkillDesc(true, pSlotData->m_eSkillID, pos, size, true );			
			return true;
*/
		} break;
	case GSTUCM_SKILLTREESLOT_MOUSEOUT:
	case GSTUCM_EQUIPSLOT_MOUSEOUT:
		{			

			ShowUIDesc( false );
			ShowSkillDesc(false);
			return true;

		} break;
	case GSTUCM_SKILLTREESLOT_RMOUSEUP: // oasis907
		{
			const CX2UserSkillTree* refUserSkillTree = &(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree); // oasis907
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2GuildSkillTreeSlotData* pSlotData = (CX2GuildSkillTreeSlotData*)pSlot->GetSlotData();
			if( pSlotData != NULL)
			{

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSlotData->m_eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSlotData->m_eSkillID, 1 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				if(pSkillTemplet != NULL)
				{		
					if( true == pSkillTemplet->m_bBornSealed && false == refUserSkillTree->IsSkillUnsealed(pSlotData->m_eSkillID) )
					{
						D3DXVECTOR2 CenterPos = pSlot->GetPos() + pSlot->GetDialog()->GetPos() + pSlot->GetSize() * 0.5f;
						OpenSkillUnsealInformDLG( pSkillTemplet, CenterPos );
					}
					else if( refUserSkillTree->DoIHaveThisSkill( pSlotData->m_eSkillID ) )
					{
						
						switch(pSkillTemplet->m_eType)
						{
						case CX2SkillTree::ST_ACTIVE:
						case CX2SkillTree::ST_BUFF:
						case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
						case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
						case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
							{
								// ��� ��ų ��Ƽ��� ���� X 
								return true;
								
							} break;


						default:
							break;
						}
					}
				}
			}
			return true;		

		} break;

	case GSTUCM_EXIT:
		{
			SetShow(false);
			return true;
		} break;

	case GSTUCM_EQUIPSLOT_DROPPED:
		{	
/*
			const CX2UserSkillTree* refUserSkillTree = &(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree); // oasis907
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2GuildEquippedSkillSlotData* pSlotData = (CX2GuildEquippedSkillSlotData*)pSlot->GetSlotData();

			CKTDGUISlot* pDroppedSlot = pSlot->GetDroppedSlot();
			CKTDGUISlotData* pDroppedSlotData = NULL;
			if( pDroppedSlot != NULL )
			{
				pDroppedSlotData = pDroppedSlot->GetSlotData();
				switch(pDroppedSlotData->m_eSlotDataType)
				{
				case CKTDGUISlotData::SDT_SKILL_TREE:
					{
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
						CX2GuildSkillTreeSlotData* pDropData = (CX2GuildSkillTreeSlotData*) pDroppedSlotData;
						int iAlreadyEquippedSlotID = refUserSkillTree->GetSkillEquippedSlotIndex(pDropData->m_eSkillID, pSlotData->m_bSlotb );
						// ���� ��ų�� ���� set�� �����Ǿ� ������ ��������
						if( -1 != iAlreadyEquippedSlotID )
						{
							if( true == pSlotData->m_bSlotb )
							{
								g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID+EQUIPPED_SKILL_SLOT_COUNT, CX2SkillTree::SI_NONE );
							}
							else
							{
								g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID, CX2SkillTree::SI_NONE );
							}
						}
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), pDropData->m_eSkillID );
						
						return true;
					} break;
				case CKTDGUISlotData::SDT_SKILL_EQUIP:
					{
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
						// Skill Swap
						CX2GuildEquippedSkillSlotData* pDropData = (CX2GuildEquippedSkillSlotData*) pDroppedSlotData;
						CX2SkillTree::SKILL_ID SkillIDA = pSlotData->m_eSkillID;
						CX2SkillTree::SKILL_ID SkillIDB = pDropData->m_eSkillID;
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), SkillIDB );
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pDropData->GetSlotID(), SkillIDA );
						return true;
					} break;
				default:
					break;
				}
			} 
			return false;

*/
		} break;
	case GSTUCM_BUTTON_SKILL_LEVEL_PLUS:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			int eSkillID = pControl->GetDummyInt(0);

			if( g_pData->GetSkillTree() != NULL)
			{
				g_pData->GetSkillTree()->Handler_EGS_GET_GUILD_SKILL_REQ((CX2SkillTree::SKILL_ID)eSkillID );
			}
			return true;
		} break;
	case GSTUCM_BUTTON_SKILL_LEVEL_MINUS:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			int eSkillID = pControl->GetDummyInt(0);

			CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
			if( NULL != pMyUnit )
			{
				{
					if( NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_GUILD_SKILL_ITEM_ID ) 
#ifdef EVENT_GUILD_ITEM
						|| NULL != pMyUnit->GetInventory().GetItemByTID( EVENT_RESET_A_GUILD_SKILL_ITEM_ID ) 
#endif //EVENT_GUILD_ITEM						
						)
					{
						if(g_pData->GetSkillTree() != NULL)
						{
							g_pData->GetSkillTree()->Handler_EGS_RESET_GUILD_SKILL_REQ((CX2SkillTree::SKILL_ID)eSkillID);
						}
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2678 ), g_pMain->GetNowState() );
					}

				}
				
			}			
			
			return true;
		} break;
	case GSTUCM_EQUIPSLOT_RMOUSEUP:
		{
/*
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2GuildEquippedSkillSlotData* pSlotData = (CX2GuildEquippedSkillSlotData*)pSlot->GetSlotData();
			if(pSlotData->m_eSkillID != CX2SkillTree::SI_NONE)
			{
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_UnEquip.ogg", false, false );
			}			
			g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), CX2SkillTree::SI_NONE );
			return true;
*/
		} break;

	case GSTUCM_SKILL_RESET_MODE_CHANGE:
		{
			m_bSkillRemoveMode = !m_bSkillRemoveMode;
			UpdateUI(false, false, true);
 
			return true;
		} break;
		


	case GSTUCM_GUIDE_BOOK:
		{






		}

	default:
		break;
	}
	return false;
}

void CX2UIGuildSkillTree::SetShow(bool val)
{
	m_bShow = val;

	if(val)	// ���� �� ó���ؾ� �� �κ�
	{

		//{{ oasis907 : ����� [2009.12.2] GuildInfo, GuildMemberInfo ���̾�α׿� GuildSkill ���̾�αװ� ���� ���� �ʴ� ������ ó�� 
		// (������ �� ���̾�α��� ����� �������� �����Ǵ°��� �ٶ���)

		CKTDGUIRadioButton*	pRadio_GuildInfo = static_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Radio_Guild_Info" ) );
		CKTDGUIRadioButton*	pRadio_GuildMemberInfo = static_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Radio_Guild_Member_Info" ) ); 
		CKTDGUIRadioButton*	pRadio_GuildSkill = static_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"RadioButtonGuild_Skill" ) ); 

		if( pRadio_GuildInfo == NULL || 
			pRadio_GuildMemberInfo == NULL  ||
			pRadio_GuildSkill == NULL )
		{
			ASSERT( !"Check Radio buttons at DLG_UI_Guild_Skill_Tree.lua" );
			return;
		}
		pRadio_GuildInfo->SetChecked( false );
		pRadio_GuildMemberInfo->SetChecked( false );
		pRadio_GuildSkill->SetChecked( true );

		//}} oasis907 : ����� [2009.12.2] //

		m_bSkillRemoveMode = false;
		UpdateUI(true, true, true);

		//g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_SKILL);
	}
	else	// ���� �� ó���ؾ� �� �κ�
	{
		ShowSkillDesc(false);
		ShowUIDesc(false);
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);

		//g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_SKILL);
	}

	m_pDLGUISkillTree->SetShowEnable(m_bShow, m_bShow);
}

// ĳ���Ͱ� ���õ��� �� / �������� ���� �� �ش�.
void CX2UIGuildSkillTree::InitSkillTreeUI()
{
	SAFE_DELETE_DIALOG(m_pDLGUISkillTree);
	
	m_mapSkillSlotUISet.clear();
	m_mapArrowUISet.clear();
	m_pDLGUISkillTree = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Guild_Skill_Tree.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUISkillTree );
	m_pDLGUISkillTree->SetDisableUnderWindow(true);
	m_pDLGUISkillTree->SetShow(false);



// 	m_pDLGUISkillTree->CreateGuideDesc();
// 	CKTDGUIStatic* pGuideStatic = (CKTDGUIStatic*) m_pDLGUISkillTree->GetGuideControl();
// 	pGuideStatic->GetString(0)->fontIndex = XUF_DODUM_11_NORMAL;

	D3DXVECTOR3 tmp;
// 	tmp = m_pDLGUISkillTree->GetDummyPos(0);
// 	m_DLGPosition.x = tmp.x;
// 	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUISkillTree->GetDummyPos(1);
 	m_DLGSize.x = tmp.x;
 	m_DLGSize.y = tmp.y;

	

	if( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL && 
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() != NULL  )
	{
		m_pUserSkillTree = &(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree);

		const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet();

		CKTDGUIStatic* pStatic_CharClass = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticCharClass" );
		if( NULL != pStatic_CharClass )
		{
			wstring wstrClassName;
			
			//switch( pUnitTemplet->m_UnitClass )
			switch( CX2Unit::UC_NONE )
			{
				//�ʱ�����
			case CX2Unit::UC_ELSWORD_SWORDMAN:		// 1
			case CX2Unit::UC_ARME_VIOLET_MAGE:		// 2
			case CX2Unit::UC_LIRE_ELVEN_RANGER:		// 3
			case CX2Unit::UC_RAVEN_FIGHTER:			// 4
			case CX2Unit::UC_EVE_NASOD:				// 5
				{
					wstrClassName = pUnitTemplet->m_Description;
				} break;
			case CX2Unit::UC_ELSWORD_KNIGHT:	
				{
					wstrClassName = GET_STRING(STR_ID_2652);                    
				} break;
			case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:	// 11
				{
					wstrClassName = GET_STRING(STR_ID_2653);                    
				} break;
#ifdef ELSWORD_SHEATH_KNIGHT
			case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:		// 22
				{
					wstrClassName = GET_STRING(STR_ID_2652);                    
				} break;
#endif ELSWORD_SHEATH_KNIGHT
			case CX2Unit::UC_LIRE_COMBAT_RANGER:		// 12
				{
					wstrClassName = GET_STRING(STR_ID_2654);                    
				} break;
			case CX2Unit::UC_LIRE_SNIPING_RANGER:		// 13
				{
					wstrClassName = GET_STRING(STR_ID_2655);                    
				} break;
#ifdef	SERV_TRAPPING_RANGER_TEST
			case CX2Unit::UC_LIRE_TRAPPING_RANGER:
				{
					wstrClassName = GET_STRING(STR_ID_15009);
				} break;
#endif	SERV_TRAPPING_RANGER_TEST
			case CX2Unit::UC_ARME_HIGH_MAGICIAN:		// 14
				{
					wstrClassName = GET_STRING(STR_ID_2656);                    
				} break;
			case CX2Unit::UC_ARME_DARK_MAGICIAN:		// 15
				{
					wstrClassName = GET_STRING(STR_ID_2657);                    
				} break;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			case CX2Unit::UC_ARME_BATTLE_MAGICIAN:		// 15
				{
					wstrClassName = GET_STRING(STR_ID_15339);                    
				} break;
#endif
			case CX2Unit::UC_RAVEN_SOUL_TAKER:		// 16
				{
					wstrClassName = GET_STRING(STR_ID_2658);                    
				} break;
			case CX2Unit::UC_RAVEN_OVER_TAKER:		// 17
				{
					wstrClassName = GET_STRING(STR_ID_2659);                    
				} break;
#ifdef RAVEN_WEAPON_TAKER
			case CX2Unit::UC_RAVEN_WEAPON_TAKER:	// 25
				{
					wstrClassName = GET_STRING(STR_ID_15436);                    
				} break;
#endif RAVEN_WEAPON_TAKER
			case CX2Unit::UC_EVE_EXOTIC_GEAR:			// 18
				{
					wstrClassName = GET_STRING(STR_ID_2660);                    
				} break;
			case CX2Unit::UC_EVE_ARCHITECTURE:		// 19
				{
					wstrClassName = GET_STRING(STR_ID_2661);                    
				} break;
#ifdef EVE_ELECTRA
			case CX2Unit::UC_EVE_ELECTRA:			// 26
				{
					wstrClassName = GET_STRING(STR_ID_15828);                    
				} break;
#endif EVE_ELECTRA

			case CX2Unit::UC_ELSWORD_LORD_KNIGHT:		
				{
					wstrClassName = GET_STRING(STR_ID_4641);                    
				} break;
			case CX2Unit::UC_ELSWORD_RUNE_SLAYER:		
				{
					wstrClassName = GET_STRING(STR_ID_4642);                    
				} break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
			case CX2Unit::UC_ELSWORD_INFINITY_SWORD:		
				{
					wstrClassName = GET_STRING(STR_ID_17425);                    
				} break;
#endif

			case CX2Unit::UC_LIRE_WIND_SNEAKER:		
				{
					wstrClassName = GET_STRING(STR_ID_4781);                    
				} break;
			case CX2Unit::UC_LIRE_GRAND_ARCHER:		
				{
					wstrClassName = GET_STRING(STR_ID_4782);                    
				} break;
#ifdef SERV_RENA_NIGHT_WATCHER
			case CX2Unit::UC_LIRE_NIGHT_WATCHER:		
				{
					wstrClassName = GET_STRING(STR_ID_17428);                    
				} break;
#endif


			case CX2Unit::UC_ARME_ELEMENTAL_MASTER:		
				{
					wstrClassName = GET_STRING(STR_ID_4790);
				} break;
			case CX2Unit::UC_ARME_VOID_PRINCESS:		
				{
					wstrClassName = GET_STRING(STR_ID_4791);
				} break;
#ifdef SERV_ARME_DIMENSION_WITCH
			case CX2Unit::UC_ARME_DIMENSION_WITCH:
				{
					wstrClassName = GET_STRING(STR_ID_17427);
				} break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
			case CX2Unit::UC_RAVEN_BLADE_MASTER:		
				{
					wstrClassName = GET_STRING(STR_ID_4849);                    
				} break;
			case CX2Unit::UC_RAVEN_RECKLESS_FIST:		
				{
					wstrClassName = GET_STRING(STR_ID_4850);                    
				} break;
#endif

			//{{ kimhc // 2010.1.22 // �̺� 2�� ����
#ifdef EVE_SECOND_CLASS_CHANGE
			case CX2Unit::UC_EVE_CODE_NEMESIS:
				{
					wstrClassName = GET_STRING( STR_ID_4845 );
				} break;

			case CX2Unit::UC_EVE_CODE_EMPRESS:
				{
					wstrClassName = GET_STRING( STR_ID_4846 );
				} break;
#endif EVE_SECOND_CLASS_CHANGE
			//}} kimhc // 2010.1.22 // �̺� 2�� ����

			//{{ kimhc // 2011.1.3 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
			case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
				{
					wstrClassName = GET_STRING( STR_ID_10237 );
				} break;

			case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
				{
					wstrClassName = GET_STRING( STR_ID_10238 );
				} break;
#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.3 // û 1�� ����

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
				{
					wstrClassName = GET_STRING( STR_ID_15999 );
				} break;
#endif

				//{{ JHKang / ������ / 2011.6.13 / û 2�� ����
#ifdef CHUNG_SECOND_CLASS_CHANGE
			case CX2Unit::UC_CHUNG_IRON_PALADIN:
				{
					wstrClassName = GET_STRING( STR_ID_12769 );
				} break;

			case CX2Unit::UC_CHUNG_DEADLY_CHASER:
				{
					wstrClassName = GET_STRING( STR_ID_12770 );
				} break;
#endif
					//}}
#ifdef ARA_CHANGE_CLASS_FIRST
			case CX2Unit::UC_ARA_LITTLE_HSIEN:
				{
					wstrClassName = GET_STRING( STR_ID_22115 ); //�Ҽ�
				} break;

			case CX2Unit::UC_ARA_SAKRA_DEVANAM:
				{
					wstrClassName = GET_STRING( STR_ID_22116 ); // ��õ
				} break;
#endif //ARA_CHANGE_CLASS_FIRST
			case CX2Unit::UC_NONE:
			default:
				{

				} break;
			}

			pStatic_CharClass->GetString(0)->msg = wstrClassName.c_str();


		}

		//CreateSkillTree( pUnitTemplet->m_UnitClass );
		CreateSkillTree( CX2Unit::UC_NONE ); // oasis

	}	
}

void CX2UIGuildSkillTree::CreateSkillTree( CX2Unit::UNIT_CLASS eUnitClass )
{

	if(m_pDLGUISkillTree == NULL)
		return;
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	if( pControlList == NULL )
		return;

	//1. �ش� Ŭ������ ��ųƮ�� ���ø��� �޾Ƽ�
	//2. iteration�� �� �س����鼭 ������ ��ų���� �ϳ��� ���԰� ��ư�� ����
	// "Slot_SKILLID" "Button_SKILLID" ��� �̸��� �ٿ��ְ� ���� ID���� ��ų ID�� �ٿ��ش�.
	// ���Կ� �ʿ��� �������� ����Ѵ�. 
	//3. �ش��ϴ� ��ġ�� ControlList�� ����־� �ش�
	//4. ���ེų�� ������ y���� �������� �ö󰡸鼭 �ش� ���ེų ��ġ������ ȭ��ǥ�� �����.

	// Unit Class�� �̿��ؼ� �ش� Map�� ��Ե� �޾ƿ´�. �Դٰ� ġ��.
	const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetGuildSkillTreeTempletMap( (int)eUnitClass );


	// �ӽ� ����
	if(refSkillTreeTempletMap.size() == 0)
	{
		return;
	}
	//////

	for( CX2SkillTree::SkillTreeTempletMap::const_iterator mapit = refSkillTreeTempletMap.begin(); mapit != refSkillTreeTempletMap.end(); ++mapit )
	{
		CX2SkillTree::SKILL_ID eSkillID = mapit->first;
		const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = mapit->second;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if(pSkillTemplet == NULL)
		{
#ifndef _SERVICE_
			WCHAR buf[256] = {0,};
			//wsprintf(buf, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
			StringCchPrintf(buf, 256, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
			MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif
			continue;
		}

		int row = refSkillTreeTemplet.m_iTier;
		int col = refSkillTreeTemplet.m_iIndexInTier;

		SkillSlotUI UISet;
		CreateSlotUIPreset( UISet, (int)eSkillID );

		// ��ųID �־� �ְ�
		UISet.m_pSlot->SetID((int)eSkillID);
		UISet.m_pButtonPlus->AddDummyInt((int)eSkillID);
		UISet.m_pButtonMinus->AddDummyInt((int)eSkillID);

		// ����ְ�
		pControlList->InsertItem( UISet.m_pStaticBackGround, row, col );
		pControlList->InsertItem( UISet.m_pSlot, row, col );
		pControlList->InsertItem( UISet.m_pButtonPlus, row, col );
		pControlList->InsertItem( UISet.m_pButtonMinus, row, col );
		pControlList->InsertItem( UISet.m_pButtonMaster, row, col );
		pControlList->InsertItem( UISet.m_pStaticLevelString, row, col );
		pControlList->InsertItem( UISet.m_pStaticActiveMark, row, col );
		pControlList->InsertItem( UISet.m_pStaticLock, row, col );

		// ������ Map�� ����ְ�
		m_mapSkillSlotUISet.insert(std::make_pair(eSkillID, UISet));

		// ���ེų�� �����ϸ� 
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		CX2SkillTree::SKILL_ID PreccedSkillID = static_cast<CX2SkillTree::SKILL_ID>( refSkillTreeTemplet.m_iPrecedingSkill );
		{
			if ( CX2SkillTree::SI_NONE != PreccedSkillID )
			{
				
#else // UPGRADE_SKILL_SYSTEM_2013
		if(refSkillTreeTemplet.m_vecPrecedingSkill.empty() == false)
		{
			// ȭ��ǥ�� ������
			for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
			{
				CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
#endif // UPGRADE_SKILL_SYSTEM_2013

				const CX2SkillTree::SkillTreeTemplet* pPreccedSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)eUnitClass, PreccedSkillID );
				if ( pPreccedSkillTreeTemplet == NULL )
				{
#ifndef _SERVICE_
					WCHAR buf[256] = {0,};
					//wsprintf(buf, L"Skill ID : %d Precceding Skill ID : %d\nPrecceding Skill Exist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID, (int)PreccedSkillID);
					StringCchPrintf(buf, 256, L"Skill ID : %d Precceding Skill ID : %d\nPrecceding Skill Exist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID, (int)PreccedSkillID);

					MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif
					continue;						
				}

				ArrowUI ArrowUISet;
				if(pPreccedSkillTreeTemplet->m_iIndexInTier == col)
				{
					int LineLength = row - pPreccedSkillTreeTemplet->m_iTier - 1;
					CreateVerticalArrowUIPreset( ArrowUISet, LineLength, (int)PreccedSkillID);

					// ����
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowEnd, pPreccedSkillTreeTemplet->m_iTier, col );
					
					// ��
					int LineBodyBegin = pPreccedSkillTreeTemplet->m_iTier + 1;
					for( int i = 0; i < LineLength; ++i )
					{	
						CKTDGUIStatic* pStaticArrowLine = ArrowUISet.m_vpStaticArrowLine[i];
						pControlList->InsertItem( pStaticArrowLine, i+LineBodyBegin, col );
					}
					// �Ӹ�
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowHead, row, col );

				}
				else if(pPreccedSkillTreeTemplet->m_iTier == row)
				{
					// ���� ȭ��ǥ
					if( col > pPreccedSkillTreeTemplet->m_iIndexInTier )
					{
						// ���� ��ų�� ���ʿ� �ִ� : ���������� ���ϴ� ȭ��ǥ
						int LineLength = col - pPreccedSkillTreeTemplet->m_iIndexInTier - 1;
						CreateHorizonalArrowUIPreset( ArrowUISet, LineLength, (int)PreccedSkillID, true );

						// ���� ����
						int LineBodyBegin = pPreccedSkillTreeTemplet->m_iIndexInTier + 1;
						for( int i = 0; i < LineLength; ++i )
						{	
							CKTDGUIStatic* pStaticArrowLine = ArrowUISet.m_vpStaticArrowLine[i];
							pControlList->InsertItem( pStaticArrowLine, row, i+LineBodyBegin );
						}

					}
					else
					{
						// ���� ��ų�� �����ʿ� �ִ� : �������� ���ϴ� ȭ��ǥ
						int LineLength = pPreccedSkillTreeTemplet->m_iIndexInTier - col - 1;
						CreateHorizonalArrowUIPreset( ArrowUISet, LineLength, (int)PreccedSkillID, false );

						// ���� ����
						int LineBodyBegin = col + 1;
						for( int i = 0; i < LineLength; ++i )
						{	
							CKTDGUIStatic* pStaticArrowLine = ArrowUISet.m_vpStaticArrowLine[i];
							pControlList->InsertItem( pStaticArrowLine, row, i+LineBodyBegin );
						}
					}
					
					// ����
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowEnd, row, pPreccedSkillTreeTemplet->m_iIndexInTier);
					// �Ӹ�
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowHead, row, col );
					 
				}
				// ������ �ʿ� �����~��~
				m_mapArrowUISet.insert(std::make_pair(PreccedSkillID, ArrowUISet));
			}
		}
		//}} UI ����ִ� �ڵ�� ������ �������~
		// ���⼭�� ����ֱ⸸ �ϰ�, �� Control�� On/Off ���� �������� ������ ���� UpdateUI �� �� �� �ٷ��.

		// ���� Data�� ����� ���� ����				
		CX2GuildSkillTreeSlotData* pSkillTreeSlotData = new CX2GuildSkillTreeSlotData;
		pSkillTreeSlotData->m_eSkillID = eSkillID;

		UISet.m_pSlot->SetID((int)eSkillID);
		UISet.m_pSlot->SetSlotData( pSkillTreeSlotData );

		pSkillTreeSlotData->m_eSkillID = eSkillID;
		pSkillTreeSlotData->m_bDataUpdated = true;
	
	}

	pControlList->SetIndex(0, 0);

	// Blind Static�� ���� ������ �������
	CKTDGUIStatic* pStaticBlind = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind" );
	m_pDLGUISkillTree->ChangeSequence( pStaticBlind, true );
	CKTDGUIStatic* pStaticBlindSPNotice = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind_Notice" );
	m_pDLGUISkillTree->ChangeSequence( pStaticBlindSPNotice, true );


}

void CX2UIGuildSkillTree::UpdateUI( bool bEquipped, bool bSP /*= true*/, bool bSkillTree /*= false*/ )
{

	if(bSP)
	{
		UpdateSPInfo();
	}

	if(bSkillTree)
	{
		UpdateSkillTree();
	}

	

}

void CX2UIGuildSkillTree::UpdateSkillTree()
{
	// ControlList�� Map�� ���ʷ� Iteration�� ���鼭 (������ ������!)
	// �����̶� ��ư�� �����ϸ� �ش� ������ �����͸� �о�ͼ�
	// �׳� ����Ÿ �ٲ���ٶ�� �����ָ� �ǳ�? -_-
	// 1. ���� ���� �� �ִ� ��ų ���� : ��ư�� Ȱ��ȭ/��Ȱ��ȭ. bool ���� ����
	// 2. 1�����̶� ���� ��ų ���� : ��ų ������ �巡�� Ȱ��ȭ / ��Ȱ��ȭ(�нú�� �巡�� �� �ǰ�), �ؽ��� ����. ��Ʈ�� ���� (����/�ִ뷹��)

	for(map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.begin(); it != m_mapSkillSlotUISet.end(); ++it)
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			if(refSkillSlotUI.m_pSlot->GetSlotData() != NULL)
			{
				refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
			}		 
		}
	}
	UpdateBlind();
}

void CX2UIGuildSkillTree::UpdateSkillTree( CX2SkillTree::SKILL_ID eSkillID )
{
	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.find(eSkillID);
	if( it != m_mapSkillSlotUISet.end() )
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
		}
		const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet((int)0, eSkillID);
		if(pSkillTreeTemplet != NULL)
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			
			CX2SkillTree::SKILL_ID eFollowingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill );
			map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itfol =	m_mapSkillSlotUISet.find(eFollowingSkillID);
			if( itfol != m_mapSkillSlotUISet.end())
			{
				SkillSlotUI& refFollowingSkillSlotUI = itfol->second;
				if ( refFollowingSkillSlotUI.m_pSlot != NULL )
				{
					refFollowingSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
				}

			}
#else // UPGRADE_SKILL_SYSTEM_2013
			for(vector<int>::const_iterator it = pSkillTreeTemplet->m_vecFollowingSkill.begin(); it != pSkillTreeTemplet->m_vecFollowingSkill.end(); ++it)
			{
				CX2SkillTree::SKILL_ID eFollowingSkillID = (CX2SkillTree::SKILL_ID) *it;
				map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itfol =	m_mapSkillSlotUISet.find(eFollowingSkillID);
				if( itfol != m_mapSkillSlotUISet.end())
				{
					SkillSlotUI& refFollowingSkillSlotUI = itfol->second;
					if ( refFollowingSkillSlotUI.m_pSlot != NULL )
					{
						refFollowingSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
					}

				}

			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}

	}
	UpdateBlind();

}

void CX2UIGuildSkillTree::UpdateBlind()
{
	if( m_pDLGUISkillTree == NULL )
		return;
    
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	CKTDGUIStatic* pStaticBlind = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind" );
	CKTDGUIStatic* pStaticBlindSPNotice = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind_Notice" );

	if( pStaticBlindSPNotice != NULL )
	{
		for(int i=0; i<pStaticBlindSPNotice->GetPictureNum(); ++i)
		{
			if( NULL != pStaticBlindSPNotice->GetPicture(i) )
			{
				pStaticBlindSPNotice->GetPicture(i)->SetShow(false);
			}
			if( NULL != pStaticBlindSPNotice->GetString(i) )
			{
				pStaticBlindSPNotice->GetString(i)->msg = L"";
			}
		}
	}
	
	if( pStaticBlind != NULL && pControlList != NULL )
	{
		int TopTier = pControlList->GetIndexY();
		int iUsedSP = 0;
		int iUsedCP = 0;
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.CalcUsedGuildSPointAndCSPoint( iUsedSP, iUsedCP );
		int iTotalUsedSP = iUsedCP + iUsedSP;

		const int MAGIC_SP_NEED_PER_TIER = 5;
		int OpenedLastTier = iTotalUsedSP / MAGIC_SP_NEED_PER_TIER;

		for(int i=0; i<pControlList->GetViewSizeY(); ++i)
		{
			if(TopTier + i > OpenedLastTier)
			{
				if(pStaticBlind->GetPicture(i) != NULL)
					pStaticBlind->GetPicture(i)->SetShow(true);
				if( TopTier + i == OpenedLastTier+1  )
				{
					if( pStaticBlindSPNotice->GetPicture(i) != NULL && pStaticBlindSPNotice->GetString(i) != NULL )
					{
						pStaticBlindSPNotice->GetPicture(i)->SetShow(true);
						pStaticBlindSPNotice->GetString(i)->msg = GET_REPLACED_STRING( ( STR_ID_4807, "i", (OpenedLastTier+1)*MAGIC_SP_NEED_PER_TIER) );
					}					
				}
			}
			else
			{
				if(pStaticBlind->GetPicture(i) != NULL)
					pStaticBlind->GetPicture(i)->SetShow(false);
			}
		}
	}
}


void CX2UIGuildSkillTree::UpdateSPInfo()
{
	if(m_pDLGUISkillTree == NULL)
		return;

	CKTDGUIStatic* pStatic_SP = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticSP" );
	CKTDGUIStatic* pStatic_UsedSP = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticUsedSP" );
	CKTDGUIStatic* pStatic_SPUndo = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticUndo" );

	bool bUsingCSP = false;
	CTime cTime;
	// oasis907 : ����� [2009.12.3] //
	KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrGuildCSPointEndDate, cTime );
	CTime tCurrentTime = g_pData->GetServerCurrentTime();

	if( tCurrentTime >= cTime )
	{
		bUsingCSP = false;
	}
	else
	{
		bUsingCSP = true;
	}

	CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Notice_Notice_SP" );
	if(pButton != NULL)
	{

		CTime cTime;
		// oasis907 : ����� [2009.12.3] //
		KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrGuildCSPointEndDate, cTime );
		CTime tCurrentTime = g_pData->GetServerCurrentTime();

		if( bUsingCSP )
		{
			pButton->SetShowEnable(true, true);
		}
		else
		{
			pButton->SetShowEnable(false, false);			
		}
	}

	if( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		int iNewSP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iGuildSPoint + g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iGuildCSPoint;
		int iUsedSP = 0;
		int iUsedCP = 0;
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.CalcUsedGuildSPointAndCSPoint( iUsedSP, iUsedCP );
		int iTotalUsedSP = iUsedCP + iUsedSP;

		if( NULL != pStatic_SP )
		{		
			wstring wstrText;
			if(bUsingCSP)
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5301, "i", iNewSP ) );
			}
			else
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5300, "i", iNewSP ) );
			}
			
			pStatic_SP->GetString(0)->msg = wstrText.c_str();
		}

		if ( NULL != pStatic_UsedSP )
		{
			wstring wstrText;
			if(bUsingCSP)
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5303, "i", iTotalUsedSP ) );
			}
			else
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5302, "i", iTotalUsedSP ) );
			}
			pStatic_UsedSP->GetString(0)->msg = wstrText.c_str();
		}

		if( NULL != pStatic_SPUndo )		
		{		
			wstring wstrText;
			{	
				int iSkillRevertItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RESET_A_GUILD_SKILL_ITEM_ID );

#ifdef EVENT_GUILD_ITEM
				int iSkillRevertEventItemCount = 0;
				iSkillRevertEventItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( EVENT_RESET_A_GUILD_SKILL_ITEM_ID );
				iSkillRevertItemCount += iSkillRevertEventItemCount;
#endif //EVENT_GUILD_ITEM

				wstrText = GET_REPLACED_STRING( ( STR_ID_5304, "i", iSkillRevertItemCount ) );
			
			}			
			pStatic_SPUndo->GetString(0)->msg = wstrText.c_str();
		}

		if( (m_iPrevSP == 0 && iNewSP > 0) || // SP�� 0���� 0�̻����� ���� ��Ȳ
			(m_iPrevSP > 0 && iNewSP <= 0 ) )	// SP�� 0�̻󿡼� 0���� ���� ��Ȳ
		{
			UpdateSkillTree();
		}
		m_iPrevSP = iNewSP;
	}
	else
	{
		if( NULL != pStatic_SP )
			pStatic_SP->GetString(0)->msg = L"";
		if( NULL != pStatic_SPUndo )
			pStatic_SPUndo->GetString(0)->msg = L"";
	}

	// oasis907 : ����� [2009.12.7] //
	CKTDGUIStatic* pStaticNoticeTop = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticNoticeTop" );
	if( NULL != pStaticNoticeTop )
	{		
		pStaticNoticeTop->GetString(0)->msg = GET_STRING(STR_ID_4814);
	}

	// oasis907 : ����� [2009.12.7] //
	if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
	{	
		CKTDGUIButton* pSkill_Reset = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Skill_Reset" ) );
		pSkill_Reset->SetEnable(false);

		CKTDGUIStatic* pStaticUndo = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStaticUndo" ) );
		pStaticUndo->GetString(0)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
		pStaticUndo->GetString(0)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );
	}
	else
	{
		CKTDGUIButton* pSkill_Reset = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Skill_Reset" ) );
		pSkill_Reset->SetEnable(true);

		CKTDGUIStatic* pStaticUndo = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStaticUndo" ) );
		pStaticUndo->GetString(0)->color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pStaticUndo->GetString(0)->outlineColor = D3DXCOLOR( 0.5f,0.3f, 0.0f, 1.0f );
	}	
}

void CX2UIGuildSkillTree::CreateSlotUIPreset(SkillSlotUI& UISet, int eSkillID )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	WCHAR buf[255] = {0,};
	
	m_pDLGUISkillTree->OpenScriptFile( L"DLG_Guild_Skill_Icon.lua" );
	UISet.m_pStaticBackGround =			(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Background" );
	UISet.m_pStaticActiveMark =			(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticActive" );
	if(UISet.m_pStaticBackGround != NULL && UISet.m_pStaticActiveMark != NULL)
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID)eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID)eSkillID, 1 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if(pSkillTemplet != NULL)
		{
			UISet.m_pStaticBackGround->GetPicture(0)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(1)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(2)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(3)->SetShow(false);

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			UISet.m_pStaticBackGround->GetPicture(4)->SetShow(true);	/// ��ų ���� ���� �и��� ���Ͽ�, �׻� ǥ��
#endif // UPGRADE_SKILL_SYSTEM_2013

			UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(false);
			UISet.m_pStaticActiveMark->GetPicture(1)->SetShow(false);

			switch(pSkillTemplet->m_eType)
			{
			case CX2SkillTree::ST_ACTIVE:
				{
					UISet.m_pStaticBackGround->GetPicture(0)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);
				} break;
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					UISet.m_pStaticBackGround->GetPicture(1)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);
				} break;
				
			case CX2SkillTree::ST_BUFF:
				{
					UISet.m_pStaticBackGround->GetPicture(2)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);					
				} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2SkillTree::ST_RELATIONSHIP_SKILL:
				{
					UISet.m_pStaticBackGround->GetPicture(3)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);										 
					break;
				}
#endif // ADDED_RELATIONSHIP_SYSTEM

			default:
				{
					UISet.m_pStaticBackGround->GetPicture(3)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(1)->SetShow(true);
				} break;
			}
		}
		else
		{
			ASSERT( !"NULL SKILLTEMPLET!!!" );
		}
		//wsprintf( buf, L"g_pStaticSkill_Background_%d", eSkillID );
		StringCchPrintf(buf, 255, L"g_pStaticSkill_Background_%d", eSkillID );
		UISet.m_pStaticBackGround->SetName(buf);
		//wsprintf( buf, L"StaticActive_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticActive_%d", eSkillID );
		UISet.m_pStaticActiveMark->SetName(buf);
		
	}
	
	UISet.m_pSlot =						(CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"g_pSlot_SkillTree" );
	if(UISet.m_pSlot != NULL)
	{
		//wsprintf( buf, L"g_pSlot_SkillTree_%d", eSkillID );
		StringCchPrintf(buf, 255, L"g_pSlot_SkillTree_%d", eSkillID );
		UISet.m_pSlot->SetName(buf);
	}
	

	UISet.m_pButtonPlus =				(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Plus" );
	if(UISet.m_pButtonPlus != NULL)
	{
		//wsprintf( buf, L"Button_Plus_%d", eSkillID );
		StringCchPrintf(buf, 255, L"Button_Plus_%d", eSkillID );
		UISet.m_pButtonPlus->SetName(buf);
		UISet.m_pButtonPlus->ChangeMouseUpSound( L"" );
		UISet.m_pButtonPlus->ChangeMouseOverSound( L"" );

	}
	
	UISet.m_pButtonMinus =				(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Minus" );
	if(UISet.m_pButtonMinus != NULL)
	{
		//wsprintf( buf, L"Button_Minus_%d", eSkillID );
		StringCchPrintf(buf, 255, L"Button_Minus_%d", eSkillID );
		UISet.m_pButtonMinus->SetName(buf);
		UISet.m_pButtonMinus->ChangeMouseUpSound( L"" );
		UISet.m_pButtonMinus->ChangeMouseOverSound( L"" );
	}
	

	UISet.m_pButtonMaster =	(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Master" );
	if( UISet.m_pButtonMaster != NULL )
	{
		//wsprintf( buf, L"Button_Master_%d", eSkillID );
		StringCchPrintf(buf, 255, L"Button_Master_%d", eSkillID );
		UISet.m_pButtonMaster->SetName(buf);
	}
	
	UISet.m_pStaticLevelString =		(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticLevel" );
	if( UISet.m_pStaticLevelString != NULL)
	{
		//wsprintf( buf, L"StaticLevel_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticLevel_%d", eSkillID );
		UISet.m_pStaticLevelString->SetName(buf);

	}


	UISet.m_pStaticLock =		(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Lock" );
	if( UISet.m_pStaticLock != NULL)
	{
		//wsprintf( buf, L"StaticLock_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticLock_%d", eSkillID );
		UISet.m_pStaticLock->SetName(buf);
	}


	return;
}

void CX2UIGuildSkillTree::CreateVerticalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail.lua" );

	WCHAR buf[255] = {0,};

	UISet.m_pStaticArrowEnd =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_End" );
	if( UISet.m_pStaticArrowEnd != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_End_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticSkill_Arrow_End_%d", eSkillID );
		UISet.m_pStaticArrowEnd->SetName(buf);
	}

	UISet.m_pStaticArrowHead =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Head" );
	if( UISet.m_pStaticArrowHead != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_Head_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticSkill_Arrow_Head_%d", eSkillID );
		UISet.m_pStaticArrowHead->SetName(buf);
	}

	for( int i = 0; i < iLineLength; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_Line.lua" );
		CKTDGUIStatic* pStaticArrowLine 		= (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Line" );
		if( pStaticArrowLine != NULL )
		{
			//wsprintf( buf, L"StaticSkill_Arrow_Line_%d_%d", eSkillID, i );
			StringCchPrintf(buf, 255, L"StaticSkill_Arrow_Line_%d_%d", eSkillID, i );
			pStaticArrowLine->SetName(buf);
			UISet.m_vpStaticArrowLine.push_back(pStaticArrowLine);
		}		
	}
}

void CX2UIGuildSkillTree::CreateHorizonalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID, bool bRight )
{

	if( m_pDLGUISkillTree == NULL )
		return;

	if(bRight)
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail_Right.lua" );
	}
	else
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail_Left.lua" );
	}
	

	WCHAR buf[255] = {0,};

	UISet.m_pStaticArrowEnd =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_End_H" );
	if( UISet.m_pStaticArrowEnd != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_End_H_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticSkill_Arrow_End_H_%d", eSkillID );
		UISet.m_pStaticArrowEnd->SetName(buf);
	}

	UISet.m_pStaticArrowHead =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Head_H" );
	if( UISet.m_pStaticArrowHead != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_Head_H_%d", eSkillID );
		StringCchPrintf(buf, 255, L"StaticSkill_Arrow_Head_H_%d", eSkillID );
		UISet.m_pStaticArrowHead->SetName(buf);
	}

	for( int i = 0; i < iLineLength; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_Line_Horizonal.lua" );
		CKTDGUIStatic* pStaticArrowLine 		= (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Line_H" );
		if( pStaticArrowLine != NULL )
		{
			//wsprintf( buf, L"StaticSkill_Arrow_Line_H_%d_%d", eSkillID, i );
			StringCchPrintf(buf, 255, L"StaticSkill_Arrow_Line_H_%d_%d", eSkillID, i );
			pStaticArrowLine->SetName(buf);
			UISet.m_vpStaticArrowLine.push_back(pStaticArrowLine);
		}		
	}

}



void CX2UIGuildSkillTree::SetArrowColor( CX2SkillTree::SKILL_ID eSkillID, D3DXCOLOR cCol )
{
	std::pair< std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator, std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator> range = m_mapArrowUISet.equal_range(eSkillID);
	
	for (std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator it = range.first; it != range.second; ++it)
	{
		ArrowUI& refArrow = it->second;

		refArrow.m_pStaticArrowEnd->SetColor( cCol );
		refArrow.m_pStaticArrowHead->SetColor( cCol );
		for(vector<CKTDGUIStatic*>::iterator itstatic =	refArrow.m_vpStaticArrowLine.begin(); itstatic != refArrow.m_vpStaticArrowLine.end(); ++itstatic)
		{
			CKTDGUIStatic* pStatic = *itstatic;
			pStatic->SetColor( cCol );
		}
		
	}
}



// CX2UIGuildSkillTree::SkillSlotUI& CX2UIGuildSkillTree::GetSkillSlotUI( CX2SkillTree::SKILL_ID eSkillID )
// {
// 	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it = m_mapSkillSlotUISet.find(eSkillID);
// 	if(it != m_mapSkillSlotUISet.end())
// 	{
// 		return it->second;
// 	}
// 
// 	ASSERT( !"Wrong Skill ID" );
// 	return m_mapSkillSlotUISet.begin()->second;
// 
// }

// CX2UIGuildSkillTree::ArrowUI& CX2UIGuildSkillTree::GetArrowUI( CX2SkillTree::SKILL_ID eSkillID )
// {
// 	map<CX2SkillTree::SKILL_ID, ArrowUI>::iterator it = m_mapArrowUISet.find(eSkillID);
// 	if(it != m_mapArrowUISet.end())
// 	{
// 		return it->second;
// 	}
// 
// 	ASSERT( !"Wrong Skill ID" );
// 	return m_mapArrowUISet.begin()->second;
// }


// ��� ��ų�� ��Ŭ���� �ϸ� ��Ÿ���� ���̾�α׸� ���� ���� �Լ�.
// ���� ��ų �������� ��ü�� �̱����̹Ƿ� ��ݸ� �۾� �д�.
// ���̾�α� �ݴ� �������� ����� �׽��ϴ�. ���� ��ư�� ������ ���� �ƹ��͵� �� �ϰ� �Ǿ� ������ ���� �۾��� �в����� 
// UICustomMsgProc���� case GSTUCM_UNSEAL_SKILL_BUY �׸� �� �κ��� �۾��� �ּ���.
void CX2UIGuildSkillTree::OpenSkillUnsealInformDLG( const CX2SkillTree::SkillTemplet* pSkillTemplet, D3DXVECTOR2 SlotCenterPos )
{	
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );
		
	m_pDLGSkillUnsealInform = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Purchase_Window.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealInform );
	m_pDLGSkillUnsealInform->SetDisableUnderWindow(true);

	// ���̾�αװ� ������ �� ��ġ(���� �߽ɿ��������� ��밪) : ��ũ��Ʈ�� �־� ��. DLG_Skill_Purchase_Window.lua �ֻ�� ����.
	D3DXVECTOR2 OffsetPos((float)m_pDLGSkillUnsealInform->GetDummyInt(0), (float)m_pDLGSkillUnsealInform->GetDummyInt(1));	
	m_pDLGSkillUnsealInform->SetPos( SlotCenterPos + OffsetPos );

#if 0		//** fix!! ��ų �������� ����� �� �� ���⿡ �߰��۾� �� �ּ���. 
//{{AFX
	CKTDGUIStatic* pStaticRequireItem = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticRequireItemName" );
	CKTDGUIStatic* pStaticRequireCash = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticPrice" );

	if( NULL != pStaticRequireCash->GetString(0) )
	{
		//** ���⿡ �ش� ��ų�� ���������� ������ �� �ּ���
		int iRequireCash = pSkillTemplet->����;
		WCHAR buf[256] = {0,};
		//wsprintf( buf, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
		StringCchPrintf( buf, 256, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
		pStaticRequireCash->GetString(0)->msg = buf;
	}
	else
	{
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
		ASSERT( "Script Error." );
		return;
	}

	if( NULL != pStaticRequireItem->GetString(0) )
	{

		//** ���⿡ �ش� ��ų�� ���������� �̸��� �� �ּ���
		WCHAR buf[256] = {0,};
		//wsprintf( buf, L"%s", pSkillTemplet->�ʿ��Ѿ������̸� );
		StringCchPrintf( buf, 256, L"%s", pSkillTemplet->�ʿ��Ѿ������̸� );
		pStaticRequireCash->GetString(0)->msg = buf;
	}
	else
	{
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
		ASSERT( "Script Error." );
		return;
	}

	m_pDLGSkillUnsealInform->SetShowEnable(true, true);
//}}AFX
#endif

		
}

// SlotManager�� ItemDesc �� �ڵ带 �ܾ����
void CX2UIGuildSkillTree::ShowSkillDesc( bool bShow, CX2SkillTree::SKILL_ID eSkillID /*= CX2SkillTree::SI_NONE*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, 
								    D3DXVECTOR2 size /*= D3DXVECTOR2(0,0)*/, bool bEquipped /*= false*/ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

	if(m_pDLGSkillToolTip == NULL )
	{
		/// ���� ���̾�α� ����
		m_pDLGSkillToolTip = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Skill_Desc.lua", 0.07f, XDL_POP_UP );

		if ( NULL != m_pDLGSkillToolTip )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillToolTip );
		else
			return;
	}

	// �������..

	if(bShow)
	{	
		CKTDGUIStatic* pItemDesc = static_cast<CKTDGUIStatic*>( m_pDLGSkillToolTip->GetControl(L"SkillDesc") );

		if ( NULL == g_pData ||
			NULL == g_pData->GetSkillTree() ||
			NULL == g_pData->GetTitleManager() ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() ||
			NULL == g_pKTDXApp ||
			NULL == g_pKTDXApp->GetDGManager() ||
			NULL == g_pKTDXApp->GetDGManager()->GetDialogManager() ||
			NULL == pItemDesc )
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		CKTDGUIControl::UIStringData* pTitleStringData		= pItemDesc->GetString( 0 );	/// Ÿ��Ʋ ���� ��Ʈ�� ������ ( ��ų �̸� �� ���� )
		CKTDGUIControl::UIStringData* pMainStringData		= pItemDesc->GetString( 1 );	/// ���� ��ų ���� ���� ��Ʈ�� ������ ( ��ų ���� �� ȿ�� )
		CKTDGUIControl::UIStringData* pSubStringData		= pItemDesc->GetString( 2 );	/// ���� ��ų ���� ��Ʈ�� ������ ( ��ų Ÿ��, �Ҹ� MP, ���� �ð� )

		CKTDGUIControl::CPictureData* pMainPictureData		= pItemDesc->GetPicture( 0 );	/// ���� ��ų ���� ���� ��� �̹��� ������
		CKTDGUIControl::CPictureData* pIconPictureData		= pItemDesc->GetPicture( 1 );	/// ��ų ������ �̹��� ������
		CKTDGUIControl::CPictureData* pUseSkillPictureData	= pItemDesc->GetPicture( 2 );	/// ��ų ���� ���� �̹��� ������

		if ( NULL == pTitleStringData ||
			NULL == pMainStringData ||
			NULL == pSubStringData ||
			NULL == pMainPictureData ||
			NULL == pIconPictureData ||
			NULL == pUseSkillPictureData )
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}


		/// ��ų ���� ���� -------------------------------------------------------------------------------------
		const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;

		int iSkillLevel = refUserSkillTree.GetSkillLevel( eSkillID , true );		/// ��ų ���� ( ������ �����Ǿ� �ִ� ���� )

		/// ��ų ���ø� ��ȯ -----------------------------------------------------------------------------------
		int iSkillTempletLevel =  iSkillLevel != 0 ? iSkillLevel : 1;

		iSkillTempletLevel = min( iSkillTempletLevel, MAX_LIMITED_SKILL_LEVEL );

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );

		if ( NULL == pSkillTemplet )	/// ��ų ���ø��� ������, ������ ���� ������
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}


		/// ��ų Ʈ�� ���ø� ��ȯ ------------------------------------------------------------------------------
		int										iUnitClass			= static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
		const CX2SkillTree::SkillTreeTemplet*	pSkillTreeTemplet	= g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( 0, pSkillTemplet->m_eID );


		if ( NULL == pSkillTreeTemplet )	/// ��ų Ʈ�� ���ø��� ������, ������ ���� ������
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}


		/// ��ų ������ ���� -----------------------------------------------------------------------------------
		pIconPictureData->SetTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );
		//pIconPictureData->SetSize( D3DXVECTOR2( 30.f, 30.f ) );


		/// ��ų ���� ���� �̹��� ���� -------------------------------------------------------------------------
		if ( L"" != pSkillTemplet->m_wstrImageName && L"" != pSkillTemplet->m_wstrImagePieceName )
		{
			pUseSkillPictureData->SetShow( true );		/// ��ų �̹����� �����Ǿ� ���� ���� ǥ��
			pUseSkillPictureData->SetTex( pSkillTemplet->m_wstrImageName.c_str(), pSkillTemplet->m_wstrImagePieceName.c_str() );
		}
		else
		{
			pUseSkillPictureData->SetShow( false );
		}


		/// Ÿ��Ʋ ���� ���� -----------------------------------------------------------------------------------
		wstring SkillSkillNameAndLevelDesc = L""; 

		GetSkillTitleDesc( SkillSkillNameAndLevelDesc, pSkillTemplet, pSkillTreeTemplet, iSkillLevel );		/// ��ų �̸� �� ���� ��Ʈ�� ����

		if( SkillSkillNameAndLevelDesc.empty() == true )		/// ��Ʈ���� ������, ������ ���� ������
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		/// ��Ʈ�� ����
		pTitleStringData->msg	= SkillSkillNameAndLevelDesc;

		/// ��ġ ���� ( ��ų ������ ������ ���� )
		pTitleStringData->pos.x	= pMainStringData->pos.x + 45.f;


		/// ���� ���� ���� -------------------------------------------------------------------------------------
		wstring SkillSubDesc = L"";

		GetSkillSubDesc( pSkillTemplet, SkillSubDesc );		/// ���� ���� ��Ʈ�� ����

		if( SkillSubDesc.empty() == true )			/// ��Ʈ���� ������, ������ ���� ������
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		/// ��Ʈ�� ����
		pSubStringData->msg		= SkillSubDesc;


		wstring wstrMainSkillDesc = L"";
		/// ���� ��ų ���� ���� --------------------------------------------------------------------------------
		if ( 0 < pSkillTreeTemplet->m_iPrecedingSkill )		/// ���� ��ų�� ���� ��, ���� ��ų 
			GetPrecedingSkillDesc( wstrMainSkillDesc, static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iPrecedingSkill ) );

		/// ���� ���� ���� -------------------------------------------------------------------------------------
		wstrMainSkillDesc += pSkillTemplet->m_wstrMainDesc;					/// ����Ǵ� ��ų ���� ��Ʈ�� �߰� ( ���� ����� ��ų ȿ�� ���̿� ���� �ֱ� ����, ���� �и� )

		wstrMainSkillDesc += L"\n";

		/// �� ����
		wstrMainSkillDesc = g_pMain->GetStrByLineBreakColor
			( wstrMainSkillDesc.c_str(), static_cast<int>( MAGIC_SKILL_DESC_WIDTH ), SLOT_MANAGER_FONT_INDEX );


		/// ���� ũ�� ���� -------------------------------------------------------------------------------------
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );	/// XUF_DODUM_13_SEMIBOLD

		/// ���� ���� ��Ʈ�� ����
		int iMainDescHeight = static_cast<int>( pItemDescFont->GetHeight( wstrMainSkillDesc.c_str() ) );

		/// ���ڰ� ���� ó��
		int iLineNum = 0;
		int iOffSetSize = wstrMainSkillDesc.find( L"\n", 0 );
		while( iOffSetSize != -1 )
		{
			iOffSetSize++;
			iLineNum += 2;
			iOffSetSize = wstrMainSkillDesc.find( L"\n", iOffSetSize );
		}
		iMainDescHeight += iLineNum;

		/// ���� ���� Y ��ġ
		int iMainDescYPos = static_cast<int>( pTitleStringData->pos.y + SKILL_MAIN_DESC_OFFSET_Y );

		/// ������ ���� ������ ���� ����
		int			itemDescWidth		= static_cast<int>( MAGIC_SKILL_DESC_WIDTH ) + ITEM_DESC_ADD_SIZE_X;
		/// ������ ���� ������ ����
		int			itemDescHeight		= static_cast<int>( iMainDescHeight / g_pKTDXApp->GetResolutionScaleY() );


		/// ���� �� ���Ҽ� �߰� ( ���� ����� ��ų ȿ�� ���� )
		CKTDGUIStatic* pDevideLine = static_cast<CKTDGUIStatic*>( m_pDLGSkillToolTip->GetControl(L"g_pStatic_HorizonLine") );
		if ( NULL != pDevideLine && 
			NULL != pDevideLine->GetPicture( 0 ) && 
			NULL != pDevideLine->GetPicture( 0 )->pPoint && 
			NULL != pDevideLine->GetPicture( 1 ) &&
			NULL != pDevideLine->GetPicture( 1 )->pPoint )
		{
			pDevideLine->GetPicture( 0 )->SetSizeX( itemDescWidth - 20.f );
			pDevideLine->GetPicture( 1 )->SetSizeX( itemDescWidth - 20.f );
			pDevideLine->GetPicture( 1 )->SetPos( D3DXVECTOR2( 10.f, static_cast<float>( iMainDescYPos + itemDescHeight ) ) );
		}


		/// ��ų ȿ�� ���� ��ȯ ( ���� ��������� ���̸� ���� �� �ش� ��ġ�� �� ����, ���� ������ ��Ʈ�� �����Ͽ� ���� �ϼ� )
		GetSkillEffectiveDesc( wstrMainSkillDesc, iSkillLevel, pSkillTemplet );

		/// �� ����
		wstrMainSkillDesc = g_pMain->GetStrByLineBreakColor
			( wstrMainSkillDesc.c_str(), static_cast<int>( MAGIC_SKILL_DESC_WIDTH ), SLOT_MANAGER_FONT_INDEX );

		pMainStringData->msg = wstrMainSkillDesc;		/// ���� ���� ���� ��Ʈ�� ����


		/// ���� ���� ��Ʈ�� ���� ( ���� ���� + ��ų ȿ�� )
		iMainDescHeight = static_cast<int>( pItemDescFont->GetHeight( pMainStringData->msg.c_str() ) + SKILL_MAIN_DESC_OFFSET_Y );

		/// ���ڰ� ���� ó��
		iLineNum = 0;
		iOffSetSize = wstrMainSkillDesc.find( L"\n", 0 );
		while( iOffSetSize != -1 )
		{
			iOffSetSize++;
			iLineNum += 2;
			iOffSetSize = wstrMainSkillDesc.find( L"\n", iOffSetSize );
		}
		iMainDescHeight += iLineNum;


		/// ���� ���� �� ���� ( ���� ���� + ��ų ȿ�� )
		itemDescHeight	= static_cast<int>( iMainDescHeight / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;

		pTitleStringData->fontIndex	= SLOT_MANAGER_FONT_INDEX;	/// Ÿ��Ʋ ���� ��Ʈ ũ�� ����
		pMainStringData->fontIndex	= SLOT_MANAGER_FONT_INDEX;	/// ���� ���� ��Ʈ ũ�� ����
		pSubStringData->fontIndex	= SLOT_MANAGER_FONT_INDEX;	/// ���� ���� ��Ʈ ũ�� ����


		/// ��ų ���� ���� ��ġ ���� ---------------------------------------------------------------------------
		int itemDescPosX = 0;
		int itemDescPosY = 0;

		int iDescWidthWithPicture = true == pUseSkillPictureData->GetShow() ?
			static_cast<int>( itemDescWidth + ( 10.f * g_pKTDXApp->GetResolutionScaleX() ) + pUseSkillPictureData->GetSize().x ) : itemDescWidth;

		if ( pos.x + ( size.x / 2 ) - ( iDescWidthWithPicture / 2 ) >= 0 ) /// ���ʿ� ������ ����� ���
		{
			if ( pos.x+ ( size.x / 2 ) - ( iDescWidthWithPicture / 2 ) + iDescWidthWithPicture > 1024 )
			{
				itemDescPosX = static_cast<int>( 1024 - iDescWidthWithPicture );
			}
			else
			{
				itemDescPosX = static_cast<int>( pos.x + ( size.x / 2 ) - ( iDescWidthWithPicture / 2 ) );
			}
		}
		else
		{
			itemDescPosX = 0;
		}

		/// ����Ű�� �ִ� ��ų ���� �������� ������ ǥ���� �� �ִ°��
		if ( static_cast<int>( pos.y ) - itemDescHeight >= 0 )
		{
			/// ����Ű�� �ִ� ��ų ���� ��ġ + ���� ������
			itemDescPosY =  static_cast<int>( pos.y ) - itemDescHeight;
		}
		else	/// ����Ű�� �ִ� ��ų ���� �Ʒ��� ǥ��
		{
			itemDescPosY = static_cast<int>( pos.y + size.y );		/// ��ų ������ Bottom ��ġ

			int iBottomOverSize = 768 - (itemDescPosY + itemDescHeight );	/// ȭ���� �Ѿ�� ũ�⸦ ����Ͽ�, ��ų ������ Bottom ��ġ�� ���� ���� �ش�.

			if ( iBottomOverSize < 0 )
				itemDescPosY += iBottomOverSize;
		}

		// ���� ���κ��� ȭ�� ������ ������ �ʵ��� ����
		if( itemDescPosY < 0 )
			itemDescPosY = 0;

		/// ���� ��ġ ����
		m_pDLGSkillToolTip->SetPos( D3DXVECTOR2( static_cast<float>( itemDescPosX ), static_cast<float>( itemDescPosY ) ) );

		/// ���� ��� ����
		pMainPictureData->SetSize( D3DXVECTOR2( static_cast<float>( itemDescWidth ), static_cast<float>( itemDescHeight ) ) );
		pMainPictureData->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );

		bool bCheckEquipping = false;


		/// ���� ���� ������ y��ǥ�� Ÿ��Ʋ ������ ��ġ�� ���� ���� 
		/// ( Ÿ��Ʋ ������ ���� ��ܿ� �����Ƿ�, �������� �����Ͽ� ���� + Ÿ��Ʋ ��� ��Ʈ�� ���� + Ÿ��Ʋ ��Ʈ���� ���� ���� Ÿ��Ʋ ������ ���� )
		pMainStringData->pos.y = static_cast<float>( iMainDescYPos );

		/// Ÿ��Ʋ ��Ʈ�� ��ġ ���� ( ��ų ������ ������ ���� )
		pTitleStringData->pos.x	= pMainStringData->pos.x + 45.f;

		/// ���� ���� ��ġ�� ���� ���� ���� ���̿� ���� ���� ( ���� ũ���� ���� �������� '�ؽ�Ʈ ������ ����'�� ���� )
		pSubStringData->pos.x	= static_cast<float>( pMainStringData->pos.x + MAGIC_SKILL_DESC_WIDTH - 15.f );
		/// ��ų ���� ���� �̹��� ��ġ ���� ( ���� ������ �� + Offset )
		pUseSkillPictureData->SetPos( D3DXVECTOR2( static_cast<float>(itemDescWidth ) + ( 10.f * g_pKTDXApp->GetResolutionScaleX() ), 0.f ) );


		m_pDLGSkillToolTip->SetShow( true );
	}
	else
	{
		m_pDLGSkillToolTip->SetShow( false );
	}

#else // UPGRADE_SKILL_SYSTEM_2013

	if(m_pDLGSkillToolTip == NULL )
	{
		m_pDLGSkillToolTip = new CKTDGUIDialog( g_pMain->GetNowState(), L"", 0.07f, XDL_POP_UP );
		m_pDLGSkillToolTip->SetFront(true);
		m_pDLGSkillToolTip->SetCloseOnFocusOut(true);

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillToolTip );
		m_pDLGSkillToolTip->SetFront( true );
		m_pDLGSkillToolTip->SetModal( true );

		CKTDGUIStatic* pStatic = new CKTDGUIStatic();
		pStatic->SetName( L"SkillDesc" );
		m_pDLGSkillToolTip->AddControl( pStatic );
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pStatic->AddPicture( pPicture );
		pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
		pPicture->SetTex( L"FadeInOut.dds" );

		CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
		pStatic->AddString( pString );
		pString->fontIndex = SLOT_MANAGER_FONT_INDEX;
		pString->color = D3DXCOLOR( 1,1,1,1 );
		pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
		pString->msg = L"";
		pString->fontStyle = CKTDGFontManager::FS_NONE;
		pString->sortFlag = DT_LEFT;
		pString->pos = D3DXVECTOR2( 20, 20 );
	}

	// �������..

	if(bShow)
	{		
		wstring SkillDesc = GetSkillDesc(eSkillID, bEquipped);
		if( SkillDesc.empty() == true )
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*)m_pDLGSkillToolTip->GetControl(L"SkillDesc");

		const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pItemDesc->GetString(0)->fontIndex );
		
#ifdef CLIENT_GLOBAL_LINEBREAK
		CWordLineHandler::LineBreakInX2Main( SkillDesc, pFont, (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
		CX2Main::LineBreak( SkillDesc, pFont, (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#endif //CLIENT_GLOBAL_LINEBREAK
			
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
		int itemDescWidth = (int)( pItemDescFont->GetWidth( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
		int itemDescHeight = (int)( pItemDescFont->GetHeight( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
		
		// ������ ������ ���̰� �ʹ� ��� ��Ʈ ũ�⸦ ������
		
		if( NULL != pItemDesc && 
			NULL != pItemDesc->GetString(0) )
		{
			const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = 600;
			
			if( itemDescHeight > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
			{
				pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
				pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX_SMALL );
				itemDescWidth = (int)( pItemDescFont->GetWidth( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
				itemDescHeight = (int)( pItemDescFont->GetHeight( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
			}
			else
			{
				pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX;
			}
		}
		
		int offsetSize = 0;
		int lineNum = 0;
		offsetSize = SkillDesc.find( L"\n", offsetSize );
		while( offsetSize != -1 )
		{
			offsetSize++;
			lineNum++;
			offsetSize = SkillDesc.find( L"\n", offsetSize );
		}
		itemDescHeight += lineNum;

		int itemDescPosX = 0;
		int itemDescPosY = 0;

		if ( pos.x + ( size.x / 2 ) - ( itemDescWidth / 2 ) >= 0 ) //���ʿ� ������ ����� ���
		{
			if ( pos.x+ ( size.x / 2 ) - ( itemDescWidth / 2 ) + itemDescWidth > 1024 )
			{
				itemDescPosX = (int)(1024 - itemDescWidth);
			}
			else
			{
				itemDescPosX = (int)(pos.x + ( size.x / 2 ) - ( itemDescWidth / 2 ));
			}
		}
		else
		{
			itemDescPosX = 0;
		}

		if ( pos.y - itemDescHeight >= 0 ) //�������� ǥ���� �� �ִ°��
		{
			itemDescPosY = (int)(pos.y - itemDescHeight);
		}
		else
		{
			itemDescPosY = (int)(pos.y + size.y);

			int overSize = 768 - (itemDescPosY + itemDescHeight);
			if ( overSize < 0 )
			{
				itemDescPosY += overSize;
			}
		}

		// ���� ���κ��� ȭ�� ������ ������ �ʵ��� ����
		if( itemDescPosY < 0 )
			itemDescPosY = 0;
		
		m_pDLGSkillToolTip->SetPos( D3DXVECTOR2( (float)itemDescPosX, (float)itemDescPosY ) );
		m_pDLGSkillToolTip->GetStatic_LUA( "SkillDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
		m_pDLGSkillToolTip->GetStatic_LUA( "SkillDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );

		bool bCheckEquipping = false;
		

		m_pDLGSkillToolTip->GetStatic_LUA( "SkillDesc" )->GetString(0)->msg = SkillDesc.c_str();

		m_pDLGSkillToolTip->SetShow( true );
	}
	else
	{
		m_pDLGSkillToolTip->SetShow( false );
	}

#endif // UPGRADE_SKILL_SYSTEM_2013
}

#ifndef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

wstring CX2UIGuildSkillTree::GetSkillDesc( CX2SkillTree::SKILL_ID eSkillID, bool bEquipped /*= false*/ )
{
	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();

	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
	int iSkillLevel = refUserSkillTree.GetSkillLevel( eSkillID );
	//int iUnitclass = (int) g_pData->GetMyUser()->GetSelectUnit()->GetClass(); /
	int iUnitclass = 0; // oasis
	
	//{{  [11/11/2009 : oasis907 ]
	// ��� ��ų ���� ���������� ���� ���� X
	/*
	if( g_pX2Game != NULL )
	{
		int levelUpNum = g_pX2Game->GetMyUnit()->GetSkillLevelUpNum( eSkillID );
		iSkillLevel = iSkillLevel + levelUpNum;
	}
	*/
	
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, (iSkillLevel != 0 ? iSkillLevel : 1) );
	
	if(pSkillTree == NULL || pSkillTemplet == NULL)
		return L"";

	wstringstream wstrm;
	wstrm << pSkillTemplet->m_wstrName << L" [";

	switch(pSkillTemplet->m_eType)
	{	
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
		{
			wstrm << GET_STRING(STR_ID_2672);		// ����� ��Ƽ��

		} break;
	case CX2SkillTree::ST_ACTIVE:
		{
			wstrm << GET_STRING(STR_ID_2673);		// ��Ƽ��

		} break;
	case CX2SkillTree::ST_BUFF:
		{
			wstrm << GET_STRING(STR_ID_2674);		// ����

		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		{
//			wstrm << L"�ο� ��ų, String �߰� ���"; // GET_STRING(STR_ID_2674);		// ����
			wstrm << GET_STRING(STR_ID_2673);		// ��Ƽ��
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

	default:
		{
			wstrm << GET_STRING(STR_ID_2675);		// �нú�

		} break;
	}
	wstrm << L"]\n";





	// ���ε� ��ų
	if( true == pSkillTemplet->m_bBornSealed && 
		false == refUserSkillTree.IsSkillUnsealed( eSkillID ) )
	{
		wstrm << L"\n[" << GET_STRING(STR_ID_3858) << L"]\n\n";
		wstrm << GET_STRING(STR_ID_3857);
		return wstrm.str();
	}






	if( bEquipped == false )
	{
		wstrm << L"(" << iSkillLevel << L"/" << pSkillTree->GetMaxGuildSkillLevel( iUnitclass, eSkillID ) << L")" << L"\n";
		wstrm << L"\n";
	}

	if(refUserSkillTree.DoIHaveThisSkill( eSkillID ))
	{
		if(pSkillTemplet->m_bShowSkillLevel == true)
		{
			if( bEquipped == false )
			{
				wstrm << GET_STRING(STR_ID_2647) << L"\n";		// ���� ����
			}
			wstrm << GET_STRING(STR_ID_489) << L" " << iSkillLevel << L"\n";	// Lv %d
		}

		wstrm << L"#CFFFF00";
		wstrm << pSkillTemplet->m_wstrMainDesc << L"\n";
		wstrm << L"\n";

		if( pSkillTemplet->m_fMPConsumption > 0 )
		{
			wstrm << GET_STRING(STR_ID_2648) << pSkillTemplet->m_fMPConsumption << L"\n";		// �Ҹ� MP
		}		
		if( pSkillTemplet->m_fSkillCoolTime > 0 )
		{
			wstrm <<  GET_REPLACED_STRING( ( STR_ID_315, "i", (int)ceil(pSkillTemplet->m_fSkillCoolTime) ) );		// ��Ÿ�� %d��
		}
		
		wstrm << L"#CX";
	}

	if(false == refUserSkillTree.IsMaxSkillLevel( eSkillID ) && false == bEquipped )
	{
		if(refUserSkillTree.DoIHaveThisSkill( eSkillID ))
		{
			wstrm << L"\n\n";
		}

		const CX2SkillTree::SkillTemplet* pNextSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iSkillLevel+1 );

		wstrm << GET_STRING(STR_ID_2649) << L"\n";		// ���� ����(����� ����)
		wstrm << L"#CC0C0C0";
		wstrm << pNextSkillTemplet->m_wstrMainDesc << L"\n";
		wstrm << L"\n";

		if( pNextSkillTemplet->m_fMPConsumption > 0 )
		{
			wstrm << GET_STRING(STR_ID_2648) << pNextSkillTemplet->m_fMPConsumption << L"\n";		// �Ҹ� MP
		}				
		if( pNextSkillTemplet->m_fSkillCoolTime > 0 )
		{
			wstrm <<  GET_REPLACED_STRING( ( STR_ID_315, "i", (int)ceil(pNextSkillTemplet->m_fSkillCoolTime) ) );		// ��Ÿ�� %d��
		}
		wstrm << L"#CX";
	}

	if( false == bEquipped && false == refUserSkillTree.CanILearnThisSkill( eSkillID, false ) )
	{
		const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetGuildSkillTreeTempletMap( iUnitclass );
		CX2SkillTree::SkillTreeTempletMap::const_iterator it = refSkillTreeTempletMap.find(eSkillID);
		if( it != refSkillTreeTempletMap.end() )
		{		
			const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = it->second;

			if(refSkillTreeTemplet.m_vecPrecedingSkill.empty() == false)
			{
				bool bPreMastered = true;
				for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
				{	
					CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
					if(false == refUserSkillTree.IsMaxSkillLevel( PreccedSkillID ))
					{
						bPreMastered = false;
					}
				}

				if(bPreMastered == false)
				{
					wstrm << L"#CE00000" << L"\n"<< GET_STRING(STR_ID_2671) << L"#CX\n";			// ���� ��ų
					bool bFirst = true;
					wstrm << L"#CE00000";
					for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
					{	
						CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
						if(bFirst == false)
						{
							wstrm<< L", ";
						}

						const CX2SkillTree::SkillTemplet* pPreccedSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( PreccedSkillID, 1 );
						if(pPreccedSkillTemplet != NULL)
						{
							wstrm << pPreccedSkillTemplet->m_wstrName;
						}
					}
					wstrm << GET_STRING(STR_ID_2676) << L"#CX";		// �� �����Ͱ� �ʿ��մϴ�.
				}
			}
		}
	}

	return wstrm.str();
}

#endif // UPGRADE_SKILL_SYSTEM_2013


void CX2UIGuildSkillTree::ShowUIDesc( bool bShow, wstring wstr /*= L""*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT /*= CX2TalkBoxManagerImp::TBT_FROM_UP_RIGHT*/, D3DXCOLOR coTextColor /*= D3DXCOLOR(0,0,0,1)*/, D3DXCOLOR coBackColor /*= D3DXCOLOR(1, 0.96f, 0.6f, 1 )*/)
{
	m_pTalkBoxManager->Clear();

	if(bShow)
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;

		talkBox.m_vPos				= pos;
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
		talkBox.m_fRemainTime		= 2.0f;

		talkBox.m_wstrTalkContent	= wstr.c_str();
		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= TBT;
		talkBox.m_coTextColor		= coTextColor;
		talkBox.m_BackTexColor		= coBackColor;
	
		m_pTalkBoxManager->Push( talkBox );

	}
	
}





void CX2UIGuildSkillTree::OpenDLGSkillUnsealed( bool bOpen )
{
	if( false == bOpen )
	{
		if( m_pDLGSkillUnsealed != NULL )
		{
			m_pDLGSkillUnsealed->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( m_pDLGSkillUnsealed == NULL )
		{
			m_pDLGSkillUnsealed = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Acquisition.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealed );
		}

		m_pDLGSkillUnsealed->SetShowEnable( bOpen, bOpen );
	}
}


void CX2UIGuildSkillTree::UpdateDLGSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID )
{
	if( m_pDLGSkillUnsealed == NULL )
		return;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( NULL == pSkillTemplet )
		return;
	

	CKTDGUIStatic* pStaticSkillSlot = (CKTDGUIStatic*) m_pDLGSkillUnsealed->GetControl( L"g_pStaticSkill_Acquisition_Slot" );
	if( NULL != pStaticSkillSlot &&
		NULL != pStaticSkillSlot->GetPicture(1) )
	{
		std::string textureName = "";
		std::string texturePieceName = "";

		ConvertWCHARToChar( textureName, pSkillTemplet->m_wstrIconName.c_str() );
		ConvertWCHARToChar( texturePieceName, pSkillTemplet->m_wstrIconPieceName.c_str() );

		pStaticSkillSlot->GetPicture(1)->SetTex_LUA( textureName.c_str(), texturePieceName.c_str() );
	}


	CKTDGUIStatic* pStaticSkillName = (CKTDGUIStatic*) m_pDLGSkillUnsealed->GetControl( L"Static_Skill_Name" );
	if( NULL != pStaticSkillName )
	{
		pStaticSkillName->GetString(0)->msg = pSkillTemplet->m_wstrName;
	}

}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	
/** @function	: GetSkillDesc
	@brief		: ��ų ȿ���� ǥ���ϴ� ���� ���� ����
	@param		: ��Ʈ�� ������, ��ų ����, �߰� ��ų ����, ��ų ���ø�
*/
void CX2UIGuildSkillTree::GetSkillEffectiveDesc( OUT wstring& wstrDesc, IN int iSkillLevel, IN const CX2SkillTree::SkillTemplet* pSkillTemplet ) const
{
	if ( NULL == pSkillTemplet ||
		 NULL == g_pData ||
		 NULL == g_pData->GetSkillTree() )
		return;

	wstrDesc += L"\n";

	if ( 0 < iSkillLevel )		/// ��ų ������ 0���� Ŭ ����, ���� ���� ��ų ȿ�� ���
	{
		wstrDesc += L"\n";

		wstrDesc += GET_STRING( STR_ID_25071 );	/// <���� ����>

		wstrDesc += L"\n";

		wstrDesc += pSkillTemplet->GetSkillEffectiveDescValue( iSkillLevel );			/// ���� ������ ���� ��ų ȿ�� ���� ����
	}


	if ( MAX_GUILD_SKILL_LEVEL > iSkillLevel )		/// ���� ��ų ������ �ִ� ��ų ���� �Ѱ躸�� ���� ��, ���� ���� ǥ��
	{
		if ( 0 < iSkillLevel )	/// ���� ���� ǥ�� �Ǿ����� ��ĭ �߰��� �������
			wstrDesc += L"\n";

		wstrDesc += L"\n#C878787";			/// ��Ʈ�� ȸ�� ó�� ( R:135, G: 135, B: 135 )

		wstrDesc += GET_STRING( STR_ID_25375 );				/// <���� ����>

		wstrDesc += L"\n";

		/// ���� ������ ���� ��ų ȿ�� ���� ���� ( �߰��Ǵ� ���� ȿ���� �ִٸ�, �ش� ������ �����Ͽ� ��ȯ )
		wstrDesc += pSkillTemplet->GetSkillEffectiveDescValue( iSkillLevel + 1 );

		wstrDesc += L"#CX";
	}

#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
	/// ������ ��� - ��ų ���̵� ���
	if( true == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
	{
		WCHAR buf[256] = {0,};
		StringCchPrintf( buf, 255, L"\n\n#C00FF00Skill ID : %d#CX", static_cast<int>( pSkillTemplet->m_eID ) );
		
		wstrDesc += buf;
	}
#endif // _SERVICE_
}

/** @function	: GetPrecedingSkillDesc
	@brief		: ���� ��ų�� ���� ���� ����
	#param		: ���� ��Ʈ��, ���� ��ų ���̵�
*/
void CX2UIGuildSkillTree::GetPrecedingSkillDesc( OUT wstring& wstrDesc, CX2SkillTree::SKILL_ID ePrecedingSkill ) const
{
	if ( NULL == g_pData ||
		 NULL == g_pData->GetSkillTree() ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return;
	}

	 const CX2UserSkillTree& userSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;

	/// ���� ��ų�� ����
	const int iPrecedingSkillLevel = userSkillTree.GetSkillLevel( ePrecedingSkill );

	if ( 0 >= iPrecedingSkillLevel )		/// ���� ��ų�� ����� �ʾ��� ����, ���� ��ų �ȳ� �˾� ����
	{
		const CX2SkillTree::SkillTemplet* pPrecedingSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( ePrecedingSkill );

		if ( NULL != pPrecedingSkillTemplet )
		{
			WCHAR buf[256] = {0,};
			StringCchPrintf( buf, 256, L"\n#CFF0000%s#CX\n\n", GET_REPLACED_STRING( ( STR_ID_25114, "L", pPrecedingSkillTemplet->m_wstrName ) ) );	/// ���ེų : @1 Lv.1 �ʿ�
			wstrDesc += buf;
		}
	}
}

void CX2UIGuildSkillTree::GetSkillTitleDesc( OUT wstring& wstrTitleDesc, IN const CX2SkillTree::SkillTemplet* pSkillTemplet, 
										   IN const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet, IN int iSkillLevel ) const
{
	if( NULL == pSkillTemplet ||
		NULL == pSkillTreeTemplet ||
		NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() ||
		NULL == g_pData->GetSocketItem() ||
		NULL == g_pData->GetSkillTree() )
		return;

	wstrTitleDesc = pSkillTemplet->m_wstrName;		/// ��ų��

	wstrTitleDesc += L"\n";

	/// ��ų ����
	wstring wstrLevelTitle = L"";

	/// ���� ������ ������ ��, ���� �ڿ� (������) �߰�
	if ( pSkillTreeTemplet->m_iMaxSkillLevel <= iSkillLevel )
		wstrLevelTitle = GET_STRING(STR_ID_25113);

	WCHAR buf[256] = {0,};

	StringCchPrintf( buf, 256, L"%s.%d #CFF0000%s#CX\n", GET_STRING(STR_ID_489), iSkillLevel, wstrLevelTitle.c_str() );

	wstrTitleDesc += buf;
}

/** @function	: GetSkillSubDesc
	@brief		: ��ų Ÿ��, �Ҹ� MP, ���� �ð��� ǥ���ϴ� ���� ���� ����
	@param		: ��ų ���ø�, ��Ʈ�� ������
*/
void  CX2UIGuildSkillTree::GetSkillSubDesc( IN const CX2SkillTree::SkillTemplet* pSkillTemplet, OUT wstring& wstrSubDesc ) const
{
	if( NULL == pSkillTemplet ||
		NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return;

	const CX2UserSkillTree&	refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;	/// ������ ��ų ��ü

	bool bIsPassive = false;	/// �нú� ��ų ���� ( �нú� ��ų�� ���� �ð� 0�϶�, ��ǥ�� )

	/// ��ų Ÿ��
	WCHAR buf[256] = {0,};

	switch(pSkillTemplet->m_eType)
	{	
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2672) );		/// ����� ��Ƽ��

		} break;
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING( STR_ID_26134 ) );		/// ������ ��Ƽ��

		} break;
#endif //FINALITY_SKILL_SYSTEM
	case CX2SkillTree::ST_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2673) );		/// ��Ƽ��

		} break;
	case CX2SkillTree::ST_BUFF:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2674) );		/// ����

		} break;
	default:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2675) );		/// �нú�
			bIsPassive = true;		/// �нú� ��ų��
		} break;
	}
	wstrSubDesc = buf;	/// ��ų Ÿ�� ����

	const int iSkillLevelRight = max( 1, refUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );						/// ��ų ����

	/// �Ҹ� MP
	const int iMPConsumption = static_cast<int>( pSkillTemplet->GetSkillMPConsumptionValue( iSkillLevelRight ) );	/// �Ҹ� MP ��

	/// �нú� ��ų�̰� �Ҹ� MP�� �ְų�, �нú� ��ų�� �ƴ� �� �Ҹ� MP ǥ��
	if ( ( true == bIsPassive && 0 < iMPConsumption ) || false == bIsPassive )
	{
		StringCchPrintf( buf, 256, L"\n%s%d", GET_STRING(STR_ID_2648), static_cast<int>( pSkillTemplet->GetSkillMPConsumptionValue( iSkillLevelRight ) ) );
		wstrSubDesc += buf;
	}

	/// ���� �ð�
	const int iSkillCoolTime = static_cast<int>( pSkillTemplet->GetSkillCoolTimeValue( iSkillLevelRight ) );	/// ���� �ð� ��

	/// �нú� ��ų�̰� ���� �ð��� �ְų�, �нú� ��ų�� �ƴ� �� ���� �ð� ǥ��
	if ( ( true == bIsPassive && 0 < iSkillCoolTime ) || false == bIsPassive )
	{
		wstrSubDesc += L"\n";
		wstrSubDesc += GET_REPLACED_STRING( ( STR_ID_25070, "i", static_cast<int>( pSkillTemplet->GetSkillCoolTimeValue( iSkillLevelRight ) ) ) );	/// ���� �ð� @1
	}
}

#endif // UPGRADE_SKILL_SYSTEM_2013

#endif GUILD_SKILL

#endif NEW_SKILL_TREE_UI






