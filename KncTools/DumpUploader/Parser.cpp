#include "StdAfx.h"
#include ".\parser.h"
#include "IElement.h"
CParser::CParser(void)
{
}

CParser::~CParser(void)
{
}

//bool CParser::Parse( CAutoManager::LSTELEMENT& lst, char* strArgv )
//{
//    CStringEx strParse;
//    CStringEx strToken;
//    strParse = strParse;
//    if( strParse.IsEmpty() )
//        return false;
//    strParse.Trim();
//    if( strParse.IsEmpty() )
//        return false;
//
//    if( strParse.GetAt(0) == '/' ) // ù��° ���ڰ� / �� �����ؾ� �Ѵ�.
//    {
//        strToken = strParse.GetToken( "/");
//        strToken = strParse.GetNextToken( "/" );
//
//        strToken.Trim();
//
//
//        while( !strToken.IsEmpty() )
//        {
//            ElementFactory::GetElement( )
//        }//end while
//
//    }//end if
//
//    ReadDirectoryChangesW
//    
//}