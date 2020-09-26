#include "StdAfx.h"

#ifdef	GUILD_BOARD

#include ".\X2UIGuildBoard.h"
#include < iomanip >

CX2UIGuildBoard::CX2UIGuildBoard( const CKTDXStage* const pNowStage, const WCHAR* const pFileName ) :
m_bShow( false ),
m_bOldApplyDelete( false ),
m_bRegMyGuildAd( false ),
m_pNowStage( pNowStage ),
m_pDlgGuildBoard( NULL ),
m_nowSortType( ST_REG_DATE ),
m_uiNowPage( 1 ),
m_uiMaxPage( 1 ),
m_uiNowPageJoinList( 1 ),
m_uiMaxPageJoinList( 1 ),
m_uiListIndexForJoin( 0 ),
m_uiListIndexForSkill( 0 ),
m_uiJoinListIndex( 0 ),
m_vPosDlg( 0, 0 ),
m_vSizeDlg( 0, 0 ),
m_pGuildSkillTreeInfo( NULL ),
m_sAdvertisePeriod(1)	// �̰� �ʱⰪ 1�� �ؾ��ϴ°� �´°��� �ƴѰ���....
{
	m_pRadioGuildNumMember = NULL;
	m_pRadioGuildLevel = NULL;
	m_pRadioGuildNew = NULL;
	m_pRadioGuildHot = NULL;
	m_pRadioRegDate = NULL;
	m_pButtonAdvertise = NULL;
	m_pButtonJoinList = NULL;

	m_pButtonAccept = NULL;
	m_pButtonRemove = NULL;

	m_pDlgGuildBoard = new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), pFileName );

	if ( m_pDlgGuildBoard == NULL )
	{
		ASSERT( false );
		return;
	}
 
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgGuildBoard );
	m_pDlgGuildBoard->SetDisableUnderWindow( true );

	m_vPosDlg.x			= m_pDlgGuildBoard->GetDummyPos( 0 ).x;		// 0�� posion
	m_vPosDlg.y			= m_pDlgGuildBoard->GetDummyPos( 0 ).y;
	
	m_vSizeDlg.x		= m_pDlgGuildBoard->GetDummyPos( 1 ).x;		// 1�� size
	m_vSizeDlg.y		= m_pDlgGuildBoard->GetDummyPos( 1 ).y;

	// ��� Ÿ�������� �� ���̴� ��Ʈ��
	CKTDGUIStatic* pStaticCommonInfo	= NULL;
	pStaticCommonInfo				= static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( L"g_pCommonStatic" ) );

	if ( pStaticCommonInfo != NULL )
	{
		pStaticCommonInfo->SetString( 0, GET_STRING(STR_ID_4853) );		// Ÿ��Ʋ
		pStaticCommonInfo->SetString( 1, GET_STRING(STR_ID_4853) );		// Ÿ��Ʋ �׸���
		pStaticCommonInfo->SetString( 2, GET_STRING(STR_ID_4854) );		// ����
		pStaticCommonInfo->SetString( 3, GET_STRING(STR_ID_4856) );		// ����
		pStaticCommonInfo->SetString( 4, GET_STRING(STR_ID_4855) );		// �г���	
		pStaticCommonInfo->SetString( 5, GET_STRING(STR_ID_4857) );		// ���� ���
		pStaticCommonInfo->SetString( 6, GET_STRING(STR_ID_4858) );		// ��û ��� ����
	}


	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage; i++ )
	{
		m_columnForAdvertise[i].m_pStaticButtonJoin =
			static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticButtonJoin" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pStaticButtonJoin->SetString( 0, GET_STRING(STR_ID_4859) );

		m_columnForAdvertise[i].m_pStaticButtonSkill =
			static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticButtonSkill" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pStaticButtonSkill->SetString( 0, GET_STRING(STR_ID_4860) );

		m_columnForAdvertise[i].m_pStaticButtonJoin->SetShowEnable(false, false);
		m_columnForAdvertise[i].m_pStaticButtonSkill->SetShowEnable(false, false);
	}

	
	m_pRadioGuildNumMember = static_cast< CKTDGUIRadioButton* >( m_pDlgGuildBoard->GetControl( L"RadioGuildNumMember" ) );
	m_pRadioGuildLevel = static_cast< CKTDGUIRadioButton* >( m_pDlgGuildBoard->GetControl( L"RadioGuildLevel" ) );

	m_pRadioGuildNew = static_cast< CKTDGUIRadioButton* >( m_pDlgGuildBoard->GetControl( L"RadioGuildNew" ) );
	m_pRadioGuildHot = static_cast< CKTDGUIRadioButton* >( m_pDlgGuildBoard->GetControl( L"RadioGuildHot" ) );
	m_pRadioRegDate = static_cast< CKTDGUIRadioButton* >( m_pDlgGuildBoard->GetControl( L"RadioRegDate" ) );


	m_pButtonAdvertise = static_cast< CKTDGUIButton* >( m_pDlgGuildBoard->GetControl( L"ButtonRegister" ) );
	m_pButtonJoinList = static_cast< CKTDGUIButton* >( m_pDlgGuildBoard->GetControl( L"ButtonJoinList" ) );


	if( m_pGuildSkillTreeInfo == NULL)
	{
		//CreateUIGuildSkillTree();
		SAFE_DELETE(m_pGuildSkillTreeInfo);
		m_pGuildSkillTreeInfo = new CX2UIGuildSkillTreeInfo(g_pMain->GetNowState() );
		//g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU::UI_MENU_SKILL, false);
		m_pGuildSkillTreeInfo->InitSkillTreeUI();

	}

	// �ӽ�
	//CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	m_mapGuildSkillRequired.clear();
	m_iGuildSPoint = 1;
	m_iGuildCSPoint = 0;
	m_iMaxGuildCSPoint = 0;
	m_wstrGuildCSPointEndDate = L"";




	m_pDlgJoinList = new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), L"DLG_Guild_Join_List.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgJoinList );
	m_pDlgJoinList->SetDisableUnderWindow( true );
	m_pDlgJoinList->SetShowEnable(false, false);


	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage; i++ )
	{
		m_columnForJoinList[i].m_pCharacterIcon = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Character_Icon.lua" );
		D3DXVECTOR3 tempPos1 = m_pDlgJoinList->GetDummyPos(i);
		D3DXVECTOR3 tempPos2 = m_pDlgJoinList->GetPos();
		m_columnForJoinList[i].m_pCharacterIcon->SetPos_LUA(tempPos1.x + tempPos2.x, tempPos1.y + tempPos2.y);

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_columnForJoinList[i].m_pCharacterIcon );
		m_columnForJoinList[i].m_pCharacterIcon->SetLayer( XDL_GAME_EDIT );
		m_columnForJoinList[i].m_pCharacterIcon->SetShowEnable(false, false);
	}
}

CX2UIGuildBoard::~CX2UIGuildBoard()
{
	m_pRadioGuildNumMember		= NULL;
	m_pRadioGuildLevel	= NULL;

	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage; i++ )
	{
		m_columnForAdvertise[i].m_pStaticGuildName		= NULL;
		m_columnForAdvertise[i].m_pStaticGuildLevel		= NULL;
		m_columnForAdvertise[i].m_pStaticAdvertiseMessage	= NULL;
		m_columnForAdvertise[i].m_pButtonDescGuild	= NULL;
		m_columnForAdvertise[i].m_pButtonDescMessage	= NULL;

	}
	SAFE_DELETE_DIALOG( m_pDlgGuildBoard );
	SAFE_DELETE( m_pGuildSkillTreeInfo );
}

void CX2UIGuildBoard::InitUIGuildBoard()
{
	m_nowSortType = ST_REG_DATE;
	SetShowControlsByType( ST_REG_DATE, true );
}

void CX2UIGuildBoard::InitColumnAdvertise()
{	
	// column�� ��� ���� �ʱ�ȭ
	
	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage; i++ )
	{


		// ��� �̸�
		m_columnForAdvertise[i].m_pStaticGuildName		= 
			static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticGuildName" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pStaticGuildName->SetShowEnable( false, false );

		// ��� ���� 
		m_columnForAdvertise[i].m_pStaticGuildLevel		= 
			static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticGuildLevel" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pStaticGuildLevel->SetShowEnable( false, false );

		// Ŭ���� ��������
		m_columnForAdvertise[i].m_pStaticAdvertiseMessage	= 
			static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticAdvertiseMessage" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pStaticAdvertiseMessage->SetShowEnable( false, false );

		// ��� ���� ����
		m_columnForAdvertise[i].m_pButtonDescGuild	= 
			static_cast< CKTDGUIButton* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"opacity1_" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pButtonDescGuild->SetShowEnable( false, false );

		// ��� ���� �޽��� ����
		m_columnForAdvertise[i].m_pButtonDescMessage	= 
			static_cast< CKTDGUIButton* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"opacity2_" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pButtonDescMessage->SetShowEnable( false, false );


		m_columnForAdvertise[i].m_pButtonJoin	= 
			static_cast< CKTDGUIButton* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonJoin" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pButtonJoin->SetShowEnable( false, false );


		m_columnForAdvertise[i].m_pButtonSkill	= 
			static_cast< CKTDGUIButton* >( m_pDlgGuildBoard->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"ButtonSkill" ), i + 1 ) ) ) );
		m_columnForAdvertise[i].m_pButtonSkill->SetShowEnable( false, false );

		m_columnForAdvertise[i].m_pStaticButtonJoin->SetShowEnable(false, false);
		m_columnForAdvertise[i].m_pStaticButtonSkill->SetShowEnable(false, false);
	}

}


void CX2UIGuildBoard::InitColumnJoinList()
{	
	// ���� ���� ��ư
	m_pButtonAccept	= 
		static_cast< CKTDGUIButton* >( m_pDlgJoinList->GetControl( L"ButtonAccept" ));
	m_pButtonAccept->SetShowEnable( true, true );

	// �����ϱ� ��ư
	m_pButtonRemove	= 
		static_cast< CKTDGUIButton* >( m_pDlgJoinList->GetControl( L"ButtonRemove" ));
	m_pButtonRemove->SetShowEnable( true, true );


	// column�� ��� ���� �ʱ�ȭ

	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage; i++ )
	{
		// ĳ���� ������
		m_columnForJoinList[i].m_pCharacterIcon->SetShowEnable(false, false);

		// ��û�� ����Ʈ ���� ��ư
		m_columnForJoinList[i].m_pRadioJoinList	= 
			static_cast< CKTDGUIRadioButton* >( m_pDlgJoinList->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"RadioJoinList" ), i + 1 ) ) ) );
		m_columnForJoinList[i].m_pRadioJoinList->SetShowEnable( false, false );

		// ��û�� ����Ʈ ����ƽ (����Ʈ ���� ��Ʈ��)
		m_columnForJoinList[i].m_pStaticJoinList = 
			static_cast< CKTDGUIStatic* >( m_pDlgJoinList->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticJoinList" ), i + 1 ) ) ) );
		m_columnForJoinList[i].m_pStaticJoinList->SetShowEnable( false, false );

		// ��û �޽��� ����
		m_columnForJoinList[i].m_pButtonDescMessage	= 
			static_cast< CKTDGUIButton* >( m_pDlgJoinList->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"opacity" ), i + 1 ) ) ) );
		m_columnForJoinList[i].m_pButtonDescMessage->SetShowEnable( false, false );


	}
}



void		CX2UIGuildBoard::SetShowControlsByType( SORT_TYPE type, bool bShow )
{
    //{{ seojt // 2009-8-21, 12:15
    /** 'm_pRadioGuildNumMember'���� �� 'm_pDlgGuildBoard'�� �̹� invalid�ϴٸ�,
        �����Ѵ�.
        - jintaeks on 2009-08-21, 12:15 */
    if( m_pDlgGuildBoard == NULL )
    {
        return;
    }//if
    //}} seojt // 2009-8-21, 12:15

	switch ( type )
	{
	case ST_REG_DATE:
	case ST_NEW_GUILD:
	case ST_HOT_GUILD:
	case ST_MEMBER_COUNT:
	case ST_GUILD_LEVEL:
		{
			CKTDGUIRadioButton*		pRadioButton = 
				static_cast< CKTDGUIRadioButton* >( m_pDlgGuildBoard->GetControl( L"RadioButton_Henir" ) );
			pRadioButton->SetChecked( bShow );

			if ( m_pRadioGuildNumMember )
				m_pRadioGuildNumMember->SetShowEnable( bShow, bShow );
			
            if ( m_pRadioGuildLevel )
				m_pRadioGuildLevel->SetShowEnable( bShow, bShow );
			
			if ( m_pRadioGuildNew )
				m_pRadioGuildNew->SetShowEnable( bShow, bShow );

			if ( m_pRadioGuildHot )
				m_pRadioGuildHot->SetShowEnable( bShow, bShow );

			//{{ oasis907 : ����� [2010.2.16] // 
			if ( m_pRadioRegDate )
				m_pRadioRegDate->SetShowEnable( bShow, bShow );

			if ( type == ST_REG_DATE )
				m_pRadioRegDate->SetChecked( true );
		}
		break;

	default:	// MAX_NUM
		break;
	}
}


void CX2UIGuildBoard::SetShowColumn( ColumnForAdvertise* const pColumnForAdvertise, SORT_TYPE type, bool bShow )
{
	pColumnForAdvertise->m_pStaticGuildName->SetShowEnable( bShow, bShow );
	pColumnForAdvertise->m_pStaticGuildLevel->SetShowEnable( bShow, bShow );
	pColumnForAdvertise->m_pButtonJoin->SetShowEnable( bShow, bShow );
	pColumnForAdvertise->m_pButtonSkill->SetShowEnable( bShow, bShow );

	pColumnForAdvertise->m_pStaticButtonJoin->SetShowEnable(bShow, bShow);
	pColumnForAdvertise->m_pStaticButtonSkill->SetShowEnable(bShow, bShow);



	if(bShow)
	{
		BYTE byMemberShipGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
		if(byMemberShipGrade != CX2GuildManager::GUG_INVALID || // ������ ����
			true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )		// ü����̵� ����
		{

			pColumnForAdvertise->m_pStaticButtonJoin->GetString(0)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
			pColumnForAdvertise->m_pStaticButtonJoin->GetString(0)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );

			pColumnForAdvertise->m_pButtonJoin->SetShowEnable( true, false );
			pColumnForAdvertise->m_pStaticButtonJoin->SetShowEnable( true, false );
		}
		else
		{
			pColumnForAdvertise->m_pStaticButtonJoin->GetString(0)->color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
			pColumnForAdvertise->m_pStaticButtonJoin->GetString(0)->outlineColor = D3DXCOLOR( 0.5f,0.3f, 0.0f, 1.0f );

			pColumnForAdvertise->m_pButtonJoin->SetShowEnable( true, true );
			pColumnForAdvertise->m_pStaticButtonJoin->SetShowEnable( true, true );
		}
	}
	else
	{
		pColumnForAdvertise->m_pButtonJoin->SetShowEnable( false, false );
	}



	switch ( type )
	{
	case ST_REG_DATE:
	case ST_NEW_GUILD:
	case ST_HOT_GUILD:
	case ST_MEMBER_COUNT:
	case ST_GUILD_LEVEL:
		{
			pColumnForAdvertise->m_pStaticAdvertiseMessage->SetShowEnable( bShow, bShow );
			pColumnForAdvertise->m_pButtonDescGuild->SetShowEnable( bShow, bShow );
			pColumnForAdvertise->m_pButtonDescMessage->SetShowEnable( bShow, bShow );


		}
		break;
	default:	// MAX_NUM	// �̰������� �ƹ� ������ ������� �ʾƾ� �ϴ� column �뵵�� ���
		{
			pColumnForAdvertise->m_pStaticAdvertiseMessage->SetShowEnable( bShow, bShow );
			pColumnForAdvertise->m_pButtonDescGuild->SetShowEnable( bShow, bShow );
			pColumnForAdvertise->m_pButtonDescMessage->SetShowEnable( bShow, bShow );
		}
		break;
	}

	// ���Խ�û, ��ų���� ��ư Ȱ��, ��Ȱ��ȭ



}


void CX2UIGuildBoard::SetShowColumn( ColumnForJoinList* const pColumnForJoinList, bool bShow, UINT uiListIndex )
{
	pColumnForJoinList->m_pCharacterIcon->SetShowEnable( bShow, bShow );



	pColumnForJoinList->m_pRadioJoinList->SetShowEnable( bShow, bShow );
	pColumnForJoinList->m_pStaticJoinList->SetShowEnable( bShow, bShow );

	
	if(bShow)
	{
		if(uiListIndex == m_uiJoinListIndex)
		{
			pColumnForJoinList->m_pButtonDescMessage->SetShowEnable( true, true );
		}
		else
		{
			pColumnForJoinList->m_pButtonDescMessage->SetShowEnable( false, false );
		}
	}
	else
	{
		pColumnForJoinList->m_pButtonDescMessage->SetShowEnable( false, false );
	}

	return;
}




//{{ oasis907 : ����� [2009.12.28] //
void CX2UIGuildBoard::SetRequiredGuildSkill( const std::vector<KGuildSkillData>& vecSkillList )
{
	/*
	// �ӽ�
	m_mapGuildSkillRequired.clear();
	m_mapGuildSkillRequired[ (CX2SkillTree::SKILL_ID)5000 ] = UserSkillData( 1, 0 );
	*/


	m_mapGuildSkillRequired.clear();
	BOOST_TEST_FOREACH( const KGuildSkillData&, userSkillData, vecSkillList )
	{
		SkillDataMap::iterator mit;
		mit = m_mapGuildSkillRequired.find( (CX2SkillTree::SKILL_ID) userSkillData.m_iSkillID );
		ASSERT( mit == m_mapGuildSkillRequired.end() );
		if( mit == m_mapGuildSkillRequired.end() )
		{
			// �̹� �����ϸ� ���Ե��� �ʰ� �������� ������ ���Եȴ�.
			m_mapGuildSkillRequired[ (CX2SkillTree::SKILL_ID)userSkillData.m_iSkillID ] = UserSkillData( (int)userSkillData.m_cSkillLevel, (int)userSkillData.m_cSkillCSPoint );
		}
	}

}
//}} oasis907 : ����� [2009.12.28] //




bool CX2UIGuildBoard::Handler_EGS_GET_GUILD_SKILL_IN_BOARD_REQ()
{
	if(m_uiListIndexForSkill > _CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage - 1 )
		return false;

	KEGS_GET_GUILD_SKILL_IN_BOARD_REQ kPacket;

	kPacket.m_iGuildUID = m_columnForAdvertise[m_uiListIndexForSkill].m_KGuildAdInfo.m_iGuildUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_GUILD_SKILL_IN_BOARD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_GUILD_SKILL_IN_BOARD_ACK); 

	return true;
}

bool CX2UIGuildBoard::Handler_EGS_GET_GUILD_SKILL_IN_BOARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_GUILD_SKILL_IN_BOARD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_GET_GUILD_SKILL_IN_BOARD_ACK );
	{	
		SetRequiredGuildSkill(kEvent.m_kGuildSkillInfo.m_vecGuildSkill);

		m_iGuildSPoint = kEvent.m_kGuildSkillInfo.m_iGuildCSPoint;
		m_iGuildCSPoint = kEvent.m_kGuildSkillInfo.m_iGuildSPoint;
		
		m_iMaxGuildCSPoint = kEvent.m_kGuildSkillInfo.m_iMaxGuildCSPoint;
		m_wstrGuildCSPointEndDate = kEvent.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate;

		g_pData->GetUIManager()->GetUIGuildBoard()->GetUIGuildSkillTreeInfo()->SetShow(true);
	}
	return true;
}

bool CX2UIGuildBoard::Handler_EGS_GET_GUILD_AD_LIST_REQ()
{

	KEGS_GET_GUILD_AD_LIST_REQ kPacket;

	kPacket.m_cSortType	= static_cast< char >( m_nowSortType );
	kPacket.m_uiViewPage	= m_uiNowPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_GUILD_AD_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_GUILD_AD_LIST_ACK); 

	return true;
}
bool CX2UIGuildBoard::Handler_EGS_GET_GUILD_AD_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_GUILD_AD_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_GET_GUILD_AD_LIST_ACK ) == true )
	{

		m_nowSortType	= static_cast< SORT_TYPE >( kEvent.m_cSortType );

		// ��ŷ ������ �켱 �� ������ �ʵ��� ��
		for( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage; i++)
			SetShowColumn( (m_columnForAdvertise + i), ST_MAX_NUM, false );



		m_uiMaxPage				= kEvent.m_uiTotalPage;
		m_uiNowPage				= kEvent.m_uiViewPage;
		
		//////////////////////////////////////////////////////////////////////////
		u_int iSize = kEvent.m_vecGuildAdList.size();
		//////////////////////////////////////////////////////////////////////////

		UpdatePageUI();
		UpdateButtonUI(kEvent.m_bCanShowApplyList);
		m_bRegMyGuildAd = kEvent.m_bRegMyGuildAd;

		switch ( m_nowSortType )
		{
		case ST_REG_DATE:
		case ST_NEW_GUILD:
		case ST_HOT_GUILD:
		case ST_MEMBER_COUNT:
		case ST_GUILD_LEVEL:
			{
				// �ٸ� �������� ��ŷ
				int i = 0;

				//BOOST_TEST_FOREACH( KHenirRankingInfo, kHenirRankingInfo, kEvent.m_vecHenirRankingPage )
				BOOST_TEST_FOREACH( KGuildAdInfo&, kGuildAdInfo, kEvent.m_vecGuildAdList )
				{
#ifdef GUILD_UID_FOR_GM
					if( NULL != g_pData->GetMyUser() && CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
					{
						std::wstring wstrAddGuildUID = kGuildAdInfo.m_wstrGuildName.c_str();
						wstrAddGuildUID += L"\nUID: ";
						wstrAddGuildUID += boost::str( boost::wformat( L"%d" ) % kGuildAdInfo.m_iGuildUID );
						m_columnForAdvertise[i].m_pStaticGuildName->SetString( 0, wstrAddGuildUID.c_str() );	// ��� �̸�
					}
					else
#endif // GUILD_UID_FOR_GM
					m_columnForAdvertise[i].m_pStaticGuildName->SetString( 0, kGuildAdInfo.m_wstrGuildName.c_str() );	// ��� �̸�

					//SetNumImage( m_columnForAdvertise[i].m_pStaticGuildLevel, kGuildAdInfo.m_ucGuildLevel );

					WCHAR buff[256] = {0};
					_itow( kGuildAdInfo.m_ucGuildLevel, buff, 10 );
					wstring wstrNum = buff;
			
					m_columnForAdvertise[i].m_pStaticGuildLevel->SetString( 0, wstrNum.c_str() );

					wstring wstrCutString = CutString( m_columnForAdvertise[i].m_pStaticAdvertiseMessage, 265, kGuildAdInfo.m_wstrAdMessage.c_str(), L"..." );
					m_columnForAdvertise[i].m_pStaticAdvertiseMessage->SetString(0, wstrCutString.c_str() );


					//m_columnForAdvertise[i].m_pStaticAdvertiseMessage->SetString( 0, kGuildAdInfo.m_wstrAdMessage.c_str() );
					m_columnForAdvertise[i].m_pButtonDescGuild->SetGuideDesc( GET_REPLACED_STRING( ( STR_ID_4876, "LLi", kGuildAdInfo.m_wstrGuildName, kGuildAdInfo.m_wstrMasterNickName, kGuildAdInfo.m_usCurNumMember) ) );


					m_columnForAdvertise[i].m_pButtonDescMessage->SetGuideDesc( kGuildAdInfo.m_wstrAdMessage.c_str() );


					m_columnForAdvertise[i].m_KGuildAdInfo = kGuildAdInfo;

					SetShowColumn( (m_columnForAdvertise + i), m_nowSortType, true );
					i++;
				}
			}			
			break;
		}
		return true;

	}
	return false;
	
}
bool CX2UIGuildBoard::Handler_EGS_MODIFY_REG_GUILD_AD_REQ( wstring &wstrMessage ) const
{
	KEGS_MODIFY_REG_GUILD_AD_REQ kPacket;

	kPacket.m_sPeriod	= m_sAdvertisePeriod;
	kPacket.m_wstrAdMessage	= wstrMessage;

	g_pData->GetServerProtocol()->SendPacket( EGS_MODIFY_REG_GUILD_AD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_MODIFY_REG_GUILD_AD_ACK); 

	return true;
}

bool CX2UIGuildBoard::Handler_EGS_MODIFY_REG_GUILD_AD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MODIFY_REG_GUILD_AD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_MODIFY_REG_GUILD_AD_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	

			if(g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{														// ��� ���� ��Ͻ� ED �Ҹ�
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

				if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					g_pData->GetUIManager()->GetUIInventory()->SetEDString( kEvent.m_iED );
				}

			}
			int iUsedED = m_sAdvertisePeriod * 3000;

			// ��� ���� ��� �Ǿ����ϴ�. (XXX ED �Һ�)
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4884, "i", iUsedED)), g_pMain->GetNowState() );
/*
			// ��� ���� ��� �Ǿ����ϴ�. (���� ���� �Ⱓ: X��)
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4885, "i", kEvent.m_sPeriod)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
*/
			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
	}
	return false;
}


bool CX2UIGuildBoard::Handler_EGS_REGISTRATION_GUILD_AD_REQ( wstring &wstrMessage ) const
{
	KEGS_REGISTRATION_GUILD_AD_REQ kPacket;

	kPacket.m_sPeriod	= m_sAdvertisePeriod;
	kPacket.m_wstrAdMessage	= wstrMessage;

	g_pData->GetServerProtocol()->SendPacket( EGS_REGISTRATION_GUILD_AD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_REGISTRATION_GUILD_AD_ACK); 

	return true;
}

bool CX2UIGuildBoard::Handler_EGS_REGISTRATION_GUILD_AD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REGISTRATION_GUILD_AD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_REGISTRATION_GUILD_AD_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{	
			
			if(g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{														// ��� ���� ��Ͻ� ED �Ҹ�
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = kEvent.m_iED;

				if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					g_pData->GetUIManager()->GetUIInventory()->SetEDString( kEvent.m_iED );
				}
			}
			int iUsedED = m_sAdvertisePeriod * 3000;

			// ��� ���� ��� �Ǿ����ϴ�. (XXX ED �Һ�)
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4884, "i", iUsedED)), g_pMain->GetNowState() );

/*
			// ��� ���� ��� �Ǿ����ϴ�. (���� ���� �Ⱓ: X��)
			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4885, "i", kEvent.m_sPeriod)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
*/
			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
	}
	return false;
}

bool CX2UIGuildBoard::Handler_EGS_APPLY_JOIN_GUILD_REQ( wstring &wstrMessage, bool bOldApplyDelete )
{
	KEGS_APPLY_JOIN_GUILD_REQ kPacket;

	kPacket.m_bOldApplyDelete = bOldApplyDelete;
	

	kPacket.m_iGuildUID = m_columnForAdvertise[m_uiListIndexForJoin].m_KGuildAdInfo.m_iGuildUID;
	kPacket.m_wstrMessage = wstrMessage;
	m_wstrLastMessage = wstrMessage;


	g_pData->GetServerProtocol()->SendPacket( EGS_APPLY_JOIN_GUILD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_APPLY_JOIN_GUILD_ACK); 

	return true;
}

bool CX2UIGuildBoard::Handler_EGS_APPLY_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_APPLY_JOIN_GUILD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if ( g_pMain->DeleteServerPacket( EGS_APPLY_JOIN_GUILD_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::ERR_GUILD_BOARD_09)
		{
			// "��� �����ϰ� �;�...." �� �޽����� ��� �Ͻðڽ��ϱ�? ������ �ۼ��ߴ� ���� ��û �޽����� ���� �˴ϴ�."
			// m_wstrLastMessage
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4886, "L", m_wstrLastMessage)), UGBCM_PUSH_JOIN_MESSAGE_OK, g_pMain->GetNowState(), UGBCM_PUSH_JOIN_MESSAGE_CANCEL );
			m_bOldApplyDelete = true;
			return true;
		}	
		else if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4887 ), g_pMain->GetNowState() );
			Handler_EGS_GET_GUILD_AD_LIST_REQ(); // ��û ��� ��ư ������Ʈ
			return true;
		}
	}
	return false;
}

bool CX2UIGuildBoard::Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ()
{

	KEGS_GET_APPLY_JOIN_GUILD_LIST_REQ kPacket;

	kPacket.m_uiViewPage	= m_uiNowPageJoinList;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_APPLY_JOIN_GUILD_LIST_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_APPLY_JOIN_GUILD_LIST_ACK ); 


	return true;
}

bool CX2UIGuildBoard::Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if ( g_pMain->DeleteServerPacket( EGS_GET_APPLY_JOIN_GUILD_LIST_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::ERR_GUILD_BOARD_14)
		{
			m_pDlgJoinList->SetShowEnable(false, false);

			for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage; i++ )
			{
				m_columnForJoinList[i].m_pCharacterIcon->SetShowEnable(false, false);
			}
			Handler_EGS_GET_GUILD_AD_LIST_REQ(); // ��û ��� ��ư ������Ʈ
		}
		else if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			// �켱 �� ������ �ʵ��� ��
			for( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage; i++)
				SetShowColumn( (m_columnForJoinList + i), false, i );


			m_uiMaxPageJoinList				= kEvent.m_uiTotalPage;
			m_uiNowPageJoinList				= kEvent.m_uiViewPage;

			UpdatePageJoinUI();


			CKTDGUIStatic* pStaticCommonInfo = NULL;
			pStaticCommonInfo = static_cast< CKTDGUIStatic* >( m_pDlgJoinList->GetControl( L"g_pStaticTEXT" ) );

			pStaticCommonInfo->SetString( 7, kEvent.m_wstrGuildName.c_str() );
			pStaticCommonInfo->SetString( 8, kEvent.m_wstrGuildName.c_str() );


			{
				// �ٸ� �������� ��ŷ
				int i = 0;

				BOOST_TEST_FOREACH( KApplyJoinGuildInfo&, kApplyJoinGuildInfo, kEvent.m_vecApplyJoinGuildList )
				{

					///////////////////////////////////////////////////////////////

					m_columnForJoinList[i].m_pCharacterIcon->SetShowEnable(true, true);

					// ĳ���� ������ set
					CKTDGUIStatic* pStaticClass = 
						static_cast< CKTDGUIStatic* > ( m_columnForJoinList[i].m_pCharacterIcon->GetControl( L"user" ) );	// ����������
					CKTDGUIStatic* pStaticLv10	= 
						static_cast< CKTDGUIStatic* > ( m_columnForJoinList[i].m_pCharacterIcon->GetControl( L"ten" ) );	// ���� 10�ڸ�
					CKTDGUIStatic* pStaticLv1	= 
						static_cast< CKTDGUIStatic* > ( m_columnForJoinList[i].m_pCharacterIcon->GetControl( L"one" ) );	// ���� 1�ڸ�

					int unitClass = 0;
					int unitLv = 0;
					int unitLv10, unitLv1;

					unitClass = kApplyJoinGuildInfo.m_cUnitClass;
					unitLv = kApplyJoinGuildInfo.m_ucLevel;
					unitLv10 = unitLv / 10;
					unitLv1 = unitLv % 10;

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

					for(int j=0; j<10; ++j)
					{
						pStaticLv10->GetPicture(j)->SetShow(false);
						pStaticLv1->GetPicture(j)->SetShow(false);
					}

					pStaticLv10->GetPicture(unitLv10)->SetShow(true);
					pStaticLv1->GetPicture(unitLv1)->SetShow(true);
					////////////////////////////////////////////////////

					m_columnForJoinList[i].m_pStaticJoinList->SetString( 0, kApplyJoinGuildInfo.m_wstrNickName.c_str() );	// ��� �̸�

					wstring wstrCutString = CutString( m_columnForJoinList[i].m_pStaticJoinList, 205, kApplyJoinGuildInfo.m_wstrMessage.c_str(), L"..." );
					m_columnForJoinList[i].m_pStaticJoinList->SetString(1, wstrCutString.c_str() );

					//m_columnForJoinList[i].m_pStaticJoinList->SetString( 2, L"09-06-21 145:2434:434");
					WCHAR	temp[20] = { 0, };
					if( kApplyJoinGuildInfo.m_wstrRegDate.find( L" " ) >= 0 )
						kApplyJoinGuildInfo.m_wstrRegDate.copy(temp, kApplyJoinGuildInfo.m_wstrRegDate.find( L" " ) );
					
					m_columnForJoinList[i].m_pStaticJoinList->SetString( 2,  temp );

					m_columnForJoinList[i].m_KApplyJoinGuildInfo = kApplyJoinGuildInfo;

					m_columnForJoinList[i].m_pButtonDescMessage->SetGuideDesc(  (kApplyJoinGuildInfo.m_wstrMessage).c_str() );

					SetShowColumn( (m_columnForJoinList + i), true, i );
					i++;
				}
				if( i == 0) // ����Ʈ�� ������� ��
				{
					SetEnableButtonAccept(false);
					SetEnableButtonRemove(false);
				}
				else 
				{
					// ��û���â�� ���ŵɶ�����, ����Ʈ�� ó���� �⺻���� üũ�Ѵ�.
					m_columnForJoinList[0].m_pRadioJoinList->SetChecked(true);
					// ���� ���õ� ����Ʈ �ε���
					m_uiJoinListIndex = 0;
					UpdateJoinListButtonUI( m_uiJoinListIndex );
				}
			}
		}
	}

	return true;
}

bool CX2UIGuildBoard::Handler_EGS_ACCEPT_JOIN_GUILD_REQ()
{
	KEGS_ACCEPT_JOIN_GUILD_REQ kPacket;
	
	kPacket.m_iUnitUID = m_columnForJoinList[m_uiJoinListIndex].m_KApplyJoinGuildInfo.m_iUnitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_ACCEPT_JOIN_GUILD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_ACCEPT_JOIN_GUILD_ACK ); 

	return true;
}
bool CX2UIGuildBoard::Handler_EGS_ACCEPT_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ACCEPT_JOIN_GUILD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if ( g_pMain->DeleteServerPacket( EGS_ACCEPT_JOIN_GUILD_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// oasis907 : ����� [2010.2.2] //  ��� �ʴ�� �����Ϳ��Ե� ���̾�α׷� �˸�
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4909, "L", kEvent.m_wstrReceiverNickName) ), g_pMain->GetNowState() );

			Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
			return true;
		}
		else
		{	// ���� ���нÿ��� ����Ʈ�� ���ŵ� �� �ִ�
			Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
			return true;
		}
	}

	return false;
}

bool CX2UIGuildBoard::Handler_EGS_DELETE_APPLY_JOIN_GUILD_REQ()
{
	KEGS_DELETE_APPLY_JOIN_GUILD_REQ kPacket;

	kPacket.m_iUnitUID = m_columnForJoinList[m_uiJoinListIndex].m_KApplyJoinGuildInfo.m_iUnitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_APPLY_JOIN_GUILD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DELETE_APPLY_JOIN_GUILD_ACK ); 

	return true;
}
bool CX2UIGuildBoard::Handler_EGS_DELETE_APPLY_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DELETE_APPLY_JOIN_GUILD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_DELETE_APPLY_JOIN_GUILD_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
			return true;
		}
	}
	return false;
}



bool CX2UIGuildBoard::SetShowGuildAdvertiseMessageDlg(bool bEnable)
{

	if(bEnable == true)
	{
		m_pDlgGuildAdvertiseMessage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Guild_AD_Registration.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgGuildAdvertiseMessage );
		m_sAdvertisePeriod = 1;		// ���� �޽��� �Ⱓ �⺻��
		return true;
	}
	else
	{
		if(m_pDlgGuildAdvertiseMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgGuildAdvertiseMessage, NULL, false );
			m_pDlgGuildAdvertiseMessage = NULL;
		}
		return true;
	}
	return false;

}


bool CX2UIGuildBoard::SetShowGuildJoinMessageDlg(bool bEnable)
{
	if(bEnable == true)
	{
		m_pDlgGuildJoinMessage = g_pMain->KTDGUIOkAndCancelEditBox(D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_4888), CX2UIGuildBoard::UGBCM_PUSH_JOIN_MESSAGE_OK, g_pMain->GetNowState(), 80, true, CX2UIGuildBoard::UGBCM_PUSH_JOIN_MESSAGE_CANCEL );
		m_pDlgGuildJoinMessage->SetEnableMoveByDrag_LUA( true );

		return true;
	}
	else
	{
		if(m_pDlgGuildJoinMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgGuildJoinMessage, NULL, false );
			m_pDlgGuildJoinMessage = NULL;
		}
		return true;
	}
	return false;
}







void CX2UIGuildBoard::UpdatePageUI() const
{
	if ( m_pDlgGuildBoard != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)( m_pDlgGuildBoard->GetControl( L"Static_PageNumber" ) );

		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_uiNowPage
				<< L"/" << m_uiMaxPage;
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}
	}
}

void CX2UIGuildBoard::UpdatePageJoinUI() const
{
	if ( m_pDlgGuildBoard != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)( m_pDlgJoinList->GetControl( L"Static_PageNumber" ) );

		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_uiNowPageJoinList
				<< L"/" << m_uiMaxPageJoinList;
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}
	}
}

void CX2UIGuildBoard::UpdateButtonUI(bool bCanShowApplyList) // ��層����, ��û��Ϻ��� ��ư.  
{
	if(m_pButtonAdvertise)
	{
		BYTE byMemberShipGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
		if(byMemberShipGrade != CX2GuildManager::GUG_MASTER &&
			byMemberShipGrade != CX2GuildManager::GUG_SYSOP ) 
		{
			SetEnableButtonAdvertise(false);
		}
		else
		{
			SetEnableButtonAdvertise(true);
		}
	}

	if(m_pButtonJoinList)
	{
		// ��û ����� �� �� �ִ� �ڰ��� �Ǵ��� �������� �̸� �˾ƿ;� ��
		if(bCanShowApplyList == true)
		{
			SetEnableButtonJoinList(true);
		}
		else
		{
			SetEnableButtonJoinList(false);
		}
	}
	return;
}



void CX2UIGuildBoard::UpdateJoinListButtonUI(UINT uiJoinListIndex) // ���Խ�û����� ���� ����, �����ϱ� ��ư
{
	if(m_pButtonAccept)
	{
		// ��� ������, �����ڴ� Ȱ��ȭ
		BYTE byMemberShipGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
		if(byMemberShipGrade != CX2GuildManager::GUG_MASTER &&
			byMemberShipGrade != CX2GuildManager::GUG_SYSOP ) 
		{
			SetEnableButtonAccept(false);
		}
		else
		{
			SetEnableButtonAccept(true);
		}
	}

	if(m_pButtonRemove)
	{
		// ��� ������, �����ڴ� Ȱ��ȭ
		BYTE byMemberShipGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
		if(byMemberShipGrade != CX2GuildManager::GUG_MASTER &&
			byMemberShipGrade != CX2GuildManager::GUG_SYSOP ) 
		{
			SetEnableButtonRemove(false);
		}
		else
		{
			SetEnableButtonRemove(true);
		}
		// ��û���� ��� ���� ��ư Ȱ��ȭ
		if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitUID == 
			m_columnForJoinList[uiJoinListIndex].m_KApplyJoinGuildInfo.m_iUnitUID)
		{
			SetEnableButtonRemove(true);
		}
	}
	
	// ���� ��û �޽��� ����, ���� ���õ� ��ϸ� Enable
	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage; i++ )
	{
		m_columnForJoinList[i].m_pButtonDescMessage->SetShowEnable(false, false);
	}
	m_columnForJoinList[m_uiJoinListIndex].m_pButtonDescMessage->SetShowEnable(true, true);
	return;
}

void CX2UIGuildBoard::SetEnableButtonAccept(bool bEnable) // ���� ���� ��ư 
{
	CKTDGUIStatic* pStaticCommonInfo = NULL;
	pStaticCommonInfo = static_cast< CKTDGUIStatic* >( m_pDlgJoinList->GetControl( L"g_pStaticTEXT" ) );

	if(bEnable)
	{
		m_pButtonAccept->SetEnable(true);
		pStaticCommonInfo->GetString(5)->color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pStaticCommonInfo->GetString(5)->outlineColor = D3DXCOLOR( 0.5f,0.3f, 0.0f, 1.0f );
	}
	else
	{
		m_pButtonAccept->SetEnable(false);
		pStaticCommonInfo->GetString(5)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
		pStaticCommonInfo->GetString(5)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );
	}
	return;
}
void CX2UIGuildBoard::SetEnableButtonRemove(bool bEnable) // �����ϱ� ��ư
{
	CKTDGUIStatic* pStaticCommonInfo = NULL;
	pStaticCommonInfo = static_cast< CKTDGUIStatic* >( m_pDlgJoinList->GetControl( L"g_pStaticTEXT" ) );

	if(bEnable)
	{
		m_pButtonRemove->SetEnable(true);
		pStaticCommonInfo->GetString(6)->color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pStaticCommonInfo->GetString(6)->outlineColor = D3DXCOLOR( 0.5f,0.3f, 0.0f, 1.0f );
	}
	else
	{
		m_pButtonRemove->SetEnable(false);
		pStaticCommonInfo->GetString(6)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
		pStaticCommonInfo->GetString(6)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );
	}
	return;

}
void CX2UIGuildBoard::SetEnableButtonAdvertise(bool bEnable) // ���� ��� ��ư
{
	CKTDGUIStatic* pStaticCommonInfo = NULL;
	pStaticCommonInfo = static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( L"g_pCommonStatic" ) );

	if(bEnable)
	{
		m_pButtonAdvertise->SetEnable(true);
		pStaticCommonInfo->GetString(5)->color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pStaticCommonInfo->GetString(5)->outlineColor = D3DXCOLOR( 0.5f,0.3f, 0.0f, 1.0f );
	}
	else
	{
		m_pButtonAdvertise->SetEnable(false);
		pStaticCommonInfo->GetString(5)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
		pStaticCommonInfo->GetString(5)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );
	}
	return;
}
void CX2UIGuildBoard::SetEnableButtonJoinList(bool bEnable) // ��û ��� ��ư
{
	CKTDGUIStatic* pStaticCommonInfo = NULL;
	pStaticCommonInfo = static_cast< CKTDGUIStatic* >( m_pDlgGuildBoard->GetControl( L"g_pCommonStatic" ) );

	if(bEnable)
	{
		m_pButtonJoinList->SetEnable(true);
		pStaticCommonInfo->GetString(6)->color = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );
		pStaticCommonInfo->GetString(6)->outlineColor = D3DXCOLOR( 0.5f,0.3f, 0.0f, 1.0f );
	}
	else
	{
		m_pButtonJoinList->SetEnable(false);
		pStaticCommonInfo->GetString(6)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
		pStaticCommonInfo->GetString(6)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );
	}

	return;
}



std::wstring CX2UIGuildBoard::ConvertTimeToRegisterDateStr( __int64 tRegisterDate )
{
	CTime cTime( tRegisterDate );

	std::wstring wstrDate;

	wstrDate	=	GET_STRING( STR_ID_3735 );
	wstrDate	+=	L"\n";
	wstrDate	+=	static_cast< const wchar_t * >( cTime.Format( _T("%Y-%m-%d") ) );
	wstrDate	+=	L"\n";
	wstrDate	+=	static_cast< const wchar_t * >( cTime.Format( _T("%p %H:%M") ) );
	
	return wstrDate;
}


void CX2UIGuildBoard::SetNumImage( CKTDGUIStatic* const pStaticNumImg, int iRankNum )
{
	int iRank100	= 0;
	int iRank10		= 0;
	int iRank1		= 0;

	iRank100		= iRankNum	/	100;
	iRank10			= ( iRankNum	%	100 ) / 10;
	iRank1			= iRankNum	% 10;

	for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iGuildLevelLimit; i ++ )
		pStaticNumImg->GetPicture( i )->SetShow( false );

	if ( iRank100 > 0 )			// ���ڸ� ���ڸ�
	{
		SetNumToPicture( pStaticNumImg->GetPicture( 0 ), iRank100 );
		SetNumToPicture( pStaticNumImg->GetPicture( 1 ), iRank10 );
		SetNumToPicture( pStaticNumImg->GetPicture( 2 ), iRank1 );

	}
	else if ( iRank10 > 0 )		// ���ڸ� ���ڸ�
	{
		SetNumToPicture( pStaticNumImg->GetPicture( 1 ), iRank10 );
		SetNumToPicture( pStaticNumImg->GetPicture( 2 ), iRank1 );
	}
	else if ( iRank1 > 0 )		// ���ڸ� ���ڸ�
	{
		// ���ڸ� �϶��� ����� ���
		SetNumToPicture( pStaticNumImg->GetPicture( 1 ), iRank1 );
	}
	else
	{
		// ������ 0�� ���� �� �� ����
		ASSERT( false );
	}

}

void CX2UIGuildBoard::SetNumToPicture( CKTDGUIControl::CPictureData* const pPictureData, int num )
{
	std::wstring		wstrTexName;
	
	switch ( num )
	{
	case 0:
		wstrTexName		=	L"PC_0.dds";
		break;

	case 1:
		wstrTexName		=	L"PC_1.dds";
		break;

	case 2:
		wstrTexName		=	L"PC_2.dds";
		break;

	case 3:
		wstrTexName		=	L"PC_3.dds";
		break;

	case 4:
		wstrTexName		=	L"PC_4.dds";
		break;

	case 5:
		wstrTexName		=	L"PC_5.dds";
		break;

	case 6:
		wstrTexName		=	L"PC_6.dds";
		break;

	case 7:
		wstrTexName		=	L"PC_7.dds";
		break;

	case 8:
		wstrTexName		=	L"PC_8.dds";
		break;

	case 9:
		wstrTexName		=	L"PC_9.dds";
		break;

	default:
		ASSERT( false );
		break;
	}

	pPictureData->SetTex( wstrTexName.c_str() );

	pPictureData->SetSizeX( pPictureData->pTexture->pTexture->GetWidth() * 0.8f );
	pPictureData->SetSizeY( pPictureData->pTexture->pTexture->GetHeight() * 0.8f );

	pPictureData->SetShow( true );
}




HRESULT		CX2UIGuildBoard::OnFrameMove( double fTime, float fElapsedTime )
{	
	return S_OK;
}

bool		CX2UIGuildBoard::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pGuildSkillTreeInfo != NULL)
	{
		if(m_pGuildSkillTreeInfo->MsgProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}

	// �տ� ��� ���̾�αװ� ������ �޽����� ó������ �ʴ´�
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgGuildBoard ) )
		return false;

	bool bFlag	= false;

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
			if ( false == m_pDlgGuildBoard->GetIsMouseOver())
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
			if ( false == m_pDlgGuildBoard->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_GUILD_BOARD, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	return bFlag;
}

bool		CX2UIGuildBoard::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	//{{ oasis907 : ����� [2009.12.28] //
	if(m_pGuildSkillTreeInfo != NULL)
	{
		if ( m_pGuildSkillTreeInfo->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
		{
			return true;
		}
	}

	switch ( wParam )
	{
	case UGBCM_EXIT:
		{
			SetShow( false );

			if ( g_pTFieldGame != NULL )
			{
				CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
				if ( pJoinNpc != NULL )
				{
					if ( pJoinNpc->GetNpcShop() != NULL )
						pJoinNpc->GetNpcShop()->SetKeyEvent();
				}
			}

			return true;
		}
		break;

	case UGBCM_JOIN_LIST_EXIT:
		{
			m_pDlgJoinList->SetShowEnable(false, false);

			for ( int i = 0; i < _CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage; i++ )
			{
				m_columnForJoinList[i].m_pCharacterIcon->SetShowEnable(false, false);
			}
			Handler_EGS_GET_GUILD_AD_LIST_REQ(); // ��û ��� ��ư ������Ʈ
			return true;
		}
		break;
	case UGBCM_ADVERTISE_EXIT:
		{
			SetShowGuildAdvertiseMessageDlg(false);
			return true;
		}
		break;
	case UGBCM_SELECT_GUILD_AD_TAP:
		{
			SetShowControlsByType( m_nowSortType, false );
			SetShowControlsByType( ST_REG_DATE, true );
			
			m_nowSortType	=	ST_REG_DATE;
			m_uiNowPage				=	1;

			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
		break;


	case UGBCM_SELECT_GUILD_NUM_MEMBER:
		{
			m_nowSortType	=	ST_MEMBER_COUNT;
			m_uiNowPage				=	1;

			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
		break;

	case UGBCM_SELECT_GUILD_LEVEL:
		{
			m_nowSortType	=	ST_GUILD_LEVEL;
			m_uiNowPage				=	1;

			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
		break;
	case UGBCM_SELECT_GUILD_NEW:
		{
			m_nowSortType	=	ST_NEW_GUILD;
			m_uiNowPage				=	1;

			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
		break;
	case UGBCM_SELECT_GUILD_HOT:
		{
			m_nowSortType	=	ST_HOT_GUILD;
			m_uiNowPage				=	1;

			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
		break;

	case UGBCM_SELECT_REG_DATE:
		{
			m_nowSortType	=	ST_REG_DATE;
			m_uiNowPage				=	1;

			Handler_EGS_GET_GUILD_AD_LIST_REQ();
			return true;
		}
		break;


	case UGBCM_PUSH_PREV_PAGE:
		{
			if ( m_uiNowPage > 1 )
			{
				m_uiNowPage--;
				Handler_EGS_GET_GUILD_AD_LIST_REQ();
			}
			
			return true;
		}
		break;

	case UGBCM_PUSH_NEXT_PAGE:
		{
			if ( m_uiNowPage < m_uiMaxPage )
			{
				m_uiNowPage++;
				Handler_EGS_GET_GUILD_AD_LIST_REQ();
			}
			return true;
		}
		break;

	case UGBCM_PUSH_JOIN:
		{
			BYTE byMemberShipGrade = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;
			if(byMemberShipGrade != CX2GuildManager::GUG_INVALID)
			{
				// �̹� ������ ��尡 �ֽ��ϴ�.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),  GET_STRING( STR_ID_4889), g_pMain->GetNowState() );
				return true;
			}

			bool bRefuseGuildInvitation = g_pMain->GetGameOption().GetOptionList().m_bRefuseGuildInvitation;
			if(bRefuseGuildInvitation == true)
			{
				// ��� �ʴ� �ź��� �����Դϴ�. ��带 ��û�Ͻ÷��� �ɼǿ��� ��� �ʴ� �ź� �޴��� �����ؾ� �մϴ�.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),  GET_STRING( STR_ID_4913), g_pMain->GetNowState() );
				return true;
			}



			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			//int iTargetPage = pButton->GetDummyInt(0);
			//GotoPage(iTargetPage);
			m_uiListIndexForJoin = pButton->GetDummyInt(0);
			SetShowGuildJoinMessageDlg(true);
			
			return true;
		} break;

	case UGBCM_PUSH_JOIN_MESSAGE_OK:
		{
			if(m_bOldApplyDelete == true)
			{
				m_bOldApplyDelete = false;
				CKTDGUIControl* pControl = NULL;
				pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );

				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
				
				Handler_EGS_APPLY_JOIN_GUILD_REQ( m_wstrLastMessage , true);
				return true;
			}
			else
			{
				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDlgGuildJoinMessage->GetControl( L"IMEEditBoxMessage" );
				if(pIMEEditBox != NULL)
				{
					wstring wstrMessage = pIMEEditBox->GetText();

					// �� ĭ���� üũ
					if( wcscmp(pIMEEditBox->GetText(), L"") == 0 )
					{
						// ���� ��û �޽����� �Է��ϼ���.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4888), g_pMain->GetNowState() );
					}
					//	������ üũ
					else if( g_pMain->GetStringFilter()->CheckIsValidString(CX2StringFilter::FT_CHAT, wstrMessage.c_str()))
					{
						m_wstrLastMessage = wstrMessage;
						// "��� �����ϰ� �;�..." �� �޽����� ����Ͻðڽ��ϱ�?
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4890, "L", m_wstrLastMessage)), UGBCM_VERIFY_JOIN_MESSAGE_OK, g_pMain->GetNowState());
						SetShowGuildJoinMessageDlg(false);
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300),  GET_STRING( STR_ID_4891), g_pMain->GetNowState() );
						// ����� ���ԵǾ� �ֽ��ϴ�.

					}
					return true;
				}
			}



		} break;
	case UGBCM_VERIFY_JOIN_MESSAGE_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			Handler_EGS_APPLY_JOIN_GUILD_REQ( m_wstrLastMessage , false);
		} break;	
	case UGBCM_PUSH_JOIN_MESSAGE_CANCEL:
		{
			if(m_bOldApplyDelete == true)
			{
				m_bOldApplyDelete = false;
				CKTDGUIControl* pControl = NULL;
				pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}
			else
			{
				SetShowGuildJoinMessageDlg(false);
			}
		} break;

	case UGBCM_PUSH_SKILL:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			m_uiListIndexForSkill = pButton->GetDummyInt(0);

			Handler_EGS_GET_GUILD_SKILL_IN_BOARD_REQ();
			return true;
		} break;

	case UGBCM_PUSH_ADVERTISE:
		{
			if( m_bRegMyGuildAd == true ) // ��� ���� �̹� ��ϵǾ� ���� ���
			{
				// ��� ���� �̹� ��ϵǾ� �ֽ��ϴ�. ��� ���� �����Ͻðڽ��ϱ�?
				g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4892), UGBCM_MODIFY_ADVERTISE_MESSAGE_OK, g_pMain->GetNowState());
				return true;
			}
			else
			{
				SetShowGuildAdvertiseMessageDlg(true);
			}
			return true;
		} break;

	case UGBCM_SELECT_ADVERTISE_ED:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) m_pDlgGuildAdvertiseMessage->GetControl( L"Guild_AD_Date");
			m_sAdvertisePeriod = pComboBox->GetSelectedItemIndex() + 1;
		} break;
	case UGBCM_PUSH_ADVERTISE_MESSAGE_OK:
		{
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*) m_pDlgGuildAdvertiseMessage->GetControl( L"IME_AdvertiseMessage" );
			if(pIMEEditBox != NULL)
			{
				wstring wstrMessage = pIMEEditBox->GetText();
				// ��ĭ���� Ȯ��
				if( wcscmp(pIMEEditBox->GetText(), L"") == 0 )
				{
					// ���� �޽����� �Է��ϼ���.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4893) , g_pMain->GetNowState() );
				}
				// ������ üũ
				else if( g_pMain->GetStringFilter()->CheckIsValidString(CX2StringFilter::FT_CHAT, wstrMessage.c_str()))
				{
					m_wstrLastADMessage = wstrMessage;
					SetShowGuildAdvertiseMessageDlg(false);
						
					int iUsedED = m_sAdvertisePeriod * 3000;
					if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED < iUsedED )
					{
						// ���� ��� ����� �����ϹǷ� ���� ����� �� �����ϴ�.
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4894), g_pMain->GetNowState() );
					}
					else
					{
						// "XXXXXX..." �� ���� ����Ͻðڽ��ϱ�? XXXXED�� �Һ�˴ϴ�.
						g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_4895, "Li", m_wstrLastADMessage, iUsedED)), UGBCM_VERIFY_ADVERTISE_MESSAGE_OK, g_pMain->GetNowState());
					}
				}
				else
				{
					// ����� ���ԵǾ� �ֽ��ϴ�.
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING(STR_ID_4891), g_pMain->GetNowState() );
				}
				return true;
			}
		} break;
	case UGBCM_VERIFY_ADVERTISE_MESSAGE_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			if( m_bRegMyGuildAd == true )
			{
				Handler_EGS_MODIFY_REG_GUILD_AD_REQ( m_wstrLastADMessage );
			}
			else
			{
				Handler_EGS_REGISTRATION_GUILD_AD_REQ( m_wstrLastADMessage );
			}
		} break;

	case UGBCM_PUSH_ADVERTISE_MESSAGE_CANCEL:
		{
			SetShowGuildAdvertiseMessageDlg(false);
		} break;

	case UGBCM_MODIFY_ADVERTISE_MESSAGE_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			SetShowGuildAdvertiseMessageDlg(true);
		} break;

	case UGBCM_PUSH_JOIN_LIST:
		{
			m_pDlgJoinList->SetShowEnable(true, true);
			InitColumnJoinList();
			Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
			return true;
		} break;

	// ���� ��û ��� â
	case UGBCM_PUSH_PREV_PAGE_JOIN_LIST:
		{
			if ( m_uiNowPageJoinList > 1 )
			{
				m_uiNowPageJoinList--;
				Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
			}
			return true;
		}
		break;

	case UGBCM_PUSH_NEXT_PAGE_JOIN_LIST:
		{
			if ( m_uiNowPageJoinList < m_uiMaxPageJoinList )
			{
				m_uiNowPageJoinList++;
				Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
			}
			return true;
		}
		break;

	case UGBCM_PUSH_ACCEPT:
		{
			Handler_EGS_ACCEPT_JOIN_GUILD_REQ();
			return true;
		} break;

	case UGBCM_PUSH_REMOVE:
		{
			Handler_EGS_DELETE_APPLY_JOIN_GUILD_REQ();
			return true;
		} break;


	case UGBCM_SELECT_JOIN_LIST:
		{
			m_columnForJoinList[m_uiJoinListIndex].m_pButtonDescMessage->SetShowEnable(false, false);

			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*) lParam;
			m_uiJoinListIndex = pRadioButton->GetDummyInt(0);

			if( m_uiJoinListIndex >= 0 )
			{
				m_columnForJoinList[m_uiJoinListIndex].m_pButtonDescMessage->SetShowEnable(true, true);

				// ���� ����, �����ϱ� ��ư Ȱ��, ��Ȱ��ȭ ����
				UpdateJoinListButtonUI( m_uiJoinListIndex );
			}
		}
	default:
		break;
	}

	return false;
}

bool		CX2UIGuildBoard::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_GET_GUILD_AD_LIST_ACK:
		{
			return Handler_EGS_GET_GUILD_AD_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_REGISTRATION_GUILD_AD_ACK:
		{
			return Handler_EGS_REGISTRATION_GUILD_AD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_MODIFY_REG_GUILD_AD_ACK:
		{
			return Handler_EGS_MODIFY_REG_GUILD_AD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_GET_GUILD_SKILL_IN_BOARD_ACK:
		{
			return Handler_EGS_GET_GUILD_SKILL_IN_BOARD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;


	case EGS_GET_APPLY_JOIN_GUILD_LIST_ACK:
		{
			return Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_APPLY_JOIN_GUILD_ACK:
		{
			return Handler_EGS_APPLY_JOIN_GUILD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_ACCEPT_JOIN_GUILD_ACK:
		{
			return Handler_EGS_ACCEPT_JOIN_GUILD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_DELETE_APPLY_JOIN_GUILD_ACK:
		{
			return Handler_EGS_DELETE_APPLY_JOIN_GUILD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	default:
		break;
	}

	return false;
}

bool		CX2UIGuildBoard::GetShow() const
{
	return m_bShow;
}

void		CX2UIGuildBoard::SetShow( bool bShow )
{
	m_bShow = bShow;

	if ( bShow )
	{
		//UpdatePageUI();	
		//UpdateButtonUI();// ��層����, ��û��Ϻ���. 
		InitColumnAdvertise();
		InitUIGuildBoard();
		Handler_EGS_GET_GUILD_AD_LIST_REQ();
		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_GUILD_BOARD );
#ifdef SERV_EPIC_QUEST
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_QUEST_NEW, false);
#else
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_QUEST, false);
#endif SERV_EPIC_QUEST
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_CHARINFO, false);
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL, false);

	}
	else
	{
		g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_MENU_GUILD_BOARD );
#ifdef GUILD_BOARD_FIX01
		if(m_pGuildSkillTreeInfo != NULL)
		{
			m_pGuildSkillTreeInfo->SetShow(false);
		}
		if(m_pDlgGuildAdvertiseMessage != NULL)
		{
			m_pDlgGuildAdvertiseMessage->SetShow(false);
		}
		if(m_pDlgJoinList != NULL)
		{
			m_pDlgJoinList->SetShow(false);
		}
		if(m_pDlgGuildJoinMessage != NULL)
		{
			m_pDlgGuildJoinMessage->SetShow(false);
		}
#endif GUILD_BOARD_FIX01
	}

	m_pDlgGuildBoard->SetShowEnable( m_bShow, m_bShow );
	
}

D3DXVECTOR2	CX2UIGuildBoard::GetDLGSize() const
{
	return m_vSizeDlg;		// �ӽ�
}

void		CX2UIGuildBoard::SetPosition( D3DXVECTOR2 vec )
{
	m_vPosDlg = vec;
	m_pDlgGuildBoard->SetPos( m_vPosDlg );
}

void		CX2UIGuildBoard::SetLayer( X2_DIALOG_LAYER layer )
{
	if( m_pDlgGuildBoard != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgGuildBoard, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgGuildBoard, false );
	}
}


int CX2UIGuildBoard::GetGuildSkillLevel( CX2SkillTree::SKILL_ID eSkillID )
{

	SkillDataMap::iterator mit = m_mapGuildSkillRequired.find( eSkillID );
	if( mit != m_mapGuildSkillRequired.end() )
	{
		const UserSkillData& userSkillData = mit->second;
		return userSkillData.m_iSkillLevel;
	}
	return 0;
}

bool CX2UIGuildBoard::IsMaxGuildSkillLevel( IN CX2SkillTree::SKILL_ID eSkillID )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( NULL == pSkillTemplet )
		return false;

	SkillDataMap::iterator mit;
	mit = m_mapGuildSkillRequired.find( eSkillID );
	if( mit != m_mapGuildSkillRequired.end() )
	{
		const UserSkillData& userSkillData = mit->second;
		if( userSkillData.m_iSkillLevel >= g_pData->GetSkillTree()->GetMaxGuildSkillLevel( (int)0, eSkillID ) )
			return true;
		else 
			return false;
	}

	return false;
}

bool CX2UIGuildBoard::GetGuildSkillLevelAndCSP( IN CX2SkillTree::SKILL_ID eSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint )
{
	SkillDataMap::iterator mit = m_mapGuildSkillRequired.find( eSkillID );
	if( mit != m_mapGuildSkillRequired.end() )
	{
		const UserSkillData& userSkillData = mit->second;
		iSkillLevel		= userSkillData.m_iSkillLevel;
		iSkillCSPoint	= userSkillData.m_iSkillCSPoint;
		return true;
	}
	return false;
}

void CX2UIGuildBoard::CalcUsedGuildSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint )
{
	iSPoint = 0;
	iCSPoint = 0;

	for( SkillDataMap::iterator mit = m_mapGuildSkillRequired.begin(); mit != m_mapGuildSkillRequired.end(); ++mit )
	{
		const UserSkillData& userSkillData = mit->second;

		if( userSkillData.m_iSkillLevel > 0 )
		{
			iSPoint		+= userSkillData.m_iSkillLevel - userSkillData.m_iSkillCSPoint;
			iCSPoint	+= userSkillData.m_iSkillCSPoint;
		}

	}
}


bool CX2UIGuildBoard::CanILearnThisSkill( CX2SkillTree::SKILL_ID eSkillID, bool bCheckSP /*= true*/ )
{

	if(bCheckSP == true)
	{
		if( g_pData->GetUIManager()->GetUIGuildBoard()->m_iGuildSPoint + g_pData->GetUIManager()->GetUIGuildBoard()->m_iGuildCSPoint < 1 )
			return false;
	}

	if( true == IsMaxGuildSkillLevel( eSkillID ) )
		return false;

	int iSkillLevelToLearn = GetGuildSkillLevel( eSkillID ) + 1;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iSkillLevelToLearn );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( NULL == pSkillTemplet )
		return false;


	const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)0, eSkillID );
	if( NULL == pSkillTreeTemplet )
		return false;


	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	int iUsedSPoint = 0;
	int iUsedCSPoint = 0;
	CalcUsedGuildSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );
	if( iUsedSPoint + iUsedCSPoint < pSkillTreeTemplet->m_iTier * MAGIC_SKILL_POINT_PER_TIER )
	{
		return false;
	}


#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	CX2SkillTree::SKILL_ID ePrecedingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill );
	if( false == IsMaxGuildSkillLevel( ePrecedingSkillID ) )
		return false;
#else // UPGRADE_SKILL_SYSTEM_2013
	for( UINT i=0; i<pSkillTreeTemplet->m_vecPrecedingSkill.size(); i++ )
	{
		CX2SkillTree::SKILL_ID ePrecedingSkillID = (CX2SkillTree::SKILL_ID) pSkillTreeTemplet->m_vecPrecedingSkill[i];
		if( false == IsMaxGuildSkillLevel( ePrecedingSkillID ) )
			return false;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

	return true;

}

wstring	CX2UIGuildBoard::CutString( CKTDGUIStatic* pSeedStatic, int iWidth, const WCHAR* wszString, const WCHAR* wszReplaceString )
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


#endif	GUILD_BOARD

