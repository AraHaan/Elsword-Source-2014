/** @file   KFileVerifier.cpp
    @desc   implementation of KFileVerifier
*/
//
//#define _TOSTR1(x)    #x
//#define _TOSTR(x)     _TOSTR1(x)
//#include _TOSTR(PROJECT_PCH)


#include "StdAfx.h"
#include "KFileVerifier.h"
#include <sstream>
#include "tinyxml/tinyxml.h"
#include "CRC_32.h"
#include <strsafe.h>
//#include "KDebugConsole.h"

#undef super
#define super   KJohnThread


///-----------------------------------------------------------------------------
// Ver.xml ���� helper functions
//
/// @brief  <Signature> element�� ���� �����͸� ��´�.
static const TiXmlElement* GetSignElement(const TiXmlDocument& xmlDoc_) // by parkch
{
    /* 
        <PatchInfo>
            <Signature>
                <Item Name="c07b9188" />
            </Signature>
    /**/

    const TiXmlNode* node = NULL;
    node = xmlDoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild( "Signature" );
    //if ( NULL == node )
    //{
    //    TiXmlElement item( "Signature" );
    //    xmlDoc_.ToElement()->InsertEndChild( item );
    //    node = xmlDoc_.FirstChild( "PatchInfo" )->FirstChild( "Signature" );
    //}//if
    ASSERT( node );
    return node->ToElement();
}//GetSignElement()


static const std::string GetFileListStr(const TiXmlDocument& xmlDoc_) // by parkch
{
    std::ostringstream xmlStream;
    xmlStream << xmlDoc_;

    /** ��ü .xml �������� '<File>'�� �����ϰ�, '</File>'�� ������ substring�� �����Ѵ�.
        ���� substring�� "<File> ... <"(������ '/File>'�� ����)�� ���̴�.
        - jintaeks on 2008-01-05, 13:48 */
    std::string             strStream = xmlStream.str();
    std::string::size_type  beginPos = strStream.find( "<File>" );
    std::string::size_type  endPos   = strStream.find( "</File>" );

    if ( std::string::npos == beginPos || std::string::npos == endPos )
    {
        strStream.erase();
        return strStream;
    }//if

    return strStream.substr(beginPos, endPos);
}//GetFileListStr()


/// @brief  ������ ������� �ʾҴ��� <Signature>�� ��õ� checksum�� ��ü �׽�Ʈ�Ѵ�.
static bool Verify(const TiXmlDocument& xmlDoc_) // by parkch
{
    const TiXmlElement*   pxmlSignElement = GetSignElement( xmlDoc_ );
    std::string     strFileList     = GetFileListStr( xmlDoc_ );

    if ( strFileList.empty() ) return false;

    std::string strOrgSign = CRC_32::GetCRCFromString( strFileList );
    const TiXmlNode*  node = pxmlSignElement->FirstChild( "Item" );
    //if ( NULL == node )
    //{
    //    TiXmlElement item( "Item" );
    //    item.SetAttribute( "Name", "" );
    //    pxmlSignElement->InsertEndChild( item );
    //    node = pxmlSignElement->FirstChild( "Item" );
    //}//if
    ASSERT( node );
    std::string strCurSign = node->ToElement()->Attribute( "Name" );

    return (strOrgSign == strCurSign);
}//Verify()


///-----------------------------------------------------------------------------
// KFileVerifier members
//
// constructor.
KFileVerifier::KFileVerifier()
{
    m_szProgramPath[0] = NULL;
    //m_fileMap;      ///< ������ <File> ���ǿ� ��õ� ������ ����Ʈ
    //m_komFileMap;   ///< ���Ӱ� <KomFile> ���ǿ� ��õ� ������ ����Ʈ
    //m_hackFileArr;

    m_hThreadEvent = NULL;
    m_bThreadSuspended = false;
    //m_cs;
    m_dwThreadUserParam = 0L;
    m_fpPreEndThreadCallback = NULL;
    m_bForceExistThread = false;

    _InitializeObject();
}//KFileVerifier::KFileVerifier()


// destructor.
KFileVerifier::~KFileVerifier()
{
    // ����Ǿ� ���� ���� �� �ִ� thread�� ������ �����Ų��.
    m_bForceExistThread = true;
    __super::EndThread( 3000 );
    _FinalizeObject(); // thread ���ο��� ����ϴ� object�� �����Ƿ�, thread �����Ŀ� ȣ���ؾ� �Ѵ�.

    Clear(); // container�� ������ ��� clear�Ѵ�.
}//KFileVerifier::~KFileVerifier()


///-----------------------------------------------------------------------------
// thread related functions
//
/*virtual*/ DWORD KFileVerifier::RunThread()
{
    ASSERT( m_hackFileArr.empty() );

    std::string     strChecksum;
    char            szFullPathFilename[1024];

    synchronized( m_cs )
    {
        FileInfoMap::const_iterator mitor = m_fileMap.begin();
        while ( mitor != m_fileMap.end() )
        {
            ::WaitForSingleObjectEx( m_hThreadEvent, INFINITE, FALSE );

			if( true == mitor->first.empty() )
			{
				StringCchPrintfA( szFullPathFilename, 1024, "%s", m_szProgramPath );
			}
			else
			{
				StringCchPrintfA( szFullPathFilename, 1024, "%s%s", m_szProgramPath, mitor->first.c_str() );
			}

            //BREAK( mitor->first == "data2.kom" );

            // file�� checksum�� ���Ѵ�.
            strChecksum = CRC_32::GetCRCFromFile( szFullPathFilename );
            if ( m_bForceExistThread )
                break;
//{{ todo // 2008-1-17
            // TODO: �������� ���� ������ ������ �� �����ϴ�. �޽��� �ڽ��� �� ��
            // ������ �����ϴ� ���, �Ʒ� ���忡�� ������ �߻�
            // - jintaeks on 2008-01-17, 13:53
            if ( strChecksum != mitor->second.m_strCheckSum )
//}} todo // 2008-1-17
            {
                m_hackFileArr.push_back( mitor->second );
            }//if

            ++mitor;
        }//while
    }//synchronized( m_cs )

    // thread�� Ż���ϱ� ���� ����ڰ� ��ġ�� �ݹ� �Լ��� ������ ȣ���Ѵ�.
    //
    if ( m_fpPreEndThreadCallback )
    {
        m_fpPreEndThreadCallback( m_dwThreadUserParam, this );
    }//if

    return 0L;
}//KFileVerifier::RunThread()


bool KFileVerifier::BeginThreadWithPreEndCB(
    PREENDTHREAD_CALLBACK   pPreEndThreadCallback_,
    DWORD                   dwUserParam_ /*=0L*/)
{
    // 'm_fpPreEndThreadCallback'�� �����Ѵ�.
    _SetPreEndThreadCallback( pPreEndThreadCallback_, dwUserParam_ );
    return __super::BeginThread( NULL, NULL );
        //                     ^ �ݵ�� NULL �Ķ���͸� �����ؾ� �Ѵ�.
}//KFileVerifier::BeginThreadWithPreEndCB()


void KFileVerifier::_InitializeObject()
{
    ASSERT( m_hThreadEvent == NULL );
    m_hThreadEvent = ::CreateEvent( NULL, TRUE, TRUE, NULL );
    ASSERT( m_hThreadEvent );
    m_bThreadSuspended = false;
}//KFileVerifier::_InitializeObject()


void KFileVerifier::_FinalizeObject()
{
    if ( m_hThreadEvent != NULL )
    {
        if ( m_bThreadSuspended )
            ResumeThread();
        ::CloseHandle( m_hThreadEvent );
        m_hThreadEvent = NULL;
    }//if
}//KFileVerifier::_FinalizeObject()


void KFileVerifier::_SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_)
{
    m_fpPreEndThreadCallback = pPreEndThreadCallback_;
    m_dwThreadUserParam = dwUserParam_;
}//KFileVerifier::_SetPreEndThreadCallback()


void KFileVerifier::SuspendThread()
{
    ::ResetEvent( m_hThreadEvent );
    m_bThreadSuspended = true;
}//KFileVerifier::SuspendThread()


void KFileVerifier::ResumeThread()
{
    ::SetEvent( m_hThreadEvent );
    m_bThreadSuspended = false;
}//KFileVerifier::ResumeThread()


///-----------------------------------------------------------------------------
// KFileVerifier interfaces
//
bool KFileVerifier::LoadFileList(const char* pszFullPathFilename_)
{
    ASSERT( pszFullPathFilename_ );
    if ( pszFullPathFilename_ == NULL )
        return false; // ���� �̸��� �������� ����

    Clear();

    // �� �Լ��� ȣ��� ������ ���� �������(���� Ŭ���̾�Ʈ�� �������)�� ���Ѵ�.
	GetCurrentDirectoryA( _MAX_PATH, m_szProgramPath );
	
	StringCchCatA( m_szProgramPath, _MAX_PATH, "\\" );
    //strcat( m_szProgramPath, "\\" );

    // .xml ������ �д´�.
    //
    TiXmlDocument   docLocal( pszFullPathFilename_ );
//{{ robobeg : 2008-01-17
    //docLocal.LoadFile();
    if ( !docLocal.LoadFile() )
        return false; // ������ �ε��ϴµ� ������
//}} robobeg : 2008-01-17

    // Ods306���� ������ ���ܼ� ��� �ּ�ó���ϴ�.
    // ���ҵ忡�� ����� ��� comment�� Ǯ�� �־�� �Ѵ�.
    // - seojt // 2008-1-9, 15:03
    //
    if ( !Verify( docLocal ) )
        return false; // .xml ������ <Signature>�� ��õ� checksum�� Ʋ��

    // <File> �κ��� ã�´�.
    //
    TiXmlNode*      nodeLocal = NULL;
    TiXmlElement*   fileElementLocal = NULL;

    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    ASSERT( nodeLocal );
    fileElementLocal = nodeLocal->FirstChild( "File" )->ToElement();
    ASSERT( fileElementLocal );

    /** <PatchInfo>
            <File>
                <Item ...>
        �� ��õ� ��� element�� iterate �Ѵ�.
        - jintaeks on 2008-01-05, 14:10 */
    KFileInfo   fileInfo;
    int         iTemp;
    int         iDebugMapSize = m_fileMap.size();
    nodeLocal = fileElementLocal->FirstChild( "Item" );
    while ( nodeLocal != NULL )
    {
        // fileInfo�� �����Ѵ�.
        //
        fileInfo.m_strFilename = nodeLocal->ToElement()->Attribute( "Name" );
        nodeLocal->ToElement()->Attribute( "Size", &iTemp );
        fileInfo.m_dwSize = iTemp;
        nodeLocal->ToElement()->Attribute( "Version", &iTemp );
        fileInfo.m_usVersion = (USHORT)iTemp;
        fileInfo.m_strCheckSum = nodeLocal->ToElement()->Attribute( "CheckSum" );

        // �ʿ� insert�Ѵ�.
        //
        if ( !fileInfo.m_strCheckSum.empty() )
        {
            m_fileMap.insert( std::pair<std::string,KFileInfo>( fileInfo.m_strFilename,fileInfo) );
            ASSERT( (iDebugMapSize+1) == (int)m_fileMap.size() );
            iDebugMapSize = m_fileMap.size();
        }//if

        // sibling�� iterate�Ѵ�.
        nodeLocal = nodeLocal->NextSibling( "Item" );
    }//while

    return true; // ����Ʈ ���� ����
}//KFileVerifier::LoadFileList()


void KFileVerifier::Clear()
{
    m_fileMap.clear();
    m_komFileMap.clear();
    m_hackFileArr.clear();
}//KFileVerifier::Clear()


///-----------------------------------------------------------------------------
// debug functions
//
void KFileVerifier::Debug_DumpToOutputWindow()
{
#ifndef _SERVICE_

	ChecksumLog( "[KFileVerifier::Debug_DumpToOutputWindow()\r\n" );
	debugPrintf( "[KFileVerifier::Debug_DumpToOutputWindow()\r\n" );

    // m_fileMap�� ������ ��� dump�Ѵ�.
    FileInfoMap::const_iterator mitor = m_fileMap.begin();
    while ( mitor != m_fileMap.end() )
    {
		stringstream strm;
		strm.str( "" );
		strm << "Name = " << mitor->first << ", Size = " << mitor->second.m_dwSize << ", Version = " << mitor->second.m_usVersion;
		strm << ", checksum = " << mitor->second.m_strCheckSum << std::endl;
		ChecksumLog( strm.str().c_str() );
		debugPrintf( "%s", strm.str().c_str() );

        ++mitor;
    }//while

#endif //_SERVICE_

}//KFileVerifier::Debug_DumpToOutputWindow()


void KFileVerifier::Debug_DumpHackFiles()
{
#ifndef _SERVICE_

	ChecksumLog( "[KFileVerifier::Debug_DumpHackFiles()\r\n" );
	debugPrintf( "[KFileVerifier::Debug_DumpHackFiles()\r\n" );

	// �߸��� ���� ���
	for (int i=0; i < (int)m_hackFileArr.size(); ++i)
	{
		stringstream strm;
		strm.str( "" );
		strm << "HACK!!    Name = " << m_hackFileArr[i].m_strFilename << ", Size = " << m_hackFileArr[i].m_dwSize;
		strm << ", Version = " << m_hackFileArr[i].m_usVersion;
		strm << ", checksum = " << m_hackFileArr[i].m_strCheckSum << std::endl;
		ChecksumLog( strm.str().c_str() );
		debugPrintf( "%s", strm.str().c_str() );
	}//for

	if ( m_hackFileArr.empty() )
	{
		ChecksumLog( "  no hacked files\r\n" );
		debugPrintf( "  no hacked files\r\n" );

	}//if

#endif //_SERVICE_

}//KFileVerifier::Debug_DumpHackFiles()
