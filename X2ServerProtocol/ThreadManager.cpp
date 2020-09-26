#include "StdAfx.h"

#include "ThreadManager.h"
//#include "odbc/Odbc.h"

//ImplOstmOperatorW( KThreadManager );
NiImplementRootRTTI( KThreadManager );
ImplToStringW( KThreadManager )
{
    { // locking scope

        KLocker lock( m_csEventQueue );
        KLocker lock2( m_csVecThread );

        START_TOSTRINGW
            << TOSTRINGW( m_queEvent.size() )
            << TOSTRINGW( m_vecThread.size() );

    } // locking scope

    DumpMaxProcessInfo( stm_ );

    return stm_;
}

KThreadManager::KThreadManager(void) 
:
    m_bTerminateReserved( false ), 
    m_nTerminateCount( 0 )
{
    InitializeCriticalSection( &m_csEventQueue );

    m_kMaxProcessingInfo.m_strEventID   = L"Initial state.";
    //m_kMaxProcessingInfo.m_nSenderUID   = 0;
    m_kMaxProcessingInfo.m_dwTerm       = 0;
    m_kMaxProcessingInfo.m_cTime        = CTime::GetCurrentTime();
}

KThreadManager::~KThreadManager(void)
{
}

void KThreadManager::DumpMaxProcessInfo( std::wostream& stm_ ) const
{
    KLocker lock( m_kMaxProcessingInfo.m_cs );

    stm_ << L" -- Max processing time -- " << std::endl
         //<< L"  sender uid : " << m_kMaxProcessingInfo.m_nSenderUID << std::endl
         << L"  event id   : " << m_kMaxProcessingInfo.m_strEventID << std::endl
         << L"  duration   : " << m_kMaxProcessingInfo.m_dwTerm << std::endl
         << L"  executed time : " << (const wchar_t*)m_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KThreadManager::ResetMaxProcessingInfo()
{
    KLocker lock( m_kMaxProcessingInfo.m_cs );

    m_kMaxProcessingInfo.m_strEventID.clear();
    m_kMaxProcessingInfo.m_dwTerm       = 0;
    m_kMaxProcessingInfo.m_cTime        = CTime::GetCurrentTime();
    //m_kMaxProcessingInfo.m_nSenderUID   = 0;
}



bool KThreadManager::GetKEvent( KEventPtr& spEvent_ )
{

    { // locking scope
        KLocker lock( m_csEventQueue );

        if( !m_queEvent.empty() )
        {
            spEvent_ = m_queEvent.front();
            m_queEvent.pop();
            return true;
        } 
        
        else if( m_bTerminateReserved ) // ���ᰡ ����Ǿ���, ó���� �����Ͱ� ���̻� ���� �ʾҴ�.
        {
            //spEvent_ = NULL;    // Ȯ���ϰ� �ٽ��ѹ�! NULL�� ���Ϲ����� ������� �����Ѵ�.
            spEvent_.reset();
            m_nTerminateCount++;
            return true;
        }
    } // locking scope

    return false;
}

void KThreadManager::Init( int nThreadNum_ )
{
    // create I/O thread
    KLocker lock( m_csVecThread );

    for( int i = 0; i < nThreadNum_; i++ )
    {
        boost::shared_ptr<KThread> spThread( CreateThread() );
        spThread->SetThreadManager( this );
        m_vecThread.push_back( spThread );
    }
}

void KThreadManager::BeginThread()
{
    KLocker lock( m_csVecThread );

	for( int i = 0; i < (int)m_vecThread.size(); i++ )
	{
		m_vecThread[i]->Begin();
	}

    //std::for_each( m_vecThread.begin(), m_vecThread.end(), boost::bind( KThread::Begin, _1 ) );
}

void KThreadManager::EndThread( DWORD dwTimeOut/* = 10000 */ )
{
    KLocker lock( m_csVecThread );

    if( m_vecThread.empty() )
        return;

    m_bTerminateReserved = true;

    START_LOG( cout, L"��� �����尡 ����ǵ��� ����մϴ�." );

    DWORD dwWaitTick = ::GetTickCount();
    while( m_nTerminateCount < m_vecThread.size() && ::GetTickCount() - dwWaitTick < dwTimeOut )
    {
        ::Sleep( 20 );
    }

    START_LOG( cout, L"��� �� ���� ������ �� : " << m_vecThread.size() - m_nTerminateCount )
        << BUILD_LOG( m_vecThread.size() )
        << BUILD_LOG( m_nTerminateCount );

    m_vecThread.clear();
}

void KThreadManager::SetThreadNum( size_t nThreadNum_ )
{
    KLocker lock( m_csVecThread );

    if( nThreadNum_ == m_vecThread.size() )
    {
        std::cout << " ���� " << nThreadNum_ << "���� �����尡 ���� ���Դϴ�." << std::endl;
        return;
    }

    std::cout << " ������ �� ����. " << m_vecThread.size() << " -> " << nThreadNum_ << std::endl;

    if( nThreadNum_ < m_vecThread.size() )    // �����带 �ٿ��� �Ѵ�.
    {
        m_vecThread.resize( nThreadNum_ );
    }

    else    // �����带 �÷��� �Ѵ�.
    {
        for( size_t i = m_vecThread.size(); i < nThreadNum_; i++ )
        {
            boost::shared_ptr<KThread> spThread( CreateThread() );
            spThread->SetThreadManager( this );
            spThread->Begin();
            m_vecThread.push_back( spThread );
        }
    }

    std::cout << "���� ������ ���� : " << m_vecThread.size() << std::endl;
}
