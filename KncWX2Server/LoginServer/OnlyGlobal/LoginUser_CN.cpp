#include "LoginUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"

#include "DBLayer.h"
#include "NetError.h"
#include "LoginSimLayer.h"
#include "LoginServer.h"
//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	#include "LoginRoomManager.h"
#endif SERV_PSHOP_AGENCY
//}}
#include "Nexon/AuthCheck.h"
#include <boost/format.hpp>
//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
	#include "NexonAuthManager.h"
#else
	#include "NexonBillingAuthManager.h"
#endif SERV_PCBANG_AUTH_NEW
//}}
//{{ 2008. 7. 14  ������  ��ŷ
#include "RankingManager.h"
//}}
//{{ 2009. 9. 23  ������	���
#include "GuildManager.h"
//}}
//{{ 2010. 01. 13  ������	���Խ���
#include "GuildBoardManager.h"
//}}
//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
	#include "ServerCCUManager.h"
#endif SERV_CCU_NEW
//}}
//{{ 2010. 06. 22  ������	���������
#ifdef SERV_ACCOUNT_BLOCK
	#include "AccountBlockManager.h"
#endif SERV_ACCOUNT_BLOCK
//}}
//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	#include "DLLManager.h"
#endif SERV_DLL_MANAGER
//}}
//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	#include "RandomItemEventManager.h"
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}
//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#include "GameSysVal/GameSysVal.h"
//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#ifdef SERV_COUNTRY_CN



#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantAuthManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantRoleRegManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantInfoManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantBillingManager.h"


void KLoginUser::RequestPublisherLogin(const KEPUBLISHER_AUTHENTICATION_REQ& kPacket_)
{
	
	KEPUBLISHER_AUTHENTICATION_REQ kPacketReq = kPacket_;

	// LoginServer���� �߰� ���� �߰� 
	kPacketReq.m_uiRequestID = static_cast<unsigned int>(GetUID());
	kPacketReq.m_usZone = SiKGiantAuthManager()->GetServerInfo().m_usZone;
	kPacketReq.m_usGame = SiKGiantAuthManager()->GetServerInfo().m_usGame;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_NULL, anTrace, EPUBLISHER_AUTHENTICATION_REQ, kPacketReq );
	SiKGiantAuthManager()->QueueingEvent( spEvent );
	
}

void KLoginUser::HandlePublisherLoginAck(const KEPUBLISHER_AUTHENTICATION_ACK& kPacket_, OUT bool& bDoNotDisconnect)
{


}

void KLoginUser::RequestKOGOTPLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_)
{

}

void KLoginUser::HandleKOGOTPLoginAck()
{
	// TW�� ��� ���⼭ ���� �ϴ°� ���� 
}

void KLoginUser::RequestPublisherSecurityAuth(const KEPUBLISHER_SECURITY_AUTH_REQ& kPacket_)
{
	
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEPUBLISHER_SECURITY_AUTH_REQ kPacket = kPacket_;
	kPacket.m_uiRequestID = static_cast<unsigned int>(GetUID());
	kPacket.m_usZone = SiKGiantAuthManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantAuthManager()->GetServerInfo().m_usGame;
	
	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_NULL, anTrace, EPUBLISHER_SECURITY_AUTH_REQ, kPacket );
	SiKGiantAuthManager()->QueueingEvent( spEvent );
	
}

void KLoginUser::HandlePublisherSecurityAuthAck(const KEPUBLISHER_SECURITY_AUTH_ACK& kPacket_)
{
	// TW�� ��� ���⼭ ���� �ϴ°� ���� 
}


#endif // SERV_COUNTRY_CN
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY