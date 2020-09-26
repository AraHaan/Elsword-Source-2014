#include "stdafx.h"
#include ".\x2worldmanager.h"


#ifdef KHELP_SYSTEM_TEST // 2008-10-24
IMPLEMENT_HELP_SYSTEM( CX2WorldManager )
{
    WCHAR   wbuffer[1024];
    int     ix = 16;
    int     iy = 16;
    StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer), L"%S", __FUNCTION__ );

    if ( g_pMain && g_pData )
    {
	    g_pMain->GetFontForDebuf()->OutTextXY( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
		    CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
        g_pData->GetWorldManager()->Debug_RenderHelp( ix, iy );
    }//if
}//IMPLEMENT_HELP_SYSTEM( CX2WorldManager )
#endif // KHELP_SYSTEM_TEST // 2008-10-24


///-----------------------------------------------------------------------------
//
// constructor.
CX2WorldManager::CX2WorldManager()
{
	::InitializeCriticalSection( &m_csWorldManager );
    m_pX2WorldLatestRef         = NULL;
}//CX2WorldManager::CX2WorldManager()


// destructor.
CX2WorldManager::~CX2WorldManager(void)
{
	::EnterCriticalSection( &m_csWorldManager );

	for( int i = 0; i < (int)m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		SAFE_DELETE( pWorldData );
	}
	m_WorldDataList.clear();

	::LeaveCriticalSection( &m_csWorldManager );

	::DeleteCriticalSection( &m_csWorldManager );
}//CX2WorldManager::~CX2WorldManager()

#ifdef X2TOOL
CX2World* CX2WorldManager::CreateWorld( wstring wstrWorldName, KLuabinder* pKLuabinder )
{
	CX2World::WorldData worldData;
	worldData.dataFileName = wstrWorldName;
	CX2World* pWorld = new CX2World( &worldData );
	pWorld->OpenScriptFile( wstrWorldName.c_str() );

	return pWorld;
}
#endif

CX2World* CX2WorldManager::CreateWorld( const CX2World::WORLD_ID worldID, KLuabinder* pKLuabinder /*= NULL*/, const bool bBackgroundLoad_ /*= false*/ )
{
	CX2World::WorldData* pWorldData = NULL;

	{
		CSLock locker( m_csWorldManager );

		for( UINT i = 0; i < m_WorldDataList.size(); i++ )
		{
			if( worldID == m_WorldDataList[i]->worldID )
			{
	#ifdef FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
				g_pKTDXApp->GetDSManager()->SetListenerData( D3DXVECTOR3( 99999.f, 99999.f, 99999.f ) );
	#endif //FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
				pWorldData = m_WorldDataList[i];
				break;
			}
		}//for
	}

	// WorldData�� ������
	if ( NULL == pWorldData )
		return NULL;
	else // WorldData�� ������
	{
		CX2World* pCX2World = new CX2World( pWorldData, bBackgroundLoad_ );
		ASSERT( pCX2World );

		m_pX2WorldLatestRef = pCX2World; // ���� �ֱٿ� ������ ���� ��ü�� ���� ���� �����͸� ������ �д�.

		pCX2World->OpenScriptFile( pWorldData->dataFileName.c_str(), pKLuabinder );

#ifdef X2OPTIMIZE_GAMEOPTION_BUGFIX
		pCX2World->SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );
#endif//X2OPTIMIZE_GAMEOPTION_BUGFIX

		return pCX2World;
	}
}//CX2WorldManager::CreateWorld()

bool CX2WorldManager::OpenScriptFile( const WCHAR* pFileName )
{
    ASSERT( pFileName );
    CSLock locker( m_csWorldManager );

	//���δ��� ���
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pWorldManager", this );

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR72, pFileName );

		return false;
    }

	return true;
}


//{{ seojt // 2008-10-22, 10:33
void CX2WorldManager::OnPreDestroyWorld(CX2World* pWorld_)
{
    ASSERT( pWorld_ );
    /** � ���� ��ü�� �ı��Ǹ�, ���� �ֱٿ� ������ ���� ��ü�� ���� ���� �����͸� ��ȿȭ��Ų��.
        CX2WorldManager�� ��ü�� �����ϴ� ���� ���������, ��ü�� �ı��ϴ� ���� �ð� ����
        �����Ƿ�, �� �κ��� �����ؾ� �� ���̴�. - jintaeks on 2008-10-22, 10:30
    
        @���� ��:
        
            step 1) CX2WorldManager�� �ڽ��� ������ ��� ���� ���� ��ü�� ����Ʈ�� �����Ѵ�.

            step 2) CX2World�� ���� delete���� �ʴ� �ٴ� ���� �����ϱ� ���� CX2World�� �ı���
                    ~CX2World()�� private�� �����ϰ�, CX2WorldManager�� CX2World ��ü�� �ı��ϴ�
                    ���� �Լ��� �����Ѵ�. CX2WorldManager�� CX2World�� private �ı��ڿ� ������ ��
                    �־�� �ϹǷ�, CX2World���� friend�� ����Ǿ�� �� ���̴�.

            step 3) �� ���� ������Ʈ�� CX2World�� ���� �ı����� �ʰ� - ���� ���� SAFE_DELETE()�� ȣ���Ѵ� -
                    CX2WorldManager���� CX2World::WORLD_ID�� �ش��ϴ� ���带 �ı��� ���� ��û�Ѵ�.

            ���) CX2WorldManager�� ��ü�� �����ϴ� factory�� ������ �ϹǷ�, ��ü�� �ı��� ������ �����Ѽ�
                    ������ factory�� �ǵ��� �ؾ� �� ���̴�.

        - jintaeks on 2008-10-22, 10:26 */

    m_pX2WorldLatestRef = NULL;

    // ���� ���忡�� ��û�� ���ҽ� ������ ��� clear�Ѵ�.
    ASSERT( g_pKTDXApp && g_pKTDXApp->GetDeviceManager() );
}//CX2WorldManager::OnPreDestroyWorld()
//}} seojt // 2008-10-22, 10:33



const CX2World::WorldData* CX2WorldManager::GetWorldData( CX2World::WORLD_ID eWorldID )
{
	for( UINT i=0; i<m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		if( pWorldData->worldID == eWorldID )
		{
			return pWorldData;
		}
	}

	return NULL;
}


bool CX2WorldManager::AddWorldData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	CX2World::WorldData* pWorldData = new CX2World::WorldData;
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "worldID",			pWorldData->worldID,		CX2World::WORLD_ID,	CX2World::WI_NONE,	SAFE_DELETE(pWorldData); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, "worldName",		pWorldData->worldName,		L"",										SAFE_DELETE(pWorldData); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, "dataFileName",	pWorldData->dataFileName,	L"",										SAFE_DELETE(pWorldData); return false; );

	m_WorldDataList.push_back( pWorldData );
	return true;
}


//{{ seojt // 2008-10-21, 15:14
bool CX2WorldManager::SetPreprocessingData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	WORLD_PREPROCESSING_INFO    preInfo;

    // qfff
    preInfo.m_eInfoType = X2WM_PPIT_UNKNOWN;
	LUA_GET_VALUE( luaManager, "meshName",             preInfo.m_wstrName,         L"" );
	//LUA_GET_VALUE( luaManager, "centerPos",		    preInfo.m_vCenter,          D3DXVECTOR3(0,0,0) );
	LUA_GET_VALUE( luaManager, "boundingSphereRadius",	preInfo.m_fBSphereRadiua,  0.f );
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "centerPos", preInfo.m_vCenter, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    preInfo.m_vCenter = lua_tinker::get<D3DXVECTOR3>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "centerPos" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


    ///-----------------------------------------------------------------------------
    // ���� ������ CX2World*�� ��´�.
    // �̸��� �ش��ϴ� ���� ��ü�� ã�Ƽ� �ٿ�� �� ������ �����Ѵ�.
    //
    if ( m_pX2WorldLatestRef )
    {
        CX2WorldObject* pWorldObject = m_pX2WorldLatestRef->FindObjectByName( preInfo.m_wstrName );
        if ( pWorldObject )
        {
            pWorldObject->SetBoundingRadius( preInfo.m_fBSphereRadiua );
            pWorldObject->SetCenter( preInfo.m_vCenter );
        }//if
    }//if

	return true;
}//CX2WorldManager::SetPreprocessingData_LUA()
//}} seojt // 2008-10-21, 15:14


CX2World::WORLD_ID CX2WorldManager::GetWorldIDUsingLineMapFileName( const WCHAR* pLineMapFileName )
{
	CSLock locker( m_csWorldManager );

	for( UINT i=0; i<m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		CX2World* pWorld = new CX2World( pWorldData );

		lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pWorld", pWorld );

		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pWorldData->dataFileName );
		if( Info == NULL )
			continue;

		wstring wstrWorldScript;
		ConvertCharToWCHAR( wstrWorldScript, Info->pRealData, Info->size );

		int iIndex		= wstrWorldScript.find( L"g_pWorld:SetLineMap" );
		int iStartIndex = iIndex;
		iStartIndex		= wstrWorldScript.find( L"\"", iStartIndex );
		int iEndIndex	= iStartIndex+1;
		iEndIndex		= wstrWorldScript.find( L"\"", iEndIndex );

		wstring wstrLineMapFileName = wstrWorldScript.substr( iStartIndex+1, iEndIndex-iStartIndex-1 );
		MakeUpperCase( wstrLineMapFileName );
		wstring wstrInputLineMapFileName = pLineMapFileName;
		MakeUpperCase( wstrInputLineMapFileName );
		if( 0 == wstrLineMapFileName.compare( wstrInputLineMapFileName.c_str() ) )
		{
			SAFE_DELETE( pWorld );
			return pWorldData->worldID;
		}
		SAFE_DELETE( pWorld );
	}

	return CX2World::WI_NONE;
}

// #ifdef BACKGROUND_LOADING_TEST // 2008-10-18
// CX2World* CX2WorldManager::ThreadRequest_CreateWorld(CX2World::WORLD_ID worldID_, KLuabinder* pKLuabinder_ /*=NULL*/)
// {
//     /** EnableThreadLoading( true )�� �ǹ�:
// 
//         EnableThreadLoading( true )�� �����Ǹ�, CreateWorld()���ο��� �о� ���� lua ��ũ��Ʈ��
//         �޽� �ε�� ���� �ε尡 �ƴ϶�, ������ ť���Ѵ�.
//         - jintaeks on 2008-10-24, 10:38 */
// 
//     ASSERT( g_pKTDXApp );
//     g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );
//     CX2World*   pWorld = CreateWorld( worldID_, pKLuabinder_ );
//     ASSERT( pWorld );
//     g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );
// 
//     return  pWorld;
// }//CX2WorldManager::ThreadRequest_CreateWorld()
// #endif // BACKGROUND_LOADING_TEST // 2008-10-18


void CX2WorldManager::Debug_RenderHelp( int ix_, int iy_ )
{
    int     ix = ix_ + 16;
    int     iy = iy_ + 16;
    WCHAR   wbuffer[1024];

    if ( m_pX2WorldLatestRef )
    {
        const CX2World::WorldData*  pWorldData = m_pX2WorldLatestRef->GetWorldData();
        if ( pWorldData )
        {
            StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer),
                L"WorldID = %i\n"
                L"WorldName = %s\n"
                L"DataFileName = %s\n",
                pWorldData->worldID,
                pWorldData->worldName.c_str(),
                pWorldData->dataFileName.c_str() );
	        g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
		        CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
            iy += 16 * 3;
        }//if
    }//if
}//CX2WorldManager::Debug_RenderHelp()

