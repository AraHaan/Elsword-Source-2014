#pragma once 

//////////////////////////////////////////////////////////////////////////
//081022.hoons.
//des : 1�� ������ ����, ��������Ʈ �� ���� ���������� üũ�Ͽ� ������.
//
//////////////////////////////////////////////////////////////////////////

#include "TRServer.h"
#include "BaseServer.h"
#include "Performer.h"
#include "FSM/support_FSM.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"

#include "FieldUserManager.h"

//{{ 2008. 12. 19  ������	�������Ϸ�
#include "Profiler/Profiler.h"
//}}

#define KFieldPtr KField*
#undef KFieldPtr
SmartPointer( KField );

#define _VERIFY_FIELD_STATE( varg, eventid, packet, neterror ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"���� ����. Name : " << m_strName ) \
	<< L"    ���� ���� : " << GetStateIDString() << dbg::endl \
	<< L"    ��� ���� : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToCharacter( LAST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_FIELD_STATE( varg, eventid, packet ) _VERIFY_FIELD_STATE( varg, eventid, packet, NetError::ERR_ROOM_09 );

//{{ 2007. 8. 20  ������  �Ϲ� ä�� ���� ���� ��ũ��
#define VERIFY_FIELD_STATE_CHAT_ONLY( varg, eventid, packet ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"���� ����. Name : " << m_strName ) \
	<< L"    ���� ���� : " << GetStateIDString() << dbg::endl \
	<< L"    ��� ���� : "L ## #varg << END_LOG; \
	kPacket.m_iOK = NetError::ERR_ROOM_09; \
	kPacket.m_cChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL; \
	SendToCharacter( FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \
	//}}


class KField : public KPerformer
{
	NiDeclareRTTI;
	DeclToStringW;
	DeclareFSM_MT;
	DeclPfID;

public:
	KField(void);
	virtual ~KField(void);

	// override.
	virtual	void	Tick();

	void			CloseField();

	void			SetFieldInfo( int iMapID, const KTileInfo& kTileInfo, const KMapInfo& kMapInfo );

	bool			IsEmpty() { return ( m_kFieldUserManager.GetNumMember() == 0 ); }

	//{{ 2009. 4. 20  ������	����üũ
	void			CheckZombieUser( UidType nCID );
	//}}
	
	// packet send function	
	template < class T > void SendToServer( unsigned short usEventID, const T& data );	
	void SendToServer( unsigned short usEventID );

	template < class T > void SendToCharacter( UidType nTo, unsigned short usEventID, const T& data );
	void SendToCharacter( UidType nTo, unsigned short usEventID );

	template < class T > void SendToTR( unsigned short usEventID, const T& data );

protected:
	virtual	void	ProcessEvent( const KEventPtr& spEvent ); // derived from KPerformer	

	////////////////////////////////////////////////////////////////////////// 
	// tcp relay function - SectorListType���� vector�� set�� ��������
	//template < class T >
	//void BroadCastGSUser( const std::vector< UidType >& vecUnitList, u_short usEventID, const T& data );

	//void _BroadCastGSUser( const std::vector< UidType >& vecUnitList, const KEvent& kEvent );

	//////////////////////////////////////////////////////////////////////////
	// event handler
	DECL_ON_FUNC_NOPARAM( ERM_OPEN_FIELD_NOT );

    DECL_ON_FUNC( ERM_JOIN_FIELD_REQ );
	//{{ 2009. 6. 4  ������		ä���̵�
   _DECL_ON_FUNC( ERM_JOIN_FIELD_CHANNEL_CHANGE_REQ, KERM_JOIN_FIELD_REQ );
	//}}

	DECL_ON_FUNC( ERM_LEAVE_FIELD_REQ );
   _DECL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT );	
	DECL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT );
	//{{ 2008. 12. 9  ������	ģ������Ʈ
	DECL_ON_FUNC( ERM_OPTION_UPDATE_NOT );
	DECL_ON_FUNC( ERM_UPDATE_PARTY_NOT );
	//}}
	//{{ 2008. 12. 13  ������	ä��
   _DECL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ );
	//}}
	//{{ 2008. 12. 13  ������	���ΰŷ�
	DECL_ON_FUNC( ERM_REQUEST_TRADE_NOT );
	DECL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT );
	//}}
	//{{ 2008. 12. 31  ������	����
	DECL_ON_FUNC_NOPARAM( DMP_DUMP_EVENT_NOT );
	//}}
	//{{ 2009. 1. 15  ������	���� ����Ʈ
   _DECL_ON_FUNC( ERM_FIELD_USER_LIST_REQ, KEGS_COMMUNITY_USER_LIST_REQ );
	//}}
	//{{ 2009. 4. 14  ������	ģ������Ʈ
	DECL_ON_FUNC( ERM_UPDATE_FRIEND_NOT );
	//}}
	//{{ 2009. 9. 23  ������	1������ĳ����
	DECL_ON_FUNC( ERM_CHANGE_FIELD_USER_INFO_NOT );
	//}}
	//{{ 2010. 7. 26  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
	DECL_ON_FUNC( ERM_SUMMON_PET_NOT );
	DECL_ON_FUNC( ERM_PET_ACTION_NOT );
	DECL_ON_FUNC( ERM_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	DECL_ON_FUNC( ERM_FIELD_USER_LIST_MONITORING_MODE_REQ );
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef SERV_INVISIBLE_GM
	DECL_ON_FUNC( ERM_TOGGLE_INVISIBLE_NOT );
#endif SERV_INVISIBLE_GM

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	DECL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT );
	DECL_ON_FUNC_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT );
	_DECL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT );
	DECL_ON_FUNC( ERM_CALL_MY_LOVER_LEAVE_FIELD_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	_DECL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT );
	_DECL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

protected:
	int					m_iMapID;

	KFieldUserManager	m_kFieldUserManager;
};

template < class T >
void KField::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_FIELD, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KField::SendToCharacter( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_FIELD, 0, PI_GS_CHARACTER, nTo, NULL, usEventID, data );
}

//template < class T >
//void KField::BroadCastGSUser( const std::vector< UidType >& vecUnitList, u_short usEventID, const T& data )
//{
//	KEvent kEvent;  // _BroadCast���� clone�� ���̹Ƿ� ���⼱ heap �ƴϾ ��.
//	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
//	kEvent.m_usEventID = usEventID;
//
//	KSerializer ks;
//	kEvent.m_kbuff.Clear();
//
//	// serialize - only data
//	ks.BeginWriting( &kEvent.m_kbuff );
//	ks.Put( data );
//	ks.EndWriting();
//
//	_BroadCastGSUser( vecUnitList, kEvent );
//}

template < class T >
void KField::SendToTR( unsigned short usEventID, const T& data_ )
{
	//���� ������ �̰��̰�
	//���� ������ TRUserManager::SendToRelay() ���� ó���� �����Ǿ����.
	KEventPtr spEvent( new KEvent() );

	spEvent->SetData( 0, NULL, usEventID, data_ );

	SiKTRServer()->QueueingRelayData( spEvent );
}

