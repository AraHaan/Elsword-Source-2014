#pragma once
#include <thread/Thread.h>
#include <atltime.h>
#include "ThreadManager.h"

#include "Event.h"

// ���� �Ϲ����� ����� loop, dispatch�� �����Ǿ� �ִ�. 
// �̺�Ʈ ���̵� ���� �ڵ鷯 �Լ��� �����ϰ��� �Ѵٸ� 
// KSubThread���� ��ӹް� �ڵ鷯�� �߰��Ѵ�.
class KSubThread : public KThread
{
protected:  // ���� ��ü�� ���������� �ʴ´�.
    KSubThread() {}
public:
    virtual ~KSubThread(void) {}

protected:
    inline void Loop()
    {
        KEventPtr spEvent;

        if( !m_pkThreadMgr->GetKEvent( spEvent ) )
            return;

        if( spEvent == NULL )
        {
            std::cout << "Sub-Thread Terminated." << std::endl;
            SetEvent( m_hKillEvent );
            return;
        }

        DWORD dwElapTime = ::GetTickCount();  // 040428. ó�� �ҿ�ð��� ���Ѵ�.

        ProcessEvent( spEvent );

        dwElapTime = ::GetTickCount() - dwElapTime;

        m_pkThreadMgr->CheckMaxProcessingInfo( dwElapTime, spEvent );
    }

    virtual void Run()     // derived from KThread
    {
        std::cout << "KSubThread::Run(), thread start." << std::endl;

        DWORD ret;

        while( true )
        {
            ret = ::WaitForSingleObject( m_hKillEvent, 1 );

            if( ret == WAIT_OBJECT_0 ) break;

            if( ret == WAIT_TIMEOUT ) Loop();

            else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

        }
    }

    virtual void ProcessEvent( const KEventPtr& spEvent )   = 0;
};

//////////////////////////////////////////////////////////////////////////
// Declare
#define DeclareDBThread( classname ) \
    public: \
        classname( const wchar_t* szConnStr, bool bDBConnStr ) { m_kODBC.DriverConnect( szConnStr, bDBConnStr ); } \
    protected: \
        KODBC               m_kODBC

#define DeclareSPProfiler \
	protected: \
		void ProfilerDump( const std::vector< KStoredProcedureProfiler::KSPInfo >& vecDump ); \
		KStoredProcedureProfiler m_kProfiler

#define DeclareDBThreadWithSP( classname ) \
    public: \
    classname( const wchar_t* szConnStr, bool bDBConnStr ):m_kSP(m_kODBC) { m_kODBC.DriverConnect( szConnStr, bDBConnStr ); } \
    protected: \
    KODBC               m_kODBC

//////////////////////////////////////////////////////////////////////////
// Do Query
// �������Ϸ� ��������� ProfilerHelper�� �ּ�ó�� �ϸ�˴ϴ� - ������
#define DO_QUERY(query,argtype,arg) \
{ \
	std::wstring wstrQuery; \
	try { \
		wstrQuery = boost::str(boost::wformat( query L" " argtype ) arg ); \
	} catch (const boost::io::format_error& e) { \
		START_LOG( cerr, e.what() ) \
			<< BUILD_LOG( query L" " argtype ) \
			<< END_LOG; \
		goto end_proc; \
	} \
	KSPProfileHelper Helper( m_kProfiler, query ); \
	if( !m_kODBC.Query( wstrQuery ) ) \
	{ \
		Helper.QueryFail(); \
		START_LOG( cerr, wstrQuery ) \
			<< BUILD_LOG( m_kODBC.m_strLastQuery ) \
			<< BUILD_LOG( m_kODBC.m_szDiagMsg ) \
			<< BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState ); \
		goto end_proc; \
	} \
}

#define DO_QUERY_NO_ARG(query) \
{ \
	std::wstring wstrQuery = query; \
	KSPProfileHelper Helper( m_kProfiler, query ); \
	if( !m_kODBC.Query( wstrQuery ) ) \
	{ \
		Helper.QueryFail(); \
		START_LOG( cerr, query ) \
			<< BUILD_LOG( m_kODBC.m_strLastQuery ) \
			<< BUILD_LOG( m_kODBC.m_szDiagMsg ) \
			<< BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState ); \
		goto end_proc; \
	} \
}

#define DO_QUERY_NO_PROFILE(query,argtype,arg) \
{ \
	std::wstring wstrQuery; \
	try { \
		wstrQuery = boost::str(boost::wformat( query L" " argtype ) arg ); \
	} catch (const boost::io::format_error& e) { \
		START_LOG( cerr, e.what() ) \
			<< BUILD_LOG( query L" " argtype ) \
			<< END_LOG; \
	goto end_proc; \
	} \
	if( !m_kODBC.Query( wstrQuery ) ) \
	{ \
		START_LOG( cerr, wstrQuery ) \
			<< BUILD_LOG( m_kODBC.m_strLastQuery ) \
			<< BUILD_LOG( m_kODBC.m_szDiagMsg ) \
			<< BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState ); \
		goto end_proc; \
	} \
}

#define DO_QUERY_DB_THREAD( dbID, query ) \
	if( !DoQuery( dbID, boost::str(boost::wformat query).c_str() ) ) \
	{ \
		goto end_proc; \
	}

//{{ 2012. 04. 17	������	DB���� ���� üũ
//#ifdef SERV_ALL_DB_QUERY_SUCCESS_CHECK
#define GET_LAST_QUERY()	m_kODBC.GetLastQuery()
//#endif SERV_ALL_DB_QUERY_SUCCESS_CHECK
//}}

//////////////////////////////////////////////////////////////////////////
// SP Profile
#define IMPL_PROFILER_DUMP(class)	void class::ProfilerDump( const std::vector< KStoredProcedureProfiler::KSPInfo >& vecDump )

#define CALL_PROFILER_DUMP(x) \
	if( m_kProfiler.CheckUpdateTime( x ) == true ) \
	{ \
		std::vector< KStoredProcedureProfiler::KSPInfo > vecDump; \
		m_kProfiler.GetDump( vecDump ); \
		ProfilerDump( vecDump ); \
		m_kProfiler.Clear(); \
	}

