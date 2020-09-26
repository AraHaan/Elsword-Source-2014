#pragma once

#include "RefreshSingleton.h"
#include <set>
#include <map>
#include <KncUtil.h>
#include "Lottery.h"
#include "CommonPacket.h"
#include "X2Data/XSLDungeon.h"

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM


class KBadAttitudeTable
{
	DeclareRefreshSingleton( KBadAttitudeTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KBadAttitudeTable(void);
	~KBadAttitudeTable(void);

	// ���
	void AddBadAttitudeCutLinePoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint );
	void AddForceExitPoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint );

	// ���� Ÿ���� �ҷ� ���� �Ǵ� ����
	int GetBadAttitudeCutLinePoint( CXSLDungeon::DUNGEON_TYPE eDungeonType );

	// ���� Ÿ���� ���� �Ǵ� �߰� ����
	int GetForceExitPoint( CXSLDungeon::DUNGEON_TYPE eDungeonType );

private:

	std::map< CXSLDungeon::DUNGEON_TYPE, int >			m_mapBadAttitudeCutLinePoint;		// ���� Ÿ�Ժ� , �ҷ� ������ �ǴܵǴ� ����
	std::map< CXSLDungeon::DUNGEON_TYPE, int >			m_mapForceExitPoint;				// ���� Ÿ�Ժ� , ���� �Ǵ� �߰� ����
};

DefRefreshSingletonInline( KBadAttitudeTable );



#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}