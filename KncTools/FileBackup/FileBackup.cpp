#include ".\filebackup.h"
#include <atltime.h>    // CTime
#include <direct.h>     // _mkdir

#include <windows.h>    // GetModuleFileName, GetCurrentProcess


KFileBackup::KFileBackup(void)
{
    // default skip file
    // �⺻������ �Ʒ��� ������ skip�Ѵ�. ���� ȣ��Ǵ� ParseCmdParam ���� ����� �� �ִ�.
    // �⺻ skip �Ǵ� ������ Init���� �����ϸ� �ȵȴ�. 
    // calling order : constructor -> ParseCmdParam -> Init.
    SetSkipExt( "dll" );
    SetSkipExt( "lnk" );
    //SetSkipExt( "log" );

}

KFileBackup::~KFileBackup(void)
{
}

bool KFileBackup::Init()
{
    // get file name (itself)
    char pathbuff[MAX_PATH]     = {0};
    char namebuff[_MAX_FNAME]   = {0};
    char extbuff[_MAX_EXT]      = {0};
    ::GetModuleFileName( NULL, pathbuff, MAX_PATH );            // get file name (full path)
    ::_splitpath( pathbuff, NULL, NULL, namebuff, extbuff );    // split full path

    m_strMyName = namebuff;
    m_strMyName += extbuff;

    KLOG(( 2, "my file name : %s", m_strMyName.c_str() ));

    // get source dirctory
    ::GetCurrentDirectory( MAX_PATH, pathbuff );
    m_strSourceDir = pathbuff;

    KLOG(( 2, "source directory : %s", m_strSourceDir.c_str() ));

    // make destination direcotry
    ::strcat( pathbuff, "\\Backup" );
    MakeFolder( pathbuff );

    // �ð��� �̿��� ��� ���� �̸��� ���Ѵ�.
    CTime cTime = CTime::GetCurrentTime();
    m_strDestDir = pathbuff;
    m_strDestDir += cTime.Format("\\%Y.%m.%d_%H.%M.%S");

    MakeFolder( m_strDestDir.c_str() );

    KLOG(( 2, "destination directory : %s", m_strDestDir.c_str() ));

    // set log file path
    sprintf( pathbuff, "%s\\%s", 
        m_strDestDir.c_str(), 
        static_cast<LPCTSTR>( cTime.Format("_log_%Y.%m.%d_%H.%M.%S.log") ) );
    pSKLogManager->BeginFileOutput( pathbuff );

    return true;
}

bool KFileBackup::SetSkipExt( IN const char* szExt )
{
    CString cstrTemp( szExt );
    cstrTemp.MakeLower();               // �빮�ڸ� ��� �ҹ��ڷ� ��ȯ
    cstrTemp.TrimLeft( '.' );           // ������ '.' character�� ��� ����.
    std::string strExt( cstrTemp );

    if( m_setSkipExt.find( strExt ) != m_setSkipExt.end() )
    {
        KLOGL(( 0, "%s(), �̹� �߰��� Ȯ���� : %s", __FUNCTION__, strExt.c_str() ));
        return false;
    }

    m_setSkipExt.insert( strExt );
    return true;
}

bool KFileBackup::ParseCmdParam( const int argc, const char** argv )
{
    if( argc > 2 )
    {
        KLOG(( 2, "too many command parameters." ));
        PrintHelp();
        return false;
    }

    CString cstrParam( argv[1] );
    cstrParam.TrimLeft( '/' );
    cstrParam.MakeLower();

    if( cstrParam == "?" )  // print help
    {
        PrintHelp();
        return false;
    }

    if( cstrParam == "noskip" ) // copy all extension files
    {
        ClearSkipExt();
        KLOG(( 2, "No Skip - copy all extension files." ));
        return true;
    }

    if( cstrParam[0] == '.' ) // custom extension filter
    {
        KLOG(( 2, "Custom file extension filter setting." ));
        cstrParam.TrimLeft( '.' ); // ���� ó�� �ִ� . �� �����Ѵ�.
        char* torken = ::strtok( cstrParam.GetBuffer(), "." );
        while( torken != NULL )
        {
            SetSkipExt( torken );
            KLOG(( 2, "custom filtering extension : %s ", torken ));
            torken = ::strtok( NULL, "." );
        }
    }
    return true;
}

bool KFileBackup::Backup()
{
    // copy all files
    WIN32_FIND_DATA     wfd;
    HANDLE              hSrch;
    BOOL                bResult = TRUE;
    std::string         strWildCard( m_strSourceDir );
    int                 nCount = 0;

    strWildCard += "\\*.*";

    hSrch = ::FindFirstFile( strWildCard.c_str(), &wfd );
    if( hSrch == INVALID_HANDLE_VALUE ) // ã���� �ϴ� Ȯ������ ������ �ϳ��� ���� ���
    {
        KLOGL(( 0, "%s(), no files in directory (%s)", __FUNCTION__, m_strSourceDir.c_str() ));
        return false;
    }

    TCHAR szSrcFile[MAX_PATH] = {0};
    TCHAR szDestFile[MAX_PATH] = {0};
    while( bResult )
    {
        if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )    // ���丮�� �ƴ� ���Ͽ� ���ؼ�
        {
            if( ShouldSkip( m_strSourceDir.c_str(), wfd.cFileName ) )
            {
                KLOG(( 2, "Copy %s ... Skipped", wfd.cFileName ));
                bResult = ::FindNextFile( hSrch, &wfd );
                continue;
            }

            _tcscpy( szSrcFile, m_strSourceDir.c_str() );
            _tcscat( szSrcFile, "\\" );
            _tcscat( szSrcFile, wfd.cFileName );
            _tcscpy( szDestFile, m_strDestDir.c_str() );
            _tcscat( szDestFile, "\\" );
            _tcscat( szDestFile, wfd.cFileName );

            if( 0 == ::CopyFile( szSrcFile, szDestFile, FALSE ) )
            {
                KLOGL(( 0, "BackupFiles(), ���� ���� ����( %s -> %s )", szSrcFile, szDestFile ));
                return false;
            }

            KLOG(( 2, "Copy %s ... OK", wfd.cFileName ));
            nCount++;
        } // if
        else
        {
            KLOG(( 2, "Copy %s ... Skipped (Directory)", wfd.cFileName ));
        }

        bResult = ::FindNextFile( hSrch, &wfd );
    } // while

    KLOG(( 2, "\n * Total %d files copied. \n source : %s \n destination : %s",
                nCount, 
                m_strSourceDir.c_str(),
                m_strDestDir.c_str() ));

    return true;

}

bool KFileBackup::MakeFolder( const char* szPath )
{
    if( 0 == ::_mkdir( szPath ) )
    {
        KLOG(( 2, "directory (%s) is created.", szPath ));
        return true;
    }

    switch( errno )
    {
    case EEXIST:
        KLOG(( 2, "directory (%s) already exist.", szPath ));
        return true;
        break;
    default:
        KLOGL(( 0, "unknown error" ));
        break;
    }

    return false;
}

bool KFileBackup::ShouldSkip( const char* szPath, const char* szFileName )
{
    char szFullPath[MAX_PATH]   = {0};
    char szExt[_MAX_EXT]        = {0};

    if( m_strMyName.compare( szFileName ) == 0 )
        return true; // itself

    // make full file path
    sprintf( szFullPath, "%s\\%s", szPath, szFileName );

    // split again to get file extension
    _splitpath( szFullPath, NULL, NULL, NULL, szExt );

    // all string compared as lower case.
    CString cstrExt( szExt );
    cstrExt.MakeLower();
    cstrExt.TrimLeft( '.' );

    if( m_setSkipExt.find( std::string( cstrExt ) ) != m_setSkipExt.end() )
        return true; // skip

    return false;
}

#include <iostream>

void KFileBackup::PrintHelp()
{
    std::cout << std::endl 
              << "�ش� �۾� ������ �ִ� ������ ����մϴ�." << std::endl << std::endl
              << "�ɼ� : /?, /noskip, /.ext0.ext1.ext2...." << std::endl
              << "  /?                     ���� ���" << std::endl
              << "  /noskip                ��� Ȯ������ ������ ���" << std::endl
              << "  /.ext0.ext1.ext2....   skip�� Ȯ���ڸ� ���� ����" << std::endl
              << "                              ��) /.bat.jpg.mp3 " << std::endl << std::endl;

    std::cout << "�⺻������ skip�ϴ� ���� Ȯ���ڴ� ������ �����ϴ�." << std::endl;

    std::set<std::string>::iterator sit;
    for(sit = m_setSkipExt.begin(); sit != m_setSkipExt.end(); sit++ )
    {
        std::cout << "   " << *sit;
    }
    std::cout << " (�� " << m_setSkipExt.size() << " �� Ȯ����)" << std::endl << std::endl;

    std::cout << "����Ǵ� ������ \"��������\\Backup\\�۾��ð�\\\" �� ��ο� ����˴ϴ�." << std::endl
              << "�ش� ���� ������ ��� ������ ����� �α������� �����˴ϴ�." << std::endl;
}