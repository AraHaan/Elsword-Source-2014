#include "StdAfx.h"
#include "X2EmblemManager.h"

#ifdef NEW_EMBLEM_MANAGER

CX2EmblemManager* CX2EmblemManager::m_pEmblemManager = NULL;
/*static */CX2EmblemManager* CX2EmblemManager::GetInstance()
{
	if( NULL == m_pEmblemManager)
		m_pEmblemManager = new CX2EmblemManager;

	return m_pEmblemManager;
}

/*static*/ void CX2EmblemManager::ResetEmblemManager()
{
	DestroyInstance();
	GetInstance()->OpenScriptFile(L"Emblem_Manager.lua");;
}

/*static*/ void CX2EmblemManager::DestroyInstance()
{
	SAFE_DELETE( m_pEmblemManager );
}

CX2EmblemManager::CX2EmblemManager():
m_bHasNextPlayEmblem( false ),
m_hCurrentEmblem(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hCurrentEmblem2nd(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_fWaitTime(0.f)
#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP		// ������, ���� ���� ��, ���� ��� ��� �߰�
, m_fSoundPlayingCheckTime ( 0.f )
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP	// ������, ���� ���� ��, ���� ��� ��� �߰�
{

}

CX2EmblemManager::~CX2EmblemManager()
{
	BOOST_FOREACH( EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		SAFE_DELETE ( pEmblem );
	}

#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP		// ������, ���� ���� ��, ���� ��� ��� �߰�
	// 	BOOST_FOREACH ( CKTDXDeviceSound * pSound, m_vecPlayingEmblemSound )
	// 	{
	// 		if ( NULL != pSound )
	// 		{
	// 			pSound->Stop();
	// 			SAFE_CLOSE(pSound);
	// 		}
	// 	}
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP	// ������, ���� ���� ��, ���� ��� ��� �߰�
}


bool CX2EmblemManager::OpenScriptFile( IN const WCHAR* pFileName_ )
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pEmblemManager", this );


    if ( g_pKTDXApp->LoadLuaTinker( pFileName_ ) == false )
    {
		ErrorLogMsg( XEM_ERROR149, pFileName_ );

		return false;
    }

	ParsingEmblemTemplet( luaManager );

	return true;
}
/** @function : ParsingEmblemTemplet 
	@brief : �⺻ ��ũ��Ʈ �Ľ�
*/
void CX2EmblemManager::ParsingEmblemTemplet( KLuaManager& luaManager )
{
	// �Ľ��� ����ID�� ���� ����Ʈ ���ϱ�
	vector<std::string> vecEmblemTempletNameUTF8;
	if( true == luaManager.BeginTable( "SHOW_EMBLEM_ID_LIST" ) )
	{
		int index	= 1; 
		std::string strValue = "";
		while( luaManager.GetValueUtf8( index,strValue ) == true )
		{
			if( false == strValue.empty() )
				vecEmblemTempletNameUTF8.push_back(strValue);
			index++;
		}
		luaManager.EndTable();
	}

	BOOST_FOREACH( const std::string& strTableNameUTF8, vecEmblemTempletNameUTF8 )
	{
		if( true == luaManager.BeginTable( strTableNameUTF8.c_str() ) )
		{
			EmblemTemplet* pEmblemTemplet = new EmblemTemplet;

			LUA_GET_VALUE_ENUM( luaManager, "EMBLEM_ID", pEmblemTemplet->m_eEmblemID, EMBLEM_ID, EI_NONE);

			bool bCheckDuplicate = false;
			BOOST_FOREACH( const EmblemTemplet* pEmblem, m_vecEmblemTemplet)
			{
				if( pEmblem->m_eEmblemID == pEmblemTemplet->m_eEmblemID )
				{
			#ifdef _IN_HOUSE_
					ASSERT(!"Duplicate Emblem Templet");
			#endif // _IN_HOUSE_
					bCheckDuplicate = true;
					break;
				}
			}
			if( true == bCheckDuplicate )
				continue;

			LUA_GET_VALUE( luaManager, "SHOW_STATE_VILLEAGE",		pEmblemTemplet->m_bShowVilleage, false );
			LUA_GET_VALUE( luaManager, "SHOW_STATE_BATTLE_FIELD",	pEmblemTemplet->m_bShowBattleFIeld, false );
			LUA_GET_VALUE( luaManager, "SHOW_STATE_DUNGEON_GAME",	pEmblemTemplet->m_bShowDungeon, false );

			LUA_GET_VALUE( luaManager, "IS_FIRST_ONLY",				pEmblemTemplet->m_bIsFirstOnly, false );
			LUA_GET_VALUE( luaManager, "CHECK_CONDITION",			pEmblemTemplet->m_bCheckCondition, false );

			LUA_GET_VALUE( luaManager, "SHOW_ORDER",			pEmblemTemplet->m_uiShowOrder, 0 );



			// ����Ʈ ���� ���� ���� �Ľ�
			LUA_GET_VALUE( luaManager, "TEXTUR_FILE_NAME",		pEmblemTemplet->m_wstrTextureFileName, L"" );

			LUA_GET_VALUE( luaManager, "DURATION_TIME",			pEmblemTemplet->m_vecTime.x, 0.f );
			LUA_GET_VALUE( luaManager, "FADE_IN_TIME",			pEmblemTemplet->m_vecTime.y, 0.f );
			LUA_GET_VALUE( luaManager, "FADE_OUT_TIME",			pEmblemTemplet->m_vecTime.z, 0.f );

			// ����Ʈ ���� ���� ���� �Ľ�
			LUA_GET_VALUE( luaManager, "TEXTUR_FILE_NAME_2ND",		pEmblemTemplet->m_wstrTextureFileName2nd, L"" );


			LUA_GET_VALUE( luaManager, "TEXTURE_1ST_COSTUOM_POS_X",			pEmblemTemplet->m_vTexture1stCostomPos.x, -1.f );
			LUA_GET_VALUE( luaManager, "TEXTURE_1ST_COSTUOM_POS_Y",			pEmblemTemplet->m_vTexture1stCostomPos.y, -1.f );
			
			LUA_GET_VALUE( luaManager, "TEXTURE_2ND_COSTUOM_POS_X",			pEmblemTemplet->m_vTexture2ndCostomPos.x, -1.f );
			LUA_GET_VALUE( luaManager, "TEXTURE_2ND_COSTUOM_POS_Y",			pEmblemTemplet->m_vTexture2ndCostomPos.y, -1.f );

#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP 	// ������, ���� ���� ��, ���� ��� ��� �߰�
			if( true == luaManager.BeginTable( "PLAY_SOUND_FILE_NAME_EMBLEM" ) )
			{		
				int iIndex = 1;
				while( true == luaManager.BeginTable( iIndex++  ) )
				{
					wstring wstrSoundFileName = L"";
					float fStartTime = 0;
					int eEmblemPopupSoundType = 0;
					int	eUnitType = 0;
					luaManager.GetValue( 1, wstrSoundFileName, L"" );
					luaManager.GetValue( 2, fStartTime, 0 );
					luaManager.GetValue( 3, eEmblemPopupSoundType, 0 );
					luaManager.GetValue( 4, eUnitType, 0 );

					EmblemPopupSound sEmblemPopupSound ( wstrSoundFileName, fStartTime, 
						static_cast<EMBLEM_POPUP_SOUND_TYPE> ( eEmblemPopupSoundType ), static_cast<CX2Unit::UNIT_TYPE> ( eUnitType ) );
					pEmblemTemplet->m_vecPlayEmblemSoundTemplet.push_back ( sEmblemPopupSound );
					luaManager.EndTable();
				}
			}
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP // ������, ���� ���� ��, ���� ��� ��� �߰�
			
#ifdef ADD_2013_CHARACTER_ADD_EVENT 	// ������, 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
			if( true == luaManager.BeginTable( "OPEN_TIME_SETTING" ) )
			{		
				int iIndex = 1;
				while( true == luaManager.BeginTable( iIndex++  ) )
				{
					int iYear, iMonth, iDay, iHour, iMinute, iRemainingMinute = -1;

					luaManager.GetValue( 1, iYear, 0 );
					luaManager.GetValue( 2, iMonth, 0 );
					luaManager.GetValue( 3, iDay, 0 );
					luaManager.GetValue( 4, iHour, 0 );
					luaManager.GetValue( 5, iMinute, 0 );
					luaManager.GetValue( 6, iRemainingMinute, 0 );

					OpenTimeSetting sOpenTimeSetting ( iYear, iMonth, iDay, iHour, iMinute, iRemainingMinute );
					pEmblemTemplet->m_vecOpenTimeSettingTemplet.push_back( sOpenTimeSetting );
					luaManager.EndTable();
				}
			}
			luaManager.EndTable();
#endif // ADD_2013_CHARACTER_ADD_EVENT	// ������, 2013 �ֵ� �߰� ��� ���� �̺�Ʈ

			m_vecEmblemTemplet.push_back( pEmblemTemplet );

			luaManager.EndTable();
		}
		else
		{
#ifdef _IN_HOUSE_
			ASSERT(!"Empty Emblem Templet");
#endif // _IN_HOUSE_
		}
	}

	// ��� ���� ���ؿ� �°� ����
	auto SortFunc = [](const EmblemTemplet* lhs_, const EmblemTemplet* rhs_ )->bool
	{
		return rhs_->m_uiShowOrder > lhs_->m_uiShowOrder;
	};	
	std::sort( m_vecEmblemTemplet.begin(), m_vecEmblemTemplet.end(), SortFunc );

}

/** @function : CreateEventSequence 
	@brief : ��ũ��Ʈ ������ ���� ��ƼŬ ������ ����
	@param : wstrTextureFile_ : �ؽ��� ���ϸ�
			 vTime.x : LifeTime
			 vTime.y : Fade In Time
			 vTime.z : Fade Out Time
*/
CKTDGParticleSystem::CParticleEventSequence* CX2EmblemManager::CreateEventSequence( const WCHAR* wstrTextureFile_, 
	const D3DXVECTOR3& vTime)
{
	if( NULL == g_pData || NULL == g_pData->GetUIMajorParticle() )
		return NULL;

	CKTDGParticleSystem::CParticleEventSequence* pParticleSequence= 
		CKTDGParticleSystem::CParticleEventSequence::CreateParticleEventSequence( g_pData->GetUIMajorParticle(), NULL );

	pParticleSequence->SetParticleType(CKTDGParticleSystem::PT_2D_PLANE);
	pParticleSequence->SetLayer(21);
	pParticleSequence->SetSrcBlendMode(D3DBLEND_SRCALPHA);
	pParticleSequence->SetDestBlendMode(D3DBLEND_INVSRCALPHA);
	pParticleSequence->SetZEnable(false);
	pParticleSequence->SetResolutionConvert(true);
	pParticleSequence->SetForceLayer(false);
	pParticleSequence->SetMaxParticleNum(1);
	pParticleSequence->SetTriggerCount(1);
	
	// ��ü ���ӽð��� ��ũ��Ʈ�κ��� ���� ������ ����
	pParticleSequence->SetLifetime(CMinMax<float>(vTime.x, vTime.x));
	pParticleSequence->SetEmitRate(1000.f,1000.f);
	//pParticleSequence->SetRenderStateID( CKTDGStateManager::RT_REAL_COLOR );

	// �ؽ��� ����
	CKTDGParticleSystem::CParticleEvent_Texture* pEventTexture = new CKTDGParticleSystem::CParticleEvent_Texture();
	pEventTexture->SetTextureName( wstrTextureFile_ );
	pEventTexture->SetActualTime( CMinMax<float>(0, 0) );
	pEventTexture->SetFade( false );
	pParticleSequence->m_EventList.push_back(pEventTexture);

	// ������ ����
	CKTDXDeviceTexture* pTexture = 
		pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wstrTextureFile_ );
	
	if( NULL != pTexture )
	{
		CKTDGParticleSystem::CParticleEvent_Size* pEventSize = new CKTDGParticleSystem::CParticleEvent_Size();
		D3DXVECTOR3 TextureSize = D3DXVECTOR3( static_cast<float>(pTexture->GetWidth()), 
			static_cast<float>(pTexture->GetHeight()), 1.f);
		pEventSize->SetSize(CMinMax<D3DXVECTOR3> (TextureSize, TextureSize) );
		pEventSize->SetActualTime( CMinMax<float>(0, 0) );
		pEventSize->SetFade( false );
		pParticleSequence->m_EventList.push_back(pEventSize);
	}

	// �÷� ����
	// Initialize
	CKTDGParticleSystem::CParticleEvent_Color* pEventColor = new CKTDGParticleSystem::CParticleEvent_Color();
	pEventColor->SetColor(CMinMax<D3DXCOLOR>( D3DXCOLOR(1,1,1,0), D3DXCOLOR(1,1,1,0)));
	pEventColor->SetActualTime( CMinMax<float>(0, 0) );
	pEventColor->SetFade( false );
	pParticleSequence->m_EventList.push_back(pEventColor);

	// Fade In
	pEventColor = new CKTDGParticleSystem::CParticleEvent_Color();
	pEventColor->SetColor(CMinMax<D3DXCOLOR>( D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1)));
	pEventColor->SetActualTime( CMinMax<float>(0.f, vTime.y) );
	pEventColor->SetFade( true );
	pParticleSequence->m_EventList.push_back(pEventColor);

	// Fade out
	pEventColor = new CKTDGParticleSystem::CParticleEvent_Color();
	pEventColor->SetColor(CMinMax<D3DXCOLOR>( D3DXCOLOR(1,1,1,0), D3DXCOLOR(1,1,1,0)));
	pEventColor->SetActualTime( CMinMax<float>( (vTime.x - vTime.z ), vTime.x) );
	pEventColor->SetFade( true );
	pParticleSequence->m_EventList.push_back(pEventColor);

	return pParticleSequence;
}
/** @function : PlayEmblemEffect 
	@brief : ����Ʈ ���� �Լ�
*/
void CX2EmblemManager::PlayEmblemEffect( const EmblemTemplet& sEmblemTemplet_ )
{
#ifdef DIALOG_SHOW_TOGGLE
	// ������ // 2013-07-24
	// ���� ��忡���� ���� ��½�Ű�� �ʱ�.
	// ���ٸ� ���� ó�� ���� �������� �ʵ��� ó��
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif // DIALOG_SHOW_TOGGLE

	if( NULL == g_pData || NULL == g_pData->GetUIMajorParticle() )
		return;

#ifdef _IN_HOUSE_
	if( true == sEmblemTemplet_.m_wstrTextureFileName.empty() )
		DISPLAY_ERROR( L"TextureFile Name don't exist" );
#endif // _IN_HOUSE_

#ifdef ADD_2013_CHARACTER_ADD_EVENT		// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
	if ( false == SetOpenTimeEventEmblem ( sEmblemTemplet_ ) )
		return;
#endif // ADD_2013_CHARACTER_ADD_EVENT	// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ


	{
		CKTDGParticleSystem::CParticleEventSequence* pSequence = 
			CreateEventSequence( sEmblemTemplet_.m_wstrTextureFileName.c_str(), sEmblemTemplet_.m_vecTime );
		if( NULL != pSequence )
		{
			pSequence->SetOverUI( true );
			D3DXVECTOR3 vPos;
			sEmblemTemplet_.GetTexture1stPos(vPos);
			m_hCurrentEmblem = 
				g_pData->GetUIMajorParticle()->CreateInstanceNonTemplet(pSequence, 
				vPos, D3DXVECTOR2(-1,-1), D3DXVECTOR2(-1,-1) ); 
		
#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP		// ������, ���� ���� ��, ���� ��� ��� �߰�
			vector<EmblemPopupSound> vecEmblemPopupSoundPickJustOneUnitType;
			m_vecNowPlayEmblemSound.clear();			
			m_fSoundPlayingCheckTime = 0.f;
			BOOST_FOREACH ( EmblemPopupSound sEmblemPopupSound, sEmblemTemplet_.m_vecPlayEmblemSoundTemplet )
			{
				switch ( sEmblemPopupSound.m_eEmblemPopupSoundType )
				{
					case EPST_NONE :
						{
							m_vecNowPlayEmblemSound.push_back( sEmblemPopupSound );	
						}
						break;

					case EPST_RELATIVE_UNIT_TYPE_RANDOM :
						{
							if ( NULL != g_pData->GetMyUser() && 
								NULL != g_pData->GetMyUser()->GetSelectUnit() &&
								NULL != g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() )
							{
								CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
								if ( eUnitType == sEmblemPopupSound.m_eUnitType )
								{
									vecEmblemPopupSoundPickJustOneUnitType.push_back ( sEmblemPopupSound );
								}
							}
						}
					break;

					default :
						break;
				}
			}

			int iArraySize = static_cast<int> ( vecEmblemPopupSoundPickJustOneUnitType.size () );
			if ( iArraySize > 0 )
			{
				srand( (unsigned) time( NULL ) );
				int iRandomArray = rand() % iArraySize;
				if ( static_cast<int> ( vecEmblemPopupSoundPickJustOneUnitType.size() ) > iRandomArray )
					m_vecNowPlayEmblemSound.push_back( vecEmblemPopupSoundPickJustOneUnitType[iRandomArray] );	
			}
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP	// ������, ���� ���� ��, ���� ��� ��� �߰�

		}
	}

	if( false == sEmblemTemplet_.m_wstrTextureFileName2nd.empty() )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSequence = 
			CreateEventSequence( sEmblemTemplet_.m_wstrTextureFileName2nd.c_str(), sEmblemTemplet_.m_vecTime );
		if( NULL != pSequence )
		{
			D3DXVECTOR3 vPos;
			sEmblemTemplet_.GetTexture2ndPos(vPos);
			pSequence->SetOverUI( true );
			m_hCurrentEmblem2nd = 
				g_pData->GetUIMajorParticle()->CreateInstanceNonTemplet(pSequence, 
				vPos, D3DXVECTOR2(-1,-1), D3DXVECTOR2(-1,-1) ); 
		}
	}
}

/**	@function : InitStateChange 
	@biref : ���� ���� ���� ���� �ʱ�ȭ
	@param : bCharChange : ĳ���� ���� �ʱ�ȭ��� ��� �ʱ�ȭ �ϱ�
*/
void CX2EmblemManager::InitEmbelmPlayInfo( bool bCharChange /*= false */)
{
	BOOST_FOREACH( EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		if( false == pEmblem->m_bIsFirstOnly ||
			true == bCharChange )
		{
			pEmblem->InitShowState();
		}
		pEmblem->InitIsPassCondition();
	}
	m_bHasNextPlayEmblem = true;

	// ��� ���� ������ �ִٸ� ����
	if( NULL != g_pData->GetUIMajorParticle() )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCurrentEmblem )
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCurrentEmblem );

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCurrentEmblem2nd )
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCurrentEmblem2nd );
	}

	m_fWaitTime = 3.f;
}

void CX2EmblemManager::OnFrameMove( float fElpaseTime )
{
#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP		// ������, ���� ���� ��, ���� ��� ��� �߰�
	{
		m_fSoundPlayingCheckTime += fElpaseTime;
	
		BOOST_FOREACH ( EmblemPopupSound & sEmblemPopupSound, m_vecNowPlayEmblemSound )
		{
			if ( m_fSoundPlayingCheckTime >= sEmblemPopupSound.m_fStartTime && false == sEmblemPopupSound.m_bIsPlaying)
			{
				CKTDXDeviceSound* pSound = g_pKTDXApp->GetDeviceManager()->PlaySound( sEmblemPopupSound.m_wstrSoundFileName.c_str(), false, false );
				m_vecPlayingEmblemSound.push_back( pSound );
				sEmblemPopupSound.m_bIsPlaying = true;
			}
		}
	}
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP	// ������, ���� ���� ��, ���� ��� ��� �߰�


	if( NULL != g_pData->GetCashShop() &&
		true == g_pData->GetCashShop()->GetOpen() )
	{	
		// ��� ���� ������ �ִٸ� ����
		if( NULL != g_pData->GetUIMajorParticle() )
		{
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCurrentEmblem )
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCurrentEmblem );

			if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCurrentEmblem2nd )
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hCurrentEmblem2nd );
		}
		return;
	}
	if( m_fWaitTime > 0 )
	{
		m_fWaitTime -= fElpaseTime;
		return;
	}

	if( NULL == g_pData->GetUIMajorParticle() )
		return;

	// ������� �� ������ �ϳ��� ���ٸ� return;
	if( false == m_bHasNextPlayEmblem )
		return;

	// �������� ������ �ִٸ� return;
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hCurrentEmblem &&
		true == g_pData->GetUIMajorParticle()->IsLiveInstanceHandle( m_hCurrentEmblem ))
	{
		return;
	}
#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP		// ������, ���� ���� ��, ���� ��� ��� �߰�
// 	BOOST_FOREACH ( CKTDXDeviceSound * pSound, m_vecPlayingEmblemSound )
// 	{
// 		if ( NULL != pSound )
// 		{
// 			pSound->Stop();
// 			SAFE_CLOSE(pSound);
// 		}
// 	}
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP	// ������, ���� ���� ��, ���� ��� ��� �߰�

	m_hCurrentEmblem = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hCurrentEmblem2nd = INVALID_PARTICLE_SEQUENCE_HANDLE;
	BOOST_FOREACH( EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		// ������Ʈ üũ
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( false == pEmblem->m_bShowVilleage )
					continue;
			}
			break;
		case CX2Main::XS_DUNGEON_GAME:
			{
				if( false == pEmblem->m_bShowDungeon )
					continue;

				if( true == g_pMain->GetIsPlayingTutorial() )
				{
					pEmblem->m_bIsAlreadyShow = true;
					continue;
				}
			}
			break;
		case CX2Main::XS_BATTLE_FIELD:
			{
				if( false == pEmblem->m_bShowBattleFIeld )
					continue;

#ifdef FIELD_BOSS_RAID
				if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
					continue;
#endif // FIELD_BOSS_RAID
			}
			break;
		default:
			return;
			break;
		}

		if( false == pEmblem->m_bIsAlreadyShow && // ���� �������� �ʾҰ�
			true == pEmblem->m_bIsPassCondition ) // ���� ��� �ߴٸ�
		{
			pEmblem->m_bIsAlreadyShow = true;
			pEmblem->m_bIsPassCondition = false;

			PlayEmblemEffect(*pEmblem);
			PlayEmblemOtherProcess(*pEmblem);
			CheckNextPlayingEmblem();
			break;
		}
	}
}
/** @function : CheckNextPlayingEmblem
	@brief : ������ ��� �� ������ �ִ��� üũ
*/
void CX2EmblemManager::CheckNextPlayingEmblem()
{
	bool bHasNextPlayEmblem = false;
	BOOST_FOREACH( const EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		// ���� ������� ���� ����Ʈ�� �ִٸ�
		if( false == pEmblem->m_bIsAlreadyShow )
			bHasNextPlayEmblem = true;
	}

	m_bHasNextPlayEmblem = bHasNextPlayEmblem;
}
/** @function : PlayEmblem 
	@brief : ������ ������ ������ ������ Ȱ��ȭ �����ִ� �Լ�
*/
void CX2EmblemManager::PlayEmblem( EMBLEM_ID eEmblemID_, bool bReShow_ /*= false*/ )
{	
	BOOST_FOREACH( EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		if( eEmblemID_ == pEmblem->m_eEmblemID )
		{
			pEmblem->m_bIsPassCondition = true;
			if( true == bReShow_ )
				pEmblem->m_bIsAlreadyShow = false;
			return;
		}
	}
}
/** @function : EndEmblem 
	@brief : ������ ������ ������ ������ ��Ȱ��ȭ �����ִ� �Լ�
*/
void CX2EmblemManager::EndEmblem( EMBLEM_ID eEmblemID_ )
{
	BOOST_FOREACH( EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		if( eEmblemID_ == pEmblem->m_eEmblemID )
		{
			pEmblem->m_bIsPassCondition = false;
			return;
		}
	}
}
void CX2EmblemManager::PlayEmblemOtherProcess( const EmblemTemplet& sEmblemTemplet_ )
{
	switch( sEmblemTemplet_.m_eEmblemID )
	{
	case EI_EVENT_ELESIS:
		{
			if ( NULL != g_pX2Game )
			{
				g_pX2Game->GetMyUnit()->SetBuffFactorToGameUnitByBuffFactorID( BFI_EMPTY_EXP_BUFF );
			}
		} break;

#ifdef EVENT_NEW_HENIR
	case EI_WEEKEND_BURNING_EVENT:
		{
// 			if ( NULL != g_pX2Game &&
// 				 NULL != g_pX2Game->GetMyUnit() &&
// 				 true == IsBurningEventTime() )
// 			{
// 				g_pX2Game->GetMyUnit()->SetBuffFactorToGameUnitByBuffFactorID( BFI_EMPTY_EXP_BUFF );
// 			}
		} break;
#endif // EVENT_NEW_HENIR
#ifdef FIELD_BOSS_RAID
	case EI_OPEN_FIELD_BOSS_RAID:
		{
			if( NULL != g_pChatBox )
			{
				const SEnum::VILLAGE_MAP_ID eBossFieldMapId = CX2BossRaidManager::GetInstance()->GetBossRaidCreatorMapID();
				if( eBossFieldMapId != SEnum::VMI_INVALID )
				{
					g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_29332, "S", g_pData->GetBattleFieldManager().GetBattleFieldNameByBattleFieldId(eBossFieldMapId) )) ,
						KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFF0000" );		
				}
			}
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"ES_FieldRaidBoss_Gate_Notice.ogg", false, false );
		} break; 
	case EI_CLOSE_FIELD_BOSS_RAID:
		{
			CX2BossRaidManager::GetInstance()->InitRaidFieldMapID();
		} break;
#endif // FIELD_BOSS_RAID
#ifdef ADD_2013_CHARACTER_ADD_EVENT			// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
	case EI_EVENT_BURNING_NEW_CHARACTER_TRIPLE :
	case EI_EVENT_BURNING_NEW_CHARACTER_DOUBLE :
		{

		} break;

#endif // ADD_2013_CHARACTER_ADD_EVENT		// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
#ifdef WORLD_BUFF_2013_CHRISTMAS 	// ������, 2013�� ũ���������� ���� ����
	case EI_EVENT_2013_CHRISTMAS :
		{

		} break;
#endif // WORLD_BUFF_2013_CHRISTMAS	// ������, 2013�� ũ���������� ���� ����
	default:
		{
		} break;
	}
}
bool CX2EmblemManager::HasEmblem( EMBLEM_ID eEmblemID_ )
{	
	BOOST_FOREACH( const EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		if( eEmblemID_ == pEmblem->m_eEmblemID )
		{
			return true;
		}
	}
	return false;
}
void CX2EmblemManager::SetTexture2ndName( EMBLEM_ID eEmblemID_, const wstring& wstrTextureName )
{
	BOOST_FOREACH( EmblemTemplet* pEmblem, m_vecEmblemTemplet)
	{
		if( eEmblemID_ == pEmblem->m_eEmblemID )
		{
			pEmblem->m_wstrTextureFileName2nd = wstrTextureName;
		}
	}
}
#ifdef EVENT_NEW_HENIR
bool CX2EmblemManager::IsBurningEventTime() const
{
	if( NULL == g_pData )
		return false;

	CTime ctCurrentTime( g_pData->GetServerCurrentTime() );

	const int iCurrentMonth = ctCurrentTime.GetMonth();
	const int iCurrentDay = ctCurrentTime.GetDay();
	const int iCurrentHour = ctCurrentTime.GetHour();
	
	switch( iCurrentMonth )
	{
#ifdef _OPEN_TEST_
	case 12: // 12�� 14��/15�� 14:00~15:02 (2���� ���� ��)
		{
			if( 11 == iCurrentDay )
			{
				if( iCurrentHour >= 19 && iCurrentHour < 20 )
				{
					return true;
				}
			}
		} break;
#else // _OPEN_TEST_

#ifdef _SERVICE_
	case 12: // 12�� 14��/15�� 14:00~15:02 (2���� ���� ��)
		{
			if( 14 == iCurrentDay ||
				15 == iCurrentDay )
			{
				if( iCurrentHour >= 14 && iCurrentHour < 15 )
				{
					return true;
				}
			}
		} break;
#else // _SERVICE_

		// �系 QA
	case 12: // 12�� 14�� 11:00~12:00 / 14:00~15:00 (2���� ���� ��)
		{
			if( 12 == iCurrentDay )
			{
				if( iCurrentHour >= 5 && iCurrentHour < 7 )
				{
					return true;
				}
			}
		} break;
#endif // _SERVICE_

#endif // _OPEN_TEST_

	default:
		break;
	}

	return false;
}
#endif // EVENT_NEW_HENIR

#endif // NEW_EMBLEM_MANAGER

#ifdef ADD_2013_CHARACTER_ADD_EVENT		// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
bool CX2EmblemManager::SetOpenTimeEventEmblem ( const EmblemTemplet& sEmblemTemplet_ )
{
	if( NULL == g_pData )
		return false;
	if ( true == sEmblemTemplet_.m_vecOpenTimeSettingTemplet.empty() )
		return true;

	CTime ctCurrentTime( g_pData->GetServerCurrentTime() );
	const int TERM_TIME_OF_CHECK_EVENT = 2;		// �̺�Ʈ ���� ���� �ð�, 2��
	BOOST_FOREACH ( OpenTimeSetting cOpenTimeSetting, sEmblemTemplet_.m_vecOpenTimeSettingTemplet )
	{
		CTime ctEventTime ( cOpenTimeSetting.m_iYear, cOpenTimeSetting.m_iMonth, cOpenTimeSetting.m_iDay, cOpenTimeSetting.m_iHour, cOpenTimeSetting.m_iMinute, 0 );

		CTimeSpan ctsElaspedTime = ctCurrentTime - ctEventTime;

		double fLeftTime = static_cast<double> ( ctsElaspedTime.GetTotalMinutes() );
		if ( fLeftTime < ( cOpenTimeSetting.m_iRemainingMinute ) + TERM_TIME_OF_CHECK_EVENT
			&& fLeftTime > 0  )
		{
			return true;
		}
	}
	return false;
}
#endif // ADD_2013_CHARACTER_ADD_EVENT	// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ