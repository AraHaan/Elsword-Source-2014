// 2008. 12. 16. jseop. ��Ƽ �ý���.
//
// GSUser ��ü�� ���� �ϹǷ� ���� �����忡�� �� Ŭ������ Tick�� �����ϵ��� �Ѵ�.
//
// ��Ƽ�� ����� ��ȸ���� �ĺ� ȸ������ �̷���� �ִ�.
// �ĺ� ȸ���� ���� ��Ƽ���� �ƴϰ� �������� �����̹Ƿ� ��Ƽ ���� ���μ�������
// �����Ǿ�� �Ѵ�.
// ��ȸ���� ������ ����.
//  1. ��Ƽ�� ���� ��Ƽ���� �� ȸ��.
//  2. ��Ƽ���� �ʴ븦 �����Ͽ� ��Ƽ�� ������ ȸ��.
//  3. ��Ƽ�忡�� ���� ��û�� �Ͽ� ��Ƽ�����κ��� ���ε� ȸ��.
// ��Ƽ���� �ƴ� ��ȸ���� �������� ��Ƽ�� �ʴ��� ��쿡�� ����������
// �ʴ���� ������ ��Ƽ�忡�� ���� ��û�� �ϵ��� ó���Ѵ�.
// �� �� ��쿡 �ʴ���� ������ ��ȸ���� �Ƿ��� ���� 3���� ������ ��ģ��.
// �ĺ� ȸ���� ������ ����.
//  1. ��Ƽ���� �ʴ������� ��Ƽ ������ ���� �������� ���� ȸ��.
//  2. ��Ƽ�忡�� ���� ��û�� ������ ���� ��Ƽ���� �������� ���� ȸ��.
// ���� 1���� ��� ��Ƽ���� �ʴ븦 öȸ�ϰų� �ʴ���� ȸ���� �����ϰų�
// 10�ʰ� ������ �ش� �ĺ� ȸ���� ��Ƽ�� Ż���Ѵ�.
// ���� 2���� ��� �ĺ� ȸ���� ���� ��û�� öȸ�ϰų� ��Ƽ���� �����ϰų�
// 10�ʰ� ������ �ش� �ĺ� ȸ���� ��Ƽ�� Ż���Ѵ�.
//
// ��ȸ���� �ĺ� ȸ���� ���ؼ� ��Ƽ�� ������ ���� �ʾƾ� �Ѵ�.
//
// ��Ƽ�� ���´� �÷��� ������ �ƴ��� �����Ѵ�.

#pragma once

#include <KncSmartPtr.h>
#include "CnPartyUser.h"
#include "../Common/X2Data/XSLDungeon.h"
#include "Performer.h"
#include "FSM/support_FSM.h"
#include "CnPartyUserManager.h"
#include "CnPartyGameManager.h"
//{{ 2012. 08. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "CnPartyFeverManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


#define KCnPartyPtr KCnParty*
#undef KCnPartyPtr
SmartPointer( KCnParty );

//////////////////////////////////////////////////////////////////////////
// party state ack err
#define _VERIFY_PARTY_STATE_ACK( varg, eventid, packet, neterror ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"���� ����. Name : " << m_strName ) \
	<< L"    ���� ���� : " << GetStateIDString() << dbg::endl \
	<< L"    ��� ���� : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_PARTY_STATE_ACK( varg, eventid, packet ) _VERIFY_PARTY_STATE_ACK( varg, eventid, packet, NetError::ERR_WRONG_STATE_00 );

//////////////////////////////////////////////////////////////////////////
// party state ack warn
#define _VERIFY_PARTY_STATE_ACK_WARN( varg, eventid, packet, neterror ) \
	packet kPacket; \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cwarn, L"���� ����. Name : " << m_strName ) \
	<< L"    ���� ���� : " << GetStateIDString() << dbg::endl \
	<< L"    ��� ���� : "L ## #varg << END_LOG; \
	kPacket.m_iOK = neterror; \
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
	return; \
} \
	START_LOG_WITH_NAME( clog ); \

#define VERIFY_PARTY_STATE_ACK_WARN( varg, eventid, packet ) _VERIFY_PARTY_STATE_ACK_WARN( varg, eventid, packet, NetError::ERR_WRONG_STATE_00 );

//////////////////////////////////////////////////////////////////////////
// party state err
#define VERIFY_PARTY_STATE( varg ) \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cerr, L"���� ����. Name : " << m_strName ) \
	<< L"    ���� ���� : " << GetStateIDString() << dbg::endl \
	<< L"    ��� ���� : "L ## #varg << END_LOG; \
	return; \
} \

//////////////////////////////////////////////////////////////////////////
// party state warn
#define VERIFY_PARTY_STATE_WARN( varg ) \
	if( !m_pkCurrentState->CheckState varg ) \
{ \
	START_LOG( cwarn, L"���� ����. Name : " << m_strName ) \
	<< L"    ���� ���� : " << GetStateIDString() << dbg::endl \
	<< L"    ��� ���� : "L ## #varg << END_LOG; \
	return; \
} \


class KCnParty : public KPerformer
{
	DeclareFSM_MT;
	DeclPfID;

private:
	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	enum PARTY_ENUM
	{
		PE_LIMIT_PARTY_MEBMER = 4,
		PE_CLOSE_PARTY_NUM_MEMBER = 1,
	};
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	//{{ 2012. 05. 17	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	enum TIMER_ENUM
	{
		TE_FIRST_INVITE = 0,
		TE_CHECK_PLAY_CONDITION,	// �÷��� ���� üũ�� ���� �ð����� Ÿ�̸�
		TE_CHECK_AUTO_PARTY,		// �ڵ���Ƽ ���� üũ�� ���� �ð����� Ÿ�̸�
		TE_ACCEPT_FOR_PLAY,			// �÷��� ���� üũ�� ���� �ð����� Ÿ�̸�
		TE_PREPARE_FOR_PLAY,		// �÷��� ���� ������ ���� �ð����� Ÿ�̸�

		TE_MAX,
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}


public:
	KCnParty( UidType iUID );
    virtual ~KCnParty();

	void	Tick();
		
	void	SetInfo( IN const KPartyInfo& kInfo );
	//{{ 2012. 01. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetInfo( IN const KCreatePartyInfo& kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	void	GetInfo( OUT KPartyInfo& kInfo );
	void	GetInfo( OUT std::vector< KPartyInfo >& vecInfo );
	void	Reset();

	UidType GetUID() const												{ return m_iUID; }
	void	SetName( IN const std::wstring& wstrName )					{ m_wstrName = wstrName; }
	const std::wstring& GetName() const									{ return m_wstrName; }

	//{{ 2011. 06. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	char	GetPartyType() const										{ return m_cPartyType; }
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	GetDungeonGameInfo( OUT KDungeonGameInfo& kInfo ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	int		GetDungeonID() const										{ return m_iDungeonID; }
	char	GetDungeonDif() const										{ return m_cDifficultyLevel; }
	int		GetDungeonIDAndDif() const									{ return m_iDungeonID + static_cast<int>( m_cDifficultyLevel ); }
	char	GetItemType() const											{ return m_cGetItemType; }
	void	SetDungeonID( int iDungeonID )								{ m_iDungeonID = iDungeonID; }
	void	SetDungeonDif( char cDif )									{ m_cDifficultyLevel = cDif; }
	void	SetPublic( bool bPublic )									{ m_bPublic = bPublic; }	
	void	SetItemType( char cGetItemType )							{ m_cGetItemType = cGetItemType; }
	void	SetPlaying( bool bPlay )									{ m_bIsPlaying = bPlay; }

	CXSLDungeon::DUNGEON_MODE GetDungeonMode() const					{ return m_eDungeonMode; }
	void	SetDungeonMode( const CXSLDungeon::DUNGEON_MODE eDungeonMode );
    
	bool	IsPlaying() const											{ return m_bIsPlaying; } // ���� �����濡 ������ �ִٴ� �ǹ���. [�߰�] �Լ� �̸��� �ٲٴ°� ���?
	bool	IsPublic() const											{ return m_bPublic; }	

	// ��Ÿ�� �ʱ�ȭ
	void	AllPartyMembersQuickSlotCoolTimeReset();

	// fever
	void	UpdateAndBroadCastFeverPoint( IN const int iFeverPoint );
	void	IncreaseFeverPoint( IN const char cRoomType, IN const short sSubStageNpcCount );
	void	DecreaseFeverPoint( IN const int iDecreaseFeverPoint );
	short	GetFeverPoint() const										{ return m_kFeverManager.GetFeverPoint( m_kPartyUserManager.GetNumMember() ); } 
		
	void	AddPartyInfoInPartyList();													// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ���� �߰�
	//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	void	DelPartyInfoInPartyList( IN const bool bForce = false );					// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ���� ����
	void	UpdatePartyInfoInPartyList( IN const bool bForce = false );					// ���Ӽ����� ��Ƽ ����Ʈ�� ���� ��Ƽ ���� ������Ʈ	
//#else
//	void	DelPartyInfoInPartyList();													// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ���� ����
//	void	UpdatePartyInfoInPartyList();												// ���Ӽ����� ��Ƽ ����Ʈ�� ���� ��Ƽ ���� ������Ʈ
//#endif SERV_PVP_NEW_SYSTEM
	//}}		

	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	UpdatePartyFeverInPartyList( IN const bool bForce = false );					// ���Ӽ����� ��Ƽ ����Ʈ�� ���� ��Ƽ �ǹ� ������Ʈ	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	void	AddPartyMemberInPartyList( IN const KPartyUserInfo& kInfo );				// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ��� �߰�
	//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	void	DelPartyMemberInPartyList( IN const UidType iUnitUID, IN const bool bForce = false );	// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ��� ����
//#else
//	void	DelPartyMemberInPartyList( IN UidType iUnitUID );							// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ��� ����
//#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2012. 03. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, IN const KPartyUserJoinRoomInfo& kPartyUserJoinRoomInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	void	SetCheckLowLevel( IN bool bCheck )			{ m_bCheckLowLevel = bCheck; }
	bool	GetCheckLowLevel()							{ return m_bCheckLowLevel; }
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2012. 05. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KOldPartyInfo MakeOldPartyInfo() const;

	// �÷��� ���� üũ
	void	CheckAllMembersPlayCondition();
	void	FailProcessPlayCondition( IN const int iNetError, IN const UidType iFailUnitUID = 0, IN const std::wstring wstrFailUserNickName = L"" );

	// �ڵ� ��Ƽ ���� üũ
	void	CheckAllMembersAutoParty();
	void	FailProcessAutoParty( IN const int iNetError, IN const std::wstring wstrFailUserNickName = L"" );

	// �÷��� ���� üũ
	void	CheckAllMembersAcceptForPlay();
	void	FailProcessAcceptForPlay( IN const int iNetError, IN const UidType iFailUnitUID = 0 );

	// ���� ���� ���� ���� üũ
	void	FailProcessPrepareForPlay( IN const int iNetError );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

protected:
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	LeaveCandidateMember( IN const UidType iUnitUID );
	bool	LeaveMember( IN const UidType iUnitUID 
					#ifdef LOG_PARTY_BREAK
						, IN const int iReason	
					#endif // LOG_PARTY_BREAK
						);
	void	CloseParty( IN const int iReason );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// process by gsuser event..
protected:
	virtual inline void ProcessEvent( const KEventPtr& spEvent );
	
	DECL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ );
	DECL_ON_FUNC( EPM_INVITE_PARTY_REQ );
	DECL_ON_FUNC( EPM_INVITE_PARTY_REPLY_NOT );
	DECL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_NOT );
	DECL_ON_FUNC( EPM_LEAVE_PARTY_REQ );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_READY_REQ );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_REQ );
	DECL_ON_FUNC( EPM_PARTY_GAME_START_REQ );
	//{{ 2012. 01. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK );
   _DECL_ON_FUNC( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	DECL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_ACK );
	DECL_ON_FUNC( EPM_END_GAME_NOT );	
	DECL_ON_FUNC( EPM_PARTY_CHANGE_HOST_REQ );
	DECL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_ACK );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_REQ );
	DECL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ );
	DECL_ON_FUNC( EPM_PARTY_BAN_USER_REQ );
	DECL_ON_FUNC( EPM_CHAT_REQ );
	DECL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
   _DECL_ON_FUNC( EPM_INCREASE_PARTY_FEVER_NOT, KERM_INCREASE_PARTY_FEVER_NOT );
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( EPM_MAX_PARTY_FEVER_NOT, KERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	DECL_ON_FUNC( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT );
   _DECL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType );

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	DECL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
   _DECL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_REQ, KEGS_CHANGE_PARTY_TYPE_REQ );
	DECL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ );
   _DECL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ );
	DECL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ );
	DECL_ON_FUNC( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK );
	DECL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
	DECL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT );
	DECL_ON_FUNC_NOPARAM( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT );
	DECL_ON_FUNC( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT );
	DECL_ON_FUNC_NOPARAM( EPM_END_GAME_REGROUP_PARTY_NOT );

   _DECL_ON_FUNC( EPM_UPDATE_GAME_PLAY_STATUS_NOT, KPartyMemberStatus );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   //{{ 2012. 05. 30	������		���� ���� ���� ����
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
   DECL_ON_FUNC( EPM_UPDATE_PVP_INFO_NOT );
#endif SERV_OFFCIAL_PVP_BUG_FIX
   //}}

public:
	// SendTo
	template< class T >	void SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data );

	// BroadCast
	template< class T >	void BroadCastGameServer( IN unsigned short usEventID, IN const T& data );
	template< class T >	void BroadCast( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );
	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template< class T >	void BroadCastCadidateUser( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	void BroadCastID( IN u_short usEventID, IN const UidType iUnitUID_ = 0 );

protected:
	const UidType						m_iUID;							//UID�� �ѹ� ������ �ǵ��� �ʾƾ� �Ѵ�.
	std::wstring						m_wstrName;
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	boost::timer						m_tTimer[TE_MAX];
	bool								m_bRemakingAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	int									m_iDungeonID;
	//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
    char								m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	char								m_cDifficultyLevel;	
	bool								m_bPublic;
	char								m_cGetItemType;
    bool								m_bIsPlaying;					// ���� ���� �÷��� �������� ����

	CXSLDungeon::DUNGEON_MODE			m_eDungeonMode;	

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool								m_bCheckLowLevel;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	char								m_cPvpGameType; // CXSLRoom::PVP_GAME_TYPE
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KCnPartyUserManager					m_kPartyUserManager;	// ��Ƽ ���� �Ŵ���
	KCnPartyGameManager					m_kGameManager;			// ���� ���� ������
	//{{ 2012. 08. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KCnPartyFeverManager				m_kFeverManager;		// ��Ƽ fever �Ŵ���
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

template < class T >
void KCnParty::SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template < class T >
void KCnParty::BroadCastGameServer( IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}

template< class T >
void KCnParty::BroadCast( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
    m_kPartyUserManager.BroadCastToGSCharacter( usEventID, data, iUnitUID_ );
}

//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
template< class T >
void KCnParty::BroadCastCadidateUser( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
	m_kPartyUserManager.BroadCastToGSCharacter_CadidateUser( usEventID, data, iUnitUID_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////

