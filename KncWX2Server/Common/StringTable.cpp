#include "Stringtable.h"
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <dbg/dbg.hpp>
#include <boost/shared_array.hpp>

KStringTable::KStringTable(void)
: m_strCurrentSection(L"")
{
}

KStringTable::~KStringTable(void)
{
}

void KStringTable::Dump( std::wostream& stm_ )const
{
    if( m_mapSectionTable.empty() )
        return;

    MAPSECTION::const_iterator it = m_mapSectionTable.begin();
    stm_ << L"--- Dump String Table ---" << std::endl;
    while( it != m_mapSectionTable.end() )
    {
        stm_ << boost::wformat( L"[%s]\n") % it->second.m_strSection;
        MAPKEYVALUE::const_iterator itKeyValue = it->second.m_mapKeyValue.begin();
        while( itKeyValue != it->second.m_mapKeyValue.end() )
        {
            stm_ << boost::wformat( L" %s = %s\n" ) % itKeyValue->second.m_strKey % itKeyValue->second.m_strValue;
            ++itKeyValue;
        }       
        ++it;
    }
}

bool KStringTable::LoadIni( IN const wchar_t* szFileName_ )
{
    if(  szFileName_ == NULL )
    {
        START_LOG( cerr, L"���ϸ��� NULL�Դϴ�." );
        return false;
    }

    HANDLE hFile    = NULL;
    DWORD dwFileSize  = 0;
    std::wstring strFullFileName = szFileName_;

    { // Get Full Path and File Size
//        wchar_t szFileName[MAX_PATH];
//        if( GetCurrentDirectory( MAX_PATH, szFileName ) == 0 )
//        {
//            START_LOG( cerr, L"GetCurrentDirectory");
//            return false;
//        }
//
//        strFullFileName = szFileName;
//        strFullFileName += L"\\";
//        strFullFileName += szFileName_;

        hFile=CreateFileW( strFullFileName.c_str(), GENERIC_READ, 0, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL );
        if ( hFile==INVALID_HANDLE_VALUE )
        {
            START_LOG( cerr, L"������ �������� ����")
                << BUILD_LOG( strFullFileName )
                << END_LOG;
            return false;
        }

        dwFileSize=GetFileSize(hFile,NULL);
        CloseHandle(hFile);
    }

    boost::shared_array< wchar_t > spaIniContents ( new wchar_t[dwFileSize+1] );
    DWORD dwRet = GetPrivateProfileSectionNamesW( spaIniContents.get(), dwFileSize, strFullFileName.c_str() ); // ���� �̸� ������.

    if( dwRet > 0 )
    {
        for( unsigned int i = 0 ; i < dwRet-1 ; ++i )
        {
            if( spaIniContents[i] == L'\0' )
                spaIniContents[i] = L'\n';
        }
    }

    std::wstring strSectionNames = spaIniContents.get();

    typedef boost::tokenizer<boost::char_separator<wchar_t,std::wstring::traits_type>,std::wstring::const_iterator,std::wstring > TOKENIZER;
        
    boost::char_separator<wchar_t,std::wstring::traits_type> sep(L"\n");
    TOKENIZER tokens(strSectionNames, sep);
    for (TOKENIZER::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter )
    {
        AddAndBuildSection( (*tok_iter).c_str(), strFullFileName.c_str(), dwFileSize );
    }

    return true;
}

std::wstring KStringTable::GetValue( IN const wchar_t* szSectionName_, IN const wchar_t* szKey_ )
{
    if( szSectionName_ == NULL || szKey_ == NULL )
    {
        START_LOG( cerr, L"�Է� �Ķ������ NULL�� ����" );
        std::wstring strEmpty(L"");
        return strEmpty;
    }

    MITSECTION tmpMITSECTIONTABLE;
    if( !FindSection( szSectionName_, tmpMITSECTIONTABLE ) )
    {
        START_LOG( cerr, L"������ �������� ����" )
            << BUILD_LOG( szSectionName_ )
            << END_LOG;
        std::wstring strEmpty(L"");
        return strEmpty;
    }

    MITKEYVALUE tmpMITSECTION;
    if( !FindKey_value( tmpMITSECTIONTABLE, szKey_, tmpMITSECTION ) )
    {
        START_LOG( cerr, L"Ű�� �������� ����" )
            << BUILD_LOG( szKey_ )
            << END_LOG;
        std::wstring strEmpty(L"");
        return strEmpty;
    }

    return tmpMITSECTION->second.m_strValue;
}

bool KStringTable::SetCurrentSection( IN const wchar_t* szSectionName_ )
{
    if( szSectionName_ == NULL )
    {
        START_LOG( cerr, L"�Է� �Ķ���Ͱ� NULL�̴�." );
        return false;
    }

    MITSECTION tmpMIT;
    if(!FindSection( szSectionName_, tmpMIT ) )
    {
        START_LOG( cerr, L"������ �������� ����" )
            << BUILD_LOG( szSectionName_ )
            << END_LOG;
        return false;
    }

    m_strCurrentSection = szSectionName_;
    return true;
}

std::wstring KStringTable::GetValue( IN const wchar_t* szKey_ )
{
    if( szKey_ == NULL )
    {
        START_LOG( cerr, L"�Է� �Ķ���Ͱ� NULL�̴�." );
        std::wstring strEmpty(L"");
        return strEmpty;
    }
    
    if( m_strCurrentSection.empty() )
    {
        START_LOG( cerr, L"���� ������ �������� �ʾ���" );
        return m_strCurrentSection; // return Empty String
    }

    return GetValue( m_strCurrentSection.c_str(), szKey_ );
}

/* Protected */ 

bool KStringTable::AddAndBuildSection( IN const wchar_t* szSectionName_, IN const wchar_t* szFileName_, IN DWORD dwFileSize_ )
{
    if( szSectionName_ ==NULL ||  szFileName_ == NULL || dwFileSize_ <= 0 )
    {
        START_LOG( cerr, L"�Է� �Ķ������ NULL�� ����" );
        return false;
    }

    Section tmpSection;
    tmpSection.m_strSection = szSectionName_;

    boost::shared_array< wchar_t > spaBuffer( new wchar_t[dwFileSize_] );
    DWORD dwret = GetPrivateProfileSection(tmpSection.m_strSection.c_str(), spaBuffer.get(), dwFileSize_, szFileName_ );
    if( dwret > 0)
    {
        for( unsigned int i = 0 ; i < dwret-1 ; ++i )
        {
            if( spaBuffer[i] == L'\0' )
                spaBuffer[i] = L'\n';
        }
    }

    std::wstring strIniContent = spaBuffer.get();

    typedef boost::tokenizer<boost::char_separator<wchar_t,std::wstring::traits_type>,std::wstring::const_iterator,std::wstring > TOKENIZER;

    boost::char_separator<wchar_t,std::wstring::traits_type> sep(L"\n");
    TOKENIZER tokens(strIniContent, sep);
    for (TOKENIZER::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter )
    {
        AddAndBuildKeyvalue( (*tok_iter).c_str(), tmpSection );
    }
    
    m_mapSectionTable.insert( std::make_pair( tmpSection.m_strSection, tmpSection) );

    return true;
}

bool KStringTable::AddAndBuildKeyvalue( IN const wchar_t* szKey_value_, Section& section_ )
{
    if( szKey_value_ == NULL )
    {
        START_LOG( cerr, L"�Է� �Ķ���Ͱ� NULL�̴�." );
        return false;
    }
    
    std::wstring strIniKeyValue = szKey_value_;
    typedef boost::tokenizer<boost::char_separator<wchar_t,std::wstring::traits_type>,std::wstring::const_iterator,std::wstring > TOKENIZER;

    boost::char_separator<wchar_t,std::wstring::traits_type> sep(L"=");
    TOKENIZER tokens(strIniKeyValue, sep);
    TOKENIZER::iterator tok_iter = tokens.begin();

    Key_value keyValue;

    if( tok_iter == tokens.end() )
        goto _fail_;

    keyValue.m_strKey = *tok_iter;

    if( (++tok_iter) == tokens.end() )
        goto _fail_;

    keyValue.m_strValue = *tok_iter;

    section_.m_mapKeyValue.insert(std::make_pair(keyValue.m_strKey, keyValue));

    return true;

_fail_:
    START_LOG( cerr, L"��ū�� ������ ����." );
    return false;

}

bool KStringTable::FindSection( IN const wchar_t* szSectionName_, OUT MITSECTION& itsection_ )
{
    if( szSectionName_ == NULL )
    {
        START_LOG( cerr, L"�Է� �Ķ���Ͱ� NULL�̴�." );
        return false;
    }

    itsection_ = m_mapSectionTable.find( szSectionName_ );
    if( itsection_ == m_mapSectionTable.end() )
    {
        START_LOG( cerr, L"������ �������� ����" )
            << BUILD_LOG( szSectionName_ )
            << END_LOG;
        return false;
    }

    return true;
}

bool KStringTable::FindKey_value( IN MITSECTION& itSection_, IN const wchar_t* szKeyName_,
                                  OUT MITKEYVALUE& itKeyValue_ )
{
    if( szKeyName_ == NULL )
    {
        START_LOG( cerr, L"�Է� �Ķ���Ͱ� NULL�̴�." );
        return false;
    }

    Section tmpSection = itSection_->second;

    itKeyValue_ = itSection_->second.m_mapKeyValue.find( szKeyName_ );
    if( itKeyValue_ == itSection_->second.m_mapKeyValue.end() )
    {
        START_LOG( cerr, L"Ű�� �������� ����" )
            << BUILD_LOG( szKeyName_ )
            << END_LOG;
        return false;
    }

    return true;
}