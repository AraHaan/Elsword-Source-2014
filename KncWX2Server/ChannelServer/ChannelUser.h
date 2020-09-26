#pragma once
#include "Actor.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ActorManager.h"

#include <windows.h>    // ::GetTickCount()
#include <boost/timer.hpp>

//{{ 2009. 10. 14  ������	�ߺ���Ŷ����
#include "RepeatEventFilter/RepeatEventFilter.h"
//}}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	#include "UserHackingManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}
//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "UserSecurityManager.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

SmartPointer(KChannelUser);



/** 
@class : KChannelUser
@brief : ���� Ŭ���̾�Ʈ�� ��Ŷ�� �ְ�ް� ��Ŷ���� �ڵ鸵 �ϱ����� Ŭ����		 
@date : 2012/09/28
*/
class KChannelUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KChannelUser(void);
    virtual ~KChannelUser(void);

	enum ENUM_TIMER
	{
		TM_CHANNEL_LIST = 0,						// DB ������Ʈ �ֱ�		
		//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
		TM_HACKING_TOOL_LIST,					// ��ŷ�� ����Ʈ
		//}}
		//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
		TM_WAIT_QUEUE,
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

		TM_TOTAL_NUM,
	};

    // override
    virtual void Tick();

protected:  // util function, Packet Handling

	//{{ 2009. 10. 14  ������	����	
	void		InitRepeatEventFilter();
	bool		CheckReqEvent( unsigned short usEventID );
	void		CheckAckEvent( unsigned short usEventID );	
	//}}

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	void			CheckHackingToolList();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	void			CheckVerifyAccountRecv();
	void			SetUserID( IN std::wstring& wstrUserID );
	std::wstring&	GetUserID()										{ return m_wstrUserID; }
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	bool				SetMachineID( IN const std::string& strMachineID );
	std::string			GetMachineID();
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}
	//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	void			SendChannelList();
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

#ifdef SERV_GLOBAL_AUTH
	void RequestPublisherLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_);
	void HandlePublisherLoginAck(const KEPUBLISHER_AUTHENTICATION_ACK& kPacket_, OUT bool& bDoNotDisconnect);

	void RequestKOGOTPLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_);
	void HandleKOGOTPLoginAck();

	void RequestPublisherSecurityAuth(const KECH_SECURITY_AUTH_REQ& kPacket_);
	void HandlePublisherSecurityAuthAck(const KEPUBLISHER_SECURITY_AUTH_ACK& kPacket_);

	void HandlePublisherSecurityInfoNot(const KEPUBLISHER_SECURITY_INFO_NOT& kPacket_);

#ifdef SERV_COUNTRY_TH
	void				SetMasterID( IN const std::wstring& wstrMasterID ) { m_wstrMasterID = wstrMasterID; }
	void				SetSocketID( IN const std::wstring& wstrSocketID ) { m_wstrSocketID = wstrSocketID; }
	std::wstring		GetMasterID()	{ return m_wstrMasterID; }
	std::wstring		GetSocketID()	{ return m_wstrSocketID; }
#endif //SERV_COUNTRY_TH
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_LOGIN_RESULT_INFO
	void ProcessLoginResultCount( IN const int iResult );
#endif SERV_LOGIN_RESULT_INFO

    // packet send function
    template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
	bool SendID( unsigned short usEventID );

    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
    template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
	template < class T > void SendToFunizenAuthDB( unsigned short usEventID, const T& data );
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	void SendToLogDB( unsigned short usEventID );
	
    DECL_ON_FUNC( ECH_VERIFY_ACCOUNT_REQ );
#ifndef SERV_GLOBAL_AUTH 
    DECL_ON_FUNC( ECH_VERIFY_ACCOUNT_ACK );	// �ؿܿ��� ������
#endif // SERV_GLOBAL_AUTH
    //DECL_ON_FUNC( DBE_GASH_USER_LOGIN_ACK );// �ؿܿ��� ���� ������
	DECL_ON_FUNC_NOPARAM( ECH_GET_CHANNEL_LIST_REQ );
	DECL_ON_FUNC_NOPARAM( ECH_DISCONNECT_REQ );

	//{{ 2009. 12. 16  ������	������
	DECL_ON_FUNC( E_TOOL_GET_CCU_INFO_REQ );
	DECL_ON_FUNC( DBE_GET_CONCURRENT_USER_INFO_ACK );
	//}}

	DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

	//{{ 2011. 01. 13 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL
	DECL_ON_FUNC( E_TOOL_CHECK_LOGIN_REQ );
	DECL_ON_FUNC_NOPARAM( E_TOOL_SERVER_LIST_REQ );

	DECL_ON_FUNC( DBE_GET_CONCURRENT_USER_LOGIN_ACK );
	DECL_ON_FUNC( DBE_GET_GAME_SERVER_LIST_ACK );
#endif SERV_CCU_MONITORING_TOOL
	//}}	

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	DECL_ON_FUNC( ECH_HACKING_TOOL_LIST_ACK );
	DECL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_ACK );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

	//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	DECL_ON_FUNC( DBE_CHECK_SERVER_SN_ACK );
	DECL_ON_FUNC( DBE_CREATE_SERVER_SN_ACK );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}

#ifdef SERV_GLOBAL_AUTH
	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_ACK );

	DECL_ON_FUNC( DBE_CH_USER_GENERAL_LOGIN_ACK );
	DECL_ON_FUNC( DBE_CH_USER_KOGOTP_LOGIN_ACK );

	DECL_ON_FUNC( ECH_SECURITY_AUTH_REQ );
	DECL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_ACK );

	DECL_ON_FUNC( EPUBLISHER_SECURITY_INFO_NOT );

#endif // SERV_GLOBAL_AUTH

#ifdef SERV_ID_NETMARBLE_PCBANG
	DECL_ON_FUNC( ECH_PCBANG_IP_AND_MAC_INFO_NOT );
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERVER_GROUP_UI_ADVANCED
	DECL_ON_FUNC_NOPARAM( ECH_GET_SERVERGROUP_LIST_REQ );
#endif SERVER_GROUP_UI_ADVANCED

	//{{����ȯ �����ð� �޾ƿ���
#ifdef	SERV_SERVER_TIME_GET
	DECL_ON_FUNC_NOPARAM( ECH_GET_SERVER_TIME_REQ );
	//DECL_ON_FUNC( ELG_VERIFY_ACCOUNT_ACK );
#endif  SERV_SERVER_TIME_GET

    bool RoutePacket( const KEvent* pkEvent );   // CnUser, GSUser�� ���� ���������� ����� �ƴ�.
    //                          ^ KEvent�� �������� �ʾƾ� �Ѵ�. (SmartPtr�δ� �Ұ���)

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED ���·� ����������, DB data update�� ��û�Ѵ� 

private:
	boost::timer					m_kTimer[TM_TOTAL_NUM];

	//{{ 2009. 10. 14  ������	����
	KRepeatEventFilter				m_kRepeatEventFilter;
	//}}

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	KUserHackingManager				m_kUserHackingManager;
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	bool							m_bVerifyAccount;
	CTime							m_tVerifyAccount;
	std::wstring					m_wstrUserID;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

#ifdef SERV_GLOBAL_AUTH
	std::wstring					m_wstrID;

#ifdef SERV_PUBLISHER_MATRIXKEY
	unsigned int					m_uiMatrixKey;
#endif // SERV_PUBLISHER_MATRIXKEY

#ifdef SERV_COUNTRY_TH
	std::wstring					m_wstrMasterID;
	std::wstring					m_wstrSocketID;
#endif //SERV_COUNTRY_TH
#endif //SERV_GLOBAL_AUTH

	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	KUserSecurityManager			m_kUserSecurityManager;
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}
	//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	bool							m_bPassedEnterChannel;
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}
	//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	bool							m_bPassedDDOSGuardAuth;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}
};

template < typename T >
bool KChannelUser::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
{
	//{{ 2008. 8. 28  ������	�̺�Ʈ ����
	CheckAckEvent( usEventID );
	//}}

    return KSession::SendPacket( PI_GS_PROXY, GetUID(), NULL, usEventID, data, bLogging, bCompress );
}

template < class T >
void KChannelUser::SendToLogDB( unsigned short usEventID_, const T& data_ )
{
    UidType anTrace[2] = { GetUID(), -1 };

    KncSend( GetPfID(), GetUID(), PI_LOG_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KChannelUser::SendToAccountDB( unsigned short usEventID_, const T& data_ )
{
    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( GetPfID(), GetUID(), PI_ACCOUNT_DB, 0, anTrace, usEventID_, data_ );
}

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
template < class T >
void KChannelUser::SendToFunizenAuthDB( unsigned short usEventID_, const T& data_ )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( GetPfID(), GetUID(), PI_CHANNEL_AUTH_DB, 0, anTrace, usEventID_, data_ );
}
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
template < class T >
void KChannelUser::SendToLoginServer( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_CHANNEL_USER, GetUID(), PI_LOGIN_USER, 0, anTrace, usEventID, data );
}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY