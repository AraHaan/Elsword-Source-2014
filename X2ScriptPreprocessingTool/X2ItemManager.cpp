#include <StdAfx.h>





CX2ItemManager::CX2ItemManager()
{
	lua_State* L= g_pKTDXApp->GetLuaBinder()->GetLuaState();
    RegisterProxyLuabind( L );
}


bool	CX2ItemManager::OpenItemScriptFile( const wchar_t* pKimFile, const std::vector<std::wstring>& vecScriptFile )
{
    wchar_t awszError[1024];

    if ( pKimFile == NULL || pKimFile[0] == NULL || vecScriptFile.empty() == true )
    {
        ErrorLogMsg( XEM_ERROR146, "INVALID_PARAMETER" );
        return false;
    }

    bool    bOK = false;
    KFileSerializer kimFile;
    unsigned uNumScriptFiles = vecScriptFile.size();
    KFileSerializer* pScriptFile = new KFileSerializer[uNumScriptFiles];

    HRESULT hr;
    bool    bKinFileLoaded = false;

    for ( unsigned u = 0; u != uNumScriptFiles; ++u )
    {
        if ( vecScriptFile[u].empty() == true )
        {
            ErrorLogMsg( XEM_ERROR146, "INVALID_PARAMETER" );
            goto out;
        }
        hr = pScriptFile[u].LoadFile( vecScriptFile[u].c_str() );
        if ( FAILED( hr ) )
        {
            swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot open: -in \"%s\"", vecScriptFile[u].c_str() );
            ErrorLogMsg( XEM_ERROR146, awszError );
            goto out;
        }
    }//for
    hr = kimFile.LoadFile( pKimFile );
    if ( SUCCEEDED( hr ) )
    {
        bKinFileLoaded = true;
    }

    {
		// ������ // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING�ּ� // ���� ������ ������ �������� �ۼ��� �ؼ� Ʋ�� ���� �� �� �ֽ��ϴ�.
		// KProxy�� �Ľ��ϴ� ù ��° �۾��� �������� ���� �ľ��� ���� �Ľ�
		// �ش� ������ ID��, �ش� �������� ��ȿ���� üũ�ϱ� ���� ������ �˻� ��
		// Item.lua  -> std::set<DWORD> m_setItemIDs �����̳ʿ� ItemID�� ��� ��.
		// SetItem.lua -> std::set<DWORD> m_setSetIDs �����̳ʿ� ItemID�� ��� ��.
	    KProxy	kProxy;

	    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pItemManager", &kProxy );
	    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SetItemManager", &kProxy );

        for( unsigned u = 0; u != uNumScriptFiles; ++u )
        {
            DWORD   dwItemFiletSize = pScriptFile[u].GetSize();
            const void* pItemFile = pScriptFile[u].AccessData( 0, dwItemFiletSize );
	        if( g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( (const char*)pItemFile, dwItemFiletSize ) == false )
            {
                swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot exec: -in \"%s\"", vecScriptFile[u].c_str() );
                ErrorLogMsg( XEM_ERROR146, awszError );
		        goto out;
            }
        }

        {
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
			KProxy2 kProxy2( kProxy.m_setItemIDs, kProxy.m_setSetIDs, kProxy.m_mapItemTrans, kProxy.m_mapSetItemTrans );
#else //SERV_ITEM_LUA_TRANS_DEVIDE
            KProxy2 kProxy2( kProxy.m_setItemIDs, kProxy.m_setSetIDs );
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

	        lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pItemManager", &kProxy2 );
	        lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SetItemManager", &kProxy2 );

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
			if( uNumScriptFiles != 2)
				uNumScriptFiles = 2; // ItemTrans.lua, SetItemTrans.lua �� KProxy2���� ���� �ʽ��ϴ�
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

            for( unsigned u = 0; u != uNumScriptFiles; ++u )
            {
                DWORD   dwItemFiletSize = pScriptFile[u].GetSize();
                const void* pItemFile = pScriptFile[u].AccessData( 0, dwItemFiletSize );
	            if( g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( (const char*)pItemFile, dwItemFiletSize ) == false )
                {
                    swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot exec: -in \"%s\"", vecScriptFile[u].c_str() );
                    ErrorLogMsg( XEM_ERROR146, awszError );
		            goto out;
                }
            }

            kProxy2.PostProcess();

            if ( kProxy2.m_kFileSerializer.GetSize() < sizeof(CX2Item::KItemFormatHeader) )
            {
                ErrorLogMsg( XEM_ERROR146, "cannot generate preprocessed info" );
                goto out;
            }

            if ( bKinFileLoaded == true )
            {
                bool    bDiffer = false;
                DWORD   dwKimFileSize = kimFile.GetSize();
                if ( dwKimFileSize != kProxy2.m_kFileSerializer.GetSize() )
                    bDiffer = true;
                else
                {
                    const void* pKimFile = kimFile.AccessData( 0, dwKimFileSize );
                    const void* pProxy2File = kProxy2.m_kFileSerializer.AccessData( 0, dwKimFileSize );
                    int iRet = memcmp( pKimFile, pProxy2File, dwKimFileSize );
                    if ( iRet != 0 )
                        bDiffer = true;
                }
                if ( bDiffer == false )
                {
                    bOK = true;
                    goto out;
                }
                BOOL bRet = ::DeleteFileW( pKimFile );
                if ( bRet == 0 )
                {
                    swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot delete old file: -exec \"%s\"", pKimFile );
                    ErrorLogMsg( XEM_ERROR146, awszError );
                    goto out;
                }
            }

            hr = kProxy2.m_kFileSerializer.SaveFile( pKimFile );
            if ( FAILED( hr ) )
            {
                swprintf_s( awszError, ARRAY_SIZE(awszError), L"cannot generate: -exec \"%s\"", pKimFile );
                ErrorLogMsg( XEM_ERROR146, awszError );
                goto out;
            }
            bOK = true;
        }
    }
out:

    delete[]    pScriptFile;

	return bOK;
}