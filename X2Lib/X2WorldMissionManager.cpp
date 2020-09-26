#include "StdAfx.h"
#include ".\x2WorldMissionManager.h"

#ifdef SERV_INSERT_GLOBAL_SERVER

CX2WorldMissionManager::CX2WorldMissionManager()
: m_bActiveDefenceDungeon( false )
, m_iNumOfWorldCrystalsProtected( 0 )
, m_iRemainTimeToFinish( 0 )
, m_usRemainTimeWorldBuff( 0 )
, m_usWorldBuffStepNum( 0 )
//, m_bIsEXPBonus( false )
, m_pUIWorldMission( NULL )
, m_pTextureDefenceDungeonMiniMap( NULL )
, m_pUVDefenceDungeonMiniMap( NULL )
, m_iStartKeyNumberForCrystal( 5 )
, m_iNumOfCrystals( 0 )
, m_pWorldBuffDragonBreath( NULL )
, m_pWorldBuffElBless( NULL )
, m_iRemainTimeMissionStart( -1 )
#ifdef NEW_DEFENCE_DUNGEON
, m_iNowDefenceDungeonLevel( -1 )	//���� ����� �� ���̵�
#endif NEW_DEFENCE_DUNGEON
#ifdef SERV_2012_PVP_SEASON2_EVENT
, m_bActiveTimeOfGlory( false )		///	������ �ð� Ȱ�� ����
, m_bShowTimeOfGloryDLG( false )	///	������ �ð� ���̾�α� Ȱ�� ����
#endif SERV_2012_PVP_SEASON2_EVENT
// , MAGIC_X_LEFT( 0 )
// , MAGIC_X_RIGHT( 0 )
// , MAGIC_Z_UP( 0 )
// , MAGIC_Z_DOWN( 0 )
// , MAGIC_POS_X( 0 )
// , MAGIC_POS_Y( 0 )
// , MAGIC_WIDTH( 0 )
// , MAGIC_HEIGHT( 0 )
// , MAGIC_EL_WIDTH( 0 )
// , MAGIC_EL_HEIGHT( 0 )
{
	Init();

	m_pUIWorldMission = new CX2UIWorldMission;
}		


CX2WorldMissionManager::~CX2WorldMissionManager()
{
	SAFE_DELETE( m_pUIWorldMission );
}

void CX2WorldMissionManager::Init()
{
	SetIsActiveDefenceDungeon( false );
	SetNumOfWorldCrystalsProtected( 0 );
	SetRemainTimeToFinish( 0 );
//	m_bIsEXPBonus = false;

	for( int i = 0; i < MAX_CRYSTAL_COUNT; i++ )
	{
		m_timerDamaged[i].restart();
	}
}

void CX2WorldMissionManager::RenderMiniMapTexture( CKTDXDeviceRenderTargetTexture* 	pRenderTargetTexture )
{
	if( pRenderTargetTexture == NULL )
		return;

	if( NULL == pRenderTargetTexture )
		return;

	KTDXPROFILE_BEGIN( "CLEAR" );
	pRenderTargetTexture->BeginRender( true );
	pRenderTargetTexture->Clear( D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "END_RENDER" );
	pRenderTargetTexture->EndRender();    
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "MAP_DRAW" );
	
	const int MAGIC_X_LEFT = -5869;
	const int MAGIC_X_RIGHT = 5869;
	const int MAGIC_Z_UP = 1669;
	const int MAGIC_Z_DOWN = -5187;

	const int MAGIC_POS_X = 785;
	const int MAGIC_POS_Y = 1;

	const int MAGIC_WIDTH = 180;
	const int MAGIC_HEIGHT = 65;

#ifdef NEW_DEFENCE_DUNGEON
	//���� �������� ���� �̴ϸ� ������ �̵� ��� ����
	const float	fIncreaseDegree			   = 1.34f;		//���� ��ġ ���� ����
	const float fMiniMapIconDistance	   =  70.f;		//�̴ϸ� �ȿ��� ������ �߽����� ���� ������ ���� �Ÿ�
	const float MiniMapCenterPosition_x    = 877.f;		//�̴ϸ� �ȿ��� ������ �߽��� x ��ǥ
	const float MiniMapCenterPosition_y	   =  13.f;		//�̴ϸ� �ȿ��� ������ �߽��� z ��ǥ
	const float DungeonMapCenterPosition_x =   0.f;		//������ �߽��� x ��ǥ
	const float DungeonMapCenterPosition_z = 800.f;		//������ �߽��� z ��ǥ
#endif NEW_DEFENCE_DUNGEON

	if ( NULL != GetTextureDefenceDungeonMiniMap() && NULL != GetUVDefenceDungeonMiniMap() )
	{
		const D3DXVECTOR2& vMiniMapWindowPos = g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->GetMiniMapWindowPos();
		const D3DXVECTOR2& vWindowTextureSize = g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->GetMiniMapWindowSize();

		GetTextureDefenceDungeonMiniMap()->Draw( static_cast<int>( vMiniMapWindowPos.x ), static_cast<int>( vMiniMapWindowPos.y ), 
			static_cast<int>( vWindowTextureSize.x ), static_cast<int>( vWindowTextureSize.y ), GetUVDefenceDungeonMiniMap() );

		KTDXPROFILE_BEGIN( "DRAW_CRYSTAL" );

		int iEndKeyCode = m_iStartKeyNumberForCrystal + GetNumOfCrystals();

		for( int iKeyCode = m_iStartKeyNumberForCrystal; iKeyCode < iEndKeyCode; ++iKeyCode )
		{
			const CX2GUNPC* pCrystal = g_pX2Game->GetNPCUnitByKeyCode( iKeyCode );

			if ( NULL != pCrystal ) 
			{
				const D3DXVECTOR3& vPos = pCrystal->GetPos();

#ifdef NEW_DEFENCE_DUNGEON
				//������ �߽����� ���� ���� ���� ����
				float fDegree = static_cast<float>( ( atan2( DungeonMapCenterPosition_x - vPos.x, DungeonMapCenterPosition_z - vPos.z ) ) + fIncreaseDegree );

				RenderCrystal( 
								static_cast<int>( MiniMapCenterPosition_x + ( fMiniMapIconDistance * cos( fDegree ) ) ) - 5,
								static_cast<int>( MiniMapCenterPosition_y + ( fMiniMapIconDistance * sin( fDegree ) ) ) + 1, 
								pCrystal, 
								m_timerDamaged[iKeyCode-m_iStartKeyNumberForCrystal] 
								);
#else  NEW_DEFENCE_DUNGEON
				int iPosX = static_cast<int>( MAGIC_POS_X + ( (vPos.x + MAGIC_X_RIGHT) * MAGIC_WIDTH ) / (MAGIC_X_RIGHT - MAGIC_X_LEFT) );
				int iPosY = static_cast<int>( MAGIC_POS_Y + ( (vPos.z + MAGIC_Z_DOWN) * MAGIC_HEIGHT ) / (MAGIC_Z_DOWN - MAGIC_Z_UP) );

				RenderCrystal( iPosX, iPosY, pCrystal, m_timerDamaged[iKeyCode-m_iStartKeyNumberForCrystal] );
#endif NEW_DEFENCE_DUNGEON
			}
		}
		KTDXPROFILE_END();
	}
	
	KTDXPROFILE_BEGIN( "DRAW_USER_UNIT" );

	CKTDXDeviceTexture* pTextureMyUnit  = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_My.dds" );
	CKTDXDeviceTexture* pTexturePartyUnit  = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"MiniMap_Char_Party.dds" );

	int MAGIC_PLAYER_WIDTH = 350;

	// ���� ��ġ �׸���
	CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
	for( int i = 0; i < (int)g_pX2Game->GetUserUnitListSize(); i++ )
	{
		CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnit(i);
		if( NULL != pCX2GUUser &&
			pCX2GUUser->GetNowHp() > 0.f &&
			pCX2GUUser->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			D3DXVECTOR3 vPos = pCX2GUUser->GetPos();

#ifdef NEW_DEFENCE_DUNGEON
			//������ �߽����� �������� ����
			float fDegree = static_cast<float>( ( atan2( DungeonMapCenterPosition_x - vPos.x, DungeonMapCenterPosition_z - vPos.z ) ) + fIncreaseDegree );

			//������ �߽����� ������ �̷�� ������ �״�� �̴ϸ� �����ܿ��� �������, ������ �������� ���� ��Ų��.
			if( pCX2GUUser == pMyUnit )
			{
				pTextureMyUnit->Draw( 
											static_cast<int>( MiniMapCenterPosition_x + ( fMiniMapIconDistance * cos( fDegree ) ) ), 
											static_cast<int>( MiniMapCenterPosition_y + ( fMiniMapIconDistance * sin( fDegree ) ) ), 
											30, 
											30 
									);
			}
			else if( pCX2GUUser->GetTeam() == pMyUnit->GetTeam() )
			{
				pTexturePartyUnit->Draw( 
											static_cast<int>( MiniMapCenterPosition_x + ( fMiniMapIconDistance * cos( fDegree ) ) ), 
											static_cast<int>( MiniMapCenterPosition_y + ( fMiniMapIconDistance * sin( fDegree ) ) ), 
											30, 
											30 
										);
			}
#else  NEW_DEFENCE_DUNGEON
			if( pCX2GUUser == pMyUnit )
			{
				pTextureMyUnit->Draw( static_cast<int>( MAGIC_POS_X + ((vPos.x + MAGIC_PLAYER_WIDTH + MAGIC_X_RIGHT) * MAGIC_WIDTH) / (MAGIC_X_RIGHT - MAGIC_X_LEFT) ), static_cast<int>( MAGIC_POS_Y + ((vPos.z + MAGIC_Z_DOWN) * MAGIC_HEIGHT) / (MAGIC_Z_DOWN - MAGIC_Z_UP) ), 30, 30 );
			}
			else if( pCX2GUUser->GetTeam() == pMyUnit->GetTeam() )
			{
				pTexturePartyUnit->Draw( static_cast<int>( MAGIC_POS_X + ((vPos.x + MAGIC_PLAYER_WIDTH + MAGIC_X_RIGHT) * MAGIC_WIDTH) / (MAGIC_X_RIGHT - MAGIC_X_LEFT) ), static_cast<int>( MAGIC_POS_Y + ((vPos.z + MAGIC_Z_DOWN) * MAGIC_HEIGHT) / (MAGIC_Z_DOWN - MAGIC_Z_UP) ) , 30, 30 );
			}
#endif NEW_DEFENCE_DUNGEON
		}
	}
	KTDXPROFILE_END();

	KTDXPROFILE_END();
}

void CX2WorldMissionManager::RenderCrystal( int iPosX_, int iPosY_, const CX2GUNPC* pCrystal_, CKTDXTimer& tTimer_ )
{
	CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_UI_Common_Texture46.tga" );
	if ( NULL == pTexture )
	{
		ASSERT( NULL != pTexture );
		return;
	}

	int iWidth			= 24;
	int iHeight			= 42;
	int MAGIC_EL_WIDTH	= 7;
	int MAGIC_EL_HEIGHT = -10;

	// ���� ������ ��� 1/2
	if ( CX2UnitManager::NUI_DEFENCE_CRYSTAL_SMALL == pCrystal_->GetNPCTemplet().m_nNPCUnitID )
	{
		iWidth /= 2;
		iHeight /= 2;
		MAGIC_EL_WIDTH = 15;
		MAGIC_EL_HEIGHT = 11;
	}
	
	if( tTimer_.elapsed() > 1.8f )
	{
		CX2GUNPC* pCrystal = const_cast<CX2GUNPC*>( pCrystal_ );
		if( pCrystal->GetDamagedThisFrame() == true )
		{
			tTimer_.restart();
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Dungeon_Hit_Alert.ogg", false, false );
		}
	}
	else
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTextureUVForDamaged = pTexture->GetTexUV( L"ELL_OUT");

		//	�����̴� �������Ѵ�. ���?
		int iTime = static_cast<int>( tTimer_.elapsed() * 10.0 );

		if( iTime % 4 < 2 )
		{
			int iSubtractPosValue = -3;
			int iAddWidthHeightValue = 5;			
            
			// ���� ������ ��� 1/2
			if ( CX2UnitManager::NUI_DEFENCE_CRYSTAL_SMALL == pCrystal_->GetNPCTemplet().m_nNPCUnitID )
			{
				iSubtractPosValue = -2;
				iAddWidthHeightValue = 2;				
			}

			ASSERT( NULL != pTextureUVForDamaged );
			if ( NULL != pTextureUVForDamaged )
				pTexture->Draw( iPosX_ + MAGIC_EL_WIDTH + iSubtractPosValue, iPosY_ + MAGIC_EL_HEIGHT + iSubtractPosValue, iWidth + iAddWidthHeightValue, iHeight + iAddWidthHeightValue, pTextureUVForDamaged );
		}
	}

	//	ü�¿� ���� �ٸ� �� �ؽ��ĸ� �׷�����
	const float fMaxHP = pCrystal_->GetMaxHp();
	const float fNowHP = pCrystal_->GetNowHp();
	const float fPercentHP = fNowHP / fMaxHP * 100;
	
	const CKTDXDeviceTexture::TEXTURE_UV* pTextureUV = NULL;
	if( fPercentHP > 70.0f )
	{
		pTextureUV = pTexture->GetTexUV( L"ELL_GREEN");
	}
	else if ( fPercentHP > 30.0f )
	{
		pTextureUV = pTexture->GetTexUV( L"ELL_YELLOW");
	}
	else if ( fPercentHP > 1.0f )
	{
		pTextureUV = pTexture->GetTexUV( L"ELL_RED");
	}
	else 
	{
		return;
	}

	if ( NULL == pTextureUV )
	{
		ASSERT( NULL != pTextureUV );
		return;
	}

	pTexture->Draw( iPosX_ + MAGIC_EL_WIDTH, iPosY_ + MAGIC_EL_HEIGHT, iWidth, iHeight, pTextureUV );
	
}

bool CX2WorldMissionManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_pUIWorldMission != NULL )
	{
		if( true == m_pUIWorldMission->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		{
			return true;
		}
	}

	return false;
}

bool CX2WorldMissionManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{
	case EGS_WORLD_MISSION_UPDATE_NOT:
		{
			return Handler_EGS_WORLD_MISSION_UPDATE_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_UPDATE_WORLD_BUFF_NOT:
		{
			return Handler_EGS_UPDATE_WORLD_BUFF_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef SERV_2012_PVP_SEASON2_EVENT
	case EGS_PVP_EVENT_INFO_NOT:
		{
			return Handler_EGS_PVP_EVENT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_2012_PVP_SEASON2_EVENT
#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	case EGS_EARTH_QUAKE_NOT:
		{
			return Handler_EGS_EARTH_QUAKE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif TOGETHER_FESTIVAL_2012_AUTUMN
	default:
		break;
	}
	return false;
}

HRESULT CX2WorldMissionManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if( true == IsActiveDefenceDungeon() )
	{
		//int iProgress = 0;
		int iTime =  GetRemainTimeToFinish() - static_cast<int>(m_timerMissionToFinish.elapsed() );

		if( iTime < 0 )
		{
			iTime = 0;
		}


		// �����ϰ�� UI ���� ( �ð� , ���ʽ� ����ġ )
		if( 0 != iTime &&
			(g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP || 
			g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD) )
		{
			if(g_pData != NULL && g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
			{
				// ĳ�ü������� ������ ����
			}
			else
			{
				m_pUIWorldMission->UpdateTimeDlg( iTime );
#ifndef SERV_NEW_DEFENCE_DUNGEON
				m_pUIWorldMission->UpdateNumOfCrystalsProtected( GetNumOfWorldCrystalsProtected() );
#endif // SERV_NEW_DEFENCE_DUNGEON
			}
		}	
		else
		{
			if( m_pUIWorldMission->GetTimeDlg() != NULL )
			{
				m_pUIWorldMission->SetShowTimeDlg( false );
			}
		}
	}
	else
	{
		if( m_pUIWorldMission->GetTimeDlg() != NULL )
		{
			m_pUIWorldMission->SetShowTimeDlg( false );
		}
	}

	return S_OK;
}

bool	CX2WorldMissionManager::Handler_EGS_WORLD_MISSION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_WORLD_MISSION_UPDATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( false == IsActiveDefenceDungeon() && true == kEvent.m_bActive )
	{
		 //////////////////////////////////////////////////////////////////////////
		//	���� �̼� ����

		// ������ ������ �� ��Ŷ�� ������ '����ħ��' �̶�� �̹����� ������
		// �ݴ�� �̹� �̼��� ���� ���� �� �� ��Ŷ�� ������ (ĳ���� ���� ����, ���� ��), �̹����� �������� ����
		if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ||
			g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
			g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
		{
			SetIsActiveDefenceDungeon( true );
			// ĳ�ü������� ������ ����
			if( NULL == g_pData || NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				pX2State->OpenStartMission();
#ifdef FIX_WORLDMISSION_UI_BUG
				m_pUIWorldMission->SetShowTimeDlg( true );
			}
#else
			}
			m_pUIWorldMission->SetShowTimeDlg( true );
#endif //FIX_WORLDMISSION_UI_BUG
		}

#ifdef NEW_DEFENCE_DUNGEON
		//����� �� ����� ���̵� ����
		g_pData->GetWorldMissionManager()->SetNowDefenceDungeonLevel( CX2Dungeon::DL_NORMAL );
		
#ifndef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-15	
	/// ���� ���� ����
	#ifdef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
		ChangeDefenceDungeonLevel();
	#endif FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
#endif // SERV_NEW_DEFENCE_DUNGEON

#endif NEW_DEFENCE_DUNGEON

	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �̼� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( true == IsActiveDefenceDungeon() )
	{
		if ( false == kEvent.m_bActive )
#else // SERV_NEW_DEFENCE_DUNGEON
	if( true == IsActiveDefenceDungeon() && false == kEvent.m_bActive )
	{
#endif // SERV_NEW_DEFENCE_DUNGEON
		{
			//	�����ϰ�� UI ����
			if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ||
				g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
				g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
			{
				SetIsActiveDefenceDungeon( false );
				// ĳ�ü������� ������ ����
				if( NULL == g_pData || NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
				{
					CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
		#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-04-16
					/// ����� �� ���� ���� ����
					pX2State->OpenResultMission( kEvent.m_iWorldCrystalCount >= 0 );
		#else // SERV_NEW_DEFENCE_DUNGEON
					pX2State->OpenResultMission( GetNumOfWorldCrystalsProtected() >= 3 );
		#endif // SERV_NEW_DEFENCE_DUNGEON
				}
			}

			m_pUIWorldMission->SetShowTimeDlg( false );

#ifdef NEW_DEFENCE_DUNGEON
			//����� �� ���̵� �ʱ�ȭ
			g_pData->GetWorldMissionManager()->SetNowDefenceDungeonLevel( -1 );
#endif NEW_DEFENCE_DUNGEON
		}
#ifdef SERV_NEW_DEFENCE_DUNGEON
		else		/// ���� �̼� UI ����
		{
			if ( NULL != m_pUIWorldMission )
				m_pUIWorldMission->UpdateNumOfCrystalsProtected( kEvent.m_iWorldCrystalCount, kEvent.m_iWorldMaxCrystalCount );
		}
#endif // SERV_NEW_DEFENCE_DUNGEON
	}

// 	if( true == kEvent.m_bIsEXPBonus && m_pUIWorldMission->GetEXPBonusDlg() == NULL )
// 	{
// 		if(g_pData != NULL && g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
//		{
			// ĳ�ü������� ������ ����
//		}
// 		else
// 		{
// 			m_pUIWorldMission->SetShowEXPBonusDlg( true );
// 		}
// 	}
// 	else if ( false == kEvent.m_bIsEXPBonus && m_pUIWorldMission->GetEXPBonusDlg() != NULL )
// 	{
// 		m_pUIWorldMission->SetShowEXPBonusDlg( false );
// 	}
	SetRemainTimeMissionStart( kEvent.m_iMissionStartRemainTime );
	m_timerMissionToStart.restart();

	SetRemainTimeToFinish( kEvent.m_iRemainTime );
	m_timerMissionToFinish.restart();

	SetNumOfWorldCrystalsProtected( kEvent.m_iWorldCrystalCount );
	
#ifdef NEW_DEFENCE_DUNGEON	
	//���ѳ� ������ ������ ���� ���̵� ���� �����Ѵ�.
	ChangeDefenceDungeonLevel();

#ifndef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI		/// �ش� ��Ŷ�� ���� �� ���� ��Ƽ UI�� ���ŵ��� �ʵ��� ����
	g_pMain->GetPartyUI()->UpdatePartyDLG();
#endif  FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
	
#endif NEW_DEFENCE_DUNGEON

//	m_bIsEXPBonus = kEvent.m_bIsEXPBonus;

#ifdef SERV_2012_PVP_SEASON2_EVENT
	SetTimeOfGloryDLG();		/// ������ �ð� ���̾�α� Ȱ�� �Լ� -  ���� �̼� ������Ʈ NOT �ö����� �˻� �Ѵ�.
#endif SERV_2012_PVP_SEASON2_EVENT

	return true;
}

/** @function : Handler_EGS_UPDATE_WORLD_BUFF_NOT
	@brief : ���潺���� �������� ������� ��Ŷ�� ���� �ڵ鷯
	@return : bool
*/
bool CX2WorldMissionManager::Handler_EGS_UPDATE_WORLD_BUFF_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_WORLD_BUFF_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	
	// ���� ������ �Ѱ��� ���� �Ǹ�
	if ( 0 < kEvent.m_usSuccessWorldBuff )
	{
		SetWorldBuffStepNum( kEvent.m_usSuccessWorldBuff );

		// ���潺 ���� ���� �� ���� ���� �ð�
		int iRemainTimeByClosingDefenceDungeon = GetRemainTimeToFinish() - static_cast<int>( m_timerMissionToFinish.elapsed() );
		if ( 0 > iRemainTimeByClosingDefenceDungeon )
			iRemainTimeByClosingDefenceDungeon = 0;

		SetRemainTimeWorldBuff( kEvent.m_usWorldBuffDurationTime + iRemainTimeByClosingDefenceDungeon );
		m_timerWorldBuff.restart();

		CreateWorldBuffDragonBreath();
		CreateWorldBuffElBless();
	}
	
	return true;
}
void CX2WorldMissionManager::SetDefenceDungeonMiniMap( const wstring& wstrTextureName, const wstring& wstrKeyName )
{
	SetTextureDefenceDungeonMiniMap( g_pKTDXApp->GetDeviceManager()->OpenTexture( wstrTextureName ) );

	if ( NULL != GetTextureDefenceDungeonMiniMap() )
	{
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = GetTextureDefenceDungeonMiniMap()->GetTexUV( wstrKeyName );
		SetUVDefenceDungeonMiniMap( pTexUV );
	}
}

void CX2WorldMissionManager::CreateWorldBuffDragonBreath()
{
	// �巡���� ������ �̹� ���� �Ǿ� ������
	if ( NULL != m_pWorldBuffDragonBreath )
		return;

	// ���� ������ 2�ܰ� ���� ���� ���� �ʾ�����
	if ( 2 > GetWorldBuffStepNum() )
		return;	

	m_pWorldBuffDragonBreath = new WORLD_BUFF_DRAGON_BREATH( 100.0f, 3, 1.05f );	
}

void CX2WorldMissionManager::DeleteWorldBuffDragonBreath()
{
	SAFE_DELETE( m_pWorldBuffDragonBreath );
}

bool CX2WorldMissionManager::GetDragonBreathEnchantStatIncreased( OUT const CX2Stat::EnchantStat*& pEnchantStatInreased_ ) const
{
	if ( NULL == m_pWorldBuffDragonBreath )
	{
		pEnchantStatInreased_ = NULL;
		return false;
	}
	else
	{
		pEnchantStatInreased_ = &(m_pWorldBuffDragonBreath->m_EnchantStatIncreased);
		return true;
	}
}

const int CX2WorldMissionManager::GetDragonBreathAnimSpeedRateIncreased() const
{
	if ( NULL == m_pWorldBuffDragonBreath )
		return 0;
	else
		return m_pWorldBuffDragonBreath->m_iAnimSpeedRateIncreased;
}

const float CX2WorldMissionManager::GetDragonBreathAttackStatRateIncreased() const
{
	if ( NULL == m_pWorldBuffDragonBreath )
		return 0.0f;
	else
		return m_pWorldBuffDragonBreath->m_fAttackStatRateIncreased;
}

/** @function : GetEnchantStatFromDrgonBreath
	@brief : ��þƮ Ÿ�Ժ� ��ġ �� ����
	@param : CX2EnchantItem::ENCHANT_TYPE eType (��ġ�� �˰���� ��þƮ Ÿ��)
	@return : const float (�ش� ��þƮ Ÿ���� ��ġ ��)
*/
const float CX2WorldMissionManager::GetEnchantStatFromDrgonBreath( int iEnchantType ) const
{
	// �� �Լ��� �� �ϴ� ���� �Լ����� m_pWorldBuffDragonBreath�� ���Ͽ�
	// NULL Check�� �ؾ� ��
	switch ( CX2EnchantItem::ENCHANT_TYPE( iEnchantType ) )
	{
	case CX2EnchantItem::ET_BLAZE:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefBlaze;
		break;

	case CX2EnchantItem::ET_WATER:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefWater;
		break;

	case CX2EnchantItem::ET_NATURE:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefNature;
		break;

	case CX2EnchantItem::ET_WIND:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefWind;
		break;

	case CX2EnchantItem::ET_LIGHT:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefLight;
		break;

	case CX2EnchantItem::ET_DARK:
		return m_pWorldBuffDragonBreath->m_EnchantStatIncreased.m_fDefDark;
		break;

	default:
		return 0.0f;
		break;
	} // switch
	
}

const int CX2WorldMissionManager::GetElBlessCriticalRateIncreased() const
{
	if ( NULL == m_pWorldBuffElBless )
		return 0;
	else
		return m_pWorldBuffElBless->m_iCriticalRateIncreased;
}

const float CX2WorldMissionManager::GetElBlessDefenceStatRateIncreased() const
{
	if ( NULL == m_pWorldBuffElBless )
		return 0.0f;
	else
		return m_pWorldBuffElBless->m_fDefenceStatRateIncreased;
}

void CX2WorldMissionManager::CreateWorldBuffElBless()
{
	// ���� �ູ�� �̹� ���� �Ǿ� ������
	if ( NULL != m_pWorldBuffElBless )
		return;

	// ���� ������ 3�ܰ� ���� ���� ���� �ʾ�����
	if ( 3 > GetWorldBuffStepNum() )
		return;	

	m_pWorldBuffElBless = new WORLD_BUFF_EL_BLESS( 2, 1.05f );	
}

void CX2WorldMissionManager::DeleteWorldBuffElBless()
{
	SAFE_DELETE( m_pWorldBuffElBless );
}

void CX2WorldMissionManager::GetDefenceDungeonIcnoDesc( OUT wstringstream& wstrStreamDungeonIconDesc_ )
{
	if ( false == IsActiveDefenceDungeon() )
	{
		// ������ ���̻� ���潺 ������ ������ ������
		if ( -1 == GetRemainTimeMissionStart() )
		{
			wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_12117 ) << '\n';
		}
		// ���潺 ������ ���� �������� �˷� ��
		else
		{
			wstrStreamDungeonIconDesc_ << GET_STRING( STR_ID_12116 );

			int iMinute = GetRemainTimeMissionStart() + 1;

			// 60�� �̸��̸� ������ ǥ��
			if ( 60 > iMinute )
			{
				wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_12125, "i", iMinute ) ) << '\n';
			}
			// 60�� �̻� �̸� �ð����� ǥ��
			else
			{
				int iHour = iMinute / 60;	// ��
				iMinute %= 60;	// ������

				wstrStreamDungeonIconDesc_ << GET_REPLACED_STRING( ( STR_ID_12126, "ii", iHour, iMinute ) ) << '\n';
			}		
		}
	}
	else
	{
		wstrStreamDungeonIconDesc_ << L"";
	}
}

// void CX2WorldMissionManager::LoadScript()
// {
// 	const wstring wstrFileName( L"WordlMissionTest.lua" );
// 	string	strFileName;
// 
// 
// 	ConvertWCHARToChar( strFileName, wstrFileName );
// 	ConvertFileAnsiToUTF8( strFileName, strFileName );
// 
// 	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
// 
// 	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
// 	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( strFileName );
// 
// 	if( Info != NULL )
// 	{
// 		if( luaManager.DoMemory( Info->pRealData, Info->size ) == true )
// 		{
// 			if ( luaManager.BeginTable( "MINI_MAP" ) == true )
// 			{
// 				LUA_GET_VALUE(	luaManager, "MAGIC_X_LEFT", MAGIC_X_LEFT,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_X_RIGHT", MAGIC_X_RIGHT,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_Z_UP", MAGIC_Z_UP,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_Z_DOWN", MAGIC_Z_DOWN,	0 );
// 
// 				LUA_GET_VALUE(	luaManager, "MAGIC_POS_X", MAGIC_POS_X,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_POS_Y", MAGIC_POS_Y,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_WIDTH", MAGIC_WIDTH,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_HEIGHT", MAGIC_HEIGHT,	0 );
// 
// 				LUA_GET_VALUE(	luaManager, "MAGIC_EL_WIDTH", MAGIC_EL_WIDTH,	0 );
// 				LUA_GET_VALUE(	luaManager, "MAGIC_EL_HEIGHT", MAGIC_EL_HEIGHT,	0 );
// 
// 				luaManager.EndTable();
// 			}
// 		}
// 	}
// }



#endif SERV_INSERT_GLOBAL_SERVER

#ifdef NEW_DEFENCE_DUNGEON
//���ѳ� ������ ������ ���� ����� �� ���̵� ���� �����Ѵ�.
void CX2WorldMissionManager::ChangeDefenceDungeonLevel()
{
#ifdef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
	if( NULL == g_pData || NULL == g_pData->GetWorldMissionManager() )
		return;

	const int iBeforeDefenceCrystal = GetNowDefenceDungeonLevel();		/// ���� �� ����� �� ����
#endif FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
	const int iNumOfCrystal =  g_pData->GetWorldMissionManager()->GetNumOfWorldCrystalsProtected();

	if( iNumOfCrystal >= 0 && iNumOfCrystal < 7 )
		SetNowDefenceDungeonLevel( CX2Dungeon::DL_NORMAL );
	else if( iNumOfCrystal >= 7 && iNumOfCrystal < 13 )
		SetNowDefenceDungeonLevel( CX2Dungeon::DL_HARD );
	else
		SetNowDefenceDungeonLevel( CX2Dungeon::DL_EXPERT );

#ifdef FIX_NEW_DEFENCE_DUNGEON_PARTY_UI		/// ����� �� ���̵��� ����Ǿ��� ���� ��Ƽ UI ���� ó��
	if( iBeforeDefenceCrystal != GetNowDefenceDungeonLevel() )
	{
		if( NULL == g_pData->GetPartyManager() || NULL == g_pData->GetPartyManager()->GetMyPartyData() || NULL == g_pData->GetDungeonManager() )
			return;

		/// ����� ���� �������� ���� ����
		if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) )
		{
			if( NULL != g_pMain && NULL != g_pMain->GetPartyUI() )
				g_pMain->GetPartyUI()->UpdatePartyDLG();
		}
	}
#endif  FIX_NEW_DEFENCE_DUNGEON_PARTY_UI
}
#endif NEW_DEFENCE_DUNGEON

#ifdef SERV_2012_PVP_SEASON2_EVENT
void CX2WorldMissionManager::SetTimeOfGloryDLG()
{
	if( false == m_bShowTimeOfGloryDLG && true == m_bActiveTimeOfGlory )		/// ������ �ð� Ȱ�� ���¿���, ���̾�α� ǥ�õ��� �ʾ��� ��
	{
		if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP || g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )	/// �����̳� ������ ��
		{
			if( NULL == g_pData || NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )			/// ĳ�ü��� �ƴ� ��
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );

				if( NULL != pX2State )
					pX2State->OpenTimeOfGlory();	/// ������ �ð� ���̾�α� Ȱ��

				if( NULL != g_pData && NULL != g_pData->GetPremiumBuffManager() )		/// ������ �ð� ���� ����( ���� )
					g_pData->GetPremiumBuffManager()->SetLocalTimeOfGloryBuff();

				m_bShowTimeOfGloryDLG = true;		/// ���̾�α� ǥ�� ó�� ����
			}
		}
	}
	else if( true == m_bShowTimeOfGloryDLG && false == m_bActiveTimeOfGlory )	/// ������ �ð� ��Ȱ�� ���¿���, ���̾�α� ǥ�õ� ������ ��
	{
		m_bShowTimeOfGloryDLG = false;				/// ���̾�α� ǥ�� ó�� ����
	}
}

bool CX2WorldMissionManager::Handler_EGS_PVP_EVENT_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_EVENT_INFO_NOT kNot; 
	DeSerialize( pBuff, &kNot );

	m_bActiveTimeOfGlory = kNot.m_bEventInfo;		/// ������ �ð� ����

	if( false == m_bActiveTimeOfGlory )				/// ���� ��Ȱ�� �� ��, ������ �ð� ���� ����
	{
		if( NULL != g_pData->GetPremiumBuffManager() )
		{
			g_pData->GetPremiumBuffManager()->ErasePremiumBuff( CX2PremiumBuffTemplet::BI_TIME_OF_GLORY );
		}
	}

	return S_OK;
}
#endif SERV_2012_PVP_SEASON2_EVENT

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
bool CX2WorldMissionManager::Handler_EGS_EARTH_QUAKE_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )		/// ������ ���� ���� �̺�Ʈ ��Ŷ
{
	if( NULL != g_pData && NULL != g_pData->GetCashShop() && false == g_pData->GetCashShop()->GetOpen() &&
		NULL != g_pTFieldGame && false == g_pTFieldGame->GetJoinNpc() )					/// ĳ�ü� �� ��ȭ�� ���� ó��
	{
		if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetCamera() )
		{
			if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDeviceManager() )			/// ������ ���� ���
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"Event_Earthquake.ogg", false, false );

			g_pTFieldGame->GetCamera()->GetCamera().UpDownCrashCameraSmooth( 35.f, 10.f );	/// ī�޶� ũ���� ����

			if( NULL != g_pMain )
			{
				CX2StateField* pStateField	= NULL;

				pStateField	= static_cast< CX2StateField* >( g_pMain->GetNowState() );

				if( NULL != pStateField )
					pStateField->SetTogetherFestivalEarthQuakeNoticeTime( 5.f );		/// ������ ���� ���� �̺�Ʈ �߻��� �����Ǵ� ���� ����
			}
		}
	}

	return S_OK;
}
#endif TOGETHER_FESTIVAL_2012_AUTUMN