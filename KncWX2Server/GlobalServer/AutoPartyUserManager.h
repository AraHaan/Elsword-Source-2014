#pragma once

#include "AutoPartyUser.h"


//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


SmartPointer( KAutoPartyUserFSM );


class KAutoPartyUserManager
{
public:
    KAutoPartyUserManager();
    virtual ~KAutoPartyUserManager();

	void	Init( IN const KAutoPartyConditionInfo& kInfo );

	// �ʱ�ȭ �� ����
	bool	Enter( IN const KAutoPartyUserInfo& kInfo );
	bool	Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList );
    void	Reset();
	bool	DeleteUser( IN const UidType iUnitUID );

	// ���� ���
    KAutoPartyUserPtr GetUser( IN const UidType iUnitUID );
	void	GetAutoPartyUserList( OUT std::vector< UidType >& vecUserList );
    int		GetNumMember() const;
	int		GetLimitPlayerCount() const				{ return static_cast<int>(m_kConditionInfo.m_cLimitPlayerCount); }
	bool	IsExist( IN const UidType iUnitUID )	{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }
 
	// ����
	bool	Accept( IN const UidType iUnitUID );
	bool	IsAllPlayersAccept();

	// Regroup Party
	void	ClearAllPlayerRegroupPartyInfo();
	bool	UpdateRegroupPartyInfo( IN const KPartyUserInfo& kPartyUserInfo, IN const KPartyRoomUserInfo& kPartyRoomUserInfo );
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	bool	GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo, OUT UidType& iRegroupPartyHostUnitUID );
#else
	bool	GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo );
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	bool	IsAllPlayersUpdatedRegroupPartyInfo();

	//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
	void	GetAutoPartyCancelUserList( OUT std::vector< UidType >& vecCancelUserList );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
	//}

protected:
	KAutoPartyConditionInfo					m_kConditionInfo;
    std::map< UidType, KAutoPartyUserPtr >	m_mapMember;
	static FSMclassPtr						ms_spUserFSM;
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


