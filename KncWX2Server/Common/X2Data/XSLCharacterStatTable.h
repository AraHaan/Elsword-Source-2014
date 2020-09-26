#pragma once

#include "RefreshSingleton.h"
#include <map>
#include "CommonPacket.h"
#include "XSLItemManager.h"


//{{ 2013. 05. 20	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05


class CXSLCharacterStatTable
{
	DeclareRefreshSingleton( CXSLCharacterStatTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	// ��ũ��Ʈ �Ľ� �� �� ����ϴ� enum
	enum TABLE_DATA_TYPE
	{
		TDT_TYPE = 1,
		TDT_ATK_PHYSIC,
		TDT_DEF_PHYSIC,
		TDT_ATK_MAGIC,
		TDT_DEF_MAGIC,
		TDT_HP,
	};


	struct KCharStatData
	{
		float m_fBaseHP;
		float m_fAtkPhysic;
		float m_fAtkMagic;
		float m_fDefPhysic;
		float m_fDefMagic;

		KCharStatData()
		{
			m_fBaseHP			= 0;
			m_fAtkPhysic		= 0;
			m_fAtkMagic			= 0;
			m_fDefPhysic		= 0;
			m_fDefMagic			= 0;
		}
	};

public:
	CXSLCharacterStatTable(void);
	virtual ~CXSLCharacterStatTable(void);

	// for lua
	bool	OpenScriptFile_CharacterStatTable();

	const KCharStatData* GetUnitTypeStatData( IN const int iUnitType ) const;
	const KCharStatData* GetUnitClassStatData( IN const int iUnitClass ) const;
	const KCharStatData* GetEqipStatData( IN const int iEqipPosition ) const;

	const KCharStatData* GetAData( IN const int iUnitType, IN const int iUnitClass ) const;

	bool	CalcCommonItemStat( IN const CXSLItem::ItemTemplet* pItemTemplet, 
								IN const char cUnitType, 
								IN const char cUnitClass, 
								IN const KStatRelationLevel& kAddStatRelLevel,
								OUT KStat& kCommonItemStat ) const;

	int		CalcAtkDefStat( IN const float fSecondJobStat, IN const float fUnitTypeStat, IN const float fEquipPosStat, IN const float fUseLevel, IN const float fItemAddLv ) const;
	int		CalcHPStat( IN const float fSecondJobStat, IN const float fUnitTypeStat, IN const float fEquipPosStat, IN const float fUseLevel, IN const float fItemAddLv ) const;

private:
	std::map< int, KCharStatData >			m_mapCharStatDataTable;  // ĳ���ͺ� ���� ���̺�
	std::map< int, KCharStatData >			m_mapClassStatDataTable; // ������ ���� ���̺�(2�� ������ �ƴ϶�� ĳ���ͺ� �ɷ�ġ ���)
	std::map< int, KCharStatData >			m_mapEqipStatDataTable;  // ��� ���� ������ ���� ���̺�
};

DefRefreshSingletonInline( CXSLCharacterStatTable );


#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}


