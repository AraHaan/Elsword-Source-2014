#include "CnUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"

#include "DBLayer.h"
#include "NetError.h"
#include "CnSimLayer.h"
#include "CnRoomManager.h"
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewCnPartyManager.h"
#else
	#include "CnPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
#include "CnServer.h"
//#include "Nexon/AuthCheck.h" -- ���� ���� �ڵ�
#include <boost/format.hpp>

//#pragma comment( lib, "../Common/Nexon/AuthCheck.lib" ) -- ���� ���� �ڵ�

NiImplementRTTI( KCnUser, KActor );
ImplPfID( KCnUser, PI_CN_USER );

#define CLASS_TYPE  KCnUser

KCnUser::KCnUser(void) 
//{{ 2010. 04. 16  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
:
m_iCnServerGroupID( -1 ),
m_iGsServerGroupID( -1 )
#endif SERV_INTEGRATION
//}}
{
}

KCnUser::~KCnUser(void)
{
}

ImplToStringW( KCnUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KCnUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    if( RoutePacket( spEvent_.get() ) )
        return;

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	CASE( ECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _CASE( ECN_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	//}}   
   _CASE( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
   _CASE( ECN_UPDATE_SERVER_INFO_ACK, KServerList );
    CASE_NOPARAM( EGS_GET_SERVER_SET_DATA_REQ );
   _CASE( ECN_UPDATE_SERVER_INFO_REQ, KServerInfo );	

	CASE_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ );
	CASE_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ );
	CASE_NOPARAM( E_RESERVE_DESTROY );

    default:
        dbg::cerr << L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr();
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

void KCnUser::Tick()
{
    KActor::Tick();

    switch( GetStateID() ) 
	{
    case KDefaultFSM::S_EXIT:
        KActorManager::GetKObj()->ReserveDelete( m_nUID );    // �ڱ� �ڽ��� ������ ������ �� ����.
        break;
    }
}

//bool KCnUser::AuthenticateUser( const wchar_t* szPassport, KNexonAccountInfo& kInfo ) -- ���� ���� �ڵ�
//{
//    BOOL bRet;
//    wchar_t szID[MAX_PATH];
//    UINT32 nNexonSN;
//    wchar_t szLoginIP[MAX_PATH];
//    UINT32 uSex;
//    UINT32 uAge;
//    UINT32 uPwdHash;
//    UINT32 uSsnHash;
//    UINT32 uFlag0;
//    UINT32 uFlag1;
//
//    bRet = AuthCheck_LocalCheckPassport( szPassport, szID, &nNexonSN, szLoginIP, &uSex, &uAge, &uPwdHash, &uSsnHash, &uFlag0, &uFlag1 );
//
//    if( bRet )
//    {
//        if( kInfo.m_wstrLoginIP.compare( szLoginIP ) != 0 )
//        {
//            START_LOG( cerr, L"KSession�� ���� IP�� �ؽ� �н����忡�� ������ IP�� �ٸ�." )
//                << BUILD_LOG( kInfo.m_wstrLoginIP )
//                << BUILD_LOG( szLoginIP )
//                << END_LOG;
//
//            // ���� ���� ó�� �ұ� ����..
//        }
//
//        kInfo.m_wstrID = szID;
//        kInfo.m_uiNexonSN = nNexonSN;
//        kInfo.m_wstrLoginIP = szLoginIP;
//        if( uSex == 1 )
//        {
//            kInfo.m_bSex = true;
//        }
//        else if( uSex == 2 )
//        {
//            kInfo.m_bSex = false;
//        }
//        else
//        {
//            START_LOG( cerr, L"�ؽ� ���� ���� ���� �̻���." )
//                << BUILD_LOG( szID )
//                << BUILD_LOG( szPassport )
//                << BUILD_LOG( uSex )
//                << END_LOG;
//        }
//        kInfo.m_uiAge = uAge;
//        kInfo.m_uiPwdHash = uPwdHash;
//        kInfo.m_uiSsnHash = uSsnHash;
//        kInfo.m_uiFlag0 = uFlag0;
//        kInfo.m_uiFlag1 = uFlag1;
//    }
//
//    return (bRet == TRUE);  // BOOL -> bool
//}

void KCnUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() ) {
    case KDefaultFSM::S_INIT:
    case KDefaultFSM::S_LOGINED:
        StateTransition( KDefaultFSM::I_EXIT );    // OnDestroy()�� �Ҹ� ���� KSession::Tick ó�� ���̸�,
                                                   // KCnUser::Tick ó�� �ܰ迡�� ���� ����ܿ� ��ϵȴ�.
        break;
    }

	//{{ 2011.2.11  ��ȿ��  ����͸��� ���� ��Ʈ üũ �� �����α� ���°� ����
#ifdef SERV_PERMIT_PORT_CHECK
	// ����͸� �� ������ IP������ ���� ������ ���� �� �� ���ٰ� ������ //
	// ����͸� ������ ���� ��Ʈ üũ�� ��� �Ʒ��� �ٸ� ���۵� �� �ʿ����
	if( KBaseServer::GetKObj()->IsMonitoringServerForPortCheck( GetIPStr() ) ){
		//KSession::OnDestroy();					// �̹� ������ ������
		//StateTransition( KDefaultFSM::I_EXIT );	// �̹� ������ ������
		START_LOG( clog, L"Mornitoring server check Port!" );
		return;
	}
#endif SERV_PERMIT_PORT_CHECK
	//}}

    // ���Ӽ����� ���� ���, �ش� ���Ӽ����� �������̴� �������� �濡�� ��� ������.
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, ERM_CHECK_INVALID_USER_NOT, m_nUID );

		//SiKRoomManager()->QueueingToAll( spEvent );
		// 081017.hoons.
		GetKCnRoomManager()->QueueingToAll( spEvent );
	}

	//{{ 2010. 03. 09  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, EPM_CHECK_INVALID_USER_NOT, m_nUID );
		//SiKCnPartyManager()->QueueingToAll( spEvent );
		SiKCnPartyManager()->QueueingEvent( spEvent );
	}
#endif SERV_CHANNEL_PARTY
	//}}

	//{{ 2009. 4. 22  ������	���Ӽ��� ���Ӳ��迡 ���� �������� ��Żó��
	{
		KEventPtr spEventPVP( new KEvent );
		spEventPVP->SetData( PI_GS_SERVER, NULL, ECN_CHECK_INVALID_PVP_USER_NOT, m_nUID );
		KBaseServer::GetKObj()->QueueingEvent( spEventPVP );
	}
	//}}

	//{{ 2009. 1. 30  ������	��ü �Ҹ� �α�
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"���� �˴ٿ����� ���� CnUser��ü �Ҹ�" );			
	}
	//{{ 2011. 10. 31	������	��Ʈ��Ʈ ���� ���� �м�
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	else if( GetDisconnectReason() == KStatistics::eSIColDR_Heart_Beat )
	{
		START_LOG( cout2, L"��Ʈ��Ʈ�� ���� CnUser��ü �Ҹ�" )
			<< BUILD_LOG( GetDisconnectReason() )
			<< BUILD_LOG( GetIPStr() )
			<< BUILD_LOG( m_dwHBTickDump )
			<< BUILD_LOG( m_dwCurrentTickDump )
			<< BUILD_LOG( m_dwCurrentTickDump - m_dwHBTickDump )
			<< dbg::tab << L"���� ���� : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetUID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
	}
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	//}}
	else
	{
		START_LOG( cout2, L"���������� �������� ���� CnUser��ü �Ҹ�" )
			<< BUILD_LOG( GetDisconnectReason() )
			<< BUILD_LOG( GetIPStr() )
			<< dbg::tab << L"���� ���� : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetUID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
	}
	//}}
}

bool KCnUser::RoutePacket( const KEvent* pkEvent_ )
{
    if( pkEvent_->m_kDestPerformer.m_dwPerformerID != PI_CN_USER )
    {
		//{{ 2011.2.11  ��ȿ��  ����͸��� ���� ��Ʈ üũ �� �����α� ���°� ����
#ifdef SERV_PERMIT_PORT_CHECK
		// CnUser ����͸� ������ ��� �������� ��ü�� �ƴϹǷ� RoutPacket ������ ��ġ�� �ȵ�
		if( KBaseServer::GetKObj()->IsMonitoringServerForPortCheck( GetIPStr() ) )
		{
			return true;
		}
#endif SERV_PERMIT_PORT_CHECK
		//}}

        int iEval = CompareServerLevel( pkEvent_->m_kDestPerformer.m_dwPerformerID, PI_CN_USER );
        if( iEval < 0 )
        {
            if( pkEvent_->IsEmptyTrace() )
            {
                START_LOG_WITH_NAME( cerr ) 
                    << BUILD_LOG( pkEvent_->GetIDStr() );

                return true;
            }

			//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
			// ������ ������ �ʱ�ȭ �Ǿ����� Ȯ��
			if( IsInitServerGroup() == false )
			{
				START_LOG( cout, L"Not Init Server Group Event : " << KEvent::GetIDStr( pkEvent_->m_usEventID ) )
					<< BUILD_LOG( m_iCnServerGroupID )
					<< BUILD_LOG( m_iGsServerGroupID );
				return true;
			}

			// ���� ���������Ը� ������ �̺�Ʈ���?
			if( GetSendType( pkEvent_->m_kDestPerformer.m_dwPerformerID ) == ST_SAME_SERV_GROUP )
			{
				if( IsSameServerGroup() == false )
				{
					START_LOG( clog, L"Not Same Server Group Event : " << KEvent::GetIDStr( pkEvent_->m_usEventID ) );
					return true;
				}
			}
#endif SERV_INTEGRATION
			//}}

            KEvent kEvent( *pkEvent_ );

            kEvent.PopTrace();

            LIF( KSession::SendPacket( kEvent ) );
            return true;
        }
        else if( iEval > 0 )
        {
            START_LOG( cerr, L"����� ���� ������ �ֻ��� �����̴�." )
                << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( pkEvent_->m_usEventID )
                << BUILD_LOG( pkEvent_->GetIDStr() )
                << END_LOG;

            return true;
        }
        else
        {
            KEventPtr spEvent( pkEvent_->Clone() );
            spEvent->PushTrace( GetUID() );

            switch( GetPerformerClass( pkEvent_->m_kDestPerformer.m_dwPerformerID ) )
            {
            case PC_SERVER:
                KBaseServer::GetKObj()->QueueingEvent( spEvent );
                return true;
            case PC_ACCOUNT_DB:
            case PC_LOG_DB:
                SiKDBLayer()->QueueingEvent( spEvent );
                return true;
            case PC_ROOM:
                //SiKRoomManager()->QueueingEventToRoom( spEvent );
				//081017.hoons.
				GetKCnRoomManager()->QueueingEventToRoom( spEvent );
                return true;
				//{{ 2010. 01. 28  ������	������Ƽ
#ifdef SERV_CHANNEL_PARTY
			case PC_PARTY:
				SiKCnPartyManager()->QueueingEventToParty( spEvent );
				return true;
#endif SERV_CHANNEL_PARTY
				//}}
            default:
                START_LOG( cerr, L"�������� �̻��� ��Ŷ." )
                    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                    << END_LOG;
                return true;
            }
        }
    }

    return false;
}

void KCnUser::SendToAccountDB( unsigned short usEventID )
{
    SendToAccountDB( usEventID, char() );
}

void KCnUser::SendToGSUser( UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nTo, usEventID, char() );
}

//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

IMPL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_usPort );

	if( kPacket_.m_kNetAddress.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"��ȿ���� ���� IP" )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() );
#else
			<< BUILD_LOG( GetIPStr() );
#endif SERV_PRIVACY_AGREEMENT

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( ECN_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

#else

_IMPL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_REQ, KNetAddress )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_usPort );

	if( kPacket_.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"��ȿ���� ���� IP" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
#else
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< BUILD_LOG( GetIPStr() );
#endif SERV_PRIVACY_AGREEMENT

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( ECN_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

#endif
//}}

_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );    

	//{{ 2010. 04. 16  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_iDBRegServerGroupID )
		<< BUILD_LOG( kPacket_.m_iLocalServerGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );

	kPacket_.m_iDBRegServerGroupID = GetKCnServer()->GetServerGroupID(); // ���ͼ����� ������ID�� ���Ӽ������� �Ѱ��ش�.
#else
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_sGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );
#endif  SERV_INTEGRATION
	//}}

    SendPacket( ECN_VERIFY_SERVER_CONNECT_ACK, kPacket_ );

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        ReserveDestroy();
        return;
    }
	else
	{
		//{{ 2010. 04. 16  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
		m_iCnServerGroupID = GetKCnServer()->GetServerGroupID();
		m_iGsServerGroupID = kPacket_.m_iLocalServerGroupID;

		//{{ 2009. 3. 25  ������	�α�
		if( IsSameServerGroup() )
		{
			START_LOG( cout2, L"���� �������� ���Ӽ����� ���������� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kPacket_.m_nUID )
				<< BUILD_LOG( kPacket_.m_strName )
				<< BUILD_LOG( m_iCnServerGroupID );
		}
		else
		{
			START_LOG( cout2, L"�ٸ� �������� ���Ӽ����� ���������� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kPacket_.m_nUID )
				<< BUILD_LOG( kPacket_.m_strName )
				<< BUILD_LOG( m_iCnServerGroupID )
				<< BUILD_LOG( m_iGsServerGroupID );
		}
		//}}
#else
		//{{ 2009. 3. 25  ������	�α�
		START_LOG( cout2, L"���Ӽ����� ���������� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_nUID )
			<< BUILD_LOG( kPacket_.m_strName );
		//}}
#endif SERV_INTEGRATION
		//}}
	}

    KActorPtr spActor( KActorManager::GetKObj()->Get( kPacket_.m_nUID ) );

    if( spActor != NULL )   // ���� ���� ����
    {
        SET_ERROR( ERR_VERIFY_03 ); // ���� UID �浹
        ReserveDestroy();
        return;
    }

    SetName( kPacket_.m_strName );
    LIF( KActorManager::GetKObj()->UpdateUID( kPacket_.m_nUID, *this ) );
    StateTransition( KDefaultFSM::I_AUTHENTICATED );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_ROOM, NULL, ERM_SQUARE_LIST_INFO_NOT, GetUID() );

	//081020.hoons.
	//SiKRoomManager()->QueueingToAll( CXSLRoom::RT_SQUARE, spEvent );
	GetKCnRoomManager()->QueueingToAll( CXSLRoom::RT_SQUARE, spEvent );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

_IMPL_ON_FUNC( ECN_UPDATE_SERVER_INFO_ACK, KServerList )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_SERVER_SET_DATA_REQ )
{
	VERIFY_STATE(( 1, KDefaultFSM::S_LOGINED ));
	
	KEGS_GET_SERVER_SET_DATA_ACK kPacket;

	kPacket.m_iOK = NetError::NET_OK;
	//KCnServer::GetKObj()->GetServeSetList( kPacket.m_ServerSetList );

	SendToGSUser( LAST_SENDER_UID, EGS_GET_SERVER_SET_DATA_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ )
{
	//{{ 2008. 7. 1  ������	 local statistics flush�� ������� ����
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD	
	KSIManager.QueueingEventID( ECN_SEND_STATISTICS_LOCLOG_REQ );
#else
	KSIManager.OnFlushLocalLog( true );
#endif SERV_STATISTICS_THREAD
	//}}
	//}}
}

IMPL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ )
{
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD	
	KSIManager.QueueingEventID( ECN_SEND_STATISTICS_DBLOG_REQ );
#else
	KSIManager.FlushData();
#endif SERV_STATISTICS_THREAD
	//}}	
}

_IMPL_ON_FUNC( ECN_UPDATE_SERVER_INFO_REQ, KServerInfo )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_iUID )
        << BUILD_LOG( kPacket_.m_wstrName );

    SendToLogDB( ECN_UPDATE_SERVER_INFO_REQ, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}
