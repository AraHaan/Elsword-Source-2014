#include "ActorManager.h"
#include "GASHAuth.h"

#ifdef SERV_COUNTRY_TWHK

#include "KncSend.h"
#include "GASHAuthManager.h"
#include "NetError.h"
#include "BaseServer.h"

#define CLASS_TYPE  KGASHAuth

KGASHAuth::KGASHAuth( void )
{
}

KGASHAuth::~KGASHAuth( void )
{
	End( 15000 );
}

void KGASHAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( EPUBLISHER_AUTHENTICATION_REQ );
    CASE( EGASH_AUTHENTICATION_ACK );

    CASE( EPUBLISHER_REG_USER_REQ );
    CASE( EGASH_AUTHORIZATION_ACK );

	CASE( EPUBLISHER_UNREG_USER_REQ );
    CASE( EGASH_ACCOUNTING_ACK );
	default:
		START_LOG( cerr, "�ڵ鷯�� �������� ���� �̺�Ʈ." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}

void KGASHAuth::GetGashAuthenticationReqPacket( const KEPUBLISHER_AUTHENTICATION_REQ& kPacket_, 
											   const std::string strServiceAccountID,
											   const std::string strIPAddress,
											   OUT std::string& strPacket )
{
	strPacket.clear();

	strPacket = SiKGASHAuthManager()->GetServiceCode();
	strPacket += "\'";
	strPacket += SiKGASHAuthManager()->GetServiceRegion();
	strPacket += "\'";
	strPacket += strServiceAccountID;
	strPacket += "\'";
	strPacket += KncUtil::toNarrowString( kPacket_.m_wstrServicePassword );
	strPacket += "\'";
	strPacket += "1";	// m_strLockFlag �ܾ� ��ȸ�� �ַ����� ��찡 �߻��Ͽ� 0���� ���� // �輮��[14.03.04] �븸 ��û���� ����(0->1)
	strPacket += "\'";
	strPacket += strIPAddress;
	strPacket += "\'";
	strPacket += strServiceAccountID;
	strPacket += "\'\r\n";

}

void KGASHAuth::GetGashAuthorizationReqPacket( const KEPUBLISHER_REG_USER_REQ& kPacket_, 
											   const std::string strServiceAccountID,
											   const std::string strIPAddress,
											   OUT std::string& strPacket )
{
	strPacket.clear();

	strPacket = SiKGASHAuthManager()->GetServiceCode();
	strPacket += "\'";
	strPacket += SiKGASHAuthManager()->GetServiceRegion();
	strPacket += "\'";
	strPacket += strServiceAccountID;
	strPacket += "\'";
	strPacket += KncUtil::toNarrowString( kPacket_.m_wstrServicePassword );
	strPacket += "\'";
	strPacket += "0";	// m_strAuthenticationFlag �ܾ� ��ȸ�� �ַ����� ��찡 �߻��Ͽ� 0���� ����
	strPacket += "\'";
	strPacket += "I";	// m_strType
	strPacket += "\'";
	strPacket += "1";	// m_strDuplicateCheck
	strPacket += "\'";
	strPacket += strIPAddress;
	strPacket += "\'";
	strPacket += strServiceAccountID;
	strPacket += "\'\r\n";

}

void KGASHAuth::GetGashAccountingReqPacket( const KEPUBLISHER_UNREG_USER_REQ& kPacket_, 
										    const std::string strServiceAccountID,
										    const std::string strIPAddress,
										    OUT std::string& strPacket )
{
	strPacket.clear();

	strPacket = SiKGASHAuthManager()->GetServiceCode();
	strPacket += "\'";
	strPacket += SiKGASHAuthManager()->GetServiceRegion();
	strPacket += "\'";
	strPacket += strServiceAccountID;
	strPacket += "\'";
	strPacket += "R";	// m_strType
	strPacket += "\'";
	strPacket += "0";	// m_strLoginTime
	strPacket += "\'";
	strPacket += "0";	// m_strLogoutTime
	strPacket += "\'";
	strPacket += strIPAddress;
	strPacket += "\'";
	strPacket += strServiceAccountID;
	strPacket += "\'\r\n";
}

IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ )
{
    START_LOG( clog, L"authenticate req" );

	// �ߺ� toNarrowString ȣ�� ���� ���� 
	std::string strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrServiceAccountID );
	std::string strstrIPAddress = KncUtil::toNarrowString( kPacket_.m_wstrIP );

    boost::shared_ptr< std::string > spPacket( new std::string );
	
	// EGASH_AUTHENTICATION_REQ
	GetGashAuthenticationReqPacket(kPacket_, strServiceAccountID, strstrIPAddress, *spPacket);

    SiKGASHAuthManager()->RegisterUserIDUserUID( strServiceAccountID, kPacket_.m_iActorUID );
	SiKGASHAuthManager()->RegisterUserIDUserIP(strServiceAccountID, strstrIPAddress);	//2011.3.16 ���� ���� ���� �� IP �ֱ� (HK/TW)
	
	// ä�� ���� ����
    SiKGASHAuthManager()->QueueingSendPacket( KGASHAuthManager::GP_AUTHENTICATION, spPacket );
}

IMPL_ON_FUNC( EGASH_AUTHENTICATION_ACK )
{	
    START_LOG( clog, L"gash authentication result" )
        << BUILD_LOG( kPacket_.m_strResultCode )
        << BUILD_LOG( kPacket_.m_strOutString )
        << BUILD_LOG( kPacket_.m_strAccountID )
        << BUILD_LOG( kPacket_.m_strPacketID )
        << END_LOG;

    UidType iActorUID = SiKGASHAuthManager()->UnregisterUserIDUserUID( kPacket_.m_strPacketID );
	std::string strActorIP = SiKGASHAuthManager()->UnregisterUserIDUserIP( kPacket_.m_strPacketID );//2011.3.16 ���� ���� ���� �� IP �ֱ� (HK/TW)

    int iResult = ::atoi( kPacket_.m_strResultCode.c_str() );
	if(1 == iResult) // Success
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iResult = 1;
		kPacketAck.m_wstrID = KncUtil::toWideString( kPacket_.m_strAccountID );
		kPacketAck.m_wstrIP = KncUtil::toWideString(strActorIP);
		
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
	}
	else
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		
		START_LOG( cerr, L"Gash ���� ����")
			<< BUILD_LOG( kPacket_.m_strAccountID )
			<< BUILD_LOG( kPacket_.m_strResultCode )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( kPacket_.m_strOutString )
			<< BUILD_LOG( kPacket_.m_strPacketID )
			<< END_LOG;
		
		// Login Error
		if(0 == iResult) 
		{
			if( kPacket_.m_strOutString.compare( "Wrong_OTP" ) == 0 )
				kPacketAck.m_iOK = NetError::ERR_GASH_12;
			else if( kPacket_.m_strOutString.compare( "Wrong_EventOTP" ) == 0 )
				kPacketAck.m_iOK = NetError::ERR_GASH_13;
			else if( kPacket_.m_strOutString.compare( "ServiceAccount_Locked_COMMLOCK" ) == 0 )
				kPacketAck.m_iOK = NetError::ERR_GASH_16;
			else if( kPacket_.m_strOutString.compare( "ServiceAccount_Locked" ) == 0 )
				kPacketAck.m_iOK = NetError::ERR_GASH_17;
			else
				kPacketAck.m_iOK = NetError::ERR_VERIFY_20;
		
		}
		// System Error
		else if(-1 == iResult) 
		{
			kPacketAck.m_iOK = NetError::ERR_VERIFY_21;
		}
		// Unknown Error
		else
		{
			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
	}

}

IMPL_ON_FUNC( EPUBLISHER_REG_USER_REQ )
{
    START_LOG( clog, L"authorize req (EPUBLISHER_REG_USER_REQ)" );

	// �ߺ� toNarrowString ȣ�� ���� ���� 
	std::string strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrServiceAccountID );
	std::string strstrIPAddress = kPacket_.m_strIP;

    boost::shared_ptr< std::string > spPacket( new std::string );

	// EGASH_AUTHORIZATION_REQ
	GetGashAuthorizationReqPacket(kPacket_, strServiceAccountID, strstrIPAddress, *spPacket);

    SiKGASHAuthManager()->RegisterUserIDUserUID( strServiceAccountID, kPacket_.m_iActorUID );

	// RegUser ���Ӽ��� ����
    SiKGASHAuthManager()->QueueingSendPacket( KGASHAuthManager::GP_AUTHORIZATION, spPacket );
}

IMPL_ON_FUNC( EGASH_AUTHORIZATION_ACK )
{
    START_LOG( clog, L"gash authorization result" )
        << BUILD_LOG( kPacket_.m_strResultCode )
        << BUILD_LOG( kPacket_.m_strPacketID )
        << END_LOG;

    UidType iActorUID = SiKGASHAuthManager()->UnregisterUserIDUserUID( kPacket_.m_strPacketID );

	KEPUBLISHER_REG_USER_ACK kPacketAck;
    int iResult = ::atoi( kPacket_.m_strResultCode.c_str() );
	kPacketAck.m_wstrServiceAccountID = KncUtil::toWideString( kPacket_.m_strPacketID ); 
	

    LOG_SUCCESS( iResult == 1 )
        << BUILD_LOG( kPacket_.m_strResultCode )
        << BUILD_LOG( iResult )
        << BUILD_LOG( kPacket_.m_strOutString )
        << BUILD_LOG( kPacket_.m_strPacketID )
        << END_LOG;

	switch( iResult )
	{
	case 1:     // success
		{
			START_LOG( clog, L"GASH authorization success" )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::NET_OK;
		}
		break;
	case 0:     // fail
		{
			START_LOG( clog, L"GASH authorization fail" )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< END_LOG;

			if( kPacket_.m_strOutString.compare( "Wrong_OTP" ) == 0 )
				kPacketAck.m_iOK = NetError::ERR_GASH_12;
			else if( kPacket_.m_strOutString.compare( "Wrong_EventOTP" ) == 0 )
				kPacketAck.m_iOK = NetError::ERR_GASH_13;
			else
				kPacketAck.m_iOK = NetError::ERR_VERIFY_20;
			
		}
		break;
	case -1:    // system error
		{
			START_LOG( clog, L"GASH authorization system error" )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_VERIFY_21;
		}
		break;
	default:
		{
			START_LOG( cerr, L"gash authorization result error" )
				<< BUILD_LOG( kPacket_.m_strResultCode )
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( kPacket_.m_strOutString )
				<< BUILD_LOG( kPacket_.m_strPacketID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		}
		break;
	}

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_LOGIN_SERVER, NULL, EPUBLISHER_REG_USER_ACK, kPacketAck );
    KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( EPUBLISHER_UNREG_USER_REQ )
{
    START_LOG( clog, L"accounting req (EPUBLISHER_UNREG_USER_REQ)" );

	// �ߺ� toNarrowString ȣ�� ���� ���� 
	std::string strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrServiceAccountID );
	std::string strstrIPAddress = kPacket_.m_strIP;

    boost::shared_ptr< std::string > spPacket( new std::string );

	// EGASH_ACCOUNTING_REQ
	GetGashAccountingReqPacket(kPacket_, strServiceAccountID, strstrIPAddress, *spPacket);
    SiKGASHAuthManager()->RegisterUserIDUserUID( strServiceAccountID, kPacket_.m_iActorUID );

	// UnRegUser ���Ӽ��� ����
    SiKGASHAuthManager()->QueueingSendPacket( KGASHAuthManager::GP_ACCOUNTING, spPacket );
}

IMPL_ON_FUNC( EGASH_ACCOUNTING_ACK )
{
    START_LOG( clog, L"gash accounting result" )
        << BUILD_LOG( kPacket_.m_strResultCode )
        << BUILD_LOG( kPacket_.m_strPacketID )
        << END_LOG;

    UidType iActorUID = SiKGASHAuthManager()->UnregisterUserIDUserUID( kPacket_.m_strPacketID );

    int iResult = ::atoi( kPacket_.m_strResultCode.c_str() );
    switch( iResult )
    {
    case 1:     // success
        break;
    case 0:     // fail
        break;
    case -1:    // system error
        break;
    default:
        START_LOG( cerr, L"gash accounting result error" )
            << BUILD_LOG( kPacket_.m_strResultCode )
            << BUILD_LOG( iResult )
            << BUILD_LOG( kPacket_.m_strOutString )
            << BUILD_LOG( kPacket_.m_strPacketID )
            << END_LOG;
        break;
    }
}

#endif // SERV_COUNTRY_TWHK