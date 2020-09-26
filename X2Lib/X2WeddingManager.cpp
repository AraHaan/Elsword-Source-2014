#include "stdafx.h"
#include ".\X2WeddingManager.h"
#ifdef ADDED_RELATIONSHIP_SYSTEM

/*
CX2WeddingManager ���� ���� ����

1. CX2WeddingGame �����ڸ� ���� ����
2. ���� ��� ���� ��ũ��Ʈ �Ľ� ( �ַ�, ������ ��ũ��Ʈ )
3. CX2StateWeddingGame ���� �� �� �ַ�, ���� ID�� �Բ� ���� ���� ����
4. StartWedding() �Լ��� ���� ��ȥ�� ����.
5. StartOfficiantsMessage() �Լ��� ���� �ַʻ� ����
6. EndWedding() �Լ��� ���� ��ȥ�� ������ ����

*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CX2WeddingManager::CX2WeddingManager() :
m_bIsPlayWedding(false),
m_fWeddingTime(0.f),
m_eCurrentWorldID(CX2World::WI_NONE)
{
	m_pOfficiantManager = new CX2OfficiantManger;
	m_pWeddingHallManager = new CX2WeddingHallManager;
}//CX2WeddingManager::CX2WeddingManager()


// destructor.
CX2WeddingManager::~CX2WeddingManager(void)
{ 
	SAFE_DELETE( m_pOfficiantManager );
	SAFE_DELETE( m_pWeddingHallManager );
}//CX2WeddingManager::~CX2WeddingManager()

HRESULT CX2WeddingManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if( NULL != m_pOfficiantManager )
		m_pOfficiantManager->OnFrameMove( fTime, fElapsedTime );

 	if( NULL != m_pWeddingHallManager )
 		m_pWeddingHallManager->OnFrameMove( fTime, fElapsedTime );

	// ��ȥ�� ���� �� �����ؾ� �� �׸�
	if( m_bIsPlayWedding )
	{
		m_fWeddingTime += fElapsedTime;

		OnFrameMove_Notice();
		OnFrameMove_Move();
		OnFrameMove_Sound();
		OnFrameMove_Effectset();
		OnFrameMove_SoundStop(fElapsedTime);
	}

	if( L"" != m_NoticeInfo.wstrNotice )
		m_NoticeInfo.m_fElapseTime += fElapsedTime;

	return S_OK;
}
HRESULT	CX2WeddingManager::OnFrameRender()
{
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return S_OK;

	// ��ȥ�� �ȳ� ����
	if( L"" != m_NoticeInfo.wstrNotice )
	{
		// ������ �ð� ��ŭ ������ٸ� �ʱ�ȭ
		if( m_NoticeInfo.m_fElapseTime >= m_NoticeInfo.m_fMaxShowTime )
			m_NoticeInfo.wstrNotice = L"";

		if( NULL != m_pWeddingHallManager )
		{
			int iAlarmX = 0;
			int iAlarmY = 240;

			RECT rt;
			rt.left = 0;
			rt.right =  (LONG)1024;

			D3DXCOLOR vColor, vOutlineColor;
			m_pWeddingHallManager->GetNoticeColor(vColor);
			m_pWeddingHallManager->GetNoticeOutlineColor(vOutlineColor);

			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL )->OutTextMultiline( iAlarmX, iAlarmY, 
				m_NoticeInfo.wstrNotice.c_str(), vColor, CKTDGFontManager::FS_SHELL, vOutlineColor,
				1.f, &rt, DT_CENTER|DT_TOP);
		}
	}

	return S_OK;
}

void CX2WeddingManager::OnFrameMove_Notice()
{
	if( true == m_WeddingBehavior.m_vecBehavior_Notice.empty() )
		return;

	CX2WeddingBehavior::Behavior_Notice CurrentBehavior = m_WeddingBehavior.m_vecBehavior_Notice[0];

	if( CurrentBehavior.m_fStartTime <= m_fWeddingTime )
	{
		m_NoticeInfo.m_fElapseTime = 0.f;
		m_NoticeInfo.m_fMaxShowTime = CurrentBehavior.m_fDurationTime;

		// ���� ���� �������� �Ŷ�,�ź� �̸� �߰�
		if( true == CurrentBehavior.m_bEndNotice && NULL != g_pWeddingGame )
		{
			bool bIsGetHeroineNickName = false;
			wstring wstrGroomNickName = L"";
			wstring wstrBrideNickName = L"";
			
			for( int i = 0; i < g_pWeddingGame->GetSquareUnitNum(); ++i )
			{
				CX2SquareUnit* pSquareUnit = g_pWeddingGame->GetSquareUnit(i);	
				if( NULL != pSquareUnit && pSquareUnit->GetUnit() )
				{
					switch( static_cast<SEnum::WEDDING_STATUS>(pSquareUnit->GetWeddingStatus()) )
					{
					case SEnum::WS_GROOM:
						{
							wstrGroomNickName = pSquareUnit->GetNickName();
						} break;
					case SEnum::WS_BRIDE:
						{
							wstrBrideNickName = pSquareUnit->GetNickName();
						} break;
					default:
						break;
					}
				}

				if( L"" != wstrGroomNickName && L"" != wstrBrideNickName )
				{
					bIsGetHeroineNickName = true;
					break;		
				}
			}
			if( true == bIsGetHeroineNickName )
			{
				m_NoticeInfo.wstrNotice = 
					GET_REPLACED_STRING( (CurrentBehavior.m_iStrID, "LL", 
										  wstrGroomNickName, wstrBrideNickName ) );
			}
		}
		// �Ϲ� ������� STRING_ID�� ���ؼ��� ������.
		else
		{
			m_NoticeInfo.wstrNotice = GET_STRING( CurrentBehavior.m_iStrID );
		}

		m_WeddingBehavior.m_vecBehavior_Notice.erase( m_WeddingBehavior.m_vecBehavior_Notice.begin() );
	}
}
void CX2WeddingManager::OnFrameMove_Move()
{
	if( true == m_WeddingBehavior.m_vecBehavior_Move.empty() )
		return;

	// ���� ��ġ ���� �̵� �ൿ ó��.
	CX2WeddingBehavior::Behavior_Move CurrentBehavior = m_WeddingBehavior.m_vecBehavior_Move[0];

	if( CurrentBehavior.m_fStartTime <= m_fWeddingTime )
	{
		switch ( CurrentBehavior.eWeddingPosMove )
		{
		case CX2WeddingBehavior::WPMT_WEDDING_START:
			{
				if( NULL != g_pWeddingGame && g_pWeddingGame->GetMyUnit() &&
					NULL != g_pWeddingGame->GetWorld() && NULL != g_pWeddingGame->GetWorld()->GetLineMap() )
				{
					UINT uiStartPos = 
						m_pWeddingHallManager->GetStartPosIndexByWeddingStatus( g_pWeddingGame->GetMyUnit()->GetWeddingStatus() );
					D3DXVECTOR3 vPos = 
						g_pWeddingGame->GetWorld()->GetLineMap()->GetStartPosition( uiStartPos );

					g_pWeddingGame->GetMyUnit()->SetPos( vPos );
				}
			} break;
		case CX2WeddingBehavior::WPMT_OFFICIANT_START:
			{
				if( NULL != g_pWeddingGame && g_pWeddingGame->GetMyUnit() &&
					NULL != g_pWeddingGame->GetWorld() && NULL != g_pWeddingGame->GetWorld()->GetLineMap() )
				{
					UINT uiStartPos = 
						m_pWeddingHallManager->GetOfficiantStartPosIndexByWeddingStatus( g_pWeddingGame->GetMyUnit()->GetWeddingStatus() );
					D3DXVECTOR3 vPos = 
						g_pWeddingGame->GetWorld()->GetLineMap()->GetStartPosition( uiStartPos );

					g_pWeddingGame->GetMyUnit()->SetPos( vPos );
				}
				// �ַʻ� ����
				m_pOfficiantManager->StartOfficiantsMessage();
			} break;
		default:
			break;
		}
		
		m_WeddingBehavior.m_vecBehavior_Move.erase( m_WeddingBehavior.m_vecBehavior_Move.begin() );
	}
}
void CX2WeddingManager::OnFrameMove_Sound()
{
	if( true == m_WeddingBehavior.m_vecBehavior_Sound.empty() )
		return;

	// ȿ���� ó��.
	CX2WeddingBehavior::Behavior_Sound CurrentBehavior = m_WeddingBehavior.m_vecBehavior_Sound[0];

	if( CurrentBehavior.m_fStartTime <= m_fWeddingTime )
	{
		// ���ӽð��� �����ϸ� ���� �� �� �ֵ��� �߰� �۾�
		if( -1.f != CurrentBehavior.m_fDurationTime )
		{
			CX2WeddingBehavior::Behavior_DurationTimeSound DurationTimeSound;
			DurationTimeSound.m_pSound = g_pKTDXApp->GetDeviceManager()->PlaySound( CurrentBehavior.m_wstrSoundFileName.c_str(), false, false );

			if( NULL != DurationTimeSound.m_pSound )
			{
				DurationTimeSound.m_fTime = 0.f;
				DurationTimeSound.m_fDurationTime = CurrentBehavior.m_fDurationTime;
				m_WeddingBehavior.m_vecBehavior_DurationTimeSound.push_back( DurationTimeSound );
			}
		}
		else
		{
			g_pKTDXApp->GetDeviceManager()->PlaySound( CurrentBehavior.m_wstrSoundFileName.c_str(), false, false );
		}

		m_WeddingBehavior.m_vecBehavior_Sound.erase( m_WeddingBehavior.m_vecBehavior_Sound.begin() );
	}
}
void CX2WeddingManager::OnFrameMove_SoundStop( float fElapsedTime_ )
{
	if( true == m_WeddingBehavior.m_vecBehavior_DurationTimeSound.empty() )
		return;

	// ȿ���� ���ӽð� üũ.
	BOOST_FOREACH( CX2WeddingBehavior::Behavior_DurationTimeSound& DurationTimeSound, m_WeddingBehavior.m_vecBehavior_DurationTimeSound )
	{
		if( NULL != DurationTimeSound.m_pSound )
		{
			DurationTimeSound.m_fTime += fElapsedTime_;
			if( DurationTimeSound.m_fTime >= DurationTimeSound.m_fDurationTime )
			{
				DurationTimeSound.m_pSound->Stop();
				DurationTimeSound.m_pSound = NULL;
			}
		}
	}
}
void CX2WeddingManager::OnFrameMove_Effectset()
{
	if( true == m_WeddingBehavior.m_vecBehavior_Effectset.empty() )
		return;

	CX2WeddingBehavior::Behavior_Effectset CurrentBehavior = m_WeddingBehavior.m_vecBehavior_Effectset[0];

	if( CurrentBehavior.m_fStartTime <= m_fWeddingTime )
	{
		if( NULL != g_pWeddingGame )
		{
			g_pWeddingGame->PlayEffectSet_WeddingGame( CurrentBehavior.m_wstrEffectSetName.c_str(),
											CurrentBehavior.m_vPos,
											CurrentBehavior.m_ePosType );

			m_WeddingBehavior.m_vecBehavior_Effectset.erase( m_WeddingBehavior.m_vecBehavior_Effectset.begin() );
		}
	}
}

/** @function : OpenScriptFile
	@brief : ��ȥ���� ���� ��ũ��Ʈ �Ľ�
	@param : 
*/
bool CX2WeddingManager::OpenScriptFile( const WCHAR* pOfficiantFileName_,  
									const WCHAR* pWeddingHallFileName_ , 
									const WCHAR* pWeddingManagerFileName_)
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pWeddingManager", this );

	if( false == g_pKTDXApp->LoadLuaTinker( pOfficiantFileName_ ) )
	{
		ErrorLogMsg( XEM_ERROR147, pOfficiantFileName_ );
		return false;
	}

	if( false == g_pKTDXApp->LoadLuaTinker( pWeddingHallFileName_ ) )
	{
		ErrorLogMsg( XEM_ERROR147, pWeddingHallFileName_ );
		return false;
	}

	if( true == g_pKTDXApp->LoadLuaTinker( pWeddingManagerFileName_ ) )
	{
		m_WeddingBehavior.OpenScript( luaManager );
	}

	return true;
}

/** @function : StartWedding
	@brief : ��ȥ�� ����
	@param : 
*/
void CX2WeddingManager::StartWedding()
{
	if( NULL == m_pOfficiantManager )
		return;

	if( false == m_pOfficiantManager->CreateOfficiant() )
	{
		ASSERT(!L"�ַʻ� Error");
	}

	m_bIsPlayWedding = true;
}
/** @function : AddOfficiantTemplet_LUA
	@brief : ��ũ��Ʈ���� �ַ� ���ø� ���
			 m_pOfficiantManager�� �Լ��� ����ϱ� ���� �����Լ�
	@param : 
	*/
bool CX2WeddingManager::AddOfficiantTemplet_LUA()
{
	if( NULL == m_pOfficiantManager )
		return false;

	return m_pOfficiantManager->AddOfficiantTemplet_LUA();
}

/** @function : AddOfficiantTemplet_LUA
	@brief : ��ũ��Ʈ���� ������ ���ø� ���
			 m_pWeddingHallManager�� �Լ��� ����ϱ� ���� �����Լ�
	@param : 
	*/
bool CX2WeddingManager::AddWeddingHallTemplet_LUA()
{
	if( NULL == m_pWeddingHallManager )
		return false;

	return m_pWeddingHallManager->AddWeddingHallTemplet_LUA();
}

/** @function : SetOfficiant
	@brief : ���� ������ �������� �ַ� ����
	@param : eOfficiantType_(�ַ� ID)
*/
void CX2WeddingManager::SetOfficiant( SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ )
{
	if( NULL == m_pOfficiantManager )
		return;

	m_pOfficiantManager->SetOfficiant( eOfficiantType_ );
}
/** @function : SetWeddingHall
	@brief : ������ ID�� ���� ������ �� SetWeddingHallAndOfficiantInfo�ַ� ���� ����
	@param : eHallType_(������ ID)
*/
void CX2WeddingManager::SetWeddingHallAndOfficiantInfo( SEnum::WEDDING_HALL_TYPE eHallType_ )
{
	// ������ ����
	if( NULL != m_pWeddingHallManager )
	{
		m_pWeddingHallManager->SetWeddingHall(eHallType_);
		m_eCurrentWorldID = m_pWeddingHallManager->GetCurrentWorldID();

		// ������ �� �ٸ� �ַ� ���� ������ ����
		if( NULL != m_pOfficiantManager )
		{
			D3DXVECTOR3 vPos, vEye, vLookAt;
			m_pWeddingHallManager->GetOfficiantPos(vPos);
			m_pWeddingHallManager->GetOfficiantsMessageEye(vEye);
			m_pWeddingHallManager->GetOfficiantsMessageLookAt(vLookAt);

			m_pOfficiantManager->SetOfficiantPos( vPos, vEye, vLookAt );
		}
	}
}
UINT CX2WeddingManager::GetStartPosIndexByWeddingStatus( IN SEnum::WEDDING_STATUS eWeddingStatus_) const
{
	if( NULL != m_pWeddingHallManager )
	{
		return m_pWeddingHallManager->GetStartPosIndexByWeddingStatus( eWeddingStatus_ );
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingHallManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CX2WeddingHallManager::CX2WeddingHallManager():
m_eCurrentWeddingHallType( SEnum::WHT_CASH_A ),
m_pCurrentWeddingHallTemplet(NULL)
{

}

CX2WeddingHallManager::~CX2WeddingHallManager()
{
	std::map<SEnum::WEDDING_HALL_TYPE, WeddingHallTemplet*>::iterator mit = m_mapWeddingHallTemplet.begin();
	for( ; mit != m_mapWeddingHallTemplet.end() ; ++mit)
	{
		SAFE_DELETE( mit->second );
	}
}

HRESULT CX2WeddingHallManager::OnFrameMove( double fTime, float fElapsedTime )
{
	return S_OK;
}

bool CX2WeddingHallManager::AddWeddingHallTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	WeddingHallTemplet* pWeddingHallTemplet = new WeddingHallTemplet;

	LUA_GET_VALUE_ENUM( luaManager, "WEDDING_HALL_TYPE",			pWeddingHallTemplet->m_eWeddingHallType,			SEnum::WEDDING_HALL_TYPE,		SEnum::WHT_CASH_A);
	LUA_GET_VALUE_ENUM( luaManager, "WORLD_ID",						pWeddingHallTemplet->m_eWorldID,					CX2World::WORLD_ID,				CX2World::WI_SANDER_WEDDING);

	LUA_GET_VALUE( luaManager, "BRIDE_START_POS",					pWeddingHallTemplet->m_iBrideStartPos,  0 );
	LUA_GET_VALUE( luaManager, "GROOM_START_POS",					pWeddingHallTemplet->m_iGroomStartPos,  0 );

	LUA_GET_VALUE( luaManager, "BRIDE_OFFICIANT_START_POS",			pWeddingHallTemplet->m_iBrideOfficiantStartPos,  0 );
	LUA_GET_VALUE( luaManager, "GROOM_OFFICIANT_START_POS",			pWeddingHallTemplet->m_iGroomOfficiantStartPos,  0 );

	if( luaManager.BeginTable( "WEDDING_GUEST_START_POS" ) == true )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == true )
		{
			if( buf > 0 )
				pWeddingHallTemplet->m_vecWeddingGuestStartPos.push_back( buf );
			index++;
		}
		luaManager.EndTable();
	}

	if( true == luaManager.BeginTable( "OFFCIANT_POSISTION" ) )
	{
		LUA_GET_VALUE( luaManager, "OFFCIANT_POS_X",					pWeddingHallTemplet->m_vOffciantPos.x,  0.f );
		LUA_GET_VALUE( luaManager, "OFFCIANT_POS_Y",					pWeddingHallTemplet->m_vOffciantPos.y,  0.f );
		LUA_GET_VALUE( luaManager, "OFFCIANT_POS_Z",					pWeddingHallTemplet->m_vOffciantPos.z,  0.f );

		LUA_GET_VALUE( luaManager, "EYE_POS_X",							pWeddingHallTemplet->m_vOffciantCameraEye.x,  0.f );
		LUA_GET_VALUE( luaManager, "EYE_POS_Y",							pWeddingHallTemplet->m_vOffciantCameraEye.y,  0.f );
		LUA_GET_VALUE( luaManager, "EYE_POS_Z",							pWeddingHallTemplet->m_vOffciantCameraEye.z,  0.f );

		LUA_GET_VALUE( luaManager, "LOOKAT_POS_X",						pWeddingHallTemplet->m_vOffciantCmaeraLookAt.x,  0.f );
		LUA_GET_VALUE( luaManager, "LOOKAT_POS_Y",						pWeddingHallTemplet->m_vOffciantCmaeraLookAt.y,  0.f );
		LUA_GET_VALUE( luaManager, "LOOKAT_POS_Z",						pWeddingHallTemplet->m_vOffciantCmaeraLookAt.z,  0.f );


		luaManager.EndTable();
	}

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "COLOR", pWeddingHallTemplet->m_vColor, D3DXCOLOR(1,1,1,1) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "OUTLINE_COLOR", pWeddingHallTemplet->m_vOutlineColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pWeddingHallTemplet->m_vColor		= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "COLOR" );
	pWeddingHallTemplet->m_vOutlineColor= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "OUTLINE_COLOR" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	m_mapWeddingHallTemplet[ pWeddingHallTemplet->m_eWeddingHallType ] = pWeddingHallTemplet ;
	return true;
}

/** @function : SetWeddingHall
	@brief : WeddingHall ����
	@param : WEDDING_HALL_TYPE ( ������ ID )
*/
void CX2WeddingHallManager::SetWeddingHall( SEnum::WEDDING_HALL_TYPE eHallType_ )
{
	m_eCurrentWeddingHallType = eHallType_;

	if( m_mapWeddingHallTemplet.find( m_eCurrentWeddingHallType ) != m_mapWeddingHallTemplet.end() )
	{
		m_pCurrentWeddingHallTemplet = m_mapWeddingHallTemplet[m_eCurrentWeddingHallType];
	}
}

/** @function : GetCurrentWorldID
	@brief : �����忡 ������ ���� ID ���. ���� �Ŵ��� Ŭ�������� ȣ���.
*/
CX2World::WORLD_ID CX2WeddingHallManager::GetCurrentWorldID() const
{
	if( NULL != m_pCurrentWeddingHallTemplet )
		return m_pCurrentWeddingHallTemplet->m_eWorldID;

	return CX2World::WI_NONE;
}

void CX2WeddingHallManager::GetOfficiantPos( OUT D3DXVECTOR3& vPos_ ) const
{
	if( NULL != m_pCurrentWeddingHallTemplet )
		vPos_ = m_pCurrentWeddingHallTemplet->m_vOffciantPos;
}
void CX2WeddingHallManager::GetOfficiantsMessageEye( OUT D3DXVECTOR3& vEye_ ) const
{
	if( NULL != m_pCurrentWeddingHallTemplet )
		vEye_ = m_pCurrentWeddingHallTemplet->m_vOffciantCameraEye;
}
void CX2WeddingHallManager::GetOfficiantsMessageLookAt( OUT D3DXVECTOR3& vLookAt_ ) const
{
	if( NULL != m_pCurrentWeddingHallTemplet )
		vLookAt_ = m_pCurrentWeddingHallTemplet->m_vOffciantCmaeraLookAt;
}
UINT CX2WeddingHallManager::GetStartPosIndexByWeddingStatus( SEnum::WEDDING_STATUS eWeddingStatus_ ) const
{
	if( NULL != m_pCurrentWeddingHallTemplet )
	{
		switch ( eWeddingStatus_ ) 
		{
		case SEnum::WS_BRIDE:
			{
				return m_pCurrentWeddingHallTemplet->m_iBrideStartPos;
			} break;
		case SEnum::WS_GROOM:
			{
				return m_pCurrentWeddingHallTemplet->m_iGroomStartPos;
			} break;
		case SEnum::WS_GUEST:
			{
				if( false == m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos.empty() )
				{
					int iRand = rand() % static_cast<int>(m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos.size());
					return m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos[iRand];
				}
			} break;
		default:
			break;
		}
	}
	return 0;
}
void CX2WeddingHallManager::GetQuestStartPosList( OUT vector<int>& vecStartPos_ )
{
	if( NULL != m_pCurrentWeddingHallTemplet )
	{
		vecStartPos_ = m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos;
	}
}
UINT CX2WeddingHallManager::GetOfficiantStartPosIndexByWeddingStatus( SEnum::WEDDING_STATUS eWeddingStatus_ ) const
{
	if( NULL != m_pCurrentWeddingHallTemplet )
	{
		switch ( eWeddingStatus_ ) 
		{
		case SEnum::WS_BRIDE:
			{
				return m_pCurrentWeddingHallTemplet->m_iBrideOfficiantStartPos;
			} break;
		case SEnum::WS_GROOM:
			{
				return m_pCurrentWeddingHallTemplet->m_iGroomOfficiantStartPos;
			} break;
		case SEnum::WS_GUEST:
			{
				if( false == m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos.empty() )
				{
					int iRand = rand() % static_cast<int>(m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos.size());
					return m_pCurrentWeddingHallTemplet->m_vecWeddingGuestStartPos[iRand];
				}
			} break;
		default:
			return 0;
			break;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2OfficiantManger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CX2OfficiantManger::CX2OfficiantManger():
m_eCurrentOfficiantType(SEnum::WOT_NPC_A),
m_hOfficiantMesh(INVALID_MESH_INSTANCE_HANDLE),
m_pCurrentOfficiant(NULL),
m_bIsPlayOfficiantsMessage(false),
m_fOfficiantTime(0.f)
{

}

CX2OfficiantManger::~CX2OfficiantManger()
{
	if( INVALID_MESH_INSTANCE_HANDLE != m_hOfficiantMesh )
	{
		if( NULL != g_pData->GetGameMajorXMeshPlayer() )
			g_pData->GetGameMajorXMeshPlayer()->DestroyInstanceHandle( m_hOfficiantMesh );
	}

	std::map<SEnum::WEDDING_OFFICIANT_TYPE, OfficiantTemplet*>::iterator mit = m_mapOfficiantTemplet.begin();
	for( ; mit != m_mapOfficiantTemplet.end() ; ++mit)
	{
		BOOST_FOREACH( OfficiantBehavior* pBehavior, mit->second->m_vecOfficiantBehavior )
		{
			SAFE_DELETE( pBehavior );
		}
		SAFE_DELETE( mit->second );
	}

	if( NULL != m_pCurrentOfficiant )
	{
		BOOST_FOREACH( OfficiantBehavior* pBehavior, m_pCurrentOfficiant->m_vecOfficiantBehavior )
		{
			SAFE_DELETE( pBehavior );
		}
	}
	SAFE_DELETE( m_pCurrentOfficiant );
}

HRESULT CX2OfficiantManger::OnFrameMove( double fTime, float fElapsedTime )
{
	if( NULL == m_pCurrentOfficiant )
		return S_OK;

	// �ַʻ� ���� ���� ���� ��� ���
	if( true == m_bIsPlayOfficiantsMessage )
	{
		if( false == m_pCurrentOfficiant->m_vecOfficiantBehavior.empty() )
		{
			OfficiantBehavior* pCurrentBehavior = m_pCurrentOfficiant->m_vecOfficiantBehavior[0];
			if( NULL == pCurrentBehavior )
				return S_OK;

			m_fOfficiantTime += fElapsedTime;
			// ������ ���� �ð� �̻� �귶���� �ൿ ����
			if( m_fOfficiantTime >= pCurrentBehavior->m_fTalkDelayTime )
			{
				m_fOfficiantTime = 0.f;

				ChangeOfficiantMotion( pCurrentBehavior->m_wstrMotionName.c_str(), pCurrentBehavior->m_eAnimPlayType );
				PlayOfficiantsMessage( pCurrentBehavior->m_iTalkStrID );
				m_pCurrentOfficiant->m_vecOfficiantBehavior.erase( m_pCurrentOfficiant->m_vecOfficiantBehavior.begin() );
			}
		}
		else
			m_bIsPlayOfficiantsMessage = false;
	}

	return S_OK;
}
/** @function : ChangeOfficiantMotion
	@brief : �ַ��� ����� ����
	@param : pMotionName_ (����̸�), eAnimPlayType_( ��� ��� Ÿ��)
*/
void CX2OfficiantManger::ChangeOfficiantMotion( const WCHAR* pMotionName_, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE eAnimPlayType_ )
{
	if( L"" == pMotionName_ )
		return;

	if( INVALID_MESH_INSTANCE_HANDLE != m_hOfficiantMesh )
	{
		if( NULL != g_pData->GetGameMajorXMeshPlayer() )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMesh = 
				g_pData->GetGameMajorXMeshPlayer()->GetMeshInstance( m_hOfficiantMesh );

			if( NULL != pMesh )
			{
				pMesh->ChangeAnim( pMotionName_, eAnimPlayType_, 1.f ) ;
			}
		}
	}
}
/** @function : PlayOfficiantsMessage
	@brief : ���� �ַ� ��ġ�� �ַʻ� ���
	@param : pMotionName_ (����̸�), eAnimPlayType_( ��� ��� Ÿ��)
*/
void CX2OfficiantManger::PlayOfficiantsMessage( int iStrID_ )
{
	if( NULL != g_pWeddingGame &&  NULL != m_pCurrentOfficiant )
	{
		g_pWeddingGame->PopOfficiantTalkBox( iStrID_, m_vPos + m_pCurrentOfficiant->m_vTalkBoxOffsetPos );	
	}
}

/** @function : AddOfficiantTemplet_LUA
	@brief : ��ũ��Ʈ���� ������ �ַ� ���� �׸� ���
*/
bool CX2OfficiantManger::AddOfficiantTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	OfficiantTemplet* pOfficiantTemplet = new OfficiantTemplet ;

	LUA_GET_VALUE_ENUM( luaManager, "OFFICANT_TYPE",			pOfficiantTemplet->m_eOfficantType,			SEnum::WEDDING_OFFICIANT_TYPE,		SEnum::WOT_NPC_END);
	if( SEnum::WOT_NPC_END == pOfficiantTemplet->m_eOfficantType )
		return false;

	LUA_GET_VALUE( luaManager, "OFFICANT_MOTION_NAME",			pOfficiantTemplet->m_wstrMeshTempletName,  L"" );
	if( L"" == pOfficiantTemplet->m_wstrMeshTempletName )
		return false;

	LUA_GET_VALUE( luaManager, "TALKBOX_POSITION_OFFSET_X",		pOfficiantTemplet->m_vTalkBoxOffsetPos.x,  0.f );
	LUA_GET_VALUE( luaManager, "TALKBOX_POSITION_OFFSET_Y",		pOfficiantTemplet->m_vTalkBoxOffsetPos.y,  0.f );
	LUA_GET_VALUE( luaManager, "TALKBOX_POSITION_OFFSET_Z",		pOfficiantTemplet->m_vTalkBoxOffsetPos.z,  0.f );
 
	if( true == luaManager.BeginTable( "OFFICANT_BEHAVIOR" ) )
	{
		int iBehaviorTableIndex = 1;
		while( true == luaManager.BeginTable( iBehaviorTableIndex  ) )
		{
			OfficiantBehavior* pOfficiantBehavior = new OfficiantBehavior;

			LUA_GET_VALUE( luaManager, 1,		pOfficiantBehavior->m_fTalkDelayTime,	 0.f );
			LUA_GET_VALUE( luaManager, 2,		pOfficiantBehavior->m_iTalkStrID ,		 STR_ID_0 );
			LUA_GET_VALUE( luaManager, 3,		pOfficiantBehavior->m_wstrMotionName,	 L"" );
			LUA_GET_VALUE_ENUM( luaManager, 4,	pOfficiantBehavior->m_eAnimPlayType,	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE ,		CKTDGXSkinAnim::XAP_LOOP);

			pOfficiantTemplet->m_vecOfficiantBehavior.push_back( pOfficiantBehavior );

			++iBehaviorTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}
	
	LUA_GET_VALUE( luaManager, "CAMERA_DURATION_TIME",			pOfficiantTemplet->m_fCameraDurationTime,  0.f );
	
	m_mapOfficiantTemplet[ pOfficiantTemplet->m_eOfficantType ] = pOfficiantTemplet ;
	return true;
}

/** @function : CreateOfficiant
	@brief : �ַ� NPC ����
*/
bool CX2OfficiantManger::CreateOfficiant()
{
	if( m_mapOfficiantTemplet.find(m_eCurrentOfficiantType) == m_mapOfficiantTemplet.end() )
		return false;

	if( NULL != m_pCurrentOfficiant )
	{
		SAFE_DELETE( m_pCurrentOfficiant )

		if( INVALID_MESH_INSTANCE_HANDLE != m_hOfficiantMesh )
		{
			if( NULL != g_pData->GetGameMajorXMeshPlayer() )
				g_pData->GetGameMajorXMeshPlayer()->DestroyInstanceHandle( m_hOfficiantMesh );
		}
	}

	if( NULL == m_pCurrentOfficiant )
	{
		m_pCurrentOfficiant = new OfficiantTemplet;
		*m_pCurrentOfficiant = *m_mapOfficiantTemplet[m_eCurrentOfficiantType];

		if( NULL != g_pData && NULL != g_pData->GetGameMajorXMeshPlayer() )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMesh =
				g_pData->GetGameMajorXMeshPlayer()->CreateInstance( NULL, m_pCurrentOfficiant->m_wstrMeshTempletName.c_str(),  m_vPos, D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0) );
			if( NULL != pMesh )
			{
				m_hOfficiantMesh = pMesh->GetHandle();
			}
		}
	}
	return true;
}

/** @function : PlayOfficiantsMessage
	@brief : �ַʻ� ����
*/
void CX2OfficiantManger::StartOfficiantsMessage()
{
	if( NULL == m_pCurrentOfficiant )
		return;

	if( true == m_pCurrentOfficiant->m_vecOfficiantBehavior.empty() )
		return;

	PlayOfficiantCamera( m_vCameraEye, m_vCameraLootAt, m_pCurrentOfficiant->m_fCameraDurationTime );
	m_bIsPlayOfficiantsMessage = true;
}
/** @function : PlayOfficiantCamera
	@brief : ������ ��ġ�� ī�޶� �̵�
	@param : vEye( �� ��ġ ), vLookAt(�ٶ� ���� ��), fTrackingTime(���ӽð�)
*/
void CX2OfficiantManger::PlayOfficiantCamera( const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, float fTrackingTime )
{
	if( NULL == g_pWeddingGame ||
		NULL == g_pWeddingGame->GetCamera() )
		return;

	g_pWeddingGame->SetOfficiantCameraTime( fTrackingTime );
	g_pWeddingGame->GetCamera()->SquareLineCamera( NULL, vEye, vLookAt, false, fTrackingTime );
}

/** @function : SetOfficiantPos
	@brief : �ַʿ��� �ʿ��� ��ġ�� ����
	@param : vPos_(�ַ���ġ), vEye_( �� ��ġ ), vLookAt_(�ٶ� ���� ��)
*/
void CX2OfficiantManger::SetOfficiantPos( const D3DXVECTOR3& vPos_, const D3DXVECTOR3& vCameraEye_, const D3DXVECTOR3& vCameraLookAt_)
{ 
	m_vPos = vPos_; 
	m_vCameraEye = vCameraEye_; 
	m_vCameraLootAt = vCameraLookAt_; 
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingBehavior
// ��ȥ�� ���࿡ �ʿ��� �ൿ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CX2WeddingBehavior::CX2WeddingBehavior()
{
}

CX2WeddingBehavior::~CX2WeddingBehavior()
{
}

void CX2WeddingBehavior::OpenScript( KLuaManager& luaManager_ )
{
	// �ൿ�� �ð� ������ �°� �Է� �� �� �ֵ��� ���� ó��
	float fPreStartTime = 0.f;

	if( true == luaManager_.BeginTable( "WEDDING_BEHAVIOR_NOTICE" ) )
	{
		int iTableIndex = 1;
		while( true == luaManager_.BeginTable( iTableIndex  ) )
		{
			Behavior_Notice stNotice;

			LUA_GET_VALUE( luaManager_, 1,		stNotice.m_fStartTime,		0.f );			
			CheckTimeError( stNotice.m_fStartTime, fPreStartTime );
			fPreStartTime = stNotice.m_fStartTime;

			LUA_GET_VALUE( luaManager_, 2,		stNotice.m_fDurationTime,	0.f);
			LUA_GET_VALUE( luaManager_, 3,		stNotice.m_iStrID,			STR_ID_1 );
			LUA_GET_VALUE( luaManager_, 4,		stNotice.m_bEndNotice,		false );

			m_vecBehavior_Notice.push_back( stNotice );

			++iTableIndex;
			luaManager_.EndTable();
		}

		luaManager_.EndTable();
	}

	fPreStartTime = 0.f;
	if( true == luaManager_.BeginTable( "WEDDING_BEHAVIOR_MOVE" ) )
	{
		int iTableIndex = 1;
		while( true == luaManager_.BeginTable( iTableIndex  ) )
		{
			Behavior_Move stMove;

			LUA_GET_VALUE( luaManager_, 1,		stMove.m_fStartTime,		0.f );
			CheckTimeError( stMove.m_fStartTime, fPreStartTime );
			fPreStartTime = stMove.m_fStartTime;

			LUA_GET_VALUE_ENUM( luaManager_, 2,	stMove.eWeddingPosMove,	WEDDING_POS_MOVE_TYPE,	WPMT_WEDDING_START);
			m_vecBehavior_Move.push_back( stMove );

			++iTableIndex;
			luaManager_.EndTable();
		}

		luaManager_.EndTable();
	}

	fPreStartTime = 0.f;
	if( true == luaManager_.BeginTable( "WEDDING_BEHAVIOR_SOUND" ) )
	{
		int iTableIndex = 1;
		while( true == luaManager_.BeginTable( iTableIndex  ) )
		{
			Behavior_Sound stSound;

			LUA_GET_VALUE( luaManager_, 1,		stSound.m_fStartTime,		0.f );
			CheckTimeError( stSound.m_fStartTime, fPreStartTime );
			fPreStartTime = stSound.m_fStartTime;

			LUA_GET_VALUE( luaManager_, 2,		stSound.m_wstrSoundFileName,	L"");

			LUA_GET_VALUE( luaManager_, 3,		stSound.m_fDurationTime,	-1.f );
			

			m_vecBehavior_Sound.push_back( stSound );

			++iTableIndex;
			luaManager_.EndTable();
		}

		luaManager_.EndTable();
	}


	fPreStartTime = 0.f;
	if( true == luaManager_.BeginTable( "WEDDING_BEHAVIOR_EFFECTSET" ) )
	{
		int iTableIndex = 1;
		while( true == luaManager_.BeginTable( iTableIndex  ) )
		{
			Behavior_Effectset stEffect;

			LUA_GET_VALUE( luaManager_, 1,		stEffect.m_fStartTime,		0.f );
			CheckTimeError( stEffect.m_fStartTime, fPreStartTime );
			fPreStartTime = stEffect.m_fStartTime;

			LUA_GET_VALUE( luaManager_, 2,		stEffect.m_wstrEffectSetName, L"" );
			LUA_GET_VALUE_ENUM( luaManager_, 3,	stEffect.m_ePosType,	WEDDING_EFFECTSET_POS_TYPE,	WEPT_CUSTOM);
			switch( stEffect.m_ePosType )
			{
			case WEPT_CUSTOM:
				{
					LUA_GET_VALUE( luaManager_, 4,		stEffect.m_vPos.x,		0.f );
					LUA_GET_VALUE( luaManager_, 5,		stEffect.m_vPos.y,		0.f );
					LUA_GET_VALUE( luaManager_, 6,		stEffect.m_vPos.z,		0.f );
				} break;
			default:
				{
					stEffect.m_vPos = D3DXVECTOR3(0,0,0);
				}
				break;
			}

			m_vecBehavior_Effectset.push_back( stEffect );

			++iTableIndex;
			luaManager_.EndTable();
		}

		luaManager_.EndTable();
	}
}
void CX2WeddingBehavior::CheckTimeError( float fBehaviorTime_, float fPreBehabiorTime_ )
{
#ifndef _SERVICE_
	if( fBehaviorTime_ < fPreBehabiorTime_ )
	{
		MessageBox( NULL, L"Wedding_Manager ��ũ��Ʈ ���� ����\n�ð� ������ �°� ���� ���ּ���", L"Error", MB_OK );
	}
#endif //_SERVICE_
}
#endif // ADDED_RELATIONSHIP_SYSTEM