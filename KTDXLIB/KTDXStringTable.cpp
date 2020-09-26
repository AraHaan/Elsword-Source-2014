#include "stdafx.h"

#include <KncUtil.h>
#include "KTDXStringTable.h"

ImplementSingleton( CKTDXStringTable );

CKTDXStringTable::CKTDXStringTable()
{
    m_vecGeneralStringTable.clear();
    m_wstrEmpty.clear();
#ifdef NETERROR_KOREAN_HELP
	m_bEnableDebugInfo = false;
#endif NETERROR_KOREAN_HELP
}

CKTDXStringTable::~CKTDXStringTable()
{
}

bool CKTDXStringTable::InitializeStringTable( std::wstring& wstrGeneralStringFileName, std::wstring& wstrScriptStringFileName )
{
    return LoadStringTable( wstrGeneralStringFileName, m_vecGeneralStringTable );
}

std::wstring CKTDXStringTable::GetReplacedString( int iID, char* szParamTypeList, ... )
{
    int iTableSize = ( int )m_vecGeneralStringTable.size();

    ASSERT( iID >= 0 && iID < iTableSize );

    if( iID < 0 || iID >= iTableSize )
    {
        return m_wstrEmpty;
    }

    std::wstring wstrReplacedString;
#ifdef NETERROR_KOREAN_HELP
	if(m_bEnableDebugInfo)
		wstrReplacedString = boost::str( boost::wformat( L"[S%1%]%2%" ) % iID % m_vecGeneralStringTable[iID] );
	else
#endif NETERROR_KOREAN_HELP
		wstrReplacedString = m_vecGeneralStringTable[iID];

    int iParamTypeListSize = strlen( szParamTypeList );

    va_list marker;
    va_start( marker, szParamTypeList );

    for( int i = 0; i < iParamTypeListSize; i++ )
    {
        std::wstringstream wstmReplaceSymbol;
        wstmReplaceSymbol << L"@" << ( i + 1 );

        std::wstringstream wstmReplaceValue;

        // ���Ե� ���� ��Ʈ������ �����.
        switch( szParamTypeList[i] )
        {
        case 'h':       // ��ȣ�� ǥ���ϴ� ������ ��ȯ
            {
                int iParam = va_arg( marker, int );
                if( iParam > 0 )
                {
                    wstmReplaceValue << L"+";
                }
                wstmReplaceValue << iParam;
            }
            break;
        case 'i':       // ������ ��ȯ
            {			
                int iParam = va_arg( marker, int );
                wstmReplaceValue << iParam;
            }
            break;
        case 'e':       // ��ȣ�� ǥ���ϴ� �Ǽ��� ��ȯ
            {
                // �Ҽ��� �� ��°�ڸ������� ǥ���ϰ�, ���� 0�̸� 0�� �ڸ���. 1.50 -> 1.5
                double dParam = va_arg( marker, double );
                wchar_t szNumber[64];
                StringCchPrintfW( szNumber, ARRAY_SIZE( szNumber ), L"%.2f", dParam );
                dParam = _wtof( szNumber );
                if( dParam > 0.0 )
                {
                    wstmReplaceValue << L"+";
                }
                wstmReplaceValue << dParam;
            }
            break;
        case 'f':       // �Ǽ��� ��ȯ
            {
                // �Ҽ��� �� ��°�ڸ������� ǥ���ϰ�, ���� 0�̸� 0�� �ڸ���. 1.50 -> 1.5
                double dParam = va_arg( marker, double );
                wchar_t szNumber[64];
                StringCchPrintfW( szNumber, ARRAY_SIZE( szNumber ), L"%.2f", dParam );
                dParam = _wtof( szNumber );
                wstmReplaceValue << dParam;
            }
            break;
        case 'I':       // ID�� ���ڿ��� ��ȯ
            {
                int iParam = va_arg( marker, int );
                wstmReplaceValue << GetGeneralString( iParam ); 
            }
            break;			
        case 's':       // ���ڿ��� ��ȯ
            {
                std::string strValue = va_arg( marker, char* );
                wstmReplaceValue << KncUtil::toWideString( strValue );
            }
            break;
        case 'S':
            {
                wstmReplaceValue << va_arg( marker, wchar_t* );
            }
            break;
        case 'l':       // std::string ���� ��ȯ
            {
                std::string strValue = va_arg( marker, std::string );
                wstmReplaceValue << KncUtil::toWideString( strValue );
            }
            break;
        case 'L':       // std::wstring ���� ��ȯ
            {
                wstmReplaceValue << va_arg( marker, std::wstring );
            }
            break;
        case 'M':       // ����� ������ �޷� ǥ��
        case 'm':
            {
            }
            break;
        case 'o':       // ����� ������ ���� ǥ��
        case 'O':
            {
            }
            break;
        case 'n':       // ����� ������ ���� ǥ��
        case 'N':
            {
            }
            break;
        }

        std::wstring wstrReplaceSymbol = wstmReplaceSymbol.str().c_str();
        std::wstring wstrReplaceValue = wstmReplaceValue.str().c_str();

        size_t iFindPos = 0;
		size_t iCurrentPos = 0;
        while( ( iFindPos = wstrReplacedString.find( wstrReplaceSymbol, iCurrentPos ) ) != std::wstring::npos )
        {
            wstrReplacedString.replace( iFindPos, wstrReplaceSymbol.size(), wstrReplaceValue );
			iCurrentPos = iFindPos + wstrReplaceValue.length();
        }
    }
    va_end( marker );

    return wstrReplacedString;
}

const std::wstring& CKTDXStringTable::GetGeneralString( int iID )
{
    int iTableSize = ( int )m_vecGeneralStringTable.size();

    // STR_ID_EMPTY ������ -1�� �Ѿ�� ���� �ִ�.
    ASSERT( iID >= -1 && iID < iTableSize );

    if( iID < 0 || iID >= iTableSize )
    {
        return m_wstrEmpty;
    }

    return m_vecGeneralStringTable[iID];
}


const std::wstring& CKTDXStringTable::GetScriptString( int iID )
{
	int iTableSize = ( int )m_vecScriptStringTable.size();

    ASSERT( iID >= 0 && iID < iTableSize );

    if( iID < 0 || iID >= iTableSize )
    {
        return m_wstrEmpty;
    }

    return m_vecScriptStringTable[iID];

}

bool CKTDXStringTable::LoadStringTable( std::wstring& wstrFileName, std::vector< std::wstring >& vecStringTable )
{
    vecStringTable.clear();

    // �����ڵ� ������ �����Ƿ� ���̳ʸ� ���� �����Ѵ�.
	FILE* fp = NULL;
#ifdef	CONVERSION_VS
	_wfopen_s( &fp, wstrFileName.c_str(), L"rb" );
#else	CONVERSION_VS
	fp = _wfopen( wstrFileName.c_str(), L"rb" );
#endif	CONVERSION_VS

    ASSERT( fp );
    if( !fp )
    {
        return false;
    }

    // UTF-16LE�� BOM( Byte Order Mark )�� �پ� �Ѵ´�.
    int iRetFSeek = fseek( fp, 2, SEEK_CUR );
    ASSERT( iRetFSeek == 0 );
    if( iRetFSeek != 0 )
    {
        return false;
    }

    // string table ���� �� ���� ������ ������ ���� ����̴�.
    // [index]\t[string]\r\n
    const int ciMaxStringLength = 2048;
    wchar_t wszStringRead[ciMaxStringLength];
    int iIndex = 0;
    while( fgetws( wszStringRead, ciMaxStringLength, fp ) )
    {
        std::wstring wstrStringRead( wszStringRead );
        // �ִ� ��Ʈ�� ���̸�ŭ �о��ٸ� ������ �ִ��� �ǽ��غ��� �Ѵ�.
        ASSERT( ( int )wstrStringRead.size() < ( ciMaxStringLength - 1 ) );

		std::wstring wstrFullString;
		
		//09. 07. 03 ������  XOR��ȣȭ �� ���� ��ȣȭ
		size_t iEnd = wstrStringRead.find( L"\r\n" );
		if( iEnd != std::wstring::npos )
		{
			wstrStringRead.erase( iEnd );
		}		
		
		short iKey = 16;
		int nSize = wstrStringRead.length();		
		for ( int i = 0; i < nSize; i++ ) 
		{               
			wstrStringRead[i] = wstrStringRead[i] ^ iKey;
		} 

        // tab �� �о index�� �����س���.
        size_t iTabPos = wstrStringRead.find( L"\t" );
        std::wstring wstrIndex = wstrStringRead.substr( 0, iTabPos );
        int iIndexRead = _wtoi( wstrIndex.c_str() );

        std::wstring wstrContent;
        // ������ index�� ����� �����Ǿ� �ִ��� �˻��Ѵ�.
        ASSERT( iIndexRead == iIndex );
        if( iIndexRead == iIndex )
        {
            // tab �� ã�� ��쿡�� �� ���� ������ �о�� �Ѵ�.
            if( iTabPos != std::wstring::npos )
            {
				//09. 07. 03 ������  �̸� ���ֱ� ������ ����
                /*size_t iEnd = wstrStringRead.find( L"\r\n" );
                if( iEnd != std::wstring::npos )
                {
                    wstrStringRead.erase( iEnd );
                }*/

                ASSERT( iTabPos < wstrStringRead.size() );
                // tab �ڿ� �ִ� �͵鸸 �д´�.
                if( ( iTabPos + 1 ) < wstrStringRead.size() )
                {
                    wstrContent = wstrStringRead.substr( iTabPos + 1 );
                }
            }
        }

        // \n ���� ǥ���� �κп� ������ �ٹٲ� ���ش�.
        REPLACE_PART_OF_STRING( wstrContent, L"\\n", L"\r\n" );

        // ���� ����ǥ�� �ִ� ���
        //REPLACE_PART_OF_STRING( wstrContent, L"\\\'", L"\'" );

        // ū ����ǥ�� �ִ� ���
        //REPLACE_PART_OF_STRING( wstrContent, L"\\\"", L"\"" );

        vecStringTable.push_back( wstrContent );
        iIndex++;
    }

    int iRetFClose = fclose( fp );
    ASSERT( iRetFClose == 0 );

    return true;
}