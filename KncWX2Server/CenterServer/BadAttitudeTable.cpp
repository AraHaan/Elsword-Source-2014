#include "BadAttitudeTable.h"
#include "BaseServer.h"


//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

ImplementRefreshSingleton( KBadAttitudeTable );

KBadAttitudeTable::KBadAttitudeTable(void)
{
}

KBadAttitudeTable::~KBadAttitudeTable(void)
{
}

ImplToStringW( KBadAttitudeTable )
{
	stm_	<< L"----------[ KBadAttitudeTable ]----------" << std::endl
//			<< TOSTRINGW( m_mapHenirReward.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( KBadAttitudeTable )
{
	lua_tinker::class_add<KBadAttitudeTable>( GetLuaState(), "BadAttitudeTable" );
	lua_tinker::class_def<KBadAttitudeTable>( GetLuaState(), "dump",							&KBadAttitudeTable::Dump );
	lua_tinker::class_def<KBadAttitudeTable>( GetLuaState(), "AddBadAttitudeCutLinePoint",		&KBadAttitudeTable::AddBadAttitudeCutLinePoint_LUA );
	lua_tinker::class_def<KBadAttitudeTable>( GetLuaState(), "AddForceExitPoint",				&KBadAttitudeTable::AddForceExitPoint_LUA );


	lua_tinker::decl( GetLuaState(), "BadAttitudeTable", this );
}

void KBadAttitudeTable::AddBadAttitudeCutLinePoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapBadAttitudeCutLinePoint.find( eDungeonType );
	if( mit != m_mapBadAttitudeCutLinePoint.end() )	// �ִٸ� �ߺ��ε�...
	{
		START_LOG( cerr, L"(�ҷ�����)�����ϴ� ���� Ÿ�� �Դϴ�. �ߺ����� ���� �Ϸ��� �մϴ�. �����͸� ��ü �մϴ�." )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( mit->second )
			<< BUILD_LOG( iPoint )
			<< END_LOG;

		mit->second = iPoint;
	}
	else	// ������ �߰��Ѵ�.
	{
		m_mapBadAttitudeCutLinePoint.insert( std::make_pair( eDungeonType, iPoint ) );

		START_LOG( cout, L"���� Ÿ�Ժ� �ҷ����� ����Ʈ ���� ����!" )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( iPoint )
			<< END_LOG;
	}
}

void KBadAttitudeTable::AddForceExitPoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapForceExitPoint.find( eDungeonType );
	if( mit != m_mapForceExitPoint.end() )	// �ִٸ� �ߺ��ε�...
	{
		START_LOG( cerr, L"(���� �߰� ����)�����ϴ� ���� Ÿ�� �Դϴ�. �ߺ����� ���� �Ϸ��� �մϴ�. �����͸� ��ü �մϴ�." )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( mit->second )
			<< BUILD_LOG( iPoint )
			<< END_LOG;

		mit->second = iPoint;
	}
	else	// ������ �߰��Ѵ�.
	{
		m_mapForceExitPoint.insert( std::make_pair( eDungeonType, iPoint ) );

		START_LOG( cout, L"���� Ÿ�Ժ� ���� �߰� ����Ʈ ���� ����!" )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( iPoint )
			<< END_LOG;
	}
}

int KBadAttitudeTable::GetBadAttitudeCutLinePoint( CXSLDungeon::DUNGEON_TYPE eDungeonType )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapBadAttitudeCutLinePoint.find( eDungeonType );
	if( mit != m_mapBadAttitudeCutLinePoint.end() )
	{
		return mit->second;
	}

	return 1000;
}

int KBadAttitudeTable::GetForceExitPoint( CXSLDungeon::DUNGEON_TYPE eDungeonType )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapForceExitPoint.find( eDungeonType );
	if( mit != m_mapForceExitPoint.end() )
	{
		return mit->second;
	}

	return 1000;
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

