//{{ ����� : [2011/3/22/] //	���� �̼� UI

#include "StdAfx.h"
#include "X2UIWorldMission.h"

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-26

namespace _CONST_UI_WORLD_MISSION_
{
	const float MAX_DEFENCE_VALUE							= 100.f;	/// ����� �� UI �ִ� ���� ��ġ
	const float DEFENCE_DUNGEON_WORLD_MISSION_UI_GAGE_WIDTH	= 74.f;		/// ���� �̼� UI�� ������ ����
}

#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_INSERT_GLOBAL_SERVER
CX2UIWorldMission::CX2UIWorldMission() :
// m_pDlgReward( NULL )
m_pDlgTime( NULL )
//,m_pDlgEXPBonus( NULL )
//,m_bReservedReward( false )
//,m_iRewardItemID( 0 )
//,m_iRewardQuantity( 0 )
, m_iNumOfCrystalsProtected( 0 )
, m_bEnableTimeDlg( false )
, m_bForceUpdate(false)
#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-27
, m_bIsFlickerDanger(false)
#endif // SERV_NEW_DEFENCE_DUNGEON

{
}

CX2UIWorldMission::~CX2UIWorldMission()
{
	//SAFE_DELETE_DIALOG(m_pDlgReward);
	SAFE_DELETE_DIALOG(m_pDlgTime);
}

// bool CX2UIWorldMission::SetShowRewardDlg( bool bEnable )
// {
// 	if(bEnable == true)
// 	{
// 		if( m_iRewardItemID < 0 )
// 			return false;
// 
// 		if( m_iRewardQuantity < 0 )
// 			return false;
// 
// 		m_pDlgReward = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Defence_Dungeon_Reward.lua" );
// 		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgReward );
// 
// 		CKTDGUIStatic* pStaticReward = (CKTDGUIStatic*) m_pDlgReward->GetControl( L"g_pStatic_item_Slot_Image" );
// 
// 		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_iRewardItemID );
// 
// 		if(pStaticReward != NULL && pItemTemplet != NULL)
// 		{
// 			pStaticReward->GetPicture(0)->SetTex( pItemTemplet->m_ShopImage.c_str() );
// 		}
// 
// 		// ��Ʈ��(������ �� ������ ���� �з�)
// 		CKTDGUIStatic* pStaticTitle = (CKTDGUIStatic*)( m_pDlgReward->GetControl( L"Static_Item_Name" ) );
// 
// 		std::wstring wstrContent;
// 		int iStrID = 0;
// 
// 		if( m_iRewardItemID == REWARD_SUCCESS_ITEM_ID )
// 		{
// 			iStrID = STR_ID_11380;
// 		}
// 		else if( m_iRewardItemID == REWARD_FAIL_ITEM_ID )
// 		{
// 			switch( m_iRewardQuantity )
// 			{
// 			case 5:
// 				iStrID = STR_ID_11381;
// 				break;
// 			case 3:
// 				iStrID = STR_ID_11382;
// 				break;
// 			case 2:
// 				iStrID = STR_ID_11383;
// 				break;
// 			}
// 		}
// 
// 		if ( pStaticTitle != NULL )
// 		{
// 			pStaticTitle->SetString( 0, GET_STRING( iStrID ) );
// 		}
// 
// 		return true;
// 	}
// 	else
// 	{
// 		if(m_pDlgReward != NULL)
// 		{
// 			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgReward, NULL, false );
// 			m_pDlgReward = NULL;
// 		}
// 
// 		return true;
// 	}
// 
// 	return false;
// }

void CX2UIWorldMission::SetShowTimeDlg( bool bEnable )
{

	// � Ȱ��ȭ ���°� �ƴѵ� true�� ����õ��ϸ� ó������ �ʱ�
	if( true == bEnable )
	{
		if( NULL != g_pData &&
			NULL != g_pData->GetWorldMissionManager() )
		{
			if( false == g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() )
				return;
		}
	}

	if( false == bEnable )
	{
		if( NULL != m_pDlgTime )
		{
			m_pDlgTime->SetShow(false);
			SetEnableTimeDlg(false);
			return;
		}
	}

	if ( GetEnableTimeDlg() != bEnable )
	{
		if ( true == bEnable )
		{
			if(m_pDlgTime == NULL)
			{
				m_pDlgTime = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Mission_Time.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTime );
				m_pDlgTime->SetLayer(XDL_NORMAL);
			}

			m_pDlgTime->SetShowEnable( bEnable, bEnable );		
			SetEnableTimeDlg( bEnable );
		}
	}
// 	if( true == bEnable &&  )
// 	{
// 		if(m_pDlgTime == NULL)
// 		{
// 			m_pDlgTime = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_World_Mission_Time.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTime );
// 			m_pDlgTime->SetLayer(XDL_NORMAL);
// 		}
// 
// 		m_pDlgTime->SetShowEnable( bEnable, bEnable );		
// 		SetEnableTimeDlg( bEnable );
// 
// 		return true;
// 	}
// 	else if ( false == bEnable && GetEnableTimeDlg() != bEnable )
// 	{
// 		m_pDlgTime->SetShowEnable( bEnable, bEnable );
// 		SetEnableTimeDlg( bEnable );
// 		if(m_pDlgTime != NULL)
// 		{
// 			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgTime, NULL, false );
// 			m_pDlgTime = NULL;
// 
// 			// UI�� ������ ���� ��ȣ�� ���� ���� ���� �ʱ�ȭ
// 			SetNumOfCrystalsProtected( 0 );
// 		}

// 		return true;
// 	}
// 
// 	return false;
}

// bool CX2UIWorldMission::SetShowEXPBonusDlg( bool bEnable )
// {
// 	if(bEnable == true)
// 	{
// 		if(m_pDlgEXPBonus == NULL)
// 		{
// 			m_pDlgEXPBonus = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_EXP_Bonus.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgEXPBonus );
// 
// 			m_pDlgEXPBonus->SetShowEnable( true, true );
// 		}
// 
// 		return true;
// 	}
// 	else
// 	{
// 		if(m_pDlgEXPBonus != NULL)
// 		{
// 			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgEXPBonus, NULL, false );
// 			m_pDlgEXPBonus = NULL;
// 		}
// 
// 		return true;
// 	}
// 
// 	return false;
// }

bool CX2UIWorldMission::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WMUM_REWARD_CLOSE:
		{
			/*SetShowRewardDlg( false );*/
		}
		return true;
	default:
		break;
	}
	
	return false;
}

// void CX2UIWorldMission::SetReservedRewardItem( bool bEnable, int iItemID, int iQuantity )
// {
// 	if( iItemID < 0 )
// 		return;
// 
// 	if( iQuantity < 0 )
// 		return;
// 
// 	m_bReservedReward = bEnable;
// 	m_iRewardItemID = iItemID;
// 	m_iRewardQuantity = iQuantity;
// }

void CX2UIWorldMission::UpdateTimeDlg( int iTime )
{
	if( iTime < 0 )
	{
		iTime = 0;
	}

	if( m_pDlgTime == NULL )
	{
		SetShowTimeDlg( true );
	}

	int iMin = iTime / 60;
	int iSec = iTime % 60;

	const wstring MAGIC_TEXTURE_KEY[] = {	L"BG_NUM_0",
											L"BG_NUM_1",
											L"BG_NUM_2",
											L"BG_NUM_3",
											L"BG_NUM_4",
											L"BG_NUM_5",
											L"BG_NUM_6",
											L"BG_NUM_7",
											L"BG_NUM_8",
											L"BG_NUM_9" };

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-26
	CKTDGUIStatic* pStaticTime = (CKTDGUIStatic*) m_pDlgTime->GetControl( L"TIME" );
#else // SERV_NEW_DEFENCE_DUNGEON
	CKTDGUIStatic* pStaticTime = (CKTDGUIStatic*) m_pDlgTime->GetControl( L"time" );
#endif // SERV_NEW_DEFENCE_DUNGEON

	//	ũ���� ����
	const UINT uiSizeOfTextureKey = ARRAY_SIZE( MAGIC_TEXTURE_KEY );

	int iMinTen			= iMin / 10 % 10;
	if ( uiSizeOfTextureKey <= iMinTen )
		iMinTen = 0;
	int iSecTen			= iSec / 10 % 10;
	if ( uiSizeOfTextureKey <= iSecTen )
		iSecTen = 0;

	if( pStaticTime != NULL )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-03-26

		const int MAGIC_MIN_TEN			= 0;
		const int MAGIC_MIN_ONE			= 1;
		const int MAGIC_SEC_TEN			= 3;
		const int MAGIC_SEC_ONE			= 4;

#else // SERV_NEW_DEFENCE_DUNGEON

		const int MAGIC_MIN_TEN			= 21;
		const int MAGIC_MIN_ONE			= 22;
		const int MAGIC_SEC_TEN			= 24;
		const int MAGIC_SEC_ONE			= 25;

#endif // SERV_NEW_DEFENCE_DUNGEON

		//	�ð�ǥ��
		pStaticTime->GetPicture( MAGIC_MIN_TEN )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iMinTen].c_str() );
		pStaticTime->GetPicture( MAGIC_MIN_ONE )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iMin%10].c_str() );

// 		const int MAGIC_SHIELD	= 14;	//	���о�����
// 		const float MAGIC_POS_X	= 574.0f;
// 		const float MAGIC_POS_Y	= 46.0f;
// 		const int MAGIC_SIZE_X	= 127;

//		if( iProgress > 100 ) iProgress = 100;

		pStaticTime->GetPicture( MAGIC_SEC_TEN )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iSecTen].c_str() );
		pStaticTime->GetPicture( MAGIC_SEC_ONE )->SetTex( L"DLG_UI_Common_Texture65_NEW.tga", MAGIC_TEXTURE_KEY[iSec%10].c_str() );

		//	���� Ŭ���� ���൵ ǥ��
// 		pStaticTime->GetPicture( MAGIC_SHIELD )->SetPos( D3DXVECTOR2( MAGIC_POS_X + (MAGIC_SIZE_X * iProgress / 100), MAGIC_POS_Y ) );
// 
// 		wstringstream wstrstm;
// 		if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
// 		{
// 			CKTDGUIStatic* pStaticTime = (CKTDGUIStatic*) m_pDlgTime->GetControl( L"Static_Dummy" );
// 			wstrstm << L"Mission:" << g_pData->GetWorldMissionManager()->GetDungeonClearCount() << L"/" << g_pData->GetWorldMissionManager()->GetDungeonTargetCount() << L" (" << iProgress << L"%)";
// 
// 			pStaticTime->SetString(0, wstrstm.str().c_str());
// 			pStaticTime->SetShow( true );
// 		}
		
		//}} ����� : [2011/3/23/] //	�ӽ�
	}
}

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-15
void CX2UIWorldMission::UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ /*= 0*/, int iNumOfMaxCrystalsProtected_/*= 1*/ )
#else // SERV_NEW_DEFENCE_DUNGEON
void CX2UIWorldMission::UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ )
#endif // SERV_NEW_DEFENCE_DUNGEON
{
	// ������ UpdateTimeDlg���� NULL �� ��� �ٽ� ������ �����Ƿ�, ����� ��忡���� üũ��
	ASSERT( NULL != m_pDlgTime );

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-26
	/// ����� �� �������� ���Ͽ�, ���� �̼� UI ǥ�� ���� �� �ٲ�

	CKTDGUIStatic* pStaticTime = static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"WORLD_MISSION_NEW" ) );
	if ( NULL == pStaticTime )
	{
		ASSERT( ! L"Static Time Is NULL" );
		return;
	}

	CKTDGUIStatic* pStaticGageAngel		= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"GAGE_ANGEL" ) );
	CKTDGUIStatic* pStaticGageDemon		= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"GAGE_DEMON" ) );
	CKTDGUIStatic* pStaticGageParticle	= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"GAGE_PARTICLE" ) );
	CKTDGUIStatic* pStaticDanger		= static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"DANGER" ) );

	if ( NULL == pStaticGageAngel || NULL == pStaticGageDemon || NULL == pStaticGageParticle || NULL == pStaticDanger )
	{
		ASSERT( ! L"World Mission  Gage UI Is NULL" );
		return;
	}

	CKTDGUIControl::CPictureData* pGageAngel		= pStaticGageAngel->GetPicture(0);
	CKTDGUIControl::CPictureData* pGageDemon		= pStaticGageDemon->GetPicture(0);
	CKTDGUIControl::CPictureData* pGageParticle		= pStaticGageParticle->GetPicture(0);
	CKTDGUIControl::CPictureData* pDanger = pStaticDanger->GetPicture(0);

	if ( NULL == pGageAngel || NULL == pGageDemon || NULL == pGageParticle || NULL == pDanger )
	{
		ASSERT( ! L"World Mission  Gage UI Pictire Is NULL" );
		return;
	}

	/// �������� ����ǹ� ���� �ִ� ��ġ�� 0���� ������ٸ�, �ϴ� 1�� ������ ���� ( 0�̸� ���� ���� ����� 0�� �Ǿ, �������� ��µ��� �ʴ´�. )
	if ( 0 == iNumOfMaxCrystalsProtected_ )
		iNumOfMaxCrystalsProtected_ = 1;

	/// �Ʊ� ������ ����

	/// ����� �� ���� ���� % �� ����  �Ʊ� ������ ���� ����
	const float fGageWidth = _CONST_UI_WORLD_MISSION_::DEFENCE_DUNGEON_WORLD_MISSION_UI_GAGE_WIDTH;

	float fGageAngelWidth = fGageWidth + ( fGageWidth* ( static_cast<float>( iNumOfCrystalsProtected_ ) /  static_cast<float>( iNumOfMaxCrystalsProtected_ ) ) );

	if ( 0.f > fGageAngelWidth )
		fGageAngelWidth = 0.f;
	else if ( fGageWidth * 2 < fGageAngelWidth )
		fGageAngelWidth = fGageWidth * 2;

	pGageAngel->SetSize( D3DXVECTOR2( fGageAngelWidth, 13.f ) );


	/// ���� ������ ����
	D3DXVECTOR2 vGageAngelPos	=  pGageAngel->GetPos();
	float		iGageDemonWidth = ( fGageWidth * 2.f ) - fGageAngelWidth;		/// ���� ������ ���� ����
	pGageDemon->SetPos( D3DXVECTOR2( vGageAngelPos.x + fGageAngelWidth, vGageAngelPos.y ) );
	pGageDemon->SetSize( D3DXVECTOR2( iGageDemonWidth, 13.f ) );

	/// ��� �븳���� ȭ��ǥ �ƹ��� ��ġ ����
	D3DXVECTOR2 vGageParticlePos = pGageDemon->GetPos();
	pGageParticle->SetPos( D3DXVECTOR2( vGageParticlePos.x - 18.f, 02.f ) );

	/// ���� ������ �Ѿ�ٸ�, Danger ��ũ ǥ��
	if ( 93.f > fGageAngelWidth && iNumOfCrystalsProtected_ != iNumOfMaxCrystalsProtected_ )
	{
		if ( false == GetIsFlickerDanger() )	/// Danger ��ũ�� �����Ǿ� ���� ���� ��
		{
			pStaticDanger->SetShow( true );		/// ��Ʈ�� ǥ��
			SetIsFlickerDanger( true );			/// ���� ó��

			pDanger->SetFlicker(0.5f, 1.f, 0.f);		/// Danger ��ũ ������ ����
		}
	}
	/// �Ʊ� ������ �Ѿ�ٸ�, Danger ��ũ ����
	else
	{
		pStaticDanger->SetShow( false );		/// ��Ʈ�� ����
		SetIsFlickerDanger( false );			/// ���� ó��

		pDanger->SetFlicker(99999.f, 0.f, 0.f);		/// Danger ��ũ ������ ����
	}

#else // SERV_NEW_DEFENCE_DUNGEON

	// ������ ũ����Ż�� ������ ���� ������
	if ( m_iNumOfCrystalsProtected != iNumOfCrystalsProtected_ ||
		true == m_bForceUpdate )
	{
		// UIStatic�� ����
		CKTDGUIStatic* pStaticTime = static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"time" ) );
		if ( NULL == pStaticTime )
		{
			ASSERT( ! L"Static Time Is NULL" );
			return;
		}

		// ��� ���� ������ �� ����
		// ������ �ִ� ũ����Ż�� MAGIC_CRYSTAL_PICTURE_START_INDEX + 1���� ���� ���� �Ѵ�.
		const int MAGIC_CRYSTAL_PICTURE_START_INDEX = 5;
		const int MAGIC_CRYSTAL_PICTURE_END_INDEX = 20;

		for ( int i = MAGIC_CRYSTAL_PICTURE_START_INDEX + 1; i <= MAGIC_CRYSTAL_PICTURE_END_INDEX; ++i )
		{
			SetShowCrystalPictureData( pStaticTime, i, false );
		}

		// ������ ���� �ִٸ�
		if ( 0 != iNumOfCrystalsProtected_ )
		{
			// ������ �ִ� ũ����Ż�� MAGIC_CRYSTAL_PICTURE_START_INDEX + 1���� ���� ���� �Ѵ�.
			const int MAGIC_CRYSTAL_PICTURE_START_INDEX = 5;
			const int MAGIC_CRYSTAL_PICTURE_END_INDEX = MAGIC_CRYSTAL_PICTURE_START_INDEX + iNumOfCrystalsProtected_;

			for ( int i = MAGIC_CRYSTAL_PICTURE_START_INDEX + 1; i <= MAGIC_CRYSTAL_PICTURE_END_INDEX; ++i )
			{
				SetShowCrystalPictureData( pStaticTime, i, true );
			}
		}
		m_iNumOfCrystalsProtected = iNumOfCrystalsProtected_ ;
		//SetNumOfCrystalsProtected( iNumOfCrystalsProtected_ );
		/*
		// UI ���� ũ����Ż�� �Ѱ��� 3������� ǥ�õǰ�, ��ü 5���� ����

		// color�� �� ä���� ũ����Ż�� ����
		const int iNumOfCrystalsWithFullColor = iNumOfCrystalsProtected_ / 3;	

		// �κ� Į��� ä�� �� ũ����Ż�� �󸶳� ä���� ������
		const int iNumOfCrystalsWithPartialColor = iNumOfCrystalsProtected_ % 3;

		// UIStatic�� ����
		CKTDGUIStatic* pStaticTime = static_cast<CKTDGUIStatic*>( m_pDlgTime->GetControl( L"time" ) );
		if ( NULL == pStaticTime )
		{
			ASSERT( ! L"Static Time Is NULL" );
			return;
		}

		// iNumOfCrystalsWithFullColor�� ���� �ִ� ���, FullColor�� �׸��� SetShow, true�� ���� ���ְ�
		for ( int i = 1; i <= iNumOfCrystalsWithFullColor; ++i )
		{
			const int iIndexOfFullColor = MAGIC_CRYSTAL_PICTURE_START_INDEX + i * 3;

			SetShowCrystalPictureData( pStaticTime, iIndexOfFullColor, true );
			// 2/3�� �ش��ϴ� �׸��� ���ش�.
			SetShowCrystalPictureData( pStaticTime, iIndexOfFullColor - 2, false );
			// 1/3�� �ش��ϴ� �׸��� ���ش�.
			SetShowCrystalPictureData( pStaticTime, iIndexOfFullColor - 1, false );
		}

		// iNumOfCrystalsWithFullColor���� ���� ������, 1/3, 2/3�� �ش��ϴ� �׸��� true �����ش�.
		if ( 0 != iNumOfCrystalsWithPartialColor )
		{
			const int iIndexOfPartialColor = MAGIC_CRYSTAL_PICTURE_START_INDEX + iNumOfCrystalsWithFullColor * 3 + iNumOfCrystalsWithPartialColor;
			SetShowCrystalPictureData( pStaticTime, iIndexOfPartialColor, true );
		}
		*/

	}

#endif // SERV_NEW_DEFENCE_DUNGEON
}

void CX2UIWorldMission::SetShowCrystalPictureData( CKTDGUIStatic* pStaticTime_, int iIndexOfColor_, bool bShow_ )
{
	CKTDGUIControl::CPictureData* pPictureData = 
		pStaticTime_->GetPicture( iIndexOfColor_ );

	if ( NULL == pPictureData )
	{
		ASSERT( ! L"PictureData Is NULL" );
		return;
	}

	pPictureData->SetShow( bShow_ );
}
#endif	SERV_INSERT_GLOBAL_SERVER
//}} ����� : [2011/3/22/] //	���� �̼� UI

