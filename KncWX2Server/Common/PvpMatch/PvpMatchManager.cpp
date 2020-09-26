#include ".\PvpMatchManager.h"
#include "NetError.h"
#include "BaseServer.h"
#include "Lottery.h"
#include "CompareLimitList.h"
#include "Enum/Enum.h"
#include "X2Data/XSLUnitManager.h"

//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


ImplementRefreshSingleton( KPvpMatchManager );

KPvpMatchManager::KPvpMatchManager(void) :
m_iMaxRankingPoint( 0 ),
m_iMaxArenaPoint( 0 ),
m_iMatchMakingBeginCnt( 0 ),
m_iCheckWaitUserCount( 0 ),
m_iGapMinMaxRating( 0 ),
m_sDefaultPvpMapID( 0 ),
m_fHeroPvpNpcFactor( 0.f )
{
}

KPvpMatchManager::~KPvpMatchManager(void)
{
}

ImplToStringW( KPvpMatchManager )
{
	stm_	<< L"----------[ Pvp Match Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_iMaxRankingPoint )
			<< TOSTRINGW( m_iMaxArenaPoint )
			<< TOSTRINGW( m_iMatchMakingBeginCnt )
			<< TOSTRINGW( m_iCheckWaitUserCount )
			<< TOSTRINGW( m_iGapMinMaxRating )
			<< TOSTRINGW( m_mapPvpMatchInfo.size() )
			<< TOSTRINGW( m_vecPvpMapInfo.size() )
			<< TOSTRINGW( m_setPvpMapForMatch.size() )
			<< TOSTRINGW( m_sDefaultPvpMapID )
			<< TOSTRINGW( m_arrMatchWeightValue[MWE_LOW_RATING_PLAYER] )
			<< TOSTRINGW( m_arrMatchWeightValue[MWE_MIDDLE_RATING_PLAYER] )
			<< TOSTRINGW( m_arrMatchWeightValue[MWE_HIGH_RATING_PLAYER] )
			<< TOSTRINGW( m_mapPvpNpcInfo.size() )
			<< TOSTRINGW( m_mapPvpNpcName.size() )
			<< TOSTRINGW( m_fHeroPvpNpcFactor )
			;

	return stm_;
}

ImplementLuaScriptParser( KPvpMatchManager )
{
	lua_tinker::class_add<KPvpMatchManager>( GetLuaState(), "KPvpMatchManager" );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMaxRankingPoint",		&KPvpMatchManager::SetMaxRankingPoint_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMaxArenaPoint",			&KPvpMatchManager::SetMaxArenaPoint_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMatchMakingBeginCount",	&KPvpMatchManager::SetMatchMakingBeginCount_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMatchWeightValue",		&KPvpMatchManager::SetMatchWeightValue_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetCheckWaitUserCount",	&KPvpMatchManager::SetCheckWaitUserCount_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetGapMinMaxRating",		&KPvpMatchManager::SetGapMinMaxRating_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpMatchInfo",			&KPvpMatchManager::AddPvpMatchInfo_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpMapForMatch",		&KPvpMatchManager::AddPvpMapForMatch_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpMapForNpcMatch",		&KPvpMatchManager::AddPvpMapForNpcMatch_LUA );	
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpNpcInfo",			&KPvpMatchManager::AddPvpNpcInfo_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpNpcName",			&KPvpMatchManager::AddPvpNpcName_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetHeroPvpNpcFactor",		&KPvpMatchManager::SetHeroPvpNpcFactor_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "dump",						&KPvpMatchManager::Dump );

	lua_tinker::decl( GetLuaState(), "PvpMatchManager", this );
}

void KPvpMatchManager::SetMaxRankingPoint_LUA( int iMaxRankingPoint )
{
	if( iMaxRankingPoint < 0 )
	{
		START_LOG( cerr, L"�ִ� ��ŷ ����Ʈ���� �̻��մϴ�." )
			<< BUILD_LOG( iMaxRankingPoint )
			<< END_LOG;
		return;
	}

	m_iMaxRankingPoint = iMaxRankingPoint;
}

void KPvpMatchManager::SetMaxArenaPoint_LUA( int iMaxArenaPoint )
{
	if( iMaxArenaPoint < 0 )
	{
		START_LOG( cerr, L"�ִ� �Ʒ��� ����Ʈ���� �̻��մϴ�." )
			<< BUILD_LOG( iMaxArenaPoint )
			<< END_LOG;
		return;
	}

	m_iMaxArenaPoint = iMaxArenaPoint;
}

void KPvpMatchManager::SetMatchMakingBeginCount_LUA( int iMatchMakingBeginCount )
{
	if( iMatchMakingBeginCount < 0 )
	{
		START_LOG( cerr, L"��ġ ����ŷ ���� ī��Ʈ�� �̻��մϴ�." )
			<< BUILD_LOG( iMatchMakingBeginCount )
			<< END_LOG;
		return;
	}

	m_iMatchMakingBeginCnt = iMatchMakingBeginCount;
}

void KPvpMatchManager::SetMatchWeightValue_LUA( int iEnum, int iWeightValue )
{
	if( iEnum < 0  ||  iEnum >= MWE_MAX )
	{
		START_LOG( cerr, L"���� ��ġ ����ġ enum�� �̻��մϴ�!" )
			<< BUILD_LOG( iEnum )
			<< END_LOG;
		return;
	}

	m_arrMatchWeightValue[iEnum] = iWeightValue;
}

void KPvpMatchManager::SetCheckWaitUserCount_LUA( int iCheckCount )
{
	if( iCheckCount < 0 )
	{
		START_LOG( cerr, L"��ġ ������ �̻��մϴ�." )
			<< BUILD_LOG( iCheckCount )
			<< END_LOG;
		return;
	}

	m_iCheckWaitUserCount = iCheckCount;
}

void KPvpMatchManager::SetGapMinMaxRating_LUA( int iGapMinMaxRating )
{
	if( iGapMinMaxRating < 0 )
	{
		START_LOG( cerr, L"��ġ ������ �̻��մϴ�." )
			<< BUILD_LOG( iGapMinMaxRating )
			<< END_LOG;
		return;
	}

	m_iGapMinMaxRating = iGapMinMaxRating;    
}

bool KPvpMatchManager::AddPvpMatchInfo_LUA( int iMatchType, int iKillNum, float fPlayTime )
{
	if( iMatchType < 0  ||  iKillNum < 0  ||  fPlayTime < 0.f )
	{
		START_LOG( cerr, L"��ġ ������ �̻��մϴ�." )
			<< BUILD_LOG( iMatchType )
			<< BUILD_LOG( iKillNum )
			<< BUILD_LOG( fPlayTime )
			<< END_LOG;
		return false;
	}

	SPvpMatchInfo kInfo;
	kInfo.m_iKillNum = iKillNum;
	kInfo.m_fPlayTime = fPlayTime;
	m_mapPvpMatchInfo.insert( std::make_pair( iMatchType, kInfo ) );
	return true;
}

bool KPvpMatchManager::AddPvpMapForMatch_LUA( int iAddMapID )
{
	if( IsExistMapID( iAddMapID ) == true )
	{
		START_LOG( cerr, L"������ ���� �� �ֳ�?" )
			<< BUILD_LOG( iAddMapID )
			<< END_LOG;
		return false;
	}

	// ���� ó�� ����ϴ� �������� ����Ʈ ��ID�� �ȴ�.
	if( m_sDefaultPvpMapID == 0 )
	{
		m_sDefaultPvpMapID = iAddMapID;
	}
	
	m_vecPvpMapInfo.push_back( iAddMapID );
	m_setPvpMapForMatch.insert( iAddMapID );
	return true;
}

bool KPvpMatchManager::AddPvpMapForNpcMatch_LUA( int iAddMapID )
{
	if( IsExistMapID( iAddMapID ) == false )
	{
		START_LOG( cerr, L"��ϵ��� ���� ���� �ʺ��ڿ� ������ ����Ϸ��� �ϳ�?" )
			<< BUILD_LOG( iAddMapID )
			<< END_LOG;
		return false;
	}

	m_kLotPvpMapForNpcMatch.AddCaseSameProb( iAddMapID );
	return true;
}

void KPvpMatchManager::AddPvpNpcInfo_LUA()
{
    KLuaManager luaManager( GetLuaState() );

	int iNpcID = 0;
	SPvpNpcInfo sPvpNpcInfo;
	LUA_GET_VALUE_RETURN(		luaManager, L"m_PvpNpcID",			iNpcID,									0,	goto end_proc );

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_PvpNpcType",		sPvpNpcInfo.m_ePvpNpcType,	SEnum::PVP_NPC_TYPE, 	SEnum::PNT_NONE, goto end_proc );

	int iUnitClass = 0;
	LUA_GET_VALUE_RETURN(		luaManager, L"m_UnitClass",			iUnitClass,								0,	goto end_proc );
	sPvpNpcInfo.m_cUnitClass = static_cast<char>(iUnitClass);

	int iLevel = 0;
	LUA_GET_VALUE(				luaManager, L"m_Level",				iLevel,									0 );
	sPvpNpcInfo.m_ucLevel = static_cast<u_char>(iLevel);

	LUA_GET_VALUE(				luaManager, L"m_CharName",			sPvpNpcInfo.m_wstrCharName,				L"" );

	//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	int iRank;
	LUA_GET_VALUE_RETURN(		luaManager, L"m_Ranking",			iRank,					0,	goto end_proc );
	sPvpNpcInfo.m_cRank = static_cast<char>( iRank );
#endif SERV_2012_PVP_SEASON2
	//}}

	LUA_GET_VALUE_RETURN(		luaManager, L"m_Rating",			sPvpNpcInfo.m_Rating,					0,	goto end_proc );

	LUA_GET_VALUE(				luaManager, L"m_RewardTitleID",		sPvpNpcInfo.m_iRewardTitleID,			0 );
	LUA_GET_VALUE(				luaManager, L"m_RewardTitle",		sPvpNpcInfo.m_iRewardTitlePeriod,		0 );

	// ���� NPC���� ����
	m_mapPvpNpcInfo.insert( std::make_pair( iNpcID, sPvpNpcInfo ) );
	
	if( sPvpNpcInfo.m_ePvpNpcType < 0  ||  sPvpNpcInfo.m_ePvpNpcType >= SEnum::PNT_MAX )
		goto end_proc;

	// ���� �׷쿡�� ����!
	m_kLotPvpNpc[sPvpNpcInfo.m_ePvpNpcType].AddCaseSameProb( iNpcID );
	return;

end_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( iNpcID )
		<< BUILD_LOG( iUnitClass )
		<< BUILD_LOG( sPvpNpcInfo.m_Rating )
		<< BUILD_LOG( sPvpNpcInfo.m_ePvpNpcType )
		<< END_LOG;
}

void KPvpMatchManager::AddPvpNpcName_LUA( const char* pName )
{
	// ���� NPC�̸�
	std::wstring wstrPvpNpcName = KncUtil::toWideString_UTF8( std::string( pName ) );

	// ���� NPC�̸� �ε���
	const int iIndex = static_cast<int>(m_mapPvpNpcName.size());

	// ������ ����!
	m_mapPvpNpcName.insert( std::make_pair( iIndex, wstrPvpNpcName ) );
	m_vecPvpNpcName.push_back( wstrPvpNpcName );
	m_kLotPvpNpcName.AddCaseSameProb( iIndex );
}

void KPvpMatchManager::SetHeroPvpNpcFactor_LUA( float fFactor )
{
	if( fFactor < 0.0 )
	{
		START_LOG( cerr, L"��� ������ �̻��մϴ�." )
			<< BUILD_LOG( fFactor )
			<< END_LOG;
		return;
	}

	m_fHeroPvpNpcFactor = fFactor;
}

bool KPvpMatchManager::GetPvpMatchInfo( IN const int iMatchType, OUT char& cWinKillNum, OUT float& fPlaytime ) const
{
	cWinKillNum = 0;
	fPlaytime = 0;

	std::map< int, SPvpMatchInfo >::const_iterator mit;
	mit = m_mapPvpMatchInfo.find( iMatchType );
	if( mit == m_mapPvpMatchInfo.end() )
	{
        START_LOG( cerr, L"�ش� ��ġ Ÿ�Կ� ���� ������ �����ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iMatchType )
			<< END_LOG;
		return false;
	}

	cWinKillNum = static_cast<char>(mit->second.m_iKillNum);
	fPlaytime = mit->second.m_fPlayTime;
	return true;
}

bool KPvpMatchManager::DecisionPvpMap( IN const bool bIsNpcPvpMode, IN const std::map< short, int >& mapAllPlayersSelectedMap, OUT short& sMapID ) const
{
	sMapID = 0;
	std::map< short, int > mapSelectedMap = mapAllPlayersSelectedMap;
	KLotterySameProb< int > kPvpMapRandom;

	if( bIsNpcPvpMode )
	{
		//////////////////////////////////////////////////////////////////////////
		// �ʺ��� �����ʿ� ���� ó��

		// �������� ��������!
		sMapID = m_kLotPvpMapForNpcMatch.DecisionSameProb();
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// �Ϲ� �����ʿ� ���� ó��

		BOOST_TEST_FOREACH( const int, iMapID, m_vecPvpMapInfo )
		{
			// �������� ������ �ʵ��� �����Ѵ�!
			if( mapSelectedMap.find( iMapID ) != mapSelectedMap.end() )
				continue;

			// �ϳ��� ����!
			kPvpMapRandom.AddCaseSameProb( iMapID );
		}

		// �ٵ� ���࿡ ����ִٸ�?
		if( kPvpMapRandom.Empty() == true )
		{
			int iMaxSelectedMapID = 0;
			int iMaxSelectedCount = 0;
			std::map< short, int >::const_iterator mit;
			for( mit = mapSelectedMap.begin(); mit != mapSelectedMap.end(); ++mit )
			{
				if( iMaxSelectedCount < mit->second )
				{
					iMaxSelectedCount = mit->second;
					iMaxSelectedMapID = static_cast<int>(mit->first);
				}
			}

			// ���� ���� ������ ���� ���� �����Ѵ�!
			mapSelectedMap.erase( iMaxSelectedMapID );

			// ���� �ʵ�� ���� ������ ������!
			for( mit = mapSelectedMap.begin(); mit != mapSelectedMap.end(); ++mit )
			{
				if( IsExistMapID( mit->first ) == false )
				{
					START_LOG( cerr, L"��? �������� �ʴ� MapID�� �ֳ�? �Ͼ���� �ȵǴ� ����!" )
						<< BUILD_LOG( mit->first )
						<< END_LOG;
					continue;
				}

				kPvpMapRandom.AddCaseSameProb( mit->first );
			}

			if( kPvpMapRandom.Empty() == true )
			{
				START_LOG( cerr, L"�׷��� �� ���� ������ ����ֳ�? ���� �ڵ� �����ų� ��ȹ�� �߸��Ȱɰž�! �Ͼ���� �ȵǴ� ������!" )
					<< BUILD_LOG( m_vecPvpMapInfo.size() )
					<< BUILD_LOG( mapSelectedMap.size() )
					<< END_LOG;
				return false;
			}
		}

		// �������� ��������!
		sMapID = static_cast< short >(kPvpMapRandom.DecisionSameProb());
	}	
	
	if( IsExistMapID( sMapID ) == false )
	{
		START_LOG( cerr, L"�������� ������ �������� ��ϵ� �������� �ƴϳ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( sMapID )
			<< END_LOG;

		sMapID = m_sDefaultPvpMapID;
	}	

	return true;
}

int	KPvpMatchManager::GetMatchWeightValue( IN const MATCH_WEIGHT_ENUM eEnum ) const
{
	if( eEnum < 0  ||  eEnum >= MWE_MAX )
	{
		START_LOG( cerr, L"���� ��ġ ����ġ enum�� �̻��մϴ�!" )
			<< BUILD_LOG( eEnum )
			<< END_LOG;
		return -1;
	}

	return m_arrMatchWeightValue[eEnum];
}

int KPvpMatchManager::GetRatingByTeam( IN const std::map< UidType, int >& mapTeamMemberInfo ) const
{
	if( mapTeamMemberInfo.empty()  ||  mapTeamMemberInfo.size() > SEnum::PE_MAX_MATCH_PLAYER_COUNT )
	{
		START_LOG( cerr, L"�� �������� ����Ϸ��µ� �ο����� �̻��ϴ�!" )
			<< BUILD_LOG( mapTeamMemberInfo.size() )
			<< END_LOG;
		return -1;
	}

	KCompareLimitList< SUserRatingInfo > cllSortedUserList;

	std::map< UidType, int >::const_iterator mit;
	for( mit = mapTeamMemberInfo.begin(); mit != mapTeamMemberInfo.end(); ++mit )
	{
		const UidType iUnitUID = mit->first;
		const int iRating = mit->second;

		// rating���� ū������ �����ؼ� ����Ʈ�� ������!
		cllSortedUserList.IfUpperValuePushBack( iRating, SUserRatingInfo( iUnitUID, iRating ) );
	}

	std::vector< SUserRatingInfo > vecSortedUserList;
	cllSortedUserList.GetValueList( vecSortedUserList );

	int iTotalRatingWithWeightValue = 0;
	int iTotalWeightValue = 0;

	for( u_int uiIndex = 0; uiIndex < vecSortedUserList.size(); ++uiIndex )
	{
		const SUserRatingInfo& sUserInfo = vecSortedUserList[uiIndex];

		// Rating������ ������ ����ġ ���� ��´�.
		const int iMatchWeightValue = GetMatchWeightValue( (KPvpMatchManager::MATCH_WEIGHT_ENUM)uiIndex );

		// �ش� ������ Rating���� ����ġ���� ���� ���� ���Ѵ�.
		iTotalRatingWithWeightValue += ( sUserInfo.m_iRating * iMatchWeightValue );

		// ����ġ���� ������ ���Ѵ�.
		iTotalWeightValue += iMatchWeightValue;
	}

	// ����ġ ������ 0�� ���� ����� �Ͼ���� 
	if( iTotalWeightValue <= 0 )
	{
		START_LOG( cerr, L"����ġ ������ �̻��ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( mapTeamMemberInfo.size() )
			<< BUILD_LOG( iTotalWeightValue )
			<< END_LOG;
		return -1;
	}

	const int iResultTeamRating = iTotalRatingWithWeightValue / iTotalWeightValue;
	return iResultTeamRating;
}

bool KPvpMatchManager::DecisionNpcPvpMode( IN const std::vector< KRoomUserInfo >& vecTeamMember ) const
{
	static int NPC_PVP_MODE = 1;
	std::map< UidType, int > mapTeamMemberRating;

	// �� ����� Rating�� ��������!
	BOOST_TEST_FOREACH( const KRoomUserInfo&, kInfo, vecTeamMember )
	{
		mapTeamMemberRating.insert( std::make_pair( kInfo.m_nUnitUID, kInfo.m_iRating ) );
	}

	// �� �������� ������!
	const int TEAM_RATING = GetRatingByTeam( mapTeamMemberRating );
	float fNpcPvpModeRate = static_cast<float>(TEAM_RATING) * 0.001f * 0.01f * GetHeroPvpNpcFactor();
	if( fNpcPvpModeRate > 100.f )
	{
		fNpcPvpModeRate = 100.f;
	}

	//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	if( vecTeamMember.size() == 3 )
	{
		fNpcPvpModeRate *= 0.5f;
	}
#endif SERV_2012_PVP_SEASON2
	//}}

	// ���� ������ ��������!
	KLottery kLot;
	kLot.AddCase( NPC_PVP_MODE, fNpcPvpModeRate );

	const bool bRet = ( kLot.Decision() == NPC_PVP_MODE );
	return bRet;
}

bool KPvpMatchManager::IsExistPvpNpcName( IN const std::wstring& wstrNickName ) const
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrCharName, m_vecPvpNpcName )
	{
        if( wstrCharName == wstrNickName )
			return true;
	}

	return false;
}

//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
bool KPvpMatchManager::GetPvpNpcInfo( IN const bool bIsAllPlayersBeginner, IN const int iMyTeamRatingWeightAverage, IN const u_int uiPvpPlayerCount, OUT std::vector< KRoomUserInfo >& vecPvpNpcTeam, OUT std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo ) const
{
	vecPvpNpcTeam.clear();
	mapPvpNpcInfo.clear();

	// PVP NPC UID���� -10000���� �����մϴ�.
	UidType iPvpNpcUnitUID = -10000;

	std::multimap<int, int> mapNpcListOrderedByRatingGab;
	KLotterySameProb< int > kLotPvpNpc;
	if( bIsAllPlayersBeginner )
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_BEGINNER_NPC];
	}
	else
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_HERO_NPC];
	}

	BOOST_TEST_FOREACH( int, iPvpNpcID, kLotPvpNpc.m_vecCase )
	{
		// ���� �����ϴ� �༮���� üũ!
		const SPvpNpcInfo* pPvpNpcInfo = GetPvpNpcInfo( iPvpNpcID );
		if( pPvpNpcInfo == NULL )
		{
			START_LOG( cerr, L"���� NPC������ �����ϴ�!" )
				<< BUILD_LOG( bIsAllPlayersBeginner )
				<< BUILD_LOG( iPvpNpcID )
				<< END_LOG;
			return false;
		}

		// Rating Gab�� ���ؼ� npc ����Ʈ �����Ѵ�.
		mapNpcListOrderedByRatingGab.insert( std::multimap<int, int>::value_type( abs( iMyTeamRatingWeightAverage - pPvpNpcInfo->m_Rating ), iPvpNpcID ) );
	}

	// kLotPvpNpc�� �ٽ� ��´�.
	kLotPvpNpc.Clear();
	if( mapNpcListOrderedByRatingGab.empty() != true )
	{
		LIF( mapNpcListOrderedByRatingGab.size() < 100 ); 

		std::multimap<int, int>::iterator it;
		int iMultiple = 32;
		for( it = mapNpcListOrderedByRatingGab.begin(); it != mapNpcListOrderedByRatingGab.end(); ++it )
		{
			int iMultipleCopy = iMultiple;
			// 32���� �����ؼ� 
			while( 0 <= --iMultipleCopy )
			{
				kLotPvpNpc.AddCaseSameProb( it->second );
			}

			// 2���� 1�� �����Ѵ�.
			if( 1 < iMultiple )
			{
				iMultiple >>= 1;
			}
		}
	}

	for( u_int ui = 0; ui < uiPvpPlayerCount; ++ui )
	{
		// �����ϰ� ������ NPC�� ������!
		const int iPvpNpcID = kLotPvpNpc.DecisionSameProb();

		// ���� �����ϴ� �༮���� üũ!
		const SPvpNpcInfo* pPvpNpcInfo = GetPvpNpcInfo( iPvpNpcID );
		if( pPvpNpcInfo == NULL )
		{
			START_LOG( cerr, L"���� NPC������ �����ϴ�!" )
				<< BUILD_LOG( bIsAllPlayersBeginner )
				<< BUILD_LOG( iPvpNpcID )
				<< END_LOG;
			return false;
		}

		//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		// ������ ������ NPC�� �ߺ����� �ʵ��� �����صд�.
		kLotPvpNpc.EraseAll( iPvpNpcID );
#else
		// ������ ������ NPC�� �ߺ����� �ʵ��� �����صд�.
		kLotPvpNpc.Erase( iPvpNpcID );
#endif SERV_2012_PVP_SEASON2
		//}}

		// RoomUserInfo�� ���� ��������!
		KRoomUserInfo kInfo;
		kInfo.m_bIsPvpNpc			= true;
		kInfo.m_iGSUID				= -1;
		kInfo.m_iOwnerUserUID		= -1;
		kInfo.m_nUnitUID			= iPvpNpcUnitUID;
		kInfo.m_cUnitClass			= pPvpNpcInfo->m_cUnitClass;
		if( pPvpNpcInfo->m_wstrCharName.empty() )
		{
			GetPvpNpcNameByRandom( kInfo.m_wstrNickName );
		}
		else
		{
			kInfo.m_wstrNickName = pPvpNpcInfo->m_wstrCharName;
		}
		kInfo.m_ucLevel				= pPvpNpcInfo->m_ucLevel;
		kInfo.m_kGameStat.m_iBaseHP = 30000; // RoomUserManager���� Enter�Ҷ� ������ �ȳ��� ���� �⺻HP���� 30000�� �־�д�.
		//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		kInfo.m_cRank				= pPvpNpcInfo->m_cRank;
#endif SERV_2012_PVP_SEASON2
		//}}
		kInfo.m_iRating				= pPvpNpcInfo->m_Rating;
		kInfo.m_iMaxRating			= kInfo.m_iRating;
		vecPvpNpcTeam.push_back( kInfo );

		// ���� NPC������ ��������!
		KPvpNpcInfo kNpcInfo;
		kNpcInfo.m_iNpcID = iPvpNpcID;
		mapPvpNpcInfo.insert( std::make_pair( kInfo.m_nUnitUID, kNpcInfo ) );

		// ���� NpcUID�� �߱�!
		--iPvpNpcUnitUID;
	}

	return true;
}
#else
bool KPvpMatchManager::GetPvpNpcInfo( IN const bool bIsAllPlayersBeginner, IN const u_int uiPvpPlayerCount, OUT std::vector< KRoomUserInfo >& vecPvpNpcTeam, OUT std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo ) const
{
	vecPvpNpcTeam.clear();
	mapPvpNpcInfo.clear();

	// PVP NPC UID���� -10000���� �����մϴ�.
	UidType iPvpNpcUnitUID = -10000;

#ifdef SERV_NOT_2012_PVP_SEASON2_BUILD_ERROR_FIX
	KLotterySameProb< int > kLotPvpNpc;
#else // SERV_NOT_2012_PVP_SEASON2_BUILD_ERROR_FIX
	KLotterySameProb kLotPvpNpc;
#endif // SERV_NOT_2012_PVP_SEASON2_BUILD_ERROR_FIX
	if( bIsAllPlayersBeginner )
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_BEGINNER_NPC];
	}
	else
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_HERO_NPC];
	}

	for( u_int ui = 0; ui < uiPvpPlayerCount; ++ui )
	{
		// �����ϰ� ������ NPC�� ������!
		const int iPvpNpcID = kLotPvpNpc.DecisionSameProb();

		// ���� �����ϴ� �༮���� üũ!
		const SPvpNpcInfo* pPvpNpcInfo = GetPvpNpcInfo( iPvpNpcID );
		if( pPvpNpcInfo == NULL )
		{
			START_LOG( cerr, L"���� NPC������ �����ϴ�!" )
				<< BUILD_LOG( bIsAllPlayersBeginner )
				<< BUILD_LOG( iPvpNpcID )
				<< END_LOG;
			return false;
		}

		// ������ ������ NPC�� �ߺ����� �ʵ��� �����صд�.
		kLotPvpNpc.Erase( iPvpNpcID );

		// RoomUserInfo�� ���� ��������!
		KRoomUserInfo kInfo;
		kInfo.m_bIsPvpNpc			= true;
		kInfo.m_iGSUID				= -1;
		kInfo.m_iOwnerUserUID		= -1;
		kInfo.m_nUnitUID			= iPvpNpcUnitUID;
		kInfo.m_cUnitClass			= pPvpNpcInfo->m_cUnitClass;
		if( pPvpNpcInfo->m_wstrCharName.empty() )
		{
			GetPvpNpcNameByRandom( kInfo.m_wstrNickName );
		}
		else
		{
            kInfo.m_wstrNickName = pPvpNpcInfo->m_wstrCharName;
		}
		kInfo.m_ucLevel				= pPvpNpcInfo->m_ucLevel;
		kInfo.m_kGameStat.m_iBaseHP = 30000; // RoomUserManager���� Enter�Ҷ� ������ �ȳ��� ���� �⺻HP���� 30000�� �־�д�.
		kInfo.m_iRating				= pPvpNpcInfo->m_Rating;
		kInfo.m_iMaxRating			= kInfo.m_iRating;
		vecPvpNpcTeam.push_back( kInfo );

		// ���� NPC������ ��������!
		KPvpNpcInfo kNpcInfo;
		kNpcInfo.m_iNpcID = iPvpNpcID;
		mapPvpNpcInfo.insert( std::make_pair( kInfo.m_nUnitUID, kNpcInfo ) );

		// ���� NpcUID�� �߱�!
		--iPvpNpcUnitUID;
	}

	return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

const KPvpMatchManager::SPvpNpcInfo* KPvpMatchManager::GetPvpNpcInfo( IN const int iNpcID ) const
{
	std::map< int, SPvpNpcInfo >::const_iterator mit;
	mit = m_mapPvpNpcInfo.find( iNpcID );
	if( mit == m_mapPvpNpcInfo.end() )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ���� NPC ���� �Դϴ�!" )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

void KPvpMatchManager::GetPvpNpcNameByRandom( OUT std::wstring& wstrName ) const
{
    const int iNameIndex = m_kLotPvpNpcName.DecisionSameProb();
	std::map< int, std::wstring >::const_iterator mit;
	mit = m_mapPvpNpcName.find( iNameIndex );
	if( mit == m_mapPvpNpcName.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� index��? ��ũ��Ʈ �Ľ��� �߸��Ǿ���?" )
			<< BUILD_LOG( iNameIndex )
			<< END_LOG;
		return;
	}

	wstrName = mit->second;
}
#endif SERV_PVP_NEW_SYSTEM
//}}
