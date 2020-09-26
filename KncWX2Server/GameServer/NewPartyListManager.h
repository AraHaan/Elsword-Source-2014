// 2008.12.22. hoons. ��Ƽ �Ŵ���
// 1. �⺻ ��Ƽ ���
// 2. ��Ƽ ����Ʈ ���
// # ���� ����� ��� �����ؾ� �Ѵ�.
#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "GSUser.h"
#include "GSParty.h"


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////


class KPartyListManager
{
	DeclareSingleton( KPartyListManager );
	DeclDump;
	DeclToStringW;

	// ����Ʈ enum�� Ű�� �߰��Ҷ��� �׻�
	// KEGS_PARTY_LIST_REQ �� enum�� ���� �߰����־����.
	enum PARTY_LIST_TYPE
	{
		PLT_DUNGEON = 0,
		PLT_GATE_TOTAL,
		PLT_SECRET_DUNGEON,

		PLT_END,
	};

	enum PARTY_LIST_ENUM
	{
		PLE_PARTY_PAGE = 12,
	};

public:
    KPartyListManager();
    ~KPartyListManager();

	KGSPartyPtr	GetParty( IN UidType iPartyUID );
	
	bool	GetDungeonInfo( IN UidType iPartyUID, OUT int& iDungeonID, OUT char& cDifficultyLevel, OUT char& cDungeonMode );
	int		GetCurrentPartyCount()	{ return static_cast<int>(m_mapPartyInfo.size()); }
	int		GetPlayGamePartyCount() { return m_iPlayGamePartyCount; }
	bool	GetPartyUserList( IN UidType iPartyUID, OUT std::vector< KCommunityUserInfo >& vecPartyUserList );
	bool	IsPartyMember( IN UidType iPartyUID, IN UidType iUnitUID );
	bool	GetPartyNumMember( IN UidType iPartyUID, OUT int& iNumMember );
	bool	GetPartyFeverExpRate( IN UidType iPartyUID, OUT float& fAddPartyFeverExpRate );
	bool	GetPartyFever( IN const UidType iPartyUID, OUT short& sPartyFever );

	bool	CheckInvitePartyConditionForHost( IN KGSUserPtr spHost );
	bool	CheckInvitePartyConditionForReceiver( IN KGSUserPtr spReceiver );													// �ʴ� ����ڰ� ��Ƽ�ʴ밡 �ɼ� �ִ��� �˻��ϴ� �Լ�
	

	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
	
	bool	E_CreatePartyInSameField(			IN KGSUserPtr spHost,	 IN KGSUserPtr spReceiver );							// ���� �ʵ忡�� �ʴ�� �Բ� ��Ƽ ����
	bool	E_InvitePartyInSameField(			IN KGSUserPtr spHost,	 IN KGSUserPtr spReceiver );							// ���� �ʵ忡�� �ڽ��� ��Ƽ�� �ʴ�

	bool	E_CreatePartyFarFieldCheckHost(		IN KGSUserPtr spHost,	 IN const std::wstring& wstrReceiverNickName );			// �ٸ� �ʵ忡�� �ʴ�� �Բ� ��Ƽ ���� �� ��Ƽ �ʴ��� üũ
	bool	E_InvitePartyFarFieldCheckHost(		IN KGSUserPtr spHost,	 IN const std::wstring& wstrReceiverNickName );			// �ٸ� �ʵ忡�� �ڽ��� ��Ƽ�� �ʴ� �� ��Ƽ �ʴ��� üũ
	bool	E_InvitePartyFarFieldCheckReceiver(	IN KGSUserPtr spReceiver, IN const KELG_INVITE_PARTY_FIND_RECEIVER_REQ& kReq );	// �ٸ� �ʵ忡�� ��Ƽ �ʴ� ����� üũ

	bool	E_InvitePartyReply(		IN KGSUserPtr spUser, IN const KEGS_INVITE_PARTY_REPLY_NOT& kNot );							// ��Ƽ �ʴ뿡 ���� ����

	bool	E_LeavePartyReq(		IN KGSUserPtr spUser, IN const KEGS_LEAVE_PARTY_REQ& kReq );

	bool	E_UpdatePartyUserInfo( 	IN KGSUserPtr spUser, IN bool bChangeField );

	bool	E_ChangeReadyReq(		IN KGSUserPtr spUser, IN bool bReady );
	bool	E_ChangeDungeon(		IN KGSUserPtr spUser, IN const KEGS_PARTY_CHANGE_DUNGEON_REQ& kReq );

	bool	E_GameStart(			IN KGSUserPtr spHost, IN const KEGS_QUICK_START_DUNGEON_GAME_REQ& kReq );
	bool	E_EndGame(				IN KGSUserPtr spHost );
		
	bool	E_ChangeHost(			IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_HOST_REQ& kReq );
	bool	E_ChangeNumOfPer(		IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_NUM_OF_PER_REQ& kReq );
	bool	E_ChangeGetItemtype(	IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ& kReq );

	bool	E_BanUser(				IN KGSUserPtr spHost, IN const KEGS_PARTY_BAN_USER_REQ& kReq );
	bool	E_Chat(					IN KGSUserPtr spUser, IN const KEGS_CHAT_REQ& kReq );
	
	bool	E_IncreasePartyFever(	IN const KERM_INCREASE_PARTY_FEVER_NOT& kNot );	
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	E_MaxPartyFever(		IN const KERM_MAX_PARTY_FEVER_NOT& kNot );	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	bool	E_DungeonSecretStageEnterCheck( IN KGSUserPtr spUser );	

	void	E_OpenPartyList(		IN const KPartyInfo& kPartyInfo, IN const std::vector< KPartyUserInfo >& vecPartyUser );
	void	E_ClosePartyList(		IN const UidType iPartyUID );
	bool	E_AddPartyList(			IN const UidType iPartyUID );
	bool	E_DelPartyList(			IN const UidType iPartyUID );
	void	E_UpdatePartyInfo(		IN const KPartyInfo& kPartyInfo );
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	E_UpdatePartyFever(		IN const UidType iPartyUID, IN const short sPartyFever );
	void	E_EndGamePartyRegroup(	IN const UidType iPartyUID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	void	E_AddPartyMemberInPartyList( IN UidType iPartyUID, IN const KPartyUserInfo& kInfo );
	void	E_DelPartyMemberInPartyList( IN UidType iPartyUID, IN UidType iUnitUID );
	void	E_UpdatePartyMemberInfo( IN UidType iPartyUID, IN const std::vector< KPartyUserInfo >& vecPartyUserInfo );

	int		DelPartyInfo(			IN UidType iCnServerUID );

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool	IsPossibleLowLevel(		IN KGSUserPtr spUser, IN KGSPartyPtr spParty );
	bool	IsPossibleLowLevel(		IN KGSUserPtr spUser, IN UidType iPartyUID, OUT bool isPartyNULL );
	bool	E_ChangeCheckMinLevel(  IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ& kReq );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2011. 06. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	bool	E_ChangePartyType(		IN KGSUserPtr spHost, IN const KEGS_CHANGE_PARTY_TYPE_REQ& kReq	);
	bool	E_ChangePvpMatchInfo(	IN KGSUserPtr spHost, IN const KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ& kReq );
#endif SERV_PVP_NEW_SYSTEM
	//}}

protected:
	bool	AddNormalDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID );
	bool	DelNormalDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID );

	//{{ 2010. 03. 26  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	bool	AddSecretDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID );
	bool	DelSecretDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID );
#endif SERV_SECRET_HELL
	//}}
	
protected:	
	template < class T > void SendToCnParty( UidType iPartyUID, unsigned short usEventID, const T& data );
	template < class T > void SendToCnParty( UidType iPartyUID, unsigned short usEventID );

protected:
	std::map< UidType, KGSPartyPtr >				m_mapPartyInfo;
	std::map< int, std::vector< KGSPartyPtr > >		m_mapPartyList[PLT_END];
	// PLT_DUNGEON			[key:DungeonID, value:PartyList]
	// PLT_GATE_TOTAL		[key:MapID, value:PartyList]
	// PLT_SECRET_DUNGEON	[key:DungeonID, value:PartyList]

	//{{ 2008. 12. 30  ������	���� �������� ��Ƽ ����
	int							m_iPlayGamePartyCount;
	//}}
};

DefSingletonInline( KPartyListManager );


template < class T >
void KPartyListManager::SendToCnParty( UidType iPartyUID, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PARTY, KBaseServer::GetKObj()->GetUID(), PI_CN_PARTY, iPartyUID, NULL, usEventID, data );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


