// X2KomFileViewer.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2KomFileViewerDlg.h"
#include "PathDialog.h"
#include "NoticeDlg.h"
#include "SameFileDlg.h"
#include "KomListViewDlg.h"
#include <stdlib.h>
#include <strsafe.h>

using std::wifstream;

//#include <vld.h>
//#define _MODIFY_VERSION_01
#define _MODIFY_CONSOLE_VERSION_01

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


lua_State* g_pLua	= NULL;


// CX2KomFileViewerApp

BEGIN_MESSAGE_MAP(CX2KomFileViewerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

static HWND g_noticeHwnd;

// notice thread
UINT WINAPI NoticeThread( LPVOID arg )
{
	CNoticeDlg noticeDlg;	
	g_noticeHwnd = noticeDlg.m_hWnd;
	noticeDlg.DoModal();
	return 0;
}

// CX2KomFileViewerApp ����

CX2KomFileViewerApp::CX2KomFileViewerApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CX2KomFileViewerApp ��ü�Դϴ�.

CX2KomFileViewerApp theApp;


// CX2KomFileViewerApp �ʱ�ȭ

BOOL CX2KomFileViewerApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
	InitCommonControls();

	CWinApp::InitInstance();

	//lua init
	{
		g_pLua = lua_open();

		if( g_pLua == NULL )
		{
			::MessageBox( NULL, L"��� ���� ����.!", L"", MB_OK );
			return FALSE;
		}

		//luaopen_base( g_pLua );  // ����Լ� tonumber�� ����ؾ� �Ѵ�.
		//luaopen_table( g_pLua );
		//luaopen_io( g_pLua );
		//luaopen_string( g_pLua );
		//luaopen_math( g_pLua );
		//lua_settop( g_pLua, 0 );
        luaL_openlibs( g_pLua );
        lua_settop( g_pLua, 0 );

		//lua_tinker::lua_state::open( g_pLua );
	}

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	// load lua script
	theApp.m_komFileManager.LoadConfig();

	//theApp.m_komFileManager.InitNewFolderPaths_Lua();//( L"D:\X2Project\X2Project\X2KomFileViewer" );
	// Console Mode
	if( CommandLineExecute() == true )
	{
		lua_close( g_pLua );
		return FALSE;
	}
#ifdef _MODIFY_VERSION_01

	BOOL	bIsOpenKomFileList	= FALSE;
	BOOL	bIsOpenFolderList	= FALSE;
	BOOL	bIsKomParsingOnly	= FALSE;
	HANDLE	hNoticeThread		= 0;
	CPathDialog pathDlg;

	std::map< std::wstring, MemberFileInfo > mapLuaFiles; // �����ϵ� ��ũ��Ʈ ���� ����Ʈ	
	std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;	// �ߺ� ���� ����Ʈ

	if( pathDlg.DoModal() != IDOK )
	{
		lua_close( g_pLua );
		return FALSE;
	}

	if ( pathDlg.GetOldPath().empty()  == FALSE )				// ���� ������ ���� ��ΰ� �Է� ������
	{
		bIsOpenKomFileList	= TRUE;
		theApp.m_komFileManager.m_vecKomFileList.resize(0);
		theApp.m_komFileManager.m_vecKomFileList.push_back( pathDlg.GetOldPath() );
	}

	if ( pathDlg.GetNewPath().empty() == FALSE )				// ���ο� ������ ���� ��ΰ� �Է� ������
	{
		bIsOpenFolderList	= TRUE;
		theApp.m_komFileManager.m_vecFolderList.resize(0);
		theApp.m_komFileManager.m_vecFolderList.push_back( pathDlg.GetNewPath() );
	}

	if ( bIsOpenFolderList == FALSE && bIsOpenKomFileList == FALSE )			// �Ѵ� ������ ���� ���
	{
		MessageBox( NULL, L"��θ� �Է����� �ʾҽ��ϴ�!", L"����!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}
	else if ( bIsOpenFolderList == FALSE && bIsOpenKomFileList == TRUE )		// kom���ϸ� ���������(��������)
	{
		bIsKomParsingOnly = TRUE;
		theApp.m_komFileManager.GetKomFileListFromLuaScript();
	}
	else if ( bIsOpenFolderList == TRUE && bIsOpenKomFileList == FALSE )		// ������ ���������(��������)
	{
		hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread, (LPVOID)NULL, NULL, NULL );

		// �ߺ� ���� üũ
		if( theApp.m_komFileManager.CheckSameFiles( mapSameFiles ) == false )
		{
			MessageBox( NULL, L"���� ���� üũ ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		// ���� ���� ����Ʈ ����
		if( mapSameFiles.empty() == false )
		{
			CSameFileDlg sameFileDlg;
			sameFileDlg.SetSameFileList( mapSameFiles );
			sameFileDlg.DoModal();
		}
		theApp.m_komFileManager.GetFolderToCompressListFromLuaScript();
	}
	else																		// �Ѵ� ������ ���
	{
		hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread, (LPVOID)NULL, NULL, NULL );

		// �ߺ� ���� üũ
		if( theApp.m_komFileManager.CheckSameFiles(  mapSameFiles ) == false )
		{
			MessageBox( NULL, L"���� ���� üũ ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		// ���� ���� ����Ʈ ����
		if( mapSameFiles.empty() == false )
		{
			CSameFileDlg sameFileDlg;
			sameFileDlg.SetSameFileList( mapSameFiles );
			sameFileDlg.DoModal();
		}

		theApp.m_komFileManager.GetKomFileAndFolderToCompressListFromLuaScript();
	}

	// ��Ȳ�˸� ��ȭ���� ����
	if ( hNoticeThread )
	{
		::SendMessage( g_noticeHwnd, WM_CLOSE, 0, 0 );
		TerminateThread( hNoticeThread, 0 );
	}

	CX2KomFileViewerDlg dlg;
	dlg.SetKomParsingOnly( bIsKomParsingOnly );
	m_pMainWnd = &dlg;
	dlg.DoModal();

	lua_close( g_pLua );
	return FALSE;

#endif

#ifndef _MODIFY_VERSION_01
	// ��� ��� ��ȭ���� ����
	CPathDialog pathDlg;
	if( pathDlg.DoModal() != IDOK )
	{
		lua_close( g_pLua );
		return FALSE;
	}

	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == true )	// ������, �Ź��� ���� ��� ��� ���� �ʾ��� ��
	{
		MessageBox( NULL, L"��θ� �Է����� �ʾҽ��ϴ�!", L"����!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == true )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFile( pathDlg.GetOldPath() ) == false )
		{
			MessageBox( NULL, L"KOM���� �м� ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		// ����Ʈ ��ȭ����
		CX2KomFileViewerDlg dlg;
		dlg.SetKomParsingOnly( true );
		m_pMainWnd = &dlg;
		dlg.DoModal();

		lua_close( g_pLua );
		return FALSE;
	}

	// ��Ȳ�˸� ������ ����
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread, (LPVOID)NULL, NULL, NULL );
		
	std::map< std::wstring, MemberFileInfo > mapLuaFiles; // �����ϵ� ��ũ��Ʈ ���� ����Ʈ	
	std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;	// �ߺ� ���� ����Ʈ

	// �ߺ� ���� üũ
	if( theApp.m_komFileManager.CheckSameFiles( pathDlg.GetNewPath(), mapSameFiles ) == false )
	{
		MessageBox( NULL, L"���� ���� üũ ����!", L"����!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}

	// ���� ���� ����Ʈ ����
	if( mapSameFiles.empty() == false )
	{
		CSameFileDlg sameFileDlg;
		sameFileDlg.SetSameFileList( mapSameFiles );
		sameFileDlg.DoModal();
	}	
	
	// Lua Script Compile
	if( pathDlg.GetLuaScriptCompileFlag() == true )
	{			
		if( theApp.m_komFileManager.CompileLuaScript( pathDlg.GetNewPath(), mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Lua Script Compile ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

	// Lua Script Encrypt
	if( pathDlg.GetLuaScriptCryptFlag() == true )
	{
		if( theApp.m_komFileManager.PrepareTextFileEncrypt( pathDlg.GetNewPath(), mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Text File ��ȣȭ �غ� ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		if( theApp.m_komFileManager.EncryptLuaScript( mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Script File ��ȣȭ ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

	// X-File Convert
	if( pathDlg.GetXFileConvertFlag() == true )
	{
		if( theApp.m_komFileManager.Convert_X_File( pathDlg.GetNewPath() ) == false )
		{
			MessageBox( NULL, L"X-File Convert ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

	// Kom ���� �Ľ� & ��� ���� �ε�
	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == false )
	{
		// new member file obtain
		if( theApp.m_komFileManager.ParsingMemberFile( pathDlg.GetNewPath(), mapLuaFiles, pathDlg.GetLuaScriptCompileFlag(), pathDlg.GetLuaScriptCryptFlag() ) == false )
		{
			MessageBox( NULL, L"member���� �м� ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == false )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( pathDlg.GetOldPath(), pathDlg.GetNewPath(), mapLuaFiles, pathDlg.GetLuaScriptCompileFlag(), pathDlg.GetLuaScriptCryptFlag() ) == false )
		{
			MessageBox( NULL, L"member���� �м� ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}	

	// ��� ���� ����
	if( theApp.m_komFileManager.ReorganizeMemberFile() == false )
	{
		MessageBox( NULL, L"member���� ���� ����!", L"����!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}

	// ��Ȳ�˸� ��ȭ���� ����
	::SendMessage( g_noticeHwnd, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );

	// ����Ʈ ��ȭ����
	CX2KomFileViewerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ���α׷� ����� �����ϵ� Lua Script�� �ִٸ� �����Ѵ�
	theApp.m_komFileManager.RemoveCompiledLuaScript( mapLuaFiles );

	// ���α׷� ����� Convert�� X-File�� �ִٸ� Restore�Ѵ�
	if( pathDlg.GetXFileConvertFlag() == true )
	{
		if( theApp.m_komFileManager.Convert_X_File( pathDlg.GetNewPath(), true ) == false )
		{
			MessageBox( NULL, L"X-File Convert Restore ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
#endif
	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�
	// ���� ���α׷��� ���� �� �ֵ��� FALSE�� ��ȯ�մϴ�.
	lua_close( g_pLua );
	return FALSE;
}

bool CX2KomFileViewerApp::CommandLineExecute()
{
	// �ڵ� kom ����
	int iArgc = __argc;
	WCHAR** pcArgv = __wargv;

	if( iArgc <= 4 ) // ������� 4���̻� : ExeFileName, OldPath, NewPath, SavePath
		return false;
	
	KLuaManager luaManager( g_pLua );

	std::wstring wstrOldPath    = pcArgv[1];	// L"C:\\oldFolder";
	std::wstring wstrNewPath    = pcArgv[2];	// L"C:\\newFolder\\Script\\Dialog";
	std::wstring wstrSavePath   = pcArgv[3];	// L"C:\\saveFolder";

	// ��� ���� �ֱ�
	std::set< std::wstring > setArgv;
	for( int i=4; i < iArgc; i++ )
		setArgv.insert( std::wstring( pcArgv[i] ) );
#ifdef _MODIFY_CONSOLE_VERSION_01

	//{{ kimhc	// �� ��ġ�� �´� changelist version ���� �о�� �� �ش� ���� ���ķ�
	////////////// ������Ʈ �� ���� list ������
	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";

	wifstream	inVersionFile;
	int			iVersion;

	if ( setArgv.find( std::wstring( L"-internal" ) ) != setArgv.end() )
	{
		std::wstring	wstrVersionPath = L"";
		std::string		strVersionPath = "";

		_wsplitpath( pcArgv[0], drive, dir, NULL, NULL );
		wstrVersionPath += drive;
		wstrVersionPath += dir;
		wstrVersionPath += L"internal_changelist_ver.txt";
		ConvertWCHARToChar( strVersionPath, wstrVersionPath.c_str() );

		inVersionFile.open( strVersionPath.c_str(), wifstream::in );
	}

	if ( setArgv.find( std::wstring( L"-opentest" ) ) != setArgv.end() )
	{
		std::wstring	wstrVersionPath = L"";
		std::string		strVersionPath = "";

		_wsplitpath( pcArgv[0], drive, dir, NULL, NULL );
		wstrVersionPath += drive;
		wstrVersionPath += dir;
		wstrVersionPath += L"opentest_changelist_ver.txt";
		ConvertWCHARToChar( strVersionPath, wstrVersionPath.c_str() );

		inVersionFile.open( strVersionPath.c_str(), wifstream::in );
	}

	if ( inVersionFile.is_open() == true )
	{
		WCHAR			skipString[10]				= L"";	// "change " ��� �ܾ ��ŵ
		WCHAR			wstrChangeListVersion[10]	= L"";	// �Ľ̵� iVersion�� ����
		std::wstring	wstrCommand;
		std::string		strCommand;
		const std::wstring& wstrDepotPath = theApp.m_komFileManager.GetDepotPath();;

		inVersionFile >> skipString >> iVersion;
		inVersionFile.close();

		wstrCommand = L"svn  info -r base ";
		wstrCommand += wstrDepotPath;
		wstrCommand += L" | findstr Revision > latest_version.txt";
		ConvertWCHARToChar( strCommand, wstrCommand.c_str() );
		system( strCommand.c_str() );

		//MessageBox( NULL, wstrCommand.c_str(), NULL, MB_OK );

		std::wstring	wstrVersionPath = L"";
		std::string		strVersionPath = "";

		_wsplitpath( pcArgv[0], drive, dir, NULL, NULL );
		wstrVersionPath += drive;
		wstrVersionPath += dir;
		wstrVersionPath += L"latest_version.txt";
		ConvertWCHARToChar( strVersionPath, wstrVersionPath.c_str() );

		//MessageBox( NULL, wstrVersionPath.c_str(), NULL, MB_OK );

		wifstream inNewVersionFile;
		inNewVersionFile.open( strVersionPath.c_str(), wifstream::in );

		if ( inNewVersionFile.is_open() != true )
		{
			ASSERT( false );
			return false;
		}

		int iNewVer;
		
		inNewVersionFile >> skipString >> iNewVer;
		inNewVersionFile.close();

		
		wstrCommand	= L"svn diff -r ";
		_itow( iVersion, wstrChangeListVersion, 10 ) ;
		wstrCommand += wstrChangeListVersion;
		wstrCommand += L":";
		_itow( iNewVer, wstrChangeListVersion, 10 ) ;
		wstrCommand += wstrChangeListVersion;
		wstrCommand += L" ";
		wstrCommand += wstrDepotPath;
		wstrCommand += L" --summarize > updated_files.txt";

		//MessageBox( NULL, wstrCommand.c_str(), NULL, MB_OK );
		//wstrCommand +=

		//wstrCommand += L"p4 diff2 -q //depot/ElswordNew/X2/dat/...@";
		//_itow( iVersion, wstrChangeListVersion, 10 ) ;
		//wstrCommand += wstrChangeListVersion;
		//wstrCommand += L" ";
		//wstrCommand += theApp.m_komFileManager.GetDepotPath();
		//wstrCommand += L"#have";
		//wstrCommand += L"| more > ";	// "l �� or ��ȣ��"
		//wstrCommand += L"updated_files.txt";
		ConvertWCHARToChar( strCommand, wstrCommand.c_str() );
		
		system( strCommand.c_str() );

		// set�� ����� ���� ��ε��� ���
		wifstream	wifUpdatedFiles;

		wifUpdatedFiles.open( "updated_files.txt", wifstream::in );

		if ( wifUpdatedFiles.is_open() == true )
		{
			WCHAR wstrOneLine[MAX_PATH];
			WCHAR wstrFilePath[MAX_PATH];
			WCHAR *pHeadToFilePath			= NULL;
			//WCHAR *pTailToFilePath			= NULL;
			
			std::set< std::wstring > setUpdatedFolders;

			while ( wifUpdatedFiles.good() == true )
			{
				wifUpdatedFiles.getline( wstrOneLine, MAX_PATH );

				if ( StrCmpW( wstrOneLine, L"" ) != 0 )
				{
					pHeadToFilePath = StrStrIW( wstrOneLine, L"dat\\" ) + 4;	// 4�� 'dat/'�� ����
					//pTailToFilePath = StrStrIW( pHeadToFilePath, L"#" );

					//StringCchCopyNW( wstrFilePath, MAX_PATH - 1, pHeadToFilePath, pTailToFilePath - pHeadToFilePath );

					_wsplitpath( pHeadToFilePath, NULL, dir, NULL, NULL );

					if ( setUpdatedFolders.find( std::wstring( dir ) ) == setUpdatedFolders.end() )
						setUpdatedFolders.insert( std::wstring( dir ) );
				}
			}

			WCHAR wstrFolderPath[_MAX_DIR];

			std::set< std::wstring >::iterator sItrUpdatedFolders = setUpdatedFolders.begin();

			for ( ; sItrUpdatedFolders != setUpdatedFolders.end(); sItrUpdatedFolders++ )
			{
				WCHAR*			pHeadToToken	= NULL;
				std::wstring	wstrPath		= wstrNewPath;

				wstrPath += L"\\";

				StringCchCopyW( wstrFolderPath, _MAX_DIR, sItrUpdatedFolders->c_str() );
							
				pHeadToToken = wcstok( wstrFolderPath, L"/" );

				if ( pHeadToToken != NULL )
					wstrPath += pHeadToToken;

				while ( pHeadToToken != NULL )
				{
					pHeadToToken = wcstok( NULL, L"/" );

					if ( pHeadToToken != NULL)
					{
						wstrPath += L"\\";
						wstrPath += pHeadToToken;
					}
				}

				theApp.m_komFileManager.m_vecPathListToCompress.push_back( wstrPath );
				
			}
		}
	}

	//{{ hyunchul // 2009-03-31, 14:02
	// theApp.m_komfileManager.m_vecKomFileList�� wstrOldPath�� �ִ´�.
	// InitNewFolderPaths.lua �� ���� NewFolder���� ��θ� theApp.m_komFileManager.m_vecFolderList�� �о���δ�.
	theApp.m_komFileManager.m_vecKomFileList.push_back( wstrOldPath );

	std::vector< std::wstring > vecNewFolderList = theApp.m_komFileManager.GetNewFolderList();
	theApp.m_komFileManager.m_vecFolderList.resize(0);
	std::vector< std::wstring >::iterator vItr = vecNewFolderList.begin();
	std::wstring wstrTempPath;
	
	for ( ; vItr != vecNewFolderList.end(); vItr++ )
	{
		wstrTempPath = wstrNewPath;
		wstrTempPath += L"\\";
		wstrTempPath += (*vItr);

		theApp.m_komFileManager.m_vecFolderList.push_back( wstrTempPath );

	}
	//}} hyunchul // 2009-03-31, 14:02


	// �ߺ� ���� üũ(���� ���ϸ�)
	if( setArgv.find( std::wstring( L"-s" ) ) != setArgv.end() )
	{
		std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;
		//if( theApp.m_komFileManager.CheckSameFiles( wstrNewPath, mapSameFiles ) == false )
		//{{ hyunchul // 2009-03-31, 14:50
		if ( theApp.m_komFileManager.CheckSameFiles( mapSameFiles ) == false )
		{
			MessageBox( NULL, L"���� ���� üũ ����!", L"����!", MB_OK );
			return true;
		}
		//}} hyunchul // 2009-03-31, 14:50

		if( mapSameFiles.empty() == false )
		{
			MessageBox( NULL, L"�̸��� �ߺ��� ������ ����!", L"����!", MB_OK );
			return true;
		}
	}

	bool bIsLuaCompile = false;
	bool bIsEncrypt = false;
	std::map< std::wstring, MemberFileInfo > mapLuaFiles;

	//{{ hyunchul // 2009-03-31, 15:11
	// ���� ����: NewPath�� ������ �Է� ���� �� �ְ� �Ǿ����Ƿ� vecNewFolderList�� ���� ��ŭ loop
	// Lua Script Compile
	if( setArgv.find( std::wstring( L"-c" ) ) != setArgv.end() )
	{
		bIsLuaCompile = true;
	
		if( theApp.m_komFileManager.CompileLuaScript( theApp.m_komFileManager.m_vecFolderList, mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Lua Script Compile ����!", L"����!", MB_OK );
			return true;
		}
   

	}


	// Lua Script Encrypt
	if( setArgv.find( std::wstring( L"-e" ) ) != setArgv.end() )
	{
		bIsEncrypt = true;
		if( theApp.m_komFileManager.PrepareTextFileEncrypt( theApp.m_komFileManager.m_vecFolderList, mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Text File ��ȣȭ �غ� ����!", L"����!", MB_OK );
			return true;
		}

		if( theApp.m_komFileManager.EncryptLuaScript( mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Script File ��ȣȭ ����!", L"����!", MB_OK );
			return true;
		}
	}

	// X-File Convert
	if( setArgv.find( std::wstring( L"-x" ) ) != setArgv.end() )
	{
		if( theApp.m_komFileManager.Convert_X_File( theApp.m_komFileManager.m_vecFolderList ) == false )
		{
			MessageBox( NULL, L"X-File Convert ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
	//}} hyunchul // 2009-03-31, 15:11

	// Kom And Member File Parsing or new member file obtain
	if( setArgv.find( std::wstring( L"-k" ) ) != setArgv.end() )
	{
		//if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( wstrOldPath, wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		if ( theApp.m_komFileManager.GetKomFileAndFolderToCompressListFromLuaScript( vecNewFolderList, theApp.m_komFileManager.m_vecKomNameList, bIsLuaCompile, bIsEncrypt, mapLuaFiles ) == FALSE )
		{
			MessageBox( NULL, L"kom & member���� �м� ����!", L"����!", MB_OK );
			return true;
		}		
	}
	else
	{
		//if( theApp.m_komFileManager.ParsingMemberFile( wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		if ( theApp.m_komFileManager.GetFolderToCompressListFromLuaScript( vecNewFolderList, theApp.m_komFileManager.m_vecKomNameList ) == FALSE )
		{
			MessageBox( NULL, L"member���� �м� ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

#endif

#ifndef _MODIFY_CONSOLE_VERSION_01
	
	// �ߺ� ���� üũ
	if( setArgv.find( std::wstring( L"-s" ) ) != setArgv.end() )
	{
		std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;
		if( theApp.m_komFileManager.CheckSameFiles( wstrNewPath, mapSameFiles ) == false )
		{
			MessageBox( NULL, L"���� ���� üũ ����!", L"����!", MB_OK );
			return true;
		}
		
		if( mapSameFiles.empty() == false )
		{
			MessageBox( NULL, L"�̸��� �ߺ��� ������ ����!", L"����!", MB_OK );
			return true;
		}
	}

	bool bIsLuaCompile = false;
	bool bIsEncrypt = false;
	std::map< std::wstring, MemberFileInfo > mapLuaFiles;

	// Lua Script Compile
	if( setArgv.find( std::wstring( L"-c" ) ) != setArgv.end() )
	{
	bIsLuaCompile = true;

	if( theApp.m_komFileManager.CompileLuaScript( wstrNewPath, mapLuaFiles ) == false )
	{
	MessageBox( NULL, L"Lua Script Compile ����!", L"����!", MB_OK );
	return true;
	}
	}

	// Lua Script Encrypt
	if( setArgv.find( std::wstring( L"-e" ) ) != setArgv.end() )
	{
	bIsEncrypt = true;
	if( theApp.m_komFileManager.PrepareTextFileEncrypt( wstrNewPath, mapLuaFiles ) == false )
	{
	MessageBox( NULL, L"Text File ��ȣȭ �غ� ����!", L"����!", MB_OK );
	return true;
	}

	if( theApp.m_komFileManager.EncryptLuaScript( mapLuaFiles ) == false )
	{
	MessageBox( NULL, L"Script File ��ȣȭ ����!", L"����!", MB_OK );
	return true;
	}
	}

	// X-File Convert
	if( setArgv.find( std::wstring( L"-x" ) ) != setArgv.end() )
	{
	if( theApp.m_komFileManager.Convert_X_File( wstrNewPath ) == false )
	{
	MessageBox( NULL, L"X-File Convert ����!", L"����!", MB_OK );
	lua_close( g_pLua );
	return FALSE;
	}
	}

	// Kom And Member File Parsing or new member file obtain
	if( setArgv.find( std::wstring( L"-k" ) ) != setArgv.end() )
	{
		if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( wstrOldPath, wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		{
			MessageBox( NULL, L"kom & member���� �м� ����!", L"����!", MB_OK );
			return true;
		}		
	}
	else
	{
		if( theApp.m_komFileManager.ParsingMemberFile( wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		{
			MessageBox( NULL, L"member���� �м� ����!", L"����!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
	
	// Reorgnize
	if( theApp.m_komFileManager.ReorganizeMemberFile() == false )
	{
	MessageBox( NULL, L"member���� ���� ����!", L"����!", MB_OK );
	return true;
	}
	
#endif

	// Is MultiThread?
	bool bIsMultiThreadPacking = false;
	if( setArgv.find( std::wstring( L"-m" ) ) != setArgv.end() )
	{
		bIsMultiThreadPacking = true;
	}

	// Kom ���� ���� �˷��ֱ�
	if( setArgv.find( std::wstring( L"-t" ) ) != setArgv.end() )
	{
		//{{ hyunchul // 2009-04-02, 10:53
		std::map< std::wstring, std::vector< MemberFileInfo > >& mapTemp = theApp.m_komFileManager.GetMapNewMemberFile();
		CKomListViewDlg komListViewDlg( mapTemp );
		komListViewDlg.DoModal();
		//}} hyunchul // 2009-04-02, 10:53
	}
	else
	{
		if ( theApp.m_komFileManager.m_vecPathListToCopy.empty() == false )		// ������ kom ���ϵ��� �ִٸ�
		{
			//std::vector< std::wstring >::iterator vItrPathListToCopy = theApp.m_komFileManager.m_vecPathListToCopy.begin();
			std::wstring	wstrCommandNotChanged;
			std::wstring	wstrCommand;
			std::string		strCommand;

			wstrCommandNotChanged		= L"copy ";
			wstrCommandNotChanged		+= wstrOldPath;
			wstrCommandNotChanged		+= L"\\";

			for ( UINT i = 0; i < theApp.m_komFileManager.m_vecPathListToCopy.size(); i++ )
			{
				wstrCommand = wstrCommandNotChanged;
				wstrCommand += theApp.m_komFileManager.m_vecPathListToCopy[i];
				wstrCommand += L" ";
				wstrCommand += wstrSavePath;
				wstrCommand += L"\\";
				wstrCommand += theApp.m_komFileManager.m_vecPathListToCopy[i];

				ConvertWCHARToChar( strCommand, wstrCommand.c_str() );

				system( strCommand.c_str() );
				
			}

			// �ֽ� changelist version �ٽ� ���� �ϱ�(����س���)

			const std::wstring& wstrDepotPath = theApp.m_komFileManager.GetDepotPath();

			if ( setArgv.find( std::wstring( L"-internal" ) ) != setArgv.end() )
			{
				wstrCommand = L"svn  info -r base ";
				wstrCommand += wstrDepotPath;
				wstrCommand += L" | findstr Revision > internal_changelist_ver.txt";

				//MessageBox( NULL, wstrCommand.c_str(), NULL, MB_OK );
				
				//wstrCommand = L"p4 changes -m1 ";
				//wstrCommand += theApp.m_komFileManager.GetDepotPath();
				//wstrCommand += L"#have";
				//wstrCommand += L"| more > ";	// "l �� or ��ȣ��"
				//wstrCommand += L"internal_changelist_ver.txt";
			}

			if ( setArgv.find( std::wstring( L"-opentest" ) ) != setArgv.end() )
			{
				wstrCommand = L"svn  info -r base ";
				wstrCommand += wstrDepotPath;
				wstrCommand += L" | findstr Revision > internal_changelist_ver.txt";

				//wstrCommand = L"p4 changes -m1 ";
				//wstrCommand += theApp.m_komFileManager.GetDepotPath();
				//wstrCommand += L"#have";
				//wstrCommand += L"| more > ";	// "l �� or ��ȣ��"
				//wstrCommand += L"opentest_changelist_ver.txt";
			}

			ConvertWCHARToChar( strCommand, wstrCommand.c_str() );

			system( strCommand.c_str() );
		}

		// Create Kom File
		if( theApp.m_komFileManager.CreateKomFile( wstrSavePath, bIsMultiThreadPacking ) == false )
		{
			MessageBox( NULL, L"kom���� ���� ����!", L"����!", MB_OK );
			return true;
		}
	}

	// ���α׷� ����� �����ϵ� Lua Script�� �ִٸ� �����Ѵ�
	theApp.m_komFileManager.RemoveCompiledLuaScript( mapLuaFiles );

	// ���α׷� ����� Convert�� X-File�� �ִٸ� Restore�Ѵ�
	if( setArgv.find( std::wstring( L"-x" ) ) != setArgv.end() )
	{
		if( theApp.m_komFileManager.Convert_X_File( wstrNewPath, true ) == false )
		{
			MessageBox( NULL, L"X-File Convert Restore ����!", L"����!", MB_OK );
			return true;
		}
	}

	// ���� ����
	return true;
}
