#ifndef ODBC_H
#define ODBC_H "$Id: ODBC.H,v 1.1 2003/05/23 01:18:40 cvs Exp $"

#include <ostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "Serializer/SerBuffer.h"
#include <ToString.h>
#include <atltime.h>    // CTime
#include "KncException.h"
#include "KncSmartPtr.h"
//{{ 2009. 6. 15  ������	���Ƚ�Ʈ��
#include "SecurityString.h"
//}}

SmartPointer( KODBC );

class KODBC
{
public:

#   define SQLOK(result) (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
#   define SQLNODATA(result) (result == SQL_NO_DATA)
#   define THROW_INVALID_QUERY() { throw std::exception("DB Query Exception"); }
#   define FETCH_DATA( varlist ) \
            { \
                try \
                { \
                    m_kODBC >> varlist;\
                } \
                catch( std::exception& ex) \
                { \
                dbg::cout << LOG_HEADER_STR << KncUtil::toWideString( ex.what() ) << END_LOG; \
                } \
                catch( ... ) \
                { \
                dbg::cout << LOG_HEADER_STR << L"Unknown Exception Occured." << END_LOG; \
                } \
            }

#   define FETCH_DATA_PTR( pODBC, varlist ) \
	{ \
		try \
		{ \
			(*pODBC) >> varlist;\
		} \
		catch( std::exception& ex) \
		{ \
			dbg::cout << LOG_HEADER_STR << KncUtil::toWideString( ex.what() ) << END_LOG; \
		} \
		catch( ... ) \
		{ \
			dbg::cout << LOG_HEADER_STR << L"Unknown Exception Occured." << END_LOG; \
		} \
	}

	enum ODBC_INFO
	{
		QUERY_BUFFER_SIZE = 4096,
	};

    DeclToStringW;
    DeclDump;

    KODBC();
    virtual ~KODBC();

	bool DriverConnect( const wchar_t* szConnStr, bool bDBConnStr );
    bool DriverConnectFileDSN( const wchar_t* szDsn );
	//{{ 2009. 6. 15  ������	conn string
	bool DriverConnectString( const KClearString< std::wstring >& kDBConnStr );
	//}}
    bool ReConnect();
    void Close();	
    
	//{{ 2009. 9. 11  ������	������ �����Լ�
	bool Query( const std::wstring& wstrQuery );
	//}}
	//{{ 2012. 04. 17	������	DB���� ���� üũ
	//#ifdef SERV_ALL_DB_QUERY_SUCCESS_CHECK	
	const std::wstring& GetLastQuery() const	{ return m_strLastQuery; }
	//#endif SERV_ALL_DB_QUERY_SUCCESS_CHECK
	//}}
	
	//bool Query(const wchar_t* szFormat, ...);
    //bool QueryWithBlob( KSerBuffer& kbuff, const wchar_t* szFormat, ...);
    
    bool BeginFetch();
    bool Fetch();
    void EndFetch();
    
    KODBC& operator >>(bool& val);
    KODBC& operator >>(char& val);
	KODBC& operator >>(wchar_t& val);
    KODBC& operator >>(short& val);
    KODBC& operator >>(int& val);
	KODBC& operator >>(UINT& val);
    KODBC& operator >>(UCHAR& val);
    KODBC& operator >>(USHORT& val);
    KODBC& operator >>(DWORD& val);
    KODBC& operator >>(char* val);
    KODBC& operator >>(std::string& val);
    KODBC& operator >>(float& val);
    KODBC& operator >>(KSerBuffer& val);
    KODBC& operator >>(time_t& val);            // 040728
    KODBC& operator >>(wchar_t* val);           // 051229
    KODBC& operator >>(std::wstring& val);      // 051229
    KODBC& operator >>(ULONGLONG& val);         // 061009
    KODBC& operator >>(LONGLONG& val);          // 061009

    CRITICAL_SECTION& GetCS() { return m_cs; }

    static void DumpMaxTime( std::wostream& stm );
    static void ResetMaxTime();

//protected:
public:
    enum {
        BLOB_DATA_PARAM_MARKER,     // 040622. blob �����͸� �����ϱ� ���� ����� ���̵�
    };
    SQLHENV             m_hEnv;
    SQLHDBC             m_hDbc;
    SQLHSTMT            m_hStmt;

    USHORT              m_usColumNum;
    CRITICAL_SECTION    m_cs;

public:
    // diagnostic message
    SQLWCHAR            m_szDiagMsg[SQL_MAX_MESSAGE_LENGTH];
    SQLSMALLINT         m_sqlMsgLen;
    SQLWCHAR            m_sqlState[6];
    SQLINTEGER          m_sqlNativeErr;

	// DB Connection Info
	bool				m_bDBConnStr;
    std::wstring        m_strDSN;
    std::wstring        m_strAbsolutePathDSN;
	//{{ 2009. 6. 15  ������	���Ƚ�Ʈ��
	std::wstring		m_strDBConnStr;
	KSecurityString< std::wstring >	m_SecurityDBConnStr;
	//}}

    std::wstring        m_strLastQuery; // ������ ������ �����. ��ġ���� �� �� �ִ� ������ ����� ����.

    static struct KMaxProcessingInfo {
        KncCriticalSection  m_cs;
        std::wstring        m_strQuery;
        DWORD               m_dwTerm;       // �̺�Ʈ �ϳ��� ó���Ϸ��ϱ���� �ɸ� �ð�.
        CTime               m_cTime;
    } ms_kMaxProcessingInfo;

    static bool IsInvalidMarkIn( IN const std::wstring& strData );			// �������� ���ڰ� ���ԵǾ� �ִٸ� true.
	static bool IsInvalidMarkInForLetter( IN const std::wstring& strData ); // �������� ���ڰ� ���ԵǾ� �ִٸ� true. - ������ ���� ����
	static bool IsInvalidMarkInForCheat( IN const std::wstring& strData );	// �������� ���ڰ� ���ԵǾ� �ִٸ� true. - ġƮ ����
	//{{ 2011. 08. 09	������	�ӽ�ID �����
//#ifdef SERV_MACHINE_ID_BLOCK
	static bool IsInvalidMarkInForMachineID( IN const std::wstring& strData );	// �������� ���ڰ� ���ԵǾ� �ִٸ� true. - �ӽ�ID ����
//#endif SERV_MACHINE_ID_BLOCK
	//}}
    static void RemoveInvalidMark( IN OUT std::wstring& strData );
};


//{{ 2012. 04. 17	������	DB���� ���� üũ
//#ifdef SERV_ALL_DB_QUERY_SUCCESS_CHECK
//////////////////////////////////////////////////////////////////////////
// ���Ӽ������� �������� DB���� ȣ�⿡ ���ؼ� Ʈ������� ����������� ���ϴ� ���� ��ü���ֱ� ���� ����ϴ� Ŭ����
// 
class KDBQuerySuccessChecker
{
public:
	KDBQuerySuccessChecker();
	~KDBQuerySuccessChecker();

	void Clear();
	void AddSuccessQuery( IN const std::wstring& wstrQuery );
	void AddFailedQuery( IN const std::wstring& wstrQuery );
	bool IsAllQuerySuccess() const;
	void PrintLogSuccessQuery() const;
	void PrintLogFailedQuery() const;

private:
	std::vector< std::wstring >	m_vecSuccessQuery;
	std::vector< std::wstring > m_vecFailedQuery;
};
//#endif SERV_ALL_DB_QUERY_SUCCESS_CHECK
//}}


#endif


