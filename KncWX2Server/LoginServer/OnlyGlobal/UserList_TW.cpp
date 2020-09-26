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

#ifdef SERV_COUNTRY_TWHK

#include "../Common/OnlyGlobal/AuthAndBilling/TW/GASHAuthManager.h"


void KUserList::RequestRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo, std::wstring wstrSockID )
{
	// �ش� �Լ��� RegUser ���� ȣ���
	// Reguser�� ELG_REGISTER_USER_REQ (ä�μ���), ELG_CHANNEL_CHANGE_REGISTER_USER_REQ(ä���̵�)
	// ���� �ٿ��� ȣ���

	KEPUBLISHER_REG_USER_REQ kPacketReq;
	kPacketReq.m_iActorUID = iUserUID;
	kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
	kPacketReq.m_wstrServicePassword = L"";
	in_addr sIP;
	sIP.s_addr = ( u_long )kInfo.m_uiIP;
	kPacketReq.m_strIP = inet_ntoa( sIP );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_NULL, NULL, EPUBLISHER_REG_USER_REQ, kPacketReq );
	SiKGASHAuthManager()->QueueingEvent( spEvent );
}

void KUserList::RequestUnRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo )
{
	KEPUBLISHER_UNREG_USER_REQ kPacketReq;
	kPacketReq.m_iActorUID = iUserUID;
	kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
	kPacketReq.m_wstrServicePassword = L"";
	in_addr sIP;
	sIP.s_addr = ( u_long )kInfo.m_uiIP;
	kPacketReq.m_strIP = inet_ntoa( sIP );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, EPUBLISHER_UNREG_USER_REQ, kPacketReq );
	SiKGASHAuthManager()->QueueingEvent( spEvent );
}

#endif // SERV_COUNTRY_TWHK