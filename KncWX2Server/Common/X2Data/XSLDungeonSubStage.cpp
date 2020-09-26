#include "XSLDungeonSubStage.h"

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
#include <boost/foreach.hpp>
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef _CONVERT_VS_2010
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))
#endif _CONVERT_VS_2010

bool CXSLDungeonSubStage::SubStageData::LoadData( IN bool bScriptCheck
												, IN OUT KLuaManager& luaManager
												)
{
	//�� ���� ���������� ����� ���� �׷�
	LUA_GET_VALUE( luaManager, "MAIN_LINE_SET",			m_MainLineSet,			0	);
	LUA_GET_VALUE( luaManager, "START_LINE_SET",		m_StartLineSet,			-1	);
	LUA_GET_VALUE( luaManager, "END_LINE_SET",			m_EndLineSet,			-1	);	
	LUA_GET_VALUE_ENUM( luaManager, "GO_TYPE",			m_GoType, CXSLDungeonSubStage::GO_TYPE, CXSLDungeonSubStage::GT_NONE );

	//{{ 2010. 04. 26  ������	��д��� ����
#ifdef SERV_SECRET_HELL	
	LUA_GET_VALUE( luaManager, L"EXTRA_NPC_START_POS",	m_ExtraNpcStartPos,		-1	);
	LUA_GET_VALUE( luaManager, L"EXTRA_NPC_IS_LEFT",	m_bExtraNpcIsLeft,		false	);
#endif SERV_SECRET_HELL
	//}}

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	if( LoadCreatedNpcDropTimesData( luaManager ) == false )
	{
		return false;
	}
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
	if( LoadClearCondition( luaManager ) == false )
	{
		return false;
	}
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// ���볯¥: 2013-08-13
	if( LoadCreatedNpcExpRateData( luaManager ) == false )
	{
		return false;
	}
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

	//�� ���� ���������� ���� NPC ����Ÿ �ε�
	//{{ 2012. 12. 21  ���� ���� �׷� ���� ��ġ - ��μ�
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
	if( New_LoadNPCData( bScriptCheck, luaManager ) == false )
#else
	if( LoadNPCData( bScriptCheck, luaManager ) == false )
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
	//}}
	{
		return false;
	}

	return true;
}

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
int CXSLDungeonSubStage::SubStageData::GetNpcDropTimes( int iUnitID )
{
	std::vector<NPCDropData>::iterator vitNPCDropDataList;
	for( vitNPCDropDataList = m_NPCDropDataList.begin(); vitNPCDropDataList != m_NPCDropDataList.end(); ++vitNPCDropDataList )
	{
		if( vitNPCDropDataList->m_UnitID == iUnitID )
			return vitNPCDropDataList->m_iDropTimes;
	}

	return 0;
}
bool CXSLDungeonSubStage::SubStageData::LoadCreatedNpcDropTimesData( KLuaManager& luaManager )
{
	bool bRet = true;

	if( luaManager.BeginTable( "CREATED_NPC_DROP_TIMES" ) != E_FAIL )
	{
		for( int i = 1; luaManager.BeginTable( i ) != E_FAIL; ++i )
		{
			NPCDropData npcDropData;
			LUA_GET_VALUE_ENUM( luaManager, L"NPC_ID",	 		npcDropData.m_UnitID,			CXSLUnitManager::NPC_UNIT_ID, 	CXSLUnitManager::NUI_NONE	);
			LUA_GET_VALUE( luaManager, L"DROP_TIMES", 			npcDropData.m_iDropTimes,		0		);

			if( npcDropData.m_UnitID != CXSLUnitManager::NUI_NONE &&
				npcDropData.m_iDropTimes > 0 )
				m_NPCDropDataList.push_back( npcDropData );

			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	return bRet;
}
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// ���볯¥: 2013-08-13
float CXSLDungeonSubStage::SubStageData::GetNpcExpRate( int iUnitID )
{
	BOOST_TEST_FOREACH( NPCExpRateData, kNpcExpRateData, m_vecNPCExpRateDataList )
	{
		if( kNpcExpRateData.m_UnitID == iUnitID )
			return kNpcExpRateData.m_fExpRate;
	}

	return 1.f;
}
bool CXSLDungeonSubStage::SubStageData::LoadCreatedNpcExpRateData( KLuaManager& luaManager )
{
	bool bRet = true;

	if( luaManager.BeginTable( "NPC_EXP_RATE" ) != E_FAIL )
	{
		for( int i = 1; luaManager.BeginTable( i ) != E_FAIL; ++i )
		{
			NPCExpRateData kNpcExpRateData;
			LUA_GET_VALUE_ENUM( luaManager, L"NPC_ID",	 		kNpcExpRateData.m_UnitID,			CXSLUnitManager::NPC_UNIT_ID, 	CXSLUnitManager::NUI_NONE	);
			LUA_GET_VALUE( luaManager, L"EXP_RATE", 			kNpcExpRateData.m_fExpRate,		1.f		);

			if( kNpcExpRateData.m_UnitID != CXSLUnitManager::NUI_NONE &&
				kNpcExpRateData.m_fExpRate >= 0.f )
				m_vecNPCExpRateDataList.push_back( kNpcExpRateData );

			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	return bRet;
}
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

bool CXSLDungeonSubStage::SubStageData::FetchNPCData( IN bool bScriptCheck, KLuaManager& luaManager, CXSLDungeonSubStage::NPCData* pNPCData )
{
	LUA_GET_VALUE_ENUM( luaManager, L"NPC_ID",	 		pNPCData->m_UnitID,			CXSLUnitManager::NPC_UNIT_ID, 	CXSLUnitManager::NUI_NONE	);
	LUA_GET_VALUE( luaManager, L"KEY_CODE", 			pNPCData->m_KeyCode,		0		);
	LUA_GET_VALUE( luaManager, L"FOCUS_CAMERA",			pNPCData->m_bFocusCamera,	false	);

	if( pNPCData->m_UnitID == CXSLUnitManager::NUI_NONE )
	{
		START_LOG( cerr, L"NPC ID���� NONE�Դϴ�! ���� ���������� Ȯ�� �ʿ�!" );
		return false;
	}

	if( S_OK == luaManager.BeginTable( "START_POS" ) )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				pNPCData->m_vecStartPos.push_back( buf );
			}

			index++;
		}

		luaManager.EndTable();
	}
	else
	{
		START_LOG( cerr, L"Monster START_POS TABLE ���� ����.!" )
			<< BUILD_LOG( pNPCData->m_UnitID )
			<< BUILD_LOG( pNPCData->m_bSubNPC )
			<< END_LOG;
	}
	//LUA_GET_VALUE( luaManager, L"START_POS",			pNPCData->m_StartPos,		-1		);

	LUA_GET_VALUE( luaManager, L"ACTIVE",				pNPCData->m_bActive,		true	);
	LUA_GET_VALUE( luaManager, L"GAGE_BAR", 			pNPCData->m_bShowGage,		true	);

	LUA_GET_VALUE( luaManager, L"ADD_POS_X", 			pNPCData->m_AddPos.x,		0.0f	);
	LUA_GET_VALUE( luaManager, L"ADD_POS_Y", 			pNPCData->m_AddPos.y,		0.0f	);
	LUA_GET_VALUE( luaManager, L"ADD_POS_Z", 			pNPCData->m_AddPos.z,		0.0f	);

	LUA_GET_VALUE( luaManager, L"RATE", 				pNPCData->m_Rate,			100		);
	LUA_GET_VALUE( luaManager, L"LEVEL", 				pNPCData->m_Level,			0		);

	LUA_GET_VALUE( luaManager, L"HAVE_BOSS_GAGE",		pNPCData->m_bHasBossGage,	false	);
	LUA_GET_VALUE( luaManager, L"SHOW_BOSS_NAME",		pNPCData->m_bShowBossName,	false	);
	LUA_GET_VALUE( luaManager, L"SHOW_SUB_BOSS_NAME",	pNPCData->m_bShowSubBossName,	false	);

	LUA_GET_VALUE( luaManager, L"SIEGE_MODE",			pNPCData->m_bSiegeMode,		false	);
	//{{ 2007. 10. 26  ������  
	LUA_GET_VALUE( luaManager, L"IS_RIGHT",				pNPCData->m_bIsRight,		false	);
	//}}	
	LUA_GET_VALUE( luaManager, L"NO_DROP",				pNPCData->m_bNoDrop,		false	);

	LUA_GET_VALUE( luaManager, L"UNIT_SCALE",			pNPCData->m_fUnitScale,		-1.f	);

	//{{ 2009. 12. 18  ������	��������ġ����
	LUA_GET_VALUE_ENUM( luaManager, L"MONSTER_GRADE",	pNPCData->m_MonsterGrade,	CXSLUnitManager::MONSTER_GRADE, CXSLUnitManager::MG_NORMAL_NPC );
	//}}
	//{{ 2011. 01. 26	������	����Ʈ Ŭ���� ���� �߰�
#ifdef SERV_BOSS_GAUGE_HP_LINES
	LUA_GET_VALUE( luaManager, L"BOSS_GAUGE_HP_LINES",	pNPCData->m_BossGaugeHPLines, 1 );
#endif SERV_BOSS_GAUGE_HP_LINES
	//}}

	//{{ 2010. 03. 29  �տ���	���� �� ����
#ifdef SERV_ALLY_NPC
	LUA_GET_VALUE( luaManager, L"ALLY_NPC",				pNPCData->m_bAllyNpc,		false );
#endif SERV_ALLY_NPC

#ifdef SERV_MODIFY_DUNGEON_STAGING 
	LUA_GET_VALUE( luaManager, L"SHOW_BOSS_NAME_DELAY_TIME", pNPCData->m_fShowBossNameDelayTime,	-1.f	);
	if( -1 != pNPCData->m_fShowBossNameDelayTime )
	{
		pNPCData->m_bShowBossName = true;
	}

	LUA_GET_VALUE( luaManager, L"STOP_AT_START_STATE",	pNPCData->m_bStopAtStartState,	false );
	
#endif //SERV_MODIFY_DUNGEON_STAGING

	if( bScriptCheck )
	{
		if( pNPCData->m_bHasBossGage == true )
		{
			if( pNPCData->m_MonsterGrade != CXSLUnitManager::MG_MIDDLE_BOSS_NPC  &&
				pNPCData->m_MonsterGrade != CXSLUnitManager::MG_BOSS_NPC )
			{
				START_LOG( cerr, L"���� ������ ���� true�ε� MonsterType�� �Ϲݸ��ͷ� �Ǿ��ִ�!" )
					<< BUILD_LOG( pNPCData->m_UnitID )
					<< BUILD_LOGc( pNPCData->m_MonsterGrade )
					<< END_LOG;
				return false;
			}
		}
		else
		{
			if( pNPCData->m_MonsterGrade == CXSLUnitManager::MG_BOSS_NPC )
			{
				START_LOG( cerr, L"���� ������ ���� false�ε� MonsterType�� �������ͷ� �Ǿ��ִ�!" )
					<< BUILD_LOG( pNPCData->m_UnitID )
					<< BUILD_LOGc( pNPCData->m_MonsterGrade )
					<< END_LOG;
				return false;
			}
		}
	}

	return true;
}

bool CXSLDungeonSubStage::SubStageData::LoadNPCData( IN bool bScriptCheck, KLuaManager& luaManager )
{
	bool bRet = true;

	//{{ 2012. 12. 21  ���� ���� �׷� ���� ��ġ - ��μ�
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
	// NPC_GROUP_RATE �� �����Ƿ� ������ 0���� 100% Ȯ���� �����Ѵ�.
	int iGroupID = 0;
	m_kNpcGroupRate.AddCase( iGroupID, 100.f );
	std::vector< NPCData* > vecNPCList;
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
	//}}

	if( luaManager.BeginTable( "NPC_GROUP" ) != E_FAIL )
	{
		for( int i = 1; luaManager.BeginTable( i ) != E_FAIL; ++i )
		{
			bool bThereWasSubNPCScript	= false;


			for( int j=0; ; j++ )
			{
				char szTable[128] = "";
#ifdef _CONVERT_VS_2010
				sprintf_s( szTable, ARRAY_SIZE(szTable), "SUB_NPC%d", j );
#else
				sprintf( szTable, "SUB_NPC%d", j );
#endif _CONVERT_VS_2010
				if( S_OK == luaManager.BeginTable( szTable ) )
				{
					NPCData* pNPCData = new NPCData();

					pNPCData->m_bSubNPC			= true;
					pNPCData->m_iSubNPCIndex	= i;

					if( FetchNPCData( bScriptCheck, luaManager, pNPCData ) == false )	
						bRet = false;

					//{{ 2012. 12. 21  ���� ���� �׷� ���� ��ġ - ��μ�
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
					vecNPCList.push_back( pNPCData );
#else
					m_NPCDataList.push_back( pNPCData );
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
					//}}

					bThereWasSubNPCScript = true;

					{						
						LUA_GET_VALUE( luaManager, L"SUB_NPC_RATE", pNPCData->m_iSubNPCRate, 0 );
					}

					luaManager.EndTable();
				}
				else
				{
					break;
				}
			}

			if( false == bThereWasSubNPCScript )
			{
				NPCData* pNPCData = new NPCData();
				pNPCData->m_bSubNPC = false;
				if( FetchNPCData( bScriptCheck, luaManager, pNPCData ) == false ) 
					bRet = false;

				//{{ 2012. 12. 21  ���� ���� �׷� ���� ��ġ - ��μ�
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
				vecNPCList.push_back( pNPCData );
#else
				m_NPCDataList.push_back( pNPCData );
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
				//}}
			}
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	//{{ 2012. 12. 21  ���� ���� �׷� ���� ��ġ - ��μ�
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
	std::map< int, std::vector< NPCData* > >::iterator mit = m_mapNPCDataList.find( iGroupID );
	if( mit == m_mapNPCDataList.end() )
	{
		m_mapNPCDataList.insert( std::make_pair( iGroupID, vecNPCList ) );
	}
	else
	{
		// �����ϸ� �ȵǴµ� �����Ѵ�.
		// ����� NPC_GROUP �� �ϳ� ���϶��� ������ �Լ��̴�. �����Ѵٸ� �̻��ϴ�.
		START_LOG( cerr, L"�ߺ����� ���� �׷��� �����Ϸ� �մϴ�." )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( static_cast<int>(vecNPCList.size()) )
			<< END_LOG;

		bRet = false;
	}
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
	//}}

	return bRet;
}

//{{ 2012. 12. 21  ���� ���� �׷� ���� ��ġ - ��μ�
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
bool CXSLDungeonSubStage::SubStageData::New_LoadNPCData( IN bool bScriptCheck, KLuaManager& luaManager )
{
	bool bRet = true;

	// �� �ִ� ���� �ƴϴ�
	if( luaManager.BeginTable( "NPC_GROUP_RATE" ) == E_FAIL )
	{		
		// ������ ������ Load ������� �Ѵ�.  NPC_GROUP
		return LoadNPCData( bScriptCheck, luaManager );
		
	}

/*//////////////////////////////////////////////////////////////////////////////
	��ũ��Ʈ���� NPC_GROUP_RATE ���̺� ����
	NPC_GROUP_RATE
	{
		NPC_GROUP0		 = 30,
		NPC_GROUP1		 = 40,
		NPC_GROUP2		 = 30,
	}
*///////////////////////////////////////////////////////////////////////////////

	// NPC_GROUP_RATE �Ľ� ������
	char strRateTable[MAX_PATH] = "";

	for( int iGroupRateID = 0; ; ++iGroupRateID )
	{
#ifdef _CONVERT_VS_2010
		sprintf_s( strRateTable, ARRAY_SIZE(strRateTable), "NPC_GROUP_RATE%d", iGroupRateID );
#else
		sprintf( strRateTable, "NPC_GROUP_RATE%d", iGroupRateID );
#endif _CONVERT_VS_2010

		if( luaManager.BeginTable( strRateTable ) == E_FAIL )
			break;

		int index	= 1; 
		float fRate = 0.f;

		if( luaManager.GetValue( index, fRate ) == S_OK )
		{
			if( fRate <= 0.f )
			{
				START_LOG( cerr, L"NPC_GROUP_RATE ���� �̻��մϴ�." )
					<< BUILD_LOG( iGroupRateID )
					<< BUILD_LOG( fRate )
					<< END_LOG;
				break;
			}
		}
		else
		{
			START_LOG( cerr, L"NPC_GROUP_RATE ���� �������� �ʽ��ϴ�." )
				<< BUILD_LOG( iGroupRateID )
				<< BUILD_LOG( fRate )
				<< END_LOG;

			return false;
		}
		luaManager.EndTable();

		m_kNpcGroupRate.AddCase( iGroupRateID, fRate );
	}
	luaManager.EndTable();

	// TotalRate �� 100 �� ���� ������ ����
	if( m_kNpcGroupRate.GetTotalProb() < 100.f )
	{
		START_LOG( cerr, L"Total NPC_GROUP_RATE ���� �̻��մϴ�." )
			<< BUILD_LOG( m_kNpcGroupRate.GetTotalProb() )
			<< END_LOG;
	}

	// ������ �� Load ������� �Ѵ�.  NPC_GROUP0, NPC_GROUP1, NPC_GROUP2,.....
	char strTable[MAX_PATH] = "";

	// NPC_GROUP �Ľ� ������ (������ ���� �� �� ����)
	for( int iGroupID = 0; ; ++iGroupID )
	{
		// npc �����͸� ��Ƶ� �����̳� - ���߿� GroupID�� ��Ī���Ѽ� map�� �߰���
		std::vector<NPCData*> vecNPCDataList;

#ifdef _CONVERT_VS_2010
		sprintf_s( strTable, ARRAY_SIZE(strTable), "NPC_GROUP%d", iGroupID );
#else
		sprintf( strTable, "NPC_GROUP%d", iGroupID );
#endif _CONVERT_VS_2010
		if( luaManager.BeginTable( strTable ) == E_FAIL )
			break;

		for( int i = 1; luaManager.BeginTable( i ) != E_FAIL; ++i )
		{
			bool bThereWasSubNPCScript	= false;

			for( int j=0; ; j++ )
			{
				char szTable[128] = "";
#ifdef _CONVERT_VS_2010
				sprintf_s( szTable, ARRAY_SIZE(szTable), "SUB_NPC%d", j );
#else
				sprintf( szTable, "SUB_NPC%d", j );
#endif _CONVERT_VS_2010
				if( S_OK == luaManager.BeginTable( szTable ) )
				{
					NPCData* pNPCData = new NPCData();

					pNPCData->m_bSubNPC			= true;
					pNPCData->m_iSubNPCIndex	= i;

					if( FetchNPCData( bScriptCheck, luaManager, pNPCData ) == false )
						bRet = false;

					vecNPCDataList.push_back( pNPCData );

					bThereWasSubNPCScript = true;

					{						
						LUA_GET_VALUE( luaManager, L"SUB_NPC_RATE", pNPCData->m_iSubNPCRate, 0 );
					}

					luaManager.EndTable();
				}
				else
				{
					break;
				}
			}

			if( false == bThereWasSubNPCScript )
			{
				NPCData* pNPCData = new NPCData();
				pNPCData->m_bSubNPC = false;
				if( FetchNPCData( bScriptCheck, luaManager, pNPCData ) == false ) 
					bRet = false;

				vecNPCDataList.push_back( pNPCData );
			}
			luaManager.EndTable();
		}
		luaManager.EndTable();

		// map �� �����ϱ� ���� �ߺ����������� Ȯ���Ѵ�.
		std::map< int, std::vector< NPCData* > >::iterator mit = m_mapNPCDataList.find( iGroupID );
		if( mit == m_mapNPCDataList.end() )
		{
			m_mapNPCDataList.insert( std::make_pair( iGroupID, vecNPCDataList ) );
		}
		else
		{
			// �����ϸ� �ȵǴµ� �����Ѵ�.
			// ����� NPC_GROUP �� �ϳ� ���϶��� ������ �Լ��̴�. �����Ѵٸ� �̻��ϴ�.
			START_LOG( cerr, L"�ߺ����� ���� �׷��� �����Ϸ� �մϴ�." )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( static_cast<int>(vecNPCDataList.size()) )
				<< END_LOG;

			bRet = false;
		}
	}

	return bRet;
}
int CXSLDungeonSubStage::SubStageData::GetRandomNpcGruopID()
{
	int iGroupID = m_kNpcGroupRate.Decision();
	if( iGroupID < 0)
	{
		START_LOG( cerr, L"Stage �� �����ϴ� NPC Group ID ���� �̾Ƴ��� ���ߴ�? �Ľ��Ҷ� Ȯ�� �ߴµ�..." )
			<< BUILD_LOG( m_kNpcGroupRate.GetCaseNum() )
			<< BUILD_LOG( m_kNpcGroupRate.GetTotalProb() )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;		

		// �����ϸ� �Ľ̽� �ʱⰪ�� 0�� �����Ѵ�.
		return 0;
	}

	return iGroupID;
}
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
//}}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
int CXSLDungeonSubStage::SubStageData::GetSecretStageEnteringEvent( IN int iClearConditionIndex )
{
	if( ( iClearConditionIndex < 0 ) || ( m_vecClearCondData.size() <= static_cast<unsigned int>( iClearConditionIndex ) ) )
	{
		START_LOG( cerr, L"�������� �ʴ� CLEAR_COND �ε���. ��ŷ! �Ǵ� ����/Ŭ���� ������ �ٸ�" )
			<< BUILD_LOG( m_vecClearCondData.size() )
			<< BUILD_LOG( iClearConditionIndex )
			<< END_LOG;

		return 0;
	}

	std::vector<NextStageData>& vecNextStage = m_vecClearCondData[iClearConditionIndex].m_vecNextStage;
	if( vecNextStage.size() == 3 && vecNextStage[0].m_eClearType == CT_STAGE_SECRET )
	{
		if( vecNextStage[1].m_iStageIndex > 0 && vecNextStage[2].m_iStageIndex > 0 )
		{
			return 3;
		}
		else if( vecNextStage[2].m_iStageIndex > 0 )
		{
			return 2;
		}
		else if( vecNextStage[1].m_iStageIndex > 0 )
		{
			return 1;
		}
	}

	return 0;
}

bool CXSLDungeonSubStage::SubStageData::GetNextStage( OUT CXSLDungeonSubStage::NextStageData& kNextStageData, IN int iClearConditionIndex, IN int iSecretPadIndex )
{
	if( ( iClearConditionIndex < 0 ) || ( m_vecClearCondData.size() <= static_cast<unsigned int>( iClearConditionIndex ) ) )
	{
		START_LOG( cerr, L"�������� �ʴ� CLEAR_COND �ε���. ��ŷ! �Ǵ� ����/Ŭ���� ������ �ٸ�" )
			<< BUILD_LOG( m_vecClearCondData.size() )
			<< BUILD_LOG( iClearConditionIndex )
			<< END_LOG;

		return false;
	}

	std::vector<NextStageData>& vecNextStage = m_vecClearCondData[iClearConditionIndex].m_vecNextStage;
	if( vecNextStage.size() == 3 && vecNextStage[0].m_eClearType == CT_STAGE_SECRET )
	{
		// �ƿ� ��� �������� �����ϴ� ��찡 �����Ƿ� �⺻�� ��� �������� by ������
		switch( static_cast< CXSLDungeonSubStage::SECRET_STAGE_PAD >( iSecretPadIndex ) )
		{
		case CXSLDungeonSubStage::SSP_SECRET:
			{
				// ��� ��������
				kNextStageData = vecNextStage[1];
			} break;
		case CXSLDungeonSubStage::SSP_ELITE:
			{
				// ���� ��������
				kNextStageData = vecNextStage[2];
			} break;
		default:
			{
				// �Ϲ� ��������
				kNextStageData = vecNextStage[0];
			} break;
		}

		return true;
	}

	// �Ϲ� ��������
	int iRandomNumber = rand() % 100;
	int iCumulativeRate = 0;
	BOOST_FOREACH( NextStageData kNextStageDataInVector, vecNextStage )
	{
		iCumulativeRate += kNextStageDataInVector.m_iRate;
		if( iRandomNumber < iCumulativeRate )
		{
			// ����!
			kNextStageData = kNextStageDataInVector;
			return true;
		}
	}

	START_LOG( cerr, L"�� �������� �� Ȯ���� �� ������ �� 100�� �� ��" )
		<< BUILD_LOG( iCumulativeRate )
		<< BUILD_LOG( iRandomNumber )
		<< END_LOG;

	return false;
}
bool CXSLDungeonSubStage::SubStageData::LoadClearCondition( KLuaManager& luaManager )
{
	for( int iClearCondIndex = 0; ; ++iClearCondIndex )
	{
		char szTable[128] = "";
#ifdef _CONVERT_VS_2010// �۾���¥: 2013-10-31	// �ڼ���
		sprintf_s( szTable, ARRAY_SIZE( szTable ), "CLEAR_COND%d", iClearCondIndex );
#else // _CONVERT_VS_2010
		sprintf( szTable, "CLEAR_COND%d", iClearCondIndex );
#endif // _CONVERT_VS_2010
		if( E_FAIL == luaManager.BeginTable( szTable ) )
			break;

		ClearCondtionData clearCond;
		for( int iNextStageIndex = 0; ; ++iNextStageIndex )
		{
			char szTable[128] = "";
#ifdef _CONVERT_VS_2010// �۾���¥: 2013-10-31	// �ڼ���
			sprintf_s( szTable, ARRAY_SIZE( szTable ), "NEXT_STAGE%d", iNextStageIndex );
#else // _CONVERT_VS_2010
			sprintf( szTable, "NEXT_STAGE%d", iNextStageIndex );
#endif // _CONVERT_VS_2010
			if( E_FAIL == luaManager.BeginTable( szTable ) )
				break;

			int iValue = 0;
			luaManager.GetValue( 1, iValue );

			NextStageData nextStageData;
			nextStageData.m_eClearType = (CLEAR_TYPE)iValue;
			nextStageData.m_iStageIndex = -1;
			nextStageData.m_iSubStageIndex = -1;
			nextStageData.m_iRate = 100;
			nextStageData.m_iDungeonEndingSpeechTableIndex = 0;
			luaManager.GetValue( 2, nextStageData.m_iStageIndex );
			luaManager.GetValue( 3, nextStageData.m_iSubStageIndex );
			luaManager.GetValue( 4, nextStageData.m_iRate );
			luaManager.GetValue( 5, nextStageData.m_iDungeonEndingSpeechTableIndex );

			clearCond.m_vecNextStage.push_back( nextStageData );
			luaManager.EndTable(); // NEXT_STAGE(i)
		}
		m_vecClearCondData.push_back( clearCond );
		luaManager.EndTable(); // CLEAR_COND(i)	
	}

	return true;
}
#endif // SERV_STAGE_CLEAR_IN_SERVER
