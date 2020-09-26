#include "StdAfx.h"
#include ".\X2BattleFieldManager.h"

////////////////////////////////////////////////////////////////////////////////////////
// CBattleFieldPortalMovingInfo
////////////////////////////////////////////////////////////////////////////////////////
/** @function : ParsingScriptFile
	@brief : CBattleFieldPortalMovingInfo�� �ʿ��� ���� ��ũ��Ʈ�� ���� �о����
	@param : IN KLuaManager& luaManager_
	@return : bool (�ʵ忡�� �ݵ�� �Ѱ� �̻��� ��Ż ������ �־�� �ϱ� ������ bool ���� ����)
*/
bool CBattleFieldPortalMovingInfo::ParsingScriptFile( IN KLuaManager& luaManager_ )
{
	// ��Ż�� �̵� Ÿ�� (CX2BattleFieldManager::PORTAL_MOVE_TYPE, ��Ż->����, ��Ż->��Ż)
	LUA_GET_VALUE( luaManager_, "PORTAL_MOVE_TYPE", m_usPortalMoveType, 0 );

	if ( 0 != m_usPortalMoveType )
	{
		// �ʵ��� ���θʿ��� �� ��Ż�� ��ġ�� �ʵ� ���� ���� ��ȣ
		LUA_GET_VALUE( luaManager_, "LINE_NUMBER", m_uiLineNumber, 0 );

		// �̵��� ����� ID (VillageMapID or BattleFieldID)
		LUA_GET_VALUE( luaManager_, "PLACE_ID_TO_MOVE", m_uiPlaceIdToMove, 0 );
		ASSERT( 0 != m_uiPlaceIdToMove );

		// �̵��� ����� Postion Index�� ���� ������ ���� DLG_Map_Village.lua�� �ִ� StartPosId, �ʵ��� ��쿡�� StartPos(?) �� �ɵ�
		LUA_GET_VALUE( luaManager_, "POSITION_INDEX_TO_MOVE", m_usPositionIndexToMove, 0 );
		return true;
	}
	else
	{
		ASSERT( 0 != m_usPortalMoveType );
		StateLog( L"MOVE_TYPE is undefined!" );
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// CBattleFieldRiskInfo
////////////////////////////////////////////////////////////////////////////////////////
/** @function : ParsingScriptFile
	@brief : CBattleFieldRiskInfo�� �ʿ��� ���� ��ũ��Ʈ�� ���� �о����
	@param : IN KLuaManager& luaManager_
	@return : void (�ʵ忡�� RISK_INFO�� ���� �� �� �ֱ� ������ void ���� ���)
*/
bool CBattleFieldRiskInfo::ParsingScriptFile( IN KLuaManager& luaManager_ )
{
	bool bResult = true;
	// ���赵 ��ġ
	LUA_GET_VALUE( luaManager_, "RISK_VALUE", m_usRiskValue, 0 );
	ASSERT( 0 != m_usRiskValue );	// ���赵 ���̺��� �ִµ�, ���赵 ��ġ�� 0�̶�� ���� ���� �ȵ�
	if ( 0 == m_usRiskValue )
	{
		StateLog( L"RISK_VALUE is undefined!" );
		bResult = false;
	}

	// ���赵 ��ġ�� ���� ������ ����
	LUA_GET_VALUE( luaManager_, "SPAWN_MONSTER_ID", m_uiMonsterIdToBeSpawned, 0 );
	ASSERT( 0 != m_uiMonsterIdToBeSpawned );	// ���赵 ���̺��� �ִµ�, ���赵 ��ġ�� 0�̶�� ���� ���� �ȵ�
	if ( 0 == m_uiMonsterIdToBeSpawned )
	{
		StateLog( L"SPAWN_MONSTER_ID is undefined!" );
		bResult = false;
	}

	// ���� �̸��� ���� �� ������ ����
	bool bShowBossName;
	LUA_GET_VALUE( luaManager_, "SHOW_BOSS_NAME", bShowBossName, false );
	SetBitSetFlag( FRI_SHOW_BOSS_NAME, bShowBossName );

	// ������ HP �������� ���� �ٷ� ���� ������ ����
	bool bDevideBossHPGaugeToLines;
	LUA_GET_VALUE( luaManager_, "DEVIDE_BOSS_HP_GAUGE_TO_LINES", bDevideBossHPGaugeToLines, false );
	SetBitSetFlag( FRI_DEVIDE_BOSS_HP_GAUGE_TO_LINES, bDevideBossHPGaugeToLines );

	return bResult;
}

////////////////////////////////////////////////////////////////////////////////////////
// CBattleFieldData
////////////////////////////////////////////////////////////////////////////////////////
/** @function : ParsingScriptFile
	@brief : CBattleFieldData�� �ʿ��� ���� ��ũ��Ʈ�� ���� �о���� (m_wstrBattleFieldName, m_strFileNameAtLoading �� �ܺο��� �о����)
	@param : IN KLuaManager& luaManager_
	@return : bool
*/
bool CBattleFieldData::ParsingScriptFile( IN KLuaManager& luaManager_ )
{
	bool bResult = true;
	// �ʵ� ID
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_ID", m_uiBattleFieldId, 0 );
	if ( SEnum::VMI_INVALID == m_uiBattleFieldId )
	{
		ASSERT( SEnum::VMI_INVALID != m_uiBattleFieldId );
		StateLog( L"BATTLE_FIELD_ID is undefined!" );

		bResult = false;
	}

	// �� �ʵ忡�� ����ϴ� World ID
	LUA_GET_VALUE( luaManager_, "WORLD_ID", m_uiWorldId, 0 );
	if ( CX2World::WI_NONE == m_uiWorldId )
	{
		ASSERT( CX2World::WI_NONE != m_uiWorldId );
		StateLog( L"WORLD_ID is undefined!" );

		bResult = false;
	}

	// ������ �� �ʵ忡�� �װ� ��Ȱ���� ���� ��� ���ư��Ե� ����	
	LUA_GET_VALUE( luaManager_, "RETURN_VILLAGE_ID", m_uiReturnVillageId, 0 );

	// ���� �ʵ忡�� ������ ���͵��� ���� ���� 
	LUA_GET_VALUE( luaManager_, "STANDARD_MONSTER_LEVEL", m_uiStandardMonsterLevel, 0 );
	if ( 0 == m_uiStandardMonsterLevel )
	{
		ASSERT( 0 != m_uiStandardMonsterLevel );
		StateLog( L"STANDARD_MONSTER_LEVEL is undefined!" );

		bResult = false;
	}

	// ���� �ʵ忡�� �ִ�� ���� �� �ִ� ���� ��
	LUA_GET_VALUE( luaManager_, "MAX_NUMBER_OF_MONSTER_IN_THIS_BATTLE_FIELD", m_usMaxNumberOfMonsterInThisBattleField, 0 );
	if ( 0 == m_usMaxNumberOfMonsterInThisBattleField )
	{
		ASSERT( 0 != m_usMaxNumberOfMonsterInThisBattleField );
		StateLog( L"MAX_NUMBER_OF_MONSTER_IN_THIS_BATTLE_FIELD is undefined!" );

		bResult = false;
	}

	// �ʵ� ��
	int iStringIndex = 0;
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_NAME", iStringIndex,	STR_ID_EMPTY );
	
	if ( STR_ID_EMPTY != iStringIndex )
		m_wstrBattleFieldName = GET_STRING( iStringIndex );
	else
	{
		ASSERT( STR_ID_EMPTY != iStringIndex );
		StateLog( L"BATTLE_FIELD_NAME is undefined!" );
		bResult = false;
	}

	// �ʵ� ���� ��Ÿ���� �ؽ��� ���� ��
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_TEXTURE_NAME", m_wstrBattleFieldTextureName, L"" );

	// �ʵ� ���� ��Ÿ���� �ؽ��� ���� Ű��
	LUA_GET_VALUE( luaManager_, "BATTLE_FIELD_TEXTURE_KEY", m_wstrBattleFieldTextureKey, L"" );

	// �ʵ� ���� �ִ� ��Ż�� �̵� ���� �Ľ�
	if ( false == ParsingBattleFieldMovingInfo( luaManager_ ) )
		bResult = false;

	// �ʵ��� ���赵 ����
	if ( false == ParsingBattleFieldRiskInfo( luaManager_ ) )
		bResult = false;

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	ParsingBattleFieldMiddleBossInfo ( luaManager_ );
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	LUA_GET_VALUE( luaManager_, "IS_BOSS_RAID_FIELD", m_bIsBossRaidField, false);
	LUA_GET_VALUE( luaManager_, "RAID_FIELD_PORTAL_POSITION_INDEX", m_usRaidFieldPortalPositionIndex, 0);
#endif // FIELD_BOSS_RAID

	return bResult;
}

/** @function : ParsingBattleFieldMovingInfo
	@brief : CBattleFieldMovingInfo ������ lua�� ���� �о����
	@param : IN KLuaManager& luaManager_
	@return : bool (����, ����)
*/
bool CBattleFieldData::ParsingBattleFieldMovingInfo( IN KLuaManager& luaManager_ )
{
	bool bResult = true;

	// �ʵ� ���� �ִ� ��Ż�� �̵� ����
	if ( luaManager_.BeginTable( "BATTLE_FIELD_PORTAL_MOVING_INFO") )
	{
		int iTableIndex = 1;	// ���̺� �ε���
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			// BattleFieldPortalMovingInfo�� shared_ptr ����
			CBattleFieldPortalMovingInfoPtr ptrBattleFieldPortalMovingInfo = CreateBattleFieldPortalMovingInfoPtr();

			// BattleFieldPortalMovingInfo�� �������� �ʾҰų�, �Ľ� ���� ������
			if ( ptrBattleFieldPortalMovingInfo == NULL ||
				false == ptrBattleFieldPortalMovingInfo->ParsingScriptFile( luaManager_ ) )
				bResult = false;
			else	// ���������� ���� ������ vector�� push �Ѵ�.
				m_vecBattleFieldPortalMovingInfoPtr.push_back( ptrBattleFieldPortalMovingInfo );

			luaManager_.EndTable();
			++iTableIndex;	// ���̺��� �ε��� ����
		}	
		luaManager_.EndTable();
	}
	else
	{
		ASSERT( !"BATTLE_FIELD_PORTAL_MOVING_INFO is Empty!" );
		StateLog( L"BATTLE_FIELD_PORTAL_MOVING_INFO is Empty!" );

		bResult = false;
	}

	return bResult;
}

/** @function : ParsingBattleFieldRiskInfo
	@brief : CBattleFieldRiskInfo ������ lua�� ���� �о����
	@param : IN KLuaManager& luaManager_
	@return : bool (����, ����)
*/
bool CBattleFieldData::ParsingBattleFieldRiskInfo( IN KLuaManager& luaManager_ )
{
	bool bResult = true;

	// �ʵ��� ���赵 ����
	if ( luaManager_.BeginTable( "BATTLE_FIELD_RISK_INFO" ) )
	{
		int iTableIndex = 1;	// ���̺� �ε���
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			// BattleFieldRisk�� shared_ptr ����
			CBattleFieldRiskInfoPtr ptrBattleFieldRiskInfo = CreateBattleFieldRiskInfoPtr();

			// BattleFieldRiskInfo�� �������� �ʾҰų�, �Ľ� ���� ������
			if ( ptrBattleFieldRiskInfo == NULL ||
				false == ptrBattleFieldRiskInfo->ParsingScriptFile( luaManager_ ) )
				bResult = false;
			else	// ���������� ���� ������ vector�� push �Ѵ�.
				m_vecBattleFieldRiskInfoPtr.push_back( ptrBattleFieldRiskInfo );

			luaManager_.EndTable();
			++iTableIndex;	// ���̺��� �ε��� ����
		}		

		luaManager_.EndTable();
	}

	return bResult;
}

CBattleFieldData::CBattleFieldPortalMovingInfoPtr CBattleFieldData::GetBattleFieldPortalMovingInfoPtrByLineMapIndex( const UINT uiLineMapIndex_ )
{
	BOOST_FOREACH( CBattleFieldPortalMovingInfoPtr val, m_vecBattleFieldPortalMovingInfoPtr )
	{
		if ( val->GetLineNumber() == uiLineMapIndex_ )
			return val;
	}

	return CBattleFieldPortalMovingInfoPtr();
}

#ifdef QUEST_GUIDE
bool CBattleFieldData::GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_)
{
	mapBattleFieldMovingInfo_.clear();
	BOOST_FOREACH( CBattleFieldPortalMovingInfoPtr val, m_vecBattleFieldPortalMovingInfoPtr )
	{
		UINT uiPlaceToMove = val->GetPlaceIdToMove();
		UINT uiLineMapIndex = val->GetLineNumber();
		mapBattleFieldMovingInfo_.insert( std::make_pair(uiPlaceToMove, uiLineMapIndex) );
	}
	return true;
}
#endif //QUEST_GUIDE



#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void CBattleFieldData::ParsingBattleFieldMiddleBossInfo ( IN KLuaManager& luaManager_ )	// BATTLE_FIELD_MIDDLE_BOSS_INFO �Ľ�
{
	if ( true == luaManager_.BeginTable( "BATTLE_FIELD_MIDDLE_BOSS_INFO" ) )
	{
		int iTableIndex = 1;	// ���̺� �ε���
		while ( true == luaManager_.BeginTable( iTableIndex ) )
		{
			// BATTLE_FIELD_MIDDLE_BOSS_INFO �� shared_ptr ����
			CBattleFieldMiddleBossInfoPtr ptrBattleFieldMiddleBossInfo = CreateBattleFieldMiddleBossInfoPtr();

			// BATTLE_FIELD_MIDDLE_BOSS_INFO �� �������� �ʾҰų�, �Ľ� ���� ������
			if ( ptrBattleFieldMiddleBossInfo != NULL &&
				true == ptrBattleFieldMiddleBossInfo->ParsingScriptFile( luaManager_ ) )
			{
				m_vecBattleFieldMiddleBossInfoPtr.push_back( ptrBattleFieldMiddleBossInfo );
			}
			luaManager_.EndTable();
			++iTableIndex;	// ���̺��� �ε��� ����
		}		
		luaManager_.EndTable();
	}
}

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


////////////////////////////////////////////////////////////////////////////////////////
// CX2BattleFieldManager
////////////////////////////////////////////////////////////////////////////////////////
/** @function : OpenScriptFile
	@brief : BattleField ��ũ��Ʈ �Ľ�
	@param : const char* szScriptFileName_
*/
void CX2BattleFieldManager::OpenScriptFile( const wchar_t* wszScriptFileName_ )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pBattleFieldManager", this );

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName_ ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName_ );
    }
}


/** @function : AddBattleFieldData_LUA
	@brief : BattleFieldData.lua�� AddBattleFieldData ����
*/
void CX2BattleFieldManager::AddBattleFieldData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	// BattleFieldData�� shared_ptr ����
	CBattleFieldDataPtr ptrBattleFieldData = CreateBattleFieldDataPtr();

	// ������ �� �Ǿ�����
	if ( ptrBattleFieldData != NULL )
	{
		// �Ľ̿� ����������
		if ( ptrBattleFieldData->ParsingScriptFile( luaManager ) )
			m_mapBattleFieldDataPtr.insert( BattleFieldDataPtrPair( ptrBattleFieldData->GetBattleFieldId(), ptrBattleFieldData ) );
		else
		{
			// �Ľ̿� ����������
			ASSERT( !"BattleField Data is fault!" );
			StateLog( L"BattleField Data Is fault!" );
		}
	}
	else
	{
		// ������ �ȵǾ�����
		ASSERT( !"BattleField Data is NULL!" );
		StateLog( L"BattleField Data Is NULL!" );
	}
}

/** @function : ReOpenScriptFile
	@brief : ������ BattleField����(m_mapBattleFieldDataPtr)�� clear �ϰ�, ���� �о� ����
	@param : const char* szScriptFileName_
*/
void CX2BattleFieldManager::ReOpenScriptFile( const wchar_t* wszScriptFileName_ )
{
	m_mapBattleFieldDataPtr.clear();
	OpenScriptFile( wszScriptFileName_ );
}

/** @function : GetWorldIdByBattleFieldId
	@brief : ��Ʋ�ʵ� ID�� �ش��ϴ� WorldID�� ����
	@param : UINT uiBattleFieldId_
	@return : UINT (WorldId)
*/
UINT CX2BattleFieldManager::GetWorldIdByBattleFieldId( UINT uiBattleFieldId_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
			return mItrBattleFieldDataPtr->second->GetWorldId();
	}

	return 0;
}

/** @function : GetPortalMovingInfoByLineMapIndexInNowBattleField
	@brief : ���ڷ� ���� ���θ��� �ε����� ��ġ�ϴ� PortalMovingInfo�� ����
	@param : const int iLineMapIndex_ (���θ� �ε���)
	@return : 
*/
CBattleFieldData::CBattleFieldPortalMovingInfoPtr CX2BattleFieldManager::GetPortalMovingInfoByLineMapIndexInNowBattleField( const int iLineMapIndex_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// ���� ��Ʋ�ʵ��� ������ ������ �´�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( GetBattleFieldIdWhereIam() );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			// ���� ��Ʋ�ʵ� ��������
			return mItrBattleFieldDataPtr->second->GetBattleFieldPortalMovingInfoPtrByLineMapIndex( iLineMapIndex_ );
		}
	}

	return CBattleFieldData::CBattleFieldPortalMovingInfoPtr();
}
#ifdef QUEST_GUIDE
/** @function : GetBattleFieldPortalMovingInfo
	@brief : ���� �ʵ忡�� �̵� ������ ����� MapID�� ����� ���� LINE_NUMBER ����
	@param : map<UINT, UINT>& mapBattleFieldMovingInfo_ ( PLACE_ID_TO_MOVE, LINE_NUMBER )
	@return : bool(����, ����)
*/
bool CX2BattleFieldManager::GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_)
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// ���� ��Ʋ�ʵ��� ������ ������ �´�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( GetBattleFieldIdWhereIam() );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			// ���� ��Ʋ�ʵ� ��������
			mItrBattleFieldDataPtr->second->GetBattleFieldPortalMovingInfo( mapBattleFieldMovingInfo_ );
			return true;
		}
	}

	return false;
}
#endif //QUEST_GUIDE



void CX2BattleFieldManager::SetMonsterRespawnFactorByUserCount_LUA()
{
}

void CX2BattleFieldManager::SetMonsterRespawnTimeMinMax_LUA()
{
}

void CX2BattleFieldManager::SetBattleFieldFactor_LUA()
{

}

UINT CX2BattleFieldManager::GetReturnVillageId( UINT uiBattleFieldId_ /*= -1*/) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
#ifdef FIELD_BOSS_RAID // ����ȯ
		/// ���̵� �ʵ��� ���, ���� �� �ʵ� ���̵� ���
		if( true == g_pData->GetBattleFieldManager().GetIsBossRaidFieldByFieldID( uiBattleFieldId_ ) )
		{
			uiBattleFieldId_ = CX2BossRaidManager::GetInstance()->GetBossRaidCreatorMapID();
		}
		else
#endif // FIELD_BOSS_RAID
		{
			//������//2012-10-25//���� ����� ȣ���ϸ� ���� �ʵ��� ReturnVillageId ���ϵ��� ����
			if( -1 == uiBattleFieldId_ )
			{
				uiBattleFieldId_ = m_BattleFieldPositionInfo.m_uiBattleFieldIdWhereIam;
			}
		}

		// ���� ��Ʋ�ʵ��� ������ ������ �´�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			// ���� ��Ʋ�ʵ� ��������

			return mItrBattleFieldDataPtr->second->GetReturnVillageId();
		}
	}

	return 0;
}

bool CX2BattleFieldManager::GetBattleFieldTextureNameAndKey( IN const UINT uiBattleFieldId_, OUT const WCHAR** pTextureName_, OUT const WCHAR** pKeyName_ )
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// ���� ��Ʋ�ʵ��� ������ ������ �´�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			*pTextureName_ = mItrBattleFieldDataPtr->second->GetBattleFieldTextureName();
			ASSERT( NULL != pTextureName_ && NULL != pTextureName_[0] );

			*pKeyName_ = mItrBattleFieldDataPtr->second->GetBattleFieldTextureKey();
			ASSERT( NULL != pKeyName_ && NULL != pKeyName_[0] );
		}
		else
		{
			pTextureName_ = NULL;
			pKeyName_ = NULL;
			ASSERT( !L"BattleFieldData doesn't exist" );
		}
	}
	else
	{
		pTextureName_ = NULL;
		pKeyName_ = NULL;
	}

	return ( NULL != pTextureName_ && NULL != pKeyName_ );
}

#ifdef REFORM_QUEST
/** @function : GetBattleFieldNameByBattleFieldId
	@brief : ��Ʋ�ʵ� ID�� �ش��ϴ� �ʵ��̸��� ����
	@param : UINT uiBattleFieldId_
	@return : wstring
*/
const WCHAR* CX2BattleFieldManager::GetBattleFieldNameByBattleFieldId( UINT uiBattleFieldId_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
			return mItrBattleFieldDataPtr->second->GetBattleFieldName();
	}

	return 0;
}

#endif //REFORM_QUEST 


#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
wstring CX2BattleFieldManager::GetBattleFieldBossDataGroupName ( UINT uiBattleFieldId_, int iBattleFieldBossGroupId_ )
{
	if ( false == m_mapBattleFieldDataPtr.empty() )		// BattleFieldData �� �����Ѵٸ�
	{
		// FieldID �� �˻��Ѵ�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldId_ );
		
		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )	// FieldID �� �����Ѵٸ�
		{
			// Group �̸��� ��ȯ�Ѵ�.
			return mItrBattleFieldDataPtr->second->GetBattleFieldMiddleBossInfoGroupName( iBattleFieldBossGroupId_ );			
		}
	}
	return L"NoData";
}

bool CBattleFieldMiddleBossInfo::ParsingScriptFile ( IN KLuaManager& luaManager_ )
{

	LUA_GET_VALUE( luaManager_, "SPAWN_ID", m_uiMonsterSpawnID, 0 );

	if ( 0 != m_uiMonsterSpawnID )
	{
		if( true == luaManager_.BeginTable( "SPAWN_GROUP_ID" ) )
		{
			m_usMonsterSpawnMany = 0;
			UINT uiMonsterSpawnID = 0;
			while ( true == luaManager_.GetValue ( m_usMonsterSpawnMany + 1, uiMonsterSpawnID  ) )
			{		
				m_vecMonsterSpawnGroupID.push_back(uiMonsterSpawnID);
				m_usMonsterSpawnMany += 1;
			}
			luaManager_.EndTable();
		}		
		LUA_GET_VALUE( luaManager_, "SET_BOSS_GROUP_NAME", m_iBossGroupStringIndex, STR_ID_EMPTY );
		if ( STR_ID_EMPTY != m_iBossGroupStringIndex )
			m_wstrMainMonsterName = GET_STRING( m_iBossGroupStringIndex );
		else
			m_wstrMainMonsterName = L"NoData";
		LUA_GET_VALUE( luaManager_, "SPAWN_RATE", m_usSpawnRate, 0 );
		LUA_GET_VALUE( luaManager_, "RISK_VALUE", m_usRiskValue, 0 );
		LUA_GET_VALUE( luaManager_, "SHOW_BOSS_NAME", m_bIsShowBossGage, 0 );	
		LUA_GET_VALUE( luaManager_, "BOSS_GAUGE_HP_LINES", m_bIsSplitBossGage, 0 );
		return true;
	}

	return false;
}



#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
bool CX2BattleFieldManager::GetIsBossRaidFieldByFieldID( const UINT uiBattleFieldID_ ) const
{
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// ���� ��Ʋ�ʵ��� ������ ������ �´�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldID_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			return mItrBattleFieldDataPtr->second->GetIsBossRaidField();
		}
	}

	return false;
}
/** @function : GetIsBossRaidCurrentField
	@brief : ���� �ʵ尡 ���� ���̵� �ʵ����� �˻�
*/
bool CX2BattleFieldManager::GetIsBossRaidCurrentField() const
{
	if( CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
		return false;

	return GetIsBossRaidFieldByFieldID( GetBattleFieldIdWhereIam() );
}
/** @function : GetBossFieldPortalLineByFieldID
	@brief : ���� ���̵� �ʵ��� ��Ż�� ������ ���θ� �ε��� ���.
*/
const USHORT CX2BattleFieldManager::GetRaidFieldPortalLineByFieldID( const UINT uiBattleFieldID_ ) const
{	
	if ( false == m_mapBattleFieldDataPtr.empty() )
	{
		// ���� ��Ʋ�ʵ��� ������ ������ �´�.
		BattleFieldDataPtrMap::const_iterator mItrBattleFieldDataPtr = m_mapBattleFieldDataPtr.find( uiBattleFieldID_ );

		if ( mItrBattleFieldDataPtr != m_mapBattleFieldDataPtr.end() )
		{
			return mItrBattleFieldDataPtr->second->GetRaidFieldPortalLineIndex();
		}
	}

	return 0;
}
#endif // FIELD_BOSS_RAID