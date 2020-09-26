#pragma once
#include "GSUser.h"
#include "ActorManager.h"

class KUserManager : public KActorManager
{
    DeclToStringW;

private:
    KUserManager(const KUserManager& right);              // do not use.
    KUserManager& operator=(const KUserManager& right);   // do not use.

public:
    KUserManager();
    virtual ~KUserManager();

    void Insert( KGSUserPtr spActor );
    bool DeleteByCID( const UidType nCID );

    bool FindCID( const UidType nCID );
    KGSUserPtr GetByCID( const UidType nCID );
    KGSUserPtr GetByCName( const std::wstring& strCharName );

    template< class T > void ForEachUser( T& functor );
    CRITICAL_SECTION& GetCS()           { return m_csAct; }
    KGSUserPtr operator[](int i) const  { return boost::static_pointer_cast<KGSUser>( m_vecAct[i] ); }    // Note : ���ο� lock ó���� ����. 
    KGSUserPtr At( int i ) const        { return boost::static_pointer_cast<KGSUser>( m_vecAct[i] ); }

    /* Note : operator[]�� ���

    ���ο� lock ó���� �����Ƿ�, multi-thread ȯ�濡�� GetCS()�� �̿���
    �ܺο��� lock�� �ɾ��ִ� ���� ����.

    KGSPagePtr spPage( new KGSPage );

    { // locking scope

        KLocker lock( spPage->GetCS() );

        for( int i = 0; i < (int)spPage->GetCount(); i++ )
        {
            std::wstring strName = (*spPage)[i]->GetName();
        }

    } // locking scope                                          */

    // actormanager���� �����Լ���. Ȱ��ÿ� ������� �ʴ´�.
    void UnregCharInfo( IN KGSUser& kUser );    // ĳ���� ���ý�, ���� ĳ���� ��ϻ����� �����Ѵ�.
    void RegCharInfo( IN KGSUser& kUser );      // ĳ���͸� ������ ���, ����� ������ �����Ѵ�.
    virtual void QueueingByCID( const UidType nReceiverCID, const KEventPtr& spEvent );
    virtual int  MultiQueueingByCID( const KEventPtr& spEvent, std::vector<UidType>& vecUID = std::vector<UidType>() ); // function over-riding ȸ��. Event�ȿ� �ִ� uid�鿡�� ��� ����.

protected:
    virtual bool Delete( const UidType nUID );  // derived from KActorManager ( util func. )

protected:
    std::map<UidType,KGSUserPtr>        m_mapCID;   // character uid.
    std::map<std::wstring,KGSUserPtr>   m_mapCName; // character name.
};

DefKObjectInline( KUserManager, KActorManager );

template<typename Func> 
void KUserManager::ForEachUser( Func& functor )
{
    KLocker lock( m_csAct );
    for( int i = 0; i < GetCount(); i++ )
    {
        functor( At(i) );
    }
}
