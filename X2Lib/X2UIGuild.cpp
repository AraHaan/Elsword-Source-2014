//{{ ����� : [2009/9/8] //	��� UI�Ŵ��� Ŭ����

#include "StdAfx.h"
#include "X2UIGuild.h"

#ifdef GUILD_MANAGEMENT
CX2UIGuild::CX2UIGuild() :
m_pDlgCreateGuild( NULL ),
m_pDlgCreateGuildNotice( NULL ),
m_pDlgChangeGradeMenu( NULL ),
m_pDlgGuildInfo( NULL ),
m_pDlgInviteGuild( NULL ),
m_pDlgChangeGuildMessage( NULL ),
m_pDlgChangeGuildMemberMessage( NULL ),
m_cGuildInfoTabType( GIT_GUILD_INFO ),
m_pGuildInfoControl( NULL ),
m_pGuildMemberInfoControl( NULL ),
#ifdef GUILD_SKILL
m_pGuildSkillTree( NULL),
#endif GUILD_SKILL
#ifdef SERV_GUILD_CHANGE_NAME
m_pDlgChangeGuildName( NULL ),
#endif //CHANGE_GUILD_NAME
m_cNowMemberInfoPage( 1 ),
m_cMaxMemberInfoPage( 1 )
{
	for(int i=0; i<8; ++i)
	{
		m_pGuildMemberIcon[i] = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Character_Icon.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pGuildMemberIcon[i] );
		m_pGuildMemberIcon[i]->SetLayer( XDL_GAME_EDIT );
		m_pGuildMemberIcon[i]->SetShowEnable(false, false);
	}	
}

CX2UIGuild::~CX2UIGuild()
{
	SAFE_DELETE_DIALOG(m_pDlgCreateGuild);
	SAFE_DELETE_DIALOG(m_pDlgChangeGradeMenu);
	SAFE_DELETE_DIALOG( m_pDlgInviteGuild );
	
	if ( m_pDlgGuildInfo != NULL )
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgGuildInfo, NULL, false );
	m_pDlgGuildInfo = NULL;

	SAFE_DELETE( m_pGuildInfoControl );
	SAFE_DELETE( m_pGuildMemberInfoControl );

	for(int i=0; i<8; ++i)
		SAFE_DELETE_DIALOG( m_pGuildMemberIcon[i] )

#ifdef SERV_GUILD_CHANGE_NAME
		SAFE_DELETE_DIALOG(m_pDlgChangeGuildName);
#endif //CHANGE_GUILD_NAME
}
#ifdef SERV_GUILD_CHANGE_NAME
bool CX2UIGuild::SetShowChangeGuildNameDlg( bool bEnable )
{	
	if(bEnable == true)
	{
		if( m_pDlgChangeGuildName == NULL )
		{
			m_pDlgChangeGuildName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Change_Guild_Name.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgChangeGuildName ); 
			m_pDlgChangeGuildName->SetLayer(XDL_POP_UP); 
		}
		m_pDlgChangeGuildName->SetShow( true ); 

		return true;
	}
	else
	{
		if(m_pDlgChangeGuildName != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgChangeGuildName, NULL, false ); 
			m_pDlgChangeGuildName = NULL;
		} 
		return true;
	}

	return false;

}
#endif //CHANGE_GUILD_NAME
bool CX2UIGuild::SetShowCreateGuildDlg( bool bEnable, UidType iUID )
{
	if(bEnable == true)
	{
		g_pData->GetGuildManager()->SetUseGuildItemUID( iUID );

		m_pDlgCreateGuild = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Guild_License_Establish.LUA" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgCreateGuild );

		return true;
	}
	else
	{
		if(m_pDlgCreateGuild != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgCreateGuild, NULL, false );
			g_pData->GetGuildManager()->SetUseGuildItemUID( 0 );
			m_pDlgCreateGuild = NULL;
		}

		return true;
	}
	
	return false;
}

bool CX2UIGuild::SetShowCreateGuildNoticeDlg( bool bEnable, const WCHAR *pGuildName, const WCHAR *pGuildMasterName )
{
	if(bEnable == true)
	{
		m_pDlgCreateGuildNotice = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Guild_License_Notice.LUA" );
		
		
		if( m_pDlgCreateGuildNotice != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgCreateGuildNotice );

			//	���� �Է�
			//	��帶���� �Է�
			
			CKTDGUIStatic *pStaticGuildName			= (CKTDGUIStatic*)m_pDlgCreateGuildNotice->GetControl(L"Static_Notice");
			CKTDGUIStatic *pStaticGuildMasterName	= (CKTDGUIStatic*)m_pDlgCreateGuildNotice->GetControl(L"Static_MasterName");

			if( pStaticGuildName == NULL || pStaticGuildMasterName == NULL )
			{
				return false;
			}

			pStaticGuildName->SetString(0, GET_REPLACED_STRING( ( STR_ID_4747, "S", pGuildName ) ) );
			pStaticGuildMasterName->SetString(0, pGuildMasterName);

			return true;
		}

		return false;
	}
	else
	{
		if(m_pDlgCreateGuildNotice != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgCreateGuildNotice, NULL, false );
			m_pDlgCreateGuildNotice = NULL;
		}

		return true;
	}

	return false;
}

bool CX2UIGuild::SetShowInviteGuildMemberDlg( bool bEnable )
{
	if(bEnable == true)
	{
		m_pDlgInviteGuild = g_pMain->KTDGUIOkAndCancelEditBox( D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_4540 ), CX2GuildManager::GUM_GUILD_INVITE_MEMBER_OK, g_pMain->GetNowState(), 20, true, CX2GuildManager::GUM_GUILD_INVITE_MEMBER_CANCEL );

		return true;
	}
	else
	{
		if(m_pDlgInviteGuild != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgInviteGuild, NULL, false );
			m_pDlgInviteGuild = NULL;
		}

		return true;
	}

	return false;
}

bool	CX2UIGuild::SetShowChangeGradeMenu( bool bEnable, int iTargetGrade )
{
	if( bEnable == true )
	{
		int iMyGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;

		//if( iGrade > 2 || iGrade == 0 ) //	��� ������, ���������� �˻�
		if( iMyGrade != CX2GuildManager::GUG_MASTER && iMyGrade != CX2GuildManager::GUG_SYSOP )
		{
			return false;
		}

		//	�� ����� ���� ��޺��� ������ �˻�.
		if( iMyGrade >= iTargetGrade || iTargetGrade == CX2GuildManager::GUG_INVALID )
		{
			return false;
		}

		float menuPosX = 0.f;
		float menuPosY = 0.f;
		float menuHeight = 22.f;
		int	countMenuHeight = 0;

		m_pDlgChangeGradeMenu = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Guild_Menu.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgChangeGradeMenu );
		m_pDlgChangeGradeMenu->SetShowEnable( true, true );

		CKTDGUIButton *pMenuMaster	= (CKTDGUIButton*) m_pDlgChangeGradeMenu->GetControl( L"Master" );
		CKTDGUIButton *pMenuAdmin	= (CKTDGUIButton*) m_pDlgChangeGradeMenu->GetControl( L"Admin" );
		CKTDGUIButton *pMenuOld		= (CKTDGUIButton*) m_pDlgChangeGradeMenu->GetControl( L"Old" );
		CKTDGUIButton *pMenuPremium	= (CKTDGUIButton*) m_pDlgChangeGradeMenu->GetControl( L"Premium" );
		CKTDGUIButton *pMenuCommon	= (CKTDGUIButton*) m_pDlgChangeGradeMenu->GetControl( L"Common" );
		CKTDGUIButton *pMenuFresh	= (CKTDGUIButton*) m_pDlgChangeGradeMenu->GetControl( L"Fresh" );

		CKTDGUIStatic *pStaticGuild = (CKTDGUIStatic*)m_pDlgChangeGradeMenu->GetControl(L"Guild");

		CKTDGUIControl::CPictureData*	pPicMiddle1 = pStaticGuild->GetPictureIndex(1);
		CKTDGUIControl::CPictureData*	pPicBottom1 = pStaticGuild->GetPictureIndex(2);
		CKTDGUIControl::CPictureData*	pPicMiddle2 = pStaticGuild->GetPictureIndex(3);
		CKTDGUIControl::CPictureData*	pPicBottom2 = pStaticGuild->GetPictureIndex(4);

		//	���ҽ� �ʱ�ȭ
		pMenuMaster->SetShowEnable( false, false );
		pMenuAdmin->SetShowEnable( false, false );
		pMenuOld->SetShowEnable( false, false );
		pMenuPremium->SetShowEnable( false, false );
		pMenuCommon->SetShowEnable( false, false );
		pMenuFresh->SetShowEnable( false, false );

		//	�� ��� ��޿� ���� �޴� �׸� ����
		switch( iMyGrade )
		{
		case CX2GuildManager::GUG_MASTER:	//	��� ������ �޴�
			{
				//	���� ����
				if( iTargetGrade != CX2GuildManager::GUG_NEW_USER )
				{
					pMenuFresh->SetShowEnable( true, true );
					pMenuFresh->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	�Ϲ� ����
				if( iTargetGrade != CX2GuildManager::GUG_NORMAL_USER )
				{
					pMenuCommon->SetShowEnable( true, true );
					pMenuCommon->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	��� ����
				if( iTargetGrade != CX2GuildManager::GUG_VIP_USER )
				{
					pMenuPremium->SetShowEnable( true, true );
					pMenuPremium->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	���� ����
				if( iTargetGrade != CX2GuildManager::GUG_OLD_USER )
				{
					pMenuOld->SetShowEnable( true, true );
					pMenuOld->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	��� ������
				if( iTargetGrade != CX2GuildManager::GUG_SYSOP )
				{
					pMenuAdmin->SetShowEnable( true, true );
					pMenuAdmin->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	��� ������
				if( iTargetGrade != CX2GuildManager::GUG_MASTER )
				{
					pMenuMaster->SetShowEnable( true, true );
					pMenuMaster->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				
				break;
			}
		case CX2GuildManager::GUG_SYSOP:	//	��� ������ �޴�
			{
				//	���� ����
				if( iTargetGrade != CX2GuildManager::GUG_NEW_USER )
				{
					pMenuFresh->SetShowEnable( true, true );
					pMenuFresh->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	�Ϲ� ����
				if( iTargetGrade != CX2GuildManager::GUG_NORMAL_USER )
				{
					pMenuCommon->SetShowEnable( true, true );
					pMenuCommon->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	��� ����
				if( iTargetGrade != CX2GuildManager::GUG_VIP_USER )
				{
					pMenuPremium->SetShowEnable( true, true );
					pMenuPremium->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				//	���� ����
				if( iTargetGrade != CX2GuildManager::GUG_OLD_USER )
				{
					pMenuOld->SetShowEnable( true, true );
					pMenuOld->SetOffsetPos_LUA(menuPosX, menuPosY + (menuHeight * countMenuHeight));
					countMenuHeight++;
				}
				
				break;
			}
		}

		//	���� ����
		pPicMiddle1->SetSizeY(countMenuHeight * menuHeight);
		pPicMiddle2->SetSizeY(countMenuHeight * menuHeight);

		D3DXVECTOR2 offsetPos;
		offsetPos = pPicMiddle1->GetPos();
		offsetPos.y += (countMenuHeight * menuHeight);
		pPicBottom1->SetPos(offsetPos);

		offsetPos = pPicMiddle2->GetPos();
		offsetPos.y += (countMenuHeight * menuHeight);
		pPicBottom2->SetPos(offsetPos);

		D3DXVECTOR2 vMousePos;
		vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
		vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
		vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

		//	���� ����
		// �˾��޴� ��� ��ġ ����

		// �˾� �޴��� ȭ�� ������ ����� �ʰ� ����
		if( vMousePos.x > 860.f )
		{
			vMousePos.x = 860.f;
		}

		if( vMousePos.x < 0.f )
		{
			vMousePos.x = 0.f;
		}

		if( vMousePos.y > 700.f )
		{
			vMousePos.y = 700.f;
		}

		if( vMousePos.y < 0.f )
		{
			vMousePos.y = 0.f;
		}

		m_pDlgChangeGradeMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y-30.f ) );

		return true;
	}
	else //	 bEnable == false
	{
		if(m_pDlgChangeGradeMenu != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgChangeGradeMenu, NULL, false );
			m_pDlgChangeGradeMenu = NULL;
		}

		return true;
	}

	return false;
}

bool CX2UIGuild::SetShowGuildInfo( bool bEnable )
{
	if( bEnable == true )
	{
		if( m_pDlgGuildInfo == NULL )	//	�׽�Ʈ�� ���Ͽ� �ּ� ǰ
		{
			m_pDlgGuildInfo = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Guild_Info.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgGuildInfo );
		}

		m_pDlgGuildInfo->SetShowEnable( true, true );
		SetGuildInfoTab( GIT_GUILD_INFO );
		return true;
	}
	else	//	bEnable == false
	{
		if( m_pDlgGuildInfo != NULL )
		{
			m_pDlgGuildInfo->SetShowEnable( false, false );

			for(int i = 0; i < 8; i++)
			{
				ClearUserIcon(i);
			}
		}
#ifdef GUILD_SKILL
		//{{ oasis907 : ����� //// 2009-11-09 //// 

		m_pGuildSkillTree->SetShow(false);

		//{{ oasis907 : ����� //// 2009-11-09 //// 
#endif GUILD_SKILL
		return true;
	}

	return false;
}














bool		CX2UIGuild::SetShowChangeGuildMessageDlg( bool bEnable )
{
	if(bEnable == true)
	{
		m_pDlgChangeGuildMessage = g_pMain->KTDGUIOkAndCancelEditBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4598 ), CX2GuildManager::GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_OK, g_pMain->GetNowState(), 90, true, CX2GuildManager::GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_CANCEL );

		return true;
	}
	else
	{
		if(m_pDlgChangeGuildMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgChangeGuildMessage, NULL, false );
			m_pDlgChangeGuildMessage = NULL;
		}

		return true;
	}

	return false;
}

bool		CX2UIGuild::SetGuildInfoTab( char cTab )
{
	if( g_pData->GetGuildManager() == NULL || m_pDlgGuildInfo == NULL )
	{
		return false;
	}

	if( cTab != GIT_CURRENT_TAB )
	{
		m_cGuildInfoTabType = cTab;
	}

	if( m_pGuildInfoControl == NULL || m_pGuildMemberInfoControl == NULL )
	{
		ResetGuildInfoControl();
		ResetGuildMemberInfoControl();

	}
#ifdef GUILD_SKILL
	//{{ oasis907 : ����� //// 2009-11-09 //// 
	if( m_pGuildSkillTree == NULL)
	{
		CreateUIGuildSkillTree();
	}
	//}} oasis907 : ����� //// 2009-11-09 //// 
#endif GUILD_SKILL

	m_pGuildInfoControl->SetShow( false );
	m_pGuildMemberInfoControl->SetShow( false );
#ifdef GUILD_SKILL
	//{{ oasis907 : ����� //// 2009-11-09 //// 
	m_pGuildSkillTree->SetShow(false);
	//}} oasis907 : ����� //// 2009-11-09 //// 







#endif GUILD_SKILL
	switch( m_cGuildInfoTabType )
	{
	case GIT_GUILD_INFO:
		{
			// oasis907 : ����� [2009.12.2] //
			m_pDlgGuildInfo->SetShowEnable( true, true );


			for(int i = 0; i < 8; i++)
			{
				ClearUserIcon(i);
			}

			CKTDGUIStatic* pStatic_Master			= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"Master_Nick" ) );			//	��� ������
			CKTDGUIStatic* pStatic_MemberNum		= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"Count" ) );				//	�����ο�
			CKTDGUIStatic* pStatic_MemberMaxNum		= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"MaxCount" ) );				//	�����ο�
			CKTDGUIStatic* pStatic_FoundingDay		= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"Date" ) );				//	��� â����
			CKTDGUIStatic* pStatic_GuildMessage		= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"GuildMessage" ) );		//	��� �޽���
			//CKTDGUIStatic* pStatic_GuildLevel		= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"LV" ) );					//	��� ����
			//CKTDGUIStatic* pStatic_GuildEXP			= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"EXP" ) );					//	��� ����ġ
			CKTDGUIRadioButton* pRadio_GuildInfo	= static_cast<CKTDGUIRadioButton*>( m_pGuildInfoControl->GetControl( L"Radio_Guild_Info" ) );	//	��� ���� ��ư

			if( pStatic_Master == NULL ||
				pStatic_MemberNum == NULL ||
				pStatic_FoundingDay == NULL ||
				pStatic_GuildMessage == NULL ||
				/*pStatic_GuildLevel == NULL ||
				pStatic_GuildEXP == NULL ||*/
				pRadio_GuildInfo == NULL )
			{
				return false;
			}

			//	��� ������
			pStatic_Master->SetString(0, g_pData->GetGuildManager()->GetMyGuildInfo()->m_wstrGuildMasterNickName.c_str() );
			//	���� �ο�
			wstringstream wstrMember;
			wstrMember << g_pData->GetGuildManager()->GetMyGuildInfo()->m_usNowNumOfGuildMember;
			pStatic_MemberNum->SetString(0, wstrMember.str().c_str() );
			//	���� �ο�
			wstringstream wstrMaxMember;
			wstrMaxMember << g_pData->GetGuildManager()->GetMyGuildInfo()->m_usMaxNumOfGuildMember;
			pStatic_MemberMaxNum->SetString(0, wstrMaxMember.str().c_str() );

			//	���� ���
			CKTDGUIButton* pButton_SysopDesc	= static_cast<CKTDGUIButton*>( m_pGuildInfoControl->GetControl( L"SysopDesc" ) );	//	������ ��
			pButton_SysopDesc->SetGuideDesc( GET_REPLACED_STRING( ( STR_ID_4756, "i", g_pData->GetGuildManager()->GetMyGuildInfo()->m_byMaxNumOfGuildAdmin ) ) );

			//	��� â����
			pStatic_FoundingDay->SetString(0, (g_pData->GetGuildManager()->GetMyGuildInfo()->m_wstrFoundingDay.substr(0, 10)).c_str() );
			//	��� �޽���
			wstring wstrCutString = CutString( pStatic_GuildMessage, 275, g_pData->GetGuildManager()->GetMyGuildInfo()->m_wstrGuildMessage.c_str(), L"..." );
			pStatic_GuildMessage->SetString(0, wstrCutString.c_str() );
			
			CKTDGUIButton* pButton_MessageDesc	= static_cast<CKTDGUIButton*>( m_pGuildInfoControl->GetControl( L"ButtonMessageDesc" ) );	//	��� ���� ��ư
			pButton_MessageDesc->SetGuideDesc( g_pData->GetGuildManager()->GetGuildInfo()->m_wstrGuildMessage.c_str() );

			//	�׻� 100%(���߿� ����)

			BYTE	byNowGuildLevel		= g_pData->GetGuildManager()->GetMyGuildInfo()->m_byGuildLevel;
						
			UpdateGuildLevelBar( g_pData->GetGuildManager()->GetNowGuildExp( byNowGuildLevel ) - g_pData->GetGuildManager()->GetTotalGuildExp( byNowGuildLevel ), g_pData->GetGuildManager()->GetNextLevelNeedExp( byNowGuildLevel ) );
			UpdateGuildHonorBar( g_pData->GetGuildManager()->GetNowMyGuildHonorPoint( byNowGuildLevel ), _CONST_GUILD_MANAGER_::g_iMaxGuildHonorPoint );
			UpdateGuildLevelStr();
			

			//	������� �� ����
			pRadio_GuildInfo->SetChecked( true );

			m_pGuildInfoControl->SetShowEnable( true, true );
			//m_pGuildSkillTree->SetShow(false);
		}
		break;
	case GIT_GUILD_MEMBER_INFO:
		{
			// oasis907 : ����� [2009.12.2] //
			m_pDlgGuildInfo->SetShowEnable( true, true );


			CKTDGUIRadioButton*	pRadio_GuildMemberInfo	= static_cast<CKTDGUIRadioButton*>( m_pGuildMemberInfoControl->GetControl( L"Radio_Guild_Member_Info" ) );	//	��� ���� ��ư
			if( pRadio_GuildMemberInfo == NULL )
			{
				return false;
			}

			//	�������� �� ����
			pRadio_GuildMemberInfo->SetChecked( true );
			m_pGuildMemberInfoControl->SetShowEnable( true, true );

			//m_pGuildSkillTree->SetShow(false);
		}
		break;
#ifdef GUILD_SKILL
	//{{ oasis907 : ����� //// 2009-11-09 //// 
	case GIT_GUILD_SKILL:
		{

			for(int i = 0; i < 8; i++)
			{
				ClearUserIcon(i);
			}


			CKTDGUIRadioButton*	pRadio_GuildInfo = static_cast<CKTDGUIRadioButton*>( m_pDlgGuildInfo->GetControl( L"Radio_Guild_Info" ) );
			CKTDGUIRadioButton*	pRadio_GuildMemberInfo = static_cast<CKTDGUIRadioButton*>( m_pDlgGuildInfo->GetControl( L"Radio_Guild_Member_Info" ) ); 
			CKTDGUIRadioButton*	pRadio_GuildSkill = static_cast<CKTDGUIRadioButton*>( m_pDlgGuildInfo->GetControl( L"RadioButtonGuild_Skill" ) ); 

			if( pRadio_GuildInfo == NULL || 
				pRadio_GuildMemberInfo == NULL  ||
				pRadio_GuildSkill == NULL )
			{
				return false;
			}

			// oasis907 : ����� [2009.12.2] //
			//pRadio_GuildSkill->SetChecked( false );
			
			pRadio_GuildInfo->SetShow(true);
			pRadio_GuildMemberInfo->SetShow(true);
			pRadio_GuildSkill->SetShow(true);

				
			//{{ oasis907 : ����� [2009.12.2] GuildInfo, GuildMemberInfo ���̾�α׿� GuildSkill ���̾�αװ� ���� �����ʴ� ������ ó�� 
			                                    // (������ �� ���̾�α��� ����� �������� �����Ǵ°��� �ٶ���)
			pRadio_GuildInfo->SetShow(false);
			pRadio_GuildMemberInfo->SetShow(false);
			pRadio_GuildSkill->SetShow(false);

			SetShowGuildInfo(false);
			//}} oasis907 : ����� [2009.12.2] //

			m_pGuildSkillTree->SetShow(true);
		}
		break;
		//}} oasis907 : ����� //// 2009-11-09 //// 
#endif GUILD_SKILL 
	}


	// �׽�Ʈ ���� �ӽ� �ּ�
	// ���� �߿� ��� ��ų �ٲ� �� ���� 
	//{{ oasis907 : ����� [2009.12.3] //
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
		{
			CKTDGUIRadioButton*	pRadio_GuildSkill = static_cast<CKTDGUIRadioButton*>( m_pDlgGuildInfo->GetControl( L"RadioButtonGuild_Skill" ) ); 
			pRadio_GuildSkill->SetShowEnable( true, false );
		} break;
	default:
		{

		} break;
	}
	//}} oasis907 : ����� [2009.12.3] //







	return false;
}

bool CX2UIGuild::GetShowGuildInfo() const
{
	if ( m_pDlgGuildInfo == NULL )
		return false;

	return m_pDlgGuildInfo->GetShow();
}

void		CX2UIGuild::ResetGuildInfoControl()
{
	if( m_pDlgGuildInfo == NULL )
		return;

	if( m_pGuildInfoControl == NULL )
	{
		m_pGuildInfoControl = new GuildInfoControl();
	}

	m_pGuildInfoControl->Clear();
	m_pGuildInfoControl->SetParentControl( m_pDlgGuildInfo );

	m_pGuildInfoControl->Insert( L"g_pStaticGuild_Info" );
	m_pGuildInfoControl->Insert( L"g_pStaticGuild_EXP" );
	m_pGuildInfoControl->Insert( L"Master_Nick" );
	m_pGuildInfoControl->Insert( L"Count" );
	m_pGuildInfoControl->Insert( L"MaxCount" );
	m_pGuildInfoControl->Insert( L"Date" );
	m_pGuildInfoControl->Insert( L"GuildMessage" );
	m_pGuildInfoControl->Insert( L"LV" );
	m_pGuildInfoControl->Insert( L"EXP" );
	m_pGuildInfoControl->Insert( L"EXP2" );
	m_pGuildInfoControl->Insert( L"Title1" );
	m_pGuildInfoControl->Insert( L"Title2" );
	m_pGuildInfoControl->Insert( L"Title3" );
	m_pGuildInfoControl->Insert( L"Title4" );
	m_pGuildInfoControl->Insert( L"Title5" );
	m_pGuildInfoControl->Insert( L"Title6" );
	m_pGuildInfoControl->Insert( L"Title7" );
	m_pGuildInfoControl->Insert( L"BG_GuildLeave" );
	m_pGuildInfoControl->Insert( L"BTN_GuildLeave" );
	m_pGuildInfoControl->Insert( L"BG_GuildDestroy" );
	m_pGuildInfoControl->Insert( L"BTN_GuildDestroy" );
	m_pGuildInfoControl->Insert( L"BG_ChangeMessage" );
	m_pGuildInfoControl->Insert( L"BTN_ChangeMessage" );
	m_pGuildInfoControl->Insert( L"Radio_Guild_Info" );
	m_pGuildInfoControl->Insert( L"Radio_Guild_Member_Info" );
	m_pGuildInfoControl->Insert( L"RadioButtonGuild_Skill" );
	m_pGuildInfoControl->Insert( L"ButtonMessageDesc" );
	m_pGuildInfoControl->Insert( L"ButtonLevelDesc" );
	m_pGuildInfoControl->Insert( L"ButtonHonorDesc" );
	m_pGuildInfoControl->Insert( L"SysopDesc" );
	CKTDGUIButton *pButton1 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMessageDesc" ) );
	CKTDGUIButton *pButton2 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonLevelDesc" ) );
	CKTDGUIButton *pButton3 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"SysopDesc" ) );
	
	//	������ �����ش� ( ����, add : �� ������ ��ũ��Ʈ���� �⺻���� ����ϴ� �޽��� ����ϰ� ���� �� )
	if( pButton1 != NULL &&
		pButton2 != NULL &&
		pButton3 != NULL )
	{
		pButton1->SetGuideDesc( L"" );
		pButton2->SetGuideDesc( L"" );
		pButton3->SetGuideDesc( L"" );
	}
}

void		CX2UIGuild::ResetGuildMemberInfoControl()
{
	if( m_pDlgGuildInfo == NULL )
		return;

	if( m_pGuildMemberInfoControl == NULL )
	{
		m_pGuildMemberInfoControl = new GuildInfoControl();
	}

	m_pGuildMemberInfoControl->Clear();
	m_pGuildMemberInfoControl->SetParentControl( m_pDlgGuildInfo );

	m_pGuildMemberInfoControl->Insert( L"g_pStaticGuild_Member_Info" );
	m_pGuildMemberInfoControl->Insert( L"Title" );
	m_pGuildMemberInfoControl->Insert( L"Title_Introduce" );
	m_pGuildMemberInfoControl->Insert( L"Nick01" );
	m_pGuildMemberInfoControl->Insert( L"Nick02" );
	m_pGuildMemberInfoControl->Insert( L"Nick03" );
	m_pGuildMemberInfoControl->Insert( L"Nick04" );
	m_pGuildMemberInfoControl->Insert( L"Nick05" );
	m_pGuildMemberInfoControl->Insert( L"Nick06" );
	m_pGuildMemberInfoControl->Insert( L"Nick07" );
	m_pGuildMemberInfoControl->Insert( L"Nick08" );
	m_pGuildMemberInfoControl->Insert( L"Introduce01" );
	m_pGuildMemberInfoControl->Insert( L"Introduce02" );
	m_pGuildMemberInfoControl->Insert( L"Introduce03" );
	m_pGuildMemberInfoControl->Insert( L"Introduce04" );
	m_pGuildMemberInfoControl->Insert( L"Introduce05" );
	m_pGuildMemberInfoControl->Insert( L"Introduce06" );
	m_pGuildMemberInfoControl->Insert( L"Introduce07" );
	m_pGuildMemberInfoControl->Insert( L"Introduce08" );
	m_pGuildMemberInfoControl->Insert( L"BG_ChangeIntroduce" );
	m_pGuildMemberInfoControl->Insert( L"BTN_ChangeIntroduce" );
	m_pGuildMemberInfoControl->Insert( L"page_number" );
	m_pGuildMemberInfoControl->Insert( L"Inventory_arrow_left" );
	m_pGuildMemberInfoControl->Insert( L"Inventory_arrow_right" );
	m_pGuildMemberInfoControl->Insert( L"Radio_Guild_Info" );
	m_pGuildMemberInfoControl->Insert( L"Radio_Guild_Member_Info" );
	m_pGuildMemberInfoControl->Insert( L"RadioButtonGuild_Skill" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce01" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce02" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce03" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce04" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce05" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce06" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce07" );
	m_pGuildMemberInfoControl->Insert( L"BTN_DeleteIntroduce08" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc01" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc02" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc03" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc04" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc05" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc06" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc07" );
	m_pGuildMemberInfoControl->Insert( L"ButtonMemberDesc08" );

	CKTDGUIButton *pButton1 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc01" ) );
	CKTDGUIButton *pButton2 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc02" ) );
	CKTDGUIButton *pButton3 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc03" ) );
	CKTDGUIButton *pButton4 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc04" ) );
	CKTDGUIButton *pButton5 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc05" ) );
	CKTDGUIButton *pButton6 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc06" ) );
	CKTDGUIButton *pButton7 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc07" ) );
	CKTDGUIButton *pButton8 = static_cast<CKTDGUIButton *>( m_pGuildInfoControl->GetControl( L"ButtonMemberDesc08" ) );
	
	if( pButton1 == NULL &&
		pButton2 == NULL &&
		pButton3 == NULL &&
		pButton4 == NULL &&
		pButton5 == NULL &&
		pButton6 == NULL &&
		pButton7 == NULL &&
		pButton8 == NULL )
	{
		//	������ �����ش�
		pButton1->SetGuideDesc( L"" );
		pButton2->SetGuideDesc( L"" );
		pButton3->SetGuideDesc( L"" );
		pButton4->SetGuideDesc( L"" );
		pButton5->SetGuideDesc( L"" );
		pButton6->SetGuideDesc( L"" );
		pButton7->SetGuideDesc( L"" );
		pButton8->SetGuideDesc( L"" );
	}
}
#ifdef GUILD_SKILL
//{{ oasis907 : ����� //// 2009-11-09 //// 
void CX2UIGuild::CreateUIGuildSkillTree()

{
	SAFE_DELETE(m_pGuildSkillTree);
	m_pGuildSkillTree = new CX2UIGuildSkillTree(g_pMain->GetNowState() );
	//g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU::UI_MENU_SKILL, false);
	m_pGuildSkillTree->InitSkillTreeUI();
}
//}} oasis907 : ����� //// 2009-11-09 //// 
#endif GUILD_SKILL




void		CX2UIGuild::UpdateGuildMemberInfo( const KEGS_GET_GUILD_USER_LIST_ACK &kEvent )
{
	if( m_pGuildMemberInfoControl == NULL || kEvent.m_cGuildUserListType != CX2Community::GVI_GUILD_MEMBER_INFO )
	{
		ASSERT( !"Wrong Path!" );
		return;
	}

	m_cMaxMemberInfoPage = static_cast< BYTE >( kEvent.m_uiTotalPage );
	m_cNowMemberInfoPage = static_cast< BYTE >( kEvent.m_uiViewPage );

	//	������ ����
	CKTDGUIStatic* pStatic_PageNumber	= static_cast<CKTDGUIStatic*>( m_pGuildMemberInfoControl->GetControl( L"page_number" ) );

	if( pStatic_PageNumber == NULL )
	{
		return;
	}

	WCHAR wszPageNum[10] = {0,};

	//wsprintfW( wszPageNum, L"%d/%d", kEvent.m_uiViewPage, kEvent.m_uiTotalPage );
	StringCchPrintf( wszPageNum, 10, L"%d/%d", kEvent.m_uiViewPage, kEvent.m_uiTotalPage );

    pStatic_PageNumber->SetString(0, wszPageNum);

	std::vector< CX2GuildManager::GuildMemberInfo > vecMemberInfo;

	for( int i = 0; i < _CONST_UI_GUILD_::g_ShownPlayersPerPage; i++ )
	{
		WCHAR wszNameControl[20] = {0,};
		WCHAR wszMessageControl[20] = {0,};
		//wsprintfW( wszNameControl, L"Nick%02d", i+1 );
		StringCchPrintf( wszNameControl, 20, L"Nick%02d", i+1 );

		//wsprintfW( wszMessageControl, L"Introduce%02d", i+1 );
		StringCchPrintf( wszMessageControl, 20, L"Introduce%02d", i+1 );

		//	��Ʈ�� �ε�
		CKTDGUIStatic* pStatic_NickName	= static_cast<CKTDGUIStatic*>( m_pGuildMemberInfoControl->GetControl( wszNameControl ) );
		CKTDGUIStatic* pStatic_Message	= static_cast<CKTDGUIStatic*>( m_pGuildMemberInfoControl->GetControl( wszMessageControl ) );

		//	���� �˻�
		if( pStatic_NickName == NULL || pStatic_Message == NULL )
		{
			break;
		}

		//	�̸�, �λ縻 ���
		if( i < static_cast<int>(kEvent.m_vecMemberMessageList.size() ) )
		{
			pStatic_NickName->SetString(0, kEvent.m_vecMemberMessageList[i].m_wstrNickName.c_str());

			//	���� ����(115)���� ��� ���ڿ��� �����ְ� ...�� ���δ�
			wstring wstrCutString = CutString( pStatic_NickName, 115, kEvent.m_vecMemberMessageList[i].m_wstrMessage.c_str(), L"..." );
			pStatic_Message->SetString(0, wstrCutString.c_str() );

			//	���� ���
			WCHAR wszMessageDesc[200] = {0,};
			//wsprintf( wszMessageDesc, L"ButtonMemberDesc%02d", i+1 );
			StringCchPrintf( wszMessageDesc, 200, L"ButtonMemberDesc%02d", i+1 );
			CKTDGUIButton* pButton_Desc = static_cast<CKTDGUIButton*>( m_pGuildMemberInfoControl->GetControl( wszMessageDesc ) );
			if( pButton_Desc == NULL )
			{
				break;
			}
			pButton_Desc->SetGuideDesc( kEvent.m_vecMemberMessageList[i].m_wstrMessage.c_str() );
		}
		else
		{
			pStatic_NickName->SetString(0, L"");
			pStatic_Message->SetString(0, L"");
		}


		
		
		//	�λ縻 ���� ��Ʈ��
		WCHAR wszDeleteControl[50] = {0,};
		//wsprintfW( wszDeleteControl, L"BTN_DeleteIntroduce%02d", i+1 );
		StringCchPrintf( wszDeleteControl, 50, L"BTN_DeleteIntroduce%02d", i+1 );

		int		iMyGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
		UidType iMyUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

		//	��帻 ���� ��ư ���̱�, ������

		if( i < static_cast<int>(kEvent.m_vecMemberMessageList.size()) )
		{
			if( kEvent.m_vecMemberMessageList[i].m_wstrMessage == L"" )
			{
				m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, false, false );
			}
			else
			{
				switch( iMyGrade )
				{
				case CX2GuildManager::GUG_MASTER:
					{
						m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, true, true );
					}
					break;
				case CX2GuildManager::GUG_SYSOP:
					{
						if( iMyGrade < kEvent.m_vecMemberMessageList[i].m_ucMemberShipGrade )
						{
							m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, true, true );
						}
						else if( iMyUID == kEvent.m_vecMemberMessageList[i].m_iUnitUID )
						{
							m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, true, true );
						}
						else
						{
							m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, false, false );
						}
					}
					break;
				default:
					{
						if( iMyUID == kEvent.m_vecMemberMessageList[i].m_iUnitUID )
						{
							m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, true, true );
						}
						else
						{
							m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, false, false );
						}
					}
					break;
				}
			}
		}
		else
		{
			m_pGuildMemberInfoControl->SetShowEnable( wszDeleteControl, false, false );
		}

		if( i < static_cast<int>(kEvent.m_vecMemberMessageList.size()) )
		{
			CX2GuildManager::GuildMemberInfo tInfo;

			tInfo.m_UID = kEvent.m_vecMemberMessageList[i].m_iUnitUID;
			tInfo.ucGrade = kEvent.m_vecMemberMessageList[i].m_ucMemberShipGrade;	//	��Ŷ�� ���� ��� ���� �޾ƿ;���(���翡�� ���ϰ� �߰��ϱ�)
			tInfo.m_cUnitClass = kEvent.m_vecMemberMessageList[i].m_cUnitClass;
			tInfo.m_ucLevel = kEvent.m_vecMemberMessageList[i].m_ucLevel;
			//	������� ���� ���ǹ� �߰�(Xǥ�� ������ �Ⱥ�����)

			vecMemberInfo.push_back( tInfo );
		}
	}

	g_pData->GetGuildManager()->SetVecGuildMemberInfo( vecMemberInfo );

	for(int i = 0; i < 8; i++)
	{
		if( i < static_cast<int>(kEvent.m_vecMemberMessageList.size()) )
		{
			SetFieldUserIcon(i, true);
		}
		else
		{
			SetFieldUserIcon(i, false);
		}
	}
}

bool		CX2UIGuild::SetShowChangeGuildMemberMessageDlg( bool bEnable )
{
	if(bEnable == true)
	{
		//{{ ����� : [2009/10/22] //	60byte�� ����
		m_pDlgChangeGuildMemberMessage = g_pMain->KTDGUIOkAndCancelEditBox( D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_4591 ), CX2GuildManager::GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_OK, g_pMain->GetNowState(), 60, true, CX2GuildManager::GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_CANCEL );
		//{{ ����� : [2009/10/22] //	�̵� ����
		m_pDlgChangeGuildMemberMessage->SetEnableMoveByDrag_LUA( true );

		return true;
	}
	else
	{
		if(m_pDlgChangeGuildMemberMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgChangeGuildMemberMessage, NULL, false );
			m_pDlgChangeGuildMemberMessage = NULL;
		}

		return true;
	}

	return false;
}

bool		CX2UIGuild::SetFieldUserIcon(int i, bool bShow)
{
	if(i < 0 || i >= 8)
		return false;

	if(bShow == true)
	{
		if(m_pGuildMemberIcon[i] == NULL)
		{			
			m_pGuildMemberIcon[i] = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Character_Icon.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pGuildMemberIcon[i] );	
		}
		m_pGuildMemberIcon[i]->SetLayer( XDL_GAME_EDIT );
		m_pGuildMemberIcon[i]->SetShowEnable(true, true);
	}
	else
	{
		if(m_pGuildMemberIcon[i] != NULL)
		{
			m_pGuildMemberIcon[i]->SetShowEnable(false, false);
		}
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pGuildMemberIcon[i], true );

	std::vector< CX2GuildManager::GuildMemberInfo > vecGuildMemberIcon =	g_pData->GetGuildManager()->GetVecGuildMemberInfo();

	if(m_pGuildMemberIcon[i] == NULL || i >= (int)vecGuildMemberIcon.size())
		return false;

	CKTDGUIStatic* pStaticClass = (CKTDGUIStatic*) m_pGuildMemberIcon[i]->GetControl( L"user" );	// ����������
	CKTDGUIStatic* pStaticLv10	= (CKTDGUIStatic*) m_pGuildMemberIcon[i]->GetControl( L"ten" );	// ���� 10�ڸ�
	CKTDGUIStatic* pStaticLv1	= (CKTDGUIStatic*) m_pGuildMemberIcon[i]->GetControl( L"one" );	// ���� 1�ڸ�

	int unitClass = 0;
	int unitLv = 0;
	int unitLv10, unitLv1;

	unitClass = vecGuildMemberIcon[i].m_cUnitClass;
	unitLv = vecGuildMemberIcon[i].m_ucLevel;
	unitLv10 = unitLv / 10;
	unitLv1 = unitLv % 10;

	ClearUserIcon(i);


	if( NULL != pStaticClass && 
		NULL != pStaticClass->GetPicture(0) )
	{
		pStaticClass->SetShowEnable( true, true );
		CKTDGUIControl::CPictureData* pPicture = pStaticClass->GetPicture(0);

		wstring fileName;
		wstring pieceName;
		if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS)unitClass, CX2Data::CIT_20by20 ) )
		{
			pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
		}
		else
		{
			ASSERT( !"NO" );
		}
	}



	pStaticLv10->GetPicture(unitLv10)->SetShow(true);
	pStaticLv1->GetPicture(unitLv1)->SetShow(true);

	if(m_pGuildInfoControl!= NULL)
	{
		D3DXVECTOR3 tempPos1 = m_pDlgGuildInfo->GetDummyPos(i);
		D3DXVECTOR3 tempPos2 = m_pDlgGuildInfo->GetPos();
		m_pGuildMemberIcon[i]->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);
	}	

	return true;
}

void CX2UIGuild::ClearUserIcon(int i)
{	
	if(m_pGuildMemberIcon[i] == NULL)
		return;

	CKTDGUIStatic* pStaticClass = (CKTDGUIStatic*) m_pGuildMemberIcon[i]->GetControl( L"user" );	// ����������
	CKTDGUIStatic* pStaticLv10	= (CKTDGUIStatic*) m_pGuildMemberIcon[i]->GetControl( L"ten" );	// ���� 10�ڸ�
	CKTDGUIStatic* pStaticLv1	= (CKTDGUIStatic*) m_pGuildMemberIcon[i]->GetControl( L"one" );	// ���� 1�ڸ�

	if(pStaticClass == NULL || pStaticLv10 == NULL || pStaticLv1 == NULL)
		return;

	pStaticClass->SetShowEnable( false, false );


	for(int i=0; i<10; ++i)
	{
		pStaticLv10->GetPicture(i)->SetShow(false);
		pStaticLv1->GetPicture(i)->SetShow(false);
	}
}

//	wszString�� ���ڿ��� ���̸� ���ϰ�(pSeedStatic ��Ʈ�� ����) wszReplaceString�� ���� ��� �ڿ� �ٿ��ش�. ��) L"..."
wstring	CX2UIGuild::CutString( CKTDGUIStatic* pSeedStatic, int iWidth, const WCHAR* wszString, const WCHAR* wszReplaceString )
{
	if( pSeedStatic == NULL || iWidth < 1 )
	{
		return L"";
	}

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pSeedStatic->GetString(0)->fontIndex );
	int iSize = pFont->GetWidth( wszString );
    
	// g_pKTDXApp->GetResolutionScaleX() �߰�. �ػ󵵿� ���� ũ�⸦ �ٸ��� �ν��ϵ���
	float fRatio = static_cast<float>(iWidth) * g_pKTDXApp->GetResolutionScaleX() / static_cast<float>(iSize);

	if( fRatio >= 1.0f )
	{
		return wszString;
	}
	
	wstring wstrResult = wszString;
	wstrResult = wstrResult.substr(0, static_cast<int>( wstrResult.size() * fRatio ) ) + wszReplaceString;

	return wstrResult;
}

bool	CX2UIGuild::UpdateGuildLevelBar( int iCurrent, int iMax )
{
	if( m_pGuildInfoControl == NULL )
		return false;

	if( iCurrent < 0 || iMax < 0 )
	{
		return false;
	}

	//	�ۼ�Ʈ ����
	int iPercent;
	
	if( iMax == 0 || iMax < iCurrent )	//	iCurrent�� iMax���� ũ�� ������ 100%
	{
		iPercent = 100;
	}
	else
	{
		iPercent = iCurrent * 100 / iMax;
		
		if( iPercent > 100 )
			iPercent = 100;
	}

	CKTDGUIStatic* pStatic_Bar	= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"g_pStaticGuild_EXP" ) );	//	����ġ��(0: ����, 1: ������Ʈ)

	if( pStatic_Bar == NULL )
	{
		return false;
	}

	//{{ ����� : [2009/10/29] //	����ġ��
	CKTDGUIControl::CPictureData* pPicLevel	= pStatic_Bar->GetPicture( 0 );	// ��� ����ġ

	pPicLevel->SetSizeX( _CONST_UI_GUILD_::g_GuildLevelBarSizeX * iPercent / 100.0f );

	//	���� ���
	CX2GuildManager::GuildInfo *pGuildInfo = g_pData->GetGuildManager()->GetMyGuildInfo();
	if( pGuildInfo != NULL )
	{
		CKTDGUIButton* pButton_LevelDesc = static_cast<CKTDGUIButton*>( m_pGuildMemberInfoControl->GetControl( L"ButtonLevelDesc" ) );

		if( pButton_LevelDesc == NULL )
		{
			return false;
		}

		//{{ ����� : [2009/11/4] // ����ġ 100%(����)�̸� ������ ǥ������ �ʴ´�
		if( iPercent == 100 )
		{
			pButton_LevelDesc->SetGuideDesc( L"" );
		}
		else
		{
			wstringstream wstrLevel;
			wstrLevel << iCurrent << L" / " << iMax;
			pButton_LevelDesc->SetGuideDesc( wstrLevel.str().c_str() );
		}
	}

	//	��� ����ġ
	CKTDGUIStatic* pStatic_GuildEXP			= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"EXP" ) );					//	��� ����ġ

	if ( pStatic_GuildEXP == NULL )
		return false;

	wstringstream wstrExp;
	wstrExp << iPercent << L"%";
	pStatic_GuildEXP->SetString( 0, wstrExp.str().c_str() );


	return true;
}

bool	CX2UIGuild::UpdateGuildLevelStr()
{
	//	��� ����
	CKTDGUIStatic* pStatic_GuildLevel		= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"LV" ) );					//	��� ����

	if ( pStatic_GuildLevel == NULL )
		return false;

	wstringstream wstrLevel;
	wstrLevel << GET_STRING( STR_ID_2592 );
	wstrLevel << g_pData->GetGuildManager()->GetMyGuildInfo()->m_byGuildLevel;

	pStatic_GuildLevel->SetString( 0, wstrLevel.str().c_str() );

	return true;
}

bool	CX2UIGuild::UpdateGuildHonorBar( int iCurrent, int iMax )
{
	if( m_pGuildInfoControl == NULL )
		return false;

	if( iCurrent < 0 || iMax < 0 )
	{
		return false;
	}

	//	�ۼ�Ʈ ����
	int iPercent;

	if( iMax == 0 || iMax < iCurrent )	//	iCurrent�� iMax���� ũ�� ������ 100%
	{
		iPercent = 100;
	}
	else
	{
		iPercent = iCurrent * 100 / iMax;

		if( iPercent > 100 )
			iPercent = 100;
	}

	CKTDGUIStatic* pStatic_Bar	= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"g_pStaticGuild_EXP" ) );	//	����ġ��(0: ����, 1: ������Ʈ)

	if( pStatic_Bar == NULL )
	{
		return false;
	}

	//{{ ����� : [2009/10/29] //	�� ����Ʈ ��
	CKTDGUIControl::CPictureData* pPicHonor	= pStatic_Bar->GetPicture( 1 );

	pPicHonor->SetSizeX( _CONST_UI_GUILD_::g_GuildHornorBarSizeX * iPercent / 100.0f );

	//	�� ����ġ
	CKTDGUIStatic* pStatic_GuildEXP			= static_cast<CKTDGUIStatic*>( m_pGuildInfoControl->GetControl( L"EXP2" ) );					//	��� ����ġ

	if ( pStatic_GuildEXP == NULL )
		return false;

	wstringstream wstrExp;
	wstrExp << iPercent << L"%";
	pStatic_GuildEXP->SetString( 0, wstrExp.str().c_str() );

	// ��� ������ ������ ��ġ �߰�
	if( CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
	{
		CKTDGUIButton* pButton_HonorDesc = static_cast<CKTDGUIButton*>( m_pGuildMemberInfoControl->GetControl( L"ButtonHonorDesc" ) );

		if( pButton_HonorDesc == NULL )
		{
			return false;
		}

		wstringstream wstrHonor;
		wstrHonor << GET_STRING( STR_ID_4732 );
		wstrHonor << "\nHonor Point: " << iCurrent;

		pButton_HonorDesc->SetGuideDesc( wstrHonor.str().c_str() );
	}

	return true;
}
#ifdef GUILD_SKILL
//{{ oasis907 : ����� //// 2009-11-09 //// 
bool CX2UIGuild::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	if(m_pGuildSkillTree != NULL)
	{
		if ( m_pGuildSkillTree->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}
	return false;
}
//}} oasis907 : ����� //// 2009-11-09 //// 
#endif GUILD_SKILL

#endif	//	GUILD_MANAGEMENT
//}} ����� : [2009/9/8] //	��� UI�Ŵ��� Ŭ����
