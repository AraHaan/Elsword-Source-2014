#include "Match.h"
#include "MatchUserManager.h"

#include "Enum/Enum.h"
#include "NetError.h"
#include "MatchUserFSM.h"
#include "CompareLimitList.h"
//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


FSMclassPtr KMatchUserManager::ms_spUserFSM = KMatchUserFSMPtr( new KMatchUserFSM );


KMatchUserManager::KMatchUserManager() :
m_cMatchType( 0 ),
m_uiTotalPlayerCount( 0 ),
m_uiPlayerCount( 0 )
{
}

KMatchUserManager::~KMatchUserManager()
{
}

void KMatchUserManager::Init( IN const char cMatchType )
{
	m_cMatchType		 = cMatchType;
	m_uiPlayerCount		 = KMatch::GetMatchPlayerCount( static_cast<SEnum::MATCH_TYPE>(cMatchType) );
	m_uiTotalPlayerCount = KMatch::GetMatchTotalPlayerCount( static_cast<SEnum::MATCH_TYPE>(cMatchType) );
}

bool KMatchUserManager::Enter( IN const KMatchUserInfo& kInfo )
{
    if( IsExist( kInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� ������ �ִ� �����Դϴ�!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	if( ( m_mapMember.size() + 1 )  > m_uiTotalPlayerCount )
	{
		START_LOG( cerr, L"�ִ� ���� �ο� ���ѿ� �ɷȽ��ϴ�." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	KMatchUserPtr spMatchUser( new KMatchUser( kInfo ) );
	spMatchUser->SetFSM( ms_spUserFSM );
	m_mapMember.insert( std::make_pair( kInfo.m_iUnitUID, spMatchUser ) );
	return true;
}

bool KMatchUserManager::Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList )
{
	if( ( m_mapMember.size() + vecMatchUserList.size() )  > m_uiTotalPlayerCount )
	{
		START_LOG( cerr, L"�ִ� ���� �ο� ���ѿ� �ɷȽ��ϴ�." )
			<< BUILD_LOG( m_uiTotalPlayerCount )
			<< BUILD_LOG( m_mapMember.size() )
			<< BUILD_LOG( vecMatchUserList.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KMatchUserInfo, kInfo, vecMatchUserList )
	{
		if( Enter( kInfo ) == false )
		{
			START_LOG( cerr, L"��ġ ���� ó�� ����!" )
				<< END_LOG;
		}
	}

	return true;
}

void KMatchUserManager::Reset()
{
}

KMatchUserPtr KMatchUserManager::GetUser( IN const UidType iUnitUID )
{
    std::map< UidType, KMatchUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"�������� �ʴ� ����." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return KMatchUserPtr();
    }

    return mit->second;
}

void KMatchUserManager::GetMatchUserList( OUT std::vector< UidType >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		vecUserList.push_back( mit->first );
	}
}

bool KMatchUserManager::DeleteUser( IN const UidType iUnitUID )
{
    std::map< UidType, KMatchUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"�������� �ʴ� ��������." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;
        return true;
    }

    m_mapMember.erase( mit );
    return true;
}

int KMatchUserManager::GetNumMember() const
{
    return static_cast<int>(m_mapMember.size());
}

int	KMatchUserManager::GetRatingAverage() const
{
	int iTotalRating = 0;

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		iTotalRating += spMatchUser->GetRating();
	}

	const int iNumMember = GetNumMember();
	if( iNumMember == 0 )
	{
		START_LOG( cerr, L"����� ���� ���� �ʴ� ��ġ��! ���� �Ͼ���� �ȵǴ� ����!" )
			<< END_LOG;
		return 0;
	}

	return ( iTotalRating / iNumMember );
}

void KMatchUserManager::GetMatchWaitTimeList( OUT std::map< UidType, int >& mapMatchWaitTime )
{
	mapMatchWaitTime.clear();

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		mapMatchWaitTime.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser->GetMatchWaitTime() ) );
	}
}

bool KMatchUserManager::IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank )
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->GetPvpRank() != eRank )
		{
			return false;
		}
	}

	return true;
}

//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchUserManager::IsAllPlayersBeginner( void )
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsBeginner() == false )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::ExistArrangeRankPlayer( void )
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsArrangeRank() == true )
		{
			return true;
		}
	}
	return false;
}

int	KMatchUserManager::GetRatingWeightAverage( void ) const
{
	if( GetNumMember() == 1 )
	{
		// ����� ȥ�ڶ�� �׳� ��հ��� ����.
		return GetRatingAverage();
	}

	std::map< UidType, int > mapTeamMemberInfo;
	int iMaxRating = 0;

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
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

int KMatchUserManager::GetRatingWeightAverage( std::vector<KRoomUserInfo> vecTeam )
{
	if( vecTeam.empty() == true )
	{
		return 0;
	}

	if( vecTeam.size() == 1 )
	{
		KRoomUserInfo kRoomUserInfo = vecTeam.front();
		// ����� ȥ�ڶ�� �׳� Rating�� ��ȯ�Ѵ�.
		return kRoomUserInfo.m_iRating;
	}

	std::map< UidType, int > mapTeamMemberInfo;
	int iMaxRating = 0;

	BOOST_TEST_FOREACH( const KRoomUserInfo&, kRoomUserInfo, vecTeam )
	{
		mapTeamMemberInfo.insert( std::make_pair( kRoomUserInfo.m_nUnitUID, kRoomUserInfo.m_iRating ) );

		if( iMaxRating < kRoomUserInfo.m_iRating )
			iMaxRating = kRoomUserInfo.m_iRating;
	}

	const int iRatingWeightAverage = SiKPvpMatchManager()->GetRatingByTeam( mapTeamMemberInfo );
	const int iModifiedMaxRating = static_cast<int>( static_cast<double>( iMaxRating ) * 0.8 );

	return max( iRatingWeightAverage, iModifiedMaxRating );
}
#endif SERV_2012_PVP_SEASON2
//}}

bool KMatchUserManager::Accept( IN const UidType iUnitUID )
{
	KMatchUserPtr spMatchUser = GetUser( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spMatchUser->Accept();
}

bool KMatchUserManager::IsAllPlayersAccept()
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsAccepted() == false )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::UpdateSelectedPvpMap( IN const UidType iUnitUID, IN const std::set< short >& setPvpMapInfo )
{
	KMatchUserPtr spMatchUser = GetUser( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spMatchUser->UpdateSelectedPvpMap( setPvpMapInfo );
	return true;
}

void KMatchUserManager::GetAllPlayersSelectedPvpMap( OUT std::map< short, int >& mapPvpMapInfo ) const
{
	mapPvpMapInfo.clear();

	std::map< short, int >::iterator mitSM;
    
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		BOOST_TEST_FOREACH( const short, sMapID, spMatchUser->GetSelectedPvpMap() )
		{
			mitSM = mapPvpMapInfo.find( sMapID );
			if( mitSM == mapPvpMapInfo.end() )
			{
				mapPvpMapInfo.insert( std::make_pair( sMapID, 1 ) );
			}
			else
			{
                ++mitSM->second;
			}
		}
	}    
}

void KMatchUserManager::ClearAllPlayerRoomUserInfo()
{
	std::map< UidType, KMatchUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}
        
		spMatchUser->ClearRoomUserInfo();
	}
}

bool KMatchUserManager::UpdateRoomUserInfo( IN const KRoomUserInfo& kRoomUserInfo )
{
	KMatchUserPtr spMatchUser = GetUser( kRoomUserInfo.m_nUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( kRoomUserInfo.m_nUnitUID )
			<< END_LOG;
		return false;
	}

	spMatchUser->UpdateRoomUserInfo( kRoomUserInfo );
	return true;
}

bool KMatchUserManager::IsAllPlayersUpdatedRoomUserInfo()
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsEmptyRoomUserInfo() == true )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::UpdatePvpGameMode( IN const UidType iUnitUID, IN const bool bPvpPowerMode, IN const bool bPvpItemMode )
{
	KMatchUserPtr spMatchUser = GetUser( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spMatchUser->UpdatePvpGameMode( bPvpPowerMode, bPvpItemMode );
	return true;
}

void KMatchUserManager::DecisionGameMode( OUT bool& bIsItemMode ) const
{
	bIsItemMode = false;
	int iPowerModeCount = 0;
	int iItemModeCount = 0;

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spMatchUser->IsPvpPowerMode() == true )
		{
			++iPowerModeCount;
		}

		if( spMatchUser->IsPvpItemMode() == true )
		{
			++iItemModeCount;
		}
	}

	if( iPowerModeCount > iItemModeCount )
	{
		// �Ŀ���尡 �� ���� ���õ�!
		bIsItemMode = false;
		return;
	}
	else if( iPowerModeCount < iItemModeCount )
	{
		bIsItemMode = false;
		return;
	}

	// ���� ������ ī��Ʈ�� ���ٸ� 50%Ȯ���� ����!
	const int iRandomResult = rand() % 2;
	bIsItemMode = ( iRandomResult == 0 );
}

//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchUserManager::SPartyMembers::operator < ( const SPartyMembers& rhs ) const
{
	if( vecPartyMembers.size() < rhs.vecPartyMembers.size() )		/// ��Ƽ���� ���� ���� ���� ������ ������ ����
		return false;
	else if( vecPartyMembers.size() == rhs.vecPartyMembers.size() )	/// ��Ƽ���� ���� ������ �������� ���� ū���� ������ ������ ����
		return ( m_iRating < rhs.m_iRating ? false : true );
	else
		return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

bool KMatchUserManager::TeamMemberSelection( OUT std::vector< KRoomUserInfo >& vecRedTeam, OUT std::vector< KRoomUserInfo >& vecBlueTeam )
{
	vecRedTeam.clear();
	vecBlueTeam.clear();


	//////////////////////////////////////////////////////////////////////////	
	// 1. �� �����ϱ����� �ο��� �´������� Ȯ��!
	if( m_uiTotalPlayerCount != m_mapMember.size() )
	{
		START_LOG( cerr, L"���� �� �����Ϸ��µ� �ο����� �ȸ³�? ���� �Ͼ���� �ȵǴ¿���!" )
			<< BUILD_LOG( m_uiTotalPlayerCount )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;
		return false;
	}


	//////////////////////////////////////////////////////////////////////////	
	// 2. ��Ƽ������ ����!
	std::map< UidType, std::vector< UidType > > mapPartyMembers;

	{
		std::map< UidType, std::vector< UidType > >::iterator mitPM;
		std::map< UidType, KMatchUserPtr >::const_iterator mit;
		for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
		{
			KMatchUserPtr spMatchUser = mit->second;
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				return false;
			}

			// ��Ƽ ������ ������ ��Ƽ������ ����!
			mitPM = mapPartyMembers.find( spMatchUser->GetPartyUID() );
			if( mitPM == mapPartyMembers.end() )
			{
				std::vector< UidType > vecMemberList;
				vecMemberList.push_back( spMatchUser->GetCharUID() );
				mapPartyMembers.insert( std::make_pair( spMatchUser->GetPartyUID(), vecMemberList ) );
			}
			else
			{
				mitPM->second.push_back( spMatchUser->GetCharUID() );
			}
		}
	}	


	//////////////////////////////////////////////////////////////////////////	
	// 3. ��Ƽ�� ������ �Ǿ� ���ü� �ִ��� ����!
	std::vector< UidType > vecRedTeamUnitUIDList;
	std::vector< UidType > vecBlueTeamUnitUIDList;

	//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	// �ο����� ����, �������� ū ������ �����Ѵ�.
	std::multiset<SPartyMembers> msetPartyMembers;
	std::vector<KRoomUserInfo> vecTempTeam;
	for( std::map< UidType, std::vector< UidType > >::iterator it=mapPartyMembers.begin(); it != mapPartyMembers.end(); ++it )
	{
		const UidType iPartyUID = it->first;
		const std::vector<UidType>& vecUnitUID = it->second;

		// party uid�� 0�� ���� ��Ƽ�� ������ �ʰ� ȥ�ڼ� ��Ī ��û�� ����.
		// ��Ƽ�� �Ҽӵ� ���� �ƴϱ� ������ ���������� �ٷ��� �Ѵ�.
		if( iPartyUID == 0 )
		{
			BOOST_TEST_FOREACH( const UidType&, iUnitUID, vecUnitUID )
			{
				std::vector<UidType> vecTemp;
				vecTemp.push_back( iUnitUID );
				GetRoomUserInfo( vecTemp, vecTempTeam );
				msetPartyMembers.insert( SPartyMembers( GetRatingWeightAverage( vecTempTeam ), vecTemp ) );
			}
			continue;
		}
		GetRoomUserInfo( vecUnitUID, vecTempTeam );
		msetPartyMembers.insert( SPartyMembers( GetRatingWeightAverage( vecTempTeam ), vecUnitUID ) );
	}

	int iTotalRatingRed		= 0;
	int iTotalRatingBlue	= 0;
	for( std::multiset<SPartyMembers>::iterator it=msetPartyMembers.begin(); it != msetPartyMembers.end(); ++it )
	{
		const SPartyMembers& sPartyMembers = *it;

		// ������ �������� �� �۰� �� ������ �ִٸ�, �������� ����
		if( ( iTotalRatingRed < iTotalRatingBlue ) && ( ( vecRedTeamUnitUIDList.size() + sPartyMembers.vecPartyMembers.size() ) <= m_uiPlayerCount ) )
		{
			vecRedTeamUnitUIDList.insert( vecRedTeamUnitUIDList.end(), sPartyMembers.vecPartyMembers.begin(), sPartyMembers.vecPartyMembers.end() );
			iTotalRatingRed += sPartyMembers.m_iRating;
		}
		else
		{
			// �׷��� ���� ���
			if( ( vecBlueTeamUnitUIDList.size() + sPartyMembers.vecPartyMembers.size() ) <= m_uiPlayerCount )
			{
				// ��� ���� ������ �ִٸ�, ������� �ְ�
				vecBlueTeamUnitUIDList.insert( vecBlueTeamUnitUIDList.end(), sPartyMembers.vecPartyMembers.begin(), sPartyMembers.vecPartyMembers.end() );
				iTotalRatingBlue += sPartyMembers.m_iRating;
			}
			else
			{
				// ������� ������ ���ٸ�, �������� �ִ´�.
				vecRedTeamUnitUIDList.insert( vecRedTeamUnitUIDList.end(), sPartyMembers.vecPartyMembers.begin(), sPartyMembers.vecPartyMembers.end() );
				iTotalRatingRed += sPartyMembers.m_iRating;
			}
		}
	}
#else

	{
		std::set< UidType > setInsertedPartyUID;
		std::map< UidType, std::vector< UidType > >::const_iterator mitPM;
		for( mitPM = mapPartyMembers.begin(); mitPM != mapPartyMembers.end(); ++mitPM )
		{
			const UidType iPartyUID = mitPM->first;
			const std::vector< UidType >& vecMemberList = mitPM->second;
			
			// ��Ƽ�� �ƴϸ� �ǳʶ���!
			if( iPartyUID == 0 )
				continue;

			// �ַ� �÷��̾�� �ǳʶ���!
			if( vecMemberList.size() == 1 )
				continue;

			// ��Ƽ ������� ���� �ο����� ���� ���!
            if( vecMemberList.size() != m_uiPlayerCount )
			{
				START_LOG( cerr, L"��Ƽ�ε� �ַε� �ƴϸ鼭 �ο����� �ȸ³�.. �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( iPartyUID )
					<< BUILD_LOG( vecMemberList.size() )
					<< BUILD_LOG( m_uiPlayerCount )
					<< END_LOG;
				return false;
			}

			// �������� ��������� ���������� ����! �������� �̹� �����Ǿ������� ��������� ����!
			if( vecRedTeamUnitUIDList.empty() )
			{
				vecRedTeamUnitUIDList = vecMemberList;
				setInsertedPartyUID.insert( iPartyUID );
				continue;
			}
			else if( vecBlueTeamUnitUIDList.empty() )
			{
                vecBlueTeamUnitUIDList = vecMemberList;
				setInsertedPartyUID.insert( iPartyUID );
				continue;
			}
			else
			{
				START_LOG( cerr, L"�̹� ������� �����Ǿ��µ� �� �ο��� ���ҳ�? ���� �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( vecRedTeamUnitUIDList.size() )
					<< BUILD_LOG( vecBlueTeamUnitUIDList.size() )
					<< BUILD_LOG( vecMemberList.size() )
					<< END_LOG;
                return false;
			}
		}

		// �� ������ �Ϸ�� ��Ƽ�� ����Ʈ���� �����!
		BOOST_TEST_FOREACH( const UidType, iPartyUID, setInsertedPartyUID )
		{
            mapPartyMembers.erase( iPartyUID );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 4. �������� ������ �Ϸ�� ���!
	if( vecRedTeamUnitUIDList.size() == m_uiPlayerCount  &&  vecBlueTeamUnitUIDList.empty() == true )
	{
		std::map< UidType, std::vector< UidType > >::const_iterator mitPM;
		for( mitPM = mapPartyMembers.begin(); mitPM != mapPartyMembers.end(); ++mitPM )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, mitPM->second )
			{
				vecBlueTeamUnitUIDList.push_back( iUnitUID );
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 4. ������� ������ �Ϸ�� ���!
	else if( vecBlueTeamUnitUIDList.size() == m_uiPlayerCount  &&  vecRedTeamUnitUIDList.empty() == true )
	{
		std::map< UidType, std::vector< UidType > >::const_iterator mitPM;
		for( mitPM = mapPartyMembers.begin(); mitPM != mapPartyMembers.end(); ++mitPM )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, mitPM->second )
			{
				vecRedTeamUnitUIDList.push_back( iUnitUID );
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 5. ��� �� ������ �Ϸ���� ���� ���!
	else if( vecRedTeamUnitUIDList.empty() == true  &&  vecBlueTeamUnitUIDList.empty() == true )
	{
        if( MakeTeamOptimalMatch( vecRedTeamUnitUIDList, vecBlueTeamUnitUIDList ) == false )
		{
			START_LOG( cerr, L"������ �� ������ �����Ͽ����ϴ�!" )
				<< BUILD_LOG( m_uiPlayerCount )
				<< BUILD_LOG( GetNumMember() )
				<< END_LOG;
			return false;
		}
	}
#endif SERV_2012_PVP_SEASON2
	//}}


	//////////////////////////////////////////////////////////////////////////
	// 6. ������� �Դµ� �� ������ �Ϸ���� �ʾҴٸ� ������ �ִ°Ŵ�!
	if( vecRedTeamUnitUIDList.size() != m_uiPlayerCount  ||  vecBlueTeamUnitUIDList.size() != m_uiPlayerCount )
	{
		START_LOG( cerr, L"��Ī �� ������ �����Ͽ����ϴ�!" )
			<< BUILD_LOG( vecRedTeamUnitUIDList.size() )
			<< BUILD_LOG( vecBlueTeamUnitUIDList.size() )
			<< END_LOG;
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	// 7. RoomUserInfo�� ����!
	{
		if( GetRoomUserInfo( vecRedTeamUnitUIDList, vecRedTeam ) == false )
		{
			START_LOG( cerr, L"RoomUserInfo ��Ⱑ �����Ͽ����ϴ�." )
				<< BUILD_LOG( vecRedTeamUnitUIDList.size() )
				<< END_LOG;
			return false;
		}

		if( GetRoomUserInfo( vecBlueTeamUnitUIDList, vecBlueTeam ) == false )
		{
			START_LOG( cerr, L"RoomUserInfo ��Ⱑ �����Ͽ����ϴ�." )
				<< BUILD_LOG( vecBlueTeamUnitUIDList.size() )
				<< END_LOG;
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::GetRoomUserInfo( IN const std::vector< UidType >& vecUnitUIDList, OUT std::vector< KRoomUserInfo >& vecRoomUserInfo )
{
	vecRoomUserInfo.clear();

    BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDList )
	{
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� ������ �̻�." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return false;
		}
        
		KRoomUserInfo kInfo;
		if( spMatchUser->GetRoomUserInfo( kInfo ) == false )
		{
			START_LOG( cerr, L"�� ���� ������ ����!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return false;
		}

		vecRoomUserInfo.push_back( kInfo );
	}

	return true;
}

bool KMatchUserManager::MakeTeamOptimalMatch( OUT std::vector< UidType >& vecRedTeam, OUT std::vector< UidType >& vecBlueTeam )
{
	vecRedTeam.clear();
	vecBlueTeam.clear();

	std::vector< UidType > vecSortedUserList;

	//////////////////////////////////////////////////////////////////////////	
	// 1, ���� ������ ������ �����Ѵ�!
	{
		KCompareLimitList<UidType,6> listSortedUserList;

		std::map< UidType, KMatchUserPtr >::const_iterator mit;
		for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
		{
			KMatchUserPtr spMatchUser = mit->second;
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				return false;
			}

			listSortedUserList.IfLowerValuePushBack( spMatchUser->GetRating(), spMatchUser->GetCharUID() );
		}

		listSortedUserList.GetValueList( vecSortedUserList );
	}
	

	//////////////////////////////////////////////////////////////////////////	
	// (2) ���ʷ� ��� 2�ܰ� �Ǵ� 3�ܰ踦 �����.
	std::vector< int > vecIndex[RE_MAX];

	{
		// ���ĵ� ���� �� ��ŭ �����ؼ� �ֽ��ϴ�!
		for( u_int ui = 0; ui < vecSortedUserList.size(); ++ui )
		{
			const bool bIsRed = ( ( ui % 2 ) == 0 );
			const int iIndex = ( ui / 2 );
			if( iIndex < 0  ||  iIndex >= RE_MAX )
			{
				START_LOG( cerr, L"�̻��� �ε��� ���� ���Դ�! �Ͼ�� �ȵǴ� ����!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( ui )
					<< BUILD_LOG( iIndex )
					<< END_LOG;
				return false;
			}

			if( bIsRed )
			{
				vecIndex[iIndex].push_back( ui );
			}
			else
			{
				vecIndex[iIndex].push_back( ui );
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////	
	// (3) �� ����� �� ���� ������ �������� ã�Ƴ��� ������ �����Ѵ�!
	std::vector< UidType > vecRedTeamTemp;
	std::vector< UidType > vecBlueTeamTemp;

	{		
		int iBestOptimalMatchRating = INT_MAX;
		
		{
			// ù��° ����� ��
			int iGapTeamRating = 0;
			if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
			{
				START_LOG( cerr, L"�� ������ ���̰� ���ϱ� ����!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( vecIndex[RE_HIGH].size() )
					<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
					<< BUILD_LOG( vecIndex[RE_LOW].size() )
					<< END_LOG;
				return false;
			}

			// ������ ��ġ ���� �ΰ�?
			if( iGapTeamRating < iBestOptimalMatchRating )
			{
				iBestOptimalMatchRating = iGapTeamRating;
				vecRedTeam = vecRedTeamTemp;
				vecBlueTeam = vecBlueTeamTemp;
			}
		}

		while( next_permutation( vecIndex[RE_LOW].begin(), vecIndex[RE_LOW].end() ) )
		{
			// �� ���� ����� ��
			int iGapTeamRating = 0;
			std::vector< int > vecIndexResult;
			if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
			{
				START_LOG( cerr, L"�� ������ ���̰� ���ϱ� ����!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( vecIndex[RE_HIGH].size() )
					<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
					<< BUILD_LOG( vecIndex[RE_LOW].size() )
					<< END_LOG;
				return false;
			}

			// ������ ��ġ ���� �ΰ�?
			if( iGapTeamRating < iBestOptimalMatchRating )
			{
				iBestOptimalMatchRating = iGapTeamRating;
				vecRedTeam = vecRedTeamTemp;
				vecBlueTeam = vecBlueTeamTemp;
			}
		}

		while( next_permutation( vecIndex[RE_MIDDLE].begin(), vecIndex[RE_MIDDLE].end() ) )
		{
			// �� ���� ����� ��
			int iGapTeamRating = 0;
			std::vector< int > vecIndexResult;
			if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
			{
				START_LOG( cerr, L"�� ������ ���̰� ���ϱ� ����!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( vecIndex[RE_HIGH].size() )
					<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
					<< BUILD_LOG( vecIndex[RE_LOW].size() )
					<< END_LOG;
				return false;
			}

			// ������ ��ġ ���� �ΰ�?
			if( iGapTeamRating < iBestOptimalMatchRating )
			{
				iBestOptimalMatchRating = iGapTeamRating;
				vecRedTeam = vecRedTeamTemp;
				vecBlueTeam = vecBlueTeamTemp;
			}

			while( next_permutation( vecIndex[RE_LOW].begin(), vecIndex[RE_LOW].end() ) )
			{
				// �� ���� ����� ��
				int iGapTeamRating = 0;
				std::vector< int > vecIndexResult;
				if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
				{
					START_LOG( cerr, L"�� ������ ���̰� ���ϱ� ����!" )
						<< BUILD_LOG( vecSortedUserList.size() )
						<< BUILD_LOG( vecIndex[RE_HIGH].size() )
						<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
						<< BUILD_LOG( vecIndex[RE_LOW].size() )
						<< END_LOG;
					return false;
				}

				// ������ ��ġ ���� �ΰ�?
				if( iGapTeamRating < iBestOptimalMatchRating )
				{
					iBestOptimalMatchRating = iGapTeamRating;
					vecRedTeam = vecRedTeamTemp;
					vecBlueTeam = vecBlueTeamTemp;
				}
			}
		}
	}

	return true;
}

bool KMatchUserManager::GetTeamRatingGapByRatingLevel( IN const std::vector< UidType >& vecSortedUserList, 
														IN const std::vector< int >& vecHighRating, 
														IN const std::vector< int >& vecMiddleRating, 
														IN const std::vector< int >& vecLowRating,
														OUT int& iGepTeamRating,
														OUT std::vector< UidType >& vecRedTeamResult,
														OUT std::vector< UidType >& vecBlueTeamResult )
{
	iGepTeamRating = 0;
	vecRedTeamResult.clear();
	vecBlueTeamResult.clear();

	std::map< UidType, int > mapRedTeamRating;
	std::map< UidType, int > mapBlueTeamRating;	

	//////////////////////////////////////////////////////////////////////////	
	// high rating ���� ���
	for( u_int ui = 0; ui < vecHighRating.size(); ++ui )
	{
		const u_int iUserIndex = vecHighRating[ui];
		if( iUserIndex < 0  ||  iUserIndex >= vecSortedUserList.size() )
		{
			START_LOG( cerr, L"������ ���� ����Ʈ�� �������� �ʴ� index���̴�! �̰� ������ ���� ���̾�!" )
				<< BUILD_LOG( vecSortedUserList.size() )
				<< BUILD_LOG( iUserIndex )
				<< END_LOG;
			return false;
		}

		const UidType iUnitUID = vecSortedUserList[iUserIndex];
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// �� �������� ���ϱ� ���� ����
		const bool bIsRed = ( ( ui % 2 ) == 0 );
		if( bIsRed )
		{
			mapRedTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecRedTeamResult.push_back( iUnitUID );
		}
		else
		{
			mapBlueTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecBlueTeamResult.push_back( iUnitUID );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// middle rating ���� ���
	for( u_int ui = 0; ui < vecMiddleRating.size(); ++ui )
	{
		const u_int iUserIndex = vecMiddleRating[ui];
		if( iUserIndex < 0  ||  iUserIndex >= vecSortedUserList.size() )
		{
			START_LOG( cerr, L"������ ���� ����Ʈ�� �������� �ʴ� index���̴�! �̰� ������ ���� ���̾�!" )
				<< BUILD_LOG( vecSortedUserList.size() )
				<< BUILD_LOG( iUserIndex )
				<< END_LOG;
			return false;
		}

		const UidType iUnitUID = vecSortedUserList[iUserIndex];
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// �� �������� ���ϱ� ���� ����
		const bool bIsRed = ( ( ui % 2 ) == 0 );
		if( bIsRed )
		{
			mapRedTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecRedTeamResult.push_back( iUnitUID );
		}
		else
		{
			mapBlueTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecBlueTeamResult.push_back( iUnitUID );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// low rating ���� ���
	for( u_int ui = 0; ui < vecLowRating.size(); ++ui )
	{
		const u_int iUserIndex = vecLowRating[ui];
		if( iUserIndex < 0  ||  iUserIndex >= vecSortedUserList.size() )
		{
			START_LOG( cerr, L"������ ���� ����Ʈ�� �������� �ʴ� index���̴�! �̰� ������ ���� ���̾�!" )
				<< BUILD_LOG( vecSortedUserList.size() )
				<< BUILD_LOG( iUserIndex )
				<< END_LOG;
			return false;
		}

		const UidType iUnitUID = vecSortedUserList[iUserIndex];
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// �� �������� ���ϱ� ���� ����
		const bool bIsRed = ( ( ui % 2 ) == 0 );
		if( bIsRed )
		{
			mapRedTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecRedTeamResult.push_back( iUnitUID );
		}
		else
		{
			mapBlueTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecBlueTeamResult.push_back( iUnitUID );
		}
	}

	// �� ���� �������� ������!
	const int iRedTeamRating = SiKPvpMatchManager()->GetRatingByTeam( mapRedTeamRating );
	const int iBlueTeamRating = SiKPvpMatchManager()->GetRatingByTeam( mapBlueTeamRating );

	// �� ������ ���� ���밪!
	iGepTeamRating = abs( iRedTeamRating - iBlueTeamRating );
	return true;
}

//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KMatchUserManager::GetCancelUserList( OUT std::vector< UidType >& vecCancelUserList )
{
	vecCancelUserList.clear();

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;;
		}

		if( mit->second->IsAccepted() == false )
		{
			vecCancelUserList.push_back( mit->first );
		}
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}

#endif SERV_PVP_NEW_SYSTEM
//}}

