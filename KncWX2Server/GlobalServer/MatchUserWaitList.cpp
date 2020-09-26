#include "MatchUserWaitList.h"

#include "MatchUserFSM.h"
#include "Enum/Enum.h"
#include "NetError.h"
//{{ 2011. 08. 30	������	��ġ ����ŷ ����
#ifdef SERV_PVP_MATCH_UPDATE
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_MATCH_UPDATE
//}}


//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


KMatchUserWaitList::KMatchUserWaitList( IN const int iMatchWaitNumber ) :
m_iMatchWaitNumber( iMatchWaitNumber )
{
}

KMatchUserWaitList::~KMatchUserWaitList()
{
}

UidType KMatchUserWaitList::GetPartyUID() const
{
	UidType iPartyUID = 0;

	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
        if( iPartyUID == 0 )
		{
			iPartyUID = spMatchUser->GetPartyUID();
			continue;
		}

		if( iPartyUID != spMatchUser->GetPartyUID() )
		{
			START_LOG( cerr, L"��.. �̰� �̻��ѵ�.. ���� ����ε� PartyUID�� Ʋ����? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iPartyUID )
				<< BUILD_LOG( spMatchUser->GetPartyUID() )
				<< END_LOG;
			continue;
		}
	}

	return iPartyUID;
}

void KMatchUserWaitList::GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList ) const
{
	vecUnitUIDList.clear();

	BOOST_TEST_FOREACH( const KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
        if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		vecUnitUIDList.push_back( spMatchUser->GetCharUID() );
	}
}

void KMatchUserWaitList::GetMatchUserInfoList( OUT std::vector< KMatchUserInfo >& vecMatchUserInfo ) const
{
	vecMatchUserInfo.clear();

	BOOST_TEST_FOREACH( const KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�!" )
				<< END_LOG;
			continue;
		}

		// �纻�� ���� �Ѵ�.
		KMatchUserInfo kInfo = spMatchUser->GetMatchUserInfo();
		kInfo.m_iMatchWaitTime = static_cast<int>(spMatchUser->GetMatchCompleteSeconds());
		vecMatchUserInfo.push_back( kInfo );
	}
}

KMatchUserPtr KMatchUserWaitList::GetMatchUser( IN const UidType iUnitUID )
{
	std::map< UidType, KMatchUserPtr >::iterator mit = m_mapMember.find( iUnitUID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ������ �����ϴ�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ������ �̻�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	return mit->second;
}

bool KMatchUserWaitList::AddMatchUser( KMatchUserPtr spMatchUser )
{
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻���." )
			<< END_LOG;
		return false;
	}

	if( IsExistMatchUser( spMatchUser->GetCharUID() ) == true )
	{
		START_LOG( cerr, L"�̹� ��ġ������ ��ϵǾ��ֽ��ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( spMatchUser->GetCharUID() )
			<< END_LOG;
		return false;
	}

	m_mapMember.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser ) );
	m_vecWaitList.push_back( spMatchUser );
	return true;
}

void KMatchUserWaitList::DelMatchUser( IN const UidType iUnitUID )
{
	m_mapMember.erase( iUnitUID );

	std::vector< KMatchUserPtr >::iterator vit;
	for( vit = m_vecWaitList.begin(); vit != m_vecWaitList.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->GetCharUID() == iUnitUID )
			break;
	}

	if( vit != m_vecWaitList.end() )
	{
		m_vecWaitList.erase( vit );
	}
}

int	KMatchUserWaitList::GetRatingAverage() const
{
	int iTotalRating = 0;

	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
        if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		iTotalRating += spMatchUser->GetRating();
	}

	const int iNumMember = static_cast<int>(m_vecWaitList.size());
	if( iNumMember == 0 )
	{
		START_LOG( cerr, L"���� ����� ����Ʈ��! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetMatchWaitNumber() )
			<< END_LOG;
		return 0;
	}
	
	return ( iTotalRating / iNumMember );
}

//{{ 2011. 08. 30	������	��ġ ����ŷ ����
#ifdef SERV_PVP_MATCH_UPDATE
int	KMatchUserWaitList::GetRatingWeightAverage() const
{
	if( GetNumMember() == 1 )
	{
		// ����� ȥ�ڶ�� �׳� ��հ��� ����.
		return GetRatingAverage();
	}

	std::map< UidType, int > mapTeamMemberInfo;
	int iMaxRating = 0;

	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		mapTeamMemberInfo.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser->GetRating() ) );

		if( iMaxRating < spMatchUser->GetRating() )
			iMaxRating = spMatchUser->GetRating();
	}

	const int iRatingWeightEverage = SiKPvpMatchManager()->GetRatingByTeam( mapTeamMemberInfo );
	const int iModifiedMaxRating = static_cast<int>( static_cast<double>(iMaxRating) * 0.8 );

	return max( iRatingWeightEverage, iModifiedMaxRating );
}
#endif SERV_PVP_MATCH_UPDATE
//}}

bool KMatchUserWaitList::IsPossibleMatchMakingBegin() const
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		if( spMatchUser->IsPossibleMatchMakingBegin() == false )
			return false;
	}
    
	return true;
}

bool KMatchUserWaitList::IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank )
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( !spMatchUser )
		{
			START_LOG( cerr, L"��ġ ���� ������ �̻�." )
				<< END_LOG;
			return false;
		}

		if( spMatchUser->GetPvpRank() != eRank )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserWaitList::CheckOptimalMatch( IN KMatchUserWaitListPtr spWaitList, OUT int& iOptimalMatchPoint )
{
	//{{ 2011. 08. 30	������	��ġ ����ŷ ����
#ifdef SERV_PVP_MATCH_UPDATE
	iOptimalMatchPoint = ::abs( GetRatingWeightAverage() - spWaitList->GetRatingWeightAverage() );
#else
	iOptimalMatchPoint = ::abs( GetRatingAverage() - spWaitList->GetRatingAverage() );
#endif SERV_PVP_MATCH_UPDATE
	//}}	
	return true;
}

//{{ 2012. 09. 15	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchUserWaitList::IsAllPlayersArrangeRank( void )
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( !spMatchUser )
		{
			START_LOG( cerr, L"��ġ ���� ������ �̻�." )
				<< END_LOG;
			return false;
		}

		if( spMatchUser->IsArrangeRank() == false )
		{
			return false;
		}
	}
	return true;
}

bool KMatchUserWaitList::IsAllPlayersBeginner( void )
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( !spMatchUser )
		{
			START_LOG( cerr, L"��ġ ���� ������ �̻�." )
				<< END_LOG;
			return false;
		}

		if( spMatchUser->IsBeginner() == false )
		{
			return false;
		}
	}
	return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

#endif SERV_PVP_NEW_SYSTEM
//}}

