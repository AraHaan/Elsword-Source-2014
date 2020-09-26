#include "StdAfx.h"
#include ".\x2dungeonmanager.h"

CX2DungeonManager::CX2DungeonManager()
{
}		



CX2DungeonManager::~CX2DungeonManager()
{
	DungeonDataIterator it;
	for( it=m_mapDungeonData.begin(); it!=m_mapDungeonData.end(); it++ )
	{
		SAFE_DELETE( it->second );
	}
	m_mapDungeonData.clear();
}


CX2Dungeon* CX2DungeonManager::CreateDungeon( SEnum::DUNGEON_ID dungeonID )
{
	DungeonDataIterator it = m_mapDungeonData.find( dungeonID );
	if( it != m_mapDungeonData.end() )
	{
		CX2Dungeon::DungeonData* pDungeonData = it->second;
		CX2Dungeon* pDungeon = new CX2Dungeon( pDungeonData );
		return pDungeon;
	}
	else
	{
		return NULL;
	}
}

CX2Dungeon* CX2DungeonManager::CreateDungeon( SEnum::DUNGEON_ID dungeonID, bool bIsNpcLoad )
{
	DungeonDataIterator it = m_mapDungeonData.find( dungeonID );
	if( it != m_mapDungeonData.end() )
	{
		CX2Dungeon::DungeonData* pDungeonData = it->second;
		CX2Dungeon* pDungeon = new CX2Dungeon( pDungeonData, bIsNpcLoad );
		return pDungeon;
	}
	else
	{
		return NULL;
	}
}

bool CX2DungeonManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pDungeonManager", this );

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR2, pFileName );
		return false;

    }

	return true;
}



bool CX2DungeonManager::AddDefaultRoomTitle_LUA( int iStringIndex )
{
	m_vecDefaultRoomTitle.push_back( GET_STRING( iStringIndex ) );
	return true;
}

/** @function : AddDungeonData_LUA
	@brief : DungeonData.lua�� �����߰��� ���
	@return : bool
*/
bool CX2DungeonManager::AddDungeonData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	CX2Dungeon::DungeonData* pDungeonData = new CX2Dungeon::DungeonData;

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "dungeonID",			pDungeonData->m_DungeonID,	SEnum::DUNGEON_ID,	SEnum::DI_NONE,	SAFE_DELETE(pDungeonData); return false; );
    
	int iStringIndex;
	LUA_GET_VALUE_RETURN(		luaManager, "dungeonName",			iStringIndex,	STR_ID_EMPTY, SAFE_DELETE(pDungeonData); return false; );
	pDungeonData->m_DungeonName = GET_STRING( iStringIndex );

#ifdef X2TOOL
	wstring wstrToolDungeonName = L"";
	LUA_GET_VALUE(		luaManager, "toolDungeonName",		wstrToolDungeonName,	L"" );
	if( wstrToolDungeonName.empty() == false )
	{
		pDungeonData->m_DungeonName = wstrToolDungeonName;
	}
#endif
	// ���� ��� ���� ��
	LUA_GET_VALUE_RETURN(		luaManager, "dataFileName",		pDungeonData->m_DataFileName,	L"", SAFE_DELETE(pDungeonData); return false; );

	// �ʿ��� ���� ���� ID
	LUA_GET_VALUE_ENUM(			luaManager, "requireDungeonID",	pDungeonData->m_RequireDungeonID, SEnum::DUNGEON_ID,	SEnum::DI_NONE );

	//{{ 2012. 05. 22	������	���� ���� ������ ����
#ifdef SERV_DUNGEON_REQUIRE_ITEM_LEVEL
	LUA_GET_VALUE(			luaManager, "m_RequireItemLevel",	pDungeonData->m_RequireItemLevel, -1 );
#endif SERV_DUNGEON_REQUIRE_ITEM_LEVEL
	//}}


	// �ʿ��� ���� ���� ID (������ ����)
	if( true == luaManager.BeginTable( "EXTRA_REQUIRE_DUNGEON_ID" ) )
	{
		for( int iValueIndex=1; true; iValueIndex++ )
		{
			int iExtraDungeonID = -1;
			if( true == luaManager.GetValue( iValueIndex, iExtraDungeonID ) )
			{
				if( -1 != iExtraDungeonID )
				{
					pDungeonData->m_vecExtraRequireDungeonID.push_back( (SEnum::DUNGEON_ID) iExtraDungeonID );
				}
			}
			else
			{
				break;
			}
		}

		luaManager.EndTable(); // EXTRA_REQUIRE_DUNGEON_ID
	}


	// ��д����� ��� TRUE
	LUA_GET_VALUE(				luaManager, "IS_HELL_MODE",		pDungeonData->m_bHellMode,	false );

	// ��д����� ID �� ���� ��Ƶ�
	//{{ kimhc // 2010.7.08 //	��д��� ���� �۾�
#ifdef	SERV_SECRET_HELL
	if ( pDungeonData->m_bHellMode == true )
		m_vecSecretDungeonID.push_back( pDungeonData->m_DungeonID );
#endif	SERV_SECRET_HELL
	//}} kimhc // 2010.7.08 //	��д��� ���� �۾�

	// �������忡 �ʿ��� ������
	//{{ 2007. 8. 29  ������  
	LUA_GET_VALUE(				luaManager, "requireItemID",		pDungeonData->m_RequireItemID, 0 );
	LUA_GET_VALUE(				luaManager, "requireItemCount",	pDungeonData->m_RequireItemCount, 0 );
	//}}


	LUA_GET_VALUE(				luaManager, "requireItemID2",		pDungeonData->m_RequireItemID2, 0 );
	LUA_GET_VALUE(				luaManager, "requireItemCount2",	pDungeonData->m_RequireItemCount2, 0 );

	// �������忡 �ʿ��� ū����
	//{{ 2007. 10. 4  ������  �ټ���
	LUA_GET_VALUE(				luaManager, "requireSpirit",		pDungeonData->m_RequireSpirit,		0 );
	//}}
    
	// ���� ���� �����Ͱ� �����ϴ� ������ ���̵�
	LUA_GET_VALUE_ENUM(			luaManager, "difficulty",			pDungeonData->m_eDifficulty,	CX2Dungeon::DIFFICULTY_LEVEL, CX2Dungeon::DL_NORMAL );

	// �Ϲ� ���̵��� �ִ� �����ΰ�?
	LUA_GET_VALUE(				luaManager, "normalOnly",			pDungeonData->m_bNormalOnly,	false );

	// �ð������� �ִ� �����̸� �ð����� ����
	LUA_GET_VALUE(				luaManager, "m_fTimeLimit",		pDungeonData->m_fTimeLimit,		-1.f );

#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
	// �������� �ð����� ������ �ð��� ǥ��
	LUA_GET_VALUE(				luaManager, "m_bShowStageTime",	pDungeonData->m_bShowStageTime,	false );
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	LUA_GET_VALUE(				luaManager, "m_bLevelEqualized",	pDungeonData->m_bLevelEqualized,	false );
	LUA_GET_VALUE(				luaManager, "m_bDamageEqualized",	pDungeonData->m_bDamageEqualized,	false );
	LUA_GET_VALUE(				luaManager, "m_bEventDungeon",		pDungeonData->m_bEventDungeon,		false );
	LUA_GET_VALUE(				luaManager, "m_usFixedMembers",	pDungeonData->m_usFixedMembers,		0 );
#endif SERV_DUNGEON_OPTION_IN_LUA

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	LUA_GET_VALUE(				luaManager, "m_iLimitedPlayTimes",		pDungeonData->m_iLimitedPlayTimes,		0 );
	LUA_GET_VALUE(				luaManager, "m_iLimitedClearTimes",	pDungeonData->m_iLimitedClearTimes,		0 );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
	LUA_GET_VALUE(				luaManager, "m_MinLevel",			pDungeonData->m_MinLevel,		0 );
	LUA_GET_VALUE(				luaManager, "m_MaxLevel",			pDungeonData->m_MaxLevel,		0 );

#ifdef X2TOOL
	LUA_GET_VALUE(				luaManager, "m_NPCLevel",			pDungeonData->m_iNpcLevel,		0 );
#endif

	// Ŭ���� �� ������ �� ���� ������� ���� ID�� ����
	LUA_GET_VALUE_ENUM(			luaManager, "m_eDefaultDungeonLoungeWorldID",		pDungeonData->m_eDefaultDungeonLoungeWorldID,		CX2World::WORLD_ID, CX2World::WI_NONE );

#ifdef HENIR_TEST
	// ���� Ÿ�� ����( ����� �Ϲݴ���, ��ϸ�, ��д��� )
	LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonType",				pDungeonData->m_eDungeonType,				CX2Dungeon::DUNGEON_TYPE,	CX2Dungeon::DT_NORMAL );

	// ������ ������ ���� ������ ������ ����ǰ� �Ұ����� ���� ����
	LUA_GET_VALUE(				luaManager, "m_bRelativeMonsterLevel",		pDungeonData->m_bRelativeMonsterLevel,		false );
#endif HENIR_TEST

	LUA_GET_VALUE(				luaManager, "m_DungeonDescription",		iStringIndex,	STR_ID_EMPTY );
	pDungeonData->m_DungeonDescription = GET_STRING( iStringIndex );

#ifdef HIDE_LOADING_TIP
	LUA_GET_VALUE(				luaManager, "m_bHideLoadingTip",		pDungeonData->m_bHideLoadingTip,		false );
#endif HIDE_LOADING_TIP

	if( true == luaManager.BeginTable( "BOSS_DROP_ITEM" ) )
	{
		pDungeonData->m_mapBossDropItem.clear();
		if( true == luaManager.BeginTable( "COMMON" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_NONE].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // ELSWORD
		}
		if( true == luaManager.BeginTable( "ELSWORD" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_ELSWORD].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // ELSWORD
		}
		if( true == luaManager.BeginTable( "AISHA" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_ARME].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // AISHA
		}
		if( true == luaManager.BeginTable( "RENA" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_LIRE].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // RENA
		}
		if( true == luaManager.BeginTable( "RAVEN" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_RAVEN].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // RAVEN
		}
		if( true == luaManager.BeginTable( "EVE" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_EVE].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // EVE
		}
#ifdef	NEW_CHARACTER_CHUNG	
		if( true == luaManager.BeginTable( "CHUNG" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_CHUNG].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // CHUNG
		}
#endif	NEW_CHARACTER_CHUNG

#ifdef ARA_CHARACTER_BASE
		if( true == luaManager.BeginTable( "ARA" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_ARA].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // CHUNG
		}
#endif
#ifdef NEW_CHARACTER_EL
		if( true == luaManager.BeginTable( "ELESIS" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_ELESIS].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // CHUNG
		}
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		if( true == luaManager.BeginTable( "ADD" ) )
		{
			int iBossDropItemIndex = 1;
			int iBossDropItemID = -1;
			while( true == luaManager.GetValue( iBossDropItemIndex, iBossDropItemID ) )
			{
				if( -1 != iBossDropItemID )
				{
					pDungeonData->m_mapBossDropItem[CX2Unit::UT_ADD].push_back(iBossDropItemID);
				}
				++iBossDropItemIndex;
			}
			luaManager.EndTable(); // CHUNG
		}
#endif //SERV_9TH_NEW_CHARACTER
		luaManager.EndTable(); // BOSS_DROP_ITEM
	}

	// �������� ���� ���ý� ��Ƽâ�� ������ ���� ������ �̹��� ����
	LUA_GET_VALUE(				luaManager, "m_TextureName",		pDungeonData->m_UIData.m_TextureName,	L"" );
	// �̹������Ͽ��� ����� Ű��
	LUA_GET_VALUE(				luaManager, "m_PieceName",			pDungeonData->m_UIData.m_PieceName,		L"" );
	// ���� ����
	LUA_GET_VALUE(				luaManager, "m_Explanation",		iStringIndex,	STR_ID_EMPTY );
	pDungeonData->m_UIData.m_Explanation = GET_STRING( iStringIndex );
	// 2010-11-17�� ���� ��� ���ϴ� ��
	LUA_GET_VALUE(				luaManager, "m_PopUpOffsetPosX",	pDungeonData->m_UIData.m_PopUpOffsetPos.x, 0.0f );
	LUA_GET_VALUE(				luaManager, "m_PopUpOffsetPosY",	pDungeonData->m_UIData.m_PopUpOffsetPos.y, 0.0f );


	//{{ 2009.1.22 ���¿� ������ �ε�ȭ��
#ifdef ENTER_SCREEN_FOR_DUNGEON
	// ���� �ε��� �� ���� �̹���
	LUA_GET_VALUE(				luaManager, "m_LoadingScreenFileName",			pDungeonData->m_UIData.m_LoadingScreenFileName,		L"" );
	LUA_GET_VALUE(				luaManager, "m_LoadingScreenFileName2",		pDungeonData->m_UIData.m_LoadingScreenFileName2,	L"" );

#endif
	//}}

	// ���� ���ý� �ڵ����� ������ ��Ƽ ��
	wstring wstrBuf = L"";
	if( true == luaManager.BeginTable( "AUTO_DUNGEON_ROOM_TITLE" ) )
	{
		for( int iValueIndex=1; true; iValueIndex++ )
		{
			LUA_GET_VALUE_RETURN( luaManager, iValueIndex, iStringIndex, STR_ID_EMPTY, break; );

			pDungeonData->m_UIData.m_vecAutoRoomTitle.push_back( GET_STRING( iStringIndex ) );
		}
		luaManager.EndTable(); // AUTO_DUNGEON_ROOM_TITLE
	}

	// ���� ������ �ϴ� ��� ����
	int iEndSpeechTableIndex = 0;
	while( true == luaManager.BeginTable( "ENDING_SPEECH", iEndSpeechTableIndex ) )
	{
		CX2Dungeon::DungeonData::EndingSpeechSetMap endingSpeechSetMap;

		if( true == luaManager.BeginTable( "ELSWORD" ) )
		{
 			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );
			
			endingSpeechSetMap[ CX2Unit::UT_ELSWORD ] = endingSpeechSet;
			luaManager.EndTable(); // ELSWORD
		}

		if( true == luaManager.BeginTable( "AISHA" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;
			
			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_ARME ] = endingSpeechSet;
			luaManager.EndTable(); // AISHA
		}



		if( true == luaManager.BeginTable( "RENA" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_LIRE ] = endingSpeechSet;
			luaManager.EndTable(); // RENA
		}



		if( true == luaManager.BeginTable( "RAVEN" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_RAVEN ] = endingSpeechSet;
			luaManager.EndTable(); // RAVEN
		}

		if( true == luaManager.BeginTable( "EVE" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_EVE ] = endingSpeechSet;
			luaManager.EndTable(); // EVE
		}

		//{{ kimhc // 2010.11.17 //  2010-12-23 �� �߰��� ��ĳ���� û
#ifdef	NEW_CHARACTER_CHUNG	
		if( true == luaManager.BeginTable( "CHUNG" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_CHUNG ] = endingSpeechSet;
			luaManager.EndTable(); // CHUNG
		}
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.17 //  2010-12-23 �� �߰��� ��ĳ���� û

#ifdef ARA_CHARACTER_BASE
		if( true == luaManager.BeginTable( "ARA" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_ARA ] = endingSpeechSet;
			luaManager.EndTable();
		}
#endif
#ifdef NEW_CHARACTER_EL
		if( true == luaManager.BeginTable( "ELESIS" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_ELESIS ] = endingSpeechSet;
			luaManager.EndTable();
		}
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		if( true == luaManager.BeginTable( "ADD" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			SetEndingSpeech( luaManager, iStringIndex, endingSpeechSet );

			endingSpeechSetMap[ CX2Unit::UT_ADD] = endingSpeechSet;
			luaManager.EndTable();
		}
#endif //SERV_9TH_NEW_CHARACTER
		pDungeonData->m_vecEndingSpeechSetMap.push_back( endingSpeechSetMap );

		luaManager.EndTable(); // ENDING_SPEECH (iEndSpeechTableIndex)
		iEndSpeechTableIndex++;
	}

	// ���� ������ ���� ����Ÿ�� map�� �־���
	DungeonDataIterator iD = m_mapDungeonData.find( pDungeonData->m_DungeonID );
	if ( iD != m_mapDungeonData.end() )
	{
#ifdef X2TOOL
		CX2Dungeon::DungeonData *pDungeonDataTemp = m_mapDungeonData[pDungeonData->m_DungeonID];
		SAFE_DELETE(pDungeonDataTemp);
#else
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"Error - CX2DungeonManager::AddDungeonData_LUA, �̹� ���� ID(%d)�� �����մϴ�" , pDungeonData->m_DungeonID );
		/*wsprintf( buff, L"Error - CX2DungeonManager::AddDungeonData_LUA, �̹� ���� ID(%d)�� �����մϴ�" , pDungeonData->m_DungeonID );*/
		ErrorLogMsg( XEM_ERROR2, buff );
		SAFE_DELETE( pDungeonData );
		return false;
#endif
	}

	m_mapDungeonData[pDungeonData->m_DungeonID] = pDungeonData;

	return true;
}

int CX2DungeonManager::GetDungeonCount()
{
	return m_mapDungeonData.size();
}

const CX2Dungeon::DungeonData* CX2DungeonManager::GetDungeonData( SEnum::DUNGEON_ID dungeonID )
{
	return m_mapDungeonData[dungeonID];
}

CX2Dungeon::DungeonData* CX2DungeonManager::GetDungeonDataAt( int iIdx )
{
	DungeonDataIterator iD = m_mapDungeonData.begin();
	std::advance( iD, iIdx );

	CX2Dungeon::DungeonData* pkDungeonData = (*iD).second;
	return pkDungeonData;
}

bool CX2DungeonManager::IsActiveDungeon( int iDungeonID, int iDiffLevel )
{
	const CX2Dungeon::DungeonData* pkDungeonData = 
		GetDungeonData( (SEnum::DUNGEON_ID) (iDungeonID + iDiffLevel) );
	if ( pkDungeonData == NULL )
		return false;

	// ���� Ŭ���� ����
	if ( pkDungeonData->m_RequireDungeonID != SEnum::DI_NONE &&
		g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pkDungeonData->m_RequireDungeonID ) == false )
	{
		return false;
	}

	for( UINT i=0; i<pkDungeonData->m_vecExtraRequireDungeonID.size(); i++ )
	{
		if( SEnum::DI_NONE != pkDungeonData->m_vecExtraRequireDungeonID[i] &&
			false == g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pkDungeonData->m_vecExtraRequireDungeonID[i] ) )
		{
			return false;
		}
	}


	// ���� �ּҷ��� - 2 ���� ������ ������ �ʴ´�.
	if ( g_pData->GetSelectUnitLevel() < pkDungeonData->m_MinLevel - 2 )
	{
		return false;
	}

	// ���� �׽�Ʈ�� ������ �����ڿ��Ը�
#ifndef _SERVICE_
	if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
	{
		if( iDungeonID == (int)SEnum::DI_MONSTER_TEST_NORMAL ||
			iDungeonID == (int)SEnum::DI_MONSTER_TEST_HARD ||
			iDungeonID == (int)SEnum::DI_MONSTER_TEST_EXPERT )
		{
			return false;
		}
	}

    // �׽�Ʈ�� ����
    if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
    {
        if( iDungeonID == (int)SEnum::DI_TEST_DUNGEON_NORMAL ||
            iDungeonID == (int)SEnum::DI_TEST_DUNGEON_HARD ||
            iDungeonID == (int)SEnum::DI_TEST_DUNGEON_EXPERT )
        {
            return false;
        }
    }

    if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV )
    {
        if( iDungeonID == (int)SEnum::DI_TEST1_DUNGEON_NORMAL ||
            iDungeonID == (int)SEnum::DI_TEST1_DUNGEON_HARD ||
            iDungeonID == (int)SEnum::DI_TEST1_DUNGEON_EXPERT )
        {
            return false;
        }
    }
#endif

	return true;
}

//{{ ����� : [2011/3/18/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
bool CX2DungeonManager::IsDefenceDungeon( IN const int iDungeonID ) const
{
	switch( iDungeonID )
	{
	CASE_DEFENCE_DUNGEON
		return true;

	default:
		return false;
	}

	return false;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} ����� : [2011/3/18/] //	���� �̼�

//{{ 2009. 4. 24  ������ �̺�Ʈ ����
#ifdef SERV_DUNGEON_OPTION_IN_LUA
bool CX2DungeonManager::IsEventDungeon( IN const int iDungeonID )
#else SERV_DUNGEON_OPTION_IN_LUA
bool CX2DungeonManager::IsEventDungeon( IN const int iDungeonID ) const
#endif SERV_DUNGEON_OPTION_IN_LUA
{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( GetDungeonData( static_cast<SEnum::DUNGEON_ID>( iDungeonID ) )->m_bEventDungeon == true )
		return true;
#endif SERV_DUNGEON_OPTION_IN_LUA

	switch( iDungeonID )
	{
	case SEnum::DI_EVENT_KIDDAY_RUBEN:
	case SEnum::DI_EVENT_KIDDAY_ELDER:
	case SEnum::DI_EVENT_KIDDAY_BESMA:
	case SEnum::DI_EVENT_KIDDAY_ALTERA:

	case SEnum::DI_EVENT_TREE_DAY_ELDER:  // �ĸ��� �̺�Ʈ ����
	case SEnum::DI_EVENT_TREE_DAY_BESMA:
	case SEnum::DI_EVENT_TREE_DAY_ALTERA:
	case SEnum::DI_EVENT_TREE_DAY_PEITA:
	case SEnum::DI_EVENT_TREE_DAY_VELDER:
	case SEnum::DI_EVENT_TREE_DAY_HAMEL:

		//{{ 2010. 10. 19 ������ ��д��� �̺�Ʈ ������Ʈ
#ifdef SERV_SECRET_DUNGEON_EVENT
	case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
	case SEnum::DI_ELDER_HALLOWEEN_HARD:
	case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
#endif SERV_SECRET_DUNGEON_EVENT
		//}}
		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		CASE_DEFENCE_DUNGEON
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	case SEnum::DI_EVENT_VALENTINE_DAY:
			return true;
	}

	return false;
}

//{{ kimhc // 2010.3.19 //	��д��� ���� �۾�
#ifdef	SERV_SECRET_HELL

int CX2DungeonManager::GetDungeonType( int iDungeonID )
{
	const CX2Dungeon::DungeonData* pDungeonData = GetDungeonData( static_cast<SEnum::DUNGEON_ID>( iDungeonID ) );

	if ( pDungeonData == NULL )
		return CX2Dungeon::DT_NORMAL;

	return pDungeonData->m_eDungeonType;
}

// �Ѱ��� �����ִ� ��д����� �ִ°�?
bool CX2DungeonManager::CanGoOneSecretDungeonAtLeast()
{
	BOOST_TEST_FOREACH( SEnum::DUNGEON_ID&, val, m_vecSecretDungeonID )
	{
		if ( IsActiveDungeon( val, 0 ) == true )
			return true;
	}

	return false;
}

#endif	SERV_SECRET_HELL
//}} kimhc // 2010.3.19 //	��д��� ���� �۾�

vector<int> CX2DungeonManager::GetNextDungeon( int iDungeonID )
{
	vector<int> vecNewOpenedDungeon;

	map<SEnum::DUNGEON_ID, CX2Dungeon::DungeonData*>::iterator it;
	for( it = m_mapDungeonData.begin(); it != m_mapDungeonData.end(); it++ )
	{
		CX2Dungeon::DungeonData* pDungeonData = (CX2Dungeon::DungeonData*) it->second;
		if( pDungeonData != NULL )
		{
			if( pDungeonData->m_RequireDungeonID == iDungeonID )
			{
				vecNewOpenedDungeon.push_back( (int)it->first );
			}
		}		
	}

	return vecNewOpenedDungeon;
}

//{{ 2007. 9. 5  ������  DungeonTool �׽�Ʈ
void CX2DungeonManager::GetDungeonList( map< std::wstring, SEnum::DUNGEON_ID >& mapDungeonList )
{
	DungeonDataIterator mit = m_mapDungeonData.begin();
	for( ; mit != m_mapDungeonData.end(); mit++ )
	{
		mapDungeonList.insert( std::make_pair( (mit->second)->m_DataFileName, (mit->second)->m_DungeonID ) );
	}
}
//}}

D3DXCOLOR CX2DungeonManager::GetDifficultyColor( int dungeonID, int difficulty, int checkLevel )
{
	D3DXCOLOR diffColor = D3DXCOLOR( 1, 1, 1, 1 );

	const CX2Dungeon::DungeonData* pDungeonData = GetDungeonData( (SEnum::DUNGEON_ID)(dungeonID + difficulty) );
	if ( pDungeonData == NULL )
		return diffColor;

	if ( pDungeonData->m_MinLevel - 1 <= checkLevel &&
		pDungeonData->m_MaxLevel + 1 >= checkLevel )
	{
		diffColor = D3DXCOLOR( 1,0.8f,0,1 );
	}
	else if ( pDungeonData->m_MinLevel - 1 > checkLevel )
	{
		diffColor = D3DXCOLOR( 1,0,0,1 );
		
	}
	else
	{
		diffColor = D3DXCOLOR( 0,1,0,1 );
	}

	return diffColor;
}

D3DXCOLOR CX2DungeonManager::GetLimitLevelColor( int dungeonID, int difficulty, int checkLevel )
{
	D3DXCOLOR limitLevelColor = D3DXCOLOR( 1, 1, 1, 1 );

	const CX2Dungeon::DungeonData* pDungeonData = GetDungeonData( (SEnum::DUNGEON_ID)(dungeonID + difficulty) );
	if ( pDungeonData == NULL )
		return limitLevelColor;

	if ( pDungeonData->m_MinLevel == checkLevel )
	{
		limitLevelColor = D3DXCOLOR( 1,1,0,1 );
	}
	else if ( pDungeonData->m_MinLevel > checkLevel )
	{
		limitLevelColor = D3DXCOLOR( 1,0,0,1 );
	}
	else
	{
		limitLevelColor = D3DXCOLOR( 0,1,0,1 );
	}

	return limitLevelColor;
}

wstring CX2DungeonManager::MakeDungeonNameString( SEnum::DUNGEON_ID dungeonID, CX2Dungeon::DIFFICULTY_LEVEL eDifficulty, CX2Dungeon::DUNGEON_MODE eDungeonMode )
{
	const CX2Dungeon::DungeonData* pDungeonData = GetDungeonData( dungeonID );
	if( NULL == pDungeonData )
		return NULL;

	wstringstream wstrmDungeonName;


	switch( pDungeonData->m_eDungeonType )
	{
	default:
	case CX2Dungeon::DT_NORMAL:
		{
			switch( eDifficulty )
			{
			case CX2Dungeon::DL_NORMAL: 
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_141 );
				} break;

			case CX2Dungeon::DL_HARD: 
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_142 );
				} break;

			case CX2Dungeon::DL_EXPERT:
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_143 );
				} break;
			default:
				{
					wstrmDungeonName << pDungeonData->m_DungeonName;
				} break;
			}
		} break;
	
	case CX2Dungeon::DT_HENIR:
		{
			switch( eDungeonMode )
			{
			default:
			case CX2Dungeon::DM_HENIR_PRACTICE:
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_938 );
				} break;

			case CX2Dungeon::DM_HENIR_CHALLENGE:
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_3747 );
				} break;
			}

		} break;

//{{ kimhc // 2010.3.19 //	��д��� ���� �۾�
#ifdef	SERV_SECRET_HELL
		//{{ ��д��� �ΰ��
	case CX2Dungeon::DT_SECRET:
		{
			switch ( eDungeonMode )
			{
			default:
			case CX2Dungeon::DM_SECRET_NORMAL:	// �Ϲ�
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_938 );
				} break;

			case CX2Dungeon::DM_SECRET_HELL:	// ��
				{
					wstrmDungeonName << pDungeonData->m_DungeonName << L" " << GET_STRING( STR_ID_4943 );
				} break;
			}
		} break;
#endif	SERV_SECRET_HELL
//}} kimhc // 2010.3.19 //	��д��� ���� �۾�
	}

	return wstrmDungeonName.str();
}

wstring CX2DungeonManager::MakeDungeonNameString( SEnum::DUNGEON_ID dungeonID )
{
	if( SEnum::DI_NONE == dungeonID )
		return L"";

	const CX2Dungeon::DungeonData* pDungeonData = GetDungeonData( dungeonID );
	if( NULL == pDungeonData )
		return L"";

	wstringstream wstrmDungeonName;
	wstrmDungeonName << pDungeonData->m_DungeonName;

	return wstrmDungeonName.str();
}

/** @function : SetEndingSpeech
	@brief : DugeonData.lua�� �� ĳ���ͺ� endingSpeech�� ����
	@param : IN KLuaManager& luaManager_, IN int iStrIndexDungeon_, OUT CX2Dungeon::DungeonData::EndingSpeechSet& pEndingSpeechSet_
	@return : void
*/
void CX2DungeonManager::SetEndingSpeech( IN KLuaManager& luaManager_, IN int iStrIndexDungeon_, OUT CX2Dungeon::DungeonData::EndingSpeechSet& pEndingSpeechSet_ )
{
	int iTableIndex = 1;
	while( true == luaManager_.BeginTable( iTableIndex ) )
	{
		CX2Dungeon::DungeonData::EndingSpeech endingSpeech;

		int iValueIndex = 1;				
		while( true )
		{
			LUA_GET_VALUE_RETURN(		luaManager_,	 iValueIndex,	iStrIndexDungeon_, STR_ID_EMPTY, break; );
			endingSpeech.push_back( GET_STRING( iStrIndexDungeon_ ) );
			iValueIndex++;
		}
		pEndingSpeechSet_.push_back( endingSpeech );

		luaManager_.EndTable(); // iTableIndex
		iTableIndex++;
	}
}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
bool CX2DungeonManager::GetLimitedPlayTimes( IN const int nDungeonID, OUT int& iPlayTimes )
{
	const CX2Dungeon::DungeonData* pkDungeonData = GetDungeonData( (SEnum::DUNGEON_ID) nDungeonID );
	if( pkDungeonData == NULL )
		return false;

	if( pkDungeonData->m_iLimitedPlayTimes <= 0 )
		return false;

	// �÷��� Ƚ�� ���� ������ ���
	iPlayTimes = pkDungeonData->m_iLimitedPlayTimes;
	return true;
}
bool CX2DungeonManager::GetLimitedClearTimes( IN const int nDungeonID, OUT int& iClearTimes )
{
	const CX2Dungeon::DungeonData* pkDungeonData = GetDungeonData( (SEnum::DUNGEON_ID) nDungeonID );
	if( pkDungeonData == NULL )
		return false;

	if( pkDungeonData->m_iLimitedClearTimes <= 0 )
		return false;

	// Ŭ���� Ƚ�� ���� ������ ���
	iClearTimes = pkDungeonData->m_iLimitedClearTimes;
	return true;
}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES