#include "MatchUser.h"
#include "Enum/Enum.h"


//{{ 2011. 06. 14	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


KMatchUser::KMatchUser( IN const KMatchUserInfo& kInfo ) :
m_bPvpPowerMode( false ),
m_bPvpItemMode( false )
{
	m_kMatchUserInfo = kInfo;
}

KMatchUser::~KMatchUser()
{
}

CXSLUnit::PVP_RANK KMatchUser::GetPvpRank() const
{
	//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	return static_cast<CXSLUnit::PVP_RANK>( GetCurrentRank() );
#else
	const CXSLUnit::PVP_EMBLEM eEmblem = CXSLUnit::GetPVPEmblem( GetRating() );
	return CXSLUnit::PvpEmblemToPvpRank( eEmblem );
#endif SERV_2012_PVP_SEASON2
	//}}
}

bool KMatchUser::IsPossibleMatchMakingBegin() const
{
	// 5�ʰ� ������ ��ġ ����ŷ�� �����Ѵ�.
	return ( m_tTimer[TE_MATCH_MAKING_BEGIN].elapsed() > 1.0 );
}

bool KMatchUser::Accept()
{
	if( IsAccepted() == true )
	{
		START_LOG( cerr, L"�̹� ������ �����̴�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return false;
	}

	StateTransition( KMatchUserFSM::I_TO_ACCEPTED );
	return true;
}

bool KMatchUser::IsAccepted()
{
	return ( GetStateID() == KMatchUserFSM::S_ACCEPTED );
}

void KMatchUser::UpdatePvpGameMode( IN const bool bPvpPowerMode, IN const bool bPvpItemMode )
{
	m_bPvpPowerMode = bPvpPowerMode;
	m_bPvpItemMode = bPvpItemMode;
}

void KMatchUser::UpdateSelectedPvpMap( IN const std::set< short >& setPvpMapInfo )
{
	LIF( setPvpMapInfo.size() <= SEnum::PE_MAX_SELECT_PVP_MAP_COUNT );

	m_setSelectedPvpMap = setPvpMapInfo;
}

void KMatchUser::UpdateRoomUserInfo( IN const KRoomUserInfo& kRoomUserInfo )
{
	if( IsEmptyRoomUserInfo() == false )
	{
		START_LOG( cerr, L"�̹� RoomUserInfo�� �ִµ� �� ������Ʈ �Ϸ� ���Գ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecRoomUserInfo.size() )
			<< END_LOG;
	}

	// RoomUserInfo �ʱ�ȭ
	ClearRoomUserInfo();

	// ������ RoomUserInfo�� ��������!
	m_vecRoomUserInfo.push_back( kRoomUserInfo );
}

bool KMatchUser::GetRoomUserInfo( OUT KRoomUserInfo& kRoomUserInfo )
{
	if( IsEmptyRoomUserInfo() == true )
	{
		START_LOG( cerr, L"RoomUserInfo�� ����ִ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecRoomUserInfo.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KRoomUserInfo&, kInfo, m_vecRoomUserInfo )
	{
		kRoomUserInfo = kInfo;
		return true;
	}

	return false;
}

//double KMatchUser::GetExpectPercentageOfVictories() const
//{
//    return 0.0;
//}


void KMatchUser::SendToGSCharacter( unsigned short usEventID )
{
    SendToGSCharacter( usEventID, char() );
}


#endif SERV_PVP_NEW_SYSTEM
//}}



