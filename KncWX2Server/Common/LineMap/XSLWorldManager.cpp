#include "baseserver.h"
#include "X2Data/XSLStat.h"
#include ".\xslworldmanager.h"


#ifdef KHELP_SYSTEM_TEST // 2008-10-24
IMPLEMENT_HELP_SYSTEM( CXSLWorldManager )
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
}//IMPLEMENT_HELP_SYSTEM( CXSLWorldManager )
#endif // KHELP_SYSTEM_TEST // 2008-10-24


///-----------------------------------------------------------------------------
//
// constructor.
CXSLWorldManager::CXSLWorldManager()
{
	::InitializeCriticalSection( &m_csWorldManager );
    m_bWritingPreprocessingData = false;
    //m_strPreprocessingDataFile;
    m_pX2WorldLatestRef         = NULL;
}//CXSLWorldManager::CXSLWorldManager()


// destructor.
CXSLWorldManager::~CXSLWorldManager(void)
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
}//CXSLWorldManager::~CXSLWorldManager()


CX2World* CXSLWorldManager::CreateWorld( CX2World::WORLD_ID worldID, KLuabinder* pKLuabinder /* = NULL */ )
{
	CSLock locker( m_csWorldManager );

	for( int i = 0; i < (int)m_WorldDataList.size(); i++ )
	{
		CX2World::WorldData* pWorldData = m_WorldDataList[i];
		if( pWorldData->worldID == worldID )
		{
            //{{ seojt // 2008-10-21, 15:43
            // set preprocessing data filename
            // ������ ���� ��ü�� ��ũ��Ʈ ���Ͽ� �����ϴ� ��ó�� ���� �̸��� ������ �д�.
            // ��׶��� �ε�ÿ� ��ó�� ������ ������ �о ó���ϱ� ���� ����Ѵ�.
            // - jintaeks on 2008-10-22, 10:38
            //
            if ( m_bWritingPreprocessingData )
            {
	            ConvertWCHARToChar( m_strPreprocessingDataFile, pWorldData->dataFileName.c_str() );
                m_strPreprocessingDataFile += ".ppd"; // preprocessing data
            }//block
            //}} seojt // 2008-10-21, 15:43

			CX2World* pCX2World = new CX2World( pWorldData );
            ASSERT( pCX2World );

            m_pX2WorldLatestRef = pCX2World; // ���� �ֱٿ� ������ ���� ��ü�� ���� ���� �����͸� ������ �д�.

			pCX2World->OpenScriptFile( pWorldData->dataFileName.c_str(), pKLuabinder );

			return pCX2World;
		}//if
	}//for

	return NULL;
}//CXSLWorldManager::CreateWorld()

bool CXSLWorldManager::OpenScriptFile( const WCHAR* pFileName )
{
    ASSERT( pFileName );
    CSLock locker( m_csWorldManager );

	//���δ��� ���
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pWorldManager", this );

	//���� �ε�
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR0, strFileName.c_str() );

		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR72, strFileName.c_str() );

		return false;
	}

	return true;
}


//{{ seojt // 2008-10-22, 10:33
void CXSLWorldManager::OnPreDestroyWorld(CX2World* pWorld_)
{
    ASSERT( pWorld_ );
    /** � ���� ��ü�� �ı��Ǹ�, ���� �ֱٿ� ������ ���� ��ü�� ���� ���� �����͸� ��ȿȭ��Ų��.
        CXSLWorldManager�� ��ü�� �����ϴ� ���� ���������, ��ü�� �ı��ϴ� ���� �ð� ����
        �����Ƿ�, �� �κ��� �����ؾ� �� ���̴�. - jintaeks on 2008-10-22, 10:30
    
        @���� ��:
        
            step 1) CXSLWorldManager�� �ڽ��� ������ ��� ���� ���� ��ü�� ����Ʈ�� �����Ѵ�.

            step 2) CX2World�� ���� delete���� �ʴ� �ٴ� ���� �����ϱ� ���� CX2World�� �ı���
                    ~CX2World()�� private�� �����ϰ�, CXSLWorldManager�� CX2World ��ü�� �ı��ϴ�
                    ���� �Լ��� �����Ѵ�. CXSLWorldManager�� CX2World�� private �ı��ڿ� ������ ��
                    �־�� �ϹǷ�, CX2World���� friend�� ����Ǿ�� �� ���̴�.

            step 3) �� ���� ������Ʈ�� CX2World�� ���� �ı����� �ʰ� - ���� ���� SAFE_DELETE()�� ȣ���Ѵ� -
                    CXSLWorldManager���� CX2World::WORLD_ID�� �ش��ϴ� ���带 �ı��� ���� ��û�Ѵ�.

            ���) CXSLWorldManager�� ��ü�� �����ϴ� factory�� ������ �ϹǷ�, ��ü�� �ı��� ������ �����Ѽ�
                    ������ factory�� �ǵ��� �ؾ� �� ���̴�.

        - jintaeks on 2008-10-22, 10:26 */

    m_pX2WorldLatestRef = NULL;

    // ���� ���忡�� ��û�� ���ҽ� ������ ��� clear�Ѵ�.
    ASSERT( g_pKTDXApp && g_pKTDXApp->GetDeviceManager() );
    g_pKTDXApp->GetDeviceManager()->ClearResPendingQueue();
}//CXSLWorldManager::OnPreDestroyWorld()
//}} seojt // 2008-10-22, 10:33



const CX2World::WorldData* CXSLWorldManager::GetWorldData( CX2World::WORLD_ID eWorldID )
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


bool CXSLWorldManager::AddWorldData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	CX2World::WorldData* pWorldData = new CX2World::WorldData;
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"worldID",			pWorldData->worldID,		CX2World::WORLD_ID,	CX2World::WI_NONE,	SAFE_DELETE(pWorldData); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"worldName",		pWorldData->worldName,		L"",										SAFE_DELETE(pWorldData); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"dataFileName",	pWorldData->dataFileName,	L"",										SAFE_DELETE(pWorldData); return false; );

	m_WorldDataList.push_back( pWorldData );
	return true;
}


//{{ seojt // 2008-10-21, 15:14
bool CXSLWorldManager::SetPreprocessingData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	WORLD_PREPROCESSING_INFO    preInfo;

    // qfff
    preInfo.m_eInfoType = X2WM_PPIT_UNKNOWN;
	LUA_GET_VALUE( luaManager, L"meshName",             preInfo.m_wstrName,         L"" );
	//LUA_GET_VALUE( luaManager, L"centerPos",		    preInfo.m_vCenter,          D3DXVECTOR3(0,0,0) );
	LUA_GET_VALUE( luaManager, L"boundingSphereRadius",	preInfo.m_fBSphereRadiua,  0.f );
    preInfo.m_vCenter = lua_tinker::get<D3DXVECTOR3>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "centerPos" );


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
}//CXSLWorldManager::SetPreprocessingData_LUA()
//}} seojt // 2008-10-21, 15:14


CX2World::WORLD_ID CXSLWorldManager::GetWorldIDUsingLineMapFileName( const WCHAR* pLineMapFileName )
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


void CXSLWorldManager::EnableWritingPreprocessingData(bool bEnable_)
{
    m_bWritingPreprocessingData = bEnable_;
}//CXSLWorldManager::EnableWritingPreprocessingData()


bool CXSLWorldManager::WritePreprocessingData(const WORLD_PREPROCESSING_INFO& preInfo_)
{
    ASSERT( m_bWritingPreprocessingData );

    FILE*   fp = fopen( m_strPreprocessingDataFile.c_str(), "ab" );
    if ( fp )
    {
        char    buffer[1024];

        StringCchCopyA( buffer, ARRAY_SIZE(buffer), "g_pWorldManager:SetPreprocessingData\r\n{\r\n" );
        fprintf( fp, buffer );

        std::string     strName;
        ConvertWCHARToChar( strName, preInfo_.m_wstrName.c_str() );
        StringCchPrintfA( buffer, ARRAY_SIZE(buffer), "    meshName = \"%s\",\r\n", strName.c_str() );
        fprintf( fp, buffer );

        StringCchPrintfA( buffer, ARRAY_SIZE(buffer), "    \"centerPos = D3DXVECTOR3(%g,%g,%g)\",\r\n",
            preInfo_.m_vCenter.x, preInfo_.m_vCenter.y, preInfo_.m_vCenter.z );
        fprintf( fp, buffer );
        StringCchPrintfA( buffer, ARRAY_SIZE(buffer), "    boundingSphereRadius = %g,\r\n}\r\n\r\n",
            preInfo_.m_fBSphereRadiua );
        fprintf( fp, buffer );

        fclose( fp );
        return true;
    }//if

    return false;
}//CXSLWorldManager::WritePreprocessingData()


#ifdef BACKGROUND_LOADING_TEST // 2008-10-18
CX2World* CXSLWorldManager::ThreadRequest_CreateWorld(CX2World::WORLD_ID worldID_, KLuabinder* pKLuabinder_ /*=NULL*/)
{
    /** EnableThreadLoading( true )�� �ǹ�:

        EnableThreadLoading( true )�� �����Ǹ�, CreateWorld()���ο��� �о� ���� lua ��ũ��Ʈ��
        �޽� �ε�� ���� �ε尡 �ƴ϶�, ������ ť���Ѵ�.
        - jintaeks on 2008-10-24, 10:38 */

    ASSERT( g_pKTDXApp );
    g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );
    CX2World*   pWorld = CreateWorld( worldID_, pKLuabinder_ );
    ASSERT( pWorld );
    g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );

    return  pWorld;
}//CXSLWorldManager::ThreadRequest_CreateWorld()
#endif // BACKGROUND_LOADING_TEST // 2008-10-18


void CXSLWorldManager::Debug_RenderHelp( int ix_, int iy_ )
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
}//CXSLWorldManager::Debug_RenderHelp()

