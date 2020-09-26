#pragma once

#include "CnPartyUser.h"



class KCnPartyUserManager
{
public:
	KCnPartyUserManager();
	virtual ~KCnPartyUserManager();
	
	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	Tick( OUT std::vector< UidType >& vecDeleteCadidateUser );
#else
	void	Tick();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	void	Reset();
	void	SetMaxMember( char cMaxMember )								{ m_cMaxMember = cMaxMember; }

	// ������ ��� �Լ�
	KCnPartyUserPtr	GetHostPtr() const;
	UidType	GetHostUID() const;

	KCnPartyUserPtr GetPartyUser( IN const UidType nCID );
	KCnPartyUserPtr GetCandidateUser( IN const UidType nCID );

	bool	GetUserInfo( IN UidType nCID, OUT KPartyUserInfo& kInfo );
	void	GetUserInfoList( OUT std::vector< KPartyUserInfo >& vecInfoList );
	bool	GetDungeonRoomUserInfoList( OUT std::vector< KPartyRoomUserInfo >& vecInfoList );	
	void	GetListUserInfoList( OUT std::vector< KPartyListUserInfo >& vecUserInfo );
	//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	void	GetMatchUserInfoList( IN const UidType iPartyUID, OUT std::vector< KMatchUserInfo >& vecUserList );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	GetAutoPartyUserInfoList( IN const KOldPartyInfo& kOldPartyInfo, OUT std::vector< KAutoPartyUserInfo >& vecUserList );
	bool	GetGSUID( IN const UidType nCID, OUT UidType& iGSUID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	void	GetUnitUIDList( OUT std::set< UidType >& setUidList ) const;
	void	GetUnitUIDList( OUT std::vector< UidType >& vecUidList ) const;
	void	GetUnitUIDListByGSUID( IN UidType iGSUID, OUT std::vector< UidType >& vecUserList ) const;

	int		GetNumMember() const										{ return static_cast<int>(m_mapMember.size()); }
	int		GetNumCandidate() const										{ return static_cast<int>(m_mapCandidate.size()); }
	int		GetNumTotal() const											{ return GetNumMember() + GetNumCandidate(); }
	int		GetMaxMember() const										{ return m_cMaxMember; }

	// üũ �Լ�
	bool	IsMember( IN const UidType nCID ) const							{ return ( m_mapMember.find( nCID ) != m_mapMember.end() ); }
	bool	IsCandidate( IN const UidType nCID ) const						{ return ( m_mapCandidate.find( nCID ) != m_mapCandidate.end() ); }
	bool	IsMemberOrCandidate( IN const UidType nCID ) const				{ return ( IsMember( nCID )  ||  IsCandidate( nCID) ); }
	bool	IsHost( IN UidType nCID );
	bool	IsAllReady();
	bool	IsAllCheckedGameStart();
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	bool	IsAllPartyMembersSameServerGroup( IN UidType iUnitUID );
#endif SERV_INTEGRATION
	//}}
	//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	bool	IsAllPartyMemberEqualOrOverThisLevel( IN const u_char ucLevel );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	// ���� ����, ���� �Ҹ�
	bool	Create( IN const KPartyUserInfo& kUserInfo );
	bool	Create( IN const std::vector< KPartyUserInfo >& vecUserInfo );
	bool	Create( IN const std::vector< KRegroupPartyUserInfo >& vecUserInfo );
	bool	JoinCandidate( IN const KPartyUserInfo& kUserInfo, IN const bool bInvited, IN const UidType iSendInviteUnitUID );
	bool	JoinMember( IN const KPartyUserInfo& kUserInfo );
	bool	LeaveCandidate( IN UidType iUnitUID );
	bool	LeaveMember( IN UidType iUnitUID );

	bool	ChangeReady( IN UidType nCID, IN bool bIsReady );
	void	ChangeAllReady( IN bool bIsReady );	// ���� ���°� �ٲ�鼭 ����������� ��ε� ĳ���õ� �����Ѵ�.
	bool	ChangeHost( IN UidType iNewHostUID );

	void	ClearDungeonRoomUserInfoList();
	void	UpdatePartyMemberListToFieldUser( IN const UidType iPartyUID );											// ��Ƽ��� ����Ʈ�� FieldUser���� ������Ʈ

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	void	SetComeBackUserInParty( IN bool bComeBack )				{ m_bComeBackUserInParty = bComeBack; }
	bool	GetComeBackUserInParty()								{ return m_bComeBackUserInParty; }
	bool	CheckComeBackUserInParty();
	void	SetComeBackBuffEnd( IN const std::wstring& wstrBuffEnd );
	CTime&	GetComeBackBuffEnd()									{ return m_tComeBackBuffEnd; }
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	GetCadidateUserList( OUT std::vector< UidType >& vecCadidateUser );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 03. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	SetPartyMemberStatus( IN const UidType iUnitUID, IN const KPartyMemberStatus& kInfo );
	short	GetOldPartyFeverPoint() const;

	bool	UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, 
												 IN const KPartyUserJoinRoomInfo& kJoinRoomInfo, 
												 OUT std::vector< UidType >& vecOnMoveButton,
												 OUT std::vector< UidType >& vecOffMoveButton );

	void	GetUnitUIDListAboutEndGameRegroupParty( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList );
	void	GetCheckedSameOldParty( OUT std::map< UidType, std::set< UidType > >& mapCheckedSameOldParty, OUT std::set< UidType >& setLeaveUnitUIDList );
	void	CheckRegroup1Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList );
	void	CheckRegroup2Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList );
	void	CheckRegroup3Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList );
	void	CheckRegroup4Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList );

	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	bool	ChangeOldPartyHost( OUT UidType& iNewHostUID );
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	// �ڵ� ��Ƽ�� ���� üũ
	void	ClearCheckedForAutoParty();
	bool	SetCheckedForAutoParty( IN const UidType iUnitUID );
	bool	IsAllPartyMembersCheckedForAutoParty() const;

	// �ٷ� ���� ���� ������ ���� üũ
	void	ClearCheckedPlayCondition();
	bool	SetCheckedPlayCondition( IN const UidType iUnitUID );
	bool	IsAllPartyMembersCheckedPlayCondition() const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

protected:
	bool	AddCandidate( IN const KPartyUserInfo& kUserInfo, IN const bool bInvited, IN const UidType iSendInviteUnitUID );
	bool	AddMember( IN const KPartyUserInfo& kUserInfo );
	bool	DeleteCandidate( IN UidType nCID );
	bool	DeleteMember( IN UidType nCID );

public:
	template< class T >	void BroadCastToGSCharacter( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );
	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	template< class T >	void BroadCastToGSCharacter_CadidateUser( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ = 0 );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
	static const float			m_scfCandidateTimeLimit;

private:
	struct KWaitingUserInfo
	{
		KCnPartyUserPtr	m_spUser;
		boost::timer    m_kTimer;
		bool            m_bInvited;
		//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		UidType			m_iSendInviteUnitUID;	// �ʴ����� ���� ���
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		KWaitingUserInfo()
		{
			m_bInvited = false;
			//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			m_iSendInviteUnitUID = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}			
		}
	};

private:
	char									m_cMaxMember;
	std::map< UidType, KCnPartyUserPtr >	m_mapMember;        // ��Ƽ ���
	std::map< UidType, KWaitingUserInfo >	m_mapCandidate;     // ��Ƽ ���� ��� ���

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	bool									m_bComeBackUserInParty;
	CTime									m_tComeBackBuffEnd;
#endif SERV_COME_BACK_USER_REWARD
	//}}
};

template< class T >
void KCnPartyUserManager::BroadCastToGSCharacter( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( iUnitUID_ != 0  &&  mit->second->GetCharUID() == iUnitUID_ )
			continue;

		mit->second->SendToGSCharacter( usEventID, data );
	}
}

//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
template< class T >
void KCnPartyUserManager::BroadCastToGSCharacter_CadidateUser( IN u_short usEventID, IN const T& data, IN const UidType iUnitUID_ /*= 0*/ )
{
	std::map< UidType, KWaitingUserInfo >::const_iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		if( IS_NULL( mit->second.m_spUser ) )
			continue;

		if( iUnitUID_ != 0  &&  mit->second.m_spUser->GetCharUID() == iUnitUID_ )
			continue;

		mit->second.m_spUser->SendToGSCharacter( usEventID, data );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

