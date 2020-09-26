#include "GSProxy.h"
#include "NetError.h"
#include <KncSecurity/KncSecurity.h>
#include "KncClient.h"

NiImplementRTTI( KGSProxy, KActorProxy );
ImplPfID( KGSProxy, PI_GS_PROXY );

#define CLASS_TYPE KGSProxy

KGSProxy::KGSProxy(void)
{
}

KGSProxy::~KGSProxy(void)
{
}

ImplToStringW( KGSProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy );
}

//bool KGSProxy::RequestServerMigration( IN const KServerInfo& kInfo_, DWORD dwTimeOut_ /* = 3000  */)
//{
//    _JIF( SendPacket( EVENT_SERVER_MIGRATION_REQ, kInfo_.m_strName ), false );
//
//    m_nAckOK = -99; // unused-error code.
//
//    switch( ::WaitForSingleObject( m_spEventAck.get(), dwTimeOut_ ) ) {
//        case WAIT_OBJECT_0: // ack�� ����� ���� ���.
//            return (m_nAckOK == 0);
//            break;
//        case WAIT_TIMEOUT: // �ð� �ʰ�
//            START_LOG( cerr, L"verify account timeout." );
//            break;
//        default:
//            START_LOG( cerr, L"invalide event object." );
//    }
//
//    return false;
//}


void KGSProxy::OnSocketError()
{    
    KSession::OnSocketError();

    // ���� Ŭ���̾�Ʈ���� ������ ���������� �˷��� �Ѵ�.

    KEventPtr spEvent( new KEvent );
    spEvent->m_usEventID = E_CONNECTION_LOST_NOT;

    // windows msg sendmessage
    PostEvent( spEvent );
}

void KGSProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
    PostEvent( spEvent_ );

	switch( spEvent_->m_usEventID )
	{
	//CASE( EGS_CREATE_UNIT_ACK );
    CASE_NOPARAM( E_RESERVE_DESTROY );
	}

    //switch( spEvent_->m_usEventID )
    //{
    //    CASE( EVENT_VERIFY_ACCOUNT_ACK );
    //    CASE( EVENT_GMTOOL_AUTHENTICATION_ACK );
    //   _CASE( EVENT_SERVER_MIGRATION_ACK, int );
    //   // CASE( EVENT_WHERE_ACK );
    //   // CASE( EVENT_WHISPER_ACK );
    //   // CASE( EVENT_INVITE_NOT );
    //   // CASE( EVENT_OPEN_USER_INFO_ACK );

    //default:     
    //    // �̰����� �̺�Ʈ �ڵ鷯��, �� �ʿ��� �͸� ���ǵȴ�. ���ǵ��� ���� �̺�Ʈ�� ���� �� �ִ�.
    //    //START_LOG( cerr , "�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << kEvent_ );
    //    break;
    //} 
}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}

//IMPL_ON_FUNC( EGS_CREATE_UNIT_ACK )
//{
//	//SiKncClient()->m_iUnitUID = kPacket_.m_kUnitInfo.m_nUnitUID;
//}

//_IMPL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int )
//{
//    m_nAckOK = kPacket_;
//
//    SetEvent( m_spEventAck.get() );
//}
//
//IMPL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK )
//{
//    m_nAckOK = kPacket_.m_nOK;
//
//    SetEvent( m_spEventAck.get() );
//
//    if( kPacket_.m_nOK != NetError::NET_OK )
//    {
//        START_LOG_WITH_NAME( cerr )
//            << BUILD_LOGc( kPacket_.m_nOK )
//            << END_LOG;
//        return;
//    }
//
//    START_LOG_WITH_NAME( cout )
//        << BUILD_LOGc( kPacket_.m_nOK )
//        << BUILD_LOG( kPacket_.m_strLogin )
//        << BUILD_LOGc( kPacket_.m_cAuthLevel );
//
//    SetName( kPacket_.m_strLogin );
//}
//
//IMPL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK )
//{
//    m_nAckOK = kPacket_.m_ucOK;
//
//    SetEvent( m_spEventAck.get() );
//
//    if( kPacket_.m_ucOK != 0 )
//    {
//        START_LOG_WITH_NAME( cerr )
//            << BUILD_LOGc( kPacket_.m_ucOK )
//            << END_LOG;
//
//        if( kPacket_.m_ucOK != 11 ) // �븸 gash ���� ���н�, ������ ���� �ʴ´�.
//        {
//            ReserveDestroy();
//        }
//        return;
//    }
//
//    assert( !kPacket_.m_mapDifficulty.empty() );
//    
//    START_LOG_WITH_NAME( cout )
//        << BUILD_LOGc( kPacket_.m_ucOK )
//        << BUILD_LOG( kPacket_.m_strServerName )
//        << BUILD_LOG( kPacket_.m_strLogin )
//        << BUILD_LOG( kPacket_.m_iGamePoint )
//        << BUILD_LOG( kPacket_.m_vecInv.size() )
//        << BUILD_LOG( kPacket_.m_vecDurationItemInv.size() )
//        << BUILD_LOG( NetCommon::CalcIP( kPacket_.m_dwIP ) )
//        << BUILD_LOG( kPacket_.m_kGuildInfo.m_iId )
//        << BUILD_LOG( kPacket_.m_dwServerType )
//        << BUILD_LOG( kPacket_.m_mapDifficulty.size() );
//
//    SetName( kPacket_.m_strLogin );
//}