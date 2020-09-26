#pragma once

#include <map>
#include <boost/noncopyable.hpp>
#include "SecurityAssociation.h"
#include <windows.h>    // CRITICAL_SECTION
#include "Thread/Locker.h"

NAMESPACE_BEGIN( KncSecurity )

// 2009-10-16 ���� comment
// KSADatabase�� singleton���� �����˴ϴ�.
// �����Ǵ� Session���� SPIndex�� ��� �޶�� ��

class KSADatabase : public boost::noncopyable
{
public:
    KSADatabase();
    virtual ~KSADatabase();

    void Insert( OUT SpiType& nSPIndex, IN const KSecurityAssociation& sa );
    void Delete( SpiType nSPIndex );
    bool Find( SpiType nSPIndex ) const;
    KSecurityAssociation& GetSA( SpiType nSPIndex );
    const KSecurityAssociation& GetSA( SpiType nSPIndex ) const;

    const KSecurityAssociation* CreateNewSA( OUT SpiType& nSPIndex );

protected:
    mutable CRITICAL_SECTION                    m_csSaList;
    std::map<SpiType, KSecurityAssociation>     m_mapSaList;
};

NAMESPACE_END