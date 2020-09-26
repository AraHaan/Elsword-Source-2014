#include "StdAfx.h"

#ifndef UPGRADE_SKILL_SYSTEM_2013

#ifdef NEW_SKILL_TREE_UI

#include ".\x2uiskilltree.h"

const int MAGIC_SKILL_DESC_WIDTH = 270;

//////////////////////////////////////////////////////////////////////////
/// CX2SkillTreeSlot : ��ų Ʈ���� ������ ����
//////////////////////////////////////////////////////////////////////////
CX2SkillTreeSlotData::CX2SkillTreeSlotData()
{
	m_eSlotDataType = SDT_SKILL_TREE;
	m_eSkillID = CX2SkillTree::SI_NONE;
}
/*virtual*/ CX2SkillTreeSlotData::~CX2SkillTreeSlotData()
{
	

}
/*virtual*/ void CX2SkillTreeSlotData::UpdateUI( CKTDGUISlot* pSlot )
{    
	// 1. ���� ���� �� �ִ� ��ų ���� : ��ư�� Ȱ��ȭ/��Ȱ��ȭ. bool ���� ����
	// 2. 1�����̶� ���� ��ų ���� : ��ų ������ �巡�� Ȱ��ȭ / ��Ȱ��ȭ(�нú�� �巡�� �� �ǰ�), �ؽ��� ����. ��Ʈ�� ���� (����/�ִ뷹��)

	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();

	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
	int iSkillLevel = refUserSkillTree.GetSkillLevel( m_eSkillID );
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID, (iSkillLevel != 0 ? iSkillLevel : 1) );
	if(pSkillTree == NULL || pSkillTemplet == NULL)
		return;

	const map<CX2SkillTree::SKILL_ID, CX2UISkillTree::SkillSlotUI>& refmapSkillSlotUISet = g_pData->GetUIManager()->GetUISkillTree()->GetMapSkillSlotUISet();
	map<CX2SkillTree::SKILL_ID, CX2UISkillTree::SkillSlotUI>::const_iterator it = refmapSkillSlotUISet.find(m_eSkillID);
	if(it == refmapSkillSlotUISet.end())
	{
		return;
	}
	const CX2UISkillTree::SkillSlotUI& refSkillSlotUI = it->second;

	bool bRemoveMode = g_pData->GetUIManager()->GetUISkillTree()->GetRemoveMode();
	

#ifdef ADDED_RELATIONSHIP_SYSTEM
	// �ο� ��ų�� ���� ���� ó��
	if ( pSkillTemplet->m_eType == CX2SkillTree::ST_RELATIONSHIP_SKILL )
	{
		int iRelationshipLevel = 0; // ���� �ο� �ܰ�, 0 = �ַ�, 1 = Ŀ��, 2 = ��ȥ
		
		// 1�ܰ�, �ʱ�ȭ �� ��ų ���� ǥ��
		if ( NULL != g_pData->GetRelationshipManager() &&
			 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
			{
				case SEnum::RT_SOLO :
					{
						iRelationshipLevel = 0;
						// �������� �ο� ��ų ���� ����
						if( NULL != g_pData && NULL != g_pData->GetSkillTree() )
						{
							g_pData->GetSkillTree()->UnequipRelationSkill();
						}
					}
					break;
				case SEnum::RT_COUPLE :
					iRelationshipLevel = 1;
					break;

				case SEnum::RT_MARRIED :
					iRelationshipLevel = 2;
					break;
			}
				
		}
		// �ο� �ܰ�, �ַ� �� ���� ��� �Ұ�, �ڹ���
		// �ο� �ܰ�, Ŀ�� �� ���� ��� ����, Level 1
		// �ο� �ܰ�, ��ȥ �� ���� ��� ����, Level 2

		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );
		// �ؽ�ó ����,

		refUserSkillTree.SetSkillLevelAndCSP( m_eSkillID, iRelationshipLevel, 0 );

//		if(true == refUserSkillTree.DoIHaveThisSkill(m_eSkillID)) // �� ��ų�� ���� �ִٸ�..
																  // ��� ĳ���ʹ� �ο� ��ų�� ���� �־����
		{
			// Ŀ�� �̻� �ܰ��� ���� "Lv. 1" Static �� Drag �� ���´�.
			// RButtonUp ���� ��ų �ִ� �κ��� �ش� �޽������� �ٽ� �ڵ�
			// �׳� ���¿����� Drag �Ұ�
			if( iRelationshipLevel > 0 )		
			{
				ShowSlotPicture( true, pSlot, STSAPT_LEVEL, iRelationshipLevel );
				pSlot->SetDragable( true );
			}
			else
			{
				pSlot->SetDragable( false );
			}
		}



		// 2�ܰ�, ��ų ������ ������Ʈ
		// ȹ�� ����, �� ������ �Ⱥ�����	
		// ��� �Ұ�, �� ��Ȳ�� ���� ������
		ShowSlotPicture( false, pSlot, STSAPT_CANLEARN ); 

		if ( iRelationshipLevel > 0 )
		{
			ShowSlotPicture( false, pSlot, STSAPT_DISABLE );
		}
		else
		{
			ShowSlotPicture( true, pSlot, STSAPT_DISABLE );
		}

		if( -1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, false ) || 
			-1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, true ) )
		{
			// ��ų�� ���� ���̸�
			ShowSlotPicture( true, pSlot, STSAPT_EQUIPPED );
		}
		else
		{
			ShowSlotPicture( false, pSlot, STSAPT_EQUIPPED );
		}

		refSkillSlotUI.m_pStaticRelationType->SetShow ( true );
		refSkillSlotUI.m_pButtonMinus->SetShow(false);
		refSkillSlotUI.m_pButtonMaster->SetShow(false);
		refSkillSlotUI.m_pButtonPlus->SetShow(false);
#ifdef GUIDE_SKILL_LEVEL_MINUS
		refSkillSlotUI.m_pButtonMinusGuide->SetShowEnable(false, false);
#endif // GUIDE_SKILL_LEVEL_MINUS

		// 3�ܰ�, ��ų ���� ������Ʈ

		CKTDGUIControl::UIStringData* pString = refSkillSlotUI.m_pStaticRelationType->GetString (0);
		if(pString != NULL)	
		{
			int level = refUserSkillTree.GetSkillLevel(m_eSkillID);
			wstring wstrRelationState = L"";
			// �ο� ��ų�� Level �� 0 �� ��, �ַ�, 1 �� �� Ŀ��, 2 �� �� �κ�

			switch ( level ) 
			{
			case 0 :
				wstrRelationState = GET_STRING ( STR_ID_24460 );
				break;
			case 1 :
				wstrRelationState = GET_STRING ( STR_ID_24461 );		
				break;
			case 2:
				wstrRelationState = GET_STRING ( STR_ID_24462 );
				break;

			}			
			pString->msg = wstrRelationState;
		}

		// 4�ܰ�, ȭ��ǥ ������Ʈ
		// �ο� ��ų�� ȭ��ǥ�� �����ϴ�!!! Ȥ �߰��ȴٸ� ���� �߰����ּ���!!


		// 5�ܰ�, �ڹ��� ������Ʈ

		if ( iRelationshipLevel > 0 )
			refSkillSlotUI.m_pStaticLock->SetShowEnable( false, false );

		else
			refSkillSlotUI.m_pStaticLock->SetShowEnable( true, true );



	}

	else
#endif // ADDED_RELATIONSHIP_SYSTEM
	{
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
					{
						pSlot->SetDragable(true);
					} break;
				default:
					{
						pSlot->SetDragable(false);
					} break;
				}
				if(pSkillTemplet->m_bShowSkillLevel)
					ShowSlotPicture( true, pSlot, STSAPT_LEVEL, refUserSkillTree.GetSkillLevel(m_eSkillID) );
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

			if( -1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, false ) || 
				-1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, true ) )
			{
				// ��ų�� ���� ���̸�
				ShowSlotPicture( true, pSlot, STSAPT_EQUIPPED );
			}
			else
			{
				ShowSlotPicture( false, pSlot, STSAPT_EQUIPPED );
			}

		
			// 2. ��ư ������Ʈ : refUserSkilLTree�� ������ �������� �Ѵ�.

			{
				if(false == bRemoveMode)
				{		
					refSkillSlotUI.m_pButtonPlus->SetShow(true);
					refSkillSlotUI.m_pButtonMinus->SetShow(false);
					refSkillSlotUI.m_pButtonMaster->SetShow(false);
		#ifdef GUIDE_SKILL_LEVEL_MINUS
					refSkillSlotUI.m_pButtonMinusGuide->SetShowEnable(false, false);
		#endif GUIDE_SKILL_LEVEL_MINUS

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
		#ifdef GUIDE_SKILL_LEVEL_MINUS
						refSkillSlotUI.m_pButtonMinusGuide->SetShowEnable(false, false);
		#endif GUIDE_SKILL_LEVEL_MINUS
					}
					else
					{
						refSkillSlotUI.m_pButtonMinus->SetEnable(false);
		#ifdef GUIDE_SKILL_LEVEL_MINUS
						refSkillSlotUI.m_pButtonMinusGuide->SetShowEnable(true, true);
		#endif GUIDE_SKILL_LEVEL_MINUS
					}
				}
			}

			// 3. ��ų ���� ������Ʈ
			CKTDGUIControl::UIStringData* pString = refSkillSlotUI.m_pStaticLevelString->GetString(0);
			if(pString != NULL)
			{
				int level = refUserSkillTree.GetSkillLevel(m_eSkillID);
				int maxlevel = pSkillTree->GetMaxSkillLevel( g_pData->GetMyUser()->GetSelectUnit()->GetClass(), m_eSkillID );

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
				g_pData->GetUIManager()->GetUISkillTree()->SetArrowColor(m_eSkillID, D3DXCOLOR(1,1,1,1));
			}
			else
			{
				g_pData->GetUIManager()->GetUISkillTree()->SetArrowColor(m_eSkillID, D3DXCOLOR(0.5f,0.5f,0.5f,1));
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

			{
				// 2. ��ư ������Ʈ
				refSkillSlotUI.m_pButtonPlus->SetShowEnable(true, false);
				refSkillSlotUI.m_pButtonMinus->SetShow(false);
				refSkillSlotUI.m_pButtonMaster->SetShow(false);
	#ifdef GUIDE_SKILL_LEVEL_MINUS
				refSkillSlotUI.m_pButtonMinusGuide->SetShowEnable(false, false);
	#endif GUIDE_SKILL_LEVEL_MINUS

	#ifdef ADDED_RELATIONSHIP_SYSTEM
				refSkillSlotUI.m_pStaticRelationType->SetShow ( false );
	#endif // ADDED_RELATIONSHIP_SYSTEM

				// 3. ��ų ���� ������Ʈ
				CKTDGUIControl::UIStringData* pString = refSkillSlotUI.m_pStaticLevelString->GetString(0);
				if(pString != NULL)
				{
					int maxlevel = pSkillTree->GetMaxSkillLevel( g_pData->GetMyUser()->GetSelectUnit()->GetClass(), m_eSkillID );

					WCHAR buf[32] = {0,};
					//wsprintf( buf, L"0/%d", maxlevel );
					StringCchPrintf( buf, 32, L"0/%d", maxlevel );
					pString->msg = buf;
				}

			}
		
			// 4. ȭ��ǥ ������Ʈ
			g_pData->GetUIManager()->GetUISkillTree()->SetArrowColor(m_eSkillID, D3DXCOLOR(0.5f,0.5f,0.5f,1));
		
			// 4. ��ų ���� ���ĸ� �־�����!
			ShowSlotPicture( false, pSlot, STSAPT_LEVEL );
		
			// 5. ����ǥ�� �ڹ���
			refSkillSlotUI.m_pStaticLock->SetShowEnable( true, true );

		}
	}

	// �̴Ͻ��� ����������
	if(refUserSkillTree.IsMaxSkillLevel(m_eSkillID))
	{
		g_pData->GetUIManager()->GetUISkillTree()->SetMiniSkillSlotColor(m_eSkillID, D3DXCOLOR(1.f,1.f,0.0f,1));
	}
	else if ( refUserSkillTree.DoIHaveThisSkill( m_eSkillID ))
	{
		g_pData->GetUIManager()->GetUISkillTree()->SetMiniSkillSlotColor(m_eSkillID, D3DXCOLOR(1.f,1.f,1.f,1));
	}
	else if( refUserSkillTree.CanILearnThisSkill( m_eSkillID, false ) )
	{
		g_pData->GetUIManager()->GetUISkillTree()->SetMiniSkillSlotColor(m_eSkillID, D3DXCOLOR(0.7f,0.7f,0.7f,1));
	}
	else
	{
		g_pData->GetUIManager()->GetUISkillTree()->SetMiniSkillSlotColor(m_eSkillID, D3DXCOLOR(0.0f,0.0f,0.0f,1));
	}
	
}

void CX2SkillTreeSlotData::ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_TREE_SLOT_ADD_PICTURE_TYPE eType, int val /*= -1*/ )
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
/// CX2EquippedSkillSlot : ���� ������� ��ų ����
//////////////////////////////////////////////////////////////////////////
CX2EquippedSkillSlotData::CX2EquippedSkillSlotData()
{
	m_eSlotDataType = SDT_SKILL_EQUIP;
	m_bDataUpdated	= true;
	m_eSkillID		= CX2SkillTree::SI_NONE;
	m_iIndex = -1;
	m_bSlotb = false;
	m_bDataUpdated = true;
}
CX2EquippedSkillSlotData::CX2EquippedSkillSlotData(int index, bool bSlotb )
{
	m_eSlotDataType = SDT_SKILL_EQUIP;
	m_bDataUpdated	= true;
	m_eSkillID		= CX2SkillTree::SI_NONE;
	m_iIndex		= index;
	m_bSlotb		= bSlotb;
	m_bDataUpdated = true;

}
/*virtual*/ CX2EquippedSkillSlotData::~CX2EquippedSkillSlotData()
{

}
/*virtual*/ void CX2EquippedSkillSlotData::UpdateUI( CKTDGUISlot* pSlot )
{
	// 1. ���� ���� �� �ִ� ��ų ���� : ��ư�� Ȱ��ȭ/��Ȱ��ȭ. bool ���� ����
	// 2. 1�����̶� ���� ��ų ���� : ��ų ������ �巡�� Ȱ��ȭ / ��Ȱ��ȭ(�нú�� �巡�� �� �ǰ�), �ؽ��� ����. ��Ʈ�� ���� (����/�ִ뷹��)


	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;


	if(m_bSlotb == true && false == refUserSkillTree.GetEnabledSkillSlotB() )
	{
		if(m_eSkillID != CX2SkillTree::SI_NONE)
		{
			m_eSkillID = CX2SkillTree::SI_NONE;
			pSlot->SetItemTex( L"DLG_Common_New_Texture03.TGA", L"invisible" );
			pSlot->SetDragable(false);
			ShowSlotPicture(false, pSlot, SESAPT_LEVEL );
		}
		if(true == pSlot->GetEnable())
		{			
			ShowSlotPicture(true, pSlot, SESAPT_DISABLE );
			pSlot->SetEnable(false);
		}		
		return;
	}
	else
	{
		ShowSlotPicture(false, pSlot, SESAPT_DISABLE );
		pSlot->SetEnable(true);
	}	

	
	const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot( m_iIndex, m_bSlotb);
	m_eSkillID = pSkillData->m_eID;

	int iSkillLevel = refUserSkillTree.GetSkillLevel( m_eSkillID );
	if(iSkillLevel == 0 && m_eSkillID != CX2SkillTree::SI_NONE)
	{
		ASSERT( !"Skill Level 0 Equipped!" );
	}
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID, iSkillLevel );
	if( pSkillTemplet != NULL)
	{
		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );		
		pSlot->SetDragable(true);
		if(pSkillTemplet->m_bShowSkillLevel)
		{
			int ilevelUpNum = 0;
			if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			{
				ilevelUpNum = g_pX2Game->GetMyUnit()->GetSkillLevelUpNum( pSkillData->m_eID );
			}
			ShowSlotPicture(true, pSlot, SESAPT_LEVEL, min(iSkillLevel + ilevelUpNum, 10), ( ilevelUpNum != 0 ));
		}
		else
			ShowSlotPicture(false, pSlot, SESAPT_LEVEL);

#ifdef FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG	/// �б����� ������ ���� ��, CKTDGUISlot �� OnFrameMove Ÿ�鼭 ������ �ʱ�ȭ �ǹ����� ���� ����
		if( NULL != g_pX2Game && CX2SkillTree::SI_A_EEL_SPECTRO_EL_CRYSTAL == pSkillTemplet->m_eID )	/// �б����� ��ų�� ��, ������ ���Ž�������
		{
			g_pX2Game->UpdateEveElectraSkillSlotIcon();
		}
#endif FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG
	}
	else
	{
		// ������ ��ų�� ���� ���� ó��
		pSlot->SetItemTex( L"DLG_Common_New_Texture03.TGA", L"invisible" );
		ShowSlotPicture(false, pSlot, SESAPT_LEVEL);
		pSlot->SetDragable(false);
	}
}

void CX2EquippedSkillSlotData::ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_EQUIPPED_SLOT_ADD_PICTURE_TYPE eType, int val /*= -1*/ , bool bIsSkillLevelUpByItem/* = false*/)
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
			case SESAPT_DISABLE:
				{
					pPicture->SetTex( L"HQ_ImpossibleEqip.tga" );			
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,0.3f );
				} break;

			case SESAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

					if( true == bIsSkillLevelUpByItem )
					{
						pPicture->SetColor( D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.f));
					}
					else
					{
						pPicture->SetColor( D3DXCOLOR(1, 1, 1, 1));
					}

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
			case SESAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );


					if( true == bIsSkillLevelUpByItem )
					{
						pPicture->SetColor( D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.f));
					}
					else
					{
						pPicture->SetColor( D3DXCOLOR(1, 1, 1, 1));
					}
				} break;
			default:
				break;
			}

		}
		pPicture->SetShow(bShow);
	}

}

int CX2EquippedSkillSlotData::GetSlotID()		// 0~8�� ���Ǵ� ID�� �����ش�
{
	if(m_bSlotb)
	{
		return m_iIndex + EQUIPPED_SKILL_SLOT_COUNT;
	}
	else
	{
		return m_iIndex;
	}
}


//////////////////////////////////////////////////////////////////////////


CX2UISkillTree::CX2UISkillTree( CKTDXStage* pNowStage ) :
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
m_pUserSkillTree(NULL),
m_SumDelta(0),
m_iPrevSP(-1),
m_fNaviMoveRange(0.f),
m_pTalkBoxManager(NULL)
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
, m_pDLGFirstGuide(NULL)
#endif
#ifdef REFORM_UI_SKILLSLOT
, m_bIsExpandSkillSlot( false )
, m_pDLGGameSkillSlot (NULL)
, m_bShowEquipSkillGuide (false)
#endif //REFORM_UI_SKILLSLOT
, m_bSkillSlotSwap( false )
#ifdef SKILL_SLOT_UI_TYPE_B 
, m_bIsSkillUITypeA( false )
#endif //SKILL_SLOT_UI_TYPE_B
{
	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );
	m_bUsingCSP = false;	// ����ϴ� ���� ���µ� �ѵ�.....
#ifdef REFORM_UI_SKILLSLOT
	m_pDLGGameSkillSlot	= new CKTDGUIDialog( pNowStage, L"DLG_GAME_SKILL_SLOT_NEW.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGameSkillSlot );
	m_pDLGGameSkillSlot->SetShow(false);
	UpdateSkillSlotString();
#endif //REFORM_UI_SKILLSLOT
}


CX2UISkillTree::~CX2UISkillTree(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUISkillTree );	
	SAFE_DELETE_DIALOG( m_pDLGSkillToolTip );
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );

	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealed );

#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
	SAFE_DELETE_DIALOG( m_pDLGFirstGuide );
#endif
#ifdef ELSWORD_WAY_OF_SWORD
	SAFE_DELETE_DIALOG( m_pDLGElswordWSSDesc );
#endif ELSWORD_WAY_OF_SWORD

#ifdef REFORM_UI_SKILLSLOT
	SAFE_DELETE_DIALOG( m_pDLGGameSkillSlot );
#endif //REFORM_UI_SKILLSLOT
	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );

	m_mapSkillSlotUISet.clear();
	m_mapArrowUISet.clear();

	m_mapMiniSkillSlotPictureIndex.clear();
	m_mapMiniArrowPictureIndex.clear();

}

void CX2UISkillTree::SetLayer( X2_DIALOG_LAYER layer )
{
	if(m_pDLGUISkillTree != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUISkillTree, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUISkillTree, false );
	}
}

void CX2UISkillTree::SetPosition( D3DXVECTOR2 vec )
{
	// ���� ���̾�α�
	m_pDLGUISkillTree->SetPos(vec);
	//m_MovedPosition = vec;
	
}

HRESULT CX2UISkillTree::OnFrameMove( double fTime, float fElapsedTime )
{	
	return S_OK;
}

bool CX2UISkillTree::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
#ifdef REFORM_UI_SKILLSLOT
			//��ų �巡�׽ÿ� �˸� �����ֵ��� ����
			for(map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.begin(); it != m_mapSkillSlotUISet.end(); ++it)
			{
				SkillSlotUI& refSkillSlotUI = it->second;
				if ( NULL != refSkillSlotUI.m_pSlot && NULL != refSkillSlotUI.m_pSlot->GetSlotData() )
				{
					if( refSkillSlotUI.m_pSlot->GetState() == CKTDGUISlot::SCS_ONDRAG && uMsg != WM_LBUTTONUP) //�巡�� ��
					{
						ShowSkillDesc(false);
						refSkillSlotUI.m_pSlot->SetGuideDescLT(false);
						m_bShowEquipSkillGuide = true;
						break;
					}	 
					m_bShowEquipSkillGuide = false;
				}
			}
#endif //REFORM_UI_SKILLSLOT
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
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
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_SKILL, true);
			bFlag = true;		
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

bool CX2UISkillTree::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		return false;

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

bool CX2UISkillTree::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case STUCM_UNSEAL_SKILL_CANCLE:
		{
			if( m_pDLGSkillUnsealInform != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSkillUnsealInform, NULL, false );
				m_pDLGSkillUnsealInform = NULL;
			}			
		} break;
	case STUCM_UNSEAL_SKILL_BUY:
		{
			//** ��ų ���� ��ư�� ������ �� ���⿡ �۾��� �ּ���.

		} break;
	case STUCM_CSP_GUIDE_MOUSEIN:
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
				refUserSkillTree.CalcUsedSPointAndCSPoint( iSPoint,iCSPoint );
				int iMaxCSPoint = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint + g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint 
					+ iSPoint + iCSPoint - g_pData->GetSelectUnitLevel() * 2;

				WCHAR wszText[32] = L"";
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"(%dSP)\n", iMaxCSPoint );
				
				wstring wstr = L"";
				wstr += GET_STRING(STR_ID_2689);
				wstr += wszText;
				wstr += GetExpirationDateDesc( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, g_pData->GetServerCurrentTime() );

				ShowUIDesc(true, wstr, pos, CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0.97f, 0.23f, 0.06f, 1) );			
			}
			
			return true;
		} break;
	case STUCM_CSP_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;
#ifndef REFORM_UI_SKILLSLOT
	case STUCM_SKILLTREESLOTB_GUIDE_MOUSEIN:
		{
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
		} break;
	case STUCM_SKILLTREESLOTB_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;
#endif //REFORM_UI_SKILLSLOT
#ifdef GUIDE_SKILL_SLOTB
	case STUCM_SKILLTREESLOTB_BUY_GUIDE_MOUSEIN:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f; 
			ShowUIDesc( true, GET_STRING(STR_ID_16097), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT ); 

			return true;
		} break;
	case STUCM_SKILLTREESLOTB_BUY_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break; 
#endif GUIDE_SKILL_SLOTB
#ifdef GUIDE_SKILL_LEVEL_MINUS
	case STUCM_BUTTON_SKILL_LEVEL_MINUS_GUIDE_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_16099), pos );
			return true;
		} break;
	case STUCM_BUTTON_SKILL_LEVEL_MINUS_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break; 
#endif GUIDE_SKILL_LEVEL_MINUS
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
	case STUCM_FIRST_GUIDE_CLOSE:
		{
			if(m_pDLGFirstGuide != NULL)
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGFirstGuide, NULL, false );
			}
			m_pDLGFirstGuide = NULL;
			return true;
		} break;
	//case STUCM_FIRST_GUIDE_DONOTSHOW:
	//	{
	//		CKTDGUICheckBox* pCheckbox = (CKTDGUICheckBox*) lParam;
	//		bool bDoNotShow = pCheckbox->GetChecked();

	//		if( true == bDoNotShow )
	//		{
	//			FILE* file = NULL;
	//			const string strFileName = "readme.txt";
	//			file = fopen( strFileName.c_str(), "w" );
	//			fclose(file);
	//		}
	//		else
	//		{
	//			WCHAR wszFileName[256] = {0,};
	//			//wsprintf( wszFileName, L"readme.txt" );
	//			StringCchPrintf( wszFileName, 256, L"readme.txt" );	
	//			::DeleteFile( wszFileName );
	//		}
	//		return true;
	//	} break;
#endif OPEN_SKILL_GUIDE_FIRST_TIME
#ifdef ELSWORD_WAY_OF_SWORD
	case STUCM_INGAME_ELSWORD_WSS_MOUSE_OVER:
		{
			if( CX2Unit::UT_ELSWORD == g_pData->GetMyUser()->GetSelectUnit()->GetType() 
#ifdef NEW_CHARACTER_EL // ���� ��
				|| CX2Unit::UT_ELESIS == g_pData->GetMyUser()->GetSelectUnit()->GetType() 
#endif // NEW_CHARACTER_EL		
				)
			{
				CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 vControlPos = pControl->GetPos();
				DrawElswordWSSDesc( vControlPos );
			}
			return true;
		} break;
	case STUCM_INGAME_ELSWORD_WSS_MOUSE_OUT:
		{
			if( m_pDLGElswordWSSDesc != NULL )
				m_pDLGElswordWSSDesc->SetShow(false);
			return true;
		} break;
#endif ELSWORD_WAY_OF_SWORD
	case STUCM_SKILL_RESET_MODE_CHANGE_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2667), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT);

			return true;
		} break;
	case STUCM_SKILL_RESET_MODE_CHANGE_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;
	case STUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2663), pos );

			return true;
		} break;
	case STUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2664), pos );

				return true;
		} break;
	case STUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_BUTTON_SKILL_MASTER_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2665), pos );

				return true;
		} break;
	case STUCM_BUTTON_SKILL_MASTER_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_NAVI_LEFT_CLICKED:
	case STUCM_NAVI_DRAGGING:
		{
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
			return true;		

		} break;
	case STUCM_CONTROLLIST_SCROLL:
		{
			UpdateBlind();
#ifndef REFORM_UI_SKILLSLOT
			//���� �� ������� ����.
			UpdateNavi();
#endif //REFORM_UI_SKILLSLOT
		
			return true;
		} break;
	case STUCM_SKILLTREESLOT_MOUSEIN:
		{
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2SkillTreeSlotData* pSlotData = (CX2SkillTreeSlotData*) pSlot->GetSlotData();
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
	case STUCM_EQUIPSLOT_MOUSEIN:
		{
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2SkillTreeSlotData* pSlotData = (CX2SkillTreeSlotData*) pSlot->GetSlotData();
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

		} break;
	case STUCM_SKILLTREESLOT_MOUSEOUT:
	case STUCM_EQUIPSLOT_MOUSEOUT:
		{			
			ShowUIDesc( false );
			ShowSkillDesc(false);
			return true;

		} break;
	case STUCM_SKILLTREESLOT_RMOUSEUP:
		{
		//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
		#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyBySkill();
					return true;
				}
			}
		#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // ���� ��ų ���� ����			

			return ProcessSkillTreeSlotRMouseUp( lParam );

		} break;

	case STUCM_EXIT:
		{
			SetShow(false);
			return true;
		} break;


	case STUCM_OPEN_COMBO_TREE:
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMBO_TREE );
			return true;
		} break;

	case STUCM_EQUIPSLOT_DROPPED:
		{
			if ( NULL != g_pX2Game )
			{
				if ( g_pX2Game->GetGameType() == CX2Game::GT_PVP ||
					 g_pX2Game->CheckAndWarningBusyStateNow() )
					return true;
			}

	//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
	#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyBySkill();
					return true;
				}
#ifdef REFORM_UI_SKILLSLOT
				UpdateSkillEquipedSlotUI();
#endif //REFORM_UI_SKILLSLOT
			}
	#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // ���� ��ų ���� ����

			return ProcessEquipSlotDropped( lParam );

		} break;
	case STUCM_BUTTON_SKILL_LEVEL_PLUS:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			int eSkillID = pControl->GetDummyInt(0);

			if( g_pData->GetSkillTree() != NULL)
			{
				g_pData->GetSkillTree()->Handler_EGS_GET_SKILL_REQ((CX2SkillTree::SKILL_ID)eSkillID );
			}
			return true;
		} break;
	case STUCM_BUTTON_SKILL_LEVEL_MINUS:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			int eSkillID = pControl->GetDummyInt(0);

			CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
			if( NULL != pMyUnit )
			{
				if(pMyUnit->GetInventory() != NULL)
				{
					if( NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_ITEM_ID ) ||
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
						NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_ITEM ) ||
#endif SERV_EVENT_RESET_A_SKILL_ITEM
						NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_EVENT_ITEM_ID ) ||
						NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_ITEM_NOT_TRADE_ID )
#ifdef SERV_QUEST_SKILL_RESET
						|| NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_QUEST_ITEM_ID )
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_2ND_CLASS_SKILL_RESET
						|| NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_2ND_CLASS_CHANGE_EVENT_ITEM_ID )
#endif SERV_2ND_CLASS_SKILL_RESET
						)

					{
						if(g_pData->GetSkillTree() != NULL)
						{
							g_pData->GetSkillTree()->Handler_EGS_RESET_SKILL_REQ((CX2SkillTree::SKILL_ID)eSkillID);
						}
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_2678 ), g_pMain->GetNowState() );
					}

				}
				
			}			
			ShowUIDesc( false ); // ������ // 2012-02-23 // ��ų �ǵ����� �˾�â ��ġ�� ���� ����
			return true;
		} break;
	case STUCM_EQUIPSLOT_RMOUSEUP:
		{
	//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
	#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyBySkill();
					return true;
				}
			}
	#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // ���� ��ų ���� ����

			// �ٻ� ���� üũ
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				return true;

#ifdef REFORM_UI_SKILLSLOT // ��ųâ �������� �� �� ������ ��ų ���� �� �� �ֵ��� ����
			if ( false == m_bShow ) 
			{
				return true;
			}
#endif //REFORM_UI_SKILLSLOT

			return ProcessEquipSlotRMouseUp( lParam );
		} break;
	case STUCM_SKILL_RESET_MODE_CHANGE:
		{
			m_bSkillRemoveMode = !m_bSkillRemoveMode;
			UpdateUI(false, false, true);
 
			return true;
		} break;
	default:
		break;
	}
	return false;
}

void CX2UISkillTree::SetShow(bool val)
{
	m_bShow = val;

	if(val)	// ���� �� ó���ؾ� �� �κ�
	{
#ifdef FIX_INTRUSION_SKILL_BUG
		//���� ���۽� ��ųâ �� �� ������ ����
		if( CX2Main::XS_PVP_ROOM == g_pMain->GetNowStateID())
		{
			CX2StatePVPRoom* pState = (CX2StatePVPRoom*) g_pMain->GetNowState();
			if( NULL != pState )
			{
				if( true == pState->GetDisableSkillUIOpen() )
				{
					m_bShow = false;
					return;
				}
			}
		}
#endif //FIX_INTRUSION_SKILL_BUG
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
		if( g_pData != NULL && g_pData->GetSelectUnitLevel() <= 10 )
		{
			const string strFileName = "readme.txt";
			FILE* file = NULL;
			file = fopen( strFileName.c_str(), "r" );		
			if( NULL == file )
			{
				file = fopen( strFileName.c_str(), "w" );
				fclose(file);

				if( m_pDLGFirstGuide == NULL )
				{
					m_pDLGFirstGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Help_Skill.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFirstGuide );
					m_pDLGFirstGuide->SetDisableUnderWindow(true);					
				}		
				m_pDLGFirstGuide->SetShow(true);
			}
			else
			{
				fclose(file);
			}		
		}
#endif OPEN_SKILL_GUIDE_FIRST_TIME

		m_bSkillRemoveMode = false;
		UpdateUI(true, true, true);


		
		CKTDGUIRadioButton* pRadioButton_ShowSkillTree = (CKTDGUIRadioButton*) m_pDLGUISkillTree->GetControl( L"RadioButton_Skill" );
		CKTDGUIRadioButton* pRadioButton_ShowComboTree = (CKTDGUIRadioButton*) m_pDLGUISkillTree->GetControl( L"RadioButton_Command" );

		if( NULL != pRadioButton_ShowSkillTree )
		{
			pRadioButton_ShowSkillTree->SetChecked( true );
		}
		if( NULL != pRadioButton_ShowComboTree )
		{
			pRadioButton_ShowComboTree->SetChecked( false );
		}



		CKTDGUIStatic* pStaticSkill = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"Static_BTN_SKILL" );
		if( NULL != pRadioButton_ShowSkillTree )
		{
			pRadioButton_ShowSkillTree->SetShowEnable( true, true );
		}
		if( NULL != pStaticSkill )
		{
			pStaticSkill->SetShowEnable( true, true );
		}

		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_SKILL );

#ifdef REFORM_UI_SKILLSLOT
		if( CX2Main::XS_PVP_ROOM == g_pMain->GetNowStateID() )
			g_pData->GetUIManager()->SetShowSkillSlot(true);
		
#endif //REFORM_UI_SKILLSLOT

#ifdef SKILL_SLOT_UI_TYPE_B
		SetSkillUIType(true);
#endif //SKILL_SLOT_UI_TYPE_B
	}
	else	// ���� �� ó���ؾ� �� �κ�
	{
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
		if(m_pDLGFirstGuide != NULL)
			m_pDLGFirstGuide->SetShow(false);
#endif
		ShowSkillDesc(false);
		ShowUIDesc(false);
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);

#ifdef REFORM_UI_SKILLSLOT
		m_bShowEquipSkillGuide = false;
#endif //REFORM_UI_SKILLSLOT		
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_SKILL);
		
#ifdef REFORM_UI_SKILLSLOT
		if( CX2Main::XS_PVP_ROOM == g_pMain->GetNowStateID() )
			g_pData->GetUIManager()->SetShowSkillSlot(false);
#endif //REFORM_UI_SKILLSLOT

#ifdef SKILL_SLOT_UI_TYPE_B //�ɼ����� ���� �ϰԵǸ� �ɼ��ʿ��� ���������� ����
		if( NULL != g_pMain )
		{
			SetSkillUIType(g_pMain->GetGameOption().GetIsSkillUITypeA());
		}
#endif //SKILL_SLOT_UI_TYPE_B
	}
	if( NULL != m_pDLGUISkillTree )
		m_pDLGUISkillTree->SetShowEnable(m_bShow, m_bShow);
}

// ĳ���Ͱ� ���õ��� �� / �������� ���� �� �ش�.
void CX2UISkillTree::InitSkillTreeUI()
{
	SAFE_DELETE_DIALOG(m_pDLGUISkillTree);
	
	m_bSkillSlotSwap = false;

	m_mapSkillSlotUISet.clear();
	m_mapArrowUISet.clear();
	m_pDLGUISkillTree = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Skill_Tree.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUISkillTree );
	m_pDLGUISkillTree->SetDisableUnderWindow(true);
	m_pDLGUISkillTree->SetShow(false);
#ifdef REFORM_UI_SKILLSLOT
#ifndef WORLD_TOOL
	CKTDGUIStatic* pStaticNotice = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"Notice" );
#ifdef HARDCODING_STRING_TO_INDEX
	pStaticNotice->GetString(0)->msg = GET_STRING( STR_ID_24577 );
#else
	pStaticNotice->GetString(0)->msg = L"��ų�������� �巡�� �ϰų� ��Ŭ���� ���� ��ų ���Կ�\n���� �� �� �ֽ��ϴ�.";
#endif HARDCODING_STRING_TO_INDEX
#endif
#endif //REFORM_UI_SKILLSLOT

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
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() != NULL )
	{
		m_pUserSkillTree = &(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree);

		const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet();

		CKTDGUIStatic* pStatic_CharClass = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticCharClass" );
		if( NULL != pStatic_CharClass )
		{
			wstring wstrClassName;
			switch( pUnitTemplet->m_UnitClass )
			{
				//�ʱ�����
			case CX2Unit::UC_ELSWORD_SWORDMAN:		// 1
			case CX2Unit::UC_ARME_VIOLET_MAGE:		// 2
			case CX2Unit::UC_LIRE_ELVEN_RANGER:		// 3
			case CX2Unit::UC_RAVEN_FIGHTER:			// 4
			case CX2Unit::UC_EVE_NASOD:				// 5
//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UC_CHUNG_IRON_CANNON:		// 6
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UC_ARA_MARTIAL_ARTIST:	// 7
#endif
			case CX2Unit::UC_ELESIS_KNIGHT: // 8 
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
			case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:	// 22
				{
					wstrClassName = GET_STRING(STR_ID_15277);                    
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
			case CX2Unit::UC_RAVEN_WEAPON_TAKER:		// 25
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
					wstrClassName = GET_STRING(STR_ID_15827);                    
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
					wstrClassName = GET_STRING(STR_ID_4783);                    
				} break;
			case CX2Unit::UC_LIRE_GRAND_ARCHER:		
				{
					wstrClassName = GET_STRING(STR_ID_4784);                    
				} break;
#ifdef SERV_RENA_NIGHT_WATCHER
			case CX2Unit::UC_LIRE_NIGHT_WATCHER:		
				{
					wstrClassName = GET_STRING(STR_ID_17429);                    
				} break;
#endif


			case CX2Unit::UC_ARME_ELEMENTAL_MASTER:		
				{
					wstrClassName = GET_STRING(STR_ID_4792);                    
				} break;
			case CX2Unit::UC_ARME_VOID_PRINCESS:		
				{
					wstrClassName = GET_STRING(STR_ID_4793);                    
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
					wstrClassName = GET_STRING(STR_ID_4847);                    
				} break;
			case CX2Unit::UC_RAVEN_RECKLESS_FIST:		
				{
					wstrClassName = GET_STRING(STR_ID_4848);                    
				} break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:		
				{
					wstrClassName = GET_STRING(STR_ID_17431);               
				} break;
#endif

				//{{ kimhc // 2010.1.22 // �̺� 2�� ����
#ifdef EVE_SECOND_CLASS_CHANGE
			case CX2Unit::UC_EVE_CODE_NEMESIS:
				{
					wstrClassName = GET_STRING( STR_ID_4916 );
				} break;

			case CX2Unit::UC_EVE_CODE_EMPRESS:
				{
					wstrClassName = GET_STRING( STR_ID_4917 );
				} break;
#endif EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.1.22 // �̺� 2�� ����
#ifdef SERV_EVE_BATTLE_SERAPH
			case CX2Unit::UC_EVE_BATTLE_SERAPH:
				{
					wstrClassName = GET_STRING( STR_ID_17433 );
				} break;
#endif

				//{{ kimhc // 2011.1.3 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
			case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
				{
					wstrClassName = GET_STRING( STR_ID_10239 );
				} break;

			case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
				{
					wstrClassName = GET_STRING( STR_ID_10240 );
				} break;
#endif	CHUNG_FIRST_CLASS_CHANGE
				//}} kimhc // 2011.1.3 // û 1�� ����

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
				{
					wstrClassName = GET_STRING( STR_ID_15998 );
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
#ifdef SERV_CHUNG_TACTICAL_TROOPER
			case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:		
				{
					wstrClassName = GET_STRING(STR_ID_17435);               
				} break;
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
			case CX2Unit::UC_LIRE_TRAPPING_RANGER:		// 24
				{
					wstrClassName = GET_STRING(STR_ID_15009);	//	�������η������� �Ǿ����� �ٲ���ߵ�                   
				} break;
#endif	SERV_TRAPPING_RANGER_TEST
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
#ifdef NEW_CHARACTER_EL
			case CX2Unit::UC_ELESIS_SABER_KNIGHT:
				{
					wstrClassName = GET_STRING( STR_ID_24893 ); // ���̹� ����Ʈ
				} break;
			case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
				{
					wstrClassName = GET_STRING( STR_ID_24894 ); // ���̷� ����Ʈ
				} break;
#endif // NEW_CHARACTER_EL
			case CX2Unit::UC_NONE:
			default:
				{
					//ASSERT( !"NO" );
				} break;
			}

#ifdef CLIENT_GLOBAL_LINEBREAK
			// ��ų Ʈ���� ���� ��Ʈ���� ���� ����
			wstrClassName = CWordLineHandler::GetStrByLineBreakInX2Main( wstrClassName.c_str(), 90, pStatic_CharClass->GetString(0)->fontIndex);
#endif // CLIENT_GLOBAL_LINEBREAK

			pStatic_CharClass->GetString(0)->msg = wstrClassName.c_str();


		}

		CreateSkillTree( pUnitTemplet->m_UnitClass );
		InitEquippedSkillSlotData();

	}	
}

void CX2UISkillTree::CreateSkillTree( CX2Unit::UNIT_CLASS eUnitClass )
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

	if ( NULL == g_pData->GetSkillTree() )
		return;

	// Unit Class�� �̿��ؼ� �ش� Map�� ��Ե� �޾ƿ´�. �Դٰ� ġ��.
	const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetSkillTreeTempletMap( (int)eUnitClass );

	for( CX2SkillTree::SkillTreeTempletMap::const_iterator mapit = refSkillTreeTempletMap.begin(); mapit != refSkillTreeTempletMap.end(); ++mapit )
	{
		CX2SkillTree::SKILL_ID eSkillID = mapit->first;
		const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = mapit->second;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
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
#ifdef GUIDE_SKILL_LEVEL_MINUS
		UISet.m_pButtonMinusGuide->AddDummyInt((int)eSkillID);
#endif GUIDE_SKILL_LEVEL_MINUS

		// ����ְ�
		pControlList->InsertItem( UISet.m_pStaticBackGround, row, col );
		pControlList->InsertItem( UISet.m_pSlot, row, col );
		pControlList->InsertItem( UISet.m_pButtonPlus, row, col );
		pControlList->InsertItem( UISet.m_pButtonMinus, row, col );
		pControlList->InsertItem( UISet.m_pButtonMaster, row, col );
		pControlList->InsertItem( UISet.m_pStaticLevelString, row, col );
		pControlList->InsertItem( UISet.m_pStaticActiveMark, row, col );
		pControlList->InsertItem( UISet.m_pStaticLock, row, col );
#ifdef GUIDE_SKILL_LEVEL_MINUS
		pControlList->InsertItem( UISet.m_pButtonMinusGuide, row, col );
#endif GUIDE_SKILL_LEVEL_MINUS

#ifdef ADDED_RELATIONSHIP_SYSTEM
		pControlList->InsertItem( UISet.m_pStaticRelationType, row, col );
#endif //ADDED_RELATIONSHIP_SYSTEM

		// ������ Map�� ����ְ�
		m_mapSkillSlotUISet.insert(std::make_pair(eSkillID, UISet));

		// ���ེų�� �����ϸ� 
		if(refSkillTreeTemplet.m_vecPrecedingSkill.empty() == false)
		{
			// ȭ��ǥ�� ������
			for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
			{
				CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
				const CX2SkillTree::SkillTreeTemplet* pPreccedSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( (int)eUnitClass, PreccedSkillID );
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
		CX2SkillTreeSlotData* pSkillTreeSlotData = new CX2SkillTreeSlotData;
		pSkillTreeSlotData->m_eSkillID = eSkillID;

		UISet.m_pSlot->SetID((int)eSkillID);
		UISet.m_pSlot->SetSlotData( pSkillTreeSlotData );

		pSkillTreeSlotData->m_eSkillID = eSkillID;
		pSkillTreeSlotData->m_bDataUpdated = true;

	}

	pControlList->SetIndex(0, 0);
#ifndef REFORM_UI_SKILLSLOT
	// �̴� ��ųƮ���� ������
	CreateMiniSkillTree( eUnitClass );
#endif //REFORM_UI_SKILLSLOT

	// Blind Static�� ���� ������ �������
	CKTDGUIStatic* pStaticBlind = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind" );
	m_pDLGUISkillTree->ChangeSequence( pStaticBlind, true );
	CKTDGUIStatic* pStaticBlindSPNotice = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind_Notice" );
	m_pDLGUISkillTree->ChangeSequence( pStaticBlindSPNotice, true );


}

CX2EquippedSkillSlotData* CX2UISkillTree::GetEquippedSlotData( int index, bool bSlotB /*= false*/ )
{
	if (m_pDLGUISkillTree == NULL)
		return NULL;

	CX2EquippedSkillSlotData* pSlotData = NULL;
#ifdef REFORM_UI_SKILLSLOT
	if(false == bSlotB)	// ���� A
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
	else	// ���� B
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
#else
	if(false == bSlotB)	// ���� A
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
	else	// ���� B
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
#endif //REFORM_UI_SKILLSLOT
	return pSlotData;
}
#ifdef REFORM_UI_SKILLSLOT
CKTDGUISlot* CX2UISkillTree::GetEquippedSlot( int index , bool bSlotB /* = false */ )
{
	if (m_pDLGUISkillTree == NULL)
		return NULL;

	CKTDGUISlot* pSlot = NULL ;

	//�ε������� 4�̻����� ���� ������ B�������� �Ǵ�
	if( index >= 4 )
	{
		index -= 4;
		bSlotB = true;
	}
		
	if(false == bSlotB)	// ���� A
	{
		switch(index)
		{
		case 0:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA1" );
			} break;
		case 1:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA2" );
			} break;
		case 2:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA3" );
			} break;
		case 3:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA4" );
			} break;
		default:
			break;
		}
	}
	else	// ���� B
	{
		switch(index)
		{
		case 0:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB1" );
			} break;
		case 1:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB2" );
			} break;
		case 2:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB3" );
			} break;
		case 3:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB4" );
			} break;
		default:
			break;
		}
	}

	return pSlot;
}
#endif //REFORM_UI_SKILLSLOT
void CX2UISkillTree::InitEquippedSkillSlotData()
{

	if(m_pDLGUISkillTree == NULL)
		return;

	for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT*2; ++i)
	{
		bool bSlotB = (i > 3) ? true : false;
		int iSlotIndex = (i > 3) ? i-4 : i;
		CX2EquippedSkillSlotData* pData = GetEquippedSlotData(iSlotIndex, bSlotB);
		SAFE_DELETE(pData);
	}
#ifdef REFORM_UI_SKILLSLOT
	CKTDGUISlot* pSlot = NULL;
	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, true));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, true));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, true));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, true));
#else
	CKTDGUISlot* pSlot = NULL;
	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, true));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, true));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, true));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, true));
#endif //REFORM_UI_SKILLSLOT
}

void CX2UISkillTree::UpdateSkillEquipedSlotUI()
{
	if(m_pUserSkillTree != NULL)
	{
		for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT*2; ++i)
		{			
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;
			CX2EquippedSkillSlotData* pSlotData = GetEquippedSlotData(iSlotIndex, bSlotB);
			if(pSlotData != NULL)
			{
				pSlotData->m_bDataUpdated = true;				
			}
		}

#ifdef REFORM_UI_SKILLSLOT
		if(m_pDLGGameSkillSlot != NULL)
		{
			switch(m_pUserSkillTree->GetSkillSlotBExpirationState())
			{
			case CX2UserSkillTree::SSBES_NOT_EXPIRED: //���� Ȯ�� ����
			case CX2UserSkillTree::SSBES_PERMANENT: 
				{
					m_bIsExpandSkillSlot = true;
					CKTDGUIButton* pButtonBuyGuide = (CKTDGUIButton*) m_pDLGGameSkillSlot->GetControl( L"Button_Buy_Guide_Slot" );
					if( NULL != pButtonBuyGuide)
						pButtonBuyGuide->SetShowEnable(false, false);

					CKTDGUIStatic* pStaticLock = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"LOCK" );
					if( NULL != pStaticLock)
						pStaticLock->SetShow(false);

					CKTDGUIStatic* pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKeyEx" );
					if( NULL != pStaticSkillHotKey)
						pStaticSkillHotKey->SetColor(D3DXCOLOR(1.f,1.f,1.f,1.f));
				}
				break;
			case CX2UserSkillTree::SSBES_EXPIRED: //���� ��Ȯ�� ����
			default:
				{
					m_bIsExpandSkillSlot =false;
					CKTDGUIButton* pButtonBuyGuide = (CKTDGUIButton*) m_pDLGGameSkillSlot->GetControl( L"Button_Buy_Guide_Slot" );
					if( NULL != pButtonBuyGuide)
						pButtonBuyGuide->SetShowEnable(true, true);

					CKTDGUIStatic* pStaticLock = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"LOCK" );
					if( NULL != pStaticLock)
						pStaticLock->SetShow(true);

					CKTDGUIStatic* pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKeyEx" );
					if( NULL != pStaticSkillHotKey)
						pStaticSkillHotKey->SetColor(D3DXCOLOR(1.f,1.f,1.f,0.5f));
				}
				break;
			}
#else
		if(m_pDLGUISkillTree != NULL)
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Notice_Slot" );
			if(pButton != NULL)
			{
				switch(m_pUserSkillTree->GetSkillSlotBExpirationState())
				{
				case CX2UserSkillTree::SSBES_NOT_EXPIRED: //���� Ȯ�� ����
					{
						pButton->SetShowEnable(true, true);
					} break;
				case CX2UserSkillTree::SSBES_PERMANENT: //���� ��Ȯ�� ����
				case CX2UserSkillTree::SSBES_EXPIRED:
				default:
					{
						pButton->SetShowEnable(false, false);
					} break;
				}
			}
#endif //REFORM_UI_SKILLSLOT
#ifdef GUIDE_SKILL_SLOTB 
			pButton = (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Buy_Guide_Slot" );
			if(pButton != NULL)
			{
				switch(m_pUserSkillTree->GetSkillSlotBExpirationState())
				{
				case CX2UserSkillTree::SSBES_EXPIRED:
					{
						pButton->SetShowEnable(true, true);
					} break;
				case CX2UserSkillTree::SSBES_PERMANENT:
				case CX2UserSkillTree::SSBES_NOT_EXPIRED:
				default:
					{
						pButton->SetShowEnable(false, false);
					} break;
				}
			}
#endif GUIDE_SKILL_SLOTB
		}

#ifdef ELSWORD_WAY_OF_SWORD
		if( NULL == g_pX2Game && NULL != m_pDLGGameSkillSlot )
		{
			CKTDGUIStatic* pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_B_B" );
			CKTDGUIStatic* pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_R_B" );
			if( NULL != pStatic_ElSkillVigor && NULL != pStatic_ElSkillDest)
			{
				pStatic_ElSkillVigor->SetShow(false);
				pStatic_ElSkillDest->SetShow(false);
			}

			pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_B" );
			pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_R" );
			if( NULL != pStatic_ElSkillVigor && NULL != pStatic_ElSkillDest)
			{
				pStatic_ElSkillVigor->SetShow(false);
				pStatic_ElSkillDest->SetShow(false);
			}
		}
#endif //ELSWORD_WAY_OF_SWORD

	}
}

void CX2UISkillTree::UpdateUI( bool bEquipped, bool bSP /*= true*/, bool bSkillTree /*= false*/ )
{
	if(bEquipped)
	{
		UpdateSkillEquipedSlotUI();
	}

	if(bSP)
	{
		UpdateSPInfo();
	}

	if(bSkillTree)
	{
		UpdateSkillTree();
	}

	

}

void CX2UISkillTree::UpdateSkillTree()
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

void CX2UISkillTree::UpdateSkillTree( CX2SkillTree::SKILL_ID eSkillID )
{
	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.find(eSkillID);
	if( it != m_mapSkillSlotUISet.end() )
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
		}
		const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet((int)g_pData->GetMyUser()->GetSelectUnit()->GetClass(), eSkillID);
		if(pSkillTreeTemplet != NULL)
		{
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
		}

	}
	UpdateBlind();

}

void CX2UISkillTree::UpdateBlind()
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
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.CalcUsedSPointAndCSPoint( iUsedSP, iUsedCP );
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
						pStaticBlindSPNotice->GetString(i)->msg = GET_REPLACED_STRING( ( STR_ID_2680, "i", (OpenedLastTier+1)*MAGIC_SP_NEED_PER_TIER) );
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

void CX2UISkillTree::UpdateNavi()
{
	if(m_pDLGUISkillTree == NULL)
		return;

	CKTDGUINavigation* pNavi = (CKTDGUINavigation*)m_pDLGUISkillTree->GetControl( L"Navi_MiniTree" );
	if(pNavi == NULL)
		return;

	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	
	if( NULL != pNavi )
	{
		pNavi->SetShow(true);

		// ��ġ �����ְ�~
		D3DXVECTOR2 vPos;
		vPos.x = 0.0f;
		if( 0 != (float)pControlList->GetMaxIndexY() - (float)pControlList->GetViewSizeY() + 1.f )
			vPos.y = (float)(pControlList->GetIndexY()) / ((float)pControlList->GetMaxIndexY() - (float)pControlList->GetViewSizeY() + 1.f );
		else
			vPos.y = 0.0f;

		pNavi->SetRelativeWindowPos( vPos * m_fNaviMoveRange );
	}

}

void CX2UISkillTree::UpdateSPInfo()
{
	if(m_pDLGUISkillTree == NULL)
		return;

	CKTDGUIStatic* pStatic_SP = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticSP" );
	CKTDGUIStatic* pStatic_UsedSP = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticUsedSP" );
	CKTDGUIStatic* pStatic_SPUndo = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticUndo" );

	bool bUsingCSP = false;
	CTime cTime;
	KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, cTime );
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
		KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, cTime );
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
		int iNewSP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint + g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint;
		int iUsedSP = 0;
		int iUsedCP = 0;
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.CalcUsedSPointAndCSPoint( iUsedSP, iUsedCP );
		int iTotalUsedSP = iUsedCP + iUsedSP;

		if( NULL != pStatic_SP )
		{		
			WCHAR wszText[256] = {0,};
			if(bUsingCSP)
			{
				//wsprintf( wszText, L"#CEC008C%d#CX", iNewSP );
				StringCchPrintf(wszText, 256, L"#CEC008C%d#CX", iNewSP );
			}
			else
			{
				//wsprintf( wszText, L"%d", iNewSP );
				StringCchPrintf(wszText, 256, L"%d", iNewSP );
			}
			
			pStatic_SP->GetString(0)->msg = wszText;
		}

		if ( NULL != pStatic_UsedSP )
		{
			WCHAR wszText[256] = {0,};
			if(bUsingCSP)
			{
				//wsprintf( wszText, L"#CEC008C%d#CX", iTotalUsedSP );
				StringCchPrintf(wszText, 256, L"#CEC008C%d#CX", iTotalUsedSP );
			}
			else
			{
				//wsprintf( wszText, L"%d", iTotalUsedSP );
				StringCchPrintf(wszText, 256, L"%d", iTotalUsedSP );
			}
			
			pStatic_UsedSP->GetString(0)->msg = wszText;
		}

		if( NULL != pStatic_SPUndo )		
		{		
			WCHAR wszText[256] = {0,};
			if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory() != NULL )
			{	
				int iSkillRevertItemCount = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RESET_A_SKILL_ITEM_ID );
				iSkillRevertItemCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RESET_A_SKILL_EVENT_ITEM_ID );
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
				iSkillRevertItemCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RESET_A_SKILL_ITEM );
#endif SERV_EVENT_RESET_A_SKILL_ITEM
#ifdef SERV_QUEST_SKILL_RESET
				iSkillRevertItemCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RESET_A_SKILL_QUEST_ITEM_ID );
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_2ND_CLASS_SKILL_RESET
				iSkillRevertItemCount += g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( RESET_A_SKILL_2ND_CLASS_CHANGE_EVENT_ITEM_ID );
#endif SERV_2ND_CLASS_SKILL_RESET

				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"%d", iSkillRevertItemCount );
			}			
			pStatic_SPUndo->GetString(0)->msg = wszText;
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

	
	
}

void CX2UISkillTree::CreateSlotUIPreset(SkillSlotUI& UISet, int eSkillID )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	WCHAR buf[255] = {0,};
	
	m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Icon.lua" );
	UISet.m_pStaticBackGround =			(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Background" );
	UISet.m_pStaticActiveMark =			(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticActive" );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	bool bIsRelationSkill = false;
#endif // ADDED_RELATIONSHIP_SYSTEM

	if(UISet.m_pStaticBackGround != NULL && UISet.m_pStaticActiveMark != NULL)
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID)eSkillID, 1 );
		if(pSkillTemplet != NULL)
		{
			UISet.m_pStaticBackGround->GetPicture(0)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(1)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(2)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(3)->SetShow(false);
#ifdef ADDED_RELATIONSHIP_SYSTEM
			UISet.m_pStaticBackGround->GetPicture(4)->SetShow(false);
#endif // cRELATIONSHIP_SKILL

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
					bIsRelationSkill = true;
					UISet.m_pStaticBackGround->GetPicture(4)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);					

				} break;
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
		StringCchPrintf( buf, 256, L"g_pStaticSkill_Background_%d", eSkillID );
		UISet.m_pStaticBackGround->SetName(buf);
		//wsprintf( buf, L"StaticActive_%d", eSkillID );
		StringCchPrintf( buf, 256, L"StaticActive_%d", eSkillID );
		UISet.m_pStaticActiveMark->SetName(buf);
		
	}
	
	UISet.m_pSlot =						(CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"g_pSlot_SkillTree" );
	if(UISet.m_pSlot != NULL)
	{
		//wsprintf( buf, L"g_pSlot_SkillTree_%d", eSkillID );
		StringCchPrintf( buf, 256, L"g_pSlot_SkillTree_%d", eSkillID );
		UISet.m_pSlot->SetName(buf);
	}
	

	UISet.m_pButtonPlus =				(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Plus" );
	if(UISet.m_pButtonPlus != NULL)
	{
		//wsprintf( buf, L"Button_Plus_%d", eSkillID );
		StringCchPrintf( buf, 256, L"Button_Plus_%d", eSkillID );
		UISet.m_pButtonPlus->SetName(buf);
		UISet.m_pButtonPlus->ChangeMouseUpSound( L"" );
		UISet.m_pButtonPlus->ChangeMouseOverSound( L"" );

	}
	
	UISet.m_pButtonMinus =				(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Minus" );
	if(UISet.m_pButtonMinus != NULL)
	{
		//wsprintf( buf, L"Button_Minus_%d", eSkillID );
		StringCchPrintf( buf, 256, L"Button_Minus_%d", eSkillID );
		UISet.m_pButtonMinus->SetName(buf);
		UISet.m_pButtonMinus->ChangeMouseUpSound( L"" );
		UISet.m_pButtonMinus->ChangeMouseOverSound( L"" );
	}
#ifdef GUIDE_SKILL_LEVEL_MINUS
	UISet.m_pButtonMinusGuide =			(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Minus_Guide" );
	if(UISet.m_pButtonMinusGuide != NULL)
	{
		//wsprintf( buf, L"Button_Minus_%d", eSkillID );
		StringCchPrintf( buf, 256, L"Button_Minus_Guide_%d", eSkillID );
		UISet.m_pButtonMinusGuide->SetName(buf);
		UISet.m_pButtonMinusGuide->ChangeMouseUpSound( L"" );
		UISet.m_pButtonMinusGuide->ChangeMouseOverSound( L"" );
	}
#endif GUIDE_SKILL_LEVEL_MINUS


	UISet.m_pButtonMaster =	(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Master" );
	if( UISet.m_pButtonMaster != NULL )
	{
		//wsprintf( buf, L"Button_Master_%d", eSkillID );
		StringCchPrintf( buf, 256, L"Button_Master_%d", eSkillID );
		UISet.m_pButtonMaster->SetName(buf);
	}
	
	UISet.m_pStaticLevelString =		(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticLevel" );
	if( UISet.m_pStaticLevelString != NULL)
	{
		//wsprintf( buf, L"StaticLevel_%d", eSkillID );
		StringCchPrintf( buf, 256, L"StaticLevel_%d", eSkillID );
		UISet.m_pStaticLevelString->SetName(buf);

	}


	UISet.m_pStaticLock =		(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Lock" );
	if( UISet.m_pStaticLock != NULL)
	{
		//wsprintf( buf, L"StaticLock_%d", eSkillID );
		StringCchPrintf( buf, 256, L"StaticLock_%d", eSkillID );
		UISet.m_pStaticLock->SetName(buf);
	}


#ifdef ADDED_RELATIONSHIP_SYSTEM
	UISet.m_pStaticRelationType = static_cast<CKTDGUIStatic*> ( m_pDLGUISkillTree->GetControl( L"StaticRelationType" ) );
	if( UISet.m_pStaticRelationType != NULL )
	{
		//wsprintf( buf, L"Button_Master_%d", eSkillID );
		StringCchPrintf( buf, 256, L"StaticRelationType_%d", eSkillID );
		UISet.m_pStaticRelationType->SetName(buf);
	}
	// Relation Skill ���� ó��, �� ��ư�� �����.
	if ( bIsRelationSkill == true )
	{
		UISet.m_pButtonPlus->SetShowEnable( false, false );
		UISet.m_pButtonMinus->SetShowEnable( false, false );
		UISet.m_pButtonMinusGuide->SetShowEnable( false, false );
		UISet.m_pButtonMaster->SetShowEnable( false, false );
	}
#endif //ADDED_RELATIONSHIP_SYSTEM

	return;
}

void CX2UISkillTree::CreateVerticalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail.lua" );

	WCHAR buf[255] = {0,};

	UISet.m_pStaticArrowEnd =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_End" );
	if( UISet.m_pStaticArrowEnd != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_End_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_End_%d", eSkillID );
		UISet.m_pStaticArrowEnd->SetName(buf);
	}

	UISet.m_pStaticArrowHead =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Head" );
	if( UISet.m_pStaticArrowHead != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_Head_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Head_%d", eSkillID );
		UISet.m_pStaticArrowHead->SetName(buf);
	}

	for( int i = 0; i < iLineLength; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_Line.lua" );
		CKTDGUIStatic* pStaticArrowLine 		= (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Line" );
		if( pStaticArrowLine != NULL )
		{
			//wsprintf( buf, L"StaticSkill_Arrow_Line_%d_%d", eSkillID, i );
			StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Line_%d_%d", eSkillID, i );
			pStaticArrowLine->SetName(buf);
			UISet.m_vpStaticArrowLine.push_back(pStaticArrowLine);
		}		
	}
}

void CX2UISkillTree::CreateHorizonalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID, bool bRight )
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
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_End_H_%d", eSkillID );
		UISet.m_pStaticArrowEnd->SetName(buf);
	}

	UISet.m_pStaticArrowHead =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Head_H" );
	if( UISet.m_pStaticArrowHead != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_Head_H_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Head_H_%d", eSkillID );
		UISet.m_pStaticArrowHead->SetName(buf);
	}

	for( int i = 0; i < iLineLength; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_Line_Horizonal.lua" );
		CKTDGUIStatic* pStaticArrowLine 		= (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Line_H" );
		if( pStaticArrowLine != NULL )
		{
			//wsprintf( buf, L"StaticSkill_Arrow_Line_H_%d_%d", eSkillID, i );
			StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Line_H_%d_%d", eSkillID, i );
			pStaticArrowLine->SetName(buf);
			UISet.m_vpStaticArrowLine.push_back(pStaticArrowLine);
		}		
	}

}



void CX2UISkillTree::SetArrowColor( CX2SkillTree::SKILL_ID eSkillID, D3DXCOLOR cCol )
{
	std::pair< std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator, std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator> range = m_mapArrowUISet.equal_range(eSkillID);
	
	for (std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator it = range.first; it != range.second; ++it)
	{
		ArrowUI& refArrow = it->second;

		if( refArrow.m_pStaticArrowEnd != NULL )
			refArrow.m_pStaticArrowEnd->SetColor( cCol );
		if( refArrow.m_pStaticArrowHead != NULL )
			refArrow.m_pStaticArrowHead->SetColor( cCol );
		for(vector<CKTDGUIStatic*>::iterator itstatic =	refArrow.m_vpStaticArrowLine.begin(); itstatic != refArrow.m_vpStaticArrowLine.end(); ++itstatic)
		{
			CKTDGUIStatic* pStatic = *itstatic;
			if( pStatic != NULL )
				pStatic->SetColor( cCol );
		}
		
	}
#ifndef REFORM_UI_SKILLSLOT
	CKTDGUIStatic* pStatic_MiniLine = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"Mini_Skill_Line" );
	if( NULL != pStatic_MiniLine )
	{
		std::pair< std::multimap<CX2SkillTree::SKILL_ID, int>::iterator, std::multimap<CX2SkillTree::SKILL_ID, int>::iterator> minirange = m_mapMiniArrowPictureIndex.equal_range(eSkillID);
		for (std::multimap<CX2SkillTree::SKILL_ID, int>::iterator it = minirange.first; it != minirange.second; ++it)
		{
			int iPictureIndex = it->second;
			if( NULL != pStatic_MiniLine->GetPicture( iPictureIndex ) )
			{
				if( pStatic_MiniLine != NULL )
					pStatic_MiniLine->GetPicture( iPictureIndex )->SetColor( cCol );
			}
		}
	}
#endif //REFORM_UI_SKILLSLOT
		
}

void CX2UISkillTree::SetMiniSkillSlotColor( CX2SkillTree::SKILL_ID eSkillID, D3DXCOLOR cCol )
{
#ifndef REFORM_UI_SKILLSLOT
	CKTDGUIStatic* pStatic_MiniIcon = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"Mini_Skill_Icon" );
	if( NULL == pStatic_MiniIcon )
		return;

	map<CX2SkillTree::SKILL_ID, int>::iterator it = m_mapMiniSkillSlotPictureIndex.find(eSkillID);
	if( it != m_mapMiniSkillSlotPictureIndex.end() )
	{
		int iPictureIndex = it->second;
		if( NULL != pStatic_MiniIcon->GetPicture( iPictureIndex ) )
		{
			pStatic_MiniIcon->GetPicture( iPictureIndex )->SetColor( cCol );
		}

	}
#endif //REFORM_UI_SKILLSLOT
}

// CX2UISkillTree::SkillSlotUI& CX2UISkillTree::GetSkillSlotUI( CX2SkillTree::SKILL_ID eSkillID )
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

// CX2UISkillTree::ArrowUI& CX2UISkillTree::GetArrowUI( CX2SkillTree::SKILL_ID eSkillID )
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
// UICustomMsgProc���� case STUCM_UNSEAL_SKILL_BUY �׸� �� �κ��� �۾��� �ּ���.
void CX2UISkillTree::OpenSkillUnsealInformDLG( const CX2SkillTree::SkillTemplet* pSkillTemplet, D3DXVECTOR2 SlotCenterPos )
{	
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );
		
	m_pDLGSkillUnsealInform = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Purchase_Window.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealInform );
	m_pDLGSkillUnsealInform->SetDisableUnderWindow(true);

	// ���̾�αװ� ������ �� ��ġ(���� �߽ɿ��������� ��밪) : ��ũ��Ʈ�� �־� ��. DLG_Skill_Purchase_Window.lua �ֻ�� ����.
	D3DXVECTOR2 OffsetPos((float)m_pDLGSkillUnsealInform->GetDummyInt(0), (float)m_pDLGSkillUnsealInform->GetDummyInt(1));	
	m_pDLGSkillUnsealInform->SetPos( SlotCenterPos + OffsetPos );

	CKTDGUIStatic* pStaticRequireItem = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticRequireItemName" );
	CKTDGUIStatic* pStaticRequireCash = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticPrice" );
	

//#if 0		//** ��ų �������� ����� �� �� ���⿡ �߰��۾� �� �ּ���. 
////{{AFX
//	if( NULL != pStaticRequireCash->GetString(0) )
//	{
//		//** ���⿡ �ش� ��ų�� ���������� ������ �� �ּ���
//		int iRequireCash = pSkillTemplet->����;
//		WCHAR buf[256] = {0,};
//		//wsprintf( buf, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
//		StringCchPrintf( buf, 256, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
//		pStaticRequireCash->GetString(0)->msg = buf;
//	}
//	else
//	{
//		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
//		ASSERT( "Script Error." );
//		return;
//	}
//
//	if( NULL != pStaticRequireItem->GetString(0) )
//	{
//
//		//** ���⿡ �ش� ��ų�� ���������� �̸��� �� �ּ���
//		WCHAR buf[256] = {0,};
//		wsprintf( buf, L"%s", pSkillTemplet->�ʿ��Ѿ������̸� );
//		StringCchPrintf( buf, 256, L"%s", pSkillTemplet->�ʿ��Ѿ������̸� );
//		pStaticRequireCash->GetString(0)->msg = buf;
//	}
//	else
//	{
//		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
//		ASSERT( "Script Error." );
//		return;
//	}
//
//	m_pDLGSkillUnsealInform->SetShowEnable(true, true);
////}}AFX
//#endif

		
}

// SlotManager�� ItemDesc �� �ڵ带 �ܾ����
void CX2UISkillTree::ShowSkillDesc( bool bShow, CX2SkillTree::SKILL_ID eSkillID /*= CX2SkillTree::SI_NONE*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, 
								    D3DXVECTOR2 size /*= D3DXVECTOR2(0,0)*/, bool bEquipped /*= false*/ )
{
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
		
		//CX2Main::LineBreak( SkillDesc, pFont, (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#ifdef CLIENT_GLOBAL_LINEBREAK
		SkillDesc = CWordLineHandler::GetStrByLineBreakColorInX2Main( SkillDesc.c_str(), (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), SLOT_MANAGER_FONT_INDEX );
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
		SkillDesc = g_pMain->GetStrByLineBreakColor(SkillDesc.c_str(), (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), SLOT_MANAGER_FONT_INDEX);
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
	
	
}

wstring CX2UISkillTree::GetSkillDesc( CX2SkillTree::SKILL_ID eSkillID, bool bEquipped /*= false*/ )
{
	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();

	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;


#ifdef ELSWORD_SHEATH_KNIGHT
	bool bReplacedEsCounter = false;
	if( bEquipped == true && eSkillID == CX2SkillTree::SI_A_ES_COUNTER )
	{
		int iSkillLevel = refUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER );
		if( iSkillLevel > 0 )
		{
			eSkillID = CX2SkillTree::SI_P_ETK_COMPACT_COUNTER;
			bReplacedEsCounter = true;
		}
	}
#endif ELSWORD_SHEATH_KNIGHT


	int iSkillLevel = refUserSkillTree.GetSkillLevel( eSkillID );
	int iUnitclass = (int) g_pData->GetMyUser()->GetSelectUnit()->GetClass();
	
	if( true == bEquipped && g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		int levelUpNum = g_pX2Game->GetMyUnit()->GetSkillLevelUpNum( eSkillID );
#ifdef PVP_SEASON2
		iSkillLevel = min( iSkillLevel + levelUpNum, 10 );
#else
		iSkillLevel = iSkillLevel + levelUpNum;
#endif
	}
	
	
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
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			wstrm << GET_STRING(STR_ID_2673);		// ��Ƽ��

		} break;
	case CX2SkillTree::ST_BUFF:
		{
			wstrm << GET_STRING(STR_ID_2674);		// ����

		} break;
	default:
		{
#ifdef ELSWORD_SHEATH_KNIGHT
			if( bReplacedEsCounter == true )
				wstrm << GET_STRING(STR_ID_2673);
			else
#endif ELSWORD_SHEATH_KNIGHT
				wstrm << GET_STRING(STR_ID_2675);		// �нú�

		} break;
	}
	wstrm << L"]\n";





#ifndef SEAL_SKILL_GUIDE
	// ���ε� ��ų
	if( true == pSkillTemplet->m_bBornSealed && 
		false == refUserSkillTree.IsSkillUnsealed( eSkillID ) )
	{
		wstrm << L"\n[" << GET_STRING(STR_ID_3858) << L"]\n\n";
		wstrm << GET_STRING(STR_ID_3857);
		return wstrm.str();
	}
#endif //SEAL_SKILL_GUIDE






	if( bEquipped == false )
	{
		wstrm << L"(" << iSkillLevel << L"/" << pSkillTree->GetMaxSkillLevel( iUnitclass, eSkillID ) << L")" << L"\n";
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

#ifdef ELSWORD_WAY_OF_SWORD
	if( CX2Unit::UT_ELSWORD == g_pData->GetMyUser()->GetSelectUnit()->GetType() 
#ifdef NEW_CHARACTER_EL // ���� ��
		|| CX2Unit::UT_ELESIS == g_pData->GetMyUser()->GetSelectUnit()->GetType() 
#endif // NEW_CHARACTER_EL		
		)
	{
		int iWayOfSwordType = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_WAY_OF_SWORD_TYPE );
		if( iWayOfSwordType == 2 )
		{
			wstrm << L"#C4488FF" << L"\n"<< GET_STRING(STR_ID_14551) << L"#CX\n";
		}
		else if (iWayOfSwordType == 1)
		{
			wstrm << L"#CFF4444" << L"\n"<< GET_STRING(STR_ID_14552) << L"#CX\n";
		}
		else
		{

		}
	}
#endif ELSWORD_WAY_OF_SWORD


	if( false == bEquipped && false == refUserSkillTree.CanILearnThisSkill( eSkillID, false ) )
	{
		const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetSkillTreeTempletMap( iUnitclass );
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
#ifdef SEAL_SKILL_GUIDE
	if( true == pSkillTemplet->m_bBornSealed && 
		false == refUserSkillTree.IsSkillUnsealed( eSkillID ) )
	{
		wstrm << L"\n\n#CFF0000[" << GET_STRING(STR_ID_3858) << L"]\n\n";
		wstrm << L"#CFF0000"  <<GET_STRING(STR_ID_3857)  << L"#CX";
	}
#endif //SEAL_SKILL_GUIDE

#ifdef PVP_SEASON2
	if ( 10 == iSkillLevel )
	{
		wstrm << L"\n\n#CFF0000" << GET_STRING( STR_ID_18859 ) << L"\n\n";
	}
#endif

	return wstrm.str();
}




void CX2UISkillTree::CreateMiniSkillTree( CX2Unit::UNIT_CLASS eUnitClass )
{

	// �ʱ�ȭ �ڵ�� ���� �� ��������
	// 1. Size�� Scale�� �ʱ�ȭ���ְ�
	// 2. ��ųƮ�� ���鶧�� �Ȱ��� Iteration�ϸ鼭
	// 3. Static�� �׸��ϳ� ���� �־��ְ�
	// 4. Skill ID - Picture Index ���� �ʿ� �־��ְ�
	// ������Ʈ �� �� : �̴ϸʿ� ��ų���̵� ���������� �׺κи� ������Ʈ �ǰ� �ϸ� �ǰڴ�!
	
	if(m_pDLGUISkillTree == NULL)
		return;
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	if( pControlList == NULL )
		return;

	if( NULL == g_pData->GetSkillTree() )
		return;

	CKTDGUIStatic* pStatic_MiniLine = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"Mini_Skill_Line" );
	if( NULL == pStatic_MiniLine )
		return;

	CKTDGUIStatic* pStatic_MiniIcon = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"Mini_Skill_Icon" );
	if( NULL == pStatic_MiniIcon )
		return;

	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;

	D3DXVECTOR2 vGridSize;
	vGridSize.x = (float)pControlList->GetMaxIndexX();
	vGridSize.y = (float)pControlList->GetMaxIndexY();


	D3DXVECTOR2 vBackGroundSize;		// ���� ��ųƮ�� ������
	vBackGroundSize.x = (float)(pControlList->GetMaxIndexX()+1) * (float)pControlList->GetScrollOffsetX();
	vBackGroundSize.y = (float)(pControlList->GetMaxIndexY()+1) * (float)pControlList->GetScrollOffsetY();


	//const D3DXVECTOR2 MAGIC_MINI_SKILL_TREE_OFFSET( 8, 10 );
	const float MAGIC_LINE_WIDTH = 2.f;
	const float MAGIC_ICON_WIDTH = 3.f;
	D3DXVECTOR3 NavigationSize;
	NavigationSize.y = (float)pStatic_MiniIcon->GetDummyInt(0);
	float Ratio = NavigationSize.y / vBackGroundSize.y;
	NavigationSize.x = Ratio * vBackGroundSize.x;

	
	D3DXVECTOR2 OffSetPos = pStatic_MiniLine->GetOffsetPos();
	OffSetPos.x = OffSetPos.x - NavigationSize.x * 0.5f;

	pStatic_MiniLine->SetOffsetPos( OffSetPos );
	pStatic_MiniIcon->SetOffsetPos( OffSetPos );


	//////////////////////////////////////////////////////////////////////////
	// Navigatior Size Update
	CKTDGUINavigation* pNavi = (CKTDGUINavigation*)m_pDLGUISkillTree->GetControl( L"Navi_MiniTree" );
	if(pNavi != NULL)
	{
		D3DXVECTOR2 NaviPos = OffSetPos;
		NaviPos.x = NaviPos.x - MAGIC_ICON_WIDTH;
		NaviPos.y = NaviPos.y - MAGIC_ICON_WIDTH;
		D3DXVECTOR2 NaviSize;
		NaviSize.x = NavigationSize.x + MAGIC_ICON_WIDTH*2;
		NaviSize.y = NavigationSize.y + MAGIC_ICON_WIDTH*2;

		D3DXVECTOR2 WindowSize;
		WindowSize.x = NaviSize.x;
		WindowSize.y = NaviSize.y * ( (float)pControlList->GetViewSizeY() / ( (float)pControlList->GetMaxIndexY()+ 1.f) );

		CKTDGUIControl::UIPointData* pNewPoint_Navi = new CKTDGUIControl::UIPointData();
		pNewPoint_Navi->bUseTextureSize = false;
		pNewPoint_Navi->fChangeTime = 0.0f;
		//pNewPoint_Navi->color = D3DXCOLOR(1,1,1,0.5f);		// �ӽ�

		pNewPoint_Navi->leftTopPoint		= D3DXVECTOR2( NaviPos.x , NaviPos.y );
		pNewPoint_Navi->rightTopPoint		= D3DXVECTOR2( NaviPos.x + NaviSize.x, NaviPos.y );
		pNewPoint_Navi->leftBottomPoint		= D3DXVECTOR2( NaviPos.x , NaviPos.y + NaviSize.y );
		pNewPoint_Navi->rightBottomPoint	= D3DXVECTOR2( NaviPos.x + NaviSize.x, NaviPos.y + NaviSize.y );

		pNavi->SetBGPoint( pNewPoint_Navi );

		CKTDGUIControl::UIPointData* pNewPoint_NaviWindow = new CKTDGUIControl::UIPointData();
		pNewPoint_NaviWindow->bUseTextureSize = false;
		pNewPoint_NaviWindow->fChangeTime = 0.0f;
		pNewPoint_NaviWindow->leftTopPoint		= D3DXVECTOR2( NaviPos.x , NaviPos.y );
		pNewPoint_NaviWindow->rightTopPoint		= D3DXVECTOR2( NaviPos.x + WindowSize.x, NaviPos.y );
		pNewPoint_NaviWindow->leftBottomPoint		= D3DXVECTOR2( NaviPos.x , NaviPos.y + WindowSize.y );
		pNewPoint_NaviWindow->rightBottomPoint	= D3DXVECTOR2( NaviPos.x + WindowSize.x, NaviPos.y + WindowSize.y );

		pNavi->SetWindowPoint( pNewPoint_NaviWindow );

		m_fNaviMoveRange = (NaviSize.y - WindowSize.y) / NaviSize.y;


	}

	//////////////////////////////////////////////////////////////////////////




	D3DXVECTOR2 vMiniRatio(Ratio, Ratio);

	D3DXVECTOR2 vSkillIconSize(0,0);
	if(false == m_mapSkillSlotUISet.empty() )
	{
		map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it = m_mapSkillSlotUISet.begin();
		SkillSlotUI& refSkillSlotUI = it->second;
		if(refSkillSlotUI.m_pSlot != NULL)
            vSkillIconSize = refSkillSlotUI.m_pSlot->GetSize();	// ù ��° ��ų�� ��ųƮ�� ���� ũ�⸦ ���Ѵ�
	}
	else
	{
		ASSERT( !"CreateMiniSkillTree : Get SlotSize Fail!" );
		return;
	}

	D3DXVECTOR2 vMiniArrowSize = vSkillIconSize;
	vMiniArrowSize.x *= vMiniRatio.x * 0.5f;
	vMiniArrowSize.y *= vMiniRatio.y * 0.5f;

	D3DXVECTOR2 vMiniButtonSize = vSkillIconSize;			// 46, 46
	vMiniButtonSize.x *= vMiniRatio.x;
	vMiniButtonSize.y *= vMiniRatio.y;

	// ���� ȭ��ǥ�� ���� ȭ��ǥ ũ��� "���� ����ؼ� �׷��ִ�" �ϴ� �ɷ� ����!
	

	/// ��������� ���� �ʱ�ȭ �ڵ�

	//1. �ش� Ŭ������ ��ųƮ�� ���ø��� �޾Ƽ�
	//2. iteration�� �� �س����鼭 ������ ��ų���� ���� ����� ��ġ ��Ƽ�
	//3. �ش��ϴ� ��ġ�� Static�� ����־� �ش�
	//4. ���ེų�� ������ ��ų�� �Ÿ��� ����ؼ� ���� ����� �׾��ش�.
	
	int CurrentSkillIconIndex = 0;
	int CurrentArrowIndex = 0;


	const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetSkillTreeTempletMap( (int)eUnitClass );

	for( CX2SkillTree::SkillTreeTempletMap::const_iterator mapit = refSkillTreeTempletMap.begin(); mapit != refSkillTreeTempletMap.end(); ++mapit )
	{
		CX2SkillTree::SKILL_ID eSkillID = mapit->first;
		const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = mapit->second;

		// ��ų �߰�!

		//const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
// 		if(pSkillTemplet == NULL)
// 		{
// #ifndef _SERVICE_
// 			WCHAR buf[256] = {0,};
// 			//wsprintf(buf, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
//			StringCchPrintf( buf, 256, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
// 			MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
// #endif
// 			continue;
// 		}

		int row = refSkillTreeTemplet.m_iTier;
		int col = refSkillTreeTemplet.m_iIndexInTier;


		D3DXVECTOR2 vButtonCenterPos(0, 0);
		vButtonCenterPos.x = ( pControlList->GetScrollOffsetX() * vMiniRatio.x ) * col + vMiniButtonSize.x * 0.5f;
		vButtonCenterPos.y = ( pControlList->GetScrollOffsetY() * vMiniRatio.y ) * row + vMiniButtonSize.y * 0.5f;
		

		// skill icon
		CKTDGUIControl::CPictureData* pPicture_Icon = new CKTDGUIControl::CPictureData;
		pPicture_Icon->SetTex( L"DLG_Common_Texture01.tga", L"White" );
		
		CKTDGUIControl::UIPointData* pNewPoint_Icon = new CKTDGUIControl::UIPointData();
		pNewPoint_Icon->bUseTextureSize = false;
		pNewPoint_Icon->fChangeTime = 0.0f;
		//pNewPoint_Icon->color				= D3DXCOLOR( 1.0f, 0.f, 0.f, 1.f );
		pNewPoint_Icon->leftTopPoint		= D3DXVECTOR2( vButtonCenterPos.x - MAGIC_ICON_WIDTH, vButtonCenterPos.y - MAGIC_ICON_WIDTH );
		pNewPoint_Icon->rightTopPoint		= D3DXVECTOR2( vButtonCenterPos.x + MAGIC_ICON_WIDTH, vButtonCenterPos.y - MAGIC_ICON_WIDTH );
		pNewPoint_Icon->leftBottomPoint		= D3DXVECTOR2( vButtonCenterPos.x - MAGIC_ICON_WIDTH, vButtonCenterPos.y + MAGIC_ICON_WIDTH );
		pNewPoint_Icon->rightBottomPoint	= D3DXVECTOR2( vButtonCenterPos.x + MAGIC_ICON_WIDTH, vButtonCenterPos.y + MAGIC_ICON_WIDTH );
		//pPicture_Icon->SetEdge( false, 1, D3DXCOLOR(0,0,0,0) );
		// ����..
		if(refUserSkillTree.IsMaxSkillLevel(eSkillID))
		{
			pNewPoint_Icon->color				= D3DXCOLOR( 1.0f, 1.f, 0.f, 1.f );
		}
		else if ( refUserSkillTree.DoIHaveThisSkill( eSkillID ))
		{
			pNewPoint_Icon->color				= D3DXCOLOR( 1.0f, 1.f, 1.f, 1.f );
		}
		else if( refUserSkillTree.CanILearnThisSkill( eSkillID, false ) )
		{
			pNewPoint_Icon->color				= D3DXCOLOR( 0.7f, 0.7f, 0.7f, 1.f );
		}
		else
		{
			pNewPoint_Icon->color				= D3DXCOLOR( 0.f, 0.f, 0.f, 1.f );
		}
		pPicture_Icon->SetPoint( pNewPoint_Icon );


		pStatic_MiniIcon->AddPicture( pPicture_Icon );
		m_mapMiniSkillSlotPictureIndex.insert(std::make_pair(eSkillID, CurrentSkillIconIndex));
		CurrentSkillIconIndex++;

		// ���ེų�� �����ϸ� 
		if(refSkillTreeTemplet.m_vecPrecedingSkill.empty() == false)
		{
			// ȭ��ǥ�� ������
			for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
			{
				CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
				const CX2SkillTree::SkillTreeTemplet* pPreccedSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( (int)eUnitClass, PreccedSkillID );
				if ( pPreccedSkillTreeTemplet != NULL )
				{
					int BeforeRow = pPreccedSkillTreeTemplet->m_iTier;
					int BeforeCol = pPreccedSkillTreeTemplet->m_iIndexInTier;
					
					// before skill�� ������ grid�� ������
					
					CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData;
					pPicture->SetTex( L"DLG_Common_Texture01.tga", L"White" );
					

					D3DXVECTOR2 vBeforeButtonCenterPos(0, 0);
					vBeforeButtonCenterPos.x = ( pControlList->GetScrollOffsetX() * vMiniRatio.x ) * BeforeCol + vMiniButtonSize.x * 0.5f;
					vBeforeButtonCenterPos.y = ( pControlList->GetScrollOffsetY() * vMiniRatio.y ) * BeforeRow + vMiniButtonSize.y * 0.5f;
					
					D3DXVECTOR2 vDir = vButtonCenterPos - vBeforeButtonCenterPos;
															
					if( vDir.x == 0.f )
					{
						CKTDGUIControl::UIPointData* pNewPoint = new CKTDGUIControl::UIPointData();
						pNewPoint->bUseTextureSize = false;
						pNewPoint->fChangeTime = 0.0f;

						if(true == refUserSkillTree.IsMaxSkillLevel(PreccedSkillID))
						{
							pNewPoint->color = D3DXCOLOR(1,1,1,1);
						}
						else
						{
							pNewPoint->color = D3DXCOLOR(0.5f,0.5f,0.5f,1);
						}

						pNewPoint->leftTopPoint			= D3DXVECTOR2( vButtonCenterPos.x - MAGIC_LINE_WIDTH, min( vButtonCenterPos.y, vBeforeButtonCenterPos.y ) );
						pNewPoint->rightTopPoint		= D3DXVECTOR2( vButtonCenterPos.x + MAGIC_LINE_WIDTH, min( vButtonCenterPos.y, vBeforeButtonCenterPos.y ) );
						pNewPoint->leftBottomPoint		= D3DXVECTOR2( vButtonCenterPos.x - MAGIC_LINE_WIDTH, max( vButtonCenterPos.y, vBeforeButtonCenterPos.y ) );
						pNewPoint->rightBottomPoint		= D3DXVECTOR2( vButtonCenterPos.x + MAGIC_LINE_WIDTH, max( vButtonCenterPos.y, vBeforeButtonCenterPos.y ) );

						pPicture->SetPoint( pNewPoint );

					}
					else if( vDir.y == 0.f )
					{
						CKTDGUIControl::UIPointData* pNewPoint = new CKTDGUIControl::UIPointData();
						pNewPoint->bUseTextureSize = false;
						pNewPoint->fChangeTime = 0.0f;

						if(true == refUserSkillTree.IsMaxSkillLevel(PreccedSkillID))
						{
							pNewPoint->color = D3DXCOLOR(1,1,1,1);
						}
						else
						{
							pNewPoint->color = D3DXCOLOR(0.5f,0.5f,0.5f,1);
						}

						pNewPoint->leftTopPoint			= D3DXVECTOR2( min( vButtonCenterPos.x, vBeforeButtonCenterPos.x), vButtonCenterPos.y - MAGIC_LINE_WIDTH );
						pNewPoint->rightTopPoint		= D3DXVECTOR2( max( vButtonCenterPos.x, vBeforeButtonCenterPos.x), vButtonCenterPos.y - MAGIC_LINE_WIDTH );
						pNewPoint->leftBottomPoint		= D3DXVECTOR2( min( vButtonCenterPos.x, vBeforeButtonCenterPos.x), vButtonCenterPos.y + MAGIC_LINE_WIDTH );
						pNewPoint->rightBottomPoint		= D3DXVECTOR2( max( vButtonCenterPos.x, vBeforeButtonCenterPos.x), vButtonCenterPos.y + MAGIC_LINE_WIDTH );

						pPicture->SetPoint( pNewPoint );
				
					}
					else
					{
						ASSERT( !"NO" );
					}
	

					pStatic_MiniLine->AddPicture( pPicture );
					m_mapMiniArrowPictureIndex.insert(std::make_pair(PreccedSkillID, CurrentArrowIndex));
					CurrentArrowIndex++;

				}
				
			}
		}

	}

}

void CX2UISkillTree::ShowUIDesc( bool bShow, wstring wstr /*= L""*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT /*= CX2TalkBoxManagerImp::TBT_FROM_UP_RIGHT*/, D3DXCOLOR coTextColor /*= D3DXCOLOR(0,0,0,1)*/, D3DXCOLOR coBackColor /*= D3DXCOLOR(1, 0.96f, 0.6f, 1 )*/)
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


// kimhc // KTDXCommonFunc�� �ű� // 2009-06-16
//wstring CX2UISkillTree::GetExpirationDateDesc( const wstring& wstrExpirationDate )
//{
//	wstring tempDesc = L"";
//
//	
//	tempDesc = GET_STRING( STR_ID_309 );
//	//tempDesc = L"���� �ð� : ";
//
//	CTime cTime;
//	KncUtil::ConvertStringToCTime( wstrExpirationDate, cTime );
//
//	wstringstream wstrstm;
//
//	//wstrstm << cTime.GetYear() << L"�� " << cTime.GetMonth() << L"�� " << cTime.GetDay() << L"�� " << cTime.GetHour() << L"�� " << cTime.GetMinute() << L"��"; 
//	//tempDesc += wstrstm.str().c_str();
//	//tempDesc += L"\n";
//
//	wstrstm.str( L"" );
//	wstrstm << L"\n";
//
//	//time_t t0 = time(0);
//	__time64_t t0 = g_pData->GetServerCurrentTime();
//
//	double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );
//
//	int remainDay =  (int)(fDiffTime / ( 60 * 60 * 24 ) );
//	if ( remainDay > 0 )
//		wstrstm << remainDay << GET_STRING( STR_ID_14 ) << L" ";
//
//	int remainHour = ((time_t)fDiffTime % ( 60 * 60 * 24 )) / (60 * 60);
//	if ( remainHour < 0 )
//		remainHour = 0;
//
//	wstrstm << remainHour << GET_STRING( STR_ID_310 ) << L" ";
//
//	int remainMinute = ((time_t)fDiffTime % ( 60 * 60 )) / (60 );
//
//	if ( remainDay <= 0 )
//	{
//		if ( remainMinute <= 0 )
//			remainMinute = 0;
//		wstrstm << remainMinute << GET_STRING( STR_ID_311 ) << L" ";
//	}
//
//	tempDesc += wstrstm.str().c_str();
//
//
//	return tempDesc;
//
//}







void CX2UISkillTree::OpenDLGSkillUnsealed( bool bOpen )
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



void CX2UISkillTree::UpdateDLGSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID )
{
	if( m_pDLGSkillUnsealed == NULL )
		return;


	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
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

//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
bool CX2UISkillTree::ProcessSkillTreeSlotRMouseUp( LPARAM lParam )
{
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState && false == pNowState->GetEnableShortCutKey() )	/// ����Ű�� ��� ���ϴ� ���̸�
		return true;	/// �巡�׾� ����� ���ϰ� ����

	CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
	CX2SkillTreeSlotData* pSlotData = (CX2SkillTreeSlotData*)pSlot->GetSlotData();
	if( pSlotData != NULL)
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSlotData->m_eSkillID, 1 );
		if(pSkillTemplet != NULL)
		{		
			if( true == pSkillTemplet->m_bBornSealed && false == m_pUserSkillTree->IsSkillUnsealed(pSlotData->m_eSkillID) )
			{
				D3DXVECTOR2 CenterPos = pSlot->GetPos() + pSlot->GetDialog()->GetPos() + pSlot->GetSize() * 0.5f;
				OpenSkillUnsealInformDLG( pSkillTemplet, CenterPos );
			}
			else if( m_pUserSkillTree->DoIHaveThisSkill( pSlotData->m_eSkillID ) )
			{

				switch(pSkillTemplet->m_eType)
				{
				case CX2SkillTree::ST_ACTIVE:
				case CX2SkillTree::ST_BUFF:
				case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
				case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
					{
						int iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pSlotData->m_eSkillID, false );
						int iAlreadyEquippedSlotBID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pSlotData->m_eSkillID, true );
						// ���� ��ų�� �����Ǿ� ���� ������
						if( -1 == iAlreadyEquippedSlotID && -1 == iAlreadyEquippedSlotBID )					
						{
							/// A ������ ����ִ��� Ȯ��
							iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(CX2SkillTree::SI_NONE, false );
							if( -1 != iAlreadyEquippedSlotID )
							{
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
								g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID, pSlotData->m_eSkillID );
								return true;
							}
							else
							{
								// B  �� ������ ã�Ƽ� �����Ѵ�
								if( true == m_bIsExpandSkillSlot )
								{
									iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(CX2SkillTree::SI_NONE, true );
									if( -1 != iAlreadyEquippedSlotID )
									{
										g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
										g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID+EQUIPPED_SKILL_SLOT_COUNT, pSlotData->m_eSkillID );
										return true;
									}
								}
							}
						}
					} break;
				default:
					break;
				}
			}
		}
	}
	return true;
}

bool CX2UISkillTree::ProcessEquipSlotDropped( LPARAM lParam )
{
	CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
	CX2EquippedSkillSlotData* pSlotData = (CX2EquippedSkillSlotData*)pSlot->GetSlotData();

	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState && false == pNowState->GetEnableShortCutKey() )	/// ����Ű�� ��� ���ϴ� ���̸�
		return false;	/// �巡�׾� ����� ���ϰ� ����

	CKTDGUISlot* pDroppedSlot = pSlot->GetDroppedSlot();
	CKTDGUISlotData* pDroppedSlotData = NULL;
	if( pDroppedSlot != NULL )
	{
		pDroppedSlotData = pDroppedSlot->GetSlotData();
		
		if ( NULL != pDroppedSlotData )
		{
			switch(pDroppedSlotData->m_eSlotDataType)
			{
			case CKTDGUISlotData::SDT_SKILL_TREE:
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
					CX2SkillTreeSlotData* pDropData = (CX2SkillTreeSlotData*) pDroppedSlotData;
#ifndef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
					int iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pDropData->m_eSkillID, false );
					int iAlreadyEquippedSlotBID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pDropData->m_eSkillID, true );

					// ���� ��ų�� ��ų���Կ� ���� �Ǿ� ������ ����
					if( -1 != iAlreadyEquippedSlotID )					
					{
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID, CX2SkillTree::SI_NONE );
					}
					if( -1 != iAlreadyEquippedSlotBID)
					{
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotBID+EQUIPPED_SKILL_SLOT_COUNT, CX2SkillTree::SI_NONE );
					}
#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE

					if ( NULL != pSlotData )
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), pDropData->m_eSkillID );

					return true;
				} break;
			case CKTDGUISlotData::SDT_SKILL_EQUIP:
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
					CX2EquippedSkillSlotData* pDropData = (CX2EquippedSkillSlotData*) pDroppedSlotData;
					if ( NULL != pSlotData  )
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), pDropData->m_eSkillID );
#else SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
					// Skill Swap
					CX2EquippedSkillSlotData* pDropData = (CX2EquippedSkillSlotData*) pDroppedSlotData;
					CX2SkillTree::SKILL_ID SkillIDA = pSlotData->m_eSkillID;
					CX2SkillTree::SKILL_ID SkillIDB = pDropData->m_eSkillID;
					g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), SkillIDB );
					g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pDropData->GetSlotID(), SkillIDA );
#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE

					return true;
				} break;
			default:
				break;
			}
		}
	} 
	return false;
}

bool CX2UISkillTree::ProcessEquipSlotRMouseUp( LPARAM lParam )
{
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState && false == pNowState->GetEnableShortCutKey() )	/// ����Ű�� ��� ���ϴ� ���̸�
		return true;	/// �巡�׾� ����� ���ϰ� ����

	CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
	CX2EquippedSkillSlotData* pSlotData = (CX2EquippedSkillSlotData*)pSlot->GetSlotData();
	if(pSlotData->m_eSkillID != CX2SkillTree::SI_NONE)
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_UnEquip.ogg", false, false );
	}			

	g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), CX2SkillTree::SI_NONE );
	return true;
}

// bool CX2UISkillTree::CheckIsReady()
// {
// 	bool bIsReady = false;
// 
// 	switch ( g_pMain->GetNowStateID() )
// 	{
// 	case CX2Main::XS_PVP_ROOM:
// 		{
// 			CX2PVPRoom* pPvpRoom = g_pData->GetPVPRoom();
// 
// 			if ( pPvpRoom != NULL &&
// 				pPvpRoom->GetMySlot() != NULL && 
// 				pPvpRoom->GetMySlot()->m_bReady == true )
// 				bIsReady = true;
// 		} break;
// 
// 	case CX2Main::XS_VILLAGE_MAP:
// 		{
// 			CX2PartyManager::PartyMemberData* pPartyMemberData = 
// 				g_pData->GetPartyManager()->GetMyPartyMemberData();
// 
// 			if ( pPartyMemberData != NULL &&
// 				pPartyMemberData->m_bGameReady == true )
// 				bIsReady = true;
// 		} break;
// 
// 	default:
// 		break;
// 	}
// 
// 	return bIsReady;
// }
// 
// void CX2UISkillTree::ShowOkAndCancelMsgForUnReady( LPARAM lParam, SKILL_TREE_UI_CUSTOM_MSG eSTUCM )
// {
// 	m_pDLGUnReady = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4958 ),
// 		STUCM_UNREADY_OK, g_pMain->GetNowState(), STUCM_UNREADY_CANCEL );
// 	
// 	m_pDLGUnReady->ClearDummyInt();
// 	m_pDLGUnReady->AddDummyInt( static_cast<int>(lParam) );	// ù��° ���̿� ���� Ŀ���� MSG�� lParam ��( ���⼭�� ���� )
// 	m_pDLGUnReady->AddDummyInt( static_cast<int>(eSTUCM) );	// �ι�° ���� int�� ������ �����ߴ� Ŀ���� MSG�� �����ΰ�
// }
// 
// void CX2UISkillTree::ProcessChangeEquipSlotAfterUnReady()
// {
// 	if ( m_pDLGUnReady == NULL )
// 		return;
// 
// 	LPARAM lParam = m_pDLGUnReady->GetDummyInt( 0 );
// 
// 	switch ( m_pDLGUnReady->GetDummyInt( 1 ) )
// 	{
// 	case STUCM_SKILLTREESLOT_RMOUSEUP:
// 		ProcessSkillTreeSlotRMouseUp( lParam );
// 		break;
// 
// 	case STUCM_EQUIPSLOT_DROPPED:
// 		ProcessEquipSlotDropped( lParam );
// 		break;
// 
// 	case STUCM_EQUIPSLOT_RMOUSEUP:
// 		ProcessEquipSlotRMouseUp( lParam );
// 		break;
// 
// 	default:
// 		break;
// 
// 	}
// 
// 	g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUnReady, NULL, false );
// }
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // ���� ��ų ���� ����

#ifdef ELSWORD_WAY_OF_SWORD
void CX2UISkillTree::DrawElswordWSSDesc( D3DXVECTOR2 vDrawPos_ )
{
	if( CX2Unit::UT_ELSWORD != g_pData->GetMyUser()->GetSelectUnit()->GetType() )
	{
		return;
	}

	if( m_pDLGElswordWSSDesc == NULL)
	{
		m_pDLGElswordWSSDesc = new CKTDGUIDialog( g_pMain->GetNowState(), L"", 0.07f, XDL_POP_UP );
		m_pDLGElswordWSSDesc->SetFront(true);
		m_pDLGElswordWSSDesc->SetCloseOnFocusOut(true);

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElswordWSSDesc );
		m_pDLGElswordWSSDesc->SetFront( true );
		m_pDLGElswordWSSDesc->SetModal( true );

		CKTDGUIStatic* pStatic = new CKTDGUIStatic();
		pStatic->SetName( L"ItemDesc" );
		m_pDLGElswordWSSDesc->AddControl( pStatic );
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

	int itemDescWidth = 300;
	int itemDescHeight = 200;

	int itemDescPosX = (int)vDrawPos_.x;
	int itemDescPosY = (int)vDrawPos_.y;
	
	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		wstringstream wstrm;
		
		switch( g_pX2Game->GetMyUnit()->GetWayOfSwordState() )
		{
			case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14553) << L"\n" <<  L"#CX" <<
						L"\n" << L"#CFF4444" << L"\n" << GET_STRING(STR_ID_14554) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14553) << L"\n" <<  L"#CX" <<
						L"\n" << L"#CFF4444" << L"\n" << GET_STRING(STR_ID_14554) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case 0: // CX2GUElsword_SwordMan::WSS_CENTER
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14555) << L"\n" <<  
						L"\n" << L"#C999999" << L"\n" << GET_STRING(STR_ID_14556) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14557) <<  L"#CX" << 
						L"\n" << L"#C4488FF" << L"\n" << GET_STRING(STR_ID_14558) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14557) <<  L"#CX" << 
						L"\n" << L"#C4488FF" << L"\n" << GET_STRING(STR_ID_14558) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			default:
				m_pDLGElswordWSSDesc->SetShow( false );
		}
		wstring wstrDesc = wstrm.str();
		CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*)m_pDLGElswordWSSDesc->GetControl(L"ItemDesc");

		const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pItemDesc->GetString(0)->fontIndex );

		const int MAGIC_ELSWORD_DESC_WIDTH = 330;
		wstrDesc = g_pMain->GetStrByLineBreakColor(wstrDesc.c_str(), (int)((float)MAGIC_ELSWORD_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), SLOT_MANAGER_FONT_INDEX);

		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
		int itemDescWidth = (int)( pItemDescFont->GetWidth( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
		int itemDescHeight = (int)( pItemDescFont->GetHeight( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;


		m_pDLGElswordWSSDesc->SetPos( D3DXVECTOR2( (float)itemDescPosX, (float)itemDescPosY ) );
		m_pDLGElswordWSSDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
		m_pDLGElswordWSSDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );		
		m_pDLGElswordWSSDesc->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrDesc.c_str();
	}
	
	return;
}


#endif // ELSWORD_WAY_OF_SWORD

#endif // NEW_SKILL_TREE_UI


//x2game�� �����Ǿ� ���� ���� �� ����ϴ� ��Ÿ�Ӱ��� �Լ�.
void CX2UISkillTree::UpdateSkillCoolTime( float fElapsedTime )
{
	if( g_pData != NULL &&
		g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
		
		if ( m_pDLGGameSkillSlot != NULL )
		{
			CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time" );
			CKTDGUIStatic* pStatic_Skill_Cool_Time_Fade = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time_Fade" );

			CKTDGUIStatic* pStaticSlot_A = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_SKILL_SLOT_A" );
			CKTDGUIStatic* pStaticSlot_B = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_SKILL_SLOT_B" );
			for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; i++ )
			{
				pStaticSlot_A->GetPicture(i)->SetColor( D3DXCOLOR(1.f,1.f,1.f,0.3f));
				pStaticSlot_B->GetPicture(i)->SetColor( D3DXCOLOR(1.f,1.f,1.f,0.3f));
			}
			if ( pStaticCoolTime != NULL && pStatic_Skill_Cool_Time_Fade != NULL )
			{  //B���Ե� ���� �ϱ� ���ؼ� EQUIPPED_SKILL_SLOT_COUNT*2
				for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT*2; i++ )
				{				
					bool bSlotB = (i > 3) ? true : false;
					int iSlotIndex = (i > 3) ? i-4 : i;

					//���� B�� Ȱ��ȭ ���� ������ ���� A�� Ȯ���ϵ��� break;
					if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
						&& bSlotB == true)
					{
						break;
					}

					if( true == bSlotB )
					{
						pStatic_Skill_Cool_Time_Fade = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time_Fade_B" );
						if( NULL == pStatic_Skill_Cool_Time_Fade )
							break;

						pStaticCoolTime = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time_B" );
						if( NULL == pStaticCoolTime )
							break;
					}


					const CX2UserSkillTree::SkillSlotData* pSlotData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB);
					refUserSkillTree.SetSkillCoolTimeLeft(CX2SkillTree::SI_NONE,0.f);
					if(pSlotData != NULL)
					{			
						if( NULL != pStaticSlot_A && NULL != pStaticSlot_B )
						{
							if( CX2SkillTree::SI_NONE != pSlotData->m_eID )
							{// 0~3 : B����, 4~8 : A����				
								if( true == bSlotB )
								{
									pStaticSlot_B->GetPicture(iSlotIndex)->SetColor( D3DXCOLOR(1.f,1.f,1.f,1.f));
								}
								else
								{
									pStaticSlot_A->GetPicture(iSlotIndex)->SetColor( D3DXCOLOR(1.f,1.f,1.f,1.f));
								}
							}
						}

						if(pSlotData->m_fCoolTimeLeft > 0.0f)
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							wstringstream wstrstm;
							wstrstm << (int)( ceil(pSlotData->m_fCoolTimeLeft) );
							pStaticCoolTime->SetString( iSlotIndex, wstrstm.str().c_str() );
						}
						else
						{
							if ( pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
							{
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;
							}

							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}						
					}			
				}
			}
		}
	}
}

/** @function : SwapSkillSlot
	@brief  : ���� ��ų���� Ÿ�Կ� ���� ���� �Լ� ȣ��
*/
void CX2UISkillTree::SwapSkillSlot(bool bVal, bool bForce/* = false*/)
{

#ifdef SKILL_SLOT_UI_TYPE_B
	if( true == m_bIsSkillUITypeA )
	{
		SwapSkillSlotTypeA( bVal, bForce );
	}
	else
	{
		SwapSkillSlotTypeB( bVal, bForce );
	}
#else
	if( m_bSkillSlotSwap == bVal && false == bForce )
		return;

	m_bSkillSlotSwap = bVal;
	if( true == m_bSkillSlotSwap ) 
	{
		D3DXVECTOR2 vOffsetPos(0,0);
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			vOffsetPos.y = ( i >= 4 ) ? 51.f : -51.f;
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
			}
		}	
		vOffsetPos.y = 51.f;
		SetOffsetPosSkillSLotStatic(vOffsetPos);
	}
	else
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos(D3DXVECTOR2(0,0));
			}
		}

		SetOffsetPosSkillSLotStatic(D3DXVECTOR2(0,0));
	}
#endif //SKILL_SLOT_UI_TYPE_B
}

/** @function : SetOffsetPosSkillSLotStatic
	@brief : ��ų���Կ� ���� ����ƽ�� ������ ����	
*/
void CX2UISkillTree::SetOffsetPosSkillSLotStatic(const D3DXVECTOR2& vPos_, const D3DXVECTOR2& vAddPos_/* = D3DXVECTOR2(0,0)*/)
{
	if( NULL == m_pDLGGameSkillSlot )
		return;

	SetOffsetStatic( L"Static_Skill_Cool_Time_Fade", -vPos_ -vAddPos_);
	SetOffsetStatic( L"Static_Skill_Cool_Time_Fade_B", vPos_ );
	SetOffsetStatic( L"Static_Skill_Cool_Time", -vPos_ -vAddPos_);
	SetOffsetStatic( L"Static_Skill_Cool_Time_B", vPos_ );
	SetOffsetStatic( L"Static_SKILL_SLOT_A", -vPos_ -vAddPos_ );
	SetOffsetStatic( L"Static_SKILL_SLOT_B", vPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_B", -vPos_ -vAddPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_B_B", vPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_R", -vPos_ -vAddPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_R_B", vPos_ );
}

/** @function : SetOffsetStatic
	@brief : ��Ʈ�� �̸����� ���� ����ƽ ��ü�� �����°� ����
			SetOffsetPosSkillSLotStatic���� ���Ǵ� �Լ�( �ڵ��ߺ� ���� )
	@param : ��Ʈ���̸� (strControl_), �����°�(vPos)
*/
void CX2UISkillTree::SetOffsetStatic( const WCHAR* strControl_, const D3DXVECTOR2& vPos_)
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl(strControl_));
	if( NULL != pStatic )
	{
		pStatic->SetOffsetPos(vPos_);
	}
}
/** @function : SetShowStatic
	@brief : ��Ʈ�� �̸����� ���� ����ƽ ��ü�� ShowEnable
			SetOffsetPosSkillSLotStatic���� ���Ǵ� �Լ�( �ڵ��ߺ� ���� )
	@param : ��Ʈ���̸� (strControl_),
*/
void CX2UISkillTree::SetShowEnableStatic( const WCHAR* strControl_, bool bShow_, bool bEnable_)
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl(strControl_));
	if( NULL != pStatic )
	{
	}
}
void CX2UISkillTree::UpdateSkillSlotString()
{
	if ( NULL == m_pDLGGameSkillSlot )
		return;

	if ( NULL != g_pMain->GetKeyPad() )
	{
		CKTDGUIStatic* pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKeyEx" );
		if( NULL != pStaticSkillHotKey )
		{
	#ifdef SKILL_SLOT_UI_TYPE_B
			if( false == m_bIsSkillUITypeA )
			{
				pStaticSkillHotKey->GetString(0)->msg = L"";
				pStaticSkillHotKey->GetString(1)->msg = L"";
				pStaticSkillHotKey->GetString(2)->msg = L"";
				pStaticSkillHotKey->GetString(3)->msg = L"";
			}
			else
	#endif //SKILL_SLOT_UI_TYPE_B
			{
				pStaticSkillHotKey->GetString(0)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX1 );
				pStaticSkillHotKey->GetString(1)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX2 );
				pStaticSkillHotKey->GetString(2)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX3 );
				pStaticSkillHotKey->GetString(3)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX4 );
			}
		}

		pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKey" );
		{
			pStaticSkillHotKey->GetString(0)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL1 );
			pStaticSkillHotKey->GetString(1)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL2 );
			pStaticSkillHotKey->GetString(2)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL3 );
			pStaticSkillHotKey->GetString(3)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL4 );
		}
	}
}

#ifdef SKILL_SLOT_UI_TYPE_B
void CX2UISkillTree::SetSkillUIType( bool bIsTypeA_ )
{
	if( NULL == m_pDLGGameSkillSlot)
		return;

	m_bIsSkillUITypeA = bIsTypeA_;


	D3DXVECTOR2 vSlotPos(0,0);
	if( false == m_bIsSkillUITypeA )
	{
		vSlotPos.x = -297.f;
#ifdef INT_WIDE_BAR
		vSlotPos.y = -599.f;
#else // INT_WIDE_BAR
		vSlotPos.y = -619.f;
#endif // INT_WIDE_BAR
	}
	//���� B Ȱ��ȭ ����
	bool bEnableSlotB = false;
	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() )
			bEnableSlotB = true;
	}

	//���� ������ A�������� B�������� �˷��ִ� UI
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl( L"g_pStatic_Slot_Guide" ));
	if( NULL != pStatic )
	{
		if( false == m_bIsSkillUITypeA && //��ų���� BŸ���̰�
			true == bEnableSlotB ) //���� B�� Ȱ��ȭ �Ǿ� �ִٸ�
			pStatic->SetShow(true); 
		else
			pStatic->SetShow(false);
	}

	
	

 	m_pDLGGameSkillSlot->SetPos( vSlotPos );

	SetOffsetPosEtcSkillSlotUI();// B���� ���/���Ű��̵� ��ġ ����
	UpdateSkillSlotString();// B���� ����Ű ��Ʈ�� ����
	SwapSkillSlot( m_bSkillSlotSwap, true );
}

void CX2UISkillTree::SetOffsetPosEtcSkillSlotUI()
{
	if( NULL == m_pDLGGameSkillSlot )
		return;

	D3DXVECTOR2 vOffsetPos(0,0);
	if( false == m_bIsSkillUITypeA )
		vOffsetPos.y = -200.f;

	CKTDGUIStatic* pStaticLock = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl(L"LOCK"));
	if( NULL != pStaticLock )
		pStaticLock->SetOffsetPos(vOffsetPos);

	CKTDGUIButton* pButtonBuyGuide = static_cast<CKTDGUIButton*>(m_pDLGGameSkillSlot->GetControl(L"Button_Buy_Guide_Slot"));
		pButtonBuyGuide->SetOffsetPos(vOffsetPos);

}

/** @function : SwapSkillSlotTypeA
	@brief  : ���/�ϴ� ���� ���� ��ġ ����
*/
void CX2UISkillTree::SwapSkillSlotTypeA(bool bVal, bool bForce/* = false*/)
{
	if( m_bSkillSlotSwap == bVal && false == bForce )
		return;

	m_bSkillSlotSwap = bVal;
	if( true == m_bSkillSlotSwap ) 
	{
		D3DXVECTOR2 vOffsetPos(0,0);
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			vOffsetPos.y = ( i >= 4 ) ? 51.f : -51.f;
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
			}
		}	
		vOffsetPos.y = 51.f;
		SetOffsetPosSkillSLotStatic(vOffsetPos);
	}
	else
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos(D3DXVECTOR2(0,0));
			}
		}

		SetOffsetPosSkillSLotStatic(D3DXVECTOR2(0,0));
	}
}


/** @function : SwapSkillSlotTypeB
	@brief  : ��� ������ ȭ�鿡�� �Ⱥ��̵��� ���� �����鼭 ��ġ ����
*/
void CX2UISkillTree::SwapSkillSlotTypeB(bool bVal_, bool bForce_/* = false */)
{
	if( m_bSkillSlotSwap == bVal_ && false == bForce_)
		return;

	m_bSkillSlotSwap = bVal_;

	D3DXVECTOR2 vOffsetPos(0,0);	// ���� ��ü�� ���� �⺻ ������ ��
	D3DXVECTOR2 vOffsetAddPos(0,0); // ��� ������ ���� �� ������ ���� �߰� ������ ��

	if( true == m_bSkillSlotSwap ) //���� ��ü ��(B������ �ϴܿ� ������ ����)
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )//4~7 ��ܽ���
		{
			vOffsetPos.y = ( i >= 4 ) ? 51.f : -200.f;
			if( NULL != GetEquippedSlot(i) )
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
		}	
		vOffsetPos.y = 51.f;
		vOffsetAddPos.y = 200.f;

	}
	else //�⺻ ����( A������ �ϴܿ� ������ ����)
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i ) //4~7 ��ܽ���
		{
			if( NULL != GetEquippedSlot(i) )
				vOffsetPos.y = ( i >= 4 ) ? -150.f : 0.f;
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
		}

		vOffsetPos.y = -200.f;
		vOffsetAddPos.y = 200.f;
	}
	SetOffsetPosSkillSLotStatic(vOffsetPos, vOffsetAddPos);
}
#endif //SKILL_SLOT_UI_TYPE_B

#endif UPGRADE_SKILL_SYSTEM_2013