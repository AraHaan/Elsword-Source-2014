#include "stdafx.h"
#include "EnumFile.h"
#include "CRC_32.h"
#include <sstream>
#include <io.h>
#include <iostream>
#include <fstream>
using namespace std;
#define COMP_BUFFER_SIZE	(512*1024)

char compbuffer0[COMP_BUFFER_SIZE];
char compbuffer1[COMP_BUFFER_SIZE];

KEnumFile::KEnumFile()
{
    memset( m_szFileName, 0, _MAX_FNAME );
    //strcpy( m_szCurPath, szCurDir ); // for full path 
    memset( m_szCurPath, 0, _MAX_PATH );
    m_nDepth          = 0;
    
    // xml
    m_pxmlSignElement = 0;
    m_pxmlDirElement  = 0;
    m_pxmlFileElement = 0;
    
    m_pxmlDoc         = NULL;

    m_pDirMap         = NULL;
    m_pFileMap        = NULL;
} // KEumFile()


KEnumFile::~KEnumFile()
{
    memset(m_szFileName, 0, _MAX_FNAME );
    memset( m_szCurPath, 0, _MAX_PATH );
    m_nDepth    = 0;
    
    DirMap::iterator    dit;
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if
    
    FileMap::iterator   fit;
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if
    
    SAFE_DELETE( m_pxmlDoc );
} // ~KEnumFile()


bool KEnumFile::Signing(TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlElement*   pxmlSignElement = GetSignElement(xmlDoc_);
    std::string     strFileList     = GetFileListStr(xmlDoc_);

    if (true == strFileList.empty()) return false;

    std::string strSign = CRC_32::GetCRCFromString( strFileList );
    TiXmlNode* node = pxmlSignElement->FirstChild("Item");
    if (NULL == node)
    {
        TiXmlElement item("Item");
        item.SetAttribute("Name", "");
        pxmlSignElement->InsertEndChild(item);
        node = pxmlSignElement->FirstChild("Item");
    }
    node->ToElement()->SetAttribute("Name", strSign);

    return true;
} // Signing()


bool KEnumFile::Verify(const TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlElement*   pxmlSignElement = GetSignElement(xmlDoc_);
    std::string     strFileList     = GetFileListStr(xmlDoc_);

    if (true == strFileList.empty()) return false;

    std::string strOrgSign = CRC_32::GetCRCFromString( strFileList );
    TiXmlNode* node = pxmlSignElement->FirstChild("Item");
    if (NULL == node)
    {
        TiXmlElement item("Item");
        item.SetAttribute("Name", "");
        pxmlSignElement->InsertEndChild(item);
        node = pxmlSignElement->FirstChild("Item");
    }
    std::string strCurSign = node->ToElement()->Attribute("Name");

    return (strOrgSign == strCurSign);
} // Verify()


TiXmlElement* KEnumFile::GetSignElement(const TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlNode* node = NULL;
    node = xmlDoc_.FirstChild("PatchInfo");
    node = node->FirstChild("Signature");
    if (NULL == node)
    {
        TiXmlElement item("Signature");
        xmlDoc_.ToElement()->InsertEndChild(item);
        node = xmlDoc_.FirstChild("PatchInfo")->FirstChild("Signature");
        ASSERT(node);
    }
    return node->ToElement();
} // GetSignElement()


const std::string KEnumFile::GetFileListStr(const TiXmlDocument& xmlDoc_) // by parkch
{
    std::ostringstream xmlStream;
    xmlStream << xmlDoc_;
    std::string strStream = xmlStream.str();
    std::string::size_type beginPos = strStream.find("<File>");
    std::string::size_type endPos   = strStream.find("</File>");
    if (std::string::npos == beginPos || std::string::npos == endPos)
    {
        strStream.erase();
        return strStream;
    }
    return strStream.substr(beginPos, endPos);
} // GetFileListStr()


bool KEnumFile::CreateFileList(const char* szCurDir_, 
                               const char* szFileName_)
{
    if( NULL != szFileName_)
    {
        strcpy( m_szFileName, szFileName_ );
        m_pxmlDoc         = new TiXmlDocument( szFileName_ );
        m_pDirMap         = new DirMap;
        m_pFileMap        = new FileMap;
    }
    else
    {
        return false;
    }

//    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >\n"
//                          "<PatchInfo>"
//                          "<Directory>\n"
//                          "</Directory>"
//                          "<File>\n"
//                          "</File>"
//                          "</PatchInfo>";
    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >\n"
                          "<PatchInfo>"
                          "<Signature>"
                          "</Signature>"
                          "<Directory>\n"
                          "</Directory>"
                          "<File>\n"
                          "</File>"
                          "</PatchInfo>";
    m_pxmlDoc->Parse( szStart );

    if( m_pxmlDoc->Error() )
    {
        printf( "Error in %s: %s\n", m_pxmlDoc->Value(), m_pxmlDoc->ErrorDesc() );
        return false;
    } // if

    TiXmlNode* node = 0;
    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();

    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("Directory");
    assert( node );
    m_pxmlDirElement = node->ToElement();

    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("File");
    assert( node );
    m_pxmlFileElement = node->ToElement();

    // enumerate file
    SetCurrentDirectory( szCurDir_ );
    EnumFile();

    // generate xml
    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );

    // directory
    DirMap::iterator    dit;
    for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++ )
    {
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", dit->second->m_szName );
        m_pxmlDirElement->InsertEndChild( item );
    } // for
    
    // file
    FileMap::iterator   fit;
    for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++ )
    {
//{{ Modified by ct 2003-04-29 ���� 04:54:04
//        char buf[40];
//}} Modified by ct 2003-04-29 ���� 04:54:04
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", fit->second->m_szName );
        item.SetAttribute( "Size", fit->second->m_dwSize );
        item.SetAttribute( "Version", fit->second->m_usVersion );
        item.SetAttribute( "CheckSum", fit->second->m_strCheckSum );
//{{ Modified by ct 2003-04-29 ���� 04:53:34
/*
        ultoa( fit->second->m_ftLastWriteTime.dwHighDateTime, buf, 10 );
        item.SetAttribute( "LastHigh", buf );
        ultoa( fit->second->m_ftLastWriteTime.dwLowDateTime, buf, 10 );
        item.SetAttribute( "LastLow",  buf );
*/
//}} Modified by ct 2003-04-29 ���� 04:53:34

        m_pxmlFileElement->InsertEndChild( item );
    } // for

    m_pxmlDoc->SaveFile();

    // �ı�
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if
    
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if
    
    SAFE_DELETE( m_pxmlDoc );

    return true;
}


void KEnumFile::EnumFile()
{
    HANDLE          hFile;
    WIN32_FIND_DATA	wfd;
    
    hFile = FindFirstFile( "*.*", &wfd );

    while( FindNextFile(hFile, &wfd) )
	{
		// parent directory�� ��� skip
        if( strcmp(wfd.cFileName, "..") == 0 ) 
            continue;

		// directory�� ���
        if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
            SetCurrentDirectory( wfd.cFileName );
			m_nDepth++;

			strcat( m_szCurPath, wfd.cFileName );
			strcat( m_szCurPath, "\\" );
            
            // Add item to Directory element
            KDirInfo*   pkDir = new KDirInfo;
            strcpy( pkDir->m_szName, m_szCurPath );
            (*m_pDirMap)[pkDir->m_szName] = pkDir;

			EnumFile();
		}
		else
		{
			int nTempLen;
            char szTempPath[_MAX_PATH];

			nTempLen = sprintf( szTempPath, "%s%s", m_szCurPath, wfd.cFileName );
			szTempPath[nTempLen++] = 0;
            
            // Add item to File element
            KFileInfo*  pkFile  = new KFileInfo;
            strcpy( pkFile->m_szName, szTempPath );
            pkFile->m_dwSize    = wfd.nFileSizeLow;
            pkFile->m_usVersion = 1;
            ::CharUpper( wfd.cFileName );
            std::string strFileName( wfd.cFileName );
			// �߿� ���ϸ� checksum�� �����Ѵ�
            if ( ( strFileName.find( ".EXE" ) != std::string::npos ) || 
                 ( strFileName.find( ".DLL" ) != std::string::npos ) ||
                 ( strFileName.find( ".FRM" ) != std::string::npos ) ||
                 ( strFileName.find( ".STG" ) != std::string::npos ) ||
                 ( strFileName.find( ".LUA" ) != std::string::npos ) ||
                 ( strFileName.find( ".KOM" ) != std::string::npos ) ||
                 ( strFileName.find( ".KSF" ) != std::string::npos ) ||
				 ( strFileName.find( ".DES" ) != std::string::npos ) || 
				 //( strFileName.find( ".MP3" ) != std::string::npos ) || 
                 ( strFileName.find( ".INI" ) != std::string::npos ) )
            {
                pkFile->m_strCheckSum = CRC_32::GetCRCFromFile( wfd.cFileName );
            }
            else
            {
                pkFile->m_strCheckSum.erase();
            }
//{{ Modified by ct 2003-04-29 ���� 04:54:24
//            pkFile->m_ftLastWriteTime = wfd.ftLastWriteTime;
//}} Modified by ct 2003-04-29 ���� 04:54:24
            (*m_pFileMap)[pkFile->m_szName] = pkFile;

        } // else
    } // while

	if( m_nDepth ) 
	{
		if( m_nDepth == 1 ) 
            m_szCurPath[0] = 0;
		else 
		{
			*strrchr( m_szCurPath, '\\' ) = '\0';
			*(strrchr( m_szCurPath, '\\' ) + 1) = '\0';
        } // else

		SetCurrentDirectory( ".." );
		m_nDepth--;
    } // if

    if( hFile != INVALID_HANDLE_VALUE )
        FindClose( hFile );
} // EnumFile()


bool KEnumFile::CreateFileList(const char* szCurDir_, 
                               TiXmlDocument& xmldoc_)
{
    m_pDirMap         = new DirMap;
    m_pFileMap        = new FileMap;
    
    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >"
                          "<PatchInfo>"
                          "<Signature>"
                          "</Signature>"
                          "<Directory>"
                          "</Directory>"
                          "<File>"
                          "</File>"
                          "</PatchInfo>";
    
    xmldoc_.Parse( szStart );
    if( xmldoc_.Error() )
    {
        printf( "Error in %s: %s\n", xmldoc_.Value(), xmldoc_.ErrorDesc() );
        return false;
    } // if
    
    //xmldoc_.Print( stdout );
    
    TiXmlNode* node = 0;
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Directory");
    assert( node );
    m_pxmlDirElement = node->ToElement();
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("File");
    assert( node );
    m_pxmlFileElement = node->ToElement();
    
    // enumerate file
    GetCurrentDirectory( _MAX_PATH, m_szCurWorkingDir );
    SetCurrentDirectory( szCurDir_ );
    EnumFile();
    SetCurrentDirectory( m_szCurWorkingDir );
    
    // generate xml
    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );
    
    // directory
    DirMap::iterator    dit;
    for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++ )
    {
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", dit->second->m_szName );
        m_pxmlDirElement->InsertEndChild( item );
    } // for
    
    // file
    FileMap::iterator   fit;
    for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++ )
    {
//{{ Modified by ct 2003-04-29 ���� 04:56:05
/*
        char buf[40];
*/
//}} Modified by ct 2003-04-29 ���� 04:56:05
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", fit->second->m_szName );
        item.SetAttribute( "Size", fit->second->m_dwSize );
        item.SetAttribute( "Version", fit->second->m_usVersion );
        item.SetAttribute( "CheckSum", fit->second->m_strCheckSum );
//{{ Modified by ct 2003-04-29 ���� 04:55:35

/*
        ultoa( fit->second->m_ftLastWriteTime.dwHighDateTime, buf, 10 );
        item.SetAttribute( "LastHigh", buf );
        ultoa( fit->second->m_ftLastWriteTime.dwLowDateTime, buf, 10 );
        item.SetAttribute( "LastLow",  buf );
*/
//}} Modified by ct 2003-04-29 ���� 04:55:35

        m_pxmlFileElement->InsertEndChild( item );
    } // for
    
    //xmldoc_.SaveFile();
    //xmldoc_.Print( stdout );
    
    // �ı�
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if
    
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if
    
    return true;
}

bool KEnumFile::CompareFileVersion(const TiXmlDocument& xmlOldDoc_, 
                                   const TiXmlDocument& xmlDoc_, 
                                   TiXmlDocument& xmlNewDoc_)
{
    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
                            "<PatchInfo>"
                            "<Signature>"
                            "</Signature>"
                            "<Directory>\n"
                            "</Directory>"
                            "<File>\n"
                            "</File>"
                            "</PatchInfo>";
    xmlNewDoc_.Parse(szInitDoc);
    
    TiXmlNode*      node = 0;
    TiXmlNode*      nodeOld = 0;
    TiXmlNode*      nodeNew = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementOld = 0;
    TiXmlElement*   dirElementNew = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementOld = 0;
    TiXmlElement*   fileElementNew = 0;

    node = xmlDoc_.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    dirElementOld = nodeOld->FirstChild("Directory")->ToElement();
    assert( dirElementOld );
    
    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    dirElementNew = node->FirstChild("Directory")->ToElement();
    assert( dirElementNew );
    
    // Directory
    node = dirElement->FirstChild("Item");
    while( node != 0 )
    {
        //dirElementNew->InsertEndChild(*(node->Clone()));
        dirElementNew->InsertEndChild(*node);
        node = node->NextSibling("Item");
    } // while

    // File
    node = xmlDoc_.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    fileElementOld = nodeOld->FirstChild("File")->ToElement();
    assert( fileElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    fileElementNew = node->FirstChild("File")->ToElement();
    assert( fileElementNew );

    int nSize = 0,
        nSizeOld = 0,
        nVersion = 0,
        nVersionOld = 0;
    
    node = fileElement->FirstChild("Item");
    nodeOld = fileElementOld->FirstChild("Item");
    while( node != 0 || nodeOld != 0 )
    {
        if( nodeOld != 0 && node == 0 )
        {
            // Old ���� ������ ���
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } // if
        
        if( nodeOld == 0 && node != 0 )
        {
            // new list�� �߰� �Ǿ�� �� ���            
            //fileElementNew->InsertEndChild(*(node->Clone()));
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name�� ���� ���
        if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            nodeOld->ToElement()->Attribute("Size", &nSizeOld);
            nodeOld->ToElement()->Attribute("Version", &nVersionOld);
        
//{{ Modified by ct 2003-04-29 ���� 04:57:32
/*
            FILETIME ft;
            FILETIME ftOld;
            ft.dwHighDateTime   = atol( node->ToElement()->Attribute("LastHigh") );
            ft.dwLowDateTime    = atol( node->ToElement()->Attribute("LastLow") );
            ftOld.dwHighDateTime= atol( nodeOld->ToElement()->Attribute("LastHigh") );
            ftOld.dwLowDateTime = atol( nodeOld->ToElement()->Attribute("LastLow") );
*/
//}} Modified by ct 2003-04-29 ���� 04:57:32

            
//{{ Modified by ct 2003-04-29 ���� 04:57:57
/*
            if( (nSize != nSizeOld) ||
                (nVersion != nVersionOld) ||
                (CompareFileTime(&ft, &ftOld) != 0) ) // �ð��� �ٸ� ���
            {
*/
//}} Modified by ct 2003-04-29 ���� 04:57:57
            if( (nSize != nSizeOld) && 
                (nVersion != nVersionOld) ) // �ð��� �ٸ� ���
            {
                // file�� version�� ����� ���
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")) + 1);
            }
            else
            {
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")));
            }
            //fileElementNew->InsertEndChild(*(node->Clone()));
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // Old�� �����Ǵ� ���
                nodeOld = nodeOld->NextSibling("Item");
            }
            else
            {
                //fileElementNew->InsertEndChild(*(node->Clone()));
                fileElementNew->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    return true;
} // CompareFileVersion()


bool KEnumFile::CreateUpdateFileList(const char* szLocalFileName_, 
                                     const char* szServerFileName_, 
                                     TiXmlDocument& xmlUpdateDoc_)
{
    char szPath[_MAX_PATH];
    char szLocalFileFullPath[_MAX_PATH];
    GetCurrentDirectory( _MAX_PATH, szPath );
    strcat( szPath, "\\" );

    // local filelist
    strcpy( szLocalFileFullPath, szPath );
    strcat( szLocalFileFullPath, szLocalFileName_ );

    TiXmlDocument docLocal_;

    //TiXmlDocument docLocal(szLocalFileFullPath);
    TiXmlDocument docLocal(szLocalFileFullPath);
    if( !docLocal_.LoadFile(szLocalFileFullPath) )
    {
//        const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
//                                "<PatchInfo>"
//                                "<Signature>"
//                                "</Signature>"
//                                "<Directory>\n"
//                                "</Directory>"
//                                "<File>\n"
//                                "</File>"
//                                "</PatchInfo>";
//        docLocal.Parse(szInitDoc);
        CreateFileList(szPath, docLocal);
        KEnumFile::Signing(docLocal);
        docLocal.SaveFile();
    }
    else
    {
        docLocal.LoadFile();
        if (false == Verify(docLocal))
        {
            docLocal.Clear();
            CreateFileList(szPath, docLocal);
            KEnumFile::Signing(docLocal);
            docLocal.SaveFile();
        }
    }

    // server filelist
    TiXmlDocument docServer( szServerFileName_ );
    if( !docServer.LoadFile() )
    {
        printf( "Could not load file. Error='%s'. Exiting.\n", docServer.ErrorDesc() );
        return false;
    }
    //docServer.Print( stdout );

    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
                            "<PatchInfo>"
//{{ Appended by parkch: 2003-11-18 ���� 9:23:19
                            "<PatchVersion>"
                            "</PatchVersion>"
//}} Appended by parkch: 2003-11-18 ���� 9:23:19
                            "<Signature>"
                            "</Signature>"
                            "<Directory>\n"
                            "</Directory>"
                            "<File>\n"
                            "</File>"
                            "</PatchInfo>";

    //TiXmlDocument docDownload( "downloadfilelist.xml" );
    xmlUpdateDoc_.Parse(szInitDoc);
    
    TiXmlNode*      node = 0;
    TiXmlNode*      nodeLocal = 0;
    TiXmlNode*      nodeDownload = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementLocal = 0;
    TiXmlElement*   dirElementDownload = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementLocal = 0;
    TiXmlElement*   fileElementDownload = 0;

    node = docServer.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );
    
    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    dirElementLocal = nodeLocal->FirstChild("Directory")->ToElement();
    assert( dirElementLocal );
    
    node = xmlUpdateDoc_.FirstChild( "PatchInfo" );
    dirElementDownload = node->FirstChild("Directory")->ToElement();
    assert( dirElementDownload );
    
    node = dirElement->FirstChild("Item");
    nodeLocal = dirElementLocal->FirstChild("Item");
    while( node != 0 || nodeLocal != 0 )
    {
        if( nodeLocal != 0 && node == 0 )
        {
            // local ���� ������ ���
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } // if
        
        if( nodeLocal == 0 && node != 0 )
        {
            // server�� list�� �߰� �Ǿ�� �� ���            
            //dirElementDownload->InsertEndChild(*(node->Clone()));
            dirElementDownload->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if
        
        // name�� ���� ���
        if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node = node->NextSibling("Item");
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // local�� �����Ǵ� ���
                nodeLocal = nodeLocal->NextSibling("Item");
            }
            else
            {
                //dirElementDownload->InsertEndChild(*(node->Clone()));
                dirElementDownload->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while


    node = docServer.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );
    
    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    fileElementLocal = nodeLocal->FirstChild("File")->ToElement();
    assert( fileElementLocal );

    node = xmlUpdateDoc_.FirstChild( "PatchInfo" );
    fileElementDownload = node->FirstChild("File")->ToElement();
    assert( fileElementDownload );

    int nSize = 0,
        nSizeLocal = 0,
        nVersion = 0,
        nVersionLocal = 0;
    std::string strCheckSum;
    std::string strCheckSumLocal;
    std::string strCheckSumActual;
    char szActualFilePath[_MAX_PATH];

    node = fileElement->FirstChild("Item");
    nodeLocal = fileElementLocal->FirstChild("Item");
    while( node != 0 || nodeLocal != 0 )
    {
        if( nodeLocal != 0 && node == 0 )
        {
            // local ���� ������ ���
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } // if
        
        if( nodeLocal == 0 && node != 0 )
        {
            // server�� list�� �߰� �Ǿ�� �� ���            
            // fileElementDownload->InsertEndChild(*(node->Clone()));
            fileElementDownload->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name�� ���� ���
        if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            strCheckSum = node->ToElement()->Attribute("CheckSum");
            nodeLocal->ToElement()->Attribute("Size", &nSizeLocal);
            nodeLocal->ToElement()->Attribute("Version", &nVersionLocal);
            strCheckSumLocal = nodeLocal->ToElement()->Attribute("CheckSum");

//{{ Appended by parkch: 2003-07-09 ���� 5:24:01
            // ���� ������ ��θ� ���Ѵ�.
            ZeroMemory( szActualFilePath, _MAX_PATH );
            strcpy( szActualFilePath, szPath );
            strcat( szActualFilePath, node->ToElement()->Attribute("Name") );

            // ������Ʈ ����Ʈ�� ���� ���ϵ��� �����Ѵ�.
            if( -1 == _access( szActualFilePath, 0 ) ) // 0. ���� ���� ���� üũ
            {
                // ������ �������� ������ ������Ʈ ����Ʈ�� �߰�.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( nVersion != nVersionLocal ) // 1. ���� üũ
            {
                // ������ �ٸ��� ������Ʈ ����Ʈ�� �߰�.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( nSize != nSizeLocal ) // 2. ���� ũ�� üũ
            {
                // ���� ũ�Ⱑ �ٸ��� ������Ʈ ����Ʈ�� �߰�.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( !strCheckSum.empty() ) // üũ�� üũ�� �ʿ��� ������ ���
            {
                // �߿� ������ ���, ��ŷ ������ ���� üũ������ ��ȿ���� �Ǵ��Ѵ�.
                strCheckSumActual = CRC_32::GetCRCFromFile(szActualFilePath);

                if ( strCheckSum != strCheckSumLocal ) // 3. �� ����Ʈ�� üũ�� üũ
                {
                    // �� ����Ʈ�� üũ���� �ٸ��� ������Ʈ ����Ʈ�� �߰�.
                    fileElementDownload->InsertEndChild(*node);
                }
                else if( strCheckSum != strCheckSumActual ) // 4. ���� ������ üũ�� üũ
                {
                    // ���� ������ üũ���� �ٸ��� ������Ʈ ����Ʈ�� �߰�.
                    fileElementDownload->InsertEndChild(*node);
                }
            }
//}} Appended by parkch: 2003-07-09 ���� 5:24:01

            node = node->NextSibling("Item");
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // local�� �����Ǵ� ���
                nodeLocal = nodeLocal->NextSibling("Item");
            }
            else
            {
                // fileElementDownload->InsertEndChild(*(node->Clone()));
                fileElementDownload->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    KEnumFile::Signing(docServer);
    docServer.SaveFile();

    return true;
} //CreateUpdateFileList()



bool KEnumFile::IsDiffFiles(const TCHAR* szSrcName, const TCHAR* szDstName)
{
    FILE* fpSrc = NULL;
    FILE* fpDst = NULL;
    fpSrc = ::_tfopen(szSrcName, "rb");
    fpDst = ::_tfopen(szDstName, "rb");

    // 1. error case check
    // std::cout << "step 1: check error case" << endl;
    if (fpSrc == NULL || fpDst == NULL)
    {
        ::fprintf(stderr, "Warnning: %s or %s not exist\n", szSrcName, szDstName);
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return true;
    }

    // 2. size compare
    //cout << "step 2: size compare" << endl;
    fpos_t posSrc = NULL;
    fpos_t posDst = NULL;
    ::fseek(fpSrc, 0, SEEK_END);
    ::fgetpos(fpSrc, &posSrc);
    ::fseek(fpSrc, 0, SEEK_SET);
    ::fseek(fpDst, 0, SEEK_END);
    ::fgetpos(fpDst, &posDst);
    ::fseek(fpDst, 0, SEEK_SET);
    if (posSrc != posDst)
    {
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return true;
    }

    // 3. read the file contents
    // cout << "step 3: read the contents" << endl;
	size_t ReadByte0 = 0;
	size_t ReadByte1 = 0;

	do 
	{
		ReadByte0 = fread(compbuffer0,1,COMP_BUFFER_SIZE,fpSrc);
		ReadByte1 = fread(compbuffer1,1,COMP_BUFFER_SIZE,fpDst);
		if( ReadByte0 != ReadByte1 )
		{
			fprintf(stderr, "ERROR: Something prob(0)\n");
			return true;
		}
		if( memcmp(compbuffer0, compbuffer1, ReadByte0 ) )
		{
			return true;
		}
	}	 
	while( ReadByte0 != 0 && ReadByte1 != 0 );

	



	/*

    char* bufSrc = NULL;
    char* bufDst = NULL;
    bufSrc = new char[(size_t)posSrc];
    bufDst = new char[(size_t)posDst];
    if (bufSrc == NULL || bufDst == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory\n");
        if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
        if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return true;
    }
    ::fread(bufSrc, sizeof(char), (size_t)posSrc, fpSrc);
    ::fread(bufDst, sizeof(char), (size_t)posDst, fpDst);
    if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
    if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }

    // 4. byte-by-byte compare
    // cout << "step 4: byte compare" << endl;
    char* ptrSrc = bufSrc;
    char* ptrDst = bufDst;
    while (ptrSrc < bufSrc + posSrc)
    {
        if (*ptrSrc++ != *ptrDst++)
        {
            if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
            if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
            return true;
        }
    }
    if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
    if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
	*/
    return false;
} // IsDiffFiles()


bool KEnumFile::CompareFileVersion(const char* szOldPath_, const TiXmlDocument& xmlOldDoc_, 
                                   const char* szNewPath_, const TiXmlDocument& xmlDoc_, 
                                   TiXmlDocument& xmlNewDoc_)
{
    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
                            "<PatchInfo>"
                            "<Signature>"
                            "</Signature>"
                            "<Directory>\n"
                            "</Directory>"
                            "<File>\n"
                            "</File>"
                            "</PatchInfo>";
    xmlNewDoc_.Parse(szInitDoc);
    
    TiXmlNode*      node = 0;
    TiXmlNode*      nodeOld = 0;
    TiXmlNode*      nodeNew = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementOld = 0;
    TiXmlElement*   dirElementNew = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementOld = 0;
    TiXmlElement*   fileElementNew = 0;

    node = xmlDoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();

    node = xmlDoc_.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    dirElementOld = nodeOld->FirstChild("Directory")->ToElement();
    assert( dirElementOld );
    
    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    dirElementNew = node->FirstChild("Directory")->ToElement();
    assert( dirElementNew );

    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );
    
    // Directory
    node = dirElement->FirstChild("Item");
    while( node != 0 )
    {
        dirElementNew->InsertEndChild(*node);
        node = node->NextSibling("Item");
    } // while

    // File
    node = xmlDoc_.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    fileElementOld = nodeOld->FirstChild("File")->ToElement();
    assert( fileElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    fileElementNew = node->FirstChild("File")->ToElement();
    assert( fileElementNew );

    int nSize = 0,
        nSizeOld = 0,
        nVersion = 0,
        nVersionOld = 0;
    
    node = fileElement->FirstChild("Item");
    nodeOld = fileElementOld->FirstChild("Item");
    SYSTEMTIME st;
    GetSystemTime(&st);
    int iTotalPatchSize = 0;
    while( node != 0 || nodeOld != 0 )
    {
        if( nodeOld != 0 && node == 0 )
        {
            // Old ���� ������ ���
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } // if
        
        if( nodeOld == 0 && node != 0 )
        {
            // new list�� �߰� �Ǿ�� �� ���            
            fileElementNew->InsertEndChild(*node);
            node->ToElement()->Attribute("Size", &nSize);
            iTotalPatchSize += nSize;
            node = node->NextSibling("Item");
            continue;
        } // if

        // name�� ���� ���
        if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            nodeOld->ToElement()->Attribute("Size", &nSizeOld);
            nodeOld->ToElement()->Attribute("Version", &nVersionOld);
    
            std::string strOldFileName;
            std::string strNewFileName;
            strOldFileName = szOldPath_;
            strOldFileName.append(nodeOld->ToElement()->Attribute("Name"));
            strNewFileName = szNewPath_;
            strNewFileName.append(node->ToElement()->Attribute("Name"));

            std::string strOldCheckSum;
            std::string strNewCheckSum;
            strOldCheckSum = nodeOld->ToElement()->Attribute("CheckSum");
            strNewCheckSum = node->ToElement()->Attribute("CheckSum");

            // 2007/5/10. daeuk. ���Ǳ��
#define MAIN_FILE_NAME "Main.exe"
            std::string strOld;
            std::string strNew;
            strOld = nodeOld->ToElement()->Attribute("Name");
            strNew = node->ToElement()->Attribute("Name");
            if( strOld.compare( MAIN_FILE_NAME ) == 0 &&
                strNew.compare( MAIN_FILE_NAME ) == 0 )
            {
                m_strOldMainCRC = strOldCheckSum.c_str();
                m_strNewMainCRC = strNewCheckSum.c_str();
            }

            if( IsDiffFiles(strOldFileName.c_str(),
                            strNewFileName.c_str()) ) // ������ ��
            {
                // file�� version�� ����� ���
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")) + 1);
                fileElementNew->InsertEndChild(*node);
                iTotalPatchSize += nSize;
            }
            else
            {
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")));
                fileElementNew->InsertEndChild(*node);
            }
            node = node->NextSibling("Item");
            nodeOld = nodeOld->NextSibling("Item");


            // file�� �ð� setting
            HANDLE          hFile;
            hFile = CreateFile( strNewFileName.c_str(),
                                GENERIC_WRITE,
                                FILE_SHARE_READ,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
 
            if( hFile == INVALID_HANDLE_VALUE ) 
            { 
                continue;
            }  // if
            FILETIME ft;
            BOOL f;
            SystemTimeToFileTime(&st, &ft);
            f = SetFileTime( hFile, &ft, &ft, &ft );
            CloseHandle(hFile); 
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // Old�� �����Ǵ� ���
                nodeOld = nodeOld->NextSibling("Item");
            }
            else
            {
                fileElementNew->InsertEndChild(*node);
                node->ToElement()->Attribute("Size", &nSize);
                iTotalPatchSize += nSize;
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while
    fstream kfStream;
    kfStream.open( "FileSize.txt", std::ios::out );
    float fFileSize = (float)iTotalPatchSize;
    fFileSize = (fFileSize/1024.0f)/1024.0f;
    kfStream << fFileSize << " MB";
    kfStream.close();

    return true;
} // CompareFileVersion()