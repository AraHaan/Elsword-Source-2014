#include "CnBaseRoom.h"
#include "CnRoomManager.h"

NiImplementRTTI( KCnBaseRoom, KPerformer );
ImplPfID( KCnBaseRoom, PI_CN_ROOM );

ImplToStringW( KCnBaseRoom )
{
	return START_TOSTRING_PARENTW( KPerformer );
}

KCnBaseRoom::KCnBaseRoom()
{
}

KCnBaseRoom::~KCnBaseRoom()
{
}

void KCnBaseRoom::Tick()
{
	KPerformer::Tick();
}

void KCnBaseRoom::SetUDPRelayInfo( const KNetAddress& kAddress )
{
	m_wstrUDPRelayIP = kAddress.m_wstrIP;
	m_usUDPRelayPort = kAddress.m_usPort;
}

void KCnBaseRoom::CloseRoom()
{	
	//081020.hoons.
	LIF( GetKCnRoomManager()->CloseRoom( GetUID() ) );
	return;
}

void KCnBaseRoom::SendToGSServer( UidType nTo, unsigned short usEventID )
{
	SendToGSServer( nTo, usEventID, char() );
}

void KCnBaseRoom::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSCharacter( nGS, nTo, usEventID, char() );
}

//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
void KCnBaseRoom::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}

void KCnBaseRoom::SendToLogDB( unsigned short usEventID )
{
    SendToLogDB( usEventID, char() );
}

void KCnBaseRoom::BroadCastAllGSServer( unsigned short usEventID )
{
    BroadCastAllGSServer( usEventID, char() );
}

//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
void KCnBaseRoom::SendRoomCountInfo( CXSLRoom::ROOM_TYPE eType, bool bOpen )
{
	if( eType < 0  ||  eType >= CXSLRoom::RT_TOTAL_NUM )
	{
		START_LOG( cerr, L"RoomType�� �̻��մϴ�.!" )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return;
	}

	KERM_ROOM_COUNT_INFO_NOT kNot;
	kNot.m_cRoomType = static_cast<char>( eType );
	kNot.m_bIsOpen = bOpen;

	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_ROOM, 0, PI_CN_PROXY, 0, anTrace, ERM_ROOM_COUNT_INFO_NOT, kNot );
}
#endif SERV_ROOM_COUNT
//}}


