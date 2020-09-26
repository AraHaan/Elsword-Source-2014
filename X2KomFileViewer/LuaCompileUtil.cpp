#include "StdAfx.h"
#include ".\luacompileutil.h"
#include <windows.h>
#include <process.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>

#include <string>
#include <iostream>

long GetFileSize(const char* strFileName)
{
	FILE* file = fopen(strFileName,"rb");
	if(file == NULL)
		return -1;
	long size = 0;
	fseek(file,0,SEEK_END);
	size = ftell(file);
	fclose(file);

	return size;
}

using namespace std;


CLuaCompileUtil::CLuaCompileUtil(void)
{	
}

CLuaCompileUtil::~CLuaCompileUtil(void)
{
}

bool CLuaCompileUtil::Compile( const std::wstring& wstrFromFilePath, const std::wstring& wstrToFilePath, const std::wstring& wstrLuacCommand, const std::wstring& wstrAnsiToUTF8Command )
{
	if( wstrFromFilePath.empty() )
		return false;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( wstrFromFilePath.c_str(), drive, dir, fname, ext );

	// ������ ����� ��� ���
	std::wstring wstrToFileFullPath = wstrToFilePath;	
	wstrToFileFullPath += L"\\";
	wstrToFileFullPath += fname;
	wstrToFileFullPath += ext;
	
	MakeUpperCase( ext );
	if( wcscmp( ext, L".LUA" ) != 0 )
		return false;

	// ������ ������ ������ ���ؼ�
	_wchmod( wstrToFileFullPath.c_str(), _S_IREAD | _S_IWRITE );

	// ���� ���� ����
	DeleteFile( wstrToFileFullPath.c_str() );

	// lua ������	
	_wspawnlp( _P_WAIT, wstrLuacCommand.c_str(), L"luac.exe", L"-o", wstrToFileFullPath.c_str(), wstrFromFilePath.c_str(), NULL );
	//_wspawnlp( _P_WAIT, wstrLuacCommand.c_str(), L"luac.exe", L"-o", strBinFileName.c_str(), strSrcFileName.c_str(), NULL );



    // UTF-8 �����̹Ƿ� ���� �տ� BOM�� ���δ�
    _wspawnlp( _P_WAIT, wstrAnsiToUTF8Command.c_str(), L"ansitoutf8.exe", L"-f", wstrToFileFullPath.c_str(), NULL );





	// ������ ����� �ƴ��� Ȯ��
#include <fstream>
	std::string strTemp;
	ConvertWCHARToChar( strTemp, wstrToFileFullPath.c_str() );
	std::wifstream ifs( strTemp.c_str() );
	return ifs.good();
}

// szPath�� file �̸��� �ƴ� directory~. ���� \�� ������ ��
bool CLuaCompileUtil::Compile( const std::wstring& szPath, 
							   const std::wstring& wstrLuacCommand, 
                               const std::wstring& wstrAnsiToUTF8Command,
							   bool bSubDir, 
                               std::map< std::wstring, MemberFileInfo >& mapLuaFiles, 
							   bool bCopyFile /*= false*/ )
{
	if( szPath.empty() )
		return false;
	
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( szPath.c_str(), drive, dir, fname, ext );

	// ������ ����� �ӽ�����
	std::wstring strLuaTempPath = drive;
	strLuaTempPath += L"\\LuaTemp\\";

	// �ӽ����� ����
	CreateDirectory( strLuaTempPath.c_str(), NULL );

	std::wstring strRootDir = szPath;
	strRootDir += L"\\";

	std::wstring strFindPath = strRootDir;
	strFindPath += L"*.*";

	hSearch = FindFirstFile( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return false;

	do
	{
		if( 0 != wcscmp(fd.cFileName, L".") && 0 != wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( bSubDir == true )
				{
					wstring strSubDir = strRootDir;
					strSubDir += fd.cFileName;
					//strSubDir += L"\\";
					Compile( strSubDir.c_str(), wstrLuacCommand, wstrAnsiToUTF8Command, bSubDir, mapLuaFiles, bCopyFile );
				}
			}
			else
			{
				//_chdir( szPath );

				_wsplitpath( fd.cFileName, NULL, NULL, fname, ext );
				MakeUpperCase( ext );

				if( wcscmp( ext, L".LUA" ) == 0 )
				{
					wstring strSrcFileName = strRootDir;
					strSrcFileName += fd.cFileName;

					wstring strBinFileName = strLuaTempPath;
					strBinFileName += fname;
					strBinFileName += L".lua";

					// ������ �Ұ��ΰ�? ī�� �Ұ��ΰ�?
					if( bCopyFile == false )
                    {
						_wspawnlp( _P_WAIT, wstrLuacCommand.c_str(), L"luac.exe", L"-o", strBinFileName.c_str(), strSrcFileName.c_str(), NULL );

                        _wspawnlp( _P_WAIT, wstrAnsiToUTF8Command.c_str(), L"ansitoutf8.exe", L"-f", strBinFileName.c_str(), NULL );
                    }
					else
						CopyFileW( strSrcFileName.c_str(), strBinFileName.c_str(), false );

					std::string strFileNameForSize;
					ConvertWCHARToChar( strFileNameForSize, strBinFileName.c_str() );

					// �����ϵ� Lua Script ���� ���� ���
					MemberFileInfo memberFile;
					memberFile.m_wstrFileName = fd.cFileName;
					memberFile.m_iFileSize	  = GetFileSize( strFileNameForSize.c_str() ) / 1024; // byte -> kilobyte
					memberFile.m_iFileState	  = FS_NONE;
					memberFile.m_wstrRealPath = strBinFileName; // ������ ���� �ִ� ���

					mapLuaFiles.insert( std::make_pair( memberFile.m_wstrFileName, memberFile ) );

					_wchmod( strBinFileName.c_str(), _S_IREAD | _S_IWRITE ); // ���� ������ ���ؼ�
				}							
			}			
		}
	} while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
	return true;
}

bool CLuaCompileUtil::TextFileCopy( const std::wstring& szPath, bool bSubDir, std::map< std::wstring, MemberFileInfo >& mapTextFiles )
{
	if( szPath.empty() )
		return false;

	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;

	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( szPath.c_str(), drive, dir, fname, ext);

	// Text���� ���� �ӽ�����
	std::wstring strLuaTempPath = drive;
	strLuaTempPath += L"\\LuaTemp\\";

	// �ӽ����� ����
	CreateDirectory( strLuaTempPath.c_str(), NULL );

	std::wstring strRootDir = szPath;
	strRootDir += L"\\";

	std::wstring strFindPath = strRootDir;
	strFindPath += L"\\*";

	hSearch = FindFirstFile( strFindPath.c_str(), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
		return false;

	do
	{
		if( 0 != wcscmp(fd.cFileName, L".") && 0 != wcscmp(fd.cFileName, L"..") && wcscmp(fd.cFileName, L".svn") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( bSubDir == true )
				{
					wstring strSubDir = strRootDir;
					strSubDir += fd.cFileName;
					strSubDir += L"\\";
					TextFileCopy( strSubDir.c_str(), bSubDir, mapTextFiles );
				}
			}
			else
			{
				//_chdir( szPath );

				_wsplitpath( fd.cFileName, NULL, NULL, fname, ext );
				MakeUpperCase( ext );

				if( wcscmp( ext, L".TXT" ) == 0 )
				{
					wstring strSrcFileName = strRootDir;
					strSrcFileName += fd.cFileName;

					wstring strBinFileName = strLuaTempPath;
					strBinFileName += fname;
					strBinFileName += L".txt";

					// Text���� ī��
					CopyFileW( strSrcFileName.c_str(), strBinFileName.c_str(), false );

					std::string strFileNameForSize;
					ConvertWCHARToChar( strFileNameForSize, strBinFileName.c_str() );

					// Text ���� ���� ���
					MemberFileInfo memberFile;
					memberFile.m_wstrFileName = fd.cFileName;
					memberFile.m_iFileSize	  = GetFileSize( strFileNameForSize.c_str() ) / 1024; // byte -> kilobyte
					memberFile.m_iFileState	  = FS_NONE;
					memberFile.m_wstrRealPath = strBinFileName; // ������ ���� �ִ� ���

					mapTextFiles.insert( std::make_pair( memberFile.m_wstrFileName, memberFile ) );

					_wchmod( strBinFileName.c_str(), _S_IREAD | _S_IWRITE ); // ���� ������ ���ؼ�
				}				
			}			
		}
	} while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
	return true;
}