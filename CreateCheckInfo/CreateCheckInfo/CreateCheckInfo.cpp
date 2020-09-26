// CreateCheckInfo.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "SHA1.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/test/utils/foreach.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>

#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment( lib, "libxml_mtd.lib" )
#else
	#pragma comment( lib, "libxml_mt.lib" )
#endif


using namespace std;
using namespace boost;
typedef tokenizer<char_separator<char>> Tokenizer;

struct CheckInfo
{
	std::string m_strFileName;
	std::string m_strSha1;

	CheckInfo( const char* szFileName_ )
		: m_strFileName( szFileName_ )
	{}

	CheckInfo( const std::string& strFileName_ )
		: m_strFileName( strFileName_ )
	{}

	CheckInfo( const char* szFileName_, const char* szSha1_ )
		: m_strFileName( szFileName_ ), m_strSha1( szSha1_ )
	{}

	CheckInfo( const std::string& strFileName_, const std::string& strSha1_ )
		: m_strFileName( strFileName_ ), m_strSha1( strSha1_ )
	{}

	// ����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// CheckInfo�� == �Ǵ��� m_strFileName�� ������ �Ǵ���
	// �׷��� = �������� ��� m_strFileName, m_strSha1 ��θ� assign ��

	bool operator == ( const CheckInfo& rhs_ )
	{
		// ���ϸ��� ������ true�� ����
		if ( 0 == m_strFileName.compare( rhs_.m_strFileName ) )
			return true;
		else
			return false;
	}

	bool operator == ( const string& strFileName_ )
	{
		// ���ϸ��� ������ true�� ����
		if ( 0 == m_strFileName.compare( strFileName_ ) )
			return true;
		else
			return false;
	}
};

struct CompareOnlyFileNameFunc
{
	bool operator()( const CheckInfo& lhs, const CheckInfo& rhs )
	{
		if ( 0 == lhs.m_strFileName.compare( rhs.m_strFileName ) )
			return true;
		else
			false;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	// ���ڰ� 3�� ���� ������ ����
	if ( 3 > argc )
	{
		std::cerr << "Argument's size is wrong!" << std::endl;
		return -1;
	}
	
	// SHA�� ���� kom ���ϵ� �̸�
	std::string strKomFiles	= argv[2];

	// �и�
	char_separator<char> sep( "/" );
	Tokenizer tok( strKomFiles, sep );
	
	std::vector<CheckInfo> vecCheckInfoFromLocalFiles;
	for ( Tokenizer::iterator tItr = tok.begin(); tItr != tok.end(); ++tItr )
	{
		//std::cout << *tItr << std::endl;
		vecCheckInfoFromLocalFiles.push_back( CheckInfo( *tItr ) );	// �и��� ���� vector�� ����
	}

	// kom ���ϵ��� �ִ� ���
	std::string strPath		= argv[1];
	// Local�� �ִ� KOM ������ sha�� ��
	BOOST_TEST_FOREACH( CheckInfo&, val, vecCheckInfoFromLocalFiles )
	{
		std::string strFullFileName = strPath;
		strFullFileName += "\\data\\";
		strFullFileName += val.m_strFileName;

		CSHA1 Sha1;
		if ( Sha1.HashFile( strFullFileName.c_str() ) )
		{
			char aReport[50] = { 0, };
			Sha1.Final();
			Sha1.ReportHash( aReport, CSHA1::REPORT_HEX, true );
			
			//std::cout << aReport << std::endl;
			val.m_strSha1 = aReport;
		}
		// Local�� Sha�� �����ϰ��� �ϴ� kom�� ���ٸ� sha�� empty ���·� �Ͽ� vector�� �־��
	}

	std::string strPathCheckInfo = strPath;
	strPathCheckInfo += "\\checkkom.xml";
	
	/*
	// xml ���� �о����
	std::vector<CheckInfo> vecCheckInfoFromXml;
	stringstream streamXml;
	ifstream inputFileStream( strPathCheckInfo, ios_base::in );
	if ( true == inputFileStream.is_open() )
	{
		streamXml << inputFileStream.rdbuf();
		inputFileStream.close();

		// xml�� ����
		xmlDoc* doc = xmlReadMemory( streamXml.str().data(), streamXml.str().size(), "checksum.xml", NULL, 0 );

		xmlNode* files = xmlDocGetRootElement(doc);
		if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"Files")==0)
		{			
			xmlNode * file = files->children;
			while ( file )
			{
				if ( file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0 )
				{	
					std::string strFileName;
					std::string strSha1;
					xmlAttr * attribute =  file->properties;

					while ( attribute )
					{
						if ( strcmp ( (char*)attribute->name,"Name" ) == 0 && attribute->children )
						{
							strFileName = (char*)attribute->children->content;						
						}
						else if ( strcmp ( (char*)attribute->name,"Sha" ) == 0 && attribute->children )
						{
							strSha1 = (char*)attribute->children->content;
						}
						attribute = attribute->next;
					}
					vecCheckInfoFromXml.push_back( CheckInfo( strFileName, strSha1 ) );
				}
				file = file->next;
			}
		}			
		xmlFreeDoc(doc);
	}	
	
	BOOST_TEST_FOREACH( CheckInfo&, localFileCheckInfo, vecCheckInfoFromLocalFiles )
	{
		if ( localFileCheckInfo.m_strSha1.empty() )
		{
			vector<CheckInfo>::iterator vItrFromXml = find( vecCheckInfoFromXml.begin(), vecCheckInfoFromXml.end(), localFileCheckInfo );

			if ( vecCheckInfoFromXml.end() != vItrFromXml )
			{
				localFileCheckInfo.m_strSha1 = vItrFromXml->m_strSha1;
			}
		}
	}
	//*/

	// xml ���� �����
	std::stringstream stream;
	stream << "<?xml version=\"1.0\"?>" << std::endl << "<Files>" << std::endl;

	BOOST_TEST_FOREACH( CheckInfo&, localFileCheckInfo, vecCheckInfoFromLocalFiles )
	{
		if ( !( localFileCheckInfo.m_strSha1.empty() ) )
		{
			stream << "<File";
			stream << " Name=\"" << localFileCheckInfo.m_strFileName << "\"";
			stream << " Sha=\"" << localFileCheckInfo.m_strSha1 << "\"";
			stream << " />" << std::endl;			

			std::cout << "KomName: " << localFileCheckInfo.m_strFileName << "\t" << "SHA-1: " << localFileCheckInfo.m_strSha1 << std::endl;
		}
		else
		{
			std::cout << localFileCheckInfo.m_strFileName << " does not exist." << std::endl;
		}
	}
	stream << "</Files>";

	ofstream outputFileStream( strPathCheckInfo, ios_base::out );
	if ( true == outputFileStream.is_open() )
	{
		outputFileStream << stream.rdbuf();
		outputFileStream.close();
	}	

	return 0;
}

