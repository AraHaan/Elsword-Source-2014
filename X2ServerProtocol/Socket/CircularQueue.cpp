#include "StdAfx.h"

#include "CircularQueue.h"

KCircularQueue::KCircularQueue() : m_nHead( 0 ), m_nTail( 0 )
{

}

KCircularQueue::~KCircularQueue()
{

}

void KCircularQueue::Resize( IN int nSize )
{
    resize( nSize );
    m_nHead = 0;
    m_nTail = 0;
}

int KCircularQueue::Push( IN const char* pData, IN int nSize )
{
    if( empty() ) return 0;

    nSize = min( nSize, GetFreeSize() );

    if( (int)size() - m_nTail <= nSize )  // queue�� ó������ �ٽ� ���ư��� �ϴ� ���
    {
        int nRearSize = size() - m_nTail;

        // ������ ���ư��� ������ ������ ����.
        ::memcpy( &at(m_nTail), pData, nRearSize );
        // ������ ���ư� ���� ������ ����.
        ::memcpy( &(*begin()), pData + nRearSize, nSize - nRearSize );

        m_nTail = nSize - nRearSize;
    }
    else    // ������ �޸� ����.
    {
        ::memcpy( &at(m_nTail), pData, nSize );
        m_nTail += nSize;
    }

    return nSize;
}

int KCircularQueue::Pop( IN int nSize, OUT char* pBuffer )
{
    if( empty() ) return 0;

    int nRDSize = min( nSize, GetRearDataSize() );
    int nFDSize = min( nSize - nRDSize, GetFrontDataSize() );

    // rear data �������� pop.
    ::memcpy( pBuffer, &at(m_nHead), nRDSize );

    m_nHead += nRDSize;
    if( m_nHead == size() )
        m_nHead = 0;

    if( nRDSize < nSize )
    {
        //front data �������� �߰��� pop.
        ::memcpy( pBuffer + nRDSize, &at(m_nHead), nFDSize );
        m_nHead += nFDSize;
    }

    return nRDSize + nFDSize;
}

int KCircularQueue::GetDataSize()
{
    if( empty() ) return 0;

    return (m_nTail - m_nHead + size()) % size();
}

int KCircularQueue::GetFreeSize()
{
    if( empty() ) return 0;

    // circular-queue������ ���� ������ ��ĭ�� ����� �� ����.
    return (m_nHead - m_nTail + size() - 1) % size(); 
}

int KCircularQueue::GetRearDataSize()
{
    if( m_nHead <= m_nTail )
        return m_nTail - m_nHead;

    return size() - m_nHead;
}

int KCircularQueue::GetFrontDataSize()
{
    if( m_nHead <= m_nTail )
        return 0;

    return m_nTail;
}