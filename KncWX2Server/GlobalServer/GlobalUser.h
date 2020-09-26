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


SmartPointer(KGlobalUser);

class KGlobalUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KGlobalUser(void);
    virtual ~KGlobalUser(void);

    // override
    virtual void Tick();

protected:  // util function, Packet Handling	
	
	bool AuthCheckPassword( const wchar_t* szPassword, UINT32 uiPwdHash );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-09	// �ڼ���
	int		BossFieldOpenProcess( IN const int iVillageMapID, IN const __time64_t tCurrentTime ) const;
	int		TotalDangerousValueUpdateProcess( IN const int iBattleFieldID, IN const byte byteValue, IN const __time64_t tCurrentTime ) const;
#endif // SERV_BATTLE_FIELD_BOSS

    // packet send function
    template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );

	//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template < class T > void SendToGameDB( unsigned short usEventID_, const T& data_ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
    
    template < class T > void SendToGameServer( unsigned short usEventID, const T& data );

	//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template < class T > void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	template < class T > void SendToGSUser( UidType nToGSUser, unsigned short usEventID, const T& data );
	template < class T > void SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data );
//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	//}}

	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
   _DECL_ON_FUNC( EGB_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}

	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	_DECL_ON_FUNC( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT );
#endif SERV_MORNITORING
	//}}
	DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	DECL_ON_FUNC( EGB_GET_MISSION_INFO_REQ );
	_DECL_ON_FUNC( EGB_CLEAR_MISSION_DUNGEON_NOT, KEGB_CLEAR_MISSION_DUNGEON_NOT );
	_DECL_ON_FUNC( EGB_UPDATE_CCU_INFO_NOT, KELG_UPDATE_CCU_INFO_NOT );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	_DECL_ON_FUNC( EGB_TODAY_RANDOM_QUEST_REQ, UidType );
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	DECL_ON_FUNC( EGB_REGISTER_SERVERSN_REQ );
	DECL_ON_FUNC( EGB_UNREGISTER_SERVERSN_NOT );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	//}}
	//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EGB_REGISTER_UNIT_NOT );
	DECL_ON_FUNC( EGB_UNIT_DISCONNECT_NOT );

   _DECL_ON_FUNC( EGB_INVITE_PARTY_FIND_RECEIVER_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );
   _DECL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
   _DECL_ON_FUNC( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   //{{ 2012. 11. 21	�ڼ���	���� ���� ������ ������ ����Ʈ
#ifdef SERV_HERO_PVP_MANAGE_LIST
   DECL_ON_FUNC_NOPARAM( EGB_ADMIN_HERO_PVP_USER_LIST_REQ );
   _DECL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_ADD_REQ, KEGS_ADMIN_HERO_PVP_USER_ADD_REQ );
   _DECL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_REMOVE_REQ, KEGS_ADMIN_HERO_PVP_USER_REMOVE_REQ );
   _DECL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_REQ, KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ );
   DECL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT );
#endif SERV_HERO_PVP_MANAGE_LIST
   //}}

	//{{ 2013. 05. 29	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	DECL_ON_FUNC( EGB_CHECK_BLOCK_USER_NOT );
   _DECL_ON_FUNC( EGB_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT );
#endif SERV_BLOCK_LIST
	//}}
   //{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
   DECL_ON_FUNC( EGB_GET_GLOBAL_MISSION_INFO_REQ );
   _DECL_ON_FUNC( EGB_GLOBAL_MISSION_UPDATE_NOT, KEGB_GLOBAL_MISSION_UPDATE_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER
   //}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
   
#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
   DECL_ON_FUNC_NOPARAM( EGB_TIME_ENCHANT_EVENT_INFO_REQ );
   DECL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_NOT );
   DECL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
   DECL_ON_FUNC_NOPARAM( EGB_EXCHANGE_LIMIT_INFO_REQ );
   DECL_ON_FUNC( EGB_EXCHANGE_LIMIT_REQ );
   DECL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK );
   DECL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
   DECL_ON_FUNC( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT );
   DECL_ON_FUNC_NOPARAM( EGB_BATTLE_FIELD_BOSS_INFO_NOT );

   DECL_ON_FUNC( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ );
   DECL_ON_FUNC_NOPARAM( EGB_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );
   DECL_ON_FUNC( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ );
   DECL_ON_FUNC( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ );
#endif // SERV_BATTLE_FIELD_BOSS
	    
    bool RoutePacket( const KEvent* pkEvent );   // CnUser, GSUser�� ���� ���������� ����� �ƴ�.
    //                          ^ KEvent�� �������� �ʾƾ� �Ѵ�. (SmartPtr�δ� �Ұ���)

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED ���·� ����������, DB data update�� ��û�Ѵ� 

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
private:
	bool			m_bConnectGameServer;

#endif SERV_RANDOM_DAY_QUEST
	//}}
};

template < typename T >
bool KGlobalUser::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
{
    return KSession::SendPacket( PI_GLOBAL_PROXY, GetUID(), NULL, usEventID, data, bLogging, bCompress );
}

//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
template < class T >
void KGlobalUser::SendToGameDB( unsigned short usEventID_, const T& data_ )
{
	UidType anTrace[2] = { -1, -1 };

	if( anTrace[0] == -1 )
		anTrace[0] = GetUID();
	else
		anTrace[1] = GetUID();

	KncSend( GetPfID(), GetUID(), PI_GLOBAL_GAME_DB, 0, anTrace, usEventID_, data_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

template < class T >
void KGlobalUser::SendToLogDB( unsigned short usEventID_, const T& data_ )
{
    UidType anTrace[2] = { -1, -1 };

    if( anTrace[0] == -1 )
        anTrace[0] = GetUID();
    else
        anTrace[1] = GetUID();

    KncSend( PI_GLOBAL_USER, GetUID(), PI_LOG_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KGlobalUser::SendToGameServer( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KEvent kEvent;
	kEvent.SetData( PI_GS_SERVER, anTrace, usEventID, data );
	KSession::SendPacket( kEvent );
}

//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
template < class T >
void KGlobalUser::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"Ÿ ���Ӽ����� ��Ŷ ������ ����.? �ֿ�" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToUnitUID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//�ٸ� ���Ӽ����� �ڷḦ ������ ����
template < class T >
void KGlobalUser::SendToGSUser( UidType nToGSUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );
	KSession::SendPacket( kEvent );
}

template < class T >
void KGlobalUser::SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"Ÿ ���Ӽ����� ��Ŷ ������ ����.? �ֿ�" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToGSUser )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}



