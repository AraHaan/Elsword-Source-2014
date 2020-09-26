#include ".\xslmapdata.h"
#include "XSLDungeon.h"
#include "NetError.h"
#include "Enum/Enum.h"



ImplementRefreshSingleton( CXSLMapData );

CXSLMapData::CXSLMapData(void)
{
}

CXSLMapData::~CXSLMapData(void)
{
}

ImplToStringW( CXSLMapData )
{
	stm_	<< L"----------[ Map Data ]----------" << std::endl
			<< TOSTRINGW( m_mapMapData.size() )
			<< TOSTRINGW( m_kDefaultMapData.m_iID )
			<< TOSTRINGW( m_mapFieldInfo.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLMapData )
{
	lua_tinker::class_add<CXSLMapData>( GetLuaState(), "CXSLMapData" );
	lua_tinker::class_def<CXSLMapData>( GetLuaState(), "AddMapData",		&CXSLMapData::AddMapData_LUA );
	//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<CXSLMapData>( GetLuaState(), "AddLocalMapInfo",	&CXSLMapData::AddLocalMapInfo_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	lua_tinker::decl( GetLuaState(), "MapData", this );
}

void CXSLMapData::AddMapData_LUA()
{
	KLuaManager lM( GetLuaState() );

	sMAP_DATA kMapData;

	LUA_GET_VALUE( lM, "ID", kMapData.m_iID, -1 );

	if( kMapData.m_iID == -1 )
	{
		std::map< int, sMAP_DATA >::reverse_iterator rmit;
		rmit = m_mapMapData.rbegin();

		int iBeforeID = -1;
		if( rmit != m_mapMapData.rend() )
			iBeforeID = rmit->second.m_iID;

		START_LOG( cerr, L"MAP ID PARSING FAIL.!" )
			<< BUILD_LOG( iBeforeID )
			<< END_LOG;
		return;
	}

	LUA_GET_VALUE( lM, "BEFORE",	kMapData.m_iBeforeID,	-1 );
	LUA_GET_VALUE( lM, "AFTER",		kMapData.m_iAfterID,	-1 );

	if( kMapData.m_iBeforeID == -1  ||  kMapData.m_iBeforeID == -1 )
	{
		START_LOG( cerr, L"BEFORE ID OR AFTER ID OF MAP PARSING FAIL.!" )
			<< BUILD_LOG( kMapData.m_iID )
			<< BUILD_LOG( kMapData.m_iBeforeID )
			<< BUILD_LOG( kMapData.m_iAfterID )
			<< END_LOG;
		return;
	}

	if( lM.BeginTable( L"REQUIRE_LEVEL" ) == S_OK )
	{
		LUA_GET_VALUE( lM, "MIN", kMapData.m_iRequireMinLevel, -1 );
		LUA_GET_VALUE( lM, "MAX", kMapData.m_iRequireMaxLevel, -1 );

		if( kMapData.m_iRequireMinLevel == -1  ||  kMapData.m_iRequireMaxLevel == -1 )
		{
			START_LOG( cerr, L" REQUIRE MIN LEVEL OR MAX LEVEL PARSING FAIL.!" )
				<< BUILD_LOG( kMapData.m_iID )
				<< BUILD_LOG( kMapData.m_iRequireMinLevel )
				<< BUILD_LOG( kMapData.m_iRequireMaxLevel )
				<< END_LOG;
			return;
		}

		lM.EndTable();
	}

	if( lM.BeginTable( L"REQUIRE_DUNGEON" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( lM.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
			{
				kMapData.m_vecRequireDungeonID.push_back( buf );
			}

			index++;
		}

		lM.EndTable();
	}

	m_mapMapData.insert( std::make_pair( kMapData.m_iID, kMapData ) );

	int iDefault = 0;
	LUA_GET_VALUE( lM, L"DEFAULT", iDefault, 0 );
	if( iDefault == 1 )
	{
		m_kDefaultMapData = kMapData;
	}
}

//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void CXSLMapData::AddLocalMapInfo_LUA( int iVillageMapID, int iDungeonMode, int iDungeonIDWithDif )
{
	if( iVillageMapID < 0  ||  iDungeonIDWithDif < 0 )
	{
		START_LOG( cerr, L"�߸��� �����Դϴ�!" )
			<< BUILD_LOG( iVillageMapID )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;
		return;
	}

	std::map< int, SLocalMapInfo >::iterator mit;
	mit = m_mapLocalMap.find( iVillageMapID );
	if( mit == m_mapLocalMap.end() )
	{
		SLocalMapInfo sLocalMapInfo;
		sLocalMapInfo.m_vecDungeonInfo.push_back( KDungeonGameSimpleInfo( iDungeonMode, iDungeonIDWithDif ) );
		m_mapLocalMap.insert( std::make_pair( iVillageMapID, sLocalMapInfo ) );
	}
	else
	{
        mit->second.m_vecDungeonInfo.push_back( KDungeonGameSimpleInfo( iDungeonMode, iDungeonIDWithDif ) );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

int	CXSLMapData::GetBeforeMapID( int iMapID )
{
	const sMAP_DATA* pMapData = GetMapData( iMapID );
	if( pMapData == NULL )
	{
		return GetDefaultMapID();
	}

	return pMapData->m_iBeforeID;
}

//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
int CXSLMapData::GetPossibleEnterVillageMapID( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapClearInfo )
{
	int iPossibleMapID = GetDefaultMapID();

	std::map< int, sMAP_DATA >::const_iterator mit;
	for( mit = m_mapMapData.begin(); mit != m_mapMapData.end(); ++mit )
	{
		// �������� �ƴϸ� �н�!
		if( SEnum::IsVillageMapID( mit->first ) == false )
			continue;

		// �ִ��� ���� ������ ������ ã��!
		int iDummy1 = 0;
		int iDummy2 = 0;
		if( CheckRequireData( mit->first, ucLevel, mapClearInfo, iDummy1, iDummy2 ) == false )
			continue;

		// ���� ���� mapid�� ����!
		iPossibleMapID = mit->first;
	}

	return iPossibleMapID;
}
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
//}}

bool CXSLMapData::CheckMapData( const int iMapID )
{
	const sMAP_DATA* pMapData = GetMapData( iMapID );

	if( pMapData != NULL )
	{
		return true;
	}

	START_LOG( cerr, L"�ʵ����� ������ �̻�.!" )
		<< END_LOG;

	return false;
}

//{{ 2012. 09. 21   ��μ�   ���� ���� ���� �޽��� ����
#ifdef SERV_ENTER_VILLAGE_ERROR
bool CXSLMapData::CheckRequireData( IN const int iMapID, IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapClearInfo, OUT int& iLimitLevel, OUT int& iRequireDungeonID )
{
	SET_ERROR( NET_OK );

	const sMAP_DATA* pMapData = GetMapData( iMapID );
	if( pMapData == NULL )
	{
		START_LOG( cerr, L"�ʵ����� ������ �̻�.!" )
			<< END_LOG;

		SET_ERROR( ERR_FIELD_00 );
		return false;
	}

	if( static_cast<int>(ucLevel) < pMapData->m_iRequireMinLevel )
	{
		START_LOG( clog, L"���� ���� ���� ����." )
			<< BUILD_LOG( static_cast<int>(ucLevel) )
			<< BUILD_LOG( pMapData->m_iRequireMinLevel )
			<< BUILD_LOG( pMapData->m_iRequireMaxLevel )
			;

		iLimitLevel = pMapData->m_iRequireMinLevel;

		SET_ERROR( ERR_FIELD_05 );
		return false;
	}

// ���� ���� ���� ���� ����
	std::map<int,KDungeonClearInfo>::const_iterator mit;
	for( int i = 0; i < static_cast<int>(pMapData->m_vecRequireDungeonID.size()); ++i )
	{
		mit = mapClearInfo.find( pMapData->m_vecRequireDungeonID[i] );
		if( mit == mapClearInfo.end() )
		{
			START_LOG( clog, L"Ŭ���� �ؾ��� ������ ������." )
				<< BUILD_LOG( pMapData->m_vecRequireDungeonID[i] )
				;

			iRequireDungeonID = pMapData->m_vecRequireDungeonID[i];
				 
			SET_ERROR( ERR_FIELD_06 );
			return false;
		}
	}

	return true;
}
#else
bool CXSLMapData::CheckRequireData( int iMapID, u_char ucLevel, std::map< int, KDungeonClearInfo >& mapClearInfo )
{
	SET_ERROR( NET_OK );

	const sMAP_DATA* pMapData = GetMapData( iMapID );

	if( pMapData == NULL )
	{
		START_LOG( cerr, L"�ʵ����� ������ �̻�.!" )
			<< END_LOG;

		SET_ERROR( ERR_FIELD_00 );
		return false;
	}

	if( static_cast<int>(ucLevel) < pMapData->m_iRequireMinLevel )
	{
		START_LOG( clog, L"���� ���� ���� ����." )
			<< BUILD_LOG( static_cast<int>(ucLevel) )
			<< BUILD_LOG( pMapData->m_iRequireMinLevel )
			<< BUILD_LOG( pMapData->m_iRequireMaxLevel )
			;

		SET_ERROR( ERR_FIELD_05 );
		return false;
	}

	std::map<int,KDungeonClearInfo>::iterator mit;
	for( int i = 0; i < static_cast<int>(pMapData->m_vecRequireDungeonID.size()); ++i )
	{
		mit = mapClearInfo.find( pMapData->m_vecRequireDungeonID[i] );
		if( mit == mapClearInfo.end() )
		{
			START_LOG( clog, L"Ŭ���� �ؾ��� ������ ������." )
				<< BUILD_LOG( pMapData->m_vecRequireDungeonID[i] )
				;

			SET_ERROR( ERR_FIELD_06 );
			return false;
		}
	}

	return true;
}
#endif SERV_ENTER_VILLAGE_ERROR
//}}

const CXSLMapData::sMAP_DATA* CXSLMapData::GetMapData( const int iMapID )
{
	std::map< int,sMAP_DATA >::const_iterator mit;
	mit = m_mapMapData.find( iMapID );
	if( mit == m_mapMapData.end() )
	{
		START_LOG( cerr, L"MAP DATA SEARCHING FAIL.!" )
			<< BUILD_LOG( iMapID )
			<< END_LOG;
		return NULL;		
	}

	return &mit->second;
}

//{{ 2008. 11. 5  ������	�ʵ� ����
void CXSLMapData::UpdateFieldInfo( int iMapID, UidType iFieldUID )
{
	m_mapFieldInfo.insert( std::make_pair( iMapID, iFieldUID ) );
}

bool CXSLMapData::GetFieldUID( int iMapID, UidType& iFieldUID )
{
	std::map< int, UidType >::const_iterator mit;
	mit = m_mapFieldInfo.find( iMapID );
	if( mit == m_mapFieldInfo.end() )
		return false;

	iFieldUID = mit->second;
	return true;
}

bool CXSLMapData::VerifyMapID( int iMapID )
{
	std::map< int, UidType >::const_iterator mit;
	mit = m_mapFieldInfo.find( iMapID );
	if( mit == m_mapFieldInfo.end() )
		return false;
	
	return true;
}
//}}

void CXSLMapData::PackingLastPosition( IN const int iMapID, IN const u_char ucLastTouchLineIndex, IN const u_short usLastPosValue, OUT int& iLastPosition )
{
	// DB�� 4byte�� �����ؼ� ��ġ������ �����Ѵ�; mapid(1byte), lasttouchlineindex(1byte), lastposvalue(2byte)
	int iTempMapID = iMapID;
	if( iTempMapID >= 20000 )	// village
	{
		iTempMapID -= 20000;
	}
	else					// dungeon gate
	{
		iTempMapID -= 10000;
		iTempMapID += 1;
		iTempMapID = -iTempMapID;
	}

	int iLastPosData = 0;
	char* pChar = (char*) &iLastPosData;
	*pChar = (char) iTempMapID;

	++pChar;
	u_char* pUnsignedChar = (u_char*) pChar;
	*pUnsignedChar = ucLastTouchLineIndex;

	++pChar;
	u_short* pUnsignedShort = (u_short*) pChar;
	*pUnsignedShort = usLastPosValue;

	// packing�� lastposition����
	iLastPosition = iLastPosData;
}

void CXSLMapData::UnpackingLastPosition( IN const int iLastPosition, OUT UINT& uiMapID, OUT u_char& ucLastTouchLineIndex, OUT u_short& usLastPosValue )
{
	// DB�� 4byte�� ����Ǿ� �ִ� ��ġ ������ ���� �Ѵ�; mapid(1byte), lasttouchlineindex(1byte), lastposvalue(2byte)
	const int iPackedPositionData = iLastPosition;
	char* pChar = (char*) &iLastPosition;
	int iActualMapID = (int) *pChar;
	if( iActualMapID >= 0 )
	{
		iActualMapID += 20000;			// village
	}
	else
	{
		iActualMapID = -iActualMapID;
		iActualMapID -= 1;
		iActualMapID += 10000;			// dungeon gate
	}

	++pChar;
	u_short* pUnsignedChar = (u_short*) pChar;
	ucLastTouchLineIndex = (u_char) *pUnsignedChar;

	++pChar;
	u_short* pUnsignedShort = (u_short*) pChar;
	usLastPosValue = *pUnsignedShort;

	// �Ľ̵� �� �ޱ�
	uiMapID = static_cast<UINT>(iActualMapID);
}

int CXSLMapData::GetMapID( int iDungeonID )
{
	if( CXSLDungeon::IsRubenDungeon( iDungeonID ) )
	{
		return SEnum::VMI_RUBEN;
	}
	else if( CXSLDungeon::IsElderDungeon( iDungeonID ) )
	{
		return SEnum::VMI_ELDER;
	}
	else if( CXSLDungeon::IsBesmaDungeon( iDungeonID ) )
	{
		return SEnum::VMI_BESMA;
	}
	else if( CXSLDungeon::IsAlteraDungeon( iDungeonID ) )
	{
		return SEnum::VMI_ALTERA;
	}
	else if( CXSLDungeon::IsPeitaDungeon( iDungeonID ) )
	{
		return SEnum::VMI_PEITA;
	}
	else if( CXSLDungeon::IsVelderDungeon( iDungeonID ) )
	{
		return SEnum::VMI_VELDER;
	}
	else if( CXSLDungeon::IsHamelDungeon( iDungeonID ) )
	{
		return SEnum::VMI_HAMEL;
	}
	//{{ 2012. 10. 29	�ڼ���	���� ���� �߰�
#ifdef SERV_VILLAGE_SANDER
	else if( CXSLDungeon::IsSanderDungeon( iDungeonID ) )
	{
		return SEnum::VMI_SANDER;
	}
#endif SERV_VILLAGE_SANDER
	//}}
	else
	{
		START_LOG( cerr, L"DungeonID���� �̻��մϴ�." )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
	}

	return SEnum::VMI_INVALID;
}

bool CXSLMapData::IsDungeonGate( int iMapID )
{
	switch( iMapID )
	{
	case SEnum::VMI_DUNGEON_GATE_RUBEN:			
	case SEnum::VMI_DUNGEON_GATE_ELDER:			
	case SEnum::VMI_DUNGEON_GATE_BESMA:			
	case SEnum::VMI_DUNGEON_GATE_ALTERA:
	case SEnum::VMI_PEITA:
	case SEnum::VMI_DUNGEON_GATE_VELDER:
	case SEnum::VMI_DUNGEON_GATE_HAMEL:
		return true;
	}

	return false;
}

//{{ 2012. 05. 16	��μ�       �ɸ��� ���ý� ������ ���� ��ID �� ��ȯ
#ifdef SERV_SELECT_UNIT_CHANGE_MAP_ID
void CXSLMapData::CheckMapID( IN unsigned char& ucLevel, IN OUT int& iMapID )
{
	if( ucLevel < 7 )
	{
		switch( iMapID )
		{
		case SEnum::VMI_ELDER:
		case SEnum::VMI_BESMA:
		case SEnum::VMI_ALTERA:
		case SEnum::VMI_VELDER:
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_DUNGEON_GATE_ELDER:
		case SEnum::VMI_DUNGEON_GATE_BESMA:
		case SEnum::VMI_DUNGEON_GATE_ALTERA:
		case SEnum::VMI_PEITA:
		case SEnum::VMI_DUNGEON_GATE_VELDER:
		case SEnum::VMI_DUNGEON_GATE_HAMEL:
			{
				iMapID = SEnum::VMI_RUBEN;
			}
			break;
		}
	}
	else if( ucLevel < 15 )
	{
		switch( iMapID )
		{
		case SEnum::VMI_BESMA:
		case SEnum::VMI_ALTERA:
		case SEnum::VMI_VELDER:
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_DUNGEON_GATE_BESMA:
		case SEnum::VMI_DUNGEON_GATE_ALTERA:
		case SEnum::VMI_PEITA:
		case SEnum::VMI_DUNGEON_GATE_VELDER:
		case SEnum::VMI_DUNGEON_GATE_HAMEL:
			{
				iMapID = SEnum::VMI_ELDER;
			}
			break;
		}
	}
	else if( ucLevel < 27 )
	{
		switch( iMapID )
		{
		case SEnum::VMI_ALTERA:
		case SEnum::VMI_VELDER:
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_DUNGEON_GATE_ALTERA:
		case SEnum::VMI_PEITA:
		case SEnum::VMI_DUNGEON_GATE_VELDER:
		case SEnum::VMI_DUNGEON_GATE_HAMEL:
			{
				iMapID = SEnum::VMI_BESMA;
			}
			break;
		}
	}
	else if( ucLevel < 34 )
	{
		switch( iMapID )
		{
		case SEnum::VMI_VELDER:
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_PEITA:
		case SEnum::VMI_DUNGEON_GATE_VELDER:
		case SEnum::VMI_DUNGEON_GATE_HAMEL:
			{
				iMapID = SEnum::VMI_ALTERA;
			}
			break;
		}
	}
	else if( ucLevel < 41 )
	{
		switch( iMapID )
		{
		case SEnum::VMI_VELDER:
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_DUNGEON_GATE_VELDER:
		case SEnum::VMI_DUNGEON_GATE_HAMEL:
			{
				iMapID = SEnum::VMI_PEITA;
			}
			break;
		}
	}
	else if( ucLevel < 48 )
	{
		switch( iMapID )
		{
		case SEnum::VMI_HAMEL:
		case SEnum::VMI_DUNGEON_GATE_HAMEL:
			{
				iMapID = SEnum::VMI_VELDER;
			}
			break;
		}
	}
	else
	{
		// 50 ���ʹ� �����Ӱ� ���� �� �� �����ϱ�...
		//START_LOG( cerr, L"������ �� ���� ���� �ƴѰ�?!" )
		//	<< BUILD_LOG( static_cast<int>(ucLevel) );
	}
}
#endif SERV_SELECT_UNIT_CHANGE_MAP_ID
//}}

//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
const CXSLMapData::SLocalMapInfo* CXSLMapData::GetLocalMapList( IN const int iVillageMapID ) const
{
	std::map< int, SLocalMapInfo >::const_iterator mit;
	mit = m_mapLocalMap.find( iVillageMapID );
	if( mit == m_mapLocalMap.end() )
	{
		START_LOG( cerr, L"��ϵ��� ���� ��ID�Դϴ�!" )
			<< BUILD_LOG( iVillageMapID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 10. 31	�ڼ���	�ں� �ͽ������� Ƽ�� �߰�
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
int CXSLMapData::CheckCOBOExpressTicketMapID( IN int iMapID )
{
	return lua_tinker::call<int>( GetLuaState(), "CheckCOBOExpressTicketMapID", iMapID );
}

int CXSLMapData::ComputeCOBOExpressTicketCost( IN int iSrcMapID, IN int iDesMapID, IN u_char ucLevel )
{
	return lua_tinker::call<int>( GetLuaState(), "ComputeCOBOExpressTicketCost", iSrcMapID, iDesMapID, ucLevel );
}
#endif SERV_ADD_COBO_EXPRESS_TICKET
//}}
