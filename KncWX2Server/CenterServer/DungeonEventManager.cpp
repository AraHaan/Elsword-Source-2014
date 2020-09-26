#include ".\DungeonEventManager.h"
#include "X2Data\XSLDungeonManager.h"

#include <dbg/dbg.hpp>
#include "BaseServer.h"
#include "NetError.h"

#include <iomanip>


//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT


ImplementRefreshSingleton( KDungeonEventManager );

KDungeonEventManager::KDungeonEventManager(void)
{
}

KDungeonEventManager::~KDungeonEventManager(void)
{
}

ImplToStringW( KDungeonEventManager )
{
	return stm_;
}

ImplementLuaScriptParser( KDungeonEventManager )
{
	lua_tinker::class_add<KDungeonEventManager>( GetLuaState(), "KDungeonEventManager" );
	lua_tinker::class_def<KDungeonEventManager>( GetLuaState(), "dump",						&KDungeonEventManager::Dump );

	lua_tinker::decl( GetLuaState(), "DungeonEventManager", this );
}

void KDungeonEventManager::Init()
{
	// �̺�Ʈ ���� ��������!
    SendToLogDB( DBE_GET_DUNGEON_EVENT_INFO_REQ );
}

void KDungeonEventManager::UpdateTimeDropEventMonsterInfo( IN const std::vector< KDungeonDropEventNpcInfo >& vecEventInfo )
{
    BOOST_TEST_FOREACH( const KDungeonDropEventNpcInfo&, kInfo, vecEventInfo )
	{
		m_mapTimeDropEventMonster.insert( std::make_pair( kInfo.m_tDropDate, kInfo ) );
	}
}

bool KDungeonEventManager::CheckTimeDropEventMonster( IN int iDungeonID, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT int& iEventUID, OUT int& iNpcID )
{
	// �����ϴ� ���Ͱ� �Ѹ����� ������ �̺�Ʈ ���͵� ������Ű�� �ʴ´�.
	if( kNot.m_mapNPCData.size() <= 0 )
	{
		START_LOG( cerr, L"�̺�Ʈ ���� ���� ������ ���� ������ ����?" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( kNot.m_mapNPCData.size() )
			<< END_LOG;
		return false;
	}

	// ���õ� �̺�Ʈ ���Ͱ� �ִ��� Ȯ��!
	if( m_mapTimeDropEventMonster.empty() == true )
		return false;

	CTime tCurTime = CTime::GetCurrentTime();

	// ������ NPC���� ���!
	{
		std::map< __int64, KDungeonDropEventNpcInfo >::iterator mit;
		for( mit = m_mapTimeDropEventMonster.begin(); mit != m_mapTimeDropEventMonster.end(); ++mit )
		{
			CTime tDropNpcTime = CTime( mit->first );
			if( tCurTime < tDropNpcTime )
			{
				return false;
			}

			iEventUID = mit->second.m_iEventUID;
			iNpcID = mit->second.m_iNpcID;
			break;
		}

		// �̺�Ʈ�� �ϴ� �����!
		if( mit != m_mapTimeDropEventMonster.end() )
		{
			m_mapTimeDropEventMonster.erase( mit );
		}
	}

	// ���° SubStage�� �����ų�� �����ϰ� ����!
	int iRandomSubStage = rand() % static_cast<int>(kNot.m_mapNPCData.size());

	std::map< int, KNPCList >::const_iterator mit;
	int iSubStageCount = 0;
	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
	{
		if( iRandomSubStage == iSubStageCount )
		{
			iRandomSubStage = mit->first;
			break;
		}
		++iSubStageCount;
	}

	KNPCUnitReq kNPCData;
	kNPCData.Init();

	//NPC DATA SET..
	kNPCData.m_NPCID			= iNpcID;
	kNPCData.m_KeyCode			= -1;
	kNPCData.m_bFocusCamera		= false;
	kNPCData.m_nStartPos		= SiCXSLDungeonManager()->GetStartPosByRandom( iDungeonID, iStageID, iRandomSubStage );
	kNPCData.m_bActive			= true;
	kNPCData.m_bShowGage		= true;
	kNPCData.m_AddPos.x			= 0.0f;
	kNPCData.m_AddPos.y			= 0.0f;
	kNPCData.m_AddPos.z			= 0.0f;
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_DUNGEON_NPC_LEVEL
	kNPCData.m_Level			= SiCXSLDungeonManager()->GetDungeonNpcLevel( iDungeonID );
#else
	kNPCData.m_Level			= SiCXSLDungeonManager()->GetDungeonMinLevel( iDungeonID );
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	kNPCData.m_bHasBossGage		= false;
	kNPCData.m_bShowBossName	= false;
	kNPCData.m_bShowSubBossName	= false;
	kNPCData.m_bSiegeMode		= false;
	kNPCData.m_bIsRight			= (rand()%2) ? true : false;
	kNPCData.m_bNoDrop			= false;
	kNPCData.m_fUnitScale		= -1.0f;

	//{{ 2008. 9. 12  ������	npc���� ����������?
	if( kNPCData.m_nStartPos != -1 )
	{
		kNot.m_mapNPCData[iRandomSubStage].m_NPCList.push_back( kNPCData );
	}
	//}}

	return true;
}

void KDungeonEventManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_TIME_DROP_MONSTER_EVENT
//}}


