#include "Patcher.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <io.h>

//#define SKIP_PATCH // ��ȸ������ ����� �� (��ġ ������ ��ŵ�Ѵ�.)


#define MAX_RETRY           30                          // �ִ� ��õ� Ƚ��
#define MAX_DELAY           500                         // ��õ� ���� ����

// Variable declarations.
KPatcher*	g_pPatcher          = NULL;  ///< Patcher instance.
bool		g_bTestMode  = true; ///< �系 �׽�Ʈ ����ΰ�?



#ifdef DMLEE_PATCHER_TEST
extern bool g_bPatchLaunchButtonEnabled;
#endif // DMLEE_PATCHER_TEST

// Function declarations.
void Log( const char* szLogMsg );
void LogError( const char* szLogMsg );


/// Default constructor.
/// @param hWnd handle to download window.
KPatcher::KPatcher( HWND hWnd )
{
    m_hWnd       = hWnd;
    m_bIsPatched = false;
    m_dwByteFile = 0;
    m_dwRateDown = 0;

	m_dwCurDownAllFileSize = 0;
	m_dwDownAllFileSize = 0;

	m_strProgramPath = "";
}

/// Default destructor.
KPatcher::~KPatcher( void )
{
    // empty
}

void KPatcher::AddCurDownAllFileSize( DWORD dwSize ) 
{ 
	m_dwCurDownAllFileSize += dwSize;
}

/// Create directory.
/// @param szPath_ ������ ���丮 ���
/// @return �����ϸ� true, �����ϸ� false
bool KPatcher::CreateDirectory( const char* szPath_ ) const // by ctkim
{
    char* szToken   = NULL;
    char* szToken2  = NULL;
    char szDirName[20][64];

    int len = (int)strlen(szPath_);
    int ct = 0;
    int numDir = -1;
    while( ct < len )
    {
        if( szPath_[ct] == '\\' )
        {
            // ���� ó���� �߰ߵǸ� ����
            if( numDir == -1 )
            {
                numDir++;
            }
            else
            {
                memcpy( szDirName[numDir], szPath_, (ct+1) );
                szDirName[numDir][ct+1] = '\0';
                numDir++;
            } // else
        } // if
        ct++;
    } // while

    // Create a new directory.
    ct = 0;
    while( ct < numDir )
    {
        ::CreateDirectoryA( szDirName[ct], NULL );
        ct++;
    } // while
    return true;
} // KPatcher::CreateDirectory()

/// �־��� �ҽ� ��ηκ��� ���� ��θ� ������ �����̸��� ��´�.
/// @param szDest output parameter
/// @param szSrc source path
/// @return void
void KPatcher::ExceptFolder( char* szDest, const char* szSrc ) const
{
    for ( int iDestFeed = 0, iSrcFeed = 0;
          NULL != szSrc[iSrcFeed];
          iDestFeed++, iSrcFeed++ )
    {
        szDest[iDestFeed] = szSrc[iSrcFeed];
        if ( '\\' == szSrc[iSrcFeed] ) iDestFeed = -1;
    }
    szDest[iDestFeed] = '\0';
}

/// ��ġ��� ���Ϸκ��� ���� ������ ��ġ ��θ� �д´�.
/// @param szSrcPath output parameter
/// @return �����ϸ� true, �����ϸ� false
bool KPatcher::ReadPatchPathFile( char* szSrcPath ) const
{
    HANDLE hFile    = NULL;
    DWORD  dwRead   = 0;
    BYTE   curByte  = 0;
    int    iFeed    = -1;
    bool   bSuccess = false;

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, PATCH_PATH_FILE, -1, wcharBuf, 255 );
	
    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFile( wcharBuf,
                                                         GENERIC_READ,
                                                         0,
                                                         NULL,
                                                         OPEN_EXISTING,
                                                         FILE_ATTRIBUTE_NORMAL,
                                                         NULL ) ) )
    {
        return false;
    }

    while ( FALSE != ::ReadFile( hFile, &curByte, 1, &dwRead, NULL ) )
    {
        if ( 0 == dwRead ) break;

        switch ( curByte )
        {
            case '\n': case '\t': case ' ': case '\0':                 // skip
                break;
            case '<': if (-1 == iFeed) iFeed = 0; else goto Exit;      // begin
                break;
            case '>': if (-1 != iFeed) { bSuccess = true; goto Exit; } // end
                break;
            default:  if (-1 != iFeed) szSrcPath[iFeed++] = curByte;   // read
                break;
        }
    }

Exit:
    ::CloseHandle( hFile );
    return bSuccess;
}

/// ��ġ��� ���Ϸκ��� ���� ������ ��ġ ��θ� �д´�.
/// @param strSrcPath output parameter
/// @return �����ϸ� true, �����ϸ� false
bool KPatcher::ReadPatchPathFile( std::string& strSrcPath ) const
{
    char szSrcPath[MAX_PATH] = { 0, };
    bool bRet  = ReadPatchPathFile( szSrcPath );
    strSrcPath = szSrcPath;
    return bRet;
}

/// ������ �ݵ�� �����.
/// @param szFileName ���� �̸�
/// @return void
void KPatcher::DeleteFileAbsolutely( const char* szFileName ) const
{
    DWORD dwAtt;
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wcharBuf, 255 );

    if ( 0xffffffff != ( dwAtt = ::GetFileAttributes( wcharBuf ) ) )
    {
        ::SetFileAttributes( wcharBuf, dwAtt & (~FILE_ATTRIBUTE_READONLY) );
    }
    ::DeleteFile( wcharBuf );
}

/// ������ ��� ����(Ver.xml)�� �Ź��� ��� ����(NewVer.xml)�� �����Ѵ�.
/// @param void
/// @return �����ϸ� true, �����ϸ� false
bool KPatcher::UpdateVersionFile( void ) const
{
    // NewVer.xml�� name�� Ver.xml�� ����.

    DeleteFileAbsolutely( "Ver.xml" );

    if ( FALSE == ::MoveFile( L"NewVer.xml", L"Ver.xml" ) )
    {
        Log( "Ver.xml ���� ������Ʈ�� �����߽��ϴ�." );


        KMsgBox::Open( "���� ���� ������Ʈ ����",
                       "�ٽ� �õ��� �ֽʽÿ�.",
                       KMsgBox::MBOX_OK );

        return false;
    }
    Log( "Ver.xml ���� ������Ʈ�� �����߽��ϴ�." );

    return true;
}

/// �ٿ�ε� â�� ���ų� �ݴ´�.
/// @param bOpen �� ������ ���� ������ ����
/// @return void
void KPatcher::OpenDownloadWindow( bool bOpen ) const
{
    if ( true == bOpen )
    {
        // �ٿ�ε� â�� ����.
        ::ShowWindow( m_hWnd, SW_SHOW );
        ::SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        ::SetFocus( m_hWnd );
    }
    else
    {
        // �ٿ�ε� â�� �����.
        ::SetFocus( NULL );
        ::ShowWindow( m_hWnd, SW_HIDE );
    }
}

/// ���ڿ� ���� '\\'�� �ִ��� Ȯ���Ͽ� ������ �߰��Ѵ�.
/// @param str output parameter
/// @return void
void KPatcher::AppendBackSlash( std::string& str ) const
{
    if ( '\\' != str[str.size() - 1] )
    {
        str.append( "\\" );
    }
}

/// ���ڿ� ���� '/'�� �ִ��� Ȯ���Ͽ� ������ �߰��Ѵ�.
/// @param str output parameter
/// @return void
void KPatcher::AppendSlash( std::string& str ) const
{
    if ( '/' != str[str.size() - 1] )
    {
        str.append( "/" );
    }
}

/// ���� Ŭ���̾�Ʈ ���α׷��� üũ�Ͽ� ��ġ�� ������ �ִ��� Ȯ���Ͽ� ��ġ�Ѵ�.
/// @param void
/// @return ��ġ�� ������ ���ų� ��ġ�� �����ϸ� true, �����ϸ� false
bool KPatcher::CheckProgram( void )
{
    KRegistry       kReg;
    KHttpWrapper    kHttp;
    KEnumFile       kef;

    TiXmlNode*      node        = NULL;
    TiXmlElement*   verElement  = NULL;
    TiXmlElement*   dirElement  = NULL;
    TiXmlElement*   fileElement = NULL;

    DWORD           dwCurFileNum   = 1;
    DWORD           dwTotalFileNum = 0;
    DWORD           dwRetryCount   = 0;
    char            szStringBuffer[MAX_PATH] = { 0, };

    KMsgBox::Close();

    KMsgBox::Open( "��ġ �۾� ���� ���Դϴ�.",
                   "��ġ ��� ���� �ٿ�ε� ��" );


	if ( g_bTestMode == true )
    {
        Log("�׽�Ʈ ���� ��ġ ���α׷��� ���۵˴ϴ�.");

		
    }

	// ���� ���丮�� ���α׷��� ��η� �����Ѵ�.
	::GetCurrentDirectoryA( MAX_PATH, szStringBuffer );
	m_strProgramPath = szStringBuffer;
	m_strProgramPath += "\\Data";
	Log( m_strProgramPath.c_str() );

    // ��� ���κп� '\\'�� ���� �ִٸ� �߰����ش�.
    AppendBackSlash( m_strProgramPath );

    // ���α׷� ��� �� �ִ� ���丮���� ������.
    CreateDirectory( m_strProgramPath.c_str() );

    // ���α׷��� ��θ� ���� ���丮�� �����Ѵ�.
    ::SetCurrentDirectoryA( m_strProgramPath.c_str() );
#ifndef SKIP_PATCH
	// Http�� ����.
    if ( false == kHttp.Open() )
    {
        Log( "HTTP ���⿡ �����߽��ϴ�." );

        KMsgBox::Close();

        KMsgBox::Open( "��ġ ���� ���� ����",
                       "�ٽ� �õ��� �ֽʽÿ�.",
                       KMsgBox::MBOX_OK );

        return false;
	}
    Log( "HTTP ���⿡ �����߽��ϴ�." );

    // ��ġ ���� ��θ� �����.
    std::string strSrcPath( ( true == g_bTestMode ) ?
                            ( 5 == __argc ? __argv[2] : PATCH_TEST_SRC ) :
                            ( PATCH_ORG_SRC ) );

    // ��ġ ���� ��� ���κп� '/'�� ���� �ִٸ� �߰����ش�.
    AppendSlash( strSrcPath );

    // ��ġ ��� ����( PATCH_PATH_FILE )�� �ٿ�ε��Ѵ�.
    while ( false == kHttp.Get( strSrcPath.c_str(),
                                PATCH_PATH_FILE,
                                m_strProgramPath.c_str() ) )
    {
        // MAX_RETRY�� �õ� ���� �ᱹ �ȵǸ� �����Ѵ�.
        if ( MAX_RETRY == ++dwRetryCount )
        {
            Log( PATCH_PATH_FILE " ���� �ٿ�ε忡 �����߽��ϴ�." );

            KMsgBox::Close();

            KMsgBox::Open( "��� ���� �ޱ� ����",
                           "�ٽ� �õ��� �ֽʽÿ�.",
                           KMsgBox::MBOX_OK );

            return false;
        }
        ::Sleep( MAX_DELAY );
        kHttp.Reset();
    }
    Log( PATCH_PATH_FILE " ���� �ٿ�ε忡 �����߽��ϴ�." );

    // ��ġ ���ϵ��� �ִ� ���� ��θ� �д´�.
    if ( false == ReadPatchPathFile( strSrcPath ) )
    {
        Log( PATCH_PATH_FILE " ���� �б⿡ �����߽��ϴ�." );

        KMsgBox::Close();

        KMsgBox::Open( "��� ���� �б� ����",
                       "�ٽ� �õ��� �ֽʽÿ�.",
                       KMsgBox::MBOX_OK );


        return false;
    }
    Log( PATCH_PATH_FILE " ���� �б⿡ �����߽��ϴ�." );

    // ��ġ ���� ��� ���κп� '/'�� ���� �ִٸ� �߰����ش�.
    AppendSlash( strSrcPath );

    // �ٿ�ε��� ��ġ ��� ����( PATCH_PATH_FILE )�� �����Ѵ�.
    DeleteFileAbsolutely( PATCH_PATH_FILE );

    KMsgBox::Close();


    KMsgBox::Open( "��ġ �۾� ���� ���Դϴ�.",
                   "���� ���� �ٿ�ε� ��" );

    // ��ġ �����κ��� �� ��� ����( NewVer.xml )�� �ٿ�ε� �޴´�.
    dwRetryCount = 0;
    while ( false == kHttp.Get( strSrcPath.c_str(),
                                "NewVer.xml",
                                m_strProgramPath.c_str() ) )
    {
        // MAX_RETRY�� �õ� ���� �ᱹ �ȵǸ� �����Ѵ�.
        if ( MAX_RETRY == ++dwRetryCount )
        {
            Log( "NewVer.xml ���� �ٿ�ε忡 �����߽��ϴ�." );

            KMsgBox::Close();


            KMsgBox::Open( "���� ���� �ޱ� ����",
                           "�ٽ� �õ��� �ֽʽÿ�.",
                           KMsgBox::MBOX_OK );

            return false;
        }
        ::Sleep( MAX_DELAY );
        kHttp.Reset();
    }
    Log( "NewVer.xml ���� �ٿ�ε忡 �����߽��ϴ�." );

    // NewVer.xml�� <Directory>�� �����Ͽ� ���丮 ����.
    TiXmlDocument docServer;
	
	std::string tempPath = m_strProgramPath + "NewVer.xml";
    docServer.LoadFile( tempPath.c_str() );
    if ( NULL == ( node = docServer.FirstChild( "PatchInfo" ) ) ||
         NULL == ( dirElement = node->FirstChild( "Directory" )->ToElement() ) )
    {
        Log( "NewVer.xml ���� ������ �߸��Ǿ� �ֽ��ϴ�." );

        KMsgBox::Close();

        KMsgBox::Open( "���� ���� �б� ����",
                       "�ٽ� �õ��� �ֽʽÿ�.",
                       KMsgBox::MBOX_OK );


        return false;
    }

    // ������ <Directory>�κ��� ���丮 ����.
    for ( node = dirElement->FirstChild( "Item" ); NULL != node; )
    {
        std::string strDir( m_strProgramPath );
        strDir.append( node->ToElement()->Attribute( "Name" ) );

		WCHAR wcharBuf[255] = {0,};
		MultiByteToWideChar( CP_ACP, 0, strDir.c_str(), -1, wcharBuf, 255 );

        ::CreateDirectory( wcharBuf, NULL );
        node = node->NextSibling( "Item" );
    }

    KMsgBox::Close();


    KMsgBox::Open( "���ο� ������ ��ġ�� ������",
                   "���ϰ� �ִ� ���Դϴ�." );



    // local�� "Ver.xml"�� server���� ���� "NewVer.xml"�� ���Ͽ�
    // �ٿ�ε��ؾ� �� ���ϸ���Ʈ( updateFilelist.xml )�� ����.
    TiXmlDocument xmlUpdateDoc( "updateFilelist.xml" );



	// �� ���� .xml ������ ���Ͽ� ����� �κи� .xml�� ����.
    if ( false == kef.CreateUpdateFileList( "Ver.xml",    // local file
                                            "NewVer.xml", // server file
                                            xmlUpdateDoc ) )
    {
        Log( "updateFileList.xml ���� ����⿡ �����߽��ϴ�." );

        KMsgBox::Close();


        KMsgBox::Open( "���� �� �۾� ����",
                       "�ٽ� �õ��� �ֽʽÿ�.",
                       KMsgBox::MBOX_OK );

		return false;
    }
    Log( "updateFileList.xml ���� ����⿡ �����߽��ϴ�." );

    node        = xmlUpdateDoc.FirstChild( "PatchInfo" );
    verElement  = node->FirstChild( "PatchVersion" )->ToElement();
    dirElement  = node->FirstChild( "Directory" )->ToElement();
    fileElement = node->FirstChild( "File" )->ToElement();

    // append "PatchVersion"
    TiXmlElement verItem( "Item" );
    verItem.SetAttribute( "Name", strSrcPath.c_str() );
    verElement->InsertEndChild( verItem );

	DWORD tempAllDownFileSize = 0;
    // �ٿ�ε� ���� ���� ������ ����Ѵ�.
    for ( node = fileElement->FirstChild( "Item" ); NULL != node; )
    {
        dwTotalFileNum++;
		m_dwDownAllFileSize += ::atol( node->ToElement()->Attribute( "Size" ) );

        node = node->NextSibling( "Item" );
    }

    KMsgBox::Close(); // ����ξ��� �޼��� �ڽ��� �ݴ´�.

	if ( 0 != dwTotalFileNum ) // �ٿ�ε� ���� ������ �ϳ��� �ִ� ���.
    {
    // ��ġ�� �ϰڳİ� ���� ���̾�α׸� ����� ����.
/*
#ifdef _JAPANESE
        if ( IDCANCEL == KMsgBox::Open( "You have to patch new version.",
                                        "Continue?",
                                        KMsgBox::MBOX_OKCANCEL ) )
#else // _KOREAN
        if ( IDCANCEL == KMsgBox::Open( "���ο� �������� ��ġ�ؾ� �մϴ�.",
                                        "��� ���� �Ͻðڽ��ϱ�?",
                                        KMsgBox::MBOX_OKCANCEL ) )
#endif
        {
            Log( "���α׷� ��ġ�� ��ҵǾ����ϴ�." );

            return false;
        }
        Log( "���α׷� ��ġ�� ���εǾ����ϴ�." );
*/
        m_bIsPatched = true; // ��ġ( O )

        // �ٿ�ε� â�� ����.
        OpenDownloadWindow( true );
	}
    else // �ٿ�ε� ���� ������ ���� ���.
    {
        Log( "��ġ�� ������ �����ϴ�." );

        m_bIsPatched = false; // ��ġ( X )

#ifdef DMLEE_PATCHER_TEST
		SetRateDownload( 100 );
		SetCurFileDownloadPer( 100 );

		// �ٿ�ε� â�� ����.
		OpenDownloadWindow( true );
#else
        return UpdateVersionFile(); // update Ver.xml
#endif // DMLEE_PATCHER_TEST
    }

#ifdef DMLEE_PATCHER_TEST
	if ( 0 != dwTotalFileNum )
#endif //DMLEE_PATCHER_TEST
	{
		// updateFilelist.xml�� �����Ѵٸ� �̾�ޱ⸦ �ؾ��Ѵ�.
		if ( 0 == ::_access( "updateFileList.xml", 0 ) )
		{
			TiXmlNode*    tmpNode;
			TiXmlElement* verElmt;
			TiXmlDocument oldUpdateDoc;

			oldUpdateDoc.LoadFile( "updateFileList.xml" );

			// �����Ѵٸ� ���� ��ȿ�� üũ�� ���� üũ�� �Ѵ�.
			if ( NULL != ( tmpNode = oldUpdateDoc.FirstChild( "PatchInfo" )             ) &&
				NULL != ( verElmt = tmpNode->FirstChild( "PatchVersion" )->ToElement() ) &&
				NULL != (           tmpNode->FirstChild( "Directory" )->ToElement()    ) &&
				NULL != (           tmpNode->FirstChild( "File" )->ToElement()         ) &&
				0 == strSrcPath.compare( verElmt->FirstChild( "Item" )->ToElement()->Attribute( "Name" ) ) )
			{
				// �̹� �����ϴ� updateFileList.xml�κ��� ������ ���´�.
				xmlUpdateDoc.LoadFile( "updateFileList.xml" );
				node        = xmlUpdateDoc.FirstChild( "PatchInfo" );
				verElement  = node->FirstChild( "PatchVersion" )->ToElement();
				dirElement  = node->FirstChild( "Directory" )->ToElement();
				fileElement = node->FirstChild( "File" )->ToElement();

				// ���� ������ �ٽ� ����Ѵ�.
				dwTotalFileNum = 0;
				for ( node = fileElement->FirstChild( "Item" ); NULL != node; )
				{
					dwTotalFileNum++;
					node = node->NextSibling( "Item" );
				}
			}
		}

		// updateFileList.xml�� �����Ѵ�.
		xmlUpdateDoc.SaveFile();

		// updateFilelist.xml�� <File>�� �����Ͽ� file�� download ��.
		dwRetryCount = 0;
		for ( node = fileElement->FirstChild( "Item" ); NULL != node; )
		{
			m_dwByteFile = ::atol( node->ToElement()->Attribute( "Size" ) );

			::ZeroMemory( szStringBuffer, MAX_PATH );
			ExceptFolder( szStringBuffer, node->ToElement()->Attribute( "Name" ) );

			std::ostringstream strmCurFileDesc;

			//        strmCurFileDesc << szStringBuffer << " (" << dwCurFileNum << '/'
			//                        << dwTotalFileNum << ')';

			//		strmCurFileDesc << szStringBuffer << " ("
			//                        << static_cast<int>( 100 * dwCurFileNum / dwTotalFileNum ) << "%)";

			strmCurFileDesc << szStringBuffer << " ���� ������.." ;

			m_strCurFileDesc = strmCurFileDesc.str();

			std::ostringstream strmCurFileDownloadPer;

			strmCurFileDownloadPer  << g_pPatcher->GetCurFileDownloadPer() << "%";

			m_strCurFileDownloadPer = strmCurFileDownloadPer.str();

			if ( false == kHttp.Get( strSrcPath.c_str(),
				node->ToElement()->Attribute( "Name" ),
				m_strProgramPath.c_str() ) )
			{
				// MAX_RETRY�� �õ� ���� �ᱹ �ȵǸ� �����Ѵ�.
				if ( MAX_RETRY == ++dwRetryCount )
				{
					Log( "���� �ٿ�ε忡 �����߽��ϴ�." );

					KMsgBox::Open( "��ġ ���� �ޱ� ����",
						"�ٽ� �õ��� �ֽʽÿ�.",
						KMsgBox::MBOX_OK );


					return false;
				}
				::Sleep( MAX_DELAY );
				kHttp.Reset();
			}
			else // �ٿ�ε忡 �������� ��
			{
				// �ٿ�ε忡 ������ ������ updateFileList.xml���� �����Ѵ�.
				TiXmlNode* next = node->NextSibling( "Item" );
				fileElement->RemoveChild( node );
				node = next;
				xmlUpdateDoc.SaveFile();
				dwCurFileNum++;
				dwRetryCount = 0;

				m_strCurFileDesc = "";
			}
		}

		// updateFileList.xml�� �����Ѵ�.
		DeleteFileAbsolutely( "updateFileList.xml" );

#ifdef DMLEE_PATCHER_TEST
		KMsgBox::Close();
		KMsgBox::Open( "��ġ�� �Ϸ� �Ǿ����ϴ�.",
			"\'����\'��ư�� ���� ������ �������ּ���.",
			KMsgBox::MBOX_OK );
#endif // DMLEE_PATCHER_TEST
	}

#ifndef DMLEE_PATCHER_TEST
    // �ٿ�ε� â�� ������.
    OpenDownloadWindow( false );
#endif // DMLEE_PATCHER_TEST


	bool bOK = UpdateVersionFile(); // update Ver.xml

#ifdef DMLEE_PATCHER_TEST

	m_dwCurDownAllFileSize = m_dwDownAllFileSize;
	::InvalidateRect( m_hWnd, NULL, FALSE );

	if( true == bOK )
	{
		g_bPatchLaunchButtonEnabled = true;
	}
		
#endif

	return bOK;

#else
    KMsgBox::Close();
    return true;
#endif // SKIP_PATCH
}

/// ���� Ŭ���̾�Ʈ ���α׷��� �����Ų��.
/// @param void
/// @return ���� Ŭ���̾�Ʈ ���࿡ �����ϰų� ������ �ź��ϸ� true, ���࿡ �����ϸ� false
bool KPatcher::Launch( void ) const
{
	WCHAR wcharBuf[3000] = {0,};
	MultiByteToWideChar( CP_ACP, 0, m_strProgramPath.c_str(), -1, wcharBuf, 3000 );
    ::SetCurrentDirectory( wcharBuf );

    std::string strCmdLine( m_strProgramPath + "X2.exe " );

	if( __argv[1] != NULL )
		strCmdLine.append( __argv[1] );

    // ���� Ŭ���̾�Ʈ�� �����Ų��.
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { 0, };
    si.cb = sizeof( STARTUPINFO );

	WCHAR wcharBuf2[3000] = {0,};
	MultiByteToWideChar( CP_ACP, 0, strCmdLine.c_str(), -1, wcharBuf, 3000 );
	MultiByteToWideChar( CP_ACP, 0, m_strProgramPath.c_str(), -1, wcharBuf2, 3000 );

    if ( FALSE == ::CreateProcess( NULL, const_cast<WCHAR*>(wcharBuf ),
                                   NULL, NULL, FALSE, 0, NULL,
                                   wcharBuf2, &si, &pi ) )
    {
        Log( "���ҵ� ���࿡ �����߽��ϴ�." );

        return false;
    }
    Log( "���ҵ� ���࿡ �����߽��ϴ�." );

    return true;
}

/// ��ġ ���α׷��� �����Ų��.
/// @param void
/// @return void
void KPatcher::Exit( void ) const
{
    ::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );

    Log( "��ġ ���α׷��� �����մϴ�." );
}