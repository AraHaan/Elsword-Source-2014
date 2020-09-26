#include "XSLBattleFieldManager.h"
#include "Enum/Enum.h"


ImplementRefreshSingleton( CXSLBattleFieldManager );

CXSLBattleFieldManager::CXSLBattleFieldManager() :
m_iMonsterRespawnFactorByUserCount( DE_MONSTER_RESPAWN_FACTOR_BY_USER_COUNT ),
m_iMonsterRespawnTimeMin( DE_MONSTER_RESPAWN_TIME_MIN ),
m_iMonsterRespawnTimeMax( DE_MONSTER_RESPAWN_TIME_MAX ),
//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
m_fBattleFieldEDFactor( 1.f ),
m_fBattleFieldEXPFactor( 1.f ),
#else
m_fBattleFieldFactor( 1.f ),
#endif SERV_FIELD_ED_EXP_FACTOR
//}
m_iDangerousValueMax( 0 ),
m_iDangerousValueWarning( 0 ),
m_iBossCheckUserCount( 0 ),
m_iEliteMonsterDropValue( 0 )
{
	//{{ 2013. 03. 07	��Ʋ �ʵ� ���赵 �̺�Ʈ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	m_iDangerousValueEventRate = 0;
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	//}
}

CXSLBattleFieldManager::~CXSLBattleFieldManager()
{
}

ImplToStringW( CXSLBattleFieldManager )
{
	stm_	<< L"----------[ Battle Field Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapBattleField.size() )
			<< TOSTRINGW( m_iMonsterRespawnFactorByUserCount )
			<< TOSTRINGW( m_iMonsterRespawnTimeMin )
			<< TOSTRINGW( m_iMonsterRespawnTimeMax )
			<< TOSTRINGW( m_mapEliteMonsterDropInfo.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( CXSLBattleFieldManager )
{
	lua_tinker::class_add<CXSLBattleFieldManager>( GetLuaState(), "CXSLBattleFieldManager" );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "AddBattleFieldData",					&CXSLBattleFieldManager::AddBattleFieldData_LUA );	
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetMonsterRespawnFactorByUserCount",	&CXSLBattleFieldManager::SetMonsterRespawnFactorByUserCount_LUA );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetMonsterRespawnTimeMinMax",		&CXSLBattleFieldManager::SetMonsterRespawnTimeMinMax_LUA );
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetBattleFieldEDFactor",				&CXSLBattleFieldManager::SetBattleFieldEDFactor_LUA );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetBattleFieldEXPFactor",			&CXSLBattleFieldManager::SetBattleFieldEXPFactor_LUA );
#else
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetBattleFieldFactor",				&CXSLBattleFieldManager::SetBattleFieldFactor_LUA );
#endif SERV_FIELD_ED_EXP_FACTOR
	//}
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "AddEliteMonsterDropInfo",			&CXSLBattleFieldManager::AddEliteMonsterDropInfo_LUA );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetDangerousValueMax",				&CXSLBattleFieldManager::SetDangerousValueMax_LUA );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetDangerousValueWarning",			&CXSLBattleFieldManager::SetDangerousValueWarning_LUA );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetBossCheckUserCount",				&CXSLBattleFieldManager::SetBossCheckUserCount_LUA );
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetEliteMonsterDropValue",			&CXSLBattleFieldManager::SetEliteMonsterDropValue_LUA );
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "BattleFieldMiddleBossDropInfo",		&CXSLBattleFieldManager::SetBattleFieldMiddleBossDropInfo_LUA );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "BattleFieldEventBossDropInfo",		&CXSLBattleFieldManager::SetBattleFieldEventBossDropInfo_LUA );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

	//{{ 2013. 03. 07	��Ʋ �ʵ� ���赵 �̺�Ʈ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	lua_tinker::class_def<CXSLBattleFieldManager>( GetLuaState(), "SetDangerousValueEventRate",			&CXSLBattleFieldManager::SetDangerousValueEventRate_LUA );
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	//}

	lua_tinker::decl( GetLuaState(), "g_pBattleFieldManager", this );
}

bool CXSLBattleFieldManager::OpenScriptFile_AllBattleFieldScriptLoad( IN lua_State* pLuaState )
{
	if( OpenScriptFile( pLuaState ) == false )
	{
		std::wstring wstrErr = KncUtil::toWideString( lua_tostring( pLuaState, -1 ) );

		START_LOG( cerr, L"��Ʋ�ʵ� ��ũ��Ʈ �Ľ� ����!" )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;
		return false;
	}

	if( m_mapBattleField.empty() )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� ��ũ��Ʈ �Ľ� ����!" )
			<< BUILD_LOG( m_mapBattleField.size() )
			<< END_LOG;
		return false;
	}

	const DWORD dwTime = ::GetTickCount();		

	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit;
	for( mit = m_mapBattleField.begin(); mit != m_mapBattleField.end(); ++mit )
	{
		if( mit->second.OpenScriptFile() == false )
		{
			START_LOG( cerr, L"��Ʋ�ʵ� ��ũ��Ʈ �Ľ� ����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// �ʱ�ȭ�� �����ʹ� �ʱ�ȭ ����!
		mit->second.SetMonsterRespawnFactorByUserCount( GetMonsterRespawnFactorByUserCount() );
	}

	START_LOG( cout2, L"��Ʋ�ʵ� ��ũ��Ʈ �ε� �ð� : " )
		<< BUILD_LOG( GetTickCount() - dwTime );
	return true;
}

void CXSLBattleFieldManager::SetMonsterRespawnFactorByUserCount_LUA( int iFactor )
{
	m_iMonsterRespawnFactorByUserCount = iFactor;

	START_LOG( cout, L"���� �������� ���� ī��Ʈ�� �������� ����� : " << m_iMonsterRespawnFactorByUserCount );
}

void CXSLBattleFieldManager::SetMonsterRespawnTimeMinMax_LUA( int iMin, int iMax )
{
	m_iMonsterRespawnTimeMin = iMin;
	m_iMonsterRespawnTimeMax = iMax;

	START_LOG( cout, L"���� ������ Ÿ�� MinMax" )
		<< BUILD_LOG( m_iMonsterRespawnTimeMin )
		<< BUILD_LOG( m_iMonsterRespawnTimeMax );
}

//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
void CXSLBattleFieldManager::SetBattleFieldEDFactor_LUA( float fFactor )
{
	m_fBattleFieldEDFactor = fFactor;

	START_LOG( cout, L"��Ʋ�ʵ� ED ��� : " << m_fBattleFieldEDFactor );
}
void CXSLBattleFieldManager::SetBattleFieldEXPFactor_LUA( float fFactor )
{
	m_fBattleFieldEXPFactor = fFactor;

	START_LOG( cout, L"��Ʋ�ʵ� EXP��� : " << m_fBattleFieldEXPFactor );
}
#else
void CXSLBattleFieldManager::SetBattleFieldFactor_LUA( float fFactor )
{
	m_fBattleFieldFactor = fFactor;

	START_LOG( cout, L"��Ʋ�ʵ� ��� : " << m_fBattleFieldFactor );
}

#endif SERV_FIELD_ED_EXP_FACTOR
//}

void CXSLBattleFieldManager::AddEliteMonsterDropInfo_LUA( int iBeginUserCount, int iEndUserCount, char cDifficultyLevel, int iMaxMonsterCount )
{
	if( iBeginUserCount < 0  ||  iEndUserCount < 0  ||  cDifficultyLevel < 0  ||  iMaxMonsterCount <= 0  ||  iBeginUserCount > iEndUserCount )
	{
		START_LOG( cerr, L"�߸��� ����Ʈ ���� ���� ���� �Դϴ�!" )
			<< BUILD_LOG( iBeginUserCount )
			<< BUILD_LOG( iEndUserCount )
			<< BUILD_LOGc( cDifficultyLevel )
			<< BUILD_LOG( iMaxMonsterCount )
			<< END_LOG;
		return;
	}

    for( int iNum = iBeginUserCount; iNum <= iEndUserCount ; ++iNum )
	{
		SEliteMonsterDropInfo kInfo;
		kInfo.m_cDifficultyLevel = cDifficultyLevel;
		kInfo.m_iMaxMonsterCount = iMaxMonsterCount;
		m_mapEliteMonsterDropInfo.insert( std::make_pair( iNum, kInfo ) );
	}
}

void CXSLBattleFieldManager::SetDangerousValueMax_LUA( int iValue )
{
	m_iDangerousValueMax = iValue;

    START_LOG( cout, L"���赵 �ִ밪 : " << m_iDangerousValueMax );
}

void CXSLBattleFieldManager::SetDangerousValueWarning_LUA( int iValue )
{
	m_iDangerousValueWarning = iValue;
	
	START_LOG( cout, L"���赵 ��� : " << m_iDangerousValueWarning );
}

void CXSLBattleFieldManager::SetBossCheckUserCount_LUA( int iValue )
{
	m_iBossCheckUserCount = iValue;
	
	START_LOG( cout, L"���� ���� �ּ� ���� ī��Ʈ : " << m_iBossCheckUserCount );
}

void CXSLBattleFieldManager::SetEliteMonsterDropValue_LUA( int iValue )
{
    m_iEliteMonsterDropValue = iValue;

	START_LOG( cout, L"����Ʈ ���� ���� ��� �� : " << m_iEliteMonsterDropValue );
}

bool CXSLBattleFieldManager::AddBattleFieldData_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	SEnum::BATTLE_FIELD_ID eBattleFieldID;
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"BATTLE_FIELD_ID",		eBattleFieldID,		SEnum::BATTLE_FIELD_ID,	SEnum::BFI_INVALID,	return false; );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
	bool bBossField;
	LUA_GET_VALUE(	luaManager, L"IS_BOSS_RAID_FIELD",	bBossField, false );
	
	if( bBossField == true )
	{
		m_setBossFieldID.insert( static_cast<int>( eBattleFieldID ) );
	}
#endif // SERV_BATTLE_FIELD_BOSS

	std::pair< std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator, bool > pairRet;
	pairRet = m_mapBattleField.insert( std::make_pair( eBattleFieldID, CXSLBattleField() ) );
	if( pairRet.second == false )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� ���ҽ� ��ü ���� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	CXSLBattleField& kBattleFieldData = pairRet.first->second;
    if( kBattleFieldData.LoadBattleFieldData( eBattleFieldID, luaManager ) == false )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� ��ũ��Ʈ �Ľ� ����!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	return true;
}

bool CXSLBattleFieldManager::GetCreateMonsterList( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
												   IN const int iPlayerCount,
												   OUT KNPCList& kBattleFieldNpcList )
{   
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	return mit->second.GetCreateMonsterList( iPlayerCount, kBattleFieldNpcList.m_NPCList );
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
bool CXSLBattleFieldManager::GetCreateBossMonsterList( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, OUT KNPCList& kBattleFieldNpcList )
{   
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator it = m_mapBattleField.find( eBattleFieldID );
	if( it == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	return it->second.GetCreateBossMonsterList( kBattleFieldNpcList.m_NPCList );
}
#endif // SERV_BATTLE_FIELD_BOSS

bool CXSLBattleFieldManager::GetRespawnMonsterList( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
													IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterCountInfo,
													IN const std::map< int, int >& mapRespawnReadyNpcCount,
													OUT KNPCList& kBattleFieldNpcList )
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}
	
	return mit->second.GetRespawnMonsterList( kAliveMonsterCountInfo, mapRespawnReadyNpcCount, kBattleFieldNpcList.m_NPCList );
}

bool CXSLBattleFieldManager::GetBattieFieldBossMonsterInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
														    IN const int iDangerousValue,
															OUT KNPCUnitReq& kNpcInfo )
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	return mit->second.GetBattieFieldBossMonsterInfo( iDangerousValue, kNpcInfo );
}

bool CXSLBattleFieldManager::CheckPossibleEliteMonsterDrop( IN const int iPlayerCount, 
														    IN const int iCurrentEliteMonsterCount,
															OUT char& cDifficultyLevelForEliteMonsterDrop )
{
	std::map< int, SEliteMonsterDropInfo >::const_iterator mit;
	mit = m_mapEliteMonsterDropInfo.find( iPlayerCount );
	if( mit == m_mapEliteMonsterDropInfo.end() )
	{
		START_LOG( cerr, L"����Ʈ ���� ���� ������ ��ϵǾ����� ���� �÷��̾� ī��Ʈ���Դϴ�!" )
			<< BUILD_LOG( iPlayerCount )
			<< END_LOG;
		return false;
	}

	// ���� ����Ʈ ���� ���� �ʹ� ���Ƽ� �߰��� ����Ʈ ���͸� ���� �����ϴ�!
	if( iCurrentEliteMonsterCount >= mit->second.m_iMaxMonsterCount )
		return false;

	cDifficultyLevelForEliteMonsterDrop = mit->second.m_cDifficultyLevel;
    return true;
}

int	CXSLBattleFieldManager::GetMaxMonsterCountInBattleField( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount ) const
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::const_iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return 0;
	}

	return mit->second.GetMaxMonsterCountInThisBattleField( iPlayerCount );
}

int	CXSLBattleFieldManager::GetMaxJoinUserCountInBattleField( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID ) const
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::const_iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;

		return SEnum::BATTLE_FIELD_MAX_NUM;
	}

	return mit->second.GetMaxJoinUserCount();
}

double CXSLBattleFieldManager::GetMonsterRespawnTimeByRandom() const
{
	const int RANDOM_FACTOR = m_iMonsterRespawnTimeMax - m_iMonsterRespawnTimeMin;
	if( RANDOM_FACTOR == 0 )
	{
		START_LOG( cerr, L"�̰��� 0�� ������ �ȵǴµ�! ��" )
			<< BUILD_LOG( RANDOM_FACTOR )
			<< BUILD_LOG( m_iMonsterRespawnTimeMin )
			<< BUILD_LOG( m_iMonsterRespawnTimeMax )
			<< END_LOG;

		return static_cast<double>(DE_MONSTER_RESPAWN_TIME_MAX);
	}

	return static_cast<double>( ( rand() % RANDOM_FACTOR ) + m_iMonsterRespawnTimeMin );
}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
float CXSLBattleFieldManager::GetMiddleBossMonsterDropRate( IN const int iCurDangerousValue, IN const int iOldDangerousValue )
{
	const float fBossDropRate = (float)lua_tinker::call<int>( GetLuaState(), "GET_MIDDLE_BOSS_MONSTER_DROP_RATE", iCurDangerousValue, iOldDangerousValue );

#ifdef SERV_NO_BATTLEFIELD_MIDDLE_BOSS
	return 0.f;
#endif // SERV_NO_BATTLEFIELD_MIDDLE_BOSS

	return fBossDropRate;
}

bool CXSLBattleFieldManager::GetBattieFieldMiddleBossMonsterInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
																 IN const int iDangerousValue,
																 OUT std::vector<KNPCUnitReq>& vecNpcInfo )
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}
	return mit->second.GetBattieFieldMiddleBossMonsterInfo( iDangerousValue, vecNpcInfo );
}

void CXSLBattleFieldManager::SetBattleFieldMiddleBossDropInfo_LUA()
{
	int iBattleFieldID = 0;
	int iNpcID = 0;

	KFieldMiddleBossDropKey kKey;
	KFieldMiddleBossDropInfo kInfo;

	KLuaManager luaManager( GetLuaState() );

	LUA_GET_VALUE_RETURN(	luaManager, L"BattleFieldID",							iBattleFieldID,									0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"MonsterID",								iNpcID,											0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetMiddleBossCommonBonusItemID",			kInfo.m_iMiddleBossCommonBonusItemID,			0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetMiddleBossCommonBonusItemCount",		kInfo.m_iMiddleBossCommonBonusItemCount,		0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetMiddleBossBonusItemID",				kInfo.m_iMiddleBossBonusItemID,					0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetMiddleBossBonusItemIDCount_High",		kInfo.m_iMiddleBossBonusItemIDCount_High,		0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetMiddleBossBonusItemIDCount_Low",		kInfo.m_iMiddleBossBonusItemIDCount_Low,		0,	return; );

	kKey.first  = iBattleFieldID;
	kKey.second = iNpcID;

	std::map< KFieldMiddleBossDropKey, KFieldMiddleBossDropInfo >::iterator mit = m_mapBattleFieldMiddleBossDropInfo.find( kKey );
	if( mit != m_mapBattleFieldMiddleBossDropInfo.end() )
	{
		START_LOG( cerr, L"�ߺ� �����ϴ� ���� ��� ���� �Դϴ�." )
			<< BUILD_LOG( iBattleFieldID )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( kInfo.m_iMiddleBossCommonBonusItemID )
			<< BUILD_LOG( kInfo.m_iMiddleBossCommonBonusItemCount )
			<< BUILD_LOG( kInfo.m_iMiddleBossBonusItemID )
			<< BUILD_LOG( kInfo.m_iMiddleBossBonusItemIDCount_High )
			<< BUILD_LOG( kInfo.m_iMiddleBossBonusItemIDCount_High )
			<< END_LOG;
		return;
	}

	m_mapBattleFieldMiddleBossDropInfo.insert( std::make_pair( kKey, kInfo ) );
}

bool CXSLBattleFieldManager::GetBattleFieldMiddleBossDropInfo( IN int iFieldID, IN int iNpcID, OUT KFieldMiddleBossDropInfo& kDropInfo )
{
	KFieldMiddleBossDropKey kKey;
	kKey.first = iFieldID;
	kKey.second = iNpcID;

	std::map< KFieldMiddleBossDropKey, KFieldMiddleBossDropInfo >::iterator mit = m_mapBattleFieldMiddleBossDropInfo.find( kKey );
	if( mit == m_mapBattleFieldMiddleBossDropInfo.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ���� ��� ���� �Դϴ�!" )
			<< BUILD_LOG( iFieldID )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;

		return false;
	}

	kDropInfo = mit->second;

	return true;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
float CXSLBattleFieldManager::GetEventBossMonsterDropRate( IN const int iCurEventDangerousValue, IN const int iOldEventDangerousValue )
{
	const float fBossEventDropRate = (float)lua_tinker::call<int>( GetLuaState(), "GET_EVENT_BOSS_MONSTER_DROP_RATE", iCurEventDangerousValue, iOldEventDangerousValue );

#ifdef SERV_NO_BATTLEFIELD_MIDDLE_BOSS
	return 0.f;
#endif // SERV_NO_BATTLEFIELD_MIDDLE_BOSS

	return fBossEventDropRate;
}

bool CXSLBattleFieldManager::GetBattieFieldEventBossMonsterInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
	IN const int iEventDangerousValue,
	OUT std::vector<KNPCUnitReq>& vecNpcInfo )
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}
	return mit->second.GetBattieFieldEventBossMonsterInfo( iEventDangerousValue, vecNpcInfo );
}

void CXSLBattleFieldManager::SetBattleFieldEventBossDropInfo_LUA()
{
	int iBattleFieldID = 0;
	int iNpcID = 0;

	KFieldEventBossDropKey kKey;
	KFieldEventBossDropInfo kInfo;

	KLuaManager luaManager( GetLuaState() );

	LUA_GET_VALUE_RETURN(	luaManager, L"BattleFieldID",							iBattleFieldID,									0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"MonsterID",								iNpcID,											0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetEventBossCommonBonusItemID",			kInfo.m_iEventBossCommonBonusItemID,			0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetEventBossCommonBonusItemCount",		kInfo.m_iEventBossCommonBonusItemCount,		0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetEventBossBonusItemID",				kInfo.m_iEventBossBonusItemID,					0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetEventBossBonusItemIDCount_High",		kInfo.m_iEventBossBonusItemIDCount_High,		0,	return; );
	LUA_GET_VALUE_RETURN(	luaManager, L"SetEventBossBonusItemIDCount_Low",		kInfo.m_iEventBossBonusItemIDCount_Low,		0,	return; );

	kKey.first  = iBattleFieldID;
	kKey.second = iNpcID;

	std::map< KFieldEventBossDropKey, KFieldEventBossDropInfo >::iterator mit = m_mapBattleFieldEventBossDropInfo.find( kKey );
	if( mit != m_mapBattleFieldEventBossDropInfo.end() )
	{
		START_LOG( cerr, L"�ߺ� �����ϴ� ���� ��� ���� �Դϴ�." )
			<< BUILD_LOG( iBattleFieldID )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( kInfo.m_iEventBossCommonBonusItemID )
			<< BUILD_LOG( kInfo.m_iEventBossCommonBonusItemCount )
			<< BUILD_LOG( kInfo.m_iEventBossBonusItemID )
			<< BUILD_LOG( kInfo.m_iEventBossBonusItemIDCount_High )
			<< BUILD_LOG( kInfo.m_iEventBossBonusItemIDCount_High )
			<< END_LOG;
		return;
	}

	m_mapBattleFieldEventBossDropInfo.insert( std::make_pair( kKey, kInfo ) );
}

bool CXSLBattleFieldManager::GetBattleFieldEventBossDropInfo( IN int iFieldID, IN int iNpcID, OUT KFieldEventBossDropInfo& kDropInfo )
{
	KFieldEventBossDropKey kKey;
	kKey.first = iFieldID;
	kKey.second = iNpcID;

	std::map< KFieldEventBossDropKey, KFieldEventBossDropInfo >::iterator mit = m_mapBattleFieldEventBossDropInfo.find( kKey );
	if( mit == m_mapBattleFieldEventBossDropInfo.end() )
	{
		START_LOG( clog, L"�������� �ʴ� ���� ��� ���� �Դϴ�!" )
			<< BUILD_LOG( iFieldID )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;

		return false;
	}

	kDropInfo = mit->second;

	return true;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

//{{ 2013. 03. 07	��Ʋ �ʵ� ���赵 �̺�Ʈ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
void CXSLBattleFieldManager::SetDangerousValueEventRate_LUA( int iValue )
{
	m_iDangerousValueEventRate = iValue;

	START_LOG( cout, L"��Ʋ �ʵ� ���赵 ���� : " << m_iDangerousValueEventRate );
}
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
//}

float CXSLBattleFieldManager::GetBossMonsterDropRate( IN const int iCurDangerousValue )
{
    const float fBossDropRate = (float)lua_tinker::call<int>( GetLuaState(), "GET_BOSS_MONSTER_DROP_RATE", iCurDangerousValue, GetDangerousValueWarning() );
	return fBossDropRate;
}

bool CXSLBattleFieldManager::IsCanEnterBattleField( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iRequireLevel, OUT int& iRequireDungeonID ) const
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::const_iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;

		return false;
	}

	return mit->second.IsCanEnterBattleField( ucLevel, mapDungeonClear, iRequireLevel, iRequireDungeonID );
}

int CXSLBattleFieldManager::GetStandardMonsterLevel( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID )
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::const_iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;

		return 1;
	}

	return mit->second.GetStandardMonsterLevel();
}

//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
int	CXSLBattleFieldManager::GetEnterVillageMapID( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID ) const
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::const_iterator mit;
	mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;

		return 0;
	}

	return mit->second.GetEnterVillageMapID();
}
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
//}}

//{{ 2013. 1. 1	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
bool CXSLBattleFieldManager::GetEventMonsterCreateInfo( IN const SEnum::BATTLE_FIELD_ID	eBattleFieldID, OUT KNPCUnitReq& kNpcInfo )
{
	std::map< SEnum::BATTLE_FIELD_ID, CXSLBattleField >::iterator mit = m_mapBattleField.find( eBattleFieldID );
	if( mit == m_mapBattleField.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( eBattleFieldID )
			<< END_LOG;
		return false;
	}

	return mit->second.GetEventMonsterCreateInfo( kNpcInfo );
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-30	// �ڼ���
bool CXSLBattleFieldManager::GetRandomBossFieldID( OUT int& iVillageMapID ) const
{
	if( m_setBossFieldID.empty() == true )
	{
		return false;
	}

	std::set<int>::const_iterator it = m_setBossFieldID.begin();
	for( int i = ( rand() % static_cast<int>( m_setBossFieldID.size() ) ); 0 < i; --i )
	{
		++it;
	}

	iVillageMapID = *it;

	return true;
}
#endif // SERV_BATTLE_FIELD_BOSS