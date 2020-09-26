#include ".\XSLCharacterStatTable.h"
#include "neterror.h"
#include "XSLItemManager.h"
#include <math.h>


//{{ 2013. 05. 20	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05


ImplementRefreshSingleton( CXSLCharacterStatTable );

CXSLCharacterStatTable::CXSLCharacterStatTable(void)
{
}

CXSLCharacterStatTable::~CXSLCharacterStatTable(void)
{	
}

ImplToStringW( CXSLCharacterStatTable )
{
	stm_	<< L"----------[ Character Stat Table ]----------" << std::endl
			<< TOSTRINGW( m_mapCharStatDataTable.size() )
			<< TOSTRINGW( m_mapClassStatDataTable.size() )
			<< TOSTRINGW( m_mapEqipStatDataTable.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLCharacterStatTable )
{
	lua_tinker::class_add<CXSLCharacterStatTable>( GetLuaState(), "CXSLCharacterStatTable" );
	//lua_tinker::class_def<CXSLCharacterStatTable>( GetLuaState(), "AddSocketData_LUA",		 &CXSLCharacterStatTable::AddSocketData_LUA );
	lua_tinker::class_def<CXSLCharacterStatTable>( GetLuaState(), "dump",					 &CXSLCharacterStatTable::Dump );

	lua_tinker::decl( GetLuaState(), "g_pCX2SocketItem", this );
}

bool CXSLCharacterStatTable::OpenScriptFile_CharacterStatTable()
{
	KLuaManager luaManager;

	KAutoPath kautoPaht;
	std::string strFile;

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

	strFile = "CharacterStatTable.lua";
	kautoPaht.GetPullPath( strFile );

	if( luaManager.DoFile( strFile.c_str() ) == E_FAIL )
	{
		START_LOG( cerr, strFile )
			<< END_LOG;

		return false;
	}

	// ��� ���� ���� �� �ɷ�ġ
	if( luaManager.BeginTable( L"EQIP_POSITION_RELATIVE_STAT_TABLE" ) != E_FAIL )
	{
		int iTableIndex = 1;
		while( luaManager.BeginTable( iTableIndex  ) != E_FAIL )
		{
			CXSLUnit::EQIP_POSITION eEqipPosition = CXSLUnit::EP_NONE;

			LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eEqipPosition,		CXSLUnit::EQIP_POSITION,		CXSLUnit::EP_NONE );
			if( CXSLUnit::EP_NONE == eEqipPosition )
			{
				luaManager.EndTable();
				START_LOG( cerr, L"������ ���� ��ġ ������ �̻��մϴ�." )
					<< END_LOG;
				break;
			}

			KCharStatData kStatData;
			LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		kStatData.m_fAtkPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		kStatData.m_fDefPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		kStatData.m_fAtkMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		kStatData.m_fDefMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_HP,				kStatData.m_fBaseHP,		0.f );

			m_mapEqipStatDataTable.insert( std::make_pair( eEqipPosition, kStatData ) );

			++iTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	// ĳ���ͺ� �ɷ�ġ
	if( luaManager.BeginTable( L"CHARACTER_RELATIVE_STAT_TABLE" ) != E_FAIL )
	{
		int iTableIndex = 1;
		while( luaManager.BeginTable( iTableIndex  ) != E_FAIL )
		{
			CXSLUnit::UNIT_TYPE eUnitType = CXSLUnit::UT_NONE;

			LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitType,		CXSLUnit::UNIT_TYPE,		CXSLUnit::UT_NONE );
			if( CXSLUnit::UC_NONE == eUnitType )
			{
				luaManager.EndTable();
				START_LOG( cerr, L"���� Ÿ�� ������ �̻��մϴ�." )
					<< END_LOG;
				break;
			}

			KCharStatData kStatData;
			LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		kStatData.m_fAtkPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		kStatData.m_fDefPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		kStatData.m_fAtkMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		kStatData.m_fDefMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_HP,				kStatData.m_fBaseHP,		0.f );

			m_mapCharStatDataTable.insert( std::make_pair( eUnitType, kStatData ) );

			++iTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	// 2�� ������ �ɷ�ġ( 2�������� �ƴ϶�� ĳ���ͺ� �ɷ�ġ ��� )
	if( luaManager.BeginTable( L"SECOND_CLASS_RELATIVE_STAT_TABLE" ) != E_FAIL )
	{
		int iTableIndex = 1;
		while( luaManager.BeginTable( iTableIndex  ) != E_FAIL )
		{
			CXSLUnit::UNIT_CLASS eUnitClass = CXSLUnit::UC_NONE;

			LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitClass,		CXSLUnit::UNIT_CLASS,		CXSLUnit::UC_NONE );
			if( CXSLUnit::UT_NONE == eUnitClass )
			{
				luaManager.EndTable();
				START_LOG( cerr, L"���� Ŭ���� ������ �̻��մϴ�." )
					<< END_LOG;
				break;
			}

			KCharStatData kStatData;
			LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		kStatData.m_fAtkPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		kStatData.m_fDefPhysic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		kStatData.m_fAtkMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		kStatData.m_fDefMagic,		0.f );
			LUA_GET_VALUE( luaManager,		TDT_HP,				kStatData.m_fBaseHP,		0.f );

			m_mapClassStatDataTable.insert( std::make_pair( eUnitClass, kStatData ) );

			++iTableIndex;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}	

	return true;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetUnitTypeStatData( IN const int iUnitType ) const
{
	std::map< int, KCharStatData >::const_iterator mit;
	mit = m_mapCharStatDataTable.find( iUnitType );
	if( mit == m_mapCharStatDataTable.end() )
	{
		START_LOG( cerr, L"���� Ÿ�� ������ �����ϴ�." )
			<< BUILD_LOG( iUnitType )
			<< END_LOG;
		return NULL;
	}
	
	return &mit->second;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetUnitClassStatData( IN const int iUnitClass ) const
{
	std::map< int, KCharStatData >::const_iterator mit;
	mit = m_mapClassStatDataTable.find( iUnitClass );
	if( mit == m_mapClassStatDataTable.end() )
	{
		START_LOG( cerr, L"���� Ŭ���� ������ �����ϴ�." )
			<< BUILD_LOG( iUnitClass )
			<< END_LOG;
		return NULL;
	}

	return &mit->second;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetEqipStatData( IN const int iEqipPosition ) const
{
	std::map< int, KCharStatData >::const_iterator mit;
	mit = m_mapEqipStatDataTable.find( iEqipPosition );
	if( mit == m_mapEqipStatDataTable.end() )
	{
		START_LOG( cerr, L"���� ��ġ ������ �����ϴ�." )
			<< BUILD_LOG( iEqipPosition )
			<< END_LOG;
		return NULL;
	}

	return &mit->second;
}

const CXSLCharacterStatTable::KCharStatData* CXSLCharacterStatTable::GetAData( IN const int iUnitType, IN const int iUnitClass ) const
{
	if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(iUnitClass) ) == true )
	{
		return GetUnitClassStatData( iUnitClass );
	}
	else
	{
		return GetUnitTypeStatData( iUnitType );
	}
}

bool CXSLCharacterStatTable::CalcCommonItemStat( IN const CXSLItem::ItemTemplet* pItemTemplet, 
												 IN const char cUnitType, 
												 IN const char cUnitClass, 
												 IN const KStatRelationLevel& kAddStatRelLevel,
												 OUT KStat& kCommonItemStat ) const
{
	//A = ���� ĳ���� �Ǵ� ���� 2�� ���� ĳ������ �⺻ �ɷ�ġ(���̺� ����) unitclass, unittype
	const KCharStatData* pAData = GetAData( cUnitType, cUnitClass );
	if( IS_NULL( pAData ) )
	{
		START_LOG( cerr, L"A ������ ������ ���߽��ϴ�." )
			<< BUILD_LOGc( cUnitType )
			<< BUILD_LOGc( cUnitClass )
			<< END_LOG;
		return false;
	}

	//B = �⺻ ĳ������ �ɷ�ġ(���̺� ����) unittype
	const KCharStatData* pBData = GetUnitTypeStatData( cUnitType );
	if( IS_NULL( pBData ) )
	{
		START_LOG( cerr, L"B ������ ������ ���߽��ϴ�." )
			<< BUILD_LOGc( cUnitType )
			<< END_LOG;
		return false;
	}

	//C = ������ �ɷ�ġ ���� ���(���̺� ����) eqip_position
	const KCharStatData* pCData = GetEqipStatData( pItemTemplet->m_EqipPosition );
	if( IS_NULL( pBData ) )
	{
		START_LOG( cerr, L"C ������ ������ ���߽��ϴ�." )
			<< BUILD_LOGc( pItemTemplet->m_EqipPosition )
			<< END_LOG;
		return false;
	}

	KStatRelationLevel kStatRelLevel = kAddStatRelLevel;
	kStatRelLevel.m_fBaseHPRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byBaseHPRelLV);
	kStatRelLevel.m_fAtkPhysicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byAtkPhysicRelLV);
	kStatRelLevel.m_fAtkMagicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byAtkMagicRelLV);
	kStatRelLevel.m_fDefPhysicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byDefPhysicRelLV);
	kStatRelLevel.m_fDefMagicRelLV	+= static_cast<float>(pItemTemplet->m_kStatRelationLevel.m_byDefMagicRelLV);

	// stat���
	kCommonItemStat.m_iBaseHP	 = CalcHPStat( pAData->m_fBaseHP, 
											   pBData->m_fBaseHP, 
											   pCData->m_fBaseHP, 
											   static_cast<float>(pItemTemplet->m_UseLevel), 
											   kStatRelLevel.m_fBaseHPRelLV
											   );

	kCommonItemStat.m_iAtkPhysic = CalcAtkDefStat( pAData->m_fAtkPhysic, 
													pBData->m_fAtkPhysic, 
													pCData->m_fAtkPhysic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fAtkPhysicRelLV
													);

	kCommonItemStat.m_iAtkMagic = CalcAtkDefStat( pAData->m_fAtkMagic, 
													pBData->m_fAtkMagic, 
													pCData->m_fAtkMagic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fAtkMagicRelLV
													);

	kCommonItemStat.m_iDefPhysic = CalcAtkDefStat( pAData->m_fDefPhysic, 
													pBData->m_fDefPhysic, 
													pCData->m_fDefPhysic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fDefPhysicRelLV
													);

	kCommonItemStat.m_iDefMagic = CalcAtkDefStat( pAData->m_fDefMagic, 
													pBData->m_fDefMagic, 
													pCData->m_fDefMagic, 
													static_cast<float>(pItemTemplet->m_UseLevel), 
													kStatRelLevel.m_fDefMagicRelLV
													);

	return true;
}

int	CXSLCharacterStatTable::CalcAtkDefStat( IN const float fSecondJobStat, IN const float fUnitTypeStat, IN const float fEquipPosStat, IN const float fUseLevel, IN const float fItemAddLv ) const
{
	//�繰��/���� ���ݷ� + ����/���� ���� ������ �ɷ�ġ ���� ����
	//= ROUND((C*[{A+A*(D+E-1)*0.15}-[(B*0.8)+0.15*{B*(D+E-1)*0.15}]]),0)   --�Ҽ��� 1�ڸ� �ݿø� ����.	

	// 2������ �ɷ�ġ ���
	float fCal1 = fSecondJobStat + fSecondJobStat * ( fUseLevel + fItemAddLv - 1.f ) * 0.15f;

	// �⺻ ĳ���� �ɷ�ġ ���
	float fCal2 = fUnitTypeStat * 0.8f;
	float fCal3 = fUnitTypeStat * ( fUseLevel + fItemAddLv - 1.f ) * 0.0225f;

	// ������ ���� �ɷ�ġ ���
	float fCal4 = fEquipPosStat * ( fCal1 - fCal2 - fCal3 );

	// �ݿø��ؼ� ��ȯ
	return static_cast<int>( ::floor( fCal4 + 0.5f ) );
}

int	CXSLCharacterStatTable::CalcHPStat( IN const float fSecondJobStat, IN const float fUnitTypeStat, IN const float fEquipPosStat, IN const float fUseLevel, IN const float fItemAddLv ) const
{
	//��HP ������ �ɷ�ġ ���� ����
	//= ROUND((C*(E*[{A+A*(D-1)*0.15}-[(B*0.8)+0.15*{B*(D-1)*0.15}]])),0)   --�Ҽ��� 1�ڸ� �ݿø� ����.

	// 2������ �ɷ�ġ ���
	float fCal1 = fSecondJobStat + fSecondJobStat * ( fUseLevel - 1.f ) * 0.15f;

	// �⺻ ĳ���� �ɷ�ġ ���
	float fCal2 = fUnitTypeStat;
	float fCal3 = fUnitTypeStat * ( fUseLevel - 1.f ) * 0.15f;

	// ������ ���� �ɷ�ġ ���
	float fCal4 = ( fItemAddLv / 10 ) * ( fCal1 - fCal2 - fCal3 );

	// ������ ���� �ɷ�ġ ���
	float fCal5 = fEquipPosStat * fCal4;

	// �ݿø��ؼ� ��ȯgpd
	return static_cast<int>( ::floor( fCal5 + 0.5 ) );
}


#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}


