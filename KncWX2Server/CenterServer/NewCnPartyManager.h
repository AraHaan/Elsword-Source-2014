// 2008.12.22. hoons. ��Ƽ �Ŵ���
// 1. �⺻ ��Ƽ ���
// 2. ��Ƽ ����Ʈ ���
// # ���� ����� ��� �����ؾ� �Ѵ�.
#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "NewCnParty.h"


//{{ 2011. 12. 05	������	��Ʋ�ʵ�
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////



class KCnPartyManager : public KPerformer
{
	DeclareSingleton( KCnPartyManager );
	DeclDump;
	DeclToStringW;
	DeclPfID;

public:
    KCnPartyManager();
    ~KCnPartyManager();
	
	void	Init();
    void	Tick();

protected:
	KCnPartyPtr GetParty( IN UidType iPartyUID );

    int		GetMaxParty()							{ return m_ciMaxParty; }
	int		GetCurrentPartyCount()					{ return ( GetMaxParty() - m_setSpareUID.size() ); }
	//{{ 2010. 01. 27  ������	�� ����
#ifdef SERV_ROOM_COUNT
	void	SendPartyCountInfo( bool bOpen );
#endif SERV_ROOM_COUNT
	//}}

public:
	void	QueueingEventToParty( const KEventPtr& spEvent );
	void	QueueingToAll( const KEventPtr& spEvent );
	void	CloseParty( IN KCnPartyPtr spParty, bool bClosePartyList = true );

	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// ����� ��Ƽ�� ������ �Ҹ꿡 ���õ� �̺�Ʈ�� �־����..
	// �׿ܿ� �̺�Ʈ�� �ش� ��Ƽ���� ���� ó���Ѵ�.
	DECL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ );
	DECL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
	DECL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT );
   _DECL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType );

protected:
	// SendTo
	template < class T > void SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data );

	// BroadCast
	template < class T > void BroadCastGameServer( IN unsigned short usEventID, IN const T& data );

protected:	
	KCnPartyPtr OpenParty();

protected:	
    const int							m_ciMaxParty;

    std::set< UidType >					m_setSpareUID;
    std::map< UidType, KCnPartyPtr >	m_mapParty;
};

DefSingletonInline( KCnPartyManager );


template < class T >
void KCnPartyManager::SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template < class T >
void KCnPartyManager::BroadCastGameServer( IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}



//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


