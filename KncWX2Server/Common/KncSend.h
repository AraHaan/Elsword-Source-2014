#pragma once

#include "Event.h"

// Performer ����
#define PC_PROXY				0x00000001
#define PC_USER					0x00000002
#define PC_CHARACTER			0x00000003
#define PC_SERVER				0x00000004
#define PC_ROOM					0x00000005
#define PC_ACCOUNT_DB			0x00000006
#define PC_GAME_DB				0x00000007
#define PC_LOG_DB				0x00000008
#define PC_SMS_DB				0x00000009
//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#define PC_NX_WEB_DB			0x0000000A
//}}
#define PC_NX_AUTH				0x0000000B
#define PC_PARTY				0x0000000C
#define PC_NX_BILLING			0x0000000D
#define PC_NX_BILLING_TCP		0x0000000E
#define PC_MATCH_MAKING			0x0000000F
//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
//#ifdef SERV_NEXON_AUTH_SOAP
#define PC_NX_SOAP				0x00000010
//#endif SERV_NEXON_AUTH_SOAP
//}}
//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
#define PC_AUTO_PARTY			0x00000011
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
// 2013-11-21. ���Ա��� ����
#define PC_GAME_DB_2ND          0x00000012
#define PC_LOG_DB_2ND           0x00000013

//{{ 2013. 03. 13	������	�ؽ� ���� ��� �̵����
//#ifdef SERV_NEXON_OPERATIONAL_TOOLS_MIDDLEWARE
#define PC_NX_GMTOOL_TCP		0x00000014
//#endif SERV_NEXON_OPERATIONAL_TOOLS_MIDDLEWARE
//}}

//#ifdef SERV_RECORD_CHAT
#define PC_CHAT_LOG_DB			0x00000015
//#endif SERV_RECORD_CHAT

//#ifdef SERV_GLOBAL_BILLING
//-------------------------------------------------------------//
//-------------------- �ؿ� ���� ���� ���� --------------------//
#define PC_KOG_BILLING_MANAGER	0x00000051	// ��ü ���� �Ŵ���
#define PC_KOG_BILLING_DB		0x00000052	// ��ü ���� DB

#define PC_PUBLISHER_AUTH		0x00000053	// �Ϲ����� �ۺ��� ���� �ý���
#define PC_PUBLISHER_BILLING	0x00000054	// �Ϲ����� �ۺ��� ���� �ý��� (TCP ��� ��� - Soap ����)
#define PC_PUBLISHER_BILLING_DB	0x00000055	// �Ϲ����� �ۺ��� ���� �ý��� (DB SP ���)
#define PC_PUBLISHER_AUTH_DB	0x00000056	// �Ϲ����� �ۺ��� ���� �ý��� (DB SP ���)

//---------------- Ư�� ���̽� �ؿܰ� ���� �з� ---------------//
//#ifdef SERV_AUTH_TCP_THREAD_MANAGER
#define PC_AS_OTP_THREAD		0x00000071	// AsiaSoft OTP
//#endif SERV_AUTH_TCP_THREAD_MANAGER

// �߱��� Ư���� ������ �ƿ� ���� �з�
#define PC_GIANT_AUTH			0x00000083	// GIANT ���� ��� �Ŵ���
#define PC_GIANT_ROLEREG		0x00000084	// GIANT ���ϼ� ���� ��� �Ŵ���
#define PC_GIANT_INFO			0x00000085	// GIANT GMTool ��� �Ŵ���
#define PC_GIANT_BILLING		0x00000086	// GIANT ���� ��� �Ŵ���
#define PC_GIANT_COUPON			0x00000087	// GIANT ���� ��� �Ŵ���
//-------------------------------------------------------------//
//#endif SERV_GLOBAL_BILLING

//#ifdef SERV_ID_NETMARBLE_PCBANG
#define PC_ID_PCBANG_AUTH_DB	0x00000090	// 2013.07.01 lygan_������ // �ε��׽þ� PC�� �����ϱ� ���� �ۺ��� DB�� ���� ��
//#endif //SERV_ID_NETMARBLE_PCBANG

//#ifdef SERV_ADD_EVENT_DB
#define PC_EVENT_DB				0x00000091	// 2013.10.30 darkstarbt_������ // Event DB �߰�
//#endif //SERV_ADD_EVENT_DB

//#ifdef SERV_ADD_SCRIPT_DB
#define PC_SCRIPT_DB					0x00000092	// 2014.02.24 orange82_������ // Script DB �߰�
//#endif //SERV_ADD_SCRIPT_DB


//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
//#ifdef SERV_RELAY_DB_CONNECTION
#define PC_JP_RELAY_DB			0x000000A0	// �Ϻ� �̺�Ʈ �߰�DB
//#endif SERV_RELAY_DB_CONNECTION
//}}

#define PC_MASK_BIT				0x000000FF

// Server ����
#define SC_CLIENT				0x00000100
#define SC_GAME					0x00000200
#define SC_CENTER				0x00000300
#define SC_CHANNEL				0x00000400	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#define SC_LOGIN				0x00000500	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#define SC_SIMULATION			0x00000600
#define SC_GLOBAL				0x00000700
#define SC_MASK_BIT				0X00000F00

//{{ 2010. 05. 10  ������	�������� ������ ����
// Send Ÿ��
#define ST_SAME_SERV_GROUP		0x00001000
#define ST_MASK_BIT				0x0000F000
//}}

// ������ Performer Class ���� ID �� �����Ѵ�.
enum ePerformerID {
    // Client
    PI_GS_PROXY			= SC_CLIENT | PC_PROXY,

    // Game Server
    PI_GS_USER			= SC_GAME | PC_USER,
    PI_GS_CHARACTER		= SC_GAME | PC_CHARACTER,
    PI_GS_SERVER		= SC_GAME | PC_SERVER,						// ��� �������� GameServer���� Event������ ���
	PI_GS_SERVER_SAME	= SC_GAME | PC_SERVER | ST_SAME_SERV_GROUP,	// ���� �������� GameServer���Ը� Event������ ���
    PI_GS_GAME_DB		= SC_GAME | PC_GAME_DB,
    PI_GS_ACCOUNT_DB	= SC_GAME | PC_ACCOUNT_DB,
    PI_GS_LOG_DB		= SC_GAME | PC_LOG_DB,
	PI_GS_SMS_DB		= SC_GAME | PC_SMS_DB,
    PI_GS_NX_WEB_DB		= SC_GAME | PC_NX_WEB_DB,					// �ؽ� �� DB
    PI_CN_PROXY			= SC_GAME | PC_PROXY,
	PI_LOGIN_PROXY		= SC_GAME | PC_PROXY,
	PI_GLOBAL_PROXY		= SC_GAME | PC_PROXY,
    PI_GS_NX_BILLING	= SC_GAME | PC_NX_BILLING,
    PI_GS_NX_BILLING_TCP= SC_GAME | PC_NX_BILLING_TCP,				// �ؽ� ����
	PI_GS_FIELD			= SC_GAME | PC_ROOM,						// ���� [Field]
	PI_GS_PARTY			= SC_GAME | PC_PARTY,
	//{{ 2012. 04. 02	��μ�		�ؽ� auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	PI_GS_NX_SOAP		= SC_GAME | PC_NX_SOAP,						// �ؽ� SOAP
#endif SERV_NEXON_AUTH_SOAP
	//}}
#ifdef SERV_ENTRY_POINT
    PI_GS_GAME_DB_2ND   = SC_GAME | PC_GAME_DB_2ND,  // ���Ա��� ����, �ٸ� �������� ���ӵ�� ���ٿ�
    PI_GS_LOG_DB_2ND    = SC_GAME | PC_LOG_DB_2ND,  
#endif SERV_ENTRY_POINT

//#ifdef SERV_RECORD_CHAT
	PI_GS_CHAT_LOG_DB	= SC_GAME | PC_CHAT_LOG_DB,
//#endif SERV_RECORD_CHAT

//#ifdef SERV_GLOBAL_BILLING // 2013.04.11 ��ȿ�� �ؿ� ���� ���� �۾�
	PI_GS_KOG_BILLING_MANAGER		= SC_GAME | PC_KOG_BILLING_MANAGER,
	PI_GS_KOG_BILLING_DB			= SC_GAME | PC_KOG_BILLING_DB,

	PI_GS_PUBLISHER_AUTH			= SC_GAME | PC_PUBLISHER_AUTH,

	PI_GS_PUBLISHER_BILLING			= SC_GAME | PC_PUBLISHER_BILLING,	// TCP ����� �� ���
	PI_GS_PUBLISHER_BILLING_DB		= SC_GAME | PC_PUBLISHER_BILLING_DB,// DB ����� �� ���
//#endif SERV_GLOBAL_BILLING

//#ifdef SERV_ID_NETMARBLE_PCBANG
	PI_GS_ID_PUBLISHER_PCBANG_DB	= SC_GAME | PC_ID_PCBANG_AUTH_DB,// �ε��׽þ� PC �� ���� ��� ������ ���
//#endif //SERV_ID_NETMARBLE_PCBANG

//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
//#ifdef SERV_RELAY_DB_CONNECTION
	PI_GS_JP_RELAY_DB				= SC_GAME | PC_JP_RELAY_DB,	// �Ϻ� �̺�Ʈ �߰�DB
//#endif SERV_RELAY_DB_CONNECTION
//}}

//#ifdef SERV_ADD_EVENT_DB
	PI_GS_EVENT_DB					= SC_GAME | PC_EVENT_DB,
//#endif //SERV_ADD_EVENT_DB

//#ifdef SERV_ADD_SCRIPT_DB
	PI_GS_SCRIPT_DB				= SC_GAME | PC_SCRIPT_DB,
//#endif //SERV_ADD_SCRIPT_DB

	
    // Center Server
    PI_CN_USER			= SC_CENTER | PC_USER,
    PI_CN_SERVER		= SC_CENTER | PC_SERVER,
    PI_CN_ROOM			= SC_CENTER | PC_ROOM,
	PI_CN_PARTY			= SC_CENTER | PC_PARTY,						// ��Ƽ
    PI_CN_ACCOUNT_DB	= SC_CENTER | PC_ACCOUNT_DB,
	//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
//#ifdef SERV_TRADE_DDOS_DEFENCE
	PI_CN_GAME_DB		= SC_CENTER | PC_GAME_DB,
//#endif SERV_TRADE_DDOS_DEFENCE
	//}}
    PI_CN_LOG_DB		= SC_CENTER | PC_LOG_DB,
	PI_CN_SMS_DB		= SC_CENTER | PC_SMS_DB,

//#ifdef SERV_ADD_EVENT_DB
	PI_CN_EVENT_DB		= SC_CENTER | PC_EVENT_DB,
//#endif //SERV_ADD_EVENT_DB

	// Login Server
	PI_LOGIN_USER		= SC_LOGIN | PC_USER,
	PI_LOGIN_SERVER		= SC_LOGIN | PC_SERVER,
	PI_LOGIN_ROOM		= SC_LOGIN | PC_ROOM,
	PI_LOGIN_ACCOUNT_DB	= SC_LOGIN | PC_ACCOUNT_DB,
	PI_LOGIN_GAME_DB	= SC_LOGIN | PC_GAME_DB,
	PI_LOGIN_LOG_DB		= SC_LOGIN | PC_LOG_DB,	
	PI_LOGIN_SMS_DB		= SC_LOGIN | PC_SMS_DB,
	PI_LOGIN_NX_AUTH	= SC_LOGIN | PC_NX_AUTH,	
	PI_LOGIN_NX_WEB_DB	= SC_LOGIN | PC_NX_WEB_DB,

//#ifdef SERV_ADD_EVENT_DB
	PI_LOGIN_EVENT_DB	= SC_LOGIN | PC_EVENT_DB,
//#endif //SERV_ADD_EVENT_DB
	
//#ifdef SERV_GLOBAL_BILLING // 2013.04.11 ��ȿ�� �ؿ� ���� ���� �۾�
// �α��� �������� ���� ó���� �� ���
	PI_LOGIN_KOG_BILLING_MANAGER	= SC_LOGIN | PC_KOG_BILLING_MANAGER,
	PI_LOGIN_KOG_BILLING_DB			= SC_LOGIN | PC_KOG_BILLING_DB,
	
	PI_LOGIN_PUBLISHER_AUTH			= SC_LOGIN | PC_PUBLISHER_AUTH,
	PI_LOGIN_PUBLISHER_BILLING		= SC_LOGIN | PC_PUBLISHER_BILLING,	// TCP ����� �� ���
	PI_LOGIN_PUBLISHER_BILLING_DB	= SC_LOGIN | PC_PUBLISHER_BILLING_DB,// DB ����� �� ���

	// �߱�
	PI_LOGIN_GIANT_AUTH			= SC_LOGIN | PC_GIANT_AUTH,
	PI_LOGIN_GIANT_ROLEREG		= SC_LOGIN | PC_GIANT_ROLEREG,
	PI_LOGIN_GIANT_INFO			= SC_LOGIN | PC_GIANT_INFO,
	PI_LOGIN_GIANT_BILLING		= SC_LOGIN | PC_GIANT_BILLING,
	PI_LOGIN_GIANT_COUPON		= SC_LOGIN | PC_GIANT_COUPON,
//#endif SERV_GLOBAL_BILLING

//{{ ������ : [2012/10/9] //	�±� OTP ���� ��� ���
//#ifdef SERV_AUTH_TCP_THREAD_MANAGER
	PI_LOGIN_AS_OTP_THREAD		= SC_LOGIN | PC_AS_OTP_THREAD,
//#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}

//#ifdef SERV_COUNTRY_PH
	PI_LOGIN_GARENA_BILLING		= SC_LOGIN | PC_PUBLISHER_BILLING,
//#endif //SERV_COUNTRY_PH
	
	// Channel Server
	PI_CHANNEL_USER		= SC_CHANNEL | PC_USER,
	PI_CHANNEL_SERVER	= SC_CHANNEL | PC_SERVER,
	PI_CHANNEL_LOG_DB	= SC_CHANNEL | PC_LOG_DB,
	PI_CHANNEL_ACCOUNT_DB	= SC_CHANNEL | PC_ACCOUNT_DB,
	PI_CHANNEL_LOGIN_PROXY	= SC_CHANNEL | PC_PROXY,	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
//#ifdef SERV_CHECK_DROP_CCU
	PI_CHANNEL_SMS_DB		= SC_CHANNEL | PC_SMS_DB,
//#endif SERV_CHECK_DROP_CCU
	//}}
    // 2013.11.08 ���� ���Ա��� ���� ( ĳ���� ���� �� ���� ���� )
    PI_CHANNEL_GAME_DB = SC_CHANNEL | PC_GAME_DB, 
//#ifdef SERV_ADD_EVENT_DB
	PI_CHANNEL_EVENT_DB	= SC_CHANNEL | PC_EVENT_DB,
//#endif //SERV_ADD_EVENT_DB

//#ifdef SERV_GLOBAL_AUTH
	PI_CHANNEL_AUTH_DB	= SC_CHANNEL | PC_PUBLISHER_AUTH_DB,
//#endif SERV_GLOBAL_AUTH

	// Global Server
	PI_GLOBAL_USER			= SC_GLOBAL | PC_USER,
	PI_GLOBAL_SERVER		= SC_GLOBAL | PC_SERVER,
	PI_GLOBAL_MATCH_MAKING	= SC_GLOBAL | PC_MATCH_MAKING,
	//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
	PI_GLOBAL_AUTO_PARTY	= SC_GLOBAL | PC_AUTO_PARTY,
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
	PI_GLOBAL_GAME_DB		= SC_GLOBAL | PC_GAME_DB,
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	PI_GLOBAL_LOG_DB		= SC_GLOBAL | PC_LOG_DB,
	PI_GLOBAL_SMS_DB		= SC_GLOBAL | PC_SMS_DB,
	//{{ 2013. 03. 13	������	�ؽ� ���� ��� �̵����
	//#ifdef SERV_NEXON_OPERATIONAL_TOOLS_MIDDLEWARE
	PI_GLOBAL_ACCOUNT_DB	= SC_GLOBAL | PC_ACCOUNT_DB,
	PI_GLOBAL_NX_GMTOOL_TCP	= SC_GLOBAL | PC_NX_GMTOOL_TCP,
	//#endif SERV_NEXON_OPERATIONAL_TOOLS_MIDDLEWARE
	//}}
	
//#ifdef SERV_ADD_EVENT_DB
	PI_GLOBAL_EVENT_DB		= SC_GLOBAL | PC_EVENT_DB,
//#endif //SERV_ADD_EVENT_DB

    // Common DB
    PI_ACCOUNT_DB		= PC_ACCOUNT_DB,
    PI_LOG_DB			= PC_LOG_DB,
//#ifdef SERV_ADD_EVENT_DB
	PI_EVENT_DB			= PC_EVENT_DB,
//#endif //SERV_ADD_EVENT_DB
//#ifdef SERV_ADD_SCRIPT_DB
	PI_SCRIPT_DB		= PC_SCRIPT_DB,
//#endif //SERV_ADD_SCRIPT_DB
    PI_NULL				= -1,
};

int CompareServerLevel( DWORD dwDestPI, DWORD dwCurrentPI );
DWORD GetPerformerClass( DWORD dwPI );
DWORD GetServerClass( DWORD dwPI );
//{{ 2010. 05. 10  ������	�������� ������ ����
DWORD GetSendType( DWORD dwSt );
//}}

void KncSend( DWORD dwPIFrom, UidType nFrom, KEventPtr spEvent );

template <class T>
void KncSend( DWORD dwPIFrom_, UidType nFrom_, DWORD dwPITo_, UidType nTo_, UidType anTrace_[], u_short usEventID_, const T& data_ )
{
    KEventPtr spEvent( new KEvent );
    spEvent->SetData( dwPITo_, anTrace_, usEventID_, data_ );
    LIF( spEvent->m_kDestPerformer.AddUID( nTo_ ) );

    KncSend( dwPIFrom_, nFrom_, spEvent );
}
