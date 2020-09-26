#include "XSLBattleField.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"
#include "NetError.h"


CXSLBattleField::CXSLBattleField()
	: m_luaState( NULL )
	, m_eBattleFieldID( SEnum::BFI_INVALID )
	, m_sWorldID( 0 )
	, m_iStandardMonsterLevel( 0 )
	, m_iMaxNumberMonster( 0 )
	, m_iMonsterRespawnFactorByUserCount( 0 )
	, m_iMaxJoinUserCount( SEnum::BATTLE_FIELD_MAX_NUM )
	, m_iRequireLevel( 0 )
{
}

CXSLBattleField::~CXSLBattleField()
{
}

bool CXSLBattleField::OpenScriptFile()
{
	// Enum.lua ������ �Ľ̵Ǿ� �־�� �Ѵ�.

	KLuaManager luaManager;

	KAutoPath		kautoPaht;
	std::string		strFile;

	strFile = "Enum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "DungeonEnum.lua";
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}

	strFile = "DLG_Map_Enum.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;
		return false;
	}
	
	lua_tinker::class_add<CXSLBattleField>( luaManager.GetLuaState(), "CXSLBattleField" );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "SetTeamStartPos",		&CXSLBattleField::SetTeamStartPos_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddStartPos",			&CXSLBattleField::LoadLineMapData_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "SetLandHeight",			&CXSLBattleField::SetLandHeight_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddLine_LUA",			&CXSLBattleField::AddLine_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddCameraData_LUA",		&CXSLBattleField::AddCameraData_LUA );
	lua_tinker::class_def<CXSLBattleField>( luaManager.GetLuaState(), "AddSpawnMonsterGroup",	&CXSLBattleField::AddSpawnMonsterGroup_LUA );
	lua_tinker::decl( luaManager.GetLuaState(), "g_pLineMap", this );
	lua_tinker::decl( luaManager.GetLuaState(), "g_pBattleFieldManager", this );

	lua_tinker::class_add<D3DXVECTOR3>( luaManager.GetLuaState(), "D3DXVECTOR3" );
#ifdef _CONVERT_VS_2010
	lua_tinker::class_con<D3DXVECTOR3>( luaManager.GetLuaState(), lua_tinker::constructor<float, float, float> );
#else
	lua_tinker::class_con<D3DXVECTOR3>( luaManager.GetLuaState(), lua_tinker::constructor<float, float, float>() );
#endif _CONVERT_VS_2010
	lua_tinker::class_mem<D3DXVECTOR3>( luaManager.GetLuaState(), "x", &D3DXVECTOR3::x );
	lua_tinker::class_mem<D3DXVECTOR3>( luaManager.GetLuaState(), "y", &D3DXVECTOR3::y );
	lua_tinker::class_mem<D3DXVECTOR3>( luaManager.GetLuaState(), "z", &D3DXVECTOR3::z );

	// lua state�� ����!
	m_luaState = luaManager.GetLuaState();

	if( m_wstrLineMapFileName.empty() )
	{
		START_LOG( cout, L"[�˸�] �ش� �ʵ忡�� ���θ� ��ũ��Ʈ�� ���׿�!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< END_LOG;
		return true;
	}

	strFile = KncUtil::toNarrowString( m_wstrLineMapFileName );
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		std::wstring wstrErr = KncUtil::toWideString( lua_tostring( luaManager.GetLuaState(), -1 ) );
		START_LOG( cerr, m_wstrLineMapFileName )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;
		return false;
	}

	if( m_wstrNpcTableFileName.empty() )
	{
		START_LOG( cout, L"[�˸�] �ش� �ʵ忡�� NPC ��ũ��Ʈ�� ���׿�!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< END_LOG;
		return true;
	}

	strFile = KncUtil::toNarrowString( m_wstrNpcTableFileName );
	kautoPaht.GetPullPath( strFile );
	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		std::wstring wstrErr = KncUtil::toWideString( lua_tostring( luaManager.GetLuaState(), -1 ) );
		START_LOG( cerr, m_wstrNpcTableFileName )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;
		return false;
	}

	return true;
}

bool CXSLBattleField::LoadBattleFieldData( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN OUT KLuaManager& kLuaManager )
{
	m_eBattleFieldID = eBattleFieldID;

	//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	LUA_GET_VALUE_RETURN(		kLuaManager, L"ENTER_VILLAGE_MAP_ID",						m_iEnterVillageMapID,		0,		return false; );
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}
	LUA_GET_VALUE_RETURN(		kLuaManager, L"WORLD_ID",									m_sWorldID,					0,		return false; );
	LUA_GET_VALUE_RETURN(		kLuaManager, L"LINE_MAP_FILE_NAME",							m_wstrLineMapFileName,		L"",	return false; );
	LUA_GET_VALUE(				kLuaManager, L"NPC_TABLE_FILE_NAME",						m_wstrNpcTableFileName,		L"" );
	if( m_wstrNpcTableFileName.empty() )
	{
		START_LOG( cout, L"[�˸�] �ش� ��Ʋ�ʵ�� NPC���̺� ������ �����ϴ�! ���Ͱ� �������� ���� �� ������ ���� �ٶ��ϴ�." )
			<< BUILD_LOG( eBattleFieldID );
	}

	LUA_GET_VALUE(				kLuaManager, L"REQUIRE_LEVEL",								m_iRequireLevel,			0 );
	if( kLuaManager.BeginTable( L"REQUIRE_DUNGEON" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( kLuaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
			{
				m_setRequireDungeon.insert( buf );
			}

			index++;
		}

		kLuaManager.EndTable();
	}

	LUA_GET_VALUE_RETURN(		kLuaManager, L"STANDARD_MONSTER_LEVEL",						m_iStandardMonsterLevel,	0,		return false; );
	//LUA_GET_VALUE_RETURN(		kLuaManager, L"BATTLE_FIELD_NAME",							m_wstrBattleFieldName,		L"",	return false; ); -- ���������� �Ľ� �ȵ˴ϴ�.
	LUA_GET_VALUE_RETURN(		kLuaManager, L"MAX_NUMBER_OF_MONSTER_IN_THIS_BATTLE_FIELD",	m_iMaxNumberMonster,		0,		return false; );
	LUA_GET_VALUE(				kLuaManager, L"MAX_JOIN_USER_COUNT",						m_iMaxJoinUserCount,		SEnum::BATTLE_FIELD_MAX_NUM );

	if( kLuaManager.BeginTable( L"BATTLE_FIELD_PORTAL_MOVING_INFO" ) == S_OK )
	{
		int index = 1;
		while( kLuaManager.BeginTable( index ) == S_OK )
		{
			CXSLBattleField::SPotalMovingInfo sPotalMovingInfo;
			LUA_GET_VALUE_RETURN_ENUM(	kLuaManager, L"PORTAL_MOVE_TYPE",		sPotalMovingInfo.m_ePortalMoveType,			CXSLBattleField::PORTAL_MOVE_TYPE, CXSLBattleField::PMT_INVALID, return false );
			LUA_GET_VALUE(				kLuaManager, L"LINE_NUMBER",			sPotalMovingInfo.m_iLineNumber,				0 );
			LUA_GET_VALUE(				kLuaManager, L"PLACE_ID_TO_MOVE",		sPotalMovingInfo.m_iPlaceIDToMove,			0 );
			LUA_GET_VALUE(				kLuaManager, L"POSITION_INDEX_TO_MOVE",	sPotalMovingInfo.m_iPositionIndexToMove,	0 );

			m_vecPotalMovingInfo.push_back( sPotalMovingInfo );

			index++;
			kLuaManager.EndTable();
		}
		kLuaManager.EndTable();
	}

	
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	if( kLuaManager.BeginTable( L"BATTLE_FIELD_MIDDLE_BOSS_INFO" ) == S_OK )
	{
		int index = 1; 
		while( kLuaManager.BeginTable( index ) == S_OK )
		{
			SRiskInfo sRiskInfo;
			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_ID",		sRiskInfo.m_iSpawnID,	0, return false );

			if( kLuaManager.BeginTable( L"SPAWN_GROUP_ID" ) == S_OK )
			{
				int iValue	= 1; 
				int buf		= -1;
				while( kLuaManager.GetValue( iValue, buf ) == S_OK )
				{
					if( buf > 0 )
					{
						// ����ó�� �ߺ� Ȯ��
						bool bExist = false;
						BOOST_TEST_FOREACH( int, iGroupID, sRiskInfo.m_vecSpawnGroupID )
						{
							if( iGroupID == buf )
							{
								bExist = true;
								break;
							}
						}

						if( bExist == false )
						{
							sRiskInfo.m_vecSpawnGroupID.push_back( buf );
						}
						else
						{
							START_LOG( cerr, L"�����ϴ� �ߺ� �׷� ID �Է��Դϴ�." )
								<< BUILD_LOG( eBattleFieldID )
								<< END_LOG;
						}
					}
					iValue++;
				}

				kLuaManager.EndTable();
			}

			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_RATE",				sRiskInfo.m_iSpawnRate,			0, return false );
			LUA_GET_VALUE_RETURN(	kLuaManager, L"RISK_VALUE",				sRiskInfo.m_iRiskValue,			0, return false );
			LUA_GET_VALUE(			kLuaManager, L"SHOW_BOSS_NAME",			sRiskInfo.m_bShowBossName,		false );
			LUA_GET_VALUE(			kLuaManager, L"BOSS_GAUGE_HP_LINES",	sRiskInfo.m_bBossGaugeHpLines,	false );

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			if( sRiskInfo.m_iSpawnID > 100 )
			{
				m_vecEventBossInfo.push_back( sRiskInfo );
			}
			else
			{
				m_vecMiddleBossInfo.push_back( sRiskInfo );
			}
#else //SERV_BATTLEFIELD_EVENT_BOSS_INT
			m_vecMiddleBossInfo.push_back( sRiskInfo );
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT

			index++;
			kLuaManager.EndTable();
		}

		kLuaManager.EndTable();

		// ����ó�� Ȯ�� Ȯ��
		int iTotalRate = 0; 
		BOOST_TEST_FOREACH( SRiskInfo&, sRiskInfo, m_vecMiddleBossInfo )
		{
			iTotalRate += sRiskInfo.m_iSpawnRate;
		}

		if( iTotalRate < 100 || iTotalRate > 100 )
		{
			START_LOG( cerr, L"�ߺ� �׷� Ȯ������ �̻��մϴ�." )
				<< BUILD_LOG( eBattleFieldID )
				<< BUILD_LOG( iTotalRate )
				<< END_LOG;
		}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		// ����ó�� Ȯ�� Ȯ��
		if( m_vecEventBossInfo.size() != 0 )
		{
			int iEventTotalRate = 0; 
			BOOST_TEST_FOREACH( SRiskInfo&, sRiskInfo, m_vecEventBossInfo )
			{
				iEventTotalRate += sRiskInfo.m_iSpawnRate;
			}

			if( iEventTotalRate < 100 || iEventTotalRate > 100 )
			{
				START_LOG( cerr, L"�̺�Ʈ���� �׷� Ȯ������ �̻��մϴ�." )
					<< BUILD_LOG( eBattleFieldID )
					<< BUILD_LOG( iEventTotalRate )
					<< END_LOG;
			}
		}	
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	}
#else
	if( kLuaManager.BeginTable( L"BATTLE_FIELD_RISK_INFO" ) == S_OK )
	{
		int index = 1; 
		while( kLuaManager.BeginTable( index ) == S_OK )
		{
			SRiskInfo sRiskInfo;
			LUA_GET_VALUE_RETURN(	kLuaManager, L"RISK_VALUE",				sRiskInfo.m_iRiskValue,			0, return false );
			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_MONSTER_ID",		sRiskInfo.m_iSpawnMonsterID,	0, return false );
			LUA_GET_VALUE_RETURN(	kLuaManager, L"SPAWN_GROUP_ID",			sRiskInfo.m_iSpawnGroupID,		0, return false );
			LUA_GET_VALUE(			kLuaManager, L"SHOW_BOSS_NAME",			sRiskInfo.m_bShowBossName,		false );
			LUA_GET_VALUE(			kLuaManager, L"BOSS_GAUGE_HP_LINES",	sRiskInfo.m_bBossGaugeHpLines,	false );

			m_vecRiskInfo.push_back( sRiskInfo );

			index++;
			kLuaManager.EndTable();
		}
		kLuaManager.EndTable();
	}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	return true;
}

bool CXSLBattleField::AddSpawnMonsterGroup_LUA()
{
	KLuaManager luaManager( m_luaState );

	SSpawnNpcGroup sNpcGroup;
	LUA_GET_VALUE_RETURN(	luaManager, L"SPAWN_GROUP_ID",							sNpcGroup.m_iGroupID,						0,	return false; );
	LUA_GET_VALUE(			luaManager, L"IS_BOSS_MONSTER_GROUP",					sNpcGroup.m_bIsBossMonsterGroup,			false );
	LUA_GET_VALUE(			luaManager, L"MIN_NUMBER_OF_MONSTER_IN_THIS_GROUP",		sNpcGroup.m_iMinNumberOfMonsterInThisGroup,	0 );
	LUA_GET_VALUE_RETURN(	luaManager, L"MAX_NUMBER_OF_MONSTER_IN_THIS_GROUP",		sNpcGroup.m_iMaxNumberOfMonsterInThisGroup,	0,	return false; );


	if( luaManager.BeginTable( L"SIEGE_MONSTER" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
			{
				sNpcGroup.m_setSiegeMonster.insert( buf );
			}

			index++;
		}

		luaManager.EndTable();
	}

	int index = 1;
	while( luaManager.BeginTable( index ) == S_OK )
	{
		SSpawnNpcInfo sNpcInfo;
		LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"MONSTER_ID",					sNpcInfo.m_eNpcID,		CXSLUnitManager::NPC_UNIT_ID, CXSLUnitManager::NUI_NONE, return false );
		LUA_GET_VALUE(				luaManager, L"RATE",						sNpcInfo.m_fRate,					0.f );
		LUA_GET_VALUE(				luaManager, L"MAX_NUMBER_OF_THIS_MONSTER",	sNpcInfo.m_iMaxNumberOfThisMonster,	0 );
		LUA_GET_VALUE(				luaManager, L"ADD_LEVEL",					sNpcInfo.m_iAddLevel,				0 );
		LUA_GET_VALUE(				luaManager, L"DROP",						sNpcInfo.m_bDrop,					false );
		LUA_GET_VALUE(				luaManager, L"AGRESSIVE",					sNpcInfo.m_bAggressive,				true );
		LUA_GET_VALUE(				luaManager, L"ADD_POS_Y",					sNpcInfo.m_fAddPosY,				0.f );
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		LUA_GET_VALUE(				luaManager, L"FOCUS_CAMERA",				sNpcInfo.m_bFocusCamera,			false );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		if( luaManager.BeginTable( L"START_POSITION" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					sNpcInfo.m_StartPositionLot.AddCaseSameProb( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( L"PETROL_LINE_INDEX" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					sNpcInfo.m_vecPetrolLineIndex.push_back( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( L"PLAY_LINE_INDEX" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					sNpcInfo.m_vecPlayLineIndex.push_back( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		sNpcGroup.m_mapSpawnNpcList.insert( std::make_pair( sNpcInfo.m_eNpcID, sNpcInfo ) );

		index++;
		luaManager.EndTable();
	}

	if( m_mapNpcGroupList.find( sNpcGroup.m_iGroupID ) != m_mapNpcGroupList.end() )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ���� �׷��Դϴ�!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( sNpcGroup.m_iGroupID )
			<< END_LOG;
		return false;
	}

	// ���� ���� �׷��� �ƴѰ�쿡�� ����Ʈ�� �ִ´�!
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	( sNpcGroup.m_bIsBossMonsterGroup == true ) ? m_vecBossNpcGroupList.push_back( sNpcGroup ) : m_vecRespawnNpcGroupList.push_back( sNpcGroup );
#else // SERV_BATTLE_FIELD_BOSS
	if( sNpcGroup.m_bIsBossMonsterGroup == false )
	{
		m_vecRespawnNpcGroupList.push_back( sNpcGroup );
	}	
#endif // SERV_BATTLE_FIELD_BOSS
	m_mapNpcGroupList.insert( std::make_pair( sNpcGroup.m_iGroupID, sNpcGroup ) );
	return true;
}

void CXSLBattleField::SetLandHeight_LUA( int iLandHeight )
{
	// ����
}

void CXSLBattleField::SetTeamStartPos_LUA()
{
	// ����
}

bool CXSLBattleField::LoadLineMapData_LUA( int iStartPos, D3DXVECTOR3 vPos, bool bRight, int iLineMapIndex )
{
	std::map< int, SMonsterStartPos >::iterator mitMSP;
	mitMSP = m_mapMonsterStartPos.find( iStartPos );
	if( mitMSP != m_mapMonsterStartPos.end() )
	{
		START_LOG( cerr, L"�̹� �����ϴ� Start Pos���Դϴ�." )
			<< BUILD_LOG( iStartPos )
			<< END_LOG;
		return false;
	}
	else
	{
		SMonsterStartPos sMonsterStartPos;
		sMonsterStartPos.m_vPos = VECTOR3( vPos.x, vPos.y, vPos.z );
		sMonsterStartPos.m_bRight = bRight;
		m_mapMonsterStartPos.insert( std::make_pair( iStartPos, sMonsterStartPos ) );
	}

	std::map< int, std::vector< int > >::iterator mitLRS;
	mitLRS = m_mapLineIndexRandomStartPos.find( iLineMapIndex );
	if( mitLRS == m_mapLineIndexRandomStartPos.end() )
	{
		std::vector< int > vecStartPosList;
		vecStartPosList.push_back( iStartPos );
		m_mapLineIndexRandomStartPos.insert( std::make_pair( iLineMapIndex, vecStartPosList ) );
	}
	else
	{
		mitLRS->second.push_back( iStartPos );
	}

	return true;
}

void CXSLBattleField::AddLine_LUA()
{
	// ����
}

void CXSLBattleField::AddCameraData_LUA()
{
	// ����
}

void CXSLBattleField::AddNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup )
{
	m_vecRespawnNpcGroupList.push_back( sSpawnNpcGroup );
}

//bool CXSLBattleField::GetBattleFieldRespawnMonsterList( IN const int iMaxMonsterCountInBattleField, 
//													    IN const int iMaxMonsterCountByUserCount,
//													    IN const int iRespawnTimeNotOverCount, 
//													    IN const std::map< int, SMonsterCountInfo >& mapMonsterCount, 
//														OUT std::vector< KNPCUnitReq >& vecNPCList )
//{
//	vecNPCList.clear();
//	
//	// �ʿ��� ���� �� ��ŭ ���͸� ��������!
//	int iRestCount = iNeedMonsterCount;
//	while( iRestCount > 0 )
//	{
//        // 1. �� �׷��� �ѹ��� ���ư��鼭 ������Ű��!
//		BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecNpcGroupList )
//		{
//			// �� ������ �ִ��� �˻�!
//			if( iRestCount <= 0 )
//			{
//				break;
//			}
//			else
//			{
//				// ���� Ƚ���� 1ȸ ���Դϴ�.
//				--iRestCount;
//			}
//
//			KLottery kMonsterLot;
//			KLotterySameProb kMonsterSameLot;
//
//			// 2. �� ���͵鿡 ���� ���� ������ �����ϴ°� �ִ��� Ȯ��!
//			{
//				std::map< int, SSpawnNpcInfo >::const_iterator mitSN;
//				for( mitSN = sSpawnNpcGroup.m_mapSpawnNpcList.begin(); mitSN != sSpawnNpcGroup.m_mapSpawnNpcList.end(); ++mitSN )
//				{
//					const SSpawnNpcInfo& sSpawnNpcInfo = mitSN->second;
//
//					// �ش� �׷��� ���� ī��Ʈ ������ ã�´�!
//					std::map< int, SMonsterCountInfo >::const_iterator mit;
//					mit = mapMonsterCount.find( static_cast<int>(sSpawnNpcGroup.m_iGroupID) );
//					if( mit != mapMonsterCount.end() )
//					{
//						int iMonsterCount = 0;
//						if( mit->second.GetMonsterCount( sSpawnNpcInfo.m_eNpcID, iMonsterCount ) == true )
//						{
//							// �̹� �ִ� ���� �������� ���������Ƿ� �н�!
//							if( iMonsterCount >= sSpawnNpcInfo.m_iMaxNumberOfThisMonster )
//								continue;
//						}
//					}
//
//					// ���� ���� Ȯ���� ����!
//					LIF( kMonsterLot.AddCase( static_cast<int>(sSpawnNpcInfo.m_eNpcID), sSpawnNpcInfo.m_fRate ) );
//					kMonsterSameLot.AddCaseSameProb( static_cast<int>(sSpawnNpcInfo.m_eNpcID) );
//				}
//			}
//
//			// 3. ������ ���͸� ��������!
//			int iResultNpcID = kMonsterLot.Decision();
//			if( iResultNpcID == KLottery::CASE_BLANK )
//			{
//                iResultNpcID = kMonsterSameLot.DecisionSameProb();
//				if( iResultNpcID == KLottery::CASE_BLANK )
//				{
//					START_LOG( cerr, L"���� ���� ������ ������ �����ߴ�! ���� �Ͼ���� �ȵǴ� ����!" )
//						<< BUILD_LOG( iResultNpcID )
//						<< END_LOG;
//					continue;
//				}
//			}
//
//			// 4. ������ ���Ͱ� �����Ǿ��ٸ� ���� ������ ����!
//			KNPCUnitReq kInfo;
//			if( MakeMonsterInfo( sSpawnNpcGroup, iResultNpcID, kInfo ) == false )
//			{
//				START_LOG( cerr, L"���� ���� ���� ����!" )
//					<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
//					<< BUILD_LOG( iResultNpcID )
//					<< END_LOG;
//				continue;
//			}
//			
//			// 5. ������� ����!
//			vecNPCList.push_back( kInfo );
//		}
//	}
//
//	return true;
//}

bool CXSLBattleField::GetCreateMonsterList( IN const int iPlayerCount, OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const
{
	vecCreateNPCList.clear();
    	
	//////////////////////////////////////////////////////////////////////////
	// �ʵ峻�� �ּ������� �����Ǿ� �־�� �ϴ� ���� ����!

	CXSLBattleField::KBattleFieldMonsterCountInfo kCreateMonsterCountInfo;


	//////////////////////////////////////////////////////////////////////////
	// ������ ���� �� ���ϱ�!
	int iCreateMonsterCount = 0;

	{
		// ���� �ʵ忡�� �ִ� ���� ������ ���� �� : ������� ����� ���ͼ����� �ʵ� �ִ� ���ͼ��� �� ũ�ٸ� �ʵ� �ִ� ���ͼ��� �����Ѵ�.
		//const int MAX_MONSTER_COUNT_IN_BATTLE_FIELD = GetMaxMonsterCountInThisBattleField( iPlayerCount );
		iCreateMonsterCount = GetMaxNumberMonster(); // ���� �ʵ忡�� �����ϴ� �ִ� ���� ��!

		// ���� �� ���� �� ���� �������� ����� �ִ밪�� �� ũ�ٸ� �׸�ŭ ���͸� �� ��������!
		//if( MAX_MONSTER_COUNT_IN_BATTLE_FIELD > kCreateMonsterCountInfo.GetTotalMonsterCount() )
		//{
		//	iCreateMonsterCount += ( MAX_MONSTER_COUNT_IN_BATTLE_FIELD - kCreateMonsterCountInfo.GetTotalMonsterCount() );
		//}
	}


	//////////////////////////////////////////////////////////////////////////
	// ���� �߰� �Ǵ� ����
	
	// �����ؾ��ϴ� ���� ���� 0�̵ɶ����� loop�� ����!
	while( 0 <= iCreateMonsterCount )
	{
		if( m_vecRespawnNpcGroupList.empty() == true )
		{
			START_LOG( cerr, L"���� ������ �Ϸ��� �ϴµ� NPC �׷��� ����ֽ��ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( m_vecRespawnNpcGroupList.size() )
				<< BUILD_LOG( iCreateMonsterCount )
				<< END_LOG;
			break;
		}

		bool bCreatedMonsterAnyGroup = false;

		BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecRespawnNpcGroupList )
		{
			// �����ؾ��� ���� ���� ���ٸ� ���⼭ ������!
			if( iCreateMonsterCount <= 0 )
				break;

			// ���� ���� �׷��̸� ��������!
			if( sSpawnNpcGroup.m_bIsBossMonsterGroup )
				continue;

			KNPCUnitReq kInfo;
			if( CreateMonsterFromSpawnNpcGroup( sSpawnNpcGroup, kCreateMonsterCountInfo, kInfo ) == true )
			{
				// ���� ���� �����ߴٸ� ����Ʈ�� ����!
				vecCreateNPCList.push_back( kInfo );

				// ��ü ī��Ʈ���� �������!
				kCreateMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );

				// �����ؾ��� ���� ������ �ϳ� ������!
				--iCreateMonsterCount;

				// � �׷��̵� �Ѹ����� ���Ͱ� �����Ǿ���.
				bCreatedMonsterAnyGroup = true;
			}
		}

		// ���̻� ������ ���� �׷��� ���� �ʾҴٸ� �����!
		if( bCreatedMonsterAnyGroup == false )
			break;
	}

	return true;
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
bool CXSLBattleField::GetCreateBossMonsterList( OUT std::vector<KNPCUnitReq>& vecCreateNPCList ) const
{
	vecCreateNPCList.clear();

	//////////////////////////////////////////////////////////////////////////
	// �ʵ峻�� �ּ������� �����Ǿ� �־�� �ϴ� ���� ����!

	CXSLBattleField::KBattleFieldMonsterCountInfo kCreateMonsterCountInfo;


	//////////////////////////////////////////////////////////////////////////
	// ������ ���� �� ���ϱ�!
	int iCreateMonsterCount = 0;

	{
		// ���� �ʵ忡�� �ִ� ���� ������ ���� �� : ������� ����� ���ͼ����� �ʵ� �ִ� ���ͼ��� �� ũ�ٸ� �ʵ� �ִ� ���ͼ��� �����Ѵ�.
		//const int MAX_MONSTER_COUNT_IN_BATTLE_FIELD = GetMaxMonsterCountInThisBattleField( iPlayerCount );
		iCreateMonsterCount = GetMaxNumberMonster(); // ���� �ʵ忡�� �����ϴ� �ִ� ���� ��!

		// ���� �� ���� �� ���� �������� ����� �ִ밪�� �� ũ�ٸ� �׸�ŭ ���͸� �� ��������!
		//if( MAX_MONSTER_COUNT_IN_BATTLE_FIELD > kCreateMonsterCountInfo.GetTotalMonsterCount() )
		//{
		//	iCreateMonsterCount += ( MAX_MONSTER_COUNT_IN_BATTLE_FIELD - kCreateMonsterCountInfo.GetTotalMonsterCount() );
		//}
	}


	//////////////////////////////////////////////////////////////////////////
	// ���� �߰� �Ǵ� ����

	// �����ؾ��ϴ� ���� ���� 0�̵ɶ����� loop�� ����!
	while( 0 <= iCreateMonsterCount )
	{
		if( m_vecBossNpcGroupList.empty() == true )
		{
			START_LOG( cerr, L"���� ������ �Ϸ��� �ϴµ� NPC �׷��� ����ֽ��ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( m_vecBossNpcGroupList.size() )
				<< BUILD_LOG( iCreateMonsterCount )
				<< END_LOG;
			break;
		}

		bool bCreatedMonsterAnyGroup = false;

		BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecBossNpcGroupList )
		{
			// �����ؾ��� ���� ���� ���ٸ� ���⼭ ������!
			if( iCreateMonsterCount <= 0 )
				break;

			KNPCUnitReq kInfo;
			if( CreateMonsterFromSpawnNpcGroup( sSpawnNpcGroup, kCreateMonsterCountInfo, kInfo ) == true )
			{
				kInfo.m_cMonsterGrade = CXSLUnitManager::MG_BOSS_NPC;

				// ���� ���� �����ߴٸ� ����Ʈ�� ����!
				vecCreateNPCList.push_back( kInfo );

				// ��ü ī��Ʈ���� �������!
				kCreateMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );

				// �����ؾ��� ���� ������ �ϳ� ������!
				--iCreateMonsterCount;

				// � �׷��̵� �Ѹ����� ���Ͱ� �����Ǿ���.
				bCreatedMonsterAnyGroup = true;
			}
		}

		// ���̻� ������ ���� �׷��� ���� �ʾҴٸ� �����!
		if( bCreatedMonsterAnyGroup == false )
			break;
	}

	return true;
}
#endif // SERV_BATTLE_FIELD_BOSS


bool CXSLBattleField::GetRespawnMonsterList( IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterCountInfo,
											 IN const std::map< int, int >& mapRespawnReadyNpcCount,
											 OUT std::vector< KNPCUnitReq >& vecCreateNPCList ) const
{
	vecCreateNPCList.clear();

	KBattleFieldMonsterCountInfo kTotalMonsterCountInfo = kAliveMonsterCountInfo; // ��ü ���� ���� ����ϱ� ���� ���޹��� ����ִ� ���� �� ��ü�� �����մϴ�.


	// �׷캰�� ���� ����!
	//BOOST_TEST_FOREACH( const SSpawnNpcGroup&, sSpawnNpcGroup, m_vecNpcGroupList )
	//{
	//	int iCreateMonsterCount = 0;
	//	int iAliveGroupMonsterCount = 0;

	//	if( kCreateMonsterCountInfo.GetGroupMonsterCount( sSpawnNpcGroup.m_iGroupID, iAliveGroupMonsterCount ) == false )
	//	{
	//		// �ش� �׷쿡 ������ ���Ͱ� �Ѹ����� ������ �ּҰ��� �ʿ��� ���� ���� ���� ����!
	//		iCreateMonsterCount = sSpawnNpcGroup.m_iMinNumberOfMonsterInThisGroup;
	//	}
	//	else
	//	{
	//		// ���� ����ִ� ���� ���� �ּһ������ͼ����� ������ �� ���� �ϵ��� ī��Ʈ�� �ø���!
	//		if( iAliveGroupMonsterCount < sSpawnNpcGroup.m_iMinNumberOfMonsterInThisGroup )
	//		{
	//			iCreateMonsterCount = sSpawnNpcGroup.m_iMinNumberOfMonsterInThisGroup - iAliveGroupMonsterCount;
	//		}
	//	}

	//	// ������ �ʿ��� ���ͼ��� 0�̸� ���� �׷����� ����!
	//	if( iCreateMonsterCount <= 0 )
	//		continue;

	//	// �ʿ��� ���ڸ�ŭ ���� ����!
	//	for( int iCount = 0; iCount < iCreateMonsterCount; ++iCount )
	//	{
	//		KNPCUnitReq kInfo;
	//		if( CreateMonsterFromSpawnNpcGroup( sSpawnNpcGroup, kCreateMonsterCountInfo, kInfo ) == true )
	//		{
	//			// ���� ���� �����ߴٸ� ����Ʈ�� ����!
	//			vecCreateNPCList.push_back( kInfo );

	//			// ��ü ī��Ʈ���� �������!
	//			kCreateMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );
	//		}
	//	}
	//}

	// ���� ������!
	std::map< int, int >::const_iterator mit;
	for( mit = mapRespawnReadyNpcCount.begin(); mit != mapRespawnReadyNpcCount.end(); ++mit )
	{
		const int iGroupID = mit->first;
		const int iRespawnCount = mit->second;

		const SSpawnNpcGroup* pSpawnNpcGroup = GetSpawnNpcGroup( iGroupID );
		if( IS_NULL( pSpawnNpcGroup ) )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ���� �׷��ε� ������ �Ϸ��� �Ͽ����ϴ�." )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( iGroupID )
				<< END_LOG;
			continue;
		}

		for( int iCount = 0; iCount < iRespawnCount; ++iCount )
		{
			KNPCUnitReq kInfo;
			if( CreateMonsterFromSpawnNpcGroup( *pSpawnNpcGroup, kTotalMonsterCountInfo, kInfo ) == true )
			{
				// ���� ���� �����ߴٸ� ����Ʈ�� ����!
				vecCreateNPCList.push_back( kInfo );

				// ��ü ī��Ʈ���� �������!
				kTotalMonsterCountInfo.AddGroupMonsterCount( kInfo.m_iGroupID, kInfo.m_NPCID );
			}
			else
			{
				START_LOG( cerr, L"���� ������ ����! ���� �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( m_eBattleFieldID )
					<< BUILD_LOG( iGroupID )
					<< END_LOG;
			}
		}
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////



int	CXSLBattleField::GetMaxMonsterCountInThisBattleField( IN const int iPlayerCount ) const
{
	const int MAX_MONSTER_COUNT_BY_USER_COUNT = iPlayerCount * GetMonsterRespawnFactorByUserCount();
    return ( MAX_MONSTER_COUNT_BY_USER_COUNT < GetMaxNumberMonster() ? GetMaxNumberMonster() : MAX_MONSTER_COUNT_BY_USER_COUNT );
}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
bool CXSLBattleField::GetBattieFieldMiddleBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo )
{
	KLottery kMiddleBossLot;

	BOOST_TEST_FOREACH( const SRiskInfo&, sRandomInfo, m_vecMiddleBossInfo )
	{
		if( kMiddleBossLot.AddCase( sRandomInfo.m_iSpawnID, static_cast<float>(sRandomInfo.m_iSpawnRate) ) == false )
		{
			START_LOG( cerr, L"���� ���� Ȯ�� ������ ������ �����!" )
				<< BUILD_LOG( sRandomInfo.m_iSpawnID )
				<< BUILD_LOG( sRandomInfo.m_iSpawnRate )
				<< END_LOG;
		}
	}

	if( kMiddleBossLot.GetCaseNum() == 0 )
	{
		START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;

		return false;
	}

	// 3. ������ ���͸� ��������!
	int iSpawnID = kMiddleBossLot.Decision();
	if( iSpawnID == KLottery::CASE_BLANK )
	{
		START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;

		return false;
	}

	BOOST_TEST_FOREACH( const SRiskInfo&, sRiskInfo, m_vecMiddleBossInfo )
	{
		// �������� ã�� �߰����� Spwan Monster ID �� ã�´�
		if( sRiskInfo.m_iSpawnID != iSpawnID )
			continue;

		if( sRiskInfo.m_iRiskValue > iDangerousValue )			
			continue;

		BOOST_TEST_FOREACH( const int, iSpawnGroupID, sRiskInfo.m_vecSpawnGroupID )
		{
			const CXSLBattleField::SSpawnNpcGroup* pNpcSpawnGroup = GetSpawnNpcGroup( iSpawnGroupID );
			if( IS_NULL( pNpcSpawnGroup ) )
			{
				START_LOG( cerr, L"���� ���Ͱ� ������ ���� �׷��� �����ϴ�!" )
					<< BUILD_LOG( iSpawnGroupID )
					<< END_LOG;
				return false;
			}

			// ������ �ߺ��� �����Ǿ��ٸ� ���� ������ ����!
			std::map< int, SSpawnNpcInfo >::const_iterator mitSNG = pNpcSpawnGroup->m_mapSpawnNpcList.begin();
			for( ; mitSNG != pNpcSpawnGroup->m_mapSpawnNpcList.end() ; ++mitSNG )
			{
				KNPCUnitReq kInfo;

				// RATE �� ���ؼ� �Ѹ����� ���� ��Ű���� ����
				KLottery kNpcLot;
				kNpcLot.AddCase( mitSNG->first, mitSNG->second.m_fRate );
				int iNpcID = kNpcLot.Decision();
				if( iNpcID == KLottery::CASE_BLANK )
					continue;
				
				if( MakeMiddleBossMonsterInfo( mitSNG->second, pNpcSpawnGroup->IsSiegeMonster( mitSNG->first ), sRiskInfo, kInfo ) == false )
				{
					START_LOG( cerr, L"�߰� ���� ���� ������ �������� ���Ͽ����ϴ�!" )
						<< BUILD_LOG( iSpawnGroupID )
						<< END_LOG;
					return false;
				}

				vecNpcInfo.push_back( kInfo );
			}
		}

		return true;
	}

#ifdef DELETE_INCORRECT_ERROR_LOG
	// �߰� ���� ���͸� �ǵ��ؼ� �������� ���� �Ϳ� ���� ����ó��
	if( true == m_vecMiddleBossInfo.empty() )
	{
		switch ( m_eBattleFieldID )
		{
		case SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01:
			{
				return true;
			} break;
		default:
			break;
		}
	}
#endif // DELETE_INCORRECT_ERROR_LOG

	START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;

	return false;
}

bool CXSLBattleField::MakeMiddleBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const
{
	kInfo.Init(); // �ǵ������� �ʱ�ȭ ��Ų��!
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	kInfo.m_NPCID				= sSpawnNpcInfo.m_eNpcID;            
	kInfo.m_iGroupID			= 0;
	kInfo.m_iBossGroupID		= sRiskInfo.m_iSpawnID;		// Spawn Group ID �� �ƴ�, ���� �׷��� ���п� ID
	kInfo.m_Level				= m_iStandardMonsterLevel;  // ���� ���� ����
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	if( sSpawnNpcInfo.m_StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( sSpawnNpcInfo.m_vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"�ߺ� ���� ������ġ�� ���� ���߽��ϴ�!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( kInfo.m_iGroupID )
				<< BUILD_LOG( kInfo.m_iBossGroupID )
				<< BUILD_LOG( kInfo.m_NPCID )
				<< BUILD_LOG( sSpawnNpcInfo.m_vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= sSpawnNpcInfo.m_StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= sSpawnNpcInfo.m_vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= sSpawnNpcInfo.m_vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	kInfo.m_bShowGage			= true;
	kInfo.m_AddPos.y			= sSpawnNpcInfo.m_fAddPosY;
	kInfo.m_bShowBossName		= sRiskInfo.m_bShowBossName;
	kInfo.m_usBossGaugeHPLines	= sRiskInfo.m_bBossGaugeHpLines;
	kInfo.m_bFocusCamera		= sSpawnNpcInfo.m_bFocusCamera;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowSubBossName;
	kInfo.m_bSiegeMode			= bSiegeMode;
	kInfo.m_bNoDrop				= !(sSpawnNpcInfo.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_MIDDLE_BOSS_NPC;
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
bool CXSLBattleField::GetBattieFieldEventBossMonsterInfo( IN const int iDangerousValue, OUT std::vector<KNPCUnitReq>& vecNpcInfo )
{
	KLottery kEventBossLot;

	BOOST_TEST_FOREACH( const SRiskInfo&, sRandomInfo, m_vecEventBossInfo )
	{
		if( kEventBossLot.AddCase( sRandomInfo.m_iSpawnID, static_cast<float>(sRandomInfo.m_iSpawnRate) ) == false )
		{
			START_LOG( cerr, L"���� ���� Ȯ�� ������ ������ �����!" )
				<< BUILD_LOG( sRandomInfo.m_iSpawnID )
				<< BUILD_LOG( sRandomInfo.m_iSpawnRate )
				<< END_LOG;
		}
	}

	if( kEventBossLot.GetCaseNum() == 0 )
	{
		START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;

		return false;
	}

	// 3. ������ ���͸� ��������!
	int iSpawnID = kEventBossLot.Decision();
	if( iSpawnID == KLottery::CASE_BLANK )
	{
		START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( iDangerousValue )
			<< END_LOG;

		return false;
	}

	BOOST_TEST_FOREACH( const SRiskInfo&, sRiskInfo, m_vecEventBossInfo )
	{
		// �������� ã�� �߰����� Spwan Monster ID �� ã�´�
		if( sRiskInfo.m_iSpawnID != iSpawnID )
			continue;

		if( sRiskInfo.m_iRiskValue > iDangerousValue )			
			continue;

		BOOST_TEST_FOREACH( const int, iSpawnGroupID, sRiskInfo.m_vecSpawnGroupID )
		{
			const CXSLBattleField::SSpawnNpcGroup* pNpcSpawnGroup = GetSpawnNpcGroup( iSpawnGroupID );
			if( IS_NULL( pNpcSpawnGroup ) )
			{
				START_LOG( cerr, L"���� ���Ͱ� ������ ���� �׷��� �����ϴ�!" )
					<< BUILD_LOG( iSpawnGroupID )
					<< END_LOG;
				return false;
			}

			// ������ �ߺ��� �����Ǿ��ٸ� ���� ������ ����!
			std::map< int, SSpawnNpcInfo >::const_iterator mitSNG = pNpcSpawnGroup->m_mapSpawnNpcList.begin();
			for( ; mitSNG != pNpcSpawnGroup->m_mapSpawnNpcList.end() ; ++mitSNG )
			{
				KNPCUnitReq kInfo;

				// RATE �� ���ؼ� �Ѹ����� ���� ��Ű���� ����
				KLottery kNpcLot;
				kNpcLot.AddCase( mitSNG->first, mitSNG->second.m_fRate );
				int iNpcID = kNpcLot.Decision();
				if( iNpcID == KLottery::CASE_BLANK )
					continue;

				if( MakeEventBossMonsterInfo( mitSNG->second, pNpcSpawnGroup->IsSiegeMonster( mitSNG->first ), sRiskInfo, kInfo ) == false )
				{
					START_LOG( cerr, L"�߰� ���� ���� ������ �������� ���Ͽ����ϴ�!" )
						<< BUILD_LOG( iSpawnGroupID )
						<< END_LOG;
					return false;
				}

				vecNpcInfo.push_back( kInfo );
			}
		}

		return true;
	}

	START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;

	return false;
}

bool CXSLBattleField::MakeEventBossMonsterInfo( IN const SSpawnNpcInfo& sSpawnNpcInfo, IN const bool bSiegeMode, IN const SRiskInfo& sRiskInfo, OUT KNPCUnitReq& kInfo ) const
{
	kInfo.Init(); // �ǵ������� �ʱ�ȭ ��Ų��!
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	kInfo.m_NPCID				= sSpawnNpcInfo.m_eNpcID;            
	kInfo.m_iGroupID			= 0;
	kInfo.m_iBossGroupID		= sRiskInfo.m_iSpawnID;		// Spawn Group ID �� �ƴ�, ���� �׷��� ���п� ID
	kInfo.m_Level				= m_iStandardMonsterLevel;  // ���� ���� ����
	kInfo.m_bAggressive			= sSpawnNpcInfo.m_bAggressive;
	if( sSpawnNpcInfo.m_StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( sSpawnNpcInfo.m_vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"�ߺ� ���� ������ġ�� ���� ���߽��ϴ�!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( kInfo.m_iGroupID )
				<< BUILD_LOG( kInfo.m_iBossGroupID )
				<< BUILD_LOG( kInfo.m_NPCID )
				<< BUILD_LOG( sSpawnNpcInfo.m_vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= sSpawnNpcInfo.m_StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= sSpawnNpcInfo.m_vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= sSpawnNpcInfo.m_vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	kInfo.m_bShowGage			= true;
	kInfo.m_AddPos.y			= sSpawnNpcInfo.m_fAddPosY;
	kInfo.m_bShowBossName		= sRiskInfo.m_bShowBossName;
	kInfo.m_usBossGaugeHPLines	= sRiskInfo.m_bBossGaugeHpLines;
	kInfo.m_bFocusCamera		= sSpawnNpcInfo.m_bFocusCamera;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowSubBossName;
	kInfo.m_bSiegeMode			= bSiegeMode;
	kInfo.m_bNoDrop				= !(sSpawnNpcInfo.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_EVENT_BOSS_NPC;
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

bool CXSLBattleField::GetBattieFieldBossMonsterInfo( IN const int iDangerousValue, OUT KNPCUnitReq& kNpcInfo )
{
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// �ߺ� �۾� �ϸ鼭 ���� �Ǿ����� ���� ���̵� �߰� �� ���� �ؾ���
#else
	BOOST_TEST_FOREACH( const SRiskInfo&, sRiskInfo, m_vecRiskInfo )
	{
		if( sRiskInfo.m_iRiskValue > iDangerousValue )			
			continue;

		const CXSLBattleField::SSpawnNpcGroup* pNpcSpawnGroup = GetSpawnNpcGroup( sRiskInfo.m_iSpawnGroupID );
		if( IS_NULL( pNpcSpawnGroup ) )
		{
			START_LOG( cerr, L"���� ���Ͱ� ������ ���� �׷��� �����ϴ�!" )
				<< BUILD_LOG( sRiskInfo.m_iSpawnGroupID )
				<< END_LOG;
			return false;
		}

		// ���⼭ ������ų �׷��� ��������Ѵ�!
		if( MakeMonsterInfo( *pNpcSpawnGroup, sRiskInfo.m_iSpawnMonsterID, kNpcInfo ) == false )
		{
			START_LOG( cerr, L"���� ���� ������ �������� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( sRiskInfo.m_iSpawnMonsterID )
				<< END_LOG;
			return false;
		}

		kNpcInfo.m_NPCID				= sRiskInfo.m_iSpawnMonsterID;
		kNpcInfo.m_bShowBossName		= sRiskInfo.m_bShowBossName;
		kNpcInfo.m_usBossGaugeHPLines	= sRiskInfo.m_bBossGaugeHpLines;
		kNpcInfo.m_bFocusCamera			= true;
		kNpcInfo.m_cMonsterGrade		= CXSLUnitManager::MG_BOSS_NPC; // ������ ����
		return true;
	}

	START_LOG( clog, L"���赵�� ���� ���� ���� ������ ã�� ���Ͽ����ϴ�!" )
		<< BUILD_LOG( iDangerousValue )
		<< END_LOG;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	return false;
}

int	CXSLBattleField::GetMonsterStartPosByRandom( IN const std::vector< int >& vecLineMapIndexList ) const
{
	KLotterySameProb< int > kLotStartPosRandom;

    BOOST_TEST_FOREACH( const int, iLineMapIndex, vecLineMapIndexList )
	{
		std::map< int, std::vector< int > >::const_iterator mit;
		mit = m_mapLineIndexRandomStartPos.find( iLineMapIndex );
		if( mit == m_mapLineIndexRandomStartPos.end() )
		{
			// ������ �ƴ϶� �������� ��Ȳ�Դϴ�.
			START_LOG( clog, L"�ش� ���θ� �ε������� StartPos ������ �����ϴ�." )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( iLineMapIndex )
				<< END_LOG;
			continue;
		}

		BOOST_TEST_FOREACH( const int, iStartPos, mit->second )
		{
			kLotStartPosRandom.AddCaseSameProb( iStartPos );
		}
	}

	if( kLotStartPosRandom.Empty() == true )
	{
		START_LOG( cerr, L"���� ������� �����ϴ�. �Ͼ���� �ȵǴ� ����! ������ ���� ��ġ�� �����ϰ� �����˴ϴ�!!" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( vecLineMapIndexList.size() )
			<< END_LOG;
		return -1;
	}

	return kLotStartPosRandom.DecisionSameProb();
}

bool CXSLBattleField::GetMonsterStartPosInfo( IN const int iStartPos, OUT VECTOR3& vPos, OUT bool& bRight ) const
{
	std::map< int, SMonsterStartPos >::const_iterator mit;
	mit = m_mapMonsterStartPos.find( iStartPos );
	if( mit == m_mapMonsterStartPos.end() )
	{
		START_LOG( cerr, L"���������ʴ� Start Position�Դϴ�" )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( iStartPos )
			<< END_LOG;
		return false;
	}

	vPos = mit->second.m_vPos;
	bRight = mit->second.m_bRight;
	return true;
}

const CXSLBattleField::SSpawnNpcGroup* CXSLBattleField::GetSpawnNpcGroup( IN const int iGroupID ) const
{
	std::map< int, SSpawnNpcGroup >::const_iterator mit;
	mit = m_mapNpcGroupList.find( iGroupID );
	if( mit == m_mapNpcGroupList.end() )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ���� �׷��Դϴ�." )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;
		return NULL;
	}

	return &mit->second;
}

bool CXSLBattleField::CreateMonsterFromSpawnNpcGroup( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const CXSLBattleField::KBattleFieldMonsterCountInfo& kMonsterCountInfo, OUT KNPCUnitReq& kInfo ) const
{
	KLottery kMonsterLot;

	// 1. �� ���͵鿡 ���� ���� ������ �����ϴ°� �ִ��� Ȯ��!
	{
		std::map< int, SSpawnNpcInfo >::const_iterator mitSN;
		for( mitSN = sSpawnNpcGroup.m_mapSpawnNpcList.begin(); mitSN != sSpawnNpcGroup.m_mapSpawnNpcList.end(); ++mitSN )
		{
			const SSpawnNpcInfo& sSpawnNpcInfo = mitSN->second;

			// ����ִ� ���� ������ �ش� ���� GroupID������ ���� �ϴ°�?
			int iAliveMonsterCount = 0;
			if( kMonsterCountInfo.GetMonsterCountByGroupIDAndNpcID( sSpawnNpcGroup.m_iGroupID, sSpawnNpcInfo.m_eNpcID, iAliveMonsterCount ) == true )
			{
				// �ش� ������ �ִ� ���� �������� �����ߴ��� Ȯ���ϱ�! (���� ���� MAX�� ���� 1����)
				if( iAliveMonsterCount >= sSpawnNpcInfo.m_iMaxNumberOfThisMonster )
					continue;
			}

			// �ش� �׷쿡 ���� ���Ͱ� ��� ���!
			int iAliveGroupCount = 0;
			if( kMonsterCountInfo.GetGroupMonsterCount( sSpawnNpcGroup.m_iGroupID, iAliveGroupCount ) == true )
			{
				// �ش� �׷쿡�� �ִ� ���� �������� �����ߴ��� Ȯ���ϱ�! (���� ���� MAX�� ���� 2����)
				if( iAliveGroupCount >= sSpawnNpcGroup.m_iMaxNumberOfMonsterInThisGroup )
					continue;
			}

			// ���� ���� Ȯ���� ����!
			if( kMonsterLot.AddCase( static_cast<int>(sSpawnNpcInfo.m_eNpcID), sSpawnNpcInfo.m_fRate ) == false )
			{
                START_LOG( cerr, L"���� ���� Ȯ�� ������ ������ �����!" )
					<< BUILD_LOG( mitSN->first )
					<< BUILD_LOG( sSpawnNpcInfo.m_eNpcID )
					<< BUILD_LOG( sSpawnNpcInfo.m_fRate )
					<< END_LOG;
			}
		}
	}

	// 2. ������ �� �ִ� ���� ���� �Ѿ����� üũ!
	if( kMonsterLot.GetCaseNum() == 0 )
		return false;

	const float fExProb = ( 100.f - kMonsterLot.GetTotalProb() );
	const float fAddProb = fExProb / static_cast<float>(kMonsterLot.GetCaseNum());
	if( fAddProb > 0.f )
	{
		kMonsterLot.AddProbability( fAddProb );
	}

	// 3. ������ ���͸� ��������!
	int iResultNpcID = kMonsterLot.Decision();
	if( iResultNpcID == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"���� ���� ������ �����ߴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iResultNpcID )
			<< BUILD_LOG( kMonsterLot.GetTotalProb() )
			<< END_LOG;

		//////////////////////////////////////////////////////////////////////////		
		// �� ���� ���� ���̽� ���� ���� ������ ��������!
		KLotterySameProb< int > kSameMonsterLot;

		std::vector< int > vecCaseList;
		kMonsterLot.GetCaseList( vecCaseList );

		BOOST_TEST_FOREACH( const int, iCaseID, vecCaseList )
		{
			kSameMonsterLot.AddCaseSameProb( iCaseID );
		}
        
		iResultNpcID = kSameMonsterLot.DecisionSameProb();
		if( iResultNpcID == KLottery::CASE_BLANK )
		{
			START_LOG( cerr, L"���� ���� ������ ������ �����ߴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iResultNpcID )
				<< BUILD_LOG( kMonsterLot.GetCaseNum() )
				<< END_LOG;
			return false;
		}
	}

	// 4. ������ ���Ͱ� �����Ǿ��ٸ� ���� ������ ����!
	if( MakeMonsterInfo( sSpawnNpcGroup, iResultNpcID, kInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� ����!" )
			<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
			<< BUILD_LOG( iResultNpcID )
			<< END_LOG;
		return false;
	}

	return true;
}

bool CXSLBattleField::MakeMonsterInfo( IN const SSpawnNpcGroup& sSpawnNpcGroup, IN const int iNpcID, OUT KNPCUnitReq& kInfo ) const
{
	// 4. ������ ���Ͱ� �����Ǿ��ٸ� ���� ������ ����!
	const CXSLUnitManager::NPCUnitTemplet* pNpcTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) );
	if( pNpcTemplet == NULL )
	{
		START_LOG( cerr, L"���������ʴ� NpcID�Դϴ�!" )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;
		return false;
	}

	std::map< int, SSpawnNpcInfo >::const_iterator mitSNG;
	mitSNG = sSpawnNpcGroup.m_mapSpawnNpcList.find( iNpcID );
	if( mitSNG == sSpawnNpcGroup.m_mapSpawnNpcList.end() )
	{
		START_LOG( cerr, L"��ϵ������� NpcID�Դϴ�!" )
			<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;
		return false;
	}

	kInfo.Init(); // �ǵ������� �ʱ�ȭ ��Ų��!
	kInfo.m_iGroupID			= sSpawnNpcGroup.m_iGroupID;
	kInfo.m_NPCID				= iNpcID;
	kInfo.m_Level				= m_iStandardMonsterLevel + mitSNG->second.m_iAddLevel; // ���� ���� ���� + �߰� ����
	kInfo.m_bAggressive			= mitSNG->second.m_bAggressive;
	if( mitSNG->second.m_StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( mitSNG->second.m_vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"������ ���� ������ġ�� ���� ���߽��ϴ�!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( sSpawnNpcGroup.m_iGroupID )
				<< BUILD_LOG( iNpcID )
				<< BUILD_LOG( mitSNG->second.m_vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= mitSNG->second.m_StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= mitSNG->second.m_vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= mitSNG->second.m_vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	//kInfo.m_bFocusCamera;
	kInfo.m_bShowGage			= true;
	kInfo.m_AddPos.y			= mitSNG->second.m_fAddPosY;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowBossName;
	//kInfo.m_bShowSubBossName;
	kInfo.m_bSiegeMode			= sSpawnNpcGroup.IsSiegeMonster( iNpcID );
	kInfo.m_bNoDrop				= !(mitSNG->second.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_NORMAL_NPC; // �ϴ� �Ϲ�NPC�� ����
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}

bool CXSLBattleField::IsCanEnterBattleField( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iRequireLevel, OUT int& iRequireDungeonID ) const
{
	SET_ERROR( NET_OK );

	iRequireLevel = 0;
	iRequireDungeonID = 0;

    // ���� ���� ���� üũ
	if( static_cast<int>(ucLevel) < GetRequireLevel() )
	{
		iRequireLevel = GetRequireLevel();

		SET_ERROR( ERR_BATTLEFIELD_13 );
		return false;
	}

	// ���� Ŭ���� ���� üũ
	BOOST_TEST_FOREACH( const int, iDungeonID, m_setRequireDungeon )
	{
		if( mapDungeonClear.find( iDungeonID ) == mapDungeonClear.end() )
		{
            iRequireDungeonID = iDungeonID;

			SET_ERROR( ERR_BATTLEFIELD_14 );
			return false;
		}
	}

	return true;
}

//{{ 2013. 1. 1	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
bool CXSLBattleField::GetEventMonsterCreateInfo( OUT KNPCUnitReq& kInfo )
{
	// 4. ������ ���Ͱ� �����Ǿ��ٸ� ���� ������ ����!
	const CXSLUnitManager::NPCUnitTemplet* pNpcTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( static_cast<CXSLUnitManager::NPC_UNIT_ID>( kInfo.m_NPCID ) );
	if( pNpcTemplet == NULL )
	{
		START_LOG( cerr, L"���������ʴ� NpcID�Դϴ�!" )
			<< BUILD_LOG( kInfo.m_NPCID )
			<< END_LOG;
		return false;
	}

	if( m_vecRespawnNpcGroupList.empty() == true )
	{
		START_LOG( cerr, L"������ �� �ִ� NPC ������ �����ϴ�." )
			<< BUILD_LOG( m_eBattleFieldID )
			<< BUILD_LOG( m_sWorldID )
			<< END_LOG;
		return false;
	}

	unsigned int uiIndex = rand() % m_vecRespawnNpcGroupList.size();
	const SSpawnNpcGroup& sSpawnNpcGroup = m_vecRespawnNpcGroupList[uiIndex];
	
	uiIndex = rand() % sSpawnNpcGroup.m_mapSpawnNpcList.size();
	std::map<int, SSpawnNpcInfo>::const_iterator it = sSpawnNpcGroup.m_mapSpawnNpcList.begin();
	for( unsigned int i=0; i < uiIndex; ++i )
	{
		++it;
	}
	const SSpawnNpcInfo& sSpawnNpcInfo = it->second;
	const KLotterySameProb<int>&	StartPositionLot	= sSpawnNpcInfo.m_StartPositionLot;
	const std::vector<int>&			vecPetrolLineIndex	= sSpawnNpcInfo.m_vecPetrolLineIndex;
	const std::vector<int>&			vecPlayLineIndex	= sSpawnNpcInfo.m_vecPlayLineIndex;

	//kInfo.Init(); // �ǵ������� �ʱ�ȭ ��Ų��!
	//kInfo.m_iGroupID			= sSpawnNpcGroup.m_iGroupID;
	//kInfo.m_NPCID				= iNpcID;
	kInfo.m_Level				= m_iStandardMonsterLevel; // ���� ���� ����
	//kInfo.m_bAggressive			= mitSNG->second.m_bAggressive;
	if( StartPositionLot.Empty() == true )
	{
		kInfo.m_nStartPos		= GetMonsterStartPosByRandom( vecPetrolLineIndex );
		if( kInfo.m_nStartPos == -1 )
		{
			START_LOG( cerr, L"������ ���� ������ġ�� ���� ���߽��ϴ�!" )
				<< BUILD_LOG( m_eBattleFieldID )
				<< BUILD_LOG( kInfo.m_NPCID )
				<< BUILD_LOG( vecPetrolLineIndex.size() )
				<< BUILD_LOG( kInfo.m_nStartPos )
				<< END_LOG;
		}
	}
	else
	{
		kInfo.m_nStartPos		= StartPositionLot.DecisionSameProb();
	}
	LIF( GetMonsterStartPosInfo( kInfo.m_nStartPos, kInfo.m_vPos, kInfo.m_bIsRight ) );
	kInfo.m_vecPetrolLineIndex	= vecPetrolLineIndex;
	kInfo.m_vecPlayLineIndex	= vecPlayLineIndex;
	kInfo.m_bActive				= true;
	//kInfo.m_fDelayTime;
	//kInfo.m_KeyCode;
	//kInfo.m_bFocusCamera;
	kInfo.m_bShowGage			= true;
	//kInfo.m_AddPos.y			= mitSNG->second.m_fAddPosY;
	//kInfo.m_bHasBossGage;
	//kInfo.m_bShowBossName;
	//kInfo.m_bShowSubBossName;
	//kInfo.m_bSiegeMode			= sSpawnNpcGroup.IsSiegeMonster( iNpcID );
	//kInfo.m_bNoDrop				= !(mitSNG->second.m_bDrop);
	kInfo.m_fUnitScale			= 1.f;
	kInfo.m_cMonsterGrade		= CXSLUnitManager::MG_NORMAL_NPC; // �ϴ� �Ϲ�NPC�� ����
	kInfo.m_cTeamNum			= CXSLRoom::TN_MONSTER;
	kInfo.m_iAllyUID			= -1;
	return true;
}
#endif SERV_FIELD_EVENT_MONSTER
//}}
