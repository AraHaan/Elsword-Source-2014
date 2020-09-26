
#include "ChannelUser.h"

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_ID
#include <dbg/dbg.hpp>
//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
#include "CHFSM.h"
#else
#include "defaultFSM.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

#include "DBLayer.h"
#include "NetError.h"
#include "ChannelSimLayer.h"
#include "ChannelServer.h"

//{{ 2009. 12. 16  ������	������
#include "CCUToolManager.h"
//}}

//{{ 2011. 01. 20 ��μ�	���� ����͸� ��               ���Ŀ��������
#ifdef SERV_CCU_MONITORING_TOOL
#include "odbc/Odbc.h"
#endif SERV_CCU_MONITORING_TOOL
//}}

#include <boost/format.hpp>

//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
#include "MachineBlockManager.h"
#include "HackingProtectionManager.h"
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
#include "ServerSNMaker.h"
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}
//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
#include "DDOSGuardSimulator.h"
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}

#pragma comment(lib, "../Common/OnlyGlobal/AuthAndBilling/ID/NetmarbleSignChecker.lib")
#include "../Common/OnlyGlobal/AuthAndBilling/ID/NetmarbleSignChecker.h"




void KChannelUser::RequestPublisherLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_)
{
	START_LOG( clog, L"ID ���� ��û." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrID )
		<< BUILD_LOG( kPacket_.m_wstrPassword )
		<< BUILD_LOG( kPacket_.m_bLogin )
		<< END_LOG;

	KECH_VERIFY_ACCOUNT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	char szServerString[1024];
	ZeroMemory( szServerString, 1024 );
	memcpy( szServerString, kPacket_.m_strUserToken.c_str(), kPacket_.m_strUserToken.size() );

	CNetmarbleSignChecker m;
	m.parseParameter( szServerString, "cath3f6pH7tR4vAcemaPa7u4uB4c4ph9nadusUrutrezeta9uSte4EspAv3crabe" );

	bool isSignIn = m.isSignIn();
	std::string strCN;
	if( isSignIn == true )
		strCN = m.getValue( "userToken", "cn" );

	if( isSignIn == false || strCN.empty() == true )
	{
		START_LOG( clog, L"���� ����" )
			<< BUILD_LOG( szServerString )
			<< BUILD_LOG( m.getErrorCode() )
			<< BUILD_LOG( m.getErrorMessage() );

		switch( m.getErrorCode() )
		{
		default:
			kPacket.m_iOK = NetError::ERR_VERIFY_20;
			break;
		}

		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	// ���� �α��� ����
	KDBE_CH_USER_GENERAL_LOGIN_REQ kPacketReq;
	kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
	kPacketReq.m_uiPublisherUID = atoi( strCN.substr( 2 ).c_str() );
	kPacketReq.m_wstrIP = KncUtil::toWideString( GetIPStr(), strlen( GetIPStr() ) );
#ifdef SERV_PURCHASE_TOKEN
	kPacketReq.m_wstrPurchaseTok = L"";;
#endif SERV_PURCHASE_TOKEN
	kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();
#ifdef SERV_COUNTRY_TH
	kPacketReq.m_wstrMasterID = L"";
	kPacketReq.m_wstrSocketID = L"";
#endif // SERV_COUNTRY_TH

	SendToAccountDB( DBE_CH_USER_GENERAL_LOGIN_REQ, kPacketReq );
}

void KChannelUser::HandlePublisherLoginAck(const KEPUBLISHER_AUTHENTICATION_ACK& kPacket_, OUT bool& bDoNotDisconnect)
{
	// �������� ���� ���� ���ϴ� NetError �� �ٸ� �� ����
	if( kPacket_.m_iOK == NetError::ERR_GAMEFORGE_00 ||
		kPacket_.m_iOK == NetError::ERR_GAMEFORGE_01 ||
		kPacket_.m_iOK == NetError::ERR_GAMEFORGE_02 )
	{
		bDoNotDisconnect = true;
	}
	else
	{
		bDoNotDisconnect = false;
	}
	
}

void KChannelUser::RequestKOGOTPLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_)
{
	// �̹� X2StateLogin���� ���̾�Ʈ ������ �������Ƿ� KOG OTP �������� ��ü
	KDBE_CH_USER_KOGOTP_LOGIN_REQ kPacketReq;
	kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
	kPacketReq.m_wstrOTP = kPacket_.m_wstrPassword;
	kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());
#ifdef SERV_PURCHASE_TOKEN
	kPacketReq.m_wstrPurchaseTok = L"";
#endif SERV_PURCHASE_TOKEN
#if defined( SERV_STEAM ) || defined( SERV_ALL_RENEWAL_SP )
	kPacketReq.m_iChannelingCode = kPacket_.m_iChannelingCode;
#endif //( SERV_STEAM ) || ( SERV_ALL_RENEWAL_SP )

	SendToAccountDB( DBE_CH_USER_KOGOTP_LOGIN_REQ, kPacketReq );
}

void KChannelUser::HandleKOGOTPLoginAck()
{
	// �̱��� ��� ���⼭ ���� �ϴ°� ���� 
}

void KChannelUser::RequestPublisherSecurityAuth(const KECH_SECURITY_AUTH_REQ& kPacket_)
{

	
}

void KChannelUser::HandlePublisherSecurityAuthAck(const KEPUBLISHER_SECURITY_AUTH_ACK& kPacket_)
{
	// ������ ��� ���⼭ ���� �ϴ°� ���� 
}

void KChannelUser::HandlePublisherSecurityInfoNot(const KEPUBLISHER_SECURITY_INFO_NOT& kPacket_)
{

}

#endif // SERV_COUNTRY_ID
#endif // SERV_GLOBAL_AUTH
