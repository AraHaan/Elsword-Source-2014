
#include "ChannelUser.h"

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_CN
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


void KChannelUser::RequestPublisherLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_)
{
	START_LOG( clog, L"GIANT ���� ��û." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrID )
		<< BUILD_LOG( kPacket_.m_wstrPassword )
		<< BUILD_LOG( kPacket_.m_bLogin )
		<< END_LOG;

	// �α��� ������ ���� (������ ������ �α��μ����� ä���� ������ ����) //
	
	KEPUBLISHER_AUTHENTICATION_REQ kPacketReq;
	kPacketReq.m_iActorUID = GetUID();
	kPacketReq.m_wstrIP = KncUtil::toWideString( GetIPStr() );;
	kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
	kPacketReq.m_wstrServicePassword = kPacket_.m_wstrPassword;
#ifdef SERV_KOG_OTP_VERIFY
	kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_LOGIN_MAC_ADDRESS
	kPacketReq.m_wstrMACAddress = kPacket_.m_wstrMACAddress;
#endif //SERV_LOGIN_MAC_ADDRESS
#ifdef SERV_PUBLISHER_OTP
	kPacketReq.m_wstrPublisherOTP = kPacket_.m_wstrPublisherOTP;
#endif // SERV_PUBLISHER_OTP

	SendToLoginServer( ELG_VERIFY_ACCOUNT_REQ, kPacketReq );	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
}

void KChannelUser::HandlePublisherLoginAck(const KEPUBLISHER_AUTHENTICATION_ACK& kPacket_, OUT bool& bDoNotDisconnect)
{

	if( kPacket_.m_iOK == NetError::ERR_GIANT_VERIFY_01 )
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
	// CN�� ��� ���⼭ ���� �ϴ°� ���� 
}

void KChannelUser::RequestPublisherSecurityAuth(const KECH_SECURITY_AUTH_REQ& kPacket_)
{
	// �α��� ������ ���� (������ ������ �α��μ����� ä���� ������ ����) //

	KEPUBLISHER_SECURITY_AUTH_REQ kPacketReq;
	kPacketReq.m_iSecurityAuthType = kPacket_.m_iSecurityAuthType;
	kPacketReq.m_iUserUID = GetUID();
	kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());

#ifdef SERV_PUBLISHER_MATRIXKEY
	kPacketReq.m_wstrUserID = m_wstrID;
	kPacketReq.m_uiMatrixKey = m_uiMatrixKey;
	kPacketReq.m_wstrMatrix = kPacket_.m_wstrMatrix;
#endif // SERV_PUBLISHER_MATRIXKEY
	
	SendToLoginServer( ELG_SECURITY_AUTH_REQ, kPacketReq );
}

void KChannelUser::HandlePublisherSecurityAuthAck(const KEPUBLISHER_SECURITY_AUTH_ACK& kPacket_)
{
	// TW�� ��� ���⼭ ���� �ϴ°� ���� 
}

void KChannelUser::HandlePublisherSecurityInfoNot(const KEPUBLISHER_SECURITY_INFO_NOT& kPacket_)
{
	if (kPacket_.m_iSecurityInfoType == KEPUBLISHER_SECURITY_INFO_NOT::SIT_MATRIX_CARD )
	{
		m_uiMatrixKey = kPacket_.m_uiMatrixKey;

		KECH_SECURITY_INFO_NOT kNot;
		kNot.m_iSecurityInfoType = kPacket_.m_iSecurityInfoType;
		kNot.m_uiMatrixKey = kPacket_.m_uiMatrixKey;
		kNot.m_wstrCoordinate = kPacket_.m_wstrCoordinate;

		SendPacket( ECH_SECURITY_INFO_NOT, kNot );
	}
}

#endif // SERV_COUNTRY_CN
#endif // SERV_GLOBAL_AUTH
