// DevSideAutomaion.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "DevSideAutomaion.h"
#include "TinyXml/tinyxml.h"

#include <DbgHelp.h>
#pragma comment( lib, "Dbghelp.lib" )

#include "IniReader.h"
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

std::map<char,int> mapHexToDec; //16���� �ѱ� �ּҸ� 10������ �ٲٱ����� map

void InitHex()
{  
    mapHexToDec['0'] = 0;
    mapHexToDec['1'] = 1;
    mapHexToDec['2'] = 2;
    mapHexToDec['3'] = 3;
    mapHexToDec['4'] = 4;
    mapHexToDec['5'] = 5;
    mapHexToDec['6'] = 6;
    mapHexToDec['7'] = 7;
    mapHexToDec['8'] = 8;
    mapHexToDec['9'] = 9;
    mapHexToDec['A'] = 10;
    mapHexToDec['B'] = 11;
    mapHexToDec['C'] = 12;
    mapHexToDec['D'] = 13;
    mapHexToDec['E'] = 14;
    mapHexToDec['F'] = 15;
}

std::string DecodeURL(std::string strURL)
{
    std::string strURLConvert;

    for( std::string::iterator sit = strURL.begin() ; sit != strURL.end() ; ++sit )
    {
        //UTF-8���ڴ� % �ڿ� �� �����̹Ƿ� �Ʒ��Ͱ��� �����Ѵ�.
        if( *sit == '%' )
        {
            char cRes = 0;
            cRes = mapHexToDec[*(++sit)] * 16;
            cRes += mapHexToDec[*(++sit)];
            strURLConvert.append( 1, cRes );
        }
        else
        {
            strURLConvert.append( 1, *sit );
        }
    }

    return strURLConvert;
}

bool GetTimeStamp( IN const char* strSourceFile, OUT std::string& strTimeStamp )
{
    HANDLE hFile;
    HANDLE hFMap;

    CFileStatus fileStatus;
    if( !CFile::GetStatus( strSourceFile, fileStatus ) )
        return false;

    hFile = CreateFile( fileStatus.m_szFullName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile == INVALID_HANDLE_VALUE )
    {
        return false;
    }//end if
    
    // ���� ���� ������Ʈ�� �����.
    hFMap = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    // �ּ� ������ ���Ѵ�.
    HMODULE hModule;
    hModule = ( HMODULE )MapViewOfFile( hFMap, FILE_MAP_READ, 0, 0, 0 );
    CTime time( GetTimestampForLoadedLibrary( hModule ) );

    strTimeStamp = time.Format( "%y%m%d_%H%M" );

    UnmapViewOfFile( hModule );
    CloseHandle( hFMap );
    CloseHandle( hFile );
    return true;
}

#define SECTION argv[1]
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{       
    // MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
        // TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
        _tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
        return 1;
    }
    if( argc != 2 )
        return 1;
    
    InitHex();

    CIniReader iniReader( ".\\GetTagVersion.ini" );

    char CurrentDirectory[MAX_PATH] = {0};
    GetCurrentDirectoryA( MAX_PATH, CurrentDirectory );
    
    //ini���� �������� ������ �о����(�����̸�, �����̸�, SVNURL�̸�..)
    std::string strSourceFile = CurrentDirectory;
    std::string strSVNURL;
    std::string strNowFolder;
    strSourceFile += "\\";
    strSourceFile += iniReader.ReadString( SECTION, "SourceFolder_0", NULL );
    strSourceFile += iniReader.ReadString( SECTION, "SourceFile_0", NULL );
    strSourceFile += ".exe";
    strSVNURL     = iniReader.ReadString( SECTION, "SVNURL", NULL ); //svn depot �ּ�..���� ��������    
    
    //tag���...
    std::string strTimeStamp;
    if( !GetTimeStamp( strSourceFile.c_str(), strTimeStamp ) )
        return 1;

    std::stringstream stm;

    stm << "@echo off\n";

    int nFileNum = iniReader.ReadInteger( SECTION, "FileNum", 0 );
    int i;
    for( i = 0 ; i < nFileNum ; ++i )
    {
        char szfolder[256];
        char szfile[256];
        sprintf( szfolder,"SourceFolder_%d", i );
        sprintf( szfile,"SourceFile_%d", i );

        stm << "if not exist " << CurrentDirectory << "\\" 
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".exe " << "goto _noexist" << "\n";

        stm << "if not exist " << CurrentDirectory << "\\" 
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".pdb " << "goto _noexist" << "\n";
    }

    stm << "echo �ϰ��۾� ��ũ��Ʈ�� �����մϴ�!" << "\n"
        << "echo �ߴ��ϽǷ��� Ctrl + C �� ���� ���߼��� -��-/" << "\n"
        << "pause" << "\n";

    for( i = 0 ; i < nFileNum ; ++i )
    {
        char szfolder[256];
        char szfile[256];
        char szDestFolder[256];
        sprintf( szfolder,"SourceFolder_%d", i );
        sprintf( szfile,"SourceFile_%d", i );
        sprintf( szDestFolder, "DestFolder_%d", i);

        stm << "copy " << CurrentDirectory << "\\" 
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".exe" <<" " << iniReader.ReadString( SECTION, szDestFolder, NULL ) << "\n";
    }

    //xml ���� ���� �۾� svn���� �˾Ƴ���..
    std::string strXMLPath;     //xml ���� �̸� ���    
    strXMLPath.clear();    
    char cAddXML[] = "\\svn.xml";
    strXMLPath.append( CurrentDirectory, strlen(CurrentDirectory) );
    strXMLPath.append( cAddXML, sizeof(cAddXML) );
     
    //xml���� �ʿ��� ������ �̾ƿ���...url, revision
    TiXmlDocument docXML;       
    TiXmlNode* nodeInfo;   
    TiXmlNode* nodeEntry;
    TiXmlNode* nodeURL;

    std::string strRevision;
    std::string strURL;            //url �д� ����
    std::string strURLConvertBAT;  //���� URL�� %�� %%�� �ٲٴ� ����..bat���� %�ν��� ����...
    strURLConvertBAT.clear();
    strRevision.clear();
    strURL.clear();
    
    docXML.LoadFile( strXMLPath );
    nodeInfo  = docXML.FirstChild( "info" );
    nodeEntry = nodeInfo->FirstChild( "entry" );
    nodeURL   = nodeEntry->FirstChild( "url" );
       
    strRevision.append( nodeEntry->ToElement()->Attribute( "revision" ) );
    strURL.append( nodeURL->FirstChild()->Value() );
    strURL.append( "/" );
    strURLConvertBAT = strURL;
    boost::replace_all( strURLConvertBAT, "%", "%%" );    
    
    int iFolderLoc = strURLConvertBAT.rfind( "/", strURLConvertBAT.length() - 2 );

    for(iFolderLoc += 1 ; iFolderLoc < strURLConvertBAT.length() - 1 ; ++iFolderLoc )
    {
        strNowFolder += strURLConvertBAT[iFolderLoc];
    }

    ////////////////////////////////////////////////////////////

    //������ commit�� �ϱ����� ������ stream�� ����.
    //����
    /*
    1. �������� exe�� -> �系 ������ copy
    2. SVN��� + tag�� tag�� ���� ���� ����
    3. ����(�۾�����) -> tag�� copy
    4. �������� commit
    5. tag/Game..._0701../trunk�� �̵�(switch)
    6. exe, pdb add �� -> UTF-8�� commit
    7. ���� ������ switch
    8. out..
    */

    stm << "@echo on" << "\n"
        << "echo Tag Version : "<< SECTION << "_"<< strTimeStamp.c_str() << "\n" // <--tag���� ��� ex)GAMESERVER_070102_0612
        << "svn mkdir " << strSVNURL << "/tags/" << SECTION << "_"<< strTimeStamp.c_str() << " --message \"�±� ������ ���� ���� ����\"" << "\n"
        << "svn copy "<< strURLConvertBAT << " " << strSVNURL << "/tags/" << SECTION << "_"<< strTimeStamp.c_str() << " --message \"�������� ���� tag�� ���� ����\"" << "\n"
        << "svn commit " << "--message \"Now Commit...\" " << "\n"
        << "svn switch " << strSVNURL << "/tags/" << SECTION << "_"<< strTimeStamp.c_str() << "/" << strNowFolder << "\n";
    
    std::string strDecodedURL = DecodeURL( strURL );

    std::ofstream ofstmFile;
    ofstmFile.open("Comment.txt", std::ios::binary | std::ios::out );
    char cBom[3];
    cBom[0] = char(0xEF);
    cBom[1] = char(0xBB);
    cBom[2] = char(0xBF);

    ofstmFile.write( cBom, 3);
    ofstmFile << "pdb, exe Commit" << "\r\n";
    ofstmFile << "URL : " << strDecodedURL << "\r\n";
    ofstmFile << "Revision : " << strRevision;
    ofstmFile.close();

    for( i = 0 ; i < nFileNum ; ++i )
    {
        char szfolder[256];
        char szfile[256];
        sprintf( szfolder,"SourceFolder_%d", i );
        sprintf( szfile,"SourceFile_%d", i );

        stm << "svn add "            
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()            
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".exe" << "\n";

        stm << "svn add "            
            << iniReader.ReadString( SECTION, szfolder, NULL ).c_str()            
            << iniReader.ReadString( SECTION, szfile, NULL )
            << ".pdb" << "\n";
    }

    // 070207. kkurrung. �±� ������ �ڸ�Ʈ ���� �����ϵ��� notepad�� ����.
    stm << "notepad comment.txt" << "\n";
    stm << "svn commit --encoding UTF-8 --file Comment.txt" << "\n" //UTF-8�� ���ڵ��ؼ� Ŀ��..
        << "svn switch " << strURLConvertBAT << "\n"
        << ":Repeat_0 "<< "\n"
        << "del Comment.txt" << "\n"
        << "if exist Comment.txt goto Repeat_0" << "\n"
        << ":Repeat_1 "<< "\n"
        << "del svn.xml" << "\n"
        << "if exist svn.xml goto Repeat_1" << "\n"
        << "goto _out" << "\n"
        << ":_noexist" << "\n"
        << "echo ������ �������� �ʽ��ϴ�." << "\n"
        << "goto _out" << "\n"
        << ":_out" << "\n"
        << "echo �����մϴ�." << "\n"
        << "pause" << "\n"
        << std::endl;

    std::cout << stm.str();

    return 0;
}
