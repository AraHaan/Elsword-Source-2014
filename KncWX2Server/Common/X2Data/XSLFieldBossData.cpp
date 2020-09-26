#include "XSLFieldBossData.h"

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-14	// �ڼ���
#include "Enum/Enum.h"



ImplementRefreshSingleton( CXSLFieldBossData );

CXSLFieldBossData::CXSLFieldBossData( void )
	: m_iFieldBossCoolTime_M( 0 )
	, m_iPortalHoldingTime_M( 0 )
	, m_iFieldHoldingTime_M( 0 )
	, m_fLimiteContribution( 0.0f )
	, m_fMaximumContribution( 0.0f )
	, m_byteTotalDangerousValue( 0 )
	, m_byteEndGameFieldHoldingTime_S( 0 )
	, m_byteEndGameFieldHoldingTimeForce_S( 0 )
	, m_byteIntrudeRestrictionTime_S( 0 )
	, m_byteMaximumExpandedSocketCount( 0 )
{
}

CXSLFieldBossData::~CXSLFieldBossData( void )
{
}

ImplToStringW( CXSLFieldBossData )
{
	stm_	<< L"----------[ Field Boss Manager ]----------" << std::endl
		<< TOSTRINGW( m_iFieldBossCoolTime_M )
		<< TOSTRINGW( m_iPortalHoldingTime_M )
		<< TOSTRINGW( m_iFieldHoldingTime_M )
		<< TOSTRINGW( m_fLimiteContribution )
		<< TOSTRINGW( m_fMaximumContribution )
		<< TOSTRINGW( m_mapRewardData.size() )
		<< TOSTRINGW( m_byteTotalDangerousValue )
		<< TOSTRINGW( m_byteEndGameFieldHoldingTime_S )
		<< TOSTRINGW( m_byteEndGameFieldHoldingTimeForce_S )
		<< TOSTRINGW( m_byteIntrudeRestrictionTime_S )
		<< TOSTRINGW( m_setPortalLocation.size() )
		<< TOSTRINGW( m_byteMaximumExpandedSocketCount )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLFieldBossData )
{
	lua_tinker::class_add<CXSLFieldBossData>( GetLuaState(), "CXSLFieldBossData" );
	lua_tinker::class_def<CXSLFieldBossData>( GetLuaState(), "dump",	&CXSLFieldBossData::Dump );

	lua_tinker::class_def<CXSLFieldBossData>( GetLuaState(), "SetData",	&CXSLFieldBossData::SetData_LUA );

	lua_tinker::decl( GetLuaState(), "FieldBossData", this );
}

void CXSLFieldBossData::SetData_LUA( void )
{
	KLuaManager luaManager( GetLuaState() );

	int iTemp;

	LUA_GET_VALUE(	luaManager,	L"TotalDangerousValue",	iTemp,	4	);
	if( _SetData( m_byteTotalDangerousValue, 0, iTemp, 255 ) == false )
		return;

	LUA_GET_VALUE(	luaManager,	L"FieldBossCoolTime_M",	iTemp,	60	);
	if( _SetData( m_iFieldBossCoolTime_M, 0, iTemp, iTemp ) == false )
		return;

	LUA_GET_VALUE(	luaManager,	L"PortalHoldingTime_M",	iTemp,	6	);
	if( _SetData( m_iPortalHoldingTime_M, 0, iTemp, iTemp ) == false )
		return;

	LUA_GET_VALUE(	luaManager,	L"FieldHoldingTime_M",	iTemp,	6	);
	if( _SetData( m_iFieldHoldingTime_M, 0, iTemp, iTemp ) == false )
		return;

	LUA_GET_VALUE(	luaManager,	L"EndGameFieldHoldingTime_S",	iTemp,	60	);
	if( _SetData( m_byteEndGameFieldHoldingTime_S, 0, iTemp, 255 ) == false )
		return;

	LUA_GET_VALUE(	luaManager,	L"EndGameFieldHoldingTimeForce_S",	iTemp,	60	);
	if( _SetData( m_byteEndGameFieldHoldingTimeForce_S, 0, iTemp, 255 ) == false )
		return;

	LUA_GET_VALUE(	luaManager,	L"IntrudeRestrictionTime_S",	iTemp,	20	);
	if( _SetData( m_byteIntrudeRestrictionTime_S, 0, iTemp, 255 ) == false )
		return;

	// ���� ���� �߰��� �ʿ��� ����
	LUA_GET_VALUE(	luaManager,	L"MaximumExpandedSocketCount",	iTemp,	0	);
	if( _SetData( m_byteMaximumExpandedSocketCount, 0, iTemp, 1 ) == false )	// DB�� �ڵ尡 ������ �þ�� �ʴ� �ٴ� ���� �Ͽ� ¥���� ������ �ſ� ������ ��.
		return;

	if( luaManager.BeginTable( L"SocketExpandItemForWeapon" ) == S_OK )
	{
		int iSocketExpandItemForWeapon = 0;
		for( int i = 1; luaManager.GetValue( i, iSocketExpandItemForWeapon ) == S_OK; ++i )
		{
			if( iSocketExpandItemForWeapon <= 0 )
			{
				START_LOG( cerr, L"�߸��� ���� ���� ���� �Ǿ����ϴ�.")
					<< BUILD_LOG( iSocketExpandItemForWeapon )
					<< END_LOG;
				continue;
			}

			if( m_setSocketExpandItemForWeapon.insert( iSocketExpandItemForWeapon ).second == false )
			{
				START_LOG( cerr, L"�̹� �����Ǿ� �ִ� ���Դϴ�.")
					<< BUILD_LOG( iSocketExpandItemForWeapon )
					<< END_LOG;
				continue;
			}
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"SocketExpandItemForDefence" ) == S_OK )
	{
		int iSocketExpandItemForDefence = 0;
		for( int i = 1; luaManager.GetValue( i, iSocketExpandItemForDefence ) == S_OK; ++i )
		{
			if( iSocketExpandItemForDefence <= 0 )
			{
				START_LOG( cerr, L"�߸��� ���� ���� ���� �Ǿ����ϴ�.")
					<< BUILD_LOG( iSocketExpandItemForDefence )
					<< END_LOG;
				continue;
			}

			if( m_setSocketExpandItemForDefence.insert( iSocketExpandItemForDefence ).second == false )
			{
				START_LOG( cerr, L"�̹� �����Ǿ� �ִ� ���Դϴ�.")
					<< BUILD_LOG( iSocketExpandItemForDefence )
					<< END_LOG;
				continue;
			}
		}
		luaManager.EndTable();
	}

	// ���� ������ ����
	if( _PrecedenceAndRewardDataParsing( luaManager ) == false )
	{
		START_LOG( cerr, L"FieldBossData.lua ���� ���� �б⿡ �����Ͽ����ϴ�." )
			<< END_LOG;
	}

	// ��Ż�� ���� �� �ִ� �ʵ� ����
	if( luaManager.BeginTable( L"PortalLocation" ) == S_OK )
	{
		int iLastestCorrectedVillageMapID = 0;	// �Ľ� ������ �߻��� ������ �� �� ã�� ���� �ϱ� ���� �뵵
		int iVillageMapID;

		for( int i = 1; luaManager.GetValue( i, iVillageMapID ) == S_OK; ++i )
		{
			if( ( iVillageMapID <= SEnum::VMI_INVALID ) || ( SEnum::VMI_BATTLE_FIELD_END <= iVillageMapID ) )
			{
				START_LOG( cerr, L"�߸��� ���� ���� ���� �Ǿ����ϴ�.")
					<< BUILD_LOG( iLastestCorrectedVillageMapID )
					<< BUILD_LOG( iVillageMapID )
					<< END_LOG;
				continue;
			}

			if( m_setPortalLocation.insert( iVillageMapID ).second == false )
			{
				START_LOG( cerr, L"�̹� �����Ǿ� �ִ� ���Դϴ�.")
					<< BUILD_LOG( iLastestCorrectedVillageMapID )
					<< BUILD_LOG( iVillageMapID )
					<< END_LOG;
				continue;
			}

			iLastestCorrectedVillageMapID = iVillageMapID;
		}
		luaManager.EndTable();
	}

	_CheckData();
}

void CXSLFieldBossData::_CheckData( void ) const
{
	if( m_iFieldBossCoolTime_M <= m_iPortalHoldingTime_M )
	{
		START_LOG( cerr, L"��Ż�� ���� �ð��� �ʵ� ������ �� ���� ���� �ð����� �۾ƾ� �մϴ�.")
			<< BUILD_LOG( m_iPortalHoldingTime_M )
			<< BUILD_LOG( m_iFieldBossCoolTime_M )
			<< END_LOG;
	}
}

bool CXSLFieldBossData::_PrecedenceAndRewardDataParsing( IN KLuaManager& luaManager )
{
	if( luaManager.BeginTable( L"Reward" ) != S_OK )
		return false;
		
	LUA_GET_VALUE(	luaManager,	L"LimiteContribution",	m_fLimiteContribution,	0.0f	);
	if( _SetData( m_fLimiteContribution, 0.0f, m_fLimiteContribution, 1.0f ) == false )
	{
		luaManager.EndTable();
		return false;
	}

	LUA_GET_VALUE(	luaManager,	L"MaximumContribution",	m_fMaximumContribution,	0.0f	);
	if( _SetData( m_fMaximumContribution, 0.0f, m_fMaximumContribution, 1.5f ) == false )
	{
		luaManager.EndTable();
		return false;
	}

	bool bResult = true;

	int nTableIndex = 1;
	while( ( bResult == true ) && ( luaManager.BeginTable( nTableIndex ) == S_OK ) )
	{
		float fPrecedenceRate;
		LUA_GET_VALUE(	luaManager,	L"PrecedenceRate",	fPrecedenceRate,	0.0f	);
		if( _SetData( fPrecedenceRate, 0.0f, fPrecedenceRate, 1.0f ) == false )
		{
			luaManager.EndTable();
			bResult = false;
			break;
		}

		std::pair<TYPE_REWARD_DATA::iterator, bool> pairResult = m_mapRewardData.insert( TYPE_REWARD_DATA::value_type( fPrecedenceRate, std::map<int, byte>() ) );
		if( pairResult.second == false )
		{
			luaManager.EndTable();
			bResult = false;
			break;
		}

		if( _RewardParsing( luaManager, pairResult.first->second ) == false )
		{
			luaManager.EndTable();
			bResult = false;
			break;
		}

		luaManager.EndTable();
		++nTableIndex;
	}

	luaManager.EndTable();

	if( bResult == false )
	{
		m_mapRewardData.clear();
	}

	return bResult;
}

bool CXSLFieldBossData::_RewardParsing( IN KLuaManager& luaManager, OUT std::map<int, byte>& mapReward ) const
{
	if( luaManager.BeginTable( L"RewardInfo" ) != S_OK )
		return false;

	bool bResult = true;

	int nTableIndex = 1;
	while( luaManager.BeginTable( nTableIndex ) == S_OK )
	{
		int iItemID;
		LUA_GET_VALUE(	luaManager,	1,	iItemID,	0	);

		int iTemp;
		byte byteNum;
		LUA_GET_VALUE(	luaManager,	2,	iTemp,	0	);
		if( _SetData( byteNum, 0, iTemp, 255 ) == false )
		{
			luaManager.EndTable();
			bResult = false;
			break;
		}

		if( mapReward.insert( std::map<int, byte>::value_type( iItemID, byteNum ) ).second == false )
		{
			START_LOG( cerr, L"RewardInfo�� �ߺ��Ǵ� iItemID�� �����Ǿ� �ֽ��ϴ�." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			luaManager.EndTable();
			bResult = false;
			break;
		}

		luaManager.EndTable();
		++nTableIndex;
	}

	luaManager.EndTable();

	if( bResult == false )
	{
		mapReward.clear();
	}

	return bResult;
}

bool CXSLFieldBossData::GetRandomPortalLocationID( OUT int& iPortalLocationID ) const
{
	if( m_setPortalLocation.empty() == true )
	{
		return false;
	}

	std::set<int>::const_iterator it = m_setPortalLocation.begin();
	for( int i = ( rand() % static_cast<int>( m_setPortalLocation.size() ) ); 0 < i; --i )
		++it;

	iPortalLocationID = *it;

	return true;
}

void CXSLFieldBossData::GetPrecedenceRewardData( IN const size_t iRank, IN const size_t iWholeSize, OUT std::map<int, byte>& mapRewardData ) const
{
	for( TYPE_REWARD_DATA::const_iterator it = m_mapRewardData.begin(); it != m_mapRewardData.end(); ++it )
	{
		const size_t iDatumPoint = static_cast<size_t>( ceil( iWholeSize * it->first ) );

		if( iDatumPoint < iRank )
			continue;
		
		mapRewardData = it->second;
		return;
	}
}
#endif // SERV_BATTLE_FIELD_BOSS
