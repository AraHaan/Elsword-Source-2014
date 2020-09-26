#include "NetError.h"
#include "ActorManager.h"
#include "LoginUser.h"
#include "SimLayer.h"
//{{ 2010. 06. 04  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
	#include "NexonAuthManager.h"
#else
	#include "NexonBillingAuthManager.h"
#endif SERV_PCBANG_AUTH_NEW
//}}
//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}
//#include "GASHAuthManager.h"

#include "UserList.h"
#include "BaseServer.h"


KUserList::KUserList() :
m_uiConcurrentUser( 0 ),
//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
m_bEnableAuthWait( true ),
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
//}}
m_iGuestConcurrentUser( 0 )
{
}

KUserList::~KUserList()
{
}

bool KUserList::GetGSUserInfoByUserUID( UidType iUserUID, KGSUserInfo& kInfo )
{
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUserUID.find( iUserUID );
    if( mit == m_mapUserUID.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"������ �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

bool KUserList::GetGSUserInfoByUserID( const std::wstring& wstrUserID, KUserList::KGSUserInfo& kInfo )
{
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUserID.find( wstrUserID );
    if( mit == m_mapUserID.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"������ �̻�." )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

bool KUserList::GetGSUserInfoByUnitUID( UidType iUnitUID, KGSUserInfo& kInfo )
{
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUnitUID.find( iUnitUID );
    if( mit == m_mapUnitUID.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"������ �̻�." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

bool KUserList::GetGSUserInfoByUnitNickName( std::wstring& wstrUnitNickName, KGSUserInfo& kInfo )
{
	//To do..
	//���� ã�� ������ uid(first_sender)�� �޾ƿ� �������ڵ尡 ���� ������ �г����� ã�� ����
	//�׷� Ű���� ��Ƽ�� �������ڵ�, �г����� �Ǿ�� �ڴ�.

    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mit;

    mit = m_mapUnitNickName.find( wstrUnitNickName );
    if( mit == m_mapUnitNickName.end() )
    {
        return false;
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"������ �̻�." )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT			
            << END_LOG;

        return false;
    }

    kInfo = *( mit->second );
    return true;
}

#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
bool KUserList::GetGSUserInfoByPublisherUID( unsigned int uiPublisherUID, KUserList::KGSUserInfo& kInfo )
{
	std::map< unsigned int, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapPublisherUID.find( uiPublisherUID );
	if( mit == m_mapPublisherUID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"������ �̻�." )
			<< BUILD_LOG( uiPublisherUID )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;
}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
bool KUserList::GetGSUserInfoByMachineID( IN const std::string& strMachineID, KUserList::KGSUserInfo& kInfo )
{
	std::map< std::string, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapMachineID.find( strMachineID );
	if( mit == m_mapMachineID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"������ �̻�." )
			<< BUILD_LOG( strMachineID )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;    
}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW

void KUserList::CheckAuthSynchronize( const KENX_AUTH_SYNCHRONIZE_REQ& kInfo )
{
	//////////////////////////////////////////////////////////////////////////	
	// m_mapSynchronizeReq�� ������� �ʴٸ� ������������ ����ȭ ��û�� ���°��̴�!
	// UserID�� �˻��Ͽ� ���������� Ȯ���غ��� ����ȭ ������ ���������� ��������!

	if( kInfo.m_mapSynchronizeReq.empty() == false )
	{
		KENX_AUTH_SYNCHRONIZE_ACK kPacketAck;
		kPacketAck.m_bytePacketType = 46;
		kPacketAck.m_byteIsMonitoring = kInfo.m_byteIsMonitoring;
		__int64 iSessionNo = 0;

		std::map< __int64, std::wstring >::const_iterator mit;
		for( mit = kInfo.m_mapSynchronizeReq.begin(); mit != kInfo.m_mapSynchronizeReq.end(); ++mit )
		{
			KGSUserInfo kUserInfo;

			//{{ 2011. 09. 27	��μ�	PC �� ������ ����
#ifdef SERV_PCBANG_INCORRECT_BILLING
			BYTE byteSyncRet = ( GetGSUserInfoBySessionNo( mit->first, kUserInfo ) ? 1 : 0 );
#else
			BYTE byteSyncRet = ( GetGSUserInfoByUserID( mit->second, kUserInfo ) ? 1 : 0 );
#endif SERV_PCBANG_INCORRECT_BILLING
			//}}

			// �������ε� SessionNo�� 0�̸� ���⼭�� �Ѿ�� ����ȭ �Ϸ� �� ��α��� ó�� �Ѵ�.
			if( kUserInfo.m_iSessionNo == 0 )
			{
				START_LOG( clog, L"������ SessionNo �� 0 �Դϱ�?" )
					<< BUILD_LOG( kInfo.m_bytePacketType )
					<< BUILD_LOG( kInfo.m_byteIsMonitoring )
					<< BUILD_LOG( mit->second )
					<< BUILD_LOG( byteSyncRet )
					<< END_LOG;

				// PC �� ������ ���� ����
//				continue;
				byteSyncRet = 0;
			}

			// SessionNo�� 0�� �ƴѵ� ���� ������ �ִ� SessionNo�� �ٸ��ٸ�?
			if( kUserInfo.m_iSessionNo != mit->first )
			{
				byteSyncRet = 0;
			}

 			START_LOG( clog, L"�������� �Ѿ�� ���� ���ô�" )
 				<< BUILD_LOG( mit->first )
 				<< BUILD_LOG( byteSyncRet );

			kPacketAck.m_mapSynchronizeAck.insert( std::make_pair( mit->first, byteSyncRet ) );
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_AUTH_SYNCHRONIZE_ACK, kPacketAck );
		SiKNexonAuthManager()->QueueingEvent( spEvent );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// m_mapSynchronizeReq�� ����ִٸ� ������������ ����ȭ�� �Ϸ�Ȱ��̴�!
	// ���� ���� ����Ʈ�� Ȯ���ؼ� SessionNo�� 0�ΰ��� �� �α��� ó������!
	CheckUnauthorizedSession();
}

void KUserList::CheckUnauthorizedSession()
{
	// SessionNo�� 0�� ������ ã�Ƽ� �� �α��� ó������!

	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >::const_iterator mitID;
	for( mitID = m_mapUserID.begin(); mitID != m_mapUserID.end(); ++mitID )
	{
		const boost::shared_ptr< KGSUserInfo > spGSUserInfo = mitID->second;
		if( spGSUserInfo == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ� �̻��մϴ�." )
				<< BUILD_LOG( mitID->first )
				<< END_LOG;
			continue;
		}

		// SessionNo�� 0�̶�� �α��� ��õ�!
		if( spGSUserInfo->m_iSessionNo == 0 )
		{
			//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
			//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
//#ifdef SERV_NEXON_AUTH_SOAP

            

            int nRet1st = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, spGSUserInfo->m_ucChannelCode );
            int nRet2nd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
            int nRet3rd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, spGSUserInfo->m_ucChannelCode );

            if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
            {
                spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ���� , �⺻��
            }

			//int iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@tooni", 0 ) );
			//if( iRet != -1 ) // @�� ����. �˻� ����� �ִ�
			//{
			//	if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
			//	{
			//		START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
			//			<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
			//			<< END_LOG;
			//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
			//	}
			//}
			//else		// @�� �ִ�
			//{
   //             //CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
			//	iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@nx", 0 ) );
			//	if( iRet != -1 )
			//	{          
			//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
			//		{
			//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
			//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
			//				<< END_LOG;
			//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
			//		}
			//	}
			//	else
			//	{
			//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
			//	}
			//}

			std::wstring wstrResult;
			std::wstring wstrID;
			wstrResult = boost::str( boost::wformat( L"%d" ) % spGSUserInfo->m_ucChannelCode );
			wstrResult += ';';
			wstrID = boost::str( boost::wformat( L"%s" ) % spGSUserInfo->m_wstrUserID );
			wstrResult += wstrID;

			KGSUserInfo temp = *spGSUserInfo;
			temp.m_wstrUserID = wstrResult;

			// ���� ������ �α��� ��û
			NexonAuthLogin( temp, true );
#else
			// ���� ������ �α��� ��û
			NexonAuthLogin( *spGSUserInfo, true );
#endif SERV_TOONILAND_CHANNELING
			//}}

			START_LOG( clog2, L"�ؽ� PC�� ���� ���� : �������� ������з� ���� �α��� ��õ�!" )
				<< BUILD_LOG( spGSUserInfo->m_wstrUserID )
				<< BUILD_LOG( spGSUserInfo->m_uiIP )
				<< BUILD_LOG( spGSUserInfo->m_byteCanTry )
				<< BUILD_LOG( spGSUserInfo->m_bIsGuestUser );
		}
	}
}

int KUserList::CheckUserIDAndSetChannelCode( IN const std::wstring& wstrUserID_, IN const std::wstring& wstrPostfix_ , IN const int& nAcocuntType_, OUT UCHAR& ucChannelCode_ )
{
    int nRet = static_cast<int>( wstrUserID_.find( wstrPostfix_, 0 ) );

    if ( nRet != -1 )
    {
        if( ucChannelCode_ != nAcocuntType_ )
        {
            START_LOG( cwarn, L"�̻��� ä���ڵ�� ��� �Ǿ� �ִ�." )
                << BUILD_LOG( ucChannelCode_ )
                << BUILD_LOG( nAcocuntType_ )
                << END_LOG;
            ucChannelCode_ = nAcocuntType_;
        }

#ifdef SERV_NAVER_CHANNELING
        if ( KNexonAccountInfo::CE_NAVER_ACCOUNT == nAcocuntType_ ) 
        { // �ؽ�-���̹� ä�θ��� ���� ä���ڵ��  SSO ä���ڵ尡 �ٸ���. ���⼭ ���� ä���ڵ�� ��ȯ
            ucChannelCode_ = KNexonAccountInfo::CE_NAVER_SESSION;
        }
#endif SERV_NAVER_CHANNELING
    }

    return nRet;
}

void KUserList::NexonAuthLogin( const KGSUserInfo& kUserInfo, bool bOnlyAuthentication /*= false*/ )
{
	KENX_AUTH_LOGIN_REQ kPacketReq;
	kPacketReq.m_bytePacketType	= 42;
	kPacketReq.m_wstrUserID		= kUserInfo.m_wstrUserID;
	kPacketReq.m_uiIP			= kUserInfo.m_uiIP;

	// ���̵��� ü���� ����
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = kUserInfo.m_byteCanTry;
		kPacketReq.m_mapProperty.insert( std::make_pair( 3, kProperty ) );
	}

	// ü��ID ����
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = ( kUserInfo.m_bIsGuestUser ? 1 : 0 );
		kPacketReq.m_mapProperty.insert( std::make_pair( 4, kProperty ) );
	}

	// ���ҵ�� ���� ���� �� �����̾� ������ �Ұ����ϹǷ� OnlyAuthentication�� ����
	if( bOnlyAuthentication )
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = 1; // �Ϲ� ����
		kPacketReq.m_mapProperty.insert( std::make_pair( 12, kProperty ) );
	}
	
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM

	KPropertyValue kProperty;
	KRolicyResults kResult;

	//{{ 2012. 06. 13	��μ�       ������ �˴ٿ���
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_SELECTIVE_SHUTDOWN;
	kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // ���� �̿� ���� ��å
#else
	//  ��Ż ���� �� ��� ����
	// 	kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_OUT_USER_RETAINING;
	// 	kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // ��Ż ����

	kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_SHUTDOWN;
	kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // �˴ٿ���

	//{{ 2012. 02. 14    ��μ�   �ؽ� �Ƚ� �α��� - ��ǥ ��ġ��� �α���
#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	// ä�θ� ������ �����Ѵ�.
	if( kUserInfo.m_ucChannelCode == KNexonAccountInfo::CE_NEXON_ACCOUNT )
	{
		kResult.m_byteRolicyListNo = KENX_AUTH_LOGIN_ACK::NPN_PEACE_OF_MIND_LOGIN;
		kProperty.m_mapRolicyResult.insert( std::make_pair(kResult.m_byteRolicyListNo, kResult) ); // �Ƚ� �α���
	}
#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
	//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}


	kPacketReq.m_mapProperty.insert( std::make_pair( 17, kProperty ) );
#else
	/*
	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	// ��Ż ���� ����
	{
		KPropertyValue kProperty;
		kProperty.m_bytePropertyValue = 4;
		kPacketReq.m_mapProperty.insert( std::make_pair( 17, kProperty ) );
	}
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	*/
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_AUTH_LOGIN_REQ, kPacketReq );
	SiKNexonAuthManager()->QueueingEvent( spEvent );

	//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	RegAuthWaitUser( kUserInfo );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}
}

void KUserList::NexonAuthLogout( const std::wstring& wstrUserID, __int64 iSessionNo )
{
	KENX_AUTH_LOGOUT_NOT kPacketNot;
	kPacketNot.m_bytePacketType = 43;
	kPacketNot.m_byteLogoutType = 8;
	kPacketNot.m_wstrUserID		= wstrUserID;
	kPacketNot.m_iSessionNo		= iSessionNo;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_AUTH_LOGOUT_NOT, kPacketNot );
	SiKNexonAuthManager()->QueueingEvent( spEvent );    
}

bool KUserList::UpdateSessionNoByUserID( const std::wstring& wstrUserID, __int64 iSessionNo )
{
	std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapUserID.find( wstrUserID );
	if( mit == m_mapUserID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"������ �̻�." )
			<< BUILD_LOG( wstrUserID )
			<< END_LOG;

		return false;
	}

	//{{ 2011. 09. 27	��μ�	PC �� ������ ����
#ifdef SERV_PCBANG_INCORRECT_BILLING
	if( iSessionNo == 0 )
	{
		m_mapUserSessionNo.erase( mit->second->m_iSessionNo );
		return false;
	}
	else
	{
		std::map< UidType,	boost::shared_ptr< KGSUserInfo > >::iterator mitSessionNo = m_mapUserSessionNo.find( iSessionNo );
		if( mitSessionNo != m_mapUserSessionNo.end() )
		{
			mit->second->m_iSessionNo = iSessionNo;
			mitSessionNo->second = mit->second;
			START_LOG( clog, L"�̹� �����ϴ� ��ȣ�̴�? ���� ����" );
		}
		else
		{
			m_mapUserSessionNo.insert( std::make_pair( iSessionNo, mit->second ) );
		}
	}
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}

	mit->second->m_iSessionNo = iSessionNo;

	return true;
}

void KUserList::LoginTimeOut( const KELG_NEXON_USER_LOGIN_TIME_OUT_NOT& kInfo )
{
	KGSUserInfo kUserInfo;
	if( GetGSUserInfoByUserID( kInfo.m_wstrUserID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"���� ������ �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( kInfo.m_wstrUserID )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	//int iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@tooni", 0 ) );
	//if( iRet != -1 ) // @tooni �� �ִ�
	//{
	//	if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
	//	{
	//		START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
	//			<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//			<< END_LOG;
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
	//	}
	//}
	//else		// @�� �ִ�
	//{
	//	iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@nx", 0 ) );
	//	if( iRet != -1 ) // @nx �� �ִ�
	//	{
	//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//		{
	//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
	//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//				<< END_LOG;
	//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
	//		}
	//	}
	//	else
	//	{
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
	//	}
	//}

    int nRet1st = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, kUserInfo.m_ucChannelCode );
    int nRet2nd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, kUserInfo.m_ucChannelCode );
    int nRet3rd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, kUserInfo.m_ucChannelCode );

    if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
    {
        kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ���� , �⺻��
    }

//#else
//	int iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L'@', 0 ) );
//	if( iRet == -1 ) // @�� ����
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//		{
//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
//		}
//	}
//	else		// @�� �ִ�
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//		{
//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
//		}
//	}
#endif SERV_NEXON_AUTH_SOAP
	//}}

	std::wstring wstrResult;
	std::wstring wstrID;
	wstrResult = boost::str( boost::wformat( L"%d" ) % kUserInfo.m_ucChannelCode );
	wstrResult += ';';
	wstrID = boost::str( boost::wformat( L"%s" ) % kUserInfo.m_wstrUserID );
	wstrResult += wstrID; // wstrResult �� ä�θ��ڵ�(���ǿ�);userid@naver �� ���յ� ���� -SSOä�θ��ڵ� �ƴ�-�� ����

	// ���������� �α׾ƿ� ��û
	NexonAuthLogout( wstrResult, kUserInfo.m_iSessionNo ); 
#else
	// ���������� �α׾ƿ� ��û
	NexonAuthLogout( kInfo.m_wstrUserID, kUserInfo.m_iSessionNo );
#endif SERV_TOONILAND_CHANNELING
	//}}

	START_LOG( clog2, L"�ؽ� PC�� ���� ���� ���� : Ÿ�� �ƿ����� ���� ���� �α׾ƿ�!" )
		<< BUILD_LOG( kInfo.m_wstrUserID )
		<< BUILD_LOG( kUserInfo.m_iSessionNo );
	
	// SessionNo�� 0���� ����!
	LIF( UpdateSessionNoByUserID( kInfo.m_wstrUserID, 0 ) );
	
	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	//iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@tooni", 0 ) );
	//if( iRet != -1 ) // @tooni�� �ִ�
	//{
	//	if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
	//	{
	//		START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
	//			<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//			<< END_LOG;
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
	//	}
	//}
	//else		// @�� �ִ�
	//{
	//	iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L"@nx", 0 ) );
	//	if( iRet != -1 ) // @nx�� �ִ�
	//	{
	//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//		{
	//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
	//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
	//				<< END_LOG;
	//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
	//		}
	//	}
	//	else
	//	{
	//		kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
	//	}
	//}

    nRet1st = nRet2nd = nRet3rd = -1;
    nRet1st = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, kUserInfo.m_ucChannelCode );
    nRet2nd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, kUserInfo.m_ucChannelCode );
    nRet3rd = CheckUserIDAndSetChannelCode( kUserInfo.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, kUserInfo.m_ucChannelCode );  // TODO : ���̹� ä�θ��� ��쿡�� ���� ä�θ��ڵ�� ��ȯ�ؾ� �Ѵ�. 

    if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
    {
        kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ���� , �⺻��
    }
//#else
//	iRet = static_cast<int>( kUserInfo.m_wstrUserID.find( L'@', 0 ) );
//	if( iRet == -1 ) // @�� ����
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//		{
//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
//		}
//	}
//	else		// @�� �ִ�
//	{
//		if( kUserInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//		{
//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//				<< BUILD_LOG( kUserInfo.m_ucChannelCode )
//				<< END_LOG;
//			kUserInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
//		}
//	}
#endif SERV_NEXON_AUTH_SOAP
	//}}

	wstrResult.clear();
	wstrID.clear();
	wstrResult = boost::str( boost::wformat( L"%d" ) % kUserInfo.m_ucChannelCode );
	wstrResult += ';';
	wstrID = boost::str( boost::wformat( L"%s" ) % kUserInfo.m_wstrUserID );
	wstrResult += wstrID;

	KGSUserInfo temp = kUserInfo;
	temp.m_wstrUserID = wstrResult;

	// ���� ������ �α��� ��û
	NexonAuthLogin( temp, true );
#else
	// ���� ������ �α��� ��û
	NexonAuthLogin( kUserInfo, true );
#endif SERV_TOONILAND_CHANNELING
	//}}

	START_LOG( clog2, L"�ؽ� PC�� ���� ���� ���� : Ÿ�� �ƿ����� ���� ���� �α���!" )
		<< BUILD_LOG( kUserInfo.m_wstrUserID )
		<< BUILD_LOG( kUserInfo.m_uiIP )
		<< BUILD_LOG( kUserInfo.m_byteCanTry )
		<< BUILD_LOG( kUserInfo.m_bIsGuestUser );
}

#endif SERV_PCBANG_AUTH_NEW
//}}

int KUserList::RegUser( IN const UidType iUserUID, IN const UidType iGSUID, IN const KELG_REGISTER_USER_REQ& kInfo_, IN const bool bChannelChanged )
{
	// 1. �������� �� �˻�
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"���� UID �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_01;
    }

    if( kInfo_.m_wstrUserID.empty() )
    {
        START_LOG( cerr, L"���� ID �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_02;
    }

    if( iGSUID <= 0 )
    {
        START_LOG( cerr, L"���� ���� UID �̻�." )
            << BUILD_LOG( iGSUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_05;
    }

	// 2. ���� ���� ����
    boost::shared_ptr< KGSUserInfo > spGSUserInfo( new KGSUserInfo() );
    spGSUserInfo->m_nUserUID	 = iUserUID;
    spGSUserInfo->m_wstrUserID	 = kInfo_.m_wstrUserID;
    spGSUserInfo->m_nGSUID		 = iGSUID;
	spGSUserInfo->m_iChannelID	 = kInfo_.m_iChannelID; // ä��ID
    spGSUserInfo->m_uiIP		 = kInfo_.m_uiIP;
    spGSUserInfo->m_byteCanTry	 = kInfo_.m_byteCanTry;	
	spGSUserInfo->m_bIsGuestUser = IsGuestUser( kInfo_.m_byteGuestUser ); // ü��ID ����
	spGSUserInfo->m_cAuthLevel	 = kInfo_.m_cAuthLevel;
	//{{ 2010. 07. 01  ������	PC�� ���� ���� ����
	spGSUserInfo->m_bChannelChanged = bChannelChanged;
	//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	spGSUserInfo->m_uiPublisherUID = kInfo_.m_uiPublisherUID;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
	//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	//int iRet = static_cast<int>( kInfo_.m_wstrUserID.find( L"@tooni", 0 ) );
	//if( iRet != -1 ) // @tooni �ִ�
	//{
	//	if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//	{
	//		START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
	//			<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
	//			<< END_LOG;
	//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // ���Ϸ�������
	//	}
	//}
	//else		
	//{
	//	iRet = static_cast<int>( kInfo_.m_wstrUserID.find( L"@nx", 0 ) );
	//	if( iRet != -1 ) // @nx �ִ�
	//	{
	//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	//		{
	//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
	//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
	//				<< END_LOG;
	//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
	//		}
	//	}
	//	else
	//	{
	//		spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT;	// �ؽ� ����
	//	}
	//}

    int nRet1st = CheckUserIDAndSetChannelCode( kInfo_.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, spGSUserInfo->m_ucChannelCode );
    int nRet2nd = CheckUserIDAndSetChannelCode( kInfo_.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
    int nRet3rd = CheckUserIDAndSetChannelCode( kInfo_.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, spGSUserInfo->m_ucChannelCode );

    if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
    {
        spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ���� , �⺻��
    }


//#else
//	int iRet = static_cast<int>( kInfo_.m_wstrUserID.find( L'@', 0 ) );
//	if( iRet == -1 ) // @�� ����
//	{
//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//		{
//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//				<< END_LOG;
//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
//		}
//	}
//	else		// @�� �ִ�
//	{
//		if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//		{
//			START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//				<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//				<< END_LOG;
//			spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
//		}
//	}
#endif SERV_NEXON_AUTH_SOAP
	//}}

#endif SERV_TOONILAND_CHANNELING
	//}}
#ifdef SERV_JAPAN_CHANNELING
	spGSUserInfo->m_ucChannelCode = kInfo_.m_ucChannelCode;
#endif // SERV_JAPAN_CHANNELING

	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	spGSUserInfo->m_strMachineID = kInfo_.m_strMachineID;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUserID;
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	// �ߺ��Ǵ� �ӽ�ID�� �ִ��� Ȯ��!
	if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// ���� �׼������� �˻� ����
	{
		if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
		{
			if( kInfo_.m_strMachineID.empty() )
			{
				START_LOG( cout, L"[�˸�] �ӽ�ID�� ����ִ� ���� �߰�!" )
					<< BUILD_LOG( kInfo_.m_wstrUserID )
					<< BUILD_LOGc( kInfo_.m_cAuthLevel )
					<< BUILD_LOG( kInfo_.m_iChannelID )
					<< BUILD_LOG( kInfo_.m_strMachineID );
			}
			else
			{
				if( m_mapMachineID.find( kInfo_.m_strMachineID ) != m_mapMachineID.end() )
				{
					// ��ڱ� �̻��� �ƴϸ� �ӽ�ID�� �ߺ� �����Ѱ����� üũ�Ѵ�!
					if( kInfo_.m_cAuthLevel < SEnum::UAL_GM )
					{
						// �系������ �ƴҶ��� �˻��ϱ�!
						if( KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_INTERNAL )
						{
							//{{ 2012. 10. 12	������		2�� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX_COMPLETE
							KGSUserInfo kUserInfo;
							if( GetGSUserInfoByMachineID( kInfo_.m_strMachineID, kUserInfo ) == true )
							{
								KLoginUserPtr spServer;
								KActorManager::GetKObj()->Get( kUserInfo.m_nGSUID, spServer );
								if( IS_NOT_NULL( spServer ) )
								{
									START_LOG( cout, L"[�˸�] �ӽ�ID �ߺ� ����!! ������ ������ �ִ� ������ ���� ���� ��ŵ�ϴ�!" )
										<< BUILD_LOG( kUserInfo.m_nUserUID )
										<< BUILD_LOG( kUserInfo.m_wstrUserID )
										<< BUILD_LOG( kInfo_.m_wstrUserID )
										<< BUILD_LOG( kUserInfo.m_strMachineID );

									// ������ �����ߴ� ������ ƨ�ܳ����� �Ѵ�.
									int kPacket = NetError::ERR_USER_REGISTRATION_10;
									KEvent kEvent;
									kEvent.SetData( PI_GS_USER, NULL, E_KICK_USER_NOT, kPacket );
									LIF( kEvent.m_kDestPerformer.AddUID( iUserUID ) );
									spServer->KSession::SendPacket( kEvent );
								}
							}
#endif SERV_DUPLICATE_CONNECT_BUG_FIX_COMPLETE
							//}}

							START_LOG( cout, L"[�˸�] �ӽ�ID�� �ߺ��Ǵ� ������ �ֽ��ϴ�! ��ŷ�����ΰ�!?" )
								<< BUILD_LOG( kInfo_.m_wstrUserID )
								<< BUILD_LOG( kInfo_.m_strMachineID );

							// ���� �������� ������ ƨ�ܳ���.
							return NetError::ERR_USER_REGISTRATION_10;
						}
						else
						{
							START_LOG( cout, L"[�˸�] �Ϲ� �������� ���ϸӽ� �ߺ� �����Ͽ����� ���� ������ �系 �����̹Ƿ� ���� ���� ó�� �մϴ�." )
								<< BUILD_LOG( kInfo_.m_wstrUserID )
								<< BUILD_LOGc( kInfo_.m_cAuthLevel )
								<< BUILD_LOG( kInfo_.m_iChannelID )
								<< BUILD_LOG( kInfo_.m_strMachineID );
						}
					}
					else
					{
						START_LOG( cout, L"[�˸�] ��� �Ǵ� �����ڰ� ���� �ӽ����� �ߺ� ���� �õ��� �Ͽ����ϴ�! Ȯ�� �ٶ��ϴ�." )
							<< BUILD_LOG( kInfo_.m_wstrUserID )
							<< BUILD_LOGc( kInfo_.m_cAuthLevel )
							<< BUILD_LOG( kInfo_.m_iChannelID )
							<< BUILD_LOG( kInfo_.m_strMachineID );
					}
				}
			}
		}
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	// 3. �̹� �����ϴ� ���� �������� Ȯ��
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID != m_mapUserUID.end() )
    {
        START_LOG( cerr, L"���� ���� ����." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        if( !mitUserUID->second )
        {
            START_LOG( cerr, L"������ �̻�." )
                << BUILD_LOG( iUserUID )
                << END_LOG;

            // �Ͼ���� �ȵǴ� ��Ȳ. �׳� �� ������ ���� ����.
            m_mapUserUID.erase( mitUserUID );
            m_mapUserUID.insert( std::make_pair( iUserUID, spGSUserInfo ) );

            mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
            if( mitUserID != m_mapUserID.end() )
            {
                if( mitUserID->second )
                {
                    START_LOG( cerr, L"������ �̻��� �ƴ�." )
                        << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                        << BUILD_LOG( mitUserID->first )
#endif SERV_PRIVACY_AGREEMENT
                        << BUILD_LOG( mitUserID->second->m_nUserUID )
                        << END_LOG;
                }
                m_mapUserID.erase( mitUserID );
            }
            m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
			//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			if( kInfo_.m_strMachineID.empty() == false )
			{
				m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
			}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
            goto end_proc;
        }

        if( mitUserUID->second->m_nGSUID == iGSUID )
        {
            START_LOG( cerr, L"���� ���� ���� ��������." )  // �̹� �˻縦 �� ����̹Ƿ� �Ͼ �� ����.
                << BUILD_LOG( iUserUID )                    // �ƴϸ� ���� ������ ������ �������� ����̴�.
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( iGSUID )
                << END_LOG;

            mitUserUID->second.reset();
            mitUserUID->second = spGSUserInfo;
            mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
            if( mitUserID != m_mapUserID.end() )
            {
                START_LOG( cerr, L"UserID �ʿ� ����." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;

                m_mapUserID.erase( mitUserID );
            }
            else
            {
                START_LOG( cerr, L"UserID �ʿ� ����." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;
            }
            m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
			//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			if( kInfo_.m_strMachineID.empty() == false )
			{
				m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
			}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
            goto end_proc;
        }

        KLoginUserPtr spServer;
        KActorManager::GetKObj()->Get( mitUserUID->second->m_nGSUID, spServer );
        if( spServer )   // Ÿ ������ ��������
        {
            START_LOG( cerr2, L"Ÿ ������ �������� ����." )
                << BUILD_LOG( spServer->GetName() )
                << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;

            // ������ �����ߴ� ������ ƨ�ܳ����� �Ѵ�.
            int kPacket = NetError::ERR_VERIFY_01;
            KEvent kEvent;
            kEvent.SetData( PI_GS_USER, NULL, E_KICK_USER_NOT, kPacket );
            LIF( kEvent.m_kDestPerformer.AddUID( iUserUID ) );
            spServer->KSession::SendPacket( kEvent );

            // ���� �����Ϸ��� ������ ƨ�ܳ���.
            return NetError::ERR_USER_REGISTRATION_00;
        }
        else    // ������ �������־��ٴ� ������ ����Ǿ����� ����. ���� �ٿ� ������ �߸��� �����Ͱ� �����ִ� ���.
        {
            KLoginUserPtr spServerCurrent;
            KActorManager::GetKObj()->Get( iGSUID, spServerCurrent );
            if( !spServerCurrent )
            {
                // ������ �ױ� ���� ���� ��û�� �� ���ϱ�? ��·�� �� ������ ����Ʈ�� �߰��ϸ� �ȵȴ�.
                START_LOG( cerr, L"���� ��û�� �� ������ ���� ������ ����Ǿ� ���� ����." )
                    << BUILD_LOG( iGSUID )
                    << BUILD_LOG( iUserUID )
                    << END_LOG;

                return NetError::ERR_USER_REGISTRATION_07;
            }

            mitUserUID->second.reset();
            mitUserUID->second = spGSUserInfo;

            mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
            if( mitUserID != m_mapUserID.end() )
            {
                START_LOG( cerr, L"UserID �ʿ� ����." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;

                m_mapUserID.erase( mitUserID );
            }
            else
            {
                START_LOG( cerr, L"UserID �ʿ� ����." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( iGSUID )
                    << END_LOG;
            }

            m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
			//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			if( kInfo_.m_strMachineID.empty() == false )
			{
				m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
			}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
            goto end_proc;
        }
    }

	// 4. ���� ���� ����
    m_mapUserUID.insert( std::make_pair( iUserUID, spGSUserInfo ) );
    mitUserID = m_mapUserID.find( kInfo_.m_wstrUserID );
    if( mitUserID != m_mapUserID.end() )
    {
        START_LOG( cerr, L"UserUID �ʿ��� ���ٰ� Ȯ�εǾ��µ� UserID �ʿ��� ����." )
            << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( kInfo_.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
            << END_LOG;

        m_mapUserID.erase( mitUserID );
    }
    m_mapUserID.insert( std::make_pair( kInfo_.m_wstrUserID, spGSUserInfo ) );
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	if( kInfo_.m_strMachineID.empty() == false )
	{
		m_mapMachineID.insert( std::make_pair( kInfo_.m_strMachineID, spGSUserInfo ) );
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	// 4.1. �ۺ��� ���� ����
	if( kInfo_.m_uiPublisherUID != 0)
	{
		std::map< unsigned int, boost::shared_ptr< KGSUserInfo > >::iterator mitPublisherUID;

		mitPublisherUID = m_mapPublisherUID.find( kInfo_.m_uiPublisherUID );
		if( mitPublisherUID != m_mapPublisherUID.end() )
		{
			START_LOG( cerr, L"PublisherUID �ʿ� ����." )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( kInfo_.m_uiPublisherUID )
				<< END_LOG;

			m_mapPublisherUID.erase( mitPublisherUID );
		}
		m_mapPublisherUID.insert( std::make_pair( kInfo_.m_uiPublisherUID, spGSUserInfo ) );
	}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

	// 5. ���� ���� ����
	SetConcurrentUserCount( m_mapUserUID.size() );
    goto end_proc;

end_proc:
    // 6. ���� ���� �α��� - �������� ���� ����̸� ����� �´�.
    if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
    {
		//{{ 2010. 06. 04  ������	�ؽ�PC�� ���� ���� ����
//#ifdef SERV_PCBANG_AUTH_NEW
		//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
//#ifdef SERV_TOONILAND_CHANNELING
		//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
//#ifdef SERV_NEXON_AUTH_SOAP
		/*int iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@tooni", 0 ) );
		if( iRet != -1 ) // @tooni �ִ�
		{
			if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
			{
				START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
					<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
					<< END_LOG;
				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
			}
		}
		else		
		{
			iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L"@nx", 0 ) );
			if( iRet != -1 ) // @nx �ִ�
			{
				if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
				{
					START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
						<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
						<< END_LOG;
					spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
				}
			}
			else
			{
				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
			}
		}*/

        int nRet1st = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, spGSUserInfo->m_ucChannelCode );
        int nRet2nd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, spGSUserInfo->m_ucChannelCode );
        int nRet3rd = CheckUserIDAndSetChannelCode( spGSUserInfo->m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, spGSUserInfo->m_ucChannelCode );

        if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
        {
            spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ���� , �⺻��
        }

//#else
//		int iRet = static_cast<int>( spGSUserInfo->m_wstrUserID.find( L'@', 0 ) );
//		if( iRet == -1 ) // @�� ����
//		{
//			if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//			{
//				START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//					<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//					<< END_LOG;
//				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
//			}
//		}
//		else		// @�� �ִ�
//		{
//			if( spGSUserInfo->m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//			{
//				START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//					<< BUILD_LOG( spGSUserInfo->m_ucChannelCode )
//					<< END_LOG;
//				spGSUserInfo->m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
//			}
//		}
//#endif SERV_NEXON_AUTH_SOAP
		//}}
		
		std::wstring wstrResult;
		std::wstring wstrID;
		wstrResult = boost::str( boost::wformat( L"%d" ) % spGSUserInfo->m_ucChannelCode );
		wstrResult += ';';
		wstrID = boost::str( boost::wformat( L"%s" ) % spGSUserInfo->m_wstrUserID );
		wstrResult += wstrID;

		KGSUserInfo temp = *spGSUserInfo;
		temp.m_wstrUserID = wstrResult;

		// ���� ������ �α��� ��û
		NexonAuthLogin( temp );
//#else
//		// ���������� �α��� ��û
//		NexonAuthLogin( *spGSUserInfo );
//#endif SERV_TOONILAND_CHANNELING
		//}}
		
//#else
//		//////////////////////////////////////////////////////////////////////////
//		KENX_USER_LOGIN_REQ kPacketReq;
//
//		//if( StrRChrW( wstrUserID.c_str(), wstrUserID.c_str()+wstrUserID.size(), L'/' ) != NULL )
//		//{
//		//	kPacketReq.m_wstrUserID = wstrUserID.substr( 0, wstrUserID.size() - 1 );
//		//}
//
//		kPacketReq.m_bytePacketType = 39;
//		kPacketReq.m_byteLogin		= 4;
//		kPacketReq.m_wstrUserID		= kInfo_.m_wstrUserID;
//		kPacketReq.m_wstrUnitNickName.clear();
//		kPacketReq.m_uiIP			= kInfo_.m_uiIP;
//		kPacketReq.m_byteCanTry		= kInfo_.m_byteCanTry;
//		kPacketReq.m_byteGuestUser	= kInfo_.m_byteGuestUser;
//
//		KEventPtr spEvent( new KEvent );
//		spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_USER_LOGIN_REQ, kPacketReq );
//		SiKNexonBillingAuthManager()->QueueingEvent( spEvent );
//#endif SERV_PCBANG_AUTH_NEW
		//}}

		//{{ 2008. 5. 27  ������  ü��ID ����
		if( IsGuestUser( kInfo_.m_byteGuestUser ) )
		{
			IncreaseGuestConcurrentUser();
		}
		//}}
    }
	
	else
	{
		//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
		RegAuthWaitUser( *spGSUserInfo );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
		//}}
#ifdef SERV_GLOBAL_AUTH
#if defined(SERV_COUNTRY_TWHK) || defined(SERV_COUNTRY_JP)
		RequestRegUser( iUserUID, *spGSUserInfo );	// �ؿܿ� ���� ��û
#elif defined(SERV_COUNTRY_TH)
		RequestRegUser( iUserUID, *spGSUserInfo, kInfo_.m_wstrSocketID );	// �ؿܿ� ���� ��û
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH
	}

    return NetError::NET_OK;
}

bool KUserList::UnRegUser( IN const UidType iUserUID )
{
	// 1. �������� �˻�
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"���� UID �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUserID;

	// 2. �����Ϸ��� ������ �ִ��� �˻�
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID == m_mapUserUID.end() )
    {
        START_LOG( cerr, L"�����Ϸ��� ������ ����." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    if( !mitUserUID->second )
    {
        START_LOG( cerr, L"���� ���� ������ �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        m_mapUserUID.erase( mitUserUID );
        return false;
    }

    // ������ ��ϵǾ� ������ �����Ѵ�. 
    // -> ���� ��ü ���� �ÿ� ���� ������ ��ġ�� �ʰ� ���� ������ �ؿ����� �� ���� ������ ���� �ϵ��� ������.
    // LIF( !UnRegUnit( iUserUID ) );

	//{{ 2009. 7. 9  ������		�������� ���� ����� �������� ������ �Ѵ�!
	// 3. ���� ������ ���� �����ִ��� �˻�
	if( mitUserUID->second->m_nUnitUID > 0 )
	{
		START_LOG( cerr, L"���������� �������� �ʾҴµ� ���������� ����� �Ѵ�!" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( mitUserUID->second->m_nUnitUID )
			<< END_LOG;
	}
	//}}
	
    KGSUserInfo kInfo = *( mitUserUID->second );
	//{{ 2010. 06. 28  ������	���� PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
	const __int64 iSessionNo = kInfo.m_iSessionNo;
#endif SERV_PCBANG_AUTH_NEW
	//}}

	// 4. ���� ���� ����
    if( kInfo.m_wstrUserID.empty() )
    {
        START_LOG( cerr, L"���� ���̵� �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;
    }
    else
    {
        mitUserID = m_mapUserID.find( kInfo.m_wstrUserID );
        if( mitUserID == m_mapUserID.end() )
        {
            START_LOG( cerr, L"���� ID �� �˻� ����." )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( kInfo.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( iUserUID )
                << END_LOG;
        }
        else
        {
            m_mapUserID.erase( mitUserID );
        }
    }

	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	// �ӽ�ID���� ����
	if( kInfo.m_strMachineID.empty() )
	{
		START_LOG( cout, L"[�˸�] �ӽ�ID�� ����ִ� ������ �α� �ƿ� �Ͽ����ϴ�!" )
			<< BUILD_LOG( kInfo.m_wstrUserID )
			<< BUILD_LOGc( kInfo.m_cAuthLevel )
			<< BUILD_LOG( kInfo.m_iChannelID );
	}
	else
	{
		std::map< std::string, boost::shared_ptr< KGSUserInfo > >::iterator mitMachineID;
		mitMachineID = m_mapMachineID.find( kInfo.m_strMachineID );
		if( mitMachineID == m_mapMachineID.end() )
		{
			START_LOG( cerr, L"�ӽ�ID �� �˻� ����." )
				<< BUILD_LOG( kInfo.m_strMachineID )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
		}
		else
		{
			m_mapMachineID.erase( mitMachineID );
		}
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	//{{ 2011. 09. 27	��μ�	PC �� ������ ����
#ifdef SERV_PCBANG_INCORRECT_BILLING
	std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitSessionNo = m_mapUserSessionNo.find( iSessionNo );
	if( mitSessionNo != m_mapUserSessionNo.end() )
	{
		m_mapUserSessionNo.erase( mitSessionNo );
	}
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}

    m_mapUserUID.erase( mitUserUID );

#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	// 4.1. �ۺ��� ���� ����
	std::map< unsigned int, boost::shared_ptr< KGSUserInfo > >::iterator mitPublisherUID;

	if( kInfo.m_uiPublisherUID == 0)
	{
		START_LOG( cerr, L"�ۺ��� UID �̻�." )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( kInfo.m_wstrUserID )
			<< BUILD_LOG( kInfo.m_uiPublisherUID )
			<< END_LOG;
	}
	else
	{
		mitPublisherUID = m_mapPublisherUID.find( kInfo.m_uiPublisherUID );
		if( mitPublisherUID == m_mapPublisherUID.end() )
		{
			START_LOG( cerr, L"�ۺ��� UID �� �˻� ����." )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( kInfo.m_wstrUserID )
				<< BUILD_LOG( kInfo.m_uiPublisherUID )
				<< END_LOG;
		}
		else
		{
			m_mapPublisherUID.erase( mitPublisherUID );
		}
	}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

	// 5. ���� ���� ����
	SetConcurrentUserCount( m_mapUserUID.size() );

	// 6. ���� ���� �α� �ƿ�
    switch( KSimLayer::GetKObj()->GetAuthFlag() )
    {
    case KSimLayer::AF_NEXON_KOREA:
        {
			//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
			// ���������� �α׾ƿ� ��û
			//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
			//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
			/*int iRet = static_cast<int>( kInfo.m_wstrUserID.find( L"@tooni", 0 ) );
			if( iRet != -1 ) // @tooni �ִ�
			{
				if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
				{
					START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
						<< BUILD_LOG( kInfo.m_ucChannelCode )
						<< END_LOG;
					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
				}
			}
			else		
			{
				iRet = static_cast<int>( kInfo.m_wstrUserID.find( L"@nx", 0 ) );
				if( iRet != -1 ) // @nx �ִ�
				{
					if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
					{
						START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
							<< BUILD_LOG( kInfo.m_ucChannelCode )
							<< END_LOG;
						kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
					}
				}
				else
				{
					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
				}
			}*/

            int nRet1st = CheckUserIDAndSetChannelCode( kInfo.m_wstrUserID, L"@tooni", KNexonAccountInfo::CE_TOONILAND_ACCOUNT, kInfo.m_ucChannelCode );
            int nRet2nd = CheckUserIDAndSetChannelCode( kInfo.m_wstrUserID, L"@nx", KNexonAccountInfo::CE_NEXON_ACCOUNT, kInfo.m_ucChannelCode );
            int nRet3rd = CheckUserIDAndSetChannelCode( kInfo.m_wstrUserID, L"@naver", KNexonAccountInfo::CE_NAVER_ACCOUNT, kInfo.m_ucChannelCode );

            if ( nRet1st == -1 && nRet2nd == -1 && nRet3rd == -1 ) 
            {
                kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ���� , �⺻��
            }
//#else
//			int iRet = static_cast<int>( kInfo.m_wstrUserID.find( L'@', 0 ) );
//			if( iRet == -1 ) // @�� ����
//			{
//				if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
//				{
//					START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//						<< BUILD_LOG( kInfo.m_ucChannelCode )
//						<< END_LOG;
//					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT; // �ؽ� ����
//				}
//			}
//			else		// @�� �ִ�
//			{
//				if( kInfo.m_ucChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
//				{
//					START_LOG( clog, L"�̻��� ä���ڵ�� ���� �Ǿ� �ִ�." )
//						<< BUILD_LOG( kInfo.m_ucChannelCode )
//						<< END_LOG;
//					kInfo.m_ucChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT; // ���Ϸ�������
//				}
//			}
#endif SERV_NEXON_AUTH_SOAP
			//}}
		
			std::wstring wstrResult;
			std::wstring wstrID;
		
			wstrResult = boost::str( boost::wformat( L"%d" ) % kInfo.m_ucChannelCode );
			wstrResult += ';';
			wstrID = boost::str( boost::wformat( L"%s" ) % kInfo.m_wstrUserID );
			wstrResult += wstrID;
			
			NexonAuthLogout( wstrResult, iSessionNo );
#else
			NexonAuthLogout( kInfo.m_wstrUserID, iSessionNo );
#endif SERV_TOONILAND_CHANNELING
			//}}
#else
			//////////////////////////////////////////////////////////////////////////
			KENX_USER_LOGOUT_REQ kPacketReq;

			//if( StrRChrW( kInfo.m_wstrUserID.c_str(), kInfo.m_wstrUserID.c_str()+kInfo.m_wstrUserID.size(), L'/' ) != NULL )
			//{
			//	kPacketReq.m_wstrUserID = kInfo.m_wstrUserID.substr( 0, kInfo.m_wstrUserID.size() - 1 );
			//}

			kPacketReq.m_bytePacketType = 39;
			kPacketReq.m_byteLogin		= 1;
			kPacketReq.m_wstrUserID		= kInfo.m_wstrUserID;
			kPacketReq.m_wstrUnitNickName.clear();
			kPacketReq.m_uiIP			= kInfo.m_uiIP;
			kPacketReq.m_byteCanTry		= kInfo.m_byteCanTry;
			kPacketReq.m_byteGuestUser	= kInfo.m_bIsGuestUser;

			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ENX_USER_LOGOUT_REQ, kPacketReq );
			SiKNexonBillingAuthManager()->QueueingEvent( spEvent );
			//////////////////////////////////////////////////////////////////////////
#endif SERV_PCBANG_AUTH_NEW
			//}}            
			//{{ 2011. 09. 27	��μ�	PC �� ������ ����
#ifdef SERV_PCBANG_INCORRECT_BILLING
			m_mapUserSessionNo.erase( iSessionNo );
#endif SERV_PCBANG_INCORRECT_BILLING
			//}}

			//{{ 2008. 5. 27  ������  ü��ID ����
			if( kInfo.m_bIsGuestUser )
			{
				DecreaseGuestConcurrentUser();
			}
			//}}
		}
        break;
    default:
        break;
    }

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_TWHK
	RequestUnRegUser(iUserUID, kInfo);
#endif // SERV_COUNTRY_TWHK
#endif // SERV_GLOBAL_AUTH

    return true;
}

void KUserList::UnRegAllUserOfThisGS( IN const UidType iGSUID )
{
    START_LOG( cout, L"���� ���� ���� �� ó��." )
        << BUILD_LOG( iGSUID );

    if( iGSUID <= 0 )
    {
        START_LOG( cerr, L"���� UID �̻�." )
            << BUILD_LOG( iGSUID )
            << END_LOG;

        return;
    }

    std::vector< UidType > vecUserUID;
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    for( mitUserUID = m_mapUserUID.begin(); mitUserUID != m_mapUserUID.end(); mitUserUID++ )
    {
        if( !mitUserUID->second )
        {
            START_LOG( cerr, L"���� ���� ������ �̻�." )
                << BUILD_LOG( mitUserUID->first )
                << END_LOG;

            continue;
        }

        if( mitUserUID->second->m_nGSUID == iGSUID )
        {
            vecUserUID.push_back( mitUserUID->first );
        }
    }

    std::vector< UidType >::iterator vitUserUID;
    for( vitUserUID = vecUserUID.begin(); vitUserUID != vecUserUID.end(); vitUserUID++ )
    {
        UnRegUser( *vitUserUID );
    }

    return;
}

int KUserList::RegUnit( IN const UidType iUserUID, IN const UidType iUnitUID, IN const std::wstring& wstrUnitNickName, IN const unsigned int uiKNMSerialNum, IN const bool bDenyFriendShip )
{
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"���� UID �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_01;
    }

    if( iUnitUID <= 0 )
    {
        START_LOG( cerr, L"���� UID �̻�." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_03;
    }

    if( wstrUnitNickName.empty() )
    {
        START_LOG( cerr, L"���� �г��� �̻�." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_04;
    }

	if( uiKNMSerialNum <= 0  )
	{
		START_LOG( cerr, L"���� �޽��� SN �̻�." )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( uiKNMSerialNum )
			<< END_LOG;

		return NetError::ERR_USER_REGISTRATION_09;
	}

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID == m_mapUserUID.end() )
    {
        START_LOG( cerr, L"������ ����Ϸ��µ� ������ ��ϵǾ� ���� ����." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_08;
    }

    if( !mitUserUID->second )
    {
        START_LOG( cerr, L"������ �̻�." )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return NetError::ERR_USER_REGISTRATION_06;
    }

	//UID�� �˻�
    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    mitUnitUID = m_mapUnitUID.find( mitUserUID->second->m_nUnitUID );
    if( mitUnitUID != m_mapUnitUID.end() )
    {
        START_LOG( cerr, L"UnReg ���� ���� ����." )
            << BUILD_LOG( mitUnitUID->first )
            << END_LOG;

        if( !mitUnitUID->second )
        {
            START_LOG( cerr, L"������ �̻�." )
                << BUILD_LOG( iUserUID )
                << BUILD_LOG( iUnitUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
        else
        {
            LIF( mitUnitUID->second->m_nUserUID == iUserUID );
        }
        m_mapUnitUID.erase( mitUnitUID );
    }
    m_mapUnitUID.insert( std::make_pair( iUnitUID, mitUserUID->second ) );

	//�г��� �˻�
    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitNickName;
    mitUnitNickName = m_mapUnitNickName.find( mitUserUID->second->m_wstrUnitNickName );
    if( mitUnitNickName != m_mapUnitNickName.end() )
    {
        START_LOG( cerr, L"UnReg ���� ���� ���� �г���." )
#ifndef SERV_PRIVACY_AGREEMENT
            << BUILD_LOG( mitUnitNickName->first )
#endif SERV_PRIVACY_AGREEMENT
            << END_LOG;

        if( !mitUnitNickName->second )
        {
            START_LOG( cerr, L"������ �̻�." )
                << BUILD_LOG( iUserUID )
                << BUILD_LOG( iUnitUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
        else
        {
            LIF( mitUnitNickName->second->m_nUserUID == iUserUID );
        }
        m_mapUnitNickName.erase( mitUnitNickName );
    }
    m_mapUnitNickName.insert( std::make_pair( wstrUnitNickName, mitUserUID->second ) );

	//���� ������ �˻縦 ��ģ�� ����ó���� �Ѵ��� ������ ����
	//������ �̱⶧���� �Ѱ����� �ϸ�ȴ�.
	mitUserUID->second->m_nUnitUID			= iUnitUID;
	mitUserUID->second->m_wstrUnitNickName	= wstrUnitNickName;
	mitUserUID->second->m_uiKNMSerialNum	= uiKNMSerialNum;
	//{{ 2009. 4. 1  ������		ģ������
	mitUserUID->second->m_bDenyFriendShip	= bDenyFriendShip;
	//}}

    return NetError::NET_OK;
}

bool KUserList::UnRegUnit( IN const UidType iUserUID )
{
    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"���� UID �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUserUID;
    mitUserUID = m_mapUserUID.find( iUserUID );
    if( mitUserUID == m_mapUserUID.end() )
    {
        START_LOG( cerr, L"������ �����Ϸ��µ� ������ ��ϵǾ� ���� ����." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    if( !mitUserUID->second )
    {
        START_LOG( cerr, L"������ �̻�." )
            << BUILD_LOG( iUserUID )
            << END_LOG;

        return false;
    }

    std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitUID;
    if( mitUserUID->second->m_nUnitUID > 0 )
    {
        mitUnitUID = m_mapUnitUID.find( mitUserUID->second->m_nUnitUID );
        if( mitUnitUID == m_mapUnitUID.end() )
        {
            START_LOG( cerr, L"���� UID �� �˻� ����." )
                << BUILD_LOG( mitUserUID->second->m_nUnitUID )
                << END_LOG;
        }
        else
        {
            if( !mitUnitUID->second )
            {
                START_LOG( cerr, L"������ �̻�." )
                    << BUILD_LOG( iUserUID )
                    << BUILD_LOG( mitUserUID->second->m_nUnitUID )
                    << END_LOG;
            }
            else
            {
                LIF( mitUnitUID->second->m_nUserUID == iUserUID );
            }
            m_mapUnitUID.erase( mitUnitUID );
        }

        if( mitUserUID->second->m_wstrUnitNickName.empty() )
        {
            START_LOG( cerr, L"���� UID�� ����� �Ǿ��ִµ� �г����� ��ϵ��� ����." )
                << BUILD_LOG( iUserUID )
                << BUILD_LOG( mitUserUID->second->m_nUnitUID )
                << END_LOG;
        }
    }
    else
    {
        if( !mitUserUID->second->m_wstrUnitNickName.empty() )
        {
            START_LOG( cerr, L"���� UID�� ����� �ȵǾ��ִµ� �г����� ����." )
                << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( mitUserUID->second->m_wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
    }

    std::map< std::wstring, boost::shared_ptr< KGSUserInfo > >::iterator mitUnitNickName;
    if( !mitUserUID->second->m_wstrUnitNickName.empty() )
    {
        mitUnitNickName = m_mapUnitNickName.find( mitUserUID->second->m_wstrUnitNickName );
        if( mitUnitNickName == m_mapUnitNickName.end() )
        {
            START_LOG( cerr, L"���� �г������� �˻� ����." )
#ifndef SERV_PRIVACY_AGREEMENT
                << BUILD_LOG( mitUserUID->second->m_wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                << END_LOG;
        }
        else
        {
            if( !mitUnitNickName->second )
            {
                START_LOG( cerr, L"������ �̻�." )
                    << BUILD_LOG( iUserUID )
#ifndef SERV_PRIVACY_AGREEMENT
                    << BUILD_LOG( mitUserUID->second->m_wstrUnitNickName )
#endif SERV_PRIVACY_AGREEMENT
                    << END_LOG;
            }
            else
            {
                LIF( mitUnitNickName->second->m_nUserUID == iUserUID );
            }
            m_mapUnitNickName.erase( mitUnitNickName );
        }
    }

	mitUserUID->second->m_nUnitUID = 0;
	mitUserUID->second->m_wstrUnitNickName.clear();
    mitUserUID->second->m_uiKNMSerialNum = 0;
	//{{ 2009. 4. 1  ������		ģ������
	mitUserUID->second->m_bDenyFriendShip = false;
	//}}

    return true;
}

//{{ 2009. 4. 1  ������		ģ�� ����
bool KUserList::SetDenyFriendShip( UidType iUnitUID, bool bDenyFriendShip )
{
	std::map< UidType, boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapUnitUID.find( iUnitUID );
	if( mit == m_mapUnitUID.end() )
	{
		return false;
	}

	if( !mit->second )
	{
		// �ð����� �߻�����.
		START_LOG( cwarn, L"������ �̻�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	( mit->second )->m_bDenyFriendShip = bDenyFriendShip;
	return true;
}
//}}

//{{ 2009. 11. 16  ������	 ü��ID����
void KUserList::DecreaseGuestConcurrentUser()
{
	--m_iGuestConcurrentUser;

	if( m_iGuestConcurrentUser < 0 )
	{
		m_iGuestConcurrentUser = 0;
	}
}
//}}

//{{ 2011. 09. 27	��μ�	PC �� ������ ����
#ifdef SERV_PCBANG_INCORRECT_BILLING
bool KUserList::GetGSUserInfoBySessionNo( const __int64& iSessionNo, KUserList::KGSUserInfo& kInfo )
{
	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >::iterator mit;

	mit = m_mapUserSessionNo.find( iSessionNo );
	if( mit == m_mapUserSessionNo.end() )
	{
		START_LOG( clog, L"���� SessionNO �Դϴ�." )
			<< BUILD_LOG( iSessionNo )
			<< END_LOG;
		return false;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"������ �̻�." )
			<< BUILD_LOG( iSessionNo )
			<< END_LOG;

		return false;
	}

	kInfo = *( mit->second );
	return true;
}
#endif SERV_PCBANG_INCORRECT_BILLING
//}}

//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
bool KUserList::RegAuthWaitUser( IN const KUserList::KGSUserInfo& kUserInfo )
{
	if( IsEnableAuthWait() == false )
		return true;

	std::map< UidType, KAuthWaitUser >::iterator mit;
	mit = m_mapAuthWaitUser.find( kUserInfo.m_nUserUID );
	if( mit != m_mapAuthWaitUser.end() )
	{
		START_LOG( cerr, L"�̹� ���� ����ڷ� ��ϵ� �����Դϴ�!" )
			<< BUILD_LOG( kUserInfo.m_nUserUID )
			<< BUILD_LOG( kUserInfo.m_wstrUserID )
			<< END_LOG;
		return false;
	}

	KAuthWaitUser kAuthWaitUser;
	kAuthWaitUser.m_iUserUID = kUserInfo.m_nUserUID;
	m_mapAuthWaitUser.insert( std::make_pair( kUserInfo.m_nUserUID, kAuthWaitUser ) );
	return true;
}

void KUserList::UnRegAuthWaitUser( IN const UidType iUserUID )
{
	m_mapAuthWaitUser.erase( iUserUID );
}

void KUserList::Tick()
{
	if( m_tCheckAuthWaitTimer.elapsed() < 1.0 )
		return;

	m_tCheckAuthWaitTimer.restart();

	std::vector< UidType > vecDeleteUserList;

	std::map< UidType, KAuthWaitUser >::const_iterator mit;
	for( mit = m_mapAuthWaitUser.begin(); mit != m_mapAuthWaitUser.end(); ++mit )
	{
#ifdef SERV_GLOBAL_AUTH
		if( mit->second.m_tTimer.elapsed() > 30.0 )	// �ؿܴ� ��� ������ 30�ʱ��� ��� �ð� �ø�
#else //SERV_GLOBAL_AUTH
		if( mit->second.m_tTimer.elapsed() > 10.0 )
#endif //SERV_GLOBAL_AUTH
		{
			vecDeleteUserList.push_back( mit->first );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iUserUID, vecDeleteUserList )
	{
		// �ϴ� ���� �����
		UnRegAuthWaitUser( iUserUID );

		KUserList::KGSUserInfo kInfo;
		if( GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
		{
			START_LOG( cerr, L"���� ���� ���� �˻� ����." )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
			continue;
		}

		START_LOG( cout, L"�ؽ� PC�� ���� ���� : ���� �ð� �������� ������ ���� ���� ó��!" )
			<< BUILD_LOG( kInfo.m_wstrUserID );

		KENX_AUTH_LOGIN_ACK kPacketAuth;
		kPacketAuth.m_bytePacketType;
		kPacketAuth.m_byteLoginType;
		kPacketAuth.m_wstrUserID = kInfo.m_wstrUserID;
		kPacketAuth.m_byteAuthorizeResult = 2; // Trial - �����̾� ���� ������ �����Ѵ�.
		
		//{{ 2010. 07. 01  ������	PC�� ���� ���� ����
		if( kInfo.m_bChannelChanged )
		{
			UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_CHANNEL_CHANGE_USER_ACK, kPacketAuth );
		}
		else
		{
			UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_USER_ACK, kPacketAuth );
		}
		//}}
	}
}
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-04-07
void KUserList::ReserveLocalRankingUserUnitInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	if( 100 < m_mapLocalRankingUserUnitInfo.size() )
	{
		m_mapLocalRankingUserUnitInfo.clear();
	}

	m_mapLocalRankingUserUnitInfo.insert( std::map< UidType, std::pair< KLocalRankingUserInfo, KLocalRankingUnitInfo > >::value_type( kUnitInfo.m_iUnitUID, std::make_pair( kUserInfo, kUnitInfo ) ) );
}

bool KUserList::GetLocalRankingUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo, OUT KLocalRankingUnitInfo& kUnitInfo ) const
{
	std::map< UidType, std::pair< KLocalRankingUserInfo, KLocalRankingUnitInfo > >::const_iterator it = m_mapLocalRankingUserUnitInfo.find( iUnitUID );
	if( it == m_mapLocalRankingUserUnitInfo.end() )
	{
		return false;
	}

	kUserInfo = it->second.first;
	kUnitInfo = it->second.second;
	return true;
}
#endif	// SERV_LOCAL_RANKING_SYSTEM
