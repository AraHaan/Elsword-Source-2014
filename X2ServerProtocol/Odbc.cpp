#include "stdafx.h"

#pragma   warning( disable :4146 )

#include "ODBC.h"
#include "dbg/dbg.hpp"
#include <Thread/Locker.h>
#include <KncUtil.h>

KODBC::KMaxProcessingInfo KODBC::ms_kMaxProcessingInfo = {
    KncCriticalSection(), L"�ʱ����", 0, CTime::GetCurrentTime()
};

KODBC::KODBC()
{
    m_hEnv          = SQL_NULL_HENV;
    m_hDbc          = SQL_NULL_HDBC;
    m_hStmt         = SQL_NULL_HSTMT;

    m_usColumNum    = 0;
    memset(m_szDiagMsg, 0, SQL_MAX_MESSAGE_LENGTH);

    InitializeCriticalSection(&m_cs );

} // KODBC()


KODBC::~KODBC()
{
    Close();
    DeleteCriticalSection(&m_cs );
} // ~KODBC()


bool KODBC::DriverConnect( const wchar_t* szDsn_ )
{
	std::wstring strDsn_ = szDsn_;

    if( m_strDSN.size() <= 0 )
    {
        wchar_t buff[MAX_PATH] = {0};
        ::GetCurrentDirectoryW( MAX_PATH, buff );

        m_strDSN = L"FILEDSN=";
        m_strDSN += buff;
        m_strDSN += L"\\";
        m_strDSN += strDsn_;

        m_strAbsolutePathDSN = L"";
        m_strAbsolutePathDSN += buff;
        m_strAbsolutePathDSN += L"\\";
        m_strAbsolutePathDSN += strDsn_;
    }

    SQLWCHAR InCon[255];
    SQLWCHAR OutCon[255];
    SQLSMALLINT cbOutCon;
    SQLRETURN Ret;

    // allocate environment handle.
    Ret = ::SQLAllocHandle( SQL_HANDLE_ENV, 
        SQL_NULL_HANDLE, 
        &m_hEnv );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    // set the ODBC version environment attribute.
    Ret = ::SQLSetEnvAttr( m_hEnv, 
        SQL_ATTR_ODBC_VERSION, 
        ( SQLPOINTER )SQL_OV_ODBC3, 
        SQL_IS_INTEGER );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    //allocate connecton handle.
    Ret = ::SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    //dsn������ �̿��ؼ� �����Ѵ�
    wsprintfW( ( wchar_t* )InCon, L"%s", m_strDSN.c_str() );
    Ret = SQLDriverConnectW( m_hDbc, NULL, ( SQLWCHAR* )InCon, sizeof( InCon ), OutCon,
        sizeof( OutCon ), &cbOutCon, SQL_DRIVER_NOPROMPT );

    //�����˻�
    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }

    //����ڵ� �Ҵ�
    Ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );

    if( !SQLOK( Ret ) )
    {
        START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret ) 
            << BUILD_LOG( m_strDSN )
            << END_LOG;

        return false;
    }


    dbg::cout << L"DriverConnect() success. m_strDSN : " << m_strDSN << dbg::endl;

    return true;
}

bool KODBC::ReConnect()
{
    Close();

    // DB������ �õ��� �غ���.  [7/26/2004]
    //if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
    // m_strDSN�� ReConnect�� �Ҹ���, NULL�ϼ� ����. �׷��� �ǹ̾��� parameter�� �Ѱ��ش�. 
    if( !DriverConnect( m_strDSN.c_str() ) )
    {
        dbg::cerr << L"������ �õ� ����" << END_LOG;
        return false;
    }
    else
    {
        START_LOG( cout, L"������ ����" );
        return true;
    }
}

void KODBC::Close()
{
    if( SQL_NULL_HSTMT != m_hStmt )
    {
        ::SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
        m_hStmt = SQL_NULL_HSTMT;
    } // if
    
    if( SQL_NULL_HDBC != m_hDbc )
    {
        SQLDisconnect( m_hDbc );
        ::SQLFreeHandle( SQL_HANDLE_DBC, m_hDbc );
        m_hDbc = SQL_NULL_HDBC;
    } // if
    
    if( SQL_NULL_HENV != m_hEnv )
    {
        ::SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
        m_hEnv = SQL_NULL_HENV;
    } // if

    m_usColumNum    = 0;
} // Close()


bool KODBC::Query(const wchar_t* szFormat_, ...)
{
    wchar_t     szQuery[4096];
    va_list     vaList;
    SQLRETURN   ret;
    
    va_start( vaList, szFormat_ );
    vswprintf( szQuery, szFormat_, vaList );
    va_end( vaList );
    
    DWORD dwElapTime = ::GetTickCount();  // 051001. florist. ���� ���� �ɸ� ���� �ð��� ����Ѵ�.

    // �ش� query�� ���� row�� ���� ��� ret�� SQL_NO_DATA�� return �ȴ�.
    ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS );
    
//{{ Modified by ct 2003-04-30 ���� 03:53:45
    SQLRETURN     ret2;
    ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
                          m_hStmt, 
                          1, 
                          m_sqlState, 
                         &m_sqlNativeErr,
                          m_szDiagMsg, 
                          sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
                         &m_sqlMsgLen );
//}} Modified by ct 2003-04-30 ���� 03:53:45

    // godsend odbc���� ���н� �ٽ� �����ϰ� ������ �ٽ� ����  [7/22/2004]
    if ( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE )
    {
        wchar_t* err1 = L"08S01";  // Communication link failure
        wchar_t* err2 = L"24000";  // Invalid cursor state

        // godsend Ŀ�� ������ DB���� �ȵ� ���� �ΰ��� ��� �ٽ� ����  [7/26/2004]
        if( wcscmp( (const wchar_t*)m_sqlState, err1) == 0 || wcscmp( (const wchar_t*)m_sqlState, err2) == 0 ) 
        {
            dbg::cerr << szQuery << L"(err:" << (const wchar_t*)m_sqlState << L")" << END_LOG;

            // godsend ������ ���´�. 
            Close();
            
            // DB������ �õ��� �غ���.  [7/26/2004]
            //if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
            // godsend Connect -> DriverConnect�� ���� [9/6/2005]
            if( !DriverConnect( m_strDSN.c_str() ))
            {
                dbg::cerr << L"������ �õ� ����" << END_LOG;
                return false;
            }
            else
            {
                ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS );
            }
        }
    }

    dwElapTime = ::GetTickCount() - dwElapTime;

    {   // lock�� �ɸ��� ������ �����ϴ� scope
        KLocker lock( ms_kMaxProcessingInfo.m_cs );

        if( dwElapTime > ms_kMaxProcessingInfo.m_dwTerm )
        {
            ms_kMaxProcessingInfo.m_strQuery    = szQuery;
            ms_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
            ms_kMaxProcessingInfo.m_dwTerm      = dwElapTime;

            START_LOG( clog2, L"���� �ִ� �ҿ�ð� ����." )
                << BUILD_LOG( ms_kMaxProcessingInfo.m_strQuery )
                << BUILD_LOG( ms_kMaxProcessingInfo.m_dwTerm );
        }

    }   // lock�� �ɸ��� ������ �����ϴ� scope

    return SQLOK(ret);
} // Query()

bool KODBC::QueryWithBlob( KSerBuffer& kbuff_, const wchar_t* szFormat_, ... )
{
    wchar_t     szQuery[4096];
    va_list     vaList;
    SQLRETURN   ret;

    va_start( vaList, szFormat_ );
    vswprintf( szQuery, szFormat_, vaList );
    va_end( vaList );

    DWORD dwElapTime = ::GetTickCount();  // 051001. florist. ���� ���� �ɸ� ���� �ð��� ����Ѵ�.

    // binding parameter
    SQLINTEGER cbBlob = SQL_LEN_DATA_AT_EXEC( kbuff_.GetLength() );
    ::SQLBindParameter( m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
        kbuff_.GetLength(), 0, (SQLPOINTER)BLOB_DATA_PARAM_MARKER, 0, &cbBlob );

    // �ش� query�� ���� row�� ���� ��� ret�� SQL_NO_DATA�� return �ȴ�.
    ret = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS );

    if( ret == SQL_NEED_DATA )
    {
        SQLPOINTER pToken;
        ret = ::SQLParamData( m_hStmt, &pToken );
        while( ret == SQL_NEED_DATA ) {
            if( (int)pToken == BLOB_DATA_PARAM_MARKER ) 
            {
                ::SQLPutData( m_hStmt, (PTR)kbuff_.GetData(), kbuff_.GetLength() );
            }
            ret = ::SQLParamData( m_hStmt, &pToken );
        }
    }

    SQLRETURN     ret2;
    ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
                        m_hStmt, 
                        1, 
                        m_sqlState, 
                        &m_sqlNativeErr,
                        m_szDiagMsg, 
                        sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
                        &m_sqlMsgLen );

    dwElapTime = ::GetTickCount() - dwElapTime;

    {   // lock�� �ɸ��� ������ �����ϴ� scope

        KLocker lock( ms_kMaxProcessingInfo.m_cs );

        if( dwElapTime > ms_kMaxProcessingInfo.m_dwTerm )
        {
            ms_kMaxProcessingInfo.m_strQuery    = szQuery;
            ms_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
            ms_kMaxProcessingInfo.m_dwTerm      = dwElapTime;

            START_LOG( clog2, L"���� �ִ� �ҿ�ð� ����." )
                << BUILD_LOG( ms_kMaxProcessingInfo.m_strQuery )
                << BUILD_LOG( ms_kMaxProcessingInfo.m_dwTerm );
        }

    }   // lock�� �ɸ��� ������ �����ϴ� scope

    return SQLOK(ret);
} // Query()


bool KODBC::BeginFetch()
{
    SQLRETURN   ret;
    m_usColumNum = 0;
    ret = ::SQLFetch( m_hStmt );
    
    if( !SQLOK(ret) )
    {
        EndFetch();

        SQLRETURN     ret2;
        ret2 = SQLGetDiagRecW( SQL_HANDLE_STMT, 
            m_hStmt, 
            1, 
            m_sqlState, 
            &m_sqlNativeErr,
            m_szDiagMsg, 
            sizeof(m_szDiagMsg) / sizeof(SQLWCHAR), 
            &m_sqlMsgLen );

        return false;
    }

    return true;
}


bool KODBC::Fetch()
{
    m_usColumNum = 0;
    SQLRETURN ret = ::SQLFetch( m_hStmt );

    if( ret == SQL_NO_DATA )
        EndFetch();

    return SQLOK(ret);
} // Fetch()


void KODBC::EndFetch()
{
    if( SQL_NULL_HSTMT != m_hStmt )
        ::SQLCloseCursor( m_hStmt );
}


KODBC& KODBC::operator >>(bool& val_)
{
    char        val;
    SQLRETURN   ret;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_STINYINT, 
                        &val, 
                        sizeof(bool), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    val_ = !(val == 0);
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(char& val_)
{
    SQLRETURN   ret;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_STINYINT, 
                       &val_, 
                        sizeof(char), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(wchar_t& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData( m_hStmt, 
		m_usColumNum, 
		SQL_C_WCHAR, 
		&val_, 
		sizeof(wchar_t), 
		0);
	if( !SQLOK(ret) )
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >>(short& val_)
{
    SQLRETURN   ret;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SHORT, 
                        &val_, 
                        sizeof(short), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()

KODBC& KODBC::operator >>(int& val_)
{
    SQLRETURN   ret;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SLONG, 
                        &val_, 
                        sizeof(int), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()

KODBC& KODBC::operator >>(UCHAR& val_)
{
    SQLRETURN   ret;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_UTINYINT, 
                        &val_, 
                        sizeof(UCHAR), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(USHORT& val_)
{
    SQLRETURN   ret;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SSHORT, 
                        &val_, 
                        sizeof(USHORT), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(DWORD& val_)
{
    SQLRETURN   ret;
    
    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_SLONG, 
                        &val_,
                        sizeof(DWORD), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(ULONGLONG& val_)
{
    SQLRETURN   ret;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
        m_usColumNum, 
        SQL_C_SBIGINT, 
        &val_, 
        sizeof(ULONGLONG), 
        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
}

KODBC& KODBC::operator >>(LONGLONG& val_)
{
    SQLRETURN   ret;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
        m_usColumNum, 
        SQL_C_SBIGINT, 
        &val_, 
        sizeof(LONGLONG), 
        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
}


KODBC& KODBC::operator >>(std::string& val_)
{
    char acBuffer[1024] = {0, };
    
    *this >> acBuffer;
    val_ = acBuffer;
    
    return *this;
} // operator >>()

KODBC& KODBC::operator >>(char* val_)
{
    SQLRETURN   ret;
    int         nLen;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    SQLColAttribute( m_hStmt, 
                     m_usColumNum, 
                     SQL_DESC_OCTET_LENGTH, 
                     0, 0, 0, 
                    &nLen );
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_CHAR, 
                        val_, 
                        1024, 
                        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData�� ���� �ʵ尡 NULL�̴��� SQL_SUCCESS�� �����Ѵ�.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if
    
    return *this;
} // operator >>()


KODBC& KODBC::operator >>(std::wstring& val_)
{
    wchar_t acBuffer[1024] = {0, };

    *this >> acBuffer;
    val_ = acBuffer;

    return *this;
} // operator >>()


KODBC& KODBC::operator >>(wchar_t* val_)
{
    SQLRETURN   ret;
    int         nLen;
    SQLINTEGER  LenBin;

    m_usColumNum++;
    SQLColAttribute( m_hStmt, 
        m_usColumNum, 
        SQL_DESC_OCTET_LENGTH, 
        0, 0, 0, 
        &nLen );
    ret = ::SQLGetData( m_hStmt, 
        m_usColumNum, 
        SQL_C_WCHAR, 
        val_, 
        1024, 
        &LenBin);

    if( LenBin == SQL_NULL_DATA )   // SqlGetData�� ���� �ʵ尡 NULL�̴��� SQL_SUCCESS�� �����Ѵ�.
        return *this;

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(float& val_)
{
    SQLRETURN   ret;

    m_usColumNum++;
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_FLOAT, 
                        &val_, 
                        sizeof(float), 
                        0);
    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
} // operator >>()

KODBC& KODBC::operator >>(KSerBuffer& val_)
{
    SQLRETURN   ret;
    char        cbuff[1024];
    SQLINTEGER  LenBin;

    m_usColumNum++;
    val_.Clear();

    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_BINARY, 
                        cbuff, 
                        sizeof(cbuff),
                        &LenBin);

    while( ret != SQL_NO_DATA ) {

        if( LenBin == SQL_NULL_DATA )   // SqlGetData�� ���� �ʵ尡 NULL�̴��� SQL_SUCCESS�� �����Ѵ�.
            return *this;

        val_.Write( cbuff, LenBin );

        ret = ::SQLGetData( m_hStmt, 
                            m_usColumNum, 
                            SQL_C_BINARY, 
                            cbuff, 
                            sizeof(cbuff),
                            &LenBin);
    }


    if( ret != SQL_NO_DATA )
    {
        THROW_INVALID_QUERY();
    } // if

    return *this;
}

#include <time.h>

KODBC& KODBC::operator >>(time_t& val_)
{
    SQLRETURN   ret;
    TIMESTAMP_STRUCT sqlTime;

    m_usColumNum++;
 
    ret = ::SQLGetData( m_hStmt, 
                        m_usColumNum, 
                        SQL_C_TYPE_TIMESTAMP, 
                        &sqlTime, 
                        sizeof(sqlTime),
                        0);

    if( !SQLOK(ret) )
    {
        THROW_INVALID_QUERY();
    } // if

    // TIMESTAMP_STRUCT -> struct tm -> time_t
    struct tm time = {0};
    time.tm_year    = sqlTime.year - 1900;
    time.tm_mon     = sqlTime.month - 1;
    time.tm_mday    = sqlTime.day;
    time.tm_hour    = sqlTime.hour;
    time.tm_min     = sqlTime.minute;
    time.tm_sec     = sqlTime.second;

    val_ = mktime( &time );
    if( val_ == (time_t)-1 )
    {
        THROW_INVALID_QUERY();
    }

    return *this;
}


// 041115
ImplToStringW( KODBC )
{
   //SectionName�� ���´�.
    wchar_t szSectionName[10] = {0};
   GetPrivateProfileSectionNamesW( szSectionName, 10, m_strAbsolutePathDSN.c_str() );

   //Address Data�� ���´�.
   wchar_t szAddress[100] = {0};
   GetPrivateProfileStringW( szSectionName, L"Address", 0, szAddress, 100, m_strAbsolutePathDSN.c_str());
   
   //����Ѵ�.
   
   return stm_ << std::endl
               << TOSTRINGW( szAddress )
               << TOSTRINGW( m_strDSN );
}

void KODBC::DumpMaxTime( std::wostream& stm_ )
{
    KLocker lock( ms_kMaxProcessingInfo.m_cs );

    stm_ << L" -- Max Processing query -- " << std::endl
         << L"  query   : " << ms_kMaxProcessingInfo.m_strQuery.c_str() << std::endl
         << L"  elapsed : " << ms_kMaxProcessingInfo.m_dwTerm << std::endl
         << L"  execute : " << (const wchar_t*)ms_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KODBC::ResetMaxTime()
{
    KLocker lock( ms_kMaxProcessingInfo.m_cs );

	std::string strQuery;

    ms_kMaxProcessingInfo.m_dwTerm = 0;
    ms_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
    strQuery = (CStringA)ms_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
    strQuery.append( " �� ������ �ʱ�ȭ �Ǿ����ϴ�." );
	ms_kMaxProcessingInfo.m_strQuery = KncUtil::toWideString(strQuery.c_str(), strQuery.length());
}

ImplOstmOperatorW2A( KODBC );
ImplOstmOperatorW( KODBC );

