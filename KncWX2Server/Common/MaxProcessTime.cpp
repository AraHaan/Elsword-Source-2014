#include "MaxProcessTime.h"

#include <dbg/dbg.hpp>

KMaxProcessTime::KMaxProcessTime()
{
    m_iNumRecord = 1;
}

KMaxProcessTime::KMaxProcessTime( int iNumRecord )
{
    m_iNumRecord = iNumRecord;
}

KMaxProcessTime::~KMaxProcessTime()
{
}

void KMaxProcessTime::CheckMaxProcessTime( const std::wstring& wstrMsg, float fElapsedTime )
{
    KLocker lock( m_cs );

    std::list< KElapsedTimeRecord >::iterator lit;
    for( lit = m_listElapsedTimeRank.begin(); lit != m_listElapsedTimeRank.end(); lit++ )
    {
        if( fElapsedTime > lit->m_fElapsedTime )
        {
            break;
        }
    }

    if( lit == m_listElapsedTimeRank.end() && ( ( int )m_listElapsedTimeRank.size() >= m_iNumRecord ) )
    {
        return;
    }

    KElapsedTimeRecord kRecord;
    kRecord.m_wstrMsg = wstrMsg;
    kRecord.m_fElapsedTime = fElapsedTime;
    kRecord.m_cTime = CTime::GetCurrentTime();

    m_listElapsedTimeRank.insert( lit, kRecord );

    while( ( int )m_listElapsedTimeRank.size() > m_iNumRecord )
    {
        if( m_listElapsedTimeRank.empty() )
        {
            START_LOG( cerr, L"����Ʈ�� �����. �߻��� �� ���� ��Ȳ." )
                << BUILD_LOG( m_iNumRecord )
                << END_LOG;

            break;
        }

        m_listElapsedTimeRank.pop_back();
    }

    Dump();
}

void KMaxProcessTime::Dump()
{
    KLocker lock( m_cs );

    dbg::cout << L"--> �ִ� ó�� �ð�. ��� �� : " << m_iNumRecord << L"��" << dbg::endl << dbg::endl;

    std::list< KElapsedTimeRecord >::iterator lit;
    for( lit = m_listElapsedTimeRank.begin(); lit != m_listElapsedTimeRank.end(); lit++ )
    {
        std::wstring wstrTime = ( lit->m_cTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) ).GetString();
#ifndef SERV_PRIVACY_AGREEMENT
        dbg::cout << L"    �̸� : " << lit->m_wstrMsg << dbg::endl
#endif SERV_PRIVACY_AGREEMENT
                  << L"    �ҿ� �ð� : " << lit->m_fElapsedTime << dbg::endl
                  << L"    �߻� �ð� : " << wstrTime.c_str() << dbg::endl << dbg::endl;
    }

    dbg::cout << dbg::endl;
}