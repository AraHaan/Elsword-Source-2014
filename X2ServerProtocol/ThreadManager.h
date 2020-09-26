#pragma once

//#include "Performer.h"
//#include <thread/thread.h>
//#include <atltime.h> // CTime
//#include <dbg/dbg.hpp>

class KThreadManager : public KPerformer
{
    DeclToStringW;
    NiDeclareRTTI;

protected:
    KThreadManager(void);
public:
    virtual ~KThreadManager(void);

public:
    virtual void Init( int nThreadNum );
    virtual void BeginThread();
    virtual void EndThread( DWORD dwTimeOut = 10000 );   // derive to Child...

    size_t GetThreadNum() const { KLocker lock( m_csVecThread ); return m_vecThread.size(); }
    void SetThreadNum( size_t nThreadNum ); // �ǽð��� ������ ������ �����Ϸ��� �� �� ȣ��.

    // ť�׵� �̺�Ʈ�� ����. ť�� ��������� �̺�Ʈ ���� ��� ����.
    bool GetKEvent( KEventPtr& spEvent );

    int GetQueueSize()
    {
        KLocker lock( m_csEventQueue );
        return m_queEvent.size();
    }

    void DumpMaxProcessInfo( std::wostream& stm ) const;
    void ResetMaxProcessingInfo();

    void CheckMaxProcessingInfo( DWORD dwElapTime, const KEventPtr spEventPtr )
    {
        KLocker lock( m_kMaxProcessingInfo.m_cs );

        if( dwElapTime > m_kMaxProcessingInfo.m_dwTerm )
        {
            //m_kMaxProcessingInfo.m_nSenderUID  = kEvent.m_nSenderUID;
            m_kMaxProcessingInfo.m_strEventID  = spEventPtr->GetIDStr();
            m_kMaxProcessingInfo.m_cTime       = CTime::GetCurrentTime();
            m_kMaxProcessingInfo.m_dwTerm      = dwElapTime;

            START_LOG( clog2, L"�̺�Ʈ ó�� �ִ� �ҿ�ð� ����. Sender : "/* << spEventPtr->m_nSenderUID*/ )
                << BUILD_LOG( m_kMaxProcessingInfo.m_strEventID )
                << BUILD_LOG( m_kMaxProcessingInfo.m_dwTerm );
        }
    }

    // derived from KPerformer
    virtual void ProcessEvent( IN KEvent* ) {}

protected:      
    // 060113. florist. Factory Method ������ Ȱ���� ��ü ����. �̸� ���� ������ ��ü���� 
    // KThreadManager�� ��ӹ޴� �� Ŭ������ �����ؾ� ������, �̰����� ���� �������� ������
    // singleton���� �Ŵ��� ��ü�� ����Ѵٰų� �ϴ� � �����ϴ�. template�� Ȱ���� ��Ӿ���
    // thread ��ü�� �����ϰ� �߾�����, �ٽ� ����� Ȱ���� ����������� ȸ���Ͽ���. �Ź� ����ϴ°� ����.
    virtual KThread* CreateThread() = 0;

    bool                                m_bTerminateReserved;
    size_t                              m_nTerminateCount;  // ����� ������ ������ ����.
    std::vector< boost::shared_ptr<KThread> >   m_vecThread;
    mutable KncCriticalSection                  m_csVecThread;

    struct KMaxProcessingInfo {  // ó���ð��� ���� ���� �ɸ� �̺�Ʈ�� ���� ������ ���.
        mutable KncCriticalSection  m_cs;
        //UidType                     m_nSenderUID;
        std::wstring                m_strEventID;
        DWORD                       m_dwTerm;       // �̺�Ʈ �ϳ��� ó���Ϸ��ϱ���� �ɸ� �ð�.
        CTime                       m_cTime;
    } m_kMaxProcessingInfo;
};

template< typename T >
class KTThreadManager : public KThreadManager
{
    NiDeclareRTTI;
    DeclToStringW;

protected:
    KTThreadManager(void) {}
public:
    virtual ~KTThreadManager(void) {}

    static KTThreadManager<T>* GetInstance()
    {
        if( ms_selfInstance == NULL )
            ms_selfInstance = new KTThreadManager<T>;
        return ms_selfInstance;
    }
    static void ReleaseInstance()
    {
        if( ms_selfInstance != NULL )
        {
            delete ms_selfInstance;
            ms_selfInstance = NULL;
        }
    }

protected:
    // util function
    virtual KThread* CreateThread()
    {
        T* pkThread( new T );
        pkThread->SetThreadManager( this );
        return pkThread;
    }

    static KTThreadManager<T>*   ms_selfInstance;
};

template < typename T > KTThreadManager<T>* KTThreadManager<T>::ms_selfInstance = NULL;
template < typename T > NiImplementRTTI( KTThreadManager<T>, KThreadManager );
template < typename T > ImplOstmOperatorW2A( KTThreadManager<T> );
template < typename T > ImplToStringW( KTThreadManager<T> )
{
    return START_TOSTRING_PARENTW( KThreadManager );
}
