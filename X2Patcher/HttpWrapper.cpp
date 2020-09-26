#include "HttpWrapper.h"
#include <mmsystem.h>
#include <WTypes.h>

// Variable declarations.


// Function declarations.
void Log( const char* szLogMsg );
void LogError( const char* szLogMsg );

/// Default constructor.
KHttpWrapper::KHttpWrapper( void )
{
    m_hInternet = NULL;
}

/// Default destructor.
KHttpWrapper::~KHttpWrapper( void )
{
    Close();
}

/// HTTP ������ ����.
/// @param void
/// @return �����ϸ� true, �����ϸ� false
bool KHttpWrapper::Open( void )
{
	// HTTP Open
    if ( NULL == ( m_hInternet = ::InternetOpen( L"HTTP_WRAPPER_KOG",
                                                 INTERNET_OPEN_TYPE_PRECONFIG,
                                                 NULL,
                                                 NULL,
                                                 0 ) ) )
    {
        LogError( "InternetOpen failed!" );
        return false;
    }
    Log( "InternetOpen succeeded!" );
	return true;
}

/// HTTP ������ �ݴ´�.
/// @param void
/// @return void
void KHttpWrapper::Close( void )
{
    // HTTP Close
    if ( NULL != m_hInternet )
    {
        ::InternetCloseHandle( m_hInternet );
        m_hInternet = NULL;
        Log( "Internet handle closed!" );
	}
}

/// HTTP ������ �����Ѵ�.
/// @param void
/// @return �����ϸ� true, �����ϸ� false
bool KHttpWrapper::Reset( void )
{
    Close();
    return Open();
}

/// �־��� ������ �ٿ�ε��Ѵ�.
/// @param szSrcPath_ �ٿ�ε��� ������ HTTP ���
/// @param szFileName_ ���� �̸�
/// @param szDestPath_ �ٿ�ε���� ��ġ�� ���
/// @return �����ϸ� true, �����ϸ� false
bool KHttpWrapper::Get( const char* szSrcPath_,
                        const char* szFileName_,
                        const char* szDestPath_,
						bool bCallByPatcher /* = true */ ) const
{
	if ( bCallByPatcher == true )
		::InvalidateRect( g_pPatcher->GetWndHandle(), NULL, FALSE );

    char szTempPatcherName[]      = { "ElSwordTemp.exe" };
    char szSrcPath[MAX_PATH]      = { 0, };
    char szSrcPathTemp[MAX_PATH]  = { 0, };
    char szDestPath[MAX_PATH]     =	{ 0, };
    char szDestPathTemp[MAX_PATH] = { 0, };

    if ( NULL == m_hInternet || NULL == szSrcPath_ ||
         NULL == szFileName_ || NULL == szDestPath_ )
    {
        return false;
    }

    // NOTE : ���� dll���� ��ġ ���α׷� ����� �Բ�
    // �ε�Ǿ� ���� ���ɼ��� �����Ƿ� ��ġ�� ���� �ʴ´�.
    // �ε�� ��쿡 ����⸦ �õ��ϸ� ��ġ�� ���߰� �ȴ�.
    // "Visual C++ 6.0" �Ǵ� "Visual Studio .NET 2003"���� �����ϵǾ� �ִٰ� �����Ѵ�.
	/*
    if ( 0 == ::_stricmp( szFileName_, "mfc42.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcp60.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcrt.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcirt.dll" ) ||
         0 == ::_stricmp( szFileName_, "mfc71.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcp71.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcr71.dll" ) ||
         0 == ::_stricmp( szFileName_, "mfc42d.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcp60d.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcrtd.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcirtd.dll" ) ||
         0 == ::_stricmp( szFileName_, "mfc71d.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcp71d.dll" ) ||
         0 == ::_stricmp( szFileName_, "msvcr71d.dll" ) )
    {
        return true;
    }
	*/

    ::strcpy( szSrcPathTemp, szSrcPath_ );
    ::strcat( szSrcPathTemp, szFileName_ );

    ::strcpy( szDestPathTemp, szDestPath_ );
	/*
    if ( 0 == ::strcmp( szFileName_, "Elsword.exe" ) )
    {
        ::strcat( szDestPathTemp, szTempPatcherName );
        g_bIsPatcherPatched = true;
    }
    else*/
    {
        ::strcat( szDestPathTemp, szFileName_ );
    }

    BackSlash2Slash( szSrcPathTemp, szSrcPath );
    Slash2BackSlash( szDestPathTemp, szDestPath );

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szSrcPath, -1, wcharBuf, 255 );

    // ������ �ִ� URL�� ������´�.
    HINTERNET hURLFile;
    if ( NULL == ( hURLFile = ::InternetOpenUrl( m_hInternet,
                                                 wcharBuf,
                                                 NULL,
                                                 0,
                                                 INTERNET_FLAG_RELOAD |
                                                 INTERNET_FLAG_NO_CACHE_WRITE,
                                                 0 ) ) )
    {
        LogError( "InternetOpenUrl failed!" );
        return false;
    }
    Log( "InternetOpenUrl succeeded!" );

    Log( "File download start!" );

    // ���� ������ �����Ѵ�.
    DWORD dwAtt;
	MultiByteToWideChar( CP_ACP, 0, szDestPath, -1, wcharBuf, 255 );
    if ( 0xffffffff != ( dwAtt = ::GetFileAttributes( wcharBuf ) ) )
    {
		
        ::SetFileAttributes( wcharBuf, dwAtt & (~FILE_ATTRIBUTE_READONLY) );
    }

    ::DeleteFile( wcharBuf );

    // �� ������ �����.
    HANDLE hLocalFile;

    if ( INVALID_HANDLE_VALUE == ( hLocalFile = ::CreateFile( wcharBuf,
                                                              GENERIC_WRITE,
                                                              0,
                                                              NULL,
                                                              CREATE_ALWAYS,
                                                              FILE_ATTRIBUTE_NORMAL,
                                                              NULL ) ) )
    {
        LogError( "CreateFile failed!" );
        return false;
    }
    Log( "CreateFile succeeded!" );


    if ( 0xffffffff != ( dwAtt = ::GetFileAttributes( wcharBuf ) ) )
    {				
        ::SetFileAttributes( wcharBuf, dwAtt & (~FILE_ATTRIBUTE_READONLY) );
    }

	if ( bCallByPatcher == true )
	{
		ASSERT( NULL != g_pPatcher );
		ASSERT( NULL != g_pPatcher->GetWndHandle() );
	}

    BYTE  buf[100 * 1024] = { 0, };
    DWORD dwcbAvail    = 0;
    DWORD dwcbRead     = 0;
    DWORD dwcbWrite    = 0;
    DWORD dwcbDown     = 0;
	DWORD dwFileSize   = 0;
	if ( bCallByPatcher == true )
	{
		dwFileSize   = g_pPatcher->GetCurDownloadFileSize();
	}

    DWORD dwTargetTime = ::timeGetTime();

	DWORD dwDownloadedFileSize = 0;

	Log( szDestPath );

    // �ٿ�ε�޴´�.
    do
    {
		

        // ������ �غ�� �����ͷ��� Ȯ���Ѵ�.
        if ( FALSE == ::InternetQueryDataAvailable( hURLFile,
                                                    &dwcbAvail,
                                                    0, 0 ) )
        {
            LogError( "InternetQueryDataAvailable failed!" );
            ::CloseHandle( hLocalFile );
            ::InternetCloseHandle( hURLFile );
            return false;
        }
        Log( "InternetQueryDataAvailable succeeded!" );

        // ������ �о���δ�.
        if ( FALSE == ::InternetReadFile( hURLFile,
                                          buf,
                                          ( ( dwcbAvail > sizeof( buf ) ) ?
                                            sizeof( buf ) :
                                            dwcbAvail ),
                                          &dwcbRead ) )
        {
            LogError( "InternetReadFile failed!" );
            ::CloseHandle( hLocalFile );
            ::InternetCloseHandle( hURLFile );
            return false;
        }
        Log( "InternetReadFile succeeded!" );

        // �ٿ�ε� ���� �����͸� ���Ͽ� ����Ѵ�.
        if ( FALSE == ::WriteFile( hLocalFile, &buf, dwcbRead, &dwcbWrite, NULL ) )
        {
            LogError( "WriteFile failed!" );
            ::CloseHandle( hLocalFile );
            ::InternetCloseHandle( hURLFile );
            return false;
        }
        Log( "WriteFile succeeded!" );

        // �ٿ�ε� ������ ����Ѵ�.
		// dwcbDown += dwcbWrite;

		dwDownloadedFileSize += dwcbRead;
		if ( bCallByPatcher == true )
			g_pPatcher->AddCurDownAllFileSize( dwcbRead );

        if ( 0 != dwFileSize )
        {
			if ( bCallByPatcher == true )
			{
				DWORD dwRateDown = (DWORD)(dwDownloadedFileSize * 100.0f / dwFileSize);
				g_pPatcher->SetRateDownload( 100 < dwRateDown ? 100 : dwRateDown );
				g_pPatcher->SetCurFileDownloadPer(  100 < dwRateDown ? 100 : dwRateDown );
			}
        }

        // 0.05�� �������� �ٿ�ε� â�� ������Ʈ�Ѵ�.
		if ( dwTargetTime < ::timeGetTime() )
        {
			RECT rcCurFileLoadBar;
			rcCurFileLoadBar.bottom = LOAD_CUR_FILE_BAR_Y + LOAD_CUR_FILE_BAR_HEIGHT;
			rcCurFileLoadBar.left	= LOAD_CUR_FILE_BAR_X;
			rcCurFileLoadBar.right	= LOAD_CUR_FILE_BAR_X + LOAD_CUR_FILE_BAR_WIDTH;
			rcCurFileLoadBar.top	= LOAD_CUR_FILE_BAR_Y;

			RECT rcCurFileLoadPer;
			rcCurFileLoadPer.bottom = LOAD_CUR_FILE_PER_Y + LOAD_CUR_FILE_PER_HEIGHT;
			rcCurFileLoadPer.left	= LOAD_CUR_FILE_PER_X;
			rcCurFileLoadPer.right	= LOAD_CUR_FILE_PER_X + LOAD_CUR_FILE_PER_WIDTH;
			rcCurFileLoadPer.top	= LOAD_CUR_FILE_PER_Y;


			RECT rcAllFileLoadBar;
			rcAllFileLoadBar.bottom	= LOAD_ALL_FILE_BAR_Y + LOAD_ALL_FILE_BAR_HEIGHT;
			rcAllFileLoadBar.left	= LOAD_ALL_FILE_BAR_X;
			rcAllFileLoadBar.right	= LOAD_ALL_FILE_BAR_X + LOAD_ALL_FILE_BAR_WIDTH;
			rcAllFileLoadBar.top	= LOAD_ALL_FILE_BAR_Y;

			RECT rcAllFileLoadPer;
			rcAllFileLoadPer.bottom	= LOAD_ALL_FILE_PER_Y + LOAD_ALL_FILE_PER_HEIGHT;
			rcAllFileLoadPer.left	= LOAD_ALL_FILE_PER_X;
			rcAllFileLoadPer.right	= LOAD_ALL_FILE_PER_X + LOAD_ALL_FILE_PER_WIDTH;
			rcAllFileLoadPer.top	= LOAD_ALL_FILE_PER_Y;

			if ( bCallByPatcher == true )
			{
				::InvalidateRect( g_pPatcher->GetWndHandle(), &rcCurFileLoadBar, FALSE );
				::InvalidateRect( g_pPatcher->GetWndHandle(), &rcCurFileLoadPer, FALSE );

				::InvalidateRect( g_pPatcher->GetWndHandle(), &rcAllFileLoadBar, FALSE );
				::InvalidateRect( g_pPatcher->GetWndHandle(), &rcAllFileLoadPer, FALSE );
			}
           
            dwTargetTime = ::timeGetTime() + 100;
        }
    }
    while ( 0 != dwcbRead );

	if ( bCallByPatcher == true )
	    ::InvalidateRect( g_pPatcher->GetWndHandle(), NULL, FALSE );
    
	::CloseHandle( hLocalFile );
    ::InternetCloseHandle( hURLFile );

    Log( "File download completed!" );

	return true;
}

/// �־��� ���ڿ����� '\'�� '/'�� �ٲ۴�.
/// @param szSrc original string
/// @param szDst output parameter
/// @return void
void KHttpWrapper::BackSlash2Slash( const char* szSrc, char* szDst ) const // by ctkim
{
    if ( NULL == szSrc ) return;

    ::strcpy( szDst, szSrc );
    char* pch = szDst;
    while ( NULL != ( pch = ::strchr( pch, '\\' ) ) )
    {
        *pch = '/';
    }
}

/// �־��� ���ڿ����� '/'�� '\'�� �ٲ۴�.
/// @param szSrc original string
/// @param szDst output parameter
/// @return void
void KHttpWrapper::Slash2BackSlash( const char* szSrc, char* szDst ) const // by ctkim
{
    if ( NULL == szSrc ) return;

    ::strcpy( szDst, szSrc );
    char* pch = szDst;
    while ( NULL != ( pch = ::strchr( pch, '/' ) ) )
    {
        *pch = '\\';
    }
}