#pragma once

#include "DBThreadManager.h"
#include "ToString.h"
#include <KNCSingleton.h>

class KDBLayer
{
    DeclToStringW;
    DeclDump;
    DeclareSingleton( KDBLayer );

protected:
    KDBLayer(void);

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum
    {
#   include "DBConnectionInfo_def.h"
    };

    virtual ~KDBLayer(void) {}

    virtual void RegToLua();
    bool AddDBConnectionInfo_LUA( int iDBConnection, const char* szDSN, int nThread, bool bDBConnStr );
	//{{ 2012. 1. 9	������		DB���� ��ȣ ����ȭ
#ifdef SERV_DB_CONNECTION_SECURITY
	bool AddDBConnectionByString( IN const int iDBType, IN const std::wstring& wstrDBConnectionString, IN const int iThreadCount );
	bool SetManagerDBConnectionInfo_LUA( int iServerClass, const char* szDSN, bool bDBConnStr );
#endif SERV_DB_CONNECTION_SECURITY
	//}}
	void SetSMSEnable_LUA( bool bEnable );

    void Init();
    void ShutDown( DWORD dwWaitTime );   // ���� ���ð��� �и������� �޴´�.
    int GetTotalEventNum();     // ��� db thread mgr ���� ���� event ���� ���� (����ó���� ����.)

    void QueueingEvent( const KEventPtr& spEvent );

	//{{ 2009. 12. 15  ������	��������
	virtual void DumpToLogFile();
	//}}

protected:
    inline int GetTotalThreadNum();    // ��� db thread mgr ���� ���� thread ���� ���� (����ó���� ����.)

public:
    static const char*                      ms_szDBConnection[];

protected:
    std::map< int, KDBThreadManagerPtr >    m_mapDBAgent;
	bool									m_bSMSEnable;	//�߸��� ������� ���� �����߻��� ���ɼ� �־..
	//{{ 2012. 1. 9	������		DB���� ��ȣ ����ȭ
#ifdef SERV_DB_CONNECTION_SECURITY
	boost::shared_ptr< void >				m_spEventAck;
#endif SERV_DB_CONNECTION_SECURITY
	//}}
};

DefSingletonInline( KDBLayer );
