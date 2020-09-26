#pragma once

#include "MatchUser.h"


//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


SmartPointer( KMatchUserFSM );


class KMatchUserManager
{
public:
	enum RATING_ENUM
	{
		RE_HIGH = 0,
		RE_MIDDLE,
		RE_LOW,

		RE_MAX,
	};

	struct SPvpTeamSet
	{
		std::vector< UidType > m_vecRedTeam;
		std::vector< UidType > m_vecBlueTeam;
	};

public:
    KMatchUserManager();
    virtual ~KMatchUserManager();

	void	Init( IN const char cMatchType );

	// �ʱ�ȭ �� ����
	bool	Enter( IN const KMatchUserInfo& kInfo );
	bool	Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList );
    void	Reset();
	bool	DeleteUser( IN const UidType iUnitUID );

	// ���� ���
    KMatchUserPtr GetUser( IN const UidType iUnitUID );
	void	GetMatchUserList( OUT std::vector< UidType >& vecUserList );
    int		GetNumMember() const;
	bool	IsExist( IN const UidType iUnitUID )	{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }
	int		GetRatingAverage() const;
	void	GetMatchWaitTimeList( OUT std::map< UidType, int >& mapMatchWaitTime );

	// ��ũ
	bool	IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank );
	
	//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	struct SPartyMembers
	{
		int						m_iRating;
		std::vector<UidType>	vecPartyMembers;
		SPartyMembers( const int iRating, const std::vector<UidType>& vecPartyMembers )
		{
			m_iRating = iRating;
			this->vecPartyMembers = vecPartyMembers;
		}
		bool operator < ( const SPartyMembers& rhs ) const;
	};
	bool	IsAllPlayersBeginner( void );
	bool	ExistArrangeRankPlayer( void );
	int		GetRatingWeightAverage( void ) const;
	static int GetRatingWeightAverage( std::vector<KRoomUserInfo> vecTeam );
#endif SERV_2012_PVP_SEASON2
	//}}
 
	// ����
	bool	Accept( IN const UidType iUnitUID );
	bool	IsAllPlayersAccept();

	// ������
	bool	UpdateSelectedPvpMap( IN const UidType iUnitUID, IN const std::set< short >& setPvpMapInfo );
	void	GetAllPlayersSelectedPvpMap( OUT std::map< short, int >& mapPvpMapInfo ) const;

	// RoomUserInfo
	void	ClearAllPlayerRoomUserInfo();
	bool	UpdateRoomUserInfo( IN const KRoomUserInfo& kRoomUserInfo );
	bool	IsAllPlayersUpdatedRoomUserInfo();

	// ���Ӹ�� ����
	bool	UpdatePvpGameMode( IN const UidType iUnitUID, IN const bool bPvpPowerMode, IN const bool bPvpItemMode );
	void	DecisionGameMode( OUT bool& bIsItemMode ) const;

	// �� ����
	bool	TeamMemberSelection( OUT std::vector< KRoomUserInfo >& vecRedTeam, OUT std::vector< KRoomUserInfo >& vecBlueTeam );
	bool	GetRoomUserInfo( IN const std::vector< UidType >& vecUnitUIDList, OUT std::vector< KRoomUserInfo >& vecRoomUserInfo );
	
	//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
	void	GetCancelUserList( OUT std::vector< UidType >& vecCancelUserList );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
	//}

protected:
	bool	MakeTeamOptimalMatch( OUT std::vector< UidType >& vecRedTeam, OUT std::vector< UidType >& vecBlueTeam );
	bool	GetTeamRatingGapByRatingLevel( IN const std::vector< UidType >& vecSortedUserList, 
											IN const std::vector< int >& vecHighRating, 
											IN const std::vector< int >& vecMiddleRating, 
											IN const std::vector< int >& vecLowRating,
											OUT int& iGepTeamRating,
											OUT std::vector< UidType >& vecRedTeamResult,
											OUT std::vector< UidType >& vecBlueTeamResult );

protected:
	char								m_cMatchType;
	u_int								m_uiTotalPlayerCount;
	u_int								m_uiPlayerCount;
    std::map< UidType, KMatchUserPtr >	m_mapMember;
	static FSMclassPtr                  ms_spUserFSM;
};


#endif SERV_PVP_NEW_SYSTEM
//}}