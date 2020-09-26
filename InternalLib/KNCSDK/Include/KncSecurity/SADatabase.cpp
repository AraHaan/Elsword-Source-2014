#include "SADatabase.h"
#include <cryptopp/osrng.h>
#include "KncSecurity.h"
#include "dbg/dbg.hpp"

#undef max
#include <boost/random.hpp>
#include <limits>

NAMESPACE_BEGIN( KncSecurity )

KSADatabase::KSADatabase()
{
    InitializeCriticalSection( &m_csSaList );

    KLocker lock( m_csSaList );

	// 2009-10-16 ���� comment
	// SADatabase�� �����ڿ����� �⺻������ SPIndex���� 0�� default KSecurityAssociation�� �����д�.
	// TCP Connection�� �ΰ��� Ŭ���̾�Ʈ�� �����κ��� ù��°�� E_ACCEPT_CONNECTION_NOT �̺�Ʈ�� ���� SPIndex�� �޾ƾ��ϴµ�
	// �� ��Ŷ�� Valid�� ��Ŷ���� üũ�Ҷ� SPIndex���� 0�� ���¿��� üũ�ϱ� ������.
	
    m_mapSaList.insert( std::pair<SpiType, KSecurityAssociation>( 0, KSecurityAssociation() ) );
}

KSADatabase::~KSADatabase()
{
    DeleteCriticalSection( &m_csSaList );
}

void KSADatabase::Insert( OUT SpiType& nSPIndex, IN const KSecurityAssociation& sa )
{
    static boost::mt19937 rng;
    static boost::uniform_int<DWORD> uint32( 1, std::numeric_limits<SpiType>::max() );   // 1 �̻�. SPI 0�� �����ڿ��� �̹� �����ߴ�.
    static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

    KLocker lock( m_csSaList );

    for( ;; )
    {
        // Generate a random number
        nSPIndex = static_cast<SpiType>( die() );

        // Search for the number in the database. If it's not there, use it.
        if( !Find( nSPIndex ) )
            break;
    }

    m_mapSaList.insert( std::pair<SpiType, KSecurityAssociation>( nSPIndex, sa ) );
}

void KSADatabase::Delete( SpiType nSPIndex )
{
    KLocker lock( m_csSaList );

    m_mapSaList.erase( nSPIndex );
}

bool KSADatabase::Find( SpiType nSPIndex ) const
{
    KLocker lock( m_csSaList );

    return m_mapSaList.find( nSPIndex ) != m_mapSaList.end();
}

KSecurityAssociation& KSADatabase::GetSA( SpiType nSPIndex )
{
    KLocker lock( m_csSaList );

    std::map<SpiType, KSecurityAssociation>::iterator i;
    i = m_mapSaList.find( nSPIndex );
    
    LIF( i != m_mapSaList.end() );

    // ã���� �ϴ� SA�� ���� ��� SPI 0 ���� ������ ���Ű�� �����Ѵ�.
    if( i == m_mapSaList.end() ) i = m_mapSaList.begin();

    return i->second;
}

const KSecurityAssociation& KSADatabase::GetSA( SpiType nSPIndex ) const
{
    KLocker lock( m_csSaList );

    std::map<SpiType, KSecurityAssociation>::const_iterator i;
    i = m_mapSaList.find( nSPIndex );

    LIF( i != m_mapSaList.end() );

    // ã���� �ϴ� SA�� ���� ��� SPI 0 ���� ������ ���Ű�� �����Ѵ�.
    if( i == m_mapSaList.end() ) i = m_mapSaList.begin();

    return i->second;
}

const KSecurityAssociation* KSADatabase::CreateNewSA( OUT SpiType& nSPIndex )
{
    KSecurityAssociation sa;
    sa.ResetRandomizeKey();

    Insert( nSPIndex, sa );

    return &GetSA( nSPIndex );
}

NAMESPACE_END