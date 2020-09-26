#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <list>
#include "Enum/Enum.h"
#include "AutoParty.h"
#include "AutoPartyUserWaitList.h"
#include "AutoPartyMakingProcess.h"

//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KAutoPartyMakingManager : public KPerformer, public KThread
{
	DeclareSingleton( KAutoPartyMakingManager );
	DeclDump;
	DeclToStringW;
	DeclPfID;	

public:
    KAutoPartyMakingManager();
    ~KAutoPartyMakingManager();
	
	virtual void Run();     // derived from KThread
	virtual void ShutDown();

	bool	Init();
	void	ClearAllAutoPartyInfo();
    void	Tick();

	// for lua
	void	Command_LUA( char cType );

	// function
	int		GetAutoPartyMakingEstimatedTime() const;
	void	RefreshTimerAutoPartyGameStart( IN const int iDungeonMode, IN const int iDungeonIDWithDif );

protected:
	// AutoPartyMakingProcess
	KAutoPartyMakingProcess* GetAutoPartyMakingProcess( IN const int iDungeonMode, IN const int iDungeonIDWithDif );
	KAutoPartyMakingProcess* GetAutoPartyMakingProcess( IN const UidType iAutoPartyWaitNumber );

	// AutoParty
	KAutoPartyPtr	GetAutoParty( IN const UidType iAutoPartyUID );
	bool			AddAutoParty( IN KAutoPartyPtr spAutoParty );
	void			DestroyAutoParty( IN const UidType iAutoPartyUID );

public:
	void	QueueingToAll( IN const KEventPtr& spEvent );

	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// ����� ��Ƽ�� ������ �Ҹ꿡 ���õ� �̺�Ʈ�� �־����..
	// �׿ܿ� �̺�Ʈ�� �ش� ��Ƽ���� ���� ó���Ѵ�.
	DECL_ON_FUNC_NOPARAM( EGB_GET_AUTO_PARTY_BONUS_INFO_REQ );
	DECL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_MAKING_REQ );
	DECL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_REQ );
	DECL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_AUTO_PARTY_NOT );
	DECL_ON_FUNC( EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT );
   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
	//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	DECL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	//}}

protected:
	template < class T > void SendToGSCharacter( IN const UidType iGSUID, IN const UidType iUnitUID, IN const u_short usEventID, IN const T& data );
	template < class T > void SendToGameServer( IN const UidType iGSUID, IN const u_short usEventID, IN const T& data );
	template < class T > void BroadCastGameServer( IN const u_short usEventID, IN const T& data );

protected:
	// �ڵ� ��Ƽ ����Ʈ
	std::map< UidType, KAutoPartyPtr >						m_mapAutoParty;
	std::vector< KAutoPartyPtr >							m_vecAutoParty;

	// �ڵ� ��Ƽ ����ŷ ���μ���
	typedef std::pair< int, int >	KMakingProcessKey;
	std::map< KMakingProcessKey, KAutoPartyMakingProcess >	m_mapAutoPartyMakingProcess;	// key:DungeonIDWithDif, value:AutoPartyMakingProcess
};

DefSingletonInline( KAutoPartyMakingManager );


template < class T >
void KAutoPartyMakingManager::SendToGSCharacter( IN const UidType iGSUID, IN const UidType iUnitUID, IN const u_short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template < class T >
void KAutoPartyMakingManager::SendToGameServer( IN const UidType iGSUID, IN const u_short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGSUID, anTrace, usEventID, data );
}

template < class T >
void KAutoPartyMakingManager::BroadCastGameServer( IN const u_short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


