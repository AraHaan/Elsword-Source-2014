#include "ActorManager.h"
#include "NetError.h"

#include <boost/bind.hpp>

NiImplementRootRTTI( KActorManager );
ImplInstanceKeeping( KActorManager );

KActorManager::KActorManager()
{
    m_vecAct.reserve( 5000 ); 
}

KActorManager::~KActorManager()
{
}


void KActorManager::AddActor( KActorPtr spActor_ )
{
    JIF( spActor_ );

    { // locking scope

        KLocker lock( m_csAct );

        for(;;) // �ߺ����� �ʴ� �ӽ� UID�� ã�´�.
        {
            UidType nUID = KncUid::GetTempUID();

            if( m_mapUID.find( nUID ) == m_mapUID.end() )
            {
                spActor_->SetUID( nUID );
                break;
            }
        }

        m_vecAct.push_back( spActor_ );

        m_mapUID.insert( std::make_pair( spActor_->GetUID(), spActor_ ) );

    } // locking scope

    START_LOG( clog, L"Name : " << spActor_->GetName() << ", UID : " << spActor_->GetUID() );
}

void KActorManager::ReserveAdd( KActorPtr spActor )
{
    KLocker lock( m_csAdd );
    m_vecAddAct.push_back( spActor );
}

void KActorManager::ReserveDelete( const UidType nUID_ )
{
    KLocker lock( m_csDel );
    m_vecDelReserved.push_back( nUID_ );
}

void KActorManager::Tick()
{
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

    // 1. �� Actor�� ���� event�� ó��
    ForEach( boost::bind( &KActor::Tick, _1 ) );

    // 2. ���� ���¿��� ������� ���� ��ü�� �ı�. ( ���� 1. �� �������� delete, insert �� �� ����. )
    { // locking scope
        KLocker lock( m_csDel );
        if( !m_vecDelReserved.empty() )
        {
            std::for_each( m_vecDelReserved.begin(), m_vecDelReserved.end(),
                boost::bind( &KActorManager::Delete, boost::ref( *this ), _1 ) );

            m_vecDelReserved.clear();
        }

    } // locking scope

    // 3. ���� �����̳ʿ� ��� ������� ��ü�� �ִٸ� �߰��۾�.
    { // locking scope
        KLocker lock( m_csAdd );
        if( !m_vecAddAct.empty() )
        {
            std::for_each( m_vecAddAct.begin(), m_vecAddAct.end(),
                boost::bind( &KActorManager::AddActor, boost::ref( *this ), _1 ) );

            m_vecAddAct.clear();
        }

    } // locking scope

	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

bool KActorManager::Delete( const UidType nUID_ )
{
    KLocker lock( m_csAct );

    std::map< UidType, KActorPtr >::iterator mit;
    mit = m_mapUID.find( nUID_ );
    if( mit == m_mapUID.end() )
    {
        START_LOG( cerr, L"�����ϰ��� �ϴ� UID�� ����" )
            << BUILD_LOG( nUID_ )
            << BUILD_LOG( m_mapUID.size() )
            << END_LOG;
        return false;
    }

    std::vector<KActorPtr>::iterator vit;
    vit = std::find( m_vecAct.begin(), m_vecAct.end(), mit->second );

    if( vit == m_vecAct.end() ) // �����ؾ� �� ���� �����͸� ã�� ����.
    {
        START_LOG( cerr, L"�����ؾ� �� ���� �����Ͱ� vector�� ����" )
            << BUILD_LOG( nUID_ )
            << BUILD_LOG( m_vecAct.size() );
    }
    else
    {
        if( ms_object == this ) // ���� ��ü�� Actor ������ ���̴� ��ü�� ���, ��ü �Ҹ� Ȯ��.
        {
			// 060321. florist. RefCount�� �����־� �������� ���� ���ɼ��� �ִ� ��츦 �˻��Ѵ�.
			//{{ 2009. 6. 19  ������	KParty��ü���� KGSUser�� �����͸� �����ϰ� �־ CheckExceedRefCount�Ҷ� 3�� ������

			(*vit)->CheckExceedRefCount( 2 ); // �׽�Ʈ�Ŀ� 3���� ��ĥ����!

			//}}
        }

        m_vecAct.erase( vit );  // ���� �����͸� ������.
    }

    m_mapUID.erase( nUID_ ); // ���������� Name key�� ������.
    return true;
}

KActorPtr KActorManager::Get( const UidType nUID_ )
{
    // 2007. 12. 07. jseop.
    KLocker lock( m_csAct );

    std::map<UidType,KActorPtr>::iterator mit;

    mit = m_mapUID.find( nUID_ );

    if( mit == m_mapUID.end() ) return KActorPtr();

    return mit->second;
}

bool KActorManager::UpdateUID( const UidType nUID_, KActor& kActor_ )
{
    // 2007. 12. 07. jseop.
    KLocker lock( m_csAct );

    KActorPtr spActor = boost::static_pointer_cast<KActor>( kActor_.shared_from_this() );

    _JIF( spActor, return false );

    // key�� ���ڷ� �޴� erase�� ������ element ���� ��ȯ�Ѵ�.
    LIF( m_mapUID.erase( spActor->GetUID() ) != 0 );

    spActor->SetUID( nUID_ );

    std::pair<std::map<UidType, KActorPtr>::iterator, bool> kPair;
    kPair = m_mapUID.insert( std::make_pair( nUID_, spActor ) );

    return kPair.second;
}

bool KActorManager::SendTo( const UidType nReceiverUID_, IN const KEvent& kEvent_ )
{
    KActorPtr spActor( Get( nReceiverUID_ ) );

    if( !spActor ) return false;

    return spActor->SendPacket( kEvent_ );
}

void KActorManager::QueueingTo( const UidType nReceiverUID_, const KEventPtr& spEvent_ )
{
    KActorPtr spActor( Get( nReceiverUID_ ) );

    if( spActor )
        spActor->QueueingEvent( spEvent_ );
}

int KActorManager::MultiQueueing( const KEventPtr& spEvent_, std::vector<UidType>& vecUID )
{
    const std::set<UidType>& setUID = spEvent_->m_kDestPerformer.GetUidListReference();

	int iNotExistUserCnt = 0;

    KActorPtr spActor;
    std::set<UidType>::const_iterator sit;
    for( sit = setUID.begin(); sit != setUID.end(); sit++ )
    {
        spActor = Get( *sit );

        if( spActor )
			spActor->QueueingEvent( spEvent_->Clone() );
		else
		{
			vecUID.push_back( *sit );
			++iNotExistUserCnt;
		}
    }

	return iNotExistUserCnt;
}

void KActorManager::SendToAll( const KEvent& kEvent )
{
    KLocker lock( m_csAct );

    std::vector<KActorPtr>::iterator vit;
    for( vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++ )
    {
        ( *vit )->SendPacket( kEvent );
    }
}

void KActorManager::QueueingToAll( const KEventPtr& spEvent )
{
    KLocker lock( m_csAct );

    std::vector<KActorPtr>::iterator vit;
    for( vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++ )
    {
        ( *vit )->QueueingEvent( spEvent->Clone() );
    }
}

//void KActorManager::SendIDToAll( const u_short usEventID )
//{
//    KLocker lock( m_csAct );
//
//    ForEach( boost::bind( &KActor::SendID, _1, usEventID ) );
//}

//{{ 2008. 10. 10  ������	ù��° UID���
UidType KActorManager::GetFirstActorKey()
{
	KLocker lock( m_csAct );

	std::map< UidType, KActorPtr >::const_iterator mit = m_mapUID.begin();
	if( mit == m_mapUID.end() )
		return 0;
	
	return mit->first;
}
//}}

ImplToStringW( KActorManager )
{
	//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
// SERV_MAX_QUEUE_SIZE_DUMP
	UidType iMaxQueueSizeActorUID = 0;
	KPerformer::KMaxQueueSize kMaxQueueSizeDump;

	{
		KLocker lock( m_csAct );

		std::vector< KActorPtr >::const_iterator vit;
		for( vit = m_vecAct.begin(); vit != m_vecAct.end(); ++vit )
		{
			// ����Ʈ �����ͷ� ����
			KActorPtr spActor = *vit;
			if( spActor == NULL )
				continue;

			const KPerformer::KMaxQueueSize kMaxQueueSize = spActor->GetMaxQueueSize();
			if( kMaxQueueSize.m_QueueSize > kMaxQueueSizeDump.m_QueueSize )
			{
				kMaxQueueSizeDump = kMaxQueueSize;
                iMaxQueueSizeActorUID = spActor->GetUID();
			}
		}
	}

	return START_TOSTRINGW
		<< TOSTRINGW( GetCount() )
		<< TOSTRINGW( iMaxQueueSizeActorUID )
		<< TOSTRINGW( kMaxQueueSizeDump.m_QueueSize )
		<< TOSTRINGW( kMaxQueueSizeDump.GetMaxQueueSizeRegDate() );
// SERV_MAX_QUEUE_SIZE_DUMP
	//}}
}


