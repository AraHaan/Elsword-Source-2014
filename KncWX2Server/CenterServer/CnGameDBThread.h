#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  ������	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "Enum/Enum.h"

//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
	#include "Room/RoomManager.h"
#endif SERV_TRADE_DDOS_DEFENCE
//}}

class KCnGameDBThread : public KSubThread
{
    DeclareDBThread( KCnGameDBThread );
	DeclareSPProfiler;
    DeclPfID;
public:

    KCnGameDBThread();   // �����δ� ����. ������� �ʴ´�. (���� linking error ����)
    virtual ~KCnGameDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );
	
	bool Query_InsertItemList( IN const SEnum::GET_ITEM_REASON eGetItemReason, 
							   IN const UidType iUnitUID, 
							   IN const std::vector< KItemInfo >& vecItemInfo, 
							   OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, 
							   IN const bool bUnSeal = true );

	bool Query_UpdateItemQuantity( IN const UidType iUnitUID, 
								   IN const std::map< UidType, int >& mapUpdated, 
								   OUT std::map< UidType, int >& mapFailed, 
								   OUT bool& bUpdateFailed );

	void Query_DeleteItem( IN const std::vector< KDeletedItemInfo >& vecDeleted, OUT std::vector< KDeletedItemInfo >& vecFailed );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToRoom( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

	//{{ 2012. 05. 08	��μ�       ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	DECL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ );
#else
//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
	DECL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ );
#endif SERV_TRADE_DDOS_DEFENCE
//}}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
	//}}
	//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	DECL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	DECL_ON_FUNC( DBE_WEDDING_COMPLETE_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
};

template < class T >
void KCnGameDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KCnGameDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_CN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KCnGameDBThread::SendToRoom( UidType nTo, unsigned short usEventID, const T& data )
{
	// KncSend�δ� ���� �ʴ´�. ���� KEvent���� ����!
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_ROOM, NULL, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( nTo ) );
	KRoomManager::GetKObj()->QueueingEventToRoom( spEvent );
}



